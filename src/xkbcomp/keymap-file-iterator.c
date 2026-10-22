/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل <limits.h>
#شامل <string.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "context.h"
#شامل "darray.h"
#شامل "include.h"
#شامل "keymap-formats.h"
#شامل "keymap-file-iterator.h"
#شامل "messages-codes.h"
#شامل "utils.h"
#شامل "util-paths.h"
#شامل "xkbcomp-priv.h"
#شامل "xkbcomp/ast.h"

ثابت حرف *
xkb_file_type_name(شمار xkb_file_type type)
{
    اگر (type > FILE_TYPE_KEYMAP)
        واپس "unknown";

    ساکن ثابت حرف *xkb_file_type_لڑیاں[FILE_TYPE_عدد_ENTRIES] = {
        [FILE_TYPE_KEYCODES] = "keycodes",
        [FILE_TYPE_TYPES] = "types",
        [FILE_TYPE_COMPAT] = "compatibility",
        [FILE_TYPE_SYMBOLS] = "symbols",
        [FILE_TYPE_GEOMETRY] = "geometry",
        [FILE_TYPE_KEYMAP] = "keymap",
    };
    واپس xkb_file_type_لڑیاں[type];
}

ثابت حرف *
xkb_ملاؤ_منوال_name(شمار ملاؤ_منوال ملاؤ)
{
    اگر (ملاؤ >= ملاؤ_منوال_عدد_ENTRIES)
        واپس "unknown";

    ساکن ثابت حرف *ملاؤ_منوال_لڑیاں[ملاؤ_منوال_عدد_ENTRIES] = {
        [ملاؤ_DEFAULT] = "default",
        [ملاؤ_AUGMENT] = "augment",
        [ملاؤ_OVERRIDE] = "override",
        [ملاؤ_REPLACE] = "replace",
    };
    واپس ملاؤ_منوال_لڑیاں[ملاؤ];
}

