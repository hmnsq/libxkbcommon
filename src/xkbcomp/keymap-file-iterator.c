/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "context.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "include.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap-formats.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap-file-iterator.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "messages-codes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-paths.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp-priv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp/ast.h"

ਅਚਲ ਅੱਖਰ *
xkb_file_type_name(ਗਿਣਤੀ xkb_file_type type)
{
    ਜੇ (type > FILE_TYPE_KEYMAP)
        ਵਾਪਸ "unknown";

    ਸਥਿਰ ਅਚਲ ਅੱਖਰ *xkb_file_type_ਲੜੀਆਂ[FILE_TYPE_ਅੰਕ_ENTRIES] = {
        [FILE_TYPE_KEYCODES] = "keycodes",
        [FILE_TYPE_TYPES] = "types",
        [FILE_TYPE_COMPAT] = "compatibility",
        [FILE_TYPE_SYMBOLS] = "symbols",
        [FILE_TYPE_GEOMETRY] = "geometry",
        [FILE_TYPE_KEYMAP] = "keymap",
    };
    ਵਾਪਸ xkb_file_type_ਲੜੀਆਂ[type];
}

ਅਚਲ ਅੱਖਰ *
xkb_ਮਿਲਾਓ_ਬਹੁਲਕ_name(ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ)
{
    ਜੇ (ਮਿਲਾਓ >= ਮਿਲਾਓ_ਬਹੁਲਕ_ਅੰਕ_ENTRIES)
        ਵਾਪਸ "unknown";

    ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਮਿਲਾਓ_ਬਹੁਲਕ_ਲੜੀਆਂ[ਮਿਲਾਓ_ਬਹੁਲਕ_ਅੰਕ_ENTRIES] = {
        [ਮਿਲਾਓ_DEFAULT] = "default",
        [ਮਿਲਾਓ_AUGMENT] = "augment",
        [ਮਿਲਾਓ_OVERRIDE] = "override",
        [ਮਿਲਾਓ_REPLACE] = "replace",
    };
    ਵਾਪਸ ਮਿਲਾਓ_ਬਹੁਲਕ_ਲੜੀਆਂ[ਮਿਲਾਓ];
}

/* Stateful lookup of map flags names */
ਅਚਲ ਅੱਖਰ *
xkb_map_flags_string_iter(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸੂਚਕਾਂਕ, ਗਿਣਤੀ xkb_map_flags flags)
{
    ਜੇ (!flags)
        ਵਾਪਸ ਨਿਰਾ;

    ਸਥਿਰ ਅਚਲ ਬਣਤਰ {
        ਗਿਣਤੀ xkb_map_flags flag;
        ਅਚਲ ਅੱਖਰ *name;
    } ਨਾਂ[] = {
        { MAP_IS_DEFAULT, "default" },
        { MAP_IS_PARTIAL, "partial" },
        { MAP_IS_HIDDEN, "hidden" },
        { MAP_HAS_ALPHANUMERIC, "alphanumeric" },
        { MAP_HAS_MODIFIER, "modifiers" },
        { MAP_HAS_KEYPAD, "keypad" },
        { MAP_HAS_FN, "fn" },
        { MAP_IS_ALTGR, "altgr" },
        { MAP_IS_DEPRECATED, "deprecated" },
    };

    ਜਦੋਂ (*ਸੂਚਕਾਂਕ < ARRAY_ਮਾਪ(ਨਾਂ)) {
        ਜੇ (flags & ਨਾਂ[*ਸੂਚਕਾਂਕ].flag)
            ਵਾਪਸ ਨਾਂ[(*ਸੂਚਕਾਂਕ)++].name;
        (*ਸੂਚਕਾਂਕ)++;
    }

    ਵਾਪਸ ਨਿਰਾ;
};

