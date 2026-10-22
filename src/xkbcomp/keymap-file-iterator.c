/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <limits.h>
#અન્તર્ભાવ <string.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "context.h"
#અન્તર્ભાવ "darray.h"
#અન્તર્ભાવ "include.h"
#અન્તર્ભાવ "keymap-formats.h"
#અન્તર્ભાવ "keymap-file-iterator.h"
#અન્તર્ભાવ "messages-codes.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "util-paths.h"
#અન્તર્ભાવ "xkbcomp-priv.h"
#અન્તર્ભાવ "xkbcomp/ast.h"

અચલ અક્ષર *
xkb_file_type_name(ગણના xkb_file_type type)
{
    જો (type > FILE_TYPE_KEYMAP)
        પાછા "unknown";

    સ્થિર અચલ અક્ષર *xkb_file_type_strings[FILE_TYPE_અંક_ENTRIES] = {
        [FILE_TYPE_KEYCODES] = "keycodes",
        [FILE_TYPE_TYPES] = "types",
        [FILE_TYPE_COMPAT] = "compatibility",
        [FILE_TYPE_SYMBOLS] = "symbols",
        [FILE_TYPE_GEOMETRY] = "geometry",
        [FILE_TYPE_KEYMAP] = "keymap",
    };
    પાછા xkb_file_type_strings[type];
}

અચલ અક્ષર *
xkb_merge_mode_name(ગણના merge_mode merge)
{
    જો (merge >= MERGE_MODE_અંક_ENTRIES)
        પાછા "unknown";

    સ્થિર અચલ અક્ષર *merge_mode_strings[MERGE_MODE_અંક_ENTRIES] = {
        [MERGE_DEFAULT] = "default",
        [MERGE_AUGMENT] = "augment",
        [MERGE_OVERRIDE] = "override",
        [MERGE_REPLACE] = "replace",
    };
    પાછા merge_mode_strings[merge];
}

