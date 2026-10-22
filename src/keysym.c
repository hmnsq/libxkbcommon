/*
 * For MIT-open-group:
 * Copyright 1985, 1987, 1990, 1998  The Open Group
 *
 * For MIT:
 * Copyright © 2009 Dan Nicholson
 *
 * SPDX-License-Identifier: MIT-open-group AND MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-keysyms.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "features/enums.h"
#ಅನ್ತರ್ಭಾವ "keysym.h"
#ಅನ್ತರ್ಭಾವ "keysym-names.h"
#ಅನ್ತರ್ಭಾವ "utf8-decoding.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "util-numbers.h"

ಸ್ಥಿರ ssize_t
find_keysym_index(xkb_keysym_t ks)
{
    /* Lower bound:
     * keysym_to_name[0].keysym
     * == XKB_KEYSYM_MIN_EXPLICIT == XKB_KEYSYM_MIN == 0
     * No need to check: xkb_keysym_t is unsigned.
     *
     * Upper bound:
     * keysym_to_name[ARRAY_SIZE(keysym_to_name) - 1].keysym
     * == XKB_KEYSYM_MAX_EXPLICIT <= XKB_KEYSYM_MAX
     */
    ಯದಿ (ks > XKB_KEYSYM_MAX_EXPLICIT)
        ಪ್ರತ್ಯಾಗಮ -1;

    ssize_t lo = 0, hi = ARRAY_SIZE(keysym_to_name) - 1;
    ಯಾವತ್ (hi >= lo) {
        ssize_t mid = (lo + hi) / 2;
        ಯದಿ (ks > keysym_to_name[mid].keysym) {
            lo = mid + 1;
        } ಅನ್ಯಥಾ ಯದಿ (ks < keysym_to_name[mid].keysym) {
            hi = mid - 1;
        } ಅನ್ಯಥಾ {
            ಪ್ರತ್ಯಾಗಮ mid;
        }
    }

    ಪ್ರತ್ಯಾಗಮ -1;
}

#define get_name_by_index(index) (keysym_names + (index))

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ ವರ್ಣ *
get_name(ಧ್ರುವ ಸಂರಚನಾ name_keysym *entry)
{
    ಪ್ರತ್ಯಾಗಮ get_name_by_index(entry->offset);
}

/* Unnamed Unicode codepoint. */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
get_unicode_name(xkb_keysym_t ks, ವರ್ಣ *buffer, size_t size)
{
    ಪ್ರತ್ಯಾಗಮ snprintf(buffer, size, "U%04"PRIX32, ks & UINT32_C(0xffffff));
}

ಪೂರ್ಣಾಙ್ಕ
xkb_keysym_get_name(xkb_keysym_t ks, ವರ್ಣ *buffer, size_t size)
{
    ಯದಿ (ks > XKB_KEYSYM_MAX) {
        snprintf(buffer, size, "Invalid");
        ಪ್ರತ್ಯಾಗಮ -1;
    }

    ssize_t index = find_keysym_index(ks);
    ಯದಿ (index != -1)
        ಪ್ರತ್ಯಾಗಮ snprintf(buffer, size, "%s", get_name(&keysym_to_name[index]));

    /*
     * Unnamed Unicode codepoint.
     * • Keysyms in the range [XKB_KEYSYM_UNICODE_OFFSET, XKB_KEYSYM_UNICODE_MIN [
     *   do not use the Unicode notation for backward compatibility.
     * • Keysyms are not normalized: e.g. given a Unicode keysym, calling
     *   `xkb_utf32_to_keysym` with the corresponding code point may return a
     *   different keysym or fail (e.g. surrogates are invalid in UTF-32).
     */
    ಯದಿ (ks >= XKB_KEYSYM_UNICODE_MIN && ks <= XKB_KEYSYM_UNICODE_MAX)
        ಪ್ರತ್ಯಾಗಮ get_unicode_name(ks, buffer, size);

    /* Unnamed, non-Unicode, symbol (shouldn't generally happen). */
    ಪ್ರತ್ಯಾಗಮ snprintf(buffer, size, "0x%08"PRIx32, ks);
}

bool
xkb_keysym_is_assigned(xkb_keysym_t ks)
{
    ಪ್ರತ್ಯಾಗಮ (XKB_KEYSYM_UNICODE_MIN <= ks && ks <= XKB_KEYSYM_UNICODE_MAX) ||
           find_keysym_index(ks) != -1;
}

