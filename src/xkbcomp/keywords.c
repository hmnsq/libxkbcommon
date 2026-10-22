// NOLINTBEGIN
/* ANSI-C code produced by gperf version 3.3 */
/* Command-line: gperf src/xkbcomp/keywords.gperf  */
/* Computed positions: -k'5,$' */

#ਜੇ !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
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
#ਗਲਤੀ "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#ਜੇ_ਅੰਤ

#ਸਤਰ 1 "src/xkbcomp/keywords.gperf"

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp-priv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parser-priv.h"

ਸਥਿਰ ਅਚਲ ਬਣਤਰ keyword_tok *
keyword_gperf_lookup (ਰਜਿਸਟਰ ਅਚਲ ਅੱਖਰ *str, ਰਜਿਸਟਰ ਮਾਪ_t len);
#ਸਤਰ 11 "src/xkbcomp/keywords.gperf"
ਬਣਤਰ keyword_tok { ਪੂਰਨ name; ਪੂਰਨ tok; };
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
/* maximum key range = 85, duplicates = 0 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GPERF_DOWNCASE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GPERF_DOWNCASE 1
ਸਥਿਰ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ gperf_downcase[256] =
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
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GPERF_CASE_MEMCMP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GPERF_CASE_MEMCMP 1
ਸਥਿਰ ਪੂਰਨ
gperf_case_memcmp (ਰਜਿਸਟਰ ਅਚਲ ਅੱਖਰ *s1, ਰਜਿਸਟਰ ਅਚਲ ਅੱਖਰ *s2, ਰਜਿਸਟਰ ਮਾਪ_t n)
{
  ਲਈ (; n > 0;)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c1 = gperf_downcase[(ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)*s1++];
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c2 = gperf_downcase[(ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)*s2++];
      ਜੇ (c1 == c2)
        {
          n--;
          ਜਾਰੀ;
        }
      ਵਾਪਸ (ਪੂਰਨ)c1 - (ਪੂਰਨ)c2;
    }
  ਵਾਪਸ 0;
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __GNUC__
__inline
#ਨਹੀਂਤਾਂ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
ਸਤਰਅੰਦਰ
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
keyword_gperf_hash (ਰਜਿਸਟਰ ਅਚਲ ਅੱਖਰ *str, ਰਜਿਸਟਰ ਮਾਪ_t len)
{
  ਸਥਿਰ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ asso_ਮੁੱਲਾਂ[] =
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
  ਰਜਿਸਟਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ hval = len;

  ਚੋਣ (hval)
    {
      ਮੂਲ:
        hval += asso_ਮੁੱਲਾਂ[(ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)str[4]];
#ਜੇ (defined __cplusplus && (__cplusplus >= 201703L || (__cplusplus >= 201103L && defined __clang__ && __clang_major__ + (__clang_minor__ >= 9) > 3))) || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 202000L && ((defined __GNUC__ && __GNUC__ >= 10) || (defined __clang__ && __clang_major__ >= 9)))
      [[fallthrough]];
#ਜਾਂਜੇ (defined __GNUC__ && __GNUC__ >= 7) || (defined __clang__ && __clang_major__ >= 10)
      __attribute__ ((__fallthrough__));
#ਜੇ_ਅੰਤ
      /*FALLTHROUGH*/
      ਹਾਲਤ 4:
      ਹਾਲਤ 3:
        ਰੋਕੋ;
    }
  ਵਾਪਸ hval + asso_ਮੁੱਲਾਂ[(ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)str[len - 1]];
}

