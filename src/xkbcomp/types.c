/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#ضمن "config.h"

#ضمن <limits.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "xkbcommon/xkbcommon-status.h"
#ضمن "xkbcomp-priv.h"
#ضمن "text.h"
#ضمن "vmod.h"
#ضمن "expr.h"
#ضمن "include.h"
#ضمن "utils.h"
#ضمن "util-mem.h"

تعداد type_field {
    TYPE_FIELD_MASK = (1 << 0),
    TYPE_FIELD_MAP = (1 << 1),
    TYPE_FIELD_PRESERVE = (1 << 2),
    TYPE_FIELD_LEVEL_NAME = (1 << 3),
};

تعريفنوع بنية {
    تعداد type_field defined;
    تعداد merge_mode merge;

    xkb_atom_t name;
    xkb_mod_mask_t mods;
    xkb_level_index_t num_levels;
    darray(بنية xkb_key_type_entry) entries;
    darray(xkb_atom_t) level_names;
} KeyTypeInfo;

تعريفنوع بنية {
    حرف *name;
    صحيح errorCount;
    unsigned صحيح include_depth;

    darray(KeyTypeInfo) types;
    بنية xkb_mod_set mods;

    بنية xkb_context *ctx;
    ثابت بنية xkb_keymap_info *keymap_info;
} KeyTypesInfo;

/***====================================================================***/

ساكن مضمن ثابت حرف *
MapEntryTxt(KeyTypesInfo *info, بنية xkb_key_type_entry *entry)
{
    ܦܢܐ ModMaskText(info->ctx, MOD_BOTH, &info->mods, entry->mods.mods);
}

ساكن مضمن ثابت حرف *
TypeTxt(KeyTypesInfo *info, KeyTypeInfo *type)
{
    ܦܢܐ xkb_atom_text(info->ctx, type->name);
}

ساكن مضمن ثابت حرف *
TypeMaskTxt(KeyTypesInfo *info, KeyTypeInfo *type)
{
    ܦܢܐ ModMaskText(info->ctx, MOD_BOTH, &info->mods, type->mods);
}

ساكن مضمن bool
ReportTypeShouldBeArray(KeyTypesInfo *info, KeyTypeInfo *type,
                        ثابت حرف *field)
{
    ܦܢܐ ReportShouldBeArray(info->ctx, "key type", field,
                               TypeTxt(info, type));
}

ساكن مضمن bool
ReportTypeBadType(KeyTypesInfo *info, تعداد xkb_message_code code,
                  KeyTypeInfo *type, ثابت حرف *field, ثابت حرف *wanted)
{
    ܦܢܐ ReportBadType(info->ctx, code, "key type", field,
                         TypeTxt(info, type), wanted);
}

/***====================================================================***/

ساكن فراغ
InitKeyTypesInfo(KeyTypesInfo *info, ثابت بنية xkb_keymap_info *keymap_info,
                 unsigned صحيح include_depth,
                 ثابت بنية xkb_mod_set *mods)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    InitVMods(&info->mods, mods, include_depth > 0);
}

ساكن فراغ
ClearKeyTypeInfo(KeyTypeInfo *type)
{
    darray_free(type->entries);
    darray_free(type->level_names);
}

ساكن فراغ
ClearKeyTypesInfo(KeyTypesInfo *info)
{
    حرر(info->name);
    KeyTypeInfo *type;
    darray_foreach(type, info->types)
        ClearKeyTypeInfo(type);
    darray_free(info->types);
}

ساكن KeyTypeInfo *
FindMatchingKeyType(KeyTypesInfo *info, xkb_atom_t name)
{
    KeyTypeInfo *old;

    darray_foreach(old, info->types)
        ܐܢ (old->name == name)
            ܦܢܐ old;

    ܦܢܐ عدم;
}

ساكن bool
AddKeyType(KeyTypesInfo *info, KeyTypeInfo *new, bool same_file)
{
    KeyTypeInfo *old;
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);

    old = FindMatchingKeyType(info, new->name);
    ܐܢ (old) {
        ܐܢ (new->merge != MERGE_AUGMENT) {
            ܐܢ ((same_file && verbosity > 0) || verbosity > 9) {
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
            ܦܢܐ true;
        }

        ܐܢ (same_file)
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_CONFLICTING_KEY_TYPE_DEFINITIONS,
                    "Multiple definitions of the %s key type; "
                    "Later definition ignored\n",
                    xkb_atom_text(info->ctx, new->name));

        ClearKeyTypeInfo(new);
        ܦܢܐ true;
    }

    darray_append(info->types, *new);
    ܦܢܐ true;
}

