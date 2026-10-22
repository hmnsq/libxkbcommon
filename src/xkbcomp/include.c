/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#شامل "config.h"

#شامل <errno.h>
#شامل <limits.h>
#شامل <stdio.h>
#شامل <string.h>

#شامل "messages-codes.h"
#شامل "utils.h"
#شامل "xkbcomp-priv.h"
#شامل "include.h"
#شامل "scanner-utils.h"
#شامل "util-paths.h"

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
                حرف *nextop_rtrn, حرف **extra_ڈیٹا)
{
    حرف *tmp, *str, *اگلا;

    str = *str_inout;

    /*
     * Find the position in the string where the next file is included,
     * if there is more than one left in the statement.
     */
    اگلا = strpbrk(str, ملاؤ_منوال_PREFIXES);
    اگر (اگلا) {
        /* Got more files, this function will be called again. */
        *nextop_rtrn = *اگلا;
        /* Separate the string, for strchr etc. to work on this file only. */
        *اگلا++ = '\0';
    }
    ورنہ {
        /* This is the last file in this statement, won't be called again. */
        *nextop_rtrn = '\0';
        اگلا = تہی;
    }

    /*
     * Search for the explicit group designator, if any. If it's there,
     * it goes after the file name and map.
     */
    tmp = strchr(str, ':');
    اگر (tmp != تہی) {
        *tmp++ = '\0';
        *extra_ڈیٹا = strdup(tmp);
    }
    ورنہ {
        *extra_ڈیٹا = تہی;
    }

    /* Look for a map, if any. */
    tmp = strchr(str, '(');
    اگر (tmp == تہی) {
        /* No map. */
        *file_rtrn = strdup(str);
        *map_rtrn = تہی;
    }
    ورنہ اگر (str[0] == '(') {
        /* Map without file - invalid. */
        آزاد(*extra_ڈیٹا);
        واپس false;
    }
    ورنہ {
        /* Got a map; separate the file and the map for the strdup's. */
        *tmp++ = '\0';
        *file_rtrn = strdup(str);
        str = tmp;
        tmp = strchr(str, ')');
        اگر (tmp == تہی || tmp[1] != '\0') {
            آزاد(*file_rtrn);
            آزاد(*extra_ڈیٹا);
            واپس false;
        }
        *tmp++ = '\0';
        *map_rtrn = strdup(str);
    }

    /* Set up the next file for the next call, if any. */
    اگر (*nextop_rtrn == '\0')
        *str_inout = تہی;
    ورنہ اگر (is_ملاؤ_منوال_سابقہ(*nextop_rtrn))
        *str_inout = اگلا;
    ورنہ
        واپس false;

    واپس true;
}

ساکن ثابت حرف *xkb_file_type_include_dirs[FILE_TYPE_عدد_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "keycodes",
    [FILE_TYPE_TYPES] = "types",
    [FILE_TYPE_COMPAT] = "compat",
    [FILE_TYPE_SYMBOLS] = "symbols",
    [FILE_TYPE_GEOMETRY] = "geometry",
    [FILE_TYPE_KEYMAP] = "keymap",
    [FILE_TYPE_قواعد] = "rules",
};

/**
 * Return the xkb directory based on the type.
 */
ساکن ثابت حرف *
DirectoryForInclude(شمار xkb_file_type type)
{
    اگر (type >= FILE_TYPE_عدد_ENTRIES)
        واپس "";
    واپس xkb_file_type_include_dirs[type];
}

ساکن خالی
LogIncludePaths(ساخت xkb_context *ctx)
{
    اگر (xkb_context_عدد_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_ملا,
                "%u include paths searched:\n",
                xkb_context_عدد_include_paths(ctx));
        برائے (غیرعلامتی صحیح i = 0; i < xkb_context_عدد_include_paths(ctx); i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_ملا,
                    "\t%s\n",
                    xkb_context_include_راستہ_get(ctx, i));
    }
    ورنہ {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_ملا,
                "There are no include paths to search\n");
    }

    اگر (xkb_context_عدد_failed_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_ملا,
                "%u include paths could not be added:\n",
                xkb_context_عدد_failed_include_paths(ctx));
        برائے (darray_سائز_t i = 0;
             i < xkb_context_عدد_failed_include_paths(ctx);
             i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_ملا,
                    "\t%s\n",
                    xkb_context_failed_include_راستہ_get(ctx, i));
    }
}

