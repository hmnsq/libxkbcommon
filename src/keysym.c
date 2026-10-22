/*
 * For MIT-open-group:
 * Copyright 1985, 1987, 1990, 1998  The Open Group
 *
 * For MIT:
 * Copyright © 2009 Dan Nicholson
 *
 * SPDX-License-Identifier: MIT-open-group AND MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <stdint.h>
#অন্তর্ভাব <stdlib.h>

#অন্তর্ভাব "xkbcommon/xkbcommon-keysyms.h"
#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "features/enums.h"
#অন্তর্ভাব "keysym.h"
#অন্তর্ভাব "keysym-names.h"
#অন্তর্ভাব "utf8-decoding.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "util-numbers.h"

স্থির ssize_t
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
    যদি (ks > XKB_KEYSYM_MAX_EXPLICIT)
        প্রত্যাগম -1;

    ssize_t lo = 0, hi = ARRAY_SIZE(keysym_to_name) - 1;
    যাবত্ (hi >= lo) {
        ssize_t mid = (lo + hi) / 2;
        যদি (ks > keysym_to_name[mid].keysym) {
            lo = mid + 1;
        } অন্যথা যদি (ks < keysym_to_name[mid].keysym) {
            hi = mid - 1;
        } অন্যথা {
            প্রত্যাগম mid;
        }
    }

    প্রত্যাগম -1;
}

#define get_name_by_index(index) (keysym_names + (index))

স্থির অন্তঃস্থ ধ্রুব বর্ণ *
get_name(ধ্রুব সংরচনা name_keysym *entry)
{
    প্রত্যাগম get_name_by_index(entry->offset);
}

/* Unnamed Unicode codepoint. */
স্থির অন্তঃস্থ পূর্ণাঙ্ক
get_unicode_name(xkb_keysym_t ks, বর্ণ *buffer, size_t size)
{
    প্রত্যাগম snprintf(buffer, size, "U%04"PRIX32, ks & UINT32_C(0xffffff));
}

পূর্ণাঙ্ক
xkb_keysym_get_name(xkb_keysym_t ks, বর্ণ *buffer, size_t size)
{
    যদি (ks > XKB_KEYSYM_MAX) {
        snprintf(buffer, size, "Invalid");
        প্রত্যাগম -1;
    }

    ssize_t index = find_keysym_index(ks);
    যদি (index != -1)
        প্রত্যাগম snprintf(buffer, size, "%s", get_name(&keysym_to_name[index]));

    /*
     * Unnamed Unicode codepoint.
     * • Keysyms in the range [XKB_KEYSYM_UNICODE_OFFSET, XKB_KEYSYM_UNICODE_MIN [
     *   do not use the Unicode notation for backward compatibility.
     * • Keysyms are not normalized: e.g. given a Unicode keysym, calling
     *   `xkb_utf32_to_keysym` with the corresponding code point may return a
     *   different keysym or fail (e.g. surrogates are invalid in UTF-32).
     */
    যদি (ks >= XKB_KEYSYM_UNICODE_MIN && ks <= XKB_KEYSYM_UNICODE_MAX)
        প্রত্যাগম get_unicode_name(ks, buffer, size);

    /* Unnamed, non-Unicode, symbol (shouldn't generally happen). */
    প্রত্যাগম snprintf(buffer, size, "0x%08"PRIx32, ks);
}

bool
xkb_keysym_is_assigned(xkb_keysym_t ks)
{
    প্রত্যাগম (XKB_KEYSYM_UNICODE_MIN <= ks && ks <= XKB_KEYSYM_UNICODE_MAX) ||
           find_keysym_index(ks) != -1;
}

পূর্ণাঙ্ক
xkb_keysym_get_explicit_names(xkb_keysym_t ks, ধ্রুব বর্ণ **buffer, size_t size)
{
    যদি (ks > XKB_KEYSYM_MAX)
        প্রত্যাগম -1;
    ধ্রুব ssize_t index = find_keysym_index(ks);
    যদি (index < 0)
        প্রত্যাগম 0;
    ধ্রুব uint16_t canonical = keysym_to_name[index].offset;
    যদি (size > 0)
        buffer[0] = get_name(&keysym_to_name[index]);
    পূর্ণাঙ্ক গুনতি = 1;
    প্রতি (size_t pos = 0; pos < ARRAY_SIZE(name_to_keysym); pos++) {
        যদি (name_to_keysym[pos].keysym == ks &&
            name_to_keysym[pos].offset != canonical) {
            যদি ((size_t) গুনতি < size) {
                buffer[গুনতি] = get_name(&name_to_keysym[pos]);
            }
            গুনতি++;
        }
    }
    প্রত্যাগম গুনতি;
}

