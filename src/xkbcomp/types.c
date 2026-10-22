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

تعداد type_میدان {
    TYPE_میدان_نقاب = (1 << 0),
    TYPE_میدان_MAP = (1 << 1),
    TYPE_میدان_PRESERVE = (1 << 2),
    TYPE_میدان_LEVEL_NAME = (1 << 3),
};

تعريفنوع بنية {
    تعداد type_میدان defined;
    تعداد ملاؤ_منوال ملاؤ;

    xkb_atom_t name;
    xkb_mod_نقاب_t mods;
    xkb_level_اشاریہ_t عدد_پدھراں;
    darray(بنية xkb_کلید_type_اندراج) entries;
    darray(xkb_atom_t) level_ناں;
} کلید_Type_Info;

تعريفنوع بنية {
    حرف *name;
    صحيح error_گنتی;
    بنانشانی صحيح include_ڈونگھائی;

    darray(کلید_Type_Info) types;
    بنية xkb_mod_سیٹ mods;

    بنية xkb_context *ctx;
    ثابت بنية xkb_keymap_info *keymap_info;
} کلید_Types_Info;

/***====================================================================***/

ساكن مضمن ثابت حرف *
Map_اندراج_Txt(کلید_Types_Info *info, بنية xkb_کلید_type_اندراج *اندراج)
{
    أرجع Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, اندراج->mods.mods);
}

ساكن مضمن ثابت حرف *
TypeTxt(کلید_Types_Info *info, کلید_Type_Info *type)
{
    أرجع xkb_atom_متن(info->ctx, type->name);
}

ساكن مضمن ثابت حرف *
Type_نقاب_Txt(کلید_Types_Info *info, کلید_Type_Info *type)
{
    أرجع Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, type->mods);
}

ساكن مضمن bool
ReportTypeShouldBeArray(کلید_Types_Info *info, کلید_Type_Info *type,
                        ثابت حرف *میدان_2)
{
    أرجع ReportShouldBeArray(info->ctx, "key type", میدان_2,
                               TypeTxt(info, type));
}

ساكن مضمن bool
ReportTypeBadType(کلید_Types_Info *info, تعداد xkb_سنیہا_کوڈ کوڈ,
                  کلید_Type_Info *type, ثابت حرف *میدان_2, ثابت حرف *wanted)
{
    أرجع ReportBadType(info->ctx, کوڈ, "key type", میدان_2,
                         TypeTxt(info, type), wanted);
}

/***====================================================================***/

ساكن فراغ
Init_کلید_Types_Info(کلید_Types_Info *info, ثابت بنية xkb_keymap_info *keymap_info,
                 بنانشانی صحيح include_ڈونگھائی,
                 ثابت بنية xkb_mod_سیٹ *mods)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_ڈونگھائی = include_ڈونگھائی;
    InitVMods(&info->mods, mods, include_ڈونگھائی > 0);
}

