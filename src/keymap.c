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

ಸಂರಚನಾ xkb_keymap *
xkb_keymap_ref(ಸಂರಚನಾ xkb_keymap *keymap)
{
    ಪ್ರತಿಜ್ಞಾ(keymap->refcnt > 0);
    keymap->refcnt++;
    ಪ್ರತ್ಯಾಗಮ keymap;
}

ಶೂನ್ಯ
clear_level(ಸಂರಚನಾ xkb_level *leveli)
{
    ಯದಿ (leveli->ಅಂಕೆ_syms > 1)
        ಮುಕ್ತ(leveli->s.syms);
    ಯದಿ (leveli->ಅಂಕೆ_actions > 1)
        ಮುಕ್ತ(leveli->a.actions);
}

ಸ್ಥಿರ ಶೂನ್ಯ
clear_interpret(ಸಂರಚನಾ xkb_sym_interpret *interp)
{
    ಯದಿ (interp->ಅಂಕೆ_actions > 1)
        ಮುಕ್ತ(interp->a.actions);
}

ಶೂನ್ಯ
xkb_keymap_unref(ಸಂರಚನಾ xkb_keymap *keymap)
{
    ಪ್ರತಿಜ್ಞಾ(!keymap || keymap->refcnt > 0);
    ಯದಿ (!keymap || --keymap->refcnt > 0)
        ಪ್ರತ್ಯಾಗಮ;

    ಯದಿ (keymap->ಕೀಲಿಗಳು) {
        ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ;
        xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap) {
            ಯದಿ (ಕೀಲಿ->groups) {
                ಪ್ರತಿ (xkb_layout_index_t i = 0; i < ಕೀಲಿ->ಅಂಕೆ_groups; i++) {
                    ಯದಿ (ಕೀಲಿ->groups[i].levels) {
                        ಪ್ರತಿ (xkb_level_index_t j = 0;
                             j < Xkb_ಕೀಲಿ_ಅಂಕೆ_Levels(ಕೀಲಿ, i);
                             j++)
                            clear_level(&ಕೀಲಿ->groups[i].levels[j]);
                        ಮುಕ್ತ(ಕೀಲಿ->groups[i].levels);
                    }
                }
                ಮುಕ್ತ(ಕೀಲಿ->groups);
            }
            ಯದಿ (!ಕೀಲಿ->overlays_inline && ಕೀಲಿ->overlays_ಕೀಲಿಗಳು) {
                ಮುಕ್ತ((ಶೂನ್ಯ *)ಕೀಲಿ->overlays_ಕೀಲಿಗಳು);
            }
        }
        ಮುಕ್ತ(keymap->ಕೀಲಿಗಳು);
    }
    ಯದಿ (keymap->types) {
        ಪ್ರತಿ (darray_size_t i = 0; i < keymap->ಅಂಕೆ_types; i++) {
            ಮುಕ್ತ(keymap->types[i].entries);
            ಮುಕ್ತ(keymap->types[i].level_names);
        }
        ಮುಕ್ತ(keymap->types);
    }
    ಪ್ರತಿ (darray_size_t k = 0; k < keymap->ಅಂಕೆ_sym_interprets; k++) {
        clear_interpret(&keymap->sym_interprets[k]);
    }
    ಮುಕ್ತ(keymap->sym_interprets);
    ಮುಕ್ತ(keymap->modmaps);
    ಮುಕ್ತ(keymap->ಕೀಲಿ_aliases);
    ಮುಕ್ತ(keymap->group_names);
    ಮುಕ್ತ(keymap->keycodes_section_name);
    ಮುಕ್ತ(keymap->symbols_section_name);
    ಮುಕ್ತ(keymap->types_section_name);
    ಮುಕ್ತ(keymap->compat_section_name);
    xkb_context_unref(keymap->ctx);
    ಮುಕ್ತ(keymap);
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_format_ops *
get_keymap_format_ops(ಗಣನಾ xkb_keymap_format format)
{
    ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_format_ops *keymap_format_ops[] = {
        [XKB_KEYMAP_FORMAT_TEXT_V1] = &text_v1_keymap_format_ops,
        [XKB_KEYMAP_FORMAT_TEXT_V2] = &text_v1_keymap_format_ops,
    };

    ಯದಿ ((ಪೂರ್ಣಾಙ್ಕ) format < 0 || (ಪೂರ್ಣಾಙ್ಕ) format >= (ಪೂರ್ಣಾಙ್ಕ) ARRAY_SIZE(keymap_format_ops))
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ keymap_format_ops[(ಪೂರ್ಣಾಙ್ಕ) format];
}

