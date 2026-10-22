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
xkb_components_ಹೆಸರುಗಳು_from_ನಿಯಮಗಳು(ರಚನೆ xkb_context *ctx,
                                ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ನಿಯಮ_ಹೆಸರುಗಳು *rmlvo_ಒಳ,
                                ರಚನೆ xkb_ನಿಯಮ_ಹೆಸರುಗಳು *rmlvo_ಹೊರಹೋಗು,
                                ರಚನೆ xkb_component_ಹೆಸರುಗಳು *components_ಹೊರಹೋಗು)
{
    /* Resolve default RMLVO values. We need a mutable copy of the input. */
    ರಚನೆ xkb_ನಿಯಮ_ಹೆಸರುಗಳು rmlvo = *rmlvo_ಒಳ;
    xkb_context_sanitize_ನಿಯಮ_ಹೆಸರುಗಳು(ctx, &rmlvo);

    ಆದರೆ (rmlvo_ಹೊರಹೋಗು) {
        /* Copy the sanitized RMLVO, if required. */
        *rmlvo_ಹೊರಹೋಗು = rmlvo;
    }
    ಆದರೆ (!components_ಹೊರಹೋಗು) {
        /* KcCGST not required.
         * While RMLVO and KcCGST outputs are both optional, at least one must
         * be provided. */
        ಹಿಂತಿರುಗಿ !!rmlvo_ಹೊರಹೋಗು;
    }

    /* Resolve the RMLVO names to KcCGST components */
    *components_ಹೊರಹೋಗು = (ರಚನೆ xkb_component_ಹೆಸರುಗಳು){ 0 };
    ಹಿಂತಿರುಗಿ xkb_components_from_ನಿಯಮಗಳು_ಹೆಸರುಗಳು(ctx, &rmlvo, components_ಹೊರಹೋಗು, ಖಾಲಿ);
}

ಸ್ಥಿರ bool
compile_keymap_file(ರಚನೆ xkb_keymap *keymap, XkbFile *file)
{
    ಆದರೆ (file->file_type != FILE_TYPE_KEYMAP) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Cannot compile a %s file alone into a keymap\n",
                xkb_file_type_ಗಮ್ಯ_string(file->file_type));
        ಹಿಂತಿರುಗಿ false;
    }

    ಆದರೆ (!CompileKeymap(file, keymap)) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to compile keymap\n");
        ಹಿಂತಿರುಗಿ false;
    }

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
ಪಠ್ಯ_v1_keymap_new_from_rmlvo(ರಚನೆ xkb_keymap *keymap,
                              ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_rmlvo_builder *rmlvo)
{
    bool ok;
    ರಚನೆ xkb_component_ಹೆಸರುಗಳು kccgst;
    XkbFile *file;

    ಆದರೆ (keymap->ctx->log_level >= XKB_LOG_LEVEL_DEBUG) {
        ರಚನೆ xkb_ನಿಯಮ_ಹೆಸರುಗಳು ಹೆಸರುಗಳು = { 0 };
        ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t buf_ಗಾತ್ರ = ಗಾತ್ರ(rmlvo->ctx->ಪಠ್ಯ_ಬಫರ್) - 1;
        ಅಕ್ಷರ *buf = xkb_context_get_ಬಫರ್(rmlvo->ctx, buf_ಗಾತ್ರ);
        ಆದರೆ (unlikely(!buf))
            ಹಿಂತಿರುಗಿ false;
        ok = xkb_rmlvo_builder_ಗಮ್ಯ_ನಿಯಮಗಳು_ಹೆಸರುಗಳು(rmlvo, &ಹೆಸರುಗಳು, buf, buf_ಗಾತ್ರ);
        ಆದರೆ (unlikely(!ok))
            ಹಿಂತಿರುಗಿ false;
        log_dbg(keymap->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Compiling from RMLVO builder: rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                ಹೆಸರುಗಳು.ನಿಯಮಗಳು, ಹೆಸರುಗಳು.model, ಹೆಸರುಗಳು.layout, ಹೆಸರುಗಳು.variant,
                ಹೆಸರುಗಳು.options);
    }

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_rmlvo_builder(rmlvo, &kccgst, &keymap->ಅಂಕೆ_groups);
    ಆದರೆ (!ok) {
        ರಚನೆ xkb_ನಿಯಮ_ಹೆಸರುಗಳು ಹೆಸರುಗಳು = { 0 };
        ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t buf_ಗಾತ್ರ = ಗಾತ್ರ(rmlvo->ctx->ಪಠ್ಯ_ಬಫರ್);
        ಅಕ್ಷರ *buf = xkb_context_get_ಬಫರ್(rmlvo->ctx, buf_ಗಾತ್ರ);
        ಆದರೆ (unlikely(!buf))
            ಹಿಂತಿರುಗಿ false;
        ok = xkb_rmlvo_builder_ಗಮ್ಯ_ನಿಯಮಗಳು_ಹೆಸರುಗಳು(rmlvo, &ಹೆಸರುಗಳು, buf, buf_ಗಾತ್ರ);
        ಆದರೆ (unlikely(!ok))
            ಹಿಂತಿರುಗಿ false;
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                ಹೆಸರುಗಳು.ನಿಯಮಗಳು, ಹೆಸರುಗಳು.model, ಹೆಸರುಗಳು.layout, ಹೆಸರುಗಳು.variant,
                ಹೆಸರುಗಳು.options);
        ಹಿಂತಿರುಗಿ false;
    }

    ಸ್ಥಿರಾಂಕ xkb_layout_ಸೂಚ್ಯಂಕ_t max_groups = format_max_groups(keymap->format);
    ಆದರೆ (keymap->ಅಂಕೆ_groups > max_groups)
        keymap->ಅಂಕೆ_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    ಬಿಡುಗಡೆ(kccgst.keycodes);
    ಬಿಡುಗಡೆ(kccgst.types);
    ಬಿಡುಗಡೆ(kccgst.compatibility);
    ಬಿಡುಗಡೆ(kccgst.symbols);
    ಬಿಡುಗಡೆ(kccgst.geometry);

    ಆದರೆ (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        ಹಿಂತಿರುಗಿ false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    ಹಿಂತಿರುಗಿ ok;
}

