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
    प्रत्यागम keymap;
}

शून्य
clear_level(संरचना xkb_level *leveli)
{
    यदि (leveli->अङ्कः_syms > 1)
        मुक्त(leveli->s.syms);
    यदि (leveli->अङ्कः_actions > 1)
        मुक्त(leveli->a.actions);
}

स्थिर शून्य
clear_interpret(संरचना xkb_sym_interpret *interp)
{
    यदि (interp->अङ्कः_actions > 1)
        मुक्त(interp->a.actions);
}

शून्य
xkb_keymap_unref(संरचना xkb_keymap *keymap)
{
    प्रतिज्ञा(!keymap || keymap->refcnt > 0);
    यदि (!keymap || --keymap->refcnt > 0)
        प्रत्यागम;

    यदि (keymap->कुञ्चिकाः) {
        संरचना xkb_कुञ्चिका *कुञ्चिका;
        xkb_कुञ्चिकाः_foreach(कुञ्चिका, keymap) {
            यदि (कुञ्चिका->groups) {
                प्रति (xkb_layout_सूचकांक_t i = 0; i < कुञ्चिका->अङ्कः_groups; i++) {
                    यदि (कुञ्चिका->groups[i].स्तराः) {
                        प्रति (xkb_level_सूचकांक_t j = 0;
                             j < Xkb_कुञ्चिका_अङ्कः_स्तराः(कुञ्चिका, i);
                             j++)
                            clear_level(&कुञ्चिका->groups[i].स्तराः[j]);
                        मुक्त(कुञ्चिका->groups[i].स्तराः);
                    }
                }
                मुक्त(कुञ्चिका->groups);
            }
            यदि (!कुञ्चिका->overlays_inline && कुञ्चिका->overlays_कुञ्चिकाः) {
                मुक्त((शून्य *)कुञ्चिका->overlays_कुञ्चिकाः);
            }
        }
        मुक्त(keymap->कुञ्चिकाः);
    }
    यदि (keymap->types) {
        प्रति (darray_आकार_t i = 0; i < keymap->अङ्कः_types; i++) {
            मुक्त(keymap->types[i].entries);
            मुक्त(keymap->types[i].level_नामानि);
        }
        मुक्त(keymap->types);
    }
    प्रति (darray_आकार_t k = 0; k < keymap->अङ्कः_sym_interprets; k++) {
        clear_interpret(&keymap->sym_interprets[k]);
    }
    मुक्त(keymap->sym_interprets);
    मुक्त(keymap->modmaps);
    मुक्त(keymap->कुञ्चिका_aliases);
    मुक्त(keymap->गणः_नामानि);
    मुक्त(keymap->keycodes_section_name);
    मुक्त(keymap->symbols_section_name);
    मुक्त(keymap->types_section_name);
    मुक्त(keymap->compat_section_name);
    xkb_context_unref(keymap->ctx);
    मुक्त(keymap);
}

स्थिर ध्रुव संरचना xkb_keymap_format_ops *
get_keymap_format_ops(गणना xkb_keymap_format format)
{
    स्थिर ध्रुव संरचना xkb_keymap_format_ops *keymap_format_ops[] = {
        [XKB_KEYMAP_FORMAT_TEXT_V1] = &text_v1_keymap_format_ops,
        [XKB_KEYMAP_FORMAT_TEXT_V2] = &text_v1_keymap_format_ops,
    };

    यदि ((पूर्णाङ्क) format < 0 || (पूर्णाङ्क) format >= (पूर्णाङ्क) ARRAY_आकार(keymap_format_ops))
        प्रत्यागम रिक्त;

    प्रत्यागम keymap_format_ops[(पूर्णाङ्क) format];
}

