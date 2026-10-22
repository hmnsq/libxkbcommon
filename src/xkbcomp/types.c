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

enum type_field {
    TYPE_FIELD_MASK = (1 << 0),
    TYPE_FIELD_MAP = (1 << 1),
    TYPE_FIELD_PRESERVE = (1 << 2),
    TYPE_FIELD_LEVEL_NAME = (1 << 3),
};

typedef struct {
    enum type_field defined;
    enum merge_mode merge;

    xkb_atom_t name;
    xkb_mod_mask_t mods;
    xkb_level_index_t num_levels;
    darray(struct xkb_key_type_entry) entries;
    darray(xkb_atom_t) level_names;
} KeyTypeInfo;

typedef struct {
    char *name;
    정수 errorCount;
    unsigned 정수 include_depth;

    darray(KeyTypeInfo) types;
    struct xkb_mod_set mods;

    struct xkb_context *ctx;
    const struct xkb_keymap_info *keymap_info;
} KeyTypesInfo;

/***====================================================================***/

static inline const char *
MapEntryTxt(KeyTypesInfo *info, struct xkb_key_type_entry *entry)
{
    반환 ModMaskText(info->ctx, MOD_BOTH, &info->mods, entry->mods.mods);
}

static inline const char *
TypeTxt(KeyTypesInfo *info, KeyTypeInfo *type)
{
    반환 xkb_atom_text(info->ctx, type->name);
}

static inline const char *
TypeMaskTxt(KeyTypesInfo *info, KeyTypeInfo *type)
{
    반환 ModMaskText(info->ctx, MOD_BOTH, &info->mods, type->mods);
}

static inline bool
ReportTypeShouldBeArray(KeyTypesInfo *info, KeyTypeInfo *type,
                        const char *field)
{
    반환 ReportShouldBeArray(info->ctx, "key type", field,
                               TypeTxt(info, type));
}

static inline bool
ReportTypeBadType(KeyTypesInfo *info, enum xkb_message_code code,
                  KeyTypeInfo *type, const char *field, const char *wanted)
{
    반환 ReportBadType(info->ctx, code, "key type", field,
                         TypeTxt(info, type), wanted);
}

/***====================================================================***/

static void
InitKeyTypesInfo(KeyTypesInfo *info, const struct xkb_keymap_info *keymap_info,
                 unsigned 정수 include_depth,
                 const struct xkb_mod_set *mods)
{
    memset(info, 0, sizeof(*info));
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
        만약 (old->name == name)
            반환 old;

    반환 NULL;
}

static bool
AddKeyType(KeyTypesInfo *info, KeyTypeInfo *new, bool same_file)
{
    KeyTypeInfo *old;
    const 정수 verbosity = xkb_context_get_log_verbosity(info->ctx);

    old = FindMatchingKeyType(info, new->name);
    만약 (old) {
        만약 (new->merge != MERGE_AUGMENT) {
            만약 ((same_file && verbosity > 0) || verbosity > 9) {
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
            반환 true;
        }

        만약 (same_file)
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_CONFLICTING_KEY_TYPE_DEFINITIONS,
                    "Multiple definitions of the %s key type; "
                    "Later definition ignored\n",
                    xkb_atom_text(info->ctx, new->name));

        ClearKeyTypeInfo(new);
        반환 true;
    }

    darray_append(info->types, *new);
    반환 true;
}

/***====================================================================***/

static void
MergeIncludedKeyTypes(KeyTypesInfo *into, KeyTypesInfo *from,
                      enum merge_mode merge)
{
    만약 (from->errorCount > 0) {
        into->errorCount += from->errorCount;
        반환;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    만약 (into->name == NULL) {
        into->name = steal(&from->name);
    }

    만약 (darray_empty(into->types)) {
        into->types = from->types;
        /* Types stolen via shallow copy, so reinitialize the array */
        darray_init(from->types);
    }
    아니면 {
        KeyTypeInfo *type;
        darray_foreach(type, from->types) {
            type->merge = merge;
            만약 (!AddKeyType(into, type, false))
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

    만약 (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->errorCount += 10;
        반환 false;
    }

    InitKeyTypesInfo(&included, info->keymap_info, info->include_depth + 1,
                     &info->mods);
    included.name = steal(&include->stmt);

    const struct parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    위해 (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        KeyTypesInfo next_incl;
        XkbFile *file;

        char path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_TYPES,
                                  path, sizeof(path));
        만약 (!file) {
            info->errorCount += 10;
            ClearKeyTypesInfo(&included);
            반환 false;
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

    반환 (info->errorCount == 0);
}

/***====================================================================***/

static bool
SetModifiers(KeyTypesInfo *info, KeyTypeInfo *type, ExprDef *arrayNdx,
             ExprDef *value)
{
    xkb_mod_mask_t mods = 0;

    만약 (arrayNdx) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The modifiers field of a key type is not an array; "
                "Illegal array subscript ignored\n");
        반환 false;
    }

    만약 (!ExprResolveModMask(info->ctx, value, MOD_BOTH, &info->mods, &mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Key type mask field must be a modifier mask; "
                "Key type definition ignored\n");
        반환 false;
    }

    만약 (type->defined & TYPE_FIELD_MASK) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple modifier mask definitions for key type %s; "
                 "Using %s, ignoring %s\n",
                 xkb_atom_text(info->ctx, type->name),
                 TypeMaskTxt(info, type),
                 ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods));
        반환 false;
    }

    type->mods = mods;
    반환 true;
}

