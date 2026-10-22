/*
 * For HPND:
 * Copyright (c) 1993 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <string.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "abi-check.h"
#ಅನ್ತರ್ಭಾವ "atom.h"
#ಅನ್ತರ್ಭಾವ "context.h"
#ಅನ್ತರ್ಭಾವ "features/enums.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "keymap-priv.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "text.h"
#ಅನ್ತರ್ಭಾವ "util-mem.h"
#ಅನ್ತರ್ಭಾವ "util-numbers.h"

ರಚನೆ xkb_keymap *
xkb_keymap_ref(ರಚನೆ xkb_keymap *keymap)
{
    ಪ್ರತಿಜ್ಞಾ(keymap->refcnt > 0);
    keymap->refcnt++;
    ಹಿಂತಿರುಗಿ keymap;
}

ಶೂನ್ಯ
clear_level(ರಚನೆ xkb_level *leveli)
{
    ಆದರೆ (leveli->ಅಂಕೆ_syms > 1)
        ಬಿಡುಗಡೆ(leveli->s.syms);
    ಆದರೆ (leveli->ಅಂಕೆ_actions > 1)
        ಬಿಡುಗಡೆ(leveli->a.actions);
}

ಸ್ಥಿರ ಶೂನ್ಯ
clear_interpret(ರಚನೆ xkb_sym_interpret *interp)
{
    ಆದರೆ (interp->ಅಂಕೆ_actions > 1)
        ಬಿಡುಗಡೆ(interp->a.actions);
}

ಶೂನ್ಯ
xkb_keymap_unref(ರಚನೆ xkb_keymap *keymap)
{
    ಪ್ರತಿಜ್ಞಾ(!keymap || keymap->refcnt > 0);
    ಆದರೆ (!keymap || --keymap->refcnt > 0)
        ಹಿಂತಿರುಗಿ;

    ಆದರೆ (keymap->ಕೀಲಿಗಳು) {
        ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ;
        xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap) {
            ಆದರೆ (ಕೀಲಿ->groups) {
                ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t i = 0; i < ಕೀಲಿ->ಅಂಕೆ_groups; i++) {
                    ಆದರೆ (ಕೀಲಿ->groups[i].ಮಟ್ಟಗಳು) {
                        ಗಾಗಿ (xkb_level_ಸೂಚ್ಯಂಕ_t j = 0;
                             j < Xkb_ಕೀಲಿ_ಅಂಕೆ_ಮಟ್ಟಗಳು(ಕೀಲಿ, i);
                             j++)
                            clear_level(&ಕೀಲಿ->groups[i].ಮಟ್ಟಗಳು[j]);
                        ಬಿಡುಗಡೆ(ಕೀಲಿ->groups[i].ಮಟ್ಟಗಳು);
                    }
                }
                ಬಿಡುಗಡೆ(ಕೀಲಿ->groups);
            }
            ಆದರೆ (!ಕೀಲಿ->overlays_inline && ಕೀಲಿ->overlays_ಕೀಲಿಗಳು) {
                ಬಿಡುಗಡೆ((ಶೂನ್ಯ *)ಕೀಲಿ->overlays_ಕೀಲಿಗಳು);
            }
        }
        ಬಿಡುಗಡೆ(keymap->ಕೀಲಿಗಳು);
    }
    ಆದರೆ (keymap->types) {
        ಗಾಗಿ (darray_ಗಾತ್ರ_t i = 0; i < keymap->ಅಂಕೆ_types; i++) {
            ಬಿಡುಗಡೆ(keymap->types[i].entries);
            ಬಿಡುಗಡೆ(keymap->types[i].level_ಹೆಸರುಗಳು);
        }
        ಬಿಡುಗಡೆ(keymap->types);
    }
    ಗಾಗಿ (darray_ಗಾತ್ರ_t k = 0; k < keymap->ಅಂಕೆ_sym_interprets; k++) {
        clear_interpret(&keymap->sym_interprets[k]);
    }
    ಬಿಡುಗಡೆ(keymap->sym_interprets);
    ಬಿಡುಗಡೆ(keymap->modmaps);
    ಬಿಡುಗಡೆ(keymap->ಕೀಲಿ_aliases);
    ಬಿಡುಗಡೆ(keymap->ಗುಂಪು_ಹೆಸರುಗಳು);
    ಬಿಡುಗಡೆ(keymap->keycodes_section_name);
    ಬಿಡುಗಡೆ(keymap->symbols_section_name);
    ಬಿಡುಗಡೆ(keymap->types_section_name);
    ಬಿಡುಗಡೆ(keymap->compat_section_name);
    xkb_context_unref(keymap->ctx);
    ಬಿಡುಗಡೆ(keymap);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_format_ops *
get_keymap_format_ops(ಎಣಿಕೆ xkb_keymap_format format)
{
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_format_ops *keymap_format_ops[] = {
        [XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1] = &ಪಠ್ಯ_v1_keymap_format_ops,
        [XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V2] = &ಪಠ್ಯ_v1_keymap_format_ops,
    };

    ಆದರೆ ((ಪೂರ್ಣಾಂಕ) format < 0 || (ಪೂರ್ಣಾಂಕ) format >= (ಪೂರ್ಣಾಂಕ) ARRAY_ಗಾತ್ರ(keymap_format_ops))
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ keymap_format_ops[(ಪೂರ್ಣಾಂಕ) format];
}

ರಚನೆ xkb_keymap *
xkb_keymap_new_from_rmlvo(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_rmlvo_builder *rmlvo,
                          ಎಣಿಕೆ xkb_keymap_format format,
                          ಎಣಿಕೆ xkb_keymap_compile_flags flags)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    ಆದರೆ (!ops || !ops->keymap_new_from_rmlvo) {
        log_err_func(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ರಚನೆ xkb_keymap *keymap = xkb_keymap_new(rmlvo->ctx, __func__, format,
                                               flags);
    ಆದರೆ (!keymap)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಆದರೆ (!ops->keymap_new_from_rmlvo(keymap, rmlvo)) {
        xkb_keymap_unref(keymap);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ keymap;
}

ರಚನೆ xkb_keymap *
xkb_keymap_new_from_names2(ರಚನೆ xkb_context *ctx,
                           ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ನಿಯಮ_ಹೆಸರುಗಳು *rmlvo_ಒಳ,
                           ಎಣಿಕೆ xkb_keymap_format format,
                           ಎಣಿಕೆ xkb_keymap_compile_flags flags)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    ಆದರೆ (!ops || !ops->keymap_new_from_ಹೆಸರುಗಳು) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ರಚನೆ xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    ಆದರೆ (!keymap)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ರಚನೆ xkb_ನಿಯಮ_ಹೆಸರುಗಳು rmlvo = {0};
    ಆದರೆ (rmlvo_ಒಳ)
        rmlvo = *rmlvo_ಒಳ;
    xkb_context_sanitize_ನಿಯಮ_ಹೆಸರುಗಳು(ctx, &rmlvo);

    ಆದರೆ (!ops->keymap_new_from_ಹೆಸರುಗಳು(keymap, &rmlvo)) {
        xkb_keymap_unref(keymap);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ keymap;
}


ರಚನೆ xkb_keymap *
xkb_keymap_new_from_ಹೆಸರುಗಳು(ರಚನೆ xkb_context *ctx,
                          ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ನಿಯಮ_ಹೆಸರುಗಳು *rmlvo_ಒಳ,
                          ಎಣಿಕೆ xkb_keymap_compile_flags flags)
{
    ಹಿಂತಿರುಗಿ xkb_keymap_new_from_names2(ctx, rmlvo_ಒಳ,
                                      XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V2, flags);
}

ರಚನೆ xkb_keymap *
xkb_keymap_new_from_string(ರಚನೆ xkb_context *ctx,
                           ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string,
                           ಎಣಿಕೆ xkb_keymap_format format,
                           ಎಣಿಕೆ xkb_keymap_compile_flags flags)
{
    ಹಿಂತಿರುಗಿ xkb_keymap_new_from_ಬಫರ್(ctx, string, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(string),
                                      format, flags);
}

ರಚನೆ xkb_keymap *
xkb_keymap_new_from_ಬಫರ್(ರಚನೆ xkb_context *ctx,
                           ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಫರ್, ಗಾತ್ರ_t ಉದ್ದಳತೆ,
                           ಎಣಿಕೆ xkb_keymap_format format,
                           ಎಣಿಕೆ xkb_keymap_compile_flags flags)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    ಆದರೆ (!ops || !ops->keymap_new_from_string) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಆದರೆ (!ಬಫರ್) {
        log_err_func1(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                      "no buffer specified\n");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ರಚನೆ xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    ಆದರೆ (!keymap)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    /* Allow a zero-terminated string as a buffer */
    ಆದರೆ (ಉದ್ದಳತೆ > 0 && ಬಫರ್[ಉದ್ದಳತೆ - 1] == '\0')
        ಉದ್ದಳತೆ--;

    ಆದರೆ (!ops->keymap_new_from_string(keymap, ಬಫರ್, ಉದ್ದಳತೆ)) {
        xkb_keymap_unref(keymap);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ keymap;
}

