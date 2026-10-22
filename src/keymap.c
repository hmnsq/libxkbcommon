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

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <stdint.h>
#অন্তর্ভাব <string.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "abi-check.h"
#অন্তর্ভাব "atom.h"
#অন্তর্ভাব "context.h"
#অন্তর্ভাব "features/enums.h"
#অন্তর্ভাব "keymap.h"
#অন্তর্ভাব "keymap-priv.h"
#অন্তর্ভাব "messages-codes.h"
#অন্তর্ভাব "text.h"
#অন্তর্ভাব "util-mem.h"
#অন্তর্ভাব "util-numbers.h"

কাঠামো xkb_keymap *
xkb_keymap_ref(কাঠামো xkb_keymap *keymap)
{
    প্রতিজ্ঞা(keymap->refcnt > 0);
    keymap->refcnt++;
    ফেরত keymap;
}

শূন্য
clear_level(কাঠামো xkb_level *leveli)
{
    যদি (leveli->অঙ্ক_syms > 1)
        মুক্ত(leveli->s.syms);
    যদি (leveli->অঙ্ক_actions > 1)
        মুক্ত(leveli->a.actions);
}

স্থির শূন্য
clear_interpret(কাঠামো xkb_sym_interpret *interp)
{
    যদি (interp->অঙ্ক_actions > 1)
        মুক্ত(interp->a.actions);
}

