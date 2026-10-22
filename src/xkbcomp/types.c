/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <limits.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon-status.h"
#അന്തര്ഭാവ "xkbcomp-priv.h"
#അന്തര്ഭാവ "text.h"
#അന്തര്ഭാവ "vmod.h"
#അന്തര്ഭാവ "expr.h"
#അന്തര്ഭാവ "include.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "util-mem.h"

എണ്ണൽ type_field {
    TYPE_FIELD_MASK = (1 << 0),
    TYPE_FIELD_MAP = (1 << 1),
    TYPE_FIELD_PRESERVE = (1 << 2),
    TYPE_FIELD_LEVEL_NAME = (1 << 3),
};

തരനിർവചനം ഘടന {
    എണ്ണൽ type_field defined;
    എണ്ണൽ merge_mode merge;

    xkb_atom_t name;
    xkb_mod_mask_t mods;
    xkb_level_index_t അക്കം_levels;
    darray(ഘടന xkb_താക്കോൽ_type_entry) entries;
    darray(xkb_atom_t) level_names;
} താക്കോൽ_Type_Info;

തരനിർവചനം ഘടന {
    അക്ഷരം *name;
    പൂർണ്ണസംഖ്യ error_എണ്ണം;
    unsigned പൂർണ്ണസംഖ്യ include_depth;

    darray(താക്കോൽ_Type_Info) types;
    ഘടന xkb_mod_set mods;

    ഘടന xkb_context *ctx;
    സ്ഥിരം ഘടന xkb_keymap_info *keymap_info;
} താക്കോൽ_Types_Info;

/***====================================================================***/

സ്ഥിതം വരിയിൽ സ്ഥിരം അക്ഷരം *
MapEntryTxt(താക്കോൽ_Types_Info *info, ഘടന xkb_താക്കോൽ_type_entry *entry)
{
    തിരികെ ModMaskText(info->ctx, MOD_BOTH, &info->mods, entry->mods.mods);
}

സ്ഥിതം വരിയിൽ സ്ഥിരം അക്ഷരം *
TypeTxt(താക്കോൽ_Types_Info *info, താക്കോൽ_Type_Info *type)
{
    തിരികെ xkb_atom_text(info->ctx, type->name);
}

സ്ഥിതം വരിയിൽ സ്ഥിരം അക്ഷരം *
TypeMaskTxt(താക്കോൽ_Types_Info *info, താക്കോൽ_Type_Info *type)
{
    തിരികെ ModMaskText(info->ctx, MOD_BOTH, &info->mods, type->mods);
}

സ്ഥിതം വരിയിൽ bool
ReportTypeShouldBeArray(താക്കോൽ_Types_Info *info, താക്കോൽ_Type_Info *type,
                        സ്ഥിരം അക്ഷരം *field)
{
    തിരികെ ReportShouldBeArray(info->ctx, "key type", field,
                               TypeTxt(info, type));
}

സ്ഥിതം വരിയിൽ bool
ReportTypeBadType(താക്കോൽ_Types_Info *info, എണ്ണൽ xkb_message_code code,
                  താക്കോൽ_Type_Info *type, സ്ഥിരം അക്ഷരം *field, സ്ഥിരം അക്ഷരം *wanted)
{
    തിരികെ ReportBadType(info->ctx, code, "key type", field,
                         TypeTxt(info, type), wanted);
}

/***====================================================================***/