ರಚನೆ xkb_keymap *
xkb_keymap_new_from_file(ರಚನೆ xkb_context *ctx,
                         FILE *file,
                         ಎಣಿಕೆ xkb_keymap_format format,
                         ಎಣಿಕೆ xkb_keymap_compile_flags flags)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    ಆದರೆ (!ops || !ops->keymap_new_from_file) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಆದರೆ (!file) {
        log_err_func1(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                      "no file specified\n");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ರಚನೆ xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    ಆದರೆ (!keymap)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಆದರೆ (!ops->keymap_new_from_file(keymap, file)) {
        xkb_keymap_unref(keymap);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಹಿಂತಿರುಗಿ keymap;
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
ಸ್ಥಿರ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
check_keymap_serialize_abi(
    ರಚನೆ xkb_context * ಸೀಮಿತ ctx,
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸೀಮಿತ func,
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_config * ಸೀಮಿತ config,
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ * ಸೀಮಿತ ಫಲಿತಾಂಶ
)
{
    ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2;
    ಆದರೆ ((ಸ್ಥಿತಿ_2 = xkb_check_keymap_abi(config)) ||
        (ಸ್ಥಿತಿ_2 = xkb_check_keymap_abi(ಫಲಿತಾಂಶ))) {
        xkb_log_abi_error(ctx, func, ಸ್ಥಿತಿ_2);
    }
    ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
xkb_keymap_serialize(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap,
                     ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_config *config,
                     ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ *ಫಲಿತಾಂಶ)
{
    /* Check ABI compatibility */
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2 =
        check_keymap_serialize_abi(keymap->ctx, __func__, config, ಫಲಿತಾಂಶ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != XKB_SUCCESS)
        ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;

    ರಚನೆ xkb_keymap_serialize_config new_config = *config;

    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_keymap_serialize_flags invalid_flags = (
        new_config.flags &
        ~(ಎಣಿಕೆ xkb_keymap_serialize_flags)XKB_KEYMAP_SERIALIZE_FLAGS_ಮೌಲ್ಯಗಳು
    );
    ಆದರೆ (invalid_flags) {
        log_err_func(keymap->ctx,
                     XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS_,
                     "unrecognized serialization flags: %#x\n",
                     invalid_flags);
        ಹಿಂತಿರುಗಿ XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS;
    }

    ಆದರೆ (new_config.format == XKB_KEYMAP_USE_ORIGINAL_FORMAT)
        new_config.format = keymap->format;

    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_format_ops * ಸ್ಥಿರಾಂಕ ops =
        get_keymap_format_ops(new_config.format);
    ಆದರೆ (!ops || !ops->keymap_serialize) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", new_config.format);
        ಹಿಂತಿರುಗಿ XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT;
    }

    ಸ್ಥಿರಾಂಕ xkb_layout_ಮುಖವಾಡ_t all_layouts =
        (xkb_layout_ಮುಖವಾಡ_t)((UINT64_C(1) << keymap->ಅಂಕೆ_groups) - 1);
    ಆದರೆ (!new_config.layouts) {
        new_config.layouts = all_layouts;
    }

    ಆದರೆ (new_config.layouts & ~all_layouts) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ_2,
                     "unsupported layout mask: "
                     "expected subset of 0x%08"PRIx32", got: 0x%08"PRIx32"\n",
                     all_layouts, new_config.layouts);
        ಹಿಂತಿರುಗಿ XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ;
    }

    ಸ್ಥಿರಾಂಕ xkb_layout_ಸೂಚ್ಯಂಕ_t max_groups = format_max_groups(config->format);
    ಸ್ಥಿರಾಂಕ xkb_layout_ಸೂಚ್ಯಂಕ_t ಅಂಕೆ_groups =
        (xkb_layout_ಸೂಚ್ಯಂಕ_t)popcount32(new_config.layouts);

    ಆದರೆ (ಅಂಕೆ_groups > max_groups) {
        ಸ್ಥಿರಾಂಕ bool strict =
            (new_config.flags & XKB_KEYMAP_SERIALIZE_STRICT_ಬಹುಲಕ);
        xkb_log_with_ಸಂಕೇತ(
            keymap->ctx,
            (strict ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
            XKB_LOG_VERBOSITY_ಕನಿಷ್ಠತಮ,
            XKB_ERROR_LAYOUT_ಎಣಿಕೆ_ಎಲ್ಲೆ_EXCEEDED_2,
            "Cannot serialize %"PRIu32" groups in keymap format %d: "
            "maximum is %"PRIu32"%s\n",
            ಅಂಕೆ_groups, config->format, max_groups,
            (strict ? "" : "; discarding unsupported groups")
        );
        ಆದರೆ (strict) {
            ಹಿಂತಿರುಗಿ XKB_ERROR_LAYOUT_ಎಣಿಕೆ_ಎಲ್ಲೆ_EXCEEDED;
        } ಇಲ್ಲದಿದ್ದರೆ {
            new_config.layouts =
                keep_ಅತಿಕಡಿಮೆ_n_ಗಣ_ಬಿಟ್ಟುಗಳು(new_config.layouts, max_groups);
        }
    }

    log_dbg(keymap->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
            "Serializing group mask 0x%"PRIx32" (total: %"PRIu32")\n",
            new_config.layouts, ಅಂಕೆ_groups);

    ಆದರೆ (new_config.layouts != all_layouts) {
        /*
         * If some layouts are discarded, then interprets may not trigger
         * properly. Avoid this by requiring explicit values.
         */
        new_config.flags |= (XKB_KEYMAP_SERIALIZE_EXPLICIT_ಕೀಲಿ_ಮೌಲ್ಯಗಳು |
                             XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS);
    }

    ಹಿಂತಿರುಗಿ ops->keymap_serialize(keymap, &new_config, ಫಲಿತಾಂಶ);
}

ಅಕ್ಷರ *
xkb_keymap_get_as_string2(ರಚನೆ xkb_keymap *keymap,
                          ಎಣಿಕೆ xkb_keymap_format format,
                          ಎಣಿಕೆ xkb_keymap_serialize_flags flags)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_serialize_config config = {
        .ಗಾತ್ರ_2 = ಗಾತ್ರ(config),
        .flags = flags,
        .format = format,
        .layouts = 0,
    };

    ರಚನೆ xkb_keymap_serialize_ಫಲಿತಾಂಶ ಫಲಿತಾಂಶ = { .ಗಾತ್ರ_2 = ಗಾತ್ರ(ಫಲಿತಾಂಶ) };

    ಹಿಂತಿರುಗಿ (xkb_keymap_serialize(keymap, &config, &ಫಲಿತಾಂಶ) == XKB_SUCCESS)
        ? ಫಲಿತಾಂಶ.serialized
        : ಖಾಲಿ;
}

