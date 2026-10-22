/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#شامل "config.h"

#شامل "xkbcommon/xkbcommon.h"
#شامل "ast.h"
#شامل "darray.h"
#شامل "keymap.h"
#شامل "scanner-utils.h"
#شامل "text.h"

شمارش xkb_parser_error {
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
شمارش xkb_parser_strict_flags {
    PARSER_NO_STRICT_FLAGS = 0,
    PARSER_NO_UNKNOWN_STATEMENTS = (1 << 0),
    PARSER_NO_FIELD_TYPE_MISMATCH = (1 << 1),
    PARSER_NO_FIELD_مقدار_MISMATCH = (1 << 2),

    PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS = (1 << 3),

    PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS = (1 << 4),
    PARSER_NO_UNKNOWN_TYPE_FIELDS = (1 << 5),

    PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS = (1 << 6),
    PARSER_NO_UNKNOWN_INTERPRET_FIELDS = (1 << 7),
    PARSER_NO_UNKNOWN_LED_FIELDS = (1 << 8),

    PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS = (1 << 9),
    PARSER_NO_UNKNOWN_کلید_FIELDS = (1 << 10),

    PARSER_NO_UNKNOWN_ACTION = (1 << 11),
    PARSER_NO_UNKNOWN_ACTION_FIELDS = (1 << 12),
    PARSER_NO_ILLEGAL_ACTION_FIELDS = (1 << 13),

    PARSER_NO_UNKNOWN_SECTION_FLAGS = (1 << 14),

    PARSER_STRICT_FLAGS_ALL = ((PARSER_NO_UNKNOWN_SECTION_FLAGS << 1) - 1),

    PARSER_V1_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    /* Limited flexibility */
    PARSER_V1_LAX_FLAGS = (PARSER_V1_STRICT_FLAGS &
                           ~PARSER_NO_FIELD_مقدار_MISMATCH),

    PARSER_V2_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    PARSER_V2_LAX_FLAGS = PARSER_NO_STRICT_FLAGS,
};

ایستا درونخطی شمارش xkb_parser_strict_flags
parser_strict_flags_from_keymap_format(شمارش xkb_keymap_format format)
{
    بازگشت (format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? PARSER_V1_LAX_FLAGS
            : PARSER_V2_LAX_FLAGS;
}

ایستا درونخطی شمارش xkb_parser_strict_flags
parser_strict_flags_from_keymap(ثابت ساختار xkb_keymap *keymap)
{
    بازگشت (keymap->format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_MODE
                ? PARSER_V1_STRICT_FLAGS
                : PARSER_V1_LAX_FLAGS)
            : (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_MODE
                ? PARSER_V2_STRICT_FLAGS
                : PARSER_V2_LAX_FLAGS);
}

ساختار parser_keymap_config {
    شمارش xkb_keymap_format format;
    شمارش xkb_parser_strict_flags strict;
};

تعریفنوع اجتماع عبارت_Def عبارت_Def;
ساختار pending_computation {
    عبارت_Def * عبارت;
    bool computed;
    uint32_t مقدار;
};
تعریفنوع darray(ساختار pending_computation) pending_computation_array;

/** Keymap augmented with miscellaneous data used during compilation */
ساختار xkb_keymap_info {
    /** The keymap being compiled */
    ساختار xkb_keymap keymap;

    /** Flags of the strict mode */
    شمارش xkb_parser_strict_flags strict;

    /** Features */
    ساختار {
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
    ساختار {
        LookupEntry groupIndexNames[3];
        LookupEntry groupMaskNames[5];
    } lookup;

    /** Pending computations */
    pending_computation_array *pending_computations;
};

شمارش xkb_status
text_v1_keymap_serialize(
        ثابت ساختار xkb_keymap *keymap,
        ثابت ساختار xkb_keymap_serialize_config *config,
        ساختار xkb_keymap_serialize_result *result
);

XkbFile *
XkbParseFile(ساختار xkb_context *ctx,
             ثابت ساختار parser_keymap_config *config,
             FILE *file, ثابت نویسه *file_name, ثابت نویسه *map);
bool
XkbParseStringInit(ساختار xkb_context *ctx, ساختار scanner *scanner,
                   ثابت نویسه *string, size_t len,
                   ثابت نویسه *file_name, ثابت نویسه *map);

XkbFile *
XkbParseString(ساختار xkb_context *ctx,
               ثابت ساختار parser_keymap_config *config,
               ثابت نویسه *string, size_t len,
               ثابت نویسه *file_name, ثابت نویسه *map);

bool
XkbParseStringNext(ساختار xkb_context *ctx,
                   ثابت ساختار parser_keymap_config *config,
                   ساختار scanner *scanner, ثابت نویسه *map, XkbFile **out);

تهی
FreeXkbFile(XkbFile *file);

XkbFile *
XkbFileFromComponents(ساختار xkb_context *ctx,
                      ثابت ساختار xkb_component_names *kkctgs);

bool
CompileKeycodes(XkbFile *file, ساختار xkb_keymap_info *keymap_info);

bool
Compile_کلید_Types(XkbFile *file, ساختار xkb_keymap_info *keymap_info);

bool
CompileCompatMap(XkbFile *file, ساختار xkb_keymap_info *keymap_info);

bool
CompileSymbols(XkbFile *file, ساختار xkb_keymap_info *keymap_info);

bool
CompileKeymap(XkbFile *file, ساختار xkb_keymap *keymap);

/***====================================================================***/

ایستا درونخطی bool
ReportNotArray(ساختار xkb_context *ctx, ثابت نویسه *type, ثابت نویسه *field,
               ثابت نویسه *name)
{
    log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
            "The %s %s field is not an array; "
            "Ignoring illegal assignment in %s\n",
            type, field, name);
    بازگشت false;
}

ایستا درونخطی bool
ReportShouldBeArray(ساختار xkb_context *ctx, ثابت نویسه *type,
                    ثابت نویسه *field, ثابت نویسه *name)
{
    log_err(ctx, XKB_ERROR_EXPECTED_ARRAY_ENTRY,
            "Missing subscript for %s %s; "
            "Ignoring illegal assignment in %s\n",
            type, field, name);
    بازگشت false;
}

ایستا درونخطی bool
ReportBadType(ساختار xkb_context *ctx, شمارش xkb_message_code code,
              ثابت نویسه *type, ثابت نویسه *field, ثابت نویسه *name,
              ثابت نویسه *wanted)
{
    log_err(ctx, code,
            "The %s %s field must be a %s; "
            "Ignoring illegal assignment in %s\n",
            type, field, wanted, name);
    بازگشت false;
}

ایستا درونخطی bool
ReportBadField(ساختار xkb_context *ctx, ثابت نویسه *type, ثابت نویسه *field,
               ثابت نویسه *name)
{
    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Unknown %s field \"%s\" in %s; "
            "Ignoring assignment to unknown field in %s\n",
            type, field, name, name);
    بازگشت false;
}

ایستا درونخطی ثابت نویسه*
safe_map_name(ثابت نویسه* name)
{
    بازگشت name ? name : "(unnamed map)";
}
