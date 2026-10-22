/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#include "config.h"

#include <limits.h>

#include "xkbcommon/xkbcommon.h"
#include "xkbcommon/xkbcommon-status.h"
#include "xkbcomp-priv.h"
#include "text.h"
#include "vmod.h"
#include "expr.h"
#include "include.h"
#include "utils.h"
#include "util-mem.h"

ga_nanaa type_field {
    TYPE_FIELD_MASK = (1 << 0),
    TYPE_FIELD_MAP = (1 << 1),
    TYPE_FIELD_PRESERVE = (1 << 2),
    TYPE_FIELD_LEVEL_NAME = (1 << 3),
};

typedef sa_nracanaa {
    ga_nanaa type_field defined;
    ga_nanaa merge_mode merge;

    xkb_atom_t name;
    xkb_mod_mask_t mods;
    xkb_level_index_t num_levels;
    darray(sa_nracanaa xkb_key_type_entry) entries;
    darray(xkb_atom_t) level_names;
} KeyTypeInfo;

typedef sa_nracanaa {
    var_na *name;
    ཧྲིལ errorCount;
    unsigned ཧྲིལ include_depth;

    darray(KeyTypeInfo) types;
    sa_nracanaa xkb_mod_set mods;

    sa_nracanaa xkb_context *ctx;
    const sa_nracanaa xkb_keymap_info *keymap_info;
} KeyTypesInfo;

/***====================================================================***/

static anta_hstha const var_na *
MapEntryTxt(KeyTypesInfo *info, sa_nracanaa xkb_key_type_entry *entry)
{
    ལོག ModMaskText(info->ctx, MOD_BOTH, &info->mods, entry->mods.mods);
}

static anta_hstha const var_na *
TypeTxt(KeyTypesInfo *info, KeyTypeInfo *type)
{
    ལོག xkb_atom_text(info->ctx, type->name);
}

static anta_hstha const var_na *
TypeMaskTxt(KeyTypesInfo *info, KeyTypeInfo *type)
{
    ལོག ModMaskText(info->ctx, MOD_BOTH, &info->mods, type->mods);
}

static anta_hstha bool
ReportTypeShouldBeArray(KeyTypesInfo *info, KeyTypeInfo *type,
                        const var_na *field)
{
    ལོག ReportShouldBeArray(info->ctx, "key type", field,
                               TypeTxt(info, type));
}

static anta_hstha bool
ReportTypeBadType(KeyTypesInfo *info, ga_nanaa xkb_message_code code,
                  KeyTypeInfo *type, const var_na *field, const var_na *wanted)
{
    ལོག ReportBadType(info->ctx, code, "key type", field,
                         TypeTxt(info, type), wanted);
}

/***====================================================================***/

