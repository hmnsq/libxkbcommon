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
#अन्तर्भाव "config.h"

#अन्तर्भाव "scanner-utils.h"
#अन्तर्भाव "xkbcomp/ast.h"
#अन्तर्भाव "xkbcomp/xkbcomp-priv.h"
}

%{
#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>

#अन्तर्भाव "keysym.h"
#अन्तर्भाव "xkbcomp/xkbcomp-priv.h"
#अन्तर्भाव "xkbcomp/parser-priv.h"
#अन्तर्भाव "xkbcomp/ast-build.h"

संरचना parser_param {
    संरचना xkb_context *ctx;
    संरचना scanner *scanner;
    XkbFile *rtrn;
    संरचना parser_keymap_config config;
    bool more_maps;
};

#define parser_log_with_code(param, level, verbosity, log_सन्देश_id, fmt, ...)   \
    scanner_log_with_code((param)->scanner, level, verbosity, log_सन्देश_id, fmt,\
                          ##__VA_ARGS__)

#define parser_err(param, error_id, fmt, ...) \
    scanner_err((param)->scanner, error_id, fmt, ##__VA_ARGS__)

#define parser_warn(param, warning_id, fmt, ...) \
    scanner_warn((param)->scanner, warning_id, fmt, ##__VA_ARGS__)

#define parser_vrb(param, verbosity, warning_id, fmt, ...) \
    scanner_vrb((param)->scanner, verbosity, warning_id, fmt, ##__VA_ARGS__)

स्थिर शून्य
_xkbcommon_error(संरचना parser_param *param, अचर वर्ण *सन्देश)
{
    parser_err(param, XKB_ERROR_INVALID_XKB_SYNTAX, "%s", सन्देश);
}

स्थिर bool
resolve_keysym(संरचना parser_param *param, संरचना sval name, xkb_keysym_t *sym_rtrn)
{
    xkb_keysym_t sym;

    यदि (isvaleq(name, SVAL_LIT("any")) || isvaleq(name, SVAL_LIT("nosymbol"))) {
        *sym_rtrn = XKB_साँचो_No_चिह्न;
        फर्काऊ true;
    }

    यदि (isvaleq(name, SVAL_LIT("none")) || isvaleq(name, SVAL_LIT("voidsymbol"))) {
        *sym_rtrn = XKB_साँचो_Void_चिह्न;
        फर्काऊ true;
    }

    /* xkb_keysym_from_name needs a C string. */
    वर्ण buf[XKB_KEYSYM_NAME_MAX_SIZE];
    यदि (name.len >= परिमाण(buf)) {
        फर्काऊ false;
    }
    स_नकल(buf, name.start, name.len);
    buf[name.len] = '\0';

    sym = xkb_keysym_from_name(buf, XKB_KEYSYM_NO_FLAGS);
    यदि (sym != XKB_साँचो_No_चिह्न) {
        *sym_rtrn = sym;
        check_deprecated_keysyms(parser_warn, param, param->ctx,
                                 sym, buf, buf, "%s", "");
        फर्काऊ true;
    }

    फर्काऊ false;
}

#define param_scanner param->scanner
%}

%define api.pure
%lex-param      { संरचना scanner *param_scanner }
%parse-param    { संरचना parser_param *param }

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
        साँचो_3             24 "key"
        ALIAS           25 "alias"
        GROUP           26 "group"
        MODIFIER_MAP    27 "modifier_map"
        INDICATOR       28 "indicator"
        SHAPE           29 "shape"
        साँचोहरू_2            30 "keys"
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
        ALPHANUMERIC_साँचोहरू       73 "alphanumeric_keys"
        MODIFIER_साँचोहरू           74 "modifier_keys"
        KEYPAD_साँचोहरू             75 "keypad_keys"
        FUNCTION_साँचोहरू           76 "function_keys"
        ALTERNATE_GROUP         77 "alternate_group"
        DEPRECATED              78 "deprecated"

%right  EQUALS
%left   PLUS MINUS
%left   TIMES DIVIDE
%left   EXCLAM INVERT
%left   OPAREN

%start  XkbFile

%सङ्घ  {
        int64_t          अङ्क;
        गणना xkb_file_type file_type;
        वर्ण            *str;
        संरचना sval     sval;
        xkb_atom_t      atom;
        गणना merge_mode merge;
        गणना xkb_map_flags mapFlags;
        xkb_keysym_t    keysym;
        ParseCommon     *any;
        संरचना { ParseCommon *head; ParseCommon *last; } anyList;
        uint32_t        no_चिह्न_Or_Action_List;
        अभिव्यक्ति_Def         *अभिव्यक्ति;
        संरचना { अभिव्यक्ति_Def *head; अभिव्यक्ति_Def *last; } अभिव्यक्ति_List;
        VarDef          *var;
        संरचना { VarDef *head; VarDef *last; } varList;
        VModDef         *vmod;
        संरचना { VModDef *head; VModDef *last; } vmodList;
        InterpDef       *interp;
        साँचो_Type_Def      *साँचो_Type;
        SymbolsDef      *syms;
        ModMapDef       *modMask;
        GroupCompatDef  *groupCompat;
        LedMapDef       *ledMap;
        LedNameDef      *ledName;
        KeycodeDef      *साँचो_Code;
        साँचो_Alias_Def     *साँचो_Alias;
        UnknownStatement *unknown;
        शून्य            *geom;
        XkbFile         *file;
        संरचना { XkbFile *head; XkbFile *last; } fileList;
}

%प्रकार <अङ्क>     DECIMAL_DIGIT INTEGER FLOAT
%प्रकार <str>     STRING
%प्रकार <sval>    IDENT
%प्रकार <atom>    KEYNAME
%प्रकार <अङ्क>     साँचो_Code_2 सङ्ख्या Integer Float Signed_सङ्ख्या DoodadType
%प्रकार <merge>   MergeMode OptMergeMode
%प्रकार <file_type> XkbCompositeType FileType
%प्रकार <mapFlags> Flag Flags OptFlags
%प्रकार <str>     MapName OptMapName
%प्रकार <atom>    FieldSpec Ident Element String
%प्रकार <keysym>  साँचो_Sym_2 साँचो_Sym_Lit
%प्रकार <no_चिह्न_Or_Action_List> No_चिह्न_Or_Action_List
%प्रकार <any>     Decl
%प्रकार <anyList> DeclList
%प्रकार <अभिव्यक्ति>    अभिव्यक्ति_2 Term Lhs Terminal OptTerminal Coord CoordList
%प्रकार <अभिव्यक्ति>    Multi_साँचो_Sym_Or_Action_List NonEmptyActions Actions Action
%प्रकार <अभिव्यक्ति>    साँचो_Syms Non_Empty_साँचो_Syms साँचो_Sym_List साँचो_Or_साँचो_Sym
%प्रकार <अभिव्यक्ति_List> अभिव्यक्ति_List_2 साँचो_Or_साँचो_Sym_List
%प्रकार <अभिव्यक्ति_List> ActionList MultiActionList Multi_साँचो_Sym_List
%प्रकार <var>     VarDecl SymbolsVarDecl
%प्रकार <varList> VarDeclList SymbolsBody OptSymbolsBody
%प्रकार <vmod>    VModDef
%प्रकार <vmodList> VModDefList VModDecl
%प्रकार <interp>  InterpretDecl InterpretMatch
%प्रकार <साँचो_Type> साँचो_Type_Decl
%प्रकार <syms>    SymbolsDecl
%प्रकार <modMask> ModMapDecl
%प्रकार <groupCompat> GroupCompatDecl
%प्रकार <ledMap>  LedMapDecl
%प्रकार <ledName> LedNameDecl
%प्रकार <साँचो_Code> साँचो_Name_Decl
%प्रकार <साँचो_Alias> साँचो_Alias_Decl
%प्रकार <geom>    ShapeDecl SectionDecl SectionBody SectionBodyItem RowBody RowBodyItem
%प्रकार <geom>    साँचोहरू_3 साँचो_4 OverlayDecl Overlay_साँचो_List Overlay_साँचो OutlineList OutlineInList
%प्रकार <geom>    DoodadDecl
%प्रकार <unknown> UnknownDecl UnknownCompoundStatementDecl
%प्रकार <file>    XkbFile XkbMapConfig
%प्रकार <fileList> XkbMapConfigList
%प्रकार <file>    XkbCompositeMap

%destructor { FreeStmt((ParseCommon *) $$); }
    <any> <अभिव्यक्ति> <var> <vmod> <interp> <साँचो_Type> <syms> <modMask> <groupCompat>
    <ledMap> <ledName> <साँचो_Code> <साँचो_Alias>
%destructor { FreeStmt((ParseCommon *) $$.head); }
    <anyList> <अभिव्यक्ति_List> <varList> <vmodList>
/* The destructor also runs on the start symbol when the parser *succeeds*.
 * The `if` here catches this case. */
%destructor { यदि (!param->rtrn) FreeXkbFile($$); } <file>
%destructor { FreeXkbFile($$.head); } <fileList>
%destructor { मुक्त($$); } <str>

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
                        { $$ = param->rtrn = $1; param->more_maps = !!param->rtrn; (शून्य) yynerrs; }
                |       XkbMapConfig
                        { $$ = param->rtrn = $1; param->more_maps = !!param->rtrn; YYACCEPT; }
                |       END_OF_FILE
                        { $$ = param->rtrn = रिक्त; param->more_maps = false; }
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
                            यदि ($2) {
                                यदि ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$2->common;
                                    $$.last = $2;
                                } अन्यथा {
                                    $$.head = $$.last = $2;
                                }
                            }
                        }
                |       { $$.head = $$.last = रिक्त; }
                ;

XkbMapConfig    :       OptFlags FileType OptMapName OBRACE
                            DeclList
                        CBRACE SEMI
                        {
                            यदि ($1 & MAP_IS_DEPRECATED) {
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
                |       ALPHANUMERIC_साँचोहरू       { $$ = MAP_HAS_ALPHANUMERIC; }
                |       MODIFIER_साँचोहरू           { $$ = MAP_HAS_MODIFIER; }
                |       KEYPAD_साँचोहरू             { $$ = MAP_HAS_KEYPAD; }
                |       FUNCTION_साँचोहरू           { $$ = MAP_HAS_FN; }
                |       ALTERNATE_GROUP         { $$ = MAP_IS_ALTGR; }
                |       DEPRECATED              { $$ = MAP_IS_DEPRECATED; }
                |       IDENT
                        {
                            अचर bool error = (param->config.strict & PARSER_NO_UNKNOWN_SECTION_FLAGS);
                            parser_log_with_code(
                                param, (error ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
                                XKB_LOG_VERBOSITY_MINIMAL,
                                XKB_ERROR_UNKNOWN_SECTION_FLAG,
                                "Unknown section flag \"%.*s\"%s",
                                (अचिन्हित)$1.len, $1.start,
                                (error ? "" : "; ignored")
                            );
                            यदि (error)
                                YYABORT;
                            $$ = 0;
                        }
                ;

DeclList        :       DeclList Decl
                        {
                            यदि ($2) {
                                यदि ($1.head) {
                                    $$.head = $1.head; $1.last->next = $2; $$.last = $2;
                                } अन्यथा {
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
                            लागि (VModDef *vmod = $3.head; vmod; vmod = (VModDef *) vmod->common.next)
                                vmod->merge = $2;
                            यदि ($1.head) {
                                $$.head = $1.head; $1.last->next = &$3.head->common; $$.last = &$3.last->common;
                            } अन्यथा {
                                $$.head = &$3.head->common; $$.last = &$3.last->common;
                            }
                        }
                |       { $$.head = $$.last = रिक्त; }
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
                |       OptMergeMode साँचो_Name_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode साँचो_Alias_Decl
                        {
                            $2->merge = $1;
                            $$ = (ParseCommon *) $2;
                        }
                |       OptMergeMode साँचो_Type_Decl
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
                |       OptMergeMode ShapeDecl          { $$ = रिक्त; }
                |       OptMergeMode SectionDecl        { $$ = रिक्त; }
                |       OptMergeMode DoodadDecl         { $$ = रिक्त; }
                |       OptMergeMode UnknownDecl
                            { $$ = (ParseCommon *) $2; }
                |       OptMergeMode UnknownCompoundStatementDecl
                            { $$ = (ParseCommon *) $2; }
                |       MergeMode STRING
                        {
                            $$ = (ParseCommon *) IncludeCreate(param->ctx, $2, $1);
                            मुक्त($2);
                        }
                ;

VarDecl         :       Lhs EQUALS अभिव्यक्ति_2 SEMI
                        { $$ = VarCreate($1, $3); }
                |       Ident SEMI
                        { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident SEMI
                        { $$ = BoolVarCreate($2, false); }
                ;

साँचो_Name_Decl     :       KEYNAME EQUALS साँचो_Code_2 SEMI
                        { $$ = KeycodeCreate($1, $3); }
                ;

साँचो_Alias_Decl    :       ALIAS KEYNAME EQUALS KEYNAME SEMI
                        { $$ = साँचो_Alias_Create($2, $4); }
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
                        { $$ = VModCreate($1, रिक्त); }
                |       Ident EQUALS अभिव्यक्ति_2
                        { $$ = VModCreate($1, $3); }
                ;

InterpretDecl   :       INTERPRET InterpretMatch OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $2->def = $4.head; $$ = $2; }
                ;

InterpretMatch  :       साँचो_Sym_2 PLUS अभिव्यक्ति_2
                        { $$ = InterpCreate($1, $3); }
                |       साँचो_Sym_2
                        { $$ = InterpCreate($1, रिक्त); }
                ;

VarDeclList     :       VarDeclList VarDecl
                        {
                            यदि ($2) {
                                यदि ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$2->common;
                                    $$.last = $2;
                                } अन्यथा {
                                    $$.head = $$.last = $2;
                                }
                            }
                        }
                |       { $$.head = $$.last = रिक्त; }
                ;

साँचो_Type_Decl     :       TYPE String OBRACE
                            VarDeclList
                        CBRACE SEMI
                        { $$ = साँचो_Type_Create($2, $4.head); }
                ;

SymbolsDecl     :       साँचो_3 KEYNAME OBRACE
                            OptSymbolsBody
                        CBRACE SEMI
                        { $$ = SymbolsCreate($2, $4.head); }
                ;

OptSymbolsBody  :       SymbolsBody { $$ = $1; }
                |                   { $$.head = $$.last = रिक्त; }
                ;

SymbolsBody     :       SymbolsBody COMMA SymbolsVarDecl
                        { $$.head = $1.head; $$.last->common.next = &$3->common; $$.last = $3; }
                |       SymbolsVarDecl
                        { $$.head = $$.last = $1; }
                ;

SymbolsVarDecl  :       Lhs EQUALS अभिव्यक्ति_2         { $$ = VarCreate($1, $3); }
                |       Lhs EQUALS Multi_साँचो_Sym_Or_Action_List { $$ = VarCreate($1, $3); }
                |       Ident                   { $$ = BoolVarCreate($1, true); }
                |       EXCLAM Ident            { $$ = BoolVarCreate($2, false); }
                |       Multi_साँचो_Sym_Or_Action_List { $$ = VarCreate(रिक्त, $1); }
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
Multi_साँचो_Sym_Or_Action_List
                :       OBRACKET Multi_साँचो_Sym_List CBRACKET
                        { $$ = $2.head; }
                |       OBRACKET No_चिह्न_Or_Action_List COMMA Multi_साँचो_Sym_List CBRACKET
                        {
                            /* Prepend n times NoSymbol */
                            संरचना {अभिव्यक्ति_Def *head; अभिव्यक्ति_Def *last;} list = {
                                .head = $4.head, .last = $4.last
                            };
                            लागि (uint32_t k = 0; k < $2; k++) {
                                अभिव्यक्ति_Def* अचर syms =
                                    अभिव्यक्ति_Create_साँचो_Sym_List(XKB_साँचो_No_चिह्न);
                                यदि (!syms) {
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
                |       OBRACKET No_चिह्न_Or_Action_List COMMA MultiActionList CBRACKET
                        {
                            /* Prepend n times NoAction() */
                            संरचना {अभिव्यक्ति_Def *head; अभिव्यक्ति_Def *last;} list = {
                                .head = $4.head, .last = $4.last
                            };
                            लागि (uint32_t k = 0; k < $2; k++) {
                                अभिव्यक्ति_Def* अचर acts = अभिव्यक्ति_Create_Action_List(रिक्त);
                                यदि (!acts) {
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
                |       OBRACKET No_चिह्न_Or_Action_List CBRACKET
                        { $$ = अभिव्यक्ति_Empty_List(); }
                ;

/* A list of `{}`, which remains ambiguous until reaching a keysym or action list */
No_चिह्न_Or_Action_List
                :       No_चिह्न_Or_Action_List COMMA OBRACE CBRACE
                        { $$ = $1 + 1; }
                |       OBRACE CBRACE
                        { $$ = 1; }
                |       { $$ = 0; }
                ;

GroupCompatDecl :       GROUP Integer EQUALS अभिव्यक्ति_2 SEMI
                        { $$ = GroupCompatCreate($2, $4); }
                ;

ModMapDecl      :       MODIFIER_MAP अभिव्यक्ति_2 OBRACE साँचो_Or_साँचो_Sym_List CBRACE SEMI
                        {
                            यदि (param->config.format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
                                $2->common.प्रकार != STMT_अभिव्यक्ति_IDENT) {
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

साँचो_Or_साँचो_Sym_List :       साँचो_Or_साँचो_Sym_List COMMA साँचो_Or_साँचो_Sym
                        { $$.head = $1.head; $$.last->common.next = &$3->common; $$.last = $3; }
                |       साँचो_Or_साँचो_Sym
                        { $$.head = $$.last = $1; }
                ;

साँचो_Or_साँचो_Sym     :       KEYNAME
                        { $$ = अभिव्यक्ति_Create_साँचो_Name($1); }
                |       साँचो_Sym_2
                        { $$ = अभिव्यक्ति_Create_साँचो_Sym($1); }
                ;

LedMapDecl:             INDICATOR String OBRACE VarDeclList CBRACE SEMI
                        { $$ = LedMapCreate($2, $4.head); }
                ;

LedNameDecl:            INDICATOR Integer EQUALS अभिव्यक्ति_2 SEMI
                        { $$ = LedNameCreate($2, $4, false); }
                |       VIRTUAL INDICATOR Integer EQUALS अभिव्यक्ति_2 SEMI
                        { $$ = LedNameCreate($3, $5, true); }
                ;

UnknownDecl     :       IDENT Terminal EQUALS अभिव्यक्ति_2 SEMI
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
                        { $$ = रिक्त; }
                |       SHAPE String OBRACE CoordList CBRACE SEMI
                        { (शून्य) $4; $$ = रिक्त; }
                ;

SectionDecl     :       SECTION String OBRACE SectionBody CBRACE SEMI
                        { $$ = रिक्त; }
                ;

SectionBody     :       SectionBody SectionBodyItem     { $$ = रिक्त;}
                |       SectionBodyItem                 { $$ = रिक्त; }
                ;

SectionBodyItem :       ROW OBRACE RowBody CBRACE SEMI
                        { $$ = रिक्त; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = रिक्त; }
                |       DoodadDecl
                        { $$ = रिक्त; }
                |       LedMapDecl
                        { FreeStmt((ParseCommon *) $1); $$ = रिक्त; }
                |       OverlayDecl
                        { $$ = रिक्त; }
                ;

RowBody         :       RowBody RowBodyItem     { $$ = रिक्त;}
                |       RowBodyItem             { $$ = रिक्त; }
                ;

RowBodyItem     :       साँचोहरू_2 OBRACE साँचोहरू_3 CBRACE SEMI { $$ = रिक्त; }
                |       VarDecl
                        { FreeStmt((ParseCommon *) $1); $$ = रिक्त; }
                ;

साँचोहरू_3            :       साँचोहरू_3 COMMA साँचो_4          { $$ = रिक्त; }
                |       साँचो_4                     { $$ = रिक्त; }
                ;

साँचो_4             :       KEYNAME
                        { $$ = रिक्त; }
                |       OBRACE अभिव्यक्ति_List_2 CBRACE
                        { FreeStmt((ParseCommon *) $2.head); $$ = रिक्त; }
                ;

OverlayDecl     :       OVERLAY String OBRACE Overlay_साँचो_List CBRACE SEMI
                        { $$ = रिक्त; }
                ;

Overlay_साँचो_List  :       Overlay_साँचो_List COMMA Overlay_साँचो { $$ = रिक्त; }
                |       Overlay_साँचो                      { $$ = रिक्त; }
                ;

Overlay_साँचो      :       KEYNAME EQUALS KEYNAME          { $$ = रिक्त; }
                ;

OutlineList     :       OutlineList COMMA OutlineInList
                        { $$ = रिक्त;}
                |       OutlineInList
                        { $$ = रिक्त; }
                ;

OutlineInList   :       OBRACE CoordList CBRACE
                        { (शून्य) $2; $$ = रिक्त; }
                |       Ident EQUALS OBRACE CoordList CBRACE
                        { (शून्य) $4; $$ = रिक्त; }
                |       Ident EQUALS अभिव्यक्ति_2
                        { FreeStmt((ParseCommon *) $3); $$ = रिक्त; }
                ;

CoordList       :       CoordList COMMA Coord
                        { (शून्य) $1; (शून्य) $3; $$ = रिक्त; }
                |       Coord
                        { (शून्य) $1; $$ = रिक्त; }
                ;

Coord           :       OBRACKET Signed_सङ्ख्या COMMA Signed_सङ्ख्या CBRACKET
                        { $$ = रिक्त; }
                ;

DoodadDecl      :       DoodadType String OBRACE VarDeclList CBRACE SEMI
                        { FreeStmt((ParseCommon *) $4.head); $$ = रिक्त; }
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
                |       साँचो_3
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

अभिव्यक्ति_List_2        :       अभिव्यक्ति_List_2 COMMA अभिव्यक्ति_2
                        {
                            यदि ($3) {
                                यदि ($1.head) {
                                    $$.head = $1.head;
                                    $$.last->common.next = &$3->common;
                                    $$.last = $3;
                                } अन्यथा {
                                    $$.head = $$.last = $3;
                                }
                            }
                        }
                |       अभिव्यक्ति_2
                        { $$.head = $$.last = $1; }
                |       { $$.head = $$.last = रिक्त; }
                ;

अभिव्यक्ति_2            :       अभिव्यक्ति_2 DIVIDE अभिव्यक्ति_2
                        { $$ = अभिव्यक्ति_Create_द्विआधारी(STMT_अभिव्यक्ति_DIVIDE, $1, $3); }
                |       अभिव्यक्ति_2 PLUS अभिव्यक्ति_2
                        { $$ = अभिव्यक्ति_Create_द्विआधारी(STMT_अभिव्यक्ति_ADD, $1, $3); }
                |       अभिव्यक्ति_2 MINUS अभिव्यक्ति_2
                        { $$ = अभिव्यक्ति_Create_द्विआधारी(STMT_अभिव्यक्ति_SUBTRACT, $1, $3); }
                |       अभिव्यक्ति_2 TIMES अभिव्यक्ति_2
                        { $$ = अभिव्यक्ति_Create_द्विआधारी(STMT_अभिव्यक्ति_MULTIPLY, $1, $3); }
                |       Lhs EQUALS अभिव्यक्ति_2
                        { $$ = अभिव्यक्ति_Create_द्विआधारी(STMT_अभिव्यक्ति_ASSIGN, $1, $3); }
                |       Term
                        { $$ = $1; }
                ;

Term            :       MINUS Term
                        { $$ = अभिव्यक्ति_Create_Unary(STMT_अभिव्यक्ति_NEGATE, $2); }
                |       PLUS Term
                        { $$ = अभिव्यक्ति_Create_Unary(STMT_अभिव्यक्ति_UNARY_PLUS, $2); }
                |       EXCLAM Term
                        { $$ = अभिव्यक्ति_Create_Unary(STMT_अभिव्यक्ति_NOT, $2); }
                |       INVERT Term
                        { $$ = अभिव्यक्ति_Create_Unary(STMT_अभिव्यक्ति_INVERT, $2); }
                |       Lhs
                        { $$ = $1; }
                |       FieldSpec OPAREN अभिव्यक्ति_List_2 CPAREN %prec OPAREN
                        { $$ = अभिव्यक्ति_Create_Action($1, $3.head); }
                |       Actions
                        { $$ = $1; }
                |       Terminal
                        { $$ = $1; }
                |       OPAREN अभिव्यक्ति_2 CPAREN
                        { $$ = $2; }
                ;

MultiActionList :       MultiActionList COMMA Action
                        {
                            अभिव्यक्ति_Def *अभिव्यक्ति = अभिव्यक्ति_Create_Action_List($3);
                            $$ = $1;
                            $$.last->common.next = &अभिव्यक्ति->common; $$.last = अभिव्यक्ति;
                        }
                |       MultiActionList COMMA Actions
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       Action
                        { $$.head = $$.last = अभिव्यक्ति_Create_Action_List($1); }
                |       NonEmptyActions
                        { $$.head = $$.last = $1; }
                ;

ActionList      :       ActionList COMMA Action
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       Action
                        { $$.head = $$.last = $1; }
                ;

NonEmptyActions :       OBRACE ActionList CBRACE
                        { $$ = अभिव्यक्ति_Create_Action_List($2.head); }
                ;

Actions         :       NonEmptyActions
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = अभिव्यक्ति_Create_Action_List(रिक्त); }
                ;

Action          :       FieldSpec OPAREN अभिव्यक्ति_List_2 CPAREN
                        { $$ = अभिव्यक्ति_Create_Action($1, $3.head); }
                ;

Lhs             :       FieldSpec
                        { $$ = अभिव्यक्ति_Create_Ident($1); }
                |       FieldSpec DOT FieldSpec
                        { $$ = अभिव्यक्ति_Create_Field_Ref($1, $3); }
                |       FieldSpec OBRACKET अभिव्यक्ति_2 CBRACKET
                        { $$ = अभिव्यक्ति_Create_Array_Ref(XKB_ATOM_NONE, $1, $3); }
                |       FieldSpec DOT FieldSpec OBRACKET अभिव्यक्ति_2 CBRACKET
                        { $$ = अभिव्यक्ति_Create_Array_Ref($1, $3, $5); }
                ;

OptTerminal     :       Terminal
                        { $$ = $1; }
                |       { $$ = रिक्त; }
                ;

Terminal        :       String
                        { $$ = अभिव्यक्ति_Create_String($1); }
                |       Integer
                        { $$ = अभिव्यक्ति_Create_Integer($1); }
                |       Float
                        { $$ = अभिव्यक्ति_Create_Float(/* Discard $1 */); }
                |       KEYNAME
                        { $$ = अभिव्यक्ति_Create_साँचो_Name($1); }
                ;

Multi_साँचो_Sym_List :       Multi_साँचो_Sym_List COMMA साँचो_Sym_Lit
                        {
                            अभिव्यक्ति_Def *अभिव्यक्ति = अभिव्यक्ति_Create_साँचो_Sym_List($3);
                            $$ = $1;
                            $$.last->common.next = &अभिव्यक्ति->common; $$.last = अभिव्यक्ति;
                        }
                |       Multi_साँचो_Sym_List COMMA साँचो_Syms
                        { $$ = $1; $$.last->common.next = &$3->common; $$.last = $3; }
                |       साँचो_Sym_Lit
                        { $$.head = $$.last = अभिव्यक्ति_Create_साँचो_Sym_List($1); }
                |       Non_Empty_साँचो_Syms
                        { $$.head = $$.last = $1; }
                ;

साँचो_Sym_List      :       साँचो_Sym_List COMMA साँचो_Sym_Lit
                        { $$ = अभिव्यक्ति_Append_साँचो_Sym_List($1, $3); }
                |       साँचो_Sym_List COMMA STRING
                        {
                            $$ = अभिव्यक्ति_साँचो_Sym_List_Append_String(param->scanner, $1, $3);
                            मुक्त($3);
                            यदि (!$$)
                                YYERROR;
                        }
                |       साँचो_Sym_Lit
                        {
                            $$ = अभिव्यक्ति_Create_साँचो_Sym_List($1);
                            यदि (!$$)
                                YYERROR;
                        }
                |       STRING
                        {
                            $$ = अभिव्यक्ति_Create_साँचो_Sym_List(XKB_साँचो_No_चिह्न);
                            यदि (!$$)
                                YYERROR;
                            $$ = अभिव्यक्ति_साँचो_Sym_List_Append_String(param->scanner, $$, $1);
                            मुक्त($1);
                            यदि (!$$)
                                YYERROR;
                        }
                ;

Non_Empty_साँचो_Syms :       OBRACE साँचो_Sym_List CBRACE
                        { $$ = $2; }
                |       STRING
                        {
                            $$ = अभिव्यक्ति_Create_साँचो_Sym_List(XKB_साँचो_No_चिह्न);
                            यदि (!$$)
                                YYERROR;
                            $$ = अभिव्यक्ति_साँचो_Sym_List_Append_String(param->scanner, $$, $1);
                            मुक्त($1);
                            यदि (!$$)
                                YYERROR;
                        }
                ;

साँचो_Syms         :       Non_Empty_साँचो_Syms
                        { $$ = $1; }
                |       OBRACE CBRACE
                        { $$ = अभिव्यक्ति_Create_साँचो_Sym_List(XKB_साँचो_No_चिह्न); }
                ;

साँचो_Sym_2          :       साँचो_Sym_Lit
                        { $$ = $1; }
                |       STRING
                        {
                            $$ = KeysymParseString(param->scanner, $1);
                            मुक्त($1);
                            यदि ($$ == XKB_साँचो_No_चिह्न)
                                YYERROR;
                        }
                ;

साँचो_Sym_Lit       :       IDENT
                        {
                            यदि (!resolve_keysym(param, $1, &$$)) {
                                parser_warn(
                                    param,
                                    XKB_WARNING_UNRECOGNIZED_KEYSYM,
                                    "unrecognized keysym \"%.*s\"",
                                    (अचिन्हित पूर्णाङ्क) $1.len, $1.start
                                );
                                $$ = XKB_साँचो_No_चिह्न;
                            }
                        }
                        /* Handle keysym that is also a keyword  */
                |       SECTION { $$ = XKB_साँचो_section; }
                |       DECIMAL_DIGIT
                        {
                            /*
                             * Special case for digits 0..9:
                             * map to XKB_KEY_0 .. XKB_KEY_9, consistent with
                             * other keysym names: <name> → XKB_KEY_<name>.
                             */
                            $$ = XKB_साँचो_0 + (xkb_keysym_t) $1;
                        }
                |       INTEGER
                        {
                            यदि ($1 < XKB_KEYSYM_MIN) {
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
                                $$ = XKB_साँचो_No_चिह्न;
                            }
                            /*
                             * Integers 0..9 are handled with DECIMAL_DIGIT if
                             * they were formatted as single characters '0'..'9'.
                             * Otherwise they are handled here as raw keysyms
                             * values. E.g. `01` and `0x1` are interpreted as
                             * the keysym 0x0001, while `1` is interpreted as
                             * XKB_KEY_1.
                             */
                            अन्यथा {
                                /* Any other numeric value */
                                यदि ($1 <= XKB_KEYSYM_MAX) {
                                    /*
                                     * Valid keysym
                                     * No normalization is performed and value
                                     * is used as is.
                                     */
                                    $$ = (xkb_keysym_t) $1;
                                    check_deprecated_keysyms(
                                        parser_warn, param, param->ctx,
                                        $$, रिक्त, $$, "%#06"PRIx32, "");
                                } अन्यथा {
                                    /* Invalid keysym */
                                    parser_warn(
                                        param, XKB_ERROR_INVALID_NUMERIC_KEYSYM,
                                        "unrecognized keysym \"%#06"PRIx64"\" "
                                        "(%"PRId64")", $1, $1
                                    );
                                    $$ = XKB_साँचो_No_चिह्न;
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

Signed_सङ्ख्या    :       MINUS सङ्ख्या    { $$ = -$2; }
                |       सङ्ख्या          { $$ = $1; }
                ;

सङ्ख्या          :       FLOAT         { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                |       INTEGER       { $$ = $1; }
                ;

Float           :       FLOAT   { $$ = 0; }
                ;

Integer         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

साँचो_Code_2         :       INTEGER       { $$ = $1; }
                |       DECIMAL_DIGIT { $$ = $1; }
                ;

Ident           :       IDENT   { $$ = xkb_atom_intern(param->ctx, $1.start, $1.len); }
                |       DEFAULT { $$ = xkb_atom_intern_literal(param->ctx, "default"); }
                ;

String          :       STRING  { $$ = xkb_atom_intern(param->ctx, $1, म_माप($1)); मुक्त($1); }
                ;

OptMapName      :       MapName { $$ = $1; }
                |               { $$ = रिक्त; }
                ;

MapName         :       STRING  { $$ = $1; }
                ;

%%

/* Parse a specific section */
XkbFile *
parse(संरचना xkb_context *ctx, अचर संरचना parser_keymap_config *config,
      संरचना scanner *scanner, अचर वर्ण *map)
{
    पूर्णाङ्क ret;
    XkbFile *first = रिक्त;
    संरचना parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = रिक्त,
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

    जबसम्म ((ret = yyparse(&param)) == 0 && param.more_maps) {
        यदि (map) {
            यदि (streq_not_null(map, param.rtrn->name))
                फर्काऊ param.rtrn;
            अन्यथा
                FreeXkbFile(param.rtrn);
        }
        अन्यथा {
            यदि (param.rtrn->flags & MAP_IS_DEFAULT) {
                FreeXkbFile(first);
                फर्काऊ param.rtrn;
            }
            अन्यथा यदि (!first) {
                first = param.rtrn;
            }
            अन्यथा {
                FreeXkbFile(param.rtrn);
            }
        }
        param.rtrn = रिक्त;
    }

    यदि (ret != 0) {
        /* Some error happend; clear the Xkbfiles parsed so far */
        FreeXkbFile(first);
        FreeXkbFile(param.rtrn);
        फर्काऊ रिक्त;
    }

    /*
     * Warn about implicit default section,
     * but only if not a keymap: multiple keymaps per file not supported
     */
    यदि (first && first->file_type != FILE_TYPE_KEYMAP)
        log_vrb(ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_MISSING_DEFAULT_SECTION,
                "No section name in include statement, but \"%s\" contains several; "
                "Using first defined section, \"%s\"\n",
                scanner->file_name, safe_map_name(first->name));

    फर्काऊ first;
}

/* Parse the next section */
bool
parse_next(संरचना xkb_context *ctx,
           अचर संरचना parser_keymap_config *config,
           संरचना scanner *scanner, XkbFile **xkb_file)
{
    पूर्णाङ्क ret;
    संरचना parser_param param = {
        .scanner = scanner,
        .ctx = ctx,
        .rtrn = रिक्त,
        .config = *config,
        .more_maps = false,
    };

    यदि ((ret = yyparse(&param)) == 0 && param.more_maps) {
        *xkb_file = param.rtrn;
        फर्काऊ true;
    } अन्यथा {
        FreeXkbFile(param.rtrn);
        *xkb_file = रिक्त;
        फर्काऊ (ret == 0);
    }
}