संरचना xkb_keymap *
xkb_keymap_new_from_rmlvo(ध्रुव संरचना xkb_rmlvo_builder *rmlvo,
                          गणना xkb_keymap_format format,
                          गणना xkb_keymap_compile_flags flags)
{
    ध्रुव संरचना xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    यदि (!ops || !ops->keymap_new_from_rmlvo) {
        log_err_func(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        प्रत्यागम रिक्त;
    }

    संरचना xkb_keymap *keymap = xkb_keymap_new(rmlvo->ctx, __func__, format,
                                               flags);
    यदि (!keymap)
        प्रत्यागम रिक्त;

    यदि (!ops->keymap_new_from_rmlvo(keymap, rmlvo)) {
        xkb_keymap_unref(keymap);
        प्रत्यागम रिक्त;
    }

    प्रत्यागम keymap;
}

संरचना xkb_keymap *
xkb_keymap_new_from_names2(संरचना xkb_context *ctx,
                           ध्रुव संरचना xkb_नियमः_नामानि *rmlvo_अन्तर्,
                           गणना xkb_keymap_format format,
                           गणना xkb_keymap_compile_flags flags)
{
    ध्रुव संरचना xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    यदि (!ops || !ops->keymap_new_from_नामानि) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        प्रत्यागम रिक्त;
    }

    संरचना xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    यदि (!keymap)
        प्रत्यागम रिक्त;

    संरचना xkb_नियमः_नामानि rmlvo = {0};
    यदि (rmlvo_अन्तर्)
        rmlvo = *rmlvo_अन्तर्;
    xkb_context_sanitize_नियमः_नामानि(ctx, &rmlvo);

    यदि (!ops->keymap_new_from_नामानि(keymap, &rmlvo)) {
        xkb_keymap_unref(keymap);
        प्रत्यागम रिक्त;
    }

    प्रत्यागम keymap;
}


संरचना xkb_keymap *
xkb_keymap_new_from_नामानि(संरचना xkb_context *ctx,
                          ध्रुव संरचना xkb_नियमः_नामानि *rmlvo_अन्तर्,
                          गणना xkb_keymap_compile_flags flags)
{
    प्रत्यागम xkb_keymap_new_from_names2(ctx, rmlvo_अन्तर्,
                                      XKB_KEYMAP_FORMAT_TEXT_V2, flags);
}

संरचना xkb_keymap *
xkb_keymap_new_from_string(संरचना xkb_context *ctx,
                           ध्रुव वर्ण *string,
                           गणना xkb_keymap_format format,
                           गणना xkb_keymap_compile_flags flags)
{
    प्रत्यागम xkb_keymap_new_from_बफर(ctx, string, वाक्यदैर्घ्यम्(string),
                                      format, flags);
}

संरचना xkb_keymap *
xkb_keymap_new_from_बफर(संरचना xkb_context *ctx,
                           ध्रुव वर्ण *बफर, आकार_t दीर्घता,
                           गणना xkb_keymap_format format,
                           गणना xkb_keymap_compile_flags flags)
{
    ध्रुव संरचना xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    यदि (!ops || !ops->keymap_new_from_string) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        प्रत्यागम रिक्त;
    }

    यदि (!बफर) {
        log_err_func1(ctx, XKB_LOG_सन्देशः_NO_ID,
                      "no buffer specified\n");
        प्रत्यागम रिक्त;
    }

    संरचना xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    यदि (!keymap)
        प्रत्यागम रिक्त;

    /* Allow a zero-terminated string as a buffer */
    यदि (दीर्घता > 0 && बफर[दीर्घता - 1] == '\0')
        दीर्घता--;

    यदि (!ops->keymap_new_from_string(keymap, बफर, दीर्घता)) {
        xkb_keymap_unref(keymap);
        प्रत्यागम रिक्त;
    }

    प्रत्यागम keymap;
}

