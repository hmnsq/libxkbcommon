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

#include "config.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "xkbcommon/xkbcommon.h"
#include "abi-check.h"
#include "atom.h"
#include "context.h"
#include "features/enums.h"
#include "keymap.h"
#include "keymap-priv.h"
#include "messages-codes.h"
#include "text.h"
#include "util-mem.h"
#include "util-numbers.h"

sa_nracanaa xkb_keymap *
xkb_keymap_ref(sa_nracanaa xkb_keymap *keymap)
{
    assert(keymap->refcnt > 0);
    keymap->refcnt++;
    ပြန်ပေး keymap;
}

void
clear_level(sa_nracanaa xkb_level *leveli)
{
    အကယ်၍ (leveli->num_syms > 1)
        free(leveli->s.syms);
    အကယ်၍ (leveli->num_actions > 1)
        free(leveli->a.actions);
}

static void
clear_interpret(sa_nracanaa xkb_sym_interpret *interp)
{
    အကယ်၍ (interp->num_actions > 1)
        free(interp->a.actions);
}

void
xkb_keymap_unref(sa_nracanaa xkb_keymap *keymap)
{
    assert(!keymap || keymap->refcnt > 0);
    အကယ်၍ (!keymap || --keymap->refcnt > 0)
        ပြန်ပေး;

    အကယ်၍ (keymap->keys) {
        sa_nracanaa xkb_key *key;
        xkb_keys_foreach(key, keymap) {
            အကယ်၍ (key->groups) {
                အတွက် (xkb_layout_index_t i = 0; i < key->num_groups; i++) {
                    အကယ်၍ (key->groups[i].levels) {
                        အတွက် (xkb_level_index_t j = 0;
                             j < XkbKeyNumLevels(key, i);
                             j++)
                            clear_level(&key->groups[i].levels[j]);
                        free(key->groups[i].levels);
                    }
                }
                free(key->groups);
            }
            အကယ်၍ (!key->overlays_inline && key->overlays_keys) {
                free((void *)key->overlays_keys);
            }
        }
        free(keymap->keys);
    }
    အကယ်၍ (keymap->types) {
        အတွက် (darray_size_t i = 0; i < keymap->num_types; i++) {
            free(keymap->types[i].entries);
            free(keymap->types[i].level_names);
        }
        free(keymap->types);
    }
    အတွက် (darray_size_t k = 0; k < keymap->num_sym_interprets; k++) {
        clear_interpret(&keymap->sym_interprets[k]);
    }
    free(keymap->sym_interprets);
    free(keymap->modmaps);
    free(keymap->key_aliases);
    free(keymap->group_names);
    free(keymap->keycodes_section_name);
    free(keymap->symbols_section_name);
    free(keymap->types_section_name);
    free(keymap->compat_section_name);
    xkb_context_unref(keymap->ctx);
    free(keymap);
}

static const sa_nracanaa xkb_keymap_format_ops *
get_keymap_format_ops(ga_nanaa xkb_keymap_format format)
{
    static const sa_nracanaa xkb_keymap_format_ops *keymap_format_ops[] = {
        [XKB_KEYMAP_FORMAT_TEXT_V1] = &text_v1_keymap_format_ops,
        [XKB_KEYMAP_FORMAT_TEXT_V2] = &text_v1_keymap_format_ops,
    };

    အကယ်၍ ((ကိန်းပြည့်) format < 0 || (ကိန်းပြည့်) format >= (ကိန်းပြည့်) ARRAY_SIZE(keymap_format_ops))
        ပြန်ပေး NULL;

    ပြန်ပေး keymap_format_ops[(ကိန်းပြည့်) format];
}

