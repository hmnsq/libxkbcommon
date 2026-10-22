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

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdbool.h>

#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "rules.h"
#ಅನ್ತರ್ಭಾವ "rmlvo.h"

bool
xkb_components_names_from_rules(ಸಂರಚನಾ xkb_context *ctx,
                                ಧ್ರುವ ಸಂರಚನಾ xkb_rule_names *rmlvo_in,
                                ಸಂರಚನಾ xkb_rule_names *rmlvo_out,
                                ಸಂರಚನಾ xkb_component_names *components_out)
{
    /* Resolve default RMLVO values. We need a mutable copy of the input. */
    ಸಂರಚನಾ xkb_rule_names rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    ಯದಿ (rmlvo_out) {
        /* Copy the sanitized RMLVO, if required. */
        *rmlvo_out = rmlvo;
    }
    ಯದಿ (!components_out) {
        /* KcCGST not required.
         * While RMLVO and KcCGST outputs are both optional, at least one must
         * be provided. */
        ಪ್ರತ್ಯಾಗಮ !!rmlvo_out;
    }

    /* Resolve the RMLVO names to KcCGST components */
    *components_out = (ಸಂರಚನಾ xkb_component_names){ 0 };
    ಪ್ರತ್ಯಾಗಮ xkb_components_from_rules_names(ctx, &rmlvo, components_out, ರಿಕ್ತ);
}

ಸ್ಥಿರ bool
compile_keymap_file(ಸಂರಚನಾ xkb_keymap *keymap, XkbFile *file)
{
    ಯದಿ (file->file_type != FILE_TYPE_KEYMAP) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Cannot compile a %s file alone into a keymap\n",
                xkb_file_type_to_string(file->file_type));
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಯದಿ (!CompileKeymap(file, keymap)) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to compile keymap\n");
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
text_v1_keymap_new_from_rmlvo(ಸಂರಚನಾ xkb_keymap *keymap,
                              ಧ್ರುವ ಸಂರಚನಾ xkb_rmlvo_builder *rmlvo)
{
    bool ok;
    ಸಂರಚನಾ xkb_component_names kccgst;
    XkbFile *file;

    ಯದಿ (keymap->ctx->log_level >= XKB_LOG_LEVEL_DEBUG) {
        ಸಂರಚನಾ xkb_rule_names names = { 0 };
        ಧ್ರುವ size_t buf_size = ಪರಿಮಾಣ(rmlvo->ctx->text_buffer) - 1;
        ವರ್ಣ *buf = xkb_context_get_buffer(rmlvo->ctx, buf_size);
        ಯದಿ (unlikely(!buf))
            ಪ್ರತ್ಯಾಗಮ false;
        ok = xkb_rmlvo_builder_to_rules_names(rmlvo, &names, buf, buf_size);
        ಯದಿ (unlikely(!ok))
            ಪ್ರತ್ಯಾಗಮ false;
        log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Compiling from RMLVO builder: rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                names.rules, names.model, names.layout, names.variant,
                names.options);
    }

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rmlvo_builder(rmlvo, &kccgst, &keymap->ಅಂಕೆ_groups);
    ಯದಿ (!ok) {
        ಸಂರಚನಾ xkb_rule_names names = { 0 };
        ಧ್ರುವ size_t buf_size = ಪರಿಮಾಣ(rmlvo->ctx->text_buffer);
        ವರ್ಣ *buf = xkb_context_get_buffer(rmlvo->ctx, buf_size);
        ಯದಿ (unlikely(!buf))
            ಪ್ರತ್ಯಾಗಮ false;
        ok = xkb_rmlvo_builder_to_rules_names(rmlvo, &names, buf, buf_size);
        ಯದಿ (unlikely(!ok))
            ಪ್ರತ್ಯಾಗಮ false;
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                names.rules, names.model, names.layout, names.variant,
                names.options);
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಧ್ರುವ xkb_layout_index_t max_groups = format_max_groups(keymap->format);
    ಯದಿ (keymap->ಅಂಕೆ_groups > max_groups)
        keymap->ಅಂಕೆ_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    ಮುಕ್ತ(kccgst.keycodes);
    ಮುಕ್ತ(kccgst.types);
    ಮುಕ್ತ(kccgst.compatibility);
    ಮುಕ್ತ(kccgst.symbols);
    ಮುಕ್ತ(kccgst.geometry);

    ಯದಿ (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    ಪ್ರತ್ಯಾಗಮ ok;
}

ಸ್ಥಿರ bool
text_v1_keymap_new_from_names(ಸಂರಚನಾ xkb_keymap *keymap,
                              ಧ್ರುವ ಸಂರಚನಾ xkb_rule_names *rmlvo)
{
    bool ok;
    ಸಂರಚನಾ xkb_component_names kccgst;
    XkbFile *file;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from RMLVO: rules '%s', model '%s', layout '%s', "
            "variant '%s', options '%s'\n",
            rmlvo->rules, rmlvo->model, rmlvo->layout, rmlvo->variant,
            rmlvo->options);

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rules_names(keymap->ctx, rmlvo, &kccgst,
                                         &keymap->ಅಂಕೆ_groups);
    ಯದಿ (!ok) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                rmlvo->rules, rmlvo->model, rmlvo->layout, rmlvo->variant,
                rmlvo->options);
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಧ್ರುವ xkb_layout_index_t max_groups = format_max_groups(keymap->format);
    ಯದಿ (keymap->ಅಂಕೆ_groups > max_groups)
        keymap->ಅಂಕೆ_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    ಮುಕ್ತ(kccgst.keycodes);
    ಮುಕ್ತ(kccgst.types);
    ಮುಕ್ತ(kccgst.compatibility);
    ಮುಕ್ತ(kccgst.symbols);
    ಮುಕ್ತ(kccgst.geometry);

    ಯದಿ (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    ಪ್ರತ್ಯಾಗಮ ok;
}

ಸ್ಥಿರ bool
text_v1_keymap_new_from_string(ಸಂರಚನಾ xkb_keymap *keymap,
                               ಧ್ರುವ ವರ್ಣ *string, size_t len)
{
    bool ok;
    XkbFile *xkb_file;

    ಧ್ರುವ ಸಂರಚನಾ parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseString(keymap->ctx, &config,
                              string, len, "(input string)", ರಿಕ್ತ);
    ಯದಿ (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb string\n");
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    ಪ್ರತ್ಯಾಗಮ ok;
}

ಸ್ಥಿರ bool
text_v1_keymap_new_from_file(ಸಂರಚನಾ xkb_keymap *keymap, FILE *file)
{
    bool ok;
    XkbFile *xkb_file;

    ಧ್ರುವ ಸಂರಚನಾ parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseFile(keymap->ctx, &config, file, "(unknown file)", ರಿಕ್ತ);
    ಯದಿ (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb file\n");
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    ಪ್ರತ್ಯಾಗಮ ok;
}

ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_format_ops text_v1_keymap_format_ops = {
    .keymap_new_from_rmlvo = text_v1_keymap_new_from_rmlvo,
    .keymap_new_from_names = text_v1_keymap_new_from_names,
    .keymap_new_from_string = text_v1_keymap_new_from_string,
    .keymap_new_from_file = text_v1_keymap_new_from_file,
    .keymap_serialize = text_v1_keymap_serialize,
};