ساكن فراغ
Clear_کلید_Type_Info(کلید_Type_Info *type)
{
    darray_free(type->entries);
    darray_free(type->level_ناں);
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
لبھو_Matching_کلید_Type(کلید_Types_Info *info, xkb_atom_t name)
{
    کلید_Type_Info *old;

    darray_foreach(old, info->types)
        إذا (old->name == name)
            أرجع old;

    أرجع عدم;
}

ساكن bool
جوڑو_کلید_Type(کلید_Types_Info *info, کلید_Type_Info *new, bool same_file)
{
    کلید_Type_Info *old;
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);

    old = لبھو_Matching_کلید_Type(info, new->name);
    إذا (old) {
        إذا (new->ملاؤ != ملاؤ_AUGMENT) {
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
            darray_init(new->level_ناں);
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
ملاؤ_Included_کلید_Types(کلید_Types_Info *into, کلید_Types_Info *from,
                      تعداد ملاؤ_منوال ملاؤ)
{
    إذا (from->error_گنتی > 0) {
        into->error_گنتی += from->error_گنتی;
        أرجع;
    }

    ملاؤ_Mod_Sets(into->ctx, &into->mods, &from->mods, ملاؤ);

    إذا (into->name == عدم) {
        into->name = steal(&from->name);
    }

    إذا (darray_خالی_تھاواں(into->types)) {
        into->types = from->types;
        /* Types stolen via shallow copy, so reinitialize the array */
        darray_init(from->types);
    }
    وإلا {
        کلید_Type_Info *type;
        darray_foreach(type, from->types) {
            type->ملاؤ = ملاؤ;
            إذا (!جوڑو_کلید_Type(into, type, false))
                into->error_گنتی++;
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

    إذا (Exceeds_Include_Max_ڈونگھائی(info->ctx, info->include_ڈونگھائی)) {
        info->error_گنتی += 10;
        أرجع false;
    }

    Init_کلید_Types_Info(&included, info->keymap_info, info->include_ڈونگھائی + 1,
                     &info->mods);
    included.name = steal(&include->stmt);

    ثابت بنية parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    لكل (IncludeStmt *stmt = include; stmt; stmt = stmt->اگلا_incl) {
        کلید_Types_Info اگلا_incl;
        XkbFile *file;

        حرف رستہ[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_TYPES,
                                  رستہ, حجم(رستہ));
        إذا (!file) {
            info->error_گنتی += 10;
            Clear_کلید_Types_Info(&included);
            أرجع false;
        }

        Init_کلید_Types_Info(&اگلا_incl, info->keymap_info, info->include_ڈونگھائی + 1,
                         &included.mods);

        Handle_کلید_Types_File(&اگلا_incl, file);

        ملاؤ_Included_کلید_Types(&included, &اگلا_incl, stmt->ملاؤ);

        Clear_کلید_Types_Info(&اگلا_incl);
        FreeXkbFile(file);
    }

    ملاؤ_Included_کلید_Types(info, &included, include->ملاؤ);
    Clear_کلید_Types_Info(&included);

    أرجع (info->error_گنتی == 0);
}

/***====================================================================***/

ساكن bool
سیٹ_Modifiers(کلید_Types_Info *info, کلید_Type_Info *type, اظہاریہ_Def *arrayNdx,
             اظہاریہ_Def *قدر)
{
    xkb_mod_نقاب_t mods = 0;

    إذا (arrayNdx) {
        log_err(info->ctx, XKB_LOG_سنیہا_NO_ID,
                "The modifiers field of a key type is not an array; "
                "Illegal array subscript ignored\n");
        أرجع false;
    }

    إذا (!اظہاریہ_Resolve_Mod_نقاب(info->ctx, قدر, MOD_BOTH, &info->mods, &mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب,
                "Key type mask field must be a modifier mask; "
                "Key type definition ignored\n");
        أرجع false;
    }

    إذا (type->defined & TYPE_میدان_نقاب) {
        log_warn(info->ctx, XKB_LOG_سنیہا_NO_ID,
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

ساكن بنية xkb_کلید_type_اندراج *
لبھو_Matching_Map_اندراج(کلید_Type_Info *type, xkb_mod_نقاب_t mods)
{
    بنية xkb_کلید_type_اندراج *اندراج;

    darray_foreach(اندراج, type->entries)
        إذا (اندراج->mods.mods == mods)
            أرجع اندراج;

    أرجع عدم;
}

ساكن bool
جوڑو_Map_اندراج(کلید_Types_Info *info, کلید_Type_Info *type,
            بنية xkb_کلید_type_اندراج *new, bool clobber, bool report)
{
    بنية xkb_کلید_type_اندراج *old;

    old = لبھو_Matching_Map_اندراج(type, new->mods.mods);
    إذا (old) {
        إذا (report && old->level != new->level) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_TYPE_MAP_اندراج,
                     "Multiple map entries for %s in %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     Map_اندراج_Txt(info, new), TypeTxt(info, type),
                     (clobber ? new->level : old->level) + 1,
                     (clobber ? old->level : new->level) + 1);
        }
        وإلا {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_CONFLICTING_کلید_TYPE_MAP_اندراج,
                    "Multiple occurrences of map[%s]= %"PRIu32" in %s; Ignored\n",
                    Map_اندراج_Txt(info, new), new->level + 1,
                    TypeTxt(info, type));
            أرجع true;
        }

        إذا (clobber) {
            إذا (new->level >= type->عدد_پدھراں)
                type->عدد_پدھراں = new->level + 1;
            old->level = new->level;
        }

        أرجع true;
    }

    إذا (new->level >= type->عدد_پدھراں)
        type->عدد_پدھراں = new->level + 1;

    darray_append(type->entries, *new);
    أرجع true;
}

ساكن bool
سیٹ_Map_اندراج(کلید_Types_Info *info, کلید_Type_Info *type, اظہاریہ_Def *arrayNdx,
            اظہاریہ_Def *قدر)
{
    بنية xkb_کلید_type_اندراج اندراج;

    إذا (arrayNdx == عدم)
        أرجع ReportTypeShouldBeArray(info, type, "map entry");

    إذا (!اظہاریہ_Resolve_Mod_نقاب(info->ctx, arrayNdx, MOD_BOTH, &info->mods,
                            &اندراج.mods.mods))
        أرجع ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب_2,
                                 type, "map entry", "modifier mask");

    إذا (اندراج.mods.mods & (~type->mods)) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_وچ_کلید_TYPE,
                "Map entry for modifiers not used by type %s; "
                "Using %s instead of %s\n",
                TypeTxt(info, type),
                Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods,
                            اندراج.mods.mods & type->mods),
                Map_اندراج_Txt(info, &اندراج));
        اندراج.mods.mods &= type->mods;
    }

    إذا (!اظہاریہ_Resolve_Level(info->ctx, قدر, &اندراج.level)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_کھسکاؤ_LEVEL,
                          "Level specifications in a key type must be integer; "
                          "Ignoring malformed level specification\n");
        أرجع false;
    }

    اندراج.preserve.mods = 0;

    أرجع جوڑو_Map_اندراج(info, type, &اندراج, true, true);
}