শূন্য
xkb_keymap_unref(কাঠামো xkb_keymap *keymap)
{
    প্রতিজ্ঞা(!keymap || keymap->refcnt > 0);
    যদি (!keymap || --keymap->refcnt > 0)
        ফেরত;

    যদি (keymap->চাবিগুলি) {
        কাঠামো xkb_চাবি *চাবি;
        xkb_চাবিগুলি_foreach(চাবি, keymap) {
            যদি (চাবি->groups) {
                জন্য (xkb_layout_সূচক_t i = 0; i < চাবি->অঙ্ক_groups; i++) {
                    যদি (চাবি->groups[i].levels) {
                        জন্য (xkb_level_সূচক_t j = 0;
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
        জন্য (darray_আকার_t i = 0; i < keymap->অঙ্ক_types; i++) {
            মুক্ত(keymap->types[i].entries);
            মুক্ত(keymap->types[i].level_names);
        }
        মুক্ত(keymap->types);
    }
    জন্য (darray_আকার_t k = 0; k < keymap->অঙ্ক_sym_interprets; k++) {
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

স্থির ধ্রুবক কাঠামো xkb_keymap_format_ops *
get_keymap_format_ops(গণনা xkb_keymap_format format)
{
    স্থির ধ্রুবক কাঠামো xkb_keymap_format_ops *keymap_format_ops[] = {
        [XKB_KEYMAP_FORMAT_TEXT_V1] = &text_v1_keymap_format_ops,
        [XKB_KEYMAP_FORMAT_TEXT_V2] = &text_v1_keymap_format_ops,
    };

    যদি ((পূর্ণসংখ্যা) format < 0 || (পূর্ণসংখ্যা) format >= (পূর্ণসংখ্যা) ARRAY_আকার(keymap_format_ops))
        ফেরত ফাঁকা;

    ফেরত keymap_format_ops[(পূর্ণসংখ্যা) format];
}

কাঠামো xkb_keymap *
xkb_keymap_new_from_rmlvo(ধ্রুবক কাঠামো xkb_rmlvo_builder *rmlvo,
                          গণনা xkb_keymap_format format,
                          গণনা xkb_keymap_compile_flags flags)
{
    ধ্রুবক কাঠামো xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    যদি (!ops || !ops->keymap_new_from_rmlvo) {
        log_err_func(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ফেরত ফাঁকা;
    }

    কাঠামো xkb_keymap *keymap = xkb_keymap_new(rmlvo->ctx, __func__, format,
                                               flags);
    যদি (!keymap)
        ফেরত ফাঁকা;

    যদি (!ops->keymap_new_from_rmlvo(keymap, rmlvo)) {
        xkb_keymap_unref(keymap);
        ফেরত ফাঁকা;
    }

    ফেরত keymap;
}

কাঠামো xkb_keymap *
xkb_keymap_new_from_names2(কাঠামো xkb_context *ctx,
                           ধ্রুবক কাঠামো xkb_rule_names *rmlvo_in,
                           গণনা xkb_keymap_format format,
                           গণনা xkb_keymap_compile_flags flags)
{
    ধ্রুবক কাঠামো xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    যদি (!ops || !ops->keymap_new_from_names) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ফেরত ফাঁকা;
    }

    কাঠামো xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    যদি (!keymap)
        ফেরত ফাঁকা;

    কাঠামো xkb_rule_names rmlvo = {0};
    যদি (rmlvo_in)
        rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    যদি (!ops->keymap_new_from_names(keymap, &rmlvo)) {
        xkb_keymap_unref(keymap);
        ফেরত ফাঁকা;
    }

    ফেরত keymap;
}


কাঠামো xkb_keymap *
xkb_keymap_new_from_names(কাঠামো xkb_context *ctx,
                          ধ্রুবক কাঠামো xkb_rule_names *rmlvo_in,
                          গণনা xkb_keymap_compile_flags flags)
{
    ফেরত xkb_keymap_new_from_names2(ctx, rmlvo_in,
                                      XKB_KEYMAP_FORMAT_TEXT_V2, flags);
}

কাঠামো xkb_keymap *
xkb_keymap_new_from_string(কাঠামো xkb_context *ctx,
                           ধ্রুবক অক্ষর *string,
                           গণনা xkb_keymap_format format,
                           গণনা xkb_keymap_compile_flags flags)
{
    ফেরত xkb_keymap_new_from_বাফার(ctx, string, strlen(string),
                                      format, flags);
}

কাঠামো xkb_keymap *
xkb_keymap_new_from_বাফার(কাঠামো xkb_context *ctx,
                           ধ্রুবক অক্ষর *বাফার, আকার_t দৈর্ঘ্য,
                           গণনা xkb_keymap_format format,
                           গণনা xkb_keymap_compile_flags flags)
{
    ধ্রুবক কাঠামো xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    যদি (!ops || !ops->keymap_new_from_string) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ফেরত ফাঁকা;
    }

    যদি (!বাফার) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no buffer specified\n");
        ফেরত ফাঁকা;
    }

    কাঠামো xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    যদি (!keymap)
        ফেরত ফাঁকা;

    /* Allow a zero-terminated string as a buffer */
    যদি (দৈর্ঘ্য > 0 && বাফার[দৈর্ঘ্য - 1] == '\0')
        দৈর্ঘ্য--;

    যদি (!ops->keymap_new_from_string(keymap, বাফার, দৈর্ঘ্য)) {
        xkb_keymap_unref(keymap);
        ফেরত ফাঁকা;
    }

    ফেরত keymap;
}

কাঠামো xkb_keymap *
xkb_keymap_new_from_file(কাঠামো xkb_context *ctx,
                         FILE *file,
                         গণনা xkb_keymap_format format,
                         গণনা xkb_keymap_compile_flags flags)
{
    ধ্রুবক কাঠামো xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    যদি (!ops || !ops->keymap_new_from_file) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ফেরত ফাঁকা;
    }

    যদি (!file) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no file specified\n");
        ফেরত ফাঁকা;
    }

    কাঠামো xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    যদি (!keymap)
        ফেরত ফাঁকা;

    যদি (!ops->keymap_new_from_file(keymap, file)) {
        xkb_keymap_unref(keymap);
        ফেরত ফাঁকা;
    }

    ফেরত keymap;
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
স্থির গণনা xkb_status
check_keymap_serialize_abi(
    কাঠামো xkb_context * restrict ctx,
    ধ্রুবক অক্ষর * restrict func,
    ধ্রুবক কাঠামো xkb_keymap_serialize_config * restrict config,
    ধ্রুবক কাঠামো xkb_keymap_serialize_ফলাফল * restrict ফলাফল
)
{
    গণনা xkb_status status;
    যদি ((status = xkb_check_keymap_abi(config)) ||
        (status = xkb_check_keymap_abi(ফলাফল))) {
        xkb_log_abi_error(ctx, func, status);
    }
    ফেরত status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

গণনা xkb_status
xkb_keymap_serialize(ধ্রুবক কাঠামো xkb_keymap *keymap,
                     ধ্রুবক কাঠামো xkb_keymap_serialize_config *config,
                     কাঠামো xkb_keymap_serialize_ফলাফল *ফলাফল)
{
    /* Check ABI compatibility */
    ধ্রুবক গণনা xkb_status status =
        check_keymap_serialize_abi(keymap->ctx, __func__, config, ফলাফল);
    যদি (status != XKB_SUCCESS)
        ফেরত status;

    কাঠামো xkb_keymap_serialize_config new_config = *config;

    ধ্রুবক গণনা xkb_keymap_serialize_flags invalid_flags = (
        new_config.flags &
        ~(গণনা xkb_keymap_serialize_flags)XKB_KEYMAP_SERIALIZE_FLAGS_মানসমূহ
    );
    যদি (invalid_flags) {
        log_err_func(keymap->ctx,
                     XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS_,
                     "unrecognized serialization flags: %#x\n",
                     invalid_flags);
        ফেরত XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS;
    }

    যদি (new_config.format == XKB_KEYMAP_USE_ORIGINAL_FORMAT)
        new_config.format = keymap->format;

    ধ্রুবক কাঠামো xkb_keymap_format_ops * ধ্রুবক ops =
        get_keymap_format_ops(new_config.format);
    যদি (!ops || !ops->keymap_serialize) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", new_config.format);
        ফেরত XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT;
    }

    ধ্রুবক xkb_layout_mask_t all_layouts =
        (xkb_layout_mask_t)((UINT64_C(1) << keymap->অঙ্ক_groups) - 1);
    যদি (!new_config.layouts) {
        new_config.layouts = all_layouts;
    }

    যদি (new_config.layouts & ~all_layouts) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_সূচক_2,
                     "unsupported layout mask: "
                     "expected subset of 0x%08"PRIx32", got: 0x%08"PRIx32"\n",
                     all_layouts, new_config.layouts);
        ফেরত XKB_ERROR_UNSUPPORTED_LAYOUT_সূচক;
    }

    ধ্রুবক xkb_layout_সূচক_t max_groups = format_max_groups(config->format);
    ধ্রুবক xkb_layout_সূচক_t অঙ্ক_groups =
        (xkb_layout_সূচক_t)popcount32(new_config.layouts);

    যদি (অঙ্ক_groups > max_groups) {
        ধ্রুবক bool strict =
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
            ফেরত XKB_ERROR_LAYOUT_গুনতি_LIMIT_EXCEEDED;
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

    ফেরত ops->keymap_serialize(keymap, &new_config, ফলাফল);
}

অক্ষর *
xkb_keymap_get_as_string2(কাঠামো xkb_keymap *keymap,
                          গণনা xkb_keymap_format format,
                          গণনা xkb_keymap_serialize_flags flags)
{
    ধ্রুবক কাঠামো xkb_keymap_serialize_config config = {
        .আকার_2 = আকার(config),
        .flags = flags,
        .format = format,
        .layouts = 0,
    };

    কাঠামো xkb_keymap_serialize_ফলাফল ফলাফল = { .আকার_2 = আকার(ফলাফল) };

    ফেরত (xkb_keymap_serialize(keymap, &config, &ফলাফল) == XKB_SUCCESS)
        ? ফলাফল.serialized
        : ফাঁকা;
}

অক্ষর *
xkb_keymap_get_as_string(কাঠামো xkb_keymap *keymap,
                         গণনা xkb_keymap_format format)
{
    ফেরত xkb_keymap_get_as_string2(keymap, format,
                                     XKB_KEYMAP_SERIALIZE_NO_FLAGS);
}

/**
 * Returns the total number of modifiers active in the keymap.
 */
xkb_mod_সূচক_t
xkb_keymap_অঙ্ক_mods(কাঠামো xkb_keymap *keymap)
{
    ফেরত keymap->mods.অঙ্ক_mods;
}

/**
 * Return the name for a given modifier.
 */
ধ্রুবক অক্ষর *
xkb_keymap_mod_get_name(কাঠামো xkb_keymap *keymap, xkb_mod_সূচক_t idx)
{
    যদি (idx >= keymap->mods.অঙ্ক_mods)
        ফেরত ফাঁকা;

    ফেরত xkb_atom_text(keymap->ctx, keymap->mods.mods[idx].name);
}

/**
 * Returns the index for a named modifier.
 */
xkb_mod_সূচক_t
xkb_keymap_mod_get_সূচক(কাঠামো xkb_keymap *keymap, ধ্রুবক অক্ষর *name)
{
    ধ্রুবক xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    ফেরত (atom == XKB_ATOM_NONE)
        ? XKB_MOD_INVALID
        : Xkb_Mod_Name_To_সূচক(&keymap->mods, atom, MOD_BOTH);
}

/**
 * Return the canonical mapping of a named modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask(কাঠামো xkb_keymap *keymap, ধ্রুবক অক্ষর* name)
{
    ধ্রুবক xkb_mod_সূচক_t idx = xkb_keymap_mod_get_সূচক(keymap, name);
    ফেরত (idx >= keymap->mods.অঙ্ক_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the canonical mapping of a given modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask2(কাঠামো xkb_keymap *keymap, xkb_mod_সূচক_t idx)
{
    ফেরত (idx >= keymap->mods.অঙ্ক_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the total number of active groups in the keymap.
 */
xkb_layout_সূচক_t
xkb_keymap_অঙ্ক_layouts(কাঠামো xkb_keymap *keymap)
{
    ফেরত keymap->অঙ্ক_groups;
}

/**
 * Returns the name for a given group.
 */
ধ্রুবক অক্ষর *
xkb_keymap_layout_get_name(কাঠামো xkb_keymap *keymap, xkb_layout_সূচক_t idx)
{
    যদি (idx >= keymap->অঙ্ক_group_names)
        ফেরত ফাঁকা;

    ফেরত xkb_atom_text(keymap->ctx, keymap->group_names[idx]);
}

/**
 * Returns the index for a named layout.
 */
xkb_layout_সূচক_t
xkb_keymap_layout_get_সূচক(কাঠামো xkb_keymap *keymap, ধ্রুবক অক্ষর *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_layout_সূচক_t i;

    যদি (atom == XKB_ATOM_NONE)
        ফেরত XKB_LAYOUT_INVALID;

    জন্য (i = 0; i < keymap->অঙ্ক_group_names; i++)
        যদি (keymap->group_names[i] == atom)
            ফেরত i;

    ফেরত XKB_LAYOUT_INVALID;
}

/**
 * Returns the number of layouts active for a particular key.
 */
xkb_layout_সূচক_t
xkb_keymap_অঙ্ক_layouts_for_চাবি(কাঠামো xkb_keymap *keymap, xkb_keycode_t kc)
{
    ধ্রুবক কাঠামো xkb_চাবি *চাবি = Xkb_চাবি(keymap, kc);

    যদি (!চাবি)
        ফেরত 0;

    ফেরত চাবি->অঙ্ক_groups;
}

/**
 * Returns the number of levels active for a particular key and layout.
 */
xkb_level_সূচক_t
xkb_keymap_অঙ্ক_levels_for_চাবি(কাঠামো xkb_keymap *keymap, xkb_keycode_t kc,
                              xkb_layout_সূচক_t layout)
{
    ধ্রুবক কাঠামো xkb_চাবি *চাবি = Xkb_চাবি(keymap, kc);

    যদি (!চাবি)
        ফেরত 0;

    layout = xkb_keymap_চাবি_effective_layout(চাবি, layout);
    যদি (layout == XKB_LAYOUT_INVALID)
        ফেরত 0;

    ফেরত Xkb_চাবি_অঙ্ক_Levels(চাবি, layout);
}

/**
 * Return the total number of LEDs in the keymap.
 */
xkb_led_সূচক_t
xkb_keymap_অঙ্ক_leds(কাঠামো xkb_keymap *keymap)
{
    ফেরত keymap->অঙ্ক_leds;
}

/**
 * Returns the name for a given LED.
 */
ধ্রুবক অক্ষর *
xkb_keymap_led_get_name(কাঠামো xkb_keymap *keymap, xkb_led_সূচক_t idx)
{
    যদি (idx >= keymap->অঙ্ক_leds)
        ফেরত ফাঁকা;

    ফেরত xkb_atom_text(keymap->ctx, keymap->leds[idx].name);
}

/**
 * Returns the index for a named LED.
 */
xkb_led_সূচক_t
xkb_keymap_led_get_সূচক(কাঠামো xkb_keymap *keymap, ধ্রুবক অক্ষর *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_led_সূচক_t i;
    ধ্রুবক কাঠামো xkb_led *led;

    যদি (atom == XKB_ATOM_NONE)
        ফেরত XKB_LED_INVALID;

    xkb_leds_enumerate(i, led, keymap)
        যদি (led->name == atom)
            ফেরত i;

    ফেরত XKB_LED_INVALID;
}

আকার_t
xkb_keymap_চাবি_get_mods_for_level(কাঠামো xkb_keymap *keymap,
                                  xkb_keycode_t kc,
                                  xkb_layout_সূচক_t layout,
                                  xkb_level_সূচক_t level,
                                  xkb_mod_mask_t *masks_out,
                                  আকার_t masks_আকার)
{
    ধ্রুবক কাঠামো xkb_চাবি *চাবি = Xkb_চাবি(keymap, kc);
    যদি (!চাবি)
        ফেরত 0;

    layout = xkb_keymap_চাবি_effective_layout(চাবি, layout);
    যদি (layout == XKB_LAYOUT_INVALID)
        ফেরত 0;

    যদি (level >= Xkb_চাবি_অঙ্ক_Levels(চাবি, layout))
        ফেরত 0;

    ধ্রুবক কাঠামো xkb_চাবি_type *type = চাবি->groups[layout].type;

    আকার_t গুনতি = 0;

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
        জন্য (darray_আকার_t i = 0; i < type->অঙ্ক_entries && গুনতি < masks_আকার; i++)
            যদি (entry_is_active(&type->entries[i]) &&
                type->entries[i].mods.mask == 0) {
                empty_mapped = true;
                বিরতি;
            }
        যদি (!empty_mapped && গুনতি < masks_আকার) {
            masks_out[গুনতি++] = 0;
        }
    }

    /* Now search explicit mappings. */
    জন্য (darray_আকার_t i = 0; i < type->অঙ্ক_entries && গুনতি < masks_আকার; i++) {
        যদি (entry_is_active(&type->entries[i]) &&
            type->entries[i].level == level) {
            masks_out[গুনতি++] = type->entries[i].mods.mask;
        }
    }

    ফেরত গুনতি;
}

কাঠামো xkb_level *
xkb_keymap_চাবি_get_level(কাঠামো xkb_keymap *keymap, ধ্রুবক কাঠামো xkb_চাবি *চাবি,
                         xkb_layout_সূচক_t layout, xkb_level_সূচক_t level)
{
    layout = xkb_keymap_চাবি_effective_layout(চাবি, layout);
    যদি (layout == XKB_LAYOUT_INVALID)
        ফেরত ফাঁকা;

    যদি (level >= Xkb_চাবি_অঙ্ক_Levels(চাবি, layout))
        ফেরত ফাঁকা;

    ফেরত &চাবি->groups[layout].levels[level];
}

/**
 * As below, but takes an explicit layout/level rather than state.
 */
পূর্ণসংখ্যা
xkb_keymap_চাবি_get_syms_by_level(কাঠামো xkb_keymap *keymap,
                                 xkb_keycode_t kc,
                                 xkb_layout_সূচক_t layout,
                                 xkb_level_সূচক_t level,
                                 ধ্রুবক xkb_keysym_t **syms_out)
{
    ধ্রুবক কাঠামো xkb_চাবি *চাবি = Xkb_চাবি(keymap, kc);

    যদি (!চাবি)
        যাও err;

    ধ্রুবক কাঠামো xkb_level* ধ্রুবক leveli =
        xkb_keymap_চাবি_get_level(keymap, চাবি, layout, level);

    যদি (!leveli)
        যাও err;

    ধ্রুবক xkb_keysym_গুনতি_t অঙ্ক_syms = leveli->অঙ্ক_syms;
    যদি (অঙ্ক_syms == 0)
        যাও err;

    যদি (অঙ্ক_syms == 1)
        *syms_out = &leveli->s.sym;
    অন্যথা
        *syms_out = leveli->s.syms;

    ফেরত (পূর্ণসংখ্যা) অঙ্ক_syms;

err:
    *syms_out = ফাঁকা;
    ফেরত 0;
}

xkb_keycode_t
xkb_keymap_min_keycode(কাঠামো xkb_keymap *keymap)
{
    ফেরত keymap->min_চাবি_code;
}

xkb_keycode_t
xkb_keymap_max_keycode(কাঠামো xkb_keymap *keymap)
{
    ফেরত keymap->max_চাবি_code;
}

স্থির গণনা xkb_status
keymap_চাবি_iterator_config_check(
    কাঠামো xkb_context * restrict ctx,
    ধ্রুবক অক্ষর *func,
    ধ্রুবক কাঠামো xkb_keymap_চাবি_iterator_config * restrict config
)
{
    /* Check ABI compatibility */
    ধ্রুবক গণনা xkb_status abi_error = xkb_check_keymap_abi(config);
    যদি (abi_error) {
        xkb_log_abi_error(ctx, func, abi_error);
        ফেরত abi_error;
    }

    /* Sanitize input: flags */
    ধ্রুবক uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_KEYMAP_চাবি_ITERATOR_FLAGS_মানসমূহ);
    যদি (invalid_flags) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_চাবি_ITERATOR_FLAGS_2,
                "%s: Unsupported keymap iterator flags: 0x%"PRIx32"\n",
                func, invalid_flags);
        ফেরত XKB_ERROR_UNSUPPORTED_চাবি_ITERATOR_FLAGS;
    }

    /* Sanitize input: start */
    যদি (config->start > XKB_KEYCODE_MAX)
        ফেরত XKB_ERROR_INVALID_KEYCODE;

    ফেরত XKB_SUCCESS;
}

