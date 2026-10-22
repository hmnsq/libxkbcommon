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

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "abi-check.h"
#अन्तर्भाव "atom.h"
#अन्तर्भाव "context.h"
#अन्तर्भाव "features/enums.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "keymap-priv.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "text.h"
#अन्तर्भाव "util-mem.h"
#अन्तर्भाव "util-numbers.h"

संरचना xkb_keymap *
xkb_keymap_ref(संरचना xkb_keymap *keymap)
{
    प्रतिज्ञा(keymap->refcnt > 0);
    keymap->refcnt++;
    फर्काऊ keymap;
}

शून्य
clear_level(संरचना xkb_level *leveli)
{
    यदि (leveli->अङ्क_syms > 1)
        मुक्त(leveli->s.syms);
    यदि (leveli->अङ्क_actions > 1)
        मुक्त(leveli->a.actions);
}

स्थिर शून्य
clear_interpret(संरचना xkb_sym_interpret *interp)
{
    यदि (interp->अङ्क_actions > 1)
        मुक्त(interp->a.actions);
}

शून्य
xkb_keymap_unref(संरचना xkb_keymap *keymap)
{
    प्रतिज्ञा(!keymap || keymap->refcnt > 0);
    यदि (!keymap || --keymap->refcnt > 0)
        फर्काऊ;

    यदि (keymap->साँचोहरू) {
        संरचना xkb_साँचो *साँचो;
        xkb_साँचोहरू_foreach(साँचो, keymap) {
            यदि (साँचो->groups) {
                लागि (xkb_layout_index_t i = 0; i < साँचो->अङ्क_groups; i++) {
                    यदि (साँचो->groups[i].levels) {
                        लागि (xkb_level_index_t j = 0;
                             j < Xkb_साँचो_अङ्क_Levels(साँचो, i);
                             j++)
                            clear_level(&साँचो->groups[i].levels[j]);
                        मुक्त(साँचो->groups[i].levels);
                    }
                }
                मुक्त(साँचो->groups);
            }
            यदि (!साँचो->overlays_inline && साँचो->overlays_साँचोहरू) {
                मुक्त((शून्य *)साँचो->overlays_साँचोहरू);
            }
        }
        मुक्त(keymap->साँचोहरू);
    }
    यदि (keymap->types) {
        लागि (darray_size_t i = 0; i < keymap->अङ्क_types; i++) {
            मुक्त(keymap->types[i].entries);
            मुक्त(keymap->types[i].level_names);
        }
        मुक्त(keymap->types);
    }
    लागि (darray_size_t k = 0; k < keymap->अङ्क_sym_interprets; k++) {
        clear_interpret(&keymap->sym_interprets[k]);
    }
    मुक्त(keymap->sym_interprets);
    मुक्त(keymap->modmaps);
    मुक्त(keymap->साँचो_aliases);
    मुक्त(keymap->group_names);
    मुक्त(keymap->keycodes_section_name);
    मुक्त(keymap->symbols_section_name);
    मुक्त(keymap->types_section_name);
    मुक्त(keymap->compat_section_name);
    xkb_context_unref(keymap->ctx);
    मुक्त(keymap);
}

स्थिर अचर संरचना xkb_keymap_format_ops *
get_keymap_format_ops(गणना xkb_keymap_format format)
{
    स्थिर अचर संरचना xkb_keymap_format_ops *keymap_format_ops[] = {
        [XKB_KEYMAP_FORMAT_TEXT_V1] = &text_v1_keymap_format_ops,
        [XKB_KEYMAP_FORMAT_TEXT_V2] = &text_v1_keymap_format_ops,
    };

    यदि ((पूर्णाङ्क) format < 0 || (पूर्णाङ्क) format >= (पूर्णाङ्क) ARRAY_SIZE(keymap_format_ops))
        फर्काऊ रिक्त;

    फर्काऊ keymap_format_ops[(पूर्णाङ्क) format];
}

