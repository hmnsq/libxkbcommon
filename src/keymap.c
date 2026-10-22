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

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <assert.h>
#অন্তর্ভাৱ <stdint.h>
#অন্তর্ভাৱ <string.h>

#অন্তর্ভাৱ "xkbcommon/xkbcommon.h"
#অন্তর্ভাৱ "abi-check.h"
#অন্তর্ভাৱ "atom.h"
#অন্তর্ভাৱ "context.h"
#অন্তর্ভাৱ "features/enums.h"
#অন্তর্ভাৱ "keymap.h"
#অন্তর্ভাৱ "keymap-priv.h"
#অন্তর্ভাৱ "messages-codes.h"
#অন্তর্ভাৱ "text.h"
#অন্তর্ভাৱ "util-mem.h"
#অন্তর্ভাৱ "util-numbers.h"

সংরচনা xkb_keymap *
xkb_keymap_ref(সংরচনা xkb_keymap *keymap)
{
    প্রতিজ্ঞা(keymap->refcnt > 0);
    keymap->refcnt++;
    ঘূৰাই keymap;
}

শূন্য
clear_level(সংরচনা xkb_level *leveli)
{
    যদি (leveli->অঙ্ক_syms > 1)
        মুক্ত(leveli->s.syms);
    যদি (leveli->অঙ্ক_actions > 1)
        মুক্ত(leveli->a.actions);
}

স্থির শূন্য
clear_interpret(সংরচনা xkb_sym_interpret *interp)
{
    যদি (interp->অঙ্ক_actions > 1)
        মুক্ত(interp->a.actions);
}

শূন্য
xkb_keymap_unref(সংরচনা xkb_keymap *keymap)
{
    প্রতিজ্ঞা(!keymap || keymap->refcnt > 0);
    যদি (!keymap || --keymap->refcnt > 0)
        ঘূৰাই;

    যদি (keymap->চাবিগুলি) {
        সংরচনা xkb_চাবি *চাবি;
        xkb_চাবিগুলি_foreach(চাবি, keymap) {
            যদি (চাবি->groups) {
                বাবে (xkb_layout_index_t i = 0; i < চাবি->অঙ্ক_groups; i++) {
                    যদি (চাবি->groups[i].levels) {
                        বাবে (xkb_level_index_t j = 0;
                             j < Xkb_চাবি_অঙ্ক_Levels(চাবি, i);
                             j++)
                            clear_level(&চাবি->groups[i].levels[j]);
                        মুক্ত(চাবি->groups[i].levels);
                    }
                }
                মুক্ত(চাবি->groups);
            }
            যদি (!চাবি->overlays_inline && চাবি->overlays_চাবিগুলি) {
                মুক্ত((শূন্য *)চাবি->overlays_চাবিগুলি);
            }
        }
        মুক্ত(keymap->চাবিগুলি);
    }
    যদি (keymap->types) {
        বাবে (darray_size_t i = 0; i < keymap->অঙ্ক_types; i++) {
            মুক্ত(keymap->types[i].entries);
            মুক্ত(keymap->types[i].level_names);
        }
        মুক্ত(keymap->types);
    }
    বাবে (darray_size_t k = 0; k < keymap->অঙ্ক_sym_interprets; k++) {
        clear_interpret(&keymap->sym_interprets[k]);
    }
    মুক্ত(keymap->sym_interprets);
    মুক্ত(keymap->modmaps);
    মুক্ত(keymap->চাবি_aliases);
    মুক্ত(keymap->group_names);
    মুক্ত(keymap->keycodes_section_name);
    মুক্ত(keymap->symbols_section_name);
    মুক্ত(keymap->types_section_name);
    মুক্ত(keymap->compat_section_name);
    xkb_context_unref(keymap->ctx);
    মুক্ত(keymap);
}

