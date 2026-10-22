/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#شامل "config.h"

#شامل <limits.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "xkbcommon/xkbcommon-status.h"
#شامل "xkbcomp-priv.h"
#شامل "text.h"
#شامل "vmod.h"
#شامل "expr.h"
#شامل "include.h"
#شامل "utils.h"
#شامل "util-mem.h"

شمار type_میدان {
    TYPE_میدان_نقاب = (1 << 0),
    TYPE_میدان_MAP = (1 << 1),
    TYPE_میدان_PRESERVE = (1 << 2),
    TYPE_میدان_LEVEL_NAME = (1 << 3),
};

قسمتعریف ساخت {
    شمار type_میدان defined;
    شمار ملاؤ_منوال ملاؤ;

    xkb_atom_t name;
    xkb_mod_نقاب_t mods;
    xkb_level_اشاریہ_t عدد_سطحیں;
    darray(ساخت xkb_کلید_type_اندراج) entries;
    darray(xkb_atom_t) level_نام;
} کلید_Type_Info;

قسمتعریف ساخت {
    حرف *name;
    صحیح error_گنتی;
    غیرعلامتی صحیح include_گہرائی;

    darray(کلید_Type_Info) types;
    ساخت xkb_mod_سیٹ mods;

    ساخت xkb_context *ctx;
    ثابت ساخت xkb_keymap_info *keymap_info;
} کلید_Types_Info;

/***====================================================================***/

ساکن اندرونی ثابت حرف *
Map_اندراج_Txt(کلید_Types_Info *info, ساخت xkb_کلید_type_اندراج *اندراج_2)
{
    واپس Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, اندراج_2->mods.mods);
}

ساکن اندرونی ثابت حرف *
TypeTxt(کلید_Types_Info *info, کلید_Type_Info *type)
{
    واپس xkb_atom_متن(info->ctx, type->name);
}

ساکن اندرونی ثابت حرف *
Type_نقاب_Txt(کلید_Types_Info *info, کلید_Type_Info *type)
{
    واپس Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, type->mods);
}

ساکن اندرونی bool
ReportTypeShouldBeArray(کلید_Types_Info *info, کلید_Type_Info *type,
                        ثابت حرف *میدان_2)
{
    واپس ReportShouldBeArray(info->ctx, "key type", میدان_2,
                               TypeTxt(info, type));
}

ساکن اندرونی bool
ReportTypeBadType(کلید_Types_Info *info, شمار xkb_پیغام_رمز رمز,
                  کلید_Type_Info *type, ثابت حرف *میدان_2, ثابت حرف *wanted)
{
    واپس ReportBadType(info->ctx, رمز, "key type", میدان_2,
                         TypeTxt(info, type), wanted);
}

/***====================================================================***/

ساکن خالی
Init_کلید_Types_Info(کلید_Types_Info *info, ثابت ساخت xkb_keymap_info *keymap_info,
                 غیرعلامتی صحیح include_گہرائی,
                 ثابت ساخت xkb_mod_سیٹ *mods)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_گہرائی = include_گہرائی;
    InitVMods(&info->mods, mods, include_گہرائی > 0);
}

ساکن خالی
Clear_کلید_Type_Info(کلید_Type_Info *type)
{
    darray_free(type->entries);
    darray_free(type->level_نام);
}

ساکن خالی
Clear_کلید_Types_Info(کلید_Types_Info *info)
{
    آزاد(info->name);
    کلید_Type_Info *type;
    darray_foreach(type, info->types)
        Clear_کلید_Type_Info(type);
    darray_free(info->types);
}

ساکن کلید_Type_Info *
ڈھونڈو_Matching_کلید_Type(کلید_Types_Info *info, xkb_atom_t name)
{
    کلید_Type_Info *old;

    darray_foreach(old, info->types)
        اگر (old->name == name)
            واپس old;

    واپس تہی;
}

