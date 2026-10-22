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

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>
#అన్తర్భావ <stdint.h>
#అన్తర్భావ <string.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "abi-check.h"
#అన్తర్భావ "atom.h"
#అన్తర్భావ "context.h"
#అన్తర్భావ "features/enums.h"
#అన్తర్భావ "keymap.h"
#అన్తర్భావ "keymap-priv.h"
#అన్తర్భావ "messages-codes.h"
#అన్తర్భావ "text.h"
#అన్తర్భావ "util-mem.h"
#అన్తర్భావ "util-numbers.h"

నిర్మాణం xkb_keymap *
xkb_keymap_ref(నిర్మాణం xkb_keymap *keymap)
{
    ప్రతిజ్ఞా(keymap->refcnt > 0);
    keymap->refcnt++;
    తిరిగి keymap;
}

శూన్య
clear_level(నిర్మాణం xkb_level *leveli)
{
    ఒకవేళ (leveli->అంకె_syms > 1)
        విడుదల(leveli->s.syms);
    ఒకవేళ (leveli->అంకె_actions > 1)
        విడుదల(leveli->a.actions);
}

స్థిర శూన్య
clear_interpret(నిర్మాణం xkb_sym_interpret *interp)
{
    ఒకవేళ (interp->అంకె_actions > 1)
        విడుదల(interp->a.actions);
}

శూన్య
xkb_keymap_unref(నిర్మాణం xkb_keymap *keymap)
{
    ప్రతిజ్ఞా(!keymap || keymap->refcnt > 0);
    ఒకవేళ (!keymap || --keymap->refcnt > 0)
        తిరిగి;

    ఒకవేళ (keymap->కీలకాలు) {
        నిర్మాణం xkb_కీలకం *కీలకం;
        xkb_కీలకాలు_foreach(కీలకం, keymap) {
            ఒకవేళ (కీలకం->groups) {
                కొరకు (xkb_layout_సూచీ_t i = 0; i < కీలకం->అంకె_groups; i++) {
                    ఒకవేళ (కీలకం->groups[i].స్థాయిలు) {
                        కొరకు (xkb_level_సూచీ_t j = 0;
                             j < Xkb_కీలకం_అంకె_స్థాయిలు(కీలకం, i);
                             j++)
                            clear_level(&కీలకం->groups[i].స్థాయిలు[j]);
                        విడుదల(కీలకం->groups[i].స్థాయిలు);
                    }
                }
                విడుదల(కీలకం->groups);
            }
            ఒకవేళ (!కీలకం->overlays_inline && కీలకం->overlays_కీలకాలు) {
                విడుదల((శూన్య *)కీలకం->overlays_కీలకాలు);
            }
        }
        విడుదల(keymap->కీలకాలు);
    }
    ఒకవేళ (keymap->types) {
        కొరకు (darray_కొలత_t i = 0; i < keymap->అంకె_types; i++) {
            విడుదల(keymap->types[i].entries);
            విడుదల(keymap->types[i].level_పేర్లు);
        }
        విడుదల(keymap->types);
    }
    కొరకు (darray_కొలత_t k = 0; k < keymap->అంకె_sym_interprets; k++) {
        clear_interpret(&keymap->sym_interprets[k]);
    }
    విడుదల(keymap->sym_interprets);
    విడుదల(keymap->modmaps);
    విడుదల(keymap->కీలకం_aliases);
    విడుదల(keymap->సమూహం_పేర్లు);
    విడుదల(keymap->keycodes_section_name);
    విడుదల(keymap->symbols_section_name);
    విడుదల(keymap->types_section_name);
    విడుదల(keymap->compat_section_name);
    xkb_context_unref(keymap->ctx);
    విడుదల(keymap);
}

స్థిర స్థిరరాశి నిర్మాణం xkb_keymap_format_ops *
get_keymap_format_ops(గణన xkb_keymap_format format)
{
    స్థిర స్థిరరాశి నిర్మాణం xkb_keymap_format_ops *keymap_format_ops[] = {
        [XKB_KEYMAP_FORMAT_పాఠ్యం_V1] = &పాఠ్యం_v1_keymap_format_ops,
        [XKB_KEYMAP_FORMAT_పాఠ్యం_V2] = &పాఠ్యం_v1_keymap_format_ops,
    };

    ఒకవేళ ((పూర్ణాంకం) format < 0 || (పూర్ణాంకం) format >= (పూర్ణాంకం) ARRAY_కొలత(keymap_format_ops))
        తిరిగి ఖాళీ;

    తిరిగి keymap_format_ops[(పూర్ణాంకం) format];
}

