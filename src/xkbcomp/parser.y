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
#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "scanner-utils.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp/ast.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp/xkbcomp-priv.h"
}

%{
#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>

#ಅನ್ತರ್ಭಾವ "keysym.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp/xkbcomp-priv.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp/parser-priv.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp/ast-build.h"

ಸಂರಚನಾ parser_param {
    ಸಂರಚನಾ xkb_context *ctx;
    ಸಂರಚನಾ scanner *scanner;
    XkbFile *rtrn;
    ಸಂರಚನಾ parser_keymap_config config;
    bool more_maps;
};

#define parser_log_with_code(param, level, verbosity, log_ಸಂದೇಶ_id, fmt, ...)   \
    scanner_log_with_code((param)->scanner, level, verbosity, log_ಸಂದೇಶ_id, fmt,\
                          ##__VA_ARGS__)

#define parser_err(param, error_id, fmt, ...) \
    scanner_err((param)->scanner, error_id, fmt, ##__VA_ARGS__)

#define parser_warn(param, warning_id, fmt, ...) \
    scanner_warn((param)->scanner, warning_id, fmt, ##__VA_ARGS__)

#define parser_vrb(param, verbosity, warning_id, fmt, ...) \
    scanner_vrb((param)->scanner, verbosity, warning_id, fmt, ##__VA_ARGS__)

ಸ್ಥಿರ ಶೂನ್ಯ
_xkbcommon_error(ಸಂರಚನಾ parser_param *param, ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ)
{
    parser_err(param, XKB_ERROR_INVALID_XKB_SYNTAX, "%s", ಸಂದೇಶ);
}

ಸ್ಥಿರ bool
resolve_keysym(ಸಂರಚನಾ parser_param *param, ಸಂರಚನಾ sval name, xkb_keysym_t *sym_rtrn)
{
    xkb_keysym_t sym;

    ಯದಿ (isvaleq(name, SVAL_LIT("any")) || isvaleq(name, SVAL_LIT("nosymbol"))) {
        *sym_rtrn = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
        ಪ್ರತ್ಯಾಗಮ true;
    }

    ಯದಿ (isvaleq(name, SVAL_LIT("none")) || isvaleq(name, SVAL_LIT("voidsymbol"))) {
        *sym_rtrn = XKB_ಕೀಲಿ_Void_ಚಿಹ್ನೆ;
        ಪ್ರತ್ಯಾಗಮ true;
    }

    /* xkb_keysym_from_name needs a C string. */
    ವರ್ಣ buf[XKB_KEYSYM_NAME_MAX_SIZE];
    ಯದಿ (name.len >= ಪರಿಮಾಣ(buf)) {
        ಪ್ರತ್ಯಾಗಮ false;
    }
    memcpy(buf, name.start, name.len);
    buf[name.len] = '\0';

    sym = xkb_keysym_from_name(buf, XKB_KEYSYM_NO_FLAGS);
    ಯದಿ (sym != XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
        *sym_rtrn = sym;
        check_deprecated_keysyms(parser_warn, param, param->ctx,
                                 sym, buf, buf, "%s", "");
        ಪ್ರತ್ಯಾಗಮ true;
    }

    ಪ್ರತ್ಯಾಗಮ false;
}

#define param_scanner param->scanner
%}

%define api.pure
%lex-param      { ಸಂರಚನಾ scanner *param_scanner }
%parse-param    { ಸಂರಚನಾ parser_param *param }

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
        ಕೀಲಿ_3             24 "key"
        ALIAS           25 "alias"
        GROUP           26 "group"
        MODIFIER_MAP    27 "modifier_map"
        INDICATOR       28 "indicator"
        SHAPE           29 "shape"
        ಕೀಲಿಗಳು_2            30 "keys"
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
        ALPHANUMERIC_ಕೀಲಿಗಳು       73 "alphanumeric_keys"
        MODIFIER_ಕೀಲಿಗಳು           74 "modifier_keys"
        KEYPAD_ಕೀಲಿಗಳು             75 "keypad_keys"
        FUNCTION_ಕೀಲಿಗಳು           76 "function_keys"
        ALTERNATE_GROUP         77 "alternate_group"
        DEPRECATED              78 "deprecated"

%right  EQUALS
%left   PLUS MINUS
%left   TIMES DIVIDE
%left   EXCLAM INVERT
%left   OPAREN

%start  XkbFile

%ಸಙ್ಘ  {
        int64_t          ಅಂಕೆ;
        ಗಣನಾ xkb_file_type file_type;
        ವರ್ಣ            *str;
        ಸಂರಚನಾ sval     sval;
        xkb_atom_t      atom;
        ಗಣನಾ merge_mode merge;
        ಗಣನಾ xkb_map_flags mapFlags;
        xkb_keysym_t    keysym;
        ParseCommon     *any;
        ಸಂರಚನಾ { ParseCommon *head; ParseCommon *last; } anyList;
        uint32_t        no_ಚಿಹ್ನೆ_Or_Action_List;
        ಅಭಿವ್ಯಕ್ತಿ_Def         *ಅಭಿವ್ಯಕ್ತಿ;
        ಸಂರಚನಾ { ಅಭಿವ್ಯಕ್ತಿ_Def *head; ಅಭಿವ್ಯಕ್ತಿ_Def *last; } ಅಭಿವ್ಯಕ್ತಿ_List;
        VarDef          *var;
        ಸಂರಚನಾ { VarDef *head; VarDef *last; } varList;
        VModDef         *vmod;
        ಸಂರಚನಾ { VModDef *head; VModDef *last; } vmodList;
        InterpDef       *interp;
        ಕೀಲಿ_Type_Def      *ಕೀಲಿ_Type;
        SymbolsDef      *syms;
        ModMapDef       *modMask;
        GroupCompatDef  *groupCompat;
        LedMapDef       *ledMap;
        LedNameDef      *ledName;
        KeycodeDef      *ಕೀಲಿ_Code;
        ಕೀಲಿ_Alias_Def     *ಕೀಲಿ_Alias;
        UnknownStatement *unknown;
        ಶೂನ್ಯ            *geom;
        XkbFile         *file;
        ಸಂರಚನಾ { XkbFile *head; XkbFile *last; } fileList;
}

%ಪ್ರಕಾರ <ಅಂಕೆ>     DECIMAL_DIGIT INTEGER FLOAT
%ಪ್ರಕಾರ <str>     STRING
%ಪ್ರಕಾರ <sval>    IDENT
%ಪ್ರಕಾರ <atom>    KEYNAME
%ಪ್ರಕಾರ <ಅಂಕೆ>     ಕೀಲಿ_Code_2 ಸಂಖ್ಯೆ Integer Float Signed_ಸಂಖ್ಯೆ DoodadType
%ಪ್ರಕಾರ <merge>   MergeMode OptMergeMode
%ಪ್ರಕಾರ <file_type> XkbCompositeType FileType
%ಪ್ರಕಾರ <mapFlags> Flag Flags OptFlags
%ಪ್ರಕಾರ <str>     MapName OptMapName
%ಪ್ರಕಾರ <atom>    FieldSpec Ident Element String
%ಪ್ರಕಾರ <keysym>  ಕೀಲಿ_Sym_2 ಕೀಲಿ_Sym_Lit
%ಪ್ರಕಾರ <no_ಚಿಹ್ನೆ_Or_Action_List> No_ಚಿಹ್ನೆ_Or_Action_List
%ಪ್ರಕಾರ <any>     Decl
%ಪ್ರಕಾರ <anyList> DeclList
%ಪ್ರಕಾರ <ಅಭಿವ್ಯಕ್ತಿ>    ಅಭಿವ್ಯಕ್ತಿ_2 Term Lhs Terminal OptTerminal Coord CoordList
%ಪ್ರಕಾರ <ಅಭಿವ್ಯಕ್ತಿ>    Multi_ಕೀಲಿ_Sym_Or_Action_List NonEmptyActions Actions Action
%ಪ್ರಕಾರ <ಅಭಿವ್ಯಕ್ತಿ>    ಕೀಲಿ_Syms Non_Empty_ಕೀಲಿ_Syms ಕೀಲಿ_Sym_List ಕೀಲಿ_Or_ಕೀಲಿ_Sym
%ಪ್ರಕಾರ <ಅಭಿವ್ಯಕ್ತಿ_List> ಅಭಿವ್ಯಕ್ತಿ_List_2 ಕೀಲಿ_Or_ಕೀಲಿ_Sym_List
%ಪ್ರಕಾರ <ಅಭಿವ್ಯಕ್ತಿ_List> ActionList MultiActionList Multi_ಕೀಲಿ_Sym_List
%ಪ್ರಕಾರ <var>     VarDecl SymbolsVarDecl
%ಪ್ರಕಾರ <varList> VarDeclList SymbolsBody OptSymbolsBody
%ಪ್ರಕಾರ <vmod>    VModDef
%ಪ್ರಕಾರ <vmodList> VModDefList VModDecl
%ಪ್ರಕಾರ <interp>  InterpretDecl InterpretMatch
%ಪ್ರಕಾರ <ಕೀಲಿ_Type> ಕೀಲಿ_Type_Decl
%ಪ್ರಕಾರ <syms>    SymbolsDecl
%ಪ್ರಕಾರ <modMask> ModMapDecl
%ಪ್ರಕಾರ <groupCompat> GroupCompatDecl
%ಪ್ರಕಾರ <ledMap>  LedMapDecl
%ಪ್ರಕಾರ <ledName> LedNameDecl
%ಪ್ರಕಾರ <ಕೀಲಿ_Code> ಕೀಲಿ_Name_Decl
%ಪ್ರಕಾರ <ಕೀಲಿ_Alias> ಕೀಲಿ_Alias_Decl
%ಪ್ರಕಾರ <geom>    ShapeDecl SectionDecl SectionBody SectionBodyItem RowBody RowBodyItem
%ಪ್ರಕಾರ <geom>    ಕೀಲಿಗಳು_3 ಕೀಲಿ_4 OverlayDecl Overlay_ಕೀಲಿ_List Overlay_ಕೀಲಿ OutlineList OutlineInList
%ಪ್ರಕಾರ <geom>    DoodadDecl
%ಪ್ರಕಾರ <unknown> UnknownDecl UnknownCompoundStatementDecl
%ಪ್ರಕಾರ <file>    XkbFile XkbMapConfig
%ಪ್ರಕಾರ <fileList> XkbMapConfigList
%ಪ್ರಕಾರ <file>    XkbCompositeMap

%destructor { FreeStmt((ParseCommon *) $$); }
    <any> <ಅಭಿವ್ಯಕ್ತಿ> <var> <vmod> <interp> <ಕೀಲಿ_Type> <syms> <modMask> <groupCompat>
    <ledMap> <ledName> <ಕೀಲಿ_Code> <ಕೀಲಿ_Alias>
%destructor { FreeStmt((ParseCommon *) $$.head); }
    <anyList> <ಅಭಿವ್ಯಕ್ತಿ_List> <varList> <vmodList>
/* The destructor also runs on the start symbol when the parser *succeeds*.
 * The `if` here catches this case. */
%destructor { ಯದಿ (!param->rtrn) FreeXkbFile($$); } <file>
%destructor { FreeXkbFile($$.head); } <fileList>
%destructor { ಮುಕ್ತ($$); } <str>

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
                        { $$ = param->rtrn = $1; param->more_maps = !!param->rtrn; (ಶೂನ್ಯ) yynerrs; }
                |       XkbMapConfig
                        { $$ = param->rtrn = $1; param->more_maps = !!param->rtrn; YYACCEPT; }
                |       END_OF_FILE
                        { $$ = param->rtrn = ರಿಕ್ತ; param->more_maps = false; }
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
                            ಯದಿ ($2) {
                                ಯದಿ ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$2->common;
                                    $$.last = $2;
                                } ಅನ್ಯಥಾ {
                                    $$.head = $$.last = $2;
                                }
                            }
                        }
                |       { $$.head = $$.last = ರಿಕ್ತ; }
                ;

XkbMapConfig    :       OptFlags FileType OptMapName OBRACE
                            DeclList
                        CBRACE SEMI
                        {
                            ಯದಿ ($1 & MAP_IS_DEPRECATED) {
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
                |       ALPHANUMERIC_ಕೀಲಿಗಳು       { $$ = MAP_HAS_ALPHANUMERIC; }
                |       MODIFIER_ಕೀಲಿಗಳು           { $$ = MAP_HAS_MODIFIER; }
                |       KEYPAD_ಕೀಲಿಗಳು             { $$ = MAP_HAS_KEYPAD; }
                |       FUNCTION_ಕೀಲಿಗಳು           { $$ = MAP_HAS_FN; }
                |       ALTERNATE_GROUP         { $$ = MAP_IS_ALTGR; }
                |       DEPRECATED              { $$ = MAP_IS_DEPRECATED; }
                |       IDENT
                        {
                            ಧ್ರುವ bool error = (param->config.strict & PARSER_NO_UNKNOWN_SECTION_FLAGS);
                            parser_log_with_code(
                                param, (error ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
                                XKB_LOG_VERBOSITY_MINIMAL,
                                XKB_ERROR_UNKNOWN_SECTION_FLAG,
                                "Unknown section flag \"%.*s\"%s",
                                (unsigned)$1.len, $1.start,
                                (error ? "" : "; ignored")
                            );
                            ಯದಿ (error)
                                YYABORT;
                            $$ = 0;
                        }
                ;

DeclList        :       DeclList Decl
                        {
                            ಯದಿ ($2) {
                                ಯದಿ ($1.head) {
                                    $$.head = $1.head; $1.last->next = $2; $$.last = $2;
                                } ಅನ್ಯಥಾ {
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
                            ಪ್ರತಿ (VModDef *vmod = $3.head; vmod; vmod = (VModDef *) vmod->common.next)
                                vmod->merge = $2;
                            ಯದಿ ($1.head) {
                                $$.head = $1.head; $1.last->next = &$3.head->common; $$.last = &$3.last->common;
                            } ಅನ್ಯಥಾ {
                                $$.head = &$3.head->common; $$.last = &$3.last->common;
                            }
                        }
                |       { $$.head = $$.last = ರಿಕ್ತ; }
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
                |       OptMergeMode ಕೀಲಿ_Name_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode ಕೀಲಿ_Alias_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode ಕೀಲಿ_Type_Decl
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
                |       OptMergeMode ShapeDecl          { $$ = ರಿಕ್ತ; }
                |       OptMergeMode SectionDecl        { $$ = ರಿಕ್ತ; }
                |       OptMergeMode DoodadDecl         { $$ = ರಿಕ್ತ; }
                |       OptMergeMode UnknownDecl
                            { $$ = (ParseCommon *) $2; }
                |       OptMergeMode UnknownCompoundStatementDecl
                            { $$ = (ParseCommon *) $2; }
                |       MergeMode STRING
                        {
                            $$ = (ParseCommon *) IncludeCreate(param->ctx, $2, $1);
                            ಮುಕ್ತ($2);
                        }
                ;

VarDecl         :       Lhs EQUALS ಅಭಿವ್ಯಕ್ತಿ_2 SEMI
                        { $$ = VarCreate($1, $3); }
                |       Ident SEMI
                        { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident SEMI
                        { $$ = BoolVarCreate($2, false); }
                ;

ಕೀಲಿ_Name_Decl     :       KEYNAME EQUALS ಕೀಲಿ_Code_2 SEMI
                        { $$ = KeycodeCreate($1, $3); }
                ;

ಕೀಲಿ_Alias_Decl    :       ALIAS KEYNAME EQUALS KEYNAME SEMI
                        { $$ = ಕೀಲಿ_Alias_Create($2, $4); }
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
                        { $$ = VModCreate($1, ರಿಕ್ತ); }
                |       Ident EQUALS ಅಭಿವ್ಯಕ್ತಿ_2
                        { $$ = VModCreate($1, $3); }
                ;

InterpretDecl   :       INTERPRET InterpretMatch OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $2->def = $4.head; $$ = $2; }
                ;

InterpretMatch  :       ಕೀಲಿ_Sym_2 PLUS ಅಭಿವ್ಯಕ್ತಿ_2
                        { $$ = InterpCreate($1, $3); }
                |       ಕೀಲಿ_Sym_2
                        { $$ = InterpCreate($1, ರಿಕ್ತ); }
                ;

VarDeclList     :       VarDeclList VarDecl
                        {
                            ಯದಿ ($2) {
                                ಯದಿ ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$2->common;
                                    $$.last = $2;
                                } ಅನ್ಯಥಾ {
                                    $$.head = $$.last = $2;
                                }
                            }
                        }
                |       { $$.head = $$.last = ರಿಕ್ತ; }
                ;

ಕೀಲಿ_Type_Decl     :       TYPE String OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $$ = ಕೀಲಿ_Type_Create($2, $4.head); }
                ;

SymbolsDecl     :       ಕೀಲಿ_3 KEYNAME OBRACE
                            OptSymbolsBody
                        CBRACE SEMI
                        { $$ = SymbolsCreate($2, $4.head); }
                ;

OptSymbolsBody  :       SymbolsBody { $$ = $1; }
                |                   { $$.head = $$.last = ರಿಕ್ತ; }
                ;

SymbolsBody     :       SymbolsBody COMMA SymbolsVarDecl
                        { $$.head = $1.head; $$.last->common.next = &$3->common; $$.last = $3; }
                |       SymbolsVarDecl
                        { $$.head = $$.last = $1; }
                ;

SymbolsVarDecl  :       Lhs EQUALS ಅಭಿವ್ಯಕ್ತಿ_2         { $$ = VarCreate($1, $3); }
                |       Lhs EQUALS Multi_ಕೀಲಿ_Sym_Or_Action_List { $$ = VarCreate($1, $3); }
                |       Ident                   { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident            { $$ = BoolVarCreate($2, false); }
                |       Multi_ಕೀಲಿ_Sym_Or_Action_List { $$ = VarCreate(ರಿಕ್ತ, $1); }
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
Multi_ಕೀಲಿ_Sym_Or_Action_List
                :       OBRACKET Multi_ಕೀಲಿ_Sym_List CBRACKET
                        { $$ = $2.head; }
                |       OBRACKET No_ಚಿಹ್ನೆ_Or_Action_List COMMA Multi_ಕೀಲಿ_Sym_List CBRACKET
                        {
                            /* Prepend n times NoSymbol */
                            ಸಂರಚನಾ {ಅಭಿವ್ಯಕ್ತಿ_Def *head; ಅಭಿವ್ಯಕ್ತಿ_Def *last;} list = {
                                .head = $4.head, .last = $4.last
                            };
                            ಪ್ರತಿ (uint32_t k = 0; k < $2; k++) {
                                ಅಭಿವ್ಯಕ್ತಿ_Def* ಧ್ರುವ syms =
                                    ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List(XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ);
                                ಯದಿ (!syms) {
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
                |       OBRACKET No_ಚಿಹ್ನೆ_Or_Action_List COMMA MultiActionList CBRACKET
                        {
                            /* Prepend n times NoAction() */
                            ಸಂರಚನಾ {ಅಭಿವ್ಯಕ್ತಿ_Def *head; ಅಭಿವ್ಯಕ್ತಿ_Def *last;} list = {
                                .head = $4.head, .last = $4.last
                            };
                            ಪ್ರತಿ (uint32_t k = 0; k < $2; k++) {
                                ಅಭಿವ್ಯಕ್ತಿ_Def* ಧ್ರುವ acts = ಅಭಿವ್ಯಕ್ತಿ_Create_Action_List(ರಿಕ್ತ);
                                ಯದಿ (!acts) {
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
                |       OBRACKET No_ಚಿಹ್ನೆ_Or_Action_List CBRACKET
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Empty_List(); }
                ;

/* A list of `{}`, which remains ambiguous until reaching a keysym or action list */
No_ಚಿಹ್ನೆ_Or_Action_List
                :       No_ಚಿಹ್ನೆ_Or_Action_List COMMA OBRACE CBRACE
                        { $$ = $1 + 1; }
                |       OBRACE CBRACE
                        { $$ = 1; }
                |       { $$ = 0; }
                ;

GroupCompatDecl :       GROUP Integer EQUALS ಅಭಿವ್ಯಕ್ತಿ_2 SEMI
                        { $$ = GroupCompatCreate($2, $4); }
                ;

ModMapDecl      :       MODIFIER_MAP ಅಭಿವ್ಯಕ್ತಿ_2 OBRACE ಕೀಲಿ_Or_ಕೀಲಿ_Sym_List CBRACE SEMI
                        {
                            ಯದಿ (param->config.format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
                                $2->common.ಪ್ರಕಾರ != STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT) {
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

ಕೀಲಿ_Or_ಕೀಲಿ_Sym_List :       ಕೀಲಿ_Or_ಕೀಲಿ_Sym_List COMMA ಕೀಲಿ_Or_ಕೀಲಿ_Sym
                        { $$.head = $1.head; $$.last->common.next = &$3->common; $$.last = $3; }
                |       ಕೀಲಿ_Or_ಕೀಲಿ_Sym
                        { $$.head = $$.last = $1; }
                ;

ಕೀಲಿ_Or_ಕೀಲಿ_Sym     :       KEYNAME
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Name($1); }
                |       ಕೀಲಿ_Sym_2
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym($1); }
                ;

LedMapDecl:             INDICATOR String OBRACE VarDeclList CBRACE SEMI
                        { $$ = LedMapCreate($2, $4.head); }
                ;

LedNameDecl:            INDICATOR Integer EQUALS ಅಭಿವ್ಯಕ್ತಿ_2 SEMI
                        { $$ = LedNameCreate($2, $4, false); }
                |       VIRTUAL INDICATOR Integer EQUALS ಅಭಿವ್ಯಕ್ತಿ_2 SEMI
                        { $$ = LedNameCreate($3, $5, true); }
                ;

UnknownDecl     :       IDENT Terminal EQUALS ಅಭಿವ್ಯಕ್ತಿ_2 SEMI
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
                        { $$ = ರಿಕ್ತ; }
                |       SHAPE String OBRACE CoordList CBRACE SEMI
                        { (ಶೂನ್ಯ) $4; $$ = ರಿಕ್ತ; }
                ;

SectionDecl     :       SECTION String OBRACE SectionBody CBRACE SEMI
                        { $$ = ರಿಕ್ತ; }
                ;

SectionBody     :       SectionBody SectionBodyItem     { $$ = ರಿಕ್ತ;}
                |       SectionBodyItem                 { $$ = ರಿಕ್ತ; }
                ;

SectionBodyItem :       ROW OBRACE RowBody CBRACE SEMI
                        { $$ = ರಿಕ್ತ; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ರಿಕ್ತ; }
                |       DoodadDecl
                        { $$ = ರಿಕ್ತ; }
                |       LedMapDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ರಿಕ್ತ; }
                |       OverlayDecl
                        { $$ = ರಿಕ್ತ; }
                ;

RowBody         :       RowBody RowBodyItem     { $$ = ರಿಕ್ತ;}
                |       RowBodyItem             { $$ = ರಿಕ್ತ; }
                ;

RowBodyItem     :       ಕೀಲಿಗಳು_2 OBRACE ಕೀಲಿಗಳು_3 CBRACE SEMI { $$ = ರಿಕ್ತ; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = ರಿಕ್ತ; }
                ;

ಕೀಲಿಗಳು_3            :       ಕೀಲಿಗಳು_3 COMMA ಕೀಲಿ_4          { $$ = ರಿಕ್ತ; }
                |       ಕೀಲಿ_4                     { $$ = ರಿಕ್ತ; }
                ;

ಕೀಲಿ_4             :       KEYNAME
                        { $$ = ರಿಕ್ತ; }
                |       OBRACE ಅಭಿವ್ಯಕ್ತಿ_List_2 CBRACE
                        { FreeStmt((ParseCommon *) $2.head); $$ = ರಿಕ್ತ; }
                ;

OverlayDecl     :       OVERLAY String OBRACE Overlay_ಕೀಲಿ_List CBRACE SEMI
                        { $$ = ರಿಕ್ತ; }
                ;

Overlay_ಕೀಲಿ_List  :       Overlay_ಕೀಲಿ_List COMMA Overlay_ಕೀಲಿ { $$ = ರಿಕ್ತ; }
                |       Overlay_ಕೀಲಿ                      { $$ = ರಿಕ್ತ; }
                ;

Overlay_ಕೀಲಿ      :       KEYNAME EQUALS KEYNAME          { $$ = ರಿಕ್ತ; }
                ;

OutlineList     :       OutlineList COMMA OutlineInList
                        { $$ = ರಿಕ್ತ;}
                |       OutlineInList
                        { $$ = ರಿಕ್ತ; }
                ;

OutlineInList   :       OBRACE CoordList CBRACE
                        { (ಶೂನ್ಯ) $2; $$ = ರಿಕ್ತ; }
                |       Ident EQUALS OBRACE CoordList CBRACE
                        { (ಶೂನ್ಯ) $4; $$ = ರಿಕ್ತ; }
                |       Ident EQUALS ಅಭಿವ್ಯಕ್ತಿ_2
                        { FreeStmt((ParseCommon *) $3); $$ = ರಿಕ್ತ; }
                ;

CoordList       :       CoordList COMMA Coord
                        { (ಶೂನ್ಯ) $1; (ಶೂನ್ಯ) $3; $$ = ರಿಕ್ತ; }
                |       Coord
                        { (ಶೂನ್ಯ) $1; $$ = ರಿಕ್ತ; }
                ;

Coord           :       OBRACKET Signed_ಸಂಖ್ಯೆ COMMA Signed_ಸಂಖ್ಯೆ CBRACKET
                        { $$ = ರಿಕ್ತ; }
                ;

DoodadDecl      :       DoodadType String OBRACE VarDeclList CBRACE SEMI
                        { FreeStmt((ParseCommon *) $4.head); $$ = ರಿಕ್ತ; }
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
                |       ಕೀಲಿ_3
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

ಅಭಿವ್ಯಕ್ತಿ_List_2        :       ಅಭಿವ್ಯಕ್ತಿ_List_2 COMMA ಅಭಿವ್ಯಕ್ತಿ_2
                        {
                            ಯದಿ ($3) {
                                ಯದಿ ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$3->common;
                                    $$.last = $3;
                                } ಅನ್ಯಥಾ {
                                    $$.head = $$.last = $3;
                                }
                            }
                        }
                |       ಅಭಿವ್ಯಕ್ತಿ_2
                        { $$.head = $$.last = $1; }
                |       { $$.head = $$.last = ರಿಕ್ತ; }
                ;

ಅಭಿವ್ಯಕ್ತಿ_2            :       ಅಭಿವ್ಯಕ್ತಿ_2 DIVIDE ಅಭಿವ್ಯಕ್ತಿ_2
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ದ್ವಿಮಾನ(STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE, $1, $3); }
                |       ಅಭಿವ್ಯಕ್ತಿ_2 PLUS ಅಭಿವ್ಯಕ್ತಿ_2
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ದ್ವಿಮಾನ(STMT_ಅಭಿವ್ಯಕ್ತಿ_ADD, $1, $3); }
                |       ಅಭಿವ್ಯಕ್ತಿ_2 MINUS ಅಭಿವ್ಯಕ್ತಿ_2
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ದ್ವಿಮಾನ(STMT_ಅಭಿವ್ಯಕ್ತಿ_SUBTRACT, $1, $3); }
                |       ಅಭಿವ್ಯಕ್ತಿ_2 TIMES ಅಭಿವ್ಯಕ್ತಿ_2
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ದ್ವಿಮಾನ(STMT_ಅಭಿವ್ಯಕ್ತಿ_MULTIPLY, $1, $3); }
                |       Lhs EQUALS ಅಭಿವ್ಯಕ್ತಿ_2
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ದ್ವಿಮಾನ(STMT_ಅಭಿವ್ಯಕ್ತಿ_ASSIGN, $1, $3); }
                |       Term
                        { $$ = $1; }
                ;

Term            :       MINUS Term
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Unary(STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE, $2); }
                |       PLUS Term
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Unary(STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS, $2); }
                |       EXCLAM Term
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Unary(STMT_ಅಭಿವ್ಯಕ್ತಿ_NOT, $2); }
                |       INVERT Term
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Unary(STMT_ಅಭಿವ್ಯಕ್ತಿ_INVERT, $2); }
                |       Lhs
                        { $$ = $1; }
                |       FieldSpec OPAREN ಅಭಿವ್ಯಕ್ತಿ_List_2 CPAREN %prec OPAREN
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Action($1, $3.head); }
                |       Actions
                        { $$ = $1; }
                |       Terminal
                        { $$ = $1; }
                |       OPAREN ಅಭಿವ್ಯಕ್ತಿ_2 CPAREN
                        { $$ = $2; }
                ;