संरचना xkb_keymap *
xkb_keymap_new_from_file(संरचना xkb_context *ctx,
                         FILE *file,
                         गणना xkb_keymap_format format,
                         गणना xkb_keymap_compile_flags flags)
{
    ध्रुव संरचना xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    यदि (!ops || !ops->keymap_new_from_file) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        प्रत्यागम रिक्त;
    }

    यदि (!file) {
        log_err_func1(ctx, XKB_LOG_सन्देशः_NO_ID,
                      "no file specified\n");
        प्रत्यागम रिक्त;
    }

    संरचना xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    यदि (!keymap)
        प्रत्यागम रिक्त;

    यदि (!ops->keymap_new_from_file(keymap, file)) {
        xkb_keymap_unref(keymap);
        प्रत्यागम रिक्त;
    }

    प्रत्यागम keymap;
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
स्थिर गणना xkb_स्थितिः
check_keymap_serialize_abi(
    संरचना xkb_context * सीमित ctx,
    ध्रुव वर्ण * सीमित func,
    ध्रुव संरचना xkb_keymap_serialize_config * सीमित config,
    ध्रुव संरचना xkb_keymap_serialize_परिणाम * सीमित परिणाम
)
{
    गणना xkb_स्थितिः स्थितिः;
    यदि ((स्थितिः = xkb_check_keymap_abi(config)) ||
        (स्थितिः = xkb_check_keymap_abi(परिणाम))) {
        xkb_log_abi_error(ctx, func, स्थितिः);
    }
    प्रत्यागम स्थितिः;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

गणना xkb_स्थितिः
xkb_keymap_serialize(ध्रुव संरचना xkb_keymap *keymap,
                     ध्रुव संरचना xkb_keymap_serialize_config *config,
                     संरचना xkb_keymap_serialize_परिणाम *परिणाम)
{
    /* Check ABI compatibility */
    ध्रुव गणना xkb_स्थितिः स्थितिः =
        check_keymap_serialize_abi(keymap->ctx, __func__, config, परिणाम);
    यदि (स्थितिः != XKB_SUCCESS)
        प्रत्यागम स्थितिः;

    संरचना xkb_keymap_serialize_config new_config = *config;

    ध्रुव गणना xkb_keymap_serialize_flags invalid_flags = (
        new_config.flags &
        ~(गणना xkb_keymap_serialize_flags)XKB_KEYMAP_SERIALIZE_FLAGS_मानानि
    );
    यदि (invalid_flags) {
        log_err_func(keymap->ctx,
                     XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS_,
                     "unrecognized serialization flags: %#x\n",
                     invalid_flags);
        प्रत्यागम XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS;
    }

    यदि (new_config.format == XKB_KEYMAP_USE_ORIGINAL_FORMAT)
        new_config.format = keymap->format;

    ध्रुव संरचना xkb_keymap_format_ops * ध्रुव ops =
        get_keymap_format_ops(new_config.format);
    यदि (!ops || !ops->keymap_serialize) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", new_config.format);
        प्रत्यागम XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT;
    }

    ध्रुव xkb_layout_आवरणम्_t all_layouts =
        (xkb_layout_आवरणम्_t)((UINT64_C(1) << keymap->अङ्कः_groups) - 1);
    यदि (!new_config.layouts) {
        new_config.layouts = all_layouts;
    }

    यदि (new_config.layouts & ~all_layouts) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                     "unsupported layout mask: "
                     "expected subset of 0x%08"PRIx32", got: 0x%08"PRIx32"\n",
                     all_layouts, new_config.layouts);
        प्रत्यागम XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक;
    }

    ध्रुव xkb_layout_सूचकांक_t max_groups = format_max_groups(config->format);
    ध्रुव xkb_layout_सूचकांक_t अङ्कः_groups =
        (xkb_layout_सूचकांक_t)popcount32(new_config.layouts);

    यदि (अङ्कः_groups > max_groups) {
        ध्रुव bool strict =
            (new_config.flags & XKB_KEYMAP_SERIALIZE_STRICT_बहुलकम्);
        xkb_log_with_संहिता(
            keymap->ctx,
            (strict ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
            XKB_LOG_VERBOSITY_अल्पिष्ठम्,
            XKB_ERROR_LAYOUT_गणनम्_सीमा_EXCEEDED_2,
            "Cannot serialize %"PRIu32" groups in keymap format %d: "
            "maximum is %"PRIu32"%s\n",
            अङ्कः_groups, config->format, max_groups,
            (strict ? "" : "; discarding unsupported groups")
        );
        यदि (strict) {
            प्रत्यागम XKB_ERROR_LAYOUT_गणनम्_सीमा_EXCEEDED;
        } अन्यथा {
            new_config.layouts =
                keep_निम्नतमम्_n_समुच्चयः_द्वयङ्काः(new_config.layouts, max_groups);
        }
    }

    log_dbg(keymap->ctx, XKB_LOG_सन्देशः_NO_ID,
            "Serializing group mask 0x%"PRIx32" (total: %"PRIu32")\n",
            new_config.layouts, अङ्कः_groups);

    यदि (new_config.layouts != all_layouts) {
        /*
         * If some layouts are discarded, then interprets may not trigger
         * properly. Avoid this by requiring explicit values.
         */
        new_config.flags |= (XKB_KEYMAP_SERIALIZE_EXPLICIT_कुञ्चिका_मानानि |
                             XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS);
    }

    प्रत्यागम ops->keymap_serialize(keymap, &new_config, परिणाम);
}

