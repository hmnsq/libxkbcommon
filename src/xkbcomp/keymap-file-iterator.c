/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن <limits.h>
#ضمن <string.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "context.h"
#ضمن "darray.h"
#ضمن "include.h"
#ضمن "keymap-formats.h"
#ضمن "keymap-file-iterator.h"
#ضمن "messages-codes.h"
#ضمن "utils.h"
#ضمن "util-paths.h"
#ضمن "xkbcomp-priv.h"
#ضمن "xkbcomp/ast.h"

ثابت حرف *
xkb_file_type_name(تعداد xkb_file_type type)
{
    إذا (type > FILE_TYPE_KEYMAP)
        أرجع "unknown";

    ساكن ثابت حرف *xkb_file_type_مزي_ګانې[FILE_TYPE_عدد_ENTRIES] = {
        [FILE_TYPE_KEYCODES] = "keycodes",
        [FILE_TYPE_TYPES] = "types",
        [FILE_TYPE_COMPAT] = "compatibility",
        [FILE_TYPE_SYMBOLS] = "symbols",
        [FILE_TYPE_GEOMETRY] = "geometry",
        [FILE_TYPE_KEYMAP] = "keymap",
    };
    أرجع xkb_file_type_مزي_ګانې[type];
}

ثابت حرف *
xkb_یوځای_ډېرپېښ_name(تعداد یوځای_ډېرپېښ یوځای)
{
    إذا (یوځای >= یوځای_ډېرپېښ_عدد_ENTRIES)
        أرجع "unknown";

    ساكن ثابت حرف *یوځای_ډېرپېښ_مزي_ګانې[یوځای_ډېرپېښ_عدد_ENTRIES] = {
        [یوځای_DEFAULT] = "default",
        [یوځای_AUGMENT] = "augment",
        [یوځای_OVERRIDE] = "override",
        [یوځای_REPLACE] = "replace",
    };
    أرجع یوځای_ډېرپېښ_مزي_ګانې[یوځای];
}

/* Stateful lookup of map flags names */
ثابت حرف *
xkb_map_flags_string_iter(بینښه صحيح *شاخص, تعداد xkb_map_flags flags)
{
    إذا (!flags)
        أرجع عدم;

    ساكن ثابت بنية {
        تعداد xkb_map_flags flag;
        ثابت حرف *name;
    } نومونه[] = {
        { MAP_IS_DEFAULT, "default" },
        { MAP_IS_PARTIAL, "partial" },
        { MAP_IS_HIDDEN, "hidden" },
        { MAP_HAS_ALPHANUMERIC, "alphanumeric" },
        { MAP_HAS_MODIFIER, "modifiers" },
        { MAP_HAS_KEYPAD, "keypad" },
        { MAP_HAS_FN, "fn" },
        { MAP_IS_ALTGR, "altgr" },
        { MAP_IS_DEPRECATED, "deprecated" },
    };

    طالما (*شاخص < ARRAY_اندازه(نومونه)) {
        إذا (flags & نومونه[*شاخص].flag)
            أرجع نومونه[(*شاخص)++].name;
        (*شاخص)++;
    }

    أرجع عدم;
};

