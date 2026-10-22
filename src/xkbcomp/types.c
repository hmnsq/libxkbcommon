/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <limits.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "xkbcommon/xkbcommon-status.h"
#अन्तर्भाव "xkbcomp-priv.h"
#अन्तर्भाव "text.h"
#अन्तर्भाव "vmod.h"
#अन्तर्भाव "expr.h"
#अन्तर्भाव "include.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "util-mem.h"

गणना type_field {
    TYPE_FIELD_MASK = (1 << 0),
    TYPE_FIELD_MAP = (1 << 1),
    TYPE_FIELD_PRESERVE = (1 << 2),
    TYPE_FIELD_LEVEL_NAME = (1 << 3),
};

प्रकारनिर्देश संरचना {
    गणना type_field defined;
    गणना merge_mode merge;

    xkb_atom_t name;
    xkb_mod_mask_t mods;
    xkb_level_index_t अङ्कः_levels;
    darray(संरचना xkb_कुञ्चिका_type_entry) entries;
    darray(xkb_atom_t) level_names;
} कुञ्चिका_Type_Info;

प्रकारनिर्देश संरचना {
    वर्ण *name;
    पूर्णाङ्क error_गणनम्;
    unsigned पूर्णाङ्क include_depth;

    darray(कुञ्चिका_Type_Info) types;
    संरचना xkb_mod_set mods;

    संरचना xkb_context *ctx;
    ध्रुव संरचना xkb_keymap_info *keymap_info;
} कुञ्चिका_Types_Info;

/***====================================================================***/

स्थिर अन्तःस्थ ध्रुव वर्ण *
MapEntryTxt(कुञ्चिका_Types_Info *info, संरचना xkb_कुञ्चिका_type_entry *entry)
{
    पटिनिवत्त ModMaskText(info->ctx, MOD_BOTH, &info->mods, entry->mods.mods);
}

स्थिर अन्तःस्थ ध्रुव वर्ण *
TypeTxt(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *type)
{
    पटिनिवत्त xkb_atom_text(info->ctx, type->name);
}

स्थिर अन्तःस्थ ध्रुव वर्ण *
TypeMaskTxt(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *type)
{
    पटिनिवत्त ModMaskText(info->ctx, MOD_BOTH, &info->mods, type->mods);
}

स्थिर अन्तःस्थ bool
ReportTypeShouldBeArray(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *type,
                        ध्रुव वर्ण *field)
{
    पटिनिवत्त ReportShouldBeArray(info->ctx, "key type", field,
                               TypeTxt(info, type));
}

स्थिर अन्तःस्थ bool
ReportTypeBadType(कुञ्चिका_Types_Info *info, गणना xkb_message_code code,
                  कुञ्चिका_Type_Info *type, ध्रुव वर्ण *field, ध्रुव वर्ण *wanted)
{
    पटिनिवत्त ReportBadType(info->ctx, code, "key type", field,
                         TypeTxt(info, type), wanted);
}

/***====================================================================***/

