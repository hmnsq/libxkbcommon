/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#समावेश "config.h"

#समावेश "xkbcommon/xkbcommon.h"

#समावेश "src/darray.h"
#समावेश "src/scanner-utils.h"
#समावेश "src/utils.h"
#समावेश "ast.h"
#समावेश "xkbcomp-priv.h"

XKB_EXPORT_PRIVATE स्थिर अक्षर *
xkb_file_type_name(क्रमागत xkb_file_type type);

XKB_EXPORT_PRIVATE स्थिर अक्षर *
xkb_merge_mode_name(क्रमागत merge_mode type);

XKB_EXPORT_PRIVATE स्थिर अक्षर *
xkb_map_flags_string_iter(अचिन्हित पूर्णांक *सूचकांक, क्रमागत xkb_map_flags flags);

XKB_EXPORT_PRIVATE खाता *
xkb_resolve_file(काष्ठा xkb_context *ctx,
                 क्रमागत xkb_file_type file_type,
                 स्थिर अक्षर *path, स्थिर अक्षर *map,
                 अक्षर *resolved_path, माप_प्रकार resolved_path_आकार,
                 अक्षर *resolved_map, माप_प्रकार resolved_map_आकार);

/** An atomic include entry */
काष्ठा xkb_file_include {
    bool valid:1;
    bool explicit_section:1;
    क्रमागत merge_mode merge;
    /*
     * The following are indices from `xkb_file_section::buffer`
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_आकार_t path;
    darray_आकार_t file;
    darray_आकार_t section;
    darray_आकार_t modifier;
    /* Flags of the included section, only set if valid = true */
    क्रमागत xkb_map_flags flags;
};

काष्ठा xkb_file_include_group {
    darray_आकार_t start;
    darray_आकार_t end;
};

/** A file section: `xkb_{keycodes,compat,geometry,symbols,types} */
काष्ठा xkb_file_section {
    darray_आकार_t name;
    क्रमागत xkb_file_type file_type;
    क्रमागत xkb_map_flags flags;
    darray(काष्ठा xkb_file_include_group) include_groups;
    darray(काष्ठा xkb_file_include) includes;
    /**
     * Char array to store all the strings
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_char बफर;
};

XKB_EXPORT_PRIVATE व्योम
xkb_file_section_init(काष्ठा xkb_file_section *section);

XKB_EXPORT_PRIVATE व्योम
xkb_file_section_free(काष्ठा xkb_file_section *section);

क्रमागत xkb_file_iterator_flags {
    XKB_FILE_ITERATOR_NO_FLAG = 0,
    /** Include errors are fatal */
    XKB_FILE_ITERATOR_FAIL_ON_INCLUDE_ERROR = (1 << 0),
    /** Do not list includes */
    XKB_FILE_ITERATOR_NO_INCLUDES = (1 << 1)
};

/**
 * Parse a single keymap file section
 *
 * @param[in]    ctx            The XKB context
 * @param[in]    iterator_flags The flags used to parse the section
 * @param[in]    format         The keymap format used to parse the keymap file
 * @param[in]    compile_flags  The flags used to parse the keymap file
 * @param[in]    path           The path of the keymap file to parse
 * @param[in]    map            The name of a specific section in the file
 * @param[inout] section        The resulting section
 *
 * @returns `true` on success, else `false`
 */
XKB_EXPORT_PRIVATE bool
xkb_file_section_parse(काष्ठा xkb_context *ctx,
                       क्रमागत xkb_file_iterator_flags iterator_flags,
                       क्रमागत xkb_keymap_format format,
                       क्रमागत xkb_keymap_compile_flags compile_flags,
                       अचिन्हित पूर्णांक include_depth,
                       स्थिर अक्षर *path, स्थिर अक्षर *map,
                       काष्ठा xkb_file_section *section);

XKB_EXPORT_PRIVATE स्थिर अक्षर *
xkb_file_section_get_string(स्थिर काष्ठा xkb_file_section *section,
                            darray_आकार_t idx);

/** Iterator over the sections of a keymap file */
काष्ठा xkb_file_iterator {
    काष्ठा xkb_context *ctx;
    /** File to analyze */
    स्थिर अक्षर *path;
    /** Section to analyze; if NULL then iter over all the sections */
    स्थिर अक्षर *map;
    /** Pending XKB file */
    XkbFile *pending_xkb_file;
    /** Pending component of the XKB file */
    XkbFile *pending_section;

    /** Scanner of the keymap file */
    काष्ठा scanner scanner;
    /** Current section */
    काष्ठा xkb_file_section section;

    /** Flags */
    क्रमागत xkb_file_iterator_flags flags;
    /** File type to check/filter */
    क्रमागत xkb_file_type type;

    /** Parser configuration */
    काष्ठा parser_keymap_config parser_config;

    /** Indicate whether the iterator reached the end */
    bool finished;
};

/**
 * Create a keymap file section iterator from a buffer
 *
 * @param ctx            The XKB context
 * @param iterator_flags The flags used to parse the section
 * @param format         The keymap format used to parse the keymap file
 * @param compile_flags  The flags used to parse the keymap file
 * @param path           The path of the keymap file parsed (informative)
 * @param map            The name of a specific section in the file, else NULL
 * to iterate over all the sections of the file.
 * @param file_type      If the input is a keymap, iterate only over the
 * corresponding component; else check that the parsed file type matches.
 * @param string         The buffer to parse
 * @param length         The length of the buffer to parse
 *
 * @returns A `malloc`ed file iterator on success, else `NULL`.
 */
XKB_EXPORT_PRIVATE काष्ठा xkb_file_iterator *
xkb_file_iterator_new_from_बफर(काष्ठा xkb_context *ctx,
                                  क्रमागत xkb_file_iterator_flags iterator_flags,
                                  क्रमागत xkb_keymap_format format,
                                  क्रमागत xkb_keymap_compile_flags compile_flags,
                                  स्थिर अक्षर *path, स्थिर अक्षर *map,
                                  क्रमागत xkb_file_type file_type,
                                  स्थिर अक्षर *string, माप_प्रकार लंबाई);

XKB_EXPORT_PRIVATE व्योम
xkb_file_iterator_free(काष्ठा xkb_file_iterator *iter);

XKB_EXPORT_PRIVATE bool
xkb_file_iterator_अग्र(काष्ठा xkb_file_iterator *iter,
                       स्थिर काष्ठा xkb_file_section **section);
