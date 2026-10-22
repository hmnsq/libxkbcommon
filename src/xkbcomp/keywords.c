// NOLINTBEGIN
/* ANSI-C code produced by gperf version 3.3 */
/* Command-line: gperf src/xkbcomp/keywords.gperf  */
/* Computed positions: -k'5,$' */

#अगर !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
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
#त्रुटि "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#पूर्ण_अगर

#पंक्ति 1 "src/xkbcomp/keywords.gperf"

#समावेश "config.h"

#समावेश "xkbcomp-priv.h"
#समावेश "parser-priv.h"

अटल स्थिर काष्ठा keyword_tok *
keyword_gperf_lookup (रेजिस्टर स्थिर अक्षर *str, रेजिस्टर माप_प्रकार len);
#पंक्ति 11 "src/xkbcomp/keywords.gperf"
काष्ठा keyword_tok { पूर्णांक name; पूर्णांक tok; };
#समावेश <string.h>
/* maximum key range = 85, duplicates = 0 */

#अगर_अघोषित GPERF_DOWNCASE
#घोषणा GPERF_DOWNCASE 1
अटल स्थिर अचिन्हित अक्षर gperf_downcase[256] =
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
#पूर्ण_अगर

#अगर_अघोषित GPERF_CASE_MEMCMP
#घोषणा GPERF_CASE_MEMCMP 1
अटल पूर्णांक
gperf_case_memcmp (रेजिस्टर स्थिर अक्षर *s1, रेजिस्टर स्थिर अक्षर *s2, रेजिस्टर माप_प्रकार n)
{
  क्रम (; n > 0;)
    {
      अचिन्हित अक्षर c1 = gperf_downcase[(अचिन्हित अक्षर)*s1++];
      अचिन्हित अक्षर c2 = gperf_downcase[(अचिन्हित अक्षर)*s2++];
      अगर (c1 == c2)
        {
          n--;
          जारी;
        }
      वापस (पूर्णांक)c1 - (पूर्णांक)c2;
    }
  वापस 0;
}
#पूर्ण_अगर

#अगर_घोषित __GNUC__
__inline
#अन्यथा
#अगर_घोषित __cplusplus
अंतरभूत
#पूर्ण_अगर
#पूर्ण_अगर
अटल अचिन्हित पूर्णांक
keyword_gperf_hash (रेजिस्टर स्थिर अक्षर *str, रेजिस्टर माप_प्रकार len)
{
  अटल स्थिर अचिन्हित अक्षर asso_मूल्यावली[] =
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
  रेजिस्टर अचिन्हित पूर्णांक hval = len;

  चयन (hval)
    {
      शेष:
        hval += asso_मूल्यावली[(अचिन्हित अक्षर)str[4]];
#अगर (defined __cplusplus && (__cplusplus >= 201703L || (__cplusplus >= 201103L && defined __clang__ && __clang_major__ + (__clang_minor__ >= 9) > 3))) || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 202000L && ((defined __GNUC__ && __GNUC__ >= 10) || (defined __clang__ && __clang_major__ >= 9)))
      [[fallthrough]];
#या_अगर (defined __GNUC__ && __GNUC__ >= 7) || (defined __clang__ && __clang_major__ >= 10)
      __attribute__ ((__fallthrough__));
#पूर्ण_अगर
      /*FALLTHROUGH*/
      हाल 4:
      हाल 3:
        अवरोध;
    }
  वापस hval + asso_मूल्यावली[(अचिन्हित अक्षर)str[len - 1]];
}

