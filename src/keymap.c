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

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <stdint.h>
#ضمن <string.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "abi-check.h"
#ضمن "atom.h"
#ضمن "context.h"
#ضمن "features/enums.h"
#ضمن "keymap.h"
#ضمن "keymap-priv.h"
#ضمن "messages-codes.h"
#ضمن "text.h"
#ضمن "util-mem.h"
#ضمن "util-numbers.h"

بنية xkb_keymap *
xkb_keymap_ref(بنية xkb_keymap *keymap)
{
    أكد(keymap->refcnt > 0);
    keymap->refcnt++;
    أرجع keymap;
}

فراغ
clear_level(بنية xkb_level *leveli)
{
    إذا (leveli->ژمارە_syms > 1)
        حرر(leveli->s.syms);
    إذا (leveli->ژمارە_actions > 1)
        حرر(leveli->a.actions);
}

ساكن فراغ
clear_interpret(بنية xkb_sym_interpret *interp)
{
    إذا (interp->ژمارە_actions > 1)
        حرر(interp->a.actions);
}

فراغ
xkb_keymap_unref(بنية xkb_keymap *keymap)
{
    أكد(!keymap || keymap->refcnt > 0);
    إذا (!keymap || --keymap->refcnt > 0)
        أرجع;

    إذا (keymap->کلیلەکان) {
        بنية xkb_کلیل *کلیل;
        xkb_کلیلەکان_foreach(کلیل, keymap) {
            إذا (کلیل->groups) {
                لكل (xkb_layout_index_t i = 0; i < کلیل->ژمارە_groups; i++) {
                    إذا (کلیل->groups[i].levels) {
                        لكل (xkb_level_index_t j = 0;
                             j < Xkb_کلیل_ژمارە_Levels(کلیل, i);
                             j++)
                            clear_level(&کلیل->groups[i].levels[j]);
                        حرر(کلیل->groups[i].levels);
                    }
                }
                حرر(کلیل->groups);
            }
            إذا (!کلیل->overlays_inline && کلیل->overlays_کلیلەکان) {
                حرر((فراغ *)کلیل->overlays_کلیلەکان);
            }
        }
        حرر(keymap->کلیلەکان);
    }
    إذا (keymap->types) {
        لكل (darray_size_t i = 0; i < keymap->ژمارە_types; i++) {
            حرر(keymap->types[i].entries);
            حرر(keymap->types[i].level_names);
        }
        حرر(keymap->types);
    }
    لكل (darray_size_t k = 0; k < keymap->ژمارە_sym_interprets; k++) {
        clear_interpret(&keymap->sym_interprets[k]);
    }
    حرر(keymap->sym_interprets);
    حرر(keymap->modmaps);
    حرر(keymap->کلیل_aliases);
    حرر(keymap->group_names);
    حرر(keymap->keycodes_section_name);
    حرر(keymap->symbols_section_name);
    حرر(keymap->types_section_name);
    حرر(keymap->compat_section_name);
    xkb_context_unref(keymap->ctx);
    حرر(keymap);
}

ساكن ثابت بنية xkb_keymap_format_ops *
get_keymap_format_ops(تعداد xkb_keymap_format format)
{
    ساكن ثابت بنية xkb_keymap_format_ops *keymap_format_ops[] = {
        [XKB_KEYMAP_FORMAT_TEXT_V1] = &text_v1_keymap_format_ops,
        [XKB_KEYMAP_FORMAT_TEXT_V2] = &text_v1_keymap_format_ops,
    };

    إذا ((صحيح) format < 0 || (صحيح) format >= (صحيح) ARRAY_SIZE(keymap_format_ops))
        أرجع عدم;

    أرجع keymap_format_ops[(صحيح) format];
}

