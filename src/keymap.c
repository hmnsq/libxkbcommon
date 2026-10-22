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

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ <string.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "abi-check.h"
#அந்தர்பாவ "atom.h"
#அந்தர்பாவ "context.h"
#அந்தர்பாவ "features/enums.h"
#அந்தர்பாவ "keymap.h"
#அந்தர்பாவ "keymap-priv.h"
#அந்தர்பாவ "messages-codes.h"
#அந்தர்பாவ "text.h"
#அந்தர்பாவ "util-mem.h"
#அந்தர்பாவ "util-numbers.h"

கட்டமைப்பு xkb_keymap *
xkb_keymap_ref(கட்டமைப்பு xkb_keymap *keymap)
{
    ப்ரதிஜ்ஞா(keymap->refcnt > 0);
    keymap->refcnt++;
    திரும்பு keymap;
}

வெறுமை
clear_level(கட்டமைப்பு xkb_level *leveli)
{
    எனில் (leveli->இலக்கம்_syms > 1)
        விடுவி(leveli->s.syms);
    எனில் (leveli->இலக்கம்_actions > 1)
        விடுவி(leveli->a.actions);
}

நிலையான வெறுமை
clear_interpret(கட்டமைப்பு xkb_sym_interpret *interp)
{
    எனில் (interp->இலக்கம்_actions > 1)
        விடுவி(interp->a.actions);
}

வெறுமை
xkb_keymap_unref(கட்டமைப்பு xkb_keymap *keymap)
{
    ப்ரதிஜ்ஞா(!keymap || keymap->refcnt > 0);
    எனில் (!keymap || --keymap->refcnt > 0)
        திரும்பு;

    எனில் (keymap->சாவிகள்) {
        கட்டமைப்பு xkb_சாவி *சாவி;
        xkb_சாவிகள்_foreach(சாவி, keymap) {
            எனில் (சாவி->groups) {
                க்கு (xkb_layout_index_t i = 0; i < சாவி->இலக்கம்_groups; i++) {
                    எனில் (சாவி->groups[i].levels) {
                        க்கு (xkb_level_index_t j = 0;
                             j < Xkb_சாவி_இலக்கம்_Levels(சாவி, i);
                             j++)
                            clear_level(&சாவி->groups[i].levels[j]);
                        விடுவி(சாவி->groups[i].levels);
                    }
                }
                விடுவி(சாவி->groups);
            }
            எனில் (!சாவி->overlays_inline && சாவி->overlays_சாவிகள்) {
                விடுவி((வெறுமை *)சாவி->overlays_சாவிகள்);
            }
        }
        விடுவி(keymap->சாவிகள்);
    }
    எனில் (keymap->types) {
        க்கு (darray_size_t i = 0; i < keymap->இலக்கம்_types; i++) {
            விடுவி(keymap->types[i].entries);
            விடுவி(keymap->types[i].level_names);
        }
        விடுவி(keymap->types);
    }
    க்கு (darray_size_t k = 0; k < keymap->இலக்கம்_sym_interprets; k++) {
        clear_interpret(&keymap->sym_interprets[k]);
    }
    விடுவி(keymap->sym_interprets);
    விடுவி(keymap->modmaps);
    விடுவி(keymap->சாவி_aliases);
    விடுவி(keymap->group_names);
    விடுவி(keymap->keycodes_section_name);
    விடுவி(keymap->symbols_section_name);
    விடுவி(keymap->types_section_name);
    விடுவி(keymap->compat_section_name);
    xkb_context_unref(keymap->ctx);
    விடுவி(keymap);
}

நிலையான மாறிலி கட்டமைப்பு xkb_keymap_format_ops *
get_keymap_format_ops(எண்ணிக்கை xkb_keymap_format format)
{
    நிலையான மாறிலி கட்டமைப்பு xkb_keymap_format_ops *keymap_format_ops[] = {
        [XKB_KEYMAP_FORMAT_TEXT_V1] = &text_v1_keymap_format_ops,
        [XKB_KEYMAP_FORMAT_TEXT_V2] = &text_v1_keymap_format_ops,
    };

    எனில் ((முழு) format < 0 || (முழு) format >= (முழு) ARRAY_SIZE(keymap_format_ops))
        திரும்பு வெற்று;

    திரும்பு keymap_format_ops[(முழு) format];
}

