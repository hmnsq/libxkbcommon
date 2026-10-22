/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#समावेश "config.h"

#समावेश <limits.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "xkbcommon/xkbcommon-status.h"
#समावेश "xkbcomp-priv.h"
#समावेश "text.h"
#समावेश "vmod.h"
#समावेश "expr.h"
#समावेश "include.h"
#समावेश "utils.h"
#समावेश "util-mem.h"

क्रमागत type_field {
    TYPE_FIELD_MASK = (1 << 0),
    TYPE_FIELD_MAP = (1 << 1),
    TYPE_FIELD_PRESERVE = (1 << 2),
    TYPE_FIELD_LEVEL_NAME = (1 << 3),
};

प्रकार काष्ठा {
    क्रमागत type_field defined;
    क्रमागत merge_mode merge;

    xkb_atom_t name;
    xkb_mod_mask_t mods;
    xkb_level_सूचकांक_t अंक_levels;
    darray(काष्ठा xkb_कुंजी_type_entry) entries;
    darray(xkb_atom_t) level_names;
} कुंजी_Type_Info;

प्रकार काष्ठा {
    अक्षर *name;
    पूर्णांक error_गिनती;
    अचिन्हित पूर्णांक include_depth;

    darray(कुंजी_Type_Info) types;
    काष्ठा xkb_mod_set mods;

    काष्ठा xkb_context *ctx;
    स्थिर काष्ठा xkb_keymap_info *keymap_info;
} कुंजी_Types_Info;

/***====================================================================***/

अटल अंतरभूत स्थिर अक्षर *
MapEntryTxt(कुंजी_Types_Info *info, काष्ठा xkb_कुंजी_type_entry *entry)
{
    वापस ModMaskText(info->ctx, MOD_BOTH, &info->mods, entry->mods.mods);
}

अटल अंतरभूत स्थिर अक्षर *
TypeTxt(कुंजी_Types_Info *info, कुंजी_Type_Info *type)
{
    वापस xkb_atom_text(info->ctx, type->name);
}

अटल अंतरभूत स्थिर अक्षर *
TypeMaskTxt(कुंजी_Types_Info *info, कुंजी_Type_Info *type)
{
    वापस ModMaskText(info->ctx, MOD_BOTH, &info->mods, type->mods);
}

अटल अंतरभूत bool
ReportTypeShouldBeArray(कुंजी_Types_Info *info, कुंजी_Type_Info *type,
                        स्थिर अक्षर *field)
{
    वापस ReportShouldBeArray(info->ctx, "key type", field,
                               TypeTxt(info, type));
}

अटल अंतरभूत bool
ReportTypeBadType(कुंजी_Types_Info *info, क्रमागत xkb_message_code code,
                  कुंजी_Type_Info *type, स्थिर अक्षर *field, स्थिर अक्षर *wanted)
{
    वापस ReportBadType(info->ctx, code, "key type", field,
                         TypeTxt(info, type), wanted);
}

/***====================================================================***/

