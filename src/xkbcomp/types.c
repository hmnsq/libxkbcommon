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

प्रकारव्याख्या संरचना {
    गणना type_field defined;
    गणना merge_mode merge;

    xkb_atom_t name;
    xkb_mod_mask_t mods;
    xkb_level_index_t अंक_levels;
    darray(संरचना xkb_किल्ली_type_entry) entries;
    darray(xkb_atom_t) level_names;
} किल्ली_Type_Info;

प्रकारव्याख्या संरचना {
    अक्षर *name;
    पूर्णांक error_मोजणी;
    अचिन्हित पूर्णांक include_depth;

    darray(किल्ली_Type_Info) types;
    संरचना xkb_mod_set mods;

    संरचना xkb_context *ctx;
    स्थिरांक संरचना xkb_keymap_info *keymap_info;
} किल्ली_Types_Info;

/***====================================================================***/

स्थिर ओळीत स्थिरांक अक्षर *
MapEntryTxt(किल्ली_Types_Info *info, संरचना xkb_किल्ली_type_entry *entry)
{
    परत ModMaskText(info->ctx, MOD_BOTH, &info->mods, entry->mods.mods);
}

स्थिर ओळीत स्थिरांक अक्षर *
TypeTxt(किल्ली_Types_Info *info, किल्ली_Type_Info *type)
{
    परत xkb_atom_text(info->ctx, type->name);
}

स्थिर ओळीत स्थिरांक अक्षर *
TypeMaskTxt(किल्ली_Types_Info *info, किल्ली_Type_Info *type)
{
    परत ModMaskText(info->ctx, MOD_BOTH, &info->mods, type->mods);
}

स्थिर ओळीत bool
ReportTypeShouldBeArray(किल्ली_Types_Info *info, किल्ली_Type_Info *type,
                        स्थिरांक अक्षर *field)
{
    परत ReportShouldBeArray(info->ctx, "key type", field,
                               TypeTxt(info, type));
}

स्थिर ओळीत bool
ReportTypeBadType(किल्ली_Types_Info *info, गणना xkb_message_code code,
                  किल्ली_Type_Info *type, स्थिरांक अक्षर *field, स्थिरांक अक्षर *wanted)
{
    परत ReportBadType(info->ctx, code, "key type", field,
                         TypeTxt(info, type), wanted);
}

/***====================================================================***/

