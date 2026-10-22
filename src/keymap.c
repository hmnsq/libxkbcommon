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

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <stdint.h>
#અન્તર્ભાવ <string.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "abi-check.h"
#અન્તર્ભાવ "atom.h"
#અન્તર્ભાવ "context.h"
#અન્તર્ભાવ "features/enums.h"
#અન્તર્ભાવ "keymap.h"
#અન્તર્ભાવ "keymap-priv.h"
#અન્તર્ભાવ "messages-codes.h"
#અન્તર્ભાવ "text.h"
#અન્તર્ભાવ "util-mem.h"
#અન્તર્ભાવ "util-numbers.h"

સંરચના xkb_keymap *
xkb_keymap_ref(સંરચના xkb_keymap *keymap)
{
    પ્રતિજ્ઞા(keymap->refcnt > 0);
    keymap->refcnt++;
    પાછા keymap;
}

શૂન્ય
clear_level(સંરચના xkb_level *leveli)
{
    જો (leveli->અંક_syms > 1)
        મુક્ત(leveli->s.syms);
    જો (leveli->અંક_actions > 1)
        મુક્ત(leveli->a.actions);
}

સ્થિર શૂન્ય
clear_interpret(સંરચના xkb_sym_interpret *interp)
{
    જો (interp->અંક_actions > 1)
        મુક્ત(interp->a.actions);
}

શૂન્ય
xkb_keymap_unref(સંરચના xkb_keymap *keymap)
{
    પ્રતિજ્ઞા(!keymap || keymap->refcnt > 0);
    જો (!keymap || --keymap->refcnt > 0)
        પાછા;

    જો (keymap->ચાવીઓ) {
        સંરચના xkb_ચાવી *ચાવી;
        xkb_ચાવીઓ_foreach(ચાવી, keymap) {
            જો (ચાવી->groups) {
                માટે (xkb_layout_index_t i = 0; i < ચાવી->અંક_groups; i++) {
                    જો (ચાવી->groups[i].levels) {
                        માટે (xkb_level_index_t j = 0;
                             j < Xkb_ચાવી_અંક_Levels(ચાવી, i);
                             j++)
                            clear_level(&ચાવી->groups[i].levels[j]);
                        મુક્ત(ચાવી->groups[i].levels);
                    }
                }
                મુક્ત(ચાવી->groups);
            }
            જો (!ચાવી->overlays_inline && ચાવી->overlays_ચાવીઓ) {
                મુક્ત((શૂન્ય *)ચાવી->overlays_ચાવીઓ);
            }
        }
        મુક્ત(keymap->ચાવીઓ);
    }
    જો (keymap->types) {
        માટે (darray_size_t i = 0; i < keymap->અંક_types; i++) {
            મુક્ત(keymap->types[i].entries);
            મુક્ત(keymap->types[i].level_names);
        }
        મુક્ત(keymap->types);
    }
    માટે (darray_size_t k = 0; k < keymap->અંક_sym_interprets; k++) {
        clear_interpret(&keymap->sym_interprets[k]);
    }
    મુક્ત(keymap->sym_interprets);
    મુક્ત(keymap->modmaps);
    મુક્ત(keymap->ચાવી_aliases);
    મુક્ત(keymap->group_names);
    મુક્ત(keymap->keycodes_section_name);
    મુક્ત(keymap->symbols_section_name);
    મુક્ત(keymap->types_section_name);
    મુક્ત(keymap->compat_section_name);
    xkb_context_unref(keymap->ctx);
    મુક્ત(keymap);
}

સ્થિર અચલ સંરચના xkb_keymap_format_ops *
get_keymap_format_ops(ગણના xkb_keymap_format format)
{
    સ્થિર અચલ સંરચના xkb_keymap_format_ops *keymap_format_ops[] = {
        [XKB_KEYMAP_FORMAT_TEXT_V1] = &text_v1_keymap_format_ops,
        [XKB_KEYMAP_FORMAT_TEXT_V2] = &text_v1_keymap_format_ops,
    };

    જો ((પૂર્ણાંક) format < 0 || (પૂર્ણાંક) format >= (પૂર્ણાંક) ARRAY_SIZE(keymap_format_ops))
        પાછા ખાલી;

    પાછા keymap_format_ops[(પૂર્ણાંક) format];
}

