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
#అన్తర్భావ "config.h"

#అన్తర్భావ "scanner-utils.h"
#అన్తర్భావ "xkbcomp/ast.h"
#అన్తర్భావ "xkbcomp/xkbcomp-priv.h"
}

%{
#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>

#అన్తర్భావ "keysym.h"
#అన్తర్భావ "xkbcomp/xkbcomp-priv.h"
#అన్తర్భావ "xkbcomp/parser-priv.h"
#అన్తర్భావ "xkbcomp/ast-build.h"

నిర్మాణం parser_param {
    నిర్మాణం xkb_context *ctx;
    నిర్మాణం scanner *scanner;
    XkbFile *rtrn;
    నిర్మాణం parser_keymap_config config;
    bool మరిన్ని_maps;
};

#నిర్వచించు parser_log_with_సంకేతం(param, level, verbosity, log_సందేశం_id, fmt, ...)   \
    scanner_log_with_సంకేతం((param)->scanner, level, verbosity, log_సందేశం_id, fmt,\
                          ##__VA_ARGS__)

#నిర్వచించు parser_err(param, error_id, fmt, ...) \
    scanner_err((param)->scanner, error_id, fmt, ##__VA_ARGS__)

#నిర్వచించు parser_warn(param, warning_id, fmt, ...) \
    scanner_warn((param)->scanner, warning_id, fmt, ##__VA_ARGS__)

#నిర్వచించు parser_vrb(param, verbosity, warning_id, fmt, ...) \
    scanner_vrb((param)->scanner, verbosity, warning_id, fmt, ##__VA_ARGS__)

స్థిర శూన్య
_xkbcommon_error(నిర్మాణం parser_param *param, స్థిరరాశి అక్షరం *సందేశం)
{
    parser_err(param, XKB_ERROR_INVALID_XKB_SYNTAX, "%s", సందేశం);
}

స్థిర bool
resolve_keysym(నిర్మాణం parser_param *param, నిర్మాణం sval name, xkb_keysym_t *sym_rtrn)
{
    xkb_keysym_t sym;

    ఒకవేళ (isvaleq(name, SVAL_LIT("any")) || isvaleq(name, SVAL_LIT("nosymbol"))) {
        *sym_rtrn = XKB_కీలకం_No_చిహ్నం;
        తిరిగి true;
    }

    ఒకవేళ (isvaleq(name, SVAL_LIT("none")) || isvaleq(name, SVAL_LIT("voidsymbol"))) {
        *sym_rtrn = XKB_కీలకం_Void_చిహ్నం;
        తిరిగి true;
    }

    /* xkb_keysym_from_name needs a C string. */
    అక్షరం buf[XKB_KEYSYM_NAME_MAX_SIZE];
    ఒకవేళ (name.len >= పరిమాణం(buf)) {
        తిరిగి false;
    }
    జ్ఞాపకం_నకలు(buf, name.ప్రారంభం, name.len);
    buf[name.len] = '\0';

    sym = xkb_keysym_from_name(buf, XKB_KEYSYM_NO_FLAGS);
    ఒకవేళ (sym != XKB_కీలకం_No_చిహ్నం) {
        *sym_rtrn = sym;
        check_deprecated_keysyms(parser_warn, param, param->ctx,
                                 sym, buf, buf, "%s", "");
        తిరిగి true;
    }

    తిరిగి false;
}

#నిర్వచించు param_scanner param->scanner
%}

%define api.శుద్ధ
%lex-param      { నిర్మాణం scanner *param_scanner }
%parse-param    { నిర్మాణం parser_param *param }

%define parse.error detailed

%token
        చివర_యొక్క_FILE     0 "end of file"
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
        కీలకం_3             24 "key"
        ALIAS           25 "alias"
        సమూహం_2           26 "group"
        MODIFIER_MAP    27 "modifier_map"
        INDICATOR       28 "indicator"
        SHAPE           29 "shape"
        కీలకాలు_2            30 "keys"
        అడ్డువరుస             31 "row"
        SECTION         32 "section"
        OVERLAY         33 "overlay"
        పాఠ్యం            34 "text"
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
        ALPHANUMERIC_కీలకాలు       73 "alphanumeric_keys"
        MODIFIER_కీలకాలు           74 "modifier_keys"
        KEYPAD_కీలకాలు             75 "keypad_keys"
        FUNCTION_కీలకాలు           76 "function_keys"
        ALTERNATE_సమూహం         77 "alternate_group"
        DEPRECATED              78 "deprecated"

%కుడి  EQUALS
%ఎడమ   PLUS MINUS
%ఎడమ   TIMES DIVIDE
%ఎడమ   EXCLAM INVERT
%ఎడమ   OPAREN

%start  XkbFile

%సంఘం  {
        int64_t          అంకె;
        గణన xkb_file_type file_type;
        అక్షరం            *str;
        నిర్మాణం sval     sval;
        xkb_atom_t      atom;
        గణన విలీనం_బహుళకం విలీనం;
        గణన xkb_map_flags mapFlags;
        xkb_keysym_t    keysym;
        ParseCommon     *any;
        నిర్మాణం { ParseCommon *తల; ParseCommon *చివరి; } anyList;
        uint32_t        no_చిహ్నం_Or_Action_List;
        సమాసం_Def         *సమాసం;
        నిర్మాణం { సమాసం_Def *తల; సమాసం_Def *చివరి; } సమాసం_List;
        VarDef          *var;
        నిర్మాణం { VarDef *తల; VarDef *చివరి; } varList;
        VModDef         *vmod;
        నిర్మాణం { VModDef *తల; VModDef *చివరి; } vmodList;
        InterpDef       *interp;
        కీలకం_Type_Def      *కీలకం_Type;
        SymbolsDef      *syms;
        ModMapDef       *mod_ముసుగు_2;
        సమూహం_Compat_Def  *సమూహం_Compat;
        LedMapDef       *ledMap;
        LedNameDef      *ledName;
        KeycodeDef      *కీలకం_సంకేతం;
        కీలకం_Alias_Def     *కీలకం_Alias;
        UnknownStatement *unknown;
        శూన్య            *geom;
        XkbFile         *file;
        నిర్మాణం { XkbFile *తల; XkbFile *చివరి; } fileList;
}

%రకం <అంకె>     DECIMAL_DIGIT INTEGER FLOAT
%రకం <str>     STRING
%రకం <sval>    IDENT
%రకం <atom>    KEYNAME
%రకం <అంకె>     కీలకం_సంకేతం_2 సంఖ్య Integer Float Signed_సంఖ్య DoodadType
%రకం <విలీనం>   విలీనం_బహుళకం_2 Opt_విలీనం_బహుళకం
%రకం <file_type> Xkb_సంయుక్త_Type FileType
%రకం <mapFlags> Flag Flags OptFlags
%రకం <str>     MapName OptMapName
%రకం <atom>    క్షేత్రం_Spec Ident Element String
%రకం <keysym>  కీలకం_Sym_2 కీలకం_Sym_Lit
%రకం <no_చిహ్నం_Or_Action_List> No_చిహ్నం_Or_Action_List
%రకం <any>     Decl
%రకం <anyList> DeclList
%రకం <సమాసం>    సమాసం_2 పదం Lhs Terminal OptTerminal Coord CoordList
%రకం <సమాసం>    Multi_కీలకం_Sym_Or_Action_List Non_ఖాళీలు_Actions Actions Action
%రకం <సమాసం>    కీలకం_Syms Non_ఖాళీలు_కీలకం_Syms కీలకం_Sym_List కీలకం_Or_కీలకం_Sym
%రకం <సమాసం_List> సమాసం_List_2 కీలకం_Or_కీలకం_Sym_List
%రకం <సమాసం_List> ActionList MultiActionList Multi_కీలకం_Sym_List
%రకం <var>     VarDecl SymbolsVarDecl
%రకం <varList> VarDeclList SymbolsBody OptSymbolsBody
%రకం <vmod>    VModDef
%రకం <vmodList> VModDefList VModDecl
%రకం <interp>  InterpretDecl Interpret_సరిపోలిక
%రకం <కీలకం_Type> కీలకం_Type_Decl
%రకం <syms>    SymbolsDecl
%రకం <mod_ముసుగు_2> ModMapDecl
%రకం <సమూహం_Compat> సమూహం_Compat_Decl
%రకం <ledMap>  LedMapDecl
%రకం <ledName> LedNameDecl
%రకం <కీలకం_సంకేతం> కీలకం_Name_Decl
%రకం <కీలకం_Alias> కీలకం_Alias_Decl
%రకం <geom>    ShapeDecl SectionDecl SectionBody Section_Body_అంశం అడ్డువరుస_Body అడ్డువరుస_Body_అంశం
%రకం <geom>    కీలకాలు_3 కీలకం_4 OverlayDecl Overlay_కీలకం_List Overlay_కీలకం OutlineList Outline_లోపల_List
%రకం <geom>    DoodadDecl
%రకం <unknown> UnknownDecl UnknownCompoundStatementDecl
%రకం <file>    XkbFile XkbMapConfig
%రకం <fileList> XkbMapConfigList
%రకం <file>    Xkb_సంయుక్త_Map

%destructor { FreeStmt((ParseCommon *) $$); }
    <any> <సమాసం> <var> <vmod> <interp> <కీలకం_Type> <syms> <mod_ముసుగు_2> <సమూహం_Compat>
    <ledMap> <ledName> <కీలకం_సంకేతం> <కీలకం_Alias>
%destructor { FreeStmt((ParseCommon *) $$.తల); }
    <anyList> <సమాసం_List> <varList> <vmodList>
/* The destructor also runs on the start symbol when the parser *succeeds*.
 * The `if` here catches this case. */
%destructor { ఒకవేళ (!param->rtrn) FreeXkbFile($$); } <file>
%destructor { FreeXkbFile($$.తల); } <fileList>
%destructor { విడుదల($$); } <str>

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

XkbFile         :       Xkb_సంయుక్త_Map
                        { $$ = param->rtrn = $1; param->మరిన్ని_maps = !!param->rtrn; (శూన్య) yynerrs; }
                |       XkbMapConfig
                        { $$ = param->rtrn = $1; param->మరిన్ని_maps = !!param->rtrn; YYACCEPT; }
                |       చివర_యొక్క_FILE
                        { $$ = param->rtrn = ఖాళీ; param->మరిన్ని_maps = false; }
                ;

Xkb_సంయుక్త_Map :       OptFlags Xkb_సంయుక్త_Type OptMapName OBRACE
                            XkbMapConfigList
                        CBRACE SEMI
                        { $$ = XkbFileCreate($2, $3, (ParseCommon *) $5.తల, $1); }
                ;

Xkb_సంయుక్త_Type:       XKB_KEYMAP      { $$ = FILE_TYPE_KEYMAP; }
                |       XKB_SEMANTICS   { $$ = FILE_TYPE_KEYMAP; }
                |       XKB_LAYOUT      { $$ = FILE_TYPE_KEYMAP; }
                ;

/*
 * NOTE: Any component is optional
 */
XkbMapConfigList :      XkbMapConfigList XkbMapConfig
                        {
                            ఒకవేళ ($2) {
                                ఒకవేళ ($1.తల) {
                                    $$.తల = $1.తల;
                                    $$.చివరి->common.తదుపరి = &$2->common;
                                    $$.చివరి = $2;
                                } లేకపోతే {
                                    $$.తల = $$.చివరి = $2;
                                }
                            }
                        }
                |       { $$.తల = $$.చివరి = ఖాళీ; }
                ;

XkbMapConfig    :       OptFlags FileType OptMapName OBRACE
                            DeclList
                        CBRACE SEMI
                        {
                            ఒకవేళ ($1 & MAP_IS_DEPRECATED) {
                                parser_warn(param, XKB_WARNING_DEPRECATED_SECTION,
                                            "deprecated section: \"%s\"",
                                            సురక్షిత_map_name($3));
                            }
                            $$ = XkbFileCreate($2, $3, $5.తల, $1);
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
                |       ALPHANUMERIC_కీలకాలు       { $$ = MAP_HAS_ALPHANUMERIC; }
                |       MODIFIER_కీలకాలు           { $$ = MAP_HAS_MODIFIER; }
                |       KEYPAD_కీలకాలు             { $$ = MAP_HAS_KEYPAD; }
                |       FUNCTION_కీలకాలు           { $$ = MAP_HAS_FN; }
                |       ALTERNATE_సమూహం         { $$ = MAP_IS_ALTGR; }
                |       DEPRECATED              { $$ = MAP_IS_DEPRECATED; }
                |       IDENT
                        {
                            స్థిరరాశి bool error = (param->config.strict & PARSER_NO_UNKNOWN_SECTION_FLAGS);
                            parser_log_with_సంకేతం(
                                param, (error ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
                                XKB_LOG_VERBOSITY_కనీస,
                                XKB_ERROR_UNKNOWN_SECTION_FLAG,
                                "Unknown section flag \"%.*s\"%s",
                                (గుర్తులేని)$1.len, $1.ప్రారంభం,
                                (error ? "" : "; ignored")
                            );
                            ఒకవేళ (error)
                                YYABORT;
                            $$ = 0;
                        }
                ;

DeclList        :       DeclList Decl
                        {
                            ఒకవేళ ($2) {
                                ఒకవేళ ($1.తల) {
                                    $$.తల = $1.తల; $1.చివరి->తదుపరి = $2; $$.చివరి = $2;
                                } లేకపోతే {
                                    $$.తల = $$.చివరి = $2;
                                }
                            }
                        }
                        /*
                         * VModDecl is "inlined" directly into DeclList, i.e.
                         * each VModDef in the VModDecl is a separate Decl in
                         * the File.
                         */
                |       DeclList Opt_విలీనం_బహుళకం VModDecl
                        {
                            కొరకు (VModDef *vmod = $3.తల; vmod; vmod = (VModDef *) vmod->common.తదుపరి)
                                vmod->విలీనం = $2;
                            ఒకవేళ ($1.తల) {
                                $$.తల = $1.తల; $1.చివరి->తదుపరి = &$3.తల->common; $$.చివరి = &$3.చివరి->common;
                            } లేకపోతే {
                                $$.తల = &$3.తల->common; $$.చివరి = &$3.చివరి->common;
                            }
                        }
                |       { $$.తల = $$.చివరి = ఖాళీ; }
                ;

Decl            :       Opt_విలీనం_బహుళకం VarDecl
                        {
                            $2->విలీనం = $1;
                            $$ = (ParseCommon *) $2;
                        }
                /*      OptMergeMode VModDecl - see above. */
                |       Opt_విలీనం_బహుళకం InterpretDecl
                        {
                            $2->విలీనం = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_విలీనం_బహుళకం కీలకం_Name_Decl
                        {
                            $2->విలీనం = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_విలీనం_బహుళకం కీలకం_Alias_Decl
                        {
                            $2->విలీనం = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_విలీనం_బహుళకం కీలకం_Type_Decl
                        {
                            $2->విలీనం = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_విలీనం_బహుళకం SymbolsDecl
                        {
                            $2->విలీనం = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_విలీనం_బహుళకం ModMapDecl
                        {
                            $2->విలీనం = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_విలీనం_బహుళకం సమూహం_Compat_Decl
                        {
                            $2->విలీనం = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_విలీనం_బహుళకం LedMapDecl
                        {
                            $2->విలీనం = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_విలీనం_బహుళకం LedNameDecl
                        {
                            $2->విలీనం = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_విలీనం_బహుళకం ShapeDecl          { $$ = ఖాళీ; }
                |       Opt_విలీనం_బహుళకం SectionDecl        { $$ = ఖాళీ; }
                |       Opt_విలీనం_బహుళకం DoodadDecl         { $$ = ఖాళీ; }
                |       Opt_విలీనం_బహుళకం UnknownDecl
                            { $$ = (ParseCommon *) $2; }
                |       Opt_విలీనం_బహుళకం UnknownCompoundStatementDecl
                            { $$ = (ParseCommon *) $2; }
                |       విలీనం_బహుళకం_2 STRING
                        {
                            $$ = (ParseCommon *) IncludeCreate(param->ctx, $2, $1);
                            విడుదల($2);
                        }
                ;

VarDecl         :       Lhs EQUALS సమాసం_2 SEMI
                        { $$ = VarCreate($1, $3); }
                |       Ident SEMI
                        { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident SEMI
                        { $$ = BoolVarCreate($2, false); }
                ;

కీలకం_Name_Decl     :       KEYNAME EQUALS కీలకం_సంకేతం_2 SEMI
                        { $$ = KeycodeCreate($1, $3); }
                ;

కీలకం_Alias_Decl    :       ALIAS KEYNAME EQUALS KEYNAME SEMI
                        { $$ = కీలకం_Alias_Create($2, $4); }
                ;

VModDecl        :       VIRTUAL_MODS VModDefList SEMI
                        { $$ = $2; }
                ;

VModDefList     :       VModDefList COMMA VModDef
                        { $$.తల = $1.తల; $$.చివరి->common.తదుపరి = &$3->common; $$.చివరి = $3; }
                |       VModDef
                        { $$.తల = $$.చివరి = $1; }
                ;

VModDef         :       Ident
                        { $$ = VModCreate($1, ఖాళీ); }
                |       Ident EQUALS సమాసం_2
                        { $$ = VModCreate($1, $3); }
                ;

InterpretDecl   :       INTERPRET Interpret_సరిపోలిక OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $2->def = $4.తల; $$ = $2; }
                ;

Interpret_సరిపోలిక  :       కీలకం_Sym_2 PLUS సమాసం_2
                        { $$ = InterpCreate($1, $3); }
                |       కీలకం_Sym_2
                        { $$ = InterpCreate($1, ఖాళీ); }
                ;

VarDeclList     :       VarDeclList VarDecl
                        {
                            ఒకవేళ ($2) {
                                ఒకవేళ ($1.తల) {
                                    $$.తల = $1.తల;
                                    $$.చివరి->common.తదుపరి = &$2->common;
                                    $$.చివరి = $2;
                                } లేకపోతే {
                                    $$.తల = $$.చివరి = $2;
                                }
                            }
                        }
                |       { $$.తల = $$.చివరి = ఖాళీ; }
                ;

కీలకం_Type_Decl     :       TYPE String OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $$ = కీలకం_Type_Create($2, $4.తల); }
                ;

SymbolsDecl     :       కీలకం_3 KEYNAME OBRACE
                            OptSymbolsBody
                        CBRACE SEMI
                        { $$ = SymbolsCreate($2, $4.తల); }
                ;

OptSymbolsBody  :       SymbolsBody { $$ = $1; }
                |                   { $$.తల = $$.చివరి = ఖాళీ; }
                ;

SymbolsBody     :       SymbolsBody COMMA SymbolsVarDecl
                        { $$.తల = $1.తల; $$.చివరి->common.తదుపరి = &$3->common; $$.చివరి = $3; }
                |       SymbolsVarDecl
                        { $$.తల = $$.చివరి = $1; }
                ;

SymbolsVarDecl  :       Lhs EQUALS సమాసం_2         { $$ = VarCreate($1, $3); }
                |       Lhs EQUALS Multi_కీలకం_Sym_Or_Action_List { $$ = VarCreate($1, $3); }
                |       Ident                   { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident            { $$ = BoolVarCreate($2, false); }
                |       Multi_కీలకం_Sym_Or_Action_List { $$ = VarCreate(ఖాళీ, $1); }
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
Multi_కీలకం_Sym_Or_Action_List
                :       OBRACKET Multi_కీలకం_Sym_List CBRACKET
                        { $$ = $2.తల; }
                |       OBRACKET No_చిహ్నం_Or_Action_List COMMA Multi_కీలకం_Sym_List CBRACKET
                        {
                            /* Prepend n times NoSymbol */
                            నిర్మాణం {సమాసం_Def *తల; సమాసం_Def *చివరి;} list = {
                                .తల = $4.తల, .చివరి = $4.చివరి
                            };
                            కొరకు (uint32_t k = 0; k < $2; k++) {
                                సమాసం_Def* స్థిరరాశి syms =
                                    సమాసం_Create_కీలకం_Sym_List(XKB_కీలకం_No_చిహ్నం);
                                ఒకవేళ (!syms) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                syms->common.తదుపరి = &list.తల->common;
                                list.తల = syms;
                            }
                            $$ = list.తల;
                        }
                |       OBRACKET MultiActionList CBRACKET
                        { $$ = $2.తల; }
                |       OBRACKET No_చిహ్నం_Or_Action_List COMMA MultiActionList CBRACKET
                        {
                            /* Prepend n times NoAction() */
                            నిర్మాణం {సమాసం_Def *తల; సమాసం_Def *చివరి;} list = {
                                .తల = $4.తల, .చివరి = $4.చివరి
                            };
                            కొరకు (uint32_t k = 0; k < $2; k++) {
                                సమాసం_Def* స్థిరరాశి acts = సమాసం_Create_Action_List(ఖాళీ);
                                ఒకవేళ (!acts) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                acts->common.తదుపరి = &list.తల->common;
                                list.తల = acts;
                            }
                            $$ = list.తల;
                        }
                        /*
                         * A list of *empty* keysym/action lists `{}`.
                         * There is not enough context here to decide if it is
                         * a `MultiKeySymList` or a `MultiActionList`.
                         */
                |       OBRACKET No_చిహ్నం_Or_Action_List CBRACKET
                        { $$ = సమాసం_ఖాళీలు_List(); }
                ;

/* A list of `{}`, which remains ambiguous until reaching a keysym or action list */
No_చిహ్నం_Or_Action_List
                :       No_చిహ్నం_Or_Action_List COMMA OBRACE CBRACE
                        { $$ = $1 + 1; }
                |       OBRACE CBRACE
                        { $$ = 1; }
                |       { $$ = 0; }
                ;

సమూహం_Compat_Decl :       సమూహం_2 Integer EQUALS సమాసం_2 SEMI
                        { $$ = సమూహం_Compat_Create($2, $4); }
                ;

ModMapDecl      :       MODIFIER_MAP సమాసం_2 OBRACE కీలకం_Or_కీలకం_Sym_List CBRACE SEMI
                        {
                            ఒకవేళ (param->config.format == XKB_KEYMAP_FORMAT_పాఠ్యం_V1 &&
                                $2->common.రకం != STMT_సమాసం_IDENT) {
                                    parser_err(
                                        param, XKB_ERROR_INVALID_MODIFIER_MAP_ముసుగు,
                                        "Invalid real modifier mask in modifier "
                                        "map definition: expected identifier"
                                    );
                                    FreeStmt((ParseCommon *) $2);
                                    FreeStmt((ParseCommon *) $4.తల);
                                    YYERROR;
                            }
                            $$ = ModMapCreate($2, $4.తల);
                        }
                ;

కీలకం_Or_కీలకం_Sym_List :       కీలకం_Or_కీలకం_Sym_List COMMA కీలకం_Or_కీలకం_Sym
                        { $$.తల = $1.తల; $$.చివరి->common.తదుపరి = &$3->common; $$.చివరి = $3; }
                |       కీలకం_Or_కీలకం_Sym
                        { $$.తల = $$.చివరి = $1; }
                ;

కీలకం_Or_కీలకం_Sym     :       KEYNAME
                        { $$ = సమాసం_Create_కీలకం_Name($1); }
                |       కీలకం_Sym_2
                        { $$ = సమాసం_Create_కీలకం_Sym($1); }
                ;

LedMapDecl:             INDICATOR String OBRACE VarDeclList CBRACE SEMI
                        { $$ = LedMapCreate($2, $4.తల); }
                ;

LedNameDecl:            INDICATOR Integer EQUALS సమాసం_2 SEMI
                        { $$ = LedNameCreate($2, $4, false); }
                |       VIRTUAL INDICATOR Integer EQUALS సమాసం_2 SEMI
                        { $$ = LedNameCreate($3, $5, true); }
                ;

UnknownDecl     :       IDENT Terminal EQUALS సమాసం_2 SEMI
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
                            FreeStmt((ParseCommon *) $4.తల);
                            $$ = UnknownStatementCreate(STMT_UNKNOWN_COMPOUND, $1);
                        }
                ;

ShapeDecl       :       SHAPE String OBRACE OutlineList CBRACE SEMI
                        { $$ = ఖాళీ; }
                |       SHAPE String OBRACE CoordList CBRACE SEMI
                        { (శూన్య) $4; $$ = ఖాళీ; }
                ;

SectionDecl     :       SECTION String OBRACE SectionBody CBRACE SEMI
                        { $$ = ఖాళీ; }
                ;

SectionBody     :       SectionBody Section_Body_అంశం     { $$ = ఖాళీ;}
                |       Section_Body_అంశం                 { $$ = ఖాళీ; }
                ;

Section_Body_అంశం :       అడ్డువరుస OBRACE అడ్డువరుస_Body CBRACE SEMI
                        { $$ = ఖాళీ; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ఖాళీ; }
                |       DoodadDecl
                        { $$ = ఖాళీ; }
                |       LedMapDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ఖాళీ; }
                |       OverlayDecl
                        { $$ = ఖాళీ; }
                ;

అడ్డువరుస_Body         :       అడ్డువరుస_Body అడ్డువరుస_Body_అంశం     { $$ = ఖాళీ;}
                |       అడ్డువరుస_Body_అంశం             { $$ = ఖాళీ; }
                ;

అడ్డువరుస_Body_అంశం     :       కీలకాలు_2 OBRACE కీలకాలు_3 CBRACE SEMI { $$ = ఖాళీ; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ఖాళీ; }
                ;

కీలకాలు_3            :       కీలకాలు_3 COMMA కీలకం_4          { $$ = ఖాళీ; }
                |       కీలకం_4                     { $$ = ఖాళీ; }
                ;

కీలకం_4             :       KEYNAME
                        { $$ = ఖాళీ; }
                |       OBRACE సమాసం_List_2 CBRACE
                        { FreeStmt((ParseCommon *) $2.తల); $$ = ఖాళీ; }
                ;

OverlayDecl     :       OVERLAY String OBRACE Overlay_కీలకం_List CBRACE SEMI
                        { $$ = ఖాళీ; }
                ;

Overlay_కీలకం_List  :       Overlay_కీలకం_List COMMA Overlay_కీలకం { $$ = ఖాళీ; }
                |       Overlay_కీలకం                      { $$ = ఖాళీ; }
                ;

Overlay_కీలకం      :       KEYNAME EQUALS KEYNAME          { $$ = ఖాళీ; }
                ;

OutlineList     :       OutlineList COMMA Outline_లోపల_List
                        { $$ = ఖాళీ;}
                |       Outline_లోపల_List
                        { $$ = ఖాళీ; }
                ;

Outline_లోపల_List   :       OBRACE CoordList CBRACE
                        { (శూన్య) $2; $$ = ఖాళీ; }
                |       Ident EQUALS OBRACE CoordList CBRACE
                        { (శూన్య) $4; $$ = ఖాళీ; }
                |       Ident EQUALS సమాసం_2
                        { FreeStmt((ParseCommon *) $3); $$ = ఖాళీ; }
                ;

CoordList       :       CoordList COMMA Coord
                        { (శూన్య) $1; (శూన్య) $3; $$ = ఖాళీ; }
                |       Coord
                        { (శూన్య) $1; $$ = ఖాళీ; }
                ;

Coord           :       OBRACKET Signed_సంఖ్య COMMA Signed_సంఖ్య CBRACKET
                        { $$ = ఖాళీ; }
                ;

DoodadDecl      :       DoodadType String OBRACE VarDeclList CBRACE SEMI
                        { FreeStmt((ParseCommon *) $4.తల); $$ = ఖాళీ; }
                ;

DoodadType      :       పాఠ్యం    { $$ = 0; }
                |       OUTLINE { $$ = 0; }
                |       SOLID   { $$ = 0; }
                |       LOGO    { $$ = 0; }
                ;

క్షేత్రం_Spec       :       Ident   { $$ = $1; }
                |       Element { $$ = $1; }
                ;

Element         :       ACTION_TOK
                        { $$ = xkb_atom_intern_literal(param->ctx, "action"); }
                |       INTERPRET
                        { $$ = xkb_atom_intern_literal(param->ctx, "interpret"); }
                |       TYPE
                        { $$ = xkb_atom_intern_literal(param->ctx, "type"); }
                |       కీలకం_3
                        { $$ = xkb_atom_intern_literal(param->ctx, "key"); }
                |       సమూహం_2
                        { $$ = xkb_atom_intern_literal(param->ctx, "group"); }
                |       MODIFIER_MAP
                        {$$ = xkb_atom_intern_literal(param->ctx, "modifier_map");}
                |       INDICATOR
                        { $$ = xkb_atom_intern_literal(param->ctx, "indicator"); }
                |       SHAPE
                        { $$ = xkb_atom_intern_literal(param->ctx, "shape"); }
                |       అడ్డువరుస
                        { $$ = xkb_atom_intern_literal(param->ctx, "row"); }
                |       SECTION
                        { $$ = xkb_atom_intern_literal(param->ctx, "section"); }
                |       పాఠ్యం
                        { $$ = xkb_atom_intern_literal(param->ctx, "text"); }
                ;

Opt_విలీనం_బహుళకం    :       విలీనం_బహుళకం_2       { $$ = $1; }
                |                       { $$ = విలీనం_DEFAULT; }
                ;

విలీనం_బహుళకం_2       :       INCLUDE         { $$ = విలీనం_DEFAULT; }
                |       AUGMENT         { $$ = విలీనం_AUGMENT; }
                |       OVERRIDE        { $$ = విలీనం_OVERRIDE; }
                |       REPLACE         { $$ = విలీనం_REPLACE; }
                |       ALTERNATE
                {
                    /*
                     * This used to be MERGE_ALT_FORM. This functionality was
                     * unused and has been removed.
                     */
                    parser_warn(param, XKB_LOG_సందేశం_NO_ID,
                                "ignored unsupported legacy merge mode \"alternate\"");
                    $$ = విలీనం_DEFAULT;
                }
                ;

సమాసం_List_2        :       సమాసం_List_2 COMMA సమాసం_2
                        {
                            ఒకవేళ ($3) {
                                ఒకవేళ ($1.తల) {
                                    $$.తల = $1.తల;
                                    $$.చివరి->common.తదుపరి = &$3->common;
                                    $$.చివరి = $3;
                                } లేకపోతే {
                                    $$.తల = $$.చివరి = $3;
                                }
                            }
                        }
                |       సమాసం_2
                        { $$.తల = $$.చివరి = $1; }
                |       { $$.తల = $$.చివరి = ఖాళీ; }
                ;

సమాసం_2            :       సమాసం_2 DIVIDE సమాసం_2
                        { $$ = సమాసం_Create_ద్విమాన(STMT_సమాసం_DIVIDE, $1, $3); }
                |       సమాసం_2 PLUS సమాసం_2
                        { $$ = సమాసం_Create_ద్విమాన(STMT_సమాసం_కూడు, $1, $3); }
                |       సమాసం_2 MINUS సమాసం_2
                        { $$ = సమాసం_Create_ద్విమాన(STMT_సమాసం_వ్యవకలనం, $1, $3); }
                |       సమాసం_2 TIMES సమాసం_2
                        { $$ = సమాసం_Create_ద్విమాన(STMT_సమాసం_MULTIPLY, $1, $3); }
                |       Lhs EQUALS సమాసం_2
                        { $$ = సమాసం_Create_ద్విమాన(STMT_సమాసం_కేటాయించు, $1, $3); }
                |       పదం
                        { $$ = $1; }
                ;

పదం            :       MINUS పదం
                        { $$ = సమాసం_Create_Unary(STMT_సమాసం_NEGATE, $2); }
                |       PLUS పదం
                        { $$ = సమాసం_Create_Unary(STMT_సమాసం_UNARY_PLUS, $2); }
                |       EXCLAM పదం
                        { $$ = సమాసం_Create_Unary(STMT_సమాసం_NOT, $2); }
                |       INVERT పదం
                        { $$ = సమాసం_Create_Unary(STMT_సమాసం_INVERT, $2); }
                |       Lhs
                        { $$ = $1; }
                |       క్షేత్రం_Spec OPAREN సమాసం_List_2 CPAREN %prec OPAREN
                        { $$ = సమాసం_Create_Action($1, $3.తల); }
                |       Actions
                        { $$ = $1; }
                |       Terminal
                        { $$ = $1; }
                |       OPAREN సమాసం_2 CPAREN
                        { $$ = $2; }
                ;

MultiActionList :       MultiActionList COMMA Action
                        {
                            సమాసం_Def *సమాసం = సమాసం_Create_Action_List($3);
                            $$ = $1;
                            $$.చివరి->common.తదుపరి = &సమాసం->common; $$.చివరి = సమాసం;
                        }
                |       MultiActionList COMMA Actions
                        { $$ = $1; $$.చివరి->common.తదుపరి = &$3->common; $$.చివరి = $3; }
                |       Action
                        { $$.తల = $$.చివరి = సమాసం_Create_Action_List($1); }
                |       Non_ఖాళీలు_Actions
                        { $$.తల = $$.చివరి = $1; }
                ;

ActionList      :       ActionList COMMA Action
                        { $$ = $1; $$.చివరి->common.తదుపరి = &$3->common; $$.చివరి = $3; }
                |       Action
                        { $$.తల = $$.చివరి = $1; }
                ;

Non_ఖాళీలు_Actions :       OBRACE ActionList CBRACE
                        { $$ = సమాసం_Create_Action_List($2.తల); }
                ;

Actions         :       Non_ఖాళీలు_Actions
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = సమాసం_Create_Action_List(ఖాళీ); }
                ;

Action          :       క్షేత్రం_Spec OPAREN సమాసం_List_2 CPAREN
                        { $$ = సమాసం_Create_Action($1, $3.తల); }
                ;

Lhs             :       క్షేత్రం_Spec
                        { $$ = సమాసం_Create_Ident($1); }
                |       క్షేత్రం_Spec DOT క్షేత్రం_Spec
                        { $$ = సమాసం_Create_క్షేత్రం_Ref($1, $3); }
                |       క్షేత్రం_Spec OBRACKET సమాసం_2 CBRACKET
                        { $$ = సమాసం_Create_Array_Ref(XKB_ATOM_NONE, $1, $3); }
                |       క్షేత్రం_Spec DOT క్షేత్రం_Spec OBRACKET సమాసం_2 CBRACKET
                        { $$ = సమాసం_Create_Array_Ref($1, $3, $5); }
                ;

OptTerminal     :       Terminal
                        { $$ = $1; }
                |       { $$ = ఖాళీ; }
                ;

Terminal        :       String
                        { $$ = సమాసం_Create_String($1); }
                |       Integer
                        { $$ = సమాసం_Create_Integer($1); }
                |       Float
                        { $$ = సమాసం_Create_Float(/* Discard $1 */); }
                |       KEYNAME
                        { $$ = సమాసం_Create_కీలకం_Name($1); }
                ;

Multi_కీలకం_Sym_List :       Multi_కీలకం_Sym_List COMMA కీలకం_Sym_Lit
                        {
                            సమాసం_Def *సమాసం = సమాసం_Create_కీలకం_Sym_List($3);
                            $$ = $1;
                            $$.చివరి->common.తదుపరి = &సమాసం->common; $$.చివరి = సమాసం;
                        }
                |       Multi_కీలకం_Sym_List COMMA కీలకం_Syms
                        { $$ = $1; $$.చివరి->common.తదుపరి = &$3->common; $$.చివరి = $3; }
                |       కీలకం_Sym_Lit
                        { $$.తల = $$.చివరి = సమాసం_Create_కీలకం_Sym_List($1); }
                |       Non_ఖాళీలు_కీలకం_Syms
                        { $$.తల = $$.చివరి = $1; }
                ;

కీలకం_Sym_List      :       కీలకం_Sym_List COMMA కీలకం_Sym_Lit
                        { $$ = సమాసం_Append_కీలకం_Sym_List($1, $3); }
                |       కీలకం_Sym_List COMMA STRING
                        {
                            $$ = సమాసం_కీలకం_Sym_List_Append_String(param->scanner, $1, $3);
                            విడుదల($3);
                            ఒకవేళ (!$$)
                                YYERROR;
                        }
                |       కీలకం_Sym_Lit
                        {
                            $$ = సమాసం_Create_కీలకం_Sym_List($1);
                            ఒకవేళ (!$$)
                                YYERROR;
                        }
                |       STRING
                        {
                            $$ = సమాసం_Create_కీలకం_Sym_List(XKB_కీలకం_No_చిహ్నం);
                            ఒకవేళ (!$$)
                                YYERROR;
                            $$ = సమాసం_కీలకం_Sym_List_Append_String(param->scanner, $$, $1);
                            విడుదల($1);
                            ఒకవేళ (!$$)
                                YYERROR;
                        }
                ;

Non_ఖాళీలు_కీలకం_Syms :       OBRACE కీలకం_Sym_List CBRACE
                        { $$ = $2; }
                |       STRING
                        {
                            $$ = సమాసం_Create_కీలకం_Sym_List(XKB_కీలకం_No_చిహ్నం);
                            ఒకవేళ (!$$)
                                YYERROR;
                            $$ = సమాసం_కీలకం_Sym_List_Append_String(param->scanner, $$, $1);
                            విడుదల($1);
                            ఒకవేళ (!$$)
                                YYERROR;
                        }
                ;

కీలకం_Syms         :       Non_ఖాళీలు_కీలకం_Syms
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = సమాసం_Create_కీలకం_Sym_List(XKB_కీలకం_No_చిహ్నం); }
                ;

కీలకం_Sym_2          :       కీలకం_Sym_Lit
                        { $$ = $1; }
                |       STRING
                        {
                            $$ = KeysymParseString(param->scanner, $1);
                            విడుదల($1);
                            ఒకవేళ ($$ == XKB_కీలకం_No_చిహ్నం)
                                YYERROR;
                        }
                ;

కీలకం_Sym_Lit       :       IDENT
                        {
                            ఒకవేళ (!resolve_keysym(param, $1, &$$)) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (గుర్తులేని పూర్ణాంకం) $1.len, $1.ప్రారంభం
                                );
                                $$ = XKB_కీలకం_No_చిహ్నం;
                            }
                        }
                        /* Handle keysym that is also a keyword  */
                |       SECTION { $$ = XKB_కీలకం_section; }
                |       DECIMAL_DIGIT
                        {
                            /*
                             * Special case for digits 0..9:
                             * map to XKB_KEY_0 .. XKB_KEY_9, consistent with
                             * other keysym names: <name> → XKB_KEY_<name>.
                             */
                            $$ = XKB_కీలకం_0 + (xkb_keysym_t) $1;
                        }
                |       INTEGER
                        {
                            ఒకవేళ ($1 < XKB_KEYSYM_MIN) {
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
                                $$ = XKB_కీలకం_No_చిహ్నం;
                            }
                            /*
                             * Integers 0..9 are handled with DECIMAL_DIGIT if
                             * they were formatted as single characters '0'..'9'.
                             * Otherwise they are handled here as raw keysyms
                             * values. E.g. `01` and `0x1` are interpreted as
                             * the keysym 0x0001, while `1` is interpreted as
                             * XKB_KEY_1.
                             */
                            లేకపోతే {
                                /* Any other numeric value */
                                ఒకవేళ ($1 <= XKB_KEYSYM_MAX) {
                                    /*
                                     * Valid keysym
                                     * No normalization is performed and value
                                     * is used as is.
                                     */
                                    $$ = (xkb_keysym_t) $1;
                                    check_deprecated_keysyms(
                                        parser_warn, param, param->ctx,
                                        $$, ఖాళీ, $$, "%#06"PRIx32, "");
                                } లేకపోతే {
                                    /* Invalid keysym */
                                    parser_warn(
                                        param, XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                        "unrecognized keysym \"%#06"PRIx64"\" "
                                        "(%"PRId64")", $1, $1
                                    );
                                    $$ = XKB_కీలకం_No_చిహ్నం;
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

Signed_సంఖ్య    :       MINUS సంఖ్య    { $$ = -$2; }
                |       సంఖ్య          { $$ = $1; }
                ;

సంఖ్య          :       FLOAT         { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                |       INTEGER       { $$ = $1; }
                ;

Float           :       FLOAT   { $$ = 0; }
                ;

Integer         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

కీలకం_సంకేతం_2         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

Ident           :       IDENT   { $$ = xkb_atom_intern(param->ctx, $1.ప్రారంభం, $1.len); }
                |       DEFAULT { $$ = xkb_atom_intern_literal(param->ctx, "default"); }
                ;

String          :       STRING  { $$ = xkb_atom_intern(param->ctx, $1, అక్షరశ్రేణి_పొడవు($1)); విడుదల($1); }
                ;

OptMapName      :       MapName { $$ = $1; }
                |               { $$ = ఖాళీ; }
                ;

MapName         :       STRING  { $$ = $1; }
                ;

%%

/* Parse a specific section */
XkbFile *
parse(నిర్మాణం xkb_context *ctx, స్థిరరాశి నిర్మాణం parser_keymap_config *config,
      నిర్మాణం scanner *scanner, స్థిరరాశి అక్షరం *map)
{
    పూర్ణాంకం ret;
    XkbFile *మొదటి = ఖాళీ;
    నిర్మాణం parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = ఖాళీ,
        .config = *config,
        .మరిన్ని_maps = false,
    };

    /*
     * If we got a specific map, we look for it exclusively and return
     * immediately upon finding it. Otherwise, we need to get the
     * default map. If we find a map marked as default, we return it
     * immediately. If there are no maps marked as default, we return
     * the first map in the file.
     */

    అయితే ((ret = yyparse(&param)) == 0 && param.మరిన్ని_maps) {
        ఒకవేళ (map) {
            ఒకవేళ (streq_not_null(map, param.rtrn->name))
                తిరిగి param.rtrn;
            లేకపోతే
                FreeXkbFile(param.rtrn);
        }
        లేకపోతే {
            ఒకవేళ (param.rtrn->flags & MAP_IS_DEFAULT) {
                FreeXkbFile(మొదటి);
                తిరిగి param.rtrn;
            }
            లేకపోతే ఒకవేళ (!మొదటి) {
                మొదటి = param.rtrn;
            }
            లేకపోతే {
                FreeXkbFile(param.rtrn);
            }
        }
        param.rtrn = ఖాళీ;
    }

    ఒకవేళ (ret != 0) {
        /* Some error happend; clear the Xkbfiles parsed so far */
        FreeXkbFile(మొదటి);
        FreeXkbFile(param.rtrn);
        తిరిగి ఖాళీ;
    }

    /*
     * Warn about implicit default section,
     * but only if not a keymap: multiple keymaps per file not supported
     */
    ఒకవేళ (మొదటి && మొదటి->file_type != FILE_TYPE_KEYMAP)
        log_vrb(ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_MISSING_DEFAULT_SECTION,
                "No section name in include statement, but \"%s\" contains several; "
                "Using first defined section, \"%s\"\n",
                scanner->file_name, సురక్షిత_map_name(మొదటి->name));

    తిరిగి మొదటి;
}

/* Parse the next section */
bool
parse_తదుపరి(నిర్మాణం xkb_context *ctx,
           స్థిరరాశి నిర్మాణం parser_keymap_config *config,
           నిర్మాణం scanner *scanner, XkbFile **xkb_file)
{
    పూర్ణాంకం ret;
    నిర్మాణం parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = ఖాళీ,
        .config = *config,
        .మరిన్ని_maps = false,
    };

    ఒకవేళ ((ret = yyparse(&param)) == 0 && param.మరిన్ని_maps) {
        *xkb_file = param.rtrn;
        తిరిగి true;
    } లేకపోతే {
        FreeXkbFile(param.rtrn);
        *xkb_file = ఖాళీ;
        తిరిగి (ret == 0);
    }
}
