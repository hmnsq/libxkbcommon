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
    bool نور_maps;
};

#تعریف_کړه parser_log_with_کوډ(param, level, verbosity, log_پیغام_id, fmt, ...)   \
    scanner_log_with_کوډ((param)->scanner, level, verbosity, log_پیغام_id, fmt,\
                          ##__VA_ARGS__)

#تعریف_کړه parser_err(param, error_id, fmt, ...) \
    scanner_err((param)->scanner, error_id, fmt, ##__VA_ARGS__)

#تعریف_کړه parser_warn(param, warning_id, fmt, ...) \
    scanner_warn((param)->scanner, warning_id, fmt, ##__VA_ARGS__)

#تعریف_کړه parser_vrb(param, verbosity, warning_id, fmt, ...) \
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
        *sym_rtrn = XKB_کلی_No_نښه;
        أرجع true;
    }

    إذا (isvaleq(name, SVAL_LIT("none")) || isvaleq(name, SVAL_LIT("voidsymbol"))) {
        *sym_rtrn = XKB_کلی_Void_نښه;
        أرجع true;
    }

    /* xkb_keysym_from_name needs a C string. */
    حرف buf[XKB_KEYSYM_NAME_MAX_SIZE];
    إذا (name.len >= حجم(buf)) {
        أرجع false;
    }
    حافظه_نقل(buf, name.پیل, name.len);
    buf[name.len] = '\0';

    sym = xkb_keysym_from_name(buf, XKB_KEYSYM_NO_FLAGS);
    إذا (sym != XKB_کلی_No_نښه) {
        *sym_rtrn = sym;
        check_deprecated_keysyms(parser_warn, param, param->ctx,
                                 sym, buf, buf, "%s", "");
        أرجع true;
    }

    أرجع false;
}

#تعریف_کړه param_scanner param->scanner
%}

%define api.خالص
%lex-param      { بنية scanner *param_scanner }
%parse-param    { بنية parser_param *param }

%define parse.error detailed

%token
        پای_ټکی_د_FILE     0 "end of file"
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
        کلی_3             24 "key"
        ALIAS           25 "alias"
        ډله_2           26 "group"
        MODIFIER_MAP    27 "modifier_map"
        INDICATOR       28 "indicator"
        SHAPE           29 "shape"
        کلیګانې_2            30 "keys"
        لیکه             31 "row"
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
        ALPHANUMERIC_کلیګانې       73 "alphanumeric_keys"
        MODIFIER_کلیګانې           74 "modifier_keys"
        KEYPAD_کلیګانې             75 "keypad_keys"
        FUNCTION_کلیګانې           76 "function_keys"
        ALTERNATE_ډله         77 "alternate_group"
        DEPRECATED              78 "deprecated"

%ښی  EQUALS
%کیڼ   PLUS MINUS
%کیڼ   TIMES DIVIDE
%کیڼ   EXCLAM INVERT
%کیڼ   OPAREN

%start  XkbFile

%اتحاد  {
        int64_t          عدد;
        تعداد xkb_file_type file_type;
        حرف            *str;
        بنية sval     sval;
        xkb_atom_t      atom;
        تعداد یوځای_ډېرپېښ یوځای;
        تعداد xkb_map_flags mapFlags;
        xkb_keysym_t    keysym;
        ParseCommon     *any;
        بنية { ParseCommon *سر; ParseCommon *وروستی; } anyList;
        uint32_t        no_نښه_Or_Action_List;
        بیان_Def         *بیان;
        بنية { بیان_Def *سر; بیان_Def *وروستی; } بیان_List;
        VarDef          *var;
        بنية { VarDef *سر; VarDef *وروستی; } varList;
        VModDef         *vmod;
        بنية { VModDef *سر; VModDef *وروستی; } vmodList;
        InterpDef       *interp;
        کلی_Type_Def      *کلی_Type;
        SymbolsDef      *syms;
        ModMapDef       *mod_ماسک_2;
        ډله_Compat_Def  *ډله_Compat;
        LedMapDef       *ledMap;
        LedNameDef      *ledName;
        KeycodeDef      *کلی_کوډ;
        کلی_Alias_Def     *کلی_Alias;
        UnknownStatement *unknown;
        فراغ            *geom;
        XkbFile         *file;
        بنية { XkbFile *سر; XkbFile *وروستی; } fileList;
}