ಅಕ್ಷರ *
xkb_keymap_get_as_string(ರಚನೆ xkb_keymap *keymap,
                         ಎಣಿಕೆ xkb_keymap_format format)
{
    ಹಿಂತಿರುಗಿ xkb_keymap_get_as_string2(keymap, format,
                                     XKB_KEYMAP_SERIALIZE_NO_FLAGS);
}

/**
 * Returns the total number of modifiers active in the keymap.
 */
xkb_mod_ಸೂಚ್ಯಂಕ_t
xkb_keymap_ಅಂಕೆ_mods(ರಚನೆ xkb_keymap *keymap)
{
    ಹಿಂತಿರುಗಿ keymap->mods.ಅಂಕೆ_mods;
}

/**
 * Return the name for a given modifier.
 */
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_keymap_mod_get_name(ರಚನೆ xkb_keymap *keymap, xkb_mod_ಸೂಚ್ಯಂಕ_t idx)
{
    ಆದರೆ (idx >= keymap->mods.ಅಂಕೆ_mods)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ xkb_atom_ಪಠ್ಯ(keymap->ctx, keymap->mods.mods[idx].name);
}

/**
 * Returns the index for a named modifier.
 */
xkb_mod_ಸೂಚ್ಯಂಕ_t
xkb_keymap_mod_get_ಸೂಚ್ಯಂಕ(ರಚನೆ xkb_keymap *keymap, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
    ಸ್ಥಿರಾಂಕ xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    ಹಿಂತಿರುಗಿ (atom == XKB_ATOM_NONE)
        ? XKB_MOD_INVALID
        : Xkb_Mod_Name_ಗಮ್ಯ_ಸೂಚ್ಯಂಕ(&keymap->mods, atom, MOD_BOTH);
}