/* Stateful lookup of map flags names */
અચલ અક્ષર *
xkb_map_flags_string_iter(unsigned પૂર્ણાંક *index, ગણના xkb_map_flags flags)
{
    જો (!flags)
        પાછા ખાલી;

    સ્થિર અચલ સંરચના {
        ગણના xkb_map_flags flag;
        અચલ અક્ષર *name;
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

    જ્યારે (*index < ARRAY_SIZE(names)) {
        જો (flags & names[*index].flag)
            પાછા names[(*index)++].name;
        (*index)++;
    }

    પાછા ખાલી;
};

FILE *
xkb_resolve_file(સંરચના xkb_context *ctx,
                 ગણના xkb_file_type file_type,
                 અચલ અક્ષર *path, અચલ અક્ષર *map,
                 અક્ષર *resolved_path, size_t resolved_path_size,
                 અક્ષર *resolved_map, size_t resolved_map_size)
{
    unsigned પૂર્ણાંક offset = 0;
    FILE *file = ખાલી; /* Exact match */
    FILE *candidate = ખાલી; /* Weak match */
    અચલ size_t path_len = strlen(path);

    અચલ bool absolute_path = is_absolute_path(path);
    જો (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        file = fopen(path, "rb");
    } નહીંતર {
        /* Relative path: lookup the first XKB path */
        file = FindFileInXkbPath(ctx, "(unknown)", path, path_len, file_type,
                                 resolved_path, resolved_path_size, &offset,
                                 true);
    }

    અચલ ગણના xkb_keymap_format format = DEFAULT_આગત_KEYMAP_FORMAT;
    અચલ સંરચના parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    જ્યારે (file) {
        XkbFile * અચલ xkb_file = XkbParseFile(ctx, &config, file, path, map);
        જો (xkb_file) {
            જો (file_type < FILE_TYPE_અંક_ENTRIES &&
                xkb_file->file_type != file_type) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File of wrong type (expected %s, got %s); "
                        "file \"%s\" ignored\n",
                        xkb_file_type_to_string(file_type),
                        xkb_file_type_to_string(xkb_file->file_type),
                        (absolute_path ? path : resolved_path));
                જાઓ invalid_file;
            } નહીંતર જો (map || (xkb_file->flags && MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops in this iteration.
                 */
                જો (!strcpy_safe(resolved_map, resolved_map_size,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    જાઓ error;
                }
            } નહીંતર જો (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = file;
                જો (!strcpy_safe(resolved_map, resolved_map_size,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    જાઓ error;
                }
            } નહીંતર {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                જાઓ invalid_file;
            }
        } નહીંતર {
invalid_file:
            fclose(file);
            file = ખાલી;
        }

        FreeXkbFile(xkb_file);

        જો (file != ખાલી || absolute_path) {
            /* Exact match or absolute path with no further file to search */
            તોડો;
        }

        offset++;
        file = FindFileInXkbPath(ctx, "(unknown)",
                                 path, path_len, file_type,
                                 resolved_path, resolved_path_size,
                                 &offset, true);
    }

    જો (!file) {
        /* No exact match: use weak match, if any */
        file = candidate;
    } નહીંતર જો (candidate) {
        /* Found exact match: discard weak match, if any */
        fclose(candidate);
    }

    જો (absolute_path && file &&
        !strcpy_safe(resolved_path, resolved_path_size, path)) {
        જાઓ error;
    }

    પાછા file;

error:
    log_err(ctx, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
            "Cannot copy resolved path or section\n");
    fclose(file);
    પાછા ખાલી;
}

શૂન્ય
xkb_file_section_init(સંરચના xkb_file_section *section)
{
    darray_init(section->include_groups);
    darray_init(section->includes);
    darray_init(section->buffer);
    darray_append(section->buffer, '\0');
}

સ્થિર શૂન્ય
xkb_file_section_reset(સંરચના xkb_file_section *section)
{
    darray_size(section->include_groups) = 0;
    darray_size(section->includes) = 0;
    darray_size(section->buffer) = 1; /* keep the dummy string */
}

શૂન્ય
xkb_file_section_free(સંરચના xkb_file_section *section)
{
    જો (!section)
        પાછા;

    darray_free(section->include_groups);
    darray_free(section->includes);
    darray_free(section->buffer);
}

સ્થિર bool
xkb_file_section_set_meta_data(સંરચના xkb_context *ctx,
                               સંરચના xkb_file_section *section,
                               અચલ XkbFile *xkb_file)
{
    section->file_type = xkb_file->file_type;
    section->flags = xkb_file->flags;
    જો (xkb_file->name) {
        darray_size_t idx = darray_size(section->buffer);
        darray_append_string0(section->buffer, xkb_file->name);
        section->name = idx;
    } નહીંતર {
        section->name = 0;
    }
    પાછા true;
}

/** Process a list of include statements */
સ્થિર bool
xkb_file_section_append_includes(સંરચના xkb_context *ctx,
                                 ગણના xkb_file_iterator_flags flags,
                                 અચલ સંરચના parser_keymap_config *config,
                                 અચલ અક્ષર *section_path,
                                 સંરચના xkb_file_section *section,
                                 ગણના xkb_file_type file_type,
                                 IncludeStmt * include)
{
    સંરચના xkb_file_include_group *group = ખાલી;
    /* Note: this will flatten statements such as `include "pc+de"` */
    માટે (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        અક્ષર buf[PATH_MAX];
        /* Parse the included file to check the include validity */
        XkbFile *xkb_file = ProcessIncludeFile(ctx, config, stmt, file_type,
                                               buf, કદ(buf));
        અચલ bool valid = (xkb_file != ખાલી);
        જો (valid || !(flags & XKB_FILE_ITERATOR_FAIL_ON_INCLUDE_ERROR)) {
            /* Collect the strings of the statement properties */

            અચલ darray_size_t path = darray_size(section->buffer);
            darray_append_string0(section->buffer, buf);

            અચલ darray_size_t file = darray_size(section->buffer);
            darray_append_string0(section->buffer, stmt->file);

            અચલ darray_size_t section_name =
                (stmt->map != ખાલી || (valid && xkb_file->name))
                    ? darray_size(section->buffer)
                    : 0;
            જો (section_name) {
                darray_append_string0(section->buffer,
                                      (stmt->map) ? stmt->map : xkb_file->name);
            }

            અચલ darray_size_t modifier = (stmt->modifier)
                ? darray_size(section->buffer)
                : 0;
            જો (modifier) {
                darray_append_string0(section->buffer, stmt->modifier);
            }

            અચલ ગણના xkb_map_flags section_flags = (valid)
                ? xkb_file->flags
                : 0;

            /* Create and append the include statement */
            અચલ સંરચના xkb_file_include inc = {
                .valid = valid,
                .explicit_section = (stmt->map != ખાલી),
                .merge = stmt->merge,
                .path = path,
                .file = file,
                .section = section_name,
                .modifier = modifier,
                .flags = section_flags
            };
            અચલ darray_size_t idx = darray_size(section->includes);
            darray_append(section->includes, inc);

            /* Update include group */
            જો (group == ખાલી) {
                અચલ darray_size_t group_idx =
                    darray_size(section->include_groups);
                darray_append(
                    section->include_groups,
                    (સંરચના xkb_file_include_group) {.start = idx, .end = idx}
                );
                group = &darray_item(section->include_groups, group_idx);
            } નહીંતર {
                group->end = idx;
            }

            FreeXkbFile(xkb_file);
        } નહીંતર {
            અચલ અક્ષર * અચલ name =
                xkb_file_section_get_string(section, section->name);
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "%s include failure in: %s%s%s%s\n",
                    xkb_file_type_name(file_type), section_path,
                    (section->name ? " (section: \"": ""), name,
                    (section->name ? "\")": ""));
            FreeXkbFile(xkb_file);
            પાછા false;
        }
    };
    પાછા true;
}

/* Process the AST of a section */
સ્થિર bool
xkb_file_section_process(સંરચના xkb_context *ctx,
                         ગણના xkb_file_iterator_flags flags,
                         અચલ સંરચના parser_keymap_config *config,
                         અચલ અક્ષર *path,
                         સંરચના xkb_file_section *section,
                         અચલ XkbFile *xkb_file)
{
    bool ok = true;
    માટે (ParseCommon *stmt = xkb_file->defs; stmt; stmt = stmt->next) {
        જો (stmt->type == STMT_INCLUDE) {
            ok = xkb_file_section_append_includes(ctx, flags, config, path,
                                                  section, xkb_file->file_type,
                                                  (IncludeStmt *) stmt);
            જો (!ok)
                તોડો;
        }
    }
    પાછા ok;
}

bool
xkb_file_section_parse(સંરચના xkb_context *ctx,
                       ગણના xkb_file_iterator_flags iterator_flags,
                       ગણના xkb_keymap_format format,
                       ગણના xkb_keymap_compile_flags compile_flags,
                       unsigned પૂર્ણાંક include_depth,
                       અચલ અક્ષર *path, અચલ અક્ષર *map,
                       સંરચના xkb_file_section *section)
{
    જો (ExceedsIncludeMaxDepth(ctx, include_depth))
        પાછા false;

    FILE *file = fopen(path, "rb");
    જો (!file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot open file: %s\n", path);
        પાછા false;
    }

    અચલ સંરચના parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    XkbFile *xkb_file = XkbParseFile(ctx, &config, file, path, map);
    fclose(file);
    જો (!xkb_file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot parse map \"%s\" in file: %s\n",
                (map ? map : "(no map)"), path);
        પાછા false;
    }

    xkb_file_section_reset(section);
    અચલ bool no_includes = iterator_flags & XKB_FILE_ITERATOR_NO_INCLUDES;
    અચલ bool ok = (
        xkb_file_section_set_meta_data(ctx, section, xkb_file) &&
        (no_includes ||
         xkb_file_section_process(ctx, iterator_flags, &config, path, section,
                                  xkb_file))
    );
    FreeXkbFile(xkb_file);
    પાછા ok;
}

