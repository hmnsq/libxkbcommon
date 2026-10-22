/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>

#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"
#ಅನ್ತರ್ಭಾವ "include.h"
#ಅನ್ತರ್ಭಾವ "scanner-utils.h"
#ಅನ್ತರ್ಭಾವ "util-paths.h"

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
ParseIncludeMap(ಅಕ್ಷರ **str_inout, ಅಕ್ಷರ **file_rtrn, ಅಕ್ಷರ **map_rtrn,
                ಅಕ್ಷರ *nextop_rtrn, ಅಕ್ಷರ **extra_ದತ್ತಾಂಶ)
{
    ಅಕ್ಷರ *tmp, *str, *ಮುಂದಿನ;

    str = *str_inout;

    /*
     * Find the position in the string where the next file is included,
     * if there is more than one left in the statement.
     */
    ಮುಂದಿನ = strpbrk(str, ವಿಲೀನ_ಬಹುಲಕ_PREFIXES);
    ಆದರೆ (ಮುಂದಿನ) {
        /* Got more files, this function will be called again. */
        *nextop_rtrn = *ಮುಂದಿನ;
        /* Separate the string, for strchr etc. to work on this file only. */
        *ಮುಂದಿನ++ = '\0';
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        /* This is the last file in this statement, won't be called again. */
        *nextop_rtrn = '\0';
        ಮುಂದಿನ = ಖಾಲಿ;
    }

    /*
     * Search for the explicit group designator, if any. If it's there,
     * it goes after the file name and map.
     */
    tmp = strchr(str, ':');
    ಆದರೆ (tmp != ಖಾಲಿ) {
        *tmp++ = '\0';
        *extra_ದತ್ತಾಂಶ = strdup(tmp);
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        *extra_ದತ್ತಾಂಶ = ಖಾಲಿ;
    }

    /* Look for a map, if any. */
    tmp = strchr(str, '(');
    ಆದರೆ (tmp == ಖಾಲಿ) {
        /* No map. */
        *file_rtrn = strdup(str);
        *map_rtrn = ಖಾಲಿ;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (str[0] == '(') {
        /* Map without file - invalid. */
        ಬಿಡುಗಡೆ(*extra_ದತ್ತಾಂಶ);
        ಹಿಂತಿರುಗಿ false;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        /* Got a map; separate the file and the map for the strdup's. */
        *tmp++ = '\0';
        *file_rtrn = strdup(str);
        str = tmp;
        tmp = strchr(str, ')');
        ಆದರೆ (tmp == ಖಾಲಿ || tmp[1] != '\0') {
            ಬಿಡುಗಡೆ(*file_rtrn);
            ಬಿಡುಗಡೆ(*extra_ದತ್ತಾಂಶ);
            ಹಿಂತಿರುಗಿ false;
        }
        *tmp++ = '\0';
        *map_rtrn = strdup(str);
    }

    /* Set up the next file for the next call, if any. */
    ಆದರೆ (*nextop_rtrn == '\0')
        *str_inout = ಖಾಲಿ;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_ವಿಲೀನ_ಬಹುಲಕ_ಪೂರ್ವಪ್ರತ್ಯಯ(*nextop_rtrn))
        *str_inout = ಮುಂದಿನ;
    ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ false;

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *xkb_file_type_include_dirs[FILE_TYPE_ಅಂಕೆ_ENTRIES] = {
    [FILE_TYPE_KEYCODES] = "keycodes",
    [FILE_TYPE_TYPES] = "types",
    [FILE_TYPE_COMPAT] = "compat",
    [FILE_TYPE_SYMBOLS] = "symbols",
    [FILE_TYPE_GEOMETRY] = "geometry",
    [FILE_TYPE_KEYMAP] = "keymap",
    [FILE_TYPE_ನಿಯಮಗಳು] = "rules",
};

/**
 * Return the xkb directory based on the type.
 */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
DirectoryForInclude(ಎಣಿಕೆ xkb_file_type type)
{
    ಆದರೆ (type >= FILE_TYPE_ಅಂಕೆ_ENTRIES)
        ಹಿಂತಿರುಗಿ "";
    ಹಿಂತಿರುಗಿ xkb_file_type_include_dirs[type];
}

ಸ್ಥಿರ ಶೂನ್ಯ
LogIncludePaths(ರಚನೆ xkb_context *ctx)
{
    ಆದರೆ (xkb_context_ಅಂಕೆ_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_ಸಿಕ್ಕಿತು,
                "%u include paths searched:\n",
                xkb_context_ಅಂಕೆ_include_paths(ctx));
        ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i = 0; i < xkb_context_ಅಂಕೆ_include_paths(ctx); i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_ಸಿಕ್ಕಿತು,
                    "\t%s\n",
                    xkb_context_include_ಮಾರ್ಗ_get(ctx, i));
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_ಸಿಕ್ಕಿತು,
                "There are no include paths to search\n");
    }

    ಆದರೆ (xkb_context_ಅಂಕೆ_failed_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_ಸಿಕ್ಕಿತು,
                "%u include paths could not be added:\n",
                xkb_context_ಅಂಕೆ_failed_include_paths(ctx));
        ಗಾಗಿ (darray_ಗಾತ್ರ_t i = 0;
             i < xkb_context_ಅಂಕೆ_failed_include_paths(ctx);
             i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_ಸಿಕ್ಕಿತು,
                    "\t%s\n",
                    xkb_context_failed_include_ಮಾರ್ಗ_get(ctx, i));
    }
}

