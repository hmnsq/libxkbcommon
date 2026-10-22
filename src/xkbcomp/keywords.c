// NOLINTBEGIN
/* ANSI-C code produced by gperf version 3.3 */
/* Command-line: gperf src/xkbcomp/keywords.gperf  */
/* Computed positions: -k'5,$' */

#যদি !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "src/xkbcomp/keywords.gperf"

#অন্তর্ভাব "config.h"

#অন্তর্ভাব "xkbcomp-priv.h"
#অন্তর্ভাব "parser-priv.h"

স্থির ধ্রুবক কাঠামো keyword_tok *
keyword_gperf_lookup (নিবন্ধ ধ্রুবক অক্ষর *str, নিবন্ধ আকার_t len);
#line 11 "src/xkbcomp/keywords.gperf"
কাঠামো keyword_tok { পূর্ণসংখ্যা name; পূর্ণসংখ্যা tok; };
#অন্তর্ভাব <string.h>
/* maximum key range = 85, duplicates = 0 */

#ifndef GPERF_DOWNCASE
#define GPERF_DOWNCASE 1
স্থির ধ্রুবক unsigned অক্ষর gperf_downcase[256] =
  {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
     15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,
     30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,
     45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
     60,  61,  62,  63,  64,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106,
    107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
    122,  91,  92,  93,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104,
    105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
    120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134,
    135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
    150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
    165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
    180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194,
    195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
    210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224,
    225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254,
    255
  };
#endif

#ifndef GPERF_CASE_MEMCMP
#define GPERF_CASE_MEMCMP 1
স্থির পূর্ণসংখ্যা
gperf_case_memcmp (নিবন্ধ ধ্রুবক অক্ষর *s1, নিবন্ধ ধ্রুবক অক্ষর *s2, নিবন্ধ আকার_t n)
{
  জন্য (; n > 0;)
    {
      unsigned অক্ষর c1 = gperf_downcase[(unsigned অক্ষর)*s1++];
      unsigned অক্ষর c2 = gperf_downcase[(unsigned অক্ষর)*s2++];
      যদি (c1 == c2)
        {
          n--;
          চালু;
        }
      ফেরত (পূর্ণসংখ্যা)c1 - (পূর্ণসংখ্যা)c2;
    }
  ফেরত 0;
}
#endif

#ifdef __GNUC__
__inline
#অন্যথা
#ifdef __cplusplus
অন্তঃরেখ
#endif
#endif
স্থির unsigned পূর্ণসংখ্যা
keyword_gperf_hash (নিবন্ধ ধ্রুবক অক্ষর *str, নিবন্ধ আকার_t len)
{
  স্থির ধ্রুবক unsigned অক্ষর asso_মানসমূহ[] =
    {
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88,  5, 88, 25, 15,  5,
      30, 20, 88, 50, 88,  0, 30, 30, 25, 10,
      10, 88, 20,  0, 15, 20, 88,  0, 88, 35,
      88, 88, 88, 88, 88, 88, 88,  5, 88, 25,
      15,  5, 30, 20, 88, 50, 88,  0, 30, 30,
      25, 10, 10, 88, 20,  0, 15, 20, 88,  0,
      88, 35, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88
    };
  নিবন্ধ unsigned পূর্ণসংখ্যা hval = len;

  নির্বাচন (hval)
    {
      পূর্বনির্ধারিত:
        hval += asso_মানসমূহ[(unsigned অক্ষর)str[4]];
#যদি (defined __cplusplus && (__cplusplus >= 201703L || (__cplusplus >= 201103L && defined __clang__ && __clang_major__ + (__clang_minor__ >= 9) > 3))) || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 202000L && ((defined __GNUC__ && __GNUC__ >= 10) || (defined __clang__ && __clang_major__ >= 9)))
      [[fallthrough]];
#নতুবা_যদি (defined __GNUC__ && __GNUC__ >= 7) || (defined __clang__ && __clang_major__ >= 10)
      __attribute__ ((__fallthrough__));
#endif
      /*FALLTHROUGH*/
      ক্ষেত্র 4:
      ক্ষেত্র 3:
        বিরতি;
    }
  ফেরত hval + asso_মানসমূহ[(unsigned অক্ষর)str[len - 1]];
}