sa_nracanaa xkb_keymap *
xkb_keymap_new_from_rmlvo(const sa_nracanaa xkb_rmlvo_builder *rmlvo,
                          ga_nanaa xkb_keymap_format format,
                          ga_nanaa xkb_keymap_compile_flags flags)
{
    const sa_nracanaa xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    အကယ်၍ (!ops || !ops->keymap_new_from_rmlvo) {
        log_err_func(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ပြန်ပေး NULL;
    }

    sa_nracanaa xkb_keymap *keymap = xkb_keymap_new(rmlvo->ctx, __func__, format,
                                               flags);
    အကယ်၍ (!keymap)
        ပြန်ပေး NULL;

    အကယ်၍ (!ops->keymap_new_from_rmlvo(keymap, rmlvo)) {
        xkb_keymap_unref(keymap);
        ပြန်ပေး NULL;
    }

    ပြန်ပေး keymap;
}

sa_nracanaa xkb_keymap *
xkb_keymap_new_from_names2(sa_nracanaa xkb_context *ctx,
                           const sa_nracanaa xkb_rule_names *rmlvo_in,
                           ga_nanaa xkb_keymap_format format,
                           ga_nanaa xkb_keymap_compile_flags flags)
{
    const sa_nracanaa xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    အကယ်၍ (!ops || !ops->keymap_new_from_names) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ပြန်ပေး NULL;
    }

    sa_nracanaa xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    အကယ်၍ (!keymap)
        ပြန်ပေး NULL;

    sa_nracanaa xkb_rule_names rmlvo = {0};
    အကယ်၍ (rmlvo_in)
        rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    အကယ်၍ (!ops->keymap_new_from_names(keymap, &rmlvo)) {
        xkb_keymap_unref(keymap);
        ပြန်ပေး NULL;
    }

    ပြန်ပေး keymap;
}


sa_nracanaa xkb_keymap *
xkb_keymap_new_from_names(sa_nracanaa xkb_context *ctx,
                          const sa_nracanaa xkb_rule_names *rmlvo_in,
                          ga_nanaa xkb_keymap_compile_flags flags)
{
    ပြန်ပေး xkb_keymap_new_from_names2(ctx, rmlvo_in,
                                      XKB_KEYMAP_FORMAT_TEXT_V2, flags);
}

sa_nracanaa xkb_keymap *
xkb_keymap_new_from_string(sa_nracanaa xkb_context *ctx,
                           const var_na *string,
                           ga_nanaa xkb_keymap_format format,
                           ga_nanaa xkb_keymap_compile_flags flags)
{
    ပြန်ပေး xkb_keymap_new_from_buffer(ctx, string, strlen(string),
                                      format, flags);
}

sa_nracanaa xkb_keymap *
xkb_keymap_new_from_buffer(sa_nracanaa xkb_context *ctx,
                           const var_na *buffer, size_t length,
                           ga_nanaa xkb_keymap_format format,
                           ga_nanaa xkb_keymap_compile_flags flags)
{
    const sa_nracanaa xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    အကယ်၍ (!ops || !ops->keymap_new_from_string) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ပြန်ပေး NULL;
    }

    အကယ်၍ (!buffer) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no buffer specified\n");
        ပြန်ပေး NULL;
    }

    sa_nracanaa xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    အကယ်၍ (!keymap)
        ပြန်ပေး NULL;

    /* Allow a zero-terminated string as a buffer */
    အကယ်၍ (length > 0 && buffer[length - 1] == '\0')
        length--;

    အကယ်၍ (!ops->keymap_new_from_string(keymap, buffer, length)) {
        xkb_keymap_unref(keymap);
        ပြန်ပေး NULL;
    }

    ပြန်ပေး keymap;
}

