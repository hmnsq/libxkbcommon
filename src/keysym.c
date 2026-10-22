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

ساکن ssize_t
ڈھونڈو_keysym_اشاریہ(xkb_keysym_t ks)
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
        واپس -1;

    ssize_t lo = 0, hi = ARRAY_سائز(keysym_مقصد_name) - 1;
    جب (hi >= lo) {
        ssize_t mid = (lo + hi) / 2;
        اگر (ks > keysym_مقصد_name[mid].keysym) {
            lo = mid + 1;
        } ورنہ اگر (ks < keysym_مقصد_name[mid].keysym) {
            hi = mid - 1;
        } ورنہ {
            واپس mid;
        }
    }

    واپس -1;
}

#تعریف_کرو get_name_by_اشاریہ(اشاریہ) (keysym_نام + (اشاریہ))

ساکن اندرونی ثابت حرف *
get_name(ثابت ساخت name_keysym *اندراج_2)
{
    واپس get_name_by_اشاریہ(اندراج_2->انحراف);
}

/* Unnamed Unicode codepoint. */
ساکن اندرونی صحیح
get_unicode_name(xkb_keysym_t ks, حرف *بفر, سائز_t سائز)
{
    واپس snprintf(بفر, سائز, "U%04"PRIX32, ks & UINT32_C(0xffffff));
}

صحیح
xkb_keysym_get_name(xkb_keysym_t ks, حرف *بفر, سائز_t سائز)
{
    اگر (ks > XKB_KEYSYM_MAX) {
        snprintf(بفر, سائز, "Invalid");
        واپس -1;
    }

    ssize_t اشاریہ = ڈھونڈو_keysym_اشاریہ(ks);
    اگر (اشاریہ != -1)
        واپس snprintf(بفر, سائز, "%s", get_name(&keysym_مقصد_name[اشاریہ]));

    /*
     * Unnamed Unicode codepoint.
     * • Keysyms in the range [XKB_KEYSYM_UNICODE_OFFSET, XKB_KEYSYM_UNICODE_MIN [
     *   do not use the Unicode notation for backward compatibility.
     * • Keysyms are not normalized: e.g. given a Unicode keysym, calling
     *   `xkb_utf32_to_keysym` with the corresponding code point may return a
     *   different keysym or fail (e.g. surrogates are invalid in UTF-32).
     */
    اگر (ks >= XKB_KEYSYM_UNICODE_MIN && ks <= XKB_KEYSYM_UNICODE_MAX)
        واپس get_unicode_name(ks, بفر, سائز);

    /* Unnamed, non-Unicode, symbol (shouldn't generally happen). */
    واپس snprintf(بفر, سائز, "0x%08"PRIx32, ks);
}

bool
xkb_keysym_is_assigned(xkb_keysym_t ks)
{
    واپس (XKB_KEYSYM_UNICODE_MIN <= ks && ks <= XKB_KEYSYM_UNICODE_MAX) ||
           ڈھونڈو_keysym_اشاریہ(ks) != -1;
}

صحیح
xkb_keysym_get_explicit_نام(xkb_keysym_t ks, ثابت حرف **بفر, سائز_t سائز)
{
    اگر (ks > XKB_KEYSYM_MAX)
        واپس -1;
    ثابت ssize_t اشاریہ = ڈھونڈو_keysym_اشاریہ(ks);
    اگر (اشاریہ < 0)
        واپس 0;
    ثابت uint16_t canonical = keysym_مقصد_name[اشاریہ].انحراف;
    اگر (سائز > 0)
        بفر[0] = get_name(&keysym_مقصد_name[اشاریہ]);
    صحیح گنتی = 1;
    برائے (سائز_t pos = 0; pos < ARRAY_سائز(name_مقصد_keysym); pos++) {
        اگر (name_مقصد_keysym[pos].keysym == ks &&
            name_مقصد_keysym[pos].انحراف != canonical) {
            اگر ((سائز_t) گنتی < سائز) {
                بفر[گنتی] = get_name(&name_مقصد_keysym[pos]);
            }
            گنتی++;
        }
    }
    واپس گنتی;
}

