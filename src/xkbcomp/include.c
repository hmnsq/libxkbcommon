/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <limits.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>

#અન્તર્ભાવ "messages-codes.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "xkbcomp-priv.h"
#અન્તર્ભાવ "include.h"
#અન્તર્ભાવ "scanner-utils.h"
#અન્તર્ભાવ "util-paths.h"

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
ParseIncludeMap(અક્ષર **str_inout, અક્ષર **file_rtrn, અક્ષર **map_rtrn,
                અક્ષર *nextop_rtrn, અક્ષર **extra_data)
{
    અક્ષર *tmp, *str, *next;

    str = *str_inout;

    /*
     * Find the position in the string where the next file is included,
     * if there is more than one left in the statement.
     */
    next = strpbrk(str, MERGE_MODE_PREFIXES);
    જો (next) {
        /* Got more files, this function will be called again. */
        *nextop_rtrn = *next;
        /* Separate the string, for strchr etc. to work on this file only. */
        *next++ = '\0';
    }
    નહીંતર {
        /* This is the last file in this statement, won't be called again. */
        *nextop_rtrn = '\0';
        next = ખાલી;
    }

    /*
     * Search for the explicit group designator, if any. If it's there,
     * it goes after the file name and map.
     */
    tmp = strchr(str, ':');
    જો (tmp != ખાલી) {
        *tmp++ = '\0';
        *extra_data = strdup(tmp);
    }
    નહીંતર {
        *extra_data = ખાલી;
    }

    /* Look for a map, if any. */
    tmp = strchr(str, '(');
    જો (tmp == ખાલી) {
        /* No map. */
        *file_rtrn = strdup(str);
        *map_rtrn = ખાલી;
    }
    નહીંતર જો (str[0] == '(') {
        /* Map without file - invalid. */
        મુક્ત(*extra_data);
        પાછા false;
    }
    નહીંતર {
        /* Got a map; separate the file and the map for the strdup's. */
        *tmp++ = '\0';
        *file_rtrn = strdup(str);
        str = tmp;
        tmp = strchr(str, ')');
        જો (tmp == ખાલી || tmp[1] != '\0') {
            મુક્ત(*file_rtrn);
            મુક્ત(*extra_data);
            પાછા false;
        }
        *tmp++ = '\0';
        *map_rtrn = strdup(str);
    }

    /* Set up the next file for the next call, if any. */
    જો (*nextop_rtrn == '\0')
        *str_inout = ખાલી;
    નહીંતર જો (is_merge_mode_prefix(*nextop_rtrn))
        *str_inout = next;
    નહીંતર
        પાછા false;

    પાછા true;
}

સ્થિર અચલ અક્ષર *xkb_file_type_include_dirs[FILE_TYPE_અંક_ENTRIES] = {
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
સ્થિર અચલ અક્ષર *
DirectoryForInclude(ગણના xkb_file_type type)
{
    જો (type >= FILE_TYPE_અંક_ENTRIES)
        પાછા "";
    પાછા xkb_file_type_include_dirs[type];
}

સ્થિર શૂન્ય
LogIncludePaths(સંરચના xkb_context *ctx)
{
    જો (xkb_context_અંક_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "%u include paths searched:\n",
                xkb_context_અંક_include_paths(ctx));
        માટે (unsigned પૂર્ણાંક i = 0; i < xkb_context_અંક_include_paths(ctx); i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "\t%s\n",
                    xkb_context_include_path_get(ctx, i));
    }
    નહીંતર {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "There are no include paths to search\n");
    }

    જો (xkb_context_અંક_failed_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "%u include paths could not be added:\n",
                xkb_context_અંક_failed_include_paths(ctx));
        માટે (darray_size_t i = 0;
             i < xkb_context_અંક_failed_include_paths(ctx);
             i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "\t%s\n",
                    xkb_context_failed_include_path_get(ctx, i));
    }
}

