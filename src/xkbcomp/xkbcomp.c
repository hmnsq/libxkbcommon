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
xkb_components_names_from_rules(ساختار xkb_context *ctx,
                                ثابت ساختار xkb_rule_names *rmlvo_in,
                                ساختار xkb_rule_names *rmlvo_out,
                                ساختار xkb_component_names *components_out)
{
    /* Resolve default RMLVO values. We need a mutable copy of the input. */
    ساختار xkb_rule_names rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    اگر (rmlvo_out) {
        /* Copy the sanitized RMLVO, if required. */
        *rmlvo_out = rmlvo;
    }
    اگر (!components_out) {
        /* KcCGST not required.
         * While RMLVO and KcCGST outputs are both optional, at least one must
         * be provided. */
        بازگشت !!rmlvo_out;
    }

    /* Resolve the RMLVO names to KcCGST components */
    *components_out = (ساختار xkb_component_names){ 0 };
    بازگشت xkb_components_from_rules_names(ctx, &rmlvo, components_out, پوچ);
}

ایستا bool
compile_keymap_file(ساختار xkb_keymap *keymap, XkbFile *file)
{
    اگر (file->file_type != FILE_TYPE_KEYMAP) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Cannot compile a %s file alone into a keymap\n",
                xkb_file_type_to_string(file->file_type));
        بازگشت false;
    }

    اگر (!CompileKeymap(file, keymap)) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to compile keymap\n");
        بازگشت false;
    }

    بازگشت true;
}

ایستا bool
text_v1_keymap_new_from_rmlvo(ساختار xkb_keymap *keymap,
                              ثابت ساختار xkb_rmlvo_builder *rmlvo)
{
    bool ok;
    ساختار xkb_component_names kccgst;
    XkbFile *file;

    اگر (keymap->ctx->log_level >= XKB_LOG_LEVEL_DEBUG) {
        ساختار xkb_rule_names names = { 0 };
        ثابت size_t buf_size = اندازه(rmlvo->ctx->text_buffer) - 1;
        نویسه *buf = xkb_context_get_buffer(rmlvo->ctx, buf_size);
        اگر (unlikely(!buf))
            بازگشت false;
        ok = xkb_rmlvo_builder_to_rules_names(rmlvo, &names, buf, buf_size);
        اگر (unlikely(!ok))
            بازگشت false;
        log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Compiling from RMLVO builder: rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                names.rules, names.model, names.layout, names.variant,
                names.options);
    }

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rmlvo_builder(rmlvo, &kccgst, &keymap->رقم_groups);
    اگر (!ok) {
        ساختار xkb_rule_names names = { 0 };
        ثابت size_t buf_size = اندازه(rmlvo->ctx->text_buffer);
        نویسه *buf = xkb_context_get_buffer(rmlvo->ctx, buf_size);
        اگر (unlikely(!buf))
            بازگشت false;
        ok = xkb_rmlvo_builder_to_rules_names(rmlvo, &names, buf, buf_size);
        اگر (unlikely(!ok))
            بازگشت false;
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                names.rules, names.model, names.layout, names.variant,
                names.options);
        بازگشت false;
    }

    ثابت xkb_layout_index_t max_groups = format_max_groups(keymap->format);
    اگر (keymap->رقم_groups > max_groups)
        keymap->رقم_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
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
        بازگشت false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    بازگشت ok;
}

ایستا bool
text_v1_keymap_new_from_names(ساختار xkb_keymap *keymap,
                              ثابت ساختار xkb_rule_names *rmlvo)
{
    bool ok;
    ساختار xkb_component_names kccgst;
    XkbFile *file;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from RMLVO: rules '%s', model '%s', layout '%s', "
            "variant '%s', options '%s'\n",
            rmlvo->rules, rmlvo->model, rmlvo->layout, rmlvo->variant,
            rmlvo->options);

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rules_names(keymap->ctx, rmlvo, &kccgst,
                                         &keymap->رقم_groups);
    اگر (!ok) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                rmlvo->rules, rmlvo->model, rmlvo->layout, rmlvo->variant,
                rmlvo->options);
        بازگشت false;
    }

    ثابت xkb_layout_index_t max_groups = format_max_groups(keymap->format);
    اگر (keymap->رقم_groups > max_groups)
        keymap->رقم_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
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
        بازگشت false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    بازگشت ok;
}

ایستا bool
text_v1_keymap_new_from_string(ساختار xkb_keymap *keymap,
                               ثابت نویسه *string, size_t len)
{
    bool ok;
    XkbFile *xkb_file;

    ثابت ساختار parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseString(keymap->ctx, &config,
                              string, len, "(input string)", پوچ);
    اگر (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb string\n");
        بازگشت false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    بازگشت ok;
}

ایستا bool
text_v1_keymap_new_from_file(ساختار xkb_keymap *keymap, FILE *file)
{
    bool ok;
    XkbFile *xkb_file;

    ثابت ساختار parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseFile(keymap->ctx, &config, file, "(unknown file)", پوچ);
    اگر (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb file\n");
        بازگشت false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    بازگشت ok;
}

ثابت ساختار xkb_keymap_format_ops text_v1_keymap_format_ops = {
    .keymap_new_from_rmlvo = text_v1_keymap_new_from_rmlvo,
    .keymap_new_from_names = text_v1_keymap_new_from_names,
    .keymap_new_from_string = text_v1_keymap_new_from_string,
    .keymap_new_from_file = text_v1_keymap_new_from_file,
    .keymap_serialize = text_v1_keymap_serialize,
};
