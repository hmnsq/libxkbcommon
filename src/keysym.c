/*
 * For MIT-open-group:
 * Copyright 1985, 1987, 1990, 1998  The Open Group
 *
 * For MIT:
 * Copyright © 2009 Dan Nicholson
 *
 * SPDX-License-Identifier: MIT-open-group AND MIT
 */

#شامل "config.h"

#شامل <assert.h>
#شامل <stdint.h>
#شامل <stdlib.h>

#شامل "xkbcommon/xkbcommon-keysyms.h"
#شامل "xkbcommon/xkbcommon.h"
#شامل "features/enums.h"
#شامل "keysym.h"
#شامل "keysym-names.h"
#شامل "utf8-decoding.h"
#شامل "utils.h"
#شامل "util-numbers.h"

ایستا ssize_t
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
    اگر (ks > XKB_KEYSYM_MAX_EXPLICIT)
        بازگشت -1;

    ssize_t lo = 0, hi = ARRAY_SIZE(keysym_to_name) - 1;
    وقتی (hi >= lo) {
        ssize_t mid = (lo + hi) / 2;
        اگر (ks > keysym_to_name[mid].keysym) {
            lo = mid + 1;
        } وگرنه اگر (ks < keysym_to_name[mid].keysym) {
            hi = mid - 1;
        } وگرنه {
            بازگشت mid;
        }
    }

    بازگشت -1;
}

#define get_name_by_index(index) (keysym_names + (index))

ایستا درونخطی ثابت نویسه *
get_name(ثابت ساختار name_keysym *entry)
{
    بازگشت get_name_by_index(entry->offset);
}

/* Unnamed Unicode codepoint. */
ایستا درونخطی صحیح
get_unicode_name(xkb_keysym_t ks, نویسه *buffer, size_t size)
{
    بازگشت snprintf(buffer, size, "U%04"PRIX32, ks & UINT32_C(0xffffff));
}

صحیح
xkb_keysym_get_name(xkb_keysym_t ks, نویسه *buffer, size_t size)
{
    اگر (ks > XKB_KEYSYM_MAX) {
        snprintf(buffer, size, "Invalid");
        بازگشت -1;
    }

    ssize_t index = find_keysym_index(ks);
    اگر (index != -1)
        بازگشت snprintf(buffer, size, "%s", get_name(&keysym_to_name[index]));

    /*
     * Unnamed Unicode codepoint.
     * • Keysyms in the range [XKB_KEYSYM_UNICODE_OFFSET, XKB_KEYSYM_UNICODE_MIN [
     *   do not use the Unicode notation for backward compatibility.
     * • Keysyms are not normalized: e.g. given a Unicode keysym, calling
     *   `xkb_utf32_to_keysym` with the corresponding code point may return a
     *   different keysym or fail (e.g. surrogates are invalid in UTF-32).
     */
    اگر (ks >= XKB_KEYSYM_UNICODE_MIN && ks <= XKB_KEYSYM_UNICODE_MAX)
        بازگشت get_unicode_name(ks, buffer, size);

    /* Unnamed, non-Unicode, symbol (shouldn't generally happen). */
    بازگشت snprintf(buffer, size, "0x%08"PRIx32, ks);
}

bool
xkb_keysym_is_assigned(xkb_keysym_t ks)
{
    بازگشت (XKB_KEYSYM_UNICODE_MIN <= ks && ks <= XKB_KEYSYM_UNICODE_MAX) ||
           find_keysym_index(ks) != -1;
}

صحیح
xkb_keysym_get_explicit_names(xkb_keysym_t ks, ثابت نویسه **buffer, size_t size)
{
    اگر (ks > XKB_KEYSYM_MAX)
        بازگشت -1;
    ثابت ssize_t index = find_keysym_index(ks);
    اگر (index < 0)
        بازگشت 0;
    ثابت uint16_t canonical = keysym_to_name[index].offset;
    اگر (size > 0)
        buffer[0] = get_name(&keysym_to_name[index]);
    صحیح شمار = 1;
    برای (size_t pos = 0; pos < ARRAY_SIZE(name_to_keysym); pos++) {
        اگر (name_to_keysym[pos].keysym == ks &&
            name_to_keysym[pos].offset != canonical) {
            اگر ((size_t) شمار < size) {
                buffer[شمار] = get_name(&name_to_keysym[pos]);
            }
            شمار++;
        }
    }
    بازگشت شمار;
}

