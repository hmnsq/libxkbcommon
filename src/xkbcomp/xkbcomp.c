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

#ضمن "config.h"

#ضمن <stdbool.h>

#ضمن "darray.h"
#ضمن "utils.h"
#ضمن "xkbcommon/xkbcommon.h"
#ضمن "xkbcomp-priv.h"
#ضمن "keymap.h"
#ضمن "rules.h"
#ضمن "rmlvo.h"

bool
xkb_components_names_from_rules(بنية xkb_context *ctx,
                                ثابت بنية xkb_rule_names *rmlvo_in,
                                بنية xkb_rule_names *rmlvo_out,
                                بنية xkb_component_names *components_out)
{
    /* Resolve default RMLVO values. We need a mutable copy of the input. */
    بنية xkb_rule_names rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    אם (rmlvo_out) {
        /* Copy the sanitized RMLVO, if required. */
        *rmlvo_out = rmlvo;
    }
    אם (!components_out) {
        /* KcCGST not required.
         * While RMLVO and KcCGST outputs are both optional, at least one must
         * be provided. */
        החזר !!rmlvo_out;
    }

    /* Resolve the RMLVO names to KcCGST components */
    *components_out = (بنية xkb_component_names){ 0 };
    החזר xkb_components_from_rules_names(ctx, &rmlvo, components_out, عدم);
}

ساكن bool
compile_keymap_file(بنية xkb_keymap *keymap, XkbFile *file)
{
    אם (file->file_type != FILE_TYPE_KEYMAP) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Cannot compile a %s file alone into a keymap\n",
                xkb_file_type_to_string(file->file_type));
        החזר false;
    }

    אם (!CompileKeymap(file, keymap)) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to compile keymap\n");
        החזר false;
    }

    החזר true;
}

ساكن bool
text_v1_keymap_new_from_rmlvo(بنية xkb_keymap *keymap,
                              ثابت بنية xkb_rmlvo_builder *rmlvo)
{
    bool ok;
    بنية xkb_component_names kccgst;
    XkbFile *file;

    אם (keymap->ctx->log_level >= XKB_LOG_LEVEL_DEBUG) {
        بنية xkb_rule_names names = { 0 };
        ثابت size_t buf_size = حجم(rmlvo->ctx->text_buffer) - 1;
        حرف *buf = xkb_context_get_buffer(rmlvo->ctx, buf_size);
        אם (unlikely(!buf))
            החזר false;
        ok = xkb_rmlvo_builder_to_rules_names(rmlvo, &names, buf, buf_size);
        אם (unlikely(!ok))
            החזר false;
        log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Compiling from RMLVO builder: rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                names.rules, names.model, names.layout, names.variant,
                names.options);
    }

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rmlvo_builder(rmlvo, &kccgst, &keymap->num_groups);
    אם (!ok) {
        بنية xkb_rule_names names = { 0 };
        ثابت size_t buf_size = حجم(rmlvo->ctx->text_buffer);
        حرف *buf = xkb_context_get_buffer(rmlvo->ctx, buf_size);
        אם (unlikely(!buf))
            החזר false;
        ok = xkb_rmlvo_builder_to_rules_names(rmlvo, &names, buf, buf_size);
        אם (unlikely(!ok))
            החזר false;
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                names.rules, names.model, names.layout, names.variant,
                names.options);
        החזר false;
    }

    ثابت xkb_layout_index_t max_groups = format_max_groups(keymap->format);
    אם (keymap->num_groups > max_groups)
        keymap->num_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    حرر(kccgst.keycodes);
    حرر(kccgst.types);
    حرر(kccgst.compatibility);
    حرر(kccgst.symbols);
    حرر(kccgst.geometry);

    אם (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        החזר false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    החזר ok;
}

ساكن bool
text_v1_keymap_new_from_names(بنية xkb_keymap *keymap,
                              ثابت بنية xkb_rule_names *rmlvo)
{
    bool ok;
    بنية xkb_component_names kccgst;
    XkbFile *file;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from RMLVO: rules '%s', model '%s', layout '%s', "
            "variant '%s', options '%s'\n",
            rmlvo->rules, rmlvo->model, rmlvo->layout, rmlvo->variant,
            rmlvo->options);

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rules_names(keymap->ctx, rmlvo, &kccgst,
                                         &keymap->num_groups);
    אם (!ok) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                rmlvo->rules, rmlvo->model, rmlvo->layout, rmlvo->variant,
                rmlvo->options);
        החזר false;
    }

    ثابت xkb_layout_index_t max_groups = format_max_groups(keymap->format);
    אם (keymap->num_groups > max_groups)
        keymap->num_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    حرر(kccgst.keycodes);
    حرر(kccgst.types);
    حرر(kccgst.compatibility);
    حرر(kccgst.symbols);
    حرر(kccgst.geometry);

    אם (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        החזר false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    החזר ok;
}

ساكن bool
text_v1_keymap_new_from_string(بنية xkb_keymap *keymap,
                               ثابت حرف *string, size_t len)
{
    bool ok;
    XkbFile *xkb_file;

    ثابت بنية parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseString(keymap->ctx, &config,
                              string, len, "(input string)", عدم);
    אם (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb string\n");
        החזר false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    החזר ok;
}

ساكن bool
text_v1_keymap_new_from_file(بنية xkb_keymap *keymap, FILE *file)
{
    bool ok;
    XkbFile *xkb_file;

    ثابت بنية parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseFile(keymap->ctx, &config, file, "(unknown file)", عدم);
    אם (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb file\n");
        החזר false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    החזר ok;
}

ثابت بنية xkb_keymap_format_ops text_v1_keymap_format_ops = {
    .keymap_new_from_rmlvo = text_v1_keymap_new_from_rmlvo,
    .keymap_new_from_names = text_v1_keymap_new_from_names,
    .keymap_new_from_string = text_v1_keymap_new_from_string,
    .keymap_new_from_file = text_v1_keymap_new_from_file,
    .keymap_serialize = text_v1_keymap_serialize,
};