काष्ठा stringpool_t
  {
    अक्षर stringpool_str3[माप("row")];
    अक्षर stringpool_str4[माप("keys")];
    अक्षर stringpool_str5[माप("alias")];
    अक्षर stringpool_str9[माप("type")];
    अक्षर stringpool_str11[माप("xkb_symbols")];
    अक्षर stringpool_str12[माप("xkb_keycodes")];
    अक्षर stringpool_str13[माप("xkb_semantics")];
    अक्षर stringpool_str14[माप("logo")];
    अक्षर stringpool_str15[माप("shape")];
    अक्षर stringpool_str16[माप("keypad_keys")];
    अक्षर stringpool_str17[माप("replace")];
    अक्षर stringpool_str19[माप("text")];
    अक्षर stringpool_str20[माप("xkb_keymap")];
    अक्षर stringpool_str21[माप("modmap")];
    अक्षर stringpool_str22[माप("alphanumeric_keys")];
    अक्षर stringpool_str24[माप("xkb_types")];
    अक्षर stringpool_str25[माप("group")];
    अक्षर stringpool_str27[माप("augment")];
    अक्षर stringpool_str28[माप("function_keys")];
    अक्षर stringpool_str30[माप("deprecated")];
    अक्षर stringpool_str32[माप("include")];
    अक्षर stringpool_str33[माप("override")];
    अक्षर stringpool_str34[माप("alternate")];
    अक्षर stringpool_str35[माप("solid")];
    अक्षर stringpool_str36[माप("hidden")];
    अक्षर stringpool_str37[माप("virtual_modifiers")];
    अक्षर stringpool_str38[माप("key")];
    अक्षर stringpool_str41[माप("action")];
    अक्षर stringpool_str42[माप("default")];
    अक्षर stringpool_str43[माप("modifier_keys")];
    अक्षर stringpool_str44[माप("interpret")];
    अक्षर stringpool_str45[माप("alternate_group")];
    अक्षर stringpool_str47[माप("mod_map")];
    अक्षर stringpool_str49[माप("xkb_compat_map")];
    अक्षर stringpool_str50[माप("xkb_compat")];
    अक्षर stringpool_str52[माप("modifier_map")];
    अक्षर stringpool_str54[माप("indicator")];
    अक्षर stringpool_str55[माप("xkb_layout")];
    अक्षर stringpool_str56[माप("xkb_compatibility_map")];
    अक्षर stringpool_str57[माप("virtual")];
    अक्षर stringpool_str62[माप("outline")];
    अक्षर stringpool_str67[माप("xkb_geometry")];
    अक्षर stringpool_str72[माप("overlay")];
    अक्षर stringpool_str77[माप("xkb_compatibility")];
    अक्षर stringpool_str82[माप("section")];
    अक्षर stringpool_str87[माप("partial")];
  };
