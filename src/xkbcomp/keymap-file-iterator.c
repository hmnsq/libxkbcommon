/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <limits.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "context.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "include.h"
#अन्तर्भाव "keymap-formats.h"
#अन्तर्भाव "keymap-file-iterator.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "util-paths.h"
#अन्तर्भाव "xkbcomp-priv.h"
#अन्तर्भाव "xkbcomp/ast.h"

ध्रुव वर्ण *
xkb_file_type_name(गणना xkb_file_type type)
{
    सचे (type > FILE_TYPE_KEYMAP)
        पटिनिवत्त "unknown";

    स्थिर ध्रुव वर्ण *xkb_file_type_strings[FILE_TYPE_अङ्कः_ENTRIES] = {
        [FILE_TYPE_KEYCODES] = "keycodes",
        [FILE_TYPE_TYPES] = "types",
        [FILE_TYPE_COMPAT] = "compatibility",
        [FILE_TYPE_SYMBOLS] = "symbols",
        [FILE_TYPE_GEOMETRY] = "geometry",
        [FILE_TYPE_KEYMAP] = "keymap",
    };
    पटिनिवत्त xkb_file_type_strings[type];
}

ध्रुव वर्ण *
xkb_merge_mode_name(गणना merge_mode merge)
{
    सचे (merge >= MERGE_MODE_अङ्कः_ENTRIES)
        पटिनिवत्त "unknown";

    स्थिर ध्रुव वर्ण *merge_mode_strings[MERGE_MODE_अङ्कः_ENTRIES] = {
        [MERGE_DEFAULT] = "default",
        [MERGE_AUGMENT] = "augment",
        [MERGE_OVERRIDE] = "override",
        [MERGE_REPLACE] = "replace",
    };
    पटिनिवत्त merge_mode_strings[merge];
}

