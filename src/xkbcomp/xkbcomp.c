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

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <stdbool.h>

#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "xkbcomp-priv.h"
#அந்தர்பாவ "keymap.h"
#அந்தர்பாவ "rules.h"
#அந்தர்பாவ "rmlvo.h"

bool
xkb_components_names_from_rules(கட்டமைப்பு xkb_context *ctx,
                                மாறிலி கட்டமைப்பு xkb_rule_names *rmlvo_in,
                                கட்டமைப்பு xkb_rule_names *rmlvo_out,
                                கட்டமைப்பு xkb_component_names *components_out)
{
    /* Resolve default RMLVO values. We need a mutable copy of the input. */
    கட்டமைப்பு xkb_rule_names rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    எனில் (rmlvo_out) {
        /* Copy the sanitized RMLVO, if required. */
        *rmlvo_out = rmlvo;
    }
    எனில் (!components_out) {
        /* KcCGST not required.
         * While RMLVO and KcCGST outputs are both optional, at least one must
         * be provided. */
        திரும்பு !!rmlvo_out;
    }

    /* Resolve the RMLVO names to KcCGST components */
    *components_out = (கட்டமைப்பு xkb_component_names){ 0 };
    திரும்பு xkb_components_from_rules_names(ctx, &rmlvo, components_out, வெற்று);
}

நிலையான bool
compile_keymap_file(கட்டமைப்பு xkb_keymap *keymap, XkbFile *file)
{
    எனில் (file->file_type != FILE_TYPE_KEYMAP) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Cannot compile a %s file alone into a keymap\n",
                xkb_file_type_to_string(file->file_type));
        திரும்பு false;
    }

    எனில் (!CompileKeymap(file, keymap)) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to compile keymap\n");
        திரும்பு false;
    }

    திரும்பு true;
}

நிலையான bool
text_v1_keymap_new_from_rmlvo(கட்டமைப்பு xkb_keymap *keymap,
                              மாறிலி கட்டமைப்பு xkb_rmlvo_builder *rmlvo)
{
    bool ok;
    கட்டமைப்பு xkb_component_names kccgst;
    XkbFile *file;

    எனில் (keymap->ctx->log_level >= XKB_LOG_LEVEL_DEBUG) {
        கட்டமைப்பு xkb_rule_names names = { 0 };
        மாறிலி size_t buf_size = அளவு(rmlvo->ctx->text_buffer) - 1;
        எழுத்து *buf = xkb_context_get_buffer(rmlvo->ctx, buf_size);
        எனில் (unlikely(!buf))
            திரும்பு false;
        ok = xkb_rmlvo_builder_to_rules_names(rmlvo, &names, buf, buf_size);
        எனில் (unlikely(!ok))
            திரும்பு false;
        log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Compiling from RMLVO builder: rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                names.rules, names.model, names.layout, names.variant,
                names.options);
    }

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rmlvo_builder(rmlvo, &kccgst, &keymap->இலக்கம்_groups);
    எனில் (!ok) {
        கட்டமைப்பு xkb_rule_names names = { 0 };
        மாறிலி size_t buf_size = அளவு(rmlvo->ctx->text_buffer);
        எழுத்து *buf = xkb_context_get_buffer(rmlvo->ctx, buf_size);
        எனில் (unlikely(!buf))
            திரும்பு false;
        ok = xkb_rmlvo_builder_to_rules_names(rmlvo, &names, buf, buf_size);
        எனில் (unlikely(!ok))
            திரும்பு false;
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                names.rules, names.model, names.layout, names.variant,
                names.options);
        திரும்பு false;
    }

    மாறிலி xkb_layout_index_t max_groups = format_max_groups(keymap->format);
    எனில் (keymap->இலக்கம்_groups > max_groups)
        keymap->இலக்கம்_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    விடுவி(kccgst.keycodes);
    விடுவி(kccgst.types);
    விடுவி(kccgst.compatibility);
    விடுவி(kccgst.symbols);
    விடுவி(kccgst.geometry);

    எனில் (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        திரும்பு false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    திரும்பு ok;
}

நிலையான bool
text_v1_keymap_new_from_names(கட்டமைப்பு xkb_keymap *keymap,
                              மாறிலி கட்டமைப்பு xkb_rule_names *rmlvo)
{
    bool ok;
    கட்டமைப்பு xkb_component_names kccgst;
    XkbFile *file;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from RMLVO: rules '%s', model '%s', layout '%s', "
            "variant '%s', options '%s'\n",
            rmlvo->rules, rmlvo->model, rmlvo->layout, rmlvo->variant,
            rmlvo->options);

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rules_names(keymap->ctx, rmlvo, &kccgst,
                                         &keymap->இலக்கம்_groups);
    எனில் (!ok) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                rmlvo->rules, rmlvo->model, rmlvo->layout, rmlvo->variant,
                rmlvo->options);
        திரும்பு false;
    }

    மாறிலி xkb_layout_index_t max_groups = format_max_groups(keymap->format);
    எனில் (keymap->இலக்கம்_groups > max_groups)
        keymap->இலக்கம்_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    விடுவி(kccgst.keycodes);
    விடுவி(kccgst.types);
    விடுவி(kccgst.compatibility);
    விடுவி(kccgst.symbols);
    விடுவி(kccgst.geometry);

    எனில் (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        திரும்பு false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    திரும்பு ok;
}

நிலையான bool
text_v1_keymap_new_from_string(கட்டமைப்பு xkb_keymap *keymap,
                               மாறிலி எழுத்து *string, size_t len)
{
    bool ok;
    XkbFile *xkb_file;

    மாறிலி கட்டமைப்பு parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseString(keymap->ctx, &config,
                              string, len, "(input string)", வெற்று);
    எனில் (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb string\n");
        திரும்பு false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    திரும்பு ok;
}

நிலையான bool
text_v1_keymap_new_from_file(கட்டமைப்பு xkb_keymap *keymap, FILE *file)
{
    bool ok;
    XkbFile *xkb_file;

    மாறிலி கட்டமைப்பு parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseFile(keymap->ctx, &config, file, "(unknown file)", வெற்று);
    எனில் (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb file\n");
        திரும்பு false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    திரும்பு ok;
}

மாறிலி கட்டமைப்பு xkb_keymap_format_ops text_v1_keymap_format_ops = {
    .keymap_new_from_rmlvo = text_v1_keymap_new_from_rmlvo,
    .keymap_new_from_names = text_v1_keymap_new_from_names,
    .keymap_new_from_string = text_v1_keymap_new_from_string,
    .keymap_new_from_file = text_v1_keymap_new_from_file,
    .keymap_serialize = text_v1_keymap_serialize,
};
