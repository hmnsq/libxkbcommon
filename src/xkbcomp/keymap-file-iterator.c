/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <limits.h>
#அந்தர்பாவ <string.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "context.h"
#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "include.h"
#அந்தர்பாவ "keymap-formats.h"
#அந்தர்பாவ "keymap-file-iterator.h"
#அந்தர்பாவ "messages-codes.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "util-paths.h"
#அந்தர்பாவ "xkbcomp-priv.h"
#அந்தர்பாவ "xkbcomp/ast.h"

மாறிலி எழுத்து *
xkb_file_type_name(எண்ணிக்கை xkb_file_type type)
{
    எனில் (type > FILE_TYPE_KEYMAP)
        திரும்பு "unknown";

    நிலையான மாறிலி எழுத்து *xkb_file_type_strings[FILE_TYPE_இலக்கம்_ENTRIES] = {
        [FILE_TYPE_KEYCODES] = "keycodes",
        [FILE_TYPE_TYPES] = "types",
        [FILE_TYPE_COMPAT] = "compatibility",
        [FILE_TYPE_SYMBOLS] = "symbols",
        [FILE_TYPE_GEOMETRY] = "geometry",
        [FILE_TYPE_KEYMAP] = "keymap",
    };
    திரும்பு xkb_file_type_strings[type];
}

மாறிலி எழுத்து *
xkb_merge_mode_name(எண்ணிக்கை merge_mode merge)
{
    எனில் (merge >= MERGE_MODE_இலக்கம்_ENTRIES)
        திரும்பு "unknown";

    நிலையான மாறிலி எழுத்து *merge_mode_strings[MERGE_MODE_இலக்கம்_ENTRIES] = {
        [MERGE_DEFAULT] = "default",
        [MERGE_AUGMENT] = "augment",
        [MERGE_OVERRIDE] = "override",
        [MERGE_REPLACE] = "replace",
    };
    திரும்பு merge_mode_strings[merge];
}

