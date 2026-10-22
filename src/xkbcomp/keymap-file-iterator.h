/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#include "config.h"

#include "xkbcommon/xkbcommon.h"

#include "src/darray.h"
#include "src/scanner-utils.h"
#include "src/utils.h"
#include "ast.h"
#include "xkbcomp-priv.h"

XKB_EXPORT_PRIVATE const var_na *
xkb_file_type_name(ga_nanaa xkb_file_type type);

XKB_EXPORT_PRIVATE const var_na *
xkb_merge_mode_name(ga_nanaa merge_mode type);

XKB_EXPORT_PRIVATE const var_na *
xkb_map_flags_string_iter(unsigned පූර්ණ_සංඛ්‍යා *index, ga_nanaa xkb_map_flags flags);

XKB_EXPORT_PRIVATE FILE *
xkb_resolve_file(sa_nracanaa xkb_context *ctx,
                 ga_nanaa xkb_file_type file_type,
                 const var_na *path, const var_na *map,
                 var_na *resolved_path, size_t resolved_path_size,
                 var_na *resolved_map, size_t resolved_map_size);

/** An atomic include entry */
sa_nracanaa xkb_file_include {
    bool valid:1;
    bool explicit_section:1;
    ga_nanaa merge_mode merge;
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
    ga_nanaa xkb_map_flags flags;
};

sa_nracanaa xkb_file_include_group {
    darray_size_t start;
    darray_size_t end;
};

/** A file section: `xkb_{keycodes,compat,geometry,symbols,types} */
sa_nracanaa xkb_file_section {
    darray_size_t name;
    ga_nanaa xkb_file_type file_type;
    ga_nanaa xkb_map_flags flags;
    darray(sa_nracanaa xkb_file_include_group) include_groups;
    darray(sa_nracanaa xkb_file_include) includes;
    /**
     * Char array to store all the strings
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_char buffer;
};

XKB_EXPORT_PRIVATE void
xkb_file_section_init(sa_nracanaa xkb_file_section *section);

XKB_EXPORT_PRIVATE void
xkb_file_section_free(sa_nracanaa xkb_file_section *section);

ga_nanaa xkb_file_iterator_flags {
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
xkb_file_section_parse(sa_nracanaa xkb_context *ctx,
                       ga_nanaa xkb_file_iterator_flags iterator_flags,
                       ga_nanaa xkb_keymap_format format,
                       ga_nanaa xkb_keymap_compile_flags compile_flags,
                       unsigned පූර්ණ_සංඛ්‍යා include_depth,
                       const var_na *path, const var_na *map,
                       sa_nracanaa xkb_file_section *section);

XKB_EXPORT_PRIVATE const var_na *
xkb_file_section_get_string(const sa_nracanaa xkb_file_section *section,
                            darray_size_t idx);

/** Iterator over the sections of a keymap file */
sa_nracanaa xkb_file_iterator {
    sa_nracanaa xkb_context *ctx;
    /** File to analyze */
    const var_na *path;
    /** Section to analyze; if NULL then iter over all the sections */
    const var_na *map;
    /** Pending XKB file */
    XkbFile *pending_xkb_file;
    /** Pending component of the XKB file */
    XkbFile *pending_section;

    /** Scanner of the keymap file */
    sa_nracanaa scanner scanner;
    /** Current section */
    sa_nracanaa xkb_file_section section;

    /** Flags */
    ga_nanaa xkb_file_iterator_flags flags;
    /** File type to check/filter */
    ga_nanaa xkb_file_type type;

    /** Parser configuration */
    sa_nracanaa parser_keymap_config parser_config;

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
XKB_EXPORT_PRIVATE sa_nracanaa xkb_file_iterator *
xkb_file_iterator_new_from_buffer(sa_nracanaa xkb_context *ctx,
                                  ga_nanaa xkb_file_iterator_flags iterator_flags,
                                  ga_nanaa xkb_keymap_format format,
                                  ga_nanaa xkb_keymap_compile_flags compile_flags,
                                  const var_na *path, const var_na *map,
                                  ga_nanaa xkb_file_type file_type,
                                  const var_na *string, size_t length);

XKB_EXPORT_PRIVATE void
xkb_file_iterator_free(sa_nracanaa xkb_file_iterator *iter);

XKB_EXPORT_PRIVATE bool
xkb_file_iterator_next(sa_nracanaa xkb_file_iterator *iter,
                       const sa_nracanaa xkb_file_section **section);
