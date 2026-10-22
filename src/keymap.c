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

#شامل "config.h"

#شامل <assert.h>
#شامل <stdint.h>
#شامل <string.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "abi-check.h"
#شامل "atom.h"
#شامل "context.h"
#شامل "features/enums.h"
#شامل "keymap.h"
#شامل "keymap-priv.h"
#شامل "messages-codes.h"
#شامل "text.h"
#شامل "util-mem.h"
#شامل "util-numbers.h"

ساختار xkb_keymap *
xkb_keymap_ref(ساختار xkb_keymap *keymap)
{
    أكد(keymap->refcnt > 0);
    keymap->refcnt++;
    بازگشت keymap;
}

تهی
clear_level(ساختار xkb_level *leveli)
{
    اگر (leveli->رقم_syms > 1)
        آزاد(leveli->s.syms);
    اگر (leveli->رقم_actions > 1)
        آزاد(leveli->a.actions);
}

ایستا تهی
clear_interpret(ساختار xkb_sym_interpret *interp)
{
    اگر (interp->رقم_actions > 1)
        آزاد(interp->a.actions);
}

تهی
xkb_keymap_unref(ساختار xkb_keymap *keymap)
{
    أكد(!keymap || keymap->refcnt > 0);
    اگر (!keymap || --keymap->refcnt > 0)
        بازگشت;

    اگر (keymap->کلیدها) {
        ساختار xkb_کلید *کلید;
        xkb_کلیدها_foreach(کلید, keymap) {
            اگر (کلید->groups) {
                برای (xkb_layout_index_t i = 0; i < کلید->رقم_groups; i++) {
                    اگر (کلید->groups[i].levels) {
                        برای (xkb_level_index_t j = 0;
                             j < Xkb_کلید_رقم_Levels(کلید, i);
                             j++)
                            clear_level(&کلید->groups[i].levels[j]);
                        آزاد(کلید->groups[i].levels);
                    }
                }
                آزاد(کلید->groups);
            }
            اگر (!کلید->overlays_inline && کلید->overlays_کلیدها) {
                آزاد((تهی *)کلید->overlays_کلیدها);
            }
        }
        آزاد(keymap->کلیدها);
    }
    اگر (keymap->types) {
        برای (darray_size_t i = 0; i < keymap->رقم_types; i++) {
            آزاد(keymap->types[i].entries);
            آزاد(keymap->types[i].level_names);
        }
        آزاد(keymap->types);
    }
    برای (darray_size_t k = 0; k < keymap->رقم_sym_interprets; k++) {
        clear_interpret(&keymap->sym_interprets[k]);
    }
    آزاد(keymap->sym_interprets);
    آزاد(keymap->modmaps);
    آزاد(keymap->کلید_aliases);
    آزاد(keymap->group_names);
    آزاد(keymap->keycodes_section_name);
    آزاد(keymap->symbols_section_name);
    آزاد(keymap->types_section_name);
    آزاد(keymap->compat_section_name);
    xkb_context_unref(keymap->ctx);
    آزاد(keymap);
}

ایستا ثابت ساختار xkb_keymap_format_ops *
get_keymap_format_ops(شمارش xkb_keymap_format format)
{
    ایستا ثابت ساختار xkb_keymap_format_ops *keymap_format_ops[] = {
        [XKB_KEYMAP_FORMAT_TEXT_V1] = &text_v1_keymap_format_ops,
        [XKB_KEYMAP_FORMAT_TEXT_V2] = &text_v1_keymap_format_ops,
    };

    اگر ((صحیح) format < 0 || (صحیح) format >= (صحیح) ARRAY_SIZE(keymap_format_ops))
        بازگشت پوچ;

    بازگشت keymap_format_ops[(صحیح) format];
}

