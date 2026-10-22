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
ಕಂಡುಹಿಡಿ_keysym_ಸೂಚ್ಯಂಕ(xkb_keysym_t ks)
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
    ಆದರೆ (ks > XKB_KEYSYM_MAX_EXPLICIT)
        ಹಿಂತಿರುಗಿ -1;

    ssize_t lo = 0, hi = ARRAY_ಗಾತ್ರ(keysym_ಗಮ್ಯ_name) - 1;
    ಆಗ (hi >= lo) {
        ssize_t mid = (lo + hi) / 2;
        ಆದರೆ (ks > keysym_ಗಮ್ಯ_name[mid].keysym) {
            lo = mid + 1;
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ks < keysym_ಗಮ್ಯ_name[mid].keysym) {
            hi = mid - 1;
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಹಿಂತಿರುಗಿ mid;
        }
    }

    ಹಿಂತಿರುಗಿ -1;
}

#ವ್ಯಾಖ್ಯಾನಿಸು get_name_by_ಸೂಚ್ಯಂಕ(ಸೂಚ್ಯಂಕ) (keysym_ಹೆಸರುಗಳು + (ಸೂಚ್ಯಂಕ))

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
get_name(ಸ್ಥಿರಾಂಕ ರಚನೆ name_keysym *ನಮೂದು)
{
    ಹಿಂತಿರುಗಿ get_name_by_ಸೂಚ್ಯಂಕ(ನಮೂದು->ಸ್ಥಾನಾಂತರ);
}

/* Unnamed Unicode codepoint. */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
get_unicode_name(xkb_keysym_t ks, ಅಕ್ಷರ *ಬಫರ್, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ಹಿಂತಿರುಗಿ snprintf(ಬಫರ್, ಗಾತ್ರ_2, "U%04"PRIX32, ks & UINT32_C(0xffffff));
}

ಪೂರ್ಣಾಂಕ
xkb_keysym_get_name(xkb_keysym_t ks, ಅಕ್ಷರ *ಬಫರ್, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ಆದರೆ (ks > XKB_KEYSYM_MAX) {
        snprintf(ಬಫರ್, ಗಾತ್ರ_2, "Invalid");
        ಹಿಂತಿರುಗಿ -1;
    }

    ssize_t ಸೂಚ್ಯಂಕ = ಕಂಡುಹಿಡಿ_keysym_ಸೂಚ್ಯಂಕ(ks);
    ಆದರೆ (ಸೂಚ್ಯಂಕ != -1)
        ಹಿಂತಿರುಗಿ snprintf(ಬಫರ್, ಗಾತ್ರ_2, "%s", get_name(&keysym_ಗಮ್ಯ_name[ಸೂಚ್ಯಂಕ]));

    /*
     * Unnamed Unicode codepoint.
     * • Keysyms in the range [XKB_KEYSYM_UNICODE_OFFSET, XKB_KEYSYM_UNICODE_MIN [
     *   do not use the Unicode notation for backward compatibility.
     * • Keysyms are not normalized: e.g. given a Unicode keysym, calling
     *   `xkb_utf32_to_keysym` with the corresponding code point may return a
     *   different keysym or fail (e.g. surrogates are invalid in UTF-32).
     */
    ಆದರೆ (ks >= XKB_KEYSYM_UNICODE_MIN && ks <= XKB_KEYSYM_UNICODE_MAX)
        ಹಿಂತಿರುಗಿ get_unicode_name(ks, ಬಫರ್, ಗಾತ್ರ_2);

    /* Unnamed, non-Unicode, symbol (shouldn't generally happen). */
    ಹಿಂತಿರುಗಿ snprintf(ಬಫರ್, ಗಾತ್ರ_2, "0x%08"PRIx32, ks);
}

bool
xkb_keysym_is_assigned(xkb_keysym_t ks)
{
    ಹಿಂತಿರುಗಿ (XKB_KEYSYM_UNICODE_MIN <= ks && ks <= XKB_KEYSYM_UNICODE_MAX) ||
           ಕಂಡುಹಿಡಿ_keysym_ಸೂಚ್ಯಂಕ(ks) != -1;
}

