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

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <stdbool.h>

#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "xkbcomp-priv.h"
#അന്തര്ഭാവ "keymap.h"
#അന്തര്ഭാവ "rules.h"
#അന്തര്ഭാവ "rmlvo.h"

bool
xkb_components_names_from_rules(ഘടന xkb_context *ctx,
                                സ്ഥിരം ഘടന xkb_rule_names *rmlvo_in,
                                ഘടന xkb_rule_names *rmlvo_out,
                                ഘടന xkb_component_names *components_out)
{
    /* Resolve default RMLVO values. We need a mutable copy of the input. */
    ഘടന xkb_rule_names rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    എങ്കിൽ (rmlvo_out) {
        /* Copy the sanitized RMLVO, if required. */
        *rmlvo_out = rmlvo;
    }
    എങ്കിൽ (!components_out) {
        /* KcCGST not required.
         * While RMLVO and KcCGST outputs are both optional, at least one must
         * be provided. */
        തിരികെ !!rmlvo_out;
    }

    /* Resolve the RMLVO names to KcCGST components */
    *components_out = (ഘടന xkb_component_names){ 0 };
    തിരികെ xkb_components_from_rules_names(ctx, &rmlvo, components_out, ശൂന്യ);
}

സ്ഥിതം bool
compile_keymap_file(ഘടന xkb_keymap *keymap, XkbFile *file)
{
    എങ്കിൽ (file->file_type != FILE_TYPE_KEYMAP) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Cannot compile a %s file alone into a keymap\n",
                xkb_file_type_to_string(file->file_type));
        തിരികെ false;
    }

    എങ്കിൽ (!CompileKeymap(file, keymap)) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to compile keymap\n");
        തിരികെ false;
    }

    തിരികെ true;
}

സ്ഥിതം bool
text_v1_keymap_new_from_rmlvo(ഘടന xkb_keymap *keymap,
                              സ്ഥിരം ഘടന xkb_rmlvo_builder *rmlvo)
{
    bool ok;
    ഘടന xkb_component_names kccgst;
    XkbFile *file;

    എങ്കിൽ (keymap->ctx->log_level >= XKB_LOG_LEVEL_DEBUG) {
        ഘടന xkb_rule_names names = { 0 };
        സ്ഥിരം size_t buf_size = വലുപ്പം(rmlvo->ctx->text_buffer) - 1;
        അക്ഷരം *buf = xkb_context_get_buffer(rmlvo->ctx, buf_size);
        എങ്കിൽ (unlikely(!buf))
            തിരികെ false;
        ok = xkb_rmlvo_builder_to_rules_names(rmlvo, &names, buf, buf_size);
        എങ്കിൽ (unlikely(!ok))
            തിരികെ false;
        log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Compiling from RMLVO builder: rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                names.rules, names.model, names.layout, names.variant,
                names.options);
    }

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rmlvo_builder(rmlvo, &kccgst, &keymap->അക്കം_groups);
    എങ്കിൽ (!ok) {
        ഘടന xkb_rule_names names = { 0 };
        സ്ഥിരം size_t buf_size = വലുപ്പം(rmlvo->ctx->text_buffer);
        അക്ഷരം *buf = xkb_context_get_buffer(rmlvo->ctx, buf_size);
        എങ്കിൽ (unlikely(!buf))
            തിരികെ false;
        ok = xkb_rmlvo_builder_to_rules_names(rmlvo, &names, buf, buf_size);
        എങ്കിൽ (unlikely(!ok))
            തിരികെ false;
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                names.rules, names.model, names.layout, names.variant,
                names.options);
        തിരികെ false;
    }

    സ്ഥിരം xkb_layout_index_t max_groups = format_max_groups(keymap->format);
    എങ്കിൽ (keymap->അക്കം_groups > max_groups)
        keymap->അക്കം_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    സ്വതന്ത്രം(kccgst.keycodes);
    സ്വതന്ത്രം(kccgst.types);
    സ്വതന്ത്രം(kccgst.compatibility);
    സ്വതന്ത്രം(kccgst.symbols);
    സ്വതന്ത്രം(kccgst.geometry);

    എങ്കിൽ (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        തിരികെ false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    തിരികെ ok;
}

സ്ഥിതം bool
text_v1_keymap_new_from_names(ഘടന xkb_keymap *keymap,
                              സ്ഥിരം ഘടന xkb_rule_names *rmlvo)
{
    bool ok;
    ഘടന xkb_component_names kccgst;
    XkbFile *file;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from RMLVO: rules '%s', model '%s', layout '%s', "
            "variant '%s', options '%s'\n",
            rmlvo->rules, rmlvo->model, rmlvo->layout, rmlvo->variant,
            rmlvo->options);

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rules_names(keymap->ctx, rmlvo, &kccgst,
                                         &keymap->അക്കം_groups);
    എങ്കിൽ (!ok) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                rmlvo->rules, rmlvo->model, rmlvo->layout, rmlvo->variant,
                rmlvo->options);
        തിരികെ false;
    }

    സ്ഥിരം xkb_layout_index_t max_groups = format_max_groups(keymap->format);
    എങ്കിൽ (keymap->അക്കം_groups > max_groups)
        keymap->അക്കം_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    സ്വതന്ത്രം(kccgst.keycodes);
    സ്വതന്ത്രം(kccgst.types);
    സ്വതന്ത്രം(kccgst.compatibility);
    സ്വതന്ത്രം(kccgst.symbols);
    സ്വതന്ത്രം(kccgst.geometry);

    എങ്കിൽ (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        തിരികെ false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    തിരികെ ok;
}

സ്ഥിതം bool
text_v1_keymap_new_from_string(ഘടന xkb_keymap *keymap,
                               സ്ഥിരം അക്ഷരം *string, size_t len)
{
    bool ok;
    XkbFile *xkb_file;

    സ്ഥിരം ഘടന parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseString(keymap->ctx, &config,
                              string, len, "(input string)", ശൂന്യ);
    എങ്കിൽ (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb string\n");
        തിരികെ false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    തിരികെ ok;
}

സ്ഥിതം bool
text_v1_keymap_new_from_file(ഘടന xkb_keymap *keymap, FILE *file)
{
    bool ok;
    XkbFile *xkb_file;

    സ്ഥിരം ഘടന parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseFile(keymap->ctx, &config, file, "(unknown file)", ശൂന്യ);
    എങ്കിൽ (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb file\n");
        തിരികെ false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    തിരികെ ok;
}

സ്ഥിരം ഘടന xkb_keymap_format_ops text_v1_keymap_format_ops = {
    .keymap_new_from_rmlvo = text_v1_keymap_new_from_rmlvo,
    .keymap_new_from_names = text_v1_keymap_new_from_names,
    .keymap_new_from_string = text_v1_keymap_new_from_string,
    .keymap_new_from_file = text_v1_keymap_new_from_file,
    .keymap_serialize = text_v1_keymap_serialize,
};