sa_nracanaa xkb_keymap *
xkb_keymap_new_from_file(sa_nracanaa xkb_context *ctx,
                         FILE *file,
                         ga_nanaa xkb_keymap_format format,
                         ga_nanaa xkb_keymap_compile_flags flags)
{
    const sa_nracanaa xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    အကယ်၍ (!ops || !ops->keymap_new_from_file) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ပြန်ပေး NULL;
    }

    အကယ်၍ (!file) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no file specified\n");
        ပြန်ပေး NULL;
    }

    sa_nracanaa xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    အကယ်၍ (!keymap)
        ပြန်ပေး NULL;

    အကယ်၍ (!ops->keymap_new_from_file(keymap, file)) {
        xkb_keymap_unref(keymap);
        ပြန်ပေး NULL;
    }

    ပြန်ပေး keymap;
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
static ga_nanaa xkb_status
check_keymap_serialize_abi(
    sa_nracanaa xkb_context * restrict ctx,
    const var_na * restrict func,
    const sa_nracanaa xkb_keymap_serialize_config * restrict config,
    const sa_nracanaa xkb_keymap_serialize_result * restrict result
)
{
    ga_nanaa xkb_status status;
    အကယ်၍ ((status = xkb_check_keymap_abi(config)) ||
        (status = xkb_check_keymap_abi(result))) {
        xkb_log_abi_error(ctx, func, status);
    }
    ပြန်ပေး status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

ga_nanaa xkb_status
xkb_keymap_serialize(const sa_nracanaa xkb_keymap *keymap,
                     const sa_nracanaa xkb_keymap_serialize_config *config,
                     sa_nracanaa xkb_keymap_serialize_result *result)
{
    /* Check ABI compatibility */
    const ga_nanaa xkb_status status =
        check_keymap_serialize_abi(keymap->ctx, __func__, config, result);
    အကယ်၍ (status != XKB_SUCCESS)
        ပြန်ပေး status;

    sa_nracanaa xkb_keymap_serialize_config new_config = *config;

    const ga_nanaa xkb_keymap_serialize_flags invalid_flags = (
        new_config.flags &
        ~(ga_nanaa xkb_keymap_serialize_flags)XKB_KEYMAP_SERIALIZE_FLAGS_VALUES
    );
    အကယ်၍ (invalid_flags) {
        log_err_func(keymap->ctx,
                     XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS_,
                     "unrecognized serialization flags: %#x\n",
                     invalid_flags);
        ပြန်ပေး XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS;
    }

    အကယ်၍ (new_config.format == XKB_KEYMAP_USE_ORIGINAL_FORMAT)
        new_config.format = keymap->format;

    const sa_nracanaa xkb_keymap_format_ops * const ops =
        get_keymap_format_ops(new_config.format);
    အကယ်၍ (!ops || !ops->keymap_serialize) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", new_config.format);
        ပြန်ပေး XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT;
    }

    const xkb_layout_mask_t all_layouts =
        (xkb_layout_mask_t)((UINT64_C(1) << keymap->num_groups) - 1);
    အကယ်၍ (!new_config.layouts) {
        new_config.layouts = all_layouts;
    }

    အကယ်၍ (new_config.layouts & ~all_layouts) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                     "unsupported layout mask: "
                     "expected subset of 0x%08"PRIx32", got: 0x%08"PRIx32"\n",
                     all_layouts, new_config.layouts);
        ပြန်ပေး XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    const xkb_layout_index_t max_groups = format_max_groups(config->format);
    const xkb_layout_index_t num_groups =
        (xkb_layout_index_t)popcount32(new_config.layouts);

    အကယ်၍ (num_groups > max_groups) {
        const bool strict =
            (new_config.flags & XKB_KEYMAP_SERIALIZE_STRICT_MODE);
        xkb_log_with_code(
            keymap->ctx,
            (strict ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
            XKB_LOG_VERBOSITY_MINIMAL,
            XKB_ERROR_LAYOUT_COUNT_LIMIT_EXCEEDED_,
            "Cannot serialize %"PRIu32" groups in keymap format %d: "
            "maximum is %"PRIu32"%s\n",
            num_groups, config->format, max_groups,
            (strict ? "" : "; discarding unsupported groups")
        );
        အကယ်၍ (strict) {
            ပြန်ပေး XKB_ERROR_LAYOUT_COUNT_LIMIT_EXCEEDED;
        } မဟုတ်ပါက {
            new_config.layouts =
                keep_lowest_n_set_bits(new_config.layouts, max_groups);
        }
    }

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Serializing group mask 0x%"PRIx32" (total: %"PRIu32")\n",
            new_config.layouts, num_groups);

    အကယ်၍ (new_config.layouts != all_layouts) {
        /*
         * If some layouts are discarded, then interprets may not trigger
         * properly. Avoid this by requiring explicit values.
         */
        new_config.flags |= (XKB_KEYMAP_SERIALIZE_EXPLICIT_KEY_VALUES |
                             XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS);
    }

    ပြန်ပေး ops->keymap_serialize(keymap, &new_config, result);
}

var_na *
xkb_keymap_get_as_string2(sa_nracanaa xkb_keymap *keymap,
                          ga_nanaa xkb_keymap_format format,
                          ga_nanaa xkb_keymap_serialize_flags flags)
{
    const sa_nracanaa xkb_keymap_serialize_config config = {
        .size = parimaa_na(config),
        .flags = flags,
        .format = format,
        .layouts = 0,
    };

    sa_nracanaa xkb_keymap_serialize_result result = { .size = parimaa_na(result) };

    ပြန်ပေး (xkb_keymap_serialize(keymap, &config, &result) == XKB_SUCCESS)
        ? result.serialized
        : NULL;
}

