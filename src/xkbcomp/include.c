/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>

#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"
#ಅನ್ತರ್ಭಾವ "include.h"
#ಅನ್ತರ್ಭಾವ "scanner-utils.h"
#ಅನ್ತರ್ಭಾವ "util-paths.h"

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
ParseIncludeMap(ವರ್ಣ **str_inout, ವರ್ಣ **file_rtrn, ವರ್ಣ **map_rtrn,
                ವರ್ಣ *nextop_rtrn, ವರ್ಣ **extra_data)
{
    ವರ್ಣ *tmp, *str, *next;

    str = *str_inout;

    /*
     * Find the position in the string where the next file is included,
     * if there is more than one left in the statement.
     */
    next = strpbrk(str, MERGE_MODE_PREFIXES);
    ಯದಿ (next) {
        /* Got more files, this function will be called again. */
        *nextop_rtrn = *next;
        /* Separate the string, for strchr etc. to work on this file only. */
        *next++ = '\0';
    }
    ಅನ್ಯಥಾ {
        /* This is the last file in this statement, won't be called again. */
        *nextop_rtrn = '\0';
        next = ರಿಕ್ತ;
    }

    /*
     * Search for the explicit group designator, if any. If it's there,
     * it goes after the file name and map.
     */
    tmp = strchr(str, ':');
    ಯದಿ (tmp != ರಿಕ್ತ) {
        *tmp++ = '\0';
        *extra_data = strdup(tmp);
    }
    ಅನ್ಯಥಾ {
        *extra_data = ರಿಕ್ತ;
    }

    /* Look for a map, if any. */
    tmp = strchr(str, '(');
    ಯದಿ (tmp == ರಿಕ್ತ) {
        /* No map. */
        *file_rtrn = strdup(str);
        *map_rtrn = ರಿಕ್ತ;
    }
    ಅನ್ಯಥಾ ಯದಿ (str[0] == '(') {
        /* Map without file - invalid. */
        ಮುಕ್ತ(*extra_data);
        ಪ್ರತ್ಯಾಗಮ false;
    }
    ಅನ್ಯಥಾ {
        /* Got a map; separate the file and the map for the strdup's. */
        *tmp++ = '\0';
        *file_rtrn = strdup(str);
        str = tmp;
        tmp = strchr(str, ')');
        ಯದಿ (tmp == ರಿಕ್ತ || tmp[1] != '\0') {
            ಮುಕ್ತ(*file_rtrn);
            ಮುಕ್ತ(*extra_data);
            ಪ್ರತ್ಯಾಗಮ false;
        }
        *tmp++ = '\0';
        *map_rtrn = strdup(str);
    }

    /* Set up the next file for the next call, if any. */
    ಯದಿ (*nextop_rtrn == '\0')
        *str_inout = ರಿಕ್ತ;
    ಅನ್ಯಥಾ ಯದಿ (is_merge_mode_prefix(*nextop_rtrn))
        *str_inout = next;
    ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ false;

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *xkb_file_type_include_dirs[FILE_TYPE_ಅಂಕೆ_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "keycodes",
    [FILE_TYPE_TYPES] = "types",
    [FILE_TYPE_COMPAT] = "compat",
    [FILE_TYPE_SYMBOLS] = "symbols",
    [FILE_TYPE_GEOMETRY] = "geometry",
    [FILE_TYPE_KEYMAP] = "keymap",
    [FILE_TYPE_RULES] = "rules",
};

/**
 * Return the xkb directory based on the type.
 */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
DirectoryForInclude(ಗಣನಾ xkb_file_type type)
{
    ಯದಿ (type >= FILE_TYPE_ಅಂಕೆ_ENTRIES)
        ಪ್ರತ್ಯಾಗಮ "";
    ಪ್ರತ್ಯಾಗಮ xkb_file_type_include_dirs[type];
}

ಸ್ಥಿರ ಶೂನ್ಯ
LogIncludePaths(ಸಂರಚನಾ xkb_context *ctx)
{
    ಯದಿ (xkb_context_ಅಂಕೆ_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "%u include paths searched:\n",
                xkb_context_ಅಂಕೆ_include_paths(ctx));
        ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < xkb_context_ಅಂಕೆ_include_paths(ctx); i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "\t%s\n",
                    xkb_context_include_path_get(ctx, i));
    }
    ಅನ್ಯಥಾ {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "There are no include paths to search\n");
    }

    ಯದಿ (xkb_context_ಅಂಕೆ_failed_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "%u include paths could not be added:\n",
                xkb_context_ಅಂಕೆ_failed_include_paths(ctx));
        ಪ್ರತಿ (darray_size_t i = 0;
             i < xkb_context_ಅಂಕೆ_failed_include_paths(ctx);
             i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "\t%s\n",
                    xkb_context_failed_include_path_get(ctx, i));
    }
}