સ્થિર size_t
expand_percent(સંરચના xkb_context *ctx, અચલ અક્ષર* parent_file_name,
               અચલ અક્ષર* typeDir, અક્ષર *buf, size_t buf_size,
               અચલ અક્ષર *name, size_t name_len)
{
    સંરચના scanner s;
    scanner_init(&s, ctx, name, name_len, parent_file_name, ખાલી);
    s.buf_pos = 0;

    જ્યારે (!scanner_eof(&s) && !scanner_eol(&s)) {
        જો (scanner_chr(&s, '%')) {
            જો (scanner_chr(&s, '%')) {
                scanner_buf_append(&s, '%');
            }
            નહીંતર જો (scanner_chr(&s, 'H')) {
                અચલ અક્ષર *home = xkb_context_getenv(ctx, "HOME");
                જો (!home) {
                    scanner_err(&s, XKB_LOG_MESSAGE_NO_ID,
                                "%%H was used in an include statement, but the "
                                "HOME environment variable is not set");
                    પાછા 0;
                }
                જો (!scanner_buf_appends(&s, home)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%H is too long");
                    પાછા 0;
                }
            }
            નહીંતર જો (scanner_chr(&s, 'S')) {
                અચલ અક્ષર *default_મૂળ =
                    xkb_context_include_path_get_system_path(ctx);
                જો (!scanner_buf_appends(&s, default_મૂળ) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%S is too long");
                    પાછા 0;
                }
            }
            નહીંતર જો (scanner_chr(&s, 'E')) {
                અચલ અક્ષર *default_મૂળ =
                    xkb_context_include_path_get_extra_path(ctx);
                જો (!scanner_buf_appends(&s, default_મૂળ) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%E is too long");
                    પાછા 0;
                }
            }
            નહીંતર {
                scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                            "unknown %% format (%c) in include statement",
                            scanner_peek(&s));
                પાછા 0;
            }
        }
        નહીંતર {
            scanner_buf_append(&s, scanner_next(&s));
        }
    }
    જો (!scanner_buf_append(&s, '\0')) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                    "include path is too long; max: %zu", કદ(s.buf));
        પાછા 0;
    }
    જો (unlikely(s.buf_pos > buf_size)) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                    "include path is too long: %zu > %zu", s.buf_pos, buf_size);
        પાછા 0;
    }
    memcpy(buf, s.buf, s.buf_pos);
    પાછા s.buf_pos;
}

ssize_t
expand_path(સંરચના xkb_context *ctx, અચલ અક્ષર* parent_file_name,
            અચલ અક્ષર *name, size_t name_len, ગણના xkb_file_type type,
            અક્ષર *buf, size_t buf_size)
{
    /* Detect %-expansion */
    size_t k;
    માટે (k = 0; k < name_len; k++) {
        જો (name[k] == '%') {
            જાઓ expand;
        }
    }

    પાછા 0;

expand:
    /* Process %-expansion */

    /* Copy prefix (usual case: k == 0) */
    જો (unlikely(k >= buf_size)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: %zu > %zu, "
                "got raw path: %.*s\n",
                k, buf_size, (unsigned પૂર્ણાંક) name_len, name);
        પાછા -1;
    }
    memcpy(buf, name, k);

    /* Proceed to %-expansion */
    અચલ અક્ષર *typeDir = DirectoryForInclude(type);
    size_t ગણતરી = expand_percent(ctx, parent_file_name, typeDir,
                                  buf + k, buf_size - k,
                                  name + k, name_len - k);
    જો (!ગણતરી)
        પાછા -1;
    ગણતરી += k;
    પ્રતિજ્ઞા(buf[ગણતરી - 1] == '\0');
    પાછા (ssize_t) ગણતરી - 1;
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
FindFileInXkbPath(સંરચના xkb_context *ctx, અચલ અક્ષર* parent_file_name,
                  અચલ અક્ષર *name, size_t name_len, ગણના xkb_file_type type,
                  અક્ષર *buf, size_t buf_size, unsigned પૂર્ણાંક *offset,
                  bool required)
{
    /* We do not handle absolute paths here */
    પ્રતિજ્ઞા(!is_absolute_path(name));

    FILE *file = ખાલી;
    અક્ષર *name_buffer = ખાલી;
    અચલ અક્ષર *typeDir = DirectoryForInclude(type);

    માટે (unsigned પૂર્ણાંક i = *offset; i < xkb_context_અંક_include_paths(ctx); i++) {
        જો (!snprintf_safe(buf, buf_size, "%s/%s/%.*s",
                           xkb_context_include_path_get(ctx, i),
                           typeDir, (unsigned પૂર્ણાંક) name_len, name)) {
            log_err(ctx, XKB_ERROR_INVALID_PATH,
                    "Path is too long: expected max length of %zu, "
                    "got: %s/%s/%.*s\n",
                    buf_size, xkb_context_include_path_get(ctx, i),
                    typeDir, (unsigned પૂર્ણાંક) name_len, name);
            ચાલુ;
        }

        file = fopen(buf, "rb");
        જો (file) {
            *offset = i;
            જાઓ out;
        }
    }

    /* We only print warnings if we can’t find the file on the first lookup */
    જો (required && *offset == 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "Couldn't find file \"%s/%.*s\" in include paths\n",
                typeDir, (unsigned પૂર્ણાંક) name_len, name);
        LogIncludePaths(ctx);
    }

