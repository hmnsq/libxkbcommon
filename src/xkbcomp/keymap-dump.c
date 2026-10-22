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

#شامل "config.h"

#شامل <assert.h>
#شامل <stdint.h>
#شامل <stdio.h>
#شامل <stdlib.h>

#شامل "xkbcommon/xkbcommon-status.h"
#شامل "xkbcommon/xkbcommon-keysyms.h"
#شامل "xkbcommon/xkbcommon.h"
#شامل "atom.h"
#شامل "action.h"
#شامل "context.h"
#شامل "darray.h"
#شامل "keymap.h"
#شامل "messages-codes.h"
#شامل "text.h"
#شامل "util-numbers.h"
#شامل "xkbcomp-priv.h"

#define BUF_CHUNK_SIZE 4096

ساختار buf {
    نویسه *buf;
    size_t size;
    size_t alloc;
};

#define xkb_abs(n) _Generic((n),                \
                            صحیح: abs,           \
                            long صحیح: labs,     \
                            پیشفرض: llabs )((n))

ایستا bool
do_realloc(ساختار buf *buf, size_t at_least)
{
    buf->alloc += BUF_CHUNK_SIZE;
    اگر (at_least >= BUF_CHUNK_SIZE)
        buf->alloc += at_least;

    نویسه *ثابت new = realloc(buf->buf, buf->alloc);
    اگر (!new)
        بازگشت false;

    buf->buf = new;
    بازگشت true;
}

ATTR_PRINTF(2, 3) ایستا bool
check_write_buf(ساختار buf *buf, ثابت نویسه *fmt, ...)
{
    va_list args;
    صحیح printed;
    size_t available;

    available = buf->alloc - buf->size;
    /* Try to write to the buffer and get the required min size (without the
     * final null byte).
     * C11 states that if `available` is 0, then the pointer may be null. */
    أكد(buf->buf || available == 0);
    va_start(args, fmt);
    printed = vsnprintf(buf->buf ? buf->buf + buf->size : buf->buf, available,
                        fmt, args);
    va_end(args);

    اگر (printed < 0) {
        /* Some error */
        برو err;
    } وگرنه اگر ((size_t)printed >= available) {
        /* Not enough space: need to realloc */
        اگر (!do_realloc(buf, (size_t)printed + 1))
            برو err;

        /* The buffer has enough space now. */
        available = buf->alloc - buf->size;
        أكد(buf->buf && (size_t) printed < available);
        va_start(args, fmt);
        printed = vsnprintf(buf->buf + buf->size, available, fmt, args);
        va_end(args);

        اگر (printed < 0 || (size_t) printed >= available)
            برو err;
    }

    buf->size += (size_t)printed;
    بازگشت true;

err:
    آزاد(buf->buf);
    buf->buf = پوچ;
    بازگشت false;
}

#define write_buf(buf, ...) انجامبده { \
    اگر (!check_write_buf(buf, __VA_ARGS__)) \
        بازگشت false; \
} وقتی (0)

ایستا bool
check_copy_to_buf(ساختار buf *buf, ثابت نویسه* source, size_t len)
{
    اگر (len == 0)
        بازگشت true;

    ثابت size_t available = buf->alloc - buf->size;
    اگر (len >= available &&
        /* len + 1 (terminating NULL) */
        !do_realloc(buf, len + 1))
    {
        آزاد(buf->buf);
        buf->buf = پوچ;
        بازگشت false;
    }

    memcpy(buf->buf + buf->size, source, len);
    buf->size += len;
    /* Append NULL byte */
    buf->buf[buf->size] = '\0';
    بازگشت true;
}

#define copy_to_buf_len(buf, source, len) انجامبده { \
    اگر (!check_copy_to_buf(buf, source, len)) \
        بازگشت false; \
} وقتی (0)

#define copy_to_buf(buf, source) \
    copy_to_buf_len(buf, source, اندازه(source) - 1)

ایستا bool
check_write_string_literal(ساختار buf *buf, ثابت نویسه* string)
{
    copy_to_buf(buf, "\"");

    /* Write chunks, separated by characters requiring escape sequence */
    size_t pending_start = 0;
    size_t current = 0;
    نویسه escape_buffer[] = {'\\', '0', '0', '0', '0'};
    برای (; string[current] != '\0'; current++) {
        uint8_t escape_len;
        انتخاب (string[current]) {
        حالت '\n':
            /* `\n` would break strings */
            escape_buffer[1] = 'n';
            escape_len = 2;
            بشکن;
        حالت '\\':
            /* `\` would create escape sequences */
            escape_buffer[1] = '\\';
            escape_len = 2;
            بشکن;
        پیشفرض:
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
            اگر (unlikely((unsigned نویسه)string[current] < 0x20U ||
                         string[current] == '"')) {
                escape_buffer[1] = '0';
                escape_buffer[2] = '0';
                escape_buffer[3] = (نویسه)('0' + (string[current] >> 3));
                escape_buffer[4] = (نویسه)('0' + (string[current] & 0x7));
                escape_len = 5;
            } وگرنه {
                /* Expand the chunk */
                ادامه;
            }
        }
        /* Write pending chunk */
        أكد(current >= pending_start);
        copy_to_buf_len(buf, string + pending_start, current - pending_start);
        /* Write escape sequence */
        copy_to_buf_len(buf, escape_buffer, escape_len);
        pending_start = current + 1;
    }
    /* Write remaining chunk */
    أكد(current >= pending_start);
    copy_to_buf_len(buf, string + pending_start, current - pending_start);

    copy_to_buf(buf, "\"");
    بازگشت true;
}

#define write_buf_string_literal(buf, string) انجامبده { \
    اگر (!check_write_string_literal(buf, string)) \
        بازگشت false; \
} وقتی (0)

ایستا تهی
delete_last_char(ساختار buf *buf, نویسه c)
{
    اگر (buf->size && buf->buf[buf->size - 1] == c)
        buf->buf[--buf->size] = '\0';
}

/** Entry for a name substituion LUT */
ساختار کلید_name_substitution {
    xkb_atom_t source;
    xkb_atom_t target;
    /** Used for intuitive renaming */
    xkb_keycode_t keycode;
};

/** Compare 2 key name substitution entries by they original name (atom) */
ایستا صحیح
cmp_کلید_name_substitution(ثابت تهی *a, ثابت تهی *b)
{
    ثابت xkb_atom_t s1 = ((ساختار کلید_name_substitution *) a)->source;
    ثابت xkb_atom_t s2 = ((ساختار کلید_name_substitution *) b)->source;
    بازگشت (s1 < s2) ? -1 : ((s1 > s2) ? 1 : 0);
}

تعریفنوع darray(ساختار کلید_name_substitution) کلید_name_substitutions;

/**
 * Rename keys that are not compatible with the original XKB protocol,
 * i.e. more that 4 bytes. The new name will be formed as prefix + 3-digit
 * hexadecimal number.
 */