अटल व्योम
Init_कुंजी_Types_Info(कुंजी_Types_Info *info, स्थिर काष्ठा xkb_keymap_info *keymap_info,
                 अचिन्हित पूर्णांक include_depth,
                 स्थिर काष्ठा xkb_mod_set *mods)
{
    स_रखो(info, 0, माप(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    InitVMods(&info->mods, mods, include_depth > 0);
}

अटल व्योम
Clear_कुंजी_Type_Info(कुंजी_Type_Info *type)
{
    darray_free(type->entries);
    darray_free(type->level_names);
}

अटल व्योम
Clear_कुंजी_Types_Info(कुंजी_Types_Info *info)
{
    मुक्त(info->name);
    कुंजी_Type_Info *type;
    darray_foreach(type, info->types)
        Clear_कुंजी_Type_Info(type);
    darray_free(info->types);
}

अटल कुंजी_Type_Info *
Find_Matching_कुंजी_Type(कुंजी_Types_Info *info, xkb_atom_t name)
{
    कुंजी_Type_Info *old;

    darray_foreach(old, info->types)
        अगर (old->name == name)
            वापस old;

    वापस शून्य;
}

अटल bool
Add_कुंजी_Type(कुंजी_Types_Info *info, कुंजी_Type_Info *new, bool same_file)
{
    कुंजी_Type_Info *old;
    स्थिर पूर्णांक verbosity = xkb_context_get_log_verbosity(info->ctx);

    old = Find_Matching_कुंजी_Type(info, new->name);
    अगर (old) {
        अगर (new->merge != MERGE_AUGMENT) {
            अगर ((same_file && verbosity > 0) || verbosity > 9) {
                log_warn(info->ctx,
                         XKB_WARNING_CONFLICTING_कुंजी_TYPE_DEFINITIONS,
                         "Multiple definitions of the %s key type; "
                         "Earlier definition ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            }

            Clear_कुंजी_Type_Info(old);
            *old = *new;
            darray_init(new->entries);
            darray_init(new->level_names);
            वापस true;
        }

        अगर (same_file)
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_CONFLICTING_कुंजी_TYPE_DEFINITIONS,
                    "Multiple definitions of the %s key type; "
                    "Later definition ignored\n",
                    xkb_atom_text(info->ctx, new->name));

        Clear_कुंजी_Type_Info(new);
        वापस true;
    }

    darray_append(info->types, *new);
    वापस true;
}

/***====================================================================***/

अटल व्योम
Merge_Included_कुंजी_Types(कुंजी_Types_Info *into, कुंजी_Types_Info *from,
                      क्रमागत merge_mode merge)
{
    अगर (from->error_गिनती > 0) {
        into->error_गिनती += from->error_गिनती;
        वापस;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    अगर (into->name == शून्य) {
        into->name = steal(&from->name);
    }

    अगर (darray_empty(into->types)) {
        into->types = from->types;
        /* Types stolen via shallow copy, so reinitialize the array */
        darray_init(from->types);
    }
    अन्यथा {
        कुंजी_Type_Info *type;
        darray_foreach(type, from->types) {
            type->merge = merge;
            अगर (!Add_कुंजी_Type(into, type, false))
                into->error_गिनती++;
        }
        /* Types were either shallow copied or reinitialized individually
           in `AddKeyType`, so we only need to free the array */
        darray_free(from->types);
    }
}

अटल व्योम
Handle_कुंजी_Types_File(कुंजी_Types_Info *info, XkbFile *file);

अटल bool
Handle_Include_कुंजी_Types(कुंजी_Types_Info *info, IncludeStmt *include)
{
    कुंजी_Types_Info included;

    अगर (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_गिनती += 10;
        वापस false;
    }

    Init_कुंजी_Types_Info(&included, info->keymap_info, info->include_depth + 1,
                     &info->mods);
    included.name = steal(&include->stmt);

    स्थिर काष्ठा parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    क्रम (IncludeStmt *stmt = include; stmt; stmt = stmt->अग्र_incl) {
        कुंजी_Types_Info अग्र_incl;
        XkbFile *file;

        अक्षर path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_TYPES,
                                  path, माप(path));
        अगर (!file) {
            info->error_गिनती += 10;
            Clear_कुंजी_Types_Info(&included);
            वापस false;
        }

        Init_कुंजी_Types_Info(&अग्र_incl, info->keymap_info, info->include_depth + 1,
                         &included.mods);

        Handle_कुंजी_Types_File(&अग्र_incl, file);

        Merge_Included_कुंजी_Types(&included, &अग्र_incl, stmt->merge);

        Clear_कुंजी_Types_Info(&अग्र_incl);
        FreeXkbFile(file);
    }

    Merge_Included_कुंजी_Types(info, &included, include->merge);
    Clear_कुंजी_Types_Info(&included);

    वापस (info->error_गिनती == 0);
}

/***====================================================================***/

अटल bool
SetModifiers(कुंजी_Types_Info *info, कुंजी_Type_Info *type, व्यंजक_Def *arrayNdx,
             व्यंजक_Def *मूल्य)
{
    xkb_mod_mask_t mods = 0;

    अगर (arrayNdx) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The modifiers field of a key type is not an array; "
                "Illegal array subscript ignored\n");
        वापस false;
    }

    अगर (!व्यंजक_Resolve_Mod_Mask(info->ctx, मूल्य, MOD_BOTH, &info->mods, &mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Key type mask field must be a modifier mask; "
                "Key type definition ignored\n");
        वापस false;
    }

    अगर (type->defined & TYPE_FIELD_MASK) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple modifier mask definitions for key type %s; "
                 "Using %s, ignoring %s\n",
                 xkb_atom_text(info->ctx, type->name),
                 TypeMaskTxt(info, type),
                 ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods));
        वापस false;
    }

    type->mods = mods;
    वापस true;
}

