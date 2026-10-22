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

تعداد type_ډګر {
    TYPE_ډګر_ماسک = (1 << 0),
    TYPE_ډګر_MAP = (1 << 1),
    TYPE_ډګر_PRESERVE = (1 << 2),
    TYPE_ډګر_LEVEL_NAME = (1 << 3),
};

تعريفنوع بنية {
    تعداد type_ډګر defined;
    تعداد یوځای_ډېرپېښ یوځای;

    xkb_atom_t name;
    xkb_mod_ماسک_t mods;
    xkb_level_شاخص_t عدد_پوړونه;
    darray(بنية xkb_کلی_type_ثبت) entries;
    darray(xkb_atom_t) level_نومونه;
} کلی_Type_Info;

تعريفنوع بنية {
    حرف *name;
    صحيح error_شمېر;
    بینښه صحيح include_ژوروالی;

    darray(کلی_Type_Info) types;
    بنية xkb_mod_سېټ mods;

    بنية xkb_context *ctx;
    ثابت بنية xkb_keymap_info *keymap_info;
} کلی_Types_Info;

/***====================================================================***/

ساكن مضمن ثابت حرف *
Map_ثبت_Txt(کلی_Types_Info *info, بنية xkb_کلی_type_ثبت *ثبت)
{
    أرجع Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods, ثبت->mods.mods);
}

ساكن مضمن ثابت حرف *
TypeTxt(کلی_Types_Info *info, کلی_Type_Info *type)
{
    أرجع xkb_atom_متن(info->ctx, type->name);
}

ساكن مضمن ثابت حرف *
Type_ماسک_Txt(کلی_Types_Info *info, کلی_Type_Info *type)
{
    أرجع Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods, type->mods);
}

ساكن مضمن bool
ReportTypeShouldBeArray(کلی_Types_Info *info, کلی_Type_Info *type,
                        ثابت حرف *ډګر_2)
{
    أرجع ReportShouldBeArray(info->ctx, "key type", ډګر_2,
                               TypeTxt(info, type));
}

ساكن مضمن bool
ReportTypeBadType(کلی_Types_Info *info, تعداد xkb_پیغام_کوډ کوډ,
                  کلی_Type_Info *type, ثابت حرف *ډګر_2, ثابت حرف *wanted)
{
    أرجع ReportBadType(info->ctx, کوډ, "key type", ډګر_2,
                         TypeTxt(info, type), wanted);
}

/***====================================================================***/

ساكن فراغ
Init_کلی_Types_Info(کلی_Types_Info *info, ثابت بنية xkb_keymap_info *keymap_info,
                 بینښه صحيح include_ژوروالی,
                 ثابت بنية xkb_mod_سېټ *mods)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_ژوروالی = include_ژوروالی;
    InitVMods(&info->mods, mods, include_ژوروالی > 0);
}

ساكن فراغ
Clear_کلی_Type_Info(کلی_Type_Info *type)
{
    darray_free(type->entries);
    darray_free(type->level_نومونه);
}

ساكن فراغ
Clear_کلی_Types_Info(کلی_Types_Info *info)
{
    حرر(info->name);
    کلی_Type_Info *type;
    darray_foreach(type, info->types)
        Clear_کلی_Type_Info(type);
    darray_free(info->types);
}

ساكن کلی_Type_Info *
پیدا_کړه_Matching_کلی_Type(کلی_Types_Info *info, xkb_atom_t name)
{
    کلی_Type_Info *old;

    darray_foreach(old, info->types)
        إذا (old->name == name)
            أرجع old;

    أرجع عدم;
}