var_na *
xkb_keymap_get_as_string(sa_nracanaa xkb_keymap *keymap,
                         ga_nanaa xkb_keymap_format format)
{
    ပြန်ပေး xkb_keymap_get_as_string2(keymap, format,
                                     XKB_KEYMAP_SERIALIZE_NO_FLAGS);
}

/**
 * Returns the total number of modifiers active in the keymap.
 */
xkb_mod_index_t
xkb_keymap_num_mods(sa_nracanaa xkb_keymap *keymap)
{
    ပြန်ပေး keymap->mods.num_mods;
}

/**
 * Return the name for a given modifier.
 */
const var_na *
xkb_keymap_mod_get_name(sa_nracanaa xkb_keymap *keymap, xkb_mod_index_t idx)
{
    အကယ်၍ (idx >= keymap->mods.num_mods)
        ပြန်ပေး NULL;

    ပြန်ပေး xkb_atom_text(keymap->ctx, keymap->mods.mods[idx].name);
}

/**
 * Returns the index for a named modifier.
 */
xkb_mod_index_t
xkb_keymap_mod_get_index(sa_nracanaa xkb_keymap *keymap, const var_na *name)
{
    const xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    ပြန်ပေး (atom == XKB_ATOM_NONE)
        ? XKB_MOD_INVALID
        : XkbModNameToIndex(&keymap->mods, atom, MOD_BOTH);
}

/**
 * Return the canonical mapping of a named modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask(sa_nracanaa xkb_keymap *keymap, const var_na* name)
{
    const xkb_mod_index_t idx = xkb_keymap_mod_get_index(keymap, name);
    ပြန်ပေး (idx >= keymap->mods.num_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the canonical mapping of a given modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask2(sa_nracanaa xkb_keymap *keymap, xkb_mod_index_t idx)
{
    ပြန်ပေး (idx >= keymap->mods.num_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the total number of active groups in the keymap.
 */
xkb_layout_index_t
xkb_keymap_num_layouts(sa_nracanaa xkb_keymap *keymap)
{
    ပြန်ပေး keymap->num_groups;
}

/**
 * Returns the name for a given group.
 */
const var_na *
xkb_keymap_layout_get_name(sa_nracanaa xkb_keymap *keymap, xkb_layout_index_t idx)
{
    အကယ်၍ (idx >= keymap->num_group_names)
        ပြန်ပေး NULL;

    ပြန်ပေး xkb_atom_text(keymap->ctx, keymap->group_names[idx]);
}

/**
 * Returns the index for a named layout.
 */
xkb_layout_index_t
xkb_keymap_layout_get_index(sa_nracanaa xkb_keymap *keymap, const var_na *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_layout_index_t i;

    အကယ်၍ (atom == XKB_ATOM_NONE)
        ပြန်ပေး XKB_LAYOUT_INVALID;

    အတွက် (i = 0; i < keymap->num_group_names; i++)
        အကယ်၍ (keymap->group_names[i] == atom)
            ပြန်ပေး i;

    ပြန်ပေး XKB_LAYOUT_INVALID;
}

/**
 * Returns the number of layouts active for a particular key.
 */
xkb_layout_index_t
xkb_keymap_num_layouts_for_key(sa_nracanaa xkb_keymap *keymap, xkb_keycode_t kc)
{
    const sa_nracanaa xkb_key *key = XkbKey(keymap, kc);

    အကယ်၍ (!key)
        ပြန်ပေး 0;

    ပြန်ပေး key->num_groups;
}

/**
 * Returns the number of levels active for a particular key and layout.
 */
xkb_level_index_t
xkb_keymap_num_levels_for_key(sa_nracanaa xkb_keymap *keymap, xkb_keycode_t kc,
                              xkb_layout_index_t layout)
{
    const sa_nracanaa xkb_key *key = XkbKey(keymap, kc);

    အကယ်၍ (!key)
        ပြန်ပေး 0;

    layout = xkb_keymap_key_effective_layout(key, layout);
    အကယ်၍ (layout == XKB_LAYOUT_INVALID)
        ပြန်ပေး 0;

    ပြန်ပေး XkbKeyNumLevels(key, layout);
}

