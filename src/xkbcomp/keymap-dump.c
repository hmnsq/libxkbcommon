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

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon-status.h"
#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon-keysyms.h"
#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "atom.h"
#ଅନ୍ତର୍ଭାଵ "action.h"
#ଅନ୍ତର୍ଭାଵ "context.h"
#ଅନ୍ତର୍ଭାଵ "darray.h"
#ଅନ୍ତର୍ଭାଵ "keymap.h"
#ଅନ୍ତର୍ଭାଵ "messages-codes.h"
#ଅନ୍ତର୍ଭାଵ "text.h"
#ଅନ୍ତର୍ଭାଵ "util-numbers.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp-priv.h"

#define BUF_CHUNK_SIZE 4096

ସଂରଚନା buf {
    ଅକ୍ଷର *buf;
    size_t size;
    size_t alloc;
};

#define xkb_abs(n) _Generic((n),                \
                            ପୂର୍ଣ୍ଣସଂଖ୍ୟା: abs,           \
                            long ପୂର୍ଣ୍ଣସଂଖ୍ୟା: labs,     \
                            ପୂର୍ବନିର୍ଦ୍ଧାରିତ: llabs )((n))

ସ୍ଥିର bool
do_realloc(ସଂରଚନା buf *buf, size_t at_least)
{
    buf->alloc += BUF_CHUNK_SIZE;
    ଯଦି (at_least >= BUF_CHUNK_SIZE)
        buf->alloc += at_least;

    ଅକ୍ଷର *ଅଚଳ new = realloc(buf->buf, buf->alloc);
    ଯଦି (!new)
        ଫେରନ୍ତୁ false;

    buf->buf = new;
    ଫେରନ୍ତୁ true;
}

ATTR_PRINTF(2, 3) ସ୍ଥିର bool
check_write_buf(ସଂରଚନା buf *buf, ଅଚଳ ଅକ୍ଷର *fmt, ...)
{
    va_list args;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା printed;
    size_t available;

    available = buf->alloc - buf->size;
    /* Try to write to the buffer and get the required min size (without the
     * final null byte).
     * C11 states that if `available` is 0, then the pointer may be null. */
    ପ୍ରତିଜ୍ଞା(buf->buf || available == 0);
    va_start(args, fmt);
    printed = vsnprintf(buf->buf ? buf->buf + buf->size : buf->buf, available,
                        fmt, args);
    va_end(args);

    ଯଦି (printed < 0) {
        /* Some error */
        ଯାଅ err;
    } ଅନ୍ୟଥା ଯଦି ((size_t)printed >= available) {
        /* Not enough space: need to realloc */
        ଯଦି (!do_realloc(buf, (size_t)printed + 1))
            ଯାଅ err;

        /* The buffer has enough space now. */
        available = buf->alloc - buf->size;
        ପ୍ରତିଜ୍ଞା(buf->buf && (size_t) printed < available);
        va_start(args, fmt);
        printed = vsnprintf(buf->buf + buf->size, available, fmt, args);
        va_end(args);

        ଯଦି (printed < 0 || (size_t) printed >= available)
            ଯାଅ err;
    }

    buf->size += (size_t)printed;
    ଫେରନ୍ତୁ true;

err:
    ମୁକ୍ତ(buf->buf);
    buf->buf = ଖାଲି;
    ଫେରନ୍ତୁ false;
}

#define write_buf(buf, ...) କର { \
    ଯଦି (!check_write_buf(buf, __VA_ARGS__)) \
        ଫେରନ୍ତୁ false; \
} ଯେତେବେଳେ (0)

ସ୍ଥିର bool
check_copy_to_buf(ସଂରଚନା buf *buf, ଅଚଳ ଅକ୍ଷର* source, size_t len)
{
    ଯଦି (len == 0)
        ଫେରନ୍ତୁ true;

    ଅଚଳ size_t available = buf->alloc - buf->size;
    ଯଦି (len >= available &&
        /* len + 1 (terminating NULL) */
        !do_realloc(buf, len + 1))
    {
        ମୁକ୍ତ(buf->buf);
        buf->buf = ଖାଲି;
        ଫେରନ୍ତୁ false;
    }

    memcpy(buf->buf + buf->size, source, len);
    buf->size += len;
    /* Append NULL byte */
    buf->buf[buf->size] = '\0';
    ଫେରନ୍ତୁ true;
}

#define copy_to_buf_len(buf, source, len) କର { \
    ଯଦି (!check_copy_to_buf(buf, source, len)) \
        ଫେରନ୍ତୁ false; \
} ଯେତେବେଳେ (0)

#define copy_to_buf(buf, source) \
    copy_to_buf_len(buf, source, ଆକାର(source) - 1)

ସ୍ଥିର bool
check_write_string_literal(ସଂରଚନା buf *buf, ଅଚଳ ଅକ୍ଷର* string)
{
    copy_to_buf(buf, "\"");

    /* Write chunks, separated by characters requiring escape sequence */
    size_t pending_start = 0;
    size_t current = 0;
    ଅକ୍ଷର escape_buffer[] = {'\\', '0', '0', '0', '0'};
    ପାଇଁ (; string[current] != '\0'; current++) {
        uint8_t escape_len;
        ବାଛ (string[current]) {
        ସ୍ଥିତି '\n':
            /* `\n` would break strings */
            escape_buffer[1] = 'n';
            escape_len = 2;
            ଭାଙ୍ଗ;
        ସ୍ଥିତି '\\':
            /* `\` would create escape sequences */
            escape_buffer[1] = '\\';
            escape_len = 2;
            ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
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
            ଯଦି (unlikely((unsigned ଅକ୍ଷର)string[current] < 0x20U ||
                         string[current] == '"')) {
                escape_buffer[1] = '0';
                escape_buffer[2] = '0';
                escape_buffer[3] = (ଅକ୍ଷର)('0' + (string[current] >> 3));
                escape_buffer[4] = (ଅକ୍ଷର)('0' + (string[current] & 0x7));
                escape_len = 5;
            } ଅନ୍ୟଥା {
                /* Expand the chunk */
                ଜାରି;
            }
        }
        /* Write pending chunk */
        ପ୍ରତିଜ୍ଞା(current >= pending_start);
        copy_to_buf_len(buf, string + pending_start, current - pending_start);
        /* Write escape sequence */
        copy_to_buf_len(buf, escape_buffer, escape_len);
        pending_start = current + 1;
    }
    /* Write remaining chunk */
    ପ୍ରତିଜ୍ଞା(current >= pending_start);
    copy_to_buf_len(buf, string + pending_start, current - pending_start);

    copy_to_buf(buf, "\"");
    ଫେରନ୍ତୁ true;
}

#define write_buf_string_literal(buf, string) କର { \
    ଯଦି (!check_write_string_literal(buf, string)) \
        ଫେରନ୍ତୁ false; \
} ଯେତେବେଳେ (0)

ସ୍ଥିର ଶୂନ୍ୟ
delete_last_char(ସଂରଚନା buf *buf, ଅକ୍ଷର c)
{
    ଯଦି (buf->size && buf->buf[buf->size - 1] == c)
        buf->buf[--buf->size] = '\0';
}

/** Entry for a name substituion LUT */
ସଂରଚନା ଚାବି_name_substitution {
    xkb_atom_t source;
    xkb_atom_t target;
    /** Used for intuitive renaming */
    xkb_keycode_t keycode;
};