સંરચના xkb_keymap *
xkb_keymap_new_from_rmlvo(અચલ સંરચના xkb_rmlvo_builder *rmlvo,
                          ગણના xkb_keymap_format format,
                          ગણના xkb_keymap_compile_flags flags)
{
    અચલ સંરચના xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    જો (!ops || !ops->keymap_new_from_rmlvo) {
        log_err_func(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        પાછા ખાલી;
    }

    સંરચના xkb_keymap *keymap = xkb_keymap_new(rmlvo->ctx, __func__, format,
                                               flags);
    જો (!keymap)
        પાછા ખાલી;

    જો (!ops->keymap_new_from_rmlvo(keymap, rmlvo)) {
        xkb_keymap_unref(keymap);
        પાછા ખાલી;
    }

    પાછા keymap;
}

સંરચના xkb_keymap *
xkb_keymap_new_from_names2(સંરચના xkb_context *ctx,
                           અચલ સંરચના xkb_rule_names *rmlvo_in,
                           ગણના xkb_keymap_format format,
                           ગણના xkb_keymap_compile_flags flags)
{
    અચલ સંરચના xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    જો (!ops || !ops->keymap_new_from_names) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        પાછા ખાલી;
    }

    સંરચના xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    જો (!keymap)
        પાછા ખાલી;

    સંરચના xkb_rule_names rmlvo = {0};
    જો (rmlvo_in)
        rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    જો (!ops->keymap_new_from_names(keymap, &rmlvo)) {
        xkb_keymap_unref(keymap);
        પાછા ખાલી;
    }

    પાછા keymap;
}


સંરચના xkb_keymap *
xkb_keymap_new_from_names(સંરચના xkb_context *ctx,
                          અચલ સંરચના xkb_rule_names *rmlvo_in,
                          ગણના xkb_keymap_compile_flags flags)
{
    પાછા xkb_keymap_new_from_names2(ctx, rmlvo_in,
                                      XKB_KEYMAP_FORMAT_TEXT_V2, flags);
}

સંરચના xkb_keymap *
xkb_keymap_new_from_string(સંરચના xkb_context *ctx,
                           અચલ અક્ષર *string,
                           ગણના xkb_keymap_format format,
                           ગણના xkb_keymap_compile_flags flags)
{
    પાછા xkb_keymap_new_from_buffer(ctx, string, strlen(string),
                                      format, flags);
}

સંરચના xkb_keymap *
xkb_keymap_new_from_buffer(સંરચના xkb_context *ctx,
                           અચલ અક્ષર *buffer, size_t length,
                           ગણના xkb_keymap_format format,
                           ગણના xkb_keymap_compile_flags flags)
{
    અચલ સંરચના xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    જો (!ops || !ops->keymap_new_from_string) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        પાછા ખાલી;
    }

    જો (!buffer) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no buffer specified\n");
        પાછા ખાલી;
    }

    સંરચના xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    જો (!keymap)
        પાછા ખાલી;

    /* Allow a zero-terminated string as a buffer */
    જો (length > 0 && buffer[length - 1] == '\0')
        length--;

    જો (!ops->keymap_new_from_string(keymap, buffer, length)) {
        xkb_keymap_unref(keymap);
        પાછા ખાલી;
    }

    પાછા keymap;
}