সংরচনা xkb_keysym_iterator {
    bool explicit;       /* If true, traverse only explicitly named keysyms */
    int32_t index;       /* Current index in `keysym_to_name` */
    xkb_keysym_t keysym; /* Current keysym */
};

সংরচনা xkb_keysym_iterator*
xkb_keysym_iterator_new(bool iterate_only_explicit_keysyms)
{
    সংরচনা xkb_keysym_iterator* iter = calloc(1, পরিমাণ(*iter));
    যদি (!iter)
        প্রত্যাগম রিক্ত;
    iter->explicit = iterate_only_explicit_keysyms;
    iter->index = -1;
    iter->keysym = XKB_KEYSYM_UNICODE_MAX;
    প্রত্যাগম iter;
}

সংরচনা xkb_keysym_iterator*
xkb_keysym_iterator_unref(সংরচনা xkb_keysym_iterator *iter)
{
    /*
     * [NOTE] If we ever make this API public, add:
     * - an `refcnt` member,
     * - `xkb_keysym_iterator_ref()`,
     * - `assert(!iter || iter->refcnt > 0)`.
     */
    যদি (iter)
        মুক্ত(iter);
    প্রত্যাগম রিক্ত;
}

xkb_keysym_t
xkb_keysym_iterator_get_keysym(সংরচনা xkb_keysym_iterator *iter)
{
    প্রত্যাগম iter->keysym;
}

bool
xkb_keysym_iterator_is_explicitly_named(সংরচনা xkb_keysym_iterator *iter)
{
    প্রত্যাগম iter->index >= 0 &&
           iter->index < (int32_t)ARRAY_SIZE(keysym_to_name) &&
           (iter->explicit ||
            iter->keysym == keysym_to_name[iter->index].keysym);
}