கட்டமைப்பு xkb_keymap *
xkb_keymap_new_from_rmlvo(மாறிலி கட்டமைப்பு xkb_rmlvo_builder *rmlvo,
                          எண்ணிக்கை xkb_keymap_format format,
                          எண்ணிக்கை xkb_keymap_compile_flags flags)
{
    மாறிலி கட்டமைப்பு xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    எனில் (!ops || !ops->keymap_new_from_rmlvo) {
        log_err_func(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        திரும்பு வெற்று;
    }

    கட்டமைப்பு xkb_keymap *keymap = xkb_keymap_new(rmlvo->ctx, __func__, format,
                                               flags);
    எனில் (!keymap)
        திரும்பு வெற்று;

    எனில் (!ops->keymap_new_from_rmlvo(keymap, rmlvo)) {
        xkb_keymap_unref(keymap);
        திரும்பு வெற்று;
    }

    திரும்பு keymap;
}

கட்டமைப்பு xkb_keymap *
xkb_keymap_new_from_names2(கட்டமைப்பு xkb_context *ctx,
                           மாறிலி கட்டமைப்பு xkb_rule_names *rmlvo_in,
                           எண்ணிக்கை xkb_keymap_format format,
                           எண்ணிக்கை xkb_keymap_compile_flags flags)
{
    மாறிலி கட்டமைப்பு xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    எனில் (!ops || !ops->keymap_new_from_names) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        திரும்பு வெற்று;
    }

    கட்டமைப்பு xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    எனில் (!keymap)
        திரும்பு வெற்று;

    கட்டமைப்பு xkb_rule_names rmlvo = {0};
    எனில் (rmlvo_in)
        rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    எனில் (!ops->keymap_new_from_names(keymap, &rmlvo)) {
        xkb_keymap_unref(keymap);
        திரும்பு வெற்று;
    }

    திரும்பு keymap;
}


கட்டமைப்பு xkb_keymap *
xkb_keymap_new_from_names(கட்டமைப்பு xkb_context *ctx,
                          மாறிலி கட்டமைப்பு xkb_rule_names *rmlvo_in,
                          எண்ணிக்கை xkb_keymap_compile_flags flags)
{
    திரும்பு xkb_keymap_new_from_names2(ctx, rmlvo_in,
                                      XKB_KEYMAP_FORMAT_TEXT_V2, flags);
}

கட்டமைப்பு xkb_keymap *
xkb_keymap_new_from_string(கட்டமைப்பு xkb_context *ctx,
                           மாறிலி எழுத்து *string,
                           எண்ணிக்கை xkb_keymap_format format,
                           எண்ணிக்கை xkb_keymap_compile_flags flags)
{
    திரும்பு xkb_keymap_new_from_buffer(ctx, string, strlen(string),
                                      format, flags);
}

கட்டமைப்பு xkb_keymap *
xkb_keymap_new_from_buffer(கட்டமைப்பு xkb_context *ctx,
                           மாறிலி எழுத்து *buffer, size_t length,
                           எண்ணிக்கை xkb_keymap_format format,
                           எண்ணிக்கை xkb_keymap_compile_flags flags)
{
    மாறிலி கட்டமைப்பு xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    எனில் (!ops || !ops->keymap_new_from_string) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        திரும்பு வெற்று;
    }

    எனில் (!buffer) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no buffer specified\n");
        திரும்பு வெற்று;
    }

    கட்டமைப்பு xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    எனில் (!keymap)
        திரும்பு வெற்று;

    /* Allow a zero-terminated string as a buffer */
    எனில் (length > 0 && buffer[length - 1] == '\0')
        length--;

    எனில் (!ops->keymap_new_from_string(keymap, buffer, length)) {
        xkb_keymap_unref(keymap);
        திரும்பு வெற்று;
    }

    திரும்பு keymap;
}

