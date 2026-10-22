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

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>
#అన్తర్భావ <stdint.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>

#అన్తర్భావ "xkbcommon/xkbcommon-status.h"
#అన్తర్భావ "xkbcommon/xkbcommon-keysyms.h"
#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "atom.h"
#అన్తర్భావ "action.h"
#అన్తర్భావ "context.h"
#అన్తర్భావ "darray.h"
#అన్తర్భావ "keymap.h"
#అన్తర్భావ "messages-codes.h"
#అన్తర్భావ "text.h"
#అన్తర్భావ "util-numbers.h"
#అన్తర్భావ "xkbcomp-priv.h"

#నిర్వచించు BUF_CHUNK_కొలత 4096

నిర్మాణం buf {
    అక్షరం *buf;
    కొలత_t కొలత;
    కొలత_t alloc;
};

#నిర్వచించు xkb_abs(n) _Generic((n),                \
                            పూర్ణాంకం: నిరపేక్ష,           \
                            దీర్ఘ పూర్ణాంకం: labs,     \
                            అప్రమేయ: llabs )((n))

స్థిర bool
do_realloc(నిర్మాణం buf *buf, కొలత_t at_కనిష్ఠ)
{
    buf->alloc += BUF_CHUNK_కొలత;
    ఒకవేళ (at_కనిష్ఠ >= BUF_CHUNK_కొలత)
        buf->alloc += at_కనిష్ఠ;

    అక్షరం *స్థిరరాశి new = realloc(buf->buf, buf->alloc);
    ఒకవేళ (!new)
        తిరిగి false;

    buf->buf = new;
    తిరిగి true;
}

ATTR_PRINTF(2, 3) స్థిర bool
check_write_buf(నిర్మాణం buf *buf, స్థిరరాశి అక్షరం *fmt, ...)
{
    va_list వాదనలు;
    పూర్ణాంకం printed;
    కొలత_t అందుబాటు;

    అందుబాటు = buf->alloc - buf->కొలత;
    /* Try to write to the buffer and get the required min size (without the
     * final null byte).
     * C11 states that if `available` is 0, then the pointer may be null. */
    ప్రతిజ్ఞా(buf->buf || అందుబాటు == 0);
    va_ప్రారంభం(వాదనలు, fmt);
    printed = vsnprintf(buf->buf ? buf->buf + buf->కొలత : buf->buf, అందుబాటు,
                        fmt, వాదనలు);
    va_చివర(వాదనలు);

    ఒకవేళ (printed < 0) {
        /* Some error */
        వెళ్ళు err;
    } లేకపోతే ఒకవేళ ((కొలత_t)printed >= అందుబాటు) {
        /* Not enough space: need to realloc */
        ఒకవేళ (!do_realloc(buf, (కొలత_t)printed + 1))
            వెళ్ళు err;

        /* The buffer has enough space now. */
        అందుబాటు = buf->alloc - buf->కొలత;
        ప్రతిజ్ఞా(buf->buf && (కొలత_t) printed < అందుబాటు);
        va_ప్రారంభం(వాదనలు, fmt);
        printed = vsnprintf(buf->buf + buf->కొలత, అందుబాటు, fmt, వాదనలు);
        va_చివర(వాదనలు);

        ఒకవేళ (printed < 0 || (కొలత_t) printed >= అందుబాటు)
            వెళ్ళు err;
    }

    buf->కొలత += (కొలత_t)printed;
    తిరిగి true;

err:
    విడుదల(buf->buf);
    buf->buf = ఖాళీ;
    తిరిగి false;
}

#నిర్వచించు write_buf(buf, ...) చేయి { \
    ఒకవేళ (!check_write_buf(buf, __VA_ARGS__)) \
        తిరిగి false; \
} అయితే (0)

స్థిర bool
check_copy_గమ్యం_buf(నిర్మాణం buf *buf, స్థిరరాశి అక్షరం* ఆధారం, కొలత_t len)
{
    ఒకవేళ (len == 0)
        తిరిగి true;

    స్థిరరాశి కొలత_t అందుబాటు = buf->alloc - buf->కొలత;
    ఒకవేళ (len >= అందుబాటు &&
        /* len + 1 (terminating NULL) */
        !do_realloc(buf, len + 1))
    {
        విడుదల(buf->buf);
        buf->buf = ఖాళీ;
        తిరిగి false;
    }

    జ్ఞాపకం_నకలు(buf->buf + buf->కొలత, ఆధారం, len);
    buf->కొలత += len;
    /* Append NULL byte */
    buf->buf[buf->కొలత] = '\0';
    తిరిగి true;
}

#నిర్వచించు copy_గమ్యం_buf_len(buf, ఆధారం, len) చేయి { \
    ఒకవేళ (!check_copy_గమ్యం_buf(buf, ఆధారం, len)) \
        తిరిగి false; \
} అయితే (0)

#నిర్వచించు copy_గమ్యం_buf(buf, ఆధారం) \
    copy_గమ్యం_buf_len(buf, ఆధారం, పరిమాణం(ఆధారం) - 1)

స్థిర bool
check_write_string_literal(నిర్మాణం buf *buf, స్థిరరాశి అక్షరం* string)
{
    copy_గమ్యం_buf(buf, "\"");

    /* Write chunks, separated by characters requiring escape sequence */
    కొలత_t pending_ప్రారంభం = 0;
    కొలత_t current = 0;
    అక్షరం escape_బఫరు[] = {'\\', '0', '0', '0', '0'};
    కొరకు (; string[current] != '\0'; current++) {
        uint8_t escape_len;
        ఎంపిక (string[current]) {
        స్థితి '\n':
            /* `\n` would break strings */
            escape_బఫరు[1] = 'n';
            escape_len = 2;
            ఆపు;
        స్థితి '\\':
            /* `\` would create escape sequences */
            escape_బఫరు[1] = '\\';
            escape_len = 2;
            ఆపు;
        అప్రమేయ:
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
            ఒకవేళ (unlikely((గుర్తులేని అక్షరం)string[current] < 0x20U ||
                         string[current] == '"')) {
                escape_బఫరు[1] = '0';
                escape_బఫరు[2] = '0';
                escape_బఫరు[3] = (అక్షరం)('0' + (string[current] >> 3));
                escape_బఫరు[4] = (అక్షరం)('0' + (string[current] & 0x7));
                escape_len = 5;
            } లేకపోతే {
                /* Expand the chunk */
                కొనసాగు;
            }
        }
        /* Write pending chunk */
        ప్రతిజ్ఞా(current >= pending_ప్రారంభం);
        copy_గమ్యం_buf_len(buf, string + pending_ప్రారంభం, current - pending_ప్రారంభం);
        /* Write escape sequence */
        copy_గమ్యం_buf_len(buf, escape_బఫరు, escape_len);
        pending_ప్రారంభం = current + 1;
    }
    /* Write remaining chunk */
    ప్రతిజ్ఞా(current >= pending_ప్రారంభం);
    copy_గమ్యం_buf_len(buf, string + pending_ప్రారంభం, current - pending_ప్రారంభం);

    copy_గమ్యం_buf(buf, "\"");
    తిరిగి true;
}

#నిర్వచించు write_buf_string_literal(buf, string) చేయి { \
    ఒకవేళ (!check_write_string_literal(buf, string)) \
        తిరిగి false; \
} అయితే (0)

స్థిర శూన్య
delete_చివరి_char(నిర్మాణం buf *buf, అక్షరం c)
{
    ఒకవేళ (buf->కొలత && buf->buf[buf->కొలత - 1] == c)
        buf->buf[--buf->కొలత] = '\0';
}

/** Entry for a name substituion LUT */
నిర్మాణం కీలకం_name_substitution {
    xkb_atom_t ఆధారం;
    xkb_atom_t లక్ష్యం;
    /** Used for intuitive renaming */
    xkb_keycode_t keycode;
};

/** Compare 2 key name substitution entries by they original name (atom) */
స్థిర పూర్ణాంకం
cmp_కీలకం_name_substitution(స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b)
{
    స్థిరరాశి xkb_atom_t s1 = ((నిర్మాణం కీలకం_name_substitution *) a)->ఆధారం;
    స్థిరరాశి xkb_atom_t s2 = ((నిర్మాణం కీలకం_name_substitution *) b)->ఆధారం;
    తిరిగి (s1 < s2) ? -1 : ((s1 > s2) ? 1 : 0);
}

రకనిర్వచనం darray(నిర్మాణం కీలకం_name_substitution) కీలకం_name_substitutions;

/**
 * Rename keys that are not compatible with the original XKB protocol,
 * i.e. more that 4 bytes. The new name will be formed as prefix + 3-digit
 * hexadecimal number.
 */
