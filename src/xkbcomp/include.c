/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <errno.h>
#అన్తర్భావ <limits.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>

#అన్తర్భావ "messages-codes.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "xkbcomp-priv.h"
#అన్తర్భావ "include.h"
#అన్తర్భావ "scanner-utils.h"
#అన్తర్భావ "util-paths.h"

/**
 * Parse an include statement. Each call returns a file name, along with
 * (possibly) a specific map in the file, an explicit group designator, and
 * the separator from the next file, used to determine the merge mode.
 *
 * @param str_inout Input statement, modified in-place. Should be passed in
 * repeatedly. If str_inout is NULL, the parsing has completed.
 *
 * @param file_rtrn Set to the name of the include file to be used. Combined
 * with an enum xkb_file_type, this determines which file to look for in the
 * include path.
 *
 * @param map_rtrn Set to the string between '(' and ')', if any. This will
 * result in the compilation of a specific named map within the file (e.g.
 * xkb_symbols "basic" { ... }) , as opposed to the default map of the file.
 *
 * @param nextop_rtrn Set to the next operation in the complete statement,
 * which is '\0' if it's the last file or '+' or '|' or '^' if there are more.
 * Separating the files with '+' sets the merge mode to `MERGE_MODE_OVERRIDE`,
 * while '|' sets the merge mode to `MERGE_MODE_AUGMENT` and '^' sets the merge
 * mode to `MERGE_REPLACE`.
 *
 * @param extra_data Set to the string after ':', if any. Currently the
 * extra data is only used for setting an explicit group index for a symbols
 * file.
 *
 * @return true if parsing was successful, false for an illegal string.
 *
 * Example: "evdev+aliases(qwerty):2"
 *      str_inout = "aliases(qwerty):2"
 *      file_rtrn = "evdev"
 *      map_rtrn = NULL
 *      nextop_retrn = "+"
 *      extra_data = NULL
 *
 * 2nd run with "aliases(qwerty):2"
 *      str_inout = NULL
 *      file_rtrn = "aliases"
 *      map_rtrn = "qwerty"
 *      nextop_retrn = ""
 *      extra_data = "2"
 *
 */
bool
ParseIncludeMap(అక్షరం **str_inout, అక్షరం **file_rtrn, అక్షరం **map_rtrn,
                అక్షరం *nextop_rtrn, అక్షరం **extra_దత్తాంశం)
{
    అక్షరం *tmp, *str, *తదుపరి;

    str = *str_inout;

    /*
     * Find the position in the string where the next file is included,
     * if there is more than one left in the statement.
     */
    తదుపరి = strpbrk(str, విలీనం_బహుళకం_PREFIXES);
    ఒకవేళ (తదుపరి) {
        /* Got more files, this function will be called again. */
        *nextop_rtrn = *తదుపరి;
        /* Separate the string, for strchr etc. to work on this file only. */
        *తదుపరి++ = '\0';
    }
    లేకపోతే {
        /* This is the last file in this statement, won't be called again. */
        *nextop_rtrn = '\0';
        తదుపరి = ఖాళీ;
    }

    /*
     * Search for the explicit group designator, if any. If it's there,
     * it goes after the file name and map.
     */
    tmp = strchr(str, ':');
    ఒకవేళ (tmp != ఖాళీ) {
        *tmp++ = '\0';
        *extra_దత్తాంశం = strdup(tmp);
    }
    లేకపోతే {
        *extra_దత్తాంశం = ఖాళీ;
    }

    /* Look for a map, if any. */
    tmp = strchr(str, '(');
    ఒకవేళ (tmp == ఖాళీ) {
        /* No map. */
        *file_rtrn = strdup(str);
        *map_rtrn = ఖాళీ;
    }
    లేకపోతే ఒకవేళ (str[0] == '(') {
        /* Map without file - invalid. */
        విడుదల(*extra_దత్తాంశం);
        తిరిగి false;
    }
    లేకపోతే {
        /* Got a map; separate the file and the map for the strdup's. */
        *tmp++ = '\0';
        *file_rtrn = strdup(str);
        str = tmp;
        tmp = strchr(str, ')');
        ఒకవేళ (tmp == ఖాళీ || tmp[1] != '\0') {
            విడుదల(*file_rtrn);
            విడుదల(*extra_దత్తాంశం);
            తిరిగి false;
        }
        *tmp++ = '\0';
        *map_rtrn = strdup(str);
    }

    /* Set up the next file for the next call, if any. */
    ఒకవేళ (*nextop_rtrn == '\0')
        *str_inout = ఖాళీ;
    లేకపోతే ఒకవేళ (is_విలీనం_బహుళకం_ఉపసర్గ(*nextop_rtrn))
        *str_inout = తదుపరి;
    లేకపోతే
        తిరిగి false;

    తిరిగి true;
}