वर्ण *
xkb_keymap_get_as_string2(संरचना xkb_keymap *keymap,
                          गणना xkb_keymap_format format,
                          गणना xkb_keymap_serialize_flags flags)
{
    ध्रुव संरचना xkb_keymap_serialize_config config = {
        .आकार = परिमाण(config),
        .flags = flags,
        .format = format,
        .layouts = 0,
    };

    संरचना xkb_keymap_serialize_परिणाम परिणाम = { .आकार = परिमाण(परिणाम) };

    प्रत्यागम (xkb_keymap_serialize(keymap, &config, &परिणाम) == XKB_SUCCESS)
        ? परिणाम.serialized
        : रिक्त;
}

वर्ण *
xkb_keymap_get_as_string(संरचना xkb_keymap *keymap,
                         गणना xkb_keymap_format format)
{
    प्रत्यागम xkb_keymap_get_as_string2(keymap, format,
                                     XKB_KEYMAP_SERIALIZE_NO_FLAGS);
}

/**
 * Returns the total number of modifiers active in the keymap.
 */
xkb_mod_सूचकांक_t
xkb_keymap_अङ्कः_mods(संरचना xkb_keymap *keymap)
{
    प्रत्यागम keymap->mods.अङ्कः_mods;
}

/**
 * Return the name for a given modifier.
 */
ध्रुव वर्ण *
xkb_keymap_mod_get_name(संरचना xkb_keymap *keymap, xkb_mod_सूचकांक_t idx)
{
    यदि (idx >= keymap->mods.अङ्कः_mods)
        प्रत्यागम रिक्त;

    प्रत्यागम xkb_atom_text(keymap->ctx, keymap->mods.mods[idx].name);
}

/**
 * Returns the index for a named modifier.
 */
xkb_mod_सूचकांक_t
xkb_keymap_mod_get_सूचकांक(संरचना xkb_keymap *keymap, ध्रुव वर्ण *name)
{
    ध्रुव xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    प्रत्यागम (atom == XKB_ATOM_NONE)
        ? XKB_MOD_INVALID
        : Xkb_Mod_Name_गन्तव्यम्_सूचकांक(&keymap->mods, atom, MOD_BOTH);
}

/**
 * Return the canonical mapping of a named modifier.
 */