MultiActionList :       MultiActionList COMMA Action
                        {
                            ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create_Action_List($3);
                            $$ = $1;
                            $$.last->common.next = &ಅಭಿವ್ಯಕ್ತಿ->common; $$.last = ಅಭಿವ್ಯಕ್ತಿ;
                        }
                |       MultiActionList COMMA Actions
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       Action
                        { $$.head = $$.last = ಅಭಿವ್ಯಕ್ತಿ_Create_Action_List($1); }
                |       NonEmptyActions
                        { $$.head = $$.last = $1; }
                ;

ActionList      :       ActionList COMMA Action
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       Action
                        { $$.head = $$.last = $1; }
                ;

NonEmptyActions :       OBRACE ActionList CBRACE
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Action_List($2.head); }
                ;

Actions         :       NonEmptyActions
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Action_List(ರಿಕ್ತ); }
                ;

Action          :       FieldSpec OPAREN ಅಭಿವ್ಯಕ್ತಿ_List_2 CPAREN
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Action($1, $3.head); }
                ;

Lhs             :       FieldSpec
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Ident($1); }
                |       FieldSpec DOT FieldSpec
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Field_Ref($1, $3); }
                |       FieldSpec OBRACKET ಅಭಿವ್ಯಕ್ತಿ_2 CBRACKET
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Array_Ref(XKB_ATOM_NONE, $1, $3); }
                |       FieldSpec DOT FieldSpec OBRACKET ಅಭಿವ್ಯಕ್ತಿ_2 CBRACKET
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Array_Ref($1, $3, $5); }
                ;