ಸ್ಥಿರ bool
ಪಠ್ಯ_v1_keymap_new_from_ಹೆಸರುಗಳು(ರಚನೆ xkb_keymap *keymap,
                              ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ನಿಯಮ_ಹೆಸರುಗಳು *rmlvo)
{
    bool ok;
    ರಚನೆ xkb_component_ಹೆಸರುಗಳು kccgst;
    XkbFile *file;

    log_dbg(keymap->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
            "Compiling from RMLVO: rules '%s', model '%s', layout '%s', "
            "variant '%s', options '%s'\n",
            rmlvo->ನಿಯಮಗಳು, rmlvo->model, rmlvo->layout, rmlvo->variant,
            rmlvo->options);

    /* Resolve the RMLVO components to KcCGST components and get the
     * expected number of layouts */
    ok = xkb_components_from_ನಿಯಮಗಳು_ಹೆಸರುಗಳು(keymap->ctx, rmlvo, &kccgst,
                                         &keymap->ಅಂಕೆ_groups);
    ಆದರೆ (!ok) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Couldn't look up rules '%s', model '%s', layout '%s', "
                "variant '%s', options '%s'\n",
                rmlvo->ನಿಯಮಗಳು, rmlvo->model, rmlvo->layout, rmlvo->variant,
                rmlvo->options);
        ಹಿಂತಿರುಗಿ false;
    }

    ಸ್ಥಿರಾಂಕ xkb_layout_ಸೂಚ್ಯಂಕ_t max_groups = format_max_groups(keymap->format);
    ಆದರೆ (keymap->ಅಂಕೆ_groups > max_groups)
        keymap->ಅಂಕೆ_groups = max_groups;

    log_dbg(keymap->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
            "Compiling from KcCGST: keycodes '%s', types '%s', "
            "compat '%s', symbols '%s'\n",
            kccgst.keycodes, kccgst.types, kccgst.compatibility, kccgst.symbols);

    file = XkbFileFromComponents(keymap->ctx, &kccgst);

    ಬಿಡುಗಡೆ(kccgst.keycodes);
    ಬಿಡುಗಡೆ(kccgst.types);
    ಬಿಡುಗಡೆ(kccgst.compatibility);
    ಬಿಡುಗಡೆ(kccgst.symbols);
    ಬಿಡುಗಡೆ(kccgst.geometry);

    ಆದರೆ (!file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to generate parsed XKB file from components\n");
        ಹಿಂತಿರುಗಿ false;
    }

    ok = compile_keymap_file(keymap, file);
    FreeXkbFile(file);
    ಹಿಂತಿರುಗಿ ok;
}

ಸ್ಥಿರ bool
ಪಠ್ಯ_v1_keymap_new_from_string(ರಚನೆ xkb_keymap *keymap,
                               ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string, ಗಾತ್ರ_t len)
{
    bool ok;
    XkbFile *xkb_file;

    ಸ್ಥಿರಾಂಕ ರಚನೆ parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseString(keymap->ctx, &config,
                              string, len, "(input string)", ಖಾಲಿ);
    ಆದರೆ (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb string\n");
        ಹಿಂತಿರುಗಿ false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    ಹಿಂತಿರುಗಿ ok;
}

ಸ್ಥಿರ bool
ಪಠ್ಯ_v1_keymap_new_from_file(ರಚನೆ xkb_keymap *keymap, FILE *file)
{
    bool ok;
    XkbFile *xkb_file;

    ಸ್ಥಿರಾಂಕ ರಚನೆ parser_keymap_config config = {
        .format = keymap->format,
        .strict = parser_strict_flags_from_keymap(keymap)
    };

    xkb_file = XkbParseFile(keymap->ctx, &config, file, "(unknown file)", ಖಾಲಿ);
    ಆದರೆ (!xkb_file) {
        log_err(keymap->ctx, XKB_ERROR_KEYMAP_COMPILATION_FAILED,
                "Failed to parse input xkb file\n");
        ಹಿಂತಿರುಗಿ false;
    }

    ok = compile_keymap_file(keymap, xkb_file);
    FreeXkbFile(xkb_file);
    ಹಿಂತಿರುಗಿ ok;
}

ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_format_ops ಪಠ್ಯ_v1_keymap_format_ops = {
    .keymap_new_from_rmlvo = ಪಠ್ಯ_v1_keymap_new_from_rmlvo,
    .keymap_new_from_ಹೆಸರುಗಳು = ಪಠ್ಯ_v1_keymap_new_from_ಹೆಸರುಗಳು,
    .keymap_new_from_string = ಪಠ್ಯ_v1_keymap_new_from_string,
    .keymap_new_from_file = ಪಠ್ಯ_v1_keymap_new_from_file,
    .keymap_serialize = ಪಠ್ಯ_v1_keymap_serialize,
};
