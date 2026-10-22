/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <limits.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-status.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"
#ಅನ್ತರ್ಭಾವ "text.h"
#ಅನ್ತರ್ಭಾವ "vmod.h"
#ಅನ್ತರ್ಭಾವ "expr.h"
#ಅನ್ತರ್ಭಾವ "include.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "util-mem.h"

ಗಣನಾ type_field {
    TYPE_FIELD_MASK = (1 << 0),
    TYPE_FIELD_MAP = (1 << 1),
    TYPE_FIELD_PRESERVE = (1 << 2),
    TYPE_FIELD_LEVEL_NAME = (1 << 3),
};

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ಗಣನಾ type_field defined;
    ಗಣನಾ merge_mode merge;

    xkb_atom_t name;
    xkb_mod_mask_t mods;
    xkb_level_index_t ಅಂಕೆ_levels;
    darray(ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry) entries;
    darray(xkb_atom_t) level_names;
} ಕೀಲಿ_Type_Info;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ವರ್ಣ *name;
    ಪೂರ್ಣಾಙ್ಕ error_ಎಣಿಕೆ;
    unsigned ಪೂರ್ಣಾಙ್ಕ include_depth;

    darray(ಕೀಲಿ_Type_Info) types;
    ಸಂರಚನಾ xkb_mod_set mods;

    ಸಂರಚನಾ xkb_context *ctx;
    ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info;
} ಕೀಲಿ_Types_Info;