/**
 * Return the canonical mapping of a named modifier.
 */
xkb_mod_ಮುಖವಾಡ_t
xkb_keymap_mod_get_ಮುಖವಾಡ(ರಚನೆ xkb_keymap *keymap, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name)
{
    ಸ್ಥಿರಾಂಕ xkb_mod_ಸೂಚ್ಯಂಕ_t idx = xkb_keymap_mod_get_ಸೂಚ್ಯಂಕ(keymap, name);
    ಹಿಂತಿರುಗಿ (idx >= keymap->mods.ಅಂಕೆ_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the canonical mapping of a given modifier.
 */
xkb_mod_ಮುಖವಾಡ_t
xkb_keymap_mod_get_mask2(ರಚನೆ xkb_keymap *keymap, xkb_mod_ಸೂಚ್ಯಂಕ_t idx)
{
    ಹಿಂತಿರುಗಿ (idx >= keymap->mods.ಅಂಕೆ_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the total number of active groups in the keymap.
 */
xkb_layout_ಸೂಚ್ಯಂಕ_t
xkb_keymap_ಅಂಕೆ_layouts(ರಚನೆ xkb_keymap *keymap)
{
    ಹಿಂತಿರುಗಿ keymap->ಅಂಕೆ_groups;
}

/**
 * Returns the name for a given group.
 */
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_keymap_layout_get_name(ರಚನೆ xkb_keymap *keymap, xkb_layout_ಸೂಚ್ಯಂಕ_t idx)
{
    ಆದರೆ (idx >= keymap->ಅಂಕೆ_ಗುಂಪು_ಹೆಸರುಗಳು)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ xkb_atom_ಪಠ್ಯ(keymap->ctx, keymap->ಗುಂಪು_ಹೆಸರುಗಳು[idx]);
}

/**
 * Returns the index for a named layout.
 */
xkb_layout_ಸೂಚ್ಯಂಕ_t
xkb_keymap_layout_get_ಸೂಚ್ಯಂಕ(ರಚನೆ xkb_keymap *keymap, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_layout_ಸೂಚ್ಯಂಕ_t i;

    ಆದರೆ (atom == XKB_ATOM_NONE)
        ಹಿಂತಿರುಗಿ XKB_LAYOUT_INVALID;

    ಗಾಗಿ (i = 0; i < keymap->ಅಂಕೆ_ಗುಂಪು_ಹೆಸರುಗಳು; i++)
        ಆದರೆ (keymap->ಗುಂಪು_ಹೆಸರುಗಳು[i] == atom)
            ಹಿಂತಿರುಗಿ i;

    ಹಿಂತಿರುಗಿ XKB_LAYOUT_INVALID;
}

/**
 * Returns the number of layouts active for a particular key.
 */
xkb_layout_ಸೂಚ್ಯಂಕ_t
xkb_keymap_ಅಂಕೆ_layouts_for_ಕೀಲಿ(ರಚನೆ xkb_keymap *keymap, xkb_keycode_t kc)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ = Xkb_ಕೀಲಿ(keymap, kc);

    ಆದರೆ (!ಕೀಲಿ)
        ಹಿಂತಿರುಗಿ 0;

    ಹಿಂತಿರುಗಿ ಕೀಲಿ->ಅಂಕೆ_groups;
}

/**
 * Returns the number of levels active for a particular key and layout.
 */
xkb_level_ಸೂಚ್ಯಂಕ_t
xkb_keymap_ಅಂಕೆ_ಮಟ್ಟಗಳು_for_ಕೀಲಿ(ರಚನೆ xkb_keymap *keymap, xkb_keycode_t kc,
                              xkb_layout_ಸೂಚ್ಯಂಕ_t layout)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ = Xkb_ಕೀಲಿ(keymap, kc);

    ಆದರೆ (!ಕೀಲಿ)
        ಹಿಂತಿರುಗಿ 0;

    layout = xkb_keymap_ಕೀಲಿ_effective_layout(ಕೀಲಿ, layout);
    ಆದರೆ (layout == XKB_LAYOUT_INVALID)
        ಹಿಂತಿರುಗಿ 0;

    ಹಿಂತಿರುಗಿ Xkb_ಕೀಲಿ_ಅಂಕೆ_ಮಟ್ಟಗಳು(ಕೀಲಿ, layout);
}

/**
 * Return the total number of LEDs in the keymap.
 */
xkb_led_ಸೂಚ್ಯಂಕ_t
xkb_keymap_ಅಂಕೆ_leds(ರಚನೆ xkb_keymap *keymap)
{
    ಹಿಂತಿರುಗಿ keymap->ಅಂಕೆ_leds;
}

/**
 * Returns the name for a given LED.
 */
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_keymap_led_get_name(ರಚನೆ xkb_keymap *keymap, xkb_led_ಸೂಚ್ಯಂಕ_t idx)
{
    ಆದರೆ (idx >= keymap->ಅಂಕೆ_leds)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ xkb_atom_ಪಠ್ಯ(keymap->ctx, keymap->leds[idx].name);
}

/**
 * Returns the index for a named LED.
 */
xkb_led_ಸೂಚ್ಯಂಕ_t
xkb_keymap_led_get_ಸೂಚ್ಯಂಕ(ರಚನೆ xkb_keymap *keymap, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_led_ಸೂಚ್ಯಂಕ_t i;
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_led *led;

    ಆದರೆ (atom == XKB_ATOM_NONE)
        ಹಿಂತಿರುಗಿ XKB_LED_INVALID;

    xkb_leds_enumerate(i, led, keymap)
        ಆದರೆ (led->name == atom)
            ಹಿಂತಿರುಗಿ i;

    ಹಿಂತಿರುಗಿ XKB_LED_INVALID;
}

ಗಾತ್ರ_t
xkb_keymap_ಕೀಲಿ_get_mods_for_level(ರಚನೆ xkb_keymap *keymap,
                                  xkb_keycode_t kc,
                                  xkb_layout_ಸೂಚ್ಯಂಕ_t layout,
                                  xkb_level_ಸೂಚ್ಯಂಕ_t level,
                                  xkb_mod_ಮುಖವಾಡ_t *masks_ಹೊರಹೋಗು,
                                  ಗಾತ್ರ_t masks_ಗಾತ್ರ)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ = Xkb_ಕೀಲಿ(keymap, kc);
    ಆದರೆ (!ಕೀಲಿ)
        ಹಿಂತಿರುಗಿ 0;

    layout = xkb_keymap_ಕೀಲಿ_effective_layout(ಕೀಲಿ, layout);
    ಆದರೆ (layout == XKB_LAYOUT_INVALID)
        ಹಿಂತಿರುಗಿ 0;

    ಆದರೆ (level >= Xkb_ಕೀಲಿ_ಅಂಕೆ_ಮಟ್ಟಗಳು(ಕೀಲಿ, layout))
        ಹಿಂತಿರುಗಿ 0;

    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type *type = ಕೀಲಿ->groups[layout].type;

    ಗಾತ್ರ_t ಎಣಿಕೆ_2 = 0;

    /*
     * If the active set of modifiers doesn't match any explicit entry of
     * the key type, the resulting level is 0 (i.e. Level 1).
     * So, if we are asked to find the modifiers for level==0, we can offer
     * an ~infinite supply, which is not very workable.
     * What we do instead, is special case the empty set of modifiers for
     * this purpose. If the empty set isn't explicit mapped to a level, we
     * take it to map to Level 1.
     * This is almost always what we want. If applicable, given it priority
     * over other ways to generate the level.
     */
    ಆದರೆ (level == 0) {
        bool ಖಾಲಿಗಳು_mapped = false;
        ಗಾಗಿ (darray_ಗಾತ್ರ_t i = 0; i < type->ಅಂಕೆ_entries && ಎಣಿಕೆ_2 < masks_ಗಾತ್ರ; i++)
            ಆದರೆ (ನಮೂದು_is_ಸಕ್ರಿಯ(&type->entries[i]) &&
                type->entries[i].mods.ಮುಖವಾಡ == 0) {
                ಖಾಲಿಗಳು_mapped = true;
                ನಿಲ್ಲಿಸು;
            }
        ಆದರೆ (!ಖಾಲಿಗಳು_mapped && ಎಣಿಕೆ_2 < masks_ಗಾತ್ರ) {
            masks_ಹೊರಹೋಗು[ಎಣಿಕೆ_2++] = 0;
        }
    }

    /* Now search explicit mappings. */
    ಗಾಗಿ (darray_ಗಾತ್ರ_t i = 0; i < type->ಅಂಕೆ_entries && ಎಣಿಕೆ_2 < masks_ಗಾತ್ರ; i++) {
        ಆದರೆ (ನಮೂದು_is_ಸಕ್ರಿಯ(&type->entries[i]) &&
            type->entries[i].level == level) {
            masks_ಹೊರಹೋಗು[ಎಣಿಕೆ_2++] = type->entries[i].mods.ಮುಖವಾಡ;
        }
    }

    ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_2;
}

ರಚನೆ xkb_level *
xkb_keymap_ಕೀಲಿ_get_level(ರಚನೆ xkb_keymap *keymap, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                         xkb_layout_ಸೂಚ್ಯಂಕ_t layout, xkb_level_ಸೂಚ್ಯಂಕ_t level)
{
    layout = xkb_keymap_ಕೀಲಿ_effective_layout(ಕೀಲಿ, layout);
    ಆದರೆ (layout == XKB_LAYOUT_INVALID)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಆದರೆ (level >= Xkb_ಕೀಲಿ_ಅಂಕೆ_ಮಟ್ಟಗಳು(ಕೀಲಿ, layout))
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ &ಕೀಲಿ->groups[layout].ಮಟ್ಟಗಳು[level];
}

/**
 * As below, but takes an explicit layout/level rather than state.
 */
ಪೂರ್ಣಾಂಕ
xkb_keymap_ಕೀಲಿ_get_syms_by_level(ರಚನೆ xkb_keymap *keymap,
                                 xkb_keycode_t kc,
                                 xkb_layout_ಸೂಚ್ಯಂಕ_t layout,
                                 xkb_level_ಸೂಚ್ಯಂಕ_t level,
                                 ಸ್ಥಿರಾಂಕ xkb_keysym_t **syms_ಹೊರಹೋಗು)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ = Xkb_ಕೀಲಿ(keymap, kc);

    ಆದರೆ (!ಕೀಲಿ)
        ಹೋಗು err;

    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_level* ಸ್ಥಿರಾಂಕ leveli =
        xkb_keymap_ಕೀಲಿ_get_level(keymap, ಕೀಲಿ, layout, level);

    ಆದರೆ (!leveli)
        ಹೋಗು err;

    ಸ್ಥಿರಾಂಕ xkb_keysym_ಎಣಿಕೆ_t ಅಂಕೆ_syms = leveli->ಅಂಕೆ_syms;
    ಆದರೆ (ಅಂಕೆ_syms == 0)
        ಹೋಗು err;

    ಆದರೆ (ಅಂಕೆ_syms == 1)
        *syms_ಹೊರಹೋಗು = &leveli->s.sym;
    ಇಲ್ಲದಿದ್ದರೆ
        *syms_ಹೊರಹೋಗು = leveli->s.syms;

    ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) ಅಂಕೆ_syms;

