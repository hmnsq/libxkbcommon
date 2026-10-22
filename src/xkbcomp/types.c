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

ಎಣಿಕೆ type_ಕ್ಷೇತ್ರ {
    TYPE_ಕ್ಷೇತ್ರ_ಮುಖವಾಡ = (1 << 0),
    TYPE_ಕ್ಷೇತ್ರ_MAP = (1 << 1),
    TYPE_ಕ್ಷೇತ್ರ_PRESERVE = (1 << 2),
    TYPE_ಕ್ಷೇತ್ರ_LEVEL_NAME = (1 << 3),
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಎಣಿಕೆ type_ಕ್ಷೇತ್ರ defined;
    ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ;

    xkb_atom_t name;
    xkb_mod_ಮುಖವಾಡ_t mods;
    xkb_level_ಸೂಚ್ಯಂಕ_t ಅಂಕೆ_ಮಟ್ಟಗಳು;
    darray(ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು) entries;
    darray(xkb_atom_t) level_ಹೆಸರುಗಳು;
} ಕೀಲಿ_Type_Info;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಅಕ್ಷರ *name;
    ಪೂರ್ಣಾಂಕ error_ಎಣಿಕೆ;
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ include_ಆಳ;

    darray(ಕೀಲಿ_Type_Info) types;
    ರಚನೆ xkb_mod_ಗಣ mods;

    ರಚನೆ xkb_context *ctx;
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info;
} ಕೀಲಿ_Types_Info;