స్థిర స్థిరరాశి అక్షరం *xkb_file_type_include_dirs[FILE_TYPE_అంకె_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "keycodes",
    [FILE_TYPE_TYPES] = "types",
    [FILE_TYPE_COMPAT] = "compat",
    [FILE_TYPE_SYMBOLS] = "symbols",
    [FILE_TYPE_GEOMETRY] = "geometry",
    [FILE_TYPE_KEYMAP] = "keymap",
    [FILE_TYPE_నియమాలు] = "rules",
};

/**
 * Return the xkb directory based on the type.
 */
స్థిర స్థిరరాశి అక్షరం *
DirectoryForInclude(గణన xkb_file_type type)
{
    ఒకవేళ (type >= FILE_TYPE_అంకె_ENTRIES)
        తిరిగి "";
    తిరిగి xkb_file_type_include_dirs[type];
}

స్థిర శూన్య
LogIncludePaths(నిర్మాణం xkb_context *ctx)
{
    ఒకవేళ (xkb_context_అంకె_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_దొరికింది,
                "%u include paths searched:\n",
                xkb_context_అంకె_include_paths(ctx));
        కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < xkb_context_అంకె_include_paths(ctx); i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_దొరికింది,
                    "\t%s\n",
                    xkb_context_include_మార్గం_get(ctx, i));
    }
    లేకపోతే {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_దొరికింది,
                "There are no include paths to search\n");
    }

    ఒకవేళ (xkb_context_అంకె_failed_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_దొరికింది,
                "%u include paths could not be added:\n",
                xkb_context_అంకె_failed_include_paths(ctx));
        కొరకు (darray_కొలత_t i = 0;
             i < xkb_context_అంకె_failed_include_paths(ctx);
             i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_దొరికింది,
                    "\t%s\n",
                    xkb_context_failed_include_మార్గం_get(ctx, i));
    }
}

స్థిర కొలత_t
expand_percent(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం* మాతృ_file_name,
               స్థిరరాశి అక్షరం* typeDir, అక్షరం *buf, కొలత_t buf_కొలత,
               స్థిరరాశి అక్షరం *name, కొలత_t name_len)
{
    నిర్మాణం scanner s;
    scanner_init(&s, ctx, name, name_len, మాతృ_file_name, ఖాళీ);
    s.buf_pos = 0;

    అయితే (!scanner_eof(&s) && !scanner_eol(&s)) {
        ఒకవేళ (scanner_chr(&s, '%')) {
            ఒకవేళ (scanner_chr(&s, '%')) {
                scanner_buf_append(&s, '%');
            }
            లేకపోతే ఒకవేళ (scanner_chr(&s, 'H')) {
                స్థిరరాశి అక్షరం *home = xkb_context_getenv(ctx, "HOME");
                ఒకవేళ (!home) {
                    scanner_err(&s, XKB_LOG_సందేశం_NO_ID,
                                "%%H was used in an include statement, but the "
                                "HOME environment variable is not set");
                    తిరిగి 0;
                }
                ఒకవేళ (!scanner_buf_appends(&s, home)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_బఫరు_కొలత,
                                "include path after expanding %%H is too long");
                    తిరిగి 0;
                }
            }
            లేకపోతే ఒకవేళ (scanner_chr(&s, 'S')) {
                స్థిరరాశి అక్షరం *default_మూలం =
                    xkb_context_include_మార్గం_get_system_మార్గం(ctx);
                ఒకవేళ (!scanner_buf_appends(&s, default_మూలం) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_బఫరు_కొలత,
                                "include path after expanding %%S is too long");
                    తిరిగి 0;
                }
            }
            లేకపోతే ఒకవేళ (scanner_chr(&s, 'E')) {
                స్థిరరాశి అక్షరం *default_మూలం =
                    xkb_context_include_మార్గం_get_extra_మార్గం(ctx);
                ఒకవేళ (!scanner_buf_appends(&s, default_మూలం) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_బఫరు_కొలత,
                                "include path after expanding %%E is too long");
                    తిరిగి 0;
                }
            }
            లేకపోతే {
                scanner_err(&s, XKB_ERROR_INSUFFICIENT_బఫరు_కొలత,
                            "unknown %% format (%c) in include statement",
                            scanner_peek(&s));
                తిరిగి 0;
            }
        }
        లేకపోతే {
            scanner_buf_append(&s, scanner_తదుపరి(&s));
        }
    }
    ఒకవేళ (!scanner_buf_append(&s, '\0')) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_బఫరు_కొలత,
                    "include path is too long; max: %zu", పరిమాణం(s.buf));
        తిరిగి 0;
    }
    ఒకవేళ (unlikely(s.buf_pos > buf_కొలత)) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_బఫరు_కొలత,
                    "include path is too long: %zu > %zu", s.buf_pos, buf_కొలత);
        తిరిగి 0;
    }
    జ్ఞాపకం_నకలు(buf, s.buf, s.buf_pos);
    తిరిగి s.buf_pos;
}

