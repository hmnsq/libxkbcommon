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
#شامل "config.h"

#شامل "scanner-utils.h"
#شامل "xkbcomp/ast.h"
#شامل "xkbcomp/xkbcomp-priv.h"
}

%{
#شامل "config.h"

#شامل <assert.h>

#شامل "keysym.h"
#شامل "xkbcomp/xkbcomp-priv.h"
#شامل "xkbcomp/parser-priv.h"
#شامل "xkbcomp/ast-build.h"

ساختار parser_param {
    ساختار xkb_context *ctx;
    ساختار scanner *scanner;
    XkbFile *rtrn;
    ساختار parser_keymap_config config;
    bool more_maps;
};

#define parser_log_with_code(param, level, verbosity, log_پیام_id, fmt, ...)   \
    scanner_log_with_code((param)->scanner, level, verbosity, log_پیام_id, fmt,\
                          ##__VA_ARGS__)

#define parser_err(param, error_id, fmt, ...) \
    scanner_err((param)->scanner, error_id, fmt, ##__VA_ARGS__)

#define parser_warn(param, warning_id, fmt, ...) \
    scanner_warn((param)->scanner, warning_id, fmt, ##__VA_ARGS__)

#define parser_vrb(param, verbosity, warning_id, fmt, ...) \
    scanner_vrb((param)->scanner, verbosity, warning_id, fmt, ##__VA_ARGS__)

ایستا تهی
_xkbcommon_error(ساختار parser_param *param, ثابت نویسه *پیام)
{
    parser_err(param, XKB_ERROR_INVALID_XKB_SYNTAX, "%s", پیام);
}

ایستا bool
resolve_keysym(ساختار parser_param *param, ساختار sval name, xkb_keysym_t *sym_rtrn)
{
    xkb_keysym_t sym;

    اگر (isvaleq(name, SVAL_LIT("any")) || isvaleq(name, SVAL_LIT("nosymbol"))) {
        *sym_rtrn = XKB_کلید_No_نماد;
        بازگشت true;
    }

    اگر (isvaleq(name, SVAL_LIT("none")) || isvaleq(name, SVAL_LIT("voidsymbol"))) {
        *sym_rtrn = XKB_کلید_Void_نماد;
        بازگشت true;
    }

    /* xkb_keysym_from_name needs a C string. */
    نویسه buf[XKB_KEYSYM_NAME_MAX_SIZE];
    اگر (name.len >= اندازه(buf)) {
        بازگشت false;
    }
    memcpy(buf, name.start, name.len);
    buf[name.len] = '\0';

    sym = xkb_keysym_from_name(buf, XKB_KEYSYM_NO_FLAGS);
    اگر (sym != XKB_کلید_No_نماد) {
        *sym_rtrn = sym;
        check_deprecated_keysyms(parser_warn, param, param->ctx,
                                 sym, buf, buf, "%s", "");
        بازگشت true;
    }

    بازگشت false;
}

#define param_scanner param->scanner
%}

%define api.pure
%lex-param      { ساختار scanner *param_scanner }
%parse-param    { ساختار parser_param *param }

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
        کلید_3             24 "key"
        ALIAS           25 "alias"
        GROUP           26 "group"
        MODIFIER_MAP    27 "modifier_map"
        INDICATOR       28 "indicator"
        SHAPE           29 "shape"
        کلیدها_2            30 "keys"
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
        ALPHANUMERIC_کلیدها       73 "alphanumeric_keys"
        MODIFIER_کلیدها           74 "modifier_keys"
        KEYPAD_کلیدها             75 "keypad_keys"
        FUNCTION_کلیدها           76 "function_keys"
        ALTERNATE_GROUP         77 "alternate_group"
        DEPRECATED              78 "deprecated"

%right  EQUALS
%left   PLUS MINUS
%left   TIMES DIVIDE
%left   EXCLAM INVERT
%left   OPAREN

%start  XkbFile

%اجتماع  {
        int64_t          رقم;
        شمارش xkb_file_type file_type;
        نویسه            *str;
        ساختار sval     sval;
        xkb_atom_t      atom;
        شمارش merge_mode merge;
        شمارش xkb_map_flags mapFlags;
        xkb_keysym_t    keysym;
        ParseCommon     *any;
        ساختار { ParseCommon *head; ParseCommon *last; } anyList;
        uint32_t        no_نماد_Or_Action_List;
        عبارت_Def         *عبارت;
        ساختار { عبارت_Def *head; عبارت_Def *last; } عبارت_List;
        VarDef          *var;
        ساختار { VarDef *head; VarDef *last; } varList;
        VModDef         *vmod;
        ساختار { VModDef *head; VModDef *last; } vmodList;
        InterpDef       *interp;
        کلید_Type_Def      *کلید_Type;
        SymbolsDef      *syms;
        ModMapDef       *modMask;
        GroupCompatDef  *groupCompat;
        LedMapDef       *ledMap;
        LedNameDef      *ledName;
        KeycodeDef      *کلید_Code;
        کلید_Alias_Def     *کلید_Alias;
        UnknownStatement *unknown;
        تهی            *geom;
        XkbFile         *file;
        ساختار { XkbFile *head; XkbFile *last; } fileList;
}

%نوع <رقم>     DECIMAL_DIGIT INTEGER FLOAT
%نوع <str>     STRING
%نوع <sval>    IDENT
%نوع <atom>    KEYNAME
%نوع <رقم>     کلید_Code_2 عدد Integer Float Signed_عدد DoodadType
%نوع <merge>   MergeMode OptMergeMode
%نوع <file_type> XkbCompositeType FileType
%نوع <mapFlags> Flag Flags OptFlags
%نوع <str>     MapName OptMapName
%نوع <atom>    FieldSpec Ident Element String
%نوع <keysym>  کلید_Sym_2 کلید_Sym_Lit
%نوع <no_نماد_Or_Action_List> No_نماد_Or_Action_List
%نوع <any>     Decl
%نوع <anyList> DeclList
%نوع <عبارت>    عبارت_2 Term Lhs Terminal OptTerminal Coord CoordList
%نوع <عبارت>    Multi_کلید_Sym_Or_Action_List NonEmptyActions Actions Action
%نوع <عبارت>    کلید_Syms Non_Empty_کلید_Syms کلید_Sym_List کلید_Or_کلید_Sym
%نوع <عبارت_List> عبارت_List_2 کلید_Or_کلید_Sym_List
%نوع <عبارت_List> ActionList MultiActionList Multi_کلید_Sym_List
%نوع <var>     VarDecl SymbolsVarDecl
%نوع <varList> VarDeclList SymbolsBody OptSymbolsBody
%نوع <vmod>    VModDef
%نوع <vmodList> VModDefList VModDecl
%نوع <interp>  InterpretDecl InterpretMatch
%نوع <کلید_Type> کلید_Type_Decl
%نوع <syms>    SymbolsDecl
%نوع <modMask> ModMapDecl
%نوع <groupCompat> GroupCompatDecl
%نوع <ledMap>  LedMapDecl
%نوع <ledName> LedNameDecl
%نوع <کلید_Code> کلید_Name_Decl
%نوع <کلید_Alias> کلید_Alias_Decl
%نوع <geom>    ShapeDecl SectionDecl SectionBody SectionBodyItem RowBody RowBodyItem
%نوع <geom>    کلیدها_3 کلید_4 OverlayDecl Overlay_کلید_List Overlay_کلید OutlineList OutlineInList
%نوع <geom>    DoodadDecl
%نوع <unknown> UnknownDecl UnknownCompoundStatementDecl
%نوع <file>    XkbFile XkbMapConfig
%نوع <fileList> XkbMapConfigList
%نوع <file>    XkbCompositeMap

%destructor { FreeStmt((ParseCommon *) $$); }
    <any> <عبارت> <var> <vmod> <interp> <کلید_Type> <syms> <modMask> <groupCompat>
    <ledMap> <ledName> <کلید_Code> <کلید_Alias>
%destructor { FreeStmt((ParseCommon *) $$.head); }
    <anyList> <عبارت_List> <varList> <vmodList>
/* The destructor also runs on the start symbol when the parser *succeeds*.
 * The `if` here catches this case. */
%destructor { اگر (!param->rtrn) FreeXkbFile($$); } <file>
%destructor { FreeXkbFile($$.head); } <fileList>
%destructor { آزاد($$); } <str>

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
                        { $$ = param->rtrn = $1; param->more_maps = !!param->rtrn; (تهی) yynerrs; }
                |       XkbMapConfig
                        { $$ = param->rtrn = $1; param->more_maps = !!param->rtrn; YYACCEPT; }
                |       END_OF_FILE
                        { $$ = param->rtrn = پوچ; param->more_maps = false; }
                ;

XkbCompositeMap :       OptFlags XkbCompositeType OptMapName OBRACE
                            XkbMapConfigList
                        CBRACE SEMI
                        { $$ = XkbFileCreate($2, $3, (ParseCommon *) $5.head, $1); }
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
                            اگر ($2) {
                                اگر ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$2->common;
                                    $$.last = $2;
                                } وگرنه {
                                    $$.head = $$.last = $2;
                                }
                            }
                        }
                |       { $$.head = $$.last = پوچ; }
                ;

XkbMapConfig    :       OptFlags FileType OptMapName OBRACE
                            DeclList
                        CBRACE SEMI
                        {
                            اگر ($1 & MAP_IS_DEPRECATED) {
                                parser_warn(param, XKB_WARNING_DEPRECATED_SECTION,
                                            "deprecated section: \"%s\"",
                                            safe_map_name($3));
                            }
                            $$ = XkbFileCreate($2, $3, $5.head, $1);
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
                |       ALPHANUMERIC_کلیدها       { $$ = MAP_HAS_ALPHANUMERIC; }
                |       MODIFIER_کلیدها           { $$ = MAP_HAS_MODIFIER; }
                |       KEYPAD_کلیدها             { $$ = MAP_HAS_KEYPAD; }
                |       FUNCTION_کلیدها           { $$ = MAP_HAS_FN; }
                |       ALTERNATE_GROUP         { $$ = MAP_IS_ALTGR; }
                |       DEPRECATED              { $$ = MAP_IS_DEPRECATED; }
                |       IDENT
                        {
                            ثابت bool error = (param->config.strict & PARSER_NO_UNKNOWN_SECTION_FLAGS);
                            parser_log_with_code(
                                param, (error ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
                                XKB_LOG_VERBOSITY_MINIMAL,
                                XKB_ERROR_UNKNOWN_SECTION_FLAG,
                                "Unknown section flag \"%.*s\"%s",
                                (unsigned)$1.len, $1.start,
                                (error ? "" : "; ignored")
                            );
                            اگر (error)
                                YYABORT;
                            $$ = 0;
                        }
                ;

DeclList        :       DeclList Decl
                        {
                            اگر ($2) {
                                اگر ($1.head) {
                                    $$.head = $1.head; $1.last->next = $2; $$.last = $2;
                                } وگرنه {
                                    $$.head = $$.last = $2;
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
                            برای (VModDef *vmod = $3.head; vmod; vmod = (VModDef *) vmod->common.next)
                                vmod->merge = $2;
                            اگر ($1.head) {
                                $$.head = $1.head; $1.last->next = &$3.head->common; $$.last = &$3.last->common;
                            } وگرنه {
                                $$.head = &$3.head->common; $$.last = &$3.last->common;
                            }
                        }
                |       { $$.head = $$.last = پوچ; }
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
                |       OptMergeMode کلید_Name_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode کلید_Alias_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode کلید_Type_Decl
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
                |       OptMergeMode ShapeDecl          { $$ = پوچ; }
                |       OptMergeMode SectionDecl        { $$ = پوچ; }
                |       OptMergeMode DoodadDecl         { $$ = پوچ; }
                |       OptMergeMode UnknownDecl
                            { $$ = (ParseCommon *) $2; }
                |       OptMergeMode UnknownCompoundStatementDecl
                            { $$ = (ParseCommon *) $2; }
                |       MergeMode STRING
                        {
                            $$ = (ParseCommon *) IncludeCreate(param->ctx, $2, $1);
                            آزاد($2);
                        }
                ;

VarDecl         :       Lhs EQUALS عبارت_2 SEMI
                        { $$ = VarCreate($1, $3); }
                |       Ident SEMI
                        { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident SEMI
                        { $$ = BoolVarCreate($2, false); }
                ;

کلید_Name_Decl     :       KEYNAME EQUALS کلید_Code_2 SEMI
                        { $$ = KeycodeCreate($1, $3); }
                ;

کلید_Alias_Decl    :       ALIAS KEYNAME EQUALS KEYNAME SEMI
                        { $$ = کلید_Alias_Create($2, $4); }
                ;

VModDecl        :       VIRTUAL_MODS VModDefList SEMI
                        { $$ = $2; }
                ;

VModDefList     :       VModDefList COMMA VModDef
                        { $$.head = $1.head; $$.last->common.next = &$3->common; $$.last = $3; }
                |       VModDef
                        { $$.head = $$.last = $1; }
                ;

VModDef         :       Ident
                        { $$ = VModCreate($1, پوچ); }
                |       Ident EQUALS عبارت_2
                        { $$ = VModCreate($1, $3); }
                ;

InterpretDecl   :       INTERPRET InterpretMatch OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $2->def = $4.head; $$ = $2; }
                ;

InterpretMatch  :       کلید_Sym_2 PLUS عبارت_2
                        { $$ = InterpCreate($1, $3); }
                |       کلید_Sym_2
                        { $$ = InterpCreate($1, پوچ); }
                ;

VarDeclList     :       VarDeclList VarDecl
                        {
                            اگر ($2) {
                                اگر ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$2->common;
                                    $$.last = $2;
                                } وگرنه {
                                    $$.head = $$.last = $2;
                                }
                            }
                        }
                |       { $$.head = $$.last = پوچ; }
                ;

کلید_Type_Decl     :       TYPE String OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $$ = کلید_Type_Create($2, $4.head); }
                ;

SymbolsDecl     :       کلید_3 KEYNAME OBRACE
                            OptSymbolsBody
                        CBRACE SEMI
                        { $$ = SymbolsCreate($2, $4.head); }
                ;

OptSymbolsBody  :       SymbolsBody { $$ = $1; }
                |                   { $$.head = $$.last = پوچ; }
                ;

SymbolsBody     :       SymbolsBody COMMA SymbolsVarDecl
                        { $$.head = $1.head; $$.last->common.next = &$3->common; $$.last = $3; }
                |       SymbolsVarDecl
                        { $$.head = $$.last = $1; }
                ;

SymbolsVarDecl  :       Lhs EQUALS عبارت_2         { $$ = VarCreate($1, $3); }
                |       Lhs EQUALS Multi_کلید_Sym_Or_Action_List { $$ = VarCreate($1, $3); }
                |       Ident                   { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident            { $$ = BoolVarCreate($2, false); }
                |       Multi_کلید_Sym_Or_Action_List { $$ = VarCreate(پوچ, $1); }
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
                        { $$ = $2.head; }
                |       OBRACKET No_نماد_Or_Action_List COMMA Multi_کلید_Sym_List CBRACKET
                        {
                            /* Prepend n times NoSymbol */
                            ساختار {عبارت_Def *head; عبارت_Def *last;} list = {
                                .head = $4.head, .last = $4.last
                            };
                            برای (uint32_t k = 0; k < $2; k++) {
                                عبارت_Def* ثابت syms =
                                    عبارت_Create_کلید_Sym_List(XKB_کلید_No_نماد);
                                اگر (!syms) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                syms->common.next = &list.head->common;
                                list.head = syms;
                            }
                            $$ = list.head;
                        }
                |       OBRACKET MultiActionList CBRACKET
                        { $$ = $2.head; }
                |       OBRACKET No_نماد_Or_Action_List COMMA MultiActionList CBRACKET
                        {
                            /* Prepend n times NoAction() */
                            ساختار {عبارت_Def *head; عبارت_Def *last;} list = {
                                .head = $4.head, .last = $4.last
                            };
                            برای (uint32_t k = 0; k < $2; k++) {
                                عبارت_Def* ثابت acts = عبارت_Create_Action_List(پوچ);
                                اگر (!acts) {
                                    /* TODO: Use Bison’s more appropriate YYNOMEM */
                                    YYABORT;
                                }
                                acts->common.next = &list.head->common;
                                list.head = acts;
                            }
                            $$ = list.head;
                        }
                        /*
                         * A list of *empty* keysym/action lists `{}`.
                         * There is not enough context here to decide if it is
                         * a `MultiKeySymList` or a `MultiActionList`.
                         */
                |       OBRACKET No_نماد_Or_Action_List CBRACKET
                        { $$ = عبارت_Empty_List(); }
                ;

/* A list of `{}`, which remains ambiguous until reaching a keysym or action list */
No_نماد_Or_Action_List
                :       No_نماد_Or_Action_List COMMA OBRACE CBRACE
                        { $$ = $1 + 1; }
                |       OBRACE CBRACE
                        { $$ = 1; }
                |       { $$ = 0; }
                ;

GroupCompatDecl :       GROUP Integer EQUALS عبارت_2 SEMI
                        { $$ = GroupCompatCreate($2, $4); }
                ;

ModMapDecl      :       MODIFIER_MAP عبارت_2 OBRACE کلید_Or_کلید_Sym_List CBRACE SEMI
                        {
                            اگر (param->config.format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
                                $2->common.نوع != STMT_عبارت_IDENT) {
                                    parser_err(
                                        param, XKB_ERROR_INVALID_MODIFIER_MAP_MASK,
                                        "Invalid real modifier mask in modifier "
                                        "map definition: expected identifier"
                                    );
                                    FreeStmt((ParseCommon *) $2);
                                    FreeStmt((ParseCommon *) $4.head);
                                    YYERROR;
                            }
                            $$ = ModMapCreate($2, $4.head);
                        }
                ;

کلید_Or_کلید_Sym_List :       کلید_Or_کلید_Sym_List COMMA کلید_Or_کلید_Sym
                        { $$.head = $1.head; $$.last->common.next = &$3->common; $$.last = $3; }
                |       کلید_Or_کلید_Sym
                        { $$.head = $$.last = $1; }
                ;

کلید_Or_کلید_Sym     :       KEYNAME
                        { $$ = عبارت_Create_کلید_Name($1); }
                |       کلید_Sym_2
                        { $$ = عبارت_Create_کلید_Sym($1); }
                ;

LedMapDecl:             INDICATOR String OBRACE VarDeclList CBRACE SEMI
                        { $$ = LedMapCreate($2, $4.head); }
                ;

LedNameDecl:            INDICATOR Integer EQUALS عبارت_2 SEMI
                        { $$ = LedNameCreate($2, $4, false); }
                |       VIRTUAL INDICATOR Integer EQUALS عبارت_2 SEMI
                        { $$ = LedNameCreate($3, $5, true); }
                ;

UnknownDecl     :       IDENT Terminal EQUALS عبارت_2 SEMI
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
                            FreeStmt((ParseCommon *) $4.head);
                            $$ = UnknownStatementCreate(STMT_UNKNOWN_COMPOUND, $1);
                        }
                ;

ShapeDecl       :       SHAPE String OBRACE OutlineList CBRACE SEMI
                        { $$ = پوچ; }
                |       SHAPE String OBRACE CoordList CBRACE SEMI
                        { (تهی) $4; $$ = پوچ; }
                ;

SectionDecl     :       SECTION String OBRACE SectionBody CBRACE SEMI
                        { $$ = پوچ; }
                ;

SectionBody     :       SectionBody SectionBodyItem     { $$ = پوچ;}
                |       SectionBodyItem                 { $$ = پوچ; }
                ;

SectionBodyItem :       ROW OBRACE RowBody CBRACE SEMI
                        { $$ = پوچ; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = پوچ; }
                |       DoodadDecl
                        { $$ = پوچ; }
                |       LedMapDecl
                        { FreeStmt((ParseCommon *) $1); $$ = پوچ; }
                |       OverlayDecl
                        { $$ = پوچ; }
                ;

RowBody         :       RowBody RowBodyItem     { $$ = پوچ;}
                |       RowBodyItem             { $$ = پوچ; }
                ;

RowBodyItem     :       کلیدها_2 OBRACE کلیدها_3 CBRACE SEMI { $$ = پوچ; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = پوچ; }
                ;

کلیدها_3            :       کلیدها_3 COMMA کلید_4          { $$ = پوچ; }
                |       کلید_4                     { $$ = پوچ; }
                ;

کلید_4             :       KEYNAME
                        { $$ = پوچ; }
                |       OBRACE عبارت_List_2 CBRACE
                        { FreeStmt((ParseCommon *) $2.head); $$ = پوچ; }
                ;

OverlayDecl     :       OVERLAY String OBRACE Overlay_کلید_List CBRACE SEMI
                        { $$ = پوچ; }
                ;

Overlay_کلید_List  :       Overlay_کلید_List COMMA Overlay_کلید { $$ = پوچ; }
                |       Overlay_کلید                      { $$ = پوچ; }
                ;

Overlay_کلید      :       KEYNAME EQUALS KEYNAME          { $$ = پوچ; }
                ;

OutlineList     :       OutlineList COMMA OutlineInList
                        { $$ = پوچ;}
                |       OutlineInList
                        { $$ = پوچ; }
                ;

OutlineInList   :       OBRACE CoordList CBRACE
                        { (تهی) $2; $$ = پوچ; }
                |       Ident EQUALS OBRACE CoordList CBRACE
                        { (تهی) $4; $$ = پوچ; }
                |       Ident EQUALS عبارت_2
                        { FreeStmt((ParseCommon *) $3); $$ = پوچ; }
                ;

CoordList       :       CoordList COMMA Coord
                        { (تهی) $1; (تهی) $3; $$ = پوچ; }
                |       Coord
                        { (تهی) $1; $$ = پوچ; }
                ;

Coord           :       OBRACKET Signed_عدد COMMA Signed_عدد CBRACKET
                        { $$ = پوچ; }
                ;

DoodadDecl      :       DoodadType String OBRACE VarDeclList CBRACE SEMI
                        { FreeStmt((ParseCommon *) $4.head); $$ = پوچ; }
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
                |       کلید_3
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

عبارت_List_2        :       عبارت_List_2 COMMA عبارت_2
                        {
                            اگر ($3) {
                                اگر ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$3->common;
                                    $$.last = $3;
                                } وگرنه {
                                    $$.head = $$.last = $3;
                                }
                            }
                        }
                |       عبارت_2
                        { $$.head = $$.last = $1; }
                |       { $$.head = $$.last = پوچ; }
                ;

عبارت_2            :       عبارت_2 DIVIDE عبارت_2
                        { $$ = عبارت_Create_دودویی(STMT_عبارت_DIVIDE, $1, $3); }
                |       عبارت_2 PLUS عبارت_2
                        { $$ = عبارت_Create_دودویی(STMT_عبارت_ADD, $1, $3); }
                |       عبارت_2 MINUS عبارت_2
                        { $$ = عبارت_Create_دودویی(STMT_عبارت_SUBTRACT, $1, $3); }
                |       عبارت_2 TIMES عبارت_2
                        { $$ = عبارت_Create_دودویی(STMT_عبارت_MULTIPLY, $1, $3); }
                |       Lhs EQUALS عبارت_2
                        { $$ = عبارت_Create_دودویی(STMT_عبارت_ASSIGN, $1, $3); }
                |       Term
                        { $$ = $1; }
                ;

Term            :       MINUS Term
                        { $$ = عبارت_Create_Unary(STMT_عبارت_NEGATE, $2); }
                |       PLUS Term
                        { $$ = عبارت_Create_Unary(STMT_عبارت_UNARY_PLUS, $2); }
                |       EXCLAM Term
                        { $$ = عبارت_Create_Unary(STMT_عبارت_NOT, $2); }
                |       INVERT Term
                        { $$ = عبارت_Create_Unary(STMT_عبارت_INVERT, $2); }
                |       Lhs
                        { $$ = $1; }
                |       FieldSpec OPAREN عبارت_List_2 CPAREN %prec OPAREN
                        { $$ = عبارت_Create_Action($1, $3.head); }
                |       Actions
                        { $$ = $1; }
                |       Terminal
                        { $$ = $1; }
                |       OPAREN عبارت_2 CPAREN
                        { $$ = $2; }
                ;

MultiActionList :       MultiActionList COMMA Action
                        {
                            عبارت_Def *عبارت = عبارت_Create_Action_List($3);
                            $$ = $1;
                            $$.last->common.next = &عبارت->common; $$.last = عبارت;
                        }
                |       MultiActionList COMMA Actions
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       Action
                        { $$.head = $$.last = عبارت_Create_Action_List($1); }
                |       NonEmptyActions
                        { $$.head = $$.last = $1; }
                ;

ActionList      :       ActionList COMMA Action
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       Action
                        { $$.head = $$.last = $1; }
                ;

NonEmptyActions :       OBRACE ActionList CBRACE
                        { $$ = عبارت_Create_Action_List($2.head); }
                ;

Actions         :       NonEmptyActions
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = عبارت_Create_Action_List(پوچ); }
                ;

Action          :       FieldSpec OPAREN عبارت_List_2 CPAREN
                        { $$ = عبارت_Create_Action($1, $3.head); }
                ;

Lhs             :       FieldSpec
                        { $$ = عبارت_Create_Ident($1); }
                |       FieldSpec DOT FieldSpec
                        { $$ = عبارت_Create_Field_Ref($1, $3); }
                |       FieldSpec OBRACKET عبارت_2 CBRACKET
                        { $$ = عبارت_Create_Array_Ref(XKB_ATOM_NONE, $1, $3); }
                |       FieldSpec DOT FieldSpec OBRACKET عبارت_2 CBRACKET
                        { $$ = عبارت_Create_Array_Ref($1, $3, $5); }
                ;

OptTerminal     :       Terminal
                        { $$ = $1; }
                |       { $$ = پوچ; }
                ;

Terminal        :       String
                        { $$ = عبارت_Create_String($1); }
                |       Integer
                        { $$ = عبارت_Create_Integer($1); }
                |       Float
                        { $$ = عبارت_Create_Float(/* Discard $1 */); }
                |       KEYNAME
                        { $$ = عبارت_Create_کلید_Name($1); }
                ;

Multi_کلید_Sym_List :       Multi_کلید_Sym_List COMMA کلید_Sym_Lit
                        {
                            عبارت_Def *عبارت = عبارت_Create_کلید_Sym_List($3);
                            $$ = $1;
                            $$.last->common.next = &عبارت->common; $$.last = عبارت;
                        }
                |       Multi_کلید_Sym_List COMMA کلید_Syms
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       کلید_Sym_Lit
                        { $$.head = $$.last = عبارت_Create_کلید_Sym_List($1); }
                |       Non_Empty_کلید_Syms
                        { $$.head = $$.last = $1; }
                ;

کلید_Sym_List      :       کلید_Sym_List COMMA کلید_Sym_Lit
                        { $$ = عبارت_Append_کلید_Sym_List($1, $3); }
                |       کلید_Sym_List COMMA STRING
                        {
                            $$ = عبارت_کلید_Sym_List_Append_String(param->scanner, $1, $3);
                            آزاد($3);
                            اگر (!$$)
                                YYERROR;
                        }
                |       کلید_Sym_Lit
                        {
                            $$ = عبارت_Create_کلید_Sym_List($1);
                            اگر (!$$)
                                YYERROR;
                        }
                |       STRING
                        {
                            $$ = عبارت_Create_کلید_Sym_List(XKB_کلید_No_نماد);
                            اگر (!$$)
                                YYERROR;
                            $$ = عبارت_کلید_Sym_List_Append_String(param->scanner, $$, $1);
                            آزاد($1);
                            اگر (!$$)
                                YYERROR;
                        }
                ;

Non_Empty_کلید_Syms :       OBRACE کلید_Sym_List CBRACE
                        { $$ = $2; }
                |       STRING
                        {
                            $$ = عبارت_Create_کلید_Sym_List(XKB_کلید_No_نماد);
                            اگر (!$$)
                                YYERROR;
                            $$ = عبارت_کلید_Sym_List_Append_String(param->scanner, $$, $1);
                            آزاد($1);
                            اگر (!$$)
                                YYERROR;
                        }
                ;

کلید_Syms         :       Non_Empty_کلید_Syms
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = عبارت_Create_کلید_Sym_List(XKB_کلید_No_نماد); }
                ;

کلید_Sym_2          :       کلید_Sym_Lit
                        { $$ = $1; }
                |       STRING
                        {
                            $$ = KeysymParseString(param->scanner, $1);
                            آزاد($1);
                            اگر ($$ == XKB_کلید_No_نماد)
                                YYERROR;
                        }
                ;

کلید_Sym_Lit       :       IDENT
                        {
                            اگر (!resolve_keysym(param, $1, &$$)) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (unsigned صحیح) $1.len, $1.start
                                );
                                $$ = XKB_کلید_No_نماد;
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
                            اگر ($1 < XKB_KEYSYM_MIN) {
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
                                $$ = XKB_کلید_No_نماد;
                            }
                            /*
                             * Integers 0..9 are handled with DECIMAL_DIGIT if
                             * they were formatted as single characters '0'..'9'.
                             * Otherwise they are handled here as raw keysyms
                             * values. E.g. `01` and `0x1` are interpreted as
                             * the keysym 0x0001, while `1` is interpreted as
                             * XKB_KEY_1.
                             */
                            وگرنه {
                                /* Any other numeric value */
                                اگر ($1 <= XKB_KEYSYM_MAX) {
                                    /*
                                     * Valid keysym
                                     * No normalization is performed and value
                                     * is used as is.
                                     */
                                    $$ = (xkb_keysym_t) $1;
                                    check_deprecated_keysyms(
                                        parser_warn, param, param->ctx,
                                        $$, پوچ, $$, "%#06"PRIx32, "");
                                } وگرنه {
                                    /* Invalid keysym */
                                    parser_warn(
                                        param, XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                        "unrecognized keysym \"%#06"PRIx64"\" "
                                        "(%"PRId64")", $1, $1
                                    );
                                    $$ = XKB_کلید_No_نماد;
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

کلید_Code_2         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

Ident           :       IDENT   { $$ = xkb_atom_intern(param->ctx, $1.start, $1.len); }
                |       DEFAULT { $$ = xkb_atom_intern_literal(param->ctx, "default"); }
                ;

String          :       STRING  { $$ = xkb_atom_intern(param->ctx, $1, strlen($1)); آزاد($1); }
                ;

OptMapName      :       MapName { $$ = $1; }
                |               { $$ = پوچ; }
                ;

MapName         :       STRING  { $$ = $1; }
                ;

%%

/* Parse a specific section */
XkbFile *
parse(ساختار xkb_context *ctx, ثابت ساختار parser_keymap_config *config,
      ساختار scanner *scanner, ثابت نویسه *map)
{
    صحیح ret;
    XkbFile *first = پوچ;
    ساختار parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = پوچ,
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

    وقتی ((ret = yyparse(&param)) == 0 && param.more_maps) {
        اگر (map) {
            اگر (streq_not_null(map, param.rtrn->name))
                بازگشت param.rtrn;
            وگرنه
                FreeXkbFile(param.rtrn);
        }
        وگرنه {
            اگر (param.rtrn->flags & MAP_IS_DEFAULT) {
                FreeXkbFile(first);
                بازگشت param.rtrn;
            }
            وگرنه اگر (!first) {
                first = param.rtrn;
            }
            وگرنه {
                FreeXkbFile(param.rtrn);
            }
        }
        param.rtrn = پوچ;
    }

    اگر (ret != 0) {
        /* Some error happend; clear the Xkbfiles parsed so far */
        FreeXkbFile(first);
        FreeXkbFile(param.rtrn);
        بازگشت پوچ;
    }

    /*
     * Warn about implicit default section,
     * but only if not a keymap: multiple keymaps per file not supported
     */
    اگر (first && first->file_type != FILE_TYPE_KEYMAP)
        log_vrb(ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_MISSING_DEFAULT_SECTION,
                "No section name in include statement, but \"%s\" contains several; "
                "Using first defined section, \"%s\"\n",
                scanner->file_name, safe_map_name(first->name));

    بازگشت first;
}

/* Parse the next section */
bool
parse_next(ساختار xkb_context *ctx,
           ثابت ساختار parser_keymap_config *config,
           ساختار scanner *scanner, XkbFile **xkb_file)
{
    صحیح ret;
    ساختار parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = پوچ,
        .config = *config,
        .more_maps = false,
    };

    اگر ((ret = yyparse(&param)) == 0 && param.more_maps) {
        *xkb_file = param.rtrn;
        بازگشت true;
    } وگرنه {
        FreeXkbFile(param.rtrn);
        *xkb_file = پوچ;
        بازگشت (ret == 0);
    }
}
