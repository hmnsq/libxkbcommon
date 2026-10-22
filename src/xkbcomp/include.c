/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <limits.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>

#അന്തര്ഭാവ "messages-codes.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "xkbcomp-priv.h"
#അന്തര്ഭാവ "include.h"
#അന്തര്ഭാവ "scanner-utils.h"
#അന്തര്ഭാവ "util-paths.h"

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
ParseIncludeMap(അക്ഷരം **str_inout, അക്ഷരം **file_rtrn, അക്ഷരം **map_rtrn,
                അക്ഷരം *nextop_rtrn, അക്ഷരം **extra_data)
{
    അക്ഷരം *tmp, *str, *next;

    str = *str_inout;

    /*
     * Find the position in the string where the next file is included,
     * if there is more than one left in the statement.
     */
    next = strpbrk(str, MERGE_MODE_PREFIXES);
    എങ്കിൽ (next) {
        /* Got more files, this function will be called again. */
        *nextop_rtrn = *next;
        /* Separate the string, for strchr etc. to work on this file only. */
        *next++ = '\0';
    }
    അല്ലെങ്കിൽ {
        /* This is the last file in this statement, won't be called again. */
        *nextop_rtrn = '\0';
        next = ശൂന്യ;
    }

    /*
     * Search for the explicit group designator, if any. If it's there,
     * it goes after the file name and map.
     */
    tmp = strchr(str, ':');
    എങ്കിൽ (tmp != ശൂന്യ) {
        *tmp++ = '\0';
        *extra_data = strdup(tmp);
    }
    അല്ലെങ്കിൽ {
        *extra_data = ശൂന്യ;
    }

    /* Look for a map, if any. */
    tmp = strchr(str, '(');
    എങ്കിൽ (tmp == ശൂന്യ) {
        /* No map. */
        *file_rtrn = strdup(str);
        *map_rtrn = ശൂന്യ;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (str[0] == '(') {
        /* Map without file - invalid. */
        സ്വതന്ത്രം(*extra_data);
        തിരികെ false;
    }
    അല്ലെങ്കിൽ {
        /* Got a map; separate the file and the map for the strdup's. */
        *tmp++ = '\0';
        *file_rtrn = strdup(str);
        str = tmp;
        tmp = strchr(str, ')');
        എങ്കിൽ (tmp == ശൂന്യ || tmp[1] != '\0') {
            സ്വതന്ത്രം(*file_rtrn);
            സ്വതന്ത്രം(*extra_data);
            തിരികെ false;
        }
        *tmp++ = '\0';
        *map_rtrn = strdup(str);
    }

    /* Set up the next file for the next call, if any. */
    എങ്കിൽ (*nextop_rtrn == '\0')
        *str_inout = ശൂന്യ;
    അല്ലെങ്കിൽ എങ്കിൽ (is_merge_mode_prefix(*nextop_rtrn))
        *str_inout = next;
    അല്ലെങ്കിൽ
        തിരികെ false;

    തിരികെ true;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *xkb_file_type_include_dirs[FILE_TYPE_അക്കം_ENTRIES] = {
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
സ്ഥിതം സ്ഥിരം അക്ഷരം *
DirectoryForInclude(എണ്ണൽ xkb_file_type type)
{
    എങ്കിൽ (type >= FILE_TYPE_അക്കം_ENTRIES)
        തിരികെ "";
    തിരികെ xkb_file_type_include_dirs[type];
}

സ്ഥിതം ശൂന്യം
LogIncludePaths(ഘടന xkb_context *ctx)
{
    എങ്കിൽ (xkb_context_അക്കം_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "%u include paths searched:\n",
                xkb_context_അക്കം_include_paths(ctx));
        വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < xkb_context_അക്കം_include_paths(ctx); i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "\t%s\n",
                    xkb_context_include_path_get(ctx, i));
    }
    അല്ലെങ്കിൽ {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "There are no include paths to search\n");
    }

    എങ്കിൽ (xkb_context_അക്കം_failed_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "%u include paths could not be added:\n",
                xkb_context_അക്കം_failed_include_paths(ctx));
        വേണ്ടി (darray_size_t i = 0;
             i < xkb_context_അക്കം_failed_include_paths(ctx);
             i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "\t%s\n",
                    xkb_context_failed_include_path_get(ctx, i));
    }
}