ساکن bool
جوڑو_کلید_Type(کلید_Types_Info *info, کلید_Type_Info *new, bool same_file)
{
    کلید_Type_Info *old;
    ثابت صحیح verbosity = xkb_context_get_log_verbosity(info->ctx);

    old = ڈھونڈو_Matching_کلید_Type(info, new->name);
    اگر (old) {
        اگر (new->ملاؤ != ملاؤ_AUGMENT) {
            اگر ((same_file && verbosity > 0) || verbosity > 9) {
                log_warn(info->ctx,
                         XKB_WARNING_CONFLICTING_کلید_TYPE_DEFINITIONS,
                         "Multiple definitions of the %s key type; "
                         "Earlier definition ignored\n",
                         xkb_atom_متن(info->ctx, new->name));
            }

            Clear_کلید_Type_Info(old);
            *old = *new;
            darray_init(new->entries);
            darray_init(new->level_نام);
            واپس true;
        }

        اگر (same_file)
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_CONFLICTING_کلید_TYPE_DEFINITIONS,
                    "Multiple definitions of the %s key type; "
                    "Later definition ignored\n",
                    xkb_atom_متن(info->ctx, new->name));

        Clear_کلید_Type_Info(new);
        واپس true;
    }

    darray_append(info->types, *new);
    واپس true;
}

/***====================================================================***/

ساکن خالی
ملاؤ_Included_کلید_Types(کلید_Types_Info *into, کلید_Types_Info *from,
                      شمار ملاؤ_منوال ملاؤ)
{
    اگر (from->error_گنتی > 0) {
        into->error_گنتی += from->error_گنتی;
        واپس;
    }

    ملاؤ_Mod_Sets(into->ctx, &into->mods, &from->mods, ملاؤ);

    اگر (into->name == تہی) {
        into->name = steal(&from->name);
    }

    اگر (darray_خالی_خانے(into->types)) {
        into->types = from->types;
        /* Types stolen via shallow copy, so reinitialize the array */
        darray_init(from->types);
    }
    ورنہ {
        کلید_Type_Info *type;
        darray_foreach(type, from->types) {
            type->ملاؤ = ملاؤ;
            اگر (!جوڑو_کلید_Type(into, type, false))
                into->error_گنتی++;
        }
        /* Types were either shallow copied or reinitialized individually
           in `AddKeyType`, so we only need to free the array */
        darray_free(from->types);
    }
}

ساکن خالی
Handle_کلید_Types_File(کلید_Types_Info *info, XkbFile *file);

ساکن bool
Handle_Include_کلید_Types(کلید_Types_Info *info, IncludeStmt *include)
{
    کلید_Types_Info included;

    اگر (Exceeds_Include_Max_گہرائی(info->ctx, info->include_گہرائی)) {
        info->error_گنتی += 10;
        واپس false;
    }

    Init_کلید_Types_Info(&included, info->keymap_info, info->include_گہرائی + 1,
                     &info->mods);
    included.name = steal(&include->stmt);

    ثابت ساخت parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    برائے (IncludeStmt *stmt = include; stmt; stmt = stmt->اگلا_incl) {
        کلید_Types_Info اگلا_incl;
        XkbFile *file;

        حرف راستہ[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_TYPES,
                                  راستہ, حجم(راستہ));
        اگر (!file) {
            info->error_گنتی += 10;
            Clear_کلید_Types_Info(&included);
            واپس false;
        }

        Init_کلید_Types_Info(&اگلا_incl, info->keymap_info, info->include_گہرائی + 1,
                         &included.mods);

        Handle_کلید_Types_File(&اگلا_incl, file);

        ملاؤ_Included_کلید_Types(&included, &اگلا_incl, stmt->ملاؤ);

        Clear_کلید_Types_Info(&اگلا_incl);
        FreeXkbFile(file);
    }

    ملاؤ_Included_کلید_Types(info, &included, include->ملاؤ);
    Clear_کلید_Types_Info(&included);

    واپس (info->error_گنتی == 0);
}