സ്ഥിതം ശൂന്യം
Init_താക്കോൽ_Types_Info(താക്കോൽ_Types_Info *info, സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
                 unsigned പൂർണ്ണസംഖ്യ include_depth,
                 സ്ഥിരം ഘടന xkb_mod_set *mods)
{
    memset(info, 0, വലുപ്പം(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    InitVMods(&info->mods, mods, include_depth > 0);
}

സ്ഥിതം ശൂന്യം
Clear_താക്കോൽ_Type_Info(താക്കോൽ_Type_Info *type)
{
    darray_free(type->entries);
    darray_free(type->level_names);
}

സ്ഥിതം ശൂന്യം
Clear_താക്കോൽ_Types_Info(താക്കോൽ_Types_Info *info)
{
    സ്വതന്ത്രം(info->name);
    താക്കോൽ_Type_Info *type;
    darray_foreach(type, info->types)
        Clear_താക്കോൽ_Type_Info(type);
    darray_free(info->types);
}

സ്ഥിതം താക്കോൽ_Type_Info *
Find_Matching_താക്കോൽ_Type(താക്കോൽ_Types_Info *info, xkb_atom_t name)
{
    താക്കോൽ_Type_Info *old;

    darray_foreach(old, info->types)
        എങ്കിൽ (old->name == name)
            തിരികെ old;

    തിരികെ ശൂന്യ;
}

സ്ഥിതം bool
Add_താക്കോൽ_Type(താക്കോൽ_Types_Info *info, താക്കോൽ_Type_Info *new, bool same_file)
{
    താക്കോൽ_Type_Info *old;
    സ്ഥിരം പൂർണ്ണസംഖ്യ verbosity = xkb_context_get_log_verbosity(info->ctx);

    old = Find_Matching_താക്കോൽ_Type(info, new->name);
    എങ്കിൽ (old) {
        എങ്കിൽ (new->merge != MERGE_AUGMENT) {
            എങ്കിൽ ((same_file && verbosity > 0) || verbosity > 9) {
                log_warn(info->ctx,
                         XKB_WARNING_CONFLICTING_താക്കോൽ_TYPE_DEFINITIONS,
                         "Multiple definitions of the %s key type; "
                         "Earlier definition ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            }

            Clear_താക്കോൽ_Type_Info(old);
            *old = *new;
            darray_init(new->entries);
            darray_init(new->level_names);
            തിരികെ true;
        }

        എങ്കിൽ (same_file)
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_CONFLICTING_താക്കോൽ_TYPE_DEFINITIONS,
                    "Multiple definitions of the %s key type; "
                    "Later definition ignored\n",
                    xkb_atom_text(info->ctx, new->name));

        Clear_താക്കോൽ_Type_Info(new);
        തിരികെ true;
    }

    darray_append(info->types, *new);
    തിരികെ true;
}

/***====================================================================***/

സ്ഥിതം ശൂന്യം
Merge_Included_താക്കോൽ_Types(താക്കോൽ_Types_Info *into, താക്കോൽ_Types_Info *from,
                      എണ്ണൽ merge_mode merge)
{
    എങ്കിൽ (from->error_എണ്ണം > 0) {
        into->error_എണ്ണം += from->error_എണ്ണം;
        തിരികെ;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    എങ്കിൽ (into->name == ശൂന്യ) {
        into->name = steal(&from->name);
    }

    എങ്കിൽ (darray_empty(into->types)) {
        into->types = from->types;
        /* Types stolen via shallow copy, so reinitialize the array */
        darray_init(from->types);
    }
    അല്ലെങ്കിൽ {
        താക്കോൽ_Type_Info *type;
        darray_foreach(type, from->types) {
            type->merge = merge;
            എങ്കിൽ (!Add_താക്കോൽ_Type(into, type, false))
                into->error_എണ്ണം++;
        }
        /* Types were either shallow copied or reinitialized individually
           in `AddKeyType`, so we only need to free the array */
        darray_free(from->types);
    }
}

സ്ഥിതം ശൂന്യം
Handle_താക്കോൽ_Types_File(താക്കോൽ_Types_Info *info, XkbFile *file);

സ്ഥിതം bool
Handle_Include_താക്കോൽ_Types(താക്കോൽ_Types_Info *info, IncludeStmt *include)
{
    താക്കോൽ_Types_Info included;

    എങ്കിൽ (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_എണ്ണം += 10;
        തിരികെ false;
    }

    Init_താക്കോൽ_Types_Info(&included, info->keymap_info, info->include_depth + 1,
                     &info->mods);
    included.name = steal(&include->stmt);

    സ്ഥിരം ഘടന parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    വേണ്ടി (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        താക്കോൽ_Types_Info next_incl;
        XkbFile *file;

        അക്ഷരം path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_TYPES,
                                  path, വലുപ്പം(path));
        എങ്കിൽ (!file) {
            info->error_എണ്ണം += 10;
            Clear_താക്കോൽ_Types_Info(&included);
            തിരികെ false;
        }

        Init_താക്കോൽ_Types_Info(&next_incl, info->keymap_info, info->include_depth + 1,
                         &included.mods);

        Handle_താക്കോൽ_Types_File(&next_incl, file);

        Merge_Included_താക്കോൽ_Types(&included, &next_incl, stmt->merge);

        Clear_താക്കോൽ_Types_Info(&next_incl);
        FreeXkbFile(file);
    }

    Merge_Included_താക്കോൽ_Types(info, &included, include->merge);
    Clear_താക്കോൽ_Types_Info(&included);

    തിരികെ (info->error_എണ്ണം == 0);
}

/***====================================================================***/

സ്ഥിതം bool
SetModifiers(താക്കോൽ_Types_Info *info, താക്കോൽ_Type_Info *type, വ്യഞ്ജകം_Def *arrayNdx,
             വ്യഞ്ജകം_Def *മൂല്യം)
{
    xkb_mod_mask_t mods = 0;

    എങ്കിൽ (arrayNdx) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The modifiers field of a key type is not an array; "
                "Illegal array subscript ignored\n");
        തിരികെ false;
    }

    എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Mod_Mask(info->ctx, മൂല്യം, MOD_BOTH, &info->mods, &mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Key type mask field must be a modifier mask; "
                "Key type definition ignored\n");
        തിരികെ false;
    }

    എങ്കിൽ (type->defined & TYPE_FIELD_MASK) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple modifier mask definitions for key type %s; "
                 "Using %s, ignoring %s\n",
                 xkb_atom_text(info->ctx, type->name),
                 TypeMaskTxt(info, type),
                 ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods));
        തിരികെ false;
    }

    type->mods = mods;
    തിരികെ true;
}

