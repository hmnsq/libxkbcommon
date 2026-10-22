/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <limits.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>

#অন্তর্ভাব "messages-codes.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "xkbcomp-priv.h"
#অন্তর্ভাব "include.h"
#অন্তর্ভাব "scanner-utils.h"
#অন্তর্ভাব "util-paths.h"

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
ParseIncludeMap(বর্ণ **str_inout, বর্ণ **file_rtrn, বর্ণ **map_rtrn,
                বর্ণ *nextop_rtrn, বর্ণ **extra_data)
{
    বর্ণ *tmp, *str, *next;

    str = *str_inout;

    /*
     * Find the position in the string where the next file is included,
     * if there is more than one left in the statement.
     */
    next = strpbrk(str, MERGE_MODE_PREFIXES);
    যদি (next) {
        /* Got more files, this function will be called again. */
        *nextop_rtrn = *next;
        /* Separate the string, for strchr etc. to work on this file only. */
        *next++ = '\0';
    }
    অন্যথা {
        /* This is the last file in this statement, won't be called again. */
        *nextop_rtrn = '\0';
        next = রিক্ত;
    }

    /*
     * Search for the explicit group designator, if any. If it's there,
     * it goes after the file name and map.
     */
    tmp = strchr(str, ':');
    যদি (tmp != রিক্ত) {
        *tmp++ = '\0';
        *extra_data = strdup(tmp);
    }
    অন্যথা {
        *extra_data = রিক্ত;
    }

    /* Look for a map, if any. */
    tmp = strchr(str, '(');
    যদি (tmp == রিক্ত) {
        /* No map. */
        *file_rtrn = strdup(str);
        *map_rtrn = রিক্ত;
    }
    অন্যথা যদি (str[0] == '(') {
        /* Map without file - invalid. */
        মুক্ত(*extra_data);
        প্রত্যাগম false;
    }
    অন্যথা {
        /* Got a map; separate the file and the map for the strdup's. */
        *tmp++ = '\0';
        *file_rtrn = strdup(str);
        str = tmp;
        tmp = strchr(str, ')');
        যদি (tmp == রিক্ত || tmp[1] != '\0') {
            মুক্ত(*file_rtrn);
            মুক্ত(*extra_data);
            প্রত্যাগম false;
        }
        *tmp++ = '\0';
        *map_rtrn = strdup(str);
    }

    /* Set up the next file for the next call, if any. */
    যদি (*nextop_rtrn == '\0')
        *str_inout = রিক্ত;
    অন্যথা যদি (is_merge_mode_prefix(*nextop_rtrn))
        *str_inout = next;
    অন্যথা
        প্রত্যাগম false;

    প্রত্যাগম true;
}

স্থির ধ্রুব বর্ণ *xkb_file_type_include_dirs[FILE_TYPE_অঙ্ক_ENTRIES] = {
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
স্থির ধ্রুব বর্ণ *
DirectoryForInclude(গণনা xkb_file_type type)
{
    যদি (type >= FILE_TYPE_অঙ্ক_ENTRIES)
        প্রত্যাগম "";
    প্রত্যাগম xkb_file_type_include_dirs[type];
}

স্থির শূন্য
LogIncludePaths(সংরচনা xkb_context *ctx)
{
    যদি (xkb_context_অঙ্ক_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "%u include paths searched:\n",
                xkb_context_অঙ্ক_include_paths(ctx));
        প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < xkb_context_অঙ্ক_include_paths(ctx); i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "\t%s\n",
                    xkb_context_include_path_get(ctx, i));
    }
    অন্যথা {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "There are no include paths to search\n");
    }

    যদি (xkb_context_অঙ্ক_failed_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "%u include paths could not be added:\n",
                xkb_context_অঙ্ক_failed_include_paths(ctx));
        প্রতি (darray_size_t i = 0;
             i < xkb_context_অঙ্ক_failed_include_paths(ctx);
             i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "\t%s\n",
                    xkb_context_failed_include_path_get(ctx, i));
    }
}

