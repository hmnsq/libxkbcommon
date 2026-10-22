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
                حرف *nextop_rtrn, حرف **extra_معلومات)
{
    حرف *tmp, *str, *ايندڙ;

    str = *str_inout;

    /*
     * Find the position in the string where the next file is included,
     * if there is more than one left in the statement.
     */
    ايندڙ = strpbrk(str, ملايو_گهڻائي_PREFIXES);
    إذا (ايندڙ) {
        /* Got more files, this function will be called again. */
        *nextop_rtrn = *ايندڙ;
        /* Separate the string, for strchr etc. to work on this file only. */
        *ايندڙ++ = '\0';
    }
    وإلا {
        /* This is the last file in this statement, won't be called again. */
        *nextop_rtrn = '\0';
        ايندڙ = عدم;
    }

    /*
     * Search for the explicit group designator, if any. If it's there,
     * it goes after the file name and map.
     */
    tmp = strchr(str, ':');
    إذا (tmp != عدم) {
        *tmp++ = '\0';
        *extra_معلومات = strdup(tmp);
    }
    وإلا {
        *extra_معلومات = عدم;
    }

    /* Look for a map, if any. */
    tmp = strchr(str, '(');
    إذا (tmp == عدم) {
        /* No map. */
        *file_rtrn = strdup(str);
        *map_rtrn = عدم;
    }
    وإلا إذا (str[0] == '(') {
        /* Map without file - invalid. */
        حرر(*extra_معلومات);
        أرجع false;
    }
    وإلا {
        /* Got a map; separate the file and the map for the strdup's. */
        *tmp++ = '\0';
        *file_rtrn = strdup(str);
        str = tmp;
        tmp = strchr(str, ')');
        إذا (tmp == عدم || tmp[1] != '\0') {
            حرر(*file_rtrn);
            حرر(*extra_معلومات);
            أرجع false;
        }
        *tmp++ = '\0';
        *map_rtrn = strdup(str);
    }

    /* Set up the next file for the next call, if any. */
    إذا (*nextop_rtrn == '\0')
        *str_inout = عدم;
    وإلا إذا (is_ملايو_گهڻائي_اڳياڙي(*nextop_rtrn))
        *str_inout = ايندڙ;
    وإلا
        أرجع false;

    أرجع true;
}

ساكن ثابت حرف *xkb_file_type_include_dirs[FILE_TYPE_انگ_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "keycodes",
    [FILE_TYPE_TYPES] = "types",
    [FILE_TYPE_COMPAT] = "compat",
    [FILE_TYPE_SYMBOLS] = "symbols",
    [FILE_TYPE_GEOMETRY] = "geometry",
    [FILE_TYPE_KEYMAP] = "keymap",
    [FILE_TYPE_قاعدا] = "rules",
};

/**
 * Return the xkb directory based on the type.
 */
ساكن ثابت حرف *
DirectoryForInclude(تعداد xkb_file_type type)
{
    إذا (type >= FILE_TYPE_انگ_ENTRIES)
        أرجع "";
    أرجع xkb_file_type_include_dirs[type];
}

ساكن فراغ
LogIncludePaths(بنية xkb_context *ctx)
{
    إذا (xkb_context_انگ_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_مليو,
                "%u include paths searched:\n",
                xkb_context_انگ_include_paths(ctx));
        لكل (بنانشان صحيح i = 0; i < xkb_context_انگ_include_paths(ctx); i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_مليو,
                    "\t%s\n",
                    xkb_context_include_رستو_get(ctx, i));
    }
    وإلا {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_مليو,
                "There are no include paths to search\n");
    }

    إذا (xkb_context_انگ_failed_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_مليو,
                "%u include paths could not be added:\n",
                xkb_context_انگ_failed_include_paths(ctx));
        لكل (darray_سائيز_t i = 0;
             i < xkb_context_انگ_failed_include_paths(ctx);
             i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_مليو,
                    "\t%s\n",
                    xkb_context_failed_include_رستو_get(ctx, i));
    }
}

