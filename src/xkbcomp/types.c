/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon-status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp-priv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "text.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "vmod.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "expr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "include.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-mem.h"

ਗਿਣਤੀ type_ਖੇਤਰ {
    TYPE_ਖੇਤਰ_ਨਕਾਬ = (1 << 0),
    TYPE_ਖੇਤਰ_MAP = (1 << 1),
    TYPE_ਖੇਤਰ_PRESERVE = (1 << 2),
    TYPE_ਖੇਤਰ_LEVEL_NAME = (1 << 3),
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਗਿਣਤੀ type_ਖੇਤਰ defined;
    ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ;

    xkb_atom_t name;
    xkb_mod_ਨਕਾਬ_t mods;
    xkb_level_ਸੂਚਕਾਂਕ_t ਅੰਕ_ਪੱਧਰ;
    darray(ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ) entries;
    darray(xkb_atom_t) level_ਨਾਂ;
} ਕੁੰਜੀ_Type_Info;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਅੱਖਰ *name;
    ਪੂਰਨ error_ਗਿਣਤੀ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ include_ਡੂੰਘਾਈ;

    darray(ਕੁੰਜੀ_Type_Info) types;
    ਬਣਤਰ xkb_mod_ਸੈੱਟ mods;

    ਬਣਤਰ xkb_context *ctx;
    ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info;
} ਕੁੰਜੀ_Types_Info;