err:
    *syms_ಹೊರಹೋಗು = ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ 0;
}

xkb_keycode_t
xkb_keymap_min_keycode(ರಚನೆ xkb_keymap *keymap)
{
    ಹಿಂತಿರುಗಿ keymap->min_ಕೀಲಿ_ಸಂಕೇತ;
}

xkb_keycode_t
xkb_keymap_max_keycode(ರಚನೆ xkb_keymap *keymap)
{
    ಹಿಂತಿರುಗಿ keymap->max_ಕೀಲಿ_ಸಂಕೇತ;
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
keymap_ಕೀಲಿ_iterator_config_check(
    ರಚನೆ xkb_context * ಸೀಮಿತ ctx,
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *func,
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config * ಸೀಮಿತ config
)
{
    /* Check ABI compatibility */
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ abi_error = xkb_check_keymap_abi(config);
    ಆದರೆ (abi_error) {
        xkb_log_abi_error(ctx, func, abi_error);
        ಹಿಂತಿರುಗಿ abi_error;
    }

    /* Sanitize input: flags */
    ಸ್ಥಿರಾಂಕ uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_KEYMAP_ಕೀಲಿ_ITERATOR_FLAGS_ಮೌಲ್ಯಗಳು);
    ಆದರೆ (invalid_flags) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_ಕೀಲಿ_ITERATOR_FLAGS_2,
                "%s: Unsupported keymap iterator flags: 0x%"PRIx32"\n",
                func, invalid_flags);
        ಹಿಂತಿರುಗಿ XKB_ERROR_UNSUPPORTED_ಕೀಲಿ_ITERATOR_FLAGS;
    }

    /* Sanitize input: start */
    ಆದರೆ (config->ಆರಂಭ > XKB_KEYCODE_MAX)
        ಹಿಂತಿರುಗಿ XKB_ERROR_INVALID_KEYCODE;

    ಹಿಂತಿರುಗಿ XKB_SUCCESS;
}