నిర్మాణం xkb_keymap *
xkb_keymap_new_from_rmlvo(స్థిరరాశి నిర్మాణం xkb_rmlvo_builder *rmlvo,
                          గణన xkb_keymap_format format,
                          గణన xkb_keymap_compile_flags flags)
{
    స్థిరరాశి నిర్మాణం xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    ఒకవేళ (!ops || !ops->keymap_new_from_rmlvo) {
        log_err_func(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        తిరిగి ఖాళీ;
    }

    నిర్మాణం xkb_keymap *keymap = xkb_keymap_new(rmlvo->ctx, __func__, format,
                                               flags);
    ఒకవేళ (!keymap)
        తిరిగి ఖాళీ;

    ఒకవేళ (!ops->keymap_new_from_rmlvo(keymap, rmlvo)) {
        xkb_keymap_unref(keymap);
        తిరిగి ఖాళీ;
    }

    తిరిగి keymap;
}

నిర్మాణం xkb_keymap *
xkb_keymap_new_from_names2(నిర్మాణం xkb_context *ctx,
                           స్థిరరాశి నిర్మాణం xkb_నియమం_పేర్లు *rmlvo_లోపల,
                           గణన xkb_keymap_format format,
                           గణన xkb_keymap_compile_flags flags)
{
    స్థిరరాశి నిర్మాణం xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    ఒకవేళ (!ops || !ops->keymap_new_from_పేర్లు) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        తిరిగి ఖాళీ;
    }

    నిర్మాణం xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    ఒకవేళ (!keymap)
        తిరిగి ఖాళీ;

    నిర్మాణం xkb_నియమం_పేర్లు rmlvo = {0};
    ఒకవేళ (rmlvo_లోపల)
        rmlvo = *rmlvo_లోపల;
    xkb_context_sanitize_నియమం_పేర్లు(ctx, &rmlvo);

    ఒకవేళ (!ops->keymap_new_from_పేర్లు(keymap, &rmlvo)) {
        xkb_keymap_unref(keymap);
        తిరిగి ఖాళీ;
    }

    తిరిగి keymap;
}


నిర్మాణం xkb_keymap *
xkb_keymap_new_from_పేర్లు(నిర్మాణం xkb_context *ctx,
                          స్థిరరాశి నిర్మాణం xkb_నియమం_పేర్లు *rmlvo_లోపల,
                          గణన xkb_keymap_compile_flags flags)
{
    తిరిగి xkb_keymap_new_from_names2(ctx, rmlvo_లోపల,
                                      XKB_KEYMAP_FORMAT_పాఠ్యం_V2, flags);
}

నిర్మాణం xkb_keymap *
xkb_keymap_new_from_string(నిర్మాణం xkb_context *ctx,
                           స్థిరరాశి అక్షరం *string,
                           గణన xkb_keymap_format format,
                           గణన xkb_keymap_compile_flags flags)
{
    తిరిగి xkb_keymap_new_from_బఫరు(ctx, string, అక్షరశ్రేణి_పొడవు(string),
                                      format, flags);
}

నిర్మాణం xkb_keymap *
xkb_keymap_new_from_బఫరు(నిర్మాణం xkb_context *ctx,
                           స్థిరరాశి అక్షరం *బఫరు, కొలత_t నిడివి,
                           గణన xkb_keymap_format format,
                           గణన xkb_keymap_compile_flags flags)
{
    స్థిరరాశి నిర్మాణం xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    ఒకవేళ (!ops || !ops->keymap_new_from_string) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        తిరిగి ఖాళీ;
    }

    ఒకవేళ (!బఫరు) {
        log_err_func1(ctx, XKB_LOG_సందేశం_NO_ID,
                      "no buffer specified\n");
        తిరిగి ఖాళీ;
    }

    నిర్మాణం xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    ఒకవేళ (!keymap)
        తిరిగి ఖాళీ;

    /* Allow a zero-terminated string as a buffer */
    ఒకవేళ (నిడివి > 0 && బఫరు[నిడివి - 1] == '\0')
        నిడివి--;

    ఒకవేళ (!ops->keymap_new_from_string(keymap, బఫరు, నిడివి)) {
        xkb_keymap_unref(keymap);
        తిరిగి ఖాళీ;
    }

    తిరిగి keymap;
}

