/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <limits.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

#ଅନ୍ତର୍ଭାଵ "messages-codes.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp-priv.h"
#ଅନ୍ତର୍ଭାଵ "include.h"
#ଅନ୍ତର୍ଭାଵ "scanner-utils.h"
#ଅନ୍ତର୍ଭାଵ "util-paths.h"

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
ParseIncludeMap(ଅକ୍ଷର **str_inout, ଅକ୍ଷର **file_rtrn, ଅକ୍ଷର **map_rtrn,
                ଅକ୍ଷର *nextop_rtrn, ଅକ୍ଷର **extra_data)
{
    ଅକ୍ଷର *tmp, *str, *next;

    str = *str_inout;

    /*
     * Find the position in the string where the next file is included,
     * if there is more than one left in the statement.
     */
    next = strpbrk(str, MERGE_MODE_PREFIXES);
    ଯଦି (next) {
        /* Got more files, this function will be called again. */
        *nextop_rtrn = *next;
        /* Separate the string, for strchr etc. to work on this file only. */
        *next++ = '\0';
    }
    ଅନ୍ୟଥା {
        /* This is the last file in this statement, won't be called again. */
        *nextop_rtrn = '\0';
        next = ଖାଲି;
    }

    /*
     * Search for the explicit group designator, if any. If it's there,
     * it goes after the file name and map.
     */
    tmp = strchr(str, ':');
    ଯଦି (tmp != ଖାଲି) {
        *tmp++ = '\0';
        *extra_data = strdup(tmp);
    }
    ଅନ୍ୟଥା {
        *extra_data = ଖାଲି;
    }

    /* Look for a map, if any. */
    tmp = strchr(str, '(');
    ଯଦି (tmp == ଖାଲି) {
        /* No map. */
        *file_rtrn = strdup(str);
        *map_rtrn = ଖାଲି;
    }
    ଅନ୍ୟଥା ଯଦି (str[0] == '(') {
        /* Map without file - invalid. */
        ମୁକ୍ତ(*extra_data);
        ଫେରନ୍ତୁ false;
    }
    ଅନ୍ୟଥା {
        /* Got a map; separate the file and the map for the strdup's. */
        *tmp++ = '\0';
        *file_rtrn = strdup(str);
        str = tmp;
        tmp = strchr(str, ')');
        ଯଦି (tmp == ଖାଲି || tmp[1] != '\0') {
            ମୁକ୍ତ(*file_rtrn);
            ମୁକ୍ତ(*extra_data);
            ଫେରନ୍ତୁ false;
        }
        *tmp++ = '\0';
        *map_rtrn = strdup(str);
    }

    /* Set up the next file for the next call, if any. */
    ଯଦି (*nextop_rtrn == '\0')
        *str_inout = ଖାଲି;
    ଅନ୍ୟଥା ଯଦି (is_merge_mode_prefix(*nextop_rtrn))
        *str_inout = next;
    ଅନ୍ୟଥା
        ଫେରନ୍ତୁ false;

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *xkb_file_type_include_dirs[FILE_TYPE_ଅଙ୍କ_ENTRIES] = {
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
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
DirectoryForInclude(ଗଣନା xkb_file_type type)
{
    ଯଦି (type >= FILE_TYPE_ଅଙ୍କ_ENTRIES)
        ଫେରନ୍ତୁ "";
    ଫେରନ୍ତୁ xkb_file_type_include_dirs[type];
}

ସ୍ଥିର ଶୂନ୍ୟ
LogIncludePaths(ସଂରଚନା xkb_context *ctx)
{
    ଯଦି (xkb_context_ଅଙ୍କ_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "%u include paths searched:\n",
                xkb_context_ଅଙ୍କ_include_paths(ctx));
        ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < xkb_context_ଅଙ୍କ_include_paths(ctx); i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "\t%s\n",
                    xkb_context_include_path_get(ctx, i));
    }
    ଅନ୍ୟଥା {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "There are no include paths to search\n");
    }

    ଯଦି (xkb_context_ଅଙ୍କ_failed_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "%u include paths could not be added:\n",
                xkb_context_ଅଙ୍କ_failed_include_paths(ctx));
        ପାଇଁ (darray_size_t i = 0;
             i < xkb_context_ଅଙ୍କ_failed_include_paths(ctx);
             i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "\t%s\n",
                    xkb_context_failed_include_path_get(ctx, i));
    }
}

