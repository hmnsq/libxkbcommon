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

#समावेश "config.h"

#समावेश <assert.h>
#समावेश <stdint.h>
#समावेश <string.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "abi-check.h"
#समावेश "atom.h"
#समावेश "context.h"
#समावेश "features/enums.h"
#समावेश "keymap.h"
#समावेश "keymap-priv.h"
#समावेश "messages-codes.h"
#समावेश "text.h"
#समावेश "util-mem.h"
#समावेश "util-numbers.h"

काष्ठा xkb_keymap *
xkb_keymap_ref(काष्ठा xkb_keymap *keymap)
{
    निश्चित(keymap->refcnt > 0);
    keymap->refcnt++;
    वापस keymap;
}

व्योम
clear_level(काष्ठा xkb_level *leveli)
{
    अगर (leveli->अंक_syms > 1)
        मुक्त(leveli->s.syms);
    अगर (leveli->अंक_actions > 1)
        मुक्त(leveli->a.actions);
}

अटल व्योम
clear_interpret(काष्ठा xkb_sym_interpret *interp)
{
    अगर (interp->अंक_actions > 1)
        मुक्त(interp->a.actions);
}

व्योम
xkb_keymap_unref(काष्ठा xkb_keymap *keymap)
{
    निश्चित(!keymap || keymap->refcnt > 0);
    अगर (!keymap || --keymap->refcnt > 0)
        वापस;

    अगर (keymap->कुंजियाँ) {
        काष्ठा xkb_कुंजी *कुंजी;
        xkb_कुंजियाँ_foreach(कुंजी, keymap) {
            अगर (कुंजी->groups) {
                क्रम (xkb_layout_सूचकांक_t i = 0; i < कुंजी->अंक_groups; i++) {
                    अगर (कुंजी->groups[i].levels) {
                        क्रम (xkb_level_सूचकांक_t j = 0;
                             j < Xkb_कुंजी_अंक_Levels(कुंजी, i);
                             j++)
                            clear_level(&कुंजी->groups[i].levels[j]);
                        मुक्त(कुंजी->groups[i].levels);
                    }
                }
                मुक्त(कुंजी->groups);
            }
            अगर (!कुंजी->overlays_inline && कुंजी->overlays_कुंजियाँ) {
                मुक्त((व्योम *)कुंजी->overlays_कुंजियाँ);
            }
        }
        मुक्त(keymap->कुंजियाँ);
    }
    अगर (keymap->types) {
        क्रम (darray_आकार_t i = 0; i < keymap->अंक_types; i++) {
            मुक्त(keymap->types[i].entries);
            मुक्त(keymap->types[i].level_names);
        }
        मुक्त(keymap->types);
    }
    क्रम (darray_आकार_t k = 0; k < keymap->अंक_sym_interprets; k++) {
        clear_interpret(&keymap->sym_interprets[k]);
    }
    मुक्त(keymap->sym_interprets);
    मुक्त(keymap->modmaps);
    मुक्त(keymap->कुंजी_aliases);
    मुक्त(keymap->group_names);
    मुक्त(keymap->keycodes_section_name);
    मुक्त(keymap->symbols_section_name);
    मुक्त(keymap->types_section_name);
    मुक्त(keymap->compat_section_name);
    xkb_context_unref(keymap->ctx);
    मुक्त(keymap);
}

अटल स्थिर काष्ठा xkb_keymap_format_ops *
get_keymap_format_ops(क्रमागत xkb_keymap_format format)
{
    अटल स्थिर काष्ठा xkb_keymap_format_ops *keymap_format_ops[] = {
        [XKB_KEYMAP_FORMAT_TEXT_V1] = &text_v1_keymap_format_ops,
        [XKB_KEYMAP_FORMAT_TEXT_V2] = &text_v1_keymap_format_ops,
    };

    अगर ((पूर्णांक) format < 0 || (पूर्णांक) format >= (पूर्णांक) ARRAY_आकार(keymap_format_ops))
        वापस शून्य;

    वापस keymap_format_ops[(पूर्णांक) format];
}

