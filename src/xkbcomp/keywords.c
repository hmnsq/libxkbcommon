// NOLINTBEGIN
/* ANSI-C code produced by gperf version 3.3 */
/* Command-line: gperf src/xkbcomp/keywords.gperf  */
/* Computed positions: -k'5,$' */

#ఒకవేళ !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
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
#దోషం "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#అయితే_ముగింపు

#పంక్తి 1 "src/xkbcomp/keywords.gperf"

#అన్తర్భావ "config.h"

#అన్తర్భావ "xkbcomp-priv.h"
#అన్తర్భావ "parser-priv.h"

స్థిర స్థిరరాశి నిర్మాణం keyword_tok *
keyword_gperf_lookup (నమోదు స్థిరరాశి అక్షరం *str, నమోదు కొలత_t len);
#పంక్తి 11 "src/xkbcomp/keywords.gperf"
నిర్మాణం keyword_tok { పూర్ణాంకం name; పూర్ణాంకం tok; };
#అన్తర్భావ <string.h>
/* maximum key range = 85, duplicates = 0 */

#నిర్వచితం_కాకపోతే GPERF_DOWNCASE
#నిర్వచించు GPERF_DOWNCASE 1
స్థిర స్థిరరాశి గుర్తులేని అక్షరం gperf_downcase[256] =
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
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే GPERF_CASE_MEMCMP
#నిర్వచించు GPERF_CASE_MEMCMP 1
స్థిర పూర్ణాంకం
gperf_case_memcmp (నమోదు స్థిరరాశి అక్షరం *s1, నమోదు స్థిరరాశి అక్షరం *s2, నమోదు కొలత_t n)
{
  కొరకు (; n > 0;)
    {
      గుర్తులేని అక్షరం c1 = gperf_downcase[(గుర్తులేని అక్షరం)*s1++];
      గుర్తులేని అక్షరం c2 = gperf_downcase[(గుర్తులేని అక్షరం)*s2++];
      ఒకవేళ (c1 == c2)
        {
          n--;
          కొనసాగు;
        }
      తిరిగి (పూర్ణాంకం)c1 - (పూర్ణాంకం)c2;
    }
  తిరిగి 0;
}
#అయితే_ముగింపు

#నిర్వచితమైతే __GNUC__
__inline
#లేకపోతే
#నిర్వచితమైతే __cplusplus
అంతఃవరుస
#అయితే_ముగింపు
#అయితే_ముగింపు
స్థిర గుర్తులేని పూర్ణాంకం
keyword_gperf_hash (నమోదు స్థిరరాశి అక్షరం *str, నమోదు కొలత_t len)
{
  స్థిర స్థిరరాశి గుర్తులేని అక్షరం asso_విలువలు[] =
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
  నమోదు గుర్తులేని పూర్ణాంకం hval = len;

  ఎంపిక (hval)
    {
      అప్రమేయ:
        hval += asso_విలువలు[(గుర్తులేని అక్షరం)str[4]];
#ఒకవేళ (defined __cplusplus && (__cplusplus >= 201703L || (__cplusplus >= 201103L && defined __clang__ && __clang_major__ + (__clang_minor__ >= 9) > 3))) || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 202000L && ((defined __GNUC__ && __GNUC__ >= 10) || (defined __clang__ && __clang_major__ >= 9)))
      [[fallthrough]];
#లేదావేళ (defined __GNUC__ && __GNUC__ >= 7) || (defined __clang__ && __clang_major__ >= 10)
      __attribute__ ((__fallthrough__));
#అయితే_ముగింపు
      /*FALLTHROUGH*/
      స్థితి 4:
      స్థితి 3:
        ఆపు;
    }
  తిరిగి hval + asso_విలువలు[(గుర్తులేని అక్షరం)str[len - 1]];
}