স্থির size_t
expand_percent(সংরচনা xkb_context *ctx, ধ্রুব বর্ণ* parent_file_name,
               ধ্রুব বর্ণ* typeDir, বর্ণ *buf, size_t buf_size,
               ধ্রুব বর্ণ *name, size_t name_len)
{
    সংরচনা scanner s;
    scanner_init(&s, ctx, name, name_len, parent_file_name, রিক্ত);
    s.buf_pos = 0;

    যাবত্ (!scanner_eof(&s) && !scanner_eol(&s)) {
        যদি (scanner_chr(&s, '%')) {
            যদি (scanner_chr(&s, '%')) {
                scanner_buf_append(&s, '%');
            }
            অন্যথা যদি (scanner_chr(&s, 'H')) {
                ধ্রুব বর্ণ *home = xkb_context_getenv(ctx, "HOME");
                যদি (!home) {
                    scanner_err(&s, XKB_LOG_MESSAGE_NO_ID,
                                "%%H was used in an include statement, but the "
                                "HOME environment variable is not set");
                    প্রত্যাগম 0;
                }
                যদি (!scanner_buf_appends(&s, home)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%H is too long");
                    প্রত্যাগম 0;
                }
            }
            অন্যথা যদি (scanner_chr(&s, 'S')) {
                ধ্রুব বর্ণ *default_মূল =
                    xkb_context_include_path_get_system_path(ctx);
                যদি (!scanner_buf_appends(&s, default_মূল) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%S is too long");
                    প্রত্যাগম 0;
                }
            }
            অন্যথা যদি (scanner_chr(&s, 'E')) {
                ধ্রুব বর্ণ *default_মূল =
                    xkb_context_include_path_get_extra_path(ctx);
                যদি (!scanner_buf_appends(&s, default_মূল) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%E is too long");
                    প্রত্যাগম 0;
                }
            }
            অন্যথা {
                scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                            "unknown %% format (%c) in include statement",
                            scanner_peek(&s));
                প্রত্যাগম 0;
            }
        }
        অন্যথা {
            scanner_buf_append(&s, scanner_next(&s));
        }
    }
    যদি (!scanner_buf_append(&s, '\0')) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                    "include path is too long; max: %zu", পরিমাণ(s.buf));
        প্রত্যাগম 0;
    }
    যদি (unlikely(s.buf_pos > buf_size)) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                    "include path is too long: %zu > %zu", s.buf_pos, buf_size);
        প্রত্যাগম 0;
    }
    memcpy(buf, s.buf, s.buf_pos);
    প্রত্যাগম s.buf_pos;
}

ssize_t
expand_path(সংরচনা xkb_context *ctx, ধ্রুব বর্ণ* parent_file_name,
            ধ্রুব বর্ণ *name, size_t name_len, গণনা xkb_file_type type,
            বর্ণ *buf, size_t buf_size)
{
    /* Detect %-expansion */
    size_t k;
    প্রতি (k = 0; k < name_len; k++) {
        যদি (name[k] == '%') {
            গচ্ছ expand;
        }
    }

    প্রত্যাগম 0;

expand:
    /* Process %-expansion */

    /* Copy prefix (usual case: k == 0) */
    যদি (unlikely(k >= buf_size)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: %zu > %zu, "
                "got raw path: %.*s\n",
                k, buf_size, (unsigned পূর্ণাঙ্ক) name_len, name);
        প্রত্যাগম -1;
    }
    memcpy(buf, name, k);

    /* Proceed to %-expansion */
    ধ্রুব বর্ণ *typeDir = DirectoryForInclude(type);
    size_t গুনতি = expand_percent(ctx, parent_file_name, typeDir,
                                  buf + k, buf_size - k,
                                  name + k, name_len - k);
    যদি (!গুনতি)
        প্রত্যাগম -1;
    গুনতি += k;
    প্রতিজ্ঞা(buf[গুনতি - 1] == '\0');
    প্রত্যাগম (ssize_t) গুনতি - 1;
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
FindFileInXkbPath(সংরচনা xkb_context *ctx, ধ্রুব বর্ণ* parent_file_name,
                  ধ্রুব বর্ণ *name, size_t name_len, গণনা xkb_file_type type,
                  বর্ণ *buf, size_t buf_size, unsigned পূর্ণাঙ্ক *offset,
                  bool required)
{
    /* We do not handle absolute paths here */
    প্রতিজ্ঞা(!is_absolute_path(name));

    FILE *file = রিক্ত;
    বর্ণ *name_buffer = রিক্ত;
    ধ্রুব বর্ণ *typeDir = DirectoryForInclude(type);

    প্রতি (unsigned পূর্ণাঙ্ক i = *offset; i < xkb_context_অঙ্ক_include_paths(ctx); i++) {
        যদি (!snprintf_safe(buf, buf_size, "%s/%s/%.*s",
                           xkb_context_include_path_get(ctx, i),
                           typeDir, (unsigned পূর্ণাঙ্ক) name_len, name)) {
            log_err(ctx, XKB_ERROR_INVALID_PATH,
                    "Path is too long: expected max length of %zu, "
                    "got: %s/%s/%.*s\n",
                    buf_size, xkb_context_include_path_get(ctx, i),
                    typeDir, (unsigned পূর্ণাঙ্ক) name_len, name);
            অনুবর্ত;
        }

        file = fopen(buf, "rb");
        যদি (file) {
            *offset = i;
            গচ্ছ out;
        }
    }

    /* We only print warnings if we can’t find the file on the first lookup */
    যদি (required && *offset == 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "Couldn't find file \"%s/%.*s\" in include paths\n",
                typeDir, (unsigned পূর্ণাঙ্ক) name_len, name);
        LogIncludePaths(ctx);
    }