ساكن bool
جمع_کړه_کلی_Type(کلی_Types_Info *info, کلی_Type_Info *new, bool same_file)
{
    کلی_Type_Info *old;
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);

    old = پیدا_کړه_Matching_کلی_Type(info, new->name);
    إذا (old) {
        إذا (new->یوځای != یوځای_AUGMENT) {
            إذا ((same_file && verbosity > 0) || verbosity > 9) {
                log_warn(info->ctx,
                         XKB_WARNING_CONFLICTING_کلی_TYPE_DEFINITIONS,
                         "Multiple definitions of the %s key type; "
                         "Earlier definition ignored\n",
                         xkb_atom_متن(info->ctx, new->name));
            }

            Clear_کلی_Type_Info(old);
            *old = *new;
            darray_init(new->entries);
            darray_init(new->level_نومونه);
            أرجع true;
        }

        إذا (same_file)
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_CONFLICTING_کلی_TYPE_DEFINITIONS,
                    "Multiple definitions of the %s key type; "
                    "Later definition ignored\n",
                    xkb_atom_متن(info->ctx, new->name));

        Clear_کلی_Type_Info(new);
        أرجع true;
    }

    darray_append(info->types, *new);
    أرجع true;
}

/***====================================================================***/

ساكن فراغ
یوځای_Included_کلی_Types(کلی_Types_Info *into, کلی_Types_Info *from,
                      تعداد یوځای_ډېرپېښ یوځای)
{
    إذا (from->error_شمېر > 0) {
        into->error_شمېر += from->error_شمېر;
        أرجع;
    }

    یوځای_Mod_Sets(into->ctx, &into->mods, &from->mods, یوځای);

    إذا (into->name == عدم) {
        into->name = steal(&from->name);
    }

    إذا (darray_تشه(into->types)) {
        into->types = from->types;
        /* Types stolen via shallow copy, so reinitialize the array */
        darray_init(from->types);
    }
    وإلا {
        کلی_Type_Info *type;
        darray_foreach(type, from->types) {
            type->یوځای = یوځای;
            إذا (!جمع_کړه_کلی_Type(into, type, false))
                into->error_شمېر++;
        }
        /* Types were either shallow copied or reinitialized individually
           in `AddKeyType`, so we only need to free the array */
        darray_free(from->types);
    }
}

ساكن فراغ
Handle_کلی_Types_File(کلی_Types_Info *info, XkbFile *file);

ساكن bool
Handle_Include_کلی_Types(کلی_Types_Info *info, IncludeStmt *include)
{
    کلی_Types_Info included;

    إذا (Exceeds_Include_Max_ژوروالی(info->ctx, info->include_ژوروالی)) {
        info->error_شمېر += 10;
        أرجع false;
    }

    Init_کلی_Types_Info(&included, info->keymap_info, info->include_ژوروالی + 1,
                     &info->mods);
    included.name = steal(&include->stmt);

    ثابت بنية parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    لكل (IncludeStmt *stmt = include; stmt; stmt = stmt->بل_incl) {
        کلی_Types_Info بل_incl;
        XkbFile *file;

        حرف لار[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_TYPES,
                                  لار, حجم(لار));
        إذا (!file) {
            info->error_شمېر += 10;
            Clear_کلی_Types_Info(&included);
            أرجع false;
        }

        Init_کلی_Types_Info(&بل_incl, info->keymap_info, info->include_ژوروالی + 1,
                         &included.mods);

        Handle_کلی_Types_File(&بل_incl, file);

        یوځای_Included_کلی_Types(&included, &بل_incl, stmt->یوځای);

        Clear_کلی_Types_Info(&بل_incl);
        FreeXkbFile(file);
    }

    یوځای_Included_کلی_Types(info, &included, include->یوځای);
    Clear_کلی_Types_Info(&included);

    أرجع (info->error_شمېر == 0);
}

/***====================================================================***/

