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
    परत keymap;
}

शून्य
clear_level(संरचना xkb_level *leveli)
{
    जर (leveli->अंक_syms > 1)
        मुक्त(leveli->s.syms);
    जर (leveli->अंक_actions > 1)
        मुक्त(leveli->a.actions);
}

स्थिर शून्य
clear_interpret(संरचना xkb_sym_interpret *interp)
{
    जर (interp->अंक_actions > 1)
        मुक्त(interp->a.actions);
}

शून्य
xkb_keymap_unref(संरचना xkb_keymap *keymap)
{
    प्रतिज्ञा(!keymap || keymap->refcnt > 0);
    जर (!keymap || --keymap->refcnt > 0)
        परत;

    जर (keymap->किल्ल्या) {
        संरचना xkb_किल्ली *किल्ली;
        xkb_किल्ल्या_foreach(किल्ली, keymap) {
            जर (किल्ली->groups) {
                साठी (xkb_layout_index_t i = 0; i < किल्ली->अंक_groups; i++) {
                    जर (किल्ली->groups[i].levels) {
                        साठी (xkb_level_index_t j = 0;
                             j < Xkb_किल्ली_अंक_Levels(किल्ली, i);
                             j++)
                            clear_level(&किल्ली->groups[i].levels[j]);
                        मुक्त(किल्ली->groups[i].levels);
                    }
                }
                मुक्त(किल्ली->groups);
            }
            जर (!किल्ली->overlays_inline && किल्ली->overlays_किल्ल्या) {
                मुक्त((शून्य *)किल्ली->overlays_किल्ल्या);
            }
        }
        मुक्त(keymap->किल्ल्या);
    }
    जर (keymap->types) {
        साठी (darray_size_t i = 0; i < keymap->अंक_types; i++) {
            मुक्त(keymap->types[i].entries);
            मुक्त(keymap->types[i].level_names);
        }
        मुक्त(keymap->types);
    }
    साठी (darray_size_t k = 0; k < keymap->अंक_sym_interprets; k++) {
        clear_interpret(&keymap->sym_interprets[k]);
    }
    मुक्त(keymap->sym_interprets);
    मुक्त(keymap->modmaps);
    मुक्त(keymap->किल्ली_aliases);
    मुक्त(keymap->group_names);
    मुक्त(keymap->keycodes_section_name);
    मुक्त(keymap->symbols_section_name);
    मुक्त(keymap->types_section_name);
    मुक्त(keymap->compat_section_name);
    xkb_context_unref(keymap->ctx);
    मुक्त(keymap);
}

स्थिर स्थिरांक संरचना xkb_keymap_format_ops *
get_keymap_format_ops(गणना xkb_keymap_format format)
{
    स्थिर स्थिरांक संरचना xkb_keymap_format_ops *keymap_format_ops[] = {
        [XKB_KEYMAP_FORMAT_TEXT_V1] = &text_v1_keymap_format_ops,
        [XKB_KEYMAP_FORMAT_TEXT_V2] = &text_v1_keymap_format_ops,
    };

    जर ((पूर्णांक) format < 0 || (पूर्णांक) format >= (पूर्णांक) ARRAY_SIZE(keymap_format_ops))
        परत रिक्त;

    परत keymap_format_ops[(पूर्णांक) format];
}

संरचना xkb_keymap *
xkb_keymap_new_from_rmlvo(स्थिरांक संरचना xkb_rmlvo_builder *rmlvo,
                          गणना xkb_keymap_format format,
                          गणना xkb_keymap_compile_flags flags)
{
    स्थिरांक संरचना xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    जर (!ops || !ops->keymap_new_from_rmlvo) {
        log_err_func(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        परत रिक्त;
    }

    संरचना xkb_keymap *keymap = xkb_keymap_new(rmlvo->ctx, __func__, format,
                                               flags);
    जर (!keymap)
        परत रिक्त;

    जर (!ops->keymap_new_from_rmlvo(keymap, rmlvo)) {
        xkb_keymap_unref(keymap);
        परत रिक्त;
    }

    परत keymap;
}

संरचना xkb_keymap *
xkb_keymap_new_from_names2(संरचना xkb_context *ctx,
                           स्थिरांक संरचना xkb_rule_names *rmlvo_in,
                           गणना xkb_keymap_format format,
                           गणना xkb_keymap_compile_flags flags)
{
    स्थिरांक संरचना xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    जर (!ops || !ops->keymap_new_from_names) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        परत रिक्त;
    }

    संरचना xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    जर (!keymap)
        परत रिक्त;

    संरचना xkb_rule_names rmlvo = {0};
    जर (rmlvo_in)
        rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    जर (!ops->keymap_new_from_names(keymap, &rmlvo)) {
        xkb_keymap_unref(keymap);
        परत रिक्त;
    }

    परत keymap;
}