கட்டமைப்பு xkb_keymap *
xkb_keymap_new_from_file(கட்டமைப்பு xkb_context *ctx,
                         FILE *file,
                         எண்ணிக்கை xkb_keymap_format format,
                         எண்ணிக்கை xkb_keymap_compile_flags flags)
{
    மாறிலி கட்டமைப்பு xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    எனில் (!ops || !ops->keymap_new_from_file) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        திரும்பு வெற்று;
    }

    எனில் (!file) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no file specified\n");
        திரும்பு வெற்று;
    }

    கட்டமைப்பு xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    எனில் (!keymap)
        திரும்பு வெற்று;

    எனில் (!ops->keymap_new_from_file(keymap, file)) {
        xkb_keymap_unref(keymap);
        திரும்பு வெற்று;
    }

    திரும்பு keymap;
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
நிலையான எண்ணிக்கை xkb_status
check_keymap_serialize_abi(
    கட்டமைப்பு xkb_context * restrict ctx,
    மாறிலி எழுத்து * restrict func,
    மாறிலி கட்டமைப்பு xkb_keymap_serialize_config * restrict config,
    மாறிலி கட்டமைப்பு xkb_keymap_serialize_result * restrict result
)
{
    எண்ணிக்கை xkb_status status;
    எனில் ((status = xkb_check_keymap_abi(config)) ||
        (status = xkb_check_keymap_abi(result))) {
        xkb_log_abi_error(ctx, func, status);
    }
    திரும்பு status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

எண்ணிக்கை xkb_status
xkb_keymap_serialize(மாறிலி கட்டமைப்பு xkb_keymap *keymap,
                     மாறிலி கட்டமைப்பு xkb_keymap_serialize_config *config,
                     கட்டமைப்பு xkb_keymap_serialize_result *result)
{
    /* Check ABI compatibility */
    மாறிலி எண்ணிக்கை xkb_status status =
        check_keymap_serialize_abi(keymap->ctx, __func__, config, result);
    எனில் (status != XKB_SUCCESS)
        திரும்பு status;

    கட்டமைப்பு xkb_keymap_serialize_config new_config = *config;

    மாறிலி எண்ணிக்கை xkb_keymap_serialize_flags invalid_flags = (
        new_config.flags &
        ~(எண்ணிக்கை xkb_keymap_serialize_flags)XKB_KEYMAP_SERIALIZE_FLAGS_மதிப்புகள்
    );
    எனில் (invalid_flags) {
        log_err_func(keymap->ctx,
                     XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS_,
                     "unrecognized serialization flags: %#x\n",
                     invalid_flags);
        திரும்பு XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS;
    }

    எனில் (new_config.format == XKB_KEYMAP_USE_ORIGINAL_FORMAT)
        new_config.format = keymap->format;

    மாறிலி கட்டமைப்பு xkb_keymap_format_ops * மாறிலி ops =
        get_keymap_format_ops(new_config.format);
    எனில் (!ops || !ops->keymap_serialize) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", new_config.format);
        திரும்பு XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT;
    }

    மாறிலி xkb_layout_mask_t all_layouts =
        (xkb_layout_mask_t)((UINT64_C(1) << keymap->இலக்கம்_groups) - 1);
    எனில் (!new_config.layouts) {
        new_config.layouts = all_layouts;
    }

    எனில் (new_config.layouts & ~all_layouts) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                     "unsupported layout mask: "
                     "expected subset of 0x%08"PRIx32", got: 0x%08"PRIx32"\n",
                     all_layouts, new_config.layouts);
        திரும்பு XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    மாறிலி xkb_layout_index_t max_groups = format_max_groups(config->format);
    மாறிலி xkb_layout_index_t இலக்கம்_groups =
        (xkb_layout_index_t)popcount32(new_config.layouts);

    எனில் (இலக்கம்_groups > max_groups) {
        மாறிலி bool strict =
            (new_config.flags & XKB_KEYMAP_SERIALIZE_STRICT_MODE);
        xkb_log_with_code(
            keymap->ctx,
            (strict ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
            XKB_LOG_VERBOSITY_MINIMAL,
            XKB_ERROR_LAYOUT_மொத்தம்_LIMIT_EXCEEDED_2,
            "Cannot serialize %"PRIu32" groups in keymap format %d: "
            "maximum is %"PRIu32"%s\n",
            இலக்கம்_groups, config->format, max_groups,
            (strict ? "" : "; discarding unsupported groups")
        );
        எனில் (strict) {
            திரும்பு XKB_ERROR_LAYOUT_மொத்தம்_LIMIT_EXCEEDED;
        } இல்லையெனில் {
            new_config.layouts =
                keep_lowest_n_set_bits(new_config.layouts, max_groups);
        }
    }

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Serializing group mask 0x%"PRIx32" (total: %"PRIu32")\n",
            new_config.layouts, இலக்கம்_groups);

    எனில் (new_config.layouts != all_layouts) {
        /*
         * If some layouts are discarded, then interprets may not trigger
         * properly. Avoid this by requiring explicit values.
         */
        new_config.flags |= (XKB_KEYMAP_SERIALIZE_EXPLICIT_சாவி_மதிப்புகள் |
                             XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS);
    }

    திரும்பு ops->keymap_serialize(keymap, &new_config, result);
}