/***====================================================================***/

static struct xkb_key_type_entry *
FindMatchingMapEntry(KeyTypeInfo *type, xkb_mod_mask_t mods)
{
    struct xkb_key_type_entry *entry;

    darray_foreach(entry, type->entries)
        만약 (entry->mods.mods == mods)
            반환 entry;

    반환 NULL;
}

static bool
AddMapEntry(KeyTypesInfo *info, KeyTypeInfo *type,
            struct xkb_key_type_entry *new, bool clobber, bool report)
{
    struct xkb_key_type_entry *old;

    old = FindMatchingMapEntry(type, new->mods.mods);
    만약 (old) {
        만약 (report && old->level != new->level) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_KEY_TYPE_MAP_ENTRY,
                     "Multiple map entries for %s in %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     MapEntryTxt(info, new), TypeTxt(info, type),
                     (clobber ? new->level : old->level) + 1,
                     (clobber ? old->level : new->level) + 1);
        }
        아니면 {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_CONFLICTING_KEY_TYPE_MAP_ENTRY,
                    "Multiple occurrences of map[%s]= %"PRIu32" in %s; Ignored\n",
                    MapEntryTxt(info, new), new->level + 1,
                    TypeTxt(info, type));
            반환 true;
        }

        만약 (clobber) {
            만약 (new->level >= type->num_levels)
                type->num_levels = new->level + 1;
            old->level = new->level;
        }

        반환 true;
    }

    만약 (new->level >= type->num_levels)
        type->num_levels = new->level + 1;

    darray_append(type->entries, *new);
    반환 true;
}

static bool
SetMapEntry(KeyTypesInfo *info, KeyTypeInfo *type, ExprDef *arrayNdx,
            ExprDef *value)
{
    struct xkb_key_type_entry entry;

    만약 (arrayNdx == NULL)
        반환 ReportTypeShouldBeArray(info, type, "map entry");

    만약 (!ExprResolveModMask(info->ctx, arrayNdx, MOD_BOTH, &info->mods,
                            &entry.mods.mods))
        반환 ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "map entry", "modifier mask");

    만약 (entry.mods.mods & (~type->mods)) {
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

    만약 (!ExprResolveLevel(info->ctx, value, &entry.level)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                          "Level specifications in a key type must be integer; "
                          "Ignoring malformed level specification\n");
        반환 false;
    }

    entry.preserve.mods = 0;

    반환 AddMapEntry(info, type, &entry, true, true);
}

/***====================================================================***/

static bool
AddPreserve(KeyTypesInfo *info, KeyTypeInfo *type,
            xkb_mod_mask_t mods, xkb_mod_mask_t preserve_mods)
{
    struct xkb_key_type_entry *entry;

    darray_foreach(entry, type->entries) {
        만약 (entry->mods.mods != mods)
            continue;

        /* Map exists without previous preserve (or "None"); override. */
        만약 (entry->preserve.mods == 0) {
            entry->preserve.mods = preserve_mods;
            반환 true;
        }

        /* Map exists with same preserve; do nothing. */
        만약 (entry->preserve.mods == preserve_mods) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_DUPLICATE_ENTRY,
                    "Identical definitions for preserve[%s] in %s; "
                    "Ignored\n",
                    ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                    TypeTxt(info, type));
            반환 true;
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
        반환 true;
    }

    /*
     * Map does not exist, i.e. preserve[] came before map[].
     * Create a map with the specified mask mapping to Level1. The level
     * may be overridden later with an explicit map[] statement.
     */
    const struct xkb_key_type_entry new = {
        .level = 0,
        .mods = { .mods = mods },
        .preserve = { .mods = preserve_mods },
    };
    darray_append(type->entries, new);
    반환 true;
}

