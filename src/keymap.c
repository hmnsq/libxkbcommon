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

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <string.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "abi-check.h"
#അന്തര്ഭാവ "atom.h"
#അന്തര്ഭാവ "context.h"
#അന്തര്ഭാവ "features/enums.h"
#അന്തര്ഭാവ "keymap.h"
#അന്തര്ഭാവ "keymap-priv.h"
#അന്തര്ഭാവ "messages-codes.h"
#അന്തര്ഭാവ "text.h"
#അന്തര്ഭാവ "util-mem.h"
#അന്തര്ഭാവ "util-numbers.h"

ഘടന xkb_keymap *
xkb_keymap_ref(ഘടന xkb_keymap *keymap)
{
    പ്രതിജ്ഞാ(keymap->refcnt > 0);
    keymap->refcnt++;
    തിരികെ keymap;
}

ശൂന്യം
clear_level(ഘടന xkb_level *leveli)
{
    എങ്കിൽ (leveli->അക്കം_syms > 1)
        സ്വതന്ത്രം(leveli->s.syms);
    എങ്കിൽ (leveli->അക്കം_actions > 1)
        സ്വതന്ത്രം(leveli->a.actions);
}

സ്ഥിതം ശൂന്യം
clear_interpret(ഘടന xkb_sym_interpret *interp)
{
    എങ്കിൽ (interp->അക്കം_actions > 1)
        സ്വതന്ത്രം(interp->a.actions);
}

ശൂന്യം
xkb_keymap_unref(ഘടന xkb_keymap *keymap)
{
    പ്രതിജ്ഞാ(!keymap || keymap->refcnt > 0);
    എങ്കിൽ (!keymap || --keymap->refcnt > 0)
        തിരികെ;

    എങ്കിൽ (keymap->താക്കോലുകൾ) {
        ഘടന xkb_താക്കോൽ *താക്കോൽ;
        xkb_താക്കോലുകൾ_foreach(താക്കോൽ, keymap) {
            എങ്കിൽ (താക്കോൽ->groups) {
                വേണ്ടി (xkb_layout_index_t i = 0; i < താക്കോൽ->അക്കം_groups; i++) {
                    എങ്കിൽ (താക്കോൽ->groups[i].levels) {
                        വേണ്ടി (xkb_level_index_t j = 0;
                             j < Xkb_താക്കോൽ_അക്കം_Levels(താക്കോൽ, i);
                             j++)
                            clear_level(&താക്കോൽ->groups[i].levels[j]);
                        സ്വതന്ത്രം(താക്കോൽ->groups[i].levels);
                    }
                }
                സ്വതന്ത്രം(താക്കോൽ->groups);
            }
            എങ്കിൽ (!താക്കോൽ->overlays_inline && താക്കോൽ->overlays_താക്കോലുകൾ) {
                സ്വതന്ത്രം((ശൂന്യം *)താക്കോൽ->overlays_താക്കോലുകൾ);
            }
        }
        സ്വതന്ത്രം(keymap->താക്കോലുകൾ);
    }
    എങ്കിൽ (keymap->types) {
        വേണ്ടി (darray_size_t i = 0; i < keymap->അക്കം_types; i++) {
            സ്വതന്ത്രം(keymap->types[i].entries);
            സ്വതന്ത്രം(keymap->types[i].level_names);
        }
        സ്വതന്ത്രം(keymap->types);
    }
    വേണ്ടി (darray_size_t k = 0; k < keymap->അക്കം_sym_interprets; k++) {
        clear_interpret(&keymap->sym_interprets[k]);
    }
    സ്വതന്ത്രം(keymap->sym_interprets);
    സ്വതന്ത്രം(keymap->modmaps);
    സ്വതന്ത്രം(keymap->താക്കോൽ_aliases);
    സ്വതന്ത്രം(keymap->group_names);
    സ്വതന്ത്രം(keymap->keycodes_section_name);
    സ്വതന്ത്രം(keymap->symbols_section_name);
    സ്വതന്ത്രം(keymap->types_section_name);
    സ്വതന്ത്രം(keymap->compat_section_name);
    xkb_context_unref(keymap->ctx);
    സ്വതന്ത്രം(keymap);
}