स्थिर शून्य
Init_कुञ्चिका_Types_Info(कुञ्चिका_Types_Info *info, ध्रुव संरचना xkb_keymap_info *keymap_info,
                 unsigned पूर्णाङ्क include_depth,
                 ध्रुव संरचना xkb_mod_set *mods)
{
    memset(info, 0, परिमाण(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    InitVMods(&info->mods, mods, include_depth > 0);
}

स्थिर शून्य
Clear_कुञ्चिका_Type_Info(कुञ्चिका_Type_Info *type)
{
    darray_free(type->entries);
    darray_free(type->level_names);
}

स्थिर शून्य
Clear_कुञ्चिका_Types_Info(कुञ्चिका_Types_Info *info)
{
    मुक्त(info->name);
    कुञ्चिका_Type_Info *type;
    darray_foreach(type, info->types)
        Clear_कुञ्चिका_Type_Info(type);
    darray_free(info->types);
}

स्थिर कुञ्चिका_Type_Info *
Find_Matching_कुञ्चिका_Type(कुञ्चिका_Types_Info *info, xkb_atom_t name)
{
    कुञ्चिका_Type_Info *old;

    darray_foreach(old, info->types)
        सचे (old->name == name)
            पटिनिवत्त old;

    पटिनिवत्त रिक्त;
}

स्थिर bool
Add_कुञ्चिका_Type(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *new, bool same_file)
{
    कुञ्चिका_Type_Info *old;
    ध्रुव पूर्णाङ्क verbosity = xkb_context_get_log_verbosity(info->ctx);

    old = Find_Matching_कुञ्चिका_Type(info, new->name);
    सचे (old) {
        सचे (new->merge != MERGE_AUGMENT) {
            सचे ((same_file && verbosity > 0) || verbosity > 9) {
                log_warn(info->ctx,
                         XKB_WARNING_CONFLICTING_कुञ्चिका_TYPE_DEFINITIONS,
                         "Multiple definitions of the %s key type; "
                         "Earlier definition ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            }

            Clear_कुञ्चिका_Type_Info(old);
            *old = *new;
            darray_init(new->entries);
            darray_init(new->level_names);
            पटिनिवत्त true;
        }

        सचे (same_file)
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_CONFLICTING_कुञ्चिका_TYPE_DEFINITIONS,
                    "Multiple definitions of the %s key type; "
                    "Later definition ignored\n",
                    xkb_atom_text(info->ctx, new->name));

        Clear_कुञ्चिका_Type_Info(new);
        पटिनिवत्त true;
    }

    darray_append(info->types, *new);
    पटिनिवत्त true;
}

/***====================================================================***/

स्थिर शून्य
Merge_Included_कुञ्चिका_Types(कुञ्चिका_Types_Info *into, कुञ्चिका_Types_Info *from,
                      गणना merge_mode merge)
{
    सचे (from->error_गणनम् > 0) {
        into->error_गणनम् += from->error_गणनम्;
        पटिनिवत्त;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    सचे (into->name == रिक्त) {
        into->name = steal(&from->name);
    }

    सचे (darray_empty(into->types)) {
        into->types = from->types;
        /* Types stolen via shallow copy, so reinitialize the array */
        darray_init(from->types);
    }
    अञ्ञथा {
        कुञ्चिका_Type_Info *type;
        darray_foreach(type, from->types) {
            type->merge = merge;
            सचे (!Add_कुञ्चिका_Type(into, type, false))
                into->error_गणनम्++;
        }
        /* Types were either shallow copied or reinitialized individually
           in `AddKeyType`, so we only need to free the array */
        darray_free(from->types);
    }
}

स्थिर शून्य
Handle_कुञ्चिका_Types_File(कुञ्चिका_Types_Info *info, XkbFile *file);

स्थिर bool
Handle_Include_कुञ्चिका_Types(कुञ्चिका_Types_Info *info, IncludeStmt *include)
{
    कुञ्चिका_Types_Info included;

    सचे (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_गणनम् += 10;
        पटिनिवत्त false;
    }

    Init_कुञ्चिका_Types_Info(&included, info->keymap_info, info->include_depth + 1,
                     &info->mods);
    included.name = steal(&include->stmt);

    ध्रुव संरचना parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    अत्थाय (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        कुञ्चिका_Types_Info next_incl;
        XkbFile *file;

        वर्ण path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_TYPES,
                                  path, परिमाण(path));
        सचे (!file) {
            info->error_गणनम् += 10;
            Clear_कुञ्चिका_Types_Info(&included);
            पटिनिवत्त false;
        }

        Init_कुञ्चिका_Types_Info(&next_incl, info->keymap_info, info->include_depth + 1,
                         &included.mods);

        Handle_कुञ्चिका_Types_File(&next_incl, file);

        Merge_Included_कुञ्चिका_Types(&included, &next_incl, stmt->merge);

        Clear_कुञ्चिका_Types_Info(&next_incl);
        FreeXkbFile(file);
    }

    Merge_Included_कुञ्चिका_Types(info, &included, include->merge);
    Clear_कुञ्चिका_Types_Info(&included);

    पटिनिवत्त (info->error_गणनम् == 0);
}

/***====================================================================***/

स्थिर bool
SetModifiers(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *type, व्यञ्जकम्_Def *arrayNdx,
             व्यञ्जकम्_Def *मानम्)
{
    xkb_mod_mask_t mods = 0;

    सचे (arrayNdx) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The modifiers field of a key type is not an array; "
                "Illegal array subscript ignored\n");
        पटिनिवत्त false;
    }

    सचे (!व्यञ्जकम्_Resolve_Mod_Mask(info->ctx, मानम्, MOD_BOTH, &info->mods, &mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Key type mask field must be a modifier mask; "
                "Key type definition ignored\n");
        पटिनिवत्त false;
    }

    सचे (type->defined & TYPE_FIELD_MASK) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple modifier mask definitions for key type %s; "
                 "Using %s, ignoring %s\n",
                 xkb_atom_text(info->ctx, type->name),
                 TypeMaskTxt(info, type),
                 ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods));
        पटिनिवत्त false;
    }

    type->mods = mods;
    पटिनिवत्त true;
}