స్థిర bool
rename_long_కీలకాలు(స్థిరరాశి నిర్మాణం xkb_keymap *keymap,
                 కీలకం_name_substitutions *substitutions)
{
    /*
     * Prefixes we want to use for the new names: ASCII range `!`..`0`.
     * They are really unlikely to be used in usual keymaps.
     * NULL denotes an unavailable prefix.
     */
    గణన {
        మొదటి = '!',
        చివరి = '0',
        invalid = '\0'
    };
    అక్షరం prefixes[] = {
        మొదటి, invalid /* skip ‘"’, which would look buggy otherwise */,
        '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', చివరి
    };
    static_assert(పరిమాణం(prefixes) == చివరి - మొదటి + 1, "invalid range");

    /*
     * Traverse all the keys and aliases:
     * - Collect items with incompatible names.
     * - Remove prefixes that are already used in valid 4-char names.
     *   This ensures no name conflict will occur with the new names, without
     *   requiring an extensive check.
     */

    /* Check key names */
    స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం;
    xkb_కీలకాలు_foreach(కీలకం, keymap) {
        ఒకవేళ (కీలకం->name == XKB_ATOM_NONE)
            కొనసాగు;

        స్థిరరాశి అక్షరం * స్థిరరాశి name = xkb_atom_పాఠ్యం(keymap->ctx, కీలకం->name);
        స్థిరరాశి కొలత_t len = strlen_సురక్షిత(name);
        ఒకవేళ (len > 4) {
            /* Key name too long: append key, pending for renaming */
            darray_append(*substitutions, (నిర్మాణం కీలకం_name_substitution) {
                .ఆధారం = కీలకం->name,
                .keycode = కీలకం->keycode,
                .లక్ష్యం = కీలకం->name
            });
        } లేకపోతే ఒకవేళ (len == 4 && name[0] >= మొదటి && name[0] <= చివరి) {
            /* Potential name conflict: deactivate this prefix */
            prefixes[name[0] - మొదటి] = invalid;
        }
    }

    /* Check aliases */
    కొరకు (darray_కొలత_t a = 0; a < keymap->అంకె_కీలకం_aliases; a++) {
        స్థిరరాశి నిర్మాణం xkb_కీలకం_alias * స్థిరరాశి alias = &keymap->కీలకం_aliases[a];
        స్థిరరాశి అక్షరం * స్థిరరాశి name = xkb_atom_పాఠ్యం(keymap->ctx, alias->alias);
        స్థిరరాశి కొలత_t len = strlen_సురక్షిత(name);
        ఒకవేళ (len > 4) {
            /* Alias name too long: append alias, pending for renaming */
            darray_append(*substitutions, (నిర్మాణం కీలకం_name_substitution) {
                .ఆధారం = alias->alias,
                .keycode = XKB_KEYCODE_INVALID, /* Too expensive to lookup */
                .లక్ష్యం = alias->alias
            });
        } లేకపోతే ఒకవేళ (len == 4 && name[0] >= మొదటి && name[0] <= చివరి) {
            /* Potential conflict with names substitutes: deactivate prefix */
            prefixes[name[0] - మొదటి] = invalid;
        }
    }

    ఒకవేళ darray_ఖాళీలు(*substitutions) {
        /* Nothing to do */
        తిరిగి true;
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
    అక్షరం *ఉపసర్గ = prefixes;
    స్థిరరాశి అక్షరం *ఉపసర్గ_max = prefixes + పరిమాణం(prefixes) - 1;
    అయితే (*ఉపసర్గ == invalid && ఉపసర్గ < ఉపసర్గ_max)
        ఉపసర్గ++;

    /* Keycode prefix: prefer ‘0’ if available */
    అక్షరం keycode_ఉపసర్గ;
    ఒకవేళ (prefixes[చివరి - మొదటి] == '0') {
        /* ‘0’ is available: use it then mark it unavailable for further use */
        keycode_ఉపసర్గ = చివరి;
        prefixes[చివరి - మొదటి] = invalid;
    } లేకపోతే {
        /* ‘0’ is unavailable: use first available prefix */
        keycode_ఉపసర్గ = *ఉపసర్గ;
        ఒకవేళ (unlikely(keycode_ఉపసర్గ == invalid)) {
            /*
             * Abort: no prefix available (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            darray_free(*substitutions);
            తిరిగి true;
        }
        /* Update current prefix */
        *ఉపసర్గ = invalid;
        అయితే (*ఉపసర్గ == invalid && ఉపసర్గ < ఉపసర్గ_max)
            ఉపసర్గ++;
    }

    /* Proceed renaming */
    uint16_t తదుపరి_id = 1;
    నిర్మాణం కీలకం_name_substitution *sub;
    అక్షరం buf[6] = {0}; /* 5 would suffice, but generates a compiler warning */
    darray_foreach(sub, *substitutions) {
        static_assert(XKB_KEYCODE_INVALID > 0xfff, "Invalid keys filtered out");
        ఒకవేళ (sub->keycode <= 0xfff) {
            /*
             * Try nice formatting with the hexadecimal keycode.
             * We use lower case to reduce possible conflict because the
             * traditional key names are upper-cased.
             */
            snprintf(buf, పరిమాణం(buf), "%c%03"PRIx32,
                     keycode_ఉపసర్గ, sub->keycode);
            స్థిరరాశి xkb_atom_t లక్ష్యం = xkb_atom_intern(keymap->ctx,
                                                      buf, పరిమాణం(buf) - 2);
            ఒకవేళ (లక్ష్యం == XKB_ATOM_NONE)
                వెళ్ళు error;
            sub->లక్ష్యం = లక్ష్యం;
        } లేకపోతే ఒకవేళ (*ఉపసర్గ != invalid) {
            /* Cannot use keycode for formatting: generate next arbitrary ID */
            snprintf(buf, పరిమాణం(buf), "%c%03"PRIx16, *ఉపసర్గ, తదుపరి_id);
            స్థిరరాశి xkb_atom_t లక్ష్యం = xkb_atom_intern(keymap->ctx,
                                                      buf, పరిమాణం(buf) - 2);
            ఒకవేళ (లక్ష్యం == XKB_ATOM_NONE)
                వెళ్ళు error;
            sub->లక్ష్యం = లక్ష్యం;
            ఒకవేళ (++తదుపరి_id > 0xfff) {
                *ఉపసర్గ = invalid;
                /* Get next available prefix */
                అయితే (*ఉపసర్గ == invalid && ఉపసర్గ < ఉపసర్గ_max)
                    ఉపసర్గ++;
                /* Reset counter */
                తదుపరి_id = 1;
            }
        } లేకపోతే {
            /*
             * No prefix available: skip substitution (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            sub->లక్ష్యం = sub->ఆధారం;
        }
    }

    /* Now sort by the source names so we can do a binary search */
    qsort(darray_వస్తువులు(*substitutions), darray_కొలత(*substitutions),
          పరిమాణం(*darray_వస్తువులు(*substitutions)), &cmp_కీలకం_name_substitution);

    తిరిగి true;

error:
    darray_free(*substitutions);
    /* There was a memory error */
    log_err(keymap->ctx, XKB_ERROR_కేటాయింపు_FAILURE_2,
            "Cannot allocate key name substitution\n");
    తిరిగి false;
}

/** Substitute a key name with a valid X11 key name */
స్థిర xkb_atom_t
substitute_name(స్థిరరాశి కీలకం_name_substitutions *substitutions, xkb_atom_t name)
{
    /*ద్విమాన శోధన*/
    darray_కొలత_t మొదటి = 0;
    darray_కొలత_t చివరి = darray_కొలత(*substitutions);

    అయితే (చివరి > మొదటి) {
        darray_కొలత_t mid = మొదటి + (చివరి - 1 - మొదటి) / 2;
        స్థిరరాశి xkb_atom_t ఆధారం = darray_అంశం(*substitutions, mid).ఆధారం;
        ఒకవేళ (ఆధారం < name)
            మొదటి = mid + 1;
        లేకపోతే ఒకవేళ (ఆధారం > name)
            చివరి = mid;
        లేకపోతే /* found it */
            తిరిగి darray_అంశం(*substitutions, mid).లక్ష్యం;
    }

    /* No name substitution: return original name */
    తిరిగి name;
}

స్థిర bool
write_vmods(స్థిరరాశి నిర్మాణం xkb_keymap *keymap, గణన xkb_keymap_format format,
            bool explicit, నిర్మాణం buf *buf)
{
    స్థిరరాశి నిర్మాణం xkb_mod *mod;
    xkb_mod_సూచీ_t vmod = 0;
    bool has_some = false;

    xkb_vmods_enumerate(vmod, mod, &keymap->mods) {
        ఒకవేళ (!has_some) {
            copy_గమ్యం_buf(buf, "\tvirtual_modifiers ");
            has_some = true;
        } లేకపోతే {
            copy_గమ్యం_buf(buf, ",");
        }
        write_buf(buf, "%s", xkb_atom_పాఠ్యం(keymap->ctx, mod->name));

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
        స్థిరరాశి bool auto_canonical_mods = (format >= XKB_KEYMAP_FORMAT_పాఠ్యం_V2);

        ఒకవేళ ((keymap->mods.explicit_vmods & (UINT32_C(1) << vmod) ||
             explicit) &&
            (auto_canonical_mods || mod->mapping != 0)) {
            /*
             * Explicit non-default mapping
             * NOTE: we can only pretty-print *real* modifiers in this context.
             */
            write_buf(buf, "=%s",
                      Mod_ముసుగు_పాఠ్యం(keymap->ctx, MOD_REAL, &keymap->mods,
                                  mod->mapping));
        }
    }

    ఒకవేళ (has_some)
        copy_గమ్యం_buf(buf, ";\n\n");

    తిరిగి true;
}

స్థిర bool
write_keycodes(స్థిరరాశి నిర్మాణం xkb_keymap *keymap,
               స్థిరరాశి కీలకం_name_substitutions *substitutions,
               గణన xkb_keymap_serialize_flags flags, నిర్మాణం buf *buf)
{
    స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం;
    xkb_led_సూచీ_t idx;
    స్థిరరాశి నిర్మాణం xkb_led *led;

    స్థిరరాశి bool pretty = !!(flags & XKB_KEYMAP_SERIALIZE_PRETTY);

    ఒకవేళ (keymap->keycodes_section_name)
        write_buf(buf, "xkb_keycodes \"%s\" {\n",
                  keymap->keycodes_section_name);
    లేకపోతే
        copy_గమ్యం_buf(buf, "xkb_keycodes {\n");

    /* NOTE: there is no support for default values */

    /* xkbcomp and X11 really want to see keymaps with a minimum of 8, and
     * a maximum of at least 255, else XWayland really starts hating life.
     * If this is a problem and people really need strictly bounded keymaps,
     * we should probably control this with a flag. */
    write_buf(buf, "\tminimum = %"PRIu32";\n", MIN(keymap->min_కీలకం_సంకేతం, 8));
    write_buf(buf, "\tmaximum = %"PRIu32";\n", MAX(keymap->max_కీలకం_సంకేతం, 255));

    xkb_కీలకాలు_foreach(కీలకం, keymap) {
        ఒకవేళ (కీలకం->name == XKB_ATOM_NONE)
            కొనసాగు;

        స్థిరరాశి xkb_atom_t name = (substitutions == ఖాళీ)
            ? కీలకం->name
            : substitute_name(substitutions, కీలకం->name);
        ఒకవేళ (pretty)
            write_buf(buf, "\t%-20s", కీలకం_Name_పాఠ్యం(keymap->ctx, name));
        లేకపోతే
            write_buf(buf, "\t%s", కీలకం_Name_పాఠ్యం(keymap->ctx, name));
        write_buf(buf, " = %"PRIu32";\n", కీలకం->keycode);
    }

    xkb_leds_enumerate(idx, led, keymap)
        ఒకవేళ (led->name != XKB_ATOM_NONE) {
            write_buf(buf, "\tindicator %"PRIu32" = ", idx + 1);
            write_buf_string_literal(buf, xkb_atom_పాఠ్యం(keymap->ctx, led->name));
            copy_గమ్యం_buf(buf, ";\n");
        }

    కొరకు (darray_కొలత_t i = 0; i < keymap->అంకె_కీలకం_aliases; i++) {
        స్థిరరాశి xkb_atom_t alias = (substitutions == ఖాళీ)
            ? keymap->కీలకం_aliases[i].alias
            : substitute_name(substitutions, keymap->కీలకం_aliases[i].alias);
        స్థిరరాశి xkb_atom_t real = (substitutions == ఖాళీ)
            ? keymap->కీలకం_aliases[i].real
            : substitute_name(substitutions, keymap->కీలకం_aliases[i].real);

        ఒకవేళ (pretty)
            write_buf(buf, "\talias %-14s",
                      కీలకం_Name_పాఠ్యం(keymap->ctx, alias));
        లేకపోతే
            write_buf(buf, "\talias %s",
                      కీలకం_Name_పాఠ్యం(keymap->ctx, alias));
        write_buf(buf, " = %s;\n",
                  కీలకం_Name_పాఠ్యం(keymap->ctx, real));
    }

    copy_గమ్యం_buf(buf, "};\n\n");
    తిరిగి true;
}

స్థిర bool
write_types(స్థిరరాశి నిర్మాణం xkb_keymap *keymap, గణన xkb_keymap_format format,
            గణన xkb_keymap_serialize_flags flags, నిర్మాణం buf *buf)
{
    ఒకవేళ (keymap->types_section_name)
        write_buf(buf, "xkb_types \"%s\" {\n",
                  keymap->types_section_name);
    లేకపోతే
        copy_గమ్యం_buf(buf, "xkb_types {\n");

    /* NOTE: support for default values has been removed */

    ఒకవేళ (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        తిరిగి false;

    స్థిరరాశి bool drop_వాడని = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_వాడని);
    కొరకు (darray_కొలత_t i = 0; i < keymap->అంకె_types; i++) {
        స్థిరరాశి నిర్మాణం xkb_కీలకం_type * స్థిరరాశి type = &keymap->types[i];
        ఒకవేళ (!type->required && drop_వాడని)
            కొనసాగు;

        copy_గమ్యం_buf(buf, "\ttype ");
        write_buf_string_literal(buf, xkb_atom_పాఠ్యం(keymap->ctx, type->name));
        copy_గమ్యం_buf(buf, " {\n");

        write_buf(buf, "\t\tmodifiers= %s;\n",
                  Mod_ముసుగు_పాఠ్యం(keymap->ctx, MOD_BOTH, &keymap->mods,
                              type->mods.mods));

        కొరకు (darray_కొలత_t j = 0; j < type->అంకె_entries; j++) {
            స్థిరరాశి అక్షరం *str;
            స్థిరరాశి నిర్మాణం xkb_కీలకం_type_నమోదు *నమోదు_2 = &type->entries[j];

            /*
             * Printing level 1 entries is redundant, it's the default,
             * unless there's preserve info.
             */
            ఒకవేళ (నమోదు_2->level == 0 && నమోదు_2->preserve.mods == 0 &&
                !(flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_విలువలు))
                కొనసాగు;

            str = Mod_ముసుగు_పాఠ్యం(keymap->ctx, MOD_BOTH, &keymap->mods,
                              నమోదు_2->mods.mods);
            write_buf(buf, "\t\tmap[%s]= %"PRIu32";\n",
                      str, నమోదు_2->level + 1);

            ఒకవేళ (నమోదు_2->preserve.mods)
                write_buf(buf, "\t\tpreserve[%s]= %s;\n",
                          str, Mod_ముసుగు_పాఠ్యం(keymap->ctx, MOD_BOTH, &keymap->mods,
                                           నమోదు_2->preserve.mods));
        }

        కొరకు (xkb_level_సూచీ_t n = 0; n < type->అంకె_level_పేర్లు; n++)
            ఒకవేళ (type->level_పేర్లు[n]) {
                write_buf(buf, "\t\tlevel_name[%"PRIu32"]= ", n + 1);
                write_buf_string_literal(
                    buf, xkb_atom_పాఠ్యం(keymap->ctx, type->level_పేర్లు[n]));
                copy_గమ్యం_buf(buf, ";\n");
            }

        copy_గమ్యం_buf(buf, "\t};\n");
    }

    copy_గమ్యం_buf(buf, "};\n\n");
    తిరిగి true;
}

స్థిర bool
write_led_map(స్థిరరాశి నిర్మాణం xkb_keymap *keymap, గణన xkb_keymap_format format,
              bool defaults, నిర్మాణం buf *buf, స్థిరరాశి నిర్మాణం xkb_led *led)
{
    copy_గమ్యం_buf(buf, "\tindicator ");
    write_buf_string_literal(buf, xkb_atom_పాఠ్యం(keymap->ctx, led->name));
    copy_గమ్యం_buf(buf, " {\n");

    ఒకవేళ (led->which_groups) {
        ఒకవేళ (led->which_groups != XKB_STATE_LAYOUT_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichGroupState= %s;\n",
                      Led_State_ముసుగు_పాఠ్యం(keymap->ctx, సమూహం_Component_ముసుగు_పేర్లు,
                                       led->which_groups));
        }
        write_buf(buf, "\t\tgroups= 0x%02"PRIx32";\n",
                  led->groups);
    }

    ఒకవేళ (led->which_mods) {
        ఒకవేళ (led->which_mods != XKB_STATE_MODS_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichModState= %s;\n",
                      Led_State_ముసుగు_పాఠ్యం(keymap->ctx, mod_Component_ముసుగు_పేర్లు,
                                       led->which_mods));
        }
        write_buf(buf, "\t\tmodifiers= %s;\n",
                  Mod_ముసుగు_పాఠ్యం(keymap->ctx, MOD_BOTH, &keymap->mods,
                              led->mods.mods));
    }

    ఒకవేళ (led->ctrls) {
        write_buf(buf, "\t\tcontrols= %s;\n",
                  Control_ముసుగు_పాఠ్యం(keymap->ctx, format, led->ctrls));
    }

    copy_గమ్యం_buf(buf, "\t};\n");
    తిరిగి true;
}

స్థిర స్థిరరాశి అక్షరం *
affect_తాళం_పాఠ్యం(గణన xkb_action_flags flags, bool show_both)
{
    ఎంపిక (flags & (ACTION_తాళం_NO_తాళం | ACTION_తాళం_NO_UNLOCK)) {
    స్థితి 0:
        తిరిగి show_both ? ",affect=both" : "";
    స్థితి ACTION_తాళం_NO_UNLOCK:
        తిరిగి ",affect=lock";
    స్థితి ACTION_తాళం_NO_తాళం:
        తిరిగి ",affect=unlock";
    స్థితి ACTION_తాళం_NO_తాళం | ACTION_తాళం_NO_UNLOCK:
        తిరిగి ",affect=neither";
    అప్రమేయ:
        తిరిగి "";
    }
}

#నిర్వచించు చిహ్నం_PADDING 15
#నిర్వచించు ACTION_PADDING 30

స్థిర bool
write_action(స్థిరరాశి నిర్మాణం xkb_keymap *keymap, గణన xkb_keymap_format format,
             xkb_layout_సూచీ_t max_groups, నిర్మాణం buf *buf,
             స్థిరరాశి సంఘం xkb_action *action,
             స్థిరరాశి అక్షరం *ఉపసర్గ, స్థిరరాశి అక్షరం *suffix)
{
    స్థిరరాశి అక్షరం *type;
    స్థిరరాశి అక్షరం *వాదనలు = ఖాళీ;

    ఒకవేళ (!ఉపసర్గ)
        ఉపసర్గ = "";
    ఒకవేళ (!suffix)
        suffix = "";

    type = Action_Type_పాఠ్యం(action->type);

    ఎంపిక (action->type) {
    స్థితి ACTION_TYPE_MOD_సమితి:
    స్థితి ACTION_TYPE_MOD_LATCH:
    స్థితి ACTION_TYPE_MOD_తాళం:
        ఒకవేళ (action->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            వాదనలు = "modMapMods";
        లేకపోతే
            వాదనలు = Mod_ముసుగు_పాఠ్యం(keymap->ctx, MOD_BOTH, &keymap->mods,
                               action->mods.mods.mods);
        bool unlockOnPress = (action->mods.flags & ACTION_UNLOCK_ON_PRESS);
        ఒకవేళ (unlockOnPress && !is_Mods_Un_తాళం_On_Press_Supported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_పాఠ్యం_FORMAT,
                    "Cannot use \"%s(unlockOnPress=true)\" in keymap format %d\n",
                    Action_Type_పాఠ్యం(action->type), format);
            unlockOnPress = false;
        }
        bool latchOnPress = action->type == ACTION_TYPE_MOD_LATCH &&
                            (action->సమూహం.flags & ACTION_LATCH_ON_PRESS);
        ఒకవేళ (latchOnPress && !isModsLatchOnPressSupported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_పాఠ్యం_FORMAT,
                    "Cannot use \"LatchMods(latchOnPress=true)\" "
                    "in keymap format %d\n", format);
            latchOnPress = false;
        }
        write_buf(buf, "%s%s(modifiers=%s%s%s%s%s%s)%s", ఉపసర్గ, type, వాదనలు,
                  (action->type != ACTION_TYPE_MOD_తాళం && (action->mods.flags & ACTION_తాళం_CLEAR)) ? ",clearLocks" : "",
                  (action->type == ACTION_TYPE_MOD_LATCH && (action->mods.flags & ACTION_LATCH_గమ్యం_తాళం)) ? ",latchToLock" : "",
                  (action->type == ACTION_TYPE_MOD_తాళం) ? affect_తాళం_పాఠ్యం(action->mods.flags, false) : "",
                  (unlockOnPress) ? ",unlockOnPress" : "",
                  (latchOnPress) ? ",latchOnPress" : "",
                  suffix);
        ఆపు;

    స్థితి ACTION_TYPE_సమూహం_సమితి:
    స్థితి ACTION_TYPE_సమూహం_LATCH:
    స్థితి ACTION_TYPE_సమూహం_తాళం:
        ఒకవేళ ((uint32_t) xkb_abs(action->సమూహం.సమూహం) <
            (max_groups + !(action->సమూహం.flags & ACTION_ABSOLUTE_SWITCH))) {
            bool తాళం_On_Release = action->type == ACTION_TYPE_సమూహం_తాళం &&
                                 (action->సమూహం.flags & ACTION_తాళం_ON_RELEASE);
            ఒకవేళ (తాళం_On_Release && !is_సమూహం_తాళం_On_Release_Supported(format)) {
                log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_పాఠ్యం_FORMAT,
                        "Cannot use \"GroupLock(lockOnRelease=true)\" "
                        "in keymap format %d\n", format);
                తాళం_On_Release = false;
            }
            write_buf(buf, "%s%s(group=%s%"PRId32"%s%s%s)%s", ఉపసర్గ, type,
                      (!(action->సమూహం.flags & ACTION_ABSOLUTE_SWITCH) && action->సమూహం.సమూహం >= 0) ? "+" : "",
                      (action->సమూహం.flags & ACTION_ABSOLUTE_SWITCH) ? action->సమూహం.సమూహం + 1 : action->సమూహం.సమూహం,
                      (action->type != ACTION_TYPE_సమూహం_తాళం && (action->సమూహం.flags & ACTION_తాళం_CLEAR)) ? ",clearLocks" : "",
                      (action->type == ACTION_TYPE_సమూహం_LATCH && (action->సమూహం.flags & ACTION_LATCH_గమ్యం_తాళం)) ? ",latchToLock" : "",
                      (తాళం_On_Release) ? ",lockOnRelease" : "",
                      suffix);
        } లేకపోతే {
            /* Unsupported group index: degrade to VoidAction() */
            log_warn(keymap->ctx, XKB_LOG_సందేశం_NO_ID,
                     "Unsupported group index %"PRIu32" for action %s()\n",
                      (action->సమూహం.సమూహం +
                       !!(action->సమూహం.flags & ACTION_ABSOLUTE_SWITCH)),
                     type);
            వెళ్ళు void_action;
        }
        ఆపు;

    స్థితి ACTION_TYPE_TERMINATE:
        write_buf(buf, "%s%s()%s", ఉపసర్గ, type, suffix);
        ఆపు;

    స్థితి ACTION_TYPE_PTR_MOVE:
        write_buf(buf, "%s%s(x=%s%d,y=%s%d%s)%s", ఉపసర్గ, type,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_X) && action->ptr.x >= 0) ? "+" : "",
                  action->ptr.x,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_Y) && action->ptr.y >= 0) ? "+" : "",
                  action->ptr.y,
                  (action->ptr.flags & ACTION_REPEAT) ? "" : ",!repeat",
                  suffix);
        ఆపు;

    స్థితి ACTION_TYPE_PTR_తాళం:
        వాదనలు = affect_తాళం_పాఠ్యం(action->btn.flags, true);
        /* fallthrough */
    స్థితి ACTION_TYPE_PTR_BUTTON:
        write_buf(buf, "%s%s(button=", ఉపసర్గ, type);
        static_assert(XKB_సూచిక_BUTTON_DEFAULT < XKB_సూచిక_BUTTON_MIN, "");
        ఒకవేళ (action->btn.button > XKB_సూచిక_BUTTON_DEFAULT &&
            action->btn.button <= XKB_సూచిక_BUTTON_MAX)
            write_buf(buf, "%"PRIu8, action->btn.button);
        లేకపోతే
            copy_గమ్యం_buf(buf, "default");
        ఒకవేళ (action->btn.లెక్క) {
            ప్రతిజ్ఞా(action->type != ACTION_TYPE_PTR_తాళం);
            write_buf(buf, ",count=%u", action->btn.లెక్క);
        }
        ఒకవేళ (వాదనలు)
            write_buf(buf, "%s", వాదనలు);
        write_buf(buf, ")%s", suffix);
        ఆపు;

    స్థితి ACTION_TYPE_PTR_DEFAULT:
        write_buf(buf, "%s%s(", ఉపసర్గ, type);
        write_buf(buf, "affect=button,button=%s%"PRId8,
                  (!(action->dflt.flags & ACTION_ABSOLUTE_SWITCH) && action->dflt.విలువ >= 0) ? "+" : "",
                  action->dflt.విలువ);
        write_buf(buf, ")%s", suffix);
        ఆపు;

    స్థితి ACTION_TYPE_SWITCH_VT:
        write_buf(buf, "%s%s(screen=%s%d,%ssame)%s", ఉపసర్గ, type,
                  (!(action->screen.flags & ACTION_ABSOLUTE_SWITCH) && action->screen.screen >= 0) ? "+" : "",
                  action->screen.screen,
                  (action->screen.flags & ACTION_SAME_SCREEN) ? "" : "!",
                  suffix);
        ఆపు;

    స్థితి ACTION_TYPE_CTRL_సమితి:
    స్థితి ACTION_TYPE_CTRL_తాళం:
        write_buf(buf, "%s%s(controls=%s%s)%s", ఉపసర్గ, type,
                  Control_ముసుగు_పాఠ్యం(keymap->ctx, format, action->ctrls.ctrls),
                  (action->type == ACTION_TYPE_CTRL_తాళం
                    ? affect_తాళం_పాఠ్యం(action->ctrls.flags, false)
                    : ""),
                  suffix);
        ఆపు;

    స్థితి ACTION_TYPE_REDIRECT_కీలకం: {
        write_buf(buf, "%s%s(", ఉపసర్గ, type);
        స్థిరరాశి నిర్మాణం xkb_కీలకం * స్థిరరాశి కీలకం = Xkb_కీలకం(keymap, action->redirect.keycode);
        /* Can fail if the keycode was not initialized */
        ఒకవేళ (కీలకం)
            write_buf(buf, "keycode=%s", కీలకం_Name_పాఠ్యం(keymap->ctx, కీలకం->name));
        ఒకవేళ (action->redirect.affect_mods) {
            xkb_mod_ముసుగు_t ముసుగు;
            ముసుగు = (action->redirect.affect_mods & action->redirect.mods);
            ఒకవేళ (ముసుగు)
                write_buf(buf, ",modifiers=%s",
                          Mod_ముసుగు_పాఠ్యం(keymap->ctx, MOD_BOTH, &keymap->mods, ముసుగు));
            ముసుగు = (action->redirect.affect_mods & ~action->redirect.mods);
            ఒకవేళ (ముసుగు)
                write_buf(buf, ",clearMods=%s",
                          Mod_ముసుగు_పాఠ్యం(keymap->ctx, MOD_BOTH, &keymap->mods, ముసుగు));
        }
        write_buf(buf, ")%s", suffix);
        ఆపు;
    }

    స్థితి ACTION_TYPE_UNKNOWN:
    స్థితి ACTION_TYPE_NONE:
        write_buf(buf, "%sNoAction()%s", ఉపసర్గ, suffix);
        ఆపు;

    స్థితి ACTION_TYPE_VOID:
