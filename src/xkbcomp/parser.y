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
#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ "scanner-utils.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp/ast.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp/xkbcomp-priv.h"
}

%{
#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>

#ଅନ୍ତର୍ଭାଵ "keysym.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp/xkbcomp-priv.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp/parser-priv.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp/ast-build.h"

ସଂରଚନା parser_param {
    ସଂରଚନା xkb_context *ctx;
    ସଂରଚନା scanner *scanner;
    XkbFile *rtrn;
    ସଂରଚନା parser_keymap_config config;
    bool more_maps;
};

#define parser_log_with_code(param, level, verbosity, log_ସନ୍ଦେଶ_id, fmt, ...)   \
    scanner_log_with_code((param)->scanner, level, verbosity, log_ସନ୍ଦେଶ_id, fmt,\
                          ##__VA_ARGS__)

#define parser_err(param, error_id, fmt, ...) \
    scanner_err((param)->scanner, error_id, fmt, ##__VA_ARGS__)

#define parser_warn(param, warning_id, fmt, ...) \
    scanner_warn((param)->scanner, warning_id, fmt, ##__VA_ARGS__)

#define parser_vrb(param, verbosity, warning_id, fmt, ...) \
    scanner_vrb((param)->scanner, verbosity, warning_id, fmt, ##__VA_ARGS__)

ସ୍ଥିର ଶୂନ୍ୟ
_xkbcommon_error(ସଂରଚନା parser_param *param, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ)
{
    parser_err(param, XKB_ERROR_INVALID_XKB_SYNTAX, "%s", ସନ୍ଦେଶ);
}

ସ୍ଥିର bool
resolve_keysym(ସଂରଚନା parser_param *param, ସଂରଚନା sval name, xkb_keysym_t *sym_rtrn)
{
    xkb_keysym_t sym;

    ଯଦି (isvaleq(name, SVAL_LIT("any")) || isvaleq(name, SVAL_LIT("nosymbol"))) {
        *sym_rtrn = XKB_ଚାବି_No_ପ୍ରତୀକ;
        ଫେରନ୍ତୁ true;
    }

    ଯଦି (isvaleq(name, SVAL_LIT("none")) || isvaleq(name, SVAL_LIT("voidsymbol"))) {
        *sym_rtrn = XKB_ଚାବି_Void_ପ୍ରତୀକ;
        ଫେରନ୍ତୁ true;
    }

    /* xkb_keysym_from_name needs a C string. */
    ଅକ୍ଷର buf[XKB_KEYSYM_NAME_MAX_SIZE];
    ଯଦି (name.len >= ଆକାର(buf)) {
        ଫେରନ୍ତୁ false;
    }
    memcpy(buf, name.start, name.len);
    buf[name.len] = '\0';

    sym = xkb_keysym_from_name(buf, XKB_KEYSYM_NO_FLAGS);
    ଯଦି (sym != XKB_ଚାବି_No_ପ୍ରତୀକ) {
        *sym_rtrn = sym;
        check_deprecated_keysyms(parser_warn, param, param->ctx,
                                 sym, buf, buf, "%s", "");
        ଫେରନ୍ତୁ true;
    }

    ଫେରନ୍ତୁ false;
}

#define param_scanner param->scanner
%}

%define api.pure
%lex-param      { ସଂରଚନା scanner *param_scanner }
%parse-param    { ସଂରଚନା parser_param *param }

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
        ଚାବି_3             24 "key"
        ALIAS           25 "alias"
        GROUP           26 "group"
        MODIFIER_MAP    27 "modifier_map"
        INDICATOR       28 "indicator"
        SHAPE           29 "shape"
        ଚାବିଗୁଡ଼ିକ_2            30 "keys"
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
        ALPHANUMERIC_ଚାବିଗୁଡ଼ିକ       73 "alphanumeric_keys"
        MODIFIER_ଚାବିଗୁଡ଼ିକ           74 "modifier_keys"
        KEYPAD_ଚାବିଗୁଡ଼ିକ             75 "keypad_keys"
        FUNCTION_ଚାବିଗୁଡ଼ିକ           76 "function_keys"
        ALTERNATE_GROUP         77 "alternate_group"
        DEPRECATED              78 "deprecated"

%right  EQUALS
%left   PLUS MINUS
%left   TIMES DIVIDE
%left   EXCLAM INVERT
%left   OPAREN

%start  XkbFile

%ସଂଘ  {
        int64_t          ଅଙ୍କ;
        ଗଣନା xkb_file_type file_type;
        ଅକ୍ଷର            *str;
        ସଂରଚନା sval     sval;
        xkb_atom_t      atom;
        ଗଣନା merge_mode merge;
        ଗଣନା xkb_map_flags mapFlags;
        xkb_keysym_t    keysym;
        ParseCommon     *any;
        ସଂରଚନା { ParseCommon *head; ParseCommon *last; } anyList;
        uint32_t        no_ପ୍ରତୀକ_Or_Action_List;
        ପଦସମଷ୍ଟି_Def         *ପଦସମଷ୍ଟି;
        ସଂରଚନା { ପଦସମଷ୍ଟି_Def *head; ପଦସମଷ୍ଟି_Def *last; } ପଦସମଷ୍ଟି_List;
        VarDef          *var;
        ସଂରଚନା { VarDef *head; VarDef *last; } varList;
        VModDef         *vmod;
        ସଂରଚନା { VModDef *head; VModDef *last; } vmodList;
        InterpDef       *interp;
        ଚାବି_Type_Def      *ଚାବି_Type;
        SymbolsDef      *syms;
        ModMapDef       *modMask;
        GroupCompatDef  *groupCompat;
        LedMapDef       *ledMap;
        LedNameDef      *ledName;
        KeycodeDef      *ଚାବି_Code;
        ଚାବି_Alias_Def     *ଚାବି_Alias;
        UnknownStatement *unknown;
        ଶୂନ୍ୟ            *geom;
        XkbFile         *file;
        ସଂରଚନା { XkbFile *head; XkbFile *last; } fileList;
}

%ପ୍ରକାର <ଅଙ୍କ>     DECIMAL_DIGIT INTEGER FLOAT
%ପ୍ରକାର <str>     STRING
%ପ୍ରକାର <sval>    IDENT
%ପ୍ରକାର <atom>    KEYNAME
%ପ୍ରକାର <ଅଙ୍କ>     ଚାବି_Code_2 ସଂଖ୍ୟା Integer Float Signed_ସଂଖ୍ୟା DoodadType
%ପ୍ରକାର <merge>   MergeMode OptMergeMode
%ପ୍ରକାର <file_type> XkbCompositeType FileType
%ପ୍ରକାର <mapFlags> Flag Flags OptFlags
%ପ୍ରକାର <str>     MapName OptMapName
%ପ୍ରକାର <atom>    FieldSpec Ident Element String
%ପ୍ରକାର <keysym>  ଚାବି_Sym_2 ଚାବି_Sym_Lit
%ପ୍ରକାର <no_ପ୍ରତୀକ_Or_Action_List> No_ପ୍ରତୀକ_Or_Action_List
%ପ୍ରକାର <any>     Decl
%ପ୍ରକାର <anyList> DeclList
%ପ୍ରକାର <ପଦସମଷ୍ଟି>    ପଦସମଷ୍ଟି_2 Term Lhs Terminal OptTerminal Coord CoordList
%ପ୍ରକାର <ପଦସମଷ୍ଟି>    Multi_ଚାବି_Sym_Or_Action_List NonEmptyActions Actions Action
%ପ୍ରକାର <ପଦସମଷ୍ଟି>    ଚାବି_Syms Non_Empty_ଚାବି_Syms ଚାବି_Sym_List ଚାବି_Or_ଚାବି_Sym
%ପ୍ରକାର <ପଦସମଷ୍ଟି_List> ପଦସମଷ୍ଟି_List_2 ଚାବି_Or_ଚାବି_Sym_List
%ପ୍ରକାର <ପଦସମଷ୍ଟି_List> ActionList MultiActionList Multi_ଚାବି_Sym_List
%ପ୍ରକାର <var>     VarDecl SymbolsVarDecl
%ପ୍ରକାର <varList> VarDeclList SymbolsBody OptSymbolsBody
%ପ୍ରକାର <vmod>    VModDef
%ପ୍ରକାର <vmodList> VModDefList VModDecl
%ପ୍ରକାର <interp>  InterpretDecl InterpretMatch
%ପ୍ରକାର <ଚାବି_Type> ଚାବି_Type_Decl
%ପ୍ରକାର <syms>    SymbolsDecl
%ପ୍ରକାର <modMask> ModMapDecl
%ପ୍ରକାର <groupCompat> GroupCompatDecl
%ପ୍ରକାର <ledMap>  LedMapDecl
%ପ୍ରକାର <ledName> LedNameDecl
%ପ୍ରକାର <ଚାବି_Code> ଚାବି_Name_Decl
%ପ୍ରକାର <ଚାବି_Alias> ଚାବି_Alias_Decl
%ପ୍ରକାର <geom>    ShapeDecl SectionDecl SectionBody SectionBodyItem RowBody RowBodyItem
%ପ୍ରକାର <geom>    ଚାବିଗୁଡ଼ିକ_3 ଚାବି_4 OverlayDecl Overlay_ଚାବି_List Overlay_ଚାବି OutlineList OutlineInList
%ପ୍ରକାର <geom>    DoodadDecl
%ପ୍ରକାର <unknown> UnknownDecl UnknownCompoundStatementDecl
%ପ୍ରକାର <file>    XkbFile XkbMapConfig
%ପ୍ରକାର <fileList> XkbMapConfigList
%ପ୍ରକାର <file>    XkbCompositeMap

%destructor { FreeStmt((ParseCommon *) $$); }
    <any> <ପଦସମଷ୍ଟି> <var> <vmod> <interp> <ଚାବି_Type> <syms> <modMask> <groupCompat>
    <ledMap> <ledName> <ଚାବି_Code> <ଚାବି_Alias>
%destructor { FreeStmt((ParseCommon *) $$.head); }
    <anyList> <ପଦସମଷ୍ଟି_List> <varList> <vmodList>
/* The destructor also runs on the start symbol when the parser *succeeds*.
 * The `if` here catches this case. */
%destructor { ଯଦି (!param->rtrn) FreeXkbFile($$); } <file>
%destructor { FreeXkbFile($$.head); } <fileList>
%destructor { ମୁକ୍ତ($$); } <str>

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
                        { $$ = param->rtrn = $1; param->more_maps = !!param->rtrn; (ଶୂନ୍ୟ) yynerrs; }
                |       XkbMapConfig
                        { $$ = param->rtrn = $1; param->more_maps = !!param->rtrn; YYACCEPT; }
                |       END_OF_FILE
                        { $$ = param->rtrn = ଖାଲି; param->more_maps = false; }
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
                            ଯଦି ($2) {
                                ଯଦି ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$2->common;
                                    $$.last = $2;
                                } ଅନ୍ୟଥା {
                                    $$.head = $$.last = $2;
                                }
                            }
                        }
                |       { $$.head = $$.last = ଖାଲି; }
                ;

XkbMapConfig    :       OptFlags FileType OptMapName OBRACE
                            DeclList
                        CBRACE SEMI
                        {
                            ଯଦି ($1 & MAP_IS_DEPRECATED) {
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
                |       ALPHANUMERIC_ଚାବିଗୁଡ଼ିକ       { $$ = MAP_HAS_ALPHANUMERIC; }
                |       MODIFIER_ଚାବିଗୁଡ଼ିକ           { $$ = MAP_HAS_MODIFIER; }
                |       KEYPAD_ଚାବିଗୁଡ଼ିକ             { $$ = MAP_HAS_KEYPAD; }
                |       FUNCTION_ଚାବିଗୁଡ଼ିକ           { $$ = MAP_HAS_FN; }
                |       ALTERNATE_GROUP         { $$ = MAP_IS_ALTGR; }
                |       DEPRECATED              { $$ = MAP_IS_DEPRECATED; }
                |       IDENT
                        {
                            ଅଚଳ bool error = (param->config.strict & PARSER_NO_UNKNOWN_SECTION_FLAGS);
                            parser_log_with_code(
                                param, (error ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
                                XKB_LOG_VERBOSITY_MINIMAL,
                                XKB_ERROR_UNKNOWN_SECTION_FLAG,
                                "Unknown section flag \"%.*s\"%s",
                                (unsigned)$1.len, $1.start,
                                (error ? "" : "; ignored")
                            );
                            ଯଦି (error)
                                YYABORT;
                            $$ = 0;
                        }
                ;

DeclList        :       DeclList Decl
                        {
                            ଯଦି ($2) {
                                ଯଦି ($1.head) {
                                    $$.head = $1.head; $1.last->next = $2; $$.last = $2;
                                } ଅନ୍ୟଥା {
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
                            ପାଇଁ (VModDef *vmod = $3.head; vmod; vmod = (VModDef *) vmod->common.next)
                                vmod->merge = $2;
                            ଯଦି ($1.head) {
                                $$.head = $1.head; $1.last->next = &$3.head->common; $$.last = &$3.last->common;
                            } ଅନ୍ୟଥା {
                                $$.head = &$3.head->common; $$.last = &$3.last->common;
                            }
                        }
                |       { $$.head = $$.last = ଖାଲି; }
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
                |       OptMergeMode ଚାବି_Name_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode ଚାବି_Alias_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode ଚାବି_Type_Decl
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
                |       OptMergeMode ShapeDecl          { $$ = ଖାଲି; }
                |       OptMergeMode SectionDecl        { $$ = ଖାଲି; }
                |       OptMergeMode DoodadDecl         { $$ = ଖାଲି; }
                |       OptMergeMode UnknownDecl
                            { $$ = (ParseCommon *) $2; }
                |       OptMergeMode UnknownCompoundStatementDecl
                            { $$ = (ParseCommon *) $2; }
                |       MergeMode STRING
                        {
                            $$ = (ParseCommon *) IncludeCreate(param->ctx, $2, $1);
                            ମୁକ୍ତ($2);
                        }
                ;

VarDecl         :       Lhs EQUALS ପଦସମଷ୍ଟି_2 SEMI
                        { $$ = VarCreate($1, $3); }
                |       Ident SEMI
                        { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident SEMI
                        { $$ = BoolVarCreate($2, false); }
                ;

ଚାବି_Name_Decl     :       KEYNAME EQUALS ଚାବି_Code_2 SEMI
                        { $$ = KeycodeCreate($1, $3); }
                ;

ଚାବି_Alias_Decl    :       ALIAS KEYNAME EQUALS KEYNAME SEMI
                        { $$ = ଚାବି_Alias_Create($2, $4); }
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
                        { $$ = VModCreate($1, ଖାଲି); }
                |       Ident EQUALS ପଦସମଷ୍ଟି_2
                        { $$ = VModCreate($1, $3); }
                ;

InterpretDecl   :       INTERPRET InterpretMatch OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $2->def = $4.head; $$ = $2; }
                ;

InterpretMatch  :       ଚାବି_Sym_2 PLUS ପଦସମଷ୍ଟି_2
                        { $$ = InterpCreate($1, $3); }
                |       ଚାବି_Sym_2
                        { $$ = InterpCreate($1, ଖାଲି); }
                ;

VarDeclList     :       VarDeclList VarDecl
                        {
                            ଯଦି ($2) {
                                ଯଦି ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$2->common;
                                    $$.last = $2;
                                } ଅନ୍ୟଥା {
                                    $$.head = $$.last = $2;
                                }
                            }
                        }
                |       { $$.head = $$.last = ଖାଲି; }
                ;

ଚାବି_Type_Decl     :       TYPE String OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $$ = ଚାବି_Type_Create($2, $4.head); }
                ;

SymbolsDecl     :       ଚାବି_3 KEYNAME OBRACE
                            OptSymbolsBody
                        CBRACE SEMI
                        { $$ = SymbolsCreate($2, $4.head); }
                ;

OptSymbolsBody  :       SymbolsBody { $$ = $1; }
                |                   { $$.head = $$.last = ଖାଲି; }
                ;

SymbolsBody     :       SymbolsBody COMMA SymbolsVarDecl
                        { $$.head = $1.head; $$.last->common.next = &$3->common; $$.last = $3; }
                |       SymbolsVarDecl
                        { $$.head = $$.last = $1; }
                ;

SymbolsVarDecl  :       Lhs EQUALS ପଦସମଷ୍ଟି_2         { $$ = VarCreate($1, $3); }
                |       Lhs EQUALS Multi_ଚାବି_Sym_Or_Action_List { $$ = VarCreate($1, $3); }
                |       Ident                   { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident            { $$ = BoolVarCreate($2, false); }
                |       Multi_ଚାବି_Sym_Or_Action_List { $$ = VarCreate(ଖାଲି, $1); }
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
Multi_ଚାବି_Sym_Or_Action_List
                :       OBRACKET Multi_ଚାବି_Sym_List CBRACKET
                        { $$ = $2.head; }
                |       OBRACKET No_ପ୍ରତୀକ_Or_Action_List COMMA Multi_ଚାବି_Sym_List CBRACKET
                        {
                            /* Prepend n times NoSymbol */
                            ସଂରଚନା {ପଦସମଷ୍ଟି_Def *head; ପଦସମଷ୍ଟି_Def *last;} list = {
                                .head = $4.head, .last = $4.last
                            };
                            ପାଇଁ (uint32_t k = 0; k < $2; k++) {
                                ପଦସମଷ୍ଟି_Def* ଅଚଳ syms =
                                    ପଦସମଷ୍ଟି_Create_ଚାବି_Sym_List(XKB_ଚାବି_No_ପ୍ରତୀକ);
                                ଯଦି (!syms) {
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
                |       OBRACKET No_ପ୍ରତୀକ_Or_Action_List COMMA MultiActionList CBRACKET
                        {
                            /* Prepend n times NoAction() */
                            ସଂରଚନା {ପଦସମଷ୍ଟି_Def *head; ପଦସମଷ୍ଟି_Def *last;} list = {
                                .head = $4.head, .last = $4.last
                            };
                            ପାଇଁ (uint32_t k = 0; k < $2; k++) {
                                ପଦସମଷ୍ଟି_Def* ଅଚଳ acts = ପଦସମଷ୍ଟି_Create_Action_List(ଖାଲି);
                                ଯଦି (!acts) {
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
                |       OBRACKET No_ପ୍ରତୀକ_Or_Action_List CBRACKET
                        { $$ = ପଦସମଷ୍ଟି_Empty_List(); }
                ;

/* A list of `{}`, which remains ambiguous until reaching a keysym or action list */
No_ପ୍ରତୀକ_Or_Action_List
                :       No_ପ୍ରତୀକ_Or_Action_List COMMA OBRACE CBRACE
                        { $$ = $1 + 1; }
                |       OBRACE CBRACE
                        { $$ = 1; }
                |       { $$ = 0; }
                ;

GroupCompatDecl :       GROUP Integer EQUALS ପଦସମଷ୍ଟି_2 SEMI
                        { $$ = GroupCompatCreate($2, $4); }
                ;

ModMapDecl      :       MODIFIER_MAP ପଦସମଷ୍ଟି_2 OBRACE ଚାବି_Or_ଚାବି_Sym_List CBRACE SEMI
                        {
                            ଯଦି (param->config.format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
                                $2->common.ପ୍ରକାର != STMT_ପଦସମଷ୍ଟି_IDENT) {
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

ଚାବି_Or_ଚାବି_Sym_List :       ଚାବି_Or_ଚାବି_Sym_List COMMA ଚାବି_Or_ଚାବି_Sym
                        { $$.head = $1.head; $$.last->common.next = &$3->common; $$.last = $3; }
                |       ଚାବି_Or_ଚାବି_Sym
                        { $$.head = $$.last = $1; }
                ;

ଚାବି_Or_ଚାବି_Sym     :       KEYNAME
                        { $$ = ପଦସମଷ୍ଟି_Create_ଚାବି_Name($1); }
                |       ଚାବି_Sym_2
                        { $$ = ପଦସମଷ୍ଟି_Create_ଚାବି_Sym($1); }
                ;

LedMapDecl:             INDICATOR String OBRACE VarDeclList CBRACE SEMI
                        { $$ = LedMapCreate($2, $4.head); }
                ;

LedNameDecl:            INDICATOR Integer EQUALS ପଦସମଷ୍ଟି_2 SEMI
                        { $$ = LedNameCreate($2, $4, false); }
                |       VIRTUAL INDICATOR Integer EQUALS ପଦସମଷ୍ଟି_2 SEMI
                        { $$ = LedNameCreate($3, $5, true); }
                ;

UnknownDecl     :       IDENT Terminal EQUALS ପଦସମଷ୍ଟି_2 SEMI
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
                        { $$ = ଖାଲି; }
                |       SHAPE String OBRACE CoordList CBRACE SEMI
                        { (ଶୂନ୍ୟ) $4; $$ = ଖାଲି; }
                ;

SectionDecl     :       SECTION String OBRACE SectionBody CBRACE SEMI
                        { $$ = ଖାଲି; }
                ;

SectionBody     :       SectionBody SectionBodyItem     { $$ = ଖାଲି;}
                |       SectionBodyItem                 { $$ = ଖାଲି; }
                ;

SectionBodyItem :       ROW OBRACE RowBody CBRACE SEMI
                        { $$ = ଖାଲି; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ଖାଲି; }
                |       DoodadDecl
                        { $$ = ଖାଲି; }
                |       LedMapDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ଖାଲି; }
                |       OverlayDecl
                        { $$ = ଖାଲି; }
                ;

RowBody         :       RowBody RowBodyItem     { $$ = ଖାଲି;}
                |       RowBodyItem             { $$ = ଖାଲି; }
                ;

RowBodyItem     :       ଚାବିଗୁଡ଼ିକ_2 OBRACE ଚାବିଗୁଡ଼ିକ_3 CBRACE SEMI { $$ = ଖାଲି; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ଖାଲି; }
                ;

ଚାବିଗୁଡ଼ିକ_3            :       ଚାବିଗୁଡ଼ିକ_3 COMMA ଚାବି_4          { $$ = ଖାଲି; }
                |       ଚାବି_4                     { $$ = ଖାଲି; }
                ;

ଚାବି_4             :       KEYNAME
                        { $$ = ଖାଲି; }
                |       OBRACE ପଦସମଷ୍ଟି_List_2 CBRACE
                        { FreeStmt((ParseCommon *) $2.head); $$ = ଖାଲି; }
                ;

OverlayDecl     :       OVERLAY String OBRACE Overlay_ଚାବି_List CBRACE SEMI
                        { $$ = ଖାଲି; }
                ;

Overlay_ଚାବି_List  :       Overlay_ଚାବି_List COMMA Overlay_ଚାବି { $$ = ଖାଲି; }
                |       Overlay_ଚାବି                      { $$ = ଖାଲି; }
                ;

Overlay_ଚାବି      :       KEYNAME EQUALS KEYNAME          { $$ = ଖାଲି; }
                ;

OutlineList     :       OutlineList COMMA OutlineInList
                        { $$ = ଖାଲି;}
                |       OutlineInList
                        { $$ = ଖାଲି; }
                ;

OutlineInList   :       OBRACE CoordList CBRACE
                        { (ଶୂନ୍ୟ) $2; $$ = ଖାଲି; }
                |       Ident EQUALS OBRACE CoordList CBRACE
                        { (ଶୂନ୍ୟ) $4; $$ = ଖାଲି; }
                |       Ident EQUALS ପଦସମଷ୍ଟି_2
                        { FreeStmt((ParseCommon *) $3); $$ = ଖାଲି; }
                ;

CoordList       :       CoordList COMMA Coord
                        { (ଶୂନ୍ୟ) $1; (ଶୂନ୍ୟ) $3; $$ = ଖାଲି; }
                |       Coord
                        { (ଶୂନ୍ୟ) $1; $$ = ଖାଲି; }
                ;

Coord           :       OBRACKET Signed_ସଂଖ୍ୟା COMMA Signed_ସଂଖ୍ୟା CBRACKET
                        { $$ = ଖାଲି; }
                ;

DoodadDecl      :       DoodadType String OBRACE VarDeclList CBRACE SEMI
                        { FreeStmt((ParseCommon *) $4.head); $$ = ଖାଲି; }
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
                |       ଚାବି_3
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

ପଦସମଷ୍ଟି_List_2        :       ପଦସମଷ୍ଟି_List_2 COMMA ପଦସମଷ୍ଟି_2
                        {
                            ଯଦି ($3) {
                                ଯଦି ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$3->common;
                                    $$.last = $3;
                                } ଅନ୍ୟଥା {
                                    $$.head = $$.last = $3;
                                }
                            }
                        }
                |       ପଦସମଷ୍ଟି_2
                        { $$.head = $$.last = $1; }
                |       { $$.head = $$.last = ଖାଲି; }
                ;

ପଦସମଷ୍ଟି_2            :       ପଦସମଷ୍ଟି_2 DIVIDE ପଦସମଷ୍ଟି_2
                        { $$ = ପଦସମଷ୍ଟି_Create_ଦ୍ୱିମିକ(STMT_ପଦସମଷ୍ଟି_DIVIDE, $1, $3); }
                |       ପଦସମଷ୍ଟି_2 PLUS ପଦସମଷ୍ଟି_2
                        { $$ = ପଦସମଷ୍ଟି_Create_ଦ୍ୱିମିକ(STMT_ପଦସମଷ୍ଟି_ADD, $1, $3); }
                |       ପଦସମଷ୍ଟି_2 MINUS ପଦସମଷ୍ଟି_2
                        { $$ = ପଦସମଷ୍ଟି_Create_ଦ୍ୱିମିକ(STMT_ପଦସମଷ୍ଟି_SUBTRACT, $1, $3); }
                |       ପଦସମଷ୍ଟି_2 TIMES ପଦସମଷ୍ଟି_2
                        { $$ = ପଦସମଷ୍ଟି_Create_ଦ୍ୱିମିକ(STMT_ପଦସମଷ୍ଟି_MULTIPLY, $1, $3); }
                |       Lhs EQUALS ପଦସମଷ୍ଟି_2
                        { $$ = ପଦସମଷ୍ଟି_Create_ଦ୍ୱିମିକ(STMT_ପଦସମଷ୍ଟି_ASSIGN, $1, $3); }
                |       Term
                        { $$ = $1; }
                ;

Term            :       MINUS Term
                        { $$ = ପଦସମଷ୍ଟି_Create_Unary(STMT_ପଦସମଷ୍ଟି_NEGATE, $2); }
                |       PLUS Term
                        { $$ = ପଦସମଷ୍ଟି_Create_Unary(STMT_ପଦସମଷ୍ଟି_UNARY_PLUS, $2); }
                |       EXCLAM Term
                        { $$ = ପଦସମଷ୍ଟି_Create_Unary(STMT_ପଦସମଷ୍ଟି_NOT, $2); }
                |       INVERT Term
                        { $$ = ପଦସମଷ୍ଟି_Create_Unary(STMT_ପଦସମଷ୍ଟି_INVERT, $2); }
                |       Lhs
                        { $$ = $1; }
                |       FieldSpec OPAREN ପଦସମଷ୍ଟି_List_2 CPAREN %prec OPAREN
                        { $$ = ପଦସମଷ୍ଟି_Create_Action($1, $3.head); }
                |       Actions
                        { $$ = $1; }
                |       Terminal
                        { $$ = $1; }
                |       OPAREN ପଦସମଷ୍ଟି_2 CPAREN
                        { $$ = $2; }
                ;

MultiActionList :       MultiActionList COMMA Action
                        {
                            ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି = ପଦସମଷ୍ଟି_Create_Action_List($3);
                            $$ = $1;
                            $$.last->common.next = &ପଦସମଷ୍ଟି->common; $$.last = ପଦସମଷ୍ଟି;
                        }
                |       MultiActionList COMMA Actions
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       Action
                        { $$.head = $$.last = ପଦସମଷ୍ଟି_Create_Action_List($1); }
                |       NonEmptyActions
                        { $$.head = $$.last = $1; }
                ;

ActionList      :       ActionList COMMA Action
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       Action
                        { $$.head = $$.last = $1; }
                ;

NonEmptyActions :       OBRACE ActionList CBRACE
                        { $$ = ପଦସମଷ୍ଟି_Create_Action_List($2.head); }
                ;

Actions         :       NonEmptyActions
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = ପଦସମଷ୍ଟି_Create_Action_List(ଖାଲି); }
                ;

Action          :       FieldSpec OPAREN ପଦସମଷ୍ଟି_List_2 CPAREN
                        { $$ = ପଦସମଷ୍ଟି_Create_Action($1, $3.head); }
                ;

Lhs             :       FieldSpec
                        { $$ = ପଦସମଷ୍ଟି_Create_Ident($1); }
                |       FieldSpec DOT FieldSpec
                        { $$ = ପଦସମଷ୍ଟି_Create_Field_Ref($1, $3); }
                |       FieldSpec OBRACKET ପଦସମଷ୍ଟି_2 CBRACKET
                        { $$ = ପଦସମଷ୍ଟି_Create_Array_Ref(XKB_ATOM_NONE, $1, $3); }
                |       FieldSpec DOT FieldSpec OBRACKET ପଦସମଷ୍ଟି_2 CBRACKET
                        { $$ = ପଦସମଷ୍ଟି_Create_Array_Ref($1, $3, $5); }
                ;

OptTerminal     :       Terminal
                        { $$ = $1; }
                |       { $$ = ଖାଲି; }
                ;

Terminal        :       String
                        { $$ = ପଦସମଷ୍ଟି_Create_String($1); }
                |       Integer
                        { $$ = ପଦସମଷ୍ଟି_Create_Integer($1); }
                |       Float
                        { $$ = ପଦସମଷ୍ଟି_Create_Float(/* Discard $1 */); }
                |       KEYNAME
                        { $$ = ପଦସମଷ୍ଟି_Create_ଚାବି_Name($1); }
                ;

Multi_ଚାବି_Sym_List :       Multi_ଚାବି_Sym_List COMMA ଚାବି_Sym_Lit
                        {
                            ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି = ପଦସମଷ୍ଟି_Create_ଚାବି_Sym_List($3);
                            $$ = $1;
                            $$.last->common.next = &ପଦସମଷ୍ଟି->common; $$.last = ପଦସମଷ୍ଟି;
                        }
                |       Multi_ଚାବି_Sym_List COMMA ଚାବି_Syms
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       ଚାବି_Sym_Lit
                        { $$.head = $$.last = ପଦସମଷ୍ଟି_Create_ଚାବି_Sym_List($1); }
                |       Non_Empty_ଚାବି_Syms
                        { $$.head = $$.last = $1; }
                ;

ଚାବି_Sym_List      :       ଚାବି_Sym_List COMMA ଚାବି_Sym_Lit
                        { $$ = ପଦସମଷ୍ଟି_Append_ଚାବି_Sym_List($1, $3); }
                |       ଚାବି_Sym_List COMMA STRING
                        {
                            $$ = ପଦସମଷ୍ଟି_ଚାବି_Sym_List_Append_String(param->scanner, $1, $3);
                            ମୁକ୍ତ($3);
                            ଯଦି (!$$)
                                YYERROR;
                        }
                |       ଚାବି_Sym_Lit
                        {
                            $$ = ପଦସମଷ୍ଟି_Create_ଚାବି_Sym_List($1);
                            ଯଦି (!$$)
                                YYERROR;
                        }
                |       STRING
                        {
                            $$ = ପଦସମଷ୍ଟି_Create_ଚାବି_Sym_List(XKB_ଚାବି_No_ପ୍ରତୀକ);
                            ଯଦି (!$$)
                                YYERROR;
                            $$ = ପଦସମଷ୍ଟି_ଚାବି_Sym_List_Append_String(param->scanner, $$, $1);
                            ମୁକ୍ତ($1);
                            ଯଦି (!$$)
                                YYERROR;
                        }
                ;

Non_Empty_ଚାବି_Syms :       OBRACE ଚାବି_Sym_List CBRACE
                        { $$ = $2; }
                |       STRING
                        {
                            $$ = ପଦସମଷ୍ଟି_Create_ଚାବି_Sym_List(XKB_ଚାବି_No_ପ୍ରତୀକ);
                            ଯଦି (!$$)
                                YYERROR;
                            $$ = ପଦସମଷ୍ଟି_ଚାବି_Sym_List_Append_String(param->scanner, $$, $1);
                            ମୁକ୍ତ($1);
                            ଯଦି (!$$)
                                YYERROR;
                        }
                ;

ଚାବି_Syms         :       Non_Empty_ଚାବି_Syms
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = ପଦସମଷ୍ଟି_Create_ଚାବି_Sym_List(XKB_ଚାବି_No_ପ୍ରତୀକ); }
                ;

ଚାବି_Sym_2          :       ଚାବି_Sym_Lit
                        { $$ = $1; }
                |       STRING
                        {
                            $$ = KeysymParseString(param->scanner, $1);
                            ମୁକ୍ତ($1);
                            ଯଦି ($$ == XKB_ଚାବି_No_ପ୍ରତୀକ)
                                YYERROR;
                        }
                ;

ଚାବି_Sym_Lit       :       IDENT
                        {
                            ଯଦି (!resolve_keysym(param, $1, &$$)) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) $1.len, $1.start
                                );
                                $$ = XKB_ଚାବି_No_ପ୍ରତୀକ;
                            }
                        }
                        /* Handle keysym that is also a keyword  */
                |       SECTION { $$ = XKB_ଚାବି_section; }
                |       DECIMAL_DIGIT
                        {
                            /*
                             * Special case for digits 0..9:
                             * map to XKB_KEY_0 .. XKB_KEY_9, consistent with
                             * other keysym names: <name> → XKB_KEY_<name>.
                             */
                            $$ = XKB_ଚାବି_0 + (xkb_keysym_t) $1;
                        }
                |       INTEGER
                        {
                            ଯଦି ($1 < XKB_KEYSYM_MIN) {
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
                                $$ = XKB_ଚାବି_No_ପ୍ରତୀକ;
                            }
                            /*
                             * Integers 0..9 are handled with DECIMAL_DIGIT if
                             * they were formatted as single characters '0'..'9'.
                             * Otherwise they are handled here as raw keysyms
                             * values. E.g. `01` and `0x1` are interpreted as
                             * the keysym 0x0001, while `1` is interpreted as
                             * XKB_KEY_1.
                             */
                            ଅନ୍ୟଥା {
                                /* Any other numeric value */
                                ଯଦି ($1 <= XKB_KEYSYM_MAX) {
                                    /*
                                     * Valid keysym
                                     * No normalization is performed and value
                                     * is used as is.
                                     */
                                    $$ = (xkb_keysym_t) $1;
                                    check_deprecated_keysyms(
                                        parser_warn, param, param->ctx,
                                        $$, ଖାଲି, $$, "%#06"PRIx32, "");
                                } ଅନ୍ୟଥା {
                                    /* Invalid keysym */
                                    parser_warn(
                                        param, XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                        "unrecognized keysym \"%#06"PRIx64"\" "
                                        "(%"PRId64")", $1, $1
                                    );
                                    $$ = XKB_ଚାବି_No_ପ୍ରତୀକ;
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

Signed_ସଂଖ୍ୟା    :       MINUS ସଂଖ୍ୟା    { $$ = -$2; }
                |       ସଂଖ୍ୟା          { $$ = $1; }
                ;

ସଂଖ୍ୟା          :       FLOAT         { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                |       INTEGER       { $$ = $1; }
                ;

Float           :       FLOAT   { $$ = 0; }
                ;

Integer         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

ଚାବି_Code_2         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

Ident           :       IDENT   { $$ = xkb_atom_intern(param->ctx, $1.start, $1.len); }
                |       DEFAULT { $$ = xkb_atom_intern_literal(param->ctx, "default"); }
                ;

String          :       STRING  { $$ = xkb_atom_intern(param->ctx, $1, strlen($1)); ମୁକ୍ତ($1); }
                ;

OptMapName      :       MapName { $$ = $1; }
                |               { $$ = ଖାଲି; }
                ;

MapName         :       STRING  { $$ = $1; }
                ;

%%

/* Parse a specific section */
XkbFile *
parse(ସଂରଚନା xkb_context *ctx, ଅଚଳ ସଂରଚନା parser_keymap_config *config,
      ସଂରଚନା scanner *scanner, ଅଚଳ ଅକ୍ଷର *map)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
    XkbFile *first = ଖାଲି;
    ସଂରଚନା parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = ଖାଲି,
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

    ଯେତେବେଳେ ((ret = yyparse(&param)) == 0 && param.more_maps) {
        ଯଦି (map) {
            ଯଦି (streq_not_null(map, param.rtrn->name))
                ଫେରନ୍ତୁ param.rtrn;
            ଅନ୍ୟଥା
                FreeXkbFile(param.rtrn);
        }
        ଅନ୍ୟଥା {
            ଯଦି (param.rtrn->flags & MAP_IS_DEFAULT) {
                FreeXkbFile(first);
                ଫେରନ୍ତୁ param.rtrn;
            }
            ଅନ୍ୟଥା ଯଦି (!first) {
                first = param.rtrn;
            }
            ଅନ୍ୟଥା {
                FreeXkbFile(param.rtrn);
            }
        }
        param.rtrn = ଖାଲି;
    }

    ଯଦି (ret != 0) {
        /* Some error happend; clear the Xkbfiles parsed so far */
        FreeXkbFile(first);
        FreeXkbFile(param.rtrn);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    /*
     * Warn about implicit default section,
     * but only if not a keymap: multiple keymaps per file not supported
     */
    ଯଦି (first && first->file_type != FILE_TYPE_KEYMAP)
        log_vrb(ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_MISSING_DEFAULT_SECTION,
                "No section name in include statement, but \"%s\" contains several; "
                "Using first defined section, \"%s\"\n",
                scanner->file_name, safe_map_name(first->name));

    ଫେରନ୍ତୁ first;
}

/* Parse the next section */
bool
parse_next(ସଂରଚନା xkb_context *ctx,
           ଅଚଳ ସଂରଚନା parser_keymap_config *config,
           ସଂରଚନା scanner *scanner, XkbFile **xkb_file)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
    ସଂରଚନା parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = ଖାଲି,
        .config = *config,
        .more_maps = false,
    };

    ଯଦି ((ret = yyparse(&param)) == 0 && param.more_maps) {
        *xkb_file = param.rtrn;
        ଫେରନ୍ତୁ true;
    } ଅନ୍ୟଥା {
        FreeXkbFile(param.rtrn);
        *xkb_file = ଖାଲି;
        ଫେରନ୍ତୁ (ret == 0);
    }
}