ಪೂರ್ಣಾಙ್ಕ
xkb_keysym_get_explicit_names(xkb_keysym_t ks, ಧ್ರುವ ವರ್ಣ **buffer, size_t size)
{
    ಯದಿ (ks > XKB_KEYSYM_MAX)
        ಪ್ರತ್ಯಾಗಮ -1;
    ಧ್ರುವ ssize_t index = find_keysym_index(ks);
    ಯದಿ (index < 0)
        ಪ್ರತ್ಯಾಗಮ 0;
    ಧ್ರುವ uint16_t canonical = keysym_to_name[index].offset;
    ಯದಿ (size > 0)
        buffer[0] = get_name(&keysym_to_name[index]);
    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 1;
    ಪ್ರತಿ (size_t pos = 0; pos < ARRAY_SIZE(name_to_keysym); pos++) {
        ಯದಿ (name_to_keysym[pos].keysym == ks &&
            name_to_keysym[pos].offset != canonical) {
            ಯದಿ ((size_t) ಎಣಿಕೆ < size) {
                buffer[ಎಣಿಕೆ] = get_name(&name_to_keysym[pos]);
            }
            ಎಣಿಕೆ++;
        }
    }
    ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;
}

ಸಂರಚನಾ xkb_keysym_iterator {
    bool explicit;       /* If true, traverse only explicitly named keysyms */
    int32_t index;       /* Current index in `keysym_to_name` */
    xkb_keysym_t keysym; /* Current keysym */
};

ಸಂರಚನಾ xkb_keysym_iterator*
xkb_keysym_iterator_new(bool iterate_only_explicit_keysyms)
{
    ಸಂರಚನಾ xkb_keysym_iterator* iter = calloc(1, ಪರಿಮಾಣ(*iter));
    ಯದಿ (!iter)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    iter->explicit = iterate_only_explicit_keysyms;
    iter->index = -1;
    iter->keysym = XKB_KEYSYM_UNICODE_MAX;
    ಪ್ರತ್ಯಾಗಮ iter;
}