কাঠামো stringpool_t
  {
    অক্ষর stringpool_str3[আকার("row")];
    অক্ষর stringpool_str4[আকার("keys")];
    অক্ষর stringpool_str5[আকার("alias")];
    অক্ষর stringpool_str9[আকার("type")];
    অক্ষর stringpool_str11[আকার("xkb_symbols")];
    অক্ষর stringpool_str12[আকার("xkb_keycodes")];
    অক্ষর stringpool_str13[আকার("xkb_semantics")];
    অক্ষর stringpool_str14[আকার("logo")];
    অক্ষর stringpool_str15[আকার("shape")];
    অক্ষর stringpool_str16[আকার("keypad_keys")];
    অক্ষর stringpool_str17[আকার("replace")];
    অক্ষর stringpool_str19[আকার("text")];
    অক্ষর stringpool_str20[আকার("xkb_keymap")];
    অক্ষর stringpool_str21[আকার("modmap")];
    অক্ষর stringpool_str22[আকার("alphanumeric_keys")];
    অক্ষর stringpool_str24[আকার("xkb_types")];
    অক্ষর stringpool_str25[আকার("group")];
    অক্ষর stringpool_str27[আকার("augment")];
    অক্ষর stringpool_str28[আকার("function_keys")];
    অক্ষর stringpool_str30[আকার("deprecated")];
    অক্ষর stringpool_str32[আকার("include")];
    অক্ষর stringpool_str33[আকার("override")];
    অক্ষর stringpool_str34[আকার("alternate")];
    অক্ষর stringpool_str35[আকার("solid")];
    অক্ষর stringpool_str36[আকার("hidden")];
    অক্ষর stringpool_str37[আকার("virtual_modifiers")];
    অক্ষর stringpool_str38[আকার("key")];
    অক্ষর stringpool_str41[আকার("action")];
    অক্ষর stringpool_str42[আকার("default")];
    অক্ষর stringpool_str43[আকার("modifier_keys")];
    অক্ষর stringpool_str44[আকার("interpret")];
    অক্ষর stringpool_str45[আকার("alternate_group")];
    অক্ষর stringpool_str47[আকার("mod_map")];
    অক্ষর stringpool_str49[আকার("xkb_compat_map")];
    অক্ষর stringpool_str50[আকার("xkb_compat")];
    অক্ষর stringpool_str52[আকার("modifier_map")];
    অক্ষর stringpool_str54[আকার("indicator")];
    অক্ষর stringpool_str55[আকার("xkb_layout")];
    অক্ষর stringpool_str56[আকার("xkb_compatibility_map")];
    অক্ষর stringpool_str57[আকার("virtual")];
    অক্ষর stringpool_str62[আকার("outline")];
    অক্ষর stringpool_str67[আকার("xkb_geometry")];
    অক্ষর stringpool_str72[আকার("overlay")];
    অক্ষর stringpool_str77[আকার("xkb_compatibility")];
    অক্ষর stringpool_str82[আকার("section")];
    অক্ষর stringpool_str87[আকার("partial")];
  };
স্থির ধ্রুবক কাঠামো stringpool_t stringpool_contents =
  {
    "row",
    "keys",
    "alias",
    "type",
    "xkb_symbols",
    "xkb_keycodes",
    "xkb_semantics",
    "logo",
    "shape",
    "keypad_keys",
    "replace",
    "text",
    "xkb_keymap",
    "modmap",
    "alphanumeric_keys",
    "xkb_types",
    "group",
    "augment",
    "function_keys",
    "deprecated",
    "include",
    "override",
    "alternate",
    "solid",
    "hidden",
    "virtual_modifiers",
    "key",
    "action",
    "default",
    "modifier_keys",
    "interpret",
    "alternate_group",
    "mod_map",
    "xkb_compat_map",
    "xkb_compat",
    "modifier_map",
    "indicator",
    "xkb_layout",
    "xkb_compatibility_map",
    "virtual",
    "outline",
    "xkb_geometry",
    "overlay",
    "xkb_compatibility",
    "section",
    "partial"
  };
