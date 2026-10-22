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

#ವ್ಯಾಖ್ಯಾನಿಸು BUF_CHUNK_ಗಾತ್ರ 4096

ರಚನೆ buf {
    ಅಕ್ಷರ *buf;
    ಗಾತ್ರ_t ಗಾತ್ರ_2;
    ಗಾತ್ರ_t alloc;
};

#ವ್ಯಾಖ್ಯಾನಿಸು xkb_abs(n) _Generic((n),                \
                            ಪೂರ್ಣಾಂಕ: ನಿರಪೇಕ್ಷ,           \
                            ದೀರ್ಘ ಪೂರ್ಣಾಂಕ: labs,     \
                            ಪೂರ್ವನಿಯೋಜಿತ: llabs )((n))

ಸ್ಥಿರ bool
do_realloc(ರಚನೆ buf *buf, ಗಾತ್ರ_t at_ಕನಿಷ್ಠ)
{
    buf->alloc += BUF_CHUNK_ಗಾತ್ರ;
    ಆದರೆ (at_ಕನಿಷ್ಠ >= BUF_CHUNK_ಗಾತ್ರ)
        buf->alloc += at_ಕನಿಷ್ಠ;

    ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ new = realloc(buf->buf, buf->alloc);
    ಆದರೆ (!new)
        ಹಿಂತಿರುಗಿ false;

    buf->buf = new;
    ಹಿಂತಿರುಗಿ true;
}

ATTR_PRINTF(2, 3) ಸ್ಥಿರ bool
check_write_buf(ರಚನೆ buf *buf, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)
{
    va_list ವಾದಗಳು;
    ಪೂರ್ಣಾಂಕ printed;
    ಗಾತ್ರ_t ಲಭ್ಯ;

    ಲಭ್ಯ = buf->alloc - buf->ಗಾತ್ರ_2;
    /* Try to write to the buffer and get the required min size (without the
     * final null byte).
     * C11 states that if `available` is 0, then the pointer may be null. */
    ಪ್ರತಿಜ್ಞಾ(buf->buf || ಲಭ್ಯ == 0);
    va_ಆರಂಭ(ವಾದಗಳು, fmt);
    printed = vsnprintf(buf->buf ? buf->buf + buf->ಗಾತ್ರ_2 : buf->buf, ಲಭ್ಯ,
                        fmt, ವಾದಗಳು);
    va_ಕೊನೆ(ವಾದಗಳು);

    ಆದರೆ (printed < 0) {
        /* Some error */
        ಹೋಗು err;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((ಗಾತ್ರ_t)printed >= ಲಭ್ಯ) {
        /* Not enough space: need to realloc */
        ಆದರೆ (!do_realloc(buf, (ಗಾತ್ರ_t)printed + 1))
            ಹೋಗು err;

        /* The buffer has enough space now. */
        ಲಭ್ಯ = buf->alloc - buf->ಗಾತ್ರ_2;
        ಪ್ರತಿಜ್ಞಾ(buf->buf && (ಗಾತ್ರ_t) printed < ಲಭ್ಯ);
        va_ಆರಂಭ(ವಾದಗಳು, fmt);
        printed = vsnprintf(buf->buf + buf->ಗಾತ್ರ_2, ಲಭ್ಯ, fmt, ವಾದಗಳು);
        va_ಕೊನೆ(ವಾದಗಳು);

        ಆದರೆ (printed < 0 || (ಗಾತ್ರ_t) printed >= ಲಭ್ಯ)
            ಹೋಗು err;
    }

    buf->ಗಾತ್ರ_2 += (ಗಾತ್ರ_t)printed;
    ಹಿಂತಿರುಗಿ true;

err:
    ಬಿಡುಗಡೆ(buf->buf);
    buf->buf = ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ false;
}

#ವ್ಯಾಖ್ಯಾನಿಸು write_buf(buf, ...) ಮಾಡು { \
    ಆದರೆ (!check_write_buf(buf, __VA_ARGS__)) \
        ಹಿಂತಿರುಗಿ false; \
} ಆಗ (0)

ಸ್ಥಿರ bool
check_copy_ಗಮ್ಯ_buf(ರಚನೆ buf *buf, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಮೂಲ, ಗಾತ್ರ_t len)
{
    ಆದರೆ (len == 0)
        ಹಿಂತಿರುಗಿ true;

    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಲಭ್ಯ = buf->alloc - buf->ಗಾತ್ರ_2;
    ಆದರೆ (len >= ಲಭ್ಯ &&
        /* len + 1 (terminating NULL) */
        !do_realloc(buf, len + 1))
    {
        ಬಿಡುಗಡೆ(buf->buf);
        buf->buf = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ false;
    }

    ಸ್ಮರಣೆ_ನಕಲು(buf->buf + buf->ಗಾತ್ರ_2, ಮೂಲ, len);
    buf->ಗಾತ್ರ_2 += len;
    /* Append NULL byte */
    buf->buf[buf->ಗಾತ್ರ_2] = '\0';
    ಹಿಂತಿರುಗಿ true;
}

#ವ್ಯಾಖ್ಯಾನಿಸು copy_ಗಮ್ಯ_buf_len(buf, ಮೂಲ, len) ಮಾಡು { \
    ಆದರೆ (!check_copy_ಗಮ್ಯ_buf(buf, ಮೂಲ, len)) \
        ಹಿಂತಿರುಗಿ false; \
} ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು copy_ಗಮ್ಯ_buf(buf, ಮೂಲ) \
    copy_ಗಮ್ಯ_buf_len(buf, ಮೂಲ, ಗಾತ್ರ(ಮೂಲ) - 1)

ಸ್ಥಿರ bool
check_write_string_literal(ರಚನೆ buf *buf, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* string)
{
    copy_ಗಮ್ಯ_buf(buf, "\"");

    /* Write chunks, separated by characters requiring escape sequence */
    ಗಾತ್ರ_t pending_ಆರಂಭ = 0;
    ಗಾತ್ರ_t current = 0;
    ಅಕ್ಷರ escape_ಬಫರ್[] = {'\\', '0', '0', '0', '0'};
    ಗಾಗಿ (; string[current] != '\0'; current++) {
        uint8_t escape_len;
        ಆಯ್ಕೆ (string[current]) {
        ಸ್ಥಿತಿ '\n':
            /* `\n` would break strings */
            escape_ಬಫರ್[1] = 'n';
            escape_len = 2;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ '\\':
            /* `\` would create escape sequences */
            escape_ಬಫರ್[1] = '\\';
            escape_len = 2;
            ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
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
            ಆದರೆ (unlikely((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)string[current] < 0x20U ||
                         string[current] == '"')) {
                escape_ಬಫರ್[1] = '0';
                escape_ಬಫರ್[2] = '0';
                escape_ಬಫರ್[3] = (ಅಕ್ಷರ)('0' + (string[current] >> 3));
                escape_ಬಫರ್[4] = (ಅಕ್ಷರ)('0' + (string[current] & 0x7));
                escape_len = 5;
            } ಇಲ್ಲದಿದ್ದರೆ {
                /* Expand the chunk */
                ಮುಂದುವರಿ;
            }
        }
        /* Write pending chunk */
        ಪ್ರತಿಜ್ಞಾ(current >= pending_ಆರಂಭ);
        copy_ಗಮ್ಯ_buf_len(buf, string + pending_ಆರಂಭ, current - pending_ಆರಂಭ);
        /* Write escape sequence */
        copy_ಗಮ್ಯ_buf_len(buf, escape_ಬಫರ್, escape_len);
        pending_ಆರಂಭ = current + 1;
    }
    /* Write remaining chunk */
    ಪ್ರತಿಜ್ಞಾ(current >= pending_ಆರಂಭ);
    copy_ಗಮ್ಯ_buf_len(buf, string + pending_ಆರಂಭ, current - pending_ಆರಂಭ);

    copy_ಗಮ್ಯ_buf(buf, "\"");
    ಹಿಂತಿರುಗಿ true;
}

#ವ್ಯಾಖ್ಯಾನಿಸು write_buf_string_literal(buf, string) ಮಾಡು { \
    ಆದರೆ (!check_write_string_literal(buf, string)) \
        ಹಿಂತಿರುಗಿ false; \
} ಆಗ (0)

ಸ್ಥಿರ ಶೂನ್ಯ
delete_ಕೊನೆಯ_char(ರಚನೆ buf *buf, ಅಕ್ಷರ c)
{
    ಆದರೆ (buf->ಗಾತ್ರ_2 && buf->buf[buf->ಗಾತ್ರ_2 - 1] == c)
        buf->buf[--buf->ಗಾತ್ರ_2] = '\0';
}

/** Entry for a name substituion LUT */
ರಚನೆ ಕೀಲಿ_name_substitution {
    xkb_atom_t ಮೂಲ;
    xkb_atom_t ಗುರಿ;
    /** Used for intuitive renaming */
    xkb_keycode_t keycode;
};