संरचना xkb_keymap *
xkb_keymap_new_from_rmlvo(अचर संरचना xkb_rmlvo_builder *rmlvo,
                          गणना xkb_keymap_format format,
                          गणना xkb_keymap_compile_flags flags)
{
    अचर संरचना xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    यदि (!ops || !ops->keymap_new_from_rmlvo) {
        log_err_func(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        फर्काऊ रिक्त;
    }

    संरचना xkb_keymap *keymap = xkb_keymap_new(rmlvo->ctx, __func__, format,
                                               flags);
    यदि (!keymap)
        फर्काऊ रिक्त;

    यदि (!ops->keymap_new_from_rmlvo(keymap, rmlvo)) {
        xkb_keymap_unref(keymap);
        फर्काऊ रिक्त;
    }

    फर्काऊ keymap;
}

संरचना xkb_keymap *
xkb_keymap_new_from_names2(संरचना xkb_context *ctx,
                           अचर संरचना xkb_rule_names *rmlvo_in,
                           गणना xkb_keymap_format format,
                           गणना xkb_keymap_compile_flags flags)
{
    अचर संरचना xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    यदि (!ops || !ops->keymap_new_from_names) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        फर्काऊ रिक्त;
    }

    संरचना xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    यदि (!keymap)
        फर्काऊ रिक्त;

    संरचना xkb_rule_names rmlvo = {0};
    यदि (rmlvo_in)
        rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    यदि (!ops->keymap_new_from_names(keymap, &rmlvo)) {
        xkb_keymap_unref(keymap);
        फर्काऊ रिक्त;
    }

    फर्काऊ keymap;
}


संरचना xkb_keymap *
xkb_keymap_new_from_names(संरचना xkb_context *ctx,
                          अचर संरचना xkb_rule_names *rmlvo_in,
                          गणना xkb_keymap_compile_flags flags)
{
    फर्काऊ xkb_keymap_new_from_names2(ctx, rmlvo_in,
                                      XKB_KEYMAP_FORMAT_TEXT_V2, flags);
}

संरचना xkb_keymap *
xkb_keymap_new_from_string(संरचना xkb_context *ctx,
                           अचर वर्ण *string,
                           गणना xkb_keymap_format format,
                           गणना xkb_keymap_compile_flags flags)
{
    फर्काऊ xkb_keymap_new_from_buffer(ctx, string, म_माप(string),
                                      format, flags);
}

संरचना xkb_keymap *
xkb_keymap_new_from_buffer(संरचना xkb_context *ctx,
                           अचर वर्ण *buffer, माप_प्रकार length,
                           गणना xkb_keymap_format format,
                           गणना xkb_keymap_compile_flags flags)
{
    अचर संरचना xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    यदि (!ops || !ops->keymap_new_from_string) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        फर्काऊ रिक्त;
    }

    यदि (!buffer) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no buffer specified\n");
        फर्काऊ रिक्त;
    }

    संरचना xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    यदि (!keymap)
        फर्काऊ रिक्त;

    /* Allow a zero-terminated string as a buffer */
    यदि (length > 0 && buffer[length - 1] == '\0')
        length--;

    यदि (!ops->keymap_new_from_string(keymap, buffer, length)) {
        xkb_keymap_unref(keymap);
        फर्काऊ रिक्त;
    }

    फर्काऊ keymap;
}