ਬਣਤਰ stringpool_t
  {
    ਅੱਖਰ stringpool_str3[ਆਕਾਰ("row")];
    ਅੱਖਰ stringpool_str4[ਆਕਾਰ("keys")];
    ਅੱਖਰ stringpool_str5[ਆਕਾਰ("alias")];
    ਅੱਖਰ stringpool_str9[ਆਕਾਰ("type")];
    ਅੱਖਰ stringpool_str11[ਆਕਾਰ("xkb_symbols")];
    ਅੱਖਰ stringpool_str12[ਆਕਾਰ("xkb_keycodes")];
    ਅੱਖਰ stringpool_str13[ਆਕਾਰ("xkb_semantics")];
    ਅੱਖਰ stringpool_str14[ਆਕਾਰ("logo")];
    ਅੱਖਰ stringpool_str15[ਆਕਾਰ("shape")];
    ਅੱਖਰ stringpool_str16[ਆਕਾਰ("keypad_keys")];
    ਅੱਖਰ stringpool_str17[ਆਕਾਰ("replace")];
    ਅੱਖਰ stringpool_str19[ਆਕਾਰ("text")];
    ਅੱਖਰ stringpool_str20[ਆਕਾਰ("xkb_keymap")];
    ਅੱਖਰ stringpool_str21[ਆਕਾਰ("modmap")];
    ਅੱਖਰ stringpool_str22[ਆਕਾਰ("alphanumeric_keys")];
    ਅੱਖਰ stringpool_str24[ਆਕਾਰ("xkb_types")];
    ਅੱਖਰ stringpool_str25[ਆਕਾਰ("group")];
    ਅੱਖਰ stringpool_str27[ਆਕਾਰ("augment")];
    ਅੱਖਰ stringpool_str28[ਆਕਾਰ("function_keys")];
    ਅੱਖਰ stringpool_str30[ਆਕਾਰ("deprecated")];
    ਅੱਖਰ stringpool_str32[ਆਕਾਰ("include")];
    ਅੱਖਰ stringpool_str33[ਆਕਾਰ("override")];
    ਅੱਖਰ stringpool_str34[ਆਕਾਰ("alternate")];
    ਅੱਖਰ stringpool_str35[ਆਕਾਰ("solid")];
    ਅੱਖਰ stringpool_str36[ਆਕਾਰ("hidden")];
    ਅੱਖਰ stringpool_str37[ਆਕਾਰ("virtual_modifiers")];
    ਅੱਖਰ stringpool_str38[ਆਕਾਰ("key")];
    ਅੱਖਰ stringpool_str41[ਆਕਾਰ("action")];
    ਅੱਖਰ stringpool_str42[ਆਕਾਰ("default")];
    ਅੱਖਰ stringpool_str43[ਆਕਾਰ("modifier_keys")];
    ਅੱਖਰ stringpool_str44[ਆਕਾਰ("interpret")];
    ਅੱਖਰ stringpool_str45[ਆਕਾਰ("alternate_group")];
    ਅੱਖਰ stringpool_str47[ਆਕਾਰ("mod_map")];
    ਅੱਖਰ stringpool_str49[ਆਕਾਰ("xkb_compat_map")];
    ਅੱਖਰ stringpool_str50[ਆਕਾਰ("xkb_compat")];
    ਅੱਖਰ stringpool_str52[ਆਕਾਰ("modifier_map")];
    ਅੱਖਰ stringpool_str54[ਆਕਾਰ("indicator")];
    ਅੱਖਰ stringpool_str55[ਆਕਾਰ("xkb_layout")];
    ਅੱਖਰ stringpool_str56[ਆਕਾਰ("xkb_compatibility_map")];
    ਅੱਖਰ stringpool_str57[ਆਕਾਰ("virtual")];
    ਅੱਖਰ stringpool_str62[ਆਕਾਰ("outline")];
    ਅੱਖਰ stringpool_str67[ਆਕਾਰ("xkb_geometry")];
    ਅੱਖਰ stringpool_str72[ਆਕਾਰ("overlay")];
    ਅੱਖਰ stringpool_str77[ਆਕਾਰ("xkb_compatibility")];
    ਅੱਖਰ stringpool_str82[ਆਕਾਰ("section")];
    ਅੱਖਰ stringpool_str87[ਆਕਾਰ("partial")];
  };