ساخت xkb_keysym_iterator {
    bool explicit;       /* If true, traverse only explicitly named keysyms */
    int32_t اشاریہ;       /* Current index in `keysym_to_name` */
    xkb_keysym_t keysym; /* Current keysym */
};

ساخت xkb_keysym_iterator*
xkb_keysym_iterator_new(bool iterate_only_explicit_keysyms)
{
    ساخت xkb_keysym_iterator* iter = صفر_حافظہ_دو(1, حجم(*iter));
    اگر (!iter)
        واپس تہی;
    iter->explicit = iterate_only_explicit_keysyms;
    iter->اشاریہ = -1;
    iter->keysym = XKB_KEYSYM_UNICODE_MAX;
    واپس iter;
}

ساخت xkb_keysym_iterator*
xkb_keysym_iterator_unref(ساخت xkb_keysym_iterator *iter)
{
    /*
     * [NOTE] If we ever make this API public, add:
     * - an `refcnt` member,
     * - `xkb_keysym_iterator_ref()`,
     * - `assert(!iter || iter->refcnt > 0)`.
     */
    اگر (iter)
        آزاد(iter);
    واپس تہی;
}

xkb_keysym_t
xkb_keysym_iterator_get_keysym(ساخت xkb_keysym_iterator *iter)
{
    واپس iter->keysym;
}

bool
xkb_keysym_iterator_is_explicitly_named(ساخت xkb_keysym_iterator *iter)
{
    واپس iter->اشاریہ >= 0 &&
           iter->اشاریہ < (int32_t)ARRAY_سائز(keysym_مقصد_name) &&
           (iter->explicit ||
            iter->keysym == keysym_مقصد_name[iter->اشاریہ].keysym);
}

