/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <limits.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "xkbcommon/xkbcommon-status.h"
#అన్తర్భావ "xkbcomp-priv.h"
#అన్తర్భావ "text.h"
#అన్తర్భావ "vmod.h"
#అన్తర్భావ "expr.h"
#అన్తర్భావ "include.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "util-mem.h"

గణన type_క్షేత్రం {
    TYPE_క్షేత్రం_ముసుగు = (1 << 0),
    TYPE_క్షేత్రం_MAP = (1 << 1),
    TYPE_క్షేత్రం_PRESERVE = (1 << 2),
    TYPE_క్షేత్రం_LEVEL_NAME = (1 << 3),
};

రకనిర్వచనం నిర్మాణం {
    గణన type_క్షేత్రం defined;
    గణన విలీనం_బహుళకం విలీనం;

    xkb_atom_t name;
    xkb_mod_ముసుగు_t mods;
    xkb_level_సూచీ_t అంకె_స్థాయిలు;
    darray(నిర్మాణం xkb_కీలకం_type_నమోదు) entries;
    darray(xkb_atom_t) level_పేర్లు;
} కీలకం_Type_Info;

రకనిర్వచనం నిర్మాణం {
    అక్షరం *name;
    పూర్ణాంకం error_లెక్క;
    గుర్తులేని పూర్ణాంకం include_లోతు;

    darray(కీలకం_Type_Info) types;
    నిర్మాణం xkb_mod_సమితి mods;

    నిర్మాణం xkb_context *ctx;
    స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info;
} కీలకం_Types_Info;

/***====================================================================***/

స్థిర అంతఃవరుస స్థిరరాశి అక్షరం *
Map_నమోదు_Txt(కీలకం_Types_Info *info, నిర్మాణం xkb_కీలకం_type_నమోదు *నమోదు_2)
{
    తిరిగి Mod_ముసుగు_పాఠ్యం(info->ctx, MOD_BOTH, &info->mods, నమోదు_2->mods.mods);
}

స్థిర అంతఃవరుస స్థిరరాశి అక్షరం *
TypeTxt(కీలకం_Types_Info *info, కీలకం_Type_Info *type)
{
    తిరిగి xkb_atom_పాఠ్యం(info->ctx, type->name);
}

స్థిర అంతఃవరుస స్థిరరాశి అక్షరం *
Type_ముసుగు_Txt(కీలకం_Types_Info *info, కీలకం_Type_Info *type)
{
    తిరిగి Mod_ముసుగు_పాఠ్యం(info->ctx, MOD_BOTH, &info->mods, type->mods);
}

స్థిర అంతఃవరుస bool
ReportTypeShouldBeArray(కీలకం_Types_Info *info, కీలకం_Type_Info *type,
                        స్థిరరాశి అక్షరం *క్షేత్రం_2)
{
    తిరిగి ReportShouldBeArray(info->ctx, "key type", క్షేత్రం_2,
                               TypeTxt(info, type));
}

స్థిర అంతఃవరుస bool
ReportTypeBadType(కీలకం_Types_Info *info, గణన xkb_సందేశం_సంకేతం సంకేతం_2,
                  కీలకం_Type_Info *type, స్థిరరాశి అక్షరం *క్షేత్రం_2, స్థిరరాశి అక్షరం *wanted)
{
    తిరిగి ReportBadType(info->ctx, సంకేతం_2, "key type", క్షేత్రం_2,
                         TypeTxt(info, type), wanted);
}

/***====================================================================***/