بنية xkb_keymap *
xkb_keymap_new_from_rmlvo(ثابت بنية xkb_rmlvo_builder *rmlvo,
                          تعداد xkb_keymap_format format,
                          تعداد xkb_keymap_compile_flags flags)
{
    ثابت بنية xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    إذا (!ops || !ops->keymap_new_from_rmlvo) {
        log_err_func(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        أرجع عدم;
    }

    بنية xkb_keymap *keymap = xkb_keymap_new(rmlvo->ctx, __func__, format,
                                               flags);
    إذا (!keymap)
        أرجع عدم;

    إذا (!ops->keymap_new_from_rmlvo(keymap, rmlvo)) {
        xkb_keymap_unref(keymap);
        أرجع عدم;
    }

    أرجع keymap;
}

بنية xkb_keymap *
xkb_keymap_new_from_names2(بنية xkb_context *ctx,
                           ثابت بنية xkb_rule_names *rmlvo_in,
                           تعداد xkb_keymap_format format,
                           تعداد xkb_keymap_compile_flags flags)
{
    ثابت بنية xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    إذا (!ops || !ops->keymap_new_from_names) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        أرجع عدم;
    }

    بنية xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    إذا (!keymap)
        أرجع عدم;

    بنية xkb_rule_names rmlvo = {0};
    إذا (rmlvo_in)
        rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    إذا (!ops->keymap_new_from_names(keymap, &rmlvo)) {
        xkb_keymap_unref(keymap);
        أرجع عدم;
    }

    أرجع keymap;
}


بنية xkb_keymap *
xkb_keymap_new_from_names(بنية xkb_context *ctx,
                          ثابت بنية xkb_rule_names *rmlvo_in,
                          تعداد xkb_keymap_compile_flags flags)
{
    أرجع xkb_keymap_new_from_names2(ctx, rmlvo_in,
                                      XKB_KEYMAP_FORMAT_TEXT_V2, flags);
}

بنية xkb_keymap *
xkb_keymap_new_from_string(بنية xkb_context *ctx,
                           ثابت حرف *string,
                           تعداد xkb_keymap_format format,
                           تعداد xkb_keymap_compile_flags flags)
{
    أرجع xkb_keymap_new_from_buffer(ctx, string, strlen(string),
                                      format, flags);
}

بنية xkb_keymap *
xkb_keymap_new_from_buffer(بنية xkb_context *ctx,
                           ثابت حرف *buffer, size_t length,
                           تعداد xkb_keymap_format format,
                           تعداد xkb_keymap_compile_flags flags)
{
    ثابت بنية xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    إذا (!ops || !ops->keymap_new_from_string) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        أرجع عدم;
    }

    إذا (!buffer) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no buffer specified\n");
        أرجع عدم;
    }

    بنية xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    إذا (!keymap)
        أرجع عدم;

    /* Allow a zero-terminated string as a buffer */
    إذا (length > 0 && buffer[length - 1] == '\0')
        length--;

    إذا (!ops->keymap_new_from_string(keymap, buffer, length)) {
        xkb_keymap_unref(keymap);
        أرجع عدم;
    }

    أرجع keymap;
}

