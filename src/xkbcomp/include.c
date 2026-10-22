/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#include "config.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "messages-codes.h"
#include "utils.h"
#include "xkbcomp-priv.h"
#include "include.h"
#include "scanner-utils.h"
#include "util-paths.h"

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
ParseIncludeMap(char **str_inout, char **file_rtrn, char **map_rtrn,
                char *nextop_rtrn, char **extra_data)
{
    char *tmp, *str, *next;

    str = *str_inout;

    /*
     * Find the position in the string where the next file is included,
     * if there is more than one left in the statement.
     */
    next = strpbrk(str, MERGE_MODE_PREFIXES);
    nếu (next) {
        /* Got more files, this function will be called again. */
        *nextop_rtrn = *next;
        /* Separate the string, for strchr etc. to work on this file only. */
        *next++ = '\0';
    }
    không_thì {
        /* This is the last file in this statement, won't be called again. */
        *nextop_rtrn = '\0';
        next = NULL;
    }

    /*
     * Search for the explicit group designator, if any. If it's there,
     * it goes after the file name and map.
     */
    tmp = strchr(str, ':');
    nếu (tmp != NULL) {
        *tmp++ = '\0';
        *extra_data = strdup(tmp);
    }
    không_thì {
        *extra_data = NULL;
    }

    /* Look for a map, if any. */
    tmp = strchr(str, '(');
    nếu (tmp == NULL) {
        /* No map. */
        *file_rtrn = strdup(str);
        *map_rtrn = NULL;
    }
    không_thì nếu (str[0] == '(') {
        /* Map without file - invalid. */
        free(*extra_data);
        trả_về false;
    }
    không_thì {
        /* Got a map; separate the file and the map for the strdup's. */
        *tmp++ = '\0';
        *file_rtrn = strdup(str);
        str = tmp;
        tmp = strchr(str, ')');
        nếu (tmp == NULL || tmp[1] != '\0') {
            free(*file_rtrn);
            free(*extra_data);
            trả_về false;
        }
        *tmp++ = '\0';
        *map_rtrn = strdup(str);
    }

    /* Set up the next file for the next call, if any. */
    nếu (*nextop_rtrn == '\0')
        *str_inout = NULL;
    không_thì nếu (is_merge_mode_prefix(*nextop_rtrn))
        *str_inout = next;
    không_thì
        trả_về false;

    trả_về true;
}

static const char *xkb_file_type_include_dirs[_FILE_TYPE_NUM_ENTRIES] = {
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
static const char *
DirectoryForInclude(enum xkb_file_type type)
{
    nếu (type >= _FILE_TYPE_NUM_ENTRIES)
        trả_về "";
    trả_về xkb_file_type_include_dirs[type];
}

static void
LogIncludePaths(struct xkb_context *ctx)
{
    nếu (xkb_context_num_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "%u include paths searched:\n",
                xkb_context_num_include_paths(ctx));
        cho (unsigned số_nguyên i = 0; i < xkb_context_num_include_paths(ctx); i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "\t%s\n",
                    xkb_context_include_path_get(ctx, i));
    }
    không_thì {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "There are no include paths to search\n");
    }

    nếu (xkb_context_num_failed_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "%u include paths could not be added:\n",
                xkb_context_num_failed_include_paths(ctx));
        cho (darray_size_t i = 0;
             i < xkb_context_num_failed_include_paths(ctx);
             i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "\t%s\n",
                    xkb_context_failed_include_path_get(ctx, i));
    }
}