స్థిర శూన్య
Init_కీలకం_Types_Info(కీలకం_Types_Info *info, స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
                 గుర్తులేని పూర్ణాంకం include_లోతు,
                 స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods)
{
    memset(info, 0, పరిమాణం(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_లోతు = include_లోతు;
    InitVMods(&info->mods, mods, include_లోతు > 0);
}

స్థిర శూన్య
Clear_కీలకం_Type_Info(కీలకం_Type_Info *type)
{
    darray_free(type->entries);
    darray_free(type->level_పేర్లు);
}

స్థిర శూన్య
Clear_కీలకం_Types_Info(కీలకం_Types_Info *info)
{
    విడుదల(info->name);
    కీలకం_Type_Info *type;
    darray_foreach(type, info->types)
        Clear_కీలకం_Type_Info(type);
    darray_free(info->types);
}

స్థిర కీలకం_Type_Info *
కనుగొను_Matching_కీలకం_Type(కీలకం_Types_Info *info, xkb_atom_t name)
{
    కీలకం_Type_Info *old;

    darray_foreach(old, info->types)
        ఒకవేళ (old->name == name)
            తిరిగి old;

    తిరిగి ఖాళీ;
}

స్థిర bool
కూడు_కీలకం_Type(కీలకం_Types_Info *info, కీలకం_Type_Info *new, bool same_file)
{
    కీలకం_Type_Info *old;
    స్థిరరాశి పూర్ణాంకం verbosity = xkb_context_get_log_verbosity(info->ctx);

    old = కనుగొను_Matching_కీలకం_Type(info, new->name);
    ఒకవేళ (old) {
        ఒకవేళ (new->విలీనం != విలీనం_AUGMENT) {
            ఒకవేళ ((same_file && verbosity > 0) || verbosity > 9) {
                log_warn(info->ctx,
                         XKB_WARNING_CONFLICTING_కీలకం_TYPE_DEFINITIONS,
                         "Multiple definitions of the %s key type; "
                         "Earlier definition ignored\n",
                         xkb_atom_పాఠ్యం(info->ctx, new->name));
            }

            Clear_కీలకం_Type_Info(old);
            *old = *new;
            darray_init(new->entries);
            darray_init(new->level_పేర్లు);
            తిరిగి true;
        }

        ఒకవేళ (same_file)
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_CONFLICTING_కీలకం_TYPE_DEFINITIONS,
                    "Multiple definitions of the %s key type; "
                    "Later definition ignored\n",
                    xkb_atom_పాఠ్యం(info->ctx, new->name));

        Clear_కీలకం_Type_Info(new);
        తిరిగి true;
    }

    darray_append(info->types, *new);
    తిరిగి true;
}

/***====================================================================***/

స్థిర శూన్య
విలీనం_Included_కీలకం_Types(కీలకం_Types_Info *into, కీలకం_Types_Info *from,
                      గణన విలీనం_బహుళకం విలీనం)
{
    ఒకవేళ (from->error_లెక్క > 0) {
        into->error_లెక్క += from->error_లెక్క;
        తిరిగి;
    }

    విలీనం_Mod_Sets(into->ctx, &into->mods, &from->mods, విలీనం);

    ఒకవేళ (into->name == ఖాళీ) {
        into->name = steal(&from->name);
    }

    ఒకవేళ (darray_ఖాళీలు(into->types)) {
        into->types = from->types;
        /* Types stolen via shallow copy, so reinitialize the array */
        darray_init(from->types);
    }
    లేకపోతే {
        కీలకం_Type_Info *type;
        darray_foreach(type, from->types) {
            type->విలీనం = విలీనం;
            ఒకవేళ (!కూడు_కీలకం_Type(into, type, false))
                into->error_లెక్క++;
        }
        /* Types were either shallow copied or reinitialized individually
           in `AddKeyType`, so we only need to free the array */
        darray_free(from->types);
    }
}

స్థిర శూన్య
Handle_కీలకం_Types_File(కీలకం_Types_Info *info, XkbFile *file);

స్థిర bool
Handle_Include_కీలకం_Types(కీలకం_Types_Info *info, IncludeStmt *include)
{
    కీలకం_Types_Info included;

    ఒకవేళ (Exceeds_Include_Max_లోతు(info->ctx, info->include_లోతు)) {
        info->error_లెక్క += 10;
        తిరిగి false;
    }

    Init_కీలకం_Types_Info(&included, info->keymap_info, info->include_లోతు + 1,
                     &info->mods);
    included.name = steal(&include->stmt);

    స్థిరరాశి నిర్మాణం parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    కొరకు (IncludeStmt *stmt = include; stmt; stmt = stmt->తదుపరి_incl) {
        కీలకం_Types_Info తదుపరి_incl;
        XkbFile *file;

        అక్షరం మార్గం[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_TYPES,
                                  మార్గం, పరిమాణం(మార్గం));
        ఒకవేళ (!file) {
            info->error_లెక్క += 10;
            Clear_కీలకం_Types_Info(&included);
            తిరిగి false;
        }

        Init_కీలకం_Types_Info(&తదుపరి_incl, info->keymap_info, info->include_లోతు + 1,
                         &included.mods);

        Handle_కీలకం_Types_File(&తదుపరి_incl, file);

        విలీనం_Included_కీలకం_Types(&included, &తదుపరి_incl, stmt->విలీనం);

        Clear_కీలకం_Types_Info(&తదుపరి_incl);
        FreeXkbFile(file);
    }

    విలీనం_Included_కీలకం_Types(info, &included, include->విలీనం);
    Clear_కీలకం_Types_Info(&included);

    తిరిగి (info->error_లెక్క == 0);
}

/***====================================================================***/

స్థిర bool
సమితి_Modifiers(కీలకం_Types_Info *info, కీలకం_Type_Info *type, సమాసం_Def *arrayNdx,
             సమాసం_Def *విలువ)
{
    xkb_mod_ముసుగు_t mods = 0;

    ఒకవేళ (arrayNdx) {
        log_err(info->ctx, XKB_LOG_సందేశం_NO_ID,
                "The modifiers field of a key type is not an array; "
                "Illegal array subscript ignored\n");
        తిరిగి false;
    }

    ఒకవేళ (!సమాసం_Resolve_Mod_ముసుగు(info->ctx, విలువ, MOD_BOTH, &info->mods, &mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ముసుగు,
                "Key type mask field must be a modifier mask; "
                "Key type definition ignored\n");
        తిరిగి false;
    }

    ఒకవేళ (type->defined & TYPE_క్షేత్రం_ముసుగు) {
        log_warn(info->ctx, XKB_LOG_సందేశం_NO_ID,
                 "Multiple modifier mask definitions for key type %s; "
                 "Using %s, ignoring %s\n",
                 xkb_atom_పాఠ్యం(info->ctx, type->name),
                 Type_ముసుగు_Txt(info, type),
                 Mod_ముసుగు_పాఠ్యం(info->ctx, MOD_BOTH, &info->mods, mods));
        తిరిగి false;
    }

    type->mods = mods;
    తిరిగి true;
}

/***====================================================================***/

స్థిర నిర్మాణం xkb_కీలకం_type_నమోదు *
కనుగొను_Matching_Map_నమోదు(కీలకం_Type_Info *type, xkb_mod_ముసుగు_t mods)
{
    నిర్మాణం xkb_కీలకం_type_నమోదు *నమోదు_2;

    darray_foreach(నమోదు_2, type->entries)
        ఒకవేళ (నమోదు_2->mods.mods == mods)
            తిరిగి నమోదు_2;

    తిరిగి ఖాళీ;
}

స్థిర bool
కూడు_Map_నమోదు(కీలకం_Types_Info *info, కీలకం_Type_Info *type,
            నిర్మాణం xkb_కీలకం_type_నమోదు *new, bool clobber, bool report)
{
    నిర్మాణం xkb_కీలకం_type_నమోదు *old;

    old = కనుగొను_Matching_Map_నమోదు(type, new->mods.mods);
    ఒకవేళ (old) {
        ఒకవేళ (report && old->level != new->level) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_కీలకం_TYPE_MAP_నమోదు,
                     "Multiple map entries for %s in %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     Map_నమోదు_Txt(info, new), TypeTxt(info, type),
                     (clobber ? new->level : old->level) + 1,
                     (clobber ? old->level : new->level) + 1);
        }
        లేకపోతే {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_CONFLICTING_కీలకం_TYPE_MAP_నమోదు,
                    "Multiple occurrences of map[%s]= %"PRIu32" in %s; Ignored\n",
                    Map_నమోదు_Txt(info, new), new->level + 1,
                    TypeTxt(info, type));
            తిరిగి true;
        }

        ఒకవేళ (clobber) {
            ఒకవేళ (new->level >= type->అంకె_స్థాయిలు)
                type->అంకె_స్థాయిలు = new->level + 1;
            old->level = new->level;
        }

        తిరిగి true;
    }

    ఒకవేళ (new->level >= type->అంకె_స్థాయిలు)
        type->అంకె_స్థాయిలు = new->level + 1;

    darray_append(type->entries, *new);
    తిరిగి true;
}