void_action:
        /*
         * VoidAction() is an xkbcommon extension.
         * Use LockControls as a backward-compatible fallback.
         * We cannot serialize it to `NoAction()`, as it would be dropped in
         * e.g. the context of multiple actions.
         * We better not use `Private` either, because it could still be
         * interpreted by X11.
         */
        ఒకవేళ (format == XKB_KEYMAP_FORMAT_పాఠ్యం_V1)
            write_buf(buf, "%sLockControls(controls=none,affect=neither)%s",
                      ఉపసర్గ, suffix);
        లేకపోతే
            write_buf(buf, "%sVoidAction()%s", ఉపసర్గ, suffix);
        ఆపు;

    అప్రమేయ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_అంతర్గత == 20 &&
                      ACTION_TYPE_అంతర్గత + 1 == ACTION_TYPE_అంకె_ENTRIES,
                      "Missing action type");
        /* Unsupported legacy actions should have degraded to NoAction */
        ప్రతిజ్ఞా(action->type != ACTION_TYPE_UNSUPPORTED_LEGACY);
        write_buf(buf,
                  "%s%s(type=0x%02x,data[0]=0x%02x,data[1]=0x%02x,data[2]=0x%02x,data[3]=0x%02x,data[4]=0x%02x,data[5]=0x%02x,data[6]=0x%02x)%s",
                  ఉపసర్గ, type, action->type, action->priv.దత్తాంశం[0],
                  action->priv.దత్తాంశం[1], action->priv.దత్తాంశం[2],
                  action->priv.దత్తాంశం[3], action->priv.దత్తాంశం[4],
                  action->priv.దత్తాంశం[5], action->priv.దత్తాంశం[6],
                  suffix);
        ఆపు;
    }

    తిరిగి true;
}