xkb_mod_आवरणम्_t
xkb_keymap_mod_get_आवरणम्(संरचना xkb_keymap *keymap, ध्रुव वर्ण* name)
{
    ध्रुव xkb_mod_सूचकांक_t idx = xkb_keymap_mod_get_सूचकांक(keymap, name);
    प्रत्यागम (idx >= keymap->mods.अङ्कः_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the canonical mapping of a given modifier.
 */
xkb_mod_आवरणम्_t
xkb_keymap_mod_get_mask2(संरचना xkb_keymap *keymap, xkb_mod_सूचकांक_t idx)
{
    प्रत्यागम (idx >= keymap->mods.अङ्कः_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the total number of active groups in the keymap.
 */
xkb_layout_सूचकांक_t
xkb_keymap_अङ्कः_layouts(संरचना xkb_keymap *keymap)
{
    प्रत्यागम keymap->अङ्कः_groups;
}

/**
 * Returns the name for a given group.
 */
ध्रुव वर्ण *
xkb_keymap_layout_get_name(संरचना xkb_keymap *keymap, xkb_layout_सूचकांक_t idx)
{
    यदि (idx >= keymap->अङ्कः_गणः_नामानि)
        प्रत्यागम रिक्त;

    प्रत्यागम xkb_atom_text(keymap->ctx, keymap->गणः_नामानि[idx]);
}

/**
 * Returns the index for a named layout.
 */
xkb_layout_सूचकांक_t
xkb_keymap_layout_get_सूचकांक(संरचना xkb_keymap *keymap, ध्रुव वर्ण *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_layout_सूचकांक_t i;

    यदि (atom == XKB_ATOM_NONE)
        प्रत्यागम XKB_LAYOUT_INVALID;

    प्रति (i = 0; i < keymap->अङ्कः_गणः_नामानि; i++)
        यदि (keymap->गणः_नामानि[i] == atom)
            प्रत्यागम i;

    प्रत्यागम XKB_LAYOUT_INVALID;
}

/**
 * Returns the number of layouts active for a particular key.
 */
xkb_layout_सूचकांक_t
xkb_keymap_अङ्कः_layouts_for_कुञ्चिका(संरचना xkb_keymap *keymap, xkb_keycode_t kc)
{
    ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका = Xkb_कुञ्चिका(keymap, kc);

    यदि (!कुञ्चिका)
        प्रत्यागम 0;

    प्रत्यागम कुञ्चिका->अङ्कः_groups;
}

/**
 * Returns the number of levels active for a particular key and layout.
 */
xkb_level_सूचकांक_t
xkb_keymap_अङ्कः_स्तराः_for_कुञ्चिका(संरचना xkb_keymap *keymap, xkb_keycode_t kc,
                              xkb_layout_सूचकांक_t layout)
{
    ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका = Xkb_कुञ्चिका(keymap, kc);

    यदि (!कुञ्चिका)
        प्रत्यागम 0;

    layout = xkb_keymap_कुञ्चिका_effective_layout(कुञ्चिका, layout);
    यदि (layout == XKB_LAYOUT_INVALID)
        प्रत्यागम 0;

    प्रत्यागम Xkb_कुञ्चिका_अङ्कः_स्तराः(कुञ्चिका, layout);
}

/**
 * Return the total number of LEDs in the keymap.
 */
xkb_led_सूचकांक_t
xkb_keymap_अङ्कः_leds(संरचना xkb_keymap *keymap)
{
    प्रत्यागम keymap->अङ्कः_leds;
}

/**
 * Returns the name for a given LED.
 */
ध्रुव वर्ण *
xkb_keymap_led_get_name(संरचना xkb_keymap *keymap, xkb_led_सूचकांक_t idx)
{
    यदि (idx >= keymap->अङ्कः_leds)
        प्रत्यागम रिक्त;

    प्रत्यागम xkb_atom_text(keymap->ctx, keymap->leds[idx].name);
}

/**
 * Returns the index for a named LED.
 */
xkb_led_सूचकांक_t
xkb_keymap_led_get_सूचकांक(संरचना xkb_keymap *keymap, ध्रुव वर्ण *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_led_सूचकांक_t i;
    ध्रुव संरचना xkb_led *led;

    यदि (atom == XKB_ATOM_NONE)
        प्रत्यागम XKB_LED_INVALID;

    xkb_leds_enumerate(i, led, keymap)
        यदि (led->name == atom)
            प्रत्यागम i;

    प्रत्यागम XKB_LED_INVALID;
}

आकार_t
xkb_keymap_कुञ्चिका_get_mods_for_level(संरचना xkb_keymap *keymap,
                                  xkb_keycode_t kc,
                                  xkb_layout_सूचकांक_t layout,
                                  xkb_level_सूचकांक_t level,
                                  xkb_mod_आवरणम्_t *masks_बहिर्गमनम्,
                                  आकार_t masks_आकार)
{
    ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका = Xkb_कुञ्चिका(keymap, kc);
    यदि (!कुञ्चिका)
        प्रत्यागम 0;

    layout = xkb_keymap_कुञ्चिका_effective_layout(कुञ्चिका, layout);
    यदि (layout == XKB_LAYOUT_INVALID)
        प्रत्यागम 0;

    यदि (level >= Xkb_कुञ्चिका_अङ्कः_स्तराः(कुञ्चिका, layout))
        प्रत्यागम 0;

    ध्रुव संरचना xkb_कुञ्चिका_type *type = कुञ्चिका->groups[layout].type;

    आकार_t गणनम् = 0;

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
        bool रिक्तानि_mapped = false;
        प्रति (darray_आकार_t i = 0; i < type->अङ्कः_entries && गणनम् < masks_आकार; i++)
            यदि (प्रविष्टिः_is_सक्रिय(&type->entries[i]) &&
                type->entries[i].mods.आवरणम् == 0) {
                रिक्तानि_mapped = true;
                भङ्ग;
            }
        यदि (!रिक्तानि_mapped && गणनम् < masks_आकार) {
            masks_बहिर्गमनम्[गणनम्++] = 0;
        }
    }

    /* Now search explicit mappings. */
    प्रति (darray_आकार_t i = 0; i < type->अङ्कः_entries && गणनम् < masks_आकार; i++) {
        यदि (प्रविष्टिः_is_सक्रिय(&type->entries[i]) &&
            type->entries[i].level == level) {
            masks_बहिर्गमनम्[गणनम्++] = type->entries[i].mods.आवरणम्;
        }
    }

    प्रत्यागम गणनम्;
}

संरचना xkb_level *
xkb_keymap_कुञ्चिका_get_level(संरचना xkb_keymap *keymap, ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                         xkb_layout_सूचकांक_t layout, xkb_level_सूचकांक_t level)
{
    layout = xkb_keymap_कुञ्चिका_effective_layout(कुञ्चिका, layout);
    यदि (layout == XKB_LAYOUT_INVALID)
        प्रत्यागम रिक्त;

    यदि (level >= Xkb_कुञ्चिका_अङ्कः_स्तराः(कुञ्चिका, layout))
        प्रत्यागम रिक्त;

    प्रत्यागम &कुञ्चिका->groups[layout].स्तराः[level];
}

/**
 * As below, but takes an explicit layout/level rather than state.
 */
पूर्णाङ्क
xkb_keymap_कुञ्चिका_get_syms_by_level(संरचना xkb_keymap *keymap,
                                 xkb_keycode_t kc,
                                 xkb_layout_सूचकांक_t layout,
                                 xkb_level_सूचकांक_t level,
                                 ध्रुव xkb_keysym_t **syms_बहिर्गमनम्)
{
    ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका = Xkb_कुञ्चिका(keymap, kc);

    यदि (!कुञ्चिका)
        गच्छ err;

    ध्रुव संरचना xkb_level* ध्रुव leveli =
        xkb_keymap_कुञ्चिका_get_level(keymap, कुञ्चिका, layout, level);

    यदि (!leveli)
        गच्छ err;

    ध्रुव xkb_keysym_गणनम्_t अङ्कः_syms = leveli->अङ्कः_syms;
    यदि (अङ्कः_syms == 0)
        गच्छ err;

    यदि (अङ्कः_syms == 1)
        *syms_बहिर्गमनम् = &leveli->s.sym;
    अन्यथा
        *syms_बहिर्गमनम् = leveli->s.syms;

    प्रत्यागम (पूर्णाङ्क) अङ्कः_syms;

err:
    *syms_बहिर्गमनम् = रिक्त;
    प्रत्यागम 0;
}

xkb_keycode_t
xkb_keymap_min_keycode(संरचना xkb_keymap *keymap)
{
    प्रत्यागम keymap->min_कुञ्चिका_संहिता;
}

xkb_keycode_t
xkb_keymap_max_keycode(संरचना xkb_keymap *keymap)
{
    प्रत्यागम keymap->max_कुञ्चिका_संहिता;
}

स्थिर गणना xkb_स्थितिः
keymap_कुञ्चिका_iterator_config_check(
    संरचना xkb_context * सीमित ctx,
    ध्रुव वर्ण *func,
    ध्रुव संरचना xkb_keymap_कुञ्चिका_iterator_config * सीमित config
)
{
    /* Check ABI compatibility */
    ध्रुव गणना xkb_स्थितिः abi_error = xkb_check_keymap_abi(config);
    यदि (abi_error) {
        xkb_log_abi_error(ctx, func, abi_error);
        प्रत्यागम abi_error;
    }

    /* Sanitize input: flags */
    ध्रुव uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_KEYMAP_कुञ्चिका_ITERATOR_FLAGS_मानानि);
    यदि (invalid_flags) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_कुञ्चिका_ITERATOR_FLAGS_2,
                "%s: Unsupported keymap iterator flags: 0x%"PRIx32"\n",
                func, invalid_flags);
        प्रत्यागम XKB_ERROR_UNSUPPORTED_कुञ्चिका_ITERATOR_FLAGS;
    }

    /* Sanitize input: start */
    यदि (config->start > XKB_KEYCODE_MAX)
        प्रत्यागम XKB_ERROR_INVALID_KEYCODE;

    प्रत्यागम XKB_SUCCESS;
}

