/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل <limits.h>
#شامل <string.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "context.h"
#شامل "darray.h"
#شامل "include.h"
#شامل "keymap-formats.h"
#شامل "keymap-file-iterator.h"
#شامل "messages-codes.h"
#شامل "utils.h"
#شامل "util-paths.h"
#شامل "xkbcomp-priv.h"
#شامل "xkbcomp/ast.h"

ثابت نویسه *
xkb_file_type_name(شمارش xkb_file_type type)
{
    اگر (type > FILE_TYPE_KEYMAP)
        بازگشت "unknown";

    ایستا ثابت نویسه *xkb_file_type_strings[FILE_TYPE_رقم_ENTRIES] = {
        [FILE_TYPE_KEYCODES] = "keycodes",
        [FILE_TYPE_TYPES] = "types",
        [FILE_TYPE_COMPAT] = "compatibility",
        [FILE_TYPE_SYMBOLS] = "symbols",
        [FILE_TYPE_GEOMETRY] = "geometry",
        [FILE_TYPE_KEYMAP] = "keymap",
    };
    بازگشت xkb_file_type_strings[type];
}

ثابت نویسه *
xkb_merge_mode_name(شمارش merge_mode merge)
{
    اگر (merge >= MERGE_MODE_رقم_ENTRIES)
        بازگشت "unknown";

    ایستا ثابت نویسه *merge_mode_strings[MERGE_MODE_رقم_ENTRIES] = {
        [MERGE_DEFAULT] = "default",
        [MERGE_AUGMENT] = "augment",
        [MERGE_OVERRIDE] = "override",
        [MERGE_REPLACE] = "replace",
    };
    بازگشت merge_mode_strings[merge];
}