/***====================================================================***/

സ്ഥിതം ഘടന xkb_താക്കോൽ_type_entry *
FindMatchingMapEntry(താക്കോൽ_Type_Info *type, xkb_mod_mask_t mods)
{
    ഘടന xkb_താക്കോൽ_type_entry *entry;

    darray_foreach(entry, type->entries)
        എങ്കിൽ (entry->mods.mods == mods)
            തിരികെ entry;

    തിരികെ ശൂന്യ;
}

സ്ഥിതം bool
AddMapEntry(താക്കോൽ_Types_Info *info, താക്കോൽ_Type_Info *type,
            ഘടന xkb_താക്കോൽ_type_entry *new, bool clobber, bool report)
{
    ഘടന xkb_താക്കോൽ_type_entry *old;

    old = FindMatchingMapEntry(type, new->mods.mods);
    എങ്കിൽ (old) {
        എങ്കിൽ (report && old->level != new->level) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_താക്കോൽ_TYPE_MAP_ENTRY,
                     "Multiple map entries for %s in %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     MapEntryTxt(info, new), TypeTxt(info, type),
                     (clobber ? new->level : old->level) + 1,
                     (clobber ? old->level : new->level) + 1);
        }
        അല്ലെങ്കിൽ {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_CONFLICTING_താക്കോൽ_TYPE_MAP_ENTRY,
                    "Multiple occurrences of map[%s]= %"PRIu32" in %s; Ignored\n",
                    MapEntryTxt(info, new), new->level + 1,
                    TypeTxt(info, type));
            തിരികെ true;
        }

        എങ്കിൽ (clobber) {
            എങ്കിൽ (new->level >= type->അക്കം_levels)
                type->അക്കം_levels = new->level + 1;
            old->level = new->level;
        }

        തിരികെ true;
    }

    എങ്കിൽ (new->level >= type->അക്കം_levels)
        type->അക്കം_levels = new->level + 1;

    darray_append(type->entries, *new);
    തിരികെ true;
}