OptTerminal     :       Terminal
                        { $$ = $1; }
                |       { $$ = ರಿಕ್ತ; }
                ;

Terminal        :       String
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_String($1); }
                |       Integer
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Integer($1); }
                |       Float
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_Float(/* Discard $1 */); }
                |       KEYNAME
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Name($1); }
                ;

Multi_ಕೀಲಿ_Sym_List :       Multi_ಕೀಲಿ_Sym_List COMMA ಕೀಲಿ_Sym_Lit
                        {
                            ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List($3);
                            $$ = $1;
                            $$.last->common.next = &ಅಭಿವ್ಯಕ್ತಿ->common; $$.last = ಅಭಿವ್ಯಕ್ತಿ;
                        }
                |       Multi_ಕೀಲಿ_Sym_List COMMA ಕೀಲಿ_Syms
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       ಕೀಲಿ_Sym_Lit
                        { $$.head = $$.last = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List($1); }
                |       Non_Empty_ಕೀಲಿ_Syms
                        { $$.head = $$.last = $1; }
                ;

ಕೀಲಿ_Sym_List      :       ಕೀಲಿ_Sym_List COMMA ಕೀಲಿ_Sym_Lit
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Append_ಕೀಲಿ_Sym_List($1, $3); }
                |       ಕೀಲಿ_Sym_List COMMA STRING
                        {
                            $$ = ಅಭಿವ್ಯಕ್ತಿ_ಕೀಲಿ_Sym_List_Append_String(param->scanner, $1, $3);
                            ಮುಕ್ತ($3);
                            ಯದಿ (!$$)
                                YYERROR;
                        }
                |       ಕೀಲಿ_Sym_Lit
                        {
                            $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List($1);
                            ಯದಿ (!$$)
                                YYERROR;
                        }
                |       STRING
                        {
                            $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List(XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ);
                            ಯದಿ (!$$)
                                YYERROR;
                            $$ = ಅಭಿವ್ಯಕ್ತಿ_ಕೀಲಿ_Sym_List_Append_String(param->scanner, $$, $1);
                            ಮುಕ್ತ($1);
                            ಯದಿ (!$$)
                                YYERROR;
                        }
                ;

