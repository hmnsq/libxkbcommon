/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "ast.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "scanner-utils.h"
#ಅನ್ತರ್ಭಾವ "text.h"

ಗಣನಾ xkb_parser_error {
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
ಗಣನಾ xkb_parser_strict_flags {
    PARSER_NO_STRICT_FLAGS = 0,
    PARSER_NO_UNKNOWN_STATEMENTS = (1 << 0),
    PARSER_NO_FIELD_TYPE_MISMATCH = (1 << 1),
    PARSER_NO_FIELD_ಮೌಲ್ಯ_MISMATCH = (1 << 2),

    PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS = (1 << 3),

    PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS = (1 << 4),
    PARSER_NO_UNKNOWN_TYPE_FIELDS = (1 << 5),

    PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS = (1 << 6),
    PARSER_NO_UNKNOWN_INTERPRET_FIELDS = (1 << 7),
    PARSER_NO_UNKNOWN_LED_FIELDS = (1 << 8),

    PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS = (1 << 9),
    PARSER_NO_UNKNOWN_ಕೀಲಿ_FIELDS = (1 << 10),

    PARSER_NO_UNKNOWN_ACTION = (1 << 11),
    PARSER_NO_UNKNOWN_ACTION_FIELDS = (1 << 12),
    PARSER_NO_ILLEGAL_ACTION_FIELDS = (1 << 13),

    PARSER_NO_UNKNOWN_SECTION_FLAGS = (1 << 14),

    PARSER_STRICT_FLAGS_ALL = ((PARSER_NO_UNKNOWN_SECTION_FLAGS << 1) - 1),

    PARSER_V1_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    /* Limited flexibility */
    PARSER_V1_LAX_FLAGS = (PARSER_V1_STRICT_FLAGS &
                           ~PARSER_NO_FIELD_ಮೌಲ್ಯ_MISMATCH),

    PARSER_V2_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    PARSER_V2_LAX_FLAGS = PARSER_NO_STRICT_FLAGS,
};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಗಣನಾ xkb_parser_strict_flags
parser_strict_flags_from_keymap_format(ಗಣನಾ xkb_keymap_format format)
{
    ಪ್ರತ್ಯಾಗಮ (format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? PARSER_V1_LAX_FLAGS
            : PARSER_V2_LAX_FLAGS;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಗಣನಾ xkb_parser_strict_flags
parser_strict_flags_from_keymap(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap)
{
    ಪ್ರತ್ಯಾಗಮ (keymap->format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_MODE
                ? PARSER_V1_STRICT_FLAGS
                : PARSER_V1_LAX_FLAGS)
            : (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_MODE
                ? PARSER_V2_STRICT_FLAGS
                : PARSER_V2_LAX_FLAGS);
}

ಸಂರಚನಾ parser_keymap_config {
    ಗಣನಾ xkb_keymap_format format;
    ಗಣನಾ xkb_parser_strict_flags strict;
};

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಙ್ಘ ಅಭಿವ್ಯಕ್ತಿ_Def ಅಭಿವ್ಯಕ್ತಿ_Def;
ಸಂರಚನಾ pending_computation {
    ಅಭಿವ್ಯಕ್ತಿ_Def * ಅಭಿವ್ಯಕ್ತಿ;
    bool computed;
    uint32_t ಮೌಲ್ಯ;
};
ಪ್ರಕಾರನಿರ್ದೇಶ darray(ಸಂರಚನಾ pending_computation) pending_computation_array;

/** Keymap augmented with miscellaneous data used during compilation */
ಸಂರಚನಾ xkb_keymap_info {
    /** The keymap being compiled */
    ಸಂರಚನಾ xkb_keymap keymap;

    /** Flags of the strict mode */
    ಗಣನಾ xkb_parser_strict_flags strict;

    /** Features */
    ಸಂರಚನಾ {
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
    ಸಂರಚನಾ {
        LookupEntry groupIndexNames[3];
        LookupEntry groupMaskNames[5];
    } lookup;

    /** Pending computations */
    pending_computation_array *pending_computations;
};

ಗಣನಾ xkb_status
text_v1_keymap_serialize(
        ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap,
        ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_serialize_config *config,
        ಸಂರಚನಾ xkb_keymap_serialize_result *result
);

XkbFile *
XkbParseFile(ಸಂರಚನಾ xkb_context *ctx,
             ಧ್ರುವ ಸಂರಚನಾ parser_keymap_config *config,
             FILE *file, ಧ್ರುವ ವರ್ಣ *file_name, ಧ್ರುವ ವರ್ಣ *map);
bool
XkbParseStringInit(ಸಂರಚನಾ xkb_context *ctx, ಸಂರಚನಾ scanner *scanner,
                   ಧ್ರುವ ವರ್ಣ *string, size_t len,
                   ಧ್ರುವ ವರ್ಣ *file_name, ಧ್ರುವ ವರ್ಣ *map);

XkbFile *
XkbParseString(ಸಂರಚನಾ xkb_context *ctx,
               ಧ್ರುವ ಸಂರಚನಾ parser_keymap_config *config,
               ಧ್ರುವ ವರ್ಣ *string, size_t len,
               ಧ್ರುವ ವರ್ಣ *file_name, ಧ್ರುವ ವರ್ಣ *map);

bool
XkbParseStringNext(ಸಂರಚನಾ xkb_context *ctx,
                   ಧ್ರುವ ಸಂರಚನಾ parser_keymap_config *config,
                   ಸಂರಚನಾ scanner *scanner, ಧ್ರುವ ವರ್ಣ *map, XkbFile **out);

ಶೂನ್ಯ
FreeXkbFile(XkbFile *file);

XkbFile *
XkbFileFromComponents(ಸಂರಚನಾ xkb_context *ctx,
                      ಧ್ರುವ ಸಂರಚನಾ xkb_component_names *kkctgs);

bool
CompileKeycodes(XkbFile *file, ಸಂರಚನಾ xkb_keymap_info *keymap_info);

bool
Compile_ಕೀಲಿ_Types(XkbFile *file, ಸಂರಚನಾ xkb_keymap_info *keymap_info);

bool
CompileCompatMap(XkbFile *file, ಸಂರಚನಾ xkb_keymap_info *keymap_info);

bool
CompileSymbols(XkbFile *file, ಸಂರಚನಾ xkb_keymap_info *keymap_info);

bool
CompileKeymap(XkbFile *file, ಸಂರಚನಾ xkb_keymap *keymap);

/***====================================================================***/

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
ReportNotArray(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ *type, ಧ್ರುವ ವರ್ಣ *field,
               ಧ್ರುವ ವರ್ಣ *name)
{
    log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
            "The %s %s field is not an array; "
            "Ignoring illegal assignment in %s\n",
            type, field, name);
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
ReportShouldBeArray(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ *type,
                    ಧ್ರುವ ವರ್ಣ *field, ಧ್ರುವ ವರ್ಣ *name)
{
    log_err(ctx, XKB_ERROR_EXPECTED_ARRAY_ENTRY,
            "Missing subscript for %s %s; "
            "Ignoring illegal assignment in %s\n",
            type, field, name);
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
ReportBadType(ಸಂರಚನಾ xkb_context *ctx, ಗಣನಾ xkb_message_code code,
              ಧ್ರುವ ವರ್ಣ *type, ಧ್ರುವ ವರ್ಣ *field, ಧ್ರುವ ವರ್ಣ *name,
              ಧ್ರುವ ವರ್ಣ *wanted)
{
    log_err(ctx, code,
            "The %s %s field must be a %s; "
            "Ignoring illegal assignment in %s\n",
            type, field, wanted, name);
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
ReportBadField(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ *type, ಧ್ರುವ ವರ್ಣ *field,
               ಧ್ರುವ ವರ್ಣ *name)
{
    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Unknown %s field \"%s\" in %s; "
            "Ignoring assignment to unknown field in %s\n",
            type, field, name, name);
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ ವರ್ಣ*
safe_map_name(ಧ್ರುವ ವರ್ಣ* name)
{
    ಪ್ರತ್ಯಾಗಮ name ? name : "(unnamed map)";
}