સંરચના xkb_keymap *
xkb_keymap_new_from_file(સંરચના xkb_context *ctx,
                         FILE *file,
                         ગણના xkb_keymap_format format,
                         ગણના xkb_keymap_compile_flags flags)
{
    અચલ સંરચના xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    જો (!ops || !ops->keymap_new_from_file) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        પાછા ખાલી;
    }

    જો (!file) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no file specified\n");
        પાછા ખાલી;
    }

    સંરચના xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    જો (!keymap)
        પાછા ખાલી;

    જો (!ops->keymap_new_from_file(keymap, file)) {
        xkb_keymap_unref(keymap);
        પાછા ખાલી;
    }

    પાછા keymap;
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
સ્થિર ગણના xkb_status
check_keymap_serialize_abi(
    સંરચના xkb_context * restrict ctx,
    અચલ અક્ષર * restrict func,
    અચલ સંરચના xkb_keymap_serialize_config * restrict config,
    અચલ સંરચના xkb_keymap_serialize_result * restrict result
)
{
    ગણના xkb_status status;
    જો ((status = xkb_check_keymap_abi(config)) ||
        (status = xkb_check_keymap_abi(result))) {
        xkb_log_abi_error(ctx, func, status);
    }
    પાછા status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

ગણના xkb_status
xkb_keymap_serialize(અચલ સંરચના xkb_keymap *keymap,
                     અચલ સંરચના xkb_keymap_serialize_config *config,
                     સંરચના xkb_keymap_serialize_result *result)
{
    /* Check ABI compatibility */
    અચલ ગણના xkb_status status =
        check_keymap_serialize_abi(keymap->ctx, __func__, config, result);
    જો (status != XKB_SUCCESS)
        પાછા status;

    સંરચના xkb_keymap_serialize_config new_config = *config;

    અચલ ગણના xkb_keymap_serialize_flags invalid_flags = (
        new_config.flags &
        ~(ગણના xkb_keymap_serialize_flags)XKB_KEYMAP_SERIALIZE_FLAGS_કિંમતો
    );
    જો (invalid_flags) {
        log_err_func(keymap->ctx,
                     XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS_,
                     "unrecognized serialization flags: %#x\n",
                     invalid_flags);
        પાછા XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS;
    }

    જો (new_config.format == XKB_KEYMAP_USE_ORIGINAL_FORMAT)
        new_config.format = keymap->format;

    અચલ સંરચના xkb_keymap_format_ops * અચલ ops =
        get_keymap_format_ops(new_config.format);
    જો (!ops || !ops->keymap_serialize) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", new_config.format);
        પાછા XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT;
    }

    અચલ xkb_layout_mask_t all_layouts =
        (xkb_layout_mask_t)((UINT64_C(1) << keymap->અંક_groups) - 1);
    જો (!new_config.layouts) {
        new_config.layouts = all_layouts;
    }

    જો (new_config.layouts & ~all_layouts) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                     "unsupported layout mask: "
                     "expected subset of 0x%08"PRIx32", got: 0x%08"PRIx32"\n",
                     all_layouts, new_config.layouts);
        પાછા XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    અચલ xkb_layout_index_t max_groups = format_max_groups(config->format);
    અચલ xkb_layout_index_t અંક_groups =
        (xkb_layout_index_t)popcount32(new_config.layouts);

    જો (અંક_groups > max_groups) {
        અચલ bool strict =
            (new_config.flags & XKB_KEYMAP_SERIALIZE_STRICT_MODE);
        xkb_log_with_code(
            keymap->ctx,
            (strict ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
            XKB_LOG_VERBOSITY_MINIMAL,
            XKB_ERROR_LAYOUT_ગણતરી_LIMIT_EXCEEDED_2,
            "Cannot serialize %"PRIu32" groups in keymap format %d: "
            "maximum is %"PRIu32"%s\n",
            અંક_groups, config->format, max_groups,
            (strict ? "" : "; discarding unsupported groups")
        );
        જો (strict) {
            પાછા XKB_ERROR_LAYOUT_ગણતરી_LIMIT_EXCEEDED;
        } નહીંતર {
            new_config.layouts =
                keep_lowest_n_set_bits(new_config.layouts, max_groups);
        }
    }

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Serializing group mask 0x%"PRIx32" (total: %"PRIu32")\n",
            new_config.layouts, અંક_groups);

    જો (new_config.layouts != all_layouts) {
        /*
         * If some layouts are discarded, then interprets may not trigger
         * properly. Avoid this by requiring explicit values.
         */
        new_config.flags |= (XKB_KEYMAP_SERIALIZE_EXPLICIT_ચાવી_કિંમતો |
                             XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS);
    }

    પાછા ops->keymap_serialize(keymap, &new_config, result);
}

અક્ષર *
xkb_keymap_get_as_string2(સંરચના xkb_keymap *keymap,
                          ગણના xkb_keymap_format format,
                          ગણના xkb_keymap_serialize_flags flags)
{
    અચલ સંરચના xkb_keymap_serialize_config config = {
        .size = કદ(config),
        .flags = flags,
        .format = format,
        .layouts = 0,
    };

    સંરચના xkb_keymap_serialize_result result = { .size = કદ(result) };

    પાછા (xkb_keymap_serialize(keymap, &config, &result) == XKB_SUCCESS)
        ? result.serialized
        : ખાલી;
}

