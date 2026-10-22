// NOLINTBEGIN
/* ANSI-C code produced by gperf version 3.3 */
/* Command-line: gperf src/xkbcomp/keywords.gperf  */
/* Computed positions: -k'5,$' */

#എങ്കിൽ !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
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

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ "xkbcomp-priv.h"
#അന്തര്ഭാവ "parser-priv.h"

സ്ഥിതം സ്ഥിരം ഘടന keyword_tok *
keyword_gperf_lookup (രജിസ്റ്റർ സ്ഥിരം അക്ഷരം *str, രജിസ്റ്റർ size_t len);
#line 11 "src/xkbcomp/keywords.gperf"
ഘടന keyword_tok { പൂർണ്ണസംഖ്യ name; പൂർണ്ണസംഖ്യ tok; };
#അന്തര്ഭാവ <string.h>
/* maximum key range = 85, duplicates = 0 */

#ifndef GPERF_DOWNCASE
#define GPERF_DOWNCASE 1
സ്ഥിതം സ്ഥിരം unsigned അക്ഷരം gperf_downcase[256] =
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
സ്ഥിതം പൂർണ്ണസംഖ്യ
gperf_case_memcmp (രജിസ്റ്റർ സ്ഥിരം അക്ഷരം *s1, രജിസ്റ്റർ സ്ഥിരം അക്ഷരം *s2, രജിസ്റ്റർ size_t n)
{
  വേണ്ടി (; n > 0;)
    {
      unsigned അക്ഷരം c1 = gperf_downcase[(unsigned അക്ഷരം)*s1++];
      unsigned അക്ഷരം c2 = gperf_downcase[(unsigned അക്ഷരം)*s2++];
      എങ്കിൽ (c1 == c2)
        {
          n--;
          തുടരുക;
        }
      തിരികെ (പൂർണ്ണസംഖ്യ)c1 - (പൂർണ്ണസംഖ്യ)c2;
    }
  തിരികെ 0;
}
#endif

#ifdef __GNUC__
__inline
#അല്ലെങ്കിൽ
#ifdef __cplusplus
വരിയിൽ
#endif
#endif
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
keyword_gperf_hash (രജിസ്റ്റർ സ്ഥിരം അക്ഷരം *str, രജിസ്റ്റർ size_t len)
{
  സ്ഥിതം സ്ഥിരം unsigned അക്ഷരം asso_മൂല്യങ്ങൾ[] =
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
  രജിസ്റ്റർ unsigned പൂർണ്ണസംഖ്യ hval = len;

  തിരഞ്ഞെടുപ്പ് (hval)
    {
      സ്ഥിരസ്ഥിതി:
        hval += asso_മൂല്യങ്ങൾ[(unsigned അക്ഷരം)str[4]];
#എങ്കിൽ (defined __cplusplus && (__cplusplus >= 201703L || (__cplusplus >= 201103L && defined __clang__ && __clang_major__ + (__clang_minor__ >= 9) > 3))) || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 202000L && ((defined __GNUC__ && __GNUC__ >= 10) || (defined __clang__ && __clang_major__ >= 9)))
      [[fallthrough]];
#അല്ലെങ്കിലെങ്കിൽ (defined __GNUC__ && __GNUC__ >= 7) || (defined __clang__ && __clang_major__ >= 10)
      __attribute__ ((__fallthrough__));
#endif
      /*FALLTHROUGH*/
      അവസ്ഥ 4:
      അവസ്ഥ 3:
        നിർത്തുക;
    }
  തിരികെ hval + asso_മൂല്യങ്ങൾ[(unsigned അക്ഷരം)str[len - 1]];
}