ಸ್ಥಿರ size_t
expand_percent(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ* parent_file_name,
               ಧ್ರುವ ವರ್ಣ* typeDir, ವರ್ಣ *buf, size_t buf_size,
               ಧ್ರುವ ವರ್ಣ *name, size_t name_len)
{
    ಸಂರಚನಾ scanner s;
    scanner_init(&s, ctx, name, name_len, parent_file_name, ರಿಕ್ತ);
    s.buf_pos = 0;

    ಯಾವತ್ (!scanner_eof(&s) && !scanner_eol(&s)) {
        ಯದಿ (scanner_chr(&s, '%')) {
            ಯದಿ (scanner_chr(&s, '%')) {
                scanner_buf_append(&s, '%');
            }
            ಅನ್ಯಥಾ ಯದಿ (scanner_chr(&s, 'H')) {
                ಧ್ರುವ ವರ್ಣ *home = xkb_context_getenv(ctx, "HOME");
                ಯದಿ (!home) {
                    scanner_err(&s, XKB_LOG_MESSAGE_NO_ID,
                                "%%H was used in an include statement, but the "
                                "HOME environment variable is not set");
                    ಪ್ರತ್ಯಾಗಮ 0;
                }
                ಯದಿ (!scanner_buf_appends(&s, home)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%H is too long");
                    ಪ್ರತ್ಯಾಗಮ 0;
                }
            }
            ಅನ್ಯಥಾ ಯದಿ (scanner_chr(&s, 'S')) {
                ಧ್ರುವ ವರ್ಣ *default_ಬೇರು =
                    xkb_context_include_path_get_system_path(ctx);
                ಯದಿ (!scanner_buf_appends(&s, default_ಬೇರು) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%S is too long");
                    ಪ್ರತ್ಯಾಗಮ 0;
                }
            }
            ಅನ್ಯಥಾ ಯದಿ (scanner_chr(&s, 'E')) {
                ಧ್ರುವ ವರ್ಣ *default_ಬೇರು =
                    xkb_context_include_path_get_extra_path(ctx);
                ಯದಿ (!scanner_buf_appends(&s, default_ಬೇರು) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%E is too long");
                    ಪ್ರತ್ಯಾಗಮ 0;
                }
            }
            ಅನ್ಯಥಾ {
                scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                            "unknown %% format (%c) in include statement",
                            scanner_peek(&s));
                ಪ್ರತ್ಯಾಗಮ 0;
            }
        }
        ಅನ್ಯಥಾ {
            scanner_buf_append(&s, scanner_next(&s));
        }
    }
    ಯದಿ (!scanner_buf_append(&s, '\0')) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                    "include path is too long; max: %zu", ಪರಿಮಾಣ(s.buf));
        ಪ್ರತ್ಯಾಗಮ 0;
    }
    ಯದಿ (unlikely(s.buf_pos > buf_size)) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                    "include path is too long: %zu > %zu", s.buf_pos, buf_size);
        ಪ್ರತ್ಯಾಗಮ 0;
    }
    memcpy(buf, s.buf, s.buf_pos);
    ಪ್ರತ್ಯಾಗಮ s.buf_pos;
}