ସ୍ଥିର size_t
expand_percent(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର* parent_file_name,
               ଅଚଳ ଅକ୍ଷର* typeDir, ଅକ୍ଷର *buf, size_t buf_size,
               ଅଚଳ ଅକ୍ଷର *name, size_t name_len)
{
    ସଂରଚନା scanner s;
    scanner_init(&s, ctx, name, name_len, parent_file_name, ଖାଲି);
    s.buf_pos = 0;

    ଯେତେବେଳେ (!scanner_eof(&s) && !scanner_eol(&s)) {
        ଯଦି (scanner_chr(&s, '%')) {
            ଯଦି (scanner_chr(&s, '%')) {
                scanner_buf_append(&s, '%');
            }
            ଅନ୍ୟଥା ଯଦି (scanner_chr(&s, 'H')) {
                ଅଚଳ ଅକ୍ଷର *home = xkb_context_getenv(ctx, "HOME");
                ଯଦି (!home) {
                    scanner_err(&s, XKB_LOG_MESSAGE_NO_ID,
                                "%%H was used in an include statement, but the "
                                "HOME environment variable is not set");
                    ଫେରନ୍ତୁ 0;
                }
                ଯଦି (!scanner_buf_appends(&s, home)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%H is too long");
                    ଫେରନ୍ତୁ 0;
                }
            }
            ଅନ୍ୟଥା ଯଦି (scanner_chr(&s, 'S')) {
                ଅଚଳ ଅକ୍ଷର *default_ମୂଳ =
                    xkb_context_include_path_get_system_path(ctx);
                ଯଦି (!scanner_buf_appends(&s, default_ମୂଳ) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%S is too long");
                    ଫେରନ୍ତୁ 0;
                }
            }
            ଅନ୍ୟଥା ଯଦି (scanner_chr(&s, 'E')) {
                ଅଚଳ ଅକ୍ଷର *default_ମୂଳ =
                    xkb_context_include_path_get_extra_path(ctx);
                ଯଦି (!scanner_buf_appends(&s, default_ମୂଳ) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%E is too long");
                    ଫେରନ୍ତୁ 0;
                }
            }
            ଅନ୍ୟଥା {
                scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                            "unknown %% format (%c) in include statement",
                            scanner_peek(&s));
                ଫେରନ୍ତୁ 0;
            }
        }
        ଅନ୍ୟଥା {
            scanner_buf_append(&s, scanner_next(&s));
        }
    }
    ଯଦି (!scanner_buf_append(&s, '\0')) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                    "include path is too long; max: %zu", ଆକାର(s.buf));
        ଫେରନ୍ତୁ 0;
    }
    ଯଦି (unlikely(s.buf_pos > buf_size)) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                    "include path is too long: %zu > %zu", s.buf_pos, buf_size);
        ଫେରନ୍ତୁ 0;
    }
    memcpy(buf, s.buf, s.buf_pos);
    ଫେରନ୍ତୁ s.buf_pos;
}

ssize_t
expand_path(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର* parent_file_name,
            ଅଚଳ ଅକ୍ଷର *name, size_t name_len, ଗଣନା xkb_file_type type,
            ଅକ୍ଷର *buf, size_t buf_size)
{
    /* Detect %-expansion */
    size_t k;
    ପାଇଁ (k = 0; k < name_len; k++) {
        ଯଦି (name[k] == '%') {
            ଯାଅ expand;
        }
    }

    ଫେରନ୍ତୁ 0;

expand:
    /* Process %-expansion */

    /* Copy prefix (usual case: k == 0) */
    ଯଦି (unlikely(k >= buf_size)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: %zu > %zu, "
                "got raw path: %.*s\n",
                k, buf_size, (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) name_len, name);
        ଫେରନ୍ତୁ -1;
    }
    memcpy(buf, name, k);

    /* Proceed to %-expansion */
    ଅଚଳ ଅକ୍ଷର *typeDir = DirectoryForInclude(type);
    size_t ଗଣନା_2 = expand_percent(ctx, parent_file_name, typeDir,
                                  buf + k, buf_size - k,
                                  name + k, name_len - k);
    ଯଦି (!ଗଣନା_2)
        ଫେରନ୍ତୁ -1;
    ଗଣନା_2 += k;
    ପ୍ରତିଜ୍ଞା(buf[ଗଣନା_2 - 1] == '\0');
    ଫେରନ୍ତୁ (ssize_t) ଗଣନା_2 - 1;
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
FindFileInXkbPath(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର* parent_file_name,
                  ଅଚଳ ଅକ୍ଷର *name, size_t name_len, ଗଣନା xkb_file_type type,
                  ଅକ୍ଷର *buf, size_t buf_size, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *offset,
                  bool required)
{
    /* We do not handle absolute paths here */
    ପ୍ରତିଜ୍ଞା(!is_absolute_path(name));

    FILE *file = ଖାଲି;
    ଅକ୍ଷର *name_buffer = ଖାଲି;
    ଅଚଳ ଅକ୍ଷର *typeDir = DirectoryForInclude(type);

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = *offset; i < xkb_context_ଅଙ୍କ_include_paths(ctx); i++) {
        ଯଦି (!snprintf_safe(buf, buf_size, "%s/%s/%.*s",
                           xkb_context_include_path_get(ctx, i),
                           typeDir, (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) name_len, name)) {
            log_err(ctx, XKB_ERROR_INVALID_PATH,
                    "Path is too long: expected max length of %zu, "
                    "got: %s/%s/%.*s\n",
                    buf_size, xkb_context_include_path_get(ctx, i),
                    typeDir, (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) name_len, name);
            ଜାରି;
        }

        file = fopen(buf, "rb");
        ଯଦି (file) {
            *offset = i;
            ଯାଅ out;
        }
    }

    /* We only print warnings if we can’t find the file on the first lookup */
    ଯଦି (required && *offset == 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "Couldn't find file \"%s/%.*s\" in include paths\n",
                typeDir, (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) name_len, name);
        LogIncludePaths(ctx);
    }