স্থির ধ্রুৱ সংরচনা xkb_keymap_format_ops *
get_keymap_format_ops(গণনা xkb_keymap_format format)
{
    স্থির ধ্রুৱ সংরচনা xkb_keymap_format_ops *keymap_format_ops[] = {
        [XKB_KEYMAP_FORMAT_TEXT_V1] = &text_v1_keymap_format_ops,
        [XKB_KEYMAP_FORMAT_TEXT_V2] = &text_v1_keymap_format_ops,
    };

    যদি ((পূর্ণাঙ্ক) format < 0 || (পূর্ণাঙ্ক) format >= (পূর্ণাঙ্ক) ARRAY_SIZE(keymap_format_ops))
        ঘূৰাই রিক্ত;

    ঘূৰাই keymap_format_ops[(পূর্ণাঙ্ক) format];
}

সংরচনা xkb_keymap *
xkb_keymap_new_from_rmlvo(ধ্রুৱ সংরচনা xkb_rmlvo_builder *rmlvo,
                          গণনা xkb_keymap_format format,
                          গণনা xkb_keymap_compile_flags flags)
{
    ধ্রুৱ সংরচনা xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    যদি (!ops || !ops->keymap_new_from_rmlvo) {
        log_err_func(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ঘূৰাই রিক্ত;
    }

    সংরচনা xkb_keymap *keymap = xkb_keymap_new(rmlvo->ctx, __func__, format,
                                               flags);
    যদি (!keymap)
        ঘূৰাই রিক্ত;

    যদি (!ops->keymap_new_from_rmlvo(keymap, rmlvo)) {
        xkb_keymap_unref(keymap);
        ঘূৰাই রিক্ত;
    }

    ঘূৰাই keymap;
}

সংরচনা xkb_keymap *
xkb_keymap_new_from_names2(সংরচনা xkb_context *ctx,
                           ধ্রুৱ সংরচনা xkb_rule_names *rmlvo_in,
                           গণনা xkb_keymap_format format,
                           গণনা xkb_keymap_compile_flags flags)
{
    ধ্রুৱ সংরচনা xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    যদি (!ops || !ops->keymap_new_from_names) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ঘূৰাই রিক্ত;
    }

    সংরচনা xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    যদি (!keymap)
        ঘূৰাই রিক্ত;

    সংরচনা xkb_rule_names rmlvo = {0};
    যদি (rmlvo_in)
        rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    যদি (!ops->keymap_new_from_names(keymap, &rmlvo)) {
        xkb_keymap_unref(keymap);
        ঘূৰাই রিক্ত;
    }

    ঘূৰাই keymap;
}


সংরচনা xkb_keymap *
xkb_keymap_new_from_names(সংরচনা xkb_context *ctx,
                          ধ্রুৱ সংরচনা xkb_rule_names *rmlvo_in,
                          গণনা xkb_keymap_compile_flags flags)
{
    ঘূৰাই xkb_keymap_new_from_names2(ctx, rmlvo_in,
                                      XKB_KEYMAP_FORMAT_TEXT_V2, flags);
}

সংরচনা xkb_keymap *
xkb_keymap_new_from_string(সংরচনা xkb_context *ctx,
                           ধ্রুৱ ৱর্ণ *string,
                           গণনা xkb_keymap_format format,
                           গণনা xkb_keymap_compile_flags flags)
{
    ঘূৰাই xkb_keymap_new_from_buffer(ctx, string, strlen(string),
                                      format, flags);
}

সংরচনা xkb_keymap *
xkb_keymap_new_from_buffer(সংরচনা xkb_context *ctx,
                           ধ্রুৱ ৱর্ণ *buffer, size_t length,
                           গণনা xkb_keymap_format format,
                           গণনা xkb_keymap_compile_flags flags)
{
    ধ্রুৱ সংরচনা xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    যদি (!ops || !ops->keymap_new_from_string) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ঘূৰাই রিক্ত;
    }

    যদি (!buffer) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no buffer specified\n");
        ঘূৰাই রিক্ত;
    }

    সংরচনা xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    যদি (!keymap)
        ঘূৰাই রিক্ত;

    /* Allow a zero-terminated string as a buffer */
    যদি (length > 0 && buffer[length - 1] == '\0')
        length--;

    যদি (!ops->keymap_new_from_string(keymap, buffer, length)) {
        xkb_keymap_unref(keymap);
        ঘূৰাই রিক্ত;
    }

    ঘূৰাই keymap;
}

