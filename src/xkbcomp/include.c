/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "messages-codes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp-priv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "include.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "scanner-utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-paths.h"

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
ParseIncludeMap(ਅੱਖਰ **str_inout, ਅੱਖਰ **file_rtrn, ਅੱਖਰ **map_rtrn,
                ਅੱਖਰ *nextop_rtrn, ਅੱਖਰ **extra_ਅੰਕੜੇ)
{
    ਅੱਖਰ *tmp, *str, *ਅਗਲਾ;

    str = *str_inout;

    /*
     * Find the position in the string where the next file is included,
     * if there is more than one left in the statement.
     */
    ਅਗਲਾ = strpbrk(str, ਮਿਲਾਓ_ਬਹੁਲਕ_PREFIXES);
    ਜੇ (ਅਗਲਾ) {
        /* Got more files, this function will be called again. */
        *nextop_rtrn = *ਅਗਲਾ;
        /* Separate the string, for strchr etc. to work on this file only. */
        *ਅਗਲਾ++ = '\0';
    }
    ਨਹੀਂਤਾਂ {
        /* This is the last file in this statement, won't be called again. */
        *nextop_rtrn = '\0';
        ਅਗਲਾ = ਨਿਰਾ;
    }

    /*
     * Search for the explicit group designator, if any. If it's there,
     * it goes after the file name and map.
     */
    tmp = strchr(str, ':');
    ਜੇ (tmp != ਨਿਰਾ) {
        *tmp++ = '\0';
        *extra_ਅੰਕੜੇ = strdup(tmp);
    }
    ਨਹੀਂਤਾਂ {
        *extra_ਅੰਕੜੇ = ਨਿਰਾ;
    }

    /* Look for a map, if any. */
    tmp = strchr(str, '(');
    ਜੇ (tmp == ਨਿਰਾ) {
        /* No map. */
        *file_rtrn = strdup(str);
        *map_rtrn = ਨਿਰਾ;
    }
    ਨਹੀਂਤਾਂ ਜੇ (str[0] == '(') {
        /* Map without file - invalid. */
        ਮੁਕਤ(*extra_ਅੰਕੜੇ);
        ਵਾਪਸ false;
    }
    ਨਹੀਂਤਾਂ {
        /* Got a map; separate the file and the map for the strdup's. */
        *tmp++ = '\0';
        *file_rtrn = strdup(str);
        str = tmp;
        tmp = strchr(str, ')');
        ਜੇ (tmp == ਨਿਰਾ || tmp[1] != '\0') {
            ਮੁਕਤ(*file_rtrn);
            ਮੁਕਤ(*extra_ਅੰਕੜੇ);
            ਵਾਪਸ false;
        }
        *tmp++ = '\0';
        *map_rtrn = strdup(str);
    }

    /* Set up the next file for the next call, if any. */
    ਜੇ (*nextop_rtrn == '\0')
        *str_inout = ਨਿਰਾ;
    ਨਹੀਂਤਾਂ ਜੇ (is_ਮਿਲਾਓ_ਬਹੁਲਕ_ਅਗੇਤਰ(*nextop_rtrn))
        *str_inout = ਅਗਲਾ;
    ਨਹੀਂਤਾਂ
        ਵਾਪਸ false;

    ਵਾਪਸ true;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *xkb_file_type_include_dirs[FILE_TYPE_ਅੰਕ_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "keycodes",
    [FILE_TYPE_TYPES] = "types",
    [FILE_TYPE_COMPAT] = "compat",
    [FILE_TYPE_SYMBOLS] = "symbols",
    [FILE_TYPE_GEOMETRY] = "geometry",
    [FILE_TYPE_KEYMAP] = "keymap",
    [FILE_TYPE_ਨਿਯਮਾਂ] = "rules",
};

/**
 * Return the xkb directory based on the type.
 */
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
DirectoryForInclude(ਗਿਣਤੀ xkb_file_type type)
{
    ਜੇ (type >= FILE_TYPE_ਅੰਕ_ENTRIES)
        ਵਾਪਸ "";
    ਵਾਪਸ xkb_file_type_include_dirs[type];
}

ਸਥਿਰ ਖਾਲੀ
LogIncludePaths(ਬਣਤਰ xkb_context *ctx)
{
    ਜੇ (xkb_context_ਅੰਕ_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_ਮਿਲਿਆ,
                "%u include paths searched:\n",
                xkb_context_ਅੰਕ_include_paths(ctx));
        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < xkb_context_ਅੰਕ_include_paths(ctx); i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_ਮਿਲਿਆ,
                    "\t%s\n",
                    xkb_context_include_ਰਸਤਾ_get(ctx, i));
    }
    ਨਹੀਂਤਾਂ {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_ਮਿਲਿਆ,
                "There are no include paths to search\n");
    }

    ਜੇ (xkb_context_ਅੰਕ_failed_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_ਮਿਲਿਆ,
                "%u include paths could not be added:\n",
                xkb_context_ਅੰਕ_failed_include_paths(ctx));
        ਲਈ (darray_ਮਾਪ_t i = 0;
             i < xkb_context_ਅੰਕ_failed_include_paths(ctx);
             i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_ਮਿਲਿਆ,
                    "\t%s\n",
                    xkb_context_failed_include_ਰਸਤਾ_get(ctx, i));
    }
}