ಸ್ಥಿರ ಗಾತ್ರ_t
expand_percent(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಪೋಷಕ_file_name,
               ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* typeDir, ಅಕ್ಷರ *buf, ಗಾತ್ರ_t buf_ಗಾತ್ರ,
               ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಗಾತ್ರ_t name_len)
{
    ರಚನೆ scanner s;
    scanner_init(&s, ctx, name, name_len, ಪೋಷಕ_file_name, ಖಾಲಿ);
    s.buf_pos = 0;

    ಆಗ (!scanner_eof(&s) && !scanner_eol(&s)) {
        ಆದರೆ (scanner_chr(&s, '%')) {
            ಆದರೆ (scanner_chr(&s, '%')) {
                scanner_buf_append(&s, '%');
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (scanner_chr(&s, 'H')) {
                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *home = xkb_context_getenv(ctx, "HOME");
                ಆದರೆ (!home) {
                    scanner_err(&s, XKB_LOG_ಸಂದೇಶ_NO_ID,
                                "%%H was used in an include statement, but the "
                                "HOME environment variable is not set");
                    ಹಿಂತಿರುಗಿ 0;
                }
                ಆದರೆ (!scanner_buf_appends(&s, home)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_ಬಫರ್_ಗಾತ್ರ,
                                "include path after expanding %%H is too long");
                    ಹಿಂತಿರುಗಿ 0;
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (scanner_chr(&s, 'S')) {
                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *default_ಬೇರು =
                    xkb_context_include_ಮಾರ್ಗ_get_system_ಮಾರ್ಗ(ctx);
                ಆದರೆ (!scanner_buf_appends(&s, default_ಬೇರು) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_ಬಫರ್_ಗಾತ್ರ,
                                "include path after expanding %%S is too long");
                    ಹಿಂತಿರುಗಿ 0;
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (scanner_chr(&s, 'E')) {
                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *default_ಬೇರು =
                    xkb_context_include_ಮಾರ್ಗ_get_extra_ಮಾರ್ಗ(ctx);
                ಆದರೆ (!scanner_buf_appends(&s, default_ಬೇರು) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_ಬಫರ್_ಗಾತ್ರ,
                                "include path after expanding %%E is too long");
                    ಹಿಂತಿರುಗಿ 0;
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ {
                scanner_err(&s, XKB_ERROR_INSUFFICIENT_ಬಫರ್_ಗಾತ್ರ,
                            "unknown %% format (%c) in include statement",
                            scanner_peek(&s));
                ಹಿಂತಿರುಗಿ 0;
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            scanner_buf_append(&s, scanner_ಮುಂದಿನ(&s));
        }
    }
    ಆದರೆ (!scanner_buf_append(&s, '\0')) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_ಬಫರ್_ಗಾತ್ರ,
                    "include path is too long; max: %zu", ಗಾತ್ರ(s.buf));
        ಹಿಂತಿರುಗಿ 0;
    }
    ಆದರೆ (unlikely(s.buf_pos > buf_ಗಾತ್ರ)) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_ಬಫರ್_ಗಾತ್ರ,
                    "include path is too long: %zu > %zu", s.buf_pos, buf_ಗಾತ್ರ);
        ಹಿಂತಿರುಗಿ 0;
    }
    ಸ್ಮರಣೆ_ನಕಲು(buf, s.buf, s.buf_pos);
    ಹಿಂತಿರುಗಿ s.buf_pos;
}

ssize_t
expand_ಮಾರ್ಗ(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಪೋಷಕ_file_name,
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಗಾತ್ರ_t name_len, ಎಣಿಕೆ xkb_file_type type,
            ಅಕ್ಷರ *buf, ಗಾತ್ರ_t buf_ಗಾತ್ರ)
{
    /* Detect %-expansion */
    ಗಾತ್ರ_t k;
    ಗಾಗಿ (k = 0; k < name_len; k++) {
        ಆದರೆ (name[k] == '%') {
            ಹೋಗು expand;
        }
    }

    ಹಿಂತಿರುಗಿ 0;

expand:
    /* Process %-expansion */

    /* Copy prefix (usual case: k == 0) */
    ಆದರೆ (unlikely(k >= buf_ಗಾತ್ರ)) {
        log_err(ctx, XKB_ERROR_INVALID_ಮಾರ್ಗ,
                "Path is too long: %zu > %zu, "
                "got raw path: %.*s\n",
                k, buf_ಗಾತ್ರ, (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) name_len, name);
        ಹಿಂತಿರುಗಿ -1;
    }
    ಸ್ಮರಣೆ_ನಕಲು(buf, name, k);

    /* Proceed to %-expansion */
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *typeDir = DirectoryForInclude(type);
    ಗಾತ್ರ_t ಎಣಿಕೆ_2 = expand_percent(ctx, ಪೋಷಕ_file_name, typeDir,
                                  buf + k, buf_ಗಾತ್ರ - k,
                                  name + k, name_len - k);
    ಆದರೆ (!ಎಣಿಕೆ_2)
        ಹಿಂತಿರುಗಿ -1;
    ಎಣಿಕೆ_2 += k;
    ಪ್ರತಿಜ್ಞಾ(buf[ಎಣಿಕೆ_2 - 1] == '\0');
    ಹಿಂತಿರುಗಿ (ssize_t) ಎಣಿಕೆ_2 - 1;
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
ಕಂಡುಹಿಡಿ_File_ಒಳ_Xkb_ಮಾರ್ಗ(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಪೋಷಕ_file_name,
                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಗಾತ್ರ_t name_len, ಎಣಿಕೆ xkb_file_type type,
                  ಅಕ್ಷರ *buf, ಗಾತ್ರ_t buf_ಗಾತ್ರ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *ಸ್ಥಾನಾಂತರ,
                  bool required)
{
    /* We do not handle absolute paths here */
    ಪ್ರತಿಜ್ಞಾ(!is_absolute_ಮಾರ್ಗ(name));

    FILE *file = ಖಾಲಿ;
    ಅಕ್ಷರ *name_ಬಫರ್ = ಖಾಲಿ;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *typeDir = DirectoryForInclude(type);

    ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i = *ಸ್ಥಾನಾಂತರ; i < xkb_context_ಅಂಕೆ_include_paths(ctx); i++) {
        ಆದರೆ (!snprintf_ಸುರಕ್ಷಿತ(buf, buf_ಗಾತ್ರ, "%s/%s/%.*s",
                           xkb_context_include_ಮಾರ್ಗ_get(ctx, i),
                           typeDir, (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) name_len, name)) {
            log_err(ctx, XKB_ERROR_INVALID_ಮಾರ್ಗ,
                    "Path is too long: expected max length of %zu, "
                    "got: %s/%s/%.*s\n",
                    buf_ಗಾತ್ರ, xkb_context_include_ಮಾರ್ಗ_get(ctx, i),
                    typeDir, (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) name_len, name);
            ಮುಂದುವರಿ;
        }

        file = fopen(buf, "rb");
        ಆದರೆ (file) {
            *ಸ್ಥಾನಾಂತರ = i;
            ಹೋಗು ಹೊರಹೋಗು;
        }
    }

    /* We only print warnings if we can’t find the file on the first lookup */
    ಆದರೆ (required && *ಸ್ಥಾನಾಂತರ == 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_ಸಿಕ್ಕಿತು,
                "Couldn't find file \"%s/%.*s\" in include paths\n",
                typeDir, (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) name_len, name);
        LogIncludePaths(ctx);
    }

ಹೊರಹೋಗು:
    ಬಿಡುಗಡೆ(name_ಬಫರ್);
    ಹಿಂತಿರುಗಿ file;
}