ssize_t
expand_path(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ* parent_file_name,
            ಧ್ರುವ ವರ್ಣ *name, size_t name_len, ಗಣನಾ xkb_file_type type,
            ವರ್ಣ *buf, size_t buf_size)
{
    /* Detect %-expansion */
    size_t k;
    ಪ್ರತಿ (k = 0; k < name_len; k++) {
        ಯದಿ (name[k] == '%') {
            ಗಚ್ಛ expand;
        }
    }

    ಪ್ರತ್ಯಾಗಮ 0;

expand:
    /* Process %-expansion */

    /* Copy prefix (usual case: k == 0) */
    ಯದಿ (unlikely(k >= buf_size)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: %zu > %zu, "
                "got raw path: %.*s\n",
                k, buf_size, (unsigned ಪೂರ್ಣಾಙ್ಕ) name_len, name);
        ಪ್ರತ್ಯಾಗಮ -1;
    }
    memcpy(buf, name, k);

    /* Proceed to %-expansion */
    ಧ್ರುವ ವರ್ಣ *typeDir = DirectoryForInclude(type);
    size_t ಎಣಿಕೆ = expand_percent(ctx, parent_file_name, typeDir,
                                  buf + k, buf_size - k,
                                  name + k, name_len - k);
    ಯದಿ (!ಎಣಿಕೆ)
        ಪ್ರತ್ಯಾಗಮ -1;
    ಎಣಿಕೆ += k;
    ಪ್ರತಿಜ್ಞಾ(buf[ಎಣಿಕೆ - 1] == '\0');
    ಪ್ರತ್ಯಾಗಮ (ssize_t) ಎಣಿಕೆ - 1;
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
FindFileInXkbPath(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ* parent_file_name,
                  ಧ್ರುವ ವರ್ಣ *name, size_t name_len, ಗಣನಾ xkb_file_type type,
                  ವರ್ಣ *buf, size_t buf_size, unsigned ಪೂರ್ಣಾಙ್ಕ *offset,
                  bool required)
{
    /* We do not handle absolute paths here */
    ಪ್ರತಿಜ್ಞಾ(!is_absolute_path(name));

    FILE *file = ರಿಕ್ತ;
    ವರ್ಣ *name_buffer = ರಿಕ್ತ;
    ಧ್ರುವ ವರ್ಣ *typeDir = DirectoryForInclude(type);

    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = *offset; i < xkb_context_ಅಂಕೆ_include_paths(ctx); i++) {
        ಯದಿ (!snprintf_safe(buf, buf_size, "%s/%s/%.*s",
                           xkb_context_include_path_get(ctx, i),
                           typeDir, (unsigned ಪೂರ್ಣಾಙ್ಕ) name_len, name)) {
            log_err(ctx, XKB_ERROR_INVALID_PATH,
                    "Path is too long: expected max length of %zu, "
                    "got: %s/%s/%.*s\n",
                    buf_size, xkb_context_include_path_get(ctx, i),
                    typeDir, (unsigned ಪೂರ್ಣಾಙ್ಕ) name_len, name);
            ಅನುವರ್ತ;
        }

        file = fopen(buf, "rb");
        ಯದಿ (file) {
            *offset = i;
            ಗಚ್ಛ out;
        }
    }

    /* We only print warnings if we can’t find the file on the first lookup */
    ಯದಿ (required && *offset == 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "Couldn't find file \"%s/%.*s\" in include paths\n",
                typeDir, (unsigned ಪೂರ್ಣಾಙ್ಕ) name_len, name);
        LogIncludePaths(ctx);
    }

out:
    ಮುಕ್ತ(name_buffer);
    ಪ್ರತ್ಯಾಗಮ file;
}

bool
ExceedsIncludeMaxDepth(ಸಂರಚನಾ xkb_context *ctx, unsigned ಪೂರ್ಣಾಙ್ಕ include_depth)
{
    ಯದಿ (include_depth >= INCLUDE_MAX_DEPTH) {
        log_err(ctx, XKB_ERROR_RECURSIVE_INCLUDE,
                "Exceeded include depth threshold (%u)",
                INCLUDE_MAX_DEPTH);
        ಪ್ರತ್ಯಾಗಮ true;
    } ಅನ್ಯಥಾ {
        ಪ್ರತ್ಯಾಗಮ false;
    }
}

