/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن "xkbcommon/xkbcommon.h"

#ضمن "src/darray.h"
#ضمن "src/scanner-utils.h"
#ضمن "src/utils.h"
#ضمن "ast.h"
#ضمن "xkbcomp-priv.h"

XKB_EXPORT_ذاتي ثابت حرف *
xkb_file_type_name(تعداد xkb_file_type type);

XKB_EXPORT_ذاتي ثابت حرف *
xkb_ملايو_گهڻائي_name(تعداد ملايو_گهڻائي type);

XKB_EXPORT_ذاتي ثابت حرف *
xkb_map_flags_string_iter(بنانشان صحيح *اشاريو, تعداد xkb_map_flags flags);

XKB_EXPORT_ذاتي FILE *
xkb_resolve_file(بنية xkb_context *ctx,
                 تعداد xkb_file_type file_type,
                 ثابت حرف *رستو, ثابت حرف *map,
                 حرف *resolved_رستو, سائيز_t resolved_رستو_سائيز,
                 حرف *resolved_map, سائيز_t resolved_map_سائيز);

/** An atomic include entry */
بنية xkb_file_include {
    bool valid:1;
    bool explicit_section:1;
    تعداد ملايو_گهڻائي ملايو;
    /*
     * The following are indices from `xkb_file_section::buffer`
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_سائيز_t رستو;
    darray_سائيز_t file;
    darray_سائيز_t section;
    darray_سائيز_t modifier;
    /* Flags of the included section, only set if valid = true */
    تعداد xkb_map_flags flags;
};

بنية xkb_file_include_گروهه {
    darray_سائيز_t شروعات;
    darray_سائيز_t پڇاڙي;
};

/** A file section: `xkb_{keycodes,compat,geometry,symbols,types} */
بنية xkb_file_section {
    darray_سائيز_t name;
    تعداد xkb_file_type file_type;
    تعداد xkb_map_flags flags;
    darray(بنية xkb_file_include_گروهه) include_groups;
    darray(بنية xkb_file_include) includes;
    /**
     * Char array to store all the strings
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_char بفر;
};

XKB_EXPORT_ذاتي فراغ
xkb_file_section_init(بنية xkb_file_section *section);

XKB_EXPORT_ذاتي فراغ
xkb_file_section_free(بنية xkb_file_section *section);

تعداد xkb_file_iterator_flags {
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
XKB_EXPORT_ذاتي bool
xkb_file_section_parse(بنية xkb_context *ctx,
                       تعداد xkb_file_iterator_flags iterator_flags,
                       تعداد xkb_keymap_format format,
                       تعداد xkb_keymap_compile_flags compile_flags,
                       بنانشان صحيح include_اونهائي,
                       ثابت حرف *رستو, ثابت حرف *map,
                       بنية xkb_file_section *section);

XKB_EXPORT_ذاتي ثابت حرف *
xkb_file_section_get_string(ثابت بنية xkb_file_section *section,
                            darray_سائيز_t idx);

/** Iterator over the sections of a keymap file */
بنية xkb_file_iterator {
    بنية xkb_context *ctx;
    /** File to analyze */
    ثابت حرف *رستو;
    /** Section to analyze; if NULL then iter over all the sections */
    ثابت حرف *map;
    /** Pending XKB file */
    XkbFile *pending_xkb_file;
    /** Pending component of the XKB file */
    XkbFile *pending_section;

    /** Scanner of the keymap file */
    بنية scanner scanner;
    /** Current section */
    بنية xkb_file_section section;

    /** Flags */
    تعداد xkb_file_iterator_flags flags;
    /** File type to check/filter */
    تعداد xkb_file_type type;

    /** Parser configuration */
    بنية parser_keymap_config parser_config;

    /** Indicate whether the iterator reached the end */
    bool ختم;
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
XKB_EXPORT_ذاتي بنية xkb_file_iterator *
xkb_file_iterator_new_from_بفر(بنية xkb_context *ctx,
                                  تعداد xkb_file_iterator_flags iterator_flags,
                                  تعداد xkb_keymap_format format,
                                  تعداد xkb_keymap_compile_flags compile_flags,
                                  ثابت حرف *رستو, ثابت حرف *map,
                                  تعداد xkb_file_type file_type,
                                  ثابت حرف *string, سائيز_t ڊيگهائي);

XKB_EXPORT_ذاتي فراغ
xkb_file_iterator_free(بنية xkb_file_iterator *iter);

XKB_EXPORT_ذاتي bool
xkb_file_iterator_ايندڙ(بنية xkb_file_iterator *iter,
                       ثابت بنية xkb_file_section **section);