ಸಂರಚನಾ xkb_keymap *
xkb_keymap_new_from_rmlvo(ಧ್ರುವ ಸಂರಚನಾ xkb_rmlvo_builder *rmlvo,
                          ಗಣನಾ xkb_keymap_format format,
                          ಗಣನಾ xkb_keymap_compile_flags flags)
{
    ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    ಯದಿ (!ops || !ops->keymap_new_from_rmlvo) {
        log_err_func(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಸಂರಚನಾ xkb_keymap *keymap = xkb_keymap_new(rmlvo->ctx, __func__, format,
                                               flags);
    ಯದಿ (!keymap)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಯದಿ (!ops->keymap_new_from_rmlvo(keymap, rmlvo)) {
        xkb_keymap_unref(keymap);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಪ್ರತ್ಯಾಗಮ keymap;
}

ಸಂರಚನಾ xkb_keymap *
xkb_keymap_new_from_names2(ಸಂರಚನಾ xkb_context *ctx,
                           ಧ್ರುವ ಸಂರಚನಾ xkb_rule_names *rmlvo_in,
                           ಗಣನಾ xkb_keymap_format format,
                           ಗಣನಾ xkb_keymap_compile_flags flags)
{
    ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    ಯದಿ (!ops || !ops->keymap_new_from_names) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಸಂರಚನಾ xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    ಯದಿ (!keymap)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಸಂರಚನಾ xkb_rule_names rmlvo = {0};
    ಯದಿ (rmlvo_in)
        rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    ಯದಿ (!ops->keymap_new_from_names(keymap, &rmlvo)) {
        xkb_keymap_unref(keymap);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಪ್ರತ್ಯಾಗಮ keymap;
}


ಸಂರಚನಾ xkb_keymap *
xkb_keymap_new_from_names(ಸಂರಚನಾ xkb_context *ctx,
                          ಧ್ರುವ ಸಂರಚನಾ xkb_rule_names *rmlvo_in,
                          ಗಣನಾ xkb_keymap_compile_flags flags)
{
    ಪ್ರತ್ಯಾಗಮ xkb_keymap_new_from_names2(ctx, rmlvo_in,
                                      XKB_KEYMAP_FORMAT_TEXT_V2, flags);
}

ಸಂರಚನಾ xkb_keymap *
xkb_keymap_new_from_string(ಸಂರಚನಾ xkb_context *ctx,
                           ಧ್ರುವ ವರ್ಣ *string,
                           ಗಣನಾ xkb_keymap_format format,
                           ಗಣನಾ xkb_keymap_compile_flags flags)
{
    ಪ್ರತ್ಯಾಗಮ xkb_keymap_new_from_buffer(ctx, string, strlen(string),
                                      format, flags);
}

ಸಂರಚನಾ xkb_keymap *
xkb_keymap_new_from_buffer(ಸಂರಚನಾ xkb_context *ctx,
                           ಧ್ರುವ ವರ್ಣ *buffer, size_t length,
                           ಗಣನಾ xkb_keymap_format format,
                           ಗಣನಾ xkb_keymap_compile_flags flags)
{
    ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    ಯದಿ (!ops || !ops->keymap_new_from_string) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಯದಿ (!buffer) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no buffer specified\n");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಸಂರಚನಾ xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    ಯದಿ (!keymap)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    /* Allow a zero-terminated string as a buffer */
    ಯದಿ (length > 0 && buffer[length - 1] == '\0')
        length--;

    ಯದಿ (!ops->keymap_new_from_string(keymap, buffer, length)) {
        xkb_keymap_unref(keymap);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಪ್ರತ್ಯಾಗಮ keymap;
}

ಸಂರಚನಾ xkb_keymap *
xkb_keymap_new_from_file(ಸಂರಚನಾ xkb_context *ctx,
                         FILE *file,
                         ಗಣನಾ xkb_keymap_format format,
                         ಗಣನಾ xkb_keymap_compile_flags flags)
{
    ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    ಯದಿ (!ops || !ops->keymap_new_from_file) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಯದಿ (!file) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no file specified\n");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಸಂರಚನಾ xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    ಯದಿ (!keymap)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಯದಿ (!ops->keymap_new_from_file(keymap, file)) {
        xkb_keymap_unref(keymap);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಪ್ರತ್ಯಾಗಮ keymap;
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
ಸ್ಥಿರ ಗಣನಾ xkb_status
check_keymap_serialize_abi(
    ಸಂರಚನಾ xkb_context * restrict ctx,
    ಧ್ರುವ ವರ್ಣ * restrict func,
    ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_serialize_config * restrict config,
    ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_serialize_result * restrict result
)
{
    ಗಣನಾ xkb_status status;
    ಯದಿ ((status = xkb_check_keymap_abi(config)) ||
        (status = xkb_check_keymap_abi(result))) {
        xkb_log_abi_error(ctx, func, status);
    }
    ಪ್ರತ್ಯಾಗಮ status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

ಗಣನಾ xkb_status
xkb_keymap_serialize(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap,
                     ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_serialize_config *config,
                     ಸಂರಚನಾ xkb_keymap_serialize_result *result)
{
    /* Check ABI compatibility */
    ಧ್ರುವ ಗಣನಾ xkb_status status =
        check_keymap_serialize_abi(keymap->ctx, __func__, config, result);
    ಯದಿ (status != XKB_SUCCESS)
        ಪ್ರತ್ಯಾಗಮ status;

    ಸಂರಚನಾ xkb_keymap_serialize_config new_config = *config;

    ಧ್ರುವ ಗಣನಾ xkb_keymap_serialize_flags invalid_flags = (
        new_config.flags &
        ~(ಗಣನಾ xkb_keymap_serialize_flags)XKB_KEYMAP_SERIALIZE_FLAGS_ಮೌಲ್ಯಗಳು
    );
    ಯದಿ (invalid_flags) {
        log_err_func(keymap->ctx,
                     XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS_,
                     "unrecognized serialization flags: %#x\n",
                     invalid_flags);
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS;
    }

    ಯದಿ (new_config.format == XKB_KEYMAP_USE_ORIGINAL_FORMAT)
        new_config.format = keymap->format;

    ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_format_ops * ಧ್ರುವ ops =
        get_keymap_format_ops(new_config.format);
    ಯದಿ (!ops || !ops->keymap_serialize) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", new_config.format);
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT;
    }

    ಧ್ರುವ xkb_layout_mask_t all_layouts =
        (xkb_layout_mask_t)((UINT64_C(1) << keymap->ಅಂಕೆ_groups) - 1);
    ಯದಿ (!new_config.layouts) {
        new_config.layouts = all_layouts;
    }

    ಯದಿ (new_config.layouts & ~all_layouts) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                     "unsupported layout mask: "
                     "expected subset of 0x%08"PRIx32", got: 0x%08"PRIx32"\n",
                     all_layouts, new_config.layouts);
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    ಧ್ರುವ xkb_layout_index_t max_groups = format_max_groups(config->format);
    ಧ್ರುವ xkb_layout_index_t ಅಂಕೆ_groups =
        (xkb_layout_index_t)popcount32(new_config.layouts);

    ಯದಿ (ಅಂಕೆ_groups > max_groups) {
        ಧ್ರುವ bool strict =
            (new_config.flags & XKB_KEYMAP_SERIALIZE_STRICT_MODE);
        xkb_log_with_code(
            keymap->ctx,
            (strict ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
            XKB_LOG_VERBOSITY_MINIMAL,
            XKB_ERROR_LAYOUT_ಎಣಿಕೆ_LIMIT_EXCEEDED_2,
            "Cannot serialize %"PRIu32" groups in keymap format %d: "
            "maximum is %"PRIu32"%s\n",
            ಅಂಕೆ_groups, config->format, max_groups,
            (strict ? "" : "; discarding unsupported groups")
        );
        ಯದಿ (strict) {
            ಪ್ರತ್ಯಾಗಮ XKB_ERROR_LAYOUT_ಎಣಿಕೆ_LIMIT_EXCEEDED;
        } ಅನ್ಯಥಾ {
            new_config.layouts =
                keep_lowest_n_set_bits(new_config.layouts, max_groups);
        }
    }

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Serializing group mask 0x%"PRIx32" (total: %"PRIu32")\n",
            new_config.layouts, ಅಂಕೆ_groups);

    ಯದಿ (new_config.layouts != all_layouts) {
        /*
         * If some layouts are discarded, then interprets may not trigger
         * properly. Avoid this by requiring explicit values.
         */
        new_config.flags |= (XKB_KEYMAP_SERIALIZE_EXPLICIT_ಕೀಲಿ_ಮೌಲ್ಯಗಳು |
                             XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS);
    }

    ಪ್ರತ್ಯಾಗಮ ops->keymap_serialize(keymap, &new_config, result);
}

ವರ್ಣ *
xkb_keymap_get_as_string2(ಸಂರಚನಾ xkb_keymap *keymap,
                          ಗಣನಾ xkb_keymap_format format,
                          ಗಣನಾ xkb_keymap_serialize_flags flags)
{
    ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_serialize_config config = {
        .size = ಪರಿಮಾಣ(config),
        .flags = flags,
        .format = format,
        .layouts = 0,
    };

    ಸಂರಚನಾ xkb_keymap_serialize_result result = { .size = ಪರಿಮಾಣ(result) };

    ಪ್ರತ್ಯಾಗಮ (xkb_keymap_serialize(keymap, &config, &result) == XKB_SUCCESS)
        ? result.serialized
        : ರಿಕ್ತ;
}

ವರ್ಣ *
xkb_keymap_get_as_string(ಸಂರಚನಾ xkb_keymap *keymap,
                         ಗಣನಾ xkb_keymap_format format)
{
    ಪ್ರತ್ಯಾಗಮ xkb_keymap_get_as_string2(keymap, format,
                                     XKB_KEYMAP_SERIALIZE_NO_FLAGS);
}

/**
 * Returns the total number of modifiers active in the keymap.
 */
xkb_mod_index_t
xkb_keymap_ಅಂಕೆ_mods(ಸಂರಚನಾ xkb_keymap *keymap)
{
    ಪ್ರತ್ಯಾಗಮ keymap->mods.ಅಂಕೆ_mods;
}

/**
 * Return the name for a given modifier.
 */
ಧ್ರುವ ವರ್ಣ *
xkb_keymap_mod_get_name(ಸಂರಚನಾ xkb_keymap *keymap, xkb_mod_index_t idx)
{
    ಯದಿ (idx >= keymap->mods.ಅಂಕೆ_mods)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ xkb_atom_text(keymap->ctx, keymap->mods.mods[idx].name);
}

/**
 * Returns the index for a named modifier.
 */
xkb_mod_index_t
xkb_keymap_mod_get_index(ಸಂರಚನಾ xkb_keymap *keymap, ಧ್ರುವ ವರ್ಣ *name)
{
    ಧ್ರುವ xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    ಪ್ರತ್ಯಾಗಮ (atom == XKB_ATOM_NONE)
        ? XKB_MOD_INVALID
        : XkbModNameToIndex(&keymap->mods, atom, MOD_BOTH);
}

/**
 * Return the canonical mapping of a named modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask(ಸಂರಚನಾ xkb_keymap *keymap, ಧ್ರುವ ವರ್ಣ* name)
{
    ಧ್ರುವ xkb_mod_index_t idx = xkb_keymap_mod_get_index(keymap, name);
    ಪ್ರತ್ಯಾಗಮ (idx >= keymap->mods.ಅಂಕೆ_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the canonical mapping of a given modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask2(ಸಂರಚನಾ xkb_keymap *keymap, xkb_mod_index_t idx)
{
    ಪ್ರತ್ಯಾಗಮ (idx >= keymap->mods.ಅಂಕೆ_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the total number of active groups in the keymap.
 */
xkb_layout_index_t
xkb_keymap_ಅಂಕೆ_layouts(ಸಂರಚನಾ xkb_keymap *keymap)
{
    ಪ್ರತ್ಯಾಗಮ keymap->ಅಂಕೆ_groups;
}

/**
 * Returns the name for a given group.
 */
ಧ್ರುವ ವರ್ಣ *
xkb_keymap_layout_get_name(ಸಂರಚನಾ xkb_keymap *keymap, xkb_layout_index_t idx)
{
    ಯದಿ (idx >= keymap->ಅಂಕೆ_group_names)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ xkb_atom_text(keymap->ctx, keymap->group_names[idx]);
}

/**
 * Returns the index for a named layout.
 */
xkb_layout_index_t
xkb_keymap_layout_get_index(ಸಂರಚನಾ xkb_keymap *keymap, ಧ್ರುವ ವರ್ಣ *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_layout_index_t i;

    ಯದಿ (atom == XKB_ATOM_NONE)
        ಪ್ರತ್ಯಾಗಮ XKB_LAYOUT_INVALID;

    ಪ್ರತಿ (i = 0; i < keymap->ಅಂಕೆ_group_names; i++)
        ಯದಿ (keymap->group_names[i] == atom)
            ಪ್ರತ್ಯಾಗಮ i;

    ಪ್ರತ್ಯಾಗಮ XKB_LAYOUT_INVALID;
}

/**
 * Returns the number of layouts active for a particular key.
 */
xkb_layout_index_t
xkb_keymap_ಅಂಕೆ_layouts_for_ಕೀಲಿ(ಸಂರಚನಾ xkb_keymap *keymap, xkb_keycode_t kc)
{
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ = Xkb_ಕೀಲಿ(keymap, kc);

    ಯದಿ (!ಕೀಲಿ)
        ಪ್ರತ್ಯಾಗಮ 0;

    ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ->ಅಂಕೆ_groups;
}

/**
 * Returns the number of levels active for a particular key and layout.
 */
xkb_level_index_t
xkb_keymap_ಅಂಕೆ_levels_for_ಕೀಲಿ(ಸಂರಚನಾ xkb_keymap *keymap, xkb_keycode_t kc,
                              xkb_layout_index_t layout)
{
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ = Xkb_ಕೀಲಿ(keymap, kc);

    ಯದಿ (!ಕೀಲಿ)
        ಪ್ರತ್ಯಾಗಮ 0;

    layout = xkb_keymap_ಕೀಲಿ_effective_layout(ಕೀಲಿ, layout);
    ಯದಿ (layout == XKB_LAYOUT_INVALID)
        ಪ್ರತ್ಯಾಗಮ 0;

    ಪ್ರತ್ಯಾಗಮ Xkb_ಕೀಲಿ_ಅಂಕೆ_Levels(ಕೀಲಿ, layout);
}

/**
 * Return the total number of LEDs in the keymap.
 */
xkb_led_index_t
xkb_keymap_ಅಂಕೆ_leds(ಸಂರಚನಾ xkb_keymap *keymap)
{
    ಪ್ರತ್ಯಾಗಮ keymap->ಅಂಕೆ_leds;
}

/**
 * Returns the name for a given LED.
 */
ಧ್ರುವ ವರ್ಣ *
xkb_keymap_led_get_name(ಸಂರಚನಾ xkb_keymap *keymap, xkb_led_index_t idx)
{
    ಯದಿ (idx >= keymap->ಅಂಕೆ_leds)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ xkb_atom_text(keymap->ctx, keymap->leds[idx].name);
}

/**
 * Returns the index for a named LED.
 */
xkb_led_index_t
xkb_keymap_led_get_index(ಸಂರಚನಾ xkb_keymap *keymap, ಧ್ರುವ ವರ್ಣ *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_led_index_t i;
    ಧ್ರುವ ಸಂರಚನಾ xkb_led *led;

    ಯದಿ (atom == XKB_ATOM_NONE)
        ಪ್ರತ್ಯಾಗಮ XKB_LED_INVALID;

    xkb_leds_enumerate(i, led, keymap)
        ಯದಿ (led->name == atom)
            ಪ್ರತ್ಯಾಗಮ i;

    ಪ್ರತ್ಯಾಗಮ XKB_LED_INVALID;
}

size_t
xkb_keymap_ಕೀಲಿ_get_mods_for_level(ಸಂರಚನಾ xkb_keymap *keymap,
                                  xkb_keycode_t kc,
                                  xkb_layout_index_t layout,
                                  xkb_level_index_t level,
                                  xkb_mod_mask_t *masks_out,
                                  size_t masks_size)
{
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ = Xkb_ಕೀಲಿ(keymap, kc);
    ಯದಿ (!ಕೀಲಿ)
        ಪ್ರತ್ಯಾಗಮ 0;

    layout = xkb_keymap_ಕೀಲಿ_effective_layout(ಕೀಲಿ, layout);
    ಯದಿ (layout == XKB_LAYOUT_INVALID)
        ಪ್ರತ್ಯಾಗಮ 0;

    ಯದಿ (level >= Xkb_ಕೀಲಿ_ಅಂಕೆ_Levels(ಕೀಲಿ, layout))
        ಪ್ರತ್ಯಾಗಮ 0;

    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type *type = ಕೀಲಿ->groups[layout].type;

    size_t ಎಣಿಕೆ = 0;

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
    ಯದಿ (level == 0) {
        bool empty_mapped = false;
        ಪ್ರತಿ (darray_size_t i = 0; i < type->ಅಂಕೆ_entries && ಎಣಿಕೆ < masks_size; i++)
            ಯದಿ (entry_is_active(&type->entries[i]) &&
                type->entries[i].mods.mask == 0) {
                empty_mapped = true;
                ಭಙ್ಗ;
            }
        ಯದಿ (!empty_mapped && ಎಣಿಕೆ < masks_size) {
            masks_out[ಎಣಿಕೆ++] = 0;
        }
    }

    /* Now search explicit mappings. */
    ಪ್ರತಿ (darray_size_t i = 0; i < type->ಅಂಕೆ_entries && ಎಣಿಕೆ < masks_size; i++) {
        ಯದಿ (entry_is_active(&type->entries[i]) &&
            type->entries[i].level == level) {
            masks_out[ಎಣಿಕೆ++] = type->entries[i].mods.mask;
        }
    }

    ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;
}

ಸಂರಚನಾ xkb_level *
xkb_keymap_ಕೀಲಿ_get_level(ಸಂರಚನಾ xkb_keymap *keymap, ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                         xkb_layout_index_t layout, xkb_level_index_t level)
{
    layout = xkb_keymap_ಕೀಲಿ_effective_layout(ಕೀಲಿ, layout);
    ಯದಿ (layout == XKB_LAYOUT_INVALID)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಯದಿ (level >= Xkb_ಕೀಲಿ_ಅಂಕೆ_Levels(ಕೀಲಿ, layout))
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ &ಕೀಲಿ->groups[layout].levels[level];
}

/**
 * As below, but takes an explicit layout/level rather than state.
 */
ಪೂರ್ಣಾಙ್ಕ
xkb_keymap_ಕೀಲಿ_get_syms_by_level(ಸಂರಚನಾ xkb_keymap *keymap,
                                 xkb_keycode_t kc,
                                 xkb_layout_index_t layout,
                                 xkb_level_index_t level,
                                 ಧ್ರುವ xkb_keysym_t **syms_out)
{
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ = Xkb_ಕೀಲಿ(keymap, kc);

    ಯದಿ (!ಕೀಲಿ)
        ಗಚ್ಛ err;

    ಧ್ರುವ ಸಂರಚನಾ xkb_level* ಧ್ರುವ leveli =
        xkb_keymap_ಕೀಲಿ_get_level(keymap, ಕೀಲಿ, layout, level);

    ಯದಿ (!leveli)
        ಗಚ್ಛ err;

    ಧ್ರುವ xkb_keysym_ಎಣಿಕೆ_t ಅಂಕೆ_syms = leveli->ಅಂಕೆ_syms;
    ಯದಿ (ಅಂಕೆ_syms == 0)
        ಗಚ್ಛ err;

    ಯದಿ (ಅಂಕೆ_syms == 1)
        *syms_out = &leveli->s.sym;
    ಅನ್ಯಥಾ
        *syms_out = leveli->s.syms;

    ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) ಅಂಕೆ_syms;

err:
    *syms_out = ರಿಕ್ತ;
    ಪ್ರತ್ಯಾಗಮ 0;
}

xkb_keycode_t
xkb_keymap_min_keycode(ಸಂರಚನಾ xkb_keymap *keymap)
{
    ಪ್ರತ್ಯಾಗಮ keymap->min_ಕೀಲಿ_code;
}

xkb_keycode_t
xkb_keymap_max_keycode(ಸಂರಚನಾ xkb_keymap *keymap)
{
    ಪ್ರತ್ಯಾಗಮ keymap->max_ಕೀಲಿ_code;
}

ಸ್ಥಿರ ಗಣನಾ xkb_status
keymap_ಕೀಲಿ_iterator_config_check(
    ಸಂರಚನಾ xkb_context * restrict ctx,
    ಧ್ರುವ ವರ್ಣ *func,
    ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_ಕೀಲಿ_iterator_config * restrict config
)
{
    /* Check ABI compatibility */
    ಧ್ರುವ ಗಣನಾ xkb_status abi_error = xkb_check_keymap_abi(config);
    ಯದಿ (abi_error) {
        xkb_log_abi_error(ctx, func, abi_error);
        ಪ್ರತ್ಯಾಗಮ abi_error;
    }

    /* Sanitize input: flags */
    ಧ್ರುವ uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_KEYMAP_ಕೀಲಿ_ITERATOR_FLAGS_ಮೌಲ್ಯಗಳು);
    ಯದಿ (invalid_flags) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_ಕೀಲಿ_ITERATOR_FLAGS_2,
                "%s: Unsupported keymap iterator flags: 0x%"PRIx32"\n",
                func, invalid_flags);
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_UNSUPPORTED_ಕೀಲಿ_ITERATOR_FLAGS;
    }

    /* Sanitize input: start */
    ಯದಿ (config->start > XKB_KEYCODE_MAX)
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_INVALID_KEYCODE;

    ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
}

static_assert(XKB_KEYMAP_ಕೀಲಿ_ITERATOR_FLAGS_ಮೌಲ್ಯಗಳು <=
              (ಪೂರ್ಣಾಙ್ಕ)XKB_POINTER_TAG_MASK,
              "Cannot tag cursor pointer with flags");

#define iter_init_keymap(i, ptr, flags) {          \
    (i)->opaque0 = xkb_pointer_tag((ptr), (flags));\
}
#define iter_get_keymap(i) (                                  \
    (ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *)xkb_pointer_untag((i)->opaque0)\
)
#define iter_get_next(i) ((ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *)(i)->opaque1)
#define iter_set_next(i, ptr) {(i)->opaque1 = (ಧ್ರುವ ಶೂನ್ಯ *)(ptr);}
#define iter_get_descending_order(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                 \
    (uintptr_t)XKB_KEYMAP_ಕೀಲಿ_ITERATOR_DESCENDING_ORDER \
)
#define iter_get_include_unbound(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                \
    (uintptr_t)XKB_KEYMAP_ಕೀಲಿ_ITERATOR_INCLUDE_UNBOUND \
)