/***====================================================================***/

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
Map_ನಮೂದು_Txt(ಕೀಲಿ_Types_Info *info, ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು *ನಮೂದು)
{
    ಹಿಂತಿರುಗಿ Mod_ಮುಖವಾಡ_ಪಠ್ಯ(info->ctx, MOD_BOTH, &info->mods, ನಮೂದು->mods.mods);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
TypeTxt(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *type)
{
    ಹಿಂತಿರುಗಿ xkb_atom_ಪಠ್ಯ(info->ctx, type->name);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
Type_ಮುಖವಾಡ_Txt(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *type)
{
    ಹಿಂತಿರುಗಿ Mod_ಮುಖವಾಡ_ಪಠ್ಯ(info->ctx, MOD_BOTH, &info->mods, type->mods);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ReportTypeShouldBeArray(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *type,
                        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ_2)
{
    ಹಿಂತಿರುಗಿ ReportShouldBeArray(info->ctx, "key type", ಕ್ಷೇತ್ರ_2,
                               TypeTxt(info, type));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ReportTypeBadType(ಕೀಲಿ_Types_Info *info, ಎಣಿಕೆ xkb_ಸಂದೇಶ_ಸಂಕೇತ ಸಂಕೇತ_2,
                  ಕೀಲಿ_Type_Info *type, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *wanted)
{
    ಹಿಂತಿರುಗಿ ReportBadType(info->ctx, ಸಂಕೇತ_2, "key type", ಕ್ಷೇತ್ರ_2,
                         TypeTxt(info, type), wanted);
}

/***====================================================================***/

ಸ್ಥಿರ ಶೂನ್ಯ
Init_ಕೀಲಿ_Types_Info(ಕೀಲಿ_Types_Info *info, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
                 ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ include_ಆಳ,
                 ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods)
{
    memset(info, 0, ಗಾತ್ರ(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_ಆಳ = include_ಆಳ;
    InitVMods(&info->mods, mods, include_ಆಳ > 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ
Clear_ಕೀಲಿ_Type_Info(ಕೀಲಿ_Type_Info *type)
{
    darray_free(type->entries);
    darray_free(type->level_ಹೆಸರುಗಳು);
}

ಸ್ಥಿರ ಶೂನ್ಯ
Clear_ಕೀಲಿ_Types_Info(ಕೀಲಿ_Types_Info *info)
{
    ಬಿಡುಗಡೆ(info->name);
    ಕೀಲಿ_Type_Info *type;
    darray_foreach(type, info->types)
        Clear_ಕೀಲಿ_Type_Info(type);
    darray_free(info->types);
}

ಸ್ಥಿರ ಕೀಲಿ_Type_Info *
ಕಂಡುಹಿಡಿ_Matching_ಕೀಲಿ_Type(ಕೀಲಿ_Types_Info *info, xkb_atom_t name)
{
    ಕೀಲಿ_Type_Info *old;

    darray_foreach(old, info->types)
        ಆದರೆ (old->name == name)
            ಹಿಂತಿರುಗಿ old;

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ bool
ಕೂಡಿಸು_ಕೀಲಿ_Type(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *new, bool same_file)
{
    ಕೀಲಿ_Type_Info *old;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ verbosity = xkb_context_get_log_verbosity(info->ctx);

    old = ಕಂಡುಹಿಡಿ_Matching_ಕೀಲಿ_Type(info, new->name);
    ಆದರೆ (old) {
        ಆದರೆ (new->ವಿಲೀನ != ವಿಲೀನ_AUGMENT) {
            ಆದರೆ ((same_file && verbosity > 0) || verbosity > 9) {
                log_warn(info->ctx,
                         XKB_WARNING_CONFLICTING_ಕೀಲಿ_TYPE_DEFINITIONS,
                         "Multiple definitions of the %s key type; "
                         "Earlier definition ignored\n",
                         xkb_atom_ಪಠ್ಯ(info->ctx, new->name));
            }

            Clear_ಕೀಲಿ_Type_Info(old);
            *old = *new;
            darray_init(new->entries);
            darray_init(new->level_ಹೆಸರುಗಳು);
            ಹಿಂತಿರುಗಿ true;
        }

        ಆದರೆ (same_file)
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_CONFLICTING_ಕೀಲಿ_TYPE_DEFINITIONS,
                    "Multiple definitions of the %s key type; "
                    "Later definition ignored\n",
                    xkb_atom_ಪಠ್ಯ(info->ctx, new->name));

        Clear_ಕೀಲಿ_Type_Info(new);
        ಹಿಂತಿರುಗಿ true;
    }

    darray_append(info->types, *new);
    ಹಿಂತಿರುಗಿ true;
}

/***====================================================================***/

ಸ್ಥಿರ ಶೂನ್ಯ
ವಿಲೀನ_Included_ಕೀಲಿ_Types(ಕೀಲಿ_Types_Info *into, ಕೀಲಿ_Types_Info *from,
                      ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ)
{
    ಆದರೆ (from->error_ಎಣಿಕೆ > 0) {
        into->error_ಎಣಿಕೆ += from->error_ಎಣಿಕೆ;
        ಹಿಂತಿರುಗಿ;
    }

    ವಿಲೀನ_Mod_Sets(into->ctx, &into->mods, &from->mods, ವಿಲೀನ);

    ಆದರೆ (into->name == ಖಾಲಿ) {
        into->name = steal(&from->name);
    }

    ಆದರೆ (darray_ಖಾಲಿಗಳು(into->types)) {
        into->types = from->types;
        /* Types stolen via shallow copy, so reinitialize the array */
        darray_init(from->types);
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        ಕೀಲಿ_Type_Info *type;
        darray_foreach(type, from->types) {
            type->ವಿಲೀನ = ವಿಲೀನ;
            ಆದರೆ (!ಕೂಡಿಸು_ಕೀಲಿ_Type(into, type, false))
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

    ಆದರೆ (Exceeds_Include_Max_ಆಳ(info->ctx, info->include_ಆಳ)) {
        info->error_ಎಣಿಕೆ += 10;
        ಹಿಂತಿರುಗಿ false;
    }

    Init_ಕೀಲಿ_Types_Info(&included, info->keymap_info, info->include_ಆಳ + 1,
                     &info->mods);
    included.name = steal(&include->stmt);

    ಸ್ಥಿರಾಂಕ ರಚನೆ parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    ಗಾಗಿ (IncludeStmt *stmt = include; stmt; stmt = stmt->ಮುಂದಿನ_incl) {
        ಕೀಲಿ_Types_Info ಮುಂದಿನ_incl;
        XkbFile *file;

        ಅಕ್ಷರ ಮಾರ್ಗ[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_TYPES,
                                  ಮಾರ್ಗ, ಗಾತ್ರ(ಮಾರ್ಗ));
        ಆದರೆ (!file) {
            info->error_ಎಣಿಕೆ += 10;
            Clear_ಕೀಲಿ_Types_Info(&included);
            ಹಿಂತಿರುಗಿ false;
        }

        Init_ಕೀಲಿ_Types_Info(&ಮುಂದಿನ_incl, info->keymap_info, info->include_ಆಳ + 1,
                         &included.mods);

        Handle_ಕೀಲಿ_Types_File(&ಮುಂದಿನ_incl, file);

        ವಿಲೀನ_Included_ಕೀಲಿ_Types(&included, &ಮುಂದಿನ_incl, stmt->ವಿಲೀನ);

        Clear_ಕೀಲಿ_Types_Info(&ಮುಂದಿನ_incl);
        FreeXkbFile(file);
    }

    ವಿಲೀನ_Included_ಕೀಲಿ_Types(info, &included, include->ವಿಲೀನ);
    Clear_ಕೀಲಿ_Types_Info(&included);

    ಹಿಂತಿರುಗಿ (info->error_ಎಣಿಕೆ == 0);
}

/***====================================================================***/

ಸ್ಥಿರ bool
ಗಣ_Modifiers(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *type, ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx,
             ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ)
{
    xkb_mod_ಮುಖವಾಡ_t mods = 0;

    ಆದರೆ (arrayNdx) {
        log_err(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "The modifiers field of a key type is not an array; "
                "Illegal array subscript ignored\n");
        ಹಿಂತಿರುಗಿ false;
    }

    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_ಮುಖವಾಡ(info->ctx, ಮೌಲ್ಯ, MOD_BOTH, &info->mods, &mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ಮುಖವಾಡ,
                "Key type mask field must be a modifier mask; "
                "Key type definition ignored\n");
        ಹಿಂತಿರುಗಿ false;
    }

    ಆದರೆ (type->defined & TYPE_ಕ್ಷೇತ್ರ_ಮುಖವಾಡ) {
        log_warn(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                 "Multiple modifier mask definitions for key type %s; "
                 "Using %s, ignoring %s\n",
                 xkb_atom_ಪಠ್ಯ(info->ctx, type->name),
                 Type_ಮುಖವಾಡ_Txt(info, type),
                 Mod_ಮುಖವಾಡ_ಪಠ್ಯ(info->ctx, MOD_BOTH, &info->mods, mods));
        ಹಿಂತಿರುಗಿ false;
    }

    type->mods = mods;
    ಹಿಂತಿರುಗಿ true;
}

/***====================================================================***/

ಸ್ಥಿರ ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು *
ಕಂಡುಹಿಡಿ_Matching_Map_ನಮೂದು(ಕೀಲಿ_Type_Info *type, xkb_mod_ಮುಖವಾಡ_t mods)
{
    ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು *ನಮೂದು;

    darray_foreach(ನಮೂದು, type->entries)
        ಆದರೆ (ನಮೂದು->mods.mods == mods)
            ಹಿಂತಿರುಗಿ ನಮೂದು;

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ bool
ಕೂಡಿಸು_Map_ನಮೂದು(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *type,
            ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು *new, bool clobber, bool report)
{
    ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು *old;

    old = ಕಂಡುಹಿಡಿ_Matching_Map_ನಮೂದು(type, new->mods.mods);
    ಆದರೆ (old) {
        ಆದರೆ (report && old->level != new->level) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_TYPE_MAP_ನಮೂದು,
                     "Multiple map entries for %s in %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     Map_ನಮೂದು_Txt(info, new), TypeTxt(info, type),
                     (clobber ? new->level : old->level) + 1,
                     (clobber ? old->level : new->level) + 1);
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_CONFLICTING_ಕೀಲಿ_TYPE_MAP_ನಮೂದು,
                    "Multiple occurrences of map[%s]= %"PRIu32" in %s; Ignored\n",
                    Map_ನಮೂದು_Txt(info, new), new->level + 1,
                    TypeTxt(info, type));
            ಹಿಂತಿರುಗಿ true;
        }

        ಆದರೆ (clobber) {
            ಆದರೆ (new->level >= type->ಅಂಕೆ_ಮಟ್ಟಗಳು)
                type->ಅಂಕೆ_ಮಟ್ಟಗಳು = new->level + 1;
            old->level = new->level;
        }

        ಹಿಂತಿರುಗಿ true;
    }

    ಆದರೆ (new->level >= type->ಅಂಕೆ_ಮಟ್ಟಗಳು)
        type->ಅಂಕೆ_ಮಟ್ಟಗಳು = new->level + 1;

    darray_append(type->entries, *new);
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
ಗಣ_Map_ನಮೂದು(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *type, ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx,
            ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ)
{
    ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು ನಮೂದು;

    ಆದರೆ (arrayNdx == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ ReportTypeShouldBeArray(info, type, "map entry");

    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_ಮುಖವಾಡ(info->ctx, arrayNdx, MOD_BOTH, &info->mods,
                            &ನಮೂದು.mods.mods))
        ಹಿಂತಿರುಗಿ ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_ಮುಖವಾಡ_2,
                                 type, "map entry", "modifier mask");

    ಆದರೆ (ನಮೂದು.mods.mods & (~type->mods)) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_ಒಳ_ಕೀಲಿ_TYPE,
                "Map entry for modifiers not used by type %s; "
                "Using %s instead of %s\n",
                TypeTxt(info, type),
                Mod_ಮುಖವಾಡ_ಪಠ್ಯ(info->ctx, MOD_BOTH, &info->mods,
                            ನಮೂದು.mods.mods & type->mods),
                Map_ನಮೂದು_Txt(info, &ನಮೂದು));
        ನಮೂದು.mods.mods &= type->mods;
    }

    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Level(info->ctx, ಮೌಲ್ಯ, &ನಮೂದು.level)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_ಸರಿಸು_LEVEL,
                          "Level specifications in a key type must be integer; "
                          "Ignoring malformed level specification\n");
        ಹಿಂತಿರುಗಿ false;
    }

    ನಮೂದು.preserve.mods = 0;

    ಹಿಂತಿರುಗಿ ಕೂಡಿಸು_Map_ನಮೂದು(info, type, &ನಮೂದು, true, true);
}

/***====================================================================***/

ಸ್ಥಿರ bool
ಕೂಡಿಸು_Preserve(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *type,
            xkb_mod_ಮುಖವಾಡ_t mods, xkb_mod_ಮುಖವಾಡ_t preserve_mods)
{
    ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು *ನಮೂದು;

    darray_foreach(ನಮೂದು, type->entries) {
        ಆದರೆ (ನಮೂದು->mods.mods != mods)
            ಮುಂದುವರಿ;

        /* Map exists without previous preserve (or "None"); override. */
        ಆದರೆ (ನಮೂದು->preserve.mods == 0) {
            ನಮೂದು->preserve.mods = preserve_mods;
            ಹಿಂತಿರುಗಿ true;
        }

        /* Map exists with same preserve; do nothing. */
        ಆದರೆ (ನಮೂದು->preserve.mods == preserve_mods) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_DUPLICATE_ನಮೂದು,
                    "Identical definitions for preserve[%s] in %s; "
                    "Ignored\n",
                    Mod_ಮುಖವಾಡ_ಪಠ್ಯ(info->ctx, MOD_BOTH, &info->mods, mods),
                    TypeTxt(info, type));
            ಹಿಂತಿರುಗಿ true;
        }

        /* Map exists with different preserve; latter wins. */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_ಕೀಲಿ_TYPE_PRESERVE_ENTRIES,
                "Multiple definitions for preserve[%s] in %s; "
                "Using %s, ignoring %s\n",
                Mod_ಮುಖವಾಡ_ಪಠ್ಯ(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type),
                Mod_ಮುಖವಾಡ_ಪಠ್ಯ(info->ctx, MOD_BOTH, &info->mods, preserve_mods),
                Mod_ಮುಖವಾಡ_ಪಠ್ಯ(info->ctx, MOD_BOTH, &info->mods,
                            ನಮೂದು->preserve.mods));

        ನಮೂದು->preserve.mods = preserve_mods;
        ಹಿಂತಿರುಗಿ true;
    }

    /*
     * Map does not exist, i.e. preserve[] came before map[].
     * Create a map with the specified mask mapping to Level1. The level
     * may be overridden later with an explicit map[] statement.
     */
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು new = {
        .level = 0,
        .mods = { .mods = mods },
        .preserve = { .mods = preserve_mods },
    };
    darray_append(type->entries, new);
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
ಗಣ_Preserve(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *type, ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx,
            ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ)
{

    ಆದರೆ (arrayNdx == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ ReportTypeShouldBeArray(info, type, "preserve entry");

    xkb_mod_ಮುಖವಾಡ_t mods = 0;
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_ಮುಖವಾಡ(info->ctx, arrayNdx, MOD_BOTH, &info->mods, &mods))
        ಹಿಂತಿರುಗಿ ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_ಮುಖವಾಡ_2,
                                 type, "preserve entry", "modifier mask");

    ಆದರೆ (mods & ~type->mods) {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *before, *after;

        before = Mod_ಮುಖವಾಡ_ಪಠ್ಯ(info->ctx, MOD_BOTH, &info->mods, mods);
        mods &= type->mods;
        after = Mod_ಮುಖವಾಡ_ಪಠ್ಯ(info->ctx, MOD_BOTH, &info->mods, mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_ಒಳ_ಕೀಲಿ_TYPE,
                "Preserve entry for modifiers not used by the %s type; "
                "Index %s converted to %s\n",
                TypeTxt(info, type), before, after);
    }

    xkb_mod_ಮುಖವಾಡ_t preserve_mods = 0;
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_ಮುಖವಾಡ(info->ctx, ಮೌಲ್ಯ, MOD_BOTH, &info->mods,
                            &preserve_mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ಮುಖವಾಡ,
                "Preserve value in a key type is not a modifier mask; "
                "Ignoring preserve[%s] in type %s\n",
                Mod_ಮುಖವಾಡ_ಪಠ್ಯ(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type));
        ಹಿಂತಿರುಗಿ false;
    }

    ಆದರೆ (preserve_mods & ~mods) {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *before, *after;

        before = Mod_ಮುಖವಾಡ_ಪಠ್ಯ(info->ctx, MOD_BOTH, &info->mods, preserve_mods);
        preserve_mods &= mods;
        after = Mod_ಮುಖವಾಡ_ಪಠ್ಯ(info->ctx, MOD_BOTH, &info->mods, preserve_mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_ILLEGAL_ಕೀಲಿ_TYPE_PRESERVE_ಫಲಿತಾಂಶ,
                "Illegal value for preserve[%s] in type %s; "
                "Converted %s to %s\n",
                Mod_ಮುಖವಾಡ_ಪಠ್ಯ(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type), before, after);
    }

    ಹಿಂತಿರುಗಿ ಕೂಡಿಸು_Preserve(info, type, mods, preserve_mods);
}