నిర్మాణం xkb_keymap *
xkb_keymap_new_from_file(నిర్మాణం xkb_context *ctx,
                         FILE *file,
                         గణన xkb_keymap_format format,
                         గణన xkb_keymap_compile_flags flags)
{
    స్థిరరాశి నిర్మాణం xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    ఒకవేళ (!ops || !ops->keymap_new_from_file) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        తిరిగి ఖాళీ;
    }

    ఒకవేళ (!file) {
        log_err_func1(ctx, XKB_LOG_సందేశం_NO_ID,
                      "no file specified\n");
        తిరిగి ఖాళీ;
    }

    నిర్మాణం xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    ఒకవేళ (!keymap)
        తిరిగి ఖాళీ;

    ఒకవేళ (!ops->keymap_new_from_file(keymap, file)) {
        xkb_keymap_unref(keymap);
        తిరిగి ఖాళీ;
    }

    తిరిగి keymap;
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
స్థిర గణన xkb_స్థితి
check_keymap_serialize_abi(
    నిర్మాణం xkb_context * పరిమిత ctx,
    స్థిరరాశి అక్షరం * పరిమిత func,
    స్థిరరాశి నిర్మాణం xkb_keymap_serialize_config * పరిమిత config,
    స్థిరరాశి నిర్మాణం xkb_keymap_serialize_ఫలితం * పరిమిత ఫలితం
)
{
    గణన xkb_స్థితి స్థితి_2;
    ఒకవేళ ((స్థితి_2 = xkb_check_keymap_abi(config)) ||
        (స్థితి_2 = xkb_check_keymap_abi(ఫలితం))) {
        xkb_log_abi_error(ctx, func, స్థితి_2);
    }
    తిరిగి స్థితి_2;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

గణన xkb_స్థితి
xkb_keymap_serialize(స్థిరరాశి నిర్మాణం xkb_keymap *keymap,
                     స్థిరరాశి నిర్మాణం xkb_keymap_serialize_config *config,
                     నిర్మాణం xkb_keymap_serialize_ఫలితం *ఫలితం)
{
    /* Check ABI compatibility */
    స్థిరరాశి గణన xkb_స్థితి స్థితి_2 =
        check_keymap_serialize_abi(keymap->ctx, __func__, config, ఫలితం);
    ఒకవేళ (స్థితి_2 != XKB_SUCCESS)
        తిరిగి స్థితి_2;

    నిర్మాణం xkb_keymap_serialize_config new_config = *config;

    స్థిరరాశి గణన xkb_keymap_serialize_flags invalid_flags = (
        new_config.flags &
        ~(గణన xkb_keymap_serialize_flags)XKB_KEYMAP_SERIALIZE_FLAGS_విలువలు
    );
    ఒకవేళ (invalid_flags) {
        log_err_func(keymap->ctx,
                     XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS_,
                     "unrecognized serialization flags: %#x\n",
                     invalid_flags);
        తిరిగి XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS;
    }

    ఒకవేళ (new_config.format == XKB_KEYMAP_USE_ORIGINAL_FORMAT)
        new_config.format = keymap->format;

    స్థిరరాశి నిర్మాణం xkb_keymap_format_ops * స్థిరరాశి ops =
        get_keymap_format_ops(new_config.format);
    ఒకవేళ (!ops || !ops->keymap_serialize) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", new_config.format);
        తిరిగి XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT;
    }

    స్థిరరాశి xkb_layout_ముసుగు_t all_layouts =
        (xkb_layout_ముసుగు_t)((UINT64_C(1) << keymap->అంకె_groups) - 1);
    ఒకవేళ (!new_config.layouts) {
        new_config.layouts = all_layouts;
    }

    ఒకవేళ (new_config.layouts & ~all_layouts) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ_2,
                     "unsupported layout mask: "
                     "expected subset of 0x%08"PRIx32", got: 0x%08"PRIx32"\n",
                     all_layouts, new_config.layouts);
        తిరిగి XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ;
    }

    స్థిరరాశి xkb_layout_సూచీ_t max_groups = format_max_groups(config->format);
    స్థిరరాశి xkb_layout_సూచీ_t అంకె_groups =
        (xkb_layout_సూచీ_t)popcount32(new_config.layouts);

    ఒకవేళ (అంకె_groups > max_groups) {
        స్థిరరాశి bool strict =
            (new_config.flags & XKB_KEYMAP_SERIALIZE_STRICT_బహుళకం);
        xkb_log_with_సంకేతం(
            keymap->ctx,
            (strict ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
            XKB_LOG_VERBOSITY_కనీస,
            XKB_ERROR_LAYOUT_లెక్క_హద్దు_EXCEEDED_2,
            "Cannot serialize %"PRIu32" groups in keymap format %d: "
            "maximum is %"PRIu32"%s\n",
            అంకె_groups, config->format, max_groups,
            (strict ? "" : "; discarding unsupported groups")
        );
        ఒకవేళ (strict) {
            తిరిగి XKB_ERROR_LAYOUT_లెక్క_హద్దు_EXCEEDED;
        } లేకపోతే {
            new_config.layouts =
                keep_అత్యల్ప_n_సమితి_బిట్లు(new_config.layouts, max_groups);
        }
    }

    log_dbg(keymap->ctx, XKB_LOG_సందేశం_NO_ID,
            "Serializing group mask 0x%"PRIx32" (total: %"PRIu32")\n",
            new_config.layouts, అంకె_groups);

    ఒకవేళ (new_config.layouts != all_layouts) {
        /*
         * If some layouts are discarded, then interprets may not trigger
         * properly. Avoid this by requiring explicit values.
         */
        new_config.flags |= (XKB_KEYMAP_SERIALIZE_EXPLICIT_కీలకం_విలువలు |
                             XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS);
    }

    తిరిగి ops->keymap_serialize(keymap, &new_config, ఫలితం);
}