/***====================================================================***/

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ ਅੱਖਰ *
Map_ਇੰਦਰਾਜ_Txt(ਕੁੰਜੀ_Types_Info *info, ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ)
{
    ਵਾਪਸ Mod_ਨਕਾਬ_ਪਾਠ(info->ctx, MOD_BOTH, &info->mods, ਇੰਦਰਾਜ->mods.mods);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ ਅੱਖਰ *
TypeTxt(ਕੁੰਜੀ_Types_Info *info, ਕੁੰਜੀ_Type_Info *type)
{
    ਵਾਪਸ xkb_atom_ਪਾਠ(info->ctx, type->name);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ ਅੱਖਰ *
Type_ਨਕਾਬ_Txt(ਕੁੰਜੀ_Types_Info *info, ਕੁੰਜੀ_Type_Info *type)
{
    ਵਾਪਸ Mod_ਨਕਾਬ_ਪਾਠ(info->ctx, MOD_BOTH, &info->mods, type->mods);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ReportTypeShouldBeArray(ਕੁੰਜੀ_Types_Info *info, ਕੁੰਜੀ_Type_Info *type,
                        ਅਚਲ ਅੱਖਰ *ਖੇਤਰ_2)
{
    ਵਾਪਸ ReportShouldBeArray(info->ctx, "key type", ਖੇਤਰ_2,
                               TypeTxt(info, type));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ReportTypeBadType(ਕੁੰਜੀ_Types_Info *info, ਗਿਣਤੀ xkb_ਸੁਨੇਹਾ_ਕੋਡ ਕੋਡ,
                  ਕੁੰਜੀ_Type_Info *type, ਅਚਲ ਅੱਖਰ *ਖੇਤਰ_2, ਅਚਲ ਅੱਖਰ *wanted)
{
    ਵਾਪਸ ReportBadType(info->ctx, ਕੋਡ, "key type", ਖੇਤਰ_2,
                         TypeTxt(info, type), wanted);
}

/***====================================================================***/

ਸਥਿਰ ਖਾਲੀ
Init_ਕੁੰਜੀ_Types_Info(ਕੁੰਜੀ_Types_Info *info, ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
                 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ include_ਡੂੰਘਾਈ,
                 ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods)
{
    memset(info, 0, ਆਕਾਰ(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_ਡੂੰਘਾਈ = include_ਡੂੰਘਾਈ;
    InitVMods(&info->mods, mods, include_ਡੂੰਘਾਈ > 0);
}

ਸਥਿਰ ਖਾਲੀ
Clear_ਕੁੰਜੀ_Type_Info(ਕੁੰਜੀ_Type_Info *type)
{
    darray_free(type->entries);
    darray_free(type->level_ਨਾਂ);
}

ਸਥਿਰ ਖਾਲੀ
Clear_ਕੁੰਜੀ_Types_Info(ਕੁੰਜੀ_Types_Info *info)
{
    ਮੁਕਤ(info->name);
    ਕੁੰਜੀ_Type_Info *type;
    darray_foreach(type, info->types)
        Clear_ਕੁੰਜੀ_Type_Info(type);
    darray_free(info->types);
}

ਸਥਿਰ ਕੁੰਜੀ_Type_Info *
ਲੱਭੋ_Matching_ਕੁੰਜੀ_Type(ਕੁੰਜੀ_Types_Info *info, xkb_atom_t name)
{
    ਕੁੰਜੀ_Type_Info *old;

    darray_foreach(old, info->types)
        ਜੇ (old->name == name)
            ਵਾਪਸ old;

    ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ bool
ਜੋੜੋ_ਕੁੰਜੀ_Type(ਕੁੰਜੀ_Types_Info *info, ਕੁੰਜੀ_Type_Info *new, bool same_file)
{
    ਕੁੰਜੀ_Type_Info *old;
    ਅਚਲ ਪੂਰਨ verbosity = xkb_context_get_log_verbosity(info->ctx);

    old = ਲੱਭੋ_Matching_ਕੁੰਜੀ_Type(info, new->name);
    ਜੇ (old) {
        ਜੇ (new->ਮਿਲਾਓ != ਮਿਲਾਓ_AUGMENT) {
            ਜੇ ((same_file && verbosity > 0) || verbosity > 9) {
                log_warn(info->ctx,
                         XKB_WARNING_CONFLICTING_ਕੁੰਜੀ_TYPE_DEFINITIONS,
                         "Multiple definitions of the %s key type; "
                         "Earlier definition ignored\n",
                         xkb_atom_ਪਾਠ(info->ctx, new->name));
            }

            Clear_ਕੁੰਜੀ_Type_Info(old);
            *old = *new;
            darray_init(new->entries);
            darray_init(new->level_ਨਾਂ);
            ਵਾਪਸ true;
        }

        ਜੇ (same_file)
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_CONFLICTING_ਕੁੰਜੀ_TYPE_DEFINITIONS,
                    "Multiple definitions of the %s key type; "
                    "Later definition ignored\n",
                    xkb_atom_ਪਾਠ(info->ctx, new->name));

        Clear_ਕੁੰਜੀ_Type_Info(new);
        ਵਾਪਸ true;
    }

    darray_append(info->types, *new);
    ਵਾਪਸ true;
}

/***====================================================================***/

ਸਥਿਰ ਖਾਲੀ
ਮਿਲਾਓ_Included_ਕੁੰਜੀ_Types(ਕੁੰਜੀ_Types_Info *into, ਕੁੰਜੀ_Types_Info *from,
                      ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ)
{
    ਜੇ (from->error_ਗਿਣਤੀ > 0) {
        into->error_ਗਿਣਤੀ += from->error_ਗਿਣਤੀ;
        ਵਾਪਸ;
    }

    ਮਿਲਾਓ_Mod_Sets(into->ctx, &into->mods, &from->mods, ਮਿਲਾਓ);

    ਜੇ (into->name == ਨਿਰਾ) {
        into->name = steal(&from->name);
    }

    ਜੇ (darray_ਖਾਲੀ_ਥਾਵਾਂ(into->types)) {
        into->types = from->types;
        /* Types stolen via shallow copy, so reinitialize the array */
        darray_init(from->types);
    }
    ਨਹੀਂਤਾਂ {
        ਕੁੰਜੀ_Type_Info *type;
        darray_foreach(type, from->types) {
            type->ਮਿਲਾਓ = ਮਿਲਾਓ;
            ਜੇ (!ਜੋੜੋ_ਕੁੰਜੀ_Type(into, type, false))
                into->error_ਗਿਣਤੀ++;
        }
        /* Types were either shallow copied or reinitialized individually
           in `AddKeyType`, so we only need to free the array */
        darray_free(from->types);
    }
}

ਸਥਿਰ ਖਾਲੀ
Handle_ਕੁੰਜੀ_Types_File(ਕੁੰਜੀ_Types_Info *info, XkbFile *file);

ਸਥਿਰ bool
Handle_Include_ਕੁੰਜੀ_Types(ਕੁੰਜੀ_Types_Info *info, IncludeStmt *include)
{
    ਕੁੰਜੀ_Types_Info included;

    ਜੇ (Exceeds_Include_Max_ਡੂੰਘਾਈ(info->ctx, info->include_ਡੂੰਘਾਈ)) {
        info->error_ਗਿਣਤੀ += 10;
        ਵਾਪਸ false;
    }

    Init_ਕੁੰਜੀ_Types_Info(&included, info->keymap_info, info->include_ਡੂੰਘਾਈ + 1,
                     &info->mods);
    included.name = steal(&include->stmt);

    ਅਚਲ ਬਣਤਰ parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    ਲਈ (IncludeStmt *stmt = include; stmt; stmt = stmt->ਅਗਲਾ_incl) {
        ਕੁੰਜੀ_Types_Info ਅਗਲਾ_incl;
        XkbFile *file;

        ਅੱਖਰ ਰਸਤਾ[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_TYPES,
                                  ਰਸਤਾ, ਆਕਾਰ(ਰਸਤਾ));
        ਜੇ (!file) {
            info->error_ਗਿਣਤੀ += 10;
            Clear_ਕੁੰਜੀ_Types_Info(&included);
            ਵਾਪਸ false;
        }

        Init_ਕੁੰਜੀ_Types_Info(&ਅਗਲਾ_incl, info->keymap_info, info->include_ਡੂੰਘਾਈ + 1,
                         &included.mods);

        Handle_ਕੁੰਜੀ_Types_File(&ਅਗਲਾ_incl, file);

        ਮਿਲਾਓ_Included_ਕੁੰਜੀ_Types(&included, &ਅਗਲਾ_incl, stmt->ਮਿਲਾਓ);

        Clear_ਕੁੰਜੀ_Types_Info(&ਅਗਲਾ_incl);
        FreeXkbFile(file);
    }

    ਮਿਲਾਓ_Included_ਕੁੰਜੀ_Types(info, &included, include->ਮਿਲਾਓ);
    Clear_ਕੁੰਜੀ_Types_Info(&included);

    ਵਾਪਸ (info->error_ਗਿਣਤੀ == 0);
}

/***====================================================================***/

ਸਥਿਰ bool
ਸੈੱਟ_Modifiers(ਕੁੰਜੀ_Types_Info *info, ਕੁੰਜੀ_Type_Info *type, ਵਿਅੰਜਕ_Def *arrayNdx,
             ਵਿਅੰਜਕ_Def *ਮੁੱਲ)
{
    xkb_mod_ਨਕਾਬ_t mods = 0;

    ਜੇ (arrayNdx) {
        log_err(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "The modifiers field of a key type is not an array; "
                "Illegal array subscript ignored\n");
        ਵਾਪਸ false;
    }

    ਜੇ (!ਵਿਅੰਜਕ_Resolve_Mod_ਨਕਾਬ(info->ctx, ਮੁੱਲ, MOD_BOTH, &info->mods, &mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ਨਕਾਬ,
                "Key type mask field must be a modifier mask; "
                "Key type definition ignored\n");
        ਵਾਪਸ false;
    }

    ਜੇ (type->defined & TYPE_ਖੇਤਰ_ਨਕਾਬ) {
        log_warn(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                 "Multiple modifier mask definitions for key type %s; "
                 "Using %s, ignoring %s\n",
                 xkb_atom_ਪਾਠ(info->ctx, type->name),
                 Type_ਨਕਾਬ_Txt(info, type),
                 Mod_ਨਕਾਬ_ਪਾਠ(info->ctx, MOD_BOTH, &info->mods, mods));
        ਵਾਪਸ false;
    }

    type->mods = mods;
    ਵਾਪਸ true;
}

/***====================================================================***/

ਸਥਿਰ ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ *
ਲੱਭੋ_Matching_Map_ਇੰਦਰਾਜ(ਕੁੰਜੀ_Type_Info *type, xkb_mod_ਨਕਾਬ_t mods)
{
    ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;

    darray_foreach(ਇੰਦਰਾਜ, type->entries)
        ਜੇ (ਇੰਦਰਾਜ->mods.mods == mods)
            ਵਾਪਸ ਇੰਦਰਾਜ;

    ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ bool
ਜੋੜੋ_Map_ਇੰਦਰਾਜ(ਕੁੰਜੀ_Types_Info *info, ਕੁੰਜੀ_Type_Info *type,
            ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ *new, bool clobber, bool report)
{
    ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ *old;

    old = ਲੱਭੋ_Matching_Map_ਇੰਦਰਾਜ(type, new->mods.mods);
    ਜੇ (old) {
        ਜੇ (report && old->level != new->level) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_ਕੁੰਜੀ_TYPE_MAP_ਇੰਦਰਾਜ,
                     "Multiple map entries for %s in %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     Map_ਇੰਦਰਾਜ_Txt(info, new), TypeTxt(info, type),
                     (clobber ? new->level : old->level) + 1,
                     (clobber ? old->level : new->level) + 1);
        }
        ਨਹੀਂਤਾਂ {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_CONFLICTING_ਕੁੰਜੀ_TYPE_MAP_ਇੰਦਰਾਜ,
                    "Multiple occurrences of map[%s]= %"PRIu32" in %s; Ignored\n",
                    Map_ਇੰਦਰਾਜ_Txt(info, new), new->level + 1,
                    TypeTxt(info, type));
            ਵਾਪਸ true;
        }

        ਜੇ (clobber) {
            ਜੇ (new->level >= type->ਅੰਕ_ਪੱਧਰ)
                type->ਅੰਕ_ਪੱਧਰ = new->level + 1;
            old->level = new->level;
        }

        ਵਾਪਸ true;
    }

    ਜੇ (new->level >= type->ਅੰਕ_ਪੱਧਰ)
        type->ਅੰਕ_ਪੱਧਰ = new->level + 1;

    darray_append(type->entries, *new);
    ਵਾਪਸ true;
}

ਸਥਿਰ bool
ਸੈੱਟ_Map_ਇੰਦਰਾਜ(ਕੁੰਜੀ_Types_Info *info, ਕੁੰਜੀ_Type_Info *type, ਵਿਅੰਜਕ_Def *arrayNdx,
            ਵਿਅੰਜਕ_Def *ਮੁੱਲ)
{
    ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ ਇੰਦਰਾਜ;

    ਜੇ (arrayNdx == ਨਿਰਾ)
        ਵਾਪਸ ReportTypeShouldBeArray(info, type, "map entry");

    ਜੇ (!ਵਿਅੰਜਕ_Resolve_Mod_ਨਕਾਬ(info->ctx, arrayNdx, MOD_BOTH, &info->mods,
                            &ਇੰਦਰਾਜ.mods.mods))
        ਵਾਪਸ ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_ਨਕਾਬ_2,
                                 type, "map entry", "modifier mask");

    ਜੇ (ਇੰਦਰਾਜ.mods.mods & (~type->mods)) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_ਵਿੱਚ_ਕੁੰਜੀ_TYPE,
                "Map entry for modifiers not used by type %s; "
                "Using %s instead of %s\n",
                TypeTxt(info, type),
                Mod_ਨਕਾਬ_ਪਾਠ(info->ctx, MOD_BOTH, &info->mods,
                            ਇੰਦਰਾਜ.mods.mods & type->mods),
                Map_ਇੰਦਰਾਜ_Txt(info, &ਇੰਦਰਾਜ));
        ਇੰਦਰਾਜ.mods.mods &= type->mods;
    }

    ਜੇ (!ਵਿਅੰਜਕ_Resolve_Level(info->ctx, ਮੁੱਲ, &ਇੰਦਰਾਜ.level)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_ਖਿਸਕਾਓ_LEVEL,
                          "Level specifications in a key type must be integer; "
                          "Ignoring malformed level specification\n");
        ਵਾਪਸ false;
    }

    ਇੰਦਰਾਜ.preserve.mods = 0;

    ਵਾਪਸ ਜੋੜੋ_Map_ਇੰਦਰਾਜ(info, type, &ਇੰਦਰਾਜ, true, true);
}

