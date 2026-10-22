/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <limits.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>

#அந்தர்பாவ "messages-codes.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "xkbcomp-priv.h"
#அந்தர்பாவ "include.h"
#அந்தர்பாவ "scanner-utils.h"
#அந்தர்பாவ "util-paths.h"

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
ParseIncludeMap(எழுத்து **str_inout, எழுத்து **file_rtrn, எழுத்து **map_rtrn,
                எழுத்து *nextop_rtrn, எழுத்து **extra_data)
{
    எழுத்து *tmp, *str, *next;

    str = *str_inout;

    /*
     * Find the position in the string where the next file is included,
     * if there is more than one left in the statement.
     */
    next = strpbrk(str, MERGE_MODE_PREFIXES);
    எனில் (next) {
        /* Got more files, this function will be called again. */
        *nextop_rtrn = *next;
        /* Separate the string, for strchr etc. to work on this file only. */
        *next++ = '\0';
    }
    இல்லையெனில் {
        /* This is the last file in this statement, won't be called again. */
        *nextop_rtrn = '\0';
        next = வெற்று;
    }

    /*
     * Search for the explicit group designator, if any. If it's there,
     * it goes after the file name and map.
     */
    tmp = strchr(str, ':');
    எனில் (tmp != வெற்று) {
        *tmp++ = '\0';
        *extra_data = strdup(tmp);
    }
    இல்லையெனில் {
        *extra_data = வெற்று;
    }

    /* Look for a map, if any. */
    tmp = strchr(str, '(');
    எனில் (tmp == வெற்று) {
        /* No map. */
        *file_rtrn = strdup(str);
        *map_rtrn = வெற்று;
    }
    இல்லையெனில் எனில் (str[0] == '(') {
        /* Map without file - invalid. */
        விடுவி(*extra_data);
        திரும்பு false;
    }
    இல்லையெனில் {
        /* Got a map; separate the file and the map for the strdup's. */
        *tmp++ = '\0';
        *file_rtrn = strdup(str);
        str = tmp;
        tmp = strchr(str, ')');
        எனில் (tmp == வெற்று || tmp[1] != '\0') {
            விடுவி(*file_rtrn);
            விடுவி(*extra_data);
            திரும்பு false;
        }
        *tmp++ = '\0';
        *map_rtrn = strdup(str);
    }

    /* Set up the next file for the next call, if any. */
    எனில் (*nextop_rtrn == '\0')
        *str_inout = வெற்று;
    இல்லையெனில் எனில் (is_merge_mode_prefix(*nextop_rtrn))
        *str_inout = next;
    இல்லையெனில்
        திரும்பு false;

    திரும்பு true;
}

நிலையான மாறிலி எழுத்து *xkb_file_type_include_dirs[FILE_TYPE_இலக்கம்_ENTRIES] = {
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
நிலையான மாறிலி எழுத்து *
DirectoryForInclude(எண்ணிக்கை xkb_file_type type)
{
    எனில் (type >= FILE_TYPE_இலக்கம்_ENTRIES)
        திரும்பு "";
    திரும்பு xkb_file_type_include_dirs[type];
}

நிலையான வெறுமை
LogIncludePaths(கட்டமைப்பு xkb_context *ctx)
{
    எனில் (xkb_context_இலக்கம்_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "%u include paths searched:\n",
                xkb_context_இலக்கம்_include_paths(ctx));
        க்கு (unsigned முழு i = 0; i < xkb_context_இலக்கம்_include_paths(ctx); i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "\t%s\n",
                    xkb_context_include_path_get(ctx, i));
    }
    இல்லையெனில் {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "There are no include paths to search\n");
    }

    எனில் (xkb_context_இலக்கம்_failed_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "%u include paths could not be added:\n",
                xkb_context_இலக்கம்_failed_include_paths(ctx));
        க்கு (darray_size_t i = 0;
             i < xkb_context_இலக்கம்_failed_include_paths(ctx);
             i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "\t%s\n",
                    xkb_context_failed_include_path_get(ctx, i));
    }
}