/***====================================================================***/

अटल काष्ठा xkb_कुंजी_type_entry *
FindMatchingMapEntry(कुंजी_Type_Info *type, xkb_mod_mask_t mods)
{
    काष्ठा xkb_कुंजी_type_entry *entry;

    darray_foreach(entry, type->entries)
        अगर (entry->mods.mods == mods)
            वापस entry;

    वापस शून्य;
}

अटल bool
AddMapEntry(कुंजी_Types_Info *info, कुंजी_Type_Info *type,
            काष्ठा xkb_कुंजी_type_entry *new, bool clobber, bool report)
{
    काष्ठा xkb_कुंजी_type_entry *old;

    old = FindMatchingMapEntry(type, new->mods.mods);
    अगर (old) {
        अगर (report && old->level != new->level) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुंजी_TYPE_MAP_ENTRY,
                     "Multiple map entries for %s in %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     MapEntryTxt(info, new), TypeTxt(info, type),
                     (clobber ? new->level : old->level) + 1,
                     (clobber ? old->level : new->level) + 1);
        }
        अन्यथा {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_CONFLICTING_कुंजी_TYPE_MAP_ENTRY,
                    "Multiple occurrences of map[%s]= %"PRIu32" in %s; Ignored\n",
                    MapEntryTxt(info, new), new->level + 1,
                    TypeTxt(info, type));
            वापस true;
        }

        अगर (clobber) {
            अगर (new->level >= type->अंक_levels)
                type->अंक_levels = new->level + 1;
            old->level = new->level;
        }

        वापस true;
    }

    अगर (new->level >= type->अंक_levels)
        type->अंक_levels = new->level + 1;

    darray_append(type->entries, *new);
    वापस true;
}

अटल bool
SetMapEntry(कुंजी_Types_Info *info, कुंजी_Type_Info *type, व्यंजक_Def *arrayNdx,
            व्यंजक_Def *मूल्य)
{
    काष्ठा xkb_कुंजी_type_entry entry;

    अगर (arrayNdx == शून्य)
        वापस ReportTypeShouldBeArray(info, type, "map entry");

    अगर (!व्यंजक_Resolve_Mod_Mask(info->ctx, arrayNdx, MOD_BOTH, &info->mods,
                            &entry.mods.mods))
        वापस ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "map entry", "modifier mask");

    अगर (entry.mods.mods & (~type->mods)) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_IN_कुंजी_TYPE,
                "Map entry for modifiers not used by type %s; "
                "Using %s instead of %s\n",
                TypeTxt(info, type),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods,
                            entry.mods.mods & type->mods),
                MapEntryTxt(info, &entry));
        entry.mods.mods &= type->mods;
    }

    अगर (!व्यंजक_Resolve_Level(info->ctx, मूल्य, &entry.level)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                          "Level specifications in a key type must be integer; "
                          "Ignoring malformed level specification\n");
        वापस false;
    }

    entry.preserve.mods = 0;

    वापस AddMapEntry(info, type, &entry, true, true);
}

/***====================================================================***/

