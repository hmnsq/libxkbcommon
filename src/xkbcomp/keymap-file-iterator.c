/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#समावेश "config.h"

#समावेश <limits.h>
#समावेश <string.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "context.h"
#समावेश "darray.h"
#समावेश "include.h"
#समावेश "keymap-formats.h"
#समावेश "keymap-file-iterator.h"
#समावेश "messages-codes.h"
#समावेश "utils.h"
#समावेश "util-paths.h"
#समावेश "xkbcomp-priv.h"
#समावेश "xkbcomp/ast.h"

स्थिर अक्षर *
xkb_file_type_name(क्रमागत xkb_file_type type)
{
    अगर (type > FILE_TYPE_KEYMAP)
        वापस "unknown";

    अटल स्थिर अक्षर *xkb_file_type_strings[FILE_TYPE_अंक_ENTRIES] = {
        [FILE_TYPE_KEYCODES] = "keycodes",
        [FILE_TYPE_TYPES] = "types",
        [FILE_TYPE_COMPAT] = "compatibility",
        [FILE_TYPE_SYMBOLS] = "symbols",
        [FILE_TYPE_GEOMETRY] = "geometry",
        [FILE_TYPE_KEYMAP] = "keymap",
    };
    वापस xkb_file_type_strings[type];
}

स्थिर अक्षर *
xkb_merge_mode_name(क्रमागत merge_mode merge)
{
    अगर (merge >= MERGE_MODE_अंक_ENTRIES)
        वापस "unknown";

    अटल स्थिर अक्षर *merge_mode_strings[MERGE_MODE_अंक_ENTRIES] = {
        [MERGE_DEFAULT] = "default",
        [MERGE_AUGMENT] = "augment",
        [MERGE_OVERRIDE] = "override",
        [MERGE_REPLACE] = "replace",
    };
    वापस merge_mode_strings[merge];
}

