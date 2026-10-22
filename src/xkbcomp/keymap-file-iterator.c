/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <limits.h>
#অন্তর্ভাব <string.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "context.h"
#অন্তর্ভাব "darray.h"
#অন্তর্ভাব "include.h"
#অন্তর্ভাব "keymap-formats.h"
#অন্তর্ভাব "keymap-file-iterator.h"
#অন্তর্ভাব "messages-codes.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "util-paths.h"
#অন্তর্ভাব "xkbcomp-priv.h"
#অন্তর্ভাব "xkbcomp/ast.h"

ধ্রুব বর্ণ *
xkb_file_type_name(গণনা xkb_file_type type)
{
    যদি (type > FILE_TYPE_KEYMAP)
        প্রত্যাগম "unknown";

    স্থির ধ্রুব বর্ণ *xkb_file_type_strings[FILE_TYPE_অঙ্ক_ENTRIES] = {
        [FILE_TYPE_KEYCODES] = "keycodes",
        [FILE_TYPE_TYPES] = "types",
        [FILE_TYPE_COMPAT] = "compatibility",
        [FILE_TYPE_SYMBOLS] = "symbols",
        [FILE_TYPE_GEOMETRY] = "geometry",
        [FILE_TYPE_KEYMAP] = "keymap",
    };
    প্রত্যাগম xkb_file_type_strings[type];
}

ধ্রুব বর্ণ *
xkb_merge_mode_name(গণনা merge_mode merge)
{
    যদি (merge >= MERGE_MODE_অঙ্ক_ENTRIES)
        প্রত্যাগম "unknown";

    স্থির ধ্রুব বর্ণ *merge_mode_strings[MERGE_MODE_অঙ্ক_ENTRIES] = {
        [MERGE_DEFAULT] = "default",
        [MERGE_AUGMENT] = "augment",
        [MERGE_OVERRIDE] = "override",
        [MERGE_REPLACE] = "replace",
    };
    প্রত্যাগম merge_mode_strings[merge];
}

