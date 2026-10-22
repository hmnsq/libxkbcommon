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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp-priv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rules.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rmlvo.h"

bool
xkb_components_ਨਾਂ_from_ਨਿਯਮਾਂ(ਬਣਤਰ xkb_context *ctx,
                                ਅਚਲ ਬਣਤਰ xkb_ਨਿਯਮ_ਨਾਂ *rmlvo_ਵਿੱਚ,
                                ਬਣਤਰ xkb_ਨਿਯਮ_ਨਾਂ *rmlvo_ਬਾਹਰਲਾ,
                                ਬਣਤਰ xkb_component_ਨਾਂ *components_ਬਾਹਰਲਾ)
{
    /* Resolve default RMLVO values. We need a mutable copy of the input. */
    ਬਣਤਰ xkb_ਨਿਯਮ_ਨਾਂ rmlvo = *rmlvo_ਵਿੱਚ;
    xkb_context_sanitize_ਨਿਯਮ_ਨਾਂ(ctx, &rmlvo);

    ਜੇ (rmlvo_ਬਾਹਰਲਾ) {
        /* Copy the sanitized RMLVO, if required. */
        *rmlvo_ਬਾਹਰਲਾ = rmlvo;
    }
    ਜੇ (!components_ਬਾਹਰਲਾ) {
        /* KcCGST not required.
         * While RMLVO and KcCGST outputs are both optional, at least one must
         * be provided. */
        ਵਾਪਸ !!rmlvo_ਬਾਹਰਲਾ;
    }

    /* Resolve the RMLVO names to KcCGST components */
    *components_ਬਾਹਰਲਾ = (ਬਣਤਰ xkb_component_ਨਾਂ){ 0 };
    ਵਾਪਸ xkb_components_from_ਨਿਯਮਾਂ_ਨਾਂ(ctx, &rmlvo, components_ਬਾਹਰਲਾ, ਨਿਰਾ);
}

ਸਥਿਰ bool
compile_keymap_file(ਬਣਤਰ xkb_keymap *keymap, XkbFile *file)
{
    ਜੇ (file->file_type != FILE_TYPE_KEYMAP) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Cannot compile a %s file alone into a keymap\n",
                xkb_file_type_ਮੰਜ਼ਿਲ_string(file->file_type));
        ਵਾਪਸ false;
    }

    ਜੇ (!CompileKeymap(file, keymap)) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to compile keymap\n");
        ਵਾਪਸ false;
    }

    ਵਾਪਸ true;
}

ਸਥਿਰ bool
ਪਾਠ_v1_keymap_new_from_rmlvo(ਬਣਤਰ xkb_keymap *keymap,
                              ਅਚਲ ਬਣਤਰ xkb_rmlvo_builder *rmlvo)
{
    bool ok;
    ਬਣਤਰ xkb_component_ਨਾਂ kccgst;
    XkbFile *file;

    ਜੇ (keymap->ctx->log_level >= XKB_LOG_LEVEL_DEBUG) {
        ਬਣਤਰ xkb_ਨਿਯਮ_ਨਾਂ ਨਾਂ = { 0 };
        ਅਚਲ ਮਾਪ_t buf_ਮਾਪ = ਆਕਾਰ(rmlvo->ctx->ਪਾਠ_ਬਫਰ) - 1;
        ਅੱਖਰ *buf = xkb_context_get_ਬਫਰ(rmlvo->ctx, buf_ਮਾਪ);
        ਜੇ (unlikely(!buf))
            ਵਾਪਸ false;
        ok = xkb_rmlvo_builder_ਮੰਜ਼ਿਲ_ਨਿਯਮਾਂ_ਨਾਂ(rmlvo, &ਨਾਂ, buf, buf_ਮਾਪ);
        ਜੇ (unlikely(!ok))
            ਵਾਪਸ false;
        log_dbg(keymap->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Compiling from RMLVO builder: rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                ਨਾਂ.ਨਿਯਮਾਂ, ਨਾਂ.model, ਨਾਂ.layout, ਨਾਂ.variant,
                ਨਾਂ.options);
    }

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rmlvo_builder(rmlvo, &kccgst, &keymap->ਅੰਕ_groups);
    ਜੇ (!ok) {
        ਬਣਤਰ xkb_ਨਿਯਮ_ਨਾਂ ਨਾਂ = { 0 };
        ਅਚਲ ਮਾਪ_t buf_ਮਾਪ = ਆਕਾਰ(rmlvo->ctx->ਪਾਠ_ਬਫਰ);
        ਅੱਖਰ *buf = xkb_context_get_ਬਫਰ(rmlvo->ctx, buf_ਮਾਪ);
        ਜੇ (unlikely(!buf))
            ਵਾਪਸ false;
        ok = xkb_rmlvo_builder_ਮੰਜ਼ਿਲ_ਨਿਯਮਾਂ_ਨਾਂ(rmlvo, &ਨਾਂ, buf, buf_ਮਾਪ);
        ਜੇ (unlikely(!ok))
            ਵਾਪਸ false;
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                ਨਾਂ.ਨਿਯਮਾਂ, ਨਾਂ.model, ਨਾਂ.layout, ਨਾਂ.variant,
                ਨਾਂ.options);
        ਵਾਪਸ false;
    }

    ਅਚਲ xkb_layout_ਸੂਚਕਾਂਕ_t max_groups = format_max_groups(keymap->format);
    ਜੇ (keymap->ਅੰਕ_groups > max_groups)
        keymap->ਅੰਕ_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    ਮੁਕਤ(kccgst.keycodes);
    ਮੁਕਤ(kccgst.types);
    ਮੁਕਤ(kccgst.compatibility);
    ਮੁਕਤ(kccgst.symbols);
    ਮੁਕਤ(kccgst.geometry);

    ਜੇ (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        ਵਾਪਸ false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    ਵਾਪਸ ok;
}