static_assert(XKB_KEYMAP_চাবি_ITERATOR_FLAGS_মানসমূহ <=
              (পূর্ণসংখ্যা)XKB_POINTER_TAG_MASK,
              "Cannot tag cursor pointer with flags");

#define iter_init_keymap(i, ptr, flags) {          \
    (i)->opaque0 = xkb_pointer_tag((ptr), (flags));\
}
#define iter_get_keymap(i) (                                  \
    (ধ্রুবক কাঠামো xkb_keymap *)xkb_pointer_untag((i)->opaque0)\
)
#define iter_get_পরবর্তী(i) ((ধ্রুবক কাঠামো xkb_চাবি *)(i)->opaque1)
#define iter_set_পরবর্তী(i, ptr) {(i)->opaque1 = (ধ্রুবক শূন্য *)(ptr);}
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
    কাঠামো xkb_keymap_চাবি_iterator * restrict iter,
    ধ্রুবক কাঠামো xkb_keymap * restrict keymap,
    ধ্রুবক কাঠামো xkb_keymap_চাবি_iterator_config * restrict config
)
{
    /* Handle default configuration */
    স্থির ধ্রুবক কাঠামো xkb_keymap_চাবি_iterator_config default_config = {
        .আকার_2 = আকার(default_config)
    };
    যদি (!config)
        config = &default_config;

    /* Check input */
    ধ্রুবক গণনা xkb_status status =
        keymap_চাবি_iterator_config_check(keymap->ctx, __func__, config);
    যদি (status != XKB_SUCCESS) {
        ফেরত status;
    }

    /*
     * Initialization
     */

    memset(iter, 0, আকার(*iter));

    /* Borrow keymap */
    iter_init_keymap(iter, keymap, config->flags);

    যদি (keymap->অঙ্ক_চাবিগুলি == 0) {
        iter_set_পরবর্তী(iter, ফাঁকা);
        ফেরত XKB_SUCCESS;
    }

    /* Borrow key */
    ধ্রুবক কাঠামো xkb_চাবি *পরবর্তী;
    যদি (config->flags & XKB_KEYMAP_চাবি_ITERATOR_DESCENDING_ORDER) {
        যদি (!config->start || config->start > keymap->max_চাবি_code) {
            পরবর্তী = &keymap->চাবিগুলি[keymap->অঙ্ক_চাবিগুলি - 1];
        } অন্যথা {
            /*
             * May still fail *safely* if start < keymap->min_key_code:
             * there will be simply no key to iterate over.
             */
            পরবর্তী = xkb_keymap_get_পরবর্তী_defined_চাবি(keymap, false, config->start);
        }
    } অন্যথা {
        যদি (!config->start || config->start < keymap->min_চাবি_code) {
            পরবর্তী = (keymap->অঙ্ক_চাবিগুলি_নিচের)
                ? &keymap->চাবিগুলি[keymap->min_চাবি_code]
                : &keymap->চাবিগুলি[0];
        } অন্যথা {
            /*
             * May still fail *safely* if start > keymap->max_key_code:
             * there will be simply no key to iterate over.
             */
            পরবর্তী = xkb_keymap_get_পরবর্তী_defined_চাবি(keymap, true, config->start);
        }
    };
    iter_set_পরবর্তী(iter, পরবর্তী);

    ফেরত XKB_SUCCESS;
}

