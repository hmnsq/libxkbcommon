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

स्थिरांक अक्षर *
xkb_file_type_name(गणना xkb_file_type type)
{
    जर (type > FILE_TYPE_KEYMAP)
        परत "unknown";

    स्थिर स्थिरांक अक्षर *xkb_file_type_strings[FILE_TYPE_अंक_ENTRIES] = {
        [FILE_TYPE_KEYCODES] = "keycodes",
        [FILE_TYPE_TYPES] = "types",
        [FILE_TYPE_COMPAT] = "compatibility",
        [FILE_TYPE_SYMBOLS] = "symbols",
        [FILE_TYPE_GEOMETRY] = "geometry",
        [FILE_TYPE_KEYMAP] = "keymap",
    };
    परत xkb_file_type_strings[type];
}

स्थिरांक अक्षर *
xkb_merge_mode_name(गणना merge_mode merge)
{
    जर (merge >= MERGE_MODE_अंक_ENTRIES)
        परत "unknown";

    स्थिर स्थिरांक अक्षर *merge_mode_strings[MERGE_MODE_अंक_ENTRIES] = {
        [MERGE_DEFAULT] = "default",
        [MERGE_AUGMENT] = "augment",
        [MERGE_OVERRIDE] = "override",
        [MERGE_REPLACE] = "replace",
    };
    परत merge_mode_strings[merge];
}

