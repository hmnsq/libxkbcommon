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
#include "config.h"

#include "scanner-utils.h"
#include "xkbcomp/ast.h"
#include "xkbcomp/xkbcomp-priv.h"
}

%{
#include "config.h"

#include <assert.h>

#include "keysym.h"
#include "xkbcomp/xkbcomp-priv.h"
#include "xkbcomp/parser-priv.h"
#include "xkbcomp/ast-build.h"

struct parser_param {
    struct xkb_context *ctx;
    struct scanner *scanner;
    XkbFile *rtrn;
    struct parser_keymap_config config;
    bool more_maps;
};

#define parser_log_with_code(param, level, verbosity, log_मेनसाहे_id, fmt, ...)   \
    scanner_log_with_code((param)->scanner, level, verbosity, log_मेनसाहे_id, fmt,\
                          ##__VA_ARGS__)

#define parser_err(param, error_id, fmt, ...) \
    scanner_err((param)->scanner, error_id, fmt, ##__VA_ARGS__)

#define parser_warn(param, warning_id, fmt, ...) \
    scanner_warn((param)->scanner, warning_id, fmt, ##__VA_ARGS__)

#define parser_vrb(param, verbosity, warning_id, fmt, ...) \
    scanner_vrb((param)->scanner, verbosity, warning_id, fmt, ##__VA_ARGS__)

static void
_xkbcommon_error(struct parser_param *param, const char *मेनसाहे)
{
    parser_err(param, XKB_ERROR_INVALID_XKB_SYNTAX, "%s", मेनसाहे);
}

static bool
resolve_keysym(struct parser_param *param, struct sval name, xkb_keysym_t *sym_rtrn)
{
    xkb_keysym_t sym;

    if (isvaleq(name, SVAL_LIT("any")) || isvaleq(name, SVAL_LIT("nosymbol"))) {
        *sym_rtrn = XKB_सुसि_No_सिम्बोलो;
        return true;
    }

    if (isvaleq(name, SVAL_LIT("none")) || isvaleq(name, SVAL_LIT("voidsymbol"))) {
        *sym_rtrn = XKB_सुसि_Void_सिम्बोलो;
        return true;
    }

    /* xkb_keysym_from_name needs a C string. */
    char buf[XKB_KEYSYM_NAME_MAX_SIZE];
    if (name.len >= sizeof(buf)) {
        return false;
    }
    memcpy(buf, name.start, name.len);
    buf[name.len] = '\0';

    sym = xkb_keysym_from_name(buf, XKB_KEYSYM_NO_FLAGS);
    if (sym != XKB_सुसि_No_सिम्बोलो) {
        *sym_rtrn = sym;
        check_deprecated_keysyms(parser_warn, param, param->ctx,
                                 sym, buf, buf, "%s", "");
        return true;
    }

    return false;
}

#define param_scanner param->scanner
%}

%define api.pure
%lex-param      { struct scanner *param_scanner }
%parse-param    { struct parser_param *param }

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
        सुसि_3             24 "key"
        ALIAS           25 "alias"
        GROUP           26 "group"
        MODIFIER_MAP    27 "modifier_map"
        INDICATOR       28 "indicator"
        SHAPE           29 "shape"
        मगा_सुसि_2            30 "keys"
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
        ALPHANUMERIC_मगा_सुसि       73 "alphanumeric_keys"
        MODIFIER_मगा_सुसि           74 "modifier_keys"
        KEYPAD_मगा_सुसि             75 "keypad_keys"
        FUNCTION_मगा_सुसि           76 "function_keys"
        ALTERNATE_GROUP         77 "alternate_group"
        DEPRECATED              78 "deprecated"

%right  EQUALS
%left   PLUS MINUS
%left   TIMES DIVIDE
%left   EXCLAM INVERT
%left   OPAREN

%start  XkbFile

%union  {
        int64_t          नुमेरो;
        enum xkb_file_type file_type;
        char            *str;
        struct sval     sval;
        xkb_atom_t      atom;
        enum merge_mode merge;
        enum xkb_map_flags mapFlags;
        xkb_keysym_t    keysym;
        ParseCommon     *any;
        struct { ParseCommon *head; ParseCommon *last; } anyList;
        uint32_t        no_सिम्बोलो_Or_Action_List;
        पाहायाग_Def         *पाहायाग;
        struct { पाहायाग_Def *head; पाहायाग_Def *last; } पाहायाग_List;
        VarDef          *var;
        struct { VarDef *head; VarDef *last; } varList;
        VModDef         *vmod;
        struct { VModDef *head; VModDef *last; } vmodList;
        InterpDef       *interp;
        सुसि_Type_Def      *सुसि_Type;
        SymbolsDef      *syms;
        ModMapDef       *modMask;
        GroupCompatDef  *groupCompat;
        LedMapDef       *ledMap;
        LedNameDef      *ledName;
        KeycodeDef      *सुसि_Code;
        सुसि_Alias_Def     *सुसि_Alias;
        UnknownStatement *unknown;
        void            *geom;
        XkbFile         *file;
        struct { XkbFile *head; XkbFile *last; } fileList;
}

%type <नुमेरो>     DECIMAL_DIGIT INTEGER FLOAT
%type <str>     STRING
%type <sval>    IDENT
%type <atom>    KEYNAME
%type <नुमेरो>     सुसि_Code_2 नुमेरो_2 Integer Float Signed_नुमेरो DoodadType
%type <merge>   MergeMode OptMergeMode
%type <file_type> XkbCompositeType FileType
%type <mapFlags> Flag Flags OptFlags
%type <str>     MapName OptMapName
%type <atom>    FieldSpec Ident Element String
%type <keysym>  सुसि_Sym_2 सुसि_Sym_Lit
%type <no_सिम्बोलो_Or_Action_List> No_सिम्बोलो_Or_Action_List
%type <any>     Decl
%type <anyList> DeclList
%type <पाहायाग>    पाहायाग_2 Term Lhs Terminal OptTerminal Coord CoordList
%type <पाहायाग>    Multi_सुसि_Sym_Or_Action_List NonEmptyActions Actions Action
%type <पाहायाग>    सुसि_Syms Non_Empty_सुसि_Syms सुसि_Sym_List सुसि_Or_सुसि_Sym
%type <पाहायाग_List> पाहायाग_List_2 सुसि_Or_सुसि_Sym_List
%type <पाहायाग_List> ActionList MultiActionList Multi_सुसि_Sym_List
%type <var>     VarDecl SymbolsVarDecl
%type <varList> VarDeclList SymbolsBody OptSymbolsBody
%type <vmod>    VModDef
%type <vmodList> VModDefList VModDecl
%type <interp>  InterpretDecl InterpretMatch
%type <सुसि_Type> सुसि_Type_Decl
%type <syms>    SymbolsDecl
%type <modMask> ModMapDecl
%type <groupCompat> GroupCompatDecl
%type <ledMap>  LedMapDecl
%type <ledName> LedNameDecl
%type <सुसि_Code> सुसि_Name_Decl
%type <सुसि_Alias> सुसि_Alias_Decl
%type <geom>    ShapeDecl SectionDecl SectionBody SectionBodyItem RowBody RowBodyItem
%type <geom>    मगा_सुसि_3 सुसि_4 OverlayDecl Overlay_सुसि_List Overlay_सुसि OutlineList OutlineInList
%type <geom>    DoodadDecl
%type <unknown> UnknownDecl UnknownCompoundStatementDecl
%type <file>    XkbFile XkbMapConfig
%type <fileList> XkbMapConfigList
%type <file>    XkbCompositeMap

%destructor { FreeStmt((ParseCommon *) $$); }
    <any> <पाहायाग> <var> <vmod> <interp> <सुसि_Type> <syms> <modMask> <groupCompat>
    <ledMap> <ledName> <सुसि_Code> <सुसि_Alias>
%destructor { FreeStmt((ParseCommon *) $$.head); }
    <anyList> <पाहायाग_List> <varList> <vmodList>
/* The destructor also runs on the start symbol when the parser *succeeds*.
 * The `if` here catches this case. */
%destructor { if (!param->rtrn) FreeXkbFile($$); } <file>
%destructor { FreeXkbFile($$.head); } <fileList>
%destructor { free($$); } <str>

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
                        { $$ = param->rtrn = $1; param->more_maps = !!param->rtrn; (void) yynerrs; }
                |       XkbMapConfig
                        { $$ = param->rtrn = $1; param->more_maps = !!param->rtrn; YYACCEPT; }
                |       END_OF_FILE
                        { $$ = param->rtrn = NULL; param->more_maps = false; }
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
                            if ($2) {
                                if ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$2->common;
                                    $$.last = $2;
                                } else {
                                    $$.head = $$.last = $2;
                                }
                            }
                        }
                |       { $$.head = $$.last = NULL; }
                ;

XkbMapConfig    :       OptFlags FileType OptMapName OBRACE
                            DeclList
                        CBRACE SEMI
                        {
                            if ($1 & MAP_IS_DEPRECATED) {
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
                |       ALPHANUMERIC_मगा_सुसि       { $$ = MAP_HAS_ALPHANUMERIC; }
                |       MODIFIER_मगा_सुसि           { $$ = MAP_HAS_MODIFIER; }
                |       KEYPAD_मगा_सुसि             { $$ = MAP_HAS_KEYPAD; }
                |       FUNCTION_मगा_सुसि           { $$ = MAP_HAS_FN; }
                |       ALTERNATE_GROUP         { $$ = MAP_IS_ALTGR; }
                |       DEPRECATED              { $$ = MAP_IS_DEPRECATED; }
                |       IDENT
                        {
                            const bool error = (param->config.strict & PARSER_NO_UNKNOWN_SECTION_FLAGS);
                            parser_log_with_code(
                                param, (error ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
                                XKB_LOG_VERBOSITY_MINIMAL,
                                XKB_ERROR_UNKNOWN_SECTION_FLAG,
                                "Unknown section flag \"%.*s\"%s",
                                (unsigned)$1.len, $1.start,
                                (error ? "" : "; ignored")
                            );
                            if (error)
                                YYABORT;
                            $$ = 0;
                        }
                ;

DeclList        :       DeclList Decl
                        {
                            if ($2) {
                                if ($1.head) {
                                    $$.head = $1.head; $1.last->next = $2; $$.last = $2;
                                } else {
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
                            for (VModDef *vmod = $3.head; vmod; vmod = (VModDef *) vmod->common.next)
                                vmod->merge = $2;
                            if ($1.head) {
                                $$.head = $1.head; $1.last->next = &$3.head->common; $$.last = &$3.last->common;
                            } else {
                                $$.head = &$3.head->common; $$.last = &$3.last->common;
                            }
                        }
                |       { $$.head = $$.last = NULL; }
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
                |       OptMergeMode सुसि_Name_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode सुसि_Alias_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode सुसि_Type_Decl
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
                |       OptMergeMode ShapeDecl          { $$ = NULL; }
                |       OptMergeMode SectionDecl        { $$ = NULL; }
                |       OptMergeMode DoodadDecl         { $$ = NULL; }
                |       OptMergeMode UnknownDecl
                            { $$ = (ParseCommon *) $2; }
                |       OptMergeMode UnknownCompoundStatementDecl
                            { $$ = (ParseCommon *) $2; }
                |       MergeMode STRING
                        {
                            $$ = (ParseCommon *) IncludeCreate(param->ctx, $2, $1);
                            free($2);
                        }
                ;

VarDecl         :       Lhs EQUALS पाहायाग_2 SEMI
                        { $$ = VarCreate($1, $3); }
                |       Ident SEMI
                        { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident SEMI
                        { $$ = BoolVarCreate($2, false); }
                ;

सुसि_Name_Decl     :       KEYNAME EQUALS सुसि_Code_2 SEMI
                        { $$ = KeycodeCreate($1, $3); }
                ;

सुसि_Alias_Decl    :       ALIAS KEYNAME EQUALS KEYNAME SEMI
                        { $$ = सुसि_Alias_Create($2, $4); }
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
                        { $$ = VModCreate($1, NULL); }
                |       Ident EQUALS पाहायाग_2
                        { $$ = VModCreate($1, $3); }
                ;

InterpretDecl   :       INTERPRET InterpretMatch OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $2->def = $4.head; $$ = $2; }
                ;

InterpretMatch  :       सुसि_Sym_2 PLUS पाहायाग_2
                        { $$ = InterpCreate($1, $3); }
                |       सुसि_Sym_2
                        { $$ = InterpCreate($1, NULL); }
                ;

VarDeclList     :       VarDeclList VarDecl
                        {
                            if ($2) {
                                if ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$2->common;
                                    $$.last = $2;
                                } else {
                                    $$.head = $$.last = $2;
                                }
                            }
                        }
                |       { $$.head = $$.last = NULL; }
                ;

सुसि_Type_Decl     :       TYPE String OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $$ = सुसि_Type_Create($2, $4.head); }
                ;

SymbolsDecl     :       सुसि_3 KEYNAME OBRACE
                            OptSymbolsBody
                        CBRACE SEMI
                        { $$ = SymbolsCreate($2, $4.head); }
                ;

OptSymbolsBody  :       SymbolsBody { $$ = $1; }
                |                   { $$.head = $$.last = NULL; }
                ;

SymbolsBody     :       SymbolsBody COMMA SymbolsVarDecl
                        { $$.head = $1.head; $$.last->common.next = &$3->common; $$.last = $3; }
                |       SymbolsVarDecl
                        { $$.head = $$.last = $1; }
                ;

SymbolsVarDecl  :       Lhs EQUALS पाहायाग_2         { $$ = VarCreate($1, $3); }
                |       Lhs EQUALS Multi_सुसि_Sym_Or_Action_List { $$ = VarCreate($1, $3); }
                |       Ident                   { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident            { $$ = BoolVarCreate($2, false); }
                |       Multi_सुसि_Sym_Or_Action_List { $$ = VarCreate(NULL, $1); }
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
Multi_सुसि_Sym_Or_Action_List
                :       OBRACKET Multi_सुसि_Sym_List CBRACKET
                        { $$ = $2.head; }
                |       OBRACKET No_सिम्बोलो_Or_Action_List COMMA Multi_सुसि_Sym_List CBRACKET
                        {
                            /* Prepend n times NoSymbol */
                            struct {पाहायाग_Def *head; पाहायाग_Def *last;} list = {
                                .head = $4.head, .last = $4.last
                            };
                            for (uint32_t k = 0; k < $2; k++) {
                                पाहायाग_Def* const syms =
                                    पाहायाग_Create_सुसि_Sym_List(XKB_सुसि_No_सिम्बोलो);
                                if (!syms) {
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
                |       OBRACKET No_सिम्बोलो_Or_Action_List COMMA MultiActionList CBRACKET
                        {
                            /* Prepend n times NoAction() */
                            struct {पाहायाग_Def *head; पाहायाग_Def *last;} list = {
                                .head = $4.head, .last = $4.last
                            };
                            for (uint32_t k = 0; k < $2; k++) {
                                पाहायाग_Def* const acts = पाहायाग_Create_Action_List(NULL);
                                if (!acts) {
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
                |       OBRACKET No_सिम्बोलो_Or_Action_List CBRACKET
                        { $$ = पाहायाग_Empty_List(); }
                ;

/* A list of `{}`, which remains ambiguous until reaching a keysym or action list */
No_सिम्बोलो_Or_Action_List
                :       No_सिम्बोलो_Or_Action_List COMMA OBRACE CBRACE
                        { $$ = $1 + 1; }
                |       OBRACE CBRACE
                        { $$ = 1; }
                |       { $$ = 0; }
                ;

GroupCompatDecl :       GROUP Integer EQUALS पाहायाग_2 SEMI
                        { $$ = GroupCompatCreate($2, $4); }
                ;

ModMapDecl      :       MODIFIER_MAP पाहायाग_2 OBRACE सुसि_Or_सुसि_Sym_List CBRACE SEMI
                        {
                            if (param->config.format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
                                $2->common.type != STMT_पाहायाग_IDENT) {
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

सुसि_Or_सुसि_Sym_List :       सुसि_Or_सुसि_Sym_List COMMA सुसि_Or_सुसि_Sym
                        { $$.head = $1.head; $$.last->common.next = &$3->common; $$.last = $3; }
                |       सुसि_Or_सुसि_Sym
                        { $$.head = $$.last = $1; }
                ;

सुसि_Or_सुसि_Sym     :       KEYNAME
                        { $$ = पाहायाग_Create_सुसि_Name($1); }
                |       सुसि_Sym_2
                        { $$ = पाहायाग_Create_सुसि_Sym($1); }
                ;

LedMapDecl:             INDICATOR String OBRACE VarDeclList CBRACE SEMI
                        { $$ = LedMapCreate($2, $4.head); }
                ;

LedNameDecl:            INDICATOR Integer EQUALS पाहायाग_2 SEMI
                        { $$ = LedNameCreate($2, $4, false); }
                |       VIRTUAL INDICATOR Integer EQUALS पाहायाग_2 SEMI
                        { $$ = LedNameCreate($3, $5, true); }
                ;

UnknownDecl     :       IDENT Terminal EQUALS पाहायाग_2 SEMI
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
                        { $$ = NULL; }
                |       SHAPE String OBRACE CoordList CBRACE SEMI
                        { (void) $4; $$ = NULL; }
                ;

SectionDecl     :       SECTION String OBRACE SectionBody CBRACE SEMI
                        { $$ = NULL; }
                ;

SectionBody     :       SectionBody SectionBodyItem     { $$ = NULL;}
                |       SectionBodyItem                 { $$ = NULL; }
                ;

SectionBodyItem :       ROW OBRACE RowBody CBRACE SEMI
                        { $$ = NULL; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = NULL; }
                |       DoodadDecl
                        { $$ = NULL; }
                |       LedMapDecl
                        { FreeStmt((ParseCommon *) $1); $$ = NULL; }
                |       OverlayDecl
                        { $$ = NULL; }
                ;

RowBody         :       RowBody RowBodyItem     { $$ = NULL;}
                |       RowBodyItem             { $$ = NULL; }
                ;

RowBodyItem     :       मगा_सुसि_2 OBRACE मगा_सुसि_3 CBRACE SEMI { $$ = NULL; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = NULL; }
                ;

मगा_सुसि_3            :       मगा_सुसि_3 COMMA सुसि_4          { $$ = NULL; }
                |       सुसि_4                     { $$ = NULL; }
                ;

सुसि_4             :       KEYNAME
                        { $$ = NULL; }
                |       OBRACE पाहायाग_List_2 CBRACE
                        { FreeStmt((ParseCommon *) $2.head); $$ = NULL; }
                ;

OverlayDecl     :       OVERLAY String OBRACE Overlay_सुसि_List CBRACE SEMI
                        { $$ = NULL; }
                ;

Overlay_सुसि_List  :       Overlay_सुसि_List COMMA Overlay_सुसि { $$ = NULL; }
                |       Overlay_सुसि                      { $$ = NULL; }
                ;

Overlay_सुसि      :       KEYNAME EQUALS KEYNAME          { $$ = NULL; }
                ;

OutlineList     :       OutlineList COMMA OutlineInList
                        { $$ = NULL;}
                |       OutlineInList
                        { $$ = NULL; }
                ;

OutlineInList   :       OBRACE CoordList CBRACE
                        { (void) $2; $$ = NULL; }
                |       Ident EQUALS OBRACE CoordList CBRACE
                        { (void) $4; $$ = NULL; }
                |       Ident EQUALS पाहायाग_2
                        { FreeStmt((ParseCommon *) $3); $$ = NULL; }
                ;

CoordList       :       CoordList COMMA Coord
                        { (void) $1; (void) $3; $$ = NULL; }
                |       Coord
                        { (void) $1; $$ = NULL; }
                ;

Coord           :       OBRACKET Signed_नुमेरो COMMA Signed_नुमेरो CBRACKET
                        { $$ = NULL; }
                ;

DoodadDecl      :       DoodadType String OBRACE VarDeclList CBRACE SEMI
                        { FreeStmt((ParseCommon *) $4.head); $$ = NULL; }
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
                |       सुसि_3
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

पाहायाग_List_2        :       पाहायाग_List_2 COMMA पाहायाग_2
                        {
                            if ($3) {
                                if ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$3->common;
                                    $$.last = $3;
                                } else {
                                    $$.head = $$.last = $3;
                                }
                            }
                        }
                |       पाहायाग_2
                        { $$.head = $$.last = $1; }
                |       { $$.head = $$.last = NULL; }
                ;

पाहायाग_2            :       पाहायाग_2 DIVIDE पाहायाग_2
                        { $$ = पाहायाग_Create_दालावाहान(STMT_पाहायाग_DIVIDE, $1, $3); }
                |       पाहायाग_2 PLUS पाहायाग_2
                        { $$ = पाहायाग_Create_दालावाहान(STMT_पाहायाग_ADD, $1, $3); }
                |       पाहायाग_2 MINUS पाहायाग_2
                        { $$ = पाहायाग_Create_दालावाहान(STMT_पाहायाग_SUBTRACT, $1, $3); }
                |       पाहायाग_2 TIMES पाहायाग_2
                        { $$ = पाहायाग_Create_दालावाहान(STMT_पाहायाग_MULTIPLY, $1, $3); }
                |       Lhs EQUALS पाहायाग_2
                        { $$ = पाहायाग_Create_दालावाहान(STMT_पाहायाग_ASSIGN, $1, $3); }
                |       Term
                        { $$ = $1; }
                ;

Term            :       MINUS Term
                        { $$ = पाहायाग_Create_Unary(STMT_पाहायाग_NEGATE, $2); }
                |       PLUS Term
                        { $$ = पाहायाग_Create_Unary(STMT_पाहायाग_UNARY_PLUS, $2); }
                |       EXCLAM Term
                        { $$ = पाहायाग_Create_Unary(STMT_पाहायाग_NOT, $2); }
                |       INVERT Term
                        { $$ = पाहायाग_Create_Unary(STMT_पाहायाग_INVERT, $2); }
                |       Lhs
                        { $$ = $1; }
                |       FieldSpec OPAREN पाहायाग_List_2 CPAREN %prec OPAREN
                        { $$ = पाहायाग_Create_Action($1, $3.head); }
                |       Actions
                        { $$ = $1; }
                |       Terminal
                        { $$ = $1; }
                |       OPAREN पाहायाग_2 CPAREN
                        { $$ = $2; }
                ;

MultiActionList :       MultiActionList COMMA Action
                        {
                            पाहायाग_Def *पाहायाग = पाहायाग_Create_Action_List($3);
                            $$ = $1;
                            $$.last->common.next = &पाहायाग->common; $$.last = पाहायाग;
                        }
                |       MultiActionList COMMA Actions
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       Action
                        { $$.head = $$.last = पाहायाग_Create_Action_List($1); }
                |       NonEmptyActions
                        { $$.head = $$.last = $1; }
                ;

ActionList      :       ActionList COMMA Action
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       Action
                        { $$.head = $$.last = $1; }
                ;

NonEmptyActions :       OBRACE ActionList CBRACE
                        { $$ = पाहायाग_Create_Action_List($2.head); }
                ;

Actions         :       NonEmptyActions
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = पाहायाग_Create_Action_List(NULL); }
                ;

Action          :       FieldSpec OPAREN पाहायाग_List_2 CPAREN
                        { $$ = पाहायाग_Create_Action($1, $3.head); }
                ;

Lhs             :       FieldSpec
                        { $$ = पाहायाग_Create_Ident($1); }
                |       FieldSpec DOT FieldSpec
                        { $$ = पाहायाग_Create_Field_Ref($1, $3); }
                |       FieldSpec OBRACKET पाहायाग_2 CBRACKET
                        { $$ = पाहायाग_Create_Array_Ref(XKB_ATOM_NONE, $1, $3); }
                |       FieldSpec DOT FieldSpec OBRACKET पाहायाग_2 CBRACKET
                        { $$ = पाहायाग_Create_Array_Ref($1, $3, $5); }
                ;

OptTerminal     :       Terminal
                        { $$ = $1; }
                |       { $$ = NULL; }
                ;

Terminal        :       String
                        { $$ = पाहायाग_Create_String($1); }
                |       Integer
                        { $$ = पाहायाग_Create_Integer($1); }
                |       Float
                        { $$ = पाहायाग_Create_Float(/* Discard $1 */); }
                |       KEYNAME
                        { $$ = पाहायाग_Create_सुसि_Name($1); }
                ;

Multi_सुसि_Sym_List :       Multi_सुसि_Sym_List COMMA सुसि_Sym_Lit
                        {
                            पाहायाग_Def *पाहायाग = पाहायाग_Create_सुसि_Sym_List($3);
                            $$ = $1;
                            $$.last->common.next = &पाहायाग->common; $$.last = पाहायाग;
                        }
                |       Multi_सुसि_Sym_List COMMA सुसि_Syms
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       सुसि_Sym_Lit
                        { $$.head = $$.last = पाहायाग_Create_सुसि_Sym_List($1); }
                |       Non_Empty_सुसि_Syms
                        { $$.head = $$.last = $1; }
                ;

सुसि_Sym_List      :       सुसि_Sym_List COMMA सुसि_Sym_Lit
                        { $$ = पाहायाग_Append_सुसि_Sym_List($1, $3); }
                |       सुसि_Sym_List COMMA STRING
                        {
                            $$ = पाहायाग_सुसि_Sym_List_Append_String(param->scanner, $1, $3);
                            free($3);
                            if (!$$)
                                YYERROR;
                        }
                |       सुसि_Sym_Lit
                        {
                            $$ = पाहायाग_Create_सुसि_Sym_List($1);
                            if (!$$)
                                YYERROR;
                        }
                |       STRING
                        {
                            $$ = पाहायाग_Create_सुसि_Sym_List(XKB_सुसि_No_सिम्बोलो);
                            if (!$$)
                                YYERROR;
                            $$ = पाहायाग_सुसि_Sym_List_Append_String(param->scanner, $$, $1);
                            free($1);
                            if (!$$)
                                YYERROR;
                        }
                ;

Non_Empty_सुसि_Syms :       OBRACE सुसि_Sym_List CBRACE
                        { $$ = $2; }
                |       STRING
                        {
                            $$ = पाहायाग_Create_सुसि_Sym_List(XKB_सुसि_No_सिम्बोलो);
                            if (!$$)
                                YYERROR;
                            $$ = पाहायाग_सुसि_Sym_List_Append_String(param->scanner, $$, $1);
                            free($1);
                            if (!$$)
                                YYERROR;
                        }
                ;

सुसि_Syms         :       Non_Empty_सुसि_Syms
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = पाहायाग_Create_सुसि_Sym_List(XKB_सुसि_No_सिम्बोलो); }
                ;

सुसि_Sym_2          :       सुसि_Sym_Lit
                        { $$ = $1; }
                |       STRING
                        {
                            $$ = KeysymParseString(param->scanner, $1);
                            free($1);
                            if ($$ == XKB_सुसि_No_सिम्बोलो)
                                YYERROR;
                        }
                ;

सुसि_Sym_Lit       :       IDENT
                        {
                            if (!resolve_keysym(param, $1, &$$)) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (unsigned int) $1.len, $1.start
                                );
                                $$ = XKB_सुसि_No_सिम्बोलो;
                            }
                        }
                        /* Handle keysym that is also a keyword  */
                |       SECTION { $$ = XKB_सुसि_section; }
                |       DECIMAL_DIGIT
                        {
                            /*
                             * Special case for digits 0..9:
                             * map to XKB_KEY_0 .. XKB_KEY_9, consistent with
                             * other keysym names: <name> → XKB_KEY_<name>.
                             */
                            $$ = XKB_सुसि_0 + (xkb_keysym_t) $1;
                        }
                |       INTEGER
                        {
                            if ($1 < XKB_KEYSYM_MIN) {
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
                                $$ = XKB_सुसि_No_सिम्बोलो;
                            }
                            /*
                             * Integers 0..9 are handled with DECIMAL_DIGIT if
                             * they were formatted as single characters '0'..'9'.
                             * Otherwise they are handled here as raw keysyms
                             * values. E.g. `01` and `0x1` are interpreted as
                             * the keysym 0x0001, while `1` is interpreted as
                             * XKB_KEY_1.
                             */
                            else {
                                /* Any other numeric value */
                                if ($1 <= XKB_KEYSYM_MAX) {
                                    /*
                                     * Valid keysym
                                     * No normalization is performed and value
                                     * is used as is.
                                     */
                                    $$ = (xkb_keysym_t) $1;
                                    check_deprecated_keysyms(
                                        parser_warn, param, param->ctx,
                                        $$, NULL, $$, "%#06"PRIx32, "");
                                } else {
                                    /* Invalid keysym */
                                    parser_warn(
                                        param, XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                        "unrecognized keysym \"%#06"PRIx64"\" "
                                        "(%"PRId64")", $1, $1
                                    );
                                    $$ = XKB_सुसि_No_सिम्बोलो;
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

Signed_नुमेरो    :       MINUS नुमेरो_2    { $$ = -$2; }
                |       नुमेरो_2          { $$ = $1; }
                ;

नुमेरो_2          :       FLOAT         { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                |       INTEGER       { $$ = $1; }
                ;

Float           :       FLOAT   { $$ = 0; }
                ;

Integer         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

सुसि_Code_2         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

Ident           :       IDENT   { $$ = xkb_atom_intern(param->ctx, $1.start, $1.len); }
                |       DEFAULT { $$ = xkb_atom_intern_literal(param->ctx, "default"); }
                ;

String          :       STRING  { $$ = xkb_atom_intern(param->ctx, $1, strlen($1)); free($1); }
                ;

OptMapName      :       MapName { $$ = $1; }
                |               { $$ = NULL; }
                ;

MapName         :       STRING  { $$ = $1; }
                ;

%%

/* Parse a specific section */
XkbFile *
parse(struct xkb_context *ctx, const struct parser_keymap_config *config,
      struct scanner *scanner, const char *map)
{
    int ret;
    XkbFile *first = NULL;
    struct parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = NULL,
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

    while ((ret = yyparse(&param)) == 0 && param.more_maps) {
        if (map) {
            if (streq_not_null(map, param.rtrn->name))
                return param.rtrn;
            else
                FreeXkbFile(param.rtrn);
        }
        else {
            if (param.rtrn->flags & MAP_IS_DEFAULT) {
                FreeXkbFile(first);
                return param.rtrn;
            }
            else if (!first) {
                first = param.rtrn;
            }
            else {
                FreeXkbFile(param.rtrn);
            }
        }
        param.rtrn = NULL;
    }

    if (ret != 0) {
        /* Some error happend; clear the Xkbfiles parsed so far */
        FreeXkbFile(first);
        FreeXkbFile(param.rtrn);
        return NULL;
    }

    /*
     * Warn about implicit default section,
     * but only if not a keymap: multiple keymaps per file not supported
     */
    if (first && first->file_type != FILE_TYPE_KEYMAP)
        log_vrb(ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_MISSING_DEFAULT_SECTION,
                "No section name in include statement, but \"%s\" contains several; "
                "Using first defined section, \"%s\"\n",
                scanner->file_name, safe_map_name(first->name));

    return first;
}

/* Parse the next section */
bool
parse_next(struct xkb_context *ctx,
           const struct parser_keymap_config *config,
           struct scanner *scanner, XkbFile **xkb_file)
{
    int ret;
    struct parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = NULL,
        .config = *config,
        .more_maps = false,
    };

    if ((ret = yyparse(&param)) == 0 && param.more_maps) {
        *xkb_file = param.rtrn;
        return true;
    } else {
        FreeXkbFile(param.rtrn);
        *xkb_file = NULL;
        return (ret == 0);
    }
}
