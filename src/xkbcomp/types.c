/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#ضمن "config.h"

#ضمن <limits.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "xkbcommon/xkbcommon-status.h"
#ضمن "xkbcomp-priv.h"
#ضمن "text.h"
#ضمن "vmod.h"
#ضمن "expr.h"
#ضمن "include.h"
#ضمن "utils.h"
#ضمن "util-mem.h"

تعداد type_ميدان {
    TYPE_ميدان_نقاب = (1 << 0),
    TYPE_ميدان_MAP = (1 << 1),
    TYPE_ميدان_PRESERVE = (1 << 2),
    TYPE_ميدان_LEVEL_NAME = (1 << 3),
};

تعريفنوع بنية {
    تعداد type_ميدان defined;
    تعداد ملايو_گهڻائي ملايو;

    xkb_atom_t name;
    xkb_mod_نقاب_t mods;
    xkb_level_اشاريو_t انگ_سطحون;
    darray(بنية xkb_ڪنجي_type_داخلا) entries;
    darray(xkb_atom_t) level_نالا;
} ڪنجي_Type_Info;

تعريفنوع بنية {
    حرف *name;
    صحيح error_ڳڻپ;
    بنانشان صحيح include_اونهائي;

    darray(ڪنجي_Type_Info) types;
    بنية xkb_mod_سيٽ mods;

    بنية xkb_context *ctx;
    ثابت بنية xkb_keymap_info *keymap_info;
} ڪنجي_Types_Info;

/***====================================================================***/

ساكن مضمن ثابت حرف *
Map_داخلا_Txt(ڪنجي_Types_Info *info, بنية xkb_ڪنجي_type_داخلا *داخلا)
{
    أرجع Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, داخلا->mods.mods);
}

ساكن مضمن ثابت حرف *
TypeTxt(ڪنجي_Types_Info *info, ڪنجي_Type_Info *type)
{
    أرجع xkb_atom_متن(info->ctx, type->name);
}

ساكن مضمن ثابت حرف *
Type_نقاب_Txt(ڪنجي_Types_Info *info, ڪنجي_Type_Info *type)
{
    أرجع Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, type->mods);
}

ساكن مضمن bool
ReportTypeShouldBeArray(ڪنجي_Types_Info *info, ڪنجي_Type_Info *type,
                        ثابت حرف *ميدان_2)
{
    أرجع ReportShouldBeArray(info->ctx, "key type", ميدان_2,
                               TypeTxt(info, type));
}

ساكن مضمن bool
ReportTypeBadType(ڪنجي_Types_Info *info, تعداد xkb_پيغام_ڪوڊ ڪوڊ,
                  ڪنجي_Type_Info *type, ثابت حرف *ميدان_2, ثابت حرف *wanted)
{
    أرجع ReportBadType(info->ctx, ڪوڊ, "key type", ميدان_2,
                         TypeTxt(info, type), wanted);
}

/***====================================================================***/

ساكن فراغ
Init_ڪنجي_Types_Info(ڪنجي_Types_Info *info, ثابت بنية xkb_keymap_info *keymap_info,
                 بنانشان صحيح include_اونهائي,
                 ثابت بنية xkb_mod_سيٽ *mods)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_اونهائي = include_اونهائي;
    InitVMods(&info->mods, mods, include_اونهائي > 0);
}

ساكن فراغ
Clear_ڪنجي_Type_Info(ڪنجي_Type_Info *type)
{
    darray_free(type->entries);
    darray_free(type->level_نالا);
}

ساكن فراغ
Clear_ڪنجي_Types_Info(ڪنجي_Types_Info *info)
{
    حرر(info->name);
    ڪنجي_Type_Info *type;
    darray_foreach(type, info->types)
        Clear_ڪنجي_Type_Info(type);
    darray_free(info->types);
}

ساكن ڪنجي_Type_Info *
ڳوليو_Matching_ڪنجي_Type(ڪنجي_Types_Info *info, xkb_atom_t name)
{
    ڪنجي_Type_Info *old;

    darray_foreach(old, info->types)
        إذا (old->name == name)
            أرجع old;

    أرجع عدم;
}

