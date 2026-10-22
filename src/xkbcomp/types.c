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

गणना type_क्षेत्रम् {
    TYPE_क्षेत्रम्_आवरणम् = (1 << 0),
    TYPE_क्षेत्रम्_MAP = (1 << 1),
    TYPE_क्षेत्रम्_PRESERVE = (1 << 2),
    TYPE_क्षेत्रम्_LEVEL_NAME = (1 << 3),
};

प्रकारनिर्देश संरचना {
    गणना type_क्षेत्रम् defined;
    गणना merge_बहुलकम् merge;

    xkb_atom_t name;
    xkb_mod_आवरणम्_t mods;
    xkb_level_सूचकांक_t अङ्कः_स्तराः;
    darray(संरचना xkb_कुञ्चिका_type_प्रविष्टिः) entries;
    darray(xkb_atom_t) level_नामानि;
} कुञ्चिका_Type_Info;

प्रकारनिर्देश संरचना {
    वर्ण *name;
    पूर्णाङ्क error_गणनम्;
    अचिह्नित पूर्णाङ्क include_गभीरता;

    darray(कुञ्चिका_Type_Info) types;
    संरचना xkb_mod_समुच्चयः mods;

    संरचना xkb_context *ctx;
    ध्रुव संरचना xkb_keymap_info *keymap_info;
} कुञ्चिका_Types_Info;

/***====================================================================***/

स्थिर अन्तःस्थ ध्रुव वर्ण *
Map_प्रविष्टिः_Txt(कुञ्चिका_Types_Info *info, संरचना xkb_कुञ्चिका_type_प्रविष्टिः *प्रविष्टिः)
{
    प्रत्यागम Mod_आवरणम्_Text(info->ctx, MOD_BOTH, &info->mods, प्रविष्टिः->mods.mods);
}

स्थिर अन्तःस्थ ध्रुव वर्ण *
TypeTxt(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *type)
{
    प्रत्यागम xkb_atom_text(info->ctx, type->name);
}

स्थिर अन्तःस्थ ध्रुव वर्ण *
Type_आवरणम्_Txt(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *type)
{
    प्रत्यागम Mod_आवरणम्_Text(info->ctx, MOD_BOTH, &info->mods, type->mods);
}

स्थिर अन्तःस्थ bool
ReportTypeShouldBeArray(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *type,
                        ध्रुव वर्ण *क्षेत्रम्_2)
{
    प्रत्यागम ReportShouldBeArray(info->ctx, "key type", क्षेत्रम्_2,
                               TypeTxt(info, type));
}

स्थिर अन्तःस्थ bool
ReportTypeBadType(कुञ्चिका_Types_Info *info, गणना xkb_सन्देशः_संहिता संहिता,
                  कुञ्चिका_Type_Info *type, ध्रुव वर्ण *क्षेत्रम्_2, ध्रुव वर्ण *wanted)
{
    प्रत्यागम ReportBadType(info->ctx, संहिता, "key type", क्षेत्रम्_2,
                         TypeTxt(info, type), wanted);
}

/***====================================================================***/

