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

#అన్తర్భావ "config.h"

#అన్తర్భావ <stdbool.h>

#అన్తర్భావ "darray.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "xkbcomp-priv.h"
#అన్తర్భావ "keymap.h"
#అన్తర్భావ "rules.h"
#అన్తర్భావ "rmlvo.h"

bool
xkb_components_పేర్లు_from_నియమాలు(నిర్మాణం xkb_context *ctx,
                                స్థిరరాశి నిర్మాణం xkb_నియమం_పేర్లు *rmlvo_లోపల,
                                నిర్మాణం xkb_నియమం_పేర్లు *rmlvo_బహిర్గతం,
                                నిర్మాణం xkb_component_పేర్లు *components_బహిర్గతం)
{
    /* Resolve default RMLVO values. We need a mutable copy of the input. */
    నిర్మాణం xkb_నియమం_పేర్లు rmlvo = *rmlvo_లోపల;
    xkb_context_sanitize_నియమం_పేర్లు(ctx, &rmlvo);

    ఒకవేళ (rmlvo_బహిర్గతం) {
        /* Copy the sanitized RMLVO, if required. */
        *rmlvo_బహిర్గతం = rmlvo;
    }
    ఒకవేళ (!components_బహిర్గతం) {
        /* KcCGST not required.
         * While RMLVO and KcCGST outputs are both optional, at least one must
         * be provided. */
        తిరిగి !!rmlvo_బహిర్గతం;
    }

    /* Resolve the RMLVO names to KcCGST components */
    *components_బహిర్గతం = (నిర్మాణం xkb_component_పేర్లు){ 0 };
    తిరిగి xkb_components_from_నియమాలు_పేర్లు(ctx, &rmlvo, components_బహిర్గతం, ఖాళీ);
}

స్థిర bool
compile_keymap_file(నిర్మాణం xkb_keymap *keymap, XkbFile *file)
{
    ఒకవేళ (file->file_type != FILE_TYPE_KEYMAP) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Cannot compile a %s file alone into a keymap\n",
                xkb_file_type_గమ్యం_string(file->file_type));
        తిరిగి false;
    }

    ఒకవేళ (!CompileKeymap(file, keymap)) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to compile keymap\n");
        తిరిగి false;
    }

    తిరిగి true;
}

స్థిర bool
పాఠ్యం_v1_keymap_new_from_rmlvo(నిర్మాణం xkb_keymap *keymap,
                              స్థిరరాశి నిర్మాణం xkb_rmlvo_builder *rmlvo)
{
    bool ok;
    నిర్మాణం xkb_component_పేర్లు kccgst;
    XkbFile *file;

    ఒకవేళ (keymap->ctx->log_level >= XKB_LOG_LEVEL_DEBUG) {
        నిర్మాణం xkb_నియమం_పేర్లు పేర్లు = { 0 };
        స్థిరరాశి కొలత_t buf_కొలత = పరిమాణం(rmlvo->ctx->పాఠ్యం_బఫరు) - 1;
        అక్షరం *buf = xkb_context_get_బఫరు(rmlvo->ctx, buf_కొలత);
        ఒకవేళ (unlikely(!buf))
            తిరిగి false;
        ok = xkb_rmlvo_builder_గమ్యం_నియమాలు_పేర్లు(rmlvo, &పేర్లు, buf, buf_కొలత);
        ఒకవేళ (unlikely(!ok))
            తిరిగి false;
        log_dbg(keymap->ctx, XKB_LOG_సందేశం_NO_ID,
                "Compiling from RMLVO builder: rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                పేర్లు.నియమాలు, పేర్లు.model, పేర్లు.layout, పేర్లు.variant,
                పేర్లు.options);
    }

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rmlvo_builder(rmlvo, &kccgst, &keymap->అంకె_groups);
    ఒకవేళ (!ok) {
        నిర్మాణం xkb_నియమం_పేర్లు పేర్లు = { 0 };
        స్థిరరాశి కొలత_t buf_కొలత = పరిమాణం(rmlvo->ctx->పాఠ్యం_బఫరు);
        అక్షరం *buf = xkb_context_get_బఫరు(rmlvo->ctx, buf_కొలత);
        ఒకవేళ (unlikely(!buf))
            తిరిగి false;
        ok = xkb_rmlvo_builder_గమ్యం_నియమాలు_పేర్లు(rmlvo, &పేర్లు, buf, buf_కొలత);
        ఒకవేళ (unlikely(!ok))
            తిరిగి false;
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                పేర్లు.నియమాలు, పేర్లు.model, పేర్లు.layout, పేర్లు.variant,
                పేర్లు.options);
        తిరిగి false;
    }

    స్థిరరాశి xkb_layout_సూచీ_t max_groups = format_max_groups(keymap->format);
    ఒకవేళ (keymap->అంకె_groups > max_groups)
        keymap->అంకె_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_సందేశం_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    విడుదల(kccgst.keycodes);
    విడుదల(kccgst.types);
    విడుదల(kccgst.compatibility);
    విడుదల(kccgst.symbols);
    విడుదల(kccgst.geometry);

    ఒకవేళ (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        తిరిగి false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    తిరిగి ok;
}

