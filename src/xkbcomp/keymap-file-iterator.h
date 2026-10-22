/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"

#അന്തര്ഭാവ "src/darray.h"
#അന്തര്ഭാവ "src/scanner-utils.h"
#അന്തര്ഭാവ "src/utils.h"
#അന്തര്ഭാവ "ast.h"
#അന്തര്ഭാവ "xkbcomp-priv.h"

XKB_EXPORT_PRIVATE സ്ഥിരം അക്ഷരം *
xkb_file_type_name(എണ്ണൽ xkb_file_type type);

XKB_EXPORT_PRIVATE സ്ഥിരം അക്ഷരം *
xkb_merge_mode_name(എണ്ണൽ merge_mode type);

XKB_EXPORT_PRIVATE സ്ഥിരം അക്ഷരം *
xkb_map_flags_string_iter(unsigned പൂർണ്ണസംഖ്യ *index, എണ്ണൽ xkb_map_flags flags);

XKB_EXPORT_PRIVATE FILE *
xkb_resolve_file(ഘടന xkb_context *ctx,
                 എണ്ണൽ xkb_file_type file_type,
                 സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *map,
                 അക്ഷരം *resolved_path, size_t resolved_path_size,
                 അക്ഷരം *resolved_map, size_t resolved_map_size);

/** An atomic include entry */
ഘടന xkb_file_include {
    bool valid:1;
    bool explicit_section:1;
    എണ്ണൽ merge_mode merge;
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
    എണ്ണൽ xkb_map_flags flags;
};

ഘടന xkb_file_include_group {
    darray_size_t start;
    darray_size_t end;
};

/** A file section: `xkb_{keycodes,compat,geometry,symbols,types} */
ഘടന xkb_file_section {
    darray_size_t name;
    എണ്ണൽ xkb_file_type file_type;
    എണ്ണൽ xkb_map_flags flags;
    darray(ഘടന xkb_file_include_group) include_groups;
    darray(ഘടന xkb_file_include) includes;
    /**
     * Char array to store all the strings
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_char buffer;
};

XKB_EXPORT_PRIVATE ശൂന്യം
xkb_file_section_init(ഘടന xkb_file_section *section);

XKB_EXPORT_PRIVATE ശൂന്യം
xkb_file_section_free(ഘടന xkb_file_section *section);

എണ്ണൽ xkb_file_iterator_flags {
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
xkb_file_section_parse(ഘടന xkb_context *ctx,
                       എണ്ണൽ xkb_file_iterator_flags iterator_flags,
                       എണ്ണൽ xkb_keymap_format format,
                       എണ്ണൽ xkb_keymap_compile_flags compile_flags,
                       unsigned പൂർണ്ണസംഖ്യ include_depth,
                       സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *map,
                       ഘടന xkb_file_section *section);

XKB_EXPORT_PRIVATE സ്ഥിരം അക്ഷരം *
xkb_file_section_get_string(സ്ഥിരം ഘടന xkb_file_section *section,
                            darray_size_t idx);

/** Iterator over the sections of a keymap file */
ഘടന xkb_file_iterator {
    ഘടന xkb_context *ctx;
    /** File to analyze */
    സ്ഥിരം അക്ഷരം *path;
    /** Section to analyze; if NULL then iter over all the sections */
    സ്ഥിരം അക്ഷരം *map;
    /** Pending XKB file */
    XkbFile *pending_xkb_file;
    /** Pending component of the XKB file */
    XkbFile *pending_section;

    /** Scanner of the keymap file */
    ഘടന scanner scanner;
    /** Current section */
    ഘടന xkb_file_section section;

    /** Flags */
    എണ്ണൽ xkb_file_iterator_flags flags;
    /** File type to check/filter */
    എണ്ണൽ xkb_file_type type;

    /** Parser configuration */
    ഘടന parser_keymap_config parser_config;

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
XKB_EXPORT_PRIVATE ഘടന xkb_file_iterator *
xkb_file_iterator_new_from_buffer(ഘടന xkb_context *ctx,
                                  എണ്ണൽ xkb_file_iterator_flags iterator_flags,
                                  എണ്ണൽ xkb_keymap_format format,
                                  എണ്ണൽ xkb_keymap_compile_flags compile_flags,
                                  സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *map,
                                  എണ്ണൽ xkb_file_type file_type,
                                  സ്ഥിരം അക്ഷരം *string, size_t length);

XKB_EXPORT_PRIVATE ശൂന്യം
xkb_file_iterator_free(ഘടന xkb_file_iterator *iter);

XKB_EXPORT_PRIVATE bool
xkb_file_iterator_next(ഘടന xkb_file_iterator *iter,
                       സ്ഥിരം ഘടന xkb_file_section **section);