بنية xkb_keymap *
xkb_keymap_new_from_file(بنية xkb_context *ctx,
                         FILE *file,
                         تعداد xkb_keymap_format format,
                         تعداد xkb_keymap_compile_flags flags)
{
    ثابت بنية xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    إذا (!ops || !ops->keymap_new_from_file) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        أرجع عدم;
    }

    إذا (!file) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no file specified\n");
        أرجع عدم;
    }

    بنية xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    إذا (!keymap)
        أرجع عدم;

    إذا (!ops->keymap_new_from_file(keymap, file)) {
        xkb_keymap_unref(keymap);
        أرجع عدم;
    }

    أرجع keymap;
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
ساكن تعداد xkb_status
check_keymap_serialize_abi(
    بنية xkb_context * restrict ctx,
    ثابت حرف * restrict func,
    ثابت بنية xkb_keymap_serialize_config * restrict config,
    ثابت بنية xkb_keymap_serialize_result * restrict result
)
{
    تعداد xkb_status status;
    إذا ((status = xkb_check_keymap_abi(config)) ||
        (status = xkb_check_keymap_abi(result))) {
        xkb_log_abi_error(ctx, func, status);
    }
    أرجع status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

تعداد xkb_status
xkb_keymap_serialize(ثابت بنية xkb_keymap *keymap,
                     ثابت بنية xkb_keymap_serialize_config *config,
                     بنية xkb_keymap_serialize_result *result)
{
    /* Check ABI compatibility */
    ثابت تعداد xkb_status status =
        check_keymap_serialize_abi(keymap->ctx, __func__, config, result);
    إذا (status != XKB_SUCCESS)
        أرجع status;

    بنية xkb_keymap_serialize_config new_config = *config;

    ثابت تعداد xkb_keymap_serialize_flags invalid_flags = (
        new_config.flags &
        ~(تعداد xkb_keymap_serialize_flags)XKB_KEYMAP_SERIALIZE_FLAGS_بەهاکان
    );
    إذا (invalid_flags) {
        log_err_func(keymap->ctx,
                     XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS_,
                     "unrecognized serialization flags: %#x\n",
                     invalid_flags);
        أرجع XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS;
    }

    إذا (new_config.format == XKB_KEYMAP_USE_ORIGINAL_FORMAT)
        new_config.format = keymap->format;

    ثابت بنية xkb_keymap_format_ops * ثابت ops =
        get_keymap_format_ops(new_config.format);
    إذا (!ops || !ops->keymap_serialize) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", new_config.format);
        أرجع XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT;
    }

    ثابت xkb_layout_mask_t all_layouts =
        (xkb_layout_mask_t)((UINT64_C(1) << keymap->ژمارە_groups) - 1);
    إذا (!new_config.layouts) {
        new_config.layouts = all_layouts;
    }

    إذا (new_config.layouts & ~all_layouts) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                     "unsupported layout mask: "
                     "expected subset of 0x%08"PRIx32", got: 0x%08"PRIx32"\n",
                     all_layouts, new_config.layouts);
        أرجع XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    ثابت xkb_layout_index_t max_groups = format_max_groups(config->format);
    ثابت xkb_layout_index_t ژمارە_groups =
        (xkb_layout_index_t)popcount32(new_config.layouts);

    إذا (ژمارە_groups > max_groups) {
        ثابت bool strict =
            (new_config.flags & XKB_KEYMAP_SERIALIZE_STRICT_MODE);
        xkb_log_with_code(
            keymap->ctx,
            (strict ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
            XKB_LOG_VERBOSITY_MINIMAL,
            XKB_ERROR_LAYOUT_ژماردن_LIMIT_EXCEEDED_2,
            "Cannot serialize %"PRIu32" groups in keymap format %d: "
            "maximum is %"PRIu32"%s\n",
            ژمارە_groups, config->format, max_groups,
            (strict ? "" : "; discarding unsupported groups")
        );
        إذا (strict) {
            أرجع XKB_ERROR_LAYOUT_ژماردن_LIMIT_EXCEEDED;
        } وإلا {
            new_config.layouts =
                keep_lowest_n_set_bits(new_config.layouts, max_groups);
        }
    }

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Serializing group mask 0x%"PRIx32" (total: %"PRIu32")\n",
            new_config.layouts, ژمارە_groups);

    إذا (new_config.layouts != all_layouts) {
        /*
         * If some layouts are discarded, then interprets may not trigger
         * properly. Avoid this by requiring explicit values.
         */
        new_config.flags |= (XKB_KEYMAP_SERIALIZE_EXPLICIT_کلیل_بەهاکان |
                             XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS);
    }

    أرجع ops->keymap_serialize(keymap, &new_config, result);
}

حرف *
xkb_keymap_get_as_string2(بنية xkb_keymap *keymap,
                          تعداد xkb_keymap_format format,
                          تعداد xkb_keymap_serialize_flags flags)
{
    ثابت بنية xkb_keymap_serialize_config config = {
        .size = حجم(config),
        .flags = flags,
        .format = format,
        .layouts = 0,
    };

    بنية xkb_keymap_serialize_result result = { .size = حجم(result) };

    أرجع (xkb_keymap_serialize(keymap, &config, &result) == XKB_SUCCESS)
        ? result.serialized
        : عدم;
}

حرف *
xkb_keymap_get_as_string(بنية xkb_keymap *keymap,
                         تعداد xkb_keymap_format format)
{
    أرجع xkb_keymap_get_as_string2(keymap, format,
                                     XKB_KEYMAP_SERIALIZE_NO_FLAGS);
}

