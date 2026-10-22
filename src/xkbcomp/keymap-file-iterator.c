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

ಧ್ರುವ ವರ್ಣ *
xkb_file_type_name(ಗಣನಾ xkb_file_type type)
{
    ಯದಿ (type > FILE_TYPE_KEYMAP)
        ಪ್ರತ್ಯಾಗಮ "unknown";

    ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *xkb_file_type_strings[FILE_TYPE_ಅಂಕೆ_ENTRIES] = {
        [FILE_TYPE_KEYCODES] = "keycodes",
        [FILE_TYPE_TYPES] = "types",
        [FILE_TYPE_COMPAT] = "compatibility",
        [FILE_TYPE_SYMBOLS] = "symbols",
        [FILE_TYPE_GEOMETRY] = "geometry",
        [FILE_TYPE_KEYMAP] = "keymap",
    };
    ಪ್ರತ್ಯಾಗಮ xkb_file_type_strings[type];
}

ಧ್ರುವ ವರ್ಣ *
xkb_merge_mode_name(ಗಣನಾ merge_mode merge)
{
    ಯದಿ (merge >= MERGE_MODE_ಅಂಕೆ_ENTRIES)
        ಪ್ರತ್ಯಾಗಮ "unknown";

    ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *merge_mode_strings[MERGE_MODE_ಅಂಕೆ_ENTRIES] = {
        [MERGE_DEFAULT] = "default",
        [MERGE_AUGMENT] = "augment",
        [MERGE_OVERRIDE] = "override",
        [MERGE_REPLACE] = "replace",
    };
    ಪ್ರತ್ಯಾಗಮ merge_mode_strings[merge];
}

