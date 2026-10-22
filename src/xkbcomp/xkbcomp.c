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

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <stdbool.h>

#অন্তর্ভাব "darray.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "xkbcomp-priv.h"
#অন্তর্ভাব "keymap.h"
#অন্তর্ভাব "rules.h"
#অন্তর্ভাব "rmlvo.h"

bool
xkb_components_names_from_rules(সংরচনা xkb_context *ctx,
                                ধ্রুব সংরচনা xkb_rule_names *rmlvo_in,
                                সংরচনা xkb_rule_names *rmlvo_out,
                                সংরচনা xkb_component_names *components_out)
{
    /* Resolve default RMLVO values. We need a mutable copy of the input. */
    সংরচনা xkb_rule_names rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    যদি (rmlvo_out) {
        /* Copy the sanitized RMLVO, if required. */
        *rmlvo_out = rmlvo;
    }
    যদি (!components_out) {
        /* KcCGST not required.
         * While RMLVO and KcCGST outputs are both optional, at least one must
         * be provided. */
        প্রত্যাগম !!rmlvo_out;
    }

    /* Resolve the RMLVO names to KcCGST components */
    *components_out = (সংরচনা xkb_component_names){ 0 };
    প্রত্যাগম xkb_components_from_rules_names(ctx, &rmlvo, components_out, রিক্ত);
}

স্থির bool
compile_keymap_file(সংরচনা xkb_keymap *keymap, XkbFile *file)
{
    যদি (file->file_type != FILE_TYPE_KEYMAP) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Cannot compile a %s file alone into a keymap\n",
                xkb_file_type_to_string(file->file_type));
        প্রত্যাগম false;
    }

    যদি (!CompileKeymap(file, keymap)) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to compile keymap\n");
        প্রত্যাগম false;
    }

    প্রত্যাগম true;
}

স্থির bool
text_v1_keymap_new_from_rmlvo(সংরচনা xkb_keymap *keymap,
                              ধ্রুব সংরচনা xkb_rmlvo_builder *rmlvo)
{
    bool ok;
    সংরচনা xkb_component_names kccgst;
    XkbFile *file;

    যদি (keymap->ctx->log_level >= XKB_LOG_LEVEL_DEBUG) {
        সংরচনা xkb_rule_names names = { 0 };
        ধ্রুব size_t buf_size = পরিমাণ(rmlvo->ctx->text_buffer) - 1;
        বর্ণ *buf = xkb_context_get_buffer(rmlvo->ctx, buf_size);
        যদি (unlikely(!buf))
            প্রত্যাগম false;
        ok = xkb_rmlvo_builder_to_rules_names(rmlvo, &names, buf, buf_size);
        যদি (unlikely(!ok))
            প্রত্যাগম false;
        log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Compiling from RMLVO builder: rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                names.rules, names.model, names.layout, names.variant,
                names.options);
    }

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rmlvo_builder(rmlvo, &kccgst, &keymap->অঙ্ক_groups);
    যদি (!ok) {
        সংরচনা xkb_rule_names names = { 0 };
        ধ্রুব size_t buf_size = পরিমাণ(rmlvo->ctx->text_buffer);
        বর্ণ *buf = xkb_context_get_buffer(rmlvo->ctx, buf_size);
        যদি (unlikely(!buf))
            প্রত্যাগম false;
        ok = xkb_rmlvo_builder_to_rules_names(rmlvo, &names, buf, buf_size);
        যদি (unlikely(!ok))
            প্রত্যাগম false;
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                names.rules, names.model, names.layout, names.variant,
                names.options);
        প্রত্যাগম false;
    }

    ধ্রুব xkb_layout_index_t max_groups = format_max_groups(keymap->format);
    যদি (keymap->অঙ্ক_groups > max_groups)
        keymap->অঙ্ক_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    মুক্ত(kccgst.keycodes);
    মুক্ত(kccgst.types);
    মুক্ত(kccgst.compatibility);
    মুক্ত(kccgst.symbols);
    মুক্ত(kccgst.geometry);

    যদি (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        প্রত্যাগম false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    প্রত্যাগম ok;
}

স্থির bool
text_v1_keymap_new_from_names(সংরচনা xkb_keymap *keymap,
                              ধ্রুব সংরচনা xkb_rule_names *rmlvo)
{
    bool ok;
    সংরচনা xkb_component_names kccgst;
    XkbFile *file;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from RMLVO: rules '%s', model '%s', layout '%s', "
            "variant '%s', options '%s'\n",
            rmlvo->rules, rmlvo->model, rmlvo->layout, rmlvo->variant,
            rmlvo->options);

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rules_names(keymap->ctx, rmlvo, &kccgst,
                                         &keymap->অঙ্ক_groups);
    যদি (!ok) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                rmlvo->rules, rmlvo->model, rmlvo->layout, rmlvo->variant,
                rmlvo->options);
        প্রত্যাগম false;
    }

    ধ্রুব xkb_layout_index_t max_groups = format_max_groups(keymap->format);
    যদি (keymap->অঙ্ক_groups > max_groups)
        keymap->অঙ্ক_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    মুক্ত(kccgst.keycodes);
    মুক্ত(kccgst.types);
    মুক্ত(kccgst.compatibility);
    মুক্ত(kccgst.symbols);
    মুক্ত(kccgst.geometry);

    যদি (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        প্রত্যাগম false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    প্রত্যাগম ok;
}

স্থির bool
text_v1_keymap_new_from_string(সংরচনা xkb_keymap *keymap,
                               ধ্রুব বর্ণ *string, size_t len)
{
    bool ok;
    XkbFile *xkb_file;

    ধ্রুব সংরচনা parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseString(keymap->ctx, &config,
                              string, len, "(input string)", রিক্ত);
    যদি (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb string\n");
        প্রত্যাগম false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    প্রত্যাগম ok;
}

স্থির bool
text_v1_keymap_new_from_file(সংরচনা xkb_keymap *keymap, FILE *file)
{
    bool ok;
    XkbFile *xkb_file;

    ধ্রুব সংরচনা parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseFile(keymap->ctx, &config, file, "(unknown file)", রিক্ত);
    যদি (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb file\n");
        প্রত্যাগম false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    প্রত্যাগম ok;
}

ধ্রুব সংরচনা xkb_keymap_format_ops text_v1_keymap_format_ops = {
    .keymap_new_from_rmlvo = text_v1_keymap_new_from_rmlvo,
    .keymap_new_from_names = text_v1_keymap_new_from_names,
    .keymap_new_from_string = text_v1_keymap_new_from_string,
    .keymap_new_from_file = text_v1_keymap_new_from_file,
    .keymap_serialize = text_v1_keymap_serialize,
};