/* Stateful lookup of map flags names */
ثابت نویسه *
xkb_map_flags_string_iter(unsigned صحیح *index, شمارش xkb_map_flags flags)
{
    اگر (!flags)
        بازگشت پوچ;

    ایستا ثابت ساختار {
        شمارش xkb_map_flags flag;
        ثابت نویسه *name;
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

    وقتی (*index < ARRAY_SIZE(names)) {
        اگر (flags & names[*index].flag)
            بازگشت names[(*index)++].name;
        (*index)++;
    }

    بازگشت پوچ;
};

FILE *
xkb_resolve_file(ساختار xkb_context *ctx,
                 شمارش xkb_file_type file_type,
                 ثابت نویسه *path, ثابت نویسه *map,
                 نویسه *resolved_path, size_t resolved_path_size,
                 نویسه *resolved_map, size_t resolved_map_size)
{
    unsigned صحیح offset = 0;
    FILE *file = پوچ; /* Exact match */
    FILE *candidate = پوچ; /* Weak match */
    ثابت size_t path_len = strlen(path);

    ثابت bool absolute_path = is_absolute_path(path);
    اگر (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        file = fopen(path, "rb");
    } وگرنه {
        /* Relative path: lookup the first XKB path */
        file = FindFileInXkbPath(ctx, "(unknown)", path, path_len, file_type,
                                 resolved_path, resolved_path_size, &offset,
                                 true);
    }

    ثابت شمارش xkb_keymap_format format = DEFAULT_درون_داد_KEYMAP_FORMAT;
    ثابت ساختار parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    وقتی (file) {
        XkbFile * ثابت xkb_file = XkbParseFile(ctx, &config, file, path, map);
        اگر (xkb_file) {
            اگر (file_type < FILE_TYPE_رقم_ENTRIES &&
                xkb_file->file_type != file_type) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File of wrong type (expected %s, got %s); "
                        "file \"%s\" ignored\n",
                        xkb_file_type_to_string(file_type),
                        xkb_file_type_to_string(xkb_file->file_type),
                        (absolute_path ? path : resolved_path));
                برو invalid_file;
            } وگرنه اگر (map || (xkb_file->flags && MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops in this iteration.
                 */
                اگر (!strcpy_safe(resolved_map, resolved_map_size,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    برو error;
                }
            } وگرنه اگر (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = file;
                اگر (!strcpy_safe(resolved_map, resolved_map_size,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    برو error;
                }
            } وگرنه {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                برو invalid_file;
            }
        } وگرنه {
invalid_file:
            fclose(file);
            file = پوچ;
        }

        FreeXkbFile(xkb_file);

        اگر (file != پوچ || absolute_path) {
            /* Exact match or absolute path with no further file to search */
            بشکن;
        }

        offset++;
        file = FindFileInXkbPath(ctx, "(unknown)",
                                 path, path_len, file_type,
                                 resolved_path, resolved_path_size,
                                 &offset, true);
    }

    اگر (!file) {
        /* No exact match: use weak match, if any */
        file = candidate;
    } وگرنه اگر (candidate) {
        /* Found exact match: discard weak match, if any */
        fclose(candidate);
    }

    اگر (absolute_path && file &&
        !strcpy_safe(resolved_path, resolved_path_size, path)) {
        برو error;
    }

    بازگشت file;

error:
    log_err(ctx, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
            "Cannot copy resolved path or section\n");
    fclose(file);
    بازگشت پوچ;
}

تهی
xkb_file_section_init(ساختار xkb_file_section *section)
{
    darray_init(section->include_groups);
    darray_init(section->includes);
    darray_init(section->buffer);
    darray_append(section->buffer, '\0');
}

ایستا تهی
xkb_file_section_reset(ساختار xkb_file_section *section)
{
    darray_size(section->include_groups) = 0;
    darray_size(section->includes) = 0;
    darray_size(section->buffer) = 1; /* keep the dummy string */
}

تهی
xkb_file_section_free(ساختار xkb_file_section *section)
{
    اگر (!section)
        بازگشت;

    darray_free(section->include_groups);
    darray_free(section->includes);
    darray_free(section->buffer);
}

ایستا bool
xkb_file_section_set_meta_data(ساختار xkb_context *ctx,
                               ساختار xkb_file_section *section,
                               ثابت XkbFile *xkb_file)
{
    section->file_type = xkb_file->file_type;
    section->flags = xkb_file->flags;
    اگر (xkb_file->name) {
        darray_size_t idx = darray_size(section->buffer);
        darray_append_string0(section->buffer, xkb_file->name);
        section->name = idx;
    } وگرنه {
        section->name = 0;
    }
    بازگشت true;
}

/** Process a list of include statements */
ایستا bool
xkb_file_section_append_includes(ساختار xkb_context *ctx,
                                 شمارش xkb_file_iterator_flags flags,
                                 ثابت ساختار parser_keymap_config *config,
                                 ثابت نویسه *section_path,
                                 ساختار xkb_file_section *section,
                                 شمارش xkb_file_type file_type,
                                 IncludeStmt * include)
{
    ساختار xkb_file_include_group *group = پوچ;
    /* Note: this will flatten statements such as `include "pc+de"` */
    برای (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        نویسه buf[PATH_MAX];
        /* Parse the included file to check the include validity */
        XkbFile *xkb_file = ProcessIncludeFile(ctx, config, stmt, file_type,
                                               buf, اندازه(buf));
        ثابت bool valid = (xkb_file != پوچ);
        اگر (valid || !(flags & XKB_FILE_ITERATOR_FAIL_ON_INCLUDE_ERROR)) {
            /* Collect the strings of the statement properties */

            ثابت darray_size_t path = darray_size(section->buffer);
            darray_append_string0(section->buffer, buf);

            ثابت darray_size_t file = darray_size(section->buffer);
            darray_append_string0(section->buffer, stmt->file);

            ثابت darray_size_t section_name =
                (stmt->map != پوچ || (valid && xkb_file->name))
                    ? darray_size(section->buffer)
                    : 0;
            اگر (section_name) {
                darray_append_string0(section->buffer,
                                      (stmt->map) ? stmt->map : xkb_file->name);
            }

            ثابت darray_size_t modifier = (stmt->modifier)
                ? darray_size(section->buffer)
                : 0;
            اگر (modifier) {
                darray_append_string0(section->buffer, stmt->modifier);
            }

            ثابت شمارش xkb_map_flags section_flags = (valid)
                ? xkb_file->flags
                : 0;

            /* Create and append the include statement */
            ثابت ساختار xkb_file_include inc = {
                .valid = valid,
                .explicit_section = (stmt->map != پوچ),
                .merge = stmt->merge,
                .path = path,
                .file = file,
                .section = section_name,
                .modifier = modifier,
                .flags = section_flags
            };
            ثابت darray_size_t idx = darray_size(section->includes);
            darray_append(section->includes, inc);

            /* Update include group */
            اگر (group == پوچ) {
                ثابت darray_size_t group_idx =
                    darray_size(section->include_groups);
                darray_append(
                    section->include_groups,
                    (ساختار xkb_file_include_group) {.start = idx, .end = idx}
                );
                group = &darray_item(section->include_groups, group_idx);
            } وگرنه {
                group->end = idx;
            }

            FreeXkbFile(xkb_file);
        } وگرنه {
            ثابت نویسه * ثابت name =
                xkb_file_section_get_string(section, section->name);
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "%s include failure in: %s%s%s%s\n",
                    xkb_file_type_name(file_type), section_path,
                    (section->name ? " (section: \"": ""), name,
                    (section->name ? "\")": ""));
            FreeXkbFile(xkb_file);
            بازگشت false;
        }
    };
    بازگشت true;
}

/* Process the AST of a section */
ایستا bool
xkb_file_section_process(ساختار xkb_context *ctx,
                         شمارش xkb_file_iterator_flags flags,
                         ثابت ساختار parser_keymap_config *config,
                         ثابت نویسه *path,
                         ساختار xkb_file_section *section,
                         ثابت XkbFile *xkb_file)
{
    bool ok = true;
    برای (ParseCommon *stmt = xkb_file->defs; stmt; stmt = stmt->next) {
        اگر (stmt->type == STMT_INCLUDE) {
            ok = xkb_file_section_append_includes(ctx, flags, config, path,
                                                  section, xkb_file->file_type,
                                                  (IncludeStmt *) stmt);
            اگر (!ok)
                بشکن;
        }
    }
    بازگشت ok;
}

bool
xkb_file_section_parse(ساختار xkb_context *ctx,
                       شمارش xkb_file_iterator_flags iterator_flags,
                       شمارش xkb_keymap_format format,
                       شمارش xkb_keymap_compile_flags compile_flags,
                       unsigned صحیح include_depth,
                       ثابت نویسه *path, ثابت نویسه *map,
                       ساختار xkb_file_section *section)
{
    اگر (ExceedsIncludeMaxDepth(ctx, include_depth))
        بازگشت false;

    FILE *file = fopen(path, "rb");
    اگر (!file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot open file: %s\n", path);
        بازگشت false;
    }

    ثابت ساختار parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    XkbFile *xkb_file = XkbParseFile(ctx, &config, file, path, map);
    fclose(file);
    اگر (!xkb_file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot parse map \"%s\" in file: %s\n",
                (map ? map : "(no map)"), path);
        بازگشت false;
    }

    xkb_file_section_reset(section);
    ثابت bool no_includes = iterator_flags & XKB_FILE_ITERATOR_NO_INCLUDES;
    ثابت bool ok = (
        xkb_file_section_set_meta_data(ctx, section, xkb_file) &&
        (no_includes ||
         xkb_file_section_process(ctx, iterator_flags, &config, path, section,
                                  xkb_file))
    );
    FreeXkbFile(xkb_file);
    بازگشت ok;
}