%نوع <عدد>     DECIMAL_DIGIT INTEGER FLOAT
%نوع <str>     STRING
%نوع <sval>    IDENT
%نوع <atom>    KEYNAME
%نوع <عدد>     کلی_کوډ_2 شمېره Integer Float Signed_شمېره DoodadType
%نوع <یوځای>   یوځای_ډېرپېښ_2 Opt_یوځای_ډېرپېښ
%نوع <file_type> Xkb_مرکب_Type FileType
%نوع <mapFlags> Flag Flags OptFlags
%نوع <str>     MapName OptMapName
%نوع <atom>    ډګر_Spec Ident Element String
%نوع <keysym>  کلی_Sym_2 کلی_Sym_Lit
%نوع <no_نښه_Or_Action_List> No_نښه_Or_Action_List
%نوع <any>     Decl
%نوع <anyList> DeclList
%نوع <بیان>    بیان_2 غړی Lhs Terminal OptTerminal Coord CoordList
%نوع <بیان>    Multi_کلی_Sym_Or_Action_List Non_تشه_Actions Actions Action
%نوع <بیان>    کلی_Syms Non_تشه_کلی_Syms کلی_Sym_List کلی_Or_کلی_Sym
%نوع <بیان_List> بیان_List_2 کلی_Or_کلی_Sym_List
%نوع <بیان_List> ActionList MultiActionList Multi_کلی_Sym_List
%نوع <var>     VarDecl SymbolsVarDecl
%نوع <varList> VarDeclList SymbolsBody OptSymbolsBody
%نوع <vmod>    VModDef
%نوع <vmodList> VModDefList VModDecl
%نوع <interp>  InterpretDecl Interpret_سمون
%نوع <کلی_Type> کلی_Type_Decl
%نوع <syms>    SymbolsDecl
%نوع <mod_ماسک_2> ModMapDecl
%نوع <ډله_Compat> ډله_Compat_Decl
%نوع <ledMap>  LedMapDecl
%نوع <ledName> LedNameDecl
%نوع <کلی_کوډ> کلی_Name_Decl
%نوع <کلی_Alias> کلی_Alias_Decl
%نوع <geom>    ShapeDecl SectionDecl SectionBody Section_Body_توکی لیکه_Body لیکه_Body_توکی
%نوع <geom>    کلیګانې_3 کلی_4 OverlayDecl Overlay_کلی_List Overlay_کلی OutlineList Outline_دننه_List
%نوع <geom>    DoodadDecl
%نوع <unknown> UnknownDecl UnknownCompoundStatementDecl
%نوع <file>    XkbFile XkbMapConfig
%نوع <fileList> XkbMapConfigList
%نوع <file>    Xkb_مرکب_Map

%destructor { FreeStmt((ParseCommon *) $$); }
    <any> <بیان> <var> <vmod> <interp> <کلی_Type> <syms> <mod_ماسک_2> <ډله_Compat>
    <ledMap> <ledName> <کلی_کوډ> <کلی_Alias>
%destructor { FreeStmt((ParseCommon *) $$.سر); }
    <anyList> <بیان_List> <varList> <vmodList>
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
                        { $$ = param->rtrn = $1; param->نور_maps = !!param->rtrn; (فراغ) yynerrs; }
                |       XkbMapConfig
                        { $$ = param->rtrn = $1; param->نور_maps = !!param->rtrn; YYACCEPT; }
                |       پای_ټکی_د_FILE
                        { $$ = param->rtrn = عدم; param->نور_maps = false; }
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
                                    $$.وروستی->common.بل = &$2->common;
                                    $$.وروستی = $2;
                                } وإلا {
                                    $$.سر = $$.وروستی = $2;
                                }
                            }
                        }
                |       { $$.سر = $$.وروستی = عدم; }
                ;

