/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل "xkbcommon/xkbcommon.h"

#شامل "src/darray.h"
#شامل "src/scanner-utils.h"
#شامل "src/utils.h"
#شامل "ast.h"
#شامل "xkbcomp-priv.h"

XKB_EXPORT_نجی ثابت حرف *
xkb_file_type_name(شمار xkb_file_type type);

XKB_EXPORT_نجی ثابت حرف *
xkb_ملاؤ_منوال_name(شمار ملاؤ_منوال type);

XKB_EXPORT_نجی ثابت حرف *
xkb_map_flags_string_iter(غیرعلامتی صحیح *اشاریہ, شمار xkb_map_flags flags);

XKB_EXPORT_نجی FILE *
xkb_resolve_file(ساخت xkb_context *ctx,
                 شمار xkb_file_type file_type,
                 ثابت حرف *راستہ, ثابت حرف *map,
                 حرف *resolved_راستہ, سائز_t resolved_راستہ_سائز,
                 حرف *resolved_map, سائز_t resolved_map_سائز);

/** An atomic include entry */
ساخت xkb_file_include {
    bool valid:1;
    bool explicit_section:1;
    شمار ملاؤ_منوال ملاؤ;
    /*
     * The following are indices from `xkb_file_section::buffer`
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_سائز_t راستہ;
    darray_سائز_t file;
    darray_سائز_t section;
    darray_سائز_t modifier;
    /* Flags of the included section, only set if valid = true */
    شمار xkb_map_flags flags;
};

ساخت xkb_file_include_گروہ {
    darray_سائز_t آغاز;
    darray_سائز_t انتہا;
};

/** A file section: `xkb_{keycodes,compat,geometry,symbols,types} */
ساخت xkb_file_section {
    darray_سائز_t name;
    شمار xkb_file_type file_type;
    شمار xkb_map_flags flags;
    darray(ساخت xkb_file_include_گروہ) include_groups;
    darray(ساخت xkb_file_include) includes;
    /**
     * Char array to store all the strings
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_char بفر;
};

XKB_EXPORT_نجی خالی
xkb_file_section_init(ساخت xkb_file_section *section);

XKB_EXPORT_نجی خالی
xkb_file_section_free(ساخت xkb_file_section *section);

شمار xkb_file_iterator_flags {
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
XKB_EXPORT_نجی bool
xkb_file_section_parse(ساخت xkb_context *ctx,
                       شمار xkb_file_iterator_flags iterator_flags,
                       شمار xkb_keymap_format format,
                       شمار xkb_keymap_compile_flags compile_flags,
                       غیرعلامتی صحیح include_گہرائی,
                       ثابت حرف *راستہ, ثابت حرف *map,
                       ساخت xkb_file_section *section);

XKB_EXPORT_نجی ثابت حرف *
xkb_file_section_get_string(ثابت ساخت xkb_file_section *section,
                            darray_سائز_t idx);

/** Iterator over the sections of a keymap file */
ساخت xkb_file_iterator {
    ساخت xkb_context *ctx;
    /** File to analyze */
    ثابت حرف *راستہ;
    /** Section to analyze; if NULL then iter over all the sections */
    ثابت حرف *map;
    /** Pending XKB file */
    XkbFile *pending_xkb_file;
    /** Pending component of the XKB file */
    XkbFile *pending_section;

    /** Scanner of the keymap file */
    ساخت scanner scanner;
    /** Current section */
    ساخت xkb_file_section section;

    /** Flags */
    شمار xkb_file_iterator_flags flags;
    /** File type to check/filter */
    شمار xkb_file_type type;

    /** Parser configuration */
    ساخت parser_keymap_config parser_config;

    /** Indicate whether the iterator reached the end */
    bool ختم_شدہ;
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
XKB_EXPORT_نجی ساخت xkb_file_iterator *
xkb_file_iterator_new_from_بفر(ساخت xkb_context *ctx,
                                  شمار xkb_file_iterator_flags iterator_flags,
                                  شمار xkb_keymap_format format,
                                  شمار xkb_keymap_compile_flags compile_flags,
                                  ثابت حرف *راستہ, ثابت حرف *map,
                                  شمار xkb_file_type file_type,
                                  ثابت حرف *string, سائز_t لمبائی);

XKB_EXPORT_نجی خالی
xkb_file_iterator_free(ساخت xkb_file_iterator *iter);

XKB_EXPORT_نجی bool
xkb_file_iterator_اگلا(ساخت xkb_file_iterator *iter,
                       ثابت ساخت xkb_file_section **section);
