/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ "xkbcommon/xkbcommon.h"

#அந்தர்பாவ "src/darray.h"
#அந்தர்பாவ "src/scanner-utils.h"
#அந்தர்பாவ "src/utils.h"
#அந்தர்பாவ "ast.h"
#அந்தர்பாவ "xkbcomp-priv.h"

XKB_EXPORT_PRIVATE மாறிலி எழுத்து *
xkb_file_type_name(எண்ணிக்கை xkb_file_type type);

XKB_EXPORT_PRIVATE மாறிலி எழுத்து *
xkb_merge_mode_name(எண்ணிக்கை merge_mode type);

XKB_EXPORT_PRIVATE மாறிலி எழுத்து *
xkb_map_flags_string_iter(unsigned முழு *index, எண்ணிக்கை xkb_map_flags flags);

XKB_EXPORT_PRIVATE FILE *
xkb_resolve_file(கட்டமைப்பு xkb_context *ctx,
                 எண்ணிக்கை xkb_file_type file_type,
                 மாறிலி எழுத்து *path, மாறிலி எழுத்து *map,
                 எழுத்து *resolved_path, size_t resolved_path_size,
                 எழுத்து *resolved_map, size_t resolved_map_size);

/** An atomic include entry */
கட்டமைப்பு xkb_file_include {
    bool valid:1;
    bool explicit_section:1;
    எண்ணிக்கை merge_mode merge;
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
    எண்ணிக்கை xkb_map_flags flags;
};

கட்டமைப்பு xkb_file_include_group {
    darray_size_t start;
    darray_size_t end;
};

/** A file section: `xkb_{keycodes,compat,geometry,symbols,types} */
கட்டமைப்பு xkb_file_section {
    darray_size_t name;
    எண்ணிக்கை xkb_file_type file_type;
    எண்ணிக்கை xkb_map_flags flags;
    darray(கட்டமைப்பு xkb_file_include_group) include_groups;
    darray(கட்டமைப்பு xkb_file_include) includes;
    /**
     * Char array to store all the strings
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_char buffer;
};

XKB_EXPORT_PRIVATE வெறுமை
xkb_file_section_init(கட்டமைப்பு xkb_file_section *section);

XKB_EXPORT_PRIVATE வெறுமை
xkb_file_section_free(கட்டமைப்பு xkb_file_section *section);

எண்ணிக்கை xkb_file_iterator_flags {
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
xkb_file_section_parse(கட்டமைப்பு xkb_context *ctx,
                       எண்ணிக்கை xkb_file_iterator_flags iterator_flags,
                       எண்ணிக்கை xkb_keymap_format format,
                       எண்ணிக்கை xkb_keymap_compile_flags compile_flags,
                       unsigned முழு include_depth,
                       மாறிலி எழுத்து *path, மாறிலி எழுத்து *map,
                       கட்டமைப்பு xkb_file_section *section);

XKB_EXPORT_PRIVATE மாறிலி எழுத்து *
xkb_file_section_get_string(மாறிலி கட்டமைப்பு xkb_file_section *section,
                            darray_size_t idx);

/** Iterator over the sections of a keymap file */
கட்டமைப்பு xkb_file_iterator {
    கட்டமைப்பு xkb_context *ctx;
    /** File to analyze */
    மாறிலி எழுத்து *path;
    /** Section to analyze; if NULL then iter over all the sections */
    மாறிலி எழுத்து *map;
    /** Pending XKB file */
    XkbFile *pending_xkb_file;
    /** Pending component of the XKB file */
    XkbFile *pending_section;

    /** Scanner of the keymap file */
    கட்டமைப்பு scanner scanner;
    /** Current section */
    கட்டமைப்பு xkb_file_section section;

    /** Flags */
    எண்ணிக்கை xkb_file_iterator_flags flags;
    /** File type to check/filter */
    எண்ணிக்கை xkb_file_type type;

    /** Parser configuration */
    கட்டமைப்பு parser_keymap_config parser_config;

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
XKB_EXPORT_PRIVATE கட்டமைப்பு xkb_file_iterator *
xkb_file_iterator_new_from_buffer(கட்டமைப்பு xkb_context *ctx,
                                  எண்ணிக்கை xkb_file_iterator_flags iterator_flags,
                                  எண்ணிக்கை xkb_keymap_format format,
                                  எண்ணிக்கை xkb_keymap_compile_flags compile_flags,
                                  மாறிலி எழுத்து *path, மாறிலி எழுத்து *map,
                                  எண்ணிக்கை xkb_file_type file_type,
                                  மாறிலி எழுத்து *string, size_t length);

XKB_EXPORT_PRIVATE வெறுமை
xkb_file_iterator_free(கட்டமைப்பு xkb_file_iterator *iter);

XKB_EXPORT_PRIVATE bool
xkb_file_iterator_next(கட்டமைப்பு xkb_file_iterator *iter,
                       மாறிலி கட்டமைப்பு xkb_file_section **section);