bool
Exceeds_Include_Max_ಆಳ(ರಚನೆ xkb_context *ctx, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ include_ಆಳ)
{
    ಆದರೆ (include_ಆಳ >= INCLUDE_MAX_ಆಳ) {
        log_err(ctx, XKB_ERROR_RECURSIVE_INCLUDE,
                "Exceeded include depth threshold (%u)",
                INCLUDE_MAX_ಆಳ);
        ಹಿಂತಿರುಗಿ true;
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಹಿಂತಿರುಗಿ false;
    }
}

XkbFile *
ProcessIncludeFile(ರಚನೆ xkb_context *ctx,
                   ಸ್ಥಿರಾಂಕ ರಚನೆ parser_keymap_config *config,
                   ಸ್ಥಿರಾಂಕ IncludeStmt *stmt,
                   ಎಣಿಕೆ xkb_file_type file_type, ಅಕ್ಷರ *ಮಾರ್ಗ, ಗಾತ್ರ_t ಮಾರ್ಗ_ಗಾತ್ರ)
{
    /*
     * Resolve include statement:
     * - Explicit map name: look for an *exact match* only.
     * - Default map: look for an *explicit default* map (i.e. tagged with
     *   `default`: exact match), else fallback to the first *implicit* default
     *   map (weak match).
     */
    XkbFile *xkb_file = ಖಾಲಿ;  /* Exact match */
    XkbFile *candidate = ಖಾಲಿ; /* Weak match */

    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *stmt_file = stmt->file;
    ಗಾತ್ರ_t stmt_file_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(stmt_file);

    /* Process %-expansion, if any */
    // FIXME: use parent file name instead of “(unknow)”.
    ಸ್ಥಿರಾಂಕ ssize_t expanded = expand_ಮಾರ್ಗ(ctx, "(unknown)",
                                         stmt_file, stmt_file_len, file_type,
                                         ಮಾರ್ಗ, ಮಾರ್ಗ_ಗಾತ್ರ);
    ಆದರೆ (expanded < 0) {
        /* Error */
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (expanded > 0) {
        /* %-expanded */
        stmt_file = ಮಾರ್ಗ;
        stmt_file_len = (ಗಾತ್ರ_t) expanded;
        ಪ್ರತಿಜ್ಞಾ(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ = 0;
    ಸ್ಥಿರಾಂಕ bool absolute_ಮಾರ್ಗ = is_absolute_ಮಾರ್ಗ(stmt_file);
    ಆದರೆ (absolute_ಮಾರ್ಗ) {
        /* Absolute path: no need for lookup in XKB paths */
        ಪ್ರತಿಜ್ಞಾ(stmt_file[stmt_file_len] == '\0');
        file = fopen(stmt_file, "rb");
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* Relative path: lookup the first XKB path */
        ಆದರೆ (unlikely(expanded)) {
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
            file = ಖಾಲಿ;
        } ಇಲ್ಲದಿದ್ದರೆ {
            // FIXME: use parent file name instead of “(unknow)”.
            file = ಕಂಡುಹಿಡಿ_File_ಒಳ_Xkb_ಮಾರ್ಗ(ctx, "(unknown)",
                                     stmt_file, stmt_file_len, file_type,
                                     ಮಾರ್ಗ, ಮಾರ್ಗ_ಗಾತ್ರ, &ಸ್ಥಾನಾಂತರ, true);
        }
    }

    ಆಗ (file) {
        xkb_file = XkbParseFile(ctx, config, file, stmt->file, stmt->map);
        fclose(file);

        ಆದರೆ (xkb_file) {
            ಆದರೆ (xkb_file->file_type != file_type) {
                log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                        "Include file of wrong type (expected %s, got %s); "
                        "Include file \"%s\" ignored\n",
                        xkb_file_type_ಗಮ್ಯ_string(file_type),
                        xkb_file_type_ಗಮ್ಯ_string(xkb_file->file_type), stmt->file);
                FreeXkbFile(xkb_file);
                xkb_file = ಖಾಲಿ;
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (stmt->map || (xkb_file->flags & MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops here.
                 */
                ನಿಲ್ಲಿಸು;
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = xkb_file;
                xkb_file = ಖಾಲಿ;
            } ಇಲ್ಲದಿದ್ದರೆ {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                FreeXkbFile(xkb_file);
                xkb_file = ಖಾಲಿ;
            }
        }

        ಆದರೆ (absolute_ಮಾರ್ಗ) {
            /* There is no point to search further if the path is absolute */
            ನಿಲ್ಲಿಸು;
        }

        ಸ್ಥಾನಾಂತರ++;
        file = ಕಂಡುಹಿಡಿ_File_ಒಳ_Xkb_ಮಾರ್ಗ(ctx, "(unknown)",
                                 stmt_file, stmt_file_len, file_type,
                                 ಮಾರ್ಗ, ಮಾರ್ಗ_ಗಾತ್ರ, &ಸ್ಥಾನಾಂತರ, true);
    }

    ಆದರೆ (!xkb_file) {
        /* No exact match: use weak match, if any */
        xkb_file = candidate;
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* Found exact match: discard weak match, if any */
        FreeXkbFile(candidate);
    }

    ಆದರೆ (!xkb_file) {
        ಆದರೆ (stmt->map)
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s(%s)'\n",
                    stmt->file, stmt->map);
        ಇಲ್ಲದಿದ್ದರೆ
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s'\n",
                    stmt->file);
    }

    ಹಿಂತಿರುಗಿ xkb_file;
}
