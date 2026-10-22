/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#include "config.h"

#include <limits.h>
#include <string.h>

#include "xkbcommon/xkbcommon.h"
#include "context.h"
#include "darray.h"
#include "include.h"
#include "keymap-formats.h"
#include "keymap-file-iterator.h"
#include "messages-codes.h"
#include "utils.h"
#include "util-paths.h"
#include "xkbcomp-priv.h"
#include "xkbcomp/ast.h"

const var_na *
xkb_file_type_name(ga_nanaa xkb_file_type type)
{
    නම් (type > FILE_TYPE_KEYMAP)
        ආපසු "unknown";

    static const var_na *xkb_file_type_strings[_FILE_TYPE_NUM_ENTRIES] = {
        [FILE_TYPE_KEYCODES] = "keycodes",
        [FILE_TYPE_TYPES] = "types",
        [FILE_TYPE_COMPAT] = "compatibility",
        [FILE_TYPE_SYMBOLS] = "symbols",
        [FILE_TYPE_GEOMETRY] = "geometry",
        [FILE_TYPE_KEYMAP] = "keymap",
    };
    ආපසු xkb_file_type_strings[type];
}

const var_na *
xkb_merge_mode_name(ga_nanaa merge_mode merge)
{
    නම් (merge >= _MERGE_MODE_NUM_ENTRIES)
        ආපසු "unknown";

    static const var_na *merge_mode_strings[_MERGE_MODE_NUM_ENTRIES] = {
        [MERGE_DEFAULT] = "default",
        [MERGE_AUGMENT] = "augment",
        [MERGE_OVERRIDE] = "override",
        [MERGE_REPLACE] = "replace",
    };
    ආපසු merge_mode_strings[merge];
}