/***====================================================================***/

ਸਥਿਰ bool
ਜੋੜੋ_Preserve(ਕੁੰਜੀ_Types_Info *info, ਕੁੰਜੀ_Type_Info *type,
            xkb_mod_ਨਕਾਬ_t mods, xkb_mod_ਨਕਾਬ_t preserve_mods)
{
    ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;

    darray_foreach(ਇੰਦਰਾਜ, type->entries) {
        ਜੇ (ਇੰਦਰਾਜ->mods.mods != mods)
            ਜਾਰੀ;

        /* Map exists without previous preserve (or "None"); override. */
        ਜੇ (ਇੰਦਰਾਜ->preserve.mods == 0) {
            ਇੰਦਰਾਜ->preserve.mods = preserve_mods;
            ਵਾਪਸ true;
        }

        /* Map exists with same preserve; do nothing. */
        ਜੇ (ਇੰਦਰਾਜ->preserve.mods == preserve_mods) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE,
                    XKB_WARNING_DUPLICATE_ਇੰਦਰਾਜ,
                    "Identical definitions for preserve[%s] in %s; "
                    "Ignored\n",
                    Mod_ਨਕਾਬ_ਪਾਠ(info->ctx, MOD_BOTH, &info->mods, mods),
                    TypeTxt(info, type));
            ਵਾਪਸ true;
        }

        /* Map exists with different preserve; latter wins. */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_ਕੁੰਜੀ_TYPE_PRESERVE_ENTRIES,
                "Multiple definitions for preserve[%s] in %s; "
                "Using %s, ignoring %s\n",
                Mod_ਨਕਾਬ_ਪਾਠ(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type),
                Mod_ਨਕਾਬ_ਪਾਠ(info->ctx, MOD_BOTH, &info->mods, preserve_mods),
                Mod_ਨਕਾਬ_ਪਾਠ(info->ctx, MOD_BOTH, &info->mods,
                            ਇੰਦਰਾਜ->preserve.mods));

        ਇੰਦਰਾਜ->preserve.mods = preserve_mods;
        ਵਾਪਸ true;
    }

    /*
     * Map does not exist, i.e. preserve[] came before map[].
     * Create a map with the specified mask mapping to Level1. The level
     * may be overridden later with an explicit map[] statement.
     */
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ new = {
        .level = 0,
        .mods = { .mods = mods },
        .preserve = { .mods = preserve_mods },
    };
    darray_append(type->entries, new);
    ਵਾਪਸ true;
}