ساختار xkb_keymap *
xkb_keymap_new_from_rmlvo(ثابت ساختار xkb_rmlvo_builder *rmlvo,
                          شمارش xkb_keymap_format format,
                          شمارش xkb_keymap_compile_flags flags)
{
    ثابت ساختار xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    اگر (!ops || !ops->keymap_new_from_rmlvo) {
        log_err_func(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        بازگشت پوچ;
    }

    ساختار xkb_keymap *keymap = xkb_keymap_new(rmlvo->ctx, __func__, format,
                                               flags);
    اگر (!keymap)
        بازگشت پوچ;

    اگر (!ops->keymap_new_from_rmlvo(keymap, rmlvo)) {
        xkb_keymap_unref(keymap);
        بازگشت پوچ;
    }

    بازگشت keymap;
}

ساختار xkb_keymap *
xkb_keymap_new_from_names2(ساختار xkb_context *ctx,
                           ثابت ساختار xkb_rule_names *rmlvo_in,
                           شمارش xkb_keymap_format format,
                           شمارش xkb_keymap_compile_flags flags)
{
    ثابت ساختار xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    اگر (!ops || !ops->keymap_new_from_names) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        بازگشت پوچ;
    }

    ساختار xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    اگر (!keymap)
        بازگشت پوچ;

    ساختار xkb_rule_names rmlvo = {0};
    اگر (rmlvo_in)
        rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    اگر (!ops->keymap_new_from_names(keymap, &rmlvo)) {
        xkb_keymap_unref(keymap);
        بازگشت پوچ;
    }

    بازگشت keymap;
}


ساختار xkb_keymap *
xkb_keymap_new_from_names(ساختار xkb_context *ctx,
                          ثابت ساختار xkb_rule_names *rmlvo_in,
                          شمارش xkb_keymap_compile_flags flags)
{
    بازگشت xkb_keymap_new_from_names2(ctx, rmlvo_in,
                                      XKB_KEYMAP_FORMAT_TEXT_V2, flags);
}

ساختار xkb_keymap *
xkb_keymap_new_from_string(ساختار xkb_context *ctx,
                           ثابت نویسه *string,
                           شمارش xkb_keymap_format format,
                           شمارش xkb_keymap_compile_flags flags)
{
    بازگشت xkb_keymap_new_from_buffer(ctx, string, strlen(string),
                                      format, flags);
}

ساختار xkb_keymap *
xkb_keymap_new_from_buffer(ساختار xkb_context *ctx,
                           ثابت نویسه *buffer, size_t length,
                           شمارش xkb_keymap_format format,
                           شمارش xkb_keymap_compile_flags flags)
{
    ثابت ساختار xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    اگر (!ops || !ops->keymap_new_from_string) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        بازگشت پوچ;
    }

    اگر (!buffer) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no buffer specified\n");
        بازگشت پوچ;
    }

    ساختار xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    اگر (!keymap)
        بازگشت پوچ;

    /* Allow a zero-terminated string as a buffer */
    اگر (length > 0 && buffer[length - 1] == '\0')
        length--;

    اگر (!ops->keymap_new_from_string(keymap, buffer, length)) {
        xkb_keymap_unref(keymap);
        بازگشت پوچ;
    }

    بازگشت keymap;
}