static size_t
expand_percent(struct xkb_context *ctx, const char* parent_file_name,
               const char* typeDir, char *buf, size_t buf_size,
               const char *name, size_t name_len)
{
    struct scanner s;
    scanner_init(&s, ctx, name, name_len, parent_file_name, NULL);
    s.buf_pos = 0;

    trong_khi (!scanner_eof(&s) && !scanner_eol(&s)) {
        nếu (scanner_chr(&s, '%')) {
            nếu (scanner_chr(&s, '%')) {
                scanner_buf_append(&s, '%');
            }
            không_thì nếu (scanner_chr(&s, 'H')) {
                const char *home = xkb_context_getenv(ctx, "HOME");
                nếu (!home) {
                    scanner_err(&s, XKB_LOG_MESSAGE_NO_ID,
                                "%%H was used in an include statement, but the "
                                "HOME environment variable is not set");
                    trả_về 0;
                }
                nếu (!scanner_buf_appends(&s, home)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%H is too long");
                    trả_về 0;
                }
            }
            không_thì nếu (scanner_chr(&s, 'S')) {
                const char *default_root =
                    xkb_context_include_path_get_system_path(ctx);
                nếu (!scanner_buf_appends(&s, default_root) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%S is too long");
                    trả_về 0;
                }
            }
            không_thì nếu (scanner_chr(&s, 'E')) {
                const char *default_root =
                    xkb_context_include_path_get_extra_path(ctx);
                nếu (!scanner_buf_appends(&s, default_root) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%E is too long");
                    trả_về 0;
                }
            }
            không_thì {
                scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                            "unknown %% format (%c) in include statement",
                            scanner_peek(&s));
                trả_về 0;
            }
        }
        không_thì {
            scanner_buf_append(&s, scanner_next(&s));
        }
    }
    nếu (!scanner_buf_append(&s, '\0')) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                    "include path is too long; max: %zu", sizeof(s.buf));
        trả_về 0;
    }
    nếu (unlikely(s.buf_pos > buf_size)) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                    "include path is too long: %zu > %zu", s.buf_pos, buf_size);
        trả_về 0;
    }
    memcpy(buf, s.buf, s.buf_pos);
    trả_về s.buf_pos;
}

ssize_t
expand_path(struct xkb_context *ctx, const char* parent_file_name,
            const char *name, size_t name_len, enum xkb_file_type type,
            char *buf, size_t buf_size)
{
    /* Detect %-expansion */
    size_t k;
    cho (k = 0; k < name_len; k++) {
        nếu (name[k] == '%') {
            goto expand;
        }
    }

    trả_về 0;

expand:
    /* Process %-expansion */

    /* Copy prefix (usual case: k == 0) */
    nếu (unlikely(k >= buf_size)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: %zu > %zu, "
                "got raw path: %.*s\n",
                k, buf_size, (unsigned số_nguyên) name_len, name);
        trả_về -1;
    }
    memcpy(buf, name, k);

    /* Proceed to %-expansion */
    const char *typeDir = DirectoryForInclude(type);
    size_t count = expand_percent(ctx, parent_file_name, typeDir,
                                  buf + k, buf_size - k,
                                  name + k, name_len - k);
    nếu (!count)
        trả_về -1;
    count += k;
    assert(buf[count - 1] == '\0');
    trả_về (ssize_t) count - 1;
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
FindFileInXkbPath(struct xkb_context *ctx, const char* parent_file_name,
                  const char *name, size_t name_len, enum xkb_file_type type,
                  char *buf, size_t buf_size, unsigned số_nguyên *offset,
                  bool required)
{
    /* We do not handle absolute paths here */
    assert(!is_absolute_path(name));

    FILE *file = NULL;
    char *name_buffer = NULL;
    const char *typeDir = DirectoryForInclude(type);

    cho (unsigned số_nguyên i = *offset; i < xkb_context_num_include_paths(ctx); i++) {
        nếu (!snprintf_safe(buf, buf_size, "%s/%s/%.*s",
                           xkb_context_include_path_get(ctx, i),
                           typeDir, (unsigned số_nguyên) name_len, name)) {
            log_err(ctx, XKB_ERROR_INVALID_PATH,
                    "Path is too long: expected max length of %zu, "
                    "got: %s/%s/%.*s\n",
                    buf_size, xkb_context_include_path_get(ctx, i),
                    typeDir, (unsigned số_nguyên) name_len, name);
            continue;
        }

        file = fopen(buf, "rb");
        nếu (file) {
            *offset = i;
            goto out;
        }
    }

    /* We only print warnings if we can’t find the file on the first lookup */
    nếu (required && *offset == 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "Couldn't find file \"%s/%.*s\" in include paths\n",
                typeDir, (unsigned số_nguyên) name_len, name);
        LogIncludePaths(ctx);
    }