అక్షరం *
xkb_keymap_get_as_string2(నిర్మాణం xkb_keymap *keymap,
                          గణన xkb_keymap_format format,
                          గణన xkb_keymap_serialize_flags flags)
{
    స్థిరరాశి నిర్మాణం xkb_keymap_serialize_config config = {
        .కొలత = పరిమాణం(config),
        .flags = flags,
        .format = format,
        .layouts = 0,
    };

    నిర్మాణం xkb_keymap_serialize_ఫలితం ఫలితం = { .కొలత = పరిమాణం(ఫలితం) };

    తిరిగి (xkb_keymap_serialize(keymap, &config, &ఫలితం) == XKB_SUCCESS)
        ? ఫలితం.serialized
        : ఖాళీ;
}

అక్షరం *
xkb_keymap_get_as_string(నిర్మాణం xkb_keymap *keymap,
                         గణన xkb_keymap_format format)
{
    తిరిగి xkb_keymap_get_as_string2(keymap, format,
                                     XKB_KEYMAP_SERIALIZE_NO_FLAGS);
}

/**
 * Returns the total number of modifiers active in the keymap.
 */
xkb_mod_సూచీ_t
xkb_keymap_అంకె_mods(నిర్మాణం xkb_keymap *keymap)
{
    తిరిగి keymap->mods.అంకె_mods;
}

/**
 * Return the name for a given modifier.
 */
స్థిరరాశి అక్షరం *
xkb_keymap_mod_get_name(నిర్మాణం xkb_keymap *keymap, xkb_mod_సూచీ_t idx)
{
    ఒకవేళ (idx >= keymap->mods.అంకె_mods)
        తిరిగి ఖాళీ;

    తిరిగి xkb_atom_పాఠ్యం(keymap->ctx, keymap->mods.mods[idx].name);
}

/**
 * Returns the index for a named modifier.
 */
xkb_mod_సూచీ_t
xkb_keymap_mod_get_సూచీ(నిర్మాణం xkb_keymap *keymap, స్థిరరాశి అక్షరం *name)
{
    స్థిరరాశి xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    తిరిగి (atom == XKB_ATOM_NONE)
        ? XKB_MOD_INVALID
        : Xkb_Mod_Name_గమ్యం_సూచీ(&keymap->mods, atom, MOD_BOTH);
}

/**
 * Return the canonical mapping of a named modifier.
 */