ایستا bool
rename_long_کلیدها(ثابت ساختار xkb_keymap *keymap,
                 کلید_name_substitutions *substitutions)
{
    /*
     * Prefixes we want to use for the new names: ASCII range `!`..`0`.
     * They are really unlikely to be used in usual keymaps.
     * NULL denotes an unavailable prefix.
     */
    شمارش {
        first = '!',
        last = '0',
        invalid = '\0'
    };
    نویسه prefixes[] = {
        first, invalid /* skip ‘"’, which would look buggy otherwise */,
        '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', last
    };
    static_assert(اندازه(prefixes) == last - first + 1, "invalid range");

    /*
     * Traverse all the keys and aliases:
     * - Collect items with incompatible names.
     * - Remove prefixes that are already used in valid 4-char names.
     *   This ensures no name conflict will occur with the new names, without
     *   requiring an extensive check.
     */

    /* Check key names */
    ثابت ساختار xkb_کلید *کلید;
    xkb_کلیدها_foreach(کلید, keymap) {
        اگر (کلید->name == XKB_ATOM_NONE)
            ادامه;

        ثابت نویسه * ثابت name = xkb_atom_text(keymap->ctx, کلید->name);
        ثابت size_t len = strlen_safe(name);
        اگر (len > 4) {
            /* Key name too long: append key, pending for renaming */
            darray_append(*substitutions, (ساختار کلید_name_substitution) {
                .source = کلید->name,
                .keycode = کلید->keycode,
                .target = کلید->name
            });
        } وگرنه اگر (len == 4 && name[0] >= first && name[0] <= last) {
            /* Potential name conflict: deactivate this prefix */
            prefixes[name[0] - first] = invalid;
        }
    }

    /* Check aliases */
    برای (darray_size_t a = 0; a < keymap->رقم_کلید_aliases; a++) {
        ثابت ساختار xkb_کلید_alias * ثابت alias = &keymap->کلید_aliases[a];
        ثابت نویسه * ثابت name = xkb_atom_text(keymap->ctx, alias->alias);
        ثابت size_t len = strlen_safe(name);
        اگر (len > 4) {
            /* Alias name too long: append alias, pending for renaming */
            darray_append(*substitutions, (ساختار کلید_name_substitution) {
                .source = alias->alias,
                .keycode = XKB_KEYCODE_INVALID, /* Too expensive to lookup */
                .target = alias->alias
            });
        } وگرنه اگر (len == 4 && name[0] >= first && name[0] <= last) {
            /* Potential conflict with names substitutes: deactivate prefix */
            prefixes[name[0] - first] = invalid;
        }
    }

    اگر darray_empty(*substitutions) {
        /* Nothing to do */
        بازگشت true;
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
    نویسه *prefix = prefixes;
    ثابت نویسه *prefix_max = prefixes + اندازه(prefixes) - 1;
    وقتی (*prefix == invalid && prefix < prefix_max)
        prefix++;

    /* Keycode prefix: prefer ‘0’ if available */
    نویسه keycode_prefix;
    اگر (prefixes[last - first] == '0') {
        /* ‘0’ is available: use it then mark it unavailable for further use */
        keycode_prefix = last;
        prefixes[last - first] = invalid;
    } وگرنه {
        /* ‘0’ is unavailable: use first available prefix */
        keycode_prefix = *prefix;
        اگر (unlikely(keycode_prefix == invalid)) {
            /*
             * Abort: no prefix available (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            darray_free(*substitutions);
            بازگشت true;
        }
        /* Update current prefix */
        *prefix = invalid;
        وقتی (*prefix == invalid && prefix < prefix_max)
            prefix++;
    }

    /* Proceed renaming */
    uint16_t next_id = 1;
    ساختار کلید_name_substitution *sub;
    نویسه buf[6] = {0}; /* 5 would suffice, but generates a compiler warning */
    darray_foreach(sub, *substitutions) {
        static_assert(XKB_KEYCODE_INVALID > 0xfff, "Invalid keys filtered out");
        اگر (sub->keycode <= 0xfff) {
            /*
             * Try nice formatting with the hexadecimal keycode.
             * We use lower case to reduce possible conflict because the
             * traditional key names are upper-cased.
             */
            snprintf(buf, اندازه(buf), "%c%03"PRIx32,
                     keycode_prefix, sub->keycode);
            ثابت xkb_atom_t target = xkb_atom_intern(keymap->ctx,
                                                      buf, اندازه(buf) - 2);
            اگر (target == XKB_ATOM_NONE)
                برو error;
            sub->target = target;
        } وگرنه اگر (*prefix != invalid) {
            /* Cannot use keycode for formatting: generate next arbitrary ID */
            snprintf(buf, اندازه(buf), "%c%03"PRIx16, *prefix, next_id);
            ثابت xkb_atom_t target = xkb_atom_intern(keymap->ctx,
                                                      buf, اندازه(buf) - 2);
            اگر (target == XKB_ATOM_NONE)
                برو error;
            sub->target = target;
            اگر (++next_id > 0xfff) {
                *prefix = invalid;
                /* Get next available prefix */
                وقتی (*prefix == invalid && prefix < prefix_max)
                    prefix++;
                /* Reset counter */
                next_id = 1;
            }
        } وگرنه {
            /*
             * No prefix available: skip substitution (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            sub->target = sub->source;
        }
    }

    /* Now sort by the source names so we can do a binary search */
    qsort(darray_اقلام(*substitutions), darray_size(*substitutions),
          اندازه(*darray_اقلام(*substitutions)), &cmp_کلید_name_substitution);

    بازگشت true;

error:
    darray_free(*substitutions);
    /* There was a memory error */
    log_err(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate key name substitution\n");
    بازگشت false;
}

/** Substitute a key name with a valid X11 key name */
ایستا xkb_atom_t
substitute_name(ثابت کلید_name_substitutions *substitutions, xkb_atom_t name)
{
    /*جستجوی دودویی*/
    darray_size_t first = 0;
    darray_size_t last = darray_size(*substitutions);

    وقتی (last > first) {
        darray_size_t mid = first + (last - 1 - first) / 2;
        ثابت xkb_atom_t source = darray_item(*substitutions, mid).source;
        اگر (source < name)
            first = mid + 1;
        وگرنه اگر (source > name)
            last = mid;
        وگرنه /* found it */
            بازگشت darray_item(*substitutions, mid).target;
    }

    /* No name substitution: return original name */
    بازگشت name;
}

ایستا bool
write_vmods(ثابت ساختار xkb_keymap *keymap, شمارش xkb_keymap_format format,
            bool explicit, ساختار buf *buf)
{
    ثابت ساختار xkb_mod *mod;
    xkb_mod_index_t vmod = 0;
    bool has_some = false;

    xkb_vmods_enumerate(vmod, mod, &keymap->mods) {
        اگر (!has_some) {
            copy_to_buf(buf, "\tvirtual_modifiers ");
            has_some = true;
        } وگرنه {
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
        ثابت bool auto_canonical_mods = (format >= XKB_KEYMAP_FORMAT_TEXT_V2);

        اگر ((keymap->mods.explicit_vmods & (UINT32_C(1) << vmod) ||
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

    اگر (has_some)
        copy_to_buf(buf, ";\n\n");

    بازگشت true;
}

ایستا bool
write_keycodes(ثابت ساختار xkb_keymap *keymap,
               ثابت کلید_name_substitutions *substitutions,
               شمارش xkb_keymap_serialize_flags flags, ساختار buf *buf)
{
    ثابت ساختار xkb_کلید *کلید;
    xkb_led_index_t idx;
    ثابت ساختار xkb_led *led;

    ثابت bool pretty = !!(flags & XKB_KEYMAP_SERIALIZE_PRETTY);

    اگر (keymap->keycodes_section_name)
        write_buf(buf, "xkb_keycodes \"%s\" {\n",
                  keymap->keycodes_section_name);
    وگرنه
        copy_to_buf(buf, "xkb_keycodes {\n");

    /* NOTE: there is no support for default values */

    /* xkbcomp and X11 really want to see keymaps with a minimum of 8, and
     * a maximum of at least 255, else XWayland really starts hating life.
     * If this is a problem and people really need strictly bounded keymaps,
     * we should probably control this with a flag. */
    write_buf(buf, "\tminimum = %"PRIu32";\n", MIN(keymap->min_کلید_code, 8));
    write_buf(buf, "\tmaximum = %"PRIu32";\n", MAX(keymap->max_کلید_code, 255));

    xkb_کلیدها_foreach(کلید, keymap) {
        اگر (کلید->name == XKB_ATOM_NONE)
            ادامه;

        ثابت xkb_atom_t name = (substitutions == پوچ)
            ? کلید->name
            : substitute_name(substitutions, کلید->name);
        اگر (pretty)
            write_buf(buf, "\t%-20s", کلید_Name_Text(keymap->ctx, name));
        وگرنه
            write_buf(buf, "\t%s", کلید_Name_Text(keymap->ctx, name));
        write_buf(buf, " = %"PRIu32";\n", کلید->keycode);
    }

    xkb_leds_enumerate(idx, led, keymap)
        اگر (led->name != XKB_ATOM_NONE) {
            write_buf(buf, "\tindicator %"PRIu32" = ", idx + 1);
            write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, led->name));
            copy_to_buf(buf, ";\n");
        }

    برای (darray_size_t i = 0; i < keymap->رقم_کلید_aliases; i++) {
        ثابت xkb_atom_t alias = (substitutions == پوچ)
            ? keymap->کلید_aliases[i].alias
            : substitute_name(substitutions, keymap->کلید_aliases[i].alias);
        ثابت xkb_atom_t real = (substitutions == پوچ)
            ? keymap->کلید_aliases[i].real
            : substitute_name(substitutions, keymap->کلید_aliases[i].real);

        اگر (pretty)
            write_buf(buf, "\talias %-14s",
                      کلید_Name_Text(keymap->ctx, alias));
        وگرنه
            write_buf(buf, "\talias %s",
                      کلید_Name_Text(keymap->ctx, alias));
        write_buf(buf, " = %s;\n",
                  کلید_Name_Text(keymap->ctx, real));
    }

    copy_to_buf(buf, "};\n\n");
    بازگشت true;
}

ایستا bool
write_types(ثابت ساختار xkb_keymap *keymap, شمارش xkb_keymap_format format,
            شمارش xkb_keymap_serialize_flags flags, ساختار buf *buf)
{
    اگر (keymap->types_section_name)
        write_buf(buf, "xkb_types \"%s\" {\n",
                  keymap->types_section_name);
    وگرنه
        copy_to_buf(buf, "xkb_types {\n");

    /* NOTE: support for default values has been removed */

    اگر (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        بازگشت false;

    ثابت bool drop_unused = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    برای (darray_size_t i = 0; i < keymap->رقم_types; i++) {
        ثابت ساختار xkb_کلید_type * ثابت type = &keymap->types[i];
        اگر (!type->required && drop_unused)
            ادامه;

        copy_to_buf(buf, "\ttype ");
        write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, type->name));
        copy_to_buf(buf, " {\n");

        write_buf(buf, "\t\tmodifiers= %s;\n",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              type->mods.mods));

        برای (darray_size_t j = 0; j < type->رقم_entries; j++) {
            ثابت نویسه *str;
            ثابت ساختار xkb_کلید_type_entry *entry = &type->entries[j];

            /*
             * Printing level 1 entries is redundant, it's the default,
             * unless there's preserve info.
             */
            اگر (entry->level == 0 && entry->preserve.mods == 0 &&
                !(flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_مقادیر))
                ادامه;

            str = ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              entry->mods.mods);
            write_buf(buf, "\t\tmap[%s]= %"PRIu32";\n",
                      str, entry->level + 1);

            اگر (entry->preserve.mods)
                write_buf(buf, "\t\tpreserve[%s]= %s;\n",
                          str, ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                                           entry->preserve.mods));
        }

        برای (xkb_level_index_t n = 0; n < type->رقم_level_names; n++)
            اگر (type->level_names[n]) {
                write_buf(buf, "\t\tlevel_name[%"PRIu32"]= ", n + 1);
                write_buf_string_literal(
                    buf, xkb_atom_text(keymap->ctx, type->level_names[n]));
                copy_to_buf(buf, ";\n");
            }

        copy_to_buf(buf, "\t};\n");
    }

    copy_to_buf(buf, "};\n\n");
    بازگشت true;
}

ایستا bool
write_led_map(ثابت ساختار xkb_keymap *keymap, شمارش xkb_keymap_format format,
              bool defaults, ساختار buf *buf, ثابت ساختار xkb_led *led)
{
    copy_to_buf(buf, "\tindicator ");
    write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, led->name));
    copy_to_buf(buf, " {\n");

    اگر (led->which_groups) {
        اگر (led->which_groups != XKB_STATE_LAYOUT_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichGroupState= %s;\n",
                      LedStateMaskText(keymap->ctx, groupComponentMaskNames,
                                       led->which_groups));
        }
        write_buf(buf, "\t\tgroups= 0x%02"PRIx32";\n",
                  led->groups);
    }

    اگر (led->which_mods) {
        اگر (led->which_mods != XKB_STATE_MODS_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichModState= %s;\n",
                      LedStateMaskText(keymap->ctx, modComponentMaskNames,
                                       led->which_mods));
        }
        write_buf(buf, "\t\tmodifiers= %s;\n",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              led->mods.mods));
    }

    اگر (led->ctrls) {
        write_buf(buf, "\t\tcontrols= %s;\n",
                  ControlMaskText(keymap->ctx, format, led->ctrls));
    }

    copy_to_buf(buf, "\t};\n");
    بازگشت true;
}