स्थिर शून्य
Init_कुञ्चिका_Types_Info(कुञ्चिका_Types_Info *info, ध्रुव संरचना xkb_keymap_info *keymap_info,
                 अचिह्नित पूर्णाङ्क include_गभीरता,
                 ध्रुव संरचना xkb_mod_समुच्चयः *mods)
{
    स्मृतिं_पूरय(info, 0, परिमाण(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_गभीरता = include_गभीरता;
    InitVMods(&info->mods, mods, include_गभीरता > 0);
}

स्थिर शून्य
Clear_कुञ्चिका_Type_Info(कुञ्चिका_Type_Info *type)
{
    darray_free(type->entries);
    darray_free(type->level_नामानि);
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
        यदि (old->name == name)
            प्रत्यागम old;

    प्रत्यागम रिक्त;
}

स्थिर bool
सङ्कलय_कुञ्चिका_Type(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *new, bool same_file)
{
    कुञ्चिका_Type_Info *old;
    ध्रुव पूर्णाङ्क verbosity = xkb_context_get_log_verbosity(info->ctx);

    old = Find_Matching_कुञ्चिका_Type(info, new->name);
    यदि (old) {
        यदि (new->merge != MERGE_AUGMENT) {
            यदि ((same_file && verbosity > 0) || verbosity > 9) {
                log_warn(info->ctx,
                         XKB_WARNING_CONFLICTING_कुञ्चिका_TYPE_DEFINITIONS,
                         "Multiple definitions of the %s key type; "
                         "Earlier definition ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            }

            Clear_कुञ्चिका_Type_Info(old);
            *old = *new;
            darray_init(new->entries);
            darray_init(new->level_नामानि);
            प्रत्यागम true;
        }

        यदि (same_file)
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_CONFLICTING_कुञ्चिका_TYPE_DEFINITIONS,
                    "Multiple definitions of the %s key type; "
                    "Later definition ignored\n",
                    xkb_atom_text(info->ctx, new->name));

        Clear_कुञ्चिका_Type_Info(new);
        प्रत्यागम true;
    }

    darray_append(info->types, *new);
    प्रत्यागम true;
}

/***====================================================================***/

स्थिर शून्य
Merge_Included_कुञ्चिका_Types(कुञ्चिका_Types_Info *into, कुञ्चिका_Types_Info *from,
                      गणना merge_बहुलकम् merge)
{
    यदि (from->error_गणनम् > 0) {
        into->error_गणनम् += from->error_गणनम्;
        प्रत्यागम;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    यदि (into->name == रिक्त) {
        into->name = steal(&from->name);
    }

    यदि (darray_रिक्तानि(into->types)) {
        into->types = from->types;
        /* Types stolen via shallow copy, so reinitialize the array */
        darray_init(from->types);
    }
    अन्यथा {
        कुञ्चिका_Type_Info *type;
        darray_foreach(type, from->types) {
            type->merge = merge;
            यदि (!सङ्कलय_कुञ्चिका_Type(into, type, false))
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

    यदि (Exceeds_Include_Max_गभीरता(info->ctx, info->include_गभीरता)) {
        info->error_गणनम् += 10;
        प्रत्यागम false;
    }

    Init_कुञ्चिका_Types_Info(&included, info->keymap_info, info->include_गभीरता + 1,
                     &info->mods);
    included.name = steal(&include->stmt);

    ध्रुव संरचना parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    प्रति (IncludeStmt *stmt = include; stmt; stmt = stmt->अग्र_incl) {
        कुञ्चिका_Types_Info अग्र_incl;
        XkbFile *file;

        वर्ण मार्गः[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_TYPES,
                                  मार्गः, परिमाण(मार्गः));
        यदि (!file) {
            info->error_गणनम् += 10;
            Clear_कुञ्चिका_Types_Info(&included);
            प्रत्यागम false;
        }

        Init_कुञ्चिका_Types_Info(&अग्र_incl, info->keymap_info, info->include_गभीरता + 1,
                         &included.mods);

        Handle_कुञ्चिका_Types_File(&अग्र_incl, file);

        Merge_Included_कुञ्चिका_Types(&included, &अग्र_incl, stmt->merge);

        Clear_कुञ्चिका_Types_Info(&अग्र_incl);
        FreeXkbFile(file);
    }

    Merge_Included_कुञ्चिका_Types(info, &included, include->merge);
    Clear_कुञ्चिका_Types_Info(&included);

    प्रत्यागम (info->error_गणनम् == 0);
}

/***====================================================================***/

स्थिर bool
समुच्चयः_Modifiers(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *type, व्यञ्जकम्_Def *arrayNdx,
             व्यञ्जकम्_Def *मानम्)
{
    xkb_mod_आवरणम्_t mods = 0;

    यदि (arrayNdx) {
        log_err(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                "The modifiers field of a key type is not an array; "
                "Illegal array subscript ignored\n");
        प्रत्यागम false;
    }

    यदि (!व्यञ्जकम्_Resolve_Mod_आवरणम्(info->ctx, मानम्, MOD_BOTH, &info->mods, &mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_आवरणम्,
                "Key type mask field must be a modifier mask; "
                "Key type definition ignored\n");
        प्रत्यागम false;
    }

    यदि (type->defined & TYPE_क्षेत्रम्_आवरणम्) {
        log_warn(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                 "Multiple modifier mask definitions for key type %s; "
                 "Using %s, ignoring %s\n",
                 xkb_atom_text(info->ctx, type->name),
                 Type_आवरणम्_Txt(info, type),
                 Mod_आवरणम्_Text(info->ctx, MOD_BOTH, &info->mods, mods));
        प्रत्यागम false;
    }

    type->mods = mods;
    प्रत्यागम true;
}

/***====================================================================***/

स्थिर संरचना xkb_कुञ्चिका_type_प्रविष्टिः *
Find_Matching_Map_प्रविष्टिः(कुञ्चिका_Type_Info *type, xkb_mod_आवरणम्_t mods)
{
    संरचना xkb_कुञ्चिका_type_प्रविष्टिः *प्रविष्टिः;

    darray_foreach(प्रविष्टिः, type->entries)
        यदि (प्रविष्टिः->mods.mods == mods)
            प्रत्यागम प्रविष्टिः;

    प्रत्यागम रिक्त;
}

स्थिर bool
सङ्कलय_Map_प्रविष्टिः(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *type,
            संरचना xkb_कुञ्चिका_type_प्रविष्टिः *new, bool clobber, bool report)
{
    संरचना xkb_कुञ्चिका_type_प्रविष्टिः *old;

    old = Find_Matching_Map_प्रविष्टिः(type, new->mods.mods);
    यदि (old) {
        यदि (report && old->level != new->level) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_TYPE_MAP_प्रविष्टिः,
                     "Multiple map entries for %s in %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     Map_प्रविष्टिः_Txt(info, new), TypeTxt(info, type),
                     (clobber ? new->level : old->level) + 1,
                     (clobber ? old->level : new->level) + 1);
        }
        अन्यथा {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_CONFLICTING_कुञ्चिका_TYPE_MAP_प्रविष्टिः,
                    "Multiple occurrences of map[%s]= %"PRIu32" in %s; Ignored\n",
                    Map_प्रविष्टिः_Txt(info, new), new->level + 1,
                    TypeTxt(info, type));
            प्रत्यागम true;
        }

        यदि (clobber) {
            यदि (new->level >= type->अङ्कः_स्तराः)
                type->अङ्कः_स्तराः = new->level + 1;
            old->level = new->level;
        }

        प्रत्यागम true;
    }

    यदि (new->level >= type->अङ्कः_स्तराः)
        type->अङ्कः_स्तराः = new->level + 1;

    darray_append(type->entries, *new);
    प्रत्यागम true;
}

स्थिर bool
समुच्चयः_Map_प्रविष्टिः(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *type, व्यञ्जकम्_Def *arrayNdx,
            व्यञ्जकम्_Def *मानम्)
{
    संरचना xkb_कुञ्चिका_type_प्रविष्टिः प्रविष्टिः;

    यदि (arrayNdx == रिक्त)
        प्रत्यागम ReportTypeShouldBeArray(info, type, "map entry");

    यदि (!व्यञ्जकम्_Resolve_Mod_आवरणम्(info->ctx, arrayNdx, MOD_BOTH, &info->mods,
                            &प्रविष्टिः.mods.mods))
        प्रत्यागम ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_आवरणम्_2,
                                 type, "map entry", "modifier mask");

    यदि (प्रविष्टिः.mods.mods & (~type->mods)) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_अन्तर्_कुञ्चिका_TYPE,
                "Map entry for modifiers not used by type %s; "
                "Using %s instead of %s\n",
                TypeTxt(info, type),
                Mod_आवरणम्_Text(info->ctx, MOD_BOTH, &info->mods,
                            प्रविष्टिः.mods.mods & type->mods),
                Map_प्रविष्टिः_Txt(info, &प्रविष्टिः));
        प्रविष्टिः.mods.mods &= type->mods;
    }

    यदि (!व्यञ्जकम्_Resolve_Level(info->ctx, मानम्, &प्रविष्टिः.level)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_सरणम्_LEVEL,
                          "Level specifications in a key type must be integer; "
                          "Ignoring malformed level specification\n");
        प्रत्यागम false;
    }

    प्रविष्टिः.preserve.mods = 0;

    प्रत्यागम सङ्कलय_Map_प्रविष्टिः(info, type, &प्रविष्टिः, true, true);
}