ساكن bool
جوڙيو_ڪنجي_Type(ڪنجي_Types_Info *info, ڪنجي_Type_Info *new, bool same_file)
{
    ڪنجي_Type_Info *old;
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);

    old = ڳوليو_Matching_ڪنجي_Type(info, new->name);
    إذا (old) {
        إذا (new->ملايو != ملايو_AUGMENT) {
            إذا ((same_file && verbosity > 0) || verbosity > 9) {
                log_warn(info->ctx,
                         XKB_WARNING_CONFLICTING_ڪنجي_TYPE_DEFINITIONS,
                         "Multiple definitions of the %s key type; "
                         "Earlier definition ignored\n",
                         xkb_atom_متن(info->ctx, new->name));
            }

            Clear_ڪنجي_Type_Info(old);
            *old = *new;
            darray_init(new->entries);
            darray_init(new->level_نالا);
            أرجع true;
        }

        إذا (same_file)
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_CONFLICTING_ڪنجي_TYPE_DEFINITIONS,
                    "Multiple definitions of the %s key type; "
                    "Later definition ignored\n",
                    xkb_atom_متن(info->ctx, new->name));

        Clear_ڪنجي_Type_Info(new);
        أرجع true;
    }

    darray_append(info->types, *new);
    أرجع true;
}

/***====================================================================***/

ساكن فراغ
ملايو_Included_ڪنجي_Types(ڪنجي_Types_Info *into, ڪنجي_Types_Info *from,
                      تعداد ملايو_گهڻائي ملايو)
{
    إذا (from->error_ڳڻپ > 0) {
        into->error_ڳڻپ += from->error_ڳڻپ;
        أرجع;
    }

    ملايو_Mod_Sets(into->ctx, &into->mods, &from->mods, ملايو);

    إذا (into->name == عدم) {
        into->name = steal(&from->name);
    }

    إذا (darray_خالي_جايون(into->types)) {
        into->types = from->types;
        /* Types stolen via shallow copy, so reinitialize the array */
        darray_init(from->types);
    }
    وإلا {
        ڪنجي_Type_Info *type;
        darray_foreach(type, from->types) {
            type->ملايو = ملايو;
            إذا (!جوڙيو_ڪنجي_Type(into, type, false))
                into->error_ڳڻپ++;
        }
        /* Types were either shallow copied or reinitialized individually
           in `AddKeyType`, so we only need to free the array */
        darray_free(from->types);
    }
}

ساكن فراغ
Handle_ڪنجي_Types_File(ڪنجي_Types_Info *info, XkbFile *file);

ساكن bool
Handle_Include_ڪنجي_Types(ڪنجي_Types_Info *info, IncludeStmt *include)
{
    ڪنجي_Types_Info included;

    إذا (Exceeds_Include_Max_اونهائي(info->ctx, info->include_اونهائي)) {
        info->error_ڳڻپ += 10;
        أرجع false;
    }

    Init_ڪنجي_Types_Info(&included, info->keymap_info, info->include_اونهائي + 1,
                     &info->mods);
    included.name = steal(&include->stmt);

    ثابت بنية parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    لكل (IncludeStmt *stmt = include; stmt; stmt = stmt->ايندڙ_incl) {
        ڪنجي_Types_Info ايندڙ_incl;
        XkbFile *file;

        حرف رستو[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_TYPES,
                                  رستو, حجم(رستو));
        إذا (!file) {
            info->error_ڳڻپ += 10;
            Clear_ڪنجي_Types_Info(&included);
            أرجع false;
        }

        Init_ڪنجي_Types_Info(&ايندڙ_incl, info->keymap_info, info->include_اونهائي + 1,
                         &included.mods);

        Handle_ڪنجي_Types_File(&ايندڙ_incl, file);

        ملايو_Included_ڪنجي_Types(&included, &ايندڙ_incl, stmt->ملايو);

        Clear_ڪنجي_Types_Info(&ايندڙ_incl);
        FreeXkbFile(file);
    }

    ملايو_Included_ڪنجي_Types(info, &included, include->ملايو);
    Clear_ڪنجي_Types_Info(&included);

    أرجع (info->error_ڳڻپ == 0);
}