xkb_mod_ముసుగు_t
xkb_keymap_mod_get_ముసుగు(నిర్మాణం xkb_keymap *keymap, స్థిరరాశి అక్షరం* name)
{
    స్థిరరాశి xkb_mod_సూచీ_t idx = xkb_keymap_mod_get_సూచీ(keymap, name);
    తిరిగి (idx >= keymap->mods.అంకె_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the canonical mapping of a given modifier.
 */
xkb_mod_ముసుగు_t
xkb_keymap_mod_get_mask2(నిర్మాణం xkb_keymap *keymap, xkb_mod_సూచీ_t idx)
{
    తిరిగి (idx >= keymap->mods.అంకె_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the total number of active groups in the keymap.
 */
xkb_layout_సూచీ_t
xkb_keymap_అంకె_layouts(నిర్మాణం xkb_keymap *keymap)
{
    తిరిగి keymap->అంకె_groups;
}

/**
 * Returns the name for a given group.
 */
స్థిరరాశి అక్షరం *
xkb_keymap_layout_get_name(నిర్మాణం xkb_keymap *keymap, xkb_layout_సూచీ_t idx)
{
    ఒకవేళ (idx >= keymap->అంకె_సమూహం_పేర్లు)
        తిరిగి ఖాళీ;

    తిరిగి xkb_atom_పాఠ్యం(keymap->ctx, keymap->సమూహం_పేర్లు[idx]);
}

/**
 * Returns the index for a named layout.
 */
xkb_layout_సూచీ_t
xkb_keymap_layout_get_సూచీ(నిర్మాణం xkb_keymap *keymap, స్థిరరాశి అక్షరం *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_layout_సూచీ_t i;

    ఒకవేళ (atom == XKB_ATOM_NONE)
        తిరిగి XKB_LAYOUT_INVALID;

    కొరకు (i = 0; i < keymap->అంకె_సమూహం_పేర్లు; i++)
        ఒకవేళ (keymap->సమూహం_పేర్లు[i] == atom)
            తిరిగి i;

    తిరిగి XKB_LAYOUT_INVALID;
}

/**
 * Returns the number of layouts active for a particular key.
 */
xkb_layout_సూచీ_t
xkb_keymap_అంకె_layouts_for_కీలకం(నిర్మాణం xkb_keymap *keymap, xkb_keycode_t kc)
{
    స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం = Xkb_కీలకం(keymap, kc);

    ఒకవేళ (!కీలకం)
        తిరిగి 0;

    తిరిగి కీలకం->అంకె_groups;
}

/**
 * Returns the number of levels active for a particular key and layout.
 */
xkb_level_సూచీ_t
xkb_keymap_అంకె_స్థాయిలు_for_కీలకం(నిర్మాణం xkb_keymap *keymap, xkb_keycode_t kc,
                              xkb_layout_సూచీ_t layout)
{
    స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం = Xkb_కీలకం(keymap, kc);

    ఒకవేళ (!కీలకం)
        తిరిగి 0;

    layout = xkb_keymap_కీలకం_effective_layout(కీలకం, layout);
    ఒకవేళ (layout == XKB_LAYOUT_INVALID)
        తిరిగి 0;

    తిరిగి Xkb_కీలకం_అంకె_స్థాయిలు(కీలకం, layout);
}

/**
 * Return the total number of LEDs in the keymap.
 */
xkb_led_సూచీ_t
xkb_keymap_అంకె_leds(నిర్మాణం xkb_keymap *keymap)
{
    తిరిగి keymap->అంకె_leds;
}

/**
 * Returns the name for a given LED.
 */
స్థిరరాశి అక్షరం *
xkb_keymap_led_get_name(నిర్మాణం xkb_keymap *keymap, xkb_led_సూచీ_t idx)
{
    ఒకవేళ (idx >= keymap->అంకె_leds)
        తిరిగి ఖాళీ;

    తిరిగి xkb_atom_పాఠ్యం(keymap->ctx, keymap->leds[idx].name);
}

/**
 * Returns the index for a named LED.
 */
xkb_led_సూచీ_t
xkb_keymap_led_get_సూచీ(నిర్మాణం xkb_keymap *keymap, స్థిరరాశి అక్షరం *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_led_సూచీ_t i;
    స్థిరరాశి నిర్మాణం xkb_led *led;

    ఒకవేళ (atom == XKB_ATOM_NONE)
        తిరిగి XKB_LED_INVALID;

    xkb_leds_enumerate(i, led, keymap)
        ఒకవేళ (led->name == atom)
            తిరిగి i;

    తిరిగి XKB_LED_INVALID;
}

కొలత_t
xkb_keymap_కీలకం_get_mods_for_level(నిర్మాణం xkb_keymap *keymap,
                                  xkb_keycode_t kc,
                                  xkb_layout_సూచీ_t layout,
                                  xkb_level_సూచీ_t level,
                                  xkb_mod_ముసుగు_t *masks_బహిర్గతం,
                                  కొలత_t masks_కొలత)
{
    స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం = Xkb_కీలకం(keymap, kc);
    ఒకవేళ (!కీలకం)
        తిరిగి 0;

    layout = xkb_keymap_కీలకం_effective_layout(కీలకం, layout);
    ఒకవేళ (layout == XKB_LAYOUT_INVALID)
        తిరిగి 0;

    ఒకవేళ (level >= Xkb_కీలకం_అంకె_స్థాయిలు(కీలకం, layout))
        తిరిగి 0;

    స్థిరరాశి నిర్మాణం xkb_కీలకం_type *type = కీలకం->groups[layout].type;

    కొలత_t లెక్క = 0;

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
    ఒకవేళ (level == 0) {
        bool ఖాళీలు_mapped = false;
        కొరకు (darray_కొలత_t i = 0; i < type->అంకె_entries && లెక్క < masks_కొలత; i++)
            ఒకవేళ (నమోదు_is_చురుకైన(&type->entries[i]) &&
                type->entries[i].mods.ముసుగు == 0) {
                ఖాళీలు_mapped = true;
                ఆపు;
            }
        ఒకవేళ (!ఖాళీలు_mapped && లెక్క < masks_కొలత) {
            masks_బహిర్గతం[లెక్క++] = 0;
        }
    }

    /* Now search explicit mappings. */
    కొరకు (darray_కొలత_t i = 0; i < type->అంకె_entries && లెక్క < masks_కొలత; i++) {
        ఒకవేళ (నమోదు_is_చురుకైన(&type->entries[i]) &&
            type->entries[i].level == level) {
            masks_బహిర్గతం[లెక్క++] = type->entries[i].mods.ముసుగు;
        }
    }

    తిరిగి లెక్క;
}

నిర్మాణం xkb_level *
xkb_keymap_కీలకం_get_level(నిర్మాణం xkb_keymap *keymap, స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                         xkb_layout_సూచీ_t layout, xkb_level_సూచీ_t level)
{
    layout = xkb_keymap_కీలకం_effective_layout(కీలకం, layout);
    ఒకవేళ (layout == XKB_LAYOUT_INVALID)
        తిరిగి ఖాళీ;

    ఒకవేళ (level >= Xkb_కీలకం_అంకె_స్థాయిలు(కీలకం, layout))
        తిరిగి ఖాళీ;

    తిరిగి &కీలకం->groups[layout].స్థాయిలు[level];
}

/**
 * As below, but takes an explicit layout/level rather than state.
 */
పూర్ణాంకం
xkb_keymap_కీలకం_get_syms_by_level(నిర్మాణం xkb_keymap *keymap,
                                 xkb_keycode_t kc,
                                 xkb_layout_సూచీ_t layout,
                                 xkb_level_సూచీ_t level,
                                 స్థిరరాశి xkb_keysym_t **syms_బహిర్గతం)
{
    స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం = Xkb_కీలకం(keymap, kc);

    ఒకవేళ (!కీలకం)
        వెళ్ళు err;

    స్థిరరాశి నిర్మాణం xkb_level* స్థిరరాశి leveli =
        xkb_keymap_కీలకం_get_level(keymap, కీలకం, layout, level);

    ఒకవేళ (!leveli)
        వెళ్ళు err;

    స్థిరరాశి xkb_keysym_లెక్క_t అంకె_syms = leveli->అంకె_syms;
    ఒకవేళ (అంకె_syms == 0)
        వెళ్ళు err;

    ఒకవేళ (అంకె_syms == 1)
        *syms_బహిర్గతం = &leveli->s.sym;
    లేకపోతే
        *syms_బహిర్గతం = leveli->s.syms;

    తిరిగి (పూర్ణాంకం) అంకె_syms;

err:
    *syms_బహిర్గతం = ఖాళీ;
    తిరిగి 0;
}

xkb_keycode_t
xkb_keymap_min_keycode(నిర్మాణం xkb_keymap *keymap)
{
    తిరిగి keymap->min_కీలకం_సంకేతం;
}

xkb_keycode_t
xkb_keymap_max_keycode(నిర్మాణం xkb_keymap *keymap)
{
    తిరిగి keymap->max_కీలకం_సంకేతం;
}

స్థిర గణన xkb_స్థితి
keymap_కీలకం_iterator_config_check(
    నిర్మాణం xkb_context * పరిమిత ctx,
    స్థిరరాశి అక్షరం *func,
    స్థిరరాశి నిర్మాణం xkb_keymap_కీలకం_iterator_config * పరిమిత config
)
{
    /* Check ABI compatibility */
    స్థిరరాశి గణన xkb_స్థితి abi_error = xkb_check_keymap_abi(config);
    ఒకవేళ (abi_error) {
        xkb_log_abi_error(ctx, func, abi_error);
        తిరిగి abi_error;
    }

    /* Sanitize input: flags */
    స్థిరరాశి uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_KEYMAP_కీలకం_ITERATOR_FLAGS_విలువలు);
    ఒకవేళ (invalid_flags) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_కీలకం_ITERATOR_FLAGS_2,
                "%s: Unsupported keymap iterator flags: 0x%"PRIx32"\n",
                func, invalid_flags);
        తిరిగి XKB_ERROR_UNSUPPORTED_కీలకం_ITERATOR_FLAGS;
    }

    /* Sanitize input: start */
    ఒకవేళ (config->ప్రారంభం > XKB_KEYCODE_MAX)
        తిరిగి XKB_ERROR_INVALID_KEYCODE;

    తిరిగి XKB_SUCCESS;
}

static_assert(XKB_KEYMAP_కీలకం_ITERATOR_FLAGS_విలువలు <=
              (పూర్ణాంకం)XKB_సూచిక_ట్యాగ్_ముసుగు,
              "Cannot tag cursor pointer with flags");

#నిర్వచించు iter_init_keymap(i, ptr, flags) {          \
    (i)->opaque0 = xkb_సూచిక_ట్యాగ్((ptr), (flags));\
}
#నిర్వచించు iter_get_keymap(i) (                                  \
    (స్థిరరాశి నిర్మాణం xkb_keymap *)xkb_సూచిక_untag((i)->opaque0)\
)
#నిర్వచించు iter_get_తదుపరి(i) ((స్థిరరాశి నిర్మాణం xkb_కీలకం *)(i)->opaque1)
#నిర్వచించు iter_సమితి_తదుపరి(i, ptr) {(i)->opaque1 = (స్థిరరాశి శూన్య *)(ptr);}
#నిర్వచించు iter_get_descending_క్రమం(i) (                  \
    xkb_సూచిక_get_ట్యాగ్((i)->opaque0) &                 \
    (uintptr_t)XKB_KEYMAP_కీలకం_ITERATOR_DESCENDING_క్రమం \
)
#నిర్వచించు iter_get_include_unbound(i) (                  \
    xkb_సూచిక_get_ట్యాగ్((i)->opaque0) &                \
    (uintptr_t)XKB_KEYMAP_కీలకం_ITERATOR_INCLUDE_UNBOUND \
)