संरचना xkb_keymap *
xkb_keymap_new_from_names(संरचना xkb_context *ctx,
                          स्थिरांक संरचना xkb_rule_names *rmlvo_in,
                          गणना xkb_keymap_compile_flags flags)
{
    परत xkb_keymap_new_from_names2(ctx, rmlvo_in,
                                      XKB_KEYMAP_FORMAT_TEXT_V2, flags);
}

संरचना xkb_keymap *
xkb_keymap_new_from_string(संरचना xkb_context *ctx,
                           स्थिरांक अक्षर *string,
                           गणना xkb_keymap_format format,
                           गणना xkb_keymap_compile_flags flags)
{
    परत xkb_keymap_new_from_buffer(ctx, string, म_माप(string),
                                      format, flags);
}

संरचना xkb_keymap *
xkb_keymap_new_from_buffer(संरचना xkb_context *ctx,
                           स्थिरांक अक्षर *buffer, माप_प्रकार length,
                           गणना xkb_keymap_format format,
                           गणना xkb_keymap_compile_flags flags)
{
    स्थिरांक संरचना xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    जर (!ops || !ops->keymap_new_from_string) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        परत रिक्त;
    }

    जर (!buffer) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no buffer specified\n");
        परत रिक्त;
    }

    संरचना xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    जर (!keymap)
        परत रिक्त;

    /* Allow a zero-terminated string as a buffer */
    जर (length > 0 && buffer[length - 1] == '\0')
        length--;

    जर (!ops->keymap_new_from_string(keymap, buffer, length)) {
        xkb_keymap_unref(keymap);
        परत रिक्त;
    }

    परत keymap;
}