ساكن bool
سېټ_Modifiers(کلی_Types_Info *info, کلی_Type_Info *type, بیان_Def *arrayNdx,
             بیان_Def *ارزښت)
{
    xkb_mod_ماسک_t mods = 0;

    إذا (arrayNdx) {
        log_err(info->ctx, XKB_LOG_پیغام_NO_ID,
                "The modifiers field of a key type is not an array; "
                "Illegal array subscript ignored\n");
        أرجع false;
    }

    إذا (!بیان_Resolve_Mod_ماسک(info->ctx, ارزښت, MOD_BOTH, &info->mods, &mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک,
                "Key type mask field must be a modifier mask; "
                "Key type definition ignored\n");
        أرجع false;
    }

    إذا (type->defined & TYPE_ډګر_ماسک) {
        log_warn(info->ctx, XKB_LOG_پیغام_NO_ID,
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

ساكن بنية xkb_کلی_type_ثبت *
پیدا_کړه_Matching_Map_ثبت(کلی_Type_Info *type, xkb_mod_ماسک_t mods)
{
    بنية xkb_کلی_type_ثبت *ثبت;

    darray_foreach(ثبت, type->entries)
        إذا (ثبت->mods.mods == mods)
            أرجع ثبت;

    أرجع عدم;
}

ساكن bool
جمع_کړه_Map_ثبت(کلی_Types_Info *info, کلی_Type_Info *type,
            بنية xkb_کلی_type_ثبت *new, bool clobber, bool report)
{
    بنية xkb_کلی_type_ثبت *old;

    old = پیدا_کړه_Matching_Map_ثبت(type, new->mods.mods);
    إذا (old) {
        إذا (report && old->level != new->level) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلی_TYPE_MAP_ثبت,
                     "Multiple map entries for %s in %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     Map_ثبت_Txt(info, new), TypeTxt(info, type),
                     (clobber ? new->level : old->level) + 1,
                     (clobber ? old->level : new->level) + 1);
        }
        وإلا {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_CONFLICTING_کلی_TYPE_MAP_ثبت,
                    "Multiple occurrences of map[%s]= %"PRIu32" in %s; Ignored\n",
                    Map_ثبت_Txt(info, new), new->level + 1,
                    TypeTxt(info, type));
            أرجع true;
        }

        إذا (clobber) {
            إذا (new->level >= type->عدد_پوړونه)
                type->عدد_پوړونه = new->level + 1;
            old->level = new->level;
        }

        أرجع true;
    }

    إذا (new->level >= type->عدد_پوړونه)
        type->عدد_پوړونه = new->level + 1;

    darray_append(type->entries, *new);
    أرجع true;
}

ساكن bool
سېټ_Map_ثبت(کلی_Types_Info *info, کلی_Type_Info *type, بیان_Def *arrayNdx,
            بیان_Def *ارزښت)
{
    بنية xkb_کلی_type_ثبت ثبت;

    إذا (arrayNdx == عدم)
        أرجع ReportTypeShouldBeArray(info, type, "map entry");

    إذا (!بیان_Resolve_Mod_ماسک(info->ctx, arrayNdx, MOD_BOTH, &info->mods,
                            &ثبت.mods.mods))
        أرجع ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک_2,
                                 type, "map entry", "modifier mask");

    إذا (ثبت.mods.mods & (~type->mods)) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_دننه_کلی_TYPE,
                "Map entry for modifiers not used by type %s; "
                "Using %s instead of %s\n",
                TypeTxt(info, type),
                Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods,
                            ثبت.mods.mods & type->mods),
                Map_ثبت_Txt(info, &ثبت));
        ثبت.mods.mods &= type->mods;
    }

    إذا (!بیان_Resolve_Level(info->ctx, ارزښت, &ثبت.level)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_ښويدنه_LEVEL,
                          "Level specifications in a key type must be integer; "
                          "Ignoring malformed level specification\n");
        أرجع false;
    }

    ثبت.preserve.mods = 0;

    أرجع جمع_کړه_Map_ثبت(info, type, &ثبت, true, true);
}

/***====================================================================***/