/* Stateful lookup of map flags names */
ಧ್ರುವ ವರ್ಣ *
xkb_map_flags_string_iter(unsigned ಪೂರ್ಣಾಙ್ಕ *index, ಗಣನಾ xkb_map_flags flags)
{
    ಯದಿ (!flags)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ {
        ಗಣನಾ xkb_map_flags flag;
        ಧ್ರುವ ವರ್ಣ *name;
    } names[] = {
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

    ಯಾವತ್ (*index < ARRAY_SIZE(names)) {
        ಯದಿ (flags & names[*index].flag)
            ಪ್ರತ್ಯಾಗಮ names[(*index)++].name;
        (*index)++;
    }

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
};

FILE *
xkb_resolve_file(ಸಂರಚನಾ xkb_context *ctx,
                 ಗಣನಾ xkb_file_type file_type,
                 ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *map,
                 ವರ್ಣ *resolved_path, size_t resolved_path_size,
                 ವರ್ಣ *resolved_map, size_t resolved_map_size)
{
    unsigned ಪೂರ್ಣಾಙ್ಕ offset = 0;
    FILE *file = ರಿಕ್ತ; /* Exact match */
    FILE *candidate = ರಿಕ್ತ; /* Weak match */
    ಧ್ರುವ size_t path_len = strlen(path);

    ಧ್ರುವ bool absolute_path = is_absolute_path(path);
    ಯದಿ (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        file = fopen(path, "rb");
    } ಅನ್ಯಥಾ {
        /* Relative path: lookup the first XKB path */
        file = FindFileInXkbPath(ctx, "(unknown)", path, path_len, file_type,
                                 resolved_path, resolved_path_size, &offset,
                                 true);
    }

    ಧ್ರುವ ಗಣನಾ xkb_keymap_format format = DEFAULT_ಒಳಹರಿವು_KEYMAP_FORMAT;
    ಧ್ರುವ ಸಂರಚನಾ parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    ಯಾವತ್ (file) {
        XkbFile * ಧ್ರುವ xkb_file = XkbParseFile(ctx, &config, file, path, map);
        ಯದಿ (xkb_file) {
            ಯದಿ (file_type < FILE_TYPE_ಅಂಕೆ_ENTRIES &&
                xkb_file->file_type != file_type) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File of wrong type (expected %s, got %s); "
                        "file \"%s\" ignored\n",
                        xkb_file_type_to_string(file_type),
                        xkb_file_type_to_string(xkb_file->file_type),
                        (absolute_path ? path : resolved_path));
                ಗಚ್ಛ invalid_file;
            } ಅನ್ಯಥಾ ಯದಿ (map || (xkb_file->flags && MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops in this iteration.
                 */
                ಯದಿ (!strcpy_safe(resolved_map, resolved_map_size,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    ಗಚ್ಛ error;
                }
            } ಅನ್ಯಥಾ ಯದಿ (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = file;
                ಯದಿ (!strcpy_safe(resolved_map, resolved_map_size,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    ಗಚ್ಛ error;
                }
            } ಅನ್ಯಥಾ {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                ಗಚ್ಛ invalid_file;
            }
        } ಅನ್ಯಥಾ {
invalid_file:
            fclose(file);
            file = ರಿಕ್ತ;
        }

        FreeXkbFile(xkb_file);

        ಯದಿ (file != ರಿಕ್ತ || absolute_path) {
            /* Exact match or absolute path with no further file to search */
            ಭಙ್ಗ;
        }

        offset++;
        file = FindFileInXkbPath(ctx, "(unknown)",
                                 path, path_len, file_type,
                                 resolved_path, resolved_path_size,
                                 &offset, true);
    }

    ಯದಿ (!file) {
        /* No exact match: use weak match, if any */
        file = candidate;
    } ಅನ್ಯಥಾ ಯದಿ (candidate) {
        /* Found exact match: discard weak match, if any */
        fclose(candidate);
    }

    ಯದಿ (absolute_path && file &&
        !strcpy_safe(resolved_path, resolved_path_size, path)) {
        ಗಚ್ಛ error;
    }

    ಪ್ರತ್ಯಾಗಮ file;

error:
    log_err(ctx, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
            "Cannot copy resolved path or section\n");
    fclose(file);
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಶೂನ್ಯ
xkb_file_section_init(ಸಂರಚನಾ xkb_file_section *section)
{
    darray_init(section->include_groups);
    darray_init(section->includes);
    darray_init(section->buffer);
    darray_append(section->buffer, '\0');
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_file_section_reset(ಸಂರಚನಾ xkb_file_section *section)
{
    darray_size(section->include_groups) = 0;
    darray_size(section->includes) = 0;
    darray_size(section->buffer) = 1; /* keep the dummy string */
}

ಶೂನ್ಯ
xkb_file_section_free(ಸಂರಚನಾ xkb_file_section *section)
{
    ಯದಿ (!section)
        ಪ್ರತ್ಯಾಗಮ;

    darray_free(section->include_groups);
    darray_free(section->includes);
    darray_free(section->buffer);
}

ಸ್ಥಿರ bool
xkb_file_section_set_meta_data(ಸಂರಚನಾ xkb_context *ctx,
                               ಸಂರಚನಾ xkb_file_section *section,
                               ಧ್ರುವ XkbFile *xkb_file)
{
    section->file_type = xkb_file->file_type;
    section->flags = xkb_file->flags;
    ಯದಿ (xkb_file->name) {
        darray_size_t idx = darray_size(section->buffer);
        darray_append_string0(section->buffer, xkb_file->name);
        section->name = idx;
    } ಅನ್ಯಥಾ {
        section->name = 0;
    }
    ಪ್ರತ್ಯಾಗಮ true;
}

/** Process a list of include statements */
ಸ್ಥಿರ bool
xkb_file_section_append_includes(ಸಂರಚನಾ xkb_context *ctx,
                                 ಗಣನಾ xkb_file_iterator_flags flags,
                                 ಧ್ರುವ ಸಂರಚನಾ parser_keymap_config *config,
                                 ಧ್ರುವ ವರ್ಣ *section_path,
                                 ಸಂರಚನಾ xkb_file_section *section,
                                 ಗಣನಾ xkb_file_type file_type,
                                 IncludeStmt * include)
{
    ಸಂರಚನಾ xkb_file_include_group *group = ರಿಕ್ತ;
    /* Note: this will flatten statements such as `include "pc+de"` */
    ಪ್ರತಿ (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        ವರ್ಣ buf[PATH_MAX];
        /* Parse the included file to check the include validity */
        XkbFile *xkb_file = ProcessIncludeFile(ctx, config, stmt, file_type,
                                               buf, ಪರಿಮಾಣ(buf));
        ಧ್ರುವ bool valid = (xkb_file != ರಿಕ್ತ);
        ಯದಿ (valid || !(flags & XKB_FILE_ITERATOR_FAIL_ON_INCLUDE_ERROR)) {
            /* Collect the strings of the statement properties */

            ಧ್ರುವ darray_size_t path = darray_size(section->buffer);
            darray_append_string0(section->buffer, buf);

            ಧ್ರುವ darray_size_t file = darray_size(section->buffer);
            darray_append_string0(section->buffer, stmt->file);

            ಧ್ರುವ darray_size_t section_name =
                (stmt->map != ರಿಕ್ತ || (valid && xkb_file->name))
                    ? darray_size(section->buffer)
                    : 0;
            ಯದಿ (section_name) {
                darray_append_string0(section->buffer,
                                      (stmt->map) ? stmt->map : xkb_file->name);
            }

            ಧ್ರುವ darray_size_t modifier = (stmt->modifier)
                ? darray_size(section->buffer)
                : 0;
            ಯದಿ (modifier) {
                darray_append_string0(section->buffer, stmt->modifier);
            }

            ಧ್ರುವ ಗಣನಾ xkb_map_flags section_flags = (valid)
                ? xkb_file->flags
                : 0;

            /* Create and append the include statement */
            ಧ್ರುವ ಸಂರಚನಾ xkb_file_include inc = {
                .valid = valid,
                .explicit_section = (stmt->map != ರಿಕ್ತ),
                .merge = stmt->merge,
                .path = path,
                .file = file,
                .section = section_name,
                .modifier = modifier,
                .flags = section_flags
            };
            ಧ್ರುವ darray_size_t idx = darray_size(section->includes);
            darray_append(section->includes, inc);

            /* Update include group */
            ಯದಿ (group == ರಿಕ್ತ) {
                ಧ್ರುವ darray_size_t group_idx =
                    darray_size(section->include_groups);
                darray_append(
                    section->include_groups,
                    (ಸಂರಚನಾ xkb_file_include_group) {.start = idx, .end = idx}
                );
                group = &darray_item(section->include_groups, group_idx);
            } ಅನ್ಯಥಾ {
                group->end = idx;
            }

            FreeXkbFile(xkb_file);
        } ಅನ್ಯಥಾ {
            ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ name =
                xkb_file_section_get_string(section, section->name);
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "%s include failure in: %s%s%s%s\n",
                    xkb_file_type_name(file_type), section_path,
                    (section->name ? " (section: \"": ""), name,
                    (section->name ? "\")": ""));
            FreeXkbFile(xkb_file);
            ಪ್ರತ್ಯಾಗಮ false;
        }
    };
    ಪ್ರತ್ಯಾಗಮ true;
}

/* Process the AST of a section */
ಸ್ಥಿರ bool
xkb_file_section_process(ಸಂರಚನಾ xkb_context *ctx,
                         ಗಣನಾ xkb_file_iterator_flags flags,
                         ಧ್ರುವ ಸಂರಚನಾ parser_keymap_config *config,
                         ಧ್ರುವ ವರ್ಣ *path,
                         ಸಂರಚನಾ xkb_file_section *section,
                         ಧ್ರುವ XkbFile *xkb_file)
{
    bool ok = true;
    ಪ್ರತಿ (ParseCommon *stmt = xkb_file->defs; stmt; stmt = stmt->next) {
        ಯದಿ (stmt->type == STMT_INCLUDE) {
            ok = xkb_file_section_append_includes(ctx, flags, config, path,
                                                  section, xkb_file->file_type,
                                                  (IncludeStmt *) stmt);
            ಯದಿ (!ok)
                ಭಙ್ಗ;
        }
    }
    ಪ್ರತ್ಯಾಗಮ ok;
}

bool
xkb_file_section_parse(ಸಂರಚನಾ xkb_context *ctx,
                       ಗಣನಾ xkb_file_iterator_flags iterator_flags,
                       ಗಣನಾ xkb_keymap_format format,
                       ಗಣನಾ xkb_keymap_compile_flags compile_flags,
                       unsigned ಪೂರ್ಣಾಙ್ಕ include_depth,
                       ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *map,
                       ಸಂರಚನಾ xkb_file_section *section)
{
    ಯದಿ (ExceedsIncludeMaxDepth(ctx, include_depth))
        ಪ್ರತ್ಯಾಗಮ false;

    FILE *file = fopen(path, "rb");
    ಯದಿ (!file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot open file: %s\n", path);
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಧ್ರುವ ಸಂರಚನಾ parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    XkbFile *xkb_file = XkbParseFile(ctx, &config, file, path, map);
    fclose(file);
    ಯದಿ (!xkb_file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot parse map \"%s\" in file: %s\n",
                (map ? map : "(no map)"), path);
        ಪ್ರತ್ಯಾಗಮ false;
    }

    xkb_file_section_reset(section);
    ಧ್ರುವ bool no_includes = iterator_flags & XKB_FILE_ITERATOR_NO_INCLUDES;
    ಧ್ರುವ bool ok = (
        xkb_file_section_set_meta_data(ctx, section, xkb_file) &&
        (no_includes ||
         xkb_file_section_process(ctx, iterator_flags, &config, path, section,
                                  xkb_file))
    );
    FreeXkbFile(xkb_file);
    ಪ್ರತ್ಯಾಗಮ ok;
}

ಸಂರಚನಾ xkb_file_iterator *
xkb_file_iterator_new_from_buffer(ಸಂರಚನಾ xkb_context *ctx,
                                  ಗಣನಾ xkb_file_iterator_flags iterator_flags,
                                  ಗಣನಾ xkb_keymap_format format,
                                  ಗಣನಾ xkb_keymap_compile_flags compile_flags,
                                  ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *map,
                                  ಗಣನಾ xkb_file_type file_type,
                                  ಧ್ರುವ ವರ್ಣ *string, size_t length)
{
    ಸಂರಚನಾ xkb_file_iterator * ಧ್ರುವ iter = calloc(1, ಪರಿಮಾಣ(*iter));
    ಯದಿ (!iter) {
        log_err(ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate file iterator\n");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    iter->flags = iterator_flags;
    iter->ctx = ctx;
    iter->path = path;
    iter->map = map;
    iter->type = file_type;
    iter->parser_config = (ಸಂರಚನಾ parser_keymap_config) {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };
    xkb_file_section_init(&iter->section);

    ಯದಿ (!XkbParseStringInit(ctx, &iter->scanner, string, length, path, ರಿಕ್ತ)) {
        xkb_file_iterator_free(iter);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಪ್ರತ್ಯಾಗಮ iter;
}

ಶೂನ್ಯ
xkb_file_iterator_free(ಸಂರಚನಾ xkb_file_iterator *iter)
{
    ಯದಿ (!iter)
        ಪ್ರತ್ಯಾಗಮ;

    xkb_file_section_free(&iter->section);
    FreeXkbFile(iter->pending_xkb_file);
    ಮುಕ್ತ(iter);
}

bool
xkb_file_iterator_next(ಸಂರಚನಾ xkb_file_iterator *iter,
                       ಧ್ರುವ ಸಂರಚನಾ xkb_file_section **section)
{
    ಯದಿ (iter->finished) {
        *section = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ true;
    }

next:
    { /* C11 compatibility: label cannot be followed by a declaration */ }
    /* Parse next section in the file */
    XkbFile *xkb_file = ರಿಕ್ತ;

    ಯದಿ (iter->pending_xkb_file) {
        /* We are parsing the components of a keymap */
        ಯದಿ (iter->pending_section) {
            /* Parse next component */
            xkb_file = iter->pending_section;
            ಗಚ್ಛ parse_components;
        } ಅನ್ಯಥಾ {
            /* No more components, try next keymap */
            FreeXkbFile(iter->pending_xkb_file);
            iter->pending_xkb_file = ರಿಕ್ತ;
        }
    }

    ಯದಿ (!XkbParseStringNext(iter->ctx, &iter->parser_config, &iter->scanner,
                            iter->map, &xkb_file)) {
        log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Error while parsing section in file: %s\n", iter->path);
        ಗಚ್ಛ error;
    } ಅನ್ಯಥಾ ಯದಿ (!xkb_file) {
        /* No more sections */
        iter->finished = true;
        *section = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ true;
    }

parse_components:
    /* Reset section */
    xkb_file_section_reset(&iter->section);

    /* Meta data */
    ಯದಿ (!xkb_file_section_set_meta_data(iter->ctx, &iter->section, xkb_file))
        ಗಚ್ಛ error;

    /* Return current section */
    *section = &iter->section;

    ಯದಿ (xkb_file->file_type == FILE_TYPE_KEYMAP) {
        /*
         * If it’s a keymap, then stop here.
         * Next iteration will process its components.
         */
        iter->pending_xkb_file = xkb_file;
        iter->pending_section = (XkbFile *) xkb_file->defs;
        iter->map = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ true;
    } ಅನ್ಯಥಾ {
        ಯದಿ (iter->type != FILE_TYPE_INVALID &&
            xkb_file->file_type != iter->type) {
            ಯದಿ (iter->pending_xkb_file) {
                /* Within a keymap: filter out component */
                iter->pending_section = (XkbFile *) xkb_file->common.next;
                ಗಚ್ಛ next;
            } ಅನ್ಯಥಾ {
                /* Component-specific file: type mismatch */
                log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File type mismatch: %s, section: %s\n",
                        iter->path,
                        (xkb_file->name ? xkb_file->name : "(no name)"));
                ಗಚ್ಛ error;
            }
        }
        ಯದಿ (iter->map) {
            iter->finished = true;
        }
    }

    /* Collect include statements of current section */
    ಧ್ರುವ bool process_includes = !(iter->flags & XKB_FILE_ITERATOR_NO_INCLUDES);
    ಯದಿ (process_includes &&
        !xkb_file_section_process(iter->ctx, iter->flags, &iter->parser_config,
                                  iter->path, &iter->section, xkb_file)) {
        ಗಚ್ಛ error;
    } ಅನ್ಯಥಾ ಯದಿ (iter->pending_section) {
        /* Next component */
        iter->pending_section = (XkbFile *) xkb_file->common.next;
    } ಅನ್ಯಥಾ {
        /* No more component, free the file containing the components */
        FreeXkbFile(xkb_file);
    }

    ಪ್ರತ್ಯಾಗಮ true;

error:
    FreeXkbFile(xkb_file);
    *section = ರಿಕ್ತ;
    ಪ್ರತ್ಯಾಗಮ false;
}

/* Lookup a string by its index */
ಧ್ರುವ ವರ್ಣ *
xkb_file_section_get_string(ಧ್ರುವ ಸಂರಚನಾ xkb_file_section *section, darray_size_t idx)
{
    ಪ್ರತ್ಯಾಗಮ &darray_item(section->buffer, idx);
}
