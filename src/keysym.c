/*
 * For MIT-open-group:
 * Copyright 1985, 1987, 1990, 1998  The Open Group
 *
 * For MIT:
 * Copyright © 2009 Dan Nicholson
 *
 * SPDX-License-Identifier: MIT-open-group AND MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <stdlib.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon-keysyms.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "features/enums.h"
#അന്തര്ഭാവ "keysym.h"
#അന്തര്ഭാവ "keysym-names.h"
#അന്തര്ഭാവ "utf8-decoding.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "util-numbers.h"

സ്ഥിതം ssize_t
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
    എങ്കിൽ (ks > XKB_KEYSYM_MAX_EXPLICIT)
        തിരികെ -1;

    ssize_t lo = 0, hi = ARRAY_SIZE(keysym_to_name) - 1;
    ആകുമ്പോൾ (hi >= lo) {
        ssize_t mid = (lo + hi) / 2;
        എങ്കിൽ (ks > keysym_to_name[mid].keysym) {
            lo = mid + 1;
        } അല്ലെങ്കിൽ എങ്കിൽ (ks < keysym_to_name[mid].keysym) {
            hi = mid - 1;
        } അല്ലെങ്കിൽ {
            തിരികെ mid;
        }
    }

    തിരികെ -1;
}

#define get_name_by_index(index) (keysym_names + (index))

സ്ഥിതം വരിയിൽ സ്ഥിരം അക്ഷരം *
get_name(സ്ഥിരം ഘടന name_keysym *entry)
{
    തിരികെ get_name_by_index(entry->offset);
}

/* Unnamed Unicode codepoint. */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
get_unicode_name(xkb_keysym_t ks, അക്ഷരം *buffer, size_t size)
{
    തിരികെ snprintf(buffer, size, "U%04"PRIX32, ks & UINT32_C(0xffffff));
}

പൂർണ്ണസംഖ്യ
xkb_keysym_get_name(xkb_keysym_t ks, അക്ഷരം *buffer, size_t size)
{
    എങ്കിൽ (ks > XKB_KEYSYM_MAX) {
        snprintf(buffer, size, "Invalid");
        തിരികെ -1;
    }

    ssize_t index = find_keysym_index(ks);
    എങ്കിൽ (index != -1)
        തിരികെ snprintf(buffer, size, "%s", get_name(&keysym_to_name[index]));

    /*
     * Unnamed Unicode codepoint.
     * • Keysyms in the range [XKB_KEYSYM_UNICODE_OFFSET, XKB_KEYSYM_UNICODE_MIN [
     *   do not use the Unicode notation for backward compatibility.
     * • Keysyms are not normalized: e.g. given a Unicode keysym, calling
     *   `xkb_utf32_to_keysym` with the corresponding code point may return a
     *   different keysym or fail (e.g. surrogates are invalid in UTF-32).
     */
    എങ്കിൽ (ks >= XKB_KEYSYM_UNICODE_MIN && ks <= XKB_KEYSYM_UNICODE_MAX)
        തിരികെ get_unicode_name(ks, buffer, size);

    /* Unnamed, non-Unicode, symbol (shouldn't generally happen). */
    തിരികെ snprintf(buffer, size, "0x%08"PRIx32, ks);
}

bool
xkb_keysym_is_assigned(xkb_keysym_t ks)
{
    തിരികെ (XKB_KEYSYM_UNICODE_MIN <= ks && ks <= XKB_KEYSYM_UNICODE_MAX) ||
           find_keysym_index(ks) != -1;
}

പൂർണ്ണസംഖ്യ
xkb_keysym_get_explicit_names(xkb_keysym_t ks, സ്ഥിരം അക്ഷരം **buffer, size_t size)
{
    എങ്കിൽ (ks > XKB_KEYSYM_MAX)
        തിരികെ -1;
    സ്ഥിരം ssize_t index = find_keysym_index(ks);
    എങ്കിൽ (index < 0)
        തിരികെ 0;
    സ്ഥിരം uint16_t canonical = keysym_to_name[index].offset;
    എങ്കിൽ (size > 0)
        buffer[0] = get_name(&keysym_to_name[index]);
    പൂർണ്ണസംഖ്യ എണ്ണം = 1;
    വേണ്ടി (size_t pos = 0; pos < ARRAY_SIZE(name_to_keysym); pos++) {
        എങ്കിൽ (name_to_keysym[pos].keysym == ks &&
            name_to_keysym[pos].offset != canonical) {
            എങ്കിൽ ((size_t) എണ്ണം < size) {
                buffer[എണ്ണം] = get_name(&name_to_keysym[pos]);
            }
            എണ്ണം++;
        }
    }
    തിരികെ എണ്ണം;
}