काष्ठा xkb_keymap *
xkb_keymap_new_from_rmlvo(स्थिर काष्ठा xkb_rmlvo_builder *rmlvo,
                          क्रमागत xkb_keymap_format format,
                          क्रमागत xkb_keymap_compile_flags flags)
{
    स्थिर काष्ठा xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    अगर (!ops || !ops->keymap_new_from_rmlvo) {
        log_err_func(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        वापस शून्य;
    }

    काष्ठा xkb_keymap *keymap = xkb_keymap_new(rmlvo->ctx, __func__, format,
                                               flags);
    अगर (!keymap)
        वापस शून्य;

    अगर (!ops->keymap_new_from_rmlvo(keymap, rmlvo)) {
        xkb_keymap_unref(keymap);
        वापस शून्य;
    }

    वापस keymap;
}

काष्ठा xkb_keymap *
xkb_keymap_new_from_names2(काष्ठा xkb_context *ctx,
                           स्थिर काष्ठा xkb_rule_names *rmlvo_in,
                           क्रमागत xkb_keymap_format format,
                           क्रमागत xkb_keymap_compile_flags flags)
{
    स्थिर काष्ठा xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    अगर (!ops || !ops->keymap_new_from_names) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        वापस शून्य;
    }

    काष्ठा xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    अगर (!keymap)
        वापस शून्य;

    काष्ठा xkb_rule_names rmlvo = {0};
    अगर (rmlvo_in)
        rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    अगर (!ops->keymap_new_from_names(keymap, &rmlvo)) {
        xkb_keymap_unref(keymap);
        वापस शून्य;
    }

    वापस keymap;
}


काष्ठा xkb_keymap *
xkb_keymap_new_from_names(काष्ठा xkb_context *ctx,
                          स्थिर काष्ठा xkb_rule_names *rmlvo_in,
                          क्रमागत xkb_keymap_compile_flags flags)
{
    वापस xkb_keymap_new_from_names2(ctx, rmlvo_in,
                                      XKB_KEYMAP_FORMAT_TEXT_V2, flags);
}

काष्ठा xkb_keymap *
xkb_keymap_new_from_string(काष्ठा xkb_context *ctx,
                           स्थिर अक्षर *string,
                           क्रमागत xkb_keymap_format format,
                           क्रमागत xkb_keymap_compile_flags flags)
{
    वापस xkb_keymap_new_from_बफर(ctx, string, म_माप(string),
                                      format, flags);
}

काष्ठा xkb_keymap *
xkb_keymap_new_from_बफर(काष्ठा xkb_context *ctx,
                           स्थिर अक्षर *बफर, माप_प्रकार लंबाई,
                           क्रमागत xkb_keymap_format format,
                           क्रमागत xkb_keymap_compile_flags flags)
{
    स्थिर काष्ठा xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    अगर (!ops || !ops->keymap_new_from_string) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        वापस शून्य;
    }

    अगर (!बफर) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no buffer specified\n");
        वापस शून्य;
    }

    काष्ठा xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    अगर (!keymap)
        वापस शून्य;

    /* Allow a zero-terminated string as a buffer */
    अगर (लंबाई > 0 && बफर[लंबाई - 1] == '\0')
        लंबाई--;

    अगर (!ops->keymap_new_from_string(keymap, बफर, लंबाई)) {
        xkb_keymap_unref(keymap);
        वापस शून्य;
    }

    वापस keymap;
}

