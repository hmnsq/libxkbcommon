/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#অন্তর্ভাব "config.h"

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "ast.h"
#অন্তর্ভাব "darray.h"
#অন্তর্ভাব "keymap.h"
#অন্তর্ভাব "scanner-utils.h"
#অন্তর্ভাব "text.h"

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

স্থির অন্তঃরেখ গণনা xkb_parser_strict_flags
parser_strict_flags_from_keymap_format(গণনা xkb_keymap_format format)
{
    ফেরত (format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? PARSER_V1_LAX_FLAGS
            : PARSER_V2_LAX_FLAGS;
}

স্থির অন্তঃরেখ গণনা xkb_parser_strict_flags
parser_strict_flags_from_keymap(ধ্রুবক কাঠামো xkb_keymap *keymap)
{
    ফেরত (keymap->format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_MODE
                ? PARSER_V1_STRICT_FLAGS
                : PARSER_V1_LAX_FLAGS)
            : (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_MODE
                ? PARSER_V2_STRICT_FLAGS
                : PARSER_V2_LAX_FLAGS);
}

কাঠামো parser_keymap_config {
    গণনা xkb_keymap_format format;
    গণনা xkb_parser_strict_flags strict;
};

প্রকারলেখ সংঘ রাশিমালা_Def রাশিমালা_Def;
কাঠামো pending_computation {
    রাশিমালা_Def * রাশিমালা;
    bool computed;
    uint32_t মান;
};
প্রকারলেখ darray(কাঠামো pending_computation) pending_computation_array;

/** Keymap augmented with miscellaneous data used during compilation */
কাঠামো xkb_keymap_info {
    /** The keymap being compiled */
    কাঠামো xkb_keymap keymap;

    /** Flags of the strict mode */
    গণনা xkb_parser_strict_flags strict;

    /** Features */
    কাঠামো {
        /** Maximum groups for the keymap format */
        xkb_layout_সূচক_t max_groups;
        /** Maximum overlays for the keymap format */
        xkb_overlay_সূচক_t max_overlays;
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
    কাঠামো {
        LookupEntry group_সূচক_Names[3];
        LookupEntry groupMaskNames[5];
    } lookup;

    /** Pending computations */
    pending_computation_array *pending_computations;
};

গণনা xkb_status
text_v1_keymap_serialize(
        ধ্রুবক কাঠামো xkb_keymap *keymap,
        ধ্রুবক কাঠামো xkb_keymap_serialize_config *config,
        কাঠামো xkb_keymap_serialize_ফলাফল *ফলাফল
);

XkbFile *
XkbParseFile(কাঠামো xkb_context *ctx,
             ধ্রুবক কাঠামো parser_keymap_config *config,
             FILE *file, ধ্রুবক অক্ষর *file_name, ধ্রুবক অক্ষর *map);
bool
XkbParseStringInit(কাঠামো xkb_context *ctx, কাঠামো scanner *scanner,
                   ধ্রুবক অক্ষর *string, আকার_t len,
                   ধ্রুবক অক্ষর *file_name, ধ্রুবক অক্ষর *map);

XkbFile *
XkbParseString(কাঠামো xkb_context *ctx,
               ধ্রুবক কাঠামো parser_keymap_config *config,
               ধ্রুবক অক্ষর *string, আকার_t len,
               ধ্রুবক অক্ষর *file_name, ধ্রুবক অক্ষর *map);

bool
Xkb_Parse_String_পরবর্তী(কাঠামো xkb_context *ctx,
                   ধ্রুবক কাঠামো parser_keymap_config *config,
                   কাঠামো scanner *scanner, ধ্রুবক অক্ষর *map, XkbFile **out);

শূন্য
FreeXkbFile(XkbFile *file);

XkbFile *
XkbFileFromComponents(কাঠামো xkb_context *ctx,
                      ধ্রুবক কাঠামো xkb_component_names *kkctgs);

bool
CompileKeycodes(XkbFile *file, কাঠামো xkb_keymap_info *keymap_info);

bool
Compile_চাবি_Types(XkbFile *file, কাঠামো xkb_keymap_info *keymap_info);

bool
CompileCompatMap(XkbFile *file, কাঠামো xkb_keymap_info *keymap_info);

bool
CompileSymbols(XkbFile *file, কাঠামো xkb_keymap_info *keymap_info);

bool
CompileKeymap(XkbFile *file, কাঠামো xkb_keymap *keymap);

/***====================================================================***/

স্থির অন্তঃরেখ bool
ReportNotArray(কাঠামো xkb_context *ctx, ধ্রুবক অক্ষর *type, ধ্রুবক অক্ষর *field,
               ধ্রুবক অক্ষর *name)
{
    log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
            "The %s %s field is not an array; "
            "Ignoring illegal assignment in %s\n",
            type, field, name);
    ফেরত false;
}

স্থির অন্তঃরেখ bool
ReportShouldBeArray(কাঠামো xkb_context *ctx, ধ্রুবক অক্ষর *type,
                    ধ্রুবক অক্ষর *field, ধ্রুবক অক্ষর *name)
{
    log_err(ctx, XKB_ERROR_EXPECTED_ARRAY_ENTRY,
            "Missing subscript for %s %s; "
            "Ignoring illegal assignment in %s\n",
            type, field, name);
    ফেরত false;
}

স্থির অন্তঃরেখ bool
ReportBadType(কাঠামো xkb_context *ctx, গণনা xkb_message_code code,
              ধ্রুবক অক্ষর *type, ধ্রুবক অক্ষর *field, ধ্রুবক অক্ষর *name,
              ধ্রুবক অক্ষর *wanted)
{
    log_err(ctx, code,
            "The %s %s field must be a %s; "
            "Ignoring illegal assignment in %s\n",
            type, field, wanted, name);
    ফেরত false;
}

স্থির অন্তঃরেখ bool
ReportBadField(কাঠামো xkb_context *ctx, ধ্রুবক অক্ষর *type, ধ্রুবক অক্ষর *field,
               ধ্রুবক অক্ষর *name)
{
    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Unknown %s field \"%s\" in %s; "
            "Ignoring assignment to unknown field in %s\n",
            type, field, name, name);
    ফেরত false;
}

স্থির অন্তঃরেখ ধ্রুবক অক্ষর*
safe_map_name(ধ্রুবক অক্ষর* name)
{
    ফেরত name ? name : "(unnamed map)";
}
