/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <limits.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon-status.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp-priv.h"
#ଅନ୍ତର୍ଭାଵ "text.h"
#ଅନ୍ତର୍ଭାଵ "vmod.h"
#ଅନ୍ତର୍ଭାଵ "expr.h"
#ଅନ୍ତର୍ଭାଵ "include.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "util-mem.h"

ଗଣନା type_field {
    TYPE_FIELD_MASK = (1 << 0),
    TYPE_FIELD_MAP = (1 << 1),
    TYPE_FIELD_PRESERVE = (1 << 2),
    TYPE_FIELD_LEVEL_NAME = (1 << 3),
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ଗଣନା type_field defined;
    ଗଣନା merge_mode merge;

    xkb_atom_t name;
    xkb_mod_mask_t mods;
    xkb_level_index_t ଅଙ୍କ_levels;
    darray(ସଂରଚନା xkb_ଚାବି_type_entry) entries;
    darray(xkb_atom_t) level_names;
} ଚାବି_Type_Info;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ଅକ୍ଷର *name;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା error_ଗଣନା;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_depth;

    darray(ଚାବି_Type_Info) types;
    ସଂରଚନା xkb_mod_set mods;

    ସଂରଚନା xkb_context *ctx;
    ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info;
} ଚାବି_Types_Info;