స్థిర bool
పాఠ్యం_v1_keymap_new_from_పేర్లు(నిర్మాణం xkb_keymap *keymap,
                              స్థిరరాశి నిర్మాణం xkb_నియమం_పేర్లు *rmlvo)
{
    bool ok;
    నిర్మాణం xkb_component_పేర్లు kccgst;
    XkbFile *file;

    log_dbg(keymap->ctx, XKB_LOG_సందేశం_NO_ID,
            "Compiling from RMLVO: rules '%s', model '%s', layout '%s', "
            "variant '%s', options '%s'\n",
            rmlvo->నియమాలు, rmlvo->model, rmlvo->layout, rmlvo->variant,
            rmlvo->options);

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_నియమాలు_పేర్లు(keymap->ctx, rmlvo, &kccgst,
                                         &keymap->అంకె_groups);
    ఒకవేళ (!ok) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                rmlvo->నియమాలు, rmlvo->model, rmlvo->layout, rmlvo->variant,
                rmlvo->options);
        తిరిగి false;
    }

    స్థిరరాశి xkb_layout_సూచీ_t max_groups = format_max_groups(keymap->format);
    ఒకవేళ (keymap->అంకె_groups > max_groups)
        keymap->అంకె_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_సందేశం_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    విడుదల(kccgst.keycodes);
    విడుదల(kccgst.types);
    విడుదల(kccgst.compatibility);
    విడుదల(kccgst.symbols);
    విడుదల(kccgst.geometry);

    ఒకవేళ (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        తిరిగి false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    తిరిగి ok;
}

స్థిర bool
పాఠ్యం_v1_keymap_new_from_string(నిర్మాణం xkb_keymap *keymap,
                               స్థిరరాశి అక్షరం *string, కొలత_t len)
{
    bool ok;
    XkbFile *xkb_file;

    స్థిరరాశి నిర్మాణం parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseString(keymap->ctx, &config,
                              string, len, "(input string)", ఖాళీ);
    ఒకవేళ (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb string\n");
        తిరిగి false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    తిరిగి ok;
}

స్థిర bool
పాఠ్యం_v1_keymap_new_from_file(నిర్మాణం xkb_keymap *keymap, FILE *file)
{
    bool ok;
    XkbFile *xkb_file;

    స్థిరరాశి నిర్మాణం parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseFile(keymap->ctx, &config, file, "(unknown file)", ఖాళీ);
    ఒకవేళ (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb file\n");
        తిరిగి false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    తిరిగి ok;
}

స్థిరరాశి నిర్మాణం xkb_keymap_format_ops పాఠ్యం_v1_keymap_format_ops = {
    .keymap_new_from_rmlvo = పాఠ్యం_v1_keymap_new_from_rmlvo,
    .keymap_new_from_పేర్లు = పాఠ్యం_v1_keymap_new_from_పేర్లు,
    .keymap_new_from_string = పాఠ్యం_v1_keymap_new_from_string,
    .keymap_new_from_file = పాఠ్యం_v1_keymap_new_from_file,
    .keymap_serialize = పాఠ్యం_v1_keymap_serialize,
};
