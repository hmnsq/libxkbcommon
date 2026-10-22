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

ধ্রুবক অক্ষর *
xkb_file_type_name(গণনা xkb_file_type type)
{
    যদি (type > FILE_TYPE_KEYMAP)
        ফেরত "unknown";

    স্থির ধ্রুবক অক্ষর *xkb_file_type_strings[FILE_TYPE_অঙ্ক_ENTRIES] = {
        [FILE_TYPE_KEYCODES] = "keycodes",
        [FILE_TYPE_TYPES] = "types",
        [FILE_TYPE_COMPAT] = "compatibility",
        [FILE_TYPE_SYMBOLS] = "symbols",
        [FILE_TYPE_GEOMETRY] = "geometry",
        [FILE_TYPE_KEYMAP] = "keymap",
    };
    ফেরত xkb_file_type_strings[type];
}

ধ্রুবক অক্ষর *
xkb_merge_mode_name(গণনা merge_mode merge)
{
    যদি (merge >= MERGE_MODE_অঙ্ক_ENTRIES)
        ফেরত "unknown";

    স্থির ধ্রুবক অক্ষর *merge_mode_strings[MERGE_MODE_অঙ্ক_ENTRIES] = {
        [MERGE_DEFAULT] = "default",
        [MERGE_AUGMENT] = "augment",
        [MERGE_OVERRIDE] = "override",
        [MERGE_REPLACE] = "replace",
    };
    ফেরত merge_mode_strings[merge];
}