अटल bool
AddPreserve(कुंजी_Types_Info *info, कुंजी_Type_Info *type,
            xkb_mod_mask_t mods, xkb_mod_mask_t preserve_mods)
{
    काष्ठा xkb_कुंजी_type_entry *entry;

    darray_foreach(entry, type->entries) {
        अगर (entry->mods.mods != mods)
            जारी;

        /* Map exists without previous preserve (or "None"); override. */
        अगर (entry->preserve.mods == 0) {
            entry->preserve.mods = preserve_mods;
            वापस true;
        }

        /* Map exists with same preserve; do nothing. */
        अगर (entry->preserve.mods == preserve_mods) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_DUPLICATE_ENTRY,
                    "Identical definitions for preserve[%s] in %s; "
                    "Ignored\n",
                    ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                    TypeTxt(info, type));
            वापस true;
        }

        /* Map exists with different preserve; latter wins. */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_कुंजी_TYPE_PRESERVE_ENTRIES,
                "Multiple definitions for preserve[%s] in %s; "
                "Using %s, ignoring %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods,
                            entry->preserve.mods));

        entry->preserve.mods = preserve_mods;
        वापस true;
    }

    /*
     * Map does not exist, i.e. preserve[] came before map[].
     * Create a map with the specified mask mapping to Level1. The level
     * may be overridden later with an explicit map[] statement.
     */
    स्थिर काष्ठा xkb_कुंजी_type_entry new = {
        .level = 0,
        .mods = { .mods = mods },
        .preserve = { .mods = preserve_mods },
    };
    darray_append(type->entries, new);
    वापस true;
}

अटल bool
SetPreserve(कुंजी_Types_Info *info, कुंजी_Type_Info *type, व्यंजक_Def *arrayNdx,
            व्यंजक_Def *मूल्य)
{

    अगर (arrayNdx == शून्य)
        वापस ReportTypeShouldBeArray(info, type, "preserve entry");

    xkb_mod_mask_t mods = 0;
    अगर (!व्यंजक_Resolve_Mod_Mask(info->ctx, arrayNdx, MOD_BOTH, &info->mods, &mods))
        वापस ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "preserve entry", "modifier mask");

    अगर (mods & ~type->mods) {
        स्थिर अक्षर *before, *after;

        before = ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods);
        mods &= type->mods;
        after = ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_IN_कुंजी_TYPE,
                "Preserve entry for modifiers not used by the %s type; "
                "Index %s converted to %s\n",
                TypeTxt(info, type), before, after);
    }

    xkb_mod_mask_t preserve_mods = 0;
    अगर (!व्यंजक_Resolve_Mod_Mask(info->ctx, मूल्य, MOD_BOTH, &info->mods,
                            &preserve_mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Preserve value in a key type is not a modifier mask; "
                "Ignoring preserve[%s] in type %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type));
        वापस false;
    }

    अगर (preserve_mods & ~mods) {
        स्थिर अक्षर *before, *after;

        before = ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods);
        preserve_mods &= mods;
        after = ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_ILLEGAL_कुंजी_TYPE_PRESERVE_परिणाम,
                "Illegal value for preserve[%s] in type %s; "
                "Converted %s to %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type), before, after);
    }

    वापस AddPreserve(info, type, mods, preserve_mods);
}

/***====================================================================***/

अटल bool
AddLevelName(कुंजी_Types_Info *info, कुंजी_Type_Info *type,
             xkb_level_सूचकांक_t level, xkb_atom_t name, bool clobber)
{
    /* New name. */
    अगर (level >= darray_आकार(type->level_names)) {
        darray_resize0(type->level_names, level + 1);
        जाओ finish;
    }

    /* Same level, same name. */
    अगर (darray_item(type->level_names, level) == name) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE, XKB_WARNING_DUPLICATE_ENTRY,
                "Duplicate names for level %"PRIu32" of key type %s; Ignored\n",
                level + 1, TypeTxt(info, type));
        वापस true;
    }

    /* Same level, different name. */
    अगर (darray_item(type->level_names, level) != XKB_ATOM_NONE) {
        स्थिर अक्षर *old, *new;
        old = xkb_atom_text(info->ctx,
                            darray_item(type->level_names, level));
        new = xkb_atom_text(info->ctx, name);
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_कुंजी_TYPE_LEVEL_NAMES,
                "Multiple names for level %"PRIu32" of key type %s; "
                "Using %s, ignoring %s\n",
                level + 1, TypeTxt(info, type),
                (clobber ? new : old), (clobber ? old : new));

        अगर (!clobber)
            वापस true;
    }

    /* FIXME: What about different level, same name? */

finish:
    darray_item(type->level_names, level) = name;
    वापस true;
}