संरचना xkb_keymap *
xkb_keymap_new_from_file(संरचना xkb_context *ctx,
                         खाता *file,
                         गणना xkb_keymap_format format,
                         गणना xkb_keymap_compile_flags flags)
{
    स्थिरांक संरचना xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    जर (!ops || !ops->keymap_new_from_file) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        परत रिक्त;
    }

    जर (!file) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no file specified\n");
        परत रिक्त;
    }

    संरचना xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    जर (!keymap)
        परत रिक्त;

    जर (!ops->keymap_new_from_file(keymap, file)) {
        xkb_keymap_unref(keymap);
        परत रिक्त;
    }

    परत keymap;
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
स्थिर गणना xkb_status
check_keymap_serialize_abi(
    संरचना xkb_context * restrict ctx,
    स्थिरांक अक्षर * restrict func,
    स्थिरांक संरचना xkb_keymap_serialize_config * restrict config,
    स्थिरांक संरचना xkb_keymap_serialize_result * restrict result
)
{
    गणना xkb_status status;
    जर ((status = xkb_check_keymap_abi(config)) ||
        (status = xkb_check_keymap_abi(result))) {
        xkb_log_abi_error(ctx, func, status);
    }
    परत status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

गणना xkb_status
xkb_keymap_serialize(स्थिरांक संरचना xkb_keymap *keymap,
                     स्थिरांक संरचना xkb_keymap_serialize_config *config,
                     संरचना xkb_keymap_serialize_result *result)
{
    /* Check ABI compatibility */
    स्थिरांक गणना xkb_status status =
        check_keymap_serialize_abi(keymap->ctx, __func__, config, result);
    जर (status != XKB_SUCCESS)
        परत status;

    संरचना xkb_keymap_serialize_config new_config = *config;

    स्थिरांक गणना xkb_keymap_serialize_flags invalid_flags = (
        new_config.flags &
        ~(गणना xkb_keymap_serialize_flags)XKB_KEYMAP_SERIALIZE_FLAGS_मूल्ये
    );
    जर (invalid_flags) {
        log_err_func(keymap->ctx,
                     XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS_,
                     "unrecognized serialization flags: %#x\n",
                     invalid_flags);
        परत XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS;
    }

    जर (new_config.format == XKB_KEYMAP_USE_ORIGINAL_FORMAT)
        new_config.format = keymap->format;

    स्थिरांक संरचना xkb_keymap_format_ops * स्थिरांक ops =
        get_keymap_format_ops(new_config.format);
    जर (!ops || !ops->keymap_serialize) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", new_config.format);
        परत XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT;
    }

    स्थिरांक xkb_layout_mask_t all_layouts =
        (xkb_layout_mask_t)((UINT64_C(1) << keymap->अंक_groups) - 1);
    जर (!new_config.layouts) {
        new_config.layouts = all_layouts;
    }

    जर (new_config.layouts & ~all_layouts) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                     "unsupported layout mask: "
                     "expected subset of 0x%08"PRIx32", got: 0x%08"PRIx32"\n",
                     all_layouts, new_config.layouts);
        परत XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    स्थिरांक xkb_layout_index_t max_groups = format_max_groups(config->format);
    स्थिरांक xkb_layout_index_t अंक_groups =
        (xkb_layout_index_t)popcount32(new_config.layouts);

    जर (अंक_groups > max_groups) {
        स्थिरांक bool strict =
            (new_config.flags & XKB_KEYMAP_SERIALIZE_STRICT_MODE);
        xkb_log_with_code(
            keymap->ctx,
            (strict ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
            XKB_LOG_VERBOSITY_MINIMAL,
            XKB_ERROR_LAYOUT_मोजणी_LIMIT_EXCEEDED_2,
            "Cannot serialize %"PRIu32" groups in keymap format %d: "
            "maximum is %"PRIu32"%s\n",
            अंक_groups, config->format, max_groups,
            (strict ? "" : "; discarding unsupported groups")
        );
        जर (strict) {
            परत XKB_ERROR_LAYOUT_मोजणी_LIMIT_EXCEEDED;
        } अन्यथा {
            new_config.layouts =
                keep_lowest_n_set_bits(new_config.layouts, max_groups);
        }
    }

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Serializing group mask 0x%"PRIx32" (total: %"PRIu32")\n",
            new_config.layouts, अंक_groups);

    जर (new_config.layouts != all_layouts) {
        /*
         * If some layouts are discarded, then interprets may not trigger
         * properly. Avoid this by requiring explicit values.
         */
        new_config.flags |= (XKB_KEYMAP_SERIALIZE_EXPLICIT_किल्ली_मूल्ये |
                             XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS);
    }

    परत ops->keymap_serialize(keymap, &new_config, result);
}

अक्षर *
xkb_keymap_get_as_string2(संरचना xkb_keymap *keymap,
                          गणना xkb_keymap_format format,
                          गणना xkb_keymap_serialize_flags flags)
{
    स्थिरांक संरचना xkb_keymap_serialize_config config = {
        .size = आकार(config),
        .flags = flags,
        .format = format,
        .layouts = 0,
    };

    संरचना xkb_keymap_serialize_result result = { .size = आकार(result) };

    परत (xkb_keymap_serialize(keymap, &config, &result) == XKB_SUCCESS)
        ? result.serialized
        : रिक्त;
}

अक्षर *
xkb_keymap_get_as_string(संरचना xkb_keymap *keymap,
                         गणना xkb_keymap_format format)
{
    परत xkb_keymap_get_as_string2(keymap, format,
                                     XKB_KEYMAP_SERIALIZE_NO_FLAGS);
}

/**
 * Returns the total number of modifiers active in the keymap.
 */
xkb_mod_index_t
xkb_keymap_अंक_mods(संरचना xkb_keymap *keymap)
{
    परत keymap->mods.अंक_mods;
}

/**
 * Return the name for a given modifier.
 */
