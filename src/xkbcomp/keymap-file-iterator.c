/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <limits.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "context.h"
#ଅନ୍ତର୍ଭାଵ "darray.h"
#ଅନ୍ତର୍ଭାଵ "include.h"
#ଅନ୍ତର୍ଭାଵ "keymap-formats.h"
#ଅନ୍ତର୍ଭାଵ "keymap-file-iterator.h"
#ଅନ୍ତର୍ଭାଵ "messages-codes.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "util-paths.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp-priv.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp/ast.h"

ଅଚଳ ଅକ୍ଷର *
xkb_file_type_name(ଗଣନା xkb_file_type type)
{
    ଯଦି (type > FILE_TYPE_KEYMAP)
        ଫେରନ୍ତୁ "unknown";

    ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *xkb_file_type_strings[FILE_TYPE_ଅଙ୍କ_ENTRIES] = {
        [FILE_TYPE_KEYCODES] = "keycodes",
        [FILE_TYPE_TYPES] = "types",
        [FILE_TYPE_COMPAT] = "compatibility",
        [FILE_TYPE_SYMBOLS] = "symbols",
        [FILE_TYPE_GEOMETRY] = "geometry",
        [FILE_TYPE_KEYMAP] = "keymap",
    };
    ଫେରନ୍ତୁ xkb_file_type_strings[type];
}

ଅଚଳ ଅକ୍ଷର *
xkb_merge_mode_name(ଗଣନା merge_mode merge)
{
    ଯଦି (merge >= MERGE_MODE_ଅଙ୍କ_ENTRIES)
        ଫେରନ୍ତୁ "unknown";

    ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *merge_mode_strings[MERGE_MODE_ଅଙ୍କ_ENTRIES] = {
        [MERGE_DEFAULT] = "default",
        [MERGE_AUGMENT] = "augment",
        [MERGE_OVERRIDE] = "override",
        [MERGE_REPLACE] = "replace",
    };
    ଫେରନ୍ତୁ merge_mode_strings[merge];
}

