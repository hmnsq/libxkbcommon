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
    ако (ks > XKB_KEYSYM_MAX_EXPLICIT)
        врати -1;

    ssize_t lo = 0, hi = ARRAY_SIZE(keysym_to_name) - 1;
    док (hi >= lo) {
        ssize_t mid = (lo + hi) / 2;
        ако (ks > keysym_to_name[mid].keysym) {
            lo = mid + 1;
        } инако ако (ks < keysym_to_name[mid].keysym) {
            hi = mid - 1;
        } инако {
            врати mid;
        }
    }

    врати -1;
}

#define get_name_by_index(index) (keysym_names + (index))

static inline const char *
get_name(const struct name_keysym *entry)
{
    врати get_name_by_index(entry->offset);
}

/* Unnamed Unicode codepoint. */
static inline цео
get_unicode_name(xkb_keysym_t ks, char *buffer, size_t size)
{
    врати snprintf(buffer, size, "U%04"PRIX32, ks & UINT32_C(0xffffff));
}

цео
xkb_keysym_get_name(xkb_keysym_t ks, char *buffer, size_t size)
{
    ако (ks > XKB_KEYSYM_MAX) {
        snprintf(buffer, size, "Invalid");
        врати -1;
    }

    ssize_t index = find_keysym_index(ks);
    ако (index != -1)
        врати snprintf(buffer, size, "%s", get_name(&keysym_to_name[index]));

    /*
     * Unnamed Unicode codepoint.
     * • Keysyms in the range [XKB_KEYSYM_UNICODE_OFFSET, XKB_KEYSYM_UNICODE_MIN [
     *   do not use the Unicode notation for backward compatibility.
     * • Keysyms are not normalized: e.g. given a Unicode keysym, calling
     *   `xkb_utf32_to_keysym` with the corresponding code point may return a
     *   different keysym or fail (e.g. surrogates are invalid in UTF-32).
     */
    ако (ks >= XKB_KEYSYM_UNICODE_MIN && ks <= XKB_KEYSYM_UNICODE_MAX)
        врати get_unicode_name(ks, buffer, size);

    /* Unnamed, non-Unicode, symbol (shouldn't generally happen). */
    врати snprintf(buffer, size, "0x%08"PRIx32, ks);
}

bool
xkb_keysym_is_assigned(xkb_keysym_t ks)
{
    врати (XKB_KEYSYM_UNICODE_MIN <= ks && ks <= XKB_KEYSYM_UNICODE_MAX) ||
           find_keysym_index(ks) != -1;
}

цео
xkb_keysym_get_explicit_names(xkb_keysym_t ks, const char **buffer, size_t size)
{
    ако (ks > XKB_KEYSYM_MAX)
        врати -1;
    const ssize_t index = find_keysym_index(ks);
    ако (index < 0)
        врати 0;
    const uint16_t canonical = keysym_to_name[index].offset;
    ако (size > 0)
        buffer[0] = get_name(&keysym_to_name[index]);
    цео count = 1;
    за (size_t pos = 0; pos < ARRAY_SIZE(name_to_keysym); pos++) {
        ако (name_to_keysym[pos].keysym == ks &&
            name_to_keysym[pos].offset != canonical) {
            ако ((size_t) count < size) {
                buffer[count] = get_name(&name_to_keysym[pos]);
            }
            count++;
        }
    }
    врати count;
}

struct xkb_keysym_iterator {
    bool explicit;       /* If true, traverse only explicitly named keysyms */
    int32_t index;       /* Current index in `keysym_to_name` */
    xkb_keysym_t keysym; /* Current keysym */
};

struct xkb_keysym_iterator*
xkb_keysym_iterator_new(bool iterate_only_explicit_keysyms)
{
    struct xkb_keysym_iterator* iter = calloc(1, sizeof(*iter));
    ако (!iter)
        врати NULL;
    iter->explicit = iterate_only_explicit_keysyms;
    iter->index = -1;
    iter->keysym = XKB_KEYSYM_UNICODE_MAX;
    врати iter;
}

struct xkb_keysym_iterator*
xkb_keysym_iterator_unref(struct xkb_keysym_iterator *iter)
{
    /*
     * [NOTE] If we ever make this API public, add:
     * - an `refcnt` member,
     * - `xkb_keysym_iterator_ref()`,
     * - `assert(!iter || iter->refcnt > 0)`.
     */
    ако (iter)
        free(iter);
    врати NULL;
}

xkb_keysym_t
xkb_keysym_iterator_get_keysym(struct xkb_keysym_iterator *iter)
{
    врати iter->keysym;
}