സ്ഥിതം സ്ഥിരം ഘടന xkb_keymap_format_ops *
get_keymap_format_ops(എണ്ണൽ xkb_keymap_format format)
{
    സ്ഥിതം സ്ഥിരം ഘടന xkb_keymap_format_ops *keymap_format_ops[] = {
        [XKB_KEYMAP_FORMAT_TEXT_V1] = &text_v1_keymap_format_ops,
        [XKB_KEYMAP_FORMAT_TEXT_V2] = &text_v1_keymap_format_ops,
    };

    എങ്കിൽ ((പൂർണ്ണസംഖ്യ) format < 0 || (പൂർണ്ണസംഖ്യ) format >= (പൂർണ്ണസംഖ്യ) ARRAY_SIZE(keymap_format_ops))
        തിരികെ ശൂന്യ;

    തിരികെ keymap_format_ops[(പൂർണ്ണസംഖ്യ) format];
}

ഘടന xkb_keymap *
xkb_keymap_new_from_rmlvo(സ്ഥിരം ഘടന xkb_rmlvo_builder *rmlvo,
                          എണ്ണൽ xkb_keymap_format format,
                          എണ്ണൽ xkb_keymap_compile_flags flags)
{
    സ്ഥിരം ഘടന xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    എങ്കിൽ (!ops || !ops->keymap_new_from_rmlvo) {
        log_err_func(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        തിരികെ ശൂന്യ;
    }

    ഘടന xkb_keymap *keymap = xkb_keymap_new(rmlvo->ctx, __func__, format,
                                               flags);
    എങ്കിൽ (!keymap)
        തിരികെ ശൂന്യ;

    എങ്കിൽ (!ops->keymap_new_from_rmlvo(keymap, rmlvo)) {
        xkb_keymap_unref(keymap);
        തിരികെ ശൂന്യ;
    }

    തിരികെ keymap;
}

ഘടന xkb_keymap *
xkb_keymap_new_from_names2(ഘടന xkb_context *ctx,
                           സ്ഥിരം ഘടന xkb_rule_names *rmlvo_in,
                           എണ്ണൽ xkb_keymap_format format,
                           എണ്ണൽ xkb_keymap_compile_flags flags)
{
    സ്ഥിരം ഘടന xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    എങ്കിൽ (!ops || !ops->keymap_new_from_names) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        തിരികെ ശൂന്യ;
    }

    ഘടന xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    എങ്കിൽ (!keymap)
        തിരികെ ശൂന്യ;

    ഘടന xkb_rule_names rmlvo = {0};
    എങ്കിൽ (rmlvo_in)
        rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    എങ്കിൽ (!ops->keymap_new_from_names(keymap, &rmlvo)) {
        xkb_keymap_unref(keymap);
        തിരികെ ശൂന്യ;
    }

    തിരികെ keymap;
}


ഘടന xkb_keymap *
xkb_keymap_new_from_names(ഘടന xkb_context *ctx,
                          സ്ഥിരം ഘടന xkb_rule_names *rmlvo_in,
                          എണ്ണൽ xkb_keymap_compile_flags flags)
{
    തിരികെ xkb_keymap_new_from_names2(ctx, rmlvo_in,
                                      XKB_KEYMAP_FORMAT_TEXT_V2, flags);
}

ഘടന xkb_keymap *
xkb_keymap_new_from_string(ഘടന xkb_context *ctx,
                           സ്ഥിരം അക്ഷരം *string,
                           എണ്ണൽ xkb_keymap_format format,
                           എണ്ണൽ xkb_keymap_compile_flags flags)
{
    തിരികെ xkb_keymap_new_from_buffer(ctx, string, strlen(string),
                                      format, flags);
}

ഘടന xkb_keymap *
xkb_keymap_new_from_buffer(ഘടന xkb_context *ctx,
                           സ്ഥിരം അക്ഷരം *buffer, size_t length,
                           എണ്ണൽ xkb_keymap_format format,
                           എണ്ണൽ xkb_keymap_compile_flags flags)
{
    സ്ഥിരം ഘടന xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    എങ്കിൽ (!ops || !ops->keymap_new_from_string) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        തിരികെ ശൂന്യ;
    }

    എങ്കിൽ (!buffer) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no buffer specified\n");
        തിരികെ ശൂന്യ;
    }

    ഘടന xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    എങ്കിൽ (!keymap)
        തിരികെ ശൂന്യ;

    /* Allow a zero-terminated string as a buffer */
    എങ്കിൽ (length > 0 && buffer[length - 1] == '\0')
        length--;

    എങ്കിൽ (!ops->keymap_new_from_string(keymap, buffer, length)) {
        xkb_keymap_unref(keymap);
        തിരികെ ശൂന്യ;
    }

    തിരികെ keymap;
}