काष्ठा xkb_keymap *
xkb_keymap_new_from_file(काष्ठा xkb_context *ctx,
                         खाता *file,
                         क्रमागत xkb_keymap_format format,
                         क्रमागत xkb_keymap_compile_flags flags)
{
    स्थिर काष्ठा xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    अगर (!ops || !ops->keymap_new_from_file) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        वापस शून्य;
    }

    अगर (!file) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no file specified\n");
        वापस शून्य;
    }

    काष्ठा xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    अगर (!keymap)
        वापस शून्य;

    अगर (!ops->keymap_new_from_file(keymap, file)) {
        xkb_keymap_unref(keymap);
        वापस शून्य;
    }

    वापस keymap;
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
अटल क्रमागत xkb_status
check_keymap_serialize_abi(
    काष्ठा xkb_context * restrict ctx,
    स्थिर अक्षर * restrict func,
    स्थिर काष्ठा xkb_keymap_serialize_config * restrict config,
    स्थिर काष्ठा xkb_keymap_serialize_परिणाम * restrict परिणाम
)
{
    क्रमागत xkb_status status;
    अगर ((status = xkb_check_keymap_abi(config)) ||
        (status = xkb_check_keymap_abi(परिणाम))) {
        xkb_log_abi_error(ctx, func, status);
    }
    वापस status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

क्रमागत xkb_status
xkb_keymap_serialize(स्थिर काष्ठा xkb_keymap *keymap,
                     स्थिर काष्ठा xkb_keymap_serialize_config *config,
                     काष्ठा xkb_keymap_serialize_परिणाम *परिणाम)
{
    /* Check ABI compatibility */
    स्थिर क्रमागत xkb_status status =
        check_keymap_serialize_abi(keymap->ctx, __func__, config, परिणाम);
    अगर (status != XKB_SUCCESS)
        वापस status;

    काष्ठा xkb_keymap_serialize_config new_config = *config;

    स्थिर क्रमागत xkb_keymap_serialize_flags invalid_flags = (
        new_config.flags &
        ~(क्रमागत xkb_keymap_serialize_flags)XKB_KEYMAP_SERIALIZE_FLAGS_मूल्यावली
    );
    अगर (invalid_flags) {
        log_err_func(keymap->ctx,
                     XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS_,
                     "unrecognized serialization flags: %#x\n",
                     invalid_flags);
        वापस XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS;
    }

    अगर (new_config.format == XKB_KEYMAP_USE_ORIGINAL_FORMAT)
        new_config.format = keymap->format;

    स्थिर काष्ठा xkb_keymap_format_ops * स्थिर ops =
        get_keymap_format_ops(new_config.format);
    अगर (!ops || !ops->keymap_serialize) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", new_config.format);
        वापस XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT;
    }

    स्थिर xkb_layout_mask_t all_layouts =
        (xkb_layout_mask_t)((UINT64_C(1) << keymap->अंक_groups) - 1);
    अगर (!new_config.layouts) {
        new_config.layouts = all_layouts;
    }

    अगर (new_config.layouts & ~all_layouts) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                     "unsupported layout mask: "
                     "expected subset of 0x%08"PRIx32", got: 0x%08"PRIx32"\n",
                     all_layouts, new_config.layouts);
        वापस XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक;
    }

    स्थिर xkb_layout_सूचकांक_t max_groups = format_max_groups(config->format);
    स्थिर xkb_layout_सूचकांक_t अंक_groups =
        (xkb_layout_सूचकांक_t)popcount32(new_config.layouts);

    अगर (अंक_groups > max_groups) {
        स्थिर bool strict =
            (new_config.flags & XKB_KEYMAP_SERIALIZE_STRICT_MODE);
        xkb_log_with_code(
            keymap->ctx,
            (strict ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
            XKB_LOG_VERBOSITY_MINIMAL,
            XKB_ERROR_LAYOUT_गिनती_LIMIT_EXCEEDED_2,
            "Cannot serialize %"PRIu32" groups in keymap format %d: "
            "maximum is %"PRIu32"%s\n",
            अंक_groups, config->format, max_groups,
            (strict ? "" : "; discarding unsupported groups")
        );
        अगर (strict) {
            वापस XKB_ERROR_LAYOUT_गिनती_LIMIT_EXCEEDED;
        } अन्यथा {
            new_config.layouts =
                keep_lowest_n_set_bits(new_config.layouts, max_groups);
        }
    }

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Serializing group mask 0x%"PRIx32" (total: %"PRIu32")\n",
            new_config.layouts, अंक_groups);

    अगर (new_config.layouts != all_layouts) {
        /*
         * If some layouts are discarded, then interprets may not trigger
         * properly. Avoid this by requiring explicit values.
         */
        new_config.flags |= (XKB_KEYMAP_SERIALIZE_EXPLICIT_कुंजी_मूल्यावली |
                             XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS);
    }

    वापस ops->keymap_serialize(keymap, &new_config, परिणाम);
}