स्थिर शून्य
Init_किल्ली_Types_Info(किल्ली_Types_Info *info, स्थिरांक संरचना xkb_keymap_info *keymap_info,
                 अचिन्हित पूर्णांक include_depth,
                 स्थिरांक संरचना xkb_mod_set *mods)
{
    स_रखो(info, 0, आकार(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    InitVMods(&info->mods, mods, include_depth > 0);
}

स्थिर शून्य
Clear_किल्ली_Type_Info(किल्ली_Type_Info *type)
{
    darray_free(type->entries);
    darray_free(type->level_names);
}

स्थिर शून्य
Clear_किल्ली_Types_Info(किल्ली_Types_Info *info)
{
    मुक्त(info->name);
    किल्ली_Type_Info *type;
    darray_foreach(type, info->types)
        Clear_किल्ली_Type_Info(type);
    darray_free(info->types);
}

स्थिर किल्ली_Type_Info *
Find_Matching_किल्ली_Type(किल्ली_Types_Info *info, xkb_atom_t name)
{
    किल्ली_Type_Info *old;

    darray_foreach(old, info->types)
        जर (old->name == name)
            परत old;

    परत रिक्त;
}

स्थिर bool
Add_किल्ली_Type(किल्ली_Types_Info *info, किल्ली_Type_Info *new, bool same_file)
{
    किल्ली_Type_Info *old;
    स्थिरांक पूर्णांक verbosity = xkb_context_get_log_verbosity(info->ctx);

    old = Find_Matching_किल्ली_Type(info, new->name);
    जर (old) {
        जर (new->merge != MERGE_AUGMENT) {
            जर ((same_file && verbosity > 0) || verbosity > 9) {
                log_warn(info->ctx,
                         XKB_WARNING_CONFLICTING_किल्ली_TYPE_DEFINITIONS,
                         "Multiple definitions of the %s key type; "
                         "Earlier definition ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            }

            Clear_किल्ली_Type_Info(old);
            *old = *new;
            darray_init(new->entries);
            darray_init(new->level_names);
            परत true;
        }

        जर (same_file)
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_CONFLICTING_किल्ली_TYPE_DEFINITIONS,
                    "Multiple definitions of the %s key type; "
                    "Later definition ignored\n",
                    xkb_atom_text(info->ctx, new->name));

        Clear_किल्ली_Type_Info(new);
        परत true;
    }

    darray_append(info->types, *new);
    परत true;
}

/***====================================================================***/

स्थिर शून्य
Merge_Included_किल्ली_Types(किल्ली_Types_Info *into, किल्ली_Types_Info *from,
                      गणना merge_mode merge)
{
    जर (from->error_मोजणी > 0) {
        into->error_मोजणी += from->error_मोजणी;
        परत;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    जर (into->name == रिक्त) {
        into->name = steal(&from->name);
    }

    जर (darray_empty(into->types)) {
        into->types = from->types;
        /* Types stolen via shallow copy, so reinitialize the array */
        darray_init(from->types);
    }
    अन्यथा {
        किल्ली_Type_Info *type;
        darray_foreach(type, from->types) {
            type->merge = merge;
            जर (!Add_किल्ली_Type(into, type, false))
                into->error_मोजणी++;
        }
        /* Types were either shallow copied or reinitialized individually
           in `AddKeyType`, so we only need to free the array */
        darray_free(from->types);
    }
}

स्थिर शून्य
Handle_किल्ली_Types_File(किल्ली_Types_Info *info, XkbFile *file);

स्थिर bool
Handle_Include_किल्ली_Types(किल्ली_Types_Info *info, IncludeStmt *include)
{
    किल्ली_Types_Info included;

    जर (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_मोजणी += 10;
        परत false;
    }

    Init_किल्ली_Types_Info(&included, info->keymap_info, info->include_depth + 1,
                     &info->mods);
    included.name = steal(&include->stmt);

    स्थिरांक संरचना parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    साठी (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        किल्ली_Types_Info next_incl;
        XkbFile *file;

        अक्षर path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_TYPES,
                                  path, आकार(path));
        जर (!file) {
            info->error_मोजणी += 10;
            Clear_किल्ली_Types_Info(&included);
            परत false;
        }

        Init_किल्ली_Types_Info(&next_incl, info->keymap_info, info->include_depth + 1,
                         &included.mods);

        Handle_किल्ली_Types_File(&next_incl, file);

        Merge_Included_किल्ली_Types(&included, &next_incl, stmt->merge);

        Clear_किल्ली_Types_Info(&next_incl);
        FreeXkbFile(file);
    }

    Merge_Included_किल्ली_Types(info, &included, include->merge);
    Clear_किल्ली_Types_Info(&included);

    परत (info->error_मोजणी == 0);
}

/***====================================================================***/

स्थिर bool
SetModifiers(किल्ली_Types_Info *info, किल्ली_Type_Info *type, अभिव्यक्ती_Def *arrayNdx,
             अभिव्यक्ती_Def *मूल्य)
{
    xkb_mod_mask_t mods = 0;

    जर (arrayNdx) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The modifiers field of a key type is not an array; "
                "Illegal array subscript ignored\n");
        परत false;
    }

    जर (!अभिव्यक्ती_Resolve_Mod_Mask(info->ctx, मूल्य, MOD_BOTH, &info->mods, &mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Key type mask field must be a modifier mask; "
                "Key type definition ignored\n");
        परत false;
    }

    जर (type->defined & TYPE_FIELD_MASK) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple modifier mask definitions for key type %s; "
                 "Using %s, ignoring %s\n",
                 xkb_atom_text(info->ctx, type->name),
                 TypeMaskTxt(info, type),
                 ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods));
        परत false;
    }

    type->mods = mods;
    परत true;
}

/***====================================================================***/

