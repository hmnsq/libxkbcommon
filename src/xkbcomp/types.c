/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#شامل "config.h"

#شامل <limits.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "xkbcommon/xkbcommon-status.h"
#شامل "xkbcomp-priv.h"
#شامل "text.h"
#شامل "vmod.h"
#شامل "expr.h"
#شامل "include.h"
#شامل "utils.h"
#شامل "util-mem.h"

شمارش type_field {
    TYPE_FIELD_MASK = (1 << 0),
    TYPE_FIELD_MAP = (1 << 1),
    TYPE_FIELD_PRESERVE = (1 << 2),
    TYPE_FIELD_LEVEL_NAME = (1 << 3),
};

تعریفنوع ساختار {
    شمارش type_field defined;
    شمارش merge_mode merge;

    xkb_atom_t name;
    xkb_mod_mask_t mods;
    xkb_level_index_t رقم_levels;
    darray(ساختار xkb_کلید_type_entry) entries;
    darray(xkb_atom_t) level_names;
} کلید_Type_Info;

تعریفنوع ساختار {
    نویسه *name;
    صحیح error_شمار;
    unsigned صحیح include_depth;

    darray(کلید_Type_Info) types;
    ساختار xkb_mod_set mods;

    ساختار xkb_context *ctx;
    ثابت ساختار xkb_keymap_info *keymap_info;
} کلید_Types_Info;

/***====================================================================***/

ایستا درونخطی ثابت نویسه *
MapEntryTxt(کلید_Types_Info *info, ساختار xkb_کلید_type_entry *entry)
{
    بازگشت ModMaskText(info->ctx, MOD_BOTH, &info->mods, entry->mods.mods);
}

ایستا درونخطی ثابت نویسه *
TypeTxt(کلید_Types_Info *info, کلید_Type_Info *type)
{
    بازگشت xkb_atom_text(info->ctx, type->name);
}

ایستا درونخطی ثابت نویسه *
TypeMaskTxt(کلید_Types_Info *info, کلید_Type_Info *type)
{
    بازگشت ModMaskText(info->ctx, MOD_BOTH, &info->mods, type->mods);
}

ایستا درونخطی bool
ReportTypeShouldBeArray(کلید_Types_Info *info, کلید_Type_Info *type,
                        ثابت نویسه *field)
{
    بازگشت ReportShouldBeArray(info->ctx, "key type", field,
                               TypeTxt(info, type));
}

ایستا درونخطی bool
ReportTypeBadType(کلید_Types_Info *info, شمارش xkb_message_code code,
                  کلید_Type_Info *type, ثابت نویسه *field, ثابت نویسه *wanted)
{
    بازگشت ReportBadType(info->ctx, code, "key type", field,
                         TypeTxt(info, type), wanted);
}

/***====================================================================***/