અક્ષર *
xkb_keymap_get_as_string(સંરચના xkb_keymap *keymap,
                         ગણના xkb_keymap_format format)
{
    પાછા xkb_keymap_get_as_string2(keymap, format,
                                     XKB_KEYMAP_SERIALIZE_NO_FLAGS);
}

/**
 * Returns the total number of modifiers active in the keymap.
 */
xkb_mod_index_t
xkb_keymap_અંક_mods(સંરચના xkb_keymap *keymap)
{
    પાછા keymap->mods.અંક_mods;
}

/**
 * Return the name for a given modifier.
 */
અચલ અક્ષર *
xkb_keymap_mod_get_name(સંરચના xkb_keymap *keymap, xkb_mod_index_t idx)
{
    જો (idx >= keymap->mods.અંક_mods)
        પાછા ખાલી;

    પાછા xkb_atom_text(keymap->ctx, keymap->mods.mods[idx].name);
}

/**
 * Returns the index for a named modifier.
 */
xkb_mod_index_t
xkb_keymap_mod_get_index(સંરચના xkb_keymap *keymap, અચલ અક્ષર *name)
{
    અચલ xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    પાછા (atom == XKB_ATOM_NONE)
        ? XKB_MOD_INVALID
        : XkbModNameToIndex(&keymap->mods, atom, MOD_BOTH);
}

/**
 * Return the canonical mapping of a named modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask(સંરચના xkb_keymap *keymap, અચલ અક્ષર* name)
{
    અચલ xkb_mod_index_t idx = xkb_keymap_mod_get_index(keymap, name);
    પાછા (idx >= keymap->mods.અંક_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the canonical mapping of a given modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask2(સંરચના xkb_keymap *keymap, xkb_mod_index_t idx)
{
    પાછા (idx >= keymap->mods.અંક_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the total number of active groups in the keymap.
 */
xkb_layout_index_t
xkb_keymap_અંક_layouts(સંરચના xkb_keymap *keymap)
{
    પાછા keymap->અંક_groups;
}

/**
 * Returns the name for a given group.
 */
અચલ અક્ષર *
xkb_keymap_layout_get_name(સંરચના xkb_keymap *keymap, xkb_layout_index_t idx)
{
    જો (idx >= keymap->અંક_group_names)
        પાછા ખાલી;

    પાછા xkb_atom_text(keymap->ctx, keymap->group_names[idx]);
}

/**
 * Returns the index for a named layout.
 */
xkb_layout_index_t
xkb_keymap_layout_get_index(સંરચના xkb_keymap *keymap, અચલ અક્ષર *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_layout_index_t i;

    જો (atom == XKB_ATOM_NONE)
        પાછા XKB_LAYOUT_INVALID;

    માટે (i = 0; i < keymap->અંક_group_names; i++)
        જો (keymap->group_names[i] == atom)
            પાછા i;

    પાછા XKB_LAYOUT_INVALID;
}

/**
 * Returns the number of layouts active for a particular key.
 */
xkb_layout_index_t
xkb_keymap_અંક_layouts_for_ચાવી(સંરચના xkb_keymap *keymap, xkb_keycode_t kc)
{
    અચલ સંરચના xkb_ચાવી *ચાવી = Xkb_ચાવી(keymap, kc);

    જો (!ચાવી)
        પાછા 0;

    પાછા ચાવી->અંક_groups;
}

/**
 * Returns the number of levels active for a particular key and layout.
 */
xkb_level_index_t
xkb_keymap_અંક_levels_for_ચાવી(સંરચના xkb_keymap *keymap, xkb_keycode_t kc,
                              xkb_layout_index_t layout)
{
    અચલ સંરચના xkb_ચાવી *ચાવી = Xkb_ચાવી(keymap, kc);

    જો (!ચાવી)
        પાછા 0;

    layout = xkb_keymap_ચાવી_effective_layout(ચાવી, layout);
    જો (layout == XKB_LAYOUT_INVALID)
        પાછા 0;

    પાછા Xkb_ચાવી_અંક_Levels(ચાવી, layout);
}

/**
 * Return the total number of LEDs in the keymap.
 */
xkb_led_index_t
xkb_keymap_અંક_leds(સંરચના xkb_keymap *keymap)
{
    પાછા keymap->અંક_leds;
}

