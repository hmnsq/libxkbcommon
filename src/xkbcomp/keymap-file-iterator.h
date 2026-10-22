/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ "xkbcommon/xkbcommon.h"

#అన్తర్భావ "src/darray.h"
#అన్తర్భావ "src/scanner-utils.h"
#అన్తర్భావ "src/utils.h"
#అన్తర్భావ "ast.h"
#అన్తర్భావ "xkbcomp-priv.h"

XKB_EXPORT_వ్యక్తిగత స్థిరరాశి అక్షరం *
xkb_file_type_name(గణన xkb_file_type type);

XKB_EXPORT_వ్యక్తిగత స్థిరరాశి అక్షరం *
xkb_విలీనం_బహుళకం_name(గణన విలీనం_బహుళకం type);

XKB_EXPORT_వ్యక్తిగత స్థిరరాశి అక్షరం *
xkb_map_flags_string_iter(గుర్తులేని పూర్ణాంకం *సూచీ, గణన xkb_map_flags flags);

XKB_EXPORT_వ్యక్తిగత FILE *
xkb_resolve_file(నిర్మాణం xkb_context *ctx,
                 గణన xkb_file_type file_type,
                 స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *map,
                 అక్షరం *resolved_మార్గం, కొలత_t resolved_మార్గం_కొలత,
                 అక్షరం *resolved_map, కొలత_t resolved_map_కొలత);

/** An atomic include entry */
నిర్మాణం xkb_file_include {
    bool valid:1;
    bool explicit_section:1;
    గణన విలీనం_బహుళకం విలీనం;
    /*
     * The following are indices from `xkb_file_section::buffer`
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_కొలత_t మార్గం;
    darray_కొలత_t file;
    darray_కొలత_t section;
    darray_కొలత_t modifier;
    /* Flags of the included section, only set if valid = true */
    గణన xkb_map_flags flags;
};

నిర్మాణం xkb_file_include_సమూహం {
    darray_కొలత_t ప్రారంభం;
    darray_కొలత_t చివర;
};

/** A file section: `xkb_{keycodes,compat,geometry,symbols,types} */
నిర్మాణం xkb_file_section {
    darray_కొలత_t name;
    గణన xkb_file_type file_type;
    గణన xkb_map_flags flags;
    darray(నిర్మాణం xkb_file_include_సమూహం) include_groups;
    darray(నిర్మాణం xkb_file_include) includes;
    /**
     * Char array to store all the strings
     *
     * Use `xkb_file_section_get_string()` to retrieve them.
     */
    darray_char బఫరు;
};

XKB_EXPORT_వ్యక్తిగత శూన్య
xkb_file_section_init(నిర్మాణం xkb_file_section *section);

XKB_EXPORT_వ్యక్తిగత శూన్య
xkb_file_section_free(నిర్మాణం xkb_file_section *section);

గణన xkb_file_iterator_flags {
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
XKB_EXPORT_వ్యక్తిగత bool
xkb_file_section_parse(నిర్మాణం xkb_context *ctx,
                       గణన xkb_file_iterator_flags iterator_flags,
                       గణన xkb_keymap_format format,
                       గణన xkb_keymap_compile_flags compile_flags,
                       గుర్తులేని పూర్ణాంకం include_లోతు,
                       స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *map,
                       నిర్మాణం xkb_file_section *section);

XKB_EXPORT_వ్యక్తిగత స్థిరరాశి అక్షరం *
xkb_file_section_get_string(స్థిరరాశి నిర్మాణం xkb_file_section *section,
                            darray_కొలత_t idx);

/** Iterator over the sections of a keymap file */
నిర్మాణం xkb_file_iterator {
    నిర్మాణం xkb_context *ctx;
    /** File to analyze */
    స్థిరరాశి అక్షరం *మార్గం;
    /** Section to analyze; if NULL then iter over all the sections */
    స్థిరరాశి అక్షరం *map;
    /** Pending XKB file */
    XkbFile *pending_xkb_file;
    /** Pending component of the XKB file */
    XkbFile *pending_section;

    /** Scanner of the keymap file */
    నిర్మాణం scanner scanner;
    /** Current section */
    నిర్మాణం xkb_file_section section;

    /** Flags */
    గణన xkb_file_iterator_flags flags;
    /** File type to check/filter */
    గణన xkb_file_type type;

    /** Parser configuration */
    నిర్మాణం parser_keymap_config parser_config;

    /** Indicate whether the iterator reached the end */
    bool ముగిసిన;
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
XKB_EXPORT_వ్యక్తిగత నిర్మాణం xkb_file_iterator *
xkb_file_iterator_new_from_బఫరు(నిర్మాణం xkb_context *ctx,
                                  గణన xkb_file_iterator_flags iterator_flags,
                                  గణన xkb_keymap_format format,
                                  గణన xkb_keymap_compile_flags compile_flags,
                                  స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *map,
                                  గణన xkb_file_type file_type,
                                  స్థిరరాశి అక్షరం *string, కొలత_t నిడివి);

XKB_EXPORT_వ్యక్తిగత శూన్య
xkb_file_iterator_free(నిర్మాణం xkb_file_iterator *iter);

XKB_EXPORT_వ్యక్తిగత bool
xkb_file_iterator_తదుపరి(నిర్మాణం xkb_file_iterator *iter,
                       స్థిరరాశి నిర్మాణం xkb_file_section **section);