எழுத்து *
xkb_keymap_get_as_string2(கட்டமைப்பு xkb_keymap *keymap,
                          எண்ணிக்கை xkb_keymap_format format,
                          எண்ணிக்கை xkb_keymap_serialize_flags flags)
{
    மாறிலி கட்டமைப்பு xkb_keymap_serialize_config config = {
        .size = அளவு(config),
        .flags = flags,
        .format = format,
        .layouts = 0,
    };

    கட்டமைப்பு xkb_keymap_serialize_result result = { .size = அளவு(result) };

    திரும்பு (xkb_keymap_serialize(keymap, &config, &result) == XKB_SUCCESS)
        ? result.serialized
        : வெற்று;
}

எழுத்து *
xkb_keymap_get_as_string(கட்டமைப்பு xkb_keymap *keymap,
                         எண்ணிக்கை xkb_keymap_format format)
{
    திரும்பு xkb_keymap_get_as_string2(keymap, format,
                                     XKB_KEYMAP_SERIALIZE_NO_FLAGS);
}

/**
 * Returns the total number of modifiers active in the keymap.
 */
xkb_mod_index_t
xkb_keymap_இலக்கம்_mods(கட்டமைப்பு xkb_keymap *keymap)
{
    திரும்பு keymap->mods.இலக்கம்_mods;
}

/**
 * Return the name for a given modifier.
 */
மாறிலி எழுத்து *
xkb_keymap_mod_get_name(கட்டமைப்பு xkb_keymap *keymap, xkb_mod_index_t idx)
{
    எனில் (idx >= keymap->mods.இலக்கம்_mods)
        திரும்பு வெற்று;

    திரும்பு xkb_atom_text(keymap->ctx, keymap->mods.mods[idx].name);
}

/**
 * Returns the index for a named modifier.
 */
xkb_mod_index_t
xkb_keymap_mod_get_index(கட்டமைப்பு xkb_keymap *keymap, மாறிலி எழுத்து *name)
{
    மாறிலி xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    திரும்பு (atom == XKB_ATOM_NONE)
        ? XKB_MOD_INVALID
        : XkbModNameToIndex(&keymap->mods, atom, MOD_BOTH);
}

/**
 * Return the canonical mapping of a named modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask(கட்டமைப்பு xkb_keymap *keymap, மாறிலி எழுத்து* name)
{
    மாறிலி xkb_mod_index_t idx = xkb_keymap_mod_get_index(keymap, name);
    திரும்பு (idx >= keymap->mods.இலக்கம்_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the canonical mapping of a given modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask2(கட்டமைப்பு xkb_keymap *keymap, xkb_mod_index_t idx)
{
    திரும்பு (idx >= keymap->mods.இலக்கம்_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the total number of active groups in the keymap.
 */
xkb_layout_index_t
xkb_keymap_இலக்கம்_layouts(கட்டமைப்பு xkb_keymap *keymap)
{
    திரும்பு keymap->இலக்கம்_groups;
}