/* Stateful lookup of map flags names */
ثابت حرف *
xkb_map_flags_string_iter(غیرعلامتی صحیح *اشاریہ, شمار xkb_map_flags flags)
{
    اگر (!flags)
        واپس تہی;

    ساکن ثابت ساخت {
        شمار xkb_map_flags flag;
        ثابت حرف *name;
    } نام[] = {
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

    جب (*اشاریہ < ARRAY_سائز(نام)) {
        اگر (flags & نام[*اشاریہ].flag)
            واپس نام[(*اشاریہ)++].name;
        (*اشاریہ)++;
    }

    واپس تہی;
};

FILE *
xkb_resolve_file(ساخت xkb_context *ctx,
                 شمار xkb_file_type file_type,
                 ثابت حرف *راستہ, ثابت حرف *map,
                 حرف *resolved_راستہ, سائز_t resolved_راستہ_سائز,
                 حرف *resolved_map, سائز_t resolved_map_سائز)
{
    غیرعلامتی صحیح انحراف = 0;
    FILE *file = تہی; /* Exact match */
    FILE *candidate = تہی; /* Weak match */
    ثابت سائز_t راستہ_len = لڑی_لمبائی(راستہ);

    ثابت bool absolute_راستہ = is_absolute_راستہ(راستہ);
    اگر (absolute_راستہ) {
        /* Absolute path: no need for lookup in XKB paths */
        file = fopen(راستہ, "rb");
    } ورنہ {
        /* Relative path: lookup the first XKB path */
        file = ڈھونڈو_File_اندر_Xkb_راستہ(ctx, "(unknown)", راستہ, راستہ_len, file_type,
                                 resolved_راستہ, resolved_راستہ_سائز, &انحراف,
                                 true);
    }

    ثابت شمار xkb_keymap_format format = DEFAULT_داخلہ_KEYMAP_FORMAT;
    ثابت ساخت parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    جب (file) {
        XkbFile * ثابت xkb_file = XkbParseFile(ctx, &config, file, راستہ, map);
        اگر (xkb_file) {
            اگر (file_type < FILE_TYPE_عدد_ENTRIES &&
                xkb_file->file_type != file_type) {
                log_err(ctx, XKB_LOG_پیغام_NO_ID,
                        "File of wrong type (expected %s, got %s); "
                        "file \"%s\" ignored\n",
                        xkb_file_type_مقصد_string(file_type),
                        xkb_file_type_مقصد_string(xkb_file->file_type),
                        (absolute_راستہ ? راستہ : resolved_راستہ));
                جاؤ invalid_file;
            } ورنہ اگر (map || (xkb_file->flags && MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops in this iteration.
                 */
                اگر (!strcpy_محفوظ(resolved_map, resolved_map_سائز,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    جاؤ error;
                }
            } ورنہ اگر (!candidate) {
                /*
                 * Weak match: looking for an explicit default map, but found
                 * only the first *implicit* default map (e.g. first map of the
                 * file). Use as fallback, but keep looking for an exact match.
                 */
                candidate = file;
                اگر (!strcpy_محفوظ(resolved_map, resolved_map_سائز,
                                 (xkb_file->name ? xkb_file->name : ""))) {
                    FreeXkbFile(xkb_file);
                    جاؤ error;
                }
            } ورنہ {
                /*
                 * Weak match, but we already have a previous candidate.
                 * Keep looking for an exact match.
                 */
                جاؤ invalid_file;
            }
        } ورنہ {
invalid_file:
            fclose(file);
            file = تہی;
        }

        FreeXkbFile(xkb_file);

        اگر (file != تہی || absolute_راستہ) {
            /* Exact match or absolute path with no further file to search */
            توڑو;
        }

        انحراف++;
        file = ڈھونڈو_File_اندر_Xkb_راستہ(ctx, "(unknown)",
                                 راستہ, راستہ_len, file_type,
                                 resolved_راستہ, resolved_راستہ_سائز,
                                 &انحراف, true);
    }

    اگر (!file) {
        /* No exact match: use weak match, if any */
        file = candidate;
    } ورنہ اگر (candidate) {
        /* Found exact match: discard weak match, if any */
        fclose(candidate);
    }

    اگر (absolute_راستہ && file &&
        !strcpy_محفوظ(resolved_راستہ, resolved_راستہ_سائز, راستہ)) {
        جاؤ error;
    }

    واپس file;

error:
    log_err(ctx, XKB_ERROR_INSUFFICIENT_بفر_سائز,
            "Cannot copy resolved path or section\n");
    fclose(file);
    واپس تہی;
}

خالی
xkb_file_section_init(ساخت xkb_file_section *section)
{
    darray_init(section->include_groups);
    darray_init(section->includes);
    darray_init(section->بفر);
    darray_append(section->بفر, '\0');
}

ساکن خالی
xkb_file_section_reset(ساخت xkb_file_section *section)
{
    darray_سائز(section->include_groups) = 0;
    darray_سائز(section->includes) = 0;
    darray_سائز(section->بفر) = 1; /* keep the dummy string */
}

خالی
xkb_file_section_free(ساخت xkb_file_section *section)
{
    اگر (!section)
        واپس;

    darray_free(section->include_groups);
    darray_free(section->includes);
    darray_free(section->بفر);
}

ساکن bool
xkb_file_section_سیٹ_meta_ڈیٹا(ساخت xkb_context *ctx,
                               ساخت xkb_file_section *section,
                               ثابت XkbFile *xkb_file)
{
    section->file_type = xkb_file->file_type;
    section->flags = xkb_file->flags;
    اگر (xkb_file->name) {
        darray_سائز_t idx = darray_سائز(section->بفر);
        darray_append_string0(section->بفر, xkb_file->name);
        section->name = idx;
    } ورنہ {
        section->name = 0;
    }
    واپس true;
}

/** Process a list of include statements */
ساکن bool
xkb_file_section_append_includes(ساخت xkb_context *ctx,
                                 شمار xkb_file_iterator_flags flags,
                                 ثابت ساخت parser_keymap_config *config,
                                 ثابت حرف *section_راستہ,
                                 ساخت xkb_file_section *section,
                                 شمار xkb_file_type file_type,
                                 IncludeStmt * include)
{
    ساخت xkb_file_include_گروہ *گروہ = تہی;
    /* Note: this will flatten statements such as `include "pc+de"` */
    برائے (IncludeStmt *stmt = include; stmt; stmt = stmt->اگلا_incl) {
        حرف buf[PATH_MAX];
        /* Parse the included file to check the include validity */
        XkbFile *xkb_file = ProcessIncludeFile(ctx, config, stmt, file_type,
                                               buf, حجم(buf));
        ثابت bool valid = (xkb_file != تہی);
        اگر (valid || !(flags & XKB_FILE_ITERATOR_FAIL_ON_INCLUDE_ERROR)) {
            /* Collect the strings of the statement properties */

            ثابت darray_سائز_t راستہ = darray_سائز(section->بفر);
            darray_append_string0(section->بفر, buf);

            ثابت darray_سائز_t file = darray_سائز(section->بفر);
            darray_append_string0(section->بفر, stmt->file);

            ثابت darray_سائز_t section_name =
                (stmt->map != تہی || (valid && xkb_file->name))
                    ? darray_سائز(section->بفر)
                    : 0;
            اگر (section_name) {
                darray_append_string0(section->بفر,
                                      (stmt->map) ? stmt->map : xkb_file->name);
            }

            ثابت darray_سائز_t modifier = (stmt->modifier)
                ? darray_سائز(section->بفر)
                : 0;
            اگر (modifier) {
                darray_append_string0(section->بفر, stmt->modifier);
            }

            ثابت شمار xkb_map_flags section_flags = (valid)
                ? xkb_file->flags
                : 0;

            /* Create and append the include statement */
            ثابت ساخت xkb_file_include inc = {
                .valid = valid,
                .explicit_section = (stmt->map != تہی),
                .ملاؤ = stmt->ملاؤ,
                .راستہ = راستہ,
                .file = file,
                .section = section_name,
                .modifier = modifier,
                .flags = section_flags
            };
            ثابت darray_سائز_t idx = darray_سائز(section->includes);
            darray_append(section->includes, inc);

            /* Update include group */
            اگر (گروہ == تہی) {
                ثابت darray_سائز_t گروہ_idx =
                    darray_سائز(section->include_groups);
                darray_append(
                    section->include_groups,
                    (ساخت xkb_file_include_گروہ) {.آغاز = idx, .انتہا = idx}
                );
                گروہ = &darray_شے(section->include_groups, گروہ_idx);
            } ورنہ {
                گروہ->انتہا = idx;
            }

            FreeXkbFile(xkb_file);
        } ورنہ {
            ثابت حرف * ثابت name =
                xkb_file_section_get_string(section, section->name);
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_ملا,
                    "%s include failure in: %s%s%s%s\n",
                    xkb_file_type_name(file_type), section_راستہ,
                    (section->name ? " (section: \"": ""), name,
                    (section->name ? "\")": ""));
            FreeXkbFile(xkb_file);
            واپس false;
        }
    };
    واپس true;
}

/* Process the AST of a section */
ساکن bool
xkb_file_section_process(ساخت xkb_context *ctx,
                         شمار xkb_file_iterator_flags flags,
                         ثابت ساخت parser_keymap_config *config,
                         ثابت حرف *راستہ,
                         ساخت xkb_file_section *section,
                         ثابت XkbFile *xkb_file)
{
    bool ok = true;
    برائے (ParseCommon *stmt = xkb_file->defs; stmt; stmt = stmt->اگلا) {
        اگر (stmt->type == STMT_INCLUDE) {
            ok = xkb_file_section_append_includes(ctx, flags, config, راستہ,
                                                  section, xkb_file->file_type,
                                                  (IncludeStmt *) stmt);
            اگر (!ok)
                توڑو;
        }
    }
    واپس ok;
}

bool
xkb_file_section_parse(ساخت xkb_context *ctx,
                       شمار xkb_file_iterator_flags iterator_flags,
                       شمار xkb_keymap_format format,
                       شمار xkb_keymap_compile_flags compile_flags,
                       غیرعلامتی صحیح include_گہرائی,
                       ثابت حرف *راستہ, ثابت حرف *map,
                       ساخت xkb_file_section *section)
{
    اگر (Exceeds_Include_Max_گہرائی(ctx, include_گہرائی))
        واپس false;

    FILE *file = fopen(راستہ, "rb");
    اگر (!file) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Cannot open file: %s\n", راستہ);
        واپس false;
    }

    ثابت ساخت parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    XkbFile *xkb_file = XkbParseFile(ctx, &config, file, راستہ, map);
    fclose(file);
    اگر (!xkb_file) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Cannot parse map \"%s\" in file: %s\n",
                (map ? map : "(no map)"), راستہ);
        واپس false;
    }

    xkb_file_section_reset(section);
    ثابت bool no_includes = iterator_flags & XKB_FILE_ITERATOR_NO_INCLUDES;
    ثابت bool ok = (
        xkb_file_section_سیٹ_meta_ڈیٹا(ctx, section, xkb_file) &&
        (no_includes ||
         xkb_file_section_process(ctx, iterator_flags, &config, راستہ, section,
                                  xkb_file))
    );
    FreeXkbFile(xkb_file);
    واپس ok;
}