ssize_t
expand_మార్గం(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం* మాతృ_file_name,
            స్థిరరాశి అక్షరం *name, కొలత_t name_len, గణన xkb_file_type type,
            అక్షరం *buf, కొలత_t buf_కొలత)
{
    /* Detect %-expansion */
    కొలత_t k;
    కొరకు (k = 0; k < name_len; k++) {
        ఒకవేళ (name[k] == '%') {
            వెళ్ళు expand;
        }
    }

    తిరిగి 0;

expand:
    /* Process %-expansion */

    /* Copy prefix (usual case: k == 0) */
    ఒకవేళ (unlikely(k >= buf_కొలత)) {
        log_err(ctx, XKB_ERROR_INVALID_మార్గం,
                "Path is too long: %zu > %zu, "
                "got raw path: %.*s\n",
                k, buf_కొలత, (గుర్తులేని పూర్ణాంకం) name_len, name);
        తిరిగి -1;
    }
    జ్ఞాపకం_నకలు(buf, name, k);

    /* Proceed to %-expansion */
    స్థిరరాశి అక్షరం *typeDir = DirectoryForInclude(type);
    కొలత_t లెక్క = expand_percent(ctx, మాతృ_file_name, typeDir,
                                  buf + k, buf_కొలత - k,
                                  name + k, name_len - k);
    ఒకవేళ (!లెక్క)
        తిరిగి -1;
    లెక్క += k;
    ప్రతిజ్ఞా(buf[లెక్క - 1] == '\0');
    తిరిగి (ssize_t) లెక్క - 1;
}

/**
 * Return an open file handle to the first file (counting from offset) with the
 * given name in the include paths, starting at the offset.
 *
 * offset must be zero the first time this is called and is set to the index the
 * file was found. Call again with offset+1 to keep searching through the
 * include paths.
 *
 * If this function returns NULL, no more files are available.
 */
FILE *
కనుగొను_File_లోపల_Xkb_మార్గం(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం* మాతృ_file_name,
                  స్థిరరాశి అక్షరం *name, కొలత_t name_len, గణన xkb_file_type type,
                  అక్షరం *buf, కొలత_t buf_కొలత, గుర్తులేని పూర్ణాంకం *స్థానభ్రంశం,
                  bool required)
{
    /* We do not handle absolute paths here */
    ప్రతిజ్ఞా(!is_absolute_మార్గం(name));

    FILE *file = ఖాళీ;
    అక్షరం *name_బఫరు = ఖాళీ;
    స్థిరరాశి అక్షరం *typeDir = DirectoryForInclude(type);

    కొరకు (గుర్తులేని పూర్ణాంకం i = *స్థానభ్రంశం; i < xkb_context_అంకె_include_paths(ctx); i++) {
        ఒకవేళ (!snprintf_సురక్షిత(buf, buf_కొలత, "%s/%s/%.*s",
                           xkb_context_include_మార్గం_get(ctx, i),
                           typeDir, (గుర్తులేని పూర్ణాంకం) name_len, name)) {
            log_err(ctx, XKB_ERROR_INVALID_మార్గం,
                    "Path is too long: expected max length of %zu, "
                    "got: %s/%s/%.*s\n",
                    buf_కొలత, xkb_context_include_మార్గం_get(ctx, i),
                    typeDir, (గుర్తులేని పూర్ణాంకం) name_len, name);
            కొనసాగు;
        }

        file = fopen(buf, "rb");
        ఒకవేళ (file) {
            *స్థానభ్రంశం = i;
            వెళ్ళు బహిర్గతం;
        }
    }

    /* We only print warnings if we can’t find the file on the first lookup */
    ఒకవేళ (required && *స్థానభ్రంశం == 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_దొరికింది,
                "Couldn't find file \"%s/%.*s\" in include paths\n",
                typeDir, (గుర్తులేని పూర్ణాంకం) name_len, name);
        LogIncludePaths(ctx);
    }

బహిర్గతం:
    విడుదల(name_బఫరు);
    తిరిగి file;
}

bool
Exceeds_Include_Max_లోతు(నిర్మాణం xkb_context *ctx, గుర్తులేని పూర్ణాంకం include_లోతు)
{
    ఒకవేళ (include_లోతు >= INCLUDE_MAX_లోతు) {
        log_err(ctx, XKB_ERROR_RECURSIVE_INCLUDE,
                "Exceeded include depth threshold (%u)",
                INCLUDE_MAX_లోతు);
        తిరిగి true;
    } లేకపోతే {
        తిరిగి false;
    }
}

