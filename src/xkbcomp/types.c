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

تعداد type_فیلد {
    TYPE_فیلد_ماسک = (1 << 0),
    TYPE_فیلد_MAP = (1 << 1),
    TYPE_فیلد_PRESERVE = (1 << 2),
    TYPE_فیلد_LEVEL_NAME = (1 << 3),
};

تعريفنوع بنية {
    تعداد type_فیلد defined;
    تعداد ادغام_نما ادغام;

    xkb_atom_t name;
    xkb_mod_ماسک_t mods;
    xkb_level_شاخص_t رقم_سطوح;
    darray(بنية xkb_کلید_type_مدخل) entries;
    darray(xkb_atom_t) level_نامها;
} کلید_Type_Info;

تعريفنوع بنية {
    حرف *name;
    صحيح error_شمار;
    بیعلامت صحيح include_عمق;

    darray(کلید_Type_Info) types;
    بنية xkb_mod_مجموعه mods;

    بنية xkb_context *ctx;
    ثابت بنية xkb_keymap_info *keymap_info;
} کلید_Types_Info;

/***====================================================================***/

ساكن مضمن ثابت حرف *
Map_مدخل_Txt(کلید_Types_Info *info, بنية xkb_کلید_type_مدخل *مدخل)
{
    أرجع Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods, مدخل->mods.mods);
}

ساكن مضمن ثابت حرف *
TypeTxt(کلید_Types_Info *info, کلید_Type_Info *type)
{
    أرجع xkb_atom_متن(info->ctx, type->name);
}

ساكن مضمن ثابت حرف *
Type_ماسک_Txt(کلید_Types_Info *info, کلید_Type_Info *type)
{
    أرجع Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods, type->mods);
}

ساكن مضمن bool
ReportTypeShouldBeArray(کلید_Types_Info *info, کلید_Type_Info *type,
                        ثابت حرف *فیلد_2)
{
    أرجع ReportShouldBeArray(info->ctx, "key type", فیلد_2,
                               TypeTxt(info, type));
}

ساكن مضمن bool
ReportTypeBadType(کلید_Types_Info *info, تعداد xkb_پیام_کد کد,
                  کلید_Type_Info *type, ثابت حرف *فیلد_2, ثابت حرف *wanted)
{
    أرجع ReportBadType(info->ctx, کد, "key type", فیلد_2,
                         TypeTxt(info, type), wanted);
}

/***====================================================================***/

ساكن فراغ
Init_کلید_Types_Info(کلید_Types_Info *info, ثابت بنية xkb_keymap_info *keymap_info,
                 بیعلامت صحيح include_عمق,
                 ثابت بنية xkb_mod_مجموعه *mods)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_عمق = include_عمق;
    InitVMods(&info->mods, mods, include_عمق > 0);
}

ساكن فراغ
Clear_کلید_Type_Info(کلید_Type_Info *type)
{
    darray_free(type->entries);
    darray_free(type->level_نامها);
}

ساكن فراغ
Clear_کلید_Types_Info(کلید_Types_Info *info)
{
    حرر(info->name);
    کلید_Type_Info *type;
    darray_foreach(type, info->types)
        Clear_کلید_Type_Info(type);
    darray_free(info->types);
}

ساكن کلید_Type_Info *
پیدا_کن_Matching_کلید_Type(کلید_Types_Info *info, xkb_atom_t name)
{
    کلید_Type_Info *old;

    darray_foreach(old, info->types)
        إذا (old->name == name)
            أرجع old;

    أرجع عدم;
}

ساكن bool
جمع_کن_کلید_Type(کلید_Types_Info *info, کلید_Type_Info *new, bool same_file)
{
    کلید_Type_Info *old;
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);

    old = پیدا_کن_Matching_کلید_Type(info, new->name);
    إذا (old) {
        إذا (new->ادغام != ادغام_AUGMENT) {
            إذا ((same_file && verbosity > 0) || verbosity > 9) {
                log_warn(info->ctx,
                         XKB_WARNING_CONFLICTING_کلید_TYPE_DEFINITIONS,
                         "Multiple definitions of the %s key type; "
                         "Earlier definition ignored\n",
                         xkb_atom_متن(info->ctx, new->name));
            }

            Clear_کلید_Type_Info(old);
            *old = *new;
            darray_init(new->entries);
            darray_init(new->level_نامها);
            أرجع true;
        }

        إذا (same_file)
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_CONFLICTING_کلید_TYPE_DEFINITIONS,
                    "Multiple definitions of the %s key type; "
                    "Later definition ignored\n",
                    xkb_atom_متن(info->ctx, new->name));

        Clear_کلید_Type_Info(new);
        أرجع true;
    }

    darray_append(info->types, *new);
    أرجع true;
}

