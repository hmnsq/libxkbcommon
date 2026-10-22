/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#निर्देशः once

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
    PARSER_NO_क्षेत्रम्_TYPE_MISMATCH = (1 << 1),
    PARSER_NO_क्षेत्रम्_मानम्_MISMATCH = (1 << 2),

    PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS = (1 << 3),

    PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS = (1 << 4),
    PARSER_NO_UNKNOWN_TYPE_FIELDS = (1 << 5),

    PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS = (1 << 6),
    PARSER_NO_UNKNOWN_INTERPRET_FIELDS = (1 << 7),
    PARSER_NO_UNKNOWN_LED_FIELDS = (1 << 8),

    PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS = (1 << 9),
    PARSER_NO_UNKNOWN_कुञ्चिका_FIELDS = (1 << 10),

    PARSER_NO_UNKNOWN_ACTION = (1 << 11),
    PARSER_NO_UNKNOWN_ACTION_FIELDS = (1 << 12),
    PARSER_NO_ILLEGAL_ACTION_FIELDS = (1 << 13),

    PARSER_NO_UNKNOWN_SECTION_FLAGS = (1 << 14),

    PARSER_STRICT_FLAGS_ALL = ((PARSER_NO_UNKNOWN_SECTION_FLAGS << 1) - 1),

    PARSER_V1_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    /* Limited flexibility */
    PARSER_V1_LAX_FLAGS = (PARSER_V1_STRICT_FLAGS &
                           ~PARSER_NO_क्षेत्रम्_मानम्_MISMATCH),

    PARSER_V2_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    PARSER_V2_LAX_FLAGS = PARSER_NO_STRICT_FLAGS,
};