/* Stateful lookup of map flags names */
மாறிலி எழுத்து *
xkb_map_flags_string_iter(unsigned முழு *index, எண்ணிக்கை xkb_map_flags flags)
{
    எனில் (!flags)
        திரும்பு வெற்று;

    நிலையான மாறிலி கட்டமைப்பு {
        எண்ணிக்கை xkb_map_flags flag;
        மாறிலி எழுத்து *name;
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

    போது (*index < ARRAY_SIZE(names)) {
        எனில் (flags & names[*index].flag)
            திரும்பு names[(*index)++].name;
        (*index)++;
    }

    திரும்பு வெற்று;
};

FILE *
xkb_resolve_file(கட்டமைப்பு xkb_context *ctx,
                 எண்ணிக்கை xkb_file_type file_type,
                 மாறிலி எழுத்து *path, மாறிலி எழுத்து *map,
                 எழுத்து *resolved_path, size_t resolved_path_size,
                 எழுத்து *resolved_map, size_t resolved_map_size)
{
    unsigned முழு offset = 0;
    FILE *file = வெற்று; /* Exact match */
    FILE *candidate = வெற்று; /* Weak match */
    மாறிலி size_t path_len = strlen(path);

    மாறிலி bool absolute_path = is_absolute_path(path);
    எனில் (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        file = fopen(path, "rb");
    } இல்லையெனில் {
        /* Relative path: lookup the first XKB path */
        file = FindFileInXkbPath(ctx, "(unknown)", path, path_len, file_type,
                                 resolved_path, resolved_path_size, &offset,
                                 true);
    }

    மாறிலி எண்ணிக்கை xkb_keymap_format format = DEFAULT_உள்ளீடு_KEYMAP_FORMAT;
    மாறிலி கட்டமைப்பு parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    போது (file) {
        XkbFile * மாறிலி xkb_file = XkbParseFile(ctx, &config, file, path, map);
        எனில் (xkb_file) {
            எனில் (file_type < FILE_TYPE_இலக்கம்_ENTRIES &&
                xkb_file->file_type != file_type) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File of wrong type (expected %s, got %s); "
                        "file \"%s\" ignored\n",
                        xkb_file_type_to_string(file_type),
                        xkb_file_type_to_string(xkb_file->file_type),
                        (absolute_path ? path : resolved_path));
                செல் invalid_file;
            } இல்லையெனில் எனில் (map || (xkb_file->flags && MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops in this iteration.
                 */
                எனில் (!strcpy_safe(resolved_map, resolved_map_size,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    செல் error;
                }
            } இல்லையெனில் எனில் (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = file;
                எனில் (!strcpy_safe(resolved_map, resolved_map_size,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    செல் error;
                }
            } இல்லையெனில் {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                செல் invalid_file;
            }
        } இல்லையெனில் {
invalid_file:
            fclose(file);
            file = வெற்று;
        }

        FreeXkbFile(xkb_file);

        எனில் (file != வெற்று || absolute_path) {
            /* Exact match or absolute path with no further file to search */
            நிறுத்து;
        }

        offset++;
        file = FindFileInXkbPath(ctx, "(unknown)",
                                 path, path_len, file_type,
                                 resolved_path, resolved_path_size,
                                 &offset, true);
    }

    எனில் (!file) {
        /* No exact match: use weak match, if any */
        file = candidate;
    } இல்லையெனில் எனில் (candidate) {
        /* Found exact match: discard weak match, if any */
        fclose(candidate);
    }

    எனில் (absolute_path && file &&
        !strcpy_safe(resolved_path, resolved_path_size, path)) {
        செல் error;
    }

    திரும்பு file;

error:
    log_err(ctx, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
            "Cannot copy resolved path or section\n");
    fclose(file);
    திரும்பு வெற்று;
}

வெறுமை
xkb_file_section_init(கட்டமைப்பு xkb_file_section *section)
{
    darray_init(section->include_groups);
    darray_init(section->includes);
    darray_init(section->buffer);
    darray_append(section->buffer, '\0');
}

நிலையான வெறுமை
xkb_file_section_reset(கட்டமைப்பு xkb_file_section *section)
{
    darray_size(section->include_groups) = 0;
    darray_size(section->includes) = 0;
    darray_size(section->buffer) = 1; /* keep the dummy string */
}

வெறுமை
xkb_file_section_free(கட்டமைப்பு xkb_file_section *section)
{
    எனில் (!section)
        திரும்பு;

    darray_free(section->include_groups);
    darray_free(section->includes);
    darray_free(section->buffer);
}

நிலையான bool
xkb_file_section_set_meta_data(கட்டமைப்பு xkb_context *ctx,
                               கட்டமைப்பு xkb_file_section *section,
                               மாறிலி XkbFile *xkb_file)
{
    section->file_type = xkb_file->file_type;
    section->flags = xkb_file->flags;
    எனில் (xkb_file->name) {
        darray_size_t idx = darray_size(section->buffer);
        darray_append_string0(section->buffer, xkb_file->name);
        section->name = idx;
    } இல்லையெனில் {
        section->name = 0;
    }
    திரும்பு true;
}

/** Process a list of include statements */
நிலையான bool
xkb_file_section_append_includes(கட்டமைப்பு xkb_context *ctx,
                                 எண்ணிக்கை xkb_file_iterator_flags flags,
                                 மாறிலி கட்டமைப்பு parser_keymap_config *config,
                                 மாறிலி எழுத்து *section_path,
                                 கட்டமைப்பு xkb_file_section *section,
                                 எண்ணிக்கை xkb_file_type file_type,
                                 IncludeStmt * include)
{
    கட்டமைப்பு xkb_file_include_group *group = வெற்று;
    /* Note: this will flatten statements such as `include "pc+de"` */
    க்கு (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        எழுத்து buf[PATH_MAX];
        /* Parse the included file to check the include validity */
        XkbFile *xkb_file = ProcessIncludeFile(ctx, config, stmt, file_type,
                                               buf, அளவு(buf));
        மாறிலி bool valid = (xkb_file != வெற்று);
        எனில் (valid || !(flags & XKB_FILE_ITERATOR_FAIL_ON_INCLUDE_ERROR)) {
            /* Collect the strings of the statement properties */

            மாறிலி darray_size_t path = darray_size(section->buffer);
            darray_append_string0(section->buffer, buf);

            மாறிலி darray_size_t file = darray_size(section->buffer);
            darray_append_string0(section->buffer, stmt->file);

            மாறிலி darray_size_t section_name =
                (stmt->map != வெற்று || (valid && xkb_file->name))
                    ? darray_size(section->buffer)
                    : 0;
            எனில் (section_name) {
                darray_append_string0(section->buffer,
                                      (stmt->map) ? stmt->map : xkb_file->name);
            }

            மாறிலி darray_size_t modifier = (stmt->modifier)
                ? darray_size(section->buffer)
                : 0;
            எனில் (modifier) {
                darray_append_string0(section->buffer, stmt->modifier);
            }

            மாறிலி எண்ணிக்கை xkb_map_flags section_flags = (valid)
                ? xkb_file->flags
                : 0;

            /* Create and append the include statement */
            மாறிலி கட்டமைப்பு xkb_file_include inc = {
                .valid = valid,
                .explicit_section = (stmt->map != வெற்று),
                .merge = stmt->merge,
                .path = path,
                .file = file,
                .section = section_name,
                .modifier = modifier,
                .flags = section_flags
            };
            மாறிலி darray_size_t idx = darray_size(section->includes);
            darray_append(section->includes, inc);

            /* Update include group */
            எனில் (group == வெற்று) {
                மாறிலி darray_size_t group_idx =
                    darray_size(section->include_groups);
                darray_append(
                    section->include_groups,
                    (கட்டமைப்பு xkb_file_include_group) {.start = idx, .end = idx}
                );
                group = &darray_item(section->include_groups, group_idx);
            } இல்லையெனில் {
                group->end = idx;
            }

            FreeXkbFile(xkb_file);
        } இல்லையெனில் {
            மாறிலி எழுத்து * மாறிலி name =
                xkb_file_section_get_string(section, section->name);
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "%s include failure in: %s%s%s%s\n",
                    xkb_file_type_name(file_type), section_path,
                    (section->name ? " (section: \"": ""), name,
                    (section->name ? "\")": ""));
            FreeXkbFile(xkb_file);
            திரும்பு false;
        }
    };
    திரும்பு true;
}

/* Process the AST of a section */
நிலையான bool
xkb_file_section_process(கட்டமைப்பு xkb_context *ctx,
                         எண்ணிக்கை xkb_file_iterator_flags flags,
                         மாறிலி கட்டமைப்பு parser_keymap_config *config,
                         மாறிலி எழுத்து *path,
                         கட்டமைப்பு xkb_file_section *section,
                         மாறிலி XkbFile *xkb_file)
{
    bool ok = true;
    க்கு (ParseCommon *stmt = xkb_file->defs; stmt; stmt = stmt->next) {
        எனில் (stmt->type == STMT_INCLUDE) {
            ok = xkb_file_section_append_includes(ctx, flags, config, path,
                                                  section, xkb_file->file_type,
                                                  (IncludeStmt *) stmt);
            எனில் (!ok)
                நிறுத்து;
        }
    }
    திரும்பு ok;
}

bool
xkb_file_section_parse(கட்டமைப்பு xkb_context *ctx,
                       எண்ணிக்கை xkb_file_iterator_flags iterator_flags,
                       எண்ணிக்கை xkb_keymap_format format,
                       எண்ணிக்கை xkb_keymap_compile_flags compile_flags,
                       unsigned முழு include_depth,
                       மாறிலி எழுத்து *path, மாறிலி எழுத்து *map,
                       கட்டமைப்பு xkb_file_section *section)
{
    எனில் (ExceedsIncludeMaxDepth(ctx, include_depth))
        திரும்பு false;

    FILE *file = fopen(path, "rb");
    எனில் (!file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot open file: %s\n", path);
        திரும்பு false;
    }

    மாறிலி கட்டமைப்பு parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    XkbFile *xkb_file = XkbParseFile(ctx, &config, file, path, map);
    fclose(file);
    எனில் (!xkb_file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot parse map \"%s\" in file: %s\n",
                (map ? map : "(no map)"), path);
        திரும்பு false;
    }

    xkb_file_section_reset(section);
    மாறிலி bool no_includes = iterator_flags & XKB_FILE_ITERATOR_NO_INCLUDES;
    மாறிலி bool ok = (
        xkb_file_section_set_meta_data(ctx, section, xkb_file) &&
        (no_includes ||
         xkb_file_section_process(ctx, iterator_flags, &config, path, section,
                                  xkb_file))
    );
    FreeXkbFile(xkb_file);
    திரும்பு ok;
}