static_assert(XKB_KEYMAP_ಕೀಲಿ_ITERATOR_FLAGS_ಮೌಲ್ಯಗಳು <=
              (ಪೂರ್ಣಾಂಕ)XKB_ಸೂಚಕ_ಟ್ಯಾಗ್_ಮುಖವಾಡ,
              "Cannot tag cursor pointer with flags");

#ವ್ಯಾಖ್ಯಾನಿಸು iter_init_keymap(i, ptr, flags) {          \
    (i)->opaque0 = xkb_ಸೂಚಕ_ಟ್ಯಾಗ್((ptr), (flags));\
}
#ವ್ಯಾಖ್ಯಾನಿಸು iter_get_keymap(i) (                                  \
    (ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *)xkb_ಸೂಚಕ_untag((i)->opaque0)\
)
#ವ್ಯಾಖ್ಯಾನಿಸು iter_get_ಮುಂದಿನ(i) ((ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *)(i)->opaque1)
#ವ್ಯಾಖ್ಯಾನಿಸು iter_ಗಣ_ಮುಂದಿನ(i, ptr) {(i)->opaque1 = (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *)(ptr);}
#ವ್ಯಾಖ್ಯಾನಿಸು iter_get_descending_ಕ್ರಮ(i) (                  \
    xkb_ಸೂಚಕ_get_ಟ್ಯಾಗ್((i)->opaque0) &                 \
    (uintptr_t)XKB_KEYMAP_ಕೀಲಿ_ITERATOR_DESCENDING_ಕ್ರಮ \
)
#ವ್ಯಾಖ್ಯಾನಿಸು iter_get_include_unbound(i) (                  \
    xkb_ಸೂಚಕ_get_ಟ್ಯಾಗ್((i)->opaque0) &                \
    (uintptr_t)XKB_KEYMAP_ಕೀಲಿ_ITERATOR_INCLUDE_UNBOUND \
)

ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
xkb_keymap_ಕೀಲಿ_iterator_init(
    ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator * ಸೀಮಿತ iter,
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap * ಸೀಮಿತ keymap,
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config * ಸೀಮಿತ config
)
{
    /* Handle default configuration */
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config default_config = {
        .ಗಾತ್ರ_2 = ಗಾತ್ರ(default_config)
    };
    ಆದರೆ (!config)
        config = &default_config;

    /* Check input */
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2 =
        keymap_ಕೀಲಿ_iterator_config_check(keymap->ctx, __func__, config);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != XKB_SUCCESS) {
        ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
    }

    /*
     * Initialization
     */

    memset(iter, 0, ಗಾತ್ರ(*iter));

    /* Borrow keymap */
    iter_init_keymap(iter, keymap, config->flags);

    ಆದರೆ (keymap->ಅಂಕೆ_ಕೀಲಿಗಳು == 0) {
        iter_ಗಣ_ಮುಂದಿನ(iter, ಖಾಲಿ);
        ಹಿಂತಿರುಗಿ XKB_SUCCESS;
    }

    /* Borrow key */
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಮುಂದಿನ;
    ಆದರೆ (config->flags & XKB_KEYMAP_ಕೀಲಿ_ITERATOR_DESCENDING_ಕ್ರಮ) {
        ಆದರೆ (!config->ಆರಂಭ || config->ಆರಂಭ > keymap->max_ಕೀಲಿ_ಸಂಕೇತ) {
            ಮುಂದಿನ = &keymap->ಕೀಲಿಗಳು[keymap->ಅಂಕೆ_ಕೀಲಿಗಳು - 1];
        } ಇಲ್ಲದಿದ್ದರೆ {
            /*
             * May still fail *safely* if start < keymap->min_key_code:
             * there will be simply no key to iterate over.
             */
            ಮುಂದಿನ = xkb_keymap_get_ಮುಂದಿನ_defined_ಕೀಲಿ(keymap, false, config->ಆರಂಭ);
        }
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಆದರೆ (!config->ಆರಂಭ || config->ಆರಂಭ < keymap->min_ಕೀಲಿ_ಸಂಕೇತ) {
            ಮುಂದಿನ = (keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ)
                ? &keymap->ಕೀಲಿಗಳು[keymap->min_ಕೀಲಿ_ಸಂಕೇತ]
                : &keymap->ಕೀಲಿಗಳು[0];
        } ಇಲ್ಲದಿದ್ದರೆ {
            /*
             * May still fail *safely* if start > keymap->max_key_code:
             * there will be simply no key to iterate over.
             */
            ಮುಂದಿನ = xkb_keymap_get_ಮುಂದಿನ_defined_ಕೀಲಿ(keymap, true, config->ಆರಂಭ);
        }
    };
    iter_ಗಣ_ಮುಂದಿನ(iter, ಮುಂದಿನ);

    ಹಿಂತಿರುಗಿ XKB_SUCCESS;
}