xkb_keycode_t
xkb_keymap_চাবি_iterator_পরবর্তী(কাঠামো xkb_keymap_চাবি_iterator *iter)
{
    ধ্রুবক কাঠামো xkb_চাবি *পরবর্তী = iter_get_পরবর্তী(iter);
    যদি (!পরবর্তী)
        ফেরত XKB_KEYCODE_INVALID;

    ধ্রুবক কাঠামো xkb_keymap *keymap = iter_get_keymap(iter);
    প্রতিজ্ঞা(keymap->অঙ্ক_চাবিগুলি);

    ধ্রুবক bool descending = iter_get_descending_order(iter);
    ধ্রুবক bool skip_unbound = !iter_get_include_unbound(iter);
    ধ্রুবক কাঠামো xkb_চাবি *last = descending
        ? (keymap->অঙ্ক_চাবিগুলি_নিচের)
            ? &keymap->চাবিগুলি[keymap->min_চাবি_code]
            : &keymap->চাবিগুলি[0]
        : &keymap->চাবিগুলি[keymap->অঙ্ক_চাবিগুলি - 1];
    ধ্রুবক ptrdiff_t step = descending ? -1 : 1;

    /* Skip undefined keys (no name) and optionally unbound keys */
    জন্য (;;) {
        ধ্রুবক bool iter_end = descending ? (পরবর্তী <= last) : (পরবর্তী >= last);
        ধ্রুবক bool skip = (
            পরবর্তী->name == XKB_ATOM_NONE ||
            (skip_unbound && (পরবর্তী->অঙ্ক_groups == 0))
        );
        যদি (!skip) {
            ধ্রুবক xkb_keycode_t ret = পরবর্তী->keycode;
            iter_set_পরবর্তী(iter, iter_end ? ফাঁকা : পরবর্তী + step);
            ফেরত ret;
        }
        যদি (iter_end) {
            iter_set_পরবর্তী(iter, ফাঁকা);
            ফেরত XKB_KEYCODE_INVALID;
        }
        পরবর্তী += step;
    }
}