संरचना xkb_keymap *
xkb_keymap_new_from_file(संरचना xkb_context *ctx,
                         खाता *file,
                         गणना xkb_keymap_format format,
                         गणना xkb_keymap_compile_flags flags)
{
    अचर संरचना xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    यदि (!ops || !ops->keymap_new_from_file) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        फर्काऊ रिक्त;
    }

    यदि (!file) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no file specified\n");
        फर्काऊ रिक्त;
    }

    संरचना xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    यदि (!keymap)
        फर्काऊ रिक्त;

    यदि (!ops->keymap_new_from_file(keymap, file)) {
        xkb_keymap_unref(keymap);
        फर्काऊ रिक्त;
    }

    फर्काऊ keymap;
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
स्थिर गणना xkb_status
check_keymap_serialize_abi(
    संरचना xkb_context * restrict ctx,
    अचर वर्ण * restrict func,
    अचर संरचना xkb_keymap_serialize_config * restrict config,
    अचर संरचना xkb_keymap_serialize_result * restrict result
)
{
    गणना xkb_status status;
    यदि ((status = xkb_check_keymap_abi(config)) ||
        (status = xkb_check_keymap_abi(result))) {
        xkb_log_abi_error(ctx, func, status);
    }
    फर्काऊ status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

गणना xkb_status
xkb_keymap_serialize(अचर संरचना xkb_keymap *keymap,
                     अचर संरचना xkb_keymap_serialize_config *config,
                     संरचना xkb_keymap_serialize_result *result)
{
    /* Check ABI compatibility */
    अचर गणना xkb_status status =
        check_keymap_serialize_abi(keymap->ctx, __func__, config, result);
    यदि (status != XKB_SUCCESS)
        फर्काऊ status;

    संरचना xkb_keymap_serialize_config new_config = *config;

    अचर गणना xkb_keymap_serialize_flags invalid_flags = (
        new_config.flags &
        ~(गणना xkb_keymap_serialize_flags)XKB_KEYMAP_SERIALIZE_FLAGS_मानहरू
    );
    यदि (invalid_flags) {
        log_err_func(keymap->ctx,
                     XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS_,
                     "unrecognized serialization flags: %#x\n",
                     invalid_flags);
        फर्काऊ XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS;
    }

    यदि (new_config.format == XKB_KEYMAP_USE_ORIGINAL_FORMAT)
        new_config.format = keymap->format;

    अचर संरचना xkb_keymap_format_ops * अचर ops =
        get_keymap_format_ops(new_config.format);
    यदि (!ops || !ops->keymap_serialize) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", new_config.format);
        फर्काऊ XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT;
    }

    अचर xkb_layout_mask_t all_layouts =
        (xkb_layout_mask_t)((UINT64_C(1) << keymap->अङ्क_groups) - 1);
    यदि (!new_config.layouts) {
        new_config.layouts = all_layouts;
    }

    यदि (new_config.layouts & ~all_layouts) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                     "unsupported layout mask: "
                     "expected subset of 0x%08"PRIx32", got: 0x%08"PRIx32"\n",
                     all_layouts, new_config.layouts);
        फर्काऊ XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    अचर xkb_layout_index_t max_groups = format_max_groups(config->format);
    अचर xkb_layout_index_t अङ्क_groups =
        (xkb_layout_index_t)popcount32(new_config.layouts);

    यदि (अङ्क_groups > max_groups) {
        अचर bool strict =
            (new_config.flags & XKB_KEYMAP_SERIALIZE_STRICT_MODE);
        xkb_log_with_code(
            keymap->ctx,
            (strict ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
            XKB_LOG_VERBOSITY_MINIMAL,
            XKB_ERROR_LAYOUT_गन्ती_LIMIT_EXCEEDED_2,
            "Cannot serialize %"PRIu32" groups in keymap format %d: "
            "maximum is %"PRIu32"%s\n",
            अङ्क_groups, config->format, max_groups,
            (strict ? "" : "; discarding unsupported groups")
        );
        यदि (strict) {
            फर्काऊ XKB_ERROR_LAYOUT_गन्ती_LIMIT_EXCEEDED;
        } अन्यथा {
            new_config.layouts =
                keep_lowest_n_set_bits(new_config.layouts, max_groups);
        }
    }

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Serializing group mask 0x%"PRIx32" (total: %"PRIu32")\n",
            new_config.layouts, अङ्क_groups);

    यदि (new_config.layouts != all_layouts) {
        /*
         * If some layouts are discarded, then interprets may not trigger
         * properly. Avoid this by requiring explicit values.
         */
        new_config.flags |= (XKB_KEYMAP_SERIALIZE_EXPLICIT_साँचो_मानहरू |
                             XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS);
    }

    फर्काऊ ops->keymap_serialize(keymap, &new_config, result);
}

वर्ण *
xkb_keymap_get_as_string2(संरचना xkb_keymap *keymap,
                          गणना xkb_keymap_format format,
                          गणना xkb_keymap_serialize_flags flags)
{
    अचर संरचना xkb_keymap_serialize_config config = {
        .size = परिमाण(config),
        .flags = flags,
        .format = format,
        .layouts = 0,
    };

    संरचना xkb_keymap_serialize_result result = { .size = परिमाण(result) };

    फर्काऊ (xkb_keymap_serialize(keymap, &config, &result) == XKB_SUCCESS)
        ? result.serialized
        : रिक्त;
}

वर्ण *
xkb_keymap_get_as_string(संरचना xkb_keymap *keymap,
                         गणना xkb_keymap_format format)
{
    फर्काऊ xkb_keymap_get_as_string2(keymap, format,
                                     XKB_KEYMAP_SERIALIZE_NO_FLAGS);
}