/* Stateful lookup of map flags names */
स्थिर अक्षर *
xkb_map_flags_string_iter(अचिन्हित पूर्णांक *सूचकांक, क्रमागत xkb_map_flags flags)
{
    अगर (!flags)
        वापस शून्य;

    अटल स्थिर काष्ठा {
        क्रमागत xkb_map_flags flag;
        स्थिर अक्षर *name;
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

    जबतक (*सूचकांक < ARRAY_आकार(names)) {
        अगर (flags & names[*सूचकांक].flag)
            वापस names[(*सूचकांक)++].name;
        (*सूचकांक)++;
    }

    वापस शून्य;
};

खाता *
xkb_resolve_file(काष्ठा xkb_context *ctx,
                 क्रमागत xkb_file_type file_type,
                 स्थिर अक्षर *path, स्थिर अक्षर *map,
                 अक्षर *resolved_path, माप_प्रकार resolved_path_आकार,
                 अक्षर *resolved_map, माप_प्रकार resolved_map_आकार)
{
    अचिन्हित पूर्णांक offset = 0;
    खाता *file = शून्य; /* Exact match */
    खाता *candidate = शून्य; /* Weak match */
    स्थिर माप_प्रकार path_len = म_माप(path);

    स्थिर bool absolute_path = is_absolute_path(path);
    अगर (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        file = ख_खोलो(path, "rb");
    } अन्यथा {
        /* Relative path: lookup the first XKB path */
        file = FindFileInXkbPath(ctx, "(unknown)", path, path_len, file_type,
                                 resolved_path, resolved_path_आकार, &offset,
                                 true);
    }

    स्थिर क्रमागत xkb_keymap_format format = DEFAULT_निवेश_KEYMAP_FORMAT;
    स्थिर काष्ठा parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    जबतक (file) {
        XkbFile * स्थिर xkb_file = XkbParseFile(ctx, &config, file, path, map);
        अगर (xkb_file) {
            अगर (file_type < FILE_TYPE_अंक_ENTRIES &&
                xkb_file->file_type != file_type) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File of wrong type (expected %s, got %s); "
                        "file \"%s\" ignored\n",
                        xkb_file_type_to_string(file_type),
                        xkb_file_type_to_string(xkb_file->file_type),
                        (absolute_path ? path : resolved_path));
                जाओ invalid_file;
            } अन्यथा अगर (map || (xkb_file->flags && MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops in this iteration.
                 */
                अगर (!strcpy_safe(resolved_map, resolved_map_आकार,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    जाओ error;
                }
            } अन्यथा अगर (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = file;
                अगर (!strcpy_safe(resolved_map, resolved_map_आकार,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    जाओ error;
                }
            } अन्यथा {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                जाओ invalid_file;
            }
        } अन्यथा {
invalid_file:
            ख_बंद(file);
            file = शून्य;
        }

        FreeXkbFile(xkb_file);

        अगर (file != शून्य || absolute_path) {
            /* Exact match or absolute path with no further file to search */
            अवरोध;
        }

        offset++;
        file = FindFileInXkbPath(ctx, "(unknown)",
                                 path, path_len, file_type,
                                 resolved_path, resolved_path_आकार,
                                 &offset, true);
    }

    अगर (!file) {
        /* No exact match: use weak match, if any */
        file = candidate;
    } अन्यथा अगर (candidate) {
        /* Found exact match: discard weak match, if any */
        ख_बंद(candidate);
    }

    अगर (absolute_path && file &&
        !strcpy_safe(resolved_path, resolved_path_आकार, path)) {
        जाओ error;
    }

    वापस file;

error:
    log_err(ctx, XKB_ERROR_INSUFFICIENT_बफर_आकार,
            "Cannot copy resolved path or section\n");
    ख_बंद(file);
    वापस शून्य;
}

व्योम
xkb_file_section_init(काष्ठा xkb_file_section *section)
{
    darray_init(section->include_groups);
    darray_init(section->includes);
    darray_init(section->बफर);
    darray_append(section->बफर, '\0');
}

अटल व्योम
xkb_file_section_reset(काष्ठा xkb_file_section *section)
{
    darray_आकार(section->include_groups) = 0;
    darray_आकार(section->includes) = 0;
    darray_आकार(section->बफर) = 1; /* keep the dummy string */
}

व्योम
xkb_file_section_free(काष्ठा xkb_file_section *section)
{
    अगर (!section)
        वापस;

    darray_free(section->include_groups);
    darray_free(section->includes);
    darray_free(section->बफर);
}

अटल bool
xkb_file_section_set_meta_तथ्य(काष्ठा xkb_context *ctx,
                               काष्ठा xkb_file_section *section,
                               स्थिर XkbFile *xkb_file)
{
    section->file_type = xkb_file->file_type;
    section->flags = xkb_file->flags;
    अगर (xkb_file->name) {
        darray_आकार_t idx = darray_आकार(section->बफर);
        darray_append_string0(section->बफर, xkb_file->name);
        section->name = idx;
    } अन्यथा {
        section->name = 0;
    }
    वापस true;
}

/** Process a list of include statements */
अटल bool
xkb_file_section_append_includes(काष्ठा xkb_context *ctx,
                                 क्रमागत xkb_file_iterator_flags flags,
                                 स्थिर काष्ठा parser_keymap_config *config,
                                 स्थिर अक्षर *section_path,
                                 काष्ठा xkb_file_section *section,
                                 क्रमागत xkb_file_type file_type,
                                 IncludeStmt * include)
{
    काष्ठा xkb_file_include_group *group = शून्य;
    /* Note: this will flatten statements such as `include "pc+de"` */
    क्रम (IncludeStmt *stmt = include; stmt; stmt = stmt->अग्र_incl) {
        अक्षर buf[PATH_MAX];
        /* Parse the included file to check the include validity */
        XkbFile *xkb_file = ProcessIncludeFile(ctx, config, stmt, file_type,
                                               buf, माप(buf));
        स्थिर bool valid = (xkb_file != शून्य);
        अगर (valid || !(flags & XKB_FILE_ITERATOR_FAIL_ON_INCLUDE_ERROR)) {
            /* Collect the strings of the statement properties */

            स्थिर darray_आकार_t path = darray_आकार(section->बफर);
            darray_append_string0(section->बफर, buf);

            स्थिर darray_आकार_t file = darray_आकार(section->बफर);
            darray_append_string0(section->बफर, stmt->file);

            स्थिर darray_आकार_t section_name =
                (stmt->map != शून्य || (valid && xkb_file->name))
                    ? darray_आकार(section->बफर)
                    : 0;
            अगर (section_name) {
                darray_append_string0(section->बफर,
                                      (stmt->map) ? stmt->map : xkb_file->name);
            }

            स्थिर darray_आकार_t modifier = (stmt->modifier)
                ? darray_आकार(section->बफर)
                : 0;
            अगर (modifier) {
                darray_append_string0(section->बफर, stmt->modifier);
            }

            स्थिर क्रमागत xkb_map_flags section_flags = (valid)
                ? xkb_file->flags
                : 0;

            /* Create and append the include statement */
            स्थिर काष्ठा xkb_file_include inc = {
                .valid = valid,
                .explicit_section = (stmt->map != शून्य),
                .merge = stmt->merge,
                .path = path,
                .file = file,
                .section = section_name,
                .modifier = modifier,
                .flags = section_flags
            };
            स्थिर darray_आकार_t idx = darray_आकार(section->includes);
            darray_append(section->includes, inc);

            /* Update include group */
            अगर (group == शून्य) {
                स्थिर darray_आकार_t group_idx =
                    darray_आकार(section->include_groups);
                darray_append(
                    section->include_groups,
                    (काष्ठा xkb_file_include_group) {.start = idx, .end = idx}
                );
                group = &darray_item(section->include_groups, group_idx);
            } अन्यथा {
                group->end = idx;
            }

            FreeXkbFile(xkb_file);
        } अन्यथा {
            स्थिर अक्षर * स्थिर name =
                xkb_file_section_get_string(section, section->name);
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "%s include failure in: %s%s%s%s\n",
                    xkb_file_type_name(file_type), section_path,
                    (section->name ? " (section: \"": ""), name,
                    (section->name ? "\")": ""));
            FreeXkbFile(xkb_file);
            वापस false;
        }
    };
    वापस true;
}

/* Process the AST of a section */
अटल bool
xkb_file_section_process(काष्ठा xkb_context *ctx,
                         क्रमागत xkb_file_iterator_flags flags,
                         स्थिर काष्ठा parser_keymap_config *config,
                         स्थिर अक्षर *path,
                         काष्ठा xkb_file_section *section,
                         स्थिर XkbFile *xkb_file)
{
    bool ok = true;
    क्रम (ParseCommon *stmt = xkb_file->defs; stmt; stmt = stmt->अग्र) {
        अगर (stmt->type == STMT_INCLUDE) {
            ok = xkb_file_section_append_includes(ctx, flags, config, path,
                                                  section, xkb_file->file_type,
                                                  (IncludeStmt *) stmt);
            अगर (!ok)
                अवरोध;
        }
    }
    वापस ok;
}

bool
xkb_file_section_parse(काष्ठा xkb_context *ctx,
                       क्रमागत xkb_file_iterator_flags iterator_flags,
                       क्रमागत xkb_keymap_format format,
                       क्रमागत xkb_keymap_compile_flags compile_flags,
                       अचिन्हित पूर्णांक include_depth,
                       स्थिर अक्षर *path, स्थिर अक्षर *map,
                       काष्ठा xkb_file_section *section)
{
    अगर (ExceedsIncludeMaxDepth(ctx, include_depth))
        वापस false;

    खाता *file = ख_खोलो(path, "rb");
    अगर (!file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot open file: %s\n", path);
        वापस false;
    }

    स्थिर काष्ठा parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    XkbFile *xkb_file = XkbParseFile(ctx, &config, file, path, map);
    ख_बंद(file);
    अगर (!xkb_file) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot parse map \"%s\" in file: %s\n",
                (map ? map : "(no map)"), path);
        वापस false;
    }

    xkb_file_section_reset(section);
    स्थिर bool no_includes = iterator_flags & XKB_FILE_ITERATOR_NO_INCLUDES;
    स्थिर bool ok = (
        xkb_file_section_set_meta_तथ्य(ctx, section, xkb_file) &&
        (no_includes ||
         xkb_file_section_process(ctx, iterator_flags, &config, path, section,
                                  xkb_file))
    );
    FreeXkbFile(xkb_file);
    वापस ok;
}