#undef iter_init_keymap
#undef iter_get_keymap
#undef iter_set_পরবর্তী
#undef iter_get_পরবর্তী
#undef iter_get_descending_order
#undef iter_get_include_unbound

শূন্য
xkb_keymap_চাবি_for_each(কাঠামো xkb_keymap *keymap, xkb_keymap_চাবি_iter_t iter,
                        শূন্য *তথ্য)
{
    কাঠামো xkb_চাবি *চাবি;

    xkb_চাবিগুলি_foreach(চাবি, keymap)
        iter(keymap, চাবি->keycode, তথ্য);
}

ধ্রুবক অক্ষর *
xkb_keymap_চাবি_get_name(কাঠামো xkb_keymap *keymap, xkb_keycode_t kc)
{
    ধ্রুবক কাঠামো xkb_চাবি *চাবি = Xkb_চাবি(keymap, kc);

    যদি (!চাবি)
        ফেরত ফাঁকা;

    ফেরত xkb_atom_text(keymap->ctx, চাবি->name);
}

xkb_keycode_t
xkb_keymap_চাবি_by_name(কাঠামো xkb_keymap *keymap, ধ্রুবক অক্ষর *name)
{
    কাঠামো xkb_চাবি *চাবি;
    xkb_atom_t atom;

    atom = xkb_atom_lookup(keymap->ctx, name);
    যদি (atom) {
        জন্য (darray_আকার_t i = 0; i < keymap->অঙ্ক_চাবি_aliases; i++)
            যদি (keymap->চাবি_aliases[i].alias == atom)
                atom = keymap->চাবি_aliases[i].real;
    }
    যদি (!atom)
        ফেরত XKB_KEYCODE_INVALID;

    xkb_চাবিগুলি_foreach(চাবি, keymap) {
        যদি (চাবি->name == atom)
            ফেরত চাবি->keycode;
    }

    ফেরত XKB_KEYCODE_INVALID;
}

/**
 * Simple boolean specifying whether or not the key should repeat.
 */
পূর্ণসংখ্যা
xkb_keymap_চাবি_repeats(কাঠামো xkb_keymap *keymap, xkb_keycode_t kc)
{
    ধ্রুবক কাঠামো xkb_চাবি *চাবি = Xkb_চাবি(keymap, kc);

    যদি (!চাবি)
        ফেরত 0;

    ফেরত চাবি->repeats;
}