నిర్మాణం stringpool_t
  {
    అక్షరం stringpool_str3[పరిమాణం("row")];
    అక్షరం stringpool_str4[పరిమాణం("keys")];
    అక్షరం stringpool_str5[పరిమాణం("alias")];
    అక్షరం stringpool_str9[పరిమాణం("type")];
    అక్షరం stringpool_str11[పరిమాణం("xkb_symbols")];
    అక్షరం stringpool_str12[పరిమాణం("xkb_keycodes")];
    అక్షరం stringpool_str13[పరిమాణం("xkb_semantics")];
    అక్షరం stringpool_str14[పరిమాణం("logo")];
    అక్షరం stringpool_str15[పరిమాణం("shape")];
    అక్షరం stringpool_str16[పరిమాణం("keypad_keys")];
    అక్షరం stringpool_str17[పరిమాణం("replace")];
    అక్షరం stringpool_str19[పరిమాణం("text")];
    అక్షరం stringpool_str20[పరిమాణం("xkb_keymap")];
    అక్షరం stringpool_str21[పరిమాణం("modmap")];
    అక్షరం stringpool_str22[పరిమాణం("alphanumeric_keys")];
    అక్షరం stringpool_str24[పరిమాణం("xkb_types")];
    అక్షరం stringpool_str25[పరిమాణం("group")];
    అక్షరం stringpool_str27[పరిమాణం("augment")];
    అక్షరం stringpool_str28[పరిమాణం("function_keys")];
    అక్షరం stringpool_str30[పరిమాణం("deprecated")];
    అక్షరం stringpool_str32[పరిమాణం("include")];
    అక్షరం stringpool_str33[పరిమాణం("override")];
    అక్షరం stringpool_str34[పరిమాణం("alternate")];
    అక్షరం stringpool_str35[పరిమాణం("solid")];
    అక్షరం stringpool_str36[పరిమాణం("hidden")];
    అక్షరం stringpool_str37[పరిమాణం("virtual_modifiers")];
    అక్షరం stringpool_str38[పరిమాణం("key")];
    అక్షరం stringpool_str41[పరిమాణం("action")];
    అక్షరం stringpool_str42[పరిమాణం("default")];
    అక్షరం stringpool_str43[పరిమాణం("modifier_keys")];
    అక్షరం stringpool_str44[పరిమాణం("interpret")];
    అక్షరం stringpool_str45[పరిమాణం("alternate_group")];
    అక్షరం stringpool_str47[పరిమాణం("mod_map")];
    అక్షరం stringpool_str49[పరిమాణం("xkb_compat_map")];
    అక్షరం stringpool_str50[పరిమాణం("xkb_compat")];
    అక్షరం stringpool_str52[పరిమాణం("modifier_map")];
    అక్షరం stringpool_str54[పరిమాణం("indicator")];
    అక్షరం stringpool_str55[పరిమాణం("xkb_layout")];
    అక్షరం stringpool_str56[పరిమాణం("xkb_compatibility_map")];
    అక్షరం stringpool_str57[పరిమాణం("virtual")];
    అక్షరం stringpool_str62[పరిమాణం("outline")];
    అక్షరం stringpool_str67[పరిమాణం("xkb_geometry")];
    అక్షరం stringpool_str72[పరిమాణం("overlay")];
    అక్షరం stringpool_str77[పరిమాణం("xkb_compatibility")];
    అక్షరం stringpool_str82[పరిమాణం("section")];
    అక్షరం stringpool_str87[పరిమాణం("partial")];
  };