/***====================================================================***/

स्थिर bool
सङ्कलय_Preserve(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *type,
            xkb_mod_आवरणम्_t mods, xkb_mod_आवरणम्_t preserve_mods)
{
    संरचना xkb_कुञ्चिका_type_प्रविष्टिः *प्रविष्टिः;

    darray_foreach(प्रविष्टिः, type->entries) {
        यदि (प्रविष्टिः->mods.mods != mods)
            अनुवर्त;

        /* Map exists without previous preserve (or "None"); override. */
        यदि (प्रविष्टिः->preserve.mods == 0) {
            प्रविष्टिः->preserve.mods = preserve_mods;
            प्रत्यागम true;
        }

        /* Map exists with same preserve; do nothing. */
        यदि (प्रविष्टिः->preserve.mods == preserve_mods) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_DUPLICATE_प्रविष्टिः,
                    "Identical definitions for preserve[%s] in %s; "
                    "Ignored\n",
                    Mod_आवरणम्_Text(info->ctx, MOD_BOTH, &info->mods, mods),
                    TypeTxt(info, type));
            प्रत्यागम true;
        }

        /* Map exists with different preserve; latter wins. */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_कुञ्चिका_TYPE_PRESERVE_ENTRIES,
                "Multiple definitions for preserve[%s] in %s; "
                "Using %s, ignoring %s\n",
                Mod_आवरणम्_Text(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type),
                Mod_आवरणम्_Text(info->ctx, MOD_BOTH, &info->mods, preserve_mods),
                Mod_आवरणम्_Text(info->ctx, MOD_BOTH, &info->mods,
                            प्रविष्टिः->preserve.mods));

        प्रविष्टिः->preserve.mods = preserve_mods;
        प्रत्यागम true;
    }

    /*
     * Map does not exist, i.e. preserve[] came before map[].
     * Create a map with the specified mask mapping to Level1. The level
     * may be overridden later with an explicit map[] statement.
     */
    ध्रुव संरचना xkb_कुञ्चिका_type_प्रविष्टिः new = {
        .level = 0,
        .mods = { .mods = mods },
        .preserve = { .mods = preserve_mods },
    };
    darray_append(type->entries, new);
    प्रत्यागम true;
}

