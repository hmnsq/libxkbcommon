/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

/*
 * The parser should work with reasonably recent versions of bison.
 *
 * byacc support was dropped, because it does not support the following features:
 * - `%define parse.error detailed`
 */

/* The following goes in the header */
%code requires {
#अन्तर्भाव "config.h"

#अन्तर्भाव "scanner-utils.h"
#अन्तर्भाव "xkbcomp/ast.h"
#अन्तर्भाव "xkbcomp/xkbcomp-priv.h"
}

%{
#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>

#अन्तर्भाव "keysym.h"
#अन्तर्भाव "xkbcomp/xkbcomp-priv.h"
#अन्तर्भाव "xkbcomp/parser-priv.h"
#अन्तर्भाव "xkbcomp/ast-build.h"

संरचना parser_param {
    संरचना xkb_context *ctx;
    संरचना scanner *scanner;
    XkbFile *rtrn;
    संरचना parser_keymap_config config;
    bool अधिकम्_maps;
};

#परिभाषय parser_log_with_संहिता(param, level, verbosity, log_सन्देशः_id, fmt, ...)   \
    scanner_log_with_संहिता((param)->scanner, level, verbosity, log_सन्देशः_id, fmt,\
                          ##__VA_ARGS__)

#परिभाषय parser_err(param, error_id, fmt, ...) \
    scanner_err((param)->scanner, error_id, fmt, ##__VA_ARGS__)

#परिभाषय parser_warn(param, warning_id, fmt, ...) \
    scanner_warn((param)->scanner, warning_id, fmt, ##__VA_ARGS__)

#परिभाषय parser_vrb(param, verbosity, warning_id, fmt, ...) \
    scanner_vrb((param)->scanner, verbosity, warning_id, fmt, ##__VA_ARGS__)

स्थिर शून्य
_xkbcommon_error(संरचना parser_param *param, ध्रुव वर्ण *सन्देशः)
{
    parser_err(param, XKB_ERROR_INVALID_XKB_SYNTAX, "%s", सन्देशः);
}

स्थिर bool
resolve_keysym(संरचना parser_param *param, संरचना sval name, xkb_keysym_t *sym_rtrn)
{
    xkb_keysym_t sym;

    यदि (isvaleq(name, SVAL_LIT("any")) || isvaleq(name, SVAL_LIT("nosymbol"))) {
        *sym_rtrn = XKB_कुञ्चिका_No_चिह्नम्;
        प्रत्यागम true;
    }

    यदि (isvaleq(name, SVAL_LIT("none")) || isvaleq(name, SVAL_LIT("voidsymbol"))) {
        *sym_rtrn = XKB_कुञ्चिका_Void_चिह्नम्;
        प्रत्यागम true;
    }

    /* xkb_keysym_from_name needs a C string. */
    वर्ण buf[XKB_KEYSYM_NAME_MAX_SIZE];
    यदि (name.len >= परिमाण(buf)) {
        प्रत्यागम false;
    }
    स्मृतिं_प्रतिलिख(buf, name.आरम्भः, name.len);
    buf[name.len] = '\0';

    sym = xkb_keysym_from_name(buf, XKB_KEYSYM_NO_FLAGS);
    यदि (sym != XKB_कुञ्चिका_No_चिह्नम्) {
        *sym_rtrn = sym;
        check_deprecated_keysyms(parser_warn, param, param->ctx,
                                 sym, buf, buf, "%s", "");
        प्रत्यागम true;
    }

    प्रत्यागम false;
}

#परिभाषय param_scanner param->scanner
%}

%define api.विशुद्धम्
%lex-param      { संरचना scanner *param_scanner }
%parse-param    { संरचना parser_param *param }

%define parse.error detailed

%शब्दांशः
        अन्तः_सम्बन्धि_FILE     0 "end of file"
        ERROR_TOK       255 "invalid token"
        XKB_KEYMAP      1 "xkb_keymap"
        XKB_KEYCODES    2 "xkb_keycodes"
        XKB_TYPES       3 "xkb_types"
        XKB_SYMBOLS     4 "xkb_symbols"
        XKB_COMPATMAP   5 "xkb_compatibility"
        XKB_GEOMETRY    6 "xkb_geometry"
        XKB_SEMANTICS   7 "xkb_semantics"
        XKB_LAYOUT      8 "xkb_layout"
        INCLUDE         10 "include"
        OVERRIDE        11 "override"
        AUGMENT         12 "augment"
        REPLACE         13 "replace"
        ALTERNATE       14 "alternate"
        VIRTUAL_MODS    20 "virtual_modifiers"
        TYPE            21 "type"
        INTERPRET       22 "interpret"
        ACTION_TOK      23 "action"
        कुञ्चिका_3             24 "key"
        ALIAS           25 "alias"
        गणः_2           26 "group"
        MODIFIER_MAP    27 "modifier_map"
        INDICATOR       28 "indicator"
        SHAPE           29 "shape"
        कुञ्चिकाः_2            30 "keys"
        ROW             31 "row"
        SECTION         32 "section"
        OVERLAY         33 "overlay"
        TEXT            34 "text"
        OUTLINE         35 "outline"
        SOLID           36 "solid"
        LOGO            37 "logo"
        VIRTUAL         38 "virtual"
        EQUALS          40 "="
        PLUS            41 "+"
        MINUS           42 "-"
        DIVIDE          43 "/"
        TIMES           44 "*"
        OBRACE          45 "{"
        CBRACE          46 "}"
        OPAREN          47 "("
        CPAREN          48 ")"
        OBRACKET        49 "["
        CBRACKET        50 "]"
        DOT             51 "."
        COMMA           52 ","
        SEMI            53 ";"
        EXCLAM          54 "!"
        INVERT          55 "~"
        STRING          60 "string literal"
        DECIMAL_DIGIT   61 "decimal digit"
        INTEGER         62 "integer literal"
        FLOAT           63 "float literal"
        IDENT           64 "identifier"
        KEYNAME         65 "key name"
        PARTIAL         70 "partial"
        DEFAULT         71 "default"
        HIDDEN          72 "hidden"
        ALPHANUMERIC_कुञ्चिकाः       73 "alphanumeric_keys"
        MODIFIER_कुञ्चिकाः           74 "modifier_keys"
        KEYPAD_कुञ्चिकाः             75 "keypad_keys"
        FUNCTION_कुञ्चिकाः           76 "function_keys"
        ALTERNATE_गणः         77 "alternate_group"
        DEPRECATED              78 "deprecated"

%दक्षिणतः  EQUALS
%वामतः   PLUS MINUS
%वामतः   TIMES DIVIDE
%वामतः   EXCLAM INVERT
%वामतः   OPAREN

%आरम्भः  XkbFile

%सङ्घ  {
        int64_t          अङ्कः;
        गणना xkb_file_type file_type;
        वर्ण            *str;
        संरचना sval     sval;
        xkb_atom_t      atom;
        गणना merge_बहुलकम् merge;
        गणना xkb_map_flags mapFlags;
        xkb_keysym_t    keysym;
        ParseCommon     *any;
        संरचना { ParseCommon *शीर्ष; ParseCommon *चरमः; } anyList;
        uint32_t        no_चिह्नम्_Or_Action_List;
        व्यञ्जकम्_Def         *व्यञ्जकम्;
        संरचना { व्यञ्जकम्_Def *शीर्ष; व्यञ्जकम्_Def *चरमः; } व्यञ्जकम्_List;
        VarDef          *var;
        संरचना { VarDef *शीर्ष; VarDef *चरमः; } varList;
        VModDef         *vmod;
        संरचना { VModDef *शीर्ष; VModDef *चरमः; } vmodList;
        InterpDef       *interp;
        कुञ्चिका_Type_Def      *कुञ्चिका_Type;
        SymbolsDef      *syms;
        ModMapDef       *mod_आवरणम्_2;
        गणः_Compat_Def  *गणः_Compat;
        LedMapDef       *ledMap;
        LedNameDef      *ledName;
        KeycodeDef      *कुञ्चिका_संहिता;
        कुञ्चिका_Alias_Def     *कुञ्चिका_Alias;
        UnknownStatement *unknown;
        शून्य            *geom;
        XkbFile         *file;
        संरचना { XkbFile *शीर्ष; XkbFile *चरमः; } fileList;
}

%प्रकार <अङ्कः>     DECIMAL_DIGIT INTEGER FLOAT
%प्रकार <str>     STRING
%प्रकार <sval>    IDENT
%प्रकार <atom>    KEYNAME
%प्रकार <अङ्कः>     कुञ्चिका_संहिता_2 सङ्ख्या Integer Float Signed_सङ्ख्या DoodadType
%प्रकार <merge>   Merge_बहुलकम् Opt_Merge_बहुलकम्
%प्रकार <file_type> Xkb_संयुक्त_Type FileType
%प्रकार <mapFlags> Flag Flags OptFlags
%प्रकार <str>     MapName OptMapName
%प्रकार <atom>    क्षेत्रम्_Spec Ident Element String
%प्रकार <keysym>  कुञ्चिका_Sym_2 कुञ्चिका_Sym_Lit
%प्रकार <no_चिह्नम्_Or_Action_List> No_चिह्नम्_Or_Action_List
%प्रकार <any>     Decl
%प्रकार <anyList> DeclList
%प्रकार <व्यञ्जकम्>    व्यञ्जकम्_2 पदांशः Lhs Terminal OptTerminal Coord CoordList
%प्रकार <व्यञ्जकम्>    Multi_कुञ्चिका_Sym_Or_Action_List Non_रिक्तानि_Actions Actions Action
%प्रकार <व्यञ्जकम्>    कुञ्चिका_Syms Non_रिक्तानि_कुञ्चिका_Syms कुञ्चिका_Sym_List कुञ्चिका_Or_कुञ्चिका_Sym
%प्रकार <व्यञ्जकम्_List> व्यञ्जकम्_List_2 कुञ्चिका_Or_कुञ्चिका_Sym_List
%प्रकार <व्यञ्जकम्_List> ActionList MultiActionList Multi_कुञ्चिका_Sym_List
%प्रकार <var>     VarDecl SymbolsVarDecl
%प्रकार <varList> VarDeclList SymbolsBody OptSymbolsBody
%प्रकार <vmod>    VModDef
%प्रकार <vmodList> VModDefList VModDecl
%प्रकार <interp>  InterpretDecl Interpret_मेलनम्
%प्रकार <कुञ्चिका_Type> कुञ्चिका_Type_Decl
%प्रकार <syms>    SymbolsDecl
%प्रकार <mod_आवरणम्_2> ModMapDecl
%प्रकार <गणः_Compat> गणः_Compat_Decl
%प्रकार <ledMap>  LedMapDecl
%प्रकार <ledName> LedNameDecl
%प्रकार <कुञ्चिका_संहिता> कुञ्चिका_Name_Decl
%प्रकार <कुञ्चिका_Alias> कुञ्चिका_Alias_Decl
%प्रकार <geom>    ShapeDecl SectionDecl SectionBody SectionBodyItem RowBody RowBodyItem
%प्रकार <geom>    कुञ्चिकाः_3 कुञ्चिका_4 OverlayDecl Overlay_कुञ्चिका_List Overlay_कुञ्चिका OutlineList Outline_अन्तर्_List
%प्रकार <geom>    DoodadDecl
%प्रकार <unknown> UnknownDecl UnknownCompoundStatementDecl
%प्रकार <file>    XkbFile XkbMapConfig
%प्रकार <fileList> XkbMapConfigList
%प्रकार <file>    Xkb_संयुक्त_Map

%destructor { FreeStmt((ParseCommon *) $$); }
    <any> <व्यञ्जकम्> <var> <vmod> <interp> <कुञ्चिका_Type> <syms> <mod_आवरणम्_2> <गणः_Compat>
    <ledMap> <ledName> <कुञ्चिका_संहिता> <कुञ्चिका_Alias>
%destructor { FreeStmt((ParseCommon *) $$.शीर्ष); }
    <anyList> <व्यञ्जकम्_List> <varList> <vmodList>
/* The destructor also runs on the start symbol when the parser *succeeds*.
 * The `if` here catches this case. */
%destructor { यदि (!param->rtrn) FreeXkbFile($$); } <file>
%destructor { FreeXkbFile($$.शीर्ष); } <fileList>
%destructor { मुक्त($$); } <str>

%%

/*
 * An actual file may contain more than one map. However, if we do things
 * in the normal yacc way, i.e. aggregate all of the maps into a list and
 * let the caller find the map it wants, we end up scanning and parsing a
 * lot of unneeded maps (in the end we always just need one).
 * Instead of doing that, we make yyparse return one map at a time, and
 * then call it repeatedly until we find the map we need. Once we find it,
 * we don't need to parse everything that follows in the file.
 * This does mean that if we e.g. always use the first map, the file may
 * contain complete garbage after that. But it's worth it.
 */

XkbFile         :       Xkb_संयुक्त_Map
                        { $$ = param->rtrn = $1; param->अधिकम्_maps = !!param->rtrn; (शून्य) yynerrs; }
                |       XkbMapConfig
                        { $$ = param->rtrn = $1; param->अधिकम्_maps = !!param->rtrn; YYACCEPT; }
                |       अन्तः_सम्बन्धि_FILE
                        { $$ = param->rtrn = रिक्त; param->अधिकम्_maps = false; }
                ;

Xkb_संयुक्त_Map :       OptFlags Xkb_संयुक्त_Type OptMapName OBRACE
                            XkbMapConfigList
                        CBRACE SEMI
                        { $$ = XkbFileCreate($2, $3, (ParseCommon *) $5.शीर्ष, $1); }
                ;

Xkb_संयुक्त_Type:       XKB_KEYMAP      { $$ = FILE_TYPE_KEYMAP; }
                |       XKB_SEMANTICS   { $$ = FILE_TYPE_KEYMAP; }
                |       XKB_LAYOUT      { $$ = FILE_TYPE_KEYMAP; }
                ;

/*
 * NOTE: Any component is optional
 */
XkbMapConfigList :      XkbMapConfigList XkbMapConfig
                        {
                            यदि ($2) {
                                यदि ($1.शीर्ष) {
                                    $$.शीर्ष = $1.शीर्ष;
                                    $$.चरमः->common.अग्र = &$2->common;
                                    $$.चरमः = $2;
                                } अन्यथा {
                                    $$.शीर्ष = $$.चरमः = $2;
                                }
                            }
                        }
                |       { $$.शीर्ष = $$.चरमः = रिक्त; }
                ;

XkbMapConfig    :       OptFlags FileType OptMapName OBRACE
                            DeclList
                        CBRACE SEMI
                        {
                            यदि ($1 & MAP_IS_DEPRECATED) {
                                parser_warn(param, XKB_WARNING_DEPRECATED_SECTION,
                                            "deprecated section: \"%s\"",
                                            safe_map_name($3));
                            }
                            $$ = XkbFileCreate($2, $3, $5.शीर्ष, $1);
                        }
                ;

FileType        :       XKB_KEYCODES            { $$ = FILE_TYPE_KEYCODES; }
                |       XKB_TYPES               { $$ = FILE_TYPE_TYPES; }
                |       XKB_COMPATMAP           { $$ = FILE_TYPE_COMPAT; }
                |       XKB_SYMBOLS             { $$ = FILE_TYPE_SYMBOLS; }
                |       XKB_GEOMETRY            { $$ = FILE_TYPE_GEOMETRY; }
                ;

OptFlags        :       Flags                   { $$ = $1; }
                |                               { $$ = 0; }
                ;

Flags           :       Flags Flag              { $$ = ($1 | $2); }
                |       Flag                    { $$ = $1; }
                ;

Flag            :       PARTIAL                 { $$ = MAP_IS_PARTIAL; }
                |       DEFAULT                 { $$ = MAP_IS_DEFAULT; }
                |       HIDDEN                  { $$ = MAP_IS_HIDDEN; }
                |       ALPHANUMERIC_कुञ्चिकाः       { $$ = MAP_HAS_ALPHANUMERIC; }
                |       MODIFIER_कुञ्चिकाः           { $$ = MAP_HAS_MODIFIER; }
                |       KEYPAD_कुञ्चिकाः             { $$ = MAP_HAS_KEYPAD; }
                |       FUNCTION_कुञ्चिकाः           { $$ = MAP_HAS_FN; }
                |       ALTERNATE_गणः         { $$ = MAP_IS_ALTGR; }
                |       DEPRECATED              { $$ = MAP_IS_DEPRECATED; }
                |       IDENT
                        {
                            ध्रुव bool error = (param->config.strict & PARSER_NO_UNKNOWN_SECTION_FLAGS);
                            parser_log_with_संहिता(
                                param, (error ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
                                XKB_LOG_VERBOSITY_अल्पिष्ठम्,
                                XKB_ERROR_UNKNOWN_SECTION_FLAG,
                                "Unknown section flag \"%.*s\"%s",
                                (अचिह्नित)$1.len, $1.आरम्भः,
                                (error ? "" : "; ignored")
                            );
                            यदि (error)
                                YYABORT;
                            $$ = 0;
                        }
                ;

DeclList        :       DeclList Decl
                        {
                            यदि ($2) {
                                यदि ($1.शीर्ष) {
                                    $$.शीर्ष = $1.शीर्ष; $1.चरमः->अग्र = $2; $$.चरमः = $2;
                                } अन्यथा {
                                    $$.शीर्ष = $$.चरमः = $2;
                                }
                            }
                        }
                        /*
                         * VModDecl is "inlined" directly into DeclList, i.e.
                         * each VModDef in the VModDecl is a separate Decl in
                         * the File.
                         */
                |       DeclList Opt_Merge_बहुलकम् VModDecl
                        {
                            प्रति (VModDef *vmod = $3.शीर्ष; vmod; vmod = (VModDef *) vmod->common.अग्र)
                                vmod->merge = $2;
                            यदि ($1.शीर्ष) {
                                $$.शीर्ष = $1.शीर्ष; $1.चरमः->अग्र = &$3.शीर्ष->common; $$.चरमः = &$3.चरमः->common;
                            } अन्यथा {
                                $$.शीर्ष = &$3.शीर्ष->common; $$.चरमः = &$3.चरमः->common;
                            }
                        }
                |       { $$.शीर्ष = $$.चरमः = रिक्त; }
                ;

Decl            :       Opt_Merge_बहुलकम् VarDecl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                /*      OptMergeMode VModDecl - see above. */
                |       Opt_Merge_बहुलकम् InterpretDecl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_Merge_बहुलकम् कुञ्चिका_Name_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_Merge_बहुलकम् कुञ्चिका_Alias_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_Merge_बहुलकम् कुञ्चिका_Type_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_Merge_बहुलकम् SymbolsDecl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_Merge_बहुलकम् ModMapDecl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_Merge_बहुलकम् गणः_Compat_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_Merge_बहुलकम् LedMapDecl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_Merge_बहुलकम् LedNameDecl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_Merge_बहुलकम् ShapeDecl          { $$ = रिक्त; }
                |       Opt_Merge_बहुलकम् SectionDecl        { $$ = रिक्त; }
                |       Opt_Merge_बहुलकम् DoodadDecl         { $$ = रिक्त; }
                |       Opt_Merge_बहुलकम् UnknownDecl
                            { $$ = (ParseCommon *) $2; }
                |       Opt_Merge_बहुलकम् UnknownCompoundStatementDecl
                            { $$ = (ParseCommon *) $2; }
                |       Merge_बहुलकम् STRING
                        {
                            $$ = (ParseCommon *) IncludeCreate(param->ctx, $2, $1);
                            मुक्त($2);
                        }
                ;

VarDecl         :       Lhs EQUALS व्यञ्जकम्_2 SEMI
                        { $$ = VarCreate($1, $3); }
                |       Ident SEMI
                        { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident SEMI
                        { $$ = BoolVarCreate($2, false); }
                ;

कुञ्चिका_Name_Decl     :       KEYNAME EQUALS कुञ्चिका_संहिता_2 SEMI
                        { $$ = KeycodeCreate($1, $3); }
                ;

कुञ्चिका_Alias_Decl    :       ALIAS KEYNAME EQUALS KEYNAME SEMI
                        { $$ = कुञ्चिका_Alias_Create($2, $4); }
                ;

VModDecl        :       VIRTUAL_MODS VModDefList SEMI
                        { $$ = $2; }
                ;

VModDefList     :       VModDefList COMMA VModDef
                        { $$.शीर्ष = $1.शीर्ष; $$.चरमः->common.अग्र = &$3->common; $$.चरमः = $3; }
                |       VModDef
                        { $$.शीर्ष = $$.चरमः = $1; }
                ;

VModDef         :       Ident
                        { $$ = VModCreate($1, रिक्त); }
                |       Ident EQUALS व्यञ्जकम्_2
                        { $$ = VModCreate($1, $3); }
                ;

InterpretDecl   :       INTERPRET Interpret_मेलनम् OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $2->def = $4.शीर्ष; $$ = $2; }
                ;

Interpret_मेलनम्  :       कुञ्चिका_Sym_2 PLUS व्यञ्जकम्_2
                        { $$ = InterpCreate($1, $3); }
                |       कुञ्चिका_Sym_2
                        { $$ = InterpCreate($1, रिक्त); }
                ;

VarDeclList     :       VarDeclList VarDecl
                        {
                            यदि ($2) {
                                यदि ($1.शीर्ष) {
                                    $$.शीर्ष = $1.शीर्ष;
                                    $$.चरमः->common.अग्र = &$2->common;
                                    $$.चरमः = $2;
                                } अन्यथा {
                                    $$.शीर्ष = $$.चरमः = $2;
                                }
                            }
                        }
                |       { $$.शीर्ष = $$.चरमः = रिक्त; }
                ;

कुञ्चिका_Type_Decl     :       TYPE String OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $$ = कुञ्चिका_Type_Create($2, $4.शीर्ष); }
                ;

SymbolsDecl     :       कुञ्चिका_3 KEYNAME OBRACE
                            OptSymbolsBody
                        CBRACE SEMI
                        { $$ = SymbolsCreate($2, $4.शीर्ष); }
                ;

OptSymbolsBody  :       SymbolsBody { $$ = $1; }
                |                   { $$.शीर्ष = $$.चरमः = रिक्त; }
                ;

SymbolsBody     :       SymbolsBody COMMA SymbolsVarDecl
                        { $$.शीर्ष = $1.शीर्ष; $$.चरमः->common.अग्र = &$3->common; $$.चरमः = $3; }
                |       SymbolsVarDecl
                        { $$.शीर्ष = $$.चरमः = $1; }
                ;

SymbolsVarDecl  :       Lhs EQUALS व्यञ्जकम्_2         { $$ = VarCreate($1, $3); }
                |       Lhs EQUALS Multi_कुञ्चिका_Sym_Or_Action_List { $$ = VarCreate($1, $3); }
                |       Ident                   { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident            { $$ = BoolVarCreate($2, false); }
                |       Multi_कुञ्चिका_Sym_Or_Action_List { $$ = VarCreate(रिक्त, $1); }
                ;

/*
 * A list of keysym/action lists
 *
 * There is some ambiguity because we use `{}` to denote both an empty list of
 * keysyms and an empty list of actions. But as soon as we get a keysym or an
 * action, we know whether it is a `MultiKeySymList` or a `MultiActionList`.
 * So we just count the `{}` at the *beginning* using `NoSymbolOrActionList`,
 * then replace it by the relevant count of `NoSymbol` or `NoAction()` once the
 * ambiguity is solved. If not, this is a list of empties of *some* type: we
 * drop those empties and delegate the type resolution using `ExprEmptyList()`.
 */
Multi_कुञ्चिका_Sym_Or_Action_List
                :       OBRACKET Multi_कुञ्चिका_Sym_List CBRACKET
                        { $$ = $2.शीर्ष; }
                |       OBRACKET No_चिह्नम्_Or_Action_List COMMA Multi_कुञ्चिका_Sym_List CBRACKET
                        {
                            /* Prepend n times NoSymbol */
                            संरचना {व्यञ्जकम्_Def *शीर्ष; व्यञ्जकम्_Def *चरमः;} list = {
                                .शीर्ष = $4.शीर्ष, .चरमः = $4.चरमः
                            };
                            प्रति (uint32_t k = 0; k < $2; k++) {
                                व्यञ्जकम्_Def* ध्रुव syms =
                                    व्यञ्जकम्_Create_कुञ्चिका_Sym_List(XKB_कुञ्चिका_No_चिह्नम्);
                                यदि (!syms) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                syms->common.अग्र = &list.शीर्ष->common;
                                list.शीर्ष = syms;
                            }
                            $$ = list.शीर्ष;
                        }
                |       OBRACKET MultiActionList CBRACKET
                        { $$ = $2.शीर्ष; }
                |       OBRACKET No_चिह्नम्_Or_Action_List COMMA MultiActionList CBRACKET
                        {
                            /* Prepend n times NoAction() */
                            संरचना {व्यञ्जकम्_Def *शीर्ष; व्यञ्जकम्_Def *चरमः;} list = {
                                .शीर्ष = $4.शीर्ष, .चरमः = $4.चरमः
                            };
                            प्रति (uint32_t k = 0; k < $2; k++) {
                                व्यञ्जकम्_Def* ध्रुव acts = व्यञ्जकम्_Create_Action_List(रिक्त);
                                यदि (!acts) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                acts->common.अग्र = &list.शीर्ष->common;
                                list.शीर्ष = acts;
                            }
                            $$ = list.शीर्ष;
                        }
                        /*
                         * A list of *empty* keysym/action lists `{}`.
                         * There is not enough context here to decide if it is
                         * a `MultiKeySymList` or a `MultiActionList`.
                         */
                |       OBRACKET No_चिह्नम्_Or_Action_List CBRACKET
                        { $$ = व्यञ्जकम्_रिक्तानि_List(); }
                ;

/* A list of `{}`, which remains ambiguous until reaching a keysym or action list */
No_चिह्नम्_Or_Action_List
                :       No_चिह्नम्_Or_Action_List COMMA OBRACE CBRACE
                        { $$ = $1 + 1; }
                |       OBRACE CBRACE
                        { $$ = 1; }
                |       { $$ = 0; }
                ;

गणः_Compat_Decl :       गणः_2 Integer EQUALS व्यञ्जकम्_2 SEMI
                        { $$ = गणः_Compat_Create($2, $4); }
                ;

ModMapDecl      :       MODIFIER_MAP व्यञ्जकम्_2 OBRACE कुञ्चिका_Or_कुञ्चिका_Sym_List CBRACE SEMI
                        {
                            यदि (param->config.format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
                                $2->common.प्रकार != STMT_व्यञ्जकम्_IDENT) {
                                    parser_err(
                                        param, XKB_ERROR_INVALID_MODIFIER_MAP_आवरणम्,
                                        "Invalid real modifier mask in modifier "
                                        "map definition: expected identifier"
                                    );
                                    FreeStmt((ParseCommon *) $2);
                                    FreeStmt((ParseCommon *) $4.शीर्ष);
                                    YYERROR;
                            }
                            $$ = ModMapCreate($2, $4.शीर्ष);
                        }
                ;

कुञ्चिका_Or_कुञ्चिका_Sym_List :       कुञ्चिका_Or_कुञ्चिका_Sym_List COMMA कुञ्चिका_Or_कुञ्चिका_Sym
                        { $$.शीर्ष = $1.शीर्ष; $$.चरमः->common.अग्र = &$3->common; $$.चरमः = $3; }
                |       कुञ्चिका_Or_कुञ्चिका_Sym
                        { $$.शीर्ष = $$.चरमः = $1; }
                ;

कुञ्चिका_Or_कुञ्चिका_Sym     :       KEYNAME
                        { $$ = व्यञ्जकम्_Create_कुञ्चिका_Name($1); }
                |       कुञ्चिका_Sym_2
                        { $$ = व्यञ्जकम्_Create_कुञ्चिका_Sym($1); }
                ;

LedMapDecl:             INDICATOR String OBRACE VarDeclList CBRACE SEMI
                        { $$ = LedMapCreate($2, $4.शीर्ष); }
                ;

LedNameDecl:            INDICATOR Integer EQUALS व्यञ्जकम्_2 SEMI
                        { $$ = LedNameCreate($2, $4, false); }
                |       VIRTUAL INDICATOR Integer EQUALS व्यञ्जकम्_2 SEMI
                        { $$ = LedNameCreate($3, $5, true); }
                ;

UnknownDecl     :       IDENT Terminal EQUALS व्यञ्जकम्_2 SEMI
                        {
                            FreeStmt((ParseCommon *) $2);
                            FreeStmt((ParseCommon *) $4);
                            $$ = UnknownStatementCreate(STMT_UNKNOWN_DECLARATION, $1);
                        }
                ;

UnknownCompoundStatementDecl:
                        IDENT OptTerminal OBRACE VarDeclList CBRACE SEMI
                        {
                            FreeStmt((ParseCommon *) $2);
                            FreeStmt((ParseCommon *) $4.शीर्ष);
                            $$ = UnknownStatementCreate(STMT_UNKNOWN_COMPOUND, $1);
                        }
                ;

ShapeDecl       :       SHAPE String OBRACE OutlineList CBRACE SEMI
                        { $$ = रिक्त; }
                |       SHAPE String OBRACE CoordList CBRACE SEMI
                        { (शून्य) $4; $$ = रिक्त; }
                ;

SectionDecl     :       SECTION String OBRACE SectionBody CBRACE SEMI
                        { $$ = रिक्त; }
                ;

SectionBody     :       SectionBody SectionBodyItem     { $$ = रिक्त;}
                |       SectionBodyItem                 { $$ = रिक्त; }
                ;

SectionBodyItem :       ROW OBRACE RowBody CBRACE SEMI
                        { $$ = रिक्त; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = रिक्त; }
                |       DoodadDecl
                        { $$ = रिक्त; }
                |       LedMapDecl
                        { FreeStmt((ParseCommon *) $1); $$ = रिक्त; }
                |       OverlayDecl
                        { $$ = रिक्त; }
                ;

RowBody         :       RowBody RowBodyItem     { $$ = रिक्त;}
                |       RowBodyItem             { $$ = रिक्त; }
                ;

RowBodyItem     :       कुञ्चिकाः_2 OBRACE कुञ्चिकाः_3 CBRACE SEMI { $$ = रिक्त; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = रिक्त; }
                ;

कुञ्चिकाः_3            :       कुञ्चिकाः_3 COMMA कुञ्चिका_4          { $$ = रिक्त; }
                |       कुञ्चिका_4                     { $$ = रिक्त; }
                ;

कुञ्चिका_4             :       KEYNAME
                        { $$ = रिक्त; }
                |       OBRACE व्यञ्जकम्_List_2 CBRACE
                        { FreeStmt((ParseCommon *) $2.शीर्ष); $$ = रिक्त; }
                ;

OverlayDecl     :       OVERLAY String OBRACE Overlay_कुञ्चिका_List CBRACE SEMI
                        { $$ = रिक्त; }
                ;

Overlay_कुञ्चिका_List  :       Overlay_कुञ्चिका_List COMMA Overlay_कुञ्चिका { $$ = रिक्त; }
                |       Overlay_कुञ्चिका                      { $$ = रिक्त; }
                ;

Overlay_कुञ्चिका      :       KEYNAME EQUALS KEYNAME          { $$ = रिक्त; }
                ;

OutlineList     :       OutlineList COMMA Outline_अन्तर्_List
                        { $$ = रिक्त;}
                |       Outline_अन्तर्_List
                        { $$ = रिक्त; }
                ;

Outline_अन्तर्_List   :       OBRACE CoordList CBRACE
                        { (शून्य) $2; $$ = रिक्त; }
                |       Ident EQUALS OBRACE CoordList CBRACE
                        { (शून्य) $4; $$ = रिक्त; }
                |       Ident EQUALS व्यञ्जकम्_2
                        { FreeStmt((ParseCommon *) $3); $$ = रिक्त; }
                ;

CoordList       :       CoordList COMMA Coord
                        { (शून्य) $1; (शून्य) $3; $$ = रिक्त; }
                |       Coord
                        { (शून्य) $1; $$ = रिक्त; }
                ;

Coord           :       OBRACKET Signed_सङ्ख्या COMMA Signed_सङ्ख्या CBRACKET
                        { $$ = रिक्त; }
                ;

DoodadDecl      :       DoodadType String OBRACE VarDeclList CBRACE SEMI
                        { FreeStmt((ParseCommon *) $4.शीर्ष); $$ = रिक्त; }
                ;

DoodadType      :       TEXT    { $$ = 0; }
                |       OUTLINE { $$ = 0; }
                |       SOLID   { $$ = 0; }
                |       LOGO    { $$ = 0; }
                ;

क्षेत्रम्_Spec       :       Ident   { $$ = $1; }
                |       Element { $$ = $1; }
                ;

Element         :       ACTION_TOK
                        { $$ = xkb_atom_intern_literal(param->ctx, "action"); }
                |       INTERPRET
                        { $$ = xkb_atom_intern_literal(param->ctx, "interpret"); }
                |       TYPE
                        { $$ = xkb_atom_intern_literal(param->ctx, "type"); }
                |       कुञ्चिका_3
                        { $$ = xkb_atom_intern_literal(param->ctx, "key"); }
                |       गणः_2
                        { $$ = xkb_atom_intern_literal(param->ctx, "group"); }
                |       MODIFIER_MAP
                        {$$ = xkb_atom_intern_literal(param->ctx, "modifier_map");}
                |       INDICATOR
                        { $$ = xkb_atom_intern_literal(param->ctx, "indicator"); }
                |       SHAPE
                        { $$ = xkb_atom_intern_literal(param->ctx, "shape"); }
                |       ROW
                        { $$ = xkb_atom_intern_literal(param->ctx, "row"); }
                |       SECTION
                        { $$ = xkb_atom_intern_literal(param->ctx, "section"); }
                |       TEXT
                        { $$ = xkb_atom_intern_literal(param->ctx, "text"); }
                ;

Opt_Merge_बहुलकम्    :       Merge_बहुलकम्       { $$ = $1; }
                |                       { $$ = MERGE_DEFAULT; }
                ;

Merge_बहुलकम्       :       INCLUDE         { $$ = MERGE_DEFAULT; }
                |       AUGMENT         { $$ = MERGE_AUGMENT; }
                |       OVERRIDE        { $$ = MERGE_OVERRIDE; }
                |       REPLACE         { $$ = MERGE_REPLACE; }
                |       ALTERNATE
                {
                    /*
                     * This used to be MERGE_ALT_FORM. This functionality was
                     * unused and has been removed.
                     */
                    parser_warn(param, XKB_LOG_सन्देशः_NO_ID,
                                "ignored unsupported legacy merge mode \"alternate\"");
                    $$ = MERGE_DEFAULT;
                }
                ;

व्यञ्जकम्_List_2        :       व्यञ्जकम्_List_2 COMMA व्यञ्जकम्_2
                        {
                            यदि ($3) {
                                यदि ($1.शीर्ष) {
                                    $$.शीर्ष = $1.शीर्ष;
                                    $$.चरमः->common.अग्र = &$3->common;
                                    $$.चरमः = $3;
                                } अन्यथा {
                                    $$.शीर्ष = $$.चरमः = $3;
                                }
                            }
                        }
                |       व्यञ्जकम्_2
                        { $$.शीर्ष = $$.चरमः = $1; }
                |       { $$.शीर्ष = $$.चरमः = रिक्त; }
                ;

व्यञ्जकम्_2            :       व्यञ्जकम्_2 DIVIDE व्यञ्जकम्_2
                        { $$ = व्यञ्जकम्_Create_द्विमान(STMT_व्यञ्जकम्_DIVIDE, $1, $3); }
                |       व्यञ्जकम्_2 PLUS व्यञ्जकम्_2
                        { $$ = व्यञ्जकम्_Create_द्विमान(STMT_व्यञ्जकम्_सङ्कलय, $1, $3); }
                |       व्यञ्जकम्_2 MINUS व्यञ्जकम्_2
                        { $$ = व्यञ्जकम्_Create_द्विमान(STMT_व्यञ्जकम्_व्यवकलय, $1, $3); }
                |       व्यञ्जकम्_2 TIMES व्यञ्जकम्_2
                        { $$ = व्यञ्जकम्_Create_द्विमान(STMT_व्यञ्जकम्_MULTIPLY, $1, $3); }
                |       Lhs EQUALS व्यञ्जकम्_2
                        { $$ = व्यञ्जकम्_Create_द्विमान(STMT_व्यञ्जकम्_नियोजय, $1, $3); }
                |       पदांशः
                        { $$ = $1; }
                ;

पदांशः            :       MINUS पदांशः
                        { $$ = व्यञ्जकम्_Create_Unary(STMT_व्यञ्जकम्_NEGATE, $2); }
                |       PLUS पदांशः
                        { $$ = व्यञ्जकम्_Create_Unary(STMT_व्यञ्जकम्_UNARY_PLUS, $2); }
                |       EXCLAM पदांशः
                        { $$ = व्यञ्जकम्_Create_Unary(STMT_व्यञ्जकम्_NOT, $2); }
                |       INVERT पदांशः
                        { $$ = व्यञ्जकम्_Create_Unary(STMT_व्यञ्जकम्_INVERT, $2); }
                |       Lhs
                        { $$ = $1; }
                |       क्षेत्रम्_Spec OPAREN व्यञ्जकम्_List_2 CPAREN %prec OPAREN
                        { $$ = व्यञ्जकम्_Create_Action($1, $3.शीर्ष); }
                |       Actions
                        { $$ = $1; }
                |       Terminal
                        { $$ = $1; }
                |       OPAREN व्यञ्जकम्_2 CPAREN
                        { $$ = $2; }
                ;

MultiActionList :       MultiActionList COMMA Action
                        {
                            व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create_Action_List($3);
                            $$ = $1;
                            $$.चरमः->common.अग्र = &व्यञ्जकम्->common; $$.चरमः = व्यञ्जकम्;
                        }
                |       MultiActionList COMMA Actions
                        { $$ = $1; $$.चरमः->common.अग्र = &$3->common; $$.चरमः = $3; }
                |       Action
                        { $$.शीर्ष = $$.चरमः = व्यञ्जकम्_Create_Action_List($1); }
                |       Non_रिक्तानि_Actions
                        { $$.शीर्ष = $$.चरमः = $1; }
                ;

ActionList      :       ActionList COMMA Action
                        { $$ = $1; $$.चरमः->common.अग्र = &$3->common; $$.चरमः = $3; }
                |       Action
                        { $$.शीर्ष = $$.चरमः = $1; }
                ;

Non_रिक्तानि_Actions :       OBRACE ActionList CBRACE
                        { $$ = व्यञ्जकम्_Create_Action_List($2.शीर्ष); }
                ;

Actions         :       Non_रिक्तानि_Actions
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = व्यञ्जकम्_Create_Action_List(रिक्त); }
                ;

Action          :       क्षेत्रम्_Spec OPAREN व्यञ्जकम्_List_2 CPAREN
                        { $$ = व्यञ्जकम्_Create_Action($1, $3.शीर्ष); }
                ;

Lhs             :       क्षेत्रम्_Spec
                        { $$ = व्यञ्जकम्_Create_Ident($1); }
                |       क्षेत्रम्_Spec DOT क्षेत्रम्_Spec
                        { $$ = व्यञ्जकम्_Create_क्षेत्रम्_Ref($1, $3); }
                |       क्षेत्रम्_Spec OBRACKET व्यञ्जकम्_2 CBRACKET
                        { $$ = व्यञ्जकम्_Create_Array_Ref(XKB_ATOM_NONE, $1, $3); }
                |       क्षेत्रम्_Spec DOT क्षेत्रम्_Spec OBRACKET व्यञ्जकम्_2 CBRACKET
                        { $$ = व्यञ्जकम्_Create_Array_Ref($1, $3, $5); }
                ;

OptTerminal     :       Terminal
                        { $$ = $1; }
                |       { $$ = रिक्त; }
                ;

Terminal        :       String
                        { $$ = व्यञ्जकम्_Create_String($1); }
                |       Integer
                        { $$ = व्यञ्जकम्_Create_Integer($1); }
                |       Float
                        { $$ = व्यञ्जकम्_Create_Float(/* Discard $1 */); }
                |       KEYNAME
                        { $$ = व्यञ्जकम्_Create_कुञ्चिका_Name($1); }
                ;

Multi_कुञ्चिका_Sym_List :       Multi_कुञ्चिका_Sym_List COMMA कुञ्चिका_Sym_Lit
                        {
                            व्यञ्जकम्_Def *व्यञ्जकम् = व्यञ्जकम्_Create_कुञ्चिका_Sym_List($3);
                            $$ = $1;
                            $$.चरमः->common.अग्र = &व्यञ्जकम्->common; $$.चरमः = व्यञ्जकम्;
                        }
                |       Multi_कुञ्चिका_Sym_List COMMA कुञ्चिका_Syms
                        { $$ = $1; $$.चरमः->common.अग्र = &$3->common; $$.चरमः = $3; }
                |       कुञ्चिका_Sym_Lit
                        { $$.शीर्ष = $$.चरमः = व्यञ्जकम्_Create_कुञ्चिका_Sym_List($1); }
                |       Non_रिक्तानि_कुञ्चिका_Syms
                        { $$.शीर्ष = $$.चरमः = $1; }
                ;

कुञ्चिका_Sym_List      :       कुञ्चिका_Sym_List COMMA कुञ्चिका_Sym_Lit
                        { $$ = व्यञ्जकम्_Append_कुञ्चिका_Sym_List($1, $3); }
                |       कुञ्चिका_Sym_List COMMA STRING
                        {
                            $$ = व्यञ्जकम्_कुञ्चिका_Sym_List_Append_String(param->scanner, $1, $3);
                            मुक्त($3);
                            यदि (!$$)
                                YYERROR;
                        }
                |       कुञ्चिका_Sym_Lit
                        {
                            $$ = व्यञ्जकम्_Create_कुञ्चिका_Sym_List($1);
                            यदि (!$$)
                                YYERROR;
                        }
                |       STRING
                        {
                            $$ = व्यञ्जकम्_Create_कुञ्चिका_Sym_List(XKB_कुञ्चिका_No_चिह्नम्);
                            यदि (!$$)
                                YYERROR;
                            $$ = व्यञ्जकम्_कुञ्चिका_Sym_List_Append_String(param->scanner, $$, $1);
                            मुक्त($1);
                            यदि (!$$)
                                YYERROR;
                        }
                ;

Non_रिक्तानि_कुञ्चिका_Syms :       OBRACE कुञ्चिका_Sym_List CBRACE
                        { $$ = $2; }
                |       STRING
                        {
                            $$ = व्यञ्जकम्_Create_कुञ्चिका_Sym_List(XKB_कुञ्चिका_No_चिह्नम्);
                            यदि (!$$)
                                YYERROR;
                            $$ = व्यञ्जकम्_कुञ्चिका_Sym_List_Append_String(param->scanner, $$, $1);
                            मुक्त($1);
                            यदि (!$$)
                                YYERROR;
                        }
                ;

कुञ्चिका_Syms         :       Non_रिक्तानि_कुञ्चिका_Syms
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = व्यञ्जकम्_Create_कुञ्चिका_Sym_List(XKB_कुञ्चिका_No_चिह्नम्); }
                ;

कुञ्चिका_Sym_2          :       कुञ्चिका_Sym_Lit
                        { $$ = $1; }
                |       STRING
                        {
                            $$ = KeysymParseString(param->scanner, $1);
                            मुक्त($1);
                            यदि ($$ == XKB_कुञ्चिका_No_चिह्नम्)
                                YYERROR;
                        }
                ;

कुञ्चिका_Sym_Lit       :       IDENT
                        {
                            यदि (!resolve_keysym(param, $1, &$$)) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (अचिह्नित पूर्णाङ्क) $1.len, $1.आरम्भः
                                );
                                $$ = XKB_कुञ्चिका_No_चिह्नम्;
                            }
                        }
                        /* Handle keysym that is also a keyword  */
                |       SECTION { $$ = XKB_कुञ्चिका_section; }
                |       DECIMAL_DIGIT
                        {
                            /*
                             * Special case for digits 0..9:
                             * map to XKB_KEY_0 .. XKB_KEY_9, consistent with
                             * other keysym names: <name> → XKB_KEY_<name>.
                             */
                            $$ = XKB_कुञ्चिका_0 + (xkb_keysym_t) $1;
                        }
                |       INTEGER
                        {
                            यदि ($1 < XKB_KEYSYM_MIN) {
                                /* Negative value */
                                static_assert(XKB_KEYSYM_MIN == 0,
                                              "Keysyms are positive");
                                parser_warn(
                                    param,
                                    XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                    "unrecognized keysym \"-%#06"PRIx64"\""
                                    " (%"PRId64")",
                                    -$1, $1
                                );
                                $$ = XKB_कुञ्चिका_No_चिह्नम्;
                            }
                            /*
                             * Integers 0..9 are handled with DECIMAL_DIGIT if
                             * they were formatted as single characters '0'..'9'.
                             * Otherwise they are handled here as raw keysyms
                             * values. E.g. `01` and `0x1` are interpreted as
                             * the keysym 0x0001, while `1` is interpreted as
                             * XKB_KEY_1.
                             */
                            अन्यथा {
                                /* Any other numeric value */
                                यदि ($1 <= XKB_KEYSYM_MAX) {
                                    /*
                                     * Valid keysym
                                     * No normalization is performed and value
                                     * is used as is.
                                     */
                                    $$ = (xkb_keysym_t) $1;
                                    check_deprecated_keysyms(
                                        parser_warn, param, param->ctx,
                                        $$, रिक्त, $$, "%#06"PRIx32, "");
                                } अन्यथा {
                                    /* Invalid keysym */
                                    parser_warn(
                                        param, XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                        "unrecognized keysym \"%#06"PRIx64"\" "
                                        "(%"PRId64")", $1, $1
                                    );
                                    $$ = XKB_कुञ्चिका_No_चिह्नम्;
                                }
                                parser_vrb(
                                    /*
                                     * Require an extra high verbosity, because
                                     * keysyms are formatted as number unless
                                     * enabling pretty-pretting for the
                                     * serialization.
                                     */
                                    param, XKB_LOG_VERBOSITY_COMPREHENSIVE,
                                    XKB_WARNING_NUMERIC_KEYSYM,
                                    "numeric keysym \"%#06"PRIx64"\" (%"PRId64")",
                                    $1, $1
                                );
                            }
                        }
                ;

Signed_सङ्ख्या    :       MINUS सङ्ख्या    { $$ = -$2; }
                |       सङ्ख्या          { $$ = $1; }
                ;

सङ्ख्या          :       FLOAT         { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                |       INTEGER       { $$ = $1; }
                ;

Float           :       FLOAT   { $$ = 0; }
                ;

Integer         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

कुञ्चिका_संहिता_2         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

Ident           :       IDENT   { $$ = xkb_atom_intern(param->ctx, $1.आरम्भः, $1.len); }
                |       DEFAULT { $$ = xkb_atom_intern_literal(param->ctx, "default"); }
                ;

String          :       STRING  { $$ = xkb_atom_intern(param->ctx, $1, वाक्यदैर्घ्यम्($1)); मुक्त($1); }
                ;

OptMapName      :       MapName { $$ = $1; }
                |               { $$ = रिक्त; }
                ;

MapName         :       STRING  { $$ = $1; }
                ;

%%

/* Parse a specific section */
XkbFile *
parse(संरचना xkb_context *ctx, ध्रुव संरचना parser_keymap_config *config,
      संरचना scanner *scanner, ध्रुव वर्ण *map)
{
    पूर्णाङ्क ret;
    XkbFile *first = रिक्त;
    संरचना parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = रिक्त,
        .config = *config,
        .अधिकम्_maps = false,
    };

    /*
     * If we got a specific map, we look for it exclusively and return
     * immediately upon finding it. Otherwise, we need to get the
     * default map. If we find a map marked as default, we return it
     * immediately. If there are no maps marked as default, we return
     * the first map in the file.
     */

    यावत् ((ret = yyparse(&param)) == 0 && param.अधिकम्_maps) {
        यदि (map) {
            यदि (streq_not_null(map, param.rtrn->name))
                प्रत्यागम param.rtrn;
            अन्यथा
                FreeXkbFile(param.rtrn);
        }
        अन्यथा {
            यदि (param.rtrn->flags & MAP_IS_DEFAULT) {
                FreeXkbFile(first);
                प्रत्यागम param.rtrn;
            }
            अन्यथा यदि (!first) {
                first = param.rtrn;
            }
            अन्यथा {
                FreeXkbFile(param.rtrn);
            }
        }
        param.rtrn = रिक्त;
    }

    यदि (ret != 0) {
        /* Some error happend; clear the Xkbfiles parsed so far */
        FreeXkbFile(first);
        FreeXkbFile(param.rtrn);
        प्रत्यागम रिक्त;
    }

    /*
     * Warn about implicit default section,
     * but only if not a keymap: multiple keymaps per file not supported
     */
    यदि (first && first->file_type != FILE_TYPE_KEYMAP)
        log_vrb(ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_MISSING_DEFAULT_SECTION,
                "No section name in include statement, but \"%s\" contains several; "
                "Using first defined section, \"%s\"\n",
                scanner->file_name, safe_map_name(first->name));

    प्रत्यागम first;
}

/* Parse the next section */
bool
parse_अग्र(संरचना xkb_context *ctx,
           ध्रुव संरचना parser_keymap_config *config,
           संरचना scanner *scanner, XkbFile **xkb_file)
{
    पूर्णाङ्क ret;
    संरचना parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = रिक्त,
        .config = *config,
        .अधिकम्_maps = false,
    };

    यदि ((ret = yyparse(&param)) == 0 && param.अधिकम्_maps) {
        *xkb_file = param.rtrn;
        प्रत्यागम true;
    } अन्यथा {
        FreeXkbFile(param.rtrn);
        *xkb_file = रिक्त;
        प्रत्यागम (ret == 0);
    }
}