/***====================================================================***/

स्थिर संरचना xkb_कुञ्चिका_type_entry *
FindMatchingMapEntry(कुञ्चिका_Type_Info *type, xkb_mod_mask_t mods)
{
    संरचना xkb_कुञ्चिका_type_entry *entry;

    darray_foreach(entry, type->entries)
        सचे (entry->mods.mods == mods)
            पटिनिवत्त entry;

    पटिनिवत्त रिक्त;
}

स्थिर bool
AddMapEntry(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *type,
            संरचना xkb_कुञ्चिका_type_entry *new, bool clobber, bool report)
{
    संरचना xkb_कुञ्चिका_type_entry *old;

    old = FindMatchingMapEntry(type, new->mods.mods);
    सचे (old) {
        सचे (report && old->level != new->level) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_TYPE_MAP_ENTRY,
                     "Multiple map entries for %s in %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     MapEntryTxt(info, new), TypeTxt(info, type),
                     (clobber ? new->level : old->level) + 1,
                     (clobber ? old->level : new->level) + 1);
        }
        अञ्ञथा {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_CONFLICTING_कुञ्चिका_TYPE_MAP_ENTRY,
                    "Multiple occurrences of map[%s]= %"PRIu32" in %s; Ignored\n",
                    MapEntryTxt(info, new), new->level + 1,
                    TypeTxt(info, type));
            पटिनिवत्त true;
        }

        सचे (clobber) {
            सचे (new->level >= type->अङ्कः_levels)
                type->अङ्कः_levels = new->level + 1;
            old->level = new->level;
        }

        पटिनिवत्त true;
    }

    सचे (new->level >= type->अङ्कः_levels)
        type->अङ्कः_levels = new->level + 1;

    darray_append(type->entries, *new);
    पटिनिवत्त true;
}

स्थिर bool
SetMapEntry(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *type, व्यञ्जकम्_Def *arrayNdx,
            व्यञ्जकम्_Def *मानम्)
{
    संरचना xkb_कुञ्चिका_type_entry entry;

    सचे (arrayNdx == रिक्त)
        पटिनिवत्त ReportTypeShouldBeArray(info, type, "map entry");

    सचे (!व्यञ्जकम्_Resolve_Mod_Mask(info->ctx, arrayNdx, MOD_BOTH, &info->mods,
                            &entry.mods.mods))
        पटिनिवत्त ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "map entry", "modifier mask");

    सचे (entry.mods.mods & (~type->mods)) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_IN_कुञ्चिका_TYPE,
                "Map entry for modifiers not used by type %s; "
                "Using %s instead of %s\n",
                TypeTxt(info, type),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods,
                            entry.mods.mods & type->mods),
                MapEntryTxt(info, &entry));
        entry.mods.mods &= type->mods;
    }

    सचे (!व्यञ्जकम्_Resolve_Level(info->ctx, मानम्, &entry.level)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                          "Level specifications in a key type must be integer; "
                          "Ignoring malformed level specification\n");
        पटिनिवत्त false;
    }

    entry.preserve.mods = 0;

    पटिनिवत्त AddMapEntry(info, type, &entry, true, true);
}

/***====================================================================***/

स्थिर bool
AddPreserve(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *type,
            xkb_mod_mask_t mods, xkb_mod_mask_t preserve_mods)
{
    संरचना xkb_कुञ्चिका_type_entry *entry;

    darray_foreach(entry, type->entries) {
        सचे (entry->mods.mods != mods)
            अनुवर्त;

        /* Map exists without previous preserve (or "None"); override. */
        सचे (entry->preserve.mods == 0) {
            entry->preserve.mods = preserve_mods;
            पटिनिवत्त true;
        }

        /* Map exists with same preserve; do nothing. */
        सचे (entry->preserve.mods == preserve_mods) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_DUPLICATE_ENTRY,
                    "Identical definitions for preserve[%s] in %s; "
                    "Ignored\n",
                    ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                    TypeTxt(info, type));
            पटिनिवत्त true;
        }

        /* Map exists with different preserve; latter wins. */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_कुञ्चिका_TYPE_PRESERVE_ENTRIES,
                "Multiple definitions for preserve[%s] in %s; "
                "Using %s, ignoring %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods,
                            entry->preserve.mods));

        entry->preserve.mods = preserve_mods;
        पटिनिवत्त true;
    }

    /*
     * Map does not exist, i.e. preserve[] came before map[].
     * Create a map with the specified mask mapping to Level1. The level
     * may be overridden later with an explicit map[] statement.
     */
    ध्रुव संरचना xkb_कुञ्चिका_type_entry new = {
        .level = 0,
        .mods = { .mods = mods },
        .preserve = { .mods = preserve_mods },
    };
    darray_append(type->entries, new);
    पटिनिवत्त true;
}

