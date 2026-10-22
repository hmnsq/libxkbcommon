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
xkb_components_नामानि_from_नियमाः(संरचना xkb_context *ctx,
                                ध्रुव संरचना xkb_नियमः_नामानि *rmlvo_अन्तर्,
                                संरचना xkb_नियमः_नामानि *rmlvo_बहिर्गमनम्,
                                संरचना xkb_component_नामानि *components_बहिर्गमनम्)
{
    /* Resolve default RMLVO values. We need a mutable copy of the input. */
    संरचना xkb_नियमः_नामानि rmlvo = *rmlvo_अन्तर्;
    xkb_context_sanitize_नियमः_नामानि(ctx, &rmlvo);

    यदि (rmlvo_बहिर्गमनम्) {
        /* Copy the sanitized RMLVO, if required. */
        *rmlvo_बहिर्गमनम् = rmlvo;
    }
    यदि (!components_बहिर्गमनम्) {
        /* KcCGST not required.
         * While RMLVO and KcCGST outputs are both optional, at least one must
         * be provided. */
        प्रत्यागम !!rmlvo_बहिर्गमनम्;
    }

    /* Resolve the RMLVO names to KcCGST components */
    *components_बहिर्गमनम् = (संरचना xkb_component_नामानि){ 0 };
    प्रत्यागम xkb_components_from_नियमाः_नामानि(ctx, &rmlvo, components_बहिर्गमनम्, रिक्त);
}

स्थिर bool
compile_keymap_file(संरचना xkb_keymap *keymap, XkbFile *file)
{
    यदि (file->file_type != FILE_TYPE_KEYMAP) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Cannot compile a %s file alone into a keymap\n",
                xkb_file_type_गन्तव्यम्_string(file->file_type));
        प्रत्यागम false;
    }

    यदि (!CompileKeymap(file, keymap)) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to compile keymap\n");
        प्रत्यागम false;
    }

    प्रत्यागम true;
}

स्थिर bool
text_v1_keymap_new_from_rmlvo(संरचना xkb_keymap *keymap,
                              ध्रुव संरचना xkb_rmlvo_builder *rmlvo)
{
    bool ok;
    संरचना xkb_component_नामानि kccgst;
    XkbFile *file;

    यदि (keymap->ctx->log_level >= XKB_LOG_LEVEL_DEBUG) {
        संरचना xkb_नियमः_नामानि नामानि = { 0 };
        ध्रुव आकार_t buf_आकार = परिमाण(rmlvo->ctx->text_बफर) - 1;
        वर्ण *buf = xkb_context_get_बफर(rmlvo->ctx, buf_आकार);
        यदि (unlikely(!buf))
            प्रत्यागम false;
        ok = xkb_rmlvo_builder_गन्तव्यम्_नियमाः_नामानि(rmlvo, &नामानि, buf, buf_आकार);
        यदि (unlikely(!ok))
            प्रत्यागम false;
        log_dbg(keymap->ctx, XKB_LOG_सन्देशः_NO_ID,
                "Compiling from RMLVO builder: rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                नामानि.नियमाः, नामानि.model, नामानि.layout, नामानि.variant,
                नामानि.options);
    }

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rmlvo_builder(rmlvo, &kccgst, &keymap->अङ्कः_groups);
    यदि (!ok) {
        संरचना xkb_नियमः_नामानि नामानि = { 0 };
        ध्रुव आकार_t buf_आकार = परिमाण(rmlvo->ctx->text_बफर);
        वर्ण *buf = xkb_context_get_बफर(rmlvo->ctx, buf_आकार);
        यदि (unlikely(!buf))
            प्रत्यागम false;
        ok = xkb_rmlvo_builder_गन्तव्यम्_नियमाः_नामानि(rmlvo, &नामानि, buf, buf_आकार);
        यदि (unlikely(!ok))
            प्रत्यागम false;
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                नामानि.नियमाः, नामानि.model, नामानि.layout, नामानि.variant,
                नामानि.options);
        प्रत्यागम false;
    }

    ध्रुव xkb_layout_सूचकांक_t max_groups = format_max_groups(keymap->format);
    यदि (keymap->अङ्कः_groups > max_groups)
        keymap->अङ्कः_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_सन्देशः_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    मुक्त(kccgst.keycodes);
    मुक्त(kccgst.types);
    मुक्त(kccgst.compatibility);
    मुक्त(kccgst.symbols);
    मुक्त(kccgst.geometry);

    यदि (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        प्रत्यागम false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    प्रत्यागम ok;
}

स्थिर bool
text_v1_keymap_new_from_नामानि(संरचना xkb_keymap *keymap,
                              ध्रुव संरचना xkb_नियमः_नामानि *rmlvo)
{
    bool ok;
    संरचना xkb_component_नामानि kccgst;
    XkbFile *file;

    log_dbg(keymap->ctx, XKB_LOG_सन्देशः_NO_ID,
            "Compiling from RMLVO: rules '%s', model '%s', layout '%s', "
            "variant '%s', options '%s'\n",
            rmlvo->नियमाः, rmlvo->model, rmlvo->layout, rmlvo->variant,
            rmlvo->options);

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_नियमाः_नामानि(keymap->ctx, rmlvo, &kccgst,
                                         &keymap->अङ्कः_groups);
    यदि (!ok) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                rmlvo->नियमाः, rmlvo->model, rmlvo->layout, rmlvo->variant,
                rmlvo->options);
        प्रत्यागम false;
    }

    ध्रुव xkb_layout_सूचकांक_t max_groups = format_max_groups(keymap->format);
    यदि (keymap->अङ्कः_groups > max_groups)
        keymap->अङ्कः_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_सन्देशः_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    मुक्त(kccgst.keycodes);
    मुक्त(kccgst.types);
    मुक्त(kccgst.compatibility);
    मुक्त(kccgst.symbols);
    मुक्त(kccgst.geometry);

    यदि (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        प्रत्यागम false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    प्रत्यागम ok;
}

स्थिर bool
text_v1_keymap_new_from_string(संरचना xkb_keymap *keymap,
                               ध्रुव वर्ण *string, आकार_t len)
{
    bool ok;
    XkbFile *xkb_file;

    ध्रुव संरचना parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseString(keymap->ctx, &config,
                              string, len, "(input string)", रिक्त);
    यदि (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb string\n");
        प्रत्यागम false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    प्रत्यागम ok;
}

स्थिर bool
text_v1_keymap_new_from_file(संरचना xkb_keymap *keymap, FILE *file)
{
    bool ok;
    XkbFile *xkb_file;

    ध्रुव संरचना parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseFile(keymap->ctx, &config, file, "(unknown file)", रिक्त);
    यदि (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb file\n");
        प्रत्यागम false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    प्रत्यागम ok;
}

ध्रुव संरचना xkb_keymap_format_ops text_v1_keymap_format_ops = {
    .keymap_new_from_rmlvo = text_v1_keymap_new_from_rmlvo,
    .keymap_new_from_नामानि = text_v1_keymap_new_from_नामानि,
    .keymap_new_from_string = text_v1_keymap_new_from_string,
    .keymap_new_from_file = text_v1_keymap_new_from_file,
    .keymap_serialize = text_v1_keymap_serialize,
};