/***====================================================================***/

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ ଅକ୍ଷର *
MapEntryTxt(ଚାବି_Types_Info *info, ସଂରଚନା xkb_ଚାବି_type_entry *entry)
{
    ଫେରନ୍ତୁ ModMaskText(info->ctx, MOD_BOTH, &info->mods, entry->mods.mods);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ ଅକ୍ଷର *
TypeTxt(ଚାବି_Types_Info *info, ଚାବି_Type_Info *type)
{
    ଫେରନ୍ତୁ xkb_atom_text(info->ctx, type->name);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ ଅକ୍ଷର *
TypeMaskTxt(ଚାବି_Types_Info *info, ଚାବି_Type_Info *type)
{
    ଫେରନ୍ତୁ ModMaskText(info->ctx, MOD_BOTH, &info->mods, type->mods);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
ReportTypeShouldBeArray(ଚାବି_Types_Info *info, ଚାବି_Type_Info *type,
                        ଅଚଳ ଅକ୍ଷର *field)
{
    ଫେରନ୍ତୁ ReportShouldBeArray(info->ctx, "key type", field,
                               TypeTxt(info, type));
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
ReportTypeBadType(ଚାବି_Types_Info *info, ଗଣନା xkb_message_code code,
                  ଚାବି_Type_Info *type, ଅଚଳ ଅକ୍ଷର *field, ଅଚଳ ଅକ୍ଷର *wanted)
{
    ଫେରନ୍ତୁ ReportBadType(info->ctx, code, "key type", field,
                         TypeTxt(info, type), wanted);
}

/***====================================================================***/

ସ୍ଥିର ଶୂନ୍ୟ
Init_ଚାବି_Types_Info(ଚାବି_Types_Info *info, ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
                 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_depth,
                 ଅଚଳ ସଂରଚନା xkb_mod_set *mods)
{
    memset(info, 0, ଆକାର(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    InitVMods(&info->mods, mods, include_depth > 0);
}

ସ୍ଥିର ଶୂନ୍ୟ
Clear_ଚାବି_Type_Info(ଚାବି_Type_Info *type)
{
    darray_free(type->entries);
    darray_free(type->level_names);
}

ସ୍ଥିର ଶୂନ୍ୟ
Clear_ଚାବି_Types_Info(ଚାବି_Types_Info *info)
{
    ମୁକ୍ତ(info->name);
    ଚାବି_Type_Info *type;
    darray_foreach(type, info->types)
        Clear_ଚାବି_Type_Info(type);
    darray_free(info->types);
}

ସ୍ଥିର ଚାବି_Type_Info *
Find_Matching_ଚାବି_Type(ଚାବି_Types_Info *info, xkb_atom_t name)
{
    ଚାବି_Type_Info *old;

    darray_foreach(old, info->types)
        ଯଦି (old->name == name)
            ଫେରନ୍ତୁ old;

    ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର bool
Add_ଚାବି_Type(ଚାବି_Types_Info *info, ଚାବି_Type_Info *new, bool same_file)
{
    ଚାବି_Type_Info *old;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity = xkb_context_get_log_verbosity(info->ctx);

    old = Find_Matching_ଚାବି_Type(info, new->name);
    ଯଦି (old) {
        ଯଦି (new->merge != MERGE_AUGMENT) {
            ଯଦି ((same_file && verbosity > 0) || verbosity > 9) {
                log_warn(info->ctx,
                         XKB_WARNING_CONFLICTING_ଚାବି_TYPE_DEFINITIONS,
                         "Multiple definitions of the %s key type; "
                         "Earlier definition ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            }

            Clear_ଚାବି_Type_Info(old);
            *old = *new;
            darray_init(new->entries);
            darray_init(new->level_names);
            ଫେରନ୍ତୁ true;
        }

        ଯଦି (same_file)
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_CONFLICTING_ଚାବି_TYPE_DEFINITIONS,
                    "Multiple definitions of the %s key type; "
                    "Later definition ignored\n",
                    xkb_atom_text(info->ctx, new->name));

        Clear_ଚାବି_Type_Info(new);
        ଫେରନ୍ତୁ true;
    }

    darray_append(info->types, *new);
    ଫେରନ୍ତୁ true;
}

/***====================================================================***/

ସ୍ଥିର ଶୂନ୍ୟ
Merge_Included_ଚାବି_Types(ଚାବି_Types_Info *into, ଚାବି_Types_Info *from,
                      ଗଣନା merge_mode merge)
{
    ଯଦି (from->error_ଗଣନା > 0) {
        into->error_ଗଣନା += from->error_ଗଣନା;
        ଫେରନ୍ତୁ;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    ଯଦି (into->name == ଖାଲି) {
        into->name = steal(&from->name);
    }

    ଯଦି (darray_empty(into->types)) {
        into->types = from->types;
        /* Types stolen via shallow copy, so reinitialize the array */
        darray_init(from->types);
    }
    ଅନ୍ୟଥା {
        ଚାବି_Type_Info *type;
        darray_foreach(type, from->types) {
            type->merge = merge;
            ଯଦି (!Add_ଚାବି_Type(into, type, false))
                into->error_ଗଣନା++;
        }
        /* Types were either shallow copied or reinitialized individually
           in `AddKeyType`, so we only need to free the array */
        darray_free(from->types);
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
Handle_ଚାବି_Types_File(ଚାବି_Types_Info *info, XkbFile *file);

ସ୍ଥିର bool
Handle_Include_ଚାବି_Types(ଚାବି_Types_Info *info, IncludeStmt *include)
{
    ଚାବି_Types_Info included;

    ଯଦି (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_ଗଣନା += 10;
        ଫେରନ୍ତୁ false;
    }

    Init_ଚାବି_Types_Info(&included, info->keymap_info, info->include_depth + 1,
                     &info->mods);
    included.name = steal(&include->stmt);

    ଅଚଳ ସଂରଚନା parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    ପାଇଁ (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        ଚାବି_Types_Info next_incl;
        XkbFile *file;

        ଅକ୍ଷର path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_TYPES,
                                  path, ଆକାର(path));
        ଯଦି (!file) {
            info->error_ଗଣନା += 10;
            Clear_ଚାବି_Types_Info(&included);
            ଫେରନ୍ତୁ false;
        }

        Init_ଚାବି_Types_Info(&next_incl, info->keymap_info, info->include_depth + 1,
                         &included.mods);

        Handle_ଚାବି_Types_File(&next_incl, file);

        Merge_Included_ଚାବି_Types(&included, &next_incl, stmt->merge);

        Clear_ଚାବି_Types_Info(&next_incl);
        FreeXkbFile(file);
    }

    Merge_Included_ଚାବି_Types(info, &included, include->merge);
    Clear_ଚାବି_Types_Info(&included);

    ଫେରନ୍ତୁ (info->error_ଗଣନା == 0);
}

/***====================================================================***/

ସ୍ଥିର bool
SetModifiers(ଚାବି_Types_Info *info, ଚାବି_Type_Info *type, ପଦସମଷ୍ଟି_Def *arrayNdx,
             ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ)
{
    xkb_mod_mask_t mods = 0;

    ଯଦି (arrayNdx) {
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "The modifiers field of a key type is not an array; "
                "Illegal array subscript ignored\n");
        ଫେରନ୍ତୁ false;
    }

    ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Mod_Mask(info->ctx, ମୂଲ୍ୟ, MOD_BOTH, &info->mods, &mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Key type mask field must be a modifier mask; "
                "Key type definition ignored\n");
        ଫେରନ୍ତୁ false;
    }

    ଯଦି (type->defined & TYPE_FIELD_MASK) {
        log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Multiple modifier mask definitions for key type %s; "
                 "Using %s, ignoring %s\n",
                 xkb_atom_text(info->ctx, type->name),
                 TypeMaskTxt(info, type),
                 ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods));
        ଫେରନ୍ତୁ false;
    }

    type->mods = mods;
    ଫେରନ୍ତୁ true;
}

/***====================================================================***/

ସ୍ଥିର ସଂରଚନା xkb_ଚାବି_type_entry *
FindMatchingMapEntry(ଚାବି_Type_Info *type, xkb_mod_mask_t mods)
{
    ସଂରଚନା xkb_ଚାବି_type_entry *entry;

    darray_foreach(entry, type->entries)
        ଯଦି (entry->mods.mods == mods)
            ଫେରନ୍ତୁ entry;

    ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର bool
AddMapEntry(ଚାବି_Types_Info *info, ଚାବି_Type_Info *type,
            ସଂରଚନା xkb_ଚାବି_type_entry *new, bool clobber, bool report)
{
    ସଂରଚନା xkb_ଚାବି_type_entry *old;

    old = FindMatchingMapEntry(type, new->mods.mods);
    ଯଦି (old) {
        ଯଦି (report && old->level != new->level) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_ଚାବି_TYPE_MAP_ENTRY,
                     "Multiple map entries for %s in %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     MapEntryTxt(info, new), TypeTxt(info, type),
                     (clobber ? new->level : old->level) + 1,
                     (clobber ? old->level : new->level) + 1);
        }
        ଅନ୍ୟଥା {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_CONFLICTING_ଚାବି_TYPE_MAP_ENTRY,
                    "Multiple occurrences of map[%s]= %"PRIu32" in %s; Ignored\n",
                    MapEntryTxt(info, new), new->level + 1,
                    TypeTxt(info, type));
            ଫେରନ୍ତୁ true;
        }

        ଯଦି (clobber) {
            ଯଦି (new->level >= type->ଅଙ୍କ_levels)
                type->ଅଙ୍କ_levels = new->level + 1;
            old->level = new->level;
        }

        ଫେରନ୍ତୁ true;
    }

    ଯଦି (new->level >= type->ଅଙ୍କ_levels)
        type->ଅଙ୍କ_levels = new->level + 1;

    darray_append(type->entries, *new);
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
SetMapEntry(ଚାବି_Types_Info *info, ଚାବି_Type_Info *type, ପଦସମଷ୍ଟି_Def *arrayNdx,
            ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ)
{
    ସଂରଚନା xkb_ଚାବି_type_entry entry;

    ଯଦି (arrayNdx == ଖାଲି)
        ଫେରନ୍ତୁ ReportTypeShouldBeArray(info, type, "map entry");

    ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Mod_Mask(info->ctx, arrayNdx, MOD_BOTH, &info->mods,
                            &entry.mods.mods))
        ଫେରନ୍ତୁ ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "map entry", "modifier mask");

    ଯଦି (entry.mods.mods & (~type->mods)) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_IN_ଚାବି_TYPE,
                "Map entry for modifiers not used by type %s; "
                "Using %s instead of %s\n",
                TypeTxt(info, type),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods,
                            entry.mods.mods & type->mods),
                MapEntryTxt(info, &entry));
        entry.mods.mods &= type->mods;
    }

    ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Level(info->ctx, ମୂଲ୍ୟ, &entry.level)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                          "Level specifications in a key type must be integer; "
                          "Ignoring malformed level specification\n");
        ଫେରନ୍ତୁ false;
    }

    entry.preserve.mods = 0;

    ଫେରନ୍ତୁ AddMapEntry(info, type, &entry, true, true);
}

