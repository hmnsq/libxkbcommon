/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#ضمن "config.h"

#ضمن <errno.h>
#ضمن <limits.h>
#ضمن <stdio.h>
#ضمن <string.h>

#ضمن "messages-codes.h"
#ضمن "utils.h"
#ضمن "xkbcomp-priv.h"
#ضمن "include.h"
#ضمن "scanner-utils.h"
#ضمن "util-paths.h"

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
ParseIncludeMap(حرف **str_inout, حرف **file_rtrn, حرف **map_rtrn,
                حرف *nextop_rtrn, حرف **extra_data)
{
    حرف *tmp, *str, *next;

    str = *str_inout;

    /*
     * Find the position in the string where the next file is included,
     * if there is more than one left in the statement.
     */
    next = strpbrk(str, MERGE_MODE_PREFIXES);
    𐤀𐤌 (next) {
        /* Got more files, this function will be called again. */
        *nextop_rtrn = *next;
        /* Separate the string, for strchr etc. to work on this file only. */
        *next++ = '\0';
    }
    وإلا {
        /* This is the last file in this statement, won't be called again. */
        *nextop_rtrn = '\0';
        next = عدم;
    }

    /*
     * Search for the explicit group designator, if any. If it's there,
     * it goes after the file name and map.
     */
    tmp = strchr(str, ':');
    𐤀𐤌 (tmp != عدم) {
        *tmp++ = '\0';
        *extra_data = strdup(tmp);
    }
    وإلا {
        *extra_data = عدم;
    }

    /* Look for a map, if any. */
    tmp = strchr(str, '(');
    𐤀𐤌 (tmp == عدم) {
        /* No map. */
        *file_rtrn = strdup(str);
        *map_rtrn = عدم;
    }
    وإلا 𐤀𐤌 (str[0] == '(') {
        /* Map without file - invalid. */
        حرر(*extra_data);
        أرجع false;
    }
    وإلا {
        /* Got a map; separate the file and the map for the strdup's. */
        *tmp++ = '\0';
        *file_rtrn = strdup(str);
        str = tmp;
        tmp = strchr(str, ')');
        𐤀𐤌 (tmp == عدم || tmp[1] != '\0') {
            حرر(*file_rtrn);
            حرر(*extra_data);
            أرجع false;
        }
        *tmp++ = '\0';
        *map_rtrn = strdup(str);
    }

    /* Set up the next file for the next call, if any. */
    𐤀𐤌 (*nextop_rtrn == '\0')
        *str_inout = عدم;
    وإلا 𐤀𐤌 (is_merge_mode_prefix(*nextop_rtrn))
        *str_inout = next;
    وإلا
        أرجع false;

    أرجع true;
}

ساكن ثابت حرف *xkb_file_type_include_dirs[_FILE_TYPE_NUM_ENTRIES] = {
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
ساكن ثابت حرف *
DirectoryForInclude(تعداد xkb_file_type type)
{
    𐤀𐤌 (type >= _FILE_TYPE_NUM_ENTRIES)
        أرجع "";
    أرجع xkb_file_type_include_dirs[type];
}

ساكن فراغ
LogIncludePaths(بنية xkb_context *ctx)
{
    𐤀𐤌 (xkb_context_num_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "%u include paths searched:\n",
                xkb_context_num_include_paths(ctx));
        لكل (unsigned صحيح i = 0; i < xkb_context_num_include_paths(ctx); i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "\t%s\n",
                    xkb_context_include_path_get(ctx, i));
    }
    وإلا {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "There are no include paths to search\n");
    }

    𐤀𐤌 (xkb_context_num_failed_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "%u include paths could not be added:\n",
                xkb_context_num_failed_include_paths(ctx));
        لكل (darray_size_t i = 0;
             i < xkb_context_num_failed_include_paths(ctx);
             i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "\t%s\n",
                    xkb_context_failed_include_path_get(ctx, i));
    }
}