/***====================================================================***/

ساكن فراغ
MergeIncludedKeyTypes(KeyTypesInfo *into, KeyTypesInfo *from,
                      تعداد merge_mode merge)
{
    ܐܢ (from->errorCount > 0) {
        into->errorCount += from->errorCount;
        ܦܢܐ;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    ܐܢ (into->name == عدم) {
        into->name = steal(&from->name);
    }

    ܐܢ (darray_empty(into->types)) {
        into->types = from->types;
        /* Types stolen via shallow copy, so reinitialize the array */
        darray_init(from->types);
    }
    ܐܚܪܢܐ {
        KeyTypeInfo *type;
        darray_foreach(type, from->types) {
            type->merge = merge;
            ܐܢ (!AddKeyType(into, type, false))
                into->errorCount++;
        }
        /* Types were either shallow copied or reinitialized individually
           in `AddKeyType`, so we only need to free the array */
        darray_free(from->types);
    }
}

ساكن فراغ
HandleKeyTypesFile(KeyTypesInfo *info, XkbFile *file);

ساكن bool
HandleIncludeKeyTypes(KeyTypesInfo *info, IncludeStmt *include)
{
    KeyTypesInfo included;

    ܐܢ (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->errorCount += 10;
        ܦܢܐ false;
    }

    InitKeyTypesInfo(&included, info->keymap_info, info->include_depth + 1,
                     &info->mods);
    included.name = steal(&include->stmt);

    ثابت بنية parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    لكل (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        KeyTypesInfo next_incl;
        XkbFile *file;

        حرف path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_TYPES,
                                  path, حجم(path));
        ܐܢ (!file) {
            info->errorCount += 10;
            ClearKeyTypesInfo(&included);
            ܦܢܐ false;
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

    ܦܢܐ (info->errorCount == 0);
}

/***====================================================================***/

ساكن bool
SetModifiers(KeyTypesInfo *info, KeyTypeInfo *type, ExprDef *arrayNdx,
             ExprDef *value)
{
    xkb_mod_mask_t mods = 0;

    ܐܢ (arrayNdx) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The modifiers field of a key type is not an array; "
                "Illegal array subscript ignored\n");
        ܦܢܐ false;
    }

    ܐܢ (!ExprResolveModMask(info->ctx, value, MOD_BOTH, &info->mods, &mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Key type mask field must be a modifier mask; "
                "Key type definition ignored\n");
        ܦܢܐ false;
    }

    ܐܢ (type->defined & TYPE_FIELD_MASK) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple modifier mask definitions for key type %s; "
                 "Using %s, ignoring %s\n",
                 xkb_atom_text(info->ctx, type->name),
                 TypeMaskTxt(info, type),
                 ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods));
        ܦܢܐ false;
    }

    type->mods = mods;
    ܦܢܐ true;
}

/***====================================================================***/

ساكن بنية xkb_key_type_entry *
FindMatchingMapEntry(KeyTypeInfo *type, xkb_mod_mask_t mods)
{
    بنية xkb_key_type_entry *entry;

    darray_foreach(entry, type->entries)
        ܐܢ (entry->mods.mods == mods)
            ܦܢܐ entry;

    ܦܢܐ عدم;
}

ساكن bool
AddMapEntry(KeyTypesInfo *info, KeyTypeInfo *type,
            بنية xkb_key_type_entry *new, bool clobber, bool report)
{
    بنية xkb_key_type_entry *old;

    old = FindMatchingMapEntry(type, new->mods.mods);
    ܐܢ (old) {
        ܐܢ (report && old->level != new->level) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_KEY_TYPE_MAP_ENTRY,
                     "Multiple map entries for %s in %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     MapEntryTxt(info, new), TypeTxt(info, type),
                     (clobber ? new->level : old->level) + 1,
                     (clobber ? old->level : new->level) + 1);
        }
        ܐܚܪܢܐ {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_CONFLICTING_KEY_TYPE_MAP_ENTRY,
                    "Multiple occurrences of map[%s]= %"PRIu32" in %s; Ignored\n",
                    MapEntryTxt(info, new), new->level + 1,
                    TypeTxt(info, type));
            ܦܢܐ true;
        }

        ܐܢ (clobber) {
            ܐܢ (new->level >= type->num_levels)
                type->num_levels = new->level + 1;
            old->level = new->level;
        }

        ܦܢܐ true;
    }

    ܐܢ (new->level >= type->num_levels)
        type->num_levels = new->level + 1;

    darray_append(type->entries, *new);
    ܦܢܐ true;
}