ایستا ثابت نویسه *
affect_lock_text(شمارش xkb_action_flags flags, bool show_both)
{
    انتخاب (flags & (ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK)) {
    حالت 0:
        بازگشت show_both ? ",affect=both" : "";
    حالت ACTION_LOCK_NO_UNLOCK:
        بازگشت ",affect=lock";
    حالت ACTION_LOCK_NO_LOCK:
        بازگشت ",affect=unlock";
    حالت ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK:
        بازگشت ",affect=neither";
    پیشفرض:
        بازگشت "";
    }
}

#define نماد_PADDING 15
#define ACTION_PADDING 30

ایستا bool
write_action(ثابت ساختار xkb_keymap *keymap, شمارش xkb_keymap_format format,
             xkb_layout_index_t max_groups, ساختار buf *buf,
             ثابت اجتماع xkb_action *action,
             ثابت نویسه *prefix, ثابت نویسه *suffix)
{
    ثابت نویسه *type;
    ثابت نویسه *args = پوچ;

    اگر (!prefix)
        prefix = "";
    اگر (!suffix)
        suffix = "";

    type = ActionTypeText(action->type);

    انتخاب (action->type) {
    حالت ACTION_TYPE_MOD_SET:
    حالت ACTION_TYPE_MOD_LATCH:
    حالت ACTION_TYPE_MOD_LOCK:
        اگر (action->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            args = "modMapMods";
        وگرنه
            args = ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                               action->mods.mods.mods);
        bool unlockOnPress = (action->mods.flags & ACTION_UNLOCK_ON_PRESS);
        اگر (unlockOnPress && !isModsUnLockOnPressSupported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot use \"%s(unlockOnPress=true)\" in keymap format %d\n",
                    ActionTypeText(action->type), format);
            unlockOnPress = false;
        }
        bool latchOnPress = action->type == ACTION_TYPE_MOD_LATCH &&
                            (action->group.flags & ACTION_LATCH_ON_PRESS);
        اگر (latchOnPress && !isModsLatchOnPressSupported(format)) {
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
        بشکن;

    حالت ACTION_TYPE_GROUP_SET:
    حالت ACTION_TYPE_GROUP_LATCH:
    حالت ACTION_TYPE_GROUP_LOCK:
        اگر ((uint32_t) xkb_abs(action->group.group) <
            (max_groups + !(action->group.flags & ACTION_ABSOLUTE_SWITCH))) {
            bool lockOnRelease = action->type == ACTION_TYPE_GROUP_LOCK &&
                                 (action->group.flags & ACTION_LOCK_ON_RELEASE);
            اگر (lockOnRelease && !isGroupLockOnReleaseSupported(format)) {
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
        } وگرنه {
            /* Unsupported group index: degrade to VoidAction() */
            log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported group index %"PRIu32" for action %s()\n",
                      (action->group.group +
                       !!(action->group.flags & ACTION_ABSOLUTE_SWITCH)),
                     type);
            برو void_action;
        }
        بشکن;

    حالت ACTION_TYPE_TERMINATE:
        write_buf(buf, "%s%s()%s", prefix, type, suffix);
        بشکن;

    حالت ACTION_TYPE_PTR_MOVE:
        write_buf(buf, "%s%s(x=%s%d,y=%s%d%s)%s", prefix, type,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_X) && action->ptr.x >= 0) ? "+" : "",
                  action->ptr.x,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_Y) && action->ptr.y >= 0) ? "+" : "",
                  action->ptr.y,
                  (action->ptr.flags & ACTION_REPEAT) ? "" : ",!repeat",
                  suffix);
        بشکن;

    حالت ACTION_TYPE_PTR_LOCK:
        args = affect_lock_text(action->btn.flags, true);
        /* fallthrough */
    حالت ACTION_TYPE_PTR_BUTTON:
        write_buf(buf, "%s%s(button=", prefix, type);
        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        اگر (action->btn.button > XKB_POINTER_BUTTON_DEFAULT &&
            action->btn.button <= XKB_POINTER_BUTTON_MAX)
            write_buf(buf, "%"PRIu8, action->btn.button);
        وگرنه
            copy_to_buf(buf, "default");
        اگر (action->btn.شمار) {
            أكد(action->type != ACTION_TYPE_PTR_LOCK);
            write_buf(buf, ",count=%u", action->btn.شمار);
        }
        اگر (args)
            write_buf(buf, "%s", args);
        write_buf(buf, ")%s", suffix);
        بشکن;

    حالت ACTION_TYPE_PTR_DEFAULT:
        write_buf(buf, "%s%s(", prefix, type);
        write_buf(buf, "affect=button,button=%s%"PRId8,
                  (!(action->dflt.flags & ACTION_ABSOLUTE_SWITCH) && action->dflt.مقدار >= 0) ? "+" : "",
                  action->dflt.مقدار);
        write_buf(buf, ")%s", suffix);
        بشکن;

    حالت ACTION_TYPE_SWITCH_VT:
        write_buf(buf, "%s%s(screen=%s%d,%ssame)%s", prefix, type,
                  (!(action->screen.flags & ACTION_ABSOLUTE_SWITCH) && action->screen.screen >= 0) ? "+" : "",
                  action->screen.screen,
                  (action->screen.flags & ACTION_SAME_SCREEN) ? "" : "!",
                  suffix);
        بشکن;

    حالت ACTION_TYPE_CTRL_SET:
    حالت ACTION_TYPE_CTRL_LOCK:
        write_buf(buf, "%s%s(controls=%s%s)%s", prefix, type,
                  ControlMaskText(keymap->ctx, format, action->ctrls.ctrls),
                  (action->type == ACTION_TYPE_CTRL_LOCK
                    ? affect_lock_text(action->ctrls.flags, false)
                    : ""),
                  suffix);
        بشکن;

    حالت ACTION_TYPE_REDIRECT_کلید: {
        write_buf(buf, "%s%s(", prefix, type);
        ثابت ساختار xkb_کلید * ثابت کلید = Xkb_کلید(keymap, action->redirect.keycode);
        /* Can fail if the keycode was not initialized */
        اگر (کلید)
            write_buf(buf, "keycode=%s", کلید_Name_Text(keymap->ctx, کلید->name));
        اگر (action->redirect.affect_mods) {
            xkb_mod_mask_t mask;
            mask = (action->redirect.affect_mods & action->redirect.mods);
            اگر (mask)
                write_buf(buf, ",modifiers=%s",
                          ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, mask));
            mask = (action->redirect.affect_mods & ~action->redirect.mods);
            اگر (mask)
                write_buf(buf, ",clearMods=%s",
                          ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, mask));
        }
        write_buf(buf, ")%s", suffix);
        بشکن;
    }

    حالت ACTION_TYPE_UNKNOWN:
    حالت ACTION_TYPE_NONE:
        write_buf(buf, "%sNoAction()%s", prefix, suffix);
        بشکن;

    حالت ACTION_TYPE_VOID:
void_action:
        /*
         * VoidAction() is an xkbcommon extension.
         * Use LockControls as a backward-compatible fallback.
         * We cannot serialize it to `NoAction()`, as it would be dropped in
         * e.g. the context of multiple actions.
         * We better not use `Private` either, because it could still be
         * interpreted by X11.
         */
        اگر (format == XKB_KEYMAP_FORMAT_TEXT_V1)
            write_buf(buf, "%sLockControls(controls=none,affect=neither)%s",
                      prefix, suffix);
        وگرنه
            write_buf(buf, "%sVoidAction()%s", prefix, suffix);
        بشکن;

    پیشفرض:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_رقم_ENTRIES,
                      "Missing action type");
        /* Unsupported legacy actions should have degraded to NoAction */
        أكد(action->type != ACTION_TYPE_UNSUPPORTED_LEGACY);
        write_buf(buf,
                  "%s%s(type=0x%02x,data[0]=0x%02x,data[1]=0x%02x,data[2]=0x%02x,data[3]=0x%02x,data[4]=0x%02x,data[5]=0x%02x,data[6]=0x%02x)%s",
                  prefix, type, action->type, action->priv.data[0],
                  action->priv.data[1], action->priv.data[2],
                  action->priv.data[3], action->priv.data[4],
                  action->priv.data[5], action->priv.data[6],
                  suffix);
        بشکن;
    }

    بازگشت true;
}

ایستا ثابت اجتماع xkb_action void_actions[] = {
    { .type = ACTION_TYPE_VOID }
};

ایستا bool
write_actions(ثابت ساختار xkb_keymap *keymap, شمارش xkb_keymap_format format,
              xkb_layout_index_t max_groups, ساختار buf *buf, ساختار buf *buf2,
              ثابت ساختار xkb_کلید *کلید, xkb_layout_index_t group)
{
    ایستا ثابت اجتماع xkb_action noAction = { .type = ACTION_TYPE_NONE };

    برای (xkb_level_index_t level = 0; level < Xkb_کلید_رقم_Levels(کلید, group);
         level++) {

        اگر (level != 0)
            copy_to_buf(buf, ", ");

        ثابت اجتماع xkb_action *actions = پوچ;
        xkb_action_شمار_t شمار = xkb_keymap_کلید_get_actions_by_level(
            keymap, کلید, group, level, &actions
        );

        اگر (شمار > 1 && format < XKB_KEYMAP_FORMAT_TEXT_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            actions = void_actions;
            شمار = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        buf2->size = 0;
        اگر (شمار == 0) {
            اگر (!write_action(keymap, format, max_groups,
                              buf2, &noAction, پوچ, پوچ))
                بازگشت false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        وگرنه اگر (شمار == 1) {
            اگر (!write_action(keymap, format, max_groups,
                              buf2, &(actions[0]), پوچ, پوچ))
                بازگشت false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        وگرنه {
            copy_to_buf(buf2, "{ ");
            برای (xkb_action_شمار_t k = 0; k < شمار; k++) {
                اگر (k != 0)
                    copy_to_buf(buf2, ", ");
                size_t old_size = buf2->size;
                اگر (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), پوچ, پوچ))
                    بازگشت false;
                /* Check if padding is necessary */
                اگر (buf2->size >= old_size + ACTION_PADDING)
                    ادامه;
                /* Compute and write padding, then write the action again */
                ثابت صحیح padding = (صحیح)(old_size + ACTION_PADDING - buf2->size);
                buf2->size = old_size;
                write_buf(buf2, "%*s", padding, "");
                اگر (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), پوچ, پوچ))
                    بازگشت false;
            }
            copy_to_buf(buf2, " }");
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
    }

    بازگشت true;
}