ഘടന xkb_keysym_iterator {
    bool explicit;       /* If true, traverse only explicitly named keysyms */
    int32_t index;       /* Current index in `keysym_to_name` */
    xkb_keysym_t keysym; /* Current keysym */
};

ഘടന xkb_keysym_iterator*
xkb_keysym_iterator_new(bool iterate_only_explicit_keysyms)
{
    ഘടന xkb_keysym_iterator* iter = calloc(1, വലുപ്പം(*iter));
    എങ്കിൽ (!iter)
        തിരികെ ശൂന്യ;
    iter->explicit = iterate_only_explicit_keysyms;
    iter->index = -1;
    iter->keysym = XKB_KEYSYM_UNICODE_MAX;
    തിരികെ iter;
}

ഘടന xkb_keysym_iterator*
xkb_keysym_iterator_unref(ഘടന xkb_keysym_iterator *iter)
{
    /*
     * [NOTE] If we ever make this API public, add:
     * - an `refcnt` member,
     * - `xkb_keysym_iterator_ref()`,
     * - `assert(!iter || iter->refcnt > 0)`.
     */
    എങ്കിൽ (iter)
        സ്വതന്ത്രം(iter);
    തിരികെ ശൂന്യ;
}

xkb_keysym_t
xkb_keysym_iterator_get_keysym(ഘടന xkb_keysym_iterator *iter)
{
    തിരികെ iter->keysym;
}

bool
xkb_keysym_iterator_is_explicitly_named(ഘടന xkb_keysym_iterator *iter)
{
    തിരികെ iter->index >= 0 &&
           iter->index < (int32_t)ARRAY_SIZE(keysym_to_name) &&
           (iter->explicit ||
            iter->keysym == keysym_to_name[iter->index].keysym);
}