/* Stateful lookup of map flags names */
const var_na *
xkb_map_flags_string_iter(unsigned පූර්ණ_සංඛ්‍යා *index, ga_nanaa xkb_map_flags flags)
{
    නම් (!flags)
        ආපසු NULL;

    static const sa_nracanaa {
        ga_nanaa xkb_map_flags flag;
        const var_na *name;
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

    අතරතුර (*index < ARRAY_SIZE(names)) {
        නම් (flags & names[*index].flag)
            ආපසු names[(*index)++].name;
        (*index)++;
    }

    ආපසු NULL;
};

FILE *
xkb_resolve_file(sa_nracanaa xkb_context *ctx,
                 ga_nanaa xkb_file_type file_type,
                 const var_na *path, const var_na *map,
                 var_na *resolved_path, size_t resolved_path_size,
                 var_na *resolved_map, size_t resolved_map_size)
{
    unsigned පූර්ණ_සංඛ්‍යා offset = 0;
    FILE *file = NULL; /* Exact match */
    FILE *candidate = NULL; /* Weak match */
    const size_t path_len = strlen(path);

    const bool absolute_path = is_absolute_path(path);
    නම් (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        file = fopen(path, "rb");
    } නැතහොත් {
        /* Relative path: lookup the first XKB path */
        file = FindFileInXkbPath(ctx, "(unknown)", path, path_len, file_type,
                                 resolved_path, resolved_path_size, &offset,
                                 true);
    }

    const ga_nanaa xkb_keymap_format format = DEFAULT_INPUT_KEYMAP_FORMAT;
    const sa_nracanaa parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    අතරතුර (file) {
        XkbFile * const xkb_file = XkbParseFile(ctx, &config, file, path, map);
        නම් (xkb_file) {
            නම් (file_type < _FILE_TYPE_NUM_ENTRIES &&
                xkb_file->file_type != file_type) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File of wrong type (expected %s, got %s); "
                        "file \"%s\" ignored\n",
                        xkb_file_type_to_string(file_type),
                        xkb_file_type_to_string(xkb_file->file_type),
                        (absolute_path ? path : resolved_path));
                goto invalid_file;
            } නැතහොත් නම් (map || (xkb_file->flags && MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops in this iteration.
                 */
                නම් (!strcpy_safe(resolved_map, resolved_map_size,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    goto error;
                }
            } නැතහොත් නම් (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = file;
                නම් (!strcpy_safe(resolved_map, resolved_map_size,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    goto error;
                }
            } නැතහොත් {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                goto invalid_file;
            }
        } නැතහොත් {
invalid_file:
            fclose(file);
            file = NULL;
        }

        FreeXkbFile(xkb_file);

        නම් (file != NULL || absolute_path) {
            /* Exact match or absolute path with no further file to search */
            bha_nga;
        }

        offset++;
        file = FindFileInXkbPath(ctx, "(unknown)",
                                 path, path_len, file_type,
                                 resolved_path, resolved_path_size,
                                 &offset, true);
    }

    නම් (!file) {
        /* No exact match: use weak match, if any */
        file = candidate;
    } නැතහොත් නම් (candidate) {
        /* Found exact match: discard weak match, if any */
        fclose(candidate);
    }

    නම් (absolute_path && file &&
        !strcpy_safe(resolved_path, resolved_path_size, path)) {
        goto error;
    }

    ආපසු file;

error:
    log_err(ctx, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
            "Cannot copy resolved path or section\n");
    fclose(file);
    ආපසු NULL;
}

void
xkb_file_section_init(sa_nracanaa xkb_file_section *section)
{
    darray_init(section->include_groups);
    darray_init(section->includes);
    darray_init(section->buffer);
    darray_append(section->buffer, '\0');
}

static void
xkb_file_section_reset(sa_nracanaa xkb_file_section *section)
{
    darray_size(section->include_groups) = 0;
    darray_size(section->includes) = 0;
    darray_size(section->buffer) = 1; /* keep the dummy string */
}

void
xkb_file_section_free(sa_nracanaa xkb_file_section *section)
{
    නම් (!section)
        ආපසු;

    darray_free(section->include_groups);
    darray_free(section->includes);
    darray_free(section->buffer);
}

static bool
xkb_file_section_set_meta_data(sa_nracanaa xkb_context *ctx,
                               sa_nracanaa xkb_file_section *section,
                               const XkbFile *xkb_file)
{
    section->file_type = xkb_file->file_type;
    section->flags = xkb_file->flags;
    නම් (xkb_file->name) {
        darray_size_t idx = darray_size(section->buffer);
        darray_append_string0(section->buffer, xkb_file->name);
        section->name = idx;
    } නැතහොත් {
        section->name = 0;
    }
    ආපසු true;
}

/** Process a list of include statements */
static bool
xkb_file_section_append_includes(sa_nracanaa xkb_context *ctx,
                                 ga_nanaa xkb_file_iterator_flags flags,
                                 const sa_nracanaa parser_keymap_config *config,
                                 const var_na *section_path,
                                 sa_nracanaa xkb_file_section *section,
                                 ga_nanaa xkb_file_type file_type,
                                 IncludeStmt * include)
{
    sa_nracanaa xkb_file_include_group *group = NULL;
    /* Note: this will flatten statements such as `include "pc+de"` */
    සඳහා (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        var_na buf[PATH_MAX];
        /* Parse the included file to check the include validity */
        XkbFile *xkb_file = ProcessIncludeFile(ctx, config, stmt, file_type,
                                               buf, parimaa_na(buf));
        const bool valid = (xkb_file != NULL);
        නම් (valid || !(flags & XKB_FILE_ITERATOR_FAIL_ON_INCLUDE_ERROR)) {
            /* Collect the strings of the statement properties */

            const darray_size_t path = darray_size(section->buffer);
            darray_append_string0(section->buffer, buf);

            const darray_size_t file = darray_size(section->buffer);
            darray_append_string0(section->buffer, stmt->file);

            const darray_size_t section_name =
                (stmt->map != NULL || (valid && xkb_file->name))
                    ? darray_size(section->buffer)
                    : 0;
            නම් (section_name) {
                darray_append_string0(section->buffer,
                                      (stmt->map) ? stmt->map : xkb_file->name);
            }

            const darray_size_t modifier = (stmt->modifier)
                ? darray_size(section->buffer)
                : 0;
            නම් (modifier) {
                darray_append_string0(section->buffer, stmt->modifier);
            }

            const ga_nanaa xkb_map_flags section_flags = (valid)
                ? xkb_file->flags
                : 0;

            /* Create and append the include statement */
            const sa_nracanaa xkb_file_include inc = {
                .valid = valid,
                .explicit_section = (stmt->map != NULL),
                .merge = stmt->merge,
                .path = path,
                .file = file,
                .section = section_name,
                .modifier = modifier,
                .flags = section_flags
            };
            const darray_size_t idx = darray_size(section->includes);
            darray_append(section->includes, inc);

            /* Update include group */
            නම් (group == NULL) {
                const darray_size_t group_idx =
                    darray_size(section->include_groups);
                darray_append(
                    section->include_groups,
                    (sa_nracanaa xkb_file_include_group) {.start = idx, .end = idx}
                );
                group = &darray_item(section->include_groups, group_idx);
            } නැතහොත් {
                group->end = idx;
            }

            FreeXkbFile(xkb_file);
        } නැතහොත් {
            const var_na * const name =
                xkb_file_section_get_string(section, section->name);
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "%s include failure in: %s%s%s%s\n",
                    xkb_file_type_name(file_type), section_path,
                    (section->name ? " (section: \"": ""), name,
                    (section->name ? "\")": ""));
            FreeXkbFile(xkb_file);
            ආපසු false;
        }
    };
    ආපසු true;
}

/* Process the AST of a section */
static bool
xkb_file_section_process(sa_nracanaa xkb_context *ctx,
                         ga_nanaa xkb_file_iterator_flags flags,
                         const sa_nracanaa parser_keymap_config *config,
                         const var_na *path,
                         sa_nracanaa xkb_file_section *section,
                         const XkbFile *xkb_file)
{
    bool ok = true;
    සඳහා (ParseCommon *stmt = xkb_file->defs; stmt; stmt = stmt->next) {
        නම් (stmt->type == STMT_INCLUDE) {
            ok = xkb_file_section_append_includes(ctx, flags, config, path,
                                                  section, xkb_file->file_type,
                                                  (IncludeStmt *) stmt);
            නම් (!ok)
                bha_nga;
        }
    }
    ආපසු ok;
}

bool
xkb_file_section_parse(sa_nracanaa xkb_context *ctx,
                       ga_nanaa xkb_file_iterator_flags iterator_flags,
                       ga_nanaa xkb_keymap_format format,
                       ga_nanaa xkb_keymap_compile_flags compile_flags,
                       unsigned පූර්ණ_සංඛ්‍යා include_depth,
                       const var_na *path, const var_na *map,
                       sa_nracanaa xkb_file_section *section)
{
    නම් (ExceedsIncludeMaxDepth(ctx, include_depth))
        ආපසු false;

    FILE *file = fopen(path, "rb");
    නම් (!file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot open file: %s\n", path);
        ආපසු false;
    }

    const sa_nracanaa parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    XkbFile *xkb_file = XkbParseFile(ctx, &config, file, path, map);
    fclose(file);
    නම් (!xkb_file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot parse map \"%s\" in file: %s\n",
                (map ? map : "(no map)"), path);
        ආපසු false;
    }

    xkb_file_section_reset(section);
    const bool no_includes = iterator_flags & XKB_FILE_ITERATOR_NO_INCLUDES;
    const bool ok = (
        xkb_file_section_set_meta_data(ctx, section, xkb_file) &&
        (no_includes ||
         xkb_file_section_process(ctx, iterator_flags, &config, path, section,
                                  xkb_file))
    );
    FreeXkbFile(xkb_file);
    ආපසු ok;
}