static_assert(XKB_KEYMAP_कुञ्चिका_ITERATOR_FLAGS_मानानि <=
              (पूर्णाङ्क)XKB_POINTER_चिह्नकम्_आवरणम्,
              "Cannot tag cursor pointer with flags");

#परिभाषय iter_init_keymap(i, ptr, flags) {          \
    (i)->opaque0 = xkb_pointer_चिह्नकम्((ptr), (flags));\
}
#परिभाषय iter_get_keymap(i) (                                  \
    (ध्रुव संरचना xkb_keymap *)xkb_pointer_untag((i)->opaque0)\
)
#परिभाषय iter_get_अग्र(i) ((ध्रुव संरचना xkb_कुञ्चिका *)(i)->opaque1)
#परिभाषय iter_समुच्चयः_अग्र(i, ptr) {(i)->opaque1 = (ध्रुव शून्य *)(ptr);}
#परिभाषय iter_get_descending_क्रमः(i) (                  \
    xkb_pointer_get_चिह्नकम्((i)->opaque0) &                 \
    (uintptr_t)XKB_KEYMAP_कुञ्चिका_ITERATOR_DESCENDING_क्रमः \
)
#परिभाषय iter_get_include_unbound(i) (                  \
    xkb_pointer_get_चिह्नकम्((i)->opaque0) &                \
    (uintptr_t)XKB_KEYMAP_कुञ्चिका_ITERATOR_INCLUDE_UNBOUND \
)