स्थिरांक अक्षर *
xkb_keymap_mod_get_name(संरचना xkb_keymap *keymap, xkb_mod_index_t idx)
{
    जर (idx >= keymap->mods.अंक_mods)
        परत रिक्त;

    परत xkb_atom_text(keymap->ctx, keymap->mods.mods[idx].name);
}

/**
 * Returns the index for a named modifier.
 */
xkb_mod_index_t
xkb_keymap_mod_get_index(संरचना xkb_keymap *keymap, स्थिरांक अक्षर *name)
{
    स्थिरांक xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    परत (atom == XKB_ATOM_NONE)
        ? XKB_MOD_INVALID
        : XkbModNameToIndex(&keymap->mods, atom, MOD_BOTH);
}

/**
 * Return the canonical mapping of a named modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask(संरचना xkb_keymap *keymap, स्थिरांक अक्षर* name)
{
    स्थिरांक xkb_mod_index_t idx = xkb_keymap_mod_get_index(keymap, name);
    परत (idx >= keymap->mods.अंक_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the canonical mapping of a given modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask2(संरचना xkb_keymap *keymap, xkb_mod_index_t idx)
{
    परत (idx >= keymap->mods.अंक_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the total number of active groups in the keymap.
 */
xkb_layout_index_t
xkb_keymap_अंक_layouts(संरचना xkb_keymap *keymap)
{
    परत keymap->अंक_groups;
}

/**
 * Returns the name for a given group.
 */
स्थिरांक अक्षर *
xkb_keymap_layout_get_name(संरचना xkb_keymap *keymap, xkb_layout_index_t idx)
{
    जर (idx >= keymap->अंक_group_names)
        परत रिक्त;

    परत xkb_atom_text(keymap->ctx, keymap->group_names[idx]);
}

/**
 * Returns the index for a named layout.
 */
xkb_layout_index_t
xkb_keymap_layout_get_index(संरचना xkb_keymap *keymap, स्थिरांक अक्षर *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_layout_index_t i;

    जर (atom == XKB_ATOM_NONE)
        परत XKB_LAYOUT_INVALID;

    साठी (i = 0; i < keymap->अंक_group_names; i++)
        जर (keymap->group_names[i] == atom)
            परत i;

    परत XKB_LAYOUT_INVALID;
}

/**
 * Returns the number of layouts active for a particular key.
 */
xkb_layout_index_t
xkb_keymap_अंक_layouts_for_किल्ली(संरचना xkb_keymap *keymap, xkb_keycode_t kc)
{
    स्थिरांक संरचना xkb_किल्ली *किल्ली = Xkb_किल्ली(keymap, kc);

    जर (!किल्ली)
        परत 0;

    परत किल्ली->अंक_groups;
}

/**
 * Returns the number of levels active for a particular key and layout.
 */
xkb_level_index_t
xkb_keymap_अंक_levels_for_किल्ली(संरचना xkb_keymap *keymap, xkb_keycode_t kc,
                              xkb_layout_index_t layout)
{
    स्थिरांक संरचना xkb_किल्ली *किल्ली = Xkb_किल्ली(keymap, kc);

    जर (!किल्ली)
        परत 0;

    layout = xkb_keymap_किल्ली_effective_layout(किल्ली, layout);
    जर (layout == XKB_LAYOUT_INVALID)
        परत 0;

    परत Xkb_किल्ली_अंक_Levels(किल्ली, layout);
}

/**
 * Return the total number of LEDs in the keymap.
 */
xkb_led_index_t
xkb_keymap_अंक_leds(संरचना xkb_keymap *keymap)
{
    परत keymap->अंक_leds;
}

/**
 * Returns the name for a given LED.
 */
स्थिरांक अक्षर *
xkb_keymap_led_get_name(संरचना xkb_keymap *keymap, xkb_led_index_t idx)
{
    जर (idx >= keymap->अंक_leds)
        परत रिक्त;

    परत xkb_atom_text(keymap->ctx, keymap->leds[idx].name);
}

/**
 * Returns the index for a named LED.
 */