/* Stateful lookup of map flags names */
ध्रुव वर्ण *
xkb_map_flags_string_iter(unsigned पूर्णाङ्क *index, गणना xkb_map_flags flags)
{
    सचे (!flags)
        पटिनिवत्त रिक्त;

    स्थिर ध्रुव संरचना {
        गणना xkb_map_flags flag;
        ध्रुव वर्ण *name;
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

    याव (*index < ARRAY_SIZE(names)) {
        सचे (flags & names[*index].flag)
            पटिनिवत्त names[(*index)++].name;
        (*index)++;
    }

    पटिनिवत्त रिक्त;
};

FILE *
xkb_resolve_file(संरचना xkb_context *ctx,
                 गणना xkb_file_type file_type,
                 ध्रुव वर्ण *path, ध्रुव वर्ण *map,
                 वर्ण *resolved_path, size_t resolved_path_size,
                 वर्ण *resolved_map, size_t resolved_map_size)
{
    unsigned पूर्णाङ्क offset = 0;
    FILE *file = रिक्त; /* Exact match */
    FILE *candidate = रिक्त; /* Weak match */
    ध्रुव size_t path_len = strlen(path);

    ध्रुव bool absolute_path = is_absolute_path(path);
    सचे (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        file = fopen(path, "rb");
    } अञ्ञथा {
        /* Relative path: lookup the first XKB path */
        file = FindFileInXkbPath(ctx, "(unknown)", path, path_len, file_type,
                                 resolved_path, resolved_path_size, &offset,
                                 true);
    }

    ध्रुव गणना xkb_keymap_format format = DEFAULT_निवेशः_KEYMAP_FORMAT;
    ध्रुव संरचना parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    याव (file) {
        XkbFile * ध्रुव xkb_file = XkbParseFile(ctx, &config, file, path, map);
        सचे (xkb_file) {
            सचे (file_type < FILE_TYPE_अङ्कः_ENTRIES &&
                xkb_file->file_type != file_type) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File of wrong type (expected %s, got %s); "
                        "file \"%s\" ignored\n",
                        xkb_file_type_to_string(file_type),
                        xkb_file_type_to_string(xkb_file->file_type),
                        (absolute_path ? path : resolved_path));
                गच्छ invalid_file;
            } अञ्ञथा सचे (map || (xkb_file->flags && MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops in this iteration.
                 */
                सचे (!strcpy_safe(resolved_map, resolved_map_size,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    गच्छ error;
                }
            } अञ्ञथा सचे (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = file;
                सचे (!strcpy_safe(resolved_map, resolved_map_size,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    गच्छ error;
                }
            } अञ्ञथा {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                गच्छ invalid_file;
            }
        } अञ्ञथा {
invalid_file:
            fclose(file);
            file = रिक्त;
        }

        FreeXkbFile(xkb_file);

        सचे (file != रिक्त || absolute_path) {
            /* Exact match or absolute path with no further file to search */
            भङ्ग;
        }

        offset++;
        file = FindFileInXkbPath(ctx, "(unknown)",
                                 path, path_len, file_type,
                                 resolved_path, resolved_path_size,
                                 &offset, true);
    }

    सचे (!file) {
        /* No exact match: use weak match, if any */
        file = candidate;
    } अञ्ञथा सचे (candidate) {
        /* Found exact match: discard weak match, if any */
        fclose(candidate);
    }

    सचे (absolute_path && file &&
        !strcpy_safe(resolved_path, resolved_path_size, path)) {
        गच्छ error;
    }

    पटिनिवत्त file;

error:
    log_err(ctx, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
            "Cannot copy resolved path or section\n");
    fclose(file);
    पटिनिवत्त रिक्त;
}

शून्य
xkb_file_section_init(संरचना xkb_file_section *section)
{
    darray_init(section->include_groups);
    darray_init(section->includes);
    darray_init(section->buffer);
    darray_append(section->buffer, '\0');
}

स्थिर शून्य
xkb_file_section_reset(संरचना xkb_file_section *section)
{
    darray_size(section->include_groups) = 0;
    darray_size(section->includes) = 0;
    darray_size(section->buffer) = 1; /* keep the dummy string */
}

शून्य
xkb_file_section_free(संरचना xkb_file_section *section)
{
    सचे (!section)
        पटिनिवत्त;

    darray_free(section->include_groups);
    darray_free(section->includes);
    darray_free(section->buffer);
}

स्थिर bool
xkb_file_section_set_meta_data(संरचना xkb_context *ctx,
                               संरचना xkb_file_section *section,
                               ध्रुव XkbFile *xkb_file)
{
    section->file_type = xkb_file->file_type;
    section->flags = xkb_file->flags;
    सचे (xkb_file->name) {
        darray_size_t idx = darray_size(section->buffer);
        darray_append_string0(section->buffer, xkb_file->name);
        section->name = idx;
    } अञ्ञथा {
        section->name = 0;
    }
    पटिनिवत्त true;
}

/** Process a list of include statements */
स्थिर bool
xkb_file_section_append_includes(संरचना xkb_context *ctx,
                                 गणना xkb_file_iterator_flags flags,
                                 ध्रुव संरचना parser_keymap_config *config,
                                 ध्रुव वर्ण *section_path,
                                 संरचना xkb_file_section *section,
                                 गणना xkb_file_type file_type,
                                 IncludeStmt * include)
{
    संरचना xkb_file_include_group *group = रिक्त;
    /* Note: this will flatten statements such as `include "pc+de"` */
    अत्थाय (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        वर्ण buf[PATH_MAX];
        /* Parse the included file to check the include validity */
        XkbFile *xkb_file = ProcessIncludeFile(ctx, config, stmt, file_type,
                                               buf, परिमाण(buf));
        ध्रुव bool valid = (xkb_file != रिक्त);
        सचे (valid || !(flags & XKB_FILE_ITERATOR_FAIL_ON_INCLUDE_ERROR)) {
            /* Collect the strings of the statement properties */

            ध्रुव darray_size_t path = darray_size(section->buffer);
            darray_append_string0(section->buffer, buf);

            ध्रुव darray_size_t file = darray_size(section->buffer);
            darray_append_string0(section->buffer, stmt->file);

            ध्रुव darray_size_t section_name =
                (stmt->map != रिक्त || (valid && xkb_file->name))
                    ? darray_size(section->buffer)
                    : 0;
            सचे (section_name) {
                darray_append_string0(section->buffer,
                                      (stmt->map) ? stmt->map : xkb_file->name);
            }

            ध्रुव darray_size_t modifier = (stmt->modifier)
                ? darray_size(section->buffer)
                : 0;
            सचे (modifier) {
                darray_append_string0(section->buffer, stmt->modifier);
            }

            ध्रुव गणना xkb_map_flags section_flags = (valid)
                ? xkb_file->flags
                : 0;

            /* Create and append the include statement */
            ध्रुव संरचना xkb_file_include inc = {
                .valid = valid,
                .explicit_section = (stmt->map != रिक्त),
                .merge = stmt->merge,
                .path = path,
                .file = file,
                .section = section_name,
                .modifier = modifier,
                .flags = section_flags
            };
            ध्रुव darray_size_t idx = darray_size(section->includes);
            darray_append(section->includes, inc);

            /* Update include group */
            सचे (group == रिक्त) {
                ध्रुव darray_size_t group_idx =
                    darray_size(section->include_groups);
                darray_append(
                    section->include_groups,
                    (संरचना xkb_file_include_group) {.start = idx, .end = idx}
                );
                group = &darray_item(section->include_groups, group_idx);
            } अञ्ञथा {
                group->end = idx;
            }

            FreeXkbFile(xkb_file);
        } अञ्ञथा {
            ध्रुव वर्ण * ध्रुव name =
                xkb_file_section_get_string(section, section->name);
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "%s include failure in: %s%s%s%s\n",
                    xkb_file_type_name(file_type), section_path,
                    (section->name ? " (section: \"": ""), name,
                    (section->name ? "\")": ""));
            FreeXkbFile(xkb_file);
            पटिनिवत्त false;
        }
    };
    पटिनिवत्त true;
}

/* Process the AST of a section */
स्थिर bool
xkb_file_section_process(संरचना xkb_context *ctx,
                         गणना xkb_file_iterator_flags flags,
                         ध्रुव संरचना parser_keymap_config *config,
                         ध्रुव वर्ण *path,
                         संरचना xkb_file_section *section,
                         ध्रुव XkbFile *xkb_file)
{
    bool ok = true;
    अत्थाय (ParseCommon *stmt = xkb_file->defs; stmt; stmt = stmt->next) {
        सचे (stmt->type == STMT_INCLUDE) {
            ok = xkb_file_section_append_includes(ctx, flags, config, path,
                                                  section, xkb_file->file_type,
                                                  (IncludeStmt *) stmt);
            सचे (!ok)
                भङ्ग;
        }
    }
    पटिनिवत्त ok;
}

bool
xkb_file_section_parse(संरचना xkb_context *ctx,
                       गणना xkb_file_iterator_flags iterator_flags,
                       गणना xkb_keymap_format format,
                       गणना xkb_keymap_compile_flags compile_flags,
                       unsigned पूर्णाङ्क include_depth,
                       ध्रुव वर्ण *path, ध्रुव वर्ण *map,
                       संरचना xkb_file_section *section)
{
    सचे (ExceedsIncludeMaxDepth(ctx, include_depth))
        पटिनिवत्त false;

    FILE *file = fopen(path, "rb");
    सचे (!file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot open file: %s\n", path);
        पटिनिवत्त false;
    }

    ध्रुव संरचना parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    XkbFile *xkb_file = XkbParseFile(ctx, &config, file, path, map);
    fclose(file);
    सचे (!xkb_file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot parse map \"%s\" in file: %s\n",
                (map ? map : "(no map)"), path);
        पटिनिवत्त false;
    }

    xkb_file_section_reset(section);
    ध्रुव bool no_includes = iterator_flags & XKB_FILE_ITERATOR_NO_INCLUDES;
    ध्रुव bool ok = (
        xkb_file_section_set_meta_data(ctx, section, xkb_file) &&
        (no_includes ||
         xkb_file_section_process(ctx, iterator_flags, &config, path, section,
                                  xkb_file))
    );
    FreeXkbFile(xkb_file);
    पटिनिवत्त ok;
}