ഘടന xkb_keymap *
xkb_keymap_new_from_file(ഘടന xkb_context *ctx,
                         FILE *file,
                         എണ്ണൽ xkb_keymap_format format,
                         എണ്ണൽ xkb_keymap_compile_flags flags)
{
    സ്ഥിരം ഘടന xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    എങ്കിൽ (!ops || !ops->keymap_new_from_file) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        തിരികെ ശൂന്യ;
    }

    എങ്കിൽ (!file) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no file specified\n");
        തിരികെ ശൂന്യ;
    }

    ഘടന xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    എങ്കിൽ (!keymap)
        തിരികെ ശൂന്യ;

    എങ്കിൽ (!ops->keymap_new_from_file(keymap, file)) {
        xkb_keymap_unref(keymap);
        തിരികെ ശൂന്യ;
    }

    തിരികെ keymap;
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
സ്ഥിതം എണ്ണൽ xkb_status
check_keymap_serialize_abi(
    ഘടന xkb_context * restrict ctx,
    സ്ഥിരം അക്ഷരം * restrict func,
    സ്ഥിരം ഘടന xkb_keymap_serialize_config * restrict config,
    സ്ഥിരം ഘടന xkb_keymap_serialize_result * restrict result
)
{
    എണ്ണൽ xkb_status status;
    എങ്കിൽ ((status = xkb_check_keymap_abi(config)) ||
        (status = xkb_check_keymap_abi(result))) {
        xkb_log_abi_error(ctx, func, status);
    }
    തിരികെ status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

എണ്ണൽ xkb_status
xkb_keymap_serialize(സ്ഥിരം ഘടന xkb_keymap *keymap,
                     സ്ഥിരം ഘടന xkb_keymap_serialize_config *config,
                     ഘടന xkb_keymap_serialize_result *result)
{
    /* Check ABI compatibility */
    സ്ഥിരം എണ്ണൽ xkb_status status =
        check_keymap_serialize_abi(keymap->ctx, __func__, config, result);
    എങ്കിൽ (status != XKB_SUCCESS)
        തിരികെ status;

    ഘടന xkb_keymap_serialize_config new_config = *config;

    സ്ഥിരം എണ്ണൽ xkb_keymap_serialize_flags invalid_flags = (
        new_config.flags &
        ~(എണ്ണൽ xkb_keymap_serialize_flags)XKB_KEYMAP_SERIALIZE_FLAGS_മൂല്യങ്ങൾ
    );
    എങ്കിൽ (invalid_flags) {
        log_err_func(keymap->ctx,
                     XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS_,
                     "unrecognized serialization flags: %#x\n",
                     invalid_flags);
        തിരികെ XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS;
    }

    എങ്കിൽ (new_config.format == XKB_KEYMAP_USE_ORIGINAL_FORMAT)
        new_config.format = keymap->format;

    സ്ഥിരം ഘടന xkb_keymap_format_ops * സ്ഥിരം ops =
        get_keymap_format_ops(new_config.format);
    എങ്കിൽ (!ops || !ops->keymap_serialize) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", new_config.format);
        തിരികെ XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT;
    }

    സ്ഥിരം xkb_layout_mask_t all_layouts =
        (xkb_layout_mask_t)((UINT64_C(1) << keymap->അക്കം_groups) - 1);
    എങ്കിൽ (!new_config.layouts) {
        new_config.layouts = all_layouts;
    }

    എങ്കിൽ (new_config.layouts & ~all_layouts) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                     "unsupported layout mask: "
                     "expected subset of 0x%08"PRIx32", got: 0x%08"PRIx32"\n",
                     all_layouts, new_config.layouts);
        തിരികെ XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    സ്ഥിരം xkb_layout_index_t max_groups = format_max_groups(config->format);
    സ്ഥിരം xkb_layout_index_t അക്കം_groups =
        (xkb_layout_index_t)popcount32(new_config.layouts);

    എങ്കിൽ (അക്കം_groups > max_groups) {
        സ്ഥിരം bool strict =
            (new_config.flags & XKB_KEYMAP_SERIALIZE_STRICT_MODE);
        xkb_log_with_code(
            keymap->ctx,
            (strict ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
            XKB_LOG_VERBOSITY_MINIMAL,
            XKB_ERROR_LAYOUT_എണ്ണം_LIMIT_EXCEEDED_2,
            "Cannot serialize %"PRIu32" groups in keymap format %d: "
            "maximum is %"PRIu32"%s\n",
            അക്കം_groups, config->format, max_groups,
            (strict ? "" : "; discarding unsupported groups")
        );
        എങ്കിൽ (strict) {
            തിരികെ XKB_ERROR_LAYOUT_എണ്ണം_LIMIT_EXCEEDED;
        } അല്ലെങ്കിൽ {
            new_config.layouts =
                keep_lowest_n_set_bits(new_config.layouts, max_groups);
        }
    }

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Serializing group mask 0x%"PRIx32" (total: %"PRIu32")\n",
            new_config.layouts, അക്കം_groups);

    എങ്കിൽ (new_config.layouts != all_layouts) {
        /*
         * If some layouts are discarded, then interprets may not trigger
         * properly. Avoid this by requiring explicit values.
         */
        new_config.flags |= (XKB_KEYMAP_SERIALIZE_EXPLICIT_താക്കോൽ_മൂല്യങ്ങൾ |
                             XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS);
    }

    തിരികെ ops->keymap_serialize(keymap, &new_config, result);
}