स्थिर संरचना xkb_किल्ली_type_entry *
FindMatchingMapEntry(किल्ली_Type_Info *type, xkb_mod_mask_t mods)
{
    संरचना xkb_किल्ली_type_entry *entry;

    darray_foreach(entry, type->entries)
        जर (entry->mods.mods == mods)
            परत entry;

    परत रिक्त;
}

स्थिर bool
AddMapEntry(किल्ली_Types_Info *info, किल्ली_Type_Info *type,
            संरचना xkb_किल्ली_type_entry *new, bool clobber, bool report)
{
    संरचना xkb_किल्ली_type_entry *old;

    old = FindMatchingMapEntry(type, new->mods.mods);
    जर (old) {
        जर (report && old->level != new->level) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_किल्ली_TYPE_MAP_ENTRY,
                     "Multiple map entries for %s in %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     MapEntryTxt(info, new), TypeTxt(info, type),
                     (clobber ? new->level : old->level) + 1,
                     (clobber ? old->level : new->level) + 1);
        }
        अन्यथा {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_CONFLICTING_किल्ली_TYPE_MAP_ENTRY,
                    "Multiple occurrences of map[%s]= %"PRIu32" in %s; Ignored\n",
                    MapEntryTxt(info, new), new->level + 1,
                    TypeTxt(info, type));
            परत true;
        }

        जर (clobber) {
            जर (new->level >= type->अंक_levels)
                type->अंक_levels = new->level + 1;
            old->level = new->level;
        }

        परत true;
    }

    जर (new->level >= type->अंक_levels)
        type->अंक_levels = new->level + 1;

    darray_append(type->entries, *new);
    परत true;
}

स्थिर bool
SetMapEntry(किल्ली_Types_Info *info, किल्ली_Type_Info *type, अभिव्यक्ती_Def *arrayNdx,
            अभिव्यक्ती_Def *मूल्य)
{
    संरचना xkb_किल्ली_type_entry entry;

    जर (arrayNdx == रिक्त)
        परत ReportTypeShouldBeArray(info, type, "map entry");

    जर (!अभिव्यक्ती_Resolve_Mod_Mask(info->ctx, arrayNdx, MOD_BOTH, &info->mods,
                            &entry.mods.mods))
        परत ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "map entry", "modifier mask");

    जर (entry.mods.mods & (~type->mods)) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_IN_किल्ली_TYPE,
                "Map entry for modifiers not used by type %s; "
                "Using %s instead of %s\n",
                TypeTxt(info, type),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods,
                            entry.mods.mods & type->mods),
                MapEntryTxt(info, &entry));
        entry.mods.mods &= type->mods;
    }

    जर (!अभिव्यक्ती_Resolve_Level(info->ctx, मूल्य, &entry.level)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                          "Level specifications in a key type must be integer; "
                          "Ignoring malformed level specification\n");
        परत false;
    }

    entry.preserve.mods = 0;

    परत AddMapEntry(info, type, &entry, true, true);
}

/***====================================================================***/

स्थिर bool
AddPreserve(किल्ली_Types_Info *info, किल्ली_Type_Info *type,
            xkb_mod_mask_t mods, xkb_mod_mask_t preserve_mods)
{
    संरचना xkb_किल्ली_type_entry *entry;

    darray_foreach(entry, type->entries) {
        जर (entry->mods.mods != mods)
            सुरूठेवा;

        /* Map exists without previous preserve (or "None"); override. */
        जर (entry->preserve.mods == 0) {
            entry->preserve.mods = preserve_mods;
            परत true;
        }

        /* Map exists with same preserve; do nothing. */
        जर (entry->preserve.mods == preserve_mods) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_DUPLICATE_ENTRY,
                    "Identical definitions for preserve[%s] in %s; "
                    "Ignored\n",
                    ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                    TypeTxt(info, type));
            परत true;
        }

        /* Map exists with different preserve; latter wins. */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_किल्ली_TYPE_PRESERVE_ENTRIES,
                "Multiple definitions for preserve[%s] in %s; "
                "Using %s, ignoring %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods,
                            entry->preserve.mods));

        entry->preserve.mods = preserve_mods;
        परत true;
    }

    /*
     * Map does not exist, i.e. preserve[] came before map[].
     * Create a map with the specified mask mapping to Level1. The level
     * may be overridden later with an explicit map[] statement.
     */
    स्थिरांक संरचना xkb_किल्ली_type_entry new = {
        .level = 0,
        .mods = { .mods = mods },
        .preserve = { .mods = preserve_mods },
    };
    darray_append(type->entries, new);
    परत true;
}