स्थिर अन्तःस्थ गणना xkb_parser_strict_flags
parser_strict_flags_from_keymap_format(गणना xkb_keymap_format format)
{
    प्रत्यागम (format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? PARSER_V1_LAX_FLAGS
            : PARSER_V2_LAX_FLAGS;
}

स्थिर अन्तःस्थ गणना xkb_parser_strict_flags
parser_strict_flags_from_keymap(ध्रुव संरचना xkb_keymap *keymap)
{
    प्रत्यागम (keymap->format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_बहुलकम्
                ? PARSER_V1_STRICT_FLAGS
                : PARSER_V1_LAX_FLAGS)
            : (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_बहुलकम्
                ? PARSER_V2_STRICT_FLAGS
                : PARSER_V2_LAX_FLAGS);
}

संरचना parser_keymap_config {
    गणना xkb_keymap_format format;
    गणना xkb_parser_strict_flags strict;
};

प्रकारनिर्देश सङ्घ व्यञ्जकम्_Def व्यञ्जकम्_Def;
संरचना pending_computation {
    व्यञ्जकम्_Def * व्यञ्जकम्;
    bool computed;
    uint32_t मानम्;
};
प्रकारनिर्देश darray(संरचना pending_computation) pending_computation_array;

/** Keymap augmented with miscellaneous data used during compilation */
संरचना xkb_keymap_info {
    /** The keymap being compiled */
    संरचना xkb_keymap keymap;

    /** Flags of the strict mode */
    गणना xkb_parser_strict_flags strict;

    /** Features */
    संरचना {
        /** Maximum groups for the keymap format */
        xkb_layout_सूचकांक_t max_groups;
        /** Maximum overlays for the keymap format */
        xkb_overlay_सूचकांक_t max_overlays;
        /** Offset of the Controls names LUT for the keymap format */
        uint8_t controls_name_विस्थापनम्;
        /* Actions */
        bool गणः_तालकम्_on_release;
        bool mods_unlock_on_press;
        bool mods_latch_on_press;
        /* Overlays */
        bool overlapping_overlays;
    } features;

    /* Non-static LUTs */
    संरचना {
        Lookup_प्रविष्टिः गणः_सूचकांक_नामानि[3];
        Lookup_प्रविष्टिः गणः_आवरणम्_नामानि[5];
    } lookup;

    /** Pending computations */
    pending_computation_array *pending_computations;
};

गणना xkb_स्थितिः
text_v1_keymap_serialize(
        ध्रुव संरचना xkb_keymap *keymap,
        ध्रुव संरचना xkb_keymap_serialize_config *config,
        संरचना xkb_keymap_serialize_परिणाम *परिणाम
);

XkbFile *
XkbParseFile(संरचना xkb_context *ctx,
             ध्रुव संरचना parser_keymap_config *config,
             FILE *file, ध्रुव वर्ण *file_name, ध्रुव वर्ण *map);
bool
XkbParseStringInit(संरचना xkb_context *ctx, संरचना scanner *scanner,
                   ध्रुव वर्ण *string, आकार_t len,
                   ध्रुव वर्ण *file_name, ध्रुव वर्ण *map);

XkbFile *
XkbParseString(संरचना xkb_context *ctx,
               ध्रुव संरचना parser_keymap_config *config,
               ध्रुव वर्ण *string, आकार_t len,
               ध्रुव वर्ण *file_name, ध्रुव वर्ण *map);

bool
Xkb_Parse_String_अग्र(संरचना xkb_context *ctx,
                   ध्रुव संरचना parser_keymap_config *config,
                   संरचना scanner *scanner, ध्रुव वर्ण *map, XkbFile **बहिर्गमनम्);

शून्य
FreeXkbFile(XkbFile *file);

XkbFile *
XkbFileFromComponents(संरचना xkb_context *ctx,
                      ध्रुव संरचना xkb_component_नामानि *kkctgs);

bool
CompileKeycodes(XkbFile *file, संरचना xkb_keymap_info *keymap_info);

bool
Compile_कुञ्चिका_Types(XkbFile *file, संरचना xkb_keymap_info *keymap_info);

bool
CompileCompatMap(XkbFile *file, संरचना xkb_keymap_info *keymap_info);

bool
CompileSymbols(XkbFile *file, संरचना xkb_keymap_info *keymap_info);

bool
CompileKeymap(XkbFile *file, संरचना xkb_keymap *keymap);

/***====================================================================***/

स्थिर अन्तःस्थ bool
ReportNotArray(संरचना xkb_context *ctx, ध्रुव वर्ण *type, ध्रुव वर्ण *क्षेत्रम्_2,
               ध्रुव वर्ण *name)
{
    log_err(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE,
            "The %s %s field is not an array; "
            "Ignoring illegal assignment in %s\n",
            type, क्षेत्रम्_2, name);
    प्रत्यागम false;
}

स्थिर अन्तःस्थ bool
ReportShouldBeArray(संरचना xkb_context *ctx, ध्रुव वर्ण *type,
                    ध्रुव वर्ण *क्षेत्रम्_2, ध्रुव वर्ण *name)
{
    log_err(ctx, XKB_ERROR_EXPECTED_ARRAY_प्रविष्टिः,
            "Missing subscript for %s %s; "
            "Ignoring illegal assignment in %s\n",
            type, क्षेत्रम्_2, name);
    प्रत्यागम false;
}

स्थिर अन्तःस्थ bool
ReportBadType(संरचना xkb_context *ctx, गणना xkb_सन्देशः_संहिता संहिता,
              ध्रुव वर्ण *type, ध्रुव वर्ण *क्षेत्रम्_2, ध्रुव वर्ण *name,
              ध्रुव वर्ण *wanted)
{
    log_err(ctx, संहिता,
            "The %s %s field must be a %s; "
            "Ignoring illegal assignment in %s\n",
            type, क्षेत्रम्_2, wanted, name);
    प्रत्यागम false;
}

स्थिर अन्तःस्थ bool
Report_Bad_क्षेत्रम्(संरचना xkb_context *ctx, ध्रुव वर्ण *type, ध्रुव वर्ण *क्षेत्रम्_2,
               ध्रुव वर्ण *name)
{
    log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
            "Unknown %s field \"%s\" in %s; "
            "Ignoring assignment to unknown field in %s\n",
            type, क्षेत्रम्_2, name, name);
    प्रत्यागम false;
}

स्थिर अन्तःस्थ ध्रुव वर्ण*
safe_map_name(ध्रुव वर्ण* name)
{
    प्रत्यागम name ? name : "(unnamed map)";
}