അക്ഷരം *
xkb_keymap_get_as_string2(ഘടന xkb_keymap *keymap,
                          എണ്ണൽ xkb_keymap_format format,
                          എണ്ണൽ xkb_keymap_serialize_flags flags)
{
    സ്ഥിരം ഘടന xkb_keymap_serialize_config config = {
        .size = വലുപ്പം(config),
        .flags = flags,
        .format = format,
        .layouts = 0,
    };

    ഘടന xkb_keymap_serialize_result result = { .size = വലുപ്പം(result) };

    തിരികെ (xkb_keymap_serialize(keymap, &config, &result) == XKB_SUCCESS)
        ? result.serialized
        : ശൂന്യ;
}

അക്ഷരം *
xkb_keymap_get_as_string(ഘടന xkb_keymap *keymap,
                         എണ്ണൽ xkb_keymap_format format)
{
    തിരികെ xkb_keymap_get_as_string2(keymap, format,
                                     XKB_KEYMAP_SERIALIZE_NO_FLAGS);
}

/**
 * Returns the total number of modifiers active in the keymap.
 */
xkb_mod_index_t
xkb_keymap_അക്കം_mods(ഘടന xkb_keymap *keymap)
{
    തിരികെ keymap->mods.അക്കം_mods;
}

/**
 * Return the name for a given modifier.
 */
സ്ഥിരം അക്ഷരം *
xkb_keymap_mod_get_name(ഘടന xkb_keymap *keymap, xkb_mod_index_t idx)
{
    എങ്കിൽ (idx >= keymap->mods.അക്കം_mods)
        തിരികെ ശൂന്യ;

    തിരികെ xkb_atom_text(keymap->ctx, keymap->mods.mods[idx].name);
}

/**
 * Returns the index for a named modifier.
 */
xkb_mod_index_t
xkb_keymap_mod_get_index(ഘടന xkb_keymap *keymap, സ്ഥിരം അക്ഷരം *name)
{
    സ്ഥിരം xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    തിരികെ (atom == XKB_ATOM_NONE)
        ? XKB_MOD_INVALID
        : XkbModNameToIndex(&keymap->mods, atom, MOD_BOTH);
}

/**
 * Return the canonical mapping of a named modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask(ഘടന xkb_keymap *keymap, സ്ഥിരം അക്ഷരം* name)
{
    സ്ഥിരം xkb_mod_index_t idx = xkb_keymap_mod_get_index(keymap, name);
    തിരികെ (idx >= keymap->mods.അക്കം_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the canonical mapping of a given modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask2(ഘടന xkb_keymap *keymap, xkb_mod_index_t idx)
{
    തിരികെ (idx >= keymap->mods.അക്കം_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the total number of active groups in the keymap.
 */
xkb_layout_index_t
xkb_keymap_അക്കം_layouts(ഘടന xkb_keymap *keymap)
{
    തിരികെ keymap->അക്കം_groups;
}

/**
 * Returns the name for a given group.
 */