स्थिर bool
SetPreserve(किल्ली_Types_Info *info, किल्ली_Type_Info *type, अभिव्यक्ती_Def *arrayNdx,
            अभिव्यक्ती_Def *मूल्य)
{

    जर (arrayNdx == रिक्त)
        परत ReportTypeShouldBeArray(info, type, "preserve entry");

    xkb_mod_mask_t mods = 0;
    जर (!अभिव्यक्ती_Resolve_Mod_Mask(info->ctx, arrayNdx, MOD_BOTH, &info->mods, &mods))
        परत ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "preserve entry", "modifier mask");

    जर (mods & ~type->mods) {
        स्थिरांक अक्षर *before, *after;

        before = ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods);
        mods &= type->mods;
        after = ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_IN_किल्ली_TYPE,
                "Preserve entry for modifiers not used by the %s type; "
                "Index %s converted to %s\n",
                TypeTxt(info, type), before, after);
    }

    xkb_mod_mask_t preserve_mods = 0;
    जर (!अभिव्यक्ती_Resolve_Mod_Mask(info->ctx, मूल्य, MOD_BOTH, &info->mods,
                            &preserve_mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Preserve value in a key type is not a modifier mask; "
                "Ignoring preserve[%s] in type %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type));
        परत false;
    }

    जर (preserve_mods & ~mods) {
        स्थिरांक अक्षर *before, *after;

        before = ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods);
        preserve_mods &= mods;
        after = ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_ILLEGAL_किल्ली_TYPE_PRESERVE_RESULT,
                "Illegal value for preserve[%s] in type %s; "
                "Converted %s to %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type), before, after);
    }

    परत AddPreserve(info, type, mods, preserve_mods);
}

/***====================================================================***/

स्थिर bool
AddLevelName(किल्ली_Types_Info *info, किल्ली_Type_Info *type,
             xkb_level_index_t level, xkb_atom_t name, bool clobber)
{
    /* New name. */
    जर (level >= darray_size(type->level_names)) {
        darray_resize0(type->level_names, level + 1);
        जा finish;
    }

    /* Same level, same name. */
    जर (darray_item(type->level_names, level) == name) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE, XKB_WARNING_DUPLICATE_ENTRY,
                "Duplicate names for level %"PRIu32" of key type %s; Ignored\n",
                level + 1, TypeTxt(info, type));
        परत true;
    }

    /* Same level, different name. */
    जर (darray_item(type->level_names, level) != XKB_ATOM_NONE) {
        स्थिरांक अक्षर *old, *new;
        old = xkb_atom_text(info->ctx,
                            darray_item(type->level_names, level));
        new = xkb_atom_text(info->ctx, name);
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_किल्ली_TYPE_LEVEL_NAMES,
                "Multiple names for level %"PRIu32" of key type %s; "
                "Using %s, ignoring %s\n",
                level + 1, TypeTxt(info, type),
                (clobber ? new : old), (clobber ? old : new));

        जर (!clobber)
            परत true;
    }

    /* FIXME: What about different level, same name? */

finish:
    darray_item(type->level_names, level) = name;
    परत true;
}

स्थिर bool
SetLevelName(किल्ली_Types_Info *info, किल्ली_Type_Info *type, अभिव्यक्ती_Def *arrayNdx,
             अभिव्यक्ती_Def *मूल्य)
{
    जर (arrayNdx == रिक्त)
        परत ReportTypeShouldBeArray(info, type, "level name");

    xkb_level_index_t level = 0;
    जर (!अभिव्यक्ती_Resolve_Level(info->ctx, arrayNdx, &level))
        परत ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                                 type, "level name", "integer");

    xkb_atom_t level_name = XKB_ATOM_NONE;
    जर (!अभिव्यक्ती_Resolve_String(info->ctx, मूल्य, &level_name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Non-string name for level %"PRIu32" in key type %s; "
                "Ignoring illegal level name definition\n",
                level + 1, xkb_atom_text(info->ctx, type->name));
        परत false;
    }

    परत AddLevelName(info, type, level, level_name, true);
}

