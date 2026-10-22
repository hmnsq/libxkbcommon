/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"

#ಅನ್ತರ್ಭಾವ "src/darray.h"
#ಅನ್ತರ್ಭಾವ "src/scanner-utils.h"
#ಅನ್ತರ್ಭಾವ "src/utils.h"
#ಅನ್ತರ್ಭಾವ "ast.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"

XKB_EXPORT_PRIVATE ಧ್ರುವ ವರ್ಣ *
xkb_file_type_name(ಗಣನಾ xkb_file_type type);

XKB_EXPORT_PRIVATE ಧ್ರುವ ವರ್ಣ *
xkb_merge_mode_name(ಗಣನಾ merge_mode type);

XKB_EXPORT_PRIVATE ಧ್ರುವ ವರ್ಣ *
xkb_map_flags_string_iter(unsigned ಪೂರ್ಣಾಙ್ಕ *index, ಗಣನಾ xkb_map_flags flags);

XKB_EXPORT_PRIVATE FILE *
xkb_resolve_file(ಸಂರಚನಾ xkb_context *ctx,
                 ಗಣನಾ xkb_file_type file_type,
                 ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *map,
                 ವರ್ಣ *resolved_path, size_t resolved_path_size,
                 ವರ್ಣ *resolved_map, size_t resolved_map_size);

/** An atomic include entry */
ಸಂರಚನಾ xkb_file_include {
    bool valid:1;
    bool explicit_section:1;
    ಗಣನಾ merge_mode merge;
    /*
     * The following are indices from `xkb_file_section::buffer`
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_size_t path;
    darray_size_t file;
    darray_size_t section;
    darray_size_t modifier;
    /* Flags of the included section, only set if valid = true */
    ಗಣನಾ xkb_map_flags flags;
};

ಸಂರಚನಾ xkb_file_include_group {
    darray_size_t start;
    darray_size_t end;
};

/** A file section: `xkb_{keycodes,compat,geometry,symbols,types} */
ಸಂರಚನಾ xkb_file_section {
    darray_size_t name;
    ಗಣನಾ xkb_file_type file_type;
    ಗಣನಾ xkb_map_flags flags;
    darray(ಸಂರಚನಾ xkb_file_include_group) include_groups;
    darray(ಸಂರಚನಾ xkb_file_include) includes;
    /**
     * Char array to store all the strings
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_char buffer;
};

XKB_EXPORT_PRIVATE ಶೂನ್ಯ
xkb_file_section_init(ಸಂರಚನಾ xkb_file_section *section);

XKB_EXPORT_PRIVATE ಶೂನ್ಯ
xkb_file_section_free(ಸಂರಚನಾ xkb_file_section *section);

ಗಣನಾ xkb_file_iterator_flags {
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
xkb_file_section_parse(ಸಂರಚನಾ xkb_context *ctx,
                       ಗಣನಾ xkb_file_iterator_flags iterator_flags,
                       ಗಣನಾ xkb_keymap_format format,
                       ಗಣನಾ xkb_keymap_compile_flags compile_flags,
                       unsigned ಪೂರ್ಣಾಙ್ಕ include_depth,
                       ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *map,
                       ಸಂರಚನಾ xkb_file_section *section);

XKB_EXPORT_PRIVATE ಧ್ರುವ ವರ್ಣ *
xkb_file_section_get_string(ಧ್ರುವ ಸಂರಚನಾ xkb_file_section *section,
                            darray_size_t idx);

/** Iterator over the sections of a keymap file */
ಸಂರಚನಾ xkb_file_iterator {
    ಸಂರಚನಾ xkb_context *ctx;
    /** File to analyze */
    ಧ್ರುವ ವರ್ಣ *path;
    /** Section to analyze; if NULL then iter over all the sections */
    ಧ್ರುವ ವರ್ಣ *map;
    /** Pending XKB file */
    XkbFile *pending_xkb_file;
    /** Pending component of the XKB file */
    XkbFile *pending_section;

    /** Scanner of the keymap file */
    ಸಂರಚನಾ scanner scanner;
    /** Current section */
    ಸಂರಚನಾ xkb_file_section section;

    /** Flags */
    ಗಣನಾ xkb_file_iterator_flags flags;
    /** File type to check/filter */
    ಗಣನಾ xkb_file_type type;

    /** Parser configuration */
    ಸಂರಚನಾ parser_keymap_config parser_config;

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
XKB_EXPORT_PRIVATE ಸಂರಚನಾ xkb_file_iterator *
xkb_file_iterator_new_from_buffer(ಸಂರಚನಾ xkb_context *ctx,
                                  ಗಣನಾ xkb_file_iterator_flags iterator_flags,
                                  ಗಣನಾ xkb_keymap_format format,
                                  ಗಣನಾ xkb_keymap_compile_flags compile_flags,
                                  ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *map,
                                  ಗಣನಾ xkb_file_type file_type,
                                  ಧ್ರುವ ವರ್ಣ *string, size_t length);

XKB_EXPORT_PRIVATE ಶೂನ್ಯ
xkb_file_iterator_free(ಸಂರಚನಾ xkb_file_iterator *iter);

XKB_EXPORT_PRIVATE bool
xkb_file_iterator_next(ಸಂರಚನಾ xkb_file_iterator *iter,
                       ಧ್ರುವ ಸಂರಚನಾ xkb_file_section **section);