/***====================================================================***/

ساکن bool
سیٹ_Modifiers(کلید_Types_Info *info, کلید_Type_Info *type, اظہاریہ_Def *arrayNdx,
             اظہاریہ_Def *قدر)
{
    xkb_mod_نقاب_t mods = 0;

    اگر (arrayNdx) {
        log_err(info->ctx, XKB_LOG_پیغام_NO_ID,
                "The modifiers field of a key type is not an array; "
                "Illegal array subscript ignored\n");
        واپس false;
    }

    اگر (!اظہاریہ_Resolve_Mod_نقاب(info->ctx, قدر, MOD_BOTH, &info->mods, &mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب,
                "Key type mask field must be a modifier mask; "
                "Key type definition ignored\n");
        واپس false;
    }

    اگر (type->defined & TYPE_میدان_نقاب) {
        log_warn(info->ctx, XKB_LOG_پیغام_NO_ID,
                 "Multiple modifier mask definitions for key type %s; "
                 "Using %s, ignoring %s\n",
                 xkb_atom_متن(info->ctx, type->name),
                 Type_نقاب_Txt(info, type),
                 Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, mods));
        واپس false;
    }

    type->mods = mods;
    واپس true;
}

/***====================================================================***/

ساکن ساخت xkb_کلید_type_اندراج *
ڈھونڈو_Matching_Map_اندراج(کلید_Type_Info *type, xkb_mod_نقاب_t mods)
{
    ساخت xkb_کلید_type_اندراج *اندراج_2;

    darray_foreach(اندراج_2, type->entries)
        اگر (اندراج_2->mods.mods == mods)
            واپس اندراج_2;

    واپس تہی;
}

ساکن bool
جوڑو_Map_اندراج(کلید_Types_Info *info, کلید_Type_Info *type,
            ساخت xkb_کلید_type_اندراج *new, bool clobber, bool report)
{
    ساخت xkb_کلید_type_اندراج *old;

    old = ڈھونڈو_Matching_Map_اندراج(type, new->mods.mods);
    اگر (old) {
        اگر (report && old->level != new->level) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_TYPE_MAP_اندراج,
                     "Multiple map entries for %s in %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     Map_اندراج_Txt(info, new), TypeTxt(info, type),
                     (clobber ? new->level : old->level) + 1,
                     (clobber ? old->level : new->level) + 1);
        }
        ورنہ {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_CONFLICTING_کلید_TYPE_MAP_اندراج,
                    "Multiple occurrences of map[%s]= %"PRIu32" in %s; Ignored\n",
                    Map_اندراج_Txt(info, new), new->level + 1,
                    TypeTxt(info, type));
            واپس true;
        }

        اگر (clobber) {
            اگر (new->level >= type->عدد_سطحیں)
                type->عدد_سطحیں = new->level + 1;
            old->level = new->level;
        }

        واپس true;
    }

    اگر (new->level >= type->عدد_سطحیں)
        type->عدد_سطحیں = new->level + 1;

    darray_append(type->entries, *new);
    واپس true;
}

ساکن bool
سیٹ_Map_اندراج(کلید_Types_Info *info, کلید_Type_Info *type, اظہاریہ_Def *arrayNdx,
            اظہاریہ_Def *قدر)
{
    ساخت xkb_کلید_type_اندراج اندراج_2;

    اگر (arrayNdx == تہی)
        واپس ReportTypeShouldBeArray(info, type, "map entry");

    اگر (!اظہاریہ_Resolve_Mod_نقاب(info->ctx, arrayNdx, MOD_BOTH, &info->mods,
                            &اندراج_2.mods.mods))
        واپس ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب_2,
                                 type, "map entry", "modifier mask");

    اگر (اندراج_2.mods.mods & (~type->mods)) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_اندر_کلید_TYPE,
                "Map entry for modifiers not used by type %s; "
                "Using %s instead of %s\n",
                TypeTxt(info, type),
                Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods,
                            اندراج_2.mods.mods & type->mods),
                Map_اندراج_Txt(info, &اندراج_2));
        اندراج_2.mods.mods &= type->mods;
    }

    اگر (!اظہاریہ_Resolve_Level(info->ctx, قدر, &اندراج_2.level)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_سرکاؤ_LEVEL,
                          "Level specifications in a key type must be integer; "
                          "Ignoring malformed level specification\n");
        واپس false;
    }

    اندراج_2.preserve.mods = 0;

    واپس جوڑو_Map_اندراج(info, type, &اندراج_2, true, true);
}

