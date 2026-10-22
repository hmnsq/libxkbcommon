/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#لارښوونه once

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
    PARSER_NO_ډګر_TYPE_MISMATCH = (1 << 1),
    PARSER_NO_ډګر_ارزښت_MISMATCH = (1 << 2),

    PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS = (1 << 3),

    PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS = (1 << 4),
    PARSER_NO_UNKNOWN_TYPE_FIELDS = (1 << 5),

    PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS = (1 << 6),
    PARSER_NO_UNKNOWN_INTERPRET_FIELDS = (1 << 7),
    PARSER_NO_UNKNOWN_LED_FIELDS = (1 << 8),

    PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS = (1 << 9),
    PARSER_NO_UNKNOWN_کلی_FIELDS = (1 << 10),

    PARSER_NO_UNKNOWN_ACTION = (1 << 11),
    PARSER_NO_UNKNOWN_ACTION_FIELDS = (1 << 12),
    PARSER_NO_ILLEGAL_ACTION_FIELDS = (1 << 13),

    PARSER_NO_UNKNOWN_SECTION_FLAGS = (1 << 14),

    PARSER_STRICT_FLAGS_ALL = ((PARSER_NO_UNKNOWN_SECTION_FLAGS << 1) - 1),

    PARSER_V1_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    /* Limited flexibility */
    PARSER_V1_LAX_FLAGS = (PARSER_V1_STRICT_FLAGS &
                           ~PARSER_NO_ډګر_ارزښت_MISMATCH),

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
            ? (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_ډېرپېښ
                ? PARSER_V1_STRICT_FLAGS
                : PARSER_V1_LAX_FLAGS)
            : (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_ډېرپېښ
                ? PARSER_V2_STRICT_FLAGS
                : PARSER_V2_LAX_FLAGS);
}

بنية parser_keymap_config {
    تعداد xkb_keymap_format format;
    تعداد xkb_parser_strict_flags strict;
};

تعريفنوع اتحاد بیان_Def بیان_Def;
بنية pending_computation {
    بیان_Def * بیان;
    bool computed;
    uint32_t ارزښت;
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
        xkb_layout_شاخص_t max_groups;
        /** Maximum overlays for the keymap format */
        xkb_overlay_شاخص_t max_overlays;
        /** Offset of the Controls names LUT for the keymap format */
        uint8_t controls_name_دننه_واټن;
        /* Actions */
        bool ډله_قلف_on_release;
        bool mods_unlock_on_press;
        bool mods_latch_on_press;
        /* Overlays */
        bool overlapping_overlays;
    } features;

    /* Non-static LUTs */
    بنية {
        Lookup_ثبت ډله_شاخص_نومونه[3];
        Lookup_ثبت ډله_ماسک_نومونه[5];
    } lookup;

    /** Pending computations */
    pending_computation_array *pending_computations;
};

تعداد xkb_حالت
متن_v1_keymap_serialize(
        ثابت بنية xkb_keymap *keymap,
        ثابت بنية xkb_keymap_serialize_config *config,
        بنية xkb_keymap_serialize_پایله *پایله
);

XkbFile *
XkbParseFile(بنية xkb_context *ctx,
             ثابت بنية parser_keymap_config *config,
             FILE *file, ثابت حرف *file_name, ثابت حرف *map);
bool
XkbParseStringInit(بنية xkb_context *ctx, بنية scanner *scanner,
                   ثابت حرف *string, اندازه_t len,
                   ثابت حرف *file_name, ثابت حرف *map);

XkbFile *
XkbParseString(بنية xkb_context *ctx,
               ثابت بنية parser_keymap_config *config,
               ثابت حرف *string, اندازه_t len,
               ثابت حرف *file_name, ثابت حرف *map);

bool
Xkb_Parse_String_بل(بنية xkb_context *ctx,
                   ثابت بنية parser_keymap_config *config,
                   بنية scanner *scanner, ثابت حرف *map, XkbFile **بهر);

فراغ
FreeXkbFile(XkbFile *file);

XkbFile *
XkbFileFromComponents(بنية xkb_context *ctx,
                      ثابت بنية xkb_component_نومونه *kkctgs);

bool
CompileKeycodes(XkbFile *file, بنية xkb_keymap_info *keymap_info);

bool
Compile_کلی_Types(XkbFile *file, بنية xkb_keymap_info *keymap_info);

bool
CompileCompatMap(XkbFile *file, بنية xkb_keymap_info *keymap_info);

bool
CompileSymbols(XkbFile *file, بنية xkb_keymap_info *keymap_info);

bool
CompileKeymap(XkbFile *file, بنية xkb_keymap *keymap);

/***====================================================================***/

ساكن مضمن bool
ReportNotArray(بنية xkb_context *ctx, ثابت حرف *type, ثابت حرف *ډګر_2,
               ثابت حرف *name)
{
    log_err(ctx, XKB_ERROR_WRONG_ډګر_TYPE,
            "The %s %s field is not an array; "
            "Ignoring illegal assignment in %s\n",
            type, ډګر_2, name);
    أرجع false;
}

ساكن مضمن bool
ReportShouldBeArray(بنية xkb_context *ctx, ثابت حرف *type,
                    ثابت حرف *ډګر_2, ثابت حرف *name)
{
    log_err(ctx, XKB_ERROR_EXPECTED_ARRAY_ثبت,
            "Missing subscript for %s %s; "
            "Ignoring illegal assignment in %s\n",
            type, ډګر_2, name);
    أرجع false;
}

ساكن مضمن bool
ReportBadType(بنية xkb_context *ctx, تعداد xkb_پیغام_کوډ کوډ,
              ثابت حرف *type, ثابت حرف *ډګر_2, ثابت حرف *name,
              ثابت حرف *wanted)
{
    log_err(ctx, کوډ,
            "The %s %s field must be a %s; "
            "Ignoring illegal assignment in %s\n",
            type, ډګر_2, wanted, name);
    أرجع false;
}

ساكن مضمن bool
Report_Bad_ډګر(بنية xkb_context *ctx, ثابت حرف *type, ثابت حرف *ډګر_2,
               ثابت حرف *name)
{
    log_err(ctx, XKB_LOG_پیغام_NO_ID,
            "Unknown %s field \"%s\" in %s; "
            "Ignoring assignment to unknown field in %s\n",
            type, ډګر_2, name, name);
    أرجع false;
}

ساكن مضمن ثابت حرف*
خوندي_map_name(ثابت حرف* name)
{
    أرجع name ? name : "(unnamed map)";
}