#define stringpool ((ধ্রুবক অক্ষর *) &stringpool_contents)
ধ্রুবক কাঠামো keyword_tok *
keyword_gperf_lookup (নিবন্ধ ধ্রুবক অক্ষর *str, নিবন্ধ আকার_t len)
{
  গণনা
    {
      যোগ_KEYWORDS = 46,
      MIN_WORD_দৈর্ঘ্য = 3,
      MAX_WORD_দৈর্ঘ্য = 21,
      MIN_HASH_মান = 3,
      MAX_HASH_মান = 87
    };

  স্থির ধ্রুবক unsigned অক্ষর lengthtable[] =
    {
       0,  0,  0,  3,  4,  5,  0,  0,  0,  4,  0, 11, 12, 13,
       4,  5, 11,  7,  0,  4, 10,  6, 17,  0,  9,  5,  0,  7,
      13,  0, 10,  0,  7,  8,  9,  5,  6, 17,  3,  0,  0,  6,
       7, 13,  9, 15,  0,  7,  0, 14, 10,  0, 12,  0,  9, 10,
      21,  7,  0,  0,  0,  0,  7,  0,  0,  0,  0, 12,  0,  0,
       0,  0,  7,  0,  0,  0,  0, 17,  0,  0,  0,  0,  7,  0,
       0,  0,  0,  7
    };
#যদি (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic ঠেলো
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
  স্থির ধ্রুবক কাঠামো keyword_tok wordlist[] =
    {
      {-1}, {-1}, {-1},
#line 51 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str3,                    ROW},
#line 40 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str4,                   চাবিগুলি_2},
#line 25 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str5,                  ALIAS},
      {-1}, {-1}, {-1},
#line 56 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str9,                   TYPE},
      {-1},
#line 68 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str11,            XKB_SYMBOLS},
#line 64 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str12,           XKB_KEYCODES},
#line 67 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str13,          XKB_SEMANTICS},
#line 41 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str14,                   LOGO},
#line 53 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str15,                  SHAPE},
#line 38 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str16,            KEYPAD_চাবিগুলি},
#line 50 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str17,                REPLACE},
      {-1},
#line 55 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str19,                   TEXT},
#line 65 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str20,             XKB_KEYMAP},
#line 45 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str21,                 MODIFIER_MAP},
#line 26 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str22,      ALPHANUMERIC_চাবিগুলি},
      {-1},
#line 69 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str24,              XKB_TYPES},
#line 33 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str25,                  GROUP},
      {-1},
#line 29 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str27,                AUGMENT},
#line 32 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str28,          FUNCTION_চাবিগুলি},
      {-1},
#line 31 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str30,             DEPRECATED},
      {-1},
#line 35 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str32,                INCLUDE},
#line 48 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str33,               OVERRIDE},
#line 28 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str34,              ALTERNATE},
#line 54 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str35,                  SOLID},
#line 34 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str36,                 HIDDEN},
#line 57 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str37,      VIRTUAL_MODS},
#line 39 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str38,                    চাবি_3},
      {-1}, {-1},
#line 24 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str41,                 ACTION_TOK},
#line 30 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str42,                DEFAULT},
#line 42 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str43,          MODIFIER_চাবিগুলি},
#line 37 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str44,              INTERPRET},
#line 27 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str45,        ALTERNATE_GROUP},
      {-1},
#line 44 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str47,                MODIFIER_MAP},
      {-1},
#line 61 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str49,         XKB_COMPATMAP},
#line 62 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str50,             XKB_COMPATMAP},
      {-1},
#line 43 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str52,           MODIFIER_MAP},
      {-1},
#line 36 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str54,              INDICATOR},
#line 66 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str55,             XKB_LAYOUT},
#line 59 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str56,  XKB_COMPATMAP},
#line 58 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str57,                VIRTUAL},
      {-1}, {-1}, {-1}, {-1},
#line 46 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str62,                OUTLINE},
      {-1}, {-1}, {-1}, {-1},
#line 63 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str67,           XKB_GEOMETRY},
      {-1}, {-1}, {-1}, {-1},
#line 47 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str72,                OVERLAY},
      {-1}, {-1}, {-1}, {-1},
#line 60 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str77,      XKB_COMPATMAP},
      {-1}, {-1}, {-1}, {-1},
#line 52 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str82,                SECTION},
      {-1}, {-1}, {-1}, {-1},
#line 49 "src/xkbcomp/keywords.gperf"
      {(পূর্ণসংখ্যা)(আকার_t)&((কাঠামো stringpool_t *)0)->stringpool_str87,                PARTIAL}
    };
#যদি (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic বের_করো
#endif

  যদি (len <= MAX_WORD_দৈর্ঘ্য && len >= MIN_WORD_দৈর্ঘ্য)
    {
      নিবন্ধ unsigned পূর্ণসংখ্যা চাবি = keyword_gperf_hash (str, len);

      যদি (চাবি <= MAX_HASH_মান)
        যদি (len == lengthtable[চাবি])
          {
            নিবন্ধ ধ্রুবক অক্ষর *s = wordlist[চাবি].name + stringpool;

            যদি ((((unsigned অক্ষর)*str ^ (unsigned অক্ষর)*s) & ~32) == 0 && !gperf_case_memcmp (str, s, len))
              ফেরত &wordlist[চাবি];
          }
    }
  ফেরত (কাঠামো keyword_tok *) 0;
}
#line 70 "src/xkbcomp/keywords.gperf"


পূর্ণসংখ্যা
keyword_to_token(ধ্রুবক অক্ষর *string, আকার_t len)
{
    ধ্রুবক কাঠামো keyword_tok *kt = keyword_gperf_lookup(string, len);
    যদি (!kt)
        ফেরত -1;
    ফেরত kt->tok;
}
// NOLINTEND