ساخت xkb_file_iterator *
xkb_file_iterator_new_from_بفر(ساخت xkb_context *ctx,
                                  شمار xkb_file_iterator_flags iterator_flags,
                                  شمار xkb_keymap_format format,
                                  شمار xkb_keymap_compile_flags compile_flags,
                                  ثابت حرف *راستہ, ثابت حرف *map,
                                  شمار xkb_file_type file_type,
                                  ثابت حرف *string, سائز_t لمبائی)
{
    ساخت xkb_file_iterator * ثابت iter = صفر_حافظہ_دو(1, حجم(*iter));
    اگر (!iter) {
        log_err(ctx, XKB_ERROR_تخصیص_FAILURE_2,
                "Cannot allocate file iterator\n");
        واپس تہی;
    }

    iter->flags = iterator_flags;
    iter->ctx = ctx;
    iter->راستہ = راستہ;
    iter->map = map;
    iter->type = file_type;
    iter->parser_config = (ساخت parser_keymap_config) {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };
    xkb_file_section_init(&iter->section);

    اگر (!XkbParseStringInit(ctx, &iter->scanner, string, لمبائی, راستہ, تہی)) {
        xkb_file_iterator_free(iter);
        واپس تہی;
    }

    واپس iter;
}

خالی
xkb_file_iterator_free(ساخت xkb_file_iterator *iter)
{
    اگر (!iter)
        واپس;

    xkb_file_section_free(&iter->section);
    FreeXkbFile(iter->pending_xkb_file);
    آزاد(iter);
}