ساكن bool
SetMapEntry(KeyTypesInfo *info, KeyTypeInfo *type, ExprDef *arrayNdx,
            ExprDef *value)
{
    بنية xkb_key_type_entry entry;

    ܐܢ (arrayNdx == عدم)
        ܦܢܐ ReportTypeShouldBeArray(info, type, "map entry");

    ܐܢ (!ExprResolveModMask(info->ctx, arrayNdx, MOD_BOTH, &info->mods,
                            &entry.mods.mods))
        ܦܢܐ ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "map entry", "modifier mask");

    ܐܢ (entry.mods.mods & (~type->mods)) {
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

    ܐܢ (!ExprResolveLevel(info->ctx, value, &entry.level)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                          "Level specifications in a key type must be integer; "
                          "Ignoring malformed level specification\n");
        ܦܢܐ false;
    }

    entry.preserve.mods = 0;

    ܦܢܐ AddMapEntry(info, type, &entry, true, true);
}

/***====================================================================***/

ساكن bool
AddPreserve(KeyTypesInfo *info, KeyTypeInfo *type,
            xkb_mod_mask_t mods, xkb_mod_mask_t preserve_mods)
{
    بنية xkb_key_type_entry *entry;

    darray_foreach(entry, type->entries) {
        ܐܢ (entry->mods.mods != mods)
            استمر;

        /* Map exists without previous preserve (or "None"); override. */
        ܐܢ (entry->preserve.mods == 0) {
            entry->preserve.mods = preserve_mods;
            ܦܢܐ true;
        }

        /* Map exists with same preserve; do nothing. */
        ܐܢ (entry->preserve.mods == preserve_mods) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_DUPLICATE_ENTRY,
                    "Identical definitions for preserve[%s] in %s; "
                    "Ignored\n",
                    ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                    TypeTxt(info, type));
            ܦܢܐ true;
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
        ܦܢܐ true;
    }

    /*
     * Map does not exist, i.e. preserve[] came before map[].
     * Create a map with the specified mask mapping to Level1. The level
     * may be overridden later with an explicit map[] statement.
     */
    ثابت بنية xkb_key_type_entry new = {
        .level = 0,
        .mods = { .mods = mods },
        .preserve = { .mods = preserve_mods },
    };
    darray_append(type->entries, new);
    ܦܢܐ true;
}

ساكن bool
SetPreserve(KeyTypesInfo *info, KeyTypeInfo *type, ExprDef *arrayNdx,
            ExprDef *value)
{

    ܐܢ (arrayNdx == عدم)
        ܦܢܐ ReportTypeShouldBeArray(info, type, "preserve entry");

    xkb_mod_mask_t mods = 0;
    ܐܢ (!ExprResolveModMask(info->ctx, arrayNdx, MOD_BOTH, &info->mods, &mods))
        ܦܢܐ ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "preserve entry", "modifier mask");

    ܐܢ (mods & ~type->mods) {
        ثابت حرف *before, *after;

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
    ܐܢ (!ExprResolveModMask(info->ctx, value, MOD_BOTH, &info->mods,
                            &preserve_mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Preserve value in a key type is not a modifier mask; "
                "Ignoring preserve[%s] in type %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type));
        ܦܢܐ false;
    }

    ܐܢ (preserve_mods & ~mods) {
        ثابت حرف *before, *after;

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

    ܦܢܐ AddPreserve(info, type, mods, preserve_mods);
}

/***====================================================================***/

ساكن bool
AddLevelName(KeyTypesInfo *info, KeyTypeInfo *type,
             xkb_level_index_t level, xkb_atom_t name, bool clobber)
{
    /* New name. */
    ܐܢ (level >= darray_size(type->level_names)) {
        darray_resize0(type->level_names, level + 1);
        اذهب finish;
    }

    /* Same level, same name. */
    ܐܢ (darray_item(type->level_names, level) == name) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE, XKB_WARNING_DUPLICATE_ENTRY,
                "Duplicate names for level %"PRIu32" of key type %s; Ignored\n",
                level + 1, TypeTxt(info, type));
        ܦܢܐ true;
    }

    /* Same level, different name. */
    ܐܢ (darray_item(type->level_names, level) != XKB_ATOM_NONE) {
        ثابت حرف *old, *new;
        old = xkb_atom_text(info->ctx,
                            darray_item(type->level_names, level));
        new = xkb_atom_text(info->ctx, name);
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_KEY_TYPE_LEVEL_NAMES,
                "Multiple names for level %"PRIu32" of key type %s; "
                "Using %s, ignoring %s\n",
                level + 1, TypeTxt(info, type),
                (clobber ? new : old), (clobber ? old : new));

        ܐܢ (!clobber)
            ܦܢܐ true;
    }

    /* FIXME: What about different level, same name? */