xkb_led_index_t
xkb_keymap_led_get_index(संरचना xkb_keymap *keymap, स्थिरांक अक्षर *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_led_index_t i;
    स्थिरांक संरचना xkb_led *led;

    जर (atom == XKB_ATOM_NONE)
        परत XKB_LED_INVALID;

    xkb_leds_enumerate(i, led, keymap)
        जर (led->name == atom)
            परत i;

    परत XKB_LED_INVALID;
}

माप_प्रकार
xkb_keymap_किल्ली_get_mods_for_level(संरचना xkb_keymap *keymap,
                                  xkb_keycode_t kc,
                                  xkb_layout_index_t layout,
                                  xkb_level_index_t level,
                                  xkb_mod_mask_t *masks_out,
                                  माप_प्रकार masks_size)
{
    स्थिरांक संरचना xkb_किल्ली *किल्ली = Xkb_किल्ली(keymap, kc);
    जर (!किल्ली)
        परत 0;

    layout = xkb_keymap_किल्ली_effective_layout(किल्ली, layout);
    जर (layout == XKB_LAYOUT_INVALID)
        परत 0;

    जर (level >= Xkb_किल्ली_अंक_Levels(किल्ली, layout))
        परत 0;

    स्थिरांक संरचना xkb_किल्ली_type *type = किल्ली->groups[layout].type;

    माप_प्रकार मोजणी = 0;

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
    जर (level == 0) {
        bool empty_mapped = false;
        साठी (darray_size_t i = 0; i < type->अंक_entries && मोजणी < masks_size; i++)
            जर (entry_is_active(&type->entries[i]) &&
                type->entries[i].mods.mask == 0) {
                empty_mapped = true;
                थांबा;
            }
        जर (!empty_mapped && मोजणी < masks_size) {
            masks_out[मोजणी++] = 0;
        }
    }

    /* Now search explicit mappings. */
    साठी (darray_size_t i = 0; i < type->अंक_entries && मोजणी < masks_size; i++) {
        जर (entry_is_active(&type->entries[i]) &&
            type->entries[i].level == level) {
            masks_out[मोजणी++] = type->entries[i].mods.mask;
        }
    }

    परत मोजणी;
}

संरचना xkb_level *
xkb_keymap_किल्ली_get_level(संरचना xkb_keymap *keymap, स्थिरांक संरचना xkb_किल्ली *किल्ली,
                         xkb_layout_index_t layout, xkb_level_index_t level)
{
    layout = xkb_keymap_किल्ली_effective_layout(किल्ली, layout);
    जर (layout == XKB_LAYOUT_INVALID)
        परत रिक्त;

    जर (level >= Xkb_किल्ली_अंक_Levels(किल्ली, layout))
        परत रिक्त;

    परत &किल्ली->groups[layout].levels[level];
}

/**
 * As below, but takes an explicit layout/level rather than state.
 */
पूर्णांक
xkb_keymap_किल्ली_get_syms_by_level(संरचना xkb_keymap *keymap,
                                 xkb_keycode_t kc,
                                 xkb_layout_index_t layout,
                                 xkb_level_index_t level,
                                 स्थिरांक xkb_keysym_t **syms_out)
{
    स्थिरांक संरचना xkb_किल्ली *किल्ली = Xkb_किल्ली(keymap, kc);

    जर (!किल्ली)
        जा err;

    स्थिरांक संरचना xkb_level* स्थिरांक leveli =
        xkb_keymap_किल्ली_get_level(keymap, किल्ली, layout, level);

    जर (!leveli)
        जा err;

    स्थिरांक xkb_keysym_मोजणी_t अंक_syms = leveli->अंक_syms;
    जर (अंक_syms == 0)
        जा err;

    जर (अंक_syms == 1)
        *syms_out = &leveli->s.sym;
    अन्यथा
        *syms_out = leveli->s.syms;

    परत (पूर्णांक) अंक_syms;

err:
    *syms_out = रिक्त;
    परत 0;
}

xkb_keycode_t
xkb_keymap_min_keycode(संरचना xkb_keymap *keymap)
{
    परत keymap->min_किल्ली_code;
}

