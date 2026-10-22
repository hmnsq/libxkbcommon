/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <errno.h>
#अन्तर्भाव <limits.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "xkbcomp-priv.h"
#अन्तर्भाव "include.h"
#अन्तर्भाव "scanner-utils.h"
#अन्तर्भाव "util-paths.h"

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
ParseIncludeMap(वर्ण **str_inout, वर्ण **file_rtrn, वर्ण **map_rtrn,
                वर्ण *nextop_rtrn, वर्ण **extra_data)
{
    वर्ण *tmp, *str, *next;

    str = *str_inout;

    /*
     * Find the position in the string where the next file is included,
     * if there is more than one left in the statement.
     */
    next = strpbrk(str, MERGE_MODE_PREFIXES);
    यदि (next) {
        /* Got more files, this function will be called again. */
        *nextop_rtrn = *next;
        /* Separate the string, for strchr etc. to work on this file only. */
        *next++ = '\0';
    }
    अन्यथा {
        /* This is the last file in this statement, won't be called again. */
        *nextop_rtrn = '\0';
        next = रिक्त;
    }

    /*
     * Search for the explicit group designator, if any. If it's there,
     * it goes after the file name and map.
     */
    tmp = म_अक्षर(str, ':');
    यदि (tmp != रिक्त) {
        *tmp++ = '\0';
        *extra_data = strdup(tmp);
    }
    अन्यथा {
        *extra_data = रिक्त;
    }

    /* Look for a map, if any. */
    tmp = म_अक्षर(str, '(');
    यदि (tmp == रिक्त) {
        /* No map. */
        *file_rtrn = strdup(str);
        *map_rtrn = रिक्त;
    }
    अन्यथा यदि (str[0] == '(') {
        /* Map without file - invalid. */
        मुक्त(*extra_data);
        प्रत्यागम false;
    }
    अन्यथा {
        /* Got a map; separate the file and the map for the strdup's. */
        *tmp++ = '\0';
        *file_rtrn = strdup(str);
        str = tmp;
        tmp = म_अक्षर(str, ')');
        यदि (tmp == रिक्त || tmp[1] != '\0') {
            मुक्त(*file_rtrn);
            मुक्त(*extra_data);
            प्रत्यागम false;
        }
        *tmp++ = '\0';
        *map_rtrn = strdup(str);
    }

    /* Set up the next file for the next call, if any. */
    यदि (*nextop_rtrn == '\0')
        *str_inout = रिक्त;
    अन्यथा यदि (is_merge_mode_prefix(*nextop_rtrn))
        *str_inout = next;
    अन्यथा
        प्रत्यागम false;

    प्रत्यागम true;
}

स्थिर ध्रुव वर्ण *xkb_file_type_include_dirs[FILE_TYPE_अंक_ENTRIES] = {
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
स्थिर ध्रुव वर्ण *
DirectoryForInclude(गणना xkb_file_type type)
{
    यदि (type >= FILE_TYPE_अंक_ENTRIES)
        प्रत्यागम "";
    प्रत्यागम xkb_file_type_include_dirs[type];
}

स्थिर शून्य
LogIncludePaths(संरचना xkb_context *ctx)
{
    यदि (xkb_context_अंक_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "%u include paths searched:\n",
                xkb_context_अंक_include_paths(ctx));
        प्रति (अचिन्हित पूर्णाङ्क i = 0; i < xkb_context_अंक_include_paths(ctx); i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "\t%s\n",
                    xkb_context_include_path_get(ctx, i));
    }
    अन्यथा {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "There are no include paths to search\n");
    }

    यदि (xkb_context_अंक_failed_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "%u include paths could not be added:\n",
                xkb_context_अंक_failed_include_paths(ctx));
        प्रति (darray_size_t i = 0;
             i < xkb_context_अंक_failed_include_paths(ctx);
             i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "\t%s\n",
                    xkb_context_failed_include_path_get(ctx, i));
    }
}