সংরচনা xkb_keymap *
xkb_keymap_new_from_file(সংরচনা xkb_context *ctx,
                         FILE *file,
                         গণনা xkb_keymap_format format,
                         গণনা xkb_keymap_compile_flags flags)
{
    ধ্রুৱ সংরচনা xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    যদি (!ops || !ops->keymap_new_from_file) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ঘূৰাই রিক্ত;
    }

    যদি (!file) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no file specified\n");
        ঘূৰাই রিক্ত;
    }

    সংরচনা xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    যদি (!keymap)
        ঘূৰাই রিক্ত;

    যদি (!ops->keymap_new_from_file(keymap, file)) {
        xkb_keymap_unref(keymap);
        ঘূৰাই রিক্ত;
    }

    ঘূৰাই keymap;
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
স্থির গণনা xkb_status
check_keymap_serialize_abi(
    সংরচনা xkb_context * restrict ctx,
    ধ্রুৱ ৱর্ণ * restrict func,
    ধ্রুৱ সংরচনা xkb_keymap_serialize_config * restrict config,
    ধ্রুৱ সংরচনা xkb_keymap_serialize_result * restrict result
)
{
    গণনা xkb_status status;
    যদি ((status = xkb_check_keymap_abi(config)) ||
        (status = xkb_check_keymap_abi(result))) {
        xkb_log_abi_error(ctx, func, status);
    }
    ঘূৰাই status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

গণনা xkb_status
xkb_keymap_serialize(ধ্রুৱ সংরচনা xkb_keymap *keymap,
                     ধ্রুৱ সংরচনা xkb_keymap_serialize_config *config,
                     সংরচনা xkb_keymap_serialize_result *result)
{
    /* Check ABI compatibility */
    ধ্রুৱ গণনা xkb_status status =
        check_keymap_serialize_abi(keymap->ctx, __func__, config, result);
    যদি (status != XKB_SUCCESS)
        ঘূৰাই status;

    সংরচনা xkb_keymap_serialize_config new_config = *config;

    ধ্রুৱ গণনা xkb_keymap_serialize_flags invalid_flags = (
        new_config.flags &
        ~(গণনা xkb_keymap_serialize_flags)XKB_KEYMAP_SERIALIZE_FLAGS_মানসমূহ
    );
    যদি (invalid_flags) {
        log_err_func(keymap->ctx,
                     XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS_,
                     "unrecognized serialization flags: %#x\n",
                     invalid_flags);
        ঘূৰাই XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS;
    }

    যদি (new_config.format == XKB_KEYMAP_USE_ORIGINAL_FORMAT)
        new_config.format = keymap->format;

    ধ্রুৱ সংরচনা xkb_keymap_format_ops * ধ্রুৱ ops =
        get_keymap_format_ops(new_config.format);
    যদি (!ops || !ops->keymap_serialize) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", new_config.format);
        ঘূৰাই XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT;
    }

    ধ্রুৱ xkb_layout_mask_t all_layouts =
        (xkb_layout_mask_t)((UINT64_C(1) << keymap->অঙ্ক_groups) - 1);
    যদি (!new_config.layouts) {
        new_config.layouts = all_layouts;
    }

    যদি (new_config.layouts & ~all_layouts) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                     "unsupported layout mask: "
                     "expected subset of 0x%08"PRIx32", got: 0x%08"PRIx32"\n",
                     all_layouts, new_config.layouts);
        ঘূৰাই XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    ধ্রুৱ xkb_layout_index_t max_groups = format_max_groups(config->format);
    ধ্রুৱ xkb_layout_index_t অঙ্ক_groups =
        (xkb_layout_index_t)popcount32(new_config.layouts);

    যদি (অঙ্ক_groups > max_groups) {
        ধ্রুৱ bool strict =
            (new_config.flags & XKB_KEYMAP_SERIALIZE_STRICT_MODE);
        xkb_log_with_code(
            keymap->ctx,
            (strict ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
            XKB_LOG_VERBOSITY_MINIMAL,
            XKB_ERROR_LAYOUT_গুনতি_LIMIT_EXCEEDED_2,
            "Cannot serialize %"PRIu32" groups in keymap format %d: "
            "maximum is %"PRIu32"%s\n",
            অঙ্ক_groups, config->format, max_groups,
            (strict ? "" : "; discarding unsupported groups")
        );
        যদি (strict) {
            ঘূৰাই XKB_ERROR_LAYOUT_গুনতি_LIMIT_EXCEEDED;
        } অন্যথা {
            new_config.layouts =
                keep_lowest_n_set_bits(new_config.layouts, max_groups);
        }
    }

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Serializing group mask 0x%"PRIx32" (total: %"PRIu32")\n",
            new_config.layouts, অঙ্ক_groups);

    যদি (new_config.layouts != all_layouts) {
        /*
         * If some layouts are discarded, then interprets may not trigger
         * properly. Avoid this by requiring explicit values.
         */
        new_config.flags |= (XKB_KEYMAP_SERIALIZE_EXPLICIT_চাবি_মানসমূহ |
                             XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS);
    }

    ঘূৰাই ops->keymap_serialize(keymap, &new_config, result);
}