/**
 * Returns the total number of modifiers active in the keymap.
 */
xkb_mod_index_t
xkb_keymap_अङ्क_mods(संरचना xkb_keymap *keymap)
{
    फर्काऊ keymap->mods.अङ्क_mods;
}

/**
 * Return the name for a given modifier.
 */
अचर वर्ण *
xkb_keymap_mod_get_name(संरचना xkb_keymap *keymap, xkb_mod_index_t idx)
{
    यदि (idx >= keymap->mods.अङ्क_mods)
        फर्काऊ रिक्त;

    फर्काऊ xkb_atom_text(keymap->ctx, keymap->mods.mods[idx].name);
}

/**
 * Returns the index for a named modifier.
 */
xkb_mod_index_t
xkb_keymap_mod_get_index(संरचना xkb_keymap *keymap, अचर वर्ण *name)
{
    अचर xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    फर्काऊ (atom == XKB_ATOM_NONE)
        ? XKB_MOD_INVALID
        : XkbModNameToIndex(&keymap->mods, atom, MOD_BOTH);
}

/**
 * Return the canonical mapping of a named modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask(संरचना xkb_keymap *keymap, अचर वर्ण* name)
{
    अचर xkb_mod_index_t idx = xkb_keymap_mod_get_index(keymap, name);
    फर्काऊ (idx >= keymap->mods.अङ्क_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the canonical mapping of a given modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask2(संरचना xkb_keymap *keymap, xkb_mod_index_t idx)
{
    फर्काऊ (idx >= keymap->mods.अङ्क_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the total number of active groups in the keymap.
 */
xkb_layout_index_t
xkb_keymap_अङ्क_layouts(संरचना xkb_keymap *keymap)
{
    फर्काऊ keymap->अङ्क_groups;
}

/**
 * Returns the name for a given group.
 */
अचर वर्ण *
xkb_keymap_layout_get_name(संरचना xkb_keymap *keymap, xkb_layout_index_t idx)
{
    यदि (idx >= keymap->अङ्क_group_names)
        फर्काऊ रिक्त;

    फर्काऊ xkb_atom_text(keymap->ctx, keymap->group_names[idx]);
}

/**
 * Returns the index for a named layout.
 */
xkb_layout_index_t
xkb_keymap_layout_get_index(संरचना xkb_keymap *keymap, अचर वर्ण *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_layout_index_t i;

    यदि (atom == XKB_ATOM_NONE)
        फर्काऊ XKB_LAYOUT_INVALID;

    लागि (i = 0; i < keymap->अङ्क_group_names; i++)
        यदि (keymap->group_names[i] == atom)
            फर्काऊ i;

    फर्काऊ XKB_LAYOUT_INVALID;
}

/**
 * Returns the number of layouts active for a particular key.
 */
xkb_layout_index_t
xkb_keymap_अङ्क_layouts_for_साँचो(संरचना xkb_keymap *keymap, xkb_keycode_t kc)
{
    अचर संरचना xkb_साँचो *साँचो = Xkb_साँचो(keymap, kc);

    यदि (!साँचो)
        फर्काऊ 0;

    फर्काऊ साँचो->अङ्क_groups;
}

/**
 * Returns the number of levels active for a particular key and layout.
 */
xkb_level_index_t
xkb_keymap_अङ्क_levels_for_साँचो(संरचना xkb_keymap *keymap, xkb_keycode_t kc,
                              xkb_layout_index_t layout)
{
    अचर संरचना xkb_साँचो *साँचो = Xkb_साँचो(keymap, kc);

    यदि (!साँचो)
        फर्काऊ 0;

    layout = xkb_keymap_साँचो_effective_layout(साँचो, layout);
    यदि (layout == XKB_LAYOUT_INVALID)
        फर्काऊ 0;

    फर्काऊ Xkb_साँचो_अङ्क_Levels(साँचो, layout);
}

/**
 * Return the total number of LEDs in the keymap.
 */
xkb_led_index_t
xkb_keymap_अङ्क_leds(संरचना xkb_keymap *keymap)
{
    फर्काऊ keymap->अङ्क_leds;
}

/**
 * Returns the name for a given LED.
 */
