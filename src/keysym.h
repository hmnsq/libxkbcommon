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
#ہدایت once

#شامل "config.h"

#شامل <stdbool.h>

#شامل "xkbcommon/xkbcommon.h"

#شامل "utils.h"

/*
 * NOTE: this is not defined in xkbcommon.h, because if we did, it may add
 * overhead for library user: when handling keysyms they would also need to
 * check min keysym when previously there was no reason to.
 */
/** Minimum keysym value */
#تعریف_کرو XKB_KEYSYM_MIN            0x00000000
/** Minimum keysym value assigned */
#تعریف_کرو XKB_KEYSYM_MIN_ASSIGNED   ((xkb_keysym_t)0x00000000)
/** Maximum keysym value assigned */
#تعریف_کرو XKB_KEYSYM_MAX_ASSIGNED   0x1008ffb8
/** Minimum keysym value with explicit name */
#تعریف_کرو XKB_KEYSYM_MIN_EXPLICIT   0x00000000
/** Maximum keysym value with explicit name */
#تعریف_کرو XKB_KEYSYM_MAX_EXPLICIT   0x1008ffb8
/** Count of keysym value with explicit name */
#تعریف_کرو XKB_KEYSYM_گنتی_EXPLICIT 2505
/** Offset to use when converting a Unicode code point to a keysym */
#تعریف_کرو XKB_KEYSYM_UNICODE_انحراف 0x01000000
/** Minimum Unicode keysym. NOTE: code points in 0..0xff cannot be converted. */
#تعریف_کرو XKB_KEYSYM_UNICODE_MIN    0x01000100
/** Minimum Unicode surrogate keysym */
#تعریف_کرو XKB_KEYSYM_UNICODE_SURROGATE_MIN 0x0100d800
/** Maximum Unicode surrogate keysym */
#تعریف_کرو XKB_KEYSYM_UNICODE_SURROGATE_MAX 0x0100dfff
/** Maximum Unicode keysym, corresponding to the maximum Unicode code point */
#تعریف_کرو XKB_KEYSYM_UNICODE_MAX    0x0110ffff
/** Unicode version used for case mappings */
#تعریف_کرو XKB_KEYSYM_UNICODE_نسخہ { 18, 0, 0, 0 }
/** Maximum keysym canonical name length, plus terminating NULL byte */
#تعریف_کرو XKB_KEYSYM_NAME_MAX_سائز  31
/** Longest keysym canonical name */
#تعریف_کرو XKB_KEYSYM_LONGEST_CANONICAL_NAME XF86ElectronicPrivacyScreenOff
/** Longest keysym name */
#تعریف_کرو XKB_KEYSYM_LONGEST_NAME XF86ElectronicPrivacyScreenOff
/** Maximum of distinct explicit names for a keysym */
#تعریف_کرو XKB_KEYSYM_EXPLICIT_ALIASES_MAX 9
/** Maximum bytes to encode the Unicode representation of a keysym in UTF-8:
 * 4 bytes + NULL-terminating byte */
#تعریف_کرو XKB_KEYSYM_UTF8_MAX_سائز  5

XKB_EXPORT_نجی bool
xkb_keysym_is_assigned(xkb_keysym_t ks);

XKB_EXPORT_نجی صحیح
xkb_keysym_get_explicit_نام(xkb_keysym_t ks, ثابت حرف **بفر, سائز_t سائز);

ساخت xkb_keysym_iterator;

XKB_EXPORT_نجی ساخت xkb_keysym_iterator*
xkb_keysym_iterator_new(bool explicit);

XKB_EXPORT_نجی ساخت xkb_keysym_iterator*
xkb_keysym_iterator_unref(ساخت xkb_keysym_iterator *iter);

XKB_EXPORT_نجی bool
xkb_keysym_iterator_اگلا(ساخت xkb_keysym_iterator *iter);

XKB_EXPORT_نجی xkb_keysym_t
xkb_keysym_iterator_get_keysym(ساخت xkb_keysym_iterator *iter);

XKB_EXPORT_نجی صحیح
xkb_keysym_iterator_get_name(ساخت xkb_keysym_iterator *iter,
                             حرف *بفر, سائز_t سائز);

XKB_EXPORT_نجی bool
xkb_keysym_iterator_is_explicitly_named(ساخت xkb_keysym_iterator *iter);

XKB_EXPORT_نجی bool
xkb_keysym_is_deprecated(xkb_keysym_t keysym,
                         ثابت حرف *name,
                         ثابت حرف **reference_name);

#تعریف_کرو XKB_MIN_VERBOSITY_DEPRECATED_KEYSYM 2
#تعریف_کرو check_deprecated_keysyms(log_func, log_param, ctx, keysym, name, token, format, انتہا) \
    اگر (unlikely((ctx)->log_verbosity >= XKB_MIN_VERBOSITY_DEPRECATED_KEYSYM)) {             \
        ثابت حرف *ref_name = تہی;                                                         \
        اگر (xkb_keysym_is_deprecated(keysym, name, &ref_name)) {                             \
            اگر (ref_name == تہی)                                                            \
                log_func(log_param, XKB_WARNING_DEPRECATED_KEYSYM,                           \
                         "deprecated keysym \"" format "\"." انتہا, token);                    \
            ورنہ                                                                             \
                log_func(log_param, XKB_WARNING_DEPRECATED_KEYSYM_NAME,                      \
                         "deprecated keysym name \"" format "\"; "                           \
                         "please use \"%s\" instead." انتہا,                                   \
                         token, ref_name);                                                   \
        }                                                                                    \
    }

XKB_EXPORT_نجی bool
xkb_keysym_is_زیریں(xkb_keysym_t keysym);

XKB_EXPORT_نجی bool
xkb_keysym_is_بالائی_or_title(xkb_keysym_t keysym);

XKB_EXPORT_نجی bool
xkb_keysym_is_keypad(xkb_keysym_t keysym);

XKB_EXPORT_نجی bool
xkb_keysym_is_modifier(xkb_keysym_t keysym);