ਸਥਿਰ bool
ਪਾਠ_v1_keymap_new_from_ਨਾਂ(ਬਣਤਰ xkb_keymap *keymap,
                              ਅਚਲ ਬਣਤਰ xkb_ਨਿਯਮ_ਨਾਂ *rmlvo)
{
    bool ok;
    ਬਣਤਰ xkb_component_ਨਾਂ kccgst;
    XkbFile *file;

    log_dbg(keymap->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
            "Compiling from RMLVO: rules '%s', model '%s', layout '%s', "
            "variant '%s', options '%s'\n",
            rmlvo->ਨਿਯਮਾਂ, rmlvo->model, rmlvo->layout, rmlvo->variant,
            rmlvo->options);

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_ਨਿਯਮਾਂ_ਨਾਂ(keymap->ctx, rmlvo, &kccgst,
                                         &keymap->ਅੰਕ_groups);
    ਜੇ (!ok) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                rmlvo->ਨਿਯਮਾਂ, rmlvo->model, rmlvo->layout, rmlvo->variant,
                rmlvo->options);
        ਵਾਪਸ false;
    }

    ਅਚਲ xkb_layout_ਸੂਚਕਾਂਕ_t max_groups = format_max_groups(keymap->format);
    ਜੇ (keymap->ਅੰਕ_groups > max_groups)
        keymap->ਅੰਕ_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    ਮੁਕਤ(kccgst.keycodes);
    ਮੁਕਤ(kccgst.types);
    ਮੁਕਤ(kccgst.compatibility);
    ਮੁਕਤ(kccgst.symbols);
    ਮੁਕਤ(kccgst.geometry);

    ਜੇ (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        ਵਾਪਸ false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    ਵਾਪਸ ok;
}

ਸਥਿਰ bool
ਪਾਠ_v1_keymap_new_from_string(ਬਣਤਰ xkb_keymap *keymap,
                               ਅਚਲ ਅੱਖਰ *string, ਮਾਪ_t len)
{
    bool ok;
    XkbFile *xkb_file;

    ਅਚਲ ਬਣਤਰ parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseString(keymap->ctx, &config,
                              string, len, "(input string)", ਨਿਰਾ);
    ਜੇ (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb string\n");
        ਵਾਪਸ false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    ਵਾਪਸ ok;
}

ਸਥਿਰ bool
ਪਾਠ_v1_keymap_new_from_file(ਬਣਤਰ xkb_keymap *keymap, FILE *file)
{
    bool ok;
    XkbFile *xkb_file;

    ਅਚਲ ਬਣਤਰ parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseFile(keymap->ctx, &config, file, "(unknown file)", ਨਿਰਾ);
    ਜੇ (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb file\n");
        ਵਾਪਸ false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    ਵਾਪਸ ok;
}

ਅਚਲ ਬਣਤਰ xkb_keymap_format_ops ਪਾਠ_v1_keymap_format_ops = {
    .keymap_new_from_rmlvo = ਪਾਠ_v1_keymap_new_from_rmlvo,
    .keymap_new_from_ਨਾਂ = ਪਾਠ_v1_keymap_new_from_ਨਾਂ,
    .keymap_new_from_string = ਪਾਠ_v1_keymap_new_from_string,
    .keymap_new_from_file = ਪਾਠ_v1_keymap_new_from_file,
    .keymap_serialize = ਪਾਠ_v1_keymap_serialize,
};