xkb_keycode_t
xkb_keymap_max_keycode(संरचना xkb_keymap *keymap)
{
    परत keymap->max_किल्ली_code;
}

स्थिर गणना xkb_status
keymap_किल्ली_iterator_config_check(
    संरचना xkb_context * restrict ctx,
    स्थिरांक अक्षर *func,
    स्थिरांक संरचना xkb_keymap_किल्ली_iterator_config * restrict config
)
{
    /* Check ABI compatibility */
    स्थिरांक गणना xkb_status abi_error = xkb_check_keymap_abi(config);
    जर (abi_error) {
        xkb_log_abi_error(ctx, func, abi_error);
        परत abi_error;
    }

    /* Sanitize input: flags */
    स्थिरांक uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_KEYMAP_किल्ली_ITERATOR_FLAGS_मूल्ये);
    जर (invalid_flags) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_किल्ली_ITERATOR_FLAGS_2,
                "%s: Unsupported keymap iterator flags: 0x%"PRIx32"\n",
                func, invalid_flags);
        परत XKB_ERROR_UNSUPPORTED_किल्ली_ITERATOR_FLAGS;
    }

    /* Sanitize input: start */
    जर (config->start > XKB_KEYCODE_MAX)
        परत XKB_ERROR_INVALID_KEYCODE;

    परत XKB_SUCCESS;
}

static_assert(XKB_KEYMAP_किल्ली_ITERATOR_FLAGS_मूल्ये <=
              (पूर्णांक)XKB_POINTER_TAG_MASK,
              "Cannot tag cursor pointer with flags");

#define iter_init_keymap(i, ptr, flags) {          \
    (i)->opaque0 = xkb_pointer_tag((ptr), (flags));\
}
#define iter_get_keymap(i) (                                  \
    (स्थिरांक संरचना xkb_keymap *)xkb_pointer_untag((i)->opaque0)\
)
#define iter_get_next(i) ((स्थिरांक संरचना xkb_किल्ली *)(i)->opaque1)
#define iter_set_next(i, ptr) {(i)->opaque1 = (स्थिरांक शून्य *)(ptr);}
#define iter_get_descending_order(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                 \
    (uintptr_t)XKB_KEYMAP_किल्ली_ITERATOR_DESCENDING_ORDER \
)
#define iter_get_include_unbound(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                \
    (uintptr_t)XKB_KEYMAP_किल्ली_ITERATOR_INCLUDE_UNBOUND \
)