स्थिर bool
समुच्चयः_Preserve(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *type, व्यञ्जकम्_Def *arrayNdx,
            व्यञ्जकम्_Def *मानम्)
{

    यदि (arrayNdx == रिक्त)
        प्रत्यागम ReportTypeShouldBeArray(info, type, "preserve entry");

    xkb_mod_आवरणम्_t mods = 0;
    यदि (!व्यञ्जकम्_Resolve_Mod_आवरणम्(info->ctx, arrayNdx, MOD_BOTH, &info->mods, &mods))
        प्रत्यागम ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_आवरणम्_2,
                                 type, "preserve entry", "modifier mask");

    यदि (mods & ~type->mods) {
        ध्रुव वर्ण *before, *after;

        before = Mod_आवरणम्_Text(info->ctx, MOD_BOTH, &info->mods, mods);
        mods &= type->mods;
        after = Mod_आवरणम्_Text(info->ctx, MOD_BOTH, &info->mods, mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_अन्तर्_कुञ्चिका_TYPE,
                "Preserve entry for modifiers not used by the %s type; "
                "Index %s converted to %s\n",
                TypeTxt(info, type), before, after);
    }

    xkb_mod_आवरणम्_t preserve_mods = 0;
    यदि (!व्यञ्जकम्_Resolve_Mod_आवरणम्(info->ctx, मानम्, MOD_BOTH, &info->mods,
                            &preserve_mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_आवरणम्,
                "Preserve value in a key type is not a modifier mask; "
                "Ignoring preserve[%s] in type %s\n",
                Mod_आवरणम्_Text(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type));
        प्रत्यागम false;
    }

    यदि (preserve_mods & ~mods) {
        ध्रुव वर्ण *before, *after;

        before = Mod_आवरणम्_Text(info->ctx, MOD_BOTH, &info->mods, preserve_mods);
        preserve_mods &= mods;
        after = Mod_आवरणम्_Text(info->ctx, MOD_BOTH, &info->mods, preserve_mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_ILLEGAL_कुञ्चिका_TYPE_PRESERVE_परिणाम,
                "Illegal value for preserve[%s] in type %s; "
                "Converted %s to %s\n",
                Mod_आवरणम्_Text(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type), before, after);
    }

    प्रत्यागम सङ्कलय_Preserve(info, type, mods, preserve_mods);
}

