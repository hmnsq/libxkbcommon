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

    ساكن ثابت حرف *xkb_file_type_لڑیاں[FILE_TYPE_عدد_ENTRIES] = {
        [FILE_TYPE_KEYCODES] = "keycodes",
        [FILE_TYPE_TYPES] = "types",
        [FILE_TYPE_COMPAT] = "compatibility",
        [FILE_TYPE_SYMBOLS] = "symbols",
        [FILE_TYPE_GEOMETRY] = "geometry",
        [FILE_TYPE_KEYMAP] = "keymap",
    };
    أرجع xkb_file_type_لڑیاں[type];
}

ثابت حرف *
xkb_ملاؤ_منوال_name(تعداد ملاؤ_منوال ملاؤ)
{
    إذا (ملاؤ >= ملاؤ_منوال_عدد_ENTRIES)
        أرجع "unknown";

    ساكن ثابت حرف *ملاؤ_منوال_لڑیاں[ملاؤ_منوال_عدد_ENTRIES] = {
        [ملاؤ_DEFAULT] = "default",
        [ملاؤ_AUGMENT] = "augment",
        [ملاؤ_OVERRIDE] = "override",
        [ملاؤ_REPLACE] = "replace",
    };
    أرجع ملاؤ_منوال_لڑیاں[ملاؤ];
}