സ്ഥിരം അക്ഷരം *
xkb_keymap_layout_get_name(ഘടന xkb_keymap *keymap, xkb_layout_index_t idx)
{
    എങ്കിൽ (idx >= keymap->അക്കം_group_names)
        തിരികെ ശൂന്യ;

    തിരികെ xkb_atom_text(keymap->ctx, keymap->group_names[idx]);
}

/**
 * Returns the index for a named layout.
 */
xkb_layout_index_t
xkb_keymap_layout_get_index(ഘടന xkb_keymap *keymap, സ്ഥിരം അക്ഷരം *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_layout_index_t i;

    എങ്കിൽ (atom == XKB_ATOM_NONE)
        തിരികെ XKB_LAYOUT_INVALID;

    വേണ്ടി (i = 0; i < keymap->അക്കം_group_names; i++)
        എങ്കിൽ (keymap->group_names[i] == atom)
            തിരികെ i;

    തിരികെ XKB_LAYOUT_INVALID;
}

/**
 * Returns the number of layouts active for a particular key.
 */
xkb_layout_index_t
xkb_keymap_അക്കം_layouts_for_താക്കോൽ(ഘടന xkb_keymap *keymap, xkb_keycode_t kc)
{
    സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ = Xkb_താക്കോൽ(keymap, kc);

    എങ്കിൽ (!താക്കോൽ)
        തിരികെ 0;

    തിരികെ താക്കോൽ->അക്കം_groups;
}

/**
 * Returns the number of levels active for a particular key and layout.
 */
xkb_level_index_t
xkb_keymap_അക്കം_levels_for_താക്കോൽ(ഘടന xkb_keymap *keymap, xkb_keycode_t kc,
                              xkb_layout_index_t layout)
{
    സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ = Xkb_താക്കോൽ(keymap, kc);

    എങ്കിൽ (!താക്കോൽ)
        തിരികെ 0;

    layout = xkb_keymap_താക്കോൽ_effective_layout(താക്കോൽ, layout);
    എങ്കിൽ (layout == XKB_LAYOUT_INVALID)
        തിരികെ 0;

    തിരികെ Xkb_താക്കോൽ_അക്കം_Levels(താക്കോൽ, layout);
}

/**
 * Return the total number of LEDs in the keymap.
 */
xkb_led_index_t
xkb_keymap_അക്കം_leds(ഘടന xkb_keymap *keymap)
{
    തിരികെ keymap->അക്കം_leds;
}

/**
 * Returns the name for a given LED.
 */
സ്ഥിരം അക്ഷരം *
xkb_keymap_led_get_name(ഘടന xkb_keymap *keymap, xkb_led_index_t idx)
{
    എങ്കിൽ (idx >= keymap->അക്കം_leds)
        തിരികെ ശൂന്യ;

    തിരികെ xkb_atom_text(keymap->ctx, keymap->leds[idx].name);
}

/**
 * Returns the index for a named LED.
 */
xkb_led_index_t
xkb_keymap_led_get_index(ഘടന xkb_keymap *keymap, സ്ഥിരം അക്ഷരം *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_led_index_t i;
    സ്ഥിരം ഘടന xkb_led *led;

    എങ്കിൽ (atom == XKB_ATOM_NONE)
        തിരികെ XKB_LED_INVALID;

    xkb_leds_enumerate(i, led, keymap)
        എങ്കിൽ (led->name == atom)
            തിരികെ i;

    തിരികെ XKB_LED_INVALID;
}