/***====================================================================***/

ساكن bool
جوڑو_Preserve(کلید_Types_Info *info, کلید_Type_Info *type,
            xkb_mod_نقاب_t mods, xkb_mod_نقاب_t preserve_mods)
{
    بنية xkb_کلید_type_اندراج *اندراج;

    darray_foreach(اندراج, type->entries) {
        إذا (اندراج->mods.mods != mods)
            استمر;

        /* Map exists without previous preserve (or "None"); override. */
        إذا (اندراج->preserve.mods == 0) {
            اندراج->preserve.mods = preserve_mods;
            أرجع true;
        }

        /* Map exists with same preserve; do nothing. */
        إذا (اندراج->preserve.mods == preserve_mods) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_DUPLICATE_اندراج,
                    "Identical definitions for preserve[%s] in %s; "
                    "Ignored\n",
                    Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, mods),
                    TypeTxt(info, type));
            أرجع true;
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
                            اندراج->preserve.mods));

        اندراج->preserve.mods = preserve_mods;
        أرجع true;
    }

    /*
     * Map does not exist, i.e. preserve[] came before map[].
     * Create a map with the specified mask mapping to Level1. The level
     * may be overridden later with an explicit map[] statement.
     */
    ثابت بنية xkb_کلید_type_اندراج new = {
        .level = 0,
        .mods = { .mods = mods },
        .preserve = { .mods = preserve_mods },
    };
    darray_append(type->entries, new);
    أرجع true;
}

ساكن bool
سیٹ_Preserve(کلید_Types_Info *info, کلید_Type_Info *type, اظہاریہ_Def *arrayNdx,
            اظہاریہ_Def *قدر)
{

    إذا (arrayNdx == عدم)
        أرجع ReportTypeShouldBeArray(info, type, "preserve entry");

    xkb_mod_نقاب_t mods = 0;
    إذا (!اظہاریہ_Resolve_Mod_نقاب(info->ctx, arrayNdx, MOD_BOTH, &info->mods, &mods))
        أرجع ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب_2,
                                 type, "preserve entry", "modifier mask");

    إذا (mods & ~type->mods) {
        ثابت حرف *before, *after;

        before = Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, mods);
        mods &= type->mods;
        after = Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_وچ_کلید_TYPE,
                "Preserve entry for modifiers not used by the %s type; "
                "Index %s converted to %s\n",
                TypeTxt(info, type), before, after);
    }

    xkb_mod_نقاب_t preserve_mods = 0;
    إذا (!اظہاریہ_Resolve_Mod_نقاب(info->ctx, قدر, MOD_BOTH, &info->mods,
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
                XKB_WARNING_ILLEGAL_کلید_TYPE_PRESERVE_نتیجہ,
                "Illegal value for preserve[%s] in type %s; "
                "Converted %s to %s\n",
                Mod_نقاب_متن(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type), before, after);
    }

    أرجع جوڑو_Preserve(info, type, mods, preserve_mods);
}

/***====================================================================***/