/***====================================================================***/

ساکن bool
جوڑو_Preserve(کلید_Types_Info *info, کلید_Type_Info *type,
            xkb_mod_نقاب_t mods, xkb_mod_نقاب_t preserve_mods)
{
    ساخت xkb_کلید_type_اندراج *اندراج_2;

    darray_foreach(اندراج_2, type->entries) {
        اگر (اندراج_2->mods.mods != mods)
            جاری;

        /* Map exists without previous preserve (or "None"); override. */
        اگر (اندراج_2->preserve.mods == 0) {
            اندراج_2->preserve.mods = preserve_mods;
            واپس true;
        }

        /* Map exists with same preserve; do nothing. */
        اگر (اندراج_2->preserve.mods == preserve_mods) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_DUPLICATE_اندراج,
                    "Identical definitions for preserve[%s] in %s; "
                    "Ignored\n",
                    Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, mods),
                    TypeTxt(info, type));
            واپس true;
        }

        /* Map exists with different preserve; latter wins. */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_کلید_TYPE_PRESERVE_ENTRIES,
                "Multiple definitions for preserve[%s] in %s; "
                "Using %s, ignoring %s\n",
                Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type),
                Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, preserve_mods),
                Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods,
                            اندراج_2->preserve.mods));

        اندراج_2->preserve.mods = preserve_mods;
        واپس true;
    }

    /*
     * Map does not exist, i.e. preserve[] came before map[].
     * Create a map with the specified mask mapping to Level1. The level
     * may be overridden later with an explicit map[] statement.
     */
    ثابت ساخت xkb_کلید_type_اندراج new = {
        .level = 0,
        .mods = { .mods = mods },
        .preserve = { .mods = preserve_mods },
    };
    darray_append(type->entries, new);
    واپس true;
}

ساکن bool
سیٹ_Preserve(کلید_Types_Info *info, کلید_Type_Info *type, اظہاریہ_Def *arrayNdx,
            اظہاریہ_Def *قدر)
{

    اگر (arrayNdx == تہی)
        واپس ReportTypeShouldBeArray(info, type, "preserve entry");

    xkb_mod_نقاب_t mods = 0;
    اگر (!اظہاریہ_Resolve_Mod_نقاب(info->ctx, arrayNdx, MOD_BOTH, &info->mods, &mods))
        واپس ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب_2,
                                 type, "preserve entry", "modifier mask");

    اگر (mods & ~type->mods) {
        ثابت حرف *before, *after;

        before = Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, mods);
        mods &= type->mods;
        after = Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_اندر_کلید_TYPE,
                "Preserve entry for modifiers not used by the %s type; "
                "Index %s converted to %s\n",
                TypeTxt(info, type), before, after);
    }

    xkb_mod_نقاب_t preserve_mods = 0;
    اگر (!اظہاریہ_Resolve_Mod_نقاب(info->ctx, قدر, MOD_BOTH, &info->mods,
                            &preserve_mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب,
                "Preserve value in a key type is not a modifier mask; "
                "Ignoring preserve[%s] in type %s\n",
                Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type));
        واپس false;
    }

    اگر (preserve_mods & ~mods) {
        ثابت حرف *before, *after;

        before = Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, preserve_mods);
        preserve_mods &= mods;
        after = Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, preserve_mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_ILLEGAL_کلید_TYPE_PRESERVE_نتیجہ,
                "Illegal value for preserve[%s] in type %s; "
                "Converted %s to %s\n",
                Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type), before, after);
    }

    واپس جوڑو_Preserve(info, type, mods, preserve_mods);
}