अक्षर *
xkb_keymap_get_as_string2(काष्ठा xkb_keymap *keymap,
                          क्रमागत xkb_keymap_format format,
                          क्रमागत xkb_keymap_serialize_flags flags)
{
    स्थिर काष्ठा xkb_keymap_serialize_config config = {
        .आकार = माप(config),
        .flags = flags,
        .format = format,
        .layouts = 0,
    };

    काष्ठा xkb_keymap_serialize_परिणाम परिणाम = { .आकार = माप(परिणाम) };

    वापस (xkb_keymap_serialize(keymap, &config, &परिणाम) == XKB_SUCCESS)
        ? परिणाम.serialized
        : शून्य;
}

अक्षर *
xkb_keymap_get_as_string(काष्ठा xkb_keymap *keymap,
                         क्रमागत xkb_keymap_format format)
{
    वापस xkb_keymap_get_as_string2(keymap, format,
                                     XKB_KEYMAP_SERIALIZE_NO_FLAGS);
}

/**
 * Returns the total number of modifiers active in the keymap.
 */
xkb_mod_सूचकांक_t
xkb_keymap_अंक_mods(काष्ठा xkb_keymap *keymap)
{
    वापस keymap->mods.अंक_mods;
}

/**
 * Return the name for a given modifier.
 */
स्थिर अक्षर *
xkb_keymap_mod_get_name(काष्ठा xkb_keymap *keymap, xkb_mod_सूचकांक_t idx)
{
    अगर (idx >= keymap->mods.अंक_mods)
        वापस शून्य;

    वापस xkb_atom_text(keymap->ctx, keymap->mods.mods[idx].name);
}

/**
 * Returns the index for a named modifier.
 */
xkb_mod_सूचकांक_t
xkb_keymap_mod_get_सूचकांक(काष्ठा xkb_keymap *keymap, स्थिर अक्षर *name)
{
    स्थिर xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    वापस (atom == XKB_ATOM_NONE)
        ? XKB_MOD_INVALID
        : Xkb_Mod_Name_To_सूचकांक(&keymap->mods, atom, MOD_BOTH);
}

/**
 * Return the canonical mapping of a named modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask(काष्ठा xkb_keymap *keymap, स्थिर अक्षर* name)
{
    स्थिर xkb_mod_सूचकांक_t idx = xkb_keymap_mod_get_सूचकांक(keymap, name);
    वापस (idx >= keymap->mods.अंक_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the canonical mapping of a given modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask2(काष्ठा xkb_keymap *keymap, xkb_mod_सूचकांक_t idx)
{
    वापस (idx >= keymap->mods.अंक_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the total number of active groups in the keymap.
 */
xkb_layout_सूचकांक_t
xkb_keymap_अंक_layouts(काष्ठा xkb_keymap *keymap)
{
    वापस keymap->अंक_groups;
}

/**
 * Returns the name for a given group.
 */
स्थिर अक्षर *
xkb_keymap_layout_get_name(काष्ठा xkb_keymap *keymap, xkb_layout_सूचकांक_t idx)
{
    अगर (idx >= keymap->अंक_group_names)
        वापस शून्य;

    वापस xkb_atom_text(keymap->ctx, keymap->group_names[idx]);
}

/**
 * Returns the index for a named layout.
 */