/***====================================================================***/

ساكن فراغ
ادغام_Included_کلید_Types(کلید_Types_Info *into, کلید_Types_Info *from,
                      تعداد ادغام_نما ادغام)
{
    إذا (from->error_شمار > 0) {
        into->error_شمار += from->error_شمار;
        أرجع;
    }

    ادغام_Mod_Sets(into->ctx, &into->mods, &from->mods, ادغام);

    إذا (into->name == عدم) {
        into->name = steal(&from->name);
    }

    إذا (darray_خالی(into->types)) {
        into->types = from->types;
        /* Types stolen via shallow copy, so reinitialize the array */
        darray_init(from->types);
    }
    وإلا {
        کلید_Type_Info *type;
        darray_foreach(type, from->types) {
            type->ادغام = ادغام;
            إذا (!جمع_کن_کلید_Type(into, type, false))
                into->error_شمار++;
        }
        /* Types were either shallow copied or reinitialized individually
           in `AddKeyType`, so we only need to free the array */
        darray_free(from->types);
    }
}

ساكن فراغ
Handle_کلید_Types_File(کلید_Types_Info *info, XkbFile *file);

ساكن bool
Handle_Include_کلید_Types(کلید_Types_Info *info, IncludeStmt *include)
{
    کلید_Types_Info included;

    إذا (Exceeds_Include_Max_عمق(info->ctx, info->include_عمق)) {
        info->error_شمار += 10;
        أرجع false;
    }

    Init_کلید_Types_Info(&included, info->keymap_info, info->include_عمق + 1,
                     &info->mods);
    included.name = steal(&include->stmt);

    ثابت بنية parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    لكل (IncludeStmt *stmt = include; stmt; stmt = stmt->بعدی_incl) {
        کلید_Types_Info بعدی_incl;
        XkbFile *file;

        حرف مسیر[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_TYPES,
                                  مسیر, حجم(مسیر));
        إذا (!file) {
            info->error_شمار += 10;
            Clear_کلید_Types_Info(&included);
            أرجع false;
        }

        Init_کلید_Types_Info(&بعدی_incl, info->keymap_info, info->include_عمق + 1,
                         &included.mods);

        Handle_کلید_Types_File(&بعدی_incl, file);

        ادغام_Included_کلید_Types(&included, &بعدی_incl, stmt->ادغام);

        Clear_کلید_Types_Info(&بعدی_incl);
        FreeXkbFile(file);
    }

    ادغام_Included_کلید_Types(info, &included, include->ادغام);
    Clear_کلید_Types_Info(&included);

    أرجع (info->error_شمار == 0);
}

/***====================================================================***/

ساكن bool
مجموعه_Modifiers(کلید_Types_Info *info, کلید_Type_Info *type, عبارت_Def *arrayNdx,
             عبارت_Def *مقدار)
{
    xkb_mod_ماسک_t mods = 0;

    إذا (arrayNdx) {
        log_err(info->ctx, XKB_LOG_پیام_NO_ID,
                "The modifiers field of a key type is not an array; "
                "Illegal array subscript ignored\n");
        أرجع false;
    }

    إذا (!عبارت_Resolve_Mod_ماسک(info->ctx, مقدار, MOD_BOTH, &info->mods, &mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک,
                "Key type mask field must be a modifier mask; "
                "Key type definition ignored\n");
        أرجع false;
    }

    إذا (type->defined & TYPE_فیلد_ماسک) {
        log_warn(info->ctx, XKB_LOG_پیام_NO_ID,
                 "Multiple modifier mask definitions for key type %s; "
                 "Using %s, ignoring %s\n",
                 xkb_atom_متن(info->ctx, type->name),
                 Type_ماسک_Txt(info, type),
                 Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods, mods));
        أرجع false;
    }

    type->mods = mods;
    أرجع true;
}

