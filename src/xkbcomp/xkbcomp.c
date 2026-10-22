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

#شامل "config.h"

#شامل <stdbool.h>

#شامل "darray.h"
#شامل "utils.h"
#شامل "xkbcommon/xkbcommon.h"
#شامل "xkbcomp-priv.h"
#شامل "keymap.h"
#شامل "rules.h"
#شامل "rmlvo.h"

bool
xkb_components_نام_from_قواعد(ساخت xkb_context *ctx,
                                ثابت ساخت xkb_قاعدہ_نام *rmlvo_اندر,
                                ساخت xkb_قاعدہ_نام *rmlvo_برآمد,
                                ساخت xkb_component_نام *components_برآمد)
{
    /* Resolve default RMLVO values. We need a mutable copy of the input. */
    ساخت xkb_قاعدہ_نام rmlvo = *rmlvo_اندر;
    xkb_context_sanitize_قاعدہ_نام(ctx, &rmlvo);

    اگر (rmlvo_برآمد) {
        /* Copy the sanitized RMLVO, if required. */
        *rmlvo_برآمد = rmlvo;
    }
    اگر (!components_برآمد) {
        /* KcCGST not required.
         * While RMLVO and KcCGST outputs are both optional, at least one must
         * be provided. */
        واپس !!rmlvo_برآمد;
    }

    /* Resolve the RMLVO names to KcCGST components */
    *components_برآمد = (ساخت xkb_component_نام){ 0 };
    واپس xkb_components_from_قواعد_نام(ctx, &rmlvo, components_برآمد, تہی);
}

ساکن bool
compile_keymap_file(ساخت xkb_keymap *keymap, XkbFile *file)
{
    اگر (file->file_type != FILE_TYPE_KEYMAP) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Cannot compile a %s file alone into a keymap\n",
                xkb_file_type_مقصد_string(file->file_type));
        واپس false;
    }

    اگر (!CompileKeymap(file, keymap)) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to compile keymap\n");
        واپس false;
    }

    واپس true;
}

ساکن bool
متن_v1_keymap_new_from_rmlvo(ساخت xkb_keymap *keymap,
                              ثابت ساخت xkb_rmlvo_builder *rmlvo)
{
    bool ok;
    ساخت xkb_component_نام kccgst;
    XkbFile *file;

    اگر (keymap->ctx->log_level >= XKB_LOG_LEVEL_DEBUG) {
        ساخت xkb_قاعدہ_نام نام = { 0 };
        ثابت سائز_t buf_سائز = حجم(rmlvo->ctx->متن_بفر) - 1;
        حرف *buf = xkb_context_get_بفر(rmlvo->ctx, buf_سائز);
        اگر (unlikely(!buf))
            واپس false;
        ok = xkb_rmlvo_builder_مقصد_قواعد_نام(rmlvo, &نام, buf, buf_سائز);
        اگر (unlikely(!ok))
            واپس false;
        log_dbg(keymap->ctx, XKB_LOG_پیغام_NO_ID,
                "Compiling from RMLVO builder: rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                نام.قواعد, نام.model, نام.layout, نام.variant,
                نام.options);
    }

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rmlvo_builder(rmlvo, &kccgst, &keymap->عدد_groups);
    اگر (!ok) {
        ساخت xkb_قاعدہ_نام نام = { 0 };
        ثابت سائز_t buf_سائز = حجم(rmlvo->ctx->متن_بفر);
        حرف *buf = xkb_context_get_بفر(rmlvo->ctx, buf_سائز);
        اگر (unlikely(!buf))
            واپس false;
        ok = xkb_rmlvo_builder_مقصد_قواعد_نام(rmlvo, &نام, buf, buf_سائز);
        اگر (unlikely(!ok))
            واپس false;
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                نام.قواعد, نام.model, نام.layout, نام.variant,
                نام.options);
        واپس false;
    }

    ثابت xkb_layout_اشاریہ_t max_groups = format_max_groups(keymap->format);
    اگر (keymap->عدد_groups > max_groups)
        keymap->عدد_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_پیغام_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    آزاد(kccgst.keycodes);
    آزاد(kccgst.types);
    آزاد(kccgst.compatibility);
    آزاد(kccgst.symbols);
    آزاد(kccgst.geometry);

    اگر (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        واپس false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    واپس ok;
}

ساکن bool
متن_v1_keymap_new_from_نام(ساخت xkb_keymap *keymap,
                              ثابت ساخت xkb_قاعدہ_نام *rmlvo)
{
    bool ok;
    ساخت xkb_component_نام kccgst;
    XkbFile *file;

    log_dbg(keymap->ctx, XKB_LOG_پیغام_NO_ID,
            "Compiling from RMLVO: rules '%s', model '%s', layout '%s', "
            "variant '%s', options '%s'\n",
            rmlvo->قواعد, rmlvo->model, rmlvo->layout, rmlvo->variant,
            rmlvo->options);

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_قواعد_نام(keymap->ctx, rmlvo, &kccgst,
                                         &keymap->عدد_groups);
    اگر (!ok) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                rmlvo->قواعد, rmlvo->model, rmlvo->layout, rmlvo->variant,
                rmlvo->options);
        واپس false;
    }

    ثابت xkb_layout_اشاریہ_t max_groups = format_max_groups(keymap->format);
    اگر (keymap->عدد_groups > max_groups)
        keymap->عدد_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_پیغام_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    آزاد(kccgst.keycodes);
    آزاد(kccgst.types);
    آزاد(kccgst.compatibility);
    آزاد(kccgst.symbols);
    آزاد(kccgst.geometry);

    اگر (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        واپس false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    واپس ok;
}

ساکن bool
متن_v1_keymap_new_from_string(ساخت xkb_keymap *keymap,
                               ثابت حرف *string, سائز_t len)
{
    bool ok;
    XkbFile *xkb_file;

    ثابت ساخت parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseString(keymap->ctx, &config,
                              string, len, "(input string)", تہی);
    اگر (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb string\n");
        واپس false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    واپس ok;
}

ساکن bool
متن_v1_keymap_new_from_file(ساخت xkb_keymap *keymap, FILE *file)
{
    bool ok;
    XkbFile *xkb_file;

    ثابت ساخت parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseFile(keymap->ctx, &config, file, "(unknown file)", تہی);
    اگر (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb file\n");
        واپس false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    واپس ok;
}

ثابت ساخت xkb_keymap_format_ops متن_v1_keymap_format_ops = {
    .keymap_new_from_rmlvo = متن_v1_keymap_new_from_rmlvo,
    .keymap_new_from_نام = متن_v1_keymap_new_from_نام,
    .keymap_new_from_string = متن_v1_keymap_new_from_string,
    .keymap_new_from_file = متن_v1_keymap_new_from_file,
    .keymap_serialize = متن_v1_keymap_serialize,
};
