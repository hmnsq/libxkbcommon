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
find_keysym_सूचकांक(xkb_keysym_t ks)
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
        प्रत्यागम -1;

    ssize_t lo = 0, hi = ARRAY_आकार(keysym_गन्तव्यम्_name) - 1;
    यावत् (hi >= lo) {
        ssize_t mid = (lo + hi) / 2;
        यदि (ks > keysym_गन्तव्यम्_name[mid].keysym) {
            lo = mid + 1;
        } अन्यथा यदि (ks < keysym_गन्तव्यम्_name[mid].keysym) {
            hi = mid - 1;
        } अन्यथा {
            प्रत्यागम mid;
        }
    }

    प्रत्यागम -1;
}

#परिभाषय get_name_by_सूचकांक(सूचकांक) (keysym_नामानि + (सूचकांक))

स्थिर अन्तःस्थ ध्रुव वर्ण *
get_name(ध्रुव संरचना name_keysym *प्रविष्टिः)
{
    प्रत्यागम get_name_by_सूचकांक(प्रविष्टिः->विस्थापनम्);
}

/* Unnamed Unicode codepoint. */
स्थिर अन्तःस्थ पूर्णाङ्क
get_unicode_name(xkb_keysym_t ks, वर्ण *बफर, आकार_t आकार)
{
    प्रत्यागम snprintf(बफर, आकार, "U%04"PRIX32, ks & UINT32_C(0xffffff));
}

पूर्णाङ्क
xkb_keysym_get_name(xkb_keysym_t ks, वर्ण *बफर, आकार_t आकार)
{
    यदि (ks > XKB_KEYSYM_MAX) {
        snprintf(बफर, आकार, "Invalid");
        प्रत्यागम -1;
    }

    ssize_t सूचकांक = find_keysym_सूचकांक(ks);
    यदि (सूचकांक != -1)
        प्रत्यागम snprintf(बफर, आकार, "%s", get_name(&keysym_गन्तव्यम्_name[सूचकांक]));

    /*
     * Unnamed Unicode codepoint.
     * • Keysyms in the range [XKB_KEYSYM_UNICODE_OFFSET, XKB_KEYSYM_UNICODE_MIN [
     *   do not use the Unicode notation for backward compatibility.
     * • Keysyms are not normalized: e.g. given a Unicode keysym, calling
     *   `xkb_utf32_to_keysym` with the corresponding code point may return a
     *   different keysym or fail (e.g. surrogates are invalid in UTF-32).
     */
    यदि (ks >= XKB_KEYSYM_UNICODE_MIN && ks <= XKB_KEYSYM_UNICODE_MAX)
        प्रत्यागम get_unicode_name(ks, बफर, आकार);

    /* Unnamed, non-Unicode, symbol (shouldn't generally happen). */
    प्रत्यागम snprintf(बफर, आकार, "0x%08"PRIx32, ks);
}

bool
xkb_keysym_is_assigned(xkb_keysym_t ks)
{
    प्रत्यागम (XKB_KEYSYM_UNICODE_MIN <= ks && ks <= XKB_KEYSYM_UNICODE_MAX) ||
           find_keysym_सूचकांक(ks) != -1;
}

पूर्णाङ्क
xkb_keysym_get_explicit_नामानि(xkb_keysym_t ks, ध्रुव वर्ण **बफर, आकार_t आकार)
{
    यदि (ks > XKB_KEYSYM_MAX)
        प्रत्यागम -1;
    ध्रुव ssize_t सूचकांक = find_keysym_सूचकांक(ks);
    यदि (सूचकांक < 0)
        प्रत्यागम 0;
    ध्रुव uint16_t canonical = keysym_गन्तव्यम्_name[सूचकांक].विस्थापनम्;
    यदि (आकार > 0)
        बफर[0] = get_name(&keysym_गन्तव्यम्_name[सूचकांक]);
    पूर्णाङ्क गणनम् = 1;
    प्रति (आकार_t pos = 0; pos < ARRAY_आकार(name_गन्तव्यम्_keysym); pos++) {
        यदि (name_गन्तव्यम्_keysym[pos].keysym == ks &&
            name_गन्तव्यम्_keysym[pos].विस्थापनम् != canonical) {
            यदि ((आकार_t) गणनम् < आकार) {
                बफर[गणनम्] = get_name(&name_गन्तव्यम्_keysym[pos]);
            }
            गणनम्++;
        }
    }
    प्रत्यागम गणनम्;
}