/* Stateful lookup of map flags names */
ধ্রুবক অক্ষর *
xkb_map_flags_string_iter(unsigned পূর্ণসংখ্যা *সূচক, গণনা xkb_map_flags flags)
{
    যদি (!flags)
        ফেরত ফাঁকা;

    স্থির ধ্রুবক কাঠামো {
        গণনা xkb_map_flags flag;
        ধ্রুবক অক্ষর *name;
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

    যখন (*সূচক < ARRAY_আকার(names)) {
        যদি (flags & names[*সূচক].flag)
            ফেরত names[(*সূচক)++].name;
        (*সূচক)++;
    }

    ফেরত ফাঁকা;
};

FILE *
xkb_resolve_file(কাঠামো xkb_context *ctx,
                 গণনা xkb_file_type file_type,
                 ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর *map,
                 অক্ষর *resolved_path, আকার_t resolved_path_আকার,
                 অক্ষর *resolved_map, আকার_t resolved_map_আকার)
{
    unsigned পূর্ণসংখ্যা offset = 0;
    FILE *file = ফাঁকা; /* Exact match */
    FILE *candidate = ফাঁকা; /* Weak match */
    ধ্রুবক আকার_t path_len = strlen(path);

    ধ্রুবক bool absolute_path = is_absolute_path(path);
    যদি (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        file = fopen(path, "rb");
    } অন্যথা {
        /* Relative path: lookup the first XKB path */
        file = FindFileInXkbPath(ctx, "(unknown)", path, path_len, file_type,
                                 resolved_path, resolved_path_আকার, &offset,
                                 true);
    }

    ধ্রুবক গণনা xkb_keymap_format format = DEFAULT_প্রবেশ_KEYMAP_FORMAT;
    ধ্রুবক কাঠামো parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    যখন (file) {
        XkbFile * ধ্রুবক xkb_file = XkbParseFile(ctx, &config, file, path, map);
        যদি (xkb_file) {
            যদি (file_type < FILE_TYPE_অঙ্ক_ENTRIES &&
                xkb_file->file_type != file_type) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File of wrong type (expected %s, got %s); "
                        "file \"%s\" ignored\n",
                        xkb_file_type_to_string(file_type),
                        xkb_file_type_to_string(xkb_file->file_type),
                        (absolute_path ? path : resolved_path));
                যাও invalid_file;
            } অন্যথা যদি (map || (xkb_file->flags && MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops in this iteration.
                 */
                যদি (!strcpy_safe(resolved_map, resolved_map_আকার,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    যাও error;
                }
            } অন্যথা যদি (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = file;
                যদি (!strcpy_safe(resolved_map, resolved_map_আকার,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    যাও error;
                }
            } অন্যথা {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                যাও invalid_file;
            }
        } অন্যথা {
invalid_file:
            fclose(file);
            file = ফাঁকা;
        }

        FreeXkbFile(xkb_file);

        যদি (file != ফাঁকা || absolute_path) {
            /* Exact match or absolute path with no further file to search */
            বিরতি;
        }

        offset++;
        file = FindFileInXkbPath(ctx, "(unknown)",
                                 path, path_len, file_type,
                                 resolved_path, resolved_path_আকার,
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
        !strcpy_safe(resolved_path, resolved_path_আকার, path)) {
        যাও error;
    }

    ফেরত file;

error:
    log_err(ctx, XKB_ERROR_INSUFFICIENT_বাফার_আকার,
            "Cannot copy resolved path or section\n");
    fclose(file);
    ফেরত ফাঁকা;
}

শূন্য
xkb_file_section_init(কাঠামো xkb_file_section *section)
{
    darray_init(section->include_groups);
    darray_init(section->includes);
    darray_init(section->বাফার);
    darray_append(section->বাফার, '\0');
}

স্থির শূন্য
xkb_file_section_reset(কাঠামো xkb_file_section *section)
{
    darray_আকার(section->include_groups) = 0;
    darray_আকার(section->includes) = 0;
    darray_আকার(section->বাফার) = 1; /* keep the dummy string */
}

শূন্য
xkb_file_section_free(কাঠামো xkb_file_section *section)
{
    যদি (!section)
        ফেরত;

    darray_free(section->include_groups);
    darray_free(section->includes);
    darray_free(section->বাফার);
}

স্থির bool
xkb_file_section_set_meta_তথ্য(কাঠামো xkb_context *ctx,
                               কাঠামো xkb_file_section *section,
                               ধ্রুবক XkbFile *xkb_file)
{
    section->file_type = xkb_file->file_type;
    section->flags = xkb_file->flags;
    যদি (xkb_file->name) {
        darray_আকার_t idx = darray_আকার(section->বাফার);
        darray_append_string0(section->বাফার, xkb_file->name);
        section->name = idx;
    } অন্যথা {
        section->name = 0;
    }
    ফেরত true;
}

/** Process a list of include statements */
স্থির bool
xkb_file_section_append_includes(কাঠামো xkb_context *ctx,
                                 গণনা xkb_file_iterator_flags flags,
                                 ধ্রুবক কাঠামো parser_keymap_config *config,
                                 ধ্রুবক অক্ষর *section_path,
                                 কাঠামো xkb_file_section *section,
                                 গণনা xkb_file_type file_type,
                                 IncludeStmt * include)
{
    কাঠামো xkb_file_include_group *group = ফাঁকা;
    /* Note: this will flatten statements such as `include "pc+de"` */
    জন্য (IncludeStmt *stmt = include; stmt; stmt = stmt->পরবর্তী_incl) {
        অক্ষর buf[PATH_MAX];
        /* Parse the included file to check the include validity */
        XkbFile *xkb_file = ProcessIncludeFile(ctx, config, stmt, file_type,
                                               buf, আকার(buf));
        ধ্রুবক bool valid = (xkb_file != ফাঁকা);
        যদি (valid || !(flags & XKB_FILE_ITERATOR_FAIL_ON_INCLUDE_ERROR)) {
            /* Collect the strings of the statement properties */

            ধ্রুবক darray_আকার_t path = darray_আকার(section->বাফার);
            darray_append_string0(section->বাফার, buf);

            ধ্রুবক darray_আকার_t file = darray_আকার(section->বাফার);
            darray_append_string0(section->বাফার, stmt->file);

            ধ্রুবক darray_আকার_t section_name =
                (stmt->map != ফাঁকা || (valid && xkb_file->name))
                    ? darray_আকার(section->বাফার)
                    : 0;
            যদি (section_name) {
                darray_append_string0(section->বাফার,
                                      (stmt->map) ? stmt->map : xkb_file->name);
            }

            ধ্রুবক darray_আকার_t modifier = (stmt->modifier)
                ? darray_আকার(section->বাফার)
                : 0;
            যদি (modifier) {
                darray_append_string0(section->বাফার, stmt->modifier);
            }

            ধ্রুবক গণনা xkb_map_flags section_flags = (valid)
                ? xkb_file->flags
                : 0;

            /* Create and append the include statement */
            ধ্রুবক কাঠামো xkb_file_include inc = {
                .valid = valid,
                .explicit_section = (stmt->map != ফাঁকা),
                .merge = stmt->merge,
                .path = path,
                .file = file,
                .section = section_name,
                .modifier = modifier,
                .flags = section_flags
            };
            ধ্রুবক darray_আকার_t idx = darray_আকার(section->includes);
            darray_append(section->includes, inc);

            /* Update include group */
            যদি (group == ফাঁকা) {
                ধ্রুবক darray_আকার_t group_idx =
                    darray_আকার(section->include_groups);
                darray_append(
                    section->include_groups,
                    (কাঠামো xkb_file_include_group) {.start = idx, .end = idx}
                );
                group = &darray_item(section->include_groups, group_idx);
            } অন্যথা {
                group->end = idx;
            }

            FreeXkbFile(xkb_file);
        } অন্যথা {
            ধ্রুবক অক্ষর * ধ্রুবক name =
                xkb_file_section_get_string(section, section->name);
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "%s include failure in: %s%s%s%s\n",
                    xkb_file_type_name(file_type), section_path,
                    (section->name ? " (section: \"": ""), name,
                    (section->name ? "\")": ""));
            FreeXkbFile(xkb_file);
            ফেরত false;
        }
    };
    ফেরত true;
}

/* Process the AST of a section */
স্থির bool
xkb_file_section_process(কাঠামো xkb_context *ctx,
                         গণনা xkb_file_iterator_flags flags,
                         ধ্রুবক কাঠামো parser_keymap_config *config,
                         ধ্রুবক অক্ষর *path,
                         কাঠামো xkb_file_section *section,
                         ধ্রুবক XkbFile *xkb_file)
{
    bool ok = true;
    জন্য (ParseCommon *stmt = xkb_file->defs; stmt; stmt = stmt->পরবর্তী) {
        যদি (stmt->type == STMT_INCLUDE) {
            ok = xkb_file_section_append_includes(ctx, flags, config, path,
                                                  section, xkb_file->file_type,
                                                  (IncludeStmt *) stmt);
            যদি (!ok)
                বিরতি;
        }
    }
    ফেরত ok;
}

bool
xkb_file_section_parse(কাঠামো xkb_context *ctx,
                       গণনা xkb_file_iterator_flags iterator_flags,
                       গণনা xkb_keymap_format format,
                       গণনা xkb_keymap_compile_flags compile_flags,
                       unsigned পূর্ণসংখ্যা include_depth,
                       ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর *map,
                       কাঠামো xkb_file_section *section)
{
    যদি (ExceedsIncludeMaxDepth(ctx, include_depth))
        ফেরত false;

    FILE *file = fopen(path, "rb");
    যদি (!file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot open file: %s\n", path);
        ফেরত false;
    }

    ধ্রুবক কাঠামো parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    XkbFile *xkb_file = XkbParseFile(ctx, &config, file, path, map);
    fclose(file);
    যদি (!xkb_file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot parse map \"%s\" in file: %s\n",
                (map ? map : "(no map)"), path);
        ফেরত false;
    }

    xkb_file_section_reset(section);
    ধ্রুবক bool no_includes = iterator_flags & XKB_FILE_ITERATOR_NO_INCLUDES;
    ধ্রুবক bool ok = (
        xkb_file_section_set_meta_তথ্য(ctx, section, xkb_file) &&
        (no_includes ||
         xkb_file_section_process(ctx, iterator_flags, &config, path, section,
                                  xkb_file))
    );
    FreeXkbFile(xkb_file);
    ফেরত ok;
}

কাঠামো xkb_file_iterator *
xkb_file_iterator_new_from_বাফার(কাঠামো xkb_context *ctx,
                                  গণনা xkb_file_iterator_flags iterator_flags,
                                  গণনা xkb_keymap_format format,
                                  গণনা xkb_keymap_compile_flags compile_flags,
                                  ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর *map,
                                  গণনা xkb_file_type file_type,
                                  ধ্রুবক অক্ষর *string, আকার_t দৈর্ঘ্য)
{
    কাঠামো xkb_file_iterator * ধ্রুবক iter = calloc(1, আকার(*iter));
    যদি (!iter) {
        log_err(ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate file iterator\n");
        ফেরত ফাঁকা;
    }

    iter->flags = iterator_flags;
    iter->ctx = ctx;
    iter->path = path;
    iter->map = map;
    iter->type = file_type;
    iter->parser_config = (কাঠামো parser_keymap_config) {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };
    xkb_file_section_init(&iter->section);

    যদি (!XkbParseStringInit(ctx, &iter->scanner, string, দৈর্ঘ্য, path, ফাঁকা)) {
        xkb_file_iterator_free(iter);
        ফেরত ফাঁকা;
    }

    ফেরত iter;
}

শূন্য
xkb_file_iterator_free(কাঠামো xkb_file_iterator *iter)
{
    যদি (!iter)
        ফেরত;

    xkb_file_section_free(&iter->section);
    FreeXkbFile(iter->pending_xkb_file);
    মুক্ত(iter);
}

bool
xkb_file_iterator_পরবর্তী(কাঠামো xkb_file_iterator *iter,
                       ধ্রুবক কাঠামো xkb_file_section **section)
{
    যদি (iter->finished) {
        *section = ফাঁকা;
        ফেরত true;
    }

পরবর্তী:
    { /* C11 compatibility: label cannot be followed by a declaration */ }
    /* Parse next section in the file */
    XkbFile *xkb_file = ফাঁকা;

    যদি (iter->pending_xkb_file) {
        /* We are parsing the components of a keymap */
        যদি (iter->pending_section) {
            /* Parse next component */
            xkb_file = iter->pending_section;
            যাও parse_components;
        } অন্যথা {
            /* No more components, try next keymap */
            FreeXkbFile(iter->pending_xkb_file);
            iter->pending_xkb_file = ফাঁকা;
        }
    }

    যদি (!Xkb_Parse_String_পরবর্তী(iter->ctx, &iter->parser_config, &iter->scanner,
                            iter->map, &xkb_file)) {
        log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Error while parsing section in file: %s\n", iter->path);
        যাও error;
    } অন্যথা যদি (!xkb_file) {
        /* No more sections */
        iter->finished = true;
        *section = ফাঁকা;
        ফেরত true;
    }

parse_components:
    /* Reset section */
    xkb_file_section_reset(&iter->section);

    /* Meta data */
    যদি (!xkb_file_section_set_meta_তথ্য(iter->ctx, &iter->section, xkb_file))
        যাও error;

    /* Return current section */
    *section = &iter->section;

    যদি (xkb_file->file_type == FILE_TYPE_KEYMAP) {
        /*
         * If it’s a keymap, then stop here.
         * Next iteration will process its components.
         */
        iter->pending_xkb_file = xkb_file;
        iter->pending_section = (XkbFile *) xkb_file->defs;
        iter->map = ফাঁকা;
        ফেরত true;
    } অন্যথা {
        যদি (iter->type != FILE_TYPE_INVALID &&
            xkb_file->file_type != iter->type) {
            যদি (iter->pending_xkb_file) {
                /* Within a keymap: filter out component */
                iter->pending_section = (XkbFile *) xkb_file->common.পরবর্তী;
                যাও পরবর্তী;
            } অন্যথা {
                /* Component-specific file: type mismatch */
                log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File type mismatch: %s, section: %s\n",
                        iter->path,
                        (xkb_file->name ? xkb_file->name : "(no name)"));
                যাও error;
            }
        }
        যদি (iter->map) {
            iter->finished = true;
        }
    }

    /* Collect include statements of current section */
    ধ্রুবক bool process_includes = !(iter->flags & XKB_FILE_ITERATOR_NO_INCLUDES);
    যদি (process_includes &&
        !xkb_file_section_process(iter->ctx, iter->flags, &iter->parser_config,
                                  iter->path, &iter->section, xkb_file)) {
        যাও error;
    } অন্যথা যদি (iter->pending_section) {
        /* Next component */
        iter->pending_section = (XkbFile *) xkb_file->common.পরবর্তী;
    } অন্যথা {
        /* No more component, free the file containing the components */
        FreeXkbFile(xkb_file);
    }

    ফেরত true;

error:
    FreeXkbFile(xkb_file);
    *section = ফাঁকা;
    ফেরত false;
}

/* Lookup a string by its index */
ধ্রুবক অক্ষর *
xkb_file_section_get_string(ধ্রুবক কাঠামো xkb_file_section *section, darray_আকার_t idx)
{
    ফেরত &darray_item(section->বাফার, idx);
}