/**
 * Returns the name for a given group.
 */
மாறிலி எழுத்து *
xkb_keymap_layout_get_name(கட்டமைப்பு xkb_keymap *keymap, xkb_layout_index_t idx)
{
    எனில் (idx >= keymap->இலக்கம்_group_names)
        திரும்பு வெற்று;

    திரும்பு xkb_atom_text(keymap->ctx, keymap->group_names[idx]);
}

/**
 * Returns the index for a named layout.
 */
xkb_layout_index_t
xkb_keymap_layout_get_index(கட்டமைப்பு xkb_keymap *keymap, மாறிலி எழுத்து *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_layout_index_t i;

    எனில் (atom == XKB_ATOM_NONE)
        திரும்பு XKB_LAYOUT_INVALID;

    க்கு (i = 0; i < keymap->இலக்கம்_group_names; i++)
        எனில் (keymap->group_names[i] == atom)
            திரும்பு i;

    திரும்பு XKB_LAYOUT_INVALID;
}

/**
 * Returns the number of layouts active for a particular key.
 */
xkb_layout_index_t
xkb_keymap_இலக்கம்_layouts_for_சாவி(கட்டமைப்பு xkb_keymap *keymap, xkb_keycode_t kc)
{
    மாறிலி கட்டமைப்பு xkb_சாவி *சாவி = Xkb_சாவி(keymap, kc);

    எனில் (!சாவி)
        திரும்பு 0;

    திரும்பு சாவி->இலக்கம்_groups;
}

/**
 * Returns the number of levels active for a particular key and layout.
 */
xkb_level_index_t
xkb_keymap_இலக்கம்_levels_for_சாவி(கட்டமைப்பு xkb_keymap *keymap, xkb_keycode_t kc,
                              xkb_layout_index_t layout)
{
    மாறிலி கட்டமைப்பு xkb_சாவி *சாவி = Xkb_சாவி(keymap, kc);

    எனில் (!சாவி)
        திரும்பு 0;

    layout = xkb_keymap_சாவி_effective_layout(சாவி, layout);
    எனில் (layout == XKB_LAYOUT_INVALID)
        திரும்பு 0;

    திரும்பு Xkb_சாவி_இலக்கம்_Levels(சாவி, layout);
}

/**
 * Return the total number of LEDs in the keymap.
 */
xkb_led_index_t
xkb_keymap_இலக்கம்_leds(கட்டமைப்பு xkb_keymap *keymap)
{
    திரும்பு keymap->இலக்கம்_leds;
}

/**
 * Returns the name for a given LED.
 */
மாறிலி எழுத்து *
xkb_keymap_led_get_name(கட்டமைப்பு xkb_keymap *keymap, xkb_led_index_t idx)
{
    எனில் (idx >= keymap->இலக்கம்_leds)
        திரும்பு வெற்று;

    திரும்பு xkb_atom_text(keymap->ctx, keymap->leds[idx].name);
}

/**
 * Returns the index for a named LED.
 */
xkb_led_index_t
xkb_keymap_led_get_index(கட்டமைப்பு xkb_keymap *keymap, மாறிலி எழுத்து *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_led_index_t i;
    மாறிலி கட்டமைப்பு xkb_led *led;

    எனில் (atom == XKB_ATOM_NONE)
        திரும்பு XKB_LED_INVALID;

    xkb_leds_enumerate(i, led, keymap)
        எனில் (led->name == atom)
            திரும்பு i;

    திரும்பு XKB_LED_INVALID;
}

