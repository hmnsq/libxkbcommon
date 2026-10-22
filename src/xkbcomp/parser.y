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
    bool وڌيڪ_maps;
};

#وصف_ڪريو parser_log_with_ڪوڊ(param, level, verbosity, log_نياپو_id, fmt, ...)   \
    scanner_log_with_ڪوڊ((param)->scanner, level, verbosity, log_نياپو_id, fmt,\
                          ##__VA_ARGS__)

#وصف_ڪريو parser_err(param, error_id, fmt, ...) \
    scanner_err((param)->scanner, error_id, fmt, ##__VA_ARGS__)

#وصف_ڪريو parser_warn(param, warning_id, fmt, ...) \
    scanner_warn((param)->scanner, warning_id, fmt, ##__VA_ARGS__)

#وصف_ڪريو parser_vrb(param, verbosity, warning_id, fmt, ...) \
    scanner_vrb((param)->scanner, verbosity, warning_id, fmt, ##__VA_ARGS__)

ساكن فراغ
_xkbcommon_error(بنية parser_param *param, ثابت حرف *نياپو)
{
    parser_err(param, XKB_ERROR_INVALID_XKB_SYNTAX, "%s", نياپو);
}

ساكن bool
resolve_keysym(بنية parser_param *param, بنية sval name, xkb_keysym_t *sym_rtrn)
{
    xkb_keysym_t sym;

    إذا (isvaleq(name, SVAL_LIT("any")) || isvaleq(name, SVAL_LIT("nosymbol"))) {
        *sym_rtrn = XKB_ڪنجي_No_نشان;
        أرجع true;
    }

    إذا (isvaleq(name, SVAL_LIT("none")) || isvaleq(name, SVAL_LIT("voidsymbol"))) {
        *sym_rtrn = XKB_ڪنجي_Void_نشان;
        أرجع true;
    }

    /* xkb_keysym_from_name needs a C string. */
    حرف buf[XKB_KEYSYM_NAME_MAX_SIZE];
    إذا (name.len >= حجم(buf)) {
        أرجع false;
    }
    يادگيري_نقل(buf, name.شروعات, name.len);
    buf[name.len] = '\0';

    sym = xkb_keysym_from_name(buf, XKB_KEYSYM_NO_FLAGS);
    إذا (sym != XKB_ڪنجي_No_نشان) {
        *sym_rtrn = sym;
        check_deprecated_keysyms(parser_warn, param, param->ctx,
                                 sym, buf, buf, "%s", "");
        أرجع true;
    }

    أرجع false;
}

#وصف_ڪريو param_scanner param->scanner
%}

%define api.خالص
%lex-param      { بنية scanner *param_scanner }
%parse-param    { بنية parser_param *param }

%define parse.error detailed

%token
        پڇاڙي_جو_FILE     0 "end of file"
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
        ڪنجي_3             24 "key"
        ALIAS           25 "alias"
        گروهه_2           26 "group"
        MODIFIER_MAP    27 "modifier_map"
        INDICATOR       28 "indicator"
        SHAPE           29 "shape"
        ڪنجيون_2            30 "keys"
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
        ALPHANUMERIC_ڪنجيون       73 "alphanumeric_keys"
        MODIFIER_ڪنجيون           74 "modifier_keys"
        KEYPAD_ڪنجيون             75 "keypad_keys"
        FUNCTION_ڪنجيون           76 "function_keys"
        ALTERNATE_گروهه         77 "alternate_group"
        DEPRECATED              78 "deprecated"

%ساڄي  EQUALS
%کاٻي   PLUS MINUS
%کاٻي   TIMES DIVIDE
%کاٻي   EXCLAM INVERT
%کاٻي   OPAREN

%start  XkbFile

%اتحاد  {
        int64_t          انگ;
        تعداد xkb_file_type file_type;
        حرف            *str;
        بنية sval     sval;
        xkb_atom_t      atom;
        تعداد ملايو_گهڻائي ملايو;
        تعداد xkb_map_flags mapFlags;
        xkb_keysym_t    keysym;
        ParseCommon     *any;
        بنية { ParseCommon *سر; ParseCommon *آخري; } anyList;
        uint32_t        no_نشان_Or_Action_List;
        اظهار_Def         *اظهار;
        بنية { اظهار_Def *سر; اظهار_Def *آخري; } اظهار_List;
        VarDef          *var;
        بنية { VarDef *سر; VarDef *آخري; } varList;
        VModDef         *vmod;
        بنية { VModDef *سر; VModDef *آخري; } vmodList;
        InterpDef       *interp;
        ڪنجي_Type_Def      *ڪنجي_Type;
        SymbolsDef      *syms;
        ModMapDef       *mod_نقاب_2;
        گروهه_Compat_Def  *گروهه_Compat;
        LedMapDef       *ledMap;
        LedNameDef      *ledName;
        KeycodeDef      *ڪنجي_ڪوڊ;
        ڪنجي_Alias_Def     *ڪنجي_Alias;
        UnknownStatement *unknown;
        فراغ            *geom;
        XkbFile         *file;
        بنية { XkbFile *سر; XkbFile *آخري; } fileList;
}

%نوع <انگ>     DECIMAL_DIGIT INTEGER FLOAT
%نوع <str>     STRING
%نوع <sval>    IDENT
%نوع <atom>    KEYNAME
%نوع <انگ>     ڪنجي_ڪوڊ_2 عدد Integer Float Signed_عدد DoodadType
%نوع <ملايو>   ملايو_گهڻائي_2 Opt_ملايو_گهڻائي
%نوع <file_type> Xkb_مرکب_Type FileType
%نوع <mapFlags> Flag Flags OptFlags
%نوع <str>     MapName OptMapName
%نوع <atom>    ميدان_Spec Ident Element String
%نوع <keysym>  ڪنجي_Sym_2 ڪنجي_Sym_Lit
%نوع <no_نشان_Or_Action_List> No_نشان_Or_Action_List
%نوع <any>     Decl
%نوع <anyList> DeclList
%نوع <اظهار>    اظهار_2 رڪن Lhs Terminal OptTerminal Coord CoordList
%نوع <اظهار>    Multi_ڪنجي_Sym_Or_Action_List Non_خالي_جايون_Actions Actions Action
%نوع <اظهار>    ڪنجي_Syms Non_خالي_جايون_ڪنجي_Syms ڪنجي_Sym_List ڪنجي_Or_ڪنجي_Sym
%نوع <اظهار_List> اظهار_List_2 ڪنجي_Or_ڪنجي_Sym_List
%نوع <اظهار_List> ActionList MultiActionList Multi_ڪنجي_Sym_List
%نوع <var>     VarDecl SymbolsVarDecl
%نوع <varList> VarDeclList SymbolsBody OptSymbolsBody
%نوع <vmod>    VModDef
%نوع <vmodList> VModDefList VModDecl
%نوع <interp>  InterpretDecl Interpret_ميلاپ
%نوع <ڪنجي_Type> ڪنجي_Type_Decl
%نوع <syms>    SymbolsDecl
%نوع <mod_نقاب_2> ModMapDecl
%نوع <گروهه_Compat> گروهه_Compat_Decl
%نوع <ledMap>  LedMapDecl
%نوع <ledName> LedNameDecl
%نوع <ڪنجي_ڪوڊ> ڪنجي_Name_Decl
%نوع <ڪنجي_Alias> ڪنجي_Alias_Decl
%نوع <geom>    ShapeDecl SectionDecl SectionBody Section_Body_شي صف_Body صف_Body_شي
%نوع <geom>    ڪنجيون_3 ڪنجي_4 OverlayDecl Overlay_ڪنجي_List Overlay_ڪنجي OutlineList Outline_اندريون_List
%نوع <geom>    DoodadDecl
%نوع <unknown> UnknownDecl UnknownCompoundStatementDecl
%نوع <file>    XkbFile XkbMapConfig
%نوع <fileList> XkbMapConfigList
%نوع <file>    Xkb_مرکب_Map

%destructor { FreeStmt((ParseCommon *) $$); }
    <any> <اظهار> <var> <vmod> <interp> <ڪنجي_Type> <syms> <mod_نقاب_2> <گروهه_Compat>
    <ledMap> <ledName> <ڪنجي_ڪوڊ> <ڪنجي_Alias>
%destructor { FreeStmt((ParseCommon *) $$.سر); }
    <anyList> <اظهار_List> <varList> <vmodList>
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
                        { $$ = param->rtrn = $1; param->وڌيڪ_maps = !!param->rtrn; (فراغ) yynerrs; }
                |       XkbMapConfig
                        { $$ = param->rtrn = $1; param->وڌيڪ_maps = !!param->rtrn; YYACCEPT; }
                |       پڇاڙي_جو_FILE
                        { $$ = param->rtrn = عدم; param->وڌيڪ_maps = false; }
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
                                    $$.آخري->common.ايندڙ = &$2->common;
                                    $$.آخري = $2;
                                } وإلا {
                                    $$.سر = $$.آخري = $2;
                                }
                            }
                        }
                |       { $$.سر = $$.آخري = عدم; }
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
                |       ALPHANUMERIC_ڪنجيون       { $$ = MAP_HAS_ALPHANUMERIC; }
                |       MODIFIER_ڪنجيون           { $$ = MAP_HAS_MODIFIER; }
                |       KEYPAD_ڪنجيون             { $$ = MAP_HAS_KEYPAD; }
                |       FUNCTION_ڪنجيون           { $$ = MAP_HAS_FN; }
                |       ALTERNATE_گروهه         { $$ = MAP_IS_ALTGR; }
                |       DEPRECATED              { $$ = MAP_IS_DEPRECATED; }
                |       IDENT
                        {
                            ثابت bool error = (param->config.strict & PARSER_NO_UNKNOWN_SECTION_FLAGS);
                            parser_log_with_ڪوڊ(
                                param, (error ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
                                XKB_LOG_VERBOSITY_گهٽتر,
                                XKB_ERROR_UNKNOWN_SECTION_FLAG,
                                "Unknown section flag \"%.*s\"%s",
                                (بنانشان)$1.len, $1.شروعات,
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
                                    $$.سر = $1.سر; $1.آخري->ايندڙ = $2; $$.آخري = $2;
                                } وإلا {
                                    $$.سر = $$.آخري = $2;
                                }
                            }
                        }
                        /*
                         * VModDecl is "inlined" directly into DeclList, i.e.
                         * each VModDef in the VModDecl is a separate Decl in
                         * the File.
                         */
                |       DeclList Opt_ملايو_گهڻائي VModDecl
                        {
                            لكل (VModDef *vmod = $3.سر; vmod; vmod = (VModDef *) vmod->common.ايندڙ)
                                vmod->ملايو = $2;
                            إذا ($1.سر) {
                                $$.سر = $1.سر; $1.آخري->ايندڙ = &$3.سر->common; $$.آخري = &$3.آخري->common;
                            } وإلا {
                                $$.سر = &$3.سر->common; $$.آخري = &$3.آخري->common;
                            }
                        }
                |       { $$.سر = $$.آخري = عدم; }
                ;

