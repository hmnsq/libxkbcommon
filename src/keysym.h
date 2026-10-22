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
#निर्देशः once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdbool.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"

#अन्तर्भाव "utils.h"

/*
 * NOTE: this is not defined in xkbcommon.h, because if we did, it may add
 * overhead for library user: when handling keysyms they would also need to
 * check min keysym when previously there was no reason to.
 */
/** Minimum keysym value */
#परिभाषय XKB_KEYSYM_MIN            0x00000000
/** Minimum keysym value assigned */
#परिभाषय XKB_KEYSYM_MIN_ASSIGNED   ((xkb_keysym_t)0x00000000)
/** Maximum keysym value assigned */
#परिभाषय XKB_KEYSYM_MAX_ASSIGNED   0x1008ffb8
/** Minimum keysym value with explicit name */
#परिभाषय XKB_KEYSYM_MIN_EXPLICIT   0x00000000
/** Maximum keysym value with explicit name */
#परिभाषय XKB_KEYSYM_MAX_EXPLICIT   0x1008ffb8
/** Count of keysym value with explicit name */
#परिभाषय XKB_KEYSYM_गणनम्_EXPLICIT 2505
/** Offset to use when converting a Unicode code point to a keysym */
#परिभाषय XKB_KEYSYM_UNICODE_विस्थापनम् 0x01000000
/** Minimum Unicode keysym. NOTE: code points in 0..0xff cannot be converted. */
#परिभाषय XKB_KEYSYM_UNICODE_MIN    0x01000100
/** Minimum Unicode surrogate keysym */
#परिभाषय XKB_KEYSYM_UNICODE_SURROGATE_MIN 0x0100d800
/** Maximum Unicode surrogate keysym */
#परिभाषय XKB_KEYSYM_UNICODE_SURROGATE_MAX 0x0100dfff
/** Maximum Unicode keysym, corresponding to the maximum Unicode code point */
#परिभाषय XKB_KEYSYM_UNICODE_MAX    0x0110ffff
/** Unicode version used for case mappings */
#परिभाषय XKB_KEYSYM_UNICODE_संस्करणम् { 18, 0, 0, 0 }
/** Maximum keysym canonical name length, plus terminating NULL byte */
#परिभाषय XKB_KEYSYM_NAME_MAX_आकार  31
/** Longest keysym canonical name */
#परिभाषय XKB_KEYSYM_LONGEST_CANONICAL_NAME XF86ElectronicPrivacyScreenOff
/** Longest keysym name */
#परिभाषय XKB_KEYSYM_LONGEST_NAME XF86ElectronicPrivacyScreenOff
/** Maximum of distinct explicit names for a keysym */
#परिभाषय XKB_KEYSYM_EXPLICIT_ALIASES_MAX 9
/** Maximum bytes to encode the Unicode representation of a keysym in UTF-8:
 * 4 bytes + NULL-terminating byte */
#परिभाषय XKB_KEYSYM_UTF8_MAX_आकार  5

XKB_EXPORT_वैयक्तिकम् bool
xkb_keysym_is_assigned(xkb_keysym_t ks);

XKB_EXPORT_वैयक्तिकम् पूर्णाङ्क
xkb_keysym_get_explicit_नामानि(xkb_keysym_t ks, ध्रुव वर्ण **बफर, आकार_t आकार);

संरचना xkb_keysym_iterator;

XKB_EXPORT_वैयक्तिकम् संरचना xkb_keysym_iterator*
xkb_keysym_iterator_new(bool explicit);

XKB_EXPORT_वैयक्तिकम् संरचना xkb_keysym_iterator*
xkb_keysym_iterator_unref(संरचना xkb_keysym_iterator *iter);

XKB_EXPORT_वैयक्तिकम् bool
xkb_keysym_iterator_अग्र(संरचना xkb_keysym_iterator *iter);

XKB_EXPORT_वैयक्तिकम् xkb_keysym_t
xkb_keysym_iterator_get_keysym(संरचना xkb_keysym_iterator *iter);

XKB_EXPORT_वैयक्तिकम् पूर्णाङ्क
xkb_keysym_iterator_get_name(संरचना xkb_keysym_iterator *iter,
                             वर्ण *बफर, आकार_t आकार);

XKB_EXPORT_वैयक्तिकम् bool
xkb_keysym_iterator_is_explicitly_named(संरचना xkb_keysym_iterator *iter);

XKB_EXPORT_वैयक्तिकम् bool
xkb_keysym_is_deprecated(xkb_keysym_t keysym,
                         ध्रुव वर्ण *name,
                         ध्रुव वर्ण **reference_name);

#परिभाषय XKB_MIN_VERBOSITY_DEPRECATED_KEYSYM 2
#परिभाषय check_deprecated_keysyms(log_func, log_param, ctx, keysym, name, token, format, अन्तः) \
    यदि (unlikely((ctx)->log_verbosity >= XKB_MIN_VERBOSITY_DEPRECATED_KEYSYM)) {             \
        ध्रुव वर्ण *ref_name = रिक्त;                                                         \
        यदि (xkb_keysym_is_deprecated(keysym, name, &ref_name)) {                             \
            यदि (ref_name == रिक्त)                                                            \
                log_func(log_param, XKB_WARNING_DEPRECATED_KEYSYM,                           \
                         "deprecated keysym \"" format "\"." अन्तः, token);                    \
            अन्यथा                                                                             \
                log_func(log_param, XKB_WARNING_DEPRECATED_KEYSYM_NAME,                      \
                         "deprecated keysym name \"" format "\"; "                           \
                         "please use \"%s\" instead." अन्तः,                                   \
                         token, ref_name);                                                   \
        }                                                                                    \
    }

XKB_EXPORT_वैयक्तिकम् bool
xkb_keysym_is_अधस्तनम्(xkb_keysym_t keysym);

XKB_EXPORT_वैयक्तिकम् bool
xkb_keysym_is_ऊर्ध्वस्थम्_or_title(xkb_keysym_t keysym);

XKB_EXPORT_वैयक्तिकम् bool
xkb_keysym_is_keypad(xkb_keysym_t keysym);

XKB_EXPORT_वैयक्तिकम् bool
xkb_keysym_is_modifier(xkb_keysym_t keysym);