xkb_layout_सूचकांक_t
xkb_keymap_layout_get_सूचकांक(काष्ठा xkb_keymap *keymap, स्थिर अक्षर *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_layout_सूचकांक_t i;

    अगर (atom == XKB_ATOM_NONE)
        वापस XKB_LAYOUT_INVALID;

    क्रम (i = 0; i < keymap->अंक_group_names; i++)
        अगर (keymap->group_names[i] == atom)
            वापस i;

    वापस XKB_LAYOUT_INVALID;
}

/**
 * Returns the number of layouts active for a particular key.
 */
xkb_layout_सूचकांक_t
xkb_keymap_अंक_layouts_for_कुंजी(काष्ठा xkb_keymap *keymap, xkb_keycode_t kc)
{
    स्थिर काष्ठा xkb_कुंजी *कुंजी = Xkb_कुंजी(keymap, kc);

    अगर (!कुंजी)
        वापस 0;

    वापस कुंजी->अंक_groups;
}

/**
 * Returns the number of levels active for a particular key and layout.
 */
xkb_level_सूचकांक_t
xkb_keymap_अंक_levels_for_कुंजी(काष्ठा xkb_keymap *keymap, xkb_keycode_t kc,
                              xkb_layout_सूचकांक_t layout)
{
    स्थिर काष्ठा xkb_कुंजी *कुंजी = Xkb_कुंजी(keymap, kc);

    अगर (!कुंजी)
        वापस 0;

    layout = xkb_keymap_कुंजी_effective_layout(कुंजी, layout);
    अगर (layout == XKB_LAYOUT_INVALID)
        वापस 0;

    वापस Xkb_कुंजी_अंक_Levels(कुंजी, layout);
}

/**
 * Return the total number of LEDs in the keymap.
 */
xkb_led_सूचकांक_t
xkb_keymap_अंक_leds(काष्ठा xkb_keymap *keymap)
{
    वापस keymap->अंक_leds;
}

/**
 * Returns the name for a given LED.
 */
स्थिर अक्षर *
xkb_keymap_led_get_name(काष्ठा xkb_keymap *keymap, xkb_led_सूचकांक_t idx)
{
    अगर (idx >= keymap->अंक_leds)
        वापस शून्य;

    वापस xkb_atom_text(keymap->ctx, keymap->leds[idx].name);
}

/**
 * Returns the index for a named LED.
 */
xkb_led_सूचकांक_t
xkb_keymap_led_get_सूचकांक(काष्ठा xkb_keymap *keymap, स्थिर अक्षर *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_led_सूचकांक_t i;
    स्थिर काष्ठा xkb_led *led;

    अगर (atom == XKB_ATOM_NONE)
        वापस XKB_LED_INVALID;

    xkb_leds_enumerate(i, led, keymap)
        अगर (led->name == atom)
            वापस i;

    वापस XKB_LED_INVALID;
}

माप_प्रकार
xkb_keymap_कुंजी_get_mods_for_level(काष्ठा xkb_keymap *keymap,
                                  xkb_keycode_t kc,
                                  xkb_layout_सूचकांक_t layout,
                                  xkb_level_सूचकांक_t level,
                                  xkb_mod_mask_t *masks_out,
                                  माप_प्रकार masks_आकार)
{
    स्थिर काष्ठा xkb_कुंजी *कुंजी = Xkb_कुंजी(keymap, kc);
    अगर (!कुंजी)
        वापस 0;

    layout = xkb_keymap_कुंजी_effective_layout(कुंजी, layout);
    अगर (layout == XKB_LAYOUT_INVALID)
        वापस 0;

    अगर (level >= Xkb_कुंजी_अंक_Levels(कुंजी, layout))
        वापस 0;

    स्थिर काष्ठा xkb_कुंजी_type *type = कुंजी->groups[layout].type;

    माप_प्रकार गिनती = 0;

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
    अगर (level == 0) {
        bool empty_mapped = false;
        क्रम (darray_आकार_t i = 0; i < type->अंक_entries && गिनती < masks_आकार; i++)
            अगर (entry_is_active(&type->entries[i]) &&
                type->entries[i].mods.mask == 0) {
                empty_mapped = true;
                अवरोध;
            }
        अगर (!empty_mapped && गिनती < masks_आकार) {
            masks_out[गिनती++] = 0;
        }
    }

    /* Now search explicit mappings. */
    क्रम (darray_आकार_t i = 0; i < type->अंक_entries && गिनती < masks_आकार; i++) {
        अगर (entry_is_active(&type->entries[i]) &&
            type->entries[i].level == level) {
            masks_out[गिनती++] = type->entries[i].mods.mask;
        }
    }

    वापस गिनती;
}