/***====================================================================***/

ساكن bool
سيٽ_Modifiers(ڪنجي_Types_Info *info, ڪنجي_Type_Info *type, اظهار_Def *arrayNdx,
             اظهار_Def *قدر)
{
    xkb_mod_نقاب_t mods = 0;

    إذا (arrayNdx) {
        log_err(info->ctx, XKB_LOG_پيغام_NO_ID,
                "The modifiers field of a key type is not an array; "
                "Illegal array subscript ignored\n");
        أرجع false;
    }

    إذا (!اظهار_Resolve_Mod_نقاب(info->ctx, قدر, MOD_BOTH, &info->mods, &mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب,
                "Key type mask field must be a modifier mask; "
                "Key type definition ignored\n");
        أرجع false;
    }

    إذا (type->defined & TYPE_ميدان_نقاب) {
        log_warn(info->ctx, XKB_LOG_پيغام_NO_ID,
                 "Multiple modifier mask definitions for key type %s; "
                 "Using %s, ignoring %s\n",
                 xkb_atom_متن(info->ctx, type->name),
                 Type_نقاب_Txt(info, type),
                 Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, mods));
        أرجع false;
    }

    type->mods = mods;
    أرجع true;
}

/***====================================================================***/

ساكن بنية xkb_ڪنجي_type_داخلا *
ڳوليو_Matching_Map_داخلا(ڪنجي_Type_Info *type, xkb_mod_نقاب_t mods)
{
    بنية xkb_ڪنجي_type_داخلا *داخلا;

    darray_foreach(داخلا, type->entries)
        إذا (داخلا->mods.mods == mods)
            أرجع داخلا;

    أرجع عدم;
}

ساكن bool
جوڙيو_Map_داخلا(ڪنجي_Types_Info *info, ڪنجي_Type_Info *type,
            بنية xkb_ڪنجي_type_داخلا *new, bool clobber, bool report)
{
    بنية xkb_ڪنجي_type_داخلا *old;

    old = ڳوليو_Matching_Map_داخلا(type, new->mods.mods);
    إذا (old) {
        إذا (report && old->level != new->level) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_ڪنجي_TYPE_MAP_داخلا,
                     "Multiple map entries for %s in %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     Map_داخلا_Txt(info, new), TypeTxt(info, type),
                     (clobber ? new->level : old->level) + 1,
                     (clobber ? old->level : new->level) + 1);
        }
        وإلا {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_CONFLICTING_ڪنجي_TYPE_MAP_داخلا,
                    "Multiple occurrences of map[%s]= %"PRIu32" in %s; Ignored\n",
                    Map_داخلا_Txt(info, new), new->level + 1,
                    TypeTxt(info, type));
            أرجع true;
        }

        إذا (clobber) {
            إذا (new->level >= type->انگ_سطحون)
                type->انگ_سطحون = new->level + 1;
            old->level = new->level;
        }

        أرجع true;
    }

    إذا (new->level >= type->انگ_سطحون)
        type->انگ_سطحون = new->level + 1;

    darray_append(type->entries, *new);
    أرجع true;
}