sa_nracanaa xkb_file_iterator *
xkb_file_iterator_new_from_buffer(sa_nracanaa xkb_context *ctx,
                                  ga_nanaa xkb_file_iterator_flags iterator_flags,
                                  ga_nanaa xkb_keymap_format format,
                                  ga_nanaa xkb_keymap_compile_flags compile_flags,
                                  const var_na *path, const var_na *map,
                                  ga_nanaa xkb_file_type file_type,
                                  const var_na *string, size_t length)
{
    sa_nracanaa xkb_file_iterator * const iter = calloc(1, parimaa_na(*iter));
    නම් (!iter) {
        log_err(ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate file iterator\n");
        ආපසු NULL;
    }

    iter->flags = iterator_flags;
    iter->ctx = ctx;
    iter->path = path;
    iter->map = map;
    iter->type = file_type;
    iter->parser_config = (sa_nracanaa parser_keymap_config) {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };
    xkb_file_section_init(&iter->section);

    නම් (!XkbParseStringInit(ctx, &iter->scanner, string, length, path, NULL)) {
        xkb_file_iterator_free(iter);
        ආපසු NULL;
    }

    ආපසු iter;
}

void
xkb_file_iterator_free(sa_nracanaa xkb_file_iterator *iter)
{
    නම් (!iter)
        ආපසු;

    xkb_file_section_free(&iter->section);
    FreeXkbFile(iter->pending_xkb_file);
    free(iter);
}

bool
xkb_file_iterator_next(sa_nracanaa xkb_file_iterator *iter,
                       const sa_nracanaa xkb_file_section **section)
{
    නම් (iter->finished) {
        *section = NULL;
        ආපසු true;
    }

next:
    { /* C11 compatibility: label cannot be followed by a declaration */ }
    /* Parse next section in the file */
    XkbFile *xkb_file = NULL;

    නම් (iter->pending_xkb_file) {
        /* We are parsing the components of a keymap */
        නම් (iter->pending_section) {
            /* Parse next component */
            xkb_file = iter->pending_section;
            goto parse_components;
        } නැතහොත් {
            /* No more components, try next keymap */
            FreeXkbFile(iter->pending_xkb_file);
            iter->pending_xkb_file = NULL;
        }
    }

    නම් (!XkbParseStringNext(iter->ctx, &iter->parser_config, &iter->scanner,
                            iter->map, &xkb_file)) {
        log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Error while parsing section in file: %s\n", iter->path);
        goto error;
    } නැතහොත් නම් (!xkb_file) {
        /* No more sections */
        iter->finished = true;
        *section = NULL;
        ආපසු true;
    }

parse_components:
    /* Reset section */
    xkb_file_section_reset(&iter->section);

    /* Meta data */
    නම් (!xkb_file_section_set_meta_data(iter->ctx, &iter->section, xkb_file))
        goto error;

    /* Return current section */
    *section = &iter->section;

    නම් (xkb_file->file_type == FILE_TYPE_KEYMAP) {
        /*
         * If it’s a keymap, then stop here.
         * Next iteration will process its components.
         */
        iter->pending_xkb_file = xkb_file;
        iter->pending_section = (XkbFile *) xkb_file->defs;
        iter->map = NULL;
        ආපසු true;
    } නැතහොත් {
        නම් (iter->type != FILE_TYPE_INVALID &&
            xkb_file->file_type != iter->type) {
            නම් (iter->pending_xkb_file) {
                /* Within a keymap: filter out component */
                iter->pending_section = (XkbFile *) xkb_file->common.next;
                goto next;
            } නැතහොත් {
                /* Component-specific file: type mismatch */
                log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File type mismatch: %s, section: %s\n",
                        iter->path,
                        (xkb_file->name ? xkb_file->name : "(no name)"));
                goto error;
            }
        }
        නම් (iter->map) {
            iter->finished = true;
        }
    }

    /* Collect include statements of current section */
    const bool process_includes = !(iter->flags & XKB_FILE_ITERATOR_NO_INCLUDES);
    නම් (process_includes &&
        !xkb_file_section_process(iter->ctx, iter->flags, &iter->parser_config,
                                  iter->path, &iter->section, xkb_file)) {
        goto error;
    } නැතහොත් නම් (iter->pending_section) {
        /* Next component */
        iter->pending_section = (XkbFile *) xkb_file->common.next;
    } නැතහොත් {
        /* No more component, free the file containing the components */
        FreeXkbFile(xkb_file);
    }

    ආපසු true;

error:
    FreeXkbFile(xkb_file);
    *section = NULL;
    ආපසු false;
}

/* Lookup a string by its index */
const var_na *
xkb_file_section_get_string(const sa_nracanaa xkb_file_section *section, darray_size_t idx)
{
    ආපසු &darray_item(section->buffer, idx);
}