ਸਥਿਰ ਮਾਪ_t
expand_percent(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ* ਮਾਪਾ_file_name,
               ਅਚਲ ਅੱਖਰ* typeDir, ਅੱਖਰ *buf, ਮਾਪ_t buf_ਮਾਪ,
               ਅਚਲ ਅੱਖਰ *name, ਮਾਪ_t name_len)
{
    ਬਣਤਰ scanner s;
    scanner_init(&s, ctx, name, name_len, ਮਾਪਾ_file_name, ਨਿਰਾ);
    s.buf_pos = 0;

    ਜਦੋਂ (!scanner_eof(&s) && !scanner_eol(&s)) {
        ਜੇ (scanner_chr(&s, '%')) {
            ਜੇ (scanner_chr(&s, '%')) {
                scanner_buf_append(&s, '%');
            }
            ਨਹੀਂਤਾਂ ਜੇ (scanner_chr(&s, 'H')) {
                ਅਚਲ ਅੱਖਰ *home = xkb_context_getenv(ctx, "HOME");
                ਜੇ (!home) {
                    scanner_err(&s, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                                "%%H was used in an include statement, but the "
                                "HOME environment variable is not set");
                    ਵਾਪਸ 0;
                }
                ਜੇ (!scanner_buf_appends(&s, home)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_ਬਫਰ_ਮਾਪ,
                                "include path after expanding %%H is too long");
                    ਵਾਪਸ 0;
                }
            }
            ਨਹੀਂਤਾਂ ਜੇ (scanner_chr(&s, 'S')) {
                ਅਚਲ ਅੱਖਰ *default_ਜੜ੍ਹ =
                    xkb_context_include_ਰਸਤਾ_get_system_ਰਸਤਾ(ctx);
                ਜੇ (!scanner_buf_appends(&s, default_ਜੜ੍ਹ) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_ਬਫਰ_ਮਾਪ,
                                "include path after expanding %%S is too long");
                    ਵਾਪਸ 0;
                }
            }
            ਨਹੀਂਤਾਂ ਜੇ (scanner_chr(&s, 'E')) {
                ਅਚਲ ਅੱਖਰ *default_ਜੜ੍ਹ =
                    xkb_context_include_ਰਸਤਾ_get_extra_ਰਸਤਾ(ctx);
                ਜੇ (!scanner_buf_appends(&s, default_ਜੜ੍ਹ) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_ਬਫਰ_ਮਾਪ,
                                "include path after expanding %%E is too long");
                    ਵਾਪਸ 0;
                }
            }
            ਨਹੀਂਤਾਂ {
                scanner_err(&s, XKB_ERROR_INSUFFICIENT_ਬਫਰ_ਮਾਪ,
                            "unknown %% format (%c) in include statement",
                            scanner_peek(&s));
                ਵਾਪਸ 0;
            }
        }
        ਨਹੀਂਤਾਂ {
            scanner_buf_append(&s, scanner_ਅਗਲਾ(&s));
        }
    }
    ਜੇ (!scanner_buf_append(&s, '\0')) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_ਬਫਰ_ਮਾਪ,
                    "include path is too long; max: %zu", ਆਕਾਰ(s.buf));
        ਵਾਪਸ 0;
    }
    ਜੇ (unlikely(s.buf_pos > buf_ਮਾਪ)) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_ਬਫਰ_ਮਾਪ,
                    "include path is too long: %zu > %zu", s.buf_pos, buf_ਮਾਪ);
        ਵਾਪਸ 0;
    }
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf, s.buf, s.buf_pos);
    ਵਾਪਸ s.buf_pos;
}