ساکن سائز_t
expand_percent(ساخت xkb_context *ctx, ثابت حرف* والد_file_name,
               ثابت حرف* typeDir, حرف *buf, سائز_t buf_سائز,
               ثابت حرف *name, سائز_t name_len)
{
    ساخت scanner s;
    scanner_init(&s, ctx, name, name_len, والد_file_name, تہی);
    s.buf_pos = 0;

    جب (!scanner_eof(&s) && !scanner_eol(&s)) {
        اگر (scanner_chr(&s, '%')) {
            اگر (scanner_chr(&s, '%')) {
                scanner_buf_append(&s, '%');
            }
            ورنہ اگر (scanner_chr(&s, 'H')) {
                ثابت حرف *home = xkb_context_getenv(ctx, "HOME");
                اگر (!home) {
                    scanner_err(&s, XKB_LOG_پیغام_NO_ID,
                                "%%H was used in an include statement, but the "
                                "HOME environment variable is not set");
                    واپس 0;
                }
                اگر (!scanner_buf_appends(&s, home)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_بفر_سائز,
                                "include path after expanding %%H is too long");
                    واپس 0;
                }
            }
            ورنہ اگر (scanner_chr(&s, 'S')) {
                ثابت حرف *default_جڑ =
                    xkb_context_include_راستہ_get_system_راستہ(ctx);
                اگر (!scanner_buf_appends(&s, default_جڑ) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_بفر_سائز,
                                "include path after expanding %%S is too long");
                    واپس 0;
                }
            }
            ورنہ اگر (scanner_chr(&s, 'E')) {
                ثابت حرف *default_جڑ =
                    xkb_context_include_راستہ_get_extra_راستہ(ctx);
                اگر (!scanner_buf_appends(&s, default_جڑ) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_بفر_سائز,
                                "include path after expanding %%E is too long");
                    واپس 0;
                }
            }
            ورنہ {
                scanner_err(&s, XKB_ERROR_INSUFFICIENT_بفر_سائز,
                            "unknown %% format (%c) in include statement",
                            scanner_peek(&s));
                واپس 0;
            }
        }
        ورنہ {
            scanner_buf_append(&s, scanner_اگلا(&s));
        }
    }
    اگر (!scanner_buf_append(&s, '\0')) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_بفر_سائز,
                    "include path is too long; max: %zu", حجم(s.buf));
        واپس 0;
    }
    اگر (unlikely(s.buf_pos > buf_سائز)) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_بفر_سائز,
                    "include path is too long: %zu > %zu", s.buf_pos, buf_سائز);
        واپس 0;
    }
    حافظہ_نقل(buf, s.buf, s.buf_pos);
    واپس s.buf_pos;
}

ssize_t
expand_راستہ(ساخت xkb_context *ctx, ثابت حرف* والد_file_name,
            ثابت حرف *name, سائز_t name_len, شمار xkb_file_type type,
            حرف *buf, سائز_t buf_سائز)
{
    /* Detect %-expansion */
    سائز_t k;
    برائے (k = 0; k < name_len; k++) {
        اگر (name[k] == '%') {
            جاؤ expand;
        }
    }

    واپس 0;

expand:
    /* Process %-expansion */

    /* Copy prefix (usual case: k == 0) */
    اگر (unlikely(k >= buf_سائز)) {
        log_err(ctx, XKB_ERROR_INVALID_راستہ,
                "Path is too long: %zu > %zu, "
                "got raw path: %.*s\n",
                k, buf_سائز, (غیرعلامتی صحیح) name_len, name);
        واپس -1;
    }
    حافظہ_نقل(buf, name, k);

    /* Proceed to %-expansion */
    ثابت حرف *typeDir = DirectoryForInclude(type);
    سائز_t گنتی = expand_percent(ctx, والد_file_name, typeDir,
                                  buf + k, buf_سائز - k,
                                  name + k, name_len - k);
    اگر (!گنتی)
        واپس -1;
    گنتی += k;
    أكد(buf[گنتی - 1] == '\0');
    واپس (ssize_t) گنتی - 1;
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
ڈھونڈو_File_اندر_Xkb_راستہ(ساخت xkb_context *ctx, ثابت حرف* والد_file_name,
                  ثابت حرف *name, سائز_t name_len, شمار xkb_file_type type,
                  حرف *buf, سائز_t buf_سائز, غیرعلامتی صحیح *انحراف,
                  bool required)
{
    /* We do not handle absolute paths here */
    أكد(!is_absolute_راستہ(name));

    FILE *file = تہی;
    حرف *name_بفر = تہی;
    ثابت حرف *typeDir = DirectoryForInclude(type);

    برائے (غیرعلامتی صحیح i = *انحراف; i < xkb_context_عدد_include_paths(ctx); i++) {
        اگر (!snprintf_محفوظ(buf, buf_سائز, "%s/%s/%.*s",
                           xkb_context_include_راستہ_get(ctx, i),
                           typeDir, (غیرعلامتی صحیح) name_len, name)) {
            log_err(ctx, XKB_ERROR_INVALID_راستہ,
                    "Path is too long: expected max length of %zu, "
                    "got: %s/%s/%.*s\n",
                    buf_سائز, xkb_context_include_راستہ_get(ctx, i),
                    typeDir, (غیرعلامتی صحیح) name_len, name);
            جاری;
        }

        file = fopen(buf, "rb");
        اگر (file) {
            *انحراف = i;
            جاؤ برآمد;
        }
    }

    /* We only print warnings if we can’t find the file on the first lookup */
    اگر (required && *انحراف == 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_ملا,
                "Couldn't find file \"%s/%.*s\" in include paths\n",
                typeDir, (غیرعلامتی صحیح) name_len, name);
        LogIncludePaths(ctx);
    }

برآمد:
    آزاد(name_بفر);
    واپس file;
}

