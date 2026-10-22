/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <limits.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "xkbcommon/xkbcommon-status.h"
#અન્તર્ભાવ "xkbcomp-priv.h"
#અન્તર્ભાવ "text.h"
#અન્તર્ભાવ "vmod.h"
#અન્તર્ભાવ "expr.h"
#અન્તર્ભાવ "include.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "util-mem.h"

ગણના type_field {
    TYPE_FIELD_MASK = (1 << 0),
    TYPE_FIELD_MAP = (1 << 1),
    TYPE_FIELD_PRESERVE = (1 << 2),
    TYPE_FIELD_LEVEL_NAME = (1 << 3),
};

પ્રકારવ્યાખ્યા સંરચના {
    ગણના type_field defined;
    ગણના merge_mode merge;

    xkb_atom_t name;
    xkb_mod_mask_t mods;
    xkb_level_index_t અંક_levels;
    darray(સંરચના xkb_ચાવી_type_entry) entries;
    darray(xkb_atom_t) level_names;
} ચાવી_Type_Info;

પ્રકારવ્યાખ્યા સંરચના {
    અક્ષર *name;
    પૂર્ણાંક error_ગણતરી;
    unsigned પૂર્ણાંક include_depth;

    darray(ચાવી_Type_Info) types;
    સંરચના xkb_mod_set mods;

    સંરચના xkb_context *ctx;
    અચલ સંરચના xkb_keymap_info *keymap_info;
} ચાવી_Types_Info;

/***====================================================================***/

સ્થિર અંતઃરેખ અચલ અક્ષર *
MapEntryTxt(ચાવી_Types_Info *info, સંરચના xkb_ચાવી_type_entry *entry)
{
    પાછા ModMaskText(info->ctx, MOD_BOTH, &info->mods, entry->mods.mods);
}

સ્થિર અંતઃરેખ અચલ અક્ષર *
TypeTxt(ચાવી_Types_Info *info, ચાવી_Type_Info *type)
{
    પાછા xkb_atom_text(info->ctx, type->name);
}

સ્થિર અંતઃરેખ અચલ અક્ષર *
TypeMaskTxt(ચાવી_Types_Info *info, ચાવી_Type_Info *type)
{
    પાછા ModMaskText(info->ctx, MOD_BOTH, &info->mods, type->mods);
}

સ્થિર અંતઃરેખ bool
ReportTypeShouldBeArray(ચાવી_Types_Info *info, ચાવી_Type_Info *type,
                        અચલ અક્ષર *field)
{
    પાછા ReportShouldBeArray(info->ctx, "key type", field,
                               TypeTxt(info, type));
}

સ્થિર અંતઃરેખ bool
ReportTypeBadType(ચાવી_Types_Info *info, ગણના xkb_message_code code,
                  ચાવી_Type_Info *type, અચલ અક્ષર *field, અચલ અક્ષર *wanted)
{
    પાછા ReportBadType(info->ctx, code, "key type", field,
                         TypeTxt(info, type), wanted);
}

/***====================================================================***/