స్థిర bool
సమితి_Map_నమోదు(కీలకం_Types_Info *info, కీలకం_Type_Info *type, సమాసం_Def *arrayNdx,
            సమాసం_Def *విలువ)
{
    నిర్మాణం xkb_కీలకం_type_నమోదు నమోదు_2;

    ఒకవేళ (arrayNdx == ఖాళీ)
        తిరిగి ReportTypeShouldBeArray(info, type, "map entry");

    ఒకవేళ (!సమాసం_Resolve_Mod_ముసుగు(info->ctx, arrayNdx, MOD_BOTH, &info->mods,
                            &నమోదు_2.mods.mods))
        తిరిగి ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_ముసుగు_2,
                                 type, "map entry", "modifier mask");

    ఒకవేళ (నమోదు_2.mods.mods & (~type->mods)) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_లోపల_కీలకం_TYPE,
                "Map entry for modifiers not used by type %s; "
                "Using %s instead of %s\n",
                TypeTxt(info, type),
                Mod_ముసుగు_పాఠ్యం(info->ctx, MOD_BOTH, &info->mods,
                            నమోదు_2.mods.mods & type->mods),
                Map_నమోదు_Txt(info, &నమోదు_2));
        నమోదు_2.mods.mods &= type->mods;
    }

    ఒకవేళ (!సమాసం_Resolve_Level(info->ctx, విలువ, &నమోదు_2.level)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_జరుపు_LEVEL,
                          "Level specifications in a key type must be integer; "
                          "Ignoring malformed level specification\n");
        తిరిగి false;
    }

    నమోదు_2.preserve.mods = 0;

    తిరిగి కూడు_Map_నమోదు(info, type, &నమోదు_2, true, true);
}