काष्ठा xkb_level *
xkb_keymap_कुंजी_get_level(काष्ठा xkb_keymap *keymap, स्थिर काष्ठा xkb_कुंजी *कुंजी,
                         xkb_layout_सूचकांक_t layout, xkb_level_सूचकांक_t level)
{
    layout = xkb_keymap_कुंजी_effective_layout(कुंजी, layout);
    अगर (layout == XKB_LAYOUT_INVALID)
        वापस शून्य;

    अगर (level >= Xkb_कुंजी_अंक_Levels(कुंजी, layout))
        वापस शून्य;

    वापस &कुंजी->groups[layout].levels[level];
}

/**
 * As below, but takes an explicit layout/level rather than state.
 */
पूर्णांक
xkb_keymap_कुंजी_get_syms_by_level(काष्ठा xkb_keymap *keymap,
                                 xkb_keycode_t kc,
                                 xkb_layout_सूचकांक_t layout,
                                 xkb_level_सूचकांक_t level,
                                 स्थिर xkb_keysym_t **syms_out)
{
    स्थिर काष्ठा xkb_कुंजी *कुंजी = Xkb_कुंजी(keymap, kc);

    अगर (!कुंजी)
        जाओ err;

    स्थिर काष्ठा xkb_level* स्थिर leveli =
        xkb_keymap_कुंजी_get_level(keymap, कुंजी, layout, level);

    अगर (!leveli)
        जाओ err;

    स्थिर xkb_keysym_गिनती_t अंक_syms = leveli->अंक_syms;
    अगर (अंक_syms == 0)
        जाओ err;

    अगर (अंक_syms == 1)
        *syms_out = &leveli->s.sym;
    अन्यथा
        *syms_out = leveli->s.syms;

    वापस (पूर्णांक) अंक_syms;

err:
    *syms_out = शून्य;
    वापस 0;
}

xkb_keycode_t
xkb_keymap_min_keycode(काष्ठा xkb_keymap *keymap)
{
    वापस keymap->min_कुंजी_code;
}

xkb_keycode_t
xkb_keymap_max_keycode(काष्ठा xkb_keymap *keymap)
{
    वापस keymap->max_कुंजी_code;
}

अटल क्रमागत xkb_status
keymap_कुंजी_iterator_config_check(
    काष्ठा xkb_context * restrict ctx,
    स्थिर अक्षर *func,
    स्थिर काष्ठा xkb_keymap_कुंजी_iterator_config * restrict config
)
{
    /* Check ABI compatibility */
    स्थिर क्रमागत xkb_status abi_error = xkb_check_keymap_abi(config);
    अगर (abi_error) {
        xkb_log_abi_error(ctx, func, abi_error);
        वापस abi_error;
    }

    /* Sanitize input: flags */
    स्थिर uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_KEYMAP_कुंजी_ITERATOR_FLAGS_मूल्यावली);
    अगर (invalid_flags) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_कुंजी_ITERATOR_FLAGS_2,
                "%s: Unsupported keymap iterator flags: 0x%"PRIx32"\n",
                func, invalid_flags);
        वापस XKB_ERROR_UNSUPPORTED_कुंजी_ITERATOR_FLAGS;
    }

    /* Sanitize input: start */
    अगर (config->start > XKB_KEYCODE_MAX)
        वापस XKB_ERROR_INVALID_KEYCODE;

    वापस XKB_SUCCESS;
}