xkb_keycode_t
xkb_keymap_ಕೀಲಿ_iterator_ಮುಂದಿನ(ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator *iter)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಮುಂದಿನ = iter_get_ಮುಂದಿನ(iter);
    ಆದರೆ (!ಮುಂದಿನ)
        ಹಿಂತಿರುಗಿ XKB_KEYCODE_INVALID;

    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap = iter_get_keymap(iter);
    ಪ್ರತಿಜ್ಞಾ(keymap->ಅಂಕೆ_ಕೀಲಿಗಳು);

    ಸ್ಥಿರಾಂಕ bool descending = iter_get_descending_ಕ್ರಮ(iter);
    ಸ್ಥಿರಾಂಕ bool skip_unbound = !iter_get_include_unbound(iter);
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೊನೆಯ = descending
        ? (keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ)
            ? &keymap->ಕೀಲಿಗಳು[keymap->min_ಕೀಲಿ_ಸಂಕೇತ]
            : &keymap->ಕೀಲಿಗಳು[0]
        : &keymap->ಕೀಲಿಗಳು[keymap->ಅಂಕೆ_ಕೀಲಿಗಳು - 1];
    ಸ್ಥಿರಾಂಕ ptrdiff_t ಹೆಜ್ಜೆ = descending ? -1 : 1;

    /* Skip undefined keys (no name) and optionally unbound keys */
    ಗಾಗಿ (;;) {
        ಸ್ಥಿರಾಂಕ bool iter_ಕೊನೆ = descending ? (ಮುಂದಿನ <= ಕೊನೆಯ) : (ಮುಂದಿನ >= ಕೊನೆಯ);
        ಸ್ಥಿರಾಂಕ bool skip = (
            ಮುಂದಿನ->name == XKB_ATOM_NONE ||
            (skip_unbound && (ಮುಂದಿನ->ಅಂಕೆ_groups == 0))
        );
        ಆದರೆ (!skip) {
            ಸ್ಥಿರಾಂಕ xkb_keycode_t ret = ಮುಂದಿನ->keycode;
            iter_ಗಣ_ಮುಂದಿನ(iter, iter_ಕೊನೆ ? ಖಾಲಿ : ಮುಂದಿನ + ಹೆಜ್ಜೆ);
            ಹಿಂತಿರುಗಿ ret;
        }
        ಆದರೆ (iter_ಕೊನೆ) {
            iter_ಗಣ_ಮುಂದಿನ(iter, ಖಾಲಿ);
            ಹಿಂತಿರುಗಿ XKB_KEYCODE_INVALID;
        }
        ಮುಂದಿನ += ಹೆಜ್ಜೆ;
    }
}

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು iter_init_keymap
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು iter_get_keymap
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು iter_ಗಣ_ಮುಂದಿನ
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು iter_get_ಮುಂದಿನ
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು iter_get_descending_ಕ್ರಮ
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು iter_get_include_unbound