Decl            :       Opt_ملايو_گهڻائي VarDecl
                        {
                            $2->ملايو = $1;
                            $$ = (ParseCommon *) $2;
                        }
                /*      OptMergeMode VModDecl - see above. */
                |       Opt_ملايو_گهڻائي InterpretDecl
                        {
                            $2->ملايو = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ملايو_گهڻائي ڪنجي_Name_Decl
                        {
                            $2->ملايو = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ملايو_گهڻائي ڪنجي_Alias_Decl
                        {
                            $2->ملايو = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ملايو_گهڻائي ڪنجي_Type_Decl
                        {
                            $2->ملايو = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ملايو_گهڻائي SymbolsDecl
                        {
                            $2->ملايو = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ملايو_گهڻائي ModMapDecl
                        {
                            $2->ملايو = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ملايو_گهڻائي گروهه_Compat_Decl
                        {
                            $2->ملايو = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ملايو_گهڻائي LedMapDecl
                        {
                            $2->ملايو = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ملايو_گهڻائي LedNameDecl
                        {
                            $2->ملايو = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       Opt_ملايو_گهڻائي ShapeDecl          { $$ = عدم; }
                |       Opt_ملايو_گهڻائي SectionDecl        { $$ = عدم; }
                |       Opt_ملايو_گهڻائي DoodadDecl         { $$ = عدم; }
                |       Opt_ملايو_گهڻائي UnknownDecl
                            { $$ = (ParseCommon *) $2; }
                |       Opt_ملايو_گهڻائي UnknownCompoundStatementDecl
                            { $$ = (ParseCommon *) $2; }
                |       ملايو_گهڻائي_2 STRING
                        {
                            $$ = (ParseCommon *) IncludeCreate(param->ctx, $2, $1);
                            حرر($2);
                        }
                ;

VarDecl         :       Lhs EQUALS اظهار_2 SEMI
                        { $$ = VarCreate($1, $3); }
                |       Ident SEMI
                        { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident SEMI
                        { $$ = BoolVarCreate($2, false); }
                ;

ڪنجي_Name_Decl     :       KEYNAME EQUALS ڪنجي_ڪوڊ_2 SEMI
                        { $$ = KeycodeCreate($1, $3); }
                ;

ڪنجي_Alias_Decl    :       ALIAS KEYNAME EQUALS KEYNAME SEMI
                        { $$ = ڪنجي_Alias_Create($2, $4); }
                ;

VModDecl        :       VIRTUAL_MODS VModDefList SEMI
                        { $$ = $2; }
                ;

VModDefList     :       VModDefList COMMA VModDef
                        { $$.سر = $1.سر; $$.آخري->common.ايندڙ = &$3->common; $$.آخري = $3; }
                |       VModDef
                        { $$.سر = $$.آخري = $1; }
                ;

VModDef         :       Ident
                        { $$ = VModCreate($1, عدم); }
                |       Ident EQUALS اظهار_2
                        { $$ = VModCreate($1, $3); }
                ;

InterpretDecl   :       INTERPRET Interpret_ميلاپ OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $2->def = $4.سر; $$ = $2; }
                ;

Interpret_ميلاپ  :       ڪنجي_Sym_2 PLUS اظهار_2
                        { $$ = InterpCreate($1, $3); }
                |       ڪنجي_Sym_2
                        { $$ = InterpCreate($1, عدم); }
                ;

VarDeclList     :       VarDeclList VarDecl
                        {
                            إذا ($2) {
                                إذا ($1.سر) {
                                    $$.سر = $1.سر;
                                    $$.آخري->common.ايندڙ = &$2->common;
                                    $$.آخري = $2;
                                } وإلا {
                                    $$.سر = $$.آخري = $2;
                                }
                            }
                        }
                |       { $$.سر = $$.آخري = عدم; }
                ;

ڪنجي_Type_Decl     :       TYPE String OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $$ = ڪنجي_Type_Create($2, $4.سر); }
                ;

SymbolsDecl     :       ڪنجي_3 KEYNAME OBRACE
                            OptSymbolsBody
                        CBRACE SEMI
                        { $$ = SymbolsCreate($2, $4.سر); }
                ;

OptSymbolsBody  :       SymbolsBody { $$ = $1; }
                |                   { $$.سر = $$.آخري = عدم; }
                ;

SymbolsBody     :       SymbolsBody COMMA SymbolsVarDecl
                        { $$.سر = $1.سر; $$.آخري->common.ايندڙ = &$3->common; $$.آخري = $3; }
                |       SymbolsVarDecl
                        { $$.سر = $$.آخري = $1; }
                ;

SymbolsVarDecl  :       Lhs EQUALS اظهار_2         { $$ = VarCreate($1, $3); }
                |       Lhs EQUALS Multi_ڪنجي_Sym_Or_Action_List { $$ = VarCreate($1, $3); }
                |       Ident                   { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident            { $$ = BoolVarCreate($2, false); }
                |       Multi_ڪنجي_Sym_Or_Action_List { $$ = VarCreate(عدم, $1); }
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
Multi_ڪنجي_Sym_Or_Action_List
                :       OBRACKET Multi_ڪنجي_Sym_List CBRACKET
                        { $$ = $2.سر; }
                |       OBRACKET No_نشان_Or_Action_List COMMA Multi_ڪنجي_Sym_List CBRACKET
                        {
                            /* Prepend n times NoSymbol */
                            بنية {اظهار_Def *سر; اظهار_Def *آخري;} list = {
                                .سر = $4.سر, .آخري = $4.آخري
                            };
                            لكل (uint32_t k = 0; k < $2; k++) {
                                اظهار_Def* ثابت syms =
                                    اظهار_Create_ڪنجي_Sym_List(XKB_ڪنجي_No_نشان);
                                إذا (!syms) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                syms->common.ايندڙ = &list.سر->common;
                                list.سر = syms;
                            }
                            $$ = list.سر;
                        }
                |       OBRACKET MultiActionList CBRACKET
                        { $$ = $2.سر; }
                |       OBRACKET No_نشان_Or_Action_List COMMA MultiActionList CBRACKET
                        {
                            /* Prepend n times NoAction() */
                            بنية {اظهار_Def *سر; اظهار_Def *آخري;} list = {
                                .سر = $4.سر, .آخري = $4.آخري
                            };
                            لكل (uint32_t k = 0; k < $2; k++) {
                                اظهار_Def* ثابت acts = اظهار_Create_Action_List(عدم);
                                إذا (!acts) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                acts->common.ايندڙ = &list.سر->common;
                                list.سر = acts;
                            }
                            $$ = list.سر;
                        }
                        /*
                         * A list of *empty* keysym/action lists `{}`.
                         * There is not enough context here to decide if it is
                         * a `MultiKeySymList` or a `MultiActionList`.
                         */
                |       OBRACKET No_نشان_Or_Action_List CBRACKET
                        { $$ = اظهار_خالي_جايون_List(); }
                ;

/* A list of `{}`, which remains ambiguous until reaching a keysym or action list */
No_نشان_Or_Action_List
                :       No_نشان_Or_Action_List COMMA OBRACE CBRACE
                        { $$ = $1 + 1; }
                |       OBRACE CBRACE
                        { $$ = 1; }
                |       { $$ = 0; }
                ;

گروهه_Compat_Decl :       گروهه_2 Integer EQUALS اظهار_2 SEMI
                        { $$ = گروهه_Compat_Create($2, $4); }
                ;

ModMapDecl      :       MODIFIER_MAP اظهار_2 OBRACE ڪنجي_Or_ڪنجي_Sym_List CBRACE SEMI
                        {
                            إذا (param->config.format == XKB_KEYMAP_FORMAT_متن_V1 &&
                                $2->common.نوع != STMT_اظهار_IDENT) {
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

ڪنجي_Or_ڪنجي_Sym_List :       ڪنجي_Or_ڪنجي_Sym_List COMMA ڪنجي_Or_ڪنجي_Sym
                        { $$.سر = $1.سر; $$.آخري->common.ايندڙ = &$3->common; $$.آخري = $3; }
                |       ڪنجي_Or_ڪنجي_Sym
                        { $$.سر = $$.آخري = $1; }
                ;

ڪنجي_Or_ڪنجي_Sym     :       KEYNAME
                        { $$ = اظهار_Create_ڪنجي_Name($1); }
                |       ڪنجي_Sym_2
                        { $$ = اظهار_Create_ڪنجي_Sym($1); }
                ;

LedMapDecl:             INDICATOR String OBRACE VarDeclList CBRACE SEMI
                        { $$ = LedMapCreate($2, $4.سر); }
                ;

LedNameDecl:            INDICATOR Integer EQUALS اظهار_2 SEMI
                        { $$ = LedNameCreate($2, $4, false); }
                |       VIRTUAL INDICATOR Integer EQUALS اظهار_2 SEMI
                        { $$ = LedNameCreate($3, $5, true); }
                ;

UnknownDecl     :       IDENT Terminal EQUALS اظهار_2 SEMI
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

SectionBody     :       SectionBody Section_Body_شي     { $$ = عدم;}
                |       Section_Body_شي                 { $$ = عدم; }
                ;

Section_Body_شي :       صف OBRACE صف_Body CBRACE SEMI
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

صف_Body         :       صف_Body صف_Body_شي     { $$ = عدم;}
                |       صف_Body_شي             { $$ = عدم; }
                ;

صف_Body_شي     :       ڪنجيون_2 OBRACE ڪنجيون_3 CBRACE SEMI { $$ = عدم; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = عدم; }
                ;

ڪنجيون_3            :       ڪنجيون_3 COMMA ڪنجي_4          { $$ = عدم; }
                |       ڪنجي_4                     { $$ = عدم; }
                ;

ڪنجي_4             :       KEYNAME
                        { $$ = عدم; }
                |       OBRACE اظهار_List_2 CBRACE
                        { FreeStmt((ParseCommon *) $2.سر); $$ = عدم; }
                ;

OverlayDecl     :       OVERLAY String OBRACE Overlay_ڪنجي_List CBRACE SEMI
                        { $$ = عدم; }
                ;

Overlay_ڪنجي_List  :       Overlay_ڪنجي_List COMMA Overlay_ڪنجي { $$ = عدم; }
                |       Overlay_ڪنجي                      { $$ = عدم; }
                ;

Overlay_ڪنجي      :       KEYNAME EQUALS KEYNAME          { $$ = عدم; }
                ;

OutlineList     :       OutlineList COMMA Outline_اندريون_List
                        { $$ = عدم;}
                |       Outline_اندريون_List
                        { $$ = عدم; }
                ;

Outline_اندريون_List   :       OBRACE CoordList CBRACE
                        { (فراغ) $2; $$ = عدم; }
                |       Ident EQUALS OBRACE CoordList CBRACE
                        { (فراغ) $4; $$ = عدم; }
                |       Ident EQUALS اظهار_2
                        { FreeStmt((ParseCommon *) $3); $$ = عدم; }
                ;

CoordList       :       CoordList COMMA Coord
                        { (فراغ) $1; (فراغ) $3; $$ = عدم; }
                |       Coord
                        { (فراغ) $1; $$ = عدم; }
                ;

Coord           :       OBRACKET Signed_عدد COMMA Signed_عدد CBRACKET
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

ميدان_Spec       :       Ident   { $$ = $1; }
                |       Element { $$ = $1; }
                ;

Element         :       ACTION_TOK
                        { $$ = xkb_atom_intern_literal(param->ctx, "action"); }
                |       INTERPRET
                        { $$ = xkb_atom_intern_literal(param->ctx, "interpret"); }
                |       TYPE
                        { $$ = xkb_atom_intern_literal(param->ctx, "type"); }
                |       ڪنجي_3
                        { $$ = xkb_atom_intern_literal(param->ctx, "key"); }
                |       گروهه_2
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

Opt_ملايو_گهڻائي    :       ملايو_گهڻائي_2       { $$ = $1; }
                |                       { $$ = ملايو_DEFAULT; }
                ;

ملايو_گهڻائي_2       :       INCLUDE         { $$ = ملايو_DEFAULT; }
                |       AUGMENT         { $$ = ملايو_AUGMENT; }
                |       OVERRIDE        { $$ = ملايو_OVERRIDE; }
                |       REPLACE         { $$ = ملايو_REPLACE; }
                |       ALTERNATE
                {
                    /*
                     * This used to be MERGE_ALT_FORM. This functionality was
                     * unused and has been removed.
                     */
                    parser_warn(param, XKB_LOG_پيغام_NO_ID,
                                "ignored unsupported legacy merge mode \"alternate\"");
                    $$ = ملايو_DEFAULT;
                }
                ;

اظهار_List_2        :       اظهار_List_2 COMMA اظهار_2
                        {
                            إذا ($3) {
                                إذا ($1.سر) {
                                    $$.سر = $1.سر;
                                    $$.آخري->common.ايندڙ = &$3->common;
                                    $$.آخري = $3;
                                } وإلا {
                                    $$.سر = $$.آخري = $3;
                                }
                            }
                        }
                |       اظهار_2
                        { $$.سر = $$.آخري = $1; }
                |       { $$.سر = $$.آخري = عدم; }
                ;

اظهار_2            :       اظهار_2 DIVIDE اظهار_2
                        { $$ = اظهار_Create_ٻٽو(STMT_اظهار_DIVIDE, $1, $3); }
                |       اظهار_2 PLUS اظهار_2
                        { $$ = اظهار_Create_ٻٽو(STMT_اظهار_جوڙيو, $1, $3); }
                |       اظهار_2 MINUS اظهار_2
                        { $$ = اظهار_Create_ٻٽو(STMT_اظهار_ڪٽيو, $1, $3); }
                |       اظهار_2 TIMES اظهار_2
                        { $$ = اظهار_Create_ٻٽو(STMT_اظهار_MULTIPLY, $1, $3); }
                |       Lhs EQUALS اظهار_2
                        { $$ = اظهار_Create_ٻٽو(STMT_اظهار_سونپيو, $1, $3); }
                |       رڪن
                        { $$ = $1; }
                ;

رڪن            :       MINUS رڪن
                        { $$ = اظهار_Create_Unary(STMT_اظهار_NEGATE, $2); }
                |       PLUS رڪن
                        { $$ = اظهار_Create_Unary(STMT_اظهار_UNARY_PLUS, $2); }
                |       EXCLAM رڪن
                        { $$ = اظهار_Create_Unary(STMT_اظهار_NOT, $2); }
                |       INVERT رڪن
                        { $$ = اظهار_Create_Unary(STMT_اظهار_INVERT, $2); }
                |       Lhs
                        { $$ = $1; }
                |       ميدان_Spec OPAREN اظهار_List_2 CPAREN %prec OPAREN
                        { $$ = اظهار_Create_Action($1, $3.سر); }
                |       Actions
                        { $$ = $1; }
                |       Terminal
                        { $$ = $1; }
                |       OPAREN اظهار_2 CPAREN
                        { $$ = $2; }
                ;

MultiActionList :       MultiActionList COMMA Action
                        {
                            اظهار_Def *اظهار = اظهار_Create_Action_List($3);
                            $$ = $1;
                            $$.آخري->common.ايندڙ = &اظهار->common; $$.آخري = اظهار;
                        }
                |       MultiActionList COMMA Actions
                        { $$ = $1; $$.آخري->common.ايندڙ = &$3->common; $$.آخري = $3; }
                |       Action
                        { $$.سر = $$.آخري = اظهار_Create_Action_List($1); }
                |       Non_خالي_جايون_Actions
                        { $$.سر = $$.آخري = $1; }
                ;

ActionList      :       ActionList COMMA Action
                        { $$ = $1; $$.آخري->common.ايندڙ = &$3->common; $$.آخري = $3; }
                |       Action
                        { $$.سر = $$.آخري = $1; }
                ;

Non_خالي_جايون_Actions :       OBRACE ActionList CBRACE
                        { $$ = اظهار_Create_Action_List($2.سر); }
                ;

Actions         :       Non_خالي_جايون_Actions
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = اظهار_Create_Action_List(عدم); }
                ;

Action          :       ميدان_Spec OPAREN اظهار_List_2 CPAREN
                        { $$ = اظهار_Create_Action($1, $3.سر); }
                ;

Lhs             :       ميدان_Spec
                        { $$ = اظهار_Create_Ident($1); }
                |       ميدان_Spec DOT ميدان_Spec
                        { $$ = اظهار_Create_ميدان_Ref($1, $3); }
                |       ميدان_Spec OBRACKET اظهار_2 CBRACKET
                        { $$ = اظهار_Create_Array_Ref(XKB_ATOM_NONE, $1, $3); }
                |       ميدان_Spec DOT ميدان_Spec OBRACKET اظهار_2 CBRACKET
                        { $$ = اظهار_Create_Array_Ref($1, $3, $5); }
                ;

OptTerminal     :       Terminal
                        { $$ = $1; }
                |       { $$ = عدم; }
                ;

Terminal        :       String
                        { $$ = اظهار_Create_String($1); }
                |       Integer
                        { $$ = اظهار_Create_Integer($1); }
                |       Float
                        { $$ = اظهار_Create_Float(/* Discard $1 */); }
                |       KEYNAME
                        { $$ = اظهار_Create_ڪنجي_Name($1); }
                ;

Multi_ڪنجي_Sym_List :       Multi_ڪنجي_Sym_List COMMA ڪنجي_Sym_Lit
                        {
                            اظهار_Def *اظهار = اظهار_Create_ڪنجي_Sym_List($3);
                            $$ = $1;
                            $$.آخري->common.ايندڙ = &اظهار->common; $$.آخري = اظهار;
                        }
                |       Multi_ڪنجي_Sym_List COMMA ڪنجي_Syms
                        { $$ = $1; $$.آخري->common.ايندڙ = &$3->common; $$.آخري = $3; }
                |       ڪنجي_Sym_Lit
                        { $$.سر = $$.آخري = اظهار_Create_ڪنجي_Sym_List($1); }
                |       Non_خالي_جايون_ڪنجي_Syms
                        { $$.سر = $$.آخري = $1; }
                ;

ڪنجي_Sym_List      :       ڪنجي_Sym_List COMMA ڪنجي_Sym_Lit
                        { $$ = اظهار_Append_ڪنجي_Sym_List($1, $3); }
                |       ڪنجي_Sym_List COMMA STRING
                        {
                            $$ = اظهار_ڪنجي_Sym_List_Append_String(param->scanner, $1, $3);
                            حرر($3);
                            إذا (!$$)
                                YYERROR;
                        }
                |       ڪنجي_Sym_Lit
                        {
                            $$ = اظهار_Create_ڪنجي_Sym_List($1);
                            إذا (!$$)
                                YYERROR;
                        }
                |       STRING
                        {
                            $$ = اظهار_Create_ڪنجي_Sym_List(XKB_ڪنجي_No_نشان);
                            إذا (!$$)
                                YYERROR;
                            $$ = اظهار_ڪنجي_Sym_List_Append_String(param->scanner, $$, $1);
                            حرر($1);
                            إذا (!$$)
                                YYERROR;
                        }
                ;

Non_خالي_جايون_ڪنجي_Syms :       OBRACE ڪنجي_Sym_List CBRACE
                        { $$ = $2; }
                |       STRING
                        {
                            $$ = اظهار_Create_ڪنجي_Sym_List(XKB_ڪنجي_No_نشان);
                            إذا (!$$)
                                YYERROR;
                            $$ = اظهار_ڪنجي_Sym_List_Append_String(param->scanner, $$, $1);
                            حرر($1);
                            إذا (!$$)
                                YYERROR;
                        }
                ;

ڪنجي_Syms         :       Non_خالي_جايون_ڪنجي_Syms
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = اظهار_Create_ڪنجي_Sym_List(XKB_ڪنجي_No_نشان); }
                ;

ڪنجي_Sym_2          :       ڪنجي_Sym_Lit
                        { $$ = $1; }
                |       STRING
                        {
                            $$ = KeysymParseString(param->scanner, $1);
                            حرر($1);
                            إذا ($$ == XKB_ڪنجي_No_نشان)
                                YYERROR;
                        }
                ;

ڪنجي_Sym_Lit       :       IDENT
                        {
                            إذا (!resolve_keysym(param, $1, &$$)) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (بنانشان صحيح) $1.len, $1.شروعات
                                );
                                $$ = XKB_ڪنجي_No_نشان;
                            }
                        }
                        /* Handle keysym that is also a keyword  */
                |       SECTION { $$ = XKB_ڪنجي_section; }
                |       DECIMAL_DIGIT
                        {
                            /*
                             * Special case for digits 0..9:
                             * map to XKB_KEY_0 .. XKB_KEY_9, consistent with
                             * other keysym names: <name> → XKB_KEY_<name>.
                             */
                            $$ = XKB_ڪنجي_0 + (xkb_keysym_t) $1;
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
                                $$ = XKB_ڪنجي_No_نشان;
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
                                    $$ = XKB_ڪنجي_No_نشان;
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

Signed_عدد    :       MINUS عدد    { $$ = -$2; }
                |       عدد          { $$ = $1; }
                ;

عدد          :       FLOAT         { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                |       INTEGER       { $$ = $1; }
                ;

Float           :       FLOAT   { $$ = 0; }
                ;

Integer         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

ڪنجي_ڪوڊ_2         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

Ident           :       IDENT   { $$ = xkb_atom_intern(param->ctx, $1.شروعات, $1.len); }
                |       DEFAULT { $$ = xkb_atom_intern_literal(param->ctx, "default"); }
                ;

String          :       STRING  { $$ = xkb_atom_intern(param->ctx, $1, لڙي_ڊيگهه($1)); حرر($1); }
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
    XkbFile *پهريون = عدم;
    بنية parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = عدم,
        .config = *config,
        .وڌيڪ_maps = false,
    };

    /*
     * If we got a specific map, we look for it exclusively and return
     * immediately upon finding it. Otherwise, we need to get the
     * default map. If we find a map marked as default, we return it
     * immediately. If there are no maps marked as default, we return
     * the first map in the file.
     */

    طالما ((ret = yyparse(&param)) == 0 && param.وڌيڪ_maps) {
        إذا (map) {
            إذا (streq_not_null(map, param.rtrn->name))
                أرجع param.rtrn;
            وإلا
                FreeXkbFile(param.rtrn);
        }
        وإلا {
            إذا (param.rtrn->flags & MAP_IS_DEFAULT) {
                FreeXkbFile(پهريون);
                أرجع param.rtrn;
            }
            وإلا إذا (!پهريون) {
                پهريون = param.rtrn;
            }
            وإلا {
                FreeXkbFile(param.rtrn);
            }
        }
        param.rtrn = عدم;
    }

    إذا (ret != 0) {
        /* Some error happend; clear the Xkbfiles parsed so far */
        FreeXkbFile(پهريون);
        FreeXkbFile(param.rtrn);
        أرجع عدم;
    }

    /*
     * Warn about implicit default section,
     * but only if not a keymap: multiple keymaps per file not supported
     */
    إذا (پهريون && پهريون->file_type != FILE_TYPE_KEYMAP)
        log_vrb(ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_MISSING_DEFAULT_SECTION,
                "No section name in include statement, but \"%s\" contains several; "
                "Using first defined section, \"%s\"\n",
                scanner->file_name, محفوظ_map_name(پهريون->name));

    أرجع پهريون;
}

/* Parse the next section */
bool
parse_ايندڙ(بنية xkb_context *ctx,
           ثابت بنية parser_keymap_config *config,
           بنية scanner *scanner, XkbFile **xkb_file)
{
    صحيح ret;
    بنية parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = عدم,
        .config = *config,
        .وڌيڪ_maps = false,
    };

    إذا ((ret = yyparse(&param)) == 0 && param.وڌيڪ_maps) {
        *xkb_file = param.rtrn;
        أرجع true;
    } وإلا {
        FreeXkbFile(param.rtrn);
        *xkb_file = عدم;
        أرجع (ret == 0);
    }
}
