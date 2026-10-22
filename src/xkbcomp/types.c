/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <limits.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "xkbcommon/xkbcommon-status.h"
#অন্তর্ভাব "xkbcomp-priv.h"
#অন্তর্ভাব "text.h"
#অন্তর্ভাব "vmod.h"
#অন্তর্ভাব "expr.h"
#অন্তর্ভাব "include.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "util-mem.h"

গণনা type_field {
    TYPE_FIELD_MASK = (1 << 0),
    TYPE_FIELD_MAP = (1 << 1),
    TYPE_FIELD_PRESERVE = (1 << 2),
    TYPE_FIELD_LEVEL_NAME = (1 << 3),
};

প্রকারলেখ কাঠামো {
    গণনা type_field defined;
    গণনা merge_mode merge;

    xkb_atom_t name;
    xkb_mod_mask_t mods;
    xkb_level_সূচক_t অঙ্ক_levels;
    darray(কাঠামো xkb_চাবি_type_entry) entries;
    darray(xkb_atom_t) level_names;
} চাবি_Type_Info;

প্রকারলেখ কাঠামো {
    অক্ষর *name;
    পূর্ণসংখ্যা error_গুনতি;
    unsigned পূর্ণসংখ্যা include_depth;

    darray(চাবি_Type_Info) types;
    কাঠামো xkb_mod_set mods;

    কাঠামো xkb_context *ctx;
    ধ্রুবক কাঠামো xkb_keymap_info *keymap_info;
} চাবি_Types_Info;

/***====================================================================***/

স্থির অন্তঃরেখ ধ্রুবক অক্ষর *
MapEntryTxt(চাবি_Types_Info *info, কাঠামো xkb_চাবি_type_entry *entry)
{
    ফেরত ModMaskText(info->ctx, MOD_BOTH, &info->mods, entry->mods.mods);
}

স্থির অন্তঃরেখ ধ্রুবক অক্ষর *
TypeTxt(চাবি_Types_Info *info, চাবি_Type_Info *type)
{
    ফেরত xkb_atom_text(info->ctx, type->name);
}

স্থির অন্তঃরেখ ধ্রুবক অক্ষর *
TypeMaskTxt(চাবি_Types_Info *info, চাবি_Type_Info *type)
{
    ফেরত ModMaskText(info->ctx, MOD_BOTH, &info->mods, type->mods);
}

স্থির অন্তঃরেখ bool
ReportTypeShouldBeArray(চাবি_Types_Info *info, চাবি_Type_Info *type,
                        ধ্রুবক অক্ষর *field)
{
    ফেরত ReportShouldBeArray(info->ctx, "key type", field,
                               TypeTxt(info, type));
}

স্থির অন্তঃরেখ bool
ReportTypeBadType(চাবি_Types_Info *info, গণনা xkb_message_code code,
                  চাবি_Type_Info *type, ধ্রুবক অক্ষর *field, ধ্রুবক অক্ষর *wanted)
{
    ফেরত ReportBadType(info->ctx, code, "key type", field,
                         TypeTxt(info, type), wanted);
}

/***====================================================================***/

