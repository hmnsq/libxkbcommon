/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "ast.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "scanner-utils.h"
#अन्तर्भाव "text.h"

गणना xkb_parser_error {
    PARSER_SUCCESS = 0,
    PARSER_RECOVERABLE_ERROR,
    PARSER_FATAL_ERROR
};

/**
 * Flags to control where the parser strictness applies.
 *
 * This is probably overkill, but it could enable fine grained control
 * depending of the keymap format or other factors.
 */
गणना xkb_parser_strict_flags {
    PARSER_NO_STRICT_FLAGS = 0,
    PARSER_NO_UNKNOWN_STATEMENTS = (1 << 0),
    PARSER_NO_FIELD_TYPE_MISMATCH = (1 << 1),
    PARSER_NO_FIELD_मूल्य_MISMATCH = (1 << 2),

    PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS = (1 << 3),

    PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS = (1 << 4),
    PARSER_NO_UNKNOWN_TYPE_FIELDS = (1 << 5),

    PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS = (1 << 6),
    PARSER_NO_UNKNOWN_INTERPRET_FIELDS = (1 << 7),
    PARSER_NO_UNKNOWN_LED_FIELDS = (1 << 8),

    PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS = (1 << 9),
    PARSER_NO_UNKNOWN_किल्ली_FIELDS = (1 << 10),

    PARSER_NO_UNKNOWN_ACTION = (1 << 11),
    PARSER_NO_UNKNOWN_ACTION_FIELDS = (1 << 12),
    PARSER_NO_ILLEGAL_ACTION_FIELDS = (1 << 13),

    PARSER_NO_UNKNOWN_SECTION_FLAGS = (1 << 14),

    PARSER_STRICT_FLAGS_ALL = ((PARSER_NO_UNKNOWN_SECTION_FLAGS << 1) - 1),

    PARSER_V1_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    /* Limited flexibility */
    PARSER_V1_LAX_FLAGS = (PARSER_V1_STRICT_FLAGS &
                           ~PARSER_NO_FIELD_मूल्य_MISMATCH),

    PARSER_V2_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    PARSER_V2_LAX_FLAGS = PARSER_NO_STRICT_FLAGS,
};

