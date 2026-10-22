/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <limits.h>
#అన్తర్భావ <string.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "context.h"
#అన్తర్భావ "darray.h"
#అన్తర్భావ "include.h"
#అన్తర్భావ "keymap-formats.h"
#అన్తర్భావ "keymap-file-iterator.h"
#అన్తర్భావ "messages-codes.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "util-paths.h"
#అన్తర్భావ "xkbcomp-priv.h"
#అన్తర్భావ "xkbcomp/ast.h"

స్థిరరాశి అక్షరం *
xkb_file_type_name(గణన xkb_file_type type)
{
    ఒకవేళ (type > FILE_TYPE_KEYMAP)
        తిరిగి "unknown";

    స్థిర స్థిరరాశి అక్షరం *xkb_file_type_అక్షరశ్రేణులు[FILE_TYPE_అంకె_ENTRIES] = {
        [FILE_TYPE_KEYCODES] = "keycodes",
        [FILE_TYPE_TYPES] = "types",
        [FILE_TYPE_COMPAT] = "compatibility",
        [FILE_TYPE_SYMBOLS] = "symbols",
        [FILE_TYPE_GEOMETRY] = "geometry",
        [FILE_TYPE_KEYMAP] = "keymap",
    };
    తిరిగి xkb_file_type_అక్షరశ్రేణులు[type];
}

స్థిరరాశి అక్షరం *
xkb_విలీనం_బహుళకం_name(గణన విలీనం_బహుళకం విలీనం)
{
    ఒకవేళ (విలీనం >= విలీనం_బహుళకం_అంకె_ENTRIES)
        తిరిగి "unknown";

    స్థిర స్థిరరాశి అక్షరం *విలీనం_బహుళకం_అక్షరశ్రేణులు[విలీనం_బహుళకం_అంకె_ENTRIES] = {
        [విలీనం_DEFAULT] = "default",
        [విలీనం_AUGMENT] = "augment",
        [విలీనం_OVERRIDE] = "override",
        [విలీనం_REPLACE] = "replace",
    };
    తిరిగి విలీనం_బహుళకం_అక్షరశ్రేణులు[విలీనం];
}