স্থির শূন্য
Init_চাবি_Types_Info(চাবি_Types_Info *info, ধ্রুবক কাঠামো xkb_keymap_info *keymap_info,
                 unsigned পূর্ণসংখ্যা include_depth,
                 ধ্রুবক কাঠামো xkb_mod_set *mods)
{
    memset(info, 0, আকার(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    InitVMods(&info->mods, mods, include_depth > 0);
}

স্থির শূন্য
Clear_চাবি_Type_Info(চাবি_Type_Info *type)
{
    darray_free(type->entries);
    darray_free(type->level_names);
}

স্থির শূন্য
Clear_চাবি_Types_Info(চাবি_Types_Info *info)
{
    মুক্ত(info->name);
    চাবি_Type_Info *type;
    darray_foreach(type, info->types)
        Clear_চাবি_Type_Info(type);
    darray_free(info->types);
}

স্থির চাবি_Type_Info *
Find_Matching_চাবি_Type(চাবি_Types_Info *info, xkb_atom_t name)
{
    চাবি_Type_Info *old;

    darray_foreach(old, info->types)
        যদি (old->name == name)
            ফেরত old;

    ফেরত ফাঁকা;
}

স্থির bool
Add_চাবি_Type(চাবি_Types_Info *info, চাবি_Type_Info *new, bool same_file)
{
    চাবি_Type_Info *old;
    ধ্রুবক পূর্ণসংখ্যা verbosity = xkb_context_get_log_verbosity(info->ctx);

    old = Find_Matching_চাবি_Type(info, new->name);
    যদি (old) {
        যদি (new->merge != MERGE_AUGMENT) {
            যদি ((same_file && verbosity > 0) || verbosity > 9) {
                log_warn(info->ctx,
                         XKB_WARNING_CONFLICTING_চাবি_TYPE_DEFINITIONS,
                         "Multiple definitions of the %s key type; "
                         "Earlier definition ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            }

            Clear_চাবি_Type_Info(old);
            *old = *new;
            darray_init(new->entries);
            darray_init(new->level_names);
            ফেরত true;
        }

        যদি (same_file)
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_CONFLICTING_চাবি_TYPE_DEFINITIONS,
                    "Multiple definitions of the %s key type; "
                    "Later definition ignored\n",
                    xkb_atom_text(info->ctx, new->name));

        Clear_চাবি_Type_Info(new);
        ফেরত true;
    }

    darray_append(info->types, *new);
    ফেরত true;
}

/***====================================================================***/

স্থির শূন্য
Merge_Included_চাবি_Types(চাবি_Types_Info *into, চাবি_Types_Info *from,
                      গণনা merge_mode merge)
{
    যদি (from->error_গুনতি > 0) {
        into->error_গুনতি += from->error_গুনতি;
        ফেরত;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    যদি (into->name == ফাঁকা) {
        into->name = steal(&from->name);
    }

    যদি (darray_empty(into->types)) {
        into->types = from->types;
        /* Types stolen via shallow copy, so reinitialize the array */
        darray_init(from->types);
    }
    অন্যথা {
        চাবি_Type_Info *type;
        darray_foreach(type, from->types) {
            type->merge = merge;
            যদি (!Add_চাবি_Type(into, type, false))
                into->error_গুনতি++;
        }
        /* Types were either shallow copied or reinitialized individually
           in `AddKeyType`, so we only need to free the array */
        darray_free(from->types);
    }
}

স্থির শূন্য
Handle_চাবি_Types_File(চাবি_Types_Info *info, XkbFile *file);

স্থির bool
Handle_Include_চাবি_Types(চাবি_Types_Info *info, IncludeStmt *include)
{
    চাবি_Types_Info included;

    যদি (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_গুনতি += 10;
        ফেরত false;
    }

    Init_চাবি_Types_Info(&included, info->keymap_info, info->include_depth + 1,
                     &info->mods);
    included.name = steal(&include->stmt);

    ধ্রুবক কাঠামো parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    জন্য (IncludeStmt *stmt = include; stmt; stmt = stmt->পরবর্তী_incl) {
        চাবি_Types_Info পরবর্তী_incl;
        XkbFile *file;

        অক্ষর path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_TYPES,
                                  path, আকার(path));
        যদি (!file) {
            info->error_গুনতি += 10;
            Clear_চাবি_Types_Info(&included);
            ফেরত false;
        }

        Init_চাবি_Types_Info(&পরবর্তী_incl, info->keymap_info, info->include_depth + 1,
                         &included.mods);

        Handle_চাবি_Types_File(&পরবর্তী_incl, file);

        Merge_Included_চাবি_Types(&included, &পরবর্তী_incl, stmt->merge);

        Clear_চাবি_Types_Info(&পরবর্তী_incl);
        FreeXkbFile(file);
    }

    Merge_Included_চাবি_Types(info, &included, include->merge);
    Clear_চাবি_Types_Info(&included);

    ফেরত (info->error_গুনতি == 0);
}

/***====================================================================***/

স্থির bool
SetModifiers(চাবি_Types_Info *info, চাবি_Type_Info *type, রাশিমালা_Def *arrayNdx,
             রাশিমালা_Def *মান)
{
    xkb_mod_mask_t mods = 0;

    যদি (arrayNdx) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The modifiers field of a key type is not an array; "
                "Illegal array subscript ignored\n");
        ফেরত false;
    }

    যদি (!রাশিমালা_Resolve_Mod_Mask(info->ctx, মান, MOD_BOTH, &info->mods, &mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Key type mask field must be a modifier mask; "
                "Key type definition ignored\n");
        ফেরত false;
    }

    যদি (type->defined & TYPE_FIELD_MASK) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple modifier mask definitions for key type %s; "
                 "Using %s, ignoring %s\n",
                 xkb_atom_text(info->ctx, type->name),
                 TypeMaskTxt(info, type),
                 ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods));
        ফেরত false;
    }

    type->mods = mods;
    ফেরত true;
}

