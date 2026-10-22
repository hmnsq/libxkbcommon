/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#समावेश "config.h"

#समावेश <errno.h>
#समावेश <limits.h>
#समावेश <stdio.h>
#समावेश <string.h>

#समावेश "messages-codes.h"
#समावेश "utils.h"
#समावेश "xkbcomp-priv.h"
#समावेश "include.h"
#समावेश "scanner-utils.h"
#समावेश "util-paths.h"

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
ParseIncludeMap(अक्षर **str_inout, अक्षर **file_rtrn, अक्षर **map_rtrn,
                अक्षर *nextop_rtrn, अक्षर **extra_तथ्य)
{
    अक्षर *tmp, *str, *अग्र;

    str = *str_inout;

    /*
     * Find the position in the string where the next file is included,
     * if there is more than one left in the statement.
     */
    अग्र = strpbrk(str, MERGE_MODE_PREFIXES);
    अगर (अग्र) {
        /* Got more files, this function will be called again. */
        *nextop_rtrn = *अग्र;
        /* Separate the string, for strchr etc. to work on this file only. */
        *अग्र++ = '\0';
    }
    अन्यथा {
        /* This is the last file in this statement, won't be called again. */
        *nextop_rtrn = '\0';
        अग्र = शून्य;
    }

    /*
     * Search for the explicit group designator, if any. If it's there,
     * it goes after the file name and map.
     */
    tmp = म_अक्षर(str, ':');
    अगर (tmp != शून्य) {
        *tmp++ = '\0';
        *extra_तथ्य = strdup(tmp);
    }
    अन्यथा {
        *extra_तथ्य = शून्य;
    }

    /* Look for a map, if any. */
    tmp = म_अक्षर(str, '(');
    अगर (tmp == शून्य) {
        /* No map. */
        *file_rtrn = strdup(str);
        *map_rtrn = शून्य;
    }
    अन्यथा अगर (str[0] == '(') {
        /* Map without file - invalid. */
        मुक्त(*extra_तथ्य);
        वापस false;
    }
    अन्यथा {
        /* Got a map; separate the file and the map for the strdup's. */
        *tmp++ = '\0';
        *file_rtrn = strdup(str);
        str = tmp;
        tmp = म_अक्षर(str, ')');
        अगर (tmp == शून्य || tmp[1] != '\0') {
            मुक्त(*file_rtrn);
            मुक्त(*extra_तथ्य);
            वापस false;
        }
        *tmp++ = '\0';
        *map_rtrn = strdup(str);
    }

    /* Set up the next file for the next call, if any. */
    अगर (*nextop_rtrn == '\0')
        *str_inout = शून्य;
    अन्यथा अगर (is_merge_mode_prefix(*nextop_rtrn))
        *str_inout = अग्र;
    अन्यथा
        वापस false;

    वापस true;
}

अटल स्थिर अक्षर *xkb_file_type_include_dirs[FILE_TYPE_अंक_ENTRIES] = {
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
अटल स्थिर अक्षर *
DirectoryForInclude(क्रमागत xkb_file_type type)
{
    अगर (type >= FILE_TYPE_अंक_ENTRIES)
        वापस "";
    वापस xkb_file_type_include_dirs[type];
}

अटल व्योम
LogIncludePaths(काष्ठा xkb_context *ctx)
{
    अगर (xkb_context_अंक_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "%u include paths searched:\n",
                xkb_context_अंक_include_paths(ctx));
        क्रम (अचिन्हित पूर्णांक i = 0; i < xkb_context_अंक_include_paths(ctx); i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "\t%s\n",
                    xkb_context_include_path_get(ctx, i));
    }
    अन्यथा {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "There are no include paths to search\n");
    }

    अगर (xkb_context_अंक_failed_include_paths(ctx) > 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "%u include paths could not be added:\n",
                xkb_context_अंक_failed_include_paths(ctx));
        क्रम (darray_आकार_t i = 0;
             i < xkb_context_अंक_failed_include_paths(ctx);
             i++)
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                    "\t%s\n",
                    xkb_context_failed_include_path_get(ctx, i));
    }
}