ಶೂನ್ಯ
xkb_keymap_ಕೀಲಿ_for_each(ರಚನೆ xkb_keymap *keymap, xkb_keymap_ಕೀಲಿ_iter_t iter,
                        ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
    ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ;

    xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap)
        iter(keymap, ಕೀಲಿ->keycode, ದತ್ತಾಂಶ);
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_keymap_ಕೀಲಿ_get_name(ರಚನೆ xkb_keymap *keymap, xkb_keycode_t kc)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ = Xkb_ಕೀಲಿ(keymap, kc);

    ಆದರೆ (!ಕೀಲಿ)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ xkb_atom_ಪಠ್ಯ(keymap->ctx, ಕೀಲಿ->name);
}

xkb_keycode_t
xkb_keymap_ಕೀಲಿ_by_name(ರಚನೆ xkb_keymap *keymap, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
    ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ;
    xkb_atom_t atom;

    atom = xkb_atom_lookup(keymap->ctx, name);
    ಆದರೆ (atom) {
        ಗಾಗಿ (darray_ಗಾತ್ರ_t i = 0; i < keymap->ಅಂಕೆ_ಕೀಲಿ_aliases; i++)
            ಆದರೆ (keymap->ಕೀಲಿ_aliases[i].alias == atom)
                atom = keymap->ಕೀಲಿ_aliases[i].real;
    }
    ಆದರೆ (!atom)
        ಹಿಂತಿರುಗಿ XKB_KEYCODE_INVALID;

    xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap) {
        ಆದರೆ (ಕೀಲಿ->name == atom)
            ಹಿಂತಿರುಗಿ ಕೀಲಿ->keycode;
    }

    ಹಿಂತಿರುಗಿ XKB_KEYCODE_INVALID;
}

/**
 * Simple boolean specifying whether or not the key should repeat.
 */
ಪೂರ್ಣಾಂಕ
xkb_keymap_ಕೀಲಿ_repeats(ರಚನೆ xkb_keymap *keymap, xkb_keycode_t kc)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ = Xkb_ಕೀಲಿ(keymap, kc);

    ಆದರೆ (!ಕೀಲಿ)
        ಹಿಂತಿರುಗಿ 0;

    ಹಿಂತಿರುಗಿ ಕೀಲಿ->repeats;
}