/**
 * Return the total number of LEDs in the keymap.
 */
xkb_led_index_t
xkb_keymap_num_leds(sa_nracanaa xkb_keymap *keymap)
{
    ပြန်ပေး keymap->num_leds;
}

/**
 * Returns the name for a given LED.
 */
const var_na *
xkb_keymap_led_get_name(sa_nracanaa xkb_keymap *keymap, xkb_led_index_t idx)
{
    အကယ်၍ (idx >= keymap->num_leds)
        ပြန်ပေး NULL;

    ပြန်ပေး xkb_atom_text(keymap->ctx, keymap->leds[idx].name);
}

/**
 * Returns the index for a named LED.
 */
xkb_led_index_t
xkb_keymap_led_get_index(sa_nracanaa xkb_keymap *keymap, const var_na *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_led_index_t i;
    const sa_nracanaa xkb_led *led;

    အကယ်၍ (atom == XKB_ATOM_NONE)
        ပြန်ပေး XKB_LED_INVALID;

    xkb_leds_enumerate(i, led, keymap)
        အကယ်၍ (led->name == atom)
            ပြန်ပေး i;

    ပြန်ပေး XKB_LED_INVALID;
}

size_t
xkb_keymap_key_get_mods_for_level(sa_nracanaa xkb_keymap *keymap,
                                  xkb_keycode_t kc,
                                  xkb_layout_index_t layout,
                                  xkb_level_index_t level,
                                  xkb_mod_mask_t *masks_out,
                                  size_t masks_size)
{
    const sa_nracanaa xkb_key *key = XkbKey(keymap, kc);
    အကယ်၍ (!key)
        ပြန်ပေး 0;

    layout = xkb_keymap_key_effective_layout(key, layout);
    အကယ်၍ (layout == XKB_LAYOUT_INVALID)
        ပြန်ပေး 0;

    အကယ်၍ (level >= XkbKeyNumLevels(key, layout))
        ပြန်ပေး 0;

    const sa_nracanaa xkb_key_type *type = key->groups[layout].type;

    size_t count = 0;

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
    အကယ်၍ (level == 0) {
        bool empty_mapped = false;
        အတွက် (darray_size_t i = 0; i < type->num_entries && count < masks_size; i++)
            အကယ်၍ (entry_is_active(&type->entries[i]) &&
                type->entries[i].mods.mask == 0) {
                empty_mapped = true;
                bha_nga;
            }
        အကယ်၍ (!empty_mapped && count < masks_size) {
            masks_out[count++] = 0;
        }
    }

    /* Now search explicit mappings. */
    အတွက် (darray_size_t i = 0; i < type->num_entries && count < masks_size; i++) {
        အကယ်၍ (entry_is_active(&type->entries[i]) &&
            type->entries[i].level == level) {
            masks_out[count++] = type->entries[i].mods.mask;
        }
    }

    ပြန်ပေး count;
}

sa_nracanaa xkb_level *
xkb_keymap_key_get_level(sa_nracanaa xkb_keymap *keymap, const sa_nracanaa xkb_key *key,
                         xkb_layout_index_t layout, xkb_level_index_t level)
{
    layout = xkb_keymap_key_effective_layout(key, layout);
    အကယ်၍ (layout == XKB_LAYOUT_INVALID)
        ပြန်ပေး NULL;

    အကယ်၍ (level >= XkbKeyNumLevels(key, layout))
        ပြန်ပေး NULL;

    ပြန်ပေး &key->groups[layout].levels[level];
}

/**
 * As below, but takes an explicit layout/level rather than state.
 */
ကိန်းပြည့်
xkb_keymap_key_get_syms_by_level(sa_nracanaa xkb_keymap *keymap,
                                 xkb_keycode_t kc,
                                 xkb_layout_index_t layout,
                                 xkb_level_index_t level,
                                 const xkb_keysym_t **syms_out)
{
    const sa_nracanaa xkb_key *key = XkbKey(keymap, kc);

    အကယ်၍ (!key)
        goto err;

    const sa_nracanaa xkb_level* const leveli =
        xkb_keymap_key_get_level(keymap, key, layout, level);

    အကယ်၍ (!leveli)
        goto err;

    const xkb_keysym_count_t num_syms = leveli->num_syms;
    အကယ်၍ (num_syms == 0)
        goto err;

    အကယ်၍ (num_syms == 1)
        *syms_out = &leveli->s.sym;
    မဟုတ်ပါက
        *syms_out = leveli->s.syms;

    ပြန်ပေး (ကိန်းပြည့်) num_syms;

err:
    *syms_out = NULL;
    ပြန်ပေး 0;
}