గణన xkb_స్థితి
xkb_keymap_కీలకం_iterator_init(
    నిర్మాణం xkb_keymap_కీలకం_iterator * పరిమిత iter,
    స్థిరరాశి నిర్మాణం xkb_keymap * పరిమిత keymap,
    స్థిరరాశి నిర్మాణం xkb_keymap_కీలకం_iterator_config * పరిమిత config
)
{
    /* Handle default configuration */
    స్థిర స్థిరరాశి నిర్మాణం xkb_keymap_కీలకం_iterator_config default_config = {
        .కొలత = పరిమాణం(default_config)
    };
    ఒకవేళ (!config)
        config = &default_config;

    /* Check input */
    స్థిరరాశి గణన xkb_స్థితి స్థితి_2 =
        keymap_కీలకం_iterator_config_check(keymap->ctx, __func__, config);
    ఒకవేళ (స్థితి_2 != XKB_SUCCESS) {
        తిరిగి స్థితి_2;
    }

    /*
     * Initialization
     */

    memset(iter, 0, పరిమాణం(*iter));

    /* Borrow keymap */
    iter_init_keymap(iter, keymap, config->flags);

    ఒకవేళ (keymap->అంకె_కీలకాలు == 0) {
        iter_సమితి_తదుపరి(iter, ఖాళీ);
        తిరిగి XKB_SUCCESS;
    }

    /* Borrow key */
    స్థిరరాశి నిర్మాణం xkb_కీలకం *తదుపరి;
    ఒకవేళ (config->flags & XKB_KEYMAP_కీలకం_ITERATOR_DESCENDING_క్రమం) {
        ఒకవేళ (!config->ప్రారంభం || config->ప్రారంభం > keymap->max_కీలకం_సంకేతం) {
            తదుపరి = &keymap->కీలకాలు[keymap->అంకె_కీలకాలు - 1];
        } లేకపోతే {
            /*
             * May still fail *safely* if start < keymap->min_key_code:
             * there will be simply no key to iterate over.
             */
            తదుపరి = xkb_keymap_get_తదుపరి_defined_కీలకం(keymap, false, config->ప్రారంభం);
        }
    } లేకపోతే {
        ఒకవేళ (!config->ప్రారంభం || config->ప్రారంభం < keymap->min_కీలకం_సంకేతం) {
            తదుపరి = (keymap->అంకె_కీలకాలు_దిగువ)
                ? &keymap->కీలకాలు[keymap->min_కీలకం_సంకేతం]
                : &keymap->కీలకాలు[0];
        } లేకపోతే {
            /*
             * May still fail *safely* if start > keymap->max_key_code:
             * there will be simply no key to iterate over.
             */
            తదుపరి = xkb_keymap_get_తదుపరి_defined_కీలకం(keymap, true, config->ప్రారంభం);
        }
    };
    iter_సమితి_తదుపరి(iter, తదుపరి);

    తిరిగి XKB_SUCCESS;
}