/** Compare 2 key name substitution entries by they original name (atom) */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
cmp_ଚାବି_name_substitution(ଅଚଳ ଶୂନ୍ୟ *a, ଅଚଳ ଶୂନ୍ୟ *b)
{
    ଅଚଳ xkb_atom_t s1 = ((ସଂରଚନା ଚାବି_name_substitution *) a)->source;
    ଅଚଳ xkb_atom_t s2 = ((ସଂରଚନା ଚାବି_name_substitution *) b)->source;
    ଫେରନ୍ତୁ (s1 < s2) ? -1 : ((s1 > s2) ? 1 : 0);
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା darray(ସଂରଚନା ଚାବି_name_substitution) ଚାବି_name_substitutions;

/**
 * Rename keys that are not compatible with the original XKB protocol,
 * i.e. more that 4 bytes. The new name will be formed as prefix + 3-digit
 * hexadecimal number.
 */
ସ୍ଥିର bool
rename_long_ଚାବିଗୁଡ଼ିକ(ଅଚଳ ସଂରଚନା xkb_keymap *keymap,
                 ଚାବି_name_substitutions *substitutions)
{
    /*
     * Prefixes we want to use for the new names: ASCII range `!`..`0`.
     * They are really unlikely to be used in usual keymaps.
     * NULL denotes an unavailable prefix.
     */
    ଗଣନା {
        first = '!',
        last = '0',
        invalid = '\0'
    };
    ଅକ୍ଷର prefixes[] = {
        first, invalid /* skip ‘"’, which would look buggy otherwise */,
        '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', last
    };
    static_assert(ଆକାର(prefixes) == last - first + 1, "invalid range");

    /*
     * Traverse all the keys and aliases:
     * - Collect items with incompatible names.
     * - Remove prefixes that are already used in valid 4-char names.
     *   This ensures no name conflict will occur with the new names, without
     *   requiring an extensive check.
     */

    /* Check key names */
    ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି;
    xkb_ଚାବିଗୁଡ଼ିକ_foreach(ଚାବି, keymap) {
        ଯଦି (ଚାବି->name == XKB_ATOM_NONE)
            ଜାରି;

        ଅଚଳ ଅକ୍ଷର * ଅଚଳ name = xkb_atom_text(keymap->ctx, ଚାବି->name);
        ଅଚଳ size_t len = strlen_safe(name);
        ଯଦି (len > 4) {
            /* Key name too long: append key, pending for renaming */
            darray_append(*substitutions, (ସଂରଚନା ଚାବି_name_substitution) {
                .source = ଚାବି->name,
                .keycode = ଚାବି->keycode,
                .target = ଚାବି->name
            });
        } ଅନ୍ୟଥା ଯଦି (len == 4 && name[0] >= first && name[0] <= last) {
            /* Potential name conflict: deactivate this prefix */
            prefixes[name[0] - first] = invalid;
        }
    }

    /* Check aliases */
    ପାଇଁ (darray_size_t a = 0; a < keymap->ଅଙ୍କ_ଚାବି_aliases; a++) {
        ଅଚଳ ସଂରଚନା xkb_ଚାବି_alias * ଅଚଳ alias = &keymap->ଚାବି_aliases[a];
        ଅଚଳ ଅକ୍ଷର * ଅଚଳ name = xkb_atom_text(keymap->ctx, alias->alias);
        ଅଚଳ size_t len = strlen_safe(name);
        ଯଦି (len > 4) {
            /* Alias name too long: append alias, pending for renaming */
            darray_append(*substitutions, (ସଂରଚନା ଚାବି_name_substitution) {
                .source = alias->alias,
                .keycode = XKB_KEYCODE_INVALID, /* Too expensive to lookup */
                .target = alias->alias
            });
        } ଅନ୍ୟଥା ଯଦି (len == 4 && name[0] >= first && name[0] <= last) {
            /* Potential conflict with names substitutes: deactivate prefix */
            prefixes[name[0] - first] = invalid;
        }
    }

    ଯଦି darray_empty(*substitutions) {
        /* Nothing to do */
        ଫେରନ୍ତୁ true;
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
    ଅକ୍ଷର *prefix = prefixes;
    ଅଚଳ ଅକ୍ଷର *prefix_max = prefixes + ଆକାର(prefixes) - 1;
    ଯେତେବେଳେ (*prefix == invalid && prefix < prefix_max)
        prefix++;

    /* Keycode prefix: prefer ‘0’ if available */
    ଅକ୍ଷର keycode_prefix;
    ଯଦି (prefixes[last - first] == '0') {
        /* ‘0’ is available: use it then mark it unavailable for further use */
        keycode_prefix = last;
        prefixes[last - first] = invalid;
    } ଅନ୍ୟଥା {
        /* ‘0’ is unavailable: use first available prefix */
        keycode_prefix = *prefix;
        ଯଦି (unlikely(keycode_prefix == invalid)) {
            /*
             * Abort: no prefix available (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            darray_free(*substitutions);
            ଫେରନ୍ତୁ true;
        }
        /* Update current prefix */
        *prefix = invalid;
        ଯେତେବେଳେ (*prefix == invalid && prefix < prefix_max)
            prefix++;
    }

    /* Proceed renaming */
    uint16_t next_id = 1;
    ସଂରଚନା ଚାବି_name_substitution *sub;
    ଅକ୍ଷର buf[6] = {0}; /* 5 would suffice, but generates a compiler warning */
    darray_foreach(sub, *substitutions) {
        static_assert(XKB_KEYCODE_INVALID > 0xfff, "Invalid keys filtered out");
        ଯଦି (sub->keycode <= 0xfff) {
            /*
             * Try nice formatting with the hexadecimal keycode.
             * We use lower case to reduce possible conflict because the
             * traditional key names are upper-cased.
             */
            snprintf(buf, ଆକାର(buf), "%c%03"PRIx32,
                     keycode_prefix, sub->keycode);
            ଅଚଳ xkb_atom_t target = xkb_atom_intern(keymap->ctx,
                                                      buf, ଆକାର(buf) - 2);
            ଯଦି (target == XKB_ATOM_NONE)
                ଯାଅ error;
            sub->target = target;
        } ଅନ୍ୟଥା ଯଦି (*prefix != invalid) {
            /* Cannot use keycode for formatting: generate next arbitrary ID */
            snprintf(buf, ଆକାର(buf), "%c%03"PRIx16, *prefix, next_id);
            ଅଚଳ xkb_atom_t target = xkb_atom_intern(keymap->ctx,
                                                      buf, ଆକାର(buf) - 2);
            ଯଦି (target == XKB_ATOM_NONE)
                ଯାଅ error;
            sub->target = target;
            ଯଦି (++next_id > 0xfff) {
                *prefix = invalid;
                /* Get next available prefix */
                ଯେତେବେଳେ (*prefix == invalid && prefix < prefix_max)
                    prefix++;
                /* Reset counter */
                next_id = 1;
            }
        } ଅନ୍ୟଥା {
            /*
             * No prefix available: skip substitution (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            sub->target = sub->source;
        }
    }

    /* Now sort by the source names so we can do a binary search */
    qsort(darray_ବସ୍ତୁଗୁଡ଼ିକ(*substitutions), darray_size(*substitutions),
          ଆକାର(*darray_ବସ୍ତୁଗୁଡ଼ିକ(*substitutions)), &cmp_ଚାବି_name_substitution);

    ଫେରନ୍ତୁ true;

error:
    darray_free(*substitutions);
    /* There was a memory error */
    log_err(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate key name substitution\n");
    ଫେରନ୍ତୁ false;
}

/** Substitute a key name with a valid X11 key name */
ସ୍ଥିର xkb_atom_t
substitute_name(ଅଚଳ ଚାବି_name_substitutions *substitutions, xkb_atom_t name)
{
    /*ଦ୍ୱିମିକ ସନ୍ଧାନ*/
    darray_size_t first = 0;
    darray_size_t last = darray_size(*substitutions);

    ଯେତେବେଳେ (last > first) {
        darray_size_t mid = first + (last - 1 - first) / 2;
        ଅଚଳ xkb_atom_t source = darray_item(*substitutions, mid).source;
        ଯଦି (source < name)
            first = mid + 1;
        ଅନ୍ୟଥା ଯଦି (source > name)
            last = mid;
        ଅନ୍ୟଥା /* found it */
            ଫେରନ୍ତୁ darray_item(*substitutions, mid).target;
    }

    /* No name substitution: return original name */
    ଫେରନ୍ତୁ name;
}

ସ୍ଥିର bool
write_vmods(ଅଚଳ ସଂରଚନା xkb_keymap *keymap, ଗଣନା xkb_keymap_format format,
            bool explicit, ସଂରଚନା buf *buf)
{
    ଅଚଳ ସଂରଚନା xkb_mod *mod;
    xkb_mod_index_t vmod = 0;
    bool has_some = false;

    xkb_vmods_enumerate(vmod, mod, &keymap->mods) {
        ଯଦି (!has_some) {
            copy_to_buf(buf, "\tvirtual_modifiers ");
            has_some = true;
        } ଅନ୍ୟଥା {
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
        ଅଚଳ bool auto_canonical_mods = (format >= XKB_KEYMAP_FORMAT_TEXT_V2);

        ଯଦି ((keymap->mods.explicit_vmods & (UINT32_C(1) << vmod) ||
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

    ଯଦି (has_some)
        copy_to_buf(buf, ";\n\n");

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
write_keycodes(ଅଚଳ ସଂରଚନା xkb_keymap *keymap,
               ଅଚଳ ଚାବି_name_substitutions *substitutions,
               ଗଣନା xkb_keymap_serialize_flags flags, ସଂରଚନା buf *buf)
{
    ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି;
    xkb_led_index_t idx;
    ଅଚଳ ସଂରଚନା xkb_led *led;

    ଅଚଳ bool pretty = !!(flags & XKB_KEYMAP_SERIALIZE_PRETTY);

    ଯଦି (keymap->keycodes_section_name)
        write_buf(buf, "xkb_keycodes \"%s\" {\n",
                  keymap->keycodes_section_name);
    ଅନ୍ୟଥା
        copy_to_buf(buf, "xkb_keycodes {\n");

    /* NOTE: there is no support for default values */

    /* xkbcomp and X11 really want to see keymaps with a minimum of 8, and
     * a maximum of at least 255, else XWayland really starts hating life.
     * If this is a problem and people really need strictly bounded keymaps,
     * we should probably control this with a flag. */
    write_buf(buf, "\tminimum = %"PRIu32";\n", MIN(keymap->min_ଚାବି_code, 8));
    write_buf(buf, "\tmaximum = %"PRIu32";\n", MAX(keymap->max_ଚାବି_code, 255));

    xkb_ଚାବିଗୁଡ଼ିକ_foreach(ଚାବି, keymap) {
        ଯଦି (ଚାବି->name == XKB_ATOM_NONE)
            ଜାରି;

        ଅଚଳ xkb_atom_t name = (substitutions == ଖାଲି)
            ? ଚାବି->name
            : substitute_name(substitutions, ଚାବି->name);
        ଯଦି (pretty)
            write_buf(buf, "\t%-20s", ଚାବି_Name_Text(keymap->ctx, name));
        ଅନ୍ୟଥା
            write_buf(buf, "\t%s", ଚାବି_Name_Text(keymap->ctx, name));
        write_buf(buf, " = %"PRIu32";\n", ଚାବି->keycode);
    }

    xkb_leds_enumerate(idx, led, keymap)
        ଯଦି (led->name != XKB_ATOM_NONE) {
            write_buf(buf, "\tindicator %"PRIu32" = ", idx + 1);
            write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, led->name));
            copy_to_buf(buf, ";\n");
        }

    ପାଇଁ (darray_size_t i = 0; i < keymap->ଅଙ୍କ_ଚାବି_aliases; i++) {
        ଅଚଳ xkb_atom_t alias = (substitutions == ଖାଲି)
            ? keymap->ଚାବି_aliases[i].alias
            : substitute_name(substitutions, keymap->ଚାବି_aliases[i].alias);
        ଅଚଳ xkb_atom_t real = (substitutions == ଖାଲି)
            ? keymap->ଚାବି_aliases[i].real
            : substitute_name(substitutions, keymap->ଚାବି_aliases[i].real);

        ଯଦି (pretty)
            write_buf(buf, "\talias %-14s",
                      ଚାବି_Name_Text(keymap->ctx, alias));
        ଅନ୍ୟଥା
            write_buf(buf, "\talias %s",
                      ଚାବି_Name_Text(keymap->ctx, alias));
        write_buf(buf, " = %s;\n",
                  ଚାବି_Name_Text(keymap->ctx, real));
    }

    copy_to_buf(buf, "};\n\n");
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
write_types(ଅଚଳ ସଂରଚନା xkb_keymap *keymap, ଗଣନା xkb_keymap_format format,
            ଗଣନା xkb_keymap_serialize_flags flags, ସଂରଚନା buf *buf)
{
    ଯଦି (keymap->types_section_name)
        write_buf(buf, "xkb_types \"%s\" {\n",
                  keymap->types_section_name);
    ଅନ୍ୟଥା
        copy_to_buf(buf, "xkb_types {\n");

    /* NOTE: support for default values has been removed */

    ଯଦି (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        ଫେରନ୍ତୁ false;

    ଅଚଳ bool drop_unused = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    ପାଇଁ (darray_size_t i = 0; i < keymap->ଅଙ୍କ_types; i++) {
        ଅଚଳ ସଂରଚନା xkb_ଚାବି_type * ଅଚଳ type = &keymap->types[i];
        ଯଦି (!type->required && drop_unused)
            ଜାରି;

        copy_to_buf(buf, "\ttype ");
        write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, type->name));
        copy_to_buf(buf, " {\n");

        write_buf(buf, "\t\tmodifiers= %s;\n",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              type->mods.mods));

        ପାଇଁ (darray_size_t j = 0; j < type->ଅଙ୍କ_entries; j++) {
            ଅଚଳ ଅକ୍ଷର *str;
            ଅଚଳ ସଂରଚନା xkb_ଚାବି_type_entry *entry = &type->entries[j];

            /*
             * Printing level 1 entries is redundant, it's the default,
             * unless there's preserve info.
             */
            ଯଦି (entry->level == 0 && entry->preserve.mods == 0 &&
                !(flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_ମୂଲ୍ୟଗୁଡ଼ିକ))
                ଜାରି;

            str = ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              entry->mods.mods);
            write_buf(buf, "\t\tmap[%s]= %"PRIu32";\n",
                      str, entry->level + 1);

            ଯଦି (entry->preserve.mods)
                write_buf(buf, "\t\tpreserve[%s]= %s;\n",
                          str, ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                                           entry->preserve.mods));
        }

        ପାଇଁ (xkb_level_index_t n = 0; n < type->ଅଙ୍କ_level_names; n++)
            ଯଦି (type->level_names[n]) {
                write_buf(buf, "\t\tlevel_name[%"PRIu32"]= ", n + 1);
                write_buf_string_literal(
                    buf, xkb_atom_text(keymap->ctx, type->level_names[n]));
                copy_to_buf(buf, ";\n");
            }

        copy_to_buf(buf, "\t};\n");
    }

    copy_to_buf(buf, "};\n\n");
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
write_led_map(ଅଚଳ ସଂରଚନା xkb_keymap *keymap, ଗଣନା xkb_keymap_format format,
              bool defaults, ସଂରଚନା buf *buf, ଅଚଳ ସଂରଚନା xkb_led *led)
{
    copy_to_buf(buf, "\tindicator ");
    write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, led->name));
    copy_to_buf(buf, " {\n");

    ଯଦି (led->which_groups) {
        ଯଦି (led->which_groups != XKB_STATE_LAYOUT_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichGroupState= %s;\n",
                      LedStateMaskText(keymap->ctx, groupComponentMaskNames,
                                       led->which_groups));
        }
        write_buf(buf, "\t\tgroups= 0x%02"PRIx32";\n",
                  led->groups);
    }

    ଯଦି (led->which_mods) {
        ଯଦି (led->which_mods != XKB_STATE_MODS_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichModState= %s;\n",
                      LedStateMaskText(keymap->ctx, modComponentMaskNames,
                                       led->which_mods));
        }
        write_buf(buf, "\t\tmodifiers= %s;\n",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              led->mods.mods));
    }

    ଯଦି (led->ctrls) {
        write_buf(buf, "\t\tcontrols= %s;\n",
                  ControlMaskText(keymap->ctx, format, led->ctrls));
    }

    copy_to_buf(buf, "\t};\n");
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
affect_lock_text(ଗଣନା xkb_action_flags flags, bool show_both)
{
    ବାଛ (flags & (ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK)) {
    ସ୍ଥିତି 0:
        ଫେରନ୍ତୁ show_both ? ",affect=both" : "";
    ସ୍ଥିତି ACTION_LOCK_NO_UNLOCK:
        ଫେରନ୍ତୁ ",affect=lock";
    ସ୍ଥିତି ACTION_LOCK_NO_LOCK:
        ଫେରନ୍ତୁ ",affect=unlock";
    ସ୍ଥିତି ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK:
        ଫେରନ୍ତୁ ",affect=neither";
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଫେରନ୍ତୁ "";
    }
}

#define ପ୍ରତୀକ_PADDING 15
#define ACTION_PADDING 30

ସ୍ଥିର bool
write_action(ଅଚଳ ସଂରଚନା xkb_keymap *keymap, ଗଣନା xkb_keymap_format format,
             xkb_layout_index_t max_groups, ସଂରଚନା buf *buf,
             ଅଚଳ ସଂଘ xkb_action *action,
             ଅଚଳ ଅକ୍ଷର *prefix, ଅଚଳ ଅକ୍ଷର *suffix)
{
    ଅଚଳ ଅକ୍ଷର *type;
    ଅଚଳ ଅକ୍ଷର *args = ଖାଲି;

    ଯଦି (!prefix)
        prefix = "";
    ଯଦି (!suffix)
        suffix = "";

    type = ActionTypeText(action->type);

    ବାଛ (action->type) {
    ସ୍ଥିତି ACTION_TYPE_MOD_SET:
    ସ୍ଥିତି ACTION_TYPE_MOD_LATCH:
    ସ୍ଥିତି ACTION_TYPE_MOD_LOCK:
        ଯଦି (action->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            args = "modMapMods";
        ଅନ୍ୟଥା
            args = ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                               action->mods.mods.mods);
        bool unlockOnPress = (action->mods.flags & ACTION_UNLOCK_ON_PRESS);
        ଯଦି (unlockOnPress && !isModsUnLockOnPressSupported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot use \"%s(unlockOnPress=true)\" in keymap format %d\n",
                    ActionTypeText(action->type), format);
            unlockOnPress = false;
        }
        bool latchOnPress = action->type == ACTION_TYPE_MOD_LATCH &&
                            (action->group.flags & ACTION_LATCH_ON_PRESS);
        ଯଦି (latchOnPress && !isModsLatchOnPressSupported(format)) {
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
        ଭାଙ୍ଗ;

    ସ୍ଥିତି ACTION_TYPE_GROUP_SET:
    ସ୍ଥିତି ACTION_TYPE_GROUP_LATCH:
    ସ୍ଥିତି ACTION_TYPE_GROUP_LOCK:
        ଯଦି ((uint32_t) xkb_abs(action->group.group) <
            (max_groups + !(action->group.flags & ACTION_ABSOLUTE_SWITCH))) {
            bool lockOnRelease = action->type == ACTION_TYPE_GROUP_LOCK &&
                                 (action->group.flags & ACTION_LOCK_ON_RELEASE);
            ଯଦି (lockOnRelease && !isGroupLockOnReleaseSupported(format)) {
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
        } ଅନ୍ୟଥା {
            /* Unsupported group index: degrade to VoidAction() */
            log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported group index %"PRIu32" for action %s()\n",
                      (action->group.group +
                       !!(action->group.flags & ACTION_ABSOLUTE_SWITCH)),
                     type);
            ଯାଅ void_action;
        }
        ଭାଙ୍ଗ;

    ସ୍ଥିତି ACTION_TYPE_TERMINATE:
        write_buf(buf, "%s%s()%s", prefix, type, suffix);
        ଭାଙ୍ଗ;

    ସ୍ଥିତି ACTION_TYPE_PTR_MOVE:
        write_buf(buf, "%s%s(x=%s%d,y=%s%d%s)%s", prefix, type,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_X) && action->ptr.x >= 0) ? "+" : "",
                  action->ptr.x,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_Y) && action->ptr.y >= 0) ? "+" : "",
                  action->ptr.y,
                  (action->ptr.flags & ACTION_REPEAT) ? "" : ",!repeat",
                  suffix);
        ଭାଙ୍ଗ;

    ସ୍ଥିତି ACTION_TYPE_PTR_LOCK:
        args = affect_lock_text(action->btn.flags, true);
        /* fallthrough */
    ସ୍ଥିତି ACTION_TYPE_PTR_BUTTON:
        write_buf(buf, "%s%s(button=", prefix, type);
        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        ଯଦି (action->btn.button > XKB_POINTER_BUTTON_DEFAULT &&
            action->btn.button <= XKB_POINTER_BUTTON_MAX)
            write_buf(buf, "%"PRIu8, action->btn.button);
        ଅନ୍ୟଥା
            copy_to_buf(buf, "default");
        ଯଦି (action->btn.ଗଣନା_2) {
            ପ୍ରତିଜ୍ଞା(action->type != ACTION_TYPE_PTR_LOCK);
            write_buf(buf, ",count=%u", action->btn.ଗଣନା_2);
        }
        ଯଦି (args)
            write_buf(buf, "%s", args);
        write_buf(buf, ")%s", suffix);
        ଭାଙ୍ଗ;

    ସ୍ଥିତି ACTION_TYPE_PTR_DEFAULT:
        write_buf(buf, "%s%s(", prefix, type);
        write_buf(buf, "affect=button,button=%s%"PRId8,
                  (!(action->dflt.flags & ACTION_ABSOLUTE_SWITCH) && action->dflt.ମୂଲ୍ୟ >= 0) ? "+" : "",
                  action->dflt.ମୂଲ୍ୟ);
        write_buf(buf, ")%s", suffix);
        ଭାଙ୍ଗ;

    ସ୍ଥିତି ACTION_TYPE_SWITCH_VT:
        write_buf(buf, "%s%s(screen=%s%d,%ssame)%s", prefix, type,
                  (!(action->screen.flags & ACTION_ABSOLUTE_SWITCH) && action->screen.screen >= 0) ? "+" : "",
                  action->screen.screen,
                  (action->screen.flags & ACTION_SAME_SCREEN) ? "" : "!",
                  suffix);
        ଭାଙ୍ଗ;

    ସ୍ଥିତି ACTION_TYPE_CTRL_SET:
    ସ୍ଥିତି ACTION_TYPE_CTRL_LOCK:
        write_buf(buf, "%s%s(controls=%s%s)%s", prefix, type,
                  ControlMaskText(keymap->ctx, format, action->ctrls.ctrls),
                  (action->type == ACTION_TYPE_CTRL_LOCK
                    ? affect_lock_text(action->ctrls.flags, false)
                    : ""),
                  suffix);
        ଭାଙ୍ଗ;

    ସ୍ଥିତି ACTION_TYPE_REDIRECT_ଚାବି: {
        write_buf(buf, "%s%s(", prefix, type);
        ଅଚଳ ସଂରଚନା xkb_ଚାବି * ଅଚଳ ଚାବି = Xkb_ଚାବି(keymap, action->redirect.keycode);
        /* Can fail if the keycode was not initialized */
        ଯଦି (ଚାବି)
            write_buf(buf, "keycode=%s", ଚାବି_Name_Text(keymap->ctx, ଚାବି->name));
        ଯଦି (action->redirect.affect_mods) {
            xkb_mod_mask_t mask;
            mask = (action->redirect.affect_mods & action->redirect.mods);
            ଯଦି (mask)
                write_buf(buf, ",modifiers=%s",
                          ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, mask));
            mask = (action->redirect.affect_mods & ~action->redirect.mods);
            ଯଦି (mask)
                write_buf(buf, ",clearMods=%s",
                          ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, mask));
        }
        write_buf(buf, ")%s", suffix);
        ଭାଙ୍ଗ;
    }

    ସ୍ଥିତି ACTION_TYPE_UNKNOWN:
    ସ୍ଥିତି ACTION_TYPE_NONE:
        write_buf(buf, "%sNoAction()%s", prefix, suffix);
        ଭାଙ୍ଗ;

    ସ୍ଥିତି ACTION_TYPE_VOID:
void_action:
        /*
         * VoidAction() is an xkbcommon extension.
         * Use LockControls as a backward-compatible fallback.
         * We cannot serialize it to `NoAction()`, as it would be dropped in
         * e.g. the context of multiple actions.
         * We better not use `Private` either, because it could still be
         * interpreted by X11.
         */
        ଯଦି (format == XKB_KEYMAP_FORMAT_TEXT_V1)
            write_buf(buf, "%sLockControls(controls=none,affect=neither)%s",
                      prefix, suffix);
        ଅନ୍ୟଥା
            write_buf(buf, "%sVoidAction()%s", prefix, suffix);
        ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_ଅଙ୍କ_ENTRIES,
                      "Missing action type");
        /* Unsupported legacy actions should have degraded to NoAction */
        ପ୍ରତିଜ୍ଞା(action->type != ACTION_TYPE_UNSUPPORTED_LEGACY);
        write_buf(buf,
                  "%s%s(type=0x%02x,data[0]=0x%02x,data[1]=0x%02x,data[2]=0x%02x,data[3]=0x%02x,data[4]=0x%02x,data[5]=0x%02x,data[6]=0x%02x)%s",
                  prefix, type, action->type, action->priv.data[0],
                  action->priv.data[1], action->priv.data[2],
                  action->priv.data[3], action->priv.data[4],
                  action->priv.data[5], action->priv.data[6],
                  suffix);
        ଭାଙ୍ଗ;
    }

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଅଚଳ ସଂଘ xkb_action void_actions[] = {
    { .type = ACTION_TYPE_VOID }
};