FILE *
xkb_resolve_file(بنية xkb_context *ctx,
                 تعداد xkb_file_type file_type,
                 ثابت حرف *لار, ثابت حرف *map,
                 حرف *resolved_لار, اندازه_t resolved_لار_اندازه,
                 حرف *resolved_map, اندازه_t resolved_map_اندازه)
{
    بینښه صحيح دننه_واټن = 0;
    FILE *file = عدم; /* Exact match */
    FILE *candidate = عدم; /* Weak match */
    ثابت اندازه_t لار_len = مزي_اوږدوالی(لار);

    ثابت bool absolute_لار = is_absolute_لار(لار);
    إذا (absolute_لار) {
        /* Absolute path: no need for lookup in XKB paths */
        file = fopen(لار, "rb");
    } وإلا {
        /* Relative path: lookup the first XKB path */
        file = پیدا_کړه_File_دننه_Xkb_لار(ctx, "(unknown)", لار, لار_len, file_type,
                                 resolved_لار, resolved_لار_اندازه, &دننه_واټن,
                                 true);
    }

    ثابت تعداد xkb_keymap_format format = DEFAULT_ننوت_KEYMAP_FORMAT;
    ثابت بنية parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    طالما (file) {
        XkbFile * ثابت xkb_file = XkbParseFile(ctx, &config, file, لار, map);
        إذا (xkb_file) {
            إذا (file_type < FILE_TYPE_عدد_ENTRIES &&
                xkb_file->file_type != file_type) {
                log_err(ctx, XKB_LOG_پیغام_NO_ID,
                        "File of wrong type (expected %s, got %s); "
                        "file \"%s\" ignored\n",
                        xkb_file_type_منزل_string(file_type),
                        xkb_file_type_منزل_string(xkb_file->file_type),
                        (absolute_لار ? لار : resolved_لار));
                اذهب invalid_file;
            } وإلا إذا (map || (xkb_file->flags && MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops in this iteration.
                 */
                إذا (!strcpy_خوندي(resolved_map, resolved_map_اندازه,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    اذهب error;
                }
            } وإلا إذا (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = file;
                إذا (!strcpy_خوندي(resolved_map, resolved_map_اندازه,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    اذهب error;
                }
            } وإلا {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                اذهب invalid_file;
            }
        } وإلا {
invalid_file:
            fclose(file);
            file = عدم;
        }

        FreeXkbFile(xkb_file);

        إذا (file != عدم || absolute_لار) {
            /* Exact match or absolute path with no further file to search */
            اكسر;
        }

        دننه_واټن++;
        file = پیدا_کړه_File_دننه_Xkb_لار(ctx, "(unknown)",
                                 لار, لار_len, file_type,
                                 resolved_لار, resolved_لار_اندازه,
                                 &دننه_واټن, true);
    }

    إذا (!file) {
        /* No exact match: use weak match, if any */
        file = candidate;
    } وإلا إذا (candidate) {
        /* Found exact match: discard weak match, if any */
        fclose(candidate);
    }

    إذا (absolute_لار && file &&
        !strcpy_خوندي(resolved_لار, resolved_لار_اندازه, لار)) {
        اذهب error;
    }

    أرجع file;

error:
    log_err(ctx, XKB_ERROR_INSUFFICIENT_بفر_اندازه,
            "Cannot copy resolved path or section\n");
    fclose(file);
    أرجع عدم;
}

فراغ
xkb_file_section_init(بنية xkb_file_section *section)
{
    darray_init(section->include_groups);
    darray_init(section->includes);
    darray_init(section->بفر);
    darray_append(section->بفر, '\0');
}

ساكن فراغ
xkb_file_section_reset(بنية xkb_file_section *section)
{
    darray_اندازه(section->include_groups) = 0;
    darray_اندازه(section->includes) = 0;
    darray_اندازه(section->بفر) = 1; /* keep the dummy string */
}

فراغ
xkb_file_section_free(بنية xkb_file_section *section)
{
    إذا (!section)
        أرجع;

    darray_free(section->include_groups);
    darray_free(section->includes);
    darray_free(section->بفر);
}

ساكن bool
xkb_file_section_سېټ_meta_معلومات(بنية xkb_context *ctx,
                               بنية xkb_file_section *section,
                               ثابت XkbFile *xkb_file)
{
    section->file_type = xkb_file->file_type;
    section->flags = xkb_file->flags;
    إذا (xkb_file->name) {
        darray_اندازه_t idx = darray_اندازه(section->بفر);
        darray_append_string0(section->بفر, xkb_file->name);
        section->name = idx;
    } وإلا {
        section->name = 0;
    }
    أرجع true;
}

/** Process a list of include statements */
ساكن bool
xkb_file_section_append_includes(بنية xkb_context *ctx,
                                 تعداد xkb_file_iterator_flags flags,
                                 ثابت بنية parser_keymap_config *config,
                                 ثابت حرف *section_لار,
                                 بنية xkb_file_section *section,
                                 تعداد xkb_file_type file_type,
                                 IncludeStmt * include)
{
    بنية xkb_file_include_ډله *ډله = عدم;
    /* Note: this will flatten statements such as `include "pc+de"` */
    لكل (IncludeStmt *stmt = include; stmt; stmt = stmt->بل_incl) {
        حرف buf[PATH_MAX];
        /* Parse the included file to check the include validity */
        XkbFile *xkb_file = ProcessIncludeFile(ctx, config, stmt, file_type,
                                               buf, حجم(buf));
        ثابت bool valid = (xkb_file != عدم);
        إذا (valid || !(flags & XKB_FILE_ITERATOR_FAIL_ON_INCLUDE_ERROR)) {
            /* Collect the strings of the statement properties */

            ثابت darray_اندازه_t لار = darray_اندازه(section->بفر);
            darray_append_string0(section->بفر, buf);

            ثابت darray_اندازه_t file = darray_اندازه(section->بفر);
            darray_append_string0(section->بفر, stmt->file);

            ثابت darray_اندازه_t section_name =
                (stmt->map != عدم || (valid && xkb_file->name))
                    ? darray_اندازه(section->بفر)
                    : 0;
            إذا (section_name) {
                darray_append_string0(section->بفر,
                                      (stmt->map) ? stmt->map : xkb_file->name);
            }

            ثابت darray_اندازه_t modifier = (stmt->modifier)
                ? darray_اندازه(section->بفر)
                : 0;
            إذا (modifier) {
                darray_append_string0(section->بفر, stmt->modifier);
            }

            ثابت تعداد xkb_map_flags section_flags = (valid)
                ? xkb_file->flags
                : 0;

            /* Create and append the include statement */
            ثابت بنية xkb_file_include inc = {
                .valid = valid,
                .explicit_section = (stmt->map != عدم),
                .یوځای = stmt->یوځای,
                .لار = لار,
                .file = file,
                .section = section_name,
                .modifier = modifier,
                .flags = section_flags
            };
            ثابت darray_اندازه_t idx = darray_اندازه(section->includes);
            darray_append(section->includes, inc);

            /* Update include group */
            إذا (ډله == عدم) {
                ثابت darray_اندازه_t ډله_idx =
                    darray_اندازه(section->include_groups);
                darray_append(
                    section->include_groups,
                    (بنية xkb_file_include_ډله) {.پیل = idx, .پای_ټکی = idx}
                );
                ډله = &darray_توکی(section->include_groups, ډله_idx);
            } وإلا {
                ډله->پای_ټکی = idx;
            }

            FreeXkbFile(xkb_file);
        } وإلا {
            ثابت حرف * ثابت name =
                xkb_file_section_get_string(section, section->name);
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_وموندل,
                    "%s include failure in: %s%s%s%s\n",
                    xkb_file_type_name(file_type), section_لار,
                    (section->name ? " (section: \"": ""), name,
                    (section->name ? "\")": ""));
            FreeXkbFile(xkb_file);
            أرجع false;
        }
    };
    أرجع true;
}

/* Process the AST of a section */
ساكن bool
xkb_file_section_process(بنية xkb_context *ctx,
                         تعداد xkb_file_iterator_flags flags,
                         ثابت بنية parser_keymap_config *config,
                         ثابت حرف *لار,
                         بنية xkb_file_section *section,
                         ثابت XkbFile *xkb_file)
{
    bool ok = true;
    لكل (ParseCommon *stmt = xkb_file->defs; stmt; stmt = stmt->بل) {
        إذا (stmt->type == STMT_INCLUDE) {
            ok = xkb_file_section_append_includes(ctx, flags, config, لار,
                                                  section, xkb_file->file_type,
                                                  (IncludeStmt *) stmt);
            إذا (!ok)
                اكسر;
        }
    }
    أرجع ok;
}

bool
xkb_file_section_parse(بنية xkb_context *ctx,
                       تعداد xkb_file_iterator_flags iterator_flags,
                       تعداد xkb_keymap_format format,
                       تعداد xkb_keymap_compile_flags compile_flags,
                       بینښه صحيح include_ژوروالی,
                       ثابت حرف *لار, ثابت حرف *map,
                       بنية xkb_file_section *section)
{
    إذا (Exceeds_Include_Max_ژوروالی(ctx, include_ژوروالی))
        أرجع false;

    FILE *file = fopen(لار, "rb");
    إذا (!file) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Cannot open file: %s\n", لار);
        أرجع false;
    }

    ثابت بنية parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    XkbFile *xkb_file = XkbParseFile(ctx, &config, file, لار, map);
    fclose(file);
    إذا (!xkb_file) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Cannot parse map \"%s\" in file: %s\n",
                (map ? map : "(no map)"), لار);
        أرجع false;
    }

    xkb_file_section_reset(section);
    ثابت bool no_includes = iterator_flags & XKB_FILE_ITERATOR_NO_INCLUDES;
    ثابت bool ok = (
        xkb_file_section_سېټ_meta_معلومات(ctx, section, xkb_file) &&
        (no_includes ||
         xkb_file_section_process(ctx, iterator_flags, &config, لار, section,
                                  xkb_file))
    );
    FreeXkbFile(xkb_file);
    أرجع ok;
}

