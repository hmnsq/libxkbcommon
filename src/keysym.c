/*
 * For MIT-open-group:
 * Copyright 1985, 1987, 1990, 1998  The Open Group
 *
 * For MIT:
 * Copyright © 2009 Dan Nicholson
 *
 * SPDX-License-Identifier: MIT-open-group AND MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ <stdlib.h>

#அந்தர்பாவ "xkbcommon/xkbcommon-keysyms.h"
#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "features/enums.h"
#அந்தர்பாவ "keysym.h"
#அந்தர்பாவ "keysym-names.h"
#அந்தர்பாவ "utf8-decoding.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "util-numbers.h"

நிலையான ssize_t
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
    எனில் (ks > XKB_KEYSYM_MAX_EXPLICIT)
        திரும்பு -1;

    ssize_t lo = 0, hi = ARRAY_SIZE(keysym_to_name) - 1;
    போது (hi >= lo) {
        ssize_t mid = (lo + hi) / 2;
        எனில் (ks > keysym_to_name[mid].keysym) {
            lo = mid + 1;
        } இல்லையெனில் எனில் (ks < keysym_to_name[mid].keysym) {
            hi = mid - 1;
        } இல்லையெனில் {
            திரும்பு mid;
        }
    }

    திரும்பு -1;
}

#define get_name_by_index(index) (keysym_names + (index))

நிலையான உள்வரி மாறிலி எழுத்து *
get_name(மாறிலி கட்டமைப்பு name_keysym *entry)
{
    திரும்பு get_name_by_index(entry->offset);
}

/* Unnamed Unicode codepoint. */
நிலையான உள்வரி முழு
get_unicode_name(xkb_keysym_t ks, எழுத்து *buffer, size_t size)
{
    திரும்பு snprintf(buffer, size, "U%04"PRIX32, ks & UINT32_C(0xffffff));
}

முழு
xkb_keysym_get_name(xkb_keysym_t ks, எழுத்து *buffer, size_t size)
{
    எனில் (ks > XKB_KEYSYM_MAX) {
        snprintf(buffer, size, "Invalid");
        திரும்பு -1;
    }

    ssize_t index = find_keysym_index(ks);
    எனில் (index != -1)
        திரும்பு snprintf(buffer, size, "%s", get_name(&keysym_to_name[index]));

    /*
     * Unnamed Unicode codepoint.
     * • Keysyms in the range [XKB_KEYSYM_UNICODE_OFFSET, XKB_KEYSYM_UNICODE_MIN [
     *   do not use the Unicode notation for backward compatibility.
     * • Keysyms are not normalized: e.g. given a Unicode keysym, calling
     *   `xkb_utf32_to_keysym` with the corresponding code point may return a
     *   different keysym or fail (e.g. surrogates are invalid in UTF-32).
     */
    எனில் (ks >= XKB_KEYSYM_UNICODE_MIN && ks <= XKB_KEYSYM_UNICODE_MAX)
        திரும்பு get_unicode_name(ks, buffer, size);

    /* Unnamed, non-Unicode, symbol (shouldn't generally happen). */
    திரும்பு snprintf(buffer, size, "0x%08"PRIx32, ks);
}

bool
xkb_keysym_is_assigned(xkb_keysym_t ks)
{
    திரும்பு (XKB_KEYSYM_UNICODE_MIN <= ks && ks <= XKB_KEYSYM_UNICODE_MAX) ||
           find_keysym_index(ks) != -1;
}

முழு
xkb_keysym_get_explicit_names(xkb_keysym_t ks, மாறிலி எழுத்து **buffer, size_t size)
{
    எனில் (ks > XKB_KEYSYM_MAX)
        திரும்பு -1;
    மாறிலி ssize_t index = find_keysym_index(ks);
    எனில் (index < 0)
        திரும்பு 0;
    மாறிலி uint16_t canonical = keysym_to_name[index].offset;
    எனில் (size > 0)
        buffer[0] = get_name(&keysym_to_name[index]);
    முழு மொத்தம் = 1;
    க்கு (size_t pos = 0; pos < ARRAY_SIZE(name_to_keysym); pos++) {
        எனில் (name_to_keysym[pos].keysym == ks &&
            name_to_keysym[pos].offset != canonical) {
            எனில் ((size_t) மொத்தம் < size) {
                buffer[மொத்தம்] = get_name(&name_to_keysym[pos]);
            }
            மொத்தம்++;
        }
    }
    திரும்பு மொத்தம்;
}