/***====================================================================***/

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ ವರ್ಣ *
MapEntryTxt(ಕೀಲಿ_Types_Info *info, ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry *entry)
{
    ಪ್ರತ್ಯಾಗಮ ModMaskText(info->ctx, MOD_BOTH, &info->mods, entry->mods.mods);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ ವರ್ಣ *
TypeTxt(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *type)
{
    ಪ್ರತ್ಯಾಗಮ xkb_atom_text(info->ctx, type->name);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ ವರ್ಣ *
TypeMaskTxt(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *type)
{
    ಪ್ರತ್ಯಾಗಮ ModMaskText(info->ctx, MOD_BOTH, &info->mods, type->mods);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
ReportTypeShouldBeArray(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *type,
                        ಧ್ರುವ ವರ್ಣ *field)
{
    ಪ್ರತ್ಯಾಗಮ ReportShouldBeArray(info->ctx, "key type", field,
                               TypeTxt(info, type));
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
ReportTypeBadType(ಕೀಲಿ_Types_Info *info, ಗಣನಾ xkb_message_code code,
                  ಕೀಲಿ_Type_Info *type, ಧ್ರುವ ವರ್ಣ *field, ಧ್ರುವ ವರ್ಣ *wanted)
{
    ಪ್ರತ್ಯಾಗಮ ReportBadType(info->ctx, code, "key type", field,
                         TypeTxt(info, type), wanted);
}

/***====================================================================***/

ಸ್ಥಿರ ಶೂನ್ಯ
Init_ಕೀಲಿ_Types_Info(ಕೀಲಿ_Types_Info *info, ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
                 unsigned ಪೂರ್ಣಾಙ್ಕ include_depth,
                 ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods)
{
    memset(info, 0, ಪರಿಮಾಣ(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    InitVMods(&info->mods, mods, include_depth > 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ
Clear_ಕೀಲಿ_Type_Info(ಕೀಲಿ_Type_Info *type)
{
    darray_free(type->entries);
    darray_free(type->level_names);
}

ಸ್ಥಿರ ಶೂನ್ಯ
Clear_ಕೀಲಿ_Types_Info(ಕೀಲಿ_Types_Info *info)
{
    ಮುಕ್ತ(info->name);
    ಕೀಲಿ_Type_Info *type;
    darray_foreach(type, info->types)
        Clear_ಕೀಲಿ_Type_Info(type);
    darray_free(info->types);
}

ಸ್ಥಿರ ಕೀಲಿ_Type_Info *
Find_Matching_ಕೀಲಿ_Type(ಕೀಲಿ_Types_Info *info, xkb_atom_t name)
{
    ಕೀಲಿ_Type_Info *old;

    darray_foreach(old, info->types)
        ಯದಿ (old->name == name)
            ಪ್ರತ್ಯಾಗಮ old;

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ bool
Add_ಕೀಲಿ_Type(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *new, bool same_file)
{
    ಕೀಲಿ_Type_Info *old;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ verbosity = xkb_context_get_log_verbosity(info->ctx);

    old = Find_Matching_ಕೀಲಿ_Type(info, new->name);
    ಯದಿ (old) {
        ಯದಿ (new->merge != MERGE_AUGMENT) {
            ಯದಿ ((same_file && verbosity > 0) || verbosity > 9) {
                log_warn(info->ctx,
                         XKB_WARNING_CONFLICTING_ಕೀಲಿ_TYPE_DEFINITIONS,
                         "Multiple definitions of the %s key type; "
                         "Earlier definition ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            }

            Clear_ಕೀಲಿ_Type_Info(old);
            *old = *new;
            darray_init(new->entries);
            darray_init(new->level_names);
            ಪ್ರತ್ಯಾಗಮ true;
        }

        ಯದಿ (same_file)
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_CONFLICTING_ಕೀಲಿ_TYPE_DEFINITIONS,
                    "Multiple definitions of the %s key type; "
                    "Later definition ignored\n",
                    xkb_atom_text(info->ctx, new->name));

        Clear_ಕೀಲಿ_Type_Info(new);
        ಪ್ರತ್ಯಾಗಮ true;
    }

    darray_append(info->types, *new);
    ಪ್ರತ್ಯಾಗಮ true;
}

/***====================================================================***/

ಸ್ಥಿರ ಶೂನ್ಯ
Merge_Included_ಕೀಲಿ_Types(ಕೀಲಿ_Types_Info *into, ಕೀಲಿ_Types_Info *from,
                      ಗಣನಾ merge_mode merge)
{
    ಯದಿ (from->error_ಎಣಿಕೆ > 0) {
        into->error_ಎಣಿಕೆ += from->error_ಎಣಿಕೆ;
        ಪ್ರತ್ಯಾಗಮ;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    ಯದಿ (into->name == ರಿಕ್ತ) {
        into->name = steal(&from->name);
    }

    ಯದಿ (darray_empty(into->types)) {
        into->types = from->types;
        /* Types stolen via shallow copy, so reinitialize the array */
        darray_init(from->types);
    }
    ಅನ್ಯಥಾ {
        ಕೀಲಿ_Type_Info *type;
        darray_foreach(type, from->types) {
            type->merge = merge;
            ಯದಿ (!Add_ಕೀಲಿ_Type(into, type, false))
                into->error_ಎಣಿಕೆ++;
        }
        /* Types were either shallow copied or reinitialized individually
           in `AddKeyType`, so we only need to free the array */
        darray_free(from->types);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
Handle_ಕೀಲಿ_Types_File(ಕೀಲಿ_Types_Info *info, XkbFile *file);

ಸ್ಥಿರ bool
Handle_Include_ಕೀಲಿ_Types(ಕೀಲಿ_Types_Info *info, IncludeStmt *include)
{
    ಕೀಲಿ_Types_Info included;

    ಯದಿ (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_ಎಣಿಕೆ += 10;
        ಪ್ರತ್ಯಾಗಮ false;
    }

    Init_ಕೀಲಿ_Types_Info(&included, info->keymap_info, info->include_depth + 1,
                     &info->mods);
    included.name = steal(&include->stmt);

    ಧ್ರುವ ಸಂರಚನಾ parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    ಪ್ರತಿ (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        ಕೀಲಿ_Types_Info next_incl;
        XkbFile *file;

        ವರ್ಣ path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_TYPES,
                                  path, ಪರಿಮಾಣ(path));
        ಯದಿ (!file) {
            info->error_ಎಣಿಕೆ += 10;
            Clear_ಕೀಲಿ_Types_Info(&included);
            ಪ್ರತ್ಯಾಗಮ false;
        }

        Init_ಕೀಲಿ_Types_Info(&next_incl, info->keymap_info, info->include_depth + 1,
                         &included.mods);

        Handle_ಕೀಲಿ_Types_File(&next_incl, file);

        Merge_Included_ಕೀಲಿ_Types(&included, &next_incl, stmt->merge);

        Clear_ಕೀಲಿ_Types_Info(&next_incl);
        FreeXkbFile(file);
    }

    Merge_Included_ಕೀಲಿ_Types(info, &included, include->merge);
    Clear_ಕೀಲಿ_Types_Info(&included);

    ಪ್ರತ್ಯಾಗಮ (info->error_ಎಣಿಕೆ == 0);
}

/***====================================================================***/

ಸ್ಥಿರ bool
SetModifiers(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *type, ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx,
             ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ)
{
    xkb_mod_mask_t mods = 0;

    ಯದಿ (arrayNdx) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The modifiers field of a key type is not an array; "
                "Illegal array subscript ignored\n");
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_Mask(info->ctx, ಮೌಲ್ಯ, MOD_BOTH, &info->mods, &mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Key type mask field must be a modifier mask; "
                "Key type definition ignored\n");
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಯದಿ (type->defined & TYPE_FIELD_MASK) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple modifier mask definitions for key type %s; "
                 "Using %s, ignoring %s\n",
                 xkb_atom_text(info->ctx, type->name),
                 TypeMaskTxt(info, type),
                 ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods));
        ಪ್ರತ್ಯಾಗಮ false;
    }

    type->mods = mods;
    ಪ್ರತ್ಯಾಗಮ true;
}

/***====================================================================***/

ಸ್ಥಿರ ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry *
FindMatchingMapEntry(ಕೀಲಿ_Type_Info *type, xkb_mod_mask_t mods)
{
    ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry *entry;

    darray_foreach(entry, type->entries)
        ಯದಿ (entry->mods.mods == mods)
            ಪ್ರತ್ಯಾಗಮ entry;

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ bool
AddMapEntry(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *type,
            ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry *new, bool clobber, bool report)
{
    ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry *old;

    old = FindMatchingMapEntry(type, new->mods.mods);
    ಯದಿ (old) {
        ಯದಿ (report && old->level != new->level) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_TYPE_MAP_ENTRY,
                     "Multiple map entries for %s in %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     MapEntryTxt(info, new), TypeTxt(info, type),
                     (clobber ? new->level : old->level) + 1,
                     (clobber ? old->level : new->level) + 1);
        }
        ಅನ್ಯಥಾ {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_CONFLICTING_ಕೀಲಿ_TYPE_MAP_ENTRY,
                    "Multiple occurrences of map[%s]= %"PRIu32" in %s; Ignored\n",
                    MapEntryTxt(info, new), new->level + 1,
                    TypeTxt(info, type));
            ಪ್ರತ್ಯಾಗಮ true;
        }

        ಯದಿ (clobber) {
            ಯದಿ (new->level >= type->ಅಂಕೆ_levels)
                type->ಅಂಕೆ_levels = new->level + 1;
            old->level = new->level;
        }

        ಪ್ರತ್ಯಾಗಮ true;
    }

    ಯದಿ (new->level >= type->ಅಂಕೆ_levels)
        type->ಅಂಕೆ_levels = new->level + 1;

    darray_append(type->entries, *new);
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
SetMapEntry(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *type, ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx,
            ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ)
{
    ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry entry;

    ಯದಿ (arrayNdx == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ ReportTypeShouldBeArray(info, type, "map entry");

    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_Mask(info->ctx, arrayNdx, MOD_BOTH, &info->mods,
                            &entry.mods.mods))
        ಪ್ರತ್ಯಾಗಮ ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "map entry", "modifier mask");

    ಯದಿ (entry.mods.mods & (~type->mods)) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_IN_ಕೀಲಿ_TYPE,
                "Map entry for modifiers not used by type %s; "
                "Using %s instead of %s\n",
                TypeTxt(info, type),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods,
                            entry.mods.mods & type->mods),
                MapEntryTxt(info, &entry));
        entry.mods.mods &= type->mods;
    }

    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Level(info->ctx, ಮೌಲ್ಯ, &entry.level)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                          "Level specifications in a key type must be integer; "
                          "Ignoring malformed level specification\n");
        ಪ್ರತ್ಯಾಗಮ false;
    }

    entry.preserve.mods = 0;

    ಪ್ರತ್ಯಾಗಮ AddMapEntry(info, type, &entry, true, true);
}

/***====================================================================***/

ಸ್ಥಿರ bool
AddPreserve(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *type,
            xkb_mod_mask_t mods, xkb_mod_mask_t preserve_mods)
{
    ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry *entry;

    darray_foreach(entry, type->entries) {
        ಯದಿ (entry->mods.mods != mods)
            ಅನುವರ್ತ;

        /* Map exists without previous preserve (or "None"); override. */
        ಯದಿ (entry->preserve.mods == 0) {
            entry->preserve.mods = preserve_mods;
            ಪ್ರತ್ಯಾಗಮ true;
        }

        /* Map exists with same preserve; do nothing. */
        ಯದಿ (entry->preserve.mods == preserve_mods) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_DUPLICATE_ENTRY,
                    "Identical definitions for preserve[%s] in %s; "
                    "Ignored\n",
                    ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                    TypeTxt(info, type));
            ಪ್ರತ್ಯಾಗಮ true;
        }

        /* Map exists with different preserve; latter wins. */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_ಕೀಲಿ_TYPE_PRESERVE_ENTRIES,
                "Multiple definitions for preserve[%s] in %s; "
                "Using %s, ignoring %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods,
                            entry->preserve.mods));

        entry->preserve.mods = preserve_mods;
        ಪ್ರತ್ಯಾಗಮ true;
    }

    /*
     * Map does not exist, i.e. preserve[] came before map[].
     * Create a map with the specified mask mapping to Level1. The level
     * may be overridden later with an explicit map[] statement.
     */
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry new = {
        .level = 0,
        .mods = { .mods = mods },
        .preserve = { .mods = preserve_mods },
    };
    darray_append(type->entries, new);
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
SetPreserve(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *type, ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx,
            ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ)
{

    ಯದಿ (arrayNdx == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ ReportTypeShouldBeArray(info, type, "preserve entry");

    xkb_mod_mask_t mods = 0;
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_Mask(info->ctx, arrayNdx, MOD_BOTH, &info->mods, &mods))
        ಪ್ರತ್ಯಾಗಮ ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "preserve entry", "modifier mask");

    ಯದಿ (mods & ~type->mods) {
        ಧ್ರುವ ವರ್ಣ *before, *after;

        before = ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods);
        mods &= type->mods;
        after = ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_IN_ಕೀಲಿ_TYPE,
                "Preserve entry for modifiers not used by the %s type; "
                "Index %s converted to %s\n",
                TypeTxt(info, type), before, after);
    }

    xkb_mod_mask_t preserve_mods = 0;
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_Mask(info->ctx, ಮೌಲ್ಯ, MOD_BOTH, &info->mods,
                            &preserve_mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Preserve value in a key type is not a modifier mask; "
                "Ignoring preserve[%s] in type %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type));
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಯದಿ (preserve_mods & ~mods) {
        ಧ್ರುವ ವರ್ಣ *before, *after;

        before = ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods);
        preserve_mods &= mods;
        after = ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_ILLEGAL_ಕೀಲಿ_TYPE_PRESERVE_RESULT,
                "Illegal value for preserve[%s] in type %s; "
                "Converted %s to %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type), before, after);
    }

    ಪ್ರತ್ಯಾಗಮ AddPreserve(info, type, mods, preserve_mods);
}