संरचना xkb_file_iterator *
xkb_file_iterator_new_from_buffer(संरचना xkb_context *ctx,
                                  गणना xkb_file_iterator_flags iterator_flags,
                                  गणना xkb_keymap_format format,
                                  गणना xkb_keymap_compile_flags compile_flags,
                                  ध्रुव वर्ण *path, ध्रुव वर्ण *map,
                                  गणना xkb_file_type file_type,
                                  ध्रुव वर्ण *string, size_t length)
{
    संरचना xkb_file_iterator * ध्रुव iter = calloc(1, परिमाण(*iter));
    सचे (!iter) {
        log_err(ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate file iterator\n");
        पटिनिवत्त रिक्त;
    }

    iter->flags = iterator_flags;
    iter->ctx = ctx;
    iter->path = path;
    iter->map = map;
    iter->type = file_type;
    iter->parser_config = (संरचना parser_keymap_config) {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };
    xkb_file_section_init(&iter->section);

    सचे (!XkbParseStringInit(ctx, &iter->scanner, string, length, path, रिक्त)) {
        xkb_file_iterator_free(iter);
        पटिनिवत्त रिक्त;
    }

    पटिनिवत्त iter;
}

शून्य
xkb_file_iterator_free(संरचना xkb_file_iterator *iter)
{
    सचे (!iter)
        पटिनिवत्त;

    xkb_file_section_free(&iter->section);
    FreeXkbFile(iter->pending_xkb_file);
    मुक्त(iter);
}

bool
xkb_file_iterator_next(संरचना xkb_file_iterator *iter,
                       ध्रुव संरचना xkb_file_section **section)
{
    सचे (iter->finished) {
        *section = रिक्त;
        पटिनिवत्त true;
    }

next:
    { /* C11 compatibility: label cannot be followed by a declaration */ }
    /* Parse next section in the file */
    XkbFile *xkb_file = रिक्त;

    सचे (iter->pending_xkb_file) {
        /* We are parsing the components of a keymap */
        सचे (iter->pending_section) {
            /* Parse next component */
            xkb_file = iter->pending_section;
            गच्छ parse_components;
        } अञ्ञथा {
            /* No more components, try next keymap */
            FreeXkbFile(iter->pending_xkb_file);
            iter->pending_xkb_file = रिक्त;
        }
    }

    सचे (!XkbParseStringNext(iter->ctx, &iter->parser_config, &iter->scanner,
                            iter->map, &xkb_file)) {
        log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Error while parsing section in file: %s\n", iter->path);
        गच्छ error;
    } अञ्ञथा सचे (!xkb_file) {
        /* No more sections */
        iter->finished = true;
        *section = रिक्त;
        पटिनिवत्त true;
    }

parse_components:
    /* Reset section */
    xkb_file_section_reset(&iter->section);

    /* Meta data */
    सचे (!xkb_file_section_set_meta_data(iter->ctx, &iter->section, xkb_file))
        गच्छ error;

    /* Return current section */
    *section = &iter->section;

    सचे (xkb_file->file_type == FILE_TYPE_KEYMAP) {
        /*
         * If it’s a keymap, then stop here.
         * Next iteration will process its components.
         */
        iter->pending_xkb_file = xkb_file;
        iter->pending_section = (XkbFile *) xkb_file->defs;
        iter->map = रिक्त;
        पटिनिवत्त true;
    } अञ्ञथा {
        सचे (iter->type != FILE_TYPE_INVALID &&
            xkb_file->file_type != iter->type) {
            सचे (iter->pending_xkb_file) {
                /* Within a keymap: filter out component */
                iter->pending_section = (XkbFile *) xkb_file->common.next;
                गच्छ next;
            } अञ्ञथा {
                /* Component-specific file: type mismatch */
                log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File type mismatch: %s, section: %s\n",
                        iter->path,
                        (xkb_file->name ? xkb_file->name : "(no name)"));
                गच्छ error;
            }
        }
        सचे (iter->map) {
            iter->finished = true;
        }
    }

    /* Collect include statements of current section */
    ध्रुव bool process_includes = !(iter->flags & XKB_FILE_ITERATOR_NO_INCLUDES);
    सचे (process_includes &&
        !xkb_file_section_process(iter->ctx, iter->flags, &iter->parser_config,
                                  iter->path, &iter->section, xkb_file)) {
        गच्छ error;
    } अञ्ञथा सचे (iter->pending_section) {
        /* Next component */
        iter->pending_section = (XkbFile *) xkb_file->common.next;
    } अञ्ञथा {
        /* No more component, free the file containing the components */
        FreeXkbFile(xkb_file);
    }

    पटिनिवत्त true;

error:
    FreeXkbFile(xkb_file);
    *section = रिक्त;
    पटिनिवत्त false;
}

/* Lookup a string by its index */
ध्रुव वर्ण *
xkb_file_section_get_string(ध्रुव संरचना xkb_file_section *section, darray_size_t idx)
{
    पटिनिवत्त &darray_item(section->buffer, idx);
}