/** Compare 2 key name substitution entries by they original name (atom) */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
cmp_ಕೀಲಿ_name_substitution(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b)
{
    ಸ್ಥಿರಾಂಕ xkb_atom_t s1 = ((ರಚನೆ ಕೀಲಿ_name_substitution *) a)->ಮೂಲ;
    ಸ್ಥಿರಾಂಕ xkb_atom_t s2 = ((ರಚನೆ ಕೀಲಿ_name_substitution *) b)->ಮೂಲ;
    ಹಿಂತಿರುಗಿ (s1 < s2) ? -1 : ((s1 > s2) ? 1 : 0);
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ darray(ರಚನೆ ಕೀಲಿ_name_substitution) ಕೀಲಿ_name_substitutions;

/**
 * Rename keys that are not compatible with the original XKB protocol,
 * i.e. more that 4 bytes. The new name will be formed as prefix + 3-digit
 * hexadecimal number.
 */
ಸ್ಥಿರ bool
rename_long_ಕೀಲಿಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap,
                 ಕೀಲಿ_name_substitutions *substitutions)
{
    /*
     * Prefixes we want to use for the new names: ASCII range `!`..`0`.
     * They are really unlikely to be used in usual keymaps.
     * NULL denotes an unavailable prefix.
     */
    ಎಣಿಕೆ {
        ಮೊದಲ = '!',
        ಕೊನೆಯ = '0',
        invalid = '\0'
    };
    ಅಕ್ಷರ prefixes[] = {
        ಮೊದಲ, invalid /* skip ‘"’, which would look buggy otherwise */,
        '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', ಕೊನೆಯ
    };
    static_assert(ಗಾತ್ರ(prefixes) == ಕೊನೆಯ - ಮೊದಲ + 1, "invalid range");

    /*
     * Traverse all the keys and aliases:
     * - Collect items with incompatible names.
     * - Remove prefixes that are already used in valid 4-char names.
     *   This ensures no name conflict will occur with the new names, without
     *   requiring an extensive check.
     */

    /* Check key names */
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ;
    xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap) {
        ಆದರೆ (ಕೀಲಿ->name == XKB_ATOM_NONE)
            ಮುಂದುವರಿ;

        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ name = xkb_atom_ಪಠ್ಯ(keymap->ctx, ಕೀಲಿ->name);
        ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t len = strlen_ಸುರಕ್ಷಿತ(name);
        ಆದರೆ (len > 4) {
            /* Key name too long: append key, pending for renaming */
            darray_append(*substitutions, (ರಚನೆ ಕೀಲಿ_name_substitution) {
                .ಮೂಲ = ಕೀಲಿ->name,
                .keycode = ಕೀಲಿ->keycode,
                .ಗುರಿ = ಕೀಲಿ->name
            });
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (len == 4 && name[0] >= ಮೊದಲ && name[0] <= ಕೊನೆಯ) {
            /* Potential name conflict: deactivate this prefix */
            prefixes[name[0] - ಮೊದಲ] = invalid;
        }
    }

    /* Check aliases */
    ಗಾಗಿ (darray_ಗಾತ್ರ_t a = 0; a < keymap->ಅಂಕೆ_ಕೀಲಿ_aliases; a++) {
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_alias * ಸ್ಥಿರಾಂಕ alias = &keymap->ಕೀಲಿ_aliases[a];
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ name = xkb_atom_ಪಠ್ಯ(keymap->ctx, alias->alias);
        ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t len = strlen_ಸುರಕ್ಷಿತ(name);
        ಆದರೆ (len > 4) {
            /* Alias name too long: append alias, pending for renaming */
            darray_append(*substitutions, (ರಚನೆ ಕೀಲಿ_name_substitution) {
                .ಮೂಲ = alias->alias,
                .keycode = XKB_KEYCODE_INVALID, /* Too expensive to lookup */
                .ಗುರಿ = alias->alias
            });
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (len == 4 && name[0] >= ಮೊದಲ && name[0] <= ಕೊನೆಯ) {
            /* Potential conflict with names substitutes: deactivate prefix */
            prefixes[name[0] - ಮೊದಲ] = invalid;
        }
    }

    ಆದರೆ darray_ಖಾಲಿಗಳು(*substitutions) {
        /* Nothing to do */
        ಹಿಂತಿರುಗಿ true;
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
    ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ = prefixes;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ_max = prefixes + ಗಾತ್ರ(prefixes) - 1;
    ಆಗ (*ಪೂರ್ವಪ್ರತ್ಯಯ == invalid && ಪೂರ್ವಪ್ರತ್ಯಯ < ಪೂರ್ವಪ್ರತ್ಯಯ_max)
        ಪೂರ್ವಪ್ರತ್ಯಯ++;

    /* Keycode prefix: prefer ‘0’ if available */
    ಅಕ್ಷರ keycode_ಪೂರ್ವಪ್ರತ್ಯಯ;
    ಆದರೆ (prefixes[ಕೊನೆಯ - ಮೊದಲ] == '0') {
        /* ‘0’ is available: use it then mark it unavailable for further use */
        keycode_ಪೂರ್ವಪ್ರತ್ಯಯ = ಕೊನೆಯ;
        prefixes[ಕೊನೆಯ - ಮೊದಲ] = invalid;
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* ‘0’ is unavailable: use first available prefix */
        keycode_ಪೂರ್ವಪ್ರತ್ಯಯ = *ಪೂರ್ವಪ್ರತ್ಯಯ;
        ಆದರೆ (unlikely(keycode_ಪೂರ್ವಪ್ರತ್ಯಯ == invalid)) {
            /*
             * Abort: no prefix available (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            darray_free(*substitutions);
            ಹಿಂತಿರುಗಿ true;
        }
        /* Update current prefix */
        *ಪೂರ್ವಪ್ರತ್ಯಯ = invalid;
        ಆಗ (*ಪೂರ್ವಪ್ರತ್ಯಯ == invalid && ಪೂರ್ವಪ್ರತ್ಯಯ < ಪೂರ್ವಪ್ರತ್ಯಯ_max)
            ಪೂರ್ವಪ್ರತ್ಯಯ++;
    }

    /* Proceed renaming */
    uint16_t ಮುಂದಿನ_id = 1;
    ರಚನೆ ಕೀಲಿ_name_substitution *sub;
    ಅಕ್ಷರ buf[6] = {0}; /* 5 would suffice, but generates a compiler warning */
    darray_foreach(sub, *substitutions) {
        static_assert(XKB_KEYCODE_INVALID > 0xfff, "Invalid keys filtered out");
        ಆದರೆ (sub->keycode <= 0xfff) {
            /*
             * Try nice formatting with the hexadecimal keycode.
             * We use lower case to reduce possible conflict because the
             * traditional key names are upper-cased.
             */
            snprintf(buf, ಗಾತ್ರ(buf), "%c%03"PRIx32,
                     keycode_ಪೂರ್ವಪ್ರತ್ಯಯ, sub->keycode);
            ಸ್ಥಿರಾಂಕ xkb_atom_t ಗುರಿ = xkb_atom_intern(keymap->ctx,
                                                      buf, ಗಾತ್ರ(buf) - 2);
            ಆದರೆ (ಗುರಿ == XKB_ATOM_NONE)
                ಹೋಗು error;
            sub->ಗುರಿ = ಗುರಿ;
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*ಪೂರ್ವಪ್ರತ್ಯಯ != invalid) {
            /* Cannot use keycode for formatting: generate next arbitrary ID */
            snprintf(buf, ಗಾತ್ರ(buf), "%c%03"PRIx16, *ಪೂರ್ವಪ್ರತ್ಯಯ, ಮುಂದಿನ_id);
            ಸ್ಥಿರಾಂಕ xkb_atom_t ಗುರಿ = xkb_atom_intern(keymap->ctx,
                                                      buf, ಗಾತ್ರ(buf) - 2);
            ಆದರೆ (ಗುರಿ == XKB_ATOM_NONE)
                ಹೋಗು error;
            sub->ಗುರಿ = ಗುರಿ;
            ಆದರೆ (++ಮುಂದಿನ_id > 0xfff) {
                *ಪೂರ್ವಪ್ರತ್ಯಯ = invalid;
                /* Get next available prefix */
                ಆಗ (*ಪೂರ್ವಪ್ರತ್ಯಯ == invalid && ಪೂರ್ವಪ್ರತ್ಯಯ < ಪೂರ್ವಪ್ರತ್ಯಯ_max)
                    ಪೂರ್ವಪ್ರತ್ಯಯ++;
                /* Reset counter */
                ಮುಂದಿನ_id = 1;
            }
        } ಇಲ್ಲದಿದ್ದರೆ {
            /*
             * No prefix available: skip substitution (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            sub->ಗುರಿ = sub->ಮೂಲ;
        }
    }

    /* Now sort by the source names so we can do a binary search */
    qsort(darray_ವಸ್ತುಗಳು(*substitutions), darray_ಗಾತ್ರ(*substitutions),
          ಗಾತ್ರ(*darray_ವಸ್ತುಗಳು(*substitutions)), &cmp_ಕೀಲಿ_name_substitution);

    ಹಿಂತಿರುಗಿ true;

error:
    darray_free(*substitutions);
    /* There was a memory error */
    log_err(keymap->ctx, XKB_ERROR_ಹಂಚಿಕೆ_FAILURE_2,
            "Cannot allocate key name substitution\n");
    ಹಿಂತಿರುಗಿ false;
}

/** Substitute a key name with a valid X11 key name */
ಸ್ಥಿರ xkb_atom_t
substitute_name(ಸ್ಥಿರಾಂಕ ಕೀಲಿ_name_substitutions *substitutions, xkb_atom_t name)
{
    /*ದ್ವಿಮಾನ ಹುಡುಕು*/
    darray_ಗಾತ್ರ_t ಮೊದಲ = 0;
    darray_ಗಾತ್ರ_t ಕೊನೆಯ = darray_ಗಾತ್ರ(*substitutions);

    ಆಗ (ಕೊನೆಯ > ಮೊದಲ) {
        darray_ಗಾತ್ರ_t mid = ಮೊದಲ + (ಕೊನೆಯ - 1 - ಮೊದಲ) / 2;
        ಸ್ಥಿರಾಂಕ xkb_atom_t ಮೂಲ = darray_ಅಂಶ(*substitutions, mid).ಮೂಲ;
        ಆದರೆ (ಮೂಲ < name)
            ಮೊದಲ = mid + 1;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಮೂಲ > name)
            ಕೊನೆಯ = mid;
        ಇಲ್ಲದಿದ್ದರೆ /* found it */
            ಹಿಂತಿರುಗಿ darray_ಅಂಶ(*substitutions, mid).ಗುರಿ;
    }

    /* No name substitution: return original name */
    ಹಿಂತಿರುಗಿ name;
}

ಸ್ಥಿರ bool
write_vmods(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap, ಎಣಿಕೆ xkb_keymap_format format,
            bool explicit, ರಚನೆ buf *buf)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod *mod;
    xkb_mod_ಸೂಚ್ಯಂಕ_t vmod = 0;
    bool has_some = false;

    xkb_vmods_enumerate(vmod, mod, &keymap->mods) {
        ಆದರೆ (!has_some) {
            copy_ಗಮ್ಯ_buf(buf, "\tvirtual_modifiers ");
            has_some = true;
        } ಇಲ್ಲದಿದ್ದರೆ {
            copy_ಗಮ್ಯ_buf(buf, ",");
        }
        write_buf(buf, "%s", xkb_atom_ಪಠ್ಯ(keymap->ctx, mod->name));

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
        ಸ್ಥಿರಾಂಕ bool auto_canonical_mods = (format >= XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V2);

        ಆದರೆ ((keymap->mods.explicit_vmods & (UINT32_C(1) << vmod) ||
             explicit) &&
            (auto_canonical_mods || mod->mapping != 0)) {
            /*
             * Explicit non-default mapping
             * NOTE: we can only pretty-print *real* modifiers in this context.
             */
            write_buf(buf, "=%s",
                      Mod_ಮುಖವಾಡ_ಪಠ್ಯ(keymap->ctx, MOD_REAL, &keymap->mods,
                                  mod->mapping));
        }
    }

    ಆದರೆ (has_some)
        copy_ಗಮ್ಯ_buf(buf, ";\n\n");

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
write_keycodes(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap,
               ಸ್ಥಿರಾಂಕ ಕೀಲಿ_name_substitutions *substitutions,
               ಎಣಿಕೆ xkb_keymap_serialize_flags flags, ರಚನೆ buf *buf)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ;
    xkb_led_ಸೂಚ್ಯಂಕ_t idx;
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_led *led;

    ಸ್ಥಿರಾಂಕ bool pretty = !!(flags & XKB_KEYMAP_SERIALIZE_PRETTY);

    ಆದರೆ (keymap->keycodes_section_name)
        write_buf(buf, "xkb_keycodes \"%s\" {\n",
                  keymap->keycodes_section_name);
    ಇಲ್ಲದಿದ್ದರೆ
        copy_ಗಮ್ಯ_buf(buf, "xkb_keycodes {\n");

    /* NOTE: there is no support for default values */

    /* xkbcomp and X11 really want to see keymaps with a minimum of 8, and
     * a maximum of at least 255, else XWayland really starts hating life.
     * If this is a problem and people really need strictly bounded keymaps,
     * we should probably control this with a flag. */
    write_buf(buf, "\tminimum = %"PRIu32";\n", MIN(keymap->min_ಕೀಲಿ_ಸಂಕೇತ, 8));
    write_buf(buf, "\tmaximum = %"PRIu32";\n", MAX(keymap->max_ಕೀಲಿ_ಸಂಕೇತ, 255));

    xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap) {
        ಆದರೆ (ಕೀಲಿ->name == XKB_ATOM_NONE)
            ಮುಂದುವರಿ;

        ಸ್ಥಿರಾಂಕ xkb_atom_t name = (substitutions == ಖಾಲಿ)
            ? ಕೀಲಿ->name
            : substitute_name(substitutions, ಕೀಲಿ->name);
        ಆದರೆ (pretty)
            write_buf(buf, "\t%-20s", ಕೀಲಿ_Name_ಪಠ್ಯ(keymap->ctx, name));
        ಇಲ್ಲದಿದ್ದರೆ
            write_buf(buf, "\t%s", ಕೀಲಿ_Name_ಪಠ್ಯ(keymap->ctx, name));
        write_buf(buf, " = %"PRIu32";\n", ಕೀಲಿ->keycode);
    }

    xkb_leds_enumerate(idx, led, keymap)
        ಆದರೆ (led->name != XKB_ATOM_NONE) {
            write_buf(buf, "\tindicator %"PRIu32" = ", idx + 1);
            write_buf_string_literal(buf, xkb_atom_ಪಠ್ಯ(keymap->ctx, led->name));
            copy_ಗಮ್ಯ_buf(buf, ";\n");
        }

    ಗಾಗಿ (darray_ಗಾತ್ರ_t i = 0; i < keymap->ಅಂಕೆ_ಕೀಲಿ_aliases; i++) {
        ಸ್ಥಿರಾಂಕ xkb_atom_t alias = (substitutions == ಖಾಲಿ)
            ? keymap->ಕೀಲಿ_aliases[i].alias
            : substitute_name(substitutions, keymap->ಕೀಲಿ_aliases[i].alias);
        ಸ್ಥಿರಾಂಕ xkb_atom_t real = (substitutions == ಖಾಲಿ)
            ? keymap->ಕೀಲಿ_aliases[i].real
            : substitute_name(substitutions, keymap->ಕೀಲಿ_aliases[i].real);

        ಆದರೆ (pretty)
            write_buf(buf, "\talias %-14s",
                      ಕೀಲಿ_Name_ಪಠ್ಯ(keymap->ctx, alias));
        ಇಲ್ಲದಿದ್ದರೆ
            write_buf(buf, "\talias %s",
                      ಕೀಲಿ_Name_ಪಠ್ಯ(keymap->ctx, alias));
        write_buf(buf, " = %s;\n",
                  ಕೀಲಿ_Name_ಪಠ್ಯ(keymap->ctx, real));
    }

    copy_ಗಮ್ಯ_buf(buf, "};\n\n");
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
write_types(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap, ಎಣಿಕೆ xkb_keymap_format format,
            ಎಣಿಕೆ xkb_keymap_serialize_flags flags, ರಚನೆ buf *buf)
{
    ಆದರೆ (keymap->types_section_name)
        write_buf(buf, "xkb_types \"%s\" {\n",
                  keymap->types_section_name);
    ಇಲ್ಲದಿದ್ದರೆ
        copy_ಗಮ್ಯ_buf(buf, "xkb_types {\n");

    /* NOTE: support for default values has been removed */

    ಆದರೆ (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        ಹಿಂತಿರುಗಿ false;

    ಸ್ಥಿರಾಂಕ bool drop_ಬಳಸದ = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_ಬಳಸದ);
    ಗಾಗಿ (darray_ಗಾತ್ರ_t i = 0; i < keymap->ಅಂಕೆ_types; i++) {
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type * ಸ್ಥಿರಾಂಕ type = &keymap->types[i];
        ಆದರೆ (!type->required && drop_ಬಳಸದ)
            ಮುಂದುವರಿ;

        copy_ಗಮ್ಯ_buf(buf, "\ttype ");
        write_buf_string_literal(buf, xkb_atom_ಪಠ್ಯ(keymap->ctx, type->name));
        copy_ಗಮ್ಯ_buf(buf, " {\n");

        write_buf(buf, "\t\tmodifiers= %s;\n",
                  Mod_ಮುಖವಾಡ_ಪಠ್ಯ(keymap->ctx, MOD_BOTH, &keymap->mods,
                              type->mods.mods));

        ಗಾಗಿ (darray_ಗಾತ್ರ_t j = 0; j < type->ಅಂಕೆ_entries; j++) {
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str;
            ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು *ನಮೂದು = &type->entries[j];

            /*
             * Printing level 1 entries is redundant, it's the default,
             * unless there's preserve info.
             */
            ಆದರೆ (ನಮೂದು->level == 0 && ನಮೂದು->preserve.mods == 0 &&
                !(flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_ಮೌಲ್ಯಗಳು))
                ಮುಂದುವರಿ;

            str = Mod_ಮುಖವಾಡ_ಪಠ್ಯ(keymap->ctx, MOD_BOTH, &keymap->mods,
                              ನಮೂದು->mods.mods);
            write_buf(buf, "\t\tmap[%s]= %"PRIu32";\n",
                      str, ನಮೂದು->level + 1);

            ಆದರೆ (ನಮೂದು->preserve.mods)
                write_buf(buf, "\t\tpreserve[%s]= %s;\n",
                          str, Mod_ಮುಖವಾಡ_ಪಠ್ಯ(keymap->ctx, MOD_BOTH, &keymap->mods,
                                           ನಮೂದು->preserve.mods));
        }

        ಗಾಗಿ (xkb_level_ಸೂಚ್ಯಂಕ_t n = 0; n < type->ಅಂಕೆ_level_ಹೆಸರುಗಳು; n++)
            ಆದರೆ (type->level_ಹೆಸರುಗಳು[n]) {
                write_buf(buf, "\t\tlevel_name[%"PRIu32"]= ", n + 1);
                write_buf_string_literal(
                    buf, xkb_atom_ಪಠ್ಯ(keymap->ctx, type->level_ಹೆಸರುಗಳು[n]));
                copy_ಗಮ್ಯ_buf(buf, ";\n");
            }

        copy_ಗಮ್ಯ_buf(buf, "\t};\n");
    }

    copy_ಗಮ್ಯ_buf(buf, "};\n\n");
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
write_led_map(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap, ಎಣಿಕೆ xkb_keymap_format format,
              bool defaults, ರಚನೆ buf *buf, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_led *led)
{
    copy_ಗಮ್ಯ_buf(buf, "\tindicator ");
    write_buf_string_literal(buf, xkb_atom_ಪಠ್ಯ(keymap->ctx, led->name));
    copy_ಗಮ್ಯ_buf(buf, " {\n");

    ಆದರೆ (led->which_groups) {
        ಆದರೆ (led->which_groups != XKB_STATE_LAYOUT_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichGroupState= %s;\n",
                      Led_State_ಮುಖವಾಡ_ಪಠ್ಯ(keymap->ctx, ಗುಂಪು_Component_ಮುಖವಾಡ_ಹೆಸರುಗಳು,
                                       led->which_groups));
        }
        write_buf(buf, "\t\tgroups= 0x%02"PRIx32";\n",
                  led->groups);
    }

    ಆದರೆ (led->which_mods) {
        ಆದರೆ (led->which_mods != XKB_STATE_MODS_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichModState= %s;\n",
                      Led_State_ಮುಖವಾಡ_ಪಠ್ಯ(keymap->ctx, mod_Component_ಮುಖವಾಡ_ಹೆಸರುಗಳು,
                                       led->which_mods));
        }
        write_buf(buf, "\t\tmodifiers= %s;\n",
                  Mod_ಮುಖವಾಡ_ಪಠ್ಯ(keymap->ctx, MOD_BOTH, &keymap->mods,
                              led->mods.mods));
    }

    ಆದರೆ (led->ctrls) {
        write_buf(buf, "\t\tcontrols= %s;\n",
                  Control_ಮುಖವಾಡ_ಪಠ್ಯ(keymap->ctx, format, led->ctrls));
    }

    copy_ಗಮ್ಯ_buf(buf, "\t};\n");
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
affect_ಬೀಗ_ಪಠ್ಯ(ಎಣಿಕೆ xkb_action_flags flags, bool show_both)
{
    ಆಯ್ಕೆ (flags & (ACTION_ಬೀಗ_NO_ಬೀಗ | ACTION_ಬೀಗ_NO_UNLOCK)) {
    ಸ್ಥಿತಿ 0:
        ಹಿಂತಿರುಗಿ show_both ? ",affect=both" : "";
    ಸ್ಥಿತಿ ACTION_ಬೀಗ_NO_UNLOCK:
        ಹಿಂತಿರುಗಿ ",affect=lock";
    ಸ್ಥಿತಿ ACTION_ಬೀಗ_NO_ಬೀಗ:
        ಹಿಂತಿರುಗಿ ",affect=unlock";
    ಸ್ಥಿತಿ ACTION_ಬೀಗ_NO_ಬೀಗ | ACTION_ಬೀಗ_NO_UNLOCK:
        ಹಿಂತಿರುಗಿ ",affect=neither";
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹಿಂತಿರುಗಿ "";
    }
}

#ವ್ಯಾಖ್ಯಾನಿಸು ಚಿಹ್ನೆ_PADDING 15
#ವ್ಯಾಖ್ಯಾನಿಸು ACTION_PADDING 30

ಸ್ಥಿರ bool
write_action(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap, ಎಣಿಕೆ xkb_keymap_format format,
             xkb_layout_ಸೂಚ್ಯಂಕ_t max_groups, ರಚನೆ buf *buf,
             ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action *action,
             ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *suffix)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *type;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ವಾದಗಳು = ಖಾಲಿ;

    ಆದರೆ (!ಪೂರ್ವಪ್ರತ್ಯಯ)
        ಪೂರ್ವಪ್ರತ್ಯಯ = "";
    ಆದರೆ (!suffix)
        suffix = "";

    type = Action_Type_ಪಠ್ಯ(action->type);

    ಆಯ್ಕೆ (action->type) {
    ಸ್ಥಿತಿ ACTION_TYPE_MOD_ಗಣ:
    ಸ್ಥಿತಿ ACTION_TYPE_MOD_LATCH:
    ಸ್ಥಿತಿ ACTION_TYPE_MOD_ಬೀಗ:
        ಆದರೆ (action->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            ವಾದಗಳು = "modMapMods";
        ಇಲ್ಲದಿದ್ದರೆ
            ವಾದಗಳು = Mod_ಮುಖವಾಡ_ಪಠ್ಯ(keymap->ctx, MOD_BOTH, &keymap->mods,
                               action->mods.mods.mods);
        bool unlockOnPress = (action->mods.flags & ACTION_UNLOCK_ON_PRESS);
        ಆದರೆ (unlockOnPress && !is_Mods_Un_ಬೀಗ_On_Press_Supported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_ಪಠ್ಯ_FORMAT,
                    "Cannot use \"%s(unlockOnPress=true)\" in keymap format %d\n",
                    Action_Type_ಪಠ್ಯ(action->type), format);
            unlockOnPress = false;
        }
        bool latchOnPress = action->type == ACTION_TYPE_MOD_LATCH &&
                            (action->ಗುಂಪು.flags & ACTION_LATCH_ON_PRESS);
        ಆದರೆ (latchOnPress && !isModsLatchOnPressSupported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_ಪಠ್ಯ_FORMAT,
                    "Cannot use \"LatchMods(latchOnPress=true)\" "
                    "in keymap format %d\n", format);
            latchOnPress = false;
        }
        write_buf(buf, "%s%s(modifiers=%s%s%s%s%s%s)%s", ಪೂರ್ವಪ್ರತ್ಯಯ, type, ವಾದಗಳು,
                  (action->type != ACTION_TYPE_MOD_ಬೀಗ && (action->mods.flags & ACTION_ಬೀಗ_CLEAR)) ? ",clearLocks" : "",
                  (action->type == ACTION_TYPE_MOD_LATCH && (action->mods.flags & ACTION_LATCH_ಗಮ್ಯ_ಬೀಗ)) ? ",latchToLock" : "",
                  (action->type == ACTION_TYPE_MOD_ಬೀಗ) ? affect_ಬೀಗ_ಪಠ್ಯ(action->mods.flags, false) : "",
                  (unlockOnPress) ? ",unlockOnPress" : "",
                  (latchOnPress) ? ",latchOnPress" : "",
                  suffix);
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ACTION_TYPE_ಗುಂಪು_ಗಣ:
    ಸ್ಥಿತಿ ACTION_TYPE_ಗುಂಪು_LATCH:
    ಸ್ಥಿತಿ ACTION_TYPE_ಗುಂಪು_ಬೀಗ:
        ಆದರೆ ((uint32_t) xkb_abs(action->ಗುಂಪು.ಗುಂಪು) <
            (max_groups + !(action->ಗುಂಪು.flags & ACTION_ABSOLUTE_SWITCH))) {
            bool ಬೀಗ_On_Release = action->type == ACTION_TYPE_ಗುಂಪು_ಬೀಗ &&
                                 (action->ಗುಂಪು.flags & ACTION_ಬೀಗ_ON_RELEASE);
            ಆದರೆ (ಬೀಗ_On_Release && !is_ಗುಂಪು_ಬೀಗ_On_Release_Supported(format)) {
                log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_ಪಠ್ಯ_FORMAT,
                        "Cannot use \"GroupLock(lockOnRelease=true)\" "
                        "in keymap format %d\n", format);
                ಬೀಗ_On_Release = false;
            }
            write_buf(buf, "%s%s(group=%s%"PRId32"%s%s%s)%s", ಪೂರ್ವಪ್ರತ್ಯಯ, type,
                      (!(action->ಗುಂಪು.flags & ACTION_ABSOLUTE_SWITCH) && action->ಗುಂಪು.ಗುಂಪು >= 0) ? "+" : "",
                      (action->ಗುಂಪು.flags & ACTION_ABSOLUTE_SWITCH) ? action->ಗುಂಪು.ಗುಂಪು + 1 : action->ಗುಂಪು.ಗುಂಪು,
                      (action->type != ACTION_TYPE_ಗುಂಪು_ಬೀಗ && (action->ಗುಂಪು.flags & ACTION_ಬೀಗ_CLEAR)) ? ",clearLocks" : "",
                      (action->type == ACTION_TYPE_ಗುಂಪು_LATCH && (action->ಗುಂಪು.flags & ACTION_LATCH_ಗಮ್ಯ_ಬೀಗ)) ? ",latchToLock" : "",
                      (ಬೀಗ_On_Release) ? ",lockOnRelease" : "",
                      suffix);
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* Unsupported group index: degrade to VoidAction() */
            log_warn(keymap->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                     "Unsupported group index %"PRIu32" for action %s()\n",
                      (action->ಗುಂಪು.ಗುಂಪು +
                       !!(action->ಗುಂಪು.flags & ACTION_ABSOLUTE_SWITCH)),
                     type);
            ಹೋಗು void_action;
        }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ACTION_TYPE_TERMINATE:
        write_buf(buf, "%s%s()%s", ಪೂರ್ವಪ್ರತ್ಯಯ, type, suffix);
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ACTION_TYPE_PTR_MOVE:
        write_buf(buf, "%s%s(x=%s%d,y=%s%d%s)%s", ಪೂರ್ವಪ್ರತ್ಯಯ, type,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_X) && action->ptr.x >= 0) ? "+" : "",
                  action->ptr.x,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_Y) && action->ptr.y >= 0) ? "+" : "",
                  action->ptr.y,
                  (action->ptr.flags & ACTION_REPEAT) ? "" : ",!repeat",
                  suffix);
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ACTION_TYPE_PTR_ಬೀಗ:
        ವಾದಗಳು = affect_ಬೀಗ_ಪಠ್ಯ(action->btn.flags, true);
        /* fallthrough */
    ಸ್ಥಿತಿ ACTION_TYPE_PTR_BUTTON:
        write_buf(buf, "%s%s(button=", ಪೂರ್ವಪ್ರತ್ಯಯ, type);
        static_assert(XKB_ಸೂಚಕ_BUTTON_DEFAULT < XKB_ಸೂಚಕ_BUTTON_MIN, "");
        ಆದರೆ (action->btn.button > XKB_ಸೂಚಕ_BUTTON_DEFAULT &&
            action->btn.button <= XKB_ಸೂಚಕ_BUTTON_MAX)
            write_buf(buf, "%"PRIu8, action->btn.button);
        ಇಲ್ಲದಿದ್ದರೆ
            copy_ಗಮ್ಯ_buf(buf, "default");
        ಆದರೆ (action->btn.ಎಣಿಕೆ_2) {
            ಪ್ರತಿಜ್ಞಾ(action->type != ACTION_TYPE_PTR_ಬೀಗ);
            write_buf(buf, ",count=%u", action->btn.ಎಣಿಕೆ_2);
        }
        ಆದರೆ (ವಾದಗಳು)
            write_buf(buf, "%s", ವಾದಗಳು);
        write_buf(buf, ")%s", suffix);
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ACTION_TYPE_PTR_DEFAULT:
        write_buf(buf, "%s%s(", ಪೂರ್ವಪ್ರತ್ಯಯ, type);
        write_buf(buf, "affect=button,button=%s%"PRId8,
                  (!(action->dflt.flags & ACTION_ABSOLUTE_SWITCH) && action->dflt.ಮೌಲ್ಯ >= 0) ? "+" : "",
                  action->dflt.ಮೌಲ್ಯ);
        write_buf(buf, ")%s", suffix);
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ACTION_TYPE_SWITCH_VT:
        write_buf(buf, "%s%s(screen=%s%d,%ssame)%s", ಪೂರ್ವಪ್ರತ್ಯಯ, type,
                  (!(action->screen.flags & ACTION_ABSOLUTE_SWITCH) && action->screen.screen >= 0) ? "+" : "",
                  action->screen.screen,
                  (action->screen.flags & ACTION_SAME_SCREEN) ? "" : "!",
                  suffix);
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ACTION_TYPE_CTRL_ಗಣ:
    ಸ್ಥಿತಿ ACTION_TYPE_CTRL_ಬೀಗ:
        write_buf(buf, "%s%s(controls=%s%s)%s", ಪೂರ್ವಪ್ರತ್ಯಯ, type,
                  Control_ಮುಖವಾಡ_ಪಠ್ಯ(keymap->ctx, format, action->ctrls.ctrls),
                  (action->type == ACTION_TYPE_CTRL_ಬೀಗ
                    ? affect_ಬೀಗ_ಪಠ್ಯ(action->ctrls.flags, false)
                    : ""),
                  suffix);
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ACTION_TYPE_REDIRECT_ಕೀಲಿ: {
        write_buf(buf, "%s%s(", ಪೂರ್ವಪ್ರತ್ಯಯ, type);
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ * ಸ್ಥಿರಾಂಕ ಕೀಲಿ = Xkb_ಕೀಲಿ(keymap, action->redirect.keycode);
        /* Can fail if the keycode was not initialized */
        ಆದರೆ (ಕೀಲಿ)
            write_buf(buf, "keycode=%s", ಕೀಲಿ_Name_ಪಠ್ಯ(keymap->ctx, ಕೀಲಿ->name));
        ಆದರೆ (action->redirect.affect_mods) {
            xkb_mod_ಮುಖವಾಡ_t ಮುಖವಾಡ;
            ಮುಖವಾಡ = (action->redirect.affect_mods & action->redirect.mods);
            ಆದರೆ (ಮುಖವಾಡ)
                write_buf(buf, ",modifiers=%s",
                          Mod_ಮುಖವಾಡ_ಪಠ್ಯ(keymap->ctx, MOD_BOTH, &keymap->mods, ಮುಖವಾಡ));
            ಮುಖವಾಡ = (action->redirect.affect_mods & ~action->redirect.mods);
            ಆದರೆ (ಮುಖವಾಡ)
                write_buf(buf, ",clearMods=%s",
                          Mod_ಮುಖವಾಡ_ಪಠ್ಯ(keymap->ctx, MOD_BOTH, &keymap->mods, ಮುಖವಾಡ));
        }
        write_buf(buf, ")%s", suffix);
        ನಿಲ್ಲಿಸು;
    }

    ಸ್ಥಿತಿ ACTION_TYPE_UNKNOWN:
    ಸ್ಥಿತಿ ACTION_TYPE_NONE:
        write_buf(buf, "%sNoAction()%s", ಪೂರ್ವಪ್ರತ್ಯಯ, suffix);
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ACTION_TYPE_VOID:
void_action:
        /*
         * VoidAction() is an xkbcommon extension.
         * Use LockControls as a backward-compatible fallback.
         * We cannot serialize it to `NoAction()`, as it would be dropped in
         * e.g. the context of multiple actions.
         * We better not use `Private` either, because it could still be
         * interpreted by X11.
         */
        ಆದರೆ (format == XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1)
            write_buf(buf, "%sLockControls(controls=none,affect=neither)%s",
                      ಪೂರ್ವಪ್ರತ್ಯಯ, suffix);
        ಇಲ್ಲದಿದ್ದರೆ
            write_buf(buf, "%sVoidAction()%s", ಪೂರ್ವಪ್ರತ್ಯಯ, suffix);
        ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_ಆಂತರಿಕ == 20 &&
                      ACTION_TYPE_ಆಂತರಿಕ + 1 == ACTION_TYPE_ಅಂಕೆ_ENTRIES,
                      "Missing action type");
        /* Unsupported legacy actions should have degraded to NoAction */
        ಪ್ರತಿಜ್ಞಾ(action->type != ACTION_TYPE_UNSUPPORTED_LEGACY);
        write_buf(buf,
                  "%s%s(type=0x%02x,data[0]=0x%02x,data[1]=0x%02x,data[2]=0x%02x,data[3]=0x%02x,data[4]=0x%02x,data[5]=0x%02x,data[6]=0x%02x)%s",
                  ಪೂರ್ವಪ್ರತ್ಯಯ, type, action->type, action->priv.ದತ್ತಾಂಶ[0],
                  action->priv.ದತ್ತಾಂಶ[1], action->priv.ದತ್ತಾಂಶ[2],
                  action->priv.ದತ್ತಾಂಶ[3], action->priv.ದತ್ತಾಂಶ[4],
                  action->priv.ದತ್ತಾಂಶ[5], action->priv.ದತ್ತಾಂಶ[6],
                  suffix);
        ನಿಲ್ಲಿಸು;
    }

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action void_actions[] = {
    { .type = ACTION_TYPE_VOID }
};