/* Stateful lookup of map flags names */
स्थिरांक अक्षर *
xkb_map_flags_string_iter(अचिन्हित पूर्णांक *index, गणना xkb_map_flags flags)
{
    जर (!flags)
        परत रिक्त;

    स्थिर स्थिरांक संरचना {
        गणना xkb_map_flags flag;
        स्थिरांक अक्षर *name;
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

    पर्यंत (*index < ARRAY_SIZE(names)) {
        जर (flags & names[*index].flag)
            परत names[(*index)++].name;
        (*index)++;
    }

    परत रिक्त;
};

खाता *
xkb_resolve_file(संरचना xkb_context *ctx,
                 गणना xkb_file_type file_type,
                 स्थिरांक अक्षर *path, स्थिरांक अक्षर *map,
                 अक्षर *resolved_path, माप_प्रकार resolved_path_size,
                 अक्षर *resolved_map, माप_प्रकार resolved_map_size)
{
    अचिन्हित पूर्णांक offset = 0;
    खाता *file = रिक्त; /* Exact match */
    खाता *candidate = रिक्त; /* Weak match */
    स्थिरांक माप_प्रकार path_len = म_माप(path);

    स्थिरांक bool absolute_path = is_absolute_path(path);
    जर (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        file = ख_खोलो(path, "rb");
    } अन्यथा {
        /* Relative path: lookup the first XKB path */
        file = FindFileInXkbPath(ctx, "(unknown)", path, path_len, file_type,
                                 resolved_path, resolved_path_size, &offset,
                                 true);
    }

    स्थिरांक गणना xkb_keymap_format format = DEFAULT_आदान_KEYMAP_FORMAT;
    स्थिरांक संरचना parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    पर्यंत (file) {
        XkbFile * स्थिरांक xkb_file = XkbParseFile(ctx, &config, file, path, map);
        जर (xkb_file) {
            जर (file_type < FILE_TYPE_अंक_ENTRIES &&
                xkb_file->file_type != file_type) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File of wrong type (expected %s, got %s); "
                        "file \"%s\" ignored\n",
                        xkb_file_type_to_string(file_type),
                        xkb_file_type_to_string(xkb_file->file_type),
                        (absolute_path ? path : resolved_path));
                जा invalid_file;
            } अन्यथा जर (map || (xkb_file->flags && MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops in this iteration.
                 */
                जर (!strcpy_safe(resolved_map, resolved_map_size,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    जा error;
                }
            } अन्यथा जर (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = file;
                जर (!strcpy_safe(resolved_map, resolved_map_size,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    जा error;
                }
            } अन्यथा {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                जा invalid_file;
            }
        } अन्यथा {
invalid_file:
            ख_बंद(file);
            file = रिक्त;
        }

        FreeXkbFile(xkb_file);

        जर (file != रिक्त || absolute_path) {
            /* Exact match or absolute path with no further file to search */
            थांबा;
        }

        offset++;
        file = FindFileInXkbPath(ctx, "(unknown)",
                                 path, path_len, file_type,
                                 resolved_path, resolved_path_size,
                                 &offset, true);
    }

    जर (!file) {
        /* No exact match: use weak match, if any */
        file = candidate;
    } अन्यथा जर (candidate) {
        /* Found exact match: discard weak match, if any */
        ख_बंद(candidate);
    }

    जर (absolute_path && file &&
        !strcpy_safe(resolved_path, resolved_path_size, path)) {
        जा error;
    }

    परत file;

error:
    log_err(ctx, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
            "Cannot copy resolved path or section\n");
    ख_बंद(file);
    परत रिक्त;
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
    जर (!section)
        परत;

    darray_free(section->include_groups);
    darray_free(section->includes);
    darray_free(section->buffer);
}

स्थिर bool
xkb_file_section_set_meta_data(संरचना xkb_context *ctx,
                               संरचना xkb_file_section *section,
                               स्थिरांक XkbFile *xkb_file)
{
    section->file_type = xkb_file->file_type;
    section->flags = xkb_file->flags;
    जर (xkb_file->name) {
        darray_size_t idx = darray_size(section->buffer);
        darray_append_string0(section->buffer, xkb_file->name);
        section->name = idx;
    } अन्यथा {
        section->name = 0;
    }
    परत true;
}

/** Process a list of include statements */
स्थिर bool
xkb_file_section_append_includes(संरचना xkb_context *ctx,
                                 गणना xkb_file_iterator_flags flags,
                                 स्थिरांक संरचना parser_keymap_config *config,
                                 स्थिरांक अक्षर *section_path,
                                 संरचना xkb_file_section *section,
                                 गणना xkb_file_type file_type,
                                 IncludeStmt * include)
{
    संरचना xkb_file_include_group *group = रिक्त;
    /* Note: this will flatten statements such as `include "pc+de"` */
    साठी (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        अक्षर buf[PATH_MAX];
        /* Parse the included file to check the include validity */
        XkbFile *xkb_file = ProcessIncludeFile(ctx, config, stmt, file_type,
                                               buf, आकार(buf));
        स्थिरांक bool valid = (xkb_file != रिक्त);
        जर (valid || !(flags & XKB_FILE_ITERATOR_FAIL_ON_INCLUDE_ERROR)) {
            /* Collect the strings of the statement properties */

            स्थिरांक darray_size_t path = darray_size(section->buffer);
            darray_append_string0(section->buffer, buf);

            स्थिरांक darray_size_t file = darray_size(section->buffer);
            darray_append_string0(section->buffer, stmt->file);

            स्थिरांक darray_size_t section_name =
                (stmt->map != रिक्त || (valid && xkb_file->name))
                    ? darray_size(section->buffer)
                    : 0;
            जर (section_name) {
                darray_append_string0(section->buffer,
                                      (stmt->map) ? stmt->map : xkb_file->name);
            }

            स्थिरांक darray_size_t modifier = (stmt->modifier)
                ? darray_size(section->buffer)
                : 0;
            जर (modifier) {
                darray_append_string0(section->buffer, stmt->modifier);
            }

            स्थिरांक गणना xkb_map_flags section_flags = (valid)
                ? xkb_file->flags
                : 0;

            /* Create and append the include statement */
            स्थिरांक संरचना xkb_file_include inc = {
                .valid = valid,
                .explicit_section = (stmt->map != रिक्त),
                .merge = stmt->merge,
                .path = path,
                .file = file,
                .section = section_name,
                .modifier = modifier,
                .flags = section_flags
            };
            स्थिरांक darray_size_t idx = darray_size(section->includes);
            darray_append(section->includes, inc);

            /* Update include group */
            जर (group == रिक्त) {
                स्थिरांक darray_size_t group_idx =
                    darray_size(section->include_groups);
                darray_append(
                    section->include_groups,
                    (संरचना xkb_file_include_group) {.start = idx, .end = idx}
                );
                group = &darray_item(section->include_groups, group_idx);
            } अन्यथा {
                group->end = idx;
            }

            FreeXkbFile(xkb_file);
        } अन्यथा {
            स्थिरांक अक्षर * स्थिरांक name =
                xkb_file_section_get_string(section, section->name);
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "%s include failure in: %s%s%s%s\n",
                    xkb_file_type_name(file_type), section_path,
                    (section->name ? " (section: \"": ""), name,
                    (section->name ? "\")": ""));
            FreeXkbFile(xkb_file);
            परत false;
        }
    };
    परत true;
}

/* Process the AST of a section */
स्थिर bool
xkb_file_section_process(संरचना xkb_context *ctx,
                         गणना xkb_file_iterator_flags flags,
                         स्थिरांक संरचना parser_keymap_config *config,
                         स्थिरांक अक्षर *path,
                         संरचना xkb_file_section *section,
                         स्थिरांक XkbFile *xkb_file)
{
    bool ok = true;
    साठी (ParseCommon *stmt = xkb_file->defs; stmt; stmt = stmt->next) {
        जर (stmt->type == STMT_INCLUDE) {
            ok = xkb_file_section_append_includes(ctx, flags, config, path,
                                                  section, xkb_file->file_type,
                                                  (IncludeStmt *) stmt);
            जर (!ok)
                थांबा;
        }
    }
    परत ok;
}

bool
xkb_file_section_parse(संरचना xkb_context *ctx,
                       गणना xkb_file_iterator_flags iterator_flags,
                       गणना xkb_keymap_format format,
                       गणना xkb_keymap_compile_flags compile_flags,
                       अचिन्हित पूर्णांक include_depth,
                       स्थिरांक अक्षर *path, स्थिरांक अक्षर *map,
                       संरचना xkb_file_section *section)
{
    जर (ExceedsIncludeMaxDepth(ctx, include_depth))
        परत false;

    खाता *file = ख_खोलो(path, "rb");
    जर (!file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot open file: %s\n", path);
        परत false;
    }

    स्थिरांक संरचना parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    XkbFile *xkb_file = XkbParseFile(ctx, &config, file, path, map);
    ख_बंद(file);
    जर (!xkb_file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot parse map \"%s\" in file: %s\n",
                (map ? map : "(no map)"), path);
        परत false;
    }

    xkb_file_section_reset(section);
    स्थिरांक bool no_includes = iterator_flags & XKB_FILE_ITERATOR_NO_INCLUDES;
    स्थिरांक bool ok = (
        xkb_file_section_set_meta_data(ctx, section, xkb_file) &&
        (no_includes ||
         xkb_file_section_process(ctx, iterator_flags, &config, path, section,
                                  xkb_file))
    );
    FreeXkbFile(xkb_file);
    परत ok;
}

