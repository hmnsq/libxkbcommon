/*
 * For MIT-open-group:
 * Copyright 1985, 1987, 1990, 1998  The Open Group
 *
 * For MIT:
 * Copyright © 2009 Dan Nicholson
 *
 * SPDX-License-Identifier: MIT-open-group AND MIT
 */

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <stdint.h>
#ضمن <stdlib.h>

#ضمن "xkbcommon/xkbcommon-keysyms.h"
#ضمن "xkbcommon/xkbcommon.h"
#ضمن "features/enums.h"
#ضمن "keysym.h"
#ضمن "keysym-names.h"
#ضمن "utf8-decoding.h"
#ضمن "utils.h"
#ضمن "util-numbers.h"

ساكن ssize_t
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
    إذا (ks > XKB_KEYSYM_MAX_EXPLICIT)
        أرجع -1;

    ssize_t lo = 0, hi = ARRAY_SIZE(keysym_to_name) - 1;
    طالما (hi >= lo) {
        ssize_t mid = (lo + hi) / 2;
        إذا (ks > keysym_to_name[mid].keysym) {
            lo = mid + 1;
        } وإلا إذا (ks < keysym_to_name[mid].keysym) {
            hi = mid - 1;
        } وإلا {
            أرجع mid;
        }
    }

    أرجع -1;
}

#define get_name_by_index(index) (keysym_names + (index))

ساكن مضمن ثابت حرف *
get_name(ثابت بنية name_keysym *entry)
{
    أرجع get_name_by_index(entry->offset);
}

/* Unnamed Unicode codepoint. */
ساكن مضمن صحيح
get_unicode_name(xkb_keysym_t ks, حرف *buffer, size_t size)
{
    أرجع snprintf(buffer, size, "U%04"PRIX32, ks & UINT32_C(0xffffff));
}

صحيح
xkb_keysym_get_name(xkb_keysym_t ks, حرف *buffer, size_t size)
{
    إذا (ks > XKB_KEYSYM_MAX) {
        snprintf(buffer, size, "Invalid");
        أرجع -1;
    }

    ssize_t index = find_keysym_index(ks);
    إذا (index != -1)
        أرجع snprintf(buffer, size, "%s", get_name(&keysym_to_name[index]));

    /*
     * Unnamed Unicode codepoint.
     * • Keysyms in the range [XKB_KEYSYM_UNICODE_OFFSET, XKB_KEYSYM_UNICODE_MIN [
     *   do not use the Unicode notation for backward compatibility.
     * • Keysyms are not normalized: e.g. given a Unicode keysym, calling
     *   `xkb_utf32_to_keysym` with the corresponding code point may return a
     *   different keysym or fail (e.g. surrogates are invalid in UTF-32).
     */
    إذا (ks >= XKB_KEYSYM_UNICODE_MIN && ks <= XKB_KEYSYM_UNICODE_MAX)
        أرجع get_unicode_name(ks, buffer, size);

    /* Unnamed, non-Unicode, symbol (shouldn't generally happen). */
    أرجع snprintf(buffer, size, "0x%08"PRIx32, ks);
}

bool
xkb_keysym_is_assigned(xkb_keysym_t ks)
{
    أرجع (XKB_KEYSYM_UNICODE_MIN <= ks && ks <= XKB_KEYSYM_UNICODE_MAX) ||
           find_keysym_index(ks) != -1;
}

صحيح
xkb_keysym_get_explicit_names(xkb_keysym_t ks, ثابت حرف **buffer, size_t size)
{
    إذا (ks > XKB_KEYSYM_MAX)
        أرجع -1;
    ثابت ssize_t index = find_keysym_index(ks);
    إذا (index < 0)
        أرجع 0;
    ثابت uint16_t canonical = keysym_to_name[index].offset;
    إذا (size > 0)
        buffer[0] = get_name(&keysym_to_name[index]);
    صحيح کيراءن = 1;
    لكل (size_t pos = 0; pos < ARRAY_SIZE(name_to_keysym); pos++) {
        إذا (name_to_keysym[pos].keysym == ks &&
            name_to_keysym[pos].offset != canonical) {
            إذا ((size_t) کيراءن < size) {
                buffer[کيراءن] = get_name(&name_to_keysym[pos]);
            }
            کيراءن++;
        }
    }
    أرجع کيراءن;
}

