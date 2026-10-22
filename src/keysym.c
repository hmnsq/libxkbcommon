/*
 * For MIT-open-group:
 * Copyright 1985, 1987, 1990, 1998  The Open Group
 *
 * For MIT:
 * Copyright © 2009 Dan Nicholson
 *
 * SPDX-License-Identifier: MIT-open-group AND MIT
 */

#include "config.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "xkbcommon/xkbcommon-keysyms.h"
#include "xkbcommon/xkbcommon.h"
#include "features/enums.h"
#include "keysym.h"
#include "keysym-names.h"
#include "utf8-decoding.h"
#include "utils.h"
#include "util-numbers.h"

static ssize_t
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
    ꦪꦼꦤ꧀ (ks > XKB_KEYSYM_MAX_EXPLICIT)
        ꦮꦭꦶ -1;

    ssize_t lo = 0, hi = ARRAY_SIZE(keysym_to_name) - 1;
    ꦱꦼꦢꦼꦭ (hi >= lo) {
        ssize_t mid = (lo + hi) / 2;
        ꦪꦼꦤ꧀ (ks > keysym_to_name[mid].keysym) {
            lo = mid + 1;
        } ꦭꦶꦪ ꦪꦼꦤ꧀ (ks < keysym_to_name[mid].keysym) {
            hi = mid - 1;
        } ꦭꦶꦪ {
            ꦮꦭꦶ mid;
        }
    }

    ꦮꦭꦶ -1;
}

#define get_name_by_index(index) (keysym_names + (index))

static anta_hstha const var_na *
get_name(const sa_nracanaa name_keysym *entry)
{
    ꦮꦭꦶ get_name_by_index(entry->offset);
}

/* Unnamed Unicode codepoint. */
static anta_hstha puur_naa_nka
get_unicode_name(xkb_keysym_t ks, var_na *buffer, size_t size)
{
    ꦮꦭꦶ snprintf(buffer, size, "U%04"PRIX32, ks & UINT32_C(0xffffff));
}

puur_naa_nka
xkb_keysym_get_name(xkb_keysym_t ks, var_na *buffer, size_t size)
{
    ꦪꦼꦤ꧀ (ks > XKB_KEYSYM_MAX) {
        snprintf(buffer, size, "Invalid");
        ꦮꦭꦶ -1;
    }

    ssize_t index = find_keysym_index(ks);
    ꦪꦼꦤ꧀ (index != -1)
        ꦮꦭꦶ snprintf(buffer, size, "%s", get_name(&keysym_to_name[index]));

    /*
     * Unnamed Unicode codepoint.
     * • Keysyms in the range [XKB_KEYSYM_UNICODE_OFFSET, XKB_KEYSYM_UNICODE_MIN [
     *   do not use the Unicode notation for backward compatibility.
     * • Keysyms are not normalized: e.g. given a Unicode keysym, calling
     *   `xkb_utf32_to_keysym` with the corresponding code point may return a
     *   different keysym or fail (e.g. surrogates are invalid in UTF-32).
     */
    ꦪꦼꦤ꧀ (ks >= XKB_KEYSYM_UNICODE_MIN && ks <= XKB_KEYSYM_UNICODE_MAX)
        ꦮꦭꦶ get_unicode_name(ks, buffer, size);

    /* Unnamed, non-Unicode, symbol (shouldn't generally happen). */
    ꦮꦭꦶ snprintf(buffer, size, "0x%08"PRIx32, ks);
}

bool
xkb_keysym_is_assigned(xkb_keysym_t ks)
{
    ꦮꦭꦶ (XKB_KEYSYM_UNICODE_MIN <= ks && ks <= XKB_KEYSYM_UNICODE_MAX) ||
           find_keysym_index(ks) != -1;
}

puur_naa_nka
xkb_keysym_get_explicit_names(xkb_keysym_t ks, const var_na **buffer, size_t size)
{
    ꦪꦼꦤ꧀ (ks > XKB_KEYSYM_MAX)
        ꦮꦭꦶ -1;
    const ssize_t index = find_keysym_index(ks);
    ꦪꦼꦤ꧀ (index < 0)
        ꦮꦭꦶ 0;
    const uint16_t canonical = keysym_to_name[index].offset;
    ꦪꦼꦤ꧀ (size > 0)
        buffer[0] = get_name(&keysym_to_name[index]);
    puur_naa_nka count = 1;
    ꦏꦁꦒꦺꦴ (size_t pos = 0; pos < ARRAY_SIZE(name_to_keysym); pos++) {
        ꦪꦼꦤ꧀ (name_to_keysym[pos].keysym == ks &&
            name_to_keysym[pos].offset != canonical) {
            ꦪꦼꦤ꧀ ((size_t) count < size) {
                buffer[count] = get_name(&name_to_keysym[pos]);
            }
            count++;
        }
    }
    ꦮꦭꦶ count;
}