ৱর্ণ *
xkb_keymap_get_as_string2(সংরচনা xkb_keymap *keymap,
                          গণনা xkb_keymap_format format,
                          গণনা xkb_keymap_serialize_flags flags)
{
    ধ্রুৱ সংরচনা xkb_keymap_serialize_config config = {
        .size = পরিমাণ(config),
        .flags = flags,
        .format = format,
        .layouts = 0,
    };

    সংরচনা xkb_keymap_serialize_result result = { .size = পরিমাণ(result) };

    ঘূৰাই (xkb_keymap_serialize(keymap, &config, &result) == XKB_SUCCESS)
        ? result.serialized
        : রিক্ত;
}

ৱর্ণ *
xkb_keymap_get_as_string(সংরচনা xkb_keymap *keymap,
                         গণনা xkb_keymap_format format)
{
    ঘূৰাই xkb_keymap_get_as_string2(keymap, format,
                                     XKB_KEYMAP_SERIALIZE_NO_FLAGS);
}

/**
 * Returns the total number of modifiers active in the keymap.
 */
xkb_mod_index_t
xkb_keymap_অঙ্ক_mods(সংরচনা xkb_keymap *keymap)
{
    ঘূৰাই keymap->mods.অঙ্ক_mods;
}

/**
 * Return the name for a given modifier.
 */
ধ্রুৱ ৱর্ণ *
xkb_keymap_mod_get_name(সংরচনা xkb_keymap *keymap, xkb_mod_index_t idx)
{
    যদি (idx >= keymap->mods.অঙ্ক_mods)
        ঘূৰাই রিক্ত;

    ঘূৰাই xkb_atom_text(keymap->ctx, keymap->mods.mods[idx].name);
}

/**
 * Returns the index for a named modifier.
 */
xkb_mod_index_t
xkb_keymap_mod_get_index(সংরচনা xkb_keymap *keymap, ধ্রুৱ ৱর্ণ *name)
{
    ধ্রুৱ xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    ঘূৰাই (atom == XKB_ATOM_NONE)
        ? XKB_MOD_INVALID
        : XkbModNameToIndex(&keymap->mods, atom, MOD_BOTH);
}

/**
 * Return the canonical mapping of a named modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask(সংরচনা xkb_keymap *keymap, ধ্রুৱ ৱর্ণ* name)
{
    ধ্রুৱ xkb_mod_index_t idx = xkb_keymap_mod_get_index(keymap, name);
    ঘূৰাই (idx >= keymap->mods.অঙ্ক_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the canonical mapping of a given modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask2(সংরচনা xkb_keymap *keymap, xkb_mod_index_t idx)
{
    ঘূৰাই (idx >= keymap->mods.অঙ্ক_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the total number of active groups in the keymap.
 */
xkb_layout_index_t
xkb_keymap_অঙ্ক_layouts(সংরচনা xkb_keymap *keymap)
{
    ঘূৰাই keymap->অঙ্ক_groups;
}

/**
 * Returns the name for a given group.
 */
