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
    यदि (type > FILE_TYPE_KEYMAP)
        प्रत्यागम "unknown";

    स्थिर ध्रुव वर्ण *xkb_file_type_वाक्यानि[FILE_TYPE_अङ्कः_ENTRIES] = {
        [FILE_TYPE_KEYCODES] = "keycodes",
        [FILE_TYPE_TYPES] = "types",
        [FILE_TYPE_COMPAT] = "compatibility",
        [FILE_TYPE_SYMBOLS] = "symbols",
        [FILE_TYPE_GEOMETRY] = "geometry",
        [FILE_TYPE_KEYMAP] = "keymap",
    };
    प्रत्यागम xkb_file_type_वाक्यानि[type];
}

ध्रुव वर्ण *
xkb_merge_बहुलकम्_name(गणना merge_बहुलकम् merge)
{
    यदि (merge >= MERGE_बहुलकम्_अङ्कः_ENTRIES)
        प्रत्यागम "unknown";

    स्थिर ध्रुव वर्ण *merge_बहुलकम्_वाक्यानि[MERGE_बहुलकम्_अङ्कः_ENTRIES] = {
        [MERGE_DEFAULT] = "default",
        [MERGE_AUGMENT] = "augment",
        [MERGE_OVERRIDE] = "override",
        [MERGE_REPLACE] = "replace",
    };
    प्रत्यागम merge_बहुलकम्_वाक्यानि[merge];
}