गणना xkb_स्थितिः
xkb_keymap_कुञ्चिका_iterator_init(
    संरचना xkb_keymap_कुञ्चिका_iterator * सीमित iter,
    ध्रुव संरचना xkb_keymap * सीमित keymap,
    ध्रुव संरचना xkb_keymap_कुञ्चिका_iterator_config * सीमित config
)
{
    /* Handle default configuration */
    स्थिर ध्रुव संरचना xkb_keymap_कुञ्चिका_iterator_config default_config = {
        .आकार = परिमाण(default_config)
    };
    यदि (!config)
        config = &default_config;

    /* Check input */
    ध्रुव गणना xkb_स्थितिः स्थितिः =
        keymap_कुञ्चिका_iterator_config_check(keymap->ctx, __func__, config);
    यदि (स्थितिः != XKB_SUCCESS) {
        प्रत्यागम स्थितिः;
    }

    /*
     * Initialization
     */

    स्मृतिं_पूरय(iter, 0, परिमाण(*iter));

    /* Borrow keymap */
    iter_init_keymap(iter, keymap, config->flags);

    यदि (keymap->अङ्कः_कुञ्चिकाः == 0) {
        iter_समुच्चयः_अग्र(iter, रिक्त);
        प्रत्यागम XKB_SUCCESS;
    }

    /* Borrow key */
    ध्रुव संरचना xkb_कुञ्चिका *अग्र;
    यदि (config->flags & XKB_KEYMAP_कुञ्चिका_ITERATOR_DESCENDING_क्रमः) {
        यदि (!config->start || config->start > keymap->max_कुञ्चिका_संहिता) {
            अग्र = &keymap->कुञ्चिकाः[keymap->अङ्कः_कुञ्चिकाः - 1];
        } अन्यथा {
            /*
             * May still fail *safely* if start < keymap->min_key_code:
             * there will be simply no key to iterate over.
             */
            अग्र = xkb_keymap_get_अग्र_defined_कुञ्चिका(keymap, false, config->start);
        }
    } अन्यथा {
        यदि (!config->start || config->start < keymap->min_कुञ्चिका_संहिता) {
            अग्र = (keymap->अङ्कः_कुञ्चिकाः_नीचः)
                ? &keymap->कुञ्चिकाः[keymap->min_कुञ्चिका_संहिता]
                : &keymap->कुञ्चिकाः[0];
        } अन्यथा {
            /*
             * May still fail *safely* if start > keymap->max_key_code:
             * there will be simply no key to iterate over.
             */
            अग्र = xkb_keymap_get_अग्र_defined_कुञ्चिका(keymap, true, config->start);
        }
    };
    iter_समुच्चयः_अग्र(iter, अग्र);

    प्रत्यागम XKB_SUCCESS;
}

