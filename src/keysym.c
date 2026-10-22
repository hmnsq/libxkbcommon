/*
 * For MIT-open-group:
 * Copyright 1985, 1987, 1990, 1998  The Open Group
 *
 * For MIT:
 * Copyright © 2009 Dan Nicholson
 *
 * SPDX-License-Identifier: MIT-open-group AND MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon-keysyms.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "features/enums.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keysym.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keysym-names.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utf8-decoding.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-numbers.h"

ਸਥਿਰ ssize_t
ਲੱਭੋ_keysym_ਸੂਚਕਾਂਕ(xkb_keysym_t ks)
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
    ਜੇ (ks > XKB_KEYSYM_MAX_EXPLICIT)
        ਵਾਪਸ -1;

    ssize_t lo = 0, hi = ARRAY_ਮਾਪ(keysym_ਮੰਜ਼ਿਲ_name) - 1;
    ਜਦੋਂ (hi >= lo) {
        ssize_t mid = (lo + hi) / 2;
        ਜੇ (ks > keysym_ਮੰਜ਼ਿਲ_name[mid].keysym) {
            lo = mid + 1;
        } ਨਹੀਂਤਾਂ ਜੇ (ks < keysym_ਮੰਜ਼ਿਲ_name[mid].keysym) {
            hi = mid - 1;
        } ਨਹੀਂਤਾਂ {
            ਵਾਪਸ mid;
        }
    }

    ਵਾਪਸ -1;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ get_name_by_ਸੂਚਕਾਂਕ(ਸੂਚਕਾਂਕ) (keysym_ਨਾਂ + (ਸੂਚਕਾਂਕ))

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ ਅੱਖਰ *
get_name(ਅਚਲ ਬਣਤਰ name_keysym *ਇੰਦਰਾਜ)
{
    ਵਾਪਸ get_name_by_ਸੂਚਕਾਂਕ(ਇੰਦਰਾਜ->ਵਿੱਥ);
}

/* Unnamed Unicode codepoint. */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
get_unicode_name(xkb_keysym_t ks, ਅੱਖਰ *ਬਫਰ, ਮਾਪ_t ਮਾਪ)
{
    ਵਾਪਸ snprintf(ਬਫਰ, ਮਾਪ, "U%04"PRIX32, ks & UINT32_C(0xffffff));
}

ਪੂਰਨ
xkb_keysym_get_name(xkb_keysym_t ks, ਅੱਖਰ *ਬਫਰ, ਮਾਪ_t ਮਾਪ)
{
    ਜੇ (ks > XKB_KEYSYM_MAX) {
        snprintf(ਬਫਰ, ਮਾਪ, "Invalid");
        ਵਾਪਸ -1;
    }

    ssize_t ਸੂਚਕਾਂਕ = ਲੱਭੋ_keysym_ਸੂਚਕਾਂਕ(ks);
    ਜੇ (ਸੂਚਕਾਂਕ != -1)
        ਵਾਪਸ snprintf(ਬਫਰ, ਮਾਪ, "%s", get_name(&keysym_ਮੰਜ਼ਿਲ_name[ਸੂਚਕਾਂਕ]));

    /*
     * Unnamed Unicode codepoint.
     * • Keysyms in the range [XKB_KEYSYM_UNICODE_OFFSET, XKB_KEYSYM_UNICODE_MIN [
     *   do not use the Unicode notation for backward compatibility.
     * • Keysyms are not normalized: e.g. given a Unicode keysym, calling
     *   `xkb_utf32_to_keysym` with the corresponding code point may return a
     *   different keysym or fail (e.g. surrogates are invalid in UTF-32).
     */
    ਜੇ (ks >= XKB_KEYSYM_UNICODE_MIN && ks <= XKB_KEYSYM_UNICODE_MAX)
        ਵਾਪਸ get_unicode_name(ks, ਬਫਰ, ਮਾਪ);

    /* Unnamed, non-Unicode, symbol (shouldn't generally happen). */
    ਵਾਪਸ snprintf(ਬਫਰ, ਮਾਪ, "0x%08"PRIx32, ks);
}