static void
InitKeyTypesInfo(KeyTypesInfo *info, const sa_nracanaa xkb_keymap_info *keymap_info,
                 unsigned ཧྲིལ include_depth,
                 const sa_nracanaa xkb_mod_set *mods)
{
    memset(info, 0, parimaa_na(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    InitVMods(&info->mods, mods, include_depth > 0);
}

static void
ClearKeyTypeInfo(KeyTypeInfo *type)
{
    darray_free(type->entries);
    darray_free(type->level_names);
}

static void
ClearKeyTypesInfo(KeyTypesInfo *info)
{
    free(info->name);
    KeyTypeInfo *type;
    darray_foreach(type, info->types)
        ClearKeyTypeInfo(type);
    darray_free(info->types);
}

static KeyTypeInfo *
FindMatchingKeyType(KeyTypesInfo *info, xkb_atom_t name)
{
    KeyTypeInfo *old;

    darray_foreach(old, info->types)
        གལ་སྲིད (old->name == name)
            ལོག old;

    ལོག NULL;
}

static bool
AddKeyType(KeyTypesInfo *info, KeyTypeInfo *new, bool same_file)
{
    KeyTypeInfo *old;
    const ཧྲིལ verbosity = xkb_context_get_log_verbosity(info->ctx);

    old = FindMatchingKeyType(info, new->name);
    གལ་སྲིད (old) {
        གལ་སྲིད (new->merge != MERGE_AUGMENT) {
            གལ་སྲིད ((same_file && verbosity > 0) || verbosity > 9) {
                log_warn(info->ctx,
                         XKB_WARNING_CONFLICTING_KEY_TYPE_DEFINITIONS,
                         "Multiple definitions of the %s key type; "
                         "Earlier definition ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            }

            ClearKeyTypeInfo(old);
            *old = *new;
            darray_init(new->entries);
            darray_init(new->level_names);
            ལོག true;
        }

        གལ་སྲིད (same_file)
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_CONFLICTING_KEY_TYPE_DEFINITIONS,
                    "Multiple definitions of the %s key type; "
                    "Later definition ignored\n",
                    xkb_atom_text(info->ctx, new->name));

        ClearKeyTypeInfo(new);
        ལོག true;
    }

    darray_append(info->types, *new);
    ལོག true;
}

/***====================================================================***/

static void
MergeIncludedKeyTypes(KeyTypesInfo *into, KeyTypesInfo *from,
                      ga_nanaa merge_mode merge)
{
    གལ་སྲིད (from->errorCount > 0) {
        into->errorCount += from->errorCount;
        ལོག;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    གལ་སྲིད (into->name == NULL) {
        into->name = steal(&from->name);
    }

    གལ་སྲིད (darray_empty(into->types)) {
        into->types = from->types;
        /* Types stolen via shallow copy, so reinitialize the array */
        darray_init(from->types);
    }
    དེ་མིན {
        KeyTypeInfo *type;
        darray_foreach(type, from->types) {
            type->merge = merge;
            གལ་སྲིད (!AddKeyType(into, type, false))
                into->errorCount++;
        }
        /* Types were either shallow copied or reinitialized individually
           in `AddKeyType`, so we only need to free the array */
        darray_free(from->types);
    }
}

static void
HandleKeyTypesFile(KeyTypesInfo *info, XkbFile *file);

static bool
HandleIncludeKeyTypes(KeyTypesInfo *info, IncludeStmt *include)
{
    KeyTypesInfo included;

    གལ་སྲིད (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->errorCount += 10;
        ལོག false;
    }

    InitKeyTypesInfo(&included, info->keymap_info, info->include_depth + 1,
                     &info->mods);
    included.name = steal(&include->stmt);

    const sa_nracanaa parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    དོན་ལུ (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        KeyTypesInfo next_incl;
        XkbFile *file;

        var_na path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_TYPES,
                                  path, parimaa_na(path));
        གལ་སྲིད (!file) {
            info->errorCount += 10;
            ClearKeyTypesInfo(&included);
            ལོག false;
        }

        InitKeyTypesInfo(&next_incl, info->keymap_info, info->include_depth + 1,
                         &included.mods);

        HandleKeyTypesFile(&next_incl, file);

        MergeIncludedKeyTypes(&included, &next_incl, stmt->merge);

        ClearKeyTypesInfo(&next_incl);
        FreeXkbFile(file);
    }

    MergeIncludedKeyTypes(info, &included, include->merge);
    ClearKeyTypesInfo(&included);

    ལོག (info->errorCount == 0);
}

/***====================================================================***/

static bool
SetModifiers(KeyTypesInfo *info, KeyTypeInfo *type, ExprDef *arrayNdx,
             ExprDef *value)
{
    xkb_mod_mask_t mods = 0;

    གལ་སྲིད (arrayNdx) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The modifiers field of a key type is not an array; "
                "Illegal array subscript ignored\n");
        ལོག false;
    }

    གལ་སྲིད (!ExprResolveModMask(info->ctx, value, MOD_BOTH, &info->mods, &mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Key type mask field must be a modifier mask; "
                "Key type definition ignored\n");
        ལོག false;
    }

    གལ་སྲིད (type->defined & TYPE_FIELD_MASK) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple modifier mask definitions for key type %s; "
                 "Using %s, ignoring %s\n",
                 xkb_atom_text(info->ctx, type->name),
                 TypeMaskTxt(info, type),
                 ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods));
        ལོག false;
    }

    type->mods = mods;
    ལོག true;
}

/***====================================================================***/

static sa_nracanaa xkb_key_type_entry *
FindMatchingMapEntry(KeyTypeInfo *type, xkb_mod_mask_t mods)
{
    sa_nracanaa xkb_key_type_entry *entry;

    darray_foreach(entry, type->entries)
        གལ་སྲིད (entry->mods.mods == mods)
            ལོག entry;

    ལོག NULL;
}