/* Stateful lookup of map flags names */
ধ্রুব বর্ণ *
xkb_map_flags_string_iter(unsigned পূর্ণাঙ্ক *index, গণনা xkb_map_flags flags)
{
    যদি (!flags)
        প্রত্যাগম রিক্ত;

    স্থির ধ্রুব সংরচনা {
        গণনা xkb_map_flags flag;
        ধ্রুব বর্ণ *name;
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

    যাবত্ (*index < ARRAY_SIZE(names)) {
        যদি (flags & names[*index].flag)
            প্রত্যাগম names[(*index)++].name;
        (*index)++;
    }

    প্রত্যাগম রিক্ত;
};

FILE *
xkb_resolve_file(সংরচনা xkb_context *ctx,
                 গণনা xkb_file_type file_type,
                 ধ্রুব বর্ণ *path, ধ্রুব বর্ণ *map,
                 বর্ণ *resolved_path, size_t resolved_path_size,
                 বর্ণ *resolved_map, size_t resolved_map_size)
{
    unsigned পূর্ণাঙ্ক offset = 0;
    FILE *file = রিক্ত; /* Exact match */
    FILE *candidate = রিক্ত; /* Weak match */
    ধ্রুব size_t path_len = strlen(path);

    ধ্রুব bool absolute_path = is_absolute_path(path);
    যদি (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        file = fopen(path, "rb");
    } অন্যথা {
        /* Relative path: lookup the first XKB path */
        file = FindFileInXkbPath(ctx, "(unknown)", path, path_len, file_type,
                                 resolved_path, resolved_path_size, &offset,
                                 true);
    }

    ধ্রুব গণনা xkb_keymap_format format = DEFAULT_প্রবেশ_KEYMAP_FORMAT;
    ধ্রুব সংরচনা parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    যাবত্ (file) {
        XkbFile * ধ্রুব xkb_file = XkbParseFile(ctx, &config, file, path, map);
        যদি (xkb_file) {
            যদি (file_type < FILE_TYPE_অঙ্ক_ENTRIES &&
                xkb_file->file_type != file_type) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File of wrong type (expected %s, got %s); "
                        "file \"%s\" ignored\n",
                        xkb_file_type_to_string(file_type),
                        xkb_file_type_to_string(xkb_file->file_type),
                        (absolute_path ? path : resolved_path));
                গচ্ছ invalid_file;
            } অন্যথা যদি (map || (xkb_file->flags && MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops in this iteration.
                 */
                যদি (!strcpy_safe(resolved_map, resolved_map_size,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    গচ্ছ error;
                }
            } অন্যথা যদি (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = file;
                যদি (!strcpy_safe(resolved_map, resolved_map_size,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    গচ্ছ error;
                }
            } অন্যথা {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                গচ্ছ invalid_file;
            }
        } অন্যথা {
invalid_file:
            fclose(file);
            file = রিক্ত;
        }

        FreeXkbFile(xkb_file);

        যদি (file != রিক্ত || absolute_path) {
            /* Exact match or absolute path with no further file to search */
            ভঙ্গ;
        }

        offset++;
        file = FindFileInXkbPath(ctx, "(unknown)",
                                 path, path_len, file_type,
                                 resolved_path, resolved_path_size,
                                 &offset, true);
    }

    যদি (!file) {
        /* No exact match: use weak match, if any */
        file = candidate;
    } অন্যথা যদি (candidate) {
        /* Found exact match: discard weak match, if any */
        fclose(candidate);
    }

    যদি (absolute_path && file &&
        !strcpy_safe(resolved_path, resolved_path_size, path)) {
        গচ্ছ error;
    }

    প্রত্যাগম file;

error:
    log_err(ctx, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
            "Cannot copy resolved path or section\n");
    fclose(file);
    প্রত্যাগম রিক্ত;
}

শূন্য
xkb_file_section_init(সংরচনা xkb_file_section *section)
{
    darray_init(section->include_groups);
    darray_init(section->includes);
    darray_init(section->buffer);
    darray_append(section->buffer, '\0');
}

স্থির শূন্য
xkb_file_section_reset(সংরচনা xkb_file_section *section)
{
    darray_size(section->include_groups) = 0;
    darray_size(section->includes) = 0;
    darray_size(section->buffer) = 1; /* keep the dummy string */
}

শূন্য
xkb_file_section_free(সংরচনা xkb_file_section *section)
{
    যদি (!section)
        প্রত্যাগম;

    darray_free(section->include_groups);
    darray_free(section->includes);
    darray_free(section->buffer);
}

স্থির bool
xkb_file_section_set_meta_data(সংরচনা xkb_context *ctx,
                               সংরচনা xkb_file_section *section,
                               ধ্রুব XkbFile *xkb_file)
{
    section->file_type = xkb_file->file_type;
    section->flags = xkb_file->flags;
    যদি (xkb_file->name) {
        darray_size_t idx = darray_size(section->buffer);
        darray_append_string0(section->buffer, xkb_file->name);
        section->name = idx;
    } অন্যথা {
        section->name = 0;
    }
    প্রত্যাগম true;
}

/** Process a list of include statements */
স্থির bool
xkb_file_section_append_includes(সংরচনা xkb_context *ctx,
                                 গণনা xkb_file_iterator_flags flags,
                                 ধ্রুব সংরচনা parser_keymap_config *config,
                                 ধ্রুব বর্ণ *section_path,
                                 সংরচনা xkb_file_section *section,
                                 গণনা xkb_file_type file_type,
                                 IncludeStmt * include)
{
    সংরচনা xkb_file_include_group *group = রিক্ত;
    /* Note: this will flatten statements such as `include "pc+de"` */
    প্রতি (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        বর্ণ buf[PATH_MAX];
        /* Parse the included file to check the include validity */
        XkbFile *xkb_file = ProcessIncludeFile(ctx, config, stmt, file_type,
                                               buf, পরিমাণ(buf));
        ধ্রুব bool valid = (xkb_file != রিক্ত);
        যদি (valid || !(flags & XKB_FILE_ITERATOR_FAIL_ON_INCLUDE_ERROR)) {
            /* Collect the strings of the statement properties */

            ধ্রুব darray_size_t path = darray_size(section->buffer);
            darray_append_string0(section->buffer, buf);

            ধ্রুব darray_size_t file = darray_size(section->buffer);
            darray_append_string0(section->buffer, stmt->file);

            ধ্রুব darray_size_t section_name =
                (stmt->map != রিক্ত || (valid && xkb_file->name))
                    ? darray_size(section->buffer)
                    : 0;
            যদি (section_name) {
                darray_append_string0(section->buffer,
                                      (stmt->map) ? stmt->map : xkb_file->name);
            }

            ধ্রুব darray_size_t modifier = (stmt->modifier)
                ? darray_size(section->buffer)
                : 0;
            যদি (modifier) {
                darray_append_string0(section->buffer, stmt->modifier);
            }

            ধ্রুব গণনা xkb_map_flags section_flags = (valid)
                ? xkb_file->flags
                : 0;

            /* Create and append the include statement */
            ধ্রুব সংরচনা xkb_file_include inc = {
                .valid = valid,
                .explicit_section = (stmt->map != রিক্ত),
                .merge = stmt->merge,
                .path = path,
                .file = file,
                .section = section_name,
                .modifier = modifier,
                .flags = section_flags
            };
            ধ্রুব darray_size_t idx = darray_size(section->includes);
            darray_append(section->includes, inc);

            /* Update include group */
            যদি (group == রিক্ত) {
                ধ্রুব darray_size_t group_idx =
                    darray_size(section->include_groups);
                darray_append(
                    section->include_groups,
                    (সংরচনা xkb_file_include_group) {.start = idx, .end = idx}
                );
                group = &darray_item(section->include_groups, group_idx);
            } অন্যথা {
                group->end = idx;
            }

            FreeXkbFile(xkb_file);
        } অন্যথা {
            ধ্রুব বর্ণ * ধ্রুব name =
                xkb_file_section_get_string(section, section->name);
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "%s include failure in: %s%s%s%s\n",
                    xkb_file_type_name(file_type), section_path,
                    (section->name ? " (section: \"": ""), name,
                    (section->name ? "\")": ""));
            FreeXkbFile(xkb_file);
            প্রত্যাগম false;
        }
    };
    প্রত্যাগম true;
}

/* Process the AST of a section */
স্থির bool
xkb_file_section_process(সংরচনা xkb_context *ctx,
                         গণনা xkb_file_iterator_flags flags,
                         ধ্রুব সংরচনা parser_keymap_config *config,
                         ধ্রুব বর্ণ *path,
                         সংরচনা xkb_file_section *section,
                         ধ্রুব XkbFile *xkb_file)
{
    bool ok = true;
    প্রতি (ParseCommon *stmt = xkb_file->defs; stmt; stmt = stmt->next) {
        যদি (stmt->type == STMT_INCLUDE) {
            ok = xkb_file_section_append_includes(ctx, flags, config, path,
                                                  section, xkb_file->file_type,
                                                  (IncludeStmt *) stmt);
            যদি (!ok)
                ভঙ্গ;
        }
    }
    প্রত্যাগম ok;
}

bool
xkb_file_section_parse(সংরচনা xkb_context *ctx,
                       গণনা xkb_file_iterator_flags iterator_flags,
                       গণনা xkb_keymap_format format,
                       গণনা xkb_keymap_compile_flags compile_flags,
                       unsigned পূর্ণাঙ্ক include_depth,
                       ধ্রুব বর্ণ *path, ধ্রুব বর্ণ *map,
                       সংরচনা xkb_file_section *section)
{
    যদি (ExceedsIncludeMaxDepth(ctx, include_depth))
        প্রত্যাগম false;

    FILE *file = fopen(path, "rb");
    যদি (!file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot open file: %s\n", path);
        প্রত্যাগম false;
    }

    ধ্রুব সংরচনা parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    XkbFile *xkb_file = XkbParseFile(ctx, &config, file, path, map);
    fclose(file);
    যদি (!xkb_file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot parse map \"%s\" in file: %s\n",
                (map ? map : "(no map)"), path);
        প্রত্যাগম false;
    }

    xkb_file_section_reset(section);
    ধ্রুব bool no_includes = iterator_flags & XKB_FILE_ITERATOR_NO_INCLUDES;
    ধ্রুব bool ok = (
        xkb_file_section_set_meta_data(ctx, section, xkb_file) &&
        (no_includes ||
         xkb_file_section_process(ctx, iterator_flags, &config, path, section,
                                  xkb_file))
    );
    FreeXkbFile(xkb_file);
    প্রত্যাগম ok;
}

