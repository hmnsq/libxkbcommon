/*
 * For MIT-open-group:
 * Copyright 1985, 1987, 1990, 1998  The Open Group
 *
 * For MIT:
 * Copyright © 2009 Dan Nicholson
 *
 * SPDX-License-Identifier: MIT-open-group AND MIT
 */

#समावेश "config.h"

#समावेश <assert.h>
#समावेश <stdint.h>
#समावेश <stdlib.h>

#समावेश "xkbcommon/xkbcommon-keysyms.h"
#समावेश "xkbcommon/xkbcommon.h"
#समावेश "features/enums.h"
#समावेश "keysym.h"
#समावेश "keysym-names.h"
#समावेश "utf8-decoding.h"
#समावेश "utils.h"
#समावेश "util-numbers.h"

अटल ssize_t
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
    अगर (ks > XKB_KEYSYM_MAX_EXPLICIT)
        वापस -1;

    ssize_t lo = 0, hi = ARRAY_आकार(keysym_to_name) - 1;
    जबतक (hi >= lo) {
        ssize_t mid = (lo + hi) / 2;
        अगर (ks > keysym_to_name[mid].keysym) {
            lo = mid + 1;
        } अन्यथा अगर (ks < keysym_to_name[mid].keysym) {
            hi = mid - 1;
        } अन्यथा {
            वापस mid;
        }
    }

    वापस -1;
}

#घोषणा get_name_by_सूचकांक(सूचकांक) (keysym_names + (सूचकांक))

अटल अंतरभूत स्थिर अक्षर *
get_name(स्थिर काष्ठा name_keysym *entry)
{
    वापस get_name_by_सूचकांक(entry->offset);
}

/* Unnamed Unicode codepoint. */
अटल अंतरभूत पूर्णांक
get_unicode_name(xkb_keysym_t ks, अक्षर *बफर, माप_प्रकार आकार)
{
    वापस snprintf(बफर, आकार, "U%04"PRIX32, ks & UINT32_C(0xffffff));
}

पूर्णांक
xkb_keysym_get_name(xkb_keysym_t ks, अक्षर *बफर, माप_प्रकार आकार)
{
    अगर (ks > XKB_KEYSYM_MAX) {
        snprintf(बफर, आकार, "Invalid");
        वापस -1;
    }

    ssize_t सूचकांक = find_keysym_सूचकांक(ks);
    अगर (सूचकांक != -1)
        वापस snprintf(बफर, आकार, "%s", get_name(&keysym_to_name[सूचकांक]));

    /*
     * Unnamed Unicode codepoint.
     * • Keysyms in the range [XKB_KEYSYM_UNICODE_OFFSET, XKB_KEYSYM_UNICODE_MIN [
     *   do not use the Unicode notation for backward compatibility.
     * • Keysyms are not normalized: e.g. given a Unicode keysym, calling
     *   `xkb_utf32_to_keysym` with the corresponding code point may return a
     *   different keysym or fail (e.g. surrogates are invalid in UTF-32).
     */
    अगर (ks >= XKB_KEYSYM_UNICODE_MIN && ks <= XKB_KEYSYM_UNICODE_MAX)
        वापस get_unicode_name(ks, बफर, आकार);

    /* Unnamed, non-Unicode, symbol (shouldn't generally happen). */
    वापस snprintf(बफर, आकार, "0x%08"PRIx32, ks);
}

bool
xkb_keysym_is_assigned(xkb_keysym_t ks)
{
    वापस (XKB_KEYSYM_UNICODE_MIN <= ks && ks <= XKB_KEYSYM_UNICODE_MAX) ||
           find_keysym_सूचकांक(ks) != -1;
}