अटल bool
SetLevelName(कुंजी_Types_Info *info, कुंजी_Type_Info *type, व्यंजक_Def *arrayNdx,
             व्यंजक_Def *मूल्य)
{
    अगर (arrayNdx == शून्य)
        वापस ReportTypeShouldBeArray(info, type, "level name");

    xkb_level_सूचकांक_t level = 0;
    अगर (!व्यंजक_Resolve_Level(info->ctx, arrayNdx, &level))
        वापस ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                                 type, "level name", "integer");

    xkb_atom_t level_name = XKB_ATOM_NONE;
    अगर (!व्यंजक_Resolve_String(info->ctx, मूल्य, &level_name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Non-string name for level %"PRIu32" in key type %s; "
                "Ignoring illegal level name definition\n",
                level + 1, xkb_atom_text(info->ctx, type->name));
        वापस false;
    }

    वापस AddLevelName(info, type, level, level_name, true);
}

/***====================================================================***/

अटल bool
Set_कुंजी_Type_Field(कुंजी_Types_Info *info, कुंजी_Type_Info *type,
                स्थिर अक्षर *field, व्यंजक_Def *arrayNdx, व्यंजक_Def *मूल्य)
{
    bool ok = false;
    क्रमागत type_field type_field = 0;

    अगर (istreq(field, "modifiers")) {
        type_field = TYPE_FIELD_MASK;
        ok = SetModifiers(info, type, arrayNdx, मूल्य);
    }
    अन्यथा अगर (istreq(field, "map")) {
        type_field = TYPE_FIELD_MAP;
        ok = SetMapEntry(info, type, arrayNdx, मूल्य);
    }
    अन्यथा अगर (istreq(field, "preserve")) {
        type_field = TYPE_FIELD_PRESERVE;
        ok = SetPreserve(info, type, arrayNdx, मूल्य);
    }
    अन्यथा अगर (istreq(field, "levelname") || istreq(field, "level_name")) {
        type_field = TYPE_FIELD_LEVEL_NAME;
        ok = SetLevelName(info, type, arrayNdx, मूल्य);
    } अन्यथा {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in key type \"%s\"; Definition ignored\n",
                field, TypeTxt(info, type));
        ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_TYPE_FIELDS);
    }

    type->defined |= type_field;
    वापस ok;
}

अटल bool
Handle_कुंजी_Type_Body(कुंजी_Types_Info *info, VarDef *def, कुंजी_Type_Info *type)
{
    bool ok = true;
    स्थिर अक्षर *elem, *field;
    व्यंजक_Def *arrayNdx;

    क्रम (; def; def = (VarDef *) def->common.अग्र) {
        अगर (!व्यंजक_Resolve_Lhs(info->ctx, def->name, &elem, &field,
                            &arrayNdx)) {
            /* internal error, already reported */
            ok = false;
            जारी;
        }

        अगर (elem) {
            अगर (istreq(elem, "type")) {
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
            जारी;
        }

        अगर (!Set_कुंजी_Type_Field(info, type, field, arrayNdx, def->मूल्य))
            ok = false;
    }

    वापस ok;
}

अटल bool
Handle_कुंजी_Type_Def(कुंजी_Types_Info *info, कुंजी_Type_Def *def)
{
    कुंजी_Type_Info type = {
        .defined = 0,
        .merge = def->merge,
        .name = def->name,
        .mods = 0,
        .अंक_levels = 1,
        .entries = darray_new(),
        .level_names = darray_new(),
    };

    अगर (!Handle_कुंजी_Type_Body(info, def->body, &type) ||
        !Add_कुंजी_Type(info, &type, true))
    {
        info->error_गिनती++;
        Clear_कुंजी_Type_Info(&type);
        वापस false;
    }

    /* Type has been either stolen via shallow copy or reinitialized in
       `AddKeyType`: no need to free the arrays */
    वापस true;
}

अटल bool
HandleGlobalVar(कुंजी_Types_Info *info, VarDef *stmt)
{
    स्थिर अक्षर *elem, *field;
    व्यंजक_Def *arrayNdx;

    अगर (!व्यंजक_Resolve_Lhs(info->ctx, stmt->name, &elem, &field,
                        &arrayNdx)) {
        वापस false;           /* internal error, already reported */
    }
    अन्यथा अगर (elem && istreq(elem, "type")) {
        log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                "Support for changing the default type has been removed; "
                "Statement ignored\n");
        वापस true;
    }
    अन्यथा अगर (elem) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown element \"%s\"; "
                "Value for field \"%s.%s\" ignored\n", elem, elem, field);
        वापस !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
    }
    अन्यथा अगर (field) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        वापस !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS);
    }
    वापस false;
}