size_t
xkb_keymap_താക്കോൽ_get_mods_for_level(ഘടന xkb_keymap *keymap,
                                  xkb_keycode_t kc,
                                  xkb_layout_index_t layout,
                                  xkb_level_index_t level,
                                  xkb_mod_mask_t *masks_out,
                                  size_t masks_size)
{
    സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ = Xkb_താക്കോൽ(keymap, kc);
    എങ്കിൽ (!താക്കോൽ)
        തിരികെ 0;

    layout = xkb_keymap_താക്കോൽ_effective_layout(താക്കോൽ, layout);
    എങ്കിൽ (layout == XKB_LAYOUT_INVALID)
        തിരികെ 0;

    എങ്കിൽ (level >= Xkb_താക്കോൽ_അക്കം_Levels(താക്കോൽ, layout))
        തിരികെ 0;

    സ്ഥിരം ഘടന xkb_താക്കോൽ_type *type = താക്കോൽ->groups[layout].type;

    size_t എണ്ണം = 0;

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
    എങ്കിൽ (level == 0) {
        bool empty_mapped = false;
        വേണ്ടി (darray_size_t i = 0; i < type->അക്കം_entries && എണ്ണം < masks_size; i++)
            എങ്കിൽ (entry_is_active(&type->entries[i]) &&
                type->entries[i].mods.mask == 0) {
                empty_mapped = true;
                നിർത്തുക;
            }
        എങ്കിൽ (!empty_mapped && എണ്ണം < masks_size) {
            masks_out[എണ്ണം++] = 0;
        }
    }

    /* Now search explicit mappings. */
    വേണ്ടി (darray_size_t i = 0; i < type->അക്കം_entries && എണ്ണം < masks_size; i++) {
        എങ്കിൽ (entry_is_active(&type->entries[i]) &&
            type->entries[i].level == level) {
            masks_out[എണ്ണം++] = type->entries[i].mods.mask;
        }
    }

    തിരികെ എണ്ണം;
}

ഘടന xkb_level *
xkb_keymap_താക്കോൽ_get_level(ഘടന xkb_keymap *keymap, സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                         xkb_layout_index_t layout, xkb_level_index_t level)
{
    layout = xkb_keymap_താക്കോൽ_effective_layout(താക്കോൽ, layout);
    എങ്കിൽ (layout == XKB_LAYOUT_INVALID)
        തിരികെ ശൂന്യ;

    എങ്കിൽ (level >= Xkb_താക്കോൽ_അക്കം_Levels(താക്കോൽ, layout))
        തിരികെ ശൂന്യ;

    തിരികെ &താക്കോൽ->groups[layout].levels[level];
}

/**
 * As below, but takes an explicit layout/level rather than state.
 */
പൂർണ്ണസംഖ്യ
xkb_keymap_താക്കോൽ_get_syms_by_level(ഘടന xkb_keymap *keymap,
                                 xkb_keycode_t kc,
                                 xkb_layout_index_t layout,
                                 xkb_level_index_t level,
                                 സ്ഥിരം xkb_keysym_t **syms_out)
{
    സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ = Xkb_താക്കോൽ(keymap, kc);

    എങ്കിൽ (!താക്കോൽ)
        പോകുക err;

    സ്ഥിരം ഘടന xkb_level* സ്ഥിരം leveli =
        xkb_keymap_താക്കോൽ_get_level(keymap, താക്കോൽ, layout, level);

    എങ്കിൽ (!leveli)
        പോകുക err;

    സ്ഥിരം xkb_keysym_എണ്ണം_t അക്കം_syms = leveli->അക്കം_syms;
    എങ്കിൽ (അക്കം_syms == 0)
        പോകുക err;

    എങ്കിൽ (അക്കം_syms == 1)
        *syms_out = &leveli->s.sym;
    അല്ലെങ്കിൽ
        *syms_out = leveli->s.syms;

    തിരികെ (പൂർണ്ണസംഖ്യ) അക്കം_syms;

err:
    *syms_out = ശൂന്യ;
    തിരികെ 0;
}

xkb_keycode_t
xkb_keymap_min_keycode(ഘടന xkb_keymap *keymap)
{
    തിരികെ keymap->min_താക്കോൽ_code;
}

xkb_keycode_t
xkb_keymap_max_keycode(ഘടന xkb_keymap *keymap)
{
    തിരികെ keymap->max_താക്കോൽ_code;
}

സ്ഥിതം എണ്ണൽ xkb_status
keymap_താക്കോൽ_iterator_config_check(
    ഘടന xkb_context * restrict ctx,
    സ്ഥിരം അക്ഷരം *func,
    സ്ഥിരം ഘടന xkb_keymap_താക്കോൽ_iterator_config * restrict config
)
{
    /* Check ABI compatibility */
    സ്ഥിരം എണ്ണൽ xkb_status abi_error = xkb_check_keymap_abi(config);
    എങ്കിൽ (abi_error) {
        xkb_log_abi_error(ctx, func, abi_error);
        തിരികെ abi_error;
    }

    /* Sanitize input: flags */
    സ്ഥിരം uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_KEYMAP_താക്കോൽ_ITERATOR_FLAGS_മൂല്യങ്ങൾ);
    എങ്കിൽ (invalid_flags) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_താക്കോൽ_ITERATOR_FLAGS_2,
                "%s: Unsupported keymap iterator flags: 0x%"PRIx32"\n",
                func, invalid_flags);
        തിരികെ XKB_ERROR_UNSUPPORTED_താക്കോൽ_ITERATOR_FLAGS;
    }

    /* Sanitize input: start */
    എങ്കിൽ (config->start > XKB_KEYCODE_MAX)
        തിരികെ XKB_ERROR_INVALID_KEYCODE;

    തിരികെ XKB_SUCCESS;
}