صحیح
xkb_keysym_iterator_get_name(ساخت xkb_keysym_iterator *iter,
                             حرف *بفر, سائز_t سائز)
{
    اگر (iter->اشاریہ < 0 || iter->اشاریہ >= (int32_t)ARRAY_سائز(keysym_مقصد_name))
        واپس -1;
    اگر (iter->explicit || iter->keysym == keysym_مقصد_name[iter->اشاریہ].keysym)
        واپس snprintf(بفر, سائز, "%s",
                        get_name(&keysym_مقصد_name[iter->اشاریہ]));
    واپس get_unicode_name(iter->keysym, بفر, سائز);
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
xkb_keysym_iterator_اگلا(ساخت xkb_keysym_iterator *iter)
{
    اگر (iter->اشاریہ >= (int32_t)ARRAY_سائز(keysym_مقصد_name) - 1)
        واپس false;

    /* Next keysym */
    اگر (iter->explicit || iter->keysym >= XKB_KEYSYM_UNICODE_MAX ||
        keysym_مقصد_name[iter->اشاریہ + 1].keysym < XKB_KEYSYM_UNICODE_MIN) {
        /* Explicitly named keysyms only */
        iter->keysym = keysym_مقصد_name[++iter->اشاریہ].keysym;
        أكد(iter->explicit ||
               iter->keysym <= XKB_KEYSYM_UNICODE_MIN ||
               iter->keysym >= XKB_KEYSYM_UNICODE_MAX);
    } ورنہ {
        /* Unicode keysyms
         * NOTE: Unicode keysyms are within keysym_to_name keysyms range. */
        اگر (iter->keysym >= keysym_مقصد_name[iter->اشاریہ].keysym)
            iter->اشاریہ++;
        اگر (iter->keysym >= XKB_KEYSYM_UNICODE_MIN) {
            /* Continue Unicode keysyms */
            iter->keysym++;
        } ورنہ {
            /* Start Unicode keysyms */
            iter->keysym = XKB_KEYSYM_UNICODE_MIN;
        }
    }
    واپس true;
}

/* Parse the numeric part of a 0xXXXX and UXXXX keysym. */
ساکن bool
parse_keysym_hex(ثابت حرف *s, uint32_t *برآمد)
{
    /* We expect a NULL-terminated string of length up to 8 */
    ثابت صحیح گنتی = parse_hex_مقصد_uint32_t(s, 8, برآمد);
    /* Check that some value was parsed and we reached the end of the string */
    واپس (گنتی > 0 && s[گنتی] == '\0');
}

xkb_keysym_t
xkb_keysym_from_name(ثابت حرف *name, شمار xkb_keysym_flags flags)
{
    اگر (flags & ~(شمار xkb_keysym_flags)XKB_KEYSYM_FLAGS_اقدار)
        واپس XKB_کلید_No_علامت;

    ثابت ساخت name_keysym *اندراج_2 = تہی;
    حرف *tmp;
    uint32_t val;
    bool icase = (flags & XKB_KEYSYM_CASE_INSENSITIVE);

    /*
     * We need to !icase case to be fast, for e.g. Compose file parsing.
     * So do it in a fast path.
     */
    اگر (!icase) {
        سائز_t pos = keysym_name_perfect_hash(name);
        اگر (pos < ARRAY_سائز(name_مقصد_keysym)) {
            ثابت حرف *s = get_name(&name_مقصد_keysym[pos]);
            اگر (strcmp(name, s) == 0)
                واپس name_مقصد_keysym[pos].keysym;
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
    ورنہ {
        int32_t lo = 0, hi = ARRAY_سائز(name_مقصد_keysym) - 1;
        جب (hi >= lo) {
            int32_t mid = (lo + hi) / 2;
            صحیح cmp = istrcmp(name, get_name(&name_مقصد_keysym[mid]));
            اگر (cmp > 0) {
                lo = mid + 1;
            } ورنہ اگر (cmp < 0) {
                hi = mid - 1;
            } ورنہ {
                اندراج_2 = &name_مقصد_keysym[mid];
                توڑو;
            }
        }
        اگر (اندراج_2) {
            ثابت ساخت name_keysym *آخری;
            آخری = name_مقصد_keysym + ARRAY_سائز(name_مقصد_keysym) - 1;
            /* Keep going until we reach end of array
             * or non case-insensitve match */
            جب (اندراج_2 < آخری &&
                   istrcmp(get_name(اندراج_2 + 1), get_name(اندراج_2)) == 0) {
                اندراج_2++;
            }
            واپس اندراج_2->keysym;
        }
    }

    اگر (*name == 'U' || (icase && *name == 'u')) {
        /* Parse Unicode notation */
        اگر (!parse_keysym_hex(&name[1], &val))
            واپس XKB_کلید_No_علامت;
        واپس (val > 0xff && val <= 0x10ffff)
            /*
             * No normalization.
             *
             * NOTE: It allows surrogates, as we are dealing with Unicode
             * *code points* here, not Unicode *scalars*.
             */
            ? XKB_KEYSYM_UNICODE_انحراف + val
            /*
             * Normalize ISO-8859-1 (Latin-1 + C0 and C1 control code)
             *
             * These code points require special processing to ensure
             * backward compatibility with legacy keysyms.
             */
            : xkb_utf32_مقصد_keysym(val);
    }
    ورنہ اگر (name[0] == '0' && (name[1] == 'x' || (icase && name[1] == 'X'))) {
        /*
         * Parse numeric hexadecimal notation without any normalization, in
         * in order to be consistent with the keymap files parsing.
         */
        اگر (!parse_keysym_hex(&name[2], &val) || val > XKB_KEYSYM_MAX)
            واپس XKB_کلید_No_علامت;
        واپس (xkb_keysym_t) val;
    }

    /* Stupid inconsistency between the headers and XKeysymDB: the former has
     * no separating underscore, while some XF86* syms in the latter did.
     * As a last ditch effort, try without. */
    اگر (لڑی_موازنہ(name, "XF86_", 5) == 0 ||
        (icase && istrncmp(name, "XF86_", 5) == 0)) {
        xkb_keysym_t ret;
        tmp = strdup(name);
        اگر (!tmp)
            واپس XKB_کلید_No_علامت;
        memmove(&tmp[4], &tmp[5], لڑی_لمبائی(name) - 5 + 1);
        ret = xkb_keysym_from_name(tmp, flags);
        آزاد(tmp);
        واپس ret;
    }

    واپس XKB_کلید_No_علامت;
}

xkb_keysym_t
xkb_utf8_مقصد_keysym(ثابت حرف *بفر, سائز_t سائز)
{
    اگر (!بفر || !سائز)
        واپس 0;

    /* Try to parse the parameter as a single UTF-8 encoded character */
    سائز_t لمبائی = 0;
    ثابت uint32_t codepoint = utf8_اگلا_رمز_نقطہ(بفر, سائز, &لمبائی);
    واپس (codepoint == INVALID_UTF8_رمز_نقطہ || لمبائی == 0)
        ? XKB_کلید_No_علامت
        : xkb_utf32_مقصد_keysym(codepoint);
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
    اگر (keysym > XKB_KEYSYM_MAX) {
        /* Invalid keysym */
        *reference_name = تہی;
        واپس false;
    }
    /* [WARNING] We do not check that name, if defined, is a valid for keysym */

    int32_t lo = 0, hi = ARRAY_سائز(deprecated_keysyms) - 1;
    جب (hi >= lo) {
        int32_t mid = (lo + hi) / 2;
        اگر (keysym > deprecated_keysyms[mid].keysym) {
            lo = mid + 1;
        } ورنہ اگر (keysym < deprecated_keysyms[mid].keysym) {
            hi = mid - 1;
        } ورنہ {
            /* Keysym have some deprecated names */
            اگر (deprecated_keysyms[mid].انحراف == DEPRECATED_KEYSYM) {
                /* All names are deprecated */
                *reference_name = تہی;
                واپس true;
            } ورنہ اگر (deprecated_keysyms[mid].انحراف == UNICODE_KEYSYM) {
                /* All names are deprecated, but Unicode notation is valid */
                *reference_name = تہی;
                اگر (name == تہی)
                    واپس false;
                اگر (name[0] != 'U')
                    واپس true;
                /* Since the current function is internal, assume the name
                 * corresponds to the keysym value and just check its syntax. */
                ثابت حرف *آغاز = name + 1;
                جب (is_xdigit(*آغاز))
                    آغاز++;
                واپس *آغاز != '\0' && آغاز > name + 1;
            }
            /* There is a reference name that is not deprecated */
            *reference_name = get_name_by_اشاریہ(deprecated_keysyms[mid].انحراف);
            اگر (!name) {
                /* No name to check: indicate not deprecated */
                واپس false;
            }
            اگر (deprecated_keysyms[mid].explicit_گنتی) {
                /* Only some explicit names are deprecated */
                ثابت uint8_t k_max = deprecated_keysyms[mid].explicit_اشاریہ
                                    + deprecated_keysyms[mid].explicit_گنتی;
                أكد(k_max <= ARRAY_سائز(explicit_deprecated_aliases));
                /* Check every deprecated alias */
                برائے (uint8_t k = deprecated_keysyms[mid].explicit_اشاریہ;
                     k < k_max; k++) {
                    #اگر_تعریف_شدہ __clang__
                    /* Make clang-tidy happy */
                    __builtin_assume(k_max <=
                                     ARRAY_سائز(explicit_deprecated_aliases));
                    #اگر_ختم
                    ثابت حرف *alias =
                        get_name_by_اشاریہ(explicit_deprecated_aliases[k]);
                    اگر (strcmp(name, alias) == 0)
                        واپس true;
                }
                واپس false;
            } ورنہ {
                /* All names but the reference one are deprecated */
                واپس strcmp(name, *reference_name) != 0;
            }
        }
    }

    /* Keysym has no deprecated names */
    *reference_name = تہی;
    واپس false;
}

bool
xkb_keysym_is_keypad(xkb_keysym_t keysym)
{
    واپس keysym >= XKB_کلید_KP_Space && keysym <= XKB_کلید_KP_Equal;
}


bool
xkb_keysym_is_modifier(xkb_keysym_t keysym)
{
    واپس
        (keysym >= XKB_کلید_سرکاؤ_L && keysym <= XKB_کلید_Hyper_R) ||
        (keysym >= XKB_کلید_ISO_تالا && keysym <= XKB_کلید_ISO_Level5_تالا) ||
        keysym == XKB_کلید_منوال_switch ||
        keysym == XKB_کلید_عدد_تالا;
}