ساكن bool
سيٽ_Map_داخلا(ڪنجي_Types_Info *info, ڪنجي_Type_Info *type, اظهار_Def *arrayNdx,
            اظهار_Def *قدر)
{
    بنية xkb_ڪنجي_type_داخلا داخلا;

    إذا (arrayNdx == عدم)
        أرجع ReportTypeShouldBeArray(info, type, "map entry");

    إذا (!اظهار_Resolve_Mod_نقاب(info->ctx, arrayNdx, MOD_BOTH, &info->mods,
                            &داخلا.mods.mods))
        أرجع ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب_2,
                                 type, "map entry", "modifier mask");

    إذا (داخلا.mods.mods & (~type->mods)) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_اندريون_ڪنجي_TYPE,
                "Map entry for modifiers not used by type %s; "
                "Using %s instead of %s\n",
                TypeTxt(info, type),
                Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods,
                            داخلا.mods.mods & type->mods),
                Map_داخلا_Txt(info, &داخلا));
        داخلا.mods.mods &= type->mods;
    }

    إذا (!اظهار_Resolve_Level(info->ctx, قدر, &داخلا.level)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_سرڪايو_LEVEL,
                          "Level specifications in a key type must be integer; "
                          "Ignoring malformed level specification\n");
        أرجع false;
    }

    داخلا.preserve.mods = 0;

    أرجع جوڙيو_Map_داخلا(info, type, &داخلا, true, true);
}

/***====================================================================***/

ساكن bool
جوڙيو_Preserve(ڪنجي_Types_Info *info, ڪنجي_Type_Info *type,
            xkb_mod_نقاب_t mods, xkb_mod_نقاب_t preserve_mods)
{
    بنية xkb_ڪنجي_type_داخلا *داخلا;

    darray_foreach(داخلا, type->entries) {
        إذا (داخلا->mods.mods != mods)
            استمر;

        /* Map exists without previous preserve (or "None"); override. */
        إذا (داخلا->preserve.mods == 0) {
            داخلا->preserve.mods = preserve_mods;
            أرجع true;
        }

        /* Map exists with same preserve; do nothing. */
        إذا (داخلا->preserve.mods == preserve_mods) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_DUPLICATE_داخلا,
                    "Identical definitions for preserve[%s] in %s; "
                    "Ignored\n",
                    Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, mods),
                    TypeTxt(info, type));
            أرجع true;
        }

        /* Map exists with different preserve; latter wins. */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_ڪنجي_TYPE_PRESERVE_ENTRIES,
                "Multiple definitions for preserve[%s] in %s; "
                "Using %s, ignoring %s\n",
                Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type),
                Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, preserve_mods),
                Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods,
                            داخلا->preserve.mods));

        داخلا->preserve.mods = preserve_mods;
        أرجع true;
    }

    /*
     * Map does not exist, i.e. preserve[] came before map[].
     * Create a map with the specified mask mapping to Level1. The level
     * may be overridden later with an explicit map[] statement.
     */
    ثابت بنية xkb_ڪنجي_type_داخلا new = {
        .level = 0,
        .mods = { .mods = mods },
        .preserve = { .mods = preserve_mods },
    };
    darray_append(type->entries, new);
    أرجع true;
}

ساكن bool
سيٽ_Preserve(ڪنجي_Types_Info *info, ڪنجي_Type_Info *type, اظهار_Def *arrayNdx,
            اظهار_Def *قدر)
{

    إذا (arrayNdx == عدم)
        أرجع ReportTypeShouldBeArray(info, type, "preserve entry");

    xkb_mod_نقاب_t mods = 0;
    إذا (!اظهار_Resolve_Mod_نقاب(info->ctx, arrayNdx, MOD_BOTH, &info->mods, &mods))
        أرجع ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب_2,
                                 type, "preserve entry", "modifier mask");

    إذا (mods & ~type->mods) {
        ثابت حرف *before, *after;

        before = Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, mods);
        mods &= type->mods;
        after = Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_اندريون_ڪنجي_TYPE,
                "Preserve entry for modifiers not used by the %s type; "
                "Index %s converted to %s\n",
                TypeTxt(info, type), before, after);
    }

    xkb_mod_نقاب_t preserve_mods = 0;
    إذا (!اظهار_Resolve_Mod_نقاب(info->ctx, قدر, MOD_BOTH, &info->mods,
                            &preserve_mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب,
                "Preserve value in a key type is not a modifier mask; "
                "Ignoring preserve[%s] in type %s\n",
                Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type));
        أرجع false;
    }

    إذا (preserve_mods & ~mods) {
        ثابت حرف *before, *after;

        before = Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, preserve_mods);
        preserve_mods &= mods;
        after = Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, preserve_mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_ILLEGAL_ڪنجي_TYPE_PRESERVE_نتيجو,
                "Illegal value for preserve[%s] in type %s; "
                "Converted %s to %s\n",
                Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type), before, after);
    }

    أرجع جوڙيو_Preserve(info, type, mods, preserve_mods);
}