अचर वर्ण *
xkb_keymap_led_get_name(संरचना xkb_keymap *keymap, xkb_led_index_t idx)
{
    यदि (idx >= keymap->अङ्क_leds)
        फर्काऊ रिक्त;

    फर्काऊ xkb_atom_text(keymap->ctx, keymap->leds[idx].name);
}

/**
 * Returns the index for a named LED.
 */
xkb_led_index_t
xkb_keymap_led_get_index(संरचना xkb_keymap *keymap, अचर वर्ण *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_led_index_t i;
    अचर संरचना xkb_led *led;

    यदि (atom == XKB_ATOM_NONE)
        फर्काऊ XKB_LED_INVALID;

    xkb_leds_enumerate(i, led, keymap)
        यदि (led->name == atom)
            फर्काऊ i;

    फर्काऊ XKB_LED_INVALID;
}

माप_प्रकार
xkb_keymap_साँचो_get_mods_for_level(संरचना xkb_keymap *keymap,
                                  xkb_keycode_t kc,
                                  xkb_layout_index_t layout,
                                  xkb_level_index_t level,
                                  xkb_mod_mask_t *masks_out,
                                  माप_प्रकार masks_size)
{
    अचर संरचना xkb_साँचो *साँचो = Xkb_साँचो(keymap, kc);
    यदि (!साँचो)
        फर्काऊ 0;

    layout = xkb_keymap_साँचो_effective_layout(साँचो, layout);
    यदि (layout == XKB_LAYOUT_INVALID)
        फर्काऊ 0;

    यदि (level >= Xkb_साँचो_अङ्क_Levels(साँचो, layout))
        फर्काऊ 0;

    अचर संरचना xkb_साँचो_type *type = साँचो->groups[layout].type;

    माप_प्रकार गन्ती = 0;

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
    यदि (level == 0) {
        bool empty_mapped = false;
        लागि (darray_size_t i = 0; i < type->अङ्क_entries && गन्ती < masks_size; i++)
            यदि (entry_is_active(&type->entries[i]) &&
                type->entries[i].mods.mask == 0) {
                empty_mapped = true;
                रोक;
            }
        यदि (!empty_mapped && गन्ती < masks_size) {
            masks_out[गन्ती++] = 0;
        }
    }

    /* Now search explicit mappings. */
    लागि (darray_size_t i = 0; i < type->अङ्क_entries && गन्ती < masks_size; i++) {
        यदि (entry_is_active(&type->entries[i]) &&
            type->entries[i].level == level) {
            masks_out[गन्ती++] = type->entries[i].mods.mask;
        }
    }

    फर्काऊ गन्ती;
}

संरचना xkb_level *
xkb_keymap_साँचो_get_level(संरचना xkb_keymap *keymap, अचर संरचना xkb_साँचो *साँचो,
                         xkb_layout_index_t layout, xkb_level_index_t level)
{
    layout = xkb_keymap_साँचो_effective_layout(साँचो, layout);
    यदि (layout == XKB_LAYOUT_INVALID)
        फर्काऊ रिक्त;

    यदि (level >= Xkb_साँचो_अङ्क_Levels(साँचो, layout))
        फर्काऊ रिक्त;

    फर्काऊ &साँचो->groups[layout].levels[level];
}

/**
 * As below, but takes an explicit layout/level rather than state.
 */
पूर्णाङ्क
xkb_keymap_साँचो_get_syms_by_level(संरचना xkb_keymap *keymap,
                                 xkb_keycode_t kc,
                                 xkb_layout_index_t layout,
                                 xkb_level_index_t level,
                                 अचर xkb_keysym_t **syms_out)
{
    अचर संरचना xkb_साँचो *साँचो = Xkb_साँचो(keymap, kc);

    यदि (!साँचो)
        गच्छ err;

    अचर संरचना xkb_level* अचर leveli =
        xkb_keymap_साँचो_get_level(keymap, साँचो, layout, level);

    यदि (!leveli)
        गच्छ err;

    अचर xkb_keysym_गन्ती_t अङ्क_syms = leveli->अङ्क_syms;
    यदि (अङ्क_syms == 0)
        गच्छ err;

    यदि (अङ्क_syms == 1)
        *syms_out = &leveli->s.sym;
    अन्यथा
        *syms_out = leveli->s.syms;

    फर्काऊ (पूर्णाङ्क) अङ्क_syms;

err:
    *syms_out = रिक्त;
    फर्काऊ 0;
}