ধ্রুৱ ৱর্ণ *
xkb_keymap_layout_get_name(সংরচনা xkb_keymap *keymap, xkb_layout_index_t idx)
{
    যদি (idx >= keymap->অঙ্ক_group_names)
        ঘূৰাই রিক্ত;

    ঘূৰাই xkb_atom_text(keymap->ctx, keymap->group_names[idx]);
}

/**
 * Returns the index for a named layout.
 */
xkb_layout_index_t
xkb_keymap_layout_get_index(সংরচনা xkb_keymap *keymap, ধ্রুৱ ৱর্ণ *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_layout_index_t i;

    যদি (atom == XKB_ATOM_NONE)
        ঘূৰাই XKB_LAYOUT_INVALID;

    বাবে (i = 0; i < keymap->অঙ্ক_group_names; i++)
        যদি (keymap->group_names[i] == atom)
            ঘূৰাই i;

    ঘূৰাই XKB_LAYOUT_INVALID;
}

/**
 * Returns the number of layouts active for a particular key.
 */
xkb_layout_index_t
xkb_keymap_অঙ্ক_layouts_for_চাবি(সংরচনা xkb_keymap *keymap, xkb_keycode_t kc)
{
    ধ্রুৱ সংরচনা xkb_চাবি *চাবি = Xkb_চাবি(keymap, kc);

    যদি (!চাবি)
        ঘূৰাই 0;

    ঘূৰাই চাবি->অঙ্ক_groups;
}

/**
 * Returns the number of levels active for a particular key and layout.
 */
xkb_level_index_t
xkb_keymap_অঙ্ক_levels_for_চাবি(সংরচনা xkb_keymap *keymap, xkb_keycode_t kc,
                              xkb_layout_index_t layout)
{
    ধ্রুৱ সংরচনা xkb_চাবি *চাবি = Xkb_চাবি(keymap, kc);

    যদি (!চাবি)
        ঘূৰাই 0;

    layout = xkb_keymap_চাবি_effective_layout(চাবি, layout);
    যদি (layout == XKB_LAYOUT_INVALID)
        ঘূৰাই 0;

    ঘূৰাই Xkb_চাবি_অঙ্ক_Levels(চাবি, layout);
}

/**
 * Return the total number of LEDs in the keymap.
 */
xkb_led_index_t
xkb_keymap_অঙ্ক_leds(সংরচনা xkb_keymap *keymap)
{
    ঘূৰাই keymap->অঙ্ক_leds;
}

/**
 * Returns the name for a given LED.
 */
ধ্রুৱ ৱর্ণ *
xkb_keymap_led_get_name(সংরচনা xkb_keymap *keymap, xkb_led_index_t idx)
{
    যদি (idx >= keymap->অঙ্ক_leds)
        ঘূৰাই রিক্ত;

    ঘূৰাই xkb_atom_text(keymap->ctx, keymap->leds[idx].name);
}

/**
 * Returns the index for a named LED.
 */
xkb_led_index_t
xkb_keymap_led_get_index(সংরচনা xkb_keymap *keymap, ধ্রুৱ ৱর্ণ *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_led_index_t i;
    ধ্রুৱ সংরচনা xkb_led *led;

    যদি (atom == XKB_ATOM_NONE)
        ঘূৰাই XKB_LED_INVALID;

    xkb_leds_enumerate(i, led, keymap)
        যদি (led->name == atom)
            ঘূৰাই i;

    ঘূৰাই XKB_LED_INVALID;
}