ਸਥਿਰ bool
ਸੈੱਟ_Preserve(ਕੁੰਜੀ_Types_Info *info, ਕੁੰਜੀ_Type_Info *type, ਵਿਅੰਜਕ_Def *arrayNdx,
            ਵਿਅੰਜਕ_Def *ਮੁੱਲ)
{

    ਜੇ (arrayNdx == ਨਿਰਾ)
        ਵਾਪਸ ReportTypeShouldBeArray(info, type, "preserve entry");

    xkb_mod_ਨਕਾਬ_t mods = 0;
    ਜੇ (!ਵਿਅੰਜਕ_Resolve_Mod_ਨਕਾਬ(info->ctx, arrayNdx, MOD_BOTH, &info->mods, &mods))
        ਵਾਪਸ ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_MODIFIER_ਨਕਾਬ_2,
                                 type, "preserve entry", "modifier mask");

    ਜੇ (mods & ~type->mods) {
        ਅਚਲ ਅੱਖਰ *before, *after;

        before = Mod_ਨਕਾਬ_ਪਾਠ(info->ctx, MOD_BOTH, &info->mods, mods);
        mods &= type->mods;
        after = Mod_ਨਕਾਬ_ਪਾਠ(info->ctx, MOD_BOTH, &info->mods, mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNDECLARED_MODIFIERS_ਵਿੱਚ_ਕੁੰਜੀ_TYPE,
                "Preserve entry for modifiers not used by the %s type; "
                "Index %s converted to %s\n",
                TypeTxt(info, type), before, after);
    }

    xkb_mod_ਨਕਾਬ_t preserve_mods = 0;
    ਜੇ (!ਵਿਅੰਜਕ_Resolve_Mod_ਨਕਾਬ(info->ctx, ਮੁੱਲ, MOD_BOTH, &info->mods,
                            &preserve_mods)) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ਨਕਾਬ,
                "Preserve value in a key type is not a modifier mask; "
                "Ignoring preserve[%s] in type %s\n",
                Mod_ਨਕਾਬ_ਪਾਠ(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type));
        ਵਾਪਸ false;
    }

    ਜੇ (preserve_mods & ~mods) {
        ਅਚਲ ਅੱਖਰ *before, *after;

        before = Mod_ਨਕਾਬ_ਪਾਠ(info->ctx, MOD_BOTH, &info->mods, preserve_mods);
        preserve_mods &= mods;
        after = Mod_ਨਕਾਬ_ਪਾਠ(info->ctx, MOD_BOTH, &info->mods, preserve_mods);

        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_ILLEGAL_ਕੁੰਜੀ_TYPE_PRESERVE_ਨਤੀਜਾ,
                "Illegal value for preserve[%s] in type %s; "
                "Converted %s to %s\n",
                Mod_ਨਕਾਬ_ਪਾਠ(info->ctx, MOD_BOTH, &info->mods, mods),
                TypeTxt(info, type), before, after);
    }

    ਵਾਪਸ ਜੋੜੋ_Preserve(info, type, mods, preserve_mods);
}