अटल माप_प्रकार
expand_percent(काष्ठा xkb_context *ctx, स्थिर अक्षर* parent_file_name,
               स्थिर अक्षर* typeDir, अक्षर *buf, माप_प्रकार buf_आकार,
               स्थिर अक्षर *name, माप_प्रकार name_len)
{
    काष्ठा scanner s;
    scanner_init(&s, ctx, name, name_len, parent_file_name, शून्य);
    s.buf_pos = 0;

    जबतक (!scanner_eof(&s) && !scanner_eol(&s)) {
        अगर (scanner_chr(&s, '%')) {
            अगर (scanner_chr(&s, '%')) {
                scanner_buf_append(&s, '%');
            }
            अन्यथा अगर (scanner_chr(&s, 'H')) {
                स्थिर अक्षर *home = xkb_context_getenv(ctx, "HOME");
                अगर (!home) {
                    scanner_err(&s, XKB_LOG_MESSAGE_NO_ID,
                                "%%H was used in an include statement, but the "
                                "HOME environment variable is not set");
                    वापस 0;
                }
                अगर (!scanner_buf_appends(&s, home)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_बफर_आकार,
                                "include path after expanding %%H is too long");
                    वापस 0;
                }
            }
            अन्यथा अगर (scanner_chr(&s, 'S')) {
                स्थिर अक्षर *default_मूल =
                    xkb_context_include_path_get_system_path(ctx);
                अगर (!scanner_buf_appends(&s, default_मूल) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_बफर_आकार,
                                "include path after expanding %%S is too long");
                    वापस 0;
                }
            }
            अन्यथा अगर (scanner_chr(&s, 'E')) {
                स्थिर अक्षर *default_मूल =
                    xkb_context_include_path_get_extra_path(ctx);
                अगर (!scanner_buf_appends(&s, default_मूल) ||
                    !scanner_buf_append(&s, '/') ||
                    !scanner_buf_appends(&s, typeDir)) {
                    scanner_err(&s, XKB_ERROR_INSUFFICIENT_बफर_आकार,
                                "include path after expanding %%E is too long");
                    वापस 0;
                }
            }
            अन्यथा {
                scanner_err(&s, XKB_ERROR_INSUFFICIENT_बफर_आकार,
                            "unknown %% format (%c) in include statement",
                            scanner_peek(&s));
                वापस 0;
            }
        }
        अन्यथा {
            scanner_buf_append(&s, scanner_अग्र(&s));
        }
    }
    अगर (!scanner_buf_append(&s, '\0')) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_बफर_आकार,
                    "include path is too long; max: %zu", माप(s.buf));
        वापस 0;
    }
    अगर (unlikely(s.buf_pos > buf_आकार)) {
        scanner_err(&s, XKB_ERROR_INSUFFICIENT_बफर_आकार,
                    "include path is too long: %zu > %zu", s.buf_pos, buf_आकार);
        वापस 0;
    }
    स_नकल(buf, s.buf, s.buf_pos);
    वापस s.buf_pos;
}

ssize_t
expand_path(काष्ठा xkb_context *ctx, स्थिर अक्षर* parent_file_name,
            स्थिर अक्षर *name, माप_प्रकार name_len, क्रमागत xkb_file_type type,
            अक्षर *buf, माप_प्रकार buf_आकार)
{
    /* Detect %-expansion */
    माप_प्रकार k;
    क्रम (k = 0; k < name_len; k++) {
        अगर (name[k] == '%') {
            जाओ expand;
        }
    }

    वापस 0;

expand:
    /* Process %-expansion */

    /* Copy prefix (usual case: k == 0) */
    अगर (unlikely(k >= buf_आकार)) {
        log_err(ctx, XKB_ERROR_INVALID_PATH,
                "Path is too long: %zu > %zu, "
                "got raw path: %.*s\n",
                k, buf_आकार, (अचिन्हित पूर्णांक) name_len, name);
        वापस -1;
    }
    स_नकल(buf, name, k);

    /* Proceed to %-expansion */
    स्थिर अक्षर *typeDir = DirectoryForInclude(type);
    माप_प्रकार गिनती = expand_percent(ctx, parent_file_name, typeDir,
                                  buf + k, buf_आकार - k,
                                  name + k, name_len - k);
    अगर (!गिनती)
        वापस -1;
    गिनती += k;
    निश्चित(buf[गिनती - 1] == '\0');
    वापस (ssize_t) गिनती - 1;
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
FindFileInXkbPath(काष्ठा xkb_context *ctx, स्थिर अक्षर* parent_file_name,
                  स्थिर अक्षर *name, माप_प्रकार name_len, क्रमागत xkb_file_type type,
                  अक्षर *buf, माप_प्रकार buf_आकार, अचिन्हित पूर्णांक *offset,
                  bool required)
{
    /* We do not handle absolute paths here */
    निश्चित(!is_absolute_path(name));

    खाता *file = शून्य;
    अक्षर *name_बफर = शून्य;
    स्थिर अक्षर *typeDir = DirectoryForInclude(type);

    क्रम (अचिन्हित पूर्णांक i = *offset; i < xkb_context_अंक_include_paths(ctx); i++) {
        अगर (!snprintf_safe(buf, buf_आकार, "%s/%s/%.*s",
                           xkb_context_include_path_get(ctx, i),
                           typeDir, (अचिन्हित पूर्णांक) name_len, name)) {
            log_err(ctx, XKB_ERROR_INVALID_PATH,
                    "Path is too long: expected max length of %zu, "
                    "got: %s/%s/%.*s\n",
                    buf_आकार, xkb_context_include_path_get(ctx, i),
                    typeDir, (अचिन्हित पूर्णांक) name_len, name);
            जारी;
        }

        file = ख_खोलो(buf, "rb");
        अगर (file) {
            *offset = i;
            जाओ out;
        }
    }

    /* We only print warnings if we can’t find the file on the first lookup */
    अगर (required && *offset == 0) {
        log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_FOUND,
                "Couldn't find file \"%s/%.*s\" in include paths\n",
                typeDir, (अचिन्हित पूर्णांक) name_len, name);
        LogIncludePaths(ctx);
    }

out:
    मुक्त(name_बफर);
    वापस file;
}

