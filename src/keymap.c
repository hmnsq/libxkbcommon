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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abi-check.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "atom.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "context.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "features/enums.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap-priv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "messages-codes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "text.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-mem.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-numbers.h"

ਬਣਤਰ xkb_keymap *
xkb_keymap_ref(ਬਣਤਰ xkb_keymap *keymap)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(keymap->refcnt > 0);
    keymap->refcnt++;
    ਵਾਪਸ keymap;
}

ਖਾਲੀ
clear_level(ਬਣਤਰ xkb_level *leveli)
{
    ਜੇ (leveli->ਅੰਕ_syms > 1)
        ਮੁਕਤ(leveli->s.syms);
    ਜੇ (leveli->ਅੰਕ_actions > 1)
        ਮੁਕਤ(leveli->a.actions);
}

ਸਥਿਰ ਖਾਲੀ
clear_interpret(ਬਣਤਰ xkb_sym_interpret *interp)
{
    ਜੇ (interp->ਅੰਕ_actions > 1)
        ਮੁਕਤ(interp->a.actions);
}

ਖਾਲੀ
xkb_keymap_unref(ਬਣਤਰ xkb_keymap *keymap)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(!keymap || keymap->refcnt > 0);
    ਜੇ (!keymap || --keymap->refcnt > 0)
        ਵਾਪਸ;

    ਜੇ (keymap->ਕੁੰਜੀਆਂ) {
        ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ;
        xkb_ਕੁੰਜੀਆਂ_foreach(ਕੁੰਜੀ, keymap) {
            ਜੇ (ਕੁੰਜੀ->groups) {
                ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t i = 0; i < ਕੁੰਜੀ->ਅੰਕ_groups; i++) {
                    ਜੇ (ਕੁੰਜੀ->groups[i].ਪੱਧਰ) {
                        ਲਈ (xkb_level_ਸੂਚਕਾਂਕ_t j = 0;
                             j < Xkb_ਕੁੰਜੀ_ਅੰਕ_ਪੱਧਰ(ਕੁੰਜੀ, i);
                             j++)
                            clear_level(&ਕੁੰਜੀ->groups[i].ਪੱਧਰ[j]);
                        ਮੁਕਤ(ਕੁੰਜੀ->groups[i].ਪੱਧਰ);
                    }
                }
                ਮੁਕਤ(ਕੁੰਜੀ->groups);
            }
            ਜੇ (!ਕੁੰਜੀ->overlays_inline && ਕੁੰਜੀ->overlays_ਕੁੰਜੀਆਂ) {
                ਮੁਕਤ((ਖਾਲੀ *)ਕੁੰਜੀ->overlays_ਕੁੰਜੀਆਂ);
            }
        }
        ਮੁਕਤ(keymap->ਕੁੰਜੀਆਂ);
    }
    ਜੇ (keymap->types) {
        ਲਈ (darray_ਮਾਪ_t i = 0; i < keymap->ਅੰਕ_types; i++) {
            ਮੁਕਤ(keymap->types[i].entries);
            ਮੁਕਤ(keymap->types[i].level_ਨਾਂ);
        }
        ਮੁਕਤ(keymap->types);
    }
    ਲਈ (darray_ਮਾਪ_t k = 0; k < keymap->ਅੰਕ_sym_interprets; k++) {
        clear_interpret(&keymap->sym_interprets[k]);
    }
    ਮੁਕਤ(keymap->sym_interprets);
    ਮੁਕਤ(keymap->modmaps);
    ਮੁਕਤ(keymap->ਕੁੰਜੀ_aliases);
    ਮੁਕਤ(keymap->ਸਮੂਹ_ਨਾਂ);
    ਮੁਕਤ(keymap->keycodes_section_name);
    ਮੁਕਤ(keymap->symbols_section_name);
    ਮੁਕਤ(keymap->types_section_name);
    ਮੁਕਤ(keymap->compat_section_name);
    xkb_context_unref(keymap->ctx);
    ਮੁਕਤ(keymap);
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ xkb_keymap_format_ops *
get_keymap_format_ops(ਗਿਣਤੀ xkb_keymap_format format)
{
    ਸਥਿਰ ਅਚਲ ਬਣਤਰ xkb_keymap_format_ops *keymap_format_ops[] = {
        [XKB_KEYMAP_FORMAT_ਪਾਠ_V1] = &ਪਾਠ_v1_keymap_format_ops,
        [XKB_KEYMAP_FORMAT_ਪਾਠ_V2] = &ਪਾਠ_v1_keymap_format_ops,
    };

    ਜੇ ((ਪੂਰਨ) format < 0 || (ਪੂਰਨ) format >= (ਪੂਰਨ) ARRAY_ਮਾਪ(keymap_format_ops))
        ਵਾਪਸ ਨਿਰਾ;

    ਵਾਪਸ keymap_format_ops[(ਪੂਰਨ) format];
}

