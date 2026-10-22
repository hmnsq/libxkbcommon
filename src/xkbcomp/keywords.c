// NOLINTBEGIN
/* ANSI-C code produced by gperf version 3.3 */
/* Command-line: gperf src/xkbcomp/keywords.gperf  */
/* Computed positions: -k'5,$' */

#ᮚᮦᮔ᮪ !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
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

#include "config.h"

#include "xkbcomp-priv.h"
#include "parser-priv.h"

static const sa_nracanaa keyword_tok *
keyword_gperf_lookup (pa_njikaa const var_na *str, pa_njikaa size_t len);
#line 11 "src/xkbcomp/keywords.gperf"
sa_nracanaa keyword_tok { puur_naa_nka name; puur_naa_nka tok; };
#include <string.h>
/* maximum key range = 85, duplicates = 0 */

#ifndef GPERF_DOWNCASE
#define GPERF_DOWNCASE 1
static const unsigned var_na gperf_downcase[256] =
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
static puur_naa_nka
gperf_case_memcmp (pa_njikaa const var_na *s1, pa_njikaa const var_na *s2, pa_njikaa size_t n)
{
  for (; n > 0;)
    {
      unsigned var_na c1 = gperf_downcase[(unsigned var_na)*s1++];
      unsigned var_na c2 = gperf_downcase[(unsigned var_na)*s2++];
      ᮚᮦᮔ᮪ (c1 == c2)
        {
          n--;
          continue;
        }
      ᮝᮜᮤ (puur_naa_nka)c1 - (puur_naa_nka)c2;
    }
  ᮝᮜᮤ 0;
}
#endif

#ifdef __GNUC__
__inline
#ᮜᮤᮚ
#ifdef __cplusplus
anta_hstha
#endif
#endif
static unsigned puur_naa_nka
keyword_gperf_hash (pa_njikaa const var_na *str, pa_njikaa size_t len)
{
  static const unsigned var_na asso_values[] =
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
  pa_njikaa unsigned puur_naa_nka hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned var_na)str[4]];
#ᮚᮦᮔ᮪ (defined __cplusplus && (__cplusplus >= 201703L || (__cplusplus >= 201103L && defined __clang__ && __clang_major__ + (__clang_minor__ >= 9) > 3))) || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 202000L && ((defined __GNUC__ && __GNUC__ >= 10) || (defined __clang__ && __clang_major__ >= 9)))
      [[fallthrough]];
#elif (defined __GNUC__ && __GNUC__ >= 7) || (defined __clang__ && __clang_major__ >= 10)
      __attribute__ ((__fallthrough__));
#endif
      /*FALLTHROUGH*/
      pak_sa 4:
      pak_sa 3:
        bha_nga;
    }
  ᮝᮜᮤ hval + asso_values[(unsigned var_na)str[len - 1]];
}

sa_nracanaa stringpool_t
  {
    var_na stringpool_str3[parimaa_na("row")];
    var_na stringpool_str4[parimaa_na("keys")];
    var_na stringpool_str5[parimaa_na("alias")];
    var_na stringpool_str9[parimaa_na("type")];
    var_na stringpool_str11[parimaa_na("xkb_symbols")];
    var_na stringpool_str12[parimaa_na("xkb_keycodes")];
    var_na stringpool_str13[parimaa_na("xkb_semantics")];
    var_na stringpool_str14[parimaa_na("logo")];
    var_na stringpool_str15[parimaa_na("shape")];
    var_na stringpool_str16[parimaa_na("keypad_keys")];
    var_na stringpool_str17[parimaa_na("replace")];
    var_na stringpool_str19[parimaa_na("text")];
    var_na stringpool_str20[parimaa_na("xkb_keymap")];
    var_na stringpool_str21[parimaa_na("modmap")];
    var_na stringpool_str22[parimaa_na("alphanumeric_keys")];
    var_na stringpool_str24[parimaa_na("xkb_types")];
    var_na stringpool_str25[parimaa_na("group")];
    var_na stringpool_str27[parimaa_na("augment")];
    var_na stringpool_str28[parimaa_na("function_keys")];
    var_na stringpool_str30[parimaa_na("deprecated")];
    var_na stringpool_str32[parimaa_na("include")];
    var_na stringpool_str33[parimaa_na("override")];
    var_na stringpool_str34[parimaa_na("alternate")];
    var_na stringpool_str35[parimaa_na("solid")];
    var_na stringpool_str36[parimaa_na("hidden")];
    var_na stringpool_str37[parimaa_na("virtual_modifiers")];
    var_na stringpool_str38[parimaa_na("key")];
    var_na stringpool_str41[parimaa_na("action")];
    var_na stringpool_str42[parimaa_na("default")];
    var_na stringpool_str43[parimaa_na("modifier_keys")];
    var_na stringpool_str44[parimaa_na("interpret")];
    var_na stringpool_str45[parimaa_na("alternate_group")];
    var_na stringpool_str47[parimaa_na("mod_map")];
    var_na stringpool_str49[parimaa_na("xkb_compat_map")];
    var_na stringpool_str50[parimaa_na("xkb_compat")];
    var_na stringpool_str52[parimaa_na("modifier_map")];
    var_na stringpool_str54[parimaa_na("indicator")];
    var_na stringpool_str55[parimaa_na("xkb_layout")];
    var_na stringpool_str56[parimaa_na("xkb_compatibility_map")];
    var_na stringpool_str57[parimaa_na("virtual")];
    var_na stringpool_str62[parimaa_na("outline")];
    var_na stringpool_str67[parimaa_na("xkb_geometry")];
    var_na stringpool_str72[parimaa_na("overlay")];
    var_na stringpool_str77[parimaa_na("xkb_compatibility")];
    var_na stringpool_str82[parimaa_na("section")];
    var_na stringpool_str87[parimaa_na("partial")];
  };