ಸಂರಚನಾ xkb_keysym_iterator*
xkb_keysym_iterator_unref(ಸಂರಚನಾ xkb_keysym_iterator *iter)
{
    /*
     * [NOTE] If we ever make this API public, add:
     * - an `refcnt` member,
     * - `xkb_keysym_iterator_ref()`,
     * - `assert(!iter || iter->refcnt > 0)`.
     */
    ಯದಿ (iter)
        ಮುಕ್ತ(iter);
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

xkb_keysym_t
xkb_keysym_iterator_get_keysym(ಸಂರಚನಾ xkb_keysym_iterator *iter)
{
    ಪ್ರತ್ಯಾಗಮ iter->keysym;
}

bool
xkb_keysym_iterator_is_explicitly_named(ಸಂರಚನಾ xkb_keysym_iterator *iter)
{
    ಪ್ರತ್ಯಾಗಮ iter->index >= 0 &&
           iter->index < (int32_t)ARRAY_SIZE(keysym_to_name) &&
           (iter->explicit ||
            iter->keysym == keysym_to_name[iter->index].keysym);
}

ಪೂರ್ಣಾಙ್ಕ
xkb_keysym_iterator_get_name(ಸಂರಚನಾ xkb_keysym_iterator *iter,
                             ವರ್ಣ *buffer, size_t size)
{
    ಯದಿ (iter->index < 0 || iter->index >= (int32_t)ARRAY_SIZE(keysym_to_name))
        ಪ್ರತ್ಯಾಗಮ -1;
    ಯದಿ (iter->explicit || iter->keysym == keysym_to_name[iter->index].keysym)
        ಪ್ರತ್ಯಾಗಮ snprintf(buffer, size, "%s",
                        get_name(&keysym_to_name[iter->index]));
    ಪ್ರತ್ಯಾಗಮ get_unicode_name(iter->keysym, buffer, size);
}

/* Iterate over the *assigned* keysyms.
 *
 * Use:
 *
 * ```c
 * struct xkb_keysym_iterator *iter = xkb_keysym_iterator_new(true);
 * while (xkb_keysym_iterator_next(iter)) {
 *   ...
 * }
 * iter = xkb_keysym_iterator_unref(iter);
 * ```
 */
bool
xkb_keysym_iterator_next(ಸಂರಚನಾ xkb_keysym_iterator *iter)
{
    ಯದಿ (iter->index >= (int32_t)ARRAY_SIZE(keysym_to_name) - 1)
        ಪ್ರತ್ಯಾಗಮ false;

    /* Next keysym */
    ಯದಿ (iter->explicit || iter->keysym >= XKB_KEYSYM_UNICODE_MAX ||
        keysym_to_name[iter->index + 1].keysym < XKB_KEYSYM_UNICODE_MIN) {
        /* Explicitly named keysyms only */
        iter->keysym = keysym_to_name[++iter->index].keysym;
        ಪ್ರತಿಜ್ಞಾ(iter->explicit ||
               iter->keysym <= XKB_KEYSYM_UNICODE_MIN ||
               iter->keysym >= XKB_KEYSYM_UNICODE_MAX);
    } ಅನ್ಯಥಾ {
        /* Unicode keysyms
         * NOTE: Unicode keysyms are within keysym_to_name keysyms range. */
        ಯದಿ (iter->keysym >= keysym_to_name[iter->index].keysym)
            iter->index++;
        ಯದಿ (iter->keysym >= XKB_KEYSYM_UNICODE_MIN) {
            /* Continue Unicode keysyms */
            iter->keysym++;
        } ಅನ್ಯಥಾ {
            /* Start Unicode keysyms */
            iter->keysym = XKB_KEYSYM_UNICODE_MIN;
        }
    }
    ಪ್ರತ್ಯಾಗಮ true;
}

/* Parse the numeric part of a 0xXXXX and UXXXX keysym. */
ಸ್ಥಿರ bool
parse_keysym_hex(ಧ್ರುವ ವರ್ಣ *s, uint32_t *out)
{
    /* We expect a NULL-terminated string of length up to 8 */
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = parse_hex_to_uint32_t(s, 8, out);
    /* Check that some value was parsed and we reached the end of the string */
    ಪ್ರತ್ಯಾಗಮ (ಎಣಿಕೆ > 0 && s[ಎಣಿಕೆ] == '\0');
}

xkb_keysym_t
xkb_keysym_from_name(ಧ್ರುವ ವರ್ಣ *name, ಗಣನಾ xkb_keysym_flags flags)
{
    ಯದಿ (flags & ~(ಗಣನಾ xkb_keysym_flags)XKB_KEYSYM_FLAGS_ಮೌಲ್ಯಗಳು)
        ಪ್ರತ್ಯಾಗಮ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;

    ಧ್ರುವ ಸಂರಚನಾ name_keysym *entry = ರಿಕ್ತ;
    ವರ್ಣ *tmp;
    uint32_t val;
    bool icase = (flags & XKB_KEYSYM_CASE_INSENSITIVE);

    /*
     * We need to !icase case to be fast, for e.g. Compose file parsing.
     * So do it in a fast path.
     */
    ಯದಿ (!icase) {
        size_t pos = keysym_name_perfect_hash(name);
        ಯದಿ (pos < ARRAY_SIZE(name_to_keysym)) {
            ಧ್ರುವ ವರ್ಣ *s = get_name(&name_to_keysym[pos]);
            ಯದಿ (strcmp(name, s) == 0)
                ಪ್ರತ್ಯಾಗಮ name_to_keysym[pos].keysym;
        }
    }
    /*
    * Find the correct keysym for case-insensitive match.
    *
    * The name_to_keysym table is sorted by istrcmp(). So the binary
    * search may return _any_ of all possible case-insensitive duplicates. The
    * duplicates are sorted so that the "best" case-insensitive match comes
    * last. So the code searches the entry and all next entries that match by
    * case-insensitive comparison and returns the "best" case-insensitive match.
    *
    * The "best" case-insensitive match is the lower-case keysym name. Most
    * keysyms names that only differ by letter-case are keysyms that are
    * available as “small” and “big” variants. For example:
    *
    * - Bicameral scripts: Lower-case and upper-case variants,
    *   e.g. KEY_a and KEY_A.
    * - Non-bicameral scripts: e.g. KEY_kana_a and KEY_kana_A.
    *
    * There are some exceptions, e.g. `XF86Screensaver` and `XF86ScreenSaver`.
    */
    ಅನ್ಯಥಾ {
        int32_t lo = 0, hi = ARRAY_SIZE(name_to_keysym) - 1;
        ಯಾವತ್ (hi >= lo) {
            int32_t mid = (lo + hi) / 2;
            ಪೂರ್ಣಾಙ್ಕ cmp = istrcmp(name, get_name(&name_to_keysym[mid]));
            ಯದಿ (cmp > 0) {
                lo = mid + 1;
            } ಅನ್ಯಥಾ ಯದಿ (cmp < 0) {
                hi = mid - 1;
            } ಅನ್ಯಥಾ {
                entry = &name_to_keysym[mid];
                ಭಙ್ಗ;
            }
        }
        ಯದಿ (entry) {
            ಧ್ರುವ ಸಂರಚನಾ name_keysym *last;
            last = name_to_keysym + ARRAY_SIZE(name_to_keysym) - 1;
            /* Keep going until we reach end of array
             * or non case-insensitve match */
            ಯಾವತ್ (entry < last &&
                   istrcmp(get_name(entry + 1), get_name(entry)) == 0) {
                entry++;
            }
            ಪ್ರತ್ಯಾಗಮ entry->keysym;
        }
    }

    ಯದಿ (*name == 'U' || (icase && *name == 'u')) {
        /* Parse Unicode notation */
        ಯದಿ (!parse_keysym_hex(&name[1], &val))
            ಪ್ರತ್ಯಾಗಮ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
        ಪ್ರತ್ಯಾಗಮ (val > 0xff && val <= 0x10ffff)
            /*
             * No normalization.
             *
             * NOTE: It allows surrogates, as we are dealing with Unicode
             * *code points* here, not Unicode *scalars*.
             */
            ? XKB_KEYSYM_UNICODE_OFFSET + val
            /*
             * Normalize ISO-8859-1 (Latin-1 + C0 and C1 control code)
             *
             * These code points require special processing to ensure
             * backward compatibility with legacy keysyms.
             */
            : xkb_utf32_to_keysym(val);
    }
    ಅನ್ಯಥಾ ಯದಿ (name[0] == '0' && (name[1] == 'x' || (icase && name[1] == 'X'))) {
        /*
         * Parse numeric hexadecimal notation without any normalization, in
         * in order to be consistent with the keymap files parsing.
         */
        ಯದಿ (!parse_keysym_hex(&name[2], &val) || val > XKB_KEYSYM_MAX)
            ಪ್ರತ್ಯಾಗಮ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
        ಪ್ರತ್ಯಾಗಮ (xkb_keysym_t) val;
    }

    /* Stupid inconsistency between the headers and XKeysymDB: the former has
     * no separating underscore, while some XF86* syms in the latter did.
     * As a last ditch effort, try without. */
    ಯದಿ (strncmp(name, "XF86_", 5) == 0 ||
        (icase && istrncmp(name, "XF86_", 5) == 0)) {
        xkb_keysym_t ret;
        tmp = strdup(name);
        ಯದಿ (!tmp)
            ಪ್ರತ್ಯಾಗಮ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
        memmove(&tmp[4], &tmp[5], strlen(name) - 5 + 1);
        ret = xkb_keysym_from_name(tmp, flags);
        ಮುಕ್ತ(tmp);
        ಪ್ರತ್ಯಾಗಮ ret;
    }

    ಪ್ರತ್ಯಾಗಮ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
}

xkb_keysym_t
xkb_utf8_to_keysym(ಧ್ರುವ ವರ್ಣ *buffer, size_t size)
{
    ಯದಿ (!buffer || !size)
        ಪ್ರತ್ಯಾಗಮ 0;

    /* Try to parse the parameter as a single UTF-8 encoded character */
    size_t length = 0;
    ಧ್ರುವ uint32_t codepoint = utf8_next_code_point(buffer, size, &length);
    ಪ್ರತ್ಯಾಗಮ (codepoint == INVALID_UTF8_CODE_POINT || length == 0)
        ? XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ
        : xkb_utf32_to_keysym(codepoint);
}

/*
 * Check whether a keysym with code "keysym" and name "name" is deprecated.
 * • If the keysym is not deprecated itself and has no deprecated names,
 *   then return false and write NULL in "reference_name".
 * • If there is a non-deprecated name for the given keysym, then write this
 *   name in "reference_name", else write NULL and return true.
 * • If "name" is NULL, then returns false: the keysym itself is not deprecated.
 * • If "name" is not NULL, then check if "name" is deprecated.
 *
 * WARNING: this function is unsafe because it does not test if "name" is
 * actually a correct name for "keysym". It is intended to be used just after
 * keysym resolution.
 */
bool
xkb_keysym_is_deprecated(xkb_keysym_t keysym,
                         ಧ್ರುವ ವರ್ಣ *name,
                         ಧ್ರುವ ವರ್ಣ **reference_name)
{
    ಯದಿ (keysym > XKB_KEYSYM_MAX) {
        /* Invalid keysym */
        *reference_name = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ false;
    }
    /* [WARNING] We do not check that name, if defined, is a valid for keysym */

    int32_t lo = 0, hi = ARRAY_SIZE(deprecated_keysyms) - 1;
    ಯಾವತ್ (hi >= lo) {
        int32_t mid = (lo + hi) / 2;
        ಯದಿ (keysym > deprecated_keysyms[mid].keysym) {
            lo = mid + 1;
        } ಅನ್ಯಥಾ ಯದಿ (keysym < deprecated_keysyms[mid].keysym) {
            hi = mid - 1;
        } ಅನ್ಯಥಾ {
            /* Keysym have some deprecated names */
            ಯದಿ (deprecated_keysyms[mid].offset == DEPRECATED_KEYSYM) {
                /* All names are deprecated */
                *reference_name = ರಿಕ್ತ;
                ಪ್ರತ್ಯಾಗಮ true;
            } ಅನ್ಯಥಾ ಯದಿ (deprecated_keysyms[mid].offset == UNICODE_KEYSYM) {
                /* All names are deprecated, but Unicode notation is valid */
                *reference_name = ರಿಕ್ತ;
                ಯದಿ (name == ರಿಕ್ತ)
                    ಪ್ರತ್ಯಾಗಮ false;
                ಯದಿ (name[0] != 'U')
                    ಪ್ರತ್ಯಾಗಮ true;
                /* Since the current function is internal, assume the name
                 * corresponds to the keysym value and just check its syntax. */
                ಧ್ರುವ ವರ್ಣ *start = name + 1;
                ಯಾವತ್ (is_xdigit(*start))
                    start++;
                ಪ್ರತ್ಯಾಗಮ *start != '\0' && start > name + 1;
            }
            /* There is a reference name that is not deprecated */
            *reference_name = get_name_by_index(deprecated_keysyms[mid].offset);
            ಯದಿ (!name) {
                /* No name to check: indicate not deprecated */
                ಪ್ರತ್ಯಾಗಮ false;
            }
            ಯದಿ (deprecated_keysyms[mid].explicit_ಎಣಿಕೆ) {
                /* Only some explicit names are deprecated */
                ಧ್ರುವ uint8_t k_max = deprecated_keysyms[mid].explicit_index
                                    + deprecated_keysyms[mid].explicit_ಎಣಿಕೆ;
                ಪ್ರತಿಜ್ಞಾ(k_max <= ARRAY_SIZE(explicit_deprecated_aliases));
                /* Check every deprecated alias */
                ಪ್ರತಿ (uint8_t k = deprecated_keysyms[mid].explicit_index;
                     k < k_max; k++) {
                    #ifdef __clang__
                    /* Make clang-tidy happy */
                    __builtin_assume(k_max <=
                                     ARRAY_SIZE(explicit_deprecated_aliases));
                    #endif
                    ಧ್ರುವ ವರ್ಣ *alias =
                        get_name_by_index(explicit_deprecated_aliases[k]);
                    ಯದಿ (strcmp(name, alias) == 0)
                        ಪ್ರತ್ಯಾಗಮ true;
                }
                ಪ್ರತ್ಯಾಗಮ false;
            } ಅನ್ಯಥಾ {
                /* All names but the reference one are deprecated */
                ಪ್ರತ್ಯಾಗಮ strcmp(name, *reference_name) != 0;
            }
        }
    }

    /* Keysym has no deprecated names */
    *reference_name = ರಿಕ್ತ;
    ಪ್ರತ್ಯಾಗಮ false;
}

bool
xkb_keysym_is_keypad(xkb_keysym_t keysym)
{
    ಪ್ರತ್ಯಾಗಮ keysym >= XKB_ಕೀಲಿ_KP_Space && keysym <= XKB_ಕೀಲಿ_KP_Equal;
}


bool
xkb_keysym_is_modifier(xkb_keysym_t keysym)
{
    ಪ್ರತ್ಯಾಗಮ
        (keysym >= XKB_ಕೀಲಿ_Shift_L && keysym <= XKB_ಕೀಲಿ_Hyper_R) ||
        (keysym >= XKB_ಕೀಲಿ_ISO_Lock && keysym <= XKB_ಕೀಲಿ_ISO_Level5_Lock) ||
        keysym == XKB_ಕೀಲಿ_Mode_switch ||
        keysym == XKB_ಕೀಲಿ_ಅಂಕೆ_Lock;
}