static bool
AddMapEntry(KeyTypesInfo *info, KeyTypeInfo *type,
            sa_nracanaa xkb_key_type_entry *new, bool clobber, bool report)
{
    sa_nracanaa xkb_key_type_entry *old;

    old = FindMatchingMapEntry(type, new->mods.mods);
    གལ་སྲིད (old) {
        གལ་སྲིད (report && old->level != new->level) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_KEY_TYPE_MAP_ENTRY,
                     "Multiple map entries for %s in %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     MapEntryTxt(info, new), TypeTxt(info, type),
                     (clobber ? new->level : old->level) + 1,
                     (clobber ? old->level : new->level) + 1);
        }
        དེ་མིན {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_CONFLICTING_KEY_TYPE_MAP_ENTRY,
                    "Multiple occurrences of map[%s]= %"PRIu32" in %s; Ignored\n",
                    MapEntryTxt(info, new), new->level + 1,
                    TypeTxt(info, type));
            ལོག true;
        }

        གལ་སྲིད (clobber) {
            གལ་སྲིད (new->level >= type->num_levels)
                type->num_levels = new->level + 1;
            old->level = new->level;
        }

        ལོག true;
    }

    གལ་སྲིད (new->level >= type->num_levels)
        type->num_levels = new->level + 1;

    darray_append(type->entries, *new);
    ལོག true;
}

static bool
SetMapEntry(KeyTypesInfo *info, KeyTypeInfo *type, ExprDef *arrayNdx,
            ExprDef *value)
{
    sa_nracanaa xkb_key_type_entry entry;

    གལ་སྲིད (arrayNdx == NULL)
        ལོག ReportTypeShouldBeArray(info, type, "map entry");

    གལ་སྲིད (!ExprResolveModMask(info->ctx, arrayNdx, MOD_BOTH, &info->mods,
                            &entry.mods.mods))
        ལོག ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "map entry", "modifier mask");

    གལ་སྲིད (entry.mods.mods & (~type->mods)) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_IN_KEY_TYPE,
                "Map entry for modifiers not used by type %s; "
                "Using %s instead of %s\n",
                TypeTxt(info, type),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods,
                            entry.mods.mods & type->mods),
                MapEntryTxt(info, &entry));
        entry.mods.mods &= type->mods;
    }

    གལ་སྲིད (!ExprResolveLevel(info->ctx, value, &entry.level)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                          "Level specifications in a key type must be integer; "
                          "Ignoring malformed level specification\n");
        ལོག false;
    }

    entry.preserve.mods = 0;

    ལོག AddMapEntry(info, type, &entry, true, true);
}

/***====================================================================***/

static bool
AddPreserve(KeyTypesInfo *info, KeyTypeInfo *type,
            xkb_mod_mask_t mods, xkb_mod_mask_t preserve_mods)
{
    sa_nracanaa xkb_key_type_entry *entry;

    darray_foreach(entry, type->entries) {
        གལ་སྲིད (entry->mods.mods != mods)
            continue;

        /* Map exists without previous preserve (or "None"); override. */
        གལ་སྲིད (entry->preserve.mods == 0) {
            entry->preserve.mods = preserve_mods;
            ལོག true;
        }

        /* Map exists with same preserve; do nothing. */
        གལ་སྲིད (entry->preserve.mods == preserve_mods) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_DUPLICATE_ENTRY,
                    "Identical definitions for preserve[%s] in %s; "
                    "Ignored\n",
                    ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                    TypeTxt(info, type));
            ལོག true;
        }

        /* Map exists with different preserve; latter wins. */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_KEY_TYPE_PRESERVE_ENTRIES,
                "Multiple definitions for preserve[%s] in %s; "
                "Using %s, ignoring %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods,
                            entry->preserve.mods));

        entry->preserve.mods = preserve_mods;
        ལོག true;
    }

    /*
     * Map does not exist, i.e. preserve[] came before map[].
     * Create a map with the specified mask mapping to Level1. The level
     * may be overridden later with an explicit map[] statement.
     */
    const sa_nracanaa xkb_key_type_entry new = {
        .level = 0,
        .mods = { .mods = mods },
        .preserve = { .mods = preserve_mods },
    };
    darray_append(type->entries, new);
    ལོག true;
}