/***====================================================================***/

स्थिर bool
Set_किल्ली_Type_Field(किल्ली_Types_Info *info, किल्ली_Type_Info *type,
                स्थिरांक अक्षर *field, अभिव्यक्ती_Def *arrayNdx, अभिव्यक्ती_Def *मूल्य)
{
    bool ok = false;
    गणना type_field type_field = 0;

    जर (istreq(field, "modifiers")) {
        type_field = TYPE_FIELD_MASK;
        ok = SetModifiers(info, type, arrayNdx, मूल्य);
    }
    अन्यथा जर (istreq(field, "map")) {
        type_field = TYPE_FIELD_MAP;
        ok = SetMapEntry(info, type, arrayNdx, मूल्य);
    }
    अन्यथा जर (istreq(field, "preserve")) {
        type_field = TYPE_FIELD_PRESERVE;
        ok = SetPreserve(info, type, arrayNdx, मूल्य);
    }
    अन्यथा जर (istreq(field, "levelname") || istreq(field, "level_name")) {
        type_field = TYPE_FIELD_LEVEL_NAME;
        ok = SetLevelName(info, type, arrayNdx, मूल्य);
    } अन्यथा {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in key type \"%s\"; Definition ignored\n",
                field, TypeTxt(info, type));
        ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_TYPE_FIELDS);
    }

    type->defined |= type_field;
    परत ok;
}

स्थिर bool
Handle_किल्ली_Type_Body(किल्ली_Types_Info *info, VarDef *def, किल्ली_Type_Info *type)
{
    bool ok = true;
    स्थिरांक अक्षर *elem, *field;
    अभिव्यक्ती_Def *arrayNdx;

    साठी (; def; def = (VarDef *) def->common.next) {
        जर (!अभिव्यक्ती_Resolve_Lhs(info->ctx, def->name, &elem, &field,
                            &arrayNdx)) {
            /* internal error, already reported */
            ok = false;
            सुरूठेवा;
        }

        जर (elem) {
            जर (istreq(elem, "type")) {
                log_err(info->ctx, XKB_ERROR_INVALID_SET_DEFAULT_STATEMENT,
                        "Support for changing the default type has been removed; "
                        "Statement \"%s.%s\" ignored.\n", elem, field);
            }
            अन्यथा {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key type statement: move statements to the global "
                        "file scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
            सुरूठेवा;
        }

        जर (!Set_किल्ली_Type_Field(info, type, field, arrayNdx, def->मूल्य))
            ok = false;
    }

    परत ok;
}

स्थिर bool
Handle_किल्ली_Type_Def(किल्ली_Types_Info *info, किल्ली_Type_Def *def)
{
    किल्ली_Type_Info type = {
        .defined = 0,
        .merge = def->merge,
        .name = def->name,
        .mods = 0,
        .अंक_levels = 1,
        .entries = darray_new(),
        .level_names = darray_new(),
    };

    जर (!Handle_किल्ली_Type_Body(info, def->body, &type) ||
        !Add_किल्ली_Type(info, &type, true))
    {
        info->error_मोजणी++;
        Clear_किल्ली_Type_Info(&type);
        परत false;
    }

    /* Type has been either stolen via shallow copy or reinitialized in
       `AddKeyType`: no need to free the arrays */
    परत true;
}

स्थिर bool
HandleGlobalVar(किल्ली_Types_Info *info, VarDef *stmt)
{
    स्थिरांक अक्षर *elem, *field;
    अभिव्यक्ती_Def *arrayNdx;

    जर (!अभिव्यक्ती_Resolve_Lhs(info->ctx, stmt->name, &elem, &field,
                        &arrayNdx)) {
        परत false;           /* internal error, already reported */
    }
    अन्यथा जर (elem && istreq(elem, "type")) {
        log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                "Support for changing the default type has been removed; "
                "Statement ignored\n");
        परत true;
    }
    अन्यथा जर (elem) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown element \"%s\"; "
                "Value for field \"%s.%s\" ignored\n", elem, elem, field);
        परत !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
    }
    अन्यथा जर (field) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        परत !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS);
    }
    परत false;
}