ساختار xkb_keymap *
xkb_keymap_new_from_file(ساختار xkb_context *ctx,
                         FILE *file,
                         شمارش xkb_keymap_format format,
                         شمارش xkb_keymap_compile_flags flags)
{
    ثابت ساختار xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    اگر (!ops || !ops->keymap_new_from_file) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        بازگشت پوچ;
    }

    اگر (!file) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no file specified\n");
        بازگشت پوچ;
    }

    ساختار xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    اگر (!keymap)
        بازگشت پوچ;

    اگر (!ops->keymap_new_from_file(keymap, file)) {
        xkb_keymap_unref(keymap);
        بازگشت پوچ;
    }

    بازگشت keymap;
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
ایستا شمارش xkb_status
check_keymap_serialize_abi(
    ساختار xkb_context * restrict ctx,
    ثابت نویسه * restrict func,
    ثابت ساختار xkb_keymap_serialize_config * restrict config,
    ثابت ساختار xkb_keymap_serialize_result * restrict result
)
{
    شمارش xkb_status status;
    اگر ((status = xkb_check_keymap_abi(config)) ||
        (status = xkb_check_keymap_abi(result))) {
        xkb_log_abi_error(ctx, func, status);
    }
    بازگشت status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

شمارش xkb_status
xkb_keymap_serialize(ثابت ساختار xkb_keymap *keymap,
                     ثابت ساختار xkb_keymap_serialize_config *config,
                     ساختار xkb_keymap_serialize_result *result)
{
    /* Check ABI compatibility */
    ثابت شمارش xkb_status status =
        check_keymap_serialize_abi(keymap->ctx, __func__, config, result);
    اگر (status != XKB_SUCCESS)
        بازگشت status;

    ساختار xkb_keymap_serialize_config new_config = *config;

    ثابت شمارش xkb_keymap_serialize_flags invalid_flags = (
        new_config.flags &
        ~(شمارش xkb_keymap_serialize_flags)XKB_KEYMAP_SERIALIZE_FLAGS_مقادیر
    );
    اگر (invalid_flags) {
        log_err_func(keymap->ctx,
                     XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS_,
                     "unrecognized serialization flags: %#x\n",
                     invalid_flags);
        بازگشت XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS;
    }

    اگر (new_config.format == XKB_KEYMAP_USE_ORIGINAL_FORMAT)
        new_config.format = keymap->format;

    ثابت ساختار xkb_keymap_format_ops * ثابت ops =
        get_keymap_format_ops(new_config.format);
    اگر (!ops || !ops->keymap_serialize) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", new_config.format);
        بازگشت XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT;
    }

    ثابت xkb_layout_mask_t all_layouts =
        (xkb_layout_mask_t)((UINT64_C(1) << keymap->رقم_groups) - 1);
    اگر (!new_config.layouts) {
        new_config.layouts = all_layouts;
    }

    اگر (new_config.layouts & ~all_layouts) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                     "unsupported layout mask: "
                     "expected subset of 0x%08"PRIx32", got: 0x%08"PRIx32"\n",
                     all_layouts, new_config.layouts);
        بازگشت XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    ثابت xkb_layout_index_t max_groups = format_max_groups(config->format);
    ثابت xkb_layout_index_t رقم_groups =
        (xkb_layout_index_t)popcount32(new_config.layouts);

    اگر (رقم_groups > max_groups) {
        ثابت bool strict =
            (new_config.flags & XKB_KEYMAP_SERIALIZE_STRICT_MODE);
        xkb_log_with_code(
            keymap->ctx,
            (strict ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
            XKB_LOG_VERBOSITY_MINIMAL,
            XKB_ERROR_LAYOUT_شمار_LIMIT_EXCEEDED_2,
            "Cannot serialize %"PRIu32" groups in keymap format %d: "
            "maximum is %"PRIu32"%s\n",
            رقم_groups, config->format, max_groups,
            (strict ? "" : "; discarding unsupported groups")
        );
        اگر (strict) {
            بازگشت XKB_ERROR_LAYOUT_شمار_LIMIT_EXCEEDED;
        } وگرنه {
            new_config.layouts =
                keep_lowest_n_set_bits(new_config.layouts, max_groups);
        }
    }

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Serializing group mask 0x%"PRIx32" (total: %"PRIu32")\n",
            new_config.layouts, رقم_groups);

    اگر (new_config.layouts != all_layouts) {
        /*
         * If some layouts are discarded, then interprets may not trigger
         * properly. Avoid this by requiring explicit values.
         */
        new_config.flags |= (XKB_KEYMAP_SERIALIZE_EXPLICIT_کلید_مقادیر |
                             XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS);
    }

    بازگشت ops->keymap_serialize(keymap, &new_config, result);
}

نویسه *
xkb_keymap_get_as_string2(ساختار xkb_keymap *keymap,
                          شمارش xkb_keymap_format format,
                          شمارش xkb_keymap_serialize_flags flags)
{
    ثابت ساختار xkb_keymap_serialize_config config = {
        .size = اندازه(config),
        .flags = flags,
        .format = format,
        .layouts = 0,
    };

    ساختار xkb_keymap_serialize_result result = { .size = اندازه(result) };

    بازگشت (xkb_keymap_serialize(keymap, &config, &result) == XKB_SUCCESS)
        ? result.serialized
        : پوچ;
}