സ്ഥിതം bool
SetMapEntry(താക്കോൽ_Types_Info *info, താക്കോൽ_Type_Info *type, വ്യഞ്ജകം_Def *arrayNdx,
            വ്യഞ്ജകം_Def *മൂല്യം)
{
    ഘടന xkb_താക്കോൽ_type_entry entry;

    എങ്കിൽ (arrayNdx == ശൂന്യ)
        തിരികെ ReportTypeShouldBeArray(info, type, "map entry");

    എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Mod_Mask(info->ctx, arrayNdx, MOD_BOTH, &info->mods,
                            &entry.mods.mods))
        തിരികെ ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "map entry", "modifier mask");

    എങ്കിൽ (entry.mods.mods & (~type->mods)) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_IN_താക്കോൽ_TYPE,
                "Map entry for modifiers not used by type %s; "
                "Using %s instead of %s\n",
                TypeTxt(info, type),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods,
                            entry.mods.mods & type->mods),
                MapEntryTxt(info, &entry));
        entry.mods.mods &= type->mods;
    }

    എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Level(info->ctx, മൂല്യം, &entry.level)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                          "Level specifications in a key type must be integer; "
                          "Ignoring malformed level specification\n");
        തിരികെ false;
    }

    entry.preserve.mods = 0;

    തിരികെ AddMapEntry(info, type, &entry, true, true);
}

/***====================================================================***/

സ്ഥിതം bool
AddPreserve(താക്കോൽ_Types_Info *info, താക്കോൽ_Type_Info *type,
            xkb_mod_mask_t mods, xkb_mod_mask_t preserve_mods)
{
    ഘടന xkb_താക്കോൽ_type_entry *entry;

    darray_foreach(entry, type->entries) {
        എങ്കിൽ (entry->mods.mods != mods)
            തുടരുക;

        /* Map exists without previous preserve (or "None"); override. */
        എങ്കിൽ (entry->preserve.mods == 0) {
            entry->preserve.mods = preserve_mods;
            തിരികെ true;
        }

        /* Map exists with same preserve; do nothing. */
        എങ്കിൽ (entry->preserve.mods == preserve_mods) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_DUPLICATE_ENTRY,
                    "Identical definitions for preserve[%s] in %s; "
                    "Ignored\n",
                    ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                    TypeTxt(info, type));
            തിരികെ true;
        }

        /* Map exists with different preserve; latter wins. */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_താക്കോൽ_TYPE_PRESERVE_ENTRIES,
                "Multiple definitions for preserve[%s] in %s; "
                "Using %s, ignoring %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods,
                            entry->preserve.mods));

        entry->preserve.mods = preserve_mods;
        തിരികെ true;
    }

    /*
     * Map does not exist, i.e. preserve[] came before map[].
     * Create a map with the specified mask mapping to Level1. The level
     * may be overridden later with an explicit map[] statement.
     */
    സ്ഥിരം ഘടന xkb_താക്കോൽ_type_entry new = {
        .level = 0,
        .mods = { .mods = mods },
        .preserve = { .mods = preserve_mods },
    };
    darray_append(type->entries, new);
    തിരികെ true;
}

സ്ഥിതം bool
SetPreserve(താക്കോൽ_Types_Info *info, താക്കോൽ_Type_Info *type, വ്യഞ്ജകം_Def *arrayNdx,
            വ്യഞ്ജകം_Def *മൂല്യം)
{

    എങ്കിൽ (arrayNdx == ശൂന്യ)
        തിരികെ ReportTypeShouldBeArray(info, type, "preserve entry");

    xkb_mod_mask_t mods = 0;
    എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Mod_Mask(info->ctx, arrayNdx, MOD_BOTH, &info->mods, &mods))
        തിരികെ ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "preserve entry", "modifier mask");

    എങ്കിൽ (mods & ~type->mods) {
        സ്ഥിരം അക്ഷരം *before, *after;

        before = ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods);
        mods &= type->mods;
        after = ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_IN_താക്കോൽ_TYPE,
                "Preserve entry for modifiers not used by the %s type; "
                "Index %s converted to %s\n",
                TypeTxt(info, type), before, after);
    }

    xkb_mod_mask_t preserve_mods = 0;
    എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Mod_Mask(info->ctx, മൂല്യം, MOD_BOTH, &info->mods,
                            &preserve_mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Preserve value in a key type is not a modifier mask; "
                "Ignoring preserve[%s] in type %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type));
        തിരികെ false;
    }

    എങ്കിൽ (preserve_mods & ~mods) {
        സ്ഥിരം അക്ഷരം *before, *after;

        before = ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods);
        preserve_mods &= mods;
        after = ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_ILLEGAL_താക്കോൽ_TYPE_PRESERVE_RESULT,
                "Illegal value for preserve[%s] in type %s; "
                "Converted %s to %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type), before, after);
    }

    തിരികെ AddPreserve(info, type, mods, preserve_mods);
}