FILE *
xkb_resolve_file(ਬਣਤਰ xkb_context *ctx,
                 ਗਿਣਤੀ xkb_file_type file_type,
                 ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *map,
                 ਅੱਖਰ *resolved_ਰਸਤਾ, ਮਾਪ_t resolved_ਰਸਤਾ_ਮਾਪ,
                 ਅੱਖਰ *resolved_map, ਮਾਪ_t resolved_map_ਮਾਪ)
{
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ = 0;
    FILE *file = ਨਿਰਾ; /* Exact match */
    FILE *candidate = ਨਿਰਾ; /* Weak match */
    ਅਚਲ ਮਾਪ_t ਰਸਤਾ_len = ਲੜੀ_ਲੰਬਾਈ(ਰਸਤਾ);

    ਅਚਲ bool absolute_ਰਸਤਾ = is_absolute_ਰਸਤਾ(ਰਸਤਾ);
    ਜੇ (absolute_ਰਸਤਾ) {
        /* Absolute path: no need for lookup in XKB paths */
        file = fopen(ਰਸਤਾ, "rb");
    } ਨਹੀਂਤਾਂ {
        /* Relative path: lookup the first XKB path */
        file = ਲੱਭੋ_File_ਵਿੱਚ_Xkb_ਰਸਤਾ(ctx, "(unknown)", ਰਸਤਾ, ਰਸਤਾ_len, file_type,
                                 resolved_ਰਸਤਾ, resolved_ਰਸਤਾ_ਮਾਪ, &ਵਿੱਥ,
                                 true);
    }

    ਅਚਲ ਗਿਣਤੀ xkb_keymap_format format = DEFAULT_ਨਿਵੇਸ਼_KEYMAP_FORMAT;
    ਅਚਲ ਬਣਤਰ parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    ਜਦੋਂ (file) {
        XkbFile * ਅਚਲ xkb_file = XkbParseFile(ctx, &config, file, ਰਸਤਾ, map);
        ਜੇ (xkb_file) {
            ਜੇ (file_type < FILE_TYPE_ਅੰਕ_ENTRIES &&
                xkb_file->file_type != file_type) {
                log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                        "File of wrong type (expected %s, got %s); "
                        "file \"%s\" ignored\n",
                        xkb_file_type_ਮੰਜ਼ਿਲ_string(file_type),
                        xkb_file_type_ਮੰਜ਼ਿਲ_string(xkb_file->file_type),
                        (absolute_ਰਸਤਾ ? ਰਸਤਾ : resolved_ਰਸਤਾ));
                ਜਾਓ invalid_file;
            } ਨਹੀਂਤਾਂ ਜੇ (map || (xkb_file->flags && MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops in this iteration.
                 */
                ਜੇ (!strcpy_ਸੁਰੱਖਿਅਤ(resolved_map, resolved_map_ਮਾਪ,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    ਜਾਓ error;
                }
            } ਨਹੀਂਤਾਂ ਜੇ (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = file;
                ਜੇ (!strcpy_ਸੁਰੱਖਿਅਤ(resolved_map, resolved_map_ਮਾਪ,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    ਜਾਓ error;
                }
            } ਨਹੀਂਤਾਂ {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                ਜਾਓ invalid_file;
            }
        } ਨਹੀਂਤਾਂ {
invalid_file:
            fclose(file);
            file = ਨਿਰਾ;
        }

        FreeXkbFile(xkb_file);

        ਜੇ (file != ਨਿਰਾ || absolute_ਰਸਤਾ) {
            /* Exact match or absolute path with no further file to search */
            ਰੋਕੋ;
        }

        ਵਿੱਥ++;
        file = ਲੱਭੋ_File_ਵਿੱਚ_Xkb_ਰਸਤਾ(ctx, "(unknown)",
                                 ਰਸਤਾ, ਰਸਤਾ_len, file_type,
                                 resolved_ਰਸਤਾ, resolved_ਰਸਤਾ_ਮਾਪ,
                                 &ਵਿੱਥ, true);
    }

    ਜੇ (!file) {
        /* No exact match: use weak match, if any */
        file = candidate;
    } ਨਹੀਂਤਾਂ ਜੇ (candidate) {
        /* Found exact match: discard weak match, if any */
        fclose(candidate);
    }

    ਜੇ (absolute_ਰਸਤਾ && file &&
        !strcpy_ਸੁਰੱਖਿਅਤ(resolved_ਰਸਤਾ, resolved_ਰਸਤਾ_ਮਾਪ, ਰਸਤਾ)) {
        ਜਾਓ error;
    }

    ਵਾਪਸ file;

error:
    log_err(ctx, XKB_ERROR_INSUFFICIENT_ਬਫਰ_ਮਾਪ,
            "Cannot copy resolved path or section\n");
    fclose(file);
    ਵਾਪਸ ਨਿਰਾ;
}

ਖਾਲੀ
xkb_file_section_init(ਬਣਤਰ xkb_file_section *section)
{
    darray_init(section->include_groups);
    darray_init(section->includes);
    darray_init(section->ਬਫਰ);
    darray_append(section->ਬਫਰ, '\0');
}

ਸਥਿਰ ਖਾਲੀ
xkb_file_section_reset(ਬਣਤਰ xkb_file_section *section)
{
    darray_ਮਾਪ(section->include_groups) = 0;
    darray_ਮਾਪ(section->includes) = 0;
    darray_ਮਾਪ(section->ਬਫਰ) = 1; /* keep the dummy string */
}

ਖਾਲੀ
xkb_file_section_free(ਬਣਤਰ xkb_file_section *section)
{
    ਜੇ (!section)
        ਵਾਪਸ;

    darray_free(section->include_groups);
    darray_free(section->includes);
    darray_free(section->ਬਫਰ);
}

ਸਥਿਰ bool
xkb_file_section_ਸੈੱਟ_meta_ਅੰਕੜੇ(ਬਣਤਰ xkb_context *ctx,
                               ਬਣਤਰ xkb_file_section *section,
                               ਅਚਲ XkbFile *xkb_file)
{
    section->file_type = xkb_file->file_type;
    section->flags = xkb_file->flags;
    ਜੇ (xkb_file->name) {
        darray_ਮਾਪ_t idx = darray_ਮਾਪ(section->ਬਫਰ);
        darray_append_string0(section->ਬਫਰ, xkb_file->name);
        section->name = idx;
    } ਨਹੀਂਤਾਂ {
        section->name = 0;
    }
    ਵਾਪਸ true;
}

/** Process a list of include statements */
ਸਥਿਰ bool
xkb_file_section_append_includes(ਬਣਤਰ xkb_context *ctx,
                                 ਗਿਣਤੀ xkb_file_iterator_flags flags,
                                 ਅਚਲ ਬਣਤਰ parser_keymap_config *config,
                                 ਅਚਲ ਅੱਖਰ *section_ਰਸਤਾ,
                                 ਬਣਤਰ xkb_file_section *section,
                                 ਗਿਣਤੀ xkb_file_type file_type,
                                 IncludeStmt * include)
{
    ਬਣਤਰ xkb_file_include_ਸਮੂਹ *ਸਮੂਹ = ਨਿਰਾ;
    /* Note: this will flatten statements such as `include "pc+de"` */
    ਲਈ (IncludeStmt *stmt = include; stmt; stmt = stmt->ਅਗਲਾ_incl) {
        ਅੱਖਰ buf[PATH_MAX];
        /* Parse the included file to check the include validity */
        XkbFile *xkb_file = ProcessIncludeFile(ctx, config, stmt, file_type,
                                               buf, ਆਕਾਰ(buf));
        ਅਚਲ bool valid = (xkb_file != ਨਿਰਾ);
        ਜੇ (valid || !(flags & XKB_FILE_ITERATOR_FAIL_ON_INCLUDE_ERROR)) {
            /* Collect the strings of the statement properties */

            ਅਚਲ darray_ਮਾਪ_t ਰਸਤਾ = darray_ਮਾਪ(section->ਬਫਰ);
            darray_append_string0(section->ਬਫਰ, buf);

            ਅਚਲ darray_ਮਾਪ_t file = darray_ਮਾਪ(section->ਬਫਰ);
            darray_append_string0(section->ਬਫਰ, stmt->file);

            ਅਚਲ darray_ਮਾਪ_t section_name =
                (stmt->map != ਨਿਰਾ || (valid && xkb_file->name))
                    ? darray_ਮਾਪ(section->ਬਫਰ)
                    : 0;
            ਜੇ (section_name) {
                darray_append_string0(section->ਬਫਰ,
                                      (stmt->map) ? stmt->map : xkb_file->name);
            }

            ਅਚਲ darray_ਮਾਪ_t modifier = (stmt->modifier)
                ? darray_ਮਾਪ(section->ਬਫਰ)
                : 0;
            ਜੇ (modifier) {
                darray_append_string0(section->ਬਫਰ, stmt->modifier);
            }

            ਅਚਲ ਗਿਣਤੀ xkb_map_flags section_flags = (valid)
                ? xkb_file->flags
                : 0;

            /* Create and append the include statement */
            ਅਚਲ ਬਣਤਰ xkb_file_include inc = {
                .valid = valid,
                .explicit_section = (stmt->map != ਨਿਰਾ),
                .ਮਿਲਾਓ = stmt->ਮਿਲਾਓ,
                .ਰਸਤਾ = ਰਸਤਾ,
                .file = file,
                .section = section_name,
                .modifier = modifier,
                .flags = section_flags
            };
            ਅਚਲ darray_ਮਾਪ_t idx = darray_ਮਾਪ(section->includes);
            darray_append(section->includes, inc);

            /* Update include group */
            ਜੇ (ਸਮੂਹ == ਨਿਰਾ) {
                ਅਚਲ darray_ਮਾਪ_t ਸਮੂਹ_idx =
                    darray_ਮਾਪ(section->include_groups);
                darray_append(
                    section->include_groups,
                    (ਬਣਤਰ xkb_file_include_ਸਮੂਹ) {.ਸ਼ੁਰੂ = idx, .ਸਿਰਾ = idx}
                );
                ਸਮੂਹ = &darray_ਵਸਤੂ(section->include_groups, ਸਮੂਹ_idx);
            } ਨਹੀਂਤਾਂ {
                ਸਮੂਹ->ਸਿਰਾ = idx;
            }

            FreeXkbFile(xkb_file);
        } ਨਹੀਂਤਾਂ {
            ਅਚਲ ਅੱਖਰ * ਅਚਲ name =
                xkb_file_section_get_string(section, section->name);
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_ਮਿਲਿਆ,
                    "%s include failure in: %s%s%s%s\n",
                    xkb_file_type_name(file_type), section_ਰਸਤਾ,
                    (section->name ? " (section: \"": ""), name,
                    (section->name ? "\")": ""));
            FreeXkbFile(xkb_file);
            ਵਾਪਸ false;
        }
    };
    ਵਾਪਸ true;
}

/* Process the AST of a section */
ਸਥਿਰ bool
xkb_file_section_process(ਬਣਤਰ xkb_context *ctx,
                         ਗਿਣਤੀ xkb_file_iterator_flags flags,
                         ਅਚਲ ਬਣਤਰ parser_keymap_config *config,
                         ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
                         ਬਣਤਰ xkb_file_section *section,
                         ਅਚਲ XkbFile *xkb_file)
{
    bool ok = true;
    ਲਈ (ParseCommon *stmt = xkb_file->defs; stmt; stmt = stmt->ਅਗਲਾ) {
        ਜੇ (stmt->type == STMT_INCLUDE) {
            ok = xkb_file_section_append_includes(ctx, flags, config, ਰਸਤਾ,
                                                  section, xkb_file->file_type,
                                                  (IncludeStmt *) stmt);
            ਜੇ (!ok)
                ਰੋਕੋ;
        }
    }
    ਵਾਪਸ ok;
}

bool
xkb_file_section_parse(ਬਣਤਰ xkb_context *ctx,
                       ਗਿਣਤੀ xkb_file_iterator_flags iterator_flags,
                       ਗਿਣਤੀ xkb_keymap_format format,
                       ਗਿਣਤੀ xkb_keymap_compile_flags compile_flags,
                       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ include_ਡੂੰਘਾਈ,
                       ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *map,
                       ਬਣਤਰ xkb_file_section *section)
{
    ਜੇ (Exceeds_Include_Max_ਡੂੰਘਾਈ(ctx, include_ਡੂੰਘਾਈ))
        ਵਾਪਸ false;

    FILE *file = fopen(ਰਸਤਾ, "rb");
    ਜੇ (!file) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Cannot open file: %s\n", ਰਸਤਾ);
        ਵਾਪਸ false;
    }

    ਅਚਲ ਬਣਤਰ parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    XkbFile *xkb_file = XkbParseFile(ctx, &config, file, ਰਸਤਾ, map);
    fclose(file);
    ਜੇ (!xkb_file) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Cannot parse map \"%s\" in file: %s\n",
                (map ? map : "(no map)"), ਰਸਤਾ);
        ਵਾਪਸ false;
    }

    xkb_file_section_reset(section);
    ਅਚਲ bool no_includes = iterator_flags & XKB_FILE_ITERATOR_NO_INCLUDES;
    ਅਚਲ bool ok = (
        xkb_file_section_ਸੈੱਟ_meta_ਅੰਕੜੇ(ctx, section, xkb_file) &&
        (no_includes ||
         xkb_file_section_process(ctx, iterator_flags, &config, ਰਸਤਾ, section,
                                  xkb_file))
    );
    FreeXkbFile(xkb_file);
    ਵਾਪਸ ok;
}