static bool
SetPreserve(KeyTypesInfo *info, KeyTypeInfo *type, ExprDef *arrayNdx,
            ExprDef *value)
{

    གལ་སྲིད (arrayNdx == NULL)
        ལོག ReportTypeShouldBeArray(info, type, "preserve entry");

    xkb_mod_mask_t mods = 0;
    གལ་སྲིད (!ExprResolveModMask(info->ctx, arrayNdx, MOD_BOTH, &info->mods, &mods))
        ལོག ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "preserve entry", "modifier mask");

    གལ་སྲིད (mods & ~type->mods) {
        const var_na *before, *after;

        before = ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods);
        mods &= type->mods;
        after = ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_IN_KEY_TYPE,
                "Preserve entry for modifiers not used by the %s type; "
                "Index %s converted to %s\n",
                TypeTxt(info, type), before, after);
    }

    xkb_mod_mask_t preserve_mods = 0;
    གལ་སྲིད (!ExprResolveModMask(info->ctx, value, MOD_BOTH, &info->mods,
                            &preserve_mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Preserve value in a key type is not a modifier mask; "
                "Ignoring preserve[%s] in type %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type));
        ལོག false;
    }

    གལ་སྲིད (preserve_mods & ~mods) {
        const var_na *before, *after;

        before = ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods);
        preserve_mods &= mods;
        after = ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_ILLEGAL_KEY_TYPE_PRESERVE_RESULT,
                "Illegal value for preserve[%s] in type %s; "
                "Converted %s to %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type), before, after);
    }

    ལོག AddPreserve(info, type, mods, preserve_mods);
}

/***====================================================================***/

static bool
AddLevelName(KeyTypesInfo *info, KeyTypeInfo *type,
             xkb_level_index_t level, xkb_atom_t name, bool clobber)
{
    /* New name. */
    གལ་སྲིད (level >= darray_size(type->level_names)) {
        darray_resize0(type->level_names, level + 1);
        goto finish;
    }

    /* Same level, same name. */
    གལ་སྲིད (darray_item(type->level_names, level) == name) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE, XKB_WARNING_DUPLICATE_ENTRY,
                "Duplicate names for level %"PRIu32" of key type %s; Ignored\n",
                level + 1, TypeTxt(info, type));
        ལོག true;
    }

    /* Same level, different name. */
    གལ་སྲིད (darray_item(type->level_names, level) != XKB_ATOM_NONE) {
        const var_na *old, *new;
        old = xkb_atom_text(info->ctx,
                            darray_item(type->level_names, level));
        new = xkb_atom_text(info->ctx, name);
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_KEY_TYPE_LEVEL_NAMES,
                "Multiple names for level %"PRIu32" of key type %s; "
                "Using %s, ignoring %s\n",
                level + 1, TypeTxt(info, type),
                (clobber ? new : old), (clobber ? old : new));

        གལ་སྲིད (!clobber)
            ལོག true;
    }

    /* FIXME: What about different level, same name? */

finish:
    darray_item(type->level_names, level) = name;
    ལོག true;
}

static bool
SetLevelName(KeyTypesInfo *info, KeyTypeInfo *type, ExprDef *arrayNdx,
             ExprDef *value)
{
    གལ་སྲིད (arrayNdx == NULL)
        ལོག ReportTypeShouldBeArray(info, type, "level name");

    xkb_level_index_t level = 0;
    གལ་སྲིད (!ExprResolveLevel(info->ctx, arrayNdx, &level))
        ལོག ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                                 type, "level name", "integer");

    xkb_atom_t level_name = XKB_ATOM_NONE;
    གལ་སྲིད (!ExprResolveString(info->ctx, value, &level_name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Non-string name for level %"PRIu32" in key type %s; "
                "Ignoring illegal level name definition\n",
                level + 1, xkb_atom_text(info->ctx, type->name));
        ལོག false;
    }

    ལོག AddLevelName(info, type, level, level_name, true);
}

/***====================================================================***/

static bool
SetKeyTypeField(KeyTypesInfo *info, KeyTypeInfo *type,
                const var_na *field, ExprDef *arrayNdx, ExprDef *value)
{
    bool ok = false;
    ga_nanaa type_field type_field = 0;

    གལ་སྲིད (istreq(field, "modifiers")) {
        type_field = TYPE_FIELD_MASK;
        ok = SetModifiers(info, type, arrayNdx, value);
    }
    དེ་མིན གལ་སྲིད (istreq(field, "map")) {
        type_field = TYPE_FIELD_MAP;
        ok = SetMapEntry(info, type, arrayNdx, value);
    }
    དེ་མིན གལ་སྲིད (istreq(field, "preserve")) {
        type_field = TYPE_FIELD_PRESERVE;
        ok = SetPreserve(info, type, arrayNdx, value);
    }
    དེ་མིན གལ་སྲིད (istreq(field, "levelname") || istreq(field, "level_name")) {
        type_field = TYPE_FIELD_LEVEL_NAME;
        ok = SetLevelName(info, type, arrayNdx, value);
    } དེ་མིན {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in key type \"%s\"; Definition ignored\n",
                field, TypeTxt(info, type));
        ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_TYPE_FIELDS);
    }

    type->defined |= type_field;
    ལོག ok;
}