size_t
xkb_keymap_চাবি_get_mods_for_level(সংরচনা xkb_keymap *keymap,
                                  xkb_keycode_t kc,
                                  xkb_layout_index_t layout,
                                  xkb_level_index_t level,
                                  xkb_mod_mask_t *masks_out,
                                  size_t masks_size)
{
    ধ্রুৱ সংরচনা xkb_চাবি *চাবি = Xkb_চাবি(keymap, kc);
    যদি (!চাবি)
        ঘূৰাই 0;

    layout = xkb_keymap_চাবি_effective_layout(চাবি, layout);
    যদি (layout == XKB_LAYOUT_INVALID)
        ঘূৰাই 0;

    যদি (level >= Xkb_চাবি_অঙ্ক_Levels(চাবি, layout))
        ঘূৰাই 0;

    ধ্রুৱ সংরচনা xkb_চাবি_type *type = চাবি->groups[layout].type;

    size_t গুনতি = 0;

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
    যদি (level == 0) {
        bool empty_mapped = false;
        বাবে (darray_size_t i = 0; i < type->অঙ্ক_entries && গুনতি < masks_size; i++)
            যদি (entry_is_active(&type->entries[i]) &&
                type->entries[i].mods.mask == 0) {
                empty_mapped = true;
                ভঙ্গ;
            }
        যদি (!empty_mapped && গুনতি < masks_size) {
            masks_out[গুনতি++] = 0;
        }
    }

    /* Now search explicit mappings. */
    বাবে (darray_size_t i = 0; i < type->অঙ্ক_entries && গুনতি < masks_size; i++) {
        যদি (entry_is_active(&type->entries[i]) &&
            type->entries[i].level == level) {
            masks_out[গুনতি++] = type->entries[i].mods.mask;
        }
    }

    ঘূৰাই গুনতি;
}

সংরচনা xkb_level *
xkb_keymap_চাবি_get_level(সংরচনা xkb_keymap *keymap, ধ্রুৱ সংরচনা xkb_চাবি *চাবি,
                         xkb_layout_index_t layout, xkb_level_index_t level)
{
    layout = xkb_keymap_চাবি_effective_layout(চাবি, layout);
    যদি (layout == XKB_LAYOUT_INVALID)
        ঘূৰাই রিক্ত;

    যদি (level >= Xkb_চাবি_অঙ্ক_Levels(চাবি, layout))
        ঘূৰাই রিক্ত;

    ঘূৰাই &চাবি->groups[layout].levels[level];
}

/**
 * As below, but takes an explicit layout/level rather than state.
 */
পূর্ণাঙ্ক
xkb_keymap_চাবি_get_syms_by_level(সংরচনা xkb_keymap *keymap,
                                 xkb_keycode_t kc,
                                 xkb_layout_index_t layout,
                                 xkb_level_index_t level,
                                 ধ্রুৱ xkb_keysym_t **syms_out)
{
    ধ্রুৱ সংরচনা xkb_চাবি *চাবি = Xkb_চাবি(keymap, kc);

    যদি (!চাবি)
        গচ্ছ err;

    ধ্রুৱ সংরচনা xkb_level* ধ্রুৱ leveli =
        xkb_keymap_চাবি_get_level(keymap, চাবি, layout, level);

    যদি (!leveli)
        গচ্ছ err;

    ধ্রুৱ xkb_keysym_গুনতি_t অঙ্ক_syms = leveli->অঙ্ক_syms;
    যদি (অঙ্ক_syms == 0)
        গচ্ছ err;

    যদি (অঙ্ক_syms == 1)
        *syms_out = &leveli->s.sym;
    অন্যথা
        *syms_out = leveli->s.syms;

    ঘূৰাই (পূর্ণাঙ্ক) অঙ্ক_syms;

err:
    *syms_out = রিক্ত;
    ঘূৰাই 0;
}

xkb_keycode_t
xkb_keymap_min_keycode(সংরচনা xkb_keymap *keymap)
{
    ঘূৰাই keymap->min_চাবি_code;
}

xkb_keycode_t
xkb_keymap_max_keycode(সংরচনা xkb_keymap *keymap)
{
    ঘূৰাই keymap->max_চাবি_code;
}