/***====================================================================***/

ಸ್ಥಿರ bool
ಕೂಡಿಸು_Level_Name(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *type,
             xkb_level_ಸೂಚ್ಯಂಕ_t level, xkb_atom_t name, bool clobber)
{
    /* New name. */
    ಆದರೆ (level >= darray_ಗಾತ್ರ(type->level_ಹೆಸರುಗಳು)) {
        darray_resize0(type->level_ಹೆಸರುಗಳು, level + 1);
        ಹೋಗು ಮುಕ್ತಾಯ;
    }

    /* Same level, same name. */
    ಆದರೆ (darray_ಅಂಶ(type->level_ಹೆಸರುಗಳು, level) == name) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE, XKB_WARNING_DUPLICATE_ನಮೂದು,
                "Duplicate names for level %"PRIu32" of key type %s; Ignored\n",
                level + 1, TypeTxt(info, type));
        ಹಿಂತಿರುಗಿ true;
    }

    /* Same level, different name. */
    ಆದರೆ (darray_ಅಂಶ(type->level_ಹೆಸರುಗಳು, level) != XKB_ATOM_NONE) {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *old, *new;
        old = xkb_atom_ಪಠ್ಯ(info->ctx,
                            darray_ಅಂಶ(type->level_ಹೆಸರುಗಳು, level));
        new = xkb_atom_ಪಠ್ಯ(info->ctx, name);
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_ಕೀಲಿ_TYPE_LEVEL_ಹೆಸರುಗಳು,
                "Multiple names for level %"PRIu32" of key type %s; "
                "Using %s, ignoring %s\n",
                level + 1, TypeTxt(info, type),
                (clobber ? new : old), (clobber ? old : new));

        ಆದರೆ (!clobber)
            ಹಿಂತಿರುಗಿ true;
    }

    /* FIXME: What about different level, same name? */