بنية xkb_keysym_iterator {
    bool explicit;       /* If true, traverse only explicitly named keysyms */
    int32_t index;       /* Current index in `keysym_to_name` */
    xkb_keysym_t keysym; /* Current keysym */
};

بنية xkb_keysym_iterator*
xkb_keysym_iterator_new(bool iterate_only_explicit_keysyms)
{
    بنية xkb_keysym_iterator* iter = calloc(1, حجم(*iter));
    إذا (!iter)
        أرجع عدم;
    iter->explicit = iterate_only_explicit_keysyms;
    iter->index = -1;
    iter->keysym = XKB_KEYSYM_UNICODE_MAX;
    أرجع iter;
}

بنية xkb_keysym_iterator*
xkb_keysym_iterator_unref(بنية xkb_keysym_iterator *iter)
{
    /*
     * [NOTE] If we ever make this API public, add:
     * - an `refcnt` member,
     * - `xkb_keysym_iterator_ref()`,
     * - `assert(!iter || iter->refcnt > 0)`.
     */
    إذا (iter)
        حرر(iter);
    أرجع عدم;
}

xkb_keysym_t
xkb_keysym_iterator_get_keysym(بنية xkb_keysym_iterator *iter)
{
    أرجع iter->keysym;
}

bool
xkb_keysym_iterator_is_explicitly_named(بنية xkb_keysym_iterator *iter)
{
    أرجع iter->index >= 0 &&
           iter->index < (int32_t)ARRAY_SIZE(keysym_to_name) &&
           (iter->explicit ||
            iter->keysym == keysym_to_name[iter->index].keysym);
}