நிலையான size_t
expand_percent(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து* parent_file_name,
               மாறிலி எழுத்து* typeDir, எழுத்து *buf, size_t buf_size,
               மாறிலி எழுத்து *name, size_t name_len)
{
    கட்டமைப்பு scanner s;
    scanner_init(&s, ctx, name, name_len, parent_file_name, வெற்று);
    s.buf_pos = 0;

    போது (!scanner_eof(&s) && !scanner_eol(&s)) {
        எனில் (scanner_chr(&s, '%')) {
            எனில் (scanner_chr(&s, '%')) {
                scanner_buf_append(&s, '%');
            }
            இல்லையெனில் எனில் (scanner_chr(&s, 'H')) {
                மாறிலி எழுத்து *home = xkb_context_getenv(ctx, "HOME");
                எனில் (!home) {
                    scanner_err(&s, XKB_LOG_MESSAGE_NO_ID,
                                "%%H was used in an include statement, but the "
                                "HOME environment variable is not set");
                    திரும்பு 0;
                }
                எனில் (!scanner_buf_appends(&s, home)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%H is too long");
                    திரும்பு 0;
                }
            }
            இல்லையெனில் எனில் (scanner_chr(&s, 'S')) {
                மாறிலி எழுத்து *default_வேர் =
                    xkb_context_include_path_get_system_path(ctx);
                எனில் (!scanner_buf_appends(&s, default_வேர்) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%S is too long");
                    திரும்பு 0;
                }
            }
            இல்லையெனில் எனில் (scanner_chr(&s, 'E')) {
                மாறிலி எழுத்து *default_வேர் =
                    xkb_context_include_path_get_extra_path(ctx);
                எனில் (!scanner_buf_appends(&s, default_வேர்) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%E is too long");
                    திரும்பு 0;
                }
            }
            இல்லையெனில் {
                scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                            "unknown %% format (%c) in include statement",
                            scanner_peek(&s));
                திரும்பு 0;
            }
        }
        இல்லையெனில் {
            scanner_buf_append(&s, scanner_next(&s));
        }
    }
    எனில் (!scanner_buf_append(&s, '\0')) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                    "include path is too long; max: %zu", அளவு(s.buf));
        திரும்பு 0;
    }
    எனில் (unlikely(s.buf_pos > buf_size)) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                    "include path is too long: %zu > %zu", s.buf_pos, buf_size);
        திரும்பு 0;
    }
    memcpy(buf, s.buf, s.buf_pos);
    திரும்பு s.buf_pos;
}

ssize_t
expand_path(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து* parent_file_name,
            மாறிலி எழுத்து *name, size_t name_len, எண்ணிக்கை xkb_file_type type,
            எழுத்து *buf, size_t buf_size)
{
    /* Detect %-expansion */
    size_t k;
    க்கு (k = 0; k < name_len; k++) {
        எனில் (name[k] == '%') {
            செல் expand;
        }
    }

    திரும்பு 0;

expand:
    /* Process %-expansion */

    /* Copy prefix (usual case: k == 0) */
    எனில் (unlikely(k >= buf_size)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: %zu > %zu, "
                "got raw path: %.*s\n",
                k, buf_size, (unsigned முழு) name_len, name);
        திரும்பு -1;
    }
    memcpy(buf, name, k);

    /* Proceed to %-expansion */
    மாறிலி எழுத்து *typeDir = DirectoryForInclude(type);
    size_t மொத்தம் = expand_percent(ctx, parent_file_name, typeDir,
                                  buf + k, buf_size - k,
                                  name + k, name_len - k);
    எனில் (!மொத்தம்)
        திரும்பு -1;
    மொத்தம் += k;
    ப்ரதிஜ்ஞா(buf[மொத்தம் - 1] == '\0');
    திரும்பு (ssize_t) மொத்தம் - 1;
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
FindFileInXkbPath(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து* parent_file_name,
                  மாறிலி எழுத்து *name, size_t name_len, எண்ணிக்கை xkb_file_type type,
                  எழுத்து *buf, size_t buf_size, unsigned முழு *offset,
                  bool required)
{
    /* We do not handle absolute paths here */
    ப்ரதிஜ்ஞா(!is_absolute_path(name));

    FILE *file = வெற்று;
    எழுத்து *name_buffer = வெற்று;
    மாறிலி எழுத்து *typeDir = DirectoryForInclude(type);

    க்கு (unsigned முழு i = *offset; i < xkb_context_இலக்கம்_include_paths(ctx); i++) {
        எனில் (!snprintf_safe(buf, buf_size, "%s/%s/%.*s",
                           xkb_context_include_path_get(ctx, i),
                           typeDir, (unsigned முழு) name_len, name)) {
            log_err(ctx, XKB_ERROR_INVALID_PATH,
                    "Path is too long: expected max length of %zu, "
                    "got: %s/%s/%.*s\n",
                    buf_size, xkb_context_include_path_get(ctx, i),
                    typeDir, (unsigned முழு) name_len, name);
            தொடர்;
        }

        file = fopen(buf, "rb");
        எனில் (file) {
            *offset = i;
            செல் out;
        }
    }

    /* We only print warnings if we can’t find the file on the first lookup */
    எனில் (required && *offset == 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "Couldn't find file \"%s/%.*s\" in include paths\n",
                typeDir, (unsigned முழு) name_len, name);
        LogIncludePaths(ctx);
    }

