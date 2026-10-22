/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "ast.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "scanner-utils.h"
#ಅನ್ತರ್ಭಾವ "text.h"

ಎಣಿಕೆ xkb_parser_error {
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
ಎಣಿಕೆ xkb_parser_strict_flags {
    PARSER_NO_STRICT_FLAGS = 0,
    PARSER_NO_UNKNOWN_STATEMENTS = (1 << 0),
    PARSER_NO_ಕ್ಷೇತ್ರ_TYPE_MISMATCH = (1 << 1),
    PARSER_NO_ಕ್ಷೇತ್ರ_ಮೌಲ್ಯ_MISMATCH = (1 << 2),

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
                           ~PARSER_NO_ಕ್ಷೇತ್ರ_ಮೌಲ್ಯ_MISMATCH),

    PARSER_V2_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    PARSER_V2_LAX_FLAGS = PARSER_NO_STRICT_FLAGS,
};

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಎಣಿಕೆ xkb_parser_strict_flags
parser_strict_flags_from_keymap_format(ಎಣಿಕೆ xkb_keymap_format format)
{
    ಹಿಂತಿರುಗಿ (format == XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1)
            ? PARSER_V1_LAX_FLAGS
            : PARSER_V2_LAX_FLAGS;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಎಣಿಕೆ xkb_parser_strict_flags
parser_strict_flags_from_keymap(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap)
{
    ಹಿಂತಿರುಗಿ (keymap->format == XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1)
            ? (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_ಬಹುಲಕ
                ? PARSER_V1_STRICT_FLAGS
                : PARSER_V1_LAX_FLAGS)
            : (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_ಬಹುಲಕ
                ? PARSER_V2_STRICT_FLAGS
                : PARSER_V2_LAX_FLAGS);
}

ರಚನೆ parser_keymap_config {
    ಎಣಿಕೆ xkb_keymap_format format;
    ಎಣಿಕೆ xkb_parser_strict_flags strict;
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಒಕ್ಕೂಟ ಅಭಿವ್ಯಕ್ತಿ_Def ಅಭಿವ್ಯಕ್ತಿ_Def;
ರಚನೆ pending_computation {
    ಅಭಿವ್ಯಕ್ತಿ_Def * ಅಭಿವ್ಯಕ್ತಿ;
    bool computed;
    uint32_t ಮೌಲ್ಯ;
};
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ darray(ರಚನೆ pending_computation) pending_computation_array;

/** Keymap augmented with miscellaneous data used during compilation */
ರಚನೆ xkb_keymap_info {
    /** The keymap being compiled */
    ರಚನೆ xkb_keymap keymap;

    /** Flags of the strict mode */
    ಎಣಿಕೆ xkb_parser_strict_flags strict;

    /** Features */
    ರಚನೆ {
        /** Maximum groups for the keymap format */
        xkb_layout_ಸೂಚ್ಯಂಕ_t max_groups;
        /** Maximum overlays for the keymap format */
        xkb_overlay_ಸೂಚ್ಯಂಕ_t max_overlays;
        /** Offset of the Controls names LUT for the keymap format */
        uint8_t controls_name_ಸ್ಥಾನಾಂತರ;
        /* Actions */
        bool ಗುಂಪು_ಬೀಗ_on_release;
        bool mods_unlock_on_press;
        bool mods_latch_on_press;
        /* Overlays */
        bool overlapping_overlays;
    } features;

    /* Non-static LUTs */
    ರಚನೆ {
        Lookup_ನಮೂದು ಗುಂಪು_ಸೂಚ್ಯಂಕ_ಹೆಸರುಗಳು[3];
        Lookup_ನಮೂದು ಗುಂಪು_ಮುಖವಾಡ_ಹೆಸರುಗಳು[5];
    } lookup;

    /** Pending computations */
    pending_computation_array *pending_computations;
};

ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
ಪಠ್ಯ_v1_keymap_serialize(
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap,
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_config *config,
        ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ *ಫಲಿತಾಂಶ
);

XkbFile *
XkbParseFile(ರಚನೆ xkb_context *ctx,
             ಸ್ಥಿರಾಂಕ ರಚನೆ parser_keymap_config *config,
             FILE *file, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file_name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *map);
bool
XkbParseStringInit(ರಚನೆ xkb_context *ctx, ರಚನೆ scanner *scanner,
                   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string, ಗಾತ್ರ_t len,
                   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file_name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *map);

XkbFile *
XkbParseString(ರಚನೆ xkb_context *ctx,
               ಸ್ಥಿರಾಂಕ ರಚನೆ parser_keymap_config *config,
               ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string, ಗಾತ್ರ_t len,
               ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file_name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *map);

bool
Xkb_Parse_String_ಮುಂದಿನ(ರಚನೆ xkb_context *ctx,
                   ಸ್ಥಿರಾಂಕ ರಚನೆ parser_keymap_config *config,
                   ರಚನೆ scanner *scanner, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *map, XkbFile **ಹೊರಹೋಗು);

ಶೂನ್ಯ
FreeXkbFile(XkbFile *file);

XkbFile *
XkbFileFromComponents(ರಚನೆ xkb_context *ctx,
                      ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_component_ಹೆಸರುಗಳು *kkctgs);

bool
CompileKeycodes(XkbFile *file, ರಚನೆ xkb_keymap_info *keymap_info);

bool
Compile_ಕೀಲಿ_Types(XkbFile *file, ರಚನೆ xkb_keymap_info *keymap_info);

bool
CompileCompatMap(XkbFile *file, ರಚನೆ xkb_keymap_info *keymap_info);

bool
CompileSymbols(XkbFile *file, ರಚನೆ xkb_keymap_info *keymap_info);

bool
CompileKeymap(XkbFile *file, ರಚನೆ xkb_keymap *keymap);

/***====================================================================***/

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ReportNotArray(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *type, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ_2,
               ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
    log_err(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE,
            "The %s %s field is not an array; "
            "Ignoring illegal assignment in %s\n",
            type, ಕ್ಷೇತ್ರ_2, name);
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ReportShouldBeArray(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *type,
                    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
    log_err(ctx, XKB_ERROR_EXPECTED_ARRAY_ನಮೂದು,
            "Missing subscript for %s %s; "
            "Ignoring illegal assignment in %s\n",
            type, ಕ್ಷೇತ್ರ_2, name);
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ReportBadType(ರಚನೆ xkb_context *ctx, ಎಣಿಕೆ xkb_ಸಂದೇಶ_ಸಂಕೇತ ಸಂಕೇತ_2,
              ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *type, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
              ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *wanted)
{
    log_err(ctx, ಸಂಕೇತ_2,
            "The %s %s field must be a %s; "
            "Ignoring illegal assignment in %s\n",
            type, ಕ್ಷೇತ್ರ_2, wanted, name);
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
Report_Bad_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *type, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ_2,
               ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
    log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
            "Unknown %s field \"%s\" in %s; "
            "Ignoring assignment to unknown field in %s\n",
            type, ಕ್ಷೇತ್ರ_2, name, name);
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*
ಸುರಕ್ಷಿತ_map_name(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name)
{
    ಹಿಂತಿರುಗಿ name ? name : "(unnamed map)";
}