ساختار xkb_file_iterator *
xkb_file_iterator_new_from_buffer(ساختار xkb_context *ctx,
                                  شمارش xkb_file_iterator_flags iterator_flags,
                                  شمارش xkb_keymap_format format,
                                  شمارش xkb_keymap_compile_flags compile_flags,
                                  ثابت نویسه *path, ثابت نویسه *map,
                                  شمارش xkb_file_type file_type,
                                  ثابت نویسه *string, size_t length)
{
    ساختار xkb_file_iterator * ثابت iter = calloc(1, اندازه(*iter));
    اگر (!iter) {
        log_err(ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate file iterator\n");
        بازگشت پوچ;
    }

    iter->flags = iterator_flags;
    iter->ctx = ctx;
    iter->path = path;
    iter->map = map;
    iter->type = file_type;
    iter->parser_config = (ساختار parser_keymap_config) {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };
    xkb_file_section_init(&iter->section);

    اگر (!XkbParseStringInit(ctx, &iter->scanner, string, length, path, پوچ)) {
        xkb_file_iterator_free(iter);
        بازگشت پوچ;
    }

    بازگشت iter;
}

تهی
xkb_file_iterator_free(ساختار xkb_file_iterator *iter)
{
    اگر (!iter)
        بازگشت;

    xkb_file_section_free(&iter->section);
    FreeXkbFile(iter->pending_xkb_file);
    آزاد(iter);
}

bool
xkb_file_iterator_next(ساختار xkb_file_iterator *iter,
                       ثابت ساختار xkb_file_section **section)
{
    اگر (iter->finished) {
        *section = پوچ;
        بازگشت true;
    }

next:
    { /* C11 compatibility: label cannot be followed by a declaration */ }
    /* Parse next section in the file */
    XkbFile *xkb_file = پوچ;

    اگر (iter->pending_xkb_file) {
        /* We are parsing the components of a keymap */
        اگر (iter->pending_section) {
            /* Parse next component */
            xkb_file = iter->pending_section;
            برو parse_components;
        } وگرنه {
            /* No more components, try next keymap */
            FreeXkbFile(iter->pending_xkb_file);
            iter->pending_xkb_file = پوچ;
        }
    }

    اگر (!XkbParseStringNext(iter->ctx, &iter->parser_config, &iter->scanner,
                            iter->map, &xkb_file)) {
        log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Error while parsing section in file: %s\n", iter->path);
        برو error;
    } وگرنه اگر (!xkb_file) {
        /* No more sections */
        iter->finished = true;
        *section = پوچ;
        بازگشت true;
    }

parse_components:
    /* Reset section */
    xkb_file_section_reset(&iter->section);

    /* Meta data */
    اگر (!xkb_file_section_set_meta_data(iter->ctx, &iter->section, xkb_file))
        برو error;

    /* Return current section */
    *section = &iter->section;

    اگر (xkb_file->file_type == FILE_TYPE_KEYMAP) {
        /*
         * If it’s a keymap, then stop here.
         * Next iteration will process its components.
         */
        iter->pending_xkb_file = xkb_file;
        iter->pending_section = (XkbFile *) xkb_file->defs;
        iter->map = پوچ;
        بازگشت true;
    } وگرنه {
        اگر (iter->type != FILE_TYPE_INVALID &&
            xkb_file->file_type != iter->type) {
            اگر (iter->pending_xkb_file) {
                /* Within a keymap: filter out component */
                iter->pending_section = (XkbFile *) xkb_file->common.next;
                برو next;
            } وگرنه {
                /* Component-specific file: type mismatch */
                log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File type mismatch: %s, section: %s\n",
                        iter->path,
                        (xkb_file->name ? xkb_file->name : "(no name)"));
                برو error;
            }
        }
        اگر (iter->map) {
            iter->finished = true;
        }
    }

    /* Collect include statements of current section */
    ثابت bool process_includes = !(iter->flags & XKB_FILE_ITERATOR_NO_INCLUDES);
    اگر (process_includes &&
        !xkb_file_section_process(iter->ctx, iter->flags, &iter->parser_config,
                                  iter->path, &iter->section, xkb_file)) {
        برو error;
    } وگرنه اگر (iter->pending_section) {
        /* Next component */
        iter->pending_section = (XkbFile *) xkb_file->common.next;
    } وگرنه {
        /* No more component, free the file containing the components */
        FreeXkbFile(xkb_file);
    }

    بازگشت true;

error:
    FreeXkbFile(xkb_file);
    *section = پوچ;
    بازگشت false;
}

/* Lookup a string by its index */
ثابت نویسه *
xkb_file_section_get_string(ثابت ساختار xkb_file_section *section, darray_size_t idx)
{
    بازگشت &darray_item(section->buffer, idx);
}