संरचना xkb_file_iterator *
xkb_file_iterator_new_from_buffer(संरचना xkb_context *ctx,
                                  गणना xkb_file_iterator_flags iterator_flags,
                                  गणना xkb_keymap_format format,
                                  गणना xkb_keymap_compile_flags compile_flags,
                                  स्थिरांक अक्षर *path, स्थिरांक अक्षर *map,
                                  गणना xkb_file_type file_type,
                                  स्थिरांक अक्षर *string, माप_प्रकार length)
{
    संरचना xkb_file_iterator * स्थिरांक iter = सुस्मृति(1, आकार(*iter));
    जर (!iter) {
        log_err(ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate file iterator\n");
        परत रिक्त;
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

    जर (!XkbParseStringInit(ctx, &iter->scanner, string, length, path, रिक्त)) {
        xkb_file_iterator_free(iter);
        परत रिक्त;
    }

    परत iter;
}

शून्य
xkb_file_iterator_free(संरचना xkb_file_iterator *iter)
{
    जर (!iter)
        परत;

    xkb_file_section_free(&iter->section);
    FreeXkbFile(iter->pending_xkb_file);
    मुक्त(iter);
}

bool
xkb_file_iterator_next(संरचना xkb_file_iterator *iter,
                       स्थिरांक संरचना xkb_file_section **section)
{
    जर (iter->finished) {
        *section = रिक्त;
        परत true;
    }

next:
    { /* C11 compatibility: label cannot be followed by a declaration */ }
    /* Parse next section in the file */
    XkbFile *xkb_file = रिक्त;

    जर (iter->pending_xkb_file) {
        /* We are parsing the components of a keymap */
        जर (iter->pending_section) {
            /* Parse next component */
            xkb_file = iter->pending_section;
            जा parse_components;
        } अन्यथा {
            /* No more components, try next keymap */
            FreeXkbFile(iter->pending_xkb_file);
            iter->pending_xkb_file = रिक्त;
        }
    }

    जर (!XkbParseStringNext(iter->ctx, &iter->parser_config, &iter->scanner,
                            iter->map, &xkb_file)) {
        log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Error while parsing section in file: %s\n", iter->path);
        जा error;
    } अन्यथा जर (!xkb_file) {
        /* No more sections */
        iter->finished = true;
        *section = रिक्त;
        परत true;
    }

parse_components:
    /* Reset section */
    xkb_file_section_reset(&iter->section);

    /* Meta data */
    जर (!xkb_file_section_set_meta_data(iter->ctx, &iter->section, xkb_file))
        जा error;

    /* Return current section */
    *section = &iter->section;

    जर (xkb_file->file_type == FILE_TYPE_KEYMAP) {
        /*
         * If it’s a keymap, then stop here.
         * Next iteration will process its components.
         */
        iter->pending_xkb_file = xkb_file;
        iter->pending_section = (XkbFile *) xkb_file->defs;
        iter->map = रिक्त;
        परत true;
    } अन्यथा {
        जर (iter->type != FILE_TYPE_INVALID &&
            xkb_file->file_type != iter->type) {
            जर (iter->pending_xkb_file) {
                /* Within a keymap: filter out component */
                iter->pending_section = (XkbFile *) xkb_file->common.next;
                जा next;
            } अन्यथा {
                /* Component-specific file: type mismatch */
                log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File type mismatch: %s, section: %s\n",
                        iter->path,
                        (xkb_file->name ? xkb_file->name : "(no name)"));
                जा error;
            }
        }
        जर (iter->map) {
            iter->finished = true;
        }
    }

    /* Collect include statements of current section */
    स्थिरांक bool process_includes = !(iter->flags & XKB_FILE_ITERATOR_NO_INCLUDES);
    जर (process_includes &&
        !xkb_file_section_process(iter->ctx, iter->flags, &iter->parser_config,
                                  iter->path, &iter->section, xkb_file)) {
        जा error;
    } अन्यथा जर (iter->pending_section) {
        /* Next component */
        iter->pending_section = (XkbFile *) xkb_file->common.next;
    } अन्यथा {
        /* No more component, free the file containing the components */
        FreeXkbFile(xkb_file);
    }

    परत true;

error:
    FreeXkbFile(xkb_file);
    *section = रिक्त;
    परत false;
}

/* Lookup a string by its index */
स्थिरांक अक्षर *
xkb_file_section_get_string(स्थिरांक संरचना xkb_file_section *section, darray_size_t idx)
{
    परत &darray_item(section->buffer, idx);
}