ساكن سائيز_t
expand_percent(بنية xkb_context *ctx, ثابت حرف* والد_file_name,
               ثابت حرف* typeDir, حرف *buf, سائيز_t buf_سائيز,
               ثابت حرف *name, سائيز_t name_len)
{
    بنية scanner s;
    scanner_init(&s, ctx, name, name_len, والد_file_name, عدم);
    s.buf_pos = 0;

    طالما (!scanner_eof(&s) && !scanner_eol(&s)) {
        إذا (scanner_chr(&s, '%')) {
            إذا (scanner_chr(&s, '%')) {
                scanner_buf_append(&s, '%');
            }
            وإلا إذا (scanner_chr(&s, 'H')) {
                ثابت حرف *home = xkb_context_getenv(ctx, "HOME");
                إذا (!home) {
                    scanner_err(&s, XKB_LOG_پيغام_NO_ID,
                                "%%H was used in an include statement, but the "
                                "HOME environment variable is not set");
                    أرجع 0;
                }
                إذا (!scanner_buf_appends(&s, home)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_بفر_سائيز,
                                "include path after expanding %%H is too long");
                    أرجع 0;
                }
            }
            وإلا إذا (scanner_chr(&s, 'S')) {
                ثابت حرف *default_پاڙ =
                    xkb_context_include_رستو_get_system_رستو(ctx);
                إذا (!scanner_buf_appends(&s, default_پاڙ) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_بفر_سائيز,
                                "include path after expanding %%S is too long");
                    أرجع 0;
                }
            }
            وإلا إذا (scanner_chr(&s, 'E')) {
                ثابت حرف *default_پاڙ =
                    xkb_context_include_رستو_get_extra_رستو(ctx);
                إذا (!scanner_buf_appends(&s, default_پاڙ) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_بفر_سائيز,
                                "include path after expanding %%E is too long");
                    أرجع 0;
                }
            }
            وإلا {
                scanner_err(&s, XKB_ERROR_INSUFFICIENT_بفر_سائيز,
                            "unknown %% format (%c) in include statement",
                            scanner_peek(&s));
                أرجع 0;
            }
        }
        وإلا {
            scanner_buf_append(&s, scanner_ايندڙ(&s));
        }
    }
    إذا (!scanner_buf_append(&s, '\0')) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_بفر_سائيز,
                    "include path is too long; max: %zu", حجم(s.buf));
        أرجع 0;
    }
    إذا (unlikely(s.buf_pos > buf_سائيز)) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_بفر_سائيز,
                    "include path is too long: %zu > %zu", s.buf_pos, buf_سائيز);
        أرجع 0;
    }
    يادگيري_نقل(buf, s.buf, s.buf_pos);
    أرجع s.buf_pos;
}

ssize_t
expand_رستو(بنية xkb_context *ctx, ثابت حرف* والد_file_name,
            ثابت حرف *name, سائيز_t name_len, تعداد xkb_file_type type,
            حرف *buf, سائيز_t buf_سائيز)
{
    /* Detect %-expansion */
    سائيز_t k;
    لكل (k = 0; k < name_len; k++) {
        إذا (name[k] == '%') {
            اذهب expand;
        }
    }

    أرجع 0;

expand:
    /* Process %-expansion */

    /* Copy prefix (usual case: k == 0) */
    إذا (unlikely(k >= buf_سائيز)) {
        log_err(ctx, XKB_ERROR_INVALID_رستو,
                "Path is too long: %zu > %zu, "
                "got raw path: %.*s\n",
                k, buf_سائيز, (بنانشان صحيح) name_len, name);
        أرجع -1;
    }
    يادگيري_نقل(buf, name, k);

    /* Proceed to %-expansion */
    ثابت حرف *typeDir = DirectoryForInclude(type);
    سائيز_t ڳڻپ = expand_percent(ctx, والد_file_name, typeDir,
                                  buf + k, buf_سائيز - k,
                                  name + k, name_len - k);
    إذا (!ڳڻپ)
        أرجع -1;
    ڳڻپ += k;
    أكد(buf[ڳڻپ - 1] == '\0');
    أرجع (ssize_t) ڳڻپ - 1;
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
ڳوليو_File_اندريون_Xkb_رستو(بنية xkb_context *ctx, ثابت حرف* والد_file_name,
                  ثابت حرف *name, سائيز_t name_len, تعداد xkb_file_type type,
                  حرف *buf, سائيز_t buf_سائيز, بنانشان صحيح *وٿي,
                  bool required)
{
    /* We do not handle absolute paths here */
    أكد(!is_absolute_رستو(name));

    FILE *file = عدم;
    حرف *name_بفر = عدم;
    ثابت حرف *typeDir = DirectoryForInclude(type);

    لكل (بنانشان صحيح i = *وٿي; i < xkb_context_انگ_include_paths(ctx); i++) {
        إذا (!snprintf_محفوظ(buf, buf_سائيز, "%s/%s/%.*s",
                           xkb_context_include_رستو_get(ctx, i),
                           typeDir, (بنانشان صحيح) name_len, name)) {
            log_err(ctx, XKB_ERROR_INVALID_رستو,
                    "Path is too long: expected max length of %zu, "
                    "got: %s/%s/%.*s\n",
                    buf_سائيز, xkb_context_include_رستو_get(ctx, i),
                    typeDir, (بنانشان صحيح) name_len, name);
            استمر;
        }

        file = fopen(buf, "rb");
        إذا (file) {
            *وٿي = i;
            اذهب ٻاهريون;
        }
    }

    /* We only print warnings if we can’t find the file on the first lookup */
    إذا (required && *وٿي == 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_مليو,
                "Couldn't find file \"%s/%.*s\" in include paths\n",
                typeDir, (بنانشان صحيح) name_len, name);
        LogIncludePaths(ctx);
    }

ٻاهريون:
    حرر(name_بفر);
    أرجع file;
}

