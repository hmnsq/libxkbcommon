/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "ast.h"
#అన్తర్భావ "darray.h"
#అన్తర్భావ "keymap.h"
#అన్తర్భావ "scanner-utils.h"
#అన్తర్భావ "text.h"

గణన xkb_parser_error {
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
గణన xkb_parser_strict_flags {
    PARSER_NO_STRICT_FLAGS = 0,
    PARSER_NO_UNKNOWN_STATEMENTS = (1 << 0),
    PARSER_NO_క్షేత్రం_TYPE_MISMATCH = (1 << 1),
    PARSER_NO_క్షేత్రం_విలువ_MISMATCH = (1 << 2),

    PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS = (1 << 3),

    PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS = (1 << 4),
    PARSER_NO_UNKNOWN_TYPE_FIELDS = (1 << 5),

    PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS = (1 << 6),
    PARSER_NO_UNKNOWN_INTERPRET_FIELDS = (1 << 7),
    PARSER_NO_UNKNOWN_LED_FIELDS = (1 << 8),

    PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS = (1 << 9),
    PARSER_NO_UNKNOWN_కీలకం_FIELDS = (1 << 10),

    PARSER_NO_UNKNOWN_ACTION = (1 << 11),
    PARSER_NO_UNKNOWN_ACTION_FIELDS = (1 << 12),
    PARSER_NO_ILLEGAL_ACTION_FIELDS = (1 << 13),

    PARSER_NO_UNKNOWN_SECTION_FLAGS = (1 << 14),

    PARSER_STRICT_FLAGS_ALL = ((PARSER_NO_UNKNOWN_SECTION_FLAGS << 1) - 1),

    PARSER_V1_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    /* Limited flexibility */
    PARSER_V1_LAX_FLAGS = (PARSER_V1_STRICT_FLAGS &
                           ~PARSER_NO_క్షేత్రం_విలువ_MISMATCH),

    PARSER_V2_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    PARSER_V2_LAX_FLAGS = PARSER_NO_STRICT_FLAGS,
};

స్థిర అంతఃవరుస గణన xkb_parser_strict_flags
parser_strict_flags_from_keymap_format(గణన xkb_keymap_format format)
{
    తిరిగి (format == XKB_KEYMAP_FORMAT_పాఠ్యం_V1)
            ? PARSER_V1_LAX_FLAGS
            : PARSER_V2_LAX_FLAGS;
}

స్థిర అంతఃవరుస గణన xkb_parser_strict_flags
parser_strict_flags_from_keymap(స్థిరరాశి నిర్మాణం xkb_keymap *keymap)
{
    తిరిగి (keymap->format == XKB_KEYMAP_FORMAT_పాఠ్యం_V1)
            ? (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_బహుళకం
                ? PARSER_V1_STRICT_FLAGS
                : PARSER_V1_LAX_FLAGS)
            : (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_బహుళకం
                ? PARSER_V2_STRICT_FLAGS
                : PARSER_V2_LAX_FLAGS);
}

నిర్మాణం parser_keymap_config {
    గణన xkb_keymap_format format;
    గణన xkb_parser_strict_flags strict;
};

రకనిర్వచనం సంఘం సమాసం_Def సమాసం_Def;
నిర్మాణం pending_computation {
    సమాసం_Def * సమాసం;
    bool computed;
    uint32_t విలువ;
};
రకనిర్వచనం darray(నిర్మాణం pending_computation) pending_computation_array;

/** Keymap augmented with miscellaneous data used during compilation */
నిర్మాణం xkb_keymap_info {
    /** The keymap being compiled */
    నిర్మాణం xkb_keymap keymap;

    /** Flags of the strict mode */
    గణన xkb_parser_strict_flags strict;

    /** Features */
    నిర్మాణం {
        /** Maximum groups for the keymap format */
        xkb_layout_సూచీ_t max_groups;
        /** Maximum overlays for the keymap format */
        xkb_overlay_సూచీ_t max_overlays;
        /** Offset of the Controls names LUT for the keymap format */
        uint8_t controls_name_స్థానభ్రంశం;
        /* Actions */
        bool సమూహం_తాళం_on_release;
        bool mods_unlock_on_press;
        bool mods_latch_on_press;
        /* Overlays */
        bool overlapping_overlays;
    } features;

    /* Non-static LUTs */
    నిర్మాణం {
        Lookup_నమోదు సమూహం_సూచీ_పేర్లు[3];
        Lookup_నమోదు సమూహం_ముసుగు_పేర్లు[5];
    } lookup;

    /** Pending computations */
    pending_computation_array *pending_computations;
};

గణన xkb_స్థితి
పాఠ్యం_v1_keymap_serialize(
        స్థిరరాశి నిర్మాణం xkb_keymap *keymap,
        స్థిరరాశి నిర్మాణం xkb_keymap_serialize_config *config,
        నిర్మాణం xkb_keymap_serialize_ఫలితం *ఫలితం
);

XkbFile *
XkbParseFile(నిర్మాణం xkb_context *ctx,
             స్థిరరాశి నిర్మాణం parser_keymap_config *config,
             FILE *file, స్థిరరాశి అక్షరం *file_name, స్థిరరాశి అక్షరం *map);
bool
XkbParseStringInit(నిర్మాణం xkb_context *ctx, నిర్మాణం scanner *scanner,
                   స్థిరరాశి అక్షరం *string, కొలత_t len,
                   స్థిరరాశి అక్షరం *file_name, స్థిరరాశి అక్షరం *map);

XkbFile *
XkbParseString(నిర్మాణం xkb_context *ctx,
               స్థిరరాశి నిర్మాణం parser_keymap_config *config,
               స్థిరరాశి అక్షరం *string, కొలత_t len,
               స్థిరరాశి అక్షరం *file_name, స్థిరరాశి అక్షరం *map);

bool
Xkb_Parse_String_తదుపరి(నిర్మాణం xkb_context *ctx,
                   స్థిరరాశి నిర్మాణం parser_keymap_config *config,
                   నిర్మాణం scanner *scanner, స్థిరరాశి అక్షరం *map, XkbFile **బహిర్గతం);

శూన్య
FreeXkbFile(XkbFile *file);

XkbFile *
XkbFileFromComponents(నిర్మాణం xkb_context *ctx,
                      స్థిరరాశి నిర్మాణం xkb_component_పేర్లు *kkctgs);

bool
CompileKeycodes(XkbFile *file, నిర్మాణం xkb_keymap_info *keymap_info);

bool
Compile_కీలకం_Types(XkbFile *file, నిర్మాణం xkb_keymap_info *keymap_info);

bool
CompileCompatMap(XkbFile *file, నిర్మాణం xkb_keymap_info *keymap_info);

bool
CompileSymbols(XkbFile *file, నిర్మాణం xkb_keymap_info *keymap_info);

bool
CompileKeymap(XkbFile *file, నిర్మాణం xkb_keymap *keymap);

/***====================================================================***/

స్థిర అంతఃవరుస bool
ReportNotArray(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం *type, స్థిరరాశి అక్షరం *క్షేత్రం_2,
               స్థిరరాశి అక్షరం *name)
{
    log_err(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE,
            "The %s %s field is not an array; "
            "Ignoring illegal assignment in %s\n",
            type, క్షేత్రం_2, name);
    తిరిగి false;
}

స్థిర అంతఃవరుస bool
ReportShouldBeArray(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం *type,
                    స్థిరరాశి అక్షరం *క్షేత్రం_2, స్థిరరాశి అక్షరం *name)
{
    log_err(ctx, XKB_ERROR_EXPECTED_ARRAY_నమోదు,
            "Missing subscript for %s %s; "
            "Ignoring illegal assignment in %s\n",
            type, క్షేత్రం_2, name);
    తిరిగి false;
}

స్థిర అంతఃవరుస bool
ReportBadType(నిర్మాణం xkb_context *ctx, గణన xkb_సందేశం_సంకేతం సంకేతం_2,
              స్థిరరాశి అక్షరం *type, స్థిరరాశి అక్షరం *క్షేత్రం_2, స్థిరరాశి అక్షరం *name,
              స్థిరరాశి అక్షరం *wanted)
{
    log_err(ctx, సంకేతం_2,
            "The %s %s field must be a %s; "
            "Ignoring illegal assignment in %s\n",
            type, క్షేత్రం_2, wanted, name);
    తిరిగి false;
}

స్థిర అంతఃవరుస bool
Report_Bad_క్షేత్రం(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం *type, స్థిరరాశి అక్షరం *క్షేత్రం_2,
               స్థిరరాశి అక్షరం *name)
{
    log_err(ctx, XKB_LOG_సందేశం_NO_ID,
            "Unknown %s field \"%s\" in %s; "
            "Ignoring assignment to unknown field in %s\n",
            type, క్షేత్రం_2, name, name);
    తిరిగి false;
}

స్థిర అంతఃవరుస స్థిరరాశి అక్షరం*
సురక్షిత_map_name(స్థిరరాశి అక్షరం* name)
{
    తిరిగి name ? name : "(unnamed map)";
}