bool
ExceedsIncludeMaxDepth(काष्ठा xkb_context *ctx, अचिन्हित पूर्णांक include_depth)
{
    अगर (include_depth >= INCLUDE_MAX_DEPTH) {
        log_err(ctx, XKB_ERROR_RECURSIVE_INCLUDE,
                "Exceeded include depth threshold (%u)",
                INCLUDE_MAX_DEPTH);
        वापस true;
    } अन्यथा {
        वापस false;
    }
}

XkbFile *
ProcessIncludeFile(काष्ठा xkb_context *ctx,
                   स्थिर काष्ठा parser_keymap_config *config,
                   स्थिर IncludeStmt *stmt,
                   क्रमागत xkb_file_type file_type, अक्षर *path, माप_प्रकार path_आकार)
{
    /*
     * Resolve include statement:
     * - Explicit map name: look for an *exact match* only.
     * - Default map: look for an *explicit default* map (i.e. tagged with
     *   `default`: exact match), else fallback to the first *implicit* default
     *   map (weak match).
     */
    XkbFile *xkb_file = शून्य;  /* Exact match */
    XkbFile *candidate = शून्य; /* Weak match */

    स्थिर अक्षर *stmt_file = stmt->file;
    माप_प्रकार stmt_file_len = म_माप(stmt_file);

    /* Process %-expansion, if any */
    // FIXME: use parent file name instead of “(unknow)”.
    स्थिर ssize_t expanded = expand_path(ctx, "(unknown)",
                                         stmt_file, stmt_file_len, file_type,
                                         path, path_आकार);
    अगर (expanded < 0) {
        /* Error */
        वापस शून्य;
    } अन्यथा अगर (expanded > 0) {
        /* %-expanded */
        stmt_file = path;
        stmt_file_len = (माप_प्रकार) expanded;
        निश्चित(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    खाता* file;
    अचिन्हित पूर्णांक offset = 0;
    स्थिर bool absolute_path = is_absolute_path(stmt_file);
    अगर (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        निश्चित(stmt_file[stmt_file_len] == '\0');
        file = ख_खोलो(stmt_file, "rb");
    } अन्यथा {
        /* Relative path: lookup the first XKB path */
        अगर (unlikely(expanded)) {
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
            file = शून्य;
        } अन्यथा {
            // FIXME: use parent file name instead of “(unknow)”.
            file = FindFileInXkbPath(ctx, "(unknown)",
                                     stmt_file, stmt_file_len, file_type,
                                     path, path_आकार, &offset, true);
        }
    }

    जबतक (file) {
        xkb_file = XkbParseFile(ctx, config, file, stmt->file, stmt->map);
        ख_बंद(file);

        अगर (xkb_file) {
            अगर (xkb_file->file_type != file_type) {
                log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                        "Include file of wrong type (expected %s, got %s); "
                        "Include file \"%s\" ignored\n",
                        xkb_file_type_to_string(file_type),
                        xkb_file_type_to_string(xkb_file->file_type), stmt->file);
                FreeXkbFile(xkb_file);
                xkb_file = शून्य;
            } अन्यथा अगर (stmt->map || (xkb_file->flags & MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops here.
                 */
                अवरोध;
            } अन्यथा अगर (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = xkb_file;
                xkb_file = शून्य;
            } अन्यथा {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                FreeXkbFile(xkb_file);
                xkb_file = शून्य;
            }
        }

        अगर (absolute_path) {
            /* There is no point to search further if the path is absolute */
            अवरोध;
        }

        offset++;
        file = FindFileInXkbPath(ctx, "(unknown)",
                                 stmt_file, stmt_file_len, file_type,
                                 path, path_आकार, &offset, true);
    }

    अगर (!xkb_file) {
        /* No exact match: use weak match, if any */
        xkb_file = candidate;
    } अन्यथा {
        /* Found exact match: discard weak match, if any */
        FreeXkbFile(candidate);
    }

    अगर (!xkb_file) {
        अगर (stmt->map)
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s(%s)'\n",
                    stmt->file, stmt->map);
        अन्यथा
            log_err(ctx, XKB_ERROR_INVALID_INCLUDED_FILE,
                    "Couldn't process include statement for '%s'\n",
                    stmt->file);
    }

    वापस xkb_file;
}