ഘടന stringpool_t
  {
    അക്ഷരം stringpool_str3[വലുപ്പം("row")];
    അക്ഷരം stringpool_str4[വലുപ്പം("keys")];
    അക്ഷരം stringpool_str5[വലുപ്പം("alias")];
    അക്ഷരം stringpool_str9[വലുപ്പം("type")];
    അക്ഷരം stringpool_str11[വലുപ്പം("xkb_symbols")];
    അക്ഷരം stringpool_str12[വലുപ്പം("xkb_keycodes")];
    അക്ഷരം stringpool_str13[വലുപ്പം("xkb_semantics")];
    അക്ഷരം stringpool_str14[വലുപ്പം("logo")];
    അക്ഷരം stringpool_str15[വലുപ്പം("shape")];
    അക്ഷരം stringpool_str16[വലുപ്പം("keypad_keys")];
    അക്ഷരം stringpool_str17[വലുപ്പം("replace")];
    അക്ഷരം stringpool_str19[വലുപ്പം("text")];
    അക്ഷരം stringpool_str20[വലുപ്പം("xkb_keymap")];
    അക്ഷരം stringpool_str21[വലുപ്പം("modmap")];
    അക്ഷരം stringpool_str22[വലുപ്പം("alphanumeric_keys")];
    അക്ഷരം stringpool_str24[വലുപ്പം("xkb_types")];
    അക്ഷരം stringpool_str25[വലുപ്പം("group")];
    അക്ഷരം stringpool_str27[വലുപ്പം("augment")];
    അക്ഷരം stringpool_str28[വലുപ്പം("function_keys")];
    അക്ഷരം stringpool_str30[വലുപ്പം("deprecated")];
    അക്ഷരം stringpool_str32[വലുപ്പം("include")];
    അക്ഷരം stringpool_str33[വലുപ്പം("override")];
    അക്ഷരം stringpool_str34[വലുപ്പം("alternate")];
    അക്ഷരം stringpool_str35[വലുപ്പം("solid")];
    അക്ഷരം stringpool_str36[വലുപ്പം("hidden")];
    അക്ഷരം stringpool_str37[വലുപ്പം("virtual_modifiers")];
    അക്ഷരം stringpool_str38[വലുപ്പം("key")];
    അക്ഷരം stringpool_str41[വലുപ്പം("action")];
    അക്ഷരം stringpool_str42[വലുപ്പം("default")];
    അക്ഷരം stringpool_str43[വലുപ്പം("modifier_keys")];
    അക്ഷരം stringpool_str44[വലുപ്പം("interpret")];
    അക്ഷരം stringpool_str45[വലുപ്പം("alternate_group")];
    അക്ഷരം stringpool_str47[വലുപ്പം("mod_map")];
    അക്ഷരം stringpool_str49[വലുപ്പം("xkb_compat_map")];
    അക്ഷരം stringpool_str50[വലുപ്പം("xkb_compat")];
    അക്ഷരം stringpool_str52[വലുപ്പം("modifier_map")];
    അക്ഷരം stringpool_str54[വലുപ്പം("indicator")];
    അക്ഷരം stringpool_str55[വലുപ്പം("xkb_layout")];
    അക്ഷരം stringpool_str56[വലുപ്പം("xkb_compatibility_map")];
    അക്ഷരം stringpool_str57[വലുപ്പം("virtual")];
    അക്ഷരം stringpool_str62[വലുപ്പം("outline")];
    അക്ഷരം stringpool_str67[വലുപ്പം("xkb_geometry")];
    അക്ഷരം stringpool_str72[വലുപ്പം("overlay")];
    അക്ഷരം stringpool_str77[വലുപ്പം("xkb_compatibility")];
    അക്ഷരം stringpool_str82[വലുപ്പം("section")];
    അക്ഷരം stringpool_str87[വലുപ്പം("partial")];
  };