/***====================================================================***/

স্থির কাঠামো xkb_চাবি_type_entry *
FindMatchingMapEntry(চাবি_Type_Info *type, xkb_mod_mask_t mods)
{
    কাঠামো xkb_চাবি_type_entry *entry;

    darray_foreach(entry, type->entries)
        যদি (entry->mods.mods == mods)
            ফেরত entry;

    ফেরত ফাঁকা;
}

স্থির bool
AddMapEntry(চাবি_Types_Info *info, চাবি_Type_Info *type,
            কাঠামো xkb_চাবি_type_entry *new, bool clobber, bool report)
{
    কাঠামো xkb_চাবি_type_entry *old;

    old = FindMatchingMapEntry(type, new->mods.mods);
    যদি (old) {
        যদি (report && old->level != new->level) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_চাবি_TYPE_MAP_ENTRY,
                     "Multiple map entries for %s in %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     MapEntryTxt(info, new), TypeTxt(info, type),
                     (clobber ? new->level : old->level) + 1,
                     (clobber ? old->level : new->level) + 1);
        }
        অন্যথা {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_CONFLICTING_চাবি_TYPE_MAP_ENTRY,
                    "Multiple occurrences of map[%s]= %"PRIu32" in %s; Ignored\n",
                    MapEntryTxt(info, new), new->level + 1,
                    TypeTxt(info, type));
            ফেরত true;
        }

        যদি (clobber) {
            যদি (new->level >= type->অঙ্ক_levels)
                type->অঙ্ক_levels = new->level + 1;
            old->level = new->level;
        }

        ফেরত true;
    }

    যদি (new->level >= type->অঙ্ক_levels)
        type->অঙ্ক_levels = new->level + 1;

    darray_append(type->entries, *new);
    ফেরত true;
}

স্থির bool
SetMapEntry(চাবি_Types_Info *info, চাবি_Type_Info *type, রাশিমালা_Def *arrayNdx,
            রাশিমালা_Def *মান)
{
    কাঠামো xkb_চাবি_type_entry entry;

    যদি (arrayNdx == ফাঁকা)
        ফেরত ReportTypeShouldBeArray(info, type, "map entry");

    যদি (!রাশিমালা_Resolve_Mod_Mask(info->ctx, arrayNdx, MOD_BOTH, &info->mods,
                            &entry.mods.mods))
        ফেরত ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "map entry", "modifier mask");

    যদি (entry.mods.mods & (~type->mods)) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_IN_চাবি_TYPE,
                "Map entry for modifiers not used by type %s; "
                "Using %s instead of %s\n",
                TypeTxt(info, type),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods,
                            entry.mods.mods & type->mods),
                MapEntryTxt(info, &entry));
        entry.mods.mods &= type->mods;
    }

    যদি (!রাশিমালা_Resolve_Level(info->ctx, মান, &entry.level)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                          "Level specifications in a key type must be integer; "
                          "Ignoring malformed level specification\n");
        ফেরত false;
    }

    entry.preserve.mods = 0;

    ফেরত AddMapEntry(info, type, &entry, true, true);
}

/***====================================================================***/

স্থির bool
AddPreserve(চাবি_Types_Info *info, চাবি_Type_Info *type,
            xkb_mod_mask_t mods, xkb_mod_mask_t preserve_mods)
{
    কাঠামো xkb_চাবি_type_entry *entry;

    darray_foreach(entry, type->entries) {
        যদি (entry->mods.mods != mods)
            চালু;

        /* Map exists without previous preserve (or "None"); override. */
        যদি (entry->preserve.mods == 0) {
            entry->preserve.mods = preserve_mods;
            ফেরত true;
        }

        /* Map exists with same preserve; do nothing. */
        যদি (entry->preserve.mods == preserve_mods) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_DUPLICATE_ENTRY,
                    "Identical definitions for preserve[%s] in %s; "
                    "Ignored\n",
                    ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                    TypeTxt(info, type));
            ফেরত true;
        }

        /* Map exists with different preserve; latter wins. */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_চাবি_TYPE_PRESERVE_ENTRIES,
                "Multiple definitions for preserve[%s] in %s; "
                "Using %s, ignoring %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods,
                            entry->preserve.mods));

        entry->preserve.mods = preserve_mods;
        ফেরত true;
    }

    /*
     * Map does not exist, i.e. preserve[] came before map[].
     * Create a map with the specified mask mapping to Level1. The level
     * may be overridden later with an explicit map[] statement.
     */
    ধ্রুবক কাঠামো xkb_চাবি_type_entry new = {
        .level = 0,
        .mods = { .mods = mods },
        .preserve = { .mods = preserve_mods },
    };
    darray_append(type->entries, new);
    ফেরত true;
}