/**
 * Returns the total number of modifiers active in the keymap.
 */
xkb_mod_index_t
xkb_keymap_ژمارە_mods(بنية xkb_keymap *keymap)
{
    أرجع keymap->mods.ژمارە_mods;
}

/**
 * Return the name for a given modifier.
 */
ثابت حرف *
xkb_keymap_mod_get_name(بنية xkb_keymap *keymap, xkb_mod_index_t idx)
{
    إذا (idx >= keymap->mods.ژمارە_mods)
        أرجع عدم;

    أرجع xkb_atom_text(keymap->ctx, keymap->mods.mods[idx].name);
}

/**
 * Returns the index for a named modifier.
 */
xkb_mod_index_t
xkb_keymap_mod_get_index(بنية xkb_keymap *keymap, ثابت حرف *name)
{
    ثابت xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    أرجع (atom == XKB_ATOM_NONE)
        ? XKB_MOD_INVALID
        : XkbModNameToIndex(&keymap->mods, atom, MOD_BOTH);
}

/**
 * Return the canonical mapping of a named modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask(بنية xkb_keymap *keymap, ثابت حرف* name)
{
    ثابت xkb_mod_index_t idx = xkb_keymap_mod_get_index(keymap, name);
    أرجع (idx >= keymap->mods.ژمارە_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the canonical mapping of a given modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask2(بنية xkb_keymap *keymap, xkb_mod_index_t idx)
{
    أرجع (idx >= keymap->mods.ژمارە_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the total number of active groups in the keymap.
 */
xkb_layout_index_t
xkb_keymap_ژمارە_layouts(بنية xkb_keymap *keymap)
{
    أرجع keymap->ژمارە_groups;
}

/**
 * Returns the name for a given group.
 */
ثابت حرف *
xkb_keymap_layout_get_name(بنية xkb_keymap *keymap, xkb_layout_index_t idx)
{
    إذا (idx >= keymap->ژمارە_group_names)
        أرجع عدم;

    أرجع xkb_atom_text(keymap->ctx, keymap->group_names[idx]);
}

/**
 * Returns the index for a named layout.
 */
xkb_layout_index_t
xkb_keymap_layout_get_index(بنية xkb_keymap *keymap, ثابت حرف *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_layout_index_t i;

    إذا (atom == XKB_ATOM_NONE)
        أرجع XKB_LAYOUT_INVALID;

    لكل (i = 0; i < keymap->ژمارە_group_names; i++)
        إذا (keymap->group_names[i] == atom)
            أرجع i;

    أرجع XKB_LAYOUT_INVALID;
}

/**
 * Returns the number of layouts active for a particular key.
 */
xkb_layout_index_t
xkb_keymap_ژمارە_layouts_for_کلیل(بنية xkb_keymap *keymap, xkb_keycode_t kc)
{
    ثابت بنية xkb_کلیل *کلیل = Xkb_کلیل(keymap, kc);

    إذا (!کلیل)
        أرجع 0;

    أرجع کلیل->ژمارە_groups;
}

/**
 * Returns the number of levels active for a particular key and layout.
 */
xkb_level_index_t
xkb_keymap_ژمارە_levels_for_کلیل(بنية xkb_keymap *keymap, xkb_keycode_t kc,
                              xkb_layout_index_t layout)
{
    ثابت بنية xkb_کلیل *کلیل = Xkb_کلیل(keymap, kc);

    إذا (!کلیل)
        أرجع 0;

    layout = xkb_keymap_کلیل_effective_layout(کلیل, layout);
    إذا (layout == XKB_LAYOUT_INVALID)
        أرجع 0;

    أرجع Xkb_کلیل_ژمارە_Levels(کلیل, layout);
}

/**
 * Return the total number of LEDs in the keymap.
 */
xkb_led_index_t
xkb_keymap_ژمارە_leds(بنية xkb_keymap *keymap)
{
    أرجع keymap->ژمارە_leds;
}

/**
 * Returns the name for a given LED.
 */
