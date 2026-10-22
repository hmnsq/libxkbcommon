/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#आशय once

#समावेश "config.h"

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "ast.h"
#समावेश "darray.h"
#समावेश "keymap.h"
#समावेश "scanner-utils.h"
#समावेश "text.h"

क्रमागत xkb_parser_error {
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
क्रमागत xkb_parser_strict_flags {
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
    PARSER_NO_UNKNOWN_कुंजी_FIELDS = (1 << 10),

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

अटल अंतरभूत क्रमागत xkb_parser_strict_flags
parser_strict_flags_from_keymap_format(क्रमागत xkb_keymap_format format)
{
    वापस (format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? PARSER_V1_LAX_FLAGS
            : PARSER_V2_LAX_FLAGS;
}

अटल अंतरभूत क्रमागत xkb_parser_strict_flags
parser_strict_flags_from_keymap(स्थिर काष्ठा xkb_keymap *keymap)
{
    वापस (keymap->format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_MODE
                ? PARSER_V1_STRICT_FLAGS
                : PARSER_V1_LAX_FLAGS)
            : (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_MODE
                ? PARSER_V2_STRICT_FLAGS
                : PARSER_V2_LAX_FLAGS);
}

काष्ठा parser_keymap_config {
    क्रमागत xkb_keymap_format format;
    क्रमागत xkb_parser_strict_flags strict;
};

प्रकार जोड़ व्यंजक_Def व्यंजक_Def;
काष्ठा pending_computation {
    व्यंजक_Def * व्यंजक;
    bool computed;
    uint32_t मूल्य;
};
प्रकार darray(काष्ठा pending_computation) pending_computation_array;

/** Keymap augmented with miscellaneous data used during compilation */
काष्ठा xkb_keymap_info {
    /** The keymap being compiled */
    काष्ठा xkb_keymap keymap;

    /** Flags of the strict mode */
    क्रमागत xkb_parser_strict_flags strict;

    /** Features */
    काष्ठा {
        /** Maximum groups for the keymap format */
        xkb_layout_सूचकांक_t max_groups;
        /** Maximum overlays for the keymap format */
        xkb_overlay_सूचकांक_t max_overlays;
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
    काष्ठा {
        LookupEntry group_सूचकांक_Names[3];
        LookupEntry groupMaskNames[5];
    } lookup;

    /** Pending computations */
    pending_computation_array *pending_computations;
};

क्रमागत xkb_status
text_v1_keymap_serialize(
        स्थिर काष्ठा xkb_keymap *keymap,
        स्थिर काष्ठा xkb_keymap_serialize_config *config,
        काष्ठा xkb_keymap_serialize_परिणाम *परिणाम
);

XkbFile *
XkbParseFile(काष्ठा xkb_context *ctx,
             स्थिर काष्ठा parser_keymap_config *config,
             खाता *file, स्थिर अक्षर *file_name, स्थिर अक्षर *map);
bool
XkbParseStringInit(काष्ठा xkb_context *ctx, काष्ठा scanner *scanner,
                   स्थिर अक्षर *string, माप_प्रकार len,
                   स्थिर अक्षर *file_name, स्थिर अक्षर *map);

XkbFile *
XkbParseString(काष्ठा xkb_context *ctx,
               स्थिर काष्ठा parser_keymap_config *config,
               स्थिर अक्षर *string, माप_प्रकार len,
               स्थिर अक्षर *file_name, स्थिर अक्षर *map);

bool
Xkb_Parse_String_अग्र(काष्ठा xkb_context *ctx,
                   स्थिर काष्ठा parser_keymap_config *config,
                   काष्ठा scanner *scanner, स्थिर अक्षर *map, XkbFile **out);

व्योम
FreeXkbFile(XkbFile *file);

XkbFile *
XkbFileFromComponents(काष्ठा xkb_context *ctx,
                      स्थिर काष्ठा xkb_component_names *kkctgs);

bool
CompileKeycodes(XkbFile *file, काष्ठा xkb_keymap_info *keymap_info);

bool
Compile_कुंजी_Types(XkbFile *file, काष्ठा xkb_keymap_info *keymap_info);

bool
CompileCompatMap(XkbFile *file, काष्ठा xkb_keymap_info *keymap_info);

bool
CompileSymbols(XkbFile *file, काष्ठा xkb_keymap_info *keymap_info);

bool
CompileKeymap(XkbFile *file, काष्ठा xkb_keymap *keymap);

/***====================================================================***/

अटल अंतरभूत bool
ReportNotArray(काष्ठा xkb_context *ctx, स्थिर अक्षर *type, स्थिर अक्षर *field,
               स्थिर अक्षर *name)
{
    log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
            "The %s %s field is not an array; "
            "Ignoring illegal assignment in %s\n",
            type, field, name);
    वापस false;
}

अटल अंतरभूत bool
ReportShouldBeArray(काष्ठा xkb_context *ctx, स्थिर अक्षर *type,
                    स्थिर अक्षर *field, स्थिर अक्षर *name)
{
    log_err(ctx, XKB_ERROR_EXPECTED_ARRAY_ENTRY,
            "Missing subscript for %s %s; "
            "Ignoring illegal assignment in %s\n",
            type, field, name);
    वापस false;
}

अटल अंतरभूत bool
ReportBadType(काष्ठा xkb_context *ctx, क्रमागत xkb_message_code code,
              स्थिर अक्षर *type, स्थिर अक्षर *field, स्थिर अक्षर *name,
              स्थिर अक्षर *wanted)
{
    log_err(ctx, code,
            "The %s %s field must be a %s; "
            "Ignoring illegal assignment in %s\n",
            type, field, wanted, name);
    वापस false;
}

अटल अंतरभूत bool
ReportBadField(काष्ठा xkb_context *ctx, स्थिर अक्षर *type, स्थिर अक्षर *field,
               स्थिर अक्षर *name)
{
    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Unknown %s field \"%s\" in %s; "
            "Ignoring assignment to unknown field in %s\n",
            type, field, name, name);
    वापस false;
}

अटल अंतरभूत स्थिर अक्षर*
safe_map_name(स्थिर अक्षर* name)
{
    वापस name ? name : "(unnamed map)";
}