XkbFile *
ProcessIncludeFile(నిర్మాణం xkb_context *ctx,
                   స్థిరరాశి నిర్మాణం parser_keymap_config *config,
                   స్థిరరాశి IncludeStmt *stmt,
                   గణన xkb_file_type file_type, అక్షరం *మార్గం, కొలత_t మార్గం_కొలత)
{
    /*
     * Resolve include statement:
     * - Explicit map name: look for an *exact match* only.
     * - Default map: look for an *explicit default* map (i.e. tagged with
     *   `default`: exact match), else fallback to the first *implicit* default
     *   map (weak match).
     */
    XkbFile *xkb_file = ఖాళీ;  /* Exact match */
    XkbFile *candidate = ఖాళీ; /* Weak match */

    స్థిరరాశి అక్షరం *stmt_file = stmt->file;
    కొలత_t stmt_file_len = అక్షరశ్రేణి_పొడవు(stmt_file);

    /* Process %-expansion, if any */
    // FIXME: use parent file name instead of “(unknow)”.
    స్థిరరాశి ssize_t expanded = expand_మార్గం(ctx, "(unknown)",
                                         stmt_file, stmt_file_len, file_type,
                                         మార్గం, మార్గం_కొలత);
    ఒకవేళ (expanded < 0) {
        /* Error */
        తిరిగి ఖాళీ;
    } లేకపోతే ఒకవేళ (expanded > 0) {
        /* %-expanded */
        stmt_file = మార్గం;
        stmt_file_len = (కొలత_t) expanded;
        ప్రతిజ్ఞా(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    గుర్తులేని పూర్ణాంకం స్థానభ్రంశం = 0;
    స్థిరరాశి bool absolute_మార్గం = is_absolute_మార్గం(stmt_file);
    ఒకవేళ (absolute_మార్గం) {
        /* Absolute path: no need for lookup in XKB paths */
        ప్రతిజ్ఞా(stmt_file[stmt_file_len] == '\0');
        file = fopen(stmt_file, "rb");
    } లేకపోతే {
        /* Relative path: lookup the first XKB path */
        ఒకవేళ (unlikely(expanded)) {
            /*
             * Relative path after expansion
             *
             * Unlikely to happen, because %-expansion is meant to use absolute
             * paths. Considering that:
             * - we do not resolve paths before expansion, leading to unexpected
             *   result here;
             * - we need the buffer afterwards, but it currently contains the
             *   expanded path;
             * - this is an edge case;
             * we simply make the lookup fail.
             */
            file = ఖాళీ;
        } లేకపోతే {
            // FIXME: use parent file name instead of “(unknow)”.
            file = కనుగొను_File_లోపల_Xkb_మార్గం(ctx, "(unknown)",
                                     stmt_file, stmt_file_len, file_type,
                                     మార్గం, మార్గం_కొలత, &స్థానభ్రంశం, true);
        }
    }

    అయితే (file) {
        xkb_file = XkbParseFile(ctx, config, file, stmt->file, stmt->map);
        fclose(file);

        ఒకవేళ (xkb_file) {
            ఒకవేళ (xkb_file->file_type != file_type) {
                log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                        "Include file of wrong type (expected %s, got %s); "
                        "Include file \"%s\" ignored\n",
                        xkb_file_type_గమ్యం_string(file_type),
                        xkb_file_type_గమ్యం_string(xkb_file->file_type), stmt->file);
                FreeXkbFile(xkb_file);
                xkb_file = ఖాళీ;
            } లేకపోతే ఒకవేళ (stmt->map || (xkb_file->flags & MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops here.
                 */
                ఆపు;
            } లేకపోతే ఒకవేళ (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = xkb_file;
                xkb_file = ఖాళీ;
            } లేకపోతే {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                FreeXkbFile(xkb_file);
                xkb_file = ఖాళీ;
            }
        }

        ఒకవేళ (absolute_మార్గం) {
            /* There is no point to search further if the path is absolute */
            ఆపు;
        }

        స్థానభ్రంశం++;
        file = కనుగొను_File_లోపల_Xkb_మార్గం(ctx, "(unknown)",
                                 stmt_file, stmt_file_len, file_type,
                                 మార్గం, మార్గం_కొలత, &స్థానభ్రంశం, true);
    }

    ఒకవేళ (!xkb_file) {
        /* No exact match: use weak match, if any */
        xkb_file = candidate;
    } లేకపోతే {
        /* Found exact match: discard weak match, if any */
        FreeXkbFile(candidate);
    }

    ఒకవేళ (!xkb_file) {
        ఒకవేళ (stmt->map)
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s(%s)'\n",
                    stmt->file, stmt->map);
        లేకపోతే
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s'\n",
                    stmt->file);
    }

    తిరిగి xkb_file;
}
