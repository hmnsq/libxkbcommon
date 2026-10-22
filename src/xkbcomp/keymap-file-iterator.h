/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"

#અન્તર્ભાવ "src/darray.h"
#અન્તર્ભાવ "src/scanner-utils.h"
#અન્તર્ભાવ "src/utils.h"
#અન્તર્ભાવ "ast.h"
#અન્તર્ભાવ "xkbcomp-priv.h"

XKB_EXPORT_PRIVATE અચલ અક્ષર *
xkb_file_type_name(ગણના xkb_file_type type);

XKB_EXPORT_PRIVATE અચલ અક્ષર *
xkb_merge_mode_name(ગણના merge_mode type);

XKB_EXPORT_PRIVATE અચલ અક્ષર *
xkb_map_flags_string_iter(unsigned પૂર્ણાંક *index, ગણના xkb_map_flags flags);

XKB_EXPORT_PRIVATE FILE *
xkb_resolve_file(સંરચના xkb_context *ctx,
                 ગણના xkb_file_type file_type,
                 અચલ અક્ષર *path, અચલ અક્ષર *map,
                 અક્ષર *resolved_path, size_t resolved_path_size,
                 અક્ષર *resolved_map, size_t resolved_map_size);

/** An atomic include entry */
સંરચના xkb_file_include {
    bool valid:1;
    bool explicit_section:1;
    ગણના merge_mode merge;
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
    ગણના xkb_map_flags flags;
};

સંરચના xkb_file_include_group {
    darray_size_t start;
    darray_size_t end;
};

/** A file section: `xkb_{keycodes,compat,geometry,symbols,types} */
સંરચના xkb_file_section {
    darray_size_t name;
    ગણના xkb_file_type file_type;
    ગણના xkb_map_flags flags;
    darray(સંરચના xkb_file_include_group) include_groups;
    darray(સંરચના xkb_file_include) includes;
    /**
     * Char array to store all the strings
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_char buffer;
};

XKB_EXPORT_PRIVATE શૂન્ય
xkb_file_section_init(સંરચના xkb_file_section *section);

XKB_EXPORT_PRIVATE શૂન્ય
xkb_file_section_free(સંરચના xkb_file_section *section);

ગણના xkb_file_iterator_flags {
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
xkb_file_section_parse(સંરચના xkb_context *ctx,
                       ગણના xkb_file_iterator_flags iterator_flags,
                       ગણના xkb_keymap_format format,
                       ગણના xkb_keymap_compile_flags compile_flags,
                       unsigned પૂર્ણાંક include_depth,
                       અચલ અક્ષર *path, અચલ અક્ષર *map,
                       સંરચના xkb_file_section *section);

XKB_EXPORT_PRIVATE અચલ અક્ષર *
xkb_file_section_get_string(અચલ સંરચના xkb_file_section *section,
                            darray_size_t idx);

/** Iterator over the sections of a keymap file */
સંરચના xkb_file_iterator {
    સંરચના xkb_context *ctx;
    /** File to analyze */
    અચલ અક્ષર *path;
    /** Section to analyze; if NULL then iter over all the sections */
    અચલ અક્ષર *map;
    /** Pending XKB file */
    XkbFile *pending_xkb_file;
    /** Pending component of the XKB file */
    XkbFile *pending_section;

    /** Scanner of the keymap file */
    સંરચના scanner scanner;
    /** Current section */
    સંરચના xkb_file_section section;

    /** Flags */
    ગણના xkb_file_iterator_flags flags;
    /** File type to check/filter */
    ગણના xkb_file_type type;

    /** Parser configuration */
    સંરચના parser_keymap_config parser_config;

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
XKB_EXPORT_PRIVATE સંરચના xkb_file_iterator *
xkb_file_iterator_new_from_buffer(સંરચના xkb_context *ctx,
                                  ગણના xkb_file_iterator_flags iterator_flags,
                                  ગણના xkb_keymap_format format,
                                  ગણના xkb_keymap_compile_flags compile_flags,
                                  અચલ અક્ષર *path, અચલ અક્ષર *map,
                                  ગણના xkb_file_type file_type,
                                  અચલ અક્ષર *string, size_t length);

XKB_EXPORT_PRIVATE શૂન્ય
xkb_file_iterator_free(સંરચના xkb_file_iterator *iter);

XKB_EXPORT_PRIVATE bool
xkb_file_iterator_next(સંરચના xkb_file_iterator *iter,
                       અચલ સંરચના xkb_file_section **section);
