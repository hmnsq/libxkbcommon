/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব "xkbcommon/xkbcommon.h"

#অন্তর্ভাব "src/darray.h"
#অন্তর্ভাব "src/scanner-utils.h"
#অন্তর্ভাব "src/utils.h"
#অন্তর্ভাব "ast.h"
#অন্তর্ভাব "xkbcomp-priv.h"

XKB_EXPORT_PRIVATE ধ্রুবক অক্ষর *
xkb_file_type_name(গণনা xkb_file_type type);

XKB_EXPORT_PRIVATE ধ্রুবক অক্ষর *
xkb_merge_mode_name(গণনা merge_mode type);

XKB_EXPORT_PRIVATE ধ্রুবক অক্ষর *
xkb_map_flags_string_iter(unsigned পূর্ণসংখ্যা *সূচক, গণনা xkb_map_flags flags);

XKB_EXPORT_PRIVATE FILE *
xkb_resolve_file(কাঠামো xkb_context *ctx,
                 গণনা xkb_file_type file_type,
                 ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর *map,
                 অক্ষর *resolved_path, আকার_t resolved_path_আকার,
                 অক্ষর *resolved_map, আকার_t resolved_map_আকার);

/** An atomic include entry */
কাঠামো xkb_file_include {
    bool valid:1;
    bool explicit_section:1;
    গণনা merge_mode merge;
    /*
     * The following are indices from `xkb_file_section::buffer`
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_আকার_t path;
    darray_আকার_t file;
    darray_আকার_t section;
    darray_আকার_t modifier;
    /* Flags of the included section, only set if valid = true */
    গণনা xkb_map_flags flags;
};

কাঠামো xkb_file_include_group {
    darray_আকার_t start;
    darray_আকার_t end;
};

/** A file section: `xkb_{keycodes,compat,geometry,symbols,types} */
কাঠামো xkb_file_section {
    darray_আকার_t name;
    গণনা xkb_file_type file_type;
    গণনা xkb_map_flags flags;
    darray(কাঠামো xkb_file_include_group) include_groups;
    darray(কাঠামো xkb_file_include) includes;
    /**
     * Char array to store all the strings
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_char বাফার;
};

XKB_EXPORT_PRIVATE শূন্য
xkb_file_section_init(কাঠামো xkb_file_section *section);

XKB_EXPORT_PRIVATE শূন্য
xkb_file_section_free(কাঠামো xkb_file_section *section);

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
xkb_file_section_parse(কাঠামো xkb_context *ctx,
                       গণনা xkb_file_iterator_flags iterator_flags,
                       গণনা xkb_keymap_format format,
                       গণনা xkb_keymap_compile_flags compile_flags,
                       unsigned পূর্ণসংখ্যা include_depth,
                       ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর *map,
                       কাঠামো xkb_file_section *section);

XKB_EXPORT_PRIVATE ধ্রুবক অক্ষর *
xkb_file_section_get_string(ধ্রুবক কাঠামো xkb_file_section *section,
                            darray_আকার_t idx);

/** Iterator over the sections of a keymap file */
কাঠামো xkb_file_iterator {
    কাঠামো xkb_context *ctx;
    /** File to analyze */
    ধ্রুবক অক্ষর *path;
    /** Section to analyze; if NULL then iter over all the sections */
    ধ্রুবক অক্ষর *map;
    /** Pending XKB file */
    XkbFile *pending_xkb_file;
    /** Pending component of the XKB file */
    XkbFile *pending_section;

    /** Scanner of the keymap file */
    কাঠামো scanner scanner;
    /** Current section */
    কাঠামো xkb_file_section section;

    /** Flags */
    গণনা xkb_file_iterator_flags flags;
    /** File type to check/filter */
    গণনা xkb_file_type type;

    /** Parser configuration */
    কাঠামো parser_keymap_config parser_config;

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
XKB_EXPORT_PRIVATE কাঠামো xkb_file_iterator *
xkb_file_iterator_new_from_বাফার(কাঠামো xkb_context *ctx,
                                  গণনা xkb_file_iterator_flags iterator_flags,
                                  গণনা xkb_keymap_format format,
                                  গণনা xkb_keymap_compile_flags compile_flags,
                                  ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর *map,
                                  গণনা xkb_file_type file_type,
                                  ধ্রুবক অক্ষর *string, আকার_t দৈর্ঘ্য);

XKB_EXPORT_PRIVATE শূন্য
xkb_file_iterator_free(কাঠামো xkb_file_iterator *iter);

XKB_EXPORT_PRIVATE bool
xkb_file_iterator_পরবর্তী(কাঠামো xkb_file_iterator *iter,
                       ধ্রুবক কাঠামো xkb_file_section **section);