بنية xkb_file_iterator *
xkb_file_iterator_new_from_بفر(بنية xkb_context *ctx,
                                  تعداد xkb_file_iterator_flags iterator_flags,
                                  تعداد xkb_keymap_format format,
                                  تعداد xkb_keymap_compile_flags compile_flags,
                                  ثابت حرف *لار, ثابت حرف *map,
                                  تعداد xkb_file_type file_type,
                                  ثابت حرف *string, اندازه_t اوږدتیا)
{
    بنية xkb_file_iterator * ثابت iter = حافظه_صفر_ورکړه(1, حجم(*iter));
    إذا (!iter) {
        log_err(ctx, XKB_ERROR_ویش_FAILURE_2,
                "Cannot allocate file iterator\n");
        أرجع عدم;
    }

    iter->flags = iterator_flags;
    iter->ctx = ctx;
    iter->لار = لار;
    iter->map = map;
    iter->type = file_type;
    iter->parser_config = (بنية parser_keymap_config) {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };
    xkb_file_section_init(&iter->section);

    إذا (!XkbParseStringInit(ctx, &iter->scanner, string, اوږدتیا, لار, عدم)) {
        xkb_file_iterator_free(iter);
        أرجع عدم;
    }

    أرجع iter;
}

فراغ
xkb_file_iterator_free(بنية xkb_file_iterator *iter)
{
    إذا (!iter)
        أرجع;

    xkb_file_section_free(&iter->section);
    FreeXkbFile(iter->pending_xkb_file);
    حرر(iter);
}