xkb_keycode_t
xkb_keymap_कुञ्चिका_iterator_अग्र(संरचना xkb_keymap_कुञ्चिका_iterator *iter)
{
    ध्रुव संरचना xkb_कुञ्चिका *अग्र = iter_get_अग्र(iter);
    यदि (!अग्र)
        प्रत्यागम XKB_KEYCODE_INVALID;

    ध्रुव संरचना xkb_keymap *keymap = iter_get_keymap(iter);
    प्रतिज्ञा(keymap->अङ्कः_कुञ्चिकाः);

    ध्रुव bool descending = iter_get_descending_क्रमः(iter);
    ध्रुव bool skip_unbound = !iter_get_include_unbound(iter);
    ध्रुव संरचना xkb_कुञ्चिका *चरमः = descending
        ? (keymap->अङ्कः_कुञ्चिकाः_नीचः)
            ? &keymap->कुञ्चिकाः[keymap->min_कुञ्चिका_संहिता]
            : &keymap->कुञ्चिकाः[0]
        : &keymap->कुञ्चिकाः[keymap->अङ्कः_कुञ्चिकाः - 1];
    ध्रुव ptrdiff_t चरणः = descending ? -1 : 1;

    /* Skip undefined keys (no name) and optionally unbound keys */
    प्रति (;;) {
        ध्रुव bool iter_अन्तः = descending ? (अग्र <= चरमः) : (अग्र >= चरमः);
        ध्रुव bool skip = (
            अग्र->name == XKB_ATOM_NONE ||
            (skip_unbound && (अग्र->अङ्कः_groups == 0))
        );
        यदि (!skip) {
            ध्रुव xkb_keycode_t ret = अग्र->keycode;
            iter_समुच्चयः_अग्र(iter, iter_अन्तः ? रिक्त : अग्र + चरणः);
            प्रत्यागम ret;
        }
        यदि (iter_अन्तः) {
            iter_समुच्चयः_अग्र(iter, रिक्त);
            प्रत्यागम XKB_KEYCODE_INVALID;
        }
        अग्र += चरणः;
    }
}

#अपरिभाषय iter_init_keymap
#अपरिभाषय iter_get_keymap
#अपरिभाषय iter_समुच्चयः_अग्र
#अपरिभाषय iter_get_अग्र
#अपरिभाषय iter_get_descending_क्रमः
#अपरिभाषय iter_get_include_unbound

शून्य
xkb_keymap_कुञ्चिका_for_each(संरचना xkb_keymap *keymap, xkb_keymap_कुञ्चिका_iter_t iter,
                        शून्य *दत्त)
{
    संरचना xkb_कुञ्चिका *कुञ्चिका;

    xkb_कुञ्चिकाः_foreach(कुञ्चिका, keymap)
        iter(keymap, कुञ्चिका->keycode, दत्त);
}

ध्रुव वर्ण *
xkb_keymap_कुञ्चिका_get_name(संरचना xkb_keymap *keymap, xkb_keycode_t kc)
{
    ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका = Xkb_कुञ्चिका(keymap, kc);

    यदि (!कुञ्चिका)
        प्रत्यागम रिक्त;

    प्रत्यागम xkb_atom_text(keymap->ctx, कुञ्चिका->name);
}

xkb_keycode_t
xkb_keymap_कुञ्चिका_by_name(संरचना xkb_keymap *keymap, ध्रुव वर्ण *name)
{
    संरचना xkb_कुञ्चिका *कुञ्चिका;
    xkb_atom_t atom;

    atom = xkb_atom_lookup(keymap->ctx, name);
    यदि (atom) {
        प्रति (darray_आकार_t i = 0; i < keymap->अङ्कः_कुञ्चिका_aliases; i++)
            यदि (keymap->कुञ्चिका_aliases[i].alias == atom)
                atom = keymap->कुञ्चिका_aliases[i].real;
    }
    यदि (!atom)
        प्रत्यागम XKB_KEYCODE_INVALID;

    xkb_कुञ्चिकाः_foreach(कुञ्चिका, keymap) {
        यदि (कुञ्चिका->name == atom)
            प्रत्यागम कुञ्चिका->keycode;
    }

    प्रत्यागम XKB_KEYCODE_INVALID;
}

/**
 * Simple boolean specifying whether or not the key should repeat.
 */
पूर्णाङ्क
xkb_keymap_कुञ्चिका_repeats(संरचना xkb_keymap *keymap, xkb_keycode_t kc)
{
    ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका = Xkb_कुञ्चिका(keymap, kc);

    यदि (!कुञ्चिका)
        प्रत्यागम 0;

    प्रत्यागम कुञ्चिका->repeats;
}
