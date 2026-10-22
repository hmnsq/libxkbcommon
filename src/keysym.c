/*
 * For MIT-open-group:
 * Copyright 1985, 1987, 1990, 1998  The Open Group
 *
 * For MIT:
 * Copyright © 2009 Dan Nicholson
 *
 * SPDX-License-Identifier: MIT-open-group AND MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <stdlib.h>

#अन्तर्भाव "xkbcommon/xkbcommon-keysyms.h"
#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "features/enums.h"
#अन्तर्भाव "keysym.h"
#अन्तर्भाव "keysym-names.h"
#अन्तर्भाव "utf8-decoding.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "util-numbers.h"

स्थिर ssize_t
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
    यदि (ks > XKB_KEYSYM_MAX_EXPLICIT)
        फर्काऊ -1;

    ssize_t lo = 0, hi = ARRAY_SIZE(keysym_to_name) - 1;
    जबसम्म (hi >= lo) {
        ssize_t mid = (lo + hi) / 2;
        यदि (ks > keysym_to_name[mid].keysym) {
            lo = mid + 1;
        } अन्यथा यदि (ks < keysym_to_name[mid].keysym) {
            hi = mid - 1;
        } अन्यथा {
            फर्काऊ mid;
        }
    }

    फर्काऊ -1;
}

#define get_name_by_index(index) (keysym_names + (index))

स्थिर अन्तःस्थ अचर वर्ण *
get_name(अचर संरचना name_keysym *entry)
{
    फर्काऊ get_name_by_index(entry->offset);
}

/* Unnamed Unicode codepoint. */
स्थिर अन्तःस्थ पूर्णाङ्क
get_unicode_name(xkb_keysym_t ks, वर्ण *buffer, माप_प्रकार size)
{
    फर्काऊ snprintf(buffer, size, "U%04"PRIX32, ks & UINT32_C(0xffffff));
}

पूर्णाङ्क
xkb_keysym_get_name(xkb_keysym_t ks, वर्ण *buffer, माप_प्रकार size)
{
    यदि (ks > XKB_KEYSYM_MAX) {
        snprintf(buffer, size, "Invalid");
        फर्काऊ -1;
    }

    ssize_t index = find_keysym_index(ks);
    यदि (index != -1)
        फर्काऊ snprintf(buffer, size, "%s", get_name(&keysym_to_name[index]));

    /*
     * Unnamed Unicode codepoint.
     * • Keysyms in the range [XKB_KEYSYM_UNICODE_OFFSET, XKB_KEYSYM_UNICODE_MIN [
     *   do not use the Unicode notation for backward compatibility.
     * • Keysyms are not normalized: e.g. given a Unicode keysym, calling
     *   `xkb_utf32_to_keysym` with the corresponding code point may return a
     *   different keysym or fail (e.g. surrogates are invalid in UTF-32).
     */
    यदि (ks >= XKB_KEYSYM_UNICODE_MIN && ks <= XKB_KEYSYM_UNICODE_MAX)
        फर्काऊ get_unicode_name(ks, buffer, size);

    /* Unnamed, non-Unicode, symbol (shouldn't generally happen). */
    फर्काऊ snprintf(buffer, size, "0x%08"PRIx32, ks);
}

bool
xkb_keysym_is_assigned(xkb_keysym_t ks)
{
    फर्काऊ (XKB_KEYSYM_UNICODE_MIN <= ks && ks <= XKB_KEYSYM_UNICODE_MAX) ||
           find_keysym_index(ks) != -1;
}

पूर्णाङ्क
xkb_keysym_get_explicit_names(xkb_keysym_t ks, अचर वर्ण **buffer, माप_प्रकार size)
{
    यदि (ks > XKB_KEYSYM_MAX)
        फर्काऊ -1;
    अचर ssize_t index = find_keysym_index(ks);
    यदि (index < 0)
        फर्काऊ 0;
    अचर uint16_t canonical = keysym_to_name[index].offset;
    यदि (size > 0)
        buffer[0] = get_name(&keysym_to_name[index]);
    पूर्णाङ्क गन्ती = 1;
    लागि (माप_प्रकार pos = 0; pos < ARRAY_SIZE(name_to_keysym); pos++) {
        यदि (name_to_keysym[pos].keysym == ks &&
            name_to_keysym[pos].offset != canonical) {
            यदि ((माप_प्रकार) गन्ती < size) {
                buffer[गन्ती] = get_name(&name_to_keysym[pos]);
            }
            गन्ती++;
        }
    }
    फर्काऊ गन्ती;
}