ساكن bool
جوڑو_Level_Name(کلید_Types_Info *info, کلید_Type_Info *type,
             xkb_level_اشاریہ_t level, xkb_atom_t name, bool clobber)
{
    /* New name. */
    إذا (level >= darray_ناپ(type->level_ناں)) {
        darray_resize0(type->level_ناں, level + 1);
        اذهب انت;
    }

    /* Same level, same name. */
    إذا (darray_شے(type->level_ناں, level) == name) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE, XKB_WARNING_DUPLICATE_اندراج,
                "Duplicate names for level %"PRIu32" of key type %s; Ignored\n",
                level + 1, TypeTxt(info, type));
        أرجع true;
    }

    /* Same level, different name. */
    إذا (darray_شے(type->level_ناں, level) != XKB_ATOM_NONE) {
        ثابت حرف *old, *new;
        old = xkb_atom_متن(info->ctx,
                            darray_شے(type->level_ناں, level));
        new = xkb_atom_متن(info->ctx, name);
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_کلید_TYPE_LEVEL_ناں,
                "Multiple names for level %"PRIu32" of key type %s; "
                "Using %s, ignoring %s\n",
                level + 1, TypeTxt(info, type),
                (clobber ? new : old), (clobber ? old : new));

        إذا (!clobber)
            أرجع true;
    }

    /* FIXME: What about different level, same name? */

انت:
    darray_شے(type->level_ناں, level) = name;
    أرجع true;
}

ساكن bool
سیٹ_Level_Name(کلید_Types_Info *info, کلید_Type_Info *type, اظہاریہ_Def *arrayNdx,
             اظہاریہ_Def *قدر)
{
    إذا (arrayNdx == عدم)
        أرجع ReportTypeShouldBeArray(info, type, "level name");

    xkb_level_اشاریہ_t level = 0;
    إذا (!اظہاریہ_Resolve_Level(info->ctx, arrayNdx, &level))
        أرجع ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_کھسکاؤ_LEVEL,
                                 type, "level name", "integer");

    xkb_atom_t level_name = XKB_ATOM_NONE;
    إذا (!اظہاریہ_Resolve_String(info->ctx, قدر, &level_name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Non-string name for level %"PRIu32" in key type %s; "
                "Ignoring illegal level name definition\n",
                level + 1, xkb_atom_متن(info->ctx, type->name));
        أرجع false;
    }

    أرجع جوڑو_Level_Name(info, type, level, level_name, true);
}

/***====================================================================***/

ساكن bool
سیٹ_کلید_Type_میدان(کلید_Types_Info *info, کلید_Type_Info *type,
                ثابت حرف *میدان_2, اظہاریہ_Def *arrayNdx, اظہاریہ_Def *قدر)
{
    bool ok = false;
    تعداد type_میدان type_میدان = 0;

    إذا (istreq(میدان_2, "modifiers")) {
        type_میدان = TYPE_میدان_نقاب;
        ok = سیٹ_Modifiers(info, type, arrayNdx, قدر);
    }
    وإلا إذا (istreq(میدان_2, "map")) {
        type_میدان = TYPE_میدان_MAP;
        ok = سیٹ_Map_اندراج(info, type, arrayNdx, قدر);
    }
    وإلا إذا (istreq(میدان_2, "preserve")) {
        type_میدان = TYPE_میدان_PRESERVE;
        ok = سیٹ_Preserve(info, type, arrayNdx, قدر);
    }
    وإلا إذا (istreq(میدان_2, "levelname") || istreq(میدان_2, "level_name")) {
        type_میدان = TYPE_میدان_LEVEL_NAME;
        ok = سیٹ_Level_Name(info, type, arrayNdx, قدر);
    } وإلا {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_میدان,
                "Unknown field \"%s\" in key type \"%s\"; Definition ignored\n",
                میدان_2, TypeTxt(info, type));
        ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_TYPE_FIELDS);
    }

    type->defined |= type_میدان;
    أرجع ok;
}