static_assert(XKB_KEYMAP_താക്കോൽ_ITERATOR_FLAGS_മൂല്യങ്ങൾ <=
              (പൂർണ്ണസംഖ്യ)XKB_POINTER_TAG_MASK,
              "Cannot tag cursor pointer with flags");

#define iter_init_keymap(i, ptr, flags) {          \
    (i)->opaque0 = xkb_pointer_tag((ptr), (flags));\
}
#define iter_get_keymap(i) (                                  \
    (സ്ഥിരം ഘടന xkb_keymap *)xkb_pointer_untag((i)->opaque0)\
)
#define iter_get_next(i) ((സ്ഥിരം ഘടന xkb_താക്കോൽ *)(i)->opaque1)
#define iter_set_next(i, ptr) {(i)->opaque1 = (സ്ഥിരം ശൂന്യം *)(ptr);}
#define iter_get_descending_order(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                 \
    (uintptr_t)XKB_KEYMAP_താക്കോൽ_ITERATOR_DESCENDING_ORDER \
)
#define iter_get_include_unbound(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                \
    (uintptr_t)XKB_KEYMAP_താക്കോൽ_ITERATOR_INCLUDE_UNBOUND \
)

എണ്ണൽ xkb_status
xkb_keymap_താക്കോൽ_iterator_init(
    ഘടന xkb_keymap_താക്കോൽ_iterator * restrict iter,
    സ്ഥിരം ഘടന xkb_keymap * restrict keymap,
    സ്ഥിരം ഘടന xkb_keymap_താക്കോൽ_iterator_config * restrict config
)
{
    /* Handle default configuration */
    സ്ഥിതം സ്ഥിരം ഘടന xkb_keymap_താക്കോൽ_iterator_config default_config = {
        .size = വലുപ്പം(default_config)
    };
    എങ്കിൽ (!config)
        config = &default_config;

    /* Check input */
    സ്ഥിരം എണ്ണൽ xkb_status status =
        keymap_താക്കോൽ_iterator_config_check(keymap->ctx, __func__, config);
    എങ്കിൽ (status != XKB_SUCCESS) {
        തിരികെ status;
    }

    /*
     * Initialization
     */

    memset(iter, 0, വലുപ്പം(*iter));

    /* Borrow keymap */
    iter_init_keymap(iter, keymap, config->flags);

    എങ്കിൽ (keymap->അക്കം_താക്കോലുകൾ == 0) {
        iter_set_next(iter, ശൂന്യ);
        തിരികെ XKB_SUCCESS;
    }

    /* Borrow key */
    സ്ഥിരം ഘടന xkb_താക്കോൽ *next;
    എങ്കിൽ (config->flags & XKB_KEYMAP_താക്കോൽ_ITERATOR_DESCENDING_ORDER) {
        എങ്കിൽ (!config->start || config->start > keymap->max_താക്കോൽ_code) {
            next = &keymap->താക്കോലുകൾ[keymap->അക്കം_താക്കോലുകൾ - 1];
        } അല്ലെങ്കിൽ {
            /*
             * May still fail *safely* if start < keymap->min_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_താക്കോൽ(keymap, false, config->start);
        }
    } അല്ലെങ്കിൽ {
        എങ്കിൽ (!config->start || config->start < keymap->min_താക്കോൽ_code) {
            next = (keymap->അക്കം_താക്കോലുകൾ_താഴെയുള്ള)
                ? &keymap->താക്കോലുകൾ[keymap->min_താക്കോൽ_code]
                : &keymap->താക്കോലുകൾ[0];
        } അല്ലെങ്കിൽ {
            /*
             * May still fail *safely* if start > keymap->max_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_താക്കോൽ(keymap, true, config->start);
        }
    };
    iter_set_next(iter, next);

    തിരികെ XKB_SUCCESS;
}

xkb_keycode_t
xkb_keymap_താക്കോൽ_iterator_next(ഘടന xkb_keymap_താക്കോൽ_iterator *iter)
{
    സ്ഥിരം ഘടന xkb_താക്കോൽ *next = iter_get_next(iter);
    എങ്കിൽ (!next)
        തിരികെ XKB_KEYCODE_INVALID;

    സ്ഥിരം ഘടന xkb_keymap *keymap = iter_get_keymap(iter);
    പ്രതിജ്ഞാ(keymap->അക്കം_താക്കോലുകൾ);

    സ്ഥിരം bool descending = iter_get_descending_order(iter);
    സ്ഥിരം bool skip_unbound = !iter_get_include_unbound(iter);
    സ്ഥിരം ഘടന xkb_താക്കോൽ *last = descending
        ? (keymap->അക്കം_താക്കോലുകൾ_താഴെയുള്ള)
            ? &keymap->താക്കോലുകൾ[keymap->min_താക്കോൽ_code]
            : &keymap->താക്കോലുകൾ[0]
        : &keymap->താക്കോലുകൾ[keymap->അക്കം_താക്കോലുകൾ - 1];
    സ്ഥിരം ptrdiff_t step = descending ? -1 : 1;

    /* Skip undefined keys (no name) and optionally unbound keys */
    വേണ്ടി (;;) {
        സ്ഥിരം bool iter_end = descending ? (next <= last) : (next >= last);
        സ്ഥിരം bool skip = (
            next->name == XKB_ATOM_NONE ||
            (skip_unbound && (next->അക്കം_groups == 0))
        );
        എങ്കിൽ (!skip) {
            സ്ഥിരം xkb_keycode_t ret = next->keycode;
            iter_set_next(iter, iter_end ? ശൂന്യ : next + step);
            തിരികെ ret;
        }
        എങ്കിൽ (iter_end) {
            iter_set_next(iter, ശൂന്യ);
            തിരികെ XKB_KEYCODE_INVALID;
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

ശൂന്യം
xkb_keymap_താക്കോൽ_for_each(ഘടന xkb_keymap *keymap, xkb_keymap_താക്കോൽ_iter_t iter,
                        ശൂന്യം *data)
{
    ഘടന xkb_താക്കോൽ *താക്കോൽ;

    xkb_താക്കോലുകൾ_foreach(താക്കോൽ, keymap)
        iter(keymap, താക്കോൽ->keycode, data);
}

സ്ഥിരം അക്ഷരം *
xkb_keymap_താക്കോൽ_get_name(ഘടന xkb_keymap *keymap, xkb_keycode_t kc)
{
    സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ = Xkb_താക്കോൽ(keymap, kc);

    എങ്കിൽ (!താക്കോൽ)
        തിരികെ ശൂന്യ;

    തിരികെ xkb_atom_text(keymap->ctx, താക്കോൽ->name);
}

xkb_keycode_t
xkb_keymap_താക്കോൽ_by_name(ഘടന xkb_keymap *keymap, സ്ഥിരം അക്ഷരം *name)
{
    ഘടന xkb_താക്കോൽ *താക്കോൽ;
    xkb_atom_t atom;

    atom = xkb_atom_lookup(keymap->ctx, name);
    എങ്കിൽ (atom) {
        വേണ്ടി (darray_size_t i = 0; i < keymap->അക്കം_താക്കോൽ_aliases; i++)
            എങ്കിൽ (keymap->താക്കോൽ_aliases[i].alias == atom)
                atom = keymap->താക്കോൽ_aliases[i].real;
    }
    എങ്കിൽ (!atom)
        തിരികെ XKB_KEYCODE_INVALID;

    xkb_താക്കോലുകൾ_foreach(താക്കോൽ, keymap) {
        എങ്കിൽ (താക്കോൽ->name == atom)
            തിരികെ താക്കോൽ->keycode;
    }

    തിരികെ XKB_KEYCODE_INVALID;
}

/**
 * Simple boolean specifying whether or not the key should repeat.
 */
പൂർണ്ണസംഖ്യ
xkb_keymap_താക്കോൽ_repeats(ഘടന xkb_keymap *keymap, xkb_keycode_t kc)
{
    സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ = Xkb_താക്കോൽ(keymap, kc);

    എങ്കിൽ (!താക്കോൽ)
        തിരികെ 0;

    തിരികെ താക്കോൽ->repeats;
}