ثابت حرف *
xkb_keymap_led_get_name(بنية xkb_keymap *keymap, xkb_led_index_t idx)
{
    إذا (idx >= keymap->ژمارە_leds)
        أرجع عدم;

    أرجع xkb_atom_text(keymap->ctx, keymap->leds[idx].name);
}

/**
 * Returns the index for a named LED.
 */
xkb_led_index_t
xkb_keymap_led_get_index(بنية xkb_keymap *keymap, ثابت حرف *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_led_index_t i;
    ثابت بنية xkb_led *led;

    إذا (atom == XKB_ATOM_NONE)
        أرجع XKB_LED_INVALID;

    xkb_leds_enumerate(i, led, keymap)
        إذا (led->name == atom)
            أرجع i;

    أرجع XKB_LED_INVALID;
}

size_t
xkb_keymap_کلیل_get_mods_for_level(بنية xkb_keymap *keymap,
                                  xkb_keycode_t kc,
                                  xkb_layout_index_t layout,
                                  xkb_level_index_t level,
                                  xkb_mod_mask_t *masks_out,
                                  size_t masks_size)
{
    ثابت بنية xkb_کلیل *کلیل = Xkb_کلیل(keymap, kc);
    إذا (!کلیل)
        أرجع 0;

    layout = xkb_keymap_کلیل_effective_layout(کلیل, layout);
    إذا (layout == XKB_LAYOUT_INVALID)
        أرجع 0;

    إذا (level >= Xkb_کلیل_ژمارە_Levels(کلیل, layout))
        أرجع 0;

    ثابت بنية xkb_کلیل_type *type = کلیل->groups[layout].type;

    size_t ژماردن = 0;

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
    إذا (level == 0) {
        bool empty_mapped = false;
        لكل (darray_size_t i = 0; i < type->ژمارە_entries && ژماردن < masks_size; i++)
            إذا (entry_is_active(&type->entries[i]) &&
                type->entries[i].mods.mask == 0) {
                empty_mapped = true;
                اكسر;
            }
        إذا (!empty_mapped && ژماردن < masks_size) {
            masks_out[ژماردن++] = 0;
        }
    }

    /* Now search explicit mappings. */
    لكل (darray_size_t i = 0; i < type->ژمارە_entries && ژماردن < masks_size; i++) {
        إذا (entry_is_active(&type->entries[i]) &&
            type->entries[i].level == level) {
            masks_out[ژماردن++] = type->entries[i].mods.mask;
        }
    }

    أرجع ژماردن;
}

بنية xkb_level *
xkb_keymap_کلیل_get_level(بنية xkb_keymap *keymap, ثابت بنية xkb_کلیل *کلیل,
                         xkb_layout_index_t layout, xkb_level_index_t level)
{
    layout = xkb_keymap_کلیل_effective_layout(کلیل, layout);
    إذا (layout == XKB_LAYOUT_INVALID)
        أرجع عدم;

    إذا (level >= Xkb_کلیل_ژمارە_Levels(کلیل, layout))
        أرجع عدم;

    أرجع &کلیل->groups[layout].levels[level];
}

/**
 * As below, but takes an explicit layout/level rather than state.
 */
صحيح
xkb_keymap_کلیل_get_syms_by_level(بنية xkb_keymap *keymap,
                                 xkb_keycode_t kc,
                                 xkb_layout_index_t layout,
                                 xkb_level_index_t level,
                                 ثابت xkb_keysym_t **syms_out)
{
    ثابت بنية xkb_کلیل *کلیل = Xkb_کلیل(keymap, kc);

    إذا (!کلیل)
        اذهب err;

    ثابت بنية xkb_level* ثابت leveli =
        xkb_keymap_کلیل_get_level(keymap, کلیل, layout, level);

    إذا (!leveli)
        اذهب err;

    ثابت xkb_keysym_ژماردن_t ژمارە_syms = leveli->ژمارە_syms;
    إذا (ژمارە_syms == 0)
        اذهب err;

    إذا (ژمارە_syms == 1)
        *syms_out = &leveli->s.sym;
    وإلا
        *syms_out = leveli->s.syms;

    أرجع (صحيح) ژمارە_syms;

err:
    *syms_out = عدم;
    أرجع 0;
}