/***====================================================================***/

ਸਥਿਰ bool
ਜੋੜੋ_Level_Name(ਕੁੰਜੀ_Types_Info *info, ਕੁੰਜੀ_Type_Info *type,
             xkb_level_ਸੂਚਕਾਂਕ_t level, xkb_atom_t name, bool clobber)
{
    /* New name. */
    ਜੇ (level >= darray_ਮਾਪ(type->level_ਨਾਂ)) {
        darray_resize0(type->level_ਨਾਂ, level + 1);
        ਜਾਓ ਅੰਤ;
    }

    /* Same level, same name. */
    ਜੇ (darray_ਵਸਤੂ(type->level_ਨਾਂ, level) == name) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_VERBOSE, XKB_WARNING_DUPLICATE_ਇੰਦਰਾਜ,
                "Duplicate names for level %"PRIu32" of key type %s; Ignored\n",
                level + 1, TypeTxt(info, type));
        ਵਾਪਸ true;
    }

    /* Same level, different name. */
    ਜੇ (darray_ਵਸਤੂ(type->level_ਨਾਂ, level) != XKB_ATOM_NONE) {
        ਅਚਲ ਅੱਖਰ *old, *new;
        old = xkb_atom_ਪਾਠ(info->ctx,
                            darray_ਵਸਤੂ(type->level_ਨਾਂ, level));
        new = xkb_atom_ਪਾਠ(info->ctx, name);
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_CONFLICTING_ਕੁੰਜੀ_TYPE_LEVEL_ਨਾਂ,
                "Multiple names for level %"PRIu32" of key type %s; "
                "Using %s, ignoring %s\n",
                level + 1, TypeTxt(info, type),
                (clobber ? new : old), (clobber ? old : new));

        ਜੇ (!clobber)
            ਵਾਪਸ true;
    }

    /* FIXME: What about different level, same name? */