स्थिर ओळीत गणना xkb_parser_strict_flags
parser_strict_flags_from_keymap_format(गणना xkb_keymap_format format)
{
    परत (format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? PARSER_V1_LAX_FLAGS
            : PARSER_V2_LAX_FLAGS;
}

स्थिर ओळीत गणना xkb_parser_strict_flags
parser_strict_flags_from_keymap(स्थिरांक संरचना xkb_keymap *keymap)
{
    परत (keymap->format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_MODE
                ? PARSER_V1_STRICT_FLAGS
                : PARSER_V1_LAX_FLAGS)
            : (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_MODE
                ? PARSER_V2_STRICT_FLAGS
                : PARSER_V2_LAX_FLAGS);
}

संरचना parser_keymap_config {
    गणना xkb_keymap_format format;
    गणना xkb_parser_strict_flags strict;
};

प्रकारव्याख्या संघ अभिव्यक्ती_Def अभिव्यक्ती_Def;
संरचना pending_computation {
    अभिव्यक्ती_Def * अभिव्यक्ती;
    bool computed;
    uint32_t मूल्य;
};
प्रकारव्याख्या darray(संरचना pending_computation) pending_computation_array;

/** Keymap augmented with miscellaneous data used during compilation */
संरचना xkb_keymap_info {
    /** The keymap being compiled */
    संरचना xkb_keymap keymap;

    /** Flags of the strict mode */
    गणना xkb_parser_strict_flags strict;

    /** Features */
    संरचना {
        /** Maximum groups for the keymap format */
        xkb_layout_index_t max_groups;
        /** Maximum overlays for the keymap format */
        xkb_overlay_index_t max_overlays;
        /** Offset of the Controls names LUT for the keymap format */
        uint8_t controls_name_offset;
        /* Actions */
        bool group_lock_on_release;
        bool mods_unlock_on_press;
        bool mods_latch_on_press;
        /* Overlays */
        bool overlapping_overlays;
    } features;

    /* Non-static LUTs */
    संरचना {
        LookupEntry groupIndexNames[3];
        LookupEntry groupMaskNames[5];
    } lookup;

    /** Pending computations */
    pending_computation_array *pending_computations;
};

गणना xkb_status
text_v1_keymap_serialize(
        स्थिरांक संरचना xkb_keymap *keymap,
        स्थिरांक संरचना xkb_keymap_serialize_config *config,
        संरचना xkb_keymap_serialize_result *result
);

XkbFile *
XkbParseFile(संरचना xkb_context *ctx,
             स्थिरांक संरचना parser_keymap_config *config,
             खाता *file, स्थिरांक अक्षर *file_name, स्थिरांक अक्षर *map);
bool
XkbParseStringInit(संरचना xkb_context *ctx, संरचना scanner *scanner,
                   स्थिरांक अक्षर *string, माप_प्रकार len,
                   स्थिरांक अक्षर *file_name, स्थिरांक अक्षर *map);

XkbFile *
XkbParseString(संरचना xkb_context *ctx,
               स्थिरांक संरचना parser_keymap_config *config,
               स्थिरांक अक्षर *string, माप_प्रकार len,
               स्थिरांक अक्षर *file_name, स्थिरांक अक्षर *map);

bool
XkbParseStringNext(संरचना xkb_context *ctx,
                   स्थिरांक संरचना parser_keymap_config *config,
                   संरचना scanner *scanner, स्थिरांक अक्षर *map, XkbFile **out);

शून्य
FreeXkbFile(XkbFile *file);

XkbFile *
XkbFileFromComponents(संरचना xkb_context *ctx,
                      स्थिरांक संरचना xkb_component_names *kkctgs);

bool
CompileKeycodes(XkbFile *file, संरचना xkb_keymap_info *keymap_info);

bool
Compile_किल्ली_Types(XkbFile *file, संरचना xkb_keymap_info *keymap_info);

bool
CompileCompatMap(XkbFile *file, संरचना xkb_keymap_info *keymap_info);

bool
CompileSymbols(XkbFile *file, संरचना xkb_keymap_info *keymap_info);

bool
CompileKeymap(XkbFile *file, संरचना xkb_keymap *keymap);

/***====================================================================***/

स्थिर ओळीत bool
ReportNotArray(संरचना xkb_context *ctx, स्थिरांक अक्षर *type, स्थिरांक अक्षर *field,
               स्थिरांक अक्षर *name)
{
    log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
            "The %s %s field is not an array; "
            "Ignoring illegal assignment in %s\n",
            type, field, name);
    परत false;
}

स्थिर ओळीत bool
ReportShouldBeArray(संरचना xkb_context *ctx, स्थिरांक अक्षर *type,
                    स्थिरांक अक्षर *field, स्थिरांक अक्षर *name)
{
    log_err(ctx, XKB_ERROR_EXPECTED_ARRAY_ENTRY,
            "Missing subscript for %s %s; "
            "Ignoring illegal assignment in %s\n",
            type, field, name);
    परत false;
}

स्थिर ओळीत bool
ReportBadType(संरचना xkb_context *ctx, गणना xkb_message_code code,
              स्थिरांक अक्षर *type, स्थिरांक अक्षर *field, स्थिरांक अक्षर *name,
              स्थिरांक अक्षर *wanted)
{
    log_err(ctx, code,
            "The %s %s field must be a %s; "
            "Ignoring illegal assignment in %s\n",
            type, field, wanted, name);
    परत false;
}

स्थिर ओळीत bool
ReportBadField(संरचना xkb_context *ctx, स्थिरांक अक्षर *type, स्थिरांक अक्षर *field,
               स्थिरांक अक्षर *name)
{
    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Unknown %s field \"%s\" in %s; "
            "Ignoring assignment to unknown field in %s\n",
            type, field, name, name);
    परत false;
}

स्थिर ओळीत स्थिरांक अक्षर*
safe_map_name(स्थिरांक अक्षर* name)
{
    परत name ? name : "(unnamed map)";
}