static_assert(XKB_KEYMAP_कुंजी_ITERATOR_FLAGS_मूल्यावली <=
              (पूर्णांक)XKB_POINTER_TAG_MASK,
              "Cannot tag cursor pointer with flags");

#घोषणा iter_init_keymap(i, ptr, flags) {          \
    (i)->opaque0 = xkb_pointer_tag((ptr), (flags));\
}
#घोषणा iter_get_keymap(i) (                                  \
    (स्थिर काष्ठा xkb_keymap *)xkb_pointer_untag((i)->opaque0)\
)
#घोषणा iter_get_अग्र(i) ((स्थिर काष्ठा xkb_कुंजी *)(i)->opaque1)
#घोषणा iter_set_अग्र(i, ptr) {(i)->opaque1 = (स्थिर व्योम *)(ptr);}
#घोषणा iter_get_descending_order(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                 \
    (uintptr_t)XKB_KEYMAP_कुंजी_ITERATOR_DESCENDING_ORDER \
)
#घोषणा iter_get_include_unbound(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                \
    (uintptr_t)XKB_KEYMAP_कुंजी_ITERATOR_INCLUDE_UNBOUND \
)

क्रमागत xkb_status
xkb_keymap_कुंजी_iterator_init(
    काष्ठा xkb_keymap_कुंजी_iterator * restrict iter,
    स्थिर काष्ठा xkb_keymap * restrict keymap,
    स्थिर काष्ठा xkb_keymap_कुंजी_iterator_config * restrict config
)
{
    /* Handle default configuration */
    अटल स्थिर काष्ठा xkb_keymap_कुंजी_iterator_config default_config = {
        .आकार = माप(default_config)
    };
    अगर (!config)
        config = &default_config;

    /* Check input */
    स्थिर क्रमागत xkb_status status =
        keymap_कुंजी_iterator_config_check(keymap->ctx, __func__, config);
    अगर (status != XKB_SUCCESS) {
        वापस status;
    }

    /*
     * Initialization
     */

    स_रखो(iter, 0, माप(*iter));

    /* Borrow keymap */
    iter_init_keymap(iter, keymap, config->flags);

    अगर (keymap->अंक_कुंजियाँ == 0) {
        iter_set_अग्र(iter, शून्य);
        वापस XKB_SUCCESS;
    }

    /* Borrow key */
    स्थिर काष्ठा xkb_कुंजी *अग्र;
    अगर (config->flags & XKB_KEYMAP_कुंजी_ITERATOR_DESCENDING_ORDER) {
        अगर (!config->start || config->start > keymap->max_कुंजी_code) {
            अग्र = &keymap->कुंजियाँ[keymap->अंक_कुंजियाँ - 1];
        } अन्यथा {
            /*
             * May still fail *safely* if start < keymap->min_key_code:
             * there will be simply no key to iterate over.
             */
            अग्र = xkb_keymap_get_अग्र_defined_कुंजी(keymap, false, config->start);
        }
    } अन्यथा {
        अगर (!config->start || config->start < keymap->min_कुंजी_code) {
            अग्र = (keymap->अंक_कुंजियाँ_निचला)
                ? &keymap->कुंजियाँ[keymap->min_कुंजी_code]
                : &keymap->कुंजियाँ[0];
        } अन्यथा {
            /*
             * May still fail *safely* if start > keymap->max_key_code:
             * there will be simply no key to iterate over.
             */
            अग्र = xkb_keymap_get_अग्र_defined_कुंजी(keymap, true, config->start);
        }
    };
    iter_set_अग्र(iter, अग्र);

    वापस XKB_SUCCESS;
}