अटल स्थिर काष्ठा stringpool_t stringpool_contents =
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
#घोषणा stringpool ((स्थिर अक्षर *) &stringpool_contents)
स्थिर काष्ठा keyword_tok *
keyword_gperf_lookup (रेजिस्टर स्थिर अक्षर *str, रेजिस्टर माप_प्रकार len)
{
  क्रमागत
    {
      योग_KEYWORDS = 46,
      MIN_WORD_लंबाई = 3,
      MAX_WORD_लंबाई = 21,
      MIN_HASH_मूल्य = 3,
      MAX_HASH_मूल्य = 87
    };

  अटल स्थिर अचिन्हित अक्षर lengthtable[] =
    {
       0,  0,  0,  3,  4,  5,  0,  0,  0,  4,  0, 11, 12, 13,
       4,  5, 11,  7,  0,  4, 10,  6, 17,  0,  9,  5,  0,  7,
      13,  0, 10,  0,  7,  8,  9,  5,  6, 17,  3,  0,  0,  6,
       7, 13,  9, 15,  0,  7,  0, 14, 10,  0, 12,  0,  9, 10,
      21,  7,  0,  0,  0,  0,  7,  0,  0,  0,  0, 12,  0,  0,
       0,  0,  7,  0,  0,  0,  0, 17,  0,  0,  0,  0,  7,  0,
       0,  0,  0,  7
    };
#अगर (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#आशय GCC diagnostic धकेलो
#आशय GCC diagnostic ignored "-Wmissing-field-initializers"
#पूर्ण_अगर
  अटल स्थिर काष्ठा keyword_tok wordlist[] =
    {
      {-1}, {-1}, {-1},
#पंक्ति 51 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str3,                    ROW},
#पंक्ति 40 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str4,                   कुंजियाँ_2},
#पंक्ति 25 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str5,                  ALIAS},
      {-1}, {-1}, {-1},
#पंक्ति 56 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str9,                   TYPE},
      {-1},
#पंक्ति 68 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str11,            XKB_SYMBOLS},
#पंक्ति 64 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str12,           XKB_KEYCODES},
#पंक्ति 67 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str13,          XKB_SEMANTICS},
#पंक्ति 41 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str14,                   LOGO},
#पंक्ति 53 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str15,                  SHAPE},
#पंक्ति 38 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str16,            KEYPAD_कुंजियाँ},
#पंक्ति 50 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str17,                REPLACE},
      {-1},
#पंक्ति 55 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str19,                   TEXT},
#पंक्ति 65 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str20,             XKB_KEYMAP},
#पंक्ति 45 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str21,                 MODIFIER_MAP},
#पंक्ति 26 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str22,      ALPHANUMERIC_कुंजियाँ},
      {-1},
#पंक्ति 69 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str24,              XKB_TYPES},
#पंक्ति 33 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str25,                  GROUP},
      {-1},
#पंक्ति 29 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str27,                AUGMENT},
#पंक्ति 32 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str28,          FUNCTION_कुंजियाँ},
      {-1},
#पंक्ति 31 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str30,             DEPRECATED},
      {-1},
#पंक्ति 35 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str32,                INCLUDE},
#पंक्ति 48 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str33,               OVERRIDE},
#पंक्ति 28 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str34,              ALTERNATE},
#पंक्ति 54 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str35,                  SOLID},
#पंक्ति 34 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str36,                 HIDDEN},
#पंक्ति 57 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str37,      VIRTUAL_MODS},
#पंक्ति 39 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str38,                    कुंजी_3},
      {-1}, {-1},
#पंक्ति 24 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str41,                 ACTION_TOK},
#पंक्ति 30 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str42,                DEFAULT},
#पंक्ति 42 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str43,          MODIFIER_कुंजियाँ},
#पंक्ति 37 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str44,              INTERPRET},
#पंक्ति 27 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str45,        ALTERNATE_GROUP},
      {-1},
#पंक्ति 44 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str47,                MODIFIER_MAP},
      {-1},
#पंक्ति 61 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str49,         XKB_COMPATMAP},
#पंक्ति 62 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str50,             XKB_COMPATMAP},
      {-1},
#पंक्ति 43 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str52,           MODIFIER_MAP},
      {-1},
#पंक्ति 36 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str54,              INDICATOR},
#पंक्ति 66 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str55,             XKB_LAYOUT},
#पंक्ति 59 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str56,  XKB_COMPATMAP},
#पंक्ति 58 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str57,                VIRTUAL},
      {-1}, {-1}, {-1}, {-1},
#पंक्ति 46 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str62,                OUTLINE},
      {-1}, {-1}, {-1}, {-1},
#पंक्ति 63 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str67,           XKB_GEOMETRY},
      {-1}, {-1}, {-1}, {-1},
#पंक्ति 47 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str72,                OVERLAY},
      {-1}, {-1}, {-1}, {-1},
#पंक्ति 60 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str77,      XKB_COMPATMAP},
      {-1}, {-1}, {-1}, {-1},
#पंक्ति 52 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str82,                SECTION},
      {-1}, {-1}, {-1}, {-1},
#पंक्ति 49 "src/xkbcomp/keywords.gperf"
      {(पूर्णांक)(माप_प्रकार)&((काष्ठा stringpool_t *)0)->stringpool_str87,                PARTIAL}
    };
#अगर (defined __GNUC__ && __GNUC__ + (__GNUC_MINOR__ >= 6) > 4) || (defined __clang__ && __clang_major__ >= 3)
#आशय GCC diagnostic निकालो
#पूर्ण_अगर

  अगर (len <= MAX_WORD_लंबाई && len >= MIN_WORD_लंबाई)
    {
      रेजिस्टर अचिन्हित पूर्णांक कुंजी = keyword_gperf_hash (str, len);

      अगर (कुंजी <= MAX_HASH_मूल्य)
        अगर (len == lengthtable[कुंजी])
          {
            रेजिस्टर स्थिर अक्षर *s = wordlist[कुंजी].name + stringpool;

            अगर ((((अचिन्हित अक्षर)*str ^ (अचिन्हित अक्षर)*s) & ~32) == 0 && !gperf_case_memcmp (str, s, len))
              वापस &wordlist[कुंजी];
          }
    }
  वापस (काष्ठा keyword_tok *) 0;
}
#पंक्ति 70 "src/xkbcomp/keywords.gperf"


पूर्णांक
keyword_to_token(स्थिर अक्षर *string, माप_प्रकार len)
{
    स्थिर काष्ठा keyword_tok *kt = keyword_gperf_lookup(string, len);
    अगर (!kt)
        वापस -1;
    वापस kt->tok;
}
// NOLINTEND