ساكن size_t
expand_percent(بنية xkb_context *ctx, ثابت حرف* parent_file_name,
               ثابت حرف* typeDir, حرف *buf, size_t buf_size,
               ثابت حرف *name, size_t name_len)
{
    بنية scanner s;
    scanner_init(&s, ctx, name, name_len, parent_file_name, عدم);
    s.buf_pos = 0;

    طالما (!scanner_eof(&s) && !scanner_eol(&s)) {
        𐤀𐤌 (scanner_chr(&s, '%')) {
            𐤀𐤌 (scanner_chr(&s, '%')) {
                scanner_buf_append(&s, '%');
            }
            وإلا 𐤀𐤌 (scanner_chr(&s, 'H')) {
                ثابت حرف *home = xkb_context_getenv(ctx, "HOME");
                𐤀𐤌 (!home) {
                    scanner_err(&s, XKB_LOG_MESSAGE_NO_ID,
                                "%%H was used in an include statement, but the "
                                "HOME environment variable is not set");
                    أرجع 0;
                }
                𐤀𐤌 (!scanner_buf_appends(&s, home)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%H is too long");
                    أرجع 0;
                }
            }
            وإلا 𐤀𐤌 (scanner_chr(&s, 'S')) {
                ثابت حرف *default_root =
                    xkb_context_include_path_get_system_path(ctx);
                𐤀𐤌 (!scanner_buf_appends(&s, default_root) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%S is too long");
                    أرجع 0;
                }
            }
            وإلا 𐤀𐤌 (scanner_chr(&s, 'E')) {
                ثابت حرف *default_root =
                    xkb_context_include_path_get_extra_path(ctx);
                𐤀𐤌 (!scanner_buf_appends(&s, default_root) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%E is too long");
                    أرجع 0;
                }
            }
            وإلا {
                scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                            "unknown %% format (%c) in include statement",
                            scanner_peek(&s));
                أرجع 0;
            }
        }
        وإلا {
            scanner_buf_append(&s, scanner_next(&s));
        }
    }
    𐤀𐤌 (!scanner_buf_append(&s, '\0')) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                    "include path is too long; max: %zu", حجم(s.buf));
        أرجع 0;
    }
    𐤀𐤌 (unlikely(s.buf_pos > buf_size)) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                    "include path is too long: %zu > %zu", s.buf_pos, buf_size);
        أرجع 0;
    }
    memcpy(buf, s.buf, s.buf_pos);
    أرجع s.buf_pos;
}

ssize_t
expand_path(بنية xkb_context *ctx, ثابت حرف* parent_file_name,
            ثابت حرف *name, size_t name_len, تعداد xkb_file_type type,
            حرف *buf, size_t buf_size)
{
    /* Detect %-expansion */
    size_t k;
    لكل (k = 0; k < name_len; k++) {
        𐤀𐤌 (name[k] == '%') {
            اذهب expand;
        }
    }

    أرجع 0;

expand:
    /* Process %-expansion */

    /* Copy prefix (usual case: k == 0) */
    𐤀𐤌 (unlikely(k >= buf_size)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: %zu > %zu, "
                "got raw path: %.*s\n",
                k, buf_size, (unsigned صحيح) name_len, name);
        أرجع -1;
    }
    memcpy(buf, name, k);

    /* Proceed to %-expansion */
    ثابت حرف *typeDir = DirectoryForInclude(type);
    size_t count = expand_percent(ctx, parent_file_name, typeDir,
                                  buf + k, buf_size - k,
                                  name + k, name_len - k);
    𐤀𐤌 (!count)
        أرجع -1;
    count += k;
    أكد(buf[count - 1] == '\0');
    أرجع (ssize_t) count - 1;
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
FindFileInXkbPath(بنية xkb_context *ctx, ثابت حرف* parent_file_name,
                  ثابت حرف *name, size_t name_len, تعداد xkb_file_type type,
                  حرف *buf, size_t buf_size, unsigned صحيح *offset,
                  bool required)
{
    /* We do not handle absolute paths here */
    أكد(!is_absolute_path(name));

    FILE *file = عدم;
    حرف *name_buffer = عدم;
    ثابت حرف *typeDir = DirectoryForInclude(type);

    لكل (unsigned صحيح i = *offset; i < xkb_context_num_include_paths(ctx); i++) {
        𐤀𐤌 (!snprintf_safe(buf, buf_size, "%s/%s/%.*s",
                           xkb_context_include_path_get(ctx, i),
                           typeDir, (unsigned صحيح) name_len, name)) {
            log_err(ctx, XKB_ERROR_INVALID_PATH,
                    "Path is too long: expected max length of %zu, "
                    "got: %s/%s/%.*s\n",
                    buf_size, xkb_context_include_path_get(ctx, i),
                    typeDir, (unsigned صحيح) name_len, name);
            استمر;
        }

        file = fopen(buf, "rb");
        𐤀𐤌 (file) {
            *offset = i;
            اذهب out;
        }
    }

    /* We only print warnings if we can’t find the file on the first lookup */
    𐤀𐤌 (required && *offset == 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "Couldn't find file \"%s/%.*s\" in include paths\n",
                typeDir, (unsigned صحيح) name_len, name);
        LogIncludePaths(ctx);
    }