/***====================================================================***/

സ്ഥിതം bool
AddLevelName(താക്കോൽ_Types_Info *info, താക്കോൽ_Type_Info *type,
             xkb_level_index_t level, xkb_atom_t name, bool clobber)
{
    /* New name. */
    എങ്കിൽ (level >= darray_size(type->level_names)) {
        darray_resize0(type->level_names, level + 1);
        പോകുക finish;
    }

    /* Same level, same name. */
    എങ്കിൽ (darray_item(type->level_names, level) == name) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE, XKB_WARNING_DUPLICATE_ENTRY,
                "Duplicate names for level %"PRIu32" of key type %s; Ignored\n",
                level + 1, TypeTxt(info, type));
        തിരികെ true;
    }

    /* Same level, different name. */
    എങ്കിൽ (darray_item(type->level_names, level) != XKB_ATOM_NONE) {
        സ്ഥിരം അക്ഷരം *old, *new;
        old = xkb_atom_text(info->ctx,
                            darray_item(type->level_names, level));
        new = xkb_atom_text(info->ctx, name);
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_താക്കോൽ_TYPE_LEVEL_NAMES,
                "Multiple names for level %"PRIu32" of key type %s; "
                "Using %s, ignoring %s\n",
                level + 1, TypeTxt(info, type),
                (clobber ? new : old), (clobber ? old : new));

        എങ്കിൽ (!clobber)
            തിരികെ true;
    }

    /* FIXME: What about different level, same name? */

finish:
    darray_item(type->level_names, level) = name;
    തിരികെ true;
}

സ്ഥിതം bool
SetLevelName(താക്കോൽ_Types_Info *info, താക്കോൽ_Type_Info *type, വ്യഞ്ജകം_Def *arrayNdx,
             വ്യഞ്ജകം_Def *മൂല്യം)
{
    എങ്കിൽ (arrayNdx == ശൂന്യ)
        തിരികെ ReportTypeShouldBeArray(info, type, "level name");

    xkb_level_index_t level = 0;
    എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Level(info->ctx, arrayNdx, &level))
        തിരികെ ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                                 type, "level name", "integer");

    xkb_atom_t level_name = XKB_ATOM_NONE;
    എങ്കിൽ (!വ്യഞ്ജകം_Resolve_String(info->ctx, മൂല്യം, &level_name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Non-string name for level %"PRIu32" in key type %s; "
                "Ignoring illegal level name definition\n",
                level + 1, xkb_atom_text(info->ctx, type->name));
        തിരികെ false;
    }

    തിരികെ AddLevelName(info, type, level, level_name, true);
}

/***====================================================================***/

സ്ഥിതം bool
Set_താക്കോൽ_Type_Field(താക്കോൽ_Types_Info *info, താക്കോൽ_Type_Info *type,
                സ്ഥിരം അക്ഷരം *field, വ്യഞ്ജകം_Def *arrayNdx, വ്യഞ്ജകം_Def *മൂല്യം)
{
    bool ok = false;
    എണ്ണൽ type_field type_field = 0;

    എങ്കിൽ (istreq(field, "modifiers")) {
        type_field = TYPE_FIELD_MASK;
        ok = SetModifiers(info, type, arrayNdx, മൂല്യം);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "map")) {
        type_field = TYPE_FIELD_MAP;
        ok = SetMapEntry(info, type, arrayNdx, മൂല്യം);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "preserve")) {
        type_field = TYPE_FIELD_PRESERVE;
        ok = SetPreserve(info, type, arrayNdx, മൂല്യം);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (istreq(field, "levelname") || istreq(field, "level_name")) {
        type_field = TYPE_FIELD_LEVEL_NAME;
        ok = SetLevelName(info, type, arrayNdx, മൂല്യം);
    } അല്ലെങ്കിൽ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in key type \"%s\"; Definition ignored\n",
                field, TypeTxt(info, type));
        ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_TYPE_FIELDS);
    }

    type->defined |= type_field;
    തിരികെ ok;
}