/***====================================================================***/

ساکن bool
جوڑو_Level_Name(کلید_Types_Info *info, کلید_Type_Info *type,
             xkb_level_اشاریہ_t level, xkb_atom_t name, bool clobber)
{
    /* New name. */
    اگر (level >= darray_سائز(type->level_نام)) {
        darray_resize0(type->level_نام, level + 1);
        جاؤ اختتام;
    }

    /* Same level, same name. */
    اگر (darray_شے(type->level_نام, level) == name) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE, XKB_WARNING_DUPLICATE_اندراج,
                "Duplicate names for level %"PRIu32" of key type %s; Ignored\n",
                level + 1, TypeTxt(info, type));
        واپس true;
    }

    /* Same level, different name. */
    اگر (darray_شے(type->level_نام, level) != XKB_ATOM_NONE) {
        ثابت حرف *old, *new;
        old = xkb_atom_متن(info->ctx,
                            darray_شے(type->level_نام, level));
        new = xkb_atom_متن(info->ctx, name);
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_کلید_TYPE_LEVEL_نام,
                "Multiple names for level %"PRIu32" of key type %s; "
                "Using %s, ignoring %s\n",
                level + 1, TypeTxt(info, type),
                (clobber ? new : old), (clobber ? old : new));

        اگر (!clobber)
            واپس true;
    }

    /* FIXME: What about different level, same name? */

اختتام:
    darray_شے(type->level_نام, level) = name;
    واپس true;
}

ساکن bool
سیٹ_Level_Name(کلید_Types_Info *info, کلید_Type_Info *type, اظہاریہ_Def *arrayNdx,
             اظہاریہ_Def *قدر)
{
    اگر (arrayNdx == تہی)
        واپس ReportTypeShouldBeArray(info, type, "level name");

    xkb_level_اشاریہ_t level = 0;
    اگر (!اظہاریہ_Resolve_Level(info->ctx, arrayNdx, &level))
        واپس ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_سرکاؤ_LEVEL,
                                 type, "level name", "integer");

    xkb_atom_t level_name = XKB_ATOM_NONE;
    اگر (!اظہاریہ_Resolve_String(info->ctx, قدر, &level_name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Non-string name for level %"PRIu32" in key type %s; "
                "Ignoring illegal level name definition\n",
                level + 1, xkb_atom_متن(info->ctx, type->name));
        واپس false;
    }

    واپس جوڑو_Level_Name(info, type, level, level_name, true);
}

/***====================================================================***/

ساکن bool
سیٹ_کلید_Type_میدان(کلید_Types_Info *info, کلید_Type_Info *type,
                ثابت حرف *میدان_2, اظہاریہ_Def *arrayNdx, اظہاریہ_Def *قدر)
{
    bool ok = false;
    شمار type_میدان type_میدان = 0;

    اگر (istreq(میدان_2, "modifiers")) {
        type_میدان = TYPE_میدان_نقاب;
        ok = سیٹ_Modifiers(info, type, arrayNdx, قدر);
    }
    ورنہ اگر (istreq(میدان_2, "map")) {
        type_میدان = TYPE_میدان_MAP;
        ok = سیٹ_Map_اندراج(info, type, arrayNdx, قدر);
    }
    ورنہ اگر (istreq(میدان_2, "preserve")) {
        type_میدان = TYPE_میدان_PRESERVE;
        ok = سیٹ_Preserve(info, type, arrayNdx, قدر);
    }
    ورنہ اگر (istreq(میدان_2, "levelname") || istreq(میدان_2, "level_name")) {
        type_میدان = TYPE_میدان_LEVEL_NAME;
        ok = سیٹ_Level_Name(info, type, arrayNdx, قدر);
    } ورنہ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_میدان,
                "Unknown field \"%s\" in key type \"%s\"; Definition ignored\n",
                میدان_2, TypeTxt(info, type));
        ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_TYPE_FIELDS);
    }

    type->defined |= type_میدان;
    واپس ok;
}