finish:
    darray_item(type->level_names, level) = name;
    ܦܢܐ true;
}

ساكن bool
SetLevelName(KeyTypesInfo *info, KeyTypeInfo *type, ExprDef *arrayNdx,
             ExprDef *value)
{
    ܐܢ (arrayNdx == عدم)
        ܦܢܐ ReportTypeShouldBeArray(info, type, "level name");

    xkb_level_index_t level = 0;
    ܐܢ (!ExprResolveLevel(info->ctx, arrayNdx, &level))
        ܦܢܐ ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                                 type, "level name", "integer");

    xkb_atom_t level_name = XKB_ATOM_NONE;
    ܐܢ (!ExprResolveString(info->ctx, value, &level_name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Non-string name for level %"PRIu32" in key type %s; "
                "Ignoring illegal level name definition\n",
                level + 1, xkb_atom_text(info->ctx, type->name));
        ܦܢܐ false;
    }

    ܦܢܐ AddLevelName(info, type, level, level_name, true);
}

/***====================================================================***/

ساكن bool
SetKeyTypeField(KeyTypesInfo *info, KeyTypeInfo *type,
                ثابت حرف *field, ExprDef *arrayNdx, ExprDef *value)
{
    bool ok = false;
    تعداد type_field type_field = 0;

    ܐܢ (istreq(field, "modifiers")) {
        type_field = TYPE_FIELD_MASK;
        ok = SetModifiers(info, type, arrayNdx, value);
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "map")) {
        type_field = TYPE_FIELD_MAP;
        ok = SetMapEntry(info, type, arrayNdx, value);
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "preserve")) {
        type_field = TYPE_FIELD_PRESERVE;
        ok = SetPreserve(info, type, arrayNdx, value);
    }
    ܐܚܪܢܐ ܐܢ (istreq(field, "levelname") || istreq(field, "level_name")) {
        type_field = TYPE_FIELD_LEVEL_NAME;
        ok = SetLevelName(info, type, arrayNdx, value);
    } ܐܚܪܢܐ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in key type \"%s\"; Definition ignored\n",
                field, TypeTxt(info, type));
        ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_TYPE_FIELDS);
    }

    type->defined |= type_field;
    ܦܢܐ ok;
}

ساكن bool
HandleKeyTypeBody(KeyTypesInfo *info, VarDef *def, KeyTypeInfo *type)
{
    bool ok = true;
    ثابت حرف *elem, *field;
    ExprDef *arrayNdx;

    لكل (; def; def = (VarDef *) def->common.next) {
        ܐܢ (!ExprResolveLhs(info->ctx, def->name, &elem, &field,
                            &arrayNdx)) {
            /* internal error, already reported */
            ok = false;
            استمر;
        }

        ܐܢ (elem) {
            ܐܢ (istreq(elem, "type")) {
                log_err(info->ctx, XKB_ERROR_INVALID_SET_DEFAULT_STATEMENT,
                        "Support for changing the default type has been removed; "
                        "Statement \"%s.%s\" ignored.\n", elem, field);
            }
            ܐܚܪܢܐ {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key type statement: move statements to the global "
                        "file scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
            استمر;
        }

        ܐܢ (!SetKeyTypeField(info, type, field, arrayNdx, def->value))
            ok = false;
    }

    ܦܢܐ ok;
}

ساكن bool
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

    ܐܢ (!HandleKeyTypeBody(info, def->body, &type) ||
        !AddKeyType(info, &type, true))
    {
        info->errorCount++;
        ClearKeyTypeInfo(&type);
        ܦܢܐ false;
    }

    /* Type has been either stolen via shallow copy or reinitialized in
       `AddKeyType`: no need to free the arrays */
    ܦܢܐ true;
}