/**
 * Returns the name for a given LED.
 */
અચલ અક્ષર *
xkb_keymap_led_get_name(સંરચના xkb_keymap *keymap, xkb_led_index_t idx)
{
    જો (idx >= keymap->અંક_leds)
        પાછા ખાલી;

    પાછા xkb_atom_text(keymap->ctx, keymap->leds[idx].name);
}

/**
 * Returns the index for a named LED.
 */
xkb_led_index_t
xkb_keymap_led_get_index(સંરચના xkb_keymap *keymap, અચલ અક્ષર *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_led_index_t i;
    અચલ સંરચના xkb_led *led;

    જો (atom == XKB_ATOM_NONE)
        પાછા XKB_LED_INVALID;

    xkb_leds_enumerate(i, led, keymap)
        જો (led->name == atom)
            પાછા i;

    પાછા XKB_LED_INVALID;
}

size_t
xkb_keymap_ચાવી_get_mods_for_level(સંરચના xkb_keymap *keymap,
                                  xkb_keycode_t kc,
                                  xkb_layout_index_t layout,
                                  xkb_level_index_t level,
                                  xkb_mod_mask_t *masks_out,
                                  size_t masks_size)
{
    અચલ સંરચના xkb_ચાવી *ચાવી = Xkb_ચાવી(keymap, kc);
    જો (!ચાવી)
        પાછા 0;

    layout = xkb_keymap_ચાવી_effective_layout(ચાવી, layout);
    જો (layout == XKB_LAYOUT_INVALID)
        પાછા 0;

    જો (level >= Xkb_ચાવી_અંક_Levels(ચાવી, layout))
        પાછા 0;

    અચલ સંરચના xkb_ચાવી_type *type = ચાવી->groups[layout].type;

    size_t ગણતરી = 0;

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
    જો (level == 0) {
        bool empty_mapped = false;
        માટે (darray_size_t i = 0; i < type->અંક_entries && ગણતરી < masks_size; i++)
            જો (entry_is_active(&type->entries[i]) &&
                type->entries[i].mods.mask == 0) {
                empty_mapped = true;
                તોડો;
            }
        જો (!empty_mapped && ગણતરી < masks_size) {
            masks_out[ગણતરી++] = 0;
        }
    }

    /* Now search explicit mappings. */
    માટે (darray_size_t i = 0; i < type->અંક_entries && ગણતરી < masks_size; i++) {
        જો (entry_is_active(&type->entries[i]) &&
            type->entries[i].level == level) {
            masks_out[ગણતરી++] = type->entries[i].mods.mask;
        }
    }

    પાછા ગણતરી;
}

સંરચના xkb_level *
xkb_keymap_ચાવી_get_level(સંરચના xkb_keymap *keymap, અચલ સંરચના xkb_ચાવી *ચાવી,
                         xkb_layout_index_t layout, xkb_level_index_t level)
{
    layout = xkb_keymap_ચાવી_effective_layout(ચાવી, layout);
    જો (layout == XKB_LAYOUT_INVALID)
        પાછા ખાલી;

    જો (level >= Xkb_ચાવી_અંક_Levels(ચાવી, layout))
        પાછા ખાલી;

    પાછા &ચાવી->groups[layout].levels[level];
}

/**
 * As below, but takes an explicit layout/level rather than state.
 */
પૂર્ણાંક
xkb_keymap_ચાવી_get_syms_by_level(સંરચના xkb_keymap *keymap,
                                 xkb_keycode_t kc,
                                 xkb_layout_index_t layout,
                                 xkb_level_index_t level,
                                 અચલ xkb_keysym_t **syms_out)
{
    અચલ સંરચના xkb_ચાવી *ચાવી = Xkb_ચાવી(keymap, kc);

    જો (!ચાવી)
        જાઓ err;

    અચલ સંરચના xkb_level* અચલ leveli =
        xkb_keymap_ચાવી_get_level(keymap, ચાવી, layout, level);

    જો (!leveli)
        જાઓ err;

    અચલ xkb_keysym_ગણતરી_t અંક_syms = leveli->અંક_syms;
    જો (અંક_syms == 0)
        જાઓ err;

    જો (અંક_syms == 1)
        *syms_out = &leveli->s.sym;
    નહીંતર
        *syms_out = leveli->s.syms;

    પાછા (પૂર્ણાંક) અંક_syms;

err:
    *syms_out = ખાલી;
    પાછા 0;
}