/***====================================================================***/

స్థిర bool
కూడు_Preserve(కీలకం_Types_Info *info, కీలకం_Type_Info *type,
            xkb_mod_ముసుగు_t mods, xkb_mod_ముసుగు_t preserve_mods)
{
    నిర్మాణం xkb_కీలకం_type_నమోదు *నమోదు_2;

    darray_foreach(నమోదు_2, type->entries) {
        ఒకవేళ (నమోదు_2->mods.mods != mods)
            కొనసాగు;

        /* Map exists without previous preserve (or "None"); override. */
        ఒకవేళ (నమోదు_2->preserve.mods == 0) {
            నమోదు_2->preserve.mods = preserve_mods;
            తిరిగి true;
        }

        /* Map exists with same preserve; do nothing. */
        ఒకవేళ (నమోదు_2->preserve.mods == preserve_mods) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_DUPLICATE_నమోదు,
                    "Identical definitions for preserve[%s] in %s; "
                    "Ignored\n",
                    Mod_ముసుగు_పాఠ్యం(info->ctx, MOD_BOTH, &info->mods, mods),
                    TypeTxt(info, type));
            తిరిగి true;
        }

        /* Map exists with different preserve; latter wins. */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_కీలకం_TYPE_PRESERVE_ENTRIES,
                "Multiple definitions for preserve[%s] in %s; "
                "Using %s, ignoring %s\n",
                Mod_ముసుగు_పాఠ్యం(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type),
                Mod_ముసుగు_పాఠ్యం(info->ctx, MOD_BOTH, &info->mods, preserve_mods),
                Mod_ముసుగు_పాఠ్యం(info->ctx, MOD_BOTH, &info->mods,
                            నమోదు_2->preserve.mods));

        నమోదు_2->preserve.mods = preserve_mods;
        తిరిగి true;
    }

    /*
     * Map does not exist, i.e. preserve[] came before map[].
     * Create a map with the specified mask mapping to Level1. The level
     * may be overridden later with an explicit map[] statement.
     */
    స్థిరరాశి నిర్మాణం xkb_కీలకం_type_నమోదు new = {
        .level = 0,
        .mods = { .mods = mods },
        .preserve = { .mods = preserve_mods },
    };
    darray_append(type->entries, new);
    తిరిగి true;
}