സ്ഥിതം bool
Handle_താക്കോൽ_Type_Body(താക്കോൽ_Types_Info *info, VarDef *def, താക്കോൽ_Type_Info *type)
{
    bool ok = true;
    സ്ഥിരം അക്ഷരം *elem, *field;
    വ്യഞ്ജകം_Def *arrayNdx;

    വേണ്ടി (; def; def = (VarDef *) def->common.next) {
        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Lhs(info->ctx, def->name, &elem, &field,
                            &arrayNdx)) {
            /* internal error, already reported */
            ok = false;
            തുടരുക;
        }

        എങ്കിൽ (elem) {
            എങ്കിൽ (istreq(elem, "type")) {
                log_err(info->ctx, XKB_ERROR_INVALID_SET_DEFAULT_STATEMENT,
                        "Support for changing the default type has been removed; "
                        "Statement \"%s.%s\" ignored.\n", elem, field);
            }
            അല്ലെങ്കിൽ {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key type statement: move statements to the global "
                        "file scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
            തുടരുക;
        }

        എങ്കിൽ (!Set_താക്കോൽ_Type_Field(info, type, field, arrayNdx, def->മൂല്യം))
            ok = false;
    }

    തിരികെ ok;
}

സ്ഥിതം bool
Handle_താക്കോൽ_Type_Def(താക്കോൽ_Types_Info *info, താക്കോൽ_Type_Def *def)
{
    താക്കോൽ_Type_Info type = {
        .defined = 0,
        .merge = def->merge,
        .name = def->name,
        .mods = 0,
        .അക്കം_levels = 1,
        .entries = darray_new(),
        .level_names = darray_new(),
    };

    എങ്കിൽ (!Handle_താക്കോൽ_Type_Body(info, def->body, &type) ||
        !Add_താക്കോൽ_Type(info, &type, true))
    {
        info->error_എണ്ണം++;
        Clear_താക്കോൽ_Type_Info(&type);
        തിരികെ false;
    }

    /* Type has been either stolen via shallow copy or reinitialized in
       `AddKeyType`: no need to free the arrays */
    തിരികെ true;
}

സ്ഥിതം bool
HandleGlobalVar(താക്കോൽ_Types_Info *info, VarDef *stmt)
{
    സ്ഥിരം അക്ഷരം *elem, *field;
    വ്യഞ്ജകം_Def *arrayNdx;

    എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Lhs(info->ctx, stmt->name, &elem, &field,
                        &arrayNdx)) {
        തിരികെ false;           /* internal error, already reported */
    }
    അല്ലെങ്കിൽ എങ്കിൽ (elem && istreq(elem, "type")) {
        log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                "Support for changing the default type has been removed; "
                "Statement ignored\n");
        തിരികെ true;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (elem) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown element \"%s\"; "
                "Value for field \"%s.%s\" ignored\n", elem, elem, field);
        തിരികെ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (field) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        തിരികെ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS);
    }
    തിരികെ false;
}

സ്ഥിതം ശൂന്യം
Handle_താക്കോൽ_Types_File(താക്കോൽ_Types_Info *info, XkbFile *file)
{
    bool ok;

    സ്വതന്ത്രം(info->name);
    info->name = strdup_safe(file->name);

    വേണ്ടി (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        തിരഞ്ഞെടുപ്പ് (stmt->type) {
        അവസ്ഥ STMT_INCLUDE:
            ok = Handle_Include_താക്കോൽ_Types(info, (IncludeStmt *) stmt);
            നിർത്തുക;
        അവസ്ഥ STMT_TYPE:
            ok = Handle_താക്കോൽ_Type_Def(info, (താക്കോൽ_Type_Def *) stmt);
            നിർത്തുക;
        അവസ്ഥ STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef*) stmt);
            നിർത്തുക;
        അവസ്ഥ STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            നിർത്തുക;
        അവസ്ഥ STMT_UNKNOWN_DECLARATION:
        അവസ്ഥ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported types %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            നിർത്തുക;
        സ്ഥിരസ്ഥിതി:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Key type files may not include other declarations; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            നിർത്തുക;
        }

        എങ്കിൽ (!ok)
            info->error_എണ്ണം++;

        എങ്കിൽ (info->error_എണ്ണം > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning keytypes file \"%s\"\n",
                    safe_map_name(file->name));
            നിർത്തുക;
        }
    }
}