ਅੰਤ:
    darray_ਵਸਤੂ(type->level_ਨਾਂ, level) = name;
    ਵਾਪਸ true;
}

ਸਥਿਰ bool
ਸੈੱਟ_Level_Name(ਕੁੰਜੀ_Types_Info *info, ਕੁੰਜੀ_Type_Info *type, ਵਿਅੰਜਕ_Def *arrayNdx,
             ਵਿਅੰਜਕ_Def *ਮੁੱਲ)
{
    ਜੇ (arrayNdx == ਨਿਰਾ)
        ਵਾਪਸ ReportTypeShouldBeArray(info, type, "level name");

    xkb_level_ਸੂਚਕਾਂਕ_t level = 0;
    ਜੇ (!ਵਿਅੰਜਕ_Resolve_Level(info->ctx, arrayNdx, &level))
        ਵਾਪਸ ReportTypeBadType(info, XKB_ERROR_UNSUPPORTED_ਖਿਸਕਾਓ_LEVEL,
                                 type, "level name", "integer");

    xkb_atom_t level_name = XKB_ATOM_NONE;
    ਜੇ (!ਵਿਅੰਜਕ_Resolve_String(info->ctx, ਮੁੱਲ, &level_name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE,
                "Non-string name for level %"PRIu32" in key type %s; "
                "Ignoring illegal level name definition\n",
                level + 1, xkb_atom_ਪਾਠ(info->ctx, type->name));
        ਵਾਪਸ false;
    }

    ਵਾਪਸ ਜੋੜੋ_Level_Name(info, type, level, level_name, true);
}

/***====================================================================***/

ਸਥਿਰ bool
ਸੈੱਟ_ਕੁੰਜੀ_Type_ਖੇਤਰ(ਕੁੰਜੀ_Types_Info *info, ਕੁੰਜੀ_Type_Info *type,
                ਅਚਲ ਅੱਖਰ *ਖੇਤਰ_2, ਵਿਅੰਜਕ_Def *arrayNdx, ਵਿਅੰਜਕ_Def *ਮੁੱਲ)
{
    bool ok = false;
    ਗਿਣਤੀ type_ਖੇਤਰ type_ਖੇਤਰ = 0;

    ਜੇ (istreq(ਖੇਤਰ_2, "modifiers")) {
        type_ਖੇਤਰ = TYPE_ਖੇਤਰ_ਨਕਾਬ;
        ok = ਸੈੱਟ_Modifiers(info, type, arrayNdx, ਮੁੱਲ);
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "map")) {
        type_ਖੇਤਰ = TYPE_ਖੇਤਰ_MAP;
        ok = ਸੈੱਟ_Map_ਇੰਦਰਾਜ(info, type, arrayNdx, ਮੁੱਲ);
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "preserve")) {
        type_ਖੇਤਰ = TYPE_ਖੇਤਰ_PRESERVE;
        ok = ਸੈੱਟ_Preserve(info, type, arrayNdx, ਮੁੱਲ);
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "levelname") || istreq(ਖੇਤਰ_2, "level_name")) {
        type_ਖੇਤਰ = TYPE_ਖੇਤਰ_LEVEL_NAME;
        ok = ਸੈੱਟ_Level_Name(info, type, arrayNdx, ਮੁੱਲ);
    } ਨਹੀਂਤਾਂ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_ਖੇਤਰ,
                "Unknown field \"%s\" in key type \"%s\"; Definition ignored\n",
                ਖੇਤਰ_2, TypeTxt(info, type));
        ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_TYPE_FIELDS);
    }

    type->defined |= type_ਖੇਤਰ;
    ਵਾਪਸ ok;
}