ایستا تهی
Init_کلید_Types_Info(کلید_Types_Info *info, ثابت ساختار xkb_keymap_info *keymap_info,
                 unsigned صحیح include_depth,
                 ثابت ساختار xkb_mod_set *mods)
{
    memset(info, 0, اندازه(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    InitVMods(&info->mods, mods, include_depth > 0);
}

ایستا تهی
Clear_کلید_Type_Info(کلید_Type_Info *type)
{
    darray_free(type->entries);
    darray_free(type->level_names);
}

ایستا تهی
Clear_کلید_Types_Info(کلید_Types_Info *info)
{
    آزاد(info->name);
    کلید_Type_Info *type;
    darray_foreach(type, info->types)
        Clear_کلید_Type_Info(type);
    darray_free(info->types);
}

ایستا کلید_Type_Info *
Find_Matching_کلید_Type(کلید_Types_Info *info, xkb_atom_t name)
{
    کلید_Type_Info *old;

    darray_foreach(old, info->types)
        اگر (old->name == name)
            بازگشت old;

    بازگشت پوچ;
}

ایستا bool
Add_کلید_Type(کلید_Types_Info *info, کلید_Type_Info *new, bool same_file)
{
    کلید_Type_Info *old;
    ثابت صحیح verbosity = xkb_context_get_log_verbosity(info->ctx);

    old = Find_Matching_کلید_Type(info, new->name);
    اگر (old) {
        اگر (new->merge != MERGE_AUGMENT) {
            اگر ((same_file && verbosity > 0) || verbosity > 9) {
                log_warn(info->ctx,
                         XKB_WARNING_CONFLICTING_کلید_TYPE_DEFINITIONS,
                         "Multiple definitions of the %s key type; "
                         "Earlier definition ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            }

            Clear_کلید_Type_Info(old);
            *old = *new;
            darray_init(new->entries);
            darray_init(new->level_names);
            بازگشت true;
        }

        اگر (same_file)
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_CONFLICTING_کلید_TYPE_DEFINITIONS,
                    "Multiple definitions of the %s key type; "
                    "Later definition ignored\n",
                    xkb_atom_text(info->ctx, new->name));

        Clear_کلید_Type_Info(new);
        بازگشت true;
    }

    darray_append(info->types, *new);
    بازگشت true;
}

/***====================================================================***/

ایستا تهی
Merge_Included_کلید_Types(کلید_Types_Info *into, کلید_Types_Info *from,
                      شمارش merge_mode merge)
{
    اگر (from->error_شمار > 0) {
        into->error_شمار += from->error_شمار;
        بازگشت;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    اگر (into->name == پوچ) {
        into->name = steal(&from->name);
    }

    اگر (darray_empty(into->types)) {
        into->types = from->types;
        /* Types stolen via shallow copy, so reinitialize the array */
        darray_init(from->types);
    }
    وگرنه {
        کلید_Type_Info *type;
        darray_foreach(type, from->types) {
            type->merge = merge;
            اگر (!Add_کلید_Type(into, type, false))
                into->error_شمار++;
        }
        /* Types were either shallow copied or reinitialized individually
           in `AddKeyType`, so we only need to free the array */
        darray_free(from->types);
    }
}

ایستا تهی
Handle_کلید_Types_File(کلید_Types_Info *info, XkbFile *file);

ایستا bool
Handle_Include_کلید_Types(کلید_Types_Info *info, IncludeStmt *include)
{
    کلید_Types_Info included;

    اگر (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_شمار += 10;
        بازگشت false;
    }

    Init_کلید_Types_Info(&included, info->keymap_info, info->include_depth + 1,
                     &info->mods);
    included.name = steal(&include->stmt);

    ثابت ساختار parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    برای (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        کلید_Types_Info next_incl;
        XkbFile *file;

        نویسه path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_TYPES,
                                  path, اندازه(path));
        اگر (!file) {
            info->error_شمار += 10;
            Clear_کلید_Types_Info(&included);
            بازگشت false;
        }

        Init_کلید_Types_Info(&next_incl, info->keymap_info, info->include_depth + 1,
                         &included.mods);

        Handle_کلید_Types_File(&next_incl, file);

        Merge_Included_کلید_Types(&included, &next_incl, stmt->merge);

        Clear_کلید_Types_Info(&next_incl);
        FreeXkbFile(file);
    }

    Merge_Included_کلید_Types(info, &included, include->merge);
    Clear_کلید_Types_Info(&included);

    بازگشت (info->error_شمار == 0);
}

/***====================================================================***/

ایستا bool
SetModifiers(کلید_Types_Info *info, کلید_Type_Info *type, عبارت_Def *arrayNdx,
             عبارت_Def *مقدار)
{
    xkb_mod_mask_t mods = 0;

    اگر (arrayNdx) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The modifiers field of a key type is not an array; "
                "Illegal array subscript ignored\n");
        بازگشت false;
    }

    اگر (!عبارت_Resolve_Mod_Mask(info->ctx, مقدار, MOD_BOTH, &info->mods, &mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Key type mask field must be a modifier mask; "
                "Key type definition ignored\n");
        بازگشت false;
    }

    اگر (type->defined & TYPE_FIELD_MASK) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple modifier mask definitions for key type %s; "
                 "Using %s, ignoring %s\n",
                 xkb_atom_text(info->ctx, type->name),
                 TypeMaskTxt(info, type),
                 ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods));
        بازگشت false;
    }

    type->mods = mods;
    بازگشت true;
}