స్థిర స్థిరరాశి సంఘం xkb_action void_actions[] = {
    { .type = ACTION_TYPE_VOID }
};

స్థిర bool
write_actions(స్థిరరాశి నిర్మాణం xkb_keymap *keymap, గణన xkb_keymap_format format,
              xkb_layout_సూచీ_t max_groups, నిర్మాణం buf *buf, నిర్మాణం buf *buf2,
              స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం, xkb_layout_సూచీ_t సమూహం)
{
    స్థిర స్థిరరాశి సంఘం xkb_action noAction = { .type = ACTION_TYPE_NONE };

    కొరకు (xkb_level_సూచీ_t level = 0; level < Xkb_కీలకం_అంకె_స్థాయిలు(కీలకం, సమూహం);
         level++) {

        ఒకవేళ (level != 0)
            copy_గమ్యం_buf(buf, ", ");

        స్థిరరాశి సంఘం xkb_action *actions = ఖాళీ;
        xkb_action_లెక్క_t లెక్క = xkb_keymap_కీలకం_get_actions_by_level(
            keymap, కీలకం, సమూహం, level, &actions
        );

        ఒకవేళ (లెక్క > 1 && format < XKB_KEYMAP_FORMAT_పాఠ్యం_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            actions = void_actions;
            లెక్క = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_పాఠ్యం_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        buf2->కొలత = 0;
        ఒకవేళ (లెక్క == 0) {
            ఒకవేళ (!write_action(keymap, format, max_groups,
                              buf2, &noAction, ఖాళీ, ఖాళీ))
                తిరిగి false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        లేకపోతే ఒకవేళ (లెక్క == 1) {
            ఒకవేళ (!write_action(keymap, format, max_groups,
                              buf2, &(actions[0]), ఖాళీ, ఖాళీ))
                తిరిగి false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        లేకపోతే {
            copy_గమ్యం_buf(buf2, "{ ");
            కొరకు (xkb_action_లెక్క_t k = 0; k < లెక్క; k++) {
                ఒకవేళ (k != 0)
                    copy_గమ్యం_buf(buf2, ", ");
                కొలత_t old_కొలత = buf2->కొలత;
                ఒకవేళ (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), ఖాళీ, ఖాళీ))
                    తిరిగి false;
                /* Check if padding is necessary */
                ఒకవేళ (buf2->కొలత >= old_కొలత + ACTION_PADDING)
                    కొనసాగు;
                /* Compute and write padding, then write the action again */
                స్థిరరాశి పూర్ణాంకం padding = (పూర్ణాంకం)(old_కొలత + ACTION_PADDING - buf2->కొలత);
                buf2->కొలత = old_కొలత;
                write_buf(buf2, "%*s", padding, "");
                ఒకవేళ (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), ఖాళీ, ఖాళీ))
                    తిరిగి false;
            }
            copy_గమ్యం_buf(buf2, " }");
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
    }

    తిరిగి true;
}