স্থির bool
SetPreserve(চাবি_Types_Info *info, চাবি_Type_Info *type, রাশিমালা_Def *arrayNdx,
            রাশিমালা_Def *মান)
{

    যদি (arrayNdx == ফাঁকা)
        ফেরত ReportTypeShouldBeArray(info, type, "preserve entry");

    xkb_mod_mask_t mods = 0;
    যদি (!রাশিমালা_Resolve_Mod_Mask(info->ctx, arrayNdx, MOD_BOTH, &info->mods, &mods))
        ফেরত ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "preserve entry", "modifier mask");

    যদি (mods & ~type->mods) {
        ধ্রুবক অক্ষর *before, *after;

        before = ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods);
        mods &= type->mods;
        after = ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_IN_চাবি_TYPE,
                "Preserve entry for modifiers not used by the %s type; "
                "Index %s converted to %s\n",
                TypeTxt(info, type), before, after);
    }

    xkb_mod_mask_t preserve_mods = 0;
    যদি (!রাশিমালা_Resolve_Mod_Mask(info->ctx, মান, MOD_BOTH, &info->mods,
                            &preserve_mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Preserve value in a key type is not a modifier mask; "
                "Ignoring preserve[%s] in type %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type));
        ফেরত false;
    }

    যদি (preserve_mods & ~mods) {
        ধ্রুবক অক্ষর *before, *after;

        before = ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods);
        preserve_mods &= mods;
        after = ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_ILLEGAL_চাবি_TYPE_PRESERVE_ফলাফল,
                "Illegal value for preserve[%s] in type %s; "
                "Converted %s to %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type), before, after);
    }

    ফেরত AddPreserve(info, type, mods, preserve_mods);
}

/***====================================================================***/

স্থির bool
AddLevelName(চাবি_Types_Info *info, চাবি_Type_Info *type,
             xkb_level_সূচক_t level, xkb_atom_t name, bool clobber)
{
    /* New name. */
    যদি (level >= darray_আকার(type->level_names)) {
        darray_resize0(type->level_names, level + 1);
        যাও finish;
    }

    /* Same level, same name. */
    যদি (darray_item(type->level_names, level) == name) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE, XKB_WARNING_DUPLICATE_ENTRY,
                "Duplicate names for level %"PRIu32" of key type %s; Ignored\n",
                level + 1, TypeTxt(info, type));
        ফেরত true;
    }

    /* Same level, different name. */
    যদি (darray_item(type->level_names, level) != XKB_ATOM_NONE) {
        ধ্রুবক অক্ষর *old, *new;
        old = xkb_atom_text(info->ctx,
                            darray_item(type->level_names, level));
        new = xkb_atom_text(info->ctx, name);
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_চাবি_TYPE_LEVEL_NAMES,
                "Multiple names for level %"PRIu32" of key type %s; "
                "Using %s, ignoring %s\n",
                level + 1, TypeTxt(info, type),
                (clobber ? new : old), (clobber ? old : new));

        যদি (!clobber)
            ফেরত true;
    }

    /* FIXME: What about different level, same name? */

finish:
    darray_item(type->level_names, level) = name;
    ফেরত true;
}

স্থির bool
SetLevelName(চাবি_Types_Info *info, চাবি_Type_Info *type, রাশিমালা_Def *arrayNdx,
             রাশিমালা_Def *মান)
{
    যদি (arrayNdx == ফাঁকা)
        ফেরত ReportTypeShouldBeArray(info, type, "level name");

    xkb_level_সূচক_t level = 0;
    যদি (!রাশিমালা_Resolve_Level(info->ctx, arrayNdx, &level))
        ফেরত ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                                 type, "level name", "integer");

    xkb_atom_t level_name = XKB_ATOM_NONE;
    যদি (!রাশিমালা_Resolve_String(info->ctx, মান, &level_name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Non-string name for level %"PRIu32" in key type %s; "
                "Ignoring illegal level name definition\n",
                level + 1, xkb_atom_text(info->ctx, type->name));
        ফেরত false;
    }

    ফেরত AddLevelName(info, type, level, level_name, true);
}