ಗಣನಾ xkb_status
xkb_keymap_ಕೀಲಿ_iterator_init(
    ಸಂರಚನಾ xkb_keymap_ಕೀಲಿ_iterator * restrict iter,
    ಧ್ರುವ ಸಂರಚನಾ xkb_keymap * restrict keymap,
    ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_ಕೀಲಿ_iterator_config * restrict config
)
{
    /* Handle default configuration */
    ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_ಕೀಲಿ_iterator_config default_config = {
        .size = ಪರಿಮಾಣ(default_config)
    };
    ಯದಿ (!config)
        config = &default_config;

    /* Check input */
    ಧ್ರುವ ಗಣನಾ xkb_status status =
        keymap_ಕೀಲಿ_iterator_config_check(keymap->ctx, __func__, config);
    ಯದಿ (status != XKB_SUCCESS) {
        ಪ್ರತ್ಯಾಗಮ status;
    }

    /*
     * Initialization
     */

    memset(iter, 0, ಪರಿಮಾಣ(*iter));

    /* Borrow keymap */
    iter_init_keymap(iter, keymap, config->flags);

    ಯದಿ (keymap->ಅಂಕೆ_ಕೀಲಿಗಳು == 0) {
        iter_set_next(iter, ರಿಕ್ತ);
        ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
    }

    /* Borrow key */
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *next;
    ಯದಿ (config->flags & XKB_KEYMAP_ಕೀಲಿ_ITERATOR_DESCENDING_ORDER) {
        ಯದಿ (!config->start || config->start > keymap->max_ಕೀಲಿ_code) {
            next = &keymap->ಕೀಲಿಗಳು[keymap->ಅಂಕೆ_ಕೀಲಿಗಳು - 1];
        } ಅನ್ಯಥಾ {
            /*
             * May still fail *safely* if start < keymap->min_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_ಕೀಲಿ(keymap, false, config->start);
        }
    } ಅನ್ಯಥಾ {
        ಯದಿ (!config->start || config->start < keymap->min_ಕೀಲಿ_code) {
            next = (keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ)
                ? &keymap->ಕೀಲಿಗಳು[keymap->min_ಕೀಲಿ_code]
                : &keymap->ಕೀಲಿಗಳು[0];
        } ಅನ್ಯಥಾ {
            /*
             * May still fail *safely* if start > keymap->max_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_ಕೀಲಿ(keymap, true, config->start);
        }
    };
    iter_set_next(iter, next);

    ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
}

xkb_keycode_t
xkb_keymap_ಕೀಲಿ_iterator_next(ಸಂರಚನಾ xkb_keymap_ಕೀಲಿ_iterator *iter)
{
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *next = iter_get_next(iter);
    ಯದಿ (!next)
        ಪ್ರತ್ಯಾಗಮ XKB_KEYCODE_INVALID;

    ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap = iter_get_keymap(iter);
    ಪ್ರತಿಜ್ಞಾ(keymap->ಅಂಕೆ_ಕೀಲಿಗಳು);

    ಧ್ರುವ bool descending = iter_get_descending_order(iter);
    ಧ್ರುವ bool skip_unbound = !iter_get_include_unbound(iter);
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *last = descending
        ? (keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ)
            ? &keymap->ಕೀಲಿಗಳು[keymap->min_ಕೀಲಿ_code]
            : &keymap->ಕೀಲಿಗಳು[0]
        : &keymap->ಕೀಲಿಗಳು[keymap->ಅಂಕೆ_ಕೀಲಿಗಳು - 1];
    ಧ್ರುವ ptrdiff_t step = descending ? -1 : 1;

    /* Skip undefined keys (no name) and optionally unbound keys */
    ಪ್ರತಿ (;;) {
        ಧ್ರುವ bool iter_end = descending ? (next <= last) : (next >= last);
        ಧ್ರುವ bool skip = (
            next->name == XKB_ATOM_NONE ||
            (skip_unbound && (next->ಅಂಕೆ_groups == 0))
        );
        ಯದಿ (!skip) {
            ಧ್ರುವ xkb_keycode_t ret = next->keycode;
            iter_set_next(iter, iter_end ? ರಿಕ್ತ : next + step);
            ಪ್ರತ್ಯಾಗಮ ret;
        }
        ಯದಿ (iter_end) {
            iter_set_next(iter, ರಿಕ್ತ);
            ಪ್ರತ್ಯಾಗಮ XKB_KEYCODE_INVALID;
        }
        next += step;
    }
}

#undef iter_init_keymap
#undef iter_get_keymap
#undef iter_set_next
#undef iter_get_next
#undef iter_get_descending_order
#undef iter_get_include_unbound

ಶೂನ್ಯ
xkb_keymap_ಕೀಲಿ_for_each(ಸಂರಚನಾ xkb_keymap *keymap, xkb_keymap_ಕೀಲಿ_iter_t iter,
                        ಶೂನ್ಯ *data)
{
    ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ;

    xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap)
        iter(keymap, ಕೀಲಿ->keycode, data);
}

ಧ್ರುವ ವರ್ಣ *
xkb_keymap_ಕೀಲಿ_get_name(ಸಂರಚನಾ xkb_keymap *keymap, xkb_keycode_t kc)
{
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ = Xkb_ಕೀಲಿ(keymap, kc);

    ಯದಿ (!ಕೀಲಿ)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ xkb_atom_text(keymap->ctx, ಕೀಲಿ->name);
}

xkb_keycode_t
xkb_keymap_ಕೀಲಿ_by_name(ಸಂರಚನಾ xkb_keymap *keymap, ಧ್ರುವ ವರ್ಣ *name)
{
    ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ;
    xkb_atom_t atom;

    atom = xkb_atom_lookup(keymap->ctx, name);
    ಯದಿ (atom) {
        ಪ್ರತಿ (darray_size_t i = 0; i < keymap->ಅಂಕೆ_ಕೀಲಿ_aliases; i++)
            ಯದಿ (keymap->ಕೀಲಿ_aliases[i].alias == atom)
                atom = keymap->ಕೀಲಿ_aliases[i].real;
    }
    ಯದಿ (!atom)
        ಪ್ರತ್ಯಾಗಮ XKB_KEYCODE_INVALID;

    xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap) {
        ಯದಿ (ಕೀಲಿ->name == atom)
            ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ->keycode;
    }

    ಪ್ರತ್ಯಾಗಮ XKB_KEYCODE_INVALID;
}

/**
 * Simple boolean specifying whether or not the key should repeat.
 */
ಪೂರ್ಣಾಙ್ಕ
xkb_keymap_ಕೀಲಿ_repeats(ಸಂರಚನಾ xkb_keymap *keymap, xkb_keycode_t kc)
{
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ = Xkb_ಕೀಲಿ(keymap, kc);

    ಯದಿ (!ಕೀಲಿ)
        ಪ್ರತ್ಯಾಗಮ 0;

    ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ->repeats;
}