സ്ഥിതം size_t
expand_percent(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം* parent_file_name,
               സ്ഥിരം അക്ഷരം* typeDir, അക്ഷരം *buf, size_t buf_size,
               സ്ഥിരം അക്ഷരം *name, size_t name_len)
{
    ഘടന scanner s;
    scanner_init(&s, ctx, name, name_len, parent_file_name, ശൂന്യ);
    s.buf_pos = 0;

    ആകുമ്പോൾ (!scanner_eof(&s) && !scanner_eol(&s)) {
        എങ്കിൽ (scanner_chr(&s, '%')) {
            എങ്കിൽ (scanner_chr(&s, '%')) {
                scanner_buf_append(&s, '%');
            }
            അല്ലെങ്കിൽ എങ്കിൽ (scanner_chr(&s, 'H')) {
                സ്ഥിരം അക്ഷരം *home = xkb_context_getenv(ctx, "HOME");
                എങ്കിൽ (!home) {
                    scanner_err(&s, XKB_LOG_MESSAGE_NO_ID,
                                "%%H was used in an include statement, but the "
                                "HOME environment variable is not set");
                    തിരികെ 0;
                }
                എങ്കിൽ (!scanner_buf_appends(&s, home)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%H is too long");
                    തിരികെ 0;
                }
            }
            അല്ലെങ്കിൽ എങ്കിൽ (scanner_chr(&s, 'S')) {
                സ്ഥിരം അക്ഷരം *default_വേര് =
                    xkb_context_include_path_get_system_path(ctx);
                എങ്കിൽ (!scanner_buf_appends(&s, default_വേര്) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%S is too long");
                    തിരികെ 0;
                }
            }
            അല്ലെങ്കിൽ എങ്കിൽ (scanner_chr(&s, 'E')) {
                സ്ഥിരം അക്ഷരം *default_വേര് =
                    xkb_context_include_path_get_extra_path(ctx);
                എങ്കിൽ (!scanner_buf_appends(&s, default_വേര്) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%E is too long");
                    തിരികെ 0;
                }
            }
            അല്ലെങ്കിൽ {
                scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                            "unknown %% format (%c) in include statement",
                            scanner_peek(&s));
                തിരികെ 0;
            }
        }
        അല്ലെങ്കിൽ {
            scanner_buf_append(&s, scanner_next(&s));
        }
    }
    എങ്കിൽ (!scanner_buf_append(&s, '\0')) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                    "include path is too long; max: %zu", വലുപ്പം(s.buf));
        തിരികെ 0;
    }
    എങ്കിൽ (unlikely(s.buf_pos > buf_size)) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                    "include path is too long: %zu > %zu", s.buf_pos, buf_size);
        തിരികെ 0;
    }
    memcpy(buf, s.buf, s.buf_pos);
    തിരികെ s.buf_pos;
}

ssize_t
expand_path(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം* parent_file_name,
            സ്ഥിരം അക്ഷരം *name, size_t name_len, എണ്ണൽ xkb_file_type type,
            അക്ഷരം *buf, size_t buf_size)
{
    /* Detect %-expansion */
    size_t k;
    വേണ്ടി (k = 0; k < name_len; k++) {
        എങ്കിൽ (name[k] == '%') {
            പോകുക expand;
        }
    }

    തിരികെ 0;

expand:
    /* Process %-expansion */

    /* Copy prefix (usual case: k == 0) */
    എങ്കിൽ (unlikely(k >= buf_size)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: %zu > %zu, "
                "got raw path: %.*s\n",
                k, buf_size, (unsigned പൂർണ്ണസംഖ്യ) name_len, name);
        തിരികെ -1;
    }
    memcpy(buf, name, k);

    /* Proceed to %-expansion */
    സ്ഥിരം അക്ഷരം *typeDir = DirectoryForInclude(type);
    size_t എണ്ണം = expand_percent(ctx, parent_file_name, typeDir,
                                  buf + k, buf_size - k,
                                  name + k, name_len - k);
    എങ്കിൽ (!എണ്ണം)
        തിരികെ -1;
    എണ്ണം += k;
    പ്രതിജ്ഞാ(buf[എണ്ണം - 1] == '\0');
    തിരികെ (ssize_t) എണ്ണം - 1;
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
FindFileInXkbPath(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം* parent_file_name,
                  സ്ഥിരം അക്ഷരം *name, size_t name_len, എണ്ണൽ xkb_file_type type,
                  അക്ഷരം *buf, size_t buf_size, unsigned പൂർണ്ണസംഖ്യ *offset,
                  bool required)
{
    /* We do not handle absolute paths here */
    പ്രതിജ്ഞാ(!is_absolute_path(name));

    FILE *file = ശൂന്യ;
    അക്ഷരം *name_buffer = ശൂന്യ;
    സ്ഥിരം അക്ഷരം *typeDir = DirectoryForInclude(type);

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = *offset; i < xkb_context_അക്കം_include_paths(ctx); i++) {
        എങ്കിൽ (!snprintf_safe(buf, buf_size, "%s/%s/%.*s",
                           xkb_context_include_path_get(ctx, i),
                           typeDir, (unsigned പൂർണ്ണസംഖ്യ) name_len, name)) {
            log_err(ctx, XKB_ERROR_INVALID_PATH,
                    "Path is too long: expected max length of %zu, "
                    "got: %s/%s/%.*s\n",
                    buf_size, xkb_context_include_path_get(ctx, i),
                    typeDir, (unsigned പൂർണ്ണസംഖ്യ) name_len, name);
            തുടരുക;
        }

        file = fopen(buf, "rb");
        എങ്കിൽ (file) {
            *offset = i;
            പോകുക out;
        }
    }

    /* We only print warnings if we can’t find the file on the first lookup */
    എങ്കിൽ (required && *offset == 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "Couldn't find file \"%s/%.*s\" in include paths\n",
                typeDir, (unsigned പൂർണ്ണസംഖ്യ) name_len, name);
        LogIncludePaths(ctx);
    }