काष्ठा xkb_file_iterator *
xkb_file_iterator_new_from_बफर(काष्ठा xkb_context *ctx,
                                  क्रमागत xkb_file_iterator_flags iterator_flags,
                                  क्रमागत xkb_keymap_format format,
                                  क्रमागत xkb_keymap_compile_flags compile_flags,
                                  स्थिर अक्षर *path, स्थिर अक्षर *map,
                                  क्रमागत xkb_file_type file_type,
                                  स्थिर अक्षर *string, माप_प्रकार लंबाई)
{
    काष्ठा xkb_file_iterator * स्थिर iter = सुस्मृति(1, माप(*iter));
    अगर (!iter) {
        log_err(ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate file iterator\n");
        वापस शून्य;
    }

    iter->flags = iterator_flags;
    iter->ctx = ctx;
    iter->path = path;
    iter->map = map;
    iter->type = file_type;
    iter->parser_config = (काष्ठा parser_keymap_config) {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };
    xkb_file_section_init(&iter->section);

    अगर (!XkbParseStringInit(ctx, &iter->scanner, string, लंबाई, path, शून्य)) {
        xkb_file_iterator_free(iter);
        वापस शून्य;
    }

    वापस iter;
}

व्योम
xkb_file_iterator_free(काष्ठा xkb_file_iterator *iter)
{
    अगर (!iter)
        वापस;

    xkb_file_section_free(&iter->section);
    FreeXkbFile(iter->pending_xkb_file);
    मुक्त(iter);
}

bool
xkb_file_iterator_अग्र(काष्ठा xkb_file_iterator *iter,
                       स्थिर काष्ठा xkb_file_section **section)
{
    अगर (iter->finished) {
        *section = शून्य;
        वापस true;
    }

अग्र:
    { /* C11 compatibility: label cannot be followed by a declaration */ }
    /* Parse next section in the file */
    XkbFile *xkb_file = शून्य;

    अगर (iter->pending_xkb_file) {
        /* We are parsing the components of a keymap */
        अगर (iter->pending_section) {
            /* Parse next component */
            xkb_file = iter->pending_section;
            जाओ parse_components;
        } अन्यथा {
            /* No more components, try next keymap */
            FreeXkbFile(iter->pending_xkb_file);
            iter->pending_xkb_file = शून्य;
        }
    }

    अगर (!Xkb_Parse_String_अग्र(iter->ctx, &iter->parser_config, &iter->scanner,
                            iter->map, &xkb_file)) {
        log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Error while parsing section in file: %s\n", iter->path);
        जाओ error;
    } अन्यथा अगर (!xkb_file) {
        /* No more sections */
        iter->finished = true;
        *section = शून्य;
        वापस true;
    }

parse_components:
    /* Reset section */
    xkb_file_section_reset(&iter->section);

    /* Meta data */
    अगर (!xkb_file_section_set_meta_तथ्य(iter->ctx, &iter->section, xkb_file))
        जाओ error;

    /* Return current section */
    *section = &iter->section;

    अगर (xkb_file->file_type == FILE_TYPE_KEYMAP) {
        /*
         * If it’s a keymap, then stop here.
         * Next iteration will process its components.
         */
        iter->pending_xkb_file = xkb_file;
        iter->pending_section = (XkbFile *) xkb_file->defs;
        iter->map = शून्य;
        वापस true;
    } अन्यथा {
        अगर (iter->type != FILE_TYPE_INVALID &&
            xkb_file->file_type != iter->type) {
            अगर (iter->pending_xkb_file) {
                /* Within a keymap: filter out component */
                iter->pending_section = (XkbFile *) xkb_file->common.अग्र;
                जाओ अग्र;
            } अन्यथा {
                /* Component-specific file: type mismatch */
                log_err(iter->ctx, XKB_LOG_MESSAGE_NO_ID,
                        "File type mismatch: %s, section: %s\n",
                        iter->path,
                        (xkb_file->name ? xkb_file->name : "(no name)"));
                जाओ error;
            }
        }
        अगर (iter->map) {
            iter->finished = true;
        }
    }

    /* Collect include statements of current section */
    स्थिर bool process_includes = !(iter->flags & XKB_FILE_ITERATOR_NO_INCLUDES);
    अगर (process_includes &&
        !xkb_file_section_process(iter->ctx, iter->flags, &iter->parser_config,
                                  iter->path, &iter->section, xkb_file)) {
        जाओ error;
    } अन्यथा अगर (iter->pending_section) {
        /* Next component */
        iter->pending_section = (XkbFile *) xkb_file->common.अग्र;
    } अन्यथा {
        /* No more component, free the file containing the components */
        FreeXkbFile(xkb_file);
    }

    वापस true;

error:
    FreeXkbFile(xkb_file);
    *section = शून्य;
    वापस false;
}

/* Lookup a string by its index */
स्थिर अक्षर *
xkb_file_section_get_string(स्थिर काष्ठा xkb_file_section *section, darray_आकार_t idx)
{
    वापस &darray_item(section->बफर, idx);
}
