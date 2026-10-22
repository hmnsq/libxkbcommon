/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <string.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "context.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "include.h"
#ಅನ್ತರ್ಭಾವ "keymap-formats.h"
#ಅನ್ತರ್ಭಾವ "keymap-file-iterator.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "util-paths.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp/ast.h"

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_file_type_name(ಎಣಿಕೆ xkb_file_type type)
{
    ಆದರೆ (type > FILE_TYPE_KEYMAP)
        ಹಿಂತಿರುಗಿ "unknown";

    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *xkb_file_type_ಅಕ್ಷರಸರಣಿಗಳು[FILE_TYPE_ಅಂಕೆ_ENTRIES] = {
        [FILE_TYPE_KEYCODES] = "keycodes",
        [FILE_TYPE_TYPES] = "types",
        [FILE_TYPE_COMPAT] = "compatibility",
        [FILE_TYPE_SYMBOLS] = "symbols",
        [FILE_TYPE_GEOMETRY] = "geometry",
        [FILE_TYPE_KEYMAP] = "keymap",
    };
    ಹಿಂತಿರುಗಿ xkb_file_type_ಅಕ್ಷರಸರಣಿಗಳು[type];
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_ವಿಲೀನ_ಬಹುಲಕ_name(ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ)
{
    ಆದರೆ (ವಿಲೀನ >= ವಿಲೀನ_ಬಹುಲಕ_ಅಂಕೆ_ENTRIES)
        ಹಿಂತಿರುಗಿ "unknown";

    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ವಿಲೀನ_ಬಹುಲಕ_ಅಕ್ಷರಸರಣಿಗಳು[ವಿಲೀನ_ಬಹುಲಕ_ಅಂಕೆ_ENTRIES] = {
        [ವಿಲೀನ_DEFAULT] = "default",
        [ವಿಲೀನ_AUGMENT] = "augment",
        [ವಿಲೀನ_OVERRIDE] = "override",
        [ವಿಲೀನ_REPLACE] = "replace",
    };
    ಹಿಂತಿರುಗಿ ವಿಲೀನ_ಬಹುಲಕ_ಅಕ್ಷರಸರಣಿಗಳು[ವಿಲೀನ];
}

/* Stateful lookup of map flags names */
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_map_flags_string_iter(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *ಸೂಚ್ಯಂಕ, ಎಣಿಕೆ xkb_map_flags flags)
{
    ಆದರೆ (!flags)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ {
        ಎಣಿಕೆ xkb_map_flags flag;
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
    } ಹೆಸರುಗಳು[] = {
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

    ಆಗ (*ಸೂಚ್ಯಂಕ < ARRAY_ಗಾತ್ರ(ಹೆಸರುಗಳು)) {
        ಆದರೆ (flags & ಹೆಸರುಗಳು[*ಸೂಚ್ಯಂಕ].flag)
            ಹಿಂತಿರುಗಿ ಹೆಸರುಗಳು[(*ಸೂಚ್ಯಂಕ)++].name;
        (*ಸೂಚ್ಯಂಕ)++;
    }

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
};

FILE *
xkb_resolve_file(ರಚನೆ xkb_context *ctx,
                 ಎಣಿಕೆ xkb_file_type file_type,
                 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *map,
                 ಅಕ್ಷರ *resolved_ಮಾರ್ಗ, ಗಾತ್ರ_t resolved_ಮಾರ್ಗ_ಗಾತ್ರ,
                 ಅಕ್ಷರ *resolved_map, ಗಾತ್ರ_t resolved_map_ಗಾತ್ರ)
{
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ = 0;
    FILE *file = ಖಾಲಿ; /* Exact match */
    FILE *candidate = ಖಾಲಿ; /* Weak match */
    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಮಾರ್ಗ_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಮಾರ್ಗ);

    ಸ್ಥಿರಾಂಕ bool absolute_ಮಾರ್ಗ = is_absolute_ಮಾರ್ಗ(ಮಾರ್ಗ);
    ಆದರೆ (absolute_ಮಾರ್ಗ) {
        /* Absolute path: no need for lookup in XKB paths */
        file = fopen(ಮಾರ್ಗ, "rb");
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* Relative path: lookup the first XKB path */
        file = ಕಂಡುಹಿಡಿ_File_ಒಳ_Xkb_ಮಾರ್ಗ(ctx, "(unknown)", ಮಾರ್ಗ, ಮಾರ್ಗ_len, file_type,
                                 resolved_ಮಾರ್ಗ, resolved_ಮಾರ್ಗ_ಗಾತ್ರ, &ಸ್ಥಾನಾಂತರ,
                                 true);
    }

    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_keymap_format format = DEFAULT_ಒಳಹರಿವು_KEYMAP_FORMAT;
    ಸ್ಥಿರಾಂಕ ರಚನೆ parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    ಆಗ (file) {
        XkbFile * ಸ್ಥಿರಾಂಕ xkb_file = XkbParseFile(ctx, &config, file, ಮಾರ್ಗ, map);
        ಆದರೆ (xkb_file) {
            ಆದರೆ (file_type < FILE_TYPE_ಅಂಕೆ_ENTRIES &&
                xkb_file->file_type != file_type) {
                log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                        "File of wrong type (expected %s, got %s); "
                        "file \"%s\" ignored\n",
                        xkb_file_type_ಗಮ್ಯ_string(file_type),
                        xkb_file_type_ಗಮ್ಯ_string(xkb_file->file_type),
                        (absolute_ಮಾರ್ಗ ? ಮಾರ್ಗ : resolved_ಮಾರ್ಗ));
                ಹೋಗು invalid_file;
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (map || (xkb_file->flags && MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops in this iteration.
                 */
                ಆದರೆ (!strcpy_ಸುರಕ್ಷಿತ(resolved_map, resolved_map_ಗಾತ್ರ,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    ಹೋಗು error;
                }
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = file;
                ಆದರೆ (!strcpy_ಸುರಕ್ಷಿತ(resolved_map, resolved_map_ಗಾತ್ರ,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    ಹೋಗು error;
                }
            } ಇಲ್ಲದಿದ್ದರೆ {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                ಹೋಗು invalid_file;
            }
        } ಇಲ್ಲದಿದ್ದರೆ {
invalid_file:
            fclose(file);
            file = ಖಾಲಿ;
        }

        FreeXkbFile(xkb_file);

        ಆದರೆ (file != ಖಾಲಿ || absolute_ಮಾರ್ಗ) {
            /* Exact match or absolute path with no further file to search */
            ನಿಲ್ಲಿಸು;
        }

        ಸ್ಥಾನಾಂತರ++;
        file = ಕಂಡುಹಿಡಿ_File_ಒಳ_Xkb_ಮಾರ್ಗ(ctx, "(unknown)",
                                 ಮಾರ್ಗ, ಮಾರ್ಗ_len, file_type,
                                 resolved_ಮಾರ್ಗ, resolved_ಮಾರ್ಗ_ಗಾತ್ರ,
                                 &ಸ್ಥಾನಾಂತರ, true);
    }

    ಆದರೆ (!file) {
        /* No exact match: use weak match, if any */
        file = candidate;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (candidate) {
        /* Found exact match: discard weak match, if any */
        fclose(candidate);
    }

    ಆದರೆ (absolute_ಮಾರ್ಗ && file &&
        !strcpy_ಸುರಕ್ಷಿತ(resolved_ಮಾರ್ಗ, resolved_ಮಾರ್ಗ_ಗಾತ್ರ, ಮಾರ್ಗ)) {
        ಹೋಗು error;
    }

    ಹಿಂತಿರುಗಿ file;

error:
    log_err(ctx, XKB_ERROR_INSUFFICIENT_ಬಫರ್_ಗಾತ್ರ,
            "Cannot copy resolved path or section\n");
    fclose(file);
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಶೂನ್ಯ
xkb_file_section_init(ರಚನೆ xkb_file_section *section)
{
    darray_init(section->include_groups);
    darray_init(section->includes);
    darray_init(section->ಬಫರ್);
    darray_append(section->ಬಫರ್, '\0');
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_file_section_reset(ರಚನೆ xkb_file_section *section)
{
    darray_ಗಾತ್ರ(section->include_groups) = 0;
    darray_ಗಾತ್ರ(section->includes) = 0;
    darray_ಗಾತ್ರ(section->ಬಫರ್) = 1; /* keep the dummy string */
}

ಶೂನ್ಯ
xkb_file_section_free(ರಚನೆ xkb_file_section *section)
{
    ಆದರೆ (!section)
        ಹಿಂತಿರುಗಿ;

    darray_free(section->include_groups);
    darray_free(section->includes);
    darray_free(section->ಬಫರ್);
}

ಸ್ಥಿರ bool
xkb_file_section_ಗಣ_meta_ದತ್ತಾಂಶ(ರಚನೆ xkb_context *ctx,
                               ರಚನೆ xkb_file_section *section,
                               ಸ್ಥಿರಾಂಕ XkbFile *xkb_file)
{
    section->file_type = xkb_file->file_type;
    section->flags = xkb_file->flags;
    ಆದರೆ (xkb_file->name) {
        darray_ಗಾತ್ರ_t idx = darray_ಗಾತ್ರ(section->ಬಫರ್);
        darray_append_string0(section->ಬಫರ್, xkb_file->name);
        section->name = idx;
    } ಇಲ್ಲದಿದ್ದರೆ {
        section->name = 0;
    }
    ಹಿಂತಿರುಗಿ true;
}

/** Process a list of include statements */
ಸ್ಥಿರ bool
xkb_file_section_append_includes(ರಚನೆ xkb_context *ctx,
                                 ಎಣಿಕೆ xkb_file_iterator_flags flags,
                                 ಸ್ಥಿರಾಂಕ ರಚನೆ parser_keymap_config *config,
                                 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *section_ಮಾರ್ಗ,
                                 ರಚನೆ xkb_file_section *section,
                                 ಎಣಿಕೆ xkb_file_type file_type,
                                 IncludeStmt * include)
{
    ರಚನೆ xkb_file_include_ಗುಂಪು *ಗುಂಪು = ಖಾಲಿ;
    /* Note: this will flatten statements such as `include "pc+de"` */
    ಗಾಗಿ (IncludeStmt *stmt = include; stmt; stmt = stmt->ಮುಂದಿನ_incl) {
        ಅಕ್ಷರ buf[PATH_MAX];
        /* Parse the included file to check the include validity */
        XkbFile *xkb_file = ProcessIncludeFile(ctx, config, stmt, file_type,
                                               buf, ಗಾತ್ರ(buf));
        ಸ್ಥಿರಾಂಕ bool valid = (xkb_file != ಖಾಲಿ);
        ಆದರೆ (valid || !(flags & XKB_FILE_ITERATOR_FAIL_ON_INCLUDE_ERROR)) {
            /* Collect the strings of the statement properties */

            ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t ಮಾರ್ಗ = darray_ಗಾತ್ರ(section->ಬಫರ್);
            darray_append_string0(section->ಬಫರ್, buf);

            ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t file = darray_ಗಾತ್ರ(section->ಬಫರ್);
            darray_append_string0(section->ಬಫರ್, stmt->file);

            ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t section_name =
                (stmt->map != ಖಾಲಿ || (valid && xkb_file->name))
                    ? darray_ಗಾತ್ರ(section->ಬಫರ್)
                    : 0;
            ಆದರೆ (section_name) {
                darray_append_string0(section->ಬಫರ್,
                                      (stmt->map) ? stmt->map : xkb_file->name);
            }

            ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t modifier = (stmt->modifier)
                ? darray_ಗಾತ್ರ(section->ಬಫರ್)
                : 0;
            ಆದರೆ (modifier) {
                darray_append_string0(section->ಬಫರ್, stmt->modifier);
            }

            ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_map_flags section_flags = (valid)
                ? xkb_file->flags
                : 0;

            /* Create and append the include statement */
            ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_file_include inc = {
                .valid = valid,
                .explicit_section = (stmt->map != ಖಾಲಿ),
                .ವಿಲೀನ = stmt->ವಿಲೀನ,
                .ಮಾರ್ಗ = ಮಾರ್ಗ,
                .file = file,
                .section = section_name,
                .modifier = modifier,
                .flags = section_flags
            };
            ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t idx = darray_ಗಾತ್ರ(section->includes);
            darray_append(section->includes, inc);

            /* Update include group */
            ಆದರೆ (ಗುಂಪು == ಖಾಲಿ) {
                ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t ಗುಂಪು_idx =
                    darray_ಗಾತ್ರ(section->include_groups);
                darray_append(
                    section->include_groups,
                    (ರಚನೆ xkb_file_include_ಗುಂಪು) {.ಆರಂಭ = idx, .ಕೊನೆ = idx}
                );
                ಗುಂಪು = &darray_ಅಂಶ(section->include_groups, ಗುಂಪು_idx);
            } ಇಲ್ಲದಿದ್ದರೆ {
                ಗುಂಪು->ಕೊನೆ = idx;
            }

            FreeXkbFile(xkb_file);
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ name =
                xkb_file_section_get_string(section, section->name);
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_ಸಿಕ್ಕಿತು,
                    "%s include failure in: %s%s%s%s\n",
                    xkb_file_type_name(file_type), section_ಮಾರ್ಗ,
                    (section->name ? " (section: \"": ""), name,
                    (section->name ? "\")": ""));
            FreeXkbFile(xkb_file);
            ಹಿಂತಿರುಗಿ false;
        }
    };
    ಹಿಂತಿರುಗಿ true;
}

/* Process the AST of a section */
ಸ್ಥಿರ bool
xkb_file_section_process(ರಚನೆ xkb_context *ctx,
                         ಎಣಿಕೆ xkb_file_iterator_flags flags,
                         ಸ್ಥಿರಾಂಕ ರಚನೆ parser_keymap_config *config,
                         ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ,
                         ರಚನೆ xkb_file_section *section,
                         ಸ್ಥಿರಾಂಕ XkbFile *xkb_file)
{
    bool ok = true;
    ಗಾಗಿ (ParseCommon *stmt = xkb_file->defs; stmt; stmt = stmt->ಮುಂದಿನ) {
        ಆದರೆ (stmt->type == STMT_INCLUDE) {
            ok = xkb_file_section_append_includes(ctx, flags, config, ಮಾರ್ಗ,
                                                  section, xkb_file->file_type,
                                                  (IncludeStmt *) stmt);
            ಆದರೆ (!ok)
                ನಿಲ್ಲಿಸು;
        }
    }
    ಹಿಂತಿರುಗಿ ok;
}

bool
xkb_file_section_parse(ರಚನೆ xkb_context *ctx,
                       ಎಣಿಕೆ xkb_file_iterator_flags iterator_flags,
                       ಎಣಿಕೆ xkb_keymap_format format,
                       ಎಣಿಕೆ xkb_keymap_compile_flags compile_flags,
                       ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ include_ಆಳ,
                       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *map,
                       ರಚನೆ xkb_file_section *section)
{
    ಆದರೆ (Exceeds_Include_Max_ಆಳ(ctx, include_ಆಳ))
        ಹಿಂತಿರುಗಿ false;

    FILE *file = fopen(ಮಾರ್ಗ, "rb");
    ಆದರೆ (!file) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Cannot open file: %s\n", ಮಾರ್ಗ);
        ಹಿಂತಿರುಗಿ false;
    }

    ಸ್ಥಿರಾಂಕ ರಚನೆ parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    XkbFile *xkb_file = XkbParseFile(ctx, &config, file, ಮಾರ್ಗ, map);
    fclose(file);
    ಆದರೆ (!xkb_file) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Cannot parse map \"%s\" in file: %s\n",
                (map ? map : "(no map)"), ಮಾರ್ಗ);
        ಹಿಂತಿರುಗಿ false;
    }

    xkb_file_section_reset(section);
    ಸ್ಥಿರಾಂಕ bool no_includes = iterator_flags & XKB_FILE_ITERATOR_NO_INCLUDES;
    ಸ್ಥಿರಾಂಕ bool ok = (
        xkb_file_section_ಗಣ_meta_ದತ್ತಾಂಶ(ctx, section, xkb_file) &&
        (no_includes ||
         xkb_file_section_process(ctx, iterator_flags, &config, ಮಾರ್ಗ, section,
                                  xkb_file))
    );
    FreeXkbFile(xkb_file);
    ಹಿಂತಿರುಗಿ ok;
}

ರಚನೆ xkb_file_iterator *
xkb_file_iterator_new_from_ಬಫರ್(ರಚನೆ xkb_context *ctx,
                                  ಎಣಿಕೆ xkb_file_iterator_flags iterator_flags,
                                  ಎಣಿಕೆ xkb_keymap_format format,
                                  ಎಣಿಕೆ xkb_keymap_compile_flags compile_flags,
                                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *map,
                                  ಎಣಿಕೆ xkb_file_type file_type,
                                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string, ಗಾತ್ರ_t ಉದ್ದಳತೆ)
{
    ರಚನೆ xkb_file_iterator * ಸ್ಥಿರಾಂಕ iter = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(*iter));
    ಆದರೆ (!iter) {
        log_err(ctx, XKB_ERROR_ಹಂಚಿಕೆ_FAILURE_2,
                "Cannot allocate file iterator\n");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    iter->flags = iterator_flags;
    iter->ctx = ctx;
    iter->ಮಾರ್ಗ = ಮಾರ್ಗ;
    iter->map = map;
    iter->type = file_type;
    iter->parser_config = (ರಚನೆ parser_keymap_config) {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };
    xkb_file_section_init(&iter->section);

    ಆದರೆ (!XkbParseStringInit(ctx, &iter->scanner, string, ಉದ್ದಳತೆ, ಮಾರ್ಗ, ಖಾಲಿ)) {
        xkb_file_iterator_free(iter);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ iter;
}

ಶೂನ್ಯ
xkb_file_iterator_free(ರಚನೆ xkb_file_iterator *iter)
{
    ಆದರೆ (!iter)
        ಹಿಂತಿರುಗಿ;

    xkb_file_section_free(&iter->section);
    FreeXkbFile(iter->pending_xkb_file);
    ಬಿಡುಗಡೆ(iter);
}

bool
xkb_file_iterator_ಮುಂದಿನ(ರಚನೆ xkb_file_iterator *iter,
                       ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_file_section **section)
{
    ಆದರೆ (iter->ಪೂರ್ಣಗೊಂಡ) {
        *section = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ true;
    }

ಮುಂದಿನ:
    { /* C11 compatibility: label cannot be followed by a declaration */ }
    /* Parse next section in the file */
    XkbFile *xkb_file = ಖಾಲಿ;

    ಆದರೆ (iter->pending_xkb_file) {
        /* We are parsing the components of a keymap */
        ಆದರೆ (iter->pending_section) {
            /* Parse next component */
            xkb_file = iter->pending_section;
            ಹೋಗು parse_components;
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* No more components, try next keymap */
            FreeXkbFile(iter->pending_xkb_file);
            iter->pending_xkb_file = ಖಾಲಿ;
        }
    }

    ಆದರೆ (!Xkb_Parse_String_ಮುಂದಿನ(iter->ctx, &iter->parser_config, &iter->scanner,
                            iter->map, &xkb_file)) {
        log_err(iter->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Error while parsing section in file: %s\n", iter->ಮಾರ್ಗ);
        ಹೋಗು error;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!xkb_file) {
        /* No more sections */
        iter->ಪೂರ್ಣಗೊಂಡ = true;
        *section = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ true;
    }

parse_components:
    /* Reset section */
    xkb_file_section_reset(&iter->section);

    /* Meta data */
    ಆದರೆ (!xkb_file_section_ಗಣ_meta_ದತ್ತಾಂಶ(iter->ctx, &iter->section, xkb_file))
        ಹೋಗು error;

    /* Return current section */
    *section = &iter->section;

    ಆದರೆ (xkb_file->file_type == FILE_TYPE_KEYMAP) {
        /*
         * If it’s a keymap, then stop here.
         * Next iteration will process its components.
         */
        iter->pending_xkb_file = xkb_file;
        iter->pending_section = (XkbFile *) xkb_file->defs;
        iter->map = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ true;
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಆದರೆ (iter->type != FILE_TYPE_INVALID &&
            xkb_file->file_type != iter->type) {
            ಆದರೆ (iter->pending_xkb_file) {
                /* Within a keymap: filter out component */
                iter->pending_section = (XkbFile *) xkb_file->common.ಮುಂದಿನ;
                ಹೋಗು ಮುಂದಿನ;
            } ಇಲ್ಲದಿದ್ದರೆ {
                /* Component-specific file: type mismatch */
                log_err(iter->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                        "File type mismatch: %s, section: %s\n",
                        iter->ಮಾರ್ಗ,
                        (xkb_file->name ? xkb_file->name : "(no name)"));
                ಹೋಗು error;
            }
        }
        ಆದರೆ (iter->map) {
            iter->ಪೂರ್ಣಗೊಂಡ = true;
        }
    }

    /* Collect include statements of current section */
    ಸ್ಥಿರಾಂಕ bool process_includes = !(iter->flags & XKB_FILE_ITERATOR_NO_INCLUDES);
    ಆದರೆ (process_includes &&
        !xkb_file_section_process(iter->ctx, iter->flags, &iter->parser_config,
                                  iter->ಮಾರ್ಗ, &iter->section, xkb_file)) {
        ಹೋಗು error;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (iter->pending_section) {
        /* Next component */
        iter->pending_section = (XkbFile *) xkb_file->common.ಮುಂದಿನ;
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* No more component, free the file containing the components */
        FreeXkbFile(xkb_file);
    }

    ಹಿಂತಿರುಗಿ true;

error:
    FreeXkbFile(xkb_file);
    *section = ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ false;
}

/* Lookup a string by its index */
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_file_section_get_string(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_file_section *section, darray_ಗಾತ್ರ_t idx)
{
    ಹಿಂತಿರುಗಿ &darray_ಅಂಶ(section->ಬಫರ್, idx);
}