संरचना xkb_keysym_iterator {
    bool explicit;       /* If true, traverse only explicitly named keysyms */
    int32_t सूचकांक;       /* Current index in `keysym_to_name` */
    xkb_keysym_t keysym; /* Current keysym */
};

संरचना xkb_keysym_iterator*
xkb_keysym_iterator_new(bool iterate_only_explicit_keysyms)
{
    संरचना xkb_keysym_iterator* iter = शून्यस्मृतिं_देहि(1, परिमाण(*iter));
    यदि (!iter)
        प्रत्यागम रिक्त;
    iter->explicit = iterate_only_explicit_keysyms;
    iter->सूचकांक = -1;
    iter->keysym = XKB_KEYSYM_UNICODE_MAX;
    प्रत्यागम iter;
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
    प्रत्यागम रिक्त;
}

xkb_keysym_t
xkb_keysym_iterator_get_keysym(संरचना xkb_keysym_iterator *iter)
{
    प्रत्यागम iter->keysym;
}

bool
xkb_keysym_iterator_is_explicitly_named(संरचना xkb_keysym_iterator *iter)
{
    प्रत्यागम iter->सूचकांक >= 0 &&
           iter->सूचकांक < (int32_t)ARRAY_आकार(keysym_गन्तव्यम्_name) &&
           (iter->explicit ||
            iter->keysym == keysym_गन्तव्यम्_name[iter->सूचकांक].keysym);
}