static bool
SetPreserve(KeyTypesInfo *info, KeyTypeInfo *type, ExprDef *arrayNdx,
            ExprDef *value)
{

    만약 (arrayNdx == NULL)
        반환 ReportTypeShouldBeArray(info, type, "preserve entry");

    xkb_mod_mask_t mods = 0;
    만약 (!ExprResolveModMask(info->ctx, arrayNdx, MOD_BOTH, &info->mods, &mods))
        반환 ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "preserve entry", "modifier mask");

    만약 (mods & ~type->mods) {
        const char *before, *after;

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
    만약 (!ExprResolveModMask(info->ctx, value, MOD_BOTH, &info->mods,
                            &preserve_mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Preserve value in a key type is not a modifier mask; "
                "Ignoring preserve[%s] in type %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type));
        반환 false;
    }

    만약 (preserve_mods & ~mods) {
        const char *before, *after;

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

    반환 AddPreserve(info, type, mods, preserve_mods);
}

/***====================================================================***/

static bool
AddLevelName(KeyTypesInfo *info, KeyTypeInfo *type,
             xkb_level_index_t level, xkb_atom_t name, bool clobber)
{
    /* New name. */
    만약 (level >= darray_size(type->level_names)) {
        darray_resize0(type->level_names, level + 1);
        goto finish;
    }

    /* Same level, same name. */
    만약 (darray_item(type->level_names, level) == name) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE, XKB_WARNING_DUPLICATE_ENTRY,
                "Duplicate names for level %"PRIu32" of key type %s; Ignored\n",
                level + 1, TypeTxt(info, type));
        반환 true;
    }

    /* Same level, different name. */
    만약 (darray_item(type->level_names, level) != XKB_ATOM_NONE) {
        const char *old, *new;
        old = xkb_atom_text(info->ctx,
                            darray_item(type->level_names, level));
        new = xkb_atom_text(info->ctx, name);
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_KEY_TYPE_LEVEL_NAMES,
                "Multiple names for level %"PRIu32" of key type %s; "
                "Using %s, ignoring %s\n",
                level + 1, TypeTxt(info, type),
                (clobber ? new : old), (clobber ? old : new));

        만약 (!clobber)
            반환 true;
    }

    /* FIXME: What about different level, same name? */

finish:
    darray_item(type->level_names, level) = name;
    반환 true;
}

static bool
SetLevelName(KeyTypesInfo *info, KeyTypeInfo *type, ExprDef *arrayNdx,
             ExprDef *value)
{
    만약 (arrayNdx == NULL)
        반환 ReportTypeShouldBeArray(info, type, "level name");

    xkb_level_index_t level = 0;
    만약 (!ExprResolveLevel(info->ctx, arrayNdx, &level))
        반환 ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                                 type, "level name", "integer");

    xkb_atom_t level_name = XKB_ATOM_NONE;
    만약 (!ExprResolveString(info->ctx, value, &level_name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Non-string name for level %"PRIu32" in key type %s; "
                "Ignoring illegal level name definition\n",
                level + 1, xkb_atom_text(info->ctx, type->name));
        반환 false;
    }

    반환 AddLevelName(info, type, level, level_name, true);
}

/***====================================================================***/

static bool
SetKeyTypeField(KeyTypesInfo *info, KeyTypeInfo *type,
                const char *field, ExprDef *arrayNdx, ExprDef *value)
{
    bool ok = false;
    enum type_field type_field = 0;

    만약 (istreq(field, "modifiers")) {
        type_field = TYPE_FIELD_MASK;
        ok = SetModifiers(info, type, arrayNdx, value);
    }
    아니면 만약 (istreq(field, "map")) {
        type_field = TYPE_FIELD_MAP;
        ok = SetMapEntry(info, type, arrayNdx, value);
    }
    아니면 만약 (istreq(field, "preserve")) {
        type_field = TYPE_FIELD_PRESERVE;
        ok = SetPreserve(info, type, arrayNdx, value);
    }
    아니면 만약 (istreq(field, "levelname") || istreq(field, "level_name")) {
        type_field = TYPE_FIELD_LEVEL_NAME;
        ok = SetLevelName(info, type, arrayNdx, value);
    } 아니면 {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in key type \"%s\"; Definition ignored\n",
                field, TypeTxt(info, type));
        ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_TYPE_FIELDS);
    }

    type->defined |= type_field;
    반환 ok;
}

