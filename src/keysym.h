/*
 * NOTE: This file has been generated automatically by “scripts/update-keysyms-derived-headers.py”.
 *       Do not edit manually!
 */

/*
 * For MIT-open-group:
 * Copyright 1985, 1987, 1990, 1998  The Open Group
 *
 * For MIT:
 * Copyright © 2009 Dan Nicholson
 *
 * SPDX-License-Identifier: MIT-open-group AND MIT
 */
#هدايت once

#ضمن "config.h"

#ضمن <stdbool.h>

#ضمن "xkbcommon/xkbcommon.h"

#ضمن "utils.h"

/*
 * NOTE: this is not defined in xkbcommon.h, because if we did, it may add
 * overhead for library user: when handling keysyms they would also need to
 * check min keysym when previously there was no reason to.
 */
/** Minimum keysym value */
#وصف_ڪريو XKB_KEYSYM_MIN            0x00000000
/** Minimum keysym value assigned */
#وصف_ڪريو XKB_KEYSYM_MIN_ASSIGNED   ((xkb_keysym_t)0x00000000)
/** Maximum keysym value assigned */
#وصف_ڪريو XKB_KEYSYM_MAX_ASSIGNED   0x1008ffb8
/** Minimum keysym value with explicit name */
#وصف_ڪريو XKB_KEYSYM_MIN_EXPLICIT   0x00000000
/** Maximum keysym value with explicit name */
#وصف_ڪريو XKB_KEYSYM_MAX_EXPLICIT   0x1008ffb8
/** Count of keysym value with explicit name */
#وصف_ڪريو XKB_KEYSYM_ڳڻپ_EXPLICIT 2505
/** Offset to use when converting a Unicode code point to a keysym */
#وصف_ڪريو XKB_KEYSYM_UNICODE_وٿي 0x01000000
/** Minimum Unicode keysym. NOTE: code points in 0..0xff cannot be converted. */
#وصف_ڪريو XKB_KEYSYM_UNICODE_MIN    0x01000100
/** Minimum Unicode surrogate keysym */
#وصف_ڪريو XKB_KEYSYM_UNICODE_SURROGATE_MIN 0x0100d800
/** Maximum Unicode surrogate keysym */
#وصف_ڪريو XKB_KEYSYM_UNICODE_SURROGATE_MAX 0x0100dfff
/** Maximum Unicode keysym, corresponding to the maximum Unicode code point */
#وصف_ڪريو XKB_KEYSYM_UNICODE_MAX    0x0110ffff
/** Unicode version used for case mappings */
#وصف_ڪريو XKB_KEYSYM_UNICODE_نسخو { 18, 0, 0, 0 }
/** Maximum keysym canonical name length, plus terminating NULL byte */
#وصف_ڪريو XKB_KEYSYM_NAME_MAX_سائيز  31
/** Longest keysym canonical name */
#وصف_ڪريو XKB_KEYSYM_LONGEST_CANONICAL_NAME XF86ElectronicPrivacyScreenOff
/** Longest keysym name */
#وصف_ڪريو XKB_KEYSYM_LONGEST_NAME XF86ElectronicPrivacyScreenOff
/** Maximum of distinct explicit names for a keysym */
#وصف_ڪريو XKB_KEYSYM_EXPLICIT_ALIASES_MAX 9
/** Maximum bytes to encode the Unicode representation of a keysym in UTF-8:
 * 4 bytes + NULL-terminating byte */
#وصف_ڪريو XKB_KEYSYM_UTF8_MAX_سائيز  5

XKB_EXPORT_ذاتي bool
xkb_keysym_is_assigned(xkb_keysym_t ks);

XKB_EXPORT_ذاتي صحيح
xkb_keysym_get_explicit_نالا(xkb_keysym_t ks, ثابت حرف **بفر, سائيز_t سائيز);

بنية xkb_keysym_iterator;

XKB_EXPORT_ذاتي بنية xkb_keysym_iterator*
xkb_keysym_iterator_new(bool explicit);

XKB_EXPORT_ذاتي بنية xkb_keysym_iterator*
xkb_keysym_iterator_unref(بنية xkb_keysym_iterator *iter);

XKB_EXPORT_ذاتي bool
xkb_keysym_iterator_ايندڙ(بنية xkb_keysym_iterator *iter);

XKB_EXPORT_ذاتي xkb_keysym_t
xkb_keysym_iterator_get_keysym(بنية xkb_keysym_iterator *iter);

XKB_EXPORT_ذاتي صحيح
xkb_keysym_iterator_get_name(بنية xkb_keysym_iterator *iter,
                             حرف *بفر, سائيز_t سائيز);

XKB_EXPORT_ذاتي bool
xkb_keysym_iterator_is_explicitly_named(بنية xkb_keysym_iterator *iter);

XKB_EXPORT_ذاتي bool
xkb_keysym_is_deprecated(xkb_keysym_t keysym,
                         ثابت حرف *name,
                         ثابت حرف **reference_name);

#وصف_ڪريو XKB_MIN_VERBOSITY_DEPRECATED_KEYSYM 2
#وصف_ڪريو check_deprecated_keysyms(log_func, log_param, ctx, keysym, name, token, format, پڇاڙي) \
    إذا (unlikely((ctx)->log_verbosity >= XKB_MIN_VERBOSITY_DEPRECATED_KEYSYM)) {             \
        ثابت حرف *ref_name = عدم;                                                         \
        إذا (xkb_keysym_is_deprecated(keysym, name, &ref_name)) {                             \
            إذا (ref_name == عدم)                                                            \
                log_func(log_param, XKB_WARNING_DEPRECATED_KEYSYM,                           \
                         "deprecated keysym \"" format "\"." پڇاڙي, token);                    \
            وإلا                                                                             \
                log_func(log_param, XKB_WARNING_DEPRECATED_KEYSYM_NAME,                      \
                         "deprecated keysym name \"" format "\"; "                           \
                         "please use \"%s\" instead." پڇاڙي,                                   \
                         token, ref_name);                                                   \
        }                                                                                    \
    }

XKB_EXPORT_ذاتي bool
xkb_keysym_is_هيٺاهون(xkb_keysym_t keysym);

XKB_EXPORT_ذاتي bool
xkb_keysym_is_مٿانهون_or_title(xkb_keysym_t keysym);

XKB_EXPORT_ذاتي bool
xkb_keysym_is_keypad(xkb_keysym_t keysym);

XKB_EXPORT_ذاتي bool
xkb_keysym_is_modifier(xkb_keysym_t keysym);