स्थिर माप_प्रकार
expand_percent(संरचना xkb_context *ctx, ध्रुव वर्ण* parent_file_name,
               ध्रुव वर्ण* typeDir, वर्ण *buf, माप_प्रकार buf_size,
               ध्रुव वर्ण *name, माप_प्रकार name_len)
{
    संरचना scanner s;
    scanner_init(&s, ctx, name, name_len, parent_file_name, रिक्त);
    s.buf_pos = 0;

    यावत् (!scanner_eof(&s) && !scanner_eol(&s)) {
        यदि (scanner_chr(&s, '%')) {
            यदि (scanner_chr(&s, '%')) {
                scanner_buf_append(&s, '%');
            }
            अन्यथा यदि (scanner_chr(&s, 'H')) {
                ध्रुव वर्ण *home = xkb_context_getenv(ctx, "HOME");
                यदि (!home) {
                    scanner_err(&s, XKB_LOG_MESSAGE_NO_ID,
                                "%%H was used in an include statement, but the "
                                "HOME environment variable is not set");
                    प्रत्यागम 0;
                }
                यदि (!scanner_buf_appends(&s, home)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%H is too long");
                    प्रत्यागम 0;
                }
            }
            अन्यथा यदि (scanner_chr(&s, 'S')) {
                ध्रुव वर्ण *default_मूल =
                    xkb_context_include_path_get_system_path(ctx);
                यदि (!scanner_buf_appends(&s, default_मूल) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%S is too long");
                    प्रत्यागम 0;
                }
            }
            अन्यथा यदि (scanner_chr(&s, 'E')) {
                ध्रुव वर्ण *default_मूल =
                    xkb_context_include_path_get_extra_path(ctx);
                यदि (!scanner_buf_appends(&s, default_मूल) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                                "include path after expanding %%E is too long");
                    प्रत्यागम 0;
                }
            }
            अन्यथा {
                scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                            "unknown %% format (%c) in include statement",
                            scanner_peek(&s));
                प्रत्यागम 0;
            }
        }
        अन्यथा {
            scanner_buf_append(&s, scanner_next(&s));
        }
    }
    यदि (!scanner_buf_append(&s, '\0')) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                    "include path is too long; max: %zu", परिमाण(s.buf));
        प्रत्यागम 0;
    }
    यदि (unlikely(s.buf_pos > buf_size)) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_BUFFER_SIZE,
                    "include path is too long: %zu > %zu", s.buf_pos, buf_size);
        प्रत्यागम 0;
    }
    स_नकल(buf, s.buf, s.buf_pos);
    प्रत्यागम s.buf_pos;
}

ssize_t
expand_path(संरचना xkb_context *ctx, ध्रुव वर्ण* parent_file_name,
            ध्रुव वर्ण *name, माप_प्रकार name_len, गणना xkb_file_type type,
            वर्ण *buf, माप_प्रकार buf_size)
{
    /* Detect %-expansion */
    माप_प्रकार k;
    प्रति (k = 0; k < name_len; k++) {
        यदि (name[k] == '%') {
            गच्छ expand;
        }
    }

    प्रत्यागम 0;

expand:
    /* Process %-expansion */

    /* Copy prefix (usual case: k == 0) */
    यदि (unlikely(k >= buf_size)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: %zu > %zu, "
                "got raw path: %.*s\n",
                k, buf_size, (अचिन्हित पूर्णाङ्क) name_len, name);
        प्रत्यागम -1;
    }
    स_नकल(buf, name, k);

    /* Proceed to %-expansion */
    ध्रुव वर्ण *typeDir = DirectoryForInclude(type);
    माप_प्रकार गिनती = expand_percent(ctx, parent_file_name, typeDir,
                                  buf + k, buf_size - k,
                                  name + k, name_len - k);
    यदि (!गिनती)
        प्रत्यागम -1;
    गिनती += k;
    प्रतिज्ञा(buf[गिनती - 1] == '\0');
    प्रत्यागम (ssize_t) गिनती - 1;
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
खाता *
FindFileInXkbPath(संरचना xkb_context *ctx, ध्रुव वर्ण* parent_file_name,
                  ध्रुव वर्ण *name, माप_प्रकार name_len, गणना xkb_file_type type,
                  वर्ण *buf, माप_प्रकार buf_size, अचिन्हित पूर्णाङ्क *offset,
                  bool required)
{
    /* We do not handle absolute paths here */
    प्रतिज्ञा(!is_absolute_path(name));

    खाता *file = रिक्त;
    वर्ण *name_buffer = रिक्त;
    ध्रुव वर्ण *typeDir = DirectoryForInclude(type);

    प्रति (अचिन्हित पूर्णाङ्क i = *offset; i < xkb_context_अंक_include_paths(ctx); i++) {
        यदि (!snprintf_safe(buf, buf_size, "%s/%s/%.*s",
                           xkb_context_include_path_get(ctx, i),
                           typeDir, (अचिन्हित पूर्णाङ्क) name_len, name)) {
            log_err(ctx, XKB_ERROR_INVALID_PATH,
                    "Path is too long: expected max length of %zu, "
                    "got: %s/%s/%.*s\n",
                    buf_size, xkb_context_include_path_get(ctx, i),
                    typeDir, (अचिन्हित पूर्णाङ्क) name_len, name);
            अनुवर्त;
        }

        file = ख_खोलो(buf, "rb");
        यदि (file) {
            *offset = i;
            गच्छ out;
        }
    }

    /* We only print warnings if we can’t find the file on the first lookup */
    यदि (required && *offset == 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "Couldn't find file \"%s/%.*s\" in include paths\n",
                typeDir, (अचिन्हित पूर्णाङ्क) name_len, name);
        LogIncludePaths(ctx);
    }