पूर्णांक
xkb_keysym_get_explicit_names(xkb_keysym_t ks, स्थिर अक्षर **बफर, माप_प्रकार आकार)
{
    अगर (ks > XKB_KEYSYM_MAX)
        वापस -1;
    स्थिर ssize_t सूचकांक = find_keysym_सूचकांक(ks);
    अगर (सूचकांक < 0)
        वापस 0;
    स्थिर uint16_t canonical = keysym_to_name[सूचकांक].offset;
    अगर (आकार > 0)
        बफर[0] = get_name(&keysym_to_name[सूचकांक]);
    पूर्णांक गिनती = 1;
    क्रम (माप_प्रकार pos = 0; pos < ARRAY_आकार(name_to_keysym); pos++) {
        अगर (name_to_keysym[pos].keysym == ks &&
            name_to_keysym[pos].offset != canonical) {
            अगर ((माप_प्रकार) गिनती < आकार) {
                बफर[गिनती] = get_name(&name_to_keysym[pos]);
            }
            गिनती++;
        }
    }
    वापस गिनती;
}

काष्ठा xkb_keysym_iterator {
    bool explicit;       /* If true, traverse only explicitly named keysyms */
    int32_t सूचकांक;       /* Current index in `keysym_to_name` */
    xkb_keysym_t keysym; /* Current keysym */
};

काष्ठा xkb_keysym_iterator*
xkb_keysym_iterator_new(bool iterate_only_explicit_keysyms)
{
    काष्ठा xkb_keysym_iterator* iter = सुस्मृति(1, माप(*iter));
    अगर (!iter)
        वापस शून्य;
    iter->explicit = iterate_only_explicit_keysyms;
    iter->सूचकांक = -1;
    iter->keysym = XKB_KEYSYM_UNICODE_MAX;
    वापस iter;
}

काष्ठा xkb_keysym_iterator*
xkb_keysym_iterator_unref(काष्ठा xkb_keysym_iterator *iter)
{
    /*
     * [NOTE] If we ever make this API public, add:
     * - an `refcnt` member,
     * - `xkb_keysym_iterator_ref()`,
     * - `assert(!iter || iter->refcnt > 0)`.
     */
    अगर (iter)
        मुक्त(iter);
    वापस शून्य;
}

xkb_keysym_t
xkb_keysym_iterator_get_keysym(काष्ठा xkb_keysym_iterator *iter)
{
    वापस iter->keysym;
}

bool
xkb_keysym_iterator_is_explicitly_named(काष्ठा xkb_keysym_iterator *iter)
{
    वापस iter->सूचकांक >= 0 &&
           iter->सूचकांक < (int32_t)ARRAY_आकार(keysym_to_name) &&
           (iter->explicit ||
            iter->keysym == keysym_to_name[iter->सूचकांक].keysym);
}