XkbFile *
ProcessIncludeFile(ಸಂರಚನಾ xkb_context *ctx,
                   ಧ್ರುವ ಸಂರಚನಾ parser_keymap_config *config,
                   ಧ್ರುವ IncludeStmt *stmt,
                   ಗಣನಾ xkb_file_type file_type, ವರ್ಣ *path, size_t path_size)
{
    /*
     * Resolve include statement:
     * - Explicit map name: look for an *exact match* only.
     * - Default map: look for an *explicit default* map (i.e. tagged with
     *   `default`: exact match), else fallback to the first *implicit* default
     *   map (weak match).
     */
    XkbFile *xkb_file = ರಿಕ್ತ;  /* Exact match */
    XkbFile *candidate = ರಿಕ್ತ; /* Weak match */

    ಧ್ರುವ ವರ್ಣ *stmt_file = stmt->file;
    size_t stmt_file_len = strlen(stmt_file);

    /* Process %-expansion, if any */
    // FIXME: use parent file name instead of “(unknow)”.
    ಧ್ರುವ ssize_t expanded = expand_path(ctx, "(unknown)",
                                         stmt_file, stmt_file_len, file_type,
                                         path, path_size);
    ಯದಿ (expanded < 0) {
        /* Error */
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    } ಅನ್ಯಥಾ ಯದಿ (expanded > 0) {
        /* %-expanded */
        stmt_file = path;
        stmt_file_len = (size_t) expanded;
        ಪ್ರತಿಜ್ಞಾ(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    unsigned ಪೂರ್ಣಾಙ್ಕ offset = 0;
    ಧ್ರುವ bool absolute_path = is_absolute_path(stmt_file);
    ಯದಿ (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        ಪ್ರತಿಜ್ಞಾ(stmt_file[stmt_file_len] == '\0');
        file = fopen(stmt_file, "rb");
    } ಅನ್ಯಥಾ {
        /* Relative path: lookup the first XKB path */
        ಯದಿ (unlikely(expanded)) {
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
            file = ರಿಕ್ತ;
        } ಅನ್ಯಥಾ {
            // FIXME: use parent file name instead of “(unknow)”.
            file = FindFileInXkbPath(ctx, "(unknown)",
                                     stmt_file, stmt_file_len, file_type,
                                     path, path_size, &offset, true);
        }
    }

    ಯಾವತ್ (file) {
        xkb_file = XkbParseFile(ctx, config, file, stmt->file, stmt->map);
        fclose(file);

        ಯದಿ (xkb_file) {
            ಯದಿ (xkb_file->file_type != file_type) {
                log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                        "Include file of wrong type (expected %s, got %s); "
                        "Include file \"%s\" ignored\n",
                        xkb_file_type_to_string(file_type),
                        xkb_file_type_to_string(xkb_file->file_type), stmt->file);
                FreeXkbFile(xkb_file);
                xkb_file = ರಿಕ್ತ;
            } ಅನ್ಯಥಾ ಯದಿ (stmt->map || (xkb_file->flags & MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops here.
                 */
                ಭಙ್ಗ;
            } ಅನ್ಯಥಾ ಯದಿ (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = xkb_file;
                xkb_file = ರಿಕ್ತ;
            } ಅನ್ಯಥಾ {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                FreeXkbFile(xkb_file);
                xkb_file = ರಿಕ್ತ;
            }
        }

        ಯದಿ (absolute_path) {
            /* There is no point to search further if the path is absolute */
            ಭಙ್ಗ;
        }

        offset++;
        file = FindFileInXkbPath(ctx, "(unknown)",
                                 stmt_file, stmt_file_len, file_type,
                                 path, path_size, &offset, true);
    }

    ಯದಿ (!xkb_file) {
        /* No exact match: use weak match, if any */
        xkb_file = candidate;
    } ಅನ್ಯಥಾ {
        /* Found exact match: discard weak match, if any */
        FreeXkbFile(candidate);
    }

    ಯದಿ (!xkb_file) {
        ಯದಿ (stmt->map)
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s(%s)'\n",
                    stmt->file, stmt->map);
        ಅನ್ಯಥಾ
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s'\n",
                    stmt->file);
    }

    ಪ್ರತ್ಯಾಗಮ xkb_file;
}