अटल व्योम
Handle_कुंजी_Types_File(कुंजी_Types_Info *info, XkbFile *file)
{
    bool ok;

    मुक्त(info->name);
    info->name = strdup_safe(file->name);

    क्रम (ParseCommon *stmt = file->defs; stmt; stmt = stmt->अग्र) {
        चयन (stmt->type) {
        हाल STMT_INCLUDE:
            ok = Handle_Include_कुंजी_Types(info, (IncludeStmt *) stmt);
            अवरोध;
        हाल STMT_TYPE:
            ok = Handle_कुंजी_Type_Def(info, (कुंजी_Type_Def *) stmt);
            अवरोध;
        हाल STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef*) stmt);
            अवरोध;
        हाल STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            अवरोध;
        हाल STMT_UNKNOWN_DECLARATION:
        हाल STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported types %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            अवरोध;
        शेष:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Key type files may not include other declarations; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            अवरोध;
        }

        अगर (!ok)
            info->error_गिनती++;

        अगर (info->error_गिनती > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning keytypes file \"%s\"\n",
                    safe_map_name(file->name));
            अवरोध;
        }
    }
}

/***====================================================================***/

अटल bool
Copy_कुंजी_Types_To_Keymap(काष्ठा xkb_keymap *keymap, कुंजी_Types_Info *info)
{
    स्थिर darray_आकार_t अंक_types = darray_empty(info->types)
                                  ? 1
                                  : darray_आकार(info->types);
    काष्ठा xkb_कुंजी_type *types = सुस्मृति(अंक_types, माप(*types));
    अगर (!types)
        वापस false;

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
    अगर (darray_empty(info->types)) {
        काष्ठा xkb_कुंजी_type *type = &types[0];

        type->mods.mods = 0;
        type->अंक_levels = 1;
        type->entries = शून्य;
        type->अंक_entries = 0;
        type->name = xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL");
        type->level_names = शून्य;
        type->अंक_level_names = 0;
        type->required = true;
    }
    अन्यथा {
        /* HACK to circumvent Xorg bugs (see comment above) */
        स्थिर xkb_atom_t canonical_types[] = {
            xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "TWO_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "ALPHABETIC"),
            xkb_atom_intern_literal(keymap->ctx, "KEYPAD"),
        };

        क्रम (darray_आकार_t i = 0; i < अंक_types; i++) {
            कुंजी_Type_Info *def = &darray_item(info->types, i);
            काष्ठा xkb_कुंजी_type *type = &types[i];

            type->name = def->name;
            type->mods.mods = def->mods;
            type->अंक_levels = def->अंक_levels;
            type->अंक_level_names =
                (xkb_level_सूचकांक_t) darray_आकार(def->level_names);
            darray_steal(def->level_names, &type->level_names, शून्य);
            darray_steal(def->entries, &type->entries, &type->अंक_entries);
            type->required = false;

            /* HACK: Never drop canonical XKB key types (see comment above) */
            अगर (type->अंक_levels <= 2) {
                क्रम (uint8_t t = 0;
                     t < (uint8_t) ARRAY_आकार(canonical_types);
                     t++) {
                        अगर (type->name == canonical_types[t]) {
                            type->required = true;
                            अवरोध;
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
    वापस true;
}

/***====================================================================***/

bool
Compile_कुंजी_Types(XkbFile *file, काष्ठा xkb_keymap_info *keymap_info)
{
    कुंजी_Types_Info info;

    Init_कुंजी_Types_Info(&info, keymap_info, 0,
                     &keymap_info->keymap.mods);

    अगर (file != शून्य)
        Handle_कुंजी_Types_File(&info, file);

    अगर (info.error_गिनती != 0)
        जाओ err_info;

    अगर (!Copy_कुंजी_Types_To_Keymap(&keymap_info->keymap, &info))
        जाओ err_info;

    Clear_कुंजी_Types_Info(&info);
    वापस true;

err_info:
    Clear_कुंजी_Types_Info(&info);
    वापस false;
}
