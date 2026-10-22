/*
 * Copyright © 2009 Dan Nicholson
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Dan Nicholson <dbn.lists@gmail.com>
 * Author: Ran Benita <ran234@gmail.com>
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#ضمن "config.h"

#ضمن <stdbool.h>

#ضمن "darray.h"
#ضمن "utils.h"
#ضمن "xkbcommon/xkbcommon.h"
#ضمن "xkbcomp-priv.h"
#ضمن "keymap.h"
#ضمن "rules.h"
#ضمن "rmlvo.h"

bool
xkb_components_نالا_from_قاعدا(بنية xkb_context *ctx,
                                ثابت بنية xkb_قاعدو_نالا *rmlvo_اندريون,
                                بنية xkb_قاعدو_نالا *rmlvo_ٻاهريون,
                                بنية xkb_component_نالا *components_ٻاهريون)
{
    /* Resolve default RMLVO values. We need a mutable copy of the input. */
    بنية xkb_قاعدو_نالا rmlvo = *rmlvo_اندريون;
    xkb_context_sanitize_قاعدو_نالا(ctx, &rmlvo);

    إذا (rmlvo_ٻاهريون) {
        /* Copy the sanitized RMLVO, if required. */
        *rmlvo_ٻاهريون = rmlvo;
    }
    إذا (!components_ٻاهريون) {
        /* KcCGST not required.
         * While RMLVO and KcCGST outputs are both optional, at least one must
         * be provided. */
        أرجع !!rmlvo_ٻاهريون;
    }

    /* Resolve the RMLVO names to KcCGST components */
    *components_ٻاهريون = (بنية xkb_component_نالا){ 0 };
    أرجع xkb_components_from_قاعدا_نالا(ctx, &rmlvo, components_ٻاهريون, عدم);
}

ساكن bool
compile_keymap_file(بنية xkb_keymap *keymap, XkbFile *file)
{
    إذا (file->file_type != FILE_TYPE_KEYMAP) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Cannot compile a %s file alone into a keymap\n",
                xkb_file_type_منزل_string(file->file_type));
        أرجع false;
    }

    إذا (!CompileKeymap(file, keymap)) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to compile keymap\n");
        أرجع false;
    }

    أرجع true;
}

ساكن bool
متن_v1_keymap_new_from_rmlvo(بنية xkb_keymap *keymap,
                              ثابت بنية xkb_rmlvo_builder *rmlvo)
{
    bool ok;
    بنية xkb_component_نالا kccgst;
    XkbFile *file;

    إذا (keymap->ctx->log_level >= XKB_LOG_LEVEL_DEBUG) {
        بنية xkb_قاعدو_نالا نالا = { 0 };
        ثابت سائيز_t buf_سائيز = حجم(rmlvo->ctx->متن_بفر) - 1;
        حرف *buf = xkb_context_get_بفر(rmlvo->ctx, buf_سائيز);
        إذا (unlikely(!buf))
            أرجع false;
        ok = xkb_rmlvo_builder_منزل_قاعدا_نالا(rmlvo, &نالا, buf, buf_سائيز);
        إذا (unlikely(!ok))
            أرجع false;
        log_dbg(keymap->ctx, XKB_LOG_پيغام_NO_ID,
                "Compiling from RMLVO builder: rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                نالا.قاعدا, نالا.model, نالا.layout, نالا.variant,
                نالا.options);
    }

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rmlvo_builder(rmlvo, &kccgst, &keymap->انگ_groups);
    إذا (!ok) {
        بنية xkb_قاعدو_نالا نالا = { 0 };
        ثابت سائيز_t buf_سائيز = حجم(rmlvo->ctx->متن_بفر);
        حرف *buf = xkb_context_get_بفر(rmlvo->ctx, buf_سائيز);
        إذا (unlikely(!buf))
            أرجع false;
        ok = xkb_rmlvo_builder_منزل_قاعدا_نالا(rmlvo, &نالا, buf, buf_سائيز);
        إذا (unlikely(!ok))
            أرجع false;
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                نالا.قاعدا, نالا.model, نالا.layout, نالا.variant,
                نالا.options);
        أرجع false;
    }

    ثابت xkb_layout_اشاريو_t max_groups = format_max_groups(keymap->format);
    إذا (keymap->انگ_groups > max_groups)
        keymap->انگ_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_پيغام_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    حرر(kccgst.keycodes);
    حرر(kccgst.types);
    حرر(kccgst.compatibility);
    حرر(kccgst.symbols);
    حرر(kccgst.geometry);

    إذا (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        أرجع false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    أرجع ok;
}

ساكن bool
متن_v1_keymap_new_from_نالا(بنية xkb_keymap *keymap,
                              ثابت بنية xkb_قاعدو_نالا *rmlvo)
{
    bool ok;
    بنية xkb_component_نالا kccgst;
    XkbFile *file;

    log_dbg(keymap->ctx, XKB_LOG_پيغام_NO_ID,
            "Compiling from RMLVO: rules '%s', model '%s', layout '%s', "
            "variant '%s', options '%s'\n",
            rmlvo->قاعدا, rmlvo->model, rmlvo->layout, rmlvo->variant,
            rmlvo->options);

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_قاعدا_نالا(keymap->ctx, rmlvo, &kccgst,
                                         &keymap->انگ_groups);
    إذا (!ok) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                rmlvo->قاعدا, rmlvo->model, rmlvo->layout, rmlvo->variant,
                rmlvo->options);
        أرجع false;
    }

    ثابت xkb_layout_اشاريو_t max_groups = format_max_groups(keymap->format);
    إذا (keymap->انگ_groups > max_groups)
        keymap->انگ_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_پيغام_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    حرر(kccgst.keycodes);
    حرر(kccgst.types);
    حرر(kccgst.compatibility);
    حرر(kccgst.symbols);
    حرر(kccgst.geometry);

    إذا (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        أرجع false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    أرجع ok;
}

ساكن bool
متن_v1_keymap_new_from_string(بنية xkb_keymap *keymap,
                               ثابت حرف *string, سائيز_t len)
{
    bool ok;
    XkbFile *xkb_file;

    ثابت بنية parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseString(keymap->ctx, &config,
                              string, len, "(input string)", عدم);
    إذا (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb string\n");
        أرجع false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    أرجع ok;
}

ساكن bool
متن_v1_keymap_new_from_file(بنية xkb_keymap *keymap, FILE *file)
{
    bool ok;
    XkbFile *xkb_file;

    ثابت بنية parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseFile(keymap->ctx, &config, file, "(unknown file)", عدم);
    إذا (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb file\n");
        أرجع false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    أرجع ok;
}

ثابت بنية xkb_keymap_format_ops متن_v1_keymap_format_ops = {
    .keymap_new_from_rmlvo = متن_v1_keymap_new_from_rmlvo,
    .keymap_new_from_نالا = متن_v1_keymap_new_from_نالا,
    .keymap_new_from_string = متن_v1_keymap_new_from_string,
    .keymap_new_from_file = متن_v1_keymap_new_from_file,
    .keymap_serialize = متن_v1_keymap_serialize,
};