/***====================================================================***/

स्थिर bool
सङ्कलय_Level_Name(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *type,
             xkb_level_सूचकांक_t level, xkb_atom_t name, bool clobber)
{
    /* New name. */
    यदि (level >= darray_आकार(type->level_नामानि)) {
        darray_resize0(type->level_नामानि, level + 1);
        गच्छ finish;
    }

    /* Same level, same name. */
    यदि (darray_item(type->level_नामानि, level) == name) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE, XKB_WARNING_DUPLICATE_प्रविष्टिः,
                "Duplicate names for level %"PRIu32" of key type %s; Ignored\n",
                level + 1, TypeTxt(info, type));
        प्रत्यागम true;
    }

    /* Same level, different name. */
    यदि (darray_item(type->level_नामानि, level) != XKB_ATOM_NONE) {
        ध्रुव वर्ण *old, *new;
        old = xkb_atom_text(info->ctx,
                            darray_item(type->level_नामानि, level));
        new = xkb_atom_text(info->ctx, name);
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_कुञ्चिका_TYPE_LEVEL_नामानि,
                "Multiple names for level %"PRIu32" of key type %s; "
                "Using %s, ignoring %s\n",
                level + 1, TypeTxt(info, type),
                (clobber ? new : old), (clobber ? old : new));

        यदि (!clobber)
            प्रत्यागम true;
    }

    /* FIXME: What about different level, same name? */

finish:
    darray_item(type->level_नामानि, level) = name;
    प्रत्यागम true;
}

स्थिर bool
समुच्चयः_Level_Name(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *type, व्यञ्जकम्_Def *arrayNdx,
             व्यञ्जकम्_Def *मानम्)
{
    यदि (arrayNdx == रिक्त)
        प्रत्यागम ReportTypeShouldBeArray(info, type, "level name");

    xkb_level_सूचकांक_t level = 0;
    यदि (!व्यञ्जकम्_Resolve_Level(info->ctx, arrayNdx, &level))
        प्रत्यागम ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_सरणम्_LEVEL,
                                 type, "level name", "integer");

    xkb_atom_t level_name = XKB_ATOM_NONE;
    यदि (!व्यञ्जकम्_Resolve_String(info->ctx, मानम्, &level_name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE,
                "Non-string name for level %"PRIu32" in key type %s; "
                "Ignoring illegal level name definition\n",
                level + 1, xkb_atom_text(info->ctx, type->name));
        प्रत्यागम false;
    }

    प्रत्यागम सङ्कलय_Level_Name(info, type, level, level_name, true);
}

/***====================================================================***/