स्थिर शून्य
Handle_किल्ली_Types_File(किल्ली_Types_Info *info, XkbFile *file)
{
    bool ok;

    मुक्त(info->name);
    info->name = strdup_safe(file->name);

    साठी (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        निवड (stmt->type) {
        स्थिती STMT_INCLUDE:
            ok = Handle_Include_किल्ली_Types(info, (IncludeStmt *) stmt);
            थांबा;
        स्थिती STMT_TYPE:
            ok = Handle_किल्ली_Type_Def(info, (किल्ली_Type_Def *) stmt);
            थांबा;
        स्थिती STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef*) stmt);
            थांबा;
        स्थिती STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            थांबा;
        स्थिती STMT_UNKNOWN_DECLARATION:
        स्थिती STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported types %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            थांबा;
        पूर्वनिर्धारित:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Key type files may not include other declarations; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            थांबा;
        }

        जर (!ok)
            info->error_मोजणी++;

        जर (info->error_मोजणी > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning keytypes file \"%s\"\n",
                    safe_map_name(file->name));
            थांबा;
        }
    }
}

/***====================================================================***/

स्थिर bool
Copy_किल्ली_Types_To_Keymap(संरचना xkb_keymap *keymap, किल्ली_Types_Info *info)
{
    स्थिरांक darray_size_t अंक_types = darray_empty(info->types)
                                  ? 1
                                  : darray_size(info->types);
    संरचना xkb_किल्ली_type *types = सुस्मृति(अंक_types, आकार(*types));
    जर (!types)
        परत false;

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
    जर (darray_empty(info->types)) {
        संरचना xkb_किल्ली_type *type = &types[0];

        type->mods.mods = 0;
        type->अंक_levels = 1;
        type->entries = रिक्त;
        type->अंक_entries = 0;
        type->name = xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL");
        type->level_names = रिक्त;
        type->अंक_level_names = 0;
        type->required = true;
    }
    अन्यथा {
        /* HACK to circumvent Xorg bugs (see comment above) */
        स्थिरांक xkb_atom_t canonical_types[] = {
            xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "TWO_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "ALPHABETIC"),
            xkb_atom_intern_literal(keymap->ctx, "KEYPAD"),
        };

        साठी (darray_size_t i = 0; i < अंक_types; i++) {
            किल्ली_Type_Info *def = &darray_item(info->types, i);
            संरचना xkb_किल्ली_type *type = &types[i];

            type->name = def->name;
            type->mods.mods = def->mods;
            type->अंक_levels = def->अंक_levels;
            type->अंक_level_names =
                (xkb_level_index_t) darray_size(def->level_names);
            darray_steal(def->level_names, &type->level_names, रिक्त);
            darray_steal(def->entries, &type->entries, &type->अंक_entries);
            type->required = false;

            /* HACK: Never drop canonical XKB key types (see comment above) */
            जर (type->अंक_levels <= 2) {
                साठी (uint8_t t = 0;
                     t < (uint8_t) ARRAY_SIZE(canonical_types);
                     t++) {
                        जर (type->name == canonical_types[t]) {
                            type->required = true;
                            थांबा;
                        }
                }
            }
        }
    }

    keymap->types_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->types_section_name);
    keymap->अंक_types = अंक_types;
    keymap->types = types;
    keymap->mods = info->mods;
    परत true;
}

/***====================================================================***/

bool
Compile_किल्ली_Types(XkbFile *file, संरचना xkb_keymap_info *keymap_info)
{
    किल्ली_Types_Info info;

    Init_किल्ली_Types_Info(&info, keymap_info, 0,
                     &keymap_info->keymap.mods);

    जर (file != रिक्त)
        Handle_किल्ली_Types_File(&info, file);

    जर (info.error_मोजणी != 0)
        जा err_info;

    जर (!Copy_किल्ली_Types_To_Keymap(&keymap_info->keymap, &info))
        जा err_info;

    Clear_किल्ली_Types_Info(&info);
    परत true;

err_info:
    Clear_किल्ली_Types_Info(&info);
    परत false;
}