స్థిర bool
సమితి_Preserve(కీలకం_Types_Info *info, కీలకం_Type_Info *type, సమాసం_Def *arrayNdx,
            సమాసం_Def *విలువ)
{

    ఒకవేళ (arrayNdx == ఖాళీ)
        తిరిగి ReportTypeShouldBeArray(info, type, "preserve entry");

    xkb_mod_ముసుగు_t mods = 0;
    ఒకవేళ (!సమాసం_Resolve_Mod_ముసుగు(info->ctx, arrayNdx, MOD_BOTH, &info->mods, &mods))
        తిరిగి ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_ముసుగు_2,
                                 type, "preserve entry", "modifier mask");

    ఒకవేళ (mods & ~type->mods) {
        స్థిరరాశి అక్షరం *before, *after;

        before = Mod_ముసుగు_పాఠ్యం(info->ctx, MOD_BOTH, &info->mods, mods);
        mods &= type->mods;
        after = Mod_ముసుగు_పాఠ్యం(info->ctx, MOD_BOTH, &info->mods, mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_లోపల_కీలకం_TYPE,
                "Preserve entry for modifiers not used by the %s type; "
                "Index %s converted to %s\n",
                TypeTxt(info, type), before, after);
    }

    xkb_mod_ముసుగు_t preserve_mods = 0;
    ఒకవేళ (!సమాసం_Resolve_Mod_ముసుగు(info->ctx, విలువ, MOD_BOTH, &info->mods,
                            &preserve_mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ముసుగు,
                "Preserve value in a key type is not a modifier mask; "
                "Ignoring preserve[%s] in type %s\n",
                Mod_ముసుగు_పాఠ్యం(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type));
        తిరిగి false;
    }

    ఒకవేళ (preserve_mods & ~mods) {
        స్థిరరాశి అక్షరం *before, *after;

        before = Mod_ముసుగు_పాఠ్యం(info->ctx, MOD_BOTH, &info->mods, preserve_mods);
        preserve_mods &= mods;
        after = Mod_ముసుగు_పాఠ్యం(info->ctx, MOD_BOTH, &info->mods, preserve_mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_ILLEGAL_కీలకం_TYPE_PRESERVE_ఫలితం,
                "Illegal value for preserve[%s] in type %s; "
                "Converted %s to %s\n",
                Mod_ముసుగు_పాఠ్యం(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type), before, after);
    }

    తిరిగి కూడు_Preserve(info, type, mods, preserve_mods);
}

/***====================================================================***/

స్థిర bool
కూడు_Level_Name(కీలకం_Types_Info *info, కీలకం_Type_Info *type,
             xkb_level_సూచీ_t level, xkb_atom_t name, bool clobber)
{
    /* New name. */
    ఒకవేళ (level >= darray_కొలత(type->level_పేర్లు)) {
        darray_resize0(type->level_పేర్లు, level + 1);
        వెళ్ళు ముగింపు;
    }

    /* Same level, same name. */
    ఒకవేళ (darray_అంశం(type->level_పేర్లు, level) == name) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE, XKB_WARNING_DUPLICATE_నమోదు,
                "Duplicate names for level %"PRIu32" of key type %s; Ignored\n",
                level + 1, TypeTxt(info, type));
        తిరిగి true;
    }

    /* Same level, different name. */
    ఒకవేళ (darray_అంశం(type->level_పేర్లు, level) != XKB_ATOM_NONE) {
        స్థిరరాశి అక్షరం *old, *new;
        old = xkb_atom_పాఠ్యం(info->ctx,
                            darray_అంశం(type->level_పేర్లు, level));
        new = xkb_atom_పాఠ్యం(info->ctx, name);
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_కీలకం_TYPE_LEVEL_పేర్లు,
                "Multiple names for level %"PRIu32" of key type %s; "
                "Using %s, ignoring %s\n",
                level + 1, TypeTxt(info, type),
                (clobber ? new : old), (clobber ? old : new));

        ఒకవేళ (!clobber)
            తిరిగి true;
    }

    /* FIXME: What about different level, same name? */