static bool
HandleKeyTypeBody(KeyTypesInfo *info, VarDef *def, KeyTypeInfo *type)
{
    bool ok = true;
    const char *elem, *field;
    ExprDef *arrayNdx;

    위해 (; def; def = (VarDef *) def->common.next) {
        만약 (!ExprResolveLhs(info->ctx, def->name, &elem, &field,
                            &arrayNdx)) {
            /* internal error, already reported */
            ok = false;
            continue;
        }

        만약 (elem) {
            만약 (istreq(elem, "type")) {
                log_err(info->ctx, XKB_ERROR_INVALID_SET_DEFAULT_STATEMENT,
                        "Support for changing the default type has been removed; "
                        "Statement \"%s.%s\" ignored.\n", elem, field);
            }
            아니면 {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key type statement: move statements to the global "
                        "file scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
            continue;
        }

        만약 (!SetKeyTypeField(info, type, field, arrayNdx, def->value))
            ok = false;
    }

    반환 ok;
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

    만약 (!HandleKeyTypeBody(info, def->body, &type) ||
        !AddKeyType(info, &type, true))
    {
        info->errorCount++;
        ClearKeyTypeInfo(&type);
        반환 false;
    }

    /* Type has been either stolen via shallow copy or reinitialized in
       `AddKeyType`: no need to free the arrays */
    반환 true;
}

static bool
HandleGlobalVar(KeyTypesInfo *info, VarDef *stmt)
{
    const char *elem, *field;
    ExprDef *arrayNdx;

    만약 (!ExprResolveLhs(info->ctx, stmt->name, &elem, &field,
                        &arrayNdx)) {
        반환 false;           /* internal error, already reported */
    }
    아니면 만약 (elem && istreq(elem, "type")) {
        log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                "Support for changing the default type has been removed; "
                "Statement ignored\n");
        반환 true;
    }
    아니면 만약 (elem) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown element \"%s\"; "
                "Value for field \"%s.%s\" ignored\n", elem, elem, field);
        반환 !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
    }
    아니면 만약 (field) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        반환 !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS);
    }
    반환 false;
}

static void
HandleKeyTypesFile(KeyTypesInfo *info, XkbFile *file)
{
    bool ok;

    free(info->name);
    info->name = strdup_safe(file->name);

    위해 (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        switch (stmt->type) {
        case STMT_INCLUDE:
            ok = HandleIncludeKeyTypes(info, (IncludeStmt *) stmt);
            break;
        case STMT_TYPE:
            ok = HandleKeyTypeDef(info, (KeyTypeDef *) stmt);
            break;
        case STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef*) stmt);
            break;
        case STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            break;
        case STMT_UNKNOWN_DECLARATION:
        case STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported types %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            break;
        default:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Key type files may not include other declarations; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            break;
        }

        만약 (!ok)
            info->errorCount++;

        만약 (info->errorCount > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning keytypes file \"%s\"\n",
                    safe_map_name(file->name));
            break;
        }
    }
}

/***====================================================================***/

static bool
CopyKeyTypesToKeymap(struct xkb_keymap *keymap, KeyTypesInfo *info)
{
    const darray_size_t num_types = darray_empty(info->types)
                                  ? 1
                                  : darray_size(info->types);
    struct xkb_key_type *types = calloc(num_types, sizeof(*types));
    만약 (!types)
        반환 false;

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
    만약 (darray_empty(info->types)) {
        struct xkb_key_type *type = &types[0];

        type->mods.mods = 0;
        type->num_levels = 1;
        type->entries = NULL;
        type->num_entries = 0;
        type->name = xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL");
        type->level_names = NULL;
        type->num_level_names = 0;
        type->required = true;
    }
    아니면 {
        /* HACK to circumvent Xorg bugs (see comment above) */
        const xkb_atom_t canonical_types[] = {
            xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "TWO_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "ALPHABETIC"),
            xkb_atom_intern_literal(keymap->ctx, "KEYPAD"),
        };

        위해 (darray_size_t i = 0; i < num_types; i++) {
            KeyTypeInfo *def = &darray_item(info->types, i);
            struct xkb_key_type *type = &types[i];

            type->name = def->name;
            type->mods.mods = def->mods;
            type->num_levels = def->num_levels;
            type->num_level_names =
                (xkb_level_index_t) darray_size(def->level_names);
            darray_steal(def->level_names, &type->level_names, NULL);
            darray_steal(def->entries, &type->entries, &type->num_entries);
            type->required = false;

            /* HACK: Never drop canonical XKB key types (see comment above) */
            만약 (type->num_levels <= 2) {
                위해 (uint8_t t = 0;
                     t < (uint8_t) ARRAY_SIZE(canonical_types);
                     t++) {
                        만약 (type->name == canonical_types[t]) {
                            type->required = true;
                            break;
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
    반환 true;
}

/***====================================================================***/

bool
CompileKeyTypes(XkbFile *file, struct xkb_keymap_info *keymap_info)
{
    KeyTypesInfo info;

    InitKeyTypesInfo(&info, keymap_info, 0,
                     &keymap_info->keymap.mods);

    만약 (file != NULL)
        HandleKeyTypesFile(&info, file);

    만약 (info.errorCount != 0)
        goto err_info;

    만약 (!CopyKeyTypesToKeymap(&keymap_info->keymap, &info))
        goto err_info;

    ClearKeyTypesInfo(&info);
    반환 true;

err_info:
    ClearKeyTypesInfo(&info);
    반환 false;
}
