/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "src/darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "src/scanner-utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "src/utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ast.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp-priv.h"

XKB_EXPORT_ਨਿੱਜੀ ਅਚਲ ਅੱਖਰ *
xkb_file_type_name(ਗਿਣਤੀ xkb_file_type type);

XKB_EXPORT_ਨਿੱਜੀ ਅਚਲ ਅੱਖਰ *
xkb_ਮਿਲਾਓ_ਬਹੁਲਕ_name(ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ type);

XKB_EXPORT_ਨਿੱਜੀ ਅਚਲ ਅੱਖਰ *
xkb_map_flags_string_iter(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸੂਚਕਾਂਕ, ਗਿਣਤੀ xkb_map_flags flags);

XKB_EXPORT_ਨਿੱਜੀ FILE *
xkb_resolve_file(ਬਣਤਰ xkb_context *ctx,
                 ਗਿਣਤੀ xkb_file_type file_type,
                 ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *map,
                 ਅੱਖਰ *resolved_ਰਸਤਾ, ਮਾਪ_t resolved_ਰਸਤਾ_ਮਾਪ,
                 ਅੱਖਰ *resolved_map, ਮਾਪ_t resolved_map_ਮਾਪ);

/** An atomic include entry */
ਬਣਤਰ xkb_file_include {
    bool valid:1;
    bool explicit_section:1;
    ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ;
    /*
     * The following are indices from `xkb_file_section::buffer`
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_ਮਾਪ_t ਰਸਤਾ;
    darray_ਮਾਪ_t file;
    darray_ਮਾਪ_t section;
    darray_ਮਾਪ_t modifier;
    /* Flags of the included section, only set if valid = true */
    ਗਿਣਤੀ xkb_map_flags flags;
};

ਬਣਤਰ xkb_file_include_ਸਮੂਹ {
    darray_ਮਾਪ_t ਸ਼ੁਰੂ;
    darray_ਮਾਪ_t ਸਿਰਾ;
};

/** A file section: `xkb_{keycodes,compat,geometry,symbols,types} */
ਬਣਤਰ xkb_file_section {
    darray_ਮਾਪ_t name;
    ਗਿਣਤੀ xkb_file_type file_type;
    ਗਿਣਤੀ xkb_map_flags flags;
    darray(ਬਣਤਰ xkb_file_include_ਸਮੂਹ) include_groups;
    darray(ਬਣਤਰ xkb_file_include) includes;
    /**
     * Char array to store all the strings
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_char ਬਫਰ;
};

XKB_EXPORT_ਨਿੱਜੀ ਖਾਲੀ
xkb_file_section_init(ਬਣਤਰ xkb_file_section *section);

XKB_EXPORT_ਨਿੱਜੀ ਖਾਲੀ
xkb_file_section_free(ਬਣਤਰ xkb_file_section *section);

ਗਿਣਤੀ xkb_file_iterator_flags {
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
XKB_EXPORT_ਨਿੱਜੀ bool
xkb_file_section_parse(ਬਣਤਰ xkb_context *ctx,
                       ਗਿਣਤੀ xkb_file_iterator_flags iterator_flags,
                       ਗਿਣਤੀ xkb_keymap_format format,
                       ਗਿਣਤੀ xkb_keymap_compile_flags compile_flags,
                       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ include_ਡੂੰਘਾਈ,
                       ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *map,
                       ਬਣਤਰ xkb_file_section *section);

XKB_EXPORT_ਨਿੱਜੀ ਅਚਲ ਅੱਖਰ *
xkb_file_section_get_string(ਅਚਲ ਬਣਤਰ xkb_file_section *section,
                            darray_ਮਾਪ_t idx);

/** Iterator over the sections of a keymap file */
ਬਣਤਰ xkb_file_iterator {
    ਬਣਤਰ xkb_context *ctx;
    /** File to analyze */
    ਅਚਲ ਅੱਖਰ *ਰਸਤਾ;
    /** Section to analyze; if NULL then iter over all the sections */
    ਅਚਲ ਅੱਖਰ *map;
    /** Pending XKB file */
    XkbFile *pending_xkb_file;
    /** Pending component of the XKB file */
    XkbFile *pending_section;

    /** Scanner of the keymap file */
    ਬਣਤਰ scanner scanner;
    /** Current section */
    ਬਣਤਰ xkb_file_section section;

    /** Flags */
    ਗਿਣਤੀ xkb_file_iterator_flags flags;
    /** File type to check/filter */
    ਗਿਣਤੀ xkb_file_type type;

    /** Parser configuration */
    ਬਣਤਰ parser_keymap_config parser_config;

    /** Indicate whether the iterator reached the end */
    bool ਮੁਕੰਮਲ;
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
XKB_EXPORT_ਨਿੱਜੀ ਬਣਤਰ xkb_file_iterator *
xkb_file_iterator_new_from_ਬਫਰ(ਬਣਤਰ xkb_context *ctx,
                                  ਗਿਣਤੀ xkb_file_iterator_flags iterator_flags,
                                  ਗਿਣਤੀ xkb_keymap_format format,
                                  ਗਿਣਤੀ xkb_keymap_compile_flags compile_flags,
                                  ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *map,
                                  ਗਿਣਤੀ xkb_file_type file_type,
                                  ਅਚਲ ਅੱਖਰ *string, ਮਾਪ_t ਲੰਮਾਈ);

XKB_EXPORT_ਨਿੱਜੀ ਖਾਲੀ
xkb_file_iterator_free(ਬਣਤਰ xkb_file_iterator *iter);

XKB_EXPORT_ਨਿੱਜੀ bool
xkb_file_iterator_ਅਗਲਾ(ਬਣਤਰ xkb_file_iterator *iter,
                       ਅਚਲ ਬਣਤਰ xkb_file_section **section);