/***====================================================================***/

ساكن bool
جوڙيو_Level_Name(ڪنجي_Types_Info *info, ڪنجي_Type_Info *type,
             xkb_level_اشاريو_t level, xkb_atom_t name, bool clobber)
{
    /* New name. */
    إذا (level >= darray_سائيز(type->level_نالا)) {
        darray_resize0(type->level_نالا, level + 1);
        اذهب پڄاڻي;
    }

    /* Same level, same name. */
    إذا (darray_شي(type->level_نالا, level) == name) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE, XKB_WARNING_DUPLICATE_داخلا,
                "Duplicate names for level %"PRIu32" of key type %s; Ignored\n",
                level + 1, TypeTxt(info, type));
        أرجع true;
    }

    /* Same level, different name. */
    إذا (darray_شي(type->level_نالا, level) != XKB_ATOM_NONE) {
        ثابت حرف *old, *new;
        old = xkb_atom_متن(info->ctx,
                            darray_شي(type->level_نالا, level));
        new = xkb_atom_متن(info->ctx, name);
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_ڪنجي_TYPE_LEVEL_نالا,
                "Multiple names for level %"PRIu32" of key type %s; "
                "Using %s, ignoring %s\n",
                level + 1, TypeTxt(info, type),
                (clobber ? new : old), (clobber ? old : new));

        إذا (!clobber)
            أرجع true;
    }

    /* FIXME: What about different level, same name? */

پڄاڻي:
    darray_شي(type->level_نالا, level) = name;
    أرجع true;
}

ساكن bool
سيٽ_Level_Name(ڪنجي_Types_Info *info, ڪنجي_Type_Info *type, اظهار_Def *arrayNdx,
             اظهار_Def *قدر)
{
    إذا (arrayNdx == عدم)
        أرجع ReportTypeShouldBeArray(info, type, "level name");

    xkb_level_اشاريو_t level = 0;
    إذا (!اظهار_Resolve_Level(info->ctx, arrayNdx, &level))
        أرجع ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_سرڪايو_LEVEL,
                                 type, "level name", "integer");

    xkb_atom_t level_name = XKB_ATOM_NONE;
    إذا (!اظهار_Resolve_String(info->ctx, قدر, &level_name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_ميدان_TYPE,
                "Non-string name for level %"PRIu32" in key type %s; "
                "Ignoring illegal level name definition\n",
                level + 1, xkb_atom_متن(info->ctx, type->name));
        أرجع false;
    }

    أرجع جوڙيو_Level_Name(info, type, level, level_name, true);
}

/***====================================================================***/

ساكن bool
سيٽ_ڪنجي_Type_ميدان(ڪنجي_Types_Info *info, ڪنجي_Type_Info *type,
                ثابت حرف *ميدان_2, اظهار_Def *arrayNdx, اظهار_Def *قدر)
{
    bool ok = false;
    تعداد type_ميدان type_ميدان = 0;

    إذا (istreq(ميدان_2, "modifiers")) {
        type_ميدان = TYPE_ميدان_نقاب;
        ok = سيٽ_Modifiers(info, type, arrayNdx, قدر);
    }
    وإلا إذا (istreq(ميدان_2, "map")) {
        type_ميدان = TYPE_ميدان_MAP;
        ok = سيٽ_Map_داخلا(info, type, arrayNdx, قدر);
    }
    وإلا إذا (istreq(ميدان_2, "preserve")) {
        type_ميدان = TYPE_ميدان_PRESERVE;
        ok = سيٽ_Preserve(info, type, arrayNdx, قدر);
    }
    وإلا إذا (istreq(ميدان_2, "levelname") || istreq(ميدان_2, "level_name")) {
        type_ميدان = TYPE_ميدان_LEVEL_NAME;
        ok = سيٽ_Level_Name(info, type, arrayNdx, قدر);
    } وإلا {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_ميدان,
                "Unknown field \"%s\" in key type \"%s\"; Definition ignored\n",
                ميدان_2, TypeTxt(info, type));
        ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_TYPE_FIELDS);
    }

    type->defined |= type_ميدان;
    أرجع ok;
}