स्थिर bool
समुच्चयः_कुञ्चिका_Type_क्षेत्रम्(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Info *type,
                ध्रुव वर्ण *क्षेत्रम्_2, व्यञ्जकम्_Def *arrayNdx, व्यञ्जकम्_Def *मानम्)
{
    bool ok = false;
    गणना type_क्षेत्रम् type_क्षेत्रम् = 0;

    यदि (istreq(क्षेत्रम्_2, "modifiers")) {
        type_क्षेत्रम् = TYPE_क्षेत्रम्_आवरणम्;
        ok = समुच्चयः_Modifiers(info, type, arrayNdx, मानम्);
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "map")) {
        type_क्षेत्रम् = TYPE_क्षेत्रम्_MAP;
        ok = समुच्चयः_Map_प्रविष्टिः(info, type, arrayNdx, मानम्);
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "preserve")) {
        type_क्षेत्रम् = TYPE_क्षेत्रम्_PRESERVE;
        ok = समुच्चयः_Preserve(info, type, arrayNdx, मानम्);
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "levelname") || istreq(क्षेत्रम्_2, "level_name")) {
        type_क्षेत्रम् = TYPE_क्षेत्रम्_LEVEL_NAME;
        ok = समुच्चयः_Level_Name(info, type, arrayNdx, मानम्);
    } अन्यथा {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_क्षेत्रम्,
                "Unknown field \"%s\" in key type \"%s\"; Definition ignored\n",
                क्षेत्रम्_2, TypeTxt(info, type));
        ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_TYPE_FIELDS);
    }

    type->defined |= type_क्षेत्रम्;
    प्रत्यागम ok;
}

स्थिर bool
Handle_कुञ्चिका_Type_Body(कुञ्चिका_Types_Info *info, VarDef *def, कुञ्चिका_Type_Info *type)
{
    bool ok = true;
    ध्रुव वर्ण *elem, *क्षेत्रम्_2;
    व्यञ्जकम्_Def *arrayNdx;

    प्रति (; def; def = (VarDef *) def->common.अग्र) {
        यदि (!व्यञ्जकम्_Resolve_Lhs(info->ctx, def->name, &elem, &क्षेत्रम्_2,
                            &arrayNdx)) {
            /* internal error, already reported */
            ok = false;
            अनुवर्त;
        }

        यदि (elem) {
            यदि (istreq(elem, "type")) {
                log_err(info->ctx, XKB_ERROR_INVALID_समुच्चयः_DEFAULT_STATEMENT,
                        "Support for changing the default type has been removed; "
                        "Statement \"%s.%s\" ignored.\n", elem, क्षेत्रम्_2);
            }
            अन्यथा {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key type statement: move statements to the global "
                        "file scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, क्षेत्रम्_2);
                ok = false;
            }
            अनुवर्त;
        }

        यदि (!समुच्चयः_कुञ्चिका_Type_क्षेत्रम्(info, type, क्षेत्रम्_2, arrayNdx, def->मानम्))
            ok = false;
    }

    प्रत्यागम ok;
}

स्थिर bool
Handle_कुञ्चिका_Type_Def(कुञ्चिका_Types_Info *info, कुञ्चिका_Type_Def *def)
{
    कुञ्चिका_Type_Info type = {
        .defined = 0,
        .merge = def->merge,
        .name = def->name,
        .mods = 0,
        .अङ्कः_स्तराः = 1,
        .entries = darray_new(),
        .level_नामानि = darray_new(),
    };

    यदि (!Handle_कुञ्चिका_Type_Body(info, def->body, &type) ||
        !सङ्कलय_कुञ्चिका_Type(info, &type, true))
    {
        info->error_गणनम्++;
        Clear_कुञ्चिका_Type_Info(&type);
        प्रत्यागम false;
    }

    /* Type has been either stolen via shallow copy or reinitialized in
       `AddKeyType`: no need to free the arrays */
    प्रत्यागम true;
}

स्थिर bool
HandleGlobalVar(कुञ्चिका_Types_Info *info, VarDef *stmt)
{
    ध्रुव वर्ण *elem, *क्षेत्रम्_2;
    व्यञ्जकम्_Def *arrayNdx;

    यदि (!व्यञ्जकम्_Resolve_Lhs(info->ctx, stmt->name, &elem, &क्षेत्रम्_2,
                        &arrayNdx)) {
        प्रत्यागम false;           /* internal error, already reported */
    }
    अन्यथा यदि (elem && istreq(elem, "type")) {
        log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                "Support for changing the default type has been removed; "
                "Statement ignored\n");
        प्रत्यागम true;
    }
    अन्यथा यदि (elem) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_क्षेत्रम्,
                "Default defined for unknown element \"%s\"; "
                "Value for field \"%s.%s\" ignored\n", elem, elem, क्षेत्रम्_2);
        प्रत्यागम !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
    }
    अन्यथा यदि (क्षेत्रम्_2) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_क्षेत्रम्,
                "Default defined for unknown field \"%s\"; Ignored\n", क्षेत्रम्_2);
        प्रत्यागम !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS);
    }
    प्रत्यागम false;
}