xkb_keycode_t
xkb_keymap_min_keycode(بنية xkb_keymap *keymap)
{
    أرجع keymap->min_کلیل_code;
}

xkb_keycode_t
xkb_keymap_max_keycode(بنية xkb_keymap *keymap)
{
    أرجع keymap->max_کلیل_code;
}

ساكن تعداد xkb_status
keymap_کلیل_iterator_config_check(
    بنية xkb_context * restrict ctx,
    ثابت حرف *func,
    ثابت بنية xkb_keymap_کلیل_iterator_config * restrict config
)
{
    /* Check ABI compatibility */
    ثابت تعداد xkb_status abi_error = xkb_check_keymap_abi(config);
    إذا (abi_error) {
        xkb_log_abi_error(ctx, func, abi_error);
        أرجع abi_error;
    }

    /* Sanitize input: flags */
    ثابت uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_KEYMAP_کلیل_ITERATOR_FLAGS_بەهاکان);
    إذا (invalid_flags) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_کلیل_ITERATOR_FLAGS_2,
                "%s: Unsupported keymap iterator flags: 0x%"PRIx32"\n",
                func, invalid_flags);
        أرجع XKB_ERROR_UNSUPPORTED_کلیل_ITERATOR_FLAGS;
    }

    /* Sanitize input: start */
    إذا (config->start > XKB_KEYCODE_MAX)
        أرجع XKB_ERROR_INVALID_KEYCODE;

    أرجع XKB_SUCCESS;
}

static_assert(XKB_KEYMAP_کلیل_ITERATOR_FLAGS_بەهاکان <=
              (صحيح)XKB_POINTER_TAG_MASK,
              "Cannot tag cursor pointer with flags");

#define iter_init_keymap(i, ptr, flags) {          \
    (i)->opaque0 = xkb_pointer_tag((ptr), (flags));\
}
#define iter_get_keymap(i) (                                  \
    (ثابت بنية xkb_keymap *)xkb_pointer_untag((i)->opaque0)\
)
#define iter_get_next(i) ((ثابت بنية xkb_کلیل *)(i)->opaque1)
#define iter_set_next(i, ptr) {(i)->opaque1 = (ثابت فراغ *)(ptr);}
#define iter_get_descending_order(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                 \
    (uintptr_t)XKB_KEYMAP_کلیل_ITERATOR_DESCENDING_ORDER \
)
#define iter_get_include_unbound(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                \
    (uintptr_t)XKB_KEYMAP_کلیل_ITERATOR_INCLUDE_UNBOUND \
)