/***====================================================================***/

സ്ഥിതം bool
Copy_താക്കോൽ_Types_To_Keymap(ഘടന xkb_keymap *keymap, താക്കോൽ_Types_Info *info)
{
    സ്ഥിരം darray_size_t അക്കം_types = darray_empty(info->types)
                                  ? 1
                                  : darray_size(info->types);
    ഘടന xkb_താക്കോൽ_type *types = calloc(അക്കം_types, വലുപ്പം(*types));
    എങ്കിൽ (!types)
        തിരികെ false;

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
    എങ്കിൽ (darray_empty(info->types)) {
        ഘടന xkb_താക്കോൽ_type *type = &types[0];

        type->mods.mods = 0;
        type->അക്കം_levels = 1;
        type->entries = ശൂന്യ;
        type->അക്കം_entries = 0;
        type->name = xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL");
        type->level_names = ശൂന്യ;
        type->അക്കം_level_names = 0;
        type->required = true;
    }
    അല്ലെങ്കിൽ {
        /* HACK to circumvent Xorg bugs (see comment above) */
        സ്ഥിരം xkb_atom_t canonical_types[] = {
            xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "TWO_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "ALPHABETIC"),
            xkb_atom_intern_literal(keymap->ctx, "KEYPAD"),
        };

        വേണ്ടി (darray_size_t i = 0; i < അക്കം_types; i++) {
            താക്കോൽ_Type_Info *def = &darray_item(info->types, i);
            ഘടന xkb_താക്കോൽ_type *type = &types[i];

            type->name = def->name;
            type->mods.mods = def->mods;
            type->അക്കം_levels = def->അക്കം_levels;
            type->അക്കം_level_names =
                (xkb_level_index_t) darray_size(def->level_names);
            darray_steal(def->level_names, &type->level_names, ശൂന്യ);
            darray_steal(def->entries, &type->entries, &type->അക്കം_entries);
            type->required = false;

            /* HACK: Never drop canonical XKB key types (see comment above) */
            എങ്കിൽ (type->അക്കം_levels <= 2) {
                വേണ്ടി (uint8_t t = 0;
                     t < (uint8_t) ARRAY_SIZE(canonical_types);
                     t++) {
                        എങ്കിൽ (type->name == canonical_types[t]) {
                            type->required = true;
                            നിർത്തുക;
                        }
                }
            }
        }
    }

    keymap->types_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->types_section_name);
    keymap->അക്കം_types = അക്കം_types;
    keymap->types = types;
    keymap->mods = info->mods;
    തിരികെ true;
}

/***====================================================================***/

bool
Compile_താക്കോൽ_Types(XkbFile *file, ഘടന xkb_keymap_info *keymap_info)
{
    താക്കോൽ_Types_Info info;

    Init_താക്കോൽ_Types_Info(&info, keymap_info, 0,
                     &keymap_info->keymap.mods);

    എങ്കിൽ (file != ശൂന്യ)
        Handle_താക്കോൽ_Types_File(&info, file);

    എങ്കിൽ (info.error_എണ്ണം != 0)
        പോകുക err_info;

    എങ്കിൽ (!Copy_താക്കോൽ_Types_To_Keymap(&keymap_info->keymap, &info))
        പോകുക err_info;

    Clear_താക്കോൽ_Types_Info(&info);
    തിരികെ true;

err_info:
    Clear_താക്കോൽ_Types_Info(&info);
    തിരികെ false;
}
