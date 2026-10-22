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
#आशय once

#समावेश "config.h"

#समावेश <stdbool.h>

#समावेश "xkbcommon/xkbcommon.h"

#समावेश "utils.h"

/*
 * NOTE: this is not defined in xkbcommon.h, because if we did, it may add
 * overhead for library user: when handling keysyms they would also need to
 * check min keysym when previously there was no reason to.
 */
/** Minimum keysym value */
#घोषणा XKB_KEYSYM_MIN            0x00000000
/** Minimum keysym value assigned */
#घोषणा XKB_KEYSYM_MIN_ASSIGNED   ((xkb_keysym_t)0x00000000)
/** Maximum keysym value assigned */
#घोषणा XKB_KEYSYM_MAX_ASSIGNED   0x1008ffb8
/** Minimum keysym value with explicit name */
#घोषणा XKB_KEYSYM_MIN_EXPLICIT   0x00000000
/** Maximum keysym value with explicit name */
#घोषणा XKB_KEYSYM_MAX_EXPLICIT   0x1008ffb8
/** Count of keysym value with explicit name */
#घोषणा XKB_KEYSYM_गिनती_EXPLICIT 2505
/** Offset to use when converting a Unicode code point to a keysym */
#घोषणा XKB_KEYSYM_UNICODE_OFFSET 0x01000000
/** Minimum Unicode keysym. NOTE: code points in 0..0xff cannot be converted. */
#घोषणा XKB_KEYSYM_UNICODE_MIN    0x01000100
/** Minimum Unicode surrogate keysym */
#घोषणा XKB_KEYSYM_UNICODE_SURROGATE_MIN 0x0100d800
/** Maximum Unicode surrogate keysym */
#घोषणा XKB_KEYSYM_UNICODE_SURROGATE_MAX 0x0100dfff
/** Maximum Unicode keysym, corresponding to the maximum Unicode code point */
#घोषणा XKB_KEYSYM_UNICODE_MAX    0x0110ffff
/** Unicode version used for case mappings */
#घोषणा XKB_KEYSYM_UNICODE_VERSION { 18, 0, 0, 0 }
/** Maximum keysym canonical name length, plus terminating NULL byte */
#घोषणा XKB_KEYSYM_NAME_MAX_आकार  31
/** Longest keysym canonical name */
#घोषणा XKB_KEYSYM_LONGEST_CANONICAL_NAME XF86ElectronicPrivacyScreenOff
/** Longest keysym name */
#घोषणा XKB_KEYSYM_LONGEST_NAME XF86ElectronicPrivacyScreenOff
/** Maximum of distinct explicit names for a keysym */
#घोषणा XKB_KEYSYM_EXPLICIT_ALIASES_MAX 9
/** Maximum bytes to encode the Unicode representation of a keysym in UTF-8:
 * 4 bytes + NULL-terminating byte */
#घोषणा XKB_KEYSYM_UTF8_MAX_आकार  5

XKB_EXPORT_PRIVATE bool
xkb_keysym_is_assigned(xkb_keysym_t ks);

XKB_EXPORT_PRIVATE पूर्णांक
xkb_keysym_get_explicit_names(xkb_keysym_t ks, स्थिर अक्षर **बफर, माप_प्रकार आकार);

काष्ठा xkb_keysym_iterator;

XKB_EXPORT_PRIVATE काष्ठा xkb_keysym_iterator*
xkb_keysym_iterator_new(bool explicit);

XKB_EXPORT_PRIVATE काष्ठा xkb_keysym_iterator*
xkb_keysym_iterator_unref(काष्ठा xkb_keysym_iterator *iter);

XKB_EXPORT_PRIVATE bool
xkb_keysym_iterator_अग्र(काष्ठा xkb_keysym_iterator *iter);

XKB_EXPORT_PRIVATE xkb_keysym_t
xkb_keysym_iterator_get_keysym(काष्ठा xkb_keysym_iterator *iter);

XKB_EXPORT_PRIVATE पूर्णांक
xkb_keysym_iterator_get_name(काष्ठा xkb_keysym_iterator *iter,
                             अक्षर *बफर, माप_प्रकार आकार);

XKB_EXPORT_PRIVATE bool
xkb_keysym_iterator_is_explicitly_named(काष्ठा xkb_keysym_iterator *iter);

XKB_EXPORT_PRIVATE bool
xkb_keysym_is_deprecated(xkb_keysym_t keysym,
                         स्थिर अक्षर *name,
                         स्थिर अक्षर **reference_name);

#घोषणा XKB_MIN_VERBOSITY_DEPRECATED_KEYSYM 2
#घोषणा check_deprecated_keysyms(log_func, log_param, ctx, keysym, name, token, format, end) \
    अगर (unlikely((ctx)->log_verbosity >= XKB_MIN_VERBOSITY_DEPRECATED_KEYSYM)) {             \
        स्थिर अक्षर *ref_name = शून्य;                                                         \
        अगर (xkb_keysym_is_deprecated(keysym, name, &ref_name)) {                             \
            अगर (ref_name == शून्य)                                                            \
                log_func(log_param, XKB_WARNING_DEPRECATED_KEYSYM,                           \
                         "deprecated keysym \"" format "\"." end, token);                    \
            अन्यथा                                                                             \
                log_func(log_param, XKB_WARNING_DEPRECATED_KEYSYM_NAME,                      \
                         "deprecated keysym name \"" format "\"; "                           \
                         "please use \"%s\" instead." end,                                   \
                         token, ref_name);                                                   \
        }                                                                                    \
    }

XKB_EXPORT_PRIVATE bool
xkb_keysym_is_lower(xkb_keysym_t keysym);

XKB_EXPORT_PRIVATE bool
xkb_keysym_is_upper_or_title(xkb_keysym_t keysym);

XKB_EXPORT_PRIVATE bool
xkb_keysym_is_keypad(xkb_keysym_t keysym);

XKB_EXPORT_PRIVATE bool
xkb_keysym_is_modifier(xkb_keysym_t keysym);