xkb_keycode_t
xkb_keymap_కీలకం_iterator_తదుపరి(నిర్మాణం xkb_keymap_కీలకం_iterator *iter)
{
    స్థిరరాశి నిర్మాణం xkb_కీలకం *తదుపరి = iter_get_తదుపరి(iter);
    ఒకవేళ (!తదుపరి)
        తిరిగి XKB_KEYCODE_INVALID;

    స్థిరరాశి నిర్మాణం xkb_keymap *keymap = iter_get_keymap(iter);
    ప్రతిజ్ఞా(keymap->అంకె_కీలకాలు);

    స్థిరరాశి bool descending = iter_get_descending_క్రమం(iter);
    స్థిరరాశి bool skip_unbound = !iter_get_include_unbound(iter);
    స్థిరరాశి నిర్మాణం xkb_కీలకం *చివరి = descending
        ? (keymap->అంకె_కీలకాలు_దిగువ)
            ? &keymap->కీలకాలు[keymap->min_కీలకం_సంకేతం]
            : &keymap->కీలకాలు[0]
        : &keymap->కీలకాలు[keymap->అంకె_కీలకాలు - 1];
    స్థిరరాశి ptrdiff_t అడుగు = descending ? -1 : 1;

    /* Skip undefined keys (no name) and optionally unbound keys */
    కొరకు (;;) {
        స్థిరరాశి bool iter_చివర = descending ? (తదుపరి <= చివరి) : (తదుపరి >= చివరి);
        స్థిరరాశి bool skip = (
            తదుపరి->name == XKB_ATOM_NONE ||
            (skip_unbound && (తదుపరి->అంకె_groups == 0))
        );
        ఒకవేళ (!skip) {
            స్థిరరాశి xkb_keycode_t ret = తదుపరి->keycode;
            iter_సమితి_తదుపరి(iter, iter_చివర ? ఖాళీ : తదుపరి + అడుగు);
            తిరిగి ret;
        }
        ఒకవేళ (iter_చివర) {
            iter_సమితి_తదుపరి(iter, ఖాళీ);
            తిరిగి XKB_KEYCODE_INVALID;
        }
        తదుపరి += అడుగు;
    }
}