ساكن bool
جمع_کړه_Preserve(کلی_Types_Info *info, کلی_Type_Info *type,
            xkb_mod_ماسک_t mods, xkb_mod_ماسک_t preserve_mods)
{
    بنية xkb_کلی_type_ثبت *ثبت;

    darray_foreach(ثبت, type->entries) {
        إذا (ثبت->mods.mods != mods)
            استمر;

        /* Map exists without previous preserve (or "None"); override. */
        إذا (ثبت->preserve.mods == 0) {
            ثبت->preserve.mods = preserve_mods;
            أرجع true;
        }

        /* Map exists with same preserve; do nothing. */
        إذا (ثبت->preserve.mods == preserve_mods) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_DUPLICATE_ثبت,
                    "Identical definitions for preserve[%s] in %s; "
                    "Ignored\n",
                    Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods, mods),
                    TypeTxt(info, type));
            أرجع true;
        }

        /* Map exists with different preserve; latter wins. */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_کلی_TYPE_PRESERVE_ENTRIES,
                "Multiple definitions for preserve[%s] in %s; "
                "Using %s, ignoring %s\n",
                Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type),
                Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods, preserve_mods),
                Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods,
                            ثبت->preserve.mods));

        ثبت->preserve.mods = preserve_mods;
        أرجع true;
    }

    /*
     * Map does not exist, i.e. preserve[] came before map[].
     * Create a map with the specified mask mapping to Level1. The level
     * may be overridden later with an explicit map[] statement.
     */
    ثابت بنية xkb_کلی_type_ثبت new = {
        .level = 0,
        .mods = { .mods = mods },
        .preserve = { .mods = preserve_mods },
    };
    darray_append(type->entries, new);
    أرجع true;
}

ساكن bool
سېټ_Preserve(کلی_Types_Info *info, کلی_Type_Info *type, بیان_Def *arrayNdx,
            بیان_Def *ارزښت)
{

    إذا (arrayNdx == عدم)
        أرجع ReportTypeShouldBeArray(info, type, "preserve entry");

    xkb_mod_ماسک_t mods = 0;
    إذا (!بیان_Resolve_Mod_ماسک(info->ctx, arrayNdx, MOD_BOTH, &info->mods, &mods))
        أرجع ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک_2,
                                 type, "preserve entry", "modifier mask");

    إذا (mods & ~type->mods) {
        ثابت حرف *before, *after;

        before = Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods, mods);
        mods &= type->mods;
        after = Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods, mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_دننه_کلی_TYPE,
                "Preserve entry for modifiers not used by the %s type; "
                "Index %s converted to %s\n",
                TypeTxt(info, type), before, after);
    }

    xkb_mod_ماسک_t preserve_mods = 0;
    إذا (!بیان_Resolve_Mod_ماسک(info->ctx, ارزښت, MOD_BOTH, &info->mods,
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
                XKB_WARNING_ILLEGAL_کلی_TYPE_PRESERVE_پایله,
                "Illegal value for preserve[%s] in type %s; "
                "Converted %s to %s\n",
                Mod_ماسک_متن(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type), before, after);
    }

    أرجع جمع_کړه_Preserve(info, type, mods, preserve_mods);
}

/***====================================================================***/

ساكن bool
جمع_کړه_Level_Name(کلی_Types_Info *info, کلی_Type_Info *type,
             xkb_level_شاخص_t level, xkb_atom_t name, bool clobber)
{
    /* New name. */
    إذا (level >= darray_اندازه(type->level_نومونه)) {
        darray_resize0(type->level_نومونه, level + 1);
        اذهب پای;
    }

    /* Same level, same name. */
    إذا (darray_توکی(type->level_نومونه, level) == name) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE, XKB_WARNING_DUPLICATE_ثبت,
                "Duplicate names for level %"PRIu32" of key type %s; Ignored\n",
                level + 1, TypeTxt(info, type));
        أرجع true;
    }

    /* Same level, different name. */
    إذا (darray_توکی(type->level_نومونه, level) != XKB_ATOM_NONE) {
        ثابت حرف *old, *new;
        old = xkb_atom_متن(info->ctx,
                            darray_توکی(type->level_نومونه, level));
        new = xkb_atom_متن(info->ctx, name);
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_کلی_TYPE_LEVEL_نومونه,
                "Multiple names for level %"PRIu32" of key type %s; "
                "Using %s, ignoring %s\n",
                level + 1, TypeTxt(info, type),
                (clobber ? new : old), (clobber ? old : new));

        إذا (!clobber)
            أرجع true;
    }

    /* FIXME: What about different level, same name? */

پای:
    darray_توکی(type->level_نومونه, level) = name;
    أرجع true;
}

