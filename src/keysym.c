/*
 * For MIT-open-group:
 * Copyright 1985, 1987, 1990, 1998  The Open Group
 *
 * For MIT:
 * Copyright © 2009 Dan Nicholson
 *
 * SPDX-License-Identifier: MIT-open-group AND MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon-keysyms.h"
#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "features/enums.h"
#ଅନ୍ତର୍ଭାଵ "keysym.h"
#ଅନ୍ତର୍ଭାଵ "keysym-names.h"
#ଅନ୍ତର୍ଭାଵ "utf8-decoding.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "util-numbers.h"

ସ୍ଥିର ssize_t
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
    ଯଦି (ks > XKB_KEYSYM_MAX_EXPLICIT)
        ଫେରନ୍ତୁ -1;

    ssize_t lo = 0, hi = ARRAY_SIZE(keysym_to_name) - 1;
    ଯେତେବେଳେ (hi >= lo) {
        ssize_t mid = (lo + hi) / 2;
        ଯଦି (ks > keysym_to_name[mid].keysym) {
            lo = mid + 1;
        } ଅନ୍ୟଥା ଯଦି (ks < keysym_to_name[mid].keysym) {
            hi = mid - 1;
        } ଅନ୍ୟଥା {
            ଫେରନ୍ତୁ mid;
        }
    }

    ଫେରନ୍ତୁ -1;
}

#define get_name_by_index(index) (keysym_names + (index))

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ ଅକ୍ଷର *
get_name(ଅଚଳ ସଂରଚନା name_keysym *entry)
{
    ଫେରନ୍ତୁ get_name_by_index(entry->offset);
}

/* Unnamed Unicode codepoint. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_unicode_name(xkb_keysym_t ks, ଅକ୍ଷର *buffer, size_t size)
{
    ଫେରନ୍ତୁ snprintf(buffer, size, "U%04"PRIX32, ks & UINT32_C(0xffffff));
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_keysym_get_name(xkb_keysym_t ks, ଅକ୍ଷର *buffer, size_t size)
{
    ଯଦି (ks > XKB_KEYSYM_MAX) {
        snprintf(buffer, size, "Invalid");
        ଫେରନ୍ତୁ -1;
    }

    ssize_t index = find_keysym_index(ks);
    ଯଦି (index != -1)
        ଫେରନ୍ତୁ snprintf(buffer, size, "%s", get_name(&keysym_to_name[index]));

    /*
     * Unnamed Unicode codepoint.
     * • Keysyms in the range [XKB_KEYSYM_UNICODE_OFFSET, XKB_KEYSYM_UNICODE_MIN [
     *   do not use the Unicode notation for backward compatibility.
     * • Keysyms are not normalized: e.g. given a Unicode keysym, calling
     *   `xkb_utf32_to_keysym` with the corresponding code point may return a
     *   different keysym or fail (e.g. surrogates are invalid in UTF-32).
     */
    ଯଦି (ks >= XKB_KEYSYM_UNICODE_MIN && ks <= XKB_KEYSYM_UNICODE_MAX)
        ଫେରନ୍ତୁ get_unicode_name(ks, buffer, size);

    /* Unnamed, non-Unicode, symbol (shouldn't generally happen). */
    ଫେରନ୍ତୁ snprintf(buffer, size, "0x%08"PRIx32, ks);
}