size_t
xkb_keymap_சாவி_get_mods_for_level(கட்டமைப்பு xkb_keymap *keymap,
                                  xkb_keycode_t kc,
                                  xkb_layout_index_t layout,
                                  xkb_level_index_t level,
                                  xkb_mod_mask_t *masks_out,
                                  size_t masks_size)
{
    மாறிலி கட்டமைப்பு xkb_சாவி *சாவி = Xkb_சாவி(keymap, kc);
    எனில் (!சாவி)
        திரும்பு 0;

    layout = xkb_keymap_சாவி_effective_layout(சாவி, layout);
    எனில் (layout == XKB_LAYOUT_INVALID)
        திரும்பு 0;

    எனில் (level >= Xkb_சாவி_இலக்கம்_Levels(சாவி, layout))
        திரும்பு 0;

    மாறிலி கட்டமைப்பு xkb_சாவி_type *type = சாவி->groups[layout].type;

    size_t மொத்தம் = 0;

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
    எனில் (level == 0) {
        bool empty_mapped = false;
        க்கு (darray_size_t i = 0; i < type->இலக்கம்_entries && மொத்தம் < masks_size; i++)
            எனில் (entry_is_active(&type->entries[i]) &&
                type->entries[i].mods.mask == 0) {
                empty_mapped = true;
                நிறுத்து;
            }
        எனில் (!empty_mapped && மொத்தம் < masks_size) {
            masks_out[மொத்தம்++] = 0;
        }
    }

    /* Now search explicit mappings. */
    க்கு (darray_size_t i = 0; i < type->இலக்கம்_entries && மொத்தம் < masks_size; i++) {
        எனில் (entry_is_active(&type->entries[i]) &&
            type->entries[i].level == level) {
            masks_out[மொத்தம்++] = type->entries[i].mods.mask;
        }
    }

    திரும்பு மொத்தம்;
}

கட்டமைப்பு xkb_level *
xkb_keymap_சாவி_get_level(கட்டமைப்பு xkb_keymap *keymap, மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                         xkb_layout_index_t layout, xkb_level_index_t level)
{
    layout = xkb_keymap_சாவி_effective_layout(சாவி, layout);
    எனில் (layout == XKB_LAYOUT_INVALID)
        திரும்பு வெற்று;

    எனில் (level >= Xkb_சாவி_இலக்கம்_Levels(சாவி, layout))
        திரும்பு வெற்று;

    திரும்பு &சாவி->groups[layout].levels[level];
}

/**
 * As below, but takes an explicit layout/level rather than state.
 */
முழு
xkb_keymap_சாவி_get_syms_by_level(கட்டமைப்பு xkb_keymap *keymap,
                                 xkb_keycode_t kc,
                                 xkb_layout_index_t layout,
                                 xkb_level_index_t level,
                                 மாறிலி xkb_keysym_t **syms_out)
{
    மாறிலி கட்டமைப்பு xkb_சாவி *சாவி = Xkb_சாவி(keymap, kc);

    எனில் (!சாவி)
        செல் err;

    மாறிலி கட்டமைப்பு xkb_level* மாறிலி leveli =
        xkb_keymap_சாவி_get_level(keymap, சாவி, layout, level);

    எனில் (!leveli)
        செல் err;

    மாறிலி xkb_keysym_மொத்தம்_t இலக்கம்_syms = leveli->இலக்கம்_syms;
    எனில் (இலக்கம்_syms == 0)
        செல் err;

    எனில் (இலக்கம்_syms == 1)
        *syms_out = &leveli->s.sym;
    இல்லையெனில்
        *syms_out = leveli->s.syms;

    திரும்பு (முழு) இலக்கம்_syms;

err:
    *syms_out = வெற்று;
    திரும்பு 0;
}

xkb_keycode_t
xkb_keymap_min_keycode(கட்டமைப்பு xkb_keymap *keymap)
{
    திரும்பு keymap->min_சாவி_code;
}

xkb_keycode_t
xkb_keymap_max_keycode(கட்டமைப்பு xkb_keymap *keymap)
{
    திரும்பு keymap->max_சாவி_code;
}