ساكن bool
HandleGlobalVar(KeyTypesInfo *info, VarDef *stmt)
{
    ثابت حرف *elem, *field;
    ExprDef *arrayNdx;

    ܐܢ (!ExprResolveLhs(info->ctx, stmt->name, &elem, &field,
                        &arrayNdx)) {
        ܦܢܐ false;           /* internal error, already reported */
    }
    ܐܚܪܢܐ ܐܢ (elem && istreq(elem, "type")) {
        log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                "Support for changing the default type has been removed; "
                "Statement ignored\n");
        ܦܢܐ true;
    }
    ܐܚܪܢܐ ܐܢ (elem) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown element \"%s\"; "
                "Value for field \"%s.%s\" ignored\n", elem, elem, field);
        ܦܢܐ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
    }
    ܐܚܪܢܐ ܐܢ (field) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        ܦܢܐ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS);
    }
    ܦܢܐ false;
}

ساكن فراغ
HandleKeyTypesFile(KeyTypesInfo *info, XkbFile *file)
{
    bool ok;

    حرر(info->name);
    info->name = strdup_safe(file->name);

    لكل (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        بدل (stmt->type) {
        حالة STMT_INCLUDE:
            ok = HandleIncludeKeyTypes(info, (IncludeStmt *) stmt);
            اكسر;
        حالة STMT_TYPE:
            ok = HandleKeyTypeDef(info, (KeyTypeDef *) stmt);
            اكسر;
        حالة STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef*) stmt);
            اكسر;
        حالة STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            اكسر;
        حالة STMT_UNKNOWN_DECLARATION:
        حالة STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported types %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            اكسر;
        افتراضي:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Key type files may not include other declarations; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            اكسر;
        }

        ܐܢ (!ok)
            info->errorCount++;

        ܐܢ (info->errorCount > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning keytypes file \"%s\"\n",
                    safe_map_name(file->name));
            اكسر;
        }
    }
}

/***====================================================================***/

ساكن bool
CopyKeyTypesToKeymap(بنية xkb_keymap *keymap, KeyTypesInfo *info)
{
    ثابت darray_size_t num_types = darray_empty(info->types)
                                  ? 1
                                  : darray_size(info->types);
    بنية xkb_key_type *types = calloc(num_types, حجم(*types));
    ܐܢ (!types)
        ܦܢܐ false;

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
    ܐܢ (darray_empty(info->types)) {
        بنية xkb_key_type *type = &types[0];

        type->mods.mods = 0;
        type->num_levels = 1;
        type->entries = عدم;
        type->num_entries = 0;
        type->name = xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL");
        type->level_names = عدم;
        type->num_level_names = 0;
        type->required = true;
    }
    ܐܚܪܢܐ {
        /* HACK to circumvent Xorg bugs (see comment above) */
        ثابت xkb_atom_t canonical_types[] = {
            xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "TWO_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "ALPHABETIC"),
            xkb_atom_intern_literal(keymap->ctx, "KEYPAD"),
        };

        لكل (darray_size_t i = 0; i < num_types; i++) {
            KeyTypeInfo *def = &darray_item(info->types, i);
            بنية xkb_key_type *type = &types[i];

            type->name = def->name;
            type->mods.mods = def->mods;
            type->num_levels = def->num_levels;
            type->num_level_names =
                (xkb_level_index_t) darray_size(def->level_names);
            darray_steal(def->level_names, &type->level_names, عدم);
            darray_steal(def->entries, &type->entries, &type->num_entries);
            type->required = false;

            /* HACK: Never drop canonical XKB key types (see comment above) */
            ܐܢ (type->num_levels <= 2) {
                لكل (uint8_t t = 0;
                     t < (uint8_t) ARRAY_SIZE(canonical_types);
                     t++) {
                        ܐܢ (type->name == canonical_types[t]) {
                            type->required = true;
                            اكسر;
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
    ܦܢܐ true;
}

/***====================================================================***/

bool
CompileKeyTypes(XkbFile *file, بنية xkb_keymap_info *keymap_info)
{
    KeyTypesInfo info;

    InitKeyTypesInfo(&info, keymap_info, 0,
                     &keymap_info->keymap.mods);

    ܐܢ (file != عدم)
        HandleKeyTypesFile(&info, file);

    ܐܢ (info.errorCount != 0)
        اذهب err_info;

    ܐܢ (!CopyKeyTypesToKeymap(&keymap_info->keymap, &info))
        اذهب err_info;

    ClearKeyTypesInfo(&info);
    ܦܢܐ true;

err_info:
    ClearKeyTypesInfo(&info);
    ܦܢܐ false;
}