/***====================================================================***/

ಸ್ಥಿರ bool
AddLevelName(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *type,
             xkb_level_index_t level, xkb_atom_t name, bool clobber)
{
    /* New name. */
    ಯದಿ (level >= darray_size(type->level_names)) {
        darray_resize0(type->level_names, level + 1);
        ಗಚ್ಛ finish;
    }

    /* Same level, same name. */
    ಯದಿ (darray_item(type->level_names, level) == name) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE, XKB_WARNING_DUPLICATE_ENTRY,
                "Duplicate names for level %"PRIu32" of key type %s; Ignored\n",
                level + 1, TypeTxt(info, type));
        ಪ್ರತ್ಯಾಗಮ true;
    }

    /* Same level, different name. */
    ಯದಿ (darray_item(type->level_names, level) != XKB_ATOM_NONE) {
        ಧ್ರುವ ವರ್ಣ *old, *new;
        old = xkb_atom_text(info->ctx,
                            darray_item(type->level_names, level));
        new = xkb_atom_text(info->ctx, name);
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_ಕೀಲಿ_TYPE_LEVEL_NAMES,
                "Multiple names for level %"PRIu32" of key type %s; "
                "Using %s, ignoring %s\n",
                level + 1, TypeTxt(info, type),
                (clobber ? new : old), (clobber ? old : new));

        ಯದಿ (!clobber)
            ಪ್ರತ್ಯಾಗಮ true;
    }

    /* FIXME: What about different level, same name? */

