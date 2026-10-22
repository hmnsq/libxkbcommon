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

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <stdbool.h>

#ଅନ୍ତର୍ଭାଵ "darray.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp-priv.h"
#ଅନ୍ତର୍ଭାଵ "keymap.h"
#ଅନ୍ତର୍ଭାଵ "rules.h"
#ଅନ୍ତର୍ଭାଵ "rmlvo.h"

bool
xkb_components_names_from_rules(ସଂରଚନା xkb_context *ctx,
                                ଅଚଳ ସଂରଚନା xkb_rule_names *rmlvo_in,
                                ସଂରଚନା xkb_rule_names *rmlvo_out,
                                ସଂରଚନା xkb_component_names *components_out)
{
    /* Resolve default RMLVO values. We need a mutable copy of the input. */
    ସଂରଚନା xkb_rule_names rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    ଯଦି (rmlvo_out) {
        /* Copy the sanitized RMLVO, if required. */
        *rmlvo_out = rmlvo;
    }
    ଯଦି (!components_out) {
        /* KcCGST not required.
         * While RMLVO and KcCGST outputs are both optional, at least one must
         * be provided. */
        ଫେରନ୍ତୁ !!rmlvo_out;
    }

    /* Resolve the RMLVO names to KcCGST components */
    *components_out = (ସଂରଚନା xkb_component_names){ 0 };
    ଫେରନ୍ତୁ xkb_components_from_rules_names(ctx, &rmlvo, components_out, ଖାଲି);
}

ସ୍ଥିର bool
compile_keymap_file(ସଂରଚନା xkb_keymap *keymap, XkbFile *file)
{
    ଯଦି (file->file_type != FILE_TYPE_KEYMAP) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Cannot compile a %s file alone into a keymap\n",
                xkb_file_type_to_string(file->file_type));
        ଫେରନ୍ତୁ false;
    }

    ଯଦି (!CompileKeymap(file, keymap)) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to compile keymap\n");
        ଫେରନ୍ତୁ false;
    }

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
text_v1_keymap_new_from_rmlvo(ସଂରଚନା xkb_keymap *keymap,
                              ଅଚଳ ସଂରଚନା xkb_rmlvo_builder *rmlvo)
{
    bool ok;
    ସଂରଚନା xkb_component_names kccgst;
    XkbFile *file;

    ଯଦି (keymap->ctx->log_level >= XKB_LOG_LEVEL_DEBUG) {
        ସଂରଚନା xkb_rule_names names = { 0 };
        ଅଚଳ size_t buf_size = ଆକାର(rmlvo->ctx->text_buffer) - 1;
        ଅକ୍ଷର *buf = xkb_context_get_buffer(rmlvo->ctx, buf_size);
        ଯଦି (unlikely(!buf))
            ଫେରନ୍ତୁ false;
        ok = xkb_rmlvo_builder_to_rules_names(rmlvo, &names, buf, buf_size);
        ଯଦି (unlikely(!ok))
            ଫେରନ୍ତୁ false;
        log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Compiling from RMLVO builder: rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                names.rules, names.model, names.layout, names.variant,
                names.options);
    }

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rmlvo_builder(rmlvo, &kccgst, &keymap->ଅଙ୍କ_groups);
    ଯଦି (!ok) {
        ସଂରଚନା xkb_rule_names names = { 0 };
        ଅଚଳ size_t buf_size = ଆକାର(rmlvo->ctx->text_buffer);
        ଅକ୍ଷର *buf = xkb_context_get_buffer(rmlvo->ctx, buf_size);
        ଯଦି (unlikely(!buf))
            ଫେରନ୍ତୁ false;
        ok = xkb_rmlvo_builder_to_rules_names(rmlvo, &names, buf, buf_size);
        ଯଦି (unlikely(!ok))
            ଫେରନ୍ତୁ false;
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                names.rules, names.model, names.layout, names.variant,
                names.options);
        ଫେରନ୍ତୁ false;
    }

    ଅଚଳ xkb_layout_index_t max_groups = format_max_groups(keymap->format);
    ଯଦି (keymap->ଅଙ୍କ_groups > max_groups)
        keymap->ଅଙ୍କ_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    ମୁକ୍ତ(kccgst.keycodes);
    ମୁକ୍ତ(kccgst.types);
    ମୁକ୍ତ(kccgst.compatibility);
    ମୁକ୍ତ(kccgst.symbols);
    ମୁକ୍ତ(kccgst.geometry);

    ଯଦି (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        ଫେରନ୍ତୁ false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    ଫେରନ୍ତୁ ok;
}

ସ୍ଥିର bool
text_v1_keymap_new_from_names(ସଂରଚନା xkb_keymap *keymap,
                              ଅଚଳ ସଂରଚନା xkb_rule_names *rmlvo)
{
    bool ok;
    ସଂରଚନା xkb_component_names kccgst;
    XkbFile *file;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from RMLVO: rules '%s', model '%s', layout '%s', "
            "variant '%s', options '%s'\n",
            rmlvo->rules, rmlvo->model, rmlvo->layout, rmlvo->variant,
            rmlvo->options);

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rules_names(keymap->ctx, rmlvo, &kccgst,
                                         &keymap->ଅଙ୍କ_groups);
    ଯଦି (!ok) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                rmlvo->rules, rmlvo->model, rmlvo->layout, rmlvo->variant,
                rmlvo->options);
        ଫେରନ୍ତୁ false;
    }

    ଅଚଳ xkb_layout_index_t max_groups = format_max_groups(keymap->format);
    ଯଦି (keymap->ଅଙ୍କ_groups > max_groups)
        keymap->ଅଙ୍କ_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    ମୁକ୍ତ(kccgst.keycodes);
    ମୁକ୍ତ(kccgst.types);
    ମୁକ୍ତ(kccgst.compatibility);
    ମୁକ୍ତ(kccgst.symbols);
    ମୁକ୍ତ(kccgst.geometry);

    ଯଦି (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        ଫେରନ୍ତୁ false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    ଫେରନ୍ତୁ ok;
}

ସ୍ଥିର bool
text_v1_keymap_new_from_string(ସଂରଚନା xkb_keymap *keymap,
                               ଅଚଳ ଅକ୍ଷର *string, size_t len)
{
    bool ok;
    XkbFile *xkb_file;

    ଅଚଳ ସଂରଚନା parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseString(keymap->ctx, &config,
                              string, len, "(input string)", ଖାଲି);
    ଯଦି (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb string\n");
        ଫେରନ୍ତୁ false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    ଫେରନ୍ତୁ ok;
}

ସ୍ଥିର bool
text_v1_keymap_new_from_file(ସଂରଚନା xkb_keymap *keymap, FILE *file)
{
    bool ok;
    XkbFile *xkb_file;

    ଅଚଳ ସଂରଚନା parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseFile(keymap->ctx, &config, file, "(unknown file)", ଖାଲି);
    ଯଦି (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb file\n");
        ଫେରନ୍ତୁ false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    ଫେରନ୍ତୁ ok;
}

ଅଚଳ ସଂରଚନା xkb_keymap_format_ops text_v1_keymap_format_ops = {
    .keymap_new_from_rmlvo = text_v1_keymap_new_from_rmlvo,
    .keymap_new_from_names = text_v1_keymap_new_from_names,
    .keymap_new_from_string = text_v1_keymap_new_from_string,
    .keymap_new_from_file = text_v1_keymap_new_from_file,
    .keymap_serialize = text_v1_keymap_serialize,
};