نویسه *
xkb_keymap_get_as_string(ساختار xkb_keymap *keymap,
                         شمارش xkb_keymap_format format)
{
    بازگشت xkb_keymap_get_as_string2(keymap, format,
                                     XKB_KEYMAP_SERIALIZE_NO_FLAGS);
}

/**
 * Returns the total number of modifiers active in the keymap.
 */
xkb_mod_index_t
xkb_keymap_رقم_mods(ساختار xkb_keymap *keymap)
{
    بازگشت keymap->mods.رقم_mods;
}

/**
 * Return the name for a given modifier.
 */
ثابت نویسه *
xkb_keymap_mod_get_name(ساختار xkb_keymap *keymap, xkb_mod_index_t idx)
{
    اگر (idx >= keymap->mods.رقم_mods)
        بازگشت پوچ;

    بازگشت xkb_atom_text(keymap->ctx, keymap->mods.mods[idx].name);
}

/**
 * Returns the index for a named modifier.
 */
xkb_mod_index_t
xkb_keymap_mod_get_index(ساختار xkb_keymap *keymap, ثابت نویسه *name)
{
    ثابت xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    بازگشت (atom == XKB_ATOM_NONE)
        ? XKB_MOD_INVALID
        : XkbModNameToIndex(&keymap->mods, atom, MOD_BOTH);
}

/**
 * Return the canonical mapping of a named modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask(ساختار xkb_keymap *keymap, ثابت نویسه* name)
{
    ثابت xkb_mod_index_t idx = xkb_keymap_mod_get_index(keymap, name);
    بازگشت (idx >= keymap->mods.رقم_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the canonical mapping of a given modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask2(ساختار xkb_keymap *keymap, xkb_mod_index_t idx)
{
    بازگشت (idx >= keymap->mods.رقم_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the total number of active groups in the keymap.
 */
xkb_layout_index_t
xkb_keymap_رقم_layouts(ساختار xkb_keymap *keymap)
{
    بازگشت keymap->رقم_groups;
}

/**
 * Returns the name for a given group.
 */
ثابت نویسه *
xkb_keymap_layout_get_name(ساختار xkb_keymap *keymap, xkb_layout_index_t idx)
{
    اگر (idx >= keymap->رقم_group_names)
        بازگشت پوچ;

    بازگشت xkb_atom_text(keymap->ctx, keymap->group_names[idx]);
}

/**
 * Returns the index for a named layout.
 */
xkb_layout_index_t
xkb_keymap_layout_get_index(ساختار xkb_keymap *keymap, ثابت نویسه *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_layout_index_t i;

    اگر (atom == XKB_ATOM_NONE)
        بازگشت XKB_LAYOUT_INVALID;

    برای (i = 0; i < keymap->رقم_group_names; i++)
        اگر (keymap->group_names[i] == atom)
            بازگشت i;

    بازگشت XKB_LAYOUT_INVALID;
}

/**
 * Returns the number of layouts active for a particular key.
 */
xkb_layout_index_t
xkb_keymap_رقم_layouts_for_کلید(ساختار xkb_keymap *keymap, xkb_keycode_t kc)
{
    ثابت ساختار xkb_کلید *کلید = Xkb_کلید(keymap, kc);

    اگر (!کلید)
        بازگشت 0;

    بازگشت کلید->رقم_groups;
}

/**
 * Returns the number of levels active for a particular key and layout.
 */
xkb_level_index_t
xkb_keymap_رقم_levels_for_کلید(ساختار xkb_keymap *keymap, xkb_keycode_t kc,
                              xkb_layout_index_t layout)
{
    ثابت ساختار xkb_کلید *کلید = Xkb_کلید(keymap, kc);

    اگر (!کلید)
        بازگشت 0;

    layout = xkb_keymap_کلید_effective_layout(کلید, layout);
    اگر (layout == XKB_LAYOUT_INVALID)
        بازگشت 0;

    بازگشت Xkb_کلید_رقم_Levels(کلید, layout);
}

/**
 * Return the total number of LEDs in the keymap.
 */