/***====================================================================***/

ایستا ساختار xkb_کلید_type_entry *
FindMatchingMapEntry(کلید_Type_Info *type, xkb_mod_mask_t mods)
{
    ساختار xkb_کلید_type_entry *entry;

    darray_foreach(entry, type->entries)
        اگر (entry->mods.mods == mods)
            بازگشت entry;

    بازگشت پوچ;
}

ایستا bool
AddMapEntry(کلید_Types_Info *info, کلید_Type_Info *type,
            ساختار xkb_کلید_type_entry *new, bool clobber, bool report)
{
    ساختار xkb_کلید_type_entry *old;

    old = FindMatchingMapEntry(type, new->mods.mods);
    اگر (old) {
        اگر (report && old->level != new->level) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_TYPE_MAP_ENTRY,
                     "Multiple map entries for %s in %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     MapEntryTxt(info, new), TypeTxt(info, type),
                     (clobber ? new->level : old->level) + 1,
                     (clobber ? old->level : new->level) + 1);
        }
        وگرنه {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_CONFLICTING_کلید_TYPE_MAP_ENTRY,
                    "Multiple occurrences of map[%s]= %"PRIu32" in %s; Ignored\n",
                    MapEntryTxt(info, new), new->level + 1,
                    TypeTxt(info, type));
            بازگشت true;
        }

        اگر (clobber) {
            اگر (new->level >= type->رقم_levels)
                type->رقم_levels = new->level + 1;
            old->level = new->level;
        }

        بازگشت true;
    }

    اگر (new->level >= type->رقم_levels)
        type->رقم_levels = new->level + 1;

    darray_append(type->entries, *new);
    بازگشت true;
}

ایستا bool
SetMapEntry(کلید_Types_Info *info, کلید_Type_Info *type, عبارت_Def *arrayNdx,
            عبارت_Def *مقدار)
{
    ساختار xkb_کلید_type_entry entry;

    اگر (arrayNdx == پوچ)
        بازگشت ReportTypeShouldBeArray(info, type, "map entry");

    اگر (!عبارت_Resolve_Mod_Mask(info->ctx, arrayNdx, MOD_BOTH, &info->mods,
                            &entry.mods.mods))
        بازگشت ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "map entry", "modifier mask");

    اگر (entry.mods.mods & (~type->mods)) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_IN_کلید_TYPE,
                "Map entry for modifiers not used by type %s; "
                "Using %s instead of %s\n",
                TypeTxt(info, type),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods,
                            entry.mods.mods & type->mods),
                MapEntryTxt(info, &entry));
        entry.mods.mods &= type->mods;
    }

    اگر (!عبارت_Resolve_Level(info->ctx, مقدار, &entry.level)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                          "Level specifications in a key type must be integer; "
                          "Ignoring malformed level specification\n");
        بازگشت false;
    }

    entry.preserve.mods = 0;

    بازگشت AddMapEntry(info, type, &entry, true, true);
}

/***====================================================================***/

ایستا bool
AddPreserve(کلید_Types_Info *info, کلید_Type_Info *type,
            xkb_mod_mask_t mods, xkb_mod_mask_t preserve_mods)
{
    ساختار xkb_کلید_type_entry *entry;

    darray_foreach(entry, type->entries) {
        اگر (entry->mods.mods != mods)
            ادامه;

        /* Map exists without previous preserve (or "None"); override. */
        اگر (entry->preserve.mods == 0) {
            entry->preserve.mods = preserve_mods;
            بازگشت true;
        }

        /* Map exists with same preserve; do nothing. */
        اگر (entry->preserve.mods == preserve_mods) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_DUPLICATE_ENTRY,
                    "Identical definitions for preserve[%s] in %s; "
                    "Ignored\n",
                    ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                    TypeTxt(info, type));
            بازگشت true;
        }

        /* Map exists with different preserve; latter wins. */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_کلید_TYPE_PRESERVE_ENTRIES,
                "Multiple definitions for preserve[%s] in %s; "
                "Using %s, ignoring %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods,
                            entry->preserve.mods));

        entry->preserve.mods = preserve_mods;
        بازگشت true;
    }

    /*
     * Map does not exist, i.e. preserve[] came before map[].
     * Create a map with the specified mask mapping to Level1. The level
     * may be overridden later with an explicit map[] statement.
     */
    ثابت ساختار xkb_کلید_type_entry new = {
        .level = 0,
        .mods = { .mods = mods },
        .preserve = { .mods = preserve_mods },
    };
    darray_append(type->entries, new);
    بازگشت true;
}