/***====================================================================***/

ସ୍ଥିର bool
AddPreserve(ଚାବି_Types_Info *info, ଚାବି_Type_Info *type,
            xkb_mod_mask_t mods, xkb_mod_mask_t preserve_mods)
{
    ସଂରଚନା xkb_ଚାବି_type_entry *entry;

    darray_foreach(entry, type->entries) {
        ଯଦି (entry->mods.mods != mods)
            ଜାରି;

        /* Map exists without previous preserve (or "None"); override. */
        ଯଦି (entry->preserve.mods == 0) {
            entry->preserve.mods = preserve_mods;
            ଫେରନ୍ତୁ true;
        }

        /* Map exists with same preserve; do nothing. */
        ଯଦି (entry->preserve.mods == preserve_mods) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_DUPLICATE_ENTRY,
                    "Identical definitions for preserve[%s] in %s; "
                    "Ignored\n",
                    ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                    TypeTxt(info, type));
            ଫେରନ୍ତୁ true;
        }

        /* Map exists with different preserve; latter wins. */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_ଚାବି_TYPE_PRESERVE_ENTRIES,
                "Multiple definitions for preserve[%s] in %s; "
                "Using %s, ignoring %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods),
                ModMaskText(info->ctx, MOD_BOTH, &info->mods,
                            entry->preserve.mods));

        entry->preserve.mods = preserve_mods;
        ଫେରନ୍ତୁ true;
    }

    /*
     * Map does not exist, i.e. preserve[] came before map[].
     * Create a map with the specified mask mapping to Level1. The level
     * may be overridden later with an explicit map[] statement.
     */
    ଅଚଳ ସଂରଚନା xkb_ଚାବି_type_entry new = {
        .level = 0,
        .mods = { .mods = mods },
        .preserve = { .mods = preserve_mods },
    };
    darray_append(type->entries, new);
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
SetPreserve(ଚାବି_Types_Info *info, ଚାବି_Type_Info *type, ପଦସମଷ୍ଟି_Def *arrayNdx,
            ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ)
{

    ଯଦି (arrayNdx == ଖାଲି)
        ଫେରନ୍ତୁ ReportTypeShouldBeArray(info, type, "preserve entry");

    xkb_mod_mask_t mods = 0;
    ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Mod_Mask(info->ctx, arrayNdx, MOD_BOTH, &info->mods, &mods))
        ଫେରନ୍ତୁ ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK_,
                                 type, "preserve entry", "modifier mask");

    ଯଦି (mods & ~type->mods) {
        ଅଚଳ ଅକ୍ଷର *before, *after;

        before = ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods);
        mods &= type->mods;
        after = ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_IN_ଚାବି_TYPE,
                "Preserve entry for modifiers not used by the %s type; "
                "Index %s converted to %s\n",
                TypeTxt(info, type), before, after);
    }

    xkb_mod_mask_t preserve_mods = 0;
    ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Mod_Mask(info->ctx, ମୂଲ୍ୟ, MOD_BOTH, &info->mods,
                            &preserve_mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "Preserve value in a key type is not a modifier mask; "
                "Ignoring preserve[%s] in type %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type));
        ଫେରନ୍ତୁ false;
    }

    ଯଦି (preserve_mods & ~mods) {
        ଅଚଳ ଅକ୍ଷର *before, *after;

        before = ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods);
        preserve_mods &= mods;
        after = ModMaskText(info->ctx, MOD_BOTH, &info->mods, preserve_mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_ILLEGAL_ଚାବି_TYPE_PRESERVE_RESULT,
                "Illegal value for preserve[%s] in type %s; "
                "Converted %s to %s\n",
                ModMaskText(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type), before, after);
    }

    ଫେରନ୍ତୁ AddPreserve(info, type, mods, preserve_mods);
}