/***====================================================================***/

ساكن بنية xkb_کلید_type_مدخل *
پیدا_کن_Matching_Map_مدخل(کلید_Type_Info *type, xkb_mod_ماسک_t mods)
{
    بنية xkb_کلید_type_مدخل *مدخل;

    darray_foreach(مدخل, type->entries)
        إذا (مدخل->mods.mods == mods)
            أرجع مدخل;

    أرجع عدم;
}

ساكن bool
جمع_کن_Map_مدخل(کلید_Types_Info *info, کلید_Type_Info *type,
            بنية xkb_کلید_type_مدخل *new, bool clobber, bool report)
{
    بنية xkb_کلید_type_مدخل *old;

    old = پیدا_کن_Matching_Map_مدخل(type, new->mods.mods);
    إذا (old) {
        إذا (report && old->level != new->level) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_TYPE_MAP_مدخل,
                     "Multiple map entries for %s in %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     Map_مدخل_Txt(info, new), TypeTxt(info, type),
                     (clobber ? new->level : old->level) + 1,
                     (clobber ? old->level : new->level) + 1);
        }
        وإلا {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_CONFLICTING_کلید_TYPE_MAP_مدخل,
                    "Multiple occurrences of map[%s]= %"PRIu32" in %s; Ignored\n",
                    Map_مدخل_Txt(info, new), new->level + 1,
                    TypeTxt(info, type));
            أرجع true;
        }

        إذا (clobber) {
            إذا (new->level >= type->رقم_سطوح)
                type->رقم_سطوح = new->level + 1;
            old->level = new->level;
        }

        أرجع true;
    }

    إذا (new->level >= type->رقم_سطوح)
        type->رقم_سطوح = new->level + 1;

    darray_append(type->entries, *new);
    أرجع true;
}

ساكن bool
مجموعه_Map_مدخل(کلید_Types_Info *info, کلید_Type_Info *type, عبارت_Def *arrayNdx,
            عبارت_Def *مقدار)
{
    بنية xkb_کلید_type_مدخل مدخل;

    إذا (arrayNdx == عدم)
        أرجع ReportTypeShouldBeArray(info, type, "map entry");

    إذا (!عبارت_Resolve_Mod_ماسک(info->ctx, arrayNdx, MOD_BOTH, &info->mods,
                            &مدخل.mods.mods))
        أرجع ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک_2,
                                 type, "map entry", "modifier mask");

    إذا (مدخل.mods.mods & (~type->mods)) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_درون_کلید_TYPE,
                "Map entry for modifiers not used by type %s; "
                "Using %s instead of %s\n",
                TypeTxt(info, type),
                Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods,
                            مدخل.mods.mods & type->mods),
                Map_مدخل_Txt(info, &مدخل));
        مدخل.mods.mods &= type->mods;
    }

    إذا (!عبارت_Resolve_Level(info->ctx, مقدار, &مدخل.level)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_لغزش_LEVEL,
                          "Level specifications in a key type must be integer; "
                          "Ignoring malformed level specification\n");
        أرجع false;
    }

    مدخل.preserve.mods = 0;

    أرجع جمع_کن_Map_مدخل(info, type, &مدخل, true, true);
}

/***====================================================================***/

ساكن bool
جمع_کن_Preserve(کلید_Types_Info *info, کلید_Type_Info *type,
            xkb_mod_ماسک_t mods, xkb_mod_ماسک_t preserve_mods)
{
    بنية xkb_کلید_type_مدخل *مدخل;

    darray_foreach(مدخل, type->entries) {
        إذا (مدخل->mods.mods != mods)
            استمر;

        /* Map exists without previous preserve (or "None"); override. */
        إذا (مدخل->preserve.mods == 0) {
            مدخل->preserve.mods = preserve_mods;
            أرجع true;
        }

        /* Map exists with same preserve; do nothing. */
        إذا (مدخل->preserve.mods == preserve_mods) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_DUPLICATE_مدخل,
                    "Identical definitions for preserve[%s] in %s; "
                    "Ignored\n",
                    Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods, mods),
                    TypeTxt(info, type));
            أرجع true;
        }

        /* Map exists with different preserve; latter wins. */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_کلید_TYPE_PRESERVE_ENTRIES,
                "Multiple definitions for preserve[%s] in %s; "
                "Using %s, ignoring %s\n",
                Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type),
                Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods, preserve_mods),
                Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods,
                            مدخل->preserve.mods));

        مدخل->preserve.mods = preserve_mods;
        أرجع true;
    }

    /*
     * Map does not exist, i.e. preserve[] came before map[].
     * Create a map with the specified mask mapping to Level1. The level
     * may be overridden later with an explicit map[] statement.
     */
    ثابت بنية xkb_کلید_type_مدخل new = {
        .level = 0,
        .mods = { .mods = mods },
        .preserve = { .mods = preserve_mods },
    };
    darray_append(type->entries, new);
    أرجع true;
}