out:
    মুক্ত(name_buffer);
    প্রত্যাগম file;
}

bool
ExceedsIncludeMaxDepth(সংরচনা xkb_context *ctx, unsigned পূর্ণাঙ্ক include_depth)
{
    যদি (include_depth >= INCLUDE_MAX_DEPTH) {
        log_err(ctx, XKB_ERROR_RECURSIVE_INCLUDE,
                "Exceeded include depth threshold (%u)",
                INCLUDE_MAX_DEPTH);
        প্রত্যাগম true;
    } অন্যথা {
        প্রত্যাগম false;
    }
}

XkbFile *
ProcessIncludeFile(সংরচনা xkb_context *ctx,
                   ধ্রুব সংরচনা parser_keymap_config *config,
                   ধ্রুব IncludeStmt *stmt,
                   গণনা xkb_file_type file_type, বর্ণ *path, size_t path_size)
{
    /*
     * Resolve include statement:
     * - Explicit map name: look for an *exact match* only.
     * - Default map: look for an *explicit default* map (i.e. tagged with
     *   `default`: exact match), else fallback to the first *implicit* default
     *   map (weak match).
     */
    XkbFile *xkb_file = রিক্ত;  /* Exact match */
    XkbFile *candidate = রিক্ত; /* Weak match */

    ধ্রুব বর্ণ *stmt_file = stmt->file;
    size_t stmt_file_len = strlen(stmt_file);

    /* Process %-expansion, if any */
    // FIXME: use parent file name instead of “(unknow)”.
    ধ্রুব ssize_t expanded = expand_path(ctx, "(unknown)",
                                         stmt_file, stmt_file_len, file_type,
                                         path, path_size);
    যদি (expanded < 0) {
        /* Error */
        প্রত্যাগম রিক্ত;
    } অন্যথা যদি (expanded > 0) {
        /* %-expanded */
        stmt_file = path;
        stmt_file_len = (size_t) expanded;
        প্রতিজ্ঞা(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    unsigned পূর্ণাঙ্ক offset = 0;
    ধ্রুব bool absolute_path = is_absolute_path(stmt_file);
    যদি (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        প্রতিজ্ঞা(stmt_file[stmt_file_len] == '\0');
        file = fopen(stmt_file, "rb");
    } অন্যথা {
        /* Relative path: lookup the first XKB path */
        যদি (unlikely(expanded)) {
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
            file = রিক্ত;
        } অন্যথা {
            // FIXME: use parent file name instead of “(unknow)”.
            file = FindFileInXkbPath(ctx, "(unknown)",
                                     stmt_file, stmt_file_len, file_type,
                                     path, path_size, &offset, true);
        }
    }

    যাবত্ (file) {
        xkb_file = XkbParseFile(ctx, config, file, stmt->file, stmt->map);
        fclose(file);

        যদি (xkb_file) {
            যদি (xkb_file->file_type != file_type) {
                log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                        "Include file of wrong type (expected %s, got %s); "
                        "Include file \"%s\" ignored\n",
                        xkb_file_type_to_string(file_type),
                        xkb_file_type_to_string(xkb_file->file_type), stmt->file);
                FreeXkbFile(xkb_file);
                xkb_file = রিক্ত;
            } অন্যথা যদি (stmt->map || (xkb_file->flags & MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops here.
                 */
                ভঙ্গ;
            } অন্যথা যদি (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = xkb_file;
                xkb_file = রিক্ত;
            } অন্যথা {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                FreeXkbFile(xkb_file);
                xkb_file = রিক্ত;
            }
        }

        যদি (absolute_path) {
            /* There is no point to search further if the path is absolute */
            ভঙ্গ;
        }

        offset++;
        file = FindFileInXkbPath(ctx, "(unknown)",
                                 stmt_file, stmt_file_len, file_type,
                                 path, path_size, &offset, true);
    }

    যদি (!xkb_file) {
        /* No exact match: use weak match, if any */
        xkb_file = candidate;
    } অন্যথা {
        /* Found exact match: discard weak match, if any */
        FreeXkbFile(candidate);
    }

    যদি (!xkb_file) {
        যদি (stmt->map)
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s(%s)'\n",
                    stmt->file, stmt->map);
        অন্যথা
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s'\n",
                    stmt->file);
    }

    প্রত্যাগম xkb_file;
}