/***====================================================================***/

ସ୍ଥିର bool
AddLevelName(ଚାବି_Types_Info *info, ଚାବି_Type_Info *type,
             xkb_level_index_t level, xkb_atom_t name, bool clobber)
{
    /* New name. */
    ଯଦି (level >= darray_size(type->level_names)) {
        darray_resize0(type->level_names, level + 1);
        ଯାଅ finish;
    }

    /* Same level, same name. */
    ଯଦି (darray_item(type->level_names, level) == name) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE, XKB_WARNING_DUPLICATE_ENTRY,
                "Duplicate names for level %"PRIu32" of key type %s; Ignored\n",
                level + 1, TypeTxt(info, type));
        ଫେରନ୍ତୁ true;
    }

    /* Same level, different name. */
    ଯଦି (darray_item(type->level_names, level) != XKB_ATOM_NONE) {
        ଅଚଳ ଅକ୍ଷର *old, *new;
        old = xkb_atom_text(info->ctx,
                            darray_item(type->level_names, level));
        new = xkb_atom_text(info->ctx, name);
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_ଚାବି_TYPE_LEVEL_NAMES,
                "Multiple names for level %"PRIu32" of key type %s; "
                "Using %s, ignoring %s\n",
                level + 1, TypeTxt(info, type),
                (clobber ? new : old), (clobber ? old : new));

        ଯଦି (!clobber)
            ଫେରନ୍ତୁ true;
    }

    /* FIXME: What about different level, same name? */