ssize_t
expand_ਰਸਤਾ(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ* ਮਾਪਾ_file_name,
            ਅਚਲ ਅੱਖਰ *name, ਮਾਪ_t name_len, ਗਿਣਤੀ xkb_file_type type,
            ਅੱਖਰ *buf, ਮਾਪ_t buf_ਮਾਪ)
{
    /* Detect %-expansion */
    ਮਾਪ_t k;
    ਲਈ (k = 0; k < name_len; k++) {
        ਜੇ (name[k] == '%') {
            ਜਾਓ expand;
        }
    }

    ਵਾਪਸ 0;

expand:
    /* Process %-expansion */

    /* Copy prefix (usual case: k == 0) */
    ਜੇ (unlikely(k >= buf_ਮਾਪ)) {
        log_err(ctx, XKB_ERROR_INVALID_ਰਸਤਾ,
                "Path is too long: %zu > %zu, "
                "got raw path: %.*s\n",
                k, buf_ਮਾਪ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) name_len, name);
        ਵਾਪਸ -1;
    }
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf, name, k);

    /* Proceed to %-expansion */
    ਅਚਲ ਅੱਖਰ *typeDir = DirectoryForInclude(type);
    ਮਾਪ_t ਗਿਣਤੀ_2 = expand_percent(ctx, ਮਾਪਾ_file_name, typeDir,
                                  buf + k, buf_ਮਾਪ - k,
                                  name + k, name_len - k);
    ਜੇ (!ਗਿਣਤੀ_2)
        ਵਾਪਸ -1;
    ਗਿਣਤੀ_2 += k;
    ਪ੍ਰਤਿਜ੍ਞਾ(buf[ਗਿਣਤੀ_2 - 1] == '\0');
    ਵਾਪਸ (ssize_t) ਗਿਣਤੀ_2 - 1;
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
ਲੱਭੋ_File_ਵਿੱਚ_Xkb_ਰਸਤਾ(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ* ਮਾਪਾ_file_name,
                  ਅਚਲ ਅੱਖਰ *name, ਮਾਪ_t name_len, ਗਿਣਤੀ xkb_file_type type,
                  ਅੱਖਰ *buf, ਮਾਪ_t buf_ਮਾਪ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਵਿੱਥ,
                  bool required)
{
    /* We do not handle absolute paths here */
    ਪ੍ਰਤਿਜ੍ਞਾ(!is_absolute_ਰਸਤਾ(name));

    FILE *file = ਨਿਰਾ;
    ਅੱਖਰ *name_ਬਫਰ = ਨਿਰਾ;
    ਅਚਲ ਅੱਖਰ *typeDir = DirectoryForInclude(type);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = *ਵਿੱਥ; i < xkb_context_ਅੰਕ_include_paths(ctx); i++) {
        ਜੇ (!snprintf_ਸੁਰੱਖਿਅਤ(buf, buf_ਮਾਪ, "%s/%s/%.*s",
                           xkb_context_include_ਰਸਤਾ_get(ctx, i),
                           typeDir, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) name_len, name)) {
            log_err(ctx, XKB_ERROR_INVALID_ਰਸਤਾ,
                    "Path is too long: expected max length of %zu, "
                    "got: %s/%s/%.*s\n",
                    buf_ਮਾਪ, xkb_context_include_ਰਸਤਾ_get(ctx, i),
                    typeDir, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) name_len, name);
            ਜਾਰੀ;
        }

        file = fopen(buf, "rb");
        ਜੇ (file) {
            *ਵਿੱਥ = i;
            ਜਾਓ ਬਾਹਰਲਾ;
        }
    }

    /* We only print warnings if we can’t find the file on the first lookup */
    ਜੇ (required && *ਵਿੱਥ == 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_ਮਿਲਿਆ,
                "Couldn't find file \"%s/%.*s\" in include paths\n",
                typeDir, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) name_len, name);
        LogIncludePaths(ctx);
    }

ਬਾਹਰਲਾ:
    ਮੁਕਤ(name_ਬਫਰ);
    ਵਾਪਸ file;
}

bool
Exceeds_Include_Max_ਡੂੰਘਾਈ(ਬਣਤਰ xkb_context *ctx, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ include_ਡੂੰਘਾਈ)
{
    ਜੇ (include_ਡੂੰਘਾਈ >= INCLUDE_MAX_ਡੂੰਘਾਈ) {
        log_err(ctx, XKB_ERROR_RECURSIVE_INCLUDE,
                "Exceeded include depth threshold (%u)",
                INCLUDE_MAX_ਡੂੰਘਾਈ);
        ਵਾਪਸ true;
    } ਨਹੀਂਤਾਂ {
        ਵਾਪਸ false;
    }
}