స్థిర bool
write_action_defaults(స్థిరరాశి నిర్మాణం xkb_keymap *keymap,
                      గణన xkb_keymap_format format,
                      స్థిరరాశి సంఘం xkb_action *action,
                      నిర్మాణం buf *buf)
{
    స్థిరరాశి అక్షరం *type = Action_Type_పాఠ్యం(action->type);

#నిర్వచించు ఉపసర్గ_2 "\t"

    ఎంపిక (action->type) {
    స్థితి ACTION_TYPE_MOD_సమితి:
    స్థితి ACTION_TYPE_MOD_LATCH:
    స్థితి ACTION_TYPE_MOD_తాళం:
        ప్రతిజ్ఞా(action->mods.flags == 0);
        ప్రతిజ్ఞా(action->mods.mods.mods == 0);
        write_buf(buf, ఉపసర్గ_2"%s.modifiers = 0;\n", type);
        ఒకవేళ (action->type != ACTION_TYPE_MOD_తాళం)
            write_buf(buf, ఉపసర్గ_2"%s.clearLocks = false;\n", type);
        ఒకవేళ (action->type == ACTION_TYPE_MOD_LATCH &&
            isModsLatchOnPressSupported(format))
            write_buf(buf, ఉపసర్గ_2"%s.latchOnPress = false;\n", type);
        ఒకవేళ (action->type == ACTION_TYPE_MOD_LATCH)
            write_buf(buf, ఉపసర్గ_2"%s.latchToLock = false;\n", type);
        ఒకవేళ (action->type == ACTION_TYPE_MOD_తాళం)
            write_buf(buf, ఉపసర్గ_2"%s.affect = both;\n", type);
        ఒకవేళ (is_Mods_Un_తాళం_On_Press_Supported(format))
            write_buf(buf, ఉపసర్గ_2"%s.unlockOnPress = false;\n", type);
        ఆపు;

    స్థితి ACTION_TYPE_సమూహం_సమితి:
    స్థితి ACTION_TYPE_సమూహం_LATCH:
    స్థితి ACTION_TYPE_సమూహం_తాళం:
        ప్రతిజ్ఞా(action->సమూహం.flags == 0);
        ప్రతిజ్ఞా(action->సమూహం.సమూహం == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, ఉపసర్గ_2"%s.group = +0;\n", type);
        ఒకవేళ (action->type != ACTION_TYPE_సమూహం_తాళం)
            write_buf(buf, ఉపసర్గ_2"%s.clearLocks = false;\n", type);
        ఒకవేళ (action->type == ACTION_TYPE_సమూహం_LATCH)
            write_buf(buf, ఉపసర్గ_2"%s.latchToLock = false;\n", type);
        ఒకవేళ (action->type == ACTION_TYPE_సమూహం_తాళం &&
            is_సమూహం_తాళం_On_Release_Supported(format))
            write_buf(buf, ఉపసర్గ_2"%s.lockOnRelease = false;\n", type);
        ఆపు;


    స్థితి ACTION_TYPE_PTR_MOVE:
        ప్రతిజ్ఞా(action->ptr.x == 0);
        ప్రతిజ్ఞా(action->ptr.y == 0);
        ప్రతిజ్ఞా(action->ptr.flags == ACTION_REPEAT);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, ఉపసర్గ_2"%s.x = +0;\n", type);
        write_buf(buf, ఉపసర్గ_2"%s.y = +0;\n", type);
        write_buf(buf, ఉపసర్గ_2"%s.accel = true;\n", type);
        ఆపు;

    స్థితి ACTION_TYPE_PTR_తాళం:
    స్థితి ACTION_TYPE_PTR_BUTTON:
        ప్రతిజ్ఞా(action->btn.flags == 0);
        ప్రతిజ్ఞా(action->btn.button == 0);
        ప్రతిజ్ఞా(action->btn.లెక్క == 0);
        write_buf(buf, ఉపసర్గ_2"%s.button = 0;\n", type);
        write_buf(buf, ఉపసర్గ_2"%s.count = 0;\n", type);
        ఒకవేళ (action->type == ACTION_TYPE_PTR_తాళం)
            write_buf(buf, ఉపసర్గ_2"%s.affect = both;\n", type);
        ఆపు;

    స్థితి ACTION_TYPE_PTR_DEFAULT:
        ప్రతిజ్ఞా(action->dflt.flags == 0);
        ప్రతిజ్ఞా(action->dflt.విలువ == 1);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, ఉపసర్గ_2"%s.button = +1;\n", type);
        ఆపు;

    స్థితి ACTION_TYPE_SWITCH_VT:
        ప్రతిజ్ఞా(action->screen.flags == ACTION_SAME_SCREEN);
        ప్రతిజ్ఞా(action->screen.screen == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, ఉపసర్గ_2"%s.screen = +0;\n", type);
        write_buf(buf, ఉపసర్గ_2"%s.same = true;\n", type);
        ఆపు;

    స్థితి ACTION_TYPE_CTRL_సమితి:
    స్థితి ACTION_TYPE_CTRL_తాళం:
        ప్రతిజ్ఞా(action->ctrls.flags == 0);
        ప్రతిజ్ఞా(action->ctrls.ctrls == 0);
        write_buf(buf, ఉపసర్గ_2"%s.controls = 0;\n", type);
        ఒకవేళ (action->type == ACTION_TYPE_CTRL_తాళం)
            write_buf(buf, ఉపసర్గ_2"%s.affect = both;\n", type);
        ఆపు;

    స్థితి ACTION_TYPE_REDIRECT_కీలకం:
        ప్రతిజ్ఞా(action->redirect.keycode == keymap->redirect_కీలకం_auto);
        ప్రతిజ్ఞా(action->redirect.affect_mods == 0);
        ప్రతిజ్ఞా(action->redirect.mods == 0);
        write_buf(buf, ఉపసర్గ_2"%s.keycode = auto;\n", type);
        write_buf(buf, ఉపసర్గ_2"%s.modifiers = 0;\n", type);
        write_buf(buf, ఉపసర్గ_2"%s.clearMods = 0;\n", type);
        ఆపు;

    అప్రమేయ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_అంతర్గత == 20 &&
                      ACTION_TYPE_అంతర్గత + 1 == ACTION_TYPE_అంకె_ENTRIES,
                      "Missing action type");
    }

#నిర్వచనం_రద్దు ఉపసర్గ_2

    తిరిగి true;
}

స్థిర bool
write_actions_defaults(స్థిరరాశి నిర్మాణం xkb_keymap *keymap,
                       గణన xkb_keymap_format format, నిర్మాణం buf *buf)
{
    ActionsInfo defaults = {0};
    InitActionsInfo(keymap, &defaults);

    కొరకు (uint8_t a = 0; a < (uint8_t) ACTION_TYPE_అంకె_ENTRIES; a++) {
        స్థిరరాశి సంఘం xkb_action * పరిమిత స్థిరరాశి action = &defaults.actions[a];
        ఒకవేళ (!write_action_defaults(keymap, format, action, buf))
            తిరిగి false;
    }
    copy_గమ్యం_buf(buf, "\n");

    తిరిగి true;
}