finish:
    darray_item(type->level_names, level) = name;
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
SetLevelName(ଚାବି_Types_Info *info, ଚାବି_Type_Info *type, ପଦସମଷ୍ଟି_Def *arrayNdx,
             ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ)
{
    ଯଦି (arrayNdx == ଖାଲି)
        ଫେରନ୍ତୁ ReportTypeShouldBeArray(info, type, "level name");

    xkb_level_index_t level = 0;
    ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Level(info->ctx, arrayNdx, &level))
        ଫେରନ୍ତୁ ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                                 type, "level name", "integer");

    xkb_atom_t level_name = XKB_ATOM_NONE;
    ଯଦି (!ପଦସମଷ୍ଟି_Resolve_String(info->ctx, ମୂଲ୍ୟ, &level_name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Non-string name for level %"PRIu32" in key type %s; "
                "Ignoring illegal level name definition\n",
                level + 1, xkb_atom_text(info->ctx, type->name));
        ଫେରନ୍ତୁ false;
    }

    ଫେରନ୍ତୁ AddLevelName(info, type, level, level_name, true);
}

/***====================================================================***/

ସ୍ଥିର bool
Set_ଚାବି_Type_Field(ଚାବି_Types_Info *info, ଚାବି_Type_Info *type,
                ଅଚଳ ଅକ୍ଷର *field, ପଦସମଷ୍ଟି_Def *arrayNdx, ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ)
{
    bool ok = false;
    ଗଣନା type_field type_field = 0;

    ଯଦି (istreq(field, "modifiers")) {
        type_field = TYPE_FIELD_MASK;
        ok = SetModifiers(info, type, arrayNdx, ମୂଲ୍ୟ);
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "map")) {
        type_field = TYPE_FIELD_MAP;
        ok = SetMapEntry(info, type, arrayNdx, ମୂଲ୍ୟ);
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "preserve")) {
        type_field = TYPE_FIELD_PRESERVE;
        ok = SetPreserve(info, type, arrayNdx, ମୂଲ୍ୟ);
    }
    ଅନ୍ୟଥା ଯଦି (istreq(field, "levelname") || istreq(field, "level_name")) {
        type_field = TYPE_FIELD_LEVEL_NAME;
        ok = SetLevelName(info, type, arrayNdx, ମୂଲ୍ୟ);
    } ଅନ୍ୟଥା {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in key type \"%s\"; Definition ignored\n",
                field, TypeTxt(info, type));
        ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_TYPE_FIELDS);
    }

    type->defined |= type_field;
    ଫେରନ୍ତୁ ok;
}

