// NOLINTBEGIN
/* ANSI-C code produced by gperf version 3.3 */
/* Command-line: gperf src/xkbcomp/keywords.gperf  */
/* Computed positions: -k'5,$' */

#ಆದರೆ !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
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
#ದೋಷ "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#ಆದರೆ_ಅಂತ್ಯ

#ಸಾಲು 1 "src/xkbcomp/keywords.gperf"

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"
#ಅನ್ತರ್ಭಾವ "parser-priv.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ keyword_tok *
keyword_gperf_lookup (ನೋಂದಣಿ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str, ನೋಂದಣಿ ಗಾತ್ರ_t len);
#ಸಾಲು 11 "src/xkbcomp/keywords.gperf"
ರಚನೆ keyword_tok { ಪೂರ್ಣಾಂಕ name; ಪೂರ್ಣಾಂಕ tok; };
#ಅನ್ತರ್ಭಾವ <string.h>
/* maximum key range = 85, duplicates = 0 */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GPERF_DOWNCASE
#ವ್ಯಾಖ್ಯಾನಿಸು GPERF_DOWNCASE 1
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ gperf_downcase[256] =
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
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GPERF_CASE_MEMCMP
#ವ್ಯಾಖ್ಯಾನಿಸು GPERF_CASE_MEMCMP 1
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
gperf_case_memcmp (ನೋಂದಣಿ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s1, ನೋಂದಣಿ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s2, ನೋಂದಣಿ ಗಾತ್ರ_t n)
{
  ಗಾಗಿ (; n > 0;)
    {
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c1 = gperf_downcase[(ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)*s1++];
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c2 = gperf_downcase[(ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)*s2++];
      ಆದರೆ (c1 == c2)
        {
          n--;
          ಮುಂದುವರಿ;
        }
      ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ)c1 - (ಪೂರ್ಣಾಂಕ)c2;
    }
  ಹಿಂತಿರುಗಿ 0;
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __GNUC__
__inline
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
ಸಾಲೊಳಗೆ
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ
keyword_gperf_hash (ನೋಂದಣಿ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str, ನೋಂದಣಿ ಗಾತ್ರ_t len)
{
  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ asso_ಮೌಲ್ಯಗಳು[] =
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
  ನೋಂದಣಿ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ hval = len;

  ಆಯ್ಕೆ (hval)
    {
      ಪೂರ್ವನಿಯೋಜಿತ:
        hval += asso_ಮೌಲ್ಯಗಳು[(ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)str[4]];
#ಆದರೆ (defined __cplusplus && (__cplusplus >= 201703L || (__cplusplus >= 201103L && defined __clang__ && __clang_major__ + (__clang_minor__ >= 9) > 3))) || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 202000L && ((defined __GNUC__ && __GNUC__ >= 10) || (defined __clang__ && __clang_major__ >= 9)))
      [[fallthrough]];
#ಅಥವಾಆದರೆ (defined __GNUC__ && __GNUC__ >= 7) || (defined __clang__ && __clang_major__ >= 10)
      __attribute__ ((__fallthrough__));
#ಆದರೆ_ಅಂತ್ಯ
      /*FALLTHROUGH*/
      ಸ್ಥಿತಿ 4:
      ಸ್ಥಿತಿ 3:
        ನಿಲ್ಲಿಸು;
    }
  ಹಿಂತಿರುಗಿ hval + asso_ಮೌಲ್ಯಗಳು[(ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)str[len - 1]];
}