/* Stateful lookup of map flags names */
ଅଚଳ ଅକ୍ଷର *
xkb_map_flags_string_iter(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *index, ଗଣନା xkb_map_flags flags)
{
    ଯଦି (!flags)
        ଫେରନ୍ତୁ ଖାଲି;

    ସ୍ଥିର ଅଚଳ ସଂରଚନା {
        ଗଣନା xkb_map_flags flag;
        ଅଚଳ ଅକ୍ଷର *name;
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

    ଯେତେବେଳେ (*index < ARRAY_SIZE(names)) {
        ଯଦି (flags & names[*index].flag)
            ଫେରନ୍ତୁ names[(*index)++].name;
        (*index)++;
    }

    ଫେରନ୍ତୁ ଖାଲି;
};

FILE *
xkb_resolve_file(ସଂରଚନା xkb_context *ctx,
                 ଗଣନା xkb_file_type file_type,
                 ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *map,
                 ଅକ୍ଷର *resolved_path, size_t resolved_path_size,
                 ଅକ୍ଷର *resolved_map, size_t resolved_map_size)
{
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = 0;
    FILE *file = ଖାଲି; /* Exact match */
    FILE *candidate = ଖାଲି; /* Weak match */
    ଅଚଳ size_t path_len = strlen(path);

    ଅଚଳ bool absolute_path = is_absolute_path(path);
    ଯଦି (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        file = fopen(path, "rb");
    } ଅନ୍ୟଥା {
        /* Relative path: lookup the first XKB path */
        file = FindFileInXkbPath(ctx, "(unknown)", path, path_len, file_type,
                                 resolved_path, resolved_path_size, &offset,
                                 true);
    }

    ଅଚଳ ଗଣନା xkb_keymap_format format = DEFAULT_ନିବେଶ_KEYMAP_FORMAT;
    ଅଚଳ ସଂରଚନା parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    ଯେତେବେଳେ (file) {
        XkbFile * ଅଚଳ xkb_file = XkbParseFile(ctx, &config, file, path, map);
        ଯଦି (xkb_file) {
            ଯଦି (file_type < FILE_TYPE_ଅଙ୍କ_ENTRIES &&
                xkb_file->file_type != file_type) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File of wrong type (expected %s, got %s); "
                        "file \"%s\" ignored\n",
                        xkb_file_type_to_string(file_type),
                        xkb_file_type_to_string(xkb_file->file_type),
                        (absolute_path ? path : resolved_path));
                ଯାଅ invalid_file;
            } ଅନ୍ୟଥା ଯଦି (map || (xkb_file->flags && MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops in this iteration.
                 */
                ଯଦି (!strcpy_safe(resolved_map, resolved_map_size,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    ଯାଅ error;
                }
            } ଅନ୍ୟଥା ଯଦି (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = file;
                ଯଦି (!strcpy_safe(resolved_map, resolved_map_size,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    ଯାଅ error;
                }
            } ଅନ୍ୟଥା {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                ଯାଅ invalid_file;
            }
        } ଅନ୍ୟଥା {
invalid_file:
            fclose(file);
            file = ଖାଲି;
        }

        FreeXkbFile(xkb_file);

        ଯଦି (file != ଖାଲି || absolute_path) {
            /* Exact match or absolute path with no further file to search */
            ଭାଙ୍ଗ;
        }

        offset++;
        file = FindFileInXkbPath(ctx, "(unknown)",
                                 path, path_len, file_type,
                                 resolved_path, resolved_path_size,
                                 &offset, true);
    }

    ଯଦି (!file) {
        /* No exact match: use weak match, if any */
        file = candidate;
    } ଅନ୍ୟଥା ଯଦି (candidate) {
        /* Found exact match: discard weak match, if any */
        fclose(candidate);
    }

    ଯଦି (absolute_path && file &&
        !strcpy_safe(resolved_path, resolved_path_size, path)) {
        ଯାଅ error;
    }

    ଫେରନ୍ତୁ file;

error:
    log_err(ctx, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
            "Cannot copy resolved path or section\n");
    fclose(file);
    ଫେରନ୍ତୁ ଖାଲି;
}

ଶୂନ୍ୟ
xkb_file_section_init(ସଂରଚନା xkb_file_section *section)
{
    darray_init(section->include_groups);
    darray_init(section->includes);
    darray_init(section->buffer);
    darray_append(section->buffer, '\0');
}

ସ୍ଥିର ଶୂନ୍ୟ
xkb_file_section_reset(ସଂରଚନା xkb_file_section *section)
{
    darray_size(section->include_groups) = 0;
    darray_size(section->includes) = 0;
    darray_size(section->buffer) = 1; /* keep the dummy string */
}

ଶୂନ୍ୟ
xkb_file_section_free(ସଂରଚନା xkb_file_section *section)
{
    ଯଦି (!section)
        ଫେରନ୍ତୁ;

    darray_free(section->include_groups);
    darray_free(section->includes);
    darray_free(section->buffer);
}

ସ୍ଥିର bool
xkb_file_section_set_meta_data(ସଂରଚନା xkb_context *ctx,
                               ସଂରଚନା xkb_file_section *section,
                               ଅଚଳ XkbFile *xkb_file)
{
    section->file_type = xkb_file->file_type;
    section->flags = xkb_file->flags;
    ଯଦି (xkb_file->name) {
        darray_size_t idx = darray_size(section->buffer);
        darray_append_string0(section->buffer, xkb_file->name);
        section->name = idx;
    } ଅନ୍ୟଥା {
        section->name = 0;
    }
    ଫେରନ୍ତୁ true;
}

/** Process a list of include statements */
ସ୍ଥିର bool
xkb_file_section_append_includes(ସଂରଚନା xkb_context *ctx,
                                 ଗଣନା xkb_file_iterator_flags flags,
                                 ଅଚଳ ସଂରଚନା parser_keymap_config *config,
                                 ଅଚଳ ଅକ୍ଷର *section_path,
                                 ସଂରଚନା xkb_file_section *section,
                                 ଗଣନା xkb_file_type file_type,
                                 IncludeStmt * include)
{
    ସଂରଚନା xkb_file_include_group *group = ଖାଲି;
    /* Note: this will flatten statements such as `include "pc+de"` */
    ପାଇଁ (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        ଅକ୍ଷର buf[PATH_MAX];
        /* Parse the included file to check the include validity */
        XkbFile *xkb_file = ProcessIncludeFile(ctx, config, stmt, file_type,
                                               buf, ଆକାର(buf));
        ଅଚଳ bool valid = (xkb_file != ଖାଲି);
        ଯଦି (valid || !(flags & XKB_FILE_ITERATOR_FAIL_ON_INCLUDE_ERROR)) {
            /* Collect the strings of the statement properties */

            ଅଚଳ darray_size_t path = darray_size(section->buffer);
            darray_append_string0(section->buffer, buf);

            ଅଚଳ darray_size_t file = darray_size(section->buffer);
            darray_append_string0(section->buffer, stmt->file);

            ଅଚଳ darray_size_t section_name =
                (stmt->map != ଖାଲି || (valid && xkb_file->name))
                    ? darray_size(section->buffer)
                    : 0;
            ଯଦି (section_name) {
                darray_append_string0(section->buffer,
                                      (stmt->map) ? stmt->map : xkb_file->name);
            }

            ଅଚଳ darray_size_t modifier = (stmt->modifier)
                ? darray_size(section->buffer)
                : 0;
            ଯଦି (modifier) {
                darray_append_string0(section->buffer, stmt->modifier);
            }

            ଅଚଳ ଗଣନା xkb_map_flags section_flags = (valid)
                ? xkb_file->flags
                : 0;

            /* Create and append the include statement */
            ଅଚଳ ସଂରଚନା xkb_file_include inc = {
                .valid = valid,
                .explicit_section = (stmt->map != ଖାଲି),
                .merge = stmt->merge,
                .path = path,
                .file = file,
                .section = section_name,
                .modifier = modifier,
                .flags = section_flags
            };
            ଅଚଳ darray_size_t idx = darray_size(section->includes);
            darray_append(section->includes, inc);

            /* Update include group */
            ଯଦି (group == ଖାଲି) {
                ଅଚଳ darray_size_t group_idx =
                    darray_size(section->include_groups);
                darray_append(
                    section->include_groups,
                    (ସଂରଚନା xkb_file_include_group) {.start = idx, .end = idx}
                );
                group = &darray_item(section->include_groups, group_idx);
            } ଅନ୍ୟଥା {
                group->end = idx;
            }

            FreeXkbFile(xkb_file);
        } ଅନ୍ୟଥା {
            ଅଚଳ ଅକ୍ଷର * ଅଚଳ name =
                xkb_file_section_get_string(section, section->name);
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "%s include failure in: %s%s%s%s\n",
                    xkb_file_type_name(file_type), section_path,
                    (section->name ? " (section: \"": ""), name,
                    (section->name ? "\")": ""));
            FreeXkbFile(xkb_file);
            ଫେରନ୍ତୁ false;
        }
    };
    ଫେରନ୍ତୁ true;
}

/* Process the AST of a section */
ସ୍ଥିର bool
xkb_file_section_process(ସଂରଚନା xkb_context *ctx,
                         ଗଣନା xkb_file_iterator_flags flags,
                         ଅଚଳ ସଂରଚନା parser_keymap_config *config,
                         ଅଚଳ ଅକ୍ଷର *path,
                         ସଂରଚନା xkb_file_section *section,
                         ଅଚଳ XkbFile *xkb_file)
{
    bool ok = true;
    ପାଇଁ (ParseCommon *stmt = xkb_file->defs; stmt; stmt = stmt->next) {
        ଯଦି (stmt->type == STMT_INCLUDE) {
            ok = xkb_file_section_append_includes(ctx, flags, config, path,
                                                  section, xkb_file->file_type,
                                                  (IncludeStmt *) stmt);
            ଯଦି (!ok)
                ଭାଙ୍ଗ;
        }
    }
    ଫେରନ୍ତୁ ok;
}

bool
xkb_file_section_parse(ସଂରଚନା xkb_context *ctx,
                       ଗଣନା xkb_file_iterator_flags iterator_flags,
                       ଗଣନା xkb_keymap_format format,
                       ଗଣନା xkb_keymap_compile_flags compile_flags,
                       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_depth,
                       ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *map,
                       ସଂରଚନା xkb_file_section *section)
{
    ଯଦି (ExceedsIncludeMaxDepth(ctx, include_depth))
        ଫେରନ୍ତୁ false;

    FILE *file = fopen(path, "rb");
    ଯଦି (!file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot open file: %s\n", path);
        ଫେରନ୍ତୁ false;
    }

    ଅଚଳ ସଂରଚନା parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    XkbFile *xkb_file = XkbParseFile(ctx, &config, file, path, map);
    fclose(file);
    ଯଦି (!xkb_file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot parse map \"%s\" in file: %s\n",
                (map ? map : "(no map)"), path);
        ଫେରନ୍ତୁ false;
    }

    xkb_file_section_reset(section);
    ଅଚଳ bool no_includes = iterator_flags & XKB_FILE_ITERATOR_NO_INCLUDES;
    ଅଚଳ bool ok = (
        xkb_file_section_set_meta_data(ctx, section, xkb_file) &&
        (no_includes ||
         xkb_file_section_process(ctx, iterator_flags, &config, path, section,
                                  xkb_file))
    );
    FreeXkbFile(xkb_file);
    ଫେରନ୍ତୁ ok;
}

