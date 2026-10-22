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

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <stdbool.h>

#અન્તર્ભાવ "darray.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "xkbcomp-priv.h"
#અન્તર્ભાવ "keymap.h"
#અન્તર્ભાવ "rules.h"
#અન્તર્ભાવ "rmlvo.h"

bool
xkb_components_names_from_rules(સંરચના xkb_context *ctx,
                                અચલ સંરચના xkb_rule_names *rmlvo_in,
                                સંરચના xkb_rule_names *rmlvo_out,
                                સંરચના xkb_component_names *components_out)
{
    /* Resolve default RMLVO values. We need a mutable copy of the input. */
    સંરચના xkb_rule_names rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    જો (rmlvo_out) {
        /* Copy the sanitized RMLVO, if required. */
        *rmlvo_out = rmlvo;
    }
    જો (!components_out) {
        /* KcCGST not required.
         * While RMLVO and KcCGST outputs are both optional, at least one must
         * be provided. */
        પાછા !!rmlvo_out;
    }

    /* Resolve the RMLVO names to KcCGST components */
    *components_out = (સંરચના xkb_component_names){ 0 };
    પાછા xkb_components_from_rules_names(ctx, &rmlvo, components_out, ખાલી);
}

સ્થિર bool
compile_keymap_file(સંરચના xkb_keymap *keymap, XkbFile *file)
{
    જો (file->file_type != FILE_TYPE_KEYMAP) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Cannot compile a %s file alone into a keymap\n",
                xkb_file_type_to_string(file->file_type));
        પાછા false;
    }

    જો (!CompileKeymap(file, keymap)) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to compile keymap\n");
        પાછા false;
    }

    પાછા true;
}

સ્થિર bool
text_v1_keymap_new_from_rmlvo(સંરચના xkb_keymap *keymap,
                              અચલ સંરચના xkb_rmlvo_builder *rmlvo)
{
    bool ok;
    સંરચના xkb_component_names kccgst;
    XkbFile *file;

    જો (keymap->ctx->log_level >= XKB_LOG_LEVEL_DEBUG) {
        સંરચના xkb_rule_names names = { 0 };
        અચલ size_t buf_size = કદ(rmlvo->ctx->text_buffer) - 1;
        અક્ષર *buf = xkb_context_get_buffer(rmlvo->ctx, buf_size);
        જો (unlikely(!buf))
            પાછા false;
        ok = xkb_rmlvo_builder_to_rules_names(rmlvo, &names, buf, buf_size);
        જો (unlikely(!ok))
            પાછા false;
        log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Compiling from RMLVO builder: rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                names.rules, names.model, names.layout, names.variant,
                names.options);
    }

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rmlvo_builder(rmlvo, &kccgst, &keymap->અંક_groups);
    જો (!ok) {
        સંરચના xkb_rule_names names = { 0 };
        અચલ size_t buf_size = કદ(rmlvo->ctx->text_buffer);
        અક્ષર *buf = xkb_context_get_buffer(rmlvo->ctx, buf_size);
        જો (unlikely(!buf))
            પાછા false;
        ok = xkb_rmlvo_builder_to_rules_names(rmlvo, &names, buf, buf_size);
        જો (unlikely(!ok))
            પાછા false;
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                names.rules, names.model, names.layout, names.variant,
                names.options);
        પાછા false;
    }

    અચલ xkb_layout_index_t max_groups = format_max_groups(keymap->format);
    જો (keymap->અંક_groups > max_groups)
        keymap->અંક_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    મુક્ત(kccgst.keycodes);
    મુક્ત(kccgst.types);
    મુક્ત(kccgst.compatibility);
    મુક્ત(kccgst.symbols);
    મુક્ત(kccgst.geometry);

    જો (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        પાછા false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    પાછા ok;
}

સ્થિર bool
text_v1_keymap_new_from_names(સંરચના xkb_keymap *keymap,
                              અચલ સંરચના xkb_rule_names *rmlvo)
{
    bool ok;
    સંરચના xkb_component_names kccgst;
    XkbFile *file;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from RMLVO: rules '%s', model '%s', layout '%s', "
            "variant '%s', options '%s'\n",
            rmlvo->rules, rmlvo->model, rmlvo->layout, rmlvo->variant,
            rmlvo->options);

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rules_names(keymap->ctx, rmlvo, &kccgst,
                                         &keymap->અંક_groups);
    જો (!ok) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                rmlvo->rules, rmlvo->model, rmlvo->layout, rmlvo->variant,
                rmlvo->options);
        પાછા false;
    }

    અચલ xkb_layout_index_t max_groups = format_max_groups(keymap->format);
    જો (keymap->અંક_groups > max_groups)
        keymap->અંક_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    મુક્ત(kccgst.keycodes);
    મુક્ત(kccgst.types);
    મુક્ત(kccgst.compatibility);
    મુક્ત(kccgst.symbols);
    મુક્ત(kccgst.geometry);

    જો (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        પાછા false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    પાછા ok;
}

સ્થિર bool
text_v1_keymap_new_from_string(સંરચના xkb_keymap *keymap,
                               અચલ અક્ષર *string, size_t len)
{
    bool ok;
    XkbFile *xkb_file;

    અચલ સંરચના parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseString(keymap->ctx, &config,
                              string, len, "(input string)", ખાલી);
    જો (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb string\n");
        પાછા false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    પાછા ok;
}

સ્થિર bool
text_v1_keymap_new_from_file(સંરચના xkb_keymap *keymap, FILE *file)
{
    bool ok;
    XkbFile *xkb_file;

    અચલ સંરચના parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseFile(keymap->ctx, &config, file, "(unknown file)", ખાલી);
    જો (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb file\n");
        પાછા false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    પાછા ok;
}

અચલ સંરચના xkb_keymap_format_ops text_v1_keymap_format_ops = {
    .keymap_new_from_rmlvo = text_v1_keymap_new_from_rmlvo,
    .keymap_new_from_names = text_v1_keymap_new_from_names,
    .keymap_new_from_string = text_v1_keymap_new_from_string,
    .keymap_new_from_file = text_v1_keymap_new_from_file,
    .keymap_serialize = text_v1_keymap_serialize,
};
