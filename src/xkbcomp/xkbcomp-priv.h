/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "ast.h"
#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "keymap.h"
#അന്തര്ഭാവ "scanner-utils.h"
#അന്തര്ഭാവ "text.h"

എണ്ണൽ xkb_parser_error {
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
എണ്ണൽ xkb_parser_strict_flags {
    PARSER_NO_STRICT_FLAGS = 0,
    PARSER_NO_UNKNOWN_STATEMENTS = (1 << 0),
    PARSER_NO_FIELD_TYPE_MISMATCH = (1 << 1),
    PARSER_NO_FIELD_മൂല്യം_MISMATCH = (1 << 2),

    PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS = (1 << 3),

    PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS = (1 << 4),
    PARSER_NO_UNKNOWN_TYPE_FIELDS = (1 << 5),

    PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS = (1 << 6),
    PARSER_NO_UNKNOWN_INTERPRET_FIELDS = (1 << 7),
    PARSER_NO_UNKNOWN_LED_FIELDS = (1 << 8),

    PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS = (1 << 9),
    PARSER_NO_UNKNOWN_താക്കോൽ_FIELDS = (1 << 10),

    PARSER_NO_UNKNOWN_ACTION = (1 << 11),
    PARSER_NO_UNKNOWN_ACTION_FIELDS = (1 << 12),
    PARSER_NO_ILLEGAL_ACTION_FIELDS = (1 << 13),

    PARSER_NO_UNKNOWN_SECTION_FLAGS = (1 << 14),

    PARSER_STRICT_FLAGS_ALL = ((PARSER_NO_UNKNOWN_SECTION_FLAGS << 1) - 1),

    PARSER_V1_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    /* Limited flexibility */
    PARSER_V1_LAX_FLAGS = (PARSER_V1_STRICT_FLAGS &
                           ~PARSER_NO_FIELD_മൂല്യം_MISMATCH),

    PARSER_V2_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    PARSER_V2_LAX_FLAGS = PARSER_NO_STRICT_FLAGS,
};

സ്ഥിതം വരിയിൽ എണ്ണൽ xkb_parser_strict_flags
parser_strict_flags_from_keymap_format(എണ്ണൽ xkb_keymap_format format)
{
    തിരികെ (format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? PARSER_V1_LAX_FLAGS
            : PARSER_V2_LAX_FLAGS;
}

സ്ഥിതം വരിയിൽ എണ്ണൽ xkb_parser_strict_flags
parser_strict_flags_from_keymap(സ്ഥിരം ഘടന xkb_keymap *keymap)
{
    തിരികെ (keymap->format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_MODE
                ? PARSER_V1_STRICT_FLAGS
                : PARSER_V1_LAX_FLAGS)
            : (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_MODE
                ? PARSER_V2_STRICT_FLAGS
                : PARSER_V2_LAX_FLAGS);
}

ഘടന parser_keymap_config {
    എണ്ണൽ xkb_keymap_format format;
    എണ്ണൽ xkb_parser_strict_flags strict;
};

തരനിർവചനം സംഘം വ്യഞ്ജകം_Def വ്യഞ്ജകം_Def;
ഘടന pending_computation {
    വ്യഞ്ജകം_Def * വ്യഞ്ജകം;
    bool computed;
    uint32_t മൂല്യം;
};
തരനിർവചനം darray(ഘടന pending_computation) pending_computation_array;

/** Keymap augmented with miscellaneous data used during compilation */
ഘടന xkb_keymap_info {
    /** The keymap being compiled */
    ഘടന xkb_keymap keymap;

    /** Flags of the strict mode */
    എണ്ണൽ xkb_parser_strict_flags strict;

    /** Features */
    ഘടന {
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
    ഘടന {
        LookupEntry groupIndexNames[3];
        LookupEntry groupMaskNames[5];
    } lookup;

    /** Pending computations */
    pending_computation_array *pending_computations;
};

എണ്ണൽ xkb_status
text_v1_keymap_serialize(
        സ്ഥിരം ഘടന xkb_keymap *keymap,
        സ്ഥിരം ഘടന xkb_keymap_serialize_config *config,
        ഘടന xkb_keymap_serialize_result *result
);

XkbFile *
XkbParseFile(ഘടന xkb_context *ctx,
             സ്ഥിരം ഘടന parser_keymap_config *config,
             FILE *file, സ്ഥിരം അക്ഷരം *file_name, സ്ഥിരം അക്ഷരം *map);
bool
XkbParseStringInit(ഘടന xkb_context *ctx, ഘടന scanner *scanner,
                   സ്ഥിരം അക്ഷരം *string, size_t len,
                   സ്ഥിരം അക്ഷരം *file_name, സ്ഥിരം അക്ഷരം *map);

XkbFile *
XkbParseString(ഘടന xkb_context *ctx,
               സ്ഥിരം ഘടന parser_keymap_config *config,
               സ്ഥിരം അക്ഷരം *string, size_t len,
               സ്ഥിരം അക്ഷരം *file_name, സ്ഥിരം അക്ഷരം *map);

bool
XkbParseStringNext(ഘടന xkb_context *ctx,
                   സ്ഥിരം ഘടന parser_keymap_config *config,
                   ഘടന scanner *scanner, സ്ഥിരം അക്ഷരം *map, XkbFile **out);

ശൂന്യം
FreeXkbFile(XkbFile *file);

XkbFile *
XkbFileFromComponents(ഘടന xkb_context *ctx,
                      സ്ഥിരം ഘടന xkb_component_names *kkctgs);

bool
CompileKeycodes(XkbFile *file, ഘടന xkb_keymap_info *keymap_info);

bool
Compile_താക്കോൽ_Types(XkbFile *file, ഘടന xkb_keymap_info *keymap_info);

bool
CompileCompatMap(XkbFile *file, ഘടന xkb_keymap_info *keymap_info);

bool
CompileSymbols(XkbFile *file, ഘടന xkb_keymap_info *keymap_info);

bool
CompileKeymap(XkbFile *file, ഘടന xkb_keymap *keymap);

/***====================================================================***/

സ്ഥിതം വരിയിൽ bool
ReportNotArray(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം *type, സ്ഥിരം അക്ഷരം *field,
               സ്ഥിരം അക്ഷരം *name)
{
    log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
            "The %s %s field is not an array; "
            "Ignoring illegal assignment in %s\n",
            type, field, name);
    തിരികെ false;
}

സ്ഥിതം വരിയിൽ bool
ReportShouldBeArray(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം *type,
                    സ്ഥിരം അക്ഷരം *field, സ്ഥിരം അക്ഷരം *name)
{
    log_err(ctx, XKB_ERROR_EXPECTED_ARRAY_ENTRY,
            "Missing subscript for %s %s; "
            "Ignoring illegal assignment in %s\n",
            type, field, name);
    തിരികെ false;
}

സ്ഥിതം വരിയിൽ bool
ReportBadType(ഘടന xkb_context *ctx, എണ്ണൽ xkb_message_code code,
              സ്ഥിരം അക്ഷരം *type, സ്ഥിരം അക്ഷരം *field, സ്ഥിരം അക്ഷരം *name,
              സ്ഥിരം അക്ഷരം *wanted)
{
    log_err(ctx, code,
            "The %s %s field must be a %s; "
            "Ignoring illegal assignment in %s\n",
            type, field, wanted, name);
    തിരികെ false;
}

സ്ഥിതം വരിയിൽ bool
ReportBadField(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം *type, സ്ഥിരം അക്ഷരം *field,
               സ്ഥിരം അക്ഷരം *name)
{
    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Unknown %s field \"%s\" in %s; "
            "Ignoring assignment to unknown field in %s\n",
            type, field, name, name);
    തിരികെ false;
}

സ്ഥിതം വരിയിൽ സ്ഥിരം അക്ഷരം*
safe_map_name(സ്ഥിരം അക്ഷരം* name)
{
    തിരികെ name ? name : "(unnamed map)";
}