ସଂରଚନା xkb_file_iterator *
xkb_file_iterator_new_from_buffer(ସଂରଚନା xkb_context *ctx,
                                  ଗଣନା xkb_file_iterator_flags iterator_flags,
                                  ଗଣନା xkb_keymap_format format,
                                  ଗଣନା xkb_keymap_compile_flags compile_flags,
                                  ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *map,
                                  ଗଣନା xkb_file_type file_type,
                                  ଅଚଳ ଅକ୍ଷର *string, size_t length)
{
    ସଂରଚନା xkb_file_iterator * ଅଚଳ iter = calloc(1, ଆକାର(*iter));
    ଯଦି (!iter) {
        log_err(ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate file iterator\n");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    iter->flags = iterator_flags;
    iter->ctx = ctx;
    iter->path = path;
    iter->map = map;
    iter->type = file_type;
    iter->parser_config = (ସଂରଚନା parser_keymap_config) {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };
    xkb_file_section_init(&iter->section);

    ଯଦି (!XkbParseStringInit(ctx, &iter->scanner, string, length, path, ଖାଲି)) {
        xkb_file_iterator_free(iter);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଫେରନ୍ତୁ iter;
}

ଶୂନ୍ୟ
xkb_file_iterator_free(ସଂରଚନା xkb_file_iterator *iter)
{
    ଯଦି (!iter)
        ଫେରନ୍ତୁ;

    xkb_file_section_free(&iter->section);
    FreeXkbFile(iter->pending_xkb_file);
    ମୁକ୍ତ(iter);
}

bool
xkb_file_iterator_next(ସଂରଚନା xkb_file_iterator *iter,
                       ଅଚଳ ସଂରଚନା xkb_file_section **section)
{
    ଯଦି (iter->finished) {
        *section = ଖାଲି;
        ଫେରନ୍ତୁ true;
    }

next:
    { /* C11 compatibility: label cannot be followed by a declaration */ }
    /* Parse next section in the file */
    XkbFile *xkb_file = ଖାଲି;

    ଯଦି (iter->pending_xkb_file) {
        /* We are parsing the components of a keymap */
        ଯଦି (iter->pending_section) {
            /* Parse next component */
            xkb_file = iter->pending_section;
            ଯାଅ parse_components;
        } ଅନ୍ୟଥା {
            /* No more components, try next keymap */
            FreeXkbFile(iter->pending_xkb_file);
            iter->pending_xkb_file = ଖାଲି;
        }
    }

    ଯଦି (!XkbParseStringNext(iter->ctx, &iter->parser_config, &iter->scanner,
                            iter->map, &xkb_file)) {
        log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Error while parsing section in file: %s\n", iter->path);
        ଯାଅ error;
    } ଅନ୍ୟଥା ଯଦି (!xkb_file) {
        /* No more sections */
        iter->finished = true;
        *section = ଖାଲି;
        ଫେରନ୍ତୁ true;
    }

parse_components:
    /* Reset section */
    xkb_file_section_reset(&iter->section);

    /* Meta data */
    ଯଦି (!xkb_file_section_set_meta_data(iter->ctx, &iter->section, xkb_file))
        ଯାଅ error;

    /* Return current section */
    *section = &iter->section;

    ଯଦି (xkb_file->file_type == FILE_TYPE_KEYMAP) {
        /*
         * If it’s a keymap, then stop here.
         * Next iteration will process its components.
         */
        iter->pending_xkb_file = xkb_file;
        iter->pending_section = (XkbFile *) xkb_file->defs;
        iter->map = ଖାଲି;
        ଫେରନ୍ତୁ true;
    } ଅନ୍ୟଥା {
        ଯଦି (iter->type != FILE_TYPE_INVALID &&
            xkb_file->file_type != iter->type) {
            ଯଦି (iter->pending_xkb_file) {
                /* Within a keymap: filter out component */
                iter->pending_section = (XkbFile *) xkb_file->common.next;
                ଯାଅ next;
            } ଅନ୍ୟଥା {
                /* Component-specific file: type mismatch */
                log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File type mismatch: %s, section: %s\n",
                        iter->path,
                        (xkb_file->name ? xkb_file->name : "(no name)"));
                ଯାଅ error;
            }
        }
        ଯଦି (iter->map) {
            iter->finished = true;
        }
    }

    /* Collect include statements of current section */
    ଅଚଳ bool process_includes = !(iter->flags & XKB_FILE_ITERATOR_NO_INCLUDES);
    ଯଦି (process_includes &&
        !xkb_file_section_process(iter->ctx, iter->flags, &iter->parser_config,
                                  iter->path, &iter->section, xkb_file)) {
        ଯାଅ error;
    } ଅନ୍ୟଥା ଯଦି (iter->pending_section) {
        /* Next component */
        iter->pending_section = (XkbFile *) xkb_file->common.next;
    } ଅନ୍ୟଥା {
        /* No more component, free the file containing the components */
        FreeXkbFile(xkb_file);
    }

    ଫେରନ୍ତୁ true;

error:
    FreeXkbFile(xkb_file);
    *section = ଖାଲି;
    ଫେରନ୍ତୁ false;
}

/* Lookup a string by its index */
ଅଚଳ ଅକ୍ଷର *
xkb_file_section_get_string(ଅଚଳ ସଂରଚନା xkb_file_section *section, darray_size_t idx)
{
    ଫେରନ୍ତୁ &darray_item(section->buffer, idx);
}
