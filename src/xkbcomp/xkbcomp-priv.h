/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "ast.h"
#અન્તર્ભાવ "darray.h"
#અન્તર્ભાવ "keymap.h"
#અન્તર્ભાવ "scanner-utils.h"
#અન્તર્ભાવ "text.h"

ગણના xkb_parser_error {
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
ગણના xkb_parser_strict_flags {
    PARSER_NO_STRICT_FLAGS = 0,
    PARSER_NO_UNKNOWN_STATEMENTS = (1 << 0),
    PARSER_NO_FIELD_TYPE_MISMATCH = (1 << 1),
    PARSER_NO_FIELD_કિંમત_MISMATCH = (1 << 2),

    PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS = (1 << 3),

    PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS = (1 << 4),
    PARSER_NO_UNKNOWN_TYPE_FIELDS = (1 << 5),

    PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS = (1 << 6),
    PARSER_NO_UNKNOWN_INTERPRET_FIELDS = (1 << 7),
    PARSER_NO_UNKNOWN_LED_FIELDS = (1 << 8),

    PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS = (1 << 9),
    PARSER_NO_UNKNOWN_ચાવી_FIELDS = (1 << 10),

    PARSER_NO_UNKNOWN_ACTION = (1 << 11),
    PARSER_NO_UNKNOWN_ACTION_FIELDS = (1 << 12),
    PARSER_NO_ILLEGAL_ACTION_FIELDS = (1 << 13),

    PARSER_NO_UNKNOWN_SECTION_FLAGS = (1 << 14),

    PARSER_STRICT_FLAGS_ALL = ((PARSER_NO_UNKNOWN_SECTION_FLAGS << 1) - 1),

    PARSER_V1_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    /* Limited flexibility */
    PARSER_V1_LAX_FLAGS = (PARSER_V1_STRICT_FLAGS &
                           ~PARSER_NO_FIELD_કિંમત_MISMATCH),

    PARSER_V2_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    PARSER_V2_LAX_FLAGS = PARSER_NO_STRICT_FLAGS,
};

સ્થિર અંતઃરેખ ગણના xkb_parser_strict_flags
parser_strict_flags_from_keymap_format(ગણના xkb_keymap_format format)
{
    પાછા (format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? PARSER_V1_LAX_FLAGS
            : PARSER_V2_LAX_FLAGS;
}

સ્થિર અંતઃરેખ ગણના xkb_parser_strict_flags
parser_strict_flags_from_keymap(અચલ સંરચના xkb_keymap *keymap)
{
    પાછા (keymap->format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_MODE
                ? PARSER_V1_STRICT_FLAGS
                : PARSER_V1_LAX_FLAGS)
            : (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_MODE
                ? PARSER_V2_STRICT_FLAGS
                : PARSER_V2_LAX_FLAGS);
}

સંરચના parser_keymap_config {
    ગણના xkb_keymap_format format;
    ગણના xkb_parser_strict_flags strict;
};

પ્રકારવ્યાખ્યા સંઘ પદાવલિ_Def પદાવલિ_Def;
સંરચના pending_computation {
    પદાવલિ_Def * પદાવલિ;
    bool computed;
    uint32_t કિંમત;
};
પ્રકારવ્યાખ્યા darray(સંરચના pending_computation) pending_computation_array;

/** Keymap augmented with miscellaneous data used during compilation */
સંરચના xkb_keymap_info {
    /** The keymap being compiled */
    સંરચના xkb_keymap keymap;

    /** Flags of the strict mode */
    ગણના xkb_parser_strict_flags strict;

    /** Features */
    સંરચના {
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
    સંરચના {
        LookupEntry groupIndexNames[3];
        LookupEntry groupMaskNames[5];
    } lookup;

    /** Pending computations */
    pending_computation_array *pending_computations;
};

ગણના xkb_status
text_v1_keymap_serialize(
        અચલ સંરચના xkb_keymap *keymap,
        અચલ સંરચના xkb_keymap_serialize_config *config,
        સંરચના xkb_keymap_serialize_result *result
);

XkbFile *
XkbParseFile(સંરચના xkb_context *ctx,
             અચલ સંરચના parser_keymap_config *config,
             FILE *file, અચલ અક્ષર *file_name, અચલ અક્ષર *map);
bool
XkbParseStringInit(સંરચના xkb_context *ctx, સંરચના scanner *scanner,
                   અચલ અક્ષર *string, size_t len,
                   અચલ અક્ષર *file_name, અચલ અક્ષર *map);

XkbFile *
XkbParseString(સંરચના xkb_context *ctx,
               અચલ સંરચના parser_keymap_config *config,
               અચલ અક્ષર *string, size_t len,
               અચલ અક્ષર *file_name, અચલ અક્ષર *map);

bool
XkbParseStringNext(સંરચના xkb_context *ctx,
                   અચલ સંરચના parser_keymap_config *config,
                   સંરચના scanner *scanner, અચલ અક્ષર *map, XkbFile **out);

શૂન્ય
FreeXkbFile(XkbFile *file);

XkbFile *
XkbFileFromComponents(સંરચના xkb_context *ctx,
                      અચલ સંરચના xkb_component_names *kkctgs);

bool
CompileKeycodes(XkbFile *file, સંરચના xkb_keymap_info *keymap_info);

bool
Compile_ચાવી_Types(XkbFile *file, સંરચના xkb_keymap_info *keymap_info);

bool
CompileCompatMap(XkbFile *file, સંરચના xkb_keymap_info *keymap_info);

bool
CompileSymbols(XkbFile *file, સંરચના xkb_keymap_info *keymap_info);

bool
CompileKeymap(XkbFile *file, સંરચના xkb_keymap *keymap);

/***====================================================================***/

સ્થિર અંતઃરેખ bool
ReportNotArray(સંરચના xkb_context *ctx, અચલ અક્ષર *type, અચલ અક્ષર *field,
               અચલ અક્ષર *name)
{
    log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
            "The %s %s field is not an array; "
            "Ignoring illegal assignment in %s\n",
            type, field, name);
    પાછા false;
}

સ્થિર અંતઃરેખ bool
ReportShouldBeArray(સંરચના xkb_context *ctx, અચલ અક્ષર *type,
                    અચલ અક્ષર *field, અચલ અક્ષર *name)
{
    log_err(ctx, XKB_ERROR_EXPECTED_ARRAY_ENTRY,
            "Missing subscript for %s %s; "
            "Ignoring illegal assignment in %s\n",
            type, field, name);
    પાછા false;
}

સ્થિર અંતઃરેખ bool
ReportBadType(સંરચના xkb_context *ctx, ગણના xkb_message_code code,
              અચલ અક્ષર *type, અચલ અક્ષર *field, અચલ અક્ષર *name,
              અચલ અક્ષર *wanted)
{
    log_err(ctx, code,
            "The %s %s field must be a %s; "
            "Ignoring illegal assignment in %s\n",
            type, field, wanted, name);
    પાછા false;
}

સ્થિર અંતઃરેખ bool
ReportBadField(સંરચના xkb_context *ctx, અચલ અક્ષર *type, અચલ અક્ષર *field,
               અચલ અક્ષર *name)
{
    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Unknown %s field \"%s\" in %s; "
            "Ignoring assignment to unknown field in %s\n",
            type, field, name, name);
    પાછા false;
}

સ્થિર અંતઃરેખ અચલ અક્ષર*
safe_map_name(અચલ અક્ષર* name)
{
    પાછા name ? name : "(unnamed map)";
}