ಪೂರ್ಣಾಂಕ
xkb_keysym_get_explicit_ಹೆಸರುಗಳು(xkb_keysym_t ks, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಬಫರ್, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ಆದರೆ (ks > XKB_KEYSYM_MAX)
        ಹಿಂತಿರುಗಿ -1;
    ಸ್ಥಿರಾಂಕ ssize_t ಸೂಚ್ಯಂಕ = ಕಂಡುಹಿಡಿ_keysym_ಸೂಚ್ಯಂಕ(ks);
    ಆದರೆ (ಸೂಚ್ಯಂಕ < 0)
        ಹಿಂತಿರುಗಿ 0;
    ಸ್ಥಿರಾಂಕ uint16_t canonical = keysym_ಗಮ್ಯ_name[ಸೂಚ್ಯಂಕ].ಸ್ಥಾನಾಂತರ;
    ಆದರೆ (ಗಾತ್ರ_2 > 0)
        ಬಫರ್[0] = get_name(&keysym_ಗಮ್ಯ_name[ಸೂಚ್ಯಂಕ]);
    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 1;
    ಗಾಗಿ (ಗಾತ್ರ_t pos = 0; pos < ARRAY_ಗಾತ್ರ(name_ಗಮ್ಯ_keysym); pos++) {
        ಆದರೆ (name_ಗಮ್ಯ_keysym[pos].keysym == ks &&
            name_ಗಮ್ಯ_keysym[pos].ಸ್ಥಾನಾಂತರ != canonical) {
            ಆದರೆ ((ಗಾತ್ರ_t) ಎಣಿಕೆ_2 < ಗಾತ್ರ_2) {
                ಬಫರ್[ಎಣಿಕೆ_2] = get_name(&name_ಗಮ್ಯ_keysym[pos]);
            }
            ಎಣಿಕೆ_2++;
        }
    }
    ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_2;
}

ರಚನೆ xkb_keysym_iterator {
    bool explicit;       /* If true, traverse only explicitly named keysyms */
    int32_t ಸೂಚ್ಯಂಕ;       /* Current index in `keysym_to_name` */
    xkb_keysym_t keysym; /* Current keysym */
};

ರಚನೆ xkb_keysym_iterator*
xkb_keysym_iterator_new(bool iterate_only_explicit_keysyms)
{
    ರಚನೆ xkb_keysym_iterator* iter = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(*iter));
    ಆದರೆ (!iter)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    iter->explicit = iterate_only_explicit_keysyms;
    iter->ಸೂಚ್ಯಂಕ = -1;
    iter->keysym = XKB_KEYSYM_UNICODE_MAX;
    ಹಿಂತಿರುಗಿ iter;
}