/* Stateful lookup of map flags names */
ثابت حرف *
xkb_map_flags_string_iter(بنانشانی صحيح *اشاریہ, تعداد xkb_map_flags flags)
{
    إذا (!flags)
        أرجع عدم;

    ساكن ثابت بنية {
        تعداد xkb_map_flags flag;
        ثابت حرف *name;
    } ناں[] = {
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

    طالما (*اشاریہ < ARRAY_ناپ(ناں)) {
        إذا (flags & ناں[*اشاریہ].flag)
            أرجع ناں[(*اشاریہ)++].name;
        (*اشاریہ)++;
    }

    أرجع عدم;
};

FILE *
xkb_resolve_file(بنية xkb_context *ctx,
                 تعداد xkb_file_type file_type,
                 ثابت حرف *رستہ, ثابت حرف *map,
                 حرف *resolved_رستہ, ناپ_t resolved_رستہ_ناپ,
                 حرف *resolved_map, ناپ_t resolved_map_ناپ)
{
    بنانشانی صحيح وتھ = 0;
    FILE *file = عدم; /* Exact match */
    FILE *candidate = عدم; /* Weak match */
    ثابت ناپ_t رستہ_len = لڑی_لمبائی(رستہ);

    ثابت bool absolute_رستہ = is_absolute_رستہ(رستہ);
    إذا (absolute_رستہ) {
        /* Absolute path: no need for lookup in XKB paths */
        file = fopen(رستہ, "rb");
    } وإلا {
        /* Relative path: lookup the first XKB path */
        file = لبھو_File_وچ_Xkb_رستہ(ctx, "(unknown)", رستہ, رستہ_len, file_type,
                                 resolved_رستہ, resolved_رستہ_ناپ, &وتھ,
                                 true);
    }

    ثابت تعداد xkb_keymap_format format = DEFAULT_داخلہ_KEYMAP_FORMAT;
    ثابت بنية parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    طالما (file) {
        XkbFile * ثابت xkb_file = XkbParseFile(ctx, &config, file, رستہ, map);
        إذا (xkb_file) {
            إذا (file_type < FILE_TYPE_عدد_ENTRIES &&
                xkb_file->file_type != file_type) {
                log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                        "File of wrong type (expected %s, got %s); "
                        "file \"%s\" ignored\n",
                        xkb_file_type_منزل_string(file_type),
                        xkb_file_type_منزل_string(xkb_file->file_type),
                        (absolute_رستہ ? رستہ : resolved_رستہ));
                اذهب invalid_file;
            } وإلا إذا (map || (xkb_file->flags && MAP_IS_DEFAULT)) {
                /*
                 * Exact match: explicit map name or explicit default map.
                 * Lookup stops in this iteration.
                 */
                إذا (!strcpy_محفوظ(resolved_map, resolved_map_ناپ,
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
                إذا (!strcpy_محفوظ(resolved_map, resolved_map_ناپ,
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

        إذا (file != عدم || absolute_رستہ) {
            /* Exact match or absolute path with no further file to search */
            اكسر;
        }

        وتھ++;
        file = لبھو_File_وچ_Xkb_رستہ(ctx, "(unknown)",
                                 رستہ, رستہ_len, file_type,
                                 resolved_رستہ, resolved_رستہ_ناپ,
                                 &وتھ, true);
    }

    إذا (!file) {
        /* No exact match: use weak match, if any */
        file = candidate;
    } وإلا إذا (candidate) {
        /* Found exact match: discard weak match, if any */
        fclose(candidate);
    }

    إذا (absolute_رستہ && file &&
        !strcpy_محفوظ(resolved_رستہ, resolved_رستہ_ناپ, رستہ)) {
        اذهب error;
    }

    أرجع file;

error:
    log_err(ctx, XKB_ERROR_INSUFFICIENT_بفر_ناپ,
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
    darray_ناپ(section->include_groups) = 0;
    darray_ناپ(section->includes) = 0;
    darray_ناپ(section->بفر) = 1; /* keep the dummy string */
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
xkb_file_section_سیٹ_meta_انکڑے(بنية xkb_context *ctx,
                               بنية xkb_file_section *section,
                               ثابت XkbFile *xkb_file)
{
    section->file_type = xkb_file->file_type;
    section->flags = xkb_file->flags;
    إذا (xkb_file->name) {
        darray_ناپ_t idx = darray_ناپ(section->بفر);
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
                                 ثابت حرف *section_رستہ,
                                 بنية xkb_file_section *section,
                                 تعداد xkb_file_type file_type,
                                 IncludeStmt * include)
{
    بنية xkb_file_include_گروہ *گروہ = عدم;
    /* Note: this will flatten statements such as `include "pc+de"` */
    لكل (IncludeStmt *stmt = include; stmt; stmt = stmt->اگلا_incl) {
        حرف buf[PATH_MAX];
        /* Parse the included file to check the include validity */
        XkbFile *xkb_file = ProcessIncludeFile(ctx, config, stmt, file_type,
                                               buf, حجم(buf));
        ثابت bool valid = (xkb_file != عدم);
        إذا (valid || !(flags & XKB_FILE_ITERATOR_FAIL_ON_INCLUDE_ERROR)) {
            /* Collect the strings of the statement properties */

            ثابت darray_ناپ_t رستہ = darray_ناپ(section->بفر);
            darray_append_string0(section->بفر, buf);

            ثابت darray_ناپ_t file = darray_ناپ(section->بفر);
            darray_append_string0(section->بفر, stmt->file);

            ثابت darray_ناپ_t section_name =
                (stmt->map != عدم || (valid && xkb_file->name))
                    ? darray_ناپ(section->بفر)
                    : 0;
            إذا (section_name) {
                darray_append_string0(section->بفر,
                                      (stmt->map) ? stmt->map : xkb_file->name);
            }

            ثابت darray_ناپ_t modifier = (stmt->modifier)
                ? darray_ناپ(section->بفر)
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
                .ملاؤ = stmt->ملاؤ,
                .رستہ = رستہ,
                .file = file,
                .section = section_name,
                .modifier = modifier,
                .flags = section_flags
            };
            ثابت darray_ناپ_t idx = darray_ناپ(section->includes);
            darray_append(section->includes, inc);

            /* Update include group */
            إذا (گروہ == عدم) {
                ثابت darray_ناپ_t گروہ_idx =
                    darray_ناپ(section->include_groups);
                darray_append(
                    section->include_groups,
                    (بنية xkb_file_include_گروہ) {.شروع = idx, .سرا = idx}
                );
                گروہ = &darray_شے(section->include_groups, گروہ_idx);
            } وإلا {
                گروہ->سرا = idx;
            }

            FreeXkbFile(xkb_file);
        } وإلا {
            ثابت حرف * ثابت name =
                xkb_file_section_get_string(section, section->name);
            log_err(ctx, XKB_ERROR_INCLUDED_FILE_NOT_لبھیا,
                    "%s include failure in: %s%s%s%s\n",
                    xkb_file_type_name(file_type), section_رستہ,
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
                         ثابت حرف *رستہ,
                         بنية xkb_file_section *section,
                         ثابت XkbFile *xkb_file)
{
    bool ok = true;
    لكل (ParseCommon *stmt = xkb_file->defs; stmt; stmt = stmt->اگلا) {
        إذا (stmt->type == STMT_INCLUDE) {
            ok = xkb_file_section_append_includes(ctx, flags, config, رستہ,
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
                       بنانشانی صحيح include_ڈونگھائی,
                       ثابت حرف *رستہ, ثابت حرف *map,
                       بنية xkb_file_section *section)
{
    إذا (Exceeds_Include_Max_ڈونگھائی(ctx, include_ڈونگھائی))
        أرجع false;

    FILE *file = fopen(رستہ, "rb");
    إذا (!file) {
        log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                "Cannot open file: %s\n", رستہ);
        أرجع false;
    }

    ثابت بنية parser_keymap_config config = {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };

    XkbFile *xkb_file = XkbParseFile(ctx, &config, file, رستہ, map);
    fclose(file);
    إذا (!xkb_file) {
        log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                "Cannot parse map \"%s\" in file: %s\n",
                (map ? map : "(no map)"), رستہ);
        أرجع false;
    }

    xkb_file_section_reset(section);
    ثابت bool no_includes = iterator_flags & XKB_FILE_ITERATOR_NO_INCLUDES;
    ثابت bool ok = (
        xkb_file_section_سیٹ_meta_انکڑے(ctx, section, xkb_file) &&
        (no_includes ||
         xkb_file_section_process(ctx, iterator_flags, &config, رستہ, section,
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
                                  ثابت حرف *رستہ, ثابت حرف *map,
                                  تعداد xkb_file_type file_type,
                                  ثابت حرف *string, ناپ_t لمائی)
{
    بنية xkb_file_iterator * ثابت iter = صفر_حافظہ_دیو(1, حجم(*iter));
    إذا (!iter) {
        log_err(ctx, XKB_ERROR_ونڈ_FAILURE_2,
                "Cannot allocate file iterator\n");
        أرجع عدم;
    }

    iter->flags = iterator_flags;
    iter->ctx = ctx;
    iter->رستہ = رستہ;
    iter->map = map;
    iter->type = file_type;
    iter->parser_config = (بنية parser_keymap_config) {
        .format = format,
        .strict = parser_strict_flags_from_keymap_format(format)
    };
    xkb_file_section_init(&iter->section);

    إذا (!XkbParseStringInit(ctx, &iter->scanner, string, لمائی, رستہ, عدم)) {
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
xkb_file_iterator_اگلا(بنية xkb_file_iterator *iter,
                       ثابت بنية xkb_file_section **section)
{
    إذا (iter->مکمل) {
        *section = عدم;
        أرجع true;
    }

اگلا:
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

    إذا (!Xkb_Parse_String_اگلا(iter->ctx, &iter->parser_config, &iter->scanner,
                            iter->map, &xkb_file)) {
        log_err(iter->ctx, XKB_LOG_سنیہا_NO_ID,
                "Error while parsing section in file: %s\n", iter->رستہ);
        اذهب error;
    } وإلا إذا (!xkb_file) {
        /* No more sections */
        iter->مکمل = true;
        *section = عدم;
        أرجع true;
    }

parse_components:
    /* Reset section */
    xkb_file_section_reset(&iter->section);

    /* Meta data */
    إذا (!xkb_file_section_سیٹ_meta_انکڑے(iter->ctx, &iter->section, xkb_file))
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
                iter->pending_section = (XkbFile *) xkb_file->common.اگلا;
                اذهب اگلا;
            } وإلا {
                /* Component-specific file: type mismatch */
                log_err(iter->ctx, XKB_LOG_سنیہا_NO_ID,
                        "File type mismatch: %s, section: %s\n",
                        iter->رستہ,
                        (xkb_file->name ? xkb_file->name : "(no name)"));
                اذهب error;
            }
        }
        إذا (iter->map) {
            iter->مکمل = true;
        }
    }

    /* Collect include statements of current section */
    ثابت bool process_includes = !(iter->flags & XKB_FILE_ITERATOR_NO_INCLUDES);
    إذا (process_includes &&
        !xkb_file_section_process(iter->ctx, iter->flags, &iter->parser_config,
                                  iter->رستہ, &iter->section, xkb_file)) {
        اذهب error;
    } وإلا إذا (iter->pending_section) {
        /* Next component */
        iter->pending_section = (XkbFile *) xkb_file->common.اگلا;
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
xkb_file_section_get_string(ثابت بنية xkb_file_section *section, darray_ناپ_t idx)
{
    أرجع &darray_شے(section->بفر, idx);
}
