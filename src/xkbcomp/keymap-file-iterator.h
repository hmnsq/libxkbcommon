/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ "xkbcommon/xkbcommon.h"

#অন্তর্ভাৱ "src/darray.h"
#অন্তর্ভাৱ "src/scanner-utils.h"
#অন্তর্ভাৱ "src/utils.h"
#অন্তর্ভাৱ "ast.h"
#অন্তর্ভাৱ "xkbcomp-priv.h"

XKB_EXPORT_PRIVATE ধ্রুৱ ৱর্ণ *
xkb_file_type_name(গণনা xkb_file_type type);

XKB_EXPORT_PRIVATE ধ্রুৱ ৱর্ণ *
xkb_merge_mode_name(গণনা merge_mode type);

XKB_EXPORT_PRIVATE ধ্রুৱ ৱর্ণ *
xkb_map_flags_string_iter(unsigned পূর্ণাঙ্ক *index, গণনা xkb_map_flags flags);

XKB_EXPORT_PRIVATE FILE *
xkb_resolve_file(সংরচনা xkb_context *ctx,
                 গণনা xkb_file_type file_type,
                 ধ্রুৱ ৱর্ণ *path, ধ্রুৱ ৱর্ণ *map,
                 ৱর্ণ *resolved_path, size_t resolved_path_size,
                 ৱর্ণ *resolved_map, size_t resolved_map_size);

/** An atomic include entry */
সংরচনা xkb_file_include {
    bool valid:1;
    bool explicit_section:1;
    গণনা merge_mode merge;
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
    গণনা xkb_map_flags flags;
};

সংরচনা xkb_file_include_group {
    darray_size_t start;
    darray_size_t end;
};

/** A file section: `xkb_{keycodes,compat,geometry,symbols,types} */
সংরচনা xkb_file_section {
    darray_size_t name;
    গণনা xkb_file_type file_type;
    গণনা xkb_map_flags flags;
    darray(সংরচনা xkb_file_include_group) include_groups;
    darray(সংরচনা xkb_file_include) includes;
    /**
     * Char array to store all the strings
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_char buffer;
};

XKB_EXPORT_PRIVATE শূন্য
xkb_file_section_init(সংরচনা xkb_file_section *section);

XKB_EXPORT_PRIVATE শূন্য
xkb_file_section_free(সংরচনা xkb_file_section *section);

গণনা xkb_file_iterator_flags {
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
xkb_file_section_parse(সংরচনা xkb_context *ctx,
                       গণনা xkb_file_iterator_flags iterator_flags,
                       গণনা xkb_keymap_format format,
                       গণনা xkb_keymap_compile_flags compile_flags,
                       unsigned পূর্ণাঙ্ক include_depth,
                       ধ্রুৱ ৱর্ণ *path, ধ্রুৱ ৱর্ণ *map,
                       সংরচনা xkb_file_section *section);

XKB_EXPORT_PRIVATE ধ্রুৱ ৱর্ণ *
xkb_file_section_get_string(ধ্রুৱ সংরচনা xkb_file_section *section,
                            darray_size_t idx);

/** Iterator over the sections of a keymap file */
সংরচনা xkb_file_iterator {
    সংরচনা xkb_context *ctx;
    /** File to analyze */
    ধ্রুৱ ৱর্ণ *path;
    /** Section to analyze; if NULL then iter over all the sections */
    ধ্রুৱ ৱর্ণ *map;
    /** Pending XKB file */
    XkbFile *pending_xkb_file;
    /** Pending component of the XKB file */
    XkbFile *pending_section;

    /** Scanner of the keymap file */
    সংরচনা scanner scanner;
    /** Current section */
    সংরচনা xkb_file_section section;

    /** Flags */
    গণনা xkb_file_iterator_flags flags;
    /** File type to check/filter */
    গণনা xkb_file_type type;

    /** Parser configuration */
    সংরচনা parser_keymap_config parser_config;

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
XKB_EXPORT_PRIVATE সংরচনা xkb_file_iterator *
xkb_file_iterator_new_from_buffer(সংরচনা xkb_context *ctx,
                                  গণনা xkb_file_iterator_flags iterator_flags,
                                  গণনা xkb_keymap_format format,
                                  গণনা xkb_keymap_compile_flags compile_flags,
                                  ধ্রুৱ ৱর্ণ *path, ধ্রুৱ ৱর্ণ *map,
                                  গণনা xkb_file_type file_type,
                                  ধ্রুৱ ৱর্ণ *string, size_t length);

XKB_EXPORT_PRIVATE শূন্য
xkb_file_iterator_free(সংরচনা xkb_file_iterator *iter);

XKB_EXPORT_PRIVATE bool
xkb_file_iterator_next(সংরচনা xkb_file_iterator *iter,
                       ধ্রুৱ সংরচনা xkb_file_section **section);