/***====================================================================***/

স্থির bool
Set_চাবি_Type_Field(চাবি_Types_Info *info, চাবি_Type_Info *type,
                ধ্রুবক অক্ষর *field, রাশিমালা_Def *arrayNdx, রাশিমালা_Def *মান)
{
    bool ok = false;
    গণনা type_field type_field = 0;

    যদি (istreq(field, "modifiers")) {
        type_field = TYPE_FIELD_MASK;
        ok = SetModifiers(info, type, arrayNdx, মান);
    }
    অন্যথা যদি (istreq(field, "map")) {
        type_field = TYPE_FIELD_MAP;
        ok = SetMapEntry(info, type, arrayNdx, মান);
    }
    অন্যথা যদি (istreq(field, "preserve")) {
        type_field = TYPE_FIELD_PRESERVE;
        ok = SetPreserve(info, type, arrayNdx, মান);
    }
    অন্যথা যদি (istreq(field, "levelname") || istreq(field, "level_name")) {
        type_field = TYPE_FIELD_LEVEL_NAME;
        ok = SetLevelName(info, type, arrayNdx, মান);
    } অন্যথা {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in key type \"%s\"; Definition ignored\n",
                field, TypeTxt(info, type));
        ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_TYPE_FIELDS);
    }

    type->defined |= type_field;
    ফেরত ok;
}

স্থির bool
Handle_চাবি_Type_Body(চাবি_Types_Info *info, VarDef *def, চাবি_Type_Info *type)
{
    bool ok = true;
    ধ্রুবক অক্ষর *elem, *field;
    রাশিমালা_Def *arrayNdx;

    জন্য (; def; def = (VarDef *) def->common.পরবর্তী) {
        যদি (!রাশিমালা_Resolve_Lhs(info->ctx, def->name, &elem, &field,
                            &arrayNdx)) {
            /* internal error, already reported */
            ok = false;
            চালু;
        }

        যদি (elem) {
            যদি (istreq(elem, "type")) {
                log_err(info->ctx, XKB_ERROR_INVALID_SET_DEFAULT_STATEMENT,
                        "Support for changing the default type has been removed; "
                        "Statement \"%s.%s\" ignored.\n", elem, field);
            }
            অন্যথা {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key type statement: move statements to the global "
                        "file scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
            চালু;
        }

        যদি (!Set_চাবি_Type_Field(info, type, field, arrayNdx, def->মান))
            ok = false;
    }

    ফেরত ok;
}

স্থির bool
Handle_চাবি_Type_Def(চাবি_Types_Info *info, চাবি_Type_Def *def)
{
    চাবি_Type_Info type = {
        .defined = 0,
        .merge = def->merge,
        .name = def->name,
        .mods = 0,
        .অঙ্ক_levels = 1,
        .entries = darray_new(),
        .level_names = darray_new(),
    };

    যদি (!Handle_চাবি_Type_Body(info, def->body, &type) ||
        !Add_চাবি_Type(info, &type, true))
    {
        info->error_গুনতি++;
        Clear_চাবি_Type_Info(&type);
        ফেরত false;
    }

    /* Type has been either stolen via shallow copy or reinitialized in
       `AddKeyType`: no need to free the arrays */
    ফেরত true;
}

স্থির bool
HandleGlobalVar(চাবি_Types_Info *info, VarDef *stmt)
{
    ধ্রুবক অক্ষর *elem, *field;
    রাশিমালা_Def *arrayNdx;

    যদি (!রাশিমালা_Resolve_Lhs(info->ctx, stmt->name, &elem, &field,
                        &arrayNdx)) {
        ফেরত false;           /* internal error, already reported */
    }
    অন্যথা যদি (elem && istreq(elem, "type")) {
        log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                "Support for changing the default type has been removed; "
                "Statement ignored\n");
        ফেরত true;
    }
    অন্যথা যদি (elem) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown element \"%s\"; "
                "Value for field \"%s.%s\" ignored\n", elem, elem, field);
        ফেরত !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
    }
    অন্যথা যদি (field) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        ফেরত !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS);
    }
    ফেরত false;
}