ساكن bool
سېټ_Level_Name(کلی_Types_Info *info, کلی_Type_Info *type, بیان_Def *arrayNdx,
             بیان_Def *ارزښت)
{
    إذا (arrayNdx == عدم)
        أرجع ReportTypeShouldBeArray(info, type, "level name");

    xkb_level_شاخص_t level = 0;
    إذا (!بیان_Resolve_Level(info->ctx, arrayNdx, &level))
        أرجع ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_ښويدنه_LEVEL,
                                 type, "level name", "integer");

    xkb_atom_t level_name = XKB_ATOM_NONE;
    إذا (!بیان_Resolve_String(info->ctx, ارزښت, &level_name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_ډګر_TYPE,
                "Non-string name for level %"PRIu32" in key type %s; "
                "Ignoring illegal level name definition\n",
                level + 1, xkb_atom_متن(info->ctx, type->name));
        أرجع false;
    }

    أرجع جمع_کړه_Level_Name(info, type, level, level_name, true);
}

/***====================================================================***/

ساكن bool
سېټ_کلی_Type_ډګر(کلی_Types_Info *info, کلی_Type_Info *type,
                ثابت حرف *ډګر_2, بیان_Def *arrayNdx, بیان_Def *ارزښت)
{
    bool ok = false;
    تعداد type_ډګر type_ډګر = 0;

    إذا (istreq(ډګر_2, "modifiers")) {
        type_ډګر = TYPE_ډګر_ماسک;
        ok = سېټ_Modifiers(info, type, arrayNdx, ارزښت);
    }
    وإلا إذا (istreq(ډګر_2, "map")) {
        type_ډګر = TYPE_ډګر_MAP;
        ok = سېټ_Map_ثبت(info, type, arrayNdx, ارزښت);
    }
    وإلا إذا (istreq(ډګر_2, "preserve")) {
        type_ډګر = TYPE_ډګر_PRESERVE;
        ok = سېټ_Preserve(info, type, arrayNdx, ارزښت);
    }
    وإلا إذا (istreq(ډګر_2, "levelname") || istreq(ډګر_2, "level_name")) {
        type_ډګر = TYPE_ډګر_LEVEL_NAME;
        ok = سېټ_Level_Name(info, type, arrayNdx, ارزښت);
    } وإلا {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_ډګر,
                "Unknown field \"%s\" in key type \"%s\"; Definition ignored\n",
                ډګر_2, TypeTxt(info, type));
        ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_TYPE_FIELDS);
    }

    type->defined |= type_ډګر;
    أرجع ok;
}

ساكن bool
Handle_کلی_Type_Body(کلی_Types_Info *info, VarDef *def, کلی_Type_Info *type)
{
    bool ok = true;
    ثابت حرف *elem, *ډګر_2;
    بیان_Def *arrayNdx;

    لكل (; def; def = (VarDef *) def->common.بل) {
        إذا (!بیان_Resolve_Lhs(info->ctx, def->name, &elem, &ډګر_2,
                            &arrayNdx)) {
            /* internal error, already reported */
            ok = false;
            استمر;
        }

        إذا (elem) {
            إذا (istreq(elem, "type")) {
                log_err(info->ctx, XKB_ERROR_INVALID_سېټ_DEFAULT_STATEMENT,
                        "Support for changing the default type has been removed; "
                        "Statement \"%s.%s\" ignored.\n", elem, ډګر_2);
            }
            وإلا {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key type statement: move statements to the global "
                        "file scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, ډګر_2);
                ok = false;
            }
            استمر;
        }

        إذا (!سېټ_کلی_Type_ډګر(info, type, ډګر_2, arrayNdx, def->ارزښت))
            ok = false;
    }

    أرجع ok;
}

ساكن bool
Handle_کلی_Type_Def(کلی_Types_Info *info, کلی_Type_Def *def)
{
    کلی_Type_Info type = {
        .defined = 0,
        .یوځای = def->یوځای,
        .name = def->name,
        .mods = 0,
        .عدد_پوړونه = 1,
        .entries = darray_new(),
        .level_نومونه = darray_new(),
    };

    إذا (!Handle_کلی_Type_Body(info, def->body, &type) ||
        !جمع_کړه_کلی_Type(info, &type, true))
    {
        info->error_شمېر++;
        Clear_کلی_Type_Info(&type);
        أرجع false;
    }

    /* Type has been either stolen via shallow copy or reinitialized in
       `AddKeyType`: no need to free the arrays */
    أرجع true;
}

