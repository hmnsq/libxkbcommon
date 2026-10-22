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
xkb_components_names_from_rules(কাঠামো xkb_context *ctx,
                                ধ্রুবক কাঠামো xkb_rule_names *rmlvo_in,
                                কাঠামো xkb_rule_names *rmlvo_out,
                                কাঠামো xkb_component_names *components_out)
{
    /* Resolve default RMLVO values. We need a mutable copy of the input. */
    কাঠামো xkb_rule_names rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    যদি (rmlvo_out) {
        /* Copy the sanitized RMLVO, if required. */
        *rmlvo_out = rmlvo;
    }
    যদি (!components_out) {
        /* KcCGST not required.
         * While RMLVO and KcCGST outputs are both optional, at least one must
         * be provided. */
        ফেরত !!rmlvo_out;
    }

    /* Resolve the RMLVO names to KcCGST components */
    *components_out = (কাঠামো xkb_component_names){ 0 };
    ফেরত xkb_components_from_rules_names(ctx, &rmlvo, components_out, ফাঁকা);
}

স্থির bool
compile_keymap_file(কাঠামো xkb_keymap *keymap, XkbFile *file)
{
    যদি (file->file_type != FILE_TYPE_KEYMAP) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Cannot compile a %s file alone into a keymap\n",
                xkb_file_type_to_string(file->file_type));
        ফেরত false;
    }

    যদি (!CompileKeymap(file, keymap)) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to compile keymap\n");
        ফেরত false;
    }

    ফেরত true;
}

স্থির bool
text_v1_keymap_new_from_rmlvo(কাঠামো xkb_keymap *keymap,
                              ধ্রুবক কাঠামো xkb_rmlvo_builder *rmlvo)
{
    bool ok;
    কাঠামো xkb_component_names kccgst;
    XkbFile *file;

    যদি (keymap->ctx->log_level >= XKB_LOG_LEVEL_DEBUG) {
        কাঠামো xkb_rule_names names = { 0 };
        ধ্রুবক আকার_t buf_আকার = আকার(rmlvo->ctx->text_বাফার) - 1;
        অক্ষর *buf = xkb_context_get_বাফার(rmlvo->ctx, buf_আকার);
        যদি (unlikely(!buf))
            ফেরত false;
        ok = xkb_rmlvo_builder_to_rules_names(rmlvo, &names, buf, buf_আকার);
        যদি (unlikely(!ok))
            ফেরত false;
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
        কাঠামো xkb_rule_names names = { 0 };
        ধ্রুবক আকার_t buf_আকার = আকার(rmlvo->ctx->text_বাফার);
        অক্ষর *buf = xkb_context_get_বাফার(rmlvo->ctx, buf_আকার);
        যদি (unlikely(!buf))
            ফেরত false;
        ok = xkb_rmlvo_builder_to_rules_names(rmlvo, &names, buf, buf_আকার);
        যদি (unlikely(!ok))
            ফেরত false;
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                names.rules, names.model, names.layout, names.variant,
                names.options);
        ফেরত false;
    }

    ধ্রুবক xkb_layout_সূচক_t max_groups = format_max_groups(keymap->format);
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
        ফেরত false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    ফেরত ok;
}

স্থির bool
text_v1_keymap_new_from_names(কাঠামো xkb_keymap *keymap,
                              ধ্রুবক কাঠামো xkb_rule_names *rmlvo)
{
    bool ok;
    কাঠামো xkb_component_names kccgst;
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
        ফেরত false;
    }

    ধ্রুবক xkb_layout_সূচক_t max_groups = format_max_groups(keymap->format);
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
        ফেরত false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    ফেরত ok;
}

স্থির bool
text_v1_keymap_new_from_string(কাঠামো xkb_keymap *keymap,
                               ধ্রুবক অক্ষর *string, আকার_t len)
{
    bool ok;
    XkbFile *xkb_file;

    ধ্রুবক কাঠামো parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseString(keymap->ctx, &config,
                              string, len, "(input string)", ফাঁকা);
    যদি (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb string\n");
        ফেরত false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    ফেরত ok;
}

স্থির bool
text_v1_keymap_new_from_file(কাঠামো xkb_keymap *keymap, FILE *file)
{
    bool ok;
    XkbFile *xkb_file;

    ধ্রুবক কাঠামো parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseFile(keymap->ctx, &config, file, "(unknown file)", ফাঁকা);
    যদি (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb file\n");
        ফেরত false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    ফেরত ok;
}

ধ্রুবক কাঠামো xkb_keymap_format_ops text_v1_keymap_format_ops = {
    .keymap_new_from_rmlvo = text_v1_keymap_new_from_rmlvo,
    .keymap_new_from_names = text_v1_keymap_new_from_names,
    .keymap_new_from_string = text_v1_keymap_new_from_string,
    .keymap_new_from_file = text_v1_keymap_new_from_file,
    .keymap_serialize = text_v1_keymap_serialize,
};