bool
xkb_file_iterator_اگلا(ساخت xkb_file_iterator *iter,
                       ثابت ساخت xkb_file_section **section)
{
    اگر (iter->ختم_شدہ) {
        *section = تہی;
        واپس true;
    }

اگلا:
    { /* C11 compatibility: label cannot be followed by a declaration */ }
    /* Parse next section in the file */
    XkbFile *xkb_file = تہی;

    اگر (iter->pending_xkb_file) {
        /* We are parsing the components of a keymap */
        اگر (iter->pending_section) {
            /* Parse next component */
            xkb_file = iter->pending_section;
            جاؤ parse_components;
        } ورنہ {
            /* No more components, try next keymap */
            FreeXkbFile(iter->pending_xkb_file);
            iter->pending_xkb_file = تہی;
        }
    }

    اگر (!Xkb_Parse_String_اگلا(iter->ctx, &iter->parser_config, &iter->scanner,
                            iter->map, &xkb_file)) {
        log_err(iter->ctx, XKB_LOG_پیغام_NO_ID,
                "Error while parsing section in file: %s\n", iter->راستہ);
        جاؤ error;
    } ورنہ اگر (!xkb_file) {
        /* No more sections */
        iter->ختم_شدہ = true;
        *section = تہی;
        واپس true;
    }

parse_components:
    /* Reset section */
    xkb_file_section_reset(&iter->section);

    /* Meta data */
    اگر (!xkb_file_section_سیٹ_meta_ڈیٹا(iter->ctx, &iter->section, xkb_file))
        جاؤ error;

    /* Return current section */
    *section = &iter->section;

    اگر (xkb_file->file_type == FILE_TYPE_KEYMAP) {
        /*
         * If it’s a keymap, then stop here.
         * Next iteration will process its components.
         */
        iter->pending_xkb_file = xkb_file;
        iter->pending_section = (XkbFile *) xkb_file->defs;
        iter->map = تہی;
        واپس true;
    } ورنہ {
        اگر (iter->type != FILE_TYPE_INVALID &&
            xkb_file->file_type != iter->type) {
            اگر (iter->pending_xkb_file) {
                /* Within a keymap: filter out component */
                iter->pending_section = (XkbFile *) xkb_file->common.اگلا;
                جاؤ اگلا;
            } ورنہ {
                /* Component-specific file: type mismatch */
                log_err(iter->ctx, XKB_LOG_پیغام_NO_ID,
                        "File type mismatch: %s, section: %s\n",
                        iter->راستہ,
                        (xkb_file->name ? xkb_file->name : "(no name)"));
                جاؤ error;
            }
        }
        اگر (iter->map) {
            iter->ختم_شدہ = true;
        }
    }

    /* Collect include statements of current section */
    ثابت bool process_includes = !(iter->flags & XKB_FILE_ITERATOR_NO_INCLUDES);
    اگر (process_includes &&
        !xkb_file_section_process(iter->ctx, iter->flags, &iter->parser_config,
                                  iter->راستہ, &iter->section, xkb_file)) {
        جاؤ error;
    } ورنہ اگر (iter->pending_section) {
        /* Next component */
        iter->pending_section = (XkbFile *) xkb_file->common.اگلا;
    } ورنہ {
        /* No more component, free the file containing the components */
        FreeXkbFile(xkb_file);
    }

    واپس true;

error:
    FreeXkbFile(xkb_file);
    *section = تہی;
    واپس false;
}

/* Lookup a string by its index */
ثابت حرف *
xkb_file_section_get_string(ثابت ساخت xkb_file_section *section, darray_سائز_t idx)
{
    واپس &darray_شے(section->بفر, idx);
}
