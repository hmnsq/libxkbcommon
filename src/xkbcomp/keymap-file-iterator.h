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

XKB_EXPORT_ಖಾಸಗಿ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_file_type_name(ಎಣಿಕೆ xkb_file_type type);

XKB_EXPORT_ಖಾಸಗಿ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_ವಿಲೀನ_ಬಹುಲಕ_name(ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ type);

XKB_EXPORT_ಖಾಸಗಿ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_map_flags_string_iter(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *ಸೂಚ್ಯಂಕ, ಎಣಿಕೆ xkb_map_flags flags);

XKB_EXPORT_ಖಾಸಗಿ FILE *
xkb_resolve_file(ರಚನೆ xkb_context *ctx,
                 ಎಣಿಕೆ xkb_file_type file_type,
                 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *map,
                 ಅಕ್ಷರ *resolved_ಮಾರ್ಗ, ಗಾತ್ರ_t resolved_ಮಾರ್ಗ_ಗಾತ್ರ,
                 ಅಕ್ಷರ *resolved_map, ಗಾತ್ರ_t resolved_map_ಗಾತ್ರ);

/** An atomic include entry */
ರಚನೆ xkb_file_include {
    bool valid:1;
    bool explicit_section:1;
    ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ;
    /*
     * The following are indices from `xkb_file_section::buffer`
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_ಗಾತ್ರ_t ಮಾರ್ಗ;
    darray_ಗಾತ್ರ_t file;
    darray_ಗಾತ್ರ_t section;
    darray_ಗಾತ್ರ_t modifier;
    /* Flags of the included section, only set if valid = true */
    ಎಣಿಕೆ xkb_map_flags flags;
};

ರಚನೆ xkb_file_include_ಗುಂಪು {
    darray_ಗಾತ್ರ_t ಆರಂಭ;
    darray_ಗಾತ್ರ_t ಕೊನೆ;
};

/** A file section: `xkb_{keycodes,compat,geometry,symbols,types} */
ರಚನೆ xkb_file_section {
    darray_ಗಾತ್ರ_t name;
    ಎಣಿಕೆ xkb_file_type file_type;
    ಎಣಿಕೆ xkb_map_flags flags;
    darray(ರಚನೆ xkb_file_include_ಗುಂಪು) include_groups;
    darray(ರಚನೆ xkb_file_include) includes;
    /**
     * Char array to store all the strings
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_char ಬಫರ್;
};

XKB_EXPORT_ಖಾಸಗಿ ಶೂನ್ಯ
xkb_file_section_init(ರಚನೆ xkb_file_section *section);

XKB_EXPORT_ಖಾಸಗಿ ಶೂನ್ಯ
xkb_file_section_free(ರಚನೆ xkb_file_section *section);

ಎಣಿಕೆ xkb_file_iterator_flags {
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
XKB_EXPORT_ಖಾಸಗಿ bool
xkb_file_section_parse(ರಚನೆ xkb_context *ctx,
                       ಎಣಿಕೆ xkb_file_iterator_flags iterator_flags,
                       ಎಣಿಕೆ xkb_keymap_format format,
                       ಎಣಿಕೆ xkb_keymap_compile_flags compile_flags,
                       ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ include_ಆಳ,
                       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *map,
                       ರಚನೆ xkb_file_section *section);

XKB_EXPORT_ಖಾಸಗಿ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_file_section_get_string(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_file_section *section,
                            darray_ಗಾತ್ರ_t idx);

/** Iterator over the sections of a keymap file */
ರಚನೆ xkb_file_iterator {
    ರಚನೆ xkb_context *ctx;
    /** File to analyze */
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ;
    /** Section to analyze; if NULL then iter over all the sections */
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *map;
    /** Pending XKB file */
    XkbFile *pending_xkb_file;
    /** Pending component of the XKB file */
    XkbFile *pending_section;

    /** Scanner of the keymap file */
    ರಚನೆ scanner scanner;
    /** Current section */
    ರಚನೆ xkb_file_section section;

    /** Flags */
    ಎಣಿಕೆ xkb_file_iterator_flags flags;
    /** File type to check/filter */
    ಎಣಿಕೆ xkb_file_type type;

    /** Parser configuration */
    ರಚನೆ parser_keymap_config parser_config;

    /** Indicate whether the iterator reached the end */
    bool ಪೂರ್ಣಗೊಂಡ;
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
XKB_EXPORT_ಖಾಸಗಿ ರಚನೆ xkb_file_iterator *
xkb_file_iterator_new_from_ಬಫರ್(ರಚನೆ xkb_context *ctx,
                                  ಎಣಿಕೆ xkb_file_iterator_flags iterator_flags,
                                  ಎಣಿಕೆ xkb_keymap_format format,
                                  ಎಣಿಕೆ xkb_keymap_compile_flags compile_flags,
                                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *map,
                                  ಎಣಿಕೆ xkb_file_type file_type,
                                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string, ಗಾತ್ರ_t ಉದ್ದಳತೆ);

XKB_EXPORT_ಖಾಸಗಿ ಶೂನ್ಯ
xkb_file_iterator_free(ರಚನೆ xkb_file_iterator *iter);

XKB_EXPORT_ಖಾಸಗಿ bool
xkb_file_iterator_ಮುಂದಿನ(ರಚನೆ xkb_file_iterator *iter,
                       ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_file_section **section);
