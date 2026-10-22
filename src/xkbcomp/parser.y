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
    bool more_maps;
};

#define parser_log_with_code(param, level, verbosity, log_ڤسنن_id, fmt, ...)   \
    scanner_log_with_code((param)->scanner, level, verbosity, log_ڤسنن_id, fmt,\
                          ##__VA_ARGS__)

#define parser_err(param, error_id, fmt, ...) \
    scanner_err((param)->scanner, error_id, fmt, ##__VA_ARGS__)

#define parser_warn(param, warning_id, fmt, ...) \
    scanner_warn((param)->scanner, warning_id, fmt, ##__VA_ARGS__)

#define parser_vrb(param, verbosity, warning_id, fmt, ...) \
    scanner_vrb((param)->scanner, verbosity, warning_id, fmt, ##__VA_ARGS__)

ساكن فراغ
_xkbcommon_error(بنية parser_param *param, ثابت حرف *ڤسنن)
{
    parser_err(param, XKB_ERROR_INVALID_XKB_SYNTAX, "%s", ڤسنن);
}

ساكن bool
resolve_keysym(بنية parser_param *param, بنية sval name, xkb_keysym_t *sym_rtrn)
{
    xkb_keysym_t sym;

    إذا (isvaleq(name, SVAL_LIT("any")) || isvaleq(name, SVAL_LIT("nosymbol"))) {
        *sym_rtrn = XKB_کونچي_No_سيمبول;
        أرجع true;
    }

    إذا (isvaleq(name, SVAL_LIT("none")) || isvaleq(name, SVAL_LIT("voidsymbol"))) {
        *sym_rtrn = XKB_کونچي_Void_سيمبول;
        أرجع true;
    }

    /* xkb_keysym_from_name needs a C string. */
    حرف buf[XKB_KEYSYM_NAME_MAX_SIZE];
    إذا (name.len >= حجم(buf)) {
        أرجع false;
    }
    memcpy(buf, name.start, name.len);
    buf[name.len] = '\0';

    sym = xkb_keysym_from_name(buf, XKB_KEYSYM_NO_FLAGS);
    إذا (sym != XKB_کونچي_No_سيمبول) {
        *sym_rtrn = sym;
        check_deprecated_keysyms(parser_warn, param, param->ctx,
                                 sym, buf, buf, "%s", "");
        أرجع true;
    }

    أرجع false;
}

#define param_scanner param->scanner
%}

%define api.pure
%lex-param      { بنية scanner *param_scanner }
%parse-param    { بنية parser_param *param }

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
        کونچي_3             24 "key"
        ALIAS           25 "alias"
        GROUP           26 "group"
        MODIFIER_MAP    27 "modifier_map"
        INDICATOR       28 "indicator"
        SHAPE           29 "shape"
        کونچي_سموا_2            30 "keys"
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
        ALPHANUMERIC_کونچي_سموا       73 "alphanumeric_keys"
        MODIFIER_کونچي_سموا           74 "modifier_keys"
        KEYPAD_کونچي_سموا             75 "keypad_keys"
        FUNCTION_کونچي_سموا           76 "function_keys"
        ALTERNATE_GROUP         77 "alternate_group"
        DEPRECATED              78 "deprecated"

%right  EQUALS
%left   PLUS MINUS
%left   TIMES DIVIDE
%left   EXCLAM INVERT
%left   OPAREN

%start  XkbFile

%اتحاد  {
        int64_t          اڠک;
        تعداد xkb_file_type file_type;
        حرف            *str;
        بنية sval     sval;
        xkb_atom_t      atom;
        تعداد merge_mode merge;
        تعداد xkb_map_flags mapFlags;
        xkb_keysym_t    keysym;
        ParseCommon     *any;
        بنية { ParseCommon *head; ParseCommon *last; } anyList;
        uint32_t        no_سيمبول_Or_Action_List;
        اوڠکاڤن_Def         *اوڠکاڤن;
        بنية { اوڠکاڤن_Def *head; اوڠکاڤن_Def *last; } اوڠکاڤن_List;
        VarDef          *var;
        بنية { VarDef *head; VarDef *last; } varList;
        VModDef         *vmod;
        بنية { VModDef *head; VModDef *last; } vmodList;
        InterpDef       *interp;
        کونچي_Type_Def      *کونچي_Type;
        SymbolsDef      *syms;
        ModMapDef       *modMask;
        GroupCompatDef  *groupCompat;
        LedMapDef       *ledMap;
        LedNameDef      *ledName;
        KeycodeDef      *کونچي_Code;
        کونچي_Alias_Def     *کونچي_Alias;
        UnknownStatement *unknown;
        فراغ            *geom;
        XkbFile         *file;
        بنية { XkbFile *head; XkbFile *last; } fileList;
}

%نوع <اڠک>     DECIMAL_DIGIT INTEGER FLOAT
%نوع <str>     STRING
%نوع <sval>    IDENT
%نوع <atom>    KEYNAME
%نوع <اڠک>     کونچي_Code_2 نومبور Integer Float Signed_نومبور DoodadType
%نوع <merge>   MergeMode OptMergeMode
%نوع <file_type> XkbCompositeType FileType
%نوع <mapFlags> Flag Flags OptFlags
%نوع <str>     MapName OptMapName
%نوع <atom>    FieldSpec Ident Element String
%نوع <keysym>  کونچي_Sym_2 کونچي_Sym_Lit
%نوع <no_سيمبول_Or_Action_List> No_سيمبول_Or_Action_List
%نوع <any>     Decl
%نوع <anyList> DeclList
%نوع <اوڠکاڤن>    اوڠکاڤن_2 Term Lhs Terminal OptTerminal Coord CoordList
%نوع <اوڠکاڤن>    Multi_کونچي_Sym_Or_Action_List NonEmptyActions Actions Action
%نوع <اوڠکاڤن>    کونچي_Syms Non_Empty_کونچي_Syms کونچي_Sym_List کونچي_Or_کونچي_Sym
%نوع <اوڠکاڤن_List> اوڠکاڤن_List_2 کونچي_Or_کونچي_Sym_List
%نوع <اوڠکاڤن_List> ActionList MultiActionList Multi_کونچي_Sym_List
%نوع <var>     VarDecl SymbolsVarDecl
%نوع <varList> VarDeclList SymbolsBody OptSymbolsBody
%نوع <vmod>    VModDef
%نوع <vmodList> VModDefList VModDecl
%نوع <interp>  InterpretDecl InterpretMatch
%نوع <کونچي_Type> کونچي_Type_Decl
%نوع <syms>    SymbolsDecl
%نوع <modMask> ModMapDecl
%نوع <groupCompat> GroupCompatDecl
%نوع <ledMap>  LedMapDecl
%نوع <ledName> LedNameDecl
%نوع <کونچي_Code> کونچي_Name_Decl
%نوع <کونچي_Alias> کونچي_Alias_Decl
%نوع <geom>    ShapeDecl SectionDecl SectionBody SectionBodyItem RowBody RowBodyItem
%نوع <geom>    کونچي_سموا_3 کونچي_4 OverlayDecl Overlay_کونچي_List Overlay_کونچي OutlineList OutlineInList
%نوع <geom>    DoodadDecl
%نوع <unknown> UnknownDecl UnknownCompoundStatementDecl
%نوع <file>    XkbFile XkbMapConfig
%نوع <fileList> XkbMapConfigList
%نوع <file>    XkbCompositeMap

%destructor { FreeStmt((ParseCommon *) $$); }
    <any> <اوڠکاڤن> <var> <vmod> <interp> <کونچي_Type> <syms> <modMask> <groupCompat>
    <ledMap> <ledName> <کونچي_Code> <کونچي_Alias>
%destructor { FreeStmt((ParseCommon *) $$.head); }
    <anyList> <اوڠکاڤن_List> <varList> <vmodList>
/* The destructor also runs on the start symbol when the parser *succeeds*.
 * The `if` here catches this case. */
%destructor { إذا (!param->rtrn) FreeXkbFile($$); } <file>
%destructor { FreeXkbFile($$.head); } <fileList>
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

XkbFile         :       XkbCompositeMap
                        { $$ = param->rtrn = $1; param->more_maps = !!param->rtrn; (فراغ) yynerrs; }
                |       XkbMapConfig
                        { $$ = param->rtrn = $1; param->more_maps = !!param->rtrn; YYACCEPT; }
                |       END_OF_FILE
                        { $$ = param->rtrn = عدم; param->more_maps = false; }
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
                            إذا ($2) {
                                إذا ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$2->common;
                                    $$.last = $2;
                                } وإلا {
                                    $$.head = $$.last = $2;
                                }
                            }
                        }
                |       { $$.head = $$.last = عدم; }
                ;

XkbMapConfig    :       OptFlags FileType OptMapName OBRACE
                            DeclList
                        CBRACE SEMI
                        {
                            إذا ($1 & MAP_IS_DEPRECATED) {
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
                |       ALPHANUMERIC_کونچي_سموا       { $$ = MAP_HAS_ALPHANUMERIC; }
                |       MODIFIER_کونچي_سموا           { $$ = MAP_HAS_MODIFIER; }
                |       KEYPAD_کونچي_سموا             { $$ = MAP_HAS_KEYPAD; }
                |       FUNCTION_کونچي_سموا           { $$ = MAP_HAS_FN; }
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
                            إذا (error)
                                YYABORT;
                            $$ = 0;
                        }
                ;

DeclList        :       DeclList Decl
                        {
                            إذا ($2) {
                                إذا ($1.head) {
                                    $$.head = $1.head; $1.last->next = $2; $$.last = $2;
                                } وإلا {
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
                            لكل (VModDef *vmod = $3.head; vmod; vmod = (VModDef *) vmod->common.next)
                                vmod->merge = $2;
                            إذا ($1.head) {
                                $$.head = $1.head; $1.last->next = &$3.head->common; $$.last = &$3.last->common;
                            } وإلا {
                                $$.head = &$3.head->common; $$.last = &$3.last->common;
                            }
                        }
                |       { $$.head = $$.last = عدم; }
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
                |       OptMergeMode کونچي_Name_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode کونچي_Alias_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode کونچي_Type_Decl
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
                |       OptMergeMode ShapeDecl          { $$ = عدم; }
                |       OptMergeMode SectionDecl        { $$ = عدم; }
                |       OptMergeMode DoodadDecl         { $$ = عدم; }
                |       OptMergeMode UnknownDecl
                            { $$ = (ParseCommon *) $2; }
                |       OptMergeMode UnknownCompoundStatementDecl
                            { $$ = (ParseCommon *) $2; }
                |       MergeMode STRING
                        {
                            $$ = (ParseCommon *) IncludeCreate(param->ctx, $2, $1);
                            حرر($2);
                        }
                ;

VarDecl         :       Lhs EQUALS اوڠکاڤن_2 SEMI
                        { $$ = VarCreate($1, $3); }
                |       Ident SEMI
                        { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident SEMI
                        { $$ = BoolVarCreate($2, false); }
                ;

کونچي_Name_Decl     :       KEYNAME EQUALS کونچي_Code_2 SEMI
                        { $$ = KeycodeCreate($1, $3); }
                ;

کونچي_Alias_Decl    :       ALIAS KEYNAME EQUALS KEYNAME SEMI
                        { $$ = کونچي_Alias_Create($2, $4); }
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
                        { $$ = VModCreate($1, عدم); }
                |       Ident EQUALS اوڠکاڤن_2
                        { $$ = VModCreate($1, $3); }
                ;

InterpretDecl   :       INTERPRET InterpretMatch OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $2->def = $4.head; $$ = $2; }
                ;

InterpretMatch  :       کونچي_Sym_2 PLUS اوڠکاڤن_2
                        { $$ = InterpCreate($1, $3); }
                |       کونچي_Sym_2
                        { $$ = InterpCreate($1, عدم); }
                ;

VarDeclList     :       VarDeclList VarDecl
                        {
                            إذا ($2) {
                                إذا ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$2->common;
                                    $$.last = $2;
                                } وإلا {
                                    $$.head = $$.last = $2;
                                }
                            }
                        }
                |       { $$.head = $$.last = عدم; }
                ;

کونچي_Type_Decl     :       TYPE String OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $$ = کونچي_Type_Create($2, $4.head); }
                ;

SymbolsDecl     :       کونچي_3 KEYNAME OBRACE
                            OptSymbolsBody
                        CBRACE SEMI
                        { $$ = SymbolsCreate($2, $4.head); }
                ;

OptSymbolsBody  :       SymbolsBody { $$ = $1; }
                |                   { $$.head = $$.last = عدم; }
                ;

SymbolsBody     :       SymbolsBody COMMA SymbolsVarDecl
                        { $$.head = $1.head; $$.last->common.next = &$3->common; $$.last = $3; }
                |       SymbolsVarDecl
                        { $$.head = $$.last = $1; }
                ;

SymbolsVarDecl  :       Lhs EQUALS اوڠکاڤن_2         { $$ = VarCreate($1, $3); }
                |       Lhs EQUALS Multi_کونچي_Sym_Or_Action_List { $$ = VarCreate($1, $3); }
                |       Ident                   { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident            { $$ = BoolVarCreate($2, false); }
                |       Multi_کونچي_Sym_Or_Action_List { $$ = VarCreate(عدم, $1); }
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
Multi_کونچي_Sym_Or_Action_List
                :       OBRACKET Multi_کونچي_Sym_List CBRACKET
                        { $$ = $2.head; }
                |       OBRACKET No_سيمبول_Or_Action_List COMMA Multi_کونچي_Sym_List CBRACKET
                        {
                            /* Prepend n times NoSymbol */
                            بنية {اوڠکاڤن_Def *head; اوڠکاڤن_Def *last;} list = {
                                .head = $4.head, .last = $4.last
                            };
                            لكل (uint32_t k = 0; k < $2; k++) {
                                اوڠکاڤن_Def* ثابت syms =
                                    اوڠکاڤن_Create_کونچي_Sym_List(XKB_کونچي_No_سيمبول);
                                إذا (!syms) {
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
                |       OBRACKET No_سيمبول_Or_Action_List COMMA MultiActionList CBRACKET
                        {
                            /* Prepend n times NoAction() */
                            بنية {اوڠکاڤن_Def *head; اوڠکاڤن_Def *last;} list = {
                                .head = $4.head, .last = $4.last
                            };
                            لكل (uint32_t k = 0; k < $2; k++) {
                                اوڠکاڤن_Def* ثابت acts = اوڠکاڤن_Create_Action_List(عدم);
                                إذا (!acts) {
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
                |       OBRACKET No_سيمبول_Or_Action_List CBRACKET
                        { $$ = اوڠکاڤن_Empty_List(); }
                ;

/* A list of `{}`, which remains ambiguous until reaching a keysym or action list */
No_سيمبول_Or_Action_List
                :       No_سيمبول_Or_Action_List COMMA OBRACE CBRACE
                        { $$ = $1 + 1; }
                |       OBRACE CBRACE
                        { $$ = 1; }
                |       { $$ = 0; }
                ;

GroupCompatDecl :       GROUP Integer EQUALS اوڠکاڤن_2 SEMI
                        { $$ = GroupCompatCreate($2, $4); }
                ;

ModMapDecl      :       MODIFIER_MAP اوڠکاڤن_2 OBRACE کونچي_Or_کونچي_Sym_List CBRACE SEMI
                        {
                            إذا (param->config.format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
                                $2->common.نوع != STMT_اوڠکاڤن_IDENT) {
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

کونچي_Or_کونچي_Sym_List :       کونچي_Or_کونچي_Sym_List COMMA کونچي_Or_کونچي_Sym
                        { $$.head = $1.head; $$.last->common.next = &$3->common; $$.last = $3; }
                |       کونچي_Or_کونچي_Sym
                        { $$.head = $$.last = $1; }
                ;

کونچي_Or_کونچي_Sym     :       KEYNAME
                        { $$ = اوڠکاڤن_Create_کونچي_Name($1); }
                |       کونچي_Sym_2
                        { $$ = اوڠکاڤن_Create_کونچي_Sym($1); }
                ;

LedMapDecl:             INDICATOR String OBRACE VarDeclList CBRACE SEMI
                        { $$ = LedMapCreate($2, $4.head); }
                ;

LedNameDecl:            INDICATOR Integer EQUALS اوڠکاڤن_2 SEMI
                        { $$ = LedNameCreate($2, $4, false); }
                |       VIRTUAL INDICATOR Integer EQUALS اوڠکاڤن_2 SEMI
                        { $$ = LedNameCreate($3, $5, true); }
                ;

UnknownDecl     :       IDENT Terminal EQUALS اوڠکاڤن_2 SEMI
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
                        { $$ = عدم; }
                |       SHAPE String OBRACE CoordList CBRACE SEMI
                        { (فراغ) $4; $$ = عدم; }
                ;

SectionDecl     :       SECTION String OBRACE SectionBody CBRACE SEMI
                        { $$ = عدم; }
                ;

SectionBody     :       SectionBody SectionBodyItem     { $$ = عدم;}
                |       SectionBodyItem                 { $$ = عدم; }
                ;

SectionBodyItem :       ROW OBRACE RowBody CBRACE SEMI
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

RowBody         :       RowBody RowBodyItem     { $$ = عدم;}
                |       RowBodyItem             { $$ = عدم; }
                ;

RowBodyItem     :       کونچي_سموا_2 OBRACE کونچي_سموا_3 CBRACE SEMI { $$ = عدم; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = عدم; }
                ;

کونچي_سموا_3            :       کونچي_سموا_3 COMMA کونچي_4          { $$ = عدم; }
                |       کونچي_4                     { $$ = عدم; }
                ;

کونچي_4             :       KEYNAME
                        { $$ = عدم; }
                |       OBRACE اوڠکاڤن_List_2 CBRACE
                        { FreeStmt((ParseCommon *) $2.head); $$ = عدم; }
                ;

OverlayDecl     :       OVERLAY String OBRACE Overlay_کونچي_List CBRACE SEMI
                        { $$ = عدم; }
                ;

Overlay_کونچي_List  :       Overlay_کونچي_List COMMA Overlay_کونچي { $$ = عدم; }
                |       Overlay_کونچي                      { $$ = عدم; }
                ;

Overlay_کونچي      :       KEYNAME EQUALS KEYNAME          { $$ = عدم; }
                ;

OutlineList     :       OutlineList COMMA OutlineInList
                        { $$ = عدم;}
                |       OutlineInList
                        { $$ = عدم; }
                ;

OutlineInList   :       OBRACE CoordList CBRACE
                        { (فراغ) $2; $$ = عدم; }
                |       Ident EQUALS OBRACE CoordList CBRACE
                        { (فراغ) $4; $$ = عدم; }
                |       Ident EQUALS اوڠکاڤن_2
                        { FreeStmt((ParseCommon *) $3); $$ = عدم; }
                ;

CoordList       :       CoordList COMMA Coord
                        { (فراغ) $1; (فراغ) $3; $$ = عدم; }
                |       Coord
                        { (فراغ) $1; $$ = عدم; }
                ;

Coord           :       OBRACKET Signed_نومبور COMMA Signed_نومبور CBRACKET
                        { $$ = عدم; }
                ;

DoodadDecl      :       DoodadType String OBRACE VarDeclList CBRACE SEMI
                        { FreeStmt((ParseCommon *) $4.head); $$ = عدم; }
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
                |       کونچي_3
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

اوڠکاڤن_List_2        :       اوڠکاڤن_List_2 COMMA اوڠکاڤن_2
                        {
                            إذا ($3) {
                                إذا ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$3->common;
                                    $$.last = $3;
                                } وإلا {
                                    $$.head = $$.last = $3;
                                }
                            }
                        }
                |       اوڠکاڤن_2
                        { $$.head = $$.last = $1; }
                |       { $$.head = $$.last = عدم; }
                ;

اوڠکاڤن_2            :       اوڠکاڤن_2 DIVIDE اوڠکاڤن_2
                        { $$ = اوڠکاڤن_Create_بيناري(STMT_اوڠکاڤن_DIVIDE, $1, $3); }
                |       اوڠکاڤن_2 PLUS اوڠکاڤن_2
                        { $$ = اوڠکاڤن_Create_بيناري(STMT_اوڠکاڤن_ADD, $1, $3); }
                |       اوڠکاڤن_2 MINUS اوڠکاڤن_2
                        { $$ = اوڠکاڤن_Create_بيناري(STMT_اوڠکاڤن_SUBTRACT, $1, $3); }
                |       اوڠکاڤن_2 TIMES اوڠکاڤن_2
                        { $$ = اوڠکاڤن_Create_بيناري(STMT_اوڠکاڤن_MULTIPLY, $1, $3); }
                |       Lhs EQUALS اوڠکاڤن_2
                        { $$ = اوڠکاڤن_Create_بيناري(STMT_اوڠکاڤن_ASSIGN, $1, $3); }
                |       Term
                        { $$ = $1; }
                ;

Term            :       MINUS Term
                        { $$ = اوڠکاڤن_Create_Unary(STMT_اوڠکاڤن_NEGATE, $2); }
                |       PLUS Term
                        { $$ = اوڠکاڤن_Create_Unary(STMT_اوڠکاڤن_UNARY_PLUS, $2); }
                |       EXCLAM Term
                        { $$ = اوڠکاڤن_Create_Unary(STMT_اوڠکاڤن_NOT, $2); }
                |       INVERT Term
                        { $$ = اوڠکاڤن_Create_Unary(STMT_اوڠکاڤن_INVERT, $2); }
                |       Lhs
                        { $$ = $1; }
                |       FieldSpec OPAREN اوڠکاڤن_List_2 CPAREN %prec OPAREN
                        { $$ = اوڠکاڤن_Create_Action($1, $3.head); }
                |       Actions
                        { $$ = $1; }
                |       Terminal
                        { $$ = $1; }
                |       OPAREN اوڠکاڤن_2 CPAREN
                        { $$ = $2; }
                ;

MultiActionList :       MultiActionList COMMA Action
                        {
                            اوڠکاڤن_Def *اوڠکاڤن = اوڠکاڤن_Create_Action_List($3);
                            $$ = $1;
                            $$.last->common.next = &اوڠکاڤن->common; $$.last = اوڠکاڤن;
                        }
                |       MultiActionList COMMA Actions
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       Action
                        { $$.head = $$.last = اوڠکاڤن_Create_Action_List($1); }
                |       NonEmptyActions
                        { $$.head = $$.last = $1; }
                ;

ActionList      :       ActionList COMMA Action
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       Action
                        { $$.head = $$.last = $1; }
                ;

NonEmptyActions :       OBRACE ActionList CBRACE
                        { $$ = اوڠکاڤن_Create_Action_List($2.head); }
                ;

Actions         :       NonEmptyActions
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = اوڠکاڤن_Create_Action_List(عدم); }
                ;

Action          :       FieldSpec OPAREN اوڠکاڤن_List_2 CPAREN
                        { $$ = اوڠکاڤن_Create_Action($1, $3.head); }
                ;

Lhs             :       FieldSpec
                        { $$ = اوڠکاڤن_Create_Ident($1); }
                |       FieldSpec DOT FieldSpec
                        { $$ = اوڠکاڤن_Create_Field_Ref($1, $3); }
                |       FieldSpec OBRACKET اوڠکاڤن_2 CBRACKET
                        { $$ = اوڠکاڤن_Create_Array_Ref(XKB_ATOM_NONE, $1, $3); }
                |       FieldSpec DOT FieldSpec OBRACKET اوڠکاڤن_2 CBRACKET
                        { $$ = اوڠکاڤن_Create_Array_Ref($1, $3, $5); }
                ;

OptTerminal     :       Terminal
                        { $$ = $1; }
                |       { $$ = عدم; }
                ;

Terminal        :       String
                        { $$ = اوڠکاڤن_Create_String($1); }
                |       Integer
                        { $$ = اوڠکاڤن_Create_Integer($1); }
                |       Float
                        { $$ = اوڠکاڤن_Create_Float(/* Discard $1 */); }
                |       KEYNAME
                        { $$ = اوڠکاڤن_Create_کونچي_Name($1); }
                ;

Multi_کونچي_Sym_List :       Multi_کونچي_Sym_List COMMA کونچي_Sym_Lit
                        {
                            اوڠکاڤن_Def *اوڠکاڤن = اوڠکاڤن_Create_کونچي_Sym_List($3);
                            $$ = $1;
                            $$.last->common.next = &اوڠکاڤن->common; $$.last = اوڠکاڤن;
                        }
                |       Multi_کونچي_Sym_List COMMA کونچي_Syms
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       کونچي_Sym_Lit
                        { $$.head = $$.last = اوڠکاڤن_Create_کونچي_Sym_List($1); }
                |       Non_Empty_کونچي_Syms
                        { $$.head = $$.last = $1; }
                ;

کونچي_Sym_List      :       کونچي_Sym_List COMMA کونچي_Sym_Lit
                        { $$ = اوڠکاڤن_Append_کونچي_Sym_List($1, $3); }
                |       کونچي_Sym_List COMMA STRING
                        {
                            $$ = اوڠکاڤن_کونچي_Sym_List_Append_String(param->scanner, $1, $3);
                            حرر($3);
                            إذا (!$$)
                                YYERROR;
                        }
                |       کونچي_Sym_Lit
                        {
                            $$ = اوڠکاڤن_Create_کونچي_Sym_List($1);
                            إذا (!$$)
                                YYERROR;
                        }
                |       STRING
                        {
                            $$ = اوڠکاڤن_Create_کونچي_Sym_List(XKB_کونچي_No_سيمبول);
                            إذا (!$$)
                                YYERROR;
                            $$ = اوڠکاڤن_کونچي_Sym_List_Append_String(param->scanner, $$, $1);
                            حرر($1);
                            إذا (!$$)
                                YYERROR;
                        }
                ;

Non_Empty_کونچي_Syms :       OBRACE کونچي_Sym_List CBRACE
                        { $$ = $2; }
                |       STRING
                        {
                            $$ = اوڠکاڤن_Create_کونچي_Sym_List(XKB_کونچي_No_سيمبول);
                            إذا (!$$)
                                YYERROR;
                            $$ = اوڠکاڤن_کونچي_Sym_List_Append_String(param->scanner, $$, $1);
                            حرر($1);
                            إذا (!$$)
                                YYERROR;
                        }
                ;

کونچي_Syms         :       Non_Empty_کونچي_Syms
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = اوڠکاڤن_Create_کونچي_Sym_List(XKB_کونچي_No_سيمبول); }
                ;

کونچي_Sym_2          :       کونچي_Sym_Lit
                        { $$ = $1; }
                |       STRING
                        {
                            $$ = KeysymParseString(param->scanner, $1);
                            حرر($1);
                            إذا ($$ == XKB_کونچي_No_سيمبول)
                                YYERROR;
                        }
                ;

کونچي_Sym_Lit       :       IDENT
                        {
                            إذا (!resolve_keysym(param, $1, &$$)) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (unsigned صحيح) $1.len, $1.start
                                );
                                $$ = XKB_کونچي_No_سيمبول;
                            }
                        }
                        /* Handle keysym that is also a keyword  */
                |       SECTION { $$ = XKB_کونچي_section; }
                |       DECIMAL_DIGIT
                        {
                            /*
                             * Special case for digits 0..9:
                             * map to XKB_KEY_0 .. XKB_KEY_9, consistent with
                             * other keysym names: <name> → XKB_KEY_<name>.
                             */
                            $$ = XKB_کونچي_0 + (xkb_keysym_t) $1;
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
                                $$ = XKB_کونچي_No_سيمبول;
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
                                    $$ = XKB_کونچي_No_سيمبول;
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

Signed_نومبور    :       MINUS نومبور    { $$ = -$2; }
                |       نومبور          { $$ = $1; }
                ;

نومبور          :       FLOAT         { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                |       INTEGER       { $$ = $1; }
                ;

Float           :       FLOAT   { $$ = 0; }
                ;

Integer         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

کونچي_Code_2         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

Ident           :       IDENT   { $$ = xkb_atom_intern(param->ctx, $1.start, $1.len); }
                |       DEFAULT { $$ = xkb_atom_intern_literal(param->ctx, "default"); }
                ;

String          :       STRING  { $$ = xkb_atom_intern(param->ctx, $1, strlen($1)); حرر($1); }
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
    XkbFile *first = عدم;
    بنية parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = عدم,
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

    طالما ((ret = yyparse(&param)) == 0 && param.more_maps) {
        إذا (map) {
            إذا (streq_not_null(map, param.rtrn->name))
                أرجع param.rtrn;
            وإلا
                FreeXkbFile(param.rtrn);
        }
        وإلا {
            إذا (param.rtrn->flags & MAP_IS_DEFAULT) {
                FreeXkbFile(first);
                أرجع param.rtrn;
            }
            وإلا إذا (!first) {
                first = param.rtrn;
            }
            وإلا {
                FreeXkbFile(param.rtrn);
            }
        }
        param.rtrn = عدم;
    }

    إذا (ret != 0) {
        /* Some error happend; clear the Xkbfiles parsed so far */
        FreeXkbFile(first);
        FreeXkbFile(param.rtrn);
        أرجع عدم;
    }

    /*
     * Warn about implicit default section,
     * but only if not a keymap: multiple keymaps per file not supported
     */
    إذا (first && first->file_type != FILE_TYPE_KEYMAP)
        log_vrb(ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_MISSING_DEFAULT_SECTION,
                "No section name in include statement, but \"%s\" contains several; "
                "Using first defined section, \"%s\"\n",
                scanner->file_name, safe_map_name(first->name));

    أرجع first;
}

/* Parse the next section */
bool
parse_next(بنية xkb_context *ctx,
           ثابت بنية parser_keymap_config *config,
           بنية scanner *scanner, XkbFile **xkb_file)
{
    صحيح ret;
    بنية parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = عدم,
        .config = *config,
        .more_maps = false,
    };

    إذا ((ret = yyparse(&param)) == 0 && param.more_maps) {
        *xkb_file = param.rtrn;
        أرجع true;
    } وإلا {
        FreeXkbFile(param.rtrn);
        *xkb_file = عدم;
        أرجع (ret == 0);
    }
}