संरचना xkb_keysym_iterator {
    bool explicit;       /* If true, traverse only explicitly named keysyms */
    int32_t index;       /* Current index in `keysym_to_name` */
    xkb_keysym_t keysym; /* Current keysym */
};

संरचना xkb_keysym_iterator*
xkb_keysym_iterator_new(bool iterate_only_explicit_keysyms)
{
    संरचना xkb_keysym_iterator* iter = सुस्मृति(1, परिमाण(*iter));
    यदि (!iter)
        फर्काऊ रिक्त;
    iter->explicit = iterate_only_explicit_keysyms;
    iter->index = -1;
    iter->keysym = XKB_KEYSYM_UNICODE_MAX;
    फर्काऊ iter;
}

संरचना xkb_keysym_iterator*
xkb_keysym_iterator_unref(संरचना xkb_keysym_iterator *iter)
{
    /*
     * [NOTE] If we ever make this API public, add:
     * - an `refcnt` member,
     * - `xkb_keysym_iterator_ref()`,
     * - `assert(!iter || iter->refcnt > 0)`.
     */
    यदि (iter)
        मुक्त(iter);
    फर्काऊ रिक्त;
}

xkb_keysym_t
xkb_keysym_iterator_get_keysym(संरचना xkb_keysym_iterator *iter)
{
    फर्काऊ iter->keysym;
}

bool
xkb_keysym_iterator_is_explicitly_named(संरचना xkb_keysym_iterator *iter)
{
    फर्काऊ iter->index >= 0 &&
           iter->index < (int32_t)ARRAY_SIZE(keysym_to_name) &&
           (iter->explicit ||
            iter->keysym == keysym_to_name[iter->index].keysym);
}