ಮುಕ್ತಾಯ:
    darray_ಅಂಶ(type->level_ಹೆಸರುಗಳು, level) = name;
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
ಗಣ_Level_Name(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *type, ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx,
             ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ)
{
    ಆದರೆ (arrayNdx == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ ReportTypeShouldBeArray(info, type, "level name");

    xkb_level_ಸೂಚ್ಯಂಕ_t level = 0;
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Level(info->ctx, arrayNdx, &level))
        ಹಿಂತಿರುಗಿ ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_ಸರಿಸು_LEVEL,
                                 type, "level name", "integer");

    xkb_atom_t level_name = XKB_ATOM_NONE;
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_String(info->ctx, ಮೌಲ್ಯ, &level_name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE,
                "Non-string name for level %"PRIu32" in key type %s; "
                "Ignoring illegal level name definition\n",
                level + 1, xkb_atom_ಪಠ್ಯ(info->ctx, type->name));
        ಹಿಂತಿರುಗಿ false;
    }

    ಹಿಂತಿರುಗಿ ಕೂಡಿಸು_Level_Name(info, type, level, level_name, true);
}

/***====================================================================***/

ಸ್ಥಿರ bool
ಗಣ_ಕೀಲಿ_Type_ಕ್ಷೇತ್ರ(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Info *type,
                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ_2, ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx, ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ)
{
    bool ok = false;
    ಎಣಿಕೆ type_ಕ್ಷೇತ್ರ type_ಕ್ಷೇತ್ರ = 0;

    ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "modifiers")) {
        type_ಕ್ಷೇತ್ರ = TYPE_ಕ್ಷೇತ್ರ_ಮುಖವಾಡ;
        ok = ಗಣ_Modifiers(info, type, arrayNdx, ಮೌಲ್ಯ);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "map")) {
        type_ಕ್ಷೇತ್ರ = TYPE_ಕ್ಷೇತ್ರ_MAP;
        ok = ಗಣ_Map_ನಮೂದು(info, type, arrayNdx, ಮೌಲ್ಯ);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "preserve")) {
        type_ಕ್ಷೇತ್ರ = TYPE_ಕ್ಷೇತ್ರ_PRESERVE;
        ok = ಗಣ_Preserve(info, type, arrayNdx, ಮೌಲ್ಯ);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "levelname") || istreq(ಕ್ಷೇತ್ರ_2, "level_name")) {
        type_ಕ್ಷೇತ್ರ = TYPE_ಕ್ಷೇತ್ರ_LEVEL_NAME;
        ok = ಗಣ_Level_Name(info, type, arrayNdx, ಮೌಲ್ಯ);
    } ಇಲ್ಲದಿದ್ದರೆ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_ಕ್ಷೇತ್ರ,
                "Unknown field \"%s\" in key type \"%s\"; Definition ignored\n",
                ಕ್ಷೇತ್ರ_2, TypeTxt(info, type));
        ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_TYPE_FIELDS);
    }

    type->defined |= type_ಕ್ಷೇತ್ರ;
    ಹಿಂತಿರುಗಿ ok;
}

