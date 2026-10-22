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
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

/*
 * NOTE: this is not defined in xkbcommon.h, because if we did, it may add
 * overhead for library user: when handling keysyms they would also need to
 * check min keysym when previously there was no reason to.
 */
/** Minimum keysym value */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_KEYSYM_MIN            0x00000000
/** Minimum keysym value assigned */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_KEYSYM_MIN_ASSIGNED   ((xkb_keysym_t)0x00000000)
/** Maximum keysym value assigned */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_KEYSYM_MAX_ASSIGNED   0x1008ffb8
/** Minimum keysym value with explicit name */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_KEYSYM_MIN_EXPLICIT   0x00000000
/** Maximum keysym value with explicit name */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_KEYSYM_MAX_EXPLICIT   0x1008ffb8
/** Count of keysym value with explicit name */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_KEYSYM_ਗਿਣਤੀ_EXPLICIT 2505
/** Offset to use when converting a Unicode code point to a keysym */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_KEYSYM_UNICODE_ਵਿੱਥ 0x01000000
/** Minimum Unicode keysym. NOTE: code points in 0..0xff cannot be converted. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_KEYSYM_UNICODE_MIN    0x01000100
/** Minimum Unicode surrogate keysym */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_KEYSYM_UNICODE_SURROGATE_MIN 0x0100d800
/** Maximum Unicode surrogate keysym */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_KEYSYM_UNICODE_SURROGATE_MAX 0x0100dfff
/** Maximum Unicode keysym, corresponding to the maximum Unicode code point */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_KEYSYM_UNICODE_MAX    0x0110ffff
/** Unicode version used for case mappings */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_KEYSYM_UNICODE_ਸੰਸਕਰਣ { 18, 0, 0, 0 }
/** Maximum keysym canonical name length, plus terminating NULL byte */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_KEYSYM_NAME_MAX_ਮਾਪ  31
/** Longest keysym canonical name */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_KEYSYM_LONGEST_CANONICAL_NAME XF86ElectronicPrivacyScreenOff
/** Longest keysym name */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_KEYSYM_LONGEST_NAME XF86ElectronicPrivacyScreenOff
/** Maximum of distinct explicit names for a keysym */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_KEYSYM_EXPLICIT_ALIASES_MAX 9
/** Maximum bytes to encode the Unicode representation of a keysym in UTF-8:
 * 4 bytes + NULL-terminating byte */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_KEYSYM_UTF8_MAX_ਮਾਪ  5

XKB_EXPORT_ਨਿੱਜੀ bool
xkb_keysym_is_assigned(xkb_keysym_t ks);

XKB_EXPORT_ਨਿੱਜੀ ਪੂਰਨ
xkb_keysym_get_explicit_ਨਾਂ(xkb_keysym_t ks, ਅਚਲ ਅੱਖਰ **ਬਫਰ, ਮਾਪ_t ਮਾਪ);

ਬਣਤਰ xkb_keysym_iterator;

XKB_EXPORT_ਨਿੱਜੀ ਬਣਤਰ xkb_keysym_iterator*
xkb_keysym_iterator_new(bool explicit);

XKB_EXPORT_ਨਿੱਜੀ ਬਣਤਰ xkb_keysym_iterator*
xkb_keysym_iterator_unref(ਬਣਤਰ xkb_keysym_iterator *iter);

XKB_EXPORT_ਨਿੱਜੀ bool
xkb_keysym_iterator_ਅਗਲਾ(ਬਣਤਰ xkb_keysym_iterator *iter);

XKB_EXPORT_ਨਿੱਜੀ xkb_keysym_t
xkb_keysym_iterator_get_keysym(ਬਣਤਰ xkb_keysym_iterator *iter);

XKB_EXPORT_ਨਿੱਜੀ ਪੂਰਨ
xkb_keysym_iterator_get_name(ਬਣਤਰ xkb_keysym_iterator *iter,
                             ਅੱਖਰ *ਬਫਰ, ਮਾਪ_t ਮਾਪ);

XKB_EXPORT_ਨਿੱਜੀ bool
xkb_keysym_iterator_is_explicitly_named(ਬਣਤਰ xkb_keysym_iterator *iter);

XKB_EXPORT_ਨਿੱਜੀ bool
xkb_keysym_is_deprecated(xkb_keysym_t keysym,
                         ਅਚਲ ਅੱਖਰ *name,
                         ਅਚਲ ਅੱਖਰ **reference_name);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XKB_MIN_VERBOSITY_DEPRECATED_KEYSYM 2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ check_deprecated_keysyms(log_func, log_param, ctx, keysym, name, token, format, ਸਿਰਾ) \
    ਜੇ (unlikely((ctx)->log_verbosity >= XKB_MIN_VERBOSITY_DEPRECATED_KEYSYM)) {             \
        ਅਚਲ ਅੱਖਰ *ref_name = ਨਿਰਾ;                                                         \
        ਜੇ (xkb_keysym_is_deprecated(keysym, name, &ref_name)) {                             \
            ਜੇ (ref_name == ਨਿਰਾ)                                                            \
                log_func(log_param, XKB_WARNING_DEPRECATED_KEYSYM,                           \
                         "deprecated keysym \"" format "\"." ਸਿਰਾ, token);                    \
            ਨਹੀਂਤਾਂ                                                                             \
                log_func(log_param, XKB_WARNING_DEPRECATED_KEYSYM_NAME,                      \
                         "deprecated keysym name \"" format "\"; "                           \
                         "please use \"%s\" instead." ਸਿਰਾ,                                   \
                         token, ref_name);                                                   \
        }                                                                                    \
    }

XKB_EXPORT_ਨਿੱਜੀ bool
xkb_keysym_is_ਹੇਠਲਾ(xkb_keysym_t keysym);

XKB_EXPORT_ਨਿੱਜੀ bool
xkb_keysym_is_ਉੱਤਲਾ_or_title(xkb_keysym_t keysym);

XKB_EXPORT_ਨਿੱਜੀ bool
xkb_keysym_is_keypad(xkb_keysym_t keysym);

XKB_EXPORT_ਨਿੱਜੀ bool
xkb_keysym_is_modifier(xkb_keysym_t keysym);