out:
    സ്വതന്ത്രം(name_buffer);
    തിരികെ file;
}

bool
ExceedsIncludeMaxDepth(ഘടന xkb_context *ctx, unsigned പൂർണ്ണസംഖ്യ include_depth)
{
    എങ്കിൽ (include_depth >= INCLUDE_MAX_DEPTH) {
        log_err(ctx, XKB_ERROR_RECURSIVE_INCLUDE,
                "Exceeded include depth threshold (%u)",
                INCLUDE_MAX_DEPTH);
        തിരികെ true;
    } അല്ലെങ്കിൽ {
        തിരികെ false;
    }
}

XkbFile *
ProcessIncludeFile(ഘടന xkb_context *ctx,
                   സ്ഥിരം ഘടന parser_keymap_config *config,
                   സ്ഥിരം IncludeStmt *stmt,
                   എണ്ണൽ xkb_file_type file_type, അക്ഷരം *path, size_t path_size)
{
    /*
     * Resolve include statement:
     * - Explicit map name: look for an *exact match* only.
     * - Default map: look for an *explicit default* map (i.e. tagged with
     *   `default`: exact match), else fallback to the first *implicit* default
     *   map (weak match).
     */
    XkbFile *xkb_file = ശൂന്യ;  /* Exact match */
    XkbFile *candidate = ശൂന്യ; /* Weak match */

    സ്ഥിരം അക്ഷരം *stmt_file = stmt->file;
    size_t stmt_file_len = strlen(stmt_file);

    /* Process %-expansion, if any */
    // FIXME: use parent file name instead of “(unknow)”.
    സ്ഥിരം ssize_t expanded = expand_path(ctx, "(unknown)",
                                         stmt_file, stmt_file_len, file_type,
                                         path, path_size);
    എങ്കിൽ (expanded < 0) {
        /* Error */
        തിരികെ ശൂന്യ;
    } അല്ലെങ്കിൽ എങ്കിൽ (expanded > 0) {
        /* %-expanded */
        stmt_file = path;
        stmt_file_len = (size_t) expanded;
        പ്രതിജ്ഞാ(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    unsigned പൂർണ്ണസംഖ്യ offset = 0;
    സ്ഥിരം bool absolute_path = is_absolute_path(stmt_file);
    എങ്കിൽ (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        പ്രതിജ്ഞാ(stmt_file[stmt_file_len] == '\0');
        file = fopen(stmt_file, "rb");
    } അല്ലെങ്കിൽ {
        /* Relative path: lookup the first XKB path */
        എങ്കിൽ (unlikely(expanded)) {
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
            file = ശൂന്യ;
        } അല്ലെങ്കിൽ {
            // FIXME: use parent file name instead of “(unknow)”.
            file = FindFileInXkbPath(ctx, "(unknown)",
                                     stmt_file, stmt_file_len, file_type,
                                     path, path_size, &offset, true);
        }
    }

    ആകുമ്പോൾ (file) {
        xkb_file = XkbParseFile(ctx, config, file, stmt->file, stmt->map);
        fclose(file);

        എങ്കിൽ (xkb_file) {
            എങ്കിൽ (xkb_file->file_type != file_type) {
                log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                        "Include file of wrong type (expected %s, got %s); "
                        "Include file \"%s\" ignored\n",
                        xkb_file_type_to_string(file_type),
                        xkb_file_type_to_string(xkb_file->file_type), stmt->file);
                FreeXkbFile(xkb_file);
                xkb_file = ശൂന്യ;
            } അല്ലെങ്കിൽ എങ്കിൽ (stmt->map || (xkb_file->flags & MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops here.
                 */
                നിർത്തുക;
            } അല്ലെങ്കിൽ എങ്കിൽ (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = xkb_file;
                xkb_file = ശൂന്യ;
            } അല്ലെങ്കിൽ {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                FreeXkbFile(xkb_file);
                xkb_file = ശൂന്യ;
            }
        }

        എങ്കിൽ (absolute_path) {
            /* There is no point to search further if the path is absolute */
            നിർത്തുക;
        }

        offset++;
        file = FindFileInXkbPath(ctx, "(unknown)",
                                 stmt_file, stmt_file_len, file_type,
                                 path, path_size, &offset, true);
    }

    എങ്കിൽ (!xkb_file) {
        /* No exact match: use weak match, if any */
        xkb_file = candidate;
    } അല്ലെങ്കിൽ {
        /* Found exact match: discard weak match, if any */
        FreeXkbFile(candidate);
    }

    എങ്കിൽ (!xkb_file) {
        എങ്കിൽ (stmt->map)
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s(%s)'\n",
                    stmt->file, stmt->map);
        അല്ലെങ്കിൽ
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s'\n",
                    stmt->file);
    }

    തിരികെ xkb_file;
}