bool
xkb_keysym_iterator_is_explicitly_named(struct xkb_keysym_iterator *iter)
{
    врати iter->index >= 0 &&
           iter->index < (int32_t)ARRAY_SIZE(keysym_to_name) &&
           (iter->explicit ||
            iter->keysym == keysym_to_name[iter->index].keysym);
}

цео
xkb_keysym_iterator_get_name(struct xkb_keysym_iterator *iter,
                             char *buffer, size_t size)
{
    ако (iter->index < 0 || iter->index >= (int32_t)ARRAY_SIZE(keysym_to_name))
        врати -1;
    ако (iter->explicit || iter->keysym == keysym_to_name[iter->index].keysym)
        врати snprintf(buffer, size, "%s",
                        get_name(&keysym_to_name[iter->index]));
    врати get_unicode_name(iter->keysym, buffer, size);
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
xkb_keysym_iterator_next(struct xkb_keysym_iterator *iter)
{
    ако (iter->index >= (int32_t)ARRAY_SIZE(keysym_to_name) - 1)
        врати false;

    /* Next keysym */
    ако (iter->explicit || iter->keysym >= XKB_KEYSYM_UNICODE_MAX ||
        keysym_to_name[iter->index + 1].keysym < XKB_KEYSYM_UNICODE_MIN) {
        /* Explicitly named keysyms only */
        iter->keysym = keysym_to_name[++iter->index].keysym;
        assert(iter->explicit ||
               iter->keysym <= XKB_KEYSYM_UNICODE_MIN ||
               iter->keysym >= XKB_KEYSYM_UNICODE_MAX);
    } инако {
        /* Unicode keysyms
         * NOTE: Unicode keysyms are within keysym_to_name keysyms range. */
        ако (iter->keysym >= keysym_to_name[iter->index].keysym)
            iter->index++;
        ако (iter->keysym >= XKB_KEYSYM_UNICODE_MIN) {
            /* Continue Unicode keysyms */
            iter->keysym++;
        } инако {
            /* Start Unicode keysyms */
            iter->keysym = XKB_KEYSYM_UNICODE_MIN;
        }
    }
    врати true;
}

/* Parse the numeric part of a 0xXXXX and UXXXX keysym. */
static bool
parse_keysym_hex(const char *s, uint32_t *out)
{
    /* We expect a NULL-terminated string of length up to 8 */
    const цео count = parse_hex_to_uint32_t(s, 8, out);
    /* Check that some value was parsed and we reached the end of the string */
    врати (count > 0 && s[count] == '\0');
}

xkb_keysym_t
xkb_keysym_from_name(const char *name, enum xkb_keysym_flags flags)
{
    ако (flags & ~(enum xkb_keysym_flags)XKB_KEYSYM_FLAGS_VALUES)
        врати XKB_KEY_NoSymbol;

    const struct name_keysym *entry = NULL;
    char *tmp;
    uint32_t val;
    bool icase = (flags & XKB_KEYSYM_CASE_INSENSITIVE);

    /*
     * We need to !icase case to be fast, for e.g. Compose file parsing.
     * So do it in a fast path.
     */
    ако (!icase) {
        size_t pos = keysym_name_perfect_hash(name);
        ако (pos < ARRAY_SIZE(name_to_keysym)) {
            const char *s = get_name(&name_to_keysym[pos]);
            ако (strcmp(name, s) == 0)
                врати name_to_keysym[pos].keysym;
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
    инако {
        int32_t lo = 0, hi = ARRAY_SIZE(name_to_keysym) - 1;
        док (hi >= lo) {
            int32_t mid = (lo + hi) / 2;
            цео cmp = istrcmp(name, get_name(&name_to_keysym[mid]));
            ако (cmp > 0) {
                lo = mid + 1;
            } инако ако (cmp < 0) {
                hi = mid - 1;
            } инако {
                entry = &name_to_keysym[mid];
                break;
            }
        }
        ако (entry) {
            const struct name_keysym *last;
            last = name_to_keysym + ARRAY_SIZE(name_to_keysym) - 1;
            /* Keep going until we reach end of array
             * or non case-insensitve match */
            док (entry < last &&
                   istrcmp(get_name(entry + 1), get_name(entry)) == 0) {
                entry++;
            }
            врати entry->keysym;
        }
    }

    ако (*name == 'U' || (icase && *name == 'u')) {
        /* Parse Unicode notation */
        ако (!parse_keysym_hex(&name[1], &val))
            врати XKB_KEY_NoSymbol;
        врати (val > 0xff && val <= 0x10ffff)
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
    инако ако (name[0] == '0' && (name[1] == 'x' || (icase && name[1] == 'X'))) {
        /*
         * Parse numeric hexadecimal notation without any normalization, in
         * in order to be consistent with the keymap files parsing.
         */
        ако (!parse_keysym_hex(&name[2], &val) || val > XKB_KEYSYM_MAX)
            врати XKB_KEY_NoSymbol;
        врати (xkb_keysym_t) val;
    }

    /* Stupid inconsistency between the headers and XKeysymDB: the former has
     * no separating underscore, while some XF86* syms in the latter did.
     * As a last ditch effort, try without. */
    ако (strncmp(name, "XF86_", 5) == 0 ||
        (icase && istrncmp(name, "XF86_", 5) == 0)) {
        xkb_keysym_t ret;
        tmp = strdup(name);
        ако (!tmp)
            врати XKB_KEY_NoSymbol;
        memmove(&tmp[4], &tmp[5], strlen(name) - 5 + 1);
        ret = xkb_keysym_from_name(tmp, flags);
        free(tmp);
        врати ret;
    }

    врати XKB_KEY_NoSymbol;
}

xkb_keysym_t
xkb_utf8_to_keysym(const char *buffer, size_t size)
{
    ако (!buffer || !size)
        врати 0;

    /* Try to parse the parameter as a single UTF-8 encoded character */
    size_t length = 0;
    const uint32_t codepoint = utf8_next_code_point(buffer, size, &length);
    врати (codepoint == INVALID_UTF8_CODE_POINT || length == 0)
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
                         const char *name,
                         const char **reference_name)
{
    ако (keysym > XKB_KEYSYM_MAX) {
        /* Invalid keysym */
        *reference_name = NULL;
        врати false;
    }
    /* [WARNING] We do not check that name, if defined, is a valid for keysym */

    int32_t lo = 0, hi = ARRAY_SIZE(deprecated_keysyms) - 1;
    док (hi >= lo) {
        int32_t mid = (lo + hi) / 2;
        ако (keysym > deprecated_keysyms[mid].keysym) {
            lo = mid + 1;
        } инако ако (keysym < deprecated_keysyms[mid].keysym) {
            hi = mid - 1;
        } инако {
            /* Keysym have some deprecated names */
            ако (deprecated_keysyms[mid].offset == DEPRECATED_KEYSYM) {
                /* All names are deprecated */
                *reference_name = NULL;
                врати true;
            } инако ако (deprecated_keysyms[mid].offset == UNICODE_KEYSYM) {
                /* All names are deprecated, but Unicode notation is valid */
                *reference_name = NULL;
                ако (name == NULL)
                    врати false;
                ако (name[0] != 'U')
                    врати true;
                /* Since the current function is internal, assume the name
                 * corresponds to the keysym value and just check its syntax. */
                const char *start = name + 1;
                док (is_xdigit(*start))
                    start++;
                врати *start != '\0' && start > name + 1;
            }
            /* There is a reference name that is not deprecated */
            *reference_name = get_name_by_index(deprecated_keysyms[mid].offset);
            ако (!name) {
                /* No name to check: indicate not deprecated */
                врати false;
            }
            ако (deprecated_keysyms[mid].explicit_count) {
                /* Only some explicit names are deprecated */
                const uint8_t k_max = deprecated_keysyms[mid].explicit_index
                                    + deprecated_keysyms[mid].explicit_count;
                assert(k_max <= ARRAY_SIZE(explicit_deprecated_aliases));
                /* Check every deprecated alias */
                за (uint8_t k = deprecated_keysyms[mid].explicit_index;
                     k < k_max; k++) {
                    #ifdef __clang__
                    /* Make clang-tidy happy */
                    __builtin_assume(k_max <=
                                     ARRAY_SIZE(explicit_deprecated_aliases));
                    #endif
                    const char *alias =
                        get_name_by_index(explicit_deprecated_aliases[k]);
                    ако (strcmp(name, alias) == 0)
                        врати true;
                }
                врати false;
            } инако {
                /* All names but the reference one are deprecated */
                врати strcmp(name, *reference_name) != 0;
            }
        }
    }

    /* Keysym has no deprecated names */
    *reference_name = NULL;
    врати false;
}

bool
xkb_keysym_is_keypad(xkb_keysym_t keysym)
{
    врати keysym >= XKB_KEY_KP_Space && keysym <= XKB_KEY_KP_Equal;
}


bool
xkb_keysym_is_modifier(xkb_keysym_t keysym)
{
    врати
        (keysym >= XKB_KEY_Shift_L && keysym <= XKB_KEY_Hyper_R) ||
        (keysym >= XKB_KEY_ISO_Lock && keysym <= XKB_KEY_ISO_Level5_Lock) ||
        keysym == XKB_KEY_Mode_switch ||
        keysym == XKB_KEY_Num_Lock;
}