xkb_keycode_t
xkb_keymap_min_keycode(संरचना xkb_keymap *keymap)
{
    फर्काऊ keymap->min_साँचो_code;
}

xkb_keycode_t
xkb_keymap_max_keycode(संरचना xkb_keymap *keymap)
{
    फर्काऊ keymap->max_साँचो_code;
}

स्थिर गणना xkb_status
keymap_साँचो_iterator_config_check(
    संरचना xkb_context * restrict ctx,
    अचर वर्ण *func,
    अचर संरचना xkb_keymap_साँचो_iterator_config * restrict config
)
{
    /* Check ABI compatibility */
    अचर गणना xkb_status abi_error = xkb_check_keymap_abi(config);
    यदि (abi_error) {
        xkb_log_abi_error(ctx, func, abi_error);
        फर्काऊ abi_error;
    }

    /* Sanitize input: flags */
    अचर uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_KEYMAP_साँचो_ITERATOR_FLAGS_मानहरू);
    यदि (invalid_flags) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_साँचो_ITERATOR_FLAGS_2,
                "%s: Unsupported keymap iterator flags: 0x%"PRIx32"\n",
                func, invalid_flags);
        फर्काऊ XKB_ERROR_UNSUPPORTED_साँचो_ITERATOR_FLAGS;
    }

    /* Sanitize input: start */
    यदि (config->start > XKB_KEYCODE_MAX)
        फर्काऊ XKB_ERROR_INVALID_KEYCODE;

    फर्काऊ XKB_SUCCESS;
}

static_assert(XKB_KEYMAP_साँचो_ITERATOR_FLAGS_मानहरू <=
              (पूर्णाङ्क)XKB_POINTER_TAG_MASK,
              "Cannot tag cursor pointer with flags");

#define iter_init_keymap(i, ptr, flags) {          \
    (i)->opaque0 = xkb_pointer_tag((ptr), (flags));\
}
#define iter_get_keymap(i) (                                  \
    (अचर संरचना xkb_keymap *)xkb_pointer_untag((i)->opaque0)\
)
#define iter_get_next(i) ((अचर संरचना xkb_साँचो *)(i)->opaque1)
#define iter_set_next(i, ptr) {(i)->opaque1 = (अचर शून्य *)(ptr);}
#define iter_get_descending_order(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                 \
    (uintptr_t)XKB_KEYMAP_साँचो_ITERATOR_DESCENDING_ORDER \
)
#define iter_get_include_unbound(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                \
    (uintptr_t)XKB_KEYMAP_साँचो_ITERATOR_INCLUDE_UNBOUND \
)

