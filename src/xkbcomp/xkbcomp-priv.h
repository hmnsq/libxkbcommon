/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "ast.h"
#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "keymap.h"
#அந்தர்பாவ "scanner-utils.h"
#அந்தர்பாவ "text.h"

எண்ணிக்கை xkb_parser_error {
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
எண்ணிக்கை xkb_parser_strict_flags {
    PARSER_NO_STRICT_FLAGS = 0,
    PARSER_NO_UNKNOWN_STATEMENTS = (1 << 0),
    PARSER_NO_FIELD_TYPE_MISMATCH = (1 << 1),
    PARSER_NO_FIELD_மதிப்பு_MISMATCH = (1 << 2),

    PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS = (1 << 3),

    PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS = (1 << 4),
    PARSER_NO_UNKNOWN_TYPE_FIELDS = (1 << 5),

    PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS = (1 << 6),
    PARSER_NO_UNKNOWN_INTERPRET_FIELDS = (1 << 7),
    PARSER_NO_UNKNOWN_LED_FIELDS = (1 << 8),

    PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS = (1 << 9),
    PARSER_NO_UNKNOWN_சாவி_FIELDS = (1 << 10),

    PARSER_NO_UNKNOWN_ACTION = (1 << 11),
    PARSER_NO_UNKNOWN_ACTION_FIELDS = (1 << 12),
    PARSER_NO_ILLEGAL_ACTION_FIELDS = (1 << 13),

    PARSER_NO_UNKNOWN_SECTION_FLAGS = (1 << 14),

    PARSER_STRICT_FLAGS_ALL = ((PARSER_NO_UNKNOWN_SECTION_FLAGS << 1) - 1),

    PARSER_V1_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    /* Limited flexibility */
    PARSER_V1_LAX_FLAGS = (PARSER_V1_STRICT_FLAGS &
                           ~PARSER_NO_FIELD_மதிப்பு_MISMATCH),

    PARSER_V2_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    PARSER_V2_LAX_FLAGS = PARSER_NO_STRICT_FLAGS,
};

நிலையான உள்வரி எண்ணிக்கை xkb_parser_strict_flags
parser_strict_flags_from_keymap_format(எண்ணிக்கை xkb_keymap_format format)
{
    திரும்பு (format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? PARSER_V1_LAX_FLAGS
            : PARSER_V2_LAX_FLAGS;
}

நிலையான உள்வரி எண்ணிக்கை xkb_parser_strict_flags
parser_strict_flags_from_keymap(மாறிலி கட்டமைப்பு xkb_keymap *keymap)
{
    திரும்பு (keymap->format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_MODE
                ? PARSER_V1_STRICT_FLAGS
                : PARSER_V1_LAX_FLAGS)
            : (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_MODE
                ? PARSER_V2_STRICT_FLAGS
                : PARSER_V2_LAX_FLAGS);
}

கட்டமைப்பு parser_keymap_config {
    எண்ணிக்கை xkb_keymap_format format;
    எண்ணிக்கை xkb_parser_strict_flags strict;
};

வகைவரை ஒன்றிப்பு கோவை_Def கோவை_Def;
கட்டமைப்பு pending_computation {
    கோவை_Def * கோவை;
    bool computed;
    uint32_t மதிப்பு;
};
வகைவரை darray(கட்டமைப்பு pending_computation) pending_computation_array;

/** Keymap augmented with miscellaneous data used during compilation */
கட்டமைப்பு xkb_keymap_info {
    /** The keymap being compiled */
    கட்டமைப்பு xkb_keymap keymap;

    /** Flags of the strict mode */
    எண்ணிக்கை xkb_parser_strict_flags strict;

    /** Features */
    கட்டமைப்பு {
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
    கட்டமைப்பு {
        LookupEntry groupIndexNames[3];
        LookupEntry groupMaskNames[5];
    } lookup;

    /** Pending computations */
    pending_computation_array *pending_computations;
};

எண்ணிக்கை xkb_status
text_v1_keymap_serialize(
        மாறிலி கட்டமைப்பு xkb_keymap *keymap,
        மாறிலி கட்டமைப்பு xkb_keymap_serialize_config *config,
        கட்டமைப்பு xkb_keymap_serialize_result *result
);

XkbFile *
XkbParseFile(கட்டமைப்பு xkb_context *ctx,
             மாறிலி கட்டமைப்பு parser_keymap_config *config,
             FILE *file, மாறிலி எழுத்து *file_name, மாறிலி எழுத்து *map);
bool
XkbParseStringInit(கட்டமைப்பு xkb_context *ctx, கட்டமைப்பு scanner *scanner,
                   மாறிலி எழுத்து *string, size_t len,
                   மாறிலி எழுத்து *file_name, மாறிலி எழுத்து *map);

XkbFile *
XkbParseString(கட்டமைப்பு xkb_context *ctx,
               மாறிலி கட்டமைப்பு parser_keymap_config *config,
               மாறிலி எழுத்து *string, size_t len,
               மாறிலி எழுத்து *file_name, மாறிலி எழுத்து *map);

bool
XkbParseStringNext(கட்டமைப்பு xkb_context *ctx,
                   மாறிலி கட்டமைப்பு parser_keymap_config *config,
                   கட்டமைப்பு scanner *scanner, மாறிலி எழுத்து *map, XkbFile **out);

வெறுமை
FreeXkbFile(XkbFile *file);

XkbFile *
XkbFileFromComponents(கட்டமைப்பு xkb_context *ctx,
                      மாறிலி கட்டமைப்பு xkb_component_names *kkctgs);

bool
CompileKeycodes(XkbFile *file, கட்டமைப்பு xkb_keymap_info *keymap_info);

bool
Compile_சாவி_Types(XkbFile *file, கட்டமைப்பு xkb_keymap_info *keymap_info);

bool
CompileCompatMap(XkbFile *file, கட்டமைப்பு xkb_keymap_info *keymap_info);

bool
CompileSymbols(XkbFile *file, கட்டமைப்பு xkb_keymap_info *keymap_info);

bool
CompileKeymap(XkbFile *file, கட்டமைப்பு xkb_keymap *keymap);

/***====================================================================***/

நிலையான உள்வரி bool
ReportNotArray(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து *type, மாறிலி எழுத்து *field,
               மாறிலி எழுத்து *name)
{
    log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
            "The %s %s field is not an array; "
            "Ignoring illegal assignment in %s\n",
            type, field, name);
    திரும்பு false;
}

நிலையான உள்வரி bool
ReportShouldBeArray(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து *type,
                    மாறிலி எழுத்து *field, மாறிலி எழுத்து *name)
{
    log_err(ctx, XKB_ERROR_EXPECTED_ARRAY_ENTRY,
            "Missing subscript for %s %s; "
            "Ignoring illegal assignment in %s\n",
            type, field, name);
    திரும்பு false;
}

நிலையான உள்வரி bool
ReportBadType(கட்டமைப்பு xkb_context *ctx, எண்ணிக்கை xkb_message_code code,
              மாறிலி எழுத்து *type, மாறிலி எழுத்து *field, மாறிலி எழுத்து *name,
              மாறிலி எழுத்து *wanted)
{
    log_err(ctx, code,
            "The %s %s field must be a %s; "
            "Ignoring illegal assignment in %s\n",
            type, field, wanted, name);
    திரும்பு false;
}

நிலையான உள்வரி bool
ReportBadField(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து *type, மாறிலி எழுத்து *field,
               மாறிலி எழுத்து *name)
{
    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Unknown %s field \"%s\" in %s; "
            "Ignoring assignment to unknown field in %s\n",
            type, field, name, name);
    திரும்பு false;
}

நிலையான உள்வரி மாறிலி எழுத்து*
safe_map_name(மாறிலி எழுத்து* name)
{
    திரும்பு name ? name : "(unnamed map)";
}