സ്ഥിതം സ്ഥിരം ഘടന stringpool_t stringpool_contents =
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
#define stringpool ((സ്ഥിരം അക്ഷരം *) &stringpool_contents)
സ്ഥിരം ഘടന keyword_tok *
keyword_gperf_lookup (രജിസ്റ്റർ സ്ഥിരം അക്ഷരം *str, രജിസ്റ്റർ size_t len)
{
  എണ്ണൽ
    {
      TOTAL_KEYWORDS = 46,
      MIN_WORD_LENGTH = 3,
      MAX_WORD_LENGTH = 21,
      MIN_HASH_മൂല്യം = 3,
      MAX_HASH_മൂല്യം = 87
    };

  സ്ഥിതം സ്ഥിരം unsigned അക്ഷരം lengthtable[] =
    {
       0,  0,  0,  3,  4,  5,  0,  0,  0,  4,  0, 11, 12, 13,
       4,  5, 11,  7,  0,  4, 10,  6, 17,  0,  9,  5,  0,  7,
      13,  0, 10,  0,  7,  8,  9,  5,  6, 17,  3,  0,  0,  6,
       7, 13,  9, 15,  0,  7,  0, 14, 10,  0, 12,  0,  9, 10,
      21,  7,  0,  0,  0,  0,  7,  0,  0,  0,  0, 12,  0,  0,
       0,  0,  7,  0,  0,  0,  0, 17,  0,  0,  0,  0,  7,  0,
       0,  0,  0,  7
    };
#എങ്കിൽ (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic തള്ളുക
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
  സ്ഥിതം സ്ഥിരം ഘടന keyword_tok wordlist[] =
    {
      {-1}, {-1}, {-1},
#line 51 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str3,                    ROW},
#line 40 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str4,                   താക്കോലുകൾ_2},
#line 25 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str5,                  ALIAS},
      {-1}, {-1}, {-1},
#line 56 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str9,                   TYPE},
      {-1},
#line 68 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str11,            XKB_SYMBOLS},
#line 64 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str12,           XKB_KEYCODES},
#line 67 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str13,          XKB_SEMANTICS},
#line 41 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str14,                   LOGO},
#line 53 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str15,                  SHAPE},
#line 38 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str16,            KEYPAD_താക്കോലുകൾ},
#line 50 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str17,                REPLACE},
      {-1},
#line 55 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str19,                   TEXT},
#line 65 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str20,             XKB_KEYMAP},
#line 45 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str21,                 MODIFIER_MAP},
#line 26 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str22,      ALPHANUMERIC_താക്കോലുകൾ},
      {-1},
#line 69 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str24,              XKB_TYPES},
#line 33 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str25,                  GROUP},
      {-1},
#line 29 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str27,                AUGMENT},
#line 32 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str28,          FUNCTION_താക്കോലുകൾ},
      {-1},
#line 31 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str30,             DEPRECATED},
      {-1},
#line 35 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str32,                INCLUDE},
#line 48 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str33,               OVERRIDE},
#line 28 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str34,              ALTERNATE},
#line 54 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str35,                  SOLID},
#line 34 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str36,                 HIDDEN},
#line 57 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str37,      VIRTUAL_MODS},
#line 39 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str38,                    താക്കോൽ_3},
      {-1}, {-1},
#line 24 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str41,                 ACTION_TOK},
#line 30 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str42,                DEFAULT},
#line 42 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str43,          MODIFIER_താക്കോലുകൾ},
#line 37 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str44,              INTERPRET},
#line 27 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str45,        ALTERNATE_GROUP},
      {-1},
#line 44 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str47,                MODIFIER_MAP},
      {-1},
#line 61 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str49,         XKB_COMPATMAP},
#line 62 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str50,             XKB_COMPATMAP},
      {-1},
#line 43 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str52,           MODIFIER_MAP},
      {-1},
#line 36 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str54,              INDICATOR},
#line 66 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str55,             XKB_LAYOUT},
#line 59 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str56,  XKB_COMPATMAP},
#line 58 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str57,                VIRTUAL},
      {-1}, {-1}, {-1}, {-1},
#line 46 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str62,                OUTLINE},
      {-1}, {-1}, {-1}, {-1},
#line 63 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str67,           XKB_GEOMETRY},
      {-1}, {-1}, {-1}, {-1},
#line 47 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str72,                OVERLAY},
      {-1}, {-1}, {-1}, {-1},
#line 60 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str77,      XKB_COMPATMAP},
      {-1}, {-1}, {-1}, {-1},
#line 52 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str82,                SECTION},
      {-1}, {-1}, {-1}, {-1},
#line 49 "src/xkbcomp/keywords.gperf"
      {(പൂർണ്ണസംഖ്യ)(size_t)&((ഘടന stringpool_t *)0)->stringpool_str87,                PARTIAL}
    };
#എങ്കിൽ (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic എടുക്കുക
#endif

  എങ്കിൽ (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      രജിസ്റ്റർ unsigned പൂർണ്ണസംഖ്യ താക്കോൽ = keyword_gperf_hash (str, len);

      എങ്കിൽ (താക്കോൽ <= MAX_HASH_മൂല്യം)
        എങ്കിൽ (len == lengthtable[താക്കോൽ])
          {
            രജിസ്റ്റർ സ്ഥിരം അക്ഷരം *s = wordlist[താക്കോൽ].name + stringpool;

            എങ്കിൽ ((((unsigned അക്ഷരം)*str ^ (unsigned അക്ഷരം)*s) & ~32) == 0 && !gperf_case_memcmp (str, s, len))
              തിരികെ &wordlist[താക്കോൽ];
          }
    }
  തിരികെ (ഘടന keyword_tok *) 0;
}
#line 70 "src/xkbcomp/keywords.gperf"


പൂർണ്ണസംഖ്യ
keyword_to_token(സ്ഥിരം അക്ഷരം *string, size_t len)
{
    സ്ഥിരം ഘടന keyword_tok *kt = keyword_gperf_lookup(string, len);
    എങ്കിൽ (!kt)
        തിരികെ -1;
    തിരികെ kt->tok;
}
// NOLINTEND