ముగింపు:
    darray_అంశం(type->level_పేర్లు, level) = name;
    తిరిగి true;
}

స్థిర bool
సమితి_Level_Name(కీలకం_Types_Info *info, కీలకం_Type_Info *type, సమాసం_Def *arrayNdx,
             సమాసం_Def *విలువ)
{
    ఒకవేళ (arrayNdx == ఖాళీ)
        తిరిగి ReportTypeShouldBeArray(info, type, "level name");

    xkb_level_సూచీ_t level = 0;
    ఒకవేళ (!సమాసం_Resolve_Level(info->ctx, arrayNdx, &level))
        తిరిగి ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_జరుపు_LEVEL,
                                 type, "level name", "integer");

    xkb_atom_t level_name = XKB_ATOM_NONE;
    ఒకవేళ (!సమాసం_Resolve_String(info->ctx, విలువ, &level_name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE,
                "Non-string name for level %"PRIu32" in key type %s; "
                "Ignoring illegal level name definition\n",
                level + 1, xkb_atom_పాఠ్యం(info->ctx, type->name));
        తిరిగి false;
    }

    తిరిగి కూడు_Level_Name(info, type, level, level_name, true);
}

/***====================================================================***/

స్థిర bool
సమితి_కీలకం_Type_క్షేత్రం(కీలకం_Types_Info *info, కీలకం_Type_Info *type,
                స్థిరరాశి అక్షరం *క్షేత్రం_2, సమాసం_Def *arrayNdx, సమాసం_Def *విలువ)
{
    bool ok = false;
    గణన type_క్షేత్రం type_క్షేత్రం = 0;

    ఒకవేళ (istreq(క్షేత్రం_2, "modifiers")) {
        type_క్షేత్రం = TYPE_క్షేత్రం_ముసుగు;
        ok = సమితి_Modifiers(info, type, arrayNdx, విలువ);
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "map")) {
        type_క్షేత్రం = TYPE_క్షేత్రం_MAP;
        ok = సమితి_Map_నమోదు(info, type, arrayNdx, విలువ);
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "preserve")) {
        type_క్షేత్రం = TYPE_క్షేత్రం_PRESERVE;
        ok = సమితి_Preserve(info, type, arrayNdx, విలువ);
    }
    లేకపోతే ఒకవేళ (istreq(క్షేత్రం_2, "levelname") || istreq(క్షేత్రం_2, "level_name")) {
        type_క్షేత్రం = TYPE_క్షేత్రం_LEVEL_NAME;
        ok = సమితి_Level_Name(info, type, arrayNdx, విలువ);
    } లేకపోతే {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_క్షేత్రం,
                "Unknown field \"%s\" in key type \"%s\"; Definition ignored\n",
                క్షేత్రం_2, TypeTxt(info, type));
        ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_TYPE_FIELDS);
    }

    type->defined |= type_క్షేత్రం;
    తిరిగి ok;
}