ایستا bool
write_action_defaults(ثابت ساختار xkb_keymap *keymap,
                      شمارش xkb_keymap_format format,
                      ثابت اجتماع xkb_action *action,
                      ساختار buf *buf)
{
    ثابت نویسه *type = ActionTypeText(action->type);

#define PREFIX "\t"

    انتخاب (action->type) {
    حالت ACTION_TYPE_MOD_SET:
    حالت ACTION_TYPE_MOD_LATCH:
    حالت ACTION_TYPE_MOD_LOCK:
        أكد(action->mods.flags == 0);
        أكد(action->mods.mods.mods == 0);
        write_buf(buf, PREFIX"%s.modifiers = 0;\n", type);
        اگر (action->type != ACTION_TYPE_MOD_LOCK)
            write_buf(buf, PREFIX"%s.clearLocks = false;\n", type);
        اگر (action->type == ACTION_TYPE_MOD_LATCH &&
            isModsLatchOnPressSupported(format))
            write_buf(buf, PREFIX"%s.latchOnPress = false;\n", type);
        اگر (action->type == ACTION_TYPE_MOD_LATCH)
            write_buf(buf, PREFIX"%s.latchToLock = false;\n", type);
        اگر (action->type == ACTION_TYPE_MOD_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        اگر (isModsUnLockOnPressSupported(format))
            write_buf(buf, PREFIX"%s.unlockOnPress = false;\n", type);
        بشکن;

    حالت ACTION_TYPE_GROUP_SET:
    حالت ACTION_TYPE_GROUP_LATCH:
    حالت ACTION_TYPE_GROUP_LOCK:
        أكد(action->group.flags == 0);
        أكد(action->group.group == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.group = +0;\n", type);
        اگر (action->type != ACTION_TYPE_GROUP_LOCK)
            write_buf(buf, PREFIX"%s.clearLocks = false;\n", type);
        اگر (action->type == ACTION_TYPE_GROUP_LATCH)
            write_buf(buf, PREFIX"%s.latchToLock = false;\n", type);
        اگر (action->type == ACTION_TYPE_GROUP_LOCK &&
            isGroupLockOnReleaseSupported(format))
            write_buf(buf, PREFIX"%s.lockOnRelease = false;\n", type);
        بشکن;


    حالت ACTION_TYPE_PTR_MOVE:
        أكد(action->ptr.x == 0);
        أكد(action->ptr.y == 0);
        أكد(action->ptr.flags == ACTION_REPEAT);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.x = +0;\n", type);
        write_buf(buf, PREFIX"%s.y = +0;\n", type);
        write_buf(buf, PREFIX"%s.accel = true;\n", type);
        بشکن;

    حالت ACTION_TYPE_PTR_LOCK:
    حالت ACTION_TYPE_PTR_BUTTON:
        أكد(action->btn.flags == 0);
        أكد(action->btn.button == 0);
        أكد(action->btn.شمار == 0);
        write_buf(buf, PREFIX"%s.button = 0;\n", type);
        write_buf(buf, PREFIX"%s.count = 0;\n", type);
        اگر (action->type == ACTION_TYPE_PTR_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        بشکن;

    حالت ACTION_TYPE_PTR_DEFAULT:
        أكد(action->dflt.flags == 0);
        أكد(action->dflt.مقدار == 1);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.button = +1;\n", type);
        بشکن;

    حالت ACTION_TYPE_SWITCH_VT:
        أكد(action->screen.flags == ACTION_SAME_SCREEN);
        أكد(action->screen.screen == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.screen = +0;\n", type);
        write_buf(buf, PREFIX"%s.same = true;\n", type);
        بشکن;

    حالت ACTION_TYPE_CTRL_SET:
    حالت ACTION_TYPE_CTRL_LOCK:
        أكد(action->ctrls.flags == 0);
        أكد(action->ctrls.ctrls == 0);
        write_buf(buf, PREFIX"%s.controls = 0;\n", type);
        اگر (action->type == ACTION_TYPE_CTRL_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        بشکن;

    حالت ACTION_TYPE_REDIRECT_کلید:
        أكد(action->redirect.keycode == keymap->redirect_کلید_auto);
        أكد(action->redirect.affect_mods == 0);
        أكد(action->redirect.mods == 0);
        write_buf(buf, PREFIX"%s.keycode = auto;\n", type);
        write_buf(buf, PREFIX"%s.modifiers = 0;\n", type);
        write_buf(buf, PREFIX"%s.clearMods = 0;\n", type);
        بشکن;

    پیشفرض:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_رقم_ENTRIES,
                      "Missing action type");
    }

#undef PREFIX

    بازگشت true;
}

ایستا bool
write_actions_defaults(ثابت ساختار xkb_keymap *keymap,
                       شمارش xkb_keymap_format format, ساختار buf *buf)
{
    ActionsInfo defaults = {0};
    InitActionsInfo(keymap, &defaults);

    برای (uint8_t a = 0; a < (uint8_t) ACTION_TYPE_رقم_ENTRIES; a++) {
        ثابت اجتماع xkb_action * restrict ثابت action = &defaults.actions[a];
        اگر (!write_action_defaults(keymap, format, action, buf))
            بازگشت false;
    }
    copy_to_buf(buf, "\n");

    بازگشت true;
}

ایستا ثابت ساختار xkb_sym_interpret fallback_interpret = {
    .sym = XKB_کلید_Void_نماد,
    .required = true,
    .repeat = FALLBACK_INTERPRET_کلید_REPEAT,
    .match = MATCH_ANY_OR_NONE,
    .mods = 0,
    .virtual_mod = (xkb_mod_index_t) DEFAULT_INTERPRET_VMOD,
    .رقم_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

ایستا bool
write_compat(ثابت ساختار xkb_keymap * restrict keymap,
             شمارش xkb_keymap_format format, xkb_layout_index_t max_groups,
             شمارش xkb_keymap_serialize_flags flags, bool * restrict some_interp,
             ساختار buf *buf)
{
    ثابت bool pretty = (flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ثابت bool defaults = (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_مقادیر);
    ثابت bool drop_unused = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    ثابت bool drop_interprets =
        ((flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_کلید_مقادیر) && drop_unused);
    /* TODO: print all default values if the explicit flag is set */

    اگر (keymap->compat_section_name)
        write_buf(buf, "xkb_compatibility \"%s\" {\n",
                  keymap->compat_section_name);
    وگرنه
        copy_to_buf(buf, "xkb_compatibility {\n");

    اگر (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        بازگشت false;

    اگر (defaults && !write_actions_defaults(keymap, format, buf))
        بازگشت false;

    copy_to_buf(buf, "\tinterpret.useModMapMods= AnyLevel;\n");
    copy_to_buf(buf, "\tinterpret.repeat= False;\n");

    /* xkbcomp requires at least one interpret entry. */
    ثابت bool use_fallback_interpret =
        (!keymap->رقم_sym_interprets || drop_interprets);
    ثابت darray_size_t رقم_sym_interprets =
        (use_fallback_interpret ? 1 : keymap->رقم_sym_interprets);
    ثابت ساختار xkb_sym_interpret* ثابت sym_interprets =
        (use_fallback_interpret ? &fallback_interpret : keymap->sym_interprets);

    برای (darray_size_t i = 0; i < رقم_sym_interprets; i++) {
        ثابت ساختار xkb_sym_interpret *si = &sym_interprets[i];
        اگر (!si->required && drop_unused)
            ادامه;

        *some_interp = true;

        copy_to_buf(buf, "\tinterpret ");
        اگر (!si->sym) {
            copy_to_buf(buf, "Any");
        } وگرنه اگر (pretty) {
            write_buf(buf, "%s", KeysymText(keymap->ctx, si->sym));
        } وگرنه {
            write_buf(buf, "0x%"PRIx32, si->sym);
        }
        write_buf(buf, "+%s(%s) {",
                  SIMatchText(si->match),
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, si->mods));

        bool has_explicit_properties = false;

        اگر (si->virtual_mod != XKB_MOD_INVALID) {
            write_buf(buf, "\n\t\tvirtualModifier= %s;",
                      ModIndexText(keymap->ctx, &keymap->mods,
                                   si->virtual_mod));
            has_explicit_properties = true;
        }

        اگر (si->level_one_only) {
            copy_to_buf(buf, "\n\t\tuseModMapMods=level1;");
            has_explicit_properties = true;
        } وگرنه اگر (defaults) {
            copy_to_buf(buf, "\n\t\tuseModMapMods=AnyLevel;");
            has_explicit_properties = true;
        }

        اگر (si->repeat) {
            copy_to_buf(buf, "\n\t\trepeat= True;");
            has_explicit_properties = true;
        } وگرنه اگر (defaults) {
            copy_to_buf(buf, "\n\t\trepeat= False;");
            has_explicit_properties = true;
        }

        ثابت اجتماع xkb_action *action = پوچ;
        xkb_action_شمار_t action_شمار = si->رقم_actions;
        اگر (action_شمار <= 1) {
            action = &si->a.action;
        } وگرنه اگر (format < XKB_KEYMAP_FORMAT_TEXT_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            action = &void_actions[0];
            action_شمار = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        اگر (action_شمار > 1) {
            copy_to_buf(buf, "\n\t\taction= {");
            ثابت نویسه suffix[] = ", ";
            برای (xkb_action_شمار_t k = 0; k < si->رقم_actions; k++) {
                اگر (!write_action(keymap, format, max_groups,
                                  buf, &si->a.actions[k], "", suffix))
                    بازگشت false;
            }
            buf->size -= اندازه(suffix) - 1; /* trailing comma */
            copy_to_buf(buf, "};");
            has_explicit_properties = true;
        } وگرنه اگر (action_شمار == 1) {
            اگر (!write_action(keymap, format, max_groups, buf, action,
                              "\n\t\taction= ", ";"))
                بازگشت false;
            has_explicit_properties = true;
        }
        write_buf(buf, (has_explicit_properties
                        ? "\n\t};\n"
                        /* Empty interpret is a syntax error in xkbcomp, so
                         * use a dummy entry */
                        : "\n\t\taction= NoAction();\n\t};\n"));
    }

    اگر (use_fallback_interpret || (
        /* Detect previous interpret fallback */
        رقم_sym_interprets == 1 && *some_interp &&
        sym_interprets[0].sym == fallback_interpret.sym &&
        sym_interprets[0].match == fallback_interpret.match &&
        sym_interprets[0].mods == fallback_interpret.mods &&
        sym_interprets[0].repeat == fallback_interpret.repeat &&
        sym_interprets[0].virtual_mod == fallback_interpret.virtual_mod &&
        sym_interprets[0].level_one_only == fallback_interpret.level_one_only &&
        sym_interprets[0].رقم_actions == fallback_interpret.رقم_actions
        /* `required` field is not compared */
    ))
        *some_interp = false;

    ثابت ساختار xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        اگر ((led->which_groups || led->groups ||
             led->which_mods || led->mods.mods || led->ctrls) &&
            !write_led_map(keymap, format, defaults, buf, led))
        {
            بازگشت false;
        }
    }

    copy_to_buf(buf, "};\n\n");

    بازگشت true;
}

ایستا bool
write_keysyms(ثابت ساختار xkb_keymap *keymap,
              ساختار buf *buf, ساختار buf *buf2,
              ثابت ساختار xkb_کلید *کلید, xkb_layout_index_t group,
              bool pretty, bool show_actions)
{
    ثابت unsigned صحیح padding = (pretty)
        ? ((show_actions) ? ACTION_PADDING : نماد_PADDING)
        : 0;
    برای (xkb_level_index_t level = 0; level < Xkb_کلید_رقم_Levels(کلید, group);
         level++) {
        ثابت xkb_keysym_t *syms;
        صحیح رقم_syms;

        اگر (level != 0)
            copy_to_buf(buf, ", ");

        رقم_syms = xkb_keymap_کلید_get_syms_by_level(
            /* remove `const` constraint to use the dated public API */
            (ساختار xkb_keymap *)keymap,
            کلید->keycode, group, level, &syms
        );

        ثابت xkb_keysym_t no_نماد = XKB_کلید_No_نماد;
        اگر (رقم_syms == 0) {
            رقم_syms = 1;
            syms = &no_نماد;
        }

        /*
         * NOTE: Use `NoSymbol` even without pretty output, for compatibility
         * with xkbcomp and xkbcommon < 1.12
         */

        اگر (رقم_syms == 1) {
            اگر (pretty || syms[0] == XKB_کلید_No_نماد)
                write_buf(buf, "%*s", padding, KeysymText(keymap->ctx, syms[0]));
            وگرنه
                write_buf(buf, "0x%"PRIx32, syms[0]);
        } وگرنه {
            اگر (pretty) {
                buf2->size = 0;
                copy_to_buf(buf2, "{ ");
                برای (صحیح s = 0; s < رقم_syms; s++) {
                    اگر (s != 0)
                        copy_to_buf(buf2, ", ");
                    write_buf(buf2, "%*s", (show_actions ? padding : 0),
                              KeysymText(keymap->ctx, syms[s]));
                }
                copy_to_buf(buf2, " }");
                write_buf(buf, "%*s", padding, buf2->buf);
            } وگرنه {
                copy_to_buf(buf, "{");
                برای (صحیح s = 0; s < رقم_syms; s++) {
                    اگر (s != 0)
                        copy_to_buf(buf, ",");
                    اگر (syms[s] == XKB_کلید_No_نماد)
                        copy_to_buf(buf, "NoSymbol");
                    وگرنه
                        write_buf(buf, "0x%"PRIx32, syms[s]);
                }
                copy_to_buf(buf, "}");
            }
        }
    }

    بازگشت true;
}

ایستا bool
write_کلید(ثابت ساختار xkb_keymap *keymap,
          ثابت ساختار xkb_keymap_serialize_config *config,
          ثابت کلید_name_substitutions *substitutions,
          xkb_layout_index_t max_groups, bool some_interprets,
          bool drop_interprets, bool explicit, bool pretty,
          ساختار buf *buf, ساختار buf *buf2, ثابت ساختار xkb_کلید *کلید)
{
    bool simple = true;

    xkb_layout_mask_t layouts = config->layouts;
    اگر (popcount32(layouts) != keymap->رقم_groups) {
        /* Subset of layouts: use out-of-range key policy */
        xkb_layout_index_t l = 0;
        برای (xkb_layout_mask_t ls = layouts; ls; ls >>= 1, l++) {
            اگر (ls & 0x1) {
                /* Replace target layout with its effective layout, if valid */
                layouts &= ~(UINT32_C(1) << l);
                ثابت xkb_layout_index_t effective =
                    xkb_keymap_کلید_effective_layout(کلید, l);
                اگر (effective != XKB_LAYOUT_INVALID)
                    layouts |= (UINT32_C(1) << effective);
            }
        }
    } وگرنه {
        layouts &= ((UINT64_C(1) << کلید->رقم_groups) - 1);
    }
    ثابت xkb_layout_index_t groups_شمار = popcount32(layouts);

    ثابت xkb_atom_t name = (substitutions == پوچ)
        ? کلید->name
        : substitute_name(substitutions, کلید->name);

    اگر (pretty)
        write_buf(buf, "\tkey %-20s {", کلید_Name_Text(keymap->ctx, name));
    وگرنه
        write_buf(buf, "\tkey %s {", کلید_Name_Text(keymap->ctx, name));

    اگر ((کلید->explicit & EXPLICIT_TYPES) || explicit) {
        simple = false;

        bool multi_type = explicit;
        /* Check for distinct key types only if not requiring explicit values */
        اگر (!explicit) {
            برای (xkb_layout_index_t group = 1; group < کلید->رقم_groups; group++) {
                اگر (!((UINT32_C(1) << group) & layouts))
                    ادامه;

                اگر (کلید->groups[group].type != کلید->groups[0].type) {
                    multi_type = true;
                    بشکن;
                }
            }
        }

        اگر (multi_type) {
            xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
            برای (xkb_layout_index_t group = 0; group < کلید->رقم_groups; group++) {
                اگر (!((UINT32_C(1) << group) & layouts))
                    ادامه;

                new_group++;

                اگر (!کلید->groups[group].explicit_type && !explicit)
                    ادامه;

                ثابت ساختار xkb_کلید_type * ثابت type = کلید->groups[group].type;
                write_buf(buf, "\n\t\ttype[%"PRIu32"]= ", new_group + 1);
                write_buf_string_literal(
                  buf, xkb_atom_text(keymap->ctx, type->name));
                copy_to_buf(buf, ",");
            }
        }
        وگرنه {
            ثابت ساختار xkb_کلید_type * ثابت type = کلید->groups[0].type;
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
    ثابت bool explicit_actions = (کلید->explicit & EXPLICIT_INTERP);

    /* Check if an explicit value is required */
    #define require_explicit(کلید, prop,                                       \
                             مقدار, initial_مقدار, default_interpret_مقدار) ( \
        /* Explicit value in the source */                                    \
        ((کلید)->explicit & (prop)) ||                                         \
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
                ((کلید)->implicit_actions && !some_interprets)                 \
            ) && (                                                            \
                /* Non-initial value */                                       \
                (مقدار) != (initial_مقدار) ||                                 \
                /* Force initial value for clarity */                         \
                (initial_مقدار) != (default_interpret_مقدار)                  \
            )                                                                 \
        )                                                                     \
    )

    اگر (require_explicit(کلید, EXPLICIT_REPEAT, کلید->repeats,
                         DEFAULT_کلید_REPEAT, DEFAULT_INTERPRET_کلید_REPEAT)) {
        اگر (کلید->repeats)
            copy_to_buf(buf, "\n\t\trepeat= Yes,");
        وگرنه
            copy_to_buf(buf, "\n\t\trepeat= No,");
        simple = false;
    }

    اگر (require_explicit(کلید, EXPLICIT_VMODMAP, کلید->vmodmap,
                         (xkb_mod_mask_t) DEFAULT_کلید_VMODMAP,
                         (xkb_mod_mask_t) DEFAULT_INTERPRET_VMODMAP)) {
        write_buf(buf, "\n\t\tvirtualMods= %s,",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              کلید->vmodmap));
        simple = false;
    }

    #undef require_explicit

    انتخاب (کلید->out_of_range_group_policy) {
    حالت XKB_LAYOUT_OUT_OF_RANGE_CLAMP:
        copy_to_buf(buf, "\n\t\tgroupsClamp,");
        simple = false;
        بشکن;

    حالت XKB_LAYOUT_OUT_OF_RANGE_REDIRECT: {
        اگر (!groups_شمار) {
            /* No layout to redirect to: skip */
            بشکن;
        }

        xkb_layout_mask_t layout_mask =
            (UINT32_C(1) << کلید->out_of_range_group_عدد);
        اگر (!(layouts & layout_mask)) {
            /*
             * Layout is not included: default to first layout
             * See: XkbWrapGroupIntoRange
             */
            log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "key %s: groupsRedirect=%"PRIu32" converted to "
                     "groupsRedirect=1\n",
                     کلید_Name_Text(keymap->ctx, name),
                     کلید->out_of_range_group_عدد + 1);
            layout_mask = 0x1;
        }

        /* In case some lower layouts were discarded, update the group index */
        ثابت xkb_layout_mask_t کم = (layouts & (layout_mask - 1));
        ثابت xkb_layout_index_t new = popcount32(کم);
        write_buf(buf, "\n\t\tgroupsRedirect= %"PRIu32",", new + 1);
        simple = false;
        بشکن;
    }

    حالت XKB_LAYOUT_OUT_OF_RANGE_WRAP:
    پیشفرض:
        بشکن;
    }

    اگر (کلید->overlays) {
        xkb_overlay_mask_t remaining = کلید->overlays;

        ثابت xkb_overlay_index_t overlay_max =
            format_max_overlays(config->format);
        static_assert(XKB_OVERLAY_شمار == 8, "invalid right shift");
        ثابت xkb_overlay_mask_t valid =
            (xkb_overlay_mask_t)((1u << overlay_max) - 1u);
        remaining &= valid;
        اگر (remaining != کلید->overlays) {
            log_warn(keymap->ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                     "Overlays indices > %u in %s require using "
                     "keymap format >= v2; "
                     "keep overlays 0x%08x and discard overlays 0x%08x\n",
                     overlay_max, کلید_Name_Text(keymap->ctx, name),
                     remaining, کلید->overlays & ~valid);
        }

        اگر (remaining && !کلید->overlays_inline &&
            !areOverlappingOverlaysSupported(config->format)) {
            /* isolate lowest set bit */
            ثابت xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                remaining &
                (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
            );
            /* get its index */
            ثابت xkb_overlay_index_t overlay =
                (xkb_overlay_index_t)popcount32(lsb - 1u);
            log_warn(keymap->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                     "Overlapping overlays in %s require using "
                     "keymap format >= v2; "
                     "keep overlay %u and discard overlays 0x%x\n",
                     کلید_Name_Text(keymap->ctx, name), overlay, remaining & ~lsb);
            remaining = lsb;
        }

        اگر (remaining)
            simple = false;

        ثابت ساختار xkb_کلید * ثابت *overlays_کلیدها = (کلید->overlays_inline)
                                                    ? &کلید->overlay_کلید
                                                    : کلید->overlays_کلیدها;
        وقتی (remaining) {
            /* isolate lowest set bit */
            ثابت xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                remaining &
                (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
            );
            /* get its index */
            ثابت xkb_overlay_index_t overlay =
                (xkb_overlay_index_t)popcount32(lsb - 1u);
            remaining = (remaining & ~lsb);

            /* pop current value */
            اگر (کلید->overlays_inline && remaining)
                PANIC_UNREACHABLE();
            ثابت ساختار xkb_کلید *overlay_کلید = *(overlays_کلیدها++);

            ثابت xkb_atom_t overlay_کلید_name = (substitutions == پوچ)
                ? overlay_کلید->name
                : substitute_name(substitutions, overlay_کلید->name);
            write_buf(buf, "\n\t\toverlay%u= %s,",
                      overlay + 1, کلید_Name_Text(keymap->ctx, overlay_کلید_name));
        }
    }

    اگر (groups_شمار > 1 || explicit_actions || explicit)
        simple = false;

    اگر (simple) {
        اگر (groups_شمار == 0) {
            /* Remove trailing comma */
            delete_last_char(buf, ',');
            اگر (buf->buf[buf->size - 1] == ',')
                buf->size--;
        } وگرنه {
            copy_to_buf(buf, "\t[ ");
            /* Get the single group index */
            xkb_mod_index_t group = ctz32(layouts);
            /* The group may not be defined for this key if it is not the first one */
            group = xkb_keymap_کلید_effective_layout(کلید, group);
            اگر (!write_keysyms(keymap, buf, buf2, کلید, group, pretty, false))
                بازگشت false;
            copy_to_buf(buf, " ]");
        }
        copy_to_buf(buf, " };\n");
    }
    وگرنه {
        xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
        برای (xkb_layout_index_t group = 0; group < کلید->رقم_groups; group++) {
            اگر (!((UINT32_C(1) << group) & layouts))
                ادامه;

            new_group++;

            ثابت bool print_actions =
                /* Group has explicit actions */
                کلید->groups[group].explicit_actions ||
                /* Explicit values are required */
                explicit ||
                /* Group has symbols but no explicit actions and key has explicit
                 * actions in another group: ensure compatibility with xkbcomp
                 * and all xkbcommon versions */
                (کلید->groups[group].explicit_symbols &&
                 explicit_actions && some_interprets) ||
                /* Group has implicit actions but no interpret can set them, so
                 * the actions must be made explicit */
                (کلید->groups[group].implicit_actions && !some_interprets);

            اگر (new_group != 0)
                copy_to_buf(buf, ",");
            write_buf(buf, "\n\t\tsymbols[%"PRIu32"]= [ ", new_group + 1);

            اگر (!write_keysyms(keymap, buf, buf2, کلید, group,
                               pretty, print_actions))
                بازگشت false;
            copy_to_buf(buf, " ]");
            اگر (print_actions) {
                write_buf(buf, ",\n\t\tactions[%"PRIu32"]= [ ", new_group + 1);
                اگر (!write_actions(keymap, config->format, max_groups,
                                   buf, buf2, کلید, group))
                    بازگشت false;
                copy_to_buf(buf, " ]");
            }
        }
        اگر (!groups_شمار)
            delete_last_char(buf, ',');
        copy_to_buf(buf, "\n\t};\n");
    }

    بازگشت true;
}


ایستا صحیح
cmp_modmap_step_1(ثابت تهی *a, ثابت تهی *b)
{
    ثابت ModMapEntry *m1 = (ثابت ModMapEntry *)a;
    ثابت ModMapEntry *m2 = (ثابت ModMapEntry *)b;

    اگر (!m1->mods) {
        اگر (!m2->mods) {
            /* Entry will be dropped: do not care about the other fields */
            بازگشت 0;
        }
        بازگشت +1;
    } وگرنه اگر (!m2->mods) {
        بازگشت -1;
    }

    /* Valid only on step 1 */
    اگر (m1->کلید_Code < m2->کلید_Code)
        بازگشت -1;
    وگرنه اگر (m1->کلید_Code > m2->کلید_Code)
        بازگشت +1;

    اگر (m1->mods < m2->mods)
        بازگشت -1;
    وگرنه اگر (m1->mods > m2->mods)
        بازگشت +1;

    اگر (m1->have_نماد < m2->have_نماد)
        بازگشت +1;
    وگرنه اگر (m1->have_نماد > m2->have_نماد)
        بازگشت -1;

    اگر (m1->have_نماد) {
        اگر (m1->u.کلید_Sym < m2->u.کلید_Sym)
            بازگشت -1;
        اگر (m1->u.کلید_Sym > m2->u.کلید_Sym)
            بازگشت +1;
    } وگرنه {
        اگر (m1->u.کلید_Name < m2->u.کلید_Name)
            بازگشت -1;
        اگر (m1->u.کلید_Name > m2->u.کلید_Name)
            بازگشت +1;
    }

    بازگشت 0;
}

ایستا صحیح
cmp_modmap_step_2(ثابت تهی *a, ثابت تهی *b)
{
    ثابت ModMapEntry *m1 = (ثابت ModMapEntry *)a;
    ثابت ModMapEntry *m2 = (ثابت ModMapEntry *)b;

    اگر (m1->mods < m2->mods)
        بازگشت -1;
    اگر (m1->mods > m2->mods)
        بازگشت +1;

    اگر (m1->have_نماد < m2->have_نماد)
        بازگشت -1;
    اگر (m1->have_نماد > m2->have_نماد)
        بازگشت +1;

    اگر (m1->have_نماد) {
        اگر (m1->u.کلید_Sym < m2->u.کلید_Sym)
            بازگشت -1;
        اگر (m1->u.کلید_Sym > m2->u.کلید_Sym)
            بازگشت +1;
    } وگرنه {
        اگر (m1->کلید_Code < m2->کلید_Code)
            بازگشت -1;
        اگر (m1->کلید_Code > m2->کلید_Code)
            بازگشت +1;
    }

    بازگشت 0;
}

ایستا bool
write_modmaps(ثابت ساختار xkb_keymap *keymap, شمارش xkb_keymap_format format,
              ثابت کلید_name_substitutions *substitutions,
              bool pretty, bool drop_interprets, ساختار buf *buf)
{
    اگر (!keymap->رقم_modmaps)
        بازگشت true;

    darray(ModMapEntry) modmaps = darray_new();
    darray_from_اقلام(modmaps, keymap->modmaps, keymap->رقم_modmaps);
    اگر (!darray_اقلام(modmaps))
        بازگشت false;

    /* Sort entries by key code */
    qsort(darray_اقلام(modmaps), darray_size(modmaps),
          اندازه(*darray_اقلام(modmaps)), &cmp_modmap_step_1);

    /* Drop disabled entries at the end */
    برای (darray_size_t m = darray_size(modmaps); m-- > 0;) {
        اگر (darray_item(modmaps, m).mods)
            بشکن;
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
    برای (darray_size_t m = darray_size(modmaps); m-- > 0;) {
        ModMapEntry *mm = &darray_item(modmaps, m);
        أكد(mm->mods);

        اگر (mm->کلید_Code == current_keycode) {
            /*
             * Keycode already processed: either drop next key code entries
             * or convert them to keymap format v1
             */
            ثابت xkb_mod_mask_t core_mods = (mm->mods & MOD_REAL_MASK_ALL);
            اگر (convert_to_v1_compatibility_entry && core_mods) {
                /*
                 * Convert to keymap format V1-specific entry:
                 * keep only the highest core modifier
                 */
                mm->mods = UINT32_C(1) << (XKB_MAX_MODS - 1 - clz32(core_mods));
            } وگرنه {
                /*
                 * Drop if there is a previous entry for all keymap formats or
                 * if the current entry is incompatible with V1 keymap format.
                 */
                mm->mods = 0;
            }
            ادامه;
        }

        current_keycode = mm->کلید_Code;
        ثابت ساختار xkb_کلید *کلید = Xkb_کلید(keymap, mm->کلید_Code);

        ثابت xkb_mod_mask_t core_mods = (کلید->modmap & MOD_REAL_MASK_ALL);
        اگر (format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
            ((کلید->modmap & (کلید->modmap - 1)) || کلید->modmap != core_mods)) {
            /* V1 entry with multiple modifiers or non X11 core modifiers */
            m++; /* process in next iteration */
            convert_to_v1_compatibility_entry = true;
            log_err(keymap->ctx, XKB_ERROR_INVALID_MODIFIER_MAP_MASK,
                    "Cannot serialize modifier map of key %s: modifiers mask "
                    "0x%"PRIx32" incompatible with keymap format v1\n",
                    کلید_Name_Text(keymap->ctx, کلید->name), کلید->modmap);
        } وگرنه {
            /* V2 format or V1 format with a single core modifier */
            أكد(کلید->modmap);
            mm->mods = کلید->modmap;
            /* Force key name */
            mm->have_نماد = false;
            mm->u.کلید_Name = کلید->name;
            convert_to_v1_compatibility_entry = false;
        }
    }

    اگر (darray_empty(modmaps))
        برو exit;

    /* Sort entries by modifier mask */
    qsort(darray_اقلام(modmaps), darray_size(modmaps),
          اندازه(*darray_اقلام(modmaps)), &cmp_modmap_step_2);

    /*
     * Serialize modmaps
     */

    xkb_mod_mask_t current_mods = 0;
    ثابت ModMapEntry *entry;
    darray_foreach(entry, modmaps) {
        اگر (!entry->mods)
            ادامه;

        اگر (current_mods != entry->mods) {
            اگر (current_mods) {
                copy_to_buf(buf, " };\n");
            }
            current_mods = entry->mods;
            write_buf(buf, "\tmodifier_map %s { ",
                      ModMaskText(keymap->ctx, MOD_REAL, &keymap->mods,
                                  current_mods));
        } وگرنه {
            copy_to_buf(buf, ", ");
        }

        اگر (entry->have_نماد) {
            اگر (pretty || entry->u.کلید_Sym == XKB_کلید_No_نماد)
                write_buf(buf, "%s", KeysymText(keymap->ctx, entry->u.کلید_Sym));
            وگرنه
                write_buf(buf, "0x%"PRIx32, entry->u.کلید_Sym);
        } وگرنه {
            ثابت xkb_atom_t name = (substitutions == پوچ)
                ? entry->u.کلید_Name
                : substitute_name(substitutions, entry->u.کلید_Name);
            write_buf(buf, "%s", کلید_Name_Text(keymap->ctx, name));
        }
    }
    اگر (current_mods) {
        copy_to_buf(buf, " };\n");
    }

exit:
    darray_free(modmaps);
    بازگشت true;
}

ایستا bool
write_symbols(ثابت ساختار xkb_keymap *keymap,
              ثابت ساختار xkb_keymap_serialize_config *config,
              ثابت کلید_name_substitutions *substitutions,
              xkb_layout_index_t max_groups, bool some_interp,
              ساختار buf *buf)
{
    ثابت bool pretty = (config->flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ثابت bool defaults =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_مقادیر);
    ثابت bool explicit_کلید_مقادیر =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_کلید_مقادیر);
    ثابت bool drop_unused = !(config->flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    ثابت bool drop_interprets = (explicit_کلید_مقادیر && drop_unused);
    /* TODO: print all default values if explicit flag is set */

    اگر (keymap->symbols_section_name)
        write_buf(buf, "xkb_symbols \"%s\" {\n", keymap->symbols_section_name);
    وگرنه
        copy_to_buf(buf, "xkb_symbols {\n");

    bool has_group_names = false;
    xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
    برای (xkb_layout_index_t group = 0; group < keymap->رقم_group_names; group++) {
        اگر (!((UINT32_C(1) << group) & config->layouts))
            ادامه;

        new_group++;

        اگر (keymap->group_names[group]) {
            write_buf(buf, "\tname[%"PRIu32"]=", new_group + 1);
            write_buf_string_literal(
                buf, xkb_atom_text(keymap->ctx, keymap->group_names[group]));
            copy_to_buf(buf, ";\n");
            has_group_names = true;
        }
    }
    اگر (has_group_names)
        copy_to_buf(buf, "\n");

    اگر (defaults && !write_actions_defaults(keymap, config->format, buf))
        بازگشت false;

    ساختار buf buf2 = { پوچ, 0, 0 };
    ثابت ساختار xkb_کلید *کلید;
    xkb_کلیدها_foreach(کلید, keymap) {
        /* Skip keys with no explicit values */
        اگر (کلید->explicit &&
            !write_کلید(keymap, config, substitutions, max_groups, some_interp,
                       drop_interprets, explicit_کلید_مقادیر, pretty, buf,
                       &buf2, کلید))
        {
            آزاد(buf2.buf);
            بازگشت false;
        }
    }
    آزاد(buf2.buf);

    اگر (!write_modmaps(keymap, config->format, substitutions,
                       pretty, drop_interprets, buf))
        بازگشت false;

    copy_to_buf(buf, "};\n\n");
    بازگشت true;
}

ایستا شمارش xkb_status
write_keymap(ثابت ساختار xkb_keymap *keymap,
             ثابت ساختار xkb_keymap_serialize_config *config,
             ساختار buf *buf, ساختار xkb_keymap_serialize_result *result)
{
    کلید_name_substitutions substitutions = darray_new();
    اگر (config->format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !rename_long_کلیدها(keymap, &substitutions))
    {
        بازگشت false;
    }

    ثابت کلید_name_substitutions * ثابت substitutions_ptr =
        (darray_empty(substitutions)) ? پوچ : &substitutions;

    ثابت xkb_layout_index_t max_groups = format_max_groups(config->format);

    bool some_interp = false;
    ثابت bool ok = (
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
    بازگشت ok ? XKB_SUCCESS : -1;
}

شمارش xkb_status
text_v1_keymap_serialize(
        ثابت ساختار xkb_keymap *keymap,
        ثابت ساختار xkb_keymap_serialize_config *config,
        ساختار xkb_keymap_serialize_result *result
)
{
    ساختار buf buf = { پوچ, 0, 0 };

    ثابت شمارش xkb_status status =
        write_keymap(keymap, config, &buf, result);
    اگر (status != XKB_SUCCESS) {
        آزاد(buf.buf);
        result->serialized = پوچ;
        بازگشت status;
    }

    result->serialized = buf.buf;
    result->length = buf.size + 1;
    result->layouts = config->layouts;

    بازگشت XKB_SUCCESS;
}