bool
Exceeds_Include_Max_گہرائی(ساخت xkb_context *ctx, غیرعلامتی صحیح include_گہرائی)
{
    اگر (include_گہرائی >= INCLUDE_MAX_گہرائی) {
        log_err(ctx, XKB_ERROR_RECURSIVE_INCLUDE,
                "Exceeded include depth threshold (%u)",
                INCLUDE_MAX_گہرائی);
        واپس true;
    } ورنہ {
        واپس false;
    }
}

XkbFile *
ProcessIncludeFile(ساخت xkb_context *ctx,
                   ثابت ساخت parser_keymap_config *config,
                   ثابت IncludeStmt *stmt,
                   شمار xkb_file_type file_type, حرف *راستہ, سائز_t راستہ_سائز)
{
    /*
     * Resolve include statement:
     * - Explicit map name: look for an *exact match* only.
     * - Default map: look for an *explicit default* map (i.e. tagged with
     *   `default`: exact match), else fallback to the first *implicit* default
     *   map (weak match).
     */
    XkbFile *xkb_file = تہی;  /* Exact match */
    XkbFile *candidate = تہی; /* Weak match */

    ثابت حرف *stmt_file = stmt->file;
    سائز_t stmt_file_len = لڑی_لمبائی(stmt_file);

    /* Process %-expansion, if any */
    // FIXME: use parent file name instead of “(unknow)”.
    ثابت ssize_t expanded = expand_راستہ(ctx, "(unknown)",
                                         stmt_file, stmt_file_len, file_type,
                                         راستہ, راستہ_سائز);
    اگر (expanded < 0) {
        /* Error */
        واپس تہی;
    } ورنہ اگر (expanded > 0) {
        /* %-expanded */
        stmt_file = راستہ;
        stmt_file_len = (سائز_t) expanded;
        أكد(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    غیرعلامتی صحیح انحراف = 0;
    ثابت bool absolute_راستہ = is_absolute_راستہ(stmt_file);
    اگر (absolute_راستہ) {
        /* Absolute path: no need for lookup in XKB paths */
        أكد(stmt_file[stmt_file_len] == '\0');
        file = fopen(stmt_file, "rb");
    } ورنہ {
        /* Relative path: lookup the first XKB path */
        اگر (unlikely(expanded)) {
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
            file = تہی;
        } ورنہ {
            // FIXME: use parent file name instead of “(unknow)”.
            file = ڈھونڈو_File_اندر_Xkb_راستہ(ctx, "(unknown)",
                                     stmt_file, stmt_file_len, file_type,
                                     راستہ, راستہ_سائز, &انحراف, true);
        }
    }

    جب (file) {
        xkb_file = XkbParseFile(ctx, config, file, stmt->file, stmt->map);
        fclose(file);

        اگر (xkb_file) {
            اگر (xkb_file->file_type != file_type) {
                log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                        "Include file of wrong type (expected %s, got %s); "
                        "Include file \"%s\" ignored\n",
                        xkb_file_type_مقصد_string(file_type),
                        xkb_file_type_مقصد_string(xkb_file->file_type), stmt->file);
                FreeXkbFile(xkb_file);
                xkb_file = تہی;
            } ورنہ اگر (stmt->map || (xkb_file->flags & MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops here.
                 */
                توڑو;
            } ورنہ اگر (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = xkb_file;
                xkb_file = تہی;
            } ورنہ {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                FreeXkbFile(xkb_file);
                xkb_file = تہی;
            }
        }

        اگر (absolute_راستہ) {
            /* There is no point to search further if the path is absolute */
            توڑو;
        }

        انحراف++;
        file = ڈھونڈو_File_اندر_Xkb_راستہ(ctx, "(unknown)",
                                 stmt_file, stmt_file_len, file_type,
                                 راستہ, راستہ_سائز, &انحراف, true);
    }

    اگر (!xkb_file) {
        /* No exact match: use weak match, if any */
        xkb_file = candidate;
    } ورنہ {
        /* Found exact match: discard weak match, if any */
        FreeXkbFile(candidate);
    }

    اگر (!xkb_file) {
        اگر (stmt->map)
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s(%s)'\n",
                    stmt->file, stmt->map);
        ورنہ
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s'\n",
                    stmt->file);
    }

    واپس xkb_file;
}