સંરચના xkb_file_iterator *
xkb_file_iterator_new_from_buffer(સંરચના xkb_context *ctx,
                                  ગણના xkb_file_iterator_flags iterator_flags,
                                  ગણના xkb_keymap_format format,
                                  ગણના xkb_keymap_compile_flags compile_flags,
                                  અચલ અક્ષર *path, અચલ અક્ષર *map,
                                  ગણના xkb_file_type file_type,
                                  અચલ અક્ષર *string, size_t length)
{
    સંરચના xkb_file_iterator * અચલ iter = calloc(1, કદ(*iter));
    જો (!iter) {
        log_err(ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate file iterator\n");
        પાછા ખાલી;
    }

    iter->flags = iterator_flags;
    iter->ctx = ctx;
    iter->path = path;
    iter->map = map;
    iter->type = file_type;
    iter->parser_config = (સંરચના parser_keymap_config) {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };
    xkb_file_section_init(&iter->section);

    જો (!XkbParseStringInit(ctx, &iter->scanner, string, length, path, ખાલી)) {
        xkb_file_iterator_free(iter);
        પાછા ખાલી;
    }

    પાછા iter;
}

શૂન્ય
xkb_file_iterator_free(સંરચના xkb_file_iterator *iter)
{
    જો (!iter)
        પાછા;

    xkb_file_section_free(&iter->section);
    FreeXkbFile(iter->pending_xkb_file);
    મુક્ત(iter);
}

bool
xkb_file_iterator_next(સંરચના xkb_file_iterator *iter,
                       અચલ સંરચના xkb_file_section **section)
{
    જો (iter->finished) {
        *section = ખાલી;
        પાછા true;
    }

next:
    { /* C11 compatibility: label cannot be followed by a declaration */ }
    /* Parse next section in the file */
    XkbFile *xkb_file = ખાલી;

    જો (iter->pending_xkb_file) {
        /* We are parsing the components of a keymap */
        જો (iter->pending_section) {
            /* Parse next component */
            xkb_file = iter->pending_section;
            જાઓ parse_components;
        } નહીંતર {
            /* No more components, try next keymap */
            FreeXkbFile(iter->pending_xkb_file);
            iter->pending_xkb_file = ખાલી;
        }
    }

    જો (!XkbParseStringNext(iter->ctx, &iter->parser_config, &iter->scanner,
                            iter->map, &xkb_file)) {
        log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Error while parsing section in file: %s\n", iter->path);
        જાઓ error;
    } નહીંતર જો (!xkb_file) {
        /* No more sections */
        iter->finished = true;
        *section = ખાલી;
        પાછા true;
    }

parse_components:
    /* Reset section */
    xkb_file_section_reset(&iter->section);

    /* Meta data */
    જો (!xkb_file_section_set_meta_data(iter->ctx, &iter->section, xkb_file))
        જાઓ error;

    /* Return current section */
    *section = &iter->section;

    જો (xkb_file->file_type == FILE_TYPE_KEYMAP) {
        /*
         * If it’s a keymap, then stop here.
         * Next iteration will process its components.
         */
        iter->pending_xkb_file = xkb_file;
        iter->pending_section = (XkbFile *) xkb_file->defs;
        iter->map = ખાલી;
        પાછા true;
    } નહીંતર {
        જો (iter->type != FILE_TYPE_INVALID &&
            xkb_file->file_type != iter->type) {
            જો (iter->pending_xkb_file) {
                /* Within a keymap: filter out component */
                iter->pending_section = (XkbFile *) xkb_file->common.next;
                જાઓ next;
            } નહીંતર {
                /* Component-specific file: type mismatch */
                log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File type mismatch: %s, section: %s\n",
                        iter->path,
                        (xkb_file->name ? xkb_file->name : "(no name)"));
                જાઓ error;
            }
        }
        જો (iter->map) {
            iter->finished = true;
        }
    }

    /* Collect include statements of current section */
    અચલ bool process_includes = !(iter->flags & XKB_FILE_ITERATOR_NO_INCLUDES);
    જો (process_includes &&
        !xkb_file_section_process(iter->ctx, iter->flags, &iter->parser_config,
                                  iter->path, &iter->section, xkb_file)) {
        જાઓ error;
    } નહીંતર જો (iter->pending_section) {
        /* Next component */
        iter->pending_section = (XkbFile *) xkb_file->common.next;
    } નહીંતર {
        /* No more component, free the file containing the components */
        FreeXkbFile(xkb_file);
    }

    પાછા true;

error:
    FreeXkbFile(xkb_file);
    *section = ખાલી;
    પાછા false;
}

/* Lookup a string by its index */
અચલ અક્ષર *
xkb_file_section_get_string(અચલ સંરચના xkb_file_section *section, darray_size_t idx)
{
    પાછા &darray_item(section->buffer, idx);
}