ಸ್ಥಿರ bool
Handle_ಕೀಲಿ_Type_Body(ಕೀಲಿ_Types_Info *info, VarDef *def, ಕೀಲಿ_Type_Info *type)
{
    bool ok = true;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *elem, *ಕ್ಷೇತ್ರ_2;
    ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx;

    ಗಾಗಿ (; def; def = (VarDef *) def->common.ಮುಂದಿನ) {
        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Lhs(info->ctx, def->name, &elem, &ಕ್ಷೇತ್ರ_2,
                            &arrayNdx)) {
            /* internal error, already reported */
            ok = false;
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (elem) {
            ಆದರೆ (istreq(elem, "type")) {
                log_err(info->ctx, XKB_ERROR_INVALID_ಗಣ_DEFAULT_STATEMENT,
                        "Support for changing the default type has been removed; "
                        "Statement \"%s.%s\" ignored.\n", elem, ಕ್ಷೇತ್ರ_2);
            }
            ಇಲ್ಲದಿದ್ದರೆ {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key type statement: move statements to the global "
                        "file scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, ಕ್ಷೇತ್ರ_2);
                ok = false;
            }
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (!ಗಣ_ಕೀಲಿ_Type_ಕ್ಷೇತ್ರ(info, type, ಕ್ಷೇತ್ರ_2, arrayNdx, def->ಮೌಲ್ಯ))
            ok = false;
    }

    ಹಿಂತಿರುಗಿ ok;
}

ಸ್ಥಿರ bool
Handle_ಕೀಲಿ_Type_Def(ಕೀಲಿ_Types_Info *info, ಕೀಲಿ_Type_Def *def)
{
    ಕೀಲಿ_Type_Info type = {
        .defined = 0,
        .ವಿಲೀನ = def->ವಿಲೀನ,
        .name = def->name,
        .mods = 0,
        .ಅಂಕೆ_ಮಟ್ಟಗಳು = 1,
        .entries = darray_new(),
        .level_ಹೆಸರುಗಳು = darray_new(),
    };

    ಆದರೆ (!Handle_ಕೀಲಿ_Type_Body(info, def->body, &type) ||
        !ಕೂಡಿಸು_ಕೀಲಿ_Type(info, &type, true))
    {
        info->error_ಎಣಿಕೆ++;
        Clear_ಕೀಲಿ_Type_Info(&type);
        ಹಿಂತಿರುಗಿ false;
    }

    /* Type has been either stolen via shallow copy or reinitialized in
       `AddKeyType`: no need to free the arrays */
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
HandleGlobalVar(ಕೀಲಿ_Types_Info *info, VarDef *stmt)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *elem, *ಕ್ಷೇತ್ರ_2;
    ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx;

    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Lhs(info->ctx, stmt->name, &elem, &ಕ್ಷೇತ್ರ_2,
                        &arrayNdx)) {
        ಹಿಂತಿರುಗಿ false;           /* internal error, already reported */
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (elem && istreq(elem, "type")) {
        log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                "Support for changing the default type has been removed; "
                "Statement ignored\n");
        ಹಿಂತಿರುಗಿ true;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (elem) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_ಕ್ಷೇತ್ರ,
                "Default defined for unknown element \"%s\"; "
                "Value for field \"%s.%s\" ignored\n", elem, elem, ಕ್ಷೇತ್ರ_2);
        ಹಿಂತಿರುಗಿ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕ್ಷೇತ್ರ_2) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_ಕ್ಷೇತ್ರ,
                "Default defined for unknown field \"%s\"; Ignored\n", ಕ್ಷೇತ್ರ_2);
        ಹಿಂತಿರುಗಿ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS);
    }
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ
Handle_ಕೀಲಿ_Types_File(ಕೀಲಿ_Types_Info *info, XkbFile *file)
{
    bool ok;

    ಬಿಡುಗಡೆ(info->name);
    info->name = strdup_ಸುರಕ್ಷಿತ(file->name);

    ಗಾಗಿ (ParseCommon *stmt = file->defs; stmt; stmt = stmt->ಮುಂದಿನ) {
        ಆಯ್ಕೆ (stmt->type) {
        ಸ್ಥಿತಿ STMT_INCLUDE:
            ok = Handle_Include_ಕೀಲಿ_Types(info, (IncludeStmt *) stmt);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_TYPE:
            ok = Handle_ಕೀಲಿ_Type_Def(info, (ಕೀಲಿ_Type_Def *) stmt);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef*) stmt);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_UNKNOWN_DECLARATION:
        ಸ್ಥಿತಿ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported types %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Key type files may not include other declarations; "
                    "Ignoring %s\n", stmt_type_ಗಮ್ಯ_string(stmt->type));
            ok = false;
            ನಿಲ್ಲಿಸು;
        }

        ಆದರೆ (!ok)
            info->error_ಎಣಿಕೆ++;

        ಆದರೆ (info->error_ಎಣಿಕೆ > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning keytypes file \"%s\"\n",
                    ಸುರಕ್ಷಿತ_map_name(file->name));
            ನಿಲ್ಲಿಸು;
        }
    }
}