স্থির গণনা xkb_status
keymap_চাবি_iterator_config_check(
    সংরচনা xkb_context * restrict ctx,
    ধ্রুৱ ৱর্ণ *func,
    ধ্রুৱ সংরচনা xkb_keymap_চাবি_iterator_config * restrict config
)
{
    /* Check ABI compatibility */
    ধ্রুৱ গণনা xkb_status abi_error = xkb_check_keymap_abi(config);
    যদি (abi_error) {
        xkb_log_abi_error(ctx, func, abi_error);
        ঘূৰাই abi_error;
    }

    /* Sanitize input: flags */
    ধ্রুৱ uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_KEYMAP_চাবি_ITERATOR_FLAGS_মানসমূহ);
    যদি (invalid_flags) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_চাবি_ITERATOR_FLAGS_2,
                "%s: Unsupported keymap iterator flags: 0x%"PRIx32"\n",
                func, invalid_flags);
        ঘূৰাই XKB_ERROR_UNSUPPORTED_চাবি_ITERATOR_FLAGS;
    }

    /* Sanitize input: start */
    যদি (config->start > XKB_KEYCODE_MAX)
        ঘূৰাই XKB_ERROR_INVALID_KEYCODE;

    ঘূৰাই XKB_SUCCESS;
}

static_assert(XKB_KEYMAP_চাবি_ITERATOR_FLAGS_মানসমূহ <=
              (পূর্ণাঙ্ক)XKB_POINTER_TAG_MASK,
              "Cannot tag cursor pointer with flags");

#define iter_init_keymap(i, ptr, flags) {          \
    (i)->opaque0 = xkb_pointer_tag((ptr), (flags));\
}
#define iter_get_keymap(i) (                                  \
    (ধ্রুৱ সংরচনা xkb_keymap *)xkb_pointer_untag((i)->opaque0)\
)
#define iter_get_next(i) ((ধ্রুৱ সংরচনা xkb_চাবি *)(i)->opaque1)
#define iter_set_next(i, ptr) {(i)->opaque1 = (ধ্রুৱ শূন্য *)(ptr);}
#define iter_get_descending_order(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                 \
    (uintptr_t)XKB_KEYMAP_চাবি_ITERATOR_DESCENDING_ORDER \
)
#define iter_get_include_unbound(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                \
    (uintptr_t)XKB_KEYMAP_চাবি_ITERATOR_INCLUDE_UNBOUND \
)