/* Stateful lookup of map flags names */
ध्रुव वर्ण *
xkb_map_flags_string_iter(अचिह्नित पूर्णाङ्क *सूचकांक, गणना xkb_map_flags flags)
{
    यदि (!flags)
        प्रत्यागम रिक्त;

    स्थिर ध्रुव संरचना {
        गणना xkb_map_flags flag;
        ध्रुव वर्ण *name;
    } नामानि[] = {
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

    यावत् (*सूचकांक < ARRAY_आकार(नामानि)) {
        यदि (flags & नामानि[*सूचकांक].flag)
            प्रत्यागम नामानि[(*सूचकांक)++].name;
        (*सूचकांक)++;
    }

    प्रत्यागम रिक्त;
};

FILE *
xkb_resolve_file(संरचना xkb_context *ctx,
                 गणना xkb_file_type file_type,
                 ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *map,
                 वर्ण *resolved_मार्गः, आकार_t resolved_मार्गः_आकार,
                 वर्ण *resolved_map, आकार_t resolved_map_आकार)
{
    अचिह्नित पूर्णाङ्क विस्थापनम् = 0;
    FILE *file = रिक्त; /* Exact match */
    FILE *candidate = रिक्त; /* Weak match */
    ध्रुव आकार_t मार्गः_len = वाक्यदैर्घ्यम्(मार्गः);

    ध्रुव bool absolute_मार्गः = is_absolute_मार्गः(मार्गः);
    यदि (absolute_मार्गः) {
        /* Absolute path: no need for lookup in XKB paths */
        file = fopen(मार्गः, "rb");
    } अन्यथा {
        /* Relative path: lookup the first XKB path */
        file = Find_File_अन्तर्_Xkb_मार्गः(ctx, "(unknown)", मार्गः, मार्गः_len, file_type,
                                 resolved_मार्गः, resolved_मार्गः_आकार, &विस्थापनम्,
                                 true);
    }

    ध्रुव गणना xkb_keymap_format format = DEFAULT_निवेशः_KEYMAP_FORMAT;
    ध्रुव संरचना parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    यावत् (file) {
        XkbFile * ध्रुव xkb_file = XkbParseFile(ctx, &config, file, मार्गः, map);
        यदि (xkb_file) {
            यदि (file_type < FILE_TYPE_अङ्कः_ENTRIES &&
                xkb_file->file_type != file_type) {
                log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                        "File of wrong type (expected %s, got %s); "
                        "file \"%s\" ignored\n",
                        xkb_file_type_गन्तव्यम्_string(file_type),
                        xkb_file_type_गन्तव्यम्_string(xkb_file->file_type),
                        (absolute_मार्गः ? मार्गः : resolved_मार्गः));
                गच्छ invalid_file;
            } अन्यथा यदि (map || (xkb_file->flags && MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops in this iteration.
                 */
                यदि (!strcpy_safe(resolved_map, resolved_map_आकार,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    गच्छ error;
                }
            } अन्यथा यदि (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = file;
                यदि (!strcpy_safe(resolved_map, resolved_map_आकार,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    गच्छ error;
                }
            } अन्यथा {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                गच्छ invalid_file;
            }
        } अन्यथा {
invalid_file:
            fclose(file);
            file = रिक्त;
        }

        FreeXkbFile(xkb_file);

        यदि (file != रिक्त || absolute_मार्गः) {
            /* Exact match or absolute path with no further file to search */
            भङ्ग;
        }

        विस्थापनम्++;
        file = Find_File_अन्तर्_Xkb_मार्गः(ctx, "(unknown)",
                                 मार्गः, मार्गः_len, file_type,
                                 resolved_मार्गः, resolved_मार्गः_आकार,
                                 &विस्थापनम्, true);
    }

    यदि (!file) {
        /* No exact match: use weak match, if any */
        file = candidate;
    } अन्यथा यदि (candidate) {
        /* Found exact match: discard weak match, if any */
        fclose(candidate);
    }

    यदि (absolute_मार्गः && file &&
        !strcpy_safe(resolved_मार्गः, resolved_मार्गः_आकार, मार्गः)) {
        गच्छ error;
    }

    प्रत्यागम file;

error:
    log_err(ctx, XKB_ERROR_INSUFFICIENT_बफर_आकार,
            "Cannot copy resolved path or section\n");
    fclose(file);
    प्रत्यागम रिक्त;
}

शून्य
xkb_file_section_init(संरचना xkb_file_section *section)
{
    darray_init(section->include_groups);
    darray_init(section->includes);
    darray_init(section->बफर);
    darray_append(section->बफर, '\0');
}

स्थिर शून्य
xkb_file_section_reset(संरचना xkb_file_section *section)
{
    darray_आकार(section->include_groups) = 0;
    darray_आकार(section->includes) = 0;
    darray_आकार(section->बफर) = 1; /* keep the dummy string */
}

शून्य
xkb_file_section_free(संरचना xkb_file_section *section)
{
    यदि (!section)
        प्रत्यागम;

    darray_free(section->include_groups);
    darray_free(section->includes);
    darray_free(section->बफर);
}

स्थिर bool
xkb_file_section_समुच्चयः_meta_दत्त(संरचना xkb_context *ctx,
                               संरचना xkb_file_section *section,
                               ध्रुव XkbFile *xkb_file)
{
    section->file_type = xkb_file->file_type;
    section->flags = xkb_file->flags;
    यदि (xkb_file->name) {
        darray_आकार_t idx = darray_आकार(section->बफर);
        darray_append_string0(section->बफर, xkb_file->name);
        section->name = idx;
    } अन्यथा {
        section->name = 0;
    }
    प्रत्यागम true;
}

/** Process a list of include statements */
स्थिर bool
xkb_file_section_append_includes(संरचना xkb_context *ctx,
                                 गणना xkb_file_iterator_flags flags,
                                 ध्रुव संरचना parser_keymap_config *config,
                                 ध्रुव वर्ण *section_मार्गः,
                                 संरचना xkb_file_section *section,
                                 गणना xkb_file_type file_type,
                                 IncludeStmt * include)
{
    संरचना xkb_file_include_गणः *गणः = रिक्त;
    /* Note: this will flatten statements such as `include "pc+de"` */
    प्रति (IncludeStmt *stmt = include; stmt; stmt = stmt->अग्र_incl) {
        वर्ण buf[PATH_MAX];
        /* Parse the included file to check the include validity */
        XkbFile *xkb_file = ProcessIncludeFile(ctx, config, stmt, file_type,
                                               buf, परिमाण(buf));
        ध्रुव bool valid = (xkb_file != रिक्त);
        यदि (valid || !(flags & XKB_FILE_ITERATOR_FAIL_ON_INCLUDE_ERROR)) {
            /* Collect the strings of the statement properties */

            ध्रुव darray_आकार_t मार्गः = darray_आकार(section->बफर);
            darray_append_string0(section->बफर, buf);

            ध्रुव darray_आकार_t file = darray_आकार(section->बफर);
            darray_append_string0(section->बफर, stmt->file);

            ध्रुव darray_आकार_t section_name =
                (stmt->map != रिक्त || (valid && xkb_file->name))
                    ? darray_आकार(section->बफर)
                    : 0;
            यदि (section_name) {
                darray_append_string0(section->बफर,
                                      (stmt->map) ? stmt->map : xkb_file->name);
            }

            ध्रुव darray_आकार_t modifier = (stmt->modifier)
                ? darray_आकार(section->बफर)
                : 0;
            यदि (modifier) {
                darray_append_string0(section->बफर, stmt->modifier);
            }

            ध्रुव गणना xkb_map_flags section_flags = (valid)
                ? xkb_file->flags
                : 0;

            /* Create and append the include statement */
            ध्रुव संरचना xkb_file_include inc = {
                .valid = valid,
                .explicit_section = (stmt->map != रिक्त),
                .merge = stmt->merge,
                .मार्गः = मार्गः,
                .file = file,
                .section = section_name,
                .modifier = modifier,
                .flags = section_flags
            };
            ध्रुव darray_आकार_t idx = darray_आकार(section->includes);
            darray_append(section->includes, inc);

            /* Update include group */
            यदि (गणः == रिक्त) {
                ध्रुव darray_आकार_t गणः_idx =
                    darray_आकार(section->include_groups);
                darray_append(
                    section->include_groups,
                    (संरचना xkb_file_include_गणः) {.start = idx, .अन्तः = idx}
                );
                गणः = &darray_item(section->include_groups, गणः_idx);
            } अन्यथा {
                गणः->अन्तः = idx;
            }

            FreeXkbFile(xkb_file);
        } अन्यथा {
            ध्रुव वर्ण * ध्रुव name =
                xkb_file_section_get_string(section, section->name);
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_प्राप्तम्,
                    "%s include failure in: %s%s%s%s\n",
                    xkb_file_type_name(file_type), section_मार्गः,
                    (section->name ? " (section: \"": ""), name,
                    (section->name ? "\")": ""));
            FreeXkbFile(xkb_file);
            प्रत्यागम false;
        }
    };
    प्रत्यागम true;
}

/* Process the AST of a section */
स्थिर bool
xkb_file_section_process(संरचना xkb_context *ctx,
                         गणना xkb_file_iterator_flags flags,
                         ध्रुव संरचना parser_keymap_config *config,
                         ध्रुव वर्ण *मार्गः,
                         संरचना xkb_file_section *section,
                         ध्रुव XkbFile *xkb_file)
{
    bool ok = true;
    प्रति (ParseCommon *stmt = xkb_file->defs; stmt; stmt = stmt->अग्र) {
        यदि (stmt->type == STMT_INCLUDE) {
            ok = xkb_file_section_append_includes(ctx, flags, config, मार्गः,
                                                  section, xkb_file->file_type,
                                                  (IncludeStmt *) stmt);
            यदि (!ok)
                भङ्ग;
        }
    }
    प्रत्यागम ok;
}

bool
xkb_file_section_parse(संरचना xkb_context *ctx,
                       गणना xkb_file_iterator_flags iterator_flags,
                       गणना xkb_keymap_format format,
                       गणना xkb_keymap_compile_flags compile_flags,
                       अचिह्नित पूर्णाङ्क include_गभीरता,
                       ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *map,
                       संरचना xkb_file_section *section)
{
    यदि (Exceeds_Include_Max_गभीरता(ctx, include_गभीरता))
        प्रत्यागम false;

    FILE *file = fopen(मार्गः, "rb");
    यदि (!file) {
        log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                "Cannot open file: %s\n", मार्गः);
        प्रत्यागम false;
    }

    ध्रुव संरचना parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    XkbFile *xkb_file = XkbParseFile(ctx, &config, file, मार्गः, map);
    fclose(file);
    यदि (!xkb_file) {
        log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                "Cannot parse map \"%s\" in file: %s\n",
                (map ? map : "(no map)"), मार्गः);
        प्रत्यागम false;
    }

    xkb_file_section_reset(section);
    ध्रुव bool no_includes = iterator_flags & XKB_FILE_ITERATOR_NO_INCLUDES;
    ध्रुव bool ok = (
        xkb_file_section_समुच्चयः_meta_दत्त(ctx, section, xkb_file) &&
        (no_includes ||
         xkb_file_section_process(ctx, iterator_flags, &config, मार्गः, section,
                                  xkb_file))
    );
    FreeXkbFile(xkb_file);
    प्रत्यागम ok;
}