ساكن bool
Handle_کلید_Type_Body(کلید_Types_Info *info, VarDef *def, کلید_Type_Info *type)
{
    bool ok = true;
    ثابت حرف *elem, *میدان_2;
    اظہاریہ_Def *arrayNdx;

    لكل (; def; def = (VarDef *) def->common.اگلا) {
        إذا (!اظہاریہ_Resolve_Lhs(info->ctx, def->name, &elem, &میدان_2,
                            &arrayNdx)) {
            /* internal error, already reported */
            ok = false;
            استمر;
        }

        إذا (elem) {
            إذا (istreq(elem, "type")) {
                log_err(info->ctx, XKB_ERROR_INVALID_سیٹ_DEFAULT_STATEMENT,
                        "Support for changing the default type has been removed; "
                        "Statement \"%s.%s\" ignored.\n", elem, میدان_2);
            }
            وإلا {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key type statement: move statements to the global "
                        "file scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, میدان_2);
                ok = false;
            }
            استمر;
        }

        إذا (!سیٹ_کلید_Type_میدان(info, type, میدان_2, arrayNdx, def->قدر))
            ok = false;
    }

    أرجع ok;
}

ساكن bool
Handle_کلید_Type_Def(کلید_Types_Info *info, کلید_Type_Def *def)
{
    کلید_Type_Info type = {
        .defined = 0,
        .ملاؤ = def->ملاؤ,
        .name = def->name,
        .mods = 0,
        .عدد_پدھراں = 1,
        .entries = darray_new(),
        .level_ناں = darray_new(),
    };

    إذا (!Handle_کلید_Type_Body(info, def->body, &type) ||
        !جوڑو_کلید_Type(info, &type, true))
    {
        info->error_گنتی++;
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
    ثابت حرف *elem, *میدان_2;
    اظہاریہ_Def *arrayNdx;

    إذا (!اظہاریہ_Resolve_Lhs(info->ctx, stmt->name, &elem, &میدان_2,
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
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_میدان,
                "Default defined for unknown element \"%s\"; "
                "Value for field \"%s.%s\" ignored\n", elem, elem, میدان_2);
        أرجع !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
    }
    وإلا إذا (میدان_2) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_میدان,
                "Default defined for unknown field \"%s\"; Ignored\n", میدان_2);
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
    info->name = strdup_محفوظ(file->name);

    لكل (ParseCommon *stmt = file->defs; stmt; stmt = stmt->اگلا) {
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
                    "Ignoring %s\n", stmt_type_منزل_string(stmt->type));
            ok = false;
            اكسر;
        }

        إذا (!ok)
            info->error_گنتی++;

        إذا (info->error_گنتی > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning keytypes file \"%s\"\n",
                    محفوظ_map_name(file->name));
            اكسر;
        }
    }
}

/***====================================================================***/

ساكن bool
Copy_کلید_Types_منزل_Keymap(بنية xkb_keymap *keymap, کلید_Types_Info *info)
{
    ثابت darray_ناپ_t عدد_types = darray_خالی_تھاواں(info->types)
                                  ? 1
                                  : darray_ناپ(info->types);
    بنية xkb_کلید_type *types = صفر_حافظہ_دیو(عدد_types, حجم(*types));
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
    إذا (darray_خالی_تھاواں(info->types)) {
        بنية xkb_کلید_type *type = &types[0];

        type->mods.mods = 0;
        type->عدد_پدھراں = 1;
        type->entries = عدم;
        type->عدد_entries = 0;
        type->name = xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL");
        type->level_ناں = عدم;
        type->عدد_level_ناں = 0;
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

        لكل (darray_ناپ_t i = 0; i < عدد_types; i++) {
            کلید_Type_Info *def = &darray_شے(info->types, i);
            بنية xkb_کلید_type *type = &types[i];

            type->name = def->name;
            type->mods.mods = def->mods;
            type->عدد_پدھراں = def->عدد_پدھراں;
            type->عدد_level_ناں =
                (xkb_level_اشاریہ_t) darray_ناپ(def->level_ناں);
            darray_steal(def->level_ناں, &type->level_ناں, عدم);
            darray_steal(def->entries, &type->entries, &type->عدد_entries);
            type->required = false;

            /* HACK: Never drop canonical XKB key types (see comment above) */
            إذا (type->عدد_پدھراں <= 2) {
                لكل (uint8_t t = 0;
                     t < (uint8_t) ARRAY_ناپ(canonical_types);
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
    keymap->عدد_types = عدد_types;
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

    إذا (info.error_گنتی != 0)
        اذهب err_info;

    إذا (!Copy_کلید_Types_منزل_Keymap(&keymap_info->keymap, &info))
        اذهب err_info;

    Clear_کلید_Types_Info(&info);
    أرجع true;

err_info:
    Clear_کلید_Types_Info(&info);
    أرجع false;
}