xkb_led_index_t
xkb_keymap_رقم_leds(ساختار xkb_keymap *keymap)
{
    بازگشت keymap->رقم_leds;
}

/**
 * Returns the name for a given LED.
 */
ثابت نویسه *
xkb_keymap_led_get_name(ساختار xkb_keymap *keymap, xkb_led_index_t idx)
{
    اگر (idx >= keymap->رقم_leds)
        بازگشت پوچ;

    بازگشت xkb_atom_text(keymap->ctx, keymap->leds[idx].name);
}

/**
 * Returns the index for a named LED.
 */
xkb_led_index_t
xkb_keymap_led_get_index(ساختار xkb_keymap *keymap, ثابت نویسه *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_led_index_t i;
    ثابت ساختار xkb_led *led;

    اگر (atom == XKB_ATOM_NONE)
        بازگشت XKB_LED_INVALID;

    xkb_leds_enumerate(i, led, keymap)
        اگر (led->name == atom)
            بازگشت i;

    بازگشت XKB_LED_INVALID;
}

size_t
xkb_keymap_کلید_get_mods_for_level(ساختار xkb_keymap *keymap,
                                  xkb_keycode_t kc,
                                  xkb_layout_index_t layout,
                                  xkb_level_index_t level,
                                  xkb_mod_mask_t *masks_out,
                                  size_t masks_size)
{
    ثابت ساختار xkb_کلید *کلید = Xkb_کلید(keymap, kc);
    اگر (!کلید)
        بازگشت 0;

    layout = xkb_keymap_کلید_effective_layout(کلید, layout);
    اگر (layout == XKB_LAYOUT_INVALID)
        بازگشت 0;

    اگر (level >= Xkb_کلید_رقم_Levels(کلید, layout))
        بازگشت 0;

    ثابت ساختار xkb_کلید_type *type = کلید->groups[layout].type;

    size_t شمار = 0;

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
    اگر (level == 0) {
        bool empty_mapped = false;
        برای (darray_size_t i = 0; i < type->رقم_entries && شمار < masks_size; i++)
            اگر (entry_is_active(&type->entries[i]) &&
                type->entries[i].mods.mask == 0) {
                empty_mapped = true;
                بشکن;
            }
        اگر (!empty_mapped && شمار < masks_size) {
            masks_out[شمار++] = 0;
        }
    }

    /* Now search explicit mappings. */
    برای (darray_size_t i = 0; i < type->رقم_entries && شمار < masks_size; i++) {
        اگر (entry_is_active(&type->entries[i]) &&
            type->entries[i].level == level) {
            masks_out[شمار++] = type->entries[i].mods.mask;
        }
    }

    بازگشت شمار;
}

ساختار xkb_level *
xkb_keymap_کلید_get_level(ساختار xkb_keymap *keymap, ثابت ساختار xkb_کلید *کلید,
                         xkb_layout_index_t layout, xkb_level_index_t level)
{
    layout = xkb_keymap_کلید_effective_layout(کلید, layout);
    اگر (layout == XKB_LAYOUT_INVALID)
        بازگشت پوچ;

    اگر (level >= Xkb_کلید_رقم_Levels(کلید, layout))
        بازگشت پوچ;

    بازگشت &کلید->groups[layout].levels[level];
}

/**
 * As below, but takes an explicit layout/level rather than state.
 */
صحیح
xkb_keymap_کلید_get_syms_by_level(ساختار xkb_keymap *keymap,
                                 xkb_keycode_t kc,
                                 xkb_layout_index_t layout,
                                 xkb_level_index_t level,
                                 ثابت xkb_keysym_t **syms_out)
{
    ثابت ساختار xkb_کلید *کلید = Xkb_کلید(keymap, kc);

    اگر (!کلید)
        برو err;

    ثابت ساختار xkb_level* ثابت leveli =
        xkb_keymap_کلید_get_level(keymap, کلید, layout, level);

    اگر (!leveli)
        برو err;

    ثابت xkb_keysym_شمار_t رقم_syms = leveli->رقم_syms;
    اگر (رقم_syms == 0)
        برو err;

    اگر (رقم_syms == 1)
        *syms_out = &leveli->s.sym;
    وگرنه
        *syms_out = leveli->s.syms;

    بازگشت (صحیح) رقم_syms;

err:
    *syms_out = پوچ;
    بازگشت 0;
}