ساكن bool
مجموعه_Preserve(کلید_Types_Info *info, کلید_Type_Info *type, عبارت_Def *arrayNdx,
            عبارت_Def *مقدار)
{

    إذا (arrayNdx == عدم)
        أرجع ReportTypeShouldBeArray(info, type, "preserve entry");

    xkb_mod_ماسک_t mods = 0;
    إذا (!عبارت_Resolve_Mod_ماسک(info->ctx, arrayNdx, MOD_BOTH, &info->mods, &mods))
        أرجع ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک_2,
                                 type, "preserve entry", "modifier mask");

    إذا (mods & ~type->mods) {
        ثابت حرف *before, *after;

        before = Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods, mods);
        mods &= type->mods;
        after = Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods, mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_درون_کلید_TYPE,
                "Preserve entry for modifiers not used by the %s type; "
                "Index %s converted to %s\n",
                TypeTxt(info, type), before, after);
    }

    xkb_mod_ماسک_t preserve_mods = 0;
    إذا (!عبارت_Resolve_Mod_ماسک(info->ctx, مقدار, MOD_BOTH, &info->mods,
                            &preserve_mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک,
                "Preserve value in a key type is not a modifier mask; "
                "Ignoring preserve[%s] in type %s\n",
                Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type));
        أرجع false;
    }

    إذا (preserve_mods & ~mods) {
        ثابت حرف *before, *after;

        before = Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods, preserve_mods);
        preserve_mods &= mods;
        after = Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods, preserve_mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_ILLEGAL_کلید_TYPE_PRESERVE_نتیجه,
                "Illegal value for preserve[%s] in type %s; "
                "Converted %s to %s\n",
                Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type), before, after);
    }

    أرجع جمع_کن_Preserve(info, type, mods, preserve_mods);
}

/***====================================================================***/

ساكن bool
جمع_کن_Level_Name(کلید_Types_Info *info, کلید_Type_Info *type,
             xkb_level_شاخص_t level, xkb_atom_t name, bool clobber)
{
    /* New name. */
    إذا (level >= darray_اندازه(type->level_نامها)) {
        darray_resize0(type->level_نامها, level + 1);
        اذهب پایان;
    }

    /* Same level, same name. */
    إذا (darray_قلم(type->level_نامها, level) == name) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE, XKB_WARNING_DUPLICATE_مدخل,
                "Duplicate names for level %"PRIu32" of key type %s; Ignored\n",
                level + 1, TypeTxt(info, type));
        أرجع true;
    }

    /* Same level, different name. */
    إذا (darray_قلم(type->level_نامها, level) != XKB_ATOM_NONE) {
        ثابت حرف *old, *new;
        old = xkb_atom_متن(info->ctx,
                            darray_قلم(type->level_نامها, level));
        new = xkb_atom_متن(info->ctx, name);
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_کلید_TYPE_LEVEL_نامها,
                "Multiple names for level %"PRIu32" of key type %s; "
                "Using %s, ignoring %s\n",
                level + 1, TypeTxt(info, type),
                (clobber ? new : old), (clobber ? old : new));

        إذا (!clobber)
            أرجع true;
    }

    /* FIXME: What about different level, same name? */

پایان:
    darray_قلم(type->level_نامها, level) = name;
    أرجع true;
}