ایستا bool
SetPreserve(کلید_Types_Info *info, کلید_Type_Info *type, عبارت_Def *arrayNdx,
            عبارت_Def *مقدار)
{

    اگر (arrayNdx == پوچ)
        بازگشت ReportTypeShouldBeArray(info, type, "preserve entry");

    xkb_mod_mask_t mods = 0;
    اگر (!عبارت_Resolve_Mod_Mask(info->ctx, arrayNdx, MOD_BOTH, &info->mods, &mods))
        بازگشت ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "preserve entry", "modifier mask");

    اگر (mods & ~type->mods) {
        ثابت نویسه *before, *after;

        before = ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods);
        mods &= type->mods;
        after = ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_IN_کلید_TYPE,
                "Preserve entry for modifiers not used by the %s type; "
                "Index %s converted to %s\n",
                TypeTxt(info, type), before, after);
    }

    xkb_mod_mask_t preserve_mods = 0;
    اگر (!عبارت_Resolve_Mod_Mask(info->ctx, مقدار, MOD_BOTH, &info->mods,
                            &preserve_mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Preserve value in a key type is not a modifier mask; "
                "Ignoring preserve[%s] in type %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type));
        بازگشت false;
    }

    اگر (preserve_mods & ~mods) {
        ثابت نویسه *before, *after;

        before = ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods);
        preserve_mods &= mods;
        after = ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_ILLEGAL_کلید_TYPE_PRESERVE_RESULT,
                "Illegal value for preserve[%s] in type %s; "
                "Converted %s to %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type), before, after);
    }

    بازگشت AddPreserve(info, type, mods, preserve_mods);
}

/***====================================================================***/

ایستا bool
AddLevelName(کلید_Types_Info *info, کلید_Type_Info *type,
             xkb_level_index_t level, xkb_atom_t name, bool clobber)
{
    /* New name. */
    اگر (level >= darray_size(type->level_names)) {
        darray_resize0(type->level_names, level + 1);
        برو finish;
    }

    /* Same level, same name. */
    اگر (darray_item(type->level_names, level) == name) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE, XKB_WARNING_DUPLICATE_ENTRY,
                "Duplicate names for level %"PRIu32" of key type %s; Ignored\n",
                level + 1, TypeTxt(info, type));
        بازگشت true;
    }

    /* Same level, different name. */
    اگر (darray_item(type->level_names, level) != XKB_ATOM_NONE) {
        ثابت نویسه *old, *new;
        old = xkb_atom_text(info->ctx,
                            darray_item(type->level_names, level));
        new = xkb_atom_text(info->ctx, name);
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_کلید_TYPE_LEVEL_NAMES,
                "Multiple names for level %"PRIu32" of key type %s; "
                "Using %s, ignoring %s\n",
                level + 1, TypeTxt(info, type),
                (clobber ? new : old), (clobber ? old : new));

        اگر (!clobber)
            بازگشت true;
    }

    /* FIXME: What about different level, same name? */

finish:
    darray_item(type->level_names, level) = name;
    بازگشت true;
}

ایستا bool
SetLevelName(کلید_Types_Info *info, کلید_Type_Info *type, عبارت_Def *arrayNdx,
             عبارت_Def *مقدار)
{
    اگر (arrayNdx == پوچ)
        بازگشت ReportTypeShouldBeArray(info, type, "level name");

    xkb_level_index_t level = 0;
    اگر (!عبارت_Resolve_Level(info->ctx, arrayNdx, &level))
        بازگشت ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                                 type, "level name", "integer");

    xkb_atom_t level_name = XKB_ATOM_NONE;
    اگر (!عبارت_Resolve_String(info->ctx, مقدار, &level_name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Non-string name for level %"PRIu32" in key type %s; "
                "Ignoring illegal level name definition\n",
                level + 1, xkb_atom_text(info->ctx, type->name));
        بازگشت false;
    }

    بازگشت AddLevelName(info, type, level, level_name, true);
}