out:
    free(name_buffer);
    trả_về file;
}

bool
ExceedsIncludeMaxDepth(struct xkb_context *ctx, unsigned số_nguyên include_depth)
{
    nếu (include_depth >= INCLUDE_MAX_DEPTH) {
        log_err(ctx, XKB_ERROR_RECURSIVE_INCLUDE,
                "Exceeded include depth threshold (%u)",
                INCLUDE_MAX_DEPTH);
        trả_về true;
    } không_thì {
        trả_về false;
    }
}

XkbFile *
ProcessIncludeFile(struct xkb_context *ctx,
                   const struct parser_keymap_config *config,
                   const IncludeStmt *stmt,
                   enum xkb_file_type file_type, char *path, size_t path_size)
{
    /*
     * Resolve include statement:
     * - Explicit map name: look for an *exact match* only.
     * - Default map: look for an *explicit default* map (i.e. tagged with
     *   `default`: exact match), else fallback to the first *implicit* default
     *   map (weak match).
     */
    XkbFile *xkb_file = NULL;  /* Exact match */
    XkbFile *candidate = NULL; /* Weak match */

    const char *stmt_file = stmt->file;
    size_t stmt_file_len = strlen(stmt_file);

    /* Process %-expansion, if any */
    // FIXME: use parent file name instead of “(unknow)”.
    const ssize_t expanded = expand_path(ctx, "(unknown)",
                                         stmt_file, stmt_file_len, file_type,
                                         path, path_size);
    nếu (expanded < 0) {
        /* Error */
        trả_về NULL;
    } không_thì nếu (expanded > 0) {
        /* %-expanded */
        stmt_file = path;
        stmt_file_len = (size_t) expanded;
        assert(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    unsigned số_nguyên offset = 0;
    const bool absolute_path = is_absolute_path(stmt_file);
    nếu (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        assert(stmt_file[stmt_file_len] == '\0');
        file = fopen(stmt_file, "rb");
    } không_thì {
        /* Relative path: lookup the first XKB path */
        nếu (unlikely(expanded)) {
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
            file = NULL;
        } không_thì {
            // FIXME: use parent file name instead of “(unknow)”.
            file = FindFileInXkbPath(ctx, "(unknown)",
                                     stmt_file, stmt_file_len, file_type,
                                     path, path_size, &offset, true);
        }
    }

    trong_khi (file) {
        xkb_file = XkbParseFile(ctx, config, file, stmt->file, stmt->map);
        fclose(file);

        nếu (xkb_file) {
            nếu (xkb_file->file_type != file_type) {
                log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                        "Include file of wrong type (expected %s, got %s); "
                        "Include file \"%s\" ignored\n",
                        xkb_file_type_to_string(file_type),
                        xkb_file_type_to_string(xkb_file->file_type), stmt->file);
                FreeXkbFile(xkb_file);
                xkb_file = NULL;
            } không_thì nếu (stmt->map || (xkb_file->flags & MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops here.
                 */
                break;
            } không_thì nếu (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = xkb_file;
                xkb_file = NULL;
            } không_thì {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                FreeXkbFile(xkb_file);
                xkb_file = NULL;
            }
        }

        nếu (absolute_path) {
            /* There is no point to search further if the path is absolute */
            break;
        }

        offset++;
        file = FindFileInXkbPath(ctx, "(unknown)",
                                 stmt_file, stmt_file_len, file_type,
                                 path, path_size, &offset, true);
    }

    nếu (!xkb_file) {
        /* No exact match: use weak match, if any */
        xkb_file = candidate;
    } không_thì {
        /* Found exact match: discard weak match, if any */
        FreeXkbFile(candidate);
    }

    nếu (!xkb_file) {
        nếu (stmt->map)
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s(%s)'\n",
                    stmt->file, stmt->map);
        không_thì
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s'\n",
                    stmt->file);
    }

    trả_về xkb_file;
}