స్థిర స్థిరరాశి నిర్మాణం stringpool_t stringpool_contents =
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
#నిర్వచించు stringpool ((స్థిరరాశి అక్షరం *) &stringpool_contents)
స్థిరరాశి నిర్మాణం keyword_tok *
keyword_gperf_lookup (నమోదు స్థిరరాశి అక్షరం *str, నమోదు కొలత_t len)
{
  గణన
    {
      మొత్తం_KEYWORDS = 46,
      MIN_మాట_నిడివి = 3,
      MAX_మాట_నిడివి = 21,
      MIN_HASH_విలువ = 3,
      MAX_HASH_విలువ = 87
    };

  స్థిర స్థిరరాశి గుర్తులేని అక్షరం lengthtable[] =
    {
       0,  0,  0,  3,  4,  5,  0,  0,  0,  4,  0, 11, 12, 13,
       4,  5, 11,  7,  0,  4, 10,  6, 17,  0,  9,  5,  0,  7,
      13,  0, 10,  0,  7,  8,  9,  5,  6, 17,  3,  0,  0,  6,
       7, 13,  9, 15,  0,  7,  0, 14, 10,  0, 12,  0,  9, 10,
      21,  7,  0,  0,  0,  0,  7,  0,  0,  0,  0, 12,  0,  0,
       0,  0,  7,  0,  0,  0,  0, 17,  0,  0,  0,  0,  7,  0,
       0,  0,  0,  7
    };
#ఒకవేళ (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#సూచన GCC diagnostic నెట్టు
#సూచన GCC diagnostic ignored "-Wmissing-field-initializers"
#అయితే_ముగింపు
  స్థిర స్థిరరాశి నిర్మాణం keyword_tok wordlist[] =
    {
      {-1}, {-1}, {-1},
#పంక్తి 51 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str3,                    అడ్డువరుస},
#పంక్తి 40 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str4,                   కీలకాలు_2},
#పంక్తి 25 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str5,                  ALIAS},
      {-1}, {-1}, {-1},
#పంక్తి 56 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str9,                   TYPE},
      {-1},
#పంక్తి 68 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str11,            XKB_SYMBOLS},
#పంక్తి 64 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str12,           XKB_KEYCODES},
#పంక్తి 67 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str13,          XKB_SEMANTICS},
#పంక్తి 41 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str14,                   LOGO},
#పంక్తి 53 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str15,                  SHAPE},
#పంక్తి 38 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str16,            KEYPAD_కీలకాలు},
#పంక్తి 50 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str17,                REPLACE},
      {-1},
#పంక్తి 55 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str19,                   పాఠ్యం},
#పంక్తి 65 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str20,             XKB_KEYMAP},
#పంక్తి 45 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str21,                 MODIFIER_MAP},
#పంక్తి 26 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str22,      ALPHANUMERIC_కీలకాలు},
      {-1},
#పంక్తి 69 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str24,              XKB_TYPES},
#పంక్తి 33 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str25,                  సమూహం_2},
      {-1},
#పంక్తి 29 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str27,                AUGMENT},
#పంక్తి 32 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str28,          FUNCTION_కీలకాలు},
      {-1},
#పంక్తి 31 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str30,             DEPRECATED},
      {-1},
#పంక్తి 35 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str32,                INCLUDE},
#పంక్తి 48 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str33,               OVERRIDE},
#పంక్తి 28 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str34,              ALTERNATE},
#పంక్తి 54 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str35,                  SOLID},
#పంక్తి 34 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str36,                 HIDDEN},
#పంక్తి 57 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str37,      VIRTUAL_MODS},
#పంక్తి 39 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str38,                    కీలకం_3},
      {-1}, {-1},
#పంక్తి 24 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str41,                 ACTION_TOK},
#పంక్తి 30 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str42,                DEFAULT},
#పంక్తి 42 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str43,          MODIFIER_కీలకాలు},
#పంక్తి 37 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str44,              INTERPRET},
#పంక్తి 27 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str45,        ALTERNATE_సమూహం},
      {-1},
#పంక్తి 44 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str47,                MODIFIER_MAP},
      {-1},
#పంక్తి 61 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str49,         XKB_COMPATMAP},
#పంక్తి 62 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str50,             XKB_COMPATMAP},
      {-1},
#పంక్తి 43 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str52,           MODIFIER_MAP},
      {-1},
#పంక్తి 36 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str54,              INDICATOR},
#పంక్తి 66 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str55,             XKB_LAYOUT},
#పంక్తి 59 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str56,  XKB_COMPATMAP},
#పంక్తి 58 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str57,                VIRTUAL},
      {-1}, {-1}, {-1}, {-1},
#పంక్తి 46 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str62,                OUTLINE},
      {-1}, {-1}, {-1}, {-1},
#పంక్తి 63 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str67,           XKB_GEOMETRY},
      {-1}, {-1}, {-1}, {-1},
#పంక్తి 47 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str72,                OVERLAY},
      {-1}, {-1}, {-1}, {-1},
#పంక్తి 60 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str77,      XKB_COMPATMAP},
      {-1}, {-1}, {-1}, {-1},
#పంక్తి 52 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str82,                SECTION},
      {-1}, {-1}, {-1}, {-1},
#పంక్తి 49 "src/xkbcomp/keywords.gperf"
      {(పూర్ణాంకం)(కొలత_t)&((నిర్మాణం stringpool_t *)0)->stringpool_str87,                PARTIAL}
    };
#ఒకవేళ (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#సూచన GCC diagnostic తీయి
#అయితే_ముగింపు

  ఒకవేళ (len <= MAX_మాట_నిడివి && len >= MIN_మాట_నిడివి)
    {
      నమోదు గుర్తులేని పూర్ణాంకం కీలకం = keyword_gperf_hash (str, len);

      ఒకవేళ (కీలకం <= MAX_HASH_విలువ)
        ఒకవేళ (len == lengthtable[కీలకం])
          {
            నమోదు స్థిరరాశి అక్షరం *s = wordlist[కీలకం].name + stringpool;

            ఒకవేళ ((((గుర్తులేని అక్షరం)*str ^ (గుర్తులేని అక్షరం)*s) & ~32) == 0 && !gperf_case_memcmp (str, s, len))
              తిరిగి &wordlist[కీలకం];
          }
    }
  తిరిగి (నిర్మాణం keyword_tok *) 0;
}
#పంక్తి 70 "src/xkbcomp/keywords.gperf"


పూర్ణాంకం
keyword_గమ్యం_token(స్థిరరాశి అక్షరం *string, కొలత_t len)
{
    స్థిరరాశి నిర్మాణం keyword_tok *kt = keyword_gperf_lookup(string, len);
    ఒకవేళ (!kt)
        తిరిగి -1;
    తిరిగి kt->tok;
}
// NOLINTEND