स्थिर bool
SetPreserve(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *type, व्यञ्जकम्_Def *arrayNdx,
            व्यञ्जकम्_Def *मानम्)
{

    सचे (arrayNdx == रिक्त)
        पटिनिवत्त ReportTypeShouldBeArray(info, type, "preserve entry");

    xkb_mod_mask_t mods = 0;
    सचे (!व्यञ्जकम्_Resolve_Mod_Mask(info->ctx, arrayNdx, MOD_BOTH, &info->mods, &mods))
        पटिनिवत्त ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "preserve entry", "modifier mask");

    सचे (mods & ~type->mods) {
        ध्रुव वर्ण *before, *after;

        before = ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods);
        mods &= type->mods;
        after = ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_IN_कुञ्चिका_TYPE,
                "Preserve entry for modifiers not used by the %s type; "
                "Index %s converted to %s\n",
                TypeTxt(info, type), before, after);
    }

    xkb_mod_mask_t preserve_mods = 0;
    सचे (!व्यञ्जकम्_Resolve_Mod_Mask(info->ctx, मानम्, MOD_BOTH, &info->mods,
                            &preserve_mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Preserve value in a key type is not a modifier mask; "
                "Ignoring preserve[%s] in type %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type));
        पटिनिवत्त false;
    }

    सचे (preserve_mods & ~mods) {
        ध्रुव वर्ण *before, *after;

        before = ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods);
        preserve_mods &= mods;
        after = ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_ILLEGAL_कुञ्चिका_TYPE_PRESERVE_RESULT,
                "Illegal value for preserve[%s] in type %s; "
                "Converted %s to %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type), before, after);
    }

    पटिनिवत्त AddPreserve(info, type, mods, preserve_mods);
}

/***====================================================================***/

स्थिर bool
AddLevelName(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *type,
             xkb_level_index_t level, xkb_atom_t name, bool clobber)
{
    /* New name. */
    सचे (level >= darray_size(type->level_names)) {
        darray_resize0(type->level_names, level + 1);
        गच्छ finish;
    }

    /* Same level, same name. */
    सचे (darray_item(type->level_names, level) == name) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE, XKB_WARNING_DUPLICATE_ENTRY,
                "Duplicate names for level %"PRIu32" of key type %s; Ignored\n",
                level + 1, TypeTxt(info, type));
        पटिनिवत्त true;
    }

    /* Same level, different name. */
    सचे (darray_item(type->level_names, level) != XKB_ATOM_NONE) {
        ध्रुव वर्ण *old, *new;
        old = xkb_atom_text(info->ctx,
                            darray_item(type->level_names, level));
        new = xkb_atom_text(info->ctx, name);
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_कुञ्चिका_TYPE_LEVEL_NAMES,
                "Multiple names for level %"PRIu32" of key type %s; "
                "Using %s, ignoring %s\n",
                level + 1, TypeTxt(info, type),
                (clobber ? new : old), (clobber ? old : new));

        सचे (!clobber)
            पटिनिवत्त true;
    }

    /* FIXME: What about different level, same name? */

finish:
    darray_item(type->level_names, level) = name;
    पटिनिवत्त true;
}

स्थिर bool
SetLevelName(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *type, व्यञ्जकम्_Def *arrayNdx,
             व्यञ्जकम्_Def *मानम्)
{
    सचे (arrayNdx == रिक्त)
        पटिनिवत्त ReportTypeShouldBeArray(info, type, "level name");

    xkb_level_index_t level = 0;
    सचे (!व्यञ्जकम्_Resolve_Level(info->ctx, arrayNdx, &level))
        पटिनिवत्त ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                                 type, "level name", "integer");

    xkb_atom_t level_name = XKB_ATOM_NONE;
    सचे (!व्यञ्जकम्_Resolve_String(info->ctx, मानम्, &level_name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Non-string name for level %"PRIu32" in key type %s; "
                "Ignoring illegal level name definition\n",
                level + 1, xkb_atom_text(info->ctx, type->name));
        पटिनिवत्त false;
    }

    पटिनिवत्त AddLevelName(info, type, level, level_name, true);
}