ସ୍ଥିର bool
Handle_ଚାବି_Type_Body(ଚାବି_Types_Info *info, VarDef *def, ଚାବି_Type_Info *type)
{
    bool ok = true;
    ଅଚଳ ଅକ୍ଷର *elem, *field;
    ପଦସମଷ୍ଟି_Def *arrayNdx;

    ପାଇଁ (; def; def = (VarDef *) def->common.next) {
        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Lhs(info->ctx, def->name, &elem, &field,
                            &arrayNdx)) {
            /* internal error, already reported */
            ok = false;
            ଜାରି;
        }

        ଯଦି (elem) {
            ଯଦି (istreq(elem, "type")) {
                log_err(info->ctx, XKB_ERROR_INVALID_SET_DEFAULT_STATEMENT,
                        "Support for changing the default type has been removed; "
                        "Statement \"%s.%s\" ignored.\n", elem, field);
            }
            ଅନ୍ୟଥା {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key type statement: move statements to the global "
                        "file scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
            ଜାରି;
        }

        ଯଦି (!Set_ଚାବି_Type_Field(info, type, field, arrayNdx, def->ମୂଲ୍ୟ))
            ok = false;
    }

    ଫେରନ୍ତୁ ok;
}

ସ୍ଥିର bool
Handle_ଚାବି_Type_Def(ଚାବି_Types_Info *info, ଚାବି_Type_Def *def)
{
    ଚାବି_Type_Info type = {
        .defined = 0,
        .merge = def->merge,
        .name = def->name,
        .mods = 0,
        .ଅଙ୍କ_levels = 1,
        .entries = darray_new(),
        .level_names = darray_new(),
    };

    ଯଦି (!Handle_ଚାବି_Type_Body(info, def->body, &type) ||
        !Add_ଚାବି_Type(info, &type, true))
    {
        info->error_ଗଣନା++;
        Clear_ଚାବି_Type_Info(&type);
        ଫେରନ୍ତୁ false;
    }

    /* Type has been either stolen via shallow copy or reinitialized in
       `AddKeyType`: no need to free the arrays */
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
HandleGlobalVar(ଚାବି_Types_Info *info, VarDef *stmt)
{
    ଅଚଳ ଅକ୍ଷର *elem, *field;
    ପଦସମଷ୍ଟି_Def *arrayNdx;

    ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Lhs(info->ctx, stmt->name, &elem, &field,
                        &arrayNdx)) {
        ଫେରନ୍ତୁ false;           /* internal error, already reported */
    }
    ଅନ୍ୟଥା ଯଦି (elem && istreq(elem, "type")) {
        log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                "Support for changing the default type has been removed; "
                "Statement ignored\n");
        ଫେରନ୍ତୁ true;
    }
    ଅନ୍ୟଥା ଯଦି (elem) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown element \"%s\"; "
                "Value for field \"%s.%s\" ignored\n", elem, elem, field);
        ଫେରନ୍ତୁ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
    }
    ଅନ୍ୟଥା ଯଦି (field) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        ଫେରନ୍ତୁ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS);
    }
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଶୂନ୍ୟ
Handle_ଚାବି_Types_File(ଚାବି_Types_Info *info, XkbFile *file)
{
    bool ok;

    ମୁକ୍ତ(info->name);
    info->name = strdup_safe(file->name);

    ପାଇଁ (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        ବାଛ (stmt->type) {
        ସ୍ଥିତି STMT_INCLUDE:
            ok = Handle_Include_ଚାବି_Types(info, (IncludeStmt *) stmt);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_TYPE:
            ok = Handle_ଚାବି_Type_Def(info, (ଚାବି_Type_Def *) stmt);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef*) stmt);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_UNKNOWN_DECLARATION:
        ସ୍ଥିତି STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported types %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Key type files may not include other declarations; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            ଭାଙ୍ଗ;
        }

        ଯଦି (!ok)
            info->error_ଗଣନା++;

        ଯଦି (info->error_ଗଣନା > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning keytypes file \"%s\"\n",
                    safe_map_name(file->name));
            ଭାଙ୍ଗ;
        }
    }
}