पूर्णाङ्क
xkb_keysym_iterator_get_name(संरचना xkb_keysym_iterator *iter,
                             वर्ण *बफर, आकार_t आकार)
{
    यदि (iter->सूचकांक < 0 || iter->सूचकांक >= (int32_t)ARRAY_आकार(keysym_गन्तव्यम्_name))
        प्रत्यागम -1;
    यदि (iter->explicit || iter->keysym == keysym_गन्तव्यम्_name[iter->सूचकांक].keysym)
        प्रत्यागम snprintf(बफर, आकार, "%s",
                        get_name(&keysym_गन्तव्यम्_name[iter->सूचकांक]));
    प्रत्यागम get_unicode_name(iter->keysym, बफर, आकार);
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
xkb_keysym_iterator_अग्र(संरचना xkb_keysym_iterator *iter)
{
    यदि (iter->सूचकांक >= (int32_t)ARRAY_आकार(keysym_गन्तव्यम्_name) - 1)
        प्रत्यागम false;

    /* Next keysym */
    यदि (iter->explicit || iter->keysym >= XKB_KEYSYM_UNICODE_MAX ||
        keysym_गन्तव्यम्_name[iter->सूचकांक + 1].keysym < XKB_KEYSYM_UNICODE_MIN) {
        /* Explicitly named keysyms only */
        iter->keysym = keysym_गन्तव्यम्_name[++iter->सूचकांक].keysym;
        प्रतिज्ञा(iter->explicit ||
               iter->keysym <= XKB_KEYSYM_UNICODE_MIN ||
               iter->keysym >= XKB_KEYSYM_UNICODE_MAX);
    } अन्यथा {
        /* Unicode keysyms
         * NOTE: Unicode keysyms are within keysym_to_name keysyms range. */
        यदि (iter->keysym >= keysym_गन्तव्यम्_name[iter->सूचकांक].keysym)
            iter->सूचकांक++;
        यदि (iter->keysym >= XKB_KEYSYM_UNICODE_MIN) {
            /* Continue Unicode keysyms */
            iter->keysym++;
        } अन्यथा {
            /* Start Unicode keysyms */
            iter->keysym = XKB_KEYSYM_UNICODE_MIN;
        }
    }
    प्रत्यागम true;
}

/* Parse the numeric part of a 0xXXXX and UXXXX keysym. */
स्थिर bool
parse_keysym_hex(ध्रुव वर्ण *s, uint32_t *बहिर्गमनम्)
{
    /* We expect a NULL-terminated string of length up to 8 */
    ध्रुव पूर्णाङ्क गणनम् = parse_hex_गन्तव्यम्_uint32_t(s, 8, बहिर्गमनम्);
    /* Check that some value was parsed and we reached the end of the string */
    प्रत्यागम (गणनम् > 0 && s[गणनम्] == '\0');
}

xkb_keysym_t
xkb_keysym_from_name(ध्रुव वर्ण *name, गणना xkb_keysym_flags flags)
{
    यदि (flags & ~(गणना xkb_keysym_flags)XKB_KEYSYM_FLAGS_मानानि)
        प्रत्यागम XKB_कुञ्चिका_No_चिह्नम्;

    ध्रुव संरचना name_keysym *प्रविष्टिः = रिक्त;
    वर्ण *tmp;
    uint32_t val;
    bool icase = (flags & XKB_KEYSYM_CASE_INSENSITIVE);

    /*
     * We need to !icase case to be fast, for e.g. Compose file parsing.
     * So do it in a fast path.
     */
    यदि (!icase) {
        आकार_t pos = keysym_name_perfect_hash(name);
        यदि (pos < ARRAY_आकार(name_गन्तव्यम्_keysym)) {
            ध्रुव वर्ण *s = get_name(&name_गन्तव्यम्_keysym[pos]);
            यदि (वाक्यं_तुलय(name, s) == 0)
                प्रत्यागम name_गन्तव्यम्_keysym[pos].keysym;
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
        int32_t lo = 0, hi = ARRAY_आकार(name_गन्तव्यम्_keysym) - 1;
        यावत् (hi >= lo) {
            int32_t mid = (lo + hi) / 2;
            पूर्णाङ्क cmp = istrcmp(name, get_name(&name_गन्तव्यम्_keysym[mid]));
            यदि (cmp > 0) {
                lo = mid + 1;
            } अन्यथा यदि (cmp < 0) {
                hi = mid - 1;
            } अन्यथा {
                प्रविष्टिः = &name_गन्तव्यम्_keysym[mid];
                भङ्ग;
            }
        }
        यदि (प्रविष्टिः) {
            ध्रुव संरचना name_keysym *चरमः;
            चरमः = name_गन्तव्यम्_keysym + ARRAY_आकार(name_गन्तव्यम्_keysym) - 1;
            /* Keep going until we reach end of array
             * or non case-insensitve match */
            यावत् (प्रविष्टिः < चरमः &&
                   istrcmp(get_name(प्रविष्टिः + 1), get_name(प्रविष्टिः)) == 0) {
                प्रविष्टिः++;
            }
            प्रत्यागम प्रविष्टिः->keysym;
        }
    }

    यदि (*name == 'U' || (icase && *name == 'u')) {
        /* Parse Unicode notation */
        यदि (!parse_keysym_hex(&name[1], &val))
            प्रत्यागम XKB_कुञ्चिका_No_चिह्नम्;
        प्रत्यागम (val > 0xff && val <= 0x10ffff)
            /*
             * No normalization.
             *
             * NOTE: It allows surrogates, as we are dealing with Unicode
             * *code points* here, not Unicode *scalars*.
             */
            ? XKB_KEYSYM_UNICODE_विस्थापनम् + val
            /*
             * Normalize ISO-8859-1 (Latin-1 + C0 and C1 control code)
             *
             * These code points require special processing to ensure
             * backward compatibility with legacy keysyms.
             */
            : xkb_utf32_गन्तव्यम्_keysym(val);
    }
    अन्यथा यदि (name[0] == '0' && (name[1] == 'x' || (icase && name[1] == 'X'))) {
        /*
         * Parse numeric hexadecimal notation without any normalization, in
         * in order to be consistent with the keymap files parsing.
         */
        यदि (!parse_keysym_hex(&name[2], &val) || val > XKB_KEYSYM_MAX)
            प्रत्यागम XKB_कुञ्चिका_No_चिह्नम्;
        प्रत्यागम (xkb_keysym_t) val;
    }

    /* Stupid inconsistency between the headers and XKeysymDB: the former has
     * no separating underscore, while some XF86* syms in the latter did.
     * As a last ditch effort, try without. */
    यदि (वाक्ये_तुलय(name, "XF86_", 5) == 0 ||
        (icase && istrncmp(name, "XF86_", 5) == 0)) {
        xkb_keysym_t ret;
        tmp = strdup(name);
        यदि (!tmp)
            प्रत्यागम XKB_कुञ्चिका_No_चिह्नम्;
        memmove(&tmp[4], &tmp[5], वाक्यदैर्घ्यम्(name) - 5 + 1);
        ret = xkb_keysym_from_name(tmp, flags);
        मुक्त(tmp);
        प्रत्यागम ret;
    }

    प्रत्यागम XKB_कुञ्चिका_No_चिह्नम्;
}

xkb_keysym_t
xkb_utf8_गन्तव्यम्_keysym(ध्रुव वर्ण *बफर, आकार_t आकार)
{
    यदि (!बफर || !आकार)
        प्रत्यागम 0;

    /* Try to parse the parameter as a single UTF-8 encoded character */
    आकार_t दीर्घता = 0;
    ध्रुव uint32_t codepoint = utf8_अग्र_संहिता_point(बफर, आकार, &दीर्घता);
    प्रत्यागम (codepoint == INVALID_UTF8_संहिता_POINT || दीर्घता == 0)
        ? XKB_कुञ्चिका_No_चिह्नम्
        : xkb_utf32_गन्तव्यम्_keysym(codepoint);
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
                         ध्रुव वर्ण *name,
                         ध्रुव वर्ण **reference_name)
{
    यदि (keysym > XKB_KEYSYM_MAX) {
        /* Invalid keysym */
        *reference_name = रिक्त;
        प्रत्यागम false;
    }
    /* [WARNING] We do not check that name, if defined, is a valid for keysym */

    int32_t lo = 0, hi = ARRAY_आकार(deprecated_keysyms) - 1;
    यावत् (hi >= lo) {
        int32_t mid = (lo + hi) / 2;
        यदि (keysym > deprecated_keysyms[mid].keysym) {
            lo = mid + 1;
        } अन्यथा यदि (keysym < deprecated_keysyms[mid].keysym) {
            hi = mid - 1;
        } अन्यथा {
            /* Keysym have some deprecated names */
            यदि (deprecated_keysyms[mid].विस्थापनम् == DEPRECATED_KEYSYM) {
                /* All names are deprecated */
                *reference_name = रिक्त;
                प्रत्यागम true;
            } अन्यथा यदि (deprecated_keysyms[mid].विस्थापनम् == UNICODE_KEYSYM) {
                /* All names are deprecated, but Unicode notation is valid */
                *reference_name = रिक्त;
                यदि (name == रिक्त)
                    प्रत्यागम false;
                यदि (name[0] != 'U')
                    प्रत्यागम true;
                /* Since the current function is internal, assume the name
                 * corresponds to the keysym value and just check its syntax. */
                ध्रुव वर्ण *start = name + 1;
                यावत् (is_xdigit(*start))
                    start++;
                प्रत्यागम *start != '\0' && start > name + 1;
            }
            /* There is a reference name that is not deprecated */
            *reference_name = get_name_by_सूचकांक(deprecated_keysyms[mid].विस्थापनम्);
            यदि (!name) {
                /* No name to check: indicate not deprecated */
                प्रत्यागम false;
            }
            यदि (deprecated_keysyms[mid].explicit_गणनम्) {
                /* Only some explicit names are deprecated */
                ध्रुव uint8_t k_max = deprecated_keysyms[mid].explicit_सूचकांक
                                    + deprecated_keysyms[mid].explicit_गणनम्;
                प्रतिज्ञा(k_max <= ARRAY_आकार(explicit_deprecated_aliases));
                /* Check every deprecated alias */
                प्रति (uint8_t k = deprecated_keysyms[mid].explicit_सूचकांक;
                     k < k_max; k++) {
                    #यदि_परिभाषितम् __clang__
                    /* Make clang-tidy happy */
                    __builtin_assume(k_max <=
                                     ARRAY_आकार(explicit_deprecated_aliases));
                    #यदि_अन्तः
                    ध्रुव वर्ण *alias =
                        get_name_by_सूचकांक(explicit_deprecated_aliases[k]);
                    यदि (वाक्यं_तुलय(name, alias) == 0)
                        प्रत्यागम true;
                }
                प्रत्यागम false;
            } अन्यथा {
                /* All names but the reference one are deprecated */
                प्रत्यागम वाक्यं_तुलय(name, *reference_name) != 0;
            }
        }
    }

    /* Keysym has no deprecated names */
    *reference_name = रिक्त;
    प्रत्यागम false;
}

bool
xkb_keysym_is_keypad(xkb_keysym_t keysym)
{
    प्रत्यागम keysym >= XKB_कुञ्चिका_KP_Space && keysym <= XKB_कुञ्चिका_KP_Equal;
}


bool
xkb_keysym_is_modifier(xkb_keysym_t keysym)
{
    प्रत्यागम
        (keysym >= XKB_कुञ्चिका_सरणम्_L && keysym <= XKB_कुञ्चिका_Hyper_R) ||
        (keysym >= XKB_कुञ्चिका_ISO_तालकम् && keysym <= XKB_कुञ्चिका_ISO_Level5_तालकम्) ||
        keysym == XKB_कुञ्चिका_बहुलकम्_switch ||
        keysym == XKB_कुञ्चिका_अङ्कः_तालकम्;
}