संरचना xkb_file_iterator *
xkb_file_iterator_new_from_बफर(संरचना xkb_context *ctx,
                                  गणना xkb_file_iterator_flags iterator_flags,
                                  गणना xkb_keymap_format format,
                                  गणना xkb_keymap_compile_flags compile_flags,
                                  ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *map,
                                  गणना xkb_file_type file_type,
                                  ध्रुव वर्ण *string, आकार_t दीर्घता)
{
    संरचना xkb_file_iterator * ध्रुव iter = शून्यस्मृतिं_देहि(1, परिमाण(*iter));
    यदि (!iter) {
        log_err(ctx, XKB_ERROR_आवण्टनम्_FAILURE_2,
                "Cannot allocate file iterator\n");
        प्रत्यागम रिक्त;
    }

    iter->flags = iterator_flags;
    iter->ctx = ctx;
    iter->मार्गः = मार्गः;
    iter->map = map;
    iter->type = file_type;
    iter->parser_config = (संरचना parser_keymap_config) {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };
    xkb_file_section_init(&iter->section);

    यदि (!XkbParseStringInit(ctx, &iter->scanner, string, दीर्घता, मार्गः, रिक्त)) {
        xkb_file_iterator_free(iter);
        प्रत्यागम रिक्त;
    }

    प्रत्यागम iter;
}