/***====================================================================***/

स्थिर bool
Set_कुञ्चिका_Type_Field(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *type,
                ध्रुव वर्ण *field, व्यञ्जकम्_Def *arrayNdx, व्यञ्जकम्_Def *मानम्)
{
    bool ok = false;
    गणना type_field type_field = 0;

    सचे (istreq(field, "modifiers")) {
        type_field = TYPE_FIELD_MASK;
        ok = SetModifiers(info, type, arrayNdx, मानम्);
    }
    अञ्ञथा सचे (istreq(field, "map")) {
        type_field = TYPE_FIELD_MAP;
        ok = SetMapEntry(info, type, arrayNdx, मानम्);
    }
    अञ्ञथा सचे (istreq(field, "preserve")) {
        type_field = TYPE_FIELD_PRESERVE;
        ok = SetPreserve(info, type, arrayNdx, मानम्);
    }
    अञ्ञथा सचे (istreq(field, "levelname") || istreq(field, "level_name")) {
        type_field = TYPE_FIELD_LEVEL_NAME;
        ok = SetLevelName(info, type, arrayNdx, मानम्);
    } अञ्ञथा {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in key type \"%s\"; Definition ignored\n",
                field, TypeTxt(info, type));
        ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_TYPE_FIELDS);
    }

    type->defined |= type_field;
    पटिनिवत्त ok;
}

स्थिर bool
Handle_कुञ्चिका_Type_Body(कुञ्चिका_Types_Info *info, VarDef *def, कुञ्चिका_Type_Info *type)
{
    bool ok = true;
    ध्रुव वर्ण *elem, *field;
    व्यञ्जकम्_Def *arrayNdx;

    अत्थाय (; def; def = (VarDef *) def->common.next) {
        सचे (!व्यञ्जकम्_Resolve_Lhs(info->ctx, def->name, &elem, &field,
                            &arrayNdx)) {
            /* internal error, already reported */
            ok = false;
            अनुवर्त;
        }

        सचे (elem) {
            सचे (istreq(elem, "type")) {
                log_err(info->ctx, XKB_ERROR_INVALID_SET_DEFAULT_STATEMENT,
                        "Support for changing the default type has been removed; "
                        "Statement \"%s.%s\" ignored.\n", elem, field);
            }
            अञ्ञथा {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key type statement: move statements to the global "
                        "file scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
            अनुवर्त;
        }

        सचे (!Set_कुञ्चिका_Type_Field(info, type, field, arrayNdx, def->मानम्))
            ok = false;
    }

    पटिनिवत्त ok;
}

स्थिर bool
Handle_कुञ्चिका_Type_Def(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Def *def)
{
    कुञ्चिका_Type_Info type = {
        .defined = 0,
        .merge = def->merge,
        .name = def->name,
        .mods = 0,
        .अङ्कः_levels = 1,
        .entries = darray_new(),
        .level_names = darray_new(),
    };

    सचे (!Handle_कुञ्चिका_Type_Body(info, def->body, &type) ||
        !Add_कुञ्चिका_Type(info, &type, true))
    {
        info->error_गणनम्++;
        Clear_कुञ्चिका_Type_Info(&type);
        पटिनिवत्त false;
    }

    /* Type has been either stolen via shallow copy or reinitialized in
       `AddKeyType`: no need to free the arrays */
    पटिनिवत्त true;
}

स्थिर bool
HandleGlobalVar(कुञ्चिका_Types_Info *info, VarDef *stmt)
{
    ध्रुव वर्ण *elem, *field;
    व्यञ्जकम्_Def *arrayNdx;

    सचे (!व्यञ्जकम्_Resolve_Lhs(info->ctx, stmt->name, &elem, &field,
                        &arrayNdx)) {
        पटिनिवत्त false;           /* internal error, already reported */
    }
    अञ्ञथा सचे (elem && istreq(elem, "type")) {
        log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                "Support for changing the default type has been removed; "
                "Statement ignored\n");
        पटिनिवत्त true;
    }
    अञ्ञथा सचे (elem) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown element \"%s\"; "
                "Value for field \"%s.%s\" ignored\n", elem, elem, field);
        पटिनिवत्त !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
    }
    अञ्ञथा सचे (field) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        पटिनिवत्त !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS);
    }
    पटिनिवत्त false;
}

