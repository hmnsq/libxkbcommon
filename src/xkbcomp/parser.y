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
#ضمن "config.h"

#ضمن "scanner-utils.h"
#ضمن "xkbcomp/ast.h"
#ضمن "xkbcomp/xkbcomp-priv.h"
}

%{
#ضمن "config.h"

#ضمن <assert.h>

#ضمن "keysym.h"
#ضمن "xkbcomp/xkbcomp-priv.h"
#ضمن "xkbcomp/parser-priv.h"
#ضمن "xkbcomp/ast-build.h"

بنية parser_param {
    بنية xkb_context *ctx;
    بنية scanner *scanner;
    XkbFile *rtrn;
    بنية parser_keymap_config config;
    bool ہور_maps;
};

#تعریف_کرو parser_log_with_کوڈ(param, level, verbosity, log_پیغام_id, fmt, ...)   \
    scanner_log_with_کوڈ((param)->scanner, level, verbosity, log_پیغام_id, fmt,\
                          ##__VA_ARGS__)

#تعریف_کرو parser_err(param, error_id, fmt, ...) \
    scanner_err((param)->scanner, error_id, fmt, ##__VA_ARGS__)

#تعریف_کرو parser_warn(param, warning_id, fmt, ...) \
    scanner_warn((param)->scanner, warning_id, fmt, ##__VA_ARGS__)

#تعریف_کرو parser_vrb(param, verbosity, warning_id, fmt, ...) \
    scanner_vrb((param)->scanner, verbosity, warning_id, fmt, ##__VA_ARGS__)

ساكن فراغ
_xkbcommon_error(بنية parser_param *param, ثابت حرف *پیغام)
{
    parser_err(param, XKB_ERROR_INVALID_XKB_SYNTAX, "%s", پیغام);
}

ساكن bool
resolve_keysym(بنية parser_param *param, بنية sval name, xkb_keysym_t *sym_rtrn)
{
    xkb_keysym_t sym;

    إذا (isvaleq(name, SVAL_LIT("any")) || isvaleq(name, SVAL_LIT("nosymbol"))) {
        *sym_rtrn = XKB_کلید_No_علامت;
        أرجع true;
    }

    إذا (isvaleq(name, SVAL_LIT("none")) || isvaleq(name, SVAL_LIT("voidsymbol"))) {
        *sym_rtrn = XKB_کلید_Void_علامت;
        أرجع true;
    }

    /* xkb_keysym_from_name needs a C string. */
    حرف buf[XKB_KEYSYM_NAME_MAX_SIZE];
    إذا (name.len >= حجم(buf)) {
        أرجع false;
    }
    حافظہ_نقل(buf, name.شروع, name.len);
    buf[name.len] = '\0';

    sym = xkb_keysym_from_name(buf, XKB_KEYSYM_NO_FLAGS);
    إذا (sym != XKB_کلید_No_علامت) {
        *sym_rtrn = sym;
        check_deprecated_keysyms(parser_warn, param, param->ctx,
                                 sym, buf, buf, "%s", "");
        أرجع true;
    }

    أرجع false;
}

#تعریف_کرو param_scanner param->scanner
%}

%define api.خالص
%lex-param      { بنية scanner *param_scanner }
%parse-param    { بنية parser_param *param }

%define parse.error detailed

%token
        سرا_دا_FILE     0 "end of file"
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
        کلید_3             24 "key"
        ALIAS           25 "alias"
        گروہ_2           26 "group"
        MODIFIER_MAP    27 "modifier_map"
        INDICATOR       28 "indicator"
        SHAPE           29 "shape"
        کلیدیں_2            30 "keys"
        صف             31 "row"
        SECTION         32 "section"
        OVERLAY         33 "overlay"
        متن            34 "text"
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
        ALPHANUMERIC_کلیدیں       73 "alphanumeric_keys"
        MODIFIER_کلیدیں           74 "modifier_keys"
        KEYPAD_کلیدیں             75 "keypad_keys"
        FUNCTION_کلیدیں           76 "function_keys"
        ALTERNATE_گروہ         77 "alternate_group"
        DEPRECATED              78 "deprecated"

%سجے  EQUALS
%کھبے   PLUS MINUS
%کھبے   TIMES DIVIDE
%کھبے   EXCLAM INVERT
%کھبے   OPAREN

%start  XkbFile

%اتحاد  {
        int64_t          عدد;
        تعداد xkb_file_type file_type;
        حرف            *str;
        بنية sval     sval;
        xkb_atom_t      atom;
        تعداد ملاؤ_منوال ملاؤ;
        تعداد xkb_map_flags mapFlags;
        xkb_keysym_t    keysym;
        ParseCommon     *any;
        بنية { ParseCommon *سر; ParseCommon *آخری; } anyList;
        uint32_t        no_علامت_Or_Action_List;
        اظہاریہ_Def         *اظہاریہ;
        بنية { اظہاریہ_Def *سر; اظہاریہ_Def *آخری; } اظہاریہ_List;
        VarDef          *var;
        بنية { VarDef *سر; VarDef *آخری; } varList;
        VModDef         *vmod;
        بنية { VModDef *سر; VModDef *آخری; } vmodList;
        InterpDef       *interp;
        کلید_Type_Def      *کلید_Type;
        SymbolsDef      *syms;
        ModMapDef       *mod_نقاب_2;
        گروہ_Compat_Def  *گروہ_Compat;
        LedMapDef       *ledMap;
        LedNameDef      *ledName;
        KeycodeDef      *کلید_کوڈ;
        کلید_Alias_Def     *کلید_Alias;
        UnknownStatement *unknown;
        فراغ            *geom;
        XkbFile         *file;
        بنية { XkbFile *سر; XkbFile *آخری; } fileList;
}

%نوع <عدد>     DECIMAL_DIGIT INTEGER FLOAT
%نوع <str>     STRING
%نوع <sval>    IDENT
%نوع <atom>    KEYNAME
%نوع <عدد>     کلید_کوڈ_2 نمبر Integer Float Signed_نمبر DoodadType
%نوع <ملاؤ>   ملاؤ_منوال_2 Opt_ملاؤ_منوال
%نوع <file_type> Xkb_مرکب_Type FileType
%نوع <mapFlags> Flag Flags OptFlags
%نوع <str>     MapName OptMapName
%نوع <atom>    میدان_Spec Ident Element String
%نوع <keysym>  کلید_Sym_2 کلید_Sym_Lit
%نوع <no_علامت_Or_Action_List> No_علامت_Or_Action_List
%نوع <any>     Decl
%نوع <anyList> DeclList
%نوع <اظہاریہ>    اظہاریہ_2 رکن Lhs Terminal OptTerminal Coord CoordList
%نوع <اظہاریہ>    Multi_کلید_Sym_Or_Action_List Non_خالی_تھاواں_Actions Actions Action
%نوع <اظہاریہ>    کلید_Syms Non_خالی_تھاواں_کلید_Syms کلید_Sym_List کلید_Or_کلید_Sym
%نوع <اظہاریہ_List> اظہاریہ_List_2 کلید_Or_کلید_Sym_List
%نوع <اظہاریہ_List> ActionList MultiActionList Multi_کلید_Sym_List
%نوع <var>     VarDecl SymbolsVarDecl
%نوع <varList> VarDeclList SymbolsBody OptSymbolsBody
%نوع <vmod>    VModDef
%نوع <vmodList> VModDefList VModDecl
%نوع <interp>  InterpretDecl Interpret_میل
%نوع <کلید_Type> کلید_Type_Decl
%نوع <syms>    SymbolsDecl
%نوع <mod_نقاب_2> ModMapDecl
%نوع <گروہ_Compat> گروہ_Compat_Decl
%نوع <ledMap>  LedMapDecl
%نوع <ledName> LedNameDecl
%نوع <کلید_کوڈ> کلید_Name_Decl
%نوع <کلید_Alias> کلید_Alias_Decl
%نوع <geom>    ShapeDecl SectionDecl SectionBody Section_Body_شے صف_Body صف_Body_شے
%نوع <geom>    کلیدیں_3 کلید_4 OverlayDecl Overlay_کلید_List Overlay_کلید OutlineList Outline_وچ_List
%نوع <geom>    DoodadDecl
%نوع <unknown> UnknownDecl UnknownCompoundStatementDecl
%نوع <file>    XkbFile XkbMapConfig
%نوع <fileList> XkbMapConfigList
%نوع <file>    Xkb_مرکب_Map

%destructor { FreeStmt((ParseCommon *) $$); }
    <any> <اظہاریہ> <var> <vmod> <interp> <کلید_Type> <syms> <mod_نقاب_2> <گروہ_Compat>
    <ledMap> <ledName> <کلید_کوڈ> <کلید_Alias>
%destructor { FreeStmt((ParseCommon *) $$.سر); }
    <anyList> <اظہاریہ_List> <varList> <vmodList>
/* The destructor also runs on the start symbol when the parser *succeeds*.
 * The `if` here catches this case. */
%destructor { إذا (!param->rtrn) FreeXkbFile($$); } <file>
%destructor { FreeXkbFile($$.سر); } <fileList>
%destructor { حرر($$); } <str>

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

XkbFile         :       Xkb_مرکب_Map
                        { $$ = param->rtrn = $1; param->ہور_maps = !!param->rtrn; (فراغ) yynerrs; }
                |       XkbMapConfig
                        { $$ = param->rtrn = $1; param->ہور_maps = !!param->rtrn; YYACCEPT; }
                |       سرا_دا_FILE
                        { $$ = param->rtrn = عدم; param->ہور_maps = false; }
                ;

Xkb_مرکب_Map :       OptFlags Xkb_مرکب_Type OptMapName OBRACE
                            XkbMapConfigList
                        CBRACE SEMI
                        { $$ = XkbFileCreate($2, $3, (ParseCommon *) $5.سر, $1); }
                ;

Xkb_مرکب_Type:       XKB_KEYMAP      { $$ = FILE_TYPE_KEYMAP; }
                |       XKB_SEMANTICS   { $$ = FILE_TYPE_KEYMAP; }
                |       XKB_LAYOUT      { $$ = FILE_TYPE_KEYMAP; }
                ;

/*
 * NOTE: Any component is optional
 */
XkbMapConfigList :      XkbMapConfigList XkbMapConfig
                        {
                            إذا ($2) {
                                إذا ($1.سر) {
                                    $$.سر = $1.سر;
                                    $$.آخری->common.اگلا = &$2->common;
                                    $$.آخری = $2;
                                } وإلا {
                                    $$.سر = $$.آخری = $2;
                                }
                            }
                        }
                |       { $$.سر = $$.آخری = عدم; }
                ;

XkbMapConfig    :       OptFlags FileType OptMapName OBRACE
                            DeclList
                        CBRACE SEMI
                        {
                            إذا ($1 & MAP_IS_DEPRECATED) {
                                parser_warn(param, XKB_WARNING_DEPRECATED_SECTION,
                                            "deprecated section: \"%s\"",
                                            محفوظ_map_name($3));
                            }
                            $$ = XkbFileCreate($2, $3, $5.سر, $1);
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
                |       ALPHANUMERIC_کلیدیں       { $$ = MAP_HAS_ALPHANUMERIC; }
                |       MODIFIER_کلیدیں           { $$ = MAP_HAS_MODIFIER; }
                |       KEYPAD_کلیدیں             { $$ = MAP_HAS_KEYPAD; }
                |       FUNCTION_کلیدیں           { $$ = MAP_HAS_FN; }
                |       ALTERNATE_گروہ         { $$ = MAP_IS_ALTGR; }
                |       DEPRECATED              { $$ = MAP_IS_DEPRECATED; }
                |       IDENT
                        {
                            ثابت bool error = (param->config.strict & PARSER_NO_UNKNOWN_SECTION_FLAGS);
                            parser_log_with_کوڈ(
                                param, (error ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
                                XKB_LOG_VERBOSITY_اقل,
                                XKB_ERROR_UNKNOWN_SECTION_FLAG,
                                "Unknown section flag \"%.*s\"%s",
                                (بنانشانی)$1.len, $1.شروع,
                                (error ? "" : "; ignored")
                            );
                            إذا (error)
                                YYABORT;
                            $$ = 0;
                        }
                ;

DeclList        :       DeclList Decl
                        {
                            إذا ($2) {
                                إذا ($1.سر) {
                                    $$.سر = $1.سر; $1.آخری->اگلا = $2; $$.آخری = $2;
                                } وإلا {
                                    $$.سر = $$.آخری = $2;
                                }
                            }
                        }
                        /*
                         * VModDecl is "inlined" directly into DeclList, i.e.
                         * each VModDef in the VModDecl is a separate Decl in
                         * the File.
                         */
                |       DeclList Opt_ملاؤ_منوال VModDecl
                        {
                            لكل (VModDef *vmod = $3.سر; vmod; vmod = (VModDef *) vmod->common.اگلا)
                                vmod->ملاؤ = $2;
                            إذا ($1.سر) {
                                $$.سر = $1.سر; $1.آخری->اگلا = &$3.سر->common; $$.آخری = &$3.آخری->common;
                            } وإلا {
                                $$.سر = &$3.سر->common; $$.آخری = &$3.آخری->common;
                            }
                        }
                |       { $$.سر = $$.آخری = عدم; }
                ;

Decl            :       Opt_ملاؤ_منوال VarDecl
                        {
                            $2->ملاؤ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                /*      OptMergeMode VModDecl - see above. */
                |       Opt_ملاؤ_منوال InterpretDecl
                        {
                            $2->ملاؤ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ملاؤ_منوال کلید_Name_Decl
                        {
                            $2->ملاؤ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ملاؤ_منوال کلید_Alias_Decl
                        {
                            $2->ملاؤ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ملاؤ_منوال کلید_Type_Decl
                        {
                            $2->ملاؤ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ملاؤ_منوال SymbolsDecl
                        {
                            $2->ملاؤ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ملاؤ_منوال ModMapDecl
                        {
                            $2->ملاؤ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ملاؤ_منوال گروہ_Compat_Decl
                        {
                            $2->ملاؤ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ملاؤ_منوال LedMapDecl
                        {
                            $2->ملاؤ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ملاؤ_منوال LedNameDecl
                        {
                            $2->ملاؤ = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ملاؤ_منوال ShapeDecl          { $$ = عدم; }
                |       Opt_ملاؤ_منوال SectionDecl        { $$ = عدم; }
                |       Opt_ملاؤ_منوال DoodadDecl         { $$ = عدم; }
                |       Opt_ملاؤ_منوال UnknownDecl
                            { $$ = (ParseCommon *) $2; }
                |       Opt_ملاؤ_منوال UnknownCompoundStatementDecl
                            { $$ = (ParseCommon *) $2; }
                |       ملاؤ_منوال_2 STRING
                        {
                            $$ = (ParseCommon *) IncludeCreate(param->ctx, $2, $1);
                            حرر($2);
                        }
                ;

VarDecl         :       Lhs EQUALS اظہاریہ_2 SEMI
                        { $$ = VarCreate($1, $3); }
                |       Ident SEMI
                        { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident SEMI
                        { $$ = BoolVarCreate($2, false); }
                ;

کلید_Name_Decl     :       KEYNAME EQUALS کلید_کوڈ_2 SEMI
                        { $$ = KeycodeCreate($1, $3); }
                ;

کلید_Alias_Decl    :       ALIAS KEYNAME EQUALS KEYNAME SEMI
                        { $$ = کلید_Alias_Create($2, $4); }
                ;

VModDecl        :       VIRTUAL_MODS VModDefList SEMI
                        { $$ = $2; }
                ;

VModDefList     :       VModDefList COMMA VModDef
                        { $$.سر = $1.سر; $$.آخری->common.اگلا = &$3->common; $$.آخری = $3; }
                |       VModDef
                        { $$.سر = $$.آخری = $1; }
                ;

VModDef         :       Ident
                        { $$ = VModCreate($1, عدم); }
                |       Ident EQUALS اظہاریہ_2
                        { $$ = VModCreate($1, $3); }
                ;

InterpretDecl   :       INTERPRET Interpret_میل OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $2->def = $4.سر; $$ = $2; }
                ;

Interpret_میل  :       کلید_Sym_2 PLUS اظہاریہ_2
                        { $$ = InterpCreate($1, $3); }
                |       کلید_Sym_2
                        { $$ = InterpCreate($1, عدم); }
                ;

VarDeclList     :       VarDeclList VarDecl
                        {
                            إذا ($2) {
                                إذا ($1.سر) {
                                    $$.سر = $1.سر;
                                    $$.آخری->common.اگلا = &$2->common;
                                    $$.آخری = $2;
                                } وإلا {
                                    $$.سر = $$.آخری = $2;
                                }
                            }
                        }
                |       { $$.سر = $$.آخری = عدم; }
                ;

کلید_Type_Decl     :       TYPE String OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $$ = کلید_Type_Create($2, $4.سر); }
                ;

SymbolsDecl     :       کلید_3 KEYNAME OBRACE
                            OptSymbolsBody
                        CBRACE SEMI
                        { $$ = SymbolsCreate($2, $4.سر); }
                ;

OptSymbolsBody  :       SymbolsBody { $$ = $1; }
                |                   { $$.سر = $$.آخری = عدم; }
                ;

SymbolsBody     :       SymbolsBody COMMA SymbolsVarDecl
                        { $$.سر = $1.سر; $$.آخری->common.اگلا = &$3->common; $$.آخری = $3; }
                |       SymbolsVarDecl
                        { $$.سر = $$.آخری = $1; }
                ;

SymbolsVarDecl  :       Lhs EQUALS اظہاریہ_2         { $$ = VarCreate($1, $3); }
                |       Lhs EQUALS Multi_کلید_Sym_Or_Action_List { $$ = VarCreate($1, $3); }
                |       Ident                   { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident            { $$ = BoolVarCreate($2, false); }
                |       Multi_کلید_Sym_Or_Action_List { $$ = VarCreate(عدم, $1); }
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
Multi_کلید_Sym_Or_Action_List
                :       OBRACKET Multi_کلید_Sym_List CBRACKET
                        { $$ = $2.سر; }
                |       OBRACKET No_علامت_Or_Action_List COMMA Multi_کلید_Sym_List CBRACKET
                        {
                            /* Prepend n times NoSymbol */
                            بنية {اظہاریہ_Def *سر; اظہاریہ_Def *آخری;} list = {
                                .سر = $4.سر, .آخری = $4.آخری
                            };
                            لكل (uint32_t k = 0; k < $2; k++) {
                                اظہاریہ_Def* ثابت syms =
                                    اظہاریہ_Create_کلید_Sym_List(XKB_کلید_No_علامت);
                                إذا (!syms) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                syms->common.اگلا = &list.سر->common;
                                list.سر = syms;
                            }
                            $$ = list.سر;
                        }
                |       OBRACKET MultiActionList CBRACKET
                        { $$ = $2.سر; }
                |       OBRACKET No_علامت_Or_Action_List COMMA MultiActionList CBRACKET
                        {
                            /* Prepend n times NoAction() */
                            بنية {اظہاریہ_Def *سر; اظہاریہ_Def *آخری;} list = {
                                .سر = $4.سر, .آخری = $4.آخری
                            };
                            لكل (uint32_t k = 0; k < $2; k++) {
                                اظہاریہ_Def* ثابت acts = اظہاریہ_Create_Action_List(عدم);
                                إذا (!acts) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                acts->common.اگلا = &list.سر->common;
                                list.سر = acts;
                            }
                            $$ = list.سر;
                        }
                        /*
                         * A list of *empty* keysym/action lists `{}`.
                         * There is not enough context here to decide if it is
                         * a `MultiKeySymList` or a `MultiActionList`.
                         */
                |       OBRACKET No_علامت_Or_Action_List CBRACKET
                        { $$ = اظہاریہ_خالی_تھاواں_List(); }
                ;

/* A list of `{}`, which remains ambiguous until reaching a keysym or action list */
No_علامت_Or_Action_List
                :       No_علامت_Or_Action_List COMMA OBRACE CBRACE
                        { $$ = $1 + 1; }
                |       OBRACE CBRACE
                        { $$ = 1; }
                |       { $$ = 0; }
                ;

گروہ_Compat_Decl :       گروہ_2 Integer EQUALS اظہاریہ_2 SEMI
                        { $$ = گروہ_Compat_Create($2, $4); }
                ;

ModMapDecl      :       MODIFIER_MAP اظہاریہ_2 OBRACE کلید_Or_کلید_Sym_List CBRACE SEMI
                        {
                            إذا (param->config.format == XKB_KEYMAP_FORMAT_متن_V1 &&
                                $2->common.نوع != STMT_اظہاریہ_IDENT) {
                                    parser_err(
                                        param, XKB_ERROR_INVALID_MODIFIER_MAP_نقاب,
                                        "Invalid real modifier mask in modifier "
                                        "map definition: expected identifier"
                                    );
                                    FreeStmt((ParseCommon *) $2);
                                    FreeStmt((ParseCommon *) $4.سر);
                                    YYERROR;
                            }
                            $$ = ModMapCreate($2, $4.سر);
                        }
                ;

کلید_Or_کلید_Sym_List :       کلید_Or_کلید_Sym_List COMMA کلید_Or_کلید_Sym
                        { $$.سر = $1.سر; $$.آخری->common.اگلا = &$3->common; $$.آخری = $3; }
                |       کلید_Or_کلید_Sym
                        { $$.سر = $$.آخری = $1; }
                ;

کلید_Or_کلید_Sym     :       KEYNAME
                        { $$ = اظہاریہ_Create_کلید_Name($1); }
                |       کلید_Sym_2
                        { $$ = اظہاریہ_Create_کلید_Sym($1); }
                ;

LedMapDecl:             INDICATOR String OBRACE VarDeclList CBRACE SEMI
                        { $$ = LedMapCreate($2, $4.سر); }
                ;

LedNameDecl:            INDICATOR Integer EQUALS اظہاریہ_2 SEMI
                        { $$ = LedNameCreate($2, $4, false); }
                |       VIRTUAL INDICATOR Integer EQUALS اظہاریہ_2 SEMI
                        { $$ = LedNameCreate($3, $5, true); }
                ;

UnknownDecl     :       IDENT Terminal EQUALS اظہاریہ_2 SEMI
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
                            FreeStmt((ParseCommon *) $4.سر);
                            $$ = UnknownStatementCreate(STMT_UNKNOWN_COMPOUND, $1);
                        }
                ;

ShapeDecl       :       SHAPE String OBRACE OutlineList CBRACE SEMI
                        { $$ = عدم; }
                |       SHAPE String OBRACE CoordList CBRACE SEMI
                        { (فراغ) $4; $$ = عدم; }
                ;

SectionDecl     :       SECTION String OBRACE SectionBody CBRACE SEMI
                        { $$ = عدم; }
                ;

SectionBody     :       SectionBody Section_Body_شے     { $$ = عدم;}
                |       Section_Body_شے                 { $$ = عدم; }
                ;

Section_Body_شے :       صف OBRACE صف_Body CBRACE SEMI
                        { $$ = عدم; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = عدم; }
                |       DoodadDecl
                        { $$ = عدم; }
                |       LedMapDecl
                        { FreeStmt((ParseCommon *) $1); $$ = عدم; }
                |       OverlayDecl
                        { $$ = عدم; }
                ;

صف_Body         :       صف_Body صف_Body_شے     { $$ = عدم;}
                |       صف_Body_شے             { $$ = عدم; }
                ;

صف_Body_شے     :       کلیدیں_2 OBRACE کلیدیں_3 CBRACE SEMI { $$ = عدم; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = عدم; }
                ;

کلیدیں_3            :       کلیدیں_3 COMMA کلید_4          { $$ = عدم; }
                |       کلید_4                     { $$ = عدم; }
                ;

کلید_4             :       KEYNAME
                        { $$ = عدم; }
                |       OBRACE اظہاریہ_List_2 CBRACE
                        { FreeStmt((ParseCommon *) $2.سر); $$ = عدم; }
                ;

OverlayDecl     :       OVERLAY String OBRACE Overlay_کلید_List CBRACE SEMI
                        { $$ = عدم; }
                ;

Overlay_کلید_List  :       Overlay_کلید_List COMMA Overlay_کلید { $$ = عدم; }
                |       Overlay_کلید                      { $$ = عدم; }
                ;

Overlay_کلید      :       KEYNAME EQUALS KEYNAME          { $$ = عدم; }
                ;

OutlineList     :       OutlineList COMMA Outline_وچ_List
                        { $$ = عدم;}
                |       Outline_وچ_List
                        { $$ = عدم; }
                ;

Outline_وچ_List   :       OBRACE CoordList CBRACE
                        { (فراغ) $2; $$ = عدم; }
                |       Ident EQUALS OBRACE CoordList CBRACE
                        { (فراغ) $4; $$ = عدم; }
                |       Ident EQUALS اظہاریہ_2
                        { FreeStmt((ParseCommon *) $3); $$ = عدم; }
                ;

CoordList       :       CoordList COMMA Coord
                        { (فراغ) $1; (فراغ) $3; $$ = عدم; }
                |       Coord
                        { (فراغ) $1; $$ = عدم; }
                ;

Coord           :       OBRACKET Signed_نمبر COMMA Signed_نمبر CBRACKET
                        { $$ = عدم; }
                ;

DoodadDecl      :       DoodadType String OBRACE VarDeclList CBRACE SEMI
                        { FreeStmt((ParseCommon *) $4.سر); $$ = عدم; }
                ;

DoodadType      :       متن    { $$ = 0; }
                |       OUTLINE { $$ = 0; }
                |       SOLID   { $$ = 0; }
                |       LOGO    { $$ = 0; }
                ;

میدان_Spec       :       Ident   { $$ = $1; }
                |       Element { $$ = $1; }
                ;

Element         :       ACTION_TOK
                        { $$ = xkb_atom_intern_literal(param->ctx, "action"); }
                |       INTERPRET
                        { $$ = xkb_atom_intern_literal(param->ctx, "interpret"); }
                |       TYPE
                        { $$ = xkb_atom_intern_literal(param->ctx, "type"); }
                |       کلید_3
                        { $$ = xkb_atom_intern_literal(param->ctx, "key"); }
                |       گروہ_2
                        { $$ = xkb_atom_intern_literal(param->ctx, "group"); }
                |       MODIFIER_MAP
                        {$$ = xkb_atom_intern_literal(param->ctx, "modifier_map");}
                |       INDICATOR
                        { $$ = xkb_atom_intern_literal(param->ctx, "indicator"); }
                |       SHAPE
                        { $$ = xkb_atom_intern_literal(param->ctx, "shape"); }
                |       صف
                        { $$ = xkb_atom_intern_literal(param->ctx, "row"); }
                |       SECTION
                        { $$ = xkb_atom_intern_literal(param->ctx, "section"); }
                |       متن
                        { $$ = xkb_atom_intern_literal(param->ctx, "text"); }
                ;

Opt_ملاؤ_منوال    :       ملاؤ_منوال_2       { $$ = $1; }
                |                       { $$ = ملاؤ_DEFAULT; }
                ;

ملاؤ_منوال_2       :       INCLUDE         { $$ = ملاؤ_DEFAULT; }
                |       AUGMENT         { $$ = ملاؤ_AUGMENT; }
                |       OVERRIDE        { $$ = ملاؤ_OVERRIDE; }
                |       REPLACE         { $$ = ملاؤ_REPLACE; }
                |       ALTERNATE
                {
                    /*
                     * This used to be MERGE_ALT_FORM. This functionality was
                     * unused and has been removed.
                     */
                    parser_warn(param, XKB_LOG_سنیہا_NO_ID,
                                "ignored unsupported legacy merge mode \"alternate\"");
                    $$ = ملاؤ_DEFAULT;
                }
                ;

اظہاریہ_List_2        :       اظہاریہ_List_2 COMMA اظہاریہ_2
                        {
                            إذا ($3) {
                                إذا ($1.سر) {
                                    $$.سر = $1.سر;
                                    $$.آخری->common.اگلا = &$3->common;
                                    $$.آخری = $3;
                                } وإلا {
                                    $$.سر = $$.آخری = $3;
                                }
                            }
                        }
                |       اظہاریہ_2
                        { $$.سر = $$.آخری = $1; }
                |       { $$.سر = $$.آخری = عدم; }
                ;

اظہاریہ_2            :       اظہاریہ_2 DIVIDE اظہاریہ_2
                        { $$ = اظہاریہ_Create_ثنائی(STMT_اظہاریہ_DIVIDE, $1, $3); }
                |       اظہاریہ_2 PLUS اظہاریہ_2
                        { $$ = اظہاریہ_Create_ثنائی(STMT_اظہاریہ_جوڑو, $1, $3); }
                |       اظہاریہ_2 MINUS اظہاریہ_2
                        { $$ = اظہاریہ_Create_ثنائی(STMT_اظہاریہ_گھٹاؤ, $1, $3); }
                |       اظہاریہ_2 TIMES اظہاریہ_2
                        { $$ = اظہاریہ_Create_ثنائی(STMT_اظہاریہ_MULTIPLY, $1, $3); }
                |       Lhs EQUALS اظہاریہ_2
                        { $$ = اظہاریہ_Create_ثنائی(STMT_اظہاریہ_سونپو, $1, $3); }
                |       رکن
                        { $$ = $1; }
                ;

رکن            :       MINUS رکن
                        { $$ = اظہاریہ_Create_Unary(STMT_اظہاریہ_NEGATE, $2); }
                |       PLUS رکن
                        { $$ = اظہاریہ_Create_Unary(STMT_اظہاریہ_UNARY_PLUS, $2); }
                |       EXCLAM رکن
                        { $$ = اظہاریہ_Create_Unary(STMT_اظہاریہ_NOT, $2); }
                |       INVERT رکن
                        { $$ = اظہاریہ_Create_Unary(STMT_اظہاریہ_INVERT, $2); }
                |       Lhs
                        { $$ = $1; }
                |       میدان_Spec OPAREN اظہاریہ_List_2 CPAREN %prec OPAREN
                        { $$ = اظہاریہ_Create_Action($1, $3.سر); }
                |       Actions
                        { $$ = $1; }
                |       Terminal
                        { $$ = $1; }
                |       OPAREN اظہاریہ_2 CPAREN
                        { $$ = $2; }
                ;

MultiActionList :       MultiActionList COMMA Action
                        {
                            اظہاریہ_Def *اظہاریہ = اظہاریہ_Create_Action_List($3);
                            $$ = $1;
                            $$.آخری->common.اگلا = &اظہاریہ->common; $$.آخری = اظہاریہ;
                        }
                |       MultiActionList COMMA Actions
                        { $$ = $1; $$.آخری->common.اگلا = &$3->common; $$.آخری = $3; }
                |       Action
                        { $$.سر = $$.آخری = اظہاریہ_Create_Action_List($1); }
                |       Non_خالی_تھاواں_Actions
                        { $$.سر = $$.آخری = $1; }
                ;

ActionList      :       ActionList COMMA Action
                        { $$ = $1; $$.آخری->common.اگلا = &$3->common; $$.آخری = $3; }
                |       Action
                        { $$.سر = $$.آخری = $1; }
                ;

Non_خالی_تھاواں_Actions :       OBRACE ActionList CBRACE
                        { $$ = اظہاریہ_Create_Action_List($2.سر); }
                ;

Actions         :       Non_خالی_تھاواں_Actions
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = اظہاریہ_Create_Action_List(عدم); }
                ;

Action          :       میدان_Spec OPAREN اظہاریہ_List_2 CPAREN
                        { $$ = اظہاریہ_Create_Action($1, $3.سر); }
                ;

Lhs             :       میدان_Spec
                        { $$ = اظہاریہ_Create_Ident($1); }
                |       میدان_Spec DOT میدان_Spec
                        { $$ = اظہاریہ_Create_میدان_Ref($1, $3); }
                |       میدان_Spec OBRACKET اظہاریہ_2 CBRACKET
                        { $$ = اظہاریہ_Create_Array_Ref(XKB_ATOM_NONE, $1, $3); }
                |       میدان_Spec DOT میدان_Spec OBRACKET اظہاریہ_2 CBRACKET
                        { $$ = اظہاریہ_Create_Array_Ref($1, $3, $5); }
                ;

OptTerminal     :       Terminal
                        { $$ = $1; }
                |       { $$ = عدم; }
                ;

Terminal        :       String
                        { $$ = اظہاریہ_Create_String($1); }
                |       Integer
                        { $$ = اظہاریہ_Create_Integer($1); }
                |       Float
                        { $$ = اظہاریہ_Create_Float(/* Discard $1 */); }
                |       KEYNAME
                        { $$ = اظہاریہ_Create_کلید_Name($1); }
                ;

Multi_کلید_Sym_List :       Multi_کلید_Sym_List COMMA کلید_Sym_Lit
                        {
                            اظہاریہ_Def *اظہاریہ = اظہاریہ_Create_کلید_Sym_List($3);
                            $$ = $1;
                            $$.آخری->common.اگلا = &اظہاریہ->common; $$.آخری = اظہاریہ;
                        }
                |       Multi_کلید_Sym_List COMMA کلید_Syms
                        { $$ = $1; $$.آخری->common.اگلا = &$3->common; $$.آخری = $3; }
                |       کلید_Sym_Lit
                        { $$.سر = $$.آخری = اظہاریہ_Create_کلید_Sym_List($1); }
                |       Non_خالی_تھاواں_کلید_Syms
                        { $$.سر = $$.آخری = $1; }
                ;

کلید_Sym_List      :       کلید_Sym_List COMMA کلید_Sym_Lit
                        { $$ = اظہاریہ_Append_کلید_Sym_List($1, $3); }
                |       کلید_Sym_List COMMA STRING
                        {
                            $$ = اظہاریہ_کلید_Sym_List_Append_String(param->scanner, $1, $3);
                            حرر($3);
                            إذا (!$$)
                                YYERROR;
                        }
                |       کلید_Sym_Lit
                        {
                            $$ = اظہاریہ_Create_کلید_Sym_List($1);
                            إذا (!$$)
                                YYERROR;
                        }
                |       STRING
                        {
                            $$ = اظہاریہ_Create_کلید_Sym_List(XKB_کلید_No_علامت);
                            إذا (!$$)
                                YYERROR;
                            $$ = اظہاریہ_کلید_Sym_List_Append_String(param->scanner, $$, $1);
                            حرر($1);
                            إذا (!$$)
                                YYERROR;
                        }
                ;

Non_خالی_تھاواں_کلید_Syms :       OBRACE کلید_Sym_List CBRACE
                        { $$ = $2; }
                |       STRING
                        {
                            $$ = اظہاریہ_Create_کلید_Sym_List(XKB_کلید_No_علامت);
                            إذا (!$$)
                                YYERROR;
                            $$ = اظہاریہ_کلید_Sym_List_Append_String(param->scanner, $$, $1);
                            حرر($1);
                            إذا (!$$)
                                YYERROR;
                        }
                ;

کلید_Syms         :       Non_خالی_تھاواں_کلید_Syms
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = اظہاریہ_Create_کلید_Sym_List(XKB_کلید_No_علامت); }
                ;

کلید_Sym_2          :       کلید_Sym_Lit
                        { $$ = $1; }
                |       STRING
                        {
                            $$ = KeysymParseString(param->scanner, $1);
                            حرر($1);
                            إذا ($$ == XKB_کلید_No_علامت)
                                YYERROR;
                        }
                ;

کلید_Sym_Lit       :       IDENT
                        {
                            إذا (!resolve_keysym(param, $1, &$$)) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (بنانشانی صحيح) $1.len, $1.شروع
                                );
                                $$ = XKB_کلید_No_علامت;
                            }
                        }
                        /* Handle keysym that is also a keyword  */
                |       SECTION { $$ = XKB_کلید_section; }
                |       DECIMAL_DIGIT
                        {
                            /*
                             * Special case for digits 0..9:
                             * map to XKB_KEY_0 .. XKB_KEY_9, consistent with
                             * other keysym names: <name> → XKB_KEY_<name>.
                             */
                            $$ = XKB_کلید_0 + (xkb_keysym_t) $1;
                        }
                |       INTEGER
                        {
                            إذا ($1 < XKB_KEYSYM_MIN) {
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
                                $$ = XKB_کلید_No_علامت;
                            }
                            /*
                             * Integers 0..9 are handled with DECIMAL_DIGIT if
                             * they were formatted as single characters '0'..'9'.
                             * Otherwise they are handled here as raw keysyms
                             * values. E.g. `01` and `0x1` are interpreted as
                             * the keysym 0x0001, while `1` is interpreted as
                             * XKB_KEY_1.
                             */
                            وإلا {
                                /* Any other numeric value */
                                إذا ($1 <= XKB_KEYSYM_MAX) {
                                    /*
                                     * Valid keysym
                                     * No normalization is performed and value
                                     * is used as is.
                                     */
                                    $$ = (xkb_keysym_t) $1;
                                    check_deprecated_keysyms(
                                        parser_warn, param, param->ctx,
                                        $$, عدم, $$, "%#06"PRIx32, "");
                                } وإلا {
                                    /* Invalid keysym */
                                    parser_warn(
                                        param, XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                        "unrecognized keysym \"%#06"PRIx64"\" "
                                        "(%"PRId64")", $1, $1
                                    );
                                    $$ = XKB_کلید_No_علامت;
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

Signed_نمبر    :       MINUS نمبر    { $$ = -$2; }
                |       نمبر          { $$ = $1; }
                ;

نمبر          :       FLOAT         { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                |       INTEGER       { $$ = $1; }
                ;

Float           :       FLOAT   { $$ = 0; }
                ;

Integer         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

کلید_کوڈ_2         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

Ident           :       IDENT   { $$ = xkb_atom_intern(param->ctx, $1.شروع, $1.len); }
                |       DEFAULT { $$ = xkb_atom_intern_literal(param->ctx, "default"); }
                ;

String          :       STRING  { $$ = xkb_atom_intern(param->ctx, $1, لڑی_لمبائی($1)); حرر($1); }
                ;

OptMapName      :       MapName { $$ = $1; }
                |               { $$ = عدم; }
                ;

MapName         :       STRING  { $$ = $1; }
                ;

%%

/* Parse a specific section */
XkbFile *
parse(بنية xkb_context *ctx, ثابت بنية parser_keymap_config *config,
      بنية scanner *scanner, ثابت حرف *map)
{
    صحيح ret;
    XkbFile *پہلا = عدم;
    بنية parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = عدم,
        .config = *config,
        .ہور_maps = false,
    };

    /*
     * If we got a specific map, we look for it exclusively and return
     * immediately upon finding it. Otherwise, we need to get the
     * default map. If we find a map marked as default, we return it
     * immediately. If there are no maps marked as default, we return
     * the first map in the file.
     */

    طالما ((ret = yyparse(&param)) == 0 && param.ہور_maps) {
        إذا (map) {
            إذا (streq_not_null(map, param.rtrn->name))
                أرجع param.rtrn;
            وإلا
                FreeXkbFile(param.rtrn);
        }
        وإلا {
            إذا (param.rtrn->flags & MAP_IS_DEFAULT) {
                FreeXkbFile(پہلا);
                أرجع param.rtrn;
            }
            وإلا إذا (!پہلا) {
                پہلا = param.rtrn;
            }
            وإلا {
                FreeXkbFile(param.rtrn);
            }
        }
        param.rtrn = عدم;
    }

    إذا (ret != 0) {
        /* Some error happend; clear the Xkbfiles parsed so far */
        FreeXkbFile(پہلا);
        FreeXkbFile(param.rtrn);
        أرجع عدم;
    }

    /*
     * Warn about implicit default section,
     * but only if not a keymap: multiple keymaps per file not supported
     */
    إذا (پہلا && پہلا->file_type != FILE_TYPE_KEYMAP)
        log_vrb(ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_MISSING_DEFAULT_SECTION,
                "No section name in include statement, but \"%s\" contains several; "
                "Using first defined section, \"%s\"\n",
                scanner->file_name, محفوظ_map_name(پہلا->name));

    أرجع پہلا;
}

/* Parse the next section */
bool
parse_اگلا(بنية xkb_context *ctx,
           ثابت بنية parser_keymap_config *config,
           بنية scanner *scanner, XkbFile **xkb_file)
{
    صحيح ret;
    بنية parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = عدم,
        .config = *config,
        .ہور_maps = false,
    };

    إذا ((ret = yyparse(&param)) == 0 && param.ہور_maps) {
        *xkb_file = param.rtrn;
        أرجع true;
    } وإلا {
        FreeXkbFile(param.rtrn);
        *xkb_file = عدم;
        أرجع (ret == 0);
    }
}