finish:
    darray_item(type->level_names, level) = name;
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
SetLevelName(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *type, ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx,
             ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ)
{
    ಯದಿ (arrayNdx == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ ReportTypeShouldBeArray(info, type, "level name");

    xkb_level_index_t level = 0;
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Level(info->ctx, arrayNdx, &level))
        ಪ್ರತ್ಯಾಗಮ ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                                 type, "level name", "integer");

    xkb_atom_t level_name = XKB_ATOM_NONE;
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_String(info->ctx, ಮೌಲ್ಯ, &level_name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Non-string name for level %"PRIu32" in key type %s; "
                "Ignoring illegal level name definition\n",
                level + 1, xkb_atom_text(info->ctx, type->name));
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಪ್ರತ್ಯಾಗಮ AddLevelName(info, type, level, level_name, true);
}

/***====================================================================***/

ಸ್ಥಿರ bool
Set_ಕೀಲಿ_Type_Field(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *type,
                ಧ್ರುವ ವರ್ಣ *field, ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx, ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ)
{
    bool ok = false;
    ಗಣನಾ type_field type_field = 0;

    ಯದಿ (istreq(field, "modifiers")) {
        type_field = TYPE_FIELD_MASK;
        ok = SetModifiers(info, type, arrayNdx, ಮೌಲ್ಯ);
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "map")) {
        type_field = TYPE_FIELD_MAP;
        ok = SetMapEntry(info, type, arrayNdx, ಮೌಲ್ಯ);
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "preserve")) {
        type_field = TYPE_FIELD_PRESERVE;
        ok = SetPreserve(info, type, arrayNdx, ಮೌಲ್ಯ);
    }
    ಅನ್ಯಥಾ ಯದಿ (istreq(field, "levelname") || istreq(field, "level_name")) {
        type_field = TYPE_FIELD_LEVEL_NAME;
        ok = SetLevelName(info, type, arrayNdx, ಮೌಲ್ಯ);
    } ಅನ್ಯಥಾ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in key type \"%s\"; Definition ignored\n",
                field, TypeTxt(info, type));
        ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_TYPE_FIELDS);
    }

    type->defined |= type_field;
    ಪ್ರತ್ಯಾಗಮ ok;
}