#నిర్వచనం_రద్దు iter_init_keymap
#నిర్వచనం_రద్దు iter_get_keymap
#నిర్వచనం_రద్దు iter_సమితి_తదుపరి
#నిర్వచనం_రద్దు iter_get_తదుపరి
#నిర్వచనం_రద్దు iter_get_descending_క్రమం
#నిర్వచనం_రద్దు iter_get_include_unbound

శూన్య
xkb_keymap_కీలకం_for_each(నిర్మాణం xkb_keymap *keymap, xkb_keymap_కీలకం_iter_t iter,
                        శూన్య *దత్తాంశం)
{
    నిర్మాణం xkb_కీలకం *కీలకం;

    xkb_కీలకాలు_foreach(కీలకం, keymap)
        iter(keymap, కీలకం->keycode, దత్తాంశం);
}

స్థిరరాశి అక్షరం *
xkb_keymap_కీలకం_get_name(నిర్మాణం xkb_keymap *keymap, xkb_keycode_t kc)
{
    స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం = Xkb_కీలకం(keymap, kc);

    ఒకవేళ (!కీలకం)
        తిరిగి ఖాళీ;

    తిరిగి xkb_atom_పాఠ్యం(keymap->ctx, కీలకం->name);
}

xkb_keycode_t
xkb_keymap_కీలకం_by_name(నిర్మాణం xkb_keymap *keymap, స్థిరరాశి అక్షరం *name)
{
    నిర్మాణం xkb_కీలకం *కీలకం;
    xkb_atom_t atom;

    atom = xkb_atom_lookup(keymap->ctx, name);
    ఒకవేళ (atom) {
        కొరకు (darray_కొలత_t i = 0; i < keymap->అంకె_కీలకం_aliases; i++)
            ఒకవేళ (keymap->కీలకం_aliases[i].alias == atom)
                atom = keymap->కీలకం_aliases[i].real;
    }
    ఒకవేళ (!atom)
        తిరిగి XKB_KEYCODE_INVALID;

    xkb_కీలకాలు_foreach(కీలకం, keymap) {
        ఒకవేళ (కీలకం->name == atom)
            తిరిగి కీలకం->keycode;
    }

    తిరిగి XKB_KEYCODE_INVALID;
}

/**
 * Simple boolean specifying whether or not the key should repeat.
 */
పూర్ణాంకం
xkb_keymap_కీలకం_repeats(నిర్మాణం xkb_keymap *keymap, xkb_keycode_t kc)
{
    స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం = Xkb_కీలకం(keymap, kc);

    ఒకవేళ (!కీలకం)
        తిరిగి 0;

    తిరిగి కీలకం->repeats;
}