ਬਣਤਰ xkb_file_iterator *
xkb_file_iterator_new_from_ਬਫਰ(ਬਣਤਰ xkb_context *ctx,
                                  ਗਿਣਤੀ xkb_file_iterator_flags iterator_flags,
                                  ਗਿਣਤੀ xkb_keymap_format format,
                                  ਗਿਣਤੀ xkb_keymap_compile_flags compile_flags,
                                  ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *map,
                                  ਗਿਣਤੀ xkb_file_type file_type,
                                  ਅਚਲ ਅੱਖਰ *string, ਮਾਪ_t ਲੰਮਾਈ)
{
    ਬਣਤਰ xkb_file_iterator * ਅਚਲ iter = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(*iter));
    ਜੇ (!iter) {
        log_err(ctx, XKB_ERROR_ਵੰਡ_FAILURE_2,
                "Cannot allocate file iterator\n");
        ਵਾਪਸ ਨਿਰਾ;
    }

    iter->flags = iterator_flags;
    iter->ctx = ctx;
    iter->ਰਸਤਾ = ਰਸਤਾ;
    iter->map = map;
    iter->type = file_type;
    iter->parser_config = (ਬਣਤਰ parser_keymap_config) {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };
    xkb_file_section_init(&iter->section);

    ਜੇ (!XkbParseStringInit(ctx, &iter->scanner, string, ਲੰਮਾਈ, ਰਸਤਾ, ਨਿਰਾ)) {
        xkb_file_iterator_free(iter);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਾਪਸ iter;
}