/***====================================================================***/

ایستا bool
Set_کلید_Type_Field(کلید_Types_Info *info, کلید_Type_Info *type,
                ثابت نویسه *field, عبارت_Def *arrayNdx, عبارت_Def *مقدار)
{
    bool ok = false;
    شمارش type_field type_field = 0;

    اگر (istreq(field, "modifiers")) {
        type_field = TYPE_FIELD_MASK;
        ok = SetModifiers(info, type, arrayNdx, مقدار);
    }
    وگرنه اگر (istreq(field, "map")) {
        type_field = TYPE_FIELD_MAP;
        ok = SetMapEntry(info, type, arrayNdx, مقدار);
    }
    وگرنه اگر (istreq(field, "preserve")) {
        type_field = TYPE_FIELD_PRESERVE;
        ok = SetPreserve(info, type, arrayNdx, مقدار);
    }
    وگرنه اگر (istreq(field, "levelname") || istreq(field, "level_name")) {
        type_field = TYPE_FIELD_LEVEL_NAME;
        ok = SetLevelName(info, type, arrayNdx, مقدار);
    } وگرنه {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in key type \"%s\"; Definition ignored\n",
                field, TypeTxt(info, type));
        ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_TYPE_FIELDS);
    }

    type->defined |= type_field;
    بازگشت ok;
}

ایستا bool
Handle_کلید_Type_Body(کلید_Types_Info *info, VarDef *def, کلید_Type_Info *type)
{
    bool ok = true;
    ثابت نویسه *elem, *field;
    عبارت_Def *arrayNdx;

    برای (; def; def = (VarDef *) def->common.next) {
        اگر (!عبارت_Resolve_Lhs(info->ctx, def->name, &elem, &field,
                            &arrayNdx)) {
            /* internal error, already reported */
            ok = false;
            ادامه;
        }

        اگر (elem) {
            اگر (istreq(elem, "type")) {
                log_err(info->ctx, XKB_ERROR_INVALID_SET_DEFAULT_STATEMENT,
                        "Support for changing the default type has been removed; "
                        "Statement \"%s.%s\" ignored.\n", elem, field);
            }
            وگرنه {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key type statement: move statements to the global "
                        "file scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
            ادامه;
        }

        اگر (!Set_کلید_Type_Field(info, type, field, arrayNdx, def->مقدار))
            ok = false;
    }

    بازگشت ok;
}

ایستا bool
Handle_کلید_Type_Def(کلید_Types_Info *info, کلید_Type_Def *def)
{
    کلید_Type_Info type = {
        .defined = 0,
        .merge = def->merge,
        .name = def->name,
        .mods = 0,
        .رقم_levels = 1,
        .entries = darray_new(),
        .level_names = darray_new(),
    };

    اگر (!Handle_کلید_Type_Body(info, def->body, &type) ||
        !Add_کلید_Type(info, &type, true))
    {
        info->error_شمار++;
        Clear_کلید_Type_Info(&type);
        بازگشت false;
    }

    /* Type has been either stolen via shallow copy or reinitialized in
       `AddKeyType`: no need to free the arrays */
    بازگشت true;
}

ایستا bool
HandleGlobalVar(کلید_Types_Info *info, VarDef *stmt)
{
    ثابت نویسه *elem, *field;
    عبارت_Def *arrayNdx;

    اگر (!عبارت_Resolve_Lhs(info->ctx, stmt->name, &elem, &field,
                        &arrayNdx)) {
        بازگشت false;           /* internal error, already reported */
    }
    وگرنه اگر (elem && istreq(elem, "type")) {
        log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                "Support for changing the default type has been removed; "
                "Statement ignored\n");
        بازگشت true;
    }
    وگرنه اگر (elem) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown element \"%s\"; "
                "Value for field \"%s.%s\" ignored\n", elem, elem, field);
        بازگشت !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
    }
    وگرنه اگر (field) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        بازگشت !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS);
    }
    بازگشت false;
}