bool
Exceeds_Include_Max_اونهائي(بنية xkb_context *ctx, بنانشان صحيح include_اونهائي)
{
    إذا (include_اونهائي >= INCLUDE_MAX_اونهائي) {
        log_err(ctx, XKB_ERROR_RECURSIVE_INCLUDE,
                "Exceeded include depth threshold (%u)",
                INCLUDE_MAX_اونهائي);
        أرجع true;
    } وإلا {
        أرجع false;
    }
}

XkbFile *
ProcessIncludeFile(بنية xkb_context *ctx,
                   ثابت بنية parser_keymap_config *config,
                   ثابت IncludeStmt *stmt,
                   تعداد xkb_file_type file_type, حرف *رستو, سائيز_t رستو_سائيز)
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
    سائيز_t stmt_file_len = لڙي_ڊيگهه(stmt_file);

    /* Process %-expansion, if any */
    // FIXME: use parent file name instead of “(unknow)”.
    ثابت ssize_t expanded = expand_رستو(ctx, "(unknown)",
                                         stmt_file, stmt_file_len, file_type,
                                         رستو, رستو_سائيز);
    إذا (expanded < 0) {
        /* Error */
        أرجع عدم;
    } وإلا إذا (expanded > 0) {
        /* %-expanded */
        stmt_file = رستو;
        stmt_file_len = (سائيز_t) expanded;
        أكد(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    بنانشان صحيح وٿي = 0;
    ثابت bool absolute_رستو = is_absolute_رستو(stmt_file);
    إذا (absolute_رستو) {
        /* Absolute path: no need for lookup in XKB paths */
        أكد(stmt_file[stmt_file_len] == '\0');
        file = fopen(stmt_file, "rb");
    } وإلا {
        /* Relative path: lookup the first XKB path */
        إذا (unlikely(expanded)) {
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
            file = ڳوليو_File_اندريون_Xkb_رستو(ctx, "(unknown)",
                                     stmt_file, stmt_file_len, file_type,
                                     رستو, رستو_سائيز, &وٿي, true);
        }
    }

    طالما (file) {
        xkb_file = XkbParseFile(ctx, config, file, stmt->file, stmt->map);
        fclose(file);

        إذا (xkb_file) {
            إذا (xkb_file->file_type != file_type) {
                log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                        "Include file of wrong type (expected %s, got %s); "
                        "Include file \"%s\" ignored\n",
                        xkb_file_type_منزل_string(file_type),
                        xkb_file_type_منزل_string(xkb_file->file_type), stmt->file);
                FreeXkbFile(xkb_file);
                xkb_file = عدم;
            } وإلا إذا (stmt->map || (xkb_file->flags & MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops here.
                 */
                اكسر;
            } وإلا إذا (!candidate) {
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

        إذا (absolute_رستو) {
            /* There is no point to search further if the path is absolute */
            اكسر;
        }

        وٿي++;
        file = ڳوليو_File_اندريون_Xkb_رستو(ctx, "(unknown)",
                                 stmt_file, stmt_file_len, file_type,
                                 رستو, رستو_سائيز, &وٿي, true);
    }

    إذا (!xkb_file) {
        /* No exact match: use weak match, if any */
        xkb_file = candidate;
    } وإلا {
        /* Found exact match: discard weak match, if any */
        FreeXkbFile(candidate);
    }

    إذا (!xkb_file) {
        إذا (stmt->map)
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