/* Stateful lookup of map flags names */
స్థిరరాశి అక్షరం *
xkb_map_flags_string_iter(గుర్తులేని పూర్ణాంకం *సూచీ, గణన xkb_map_flags flags)
{
    ఒకవేళ (!flags)
        తిరిగి ఖాళీ;

    స్థిర స్థిరరాశి నిర్మాణం {
        గణన xkb_map_flags flag;
        స్థిరరాశి అక్షరం *name;
    } పేర్లు[] = {
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

    అయితే (*సూచీ < ARRAY_కొలత(పేర్లు)) {
        ఒకవేళ (flags & పేర్లు[*సూచీ].flag)
            తిరిగి పేర్లు[(*సూచీ)++].name;
        (*సూచీ)++;
    }

    తిరిగి ఖాళీ;
};

FILE *
xkb_resolve_file(నిర్మాణం xkb_context *ctx,
                 గణన xkb_file_type file_type,
                 స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *map,
                 అక్షరం *resolved_మార్గం, కొలత_t resolved_మార్గం_కొలత,
                 అక్షరం *resolved_map, కొలత_t resolved_map_కొలత)
{
    గుర్తులేని పూర్ణాంకం స్థానభ్రంశం = 0;
    FILE *file = ఖాళీ; /* Exact match */
    FILE *candidate = ఖాళీ; /* Weak match */
    స్థిరరాశి కొలత_t మార్గం_len = అక్షరశ్రేణి_పొడవు(మార్గం);

    స్థిరరాశి bool absolute_మార్గం = is_absolute_మార్గం(మార్గం);
    ఒకవేళ (absolute_మార్గం) {
        /* Absolute path: no need for lookup in XKB paths */
        file = fopen(మార్గం, "rb");
    } లేకపోతే {
        /* Relative path: lookup the first XKB path */
        file = కనుగొను_File_లోపల_Xkb_మార్గం(ctx, "(unknown)", మార్గం, మార్గం_len, file_type,
                                 resolved_మార్గం, resolved_మార్గం_కొలత, &స్థానభ్రంశం,
                                 true);
    }

    స్థిరరాశి గణన xkb_keymap_format format = DEFAULT_ఆదానం_KEYMAP_FORMAT;
    స్థిరరాశి నిర్మాణం parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    అయితే (file) {
        XkbFile * స్థిరరాశి xkb_file = XkbParseFile(ctx, &config, file, మార్గం, map);
        ఒకవేళ (xkb_file) {
            ఒకవేళ (file_type < FILE_TYPE_అంకె_ENTRIES &&
                xkb_file->file_type != file_type) {
                log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                        "File of wrong type (expected %s, got %s); "
                        "file \"%s\" ignored\n",
                        xkb_file_type_గమ్యం_string(file_type),
                        xkb_file_type_గమ్యం_string(xkb_file->file_type),
                        (absolute_మార్గం ? మార్గం : resolved_మార్గం));
                వెళ్ళు invalid_file;
            } లేకపోతే ఒకవేళ (map || (xkb_file->flags && MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops in this iteration.
                 */
                ఒకవేళ (!strcpy_సురక్షిత(resolved_map, resolved_map_కొలత,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    వెళ్ళు error;
                }
            } లేకపోతే ఒకవేళ (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = file;
                ఒకవేళ (!strcpy_సురక్షిత(resolved_map, resolved_map_కొలత,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    వెళ్ళు error;
                }
            } లేకపోతే {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                వెళ్ళు invalid_file;
            }
        } లేకపోతే {
invalid_file:
            fclose(file);
            file = ఖాళీ;
        }

        FreeXkbFile(xkb_file);

        ఒకవేళ (file != ఖాళీ || absolute_మార్గం) {
            /* Exact match or absolute path with no further file to search */
            ఆపు;
        }

        స్థానభ్రంశం++;
        file = కనుగొను_File_లోపల_Xkb_మార్గం(ctx, "(unknown)",
                                 మార్గం, మార్గం_len, file_type,
                                 resolved_మార్గం, resolved_మార్గం_కొలత,
                                 &స్థానభ్రంశం, true);
    }

    ఒకవేళ (!file) {
        /* No exact match: use weak match, if any */
        file = candidate;
    } లేకపోతే ఒకవేళ (candidate) {
        /* Found exact match: discard weak match, if any */
        fclose(candidate);
    }

    ఒకవేళ (absolute_మార్గం && file &&
        !strcpy_సురక్షిత(resolved_మార్గం, resolved_మార్గం_కొలత, మార్గం)) {
        వెళ్ళు error;
    }

    తిరిగి file;

error:
    log_err(ctx, XKB_ERROR_INSUFFICIENT_బఫరు_కొలత,
            "Cannot copy resolved path or section\n");
    fclose(file);
    తిరిగి ఖాళీ;
}

శూన్య
xkb_file_section_init(నిర్మాణం xkb_file_section *section)
{
    darray_init(section->include_groups);
    darray_init(section->includes);
    darray_init(section->బఫరు);
    darray_append(section->బఫరు, '\0');
}

స్థిర శూన్య
xkb_file_section_reset(నిర్మాణం xkb_file_section *section)
{
    darray_కొలత(section->include_groups) = 0;
    darray_కొలత(section->includes) = 0;
    darray_కొలత(section->బఫరు) = 1; /* keep the dummy string */
}

శూన్య
xkb_file_section_free(నిర్మాణం xkb_file_section *section)
{
    ఒకవేళ (!section)
        తిరిగి;

    darray_free(section->include_groups);
    darray_free(section->includes);
    darray_free(section->బఫరు);
}

స్థిర bool
xkb_file_section_సమితి_meta_దత్తాంశం(నిర్మాణం xkb_context *ctx,
                               నిర్మాణం xkb_file_section *section,
                               స్థిరరాశి XkbFile *xkb_file)
{
    section->file_type = xkb_file->file_type;
    section->flags = xkb_file->flags;
    ఒకవేళ (xkb_file->name) {
        darray_కొలత_t idx = darray_కొలత(section->బఫరు);
        darray_append_string0(section->బఫరు, xkb_file->name);
        section->name = idx;
    } లేకపోతే {
        section->name = 0;
    }
    తిరిగి true;
}

/** Process a list of include statements */
స్థిర bool
xkb_file_section_append_includes(నిర్మాణం xkb_context *ctx,
                                 గణన xkb_file_iterator_flags flags,
                                 స్థిరరాశి నిర్మాణం parser_keymap_config *config,
                                 స్థిరరాశి అక్షరం *section_మార్గం,
                                 నిర్మాణం xkb_file_section *section,
                                 గణన xkb_file_type file_type,
                                 IncludeStmt * include)
{
    నిర్మాణం xkb_file_include_సమూహం *సమూహం = ఖాళీ;
    /* Note: this will flatten statements such as `include "pc+de"` */
    కొరకు (IncludeStmt *stmt = include; stmt; stmt = stmt->తదుపరి_incl) {
        అక్షరం buf[PATH_MAX];
        /* Parse the included file to check the include validity */
        XkbFile *xkb_file = ProcessIncludeFile(ctx, config, stmt, file_type,
                                               buf, పరిమాణం(buf));
        స్థిరరాశి bool valid = (xkb_file != ఖాళీ);
        ఒకవేళ (valid || !(flags & XKB_FILE_ITERATOR_FAIL_ON_INCLUDE_ERROR)) {
            /* Collect the strings of the statement properties */

            స్థిరరాశి darray_కొలత_t మార్గం = darray_కొలత(section->బఫరు);
            darray_append_string0(section->బఫరు, buf);

            స్థిరరాశి darray_కొలత_t file = darray_కొలత(section->బఫరు);
            darray_append_string0(section->బఫరు, stmt->file);

            స్థిరరాశి darray_కొలత_t section_name =
                (stmt->map != ఖాళీ || (valid && xkb_file->name))
                    ? darray_కొలత(section->బఫరు)
                    : 0;
            ఒకవేళ (section_name) {
                darray_append_string0(section->బఫరు,
                                      (stmt->map) ? stmt->map : xkb_file->name);
            }

            స్థిరరాశి darray_కొలత_t modifier = (stmt->modifier)
                ? darray_కొలత(section->బఫరు)
                : 0;
            ఒకవేళ (modifier) {
                darray_append_string0(section->బఫరు, stmt->modifier);
            }

            స్థిరరాశి గణన xkb_map_flags section_flags = (valid)
                ? xkb_file->flags
                : 0;

            /* Create and append the include statement */
            స్థిరరాశి నిర్మాణం xkb_file_include inc = {
                .valid = valid,
                .explicit_section = (stmt->map != ఖాళీ),
                .విలీనం = stmt->విలీనం,
                .మార్గం = మార్గం,
                .file = file,
                .section = section_name,
                .modifier = modifier,
                .flags = section_flags
            };
            స్థిరరాశి darray_కొలత_t idx = darray_కొలత(section->includes);
            darray_append(section->includes, inc);

            /* Update include group */
            ఒకవేళ (సమూహం == ఖాళీ) {
                స్థిరరాశి darray_కొలత_t సమూహం_idx =
                    darray_కొలత(section->include_groups);
                darray_append(
                    section->include_groups,
                    (నిర్మాణం xkb_file_include_సమూహం) {.ప్రారంభం = idx, .చివర = idx}
                );
                సమూహం = &darray_అంశం(section->include_groups, సమూహం_idx);
            } లేకపోతే {
                సమూహం->చివర = idx;
            }

            FreeXkbFile(xkb_file);
        } లేకపోతే {
            స్థిరరాశి అక్షరం * స్థిరరాశి name =
                xkb_file_section_get_string(section, section->name);
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_దొరికింది,
                    "%s include failure in: %s%s%s%s\n",
                    xkb_file_type_name(file_type), section_మార్గం,
                    (section->name ? " (section: \"": ""), name,
                    (section->name ? "\")": ""));
            FreeXkbFile(xkb_file);
            తిరిగి false;
        }
    };
    తిరిగి true;
}

/* Process the AST of a section */
స్థిర bool
xkb_file_section_process(నిర్మాణం xkb_context *ctx,
                         గణన xkb_file_iterator_flags flags,
                         స్థిరరాశి నిర్మాణం parser_keymap_config *config,
                         స్థిరరాశి అక్షరం *మార్గం,
                         నిర్మాణం xkb_file_section *section,
                         స్థిరరాశి XkbFile *xkb_file)
{
    bool ok = true;
    కొరకు (ParseCommon *stmt = xkb_file->defs; stmt; stmt = stmt->తదుపరి) {
        ఒకవేళ (stmt->type == STMT_INCLUDE) {
            ok = xkb_file_section_append_includes(ctx, flags, config, మార్గం,
                                                  section, xkb_file->file_type,
                                                  (IncludeStmt *) stmt);
            ఒకవేళ (!ok)
                ఆపు;
        }
    }
    తిరిగి ok;
}

bool
xkb_file_section_parse(నిర్మాణం xkb_context *ctx,
                       గణన xkb_file_iterator_flags iterator_flags,
                       గణన xkb_keymap_format format,
                       గణన xkb_keymap_compile_flags compile_flags,
                       గుర్తులేని పూర్ణాంకం include_లోతు,
                       స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *map,
                       నిర్మాణం xkb_file_section *section)
{
    ఒకవేళ (Exceeds_Include_Max_లోతు(ctx, include_లోతు))
        తిరిగి false;

    FILE *file = fopen(మార్గం, "rb");
    ఒకవేళ (!file) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "Cannot open file: %s\n", మార్గం);
        తిరిగి false;
    }

    స్థిరరాశి నిర్మాణం parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    XkbFile *xkb_file = XkbParseFile(ctx, &config, file, మార్గం, map);
    fclose(file);
    ఒకవేళ (!xkb_file) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "Cannot parse map \"%s\" in file: %s\n",
                (map ? map : "(no map)"), మార్గం);
        తిరిగి false;
    }

    xkb_file_section_reset(section);
    స్థిరరాశి bool no_includes = iterator_flags & XKB_FILE_ITERATOR_NO_INCLUDES;
    స్థిరరాశి bool ok = (
        xkb_file_section_సమితి_meta_దత్తాంశం(ctx, section, xkb_file) &&
        (no_includes ||
         xkb_file_section_process(ctx, iterator_flags, &config, మార్గం, section,
                                  xkb_file))
    );
    FreeXkbFile(xkb_file);
    తిరిగి ok;
}