XkbFile *
ProcessIncludeFile(ਬਣਤਰ xkb_context *ctx,
                   ਅਚਲ ਬਣਤਰ parser_keymap_config *config,
                   ਅਚਲ IncludeStmt *stmt,
                   ਗਿਣਤੀ xkb_file_type file_type, ਅੱਖਰ *ਰਸਤਾ, ਮਾਪ_t ਰਸਤਾ_ਮਾਪ)
{
    /*
     * Resolve include statement:
     * - Explicit map name: look for an *exact match* only.
     * - Default map: look for an *explicit default* map (i.e. tagged with
     *   `default`: exact match), else fallback to the first *implicit* default
     *   map (weak match).
     */
    XkbFile *xkb_file = ਨਿਰਾ;  /* Exact match */
    XkbFile *candidate = ਨਿਰਾ; /* Weak match */

    ਅਚਲ ਅੱਖਰ *stmt_file = stmt->file;
    ਮਾਪ_t stmt_file_len = ਲੜੀ_ਲੰਬਾਈ(stmt_file);

    /* Process %-expansion, if any */
    // FIXME: use parent file name instead of “(unknow)”.
    ਅਚਲ ssize_t expanded = expand_ਰਸਤਾ(ctx, "(unknown)",
                                         stmt_file, stmt_file_len, file_type,
                                         ਰਸਤਾ, ਰਸਤਾ_ਮਾਪ);
    ਜੇ (expanded < 0) {
        /* Error */
        ਵਾਪਸ ਨਿਰਾ;
    } ਨਹੀਂਤਾਂ ਜੇ (expanded > 0) {
        /* %-expanded */
        stmt_file = ਰਸਤਾ;
        stmt_file_len = (ਮਾਪ_t) expanded;
        ਪ੍ਰਤਿਜ੍ਞਾ(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ = 0;
    ਅਚਲ bool absolute_ਰਸਤਾ = is_absolute_ਰਸਤਾ(stmt_file);
    ਜੇ (absolute_ਰਸਤਾ) {
        /* Absolute path: no need for lookup in XKB paths */
        ਪ੍ਰਤਿਜ੍ਞਾ(stmt_file[stmt_file_len] == '\0');
        file = fopen(stmt_file, "rb");
    } ਨਹੀਂਤਾਂ {
        /* Relative path: lookup the first XKB path */
        ਜੇ (unlikely(expanded)) {
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
            file = ਨਿਰਾ;
        } ਨਹੀਂਤਾਂ {
            // FIXME: use parent file name instead of “(unknow)”.
            file = ਲੱਭੋ_File_ਵਿੱਚ_Xkb_ਰਸਤਾ(ctx, "(unknown)",
                                     stmt_file, stmt_file_len, file_type,
                                     ਰਸਤਾ, ਰਸਤਾ_ਮਾਪ, &ਵਿੱਥ, true);
        }
    }

    ਜਦੋਂ (file) {
        xkb_file = XkbParseFile(ctx, config, file, stmt->file, stmt->map);
        fclose(file);

        ਜੇ (xkb_file) {
            ਜੇ (xkb_file->file_type != file_type) {
                log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                        "Include file of wrong type (expected %s, got %s); "
                        "Include file \"%s\" ignored\n",
                        xkb_file_type_ਮੰਜ਼ਿਲ_string(file_type),
                        xkb_file_type_ਮੰਜ਼ਿਲ_string(xkb_file->file_type), stmt->file);
                FreeXkbFile(xkb_file);
                xkb_file = ਨਿਰਾ;
            } ਨਹੀਂਤਾਂ ਜੇ (stmt->map || (xkb_file->flags & MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops here.
                 */
                ਰੋਕੋ;
            } ਨਹੀਂਤਾਂ ਜੇ (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = xkb_file;
                xkb_file = ਨਿਰਾ;
            } ਨਹੀਂਤਾਂ {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                FreeXkbFile(xkb_file);
                xkb_file = ਨਿਰਾ;
            }
        }

        ਜੇ (absolute_ਰਸਤਾ) {
            /* There is no point to search further if the path is absolute */
            ਰੋਕੋ;
        }

        ਵਿੱਥ++;
        file = ਲੱਭੋ_File_ਵਿੱਚ_Xkb_ਰਸਤਾ(ctx, "(unknown)",
                                 stmt_file, stmt_file_len, file_type,
                                 ਰਸਤਾ, ਰਸਤਾ_ਮਾਪ, &ਵਿੱਥ, true);
    }

    ਜੇ (!xkb_file) {
        /* No exact match: use weak match, if any */
        xkb_file = candidate;
    } ਨਹੀਂਤਾਂ {
        /* Found exact match: discard weak match, if any */
        FreeXkbFile(candidate);
    }

    ਜੇ (!xkb_file) {
        ਜੇ (stmt->map)
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s(%s)'\n",
                    stmt->file, stmt->map);
        ਨਹੀਂਤਾਂ
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s'\n",
                    stmt->file);
    }

    ਵਾਪਸ xkb_file;
}
