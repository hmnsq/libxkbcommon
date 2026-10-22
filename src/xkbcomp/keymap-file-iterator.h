/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"

#ଅନ୍ତର୍ଭାଵ "src/darray.h"
#ଅନ୍ତର୍ଭାଵ "src/scanner-utils.h"
#ଅନ୍ତର୍ଭାଵ "src/utils.h"
#ଅନ୍ତର୍ଭାଵ "ast.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp-priv.h"

XKB_EXPORT_PRIVATE ଅଚଳ ଅକ୍ଷର *
xkb_file_type_name(ଗଣନା xkb_file_type type);

XKB_EXPORT_PRIVATE ଅଚଳ ଅକ୍ଷର *
xkb_merge_mode_name(ଗଣନା merge_mode type);

XKB_EXPORT_PRIVATE ଅଚଳ ଅକ୍ଷର *
xkb_map_flags_string_iter(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *index, ଗଣନା xkb_map_flags flags);

XKB_EXPORT_PRIVATE FILE *
xkb_resolve_file(ସଂରଚନା xkb_context *ctx,
                 ଗଣନା xkb_file_type file_type,
                 ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *map,
                 ଅକ୍ଷର *resolved_path, size_t resolved_path_size,
                 ଅକ୍ଷର *resolved_map, size_t resolved_map_size);

/** An atomic include entry */
ସଂରଚନା xkb_file_include {
    bool valid:1;
    bool explicit_section:1;
    ଗଣନା merge_mode merge;
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
    ଗଣନା xkb_map_flags flags;
};

ସଂରଚନା xkb_file_include_group {
    darray_size_t start;
    darray_size_t end;
};

/** A file section: `xkb_{keycodes,compat,geometry,symbols,types} */
ସଂରଚନା xkb_file_section {
    darray_size_t name;
    ଗଣନା xkb_file_type file_type;
    ଗଣନା xkb_map_flags flags;
    darray(ସଂରଚନା xkb_file_include_group) include_groups;
    darray(ସଂରଚନା xkb_file_include) includes;
    /**
     * Char array to store all the strings
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_char buffer;
};

XKB_EXPORT_PRIVATE ଶୂନ୍ୟ
xkb_file_section_init(ସଂରଚନା xkb_file_section *section);

XKB_EXPORT_PRIVATE ଶୂନ୍ୟ
xkb_file_section_free(ସଂରଚନା xkb_file_section *section);

ଗଣନା xkb_file_iterator_flags {
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
xkb_file_section_parse(ସଂରଚନା xkb_context *ctx,
                       ଗଣନା xkb_file_iterator_flags iterator_flags,
                       ଗଣନା xkb_keymap_format format,
                       ଗଣନା xkb_keymap_compile_flags compile_flags,
                       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_depth,
                       ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *map,
                       ସଂରଚନା xkb_file_section *section);

XKB_EXPORT_PRIVATE ଅଚଳ ଅକ୍ଷର *
xkb_file_section_get_string(ଅଚଳ ସଂରଚନା xkb_file_section *section,
                            darray_size_t idx);

/** Iterator over the sections of a keymap file */
ସଂରଚନା xkb_file_iterator {
    ସଂରଚନା xkb_context *ctx;
    /** File to analyze */
    ଅଚଳ ଅକ୍ଷର *path;
    /** Section to analyze; if NULL then iter over all the sections */
    ଅଚଳ ଅକ୍ଷର *map;
    /** Pending XKB file */
    XkbFile *pending_xkb_file;
    /** Pending component of the XKB file */
    XkbFile *pending_section;

    /** Scanner of the keymap file */
    ସଂରଚନା scanner scanner;
    /** Current section */
    ସଂରଚନା xkb_file_section section;

    /** Flags */
    ଗଣନା xkb_file_iterator_flags flags;
    /** File type to check/filter */
    ଗଣନା xkb_file_type type;

    /** Parser configuration */
    ସଂରଚନା parser_keymap_config parser_config;

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
XKB_EXPORT_PRIVATE ସଂରଚନା xkb_file_iterator *
xkb_file_iterator_new_from_buffer(ସଂରଚନା xkb_context *ctx,
                                  ଗଣନା xkb_file_iterator_flags iterator_flags,
                                  ଗଣନା xkb_keymap_format format,
                                  ଗଣନା xkb_keymap_compile_flags compile_flags,
                                  ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *map,
                                  ଗଣନା xkb_file_type file_type,
                                  ଅଚଳ ଅକ୍ଷର *string, size_t length);

XKB_EXPORT_PRIVATE ଶୂନ୍ୟ
xkb_file_iterator_free(ସଂରଚନା xkb_file_iterator *iter);

XKB_EXPORT_PRIVATE bool
xkb_file_iterator_next(ସଂରଚନା xkb_file_iterator *iter,
                       ଅଚଳ ସଂରଚନା xkb_file_section **section);