கட்டமைப்பு xkb_keysym_iterator {
    bool explicit;       /* If true, traverse only explicitly named keysyms */
    int32_t index;       /* Current index in `keysym_to_name` */
    xkb_keysym_t keysym; /* Current keysym */
};

கட்டமைப்பு xkb_keysym_iterator*
xkb_keysym_iterator_new(bool iterate_only_explicit_keysyms)
{
    கட்டமைப்பு xkb_keysym_iterator* iter = calloc(1, அளவு(*iter));
    எனில் (!iter)
        திரும்பு வெற்று;
    iter->explicit = iterate_only_explicit_keysyms;
    iter->index = -1;
    iter->keysym = XKB_KEYSYM_UNICODE_MAX;
    திரும்பு iter;
}

கட்டமைப்பு xkb_keysym_iterator*
xkb_keysym_iterator_unref(கட்டமைப்பு xkb_keysym_iterator *iter)
{
    /*
     * [NOTE] If we ever make this API public, add:
     * - an `refcnt` member,
     * - `xkb_keysym_iterator_ref()`,
     * - `assert(!iter || iter->refcnt > 0)`.
     */
    எனில் (iter)
        விடுவி(iter);
    திரும்பு வெற்று;
}

xkb_keysym_t
xkb_keysym_iterator_get_keysym(கட்டமைப்பு xkb_keysym_iterator *iter)
{
    திரும்பு iter->keysym;
}

bool
xkb_keysym_iterator_is_explicitly_named(கட்டமைப்பு xkb_keysym_iterator *iter)
{
    திரும்பு iter->index >= 0 &&
           iter->index < (int32_t)ARRAY_SIZE(keysym_to_name) &&
           (iter->explicit ||
            iter->keysym == keysym_to_name[iter->index].keysym);
}