/***====================================================================***/

ಸ್ಥಿರ bool
Copy_ಕೀಲಿ_Types_ಗಮ್ಯ_Keymap(ರಚನೆ xkb_keymap *keymap, ಕೀಲಿ_Types_Info *info)
{
    ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t ಅಂಕೆ_types = darray_ಖಾಲಿಗಳು(info->types)
                                  ? 1
                                  : darray_ಗಾತ್ರ(info->types);
    ರಚನೆ xkb_ಕೀಲಿ_type *types = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ಅಂಕೆ_types, ಗಾತ್ರ(*types));
    ಆದರೆ (!types)
        ಹಿಂತಿರುಗಿ false;

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
    ಆದರೆ (darray_ಖಾಲಿಗಳು(info->types)) {
        ರಚನೆ xkb_ಕೀಲಿ_type *type = &types[0];

        type->mods.mods = 0;
        type->ಅಂಕೆ_ಮಟ್ಟಗಳು = 1;
        type->entries = ಖಾಲಿ;
        type->ಅಂಕೆ_entries = 0;
        type->name = xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL");
        type->level_ಹೆಸರುಗಳು = ಖಾಲಿ;
        type->ಅಂಕೆ_level_ಹೆಸರುಗಳು = 0;
        type->required = true;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        /* HACK to circumvent Xorg bugs (see comment above) */
        ಸ್ಥಿರಾಂಕ xkb_atom_t canonical_types[] = {
            xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "TWO_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "ALPHABETIC"),
            xkb_atom_intern_literal(keymap->ctx, "KEYPAD"),
        };

        ಗಾಗಿ (darray_ಗಾತ್ರ_t i = 0; i < ಅಂಕೆ_types; i++) {
            ಕೀಲಿ_Type_Info *def = &darray_ಅಂಶ(info->types, i);
            ರಚನೆ xkb_ಕೀಲಿ_type *type = &types[i];

            type->name = def->name;
            type->mods.mods = def->mods;
            type->ಅಂಕೆ_ಮಟ್ಟಗಳು = def->ಅಂಕೆ_ಮಟ್ಟಗಳು;
            type->ಅಂಕೆ_level_ಹೆಸರುಗಳು =
                (xkb_level_ಸೂಚ್ಯಂಕ_t) darray_ಗಾತ್ರ(def->level_ಹೆಸರುಗಳು);
            darray_steal(def->level_ಹೆಸರುಗಳು, &type->level_ಹೆಸರುಗಳು, ಖಾಲಿ);
            darray_steal(def->entries, &type->entries, &type->ಅಂಕೆ_entries);
            type->required = false;

            /* HACK: Never drop canonical XKB key types (see comment above) */
            ಆದರೆ (type->ಅಂಕೆ_ಮಟ್ಟಗಳು <= 2) {
                ಗಾಗಿ (uint8_t t = 0;
                     t < (uint8_t) ARRAY_ಗಾತ್ರ(canonical_types);
                     t++) {
                        ಆದರೆ (type->name == canonical_types[t]) {
                            type->required = true;
                            ನಿಲ್ಲಿಸು;
                        }
                }
            }
        }
    }

    keymap->types_section_name = strdup_ಸುರಕ್ಷಿತ(info->name);
    XkbEscapeMapName(keymap->types_section_name);
    keymap->ಅಂಕೆ_types = ಅಂಕೆ_types;
    keymap->types = types;
    keymap->mods = info->mods;
    ಹಿಂತಿರುಗಿ true;
}

/***====================================================================***/

bool
Compile_ಕೀಲಿ_Types(XkbFile *file, ರಚನೆ xkb_keymap_info *keymap_info)
{
    ಕೀಲಿ_Types_Info info;

    Init_ಕೀಲಿ_Types_Info(&info, keymap_info, 0,
                     &keymap_info->keymap.mods);

    ಆದರೆ (file != ಖಾಲಿ)
        Handle_ಕೀಲಿ_Types_File(&info, file);

    ಆದರೆ (info.error_ಎಣಿಕೆ != 0)
        ಹೋಗು err_info;

    ಆದರೆ (!Copy_ಕೀಲಿ_Types_ಗಮ್ಯ_Keymap(&keymap_info->keymap, &info))
        ಹೋಗು err_info;

    Clear_ಕೀಲಿ_Types_Info(&info);
    ಹಿಂತಿರುಗಿ true;

err_info:
    Clear_ಕೀಲಿ_Types_Info(&info);
    ಹಿಂತಿರುಗಿ false;
}