Non_Empty_ಕೀಲಿ_Syms :       OBRACE ಕೀಲಿ_Sym_List CBRACE
                        { $$ = $2; }
                |       STRING
                        {
                            $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List(XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ);
                            ಯದಿ (!$$)
                                YYERROR;
                            $$ = ಅಭಿವ್ಯಕ್ತಿ_ಕೀಲಿ_Sym_List_Append_String(param->scanner, $$, $1);
                            ಮುಕ್ತ($1);
                            ಯದಿ (!$$)
                                YYERROR;
                        }
                ;

ಕೀಲಿ_Syms         :       Non_Empty_ಕೀಲಿ_Syms
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = ಅಭಿವ್ಯಕ್ತಿ_Create_ಕೀಲಿ_Sym_List(XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ); }
                ;

ಕೀಲಿ_Sym_2          :       ಕೀಲಿ_Sym_Lit
                        { $$ = $1; }
                |       STRING
                        {
                            $$ = KeysymParseString(param->scanner, $1);
                            ಮುಕ್ತ($1);
                            ಯದಿ ($$ == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ)
                                YYERROR;
                        }
                ;

ಕೀಲಿ_Sym_Lit       :       IDENT
                        {
                            ಯದಿ (!resolve_keysym(param, $1, &$$)) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (unsigned ಪೂರ್ಣಾಙ್ಕ) $1.len, $1.start
                                );
                                $$ = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
                            }
                        }
                        /* Handle keysym that is also a keyword  */
                |       SECTION { $$ = XKB_ಕೀಲಿ_section; }
                |       DECIMAL_DIGIT
                        {
                            /*
                             * Special case for digits 0..9:
                             * map to XKB_KEY_0 .. XKB_KEY_9, consistent with
                             * other keysym names: <name> → XKB_KEY_<name>.
                             */
                            $$ = XKB_ಕೀಲಿ_0 + (xkb_keysym_t) $1;
                        }
                |       INTEGER
                        {
                            ಯದಿ ($1 < XKB_KEYSYM_MIN) {
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
                                $$ = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
                            }
                            /*
                             * Integers 0..9 are handled with DECIMAL_DIGIT if
                             * they were formatted as single characters '0'..'9'.
                             * Otherwise they are handled here as raw keysyms
                             * values. E.g. `01` and `0x1` are interpreted as
                             * the keysym 0x0001, while `1` is interpreted as
                             * XKB_KEY_1.
                             */
                            ಅನ್ಯಥಾ {
                                /* Any other numeric value */
                                ಯದಿ ($1 <= XKB_KEYSYM_MAX) {
                                    /*
                                     * Valid keysym
                                     * No normalization is performed and value
                                     * is used as is.
                                     */
                                    $$ = (xkb_keysym_t) $1;
                                    check_deprecated_keysyms(
                                        parser_warn, param, param->ctx,
                                        $$, ರಿಕ್ತ, $$, "%#06"PRIx32, "");
                                } ಅನ್ಯಥಾ {
                                    /* Invalid keysym */
                                    parser_warn(
                                        param, XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                        "unrecognized keysym \"%#06"PRIx64"\" "
                                        "(%"PRId64")", $1, $1
                                    );
                                    $$ = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
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

Signed_ಸಂಖ್ಯೆ    :       MINUS ಸಂಖ್ಯೆ    { $$ = -$2; }
                |       ಸಂಖ್ಯೆ          { $$ = $1; }
                ;

ಸಂಖ್ಯೆ          :       FLOAT         { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                |       INTEGER       { $$ = $1; }
                ;

Float           :       FLOAT   { $$ = 0; }
                ;

Integer         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

ಕೀಲಿ_Code_2         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

Ident           :       IDENT   { $$ = xkb_atom_intern(param->ctx, $1.start, $1.len); }
                |       DEFAULT { $$ = xkb_atom_intern_literal(param->ctx, "default"); }
                ;

String          :       STRING  { $$ = xkb_atom_intern(param->ctx, $1, strlen($1)); ಮುಕ್ತ($1); }
                ;

OptMapName      :       MapName { $$ = $1; }
                |               { $$ = ರಿಕ್ತ; }
                ;

MapName         :       STRING  { $$ = $1; }
                ;

%%

/* Parse a specific section */
XkbFile *
parse(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಸಂರಚನಾ parser_keymap_config *config,
      ಸಂರಚನಾ scanner *scanner, ಧ್ರುವ ವರ್ಣ *map)
{
    ಪೂರ್ಣಾಙ್ಕ ret;
    XkbFile *first = ರಿಕ್ತ;
    ಸಂರಚನಾ parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = ರಿಕ್ತ,
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

    ಯಾವತ್ ((ret = yyparse(&param)) == 0 && param.more_maps) {
        ಯದಿ (map) {
            ಯದಿ (streq_not_null(map, param.rtrn->name))
                ಪ್ರತ್ಯಾಗಮ param.rtrn;
            ಅನ್ಯಥಾ
                FreeXkbFile(param.rtrn);
        }
        ಅನ್ಯಥಾ {
            ಯದಿ (param.rtrn->flags & MAP_IS_DEFAULT) {
                FreeXkbFile(first);
                ಪ್ರತ್ಯಾಗಮ param.rtrn;
            }
            ಅನ್ಯಥಾ ಯದಿ (!first) {
                first = param.rtrn;
            }
            ಅನ್ಯಥಾ {
                FreeXkbFile(param.rtrn);
            }
        }
        param.rtrn = ರಿಕ್ತ;
    }

    ಯದಿ (ret != 0) {
        /* Some error happend; clear the Xkbfiles parsed so far */
        FreeXkbFile(first);
        FreeXkbFile(param.rtrn);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    /*
     * Warn about implicit default section,
     * but only if not a keymap: multiple keymaps per file not supported
     */
    ಯದಿ (first && first->file_type != FILE_TYPE_KEYMAP)
        log_vrb(ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_MISSING_DEFAULT_SECTION,
                "No section name in include statement, but \"%s\" contains several; "
                "Using first defined section, \"%s\"\n",
                scanner->file_name, safe_map_name(first->name));

    ಪ್ರತ್ಯಾಗಮ first;
}

/* Parse the next section */
bool
parse_next(ಸಂರಚನಾ xkb_context *ctx,
           ಧ್ರುವ ಸಂರಚನಾ parser_keymap_config *config,
           ಸಂರಚನಾ scanner *scanner, XkbFile **xkb_file)
{
    ಪೂರ್ಣಾಙ್ಕ ret;
    ಸಂರಚನಾ parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = ರಿಕ್ತ,
        .config = *config,
        .more_maps = false,
    };

    ಯದಿ ((ret = yyparse(&param)) == 0 && param.more_maps) {
        *xkb_file = param.rtrn;
        ಪ್ರತ್ಯಾಗಮ true;
    } ಅನ್ಯಥಾ {
        FreeXkbFile(param.rtrn);
        *xkb_file = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ (ret == 0);
    }
}
