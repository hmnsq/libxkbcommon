/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ "xkbcommon/xkbcommon.h"
#অন্তর্ভাৱ "ast.h"
#অন্তর্ভাৱ "darray.h"
#অন্তর্ভাৱ "keymap.h"
#অন্তর্ভাৱ "scanner-utils.h"
#অন্তর্ভাৱ "text.h"

গণনা xkb_parser_error {
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
গণনা xkb_parser_strict_flags {
    PARSER_NO_STRICT_FLAGS = 0,
    PARSER_NO_UNKNOWN_STATEMENTS = (1 << 0),
    PARSER_NO_FIELD_TYPE_MISMATCH = (1 << 1),
    PARSER_NO_FIELD_মান_MISMATCH = (1 << 2),

    PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS = (1 << 3),

    PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS = (1 << 4),
    PARSER_NO_UNKNOWN_TYPE_FIELDS = (1 << 5),

    PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS = (1 << 6),
    PARSER_NO_UNKNOWN_INTERPRET_FIELDS = (1 << 7),
    PARSER_NO_UNKNOWN_LED_FIELDS = (1 << 8),

    PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS = (1 << 9),
    PARSER_NO_UNKNOWN_চাবি_FIELDS = (1 << 10),

    PARSER_NO_UNKNOWN_ACTION = (1 << 11),
    PARSER_NO_UNKNOWN_ACTION_FIELDS = (1 << 12),
    PARSER_NO_ILLEGAL_ACTION_FIELDS = (1 << 13),

    PARSER_NO_UNKNOWN_SECTION_FLAGS = (1 << 14),

    PARSER_STRICT_FLAGS_ALL = ((PARSER_NO_UNKNOWN_SECTION_FLAGS << 1) - 1),

    PARSER_V1_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    /* Limited flexibility */
    PARSER_V1_LAX_FLAGS = (PARSER_V1_STRICT_FLAGS &
                           ~PARSER_NO_FIELD_মান_MISMATCH),

    PARSER_V2_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    PARSER_V2_LAX_FLAGS = PARSER_NO_STRICT_FLAGS,
};

স্থির অন্তঃস্থ গণনা xkb_parser_strict_flags
parser_strict_flags_from_keymap_format(গণনা xkb_keymap_format format)
{
    ঘূৰাই (format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? PARSER_V1_LAX_FLAGS
            : PARSER_V2_LAX_FLAGS;
}

স্থির অন্তঃস্থ গণনা xkb_parser_strict_flags
parser_strict_flags_from_keymap(ধ্রুৱ সংরচনা xkb_keymap *keymap)
{
    ঘূৰাই (keymap->format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_MODE
                ? PARSER_V1_STRICT_FLAGS
                : PARSER_V1_LAX_FLAGS)
            : (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_MODE
                ? PARSER_V2_STRICT_FLAGS
                : PARSER_V2_LAX_FLAGS);
}

সংরচনা parser_keymap_config {
    গণনা xkb_keymap_format format;
    গণনা xkb_parser_strict_flags strict;
};

প্রকারনির্দেশ সঙ্ঘ ৰাশিমালা_Def ৰাশিমালা_Def;
সংরচনা pending_computation {
    ৰাশিমালা_Def * ৰাশিমালা;
    bool computed;
    uint32_t মান;
};
প্রকারনির্দেশ darray(সংরচনা pending_computation) pending_computation_array;

/** Keymap augmented with miscellaneous data used during compilation */
সংরচনা xkb_keymap_info {
    /** The keymap being compiled */
    সংরচনা xkb_keymap keymap;

    /** Flags of the strict mode */
    গণনা xkb_parser_strict_flags strict;

    /** Features */
    সংরচনা {
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
    সংরচনা {
        LookupEntry groupIndexNames[3];
        LookupEntry groupMaskNames[5];
    } lookup;

    /** Pending computations */
    pending_computation_array *pending_computations;
};

গণনা xkb_status
text_v1_keymap_serialize(
        ধ্রুৱ সংরচনা xkb_keymap *keymap,
        ধ্রুৱ সংরচনা xkb_keymap_serialize_config *config,
        সংরচনা xkb_keymap_serialize_result *result
);

XkbFile *
XkbParseFile(সংরচনা xkb_context *ctx,
             ধ্রুৱ সংরচনা parser_keymap_config *config,
             FILE *file, ধ্রুৱ ৱর্ণ *file_name, ধ্রুৱ ৱর্ণ *map);
bool
XkbParseStringInit(সংরচনা xkb_context *ctx, সংরচনা scanner *scanner,
                   ধ্রুৱ ৱর্ণ *string, size_t len,
                   ধ্রুৱ ৱর্ণ *file_name, ধ্রুৱ ৱর্ণ *map);

XkbFile *
XkbParseString(সংরচনা xkb_context *ctx,
               ধ্রুৱ সংরচনা parser_keymap_config *config,
               ধ্রুৱ ৱর্ণ *string, size_t len,
               ধ্রুৱ ৱর্ণ *file_name, ধ্রুৱ ৱর্ণ *map);

bool
XkbParseStringNext(সংরচনা xkb_context *ctx,
                   ধ্রুৱ সংরচনা parser_keymap_config *config,
                   সংরচনা scanner *scanner, ধ্রুৱ ৱর্ণ *map, XkbFile **out);

শূন্য
FreeXkbFile(XkbFile *file);

XkbFile *
XkbFileFromComponents(সংরচনা xkb_context *ctx,
                      ধ্রুৱ সংরচনা xkb_component_names *kkctgs);

bool
CompileKeycodes(XkbFile *file, সংরচনা xkb_keymap_info *keymap_info);

bool
Compile_চাবি_Types(XkbFile *file, সংরচনা xkb_keymap_info *keymap_info);

bool
CompileCompatMap(XkbFile *file, সংরচনা xkb_keymap_info *keymap_info);

bool
CompileSymbols(XkbFile *file, সংরচনা xkb_keymap_info *keymap_info);

bool
CompileKeymap(XkbFile *file, সংরচনা xkb_keymap *keymap);

/***====================================================================***/

স্থির অন্তঃস্থ bool
ReportNotArray(সংরচনা xkb_context *ctx, ধ্রুৱ ৱর্ণ *type, ধ্রুৱ ৱর্ণ *field,
               ধ্রুৱ ৱর্ণ *name)
{
    log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
            "The %s %s field is not an array; "
            "Ignoring illegal assignment in %s\n",
            type, field, name);
    ঘূৰাই false;
}

স্থির অন্তঃস্থ bool
ReportShouldBeArray(সংরচনা xkb_context *ctx, ধ্রুৱ ৱর্ণ *type,
                    ধ্রুৱ ৱর্ণ *field, ধ্রুৱ ৱর্ণ *name)
{
    log_err(ctx, XKB_ERROR_EXPECTED_ARRAY_ENTRY,
            "Missing subscript for %s %s; "
            "Ignoring illegal assignment in %s\n",
            type, field, name);
    ঘূৰাই false;
}

স্থির অন্তঃস্থ bool
ReportBadType(সংরচনা xkb_context *ctx, গণনা xkb_message_code code,
              ধ্রুৱ ৱর্ণ *type, ধ্রুৱ ৱর্ণ *field, ধ্রুৱ ৱর্ণ *name,
              ধ্রুৱ ৱর্ণ *wanted)
{
    log_err(ctx, code,
            "The %s %s field must be a %s; "
            "Ignoring illegal assignment in %s\n",
            type, field, wanted, name);
    ঘূৰাই false;
}

স্থির অন্তঃস্থ bool
ReportBadField(সংরচনা xkb_context *ctx, ধ্রুৱ ৱর্ণ *type, ধ্রুৱ ৱর্ণ *field,
               ধ্রুৱ ৱর্ণ *name)
{
    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Unknown %s field \"%s\" in %s; "
            "Ignoring assignment to unknown field in %s\n",
            type, field, name, name);
    ঘূৰাই false;
}

স্থির অন্তঃস্থ ধ্রুৱ ৱর্ণ*
safe_map_name(ধ্রুৱ ৱর্ণ* name)
{
    ঘূৰাই name ? name : "(unnamed map)";
}