ਬਣਤਰ xkb_keymap *
xkb_keymap_new_from_rmlvo(ਅਚਲ ਬਣਤਰ xkb_rmlvo_builder *rmlvo,
                          ਗਿਣਤੀ xkb_keymap_format format,
                          ਗਿਣਤੀ xkb_keymap_compile_flags flags)
{
    ਅਚਲ ਬਣਤਰ xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    ਜੇ (!ops || !ops->keymap_new_from_rmlvo) {
        log_err_func(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਬਣਤਰ xkb_keymap *keymap = xkb_keymap_new(rmlvo->ctx, __func__, format,
                                               flags);
    ਜੇ (!keymap)
        ਵਾਪਸ ਨਿਰਾ;

    ਜੇ (!ops->keymap_new_from_rmlvo(keymap, rmlvo)) {
        xkb_keymap_unref(keymap);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਾਪਸ keymap;
}

ਬਣਤਰ xkb_keymap *
xkb_keymap_new_from_names2(ਬਣਤਰ xkb_context *ctx,
                           ਅਚਲ ਬਣਤਰ xkb_ਨਿਯਮ_ਨਾਂ *rmlvo_ਵਿੱਚ,
                           ਗਿਣਤੀ xkb_keymap_format format,
                           ਗਿਣਤੀ xkb_keymap_compile_flags flags)
{
    ਅਚਲ ਬਣਤਰ xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    ਜੇ (!ops || !ops->keymap_new_from_ਨਾਂ) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਬਣਤਰ xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    ਜੇ (!keymap)
        ਵਾਪਸ ਨਿਰਾ;

    ਬਣਤਰ xkb_ਨਿਯਮ_ਨਾਂ rmlvo = {0};
    ਜੇ (rmlvo_ਵਿੱਚ)
        rmlvo = *rmlvo_ਵਿੱਚ;
    xkb_context_sanitize_ਨਿਯਮ_ਨਾਂ(ctx, &rmlvo);

    ਜੇ (!ops->keymap_new_from_ਨਾਂ(keymap, &rmlvo)) {
        xkb_keymap_unref(keymap);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਾਪਸ keymap;
}


ਬਣਤਰ xkb_keymap *
xkb_keymap_new_from_ਨਾਂ(ਬਣਤਰ xkb_context *ctx,
                          ਅਚਲ ਬਣਤਰ xkb_ਨਿਯਮ_ਨਾਂ *rmlvo_ਵਿੱਚ,
                          ਗਿਣਤੀ xkb_keymap_compile_flags flags)
{
    ਵਾਪਸ xkb_keymap_new_from_names2(ctx, rmlvo_ਵਿੱਚ,
                                      XKB_KEYMAP_FORMAT_ਪਾਠ_V2, flags);
}

ਬਣਤਰ xkb_keymap *
xkb_keymap_new_from_string(ਬਣਤਰ xkb_context *ctx,
                           ਅਚਲ ਅੱਖਰ *string,
                           ਗਿਣਤੀ xkb_keymap_format format,
                           ਗਿਣਤੀ xkb_keymap_compile_flags flags)
{
    ਵਾਪਸ xkb_keymap_new_from_ਬਫਰ(ctx, string, ਲੜੀ_ਲੰਬਾਈ(string),
                                      format, flags);
}

ਬਣਤਰ xkb_keymap *
xkb_keymap_new_from_ਬਫਰ(ਬਣਤਰ xkb_context *ctx,
                           ਅਚਲ ਅੱਖਰ *ਬਫਰ, ਮਾਪ_t ਲੰਮਾਈ,
                           ਗਿਣਤੀ xkb_keymap_format format,
                           ਗਿਣਤੀ xkb_keymap_compile_flags flags)
{
    ਅਚਲ ਬਣਤਰ xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    ਜੇ (!ops || !ops->keymap_new_from_string) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਜੇ (!ਬਫਰ) {
        log_err_func1(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                      "no buffer specified\n");
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਬਣਤਰ xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    ਜੇ (!keymap)
        ਵਾਪਸ ਨਿਰਾ;

    /* Allow a zero-terminated string as a buffer */
    ਜੇ (ਲੰਮਾਈ > 0 && ਬਫਰ[ਲੰਮਾਈ - 1] == '\0')
        ਲੰਮਾਈ--;

    ਜੇ (!ops->keymap_new_from_string(keymap, ਬਫਰ, ਲੰਮਾਈ)) {
        xkb_keymap_unref(keymap);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਾਪਸ keymap;
}

ਬਣਤਰ xkb_keymap *
xkb_keymap_new_from_file(ਬਣਤਰ xkb_context *ctx,
                         FILE *file,
                         ਗਿਣਤੀ xkb_keymap_format format,
                         ਗਿਣਤੀ xkb_keymap_compile_flags flags)
{
    ਅਚਲ ਬਣਤਰ xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    ਜੇ (!ops || !ops->keymap_new_from_file) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਜੇ (!file) {
        log_err_func1(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                      "no file specified\n");
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਬਣਤਰ xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    ਜੇ (!keymap)
        ਵਾਪਸ ਨਿਰਾ;

    ਜੇ (!ops->keymap_new_from_file(keymap, file)) {
        xkb_keymap_unref(keymap);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਵਾਪਸ keymap;
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
ਸਥਿਰ ਗਿਣਤੀ xkb_ਸਥਿਤੀ
check_keymap_serialize_abi(
    ਬਣਤਰ xkb_context * ਸੀਮਿਤ ctx,
    ਅਚਲ ਅੱਖਰ * ਸੀਮਿਤ func,
    ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_config * ਸੀਮਿਤ config,
    ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ * ਸੀਮਿਤ ਨਤੀਜਾ
)
{
    ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ;
    ਜੇ ((ਸਥਿਤੀ = xkb_check_keymap_abi(config)) ||
        (ਸਥਿਤੀ = xkb_check_keymap_abi(ਨਤੀਜਾ))) {
        xkb_log_abi_error(ctx, func, ਸਥਿਤੀ);
    }
    ਵਾਪਸ ਸਥਿਤੀ;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

ਗਿਣਤੀ xkb_ਸਥਿਤੀ
xkb_keymap_serialize(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap,
                     ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_config *config,
                     ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ *ਨਤੀਜਾ)
{
    /* Check ABI compatibility */
    ਅਚਲ ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ =
        check_keymap_serialize_abi(keymap->ctx, __func__, config, ਨਤੀਜਾ);
    ਜੇ (ਸਥਿਤੀ != XKB_SUCCESS)
        ਵਾਪਸ ਸਥਿਤੀ;

    ਬਣਤਰ xkb_keymap_serialize_config new_config = *config;

    ਅਚਲ ਗਿਣਤੀ xkb_keymap_serialize_flags invalid_flags = (
        new_config.flags &
        ~(ਗਿਣਤੀ xkb_keymap_serialize_flags)XKB_KEYMAP_SERIALIZE_FLAGS_ਮੁੱਲਾਂ
    );
    ਜੇ (invalid_flags) {
        log_err_func(keymap->ctx,
                     XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS_,
                     "unrecognized serialization flags: %#x\n",
                     invalid_flags);
        ਵਾਪਸ XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS;
    }

    ਜੇ (new_config.format == XKB_KEYMAP_USE_ORIGINAL_FORMAT)
        new_config.format = keymap->format;

    ਅਚਲ ਬਣਤਰ xkb_keymap_format_ops * ਅਚਲ ops =
        get_keymap_format_ops(new_config.format);
    ਜੇ (!ops || !ops->keymap_serialize) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", new_config.format);
        ਵਾਪਸ XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT;
    }

    ਅਚਲ xkb_layout_ਨਕਾਬ_t all_layouts =
        (xkb_layout_ਨਕਾਬ_t)((UINT64_C(1) << keymap->ਅੰਕ_groups) - 1);
    ਜੇ (!new_config.layouts) {
        new_config.layouts = all_layouts;
    }

    ਜੇ (new_config.layouts & ~all_layouts) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ_2,
                     "unsupported layout mask: "
                     "expected subset of 0x%08"PRIx32", got: 0x%08"PRIx32"\n",
                     all_layouts, new_config.layouts);
        ਵਾਪਸ XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ;
    }

    ਅਚਲ xkb_layout_ਸੂਚਕਾਂਕ_t max_groups = format_max_groups(config->format);
    ਅਚਲ xkb_layout_ਸੂਚਕਾਂਕ_t ਅੰਕ_groups =
        (xkb_layout_ਸੂਚਕਾਂਕ_t)popcount32(new_config.layouts);

    ਜੇ (ਅੰਕ_groups > max_groups) {
        ਅਚਲ bool strict =
            (new_config.flags & XKB_KEYMAP_SERIALIZE_STRICT_ਬਹੁਲਕ);
        xkb_log_with_ਕੋਡ(
            keymap->ctx,
            (strict ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
            XKB_LOG_VERBOSITY_ਨਿਊਨਤਮ,
            XKB_ERROR_LAYOUT_ਗਿਣਤੀ_ਹੱਦਬੰਦੀ_EXCEEDED_2,
            "Cannot serialize %"PRIu32" groups in keymap format %d: "
            "maximum is %"PRIu32"%s\n",
            ਅੰਕ_groups, config->format, max_groups,
            (strict ? "" : "; discarding unsupported groups")
        );
        ਜੇ (strict) {
            ਵਾਪਸ XKB_ERROR_LAYOUT_ਗਿਣਤੀ_ਹੱਦਬੰਦੀ_EXCEEDED;
        } ਨਹੀਂਤਾਂ {
            new_config.layouts =
                keep_ਸਭ_ਤੋਂ_ਨੀਵਾਂ_n_ਸੈੱਟ_ਬਿੱਟ(new_config.layouts, max_groups);
        }
    }

    log_dbg(keymap->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
            "Serializing group mask 0x%"PRIx32" (total: %"PRIu32")\n",
            new_config.layouts, ਅੰਕ_groups);

    ਜੇ (new_config.layouts != all_layouts) {
        /*
         * If some layouts are discarded, then interprets may not trigger
         * properly. Avoid this by requiring explicit values.
         */
        new_config.flags |= (XKB_KEYMAP_SERIALIZE_EXPLICIT_ਕੁੰਜੀ_ਮੁੱਲਾਂ |
                             XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS);
    }

    ਵਾਪਸ ops->keymap_serialize(keymap, &new_config, ਨਤੀਜਾ);
}

ਅੱਖਰ *
xkb_keymap_get_as_string2(ਬਣਤਰ xkb_keymap *keymap,
                          ਗਿਣਤੀ xkb_keymap_format format,
                          ਗਿਣਤੀ xkb_keymap_serialize_flags flags)
{
    ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_config config = {
        .ਮਾਪ = ਆਕਾਰ(config),
        .flags = flags,
        .format = format,
        .layouts = 0,
    };

    ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ ਨਤੀਜਾ = { .ਮਾਪ = ਆਕਾਰ(ਨਤੀਜਾ) };

    ਵਾਪਸ (xkb_keymap_serialize(keymap, &config, &ਨਤੀਜਾ) == XKB_SUCCESS)
        ? ਨਤੀਜਾ.serialized
        : ਨਿਰਾ;
}

ਅੱਖਰ *
xkb_keymap_get_as_string(ਬਣਤਰ xkb_keymap *keymap,
                         ਗਿਣਤੀ xkb_keymap_format format)
{
    ਵਾਪਸ xkb_keymap_get_as_string2(keymap, format,
                                     XKB_KEYMAP_SERIALIZE_NO_FLAGS);
}

/**
 * Returns the total number of modifiers active in the keymap.
 */
xkb_mod_ਸੂਚਕਾਂਕ_t
xkb_keymap_ਅੰਕ_mods(ਬਣਤਰ xkb_keymap *keymap)
{
    ਵਾਪਸ keymap->mods.ਅੰਕ_mods;
}

/**
 * Return the name for a given modifier.
 */
ਅਚਲ ਅੱਖਰ *
xkb_keymap_mod_get_name(ਬਣਤਰ xkb_keymap *keymap, xkb_mod_ਸੂਚਕਾਂਕ_t idx)
{
    ਜੇ (idx >= keymap->mods.ਅੰਕ_mods)
        ਵਾਪਸ ਨਿਰਾ;

    ਵਾਪਸ xkb_atom_ਪਾਠ(keymap->ctx, keymap->mods.mods[idx].name);
}

/**
 * Returns the index for a named modifier.
 */
xkb_mod_ਸੂਚਕਾਂਕ_t
xkb_keymap_mod_get_ਸੂਚਕਾਂਕ(ਬਣਤਰ xkb_keymap *keymap, ਅਚਲ ਅੱਖਰ *name)
{
    ਅਚਲ xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    ਵਾਪਸ (atom == XKB_ATOM_NONE)
        ? XKB_MOD_INVALID
        : Xkb_Mod_Name_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ(&keymap->mods, atom, MOD_BOTH);
}

/**
 * Return the canonical mapping of a named modifier.
 */
xkb_mod_ਨਕਾਬ_t
xkb_keymap_mod_get_ਨਕਾਬ(ਬਣਤਰ xkb_keymap *keymap, ਅਚਲ ਅੱਖਰ* name)
{
    ਅਚਲ xkb_mod_ਸੂਚਕਾਂਕ_t idx = xkb_keymap_mod_get_ਸੂਚਕਾਂਕ(keymap, name);
    ਵਾਪਸ (idx >= keymap->mods.ਅੰਕ_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the canonical mapping of a given modifier.
 */
xkb_mod_ਨਕਾਬ_t
xkb_keymap_mod_get_mask2(ਬਣਤਰ xkb_keymap *keymap, xkb_mod_ਸੂਚਕਾਂਕ_t idx)
{
    ਵਾਪਸ (idx >= keymap->mods.ਅੰਕ_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the total number of active groups in the keymap.
 */
xkb_layout_ਸੂਚਕਾਂਕ_t
xkb_keymap_ਅੰਕ_layouts(ਬਣਤਰ xkb_keymap *keymap)
{
    ਵਾਪਸ keymap->ਅੰਕ_groups;
}

/**
 * Returns the name for a given group.
 */
ਅਚਲ ਅੱਖਰ *
xkb_keymap_layout_get_name(ਬਣਤਰ xkb_keymap *keymap, xkb_layout_ਸੂਚਕਾਂਕ_t idx)
{
    ਜੇ (idx >= keymap->ਅੰਕ_ਸਮੂਹ_ਨਾਂ)
        ਵਾਪਸ ਨਿਰਾ;

    ਵਾਪਸ xkb_atom_ਪਾਠ(keymap->ctx, keymap->ਸਮੂਹ_ਨਾਂ[idx]);
}

/**
 * Returns the index for a named layout.
 */
xkb_layout_ਸੂਚਕਾਂਕ_t
xkb_keymap_layout_get_ਸੂਚਕਾਂਕ(ਬਣਤਰ xkb_keymap *keymap, ਅਚਲ ਅੱਖਰ *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_layout_ਸੂਚਕਾਂਕ_t i;

    ਜੇ (atom == XKB_ATOM_NONE)
        ਵਾਪਸ XKB_LAYOUT_INVALID;

    ਲਈ (i = 0; i < keymap->ਅੰਕ_ਸਮੂਹ_ਨਾਂ; i++)
        ਜੇ (keymap->ਸਮੂਹ_ਨਾਂ[i] == atom)
            ਵਾਪਸ i;

    ਵਾਪਸ XKB_LAYOUT_INVALID;
}

/**
 * Returns the number of layouts active for a particular key.
 */
xkb_layout_ਸੂਚਕਾਂਕ_t
xkb_keymap_ਅੰਕ_layouts_for_ਕੁੰਜੀ(ਬਣਤਰ xkb_keymap *keymap, xkb_keycode_t kc)
{
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ = Xkb_ਕੁੰਜੀ(keymap, kc);

    ਜੇ (!ਕੁੰਜੀ)
        ਵਾਪਸ 0;

    ਵਾਪਸ ਕੁੰਜੀ->ਅੰਕ_groups;
}

/**
 * Returns the number of levels active for a particular key and layout.
 */
xkb_level_ਸੂਚਕਾਂਕ_t
xkb_keymap_ਅੰਕ_ਪੱਧਰ_for_ਕੁੰਜੀ(ਬਣਤਰ xkb_keymap *keymap, xkb_keycode_t kc,
                              xkb_layout_ਸੂਚਕਾਂਕ_t layout)
{
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ = Xkb_ਕੁੰਜੀ(keymap, kc);

    ਜੇ (!ਕੁੰਜੀ)
        ਵਾਪਸ 0;

    layout = xkb_keymap_ਕੁੰਜੀ_effective_layout(ਕੁੰਜੀ, layout);
    ਜੇ (layout == XKB_LAYOUT_INVALID)
        ਵਾਪਸ 0;

    ਵਾਪਸ Xkb_ਕੁੰਜੀ_ਅੰਕ_ਪੱਧਰ(ਕੁੰਜੀ, layout);
}

/**
 * Return the total number of LEDs in the keymap.
 */
xkb_led_ਸੂਚਕਾਂਕ_t
xkb_keymap_ਅੰਕ_leds(ਬਣਤਰ xkb_keymap *keymap)
{
    ਵਾਪਸ keymap->ਅੰਕ_leds;
}

/**
 * Returns the name for a given LED.
 */
ਅਚਲ ਅੱਖਰ *
xkb_keymap_led_get_name(ਬਣਤਰ xkb_keymap *keymap, xkb_led_ਸੂਚਕਾਂਕ_t idx)
{
    ਜੇ (idx >= keymap->ਅੰਕ_leds)
        ਵਾਪਸ ਨਿਰਾ;

    ਵਾਪਸ xkb_atom_ਪਾਠ(keymap->ctx, keymap->leds[idx].name);
}

/**
 * Returns the index for a named LED.
 */
xkb_led_ਸੂਚਕਾਂਕ_t
xkb_keymap_led_get_ਸੂਚਕਾਂਕ(ਬਣਤਰ xkb_keymap *keymap, ਅਚਲ ਅੱਖਰ *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_led_ਸੂਚਕਾਂਕ_t i;
    ਅਚਲ ਬਣਤਰ xkb_led *led;

    ਜੇ (atom == XKB_ATOM_NONE)
        ਵਾਪਸ XKB_LED_INVALID;

    xkb_leds_enumerate(i, led, keymap)
        ਜੇ (led->name == atom)
            ਵਾਪਸ i;

    ਵਾਪਸ XKB_LED_INVALID;
}

ਮਾਪ_t
xkb_keymap_ਕੁੰਜੀ_get_mods_for_level(ਬਣਤਰ xkb_keymap *keymap,
                                  xkb_keycode_t kc,
                                  xkb_layout_ਸੂਚਕਾਂਕ_t layout,
                                  xkb_level_ਸੂਚਕਾਂਕ_t level,
                                  xkb_mod_ਨਕਾਬ_t *masks_ਬਾਹਰਲਾ,
                                  ਮਾਪ_t masks_ਮਾਪ)
{
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ = Xkb_ਕੁੰਜੀ(keymap, kc);
    ਜੇ (!ਕੁੰਜੀ)
        ਵਾਪਸ 0;

    layout = xkb_keymap_ਕੁੰਜੀ_effective_layout(ਕੁੰਜੀ, layout);
    ਜੇ (layout == XKB_LAYOUT_INVALID)
        ਵਾਪਸ 0;

    ਜੇ (level >= Xkb_ਕੁੰਜੀ_ਅੰਕ_ਪੱਧਰ(ਕੁੰਜੀ, layout))
        ਵਾਪਸ 0;

    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type *type = ਕੁੰਜੀ->groups[layout].type;

    ਮਾਪ_t ਗਿਣਤੀ_2 = 0;

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
    ਜੇ (level == 0) {
        bool ਖਾਲੀ_ਥਾਵਾਂ_mapped = false;
        ਲਈ (darray_ਮਾਪ_t i = 0; i < type->ਅੰਕ_entries && ਗਿਣਤੀ_2 < masks_ਮਾਪ; i++)
            ਜੇ (ਇੰਦਰਾਜ_is_ਸਰਗਰਮ(&type->entries[i]) &&
                type->entries[i].mods.ਨਕਾਬ == 0) {
                ਖਾਲੀ_ਥਾਵਾਂ_mapped = true;
                ਰੋਕੋ;
            }
        ਜੇ (!ਖਾਲੀ_ਥਾਵਾਂ_mapped && ਗਿਣਤੀ_2 < masks_ਮਾਪ) {
            masks_ਬਾਹਰਲਾ[ਗਿਣਤੀ_2++] = 0;
        }
    }

    /* Now search explicit mappings. */
    ਲਈ (darray_ਮਾਪ_t i = 0; i < type->ਅੰਕ_entries && ਗਿਣਤੀ_2 < masks_ਮਾਪ; i++) {
        ਜੇ (ਇੰਦਰਾਜ_is_ਸਰਗਰਮ(&type->entries[i]) &&
            type->entries[i].level == level) {
            masks_ਬਾਹਰਲਾ[ਗਿਣਤੀ_2++] = type->entries[i].mods.ਨਕਾਬ;
        }
    }

    ਵਾਪਸ ਗਿਣਤੀ_2;
}

ਬਣਤਰ xkb_level *
xkb_keymap_ਕੁੰਜੀ_get_level(ਬਣਤਰ xkb_keymap *keymap, ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                         xkb_layout_ਸੂਚਕਾਂਕ_t layout, xkb_level_ਸੂਚਕਾਂਕ_t level)
{
    layout = xkb_keymap_ਕੁੰਜੀ_effective_layout(ਕੁੰਜੀ, layout);
    ਜੇ (layout == XKB_LAYOUT_INVALID)
        ਵਾਪਸ ਨਿਰਾ;

    ਜੇ (level >= Xkb_ਕੁੰਜੀ_ਅੰਕ_ਪੱਧਰ(ਕੁੰਜੀ, layout))
        ਵਾਪਸ ਨਿਰਾ;

    ਵਾਪਸ &ਕੁੰਜੀ->groups[layout].ਪੱਧਰ[level];
}

/**
 * As below, but takes an explicit layout/level rather than state.
 */
ਪੂਰਨ
xkb_keymap_ਕੁੰਜੀ_get_syms_by_level(ਬਣਤਰ xkb_keymap *keymap,
                                 xkb_keycode_t kc,
                                 xkb_layout_ਸੂਚਕਾਂਕ_t layout,
                                 xkb_level_ਸੂਚਕਾਂਕ_t level,
                                 ਅਚਲ xkb_keysym_t **syms_ਬਾਹਰਲਾ)
{
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ = Xkb_ਕੁੰਜੀ(keymap, kc);

    ਜੇ (!ਕੁੰਜੀ)
        ਜਾਓ err;

    ਅਚਲ ਬਣਤਰ xkb_level* ਅਚਲ leveli =
        xkb_keymap_ਕੁੰਜੀ_get_level(keymap, ਕੁੰਜੀ, layout, level);

    ਜੇ (!leveli)
        ਜਾਓ err;

    ਅਚਲ xkb_keysym_ਗਿਣਤੀ_t ਅੰਕ_syms = leveli->ਅੰਕ_syms;
    ਜੇ (ਅੰਕ_syms == 0)
        ਜਾਓ err;

    ਜੇ (ਅੰਕ_syms == 1)
        *syms_ਬਾਹਰਲਾ = &leveli->s.sym;
    ਨਹੀਂਤਾਂ
        *syms_ਬਾਹਰਲਾ = leveli->s.syms;

    ਵਾਪਸ (ਪੂਰਨ) ਅੰਕ_syms;

err:
    *syms_ਬਾਹਰਲਾ = ਨਿਰਾ;
    ਵਾਪਸ 0;
}

xkb_keycode_t
xkb_keymap_min_keycode(ਬਣਤਰ xkb_keymap *keymap)
{
    ਵਾਪਸ keymap->min_ਕੁੰਜੀ_ਕੋਡ;
}

xkb_keycode_t
xkb_keymap_max_keycode(ਬਣਤਰ xkb_keymap *keymap)
{
    ਵਾਪਸ keymap->max_ਕੁੰਜੀ_ਕੋਡ;
}

ਸਥਿਰ ਗਿਣਤੀ xkb_ਸਥਿਤੀ
keymap_ਕੁੰਜੀ_iterator_config_check(
    ਬਣਤਰ xkb_context * ਸੀਮਿਤ ctx,
    ਅਚਲ ਅੱਖਰ *func,
    ਅਚਲ ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config * ਸੀਮਿਤ config
)
{
    /* Check ABI compatibility */
    ਅਚਲ ਗਿਣਤੀ xkb_ਸਥਿਤੀ abi_error = xkb_check_keymap_abi(config);
    ਜੇ (abi_error) {
        xkb_log_abi_error(ctx, func, abi_error);
        ਵਾਪਸ abi_error;
    }

    /* Sanitize input: flags */
    ਅਚਲ uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_KEYMAP_ਕੁੰਜੀ_ITERATOR_FLAGS_ਮੁੱਲਾਂ);
    ਜੇ (invalid_flags) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_ਕੁੰਜੀ_ITERATOR_FLAGS_2,
                "%s: Unsupported keymap iterator flags: 0x%"PRIx32"\n",
                func, invalid_flags);
        ਵਾਪਸ XKB_ERROR_UNSUPPORTED_ਕੁੰਜੀ_ITERATOR_FLAGS;
    }

    /* Sanitize input: start */
    ਜੇ (config->ਸ਼ੁਰੂ > XKB_KEYCODE_MAX)
        ਵਾਪਸ XKB_ERROR_INVALID_KEYCODE;

    ਵਾਪਸ XKB_SUCCESS;
}

static_assert(XKB_KEYMAP_ਕੁੰਜੀ_ITERATOR_FLAGS_ਮੁੱਲਾਂ <=
              (ਪੂਰਨ)XKB_ਸੂਚਕ_ਟੈਗ_ਨਕਾਬ,
              "Cannot tag cursor pointer with flags");

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ iter_init_keymap(i, ptr, flags) {          \
    (i)->opaque0 = xkb_ਸੂਚਕ_ਟੈਗ((ptr), (flags));\
}
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ iter_get_keymap(i) (                                  \
    (ਅਚਲ ਬਣਤਰ xkb_keymap *)xkb_ਸੂਚਕ_untag((i)->opaque0)\
)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ iter_get_ਅਗਲਾ(i) ((ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *)(i)->opaque1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ iter_ਸੈੱਟ_ਅਗਲਾ(i, ptr) {(i)->opaque1 = (ਅਚਲ ਖਾਲੀ *)(ptr);}
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ iter_get_descending_ਕ੍ਰਮ(i) (                  \
    xkb_ਸੂਚਕ_get_ਟੈਗ((i)->opaque0) &                 \
    (uintptr_t)XKB_KEYMAP_ਕੁੰਜੀ_ITERATOR_DESCENDING_ਕ੍ਰਮ \
)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ iter_get_include_unbound(i) (                  \
    xkb_ਸੂਚਕ_get_ਟੈਗ((i)->opaque0) &                \
    (uintptr_t)XKB_KEYMAP_ਕੁੰਜੀ_ITERATOR_INCLUDE_UNBOUND \
)

ਗਿਣਤੀ xkb_ਸਥਿਤੀ
xkb_keymap_ਕੁੰਜੀ_iterator_init(
    ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator * ਸੀਮਿਤ iter,
    ਅਚਲ ਬਣਤਰ xkb_keymap * ਸੀਮਿਤ keymap,
    ਅਚਲ ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config * ਸੀਮਿਤ config
)
{
    /* Handle default configuration */
    ਸਥਿਰ ਅਚਲ ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config default_config = {
        .ਮਾਪ = ਆਕਾਰ(default_config)
    };
    ਜੇ (!config)
        config = &default_config;

    /* Check input */
    ਅਚਲ ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ =
        keymap_ਕੁੰਜੀ_iterator_config_check(keymap->ctx, __func__, config);
    ਜੇ (ਸਥਿਤੀ != XKB_SUCCESS) {
        ਵਾਪਸ ਸਥਿਤੀ;
    }

    /*
     * Initialization
     */

    memset(iter, 0, ਆਕਾਰ(*iter));

    /* Borrow keymap */
    iter_init_keymap(iter, keymap, config->flags);

    ਜੇ (keymap->ਅੰਕ_ਕੁੰਜੀਆਂ == 0) {
        iter_ਸੈੱਟ_ਅਗਲਾ(iter, ਨਿਰਾ);
        ਵਾਪਸ XKB_SUCCESS;
    }

    /* Borrow key */
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਅਗਲਾ;
    ਜੇ (config->flags & XKB_KEYMAP_ਕੁੰਜੀ_ITERATOR_DESCENDING_ਕ੍ਰਮ) {
        ਜੇ (!config->ਸ਼ੁਰੂ || config->ਸ਼ੁਰੂ > keymap->max_ਕੁੰਜੀ_ਕੋਡ) {
            ਅਗਲਾ = &keymap->ਕੁੰਜੀਆਂ[keymap->ਅੰਕ_ਕੁੰਜੀਆਂ - 1];
        } ਨਹੀਂਤਾਂ {
            /*
             * May still fail *safely* if start < keymap->min_key_code:
             * there will be simply no key to iterate over.
             */
            ਅਗਲਾ = xkb_keymap_get_ਅਗਲਾ_defined_ਕੁੰਜੀ(keymap, false, config->ਸ਼ੁਰੂ);
        }
    } ਨਹੀਂਤਾਂ {
        ਜੇ (!config->ਸ਼ੁਰੂ || config->ਸ਼ੁਰੂ < keymap->min_ਕੁੰਜੀ_ਕੋਡ) {
            ਅਗਲਾ = (keymap->ਅੰਕ_ਕੁੰਜੀਆਂ_ਨੀਵਾਂ)
                ? &keymap->ਕੁੰਜੀਆਂ[keymap->min_ਕੁੰਜੀ_ਕੋਡ]
                : &keymap->ਕੁੰਜੀਆਂ[0];
        } ਨਹੀਂਤਾਂ {
            /*
             * May still fail *safely* if start > keymap->max_key_code:
             * there will be simply no key to iterate over.
             */
            ਅਗਲਾ = xkb_keymap_get_ਅਗਲਾ_defined_ਕੁੰਜੀ(keymap, true, config->ਸ਼ੁਰੂ);
        }
    };
    iter_ਸੈੱਟ_ਅਗਲਾ(iter, ਅਗਲਾ);

    ਵਾਪਸ XKB_SUCCESS;
}

xkb_keycode_t
xkb_keymap_ਕੁੰਜੀ_iterator_ਅਗਲਾ(ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator *iter)
{
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਅਗਲਾ = iter_get_ਅਗਲਾ(iter);
    ਜੇ (!ਅਗਲਾ)
        ਵਾਪਸ XKB_KEYCODE_INVALID;

    ਅਚਲ ਬਣਤਰ xkb_keymap *keymap = iter_get_keymap(iter);
    ਪ੍ਰਤਿਜ੍ਞਾ(keymap->ਅੰਕ_ਕੁੰਜੀਆਂ);

    ਅਚਲ bool descending = iter_get_descending_ਕ੍ਰਮ(iter);
    ਅਚਲ bool skip_unbound = !iter_get_include_unbound(iter);
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਆਖਰੀ = descending
        ? (keymap->ਅੰਕ_ਕੁੰਜੀਆਂ_ਨੀਵਾਂ)
            ? &keymap->ਕੁੰਜੀਆਂ[keymap->min_ਕੁੰਜੀ_ਕੋਡ]
            : &keymap->ਕੁੰਜੀਆਂ[0]
        : &keymap->ਕੁੰਜੀਆਂ[keymap->ਅੰਕ_ਕੁੰਜੀਆਂ - 1];
    ਅਚਲ ptrdiff_t ਕਦਮ = descending ? -1 : 1;

    /* Skip undefined keys (no name) and optionally unbound keys */
    ਲਈ (;;) {
        ਅਚਲ bool iter_ਸਿਰਾ = descending ? (ਅਗਲਾ <= ਆਖਰੀ) : (ਅਗਲਾ >= ਆਖਰੀ);
        ਅਚਲ bool skip = (
            ਅਗਲਾ->name == XKB_ATOM_NONE ||
            (skip_unbound && (ਅਗਲਾ->ਅੰਕ_groups == 0))
        );
        ਜੇ (!skip) {
            ਅਚਲ xkb_keycode_t ret = ਅਗਲਾ->keycode;
            iter_ਸੈੱਟ_ਅਗਲਾ(iter, iter_ਸਿਰਾ ? ਨਿਰਾ : ਅਗਲਾ + ਕਦਮ);
            ਵਾਪਸ ret;
        }
        ਜੇ (iter_ਸਿਰਾ) {
            iter_ਸੈੱਟ_ਅਗਲਾ(iter, ਨਿਰਾ);
            ਵਾਪਸ XKB_KEYCODE_INVALID;
        }
        ਅਗਲਾ += ਕਦਮ;
    }
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ iter_init_keymap
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ iter_get_keymap
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ iter_ਸੈੱਟ_ਅਗਲਾ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ iter_get_ਅਗਲਾ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ iter_get_descending_ਕ੍ਰਮ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ iter_get_include_unbound

ਖਾਲੀ
xkb_keymap_ਕੁੰਜੀ_for_each(ਬਣਤਰ xkb_keymap *keymap, xkb_keymap_ਕੁੰਜੀ_iter_t iter,
                        ਖਾਲੀ *ਅੰਕੜੇ)
{
    ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ;

    xkb_ਕੁੰਜੀਆਂ_foreach(ਕੁੰਜੀ, keymap)
        iter(keymap, ਕੁੰਜੀ->keycode, ਅੰਕੜੇ);
}

ਅਚਲ ਅੱਖਰ *
xkb_keymap_ਕੁੰਜੀ_get_name(ਬਣਤਰ xkb_keymap *keymap, xkb_keycode_t kc)
{
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ = Xkb_ਕੁੰਜੀ(keymap, kc);

    ਜੇ (!ਕੁੰਜੀ)
        ਵਾਪਸ ਨਿਰਾ;

    ਵਾਪਸ xkb_atom_ਪਾਠ(keymap->ctx, ਕੁੰਜੀ->name);
}

xkb_keycode_t
xkb_keymap_ਕੁੰਜੀ_by_name(ਬਣਤਰ xkb_keymap *keymap, ਅਚਲ ਅੱਖਰ *name)
{
    ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ;
    xkb_atom_t atom;

    atom = xkb_atom_lookup(keymap->ctx, name);
    ਜੇ (atom) {
        ਲਈ (darray_ਮਾਪ_t i = 0; i < keymap->ਅੰਕ_ਕੁੰਜੀ_aliases; i++)
            ਜੇ (keymap->ਕੁੰਜੀ_aliases[i].alias == atom)
                atom = keymap->ਕੁੰਜੀ_aliases[i].real;
    }
    ਜੇ (!atom)
        ਵਾਪਸ XKB_KEYCODE_INVALID;

    xkb_ਕੁੰਜੀਆਂ_foreach(ਕੁੰਜੀ, keymap) {
        ਜੇ (ਕੁੰਜੀ->name == atom)
            ਵਾਪਸ ਕੁੰਜੀ->keycode;
    }

    ਵਾਪਸ XKB_KEYCODE_INVALID;
}

/**
 * Simple boolean specifying whether or not the key should repeat.
 */
ਪੂਰਨ
xkb_keymap_ਕੁੰਜੀ_repeats(ਬਣਤਰ xkb_keymap *keymap, xkb_keycode_t kc)
{
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ = Xkb_ਕੁੰਜੀ(keymap, kc);

    ਜੇ (!ਕੁੰਜੀ)
        ਵਾਪਸ 0;

    ਵਾਪਸ ਕੁੰਜੀ->repeats;
}