xkb_keycode_t
xkb_keymap_min_keycode(સંરચના xkb_keymap *keymap)
{
    પાછા keymap->min_ચાવી_code;
}

xkb_keycode_t
xkb_keymap_max_keycode(સંરચના xkb_keymap *keymap)
{
    પાછા keymap->max_ચાવી_code;
}

સ્થિર ગણના xkb_status
keymap_ચાવી_iterator_config_check(
    સંરચના xkb_context * restrict ctx,
    અચલ અક્ષર *func,
    અચલ સંરચના xkb_keymap_ચાવી_iterator_config * restrict config
)
{
    /* Check ABI compatibility */
    અચલ ગણના xkb_status abi_error = xkb_check_keymap_abi(config);
    જો (abi_error) {
        xkb_log_abi_error(ctx, func, abi_error);
        પાછા abi_error;
    }

    /* Sanitize input: flags */
    અચલ uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_KEYMAP_ચાવી_ITERATOR_FLAGS_કિંમતો);
    જો (invalid_flags) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_ચાવી_ITERATOR_FLAGS_2,
                "%s: Unsupported keymap iterator flags: 0x%"PRIx32"\n",
                func, invalid_flags);
        પાછા XKB_ERROR_UNSUPPORTED_ચાવી_ITERATOR_FLAGS;
    }

    /* Sanitize input: start */
    જો (config->start > XKB_KEYCODE_MAX)
        પાછા XKB_ERROR_INVALID_KEYCODE;

    પાછા XKB_SUCCESS;
}

static_assert(XKB_KEYMAP_ચાવી_ITERATOR_FLAGS_કિંમતો <=
              (પૂર્ણાંક)XKB_POINTER_TAG_MASK,
              "Cannot tag cursor pointer with flags");

#define iter_init_keymap(i, ptr, flags) {          \
    (i)->opaque0 = xkb_pointer_tag((ptr), (flags));\
}
#define iter_get_keymap(i) (                                  \
    (અચલ સંરચના xkb_keymap *)xkb_pointer_untag((i)->opaque0)\
)
#define iter_get_next(i) ((અચલ સંરચના xkb_ચાવી *)(i)->opaque1)
#define iter_set_next(i, ptr) {(i)->opaque1 = (અચલ શૂન્ય *)(ptr);}
#define iter_get_descending_order(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                 \
    (uintptr_t)XKB_KEYMAP_ચાવી_ITERATOR_DESCENDING_ORDER \
)
#define iter_get_include_unbound(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                \
    (uintptr_t)XKB_KEYMAP_ચાવી_ITERATOR_INCLUDE_UNBOUND \
)