ਖਾਲੀ
xkb_file_iterator_free(ਬਣਤਰ xkb_file_iterator *iter)
{
    ਜੇ (!iter)
        ਵਾਪਸ;

    xkb_file_section_free(&iter->section);
    FreeXkbFile(iter->pending_xkb_file);
    ਮੁਕਤ(iter);
}

bool
xkb_file_iterator_ਅਗਲਾ(ਬਣਤਰ xkb_file_iterator *iter,
                       ਅਚਲ ਬਣਤਰ xkb_file_section **section)
{
    ਜੇ (iter->ਮੁਕੰਮਲ) {
        *section = ਨਿਰਾ;
        ਵਾਪਸ true;
    }

ਅਗਲਾ:
    { /* C11 compatibility: label cannot be followed by a declaration */ }
    /* Parse next section in the file */
    XkbFile *xkb_file = ਨਿਰਾ;

    ਜੇ (iter->pending_xkb_file) {
        /* We are parsing the components of a keymap */
        ਜੇ (iter->pending_section) {
            /* Parse next component */
            xkb_file = iter->pending_section;
            ਜਾਓ parse_components;
        } ਨਹੀਂਤਾਂ {
            /* No more components, try next keymap */
            FreeXkbFile(iter->pending_xkb_file);
            iter->pending_xkb_file = ਨਿਰਾ;
        }
    }

    ਜੇ (!Xkb_Parse_String_ਅਗਲਾ(iter->ctx, &iter->parser_config, &iter->scanner,
                            iter->map, &xkb_file)) {
        log_err(iter->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Error while parsing section in file: %s\n", iter->ਰਸਤਾ);
        ਜਾਓ error;
    } ਨਹੀਂਤਾਂ ਜੇ (!xkb_file) {
        /* No more sections */
        iter->ਮੁਕੰਮਲ = true;
        *section = ਨਿਰਾ;
        ਵਾਪਸ true;
    }

parse_components:
    /* Reset section */
    xkb_file_section_reset(&iter->section);

    /* Meta data */
    ਜੇ (!xkb_file_section_ਸੈੱਟ_meta_ਅੰਕੜੇ(iter->ctx, &iter->section, xkb_file))
        ਜਾਓ error;

    /* Return current section */
    *section = &iter->section;

    ਜੇ (xkb_file->file_type == FILE_TYPE_KEYMAP) {
        /*
         * If it’s a keymap, then stop here.
         * Next iteration will process its components.
         */
        iter->pending_xkb_file = xkb_file;
        iter->pending_section = (XkbFile *) xkb_file->defs;
        iter->map = ਨਿਰਾ;
        ਵਾਪਸ true;
    } ਨਹੀਂਤਾਂ {
        ਜੇ (iter->type != FILE_TYPE_INVALID &&
            xkb_file->file_type != iter->type) {
            ਜੇ (iter->pending_xkb_file) {
                /* Within a keymap: filter out component */
                iter->pending_section = (XkbFile *) xkb_file->common.ਅਗਲਾ;
                ਜਾਓ ਅਗਲਾ;
            } ਨਹੀਂਤਾਂ {
                /* Component-specific file: type mismatch */
                log_err(iter->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                        "File type mismatch: %s, section: %s\n",
                        iter->ਰਸਤਾ,
                        (xkb_file->name ? xkb_file->name : "(no name)"));
                ਜਾਓ error;
            }
        }
        ਜੇ (iter->map) {
            iter->ਮੁਕੰਮਲ = true;
        }
    }

    /* Collect include statements of current section */
    ਅਚਲ bool process_includes = !(iter->flags & XKB_FILE_ITERATOR_NO_INCLUDES);
    ਜੇ (process_includes &&
        !xkb_file_section_process(iter->ctx, iter->flags, &iter->parser_config,
                                  iter->ਰਸਤਾ, &iter->section, xkb_file)) {
        ਜਾਓ error;
    } ਨਹੀਂਤਾਂ ਜੇ (iter->pending_section) {
        /* Next component */
        iter->pending_section = (XkbFile *) xkb_file->common.ਅਗਲਾ;
    } ਨਹੀਂਤਾਂ {
        /* No more component, free the file containing the components */
        FreeXkbFile(xkb_file);
    }

    ਵਾਪਸ true;

error:
    FreeXkbFile(xkb_file);
    *section = ਨਿਰਾ;
    ਵਾਪਸ false;
}

/* Lookup a string by its index */
ਅਚਲ ਅੱਖਰ *
xkb_file_section_get_string(ਅਚਲ ਬਣਤਰ xkb_file_section *section, darray_ਮਾਪ_t idx)
{
    ਵਾਪਸ &darray_ਵਸਤੂ(section->ਬਫਰ, idx);
}