கட்டமைப்பு xkb_file_iterator *
xkb_file_iterator_new_from_buffer(கட்டமைப்பு xkb_context *ctx,
                                  எண்ணிக்கை xkb_file_iterator_flags iterator_flags,
                                  எண்ணிக்கை xkb_keymap_format format,
                                  எண்ணிக்கை xkb_keymap_compile_flags compile_flags,
                                  மாறிலி எழுத்து *path, மாறிலி எழுத்து *map,
                                  எண்ணிக்கை xkb_file_type file_type,
                                  மாறிலி எழுத்து *string, size_t length)
{
    கட்டமைப்பு xkb_file_iterator * மாறிலி iter = calloc(1, அளவு(*iter));
    எனில் (!iter) {
        log_err(ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate file iterator\n");
        திரும்பு வெற்று;
    }

    iter->flags = iterator_flags;
    iter->ctx = ctx;
    iter->path = path;
    iter->map = map;
    iter->type = file_type;
    iter->parser_config = (கட்டமைப்பு parser_keymap_config) {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };
    xkb_file_section_init(&iter->section);

    எனில் (!XkbParseStringInit(ctx, &iter->scanner, string, length, path, வெற்று)) {
        xkb_file_iterator_free(iter);
        திரும்பு வெற்று;
    }

    திரும்பு iter;
}

வெறுமை
xkb_file_iterator_free(கட்டமைப்பு xkb_file_iterator *iter)
{
    எனில் (!iter)
        திரும்பு;

    xkb_file_section_free(&iter->section);
    FreeXkbFile(iter->pending_xkb_file);
    விடுவி(iter);
}

bool
xkb_file_iterator_next(கட்டமைப்பு xkb_file_iterator *iter,
                       மாறிலி கட்டமைப்பு xkb_file_section **section)
{
    எனில் (iter->finished) {
        *section = வெற்று;
        திரும்பு true;
    }

next:
    { /* C11 compatibility: label cannot be followed by a declaration */ }
    /* Parse next section in the file */
    XkbFile *xkb_file = வெற்று;

    எனில் (iter->pending_xkb_file) {
        /* We are parsing the components of a keymap */
        எனில் (iter->pending_section) {
            /* Parse next component */
            xkb_file = iter->pending_section;
            செல் parse_components;
        } இல்லையெனில் {
            /* No more components, try next keymap */
            FreeXkbFile(iter->pending_xkb_file);
            iter->pending_xkb_file = வெற்று;
        }
    }

    எனில் (!XkbParseStringNext(iter->ctx, &iter->parser_config, &iter->scanner,
                            iter->map, &xkb_file)) {
        log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Error while parsing section in file: %s\n", iter->path);
        செல் error;
    } இல்லையெனில் எனில் (!xkb_file) {
        /* No more sections */
        iter->finished = true;
        *section = வெற்று;
        திரும்பு true;
    }

parse_components:
    /* Reset section */
    xkb_file_section_reset(&iter->section);

    /* Meta data */
    எனில் (!xkb_file_section_set_meta_data(iter->ctx, &iter->section, xkb_file))
        செல் error;

    /* Return current section */
    *section = &iter->section;

    எனில் (xkb_file->file_type == FILE_TYPE_KEYMAP) {
        /*
         * If it’s a keymap, then stop here.
         * Next iteration will process its components.
         */
        iter->pending_xkb_file = xkb_file;
        iter->pending_section = (XkbFile *) xkb_file->defs;
        iter->map = வெற்று;
        திரும்பு true;
    } இல்லையெனில் {
        எனில் (iter->type != FILE_TYPE_INVALID &&
            xkb_file->file_type != iter->type) {
            எனில் (iter->pending_xkb_file) {
                /* Within a keymap: filter out component */
                iter->pending_section = (XkbFile *) xkb_file->common.next;
                செல் next;
            } இல்லையெனில் {
                /* Component-specific file: type mismatch */
                log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File type mismatch: %s, section: %s\n",
                        iter->path,
                        (xkb_file->name ? xkb_file->name : "(no name)"));
                செல் error;
            }
        }
        எனில் (iter->map) {
            iter->finished = true;
        }
    }

    /* Collect include statements of current section */
    மாறிலி bool process_includes = !(iter->flags & XKB_FILE_ITERATOR_NO_INCLUDES);
    எனில் (process_includes &&
        !xkb_file_section_process(iter->ctx, iter->flags, &iter->parser_config,
                                  iter->path, &iter->section, xkb_file)) {
        செல் error;
    } இல்லையெனில் எனில் (iter->pending_section) {
        /* Next component */
        iter->pending_section = (XkbFile *) xkb_file->common.next;
    } இல்லையெனில் {
        /* No more component, free the file containing the components */
        FreeXkbFile(xkb_file);
    }

    திரும்பு true;

error:
    FreeXkbFile(xkb_file);
    *section = வெற்று;
    திரும்பு false;
}

/* Lookup a string by its index */
மாறிலி எழுத்து *
xkb_file_section_get_string(மாறிலி கட்டமைப்பு xkb_file_section *section, darray_size_t idx)
{
    திரும்பு &darray_item(section->buffer, idx);
}