நிலையான எண்ணிக்கை xkb_status
keymap_சாவி_iterator_config_check(
    கட்டமைப்பு xkb_context * restrict ctx,
    மாறிலி எழுத்து *func,
    மாறிலி கட்டமைப்பு xkb_keymap_சாவி_iterator_config * restrict config
)
{
    /* Check ABI compatibility */
    மாறிலி எண்ணிக்கை xkb_status abi_error = xkb_check_keymap_abi(config);
    எனில் (abi_error) {
        xkb_log_abi_error(ctx, func, abi_error);
        திரும்பு abi_error;
    }

    /* Sanitize input: flags */
    மாறிலி uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_KEYMAP_சாவி_ITERATOR_FLAGS_மதிப்புகள்);
    எனில் (invalid_flags) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_சாவி_ITERATOR_FLAGS_2,
                "%s: Unsupported keymap iterator flags: 0x%"PRIx32"\n",
                func, invalid_flags);
        திரும்பு XKB_ERROR_UNSUPPORTED_சாவி_ITERATOR_FLAGS;
    }

    /* Sanitize input: start */
    எனில் (config->start > XKB_KEYCODE_MAX)
        திரும்பு XKB_ERROR_INVALID_KEYCODE;

    திரும்பு XKB_SUCCESS;
}

static_assert(XKB_KEYMAP_சாவி_ITERATOR_FLAGS_மதிப்புகள் <=
              (முழு)XKB_POINTER_TAG_MASK,
              "Cannot tag cursor pointer with flags");

#define iter_init_keymap(i, ptr, flags) {          \
    (i)->opaque0 = xkb_pointer_tag((ptr), (flags));\
}
#define iter_get_keymap(i) (                                  \
    (மாறிலி கட்டமைப்பு xkb_keymap *)xkb_pointer_untag((i)->opaque0)\
)
#define iter_get_next(i) ((மாறிலி கட்டமைப்பு xkb_சாவி *)(i)->opaque1)
#define iter_set_next(i, ptr) {(i)->opaque1 = (மாறிலி வெறுமை *)(ptr);}
#define iter_get_descending_order(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                 \
    (uintptr_t)XKB_KEYMAP_சாவி_ITERATOR_DESCENDING_ORDER \
)
#define iter_get_include_unbound(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                \
    (uintptr_t)XKB_KEYMAP_சாவி_ITERATOR_INCLUDE_UNBOUND \
)