ساكن bool
مجموعه_Level_Name(کلید_Types_Info *info, کلید_Type_Info *type, عبارت_Def *arrayNdx,
             عبارت_Def *مقدار)
{
    إذا (arrayNdx == عدم)
        أرجع ReportTypeShouldBeArray(info, type, "level name");

    xkb_level_شاخص_t level = 0;
    إذا (!عبارت_Resolve_Level(info->ctx, arrayNdx, &level))
        أرجع ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_لغزش_LEVEL,
                                 type, "level name", "integer");

    xkb_atom_t level_name = XKB_ATOM_NONE;
    إذا (!عبارت_Resolve_String(info->ctx, مقدار, &level_name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_فیلد_TYPE,
                "Non-string name for level %"PRIu32" in key type %s; "
                "Ignoring illegal level name definition\n",
                level + 1, xkb_atom_متن(info->ctx, type->name));
        أرجع false;
    }

    أرجع جمع_کن_Level_Name(info, type, level, level_name, true);
}

/***====================================================================***/

ساكن bool
مجموعه_کلید_Type_فیلد(کلید_Types_Info *info, کلید_Type_Info *type,
                ثابت حرف *فیلد_2, عبارت_Def *arrayNdx, عبارت_Def *مقدار)
{
    bool ok = false;
    تعداد type_فیلد type_فیلد = 0;

    إذا (istreq(فیلد_2, "modifiers")) {
        type_فیلد = TYPE_فیلد_ماسک;
        ok = مجموعه_Modifiers(info, type, arrayNdx, مقدار);
    }
    وإلا إذا (istreq(فیلد_2, "map")) {
        type_فیلد = TYPE_فیلد_MAP;
        ok = مجموعه_Map_مدخل(info, type, arrayNdx, مقدار);
    }
    وإلا إذا (istreq(فیلد_2, "preserve")) {
        type_فیلد = TYPE_فیلد_PRESERVE;
        ok = مجموعه_Preserve(info, type, arrayNdx, مقدار);
    }
    وإلا إذا (istreq(فیلد_2, "levelname") || istreq(فیلد_2, "level_name")) {
        type_فیلد = TYPE_فیلد_LEVEL_NAME;
        ok = مجموعه_Level_Name(info, type, arrayNdx, مقدار);
    } وإلا {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_فیلد,
                "Unknown field \"%s\" in key type \"%s\"; Definition ignored\n",
                فیلد_2, TypeTxt(info, type));
        ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_TYPE_FIELDS);
    }

    type->defined |= type_فیلد;
    أرجع ok;
}

ساكن bool
Handle_کلید_Type_Body(کلید_Types_Info *info, VarDef *def, کلید_Type_Info *type)
{
    bool ok = true;
    ثابت حرف *elem, *فیلد_2;
    عبارت_Def *arrayNdx;

    لكل (; def; def = (VarDef *) def->common.بعدی) {
        إذا (!عبارت_Resolve_Lhs(info->ctx, def->name, &elem, &فیلد_2,
                            &arrayNdx)) {
            /* internal error, already reported */
            ok = false;
            استمر;
        }

        إذا (elem) {
            إذا (istreq(elem, "type")) {
                log_err(info->ctx, XKB_ERROR_INVALID_مجموعه_DEFAULT_STATEMENT,
                        "Support for changing the default type has been removed; "
                        "Statement \"%s.%s\" ignored.\n", elem, فیلد_2);
            }
            وإلا {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key type statement: move statements to the global "
                        "file scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, فیلد_2);
                ok = false;
            }
            استمر;
        }

        إذا (!مجموعه_کلید_Type_فیلد(info, type, فیلد_2, arrayNdx, def->مقدار))
            ok = false;
    }

    أرجع ok;
}

ساكن bool
Handle_کلید_Type_Def(کلید_Types_Info *info, کلید_Type_Def *def)
{
    کلید_Type_Info type = {
        .defined = 0,
        .ادغام = def->ادغام,
        .name = def->name,
        .mods = 0,
        .رقم_سطوح = 1,
        .entries = darray_new(),
        .level_نامها = darray_new(),
    };

    إذا (!Handle_کلید_Type_Body(info, def->body, &type) ||
        !جمع_کن_کلید_Type(info, &type, true))
    {
        info->error_شمار++;
        Clear_کلید_Type_Info(&type);
        أرجع false;
    }

    /* Type has been either stolen via shallow copy or reinitialized in
       `AddKeyType`: no need to free the arrays */
    أرجع true;
}