ರಚನೆ xkb_keysym_iterator*
xkb_keysym_iterator_unref(ರಚನೆ xkb_keysym_iterator *iter)
{
    /*
     * [NOTE] If we ever make this API public, add:
     * - an `refcnt` member,
     * - `xkb_keysym_iterator_ref()`,
     * - `assert(!iter || iter->refcnt > 0)`.
     */
    ಆದರೆ (iter)
        ಬಿಡುಗಡೆ(iter);
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

xkb_keysym_t
xkb_keysym_iterator_get_keysym(ರಚನೆ xkb_keysym_iterator *iter)
{
    ಹಿಂತಿರುಗಿ iter->keysym;
}

bool
xkb_keysym_iterator_is_explicitly_named(ರಚನೆ xkb_keysym_iterator *iter)
{
    ಹಿಂತಿರುಗಿ iter->ಸೂಚ್ಯಂಕ >= 0 &&
           iter->ಸೂಚ್ಯಂಕ < (int32_t)ARRAY_ಗಾತ್ರ(keysym_ಗಮ್ಯ_name) &&
           (iter->explicit ||
            iter->keysym == keysym_ಗಮ್ಯ_name[iter->ಸೂಚ್ಯಂಕ].keysym);
}

ಪೂರ್ಣಾಂಕ
xkb_keysym_iterator_get_name(ರಚನೆ xkb_keysym_iterator *iter,
                             ಅಕ್ಷರ *ಬಫರ್, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ಆದರೆ (iter->ಸೂಚ್ಯಂಕ < 0 || iter->ಸೂಚ್ಯಂಕ >= (int32_t)ARRAY_ಗಾತ್ರ(keysym_ಗಮ್ಯ_name))
        ಹಿಂತಿರುಗಿ -1;
    ಆದರೆ (iter->explicit || iter->keysym == keysym_ಗಮ್ಯ_name[iter->ಸೂಚ್ಯಂಕ].keysym)
        ಹಿಂತಿರುಗಿ snprintf(ಬಫರ್, ಗಾತ್ರ_2, "%s",
                        get_name(&keysym_ಗಮ್ಯ_name[iter->ಸೂಚ್ಯಂಕ]));
    ಹಿಂತಿರುಗಿ get_unicode_name(iter->keysym, ಬಫರ್, ಗಾತ್ರ_2);
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
xkb_keysym_iterator_ಮುಂದಿನ(ರಚನೆ xkb_keysym_iterator *iter)
{
    ಆದರೆ (iter->ಸೂಚ್ಯಂಕ >= (int32_t)ARRAY_ಗಾತ್ರ(keysym_ಗಮ್ಯ_name) - 1)
        ಹಿಂತಿರುಗಿ false;

    /* Next keysym */
    ಆದರೆ (iter->explicit || iter->keysym >= XKB_KEYSYM_UNICODE_MAX ||
        keysym_ಗಮ್ಯ_name[iter->ಸೂಚ್ಯಂಕ + 1].keysym < XKB_KEYSYM_UNICODE_MIN) {
        /* Explicitly named keysyms only */
        iter->keysym = keysym_ಗಮ್ಯ_name[++iter->ಸೂಚ್ಯಂಕ].keysym;
        ಪ್ರತಿಜ್ಞಾ(iter->explicit ||
               iter->keysym <= XKB_KEYSYM_UNICODE_MIN ||
               iter->keysym >= XKB_KEYSYM_UNICODE_MAX);
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* Unicode keysyms
         * NOTE: Unicode keysyms are within keysym_to_name keysyms range. */
        ಆದರೆ (iter->keysym >= keysym_ಗಮ್ಯ_name[iter->ಸೂಚ್ಯಂಕ].keysym)
            iter->ಸೂಚ್ಯಂಕ++;
        ಆದರೆ (iter->keysym >= XKB_KEYSYM_UNICODE_MIN) {
            /* Continue Unicode keysyms */
            iter->keysym++;
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* Start Unicode keysyms */
            iter->keysym = XKB_KEYSYM_UNICODE_MIN;
        }
    }
    ಹಿಂತಿರುಗಿ true;
}

/* Parse the numeric part of a 0xXXXX and UXXXX keysym. */
ಸ್ಥಿರ bool
parse_keysym_hex(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, uint32_t *ಹೊರಹೋಗು)
{
    /* We expect a NULL-terminated string of length up to 8 */
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = parse_hex_ಗಮ್ಯ_uint32_t(s, 8, ಹೊರಹೋಗು);
    /* Check that some value was parsed and we reached the end of the string */
    ಹಿಂತಿರುಗಿ (ಎಣಿಕೆ_2 > 0 && s[ಎಣಿಕೆ_2] == '\0');
}

xkb_keysym_t
xkb_keysym_from_name(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಎಣಿಕೆ xkb_keysym_flags flags)
{
    ಆದರೆ (flags & ~(ಎಣಿಕೆ xkb_keysym_flags)XKB_KEYSYM_FLAGS_ಮೌಲ್ಯಗಳು)
        ಹಿಂತಿರುಗಿ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;

    ಸ್ಥಿರಾಂಕ ರಚನೆ name_keysym *ನಮೂದು = ಖಾಲಿ;
    ಅಕ್ಷರ *tmp;
    uint32_t val;
    bool icase = (flags & XKB_KEYSYM_CASE_INSENSITIVE);

    /*
     * We need to !icase case to be fast, for e.g. Compose file parsing.
     * So do it in a fast path.
     */
    ಆದರೆ (!icase) {
        ಗಾತ್ರ_t pos = keysym_name_perfect_hash(name);
        ಆದರೆ (pos < ARRAY_ಗಾತ್ರ(name_ಗಮ್ಯ_keysym)) {
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s = get_name(&name_ಗಮ್ಯ_keysym[pos]);
            ಆದರೆ (strcmp(name, s) == 0)
                ಹಿಂತಿರುಗಿ name_ಗಮ್ಯ_keysym[pos].keysym;
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
    ಇಲ್ಲದಿದ್ದರೆ {
        int32_t lo = 0, hi = ARRAY_ಗಾತ್ರ(name_ಗಮ್ಯ_keysym) - 1;
        ಆಗ (hi >= lo) {
            int32_t mid = (lo + hi) / 2;
            ಪೂರ್ಣಾಂಕ cmp = istrcmp(name, get_name(&name_ಗಮ್ಯ_keysym[mid]));
            ಆದರೆ (cmp > 0) {
                lo = mid + 1;
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cmp < 0) {
                hi = mid - 1;
            } ಇಲ್ಲದಿದ್ದರೆ {
                ನಮೂದು = &name_ಗಮ್ಯ_keysym[mid];
                ನಿಲ್ಲಿಸು;
            }
        }
        ಆದರೆ (ನಮೂದು) {
            ಸ್ಥಿರಾಂಕ ರಚನೆ name_keysym *ಕೊನೆಯ;
            ಕೊನೆಯ = name_ಗಮ್ಯ_keysym + ARRAY_ಗಾತ್ರ(name_ಗಮ್ಯ_keysym) - 1;
            /* Keep going until we reach end of array
             * or non case-insensitve match */
            ಆಗ (ನಮೂದು < ಕೊನೆಯ &&
                   istrcmp(get_name(ನಮೂದು + 1), get_name(ನಮೂದು)) == 0) {
                ನಮೂದು++;
            }
            ಹಿಂತಿರುಗಿ ನಮೂದು->keysym;
        }
    }

    ಆದರೆ (*name == 'U' || (icase && *name == 'u')) {
        /* Parse Unicode notation */
        ಆದರೆ (!parse_keysym_hex(&name[1], &val))
            ಹಿಂತಿರುಗಿ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
        ಹಿಂತಿರುಗಿ (val > 0xff && val <= 0x10ffff)
            /*
             * No normalization.
             *
             * NOTE: It allows surrogates, as we are dealing with Unicode
             * *code points* here, not Unicode *scalars*.
             */
            ? XKB_KEYSYM_UNICODE_ಸ್ಥಾನಾಂತರ + val
            /*
             * Normalize ISO-8859-1 (Latin-1 + C0 and C1 control code)
             *
             * These code points require special processing to ensure
             * backward compatibility with legacy keysyms.
             */
            : xkb_utf32_ಗಮ್ಯ_keysym(val);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (name[0] == '0' && (name[1] == 'x' || (icase && name[1] == 'X'))) {
        /*
         * Parse numeric hexadecimal notation without any normalization, in
         * in order to be consistent with the keymap files parsing.
         */
        ಆದರೆ (!parse_keysym_hex(&name[2], &val) || val > XKB_KEYSYM_MAX)
            ಹಿಂತಿರುಗಿ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
        ಹಿಂತಿರುಗಿ (xkb_keysym_t) val;
    }

    /* Stupid inconsistency between the headers and XKeysymDB: the former has
     * no separating underscore, while some XF86* syms in the latter did.
     * As a last ditch effort, try without. */
    ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(name, "XF86_", 5) == 0 ||
        (icase && istrncmp(name, "XF86_", 5) == 0)) {
        xkb_keysym_t ret;
        tmp = strdup(name);
        ಆದರೆ (!tmp)
            ಹಿಂತಿರುಗಿ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
        memmove(&tmp[4], &tmp[5], ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name) - 5 + 1);
        ret = xkb_keysym_from_name(tmp, flags);
        ಬಿಡುಗಡೆ(tmp);
        ಹಿಂತಿರುಗಿ ret;
    }

    ಹಿಂತಿರುಗಿ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
}

xkb_keysym_t
xkb_utf8_ಗಮ್ಯ_keysym(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಫರ್, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ಆದರೆ (!ಬಫರ್ || !ಗಾತ್ರ_2)
        ಹಿಂತಿರುಗಿ 0;

    /* Try to parse the parameter as a single UTF-8 encoded character */
    ಗಾತ್ರ_t ಉದ್ದಳತೆ = 0;
    ಸ್ಥಿರಾಂಕ uint32_t codepoint = utf8_ಮುಂದಿನ_ಸಂಕೇತ_ಬಿಂದು(ಬಫರ್, ಗಾತ್ರ_2, &ಉದ್ದಳತೆ);
    ಹಿಂತಿರುಗಿ (codepoint == INVALID_UTF8_ಸಂಕೇತ_ಬಿಂದು || ಉದ್ದಳತೆ == 0)
        ? XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ
        : xkb_utf32_ಗಮ್ಯ_keysym(codepoint);
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
                         ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
                         ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **reference_name)
{
    ಆದರೆ (keysym > XKB_KEYSYM_MAX) {
        /* Invalid keysym */
        *reference_name = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ false;
    }
    /* [WARNING] We do not check that name, if defined, is a valid for keysym */

    int32_t lo = 0, hi = ARRAY_ಗಾತ್ರ(deprecated_keysyms) - 1;
    ಆಗ (hi >= lo) {
        int32_t mid = (lo + hi) / 2;
        ಆದರೆ (keysym > deprecated_keysyms[mid].keysym) {
            lo = mid + 1;
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (keysym < deprecated_keysyms[mid].keysym) {
            hi = mid - 1;
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* Keysym have some deprecated names */
            ಆದರೆ (deprecated_keysyms[mid].ಸ್ಥಾನಾಂತರ == DEPRECATED_KEYSYM) {
                /* All names are deprecated */
                *reference_name = ಖಾಲಿ;
                ಹಿಂತಿರುಗಿ true;
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (deprecated_keysyms[mid].ಸ್ಥಾನಾಂತರ == UNICODE_KEYSYM) {
                /* All names are deprecated, but Unicode notation is valid */
                *reference_name = ಖಾಲಿ;
                ಆದರೆ (name == ಖಾಲಿ)
                    ಹಿಂತಿರುಗಿ false;
                ಆದರೆ (name[0] != 'U')
                    ಹಿಂತಿರುಗಿ true;
                /* Since the current function is internal, assume the name
                 * corresponds to the keysym value and just check its syntax. */
                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಆರಂಭ = name + 1;
                ಆಗ (is_xdigit(*ಆರಂಭ))
                    ಆರಂಭ++;
                ಹಿಂತಿರುಗಿ *ಆರಂಭ != '\0' && ಆರಂಭ > name + 1;
            }
            /* There is a reference name that is not deprecated */
            *reference_name = get_name_by_ಸೂಚ್ಯಂಕ(deprecated_keysyms[mid].ಸ್ಥಾನಾಂತರ);
            ಆದರೆ (!name) {
                /* No name to check: indicate not deprecated */
                ಹಿಂತಿರುಗಿ false;
            }
            ಆದರೆ (deprecated_keysyms[mid].explicit_ಎಣಿಕೆ) {
                /* Only some explicit names are deprecated */
                ಸ್ಥಿರಾಂಕ uint8_t k_max = deprecated_keysyms[mid].explicit_ಸೂಚ್ಯಂಕ
                                    + deprecated_keysyms[mid].explicit_ಎಣಿಕೆ;
                ಪ್ರತಿಜ್ಞಾ(k_max <= ARRAY_ಗಾತ್ರ(explicit_deprecated_aliases));
                /* Check every deprecated alias */
                ಗಾಗಿ (uint8_t k = deprecated_keysyms[mid].explicit_ಸೂಚ್ಯಂಕ;
                     k < k_max; k++) {
                    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __clang__
                    /* Make clang-tidy happy */
                    __builtin_assume(k_max <=
                                     ARRAY_ಗಾತ್ರ(explicit_deprecated_aliases));
                    #ಆದರೆ_ಅಂತ್ಯ
                    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *alias =
                        get_name_by_ಸೂಚ್ಯಂಕ(explicit_deprecated_aliases[k]);
                    ಆದರೆ (strcmp(name, alias) == 0)
                        ಹಿಂತಿರುಗಿ true;
                }
                ಹಿಂತಿರುಗಿ false;
            } ಇಲ್ಲದಿದ್ದರೆ {
                /* All names but the reference one are deprecated */
                ಹಿಂತಿರುಗಿ strcmp(name, *reference_name) != 0;
            }
        }
    }

    /* Keysym has no deprecated names */
    *reference_name = ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ false;
}

bool
xkb_keysym_is_keypad(xkb_keysym_t keysym)
{
    ಹಿಂತಿರುಗಿ keysym >= XKB_ಕೀಲಿ_KP_Space && keysym <= XKB_ಕೀಲಿ_KP_Equal;
}


bool
xkb_keysym_is_modifier(xkb_keysym_t keysym)
{
    ಹಿಂತಿರುಗಿ
        (keysym >= XKB_ಕೀಲಿ_ಸರಿಸು_L && keysym <= XKB_ಕೀಲಿ_Hyper_R) ||
        (keysym >= XKB_ಕೀಲಿ_ISO_ಬೀಗ && keysym <= XKB_ಕೀಲಿ_ISO_Level5_ಬೀಗ) ||
        keysym == XKB_ಕೀಲಿ_ಬಹುಲಕ_switch ||
        keysym == XKB_ಕೀಲಿ_ಅಂಕೆ_ಬೀಗ;
}