പൂർണ്ണസംഖ്യ
xkb_keysym_iterator_get_name(ഘടന xkb_keysym_iterator *iter,
                             അക്ഷരം *buffer, size_t size)
{
    എങ്കിൽ (iter->index < 0 || iter->index >= (int32_t)ARRAY_SIZE(keysym_to_name))
        തിരികെ -1;
    എങ്കിൽ (iter->explicit || iter->keysym == keysym_to_name[iter->index].keysym)
        തിരികെ snprintf(buffer, size, "%s",
                        get_name(&keysym_to_name[iter->index]));
    തിരികെ get_unicode_name(iter->keysym, buffer, size);
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
xkb_keysym_iterator_next(ഘടന xkb_keysym_iterator *iter)
{
    എങ്കിൽ (iter->index >= (int32_t)ARRAY_SIZE(keysym_to_name) - 1)
        തിരികെ false;

    /* Next keysym */
    എങ്കിൽ (iter->explicit || iter->keysym >= XKB_KEYSYM_UNICODE_MAX ||
        keysym_to_name[iter->index + 1].keysym < XKB_KEYSYM_UNICODE_MIN) {
        /* Explicitly named keysyms only */
        iter->keysym = keysym_to_name[++iter->index].keysym;
        പ്രതിജ്ഞാ(iter->explicit ||
               iter->keysym <= XKB_KEYSYM_UNICODE_MIN ||
               iter->keysym >= XKB_KEYSYM_UNICODE_MAX);
    } അല്ലെങ്കിൽ {
        /* Unicode keysyms
         * NOTE: Unicode keysyms are within keysym_to_name keysyms range. */
        എങ്കിൽ (iter->keysym >= keysym_to_name[iter->index].keysym)
            iter->index++;
        എങ്കിൽ (iter->keysym >= XKB_KEYSYM_UNICODE_MIN) {
            /* Continue Unicode keysyms */
            iter->keysym++;
        } അല്ലെങ്കിൽ {
            /* Start Unicode keysyms */
            iter->keysym = XKB_KEYSYM_UNICODE_MIN;
        }
    }
    തിരികെ true;
}

/* Parse the numeric part of a 0xXXXX and UXXXX keysym. */
സ്ഥിതം bool
parse_keysym_hex(സ്ഥിരം അക്ഷരം *s, uint32_t *out)
{
    /* We expect a NULL-terminated string of length up to 8 */
    സ്ഥിരം പൂർണ്ണസംഖ്യ എണ്ണം = parse_hex_to_uint32_t(s, 8, out);
    /* Check that some value was parsed and we reached the end of the string */
    തിരികെ (എണ്ണം > 0 && s[എണ്ണം] == '\0');
}

xkb_keysym_t
xkb_keysym_from_name(സ്ഥിരം അക്ഷരം *name, എണ്ണൽ xkb_keysym_flags flags)
{
    എങ്കിൽ (flags & ~(എണ്ണൽ xkb_keysym_flags)XKB_KEYSYM_FLAGS_മൂല്യങ്ങൾ)
        തിരികെ XKB_താക്കോൽ_No_ചിഹ്നം;

    സ്ഥിരം ഘടന name_keysym *entry = ശൂന്യ;
    അക്ഷരം *tmp;
    uint32_t val;
    bool icase = (flags & XKB_KEYSYM_CASE_INSENSITIVE);

    /*
     * We need to !icase case to be fast, for e.g. Compose file parsing.
     * So do it in a fast path.
     */
    എങ്കിൽ (!icase) {
        size_t pos = keysym_name_perfect_hash(name);
        എങ്കിൽ (pos < ARRAY_SIZE(name_to_keysym)) {
            സ്ഥിരം അക്ഷരം *s = get_name(&name_to_keysym[pos]);
            എങ്കിൽ (strcmp(name, s) == 0)
                തിരികെ name_to_keysym[pos].keysym;
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
    അല്ലെങ്കിൽ {
        int32_t lo = 0, hi = ARRAY_SIZE(name_to_keysym) - 1;
        ആകുമ്പോൾ (hi >= lo) {
            int32_t mid = (lo + hi) / 2;
            പൂർണ്ണസംഖ്യ cmp = istrcmp(name, get_name(&name_to_keysym[mid]));
            എങ്കിൽ (cmp > 0) {
                lo = mid + 1;
            } അല്ലെങ്കിൽ എങ്കിൽ (cmp < 0) {
                hi = mid - 1;
            } അല്ലെങ്കിൽ {
                entry = &name_to_keysym[mid];
                നിർത്തുക;
            }
        }
        എങ്കിൽ (entry) {
            സ്ഥിരം ഘടന name_keysym *last;
            last = name_to_keysym + ARRAY_SIZE(name_to_keysym) - 1;
            /* Keep going until we reach end of array
             * or non case-insensitve match */
            ആകുമ്പോൾ (entry < last &&
                   istrcmp(get_name(entry + 1), get_name(entry)) == 0) {
                entry++;
            }
            തിരികെ entry->keysym;
        }
    }

    എങ്കിൽ (*name == 'U' || (icase && *name == 'u')) {
        /* Parse Unicode notation */
        എങ്കിൽ (!parse_keysym_hex(&name[1], &val))
            തിരികെ XKB_താക്കോൽ_No_ചിഹ്നം;
        തിരികെ (val > 0xff && val <= 0x10ffff)
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
    അല്ലെങ്കിൽ എങ്കിൽ (name[0] == '0' && (name[1] == 'x' || (icase && name[1] == 'X'))) {
        /*
         * Parse numeric hexadecimal notation without any normalization, in
         * in order to be consistent with the keymap files parsing.
         */
        എങ്കിൽ (!parse_keysym_hex(&name[2], &val) || val > XKB_KEYSYM_MAX)
            തിരികെ XKB_താക്കോൽ_No_ചിഹ്നം;
        തിരികെ (xkb_keysym_t) val;
    }

    /* Stupid inconsistency between the headers and XKeysymDB: the former has
     * no separating underscore, while some XF86* syms in the latter did.
     * As a last ditch effort, try without. */
    എങ്കിൽ (strncmp(name, "XF86_", 5) == 0 ||
        (icase && istrncmp(name, "XF86_", 5) == 0)) {
        xkb_keysym_t ret;
        tmp = strdup(name);
        എങ്കിൽ (!tmp)
            തിരികെ XKB_താക്കോൽ_No_ചിഹ്നം;
        memmove(&tmp[4], &tmp[5], strlen(name) - 5 + 1);
        ret = xkb_keysym_from_name(tmp, flags);
        സ്വതന്ത്രം(tmp);
        തിരികെ ret;
    }

    തിരികെ XKB_താക്കോൽ_No_ചിഹ്നം;
}

xkb_keysym_t
xkb_utf8_to_keysym(സ്ഥിരം അക്ഷരം *buffer, size_t size)
{
    എങ്കിൽ (!buffer || !size)
        തിരികെ 0;

    /* Try to parse the parameter as a single UTF-8 encoded character */
    size_t length = 0;
    സ്ഥിരം uint32_t codepoint = utf8_next_code_point(buffer, size, &length);
    തിരികെ (codepoint == INVALID_UTF8_CODE_POINT || length == 0)
        ? XKB_താക്കോൽ_No_ചിഹ്നം
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
                         സ്ഥിരം അക്ഷരം *name,
                         സ്ഥിരം അക്ഷരം **reference_name)
{
    എങ്കിൽ (keysym > XKB_KEYSYM_MAX) {
        /* Invalid keysym */
        *reference_name = ശൂന്യ;
        തിരികെ false;
    }
    /* [WARNING] We do not check that name, if defined, is a valid for keysym */

    int32_t lo = 0, hi = ARRAY_SIZE(deprecated_keysyms) - 1;
    ആകുമ്പോൾ (hi >= lo) {
        int32_t mid = (lo + hi) / 2;
        എങ്കിൽ (keysym > deprecated_keysyms[mid].keysym) {
            lo = mid + 1;
        } അല്ലെങ്കിൽ എങ്കിൽ (keysym < deprecated_keysyms[mid].keysym) {
            hi = mid - 1;
        } അല്ലെങ്കിൽ {
            /* Keysym have some deprecated names */
            എങ്കിൽ (deprecated_keysyms[mid].offset == DEPRECATED_KEYSYM) {
                /* All names are deprecated */
                *reference_name = ശൂന്യ;
                തിരികെ true;
            } അല്ലെങ്കിൽ എങ്കിൽ (deprecated_keysyms[mid].offset == UNICODE_KEYSYM) {
                /* All names are deprecated, but Unicode notation is valid */
                *reference_name = ശൂന്യ;
                എങ്കിൽ (name == ശൂന്യ)
                    തിരികെ false;
                എങ്കിൽ (name[0] != 'U')
                    തിരികെ true;
                /* Since the current function is internal, assume the name
                 * corresponds to the keysym value and just check its syntax. */
                സ്ഥിരം അക്ഷരം *start = name + 1;
                ആകുമ്പോൾ (is_xdigit(*start))
                    start++;
                തിരികെ *start != '\0' && start > name + 1;
            }
            /* There is a reference name that is not deprecated */
            *reference_name = get_name_by_index(deprecated_keysyms[mid].offset);
            എങ്കിൽ (!name) {
                /* No name to check: indicate not deprecated */
                തിരികെ false;
            }
            എങ്കിൽ (deprecated_keysyms[mid].explicit_എണ്ണം) {
                /* Only some explicit names are deprecated */
                സ്ഥിരം uint8_t k_max = deprecated_keysyms[mid].explicit_index
                                    + deprecated_keysyms[mid].explicit_എണ്ണം;
                പ്രതിജ്ഞാ(k_max <= ARRAY_SIZE(explicit_deprecated_aliases));
                /* Check every deprecated alias */
                വേണ്ടി (uint8_t k = deprecated_keysyms[mid].explicit_index;
                     k < k_max; k++) {
                    #ifdef __clang__
                    /* Make clang-tidy happy */
                    __builtin_assume(k_max <=
                                     ARRAY_SIZE(explicit_deprecated_aliases));
                    #endif
                    സ്ഥിരം അക്ഷരം *alias =
                        get_name_by_index(explicit_deprecated_aliases[k]);
                    എങ്കിൽ (strcmp(name, alias) == 0)
                        തിരികെ true;
                }
                തിരികെ false;
            } അല്ലെങ്കിൽ {
                /* All names but the reference one are deprecated */
                തിരികെ strcmp(name, *reference_name) != 0;
            }
        }
    }

    /* Keysym has no deprecated names */
    *reference_name = ശൂന്യ;
    തിരികെ false;
}

bool
xkb_keysym_is_keypad(xkb_keysym_t keysym)
{
    തിരികെ keysym >= XKB_താക്കോൽ_KP_Space && keysym <= XKB_താക്കോൽ_KP_Equal;
}


bool
xkb_keysym_is_modifier(xkb_keysym_t keysym)
{
    തിരികെ
        (keysym >= XKB_താക്കോൽ_Shift_L && keysym <= XKB_താക്കോൽ_Hyper_R) ||
        (keysym >= XKB_താക്കോൽ_ISO_Lock && keysym <= XKB_താക്കോൽ_ISO_Level5_Lock) ||
        keysym == XKB_താക്കോൽ_Mode_switch ||
        keysym == XKB_താക്കോൽ_അക്കം_Lock;
}