સ્થિર શૂન્ય
Init_ચાવી_Types_Info(ચાવી_Types_Info *info, અચલ સંરચના xkb_keymap_info *keymap_info,
                 unsigned પૂર્ણાંક include_depth,
                 અચલ સંરચના xkb_mod_set *mods)
{
    memset(info, 0, કદ(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    InitVMods(&info->mods, mods, include_depth > 0);
}

સ્થિર શૂન્ય
Clear_ચાવી_Type_Info(ચાવી_Type_Info *type)
{
    darray_free(type->entries);
    darray_free(type->level_names);
}

સ્થિર શૂન્ય
Clear_ચાવી_Types_Info(ચાવી_Types_Info *info)
{
    મુક્ત(info->name);
    ચાવી_Type_Info *type;
    darray_foreach(type, info->types)
        Clear_ચાવી_Type_Info(type);
    darray_free(info->types);
}

સ્થિર ચાવી_Type_Info *
Find_Matching_ચાવી_Type(ચાવી_Types_Info *info, xkb_atom_t name)
{
    ચાવી_Type_Info *old;

    darray_foreach(old, info->types)
        જો (old->name == name)
            પાછા old;

    પાછા ખાલી;
}

સ્થિર bool
Add_ચાવી_Type(ચાવી_Types_Info *info, ચાવી_Type_Info *new, bool same_file)
{
    ચાવી_Type_Info *old;
    અચલ પૂર્ણાંક verbosity = xkb_context_get_log_verbosity(info->ctx);

    old = Find_Matching_ચાવી_Type(info, new->name);
    જો (old) {
        જો (new->merge != MERGE_AUGMENT) {
            જો ((same_file && verbosity > 0) || verbosity > 9) {
                log_warn(info->ctx,
                         XKB_WARNING_CONFLICTING_ચાવી_TYPE_DEFINITIONS,
                         "Multiple definitions of the %s key type; "
                         "Earlier definition ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            }

            Clear_ચાવી_Type_Info(old);
            *old = *new;
            darray_init(new->entries);
            darray_init(new->level_names);
            પાછા true;
        }

        જો (same_file)
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_CONFLICTING_ચાવી_TYPE_DEFINITIONS,
                    "Multiple definitions of the %s key type; "
                    "Later definition ignored\n",
                    xkb_atom_text(info->ctx, new->name));

        Clear_ચાવી_Type_Info(new);
        પાછા true;
    }

    darray_append(info->types, *new);
    પાછા true;
}

/***====================================================================***/

સ્થિર શૂન્ય
Merge_Included_ચાવી_Types(ચાવી_Types_Info *into, ચાવી_Types_Info *from,
                      ગણના merge_mode merge)
{
    જો (from->error_ગણતરી > 0) {
        into->error_ગણતરી += from->error_ગણતરી;
        પાછા;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    જો (into->name == ખાલી) {
        into->name = steal(&from->name);
    }

    જો (darray_empty(into->types)) {
        into->types = from->types;
        /* Types stolen via shallow copy, so reinitialize the array */
        darray_init(from->types);
    }
    નહીંતર {
        ચાવી_Type_Info *type;
        darray_foreach(type, from->types) {
            type->merge = merge;
            જો (!Add_ચાવી_Type(into, type, false))
                into->error_ગણતરી++;
        }
        /* Types were either shallow copied or reinitialized individually
           in `AddKeyType`, so we only need to free the array */
        darray_free(from->types);
    }
}

સ્થિર શૂન્ય
Handle_ચાવી_Types_File(ચાવી_Types_Info *info, XkbFile *file);

સ્થિર bool
Handle_Include_ચાવી_Types(ચાવી_Types_Info *info, IncludeStmt *include)
{
    ચાવી_Types_Info included;

    જો (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_ગણતરી += 10;
        પાછા false;
    }

    Init_ચાવી_Types_Info(&included, info->keymap_info, info->include_depth + 1,
                     &info->mods);
    included.name = steal(&include->stmt);

    અચલ સંરચના parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    માટે (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        ચાવી_Types_Info next_incl;
        XkbFile *file;

        અક્ષર path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_TYPES,
                                  path, કદ(path));
        જો (!file) {
            info->error_ગણતરી += 10;
            Clear_ચાવી_Types_Info(&included);
            પાછા false;
        }

        Init_ચાવી_Types_Info(&next_incl, info->keymap_info, info->include_depth + 1,
                         &included.mods);

        Handle_ચાવી_Types_File(&next_incl, file);

        Merge_Included_ચાવી_Types(&included, &next_incl, stmt->merge);

        Clear_ચાવી_Types_Info(&next_incl);
        FreeXkbFile(file);
    }

    Merge_Included_ચાવી_Types(info, &included, include->merge);
    Clear_ચાવી_Types_Info(&included);

    પાછા (info->error_ગણતરી == 0);
}

/***====================================================================***/

સ્થિર bool
SetModifiers(ચાવી_Types_Info *info, ચાવી_Type_Info *type, પદાવલિ_Def *arrayNdx,
             પદાવલિ_Def *કિંમત)
{
    xkb_mod_mask_t mods = 0;

    જો (arrayNdx) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The modifiers field of a key type is not an array; "
                "Illegal array subscript ignored\n");
        પાછા false;
    }

    જો (!પદાવલિ_Resolve_Mod_Mask(info->ctx, કિંમત, MOD_BOTH, &info->mods, &mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Key type mask field must be a modifier mask; "
                "Key type definition ignored\n");
        પાછા false;
    }

    જો (type->defined & TYPE_FIELD_MASK) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple modifier mask definitions for key type %s; "
                 "Using %s, ignoring %s\n",
                 xkb_atom_text(info->ctx, type->name),
                 TypeMaskTxt(info, type),
                 ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods));
        પાછા false;
    }

    type->mods = mods;
    પાછા true;
}