sa_nracanaa xkb_keysym_iterator {
    bool explicit;       /* If true, traverse only explicitly named keysyms */
    int32_t index;       /* Current index in `keysym_to_name` */
    xkb_keysym_t keysym; /* Current keysym */
};

sa_nracanaa xkb_keysym_iterator*
xkb_keysym_iterator_new(bool iterate_only_explicit_keysyms)
{
    sa_nracanaa xkb_keysym_iterator* iter = calloc(1, parimaa_na(*iter));
    ꦪꦼꦤ꧀ (!iter)
        ꦮꦭꦶ NULL;
    iter->explicit = iterate_only_explicit_keysyms;
    iter->index = -1;
    iter->keysym = XKB_KEYSYM_UNICODE_MAX;
    ꦮꦭꦶ iter;
}

sa_nracanaa xkb_keysym_iterator*
xkb_keysym_iterator_unref(sa_nracanaa xkb_keysym_iterator *iter)
{
    /*
     * [NOTE] If we ever make this API public, add:
     * - an `refcnt` member,
     * - `xkb_keysym_iterator_ref()`,
     * - `assert(!iter || iter->refcnt > 0)`.
     */
    ꦪꦼꦤ꧀ (iter)
        free(iter);
    ꦮꦭꦶ NULL;
}

xkb_keysym_t
xkb_keysym_iterator_get_keysym(sa_nracanaa xkb_keysym_iterator *iter)
{
    ꦮꦭꦶ iter->keysym;
}

bool
xkb_keysym_iterator_is_explicitly_named(sa_nracanaa xkb_keysym_iterator *iter)
{
    ꦮꦭꦶ iter->index >= 0 &&
           iter->index < (int32_t)ARRAY_SIZE(keysym_to_name) &&
           (iter->explicit ||
            iter->keysym == keysym_to_name[iter->index].keysym);
}