முழு
xkb_keysym_iterator_get_name(கட்டமைப்பு xkb_keysym_iterator *iter,
                             எழுத்து *buffer, size_t size)
{
    எனில் (iter->index < 0 || iter->index >= (int32_t)ARRAY_SIZE(keysym_to_name))
        திரும்பு -1;
    எனில் (iter->explicit || iter->keysym == keysym_to_name[iter->index].keysym)
        திரும்பு snprintf(buffer, size, "%s",
                        get_name(&keysym_to_name[iter->index]));
    திரும்பு get_unicode_name(iter->keysym, buffer, size);
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
xkb_keysym_iterator_next(கட்டமைப்பு xkb_keysym_iterator *iter)
{
    எனில் (iter->index >= (int32_t)ARRAY_SIZE(keysym_to_name) - 1)
        திரும்பு false;

    /* Next keysym */
    எனில் (iter->explicit || iter->keysym >= XKB_KEYSYM_UNICODE_MAX ||
        keysym_to_name[iter->index + 1].keysym < XKB_KEYSYM_UNICODE_MIN) {
        /* Explicitly named keysyms only */
        iter->keysym = keysym_to_name[++iter->index].keysym;
        ப்ரதிஜ்ஞா(iter->explicit ||
               iter->keysym <= XKB_KEYSYM_UNICODE_MIN ||
               iter->keysym >= XKB_KEYSYM_UNICODE_MAX);
    } இல்லையெனில் {
        /* Unicode keysyms
         * NOTE: Unicode keysyms are within keysym_to_name keysyms range. */
        எனில் (iter->keysym >= keysym_to_name[iter->index].keysym)
            iter->index++;
        எனில் (iter->keysym >= XKB_KEYSYM_UNICODE_MIN) {
            /* Continue Unicode keysyms */
            iter->keysym++;
        } இல்லையெனில் {
            /* Start Unicode keysyms */
            iter->keysym = XKB_KEYSYM_UNICODE_MIN;
        }
    }
    திரும்பு true;
}

/* Parse the numeric part of a 0xXXXX and UXXXX keysym. */
நிலையான bool
parse_keysym_hex(மாறிலி எழுத்து *s, uint32_t *out)
{
    /* We expect a NULL-terminated string of length up to 8 */
    மாறிலி முழு மொத்தம் = parse_hex_to_uint32_t(s, 8, out);
    /* Check that some value was parsed and we reached the end of the string */
    திரும்பு (மொத்தம் > 0 && s[மொத்தம்] == '\0');
}

xkb_keysym_t
xkb_keysym_from_name(மாறிலி எழுத்து *name, எண்ணிக்கை xkb_keysym_flags flags)
{
    எனில் (flags & ~(எண்ணிக்கை xkb_keysym_flags)XKB_KEYSYM_FLAGS_மதிப்புகள்)
        திரும்பு XKB_சாவி_No_குறியீடு;

    மாறிலி கட்டமைப்பு name_keysym *entry = வெற்று;
    எழுத்து *tmp;
    uint32_t val;
    bool icase = (flags & XKB_KEYSYM_CASE_INSENSITIVE);

    /*
     * We need to !icase case to be fast, for e.g. Compose file parsing.
     * So do it in a fast path.
     */
    எனில் (!icase) {
        size_t pos = keysym_name_perfect_hash(name);
        எனில் (pos < ARRAY_SIZE(name_to_keysym)) {
            மாறிலி எழுத்து *s = get_name(&name_to_keysym[pos]);
            எனில் (strcmp(name, s) == 0)
                திரும்பு name_to_keysym[pos].keysym;
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
    இல்லையெனில் {
        int32_t lo = 0, hi = ARRAY_SIZE(name_to_keysym) - 1;
        போது (hi >= lo) {
            int32_t mid = (lo + hi) / 2;
            முழு cmp = istrcmp(name, get_name(&name_to_keysym[mid]));
            எனில் (cmp > 0) {
                lo = mid + 1;
            } இல்லையெனில் எனில் (cmp < 0) {
                hi = mid - 1;
            } இல்லையெனில் {
                entry = &name_to_keysym[mid];
                நிறுத்து;
            }
        }
        எனில் (entry) {
            மாறிலி கட்டமைப்பு name_keysym *last;
            last = name_to_keysym + ARRAY_SIZE(name_to_keysym) - 1;
            /* Keep going until we reach end of array
             * or non case-insensitve match */
            போது (entry < last &&
                   istrcmp(get_name(entry + 1), get_name(entry)) == 0) {
                entry++;
            }
            திரும்பு entry->keysym;
        }
    }

    எனில் (*name == 'U' || (icase && *name == 'u')) {
        /* Parse Unicode notation */
        எனில் (!parse_keysym_hex(&name[1], &val))
            திரும்பு XKB_சாவி_No_குறியீடு;
        திரும்பு (val > 0xff && val <= 0x10ffff)
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
    இல்லையெனில் எனில் (name[0] == '0' && (name[1] == 'x' || (icase && name[1] == 'X'))) {
        /*
         * Parse numeric hexadecimal notation without any normalization, in
         * in order to be consistent with the keymap files parsing.
         */
        எனில் (!parse_keysym_hex(&name[2], &val) || val > XKB_KEYSYM_MAX)
            திரும்பு XKB_சாவி_No_குறியீடு;
        திரும்பு (xkb_keysym_t) val;
    }

    /* Stupid inconsistency between the headers and XKeysymDB: the former has
     * no separating underscore, while some XF86* syms in the latter did.
     * As a last ditch effort, try without. */
    எனில் (strncmp(name, "XF86_", 5) == 0 ||
        (icase && istrncmp(name, "XF86_", 5) == 0)) {
        xkb_keysym_t ret;
        tmp = strdup(name);
        எனில் (!tmp)
            திரும்பு XKB_சாவி_No_குறியீடு;
        memmove(&tmp[4], &tmp[5], strlen(name) - 5 + 1);
        ret = xkb_keysym_from_name(tmp, flags);
        விடுவி(tmp);
        திரும்பு ret;
    }

    திரும்பு XKB_சாவி_No_குறியீடு;
}

xkb_keysym_t
xkb_utf8_to_keysym(மாறிலி எழுத்து *buffer, size_t size)
{
    எனில் (!buffer || !size)
        திரும்பு 0;

    /* Try to parse the parameter as a single UTF-8 encoded character */
    size_t length = 0;
    மாறிலி uint32_t codepoint = utf8_next_code_point(buffer, size, &length);
    திரும்பு (codepoint == INVALID_UTF8_CODE_POINT || length == 0)
        ? XKB_சாவி_No_குறியீடு
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
                         மாறிலி எழுத்து *name,
                         மாறிலி எழுத்து **reference_name)
{
    எனில் (keysym > XKB_KEYSYM_MAX) {
        /* Invalid keysym */
        *reference_name = வெற்று;
        திரும்பு false;
    }
    /* [WARNING] We do not check that name, if defined, is a valid for keysym */

    int32_t lo = 0, hi = ARRAY_SIZE(deprecated_keysyms) - 1;
    போது (hi >= lo) {
        int32_t mid = (lo + hi) / 2;
        எனில் (keysym > deprecated_keysyms[mid].keysym) {
            lo = mid + 1;
        } இல்லையெனில் எனில் (keysym < deprecated_keysyms[mid].keysym) {
            hi = mid - 1;
        } இல்லையெனில் {
            /* Keysym have some deprecated names */
            எனில் (deprecated_keysyms[mid].offset == DEPRECATED_KEYSYM) {
                /* All names are deprecated */
                *reference_name = வெற்று;
                திரும்பு true;
            } இல்லையெனில் எனில் (deprecated_keysyms[mid].offset == UNICODE_KEYSYM) {
                /* All names are deprecated, but Unicode notation is valid */
                *reference_name = வெற்று;
                எனில் (name == வெற்று)
                    திரும்பு false;
                எனில் (name[0] != 'U')
                    திரும்பு true;
                /* Since the current function is internal, assume the name
                 * corresponds to the keysym value and just check its syntax. */
                மாறிலி எழுத்து *start = name + 1;
                போது (is_xdigit(*start))
                    start++;
                திரும்பு *start != '\0' && start > name + 1;
            }
            /* There is a reference name that is not deprecated */
            *reference_name = get_name_by_index(deprecated_keysyms[mid].offset);
            எனில் (!name) {
                /* No name to check: indicate not deprecated */
                திரும்பு false;
            }
            எனில் (deprecated_keysyms[mid].explicit_மொத்தம்) {
                /* Only some explicit names are deprecated */
                மாறிலி uint8_t k_max = deprecated_keysyms[mid].explicit_index
                                    + deprecated_keysyms[mid].explicit_மொத்தம்;
                ப்ரதிஜ்ஞா(k_max <= ARRAY_SIZE(explicit_deprecated_aliases));
                /* Check every deprecated alias */
                க்கு (uint8_t k = deprecated_keysyms[mid].explicit_index;
                     k < k_max; k++) {
                    #ifdef __clang__
                    /* Make clang-tidy happy */
                    __builtin_assume(k_max <=
                                     ARRAY_SIZE(explicit_deprecated_aliases));
                    #endif
                    மாறிலி எழுத்து *alias =
                        get_name_by_index(explicit_deprecated_aliases[k]);
                    எனில் (strcmp(name, alias) == 0)
                        திரும்பு true;
                }
                திரும்பு false;
            } இல்லையெனில் {
                /* All names but the reference one are deprecated */
                திரும்பு strcmp(name, *reference_name) != 0;
            }
        }
    }

    /* Keysym has no deprecated names */
    *reference_name = வெற்று;
    திரும்பு false;
}

bool
xkb_keysym_is_keypad(xkb_keysym_t keysym)
{
    திரும்பு keysym >= XKB_சாவி_KP_Space && keysym <= XKB_சாவி_KP_Equal;
}


bool
xkb_keysym_is_modifier(xkb_keysym_t keysym)
{
    திரும்பு
        (keysym >= XKB_சாவி_Shift_L && keysym <= XKB_சாவி_Hyper_R) ||
        (keysym >= XKB_சாவி_ISO_Lock && keysym <= XKB_சாவி_ISO_Level5_Lock) ||
        keysym == XKB_சாவி_Mode_switch ||
        keysym == XKB_சாவி_இலக்கம்_Lock;
}