స్థిర bool
Handle_కీలకం_Type_Body(కీలకం_Types_Info *info, VarDef *def, కీలకం_Type_Info *type)
{
    bool ok = true;
    స్థిరరాశి అక్షరం *elem, *క్షేత్రం_2;
    సమాసం_Def *arrayNdx;

    కొరకు (; def; def = (VarDef *) def->common.తదుపరి) {
        ఒకవేళ (!సమాసం_Resolve_Lhs(info->ctx, def->name, &elem, &క్షేత్రం_2,
                            &arrayNdx)) {
            /* internal error, already reported */
            ok = false;
            కొనసాగు;
        }

        ఒకవేళ (elem) {
            ఒకవేళ (istreq(elem, "type")) {
                log_err(info->ctx, XKB_ERROR_INVALID_సమితి_DEFAULT_STATEMENT,
                        "Support for changing the default type has been removed; "
                        "Statement \"%s.%s\" ignored.\n", elem, క్షేత్రం_2);
            }
            లేకపోతే {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key type statement: move statements to the global "
                        "file scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, క్షేత్రం_2);
                ok = false;
            }
            కొనసాగు;
        }

        ఒకవేళ (!సమితి_కీలకం_Type_క్షేత్రం(info, type, క్షేత్రం_2, arrayNdx, def->విలువ))
            ok = false;
    }

    తిరిగి ok;
}

స్థిర bool
Handle_కీలకం_Type_Def(కీలకం_Types_Info *info, కీలకం_Type_Def *def)
{
    కీలకం_Type_Info type = {
        .defined = 0,
        .విలీనం = def->విలీనం,
        .name = def->name,
        .mods = 0,
        .అంకె_స్థాయిలు = 1,
        .entries = darray_new(),
        .level_పేర్లు = darray_new(),
    };

    ఒకవేళ (!Handle_కీలకం_Type_Body(info, def->body, &type) ||
        !కూడు_కీలకం_Type(info, &type, true))
    {
        info->error_లెక్క++;
        Clear_కీలకం_Type_Info(&type);
        తిరిగి false;
    }

    /* Type has been either stolen via shallow copy or reinitialized in
       `AddKeyType`: no need to free the arrays */
    తిరిగి true;
}

స్థిర bool
HandleGlobalVar(కీలకం_Types_Info *info, VarDef *stmt)
{
    స్థిరరాశి అక్షరం *elem, *క్షేత్రం_2;
    సమాసం_Def *arrayNdx;

    ఒకవేళ (!సమాసం_Resolve_Lhs(info->ctx, stmt->name, &elem, &క్షేత్రం_2,
                        &arrayNdx)) {
        తిరిగి false;           /* internal error, already reported */
    }
    లేకపోతే ఒకవేళ (elem && istreq(elem, "type")) {
        log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                "Support for changing the default type has been removed; "
                "Statement ignored\n");
        తిరిగి true;
    }
    లేకపోతే ఒకవేళ (elem) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_క్షేత్రం,
                "Default defined for unknown element \"%s\"; "
                "Value for field \"%s.%s\" ignored\n", elem, elem, క్షేత్రం_2);
        తిరిగి !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
    }
    లేకపోతే ఒకవేళ (క్షేత్రం_2) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_క్షేత్రం,
                "Default defined for unknown field \"%s\"; Ignored\n", క్షేత్రం_2);
        తిరిగి !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS);
    }
    తిరిగి false;
}

స్థిర శూన్య
Handle_కీలకం_Types_File(కీలకం_Types_Info *info, XkbFile *file)
{
    bool ok;

    విడుదల(info->name);
    info->name = strdup_సురక్షిత(file->name);

    కొరకు (ParseCommon *stmt = file->defs; stmt; stmt = stmt->తదుపరి) {
        ఎంపిక (stmt->type) {
        స్థితి STMT_INCLUDE:
            ok = Handle_Include_కీలకం_Types(info, (IncludeStmt *) stmt);
            ఆపు;
        స్థితి STMT_TYPE:
            ok = Handle_కీలకం_Type_Def(info, (కీలకం_Type_Def *) stmt);
            ఆపు;
        స్థితి STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef*) stmt);
            ఆపు;
        స్థితి STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            ఆపు;
        స్థితి STMT_UNKNOWN_DECLARATION:
        స్థితి STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported types %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ఆపు;
        అప్రమేయ:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Key type files may not include other declarations; "
                    "Ignoring %s\n", stmt_type_గమ్యం_string(stmt->type));
            ok = false;
            ఆపు;
        }

        ఒకవేళ (!ok)
            info->error_లెక్క++;

        ఒకవేళ (info->error_లెక్క > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning keytypes file \"%s\"\n",
                    సురక్షిత_map_name(file->name));
            ఆపు;
        }
    }
}