/***====================================================================***/

સ્થિર સંરચના xkb_ચાવી_type_entry *
FindMatchingMapEntry(ચાવી_Type_Info *type, xkb_mod_mask_t mods)
{
    સંરચના xkb_ચાવી_type_entry *entry;

    darray_foreach(entry, type->entries)
        જો (entry->mods.mods == mods)
            પાછા entry;

    પાછા ખાલી;
}

સ્થિર bool
AddMapEntry(ચાવી_Types_Info *info, ચાવી_Type_Info *type,
            સંરચના xkb_ચાવી_type_entry *new, bool clobber, bool report)
{
    સંરચના xkb_ચાવી_type_entry *old;

    old = FindMatchingMapEntry(type, new->mods.mods);
    જો (old) {
        જો (report && old->level != new->level) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_ચાવી_TYPE_MAP_ENTRY,
                     "Multiple map entries for %s in %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     MapEntryTxt(info, new), TypeTxt(info, type),
                     (clobber ? new->level : old->level) + 1,
                     (clobber ? old->level : new->level) + 1);
        }
        નહીંતર {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_CONFLICTING_ચાવી_TYPE_MAP_ENTRY,
                    "Multiple occurrences of map[%s]= %"PRIu32" in %s; Ignored\n",
                    MapEntryTxt(info, new), new->level + 1,
                    TypeTxt(info, type));
            પાછા true;
        }

        જો (clobber) {
            જો (new->level >= type->અંક_levels)
                type->અંક_levels = new->level + 1;
            old->level = new->level;
        }

        પાછા true;
    }

    જો (new->level >= type->અંક_levels)
        type->અંક_levels = new->level + 1;

    darray_append(type->entries, *new);
    પાછા true;
}

સ્થિર bool
SetMapEntry(ચાવી_Types_Info *info, ચાવી_Type_Info *type, પદાવલિ_Def *arrayNdx,
            પદાવલિ_Def *કિંમત)
{
    સંરચના xkb_ચાવી_type_entry entry;

    જો (arrayNdx == ખાલી)
        પાછા ReportTypeShouldBeArray(info, type, "map entry");

    જો (!પદાવલિ_Resolve_Mod_Mask(info->ctx, arrayNdx, MOD_BOTH, &info->mods,
                            &entry.mods.mods))
        પાછા ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "map entry", "modifier mask");

    જો (entry.mods.mods & (~type->mods)) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_IN_ચાવી_TYPE,
                "Map entry for modifiers not used by type %s; "
                "Using %s instead of %s\n",
                TypeTxt(info, type),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods,
                            entry.mods.mods & type->mods),
                MapEntryTxt(info, &entry));
        entry.mods.mods &= type->mods;
    }

    જો (!પદાવલિ_Resolve_Level(info->ctx, કિંમત, &entry.level)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                          "Level specifications in a key type must be integer; "
                          "Ignoring malformed level specification\n");
        પાછા false;
    }

    entry.preserve.mods = 0;

    પાછા AddMapEntry(info, type, &entry, true, true);
}

/***====================================================================***/

સ્થિર bool
AddPreserve(ચાવી_Types_Info *info, ચાવી_Type_Info *type,
            xkb_mod_mask_t mods, xkb_mod_mask_t preserve_mods)
{
    સંરચના xkb_ચાવી_type_entry *entry;

    darray_foreach(entry, type->entries) {
        જો (entry->mods.mods != mods)
            ચાલુ;

        /* Map exists without previous preserve (or "None"); override. */
        જો (entry->preserve.mods == 0) {
            entry->preserve.mods = preserve_mods;
            પાછા true;
        }

        /* Map exists with same preserve; do nothing. */
        જો (entry->preserve.mods == preserve_mods) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_DUPLICATE_ENTRY,
                    "Identical definitions for preserve[%s] in %s; "
                    "Ignored\n",
                    ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                    TypeTxt(info, type));
            પાછા true;
        }

        /* Map exists with different preserve; latter wins. */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_ચાવી_TYPE_PRESERVE_ENTRIES,
                "Multiple definitions for preserve[%s] in %s; "
                "Using %s, ignoring %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods,
                            entry->preserve.mods));

        entry->preserve.mods = preserve_mods;
        પાછા true;
    }

    /*
     * Map does not exist, i.e. preserve[] came before map[].
     * Create a map with the specified mask mapping to Level1. The level
     * may be overridden later with an explicit map[] statement.
     */
    અચલ સંરચના xkb_ચાવી_type_entry new = {
        .level = 0,
        .mods = { .mods = mods },
        .preserve = { .mods = preserve_mods },
    };
    darray_append(type->entries, new);
    પાછા true;
}