ସ୍ଥିର bool
write_actions(ଅଚଳ ସଂରଚନା xkb_keymap *keymap, ଗଣନା xkb_keymap_format format,
              xkb_layout_index_t max_groups, ସଂରଚନା buf *buf, ସଂରଚନା buf *buf2,
              ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି, xkb_layout_index_t group)
{
    ସ୍ଥିର ଅଚଳ ସଂଘ xkb_action noAction = { .type = ACTION_TYPE_NONE };

    ପାଇଁ (xkb_level_index_t level = 0; level < Xkb_ଚାବି_ଅଙ୍କ_Levels(ଚାବି, group);
         level++) {

        ଯଦି (level != 0)
            copy_to_buf(buf, ", ");

        ଅଚଳ ସଂଘ xkb_action *actions = ଖାଲି;
        xkb_action_ଗଣନା_t ଗଣନା_2 = xkb_keymap_ଚାବି_get_actions_by_level(
            keymap, ଚାବି, group, level, &actions
        );

        ଯଦି (ଗଣନା_2 > 1 && format < XKB_KEYMAP_FORMAT_TEXT_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            actions = void_actions;
            ଗଣନା_2 = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        buf2->size = 0;
        ଯଦି (ଗଣନା_2 == 0) {
            ଯଦି (!write_action(keymap, format, max_groups,
                              buf2, &noAction, ଖାଲି, ଖାଲି))
                ଫେରନ୍ତୁ false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        ଅନ୍ୟଥା ଯଦି (ଗଣନା_2 == 1) {
            ଯଦି (!write_action(keymap, format, max_groups,
                              buf2, &(actions[0]), ଖାଲି, ଖାଲି))
                ଫେରନ୍ତୁ false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        ଅନ୍ୟଥା {
            copy_to_buf(buf2, "{ ");
            ପାଇଁ (xkb_action_ଗଣନା_t k = 0; k < ଗଣନା_2; k++) {
                ଯଦି (k != 0)
                    copy_to_buf(buf2, ", ");
                size_t old_size = buf2->size;
                ଯଦି (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), ଖାଲି, ଖାଲି))
                    ଫେରନ୍ତୁ false;
                /* Check if padding is necessary */
                ଯଦି (buf2->size >= old_size + ACTION_PADDING)
                    ଜାରି;
                /* Compute and write padding, then write the action again */
                ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା padding = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(old_size + ACTION_PADDING - buf2->size);
                buf2->size = old_size;
                write_buf(buf2, "%*s", padding, "");
                ଯଦି (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), ଖାଲି, ଖାଲି))
                    ଫେରନ୍ତୁ false;
            }
            copy_to_buf(buf2, " }");
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
    }

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
write_action_defaults(ଅଚଳ ସଂରଚନା xkb_keymap *keymap,
                      ଗଣନା xkb_keymap_format format,
                      ଅଚଳ ସଂଘ xkb_action *action,
                      ସଂରଚନା buf *buf)
{
    ଅଚଳ ଅକ୍ଷର *type = ActionTypeText(action->type);

#define PREFIX "\t"

    ବାଛ (action->type) {
    ସ୍ଥିତି ACTION_TYPE_MOD_SET:
    ସ୍ଥିତି ACTION_TYPE_MOD_LATCH:
    ସ୍ଥିତି ACTION_TYPE_MOD_LOCK:
        ପ୍ରତିଜ୍ଞା(action->mods.flags == 0);
        ପ୍ରତିଜ୍ଞା(action->mods.mods.mods == 0);
        write_buf(buf, PREFIX"%s.modifiers = 0;\n", type);
        ଯଦି (action->type != ACTION_TYPE_MOD_LOCK)
            write_buf(buf, PREFIX"%s.clearLocks = false;\n", type);
        ଯଦି (action->type == ACTION_TYPE_MOD_LATCH &&
            isModsLatchOnPressSupported(format))
            write_buf(buf, PREFIX"%s.latchOnPress = false;\n", type);
        ଯଦି (action->type == ACTION_TYPE_MOD_LATCH)
            write_buf(buf, PREFIX"%s.latchToLock = false;\n", type);
        ଯଦି (action->type == ACTION_TYPE_MOD_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        ଯଦି (isModsUnLockOnPressSupported(format))
            write_buf(buf, PREFIX"%s.unlockOnPress = false;\n", type);
        ଭାଙ୍ଗ;

    ସ୍ଥିତି ACTION_TYPE_GROUP_SET:
    ସ୍ଥିତି ACTION_TYPE_GROUP_LATCH:
    ସ୍ଥିତି ACTION_TYPE_GROUP_LOCK:
        ପ୍ରତିଜ୍ଞା(action->group.flags == 0);
        ପ୍ରତିଜ୍ଞା(action->group.group == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.group = +0;\n", type);
        ଯଦି (action->type != ACTION_TYPE_GROUP_LOCK)
            write_buf(buf, PREFIX"%s.clearLocks = false;\n", type);
        ଯଦି (action->type == ACTION_TYPE_GROUP_LATCH)
            write_buf(buf, PREFIX"%s.latchToLock = false;\n", type);
        ଯଦି (action->type == ACTION_TYPE_GROUP_LOCK &&
            isGroupLockOnReleaseSupported(format))
            write_buf(buf, PREFIX"%s.lockOnRelease = false;\n", type);
        ଭାଙ୍ଗ;


    ସ୍ଥିତି ACTION_TYPE_PTR_MOVE:
        ପ୍ରତିଜ୍ଞା(action->ptr.x == 0);
        ପ୍ରତିଜ୍ଞା(action->ptr.y == 0);
        ପ୍ରତିଜ୍ଞା(action->ptr.flags == ACTION_REPEAT);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.x = +0;\n", type);
        write_buf(buf, PREFIX"%s.y = +0;\n", type);
        write_buf(buf, PREFIX"%s.accel = true;\n", type);
        ଭାଙ୍ଗ;

    ସ୍ଥିତି ACTION_TYPE_PTR_LOCK:
    ସ୍ଥିତି ACTION_TYPE_PTR_BUTTON:
        ପ୍ରତିଜ୍ଞା(action->btn.flags == 0);
        ପ୍ରତିଜ୍ଞା(action->btn.button == 0);
        ପ୍ରତିଜ୍ଞା(action->btn.ଗଣନା_2 == 0);
        write_buf(buf, PREFIX"%s.button = 0;\n", type);
        write_buf(buf, PREFIX"%s.count = 0;\n", type);
        ଯଦି (action->type == ACTION_TYPE_PTR_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        ଭାଙ୍ଗ;

    ସ୍ଥିତି ACTION_TYPE_PTR_DEFAULT:
        ପ୍ରତିଜ୍ଞା(action->dflt.flags == 0);
        ପ୍ରତିଜ୍ଞା(action->dflt.ମୂଲ୍ୟ == 1);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.button = +1;\n", type);
        ଭାଙ୍ଗ;

    ସ୍ଥିତି ACTION_TYPE_SWITCH_VT:
        ପ୍ରତିଜ୍ଞା(action->screen.flags == ACTION_SAME_SCREEN);
        ପ୍ରତିଜ୍ଞା(action->screen.screen == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.screen = +0;\n", type);
        write_buf(buf, PREFIX"%s.same = true;\n", type);
        ଭାଙ୍ଗ;

    ସ୍ଥିତି ACTION_TYPE_CTRL_SET:
    ସ୍ଥିତି ACTION_TYPE_CTRL_LOCK:
        ପ୍ରତିଜ୍ଞା(action->ctrls.flags == 0);
        ପ୍ରତିଜ୍ଞା(action->ctrls.ctrls == 0);
        write_buf(buf, PREFIX"%s.controls = 0;\n", type);
        ଯଦି (action->type == ACTION_TYPE_CTRL_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        ଭାଙ୍ଗ;

    ସ୍ଥିତି ACTION_TYPE_REDIRECT_ଚାବି:
        ପ୍ରତିଜ୍ଞା(action->redirect.keycode == keymap->redirect_ଚାବି_auto);
        ପ୍ରତିଜ୍ଞା(action->redirect.affect_mods == 0);
        ପ୍ରତିଜ୍ଞା(action->redirect.mods == 0);
        write_buf(buf, PREFIX"%s.keycode = auto;\n", type);
        write_buf(buf, PREFIX"%s.modifiers = 0;\n", type);
        write_buf(buf, PREFIX"%s.clearMods = 0;\n", type);
        ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_ଅଙ୍କ_ENTRIES,
                      "Missing action type");
    }

#undef PREFIX

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
write_actions_defaults(ଅଚଳ ସଂରଚନା xkb_keymap *keymap,
                       ଗଣନା xkb_keymap_format format, ସଂରଚନା buf *buf)
{
    ActionsInfo defaults = {0};
    InitActionsInfo(keymap, &defaults);

    ପାଇଁ (uint8_t a = 0; a < (uint8_t) ACTION_TYPE_ଅଙ୍କ_ENTRIES; a++) {
        ଅଚଳ ସଂଘ xkb_action * restrict ଅଚଳ action = &defaults.actions[a];
        ଯଦି (!write_action_defaults(keymap, format, action, buf))
            ଫେରନ୍ତୁ false;
    }
    copy_to_buf(buf, "\n");

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଅଚଳ ସଂରଚନା xkb_sym_interpret fallback_interpret = {
    .sym = XKB_ଚାବି_Void_ପ୍ରତୀକ,
    .required = true,
    .repeat = FALLBACK_INTERPRET_ଚାବି_REPEAT,
    .match = MATCH_ANY_OR_NONE,
    .mods = 0,
    .virtual_mod = (xkb_mod_index_t) DEFAULT_INTERPRET_VMOD,
    .ଅଙ୍କ_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

ସ୍ଥିର bool
write_compat(ଅଚଳ ସଂରଚନା xkb_keymap * restrict keymap,
             ଗଣନା xkb_keymap_format format, xkb_layout_index_t max_groups,
             ଗଣନା xkb_keymap_serialize_flags flags, bool * restrict some_interp,
             ସଂରଚନା buf *buf)
{
    ଅଚଳ bool pretty = (flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ଅଚଳ bool defaults = (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_ମୂଲ୍ୟଗୁଡ଼ିକ);
    ଅଚଳ bool drop_unused = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    ଅଚଳ bool drop_interprets =
        ((flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_ଚାବି_ମୂଲ୍ୟଗୁଡ଼ିକ) && drop_unused);
    /* TODO: print all default values if the explicit flag is set */

    ଯଦି (keymap->compat_section_name)
        write_buf(buf, "xkb_compatibility \"%s\" {\n",
                  keymap->compat_section_name);
    ଅନ୍ୟଥା
        copy_to_buf(buf, "xkb_compatibility {\n");

    ଯଦି (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        ଫେରନ୍ତୁ false;

    ଯଦି (defaults && !write_actions_defaults(keymap, format, buf))
        ଫେରନ୍ତୁ false;

    copy_to_buf(buf, "\tinterpret.useModMapMods= AnyLevel;\n");
    copy_to_buf(buf, "\tinterpret.repeat= False;\n");

    /* xkbcomp requires at least one interpret entry. */
    ଅଚଳ bool use_fallback_interpret =
        (!keymap->ଅଙ୍କ_sym_interprets || drop_interprets);
    ଅଚଳ darray_size_t ଅଙ୍କ_sym_interprets =
        (use_fallback_interpret ? 1 : keymap->ଅଙ୍କ_sym_interprets);
    ଅଚଳ ସଂରଚନା xkb_sym_interpret* ଅଚଳ sym_interprets =
        (use_fallback_interpret ? &fallback_interpret : keymap->sym_interprets);

    ପାଇଁ (darray_size_t i = 0; i < ଅଙ୍କ_sym_interprets; i++) {
        ଅଚଳ ସଂରଚନା xkb_sym_interpret *si = &sym_interprets[i];
        ଯଦି (!si->required && drop_unused)
            ଜାରି;

        *some_interp = true;

        copy_to_buf(buf, "\tinterpret ");
        ଯଦି (!si->sym) {
            copy_to_buf(buf, "Any");
        } ଅନ୍ୟଥା ଯଦି (pretty) {
            write_buf(buf, "%s", KeysymText(keymap->ctx, si->sym));
        } ଅନ୍ୟଥା {
            write_buf(buf, "0x%"PRIx32, si->sym);
        }
        write_buf(buf, "+%s(%s) {",
                  SIMatchText(si->match),
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, si->mods));

        bool has_explicit_properties = false;

        ଯଦି (si->virtual_mod != XKB_MOD_INVALID) {
            write_buf(buf, "\n\t\tvirtualModifier= %s;",
                      ModIndexText(keymap->ctx, &keymap->mods,
                                   si->virtual_mod));
            has_explicit_properties = true;
        }

        ଯଦି (si->level_one_only) {
            copy_to_buf(buf, "\n\t\tuseModMapMods=level1;");
            has_explicit_properties = true;
        } ଅନ୍ୟଥା ଯଦି (defaults) {
            copy_to_buf(buf, "\n\t\tuseModMapMods=AnyLevel;");
            has_explicit_properties = true;
        }

        ଯଦି (si->repeat) {
            copy_to_buf(buf, "\n\t\trepeat= True;");
            has_explicit_properties = true;
        } ଅନ୍ୟଥା ଯଦି (defaults) {
            copy_to_buf(buf, "\n\t\trepeat= False;");
            has_explicit_properties = true;
        }

        ଅଚଳ ସଂଘ xkb_action *action = ଖାଲି;
        xkb_action_ଗଣନା_t action_ଗଣନା = si->ଅଙ୍କ_actions;
        ଯଦି (action_ଗଣନା <= 1) {
            action = &si->a.action;
        } ଅନ୍ୟଥା ଯଦି (format < XKB_KEYMAP_FORMAT_TEXT_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            action = &void_actions[0];
            action_ଗଣନା = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        ଯଦି (action_ଗଣନା > 1) {
            copy_to_buf(buf, "\n\t\taction= {");
            ଅଚଳ ଅକ୍ଷର suffix[] = ", ";
            ପାଇଁ (xkb_action_ଗଣନା_t k = 0; k < si->ଅଙ୍କ_actions; k++) {
                ଯଦି (!write_action(keymap, format, max_groups,
                                  buf, &si->a.actions[k], "", suffix))
                    ଫେରନ୍ତୁ false;
            }
            buf->size -= ଆକାର(suffix) - 1; /* trailing comma */
            copy_to_buf(buf, "};");
            has_explicit_properties = true;
        } ଅନ୍ୟଥା ଯଦି (action_ଗଣନା == 1) {
            ଯଦି (!write_action(keymap, format, max_groups, buf, action,
                              "\n\t\taction= ", ";"))
                ଫେରନ୍ତୁ false;
            has_explicit_properties = true;
        }
        write_buf(buf, (has_explicit_properties
                        ? "\n\t};\n"
                        /* Empty interpret is a syntax error in xkbcomp, so
                         * use a dummy entry */
                        : "\n\t\taction= NoAction();\n\t};\n"));
    }

    ଯଦି (use_fallback_interpret || (
        /* Detect previous interpret fallback */
        ଅଙ୍କ_sym_interprets == 1 && *some_interp &&
        sym_interprets[0].sym == fallback_interpret.sym &&
        sym_interprets[0].match == fallback_interpret.match &&
        sym_interprets[0].mods == fallback_interpret.mods &&
        sym_interprets[0].repeat == fallback_interpret.repeat &&
        sym_interprets[0].virtual_mod == fallback_interpret.virtual_mod &&
        sym_interprets[0].level_one_only == fallback_interpret.level_one_only &&
        sym_interprets[0].ଅଙ୍କ_actions == fallback_interpret.ଅଙ୍କ_actions
        /* `required` field is not compared */
    ))
        *some_interp = false;

    ଅଚଳ ସଂରଚନା xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        ଯଦି ((led->which_groups || led->groups ||
             led->which_mods || led->mods.mods || led->ctrls) &&
            !write_led_map(keymap, format, defaults, buf, led))
        {
            ଫେରନ୍ତୁ false;
        }
    }

    copy_to_buf(buf, "};\n\n");

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
write_keysyms(ଅଚଳ ସଂରଚନା xkb_keymap *keymap,
              ସଂରଚନା buf *buf, ସଂରଚନା buf *buf2,
              ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି, xkb_layout_index_t group,
              bool pretty, bool show_actions)
{
    ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା padding = (pretty)
        ? ((show_actions) ? ACTION_PADDING : ପ୍ରତୀକ_PADDING)
        : 0;
    ପାଇଁ (xkb_level_index_t level = 0; level < Xkb_ଚାବି_ଅଙ୍କ_Levels(ଚାବି, group);
         level++) {
        ଅଚଳ xkb_keysym_t *syms;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_syms;

        ଯଦି (level != 0)
            copy_to_buf(buf, ", ");

        ଅଙ୍କ_syms = xkb_keymap_ଚାବି_get_syms_by_level(
            /* remove `const` constraint to use the dated public API */
            (ସଂରଚନା xkb_keymap *)keymap,
            ଚାବି->keycode, group, level, &syms
        );

        ଅଚଳ xkb_keysym_t no_ପ୍ରତୀକ = XKB_ଚାବି_No_ପ୍ରତୀକ;
        ଯଦି (ଅଙ୍କ_syms == 0) {
            ଅଙ୍କ_syms = 1;
            syms = &no_ପ୍ରତୀକ;
        }

        /*
         * NOTE: Use `NoSymbol` even without pretty output, for compatibility
         * with xkbcomp and xkbcommon < 1.12
         */

        ଯଦି (ଅଙ୍କ_syms == 1) {
            ଯଦି (pretty || syms[0] == XKB_ଚାବି_No_ପ୍ରତୀକ)
                write_buf(buf, "%*s", padding, KeysymText(keymap->ctx, syms[0]));
            ଅନ୍ୟଥା
                write_buf(buf, "0x%"PRIx32, syms[0]);
        } ଅନ୍ୟଥା {
            ଯଦି (pretty) {
                buf2->size = 0;
                copy_to_buf(buf2, "{ ");
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0; s < ଅଙ୍କ_syms; s++) {
                    ଯଦି (s != 0)
                        copy_to_buf(buf2, ", ");
                    write_buf(buf2, "%*s", (show_actions ? padding : 0),
                              KeysymText(keymap->ctx, syms[s]));
                }
                copy_to_buf(buf2, " }");
                write_buf(buf, "%*s", padding, buf2->buf);
            } ଅନ୍ୟଥା {
                copy_to_buf(buf, "{");
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0; s < ଅଙ୍କ_syms; s++) {
                    ଯଦି (s != 0)
                        copy_to_buf(buf, ",");
                    ଯଦି (syms[s] == XKB_ଚାବି_No_ପ୍ରତୀକ)
                        copy_to_buf(buf, "NoSymbol");
                    ଅନ୍ୟଥା
                        write_buf(buf, "0x%"PRIx32, syms[s]);
                }
                copy_to_buf(buf, "}");
            }
        }
    }

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
write_ଚାବି(ଅଚଳ ସଂରଚନା xkb_keymap *keymap,
          ଅଚଳ ସଂରଚନା xkb_keymap_serialize_config *config,
          ଅଚଳ ଚାବି_name_substitutions *substitutions,
          xkb_layout_index_t max_groups, bool some_interprets,
          bool drop_interprets, bool explicit, bool pretty,
          ସଂରଚନା buf *buf, ସଂରଚନା buf *buf2, ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି)
{
    bool simple = true;

    xkb_layout_mask_t layouts = config->layouts;
    ଯଦି (popcount32(layouts) != keymap->ଅଙ୍କ_groups) {
        /* Subset of layouts: use out-of-range key policy */
        xkb_layout_index_t l = 0;
        ପାଇଁ (xkb_layout_mask_t ls = layouts; ls; ls >>= 1, l++) {
            ଯଦି (ls & 0x1) {
                /* Replace target layout with its effective layout, if valid */
                layouts &= ~(UINT32_C(1) << l);
                ଅଚଳ xkb_layout_index_t effective =
                    xkb_keymap_ଚାବି_effective_layout(ଚାବି, l);
                ଯଦି (effective != XKB_LAYOUT_INVALID)
                    layouts |= (UINT32_C(1) << effective);
            }
        }
    } ଅନ୍ୟଥା {
        layouts &= ((UINT64_C(1) << ଚାବି->ଅଙ୍କ_groups) - 1);
    }
    ଅଚଳ xkb_layout_index_t groups_ଗଣନା = popcount32(layouts);

    ଅଚଳ xkb_atom_t name = (substitutions == ଖାଲି)
        ? ଚାବି->name
        : substitute_name(substitutions, ଚାବି->name);

    ଯଦି (pretty)
        write_buf(buf, "\tkey %-20s {", ଚାବି_Name_Text(keymap->ctx, name));
    ଅନ୍ୟଥା
        write_buf(buf, "\tkey %s {", ଚାବି_Name_Text(keymap->ctx, name));

    ଯଦି ((ଚାବି->explicit & EXPLICIT_TYPES) || explicit) {
        simple = false;

        bool multi_type = explicit;
        /* Check for distinct key types only if not requiring explicit values */
        ଯଦି (!explicit) {
            ପାଇଁ (xkb_layout_index_t group = 1; group < ଚାବି->ଅଙ୍କ_groups; group++) {
                ଯଦି (!((UINT32_C(1) << group) & layouts))
                    ଜାରି;

                ଯଦି (ଚାବି->groups[group].type != ଚାବି->groups[0].type) {
                    multi_type = true;
                    ଭାଙ୍ଗ;
                }
            }
        }

        ଯଦି (multi_type) {
            xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
            ପାଇଁ (xkb_layout_index_t group = 0; group < ଚାବି->ଅଙ୍କ_groups; group++) {
                ଯଦି (!((UINT32_C(1) << group) & layouts))
                    ଜାରି;

                new_group++;

                ଯଦି (!ଚାବି->groups[group].explicit_type && !explicit)
                    ଜାରି;

                ଅଚଳ ସଂରଚନା xkb_ଚାବି_type * ଅଚଳ type = ଚାବି->groups[group].type;
                write_buf(buf, "\n\t\ttype[%"PRIu32"]= ", new_group + 1);
                write_buf_string_literal(
                  buf, xkb_atom_text(keymap->ctx, type->name));
                copy_to_buf(buf, ",");
            }
        }
        ଅନ୍ୟଥା {
            ଅଚଳ ସଂରଚନା xkb_ଚାବି_type * ଅଚଳ type = ଚାବି->groups[0].type;
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
    ଅଚଳ bool explicit_actions = (ଚାବି->explicit & EXPLICIT_INTERP);

    /* Check if an explicit value is required */
    #define require_explicit(ଚାବି, prop,                                       \
                             ମୂଲ୍ୟ, initial_ମୂଲ୍ୟ, default_interpret_ମୂଲ୍ୟ) ( \
        /* Explicit value in the source */                                    \
        ((ଚାବି)->explicit & (prop)) ||                                         \
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
                ((ଚାବି)->implicit_actions && !some_interprets)                 \
            ) && (                                                            \
                /* Non-initial value */                                       \
                (ମୂଲ୍ୟ) != (initial_ମୂଲ୍ୟ) ||                                 \
                /* Force initial value for clarity */                         \
                (initial_ମୂଲ୍ୟ) != (default_interpret_ମୂଲ୍ୟ)                  \
            )                                                                 \
        )                                                                     \
    )

    ଯଦି (require_explicit(ଚାବି, EXPLICIT_REPEAT, ଚାବି->repeats,
                         DEFAULT_ଚାବି_REPEAT, DEFAULT_INTERPRET_ଚାବି_REPEAT)) {
        ଯଦି (ଚାବି->repeats)
            copy_to_buf(buf, "\n\t\trepeat= Yes,");
        ଅନ୍ୟଥା
            copy_to_buf(buf, "\n\t\trepeat= No,");
        simple = false;
    }

    ଯଦି (require_explicit(ଚାବି, EXPLICIT_VMODMAP, ଚାବି->vmodmap,
                         (xkb_mod_mask_t) DEFAULT_ଚାବି_VMODMAP,
                         (xkb_mod_mask_t) DEFAULT_INTERPRET_VMODMAP)) {
        write_buf(buf, "\n\t\tvirtualMods= %s,",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              ଚାବି->vmodmap));
        simple = false;
    }

    #undef require_explicit

    ବାଛ (ଚାବି->out_of_range_group_policy) {
    ସ୍ଥିତି XKB_LAYOUT_OUT_OF_RANGE_CLAMP:
        copy_to_buf(buf, "\n\t\tgroupsClamp,");
        simple = false;
        ଭାଙ୍ଗ;

    ସ୍ଥିତି XKB_LAYOUT_OUT_OF_RANGE_REDIRECT: {
        ଯଦି (!groups_ଗଣନା) {
            /* No layout to redirect to: skip */
            ଭାଙ୍ଗ;
        }

        xkb_layout_mask_t layout_mask =
            (UINT32_C(1) << ଚାବି->out_of_range_group_ସଂଖ୍ୟା);
        ଯଦି (!(layouts & layout_mask)) {
            /*
             * Layout is not included: default to first layout
             * See: XkbWrapGroupIntoRange
             */
            log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "key %s: groupsRedirect=%"PRIu32" converted to "
                     "groupsRedirect=1\n",
                     ଚାବି_Name_Text(keymap->ctx, name),
                     ଚାବି->out_of_range_group_ସଂଖ୍ୟା + 1);
            layout_mask = 0x1;
        }

        /* In case some lower layouts were discarded, update the group index */
        ଅଚଳ xkb_layout_mask_t ତଳ = (layouts & (layout_mask - 1));
        ଅଚଳ xkb_layout_index_t new = popcount32(ତଳ);
        write_buf(buf, "\n\t\tgroupsRedirect= %"PRIu32",", new + 1);
        simple = false;
        ଭାଙ୍ଗ;
    }

    ସ୍ଥିତି XKB_LAYOUT_OUT_OF_RANGE_WRAP:
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଭାଙ୍ଗ;
    }

    ଯଦି (ଚାବି->overlays) {
        xkb_overlay_mask_t remaining = ଚାବି->overlays;

        ଅଚଳ xkb_overlay_index_t overlay_max =
            format_max_overlays(config->format);
        static_assert(XKB_OVERLAY_ଗଣନା == 8, "invalid right shift");
        ଅଚଳ xkb_overlay_mask_t valid =
            (xkb_overlay_mask_t)((1u << overlay_max) - 1u);
        remaining &= valid;
        ଯଦି (remaining != ଚାବି->overlays) {
            log_warn(keymap->ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                     "Overlays indices > %u in %s require using "
                     "keymap format >= v2; "
                     "keep overlays 0x%08x and discard overlays 0x%08x\n",
                     overlay_max, ଚାବି_Name_Text(keymap->ctx, name),
                     remaining, ଚାବି->overlays & ~valid);
        }

        ଯଦି (remaining && !ଚାବି->overlays_inline &&
            !areOverlappingOverlaysSupported(config->format)) {
            /* isolate lowest set bit */
            ଅଚଳ xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                remaining &
                (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
            );
            /* get its index */
            ଅଚଳ xkb_overlay_index_t overlay =
                (xkb_overlay_index_t)popcount32(lsb - 1u);
            log_warn(keymap->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                     "Overlapping overlays in %s require using "
                     "keymap format >= v2; "
                     "keep overlay %u and discard overlays 0x%x\n",
                     ଚାବି_Name_Text(keymap->ctx, name), overlay, remaining & ~lsb);
            remaining = lsb;
        }

        ଯଦି (remaining)
            simple = false;

        ଅଚଳ ସଂରଚନା xkb_ଚାବି * ଅଚଳ *overlays_ଚାବିଗୁଡ଼ିକ = (ଚାବି->overlays_inline)
                                                    ? &ଚାବି->overlay_ଚାବି
                                                    : ଚାବି->overlays_ଚାବିଗୁଡ଼ିକ;
        ଯେତେବେଳେ (remaining) {
            /* isolate lowest set bit */
            ଅଚଳ xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                remaining &
                (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
            );
            /* get its index */
            ଅଚଳ xkb_overlay_index_t overlay =
                (xkb_overlay_index_t)popcount32(lsb - 1u);
            remaining = (remaining & ~lsb);

            /* pop current value */
            ଯଦି (ଚାବି->overlays_inline && remaining)
                PANIC_UNREACHABLE();
            ଅଚଳ ସଂରଚନା xkb_ଚାବି *overlay_ଚାବି = *(overlays_ଚାବିଗୁଡ଼ିକ++);

            ଅଚଳ xkb_atom_t overlay_ଚାବି_name = (substitutions == ଖାଲି)
                ? overlay_ଚାବି->name
                : substitute_name(substitutions, overlay_ଚାବି->name);
            write_buf(buf, "\n\t\toverlay%u= %s,",
                      overlay + 1, ଚାବି_Name_Text(keymap->ctx, overlay_ଚାବି_name));
        }
    }

    ଯଦି (groups_ଗଣନା > 1 || explicit_actions || explicit)
        simple = false;

    ଯଦି (simple) {
        ଯଦି (groups_ଗଣନା == 0) {
            /* Remove trailing comma */
            delete_last_char(buf, ',');
            ଯଦି (buf->buf[buf->size - 1] == ',')
                buf->size--;
        } ଅନ୍ୟଥା {
            copy_to_buf(buf, "\t[ ");
            /* Get the single group index */
            xkb_mod_index_t group = ctz32(layouts);
            /* The group may not be defined for this key if it is not the first one */
            group = xkb_keymap_ଚାବି_effective_layout(ଚାବି, group);
            ଯଦି (!write_keysyms(keymap, buf, buf2, ଚାବି, group, pretty, false))
                ଫେରନ୍ତୁ false;
            copy_to_buf(buf, " ]");
        }
        copy_to_buf(buf, " };\n");
    }
    ଅନ୍ୟଥା {
        xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
        ପାଇଁ (xkb_layout_index_t group = 0; group < ଚାବି->ଅଙ୍କ_groups; group++) {
            ଯଦି (!((UINT32_C(1) << group) & layouts))
                ଜାରି;

            new_group++;

            ଅଚଳ bool print_actions =
                /* Group has explicit actions */
                ଚାବି->groups[group].explicit_actions ||
                /* Explicit values are required */
                explicit ||
                /* Group has symbols but no explicit actions and key has explicit
                 * actions in another group: ensure compatibility with xkbcomp
                 * and all xkbcommon versions */
                (ଚାବି->groups[group].explicit_symbols &&
                 explicit_actions && some_interprets) ||
                /* Group has implicit actions but no interpret can set them, so
                 * the actions must be made explicit */
                (ଚାବି->groups[group].implicit_actions && !some_interprets);

            ଯଦି (new_group != 0)
                copy_to_buf(buf, ",");
            write_buf(buf, "\n\t\tsymbols[%"PRIu32"]= [ ", new_group + 1);

            ଯଦି (!write_keysyms(keymap, buf, buf2, ଚାବି, group,
                               pretty, print_actions))
                ଫେରନ୍ତୁ false;
            copy_to_buf(buf, " ]");
            ଯଦି (print_actions) {
                write_buf(buf, ",\n\t\tactions[%"PRIu32"]= [ ", new_group + 1);
                ଯଦି (!write_actions(keymap, config->format, max_groups,
                                   buf, buf2, ଚାବି, group))
                    ଫେରନ୍ତୁ false;
                copy_to_buf(buf, " ]");
            }
        }
        ଯଦି (!groups_ଗଣନା)
            delete_last_char(buf, ',');
        copy_to_buf(buf, "\n\t};\n");
    }

    ଫେରନ୍ତୁ true;
}


ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
cmp_modmap_step_1(ଅଚଳ ଶୂନ୍ୟ *a, ଅଚଳ ଶୂନ୍ୟ *b)
{
    ଅଚଳ ModMapEntry *m1 = (ଅଚଳ ModMapEntry *)a;
    ଅଚଳ ModMapEntry *m2 = (ଅଚଳ ModMapEntry *)b;

    ଯଦି (!m1->mods) {
        ଯଦି (!m2->mods) {
            /* Entry will be dropped: do not care about the other fields */
            ଫେରନ୍ତୁ 0;
        }
        ଫେରନ୍ତୁ +1;
    } ଅନ୍ୟଥା ଯଦି (!m2->mods) {
        ଫେରନ୍ତୁ -1;
    }

    /* Valid only on step 1 */
    ଯଦି (m1->ଚାବି_Code < m2->ଚାବି_Code)
        ଫେରନ୍ତୁ -1;
    ଅନ୍ୟଥା ଯଦି (m1->ଚାବି_Code > m2->ଚାବି_Code)
        ଫେରନ୍ତୁ +1;

    ଯଦି (m1->mods < m2->mods)
        ଫେରନ୍ତୁ -1;
    ଅନ୍ୟଥା ଯଦି (m1->mods > m2->mods)
        ଫେରନ୍ତୁ +1;

    ଯଦି (m1->have_ପ୍ରତୀକ < m2->have_ପ୍ରତୀକ)
        ଫେରନ୍ତୁ +1;
    ଅନ୍ୟଥା ଯଦି (m1->have_ପ୍ରତୀକ > m2->have_ପ୍ରତୀକ)
        ଫେରନ୍ତୁ -1;

    ଯଦି (m1->have_ପ୍ରତୀକ) {
        ଯଦି (m1->u.ଚାବି_Sym < m2->u.ଚାବି_Sym)
            ଫେରନ୍ତୁ -1;
        ଯଦି (m1->u.ଚାବି_Sym > m2->u.ଚାବି_Sym)
            ଫେରନ୍ତୁ +1;
    } ଅନ୍ୟଥା {
        ଯଦି (m1->u.ଚାବି_Name < m2->u.ଚାବି_Name)
            ଫେରନ୍ତୁ -1;
        ଯଦି (m1->u.ଚାବି_Name > m2->u.ଚାବି_Name)
            ଫେରନ୍ତୁ +1;
    }

    ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
cmp_modmap_step_2(ଅଚଳ ଶୂନ୍ୟ *a, ଅଚଳ ଶୂନ୍ୟ *b)
{
    ଅଚଳ ModMapEntry *m1 = (ଅଚଳ ModMapEntry *)a;
    ଅଚଳ ModMapEntry *m2 = (ଅଚଳ ModMapEntry *)b;

    ଯଦି (m1->mods < m2->mods)
        ଫେରନ୍ତୁ -1;
    ଯଦି (m1->mods > m2->mods)
        ଫେରନ୍ତୁ +1;

    ଯଦି (m1->have_ପ୍ରତୀକ < m2->have_ପ୍ରତୀକ)
        ଫେରନ୍ତୁ -1;
    ଯଦି (m1->have_ପ୍ରତୀକ > m2->have_ପ୍ରତୀକ)
        ଫେରନ୍ତୁ +1;

    ଯଦି (m1->have_ପ୍ରତୀକ) {
        ଯଦି (m1->u.ଚାବି_Sym < m2->u.ଚାବି_Sym)
            ଫେରନ୍ତୁ -1;
        ଯଦି (m1->u.ଚାବି_Sym > m2->u.ଚାବି_Sym)
            ଫେରନ୍ତୁ +1;
    } ଅନ୍ୟଥା {
        ଯଦି (m1->ଚାବି_Code < m2->ଚାବି_Code)
            ଫେରନ୍ତୁ -1;
        ଯଦି (m1->ଚାବି_Code > m2->ଚାବି_Code)
            ଫେରନ୍ତୁ +1;
    }

    ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର bool
write_modmaps(ଅଚଳ ସଂରଚନା xkb_keymap *keymap, ଗଣନା xkb_keymap_format format,
              ଅଚଳ ଚାବି_name_substitutions *substitutions,
              bool pretty, bool drop_interprets, ସଂରଚନା buf *buf)
{
    ଯଦି (!keymap->ଅଙ୍କ_modmaps)
        ଫେରନ୍ତୁ true;

    darray(ModMapEntry) modmaps = darray_new();
    darray_from_ବସ୍ତୁଗୁଡ଼ିକ(modmaps, keymap->modmaps, keymap->ଅଙ୍କ_modmaps);
    ଯଦି (!darray_ବସ୍ତୁଗୁଡ଼ିକ(modmaps))
        ଫେରନ୍ତୁ false;

    /* Sort entries by key code */
    qsort(darray_ବସ୍ତୁଗୁଡ଼ିକ(modmaps), darray_size(modmaps),
          ଆକାର(*darray_ବସ୍ତୁଗୁଡ଼ିକ(modmaps)), &cmp_modmap_step_1);

    /* Drop disabled entries at the end */
    ପାଇଁ (darray_size_t m = darray_size(modmaps); m-- > 0;) {
        ଯଦି (darray_item(modmaps, m).mods)
            ଭାଙ୍ଗ;
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
    ପାଇଁ (darray_size_t m = darray_size(modmaps); m-- > 0;) {
        ModMapEntry *mm = &darray_item(modmaps, m);
        ପ୍ରତିଜ୍ଞା(mm->mods);

        ଯଦି (mm->ଚାବି_Code == current_keycode) {
            /*
             * Keycode already processed: either drop next key code entries
             * or convert them to keymap format v1
             */
            ଅଚଳ xkb_mod_mask_t core_mods = (mm->mods & MOD_REAL_MASK_ALL);
            ଯଦି (convert_to_v1_compatibility_entry && core_mods) {
                /*
                 * Convert to keymap format V1-specific entry:
                 * keep only the highest core modifier
                 */
                mm->mods = UINT32_C(1) << (XKB_MAX_MODS - 1 - clz32(core_mods));
            } ଅନ୍ୟଥା {
                /*
                 * Drop if there is a previous entry for all keymap formats or
                 * if the current entry is incompatible with V1 keymap format.
                 */
                mm->mods = 0;
            }
            ଜାରି;
        }

        current_keycode = mm->ଚାବି_Code;
        ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି = Xkb_ଚାବି(keymap, mm->ଚାବି_Code);

        ଅଚଳ xkb_mod_mask_t core_mods = (ଚାବି->modmap & MOD_REAL_MASK_ALL);
        ଯଦି (format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
            ((ଚାବି->modmap & (ଚାବି->modmap - 1)) || ଚାବି->modmap != core_mods)) {
            /* V1 entry with multiple modifiers or non X11 core modifiers */
            m++; /* process in next iteration */
            convert_to_v1_compatibility_entry = true;
            log_err(keymap->ctx, XKB_ERROR_INVALID_MODIFIER_MAP_MASK,
                    "Cannot serialize modifier map of key %s: modifiers mask "
                    "0x%"PRIx32" incompatible with keymap format v1\n",
                    ଚାବି_Name_Text(keymap->ctx, ଚାବି->name), ଚାବି->modmap);
        } ଅନ୍ୟଥା {
            /* V2 format or V1 format with a single core modifier */
            ପ୍ରତିଜ୍ଞା(ଚାବି->modmap);
            mm->mods = ଚାବି->modmap;
            /* Force key name */
            mm->have_ପ୍ରତୀକ = false;
            mm->u.ଚାବି_Name = ଚାବି->name;
            convert_to_v1_compatibility_entry = false;
        }
    }

    ଯଦି (darray_empty(modmaps))
        ଯାଅ exit;

    /* Sort entries by modifier mask */
    qsort(darray_ବସ୍ତୁଗୁଡ଼ିକ(modmaps), darray_size(modmaps),
          ଆକାର(*darray_ବସ୍ତୁଗୁଡ଼ିକ(modmaps)), &cmp_modmap_step_2);

    /*
     * Serialize modmaps
     */

    xkb_mod_mask_t current_mods = 0;
    ଅଚଳ ModMapEntry *entry;
    darray_foreach(entry, modmaps) {
        ଯଦି (!entry->mods)
            ଜାରି;

        ଯଦି (current_mods != entry->mods) {
            ଯଦି (current_mods) {
                copy_to_buf(buf, " };\n");
            }
            current_mods = entry->mods;
            write_buf(buf, "\tmodifier_map %s { ",
                      ModMaskText(keymap->ctx, MOD_REAL, &keymap->mods,
                                  current_mods));
        } ଅନ୍ୟଥା {
            copy_to_buf(buf, ", ");
        }

        ଯଦି (entry->have_ପ୍ରତୀକ) {
            ଯଦି (pretty || entry->u.ଚାବି_Sym == XKB_ଚାବି_No_ପ୍ରତୀକ)
                write_buf(buf, "%s", KeysymText(keymap->ctx, entry->u.ଚାବି_Sym));
            ଅନ୍ୟଥା
                write_buf(buf, "0x%"PRIx32, entry->u.ଚାବି_Sym);
        } ଅନ୍ୟଥା {
            ଅଚଳ xkb_atom_t name = (substitutions == ଖାଲି)
                ? entry->u.ଚାବି_Name
                : substitute_name(substitutions, entry->u.ଚାବି_Name);
            write_buf(buf, "%s", ଚାବି_Name_Text(keymap->ctx, name));
        }
    }
    ଯଦି (current_mods) {
        copy_to_buf(buf, " };\n");
    }

exit:
    darray_free(modmaps);
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
write_symbols(ଅଚଳ ସଂରଚନା xkb_keymap *keymap,
              ଅଚଳ ସଂରଚନା xkb_keymap_serialize_config *config,
              ଅଚଳ ଚାବି_name_substitutions *substitutions,
              xkb_layout_index_t max_groups, bool some_interp,
              ସଂରଚନା buf *buf)
{
    ଅଚଳ bool pretty = (config->flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ଅଚଳ bool defaults =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_ମୂଲ୍ୟଗୁଡ଼ିକ);
    ଅଚଳ bool explicit_ଚାବି_ମୂଲ୍ୟଗୁଡ଼ିକ =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_ଚାବି_ମୂଲ୍ୟଗୁଡ଼ିକ);
    ଅଚଳ bool drop_unused = !(config->flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    ଅଚଳ bool drop_interprets = (explicit_ଚାବି_ମୂଲ୍ୟଗୁଡ଼ିକ && drop_unused);
    /* TODO: print all default values if explicit flag is set */

    ଯଦି (keymap->symbols_section_name)
        write_buf(buf, "xkb_symbols \"%s\" {\n", keymap->symbols_section_name);
    ଅନ୍ୟଥା
        copy_to_buf(buf, "xkb_symbols {\n");

    bool has_group_names = false;
    xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
    ପାଇଁ (xkb_layout_index_t group = 0; group < keymap->ଅଙ୍କ_group_names; group++) {
        ଯଦି (!((UINT32_C(1) << group) & config->layouts))
            ଜାରି;

        new_group++;

        ଯଦି (keymap->group_names[group]) {
            write_buf(buf, "\tname[%"PRIu32"]=", new_group + 1);
            write_buf_string_literal(
                buf, xkb_atom_text(keymap->ctx, keymap->group_names[group]));
            copy_to_buf(buf, ";\n");
            has_group_names = true;
        }
    }
    ଯଦି (has_group_names)
        copy_to_buf(buf, "\n");

    ଯଦି (defaults && !write_actions_defaults(keymap, config->format, buf))
        ଫେରନ୍ତୁ false;

    ସଂରଚନା buf buf2 = { ଖାଲି, 0, 0 };
    ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି;
    xkb_ଚାବିଗୁଡ଼ିକ_foreach(ଚାବି, keymap) {
        /* Skip keys with no explicit values */
        ଯଦି (ଚାବି->explicit &&
            !write_ଚାବି(keymap, config, substitutions, max_groups, some_interp,
                       drop_interprets, explicit_ଚାବି_ମୂଲ୍ୟଗୁଡ଼ିକ, pretty, buf,
                       &buf2, ଚାବି))
        {
            ମୁକ୍ତ(buf2.buf);
            ଫେରନ୍ତୁ false;
        }
    }
    ମୁକ୍ତ(buf2.buf);

    ଯଦି (!write_modmaps(keymap, config->format, substitutions,
                       pretty, drop_interprets, buf))
        ଫେରନ୍ତୁ false;

    copy_to_buf(buf, "};\n\n");
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଗଣନା xkb_status
write_keymap(ଅଚଳ ସଂରଚନା xkb_keymap *keymap,
             ଅଚଳ ସଂରଚନା xkb_keymap_serialize_config *config,
             ସଂରଚନା buf *buf, ସଂରଚନା xkb_keymap_serialize_result *result)
{
    ଚାବି_name_substitutions substitutions = darray_new();
    ଯଦି (config->format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !rename_long_ଚାବିଗୁଡ଼ିକ(keymap, &substitutions))
    {
        ଫେରନ୍ତୁ false;
    }

    ଅଚଳ ଚାବି_name_substitutions * ଅଚଳ substitutions_ptr =
        (darray_empty(substitutions)) ? ଖାଲି : &substitutions;

    ଅଚଳ xkb_layout_index_t max_groups = format_max_groups(config->format);

    bool some_interp = false;
    ଅଚଳ bool ok = (
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
    ଫେରନ୍ତୁ ok ? XKB_SUCCESS : -1;
}

ଗଣନା xkb_status
text_v1_keymap_serialize(
        ଅଚଳ ସଂରଚନା xkb_keymap *keymap,
        ଅଚଳ ସଂରଚନା xkb_keymap_serialize_config *config,
        ସଂରଚନା xkb_keymap_serialize_result *result
)
{
    ସଂରଚନା buf buf = { ଖାଲି, 0, 0 };

    ଅଚଳ ଗଣନା xkb_status status =
        write_keymap(keymap, config, &buf, result);
    ଯଦି (status != XKB_SUCCESS) {
        ମୁକ୍ତ(buf.buf);
        result->serialized = ଖାଲି;
        ଫେରନ୍ତୁ status;
    }

    result->serialized = buf.buf;
    result->length = buf.size + 1;
    result->layouts = config->layouts;

    ଫେରନ୍ତୁ XKB_SUCCESS;
}