ساكن bool
HandleGlobalVar(کلی_Types_Info *info, VarDef *stmt)
{
    ثابت حرف *elem, *ډګر_2;
    بیان_Def *arrayNdx;

    إذا (!بیان_Resolve_Lhs(info->ctx, stmt->name, &elem, &ډګر_2,
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
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_ډګر,
                "Default defined for unknown element \"%s\"; "
                "Value for field \"%s.%s\" ignored\n", elem, elem, ډګر_2);
        أرجع !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
    }
    وإلا إذا (ډګر_2) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_ډګر,
                "Default defined for unknown field \"%s\"; Ignored\n", ډګر_2);
        أرجع !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS);
    }
    أرجع false;
}

ساكن فراغ
Handle_کلی_Types_File(کلی_Types_Info *info, XkbFile *file)
{
    bool ok;

    حرر(info->name);
    info->name = strdup_خوندي(file->name);

    لكل (ParseCommon *stmt = file->defs; stmt; stmt = stmt->بل) {
        بدل (stmt->type) {
        حالة STMT_INCLUDE:
            ok = Handle_Include_کلی_Types(info, (IncludeStmt *) stmt);
            اكسر;
        حالة STMT_TYPE:
            ok = Handle_کلی_Type_Def(info, (کلی_Type_Def *) stmt);
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
            info->error_شمېر++;

        إذا (info->error_شمېر > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning keytypes file \"%s\"\n",
                    خوندي_map_name(file->name));
            اكسر;
        }
    }
}

/***====================================================================***/

ساكن bool
Copy_کلی_Types_منزل_Keymap(بنية xkb_keymap *keymap, کلی_Types_Info *info)
{
    ثابت darray_اندازه_t عدد_types = darray_تشه(info->types)
                                  ? 1
                                  : darray_اندازه(info->types);
    بنية xkb_کلی_type *types = حافظه_صفر_ورکړه(عدد_types, حجم(*types));
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
    إذا (darray_تشه(info->types)) {
        بنية xkb_کلی_type *type = &types[0];

        type->mods.mods = 0;
        type->عدد_پوړونه = 1;
        type->entries = عدم;
        type->عدد_entries = 0;
        type->name = xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL");
        type->level_نومونه = عدم;
        type->عدد_level_نومونه = 0;
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

        لكل (darray_اندازه_t i = 0; i < عدد_types; i++) {
            کلی_Type_Info *def = &darray_توکی(info->types, i);
            بنية xkb_کلی_type *type = &types[i];

            type->name = def->name;
            type->mods.mods = def->mods;
            type->عدد_پوړونه = def->عدد_پوړونه;
            type->عدد_level_نومونه =
                (xkb_level_شاخص_t) darray_اندازه(def->level_نومونه);
            darray_steal(def->level_نومونه, &type->level_نومونه, عدم);
            darray_steal(def->entries, &type->entries, &type->عدد_entries);
            type->required = false;

            /* HACK: Never drop canonical XKB key types (see comment above) */
            إذا (type->عدد_پوړونه <= 2) {
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

    keymap->types_section_name = strdup_خوندي(info->name);
    XkbEscapeMapName(keymap->types_section_name);
    keymap->عدد_types = عدد_types;
    keymap->types = types;
    keymap->mods = info->mods;
    أرجع true;
}

/***====================================================================***/

bool
Compile_کلی_Types(XkbFile *file, بنية xkb_keymap_info *keymap_info)
{
    کلی_Types_Info info;

    Init_کلی_Types_Info(&info, keymap_info, 0,
                     &keymap_info->keymap.mods);

    إذا (file != عدم)
        Handle_کلی_Types_File(&info, file);

    إذا (info.error_شمېر != 0)
        اذهب err_info;

    إذا (!Copy_کلی_Types_منزل_Keymap(&keymap_info->keymap, &info))
        اذهب err_info;

    Clear_کلی_Types_Info(&info);
    أرجع true;

err_info:
    Clear_کلی_Types_Info(&info);
    أرجع false;
}