XkbMapConfig    :       OptFlags FileType OptMapName OBRACE
                            DeclList
                        CBRACE SEMI
                        {
                            إذا ($1 & MAP_IS_DEPRECATED) {
                                parser_warn(param, XKB_WARNING_DEPRECATED_SECTION,
                                            "deprecated section: \"%s\"",
                                            خوندي_map_name($3));
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
                |       ALPHANUMERIC_کلیګانې       { $$ = MAP_HAS_ALPHANUMERIC; }
                |       MODIFIER_کلیګانې           { $$ = MAP_HAS_MODIFIER; }
                |       KEYPAD_کلیګانې             { $$ = MAP_HAS_KEYPAD; }
                |       FUNCTION_کلیګانې           { $$ = MAP_HAS_FN; }
                |       ALTERNATE_ډله         { $$ = MAP_IS_ALTGR; }
                |       DEPRECATED              { $$ = MAP_IS_DEPRECATED; }
                |       IDENT
                        {
                            ثابت bool error = (param->config.strict & PARSER_NO_UNKNOWN_SECTION_FLAGS);
                            parser_log_with_کوډ(
                                param, (error ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
                                XKB_LOG_VERBOSITY_اصغري,
                                XKB_ERROR_UNKNOWN_SECTION_FLAG,
                                "Unknown section flag \"%.*s\"%s",
                                (بینښه)$1.len, $1.پیل,
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
                                    $$.سر = $1.سر; $1.وروستی->بل = $2; $$.وروستی = $2;
                                } وإلا {
                                    $$.سر = $$.وروستی = $2;
                                }
                            }
                        }
                        /*
                         * VModDecl is "inlined" directly into DeclList, i.e.
                         * each VModDef in the VModDecl is a separate Decl in
                         * the File.
                         */
                |       DeclList Opt_یوځای_ډېرپېښ VModDecl
                        {
                            لكل (VModDef *vmod = $3.سر; vmod; vmod = (VModDef *) vmod->common.بل)
                                vmod->یوځای = $2;
                            إذا ($1.سر) {
                                $$.سر = $1.سر; $1.وروستی->بل = &$3.سر->common; $$.وروستی = &$3.وروستی->common;
                            } وإلا {
                                $$.سر = &$3.سر->common; $$.وروستی = &$3.وروستی->common;
                            }
                        }
                |       { $$.سر = $$.وروستی = عدم; }
                ;

Decl            :       Opt_یوځای_ډېرپېښ VarDecl
                        {
                            $2->یوځای = $1;
                            $$ = (ParseCommon *) $2;
                        }
                /*      OptMergeMode VModDecl - see above. */
                |       Opt_یوځای_ډېرپېښ InterpretDecl
                        {
                            $2->یوځای = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_یوځای_ډېرپېښ کلی_Name_Decl
                        {
                            $2->یوځای = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_یوځای_ډېرپېښ کلی_Alias_Decl
                        {
                            $2->یوځای = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_یوځای_ډېرپېښ کلی_Type_Decl
                        {
                            $2->یوځای = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_یوځای_ډېرپېښ SymbolsDecl
                        {
                            $2->یوځای = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_یوځای_ډېرپېښ ModMapDecl
                        {
                            $2->یوځای = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_یوځای_ډېرپېښ ډله_Compat_Decl
                        {
                            $2->یوځای = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_یوځای_ډېرپېښ LedMapDecl
                        {
                            $2->یوځای = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_یوځای_ډېرپېښ LedNameDecl
                        {
                            $2->یوځای = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_یوځای_ډېرپېښ ShapeDecl          { $$ = عدم; }
                |       Opt_یوځای_ډېرپېښ SectionDecl        { $$ = عدم; }
                |       Opt_یوځای_ډېرپېښ DoodadDecl         { $$ = عدم; }
                |       Opt_یوځای_ډېرپېښ UnknownDecl
                            { $$ = (ParseCommon *) $2; }
                |       Opt_یوځای_ډېرپېښ UnknownCompoundStatementDecl
                            { $$ = (ParseCommon *) $2; }
                |       یوځای_ډېرپېښ_2 STRING
                        {
                            $$ = (ParseCommon *) IncludeCreate(param->ctx, $2, $1);
                            حرر($2);
                        }
                ;

VarDecl         :       Lhs EQUALS بیان_2 SEMI
                        { $$ = VarCreate($1, $3); }
                |       Ident SEMI
                        { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident SEMI
                        { $$ = BoolVarCreate($2, false); }
                ;

کلی_Name_Decl     :       KEYNAME EQUALS کلی_کوډ_2 SEMI
                        { $$ = KeycodeCreate($1, $3); }
                ;

کلی_Alias_Decl    :       ALIAS KEYNAME EQUALS KEYNAME SEMI
                        { $$ = کلی_Alias_Create($2, $4); }
                ;

VModDecl        :       VIRTUAL_MODS VModDefList SEMI
                        { $$ = $2; }
                ;

VModDefList     :       VModDefList COMMA VModDef
                        { $$.سر = $1.سر; $$.وروستی->common.بل = &$3->common; $$.وروستی = $3; }
                |       VModDef
                        { $$.سر = $$.وروستی = $1; }
                ;

VModDef         :       Ident
                        { $$ = VModCreate($1, عدم); }
                |       Ident EQUALS بیان_2
                        { $$ = VModCreate($1, $3); }
                ;

InterpretDecl   :       INTERPRET Interpret_سمون OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $2->def = $4.سر; $$ = $2; }
                ;

Interpret_سمون  :       کلی_Sym_2 PLUS بیان_2
                        { $$ = InterpCreate($1, $3); }
                |       کلی_Sym_2
                        { $$ = InterpCreate($1, عدم); }
                ;

VarDeclList     :       VarDeclList VarDecl
                        {
                            إذا ($2) {
                                إذا ($1.سر) {
                                    $$.سر = $1.سر;
                                    $$.وروستی->common.بل = &$2->common;
                                    $$.وروستی = $2;
                                } وإلا {
                                    $$.سر = $$.وروستی = $2;
                                }
                            }
                        }
                |       { $$.سر = $$.وروستی = عدم; }
                ;

کلی_Type_Decl     :       TYPE String OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $$ = کلی_Type_Create($2, $4.سر); }
                ;

SymbolsDecl     :       کلی_3 KEYNAME OBRACE
                            OptSymbolsBody
                        CBRACE SEMI
                        { $$ = SymbolsCreate($2, $4.سر); }
                ;

OptSymbolsBody  :       SymbolsBody { $$ = $1; }
                |                   { $$.سر = $$.وروستی = عدم; }
                ;

SymbolsBody     :       SymbolsBody COMMA SymbolsVarDecl
                        { $$.سر = $1.سر; $$.وروستی->common.بل = &$3->common; $$.وروستی = $3; }
                |       SymbolsVarDecl
                        { $$.سر = $$.وروستی = $1; }
                ;

SymbolsVarDecl  :       Lhs EQUALS بیان_2         { $$ = VarCreate($1, $3); }
                |       Lhs EQUALS Multi_کلی_Sym_Or_Action_List { $$ = VarCreate($1, $3); }
                |       Ident                   { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident            { $$ = BoolVarCreate($2, false); }
                |       Multi_کلی_Sym_Or_Action_List { $$ = VarCreate(عدم, $1); }
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
Multi_کلی_Sym_Or_Action_List
                :       OBRACKET Multi_کلی_Sym_List CBRACKET
                        { $$ = $2.سر; }
                |       OBRACKET No_نښه_Or_Action_List COMMA Multi_کلی_Sym_List CBRACKET
                        {
                            /* Prepend n times NoSymbol */
                            بنية {بیان_Def *سر; بیان_Def *وروستی;} list = {
                                .سر = $4.سر, .وروستی = $4.وروستی
                            };
                            لكل (uint32_t k = 0; k < $2; k++) {
                                بیان_Def* ثابت syms =
                                    بیان_Create_کلی_Sym_List(XKB_کلی_No_نښه);
                                إذا (!syms) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                syms->common.بل = &list.سر->common;
                                list.سر = syms;
                            }
                            $$ = list.سر;
                        }
                |       OBRACKET MultiActionList CBRACKET
                        { $$ = $2.سر; }
                |       OBRACKET No_نښه_Or_Action_List COMMA MultiActionList CBRACKET
                        {
                            /* Prepend n times NoAction() */
                            بنية {بیان_Def *سر; بیان_Def *وروستی;} list = {
                                .سر = $4.سر, .وروستی = $4.وروستی
                            };
                            لكل (uint32_t k = 0; k < $2; k++) {
                                بیان_Def* ثابت acts = بیان_Create_Action_List(عدم);
                                إذا (!acts) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                acts->common.بل = &list.سر->common;
                                list.سر = acts;
                            }
                            $$ = list.سر;
                        }
                        /*
                         * A list of *empty* keysym/action lists `{}`.
                         * There is not enough context here to decide if it is
                         * a `MultiKeySymList` or a `MultiActionList`.
                         */
                |       OBRACKET No_نښه_Or_Action_List CBRACKET
                        { $$ = بیان_تشه_List(); }
                ;

/* A list of `{}`, which remains ambiguous until reaching a keysym or action list */
No_نښه_Or_Action_List
                :       No_نښه_Or_Action_List COMMA OBRACE CBRACE
                        { $$ = $1 + 1; }
                |       OBRACE CBRACE
                        { $$ = 1; }
                |       { $$ = 0; }
                ;

ډله_Compat_Decl :       ډله_2 Integer EQUALS بیان_2 SEMI
                        { $$ = ډله_Compat_Create($2, $4); }
                ;

ModMapDecl      :       MODIFIER_MAP بیان_2 OBRACE کلی_Or_کلی_Sym_List CBRACE SEMI
                        {
                            إذا (param->config.format == XKB_KEYMAP_FORMAT_متن_V1 &&
                                $2->common.نوع != STMT_بیان_IDENT) {
                                    parser_err(
                                        param, XKB_ERROR_INVALID_MODIFIER_MAP_ماسک,
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

کلی_Or_کلی_Sym_List :       کلی_Or_کلی_Sym_List COMMA کلی_Or_کلی_Sym
                        { $$.سر = $1.سر; $$.وروستی->common.بل = &$3->common; $$.وروستی = $3; }
                |       کلی_Or_کلی_Sym
                        { $$.سر = $$.وروستی = $1; }
                ;

کلی_Or_کلی_Sym     :       KEYNAME
                        { $$ = بیان_Create_کلی_Name($1); }
                |       کلی_Sym_2
                        { $$ = بیان_Create_کلی_Sym($1); }
                ;

LedMapDecl:             INDICATOR String OBRACE VarDeclList CBRACE SEMI
                        { $$ = LedMapCreate($2, $4.سر); }
                ;

LedNameDecl:            INDICATOR Integer EQUALS بیان_2 SEMI
                        { $$ = LedNameCreate($2, $4, false); }
                |       VIRTUAL INDICATOR Integer EQUALS بیان_2 SEMI
                        { $$ = LedNameCreate($3, $5, true); }
                ;

UnknownDecl     :       IDENT Terminal EQUALS بیان_2 SEMI
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

SectionBody     :       SectionBody Section_Body_توکی     { $$ = عدم;}
                |       Section_Body_توکی                 { $$ = عدم; }
                ;

Section_Body_توکی :       لیکه OBRACE لیکه_Body CBRACE SEMI
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

لیکه_Body         :       لیکه_Body لیکه_Body_توکی     { $$ = عدم;}
                |       لیکه_Body_توکی             { $$ = عدم; }
                ;

لیکه_Body_توکی     :       کلیګانې_2 OBRACE کلیګانې_3 CBRACE SEMI { $$ = عدم; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = عدم; }
                ;

کلیګانې_3            :       کلیګانې_3 COMMA کلی_4          { $$ = عدم; }
                |       کلی_4                     { $$ = عدم; }
                ;

کلی_4             :       KEYNAME
                        { $$ = عدم; }
                |       OBRACE بیان_List_2 CBRACE
                        { FreeStmt((ParseCommon *) $2.سر); $$ = عدم; }
                ;

OverlayDecl     :       OVERLAY String OBRACE Overlay_کلی_List CBRACE SEMI
                        { $$ = عدم; }
                ;

Overlay_کلی_List  :       Overlay_کلی_List COMMA Overlay_کلی { $$ = عدم; }
                |       Overlay_کلی                      { $$ = عدم; }
                ;

Overlay_کلی      :       KEYNAME EQUALS KEYNAME          { $$ = عدم; }
                ;

OutlineList     :       OutlineList COMMA Outline_دننه_List
                        { $$ = عدم;}
                |       Outline_دننه_List
                        { $$ = عدم; }
                ;

Outline_دننه_List   :       OBRACE CoordList CBRACE
                        { (فراغ) $2; $$ = عدم; }
                |       Ident EQUALS OBRACE CoordList CBRACE
                        { (فراغ) $4; $$ = عدم; }
                |       Ident EQUALS بیان_2
                        { FreeStmt((ParseCommon *) $3); $$ = عدم; }
                ;

CoordList       :       CoordList COMMA Coord
                        { (فراغ) $1; (فراغ) $3; $$ = عدم; }
                |       Coord
                        { (فراغ) $1; $$ = عدم; }
                ;

Coord           :       OBRACKET Signed_شمېره COMMA Signed_شمېره CBRACKET
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

ډګر_Spec       :       Ident   { $$ = $1; }
                |       Element { $$ = $1; }
                ;

Element         :       ACTION_TOK
                        { $$ = xkb_atom_intern_literal(param->ctx, "action"); }
                |       INTERPRET
                        { $$ = xkb_atom_intern_literal(param->ctx, "interpret"); }
                |       TYPE
                        { $$ = xkb_atom_intern_literal(param->ctx, "type"); }
                |       کلی_3
                        { $$ = xkb_atom_intern_literal(param->ctx, "key"); }
                |       ډله_2
                        { $$ = xkb_atom_intern_literal(param->ctx, "group"); }
                |       MODIFIER_MAP
                        {$$ = xkb_atom_intern_literal(param->ctx, "modifier_map");}
                |       INDICATOR
                        { $$ = xkb_atom_intern_literal(param->ctx, "indicator"); }
                |       SHAPE
                        { $$ = xkb_atom_intern_literal(param->ctx, "shape"); }
                |       لیکه
                        { $$ = xkb_atom_intern_literal(param->ctx, "row"); }
                |       SECTION
                        { $$ = xkb_atom_intern_literal(param->ctx, "section"); }
                |       متن
                        { $$ = xkb_atom_intern_literal(param->ctx, "text"); }
                ;

Opt_یوځای_ډېرپېښ    :       یوځای_ډېرپېښ_2       { $$ = $1; }
                |                       { $$ = یوځای_DEFAULT; }
                ;

یوځای_ډېرپېښ_2       :       INCLUDE         { $$ = یوځای_DEFAULT; }
                |       AUGMENT         { $$ = یوځای_AUGMENT; }
                |       OVERRIDE        { $$ = یوځای_OVERRIDE; }
                |       REPLACE         { $$ = یوځای_REPLACE; }
                |       ALTERNATE
                {
                    /*
                     * This used to be MERGE_ALT_FORM. This functionality was
                     * unused and has been removed.
                     */
                    parser_warn(param, XKB_LOG_پیغام_NO_ID,
                                "ignored unsupported legacy merge mode \"alternate\"");
                    $$ = یوځای_DEFAULT;
                }
                ;

بیان_List_2        :       بیان_List_2 COMMA بیان_2
                        {
                            إذا ($3) {
                                إذا ($1.سر) {
                                    $$.سر = $1.سر;
                                    $$.وروستی->common.بل = &$3->common;
                                    $$.وروستی = $3;
                                } وإلا {
                                    $$.سر = $$.وروستی = $3;
                                }
                            }
                        }
                |       بیان_2
                        { $$.سر = $$.وروستی = $1; }
                |       { $$.سر = $$.وروستی = عدم; }
                ;

بیان_2            :       بیان_2 DIVIDE بیان_2
                        { $$ = بیان_Create_دوګونی(STMT_بیان_DIVIDE, $1, $3); }
                |       بیان_2 PLUS بیان_2
                        { $$ = بیان_Create_دوګونی(STMT_بیان_جمع_کړه, $1, $3); }
                |       بیان_2 MINUS بیان_2
                        { $$ = بیان_Create_دوګونی(STMT_بیان_کمول, $1, $3); }
                |       بیان_2 TIMES بیان_2
                        { $$ = بیان_Create_دوګونی(STMT_بیان_MULTIPLY, $1, $3); }
                |       Lhs EQUALS بیان_2
                        { $$ = بیان_Create_دوګونی(STMT_بیان_ټاکل, $1, $3); }
                |       غړی
                        { $$ = $1; }
                ;

غړی            :       MINUS غړی
                        { $$ = بیان_Create_Unary(STMT_بیان_NEGATE, $2); }
                |       PLUS غړی
                        { $$ = بیان_Create_Unary(STMT_بیان_UNARY_PLUS, $2); }
                |       EXCLAM غړی
                        { $$ = بیان_Create_Unary(STMT_بیان_NOT, $2); }
                |       INVERT غړی
                        { $$ = بیان_Create_Unary(STMT_بیان_INVERT, $2); }
                |       Lhs
                        { $$ = $1; }
                |       ډګر_Spec OPAREN بیان_List_2 CPAREN %prec OPAREN
                        { $$ = بیان_Create_Action($1, $3.سر); }
                |       Actions
                        { $$ = $1; }
                |       Terminal
                        { $$ = $1; }
                |       OPAREN بیان_2 CPAREN
                        { $$ = $2; }
                ;

MultiActionList :       MultiActionList COMMA Action
                        {
                            بیان_Def *بیان = بیان_Create_Action_List($3);
                            $$ = $1;
                            $$.وروستی->common.بل = &بیان->common; $$.وروستی = بیان;
                        }
                |       MultiActionList COMMA Actions
                        { $$ = $1; $$.وروستی->common.بل = &$3->common; $$.وروستی = $3; }
                |       Action
                        { $$.سر = $$.وروستی = بیان_Create_Action_List($1); }
                |       Non_تشه_Actions
                        { $$.سر = $$.وروستی = $1; }
                ;

ActionList      :       ActionList COMMA Action
                        { $$ = $1; $$.وروستی->common.بل = &$3->common; $$.وروستی = $3; }
                |       Action
                        { $$.سر = $$.وروستی = $1; }
                ;

Non_تشه_Actions :       OBRACE ActionList CBRACE
                        { $$ = بیان_Create_Action_List($2.سر); }
                ;

Actions         :       Non_تشه_Actions
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = بیان_Create_Action_List(عدم); }
                ;

Action          :       ډګر_Spec OPAREN بیان_List_2 CPAREN
                        { $$ = بیان_Create_Action($1, $3.سر); }
                ;

Lhs             :       ډګر_Spec
                        { $$ = بیان_Create_Ident($1); }
                |       ډګر_Spec DOT ډګر_Spec
                        { $$ = بیان_Create_ډګر_Ref($1, $3); }
                |       ډګر_Spec OBRACKET بیان_2 CBRACKET
                        { $$ = بیان_Create_Array_Ref(XKB_ATOM_NONE, $1, $3); }
                |       ډګر_Spec DOT ډګر_Spec OBRACKET بیان_2 CBRACKET
                        { $$ = بیان_Create_Array_Ref($1, $3, $5); }
                ;

OptTerminal     :       Terminal
                        { $$ = $1; }
                |       { $$ = عدم; }
                ;

Terminal        :       String
                        { $$ = بیان_Create_String($1); }
                |       Integer
                        { $$ = بیان_Create_Integer($1); }
                |       Float
                        { $$ = بیان_Create_Float(/* Discard $1 */); }
                |       KEYNAME
                        { $$ = بیان_Create_کلی_Name($1); }
                ;

Multi_کلی_Sym_List :       Multi_کلی_Sym_List COMMA کلی_Sym_Lit
                        {
                            بیان_Def *بیان = بیان_Create_کلی_Sym_List($3);
                            $$ = $1;
                            $$.وروستی->common.بل = &بیان->common; $$.وروستی = بیان;
                        }
                |       Multi_کلی_Sym_List COMMA کلی_Syms
                        { $$ = $1; $$.وروستی->common.بل = &$3->common; $$.وروستی = $3; }
                |       کلی_Sym_Lit
                        { $$.سر = $$.وروستی = بیان_Create_کلی_Sym_List($1); }
                |       Non_تشه_کلی_Syms
                        { $$.سر = $$.وروستی = $1; }
                ;

کلی_Sym_List      :       کلی_Sym_List COMMA کلی_Sym_Lit
                        { $$ = بیان_Append_کلی_Sym_List($1, $3); }
                |       کلی_Sym_List COMMA STRING
                        {
                            $$ = بیان_کلی_Sym_List_Append_String(param->scanner, $1, $3);
                            حرر($3);
                            إذا (!$$)
                                YYERROR;
                        }
                |       کلی_Sym_Lit
                        {
                            $$ = بیان_Create_کلی_Sym_List($1);
                            إذا (!$$)
                                YYERROR;
                        }
                |       STRING
                        {
                            $$ = بیان_Create_کلی_Sym_List(XKB_کلی_No_نښه);
                            إذا (!$$)
                                YYERROR;
                            $$ = بیان_کلی_Sym_List_Append_String(param->scanner, $$, $1);
                            حرر($1);
                            إذا (!$$)
                                YYERROR;
                        }
                ;

Non_تشه_کلی_Syms :       OBRACE کلی_Sym_List CBRACE
                        { $$ = $2; }
                |       STRING
                        {
                            $$ = بیان_Create_کلی_Sym_List(XKB_کلی_No_نښه);
                            إذا (!$$)
                                YYERROR;
                            $$ = بیان_کلی_Sym_List_Append_String(param->scanner, $$, $1);
                            حرر($1);
                            إذا (!$$)
                                YYERROR;
                        }
                ;

کلی_Syms         :       Non_تشه_کلی_Syms
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = بیان_Create_کلی_Sym_List(XKB_کلی_No_نښه); }
                ;

کلی_Sym_2          :       کلی_Sym_Lit
                        { $$ = $1; }
                |       STRING
                        {
                            $$ = KeysymParseString(param->scanner, $1);
                            حرر($1);
                            إذا ($$ == XKB_کلی_No_نښه)
                                YYERROR;
                        }
                ;

کلی_Sym_Lit       :       IDENT
                        {
                            إذا (!resolve_keysym(param, $1, &$$)) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (بینښه صحيح) $1.len, $1.پیل
                                );
                                $$ = XKB_کلی_No_نښه;
                            }
                        }
                        /* Handle keysym that is also a keyword  */
                |       SECTION { $$ = XKB_کلی_section; }
                |       DECIMAL_DIGIT
                        {
                            /*
                             * Special case for digits 0..9:
                             * map to XKB_KEY_0 .. XKB_KEY_9, consistent with
                             * other keysym names: <name> → XKB_KEY_<name>.
                             */
                            $$ = XKB_کلی_0 + (xkb_keysym_t) $1;
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
                                $$ = XKB_کلی_No_نښه;
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
                                    $$ = XKB_کلی_No_نښه;
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

Signed_شمېره    :       MINUS شمېره    { $$ = -$2; }
                |       شمېره          { $$ = $1; }
                ;

شمېره          :       FLOAT         { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                |       INTEGER       { $$ = $1; }
                ;

Float           :       FLOAT   { $$ = 0; }
                ;

Integer         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

کلی_کوډ_2         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

Ident           :       IDENT   { $$ = xkb_atom_intern(param->ctx, $1.پیل, $1.len); }
                |       DEFAULT { $$ = xkb_atom_intern_literal(param->ctx, "default"); }
                ;

String          :       STRING  { $$ = xkb_atom_intern(param->ctx, $1, مزي_اوږدوالی($1)); حرر($1); }
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
    XkbFile *لومړی = عدم;
    بنية parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = عدم,
        .config = *config,
        .نور_maps = false,
    };

    /*
     * If we got a specific map, we look for it exclusively and return
     * immediately upon finding it. Otherwise, we need to get the
     * default map. If we find a map marked as default, we return it
     * immediately. If there are no maps marked as default, we return
     * the first map in the file.
     */

    طالما ((ret = yyparse(&param)) == 0 && param.نور_maps) {
        إذا (map) {
            إذا (streq_not_null(map, param.rtrn->name))
                أرجع param.rtrn;
            وإلا
                FreeXkbFile(param.rtrn);
        }
        وإلا {
            إذا (param.rtrn->flags & MAP_IS_DEFAULT) {
                FreeXkbFile(لومړی);
                أرجع param.rtrn;
            }
            وإلا إذا (!لومړی) {
                لومړی = param.rtrn;
            }
            وإلا {
                FreeXkbFile(param.rtrn);
            }
        }
        param.rtrn = عدم;
    }

    إذا (ret != 0) {
        /* Some error happend; clear the Xkbfiles parsed so far */
        FreeXkbFile(لومړی);
        FreeXkbFile(param.rtrn);
        أرجع عدم;
    }

    /*
     * Warn about implicit default section,
     * but only if not a keymap: multiple keymaps per file not supported
     */
    إذا (لومړی && لومړی->file_type != FILE_TYPE_KEYMAP)
        log_vrb(ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_MISSING_DEFAULT_SECTION,
                "No section name in include statement, but \"%s\" contains several; "
                "Using first defined section, \"%s\"\n",
                scanner->file_name, خوندي_map_name(لومړی->name));

    أرجع لومړی;
}

/* Parse the next section */
bool
parse_بل(بنية xkb_context *ctx,
           ثابت بنية parser_keymap_config *config,
           بنية scanner *scanner, XkbFile **xkb_file)
{
    صحيح ret;
    بنية parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = عدم,
        .config = *config,
        .نور_maps = false,
    };

    إذا ((ret = yyparse(&param)) == 0 && param.نور_maps) {
        *xkb_file = param.rtrn;
        أرجع true;
    } وإلا {
        FreeXkbFile(param.rtrn);
        *xkb_file = عدم;
        أرجع (ret == 0);
    }
}
