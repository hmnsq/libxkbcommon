/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <limits.h>
#അന്തര്ഭാവ <string.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "context.h"
#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "include.h"
#അന്തര്ഭാവ "keymap-formats.h"
#അന്തര്ഭാവ "keymap-file-iterator.h"
#അന്തര്ഭാവ "messages-codes.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "util-paths.h"
#അന്തര്ഭാവ "xkbcomp-priv.h"
#അന്തര്ഭാവ "xkbcomp/ast.h"

സ്ഥിരം അക്ഷരം *
xkb_file_type_name(എണ്ണൽ xkb_file_type type)
{
    എങ്കിൽ (type > FILE_TYPE_KEYMAP)
        തിരികെ "unknown";

    സ്ഥിതം സ്ഥിരം അക്ഷരം *xkb_file_type_strings[FILE_TYPE_അക്കം_ENTRIES] = {
        [FILE_TYPE_KEYCODES] = "keycodes",
        [FILE_TYPE_TYPES] = "types",
        [FILE_TYPE_COMPAT] = "compatibility",
        [FILE_TYPE_SYMBOLS] = "symbols",
        [FILE_TYPE_GEOMETRY] = "geometry",
        [FILE_TYPE_KEYMAP] = "keymap",
    };
    തിരികെ xkb_file_type_strings[type];
}

സ്ഥിരം അക്ഷരം *
xkb_merge_mode_name(എണ്ണൽ merge_mode merge)
{
    എങ്കിൽ (merge >= MERGE_MODE_അക്കം_ENTRIES)
        തിരികെ "unknown";

    സ്ഥിതം സ്ഥിരം അക്ഷരം *merge_mode_strings[MERGE_MODE_അക്കം_ENTRIES] = {
        [MERGE_DEFAULT] = "default",
        [MERGE_AUGMENT] = "augment",
        [MERGE_OVERRIDE] = "override",
        [MERGE_REPLACE] = "replace",
    };
    തിരികെ merge_mode_strings[merge];
}

