/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#هدايت once

#ضمن "config.h"

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "ast.h"
#ضمن "darray.h"
#ضمن "keymap.h"
#ضمن "scanner-utils.h"
#ضمن "text.h"

تعداد xkb_parser_error {
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
تعداد xkb_parser_strict_flags {
    PARSER_NO_STRICT_FLAGS = 0,
    PARSER_NO_UNKNOWN_STATEMENTS = (1 << 0),
    PARSER_NO_ميدان_TYPE_MISMATCH = (1 << 1),
    PARSER_NO_ميدان_قدر_MISMATCH = (1 << 2),

    PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS = (1 << 3),

    PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS = (1 << 4),
    PARSER_NO_UNKNOWN_TYPE_FIELDS = (1 << 5),

    PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS = (1 << 6),
    PARSER_NO_UNKNOWN_INTERPRET_FIELDS = (1 << 7),
    PARSER_NO_UNKNOWN_LED_FIELDS = (1 << 8),

    PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS = (1 << 9),
    PARSER_NO_UNKNOWN_ڪنجي_FIELDS = (1 << 10),

    PARSER_NO_UNKNOWN_ACTION = (1 << 11),
    PARSER_NO_UNKNOWN_ACTION_FIELDS = (1 << 12),
    PARSER_NO_ILLEGAL_ACTION_FIELDS = (1 << 13),

    PARSER_NO_UNKNOWN_SECTION_FLAGS = (1 << 14),

    PARSER_STRICT_FLAGS_ALL = ((PARSER_NO_UNKNOWN_SECTION_FLAGS << 1) - 1),

    PARSER_V1_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    /* Limited flexibility */
    PARSER_V1_LAX_FLAGS = (PARSER_V1_STRICT_FLAGS &
                           ~PARSER_NO_ميدان_قدر_MISMATCH),

    PARSER_V2_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    PARSER_V2_LAX_FLAGS = PARSER_NO_STRICT_FLAGS,
};

ساكن مضمن تعداد xkb_parser_strict_flags
parser_strict_flags_from_keymap_format(تعداد xkb_keymap_format format)
{
    أرجع (format == XKB_KEYMAP_FORMAT_متن_V1)
            ? PARSER_V1_LAX_FLAGS
            : PARSER_V2_LAX_FLAGS;
}

ساكن مضمن تعداد xkb_parser_strict_flags
parser_strict_flags_from_keymap(ثابت بنية xkb_keymap *keymap)
{
    أرجع (keymap->format == XKB_KEYMAP_FORMAT_متن_V1)
            ? (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_گهڻائي
                ? PARSER_V1_STRICT_FLAGS
                : PARSER_V1_LAX_FLAGS)
            : (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_گهڻائي
                ? PARSER_V2_STRICT_FLAGS
                : PARSER_V2_LAX_FLAGS);
}

بنية parser_keymap_config {
    تعداد xkb_keymap_format format;
    تعداد xkb_parser_strict_flags strict;
};

تعريفنوع اتحاد اظهار_Def اظهار_Def;
بنية pending_computation {
    اظهار_Def * اظهار;
    bool computed;
    uint32_t قدر;
};
تعريفنوع darray(بنية pending_computation) pending_computation_array;

/** Keymap augmented with miscellaneous data used during compilation */
بنية xkb_keymap_info {
    /** The keymap being compiled */
    بنية xkb_keymap keymap;

    /** Flags of the strict mode */
    تعداد xkb_parser_strict_flags strict;

    /** Features */
    بنية {
        /** Maximum groups for the keymap format */
        xkb_layout_اشاريو_t max_groups;
        /** Maximum overlays for the keymap format */
        xkb_overlay_اشاريو_t max_overlays;
        /** Offset of the Controls names LUT for the keymap format */
        uint8_t controls_name_وٿي;
        /* Actions */
        bool گروهه_تالو_on_release;
        bool mods_unlock_on_press;
        bool mods_latch_on_press;
        /* Overlays */
        bool overlapping_overlays;
    } features;

    /* Non-static LUTs */
    بنية {
        Lookup_داخلا گروهه_اشاريو_نالا[3];
        Lookup_داخلا گروهه_نقاب_نالا[5];
    } lookup;

    /** Pending computations */
    pending_computation_array *pending_computations;
};

تعداد xkb_حالت
متن_v1_keymap_serialize(
        ثابت بنية xkb_keymap *keymap,
        ثابت بنية xkb_keymap_serialize_config *config,
        بنية xkb_keymap_serialize_نتيجو *نتيجو
);

XkbFile *
XkbParseFile(بنية xkb_context *ctx,
             ثابت بنية parser_keymap_config *config,
             FILE *file, ثابت حرف *file_name, ثابت حرف *map);
bool
XkbParseStringInit(بنية xkb_context *ctx, بنية scanner *scanner,
                   ثابت حرف *string, سائيز_t len,
                   ثابت حرف *file_name, ثابت حرف *map);

XkbFile *
XkbParseString(بنية xkb_context *ctx,
               ثابت بنية parser_keymap_config *config,
               ثابت حرف *string, سائيز_t len,
               ثابت حرف *file_name, ثابت حرف *map);

bool
Xkb_Parse_String_ايندڙ(بنية xkb_context *ctx,
                   ثابت بنية parser_keymap_config *config,
                   بنية scanner *scanner, ثابت حرف *map, XkbFile **ٻاهريون);

فراغ
FreeXkbFile(XkbFile *file);

XkbFile *
XkbFileFromComponents(بنية xkb_context *ctx,
                      ثابت بنية xkb_component_نالا *kkctgs);

bool
CompileKeycodes(XkbFile *file, بنية xkb_keymap_info *keymap_info);

bool
Compile_ڪنجي_Types(XkbFile *file, بنية xkb_keymap_info *keymap_info);

bool
CompileCompatMap(XkbFile *file, بنية xkb_keymap_info *keymap_info);

bool
CompileSymbols(XkbFile *file, بنية xkb_keymap_info *keymap_info);

bool
CompileKeymap(XkbFile *file, بنية xkb_keymap *keymap);

/***====================================================================***/

ساكن مضمن bool
ReportNotArray(بنية xkb_context *ctx, ثابت حرف *type, ثابت حرف *ميدان_2,
               ثابت حرف *name)
{
    log_err(ctx, XKB_ERROR_WRONG_ميدان_TYPE,
            "The %s %s field is not an array; "
            "Ignoring illegal assignment in %s\n",
            type, ميدان_2, name);
    أرجع false;
}

ساكن مضمن bool
ReportShouldBeArray(بنية xkb_context *ctx, ثابت حرف *type,
                    ثابت حرف *ميدان_2, ثابت حرف *name)
{
    log_err(ctx, XKB_ERROR_EXPECTED_ARRAY_داخلا,
            "Missing subscript for %s %s; "
            "Ignoring illegal assignment in %s\n",
            type, ميدان_2, name);
    أرجع false;
}

ساكن مضمن bool
ReportBadType(بنية xkb_context *ctx, تعداد xkb_پيغام_ڪوڊ ڪوڊ,
              ثابت حرف *type, ثابت حرف *ميدان_2, ثابت حرف *name,
              ثابت حرف *wanted)
{
    log_err(ctx, ڪوڊ,
            "The %s %s field must be a %s; "
            "Ignoring illegal assignment in %s\n",
            type, ميدان_2, wanted, name);
    أرجع false;
}

ساكن مضمن bool
Report_Bad_ميدان(بنية xkb_context *ctx, ثابت حرف *type, ثابت حرف *ميدان_2,
               ثابت حرف *name)
{
    log_err(ctx, XKB_LOG_پيغام_NO_ID,
            "Unknown %s field \"%s\" in %s; "
            "Ignoring assignment to unknown field in %s\n",
            type, ميدان_2, name, name);
    أرجع false;
}

ساكن مضمن ثابت حرف*
محفوظ_map_name(ثابت حرف* name)
{
    أرجع name ? name : "(unnamed map)";
}