out:
    ମୁକ୍ତ(name_buffer);
    ଫେରନ୍ତୁ file;
}

bool
ExceedsIncludeMaxDepth(ସଂରଚନା xkb_context *ctx, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_depth)
{
    ଯଦି (include_depth >= INCLUDE_MAX_DEPTH) {
        log_err(ctx, XKB_ERROR_RECURSIVE_INCLUDE,
                "Exceeded include depth threshold (%u)",
                INCLUDE_MAX_DEPTH);
        ଫେରନ୍ତୁ true;
    } ଅନ୍ୟଥା {
        ଫେରନ୍ତୁ false;
    }
}

XkbFile *
ProcessIncludeFile(ସଂରଚନା xkb_context *ctx,
                   ଅଚଳ ସଂରଚନା parser_keymap_config *config,
                   ଅଚଳ IncludeStmt *stmt,
                   ଗଣନା xkb_file_type file_type, ଅକ୍ଷର *path, size_t path_size)
{
    /*
     * Resolve include statement:
     * - Explicit map name: look for an *exact match* only.
     * - Default map: look for an *explicit default* map (i.e. tagged with
     *   `default`: exact match), else fallback to the first *implicit* default
     *   map (weak match).
     */
    XkbFile *xkb_file = ଖାଲି;  /* Exact match */
    XkbFile *candidate = ଖାଲି; /* Weak match */

    ଅଚଳ ଅକ୍ଷର *stmt_file = stmt->file;
    size_t stmt_file_len = strlen(stmt_file);

    /* Process %-expansion, if any */
    // FIXME: use parent file name instead of “(unknow)”.
    ଅଚଳ ssize_t expanded = expand_path(ctx, "(unknown)",
                                         stmt_file, stmt_file_len, file_type,
                                         path, path_size);
    ଯଦି (expanded < 0) {
        /* Error */
        ଫେରନ୍ତୁ ଖାଲି;
    } ଅନ୍ୟଥା ଯଦି (expanded > 0) {
        /* %-expanded */
        stmt_file = path;
        stmt_file_len = (size_t) expanded;
        ପ୍ରତିଜ୍ଞା(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = 0;
    ଅଚଳ bool absolute_path = is_absolute_path(stmt_file);
    ଯଦି (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        ପ୍ରତିଜ୍ଞା(stmt_file[stmt_file_len] == '\0');
        file = fopen(stmt_file, "rb");
    } ଅନ୍ୟଥା {
        /* Relative path: lookup the first XKB path */
        ଯଦି (unlikely(expanded)) {
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
            file = ଖାଲି;
        } ଅନ୍ୟଥା {
            // FIXME: use parent file name instead of “(unknow)”.
            file = FindFileInXkbPath(ctx, "(unknown)",
                                     stmt_file, stmt_file_len, file_type,
                                     path, path_size, &offset, true);
        }
    }

    ଯେତେବେଳେ (file) {
        xkb_file = XkbParseFile(ctx, config, file, stmt->file, stmt->map);
        fclose(file);

        ଯଦି (xkb_file) {
            ଯଦି (xkb_file->file_type != file_type) {
                log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                        "Include file of wrong type (expected %s, got %s); "
                        "Include file \"%s\" ignored\n",
                        xkb_file_type_to_string(file_type),
                        xkb_file_type_to_string(xkb_file->file_type), stmt->file);
                FreeXkbFile(xkb_file);
                xkb_file = ଖାଲି;
            } ଅନ୍ୟଥା ଯଦି (stmt->map || (xkb_file->flags & MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops here.
                 */
                ଭାଙ୍ଗ;
            } ଅନ୍ୟଥା ଯଦି (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = xkb_file;
                xkb_file = ଖାଲି;
            } ଅନ୍ୟଥା {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                FreeXkbFile(xkb_file);
                xkb_file = ଖାଲି;
            }
        }

        ଯଦି (absolute_path) {
            /* There is no point to search further if the path is absolute */
            ଭାଙ୍ଗ;
        }

        offset++;
        file = FindFileInXkbPath(ctx, "(unknown)",
                                 stmt_file, stmt_file_len, file_type,
                                 path, path_size, &offset, true);
    }

    ଯଦି (!xkb_file) {
        /* No exact match: use weak match, if any */
        xkb_file = candidate;
    } ଅନ୍ୟଥା {
        /* Found exact match: discard weak match, if any */
        FreeXkbFile(candidate);
    }

    ଯଦି (!xkb_file) {
        ଯଦି (stmt->map)
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s(%s)'\n",
                    stmt->file, stmt->map);
        ଅନ୍ୟଥା
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s'\n",
                    stmt->file);
    }

    ଫେରନ୍ତୁ xkb_file;
}