ਸਥਿਰ bool
Handle_ਕੁੰਜੀ_Type_Body(ਕੁੰਜੀ_Types_Info *info, VarDef *def, ਕੁੰਜੀ_Type_Info *type)
{
    bool ok = true;
    ਅਚਲ ਅੱਖਰ *elem, *ਖੇਤਰ_2;
    ਵਿਅੰਜਕ_Def *arrayNdx;

    ਲਈ (; def; def = (VarDef *) def->common.ਅਗਲਾ) {
        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Lhs(info->ctx, def->name, &elem, &ਖੇਤਰ_2,
                            &arrayNdx)) {
            /* internal error, already reported */
            ok = false;
            ਜਾਰੀ;
        }

        ਜੇ (elem) {
            ਜੇ (istreq(elem, "type")) {
                log_err(info->ctx, XKB_ERROR_INVALID_ਸੈੱਟ_DEFAULT_STATEMENT,
                        "Support for changing the default type has been removed; "
                        "Statement \"%s.%s\" ignored.\n", elem, ਖੇਤਰ_2);
            }
            ਨਹੀਂਤਾਂ {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key type statement: move statements to the global "
                        "file scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, ਖੇਤਰ_2);
                ok = false;
            }
            ਜਾਰੀ;
        }

        ਜੇ (!ਸੈੱਟ_ਕੁੰਜੀ_Type_ਖੇਤਰ(info, type, ਖੇਤਰ_2, arrayNdx, def->ਮੁੱਲ))
            ok = false;
    }

    ਵਾਪਸ ok;
}

ਸਥਿਰ bool
Handle_ਕੁੰਜੀ_Type_Def(ਕੁੰਜੀ_Types_Info *info, ਕੁੰਜੀ_Type_Def *def)
{
    ਕੁੰਜੀ_Type_Info type = {
        .defined = 0,
        .ਮਿਲਾਓ = def->ਮਿਲਾਓ,
        .name = def->name,
        .mods = 0,
        .ਅੰਕ_ਪੱਧਰ = 1,
        .entries = darray_new(),
        .level_ਨਾਂ = darray_new(),
    };

    ਜੇ (!Handle_ਕੁੰਜੀ_Type_Body(info, def->body, &type) ||
        !ਜੋੜੋ_ਕੁੰਜੀ_Type(info, &type, true))
    {
        info->error_ਗਿਣਤੀ++;
        Clear_ਕੁੰਜੀ_Type_Info(&type);
        ਵਾਪਸ false;
    }

    /* Type has been either stolen via shallow copy or reinitialized in
       `AddKeyType`: no need to free the arrays */
    ਵਾਪਸ true;
}

ਸਥਿਰ bool
HandleGlobalVar(ਕੁੰਜੀ_Types_Info *info, VarDef *stmt)
{
    ਅਚਲ ਅੱਖਰ *elem, *ਖੇਤਰ_2;
    ਵਿਅੰਜਕ_Def *arrayNdx;

    ਜੇ (!ਵਿਅੰਜਕ_Resolve_Lhs(info->ctx, stmt->name, &elem, &ਖੇਤਰ_2,
                        &arrayNdx)) {
        ਵਾਪਸ false;           /* internal error, already reported */
    }
    ਨਹੀਂਤਾਂ ਜੇ (elem && istreq(elem, "type")) {
        log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                "Support for changing the default type has been removed; "
                "Statement ignored\n");
        ਵਾਪਸ true;
    }
    ਨਹੀਂਤਾਂ ਜੇ (elem) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_ਖੇਤਰ,
                "Default defined for unknown element \"%s\"; "
                "Value for field \"%s.%s\" ignored\n", elem, elem, ਖੇਤਰ_2);
        ਵਾਪਸ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
    }
    ਨਹੀਂਤਾਂ ਜੇ (ਖੇਤਰ_2) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_ਖੇਤਰ,
                "Default defined for unknown field \"%s\"; Ignored\n", ਖੇਤਰ_2);
        ਵਾਪਸ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS);
    }
    ਵਾਪਸ false;
}

ਸਥਿਰ ਖਾਲੀ
Handle_ਕੁੰਜੀ_Types_File(ਕੁੰਜੀ_Types_Info *info, XkbFile *file)
{
    bool ok;

    ਮੁਕਤ(info->name);
    info->name = strdup_ਸੁਰੱਖਿਅਤ(file->name);

    ਲਈ (ParseCommon *stmt = file->defs; stmt; stmt = stmt->ਅਗਲਾ) {
        ਚੋਣ (stmt->type) {
        ਹਾਲਤ STMT_INCLUDE:
            ok = Handle_Include_ਕੁੰਜੀ_Types(info, (IncludeStmt *) stmt);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_TYPE:
            ok = Handle_ਕੁੰਜੀ_Type_Def(info, (ਕੁੰਜੀ_Type_Def *) stmt);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef*) stmt);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_UNKNOWN_DECLARATION:
        ਹਾਲਤ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported types %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ਰੋਕੋ;
        ਮੂਲ:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Key type files may not include other declarations; "
                    "Ignoring %s\n", stmt_type_ਮੰਜ਼ਿਲ_string(stmt->type));
            ok = false;
            ਰੋਕੋ;
        }

        ਜੇ (!ok)
            info->error_ਗਿਣਤੀ++;

        ਜੇ (info->error_ਗਿਣਤੀ > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning keytypes file \"%s\"\n",
                    ਸੁਰੱਖਿਅਤ_map_name(file->name));
            ਰੋਕੋ;
        }
    }
}