ساختار xkb_keysym_iterator {
    bool explicit;       /* If true, traverse only explicitly named keysyms */
    int32_t index;       /* Current index in `keysym_to_name` */
    xkb_keysym_t keysym; /* Current keysym */
};

ساختار xkb_keysym_iterator*
xkb_keysym_iterator_new(bool iterate_only_explicit_keysyms)
{
    ساختار xkb_keysym_iterator* iter = calloc(1, اندازه(*iter));
    اگر (!iter)
        بازگشت پوچ;
    iter->explicit = iterate_only_explicit_keysyms;
    iter->index = -1;
    iter->keysym = XKB_KEYSYM_UNICODE_MAX;
    بازگشت iter;
}

ساختار xkb_keysym_iterator*
xkb_keysym_iterator_unref(ساختار xkb_keysym_iterator *iter)
{
    /*
     * [NOTE] If we ever make this API public, add:
     * - an `refcnt` member,
     * - `xkb_keysym_iterator_ref()`,
     * - `assert(!iter || iter->refcnt > 0)`.
     */
    اگر (iter)
        آزاد(iter);
    بازگشت پوچ;
}

xkb_keysym_t
xkb_keysym_iterator_get_keysym(ساختار xkb_keysym_iterator *iter)
{
    بازگشت iter->keysym;
}

bool
xkb_keysym_iterator_is_explicitly_named(ساختار xkb_keysym_iterator *iter)
{
    بازگشت iter->index >= 0 &&
           iter->index < (int32_t)ARRAY_SIZE(keysym_to_name) &&
           (iter->explicit ||
            iter->keysym == keysym_to_name[iter->index].keysym);
}