bool
xkb_keysym_is_assigned(xkb_keysym_t ks)
{
    ਵਾਪਸ (XKB_KEYSYM_UNICODE_MIN <= ks && ks <= XKB_KEYSYM_UNICODE_MAX) ||
           ਲੱਭੋ_keysym_ਸੂਚਕਾਂਕ(ks) != -1;
}

ਪੂਰਨ
xkb_keysym_get_explicit_ਨਾਂ(xkb_keysym_t ks, ਅਚਲ ਅੱਖਰ **ਬਫਰ, ਮਾਪ_t ਮਾਪ)
{
    ਜੇ (ks > XKB_KEYSYM_MAX)
        ਵਾਪਸ -1;
    ਅਚਲ ssize_t ਸੂਚਕਾਂਕ = ਲੱਭੋ_keysym_ਸੂਚਕਾਂਕ(ks);
    ਜੇ (ਸੂਚਕਾਂਕ < 0)
        ਵਾਪਸ 0;
    ਅਚਲ uint16_t canonical = keysym_ਮੰਜ਼ਿਲ_name[ਸੂਚਕਾਂਕ].ਵਿੱਥ;
    ਜੇ (ਮਾਪ > 0)
        ਬਫਰ[0] = get_name(&keysym_ਮੰਜ਼ਿਲ_name[ਸੂਚਕਾਂਕ]);
    ਪੂਰਨ ਗਿਣਤੀ_2 = 1;
    ਲਈ (ਮਾਪ_t pos = 0; pos < ARRAY_ਮਾਪ(name_ਮੰਜ਼ਿਲ_keysym); pos++) {
        ਜੇ (name_ਮੰਜ਼ਿਲ_keysym[pos].keysym == ks &&
            name_ਮੰਜ਼ਿਲ_keysym[pos].ਵਿੱਥ != canonical) {
            ਜੇ ((ਮਾਪ_t) ਗਿਣਤੀ_2 < ਮਾਪ) {
                ਬਫਰ[ਗਿਣਤੀ_2] = get_name(&name_ਮੰਜ਼ਿਲ_keysym[pos]);
            }
            ਗਿਣਤੀ_2++;
        }
    }
    ਵਾਪਸ ਗਿਣਤੀ_2;
}

ਬਣਤਰ xkb_keysym_iterator {
    bool explicit;       /* If true, traverse only explicitly named keysyms */
    int32_t ਸੂਚਕਾਂਕ;       /* Current index in `keysym_to_name` */
    xkb_keysym_t keysym; /* Current keysym */
};

ਬਣਤਰ xkb_keysym_iterator*
xkb_keysym_iterator_new(bool iterate_only_explicit_keysyms)
{
    ਬਣਤਰ xkb_keysym_iterator* iter = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(*iter));
    ਜੇ (!iter)
        ਵਾਪਸ ਨਿਰਾ;
    iter->explicit = iterate_only_explicit_keysyms;
    iter->ਸੂਚਕਾਂਕ = -1;
    iter->keysym = XKB_KEYSYM_UNICODE_MAX;
    ਵਾਪਸ iter;
}

ਬਣਤਰ xkb_keysym_iterator*
xkb_keysym_iterator_unref(ਬਣਤਰ xkb_keysym_iterator *iter)
{
    /*
     * [NOTE] If we ever make this API public, add:
     * - an `refcnt` member,
     * - `xkb_keysym_iterator_ref()`,
     * - `assert(!iter || iter->refcnt > 0)`.
     */
    ਜੇ (iter)
        ਮੁਕਤ(iter);
    ਵਾਪਸ ਨਿਰਾ;
}