পূর্ণাঙ্ক
xkb_keysym_iterator_get_name(সংরচনা xkb_keysym_iterator *iter,
                             বর্ণ *buffer, size_t size)
{
    যদি (iter->index < 0 || iter->index >= (int32_t)ARRAY_SIZE(keysym_to_name))
        প্রত্যাগম -1;
    যদি (iter->explicit || iter->keysym == keysym_to_name[iter->index].keysym)
        প্রত্যাগম snprintf(buffer, size, "%s",
                        get_name(&keysym_to_name[iter->index]));
    প্রত্যাগম get_unicode_name(iter->keysym, buffer, size);
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
xkb_keysym_iterator_next(সংরচনা xkb_keysym_iterator *iter)
{
    যদি (iter->index >= (int32_t)ARRAY_SIZE(keysym_to_name) - 1)
        প্রত্যাগম false;

    /* Next keysym */
    যদি (iter->explicit || iter->keysym >= XKB_KEYSYM_UNICODE_MAX ||
        keysym_to_name[iter->index + 1].keysym < XKB_KEYSYM_UNICODE_MIN) {
        /* Explicitly named keysyms only */
        iter->keysym = keysym_to_name[++iter->index].keysym;
        প্রতিজ্ঞা(iter->explicit ||
               iter->keysym <= XKB_KEYSYM_UNICODE_MIN ||
               iter->keysym >= XKB_KEYSYM_UNICODE_MAX);
    } অন্যথা {
        /* Unicode keysyms
         * NOTE: Unicode keysyms are within keysym_to_name keysyms range. */
        যদি (iter->keysym >= keysym_to_name[iter->index].keysym)
            iter->index++;
        যদি (iter->keysym >= XKB_KEYSYM_UNICODE_MIN) {
            /* Continue Unicode keysyms */
            iter->keysym++;
        } অন্যথা {
            /* Start Unicode keysyms */
            iter->keysym = XKB_KEYSYM_UNICODE_MIN;
        }
    }
    প্রত্যাগম true;
}

/* Parse the numeric part of a 0xXXXX and UXXXX keysym. */
স্থির bool
parse_keysym_hex(ধ্রুব বর্ণ *s, uint32_t *out)
{
    /* We expect a NULL-terminated string of length up to 8 */
    ধ্রুব পূর্ণাঙ্ক গুনতি = parse_hex_to_uint32_t(s, 8, out);
    /* Check that some value was parsed and we reached the end of the string */
    প্রত্যাগম (গুনতি > 0 && s[গুনতি] == '\0');
}

xkb_keysym_t
xkb_keysym_from_name(ধ্রুব বর্ণ *name, গণনা xkb_keysym_flags flags)
{
    যদি (flags & ~(গণনা xkb_keysym_flags)XKB_KEYSYM_FLAGS_মানসমূহ)
        প্রত্যাগম XKB_চাবি_No_প্রতীক;

    ধ্রুব সংরচনা name_keysym *entry = রিক্ত;
    বর্ণ *tmp;
    uint32_t val;
    bool icase = (flags & XKB_KEYSYM_CASE_INSENSITIVE);

    /*
     * We need to !icase case to be fast, for e.g. Compose file parsing.
     * So do it in a fast path.
     */
    যদি (!icase) {
        size_t pos = keysym_name_perfect_hash(name);
        যদি (pos < ARRAY_SIZE(name_to_keysym)) {
            ধ্রুব বর্ণ *s = get_name(&name_to_keysym[pos]);
            যদি (strcmp(name, s) == 0)
                প্রত্যাগম name_to_keysym[pos].keysym;
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
    অন্যথা {
        int32_t lo = 0, hi = ARRAY_SIZE(name_to_keysym) - 1;
        যাবত্ (hi >= lo) {
            int32_t mid = (lo + hi) / 2;
            পূর্ণাঙ্ক cmp = istrcmp(name, get_name(&name_to_keysym[mid]));
            যদি (cmp > 0) {
                lo = mid + 1;
            } অন্যথা যদি (cmp < 0) {
                hi = mid - 1;
            } অন্যথা {
                entry = &name_to_keysym[mid];
                ভঙ্গ;
            }
        }
        যদি (entry) {
            ধ্রুব সংরচনা name_keysym *last;
            last = name_to_keysym + ARRAY_SIZE(name_to_keysym) - 1;
            /* Keep going until we reach end of array
             * or non case-insensitve match */
            যাবত্ (entry < last &&
                   istrcmp(get_name(entry + 1), get_name(entry)) == 0) {
                entry++;
            }
            প্রত্যাগম entry->keysym;
        }
    }

    যদি (*name == 'U' || (icase && *name == 'u')) {
        /* Parse Unicode notation */
        যদি (!parse_keysym_hex(&name[1], &val))
            প্রত্যাগম XKB_চাবি_No_প্রতীক;
        প্রত্যাগম (val > 0xff && val <= 0x10ffff)
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
    অন্যথা যদি (name[0] == '0' && (name[1] == 'x' || (icase && name[1] == 'X'))) {
        /*
         * Parse numeric hexadecimal notation without any normalization, in
         * in order to be consistent with the keymap files parsing.
         */
        যদি (!parse_keysym_hex(&name[2], &val) || val > XKB_KEYSYM_MAX)
            প্রত্যাগম XKB_চাবি_No_প্রতীক;
        প্রত্যাগম (xkb_keysym_t) val;
    }

    /* Stupid inconsistency between the headers and XKeysymDB: the former has
     * no separating underscore, while some XF86* syms in the latter did.
     * As a last ditch effort, try without. */
    যদি (strncmp(name, "XF86_", 5) == 0 ||
        (icase && istrncmp(name, "XF86_", 5) == 0)) {
        xkb_keysym_t ret;
        tmp = strdup(name);
        যদি (!tmp)
            প্রত্যাগম XKB_চাবি_No_প্রতীক;
        memmove(&tmp[4], &tmp[5], strlen(name) - 5 + 1);
        ret = xkb_keysym_from_name(tmp, flags);
        মুক্ত(tmp);
        প্রত্যাগম ret;
    }

    প্রত্যাগম XKB_চাবি_No_প্রতীক;
}

xkb_keysym_t
xkb_utf8_to_keysym(ধ্রুব বর্ণ *buffer, size_t size)
{
    যদি (!buffer || !size)
        প্রত্যাগম 0;

    /* Try to parse the parameter as a single UTF-8 encoded character */
    size_t length = 0;
    ধ্রুব uint32_t codepoint = utf8_next_code_point(buffer, size, &length);
    প্রত্যাগম (codepoint == INVALID_UTF8_CODE_POINT || length == 0)
        ? XKB_চাবি_No_প্রতীক
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
                         ধ্রুব বর্ণ *name,
                         ধ্রুব বর্ণ **reference_name)
{
    যদি (keysym > XKB_KEYSYM_MAX) {
        /* Invalid keysym */
        *reference_name = রিক্ত;
        প্রত্যাগম false;
    }
    /* [WARNING] We do not check that name, if defined, is a valid for keysym */

    int32_t lo = 0, hi = ARRAY_SIZE(deprecated_keysyms) - 1;
    যাবত্ (hi >= lo) {
        int32_t mid = (lo + hi) / 2;
        যদি (keysym > deprecated_keysyms[mid].keysym) {
            lo = mid + 1;
        } অন্যথা যদি (keysym < deprecated_keysyms[mid].keysym) {
            hi = mid - 1;
        } অন্যথা {
            /* Keysym have some deprecated names */
            যদি (deprecated_keysyms[mid].offset == DEPRECATED_KEYSYM) {
                /* All names are deprecated */
                *reference_name = রিক্ত;
                প্রত্যাগম true;
            } অন্যথা যদি (deprecated_keysyms[mid].offset == UNICODE_KEYSYM) {
                /* All names are deprecated, but Unicode notation is valid */
                *reference_name = রিক্ত;
                যদি (name == রিক্ত)
                    প্রত্যাগম false;
                যদি (name[0] != 'U')
                    প্রত্যাগম true;
                /* Since the current function is internal, assume the name
                 * corresponds to the keysym value and just check its syntax. */
                ধ্রুব বর্ণ *start = name + 1;
                যাবত্ (is_xdigit(*start))
                    start++;
                প্রত্যাগম *start != '\0' && start > name + 1;
            }
            /* There is a reference name that is not deprecated */
            *reference_name = get_name_by_index(deprecated_keysyms[mid].offset);
            যদি (!name) {
                /* No name to check: indicate not deprecated */
                প্রত্যাগম false;
            }
            যদি (deprecated_keysyms[mid].explicit_গুনতি) {
                /* Only some explicit names are deprecated */
                ধ্রুব uint8_t k_max = deprecated_keysyms[mid].explicit_index
                                    + deprecated_keysyms[mid].explicit_গুনতি;
                প্রতিজ্ঞা(k_max <= ARRAY_SIZE(explicit_deprecated_aliases));
                /* Check every deprecated alias */
                প্রতি (uint8_t k = deprecated_keysyms[mid].explicit_index;
                     k < k_max; k++) {
                    #ifdef __clang__
                    /* Make clang-tidy happy */
                    __builtin_assume(k_max <=
                                     ARRAY_SIZE(explicit_deprecated_aliases));
                    #endif
                    ধ্রুব বর্ণ *alias =
                        get_name_by_index(explicit_deprecated_aliases[k]);
                    যদি (strcmp(name, alias) == 0)
                        প্রত্যাগম true;
                }
                প্রত্যাগম false;
            } অন্যথা {
                /* All names but the reference one are deprecated */
                প্রত্যাগম strcmp(name, *reference_name) != 0;
            }
        }
    }

    /* Keysym has no deprecated names */
    *reference_name = রিক্ত;
    প্রত্যাগম false;
}

bool
xkb_keysym_is_keypad(xkb_keysym_t keysym)
{
    প্রত্যাগম keysym >= XKB_চাবি_KP_Space && keysym <= XKB_চাবি_KP_Equal;
}


bool
xkb_keysym_is_modifier(xkb_keysym_t keysym)
{
    প্রত্যাগম
        (keysym >= XKB_চাবি_Shift_L && keysym <= XKB_চাবি_Hyper_R) ||
        (keysym >= XKB_চাবি_ISO_Lock && keysym <= XKB_চাবি_ISO_Level5_Lock) ||
        keysym == XKB_চাবি_Mode_switch ||
        keysym == XKB_চাবি_অঙ্ক_Lock;
}