স্থির শূন্য
Handle_চাবি_Types_File(চাবি_Types_Info *info, XkbFile *file)
{
    bool ok;

    মুক্ত(info->name);
    info->name = strdup_safe(file->name);

    জন্য (ParseCommon *stmt = file->defs; stmt; stmt = stmt->পরবর্তী) {
        নির্বাচন (stmt->type) {
        ক্ষেত্র STMT_INCLUDE:
            ok = Handle_Include_চাবি_Types(info, (IncludeStmt *) stmt);
            বিরতি;
        ক্ষেত্র STMT_TYPE:
            ok = Handle_চাবি_Type_Def(info, (চাবি_Type_Def *) stmt);
            বিরতি;
        ক্ষেত্র STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef*) stmt);
            বিরতি;
        ক্ষেত্র STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            বিরতি;
        ক্ষেত্র STMT_UNKNOWN_DECLARATION:
        ক্ষেত্র STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported types %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            বিরতি;
        পূর্বনির্ধারিত:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Key type files may not include other declarations; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            বিরতি;
        }

        যদি (!ok)
            info->error_গুনতি++;

        যদি (info->error_গুনতি > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning keytypes file \"%s\"\n",
                    safe_map_name(file->name));
            বিরতি;
        }
    }
}

/***====================================================================***/

স্থির bool
Copy_চাবি_Types_To_Keymap(কাঠামো xkb_keymap *keymap, চাবি_Types_Info *info)
{
    ধ্রুবক darray_আকার_t অঙ্ক_types = darray_empty(info->types)
                                  ? 1
                                  : darray_আকার(info->types);
    কাঠামো xkb_চাবি_type *types = calloc(অঙ্ক_types, আকার(*types));
    যদি (!types)
        ফেরত false;

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
    যদি (darray_empty(info->types)) {
        কাঠামো xkb_চাবি_type *type = &types[0];

        type->mods.mods = 0;
        type->অঙ্ক_levels = 1;
        type->entries = ফাঁকা;
        type->অঙ্ক_entries = 0;
        type->name = xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL");
        type->level_names = ফাঁকা;
        type->অঙ্ক_level_names = 0;
        type->required = true;
    }
    অন্যথা {
        /* HACK to circumvent Xorg bugs (see comment above) */
        ধ্রুবক xkb_atom_t canonical_types[] = {
            xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "TWO_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "ALPHABETIC"),
            xkb_atom_intern_literal(keymap->ctx, "KEYPAD"),
        };

        জন্য (darray_আকার_t i = 0; i < অঙ্ক_types; i++) {
            চাবি_Type_Info *def = &darray_item(info->types, i);
            কাঠামো xkb_চাবি_type *type = &types[i];

            type->name = def->name;
            type->mods.mods = def->mods;
            type->অঙ্ক_levels = def->অঙ্ক_levels;
            type->অঙ্ক_level_names =
                (xkb_level_সূচক_t) darray_আকার(def->level_names);
            darray_steal(def->level_names, &type->level_names, ফাঁকা);
            darray_steal(def->entries, &type->entries, &type->অঙ্ক_entries);
            type->required = false;

            /* HACK: Never drop canonical XKB key types (see comment above) */
            যদি (type->অঙ্ক_levels <= 2) {
                জন্য (uint8_t t = 0;
                     t < (uint8_t) ARRAY_আকার(canonical_types);
                     t++) {
                        যদি (type->name == canonical_types[t]) {
                            type->required = true;
                            বিরতি;
                        }
                }
            }
        }
    }

    keymap->types_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->types_section_name);
    keymap->অঙ্ক_types = অঙ্ক_types;
    keymap->types = types;
    keymap->mods = info->mods;
    ফেরত true;
}

/***====================================================================***/

bool
Compile_চাবি_Types(XkbFile *file, কাঠামো xkb_keymap_info *keymap_info)
{
    চাবি_Types_Info info;

    Init_চাবি_Types_Info(&info, keymap_info, 0,
                     &keymap_info->keymap.mods);

    যদি (file != ফাঁকা)
        Handle_চাবি_Types_File(&info, file);

    যদি (info.error_গুনতি != 0)
        যাও err_info;

    যদি (!Copy_চাবি_Types_To_Keymap(&keymap_info->keymap, &info))
        যাও err_info;

    Clear_চাবি_Types_Info(&info);
    ফেরত true;

err_info:
    Clear_চাবি_Types_Info(&info);
    ফেরত false;
}
