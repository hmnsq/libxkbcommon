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

#समावेश "config.h"

#समावेश <stdbool.h>

#समावेश "darray.h"
#समावेश "utils.h"
#समावेश "xkbcommon/xkbcommon.h"
#समावेश "xkbcomp-priv.h"
#समावेश "keymap.h"
#समावेश "rules.h"
#समावेश "rmlvo.h"

bool
xkb_components_names_from_rules(काष्ठा xkb_context *ctx,
                                स्थिर काष्ठा xkb_rule_names *rmlvo_in,
                                काष्ठा xkb_rule_names *rmlvo_out,
                                काष्ठा xkb_component_names *components_out)
{
    /* Resolve default RMLVO values. We need a mutable copy of the input. */
    काष्ठा xkb_rule_names rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    अगर (rmlvo_out) {
        /* Copy the sanitized RMLVO, if required. */
        *rmlvo_out = rmlvo;
    }
    अगर (!components_out) {
        /* KcCGST not required.
         * While RMLVO and KcCGST outputs are both optional, at least one must
         * be provided. */
        वापस !!rmlvo_out;
    }

    /* Resolve the RMLVO names to KcCGST components */
    *components_out = (काष्ठा xkb_component_names){ 0 };
    वापस xkb_components_from_rules_names(ctx, &rmlvo, components_out, शून्य);
}

अटल bool
compile_keymap_file(काष्ठा xkb_keymap *keymap, XkbFile *file)
{
    अगर (file->file_type != FILE_TYPE_KEYMAP) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Cannot compile a %s file alone into a keymap\n",
                xkb_file_type_to_string(file->file_type));
        वापस false;
    }

    अगर (!CompileKeymap(file, keymap)) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to compile keymap\n");
        वापस false;
    }

    वापस true;
}

अटल bool
text_v1_keymap_new_from_rmlvo(काष्ठा xkb_keymap *keymap,
                              स्थिर काष्ठा xkb_rmlvo_builder *rmlvo)
{
    bool ok;
    काष्ठा xkb_component_names kccgst;
    XkbFile *file;

    अगर (keymap->ctx->log_level >= XKB_LOG_LEVEL_DEBUG) {
        काष्ठा xkb_rule_names names = { 0 };
        स्थिर माप_प्रकार buf_आकार = माप(rmlvo->ctx->text_बफर) - 1;
        अक्षर *buf = xkb_context_get_बफर(rmlvo->ctx, buf_आकार);
        अगर (unlikely(!buf))
            वापस false;
        ok = xkb_rmlvo_builder_to_rules_names(rmlvo, &names, buf, buf_आकार);
        अगर (unlikely(!ok))
            वापस false;
        log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Compiling from RMLVO builder: rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                names.rules, names.model, names.layout, names.variant,
                names.options);
    }

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rmlvo_builder(rmlvo, &kccgst, &keymap->अंक_groups);
    अगर (!ok) {
        काष्ठा xkb_rule_names names = { 0 };
        स्थिर माप_प्रकार buf_आकार = माप(rmlvo->ctx->text_बफर);
        अक्षर *buf = xkb_context_get_बफर(rmlvo->ctx, buf_आकार);
        अगर (unlikely(!buf))
            वापस false;
        ok = xkb_rmlvo_builder_to_rules_names(rmlvo, &names, buf, buf_आकार);
        अगर (unlikely(!ok))
            वापस false;
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                names.rules, names.model, names.layout, names.variant,
                names.options);
        वापस false;
    }

    स्थिर xkb_layout_सूचकांक_t max_groups = format_max_groups(keymap->format);
    अगर (keymap->अंक_groups > max_groups)
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

    अगर (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        वापस false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    वापस ok;
}

अटल bool
text_v1_keymap_new_from_names(काष्ठा xkb_keymap *keymap,
                              स्थिर काष्ठा xkb_rule_names *rmlvo)
{
    bool ok;
    काष्ठा xkb_component_names kccgst;
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
    अगर (!ok) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                rmlvo->rules, rmlvo->model, rmlvo->layout, rmlvo->variant,
                rmlvo->options);
        वापस false;
    }

    स्थिर xkb_layout_सूचकांक_t max_groups = format_max_groups(keymap->format);
    अगर (keymap->अंक_groups > max_groups)
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

    अगर (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        वापस false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    वापस ok;
}

अटल bool
text_v1_keymap_new_from_string(काष्ठा xkb_keymap *keymap,
                               स्थिर अक्षर *string, माप_प्रकार len)
{
    bool ok;
    XkbFile *xkb_file;

    स्थिर काष्ठा parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseString(keymap->ctx, &config,
                              string, len, "(input string)", शून्य);
    अगर (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb string\n");
        वापस false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    वापस ok;
}

अटल bool
text_v1_keymap_new_from_file(काष्ठा xkb_keymap *keymap, खाता *file)
{
    bool ok;
    XkbFile *xkb_file;

    स्थिर काष्ठा parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseFile(keymap->ctx, &config, file, "(unknown file)", शून्य);
    अगर (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb file\n");
        वापस false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    वापस ok;
}

स्थिर काष्ठा xkb_keymap_format_ops text_v1_keymap_format_ops = {
    .keymap_new_from_rmlvo = text_v1_keymap_new_from_rmlvo,
    .keymap_new_from_names = text_v1_keymap_new_from_names,
    .keymap_new_from_string = text_v1_keymap_new_from_string,
    .keymap_new_from_file = text_v1_keymap_new_from_file,
    .keymap_serialize = text_v1_keymap_serialize,
};