నిర్మాణం xkb_file_iterator *
xkb_file_iterator_new_from_బఫరు(నిర్మాణం xkb_context *ctx,
                                  గణన xkb_file_iterator_flags iterator_flags,
                                  గణన xkb_keymap_format format,
                                  గణన xkb_keymap_compile_flags compile_flags,
                                  స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *map,
                                  గణన xkb_file_type file_type,
                                  స్థిరరాశి అక్షరం *string, కొలత_t నిడివి)
{
    నిర్మాణం xkb_file_iterator * స్థిరరాశి iter = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(*iter));
    ఒకవేళ (!iter) {
        log_err(ctx, XKB_ERROR_కేటాయింపు_FAILURE_2,
                "Cannot allocate file iterator\n");
        తిరిగి ఖాళీ;
    }

    iter->flags = iterator_flags;
    iter->ctx = ctx;
    iter->మార్గం = మార్గం;
    iter->map = map;
    iter->type = file_type;
    iter->parser_config = (నిర్మాణం parser_keymap_config) {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };
    xkb_file_section_init(&iter->section);

    ఒకవేళ (!XkbParseStringInit(ctx, &iter->scanner, string, నిడివి, మార్గం, ఖాళీ)) {
        xkb_file_iterator_free(iter);
        తిరిగి ఖాళీ;
    }

    తిరిగి iter;
}

