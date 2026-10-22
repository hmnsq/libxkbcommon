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
#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ "scanner-utils.h"
#અન્તર્ભાવ "xkbcomp/ast.h"
#અન્તર્ભાવ "xkbcomp/xkbcomp-priv.h"
}

%{
#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>

#અન્તર્ભાવ "keysym.h"
#અન્તર્ભાવ "xkbcomp/xkbcomp-priv.h"
#અન્તર્ભાવ "xkbcomp/parser-priv.h"
#અન્તર્ભાવ "xkbcomp/ast-build.h"

સંરચના parser_param {
    સંરચના xkb_context *ctx;
    સંરચના scanner *scanner;
    XkbFile *rtrn;
    સંરચના parser_keymap_config config;
    bool more_maps;
};

#define parser_log_with_code(param, level, verbosity, log_સંદેશ_id, fmt, ...)   \
    scanner_log_with_code((param)->scanner, level, verbosity, log_સંદેશ_id, fmt,\
                          ##__VA_ARGS__)

#define parser_err(param, error_id, fmt, ...) \
    scanner_err((param)->scanner, error_id, fmt, ##__VA_ARGS__)

#define parser_warn(param, warning_id, fmt, ...) \
    scanner_warn((param)->scanner, warning_id, fmt, ##__VA_ARGS__)

#define parser_vrb(param, verbosity, warning_id, fmt, ...) \
    scanner_vrb((param)->scanner, verbosity, warning_id, fmt, ##__VA_ARGS__)

સ્થિર શૂન્ય
_xkbcommon_error(સંરચના parser_param *param, અચલ અક્ષર *સંદેશ)
{
    parser_err(param, XKB_ERROR_INVALID_XKB_SYNTAX, "%s", સંદેશ);
}

સ્થિર bool
resolve_keysym(સંરચના parser_param *param, સંરચના sval name, xkb_keysym_t *sym_rtrn)
{
    xkb_keysym_t sym;

    જો (isvaleq(name, SVAL_LIT("any")) || isvaleq(name, SVAL_LIT("nosymbol"))) {
        *sym_rtrn = XKB_ચાવી_No_પ્રતીક;
        પાછા true;
    }

    જો (isvaleq(name, SVAL_LIT("none")) || isvaleq(name, SVAL_LIT("voidsymbol"))) {
        *sym_rtrn = XKB_ચાવી_Void_પ્રતીક;
        પાછા true;
    }

    /* xkb_keysym_from_name needs a C string. */
    અક્ષર buf[XKB_KEYSYM_NAME_MAX_SIZE];
    જો (name.len >= કદ(buf)) {
        પાછા false;
    }
    memcpy(buf, name.start, name.len);
    buf[name.len] = '\0';

    sym = xkb_keysym_from_name(buf, XKB_KEYSYM_NO_FLAGS);
    જો (sym != XKB_ચાવી_No_પ્રતીક) {
        *sym_rtrn = sym;
        check_deprecated_keysyms(parser_warn, param, param->ctx,
                                 sym, buf, buf, "%s", "");
        પાછા true;
    }

    પાછા false;
}

#define param_scanner param->scanner
%}

%define api.pure
%lex-param      { સંરચના scanner *param_scanner }
%parse-param    { સંરચના parser_param *param }

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
        ચાવી_3             24 "key"
        ALIAS           25 "alias"
        GROUP           26 "group"
        MODIFIER_MAP    27 "modifier_map"
        INDICATOR       28 "indicator"
        SHAPE           29 "shape"
        ચાવીઓ_2            30 "keys"
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
        ALPHANUMERIC_ચાવીઓ       73 "alphanumeric_keys"
        MODIFIER_ચાવીઓ           74 "modifier_keys"
        KEYPAD_ચાવીઓ             75 "keypad_keys"
        FUNCTION_ચાવીઓ           76 "function_keys"
        ALTERNATE_GROUP         77 "alternate_group"
        DEPRECATED              78 "deprecated"

%right  EQUALS
%left   PLUS MINUS
%left   TIMES DIVIDE
%left   EXCLAM INVERT
%left   OPAREN

%start  XkbFile

%સંઘ  {
        int64_t          અંક;
        ગણના xkb_file_type file_type;
        અક્ષર            *str;
        સંરચના sval     sval;
        xkb_atom_t      atom;
        ગણના merge_mode merge;
        ગણના xkb_map_flags mapFlags;
        xkb_keysym_t    keysym;
        ParseCommon     *any;
        સંરચના { ParseCommon *head; ParseCommon *last; } anyList;
        uint32_t        no_પ્રતીક_Or_Action_List;
        પદાવલિ_Def         *પદાવલિ;
        સંરચના { પદાવલિ_Def *head; પદાવલિ_Def *last; } પદાવલિ_List;
        VarDef          *var;
        સંરચના { VarDef *head; VarDef *last; } varList;
        VModDef         *vmod;
        સંરચના { VModDef *head; VModDef *last; } vmodList;
        InterpDef       *interp;
        ચાવી_Type_Def      *ચાવી_Type;
        SymbolsDef      *syms;
        ModMapDef       *modMask;
        GroupCompatDef  *groupCompat;
        LedMapDef       *ledMap;
        LedNameDef      *ledName;
        KeycodeDef      *ચાવી_Code;
        ચાવી_Alias_Def     *ચાવી_Alias;
        UnknownStatement *unknown;
        શૂન્ય            *geom;
        XkbFile         *file;
        સંરચના { XkbFile *head; XkbFile *last; } fileList;
}

%પ્રકાર <અંક>     DECIMAL_DIGIT INTEGER FLOAT
%પ્રકાર <str>     STRING
%પ્રકાર <sval>    IDENT
%પ્રકાર <atom>    KEYNAME
%પ્રકાર <અંક>     ચાવી_Code_2 સંખ્યા Integer Float Signed_સંખ્યા DoodadType
%પ્રકાર <merge>   MergeMode OptMergeMode
%પ્રકાર <file_type> XkbCompositeType FileType
%પ્રકાર <mapFlags> Flag Flags OptFlags
%પ્રકાર <str>     MapName OptMapName
%પ્રકાર <atom>    FieldSpec Ident Element String
%પ્રકાર <keysym>  ચાવી_Sym_2 ચાવી_Sym_Lit
%પ્રકાર <no_પ્રતીક_Or_Action_List> No_પ્રતીક_Or_Action_List
%પ્રકાર <any>     Decl
%પ્રકાર <anyList> DeclList
%પ્રકાર <પદાવલિ>    પદાવલિ_2 Term Lhs Terminal OptTerminal Coord CoordList
%પ્રકાર <પદાવલિ>    Multi_ચાવી_Sym_Or_Action_List NonEmptyActions Actions Action
%પ્રકાર <પદાવલિ>    ચાવી_Syms Non_Empty_ચાવી_Syms ચાવી_Sym_List ચાવી_Or_ચાવી_Sym
%પ્રકાર <પદાવલિ_List> પદાવલિ_List_2 ચાવી_Or_ચાવી_Sym_List
%પ્રકાર <પદાવલિ_List> ActionList MultiActionList Multi_ચાવી_Sym_List
%પ્રકાર <var>     VarDecl SymbolsVarDecl
%પ્રકાર <varList> VarDeclList SymbolsBody OptSymbolsBody
%પ્રકાર <vmod>    VModDef
%પ્રકાર <vmodList> VModDefList VModDecl
%પ્રકાર <interp>  InterpretDecl InterpretMatch
%પ્રકાર <ચાવી_Type> ચાવી_Type_Decl
%પ્રકાર <syms>    SymbolsDecl
%પ્રકાર <modMask> ModMapDecl
%પ્રકાર <groupCompat> GroupCompatDecl
%પ્રકાર <ledMap>  LedMapDecl
%પ્રકાર <ledName> LedNameDecl
%પ્રકાર <ચાવી_Code> ચાવી_Name_Decl
%પ્રકાર <ચાવી_Alias> ચાવી_Alias_Decl
%પ્રકાર <geom>    ShapeDecl SectionDecl SectionBody SectionBodyItem RowBody RowBodyItem
%પ્રકાર <geom>    ચાવીઓ_3 ચાવી_4 OverlayDecl Overlay_ચાવી_List Overlay_ચાવી OutlineList OutlineInList
%પ્રકાર <geom>    DoodadDecl
%પ્રકાર <unknown> UnknownDecl UnknownCompoundStatementDecl
%પ્રકાર <file>    XkbFile XkbMapConfig
%પ્રકાર <fileList> XkbMapConfigList
%પ્રકાર <file>    XkbCompositeMap

%destructor { FreeStmt((ParseCommon *) $$); }
    <any> <પદાવલિ> <var> <vmod> <interp> <ચાવી_Type> <syms> <modMask> <groupCompat>
    <ledMap> <ledName> <ચાવી_Code> <ચાવી_Alias>
%destructor { FreeStmt((ParseCommon *) $$.head); }
    <anyList> <પદાવલિ_List> <varList> <vmodList>
/* The destructor also runs on the start symbol when the parser *succeeds*.
 * The `if` here catches this case. */
%destructor { જો (!param->rtrn) FreeXkbFile($$); } <file>
%destructor { FreeXkbFile($$.head); } <fileList>
%destructor { મુક્ત($$); } <str>

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
                        { $$ = param->rtrn = $1; param->more_maps = !!param->rtrn; (શૂન્ય) yynerrs; }
                |       XkbMapConfig
                        { $$ = param->rtrn = $1; param->more_maps = !!param->rtrn; YYACCEPT; }
                |       END_OF_FILE
                        { $$ = param->rtrn = ખાલી; param->more_maps = false; }
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
                            જો ($2) {
                                જો ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$2->common;
                                    $$.last = $2;
                                } નહીંતર {
                                    $$.head = $$.last = $2;
                                }
                            }
                        }
                |       { $$.head = $$.last = ખાલી; }
                ;

XkbMapConfig    :       OptFlags FileType OptMapName OBRACE
                            DeclList
                        CBRACE SEMI
                        {
                            જો ($1 & MAP_IS_DEPRECATED) {
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
                |       ALPHANUMERIC_ચાવીઓ       { $$ = MAP_HAS_ALPHANUMERIC; }
                |       MODIFIER_ચાવીઓ           { $$ = MAP_HAS_MODIFIER; }
                |       KEYPAD_ચાવીઓ             { $$ = MAP_HAS_KEYPAD; }
                |       FUNCTION_ચાવીઓ           { $$ = MAP_HAS_FN; }
                |       ALTERNATE_GROUP         { $$ = MAP_IS_ALTGR; }
                |       DEPRECATED              { $$ = MAP_IS_DEPRECATED; }
                |       IDENT
                        {
                            અચલ bool error = (param->config.strict & PARSER_NO_UNKNOWN_SECTION_FLAGS);
                            parser_log_with_code(
                                param, (error ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
                                XKB_LOG_VERBOSITY_MINIMAL,
                                XKB_ERROR_UNKNOWN_SECTION_FLAG,
                                "Unknown section flag \"%.*s\"%s",
                                (unsigned)$1.len, $1.start,
                                (error ? "" : "; ignored")
                            );
                            જો (error)
                                YYABORT;
                            $$ = 0;
                        }
                ;

DeclList        :       DeclList Decl
                        {
                            જો ($2) {
                                જો ($1.head) {
                                    $$.head = $1.head; $1.last->next = $2; $$.last = $2;
                                } નહીંતર {
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
                            માટે (VModDef *vmod = $3.head; vmod; vmod = (VModDef *) vmod->common.next)
                                vmod->merge = $2;
                            જો ($1.head) {
                                $$.head = $1.head; $1.last->next = &$3.head->common; $$.last = &$3.last->common;
                            } નહીંતર {
                                $$.head = &$3.head->common; $$.last = &$3.last->common;
                            }
                        }
                |       { $$.head = $$.last = ખાલી; }
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
                |       OptMergeMode ચાવી_Name_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode ચાવી_Alias_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode ચાવી_Type_Decl
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
                |       OptMergeMode ShapeDecl          { $$ = ખાલી; }
                |       OptMergeMode SectionDecl        { $$ = ખાલી; }
                |       OptMergeMode DoodadDecl         { $$ = ખાલી; }
                |       OptMergeMode UnknownDecl
                            { $$ = (ParseCommon *) $2; }
                |       OptMergeMode UnknownCompoundStatementDecl
                            { $$ = (ParseCommon *) $2; }
                |       MergeMode STRING
                        {
                            $$ = (ParseCommon *) IncludeCreate(param->ctx, $2, $1);
                            મુક્ત($2);
                        }
                ;

VarDecl         :       Lhs EQUALS પદાવલિ_2 SEMI
                        { $$ = VarCreate($1, $3); }
                |       Ident SEMI
                        { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident SEMI
                        { $$ = BoolVarCreate($2, false); }
                ;

ચાવી_Name_Decl     :       KEYNAME EQUALS ચાવી_Code_2 SEMI
                        { $$ = KeycodeCreate($1, $3); }
                ;

ચાવી_Alias_Decl    :       ALIAS KEYNAME EQUALS KEYNAME SEMI
                        { $$ = ચાવી_Alias_Create($2, $4); }
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
                        { $$ = VModCreate($1, ખાલી); }
                |       Ident EQUALS પદાવલિ_2
                        { $$ = VModCreate($1, $3); }
                ;

InterpretDecl   :       INTERPRET InterpretMatch OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $2->def = $4.head; $$ = $2; }
                ;

InterpretMatch  :       ચાવી_Sym_2 PLUS પદાવલિ_2
                        { $$ = InterpCreate($1, $3); }
                |       ચાવી_Sym_2
                        { $$ = InterpCreate($1, ખાલી); }
                ;

VarDeclList     :       VarDeclList VarDecl
                        {
                            જો ($2) {
                                જો ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$2->common;
                                    $$.last = $2;
                                } નહીંતર {
                                    $$.head = $$.last = $2;
                                }
                            }
                        }
                |       { $$.head = $$.last = ખાલી; }
                ;

ચાવી_Type_Decl     :       TYPE String OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $$ = ચાવી_Type_Create($2, $4.head); }
                ;

SymbolsDecl     :       ચાવી_3 KEYNAME OBRACE
                            OptSymbolsBody
                        CBRACE SEMI
                        { $$ = SymbolsCreate($2, $4.head); }
                ;

OptSymbolsBody  :       SymbolsBody { $$ = $1; }
                |                   { $$.head = $$.last = ખાલી; }
                ;

SymbolsBody     :       SymbolsBody COMMA SymbolsVarDecl
                        { $$.head = $1.head; $$.last->common.next = &$3->common; $$.last = $3; }
                |       SymbolsVarDecl
                        { $$.head = $$.last = $1; }
                ;

SymbolsVarDecl  :       Lhs EQUALS પદાવલિ_2         { $$ = VarCreate($1, $3); }
                |       Lhs EQUALS Multi_ચાવી_Sym_Or_Action_List { $$ = VarCreate($1, $3); }
                |       Ident                   { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident            { $$ = BoolVarCreate($2, false); }
                |       Multi_ચાવી_Sym_Or_Action_List { $$ = VarCreate(ખાલી, $1); }
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
Multi_ચાવી_Sym_Or_Action_List
                :       OBRACKET Multi_ચાવી_Sym_List CBRACKET
                        { $$ = $2.head; }
                |       OBRACKET No_પ્રતીક_Or_Action_List COMMA Multi_ચાવી_Sym_List CBRACKET
                        {
                            /* Prepend n times NoSymbol */
                            સંરચના {પદાવલિ_Def *head; પદાવલિ_Def *last;} list = {
                                .head = $4.head, .last = $4.last
                            };
                            માટે (uint32_t k = 0; k < $2; k++) {
                                પદાવલિ_Def* અચલ syms =
                                    પદાવલિ_Create_ચાવી_Sym_List(XKB_ચાવી_No_પ્રતીક);
                                જો (!syms) {
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
                |       OBRACKET No_પ્રતીક_Or_Action_List COMMA MultiActionList CBRACKET
                        {
                            /* Prepend n times NoAction() */
                            સંરચના {પદાવલિ_Def *head; પદાવલિ_Def *last;} list = {
                                .head = $4.head, .last = $4.last
                            };
                            માટે (uint32_t k = 0; k < $2; k++) {
                                પદાવલિ_Def* અચલ acts = પદાવલિ_Create_Action_List(ખાલી);
                                જો (!acts) {
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
                |       OBRACKET No_પ્રતીક_Or_Action_List CBRACKET
                        { $$ = પદાવલિ_Empty_List(); }
                ;

/* A list of `{}`, which remains ambiguous until reaching a keysym or action list */
No_પ્રતીક_Or_Action_List
                :       No_પ્રતીક_Or_Action_List COMMA OBRACE CBRACE
                        { $$ = $1 + 1; }
                |       OBRACE CBRACE
                        { $$ = 1; }
                |       { $$ = 0; }
                ;

GroupCompatDecl :       GROUP Integer EQUALS પદાવલિ_2 SEMI
                        { $$ = GroupCompatCreate($2, $4); }
                ;

ModMapDecl      :       MODIFIER_MAP પદાવલિ_2 OBRACE ચાવી_Or_ચાવી_Sym_List CBRACE SEMI
                        {
                            જો (param->config.format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
                                $2->common.પ્રકાર != STMT_પદાવલિ_IDENT) {
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

ચાવી_Or_ચાવી_Sym_List :       ચાવી_Or_ચાવી_Sym_List COMMA ચાવી_Or_ચાવી_Sym
                        { $$.head = $1.head; $$.last->common.next = &$3->common; $$.last = $3; }
                |       ચાવી_Or_ચાવી_Sym
                        { $$.head = $$.last = $1; }
                ;

ચાવી_Or_ચાવી_Sym     :       KEYNAME
                        { $$ = પદાવલિ_Create_ચાવી_Name($1); }
                |       ચાવી_Sym_2
                        { $$ = પદાવલિ_Create_ચાવી_Sym($1); }
                ;

LedMapDecl:             INDICATOR String OBRACE VarDeclList CBRACE SEMI
                        { $$ = LedMapCreate($2, $4.head); }
                ;

LedNameDecl:            INDICATOR Integer EQUALS પદાવલિ_2 SEMI
                        { $$ = LedNameCreate($2, $4, false); }
                |       VIRTUAL INDICATOR Integer EQUALS પદાવલિ_2 SEMI
                        { $$ = LedNameCreate($3, $5, true); }
                ;

UnknownDecl     :       IDENT Terminal EQUALS પદાવલિ_2 SEMI
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
                        { $$ = ખાલી; }
                |       SHAPE String OBRACE CoordList CBRACE SEMI
                        { (શૂન્ય) $4; $$ = ખાલી; }
                ;

SectionDecl     :       SECTION String OBRACE SectionBody CBRACE SEMI
                        { $$ = ખાલી; }
                ;

SectionBody     :       SectionBody SectionBodyItem     { $$ = ખાલી;}
                |       SectionBodyItem                 { $$ = ખાલી; }
                ;

SectionBodyItem :       ROW OBRACE RowBody CBRACE SEMI
                        { $$ = ખાલી; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ખાલી; }
                |       DoodadDecl
                        { $$ = ખાલી; }
                |       LedMapDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ખાલી; }
                |       OverlayDecl
                        { $$ = ખાલી; }
                ;

RowBody         :       RowBody RowBodyItem     { $$ = ખાલી;}
                |       RowBodyItem             { $$ = ખાલી; }
                ;

RowBodyItem     :       ચાવીઓ_2 OBRACE ચાવીઓ_3 CBRACE SEMI { $$ = ખાલી; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ખાલી; }
                ;

ચાવીઓ_3            :       ચાવીઓ_3 COMMA ચાવી_4          { $$ = ખાલી; }
                |       ચાવી_4                     { $$ = ખાલી; }
                ;

ચાવી_4             :       KEYNAME
                        { $$ = ખાલી; }
                |       OBRACE પદાવલિ_List_2 CBRACE
                        { FreeStmt((ParseCommon *) $2.head); $$ = ખાલી; }
                ;

OverlayDecl     :       OVERLAY String OBRACE Overlay_ચાવી_List CBRACE SEMI
                        { $$ = ખાલી; }
                ;

Overlay_ચાવી_List  :       Overlay_ચાવી_List COMMA Overlay_ચાવી { $$ = ખાલી; }
                |       Overlay_ચાવી                      { $$ = ખાલી; }
                ;

Overlay_ચાવી      :       KEYNAME EQUALS KEYNAME          { $$ = ખાલી; }
                ;

OutlineList     :       OutlineList COMMA OutlineInList
                        { $$ = ખાલી;}
                |       OutlineInList
                        { $$ = ખાલી; }
                ;

OutlineInList   :       OBRACE CoordList CBRACE
                        { (શૂન્ય) $2; $$ = ખાલી; }
                |       Ident EQUALS OBRACE CoordList CBRACE
                        { (શૂન્ય) $4; $$ = ખાલી; }
                |       Ident EQUALS પદાવલિ_2
                        { FreeStmt((ParseCommon *) $3); $$ = ખાલી; }
                ;

CoordList       :       CoordList COMMA Coord
                        { (શૂન્ય) $1; (શૂન્ય) $3; $$ = ખાલી; }
                |       Coord
                        { (શૂન્ય) $1; $$ = ખાલી; }
                ;

Coord           :       OBRACKET Signed_સંખ્યા COMMA Signed_સંખ્યા CBRACKET
                        { $$ = ખાલી; }
                ;

DoodadDecl      :       DoodadType String OBRACE VarDeclList CBRACE SEMI
                        { FreeStmt((ParseCommon *) $4.head); $$ = ખાલી; }
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
                |       ચાવી_3
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

પદાવલિ_List_2        :       પદાવલિ_List_2 COMMA પદાવલિ_2
                        {
                            જો ($3) {
                                જો ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$3->common;
                                    $$.last = $3;
                                } નહીંતર {
                                    $$.head = $$.last = $3;
                                }
                            }
                        }
                |       પદાવલિ_2
                        { $$.head = $$.last = $1; }
                |       { $$.head = $$.last = ખાલી; }
                ;

પદાવલિ_2            :       પદાવલિ_2 DIVIDE પદાવલિ_2
                        { $$ = પદાવલિ_Create_દ્વિઅંકી(STMT_પદાવલિ_DIVIDE, $1, $3); }
                |       પદાવલિ_2 PLUS પદાવલિ_2
                        { $$ = પદાવલિ_Create_દ્વિઅંકી(STMT_પદાવલિ_ADD, $1, $3); }
                |       પદાવલિ_2 MINUS પદાવલિ_2
                        { $$ = પદાવલિ_Create_દ્વિઅંકી(STMT_પદાવલિ_SUBTRACT, $1, $3); }
                |       પદાવલિ_2 TIMES પદાવલિ_2
                        { $$ = પદાવલિ_Create_દ્વિઅંકી(STMT_પદાવલિ_MULTIPLY, $1, $3); }
                |       Lhs EQUALS પદાવલિ_2
                        { $$ = પદાવલિ_Create_દ્વિઅંકી(STMT_પદાવલિ_ASSIGN, $1, $3); }
                |       Term
                        { $$ = $1; }
                ;

Term            :       MINUS Term
                        { $$ = પદાવલિ_Create_Unary(STMT_પદાવલિ_NEGATE, $2); }
                |       PLUS Term
                        { $$ = પદાવલિ_Create_Unary(STMT_પદાવલિ_UNARY_PLUS, $2); }
                |       EXCLAM Term
                        { $$ = પદાવલિ_Create_Unary(STMT_પદાવલિ_NOT, $2); }
                |       INVERT Term
                        { $$ = પદાવલિ_Create_Unary(STMT_પદાવલિ_INVERT, $2); }
                |       Lhs
                        { $$ = $1; }
                |       FieldSpec OPAREN પદાવલિ_List_2 CPAREN %prec OPAREN
                        { $$ = પદાવલિ_Create_Action($1, $3.head); }
                |       Actions
                        { $$ = $1; }
                |       Terminal
                        { $$ = $1; }
                |       OPAREN પદાવલિ_2 CPAREN
                        { $$ = $2; }
                ;

MultiActionList :       MultiActionList COMMA Action
                        {
                            પદાવલિ_Def *પદાવલિ = પદાવલિ_Create_Action_List($3);
                            $$ = $1;
                            $$.last->common.next = &પદાવલિ->common; $$.last = પદાવલિ;
                        }
                |       MultiActionList COMMA Actions
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       Action
                        { $$.head = $$.last = પદાવલિ_Create_Action_List($1); }
                |       NonEmptyActions
                        { $$.head = $$.last = $1; }
                ;

ActionList      :       ActionList COMMA Action
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       Action
                        { $$.head = $$.last = $1; }
                ;

NonEmptyActions :       OBRACE ActionList CBRACE
                        { $$ = પદાવલિ_Create_Action_List($2.head); }
                ;

Actions         :       NonEmptyActions
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = પદાવલિ_Create_Action_List(ખાલી); }
                ;

Action          :       FieldSpec OPAREN પદાવલિ_List_2 CPAREN
                        { $$ = પદાવલિ_Create_Action($1, $3.head); }
                ;

Lhs             :       FieldSpec
                        { $$ = પદાવલિ_Create_Ident($1); }
                |       FieldSpec DOT FieldSpec
                        { $$ = પદાવલિ_Create_Field_Ref($1, $3); }
                |       FieldSpec OBRACKET પદાવલિ_2 CBRACKET
                        { $$ = પદાવલિ_Create_Array_Ref(XKB_ATOM_NONE, $1, $3); }
                |       FieldSpec DOT FieldSpec OBRACKET પદાવલિ_2 CBRACKET
                        { $$ = પદાવલિ_Create_Array_Ref($1, $3, $5); }
                ;

OptTerminal     :       Terminal
                        { $$ = $1; }
                |       { $$ = ખાલી; }
                ;

Terminal        :       String
                        { $$ = પદાવલિ_Create_String($1); }
                |       Integer
                        { $$ = પદાવલિ_Create_Integer($1); }
                |       Float
                        { $$ = પદાવલિ_Create_Float(/* Discard $1 */); }
                |       KEYNAME
                        { $$ = પદાવલિ_Create_ચાવી_Name($1); }
                ;

Multi_ચાવી_Sym_List :       Multi_ચાવી_Sym_List COMMA ચાવી_Sym_Lit
                        {
                            પદાવલિ_Def *પદાવલિ = પદાવલિ_Create_ચાવી_Sym_List($3);
                            $$ = $1;
                            $$.last->common.next = &પદાવલિ->common; $$.last = પદાવલિ;
                        }
                |       Multi_ચાવી_Sym_List COMMA ચાવી_Syms
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       ચાવી_Sym_Lit
                        { $$.head = $$.last = પદાવલિ_Create_ચાવી_Sym_List($1); }
                |       Non_Empty_ચાવી_Syms
                        { $$.head = $$.last = $1; }
                ;

ચાવી_Sym_List      :       ચાવી_Sym_List COMMA ચાવી_Sym_Lit
                        { $$ = પદાવલિ_Append_ચાવી_Sym_List($1, $3); }
                |       ચાવી_Sym_List COMMA STRING
                        {
                            $$ = પદાવલિ_ચાવી_Sym_List_Append_String(param->scanner, $1, $3);
                            મુક્ત($3);
                            જો (!$$)
                                YYERROR;
                        }
                |       ચાવી_Sym_Lit
                        {
                            $$ = પદાવલિ_Create_ચાવી_Sym_List($1);
                            જો (!$$)
                                YYERROR;
                        }
                |       STRING
                        {
                            $$ = પદાવલિ_Create_ચાવી_Sym_List(XKB_ચાવી_No_પ્રતીક);
                            જો (!$$)
                                YYERROR;
                            $$ = પદાવલિ_ચાવી_Sym_List_Append_String(param->scanner, $$, $1);
                            મુક્ત($1);
                            જો (!$$)
                                YYERROR;
                        }
                ;

Non_Empty_ચાવી_Syms :       OBRACE ચાવી_Sym_List CBRACE
                        { $$ = $2; }
                |       STRING
                        {
                            $$ = પદાવલિ_Create_ચાવી_Sym_List(XKB_ચાવી_No_પ્રતીક);
                            જો (!$$)
                                YYERROR;
                            $$ = પદાવલિ_ચાવી_Sym_List_Append_String(param->scanner, $$, $1);
                            મુક્ત($1);
                            જો (!$$)
                                YYERROR;
                        }
                ;

ચાવી_Syms         :       Non_Empty_ચાવી_Syms
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = પદાવલિ_Create_ચાવી_Sym_List(XKB_ચાવી_No_પ્રતીક); }
                ;

ચાવી_Sym_2          :       ચાવી_Sym_Lit
                        { $$ = $1; }
                |       STRING
                        {
                            $$ = KeysymParseString(param->scanner, $1);
                            મુક્ત($1);
                            જો ($$ == XKB_ચાવી_No_પ્રતીક)
                                YYERROR;
                        }
                ;

ચાવી_Sym_Lit       :       IDENT
                        {
                            જો (!resolve_keysym(param, $1, &$$)) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (unsigned પૂર્ણાંક) $1.len, $1.start
                                );
                                $$ = XKB_ચાવી_No_પ્રતીક;
                            }
                        }
                        /* Handle keysym that is also a keyword  */
                |       SECTION { $$ = XKB_ચાવી_section; }
                |       DECIMAL_DIGIT
                        {
                            /*
                             * Special case for digits 0..9:
                             * map to XKB_KEY_0 .. XKB_KEY_9, consistent with
                             * other keysym names: <name> → XKB_KEY_<name>.
                             */
                            $$ = XKB_ચાવી_0 + (xkb_keysym_t) $1;
                        }
                |       INTEGER
                        {
                            જો ($1 < XKB_KEYSYM_MIN) {
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
                                $$ = XKB_ચાવી_No_પ્રતીક;
                            }
                            /*
                             * Integers 0..9 are handled with DECIMAL_DIGIT if
                             * they were formatted as single characters '0'..'9'.
                             * Otherwise they are handled here as raw keysyms
                             * values. E.g. `01` and `0x1` are interpreted as
                             * the keysym 0x0001, while `1` is interpreted as
                             * XKB_KEY_1.
                             */
                            નહીંતર {
                                /* Any other numeric value */
                                જો ($1 <= XKB_KEYSYM_MAX) {
                                    /*
                                     * Valid keysym
                                     * No normalization is performed and value
                                     * is used as is.
                                     */
                                    $$ = (xkb_keysym_t) $1;
                                    check_deprecated_keysyms(
                                        parser_warn, param, param->ctx,
                                        $$, ખાલી, $$, "%#06"PRIx32, "");
                                } નહીંતર {
                                    /* Invalid keysym */
                                    parser_warn(
                                        param, XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                        "unrecognized keysym \"%#06"PRIx64"\" "
                                        "(%"PRId64")", $1, $1
                                    );
                                    $$ = XKB_ચાવી_No_પ્રતીક;
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

Signed_સંખ્યા    :       MINUS સંખ્યા    { $$ = -$2; }
                |       સંખ્યા          { $$ = $1; }
                ;

સંખ્યા          :       FLOAT         { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                |       INTEGER       { $$ = $1; }
                ;

Float           :       FLOAT   { $$ = 0; }
                ;

Integer         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

ચાવી_Code_2         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

Ident           :       IDENT   { $$ = xkb_atom_intern(param->ctx, $1.start, $1.len); }
                |       DEFAULT { $$ = xkb_atom_intern_literal(param->ctx, "default"); }
                ;

String          :       STRING  { $$ = xkb_atom_intern(param->ctx, $1, strlen($1)); મુક્ત($1); }
                ;

OptMapName      :       MapName { $$ = $1; }
                |               { $$ = ખાલી; }
                ;

MapName         :       STRING  { $$ = $1; }
                ;

%%

/* Parse a specific section */
XkbFile *
parse(સંરચના xkb_context *ctx, અચલ સંરચના parser_keymap_config *config,
      સંરચના scanner *scanner, અચલ અક્ષર *map)
{
    પૂર્ણાંક ret;
    XkbFile *first = ખાલી;
    સંરચના parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = ખાલી,
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

    જ્યારે ((ret = yyparse(&param)) == 0 && param.more_maps) {
        જો (map) {
            જો (streq_not_null(map, param.rtrn->name))
                પાછા param.rtrn;
            નહીંતર
                FreeXkbFile(param.rtrn);
        }
        નહીંતર {
            જો (param.rtrn->flags & MAP_IS_DEFAULT) {
                FreeXkbFile(first);
                પાછા param.rtrn;
            }
            નહીંતર જો (!first) {
                first = param.rtrn;
            }
            નહીંતર {
                FreeXkbFile(param.rtrn);
            }
        }
        param.rtrn = ખાલી;
    }

    જો (ret != 0) {
        /* Some error happend; clear the Xkbfiles parsed so far */
        FreeXkbFile(first);
        FreeXkbFile(param.rtrn);
        પાછા ખાલી;
    }

    /*
     * Warn about implicit default section,
     * but only if not a keymap: multiple keymaps per file not supported
     */
    જો (first && first->file_type != FILE_TYPE_KEYMAP)
        log_vrb(ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_MISSING_DEFAULT_SECTION,
                "No section name in include statement, but \"%s\" contains several; "
                "Using first defined section, \"%s\"\n",
                scanner->file_name, safe_map_name(first->name));

    પાછા first;
}

/* Parse the next section */
bool
parse_next(સંરચના xkb_context *ctx,
           અચલ સંરચના parser_keymap_config *config,
           સંરચના scanner *scanner, XkbFile **xkb_file)
{
    પૂર્ણાંક ret;
    સંરચના parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = ખાલી,
        .config = *config,
        .more_maps = false,
    };

    જો ((ret = yyparse(&param)) == 0 && param.more_maps) {
        *xkb_file = param.rtrn;
        પાછા true;
    } નહીંતર {
        FreeXkbFile(param.rtrn);
        *xkb_file = ખાલી;
        પાછા (ret == 0);
    }
}