ساكن bool
Handle_ڪنجي_Type_Body(ڪنجي_Types_Info *info, VarDef *def, ڪنجي_Type_Info *type)
{
    bool ok = true;
    ثابت حرف *elem, *ميدان_2;
    اظهار_Def *arrayNdx;

    لكل (; def; def = (VarDef *) def->common.ايندڙ) {
        إذا (!اظهار_Resolve_Lhs(info->ctx, def->name, &elem, &ميدان_2,
                            &arrayNdx)) {
            /* internal error, already reported */
            ok = false;
            استمر;
        }

        إذا (elem) {
            إذا (istreq(elem, "type")) {
                log_err(info->ctx, XKB_ERROR_INVALID_سيٽ_DEFAULT_STATEMENT,
                        "Support for changing the default type has been removed; "
                        "Statement \"%s.%s\" ignored.\n", elem, ميدان_2);
            }
            وإلا {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key type statement: move statements to the global "
                        "file scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, ميدان_2);
                ok = false;
            }
            استمر;
        }

        إذا (!سيٽ_ڪنجي_Type_ميدان(info, type, ميدان_2, arrayNdx, def->قدر))
            ok = false;
    }

    أرجع ok;
}

ساكن bool
Handle_ڪنجي_Type_Def(ڪنجي_Types_Info *info, ڪنجي_Type_Def *def)
{
    ڪنجي_Type_Info type = {
        .defined = 0,
        .ملايو = def->ملايو,
        .name = def->name,
        .mods = 0,
        .انگ_سطحون = 1,
        .entries = darray_new(),
        .level_نالا = darray_new(),
    };

    إذا (!Handle_ڪنجي_Type_Body(info, def->body, &type) ||
        !جوڙيو_ڪنجي_Type(info, &type, true))
    {
        info->error_ڳڻپ++;
        Clear_ڪنجي_Type_Info(&type);
        أرجع false;
    }

    /* Type has been either stolen via shallow copy or reinitialized in
       `AddKeyType`: no need to free the arrays */
    أرجع true;
}

ساكن bool
HandleGlobalVar(ڪنجي_Types_Info *info, VarDef *stmt)
{
    ثابت حرف *elem, *ميدان_2;
    اظهار_Def *arrayNdx;

    إذا (!اظهار_Resolve_Lhs(info->ctx, stmt->name, &elem, &ميدان_2,
                        &arrayNdx)) {
        أرجع false;           /* internal error, already reported */
    }
    وإلا إذا (elem && istreq(elem, "type")) {
        log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                "Support for changing the default type has been removed; "
                "Statement ignored\n");
        أرجع true;
    }
    وإلا إذا (elem) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_ميدان,
                "Default defined for unknown element \"%s\"; "
                "Value for field \"%s.%s\" ignored\n", elem, elem, ميدان_2);
        أرجع !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
    }
    وإلا إذا (ميدان_2) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_ميدان,
                "Default defined for unknown field \"%s\"; Ignored\n", ميدان_2);
        أرجع !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS);
    }
    أرجع false;
}