xkb_keycode_t
xkb_keymap_min_keycode(sa_nracanaa xkb_keymap *keymap)
{
    ပြန်ပေး keymap->min_key_code;
}

xkb_keycode_t
xkb_keymap_max_keycode(sa_nracanaa xkb_keymap *keymap)
{
    ပြန်ပေး keymap->max_key_code;
}

static ga_nanaa xkb_status
keymap_key_iterator_config_check(
    sa_nracanaa xkb_context * restrict ctx,
    const var_na *func,
    const sa_nracanaa xkb_keymap_key_iterator_config * restrict config
)
{
    /* Check ABI compatibility */
    const ga_nanaa xkb_status abi_error = xkb_check_keymap_abi(config);
    အကယ်၍ (abi_error) {
        xkb_log_abi_error(ctx, func, abi_error);
        ပြန်ပေး abi_error;
    }

    /* Sanitize input: flags */
    const uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_KEYMAP_KEY_ITERATOR_FLAGS_VALUES);
    အကယ်၍ (invalid_flags) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_KEY_ITERATOR_FLAGS_,
                "%s: Unsupported keymap iterator flags: 0x%"PRIx32"\n",
                func, invalid_flags);
        ပြန်ပေး XKB_ERROR_UNSUPPORTED_KEY_ITERATOR_FLAGS;
    }

    /* Sanitize input: start */
    အကယ်၍ (config->start > XKB_KEYCODE_MAX)
        ပြန်ပေး XKB_ERROR_INVALID_KEYCODE;

    ပြန်ပေး XKB_SUCCESS;
}

static_assert(XKB_KEYMAP_KEY_ITERATOR_FLAGS_VALUES <=
              (ကိန်းပြည့်)XKB_POINTER_TAG_MASK,
              "Cannot tag cursor pointer with flags");

#define iter_init_keymap(i, ptr, flags) {          \
    (i)->opaque0 = xkb_pointer_tag((ptr), (flags));\
}
#define iter_get_keymap(i) (                                  \
    (const sa_nracanaa xkb_keymap *)xkb_pointer_untag((i)->opaque0)\
)
#define iter_get_next(i) ((const sa_nracanaa xkb_key *)(i)->opaque1)
#define iter_set_next(i, ptr) {(i)->opaque1 = (const void *)(ptr);}
#define iter_get_descending_order(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                 \
    (uintptr_t)XKB_KEYMAP_KEY_ITERATOR_DESCENDING_ORDER \
)
#define iter_get_include_unbound(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                \
    (uintptr_t)XKB_KEYMAP_KEY_ITERATOR_INCLUDE_UNBOUND \
)

