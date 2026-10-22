/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "ast.h"
#ଅନ୍ତର୍ଭାଵ "darray.h"
#ଅନ୍ତର୍ଭାଵ "keymap.h"
#ଅନ୍ତର୍ଭାଵ "scanner-utils.h"
#ଅନ୍ତର୍ଭାଵ "text.h"

ଗଣନା xkb_parser_error {
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
ଗଣନା xkb_parser_strict_flags {
    PARSER_NO_STRICT_FLAGS = 0,
    PARSER_NO_UNKNOWN_STATEMENTS = (1 << 0),
    PARSER_NO_FIELD_TYPE_MISMATCH = (1 << 1),
    PARSER_NO_FIELD_ମୂଲ୍ୟ_MISMATCH = (1 << 2),

    PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS = (1 << 3),

    PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS = (1 << 4),
    PARSER_NO_UNKNOWN_TYPE_FIELDS = (1 << 5),

    PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS = (1 << 6),
    PARSER_NO_UNKNOWN_INTERPRET_FIELDS = (1 << 7),
    PARSER_NO_UNKNOWN_LED_FIELDS = (1 << 8),

    PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS = (1 << 9),
    PARSER_NO_UNKNOWN_ଚାବି_FIELDS = (1 << 10),

    PARSER_NO_UNKNOWN_ACTION = (1 << 11),
    PARSER_NO_UNKNOWN_ACTION_FIELDS = (1 << 12),
    PARSER_NO_ILLEGAL_ACTION_FIELDS = (1 << 13),

    PARSER_NO_UNKNOWN_SECTION_FLAGS = (1 << 14),

    PARSER_STRICT_FLAGS_ALL = ((PARSER_NO_UNKNOWN_SECTION_FLAGS << 1) - 1),

    PARSER_V1_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    /* Limited flexibility */
    PARSER_V1_LAX_FLAGS = (PARSER_V1_STRICT_FLAGS &
                           ~PARSER_NO_FIELD_ମୂଲ୍ୟ_MISMATCH),

    PARSER_V2_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    PARSER_V2_LAX_FLAGS = PARSER_NO_STRICT_FLAGS,
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଗଣନା xkb_parser_strict_flags
parser_strict_flags_from_keymap_format(ଗଣନା xkb_keymap_format format)
{
    ଫେରନ୍ତୁ (format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? PARSER_V1_LAX_FLAGS
            : PARSER_V2_LAX_FLAGS;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଗଣନା xkb_parser_strict_flags
parser_strict_flags_from_keymap(ଅଚଳ ସଂରଚନା xkb_keymap *keymap)
{
    ଫେରନ୍ତୁ (keymap->format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_MODE
                ? PARSER_V1_STRICT_FLAGS
                : PARSER_V1_LAX_FLAGS)
            : (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_MODE
                ? PARSER_V2_STRICT_FLAGS
                : PARSER_V2_LAX_FLAGS);
}

ସଂରଚନା parser_keymap_config {
    ଗଣନା xkb_keymap_format format;
    ଗଣନା xkb_parser_strict_flags strict;
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂଘ ପଦସମଷ୍ଟି_Def ପଦସମଷ୍ଟି_Def;
ସଂରଚନା pending_computation {
    ପଦସମଷ୍ଟି_Def * ପଦସମଷ୍ଟି;
    bool computed;
    uint32_t ମୂଲ୍ୟ;
};
ପ୍ରକାରବ୍ୟାଖ୍ୟା darray(ସଂରଚନା pending_computation) pending_computation_array;

/** Keymap augmented with miscellaneous data used during compilation */
ସଂରଚନା xkb_keymap_info {
    /** The keymap being compiled */
    ସଂରଚନା xkb_keymap keymap;

    /** Flags of the strict mode */
    ଗଣନା xkb_parser_strict_flags strict;

    /** Features */
    ସଂରଚନା {
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
    ସଂରଚନା {
        LookupEntry groupIndexNames[3];
        LookupEntry groupMaskNames[5];
    } lookup;

    /** Pending computations */
    pending_computation_array *pending_computations;
};

ଗଣନା xkb_status
text_v1_keymap_serialize(
        ଅଚଳ ସଂରଚନା xkb_keymap *keymap,
        ଅଚଳ ସଂରଚନା xkb_keymap_serialize_config *config,
        ସଂରଚନା xkb_keymap_serialize_result *result
);

XkbFile *
XkbParseFile(ସଂରଚନା xkb_context *ctx,
             ଅଚଳ ସଂରଚନା parser_keymap_config *config,
             FILE *file, ଅଚଳ ଅକ୍ଷର *file_name, ଅଚଳ ଅକ୍ଷର *map);
bool
XkbParseStringInit(ସଂରଚନା xkb_context *ctx, ସଂରଚନା scanner *scanner,
                   ଅଚଳ ଅକ୍ଷର *string, size_t len,
                   ଅଚଳ ଅକ୍ଷର *file_name, ଅଚଳ ଅକ୍ଷର *map);

XkbFile *
XkbParseString(ସଂରଚନା xkb_context *ctx,
               ଅଚଳ ସଂରଚନା parser_keymap_config *config,
               ଅଚଳ ଅକ୍ଷର *string, size_t len,
               ଅଚଳ ଅକ୍ଷର *file_name, ଅଚଳ ଅକ୍ଷର *map);

bool
XkbParseStringNext(ସଂରଚନା xkb_context *ctx,
                   ଅଚଳ ସଂରଚନା parser_keymap_config *config,
                   ସଂରଚନା scanner *scanner, ଅଚଳ ଅକ୍ଷର *map, XkbFile **out);

ଶୂନ୍ୟ
FreeXkbFile(XkbFile *file);

XkbFile *
XkbFileFromComponents(ସଂରଚନା xkb_context *ctx,
                      ଅଚଳ ସଂରଚନା xkb_component_names *kkctgs);

bool
CompileKeycodes(XkbFile *file, ସଂରଚନା xkb_keymap_info *keymap_info);

bool
Compile_ଚାବି_Types(XkbFile *file, ସଂରଚନା xkb_keymap_info *keymap_info);

bool
CompileCompatMap(XkbFile *file, ସଂରଚନା xkb_keymap_info *keymap_info);

bool
CompileSymbols(XkbFile *file, ସଂରଚନା xkb_keymap_info *keymap_info);

bool
CompileKeymap(XkbFile *file, ସଂରଚନା xkb_keymap *keymap);

/***====================================================================***/

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
ReportNotArray(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର *type, ଅଚଳ ଅକ୍ଷର *field,
               ଅଚଳ ଅକ୍ଷର *name)
{
    log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
            "The %s %s field is not an array; "
            "Ignoring illegal assignment in %s\n",
            type, field, name);
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
ReportShouldBeArray(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର *type,
                    ଅଚଳ ଅକ୍ଷର *field, ଅଚଳ ଅକ୍ଷର *name)
{
    log_err(ctx, XKB_ERROR_EXPECTED_ARRAY_ENTRY,
            "Missing subscript for %s %s; "
            "Ignoring illegal assignment in %s\n",
            type, field, name);
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
ReportBadType(ସଂରଚନା xkb_context *ctx, ଗଣନା xkb_message_code code,
              ଅଚଳ ଅକ୍ଷର *type, ଅଚଳ ଅକ୍ଷର *field, ଅଚଳ ଅକ୍ଷର *name,
              ଅଚଳ ଅକ୍ଷର *wanted)
{
    log_err(ctx, code,
            "The %s %s field must be a %s; "
            "Ignoring illegal assignment in %s\n",
            type, field, wanted, name);
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
ReportBadField(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର *type, ଅଚଳ ଅକ୍ଷର *field,
               ଅଚଳ ଅକ୍ଷର *name)
{
    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Unknown %s field \"%s\" in %s; "
            "Ignoring assignment to unknown field in %s\n",
            type, field, name, name);
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ ଅକ୍ଷର*
safe_map_name(ଅଚଳ ଅକ୍ଷର* name)
{
    ଫେରନ୍ତୁ name ? name : "(unnamed map)";
}
