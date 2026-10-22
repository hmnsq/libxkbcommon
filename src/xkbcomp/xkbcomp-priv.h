/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */
#ہدایت once

#شامل "config.h"

#شامل "xkbcommon/xkbcommon.h"
#شامل "ast.h"
#شامل "darray.h"
#شامل "keymap.h"
#شامل "scanner-utils.h"
#شامل "text.h"

شمار xkb_parser_error {
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
شمار xkb_parser_strict_flags {
    PARSER_NO_STRICT_FLAGS = 0,
    PARSER_NO_UNKNOWN_STATEMENTS = (1 << 0),
    PARSER_NO_میدان_TYPE_MISMATCH = (1 << 1),
    PARSER_NO_میدان_قدر_MISMATCH = (1 << 2),

    PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS = (1 << 3),

    PARSER_NO_UNKNOWN_TYPES_GLOBAL_FIELDS = (1 << 4),
    PARSER_NO_UNKNOWN_TYPE_FIELDS = (1 << 5),

    PARSER_NO_UNKNOWN_COMPAT_GLOBAL_FIELDS = (1 << 6),
    PARSER_NO_UNKNOWN_INTERPRET_FIELDS = (1 << 7),
    PARSER_NO_UNKNOWN_LED_FIELDS = (1 << 8),

    PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS = (1 << 9),
    PARSER_NO_UNKNOWN_کلید_FIELDS = (1 << 10),

    PARSER_NO_UNKNOWN_ACTION = (1 << 11),
    PARSER_NO_UNKNOWN_ACTION_FIELDS = (1 << 12),
    PARSER_NO_ILLEGAL_ACTION_FIELDS = (1 << 13),

    PARSER_NO_UNKNOWN_SECTION_FLAGS = (1 << 14),

    PARSER_STRICT_FLAGS_ALL = ((PARSER_NO_UNKNOWN_SECTION_FLAGS << 1) - 1),

    PARSER_V1_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    /* Limited flexibility */
    PARSER_V1_LAX_FLAGS = (PARSER_V1_STRICT_FLAGS &
                           ~PARSER_NO_میدان_قدر_MISMATCH),

    PARSER_V2_STRICT_FLAGS = PARSER_STRICT_FLAGS_ALL,
    PARSER_V2_LAX_FLAGS = PARSER_NO_STRICT_FLAGS,
};

ساکن اندرونی شمار xkb_parser_strict_flags
parser_strict_flags_from_keymap_format(شمار xkb_keymap_format format)
{
    واپس (format == XKB_KEYMAP_FORMAT_متن_V1)
            ? PARSER_V1_LAX_FLAGS
            : PARSER_V2_LAX_FLAGS;
}

ساکن اندرونی شمار xkb_parser_strict_flags
parser_strict_flags_from_keymap(ثابت ساخت xkb_keymap *keymap)
{
    واپس (keymap->format == XKB_KEYMAP_FORMAT_متن_V1)
            ? (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_منوال
                ? PARSER_V1_STRICT_FLAGS
                : PARSER_V1_LAX_FLAGS)
            : (keymap->flags & XKB_KEYMAP_COMPILE_STRICT_منوال
                ? PARSER_V2_STRICT_FLAGS
                : PARSER_V2_LAX_FLAGS);
}

ساخت parser_keymap_config {
    شمار xkb_keymap_format format;
    شمار xkb_parser_strict_flags strict;
};

قسمتعریف اتحاد اظہاریہ_Def اظہاریہ_Def;
ساخت pending_computation {
    اظہاریہ_Def * اظہاریہ;
    bool computed;
    uint32_t قدر;
};
قسمتعریف darray(ساخت pending_computation) pending_computation_array;

/** Keymap augmented with miscellaneous data used during compilation */
ساخت xkb_keymap_info {
    /** The keymap being compiled */
    ساخت xkb_keymap keymap;

    /** Flags of the strict mode */
    شمار xkb_parser_strict_flags strict;

    /** Features */
    ساخت {
        /** Maximum groups for the keymap format */
        xkb_layout_اشاریہ_t max_groups;
        /** Maximum overlays for the keymap format */
        xkb_overlay_اشاریہ_t max_overlays;
        /** Offset of the Controls names LUT for the keymap format */
        uint8_t controls_name_انحراف;
        /* Actions */
        bool گروہ_تالا_on_release;
        bool mods_unlock_on_press;
        bool mods_latch_on_press;
        /* Overlays */
        bool overlapping_overlays;
    } features;

    /* Non-static LUTs */
    ساخت {
        Lookup_اندراج گروہ_اشاریہ_نام[3];
        Lookup_اندراج گروہ_نقاب_نام[5];
    } lookup;

    /** Pending computations */
    pending_computation_array *pending_computations;
};

شمار xkb_حالت
متن_v1_keymap_serialize(
        ثابت ساخت xkb_keymap *keymap,
        ثابت ساخت xkb_keymap_serialize_config *config,
        ساخت xkb_keymap_serialize_نتیجہ *نتیجہ
);

XkbFile *
XkbParseFile(ساخت xkb_context *ctx,
             ثابت ساخت parser_keymap_config *config,
             FILE *file, ثابت حرف *file_name, ثابت حرف *map);
bool
XkbParseStringInit(ساخت xkb_context *ctx, ساخت scanner *scanner,
                   ثابت حرف *string, سائز_t len,
                   ثابت حرف *file_name, ثابت حرف *map);

XkbFile *
XkbParseString(ساخت xkb_context *ctx,
               ثابت ساخت parser_keymap_config *config,
               ثابت حرف *string, سائز_t len,
               ثابت حرف *file_name, ثابت حرف *map);

bool
Xkb_Parse_String_اگلا(ساخت xkb_context *ctx,
                   ثابت ساخت parser_keymap_config *config,
                   ساخت scanner *scanner, ثابت حرف *map, XkbFile **برآمد);

خالی
FreeXkbFile(XkbFile *file);

XkbFile *
XkbFileFromComponents(ساخت xkb_context *ctx,
                      ثابت ساخت xkb_component_نام *kkctgs);

bool
CompileKeycodes(XkbFile *file, ساخت xkb_keymap_info *keymap_info);

bool
Compile_کلید_Types(XkbFile *file, ساخت xkb_keymap_info *keymap_info);

bool
CompileCompatMap(XkbFile *file, ساخت xkb_keymap_info *keymap_info);

bool
CompileSymbols(XkbFile *file, ساخت xkb_keymap_info *keymap_info);

bool
CompileKeymap(XkbFile *file, ساخت xkb_keymap *keymap);

/***====================================================================***/

ساکن اندرونی bool
ReportNotArray(ساخت xkb_context *ctx, ثابت حرف *type, ثابت حرف *میدان_2,
               ثابت حرف *name)
{
    log_err(ctx, XKB_ERROR_WRONG_میدان_TYPE,
            "The %s %s field is not an array; "
            "Ignoring illegal assignment in %s\n",
            type, میدان_2, name);
    واپس false;
}

ساکن اندرونی bool
ReportShouldBeArray(ساخت xkb_context *ctx, ثابت حرف *type,
                    ثابت حرف *میدان_2, ثابت حرف *name)
{
    log_err(ctx, XKB_ERROR_EXPECTED_ARRAY_اندراج,
            "Missing subscript for %s %s; "
            "Ignoring illegal assignment in %s\n",
            type, میدان_2, name);
    واپس false;
}

ساکن اندرونی bool
ReportBadType(ساخت xkb_context *ctx, شمار xkb_پیغام_رمز رمز,
              ثابت حرف *type, ثابت حرف *میدان_2, ثابت حرف *name,
              ثابت حرف *wanted)
{
    log_err(ctx, رمز,
            "The %s %s field must be a %s; "
            "Ignoring illegal assignment in %s\n",
            type, میدان_2, wanted, name);
    واپس false;
}

ساکن اندرونی bool
Report_Bad_میدان(ساخت xkb_context *ctx, ثابت حرف *type, ثابت حرف *میدان_2,
               ثابت حرف *name)
{
    log_err(ctx, XKB_LOG_پیغام_NO_ID,
            "Unknown %s field \"%s\" in %s; "
            "Ignoring assignment to unknown field in %s\n",
            type, میدان_2, name, name);
    واپس false;
}

ساکن اندرونی ثابت حرف*
محفوظ_map_name(ثابت حرف* name)
{
    واپس name ? name : "(unnamed map)";
}