ರಚನೆ stringpool_t
  {
    ಅಕ್ಷರ stringpool_str3[ಗಾತ್ರ("row")];
    ಅಕ್ಷರ stringpool_str4[ಗಾತ್ರ("keys")];
    ಅಕ್ಷರ stringpool_str5[ಗಾತ್ರ("alias")];
    ಅಕ್ಷರ stringpool_str9[ಗಾತ್ರ("type")];
    ಅಕ್ಷರ stringpool_str11[ಗಾತ್ರ("xkb_symbols")];
    ಅಕ್ಷರ stringpool_str12[ಗಾತ್ರ("xkb_keycodes")];
    ಅಕ್ಷರ stringpool_str13[ಗಾತ್ರ("xkb_semantics")];
    ಅಕ್ಷರ stringpool_str14[ಗಾತ್ರ("logo")];
    ಅಕ್ಷರ stringpool_str15[ಗಾತ್ರ("shape")];
    ಅಕ್ಷರ stringpool_str16[ಗಾತ್ರ("keypad_keys")];
    ಅಕ್ಷರ stringpool_str17[ಗಾತ್ರ("replace")];
    ಅಕ್ಷರ stringpool_str19[ಗಾತ್ರ("text")];
    ಅಕ್ಷರ stringpool_str20[ಗಾತ್ರ("xkb_keymap")];
    ಅಕ್ಷರ stringpool_str21[ಗಾತ್ರ("modmap")];
    ಅಕ್ಷರ stringpool_str22[ಗಾತ್ರ("alphanumeric_keys")];
    ಅಕ್ಷರ stringpool_str24[ಗಾತ್ರ("xkb_types")];
    ಅಕ್ಷರ stringpool_str25[ಗಾತ್ರ("group")];
    ಅಕ್ಷರ stringpool_str27[ಗಾತ್ರ("augment")];
    ಅಕ್ಷರ stringpool_str28[ಗಾತ್ರ("function_keys")];
    ಅಕ್ಷರ stringpool_str30[ಗಾತ್ರ("deprecated")];
    ಅಕ್ಷರ stringpool_str32[ಗಾತ್ರ("include")];
    ಅಕ್ಷರ stringpool_str33[ಗಾತ್ರ("override")];
    ಅಕ್ಷರ stringpool_str34[ಗಾತ್ರ("alternate")];
    ಅಕ್ಷರ stringpool_str35[ಗಾತ್ರ("solid")];
    ಅಕ್ಷರ stringpool_str36[ಗಾತ್ರ("hidden")];
    ಅಕ್ಷರ stringpool_str37[ಗಾತ್ರ("virtual_modifiers")];
    ಅಕ್ಷರ stringpool_str38[ಗಾತ್ರ("key")];
    ಅಕ್ಷರ stringpool_str41[ಗಾತ್ರ("action")];
    ಅಕ್ಷರ stringpool_str42[ಗಾತ್ರ("default")];
    ಅಕ್ಷರ stringpool_str43[ಗಾತ್ರ("modifier_keys")];
    ಅಕ್ಷರ stringpool_str44[ಗಾತ್ರ("interpret")];
    ಅಕ್ಷರ stringpool_str45[ಗಾತ್ರ("alternate_group")];
    ಅಕ್ಷರ stringpool_str47[ಗಾತ್ರ("mod_map")];
    ಅಕ್ಷರ stringpool_str49[ಗಾತ್ರ("xkb_compat_map")];
    ಅಕ್ಷರ stringpool_str50[ಗಾತ್ರ("xkb_compat")];
    ಅಕ್ಷರ stringpool_str52[ಗಾತ್ರ("modifier_map")];
    ಅಕ್ಷರ stringpool_str54[ಗಾತ್ರ("indicator")];
    ಅಕ್ಷರ stringpool_str55[ಗಾತ್ರ("xkb_layout")];
    ಅಕ್ಷರ stringpool_str56[ಗಾತ್ರ("xkb_compatibility_map")];
    ಅಕ್ಷರ stringpool_str57[ಗಾತ್ರ("virtual")];
    ಅಕ್ಷರ stringpool_str62[ಗಾತ್ರ("outline")];
    ಅಕ್ಷರ stringpool_str67[ಗಾತ್ರ("xkb_geometry")];
    ಅಕ್ಷರ stringpool_str72[ಗಾತ್ರ("overlay")];
    ಅಕ್ಷರ stringpool_str77[ಗಾತ್ರ("xkb_compatibility")];
    ಅಕ್ಷರ stringpool_str82[ಗಾತ್ರ("section")];
    ಅಕ್ಷರ stringpool_str87[ಗಾತ್ರ("partial")];
  };
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ stringpool_t stringpool_contents =
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
#ವ್ಯಾಖ್ಯಾನಿಸು stringpool ((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) &stringpool_contents)
ಸ್ಥಿರಾಂಕ ರಚನೆ keyword_tok *
keyword_gperf_lookup (ನೋಂದಣಿ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str, ನೋಂದಣಿ ಗಾತ್ರ_t len)
{
  ಎಣಿಕೆ
    {
      ಒಟ್ಟು_KEYWORDS = 46,
      MIN_ಶಬ್ದ_ಉದ್ದಳತೆ = 3,
      MAX_ಶಬ್ದ_ಉದ್ದಳತೆ = 21,
      MIN_HASH_ಮೌಲ್ಯ = 3,
      MAX_HASH_ಮೌಲ್ಯ = 87
    };

  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ lengthtable[] =
    {
       0,  0,  0,  3,  4,  5,  0,  0,  0,  4,  0, 11, 12, 13,
       4,  5, 11,  7,  0,  4, 10,  6, 17,  0,  9,  5,  0,  7,
      13,  0, 10,  0,  7,  8,  9,  5,  6, 17,  3,  0,  0,  6,
       7, 13,  9, 15,  0,  7,  0, 14, 10,  0, 12,  0,  9, 10,
      21,  7,  0,  0,  0,  0,  7,  0,  0,  0,  0, 12,  0,  0,
       0,  0,  7,  0,  0,  0,  0, 17,  0,  0,  0,  0,  7,  0,
       0,  0,  0,  7
    };
#ಆದರೆ (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#ಸೂಚನೆ GCC diagnostic ತಳ್ಳು
#ಸೂಚನೆ GCC diagnostic ignored "-Wmissing-field-initializers"
#ಆದರೆ_ಅಂತ್ಯ
  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ keyword_tok wordlist[] =
    {
      {-1}, {-1}, {-1},
#ಸಾಲು 51 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str3,                    ಅಡ್ಡಸಾಲು},
#ಸಾಲು 40 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str4,                   ಕೀಲಿಗಳು_2},
#ಸಾಲು 25 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str5,                  ALIAS},
      {-1}, {-1}, {-1},
#ಸಾಲು 56 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str9,                   TYPE},
      {-1},
#ಸಾಲು 68 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str11,            XKB_SYMBOLS},
#ಸಾಲು 64 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str12,           XKB_KEYCODES},
#ಸಾಲು 67 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str13,          XKB_SEMANTICS},
#ಸಾಲು 41 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str14,                   LOGO},
#ಸಾಲು 53 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str15,                  SHAPE},
#ಸಾಲು 38 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str16,            KEYPAD_ಕೀಲಿಗಳು},
#ಸಾಲು 50 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str17,                REPLACE},
      {-1},
#ಸಾಲು 55 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str19,                   ಪಠ್ಯ},
#ಸಾಲು 65 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str20,             XKB_KEYMAP},
#ಸಾಲು 45 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str21,                 MODIFIER_MAP},
#ಸಾಲು 26 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str22,      ALPHANUMERIC_ಕೀಲಿಗಳು},
      {-1},
#ಸಾಲು 69 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str24,              XKB_TYPES},
#ಸಾಲು 33 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str25,                  ಗುಂಪು_2},
      {-1},
#ಸಾಲು 29 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str27,                AUGMENT},
#ಸಾಲು 32 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str28,          FUNCTION_ಕೀಲಿಗಳು},
      {-1},
#ಸಾಲು 31 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str30,             DEPRECATED},
      {-1},
#ಸಾಲು 35 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str32,                INCLUDE},
#ಸಾಲು 48 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str33,               OVERRIDE},
#ಸಾಲು 28 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str34,              ALTERNATE},
#ಸಾಲು 54 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str35,                  SOLID},
#ಸಾಲು 34 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str36,                 HIDDEN},
#ಸಾಲು 57 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str37,      VIRTUAL_MODS},
#ಸಾಲು 39 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str38,                    ಕೀಲಿ_3},
      {-1}, {-1},
#ಸಾಲು 24 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str41,                 ACTION_TOK},
#ಸಾಲು 30 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str42,                DEFAULT},
#ಸಾಲು 42 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str43,          MODIFIER_ಕೀಲಿಗಳು},
#ಸಾಲು 37 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str44,              INTERPRET},
#ಸಾಲು 27 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str45,        ALTERNATE_ಗುಂಪು},
      {-1},
#ಸಾಲು 44 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str47,                MODIFIER_MAP},
      {-1},
#ಸಾಲು 61 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str49,         XKB_COMPATMAP},
#ಸಾಲು 62 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str50,             XKB_COMPATMAP},
      {-1},
#ಸಾಲು 43 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str52,           MODIFIER_MAP},
      {-1},
#ಸಾಲು 36 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str54,              INDICATOR},
#ಸಾಲು 66 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str55,             XKB_LAYOUT},
#ಸಾಲು 59 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str56,  XKB_COMPATMAP},
#ಸಾಲು 58 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str57,                VIRTUAL},
      {-1}, {-1}, {-1}, {-1},
#ಸಾಲು 46 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str62,                OUTLINE},
      {-1}, {-1}, {-1}, {-1},
#ಸಾಲು 63 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str67,           XKB_GEOMETRY},
      {-1}, {-1}, {-1}, {-1},
#ಸಾಲು 47 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str72,                OVERLAY},
      {-1}, {-1}, {-1}, {-1},
#ಸಾಲು 60 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str77,      XKB_COMPATMAP},
      {-1}, {-1}, {-1}, {-1},
#ಸಾಲು 52 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str82,                SECTION},
      {-1}, {-1}, {-1}, {-1},
#ಸಾಲು 49 "src/xkbcomp/keywords.gperf"
      {(ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ_t)&((ರಚನೆ stringpool_t *)0)->stringpool_str87,                PARTIAL}
    };
#ಆದರೆ (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#ಸೂಚನೆ GCC diagnostic ತೆಗೆ
#ಆದರೆ_ಅಂತ್ಯ

  ಆದರೆ (len <= MAX_ಶಬ್ದ_ಉದ್ದಳತೆ && len >= MIN_ಶಬ್ದ_ಉದ್ದಳತೆ)
    {
      ನೋಂದಣಿ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಕೀಲಿ = keyword_gperf_hash (str, len);

      ಆದರೆ (ಕೀಲಿ <= MAX_HASH_ಮೌಲ್ಯ)
        ಆದರೆ (len == lengthtable[ಕೀಲಿ])
          {
            ನೋಂದಣಿ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s = wordlist[ಕೀಲಿ].name + stringpool;

            ಆದರೆ ((((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)*str ^ (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)*s) & ~32) == 0 && !gperf_case_memcmp (str, s, len))
              ಹಿಂತಿರುಗಿ &wordlist[ಕೀಲಿ];
          }
    }
  ಹಿಂತಿರುಗಿ (ರಚನೆ keyword_tok *) 0;
}
#ಸಾಲು 70 "src/xkbcomp/keywords.gperf"


ಪೂರ್ಣಾಂಕ
keyword_ಗಮ್ಯ_token(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string, ಗಾತ್ರ_t len)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ keyword_tok *kt = keyword_gperf_lookup(string, len);
    ಆದರೆ (!kt)
        ಹಿಂತಿರುಗಿ -1;
    ಹಿಂತಿರುಗಿ kt->tok;
}
// NOLINTEND