xkb_keysym_t
xkb_keysym_iterator_get_keysym(ਬਣਤਰ xkb_keysym_iterator *iter)
{
    ਵਾਪਸ iter->keysym;
}

bool
xkb_keysym_iterator_is_explicitly_named(ਬਣਤਰ xkb_keysym_iterator *iter)
{
    ਵਾਪਸ iter->ਸੂਚਕਾਂਕ >= 0 &&
           iter->ਸੂਚਕਾਂਕ < (int32_t)ARRAY_ਮਾਪ(keysym_ਮੰਜ਼ਿਲ_name) &&
           (iter->explicit ||
            iter->keysym == keysym_ਮੰਜ਼ਿਲ_name[iter->ਸੂਚਕਾਂਕ].keysym);
}

ਪੂਰਨ
xkb_keysym_iterator_get_name(ਬਣਤਰ xkb_keysym_iterator *iter,
                             ਅੱਖਰ *ਬਫਰ, ਮਾਪ_t ਮਾਪ)
{
    ਜੇ (iter->ਸੂਚਕਾਂਕ < 0 || iter->ਸੂਚਕਾਂਕ >= (int32_t)ARRAY_ਮਾਪ(keysym_ਮੰਜ਼ਿਲ_name))
        ਵਾਪਸ -1;
    ਜੇ (iter->explicit || iter->keysym == keysym_ਮੰਜ਼ਿਲ_name[iter->ਸੂਚਕਾਂਕ].keysym)
        ਵਾਪਸ snprintf(ਬਫਰ, ਮਾਪ, "%s",
                        get_name(&keysym_ਮੰਜ਼ਿਲ_name[iter->ਸੂਚਕਾਂਕ]));
    ਵਾਪਸ get_unicode_name(iter->keysym, ਬਫਰ, ਮਾਪ);
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
xkb_keysym_iterator_ਅਗਲਾ(ਬਣਤਰ xkb_keysym_iterator *iter)
{
    ਜੇ (iter->ਸੂਚਕਾਂਕ >= (int32_t)ARRAY_ਮਾਪ(keysym_ਮੰਜ਼ਿਲ_name) - 1)
        ਵਾਪਸ false;

    /* Next keysym */
    ਜੇ (iter->explicit || iter->keysym >= XKB_KEYSYM_UNICODE_MAX ||
        keysym_ਮੰਜ਼ਿਲ_name[iter->ਸੂਚਕਾਂਕ + 1].keysym < XKB_KEYSYM_UNICODE_MIN) {
        /* Explicitly named keysyms only */
        iter->keysym = keysym_ਮੰਜ਼ਿਲ_name[++iter->ਸੂਚਕਾਂਕ].keysym;
        ਪ੍ਰਤਿਜ੍ਞਾ(iter->explicit ||
               iter->keysym <= XKB_KEYSYM_UNICODE_MIN ||
               iter->keysym >= XKB_KEYSYM_UNICODE_MAX);
    } ਨਹੀਂਤਾਂ {
        /* Unicode keysyms
         * NOTE: Unicode keysyms are within keysym_to_name keysyms range. */
        ਜੇ (iter->keysym >= keysym_ਮੰਜ਼ਿਲ_name[iter->ਸੂਚਕਾਂਕ].keysym)
            iter->ਸੂਚਕਾਂਕ++;
        ਜੇ (iter->keysym >= XKB_KEYSYM_UNICODE_MIN) {
            /* Continue Unicode keysyms */
            iter->keysym++;
        } ਨਹੀਂਤਾਂ {
            /* Start Unicode keysyms */
            iter->keysym = XKB_KEYSYM_UNICODE_MIN;
        }
    }
    ਵਾਪਸ true;
}

/* Parse the numeric part of a 0xXXXX and UXXXX keysym. */
ਸਥਿਰ bool
parse_keysym_hex(ਅਚਲ ਅੱਖਰ *s, uint32_t *ਬਾਹਰਲਾ)
{
    /* We expect a NULL-terminated string of length up to 8 */
    ਅਚਲ ਪੂਰਨ ਗਿਣਤੀ_2 = parse_hex_ਮੰਜ਼ਿਲ_uint32_t(s, 8, ਬਾਹਰਲਾ);
    /* Check that some value was parsed and we reached the end of the string */
    ਵਾਪਸ (ਗਿਣਤੀ_2 > 0 && s[ਗਿਣਤੀ_2] == '\0');
}

xkb_keysym_t
xkb_keysym_from_name(ਅਚਲ ਅੱਖਰ *name, ਗਿਣਤੀ xkb_keysym_flags flags)
{
    ਜੇ (flags & ~(ਗਿਣਤੀ xkb_keysym_flags)XKB_KEYSYM_FLAGS_ਮੁੱਲਾਂ)
        ਵਾਪਸ XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;

    ਅਚਲ ਬਣਤਰ name_keysym *ਇੰਦਰਾਜ = ਨਿਰਾ;
    ਅੱਖਰ *tmp;
    uint32_t val;
    bool icase = (flags & XKB_KEYSYM_CASE_INSENSITIVE);

    /*
     * We need to !icase case to be fast, for e.g. Compose file parsing.
     * So do it in a fast path.
     */
    ਜੇ (!icase) {
        ਮਾਪ_t pos = keysym_name_perfect_hash(name);
        ਜੇ (pos < ARRAY_ਮਾਪ(name_ਮੰਜ਼ਿਲ_keysym)) {
            ਅਚਲ ਅੱਖਰ *s = get_name(&name_ਮੰਜ਼ਿਲ_keysym[pos]);
            ਜੇ (strcmp(name, s) == 0)
                ਵਾਪਸ name_ਮੰਜ਼ਿਲ_keysym[pos].keysym;
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
    ਨਹੀਂਤਾਂ {
        int32_t lo = 0, hi = ARRAY_ਮਾਪ(name_ਮੰਜ਼ਿਲ_keysym) - 1;
        ਜਦੋਂ (hi >= lo) {
            int32_t mid = (lo + hi) / 2;
            ਪੂਰਨ cmp = istrcmp(name, get_name(&name_ਮੰਜ਼ਿਲ_keysym[mid]));
            ਜੇ (cmp > 0) {
                lo = mid + 1;
            } ਨਹੀਂਤਾਂ ਜੇ (cmp < 0) {
                hi = mid - 1;
            } ਨਹੀਂਤਾਂ {
                ਇੰਦਰਾਜ = &name_ਮੰਜ਼ਿਲ_keysym[mid];
                ਰੋਕੋ;
            }
        }
        ਜੇ (ਇੰਦਰਾਜ) {
            ਅਚਲ ਬਣਤਰ name_keysym *ਆਖਰੀ;
            ਆਖਰੀ = name_ਮੰਜ਼ਿਲ_keysym + ARRAY_ਮਾਪ(name_ਮੰਜ਼ਿਲ_keysym) - 1;
            /* Keep going until we reach end of array
             * or non case-insensitve match */
            ਜਦੋਂ (ਇੰਦਰਾਜ < ਆਖਰੀ &&
                   istrcmp(get_name(ਇੰਦਰਾਜ + 1), get_name(ਇੰਦਰਾਜ)) == 0) {
                ਇੰਦਰਾਜ++;
            }
            ਵਾਪਸ ਇੰਦਰਾਜ->keysym;
        }
    }

    ਜੇ (*name == 'U' || (icase && *name == 'u')) {
        /* Parse Unicode notation */
        ਜੇ (!parse_keysym_hex(&name[1], &val))
            ਵਾਪਸ XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
        ਵਾਪਸ (val > 0xff && val <= 0x10ffff)
            /*
             * No normalization.
             *
             * NOTE: It allows surrogates, as we are dealing with Unicode
             * *code points* here, not Unicode *scalars*.
             */
            ? XKB_KEYSYM_UNICODE_ਵਿੱਥ + val
            /*
             * Normalize ISO-8859-1 (Latin-1 + C0 and C1 control code)
             *
             * These code points require special processing to ensure
             * backward compatibility with legacy keysyms.
             */
            : xkb_utf32_ਮੰਜ਼ਿਲ_keysym(val);
    }
    ਨਹੀਂਤਾਂ ਜੇ (name[0] == '0' && (name[1] == 'x' || (icase && name[1] == 'X'))) {
        /*
         * Parse numeric hexadecimal notation without any normalization, in
         * in order to be consistent with the keymap files parsing.
         */
        ਜੇ (!parse_keysym_hex(&name[2], &val) || val > XKB_KEYSYM_MAX)
            ਵਾਪਸ XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
        ਵਾਪਸ (xkb_keysym_t) val;
    }

    /* Stupid inconsistency between the headers and XKeysymDB: the former has
     * no separating underscore, while some XF86* syms in the latter did.
     * As a last ditch effort, try without. */
    ਜੇ (ਲੜੀ_ਤੁਲਨਾ(name, "XF86_", 5) == 0 ||
        (icase && istrncmp(name, "XF86_", 5) == 0)) {
        xkb_keysym_t ret;
        tmp = strdup(name);
        ਜੇ (!tmp)
            ਵਾਪਸ XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
        memmove(&tmp[4], &tmp[5], ਲੜੀ_ਲੰਬਾਈ(name) - 5 + 1);
        ret = xkb_keysym_from_name(tmp, flags);
        ਮੁਕਤ(tmp);
        ਵਾਪਸ ret;
    }

    ਵਾਪਸ XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
}

xkb_keysym_t
xkb_utf8_ਮੰਜ਼ਿਲ_keysym(ਅਚਲ ਅੱਖਰ *ਬਫਰ, ਮਾਪ_t ਮਾਪ)
{
    ਜੇ (!ਬਫਰ || !ਮਾਪ)
        ਵਾਪਸ 0;

    /* Try to parse the parameter as a single UTF-8 encoded character */
    ਮਾਪ_t ਲੰਮਾਈ = 0;
    ਅਚਲ uint32_t codepoint = utf8_ਅਗਲਾ_ਕੋਡ_ਬਿੰਦੂ(ਬਫਰ, ਮਾਪ, &ਲੰਮਾਈ);
    ਵਾਪਸ (codepoint == INVALID_UTF8_ਕੋਡ_ਬਿੰਦੂ || ਲੰਮਾਈ == 0)
        ? XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ
        : xkb_utf32_ਮੰਜ਼ਿਲ_keysym(codepoint);
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
                         ਅਚਲ ਅੱਖਰ *name,
                         ਅਚਲ ਅੱਖਰ **reference_name)
{
    ਜੇ (keysym > XKB_KEYSYM_MAX) {
        /* Invalid keysym */
        *reference_name = ਨਿਰਾ;
        ਵਾਪਸ false;
    }
    /* [WARNING] We do not check that name, if defined, is a valid for keysym */

    int32_t lo = 0, hi = ARRAY_ਮਾਪ(deprecated_keysyms) - 1;
    ਜਦੋਂ (hi >= lo) {
        int32_t mid = (lo + hi) / 2;
        ਜੇ (keysym > deprecated_keysyms[mid].keysym) {
            lo = mid + 1;
        } ਨਹੀਂਤਾਂ ਜੇ (keysym < deprecated_keysyms[mid].keysym) {
            hi = mid - 1;
        } ਨਹੀਂਤਾਂ {
            /* Keysym have some deprecated names */
            ਜੇ (deprecated_keysyms[mid].ਵਿੱਥ == DEPRECATED_KEYSYM) {
                /* All names are deprecated */
                *reference_name = ਨਿਰਾ;
                ਵਾਪਸ true;
            } ਨਹੀਂਤਾਂ ਜੇ (deprecated_keysyms[mid].ਵਿੱਥ == UNICODE_KEYSYM) {
                /* All names are deprecated, but Unicode notation is valid */
                *reference_name = ਨਿਰਾ;
                ਜੇ (name == ਨਿਰਾ)
                    ਵਾਪਸ false;
                ਜੇ (name[0] != 'U')
                    ਵਾਪਸ true;
                /* Since the current function is internal, assume the name
                 * corresponds to the keysym value and just check its syntax. */
                ਅਚਲ ਅੱਖਰ *ਸ਼ੁਰੂ = name + 1;
                ਜਦੋਂ (is_xdigit(*ਸ਼ੁਰੂ))
                    ਸ਼ੁਰੂ++;
                ਵਾਪਸ *ਸ਼ੁਰੂ != '\0' && ਸ਼ੁਰੂ > name + 1;
            }
            /* There is a reference name that is not deprecated */
            *reference_name = get_name_by_ਸੂਚਕਾਂਕ(deprecated_keysyms[mid].ਵਿੱਥ);
            ਜੇ (!name) {
                /* No name to check: indicate not deprecated */
                ਵਾਪਸ false;
            }
            ਜੇ (deprecated_keysyms[mid].explicit_ਗਿਣਤੀ) {
                /* Only some explicit names are deprecated */
                ਅਚਲ uint8_t k_max = deprecated_keysyms[mid].explicit_ਸੂਚਕਾਂਕ
                                    + deprecated_keysyms[mid].explicit_ਗਿਣਤੀ;
                ਪ੍ਰਤਿਜ੍ਞਾ(k_max <= ARRAY_ਮਾਪ(explicit_deprecated_aliases));
                /* Check every deprecated alias */
                ਲਈ (uint8_t k = deprecated_keysyms[mid].explicit_ਸੂਚਕਾਂਕ;
                     k < k_max; k++) {
                    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __clang__
                    /* Make clang-tidy happy */
                    __builtin_assume(k_max <=
                                     ARRAY_ਮਾਪ(explicit_deprecated_aliases));
                    #ਜੇ_ਅੰਤ
                    ਅਚਲ ਅੱਖਰ *alias =
                        get_name_by_ਸੂਚਕਾਂਕ(explicit_deprecated_aliases[k]);
                    ਜੇ (strcmp(name, alias) == 0)
                        ਵਾਪਸ true;
                }
                ਵਾਪਸ false;
            } ਨਹੀਂਤਾਂ {
                /* All names but the reference one are deprecated */
                ਵਾਪਸ strcmp(name, *reference_name) != 0;
            }
        }
    }

    /* Keysym has no deprecated names */
    *reference_name = ਨਿਰਾ;
    ਵਾਪਸ false;
}

bool
xkb_keysym_is_keypad(xkb_keysym_t keysym)
{
    ਵਾਪਸ keysym >= XKB_ਕੁੰਜੀ_KP_Space && keysym <= XKB_ਕੁੰਜੀ_KP_Equal;
}


bool
xkb_keysym_is_modifier(xkb_keysym_t keysym)
{
    ਵਾਪਸ
        (keysym >= XKB_ਕੁੰਜੀ_ਖਿਸਕਾਓ_L && keysym <= XKB_ਕੁੰਜੀ_Hyper_R) ||
        (keysym >= XKB_ਕੁੰਜੀ_ISO_ਤਾਲਾ && keysym <= XKB_ਕੁੰਜੀ_ISO_Level5_ਤਾਲਾ) ||
        keysym == XKB_ਕੁੰਜੀ_ਬਹੁਲਕ_switch ||
        keysym == XKB_ਕੁੰਜੀ_ਅੰਕ_ਤਾਲਾ;
}