xkb_keycode_t
xkb_keymap_min_keycode(ساختار xkb_keymap *keymap)
{
    بازگشت keymap->min_کلید_code;
}

xkb_keycode_t
xkb_keymap_max_keycode(ساختار xkb_keymap *keymap)
{
    بازگشت keymap->max_کلید_code;
}

ایستا شمارش xkb_status
keymap_کلید_iterator_config_check(
    ساختار xkb_context * restrict ctx,
    ثابت نویسه *func,
    ثابت ساختار xkb_keymap_کلید_iterator_config * restrict config
)
{
    /* Check ABI compatibility */
    ثابت شمارش xkb_status abi_error = xkb_check_keymap_abi(config);
    اگر (abi_error) {
        xkb_log_abi_error(ctx, func, abi_error);
        بازگشت abi_error;
    }

    /* Sanitize input: flags */
    ثابت uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_KEYMAP_کلید_ITERATOR_FLAGS_مقادیر);
    اگر (invalid_flags) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_کلید_ITERATOR_FLAGS_2,
                "%s: Unsupported keymap iterator flags: 0x%"PRIx32"\n",
                func, invalid_flags);
        بازگشت XKB_ERROR_UNSUPPORTED_کلید_ITERATOR_FLAGS;
    }

    /* Sanitize input: start */
    اگر (config->start > XKB_KEYCODE_MAX)
        بازگشت XKB_ERROR_INVALID_KEYCODE;

    بازگشت XKB_SUCCESS;
}

static_assert(XKB_KEYMAP_کلید_ITERATOR_FLAGS_مقادیر <=
              (صحیح)XKB_POINTER_TAG_MASK,
              "Cannot tag cursor pointer with flags");

#define iter_init_keymap(i, ptr, flags) {          \
    (i)->opaque0 = xkb_pointer_tag((ptr), (flags));\
}
#define iter_get_keymap(i) (                                  \
    (ثابت ساختار xkb_keymap *)xkb_pointer_untag((i)->opaque0)\
)
#define iter_get_next(i) ((ثابت ساختار xkb_کلید *)(i)->opaque1)
#define iter_set_next(i, ptr) {(i)->opaque1 = (ثابت تهی *)(ptr);}
#define iter_get_descending_order(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                 \
    (uintptr_t)XKB_KEYMAP_کلید_ITERATOR_DESCENDING_ORDER \
)
#define iter_get_include_unbound(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                \
    (uintptr_t)XKB_KEYMAP_کلید_ITERATOR_INCLUDE_UNBOUND \
)

