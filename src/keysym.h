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
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdbool.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"

#ಅನ್ತರ್ಭಾವ "utils.h"

/*
 * NOTE: this is not defined in xkbcommon.h, because if we did, it may add
 * overhead for library user: when handling keysyms they would also need to
 * check min keysym when previously there was no reason to.
 */
/** Minimum keysym value */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_KEYSYM_MIN            0x00000000
/** Minimum keysym value assigned */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_KEYSYM_MIN_ASSIGNED   ((xkb_keysym_t)0x00000000)
/** Maximum keysym value assigned */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_KEYSYM_MAX_ASSIGNED   0x1008ffb8
/** Minimum keysym value with explicit name */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_KEYSYM_MIN_EXPLICIT   0x00000000
/** Maximum keysym value with explicit name */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_KEYSYM_MAX_EXPLICIT   0x1008ffb8
/** Count of keysym value with explicit name */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_KEYSYM_ಎಣಿಕೆ_EXPLICIT 2505
/** Offset to use when converting a Unicode code point to a keysym */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_KEYSYM_UNICODE_ಸ್ಥಾನಾಂತರ 0x01000000
/** Minimum Unicode keysym. NOTE: code points in 0..0xff cannot be converted. */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_KEYSYM_UNICODE_MIN    0x01000100
/** Minimum Unicode surrogate keysym */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_KEYSYM_UNICODE_SURROGATE_MIN 0x0100d800
/** Maximum Unicode surrogate keysym */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_KEYSYM_UNICODE_SURROGATE_MAX 0x0100dfff
/** Maximum Unicode keysym, corresponding to the maximum Unicode code point */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_KEYSYM_UNICODE_MAX    0x0110ffff
/** Unicode version used for case mappings */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_KEYSYM_UNICODE_ಆವೃತ್ತಿ { 18, 0, 0, 0 }
/** Maximum keysym canonical name length, plus terminating NULL byte */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_KEYSYM_NAME_MAX_ಗಾತ್ರ  31
/** Longest keysym canonical name */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_KEYSYM_LONGEST_CANONICAL_NAME XF86ElectronicPrivacyScreenOff
/** Longest keysym name */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_KEYSYM_LONGEST_NAME XF86ElectronicPrivacyScreenOff
/** Maximum of distinct explicit names for a keysym */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_KEYSYM_EXPLICIT_ALIASES_MAX 9
/** Maximum bytes to encode the Unicode representation of a keysym in UTF-8:
 * 4 bytes + NULL-terminating byte */
#ವ್ಯಾಖ್ಯಾನಿಸು XKB_KEYSYM_UTF8_MAX_ಗಾತ್ರ  5

XKB_EXPORT_ಖಾಸಗಿ bool
xkb_keysym_is_assigned(xkb_keysym_t ks);

XKB_EXPORT_ಖಾಸಗಿ ಪೂರ್ಣಾಂಕ
xkb_keysym_get_explicit_ಹೆಸರುಗಳು(xkb_keysym_t ks, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಬಫರ್, ಗಾತ್ರ_t ಗಾತ್ರ_2);

ರಚನೆ xkb_keysym_iterator;

XKB_EXPORT_ಖಾಸಗಿ ರಚನೆ xkb_keysym_iterator*
xkb_keysym_iterator_new(bool explicit);

XKB_EXPORT_ಖಾಸಗಿ ರಚನೆ xkb_keysym_iterator*
xkb_keysym_iterator_unref(ರಚನೆ xkb_keysym_iterator *iter);

XKB_EXPORT_ಖಾಸಗಿ bool
xkb_keysym_iterator_ಮುಂದಿನ(ರಚನೆ xkb_keysym_iterator *iter);

XKB_EXPORT_ಖಾಸಗಿ xkb_keysym_t
xkb_keysym_iterator_get_keysym(ರಚನೆ xkb_keysym_iterator *iter);

XKB_EXPORT_ಖಾಸಗಿ ಪೂರ್ಣಾಂಕ
xkb_keysym_iterator_get_name(ರಚನೆ xkb_keysym_iterator *iter,
                             ಅಕ್ಷರ *ಬಫರ್, ಗಾತ್ರ_t ಗಾತ್ರ_2);

XKB_EXPORT_ಖಾಸಗಿ bool
xkb_keysym_iterator_is_explicitly_named(ರಚನೆ xkb_keysym_iterator *iter);

XKB_EXPORT_ಖಾಸಗಿ bool
xkb_keysym_is_deprecated(xkb_keysym_t keysym,
                         ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
                         ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **reference_name);

#ವ್ಯಾಖ್ಯಾನಿಸು XKB_MIN_VERBOSITY_DEPRECATED_KEYSYM 2
#ವ್ಯಾಖ್ಯಾನಿಸು check_deprecated_keysyms(log_func, log_param, ctx, keysym, name, token, format, ಕೊನೆ) \
    ಆದರೆ (unlikely((ctx)->log_verbosity >= XKB_MIN_VERBOSITY_DEPRECATED_KEYSYM)) {             \
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref_name = ಖಾಲಿ;                                                         \
        ಆದರೆ (xkb_keysym_is_deprecated(keysym, name, &ref_name)) {                             \
            ಆದರೆ (ref_name == ಖಾಲಿ)                                                            \
                log_func(log_param, XKB_WARNING_DEPRECATED_KEYSYM,                           \
                         "deprecated keysym \"" format "\"." ಕೊನೆ, token);                    \
            ಇಲ್ಲದಿದ್ದರೆ                                                                             \
                log_func(log_param, XKB_WARNING_DEPRECATED_KEYSYM_NAME,                      \
                         "deprecated keysym name \"" format "\"; "                           \
                         "please use \"%s\" instead." ಕೊನೆ,                                   \
                         token, ref_name);                                                   \
        }                                                                                    \
    }

XKB_EXPORT_ಖಾಸಗಿ bool
xkb_keysym_is_ಅಧೋ(xkb_keysym_t keysym);

XKB_EXPORT_ಖಾಸಗಿ bool
xkb_keysym_is_ಊರ್ಧ್ವ_or_title(xkb_keysym_t keysym);

XKB_EXPORT_ಖಾಸಗಿ bool
xkb_keysym_is_keypad(xkb_keysym_t keysym);

XKB_EXPORT_ಖಾಸಗಿ bool
xkb_keysym_is_modifier(xkb_keysym_t keysym);