ಸ್ಥಿರ bool
write_actions(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap, ಎಣಿಕೆ xkb_keymap_format format,
              xkb_layout_ಸೂಚ್ಯಂಕ_t max_groups, ರಚನೆ buf *buf, ರಚನೆ buf *buf2,
              ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ, xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುಂಪು)
{
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action noAction = { .type = ACTION_TYPE_NONE };

    ಗಾಗಿ (xkb_level_ಸೂಚ್ಯಂಕ_t level = 0; level < Xkb_ಕೀಲಿ_ಅಂಕೆ_ಮಟ್ಟಗಳು(ಕೀಲಿ, ಗುಂಪು);
         level++) {

        ಆದರೆ (level != 0)
            copy_ಗಮ್ಯ_buf(buf, ", ");

        ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action *actions = ಖಾಲಿ;
        xkb_action_ಎಣಿಕೆ_t ಎಣಿಕೆ_2 = xkb_keymap_ಕೀಲಿ_get_actions_by_level(
            keymap, ಕೀಲಿ, ಗುಂಪು, level, &actions
        );

        ಆದರೆ (ಎಣಿಕೆ_2 > 1 && format < XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            actions = void_actions;
            ಎಣಿಕೆ_2 = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_ಪಠ್ಯ_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        buf2->ಗಾತ್ರ_2 = 0;
        ಆದರೆ (ಎಣಿಕೆ_2 == 0) {
            ಆದರೆ (!write_action(keymap, format, max_groups,
                              buf2, &noAction, ಖಾಲಿ, ಖಾಲಿ))
                ಹಿಂತಿರುಗಿ false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಎಣಿಕೆ_2 == 1) {
            ಆದರೆ (!write_action(keymap, format, max_groups,
                              buf2, &(actions[0]), ಖಾಲಿ, ಖಾಲಿ))
                ಹಿಂತಿರುಗಿ false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            copy_ಗಮ್ಯ_buf(buf2, "{ ");
            ಗಾಗಿ (xkb_action_ಎಣಿಕೆ_t k = 0; k < ಎಣಿಕೆ_2; k++) {
                ಆದರೆ (k != 0)
                    copy_ಗಮ್ಯ_buf(buf2, ", ");
                ಗಾತ್ರ_t old_ಗಾತ್ರ = buf2->ಗಾತ್ರ_2;
                ಆದರೆ (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), ಖಾಲಿ, ಖಾಲಿ))
                    ಹಿಂತಿರುಗಿ false;
                /* Check if padding is necessary */
                ಆದರೆ (buf2->ಗಾತ್ರ_2 >= old_ಗಾತ್ರ + ACTION_PADDING)
                    ಮುಂದುವರಿ;
                /* Compute and write padding, then write the action again */
                ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ padding = (ಪೂರ್ಣಾಂಕ)(old_ಗಾತ್ರ + ACTION_PADDING - buf2->ಗಾತ್ರ_2);
                buf2->ಗಾತ್ರ_2 = old_ಗಾತ್ರ;
                write_buf(buf2, "%*s", padding, "");
                ಆದರೆ (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), ಖಾಲಿ, ಖಾಲಿ))
                    ಹಿಂತಿರುಗಿ false;
            }
            copy_ಗಮ್ಯ_buf(buf2, " }");
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
    }

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
write_action_defaults(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap,
                      ಎಣಿಕೆ xkb_keymap_format format,
                      ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action *action,
                      ರಚನೆ buf *buf)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *type = Action_Type_ಪಠ್ಯ(action->type);

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೂರ್ವಪ್ರತ್ಯಯ_2 "\t"

    ಆಯ್ಕೆ (action->type) {
    ಸ್ಥಿತಿ ACTION_TYPE_MOD_ಗಣ:
    ಸ್ಥಿತಿ ACTION_TYPE_MOD_LATCH:
    ಸ್ಥಿತಿ ACTION_TYPE_MOD_ಬೀಗ:
        ಪ್ರತಿಜ್ಞಾ(action->mods.flags == 0);
        ಪ್ರತಿಜ್ಞಾ(action->mods.mods.mods == 0);
        write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.modifiers = 0;\n", type);
        ಆದರೆ (action->type != ACTION_TYPE_MOD_ಬೀಗ)
            write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.clearLocks = false;\n", type);
        ಆದರೆ (action->type == ACTION_TYPE_MOD_LATCH &&
            isModsLatchOnPressSupported(format))
            write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.latchOnPress = false;\n", type);
        ಆದರೆ (action->type == ACTION_TYPE_MOD_LATCH)
            write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.latchToLock = false;\n", type);
        ಆದರೆ (action->type == ACTION_TYPE_MOD_ಬೀಗ)
            write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.affect = both;\n", type);
        ಆದರೆ (is_Mods_Un_ಬೀಗ_On_Press_Supported(format))
            write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.unlockOnPress = false;\n", type);
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ACTION_TYPE_ಗುಂಪು_ಗಣ:
    ಸ್ಥಿತಿ ACTION_TYPE_ಗುಂಪು_LATCH:
    ಸ್ಥಿತಿ ACTION_TYPE_ಗುಂಪು_ಬೀಗ:
        ಪ್ರತಿಜ್ಞಾ(action->ಗುಂಪು.flags == 0);
        ಪ್ರತಿಜ್ಞಾ(action->ಗುಂಪು.ಗುಂಪು == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.group = +0;\n", type);
        ಆದರೆ (action->type != ACTION_TYPE_ಗುಂಪು_ಬೀಗ)
            write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.clearLocks = false;\n", type);
        ಆದರೆ (action->type == ACTION_TYPE_ಗುಂಪು_LATCH)
            write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.latchToLock = false;\n", type);
        ಆದರೆ (action->type == ACTION_TYPE_ಗುಂಪು_ಬೀಗ &&
            is_ಗುಂಪು_ಬೀಗ_On_Release_Supported(format))
            write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.lockOnRelease = false;\n", type);
        ನಿಲ್ಲಿಸು;


    ಸ್ಥಿತಿ ACTION_TYPE_PTR_MOVE:
        ಪ್ರತಿಜ್ಞಾ(action->ptr.x == 0);
        ಪ್ರತಿಜ್ಞಾ(action->ptr.y == 0);
        ಪ್ರತಿಜ್ಞಾ(action->ptr.flags == ACTION_REPEAT);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.x = +0;\n", type);
        write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.y = +0;\n", type);
        write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.accel = true;\n", type);
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ACTION_TYPE_PTR_ಬೀಗ:
    ಸ್ಥಿತಿ ACTION_TYPE_PTR_BUTTON:
        ಪ್ರತಿಜ್ಞಾ(action->btn.flags == 0);
        ಪ್ರತಿಜ್ಞಾ(action->btn.button == 0);
        ಪ್ರತಿಜ್ಞಾ(action->btn.ಎಣಿಕೆ_2 == 0);
        write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.button = 0;\n", type);
        write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.count = 0;\n", type);
        ಆದರೆ (action->type == ACTION_TYPE_PTR_ಬೀಗ)
            write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.affect = both;\n", type);
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ACTION_TYPE_PTR_DEFAULT:
        ಪ್ರತಿಜ್ಞಾ(action->dflt.flags == 0);
        ಪ್ರತಿಜ್ಞಾ(action->dflt.ಮೌಲ್ಯ == 1);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.button = +1;\n", type);
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ACTION_TYPE_SWITCH_VT:
        ಪ್ರತಿಜ್ಞಾ(action->screen.flags == ACTION_SAME_SCREEN);
        ಪ್ರತಿಜ್ಞಾ(action->screen.screen == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.screen = +0;\n", type);
        write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.same = true;\n", type);
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ACTION_TYPE_CTRL_ಗಣ:
    ಸ್ಥಿತಿ ACTION_TYPE_CTRL_ಬೀಗ:
        ಪ್ರತಿಜ್ಞಾ(action->ctrls.flags == 0);
        ಪ್ರತಿಜ್ಞಾ(action->ctrls.ctrls == 0);
        write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.controls = 0;\n", type);
        ಆದರೆ (action->type == ACTION_TYPE_CTRL_ಬೀಗ)
            write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.affect = both;\n", type);
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ACTION_TYPE_REDIRECT_ಕೀಲಿ:
        ಪ್ರತಿಜ್ಞಾ(action->redirect.keycode == keymap->redirect_ಕೀಲಿ_auto);
        ಪ್ರತಿಜ್ಞಾ(action->redirect.affect_mods == 0);
        ಪ್ರತಿಜ್ಞಾ(action->redirect.mods == 0);
        write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.keycode = auto;\n", type);
        write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.modifiers = 0;\n", type);
        write_buf(buf, ಪೂರ್ವಪ್ರತ್ಯಯ_2"%s.clearMods = 0;\n", type);
        ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_ಆಂತರಿಕ == 20 &&
                      ACTION_TYPE_ಆಂತರಿಕ + 1 == ACTION_TYPE_ಅಂಕೆ_ENTRIES,
                      "Missing action type");
    }

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ಪೂರ್ವಪ್ರತ್ಯಯ_2

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
write_actions_defaults(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap,
                       ಎಣಿಕೆ xkb_keymap_format format, ರಚನೆ buf *buf)
{
    ActionsInfo defaults = {0};
    InitActionsInfo(keymap, &defaults);

    ಗಾಗಿ (uint8_t a = 0; a < (uint8_t) ACTION_TYPE_ಅಂಕೆ_ENTRIES; a++) {
        ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action * ಸೀಮಿತ ಸ್ಥಿರಾಂಕ action = &defaults.actions[a];
        ಆದರೆ (!write_action_defaults(keymap, format, action, buf))
            ಹಿಂತಿರುಗಿ false;
    }
    copy_ಗಮ್ಯ_buf(buf, "\n");

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_sym_interpret fallback_interpret = {
    .sym = XKB_ಕೀಲಿ_Void_ಚಿಹ್ನೆ,
    .required = true,
    .repeat = FALLBACK_INTERPRET_ಕೀಲಿ_REPEAT,
    .ಹೊಂದಿಕೆ = ಹೊಂದಿಕೆ_ANY_OR_NONE,
    .mods = 0,
    .virtual_mod = (xkb_mod_ಸೂಚ್ಯಂಕ_t) DEFAULT_INTERPRET_VMOD,
    .ಅಂಕೆ_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

ಸ್ಥಿರ bool
write_compat(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap * ಸೀಮಿತ keymap,
             ಎಣಿಕೆ xkb_keymap_format format, xkb_layout_ಸೂಚ್ಯಂಕ_t max_groups,
             ಎಣಿಕೆ xkb_keymap_serialize_flags flags, bool * ಸೀಮಿತ some_interp,
             ರಚನೆ buf *buf)
{
    ಸ್ಥಿರಾಂಕ bool pretty = (flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ಸ್ಥಿರಾಂಕ bool defaults = (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_ಮೌಲ್ಯಗಳು);
    ಸ್ಥಿರಾಂಕ bool drop_ಬಳಸದ = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_ಬಳಸದ);
    ಸ್ಥಿರಾಂಕ bool drop_interprets =
        ((flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_ಕೀಲಿ_ಮೌಲ್ಯಗಳು) && drop_ಬಳಸದ);
    /* TODO: print all default values if the explicit flag is set */

    ಆದರೆ (keymap->compat_section_name)
        write_buf(buf, "xkb_compatibility \"%s\" {\n",
                  keymap->compat_section_name);
    ಇಲ್ಲದಿದ್ದರೆ
        copy_ಗಮ್ಯ_buf(buf, "xkb_compatibility {\n");

    ಆದರೆ (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        ಹಿಂತಿರುಗಿ false;

    ಆದರೆ (defaults && !write_actions_defaults(keymap, format, buf))
        ಹಿಂತಿರುಗಿ false;

    copy_ಗಮ್ಯ_buf(buf, "\tinterpret.useModMapMods= AnyLevel;\n");
    copy_ಗಮ್ಯ_buf(buf, "\tinterpret.repeat= False;\n");

    /* xkbcomp requires at least one interpret entry. */
    ಸ್ಥಿರಾಂಕ bool use_fallback_interpret =
        (!keymap->ಅಂಕೆ_sym_interprets || drop_interprets);
    ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t ಅಂಕೆ_sym_interprets =
        (use_fallback_interpret ? 1 : keymap->ಅಂಕೆ_sym_interprets);
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_sym_interpret* ಸ್ಥಿರಾಂಕ sym_interprets =
        (use_fallback_interpret ? &fallback_interpret : keymap->sym_interprets);

    ಗಾಗಿ (darray_ಗಾತ್ರ_t i = 0; i < ಅಂಕೆ_sym_interprets; i++) {
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_sym_interpret *si = &sym_interprets[i];
        ಆದರೆ (!si->required && drop_ಬಳಸದ)
            ಮುಂದುವರಿ;

        *some_interp = true;

        copy_ಗಮ್ಯ_buf(buf, "\tinterpret ");
        ಆದರೆ (!si->sym) {
            copy_ಗಮ್ಯ_buf(buf, "Any");
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (pretty) {
            write_buf(buf, "%s", Keysym_ಪಠ್ಯ(keymap->ctx, si->sym));
        } ಇಲ್ಲದಿದ್ದರೆ {
            write_buf(buf, "0x%"PRIx32, si->sym);
        }
        write_buf(buf, "+%s(%s) {",
                  SIMatch_ಪಠ್ಯ(si->ಹೊಂದಿಕೆ),
                  Mod_ಮುಖವಾಡ_ಪಠ್ಯ(keymap->ctx, MOD_BOTH, &keymap->mods, si->mods));

        bool has_explicit_properties = false;

        ಆದರೆ (si->virtual_mod != XKB_MOD_INVALID) {
            write_buf(buf, "\n\t\tvirtualModifier= %s;",
                      Mod_ಸೂಚ್ಯಂಕ_ಪಠ್ಯ(keymap->ctx, &keymap->mods,
                                   si->virtual_mod));
            has_explicit_properties = true;
        }

        ಆದರೆ (si->level_one_only) {
            copy_ಗಮ್ಯ_buf(buf, "\n\t\tuseModMapMods=level1;");
            has_explicit_properties = true;
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (defaults) {
            copy_ಗಮ್ಯ_buf(buf, "\n\t\tuseModMapMods=AnyLevel;");
            has_explicit_properties = true;
        }

        ಆದರೆ (si->repeat) {
            copy_ಗಮ್ಯ_buf(buf, "\n\t\trepeat= True;");
            has_explicit_properties = true;
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (defaults) {
            copy_ಗಮ್ಯ_buf(buf, "\n\t\trepeat= False;");
            has_explicit_properties = true;
        }

        ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action *action = ಖಾಲಿ;
        xkb_action_ಎಣಿಕೆ_t action_ಎಣಿಕೆ = si->ಅಂಕೆ_actions;
        ಆದರೆ (action_ಎಣಿಕೆ <= 1) {
            action = &si->a.action;
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (format < XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            action = &void_actions[0];
            action_ಎಣಿಕೆ = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_ಪಠ್ಯ_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        ಆದರೆ (action_ಎಣಿಕೆ > 1) {
            copy_ಗಮ್ಯ_buf(buf, "\n\t\taction= {");
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ suffix[] = ", ";
            ಗಾಗಿ (xkb_action_ಎಣಿಕೆ_t k = 0; k < si->ಅಂಕೆ_actions; k++) {
                ಆದರೆ (!write_action(keymap, format, max_groups,
                                  buf, &si->a.actions[k], "", suffix))
                    ಹಿಂತಿರುಗಿ false;
            }
            buf->ಗಾತ್ರ_2 -= ಗಾತ್ರ(suffix) - 1; /* trailing comma */
            copy_ಗಮ್ಯ_buf(buf, "};");
            has_explicit_properties = true;
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (action_ಎಣಿಕೆ == 1) {
            ಆದರೆ (!write_action(keymap, format, max_groups, buf, action,
                              "\n\t\taction= ", ";"))
                ಹಿಂತಿರುಗಿ false;
            has_explicit_properties = true;
        }
        write_buf(buf, (has_explicit_properties
                        ? "\n\t};\n"
                        /* Empty interpret is a syntax error in xkbcomp, so
                         * use a dummy entry */
                        : "\n\t\taction= NoAction();\n\t};\n"));
    }

    ಆದರೆ (use_fallback_interpret || (
        /* Detect previous interpret fallback */
        ಅಂಕೆ_sym_interprets == 1 && *some_interp &&
        sym_interprets[0].sym == fallback_interpret.sym &&
        sym_interprets[0].ಹೊಂದಿಕೆ == fallback_interpret.ಹೊಂದಿಕೆ &&
        sym_interprets[0].mods == fallback_interpret.mods &&
        sym_interprets[0].repeat == fallback_interpret.repeat &&
        sym_interprets[0].virtual_mod == fallback_interpret.virtual_mod &&
        sym_interprets[0].level_one_only == fallback_interpret.level_one_only &&
        sym_interprets[0].ಅಂಕೆ_actions == fallback_interpret.ಅಂಕೆ_actions
        /* `required` field is not compared */
    ))
        *some_interp = false;

    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        ಆದರೆ ((led->which_groups || led->groups ||
             led->which_mods || led->mods.mods || led->ctrls) &&
            !write_led_map(keymap, format, defaults, buf, led))
        {
            ಹಿಂತಿರುಗಿ false;
        }
    }

    copy_ಗಮ್ಯ_buf(buf, "};\n\n");

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
write_keysyms(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap,
              ರಚನೆ buf *buf, ರಚನೆ buf *buf2,
              ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ, xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುಂಪು,
              bool pretty, bool show_actions)
{
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ padding = (pretty)
        ? ((show_actions) ? ACTION_PADDING : ಚಿಹ್ನೆ_PADDING)
        : 0;
    ಗಾಗಿ (xkb_level_ಸೂಚ್ಯಂಕ_t level = 0; level < Xkb_ಕೀಲಿ_ಅಂಕೆ_ಮಟ್ಟಗಳು(ಕೀಲಿ, ಗುಂಪು);
         level++) {
        ಸ್ಥಿರಾಂಕ xkb_keysym_t *syms;
        ಪೂರ್ಣಾಂಕ ಅಂಕೆ_syms;

        ಆದರೆ (level != 0)
            copy_ಗಮ್ಯ_buf(buf, ", ");

        ಅಂಕೆ_syms = xkb_keymap_ಕೀಲಿ_get_syms_by_level(
            /* remove `const` constraint to use the dated public API */
            (ರಚನೆ xkb_keymap *)keymap,
            ಕೀಲಿ->keycode, ಗುಂಪು, level, &syms
        );

        ಸ್ಥಿರಾಂಕ xkb_keysym_t no_ಚಿಹ್ನೆ = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
        ಆದರೆ (ಅಂಕೆ_syms == 0) {
            ಅಂಕೆ_syms = 1;
            syms = &no_ಚಿಹ್ನೆ;
        }

        /*
         * NOTE: Use `NoSymbol` even without pretty output, for compatibility
         * with xkbcomp and xkbcommon < 1.12
         */

        ಆದರೆ (ಅಂಕೆ_syms == 1) {
            ಆದರೆ (pretty || syms[0] == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ)
                write_buf(buf, "%*s", padding, Keysym_ಪಠ್ಯ(keymap->ctx, syms[0]));
            ಇಲ್ಲದಿದ್ದರೆ
                write_buf(buf, "0x%"PRIx32, syms[0]);
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಆದರೆ (pretty) {
                buf2->ಗಾತ್ರ_2 = 0;
                copy_ಗಮ್ಯ_buf(buf2, "{ ");
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = 0; s < ಅಂಕೆ_syms; s++) {
                    ಆದರೆ (s != 0)
                        copy_ಗಮ್ಯ_buf(buf2, ", ");
                    write_buf(buf2, "%*s", (show_actions ? padding : 0),
                              Keysym_ಪಠ್ಯ(keymap->ctx, syms[s]));
                }
                copy_ಗಮ್ಯ_buf(buf2, " }");
                write_buf(buf, "%*s", padding, buf2->buf);
            } ಇಲ್ಲದಿದ್ದರೆ {
                copy_ಗಮ್ಯ_buf(buf, "{");
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = 0; s < ಅಂಕೆ_syms; s++) {
                    ಆದರೆ (s != 0)
                        copy_ಗಮ್ಯ_buf(buf, ",");
                    ಆದರೆ (syms[s] == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ)
                        copy_ಗಮ್ಯ_buf(buf, "NoSymbol");
                    ಇಲ್ಲದಿದ್ದರೆ
                        write_buf(buf, "0x%"PRIx32, syms[s]);
                }
                copy_ಗಮ್ಯ_buf(buf, "}");
            }
        }
    }

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
write_ಕೀಲಿ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap,
          ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_config *config,
          ಸ್ಥಿರಾಂಕ ಕೀಲಿ_name_substitutions *substitutions,
          xkb_layout_ಸೂಚ್ಯಂಕ_t max_groups, bool some_interprets,
          bool drop_interprets, bool explicit, bool pretty,
          ರಚನೆ buf *buf, ರಚನೆ buf *buf2, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ)
{
    bool simple = true;

    xkb_layout_ಮುಖವಾಡ_t layouts = config->layouts;
    ಆದರೆ (popcount32(layouts) != keymap->ಅಂಕೆ_groups) {
        /* Subset of layouts: use out-of-range key policy */
        xkb_layout_ಸೂಚ್ಯಂಕ_t l = 0;
        ಗಾಗಿ (xkb_layout_ಮುಖವಾಡ_t ls = layouts; ls; ls >>= 1, l++) {
            ಆದರೆ (ls & 0x1) {
                /* Replace target layout with its effective layout, if valid */
                layouts &= ~(UINT32_C(1) << l);
                ಸ್ಥಿರಾಂಕ xkb_layout_ಸೂಚ್ಯಂಕ_t effective =
                    xkb_keymap_ಕೀಲಿ_effective_layout(ಕೀಲಿ, l);
                ಆದರೆ (effective != XKB_LAYOUT_INVALID)
                    layouts |= (UINT32_C(1) << effective);
            }
        }
    } ಇಲ್ಲದಿದ್ದರೆ {
        layouts &= ((UINT64_C(1) << ಕೀಲಿ->ಅಂಕೆ_groups) - 1);
    }
    ಸ್ಥಿರಾಂಕ xkb_layout_ಸೂಚ್ಯಂಕ_t groups_ಎಣಿಕೆ = popcount32(layouts);

    ಸ್ಥಿರಾಂಕ xkb_atom_t name = (substitutions == ಖಾಲಿ)
        ? ಕೀಲಿ->name
        : substitute_name(substitutions, ಕೀಲಿ->name);

    ಆದರೆ (pretty)
        write_buf(buf, "\tkey %-20s {", ಕೀಲಿ_Name_ಪಠ್ಯ(keymap->ctx, name));
    ಇಲ್ಲದಿದ್ದರೆ
        write_buf(buf, "\tkey %s {", ಕೀಲಿ_Name_ಪಠ್ಯ(keymap->ctx, name));

    ಆದರೆ ((ಕೀಲಿ->explicit & EXPLICIT_TYPES) || explicit) {
        simple = false;

        bool multi_type = explicit;
        /* Check for distinct key types only if not requiring explicit values */
        ಆದರೆ (!explicit) {
            ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುಂಪು = 1; ಗುಂಪು < ಕೀಲಿ->ಅಂಕೆ_groups; ಗುಂಪು++) {
                ಆದರೆ (!((UINT32_C(1) << ಗುಂಪು) & layouts))
                    ಮುಂದುವರಿ;

                ಆದರೆ (ಕೀಲಿ->groups[ಗುಂಪು].type != ಕೀಲಿ->groups[0].type) {
                    multi_type = true;
                    ನಿಲ್ಲಿಸು;
                }
            }
        }

        ಆದರೆ (multi_type) {
            xkb_layout_ಸೂಚ್ಯಂಕ_t new_ಗುಂಪು = (xkb_layout_ಸೂಚ್ಯಂಕ_t)-1;
            ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುಂಪು = 0; ಗುಂಪು < ಕೀಲಿ->ಅಂಕೆ_groups; ಗುಂಪು++) {
                ಆದರೆ (!((UINT32_C(1) << ಗುಂಪು) & layouts))
                    ಮುಂದುವರಿ;

                new_ಗುಂಪು++;

                ಆದರೆ (!ಕೀಲಿ->groups[ಗುಂಪು].explicit_type && !explicit)
                    ಮುಂದುವರಿ;

                ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type * ಸ್ಥಿರಾಂಕ type = ಕೀಲಿ->groups[ಗುಂಪು].type;
                write_buf(buf, "\n\t\ttype[%"PRIu32"]= ", new_ಗುಂಪು + 1);
                write_buf_string_literal(
                  buf, xkb_atom_ಪಠ್ಯ(keymap->ctx, type->name));
                copy_ಗಮ್ಯ_buf(buf, ",");
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type * ಸ್ಥಿರಾಂಕ type = ಕೀಲಿ->groups[0].type;
            write_buf(buf, "\n\t\ttype= ");
            write_buf_string_literal(
                buf, xkb_atom_ಪಠ್ಯ(keymap->ctx, type->name));
            copy_ಗಮ್ಯ_buf(buf, ",");
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
    ಸ್ಥಿರಾಂಕ bool explicit_actions = (ಕೀಲಿ->explicit & EXPLICIT_INTERP);

    /* Check if an explicit value is required */
    #ವ್ಯಾಖ್ಯಾನಿಸು require_explicit(ಕೀಲಿ, prop,                                       \
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

    ಆದರೆ (require_explicit(ಕೀಲಿ, EXPLICIT_REPEAT, ಕೀಲಿ->repeats,
                         DEFAULT_ಕೀಲಿ_REPEAT, DEFAULT_INTERPRET_ಕೀಲಿ_REPEAT)) {
        ಆದರೆ (ಕೀಲಿ->repeats)
            copy_ಗಮ್ಯ_buf(buf, "\n\t\trepeat= Yes,");
        ಇಲ್ಲದಿದ್ದರೆ
            copy_ಗಮ್ಯ_buf(buf, "\n\t\trepeat= No,");
        simple = false;
    }

    ಆದರೆ (require_explicit(ಕೀಲಿ, EXPLICIT_VMODMAP, ಕೀಲಿ->vmodmap,
                         (xkb_mod_ಮುಖವಾಡ_t) DEFAULT_ಕೀಲಿ_VMODMAP,
                         (xkb_mod_ಮುಖವಾಡ_t) DEFAULT_INTERPRET_VMODMAP)) {
        write_buf(buf, "\n\t\tvirtualMods= %s,",
                  Mod_ಮುಖವಾಡ_ಪಠ್ಯ(keymap->ctx, MOD_BOTH, &keymap->mods,
                              ಕೀಲಿ->vmodmap));
        simple = false;
    }

    #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು require_explicit

    ಆಯ್ಕೆ (ಕೀಲಿ->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_policy) {
    ಸ್ಥಿತಿ XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_CLAMP:
        copy_ಗಮ್ಯ_buf(buf, "\n\t\tgroupsClamp,");
        simple = false;
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_REDIRECT: {
        ಆದರೆ (!groups_ಎಣಿಕೆ) {
            /* No layout to redirect to: skip */
            ನಿಲ್ಲಿಸು;
        }

        xkb_layout_ಮುಖವಾಡ_t layout_ಮುಖವಾಡ =
            (UINT32_C(1) << ಕೀಲಿ->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ);
        ಆದರೆ (!(layouts & layout_ಮುಖವಾಡ)) {
            /*
             * Layout is not included: default to first layout
             * See: XkbWrapGroupIntoRange
             */
            log_warn(keymap->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                     "key %s: groupsRedirect=%"PRIu32" converted to "
                     "groupsRedirect=1\n",
                     ಕೀಲಿ_Name_ಪಠ್ಯ(keymap->ctx, name),
                     ಕೀಲಿ->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ + 1);
            layout_ಮುಖವಾಡ = 0x1;
        }

        /* In case some lower layouts were discarded, update the group index */
        ಸ್ಥಿರಾಂಕ xkb_layout_ಮುಖವಾಡ_t ಕೆಳಗಿನ = (layouts & (layout_ಮುಖವಾಡ - 1));
        ಸ್ಥಿರಾಂಕ xkb_layout_ಸೂಚ್ಯಂಕ_t new = popcount32(ಕೆಳಗಿನ);
        write_buf(buf, "\n\t\tgroupsRedirect= %"PRIu32",", new + 1);
        simple = false;
        ನಿಲ್ಲಿಸು;
    }

    ಸ್ಥಿತಿ XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_WRAP:
    ಪೂರ್ವನಿಯೋಜಿತ:
        ನಿಲ್ಲಿಸು;
    }

    ಆದರೆ (ಕೀಲಿ->overlays) {
        xkb_overlay_ಮುಖವಾಡ_t ಉಳಿದ = ಕೀಲಿ->overlays;

        ಸ್ಥಿರಾಂಕ xkb_overlay_ಸೂಚ್ಯಂಕ_t overlay_max =
            format_max_overlays(config->format);
        static_assert(XKB_OVERLAY_ಎಣಿಕೆ == 8, "invalid right shift");
        ಸ್ಥಿರಾಂಕ xkb_overlay_ಮುಖವಾಡ_t valid =
            (xkb_overlay_ಮುಖವಾಡ_t)((1u << overlay_max) - 1u);
        ಉಳಿದ &= valid;
        ಆದರೆ (ಉಳಿದ != ಕೀಲಿ->overlays) {
            log_warn(keymap->ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_ಸೂಚ್ಯಂಕ,
                     "Overlays indices > %u in %s require using "
                     "keymap format >= v2; "
                     "keep overlays 0x%08x and discard overlays 0x%08x\n",
                     overlay_max, ಕೀಲಿ_Name_ಪಠ್ಯ(keymap->ctx, name),
                     ಉಳಿದ, ಕೀಲಿ->overlays & ~valid);
        }

        ಆದರೆ (ಉಳಿದ && !ಕೀಲಿ->overlays_inline &&
            !areOverlappingOverlaysSupported(config->format)) {
            /* isolate lowest set bit */
            ಸ್ಥಿರಾಂಕ xkb_overlay_ಮುಖವಾಡ_t lsb = (xkb_overlay_ಮುಖವಾಡ_t)(
                ಉಳಿದ &
                (xkb_overlay_ಮುಖವಾಡ_t)((ಚಿಹ್ನೆರಹಿತ)~ಉಳಿದ + 1u)
            );
            /* get its index */
            ಸ್ಥಿರಾಂಕ xkb_overlay_ಸೂಚ್ಯಂಕ_t overlay =
                (xkb_overlay_ಸೂಚ್ಯಂಕ_t)popcount32(lsb - 1u);
            log_warn(keymap->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                     "Overlapping overlays in %s require using "
                     "keymap format >= v2; "
                     "keep overlay %u and discard overlays 0x%x\n",
                     ಕೀಲಿ_Name_ಪಠ್ಯ(keymap->ctx, name), overlay, ಉಳಿದ & ~lsb);
            ಉಳಿದ = lsb;
        }

        ಆದರೆ (ಉಳಿದ)
            simple = false;

        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ * ಸ್ಥಿರಾಂಕ *overlays_ಕೀಲಿಗಳು = (ಕೀಲಿ->overlays_inline)
                                                    ? &ಕೀಲಿ->overlay_ಕೀಲಿ
                                                    : ಕೀಲಿ->overlays_ಕೀಲಿಗಳು;
        ಆಗ (ಉಳಿದ) {
            /* isolate lowest set bit */
            ಸ್ಥಿರಾಂಕ xkb_overlay_ಮುಖವಾಡ_t lsb = (xkb_overlay_ಮುಖವಾಡ_t)(
                ಉಳಿದ &
                (xkb_overlay_ಮುಖವಾಡ_t)((ಚಿಹ್ನೆರಹಿತ)~ಉಳಿದ + 1u)
            );
            /* get its index */
            ಸ್ಥಿರಾಂಕ xkb_overlay_ಸೂಚ್ಯಂಕ_t overlay =
                (xkb_overlay_ಸೂಚ್ಯಂಕ_t)popcount32(lsb - 1u);
            ಉಳಿದ = (ಉಳಿದ & ~lsb);

            /* pop current value */
            ಆದರೆ (ಕೀಲಿ->overlays_inline && ಉಳಿದ)
                PANIC_UNREACHABLE();
            ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *overlay_ಕೀಲಿ = *(overlays_ಕೀಲಿಗಳು++);

            ಸ್ಥಿರಾಂಕ xkb_atom_t overlay_ಕೀಲಿ_name = (substitutions == ಖಾಲಿ)
                ? overlay_ಕೀಲಿ->name
                : substitute_name(substitutions, overlay_ಕೀಲಿ->name);
            write_buf(buf, "\n\t\toverlay%u= %s,",
                      overlay + 1, ಕೀಲಿ_Name_ಪಠ್ಯ(keymap->ctx, overlay_ಕೀಲಿ_name));
        }
    }

    ಆದರೆ (groups_ಎಣಿಕೆ > 1 || explicit_actions || explicit)
        simple = false;

    ಆದರೆ (simple) {
        ಆದರೆ (groups_ಎಣಿಕೆ == 0) {
            /* Remove trailing comma */
            delete_ಕೊನೆಯ_char(buf, ',');
            ಆದರೆ (buf->buf[buf->ಗಾತ್ರ_2 - 1] == ',')
                buf->ಗಾತ್ರ_2--;
        } ಇಲ್ಲದಿದ್ದರೆ {
            copy_ಗಮ್ಯ_buf(buf, "\t[ ");
            /* Get the single group index */
            xkb_mod_ಸೂಚ್ಯಂಕ_t ಗುಂಪು = ctz32(layouts);
            /* The group may not be defined for this key if it is not the first one */
            ಗುಂಪು = xkb_keymap_ಕೀಲಿ_effective_layout(ಕೀಲಿ, ಗುಂಪು);
            ಆದರೆ (!write_keysyms(keymap, buf, buf2, ಕೀಲಿ, ಗುಂಪು, pretty, false))
                ಹಿಂತಿರುಗಿ false;
            copy_ಗಮ್ಯ_buf(buf, " ]");
        }
        copy_ಗಮ್ಯ_buf(buf, " };\n");
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        xkb_layout_ಸೂಚ್ಯಂಕ_t new_ಗುಂಪು = (xkb_layout_ಸೂಚ್ಯಂಕ_t)-1;
        ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುಂಪು = 0; ಗುಂಪು < ಕೀಲಿ->ಅಂಕೆ_groups; ಗುಂಪು++) {
            ಆದರೆ (!((UINT32_C(1) << ಗುಂಪು) & layouts))
                ಮುಂದುವರಿ;

            new_ಗುಂಪು++;

            ಸ್ಥಿರಾಂಕ bool print_actions =
                /* Group has explicit actions */
                ಕೀಲಿ->groups[ಗುಂಪು].explicit_actions ||
                /* Explicit values are required */
                explicit ||
                /* Group has symbols but no explicit actions and key has explicit
                 * actions in another group: ensure compatibility with xkbcomp
                 * and all xkbcommon versions */
                (ಕೀಲಿ->groups[ಗುಂಪು].explicit_symbols &&
                 explicit_actions && some_interprets) ||
                /* Group has implicit actions but no interpret can set them, so
                 * the actions must be made explicit */
                (ಕೀಲಿ->groups[ಗುಂಪು].implicit_actions && !some_interprets);

            ಆದರೆ (new_ಗುಂಪು != 0)
                copy_ಗಮ್ಯ_buf(buf, ",");
            write_buf(buf, "\n\t\tsymbols[%"PRIu32"]= [ ", new_ಗುಂಪು + 1);

            ಆದರೆ (!write_keysyms(keymap, buf, buf2, ಕೀಲಿ, ಗುಂಪು,
                               pretty, print_actions))
                ಹಿಂತಿರುಗಿ false;
            copy_ಗಮ್ಯ_buf(buf, " ]");
            ಆದರೆ (print_actions) {
                write_buf(buf, ",\n\t\tactions[%"PRIu32"]= [ ", new_ಗುಂಪು + 1);
                ಆದರೆ (!write_actions(keymap, config->format, max_groups,
                                   buf, buf2, ಕೀಲಿ, ಗುಂಪು))
                    ಹಿಂತಿರುಗಿ false;
                copy_ಗಮ್ಯ_buf(buf, " ]");
            }
        }
        ಆದರೆ (!groups_ಎಣಿಕೆ)
            delete_ಕೊನೆಯ_char(buf, ',');
        copy_ಗಮ್ಯ_buf(buf, "\n\t};\n");
    }

    ಹಿಂತಿರುಗಿ true;
}


ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
cmp_modmap_ಹೆಜ್ಜೆ_1(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b)
{
    ಸ್ಥಿರಾಂಕ Mod_Map_ನಮೂದು *m1 = (ಸ್ಥಿರಾಂಕ Mod_Map_ನಮೂದು *)a;
    ಸ್ಥಿರಾಂಕ Mod_Map_ನಮೂದು *m2 = (ಸ್ಥಿರಾಂಕ Mod_Map_ನಮೂದು *)b;

    ಆದರೆ (!m1->mods) {
        ಆದರೆ (!m2->mods) {
            /* Entry will be dropped: do not care about the other fields */
            ಹಿಂತಿರುಗಿ 0;
        }
        ಹಿಂತಿರುಗಿ +1;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!m2->mods) {
        ಹಿಂತಿರುಗಿ -1;
    }

    /* Valid only on step 1 */
    ಆದರೆ (m1->ಕೀಲಿ_ಸಂಕೇತ < m2->ಕೀಲಿ_ಸಂಕೇತ)
        ಹಿಂತಿರುಗಿ -1;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (m1->ಕೀಲಿ_ಸಂಕೇತ > m2->ಕೀಲಿ_ಸಂಕೇತ)
        ಹಿಂತಿರುಗಿ +1;

    ಆದರೆ (m1->mods < m2->mods)
        ಹಿಂತಿರುಗಿ -1;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (m1->mods > m2->mods)
        ಹಿಂತಿರುಗಿ +1;

    ಆದರೆ (m1->have_ಚಿಹ್ನೆ < m2->have_ಚಿಹ್ನೆ)
        ಹಿಂತಿರುಗಿ +1;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (m1->have_ಚಿಹ್ನೆ > m2->have_ಚಿಹ್ನೆ)
        ಹಿಂತಿರುಗಿ -1;

    ಆದರೆ (m1->have_ಚಿಹ್ನೆ) {
        ಆದರೆ (m1->u.ಕೀಲಿ_Sym < m2->u.ಕೀಲಿ_Sym)
            ಹಿಂತಿರುಗಿ -1;
        ಆದರೆ (m1->u.ಕೀಲಿ_Sym > m2->u.ಕೀಲಿ_Sym)
            ಹಿಂತಿರುಗಿ +1;
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಆದರೆ (m1->u.ಕೀಲಿ_Name < m2->u.ಕೀಲಿ_Name)
            ಹಿಂತಿರುಗಿ -1;
        ಆದರೆ (m1->u.ಕೀಲಿ_Name > m2->u.ಕೀಲಿ_Name)
            ಹಿಂತಿರುಗಿ +1;
    }

    ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
cmp_modmap_ಹೆಜ್ಜೆ_2(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b)
{
    ಸ್ಥಿರಾಂಕ Mod_Map_ನಮೂದು *m1 = (ಸ್ಥಿರಾಂಕ Mod_Map_ನಮೂದು *)a;
    ಸ್ಥಿರಾಂಕ Mod_Map_ನಮೂದು *m2 = (ಸ್ಥಿರಾಂಕ Mod_Map_ನಮೂದು *)b;

    ಆದರೆ (m1->mods < m2->mods)
        ಹಿಂತಿರುಗಿ -1;
    ಆದರೆ (m1->mods > m2->mods)
        ಹಿಂತಿರುಗಿ +1;

    ಆದರೆ (m1->have_ಚಿಹ್ನೆ < m2->have_ಚಿಹ್ನೆ)
        ಹಿಂತಿರುಗಿ -1;
    ಆದರೆ (m1->have_ಚಿಹ್ನೆ > m2->have_ಚಿಹ್ನೆ)
        ಹಿಂತಿರುಗಿ +1;

    ಆದರೆ (m1->have_ಚಿಹ್ನೆ) {
        ಆದರೆ (m1->u.ಕೀಲಿ_Sym < m2->u.ಕೀಲಿ_Sym)
            ಹಿಂತಿರುಗಿ -1;
        ಆದರೆ (m1->u.ಕೀಲಿ_Sym > m2->u.ಕೀಲಿ_Sym)
            ಹಿಂತಿರುಗಿ +1;
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಆದರೆ (m1->ಕೀಲಿ_ಸಂಕೇತ < m2->ಕೀಲಿ_ಸಂಕೇತ)
            ಹಿಂತಿರುಗಿ -1;
        ಆದರೆ (m1->ಕೀಲಿ_ಸಂಕೇತ > m2->ಕೀಲಿ_ಸಂಕೇತ)
            ಹಿಂತಿರುಗಿ +1;
    }

    ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ bool
write_modmaps(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap, ಎಣಿಕೆ xkb_keymap_format format,
              ಸ್ಥಿರಾಂಕ ಕೀಲಿ_name_substitutions *substitutions,
              bool pretty, bool drop_interprets, ರಚನೆ buf *buf)
{
    ಆದರೆ (!keymap->ಅಂಕೆ_modmaps)
        ಹಿಂತಿರುಗಿ true;

    darray(Mod_Map_ನಮೂದು) modmaps = darray_new();
    darray_from_ವಸ್ತುಗಳು(modmaps, keymap->modmaps, keymap->ಅಂಕೆ_modmaps);
    ಆದರೆ (!darray_ವಸ್ತುಗಳು(modmaps))
        ಹಿಂತಿರುಗಿ false;

    /* Sort entries by key code */
    qsort(darray_ವಸ್ತುಗಳು(modmaps), darray_ಗಾತ್ರ(modmaps),
          ಗಾತ್ರ(*darray_ವಸ್ತುಗಳು(modmaps)), &cmp_modmap_ಹೆಜ್ಜೆ_1);

    /* Drop disabled entries at the end */
    ಗಾಗಿ (darray_ಗಾತ್ರ_t m = darray_ಗಾತ್ರ(modmaps); m-- > 0;) {
        ಆದರೆ (darray_ಅಂಶ(modmaps, m).mods)
            ನಿಲ್ಲಿಸು;
        darray_ಗಾತ್ರ(modmaps)--;
    }

    /*
     * Build detailed keymap modmap
     *
     * NOTE: use an explicit for-loop instead of the darray_foreach macro
     * because we may add entries while iterating it. Iterate from the end,
     * so that we do not iterate the new entries.
     */
    xkb_keycode_t current_keycode = XKB_KEYCODE_INVALID;
    bool convert_ಗಮ್ಯ_v1_compatibility_ನಮೂದು = false;
    ಗಾಗಿ (darray_ಗಾತ್ರ_t m = darray_ಗಾತ್ರ(modmaps); m-- > 0;) {
        Mod_Map_ನಮೂದು *mm = &darray_ಅಂಶ(modmaps, m);
        ಪ್ರತಿಜ್ಞಾ(mm->mods);

        ಆದರೆ (mm->ಕೀಲಿ_ಸಂಕೇತ == current_keycode) {
            /*
             * Keycode already processed: either drop next key code entries
             * or convert them to keymap format v1
             */
            ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t core_mods = (mm->mods & MOD_REAL_ಮುಖವಾಡ_ALL);
            ಆದರೆ (convert_ಗಮ್ಯ_v1_compatibility_ನಮೂದು && core_mods) {
                /*
                 * Convert to keymap format V1-specific entry:
                 * keep only the highest core modifier
                 */
                mm->mods = UINT32_C(1) << (XKB_MAX_MODS - 1 - clz32(core_mods));
            } ಇಲ್ಲದಿದ್ದರೆ {
                /*
                 * Drop if there is a previous entry for all keymap formats or
                 * if the current entry is incompatible with V1 keymap format.
                 */
                mm->mods = 0;
            }
            ಮುಂದುವರಿ;
        }

        current_keycode = mm->ಕೀಲಿ_ಸಂಕೇತ;
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ = Xkb_ಕೀಲಿ(keymap, mm->ಕೀಲಿ_ಸಂಕೇತ);

        ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t core_mods = (ಕೀಲಿ->modmap & MOD_REAL_ಮುಖವಾಡ_ALL);
        ಆದರೆ (format == XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1 &&
            ((ಕೀಲಿ->modmap & (ಕೀಲಿ->modmap - 1)) || ಕೀಲಿ->modmap != core_mods)) {
            /* V1 entry with multiple modifiers or non X11 core modifiers */
            m++; /* process in next iteration */
            convert_ಗಮ್ಯ_v1_compatibility_ನಮೂದು = true;
            log_err(keymap->ctx, XKB_ERROR_INVALID_MODIFIER_MAP_ಮುಖವಾಡ,
                    "Cannot serialize modifier map of key %s: modifiers mask "
                    "0x%"PRIx32" incompatible with keymap format v1\n",
                    ಕೀಲಿ_Name_ಪಠ್ಯ(keymap->ctx, ಕೀಲಿ->name), ಕೀಲಿ->modmap);
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* V2 format or V1 format with a single core modifier */
            ಪ್ರತಿಜ್ಞಾ(ಕೀಲಿ->modmap);
            mm->mods = ಕೀಲಿ->modmap;
            /* Force key name */
            mm->have_ಚಿಹ್ನೆ = false;
            mm->u.ಕೀಲಿ_Name = ಕೀಲಿ->name;
            convert_ಗಮ್ಯ_v1_compatibility_ನಮೂದು = false;
        }
    }

    ಆದರೆ (darray_ಖಾಲಿಗಳು(modmaps))
        ಹೋಗು ನಿರ್ಗಮಿಸು;

    /* Sort entries by modifier mask */
    qsort(darray_ವಸ್ತುಗಳು(modmaps), darray_ಗಾತ್ರ(modmaps),
          ಗಾತ್ರ(*darray_ವಸ್ತುಗಳು(modmaps)), &cmp_modmap_ಹೆಜ್ಜೆ_2);

    /*
     * Serialize modmaps
     */

    xkb_mod_ಮುಖವಾಡ_t current_mods = 0;
    ಸ್ಥಿರಾಂಕ Mod_Map_ನಮೂದು *ನಮೂದು;
    darray_foreach(ನಮೂದು, modmaps) {
        ಆದರೆ (!ನಮೂದು->mods)
            ಮುಂದುವರಿ;

        ಆದರೆ (current_mods != ನಮೂದು->mods) {
            ಆದರೆ (current_mods) {
                copy_ಗಮ್ಯ_buf(buf, " };\n");
            }
            current_mods = ನಮೂದು->mods;
            write_buf(buf, "\tmodifier_map %s { ",
                      Mod_ಮುಖವಾಡ_ಪಠ್ಯ(keymap->ctx, MOD_REAL, &keymap->mods,
                                  current_mods));
        } ಇಲ್ಲದಿದ್ದರೆ {
            copy_ಗಮ್ಯ_buf(buf, ", ");
        }

        ಆದರೆ (ನಮೂದು->have_ಚಿಹ್ನೆ) {
            ಆದರೆ (pretty || ನಮೂದು->u.ಕೀಲಿ_Sym == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ)
                write_buf(buf, "%s", Keysym_ಪಠ್ಯ(keymap->ctx, ನಮೂದು->u.ಕೀಲಿ_Sym));
            ಇಲ್ಲದಿದ್ದರೆ
                write_buf(buf, "0x%"PRIx32, ನಮೂದು->u.ಕೀಲಿ_Sym);
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಸ್ಥಿರಾಂಕ xkb_atom_t name = (substitutions == ಖಾಲಿ)
                ? ನಮೂದು->u.ಕೀಲಿ_Name
                : substitute_name(substitutions, ನಮೂದು->u.ಕೀಲಿ_Name);
            write_buf(buf, "%s", ಕೀಲಿ_Name_ಪಠ್ಯ(keymap->ctx, name));
        }
    }
    ಆದರೆ (current_mods) {
        copy_ಗಮ್ಯ_buf(buf, " };\n");
    }

ನಿರ್ಗಮಿಸು:
    darray_free(modmaps);
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
write_symbols(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap,
              ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_config *config,
              ಸ್ಥಿರಾಂಕ ಕೀಲಿ_name_substitutions *substitutions,
              xkb_layout_ಸೂಚ್ಯಂಕ_t max_groups, bool some_interp,
              ರಚನೆ buf *buf)
{
    ಸ್ಥಿರಾಂಕ bool pretty = (config->flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ಸ್ಥಿರಾಂಕ bool defaults =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_ಮೌಲ್ಯಗಳು);
    ಸ್ಥಿರಾಂಕ bool explicit_ಕೀಲಿ_ಮೌಲ್ಯಗಳು =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_ಕೀಲಿ_ಮೌಲ್ಯಗಳು);
    ಸ್ಥಿರಾಂಕ bool drop_ಬಳಸದ = !(config->flags & XKB_KEYMAP_SERIALIZE_KEEP_ಬಳಸದ);
    ಸ್ಥಿರಾಂಕ bool drop_interprets = (explicit_ಕೀಲಿ_ಮೌಲ್ಯಗಳು && drop_ಬಳಸದ);
    /* TODO: print all default values if explicit flag is set */

    ಆದರೆ (keymap->symbols_section_name)
        write_buf(buf, "xkb_symbols \"%s\" {\n", keymap->symbols_section_name);
    ಇಲ್ಲದಿದ್ದರೆ
        copy_ಗಮ್ಯ_buf(buf, "xkb_symbols {\n");

    bool has_ಗುಂಪು_ಹೆಸರುಗಳು = false;
    xkb_layout_ಸೂಚ್ಯಂಕ_t new_ಗುಂಪು = (xkb_layout_ಸೂಚ್ಯಂಕ_t)-1;
    ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುಂಪು = 0; ಗುಂಪು < keymap->ಅಂಕೆ_ಗುಂಪು_ಹೆಸರುಗಳು; ಗುಂಪು++) {
        ಆದರೆ (!((UINT32_C(1) << ಗುಂಪು) & config->layouts))
            ಮುಂದುವರಿ;

        new_ಗುಂಪು++;

        ಆದರೆ (keymap->ಗುಂಪು_ಹೆಸರುಗಳು[ಗುಂಪು]) {
            write_buf(buf, "\tname[%"PRIu32"]=", new_ಗುಂಪು + 1);
            write_buf_string_literal(
                buf, xkb_atom_ಪಠ್ಯ(keymap->ctx, keymap->ಗುಂಪು_ಹೆಸರುಗಳು[ಗುಂಪು]));
            copy_ಗಮ್ಯ_buf(buf, ";\n");
            has_ಗುಂಪು_ಹೆಸರುಗಳು = true;
        }
    }
    ಆದರೆ (has_ಗುಂಪು_ಹೆಸರುಗಳು)
        copy_ಗಮ್ಯ_buf(buf, "\n");

    ಆದರೆ (defaults && !write_actions_defaults(keymap, config->format, buf))
        ಹಿಂತಿರುಗಿ false;

    ರಚನೆ buf buf2 = { ಖಾಲಿ, 0, 0 };
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ;
    xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap) {
        /* Skip keys with no explicit values */
        ಆದರೆ (ಕೀಲಿ->explicit &&
            !write_ಕೀಲಿ(keymap, config, substitutions, max_groups, some_interp,
                       drop_interprets, explicit_ಕೀಲಿ_ಮೌಲ್ಯಗಳು, pretty, buf,
                       &buf2, ಕೀಲಿ))
        {
            ಬಿಡುಗಡೆ(buf2.buf);
            ಹಿಂತಿರುಗಿ false;
        }
    }
    ಬಿಡುಗಡೆ(buf2.buf);

    ಆದರೆ (!write_modmaps(keymap, config->format, substitutions,
                       pretty, drop_interprets, buf))
        ಹಿಂತಿರುಗಿ false;

    copy_ಗಮ್ಯ_buf(buf, "};\n\n");
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
write_keymap(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap,
             ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_config *config,
             ರಚನೆ buf *buf, ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ *ಫಲಿತಾಂಶ)
{
    ಕೀಲಿ_name_substitutions substitutions = darray_new();
    ಆದರೆ (config->format == XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1 &&
        !rename_long_ಕೀಲಿಗಳು(keymap, &substitutions))
    {
        ಹಿಂತಿರುಗಿ false;
    }

    ಸ್ಥಿರಾಂಕ ಕೀಲಿ_name_substitutions * ಸ್ಥಿರಾಂಕ substitutions_ptr =
        (darray_ಖಾಲಿಗಳು(substitutions)) ? ಖಾಲಿ : &substitutions;

    ಸ್ಥಿರಾಂಕ xkb_layout_ಸೂಚ್ಯಂಕ_t max_groups = format_max_groups(config->format);

    bool some_interp = false;
    ಸ್ಥಿರಾಂಕ bool ok = (
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
    ಹಿಂತಿರುಗಿ ok ? XKB_SUCCESS : -1;
}

ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
ಪಠ್ಯ_v1_keymap_serialize(
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap,
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_config *config,
        ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ *ಫಲಿತಾಂಶ
)
{
    ರಚನೆ buf buf = { ಖಾಲಿ, 0, 0 };

    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2 =
        write_keymap(keymap, config, &buf, ಫಲಿತಾಂಶ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != XKB_SUCCESS) {
        ಬಿಡುಗಡೆ(buf.buf);
        ಫಲಿತಾಂಶ->serialized = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
    }

    ಫಲಿತಾಂಶ->serialized = buf.buf;
    ಫಲಿತಾಂಶ->ಉದ್ದಳತೆ = buf.ಗಾತ್ರ_2 + 1;
    ಫಲಿತಾಂಶ->layouts = config->layouts;

    ಹಿಂತಿರುಗಿ XKB_SUCCESS;
}