ગણના xkb_status
xkb_keymap_ચાવી_iterator_init(
    સંરચના xkb_keymap_ચાવી_iterator * restrict iter,
    અચલ સંરચના xkb_keymap * restrict keymap,
    અચલ સંરચના xkb_keymap_ચાવી_iterator_config * restrict config
)
{
    /* Handle default configuration */
    સ્થિર અચલ સંરચના xkb_keymap_ચાવી_iterator_config default_config = {
        .size = કદ(default_config)
    };
    જો (!config)
        config = &default_config;

    /* Check input */
    અચલ ગણના xkb_status status =
        keymap_ચાવી_iterator_config_check(keymap->ctx, __func__, config);
    જો (status != XKB_SUCCESS) {
        પાછા status;
    }

    /*
     * Initialization
     */

    memset(iter, 0, કદ(*iter));

    /* Borrow keymap */
    iter_init_keymap(iter, keymap, config->flags);

    જો (keymap->અંક_ચાવીઓ == 0) {
        iter_set_next(iter, ખાલી);
        પાછા XKB_SUCCESS;
    }

    /* Borrow key */
    અચલ સંરચના xkb_ચાવી *next;
    જો (config->flags & XKB_KEYMAP_ચાવી_ITERATOR_DESCENDING_ORDER) {
        જો (!config->start || config->start > keymap->max_ચાવી_code) {
            next = &keymap->ચાવીઓ[keymap->અંક_ચાવીઓ - 1];
        } નહીંતર {
            /*
             * May still fail *safely* if start < keymap->min_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_ચાવી(keymap, false, config->start);
        }
    } નહીંતર {
        જો (!config->start || config->start < keymap->min_ચાવી_code) {
            next = (keymap->અંક_ચાવીઓ_નીચલું)
                ? &keymap->ચાવીઓ[keymap->min_ચાવી_code]
                : &keymap->ચાવીઓ[0];
        } નહીંતર {
            /*
             * May still fail *safely* if start > keymap->max_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_ચાવી(keymap, true, config->start);
        }
    };
    iter_set_next(iter, next);

    પાછા XKB_SUCCESS;
}

xkb_keycode_t
xkb_keymap_ચાવી_iterator_next(સંરચના xkb_keymap_ચાવી_iterator *iter)
{
    અચલ સંરચના xkb_ચાવી *next = iter_get_next(iter);
    જો (!next)
        પાછા XKB_KEYCODE_INVALID;

    અચલ સંરચના xkb_keymap *keymap = iter_get_keymap(iter);
    પ્રતિજ્ઞા(keymap->અંક_ચાવીઓ);

    અચલ bool descending = iter_get_descending_order(iter);
    અચલ bool skip_unbound = !iter_get_include_unbound(iter);
    અચલ સંરચના xkb_ચાવી *last = descending
        ? (keymap->અંક_ચાવીઓ_નીચલું)
            ? &keymap->ચાવીઓ[keymap->min_ચાવી_code]
            : &keymap->ચાવીઓ[0]
        : &keymap->ચાવીઓ[keymap->અંક_ચાવીઓ - 1];
    અચલ ptrdiff_t step = descending ? -1 : 1;

    /* Skip undefined keys (no name) and optionally unbound keys */
    માટે (;;) {
        અચલ bool iter_end = descending ? (next <= last) : (next >= last);
        અચલ bool skip = (
            next->name == XKB_ATOM_NONE ||
            (skip_unbound && (next->અંક_groups == 0))
        );
        જો (!skip) {
            અચલ xkb_keycode_t ret = next->keycode;
            iter_set_next(iter, iter_end ? ખાલી : next + step);
            પાછા ret;
        }
        જો (iter_end) {
            iter_set_next(iter, ખાલી);
            પાછા XKB_KEYCODE_INVALID;
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

શૂન્ય
xkb_keymap_ચાવી_for_each(સંરચના xkb_keymap *keymap, xkb_keymap_ચાવી_iter_t iter,
                        શૂન્ય *data)
{
    સંરચના xkb_ચાવી *ચાવી;

    xkb_ચાવીઓ_foreach(ચાવી, keymap)
        iter(keymap, ચાવી->keycode, data);
}

અચલ અક્ષર *
xkb_keymap_ચાવી_get_name(સંરચના xkb_keymap *keymap, xkb_keycode_t kc)
{
    અચલ સંરચના xkb_ચાવી *ચાવી = Xkb_ચાવી(keymap, kc);

    જો (!ચાવી)
        પાછા ખાલી;

    પાછા xkb_atom_text(keymap->ctx, ચાવી->name);
}

xkb_keycode_t
xkb_keymap_ચાવી_by_name(સંરચના xkb_keymap *keymap, અચલ અક્ષર *name)
{
    સંરચના xkb_ચાવી *ચાવી;
    xkb_atom_t atom;

    atom = xkb_atom_lookup(keymap->ctx, name);
    જો (atom) {
        માટે (darray_size_t i = 0; i < keymap->અંક_ચાવી_aliases; i++)
            જો (keymap->ચાવી_aliases[i].alias == atom)
                atom = keymap->ચાવી_aliases[i].real;
    }
    જો (!atom)
        પાછા XKB_KEYCODE_INVALID;

    xkb_ચાવીઓ_foreach(ચાવી, keymap) {
        જો (ચાવી->name == atom)
            પાછા ચાવી->keycode;
    }

    પાછા XKB_KEYCODE_INVALID;
}

/**
 * Simple boolean specifying whether or not the key should repeat.
 */
પૂર્ણાંક
xkb_keymap_ચાવી_repeats(સંરચના xkb_keymap *keymap, xkb_keycode_t kc)
{
    અચલ સંરચના xkb_ચાવી *ચાવી = Xkb_ચાવી(keymap, kc);

    જો (!ચાવી)
        પાછા 0;

    પાછા ચાવી->repeats;
}