સ્થિર bool
SetPreserve(ચાવી_Types_Info *info, ચાવી_Type_Info *type, પદાવલિ_Def *arrayNdx,
            પદાવલિ_Def *કિંમત)
{

    જો (arrayNdx == ખાલી)
        પાછા ReportTypeShouldBeArray(info, type, "preserve entry");

    xkb_mod_mask_t mods = 0;
    જો (!પદાવલિ_Resolve_Mod_Mask(info->ctx, arrayNdx, MOD_BOTH, &info->mods, &mods))
        પાછા ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "preserve entry", "modifier mask");

    જો (mods & ~type->mods) {
        અચલ અક્ષર *before, *after;

        before = ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods);
        mods &= type->mods;
        after = ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_IN_ચાવી_TYPE,
                "Preserve entry for modifiers not used by the %s type; "
                "Index %s converted to %s\n",
                TypeTxt(info, type), before, after);
    }

    xkb_mod_mask_t preserve_mods = 0;
    જો (!પદાવલિ_Resolve_Mod_Mask(info->ctx, કિંમત, MOD_BOTH, &info->mods,
                            &preserve_mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Preserve value in a key type is not a modifier mask; "
                "Ignoring preserve[%s] in type %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type));
        પાછા false;
    }

    જો (preserve_mods & ~mods) {
        અચલ અક્ષર *before, *after;

        before = ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods);
        preserve_mods &= mods;
        after = ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_ILLEGAL_ચાવી_TYPE_PRESERVE_RESULT,
                "Illegal value for preserve[%s] in type %s; "
                "Converted %s to %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type), before, after);
    }

    પાછા AddPreserve(info, type, mods, preserve_mods);
}

/***====================================================================***/

સ્થિર bool
AddLevelName(ચાવી_Types_Info *info, ચાવી_Type_Info *type,
             xkb_level_index_t level, xkb_atom_t name, bool clobber)
{
    /* New name. */
    જો (level >= darray_size(type->level_names)) {
        darray_resize0(type->level_names, level + 1);
        જાઓ finish;
    }

    /* Same level, same name. */
    જો (darray_item(type->level_names, level) == name) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE, XKB_WARNING_DUPLICATE_ENTRY,
                "Duplicate names for level %"PRIu32" of key type %s; Ignored\n",
                level + 1, TypeTxt(info, type));
        પાછા true;
    }

    /* Same level, different name. */
    જો (darray_item(type->level_names, level) != XKB_ATOM_NONE) {
        અચલ અક્ષર *old, *new;
        old = xkb_atom_text(info->ctx,
                            darray_item(type->level_names, level));
        new = xkb_atom_text(info->ctx, name);
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_ચાવી_TYPE_LEVEL_NAMES,
                "Multiple names for level %"PRIu32" of key type %s; "
                "Using %s, ignoring %s\n",
                level + 1, TypeTxt(info, type),
                (clobber ? new : old), (clobber ? old : new));

        જો (!clobber)
            પાછા true;
    }

    /* FIXME: What about different level, same name? */

finish:
    darray_item(type->level_names, level) = name;
    પાછા true;
}

સ્થિર bool
SetLevelName(ચાવી_Types_Info *info, ચાવી_Type_Info *type, પદાવલિ_Def *arrayNdx,
             પદાવલિ_Def *કિંમત)
{
    જો (arrayNdx == ખાલી)
        પાછા ReportTypeShouldBeArray(info, type, "level name");

    xkb_level_index_t level = 0;
    જો (!પદાવલિ_Resolve_Level(info->ctx, arrayNdx, &level))
        પાછા ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                                 type, "level name", "integer");

    xkb_atom_t level_name = XKB_ATOM_NONE;
    જો (!પદાવલિ_Resolve_String(info->ctx, કિંમત, &level_name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Non-string name for level %"PRIu32" in key type %s; "
                "Ignoring illegal level name definition\n",
                level + 1, xkb_atom_text(info->ctx, type->name));
        પાછા false;
    }

    પાછા AddLevelName(info, type, level, level_name, true);
}