पूर्णाङ्क
xkb_keysym_iterator_get_name(संरचना xkb_keysym_iterator *iter,
                             वर्ण *buffer, माप_प्रकार size)
{
    यदि (iter->index < 0 || iter->index >= (int32_t)ARRAY_SIZE(keysym_to_name))
        फर्काऊ -1;
    यदि (iter->explicit || iter->keysym == keysym_to_name[iter->index].keysym)
        फर्काऊ snprintf(buffer, size, "%s",
                        get_name(&keysym_to_name[iter->index]));
    फर्काऊ get_unicode_name(iter->keysym, buffer, size);
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
xkb_keysym_iterator_next(संरचना xkb_keysym_iterator *iter)
{
    यदि (iter->index >= (int32_t)ARRAY_SIZE(keysym_to_name) - 1)
        फर्काऊ false;

    /* Next keysym */
    यदि (iter->explicit || iter->keysym >= XKB_KEYSYM_UNICODE_MAX ||
        keysym_to_name[iter->index + 1].keysym < XKB_KEYSYM_UNICODE_MIN) {
        /* Explicitly named keysyms only */
        iter->keysym = keysym_to_name[++iter->index].keysym;
        प्रतिज्ञा(iter->explicit ||
               iter->keysym <= XKB_KEYSYM_UNICODE_MIN ||
               iter->keysym >= XKB_KEYSYM_UNICODE_MAX);
    } अन्यथा {
        /* Unicode keysyms
         * NOTE: Unicode keysyms are within keysym_to_name keysyms range. */
        यदि (iter->keysym >= keysym_to_name[iter->index].keysym)
            iter->index++;
        यदि (iter->keysym >= XKB_KEYSYM_UNICODE_MIN) {
            /* Continue Unicode keysyms */
            iter->keysym++;
        } अन्यथा {
            /* Start Unicode keysyms */
            iter->keysym = XKB_KEYSYM_UNICODE_MIN;
        }
    }
    फर्काऊ true;
}

/* Parse the numeric part of a 0xXXXX and UXXXX keysym. */
स्थिर bool
parse_keysym_hex(अचर वर्ण *s, uint32_t *out)
{
    /* We expect a NULL-terminated string of length up to 8 */
    अचर पूर्णाङ्क गन्ती = parse_hex_to_uint32_t(s, 8, out);
    /* Check that some value was parsed and we reached the end of the string */
    फर्काऊ (गन्ती > 0 && s[गन्ती] == '\0');
}

xkb_keysym_t
xkb_keysym_from_name(अचर वर्ण *name, गणना xkb_keysym_flags flags)
{
    यदि (flags & ~(गणना xkb_keysym_flags)XKB_KEYSYM_FLAGS_मानहरू)
        फर्काऊ XKB_साँचो_No_चिह्न;

    अचर संरचना name_keysym *entry = रिक्त;
    वर्ण *tmp;
    uint32_t val;
    bool icase = (flags & XKB_KEYSYM_CASE_INSENSITIVE);

    /*
     * We need to !icase case to be fast, for e.g. Compose file parsing.
     * So do it in a fast path.
     */
    यदि (!icase) {
        माप_प्रकार pos = keysym_name_perfect_hash(name);
        यदि (pos < ARRAY_SIZE(name_to_keysym)) {
            अचर वर्ण *s = get_name(&name_to_keysym[pos]);
            यदि (म_भेद(name, s) == 0)
                फर्काऊ name_to_keysym[pos].keysym;
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
    अन्यथा {
        int32_t lo = 0, hi = ARRAY_SIZE(name_to_keysym) - 1;
        जबसम्म (hi >= lo) {
            int32_t mid = (lo + hi) / 2;
            पूर्णाङ्क cmp = istrcmp(name, get_name(&name_to_keysym[mid]));
            यदि (cmp > 0) {
                lo = mid + 1;
            } अन्यथा यदि (cmp < 0) {
                hi = mid - 1;
            } अन्यथा {
                entry = &name_to_keysym[mid];
                रोक;
            }
        }
        यदि (entry) {
            अचर संरचना name_keysym *last;
            last = name_to_keysym + ARRAY_SIZE(name_to_keysym) - 1;
            /* Keep going until we reach end of array
             * or non case-insensitve match */
            जबसम्म (entry < last &&
                   istrcmp(get_name(entry + 1), get_name(entry)) == 0) {
                entry++;
            }
            फर्काऊ entry->keysym;
        }
    }

    यदि (*name == 'U' || (icase && *name == 'u')) {
        /* Parse Unicode notation */
        यदि (!parse_keysym_hex(&name[1], &val))
            फर्काऊ XKB_साँचो_No_चिह्न;
        फर्काऊ (val > 0xff && val <= 0x10ffff)
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
    अन्यथा यदि (name[0] == '0' && (name[1] == 'x' || (icase && name[1] == 'X'))) {
        /*
         * Parse numeric hexadecimal notation without any normalization, in
         * in order to be consistent with the keymap files parsing.
         */
        यदि (!parse_keysym_hex(&name[2], &val) || val > XKB_KEYSYM_MAX)
            फर्काऊ XKB_साँचो_No_चिह्न;
        फर्काऊ (xkb_keysym_t) val;
    }

    /* Stupid inconsistency between the headers and XKeysymDB: the former has
     * no separating underscore, while some XF86* syms in the latter did.
     * As a last ditch effort, try without. */
    यदि (म_भेदन(name, "XF86_", 5) == 0 ||
        (icase && istrncmp(name, "XF86_", 5) == 0)) {
        xkb_keysym_t ret;
        tmp = strdup(name);
        यदि (!tmp)
            फर्काऊ XKB_साँचो_No_चिह्न;
        स_हटाओ(&tmp[4], &tmp[5], म_माप(name) - 5 + 1);
        ret = xkb_keysym_from_name(tmp, flags);
        मुक्त(tmp);
        फर्काऊ ret;
    }

    फर्काऊ XKB_साँचो_No_चिह्न;
}

xkb_keysym_t
xkb_utf8_to_keysym(अचर वर्ण *buffer, माप_प्रकार size)
{
    यदि (!buffer || !size)
        फर्काऊ 0;

    /* Try to parse the parameter as a single UTF-8 encoded character */
    माप_प्रकार length = 0;
    अचर uint32_t codepoint = utf8_next_code_point(buffer, size, &length);
    फर्काऊ (codepoint == INVALID_UTF8_CODE_POINT || length == 0)
        ? XKB_साँचो_No_चिह्न
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
                         अचर वर्ण *name,
                         अचर वर्ण **reference_name)
{
    यदि (keysym > XKB_KEYSYM_MAX) {
        /* Invalid keysym */
        *reference_name = रिक्त;
        फर्काऊ false;
    }
    /* [WARNING] We do not check that name, if defined, is a valid for keysym */

    int32_t lo = 0, hi = ARRAY_SIZE(deprecated_keysyms) - 1;
    जबसम्म (hi >= lo) {
        int32_t mid = (lo + hi) / 2;
        यदि (keysym > deprecated_keysyms[mid].keysym) {
            lo = mid + 1;
        } अन्यथा यदि (keysym < deprecated_keysyms[mid].keysym) {
            hi = mid - 1;
        } अन्यथा {
            /* Keysym have some deprecated names */
            यदि (deprecated_keysyms[mid].offset == DEPRECATED_KEYSYM) {
                /* All names are deprecated */
                *reference_name = रिक्त;
                फर्काऊ true;
            } अन्यथा यदि (deprecated_keysyms[mid].offset == UNICODE_KEYSYM) {
                /* All names are deprecated, but Unicode notation is valid */
                *reference_name = रिक्त;
                यदि (name == रिक्त)
                    फर्काऊ false;
                यदि (name[0] != 'U')
                    फर्काऊ true;
                /* Since the current function is internal, assume the name
                 * corresponds to the keysym value and just check its syntax. */
                अचर वर्ण *start = name + 1;
                जबसम्म (is_xdigit(*start))
                    start++;
                फर्काऊ *start != '\0' && start > name + 1;
            }
            /* There is a reference name that is not deprecated */
            *reference_name = get_name_by_index(deprecated_keysyms[mid].offset);
            यदि (!name) {
                /* No name to check: indicate not deprecated */
                फर्काऊ false;
            }
            यदि (deprecated_keysyms[mid].explicit_गन्ती) {
                /* Only some explicit names are deprecated */
                अचर uint8_t k_max = deprecated_keysyms[mid].explicit_index
                                    + deprecated_keysyms[mid].explicit_गन्ती;
                प्रतिज्ञा(k_max <= ARRAY_SIZE(explicit_deprecated_aliases));
                /* Check every deprecated alias */
                लागि (uint8_t k = deprecated_keysyms[mid].explicit_index;
                     k < k_max; k++) {
                    #ifdef __clang__
                    /* Make clang-tidy happy */
                    __builtin_assume(k_max <=
                                     ARRAY_SIZE(explicit_deprecated_aliases));
                    #endif
                    अचर वर्ण *alias =
                        get_name_by_index(explicit_deprecated_aliases[k]);
                    यदि (म_भेद(name, alias) == 0)
                        फर्काऊ true;
                }
                फर्काऊ false;
            } अन्यथा {
                /* All names but the reference one are deprecated */
                फर्काऊ म_भेद(name, *reference_name) != 0;
            }
        }
    }

    /* Keysym has no deprecated names */
    *reference_name = रिक्त;
    फर्काऊ false;
}

bool
xkb_keysym_is_keypad(xkb_keysym_t keysym)
{
    फर्काऊ keysym >= XKB_साँचो_KP_Space && keysym <= XKB_साँचो_KP_Equal;
}


bool
xkb_keysym_is_modifier(xkb_keysym_t keysym)
{
    फर्काऊ
        (keysym >= XKB_साँचो_Shift_L && keysym <= XKB_साँचो_Hyper_R) ||
        (keysym >= XKB_साँचो_ISO_Lock && keysym <= XKB_साँचो_ISO_Level5_Lock) ||
        keysym == XKB_साँचो_Mode_switch ||
        keysym == XKB_साँचो_अङ्क_Lock;
}