صحیح
xkb_keysym_iterator_get_name(ساختار xkb_keysym_iterator *iter,
                             نویسه *buffer, size_t size)
{
    اگر (iter->index < 0 || iter->index >= (int32_t)ARRAY_SIZE(keysym_to_name))
        بازگشت -1;
    اگر (iter->explicit || iter->keysym == keysym_to_name[iter->index].keysym)
        بازگشت snprintf(buffer, size, "%s",
                        get_name(&keysym_to_name[iter->index]));
    بازگشت get_unicode_name(iter->keysym, buffer, size);
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
xkb_keysym_iterator_next(ساختار xkb_keysym_iterator *iter)
{
    اگر (iter->index >= (int32_t)ARRAY_SIZE(keysym_to_name) - 1)
        بازگشت false;

    /* Next keysym */
    اگر (iter->explicit || iter->keysym >= XKB_KEYSYM_UNICODE_MAX ||
        keysym_to_name[iter->index + 1].keysym < XKB_KEYSYM_UNICODE_MIN) {
        /* Explicitly named keysyms only */
        iter->keysym = keysym_to_name[++iter->index].keysym;
        أكد(iter->explicit ||
               iter->keysym <= XKB_KEYSYM_UNICODE_MIN ||
               iter->keysym >= XKB_KEYSYM_UNICODE_MAX);
    } وگرنه {
        /* Unicode keysyms
         * NOTE: Unicode keysyms are within keysym_to_name keysyms range. */
        اگر (iter->keysym >= keysym_to_name[iter->index].keysym)
            iter->index++;
        اگر (iter->keysym >= XKB_KEYSYM_UNICODE_MIN) {
            /* Continue Unicode keysyms */
            iter->keysym++;
        } وگرنه {
            /* Start Unicode keysyms */
            iter->keysym = XKB_KEYSYM_UNICODE_MIN;
        }
    }
    بازگشت true;
}

/* Parse the numeric part of a 0xXXXX and UXXXX keysym. */
ایستا bool
parse_keysym_hex(ثابت نویسه *s, uint32_t *out)
{
    /* We expect a NULL-terminated string of length up to 8 */
    ثابت صحیح شمار = parse_hex_to_uint32_t(s, 8, out);
    /* Check that some value was parsed and we reached the end of the string */
    بازگشت (شمار > 0 && s[شمار] == '\0');
}

xkb_keysym_t
xkb_keysym_from_name(ثابت نویسه *name, شمارش xkb_keysym_flags flags)
{
    اگر (flags & ~(شمارش xkb_keysym_flags)XKB_KEYSYM_FLAGS_مقادیر)
        بازگشت XKB_کلید_No_نماد;

    ثابت ساختار name_keysym *entry = پوچ;
    نویسه *tmp;
    uint32_t val;
    bool icase = (flags & XKB_KEYSYM_CASE_INSENSITIVE);

    /*
     * We need to !icase case to be fast, for e.g. Compose file parsing.
     * So do it in a fast path.
     */
    اگر (!icase) {
        size_t pos = keysym_name_perfect_hash(name);
        اگر (pos < ARRAY_SIZE(name_to_keysym)) {
            ثابت نویسه *s = get_name(&name_to_keysym[pos]);
            اگر (strcmp(name, s) == 0)
                بازگشت name_to_keysym[pos].keysym;
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
    وگرنه {
        int32_t lo = 0, hi = ARRAY_SIZE(name_to_keysym) - 1;
        وقتی (hi >= lo) {
            int32_t mid = (lo + hi) / 2;
            صحیح cmp = istrcmp(name, get_name(&name_to_keysym[mid]));
            اگر (cmp > 0) {
                lo = mid + 1;
            } وگرنه اگر (cmp < 0) {
                hi = mid - 1;
            } وگرنه {
                entry = &name_to_keysym[mid];
                بشکن;
            }
        }
        اگر (entry) {
            ثابت ساختار name_keysym *last;
            last = name_to_keysym + ARRAY_SIZE(name_to_keysym) - 1;
            /* Keep going until we reach end of array
             * or non case-insensitve match */
            وقتی (entry < last &&
                   istrcmp(get_name(entry + 1), get_name(entry)) == 0) {
                entry++;
            }
            بازگشت entry->keysym;
        }
    }

    اگر (*name == 'U' || (icase && *name == 'u')) {
        /* Parse Unicode notation */
        اگر (!parse_keysym_hex(&name[1], &val))
            بازگشت XKB_کلید_No_نماد;
        بازگشت (val > 0xff && val <= 0x10ffff)
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
    وگرنه اگر (name[0] == '0' && (name[1] == 'x' || (icase && name[1] == 'X'))) {
        /*
         * Parse numeric hexadecimal notation without any normalization, in
         * in order to be consistent with the keymap files parsing.
         */
        اگر (!parse_keysym_hex(&name[2], &val) || val > XKB_KEYSYM_MAX)
            بازگشت XKB_کلید_No_نماد;
        بازگشت (xkb_keysym_t) val;
    }

    /* Stupid inconsistency between the headers and XKeysymDB: the former has
     * no separating underscore, while some XF86* syms in the latter did.
     * As a last ditch effort, try without. */
    اگر (strncmp(name, "XF86_", 5) == 0 ||
        (icase && istrncmp(name, "XF86_", 5) == 0)) {
        xkb_keysym_t ret;
        tmp = strdup(name);
        اگر (!tmp)
            بازگشت XKB_کلید_No_نماد;
        memmove(&tmp[4], &tmp[5], strlen(name) - 5 + 1);
        ret = xkb_keysym_from_name(tmp, flags);
        آزاد(tmp);
        بازگشت ret;
    }

    بازگشت XKB_کلید_No_نماد;
}

xkb_keysym_t
xkb_utf8_to_keysym(ثابت نویسه *buffer, size_t size)
{
    اگر (!buffer || !size)
        بازگشت 0;

    /* Try to parse the parameter as a single UTF-8 encoded character */
    size_t length = 0;
    ثابت uint32_t codepoint = utf8_next_code_point(buffer, size, &length);
    بازگشت (codepoint == INVALID_UTF8_CODE_POINT || length == 0)
        ? XKB_کلید_No_نماد
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
                         ثابت نویسه *name,
                         ثابت نویسه **reference_name)
{
    اگر (keysym > XKB_KEYSYM_MAX) {
        /* Invalid keysym */
        *reference_name = پوچ;
        بازگشت false;
    }
    /* [WARNING] We do not check that name, if defined, is a valid for keysym */

    int32_t lo = 0, hi = ARRAY_SIZE(deprecated_keysyms) - 1;
    وقتی (hi >= lo) {
        int32_t mid = (lo + hi) / 2;
        اگر (keysym > deprecated_keysyms[mid].keysym) {
            lo = mid + 1;
        } وگرنه اگر (keysym < deprecated_keysyms[mid].keysym) {
            hi = mid - 1;
        } وگرنه {
            /* Keysym have some deprecated names */
            اگر (deprecated_keysyms[mid].offset == DEPRECATED_KEYSYM) {
                /* All names are deprecated */
                *reference_name = پوچ;
                بازگشت true;
            } وگرنه اگر (deprecated_keysyms[mid].offset == UNICODE_KEYSYM) {
                /* All names are deprecated, but Unicode notation is valid */
                *reference_name = پوچ;
                اگر (name == پوچ)
                    بازگشت false;
                اگر (name[0] != 'U')
                    بازگشت true;
                /* Since the current function is internal, assume the name
                 * corresponds to the keysym value and just check its syntax. */
                ثابت نویسه *start = name + 1;
                وقتی (is_xdigit(*start))
                    start++;
                بازگشت *start != '\0' && start > name + 1;
            }
            /* There is a reference name that is not deprecated */
            *reference_name = get_name_by_index(deprecated_keysyms[mid].offset);
            اگر (!name) {
                /* No name to check: indicate not deprecated */
                بازگشت false;
            }
            اگر (deprecated_keysyms[mid].explicit_شمار) {
                /* Only some explicit names are deprecated */
                ثابت uint8_t k_max = deprecated_keysyms[mid].explicit_index
                                    + deprecated_keysyms[mid].explicit_شمار;
                أكد(k_max <= ARRAY_SIZE(explicit_deprecated_aliases));
                /* Check every deprecated alias */
                برای (uint8_t k = deprecated_keysyms[mid].explicit_index;
                     k < k_max; k++) {
                    #ifdef __clang__
                    /* Make clang-tidy happy */
                    __builtin_assume(k_max <=
                                     ARRAY_SIZE(explicit_deprecated_aliases));
                    #endif
                    ثابت نویسه *alias =
                        get_name_by_index(explicit_deprecated_aliases[k]);
                    اگر (strcmp(name, alias) == 0)
                        بازگشت true;
                }
                بازگشت false;
            } وگرنه {
                /* All names but the reference one are deprecated */
                بازگشت strcmp(name, *reference_name) != 0;
            }
        }
    }

    /* Keysym has no deprecated names */
    *reference_name = پوچ;
    بازگشت false;
}

bool
xkb_keysym_is_keypad(xkb_keysym_t keysym)
{
    بازگشت keysym >= XKB_کلید_KP_Space && keysym <= XKB_کلید_KP_Equal;
}


bool
xkb_keysym_is_modifier(xkb_keysym_t keysym)
{
    بازگشت
        (keysym >= XKB_کلید_Shift_L && keysym <= XKB_کلید_Hyper_R) ||
        (keysym >= XKB_کلید_ISO_Lock && keysym <= XKB_کلید_ISO_Level5_Lock) ||
        keysym == XKB_کلید_Mode_switch ||
        keysym == XKB_کلید_رقم_Lock;
}