out:
    મુક્ત(name_buffer);
    પાછા file;
}

bool
ExceedsIncludeMaxDepth(સંરચના xkb_context *ctx, unsigned પૂર્ણાંક include_depth)
{
    જો (include_depth >= INCLUDE_MAX_DEPTH) {
        log_err(ctx, XKB_ERROR_RECURSIVE_INCLUDE,
                "Exceeded include depth threshold (%u)",
                INCLUDE_MAX_DEPTH);
        પાછા true;
    } નહીંતર {
        પાછા false;
    }
}

XkbFile *
ProcessIncludeFile(સંરચના xkb_context *ctx,
                   અચલ સંરચના parser_keymap_config *config,
                   અચલ IncludeStmt *stmt,
                   ગણના xkb_file_type file_type, અક્ષર *path, size_t path_size)
{
    /*
     * Resolve include statement:
     * - Explicit map name: look for an *exact match* only.
     * - Default map: look for an *explicit default* map (i.e. tagged with
     *   `default`: exact match), else fallback to the first *implicit* default
     *   map (weak match).
     */
    XkbFile *xkb_file = ખાલી;  /* Exact match */
    XkbFile *candidate = ખાલી; /* Weak match */

    અચલ અક્ષર *stmt_file = stmt->file;
    size_t stmt_file_len = strlen(stmt_file);

    /* Process %-expansion, if any */
    // FIXME: use parent file name instead of “(unknow)”.
    અચલ ssize_t expanded = expand_path(ctx, "(unknown)",
                                         stmt_file, stmt_file_len, file_type,
                                         path, path_size);
    જો (expanded < 0) {
        /* Error */
        પાછા ખાલી;
    } નહીંતર જો (expanded > 0) {
        /* %-expanded */
        stmt_file = path;
        stmt_file_len = (size_t) expanded;
        પ્રતિજ્ઞા(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    unsigned પૂર્ણાંક offset = 0;
    અચલ bool absolute_path = is_absolute_path(stmt_file);
    જો (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        પ્રતિજ્ઞા(stmt_file[stmt_file_len] == '\0');
        file = fopen(stmt_file, "rb");
    } નહીંતર {
        /* Relative path: lookup the first XKB path */
        જો (unlikely(expanded)) {
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
            file = ખાલી;
        } નહીંતર {
            // FIXME: use parent file name instead of “(unknow)”.
            file = FindFileInXkbPath(ctx, "(unknown)",
                                     stmt_file, stmt_file_len, file_type,
                                     path, path_size, &offset, true);
        }
    }

    જ્યારે (file) {
        xkb_file = XkbParseFile(ctx, config, file, stmt->file, stmt->map);
        fclose(file);

        જો (xkb_file) {
            જો (xkb_file->file_type != file_type) {
                log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                        "Include file of wrong type (expected %s, got %s); "
                        "Include file \"%s\" ignored\n",
                        xkb_file_type_to_string(file_type),
                        xkb_file_type_to_string(xkb_file->file_type), stmt->file);
                FreeXkbFile(xkb_file);
                xkb_file = ખાલી;
            } નહીંતર જો (stmt->map || (xkb_file->flags & MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops here.
                 */
                તોડો;
            } નહીંતર જો (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = xkb_file;
                xkb_file = ખાલી;
            } નહીંતર {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                FreeXkbFile(xkb_file);
                xkb_file = ખાલી;
            }
        }

        જો (absolute_path) {
            /* There is no point to search further if the path is absolute */
            તોડો;
        }

        offset++;
        file = FindFileInXkbPath(ctx, "(unknown)",
                                 stmt_file, stmt_file_len, file_type,
                                 path, path_size, &offset, true);
    }

    જો (!xkb_file) {
        /* No exact match: use weak match, if any */
        xkb_file = candidate;
    } નહીંતર {
        /* Found exact match: discard weak match, if any */
        FreeXkbFile(candidate);
    }

    જો (!xkb_file) {
        જો (stmt->map)
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s(%s)'\n",
                    stmt->file, stmt->map);
        નહીંતર
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s'\n",
                    stmt->file);
    }

    પાછા xkb_file;
}