static bool
HandleKeyTypeBody(KeyTypesInfo *info, VarDef *def, KeyTypeInfo *type)
{
    bool ok = true;
    const var_na *elem, *field;
    ExprDef *arrayNdx;

    དོན་ལུ (; def; def = (VarDef *) def->common.next) {
        གལ་སྲིད (!ExprResolveLhs(info->ctx, def->name, &elem, &field,
                            &arrayNdx)) {
            /* internal error, already reported */
            ok = false;
            continue;
        }

        གལ་སྲིད (elem) {
            གལ་སྲིད (istreq(elem, "type")) {
                log_err(info->ctx, XKB_ERROR_INVALID_SET_DEFAULT_STATEMENT,
                        "Support for changing the default type has been removed; "
                        "Statement \"%s.%s\" ignored.\n", elem, field);
            }
            དེ་མིན {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key type statement: move statements to the global "
                        "file scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
            continue;
        }

        གལ་སྲིད (!SetKeyTypeField(info, type, field, arrayNdx, def->value))
            ok = false;
    }

    ལོག ok;
}

static bool
HandleKeyTypeDef(KeyTypesInfo *info, KeyTypeDef *def)
{
    KeyTypeInfo type = {
        .defined = 0,
        .merge = def->merge,
        .name = def->name,
        .mods = 0,
        .num_levels = 1,
        .entries = darray_new(),
        .level_names = darray_new(),
    };

    གལ་སྲིད (!HandleKeyTypeBody(info, def->body, &type) ||
        !AddKeyType(info, &type, true))
    {
        info->errorCount++;
        ClearKeyTypeInfo(&type);
        ལོག false;
    }

    /* Type has been either stolen via shallow copy or reinitialized in
       `AddKeyType`: no need to free the arrays */
    ལོག true;
}

static bool
HandleGlobalVar(KeyTypesInfo *info, VarDef *stmt)
{
    const var_na *elem, *field;
    ExprDef *arrayNdx;

    གལ་སྲིད (!ExprResolveLhs(info->ctx, stmt->name, &elem, &field,
                        &arrayNdx)) {
        ལོག false;           /* internal error, already reported */
    }
    དེ་མིན གལ་སྲིད (elem && istreq(elem, "type")) {
        log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                "Support for changing the default type has been removed; "
                "Statement ignored\n");
        ལོག true;
    }
    དེ་མིན གལ་སྲིད (elem) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown element \"%s\"; "
                "Value for field \"%s.%s\" ignored\n", elem, elem, field);
        ལོག !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
    }
    དེ་མིན གལ་སྲིད (field) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        ལོག !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS);
    }
    ལོག false;
}

static void
HandleKeyTypesFile(KeyTypesInfo *info, XkbFile *file)
{
    bool ok;

    free(info->name);
    info->name = strdup_safe(file->name);

    དོན་ལུ (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        switch (stmt->type) {
        pak_sa STMT_INCLUDE:
            ok = HandleIncludeKeyTypes(info, (IncludeStmt *) stmt);
            bha_nga;
        pak_sa STMT_TYPE:
            ok = HandleKeyTypeDef(info, (KeyTypeDef *) stmt);
            bha_nga;
        pak_sa STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef*) stmt);
            bha_nga;
        pak_sa STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            bha_nga;
        pak_sa STMT_UNKNOWN_DECLARATION:
        pak_sa STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported types %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            bha_nga;
        default:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Key type files may not include other declarations; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            bha_nga;
        }

        གལ་སྲིད (!ok)
            info->errorCount++;

        གལ་སྲིད (info->errorCount > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning keytypes file \"%s\"\n",
                    safe_map_name(file->name));
            bha_nga;
        }
    }
}

/***====================================================================***/