out:
    விடுவி(name_buffer);
    திரும்பு file;
}

bool
ExceedsIncludeMaxDepth(கட்டமைப்பு xkb_context *ctx, unsigned முழு include_depth)
{
    எனில் (include_depth >= INCLUDE_MAX_DEPTH) {
        log_err(ctx, XKB_ERROR_RECURSIVE_INCLUDE,
                "Exceeded include depth threshold (%u)",
                INCLUDE_MAX_DEPTH);
        திரும்பு true;
    } இல்லையெனில் {
        திரும்பு false;
    }
}

XkbFile *
ProcessIncludeFile(கட்டமைப்பு xkb_context *ctx,
                   மாறிலி கட்டமைப்பு parser_keymap_config *config,
                   மாறிலி IncludeStmt *stmt,
                   எண்ணிக்கை xkb_file_type file_type, எழுத்து *path, size_t path_size)
{
    /*
     * Resolve include statement:
     * - Explicit map name: look for an *exact match* only.
     * - Default map: look for an *explicit default* map (i.e. tagged with
     *   `default`: exact match), else fallback to the first *implicit* default
     *   map (weak match).
     */
    XkbFile *xkb_file = வெற்று;  /* Exact match */
    XkbFile *candidate = வெற்று; /* Weak match */

    மாறிலி எழுத்து *stmt_file = stmt->file;
    size_t stmt_file_len = strlen(stmt_file);

    /* Process %-expansion, if any */
    // FIXME: use parent file name instead of “(unknow)”.
    மாறிலி ssize_t expanded = expand_path(ctx, "(unknown)",
                                         stmt_file, stmt_file_len, file_type,
                                         path, path_size);
    எனில் (expanded < 0) {
        /* Error */
        திரும்பு வெற்று;
    } இல்லையெனில் எனில் (expanded > 0) {
        /* %-expanded */
        stmt_file = path;
        stmt_file_len = (size_t) expanded;
        ப்ரதிஜ்ஞா(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    unsigned முழு offset = 0;
    மாறிலி bool absolute_path = is_absolute_path(stmt_file);
    எனில் (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        ப்ரதிஜ்ஞா(stmt_file[stmt_file_len] == '\0');
        file = fopen(stmt_file, "rb");
    } இல்லையெனில் {
        /* Relative path: lookup the first XKB path */
        எனில் (unlikely(expanded)) {
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
            file = வெற்று;
        } இல்லையெனில் {
            // FIXME: use parent file name instead of “(unknow)”.
            file = FindFileInXkbPath(ctx, "(unknown)",
                                     stmt_file, stmt_file_len, file_type,
                                     path, path_size, &offset, true);
        }
    }

    போது (file) {
        xkb_file = XkbParseFile(ctx, config, file, stmt->file, stmt->map);
        fclose(file);

        எனில் (xkb_file) {
            எனில் (xkb_file->file_type != file_type) {
                log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                        "Include file of wrong type (expected %s, got %s); "
                        "Include file \"%s\" ignored\n",
                        xkb_file_type_to_string(file_type),
                        xkb_file_type_to_string(xkb_file->file_type), stmt->file);
                FreeXkbFile(xkb_file);
                xkb_file = வெற்று;
            } இல்லையெனில் எனில் (stmt->map || (xkb_file->flags & MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops here.
                 */
                நிறுத்து;
            } இல்லையெனில் எனில் (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = xkb_file;
                xkb_file = வெற்று;
            } இல்லையெனில் {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                FreeXkbFile(xkb_file);
                xkb_file = வெற்று;
            }
        }

        எனில் (absolute_path) {
            /* There is no point to search further if the path is absolute */
            நிறுத்து;
        }

        offset++;
        file = FindFileInXkbPath(ctx, "(unknown)",
                                 stmt_file, stmt_file_len, file_type,
                                 path, path_size, &offset, true);
    }

    எனில் (!xkb_file) {
        /* No exact match: use weak match, if any */
        xkb_file = candidate;
    } இல்லையெனில் {
        /* Found exact match: discard weak match, if any */
        FreeXkbFile(candidate);
    }

    எனில் (!xkb_file) {
        எனில் (stmt->map)
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s(%s)'\n",
                    stmt->file, stmt->map);
        இல்லையெனில்
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s'\n",
                    stmt->file);
    }

    திரும்பு xkb_file;
}