पूर्णांक
xkb_keysym_iterator_get_name(काष्ठा xkb_keysym_iterator *iter,
                             अक्षर *बफर, माप_प्रकार आकार)
{
    अगर (iter->सूचकांक < 0 || iter->सूचकांक >= (int32_t)ARRAY_आकार(keysym_to_name))
        वापस -1;
    अगर (iter->explicit || iter->keysym == keysym_to_name[iter->सूचकांक].keysym)
        वापस snprintf(बफर, आकार, "%s",
                        get_name(&keysym_to_name[iter->सूचकांक]));
    वापस get_unicode_name(iter->keysym, बफर, आकार);
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
xkb_keysym_iterator_अग्र(काष्ठा xkb_keysym_iterator *iter)
{
    अगर (iter->सूचकांक >= (int32_t)ARRAY_आकार(keysym_to_name) - 1)
        वापस false;

    /* Next keysym */
    अगर (iter->explicit || iter->keysym >= XKB_KEYSYM_UNICODE_MAX ||
        keysym_to_name[iter->सूचकांक + 1].keysym < XKB_KEYSYM_UNICODE_MIN) {
        /* Explicitly named keysyms only */
        iter->keysym = keysym_to_name[++iter->सूचकांक].keysym;
        निश्चित(iter->explicit ||
               iter->keysym <= XKB_KEYSYM_UNICODE_MIN ||
               iter->keysym >= XKB_KEYSYM_UNICODE_MAX);
    } अन्यथा {
        /* Unicode keysyms
         * NOTE: Unicode keysyms are within keysym_to_name keysyms range. */
        अगर (iter->keysym >= keysym_to_name[iter->सूचकांक].keysym)
            iter->सूचकांक++;
        अगर (iter->keysym >= XKB_KEYSYM_UNICODE_MIN) {
            /* Continue Unicode keysyms */
            iter->keysym++;
        } अन्यथा {
            /* Start Unicode keysyms */
            iter->keysym = XKB_KEYSYM_UNICODE_MIN;
        }
    }
    वापस true;
}

/* Parse the numeric part of a 0xXXXX and UXXXX keysym. */
अटल bool
parse_keysym_hex(स्थिर अक्षर *s, uint32_t *out)
{
    /* We expect a NULL-terminated string of length up to 8 */
    स्थिर पूर्णांक गिनती = parse_hex_to_uint32_t(s, 8, out);
    /* Check that some value was parsed and we reached the end of the string */
    वापस (गिनती > 0 && s[गिनती] == '\0');
}

xkb_keysym_t
xkb_keysym_from_name(स्थिर अक्षर *name, क्रमागत xkb_keysym_flags flags)
{
    अगर (flags & ~(क्रमागत xkb_keysym_flags)XKB_KEYSYM_FLAGS_मूल्यावली)
        वापस XKB_कुंजी_No_प्रतीक;

    स्थिर काष्ठा name_keysym *entry = शून्य;
    अक्षर *tmp;
    uint32_t val;
    bool icase = (flags & XKB_KEYSYM_CASE_INSENSITIVE);

    /*
     * We need to !icase case to be fast, for e.g. Compose file parsing.
     * So do it in a fast path.
     */
    अगर (!icase) {
        माप_प्रकार pos = keysym_name_perfect_hash(name);
        अगर (pos < ARRAY_आकार(name_to_keysym)) {
            स्थिर अक्षर *s = get_name(&name_to_keysym[pos]);
            अगर (म_भेद(name, s) == 0)
                वापस name_to_keysym[pos].keysym;
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
        int32_t lo = 0, hi = ARRAY_आकार(name_to_keysym) - 1;
        जबतक (hi >= lo) {
            int32_t mid = (lo + hi) / 2;
            पूर्णांक cmp = istrcmp(name, get_name(&name_to_keysym[mid]));
            अगर (cmp > 0) {
                lo = mid + 1;
            } अन्यथा अगर (cmp < 0) {
                hi = mid - 1;
            } अन्यथा {
                entry = &name_to_keysym[mid];
                अवरोध;
            }
        }
        अगर (entry) {
            स्थिर काष्ठा name_keysym *last;
            last = name_to_keysym + ARRAY_आकार(name_to_keysym) - 1;
            /* Keep going until we reach end of array
             * or non case-insensitve match */
            जबतक (entry < last &&
                   istrcmp(get_name(entry + 1), get_name(entry)) == 0) {
                entry++;
            }
            वापस entry->keysym;
        }
    }

    अगर (*name == 'U' || (icase && *name == 'u')) {
        /* Parse Unicode notation */
        अगर (!parse_keysym_hex(&name[1], &val))
            वापस XKB_कुंजी_No_प्रतीक;
        वापस (val > 0xff && val <= 0x10ffff)
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
    अन्यथा अगर (name[0] == '0' && (name[1] == 'x' || (icase && name[1] == 'X'))) {
        /*
         * Parse numeric hexadecimal notation without any normalization, in
         * in order to be consistent with the keymap files parsing.
         */
        अगर (!parse_keysym_hex(&name[2], &val) || val > XKB_KEYSYM_MAX)
            वापस XKB_कुंजी_No_प्रतीक;
        वापस (xkb_keysym_t) val;
    }

    /* Stupid inconsistency between the headers and XKeysymDB: the former has
     * no separating underscore, while some XF86* syms in the latter did.
     * As a last ditch effort, try without. */
    अगर (म_भेदन(name, "XF86_", 5) == 0 ||
        (icase && istrncmp(name, "XF86_", 5) == 0)) {
        xkb_keysym_t ret;
        tmp = strdup(name);
        अगर (!tmp)
            वापस XKB_कुंजी_No_प्रतीक;
        स_हटाओ(&tmp[4], &tmp[5], म_माप(name) - 5 + 1);
        ret = xkb_keysym_from_name(tmp, flags);
        मुक्त(tmp);
        वापस ret;
    }

    वापस XKB_कुंजी_No_प्रतीक;
}

xkb_keysym_t
xkb_utf8_to_keysym(स्थिर अक्षर *बफर, माप_प्रकार आकार)
{
    अगर (!बफर || !आकार)
        वापस 0;

    /* Try to parse the parameter as a single UTF-8 encoded character */
    माप_प्रकार लंबाई = 0;
    स्थिर uint32_t codepoint = utf8_अग्र_code_point(बफर, आकार, &लंबाई);
    वापस (codepoint == INVALID_UTF8_CODE_POINT || लंबाई == 0)
        ? XKB_कुंजी_No_प्रतीक
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
                         स्थिर अक्षर *name,
                         स्थिर अक्षर **reference_name)
{
    अगर (keysym > XKB_KEYSYM_MAX) {
        /* Invalid keysym */
        *reference_name = शून्य;
        वापस false;
    }
    /* [WARNING] We do not check that name, if defined, is a valid for keysym */

    int32_t lo = 0, hi = ARRAY_आकार(deprecated_keysyms) - 1;
    जबतक (hi >= lo) {
        int32_t mid = (lo + hi) / 2;
        अगर (keysym > deprecated_keysyms[mid].keysym) {
            lo = mid + 1;
        } अन्यथा अगर (keysym < deprecated_keysyms[mid].keysym) {
            hi = mid - 1;
        } अन्यथा {
            /* Keysym have some deprecated names */
            अगर (deprecated_keysyms[mid].offset == DEPRECATED_KEYSYM) {
                /* All names are deprecated */
                *reference_name = शून्य;
                वापस true;
            } अन्यथा अगर (deprecated_keysyms[mid].offset == UNICODE_KEYSYM) {
                /* All names are deprecated, but Unicode notation is valid */
                *reference_name = शून्य;
                अगर (name == शून्य)
                    वापस false;
                अगर (name[0] != 'U')
                    वापस true;
                /* Since the current function is internal, assume the name
                 * corresponds to the keysym value and just check its syntax. */
                स्थिर अक्षर *start = name + 1;
                जबतक (is_xdigit(*start))
                    start++;
                वापस *start != '\0' && start > name + 1;
            }
            /* There is a reference name that is not deprecated */
            *reference_name = get_name_by_सूचकांक(deprecated_keysyms[mid].offset);
            अगर (!name) {
                /* No name to check: indicate not deprecated */
                वापस false;
            }
            अगर (deprecated_keysyms[mid].explicit_गिनती) {
                /* Only some explicit names are deprecated */
                स्थिर uint8_t k_max = deprecated_keysyms[mid].explicit_सूचकांक
                                    + deprecated_keysyms[mid].explicit_गिनती;
                निश्चित(k_max <= ARRAY_आकार(explicit_deprecated_aliases));
                /* Check every deprecated alias */
                क्रम (uint8_t k = deprecated_keysyms[mid].explicit_सूचकांक;
                     k < k_max; k++) {
                    #अगर_घोषित __clang__
                    /* Make clang-tidy happy */
                    __builtin_assume(k_max <=
                                     ARRAY_आकार(explicit_deprecated_aliases));
                    #पूर्ण_अगर
                    स्थिर अक्षर *alias =
                        get_name_by_सूचकांक(explicit_deprecated_aliases[k]);
                    अगर (म_भेद(name, alias) == 0)
                        वापस true;
                }
                वापस false;
            } अन्यथा {
                /* All names but the reference one are deprecated */
                वापस म_भेद(name, *reference_name) != 0;
            }
        }
    }

    /* Keysym has no deprecated names */
    *reference_name = शून्य;
    वापस false;
}

bool
xkb_keysym_is_keypad(xkb_keysym_t keysym)
{
    वापस keysym >= XKB_कुंजी_KP_Space && keysym <= XKB_कुंजी_KP_Equal;
}


bool
xkb_keysym_is_modifier(xkb_keysym_t keysym)
{
    वापस
        (keysym >= XKB_कुंजी_Shift_L && keysym <= XKB_कुंजी_Hyper_R) ||
        (keysym >= XKB_कुंजी_ISO_Lock && keysym <= XKB_कुंजी_ISO_Level5_Lock) ||
        keysym == XKB_कुंजी_Mode_switch ||
        keysym == XKB_कुंजी_अंक_Lock;
}