/***====================================================================***/

స్థిర bool
Copy_కీలకం_Types_గమ్యం_Keymap(నిర్మాణం xkb_keymap *keymap, కీలకం_Types_Info *info)
{
    స్థిరరాశి darray_కొలత_t అంకె_types = darray_ఖాళీలు(info->types)
                                  ? 1
                                  : darray_కొలత(info->types);
    నిర్మాణం xkb_కీలకం_type *types = సున్నా_జ్ఞాపకం_ఇవ్వు(అంకె_types, పరిమాణం(*types));
    ఒకవేళ (!types)
        తిరిగి false;

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
    ఒకవేళ (darray_ఖాళీలు(info->types)) {
        నిర్మాణం xkb_కీలకం_type *type = &types[0];

        type->mods.mods = 0;
        type->అంకె_స్థాయిలు = 1;
        type->entries = ఖాళీ;
        type->అంకె_entries = 0;
        type->name = xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL");
        type->level_పేర్లు = ఖాళీ;
        type->అంకె_level_పేర్లు = 0;
        type->required = true;
    }
    లేకపోతే {
        /* HACK to circumvent Xorg bugs (see comment above) */
        స్థిరరాశి xkb_atom_t canonical_types[] = {
            xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "TWO_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "ALPHABETIC"),
            xkb_atom_intern_literal(keymap->ctx, "KEYPAD"),
        };

        కొరకు (darray_కొలత_t i = 0; i < అంకె_types; i++) {
            కీలకం_Type_Info *def = &darray_అంశం(info->types, i);
            నిర్మాణం xkb_కీలకం_type *type = &types[i];

            type->name = def->name;
            type->mods.mods = def->mods;
            type->అంకె_స్థాయిలు = def->అంకె_స్థాయిలు;
            type->అంకె_level_పేర్లు =
                (xkb_level_సూచీ_t) darray_కొలత(def->level_పేర్లు);
            darray_steal(def->level_పేర్లు, &type->level_పేర్లు, ఖాళీ);
            darray_steal(def->entries, &type->entries, &type->అంకె_entries);
            type->required = false;

            /* HACK: Never drop canonical XKB key types (see comment above) */
            ఒకవేళ (type->అంకె_స్థాయిలు <= 2) {
                కొరకు (uint8_t t = 0;
                     t < (uint8_t) ARRAY_కొలత(canonical_types);
                     t++) {
                        ఒకవేళ (type->name == canonical_types[t]) {
                            type->required = true;
                            ఆపు;
                        }
                }
            }
        }
    }

    keymap->types_section_name = strdup_సురక్షిత(info->name);
    XkbEscapeMapName(keymap->types_section_name);
    keymap->అంకె_types = అంకె_types;
    keymap->types = types;
    keymap->mods = info->mods;
    తిరిగి true;
}

/***====================================================================***/

bool
Compile_కీలకం_Types(XkbFile *file, నిర్మాణం xkb_keymap_info *keymap_info)
{
    కీలకం_Types_Info info;

    Init_కీలకం_Types_Info(&info, keymap_info, 0,
                     &keymap_info->keymap.mods);

    ఒకవేళ (file != ఖాళీ)
        Handle_కీలకం_Types_File(&info, file);

    ఒకవేళ (info.error_లెక్క != 0)
        వెళ్ళు err_info;

    ఒకవేళ (!Copy_కీలకం_Types_గమ్యం_Keymap(&keymap_info->keymap, &info))
        వెళ్ళు err_info;

    Clear_కీలకం_Types_Info(&info);
    తిరిగి true;

err_info:
    Clear_కీలకం_Types_Info(&info);
    తిరిగి false;
}