out:
    मुक्त(name_buffer);
    प्रत्यागम file;
}

bool
ExceedsIncludeMaxDepth(संरचना xkb_context *ctx, अचिन्हित पूर्णाङ्क include_depth)
{
    यदि (include_depth >= INCLUDE_MAX_DEPTH) {
        log_err(ctx, XKB_ERROR_RECURSIVE_INCLUDE,
                "Exceeded include depth threshold (%u)",
                INCLUDE_MAX_DEPTH);
        प्रत्यागम true;
    } अन्यथा {
        प्रत्यागम false;
    }
}

XkbFile *
ProcessIncludeFile(संरचना xkb_context *ctx,
                   ध्रुव संरचना parser_keymap_config *config,
                   ध्रुव IncludeStmt *stmt,
                   गणना xkb_file_type file_type, वर्ण *path, माप_प्रकार path_size)
{
    /*
     * Resolve include statement:
     * - Explicit map name: look for an *exact match* only.
     * - Default map: look for an *explicit default* map (i.e. tagged with
     *   `default`: exact match), else fallback to the first *implicit* default
     *   map (weak match).
     */
    XkbFile *xkb_file = रिक्त;  /* Exact match */
    XkbFile *candidate = रिक्त; /* Weak match */

    ध्रुव वर्ण *stmt_file = stmt->file;
    माप_प्रकार stmt_file_len = म_माप(stmt_file);

    /* Process %-expansion, if any */
    // FIXME: use parent file name instead of “(unknow)”.
    ध्रुव ssize_t expanded = expand_path(ctx, "(unknown)",
                                         stmt_file, stmt_file_len, file_type,
                                         path, path_size);
    यदि (expanded < 0) {
        /* Error */
        प्रत्यागम रिक्त;
    } अन्यथा यदि (expanded > 0) {
        /* %-expanded */
        stmt_file = path;
        stmt_file_len = (माप_प्रकार) expanded;
        प्रतिज्ञा(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    खाता* file;
    अचिन्हित पूर्णाङ्क offset = 0;
    ध्रुव bool absolute_path = is_absolute_path(stmt_file);
    यदि (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        प्रतिज्ञा(stmt_file[stmt_file_len] == '\0');
        file = ख_खोलो(stmt_file, "rb");
    } अन्यथा {
        /* Relative path: lookup the first XKB path */
        यदि (unlikely(expanded)) {
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
            file = रिक्त;
        } अन्यथा {
            // FIXME: use parent file name instead of “(unknow)”.
            file = FindFileInXkbPath(ctx, "(unknown)",
                                     stmt_file, stmt_file_len, file_type,
                                     path, path_size, &offset, true);
        }
    }

    यावत् (file) {
        xkb_file = XkbParseFile(ctx, config, file, stmt->file, stmt->map);
        ख_बंद(file);

        यदि (xkb_file) {
            यदि (xkb_file->file_type != file_type) {
                log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                        "Include file of wrong type (expected %s, got %s); "
                        "Include file \"%s\" ignored\n",
                        xkb_file_type_to_string(file_type),
                        xkb_file_type_to_string(xkb_file->file_type), stmt->file);
                FreeXkbFile(xkb_file);
                xkb_file = रिक्त;
            } अन्यथा यदि (stmt->map || (xkb_file->flags & MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops here.
                 */
                भङ्ग;
            } अन्यथा यदि (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = xkb_file;
                xkb_file = रिक्त;
            } अन्यथा {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                FreeXkbFile(xkb_file);
                xkb_file = रिक्त;
            }
        }

        यदि (absolute_path) {
            /* There is no point to search further if the path is absolute */
            भङ्ग;
        }

        offset++;
        file = FindFileInXkbPath(ctx, "(unknown)",
                                 stmt_file, stmt_file_len, file_type,
                                 path, path_size, &offset, true);
    }

    यदि (!xkb_file) {
        /* No exact match: use weak match, if any */
        xkb_file = candidate;
    } अन्यथा {
        /* Found exact match: discard weak match, if any */
        FreeXkbFile(candidate);
    }

    यदि (!xkb_file) {
        यदि (stmt->map)
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s(%s)'\n",
                    stmt->file, stmt->map);
        अन्यथा
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s'\n",
                    stmt->file);
    }

    प्रत्यागम xkb_file;
}