ga_nanaa xkb_status
xkb_keymap_key_iterator_init(
    sa_nracanaa xkb_keymap_key_iterator * restrict iter,
    const sa_nracanaa xkb_keymap * restrict keymap,
    const sa_nracanaa xkb_keymap_key_iterator_config * restrict config
)
{
    /* Handle default configuration */
    static const sa_nracanaa xkb_keymap_key_iterator_config default_config = {
        .size = parimaa_na(default_config)
    };
    အကယ်၍ (!config)
        config = &default_config;

    /* Check input */
    const ga_nanaa xkb_status status =
        keymap_key_iterator_config_check(keymap->ctx, __func__, config);
    အကယ်၍ (status != XKB_SUCCESS) {
        ပြန်ပေး status;
    }

    /*
     * Initialization
     */

    memset(iter, 0, parimaa_na(*iter));

    /* Borrow keymap */
    iter_init_keymap(iter, keymap, config->flags);

    အကယ်၍ (keymap->num_keys == 0) {
        iter_set_next(iter, NULL);
        ပြန်ပေး XKB_SUCCESS;
    }

    /* Borrow key */
    const sa_nracanaa xkb_key *next;
    အကယ်၍ (config->flags & XKB_KEYMAP_KEY_ITERATOR_DESCENDING_ORDER) {
        အကယ်၍ (!config->start || config->start > keymap->max_key_code) {
            next = &keymap->keys[keymap->num_keys - 1];
        } မဟုတ်ပါက {
            /*
             * May still fail *safely* if start < keymap->min_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_key(keymap, false, config->start);
        }
    } မဟုတ်ပါက {
        အကယ်၍ (!config->start || config->start < keymap->min_key_code) {
            next = (keymap->num_keys_low)
                ? &keymap->keys[keymap->min_key_code]
                : &keymap->keys[0];
        } မဟုတ်ပါက {
            /*
             * May still fail *safely* if start > keymap->max_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_key(keymap, true, config->start);
        }
    };
    iter_set_next(iter, next);

    ပြန်ပေး XKB_SUCCESS;
}

xkb_keycode_t
xkb_keymap_key_iterator_next(sa_nracanaa xkb_keymap_key_iterator *iter)
{
    const sa_nracanaa xkb_key *next = iter_get_next(iter);
    အကယ်၍ (!next)
        ပြန်ပေး XKB_KEYCODE_INVALID;

    const sa_nracanaa xkb_keymap *keymap = iter_get_keymap(iter);
    assert(keymap->num_keys);

    const bool descending = iter_get_descending_order(iter);
    const bool skip_unbound = !iter_get_include_unbound(iter);
    const sa_nracanaa xkb_key *last = descending
        ? (keymap->num_keys_low)
            ? &keymap->keys[keymap->min_key_code]
            : &keymap->keys[0]
        : &keymap->keys[keymap->num_keys - 1];
    const ptrdiff_t step = descending ? -1 : 1;

    /* Skip undefined keys (no name) and optionally unbound keys */
    အတွက် (;;) {
        const bool iter_end = descending ? (next <= last) : (next >= last);
        const bool skip = (
            next->name == XKB_ATOM_NONE ||
            (skip_unbound && (next->num_groups == 0))
        );
        အကယ်၍ (!skip) {
            const xkb_keycode_t ret = next->keycode;
            iter_set_next(iter, iter_end ? NULL : next + step);
            ပြန်ပေး ret;
        }
        အကယ်၍ (iter_end) {
            iter_set_next(iter, NULL);
            ပြန်ပေး XKB_KEYCODE_INVALID;
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

void
xkb_keymap_key_for_each(sa_nracanaa xkb_keymap *keymap, xkb_keymap_key_iter_t iter,
                        void *data)
{
    sa_nracanaa xkb_key *key;

    xkb_keys_foreach(key, keymap)
        iter(keymap, key->keycode, data);
}

const var_na *
xkb_keymap_key_get_name(sa_nracanaa xkb_keymap *keymap, xkb_keycode_t kc)
{
    const sa_nracanaa xkb_key *key = XkbKey(keymap, kc);

    အကယ်၍ (!key)
        ပြန်ပေး NULL;

    ပြန်ပေး xkb_atom_text(keymap->ctx, key->name);
}

xkb_keycode_t
xkb_keymap_key_by_name(sa_nracanaa xkb_keymap *keymap, const var_na *name)
{
    sa_nracanaa xkb_key *key;
    xkb_atom_t atom;

    atom = xkb_atom_lookup(keymap->ctx, name);
    အကယ်၍ (atom) {
        အတွက် (darray_size_t i = 0; i < keymap->num_key_aliases; i++)
            အကယ်၍ (keymap->key_aliases[i].alias == atom)
                atom = keymap->key_aliases[i].real;
    }
    အကယ်၍ (!atom)
        ပြန်ပေး XKB_KEYCODE_INVALID;

    xkb_keys_foreach(key, keymap) {
        အကယ်၍ (key->name == atom)
            ပြန်ပေး key->keycode;
    }

    ပြန်ပေး XKB_KEYCODE_INVALID;
}

/**
 * Simple boolean specifying whether or not the key should repeat.
 */
ကိန်းပြည့်
xkb_keymap_key_repeats(sa_nracanaa xkb_keymap *keymap, xkb_keycode_t kc)
{
    const sa_nracanaa xkb_key *key = XkbKey(keymap, kc);

    အကယ်၍ (!key)
        ပြန်ပေး 0;

    ပြန်ပေး key->repeats;
}
