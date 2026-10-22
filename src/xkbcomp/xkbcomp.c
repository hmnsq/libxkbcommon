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

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdbool.h>

#अन्तर्भाव "darray.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "xkbcomp-priv.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "rules.h"
#अन्तर्भाव "rmlvo.h"

bool
xkb_components_names_from_rules(संरचना xkb_context *ctx,
                                स्थिरांक संरचना xkb_rule_names *rmlvo_in,
                                संरचना xkb_rule_names *rmlvo_out,
                                संरचना xkb_component_names *components_out)
{
    /* Resolve default RMLVO values. We need a mutable copy of the input. */
    संरचना xkb_rule_names rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    जर (rmlvo_out) {
        /* Copy the sanitized RMLVO, if required. */
        *rmlvo_out = rmlvo;
    }
    जर (!components_out) {
        /* KcCGST not required.
         * While RMLVO and KcCGST outputs are both optional, at least one must
         * be provided. */
        परत !!rmlvo_out;
    }

    /* Resolve the RMLVO names to KcCGST components */
    *components_out = (संरचना xkb_component_names){ 0 };
    परत xkb_components_from_rules_names(ctx, &rmlvo, components_out, रिक्त);
}

स्थिर bool
compile_keymap_file(संरचना xkb_keymap *keymap, XkbFile *file)
{
    जर (file->file_type != FILE_TYPE_KEYMAP) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Cannot compile a %s file alone into a keymap\n",
                xkb_file_type_to_string(file->file_type));
        परत false;
    }

    जर (!CompileKeymap(file, keymap)) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to compile keymap\n");
        परत false;
    }

    परत true;
}

स्थिर bool
text_v1_keymap_new_from_rmlvo(संरचना xkb_keymap *keymap,
                              स्थिरांक संरचना xkb_rmlvo_builder *rmlvo)
{
    bool ok;
    संरचना xkb_component_names kccgst;
    XkbFile *file;

    जर (keymap->ctx->log_level >= XKB_LOG_LEVEL_DEBUG) {
        संरचना xkb_rule_names names = { 0 };
        स्थिरांक माप_प्रकार buf_size = आकार(rmlvo->ctx->text_buffer) - 1;
        अक्षर *buf = xkb_context_get_buffer(rmlvo->ctx, buf_size);
        जर (unlikely(!buf))
            परत false;
        ok = xkb_rmlvo_builder_to_rules_names(rmlvo, &names, buf, buf_size);
        जर (unlikely(!ok))
            परत false;
        log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Compiling from RMLVO builder: rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                names.rules, names.model, names.layout, names.variant,
                names.options);
    }

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rmlvo_builder(rmlvo, &kccgst, &keymap->अंक_groups);
    जर (!ok) {
        संरचना xkb_rule_names names = { 0 };
        स्थिरांक माप_प्रकार buf_size = आकार(rmlvo->ctx->text_buffer);
        अक्षर *buf = xkb_context_get_buffer(rmlvo->ctx, buf_size);
        जर (unlikely(!buf))
            परत false;
        ok = xkb_rmlvo_builder_to_rules_names(rmlvo, &names, buf, buf_size);
        जर (unlikely(!ok))
            परत false;
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                names.rules, names.model, names.layout, names.variant,
                names.options);
        परत false;
    }

    स्थिरांक xkb_layout_index_t max_groups = format_max_groups(keymap->format);
    जर (keymap->अंक_groups > max_groups)
        keymap->अंक_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    मुक्त(kccgst.keycodes);
    मुक्त(kccgst.types);
    मुक्त(kccgst.compatibility);
    मुक्त(kccgst.symbols);
    मुक्त(kccgst.geometry);

    जर (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        परत false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    परत ok;
}

स्थिर bool
text_v1_keymap_new_from_names(संरचना xkb_keymap *keymap,
                              स्थिरांक संरचना xkb_rule_names *rmlvo)
{
    bool ok;
    संरचना xkb_component_names kccgst;
    XkbFile *file;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from RMLVO: rules '%s', model '%s', layout '%s', "
            "variant '%s', options '%s'\n",
            rmlvo->rules, rmlvo->model, rmlvo->layout, rmlvo->variant,
            rmlvo->options);

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rules_names(keymap->ctx, rmlvo, &kccgst,
                                         &keymap->अंक_groups);
    जर (!ok) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                rmlvo->rules, rmlvo->model, rmlvo->layout, rmlvo->variant,
                rmlvo->options);
        परत false;
    }

    स्थिरांक xkb_layout_index_t max_groups = format_max_groups(keymap->format);
    जर (keymap->अंक_groups > max_groups)
        keymap->अंक_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    मुक्त(kccgst.keycodes);
    मुक्त(kccgst.types);
    मुक्त(kccgst.compatibility);
    मुक्त(kccgst.symbols);
    मुक्त(kccgst.geometry);

    जर (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        परत false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    परत ok;
}

स्थिर bool
text_v1_keymap_new_from_string(संरचना xkb_keymap *keymap,
                               स्थिरांक अक्षर *string, माप_प्रकार len)
{
    bool ok;
    XkbFile *xkb_file;

    स्थिरांक संरचना parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseString(keymap->ctx, &config,
                              string, len, "(input string)", रिक्त);
    जर (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb string\n");
        परत false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    परत ok;
}

स्थिर bool
text_v1_keymap_new_from_file(संरचना xkb_keymap *keymap, खाता *file)
{
    bool ok;
    XkbFile *xkb_file;

    स्थिरांक संरचना parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseFile(keymap->ctx, &config, file, "(unknown file)", रिक्त);
    जर (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb file\n");
        परत false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    परत ok;
}

स्थिरांक संरचना xkb_keymap_format_ops text_v1_keymap_format_ops = {
    .keymap_new_from_rmlvo = text_v1_keymap_new_from_rmlvo,
    .keymap_new_from_names = text_v1_keymap_new_from_names,
    .keymap_new_from_string = text_v1_keymap_new_from_string,
    .keymap_new_from_file = text_v1_keymap_new_from_file,
    .keymap_serialize = text_v1_keymap_serialize,
};