ساکن bool
Handle_کلید_Type_Body(کلید_Types_Info *info, VarDef *def, کلید_Type_Info *type)
{
    bool ok = true;
    ثابت حرف *elem, *میدان_2;
    اظہاریہ_Def *arrayNdx;

    برائے (; def; def = (VarDef *) def->common.اگلا) {
        اگر (!اظہاریہ_Resolve_Lhs(info->ctx, def->name, &elem, &میدان_2,
                            &arrayNdx)) {
            /* internal error, already reported */
            ok = false;
            جاری;
        }

        اگر (elem) {
            اگر (istreq(elem, "type")) {
                log_err(info->ctx, XKB_ERROR_INVALID_سیٹ_DEFAULT_STATEMENT,
                        "Support for changing the default type has been removed; "
                        "Statement \"%s.%s\" ignored.\n", elem, میدان_2);
            }
            ورنہ {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key type statement: move statements to the global "
                        "file scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, میدان_2);
                ok = false;
            }
            جاری;
        }

        اگر (!سیٹ_کلید_Type_میدان(info, type, میدان_2, arrayNdx, def->قدر))
            ok = false;
    }

    واپس ok;
}

ساکن bool
Handle_کلید_Type_Def(کلید_Types_Info *info, کلید_Type_Def *def)
{
    کلید_Type_Info type = {
        .defined = 0,
        .ملاؤ = def->ملاؤ,
        .name = def->name,
        .mods = 0,
        .عدد_سطحیں = 1,
        .entries = darray_new(),
        .level_نام = darray_new(),
    };

    اگر (!Handle_کلید_Type_Body(info, def->body, &type) ||
        !جوڑو_کلید_Type(info, &type, true))
    {
        info->error_گنتی++;
        Clear_کلید_Type_Info(&type);
        واپس false;
    }

    /* Type has been either stolen via shallow copy or reinitialized in
       `AddKeyType`: no need to free the arrays */
    واپس true;
}

ساکن bool
HandleGlobalVar(کلید_Types_Info *info, VarDef *stmt)
{
    ثابت حرف *elem, *میدان_2;
    اظہاریہ_Def *arrayNdx;

    اگر (!اظہاریہ_Resolve_Lhs(info->ctx, stmt->name, &elem, &میدان_2,
                        &arrayNdx)) {
        واپس false;           /* internal error, already reported */
    }
    ورنہ اگر (elem && istreq(elem, "type")) {
        log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                "Support for changing the default type has been removed; "
                "Statement ignored\n");
        واپس true;
    }
    ورنہ اگر (elem) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_میدان,
                "Default defined for unknown element \"%s\"; "
                "Value for field \"%s.%s\" ignored\n", elem, elem, میدان_2);
        واپس !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
    }
    ورنہ اگر (میدان_2) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_میدان,
                "Default defined for unknown field \"%s\"; Ignored\n", میدان_2);
        واپس !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS);
    }
    واپس false;
}