ಸ್ಥಿರ bool
Handle_ಕೀಲಿ_Type_Body(ಕೀಲಿ_Types_Info *info, VarDef *def, ಕೀಲಿ_Type_Info *type)
{
    bool ok = true;
    ಧ್ರುವ ವರ್ಣ *elem, *field;
    ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx;

    ಪ್ರತಿ (; def; def = (VarDef *) def->common.next) {
        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Lhs(info->ctx, def->name, &elem, &field,
                            &arrayNdx)) {
            /* internal error, already reported */
            ok = false;
            ಅನುವರ್ತ;
        }

        ಯದಿ (elem) {
            ಯದಿ (istreq(elem, "type")) {
                log_err(info->ctx, XKB_ERROR_INVALID_SET_DEFAULT_STATEMENT,
                        "Support for changing the default type has been removed; "
                        "Statement \"%s.%s\" ignored.\n", elem, field);
            }
            ಅನ್ಯಥಾ {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key type statement: move statements to the global "
                        "file scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
            ಅನುವರ್ತ;
        }

        ಯದಿ (!Set_ಕೀಲಿ_Type_Field(info, type, field, arrayNdx, def->ಮೌಲ್ಯ))
            ok = false;
    }

    ಪ್ರತ್ಯಾಗಮ ok;
}

