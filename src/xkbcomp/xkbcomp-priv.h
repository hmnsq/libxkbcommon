/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ast.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "scanner-utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "text.h"

ਗਿਣਤੀ xkb_parser_error {
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
ਗਿਣਤੀ xkb_parser_strict_flags {
    PARSER_NO_STRICT_FLAGS = 0,
    PARSER_NO_UNKNOWN_STATEMENTS = (1 << 0),
    PARSER_NO_ਖੇਤਰ_TYPE_MISMATCH = (1 << 1),
    PARSER_NO_ਖੇਤਰ_ਮੁੱਲ_MISMATCH = (1 << 2),

    PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS = (1 << 3),

    PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS = (1 << 4),
    PARSER_NO_UNKNOWN_TYPE_FIELDS = (1 << 5),

    PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS = (1 << 6),
    PARSER_NO_UNKNOWN_INTERPRET_FIELDS = (1 << 7),
    PARSER_NO_UNKNOWN_LED_FIELDS = (1 << 8),

    PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS = (1 << 9),
    PARSER_NO_UNKNOWN_ਕੁੰਜੀ_FIELDS = (1 << 10),

    PARSER_NO_UNKNOWN_ACTION = (1 << 11),
    PARSER_NO_UNKNOWN_ACTION_FIELDS = (1 << 12),
    PARSER_NO_ILLEGAL_ACTION_FIELDS = (1 << 13),

    PARSER_NO_UNKNOWN_SECTION_FLAGS = (1 << 14),

    PARSER_STRICT_FLAGS_ALL = ((PARSER_NO_UNKNOWN_SECTION_FLAGS << 1) - 1),

    PARSER_V1_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    /* Limited flexibility */
    PARSER_V1_LAX_FLAGS = (PARSER_V1_STRICT_FLAGS &
                           ~PARSER_NO_ਖੇਤਰ_ਮੁੱਲ_MISMATCH),

    PARSER_V2_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    PARSER_V2_LAX_FLAGS = PARSER_NO_STRICT_FLAGS,
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਗਿਣਤੀ xkb_parser_strict_flags
parser_strict_flags_from_keymap_format(ਗਿਣਤੀ xkb_keymap_format format)
{
    ਵਾਪਸ (format == XKB_KEYMAP_FORMAT_ਪਾਠ_V1)
            ? PARSER_V1_LAX_FLAGS
            : PARSER_V2_LAX_FLAGS;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਗਿਣਤੀ xkb_parser_strict_flags
parser_strict_flags_from_keymap(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap)
{
    ਵਾਪਸ (keymap->format == XKB_KEYMAP_FORMAT_ਪਾਠ_V1)
            ? (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_ਬਹੁਲਕ
                ? PARSER_V1_STRICT_FLAGS
                : PARSER_V1_LAX_FLAGS)
            : (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_ਬਹੁਲਕ
                ? PARSER_V2_STRICT_FLAGS
                : PARSER_V2_LAX_FLAGS);
}

ਬਣਤਰ parser_keymap_config {
    ਗਿਣਤੀ xkb_keymap_format format;
    ਗਿਣਤੀ xkb_parser_strict_flags strict;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਸੰਘ ਵਿਅੰਜਕ_Def ਵਿਅੰਜਕ_Def;
ਬਣਤਰ pending_computation {
    ਵਿਅੰਜਕ_Def * ਵਿਅੰਜਕ;
    bool computed;
    uint32_t ਮੁੱਲ;
};
ਕਿਸਮਪਰਿਭਾਸ਼ਾ darray(ਬਣਤਰ pending_computation) pending_computation_array;

/** Keymap augmented with miscellaneous data used during compilation */
ਬਣਤਰ xkb_keymap_info {
    /** The keymap being compiled */
    ਬਣਤਰ xkb_keymap keymap;

    /** Flags of the strict mode */
    ਗਿਣਤੀ xkb_parser_strict_flags strict;

    /** Features */
    ਬਣਤਰ {
        /** Maximum groups for the keymap format */
        xkb_layout_ਸੂਚਕਾਂਕ_t max_groups;
        /** Maximum overlays for the keymap format */
        xkb_overlay_ਸੂਚਕਾਂਕ_t max_overlays;
        /** Offset of the Controls names LUT for the keymap format */
        uint8_t controls_name_ਵਿੱਥ;
        /* Actions */
        bool ਸਮੂਹ_ਤਾਲਾ_on_release;
        bool mods_unlock_on_press;
        bool mods_latch_on_press;
        /* Overlays */
        bool overlapping_overlays;
    } features;

    /* Non-static LUTs */
    ਬਣਤਰ {
        Lookup_ਇੰਦਰਾਜ ਸਮੂਹ_ਸੂਚਕਾਂਕ_ਨਾਂ[3];
        Lookup_ਇੰਦਰਾਜ ਸਮੂਹ_ਨਕਾਬ_ਨਾਂ[5];
    } lookup;

    /** Pending computations */
    pending_computation_array *pending_computations;
};

ਗਿਣਤੀ xkb_ਸਥਿਤੀ
ਪਾਠ_v1_keymap_serialize(
        ਅਚਲ ਬਣਤਰ xkb_keymap *keymap,
        ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_config *config,
        ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ *ਨਤੀਜਾ
);

XkbFile *
XkbParseFile(ਬਣਤਰ xkb_context *ctx,
             ਅਚਲ ਬਣਤਰ parser_keymap_config *config,
             FILE *file, ਅਚਲ ਅੱਖਰ *file_name, ਅਚਲ ਅੱਖਰ *map);
bool
XkbParseStringInit(ਬਣਤਰ xkb_context *ctx, ਬਣਤਰ scanner *scanner,
                   ਅਚਲ ਅੱਖਰ *string, ਮਾਪ_t len,
                   ਅਚਲ ਅੱਖਰ *file_name, ਅਚਲ ਅੱਖਰ *map);

XkbFile *
XkbParseString(ਬਣਤਰ xkb_context *ctx,
               ਅਚਲ ਬਣਤਰ parser_keymap_config *config,
               ਅਚਲ ਅੱਖਰ *string, ਮਾਪ_t len,
               ਅਚਲ ਅੱਖਰ *file_name, ਅਚਲ ਅੱਖਰ *map);

bool
Xkb_Parse_String_ਅਗਲਾ(ਬਣਤਰ xkb_context *ctx,
                   ਅਚਲ ਬਣਤਰ parser_keymap_config *config,
                   ਬਣਤਰ scanner *scanner, ਅਚਲ ਅੱਖਰ *map, XkbFile **ਬਾਹਰਲਾ);

ਖਾਲੀ
FreeXkbFile(XkbFile *file);

XkbFile *
XkbFileFromComponents(ਬਣਤਰ xkb_context *ctx,
                      ਅਚਲ ਬਣਤਰ xkb_component_ਨਾਂ *kkctgs);

bool
CompileKeycodes(XkbFile *file, ਬਣਤਰ xkb_keymap_info *keymap_info);

bool
Compile_ਕੁੰਜੀ_Types(XkbFile *file, ਬਣਤਰ xkb_keymap_info *keymap_info);

bool
CompileCompatMap(XkbFile *file, ਬਣਤਰ xkb_keymap_info *keymap_info);

bool
CompileSymbols(XkbFile *file, ਬਣਤਰ xkb_keymap_info *keymap_info);

bool
CompileKeymap(XkbFile *file, ਬਣਤਰ xkb_keymap *keymap);

/***====================================================================***/

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ReportNotArray(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ *type, ਅਚਲ ਅੱਖਰ *ਖੇਤਰ_2,
               ਅਚਲ ਅੱਖਰ *name)
{
    log_err(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE,
            "The %s %s field is not an array; "
            "Ignoring illegal assignment in %s\n",
            type, ਖੇਤਰ_2, name);
    ਵਾਪਸ false;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ReportShouldBeArray(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ *type,
                    ਅਚਲ ਅੱਖਰ *ਖੇਤਰ_2, ਅਚਲ ਅੱਖਰ *name)
{
    log_err(ctx, XKB_ERROR_EXPECTED_ARRAY_ਇੰਦਰਾਜ,
            "Missing subscript for %s %s; "
            "Ignoring illegal assignment in %s\n",
            type, ਖੇਤਰ_2, name);
    ਵਾਪਸ false;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ReportBadType(ਬਣਤਰ xkb_context *ctx, ਗਿਣਤੀ xkb_ਸੁਨੇਹਾ_ਕੋਡ ਕੋਡ,
              ਅਚਲ ਅੱਖਰ *type, ਅਚਲ ਅੱਖਰ *ਖੇਤਰ_2, ਅਚਲ ਅੱਖਰ *name,
              ਅਚਲ ਅੱਖਰ *wanted)
{
    log_err(ctx, ਕੋਡ,
            "The %s %s field must be a %s; "
            "Ignoring illegal assignment in %s\n",
            type, ਖੇਤਰ_2, wanted, name);
    ਵਾਪਸ false;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
Report_Bad_ਖੇਤਰ(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ *type, ਅਚਲ ਅੱਖਰ *ਖੇਤਰ_2,
               ਅਚਲ ਅੱਖਰ *name)
{
    log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
            "Unknown %s field \"%s\" in %s; "
            "Ignoring assignment to unknown field in %s\n",
            type, ਖੇਤਰ_2, name, name);
    ਵਾਪਸ false;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ ਅੱਖਰ*
ਸੁਰੱਖਿਅਤ_map_name(ਅਚਲ ਅੱਖਰ* name)
{
    ਵਾਪਸ name ? name : "(unnamed map)";
}