ایستا تهی
Handle_کلید_Types_File(کلید_Types_Info *info, XkbFile *file)
{
    bool ok;

    آزاد(info->name);
    info->name = strdup_safe(file->name);

    برای (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        انتخاب (stmt->type) {
        حالت STMT_INCLUDE:
            ok = Handle_Include_کلید_Types(info, (IncludeStmt *) stmt);
            بشکن;
        حالت STMT_TYPE:
            ok = Handle_کلید_Type_Def(info, (کلید_Type_Def *) stmt);
            بشکن;
        حالت STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef*) stmt);
            بشکن;
        حالت STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            بشکن;
        حالت STMT_UNKNOWN_DECLARATION:
        حالت STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported types %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            بشکن;
        پیشفرض:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Key type files may not include other declarations; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            بشکن;
        }

        اگر (!ok)
            info->error_شمار++;

        اگر (info->error_شمار > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning keytypes file \"%s\"\n",
                    safe_map_name(file->name));
            بشکن;
        }
    }
}

/***====================================================================***/

ایستا bool
Copy_کلید_Types_To_Keymap(ساختار xkb_keymap *keymap, کلید_Types_Info *info)
{
    ثابت darray_size_t رقم_types = darray_empty(info->types)
                                  ? 1
                                  : darray_size(info->types);
    ساختار xkb_کلید_type *types = calloc(رقم_types, اندازه(*types));
    اگر (!types)
        بازگشت false;

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
    اگر (darray_empty(info->types)) {
        ساختار xkb_کلید_type *type = &types[0];

        type->mods.mods = 0;
        type->رقم_levels = 1;
        type->entries = پوچ;
        type->رقم_entries = 0;
        type->name = xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL");
        type->level_names = پوچ;
        type->رقم_level_names = 0;
        type->required = true;
    }
    وگرنه {
        /* HACK to circumvent Xorg bugs (see comment above) */
        ثابت xkb_atom_t canonical_types[] = {
            xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "TWO_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "ALPHABETIC"),
            xkb_atom_intern_literal(keymap->ctx, "KEYPAD"),
        };

        برای (darray_size_t i = 0; i < رقم_types; i++) {
            کلید_Type_Info *def = &darray_item(info->types, i);
            ساختار xkb_کلید_type *type = &types[i];

            type->name = def->name;
            type->mods.mods = def->mods;
            type->رقم_levels = def->رقم_levels;
            type->رقم_level_names =
                (xkb_level_index_t) darray_size(def->level_names);
            darray_steal(def->level_names, &type->level_names, پوچ);
            darray_steal(def->entries, &type->entries, &type->رقم_entries);
            type->required = false;

            /* HACK: Never drop canonical XKB key types (see comment above) */
            اگر (type->رقم_levels <= 2) {
                برای (uint8_t t = 0;
                     t < (uint8_t) ARRAY_SIZE(canonical_types);
                     t++) {
                        اگر (type->name == canonical_types[t]) {
                            type->required = true;
                            بشکن;
                        }
                }
            }
        }
    }

    keymap->types_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->types_section_name);
    keymap->رقم_types = رقم_types;
    keymap->types = types;
    keymap->mods = info->mods;
    بازگشت true;
}

/***====================================================================***/

bool
Compile_کلید_Types(XkbFile *file, ساختار xkb_keymap_info *keymap_info)
{
    کلید_Types_Info info;

    Init_کلید_Types_Info(&info, keymap_info, 0,
                     &keymap_info->keymap.mods);

    اگر (file != پوچ)
        Handle_کلید_Types_File(&info, file);

    اگر (info.error_شمار != 0)
        برو err_info;

    اگر (!Copy_کلید_Types_To_Keymap(&keymap_info->keymap, &info))
        برو err_info;

    Clear_کلید_Types_Info(&info);
    بازگشت true;

err_info:
    Clear_کلید_Types_Info(&info);
    بازگشت false;
}