تعداد xkb_status
xkb_keymap_کلیل_iterator_init(
    بنية xkb_keymap_کلیل_iterator * restrict iter,
    ثابت بنية xkb_keymap * restrict keymap,
    ثابت بنية xkb_keymap_کلیل_iterator_config * restrict config
)
{
    /* Handle default configuration */
    ساكن ثابت بنية xkb_keymap_کلیل_iterator_config default_config = {
        .size = حجم(default_config)
    };
    إذا (!config)
        config = &default_config;

    /* Check input */
    ثابت تعداد xkb_status status =
        keymap_کلیل_iterator_config_check(keymap->ctx, __func__, config);
    إذا (status != XKB_SUCCESS) {
        أرجع status;
    }

    /*
     * Initialization
     */

    memset(iter, 0, حجم(*iter));

    /* Borrow keymap */
    iter_init_keymap(iter, keymap, config->flags);

    إذا (keymap->ژمارە_کلیلەکان == 0) {
        iter_set_next(iter, عدم);
        أرجع XKB_SUCCESS;
    }

    /* Borrow key */
    ثابت بنية xkb_کلیل *next;
    إذا (config->flags & XKB_KEYMAP_کلیل_ITERATOR_DESCENDING_ORDER) {
        إذا (!config->start || config->start > keymap->max_کلیل_code) {
            next = &keymap->کلیلەکان[keymap->ژمارە_کلیلەکان - 1];
        } وإلا {
            /*
             * May still fail *safely* if start < keymap->min_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_کلیل(keymap, false, config->start);
        }
    } وإلا {
        إذا (!config->start || config->start < keymap->min_کلیل_code) {
            next = (keymap->ژمارە_کلیلەکان_نزم)
                ? &keymap->کلیلەکان[keymap->min_کلیل_code]
                : &keymap->کلیلەکان[0];
        } وإلا {
            /*
             * May still fail *safely* if start > keymap->max_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_کلیل(keymap, true, config->start);
        }
    };
    iter_set_next(iter, next);

    أرجع XKB_SUCCESS;
}

xkb_keycode_t
xkb_keymap_کلیل_iterator_next(بنية xkb_keymap_کلیل_iterator *iter)
{
    ثابت بنية xkb_کلیل *next = iter_get_next(iter);
    إذا (!next)
        أرجع XKB_KEYCODE_INVALID;

    ثابت بنية xkb_keymap *keymap = iter_get_keymap(iter);
    أكد(keymap->ژمارە_کلیلەکان);

    ثابت bool descending = iter_get_descending_order(iter);
    ثابت bool skip_unbound = !iter_get_include_unbound(iter);
    ثابت بنية xkb_کلیل *last = descending
        ? (keymap->ژمارە_کلیلەکان_نزم)
            ? &keymap->کلیلەکان[keymap->min_کلیل_code]
            : &keymap->کلیلەکان[0]
        : &keymap->کلیلەکان[keymap->ژمارە_کلیلەکان - 1];
    ثابت ptrdiff_t step = descending ? -1 : 1;

    /* Skip undefined keys (no name) and optionally unbound keys */
    لكل (;;) {
        ثابت bool iter_end = descending ? (next <= last) : (next >= last);
        ثابت bool skip = (
            next->name == XKB_ATOM_NONE ||
            (skip_unbound && (next->ژمارە_groups == 0))
        );
        إذا (!skip) {
            ثابت xkb_keycode_t ret = next->keycode;
            iter_set_next(iter, iter_end ? عدم : next + step);
            أرجع ret;
        }
        إذا (iter_end) {
            iter_set_next(iter, عدم);
            أرجع XKB_KEYCODE_INVALID;
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

فراغ
xkb_keymap_کلیل_for_each(بنية xkb_keymap *keymap, xkb_keymap_کلیل_iter_t iter,
                        فراغ *data)
{
    بنية xkb_کلیل *کلیل;

    xkb_کلیلەکان_foreach(کلیل, keymap)
        iter(keymap, کلیل->keycode, data);
}

ثابت حرف *
xkb_keymap_کلیل_get_name(بنية xkb_keymap *keymap, xkb_keycode_t kc)
{
    ثابت بنية xkb_کلیل *کلیل = Xkb_کلیل(keymap, kc);

    إذا (!کلیل)
        أرجع عدم;

    أرجع xkb_atom_text(keymap->ctx, کلیل->name);
}

xkb_keycode_t
xkb_keymap_کلیل_by_name(بنية xkb_keymap *keymap, ثابت حرف *name)
{
    بنية xkb_کلیل *کلیل;
    xkb_atom_t atom;

    atom = xkb_atom_lookup(keymap->ctx, name);
    إذا (atom) {
        لكل (darray_size_t i = 0; i < keymap->ژمارە_کلیل_aliases; i++)
            إذا (keymap->کلیل_aliases[i].alias == atom)
                atom = keymap->کلیل_aliases[i].real;
    }
    إذا (!atom)
        أرجع XKB_KEYCODE_INVALID;

    xkb_کلیلەکان_foreach(کلیل, keymap) {
        إذا (کلیل->name == atom)
            أرجع کلیل->keycode;
    }

    أرجع XKB_KEYCODE_INVALID;
}

/**
 * Simple boolean specifying whether or not the key should repeat.
 */
صحيح
xkb_keymap_کلیل_repeats(بنية xkb_keymap *keymap, xkb_keycode_t kc)
{
    ثابت بنية xkb_کلیل *کلیل = Xkb_کلیل(keymap, kc);

    إذا (!کلیل)
        أرجع 0;

    أرجع کلیل->repeats;
}