সংরচনা xkb_file_iterator *
xkb_file_iterator_new_from_buffer(সংরচনা xkb_context *ctx,
                                  গণনা xkb_file_iterator_flags iterator_flags,
                                  গণনা xkb_keymap_format format,
                                  গণনা xkb_keymap_compile_flags compile_flags,
                                  ধ্রুব বর্ণ *path, ধ্রুব বর্ণ *map,
                                  গণনা xkb_file_type file_type,
                                  ধ্রুব বর্ণ *string, size_t length)
{
    সংরচনা xkb_file_iterator * ধ্রুব iter = calloc(1, পরিমাণ(*iter));
    যদি (!iter) {
        log_err(ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate file iterator\n");
        প্রত্যাগম রিক্ত;
    }

    iter->flags = iterator_flags;
    iter->ctx = ctx;
    iter->path = path;
    iter->map = map;
    iter->type = file_type;
    iter->parser_config = (সংরচনা parser_keymap_config) {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };
    xkb_file_section_init(&iter->section);

    যদি (!XkbParseStringInit(ctx, &iter->scanner, string, length, path, রিক্ত)) {
        xkb_file_iterator_free(iter);
        প্রত্যাগম রিক্ত;
    }

    প্রত্যাগম iter;
}

শূন্য
xkb_file_iterator_free(সংরচনা xkb_file_iterator *iter)
{
    যদি (!iter)
        প্রত্যাগম;

    xkb_file_section_free(&iter->section);
    FreeXkbFile(iter->pending_xkb_file);
    মুক্ত(iter);
}

bool
xkb_file_iterator_next(সংরচনা xkb_file_iterator *iter,
                       ধ্রুব সংরচনা xkb_file_section **section)
{
    যদি (iter->finished) {
        *section = রিক্ত;
        প্রত্যাগম true;
    }

next:
    { /* C11 compatibility: label cannot be followed by a declaration */ }
    /* Parse next section in the file */
    XkbFile *xkb_file = রিক্ত;

    যদি (iter->pending_xkb_file) {
        /* We are parsing the components of a keymap */
        যদি (iter->pending_section) {
            /* Parse next component */
            xkb_file = iter->pending_section;
            গচ্ছ parse_components;
        } অন্যথা {
            /* No more components, try next keymap */
            FreeXkbFile(iter->pending_xkb_file);
            iter->pending_xkb_file = রিক্ত;
        }
    }

    যদি (!XkbParseStringNext(iter->ctx, &iter->parser_config, &iter->scanner,
                            iter->map, &xkb_file)) {
        log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Error while parsing section in file: %s\n", iter->path);
        গচ্ছ error;
    } অন্যথা যদি (!xkb_file) {
        /* No more sections */
        iter->finished = true;
        *section = রিক্ত;
        প্রত্যাগম true;
    }

parse_components:
    /* Reset section */
    xkb_file_section_reset(&iter->section);

    /* Meta data */
    যদি (!xkb_file_section_set_meta_data(iter->ctx, &iter->section, xkb_file))
        গচ্ছ error;

    /* Return current section */
    *section = &iter->section;

    যদি (xkb_file->file_type == FILE_TYPE_KEYMAP) {
        /*
         * If it’s a keymap, then stop here.
         * Next iteration will process its components.
         */
        iter->pending_xkb_file = xkb_file;
        iter->pending_section = (XkbFile *) xkb_file->defs;
        iter->map = রিক্ত;
        প্রত্যাগম true;
    } অন্যথা {
        যদি (iter->type != FILE_TYPE_INVALID &&
            xkb_file->file_type != iter->type) {
            যদি (iter->pending_xkb_file) {
                /* Within a keymap: filter out component */
                iter->pending_section = (XkbFile *) xkb_file->common.next;
                গচ্ছ next;
            } অন্যথা {
                /* Component-specific file: type mismatch */
                log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File type mismatch: %s, section: %s\n",
                        iter->path,
                        (xkb_file->name ? xkb_file->name : "(no name)"));
                গচ্ছ error;
            }
        }
        যদি (iter->map) {
            iter->finished = true;
        }
    }

    /* Collect include statements of current section */
    ধ্রুব bool process_includes = !(iter->flags & XKB_FILE_ITERATOR_NO_INCLUDES);
    যদি (process_includes &&
        !xkb_file_section_process(iter->ctx, iter->flags, &iter->parser_config,
                                  iter->path, &iter->section, xkb_file)) {
        গচ্ছ error;
    } অন্যথা যদি (iter->pending_section) {
        /* Next component */
        iter->pending_section = (XkbFile *) xkb_file->common.next;
    } অন্যথা {
        /* No more component, free the file containing the components */
        FreeXkbFile(xkb_file);
    }

    প্রত্যাগম true;

error:
    FreeXkbFile(xkb_file);
    *section = রিক্ত;
    প্রত্যাগম false;
}

/* Lookup a string by its index */
ধ্রুব বর্ণ *
xkb_file_section_get_string(ধ্রুব সংরচনা xkb_file_section *section, darray_size_t idx)
{
    প্রত্যাগম &darray_item(section->buffer, idx);
}