صحيح
xkb_keysym_iterator_get_name(بنية xkb_keysym_iterator *iter,
                             حرف *buffer, size_t size)
{
    إذا (iter->index < 0 || iter->index >= (int32_t)ARRAY_SIZE(keysym_to_name))
        أرجع -1;
    إذا (iter->explicit || iter->keysym == keysym_to_name[iter->index].keysym)
        أرجع snprintf(buffer, size, "%s",
                        get_name(&keysym_to_name[iter->index]));
    أرجع get_unicode_name(iter->keysym, buffer, size);
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
xkb_keysym_iterator_next(بنية xkb_keysym_iterator *iter)
{
    إذا (iter->index >= (int32_t)ARRAY_SIZE(keysym_to_name) - 1)
        أرجع false;

    /* Next keysym */
    إذا (iter->explicit || iter->keysym >= XKB_KEYSYM_UNICODE_MAX ||
        keysym_to_name[iter->index + 1].keysym < XKB_KEYSYM_UNICODE_MIN) {
        /* Explicitly named keysyms only */
        iter->keysym = keysym_to_name[++iter->index].keysym;
        أكد(iter->explicit ||
               iter->keysym <= XKB_KEYSYM_UNICODE_MIN ||
               iter->keysym >= XKB_KEYSYM_UNICODE_MAX);
    } وإلا {
        /* Unicode keysyms
         * NOTE: Unicode keysyms are within keysym_to_name keysyms range. */
        إذا (iter->keysym >= keysym_to_name[iter->index].keysym)
            iter->index++;
        إذا (iter->keysym >= XKB_KEYSYM_UNICODE_MIN) {
            /* Continue Unicode keysyms */
            iter->keysym++;
        } وإلا {
            /* Start Unicode keysyms */
            iter->keysym = XKB_KEYSYM_UNICODE_MIN;
        }
    }
    أرجع true;
}

/* Parse the numeric part of a 0xXXXX and UXXXX keysym. */
ساكن bool
parse_keysym_hex(ثابت حرف *s, uint32_t *out)
{
    /* We expect a NULL-terminated string of length up to 8 */
    ثابت صحيح کيراءن = parse_hex_to_uint32_t(s, 8, out);
    /* Check that some value was parsed and we reached the end of the string */
    أرجع (کيراءن > 0 && s[کيراءن] == '\0');
}

xkb_keysym_t
xkb_keysym_from_name(ثابت حرف *name, تعداد xkb_keysym_flags flags)
{
    إذا (flags & ~(تعداد xkb_keysym_flags)XKB_KEYSYM_FLAGS_نيلاي_سموا)
        أرجع XKB_کونچي_No_سيمبول;

    ثابت بنية name_keysym *entry = عدم;
    حرف *tmp;
    uint32_t val;
    bool icase = (flags & XKB_KEYSYM_CASE_INSENSITIVE);

    /*
     * We need to !icase case to be fast, for e.g. Compose file parsing.
     * So do it in a fast path.
     */
    إذا (!icase) {
        size_t pos = keysym_name_perfect_hash(name);
        إذا (pos < ARRAY_SIZE(name_to_keysym)) {
            ثابت حرف *s = get_name(&name_to_keysym[pos]);
            إذا (strcmp(name, s) == 0)
                أرجع name_to_keysym[pos].keysym;
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
    وإلا {
        int32_t lo = 0, hi = ARRAY_SIZE(name_to_keysym) - 1;
        طالما (hi >= lo) {
            int32_t mid = (lo + hi) / 2;
            صحيح cmp = istrcmp(name, get_name(&name_to_keysym[mid]));
            إذا (cmp > 0) {
                lo = mid + 1;
            } وإلا إذا (cmp < 0) {
                hi = mid - 1;
            } وإلا {
                entry = &name_to_keysym[mid];
                اكسر;
            }
        }
        إذا (entry) {
            ثابت بنية name_keysym *last;
            last = name_to_keysym + ARRAY_SIZE(name_to_keysym) - 1;
            /* Keep going until we reach end of array
             * or non case-insensitve match */
            طالما (entry < last &&
                   istrcmp(get_name(entry + 1), get_name(entry)) == 0) {
                entry++;
            }
            أرجع entry->keysym;
        }
    }

    إذا (*name == 'U' || (icase && *name == 'u')) {
        /* Parse Unicode notation */
        إذا (!parse_keysym_hex(&name[1], &val))
            أرجع XKB_کونچي_No_سيمبول;
        أرجع (val > 0xff && val <= 0x10ffff)
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
    وإلا إذا (name[0] == '0' && (name[1] == 'x' || (icase && name[1] == 'X'))) {
        /*
         * Parse numeric hexadecimal notation without any normalization, in
         * in order to be consistent with the keymap files parsing.
         */
        إذا (!parse_keysym_hex(&name[2], &val) || val > XKB_KEYSYM_MAX)
            أرجع XKB_کونچي_No_سيمبول;
        أرجع (xkb_keysym_t) val;
    }

    /* Stupid inconsistency between the headers and XKeysymDB: the former has
     * no separating underscore, while some XF86* syms in the latter did.
     * As a last ditch effort, try without. */
    إذا (strncmp(name, "XF86_", 5) == 0 ||
        (icase && istrncmp(name, "XF86_", 5) == 0)) {
        xkb_keysym_t ret;
        tmp = strdup(name);
        إذا (!tmp)
            أرجع XKB_کونچي_No_سيمبول;
        memmove(&tmp[4], &tmp[5], strlen(name) - 5 + 1);
        ret = xkb_keysym_from_name(tmp, flags);
        حرر(tmp);
        أرجع ret;
    }

    أرجع XKB_کونچي_No_سيمبول;
}

xkb_keysym_t
xkb_utf8_to_keysym(ثابت حرف *buffer, size_t size)
{
    إذا (!buffer || !size)
        أرجع 0;

    /* Try to parse the parameter as a single UTF-8 encoded character */
    size_t length = 0;
    ثابت uint32_t codepoint = utf8_next_code_point(buffer, size, &length);
    أرجع (codepoint == INVALID_UTF8_CODE_POINT || length == 0)
        ? XKB_کونچي_No_سيمبول
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
                         ثابت حرف *name,
                         ثابت حرف **reference_name)
{
    إذا (keysym > XKB_KEYSYM_MAX) {
        /* Invalid keysym */
        *reference_name = عدم;
        أرجع false;
    }
    /* [WARNING] We do not check that name, if defined, is a valid for keysym */

    int32_t lo = 0, hi = ARRAY_SIZE(deprecated_keysyms) - 1;
    طالما (hi >= lo) {
        int32_t mid = (lo + hi) / 2;
        إذا (keysym > deprecated_keysyms[mid].keysym) {
            lo = mid + 1;
        } وإلا إذا (keysym < deprecated_keysyms[mid].keysym) {
            hi = mid - 1;
        } وإلا {
            /* Keysym have some deprecated names */
            إذا (deprecated_keysyms[mid].offset == DEPRECATED_KEYSYM) {
                /* All names are deprecated */
                *reference_name = عدم;
                أرجع true;
            } وإلا إذا (deprecated_keysyms[mid].offset == UNICODE_KEYSYM) {
                /* All names are deprecated, but Unicode notation is valid */
                *reference_name = عدم;
                إذا (name == عدم)
                    أرجع false;
                إذا (name[0] != 'U')
                    أرجع true;
                /* Since the current function is internal, assume the name
                 * corresponds to the keysym value and just check its syntax. */
                ثابت حرف *start = name + 1;
                طالما (is_xdigit(*start))
                    start++;
                أرجع *start != '\0' && start > name + 1;
            }
            /* There is a reference name that is not deprecated */
            *reference_name = get_name_by_index(deprecated_keysyms[mid].offset);
            إذا (!name) {
                /* No name to check: indicate not deprecated */
                أرجع false;
            }
            إذا (deprecated_keysyms[mid].explicit_کيراءن) {
                /* Only some explicit names are deprecated */
                ثابت uint8_t k_max = deprecated_keysyms[mid].explicit_index
                                    + deprecated_keysyms[mid].explicit_کيراءن;
                أكد(k_max <= ARRAY_SIZE(explicit_deprecated_aliases));
                /* Check every deprecated alias */
                لكل (uint8_t k = deprecated_keysyms[mid].explicit_index;
                     k < k_max; k++) {
                    #ifdef __clang__
                    /* Make clang-tidy happy */
                    __builtin_assume(k_max <=
                                     ARRAY_SIZE(explicit_deprecated_aliases));
                    #endif
                    ثابت حرف *alias =
                        get_name_by_index(explicit_deprecated_aliases[k]);
                    إذا (strcmp(name, alias) == 0)
                        أرجع true;
                }
                أرجع false;
            } وإلا {
                /* All names but the reference one are deprecated */
                أرجع strcmp(name, *reference_name) != 0;
            }
        }
    }

    /* Keysym has no deprecated names */
    *reference_name = عدم;
    أرجع false;
}

bool
xkb_keysym_is_keypad(xkb_keysym_t keysym)
{
    أرجع keysym >= XKB_کونچي_KP_Space && keysym <= XKB_کونچي_KP_Equal;
}


bool
xkb_keysym_is_modifier(xkb_keysym_t keysym)
{
    أرجع
        (keysym >= XKB_کونچي_Shift_L && keysym <= XKB_کونچي_Hyper_R) ||
        (keysym >= XKB_کونچي_ISO_Lock && keysym <= XKB_کونچي_ISO_Level5_Lock) ||
        keysym == XKB_کونچي_Mode_switch ||
        keysym == XKB_کونچي_اڠک_Lock;
}