ساکن خالی
Handle_کلید_Types_File(کلید_Types_Info *info, XkbFile *file)
{
    bool ok;

    آزاد(info->name);
    info->name = strdup_محفوظ(file->name);

    برائے (ParseCommon *stmt = file->defs; stmt; stmt = stmt->اگلا) {
        انتخاب (stmt->type) {
        حالت STMT_INCLUDE:
            ok = Handle_Include_کلید_Types(info, (IncludeStmt *) stmt);
            توڑو;
        حالت STMT_TYPE:
            ok = Handle_کلید_Type_Def(info, (کلید_Type_Def *) stmt);
            توڑو;
        حالت STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef*) stmt);
            توڑو;
        حالت STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            توڑو;
        حالت STMT_UNKNOWN_DECLARATION:
        حالت STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported types %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            توڑو;
        طےشدہ:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Key type files may not include other declarations; "
                    "Ignoring %s\n", stmt_type_مقصد_string(stmt->type));
            ok = false;
            توڑو;
        }

        اگر (!ok)
            info->error_گنتی++;

        اگر (info->error_گنتی > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning keytypes file \"%s\"\n",
                    محفوظ_map_name(file->name));
            توڑو;
        }
    }
}

/***====================================================================***/

ساکن bool
Copy_کلید_Types_مقصد_Keymap(ساخت xkb_keymap *keymap, کلید_Types_Info *info)
{
    ثابت darray_سائز_t عدد_types = darray_خالی_خانے(info->types)
                                  ? 1
                                  : darray_سائز(info->types);
    ساخت xkb_کلید_type *types = صفر_حافظہ_دو(عدد_types, حجم(*types));
    اگر (!types)
        واپس false;

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
    اگر (darray_خالی_خانے(info->types)) {
        ساخت xkb_کلید_type *type = &types[0];

        type->mods.mods = 0;
        type->عدد_سطحیں = 1;
        type->entries = تہی;
        type->عدد_entries = 0;
        type->name = xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL");
        type->level_نام = تہی;
        type->عدد_level_نام = 0;
        type->required = true;
    }
    ورنہ {
        /* HACK to circumvent Xorg bugs (see comment above) */
        ثابت xkb_atom_t canonical_types[] = {
            xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "TWO_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "ALPHABETIC"),
            xkb_atom_intern_literal(keymap->ctx, "KEYPAD"),
        };

        برائے (darray_سائز_t i = 0; i < عدد_types; i++) {
            کلید_Type_Info *def = &darray_شے(info->types, i);
            ساخت xkb_کلید_type *type = &types[i];

            type->name = def->name;
            type->mods.mods = def->mods;
            type->عدد_سطحیں = def->عدد_سطحیں;
            type->عدد_level_نام =
                (xkb_level_اشاریہ_t) darray_سائز(def->level_نام);
            darray_steal(def->level_نام, &type->level_نام, تہی);
            darray_steal(def->entries, &type->entries, &type->عدد_entries);
            type->required = false;

            /* HACK: Never drop canonical XKB key types (see comment above) */
            اگر (type->عدد_سطحیں <= 2) {
                برائے (uint8_t t = 0;
                     t < (uint8_t) ARRAY_سائز(canonical_types);
                     t++) {
                        اگر (type->name == canonical_types[t]) {
                            type->required = true;
                            توڑو;
                        }
                }
            }
        }
    }

    keymap->types_section_name = strdup_محفوظ(info->name);
    XkbEscapeMapName(keymap->types_section_name);
    keymap->عدد_types = عدد_types;
    keymap->types = types;
    keymap->mods = info->mods;
    واپس true;
}

/***====================================================================***/

bool
Compile_کلید_Types(XkbFile *file, ساخت xkb_keymap_info *keymap_info)
{
    کلید_Types_Info info;

    Init_کلید_Types_Info(&info, keymap_info, 0,
                     &keymap_info->keymap.mods);

    اگر (file != تہی)
        Handle_کلید_Types_File(&info, file);

    اگر (info.error_گنتی != 0)
        جاؤ err_info;

    اگر (!Copy_کلید_Types_مقصد_Keymap(&keymap_info->keymap, &info))
        جاؤ err_info;

    Clear_کلید_Types_Info(&info);
    واپس true;

err_info:
    Clear_کلید_Types_Info(&info);
    واپس false;
}