गणना xkb_status
xkb_keymap_साँचो_iterator_init(
    संरचना xkb_keymap_साँचो_iterator * restrict iter,
    अचर संरचना xkb_keymap * restrict keymap,
    अचर संरचना xkb_keymap_साँचो_iterator_config * restrict config
)
{
    /* Handle default configuration */
    स्थिर अचर संरचना xkb_keymap_साँचो_iterator_config default_config = {
        .size = परिमाण(default_config)
    };
    यदि (!config)
        config = &default_config;

    /* Check input */
    अचर गणना xkb_status status =
        keymap_साँचो_iterator_config_check(keymap->ctx, __func__, config);
    यदि (status != XKB_SUCCESS) {
        फर्काऊ status;
    }

    /*
     * Initialization
     */

    स_रखो(iter, 0, परिमाण(*iter));

    /* Borrow keymap */
    iter_init_keymap(iter, keymap, config->flags);

    यदि (keymap->अङ्क_साँचोहरू == 0) {
        iter_set_next(iter, रिक्त);
        फर्काऊ XKB_SUCCESS;
    }

    /* Borrow key */
    अचर संरचना xkb_साँचो *next;
    यदि (config->flags & XKB_KEYMAP_साँचो_ITERATOR_DESCENDING_ORDER) {
        यदि (!config->start || config->start > keymap->max_साँचो_code) {
            next = &keymap->साँचोहरू[keymap->अङ्क_साँचोहरू - 1];
        } अन्यथा {
            /*
             * May still fail *safely* if start < keymap->min_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_साँचो(keymap, false, config->start);
        }
    } अन्यथा {
        यदि (!config->start || config->start < keymap->min_साँचो_code) {
            next = (keymap->अङ्क_साँचोहरू_तल्लो)
                ? &keymap->साँचोहरू[keymap->min_साँचो_code]
                : &keymap->साँचोहरू[0];
        } अन्यथा {
            /*
             * May still fail *safely* if start > keymap->max_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_साँचो(keymap, true, config->start);
        }
    };
    iter_set_next(iter, next);

    फर्काऊ XKB_SUCCESS;
}

xkb_keycode_t
xkb_keymap_साँचो_iterator_next(संरचना xkb_keymap_साँचो_iterator *iter)
{
    अचर संरचना xkb_साँचो *next = iter_get_next(iter);
    यदि (!next)
        फर्काऊ XKB_KEYCODE_INVALID;

    अचर संरचना xkb_keymap *keymap = iter_get_keymap(iter);
    प्रतिज्ञा(keymap->अङ्क_साँचोहरू);

    अचर bool descending = iter_get_descending_order(iter);
    अचर bool skip_unbound = !iter_get_include_unbound(iter);
    अचर संरचना xkb_साँचो *last = descending
        ? (keymap->अङ्क_साँचोहरू_तल्लो)
            ? &keymap->साँचोहरू[keymap->min_साँचो_code]
            : &keymap->साँचोहरू[0]
        : &keymap->साँचोहरू[keymap->अङ्क_साँचोहरू - 1];
    अचर सूचक_भेद_प्रकार step = descending ? -1 : 1;

    /* Skip undefined keys (no name) and optionally unbound keys */
    लागि (;;) {
        अचर bool iter_end = descending ? (next <= last) : (next >= last);
        अचर bool skip = (
            next->name == XKB_ATOM_NONE ||
            (skip_unbound && (next->अङ्क_groups == 0))
        );
        यदि (!skip) {
            अचर xkb_keycode_t ret = next->keycode;
            iter_set_next(iter, iter_end ? रिक्त : next + step);
            फर्काऊ ret;
        }
        यदि (iter_end) {
            iter_set_next(iter, रिक्त);
            फर्काऊ XKB_KEYCODE_INVALID;
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

शून्य
xkb_keymap_साँचो_for_each(संरचना xkb_keymap *keymap, xkb_keymap_साँचो_iter_t iter,
                        शून्य *data)
{
    संरचना xkb_साँचो *साँचो;

    xkb_साँचोहरू_foreach(साँचो, keymap)
        iter(keymap, साँचो->keycode, data);
}

अचर वर्ण *
xkb_keymap_साँचो_get_name(संरचना xkb_keymap *keymap, xkb_keycode_t kc)
{
    अचर संरचना xkb_साँचो *साँचो = Xkb_साँचो(keymap, kc);

    यदि (!साँचो)
        फर्काऊ रिक्त;

    फर्काऊ xkb_atom_text(keymap->ctx, साँचो->name);
}

xkb_keycode_t
xkb_keymap_साँचो_by_name(संरचना xkb_keymap *keymap, अचर वर्ण *name)
{
    संरचना xkb_साँचो *साँचो;
    xkb_atom_t atom;

    atom = xkb_atom_lookup(keymap->ctx, name);
    यदि (atom) {
        लागि (darray_size_t i = 0; i < keymap->अङ्क_साँचो_aliases; i++)
            यदि (keymap->साँचो_aliases[i].alias == atom)
                atom = keymap->साँचो_aliases[i].real;
    }
    यदि (!atom)
        फर्काऊ XKB_KEYCODE_INVALID;

    xkb_साँचोहरू_foreach(साँचो, keymap) {
        यदि (साँचो->name == atom)
            फर्काऊ साँचो->keycode;
    }

    फर्काऊ XKB_KEYCODE_INVALID;
}

/**
 * Simple boolean specifying whether or not the key should repeat.
 */
पूर्णाङ्क
xkb_keymap_साँचो_repeats(संरचना xkb_keymap *keymap, xkb_keycode_t kc)
{
    अचर संरचना xkb_साँचो *साँचो = Xkb_साँचो(keymap, kc);

    यदि (!साँचो)
        फर्काऊ 0;

    फर्काऊ साँचो->repeats;
}