xkb_keycode_t
xkb_keymap_कुंजी_iterator_अग्र(काष्ठा xkb_keymap_कुंजी_iterator *iter)
{
    स्थिर काष्ठा xkb_कुंजी *अग्र = iter_get_अग्र(iter);
    अगर (!अग्र)
        वापस XKB_KEYCODE_INVALID;

    स्थिर काष्ठा xkb_keymap *keymap = iter_get_keymap(iter);
    निश्चित(keymap->अंक_कुंजियाँ);

    स्थिर bool descending = iter_get_descending_order(iter);
    स्थिर bool skip_unbound = !iter_get_include_unbound(iter);
    स्थिर काष्ठा xkb_कुंजी *last = descending
        ? (keymap->अंक_कुंजियाँ_निचला)
            ? &keymap->कुंजियाँ[keymap->min_कुंजी_code]
            : &keymap->कुंजियाँ[0]
        : &keymap->कुंजियाँ[keymap->अंक_कुंजियाँ - 1];
    स्थिर सूचक_भेद_प्रकार step = descending ? -1 : 1;

    /* Skip undefined keys (no name) and optionally unbound keys */
    क्रम (;;) {
        स्थिर bool iter_end = descending ? (अग्र <= last) : (अग्र >= last);
        स्थिर bool skip = (
            अग्र->name == XKB_ATOM_NONE ||
            (skip_unbound && (अग्र->अंक_groups == 0))
        );
        अगर (!skip) {
            स्थिर xkb_keycode_t ret = अग्र->keycode;
            iter_set_अग्र(iter, iter_end ? शून्य : अग्र + step);
            वापस ret;
        }
        अगर (iter_end) {
            iter_set_अग्र(iter, शून्य);
            वापस XKB_KEYCODE_INVALID;
        }
        अग्र += step;
    }
}

#अघोषित iter_init_keymap
#अघोषित iter_get_keymap
#अघोषित iter_set_अग्र
#अघोषित iter_get_अग्र
#अघोषित iter_get_descending_order
#अघोषित iter_get_include_unbound

व्योम
xkb_keymap_कुंजी_for_each(काष्ठा xkb_keymap *keymap, xkb_keymap_कुंजी_iter_t iter,
                        व्योम *तथ्य)
{
    काष्ठा xkb_कुंजी *कुंजी;

    xkb_कुंजियाँ_foreach(कुंजी, keymap)
        iter(keymap, कुंजी->keycode, तथ्य);
}

स्थिर अक्षर *
xkb_keymap_कुंजी_get_name(काष्ठा xkb_keymap *keymap, xkb_keycode_t kc)
{
    स्थिर काष्ठा xkb_कुंजी *कुंजी = Xkb_कुंजी(keymap, kc);

    अगर (!कुंजी)
        वापस शून्य;

    वापस xkb_atom_text(keymap->ctx, कुंजी->name);
}

xkb_keycode_t
xkb_keymap_कुंजी_by_name(काष्ठा xkb_keymap *keymap, स्थिर अक्षर *name)
{
    काष्ठा xkb_कुंजी *कुंजी;
    xkb_atom_t atom;

    atom = xkb_atom_lookup(keymap->ctx, name);
    अगर (atom) {
        क्रम (darray_आकार_t i = 0; i < keymap->अंक_कुंजी_aliases; i++)
            अगर (keymap->कुंजी_aliases[i].alias == atom)
                atom = keymap->कुंजी_aliases[i].real;
    }
    अगर (!atom)
        वापस XKB_KEYCODE_INVALID;

    xkb_कुंजियाँ_foreach(कुंजी, keymap) {
        अगर (कुंजी->name == atom)
            वापस कुंजी->keycode;
    }

    वापस XKB_KEYCODE_INVALID;
}

/**
 * Simple boolean specifying whether or not the key should repeat.
 */
पूर्णांक
xkb_keymap_कुंजी_repeats(काष्ठा xkb_keymap *keymap, xkb_keycode_t kc)
{
    स्थिर काष्ठा xkb_कुंजी *कुंजी = Xkb_कुंजी(keymap, kc);

    अगर (!कुंजी)
        वापस 0;

    वापस कुंजी->repeats;
}