ಸ್ಥಿರ bool
Handle_ಕೀಲಿ_Type_Def(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Def *def)
{
    ಕೀಲಿ_Type_Info type = {
        .defined = 0,
        .merge = def->merge,
        .name = def->name,
        .mods = 0,
        .ಅಂಕೆ_levels = 1,
        .entries = darray_new(),
        .level_names = darray_new(),
    };

    ಯದಿ (!Handle_ಕೀಲಿ_Type_Body(info, def->body, &type) ||
        !Add_ಕೀಲಿ_Type(info, &type, true))
    {
        info->error_ಎಣಿಕೆ++;
        Clear_ಕೀಲಿ_Type_Info(&type);
        ಪ್ರತ್ಯಾಗಮ false;
    }

    /* Type has been either stolen via shallow copy or reinitialized in
       `AddKeyType`: no need to free the arrays */
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
HandleGlobalVar(ಕೀಲಿ_Types_Info *info, VarDef *stmt)
{
    ಧ್ರುವ ವರ್ಣ *elem, *field;
    ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx;

    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Lhs(info->ctx, stmt->name, &elem, &field,
                        &arrayNdx)) {
        ಪ್ರತ್ಯಾಗಮ false;           /* internal error, already reported */
    }
    ಅನ್ಯಥಾ ಯದಿ (elem && istreq(elem, "type")) {
        log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                "Support for changing the default type has been removed; "
                "Statement ignored\n");
        ಪ್ರತ್ಯಾಗಮ true;
    }
    ಅನ್ಯಥಾ ಯದಿ (elem) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown element \"%s\"; "
                "Value for field \"%s.%s\" ignored\n", elem, elem, field);
        ಪ್ರತ್ಯಾಗಮ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
    }
    ಅನ್ಯಥಾ ಯದಿ (field) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        ಪ್ರತ್ಯಾಗಮ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS);
    }
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ
Handle_ಕೀಲಿ_Types_File(ಕೀಲಿ_Types_Info *info, XkbFile *file)
{
    bool ok;

    ಮುಕ್ತ(info->name);
    info->name = strdup_safe(file->name);

    ಪ್ರತಿ (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        ವಿಕಲ್ಪ (stmt->type) {
        ಪಕ್ಷ STMT_INCLUDE:
            ok = Handle_Include_ಕೀಲಿ_Types(info, (IncludeStmt *) stmt);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_TYPE:
            ok = Handle_ಕೀಲಿ_Type_Def(info, (ಕೀಲಿ_Type_Def *) stmt);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef*) stmt);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_UNKNOWN_DECLARATION:
        ಪಕ್ಷ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported types %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ಭಙ್ಗ;
        ಸ್ವಭಾವ:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Key type files may not include other declarations; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            ಭಙ್ಗ;
        }

        ಯದಿ (!ok)
            info->error_ಎಣಿಕೆ++;

        ಯದಿ (info->error_ಎಣಿಕೆ > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning keytypes file \"%s\"\n",
                    safe_map_name(file->name));
            ಭಙ್ಗ;
        }
    }
}