स्थिर शून्य
Handle_कुञ्चिका_Types_File(कुञ्चिका_Types_Info *info, XkbFile *file)
{
    bool ok;

    मुक्त(info->name);
    info->name = strdup_safe(file->name);

    अत्थाय (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        विकल्प (stmt->type) {
        पक्ष STMT_INCLUDE:
            ok = Handle_Include_कुञ्चिका_Types(info, (IncludeStmt *) stmt);
            भङ्ग;
        पक्ष STMT_TYPE:
            ok = Handle_कुञ्चिका_Type_Def(info, (कुञ्चिका_Type_Def *) stmt);
            भङ्ग;
        पक्ष STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef*) stmt);
            भङ्ग;
        पक्ष STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            भङ्ग;
        पक्ष STMT_UNKNOWN_DECLARATION:
        पक्ष STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported types %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            भङ्ग;
        स्वभाव:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Key type files may not include other declarations; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            भङ्ग;
        }

        सचे (!ok)
            info->error_गणनम्++;

        सचे (info->error_गणनम् > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning keytypes file \"%s\"\n",
                    safe_map_name(file->name));
            भङ्ग;
        }
    }
}

/***====================================================================***/

स्थिर bool
Copy_कुञ्चिका_Types_To_Keymap(संरचना xkb_keymap *keymap, कुञ्चिका_Types_Info *info)
{
    ध्रुव darray_size_t अङ्कः_types = darray_empty(info->types)
                                  ? 1
                                  : darray_size(info->types);
    संरचना xkb_कुञ्चिका_type *types = calloc(अङ्कः_types, परिमाण(*types));
    सचे (!types)
        पटिनिवत्त false;

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
    सचे (darray_empty(info->types)) {
        संरचना xkb_कुञ्चिका_type *type = &types[0];

        type->mods.mods = 0;
        type->अङ्कः_levels = 1;
        type->entries = रिक्त;
        type->अङ्कः_entries = 0;
        type->name = xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL");
        type->level_names = रिक्त;
        type->अङ्कः_level_names = 0;
        type->required = true;
    }
    अञ्ञथा {
        /* HACK to circumvent Xorg bugs (see comment above) */
        ध्रुव xkb_atom_t canonical_types[] = {
            xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "TWO_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "ALPHABETIC"),
            xkb_atom_intern_literal(keymap->ctx, "KEYPAD"),
        };

        अत्थाय (darray_size_t i = 0; i < अङ्कः_types; i++) {
            कुञ्चिका_Type_Info *def = &darray_item(info->types, i);
            संरचना xkb_कुञ्चिका_type *type = &types[i];

            type->name = def->name;
            type->mods.mods = def->mods;
            type->अङ्कः_levels = def->अङ्कः_levels;
            type->अङ्कः_level_names =
                (xkb_level_index_t) darray_size(def->level_names);
            darray_steal(def->level_names, &type->level_names, रिक्त);
            darray_steal(def->entries, &type->entries, &type->अङ्कः_entries);
            type->required = false;

            /* HACK: Never drop canonical XKB key types (see comment above) */
            सचे (type->अङ्कः_levels <= 2) {
                अत्थाय (uint8_t t = 0;
                     t < (uint8_t) ARRAY_SIZE(canonical_types);
                     t++) {
                        सचे (type->name == canonical_types[t]) {
                            type->required = true;
                            भङ्ग;
                        }
                }
            }
        }
    }

    keymap->types_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->types_section_name);
    keymap->अङ्कः_types = अङ्कः_types;
    keymap->types = types;
    keymap->mods = info->mods;
    पटिनिवत्त true;
}

/***====================================================================***/

bool
Compile_कुञ्चिका_Types(XkbFile *file, संरचना xkb_keymap_info *keymap_info)
{
    कुञ्चिका_Types_Info info;

    Init_कुञ्चिका_Types_Info(&info, keymap_info, 0,
                     &keymap_info->keymap.mods);

    सचे (file != रिक्त)
        Handle_कुञ्चिका_Types_File(&info, file);

    सचे (info.error_गणनम् != 0)
        गच्छ err_info;

    सचे (!Copy_कुञ्चिका_Types_To_Keymap(&keymap_info->keymap, &info))
        गच्छ err_info;

    Clear_कुञ्चिका_Types_Info(&info);
    पटिनिवत्त true;

err_info:
    Clear_कुञ्चिका_Types_Info(&info);
    पटिनिवत्त false;
}