/* Stateful lookup of map flags names */
സ്ഥിരം അക്ഷരം *
xkb_map_flags_string_iter(unsigned പൂർണ്ണസംഖ്യ *index, എണ്ണൽ xkb_map_flags flags)
{
    എങ്കിൽ (!flags)
        തിരികെ ശൂന്യ;

    സ്ഥിതം സ്ഥിരം ഘടന {
        എണ്ണൽ xkb_map_flags flag;
        സ്ഥിരം അക്ഷരം *name;
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

    ആകുമ്പോൾ (*index < ARRAY_SIZE(names)) {
        എങ്കിൽ (flags & names[*index].flag)
            തിരികെ names[(*index)++].name;
        (*index)++;
    }

    തിരികെ ശൂന്യ;
};

FILE *
xkb_resolve_file(ഘടന xkb_context *ctx,
                 എണ്ണൽ xkb_file_type file_type,
                 സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *map,
                 അക്ഷരം *resolved_path, size_t resolved_path_size,
                 അക്ഷരം *resolved_map, size_t resolved_map_size)
{
    unsigned പൂർണ്ണസംഖ്യ offset = 0;
    FILE *file = ശൂന്യ; /* Exact match */
    FILE *candidate = ശൂന്യ; /* Weak match */
    സ്ഥിരം size_t path_len = strlen(path);

    സ്ഥിരം bool absolute_path = is_absolute_path(path);
    എങ്കിൽ (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        file = fopen(path, "rb");
    } അല്ലെങ്കിൽ {
        /* Relative path: lookup the first XKB path */
        file = FindFileInXkbPath(ctx, "(unknown)", path, path_len, file_type,
                                 resolved_path, resolved_path_size, &offset,
                                 true);
    }

    സ്ഥിരം എണ്ണൽ xkb_keymap_format format = DEFAULT_നിവേശം_KEYMAP_FORMAT;
    സ്ഥിരം ഘടന parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    ആകുമ്പോൾ (file) {
        XkbFile * സ്ഥിരം xkb_file = XkbParseFile(ctx, &config, file, path, map);
        എങ്കിൽ (xkb_file) {
            എങ്കിൽ (file_type < FILE_TYPE_അക്കം_ENTRIES &&
                xkb_file->file_type != file_type) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File of wrong type (expected %s, got %s); "
                        "file \"%s\" ignored\n",
                        xkb_file_type_to_string(file_type),
                        xkb_file_type_to_string(xkb_file->file_type),
                        (absolute_path ? path : resolved_path));
                പോകുക invalid_file;
            } അല്ലെങ്കിൽ എങ്കിൽ (map || (xkb_file->flags && MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops in this iteration.
                 */
                എങ്കിൽ (!strcpy_safe(resolved_map, resolved_map_size,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    പോകുക error;
                }
            } അല്ലെങ്കിൽ എങ്കിൽ (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = file;
                എങ്കിൽ (!strcpy_safe(resolved_map, resolved_map_size,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    പോകുക error;
                }
            } അല്ലെങ്കിൽ {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                പോകുക invalid_file;
            }
        } അല്ലെങ്കിൽ {
invalid_file:
            fclose(file);
            file = ശൂന്യ;
        }

        FreeXkbFile(xkb_file);

        എങ്കിൽ (file != ശൂന്യ || absolute_path) {
            /* Exact match or absolute path with no further file to search */
            നിർത്തുക;
        }

        offset++;
        file = FindFileInXkbPath(ctx, "(unknown)",
                                 path, path_len, file_type,
                                 resolved_path, resolved_path_size,
                                 &offset, true);
    }

    എങ്കിൽ (!file) {
        /* No exact match: use weak match, if any */
        file = candidate;
    } അല്ലെങ്കിൽ എങ്കിൽ (candidate) {
        /* Found exact match: discard weak match, if any */
        fclose(candidate);
    }

    എങ്കിൽ (absolute_path && file &&
        !strcpy_safe(resolved_path, resolved_path_size, path)) {
        പോകുക error;
    }

    തിരികെ file;

error:
    log_err(ctx, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
            "Cannot copy resolved path or section\n");
    fclose(file);
    തിരികെ ശൂന്യ;
}

ശൂന്യം
xkb_file_section_init(ഘടന xkb_file_section *section)
{
    darray_init(section->include_groups);
    darray_init(section->includes);
    darray_init(section->buffer);
    darray_append(section->buffer, '\0');
}

സ്ഥിതം ശൂന്യം
xkb_file_section_reset(ഘടന xkb_file_section *section)
{
    darray_size(section->include_groups) = 0;
    darray_size(section->includes) = 0;
    darray_size(section->buffer) = 1; /* keep the dummy string */
}

ശൂന്യം
xkb_file_section_free(ഘടന xkb_file_section *section)
{
    എങ്കിൽ (!section)
        തിരികെ;

    darray_free(section->include_groups);
    darray_free(section->includes);
    darray_free(section->buffer);
}

സ്ഥിതം bool
xkb_file_section_set_meta_data(ഘടന xkb_context *ctx,
                               ഘടന xkb_file_section *section,
                               സ്ഥിരം XkbFile *xkb_file)
{
    section->file_type = xkb_file->file_type;
    section->flags = xkb_file->flags;
    എങ്കിൽ (xkb_file->name) {
        darray_size_t idx = darray_size(section->buffer);
        darray_append_string0(section->buffer, xkb_file->name);
        section->name = idx;
    } അല്ലെങ്കിൽ {
        section->name = 0;
    }
    തിരികെ true;
}

/** Process a list of include statements */
സ്ഥിതം bool
xkb_file_section_append_includes(ഘടന xkb_context *ctx,
                                 എണ്ണൽ xkb_file_iterator_flags flags,
                                 സ്ഥിരം ഘടന parser_keymap_config *config,
                                 സ്ഥിരം അക്ഷരം *section_path,
                                 ഘടന xkb_file_section *section,
                                 എണ്ണൽ xkb_file_type file_type,
                                 IncludeStmt * include)
{
    ഘടന xkb_file_include_group *group = ശൂന്യ;
    /* Note: this will flatten statements such as `include "pc+de"` */
    വേണ്ടി (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        അക്ഷരം buf[PATH_MAX];
        /* Parse the included file to check the include validity */
        XkbFile *xkb_file = ProcessIncludeFile(ctx, config, stmt, file_type,
                                               buf, വലുപ്പം(buf));
        സ്ഥിരം bool valid = (xkb_file != ശൂന്യ);
        എങ്കിൽ (valid || !(flags & XKB_FILE_ITERATOR_FAIL_ON_INCLUDE_ERROR)) {
            /* Collect the strings of the statement properties */

            സ്ഥിരം darray_size_t path = darray_size(section->buffer);
            darray_append_string0(section->buffer, buf);

            സ്ഥിരം darray_size_t file = darray_size(section->buffer);
            darray_append_string0(section->buffer, stmt->file);

            സ്ഥിരം darray_size_t section_name =
                (stmt->map != ശൂന്യ || (valid && xkb_file->name))
                    ? darray_size(section->buffer)
                    : 0;
            എങ്കിൽ (section_name) {
                darray_append_string0(section->buffer,
                                      (stmt->map) ? stmt->map : xkb_file->name);
            }

            സ്ഥിരം darray_size_t modifier = (stmt->modifier)
                ? darray_size(section->buffer)
                : 0;
            എങ്കിൽ (modifier) {
                darray_append_string0(section->buffer, stmt->modifier);
            }

            സ്ഥിരം എണ്ണൽ xkb_map_flags section_flags = (valid)
                ? xkb_file->flags
                : 0;

            /* Create and append the include statement */
            സ്ഥിരം ഘടന xkb_file_include inc = {
                .valid = valid,
                .explicit_section = (stmt->map != ശൂന്യ),
                .merge = stmt->merge,
                .path = path,
                .file = file,
                .section = section_name,
                .modifier = modifier,
                .flags = section_flags
            };
            സ്ഥിരം darray_size_t idx = darray_size(section->includes);
            darray_append(section->includes, inc);

            /* Update include group */
            എങ്കിൽ (group == ശൂന്യ) {
                സ്ഥിരം darray_size_t group_idx =
                    darray_size(section->include_groups);
                darray_append(
                    section->include_groups,
                    (ഘടന xkb_file_include_group) {.start = idx, .end = idx}
                );
                group = &darray_item(section->include_groups, group_idx);
            } അല്ലെങ്കിൽ {
                group->end = idx;
            }

            FreeXkbFile(xkb_file);
        } അല്ലെങ്കിൽ {
            സ്ഥിരം അക്ഷരം * സ്ഥിരം name =
                xkb_file_section_get_string(section, section->name);
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "%s include failure in: %s%s%s%s\n",
                    xkb_file_type_name(file_type), section_path,
                    (section->name ? " (section: \"": ""), name,
                    (section->name ? "\")": ""));
            FreeXkbFile(xkb_file);
            തിരികെ false;
        }
    };
    തിരികെ true;
}

/* Process the AST of a section */
സ്ഥിതം bool
xkb_file_section_process(ഘടന xkb_context *ctx,
                         എണ്ണൽ xkb_file_iterator_flags flags,
                         സ്ഥിരം ഘടന parser_keymap_config *config,
                         സ്ഥിരം അക്ഷരം *path,
                         ഘടന xkb_file_section *section,
                         സ്ഥിരം XkbFile *xkb_file)
{
    bool ok = true;
    വേണ്ടി (ParseCommon *stmt = xkb_file->defs; stmt; stmt = stmt->next) {
        എങ്കിൽ (stmt->type == STMT_INCLUDE) {
            ok = xkb_file_section_append_includes(ctx, flags, config, path,
                                                  section, xkb_file->file_type,
                                                  (IncludeStmt *) stmt);
            എങ്കിൽ (!ok)
                നിർത്തുക;
        }
    }
    തിരികെ ok;
}

bool
xkb_file_section_parse(ഘടന xkb_context *ctx,
                       എണ്ണൽ xkb_file_iterator_flags iterator_flags,
                       എണ്ണൽ xkb_keymap_format format,
                       എണ്ണൽ xkb_keymap_compile_flags compile_flags,
                       unsigned പൂർണ്ണസംഖ്യ include_depth,
                       സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *map,
                       ഘടന xkb_file_section *section)
{
    എങ്കിൽ (ExceedsIncludeMaxDepth(ctx, include_depth))
        തിരികെ false;

    FILE *file = fopen(path, "rb");
    എങ്കിൽ (!file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot open file: %s\n", path);
        തിരികെ false;
    }

    സ്ഥിരം ഘടന parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    XkbFile *xkb_file = XkbParseFile(ctx, &config, file, path, map);
    fclose(file);
    എങ്കിൽ (!xkb_file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot parse map \"%s\" in file: %s\n",
                (map ? map : "(no map)"), path);
        തിരികെ false;
    }

    xkb_file_section_reset(section);
    സ്ഥിരം bool no_includes = iterator_flags & XKB_FILE_ITERATOR_NO_INCLUDES;
    സ്ഥിരം bool ok = (
        xkb_file_section_set_meta_data(ctx, section, xkb_file) &&
        (no_includes ||
         xkb_file_section_process(ctx, iterator_flags, &config, path, section,
                                  xkb_file))
    );
    FreeXkbFile(xkb_file);
    തിരികെ ok;
}