out:
    حرر(name_buffer);
    أرجع file;
}

bool
ExceedsIncludeMaxDepth(بنية xkb_context *ctx, unsigned صحيح include_depth)
{
    𐤀𐤌 (include_depth >= INCLUDE_MAX_DEPTH) {
        log_err(ctx, XKB_ERROR_RECURSIVE_INCLUDE,
                "Exceeded include depth threshold (%u)",
                INCLUDE_MAX_DEPTH);
        أرجع true;
    } وإلا {
        أرجع false;
    }
}

XkbFile *
ProcessIncludeFile(بنية xkb_context *ctx,
                   ثابت بنية parser_keymap_config *config,
                   ثابت IncludeStmt *stmt,
                   تعداد xkb_file_type file_type, حرف *path, size_t path_size)
{
    /*
     * Resolve include statement:
     * - Explicit map name: look for an *exact match* only.
     * - Default map: look for an *explicit default* map (i.e. tagged with
     *   `default`: exact match), else fallback to the first *implicit* default
     *   map (weak match).
     */
    XkbFile *xkb_file = عدم;  /* Exact match */
    XkbFile *candidate = عدم; /* Weak match */

    ثابت حرف *stmt_file = stmt->file;
    size_t stmt_file_len = strlen(stmt_file);

    /* Process %-expansion, if any */
    // FIXME: use parent file name instead of “(unknow)”.
    ثابت ssize_t expanded = expand_path(ctx, "(unknown)",
                                         stmt_file, stmt_file_len, file_type,
                                         path, path_size);
    𐤀𐤌 (expanded < 0) {
        /* Error */
        أرجع عدم;
    } وإلا 𐤀𐤌 (expanded > 0) {
        /* %-expanded */
        stmt_file = path;
        stmt_file_len = (size_t) expanded;
        أكد(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    unsigned صحيح offset = 0;
    ثابت bool absolute_path = is_absolute_path(stmt_file);
    𐤀𐤌 (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        أكد(stmt_file[stmt_file_len] == '\0');
        file = fopen(stmt_file, "rb");
    } وإلا {
        /* Relative path: lookup the first XKB path */
        𐤀𐤌 (unlikely(expanded)) {
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
            file = عدم;
        } وإلا {
            // FIXME: use parent file name instead of “(unknow)”.
            file = FindFileInXkbPath(ctx, "(unknown)",
                                     stmt_file, stmt_file_len, file_type,
                                     path, path_size, &offset, true);
        }
    }

    طالما (file) {
        xkb_file = XkbParseFile(ctx, config, file, stmt->file, stmt->map);
        fclose(file);

        𐤀𐤌 (xkb_file) {
            𐤀𐤌 (xkb_file->file_type != file_type) {
                log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                        "Include file of wrong type (expected %s, got %s); "
                        "Include file \"%s\" ignored\n",
                        xkb_file_type_to_string(file_type),
                        xkb_file_type_to_string(xkb_file->file_type), stmt->file);
                FreeXkbFile(xkb_file);
                xkb_file = عدم;
            } وإلا 𐤀𐤌 (stmt->map || (xkb_file->flags & MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops here.
                 */
                اكسر;
            } وإلا 𐤀𐤌 (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = xkb_file;
                xkb_file = عدم;
            } وإلا {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                FreeXkbFile(xkb_file);
                xkb_file = عدم;
            }
        }

        𐤀𐤌 (absolute_path) {
            /* There is no point to search further if the path is absolute */
            اكسر;
        }

        offset++;
        file = FindFileInXkbPath(ctx, "(unknown)",
                                 stmt_file, stmt_file_len, file_type,
                                 path, path_size, &offset, true);
    }

    𐤀𐤌 (!xkb_file) {
        /* No exact match: use weak match, if any */
        xkb_file = candidate;
    } وإلا {
        /* Found exact match: discard weak match, if any */
        FreeXkbFile(candidate);
    }

    𐤀𐤌 (!xkb_file) {
        𐤀𐤌 (stmt->map)
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s(%s)'\n",
                    stmt->file, stmt->map);
        وإلا
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s'\n",
                    stmt->file);
    }

    أرجع xkb_file;
}