ساكن bool
HandleGlobalVar(کلید_Types_Info *info, VarDef *stmt)
{
    ثابت حرف *elem, *فیلد_2;
    عبارت_Def *arrayNdx;

    إذا (!عبارت_Resolve_Lhs(info->ctx, stmt->name, &elem, &فیلد_2,
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
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_فیلد,
                "Default defined for unknown element \"%s\"; "
                "Value for field \"%s.%s\" ignored\n", elem, elem, فیلد_2);
        أرجع !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
    }
    وإلا إذا (فیلد_2) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_فیلد,
                "Default defined for unknown field \"%s\"; Ignored\n", فیلد_2);
        أرجع !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS);
    }
    أرجع false;
}

ساكن فراغ
Handle_کلید_Types_File(کلید_Types_Info *info, XkbFile *file)
{
    bool ok;

    حرر(info->name);
    info->name = strdup_امن(file->name);

    لكل (ParseCommon *stmt = file->defs; stmt; stmt = stmt->بعدی) {
        بدل (stmt->type) {
        حالة STMT_INCLUDE:
            ok = Handle_Include_کلید_Types(info, (IncludeStmt *) stmt);
            اكسر;
        حالة STMT_TYPE:
            ok = Handle_کلید_Type_Def(info, (کلید_Type_Def *) stmt);
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
                    "Ignoring %s\n", stmt_type_مقصد_string(stmt->type));
            ok = false;
            اكسر;
        }

        إذا (!ok)
            info->error_شمار++;

        إذا (info->error_شمار > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning keytypes file \"%s\"\n",
                    امن_map_name(file->name));
            اكسر;
        }
    }
}

/***====================================================================***/

ساكن bool
Copy_کلید_Types_مقصد_Keymap(بنية xkb_keymap *keymap, کلید_Types_Info *info)
{
    ثابت darray_اندازه_t رقم_types = darray_خالی(info->types)
                                  ? 1
                                  : darray_اندازه(info->types);
    بنية xkb_کلید_type *types = حافظه_صفر_بده(رقم_types, حجم(*types));
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
    إذا (darray_خالی(info->types)) {
        بنية xkb_کلید_type *type = &types[0];

        type->mods.mods = 0;
        type->رقم_سطوح = 1;
        type->entries = عدم;
        type->رقم_entries = 0;
        type->name = xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL");
        type->level_نامها = عدم;
        type->رقم_level_نامها = 0;
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

        لكل (darray_اندازه_t i = 0; i < رقم_types; i++) {
            کلید_Type_Info *def = &darray_قلم(info->types, i);
            بنية xkb_کلید_type *type = &types[i];

            type->name = def->name;
            type->mods.mods = def->mods;
            type->رقم_سطوح = def->رقم_سطوح;
            type->رقم_level_نامها =
                (xkb_level_شاخص_t) darray_اندازه(def->level_نامها);
            darray_steal(def->level_نامها, &type->level_نامها, عدم);
            darray_steal(def->entries, &type->entries, &type->رقم_entries);
            type->required = false;

            /* HACK: Never drop canonical XKB key types (see comment above) */
            إذا (type->رقم_سطوح <= 2) {
                لكل (uint8_t t = 0;
                     t < (uint8_t) ARRAY_اندازه(canonical_types);
                     t++) {
                        إذا (type->name == canonical_types[t]) {
                            type->required = true;
                            اكسر;
                        }
                }
            }
        }
    }

    keymap->types_section_name = strdup_امن(info->name);
    XkbEscapeMapName(keymap->types_section_name);
    keymap->رقم_types = رقم_types;
    keymap->types = types;
    keymap->mods = info->mods;
    أرجع true;
}

/***====================================================================***/

bool
Compile_کلید_Types(XkbFile *file, بنية xkb_keymap_info *keymap_info)
{
    کلید_Types_Info info;

    Init_کلید_Types_Info(&info, keymap_info, 0,
                     &keymap_info->keymap.mods);

    إذا (file != عدم)
        Handle_کلید_Types_File(&info, file);

    إذا (info.error_شمار != 0)
        اذهب err_info;

    إذا (!Copy_کلید_Types_مقصد_Keymap(&keymap_info->keymap, &info))
        اذهب err_info;

    Clear_کلید_Types_Info(&info);
    أرجع true;

err_info:
    Clear_کلید_Types_Info(&info);
    أرجع false;
}