ਸਥਿਰ ਅਚਲ ਬਣਤਰ stringpool_t stringpool_contents =
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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ stringpool ((ਅਚਲ ਅੱਖਰ *) &stringpool_contents)
ਅਚਲ ਬਣਤਰ keyword_tok *
keyword_gperf_lookup (ਰਜਿਸਟਰ ਅਚਲ ਅੱਖਰ *str, ਰਜਿਸਟਰ ਮਾਪ_t len)
{
  ਗਿਣਤੀ
    {
      ਕੁੱਲ_KEYWORDS = 46,
      MIN_ਸ਼ਬਦ_ਲੰਮਾਈ = 3,
      MAX_ਸ਼ਬਦ_ਲੰਮਾਈ = 21,
      MIN_HASH_ਮੁੱਲ = 3,
      MAX_HASH_ਮੁੱਲ = 87
    };

  ਸਥਿਰ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ lengthtable[] =
    {
       0,  0,  0,  3,  4,  5,  0,  0,  0,  4,  0, 11, 12, 13,
       4,  5, 11,  7,  0,  4, 10,  6, 17,  0,  9,  5,  0,  7,
      13,  0, 10,  0,  7,  8,  9,  5,  6, 17,  3,  0,  0,  6,
       7, 13,  9, 15,  0,  7,  0, 14, 10,  0, 12,  0,  9, 10,
      21,  7,  0,  0,  0,  0,  7,  0,  0,  0,  0, 12,  0,  0,
       0,  0,  7,  0,  0,  0,  0, 17,  0,  0,  0,  0,  7,  0,
       0,  0,  0,  7
    };
#ਜੇ (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#ਹਦਾਇਤ GCC diagnostic ਧੱਕੋ
#ਹਦਾਇਤ GCC diagnostic ignored "-Wmissing-field-initializers"
#ਜੇ_ਅੰਤ
  ਸਥਿਰ ਅਚਲ ਬਣਤਰ keyword_tok wordlist[] =
    {
      {-1}, {-1}, {-1},
#ਸਤਰ 51 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str3,                    ਪੰਕਤੀ},
#ਸਤਰ 40 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str4,                   ਕੁੰਜੀਆਂ_2},
#ਸਤਰ 25 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str5,                  ALIAS},
      {-1}, {-1}, {-1},
#ਸਤਰ 56 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str9,                   TYPE},
      {-1},
#ਸਤਰ 68 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str11,            XKB_SYMBOLS},
#ਸਤਰ 64 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str12,           XKB_KEYCODES},
#ਸਤਰ 67 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str13,          XKB_SEMANTICS},
#ਸਤਰ 41 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str14,                   LOGO},
#ਸਤਰ 53 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str15,                  SHAPE},
#ਸਤਰ 38 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str16,            KEYPAD_ਕੁੰਜੀਆਂ},
#ਸਤਰ 50 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str17,                REPLACE},
      {-1},
#ਸਤਰ 55 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str19,                   ਪਾਠ},
#ਸਤਰ 65 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str20,             XKB_KEYMAP},
#ਸਤਰ 45 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str21,                 MODIFIER_MAP},
#ਸਤਰ 26 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str22,      ALPHANUMERIC_ਕੁੰਜੀਆਂ},
      {-1},
#ਸਤਰ 69 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str24,              XKB_TYPES},
#ਸਤਰ 33 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str25,                  ਸਮੂਹ_2},
      {-1},
#ਸਤਰ 29 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str27,                AUGMENT},
#ਸਤਰ 32 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str28,          FUNCTION_ਕੁੰਜੀਆਂ},
      {-1},
#ਸਤਰ 31 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str30,             DEPRECATED},
      {-1},
#ਸਤਰ 35 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str32,                INCLUDE},
#ਸਤਰ 48 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str33,               OVERRIDE},
#ਸਤਰ 28 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str34,              ALTERNATE},
#ਸਤਰ 54 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str35,                  SOLID},
#ਸਤਰ 34 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str36,                 HIDDEN},
#ਸਤਰ 57 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str37,      VIRTUAL_MODS},
#ਸਤਰ 39 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str38,                    ਕੁੰਜੀ_3},
      {-1}, {-1},
#ਸਤਰ 24 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str41,                 ACTION_TOK},
#ਸਤਰ 30 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str42,                DEFAULT},
#ਸਤਰ 42 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str43,          MODIFIER_ਕੁੰਜੀਆਂ},
#ਸਤਰ 37 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str44,              INTERPRET},
#ਸਤਰ 27 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str45,        ALTERNATE_ਸਮੂਹ},
      {-1},
#ਸਤਰ 44 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str47,                MODIFIER_MAP},
      {-1},
#ਸਤਰ 61 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str49,         XKB_COMPATMAP},
#ਸਤਰ 62 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str50,             XKB_COMPATMAP},
      {-1},
#ਸਤਰ 43 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str52,           MODIFIER_MAP},
      {-1},
#ਸਤਰ 36 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str54,              INDICATOR},
#ਸਤਰ 66 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str55,             XKB_LAYOUT},
#ਸਤਰ 59 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str56,  XKB_COMPATMAP},
#ਸਤਰ 58 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str57,                VIRTUAL},
      {-1}, {-1}, {-1}, {-1},
#ਸਤਰ 46 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str62,                OUTLINE},
      {-1}, {-1}, {-1}, {-1},
#ਸਤਰ 63 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str67,           XKB_GEOMETRY},
      {-1}, {-1}, {-1}, {-1},
#ਸਤਰ 47 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str72,                OVERLAY},
      {-1}, {-1}, {-1}, {-1},
#ਸਤਰ 60 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str77,      XKB_COMPATMAP},
      {-1}, {-1}, {-1}, {-1},
#ਸਤਰ 52 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str82,                SECTION},
      {-1}, {-1}, {-1}, {-1},
#ਸਤਰ 49 "src/xkbcomp/keywords.gperf"
      {(ਪੂਰਨ)(ਮਾਪ_t)&((ਬਣਤਰ stringpool_t *)0)->stringpool_str87,                PARTIAL}
    };
#ਜੇ (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#ਹਦਾਇਤ GCC diagnostic ਕੱਢੋ
#ਜੇ_ਅੰਤ

  ਜੇ (len <= MAX_ਸ਼ਬਦ_ਲੰਮਾਈ && len >= MIN_ਸ਼ਬਦ_ਲੰਮਾਈ)
    {
      ਰਜਿਸਟਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਕੁੰਜੀ = keyword_gperf_hash (str, len);

      ਜੇ (ਕੁੰਜੀ <= MAX_HASH_ਮੁੱਲ)
        ਜੇ (len == lengthtable[ਕੁੰਜੀ])
          {
            ਰਜਿਸਟਰ ਅਚਲ ਅੱਖਰ *s = wordlist[ਕੁੰਜੀ].name + stringpool;

            ਜੇ ((((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)*str ^ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)*s) & ~32) == 0 && !gperf_case_memcmp (str, s, len))
              ਵਾਪਸ &wordlist[ਕੁੰਜੀ];
          }
    }
  ਵਾਪਸ (ਬਣਤਰ keyword_tok *) 0;
}
#ਸਤਰ 70 "src/xkbcomp/keywords.gperf"


ਪੂਰਨ
keyword_ਮੰਜ਼ਿਲ_token(ਅਚਲ ਅੱਖਰ *string, ਮਾਪ_t len)
{
    ਅਚਲ ਬਣਤਰ keyword_tok *kt = keyword_gperf_lookup(string, len);
    ਜੇ (!kt)
        ਵਾਪਸ -1;
    ਵਾਪਸ kt->tok;
}
// NOLINTEND