/***====================================================================***/

ସ୍ଥିର bool
Copy_ଚାବି_Types_To_Keymap(ସଂରଚନା xkb_keymap *keymap, ଚାବି_Types_Info *info)
{
    ଅଚଳ darray_size_t ଅଙ୍କ_types = darray_empty(info->types)
                                  ? 1
                                  : darray_size(info->types);
    ସଂରଚନା xkb_ଚାବି_type *types = calloc(ଅଙ୍କ_types, ଆକାର(*types));
    ଯଦି (!types)
        ଫେରନ୍ତୁ false;

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
    ଯଦି (darray_empty(info->types)) {
        ସଂରଚନା xkb_ଚାବି_type *type = &types[0];

        type->mods.mods = 0;
        type->ଅଙ୍କ_levels = 1;
        type->entries = ଖାଲି;
        type->ଅଙ୍କ_entries = 0;
        type->name = xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL");
        type->level_names = ଖାଲି;
        type->ଅଙ୍କ_level_names = 0;
        type->required = true;
    }
    ଅନ୍ୟଥା {
        /* HACK to circumvent Xorg bugs (see comment above) */
        ଅଚଳ xkb_atom_t canonical_types[] = {
            xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "TWO_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "ALPHABETIC"),
            xkb_atom_intern_literal(keymap->ctx, "KEYPAD"),
        };

        ପାଇଁ (darray_size_t i = 0; i < ଅଙ୍କ_types; i++) {
            ଚାବି_Type_Info *def = &darray_item(info->types, i);
            ସଂରଚନା xkb_ଚାବି_type *type = &types[i];

            type->name = def->name;
            type->mods.mods = def->mods;
            type->ଅଙ୍କ_levels = def->ଅଙ୍କ_levels;
            type->ଅଙ୍କ_level_names =
                (xkb_level_index_t) darray_size(def->level_names);
            darray_steal(def->level_names, &type->level_names, ଖାଲି);
            darray_steal(def->entries, &type->entries, &type->ଅଙ୍କ_entries);
            type->required = false;

            /* HACK: Never drop canonical XKB key types (see comment above) */
            ଯଦି (type->ଅଙ୍କ_levels <= 2) {
                ପାଇଁ (uint8_t t = 0;
                     t < (uint8_t) ARRAY_SIZE(canonical_types);
                     t++) {
                        ଯଦି (type->name == canonical_types[t]) {
                            type->required = true;
                            ଭାଙ୍ଗ;
                        }
                }
            }
        }
    }

    keymap->types_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->types_section_name);
    keymap->ଅଙ୍କ_types = ଅଙ୍କ_types;
    keymap->types = types;
    keymap->mods = info->mods;
    ଫେରନ୍ତୁ true;
}

/***====================================================================***/

bool
Compile_ଚାବି_Types(XkbFile *file, ସଂରଚନା xkb_keymap_info *keymap_info)
{
    ଚାବି_Types_Info info;

    Init_ଚାବି_Types_Info(&info, keymap_info, 0,
                     &keymap_info->keymap.mods);

    ଯଦି (file != ଖାଲି)
        Handle_ଚାବି_Types_File(&info, file);

    ଯଦି (info.error_ଗଣନା != 0)
        ଯାଅ err_info;

    ଯଦି (!Copy_ଚାବି_Types_To_Keymap(&keymap_info->keymap, &info))
        ଯାଅ err_info;

    Clear_ଚାବି_Types_Info(&info);
    ଫେରନ୍ତୁ true;

err_info:
    Clear_ଚାବି_Types_Info(&info);
    ଫେରନ୍ତୁ false;
}