bool
xkb_keysym_is_assigned(xkb_keysym_t ks)
{
    ଫେରନ୍ତୁ (XKB_KEYSYM_UNICODE_MIN <= ks && ks <= XKB_KEYSYM_UNICODE_MAX) ||
           find_keysym_index(ks) != -1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_keysym_get_explicit_names(xkb_keysym_t ks, ଅଚଳ ଅକ୍ଷର **buffer, size_t size)
{
    ଯଦି (ks > XKB_KEYSYM_MAX)
        ଫେରନ୍ତୁ -1;
    ଅଚଳ ssize_t index = find_keysym_index(ks);
    ଯଦି (index < 0)
        ଫେରନ୍ତୁ 0;
    ଅଚଳ uint16_t canonical = keysym_to_name[index].offset;
    ଯଦି (size > 0)
        buffer[0] = get_name(&keysym_to_name[index]);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 1;
    ପାଇଁ (size_t pos = 0; pos < ARRAY_SIZE(name_to_keysym); pos++) {
        ଯଦି (name_to_keysym[pos].keysym == ks &&
            name_to_keysym[pos].offset != canonical) {
            ଯଦି ((size_t) ଗଣନା_2 < size) {
                buffer[ଗଣନା_2] = get_name(&name_to_keysym[pos]);
            }
            ଗଣନା_2++;
        }
    }
    ଫେରନ୍ତୁ ଗଣନା_2;
}

ସଂରଚନା xkb_keysym_iterator {
    bool explicit;       /* If true, traverse only explicitly named keysyms */
    int32_t index;       /* Current index in `keysym_to_name` */
    xkb_keysym_t keysym; /* Current keysym */
};

ସଂରଚନା xkb_keysym_iterator*
xkb_keysym_iterator_new(bool iterate_only_explicit_keysyms)
{
    ସଂରଚନା xkb_keysym_iterator* iter = calloc(1, ଆକାର(*iter));
    ଯଦି (!iter)
        ଫେରନ୍ତୁ ଖାଲି;
    iter->explicit = iterate_only_explicit_keysyms;
    iter->index = -1;
    iter->keysym = XKB_KEYSYM_UNICODE_MAX;
    ଫେରନ୍ତୁ iter;
}

ସଂରଚନା xkb_keysym_iterator*
xkb_keysym_iterator_unref(ସଂରଚନା xkb_keysym_iterator *iter)
{
    /*
     * [NOTE] If we ever make this API public, add:
     * - an `refcnt` member,
     * - `xkb_keysym_iterator_ref()`,
     * - `assert(!iter || iter->refcnt > 0)`.
     */
    ଯଦି (iter)
        ମୁକ୍ତ(iter);
    ଫେରନ୍ତୁ ଖାଲି;
}

xkb_keysym_t
xkb_keysym_iterator_get_keysym(ସଂରଚନା xkb_keysym_iterator *iter)
{
    ଫେରନ୍ତୁ iter->keysym;
}

bool
xkb_keysym_iterator_is_explicitly_named(ସଂରଚନା xkb_keysym_iterator *iter)
{
    ଫେରନ୍ତୁ iter->index >= 0 &&
           iter->index < (int32_t)ARRAY_SIZE(keysym_to_name) &&
           (iter->explicit ||
            iter->keysym == keysym_to_name[iter->index].keysym);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_keysym_iterator_get_name(ସଂରଚନା xkb_keysym_iterator *iter,
                             ଅକ୍ଷର *buffer, size_t size)
{
    ଯଦି (iter->index < 0 || iter->index >= (int32_t)ARRAY_SIZE(keysym_to_name))
        ଫେରନ୍ତୁ -1;
    ଯଦି (iter->explicit || iter->keysym == keysym_to_name[iter->index].keysym)
        ଫେରନ୍ତୁ snprintf(buffer, size, "%s",
                        get_name(&keysym_to_name[iter->index]));
    ଫେରନ୍ତୁ get_unicode_name(iter->keysym, buffer, size);
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
xkb_keysym_iterator_next(ସଂରଚନା xkb_keysym_iterator *iter)
{
    ଯଦି (iter->index >= (int32_t)ARRAY_SIZE(keysym_to_name) - 1)
        ଫେରନ୍ତୁ false;

    /* Next keysym */
    ଯଦି (iter->explicit || iter->keysym >= XKB_KEYSYM_UNICODE_MAX ||
        keysym_to_name[iter->index + 1].keysym < XKB_KEYSYM_UNICODE_MIN) {
        /* Explicitly named keysyms only */
        iter->keysym = keysym_to_name[++iter->index].keysym;
        ପ୍ରତିଜ୍ଞା(iter->explicit ||
               iter->keysym <= XKB_KEYSYM_UNICODE_MIN ||
               iter->keysym >= XKB_KEYSYM_UNICODE_MAX);
    } ଅନ୍ୟଥା {
        /* Unicode keysyms
         * NOTE: Unicode keysyms are within keysym_to_name keysyms range. */
        ଯଦି (iter->keysym >= keysym_to_name[iter->index].keysym)
            iter->index++;
        ଯଦି (iter->keysym >= XKB_KEYSYM_UNICODE_MIN) {
            /* Continue Unicode keysyms */
            iter->keysym++;
        } ଅନ୍ୟଥା {
            /* Start Unicode keysyms */
            iter->keysym = XKB_KEYSYM_UNICODE_MIN;
        }
    }
    ଫେରନ୍ତୁ true;
}

/* Parse the numeric part of a 0xXXXX and UXXXX keysym. */
ସ୍ଥିର bool
parse_keysym_hex(ଅଚଳ ଅକ୍ଷର *s, uint32_t *out)
{
    /* We expect a NULL-terminated string of length up to 8 */
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = parse_hex_to_uint32_t(s, 8, out);
    /* Check that some value was parsed and we reached the end of the string */
    ଫେରନ୍ତୁ (ଗଣନା_2 > 0 && s[ଗଣନା_2] == '\0');
}

xkb_keysym_t
xkb_keysym_from_name(ଅଚଳ ଅକ୍ଷର *name, ଗଣନା xkb_keysym_flags flags)
{
    ଯଦି (flags & ~(ଗଣନା xkb_keysym_flags)XKB_KEYSYM_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ)
        ଫେରନ୍ତୁ XKB_ଚାବି_No_ପ୍ରତୀକ;

    ଅଚଳ ସଂରଚନା name_keysym *entry = ଖାଲି;
    ଅକ୍ଷର *tmp;
    uint32_t val;
    bool icase = (flags & XKB_KEYSYM_CASE_INSENSITIVE);

    /*
     * We need to !icase case to be fast, for e.g. Compose file parsing.
     * So do it in a fast path.
     */
    ଯଦି (!icase) {
        size_t pos = keysym_name_perfect_hash(name);
        ଯଦି (pos < ARRAY_SIZE(name_to_keysym)) {
            ଅଚଳ ଅକ୍ଷର *s = get_name(&name_to_keysym[pos]);
            ଯଦି (strcmp(name, s) == 0)
                ଫେରନ୍ତୁ name_to_keysym[pos].keysym;
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
    ଅନ୍ୟଥା {
        int32_t lo = 0, hi = ARRAY_SIZE(name_to_keysym) - 1;
        ଯେତେବେଳେ (hi >= lo) {
            int32_t mid = (lo + hi) / 2;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp = istrcmp(name, get_name(&name_to_keysym[mid]));
            ଯଦି (cmp > 0) {
                lo = mid + 1;
            } ଅନ୍ୟଥା ଯଦି (cmp < 0) {
                hi = mid - 1;
            } ଅନ୍ୟଥା {
                entry = &name_to_keysym[mid];
                ଭାଙ୍ଗ;
            }
        }
        ଯଦି (entry) {
            ଅଚଳ ସଂରଚନା name_keysym *last;
            last = name_to_keysym + ARRAY_SIZE(name_to_keysym) - 1;
            /* Keep going until we reach end of array
             * or non case-insensitve match */
            ଯେତେବେଳେ (entry < last &&
                   istrcmp(get_name(entry + 1), get_name(entry)) == 0) {
                entry++;
            }
            ଫେରନ୍ତୁ entry->keysym;
        }
    }

    ଯଦି (*name == 'U' || (icase && *name == 'u')) {
        /* Parse Unicode notation */
        ଯଦି (!parse_keysym_hex(&name[1], &val))
            ଫେରନ୍ତୁ XKB_ଚାବି_No_ପ୍ରତୀକ;
        ଫେରନ୍ତୁ (val > 0xff && val <= 0x10ffff)
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
    ଅନ୍ୟଥା ଯଦି (name[0] == '0' && (name[1] == 'x' || (icase && name[1] == 'X'))) {
        /*
         * Parse numeric hexadecimal notation without any normalization, in
         * in order to be consistent with the keymap files parsing.
         */
        ଯଦି (!parse_keysym_hex(&name[2], &val) || val > XKB_KEYSYM_MAX)
            ଫେରନ୍ତୁ XKB_ଚାବି_No_ପ୍ରତୀକ;
        ଫେରନ୍ତୁ (xkb_keysym_t) val;
    }

    /* Stupid inconsistency between the headers and XKeysymDB: the former has
     * no separating underscore, while some XF86* syms in the latter did.
     * As a last ditch effort, try without. */
    ଯଦି (strncmp(name, "XF86_", 5) == 0 ||
        (icase && istrncmp(name, "XF86_", 5) == 0)) {
        xkb_keysym_t ret;
        tmp = strdup(name);
        ଯଦି (!tmp)
            ଫେରନ୍ତୁ XKB_ଚାବି_No_ପ୍ରତୀକ;
        memmove(&tmp[4], &tmp[5], strlen(name) - 5 + 1);
        ret = xkb_keysym_from_name(tmp, flags);
        ମୁକ୍ତ(tmp);
        ଫେରନ୍ତୁ ret;
    }

    ଫେରନ୍ତୁ XKB_ଚାବି_No_ପ୍ରତୀକ;
}

xkb_keysym_t
xkb_utf8_to_keysym(ଅଚଳ ଅକ୍ଷର *buffer, size_t size)
{
    ଯଦି (!buffer || !size)
        ଫେରନ୍ତୁ 0;

    /* Try to parse the parameter as a single UTF-8 encoded character */
    size_t length = 0;
    ଅଚଳ uint32_t codepoint = utf8_next_code_point(buffer, size, &length);
    ଫେରନ୍ତୁ (codepoint == INVALID_UTF8_CODE_POINT || length == 0)
        ? XKB_ଚାବି_No_ପ୍ରତୀକ
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
                         ଅଚଳ ଅକ୍ଷର *name,
                         ଅଚଳ ଅକ୍ଷର **reference_name)
{
    ଯଦି (keysym > XKB_KEYSYM_MAX) {
        /* Invalid keysym */
        *reference_name = ଖାଲି;
        ଫେରନ୍ତୁ false;
    }
    /* [WARNING] We do not check that name, if defined, is a valid for keysym */

    int32_t lo = 0, hi = ARRAY_SIZE(deprecated_keysyms) - 1;
    ଯେତେବେଳେ (hi >= lo) {
        int32_t mid = (lo + hi) / 2;
        ଯଦି (keysym > deprecated_keysyms[mid].keysym) {
            lo = mid + 1;
        } ଅନ୍ୟଥା ଯଦି (keysym < deprecated_keysyms[mid].keysym) {
            hi = mid - 1;
        } ଅନ୍ୟଥା {
            /* Keysym have some deprecated names */
            ଯଦି (deprecated_keysyms[mid].offset == DEPRECATED_KEYSYM) {
                /* All names are deprecated */
                *reference_name = ଖାଲି;
                ଫେରନ୍ତୁ true;
            } ଅନ୍ୟଥା ଯଦି (deprecated_keysyms[mid].offset == UNICODE_KEYSYM) {
                /* All names are deprecated, but Unicode notation is valid */
                *reference_name = ଖାଲି;
                ଯଦି (name == ଖାଲି)
                    ଫେରନ୍ତୁ false;
                ଯଦି (name[0] != 'U')
                    ଫେରନ୍ତୁ true;
                /* Since the current function is internal, assume the name
                 * corresponds to the keysym value and just check its syntax. */
                ଅଚଳ ଅକ୍ଷର *start = name + 1;
                ଯେତେବେଳେ (is_xdigit(*start))
                    start++;
                ଫେରନ୍ତୁ *start != '\0' && start > name + 1;
            }
            /* There is a reference name that is not deprecated */
            *reference_name = get_name_by_index(deprecated_keysyms[mid].offset);
            ଯଦି (!name) {
                /* No name to check: indicate not deprecated */
                ଫେରନ୍ତୁ false;
            }
            ଯଦି (deprecated_keysyms[mid].explicit_ଗଣନା) {
                /* Only some explicit names are deprecated */
                ଅଚଳ uint8_t k_max = deprecated_keysyms[mid].explicit_index
                                    + deprecated_keysyms[mid].explicit_ଗଣନା;
                ପ୍ରତିଜ୍ଞା(k_max <= ARRAY_SIZE(explicit_deprecated_aliases));
                /* Check every deprecated alias */
                ପାଇଁ (uint8_t k = deprecated_keysyms[mid].explicit_index;
                     k < k_max; k++) {
                    #ifdef __clang__
                    /* Make clang-tidy happy */
                    __builtin_assume(k_max <=
                                     ARRAY_SIZE(explicit_deprecated_aliases));
                    #endif
                    ଅଚଳ ଅକ୍ଷର *alias =
                        get_name_by_index(explicit_deprecated_aliases[k]);
                    ଯଦି (strcmp(name, alias) == 0)
                        ଫେରନ୍ତୁ true;
                }
                ଫେରନ୍ତୁ false;
            } ଅନ୍ୟଥା {
                /* All names but the reference one are deprecated */
                ଫେରନ୍ତୁ strcmp(name, *reference_name) != 0;
            }
        }
    }

    /* Keysym has no deprecated names */
    *reference_name = ଖାଲି;
    ଫେରନ୍ତୁ false;
}

bool
xkb_keysym_is_keypad(xkb_keysym_t keysym)
{
    ଫେରନ୍ତୁ keysym >= XKB_ଚାବି_KP_Space && keysym <= XKB_ଚାବି_KP_Equal;
}


bool
xkb_keysym_is_modifier(xkb_keysym_t keysym)
{
    ଫେରନ୍ତୁ
        (keysym >= XKB_ଚାବି_Shift_L && keysym <= XKB_ଚାବି_Hyper_R) ||
        (keysym >= XKB_ଚାବି_ISO_Lock && keysym <= XKB_ଚାବି_ISO_Level5_Lock) ||
        keysym == XKB_ଚାବି_Mode_switch ||
        keysym == XKB_ଚାବି_ଅଙ୍କ_Lock;
}