स्थिर शून्य
Handle_कुञ्चिका_Types_File(कुञ्चिका_Types_Info *info, XkbFile *file)
{
    bool ok;

    मुक्त(info->name);
    info->name = strdup_safe(file->name);

    प्रति (ParseCommon *stmt = file->defs; stmt; stmt = stmt->अग्र) {
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
                    "Ignoring %s\n", stmt_type_गन्तव्यम्_string(stmt->type));
            ok = false;
            भङ्ग;
        }

        यदि (!ok)
            info->error_गणनम्++;

        यदि (info->error_गणनम् > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning keytypes file \"%s\"\n",
                    safe_map_name(file->name));
            भङ्ग;
        }
    }
}

/***====================================================================***/

स्थिर bool
Copy_कुञ्चिका_Types_गन्तव्यम्_Keymap(संरचना xkb_keymap *keymap, कुञ्चिका_Types_Info *info)
{
    ध्रुव darray_आकार_t अङ्कः_types = darray_रिक्तानि(info->types)
                                  ? 1
                                  : darray_आकार(info->types);
    संरचना xkb_कुञ्चिका_type *types = शून्यस्मृतिं_देहि(अङ्कः_types, परिमाण(*types));
    यदि (!types)
        प्रत्यागम false;

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
    यदि (darray_रिक्तानि(info->types)) {
        संरचना xkb_कुञ्चिका_type *type = &types[0];

        type->mods.mods = 0;
        type->अङ्कः_स्तराः = 1;
        type->entries = रिक्त;
        type->अङ्कः_entries = 0;
        type->name = xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL");
        type->level_नामानि = रिक्त;
        type->अङ्कः_level_नामानि = 0;
        type->required = true;
    }
    अन्यथा {
        /* HACK to circumvent Xorg bugs (see comment above) */
        ध्रुव xkb_atom_t canonical_types[] = {
            xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "TWO_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "ALPHABETIC"),
            xkb_atom_intern_literal(keymap->ctx, "KEYPAD"),
        };

        प्रति (darray_आकार_t i = 0; i < अङ्कः_types; i++) {
            कुञ्चिका_Type_Info *def = &darray_item(info->types, i);
            संरचना xkb_कुञ्चिका_type *type = &types[i];

            type->name = def->name;
            type->mods.mods = def->mods;
            type->अङ्कः_स्तराः = def->अङ्कः_स्तराः;
            type->अङ्कः_level_नामानि =
                (xkb_level_सूचकांक_t) darray_आकार(def->level_नामानि);
            darray_steal(def->level_नामानि, &type->level_नामानि, रिक्त);
            darray_steal(def->entries, &type->entries, &type->अङ्कः_entries);
            type->required = false;

            /* HACK: Never drop canonical XKB key types (see comment above) */
            यदि (type->अङ्कः_स्तराः <= 2) {
                प्रति (uint8_t t = 0;
                     t < (uint8_t) ARRAY_आकार(canonical_types);
                     t++) {
                        यदि (type->name == canonical_types[t]) {
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
    प्रत्यागम true;
}

/***====================================================================***/

bool
Compile_कुञ्चिका_Types(XkbFile *file, संरचना xkb_keymap_info *keymap_info)
{
    कुञ्चिका_Types_Info info;

    Init_कुञ्चिका_Types_Info(&info, keymap_info, 0,
                     &keymap_info->keymap.mods);

    यदि (file != रिक्त)
        Handle_कुञ्चिका_Types_File(&info, file);

    यदि (info.error_गणनम् != 0)
        गच्छ err_info;

    यदि (!Copy_कुञ्चिका_Types_गन्तव्यम्_Keymap(&keymap_info->keymap, &info))
        गच्छ err_info;

    Clear_कुञ्चिका_Types_Info(&info);
    प्रत्यागम true;

err_info:
    Clear_कुञ्चिका_Types_Info(&info);
    प्रत्यागम false;
}