/***====================================================================***/

ਸਥਿਰ bool
Copy_ਕੁੰਜੀ_Types_ਮੰਜ਼ਿਲ_Keymap(ਬਣਤਰ xkb_keymap *keymap, ਕੁੰਜੀ_Types_Info *info)
{
    ਅਚਲ darray_ਮਾਪ_t ਅੰਕ_types = darray_ਖਾਲੀ_ਥਾਵਾਂ(info->types)
                                  ? 1
                                  : darray_ਮਾਪ(info->types);
    ਬਣਤਰ xkb_ਕੁੰਜੀ_type *types = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਅੰਕ_types, ਆਕਾਰ(*types));
    ਜੇ (!types)
        ਵਾਪਸ false;

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
    ਜੇ (darray_ਖਾਲੀ_ਥਾਵਾਂ(info->types)) {
        ਬਣਤਰ xkb_ਕੁੰਜੀ_type *type = &types[0];

        type->mods.mods = 0;
        type->ਅੰਕ_ਪੱਧਰ = 1;
        type->entries = ਨਿਰਾ;
        type->ਅੰਕ_entries = 0;
        type->name = xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL");
        type->level_ਨਾਂ = ਨਿਰਾ;
        type->ਅੰਕ_level_ਨਾਂ = 0;
        type->required = true;
    }
    ਨਹੀਂਤਾਂ {
        /* HACK to circumvent Xorg bugs (see comment above) */
        ਅਚਲ xkb_atom_t canonical_types[] = {
            xkb_atom_intern_literal(keymap->ctx, "ONE_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "TWO_LEVEL"),
            xkb_atom_intern_literal(keymap->ctx, "ALPHABETIC"),
            xkb_atom_intern_literal(keymap->ctx, "KEYPAD"),
        };

        ਲਈ (darray_ਮਾਪ_t i = 0; i < ਅੰਕ_types; i++) {
            ਕੁੰਜੀ_Type_Info *def = &darray_ਵਸਤੂ(info->types, i);
            ਬਣਤਰ xkb_ਕੁੰਜੀ_type *type = &types[i];

            type->name = def->name;
            type->mods.mods = def->mods;
            type->ਅੰਕ_ਪੱਧਰ = def->ਅੰਕ_ਪੱਧਰ;
            type->ਅੰਕ_level_ਨਾਂ =
                (xkb_level_ਸੂਚਕਾਂਕ_t) darray_ਮਾਪ(def->level_ਨਾਂ);
            darray_steal(def->level_ਨਾਂ, &type->level_ਨਾਂ, ਨਿਰਾ);
            darray_steal(def->entries, &type->entries, &type->ਅੰਕ_entries);
            type->required = false;

            /* HACK: Never drop canonical XKB key types (see comment above) */
            ਜੇ (type->ਅੰਕ_ਪੱਧਰ <= 2) {
                ਲਈ (uint8_t t = 0;
                     t < (uint8_t) ARRAY_ਮਾਪ(canonical_types);
                     t++) {
                        ਜੇ (type->name == canonical_types[t]) {
                            type->required = true;
                            ਰੋਕੋ;
                        }
                }
            }
        }
    }

    keymap->types_section_name = strdup_ਸੁਰੱਖਿਅਤ(info->name);
    XkbEscapeMapName(keymap->types_section_name);
    keymap->ਅੰਕ_types = ਅੰਕ_types;
    keymap->types = types;
    keymap->mods = info->mods;
    ਵਾਪਸ true;
}

/***====================================================================***/

bool
Compile_ਕੁੰਜੀ_Types(XkbFile *file, ਬਣਤਰ xkb_keymap_info *keymap_info)
{
    ਕੁੰਜੀ_Types_Info info;

    Init_ਕੁੰਜੀ_Types_Info(&info, keymap_info, 0,
                     &keymap_info->keymap.mods);

    ਜੇ (file != ਨਿਰਾ)
        Handle_ਕੁੰਜੀ_Types_File(&info, file);

    ਜੇ (info.error_ਗਿਣਤੀ != 0)
        ਜਾਓ err_info;

    ਜੇ (!Copy_ਕੁੰਜੀ_Types_ਮੰਜ਼ਿਲ_Keymap(&keymap_info->keymap, &info))
        ਜਾਓ err_info;

    Clear_ਕੁੰਜੀ_Types_Info(&info);
    ਵਾਪਸ true;

err_info:
    Clear_ਕੁੰਜੀ_Types_Info(&info);
    ਵਾਪਸ false;
}