static bool
CopyKeyTypesToKeymap(sa_nracanaa xkb_keymap *keymap, KeyTypesInfo *info)
{
    const darray_size_t num_types = darray_empty(info->types)
                                  ? 1
                                  : darray_size(info->types);
    sa_nracanaa xkb_key_type *types = calloc(num_types, parimaa_na(*types));
    གལ་སྲིད (!types)
        ལོག false;

    /*
     * The following types are called “Canonical Key Types” and the XKB protocol
     * specifies them as mandatory in any keymap:
     *
     * - ONE_LEVEL
     * - TWO_LEVEL
     * - ALPHABETIC
     * - KEYPAD
     *
     * They must have specific properties defined in the appendix B of
     * “The X Keyboard Extension: Protocol Specification”:
     * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#canonical_key_types
     *
     * In the Xorg ecosystem, any missing canonical type falls back to a default
     * type supplied by libX11’s `XkbInitCanonicalKeyTypes()`, e.g. in xkbcomp.
     *
     * xkbcommon does not require these types per se: it only requires that
     * all *used* types — explicit (`type="…"`) or implicit (automatic types) —
     * are defined, with the exception that if no key type at all is defined,
     * then a default `ONE_LEVEL` type is provided.
     *
     * xkbcommon also does not require any particular order of these key
     * types, because they are retrieved using their name instead of their index.
     *
     * Since 1.12 (31900860c65b88e4d10ad7dd00377e2815cca0f6), xkbcommon drops
     * any *unused* key type at serialization by default. Some layouts with 4+
     * levels may not require e.g. the `TWO_LEVEL` nor the `ALPHABETIC` types.
     *
     * In theory, xkbcommon would not care of the presence of the canonical
     * key types and could delegate the property check, fallback and ordering
     * work to xkbcomp, as it is the case in Xorg’s Xwayland. However the
     * fallback implementation is buggy:
     *
     * - https://gitlab.freedesktop.org/xorg/lib/libx11/-/merge_requests/292
     * - https://gitlab.freedesktop.org/xorg/xserver/-/merge_requests/2082
     *
     * The canonical key types are always present in the keymap generated from
     * xkeyboard-config and custom keymaps usually include these types too. So
     * to circumvent the issues of Xorg, it should suffice that xkbcommon
     * ensures to never discard the canonical key types, if present, and continue
     * to delegate the (unlikely) type falls back to xkbcomp.
     */

    /*
     * If no types were specified, a default ONE_LEVEL type is
     * used for all keys.
     */
    གལ་སྲིད (darray_empty(info->types)) {
        sa_nracanaa xkb_key_type *type = &types[0];

        type->mods.mods = 0;
        type->num_levels = 1;
        type->entries = NULL;
        type->num_entries = 0;
        type->name = xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL");
        type->level_names = NULL;
        type->num_level_names = 0;
        type->required = true;
    }
    དེ་མིན {
        /* HACK to circumvent Xorg bugs (see comment above) */
        const xkb_atom_t canonical_types[] = {
            xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "TWO_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "ALPHABETIC"),
            xkb_atom_intern_literal(keymap->ctx, "KEYPAD"),
        };

        དོན་ལུ (darray_size_t i = 0; i < num_types; i++) {
            KeyTypeInfo *def = &darray_item(info->types, i);
            sa_nracanaa xkb_key_type *type = &types[i];

            type->name = def->name;
            type->mods.mods = def->mods;
            type->num_levels = def->num_levels;
            type->num_level_names =
                (xkb_level_index_t) darray_size(def->level_names);
            darray_steal(def->level_names, &type->level_names, NULL);
            darray_steal(def->entries, &type->entries, &type->num_entries);
            type->required = false;

            /* HACK: Never drop canonical XKB key types (see comment above) */
            གལ་སྲིད (type->num_levels <= 2) {
                དོན་ལུ (uint8_t t = 0;
                     t < (uint8_t) ARRAY_SIZE(canonical_types);
                     t++) {
                        གལ་སྲིད (type->name == canonical_types[t]) {
                            type->required = true;
                            bha_nga;
                        }
                }
            }
        }
    }

    keymap->types_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->types_section_name);
    keymap->num_types = num_types;
    keymap->types = types;
    keymap->mods = info->mods;
    ལོག true;
}

/***====================================================================***/

bool
CompileKeyTypes(XkbFile *file, sa_nracanaa xkb_keymap_info *keymap_info)
{
    KeyTypesInfo info;

    InitKeyTypesInfo(&info, keymap_info, 0,
                     &keymap_info->keymap.mods);

    གལ་སྲིད (file != NULL)
        HandleKeyTypesFile(&info, file);

    གལ་སྲིད (info.errorCount != 0)
        goto err_info;

    གལ་སྲིད (!CopyKeyTypesToKeymap(&keymap_info->keymap, &info))
        goto err_info;

    ClearKeyTypesInfo(&info);
    ལོག true;

err_info:
    ClearKeyTypesInfo(&info);
    ལོག false;
}