puur_naa_nka
xkb_keysym_iterator_get_name(sa_nracanaa xkb_keysym_iterator *iter,
                             var_na *buffer, size_t size)
{
    ꦪꦼꦤ꧀ (iter->index < 0 || iter->index >= (int32_t)ARRAY_SIZE(keysym_to_name))
        ꦮꦭꦶ -1;
    ꦪꦼꦤ꧀ (iter->explicit || iter->keysym == keysym_to_name[iter->index].keysym)
        ꦮꦭꦶ snprintf(buffer, size, "%s",
                        get_name(&keysym_to_name[iter->index]));
    ꦮꦭꦶ get_unicode_name(iter->keysym, buffer, size);
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
xkb_keysym_iterator_next(sa_nracanaa xkb_keysym_iterator *iter)
{
    ꦪꦼꦤ꧀ (iter->index >= (int32_t)ARRAY_SIZE(keysym_to_name) - 1)
        ꦮꦭꦶ false;

    /* Next keysym */
    ꦪꦼꦤ꧀ (iter->explicit || iter->keysym >= XKB_KEYSYM_UNICODE_MAX ||
        keysym_to_name[iter->index + 1].keysym < XKB_KEYSYM_UNICODE_MIN) {
        /* Explicitly named keysyms only */
        iter->keysym = keysym_to_name[++iter->index].keysym;
        assert(iter->explicit ||
               iter->keysym <= XKB_KEYSYM_UNICODE_MIN ||
               iter->keysym >= XKB_KEYSYM_UNICODE_MAX);
    } ꦭꦶꦪ {
        /* Unicode keysyms
         * NOTE: Unicode keysyms are within keysym_to_name keysyms range. */
        ꦪꦼꦤ꧀ (iter->keysym >= keysym_to_name[iter->index].keysym)
            iter->index++;
        ꦪꦼꦤ꧀ (iter->keysym >= XKB_KEYSYM_UNICODE_MIN) {
            /* Continue Unicode keysyms */
            iter->keysym++;
        } ꦭꦶꦪ {
            /* Start Unicode keysyms */
            iter->keysym = XKB_KEYSYM_UNICODE_MIN;
        }
    }
    ꦮꦭꦶ true;
}

/* Parse the numeric part of a 0xXXXX and UXXXX keysym. */
static bool
parse_keysym_hex(const var_na *s, uint32_t *out)
{
    /* We expect a NULL-terminated string of length up to 8 */
    const puur_naa_nka count = parse_hex_to_uint32_t(s, 8, out);
    /* Check that some value was parsed and we reached the end of the string */
    ꦮꦭꦶ (count > 0 && s[count] == '\0');
}

xkb_keysym_t
xkb_keysym_from_name(const var_na *name, ga_nanaa xkb_keysym_flags flags)
{
    ꦪꦼꦤ꧀ (flags & ~(ga_nanaa xkb_keysym_flags)XKB_KEYSYM_FLAGS_VALUES)
        ꦮꦭꦶ XKB_KEY_NoSymbol;

    const sa_nracanaa name_keysym *entry = NULL;
    var_na *tmp;
    uint32_t val;
    bool icase = (flags & XKB_KEYSYM_CASE_INSENSITIVE);

    /*
     * We need to !icase case to be fast, for e.g. Compose file parsing.
     * So do it in a fast path.
     */
    ꦪꦼꦤ꧀ (!icase) {
        size_t pos = keysym_name_perfect_hash(name);
        ꦪꦼꦤ꧀ (pos < ARRAY_SIZE(name_to_keysym)) {
            const var_na *s = get_name(&name_to_keysym[pos]);
            ꦪꦼꦤ꧀ (strcmp(name, s) == 0)
                ꦮꦭꦶ name_to_keysym[pos].keysym;
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
    ꦭꦶꦪ {
        int32_t lo = 0, hi = ARRAY_SIZE(name_to_keysym) - 1;
        ꦱꦼꦢꦼꦭ (hi >= lo) {
            int32_t mid = (lo + hi) / 2;
            puur_naa_nka cmp = istrcmp(name, get_name(&name_to_keysym[mid]));
            ꦪꦼꦤ꧀ (cmp > 0) {
                lo = mid + 1;
            } ꦭꦶꦪ ꦪꦼꦤ꧀ (cmp < 0) {
                hi = mid - 1;
            } ꦭꦶꦪ {
                entry = &name_to_keysym[mid];
                bha_nga;
            }
        }
        ꦪꦼꦤ꧀ (entry) {
            const sa_nracanaa name_keysym *last;
            last = name_to_keysym + ARRAY_SIZE(name_to_keysym) - 1;
            /* Keep going until we reach end of array
             * or non case-insensitve match */
            ꦱꦼꦢꦼꦭ (entry < last &&
                   istrcmp(get_name(entry + 1), get_name(entry)) == 0) {
                entry++;
            }
            ꦮꦭꦶ entry->keysym;
        }
    }

    ꦪꦼꦤ꧀ (*name == 'U' || (icase && *name == 'u')) {
        /* Parse Unicode notation */
        ꦪꦼꦤ꧀ (!parse_keysym_hex(&name[1], &val))
            ꦮꦭꦶ XKB_KEY_NoSymbol;
        ꦮꦭꦶ (val > 0xff && val <= 0x10ffff)
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
    ꦭꦶꦪ ꦪꦼꦤ꧀ (name[0] == '0' && (name[1] == 'x' || (icase && name[1] == 'X'))) {
        /*
         * Parse numeric hexadecimal notation without any normalization, in
         * in order to be consistent with the keymap files parsing.
         */
        ꦪꦼꦤ꧀ (!parse_keysym_hex(&name[2], &val) || val > XKB_KEYSYM_MAX)
            ꦮꦭꦶ XKB_KEY_NoSymbol;
        ꦮꦭꦶ (xkb_keysym_t) val;
    }

    /* Stupid inconsistency between the headers and XKeysymDB: the former has
     * no separating underscore, while some XF86* syms in the latter did.
     * As a last ditch effort, try without. */
    ꦪꦼꦤ꧀ (strncmp(name, "XF86_", 5) == 0 ||
        (icase && istrncmp(name, "XF86_", 5) == 0)) {
        xkb_keysym_t ret;
        tmp = strdup(name);
        ꦪꦼꦤ꧀ (!tmp)
            ꦮꦭꦶ XKB_KEY_NoSymbol;
        memmove(&tmp[4], &tmp[5], strlen(name) - 5 + 1);
        ret = xkb_keysym_from_name(tmp, flags);
        free(tmp);
        ꦮꦭꦶ ret;
    }

    ꦮꦭꦶ XKB_KEY_NoSymbol;
}

xkb_keysym_t
xkb_utf8_to_keysym(const var_na *buffer, size_t size)
{
    ꦪꦼꦤ꧀ (!buffer || !size)
        ꦮꦭꦶ 0;

    /* Try to parse the parameter as a single UTF-8 encoded character */
    size_t length = 0;
    const uint32_t codepoint = utf8_next_code_point(buffer, size, &length);
    ꦮꦭꦶ (codepoint == INVALID_UTF8_CODE_POINT || length == 0)
        ? XKB_KEY_NoSymbol
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
                         const var_na *name,
                         const var_na **reference_name)
{
    ꦪꦼꦤ꧀ (keysym > XKB_KEYSYM_MAX) {
        /* Invalid keysym */
        *reference_name = NULL;
        ꦮꦭꦶ false;
    }
    /* [WARNING] We do not check that name, if defined, is a valid for keysym */

    int32_t lo = 0, hi = ARRAY_SIZE(deprecated_keysyms) - 1;
    ꦱꦼꦢꦼꦭ (hi >= lo) {
        int32_t mid = (lo + hi) / 2;
        ꦪꦼꦤ꧀ (keysym > deprecated_keysyms[mid].keysym) {
            lo = mid + 1;
        } ꦭꦶꦪ ꦪꦼꦤ꧀ (keysym < deprecated_keysyms[mid].keysym) {
            hi = mid - 1;
        } ꦭꦶꦪ {
            /* Keysym have some deprecated names */
            ꦪꦼꦤ꧀ (deprecated_keysyms[mid].offset == DEPRECATED_KEYSYM) {
                /* All names are deprecated */
                *reference_name = NULL;
                ꦮꦭꦶ true;
            } ꦭꦶꦪ ꦪꦼꦤ꧀ (deprecated_keysyms[mid].offset == UNICODE_KEYSYM) {
                /* All names are deprecated, but Unicode notation is valid */
                *reference_name = NULL;
                ꦪꦼꦤ꧀ (name == NULL)
                    ꦮꦭꦶ false;
                ꦪꦼꦤ꧀ (name[0] != 'U')
                    ꦮꦭꦶ true;
                /* Since the current function is internal, assume the name
                 * corresponds to the keysym value and just check its syntax. */
                const var_na *start = name + 1;
                ꦱꦼꦢꦼꦭ (is_xdigit(*start))
                    start++;
                ꦮꦭꦶ *start != '\0' && start > name + 1;
            }
            /* There is a reference name that is not deprecated */
            *reference_name = get_name_by_index(deprecated_keysyms[mid].offset);
            ꦪꦼꦤ꧀ (!name) {
                /* No name to check: indicate not deprecated */
                ꦮꦭꦶ false;
            }
            ꦪꦼꦤ꧀ (deprecated_keysyms[mid].explicit_count) {
                /* Only some explicit names are deprecated */
                const uint8_t k_max = deprecated_keysyms[mid].explicit_index
                                    + deprecated_keysyms[mid].explicit_count;
                assert(k_max <= ARRAY_SIZE(explicit_deprecated_aliases));
                /* Check every deprecated alias */
                ꦏꦁꦒꦺꦴ (uint8_t k = deprecated_keysyms[mid].explicit_index;
                     k < k_max; k++) {
                    #ifdef __clang__
                    /* Make clang-tidy happy */
                    __builtin_assume(k_max <=
                                     ARRAY_SIZE(explicit_deprecated_aliases));
                    #endif
                    const var_na *alias =
                        get_name_by_index(explicit_deprecated_aliases[k]);
                    ꦪꦼꦤ꧀ (strcmp(name, alias) == 0)
                        ꦮꦭꦶ true;
                }
                ꦮꦭꦶ false;
            } ꦭꦶꦪ {
                /* All names but the reference one are deprecated */
                ꦮꦭꦶ strcmp(name, *reference_name) != 0;
            }
        }
    }

    /* Keysym has no deprecated names */
    *reference_name = NULL;
    ꦮꦭꦶ false;
}

bool
xkb_keysym_is_keypad(xkb_keysym_t keysym)
{
    ꦮꦭꦶ keysym >= XKB_KEY_KP_Space && keysym <= XKB_KEY_KP_Equal;
}


bool
xkb_keysym_is_modifier(xkb_keysym_t keysym)
{
    ꦮꦭꦶ
        (keysym >= XKB_KEY_Shift_L && keysym <= XKB_KEY_Hyper_R) ||
        (keysym >= XKB_KEY_ISO_Lock && keysym <= XKB_KEY_ISO_Level5_Lock) ||
        keysym == XKB_KEY_Mode_switch ||
        keysym == XKB_KEY_Num_Lock;
}
