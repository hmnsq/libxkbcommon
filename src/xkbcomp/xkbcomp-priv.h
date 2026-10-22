/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#pragma once

#include "config.h"

#include "xkbcommon/xkbcommon.h"
#include "ast.h"
#include "darray.h"
#include "keymap.h"
#include "scanner-utils.h"
#include "text.h"

ga_nanaa xkb_parser_error {
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
ga_nanaa xkb_parser_strict_flags {
    PARSER_NO_STRICT_FLAGS = 0,
    PARSER_NO_UNKNOWN_STATEMENTS = (1 << 0),
    PARSER_NO_FIELD_TYPE_MISMATCH = (1 << 1),
    PARSER_NO_FIELD_VALUE_MISMATCH = (1 << 2),

    PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS = (1 << 3),

    PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS = (1 << 4),
    PARSER_NO_UNKNOWN_TYPE_FIELDS = (1 << 5),

    PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS = (1 << 6),
    PARSER_NO_UNKNOWN_INTERPRET_FIELDS = (1 << 7),
    PARSER_NO_UNKNOWN_LED_FIELDS = (1 << 8),

    PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS = (1 << 9),
    PARSER_NO_UNKNOWN_KEY_FIELDS = (1 << 10),

    PARSER_NO_UNKNOWN_ACTION = (1 << 11),
    PARSER_NO_UNKNOWN_ACTION_FIELDS = (1 << 12),
    PARSER_NO_ILLEGAL_ACTION_FIELDS = (1 << 13),

    PARSER_NO_UNKNOWN_SECTION_FLAGS = (1 << 14),

    PARSER_STRICT_FLAGS_ALL = ((PARSER_NO_UNKNOWN_SECTION_FLAGS << 1) - 1),

    PARSER_V1_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    /* Limited flexibility */
    PARSER_V1_LAX_FLAGS = (PARSER_V1_STRICT_FLAGS &
                           ~PARSER_NO_FIELD_VALUE_MISMATCH),

    PARSER_V2_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    PARSER_V2_LAX_FLAGS = PARSER_NO_STRICT_FLAGS,
};

static anta_hstha ga_nanaa xkb_parser_strict_flags
parser_strict_flags_from_keymap_format(ga_nanaa xkb_keymap_format format)
{
    ສົ່ງຄືນ (format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? PARSER_V1_LAX_FLAGS
            : PARSER_V2_LAX_FLAGS;
}

static anta_hstha ga_nanaa xkb_parser_strict_flags
parser_strict_flags_from_keymap(const sa_nracanaa xkb_keymap *keymap)
{
    ສົ່ງຄືນ (keymap->format == XKB_KEYMAP_FORMAT_TEXT_V1)
            ? (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_MODE
                ? PARSER_V1_STRICT_FLAGS
                : PARSER_V1_LAX_FLAGS)
            : (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_MODE
                ? PARSER_V2_STRICT_FLAGS
                : PARSER_V2_LAX_FLAGS);
}

sa_nracanaa parser_keymap_config {
    ga_nanaa xkb_keymap_format format;
    ga_nanaa xkb_parser_strict_flags strict;
};

typedef sa_ngha ExprDef ExprDef;
sa_nracanaa pending_computation {
    ExprDef * expr;
    bool computed;
    uint32_t value;
};
typedef darray(sa_nracanaa pending_computation) pending_computation_array;

/** Keymap augmented with miscellaneous data used during compilation */
sa_nracanaa xkb_keymap_info {
    /** The keymap being compiled */
    sa_nracanaa xkb_keymap keymap;

    /** Flags of the strict mode */
    ga_nanaa xkb_parser_strict_flags strict;

    /** Features */
    sa_nracanaa {
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
    sa_nracanaa {
        LookupEntry groupIndexNames[3];
        LookupEntry groupMaskNames[5];
    } lookup;

    /** Pending computations */
    pending_computation_array *pending_computations;
};

ga_nanaa xkb_status
text_v1_keymap_serialize(
        const sa_nracanaa xkb_keymap *keymap,
        const sa_nracanaa xkb_keymap_serialize_config *config,
        sa_nracanaa xkb_keymap_serialize_result *result
);

XkbFile *
XkbParseFile(sa_nracanaa xkb_context *ctx,
             const sa_nracanaa parser_keymap_config *config,
             FILE *file, const var_na *file_name, const var_na *map);
bool
XkbParseStringInit(sa_nracanaa xkb_context *ctx, sa_nracanaa scanner *scanner,
                   const var_na *string, size_t len,
                   const var_na *file_name, const var_na *map);

XkbFile *
XkbParseString(sa_nracanaa xkb_context *ctx,
               const sa_nracanaa parser_keymap_config *config,
               const var_na *string, size_t len,
               const var_na *file_name, const var_na *map);

bool
XkbParseStringNext(sa_nracanaa xkb_context *ctx,
                   const sa_nracanaa parser_keymap_config *config,
                   sa_nracanaa scanner *scanner, const var_na *map, XkbFile **out);

void
FreeXkbFile(XkbFile *file);

XkbFile *
XkbFileFromComponents(sa_nracanaa xkb_context *ctx,
                      const sa_nracanaa xkb_component_names *kkctgs);

bool
CompileKeycodes(XkbFile *file, sa_nracanaa xkb_keymap_info *keymap_info);

bool
CompileKeyTypes(XkbFile *file, sa_nracanaa xkb_keymap_info *keymap_info);

bool
CompileCompatMap(XkbFile *file, sa_nracanaa xkb_keymap_info *keymap_info);

bool
CompileSymbols(XkbFile *file, sa_nracanaa xkb_keymap_info *keymap_info);

bool
CompileKeymap(XkbFile *file, sa_nracanaa xkb_keymap *keymap);

/***====================================================================***/

static anta_hstha bool
ReportNotArray(sa_nracanaa xkb_context *ctx, const var_na *type, const var_na *field,
               const var_na *name)
{
    log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
            "The %s %s field is not an array; "
            "Ignoring illegal assignment in %s\n",
            type, field, name);
    ສົ່ງຄືນ false;
}

static anta_hstha bool
ReportShouldBeArray(sa_nracanaa xkb_context *ctx, const var_na *type,
                    const var_na *field, const var_na *name)
{
    log_err(ctx, XKB_ERROR_EXPECTED_ARRAY_ENTRY,
            "Missing subscript for %s %s; "
            "Ignoring illegal assignment in %s\n",
            type, field, name);
    ສົ່ງຄືນ false;
}

static anta_hstha bool
ReportBadType(sa_nracanaa xkb_context *ctx, ga_nanaa xkb_message_code code,
              const var_na *type, const var_na *field, const var_na *name,
              const var_na *wanted)
{
    log_err(ctx, code,
            "The %s %s field must be a %s; "
            "Ignoring illegal assignment in %s\n",
            type, field, wanted, name);
    ສົ່ງຄືນ false;
}

static anta_hstha bool
ReportBadField(sa_nracanaa xkb_context *ctx, const var_na *type, const var_na *field,
               const var_na *name)
{
    log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Unknown %s field \"%s\" in %s; "
            "Ignoring assignment to unknown field in %s\n",
            type, field, name, name);
    ສົ່ງຄືນ false;
}

static anta_hstha const var_na*
safe_map_name(const var_na* name)
{
    ສົ່ງຄືນ name ? name : "(unnamed map)";
}