/***====================================================================***/

ಸ್ಥಿರ bool
Copy_ಕೀಲಿ_Types_To_Keymap(ಸಂರಚನಾ xkb_keymap *keymap, ಕೀಲಿ_Types_Info *info)
{
    ಧ್ರುವ darray_size_t ಅಂಕೆ_types = darray_empty(info->types)
                                  ? 1
                                  : darray_size(info->types);
    ಸಂರಚನಾ xkb_ಕೀಲಿ_type *types = calloc(ಅಂಕೆ_types, ಪರಿಮಾಣ(*types));
    ಯದಿ (!types)
        ಪ್ರತ್ಯಾಗಮ false;

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
    ಯದಿ (darray_empty(info->types)) {
        ಸಂರಚನಾ xkb_ಕೀಲಿ_type *type = &types[0];

        type->mods.mods = 0;
        type->ಅಂಕೆ_levels = 1;
        type->entries = ರಿಕ್ತ;
        type->ಅಂಕೆ_entries = 0;
        type->name = xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL");
        type->level_names = ರಿಕ್ತ;
        type->ಅಂಕೆ_level_names = 0;
        type->required = true;
    }
    ಅನ್ಯಥಾ {
        /* HACK to circumvent Xorg bugs (see comment above) */
        ಧ್ರುವ xkb_atom_t canonical_types[] = {
            xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "TWO_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "ALPHABETIC"),
            xkb_atom_intern_literal(keymap->ctx, "KEYPAD"),
        };

        ಪ್ರತಿ (darray_size_t i = 0; i < ಅಂಕೆ_types; i++) {
            ಕೀಲಿ_Type_Info *def = &darray_item(info->types, i);
            ಸಂರಚನಾ xkb_ಕೀಲಿ_type *type = &types[i];

            type->name = def->name;
            type->mods.mods = def->mods;
            type->ಅಂಕೆ_levels = def->ಅಂಕೆ_levels;
            type->ಅಂಕೆ_level_names =
                (xkb_level_index_t) darray_size(def->level_names);
            darray_steal(def->level_names, &type->level_names, ರಿಕ್ತ);
            darray_steal(def->entries, &type->entries, &type->ಅಂಕೆ_entries);
            type->required = false;

            /* HACK: Never drop canonical XKB key types (see comment above) */
            ಯದಿ (type->ಅಂಕೆ_levels <= 2) {
                ಪ್ರತಿ (uint8_t t = 0;
                     t < (uint8_t) ARRAY_SIZE(canonical_types);
                     t++) {
                        ಯದಿ (type->name == canonical_types[t]) {
                            type->required = true;
                            ಭಙ್ಗ;
                        }
                }
            }
        }
    }

    keymap->types_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->types_section_name);
    keymap->ಅಂಕೆ_types = ಅಂಕೆ_types;
    keymap->types = types;
    keymap->mods = info->mods;
    ಪ್ರತ್ಯಾಗಮ true;
}

/***====================================================================***/

bool
Compile_ಕೀಲಿ_Types(XkbFile *file, ಸಂರಚನಾ xkb_keymap_info *keymap_info)
{
    ಕೀಲಿ_Types_Info info;

    Init_ಕೀಲಿ_Types_Info(&info, keymap_info, 0,
                     &keymap_info->keymap.mods);

    ಯದಿ (file != ರಿಕ್ತ)
        Handle_ಕೀಲಿ_Types_File(&info, file);

    ಯದಿ (info.error_ಎಣಿಕೆ != 0)
        ಗಚ್ಛ err_info;

    ಯದಿ (!Copy_ಕೀಲಿ_Types_To_Keymap(&keymap_info->keymap, &info))
        ಗಚ್ಛ err_info;

    Clear_ಕೀಲಿ_Types_Info(&info);
    ಪ್ರತ್ಯಾಗಮ true;

err_info:
    Clear_ಕೀಲಿ_Types_Info(&info);
    ಪ್ರತ್ಯಾಗಮ false;
}