/***====================================================================***/

સ્થિર bool
Set_ચાવી_Type_Field(ચાવી_Types_Info *info, ચાવી_Type_Info *type,
                અચલ અક્ષર *field, પદાવલિ_Def *arrayNdx, પદાવલિ_Def *કિંમત)
{
    bool ok = false;
    ગણના type_field type_field = 0;

    જો (istreq(field, "modifiers")) {
        type_field = TYPE_FIELD_MASK;
        ok = SetModifiers(info, type, arrayNdx, કિંમત);
    }
    નહીંતર જો (istreq(field, "map")) {
        type_field = TYPE_FIELD_MAP;
        ok = SetMapEntry(info, type, arrayNdx, કિંમત);
    }
    નહીંતર જો (istreq(field, "preserve")) {
        type_field = TYPE_FIELD_PRESERVE;
        ok = SetPreserve(info, type, arrayNdx, કિંમત);
    }
    નહીંતર જો (istreq(field, "levelname") || istreq(field, "level_name")) {
        type_field = TYPE_FIELD_LEVEL_NAME;
        ok = SetLevelName(info, type, arrayNdx, કિંમત);
    } નહીંતર {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in key type \"%s\"; Definition ignored\n",
                field, TypeTxt(info, type));
        ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_TYPE_FIELDS);
    }

    type->defined |= type_field;
    પાછા ok;
}

સ્થિર bool
Handle_ચાવી_Type_Body(ચાવી_Types_Info *info, VarDef *def, ચાવી_Type_Info *type)
{
    bool ok = true;
    અચલ અક્ષર *elem, *field;
    પદાવલિ_Def *arrayNdx;

    માટે (; def; def = (VarDef *) def->common.next) {
        જો (!પદાવલિ_Resolve_Lhs(info->ctx, def->name, &elem, &field,
                            &arrayNdx)) {
            /* internal error, already reported */
            ok = false;
            ચાલુ;
        }

        જો (elem) {
            જો (istreq(elem, "type")) {
                log_err(info->ctx, XKB_ERROR_INVALID_SET_DEFAULT_STATEMENT,
                        "Support for changing the default type has been removed; "
                        "Statement \"%s.%s\" ignored.\n", elem, field);
            }
            નહીંતર {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key type statement: move statements to the global "
                        "file scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
            ચાલુ;
        }

        જો (!Set_ચાવી_Type_Field(info, type, field, arrayNdx, def->કિંમત))
            ok = false;
    }

    પાછા ok;
}

સ્થિર bool
Handle_ચાવી_Type_Def(ચાવી_Types_Info *info, ચાવી_Type_Def *def)
{
    ચાવી_Type_Info type = {
        .defined = 0,
        .merge = def->merge,
        .name = def->name,
        .mods = 0,
        .અંક_levels = 1,
        .entries = darray_new(),
        .level_names = darray_new(),
    };

    જો (!Handle_ચાવી_Type_Body(info, def->body, &type) ||
        !Add_ચાવી_Type(info, &type, true))
    {
        info->error_ગણતરી++;
        Clear_ચાવી_Type_Info(&type);
        પાછા false;
    }

    /* Type has been either stolen via shallow copy or reinitialized in
       `AddKeyType`: no need to free the arrays */
    પાછા true;
}

સ્થિર bool
HandleGlobalVar(ચાવી_Types_Info *info, VarDef *stmt)
{
    અચલ અક્ષર *elem, *field;
    પદાવલિ_Def *arrayNdx;

    જો (!પદાવલિ_Resolve_Lhs(info->ctx, stmt->name, &elem, &field,
                        &arrayNdx)) {
        પાછા false;           /* internal error, already reported */
    }
    નહીંતર જો (elem && istreq(elem, "type")) {
        log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                "Support for changing the default type has been removed; "
                "Statement ignored\n");
        પાછા true;
    }
    નહીંતર જો (elem) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown element \"%s\"; "
                "Value for field \"%s.%s\" ignored\n", elem, elem, field);
        પાછા !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
    }
    નહીંતર જો (field) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        પાછા !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS);
    }
    પાછા false;
}