شمارش xkb_status
xkb_keymap_کلید_iterator_init(
    ساختار xkb_keymap_کلید_iterator * restrict iter,
    ثابت ساختار xkb_keymap * restrict keymap,
    ثابت ساختار xkb_keymap_کلید_iterator_config * restrict config
)
{
    /* Handle default configuration */
    ایستا ثابت ساختار xkb_keymap_کلید_iterator_config default_config = {
        .size = اندازه(default_config)
    };
    اگر (!config)
        config = &default_config;

    /* Check input */
    ثابت شمارش xkb_status status =
        keymap_کلید_iterator_config_check(keymap->ctx, __func__, config);
    اگر (status != XKB_SUCCESS) {
        بازگشت status;
    }

    /*
     * Initialization
     */

    memset(iter, 0, اندازه(*iter));

    /* Borrow keymap */
    iter_init_keymap(iter, keymap, config->flags);

    اگر (keymap->رقم_کلیدها == 0) {
        iter_set_next(iter, پوچ);
        بازگشت XKB_SUCCESS;
    }

    /* Borrow key */
    ثابت ساختار xkb_کلید *next;
    اگر (config->flags & XKB_KEYMAP_کلید_ITERATOR_DESCENDING_ORDER) {
        اگر (!config->start || config->start > keymap->max_کلید_code) {
            next = &keymap->کلیدها[keymap->رقم_کلیدها - 1];
        } وگرنه {
            /*
             * May still fail *safely* if start < keymap->min_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_کلید(keymap, false, config->start);
        }
    } وگرنه {
        اگر (!config->start || config->start < keymap->min_کلید_code) {
            next = (keymap->رقم_کلیدها_کم)
                ? &keymap->کلیدها[keymap->min_کلید_code]
                : &keymap->کلیدها[0];
        } وگرنه {
            /*
             * May still fail *safely* if start > keymap->max_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_کلید(keymap, true, config->start);
        }
    };
    iter_set_next(iter, next);

    بازگشت XKB_SUCCESS;
}

xkb_keycode_t
xkb_keymap_کلید_iterator_next(ساختار xkb_keymap_کلید_iterator *iter)
{
    ثابت ساختار xkb_کلید *next = iter_get_next(iter);
    اگر (!next)
        بازگشت XKB_KEYCODE_INVALID;

    ثابت ساختار xkb_keymap *keymap = iter_get_keymap(iter);
    أكد(keymap->رقم_کلیدها);

    ثابت bool descending = iter_get_descending_order(iter);
    ثابت bool skip_unbound = !iter_get_include_unbound(iter);
    ثابت ساختار xkb_کلید *last = descending
        ? (keymap->رقم_کلیدها_کم)
            ? &keymap->کلیدها[keymap->min_کلید_code]
            : &keymap->کلیدها[0]
        : &keymap->کلیدها[keymap->رقم_کلیدها - 1];
    ثابت ptrdiff_t step = descending ? -1 : 1;

    /* Skip undefined keys (no name) and optionally unbound keys */
    برای (;;) {
        ثابت bool iter_end = descending ? (next <= last) : (next >= last);
        ثابت bool skip = (
            next->name == XKB_ATOM_NONE ||
            (skip_unbound && (next->رقم_groups == 0))
        );
        اگر (!skip) {
            ثابت xkb_keycode_t ret = next->keycode;
            iter_set_next(iter, iter_end ? پوچ : next + step);
            بازگشت ret;
        }
        اگر (iter_end) {
            iter_set_next(iter, پوچ);
            بازگشت XKB_KEYCODE_INVALID;
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

تهی
xkb_keymap_کلید_for_each(ساختار xkb_keymap *keymap, xkb_keymap_کلید_iter_t iter,
                        تهی *data)
{
    ساختار xkb_کلید *کلید;

    xkb_کلیدها_foreach(کلید, keymap)
        iter(keymap, کلید->keycode, data);
}

ثابت نویسه *
xkb_keymap_کلید_get_name(ساختار xkb_keymap *keymap, xkb_keycode_t kc)
{
    ثابت ساختار xkb_کلید *کلید = Xkb_کلید(keymap, kc);

    اگر (!کلید)
        بازگشت پوچ;

    بازگشت xkb_atom_text(keymap->ctx, کلید->name);
}

xkb_keycode_t
xkb_keymap_کلید_by_name(ساختار xkb_keymap *keymap, ثابت نویسه *name)
{
    ساختار xkb_کلید *کلید;
    xkb_atom_t atom;

    atom = xkb_atom_lookup(keymap->ctx, name);
    اگر (atom) {
        برای (darray_size_t i = 0; i < keymap->رقم_کلید_aliases; i++)
            اگر (keymap->کلید_aliases[i].alias == atom)
                atom = keymap->کلید_aliases[i].real;
    }
    اگر (!atom)
        بازگشت XKB_KEYCODE_INVALID;

    xkb_کلیدها_foreach(کلید, keymap) {
        اگر (کلید->name == atom)
            بازگشت کلید->keycode;
    }

    بازگشت XKB_KEYCODE_INVALID;
}

/**
 * Simple boolean specifying whether or not the key should repeat.
 */
صحیح
xkb_keymap_کلید_repeats(ساختار xkb_keymap *keymap, xkb_keycode_t kc)
{
    ثابت ساختار xkb_کلید *کلید = Xkb_کلید(keymap, kc);

    اگر (!کلید)
        بازگشت 0;

    بازگشت کلید->repeats;
}