శూన్య
xkb_file_iterator_free(నిర్మాణం xkb_file_iterator *iter)
{
    ఒకవేళ (!iter)
        తిరిగి;

    xkb_file_section_free(&iter->section);
    FreeXkbFile(iter->pending_xkb_file);
    విడుదల(iter);
}

bool
xkb_file_iterator_తదుపరి(నిర్మాణం xkb_file_iterator *iter,
                       స్థిరరాశి నిర్మాణం xkb_file_section **section)
{
    ఒకవేళ (iter->ముగిసిన) {
        *section = ఖాళీ;
        తిరిగి true;
    }

తదుపరి:
    { /* C11 compatibility: label cannot be followed by a declaration */ }
    /* Parse next section in the file */
    XkbFile *xkb_file = ఖాళీ;

    ఒకవేళ (iter->pending_xkb_file) {
        /* We are parsing the components of a keymap */
        ఒకవేళ (iter->pending_section) {
            /* Parse next component */
            xkb_file = iter->pending_section;
            వెళ్ళు parse_components;
        } లేకపోతే {
            /* No more components, try next keymap */
            FreeXkbFile(iter->pending_xkb_file);
            iter->pending_xkb_file = ఖాళీ;
        }
    }

    ఒకవేళ (!Xkb_Parse_String_తదుపరి(iter->ctx, &iter->parser_config, &iter->scanner,
                            iter->map, &xkb_file)) {
        log_err(iter->ctx, XKB_LOG_సందేశం_NO_ID,
                "Error while parsing section in file: %s\n", iter->మార్గం);
        వెళ్ళు error;
    } లేకపోతే ఒకవేళ (!xkb_file) {
        /* No more sections */
        iter->ముగిసిన = true;
        *section = ఖాళీ;
        తిరిగి true;
    }

parse_components:
    /* Reset section */
    xkb_file_section_reset(&iter->section);

    /* Meta data */
    ఒకవేళ (!xkb_file_section_సమితి_meta_దత్తాంశం(iter->ctx, &iter->section, xkb_file))
        వెళ్ళు error;

    /* Return current section */
    *section = &iter->section;

    ఒకవేళ (xkb_file->file_type == FILE_TYPE_KEYMAP) {
        /*
         * If it’s a keymap, then stop here.
         * Next iteration will process its components.
         */
        iter->pending_xkb_file = xkb_file;
        iter->pending_section = (XkbFile *) xkb_file->defs;
        iter->map = ఖాళీ;
        తిరిగి true;
    } లేకపోతే {
        ఒకవేళ (iter->type != FILE_TYPE_INVALID &&
            xkb_file->file_type != iter->type) {
            ఒకవేళ (iter->pending_xkb_file) {
                /* Within a keymap: filter out component */
                iter->pending_section = (XkbFile *) xkb_file->common.తదుపరి;
                వెళ్ళు తదుపరి;
            } లేకపోతే {
                /* Component-specific file: type mismatch */
                log_err(iter->ctx, XKB_LOG_సందేశం_NO_ID,
                        "File type mismatch: %s, section: %s\n",
                        iter->మార్గం,
                        (xkb_file->name ? xkb_file->name : "(no name)"));
                వెళ్ళు error;
            }
        }
        ఒకవేళ (iter->map) {
            iter->ముగిసిన = true;
        }
    }

    /* Collect include statements of current section */
    స్థిరరాశి bool process_includes = !(iter->flags & XKB_FILE_ITERATOR_NO_INCLUDES);
    ఒకవేళ (process_includes &&
        !xkb_file_section_process(iter->ctx, iter->flags, &iter->parser_config,
                                  iter->మార్గం, &iter->section, xkb_file)) {
        వెళ్ళు error;
    } లేకపోతే ఒకవేళ (iter->pending_section) {
        /* Next component */
        iter->pending_section = (XkbFile *) xkb_file->common.తదుపరి;
    } లేకపోతే {
        /* No more component, free the file containing the components */
        FreeXkbFile(xkb_file);
    }

    తిరిగి true;

error:
    FreeXkbFile(xkb_file);
    *section = ఖాళీ;
    తిరిగి false;
}

/* Lookup a string by its index */
స్థిరరాశి అక్షరం *
xkb_file_section_get_string(స్థిరరాశి నిర్మాణం xkb_file_section *section, darray_కొలత_t idx)
{
    తిరిగి &darray_అంశం(section->బఫరు, idx);
}