స్థిర స్థిరరాశి నిర్మాణం xkb_sym_interpret fallback_interpret = {
    .sym = XKB_కీలకం_Void_చిహ్నం,
    .required = true,
    .repeat = FALLBACK_INTERPRET_కీలకం_REPEAT,
    .సరిపోలిక = సరిపోలిక_ANY_OR_NONE,
    .mods = 0,
    .virtual_mod = (xkb_mod_సూచీ_t) DEFAULT_INTERPRET_VMOD,
    .అంకె_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

స్థిర bool
write_compat(స్థిరరాశి నిర్మాణం xkb_keymap * పరిమిత keymap,
             గణన xkb_keymap_format format, xkb_layout_సూచీ_t max_groups,
             గణన xkb_keymap_serialize_flags flags, bool * పరిమిత some_interp,
             నిర్మాణం buf *buf)
{
    స్థిరరాశి bool pretty = (flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    స్థిరరాశి bool defaults = (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_విలువలు);
    స్థిరరాశి bool drop_వాడని = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_వాడని);
    స్థిరరాశి bool drop_interprets =
        ((flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_కీలకం_విలువలు) && drop_వాడని);
    /* TODO: print all default values if the explicit flag is set */

    ఒకవేళ (keymap->compat_section_name)
        write_buf(buf, "xkb_compatibility \"%s\" {\n",
                  keymap->compat_section_name);
    లేకపోతే
        copy_గమ్యం_buf(buf, "xkb_compatibility {\n");

    ఒకవేళ (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        తిరిగి false;

    ఒకవేళ (defaults && !write_actions_defaults(keymap, format, buf))
        తిరిగి false;

    copy_గమ్యం_buf(buf, "\tinterpret.useModMapMods= AnyLevel;\n");
    copy_గమ్యం_buf(buf, "\tinterpret.repeat= False;\n");

    /* xkbcomp requires at least one interpret entry. */
    స్థిరరాశి bool use_fallback_interpret =
        (!keymap->అంకె_sym_interprets || drop_interprets);
    స్థిరరాశి darray_కొలత_t అంకె_sym_interprets =
        (use_fallback_interpret ? 1 : keymap->అంకె_sym_interprets);
    స్థిరరాశి నిర్మాణం xkb_sym_interpret* స్థిరరాశి sym_interprets =
        (use_fallback_interpret ? &fallback_interpret : keymap->sym_interprets);

    కొరకు (darray_కొలత_t i = 0; i < అంకె_sym_interprets; i++) {
        స్థిరరాశి నిర్మాణం xkb_sym_interpret *si = &sym_interprets[i];
        ఒకవేళ (!si->required && drop_వాడని)
            కొనసాగు;

        *some_interp = true;

        copy_గమ్యం_buf(buf, "\tinterpret ");
        ఒకవేళ (!si->sym) {
            copy_గమ్యం_buf(buf, "Any");
        } లేకపోతే ఒకవేళ (pretty) {
            write_buf(buf, "%s", Keysym_పాఠ్యం(keymap->ctx, si->sym));
        } లేకపోతే {
            write_buf(buf, "0x%"PRIx32, si->sym);
        }
        write_buf(buf, "+%s(%s) {",
                  SIMatch_పాఠ్యం(si->సరిపోలిక),
                  Mod_ముసుగు_పాఠ్యం(keymap->ctx, MOD_BOTH, &keymap->mods, si->mods));

        bool has_explicit_properties = false;

        ఒకవేళ (si->virtual_mod != XKB_MOD_INVALID) {
            write_buf(buf, "\n\t\tvirtualModifier= %s;",
                      Mod_సూచీ_పాఠ్యం(keymap->ctx, &keymap->mods,
                                   si->virtual_mod));
            has_explicit_properties = true;
        }

        ఒకవేళ (si->level_one_only) {
            copy_గమ్యం_buf(buf, "\n\t\tuseModMapMods=level1;");
            has_explicit_properties = true;
        } లేకపోతే ఒకవేళ (defaults) {
            copy_గమ్యం_buf(buf, "\n\t\tuseModMapMods=AnyLevel;");
            has_explicit_properties = true;
        }

        ఒకవేళ (si->repeat) {
            copy_గమ్యం_buf(buf, "\n\t\trepeat= True;");
            has_explicit_properties = true;
        } లేకపోతే ఒకవేళ (defaults) {
            copy_గమ్యం_buf(buf, "\n\t\trepeat= False;");
            has_explicit_properties = true;
        }

        స్థిరరాశి సంఘం xkb_action *action = ఖాళీ;
        xkb_action_లెక్క_t action_లెక్క = si->అంకె_actions;
        ఒకవేళ (action_లెక్క <= 1) {
            action = &si->a.action;
        } లేకపోతే ఒకవేళ (format < XKB_KEYMAP_FORMAT_పాఠ్యం_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            action = &void_actions[0];
            action_లెక్క = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_పాఠ్యం_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        ఒకవేళ (action_లెక్క > 1) {
            copy_గమ్యం_buf(buf, "\n\t\taction= {");
            స్థిరరాశి అక్షరం suffix[] = ", ";
            కొరకు (xkb_action_లెక్క_t k = 0; k < si->అంకె_actions; k++) {
                ఒకవేళ (!write_action(keymap, format, max_groups,
                                  buf, &si->a.actions[k], "", suffix))
                    తిరిగి false;
            }
            buf->కొలత -= పరిమాణం(suffix) - 1; /* trailing comma */
            copy_గమ్యం_buf(buf, "};");
            has_explicit_properties = true;
        } లేకపోతే ఒకవేళ (action_లెక్క == 1) {
            ఒకవేళ (!write_action(keymap, format, max_groups, buf, action,
                              "\n\t\taction= ", ";"))
                తిరిగి false;
            has_explicit_properties = true;
        }
        write_buf(buf, (has_explicit_properties
                        ? "\n\t};\n"
                        /* Empty interpret is a syntax error in xkbcomp, so
                         * use a dummy entry */
                        : "\n\t\taction= NoAction();\n\t};\n"));
    }

    ఒకవేళ (use_fallback_interpret || (
        /* Detect previous interpret fallback */
        అంకె_sym_interprets == 1 && *some_interp &&
        sym_interprets[0].sym == fallback_interpret.sym &&
        sym_interprets[0].సరిపోలిక == fallback_interpret.సరిపోలిక &&
        sym_interprets[0].mods == fallback_interpret.mods &&
        sym_interprets[0].repeat == fallback_interpret.repeat &&
        sym_interprets[0].virtual_mod == fallback_interpret.virtual_mod &&
        sym_interprets[0].level_one_only == fallback_interpret.level_one_only &&
        sym_interprets[0].అంకె_actions == fallback_interpret.అంకె_actions
        /* `required` field is not compared */
    ))
        *some_interp = false;

    స్థిరరాశి నిర్మాణం xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        ఒకవేళ ((led->which_groups || led->groups ||
             led->which_mods || led->mods.mods || led->ctrls) &&
            !write_led_map(keymap, format, defaults, buf, led))
        {
            తిరిగి false;
        }
    }

    copy_గమ్యం_buf(buf, "};\n\n");

    తిరిగి true;
}

స్థిర bool
write_keysyms(స్థిరరాశి నిర్మాణం xkb_keymap *keymap,
              నిర్మాణం buf *buf, నిర్మాణం buf *buf2,
              స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం, xkb_layout_సూచీ_t సమూహం,
              bool pretty, bool show_actions)
{
    స్థిరరాశి గుర్తులేని పూర్ణాంకం padding = (pretty)
        ? ((show_actions) ? ACTION_PADDING : చిహ్నం_PADDING)
        : 0;
    కొరకు (xkb_level_సూచీ_t level = 0; level < Xkb_కీలకం_అంకె_స్థాయిలు(కీలకం, సమూహం);
         level++) {
        స్థిరరాశి xkb_keysym_t *syms;
        పూర్ణాంకం అంకె_syms;

        ఒకవేళ (level != 0)
            copy_గమ్యం_buf(buf, ", ");

        అంకె_syms = xkb_keymap_కీలకం_get_syms_by_level(
            /* remove `const` constraint to use the dated public API */
            (నిర్మాణం xkb_keymap *)keymap,
            కీలకం->keycode, సమూహం, level, &syms
        );

        స్థిరరాశి xkb_keysym_t no_చిహ్నం = XKB_కీలకం_No_చిహ్నం;
        ఒకవేళ (అంకె_syms == 0) {
            అంకె_syms = 1;
            syms = &no_చిహ్నం;
        }

        /*
         * NOTE: Use `NoSymbol` even without pretty output, for compatibility
         * with xkbcomp and xkbcommon < 1.12
         */

        ఒకవేళ (అంకె_syms == 1) {
            ఒకవేళ (pretty || syms[0] == XKB_కీలకం_No_చిహ్నం)
                write_buf(buf, "%*s", padding, Keysym_పాఠ్యం(keymap->ctx, syms[0]));
            లేకపోతే
                write_buf(buf, "0x%"PRIx32, syms[0]);
        } లేకపోతే {
            ఒకవేళ (pretty) {
                buf2->కొలత = 0;
                copy_గమ్యం_buf(buf2, "{ ");
                కొరకు (పూర్ణాంకం s = 0; s < అంకె_syms; s++) {
                    ఒకవేళ (s != 0)
                        copy_గమ్యం_buf(buf2, ", ");
                    write_buf(buf2, "%*s", (show_actions ? padding : 0),
                              Keysym_పాఠ్యం(keymap->ctx, syms[s]));
                }
                copy_గమ్యం_buf(buf2, " }");
                write_buf(buf, "%*s", padding, buf2->buf);
            } లేకపోతే {
                copy_గమ్యం_buf(buf, "{");
                కొరకు (పూర్ణాంకం s = 0; s < అంకె_syms; s++) {
                    ఒకవేళ (s != 0)
                        copy_గమ్యం_buf(buf, ",");
                    ఒకవేళ (syms[s] == XKB_కీలకం_No_చిహ్నం)
                        copy_గమ్యం_buf(buf, "NoSymbol");
                    లేకపోతే
                        write_buf(buf, "0x%"PRIx32, syms[s]);
                }
                copy_గమ్యం_buf(buf, "}");
            }
        }
    }

    తిరిగి true;
}

స్థిర bool
write_కీలకం(స్థిరరాశి నిర్మాణం xkb_keymap *keymap,
          స్థిరరాశి నిర్మాణం xkb_keymap_serialize_config *config,
          స్థిరరాశి కీలకం_name_substitutions *substitutions,
          xkb_layout_సూచీ_t max_groups, bool some_interprets,
          bool drop_interprets, bool explicit, bool pretty,
          నిర్మాణం buf *buf, నిర్మాణం buf *buf2, స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం)
{
    bool simple = true;

    xkb_layout_ముసుగు_t layouts = config->layouts;
    ఒకవేళ (popcount32(layouts) != keymap->అంకె_groups) {
        /* Subset of layouts: use out-of-range key policy */
        xkb_layout_సూచీ_t l = 0;
        కొరకు (xkb_layout_ముసుగు_t ls = layouts; ls; ls >>= 1, l++) {
            ఒకవేళ (ls & 0x1) {
                /* Replace target layout with its effective layout, if valid */
                layouts &= ~(UINT32_C(1) << l);
                స్థిరరాశి xkb_layout_సూచీ_t effective =
                    xkb_keymap_కీలకం_effective_layout(కీలకం, l);
                ఒకవేళ (effective != XKB_LAYOUT_INVALID)
                    layouts |= (UINT32_C(1) << effective);
            }
        }
    } లేకపోతే {
        layouts &= ((UINT64_C(1) << కీలకం->అంకె_groups) - 1);
    }
    స్థిరరాశి xkb_layout_సూచీ_t groups_లెక్క = popcount32(layouts);

    స్థిరరాశి xkb_atom_t name = (substitutions == ఖాళీ)
        ? కీలకం->name
        : substitute_name(substitutions, కీలకం->name);

    ఒకవేళ (pretty)
        write_buf(buf, "\tkey %-20s {", కీలకం_Name_పాఠ్యం(keymap->ctx, name));
    లేకపోతే
        write_buf(buf, "\tkey %s {", కీలకం_Name_పాఠ్యం(keymap->ctx, name));

    ఒకవేళ ((కీలకం->explicit & EXPLICIT_TYPES) || explicit) {
        simple = false;

        bool multi_type = explicit;
        /* Check for distinct key types only if not requiring explicit values */
        ఒకవేళ (!explicit) {
            కొరకు (xkb_layout_సూచీ_t సమూహం = 1; సమూహం < కీలకం->అంకె_groups; సమూహం++) {
                ఒకవేళ (!((UINT32_C(1) << సమూహం) & layouts))
                    కొనసాగు;

                ఒకవేళ (కీలకం->groups[సమూహం].type != కీలకం->groups[0].type) {
                    multi_type = true;
                    ఆపు;
                }
            }
        }

        ఒకవేళ (multi_type) {
            xkb_layout_సూచీ_t new_సమూహం = (xkb_layout_సూచీ_t)-1;
            కొరకు (xkb_layout_సూచీ_t సమూహం = 0; సమూహం < కీలకం->అంకె_groups; సమూహం++) {
                ఒకవేళ (!((UINT32_C(1) << సమూహం) & layouts))
                    కొనసాగు;

                new_సమూహం++;

                ఒకవేళ (!కీలకం->groups[సమూహం].explicit_type && !explicit)
                    కొనసాగు;

                స్థిరరాశి నిర్మాణం xkb_కీలకం_type * స్థిరరాశి type = కీలకం->groups[సమూహం].type;
                write_buf(buf, "\n\t\ttype[%"PRIu32"]= ", new_సమూహం + 1);
                write_buf_string_literal(
                  buf, xkb_atom_పాఠ్యం(keymap->ctx, type->name));
                copy_గమ్యం_buf(buf, ",");
            }
        }
        లేకపోతే {
            స్థిరరాశి నిర్మాణం xkb_కీలకం_type * స్థిరరాశి type = కీలకం->groups[0].type;
            write_buf(buf, "\n\t\ttype= ");
            write_buf_string_literal(
                buf, xkb_atom_పాఠ్యం(keymap->ctx, type->name));
            copy_గమ్యం_buf(buf, ",");
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
    స్థిరరాశి bool explicit_actions = (కీలకం->explicit & EXPLICIT_INTERP);

    /* Check if an explicit value is required */
    #నిర్వచించు require_explicit(కీలకం, prop,                                       \
                             విలువ, initial_విలువ, default_interpret_విలువ) ( \
        /* Explicit value in the source */                                    \
        ((కీలకం)->explicit & (prop)) ||                                         \
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
                ((కీలకం)->implicit_actions && !some_interprets)                 \
            ) && (                                                            \
                /* Non-initial value */                                       \
                (విలువ) != (initial_విలువ) ||                                 \
                /* Force initial value for clarity */                         \
                (initial_విలువ) != (default_interpret_విలువ)                  \
            )                                                                 \
        )                                                                     \
    )

    ఒకవేళ (require_explicit(కీలకం, EXPLICIT_REPEAT, కీలకం->repeats,
                         DEFAULT_కీలకం_REPEAT, DEFAULT_INTERPRET_కీలకం_REPEAT)) {
        ఒకవేళ (కీలకం->repeats)
            copy_గమ్యం_buf(buf, "\n\t\trepeat= Yes,");
        లేకపోతే
            copy_గమ్యం_buf(buf, "\n\t\trepeat= No,");
        simple = false;
    }

    ఒకవేళ (require_explicit(కీలకం, EXPLICIT_VMODMAP, కీలకం->vmodmap,
                         (xkb_mod_ముసుగు_t) DEFAULT_కీలకం_VMODMAP,
                         (xkb_mod_ముసుగు_t) DEFAULT_INTERPRET_VMODMAP)) {
        write_buf(buf, "\n\t\tvirtualMods= %s,",
                  Mod_ముసుగు_పాఠ్యం(keymap->ctx, MOD_BOTH, &keymap->mods,
                              కీలకం->vmodmap));
        simple = false;
    }

    #నిర్వచనం_రద్దు require_explicit

    ఎంపిక (కీలకం->బహిర్గతం_యొక్క_range_సమూహం_policy) {
    స్థితి XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_CLAMP:
        copy_గమ్యం_buf(buf, "\n\t\tgroupsClamp,");
        simple = false;
        ఆపు;

    స్థితి XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_REDIRECT: {
        ఒకవేళ (!groups_లెక్క) {
            /* No layout to redirect to: skip */
            ఆపు;
        }

        xkb_layout_ముసుగు_t layout_ముసుగు =
            (UINT32_C(1) << కీలకం->బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య);
        ఒకవేళ (!(layouts & layout_ముసుగు)) {
            /*
             * Layout is not included: default to first layout
             * See: XkbWrapGroupIntoRange
             */
            log_warn(keymap->ctx, XKB_LOG_సందేశం_NO_ID,
                     "key %s: groupsRedirect=%"PRIu32" converted to "
                     "groupsRedirect=1\n",
                     కీలకం_Name_పాఠ్యం(keymap->ctx, name),
                     కీలకం->బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య + 1);
            layout_ముసుగు = 0x1;
        }

        /* In case some lower layouts were discarded, update the group index */
        స్థిరరాశి xkb_layout_ముసుగు_t దిగువ = (layouts & (layout_ముసుగు - 1));
        స్థిరరాశి xkb_layout_సూచీ_t new = popcount32(దిగువ);
        write_buf(buf, "\n\t\tgroupsRedirect= %"PRIu32",", new + 1);
        simple = false;
        ఆపు;
    }

    స్థితి XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_WRAP:
    అప్రమేయ:
        ఆపు;
    }

    ఒకవేళ (కీలకం->overlays) {
        xkb_overlay_ముసుగు_t మిగిలిన = కీలకం->overlays;

        స్థిరరాశి xkb_overlay_సూచీ_t overlay_max =
            format_max_overlays(config->format);
        static_assert(XKB_OVERLAY_లెక్క == 8, "invalid right shift");
        స్థిరరాశి xkb_overlay_ముసుగు_t valid =
            (xkb_overlay_ముసుగు_t)((1u << overlay_max) - 1u);
        మిగిలిన &= valid;
        ఒకవేళ (మిగిలిన != కీలకం->overlays) {
            log_warn(keymap->ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_సూచీ,
                     "Overlays indices > %u in %s require using "
                     "keymap format >= v2; "
                     "keep overlays 0x%08x and discard overlays 0x%08x\n",
                     overlay_max, కీలకం_Name_పాఠ్యం(keymap->ctx, name),
                     మిగిలిన, కీలకం->overlays & ~valid);
        }

        ఒకవేళ (మిగిలిన && !కీలకం->overlays_inline &&
            !areOverlappingOverlaysSupported(config->format)) {
            /* isolate lowest set bit */
            స్థిరరాశి xkb_overlay_ముసుగు_t lsb = (xkb_overlay_ముసుగు_t)(
                మిగిలిన &
                (xkb_overlay_ముసుగు_t)((గుర్తులేని)~మిగిలిన + 1u)
            );
            /* get its index */
            స్థిరరాశి xkb_overlay_సూచీ_t overlay =
                (xkb_overlay_సూచీ_t)popcount32(lsb - 1u);
            log_warn(keymap->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                     "Overlapping overlays in %s require using "
                     "keymap format >= v2; "
                     "keep overlay %u and discard overlays 0x%x\n",
                     కీలకం_Name_పాఠ్యం(keymap->ctx, name), overlay, మిగిలిన & ~lsb);
            మిగిలిన = lsb;
        }

        ఒకవేళ (మిగిలిన)
            simple = false;

        స్థిరరాశి నిర్మాణం xkb_కీలకం * స్థిరరాశి *overlays_కీలకాలు = (కీలకం->overlays_inline)
                                                    ? &కీలకం->overlay_కీలకం
                                                    : కీలకం->overlays_కీలకాలు;
        అయితే (మిగిలిన) {
            /* isolate lowest set bit */
            స్థిరరాశి xkb_overlay_ముసుగు_t lsb = (xkb_overlay_ముసుగు_t)(
                మిగిలిన &
                (xkb_overlay_ముసుగు_t)((గుర్తులేని)~మిగిలిన + 1u)
            );
            /* get its index */
            స్థిరరాశి xkb_overlay_సూచీ_t overlay =
                (xkb_overlay_సూచీ_t)popcount32(lsb - 1u);
            మిగిలిన = (మిగిలిన & ~lsb);

            /* pop current value */
            ఒకవేళ (కీలకం->overlays_inline && మిగిలిన)
                PANIC_UNREACHABLE();
            స్థిరరాశి నిర్మాణం xkb_కీలకం *overlay_కీలకం = *(overlays_కీలకాలు++);

            స్థిరరాశి xkb_atom_t overlay_కీలకం_name = (substitutions == ఖాళీ)
                ? overlay_కీలకం->name
                : substitute_name(substitutions, overlay_కీలకం->name);
            write_buf(buf, "\n\t\toverlay%u= %s,",
                      overlay + 1, కీలకం_Name_పాఠ్యం(keymap->ctx, overlay_కీలకం_name));
        }
    }

    ఒకవేళ (groups_లెక్క > 1 || explicit_actions || explicit)
        simple = false;

    ఒకవేళ (simple) {
        ఒకవేళ (groups_లెక్క == 0) {
            /* Remove trailing comma */
            delete_చివరి_char(buf, ',');
            ఒకవేళ (buf->buf[buf->కొలత - 1] == ',')
                buf->కొలత--;
        } లేకపోతే {
            copy_గమ్యం_buf(buf, "\t[ ");
            /* Get the single group index */
            xkb_mod_సూచీ_t సమూహం = ctz32(layouts);
            /* The group may not be defined for this key if it is not the first one */
            సమూహం = xkb_keymap_కీలకం_effective_layout(కీలకం, సమూహం);
            ఒకవేళ (!write_keysyms(keymap, buf, buf2, కీలకం, సమూహం, pretty, false))
                తిరిగి false;
            copy_గమ్యం_buf(buf, " ]");
        }
        copy_గమ్యం_buf(buf, " };\n");
    }
    లేకపోతే {
        xkb_layout_సూచీ_t new_సమూహం = (xkb_layout_సూచీ_t)-1;
        కొరకు (xkb_layout_సూచీ_t సమూహం = 0; సమూహం < కీలకం->అంకె_groups; సమూహం++) {
            ఒకవేళ (!((UINT32_C(1) << సమూహం) & layouts))
                కొనసాగు;

            new_సమూహం++;

            స్థిరరాశి bool print_actions =
                /* Group has explicit actions */
                కీలకం->groups[సమూహం].explicit_actions ||
                /* Explicit values are required */
                explicit ||
                /* Group has symbols but no explicit actions and key has explicit
                 * actions in another group: ensure compatibility with xkbcomp
                 * and all xkbcommon versions */
                (కీలకం->groups[సమూహం].explicit_symbols &&
                 explicit_actions && some_interprets) ||
                /* Group has implicit actions but no interpret can set them, so
                 * the actions must be made explicit */
                (కీలకం->groups[సమూహం].implicit_actions && !some_interprets);

            ఒకవేళ (new_సమూహం != 0)
                copy_గమ్యం_buf(buf, ",");
            write_buf(buf, "\n\t\tsymbols[%"PRIu32"]= [ ", new_సమూహం + 1);

            ఒకవేళ (!write_keysyms(keymap, buf, buf2, కీలకం, సమూహం,
                               pretty, print_actions))
                తిరిగి false;
            copy_గమ్యం_buf(buf, " ]");
            ఒకవేళ (print_actions) {
                write_buf(buf, ",\n\t\tactions[%"PRIu32"]= [ ", new_సమూహం + 1);
                ఒకవేళ (!write_actions(keymap, config->format, max_groups,
                                   buf, buf2, కీలకం, సమూహం))
                    తిరిగి false;
                copy_గమ్యం_buf(buf, " ]");
            }
        }
        ఒకవేళ (!groups_లెక్క)
            delete_చివరి_char(buf, ',');
        copy_గమ్యం_buf(buf, "\n\t};\n");
    }

    తిరిగి true;
}


స్థిర పూర్ణాంకం
cmp_modmap_అడుగు_1(స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b)
{
    స్థిరరాశి Mod_Map_నమోదు *m1 = (స్థిరరాశి Mod_Map_నమోదు *)a;
    స్థిరరాశి Mod_Map_నమోదు *m2 = (స్థిరరాశి Mod_Map_నమోదు *)b;

    ఒకవేళ (!m1->mods) {
        ఒకవేళ (!m2->mods) {
            /* Entry will be dropped: do not care about the other fields */
            తిరిగి 0;
        }
        తిరిగి +1;
    } లేకపోతే ఒకవేళ (!m2->mods) {
        తిరిగి -1;
    }

    /* Valid only on step 1 */
    ఒకవేళ (m1->కీలకం_సంకేతం < m2->కీలకం_సంకేతం)
        తిరిగి -1;
    లేకపోతే ఒకవేళ (m1->కీలకం_సంకేతం > m2->కీలకం_సంకేతం)
        తిరిగి +1;

    ఒకవేళ (m1->mods < m2->mods)
        తిరిగి -1;
    లేకపోతే ఒకవేళ (m1->mods > m2->mods)
        తిరిగి +1;

    ఒకవేళ (m1->have_చిహ్నం < m2->have_చిహ్నం)
        తిరిగి +1;
    లేకపోతే ఒకవేళ (m1->have_చిహ్నం > m2->have_చిహ్నం)
        తిరిగి -1;

    ఒకవేళ (m1->have_చిహ్నం) {
        ఒకవేళ (m1->u.కీలకం_Sym < m2->u.కీలకం_Sym)
            తిరిగి -1;
        ఒకవేళ (m1->u.కీలకం_Sym > m2->u.కీలకం_Sym)
            తిరిగి +1;
    } లేకపోతే {
        ఒకవేళ (m1->u.కీలకం_Name < m2->u.కీలకం_Name)
            తిరిగి -1;
        ఒకవేళ (m1->u.కీలకం_Name > m2->u.కీలకం_Name)
            తిరిగి +1;
    }

    తిరిగి 0;
}

స్థిర పూర్ణాంకం
cmp_modmap_అడుగు_2(స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b)
{
    స్థిరరాశి Mod_Map_నమోదు *m1 = (స్థిరరాశి Mod_Map_నమోదు *)a;
    స్థిరరాశి Mod_Map_నమోదు *m2 = (స్థిరరాశి Mod_Map_నమోదు *)b;

    ఒకవేళ (m1->mods < m2->mods)
        తిరిగి -1;
    ఒకవేళ (m1->mods > m2->mods)
        తిరిగి +1;

    ఒకవేళ (m1->have_చిహ్నం < m2->have_చిహ్నం)
        తిరిగి -1;
    ఒకవేళ (m1->have_చిహ్నం > m2->have_చిహ్నం)
        తిరిగి +1;

    ఒకవేళ (m1->have_చిహ్నం) {
        ఒకవేళ (m1->u.కీలకం_Sym < m2->u.కీలకం_Sym)
            తిరిగి -1;
        ఒకవేళ (m1->u.కీలకం_Sym > m2->u.కీలకం_Sym)
            తిరిగి +1;
    } లేకపోతే {
        ఒకవేళ (m1->కీలకం_సంకేతం < m2->కీలకం_సంకేతం)
            తిరిగి -1;
        ఒకవేళ (m1->కీలకం_సంకేతం > m2->కీలకం_సంకేతం)
            తిరిగి +1;
    }

    తిరిగి 0;
}

స్థిర bool
write_modmaps(స్థిరరాశి నిర్మాణం xkb_keymap *keymap, గణన xkb_keymap_format format,
              స్థిరరాశి కీలకం_name_substitutions *substitutions,
              bool pretty, bool drop_interprets, నిర్మాణం buf *buf)
{
    ఒకవేళ (!keymap->అంకె_modmaps)
        తిరిగి true;

    darray(Mod_Map_నమోదు) modmaps = darray_new();
    darray_from_వస్తువులు(modmaps, keymap->modmaps, keymap->అంకె_modmaps);
    ఒకవేళ (!darray_వస్తువులు(modmaps))
        తిరిగి false;

    /* Sort entries by key code */
    qsort(darray_వస్తువులు(modmaps), darray_కొలత(modmaps),
          పరిమాణం(*darray_వస్తువులు(modmaps)), &cmp_modmap_అడుగు_1);

    /* Drop disabled entries at the end */
    కొరకు (darray_కొలత_t m = darray_కొలత(modmaps); m-- > 0;) {
        ఒకవేళ (darray_అంశం(modmaps, m).mods)
            ఆపు;
        darray_కొలత(modmaps)--;
    }

    /*
     * Build detailed keymap modmap
     *
     * NOTE: use an explicit for-loop instead of the darray_foreach macro
     * because we may add entries while iterating it. Iterate from the end,
     * so that we do not iterate the new entries.
     */
    xkb_keycode_t current_keycode = XKB_KEYCODE_INVALID;
    bool convert_గమ్యం_v1_compatibility_నమోదు = false;
    కొరకు (darray_కొలత_t m = darray_కొలత(modmaps); m-- > 0;) {
        Mod_Map_నమోదు *mm = &darray_అంశం(modmaps, m);
        ప్రతిజ్ఞా(mm->mods);

        ఒకవేళ (mm->కీలకం_సంకేతం == current_keycode) {
            /*
             * Keycode already processed: either drop next key code entries
             * or convert them to keymap format v1
             */
            స్థిరరాశి xkb_mod_ముసుగు_t core_mods = (mm->mods & MOD_REAL_ముసుగు_ALL);
            ఒకవేళ (convert_గమ్యం_v1_compatibility_నమోదు && core_mods) {
                /*
                 * Convert to keymap format V1-specific entry:
                 * keep only the highest core modifier
                 */
                mm->mods = UINT32_C(1) << (XKB_MAX_MODS - 1 - clz32(core_mods));
            } లేకపోతే {
                /*
                 * Drop if there is a previous entry for all keymap formats or
                 * if the current entry is incompatible with V1 keymap format.
                 */
                mm->mods = 0;
            }
            కొనసాగు;
        }

        current_keycode = mm->కీలకం_సంకేతం;
        స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం = Xkb_కీలకం(keymap, mm->కీలకం_సంకేతం);

        స్థిరరాశి xkb_mod_ముసుగు_t core_mods = (కీలకం->modmap & MOD_REAL_ముసుగు_ALL);
        ఒకవేళ (format == XKB_KEYMAP_FORMAT_పాఠ్యం_V1 &&
            ((కీలకం->modmap & (కీలకం->modmap - 1)) || కీలకం->modmap != core_mods)) {
            /* V1 entry with multiple modifiers or non X11 core modifiers */
            m++; /* process in next iteration */
            convert_గమ్యం_v1_compatibility_నమోదు = true;
            log_err(keymap->ctx, XKB_ERROR_INVALID_MODIFIER_MAP_ముసుగు,
                    "Cannot serialize modifier map of key %s: modifiers mask "
                    "0x%"PRIx32" incompatible with keymap format v1\n",
                    కీలకం_Name_పాఠ్యం(keymap->ctx, కీలకం->name), కీలకం->modmap);
        } లేకపోతే {
            /* V2 format or V1 format with a single core modifier */
            ప్రతిజ్ఞా(కీలకం->modmap);
            mm->mods = కీలకం->modmap;
            /* Force key name */
            mm->have_చిహ్నం = false;
            mm->u.కీలకం_Name = కీలకం->name;
            convert_గమ్యం_v1_compatibility_నమోదు = false;
        }
    }

    ఒకవేళ (darray_ఖాళీలు(modmaps))
        వెళ్ళు నిష్క్రమించు;

    /* Sort entries by modifier mask */
    qsort(darray_వస్తువులు(modmaps), darray_కొలత(modmaps),
          పరిమాణం(*darray_వస్తువులు(modmaps)), &cmp_modmap_అడుగు_2);

    /*
     * Serialize modmaps
     */

    xkb_mod_ముసుగు_t current_mods = 0;
    స్థిరరాశి Mod_Map_నమోదు *నమోదు_2;
    darray_foreach(నమోదు_2, modmaps) {
        ఒకవేళ (!నమోదు_2->mods)
            కొనసాగు;

        ఒకవేళ (current_mods != నమోదు_2->mods) {
            ఒకవేళ (current_mods) {
                copy_గమ్యం_buf(buf, " };\n");
            }
            current_mods = నమోదు_2->mods;
            write_buf(buf, "\tmodifier_map %s { ",
                      Mod_ముసుగు_పాఠ్యం(keymap->ctx, MOD_REAL, &keymap->mods,
                                  current_mods));
        } లేకపోతే {
            copy_గమ్యం_buf(buf, ", ");
        }

        ఒకవేళ (నమోదు_2->have_చిహ్నం) {
            ఒకవేళ (pretty || నమోదు_2->u.కీలకం_Sym == XKB_కీలకం_No_చిహ్నం)
                write_buf(buf, "%s", Keysym_పాఠ్యం(keymap->ctx, నమోదు_2->u.కీలకం_Sym));
            లేకపోతే
                write_buf(buf, "0x%"PRIx32, నమోదు_2->u.కీలకం_Sym);
        } లేకపోతే {
            స్థిరరాశి xkb_atom_t name = (substitutions == ఖాళీ)
                ? నమోదు_2->u.కీలకం_Name
                : substitute_name(substitutions, నమోదు_2->u.కీలకం_Name);
            write_buf(buf, "%s", కీలకం_Name_పాఠ్యం(keymap->ctx, name));
        }
    }
    ఒకవేళ (current_mods) {
        copy_గమ్యం_buf(buf, " };\n");
    }

నిష్క్రమించు:
    darray_free(modmaps);
    తిరిగి true;
}

స్థిర bool
write_symbols(స్థిరరాశి నిర్మాణం xkb_keymap *keymap,
              స్థిరరాశి నిర్మాణం xkb_keymap_serialize_config *config,
              స్థిరరాశి కీలకం_name_substitutions *substitutions,
              xkb_layout_సూచీ_t max_groups, bool some_interp,
              నిర్మాణం buf *buf)
{
    స్థిరరాశి bool pretty = (config->flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    స్థిరరాశి bool defaults =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_విలువలు);
    స్థిరరాశి bool explicit_కీలకం_విలువలు =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_కీలకం_విలువలు);
    స్థిరరాశి bool drop_వాడని = !(config->flags & XKB_KEYMAP_SERIALIZE_KEEP_వాడని);
    స్థిరరాశి bool drop_interprets = (explicit_కీలకం_విలువలు && drop_వాడని);
    /* TODO: print all default values if explicit flag is set */

    ఒకవేళ (keymap->symbols_section_name)
        write_buf(buf, "xkb_symbols \"%s\" {\n", keymap->symbols_section_name);
    లేకపోతే
        copy_గమ్యం_buf(buf, "xkb_symbols {\n");

    bool has_సమూహం_పేర్లు = false;
    xkb_layout_సూచీ_t new_సమూహం = (xkb_layout_సూచీ_t)-1;
    కొరకు (xkb_layout_సూచీ_t సమూహం = 0; సమూహం < keymap->అంకె_సమూహం_పేర్లు; సమూహం++) {
        ఒకవేళ (!((UINT32_C(1) << సమూహం) & config->layouts))
            కొనసాగు;

        new_సమూహం++;

        ఒకవేళ (keymap->సమూహం_పేర్లు[సమూహం]) {
            write_buf(buf, "\tname[%"PRIu32"]=", new_సమూహం + 1);
            write_buf_string_literal(
                buf, xkb_atom_పాఠ్యం(keymap->ctx, keymap->సమూహం_పేర్లు[సమూహం]));
            copy_గమ్యం_buf(buf, ";\n");
            has_సమూహం_పేర్లు = true;
        }
    }
    ఒకవేళ (has_సమూహం_పేర్లు)
        copy_గమ్యం_buf(buf, "\n");

    ఒకవేళ (defaults && !write_actions_defaults(keymap, config->format, buf))
        తిరిగి false;

    నిర్మాణం buf buf2 = { ఖాళీ, 0, 0 };
    స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం;
    xkb_కీలకాలు_foreach(కీలకం, keymap) {
        /* Skip keys with no explicit values */
        ఒకవేళ (కీలకం->explicit &&
            !write_కీలకం(keymap, config, substitutions, max_groups, some_interp,
                       drop_interprets, explicit_కీలకం_విలువలు, pretty, buf,
                       &buf2, కీలకం))
        {
            విడుదల(buf2.buf);
            తిరిగి false;
        }
    }
    విడుదల(buf2.buf);

    ఒకవేళ (!write_modmaps(keymap, config->format, substitutions,
                       pretty, drop_interprets, buf))
        తిరిగి false;

    copy_గమ్యం_buf(buf, "};\n\n");
    తిరిగి true;
}