ഘടന xkb_file_iterator *
xkb_file_iterator_new_from_buffer(ഘടന xkb_context *ctx,
                                  എണ്ണൽ xkb_file_iterator_flags iterator_flags,
                                  എണ്ണൽ xkb_keymap_format format,
                                  എണ്ണൽ xkb_keymap_compile_flags compile_flags,
                                  സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *map,
                                  എണ്ണൽ xkb_file_type file_type,
                                  സ്ഥിരം അക്ഷരം *string, size_t length)
{
    ഘടന xkb_file_iterator * സ്ഥിരം iter = calloc(1, വലുപ്പം(*iter));
    എങ്കിൽ (!iter) {
        log_err(ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate file iterator\n");
        തിരികെ ശൂന്യ;
    }

    iter->flags = iterator_flags;
    iter->ctx = ctx;
    iter->path = path;
    iter->map = map;
    iter->type = file_type;
    iter->parser_config = (ഘടന parser_keymap_config) {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };
    xkb_file_section_init(&iter->section);

    എങ്കിൽ (!XkbParseStringInit(ctx, &iter->scanner, string, length, path, ശൂന്യ)) {
        xkb_file_iterator_free(iter);
        തിരികെ ശൂന്യ;
    }

    തിരികെ iter;
}

ശൂന്യം
xkb_file_iterator_free(ഘടന xkb_file_iterator *iter)
{
    എങ്കിൽ (!iter)
        തിരികെ;

    xkb_file_section_free(&iter->section);
    FreeXkbFile(iter->pending_xkb_file);
    സ്വതന്ത്രം(iter);
}

bool
xkb_file_iterator_next(ഘടന xkb_file_iterator *iter,
                       സ്ഥിരം ഘടന xkb_file_section **section)
{
    എങ്കിൽ (iter->finished) {
        *section = ശൂന്യ;
        തിരികെ true;
    }

next:
    { /* C11 compatibility: label cannot be followed by a declaration */ }
    /* Parse next section in the file */
    XkbFile *xkb_file = ശൂന്യ;

    എങ്കിൽ (iter->pending_xkb_file) {
        /* We are parsing the components of a keymap */
        എങ്കിൽ (iter->pending_section) {
            /* Parse next component */
            xkb_file = iter->pending_section;
            പോകുക parse_components;
        } അല്ലെങ്കിൽ {
            /* No more components, try next keymap */
            FreeXkbFile(iter->pending_xkb_file);
            iter->pending_xkb_file = ശൂന്യ;
        }
    }

    എങ്കിൽ (!XkbParseStringNext(iter->ctx, &iter->parser_config, &iter->scanner,
                            iter->map, &xkb_file)) {
        log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Error while parsing section in file: %s\n", iter->path);
        പോകുക error;
    } അല്ലെങ്കിൽ എങ്കിൽ (!xkb_file) {
        /* No more sections */
        iter->finished = true;
        *section = ശൂന്യ;
        തിരികെ true;
    }

parse_components:
    /* Reset section */
    xkb_file_section_reset(&iter->section);

    /* Meta data */
    എങ്കിൽ (!xkb_file_section_set_meta_data(iter->ctx, &iter->section, xkb_file))
        പോകുക error;

    /* Return current section */
    *section = &iter->section;

    എങ്കിൽ (xkb_file->file_type == FILE_TYPE_KEYMAP) {
        /*
         * If it’s a keymap, then stop here.
         * Next iteration will process its components.
         */
        iter->pending_xkb_file = xkb_file;
        iter->pending_section = (XkbFile *) xkb_file->defs;
        iter->map = ശൂന്യ;
        തിരികെ true;
    } അല്ലെങ്കിൽ {
        എങ്കിൽ (iter->type != FILE_TYPE_INVALID &&
            xkb_file->file_type != iter->type) {
            എങ്കിൽ (iter->pending_xkb_file) {
                /* Within a keymap: filter out component */
                iter->pending_section = (XkbFile *) xkb_file->common.next;
                പോകുക next;
            } അല്ലെങ്കിൽ {
                /* Component-specific file: type mismatch */
                log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File type mismatch: %s, section: %s\n",
                        iter->path,
                        (xkb_file->name ? xkb_file->name : "(no name)"));
                പോകുക error;
            }
        }
        എങ്കിൽ (iter->map) {
            iter->finished = true;
        }
    }

    /* Collect include statements of current section */
    സ്ഥിരം bool process_includes = !(iter->flags & XKB_FILE_ITERATOR_NO_INCLUDES);
    എങ്കിൽ (process_includes &&
        !xkb_file_section_process(iter->ctx, iter->flags, &iter->parser_config,
                                  iter->path, &iter->section, xkb_file)) {
        പോകുക error;
    } അല്ലെങ്കിൽ എങ്കിൽ (iter->pending_section) {
        /* Next component */
        iter->pending_section = (XkbFile *) xkb_file->common.next;
    } അല്ലെങ്കിൽ {
        /* No more component, free the file containing the components */
        FreeXkbFile(xkb_file);
    }

    തിരികെ true;

error:
    FreeXkbFile(xkb_file);
    *section = ശൂന്യ;
    തിരികെ false;
}

/* Lookup a string by its index */
സ്ഥിരം അക്ഷരം *
xkb_file_section_get_string(സ്ഥിരം ഘടന xkb_file_section *section, darray_size_t idx)
{
    തിരികെ &darray_item(section->buffer, idx);
}
