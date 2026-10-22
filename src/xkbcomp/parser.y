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
#অন্তর্ভাব "config.h"

#অন্তর্ভাব "scanner-utils.h"
#অন্তর্ভাব "xkbcomp/ast.h"
#অন্তর্ভাব "xkbcomp/xkbcomp-priv.h"
}

%{
#অন্তর্ভাব "config.h"

#অন্তর্ভাব <assert.h>

#অন্তর্ভাব "keysym.h"
#অন্তর্ভাব "xkbcomp/xkbcomp-priv.h"
#অন্তর্ভাব "xkbcomp/parser-priv.h"
#অন্তর্ভাব "xkbcomp/ast-build.h"

কাঠামো parser_param {
    কাঠামো xkb_context *ctx;
    কাঠামো scanner *scanner;
    XkbFile *rtrn;
    কাঠামো parser_keymap_config config;
    bool more_maps;
};

#define parser_log_with_code(param, level, verbosity, log_বার্তা_id, fmt, ...)   \
    scanner_log_with_code((param)->scanner, level, verbosity, log_বার্তা_id, fmt,\
                          ##__VA_ARGS__)

#define parser_err(param, error_id, fmt, ...) \
    scanner_err((param)->scanner, error_id, fmt, ##__VA_ARGS__)

#define parser_warn(param, warning_id, fmt, ...) \
    scanner_warn((param)->scanner, warning_id, fmt, ##__VA_ARGS__)

#define parser_vrb(param, verbosity, warning_id, fmt, ...) \
    scanner_vrb((param)->scanner, verbosity, warning_id, fmt, ##__VA_ARGS__)

স্থির শূন্য
_xkbcommon_error(কাঠামো parser_param *param, ধ্রুবক অক্ষর *বার্তা)
{
    parser_err(param, XKB_ERROR_INVALID_XKB_SYNTAX, "%s", বার্তা);
}

স্থির bool
resolve_keysym(কাঠামো parser_param *param, কাঠামো sval name, xkb_keysym_t *sym_rtrn)
{
    xkb_keysym_t sym;

    যদি (isvaleq(name, SVAL_LIT("any")) || isvaleq(name, SVAL_LIT("nosymbol"))) {
        *sym_rtrn = XKB_চাবি_No_প্রতীক;
        ফেরত true;
    }

    যদি (isvaleq(name, SVAL_LIT("none")) || isvaleq(name, SVAL_LIT("voidsymbol"))) {
        *sym_rtrn = XKB_চাবি_Void_প্রতীক;
        ফেরত true;
    }

    /* xkb_keysym_from_name needs a C string. */
    অক্ষর buf[XKB_KEYSYM_NAME_MAX_SIZE];
    যদি (name.len >= আকার(buf)) {
        ফেরত false;
    }
    memcpy(buf, name.start, name.len);
    buf[name.len] = '\0';

    sym = xkb_keysym_from_name(buf, XKB_KEYSYM_NO_FLAGS);
    যদি (sym != XKB_চাবি_No_প্রতীক) {
        *sym_rtrn = sym;
        check_deprecated_keysyms(parser_warn, param, param->ctx,
                                 sym, buf, buf, "%s", "");
        ফেরত true;
    }

    ফেরত false;
}

#define param_scanner param->scanner
%}

%define api.pure
%lex-param      { কাঠামো scanner *param_scanner }
%parse-param    { কাঠামো parser_param *param }

%define parse.error detailed

%token
        END_OF_FILE     0 "end of file"
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
        চাবি_3             24 "key"
        ALIAS           25 "alias"
        GROUP           26 "group"
        MODIFIER_MAP    27 "modifier_map"
        INDICATOR       28 "indicator"
        SHAPE           29 "shape"
        চাবিগুলি_2            30 "keys"
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
        ALPHANUMERIC_চাবিগুলি       73 "alphanumeric_keys"
        MODIFIER_চাবিগুলি           74 "modifier_keys"
        KEYPAD_চাবিগুলি             75 "keypad_keys"
        FUNCTION_চাবিগুলি           76 "function_keys"
        ALTERNATE_GROUP         77 "alternate_group"
        DEPRECATED              78 "deprecated"

%right  EQUALS
%left   PLUS MINUS
%left   TIMES DIVIDE
%left   EXCLAM INVERT
%left   OPAREN

%start  XkbFile

%সংঘ  {
        int64_t          অঙ্ক;
        গণনা xkb_file_type file_type;
        অক্ষর            *str;
        কাঠামো sval     sval;
        xkb_atom_t      atom;
        গণনা merge_mode merge;
        গণনা xkb_map_flags mapFlags;
        xkb_keysym_t    keysym;
        ParseCommon     *any;
        কাঠামো { ParseCommon *শীর্ষ; ParseCommon *last; } anyList;
        uint32_t        no_প্রতীক_Or_Action_List;
        রাশিমালা_Def         *রাশিমালা;
        কাঠামো { রাশিমালা_Def *শীর্ষ; রাশিমালা_Def *last; } রাশিমালা_List;
        VarDef          *var;
        কাঠামো { VarDef *শীর্ষ; VarDef *last; } varList;
        VModDef         *vmod;
        কাঠামো { VModDef *শীর্ষ; VModDef *last; } vmodList;
        InterpDef       *interp;
        চাবি_Type_Def      *চাবি_Type;
        SymbolsDef      *syms;
        ModMapDef       *modMask;
        GroupCompatDef  *groupCompat;
        LedMapDef       *ledMap;
        LedNameDef      *ledName;
        KeycodeDef      *চাবি_Code;
        চাবি_Alias_Def     *চাবি_Alias;
        UnknownStatement *unknown;
        শূন্য            *geom;
        XkbFile         *file;
        কাঠামো { XkbFile *শীর্ষ; XkbFile *last; } fileList;
}

%প্রকার <অঙ্ক>     DECIMAL_DIGIT INTEGER FLOAT
%প্রকার <str>     STRING
%প্রকার <sval>    IDENT
%প্রকার <atom>    KEYNAME
%প্রকার <অঙ্ক>     চাবি_Code_2 সংখ্যা Integer Float Signed_সংখ্যা DoodadType
%প্রকার <merge>   MergeMode OptMergeMode
%প্রকার <file_type> XkbCompositeType FileType
%প্রকার <mapFlags> Flag Flags OptFlags
%প্রকার <str>     MapName OptMapName
%প্রকার <atom>    FieldSpec Ident Element String
%প্রকার <keysym>  চাবি_Sym_2 চাবি_Sym_Lit
%প্রকার <no_প্রতীক_Or_Action_List> No_প্রতীক_Or_Action_List
%প্রকার <any>     Decl
%প্রকার <anyList> DeclList
%প্রকার <রাশিমালা>    রাশিমালা_2 Term Lhs Terminal OptTerminal Coord CoordList
%প্রকার <রাশিমালা>    Multi_চাবি_Sym_Or_Action_List NonEmptyActions Actions Action
%প্রকার <রাশিমালা>    চাবি_Syms Non_Empty_চাবি_Syms চাবি_Sym_List চাবি_Or_চাবি_Sym
%প্রকার <রাশিমালা_List> রাশিমালা_List_2 চাবি_Or_চাবি_Sym_List
%প্রকার <রাশিমালা_List> ActionList MultiActionList Multi_চাবি_Sym_List
%প্রকার <var>     VarDecl SymbolsVarDecl
%প্রকার <varList> VarDeclList SymbolsBody OptSymbolsBody
%প্রকার <vmod>    VModDef
%প্রকার <vmodList> VModDefList VModDecl
%প্রকার <interp>  InterpretDecl InterpretMatch
%প্রকার <চাবি_Type> চাবি_Type_Decl
%প্রকার <syms>    SymbolsDecl
%প্রকার <modMask> ModMapDecl
%প্রকার <groupCompat> GroupCompatDecl
%প্রকার <ledMap>  LedMapDecl
%প্রকার <ledName> LedNameDecl
%প্রকার <চাবি_Code> চাবি_Name_Decl
%প্রকার <চাবি_Alias> চাবি_Alias_Decl
%প্রকার <geom>    ShapeDecl SectionDecl SectionBody SectionBodyItem RowBody RowBodyItem
%প্রকার <geom>    চাবিগুলি_3 চাবি_4 OverlayDecl Overlay_চাবি_List Overlay_চাবি OutlineList OutlineInList
%প্রকার <geom>    DoodadDecl
%প্রকার <unknown> UnknownDecl UnknownCompoundStatementDecl
%প্রকার <file>    XkbFile XkbMapConfig
%প্রকার <fileList> XkbMapConfigList
%প্রকার <file>    XkbCompositeMap

%destructor { FreeStmt((ParseCommon *) $$); }
    <any> <রাশিমালা> <var> <vmod> <interp> <চাবি_Type> <syms> <modMask> <groupCompat>
    <ledMap> <ledName> <চাবি_Code> <চাবি_Alias>
%destructor { FreeStmt((ParseCommon *) $$.শীর্ষ); }
    <anyList> <রাশিমালা_List> <varList> <vmodList>
/* The destructor also runs on the start symbol when the parser *succeeds*.
 * The `if` here catches this case. */
%destructor { যদি (!param->rtrn) FreeXkbFile($$); } <file>
%destructor { FreeXkbFile($$.শীর্ষ); } <fileList>
%destructor { মুক্ত($$); } <str>

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

XkbFile         :       XkbCompositeMap
                        { $$ = param->rtrn = $1; param->more_maps = !!param->rtrn; (শূন্য) yynerrs; }
                |       XkbMapConfig
                        { $$ = param->rtrn = $1; param->more_maps = !!param->rtrn; YYACCEPT; }
                |       END_OF_FILE
                        { $$ = param->rtrn = ফাঁকা; param->more_maps = false; }
                ;

XkbCompositeMap :       OptFlags XkbCompositeType OptMapName OBRACE
                            XkbMapConfigList
                        CBRACE SEMI
                        { $$ = XkbFileCreate($2, $3, (ParseCommon *) $5.শীর্ষ, $1); }
                ;

XkbCompositeType:       XKB_KEYMAP      { $$ = FILE_TYPE_KEYMAP; }
                |       XKB_SEMANTICS   { $$ = FILE_TYPE_KEYMAP; }
                |       XKB_LAYOUT      { $$ = FILE_TYPE_KEYMAP; }
                ;

/*
 * NOTE: Any component is optional
 */
XkbMapConfigList :      XkbMapConfigList XkbMapConfig
                        {
                            যদি ($2) {
                                যদি ($1.শীর্ষ) {
                                    $$.শীর্ষ = $1.শীর্ষ;
                                    $$.last->common.পরবর্তী = &$2->common;
                                    $$.last = $2;
                                } অন্যথা {
                                    $$.শীর্ষ = $$.last = $2;
                                }
                            }
                        }
                |       { $$.শীর্ষ = $$.last = ফাঁকা; }
                ;

XkbMapConfig    :       OptFlags FileType OptMapName OBRACE
                            DeclList
                        CBRACE SEMI
                        {
                            যদি ($1 & MAP_IS_DEPRECATED) {
                                parser_warn(param, XKB_WARNING_DEPRECATED_SECTION,
                                            "deprecated section: \"%s\"",
                                            safe_map_name($3));
                            }
                            $$ = XkbFileCreate($2, $3, $5.শীর্ষ, $1);
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
                |       ALPHANUMERIC_চাবিগুলি       { $$ = MAP_HAS_ALPHANUMERIC; }
                |       MODIFIER_চাবিগুলি           { $$ = MAP_HAS_MODIFIER; }
                |       KEYPAD_চাবিগুলি             { $$ = MAP_HAS_KEYPAD; }
                |       FUNCTION_চাবিগুলি           { $$ = MAP_HAS_FN; }
                |       ALTERNATE_GROUP         { $$ = MAP_IS_ALTGR; }
                |       DEPRECATED              { $$ = MAP_IS_DEPRECATED; }
                |       IDENT
                        {
                            ধ্রুবক bool error = (param->config.strict & PARSER_NO_UNKNOWN_SECTION_FLAGS);
                            parser_log_with_code(
                                param, (error ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
                                XKB_LOG_VERBOSITY_MINIMAL,
                                XKB_ERROR_UNKNOWN_SECTION_FLAG,
                                "Unknown section flag \"%.*s\"%s",
                                (unsigned)$1.len, $1.start,
                                (error ? "" : "; ignored")
                            );
                            যদি (error)
                                YYABORT;
                            $$ = 0;
                        }
                ;

DeclList        :       DeclList Decl
                        {
                            যদি ($2) {
                                যদি ($1.শীর্ষ) {
                                    $$.শীর্ষ = $1.শীর্ষ; $1.last->পরবর্তী = $2; $$.last = $2;
                                } অন্যথা {
                                    $$.শীর্ষ = $$.last = $2;
                                }
                            }
                        }
                        /*
                         * VModDecl is "inlined" directly into DeclList, i.e.
                         * each VModDef in the VModDecl is a separate Decl in
                         * the File.
                         */
                |       DeclList OptMergeMode VModDecl
                        {
                            জন্য (VModDef *vmod = $3.শীর্ষ; vmod; vmod = (VModDef *) vmod->common.পরবর্তী)
                                vmod->merge = $2;
                            যদি ($1.শীর্ষ) {
                                $$.শীর্ষ = $1.শীর্ষ; $1.last->পরবর্তী = &$3.শীর্ষ->common; $$.last = &$3.last->common;
                            } অন্যথা {
                                $$.শীর্ষ = &$3.শীর্ষ->common; $$.last = &$3.last->common;
                            }
                        }
                |       { $$.শীর্ষ = $$.last = ফাঁকা; }
                ;

Decl            :       OptMergeMode VarDecl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                /*      OptMergeMode VModDecl - see above. */
                |       OptMergeMode InterpretDecl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode চাবি_Name_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode চাবি_Alias_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode চাবি_Type_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode SymbolsDecl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode ModMapDecl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode GroupCompatDecl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode LedMapDecl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode LedNameDecl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode ShapeDecl          { $$ = ফাঁকা; }
                |       OptMergeMode SectionDecl        { $$ = ফাঁকা; }
                |       OptMergeMode DoodadDecl         { $$ = ফাঁকা; }
                |       OptMergeMode UnknownDecl
                            { $$ = (ParseCommon *) $2; }
                |       OptMergeMode UnknownCompoundStatementDecl
                            { $$ = (ParseCommon *) $2; }
                |       MergeMode STRING
                        {
                            $$ = (ParseCommon *) IncludeCreate(param->ctx, $2, $1);
                            মুক্ত($2);
                        }
                ;

VarDecl         :       Lhs EQUALS রাশিমালা_2 SEMI
                        { $$ = VarCreate($1, $3); }
                |       Ident SEMI
                        { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident SEMI
                        { $$ = BoolVarCreate($2, false); }
                ;

চাবি_Name_Decl     :       KEYNAME EQUALS চাবি_Code_2 SEMI
                        { $$ = KeycodeCreate($1, $3); }
                ;

চাবি_Alias_Decl    :       ALIAS KEYNAME EQUALS KEYNAME SEMI
                        { $$ = চাবি_Alias_Create($2, $4); }
                ;

VModDecl        :       VIRTUAL_MODS VModDefList SEMI
                        { $$ = $2; }
                ;

VModDefList     :       VModDefList COMMA VModDef
                        { $$.শীর্ষ = $1.শীর্ষ; $$.last->common.পরবর্তী = &$3->common; $$.last = $3; }
                |       VModDef
                        { $$.শীর্ষ = $$.last = $1; }
                ;

VModDef         :       Ident
                        { $$ = VModCreate($1, ফাঁকা); }
                |       Ident EQUALS রাশিমালা_2
                        { $$ = VModCreate($1, $3); }
                ;

InterpretDecl   :       INTERPRET InterpretMatch OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $2->def = $4.শীর্ষ; $$ = $2; }
                ;

InterpretMatch  :       চাবি_Sym_2 PLUS রাশিমালা_2
                        { $$ = InterpCreate($1, $3); }
                |       চাবি_Sym_2
                        { $$ = InterpCreate($1, ফাঁকা); }
                ;

VarDeclList     :       VarDeclList VarDecl
                        {
                            যদি ($2) {
                                যদি ($1.শীর্ষ) {
                                    $$.শীর্ষ = $1.শীর্ষ;
                                    $$.last->common.পরবর্তী = &$2->common;
                                    $$.last = $2;
                                } অন্যথা {
                                    $$.শীর্ষ = $$.last = $2;
                                }
                            }
                        }
                |       { $$.শীর্ষ = $$.last = ফাঁকা; }
                ;

চাবি_Type_Decl     :       TYPE String OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $$ = চাবি_Type_Create($2, $4.শীর্ষ); }
                ;

SymbolsDecl     :       চাবি_3 KEYNAME OBRACE
                            OptSymbolsBody
                        CBRACE SEMI
                        { $$ = SymbolsCreate($2, $4.শীর্ষ); }
                ;

OptSymbolsBody  :       SymbolsBody { $$ = $1; }
                |                   { $$.শীর্ষ = $$.last = ফাঁকা; }
                ;

SymbolsBody     :       SymbolsBody COMMA SymbolsVarDecl
                        { $$.শীর্ষ = $1.শীর্ষ; $$.last->common.পরবর্তী = &$3->common; $$.last = $3; }
                |       SymbolsVarDecl
                        { $$.শীর্ষ = $$.last = $1; }
                ;

SymbolsVarDecl  :       Lhs EQUALS রাশিমালা_2         { $$ = VarCreate($1, $3); }
                |       Lhs EQUALS Multi_চাবি_Sym_Or_Action_List { $$ = VarCreate($1, $3); }
                |       Ident                   { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident            { $$ = BoolVarCreate($2, false); }
                |       Multi_চাবি_Sym_Or_Action_List { $$ = VarCreate(ফাঁকা, $1); }
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
Multi_চাবি_Sym_Or_Action_List
                :       OBRACKET Multi_চাবি_Sym_List CBRACKET
                        { $$ = $2.শীর্ষ; }
                |       OBRACKET No_প্রতীক_Or_Action_List COMMA Multi_চাবি_Sym_List CBRACKET
                        {
                            /* Prepend n times NoSymbol */
                            কাঠামো {রাশিমালা_Def *শীর্ষ; রাশিমালা_Def *last;} list = {
                                .শীর্ষ = $4.শীর্ষ, .last = $4.last
                            };
                            জন্য (uint32_t k = 0; k < $2; k++) {
                                রাশিমালা_Def* ধ্রুবক syms =
                                    রাশিমালা_Create_চাবি_Sym_List(XKB_চাবি_No_প্রতীক);
                                যদি (!syms) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                syms->common.পরবর্তী = &list.শীর্ষ->common;
                                list.শীর্ষ = syms;
                            }
                            $$ = list.শীর্ষ;
                        }
                |       OBRACKET MultiActionList CBRACKET
                        { $$ = $2.শীর্ষ; }
                |       OBRACKET No_প্রতীক_Or_Action_List COMMA MultiActionList CBRACKET
                        {
                            /* Prepend n times NoAction() */
                            কাঠামো {রাশিমালা_Def *শীর্ষ; রাশিমালা_Def *last;} list = {
                                .শীর্ষ = $4.শীর্ষ, .last = $4.last
                            };
                            জন্য (uint32_t k = 0; k < $2; k++) {
                                রাশিমালা_Def* ধ্রুবক acts = রাশিমালা_Create_Action_List(ফাঁকা);
                                যদি (!acts) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                acts->common.পরবর্তী = &list.শীর্ষ->common;
                                list.শীর্ষ = acts;
                            }
                            $$ = list.শীর্ষ;
                        }
                        /*
                         * A list of *empty* keysym/action lists `{}`.
                         * There is not enough context here to decide if it is
                         * a `MultiKeySymList` or a `MultiActionList`.
                         */
                |       OBRACKET No_প্রতীক_Or_Action_List CBRACKET
                        { $$ = রাশিমালা_Empty_List(); }
                ;

/* A list of `{}`, which remains ambiguous until reaching a keysym or action list */
No_প্রতীক_Or_Action_List
                :       No_প্রতীক_Or_Action_List COMMA OBRACE CBRACE
                        { $$ = $1 + 1; }
                |       OBRACE CBRACE
                        { $$ = 1; }
                |       { $$ = 0; }
                ;

GroupCompatDecl :       GROUP Integer EQUALS রাশিমালা_2 SEMI
                        { $$ = GroupCompatCreate($2, $4); }
                ;

ModMapDecl      :       MODIFIER_MAP রাশিমালা_2 OBRACE চাবি_Or_চাবি_Sym_List CBRACE SEMI
                        {
                            যদি (param->config.format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
                                $2->common.প্রকার != STMT_রাশিমালা_IDENT) {
                                    parser_err(
                                        param, XKB_ERROR_INVALID_MODIFIER_MAP_MASK,
                                        "Invalid real modifier mask in modifier "
                                        "map definition: expected identifier"
                                    );
                                    FreeStmt((ParseCommon *) $2);
                                    FreeStmt((ParseCommon *) $4.শীর্ষ);
                                    YYERROR;
                            }
                            $$ = ModMapCreate($2, $4.শীর্ষ);
                        }
                ;

চাবি_Or_চাবি_Sym_List :       চাবি_Or_চাবি_Sym_List COMMA চাবি_Or_চাবি_Sym
                        { $$.শীর্ষ = $1.শীর্ষ; $$.last->common.পরবর্তী = &$3->common; $$.last = $3; }
                |       চাবি_Or_চাবি_Sym
                        { $$.শীর্ষ = $$.last = $1; }
                ;

চাবি_Or_চাবি_Sym     :       KEYNAME
                        { $$ = রাশিমালা_Create_চাবি_Name($1); }
                |       চাবি_Sym_2
                        { $$ = রাশিমালা_Create_চাবি_Sym($1); }
                ;

LedMapDecl:             INDICATOR String OBRACE VarDeclList CBRACE SEMI
                        { $$ = LedMapCreate($2, $4.শীর্ষ); }
                ;

LedNameDecl:            INDICATOR Integer EQUALS রাশিমালা_2 SEMI
                        { $$ = LedNameCreate($2, $4, false); }
                |       VIRTUAL INDICATOR Integer EQUALS রাশিমালা_2 SEMI
                        { $$ = LedNameCreate($3, $5, true); }
                ;

UnknownDecl     :       IDENT Terminal EQUALS রাশিমালা_2 SEMI
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
                            FreeStmt((ParseCommon *) $4.শীর্ষ);
                            $$ = UnknownStatementCreate(STMT_UNKNOWN_COMPOUND, $1);
                        }
                ;

ShapeDecl       :       SHAPE String OBRACE OutlineList CBRACE SEMI
                        { $$ = ফাঁকা; }
                |       SHAPE String OBRACE CoordList CBRACE SEMI
                        { (শূন্য) $4; $$ = ফাঁকা; }
                ;

SectionDecl     :       SECTION String OBRACE SectionBody CBRACE SEMI
                        { $$ = ফাঁকা; }
                ;

SectionBody     :       SectionBody SectionBodyItem     { $$ = ফাঁকা;}
                |       SectionBodyItem                 { $$ = ফাঁকা; }
                ;

SectionBodyItem :       ROW OBRACE RowBody CBRACE SEMI
                        { $$ = ফাঁকা; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ফাঁকা; }
                |       DoodadDecl
                        { $$ = ফাঁকা; }
                |       LedMapDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ফাঁকা; }
                |       OverlayDecl
                        { $$ = ফাঁকা; }
                ;

RowBody         :       RowBody RowBodyItem     { $$ = ফাঁকা;}
                |       RowBodyItem             { $$ = ফাঁকা; }
                ;

RowBodyItem     :       চাবিগুলি_2 OBRACE চাবিগুলি_3 CBRACE SEMI { $$ = ফাঁকা; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ফাঁকা; }
                ;

চাবিগুলি_3            :       চাবিগুলি_3 COMMA চাবি_4          { $$ = ফাঁকা; }
                |       চাবি_4                     { $$ = ফাঁকা; }
                ;

চাবি_4             :       KEYNAME
                        { $$ = ফাঁকা; }
                |       OBRACE রাশিমালা_List_2 CBRACE
                        { FreeStmt((ParseCommon *) $2.শীর্ষ); $$ = ফাঁকা; }
                ;

OverlayDecl     :       OVERLAY String OBRACE Overlay_চাবি_List CBRACE SEMI
                        { $$ = ফাঁকা; }
                ;

Overlay_চাবি_List  :       Overlay_চাবি_List COMMA Overlay_চাবি { $$ = ফাঁকা; }
                |       Overlay_চাবি                      { $$ = ফাঁকা; }
                ;

Overlay_চাবি      :       KEYNAME EQUALS KEYNAME          { $$ = ফাঁকা; }
                ;

OutlineList     :       OutlineList COMMA OutlineInList
                        { $$ = ফাঁকা;}
                |       OutlineInList
                        { $$ = ফাঁকা; }
                ;

OutlineInList   :       OBRACE CoordList CBRACE
                        { (শূন্য) $2; $$ = ফাঁকা; }
                |       Ident EQUALS OBRACE CoordList CBRACE
                        { (শূন্য) $4; $$ = ফাঁকা; }
                |       Ident EQUALS রাশিমালা_2
                        { FreeStmt((ParseCommon *) $3); $$ = ফাঁকা; }
                ;

CoordList       :       CoordList COMMA Coord
                        { (শূন্য) $1; (শূন্য) $3; $$ = ফাঁকা; }
                |       Coord
                        { (শূন্য) $1; $$ = ফাঁকা; }
                ;

Coord           :       OBRACKET Signed_সংখ্যা COMMA Signed_সংখ্যা CBRACKET
                        { $$ = ফাঁকা; }
                ;

DoodadDecl      :       DoodadType String OBRACE VarDeclList CBRACE SEMI
                        { FreeStmt((ParseCommon *) $4.শীর্ষ); $$ = ফাঁকা; }
                ;

DoodadType      :       TEXT    { $$ = 0; }
                |       OUTLINE { $$ = 0; }
                |       SOLID   { $$ = 0; }
                |       LOGO    { $$ = 0; }
                ;

FieldSpec       :       Ident   { $$ = $1; }
                |       Element { $$ = $1; }
                ;

Element         :       ACTION_TOK
                        { $$ = xkb_atom_intern_literal(param->ctx, "action"); }
                |       INTERPRET
                        { $$ = xkb_atom_intern_literal(param->ctx, "interpret"); }
                |       TYPE
                        { $$ = xkb_atom_intern_literal(param->ctx, "type"); }
                |       চাবি_3
                        { $$ = xkb_atom_intern_literal(param->ctx, "key"); }
                |       GROUP
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

OptMergeMode    :       MergeMode       { $$ = $1; }
                |                       { $$ = MERGE_DEFAULT; }
                ;

MergeMode       :       INCLUDE         { $$ = MERGE_DEFAULT; }
                |       AUGMENT         { $$ = MERGE_AUGMENT; }
                |       OVERRIDE        { $$ = MERGE_OVERRIDE; }
                |       REPLACE         { $$ = MERGE_REPLACE; }
                |       ALTERNATE
                {
                    /*
                     * This used to be MERGE_ALT_FORM. This functionality was
                     * unused and has been removed.
                     */
                    parser_warn(param, XKB_LOG_MESSAGE_NO_ID,
                                "ignored unsupported legacy merge mode \"alternate\"");
                    $$ = MERGE_DEFAULT;
                }
                ;

রাশিমালা_List_2        :       রাশিমালা_List_2 COMMA রাশিমালা_2
                        {
                            যদি ($3) {
                                যদি ($1.শীর্ষ) {
                                    $$.শীর্ষ = $1.শীর্ষ;
                                    $$.last->common.পরবর্তী = &$3->common;
                                    $$.last = $3;
                                } অন্যথা {
                                    $$.শীর্ষ = $$.last = $3;
                                }
                            }
                        }
                |       রাশিমালা_2
                        { $$.শীর্ষ = $$.last = $1; }
                |       { $$.শীর্ষ = $$.last = ফাঁকা; }
                ;

রাশিমালা_2            :       রাশিমালা_2 DIVIDE রাশিমালা_2
                        { $$ = রাশিমালা_Create_দ্বিমিক(STMT_রাশিমালা_DIVIDE, $1, $3); }
                |       রাশিমালা_2 PLUS রাশিমালা_2
                        { $$ = রাশিমালা_Create_দ্বিমিক(STMT_রাশিমালা_ADD, $1, $3); }
                |       রাশিমালা_2 MINUS রাশিমালা_2
                        { $$ = রাশিমালা_Create_দ্বিমিক(STMT_রাশিমালা_SUBTRACT, $1, $3); }
                |       রাশিমালা_2 TIMES রাশিমালা_2
                        { $$ = রাশিমালা_Create_দ্বিমিক(STMT_রাশিমালা_MULTIPLY, $1, $3); }
                |       Lhs EQUALS রাশিমালা_2
                        { $$ = রাশিমালা_Create_দ্বিমিক(STMT_রাশিমালা_ASSIGN, $1, $3); }
                |       Term
                        { $$ = $1; }
                ;

Term            :       MINUS Term
                        { $$ = রাশিমালা_Create_Unary(STMT_রাশিমালা_NEGATE, $2); }
                |       PLUS Term
                        { $$ = রাশিমালা_Create_Unary(STMT_রাশিমালা_UNARY_PLUS, $2); }
                |       EXCLAM Term
                        { $$ = রাশিমালা_Create_Unary(STMT_রাশিমালা_NOT, $2); }
                |       INVERT Term
                        { $$ = রাশিমালা_Create_Unary(STMT_রাশিমালা_INVERT, $2); }
                |       Lhs
                        { $$ = $1; }
                |       FieldSpec OPAREN রাশিমালা_List_2 CPAREN %prec OPAREN
                        { $$ = রাশিমালা_Create_Action($1, $3.শীর্ষ); }
                |       Actions
                        { $$ = $1; }
                |       Terminal
                        { $$ = $1; }
                |       OPAREN রাশিমালা_2 CPAREN
                        { $$ = $2; }
                ;

MultiActionList :       MultiActionList COMMA Action
                        {
                            রাশিমালা_Def *রাশিমালা = রাশিমালা_Create_Action_List($3);
                            $$ = $1;
                            $$.last->common.পরবর্তী = &রাশিমালা->common; $$.last = রাশিমালা;
                        }
                |       MultiActionList COMMA Actions
                        { $$ = $1; $$.last->common.পরবর্তী = &$3->common; $$.last = $3; }
                |       Action
                        { $$.শীর্ষ = $$.last = রাশিমালা_Create_Action_List($1); }
                |       NonEmptyActions
                        { $$.শীর্ষ = $$.last = $1; }
                ;

ActionList      :       ActionList COMMA Action
                        { $$ = $1; $$.last->common.পরবর্তী = &$3->common; $$.last = $3; }
                |       Action
                        { $$.শীর্ষ = $$.last = $1; }
                ;

NonEmptyActions :       OBRACE ActionList CBRACE
                        { $$ = রাশিমালা_Create_Action_List($2.শীর্ষ); }
                ;

Actions         :       NonEmptyActions
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = রাশিমালা_Create_Action_List(ফাঁকা); }
                ;

Action          :       FieldSpec OPAREN রাশিমালা_List_2 CPAREN
                        { $$ = রাশিমালা_Create_Action($1, $3.শীর্ষ); }
                ;

Lhs             :       FieldSpec
                        { $$ = রাশিমালা_Create_Ident($1); }
                |       FieldSpec DOT FieldSpec
                        { $$ = রাশিমালা_Create_Field_Ref($1, $3); }
                |       FieldSpec OBRACKET রাশিমালা_2 CBRACKET
                        { $$ = রাশিমালা_Create_Array_Ref(XKB_ATOM_NONE, $1, $3); }
                |       FieldSpec DOT FieldSpec OBRACKET রাশিমালা_2 CBRACKET
                        { $$ = রাশিমালা_Create_Array_Ref($1, $3, $5); }
                ;

OptTerminal     :       Terminal
                        { $$ = $1; }
                |       { $$ = ফাঁকা; }
                ;

Terminal        :       String
                        { $$ = রাশিমালা_Create_String($1); }
                |       Integer
                        { $$ = রাশিমালা_Create_Integer($1); }
                |       Float
                        { $$ = রাশিমালা_Create_Float(/* Discard $1 */); }
                |       KEYNAME
                        { $$ = রাশিমালা_Create_চাবি_Name($1); }
                ;

Multi_চাবি_Sym_List :       Multi_চাবি_Sym_List COMMA চাবি_Sym_Lit
                        {
                            রাশিমালা_Def *রাশিমালা = রাশিমালা_Create_চাবি_Sym_List($3);
                            $$ = $1;
                            $$.last->common.পরবর্তী = &রাশিমালা->common; $$.last = রাশিমালা;
                        }
                |       Multi_চাবি_Sym_List COMMA চাবি_Syms
                        { $$ = $1; $$.last->common.পরবর্তী = &$3->common; $$.last = $3; }
                |       চাবি_Sym_Lit
                        { $$.শীর্ষ = $$.last = রাশিমালা_Create_চাবি_Sym_List($1); }
                |       Non_Empty_চাবি_Syms
                        { $$.শীর্ষ = $$.last = $1; }
                ;

চাবি_Sym_List      :       চাবি_Sym_List COMMA চাবি_Sym_Lit
                        { $$ = রাশিমালা_Append_চাবি_Sym_List($1, $3); }
                |       চাবি_Sym_List COMMA STRING
                        {
                            $$ = রাশিমালা_চাবি_Sym_List_Append_String(param->scanner, $1, $3);
                            মুক্ত($3);
                            যদি (!$$)
                                YYERROR;
                        }
                |       চাবি_Sym_Lit
                        {
                            $$ = রাশিমালা_Create_চাবি_Sym_List($1);
                            যদি (!$$)
                                YYERROR;
                        }
                |       STRING
                        {
                            $$ = রাশিমালা_Create_চাবি_Sym_List(XKB_চাবি_No_প্রতীক);
                            যদি (!$$)
                                YYERROR;
                            $$ = রাশিমালা_চাবি_Sym_List_Append_String(param->scanner, $$, $1);
                            মুক্ত($1);
                            যদি (!$$)
                                YYERROR;
                        }
                ;

Non_Empty_চাবি_Syms :       OBRACE চাবি_Sym_List CBRACE
                        { $$ = $2; }
                |       STRING
                        {
                            $$ = রাশিমালা_Create_চাবি_Sym_List(XKB_চাবি_No_প্রতীক);
                            যদি (!$$)
                                YYERROR;
                            $$ = রাশিমালা_চাবি_Sym_List_Append_String(param->scanner, $$, $1);
                            মুক্ত($1);
                            যদি (!$$)
                                YYERROR;
                        }
                ;

চাবি_Syms         :       Non_Empty_চাবি_Syms
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = রাশিমালা_Create_চাবি_Sym_List(XKB_চাবি_No_প্রতীক); }
                ;

চাবি_Sym_2          :       চাবি_Sym_Lit
                        { $$ = $1; }
                |       STRING
                        {
                            $$ = KeysymParseString(param->scanner, $1);
                            মুক্ত($1);
                            যদি ($$ == XKB_চাবি_No_প্রতীক)
                                YYERROR;
                        }
                ;

চাবি_Sym_Lit       :       IDENT
                        {
                            যদি (!resolve_keysym(param, $1, &$$)) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (unsigned পূর্ণসংখ্যা) $1.len, $1.start
                                );
                                $$ = XKB_চাবি_No_প্রতীক;
                            }
                        }
                        /* Handle keysym that is also a keyword  */
                |       SECTION { $$ = XKB_চাবি_section; }
                |       DECIMAL_DIGIT
                        {
                            /*
                             * Special case for digits 0..9:
                             * map to XKB_KEY_0 .. XKB_KEY_9, consistent with
                             * other keysym names: <name> → XKB_KEY_<name>.
                             */
                            $$ = XKB_চাবি_0 + (xkb_keysym_t) $1;
                        }
                |       INTEGER
                        {
                            যদি ($1 < XKB_KEYSYM_MIN) {
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
                                $$ = XKB_চাবি_No_প্রতীক;
                            }
                            /*
                             * Integers 0..9 are handled with DECIMAL_DIGIT if
                             * they were formatted as single characters '0'..'9'.
                             * Otherwise they are handled here as raw keysyms
                             * values. E.g. `01` and `0x1` are interpreted as
                             * the keysym 0x0001, while `1` is interpreted as
                             * XKB_KEY_1.
                             */
                            অন্যথা {
                                /* Any other numeric value */
                                যদি ($1 <= XKB_KEYSYM_MAX) {
                                    /*
                                     * Valid keysym
                                     * No normalization is performed and value
                                     * is used as is.
                                     */
                                    $$ = (xkb_keysym_t) $1;
                                    check_deprecated_keysyms(
                                        parser_warn, param, param->ctx,
                                        $$, ফাঁকা, $$, "%#06"PRIx32, "");
                                } অন্যথা {
                                    /* Invalid keysym */
                                    parser_warn(
                                        param, XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                        "unrecognized keysym \"%#06"PRIx64"\" "
                                        "(%"PRId64")", $1, $1
                                    );
                                    $$ = XKB_চাবি_No_প্রতীক;
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

Signed_সংখ্যা    :       MINUS সংখ্যা    { $$ = -$2; }
                |       সংখ্যা          { $$ = $1; }
                ;

সংখ্যা          :       FLOAT         { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                |       INTEGER       { $$ = $1; }
                ;

Float           :       FLOAT   { $$ = 0; }
                ;

Integer         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

চাবি_Code_2         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

Ident           :       IDENT   { $$ = xkb_atom_intern(param->ctx, $1.start, $1.len); }
                |       DEFAULT { $$ = xkb_atom_intern_literal(param->ctx, "default"); }
                ;

String          :       STRING  { $$ = xkb_atom_intern(param->ctx, $1, strlen($1)); মুক্ত($1); }
                ;

OptMapName      :       MapName { $$ = $1; }
                |               { $$ = ফাঁকা; }
                ;

MapName         :       STRING  { $$ = $1; }
                ;

%%

/* Parse a specific section */
XkbFile *
parse(কাঠামো xkb_context *ctx, ধ্রুবক কাঠামো parser_keymap_config *config,
      কাঠামো scanner *scanner, ধ্রুবক অক্ষর *map)
{
    পূর্ণসংখ্যা ret;
    XkbFile *first = ফাঁকা;
    কাঠামো parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = ফাঁকা,
        .config = *config,
        .more_maps = false,
    };

    /*
     * If we got a specific map, we look for it exclusively and return
     * immediately upon finding it. Otherwise, we need to get the
     * default map. If we find a map marked as default, we return it
     * immediately. If there are no maps marked as default, we return
     * the first map in the file.
     */

    যখন ((ret = yyparse(&param)) == 0 && param.more_maps) {
        যদি (map) {
            যদি (streq_not_null(map, param.rtrn->name))
                ফেরত param.rtrn;
            অন্যথা
                FreeXkbFile(param.rtrn);
        }
        অন্যথা {
            যদি (param.rtrn->flags & MAP_IS_DEFAULT) {
                FreeXkbFile(first);
                ফেরত param.rtrn;
            }
            অন্যথা যদি (!first) {
                first = param.rtrn;
            }
            অন্যথা {
                FreeXkbFile(param.rtrn);
            }
        }
        param.rtrn = ফাঁকা;
    }

    যদি (ret != 0) {
        /* Some error happend; clear the Xkbfiles parsed so far */
        FreeXkbFile(first);
        FreeXkbFile(param.rtrn);
        ফেরত ফাঁকা;
    }

    /*
     * Warn about implicit default section,
     * but only if not a keymap: multiple keymaps per file not supported
     */
    যদি (first && first->file_type != FILE_TYPE_KEYMAP)
        log_vrb(ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_MISSING_DEFAULT_SECTION,
                "No section name in include statement, but \"%s\" contains several; "
                "Using first defined section, \"%s\"\n",
                scanner->file_name, safe_map_name(first->name));

    ফেরত first;
}

/* Parse the next section */
bool
parse_পরবর্তী(কাঠামো xkb_context *ctx,
           ধ্রুবক কাঠামো parser_keymap_config *config,
           কাঠামো scanner *scanner, XkbFile **xkb_file)
{
    পূর্ণসংখ্যা ret;
    কাঠামো parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = ফাঁকা,
        .config = *config,
        .more_maps = false,
    };

    যদি ((ret = yyparse(&param)) == 0 && param.more_maps) {
        *xkb_file = param.rtrn;
        ফেরত true;
    } অন্যথা {
        FreeXkbFile(param.rtrn);
        *xkb_file = ফাঁকা;
        ফেরত (ret == 0);
    }
}