शून्य
xkb_file_iterator_free(संरचना xkb_file_iterator *iter)
{
    यदि (!iter)
        प्रत्यागम;

    xkb_file_section_free(&iter->section);
    FreeXkbFile(iter->pending_xkb_file);
    मुक्त(iter);
}

bool
xkb_file_iterator_अग्र(संरचना xkb_file_iterator *iter,
                       ध्रुव संरचना xkb_file_section **section)
{
    यदि (iter->परिसमाप्तम्) {
        *section = रिक्त;
        प्रत्यागम true;
    }

अग्र:
    { /* C11 compatibility: label cannot be followed by a declaration */ }
    /* Parse next section in the file */
    XkbFile *xkb_file = रिक्त;

    यदि (iter->pending_xkb_file) {
        /* We are parsing the components of a keymap */
        यदि (iter->pending_section) {
            /* Parse next component */
            xkb_file = iter->pending_section;
            गच्छ parse_components;
        } अन्यथा {
            /* No more components, try next keymap */
            FreeXkbFile(iter->pending_xkb_file);
            iter->pending_xkb_file = रिक्त;
        }
    }

    यदि (!Xkb_Parse_String_अग्र(iter->ctx, &iter->parser_config, &iter->scanner,
                            iter->map, &xkb_file)) {
        log_err(iter->ctx, XKB_LOG_सन्देशः_NO_ID,
                "Error while parsing section in file: %s\n", iter->मार्गः);
        गच्छ error;
    } अन्यथा यदि (!xkb_file) {
        /* No more sections */
        iter->परिसमाप्तम् = true;
        *section = रिक्त;
        प्रत्यागम true;
    }

parse_components:
    /* Reset section */
    xkb_file_section_reset(&iter->section);

    /* Meta data */
    यदि (!xkb_file_section_समुच्चयः_meta_दत्त(iter->ctx, &iter->section, xkb_file))
        गच्छ error;

    /* Return current section */
    *section = &iter->section;

    यदि (xkb_file->file_type == FILE_TYPE_KEYMAP) {
        /*
         * If it’s a keymap, then stop here.
         * Next iteration will process its components.
         */
        iter->pending_xkb_file = xkb_file;
        iter->pending_section = (XkbFile *) xkb_file->defs;
        iter->map = रिक्त;
        प्रत्यागम true;
    } अन्यथा {
        यदि (iter->type != FILE_TYPE_INVALID &&
            xkb_file->file_type != iter->type) {
            यदि (iter->pending_xkb_file) {
                /* Within a keymap: filter out component */
                iter->pending_section = (XkbFile *) xkb_file->common.अग्र;
                गच्छ अग्र;
            } अन्यथा {
                /* Component-specific file: type mismatch */
                log_err(iter->ctx, XKB_LOG_सन्देशः_NO_ID,
                        "File type mismatch: %s, section: %s\n",
                        iter->मार्गः,
                        (xkb_file->name ? xkb_file->name : "(no name)"));
                गच्छ error;
            }
        }
        यदि (iter->map) {
            iter->परिसमाप्तम् = true;
        }
    }

    /* Collect include statements of current section */
    ध्रुव bool process_includes = !(iter->flags & XKB_FILE_ITERATOR_NO_INCLUDES);
    यदि (process_includes &&
        !xkb_file_section_process(iter->ctx, iter->flags, &iter->parser_config,
                                  iter->मार्गः, &iter->section, xkb_file)) {
        गच्छ error;
    } अन्यथा यदि (iter->pending_section) {
        /* Next component */
        iter->pending_section = (XkbFile *) xkb_file->common.अग्र;
    } अन्यथा {
        /* No more component, free the file containing the components */
        FreeXkbFile(xkb_file);
    }

    प्रत्यागम true;

error:
    FreeXkbFile(xkb_file);
    *section = रिक्त;
    प्रत्यागम false;
}

/* Lookup a string by its index */
ध्रुव वर्ण *
xkb_file_section_get_string(ध्रुव संरचना xkb_file_section *section, darray_आकार_t idx)
{
    प्रत्यागम &darray_item(section->बफर, idx);
}