bool
xkb_file_iterator_بل(بنية xkb_file_iterator *iter,
                       ثابت بنية xkb_file_section **section)
{
    إذا (iter->پای_ته_رسېدلی) {
        *section = عدم;
        أرجع true;
    }

بل:
    { /* C11 compatibility: label cannot be followed by a declaration */ }
    /* Parse next section in the file */
    XkbFile *xkb_file = عدم;

    إذا (iter->pending_xkb_file) {
        /* We are parsing the components of a keymap */
        إذا (iter->pending_section) {
            /* Parse next component */
            xkb_file = iter->pending_section;
            اذهب parse_components;
        } وإلا {
            /* No more components, try next keymap */
            FreeXkbFile(iter->pending_xkb_file);
            iter->pending_xkb_file = عدم;
        }
    }

    إذا (!Xkb_Parse_String_بل(iter->ctx, &iter->parser_config, &iter->scanner,
                            iter->map, &xkb_file)) {
        log_err(iter->ctx, XKB_LOG_پیغام_NO_ID,
                "Error while parsing section in file: %s\n", iter->لار);
        اذهب error;
    } وإلا إذا (!xkb_file) {
        /* No more sections */
        iter->پای_ته_رسېدلی = true;
        *section = عدم;
        أرجع true;
    }

parse_components:
    /* Reset section */
    xkb_file_section_reset(&iter->section);

    /* Meta data */
    إذا (!xkb_file_section_سېټ_meta_معلومات(iter->ctx, &iter->section, xkb_file))
        اذهب error;

    /* Return current section */
    *section = &iter->section;

    إذا (xkb_file->file_type == FILE_TYPE_KEYMAP) {
        /*
         * If it’s a keymap, then stop here.
         * Next iteration will process its components.
         */
        iter->pending_xkb_file = xkb_file;
        iter->pending_section = (XkbFile *) xkb_file->defs;
        iter->map = عدم;
        أرجع true;
    } وإلا {
        إذا (iter->type != FILE_TYPE_INVALID &&
            xkb_file->file_type != iter->type) {
            إذا (iter->pending_xkb_file) {
                /* Within a keymap: filter out component */
                iter->pending_section = (XkbFile *) xkb_file->common.بل;
                اذهب بل;
            } وإلا {
                /* Component-specific file: type mismatch */
                log_err(iter->ctx, XKB_LOG_پیغام_NO_ID,
                        "File type mismatch: %s, section: %s\n",
                        iter->لار,
                        (xkb_file->name ? xkb_file->name : "(no name)"));
                اذهب error;
            }
        }
        إذا (iter->map) {
            iter->پای_ته_رسېدلی = true;
        }
    }

    /* Collect include statements of current section */
    ثابت bool process_includes = !(iter->flags & XKB_FILE_ITERATOR_NO_INCLUDES);
    إذا (process_includes &&
        !xkb_file_section_process(iter->ctx, iter->flags, &iter->parser_config,
                                  iter->لار, &iter->section, xkb_file)) {
        اذهب error;
    } وإلا إذا (iter->pending_section) {
        /* Next component */
        iter->pending_section = (XkbFile *) xkb_file->common.بل;
    } وإلا {
        /* No more component, free the file containing the components */
        FreeXkbFile(xkb_file);
    }

    أرجع true;

error:
    FreeXkbFile(xkb_file);
    *section = عدم;
    أرجع false;
}

/* Lookup a string by its index */
ثابت حرف *
xkb_file_section_get_string(ثابت بنية xkb_file_section *section, darray_اندازه_t idx)
{
    أرجع &darray_توکی(section->بفر, idx);
}