ساكن فراغ
Handle_ڪنجي_Types_File(ڪنجي_Types_Info *info, XkbFile *file)
{
    bool ok;

    حرر(info->name);
    info->name = strdup_محفوظ(file->name);

    لكل (ParseCommon *stmt = file->defs; stmt; stmt = stmt->ايندڙ) {
        بدل (stmt->type) {
        حالة STMT_INCLUDE:
            ok = Handle_Include_ڪنجي_Types(info, (IncludeStmt *) stmt);
            اكسر;
        حالة STMT_TYPE:
            ok = Handle_ڪنجي_Type_Def(info, (ڪنجي_Type_Def *) stmt);
            اكسر;
        حالة STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef*) stmt);
            اكسر;
        حالة STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            اكسر;
        حالة STMT_UNKNOWN_DECLARATION:
        حالة STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported types %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            اكسر;
        افتراضي:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Key type files may not include other declarations; "
                    "Ignoring %s\n", stmt_type_منزل_string(stmt->type));
            ok = false;
            اكسر;
        }

        إذا (!ok)
            info->error_ڳڻپ++;

        إذا (info->error_ڳڻپ > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning keytypes file \"%s\"\n",
                    محفوظ_map_name(file->name));
            اكسر;
        }
    }
}

/***====================================================================***/

ساكن bool
Copy_ڪنجي_Types_منزل_Keymap(بنية xkb_keymap *keymap, ڪنجي_Types_Info *info)
{
    ثابت darray_سائيز_t انگ_types = darray_خالي_جايون(info->types)
                                  ? 1
                                  : darray_سائيز(info->types);
    بنية xkb_ڪنجي_type *types = صفر_يادگيري_ڏيو(انگ_types, حجم(*types));
    إذا (!types)
        أرجع false;

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
    إذا (darray_خالي_جايون(info->types)) {
        بنية xkb_ڪنجي_type *type = &types[0];

        type->mods.mods = 0;
        type->انگ_سطحون = 1;
        type->entries = عدم;
        type->انگ_entries = 0;
        type->name = xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL");
        type->level_نالا = عدم;
        type->انگ_level_نالا = 0;
        type->required = true;
    }
    وإلا {
        /* HACK to circumvent Xorg bugs (see comment above) */
        ثابت xkb_atom_t canonical_types[] = {
            xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "TWO_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "ALPHABETIC"),
            xkb_atom_intern_literal(keymap->ctx, "KEYPAD"),
        };

        لكل (darray_سائيز_t i = 0; i < انگ_types; i++) {
            ڪنجي_Type_Info *def = &darray_شي(info->types, i);
            بنية xkb_ڪنجي_type *type = &types[i];

            type->name = def->name;
            type->mods.mods = def->mods;
            type->انگ_سطحون = def->انگ_سطحون;
            type->انگ_level_نالا =
                (xkb_level_اشاريو_t) darray_سائيز(def->level_نالا);
            darray_steal(def->level_نالا, &type->level_نالا, عدم);
            darray_steal(def->entries, &type->entries, &type->انگ_entries);
            type->required = false;

            /* HACK: Never drop canonical XKB key types (see comment above) */
            إذا (type->انگ_سطحون <= 2) {
                لكل (uint8_t t = 0;
                     t < (uint8_t) ARRAY_سائيز(canonical_types);
                     t++) {
                        إذا (type->name == canonical_types[t]) {
                            type->required = true;
                            اكسر;
                        }
                }
            }
        }
    }

    keymap->types_section_name = strdup_محفوظ(info->name);
    XkbEscapeMapName(keymap->types_section_name);
    keymap->انگ_types = انگ_types;
    keymap->types = types;
    keymap->mods = info->mods;
    أرجع true;
}

/***====================================================================***/

bool
Compile_ڪنجي_Types(XkbFile *file, بنية xkb_keymap_info *keymap_info)
{
    ڪنجي_Types_Info info;

    Init_ڪنجي_Types_Info(&info, keymap_info, 0,
                     &keymap_info->keymap.mods);

    إذا (file != عدم)
        Handle_ڪنجي_Types_File(&info, file);

    إذا (info.error_ڳڻپ != 0)
        اذهب err_info;

    إذا (!Copy_ڪنجي_Types_منزل_Keymap(&keymap_info->keymap, &info))
        اذهب err_info;

    Clear_ڪنجي_Types_Info(&info);
    أرجع true;

err_info:
    Clear_ڪنجي_Types_Info(&info);
    أرجع false;
}