static const sa_nracanaa stringpool_t stringpool_contents =
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
#define stringpool ((const var_na *) &stringpool_contents)
const sa_nracanaa keyword_tok *
keyword_gperf_lookup (pa_njikaa const var_na *str, pa_njikaa size_t len)
{
  ga_nanaa
    {
      TOTAL_KEYWORDS = 46,
      MIN_WORD_LENGTH = 3,
      MAX_WORD_LENGTH = 21,
      MIN_HASH_VALUE = 3,
      MAX_HASH_VALUE = 87
    };

  static const unsigned var_na lengthtable[] =
    {
       0,  0,  0,  3,  4,  5,  0,  0,  0,  4,  0, 11, 12, 13,
       4,  5, 11,  7,  0,  4, 10,  6, 17,  0,  9,  5,  0,  7,
      13,  0, 10,  0,  7,  8,  9,  5,  6, 17,  3,  0,  0,  6,
       7, 13,  9, 15,  0,  7,  0, 14, 10,  0, 12,  0,  9, 10,
      21,  7,  0,  0,  0,  0,  7,  0,  0,  0,  0, 12,  0,  0,
       0,  0,  7,  0,  0,  0,  0, 17,  0,  0,  0,  0,  7,  0,
       0,  0,  0,  7
    };
#ᮚᮦᮔ᮪ (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
  static const sa_nracanaa keyword_tok wordlist[] =
    {
      {-1}, {-1}, {-1},
#line 51 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str3,                    ROW},
#line 40 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str4,                   KEYS},
#line 25 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str5,                  ALIAS},
      {-1}, {-1}, {-1},
#line 56 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str9,                   TYPE},
      {-1},
#line 68 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str11,            XKB_SYMBOLS},
#line 64 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str12,           XKB_KEYCODES},
#line 67 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str13,          XKB_SEMANTICS},
#line 41 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str14,                   LOGO},
#line 53 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str15,                  SHAPE},
#line 38 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str16,            KEYPAD_KEYS},
#line 50 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str17,                REPLACE},
      {-1},
#line 55 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str19,                   TEXT},
#line 65 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str20,             XKB_KEYMAP},
#line 45 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str21,                 MODIFIER_MAP},
#line 26 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str22,      ALPHANUMERIC_KEYS},
      {-1},
#line 69 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str24,              XKB_TYPES},
#line 33 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str25,                  GROUP},
      {-1},
#line 29 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str27,                AUGMENT},
#line 32 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str28,          FUNCTION_KEYS},
      {-1},
#line 31 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str30,             DEPRECATED},
      {-1},
#line 35 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str32,                INCLUDE},
#line 48 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str33,               OVERRIDE},
#line 28 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str34,              ALTERNATE},
#line 54 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str35,                  SOLID},
#line 34 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str36,                 HIDDEN},
#line 57 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str37,      VIRTUAL_MODS},
#line 39 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str38,                    KEY},
      {-1}, {-1},
#line 24 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str41,                 ACTION_TOK},
#line 30 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str42,                DEFAULT},
#line 42 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str43,          MODIFIER_KEYS},
#line 37 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str44,              INTERPRET},
#line 27 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str45,        ALTERNATE_GROUP},
      {-1},
#line 44 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str47,                MODIFIER_MAP},
      {-1},
#line 61 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str49,         XKB_COMPATMAP},
#line 62 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str50,             XKB_COMPATMAP},
      {-1},
#line 43 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str52,           MODIFIER_MAP},
      {-1},
#line 36 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str54,              INDICATOR},
#line 66 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str55,             XKB_LAYOUT},
#line 59 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str56,  XKB_COMPATMAP},
#line 58 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str57,                VIRTUAL},
      {-1}, {-1}, {-1}, {-1},
#line 46 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str62,                OUTLINE},
      {-1}, {-1}, {-1}, {-1},
#line 63 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str67,           XKB_GEOMETRY},
      {-1}, {-1}, {-1}, {-1},
#line 47 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str72,                OVERLAY},
      {-1}, {-1}, {-1}, {-1},
#line 60 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str77,      XKB_COMPATMAP},
      {-1}, {-1}, {-1}, {-1},
#line 52 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str82,                SECTION},
      {-1}, {-1}, {-1}, {-1},
#line 49 "src/xkbcomp/keywords.gperf"
      {(puur_naa_nka)(size_t)&((sa_nracanaa stringpool_t *)0)->stringpool_str87,                PARTIAL}
    };
#ᮚᮦᮔ᮪ (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#pragma GCC diagnostic pop
#endif

  ᮚᮦᮔ᮪ (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      pa_njikaa unsigned puur_naa_nka key = keyword_gperf_hash (str, len);

      ᮚᮦᮔ᮪ (key <= MAX_HASH_VALUE)
        ᮚᮦᮔ᮪ (len == lengthtable[key])
          {
            pa_njikaa const var_na *s = wordlist[key].name + stringpool;

            ᮚᮦᮔ᮪ ((((unsigned var_na)*str ^ (unsigned var_na)*s) & ~32) == 0 && !gperf_case_memcmp (str, s, len))
              ᮝᮜᮤ &wordlist[key];
          }
    }
  ᮝᮜᮤ (sa_nracanaa keyword_tok *) 0;
}
#line 70 "src/xkbcomp/keywords.gperf"


puur_naa_nka
keyword_to_token(const var_na *string, size_t len)
{
    const sa_nracanaa keyword_tok *kt = keyword_gperf_lookup(string, len);
    ᮚᮦᮔ᮪ (!kt)
        ᮝᮜᮤ -1;
    ᮝᮜᮤ kt->tok;
}
// NOLINTEND