સ્થિર શૂન્ય
Handle_ચાવી_Types_File(ચાવી_Types_Info *info, XkbFile *file)
{
    bool ok;

    મુક્ત(info->name);
    info->name = strdup_safe(file->name);

    માટે (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        પસંદગી (stmt->type) {
        સ્થિતિ STMT_INCLUDE:
            ok = Handle_Include_ચાવી_Types(info, (IncludeStmt *) stmt);
            તોડો;
        સ્થિતિ STMT_TYPE:
            ok = Handle_ચાવી_Type_Def(info, (ચાવી_Type_Def *) stmt);
            તોડો;
        સ્થિતિ STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef*) stmt);
            તોડો;
        સ્થિતિ STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            તોડો;
        સ્થિતિ STMT_UNKNOWN_DECLARATION:
        સ્થિતિ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported types %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            તોડો;
        મૂળભૂત:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Key type files may not include other declarations; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            તોડો;
        }

        જો (!ok)
            info->error_ગણતરી++;

        જો (info->error_ગણતરી > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning keytypes file \"%s\"\n",
                    safe_map_name(file->name));
            તોડો;
        }
    }
}

/***====================================================================***/

સ્થિર bool
Copy_ચાવી_Types_To_Keymap(સંરચના xkb_keymap *keymap, ચાવી_Types_Info *info)
{
    અચલ darray_size_t અંક_types = darray_empty(info->types)
                                  ? 1
                                  : darray_size(info->types);
    સંરચના xkb_ચાવી_type *types = calloc(અંક_types, કદ(*types));
    જો (!types)
        પાછા false;

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
    જો (darray_empty(info->types)) {
        સંરચના xkb_ચાવી_type *type = &types[0];

        type->mods.mods = 0;
        type->અંક_levels = 1;
        type->entries = ખાલી;
        type->અંક_entries = 0;
        type->name = xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL");
        type->level_names = ખાલી;
        type->અંક_level_names = 0;
        type->required = true;
    }
    નહીંતર {
        /* HACK to circumvent Xorg bugs (see comment above) */
        અચલ xkb_atom_t canonical_types[] = {
            xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "TWO_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "ALPHABETIC"),
            xkb_atom_intern_literal(keymap->ctx, "KEYPAD"),
        };

        માટે (darray_size_t i = 0; i < અંક_types; i++) {
            ચાવી_Type_Info *def = &darray_item(info->types, i);
            સંરચના xkb_ચાવી_type *type = &types[i];

            type->name = def->name;
            type->mods.mods = def->mods;
            type->અંક_levels = def->અંક_levels;
            type->અંક_level_names =
                (xkb_level_index_t) darray_size(def->level_names);
            darray_steal(def->level_names, &type->level_names, ખાલી);
            darray_steal(def->entries, &type->entries, &type->અંક_entries);
            type->required = false;

            /* HACK: Never drop canonical XKB key types (see comment above) */
            જો (type->અંક_levels <= 2) {
                માટે (uint8_t t = 0;
                     t < (uint8_t) ARRAY_SIZE(canonical_types);
                     t++) {
                        જો (type->name == canonical_types[t]) {
                            type->required = true;
                            તોડો;
                        }
                }
            }
        }
    }

    keymap->types_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->types_section_name);
    keymap->અંક_types = અંક_types;
    keymap->types = types;
    keymap->mods = info->mods;
    પાછા true;
}

/***====================================================================***/

bool
Compile_ચાવી_Types(XkbFile *file, સંરચના xkb_keymap_info *keymap_info)
{
    ચાવી_Types_Info info;

    Init_ચાવી_Types_Info(&info, keymap_info, 0,
                     &keymap_info->keymap.mods);

    જો (file != ખાલી)
        Handle_ચાવી_Types_File(&info, file);

    જો (info.error_ગણતરી != 0)
        જાઓ err_info;

    જો (!Copy_ચાવી_Types_To_Keymap(&keymap_info->keymap, &info))
        જાઓ err_info;

    Clear_ચાવી_Types_Info(&info);
    પાછા true;

err_info:
    Clear_ચાવી_Types_Info(&info);
    પાછા false;
}
