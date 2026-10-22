/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव "xkbcommon/xkbcommon.h"

#अन्तर्भाव "src/darray.h"
#अन्तर्भाव "src/scanner-utils.h"
#अन्तर्भाव "src/utils.h"
#अन्तर्भाव "ast.h"
#अन्तर्भाव "xkbcomp-priv.h"

XKB_EXPORT_वैयक्तिकम् ध्रुव वर्ण *
xkb_file_type_name(गणना xkb_file_type type);

XKB_EXPORT_वैयक्तिकम् ध्रुव वर्ण *
xkb_merge_बहुलकम्_name(गणना merge_बहुलकम् type);

XKB_EXPORT_वैयक्तिकम् ध्रुव वर्ण *
xkb_map_flags_string_iter(अचिह्नित पूर्णाङ्क *सूचकांक, गणना xkb_map_flags flags);

XKB_EXPORT_वैयक्तिकम् FILE *
xkb_resolve_file(संरचना xkb_context *ctx,
                 गणना xkb_file_type file_type,
                 ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *map,
                 वर्ण *resolved_मार्गः, आकार_t resolved_मार्गः_आकार,
                 वर्ण *resolved_map, आकार_t resolved_map_आकार);

/** An atomic include entry */
संरचना xkb_file_include {
    bool valid:1;
    bool explicit_section:1;
    गणना merge_बहुलकम् merge;
    /*
     * The following are indices from `xkb_file_section::buffer`
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_आकार_t मार्गः;
    darray_आकार_t file;
    darray_आकार_t section;
    darray_आकार_t modifier;
    /* Flags of the included section, only set if valid = true */
    गणना xkb_map_flags flags;
};

संरचना xkb_file_include_गणः {
    darray_आकार_t start;
    darray_आकार_t अन्तः;
};

/** A file section: `xkb_{keycodes,compat,geometry,symbols,types} */
संरचना xkb_file_section {
    darray_आकार_t name;
    गणना xkb_file_type file_type;
    गणना xkb_map_flags flags;
    darray(संरचना xkb_file_include_गणः) include_groups;
    darray(संरचना xkb_file_include) includes;
    /**
     * Char array to store all the strings
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_char बफर;
};

XKB_EXPORT_वैयक्तिकम् शून्य
xkb_file_section_init(संरचना xkb_file_section *section);

XKB_EXPORT_वैयक्तिकम् शून्य
xkb_file_section_free(संरचना xkb_file_section *section);

गणना xkb_file_iterator_flags {
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
XKB_EXPORT_वैयक्तिकम् bool
xkb_file_section_parse(संरचना xkb_context *ctx,
                       गणना xkb_file_iterator_flags iterator_flags,
                       गणना xkb_keymap_format format,
                       गणना xkb_keymap_compile_flags compile_flags,
                       अचिह्नित पूर्णाङ्क include_गभीरता,
                       ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *map,
                       संरचना xkb_file_section *section);

XKB_EXPORT_वैयक्तिकम् ध्रुव वर्ण *
xkb_file_section_get_string(ध्रुव संरचना xkb_file_section *section,
                            darray_आकार_t idx);

/** Iterator over the sections of a keymap file */
संरचना xkb_file_iterator {
    संरचना xkb_context *ctx;
    /** File to analyze */
    ध्रुव वर्ण *मार्गः;
    /** Section to analyze; if NULL then iter over all the sections */
    ध्रुव वर्ण *map;
    /** Pending XKB file */
    XkbFile *pending_xkb_file;
    /** Pending component of the XKB file */
    XkbFile *pending_section;

    /** Scanner of the keymap file */
    संरचना scanner scanner;
    /** Current section */
    संरचना xkb_file_section section;

    /** Flags */
    गणना xkb_file_iterator_flags flags;
    /** File type to check/filter */
    गणना xkb_file_type type;

    /** Parser configuration */
    संरचना parser_keymap_config parser_config;

    /** Indicate whether the iterator reached the end */
    bool परिसमाप्तम्;
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
XKB_EXPORT_वैयक्तिकम् संरचना xkb_file_iterator *
xkb_file_iterator_new_from_बफर(संरचना xkb_context *ctx,
                                  गणना xkb_file_iterator_flags iterator_flags,
                                  गणना xkb_keymap_format format,
                                  गणना xkb_keymap_compile_flags compile_flags,
                                  ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *map,
                                  गणना xkb_file_type file_type,
                                  ध्रुव वर्ण *string, आकार_t दीर्घता);

XKB_EXPORT_वैयक्तिकम् शून्य
xkb_file_iterator_free(संरचना xkb_file_iterator *iter);

XKB_EXPORT_वैयक्तिकम् bool
xkb_file_iterator_अग्र(संरचना xkb_file_iterator *iter,
                       ध्रुव संरचना xkb_file_section **section);