গণনা xkb_status
xkb_keymap_চাবি_iterator_init(
    সংরচনা xkb_keymap_চাবি_iterator * restrict iter,
    ধ্রুৱ সংরচনা xkb_keymap * restrict keymap,
    ধ্রুৱ সংরচনা xkb_keymap_চাবি_iterator_config * restrict config
)
{
    /* Handle default configuration */
    স্থির ধ্রুৱ সংরচনা xkb_keymap_চাবি_iterator_config default_config = {
        .size = পরিমাণ(default_config)
    };
    যদি (!config)
        config = &default_config;

    /* Check input */
    ধ্রুৱ গণনা xkb_status status =
        keymap_চাবি_iterator_config_check(keymap->ctx, __func__, config);
    যদি (status != XKB_SUCCESS) {
        ঘূৰাই status;
    }

    /*
     * Initialization
     */

    memset(iter, 0, পরিমাণ(*iter));

    /* Borrow keymap */
    iter_init_keymap(iter, keymap, config->flags);

    যদি (keymap->অঙ্ক_চাবিগুলি == 0) {
        iter_set_next(iter, রিক্ত);
        ঘূৰাই XKB_SUCCESS;
    }

    /* Borrow key */
    ধ্রুৱ সংরচনা xkb_চাবি *next;
    যদি (config->flags & XKB_KEYMAP_চাবি_ITERATOR_DESCENDING_ORDER) {
        যদি (!config->start || config->start > keymap->max_চাবি_code) {
            next = &keymap->চাবিগুলি[keymap->অঙ্ক_চাবিগুলি - 1];
        } অন্যথা {
            /*
             * May still fail *safely* if start < keymap->min_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_চাবি(keymap, false, config->start);
        }
    } অন্যথা {
        যদি (!config->start || config->start < keymap->min_চাবি_code) {
            next = (keymap->অঙ্ক_চাবিগুলি_নিচেৰ)
                ? &keymap->চাবিগুলি[keymap->min_চাবি_code]
                : &keymap->চাবিগুলি[0];
        } অন্যথা {
            /*
             * May still fail *safely* if start > keymap->max_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_চাবি(keymap, true, config->start);
        }
    };
    iter_set_next(iter, next);

    ঘূৰাই XKB_SUCCESS;
}

xkb_keycode_t
xkb_keymap_চাবি_iterator_next(সংরচনা xkb_keymap_চাবি_iterator *iter)
{
    ধ্রুৱ সংরচনা xkb_চাবি *next = iter_get_next(iter);
    যদি (!next)
        ঘূৰাই XKB_KEYCODE_INVALID;

    ধ্রুৱ সংরচনা xkb_keymap *keymap = iter_get_keymap(iter);
    প্রতিজ্ঞা(keymap->অঙ্ক_চাবিগুলি);

    ধ্রুৱ bool descending = iter_get_descending_order(iter);
    ধ্রুৱ bool skip_unbound = !iter_get_include_unbound(iter);
    ধ্রুৱ সংরচনা xkb_চাবি *last = descending
        ? (keymap->অঙ্ক_চাবিগুলি_নিচেৰ)
            ? &keymap->চাবিগুলি[keymap->min_চাবি_code]
            : &keymap->চাবিগুলি[0]
        : &keymap->চাবিগুলি[keymap->অঙ্ক_চাবিগুলি - 1];
    ধ্রুৱ ptrdiff_t step = descending ? -1 : 1;

    /* Skip undefined keys (no name) and optionally unbound keys */
    বাবে (;;) {
        ধ্রুৱ bool iter_end = descending ? (next <= last) : (next >= last);
        ধ্রুৱ bool skip = (
            next->name == XKB_ATOM_NONE ||
            (skip_unbound && (next->অঙ্ক_groups == 0))
        );
        যদি (!skip) {
            ধ্রুৱ xkb_keycode_t ret = next->keycode;
            iter_set_next(iter, iter_end ? রিক্ত : next + step);
            ঘূৰাই ret;
        }
        যদি (iter_end) {
            iter_set_next(iter, রিক্ত);
            ঘূৰাই XKB_KEYCODE_INVALID;
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

শূন্য
xkb_keymap_চাবি_for_each(সংরচনা xkb_keymap *keymap, xkb_keymap_চাবি_iter_t iter,
                        শূন্য *data)
{
    সংরচনা xkb_চাবি *চাবি;

    xkb_চাবিগুলি_foreach(চাবি, keymap)
        iter(keymap, চাবি->keycode, data);
}

ধ্রুৱ ৱর্ণ *
xkb_keymap_চাবি_get_name(সংরচনা xkb_keymap *keymap, xkb_keycode_t kc)
{
    ধ্রুৱ সংরচনা xkb_চাবি *চাবি = Xkb_চাবি(keymap, kc);

    যদি (!চাবি)
        ঘূৰাই রিক্ত;

    ঘূৰাই xkb_atom_text(keymap->ctx, চাবি->name);
}

xkb_keycode_t
xkb_keymap_চাবি_by_name(সংরচনা xkb_keymap *keymap, ধ্রুৱ ৱর্ণ *name)
{
    সংরচনা xkb_চাবি *চাবি;
    xkb_atom_t atom;

    atom = xkb_atom_lookup(keymap->ctx, name);
    যদি (atom) {
        বাবে (darray_size_t i = 0; i < keymap->অঙ্ক_চাবি_aliases; i++)
            যদি (keymap->চাবি_aliases[i].alias == atom)
                atom = keymap->চাবি_aliases[i].real;
    }
    যদি (!atom)
        ঘূৰাই XKB_KEYCODE_INVALID;

    xkb_চাবিগুলি_foreach(চাবি, keymap) {
        যদি (চাবি->name == atom)
            ঘূৰাই চাবি->keycode;
    }

    ঘূৰাই XKB_KEYCODE_INVALID;
}

/**
 * Simple boolean specifying whether or not the key should repeat.
 */
পূর্ণাঙ্ক
xkb_keymap_চাবি_repeats(সংরচনা xkb_keymap *keymap, xkb_keycode_t kc)
{
    ধ্রুৱ সংরচনা xkb_চাবি *চাবি = Xkb_চাবি(keymap, kc);

    যদি (!চাবি)
        ঘূৰাই 0;

    ঘূৰাই চাবি->repeats;
}