गणना xkb_status
xkb_keymap_किल्ली_iterator_init(
    संरचना xkb_keymap_किल्ली_iterator * restrict iter,
    स्थिरांक संरचना xkb_keymap * restrict keymap,
    स्थिरांक संरचना xkb_keymap_किल्ली_iterator_config * restrict config
)
{
    /* Handle default configuration */
    स्थिर स्थिरांक संरचना xkb_keymap_किल्ली_iterator_config default_config = {
        .size = आकार(default_config)
    };
    जर (!config)
        config = &default_config;

    /* Check input */
    स्थिरांक गणना xkb_status status =
        keymap_किल्ली_iterator_config_check(keymap->ctx, __func__, config);
    जर (status != XKB_SUCCESS) {
        परत status;
    }

    /*
     * Initialization
     */

    स_रखो(iter, 0, आकार(*iter));

    /* Borrow keymap */
    iter_init_keymap(iter, keymap, config->flags);

    जर (keymap->अंक_किल्ल्या == 0) {
        iter_set_next(iter, रिक्त);
        परत XKB_SUCCESS;
    }

    /* Borrow key */
    स्थिरांक संरचना xkb_किल्ली *next;
    जर (config->flags & XKB_KEYMAP_किल्ली_ITERATOR_DESCENDING_ORDER) {
        जर (!config->start || config->start > keymap->max_किल्ली_code) {
            next = &keymap->किल्ल्या[keymap->अंक_किल्ल्या - 1];
        } अन्यथा {
            /*
             * May still fail *safely* if start < keymap->min_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_किल्ली(keymap, false, config->start);
        }
    } अन्यथा {
        जर (!config->start || config->start < keymap->min_किल्ली_code) {
            next = (keymap->अंक_किल्ल्या_खालचा)
                ? &keymap->किल्ल्या[keymap->min_किल्ली_code]
                : &keymap->किल्ल्या[0];
        } अन्यथा {
            /*
             * May still fail *safely* if start > keymap->max_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_किल्ली(keymap, true, config->start);
        }
    };
    iter_set_next(iter, next);

    परत XKB_SUCCESS;
}

xkb_keycode_t
xkb_keymap_किल्ली_iterator_next(संरचना xkb_keymap_किल्ली_iterator *iter)
{
    स्थिरांक संरचना xkb_किल्ली *next = iter_get_next(iter);
    जर (!next)
        परत XKB_KEYCODE_INVALID;

    स्थिरांक संरचना xkb_keymap *keymap = iter_get_keymap(iter);
    प्रतिज्ञा(keymap->अंक_किल्ल्या);

    स्थिरांक bool descending = iter_get_descending_order(iter);
    स्थिरांक bool skip_unbound = !iter_get_include_unbound(iter);
    स्थिरांक संरचना xkb_किल्ली *last = descending
        ? (keymap->अंक_किल्ल्या_खालचा)
            ? &keymap->किल्ल्या[keymap->min_किल्ली_code]
            : &keymap->किल्ल्या[0]
        : &keymap->किल्ल्या[keymap->अंक_किल्ल्या - 1];
    स्थिरांक सूचक_भेद_प्रकार step = descending ? -1 : 1;

    /* Skip undefined keys (no name) and optionally unbound keys */
    साठी (;;) {
        स्थिरांक bool iter_end = descending ? (next <= last) : (next >= last);
        स्थिरांक bool skip = (
            next->name == XKB_ATOM_NONE ||
            (skip_unbound && (next->अंक_groups == 0))
        );
        जर (!skip) {
            स्थिरांक xkb_keycode_t ret = next->keycode;
            iter_set_next(iter, iter_end ? रिक्त : next + step);
            परत ret;
        }
        जर (iter_end) {
            iter_set_next(iter, रिक्त);
            परत XKB_KEYCODE_INVALID;
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
xkb_keymap_किल्ली_for_each(संरचना xkb_keymap *keymap, xkb_keymap_किल्ली_iter_t iter,
                        शून्य *data)
{
    संरचना xkb_किल्ली *किल्ली;

    xkb_किल्ल्या_foreach(किल्ली, keymap)
        iter(keymap, किल्ली->keycode, data);
}

स्थिरांक अक्षर *
xkb_keymap_किल्ली_get_name(संरचना xkb_keymap *keymap, xkb_keycode_t kc)
{
    स्थिरांक संरचना xkb_किल्ली *किल्ली = Xkb_किल्ली(keymap, kc);

    जर (!किल्ली)
        परत रिक्त;

    परत xkb_atom_text(keymap->ctx, किल्ली->name);
}

xkb_keycode_t
xkb_keymap_किल्ली_by_name(संरचना xkb_keymap *keymap, स्थिरांक अक्षर *name)
{
    संरचना xkb_किल्ली *किल्ली;
    xkb_atom_t atom;

    atom = xkb_atom_lookup(keymap->ctx, name);
    जर (atom) {
        साठी (darray_size_t i = 0; i < keymap->अंक_किल्ली_aliases; i++)
            जर (keymap->किल्ली_aliases[i].alias == atom)
                atom = keymap->किल्ली_aliases[i].real;
    }
    जर (!atom)
        परत XKB_KEYCODE_INVALID;

    xkb_किल्ल्या_foreach(किल्ली, keymap) {
        जर (किल्ली->name == atom)
            परत किल्ली->keycode;
    }

    परत XKB_KEYCODE_INVALID;
}

/**
 * Simple boolean specifying whether or not the key should repeat.
 */
पूर्णांक
xkb_keymap_किल्ली_repeats(संरचना xkb_keymap *keymap, xkb_keycode_t kc)
{
    स्थिरांक संरचना xkb_किल्ली *किल्ली = Xkb_किल्ली(keymap, kc);

    जर (!किल्ली)
        परत 0;

    परत किल्ली->repeats;
}