எண்ணிக்கை xkb_status
xkb_keymap_சாவி_iterator_init(
    கட்டமைப்பு xkb_keymap_சாவி_iterator * restrict iter,
    மாறிலி கட்டமைப்பு xkb_keymap * restrict keymap,
    மாறிலி கட்டமைப்பு xkb_keymap_சாவி_iterator_config * restrict config
)
{
    /* Handle default configuration */
    நிலையான மாறிலி கட்டமைப்பு xkb_keymap_சாவி_iterator_config default_config = {
        .size = அளவு(default_config)
    };
    எனில் (!config)
        config = &default_config;

    /* Check input */
    மாறிலி எண்ணிக்கை xkb_status status =
        keymap_சாவி_iterator_config_check(keymap->ctx, __func__, config);
    எனில் (status != XKB_SUCCESS) {
        திரும்பு status;
    }

    /*
     * Initialization
     */

    memset(iter, 0, அளவு(*iter));

    /* Borrow keymap */
    iter_init_keymap(iter, keymap, config->flags);

    எனில் (keymap->இலக்கம்_சாவிகள் == 0) {
        iter_set_next(iter, வெற்று);
        திரும்பு XKB_SUCCESS;
    }

    /* Borrow key */
    மாறிலி கட்டமைப்பு xkb_சாவி *next;
    எனில் (config->flags & XKB_KEYMAP_சாவி_ITERATOR_DESCENDING_ORDER) {
        எனில் (!config->start || config->start > keymap->max_சாவி_code) {
            next = &keymap->சாவிகள்[keymap->இலக்கம்_சாவிகள் - 1];
        } இல்லையெனில் {
            /*
             * May still fail *safely* if start < keymap->min_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_சாவி(keymap, false, config->start);
        }
    } இல்லையெனில் {
        எனில் (!config->start || config->start < keymap->min_சாவி_code) {
            next = (keymap->இலக்கம்_சாவிகள்_கீழ்)
                ? &keymap->சாவிகள்[keymap->min_சாவி_code]
                : &keymap->சாவிகள்[0];
        } இல்லையெனில் {
            /*
             * May still fail *safely* if start > keymap->max_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_சாவி(keymap, true, config->start);
        }
    };
    iter_set_next(iter, next);

    திரும்பு XKB_SUCCESS;
}

xkb_keycode_t
xkb_keymap_சாவி_iterator_next(கட்டமைப்பு xkb_keymap_சாவி_iterator *iter)
{
    மாறிலி கட்டமைப்பு xkb_சாவி *next = iter_get_next(iter);
    எனில் (!next)
        திரும்பு XKB_KEYCODE_INVALID;

    மாறிலி கட்டமைப்பு xkb_keymap *keymap = iter_get_keymap(iter);
    ப்ரதிஜ்ஞா(keymap->இலக்கம்_சாவிகள்);

    மாறிலி bool descending = iter_get_descending_order(iter);
    மாறிலி bool skip_unbound = !iter_get_include_unbound(iter);
    மாறிலி கட்டமைப்பு xkb_சாவி *last = descending
        ? (keymap->இலக்கம்_சாவிகள்_கீழ்)
            ? &keymap->சாவிகள்[keymap->min_சாவி_code]
            : &keymap->சாவிகள்[0]
        : &keymap->சாவிகள்[keymap->இலக்கம்_சாவிகள் - 1];
    மாறிலி ptrdiff_t step = descending ? -1 : 1;

    /* Skip undefined keys (no name) and optionally unbound keys */
    க்கு (;;) {
        மாறிலி bool iter_end = descending ? (next <= last) : (next >= last);
        மாறிலி bool skip = (
            next->name == XKB_ATOM_NONE ||
            (skip_unbound && (next->இலக்கம்_groups == 0))
        );
        எனில் (!skip) {
            மாறிலி xkb_keycode_t ret = next->keycode;
            iter_set_next(iter, iter_end ? வெற்று : next + step);
            திரும்பு ret;
        }
        எனில் (iter_end) {
            iter_set_next(iter, வெற்று);
            திரும்பு XKB_KEYCODE_INVALID;
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

வெறுமை
xkb_keymap_சாவி_for_each(கட்டமைப்பு xkb_keymap *keymap, xkb_keymap_சாவி_iter_t iter,
                        வெறுமை *data)
{
    கட்டமைப்பு xkb_சாவி *சாவி;

    xkb_சாவிகள்_foreach(சாவி, keymap)
        iter(keymap, சாவி->keycode, data);
}

மாறிலி எழுத்து *
xkb_keymap_சாவி_get_name(கட்டமைப்பு xkb_keymap *keymap, xkb_keycode_t kc)
{
    மாறிலி கட்டமைப்பு xkb_சாவி *சாவி = Xkb_சாவி(keymap, kc);

    எனில் (!சாவி)
        திரும்பு வெற்று;

    திரும்பு xkb_atom_text(keymap->ctx, சாவி->name);
}

xkb_keycode_t
xkb_keymap_சாவி_by_name(கட்டமைப்பு xkb_keymap *keymap, மாறிலி எழுத்து *name)
{
    கட்டமைப்பு xkb_சாவி *சாவி;
    xkb_atom_t atom;

    atom = xkb_atom_lookup(keymap->ctx, name);
    எனில் (atom) {
        க்கு (darray_size_t i = 0; i < keymap->இலக்கம்_சாவி_aliases; i++)
            எனில் (keymap->சாவி_aliases[i].alias == atom)
                atom = keymap->சாவி_aliases[i].real;
    }
    எனில் (!atom)
        திரும்பு XKB_KEYCODE_INVALID;

    xkb_சாவிகள்_foreach(சாவி, keymap) {
        எனில் (சாவி->name == atom)
            திரும்பு சாவி->keycode;
    }

    திரும்பு XKB_KEYCODE_INVALID;
}

/**
 * Simple boolean specifying whether or not the key should repeat.
 */
முழு
xkb_keymap_சாவி_repeats(கட்டமைப்பு xkb_keymap *keymap, xkb_keycode_t kc)
{
    மாறிலி கட்டமைப்பு xkb_சாவி *சாவி = Xkb_சாவி(keymap, kc);

    எனில் (!சாவி)
        திரும்பு 0;

    திரும்பு சாவி->repeats;
}