స్థిర గణన xkb_స్థితి
write_keymap(స్థిరరాశి నిర్మాణం xkb_keymap *keymap,
             స్థిరరాశి నిర్మాణం xkb_keymap_serialize_config *config,
             నిర్మాణం buf *buf, నిర్మాణం xkb_keymap_serialize_ఫలితం *ఫలితం)
{
    కీలకం_name_substitutions substitutions = darray_new();
    ఒకవేళ (config->format == XKB_KEYMAP_FORMAT_పాఠ్యం_V1 &&
        !rename_long_కీలకాలు(keymap, &substitutions))
    {
        తిరిగి false;
    }

    స్థిరరాశి కీలకం_name_substitutions * స్థిరరాశి substitutions_ptr =
        (darray_ఖాళీలు(substitutions)) ? ఖాళీ : &substitutions;

    స్థిరరాశి xkb_layout_సూచీ_t max_groups = format_max_groups(config->format);

    bool some_interp = false;
    స్థిరరాశి bool ok = (
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
    తిరిగి ok ? XKB_SUCCESS : -1;
}

గణన xkb_స్థితి
పాఠ్యం_v1_keymap_serialize(
        స్థిరరాశి నిర్మాణం xkb_keymap *keymap,
        స్థిరరాశి నిర్మాణం xkb_keymap_serialize_config *config,
        నిర్మాణం xkb_keymap_serialize_ఫలితం *ఫలితం
)
{
    నిర్మాణం buf buf = { ఖాళీ, 0, 0 };

    స్థిరరాశి గణన xkb_స్థితి స్థితి_2 =
        write_keymap(keymap, config, &buf, ఫలితం);
    ఒకవేళ (స్థితి_2 != XKB_SUCCESS) {
        విడుదల(buf.buf);
        ఫలితం->serialized = ఖాళీ;
        తిరిగి స్థితి_2;
    }

    ఫలితం->serialized = buf.buf;
    ఫలితం->నిడివి = buf.కొలత + 1;
    ఫలితం->layouts = config->layouts;

    తిరిగి XKB_SUCCESS;
}
