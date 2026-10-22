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
    إذا (leveli->انگ_syms > 1)
        حرر(leveli->s.syms);
    إذا (leveli->انگ_actions > 1)
        حرر(leveli->a.actions);
}

ساكن فراغ
clear_interpret(بنية xkb_sym_interpret *interp)
{
    إذا (interp->انگ_actions > 1)
        حرر(interp->a.actions);
}

فراغ
xkb_keymap_unref(بنية xkb_keymap *keymap)
{
    أكد(!keymap || keymap->refcnt > 0);
    إذا (!keymap || --keymap->refcnt > 0)
        أرجع;

    إذا (keymap->ڪنجيون) {
        بنية xkb_ڪنجي *ڪنجي;
        xkb_ڪنجيون_foreach(ڪنجي, keymap) {
            إذا (ڪنجي->groups) {
                لكل (xkb_layout_اشاريو_t i = 0; i < ڪنجي->انگ_groups; i++) {
                    إذا (ڪنجي->groups[i].سطحون) {
                        لكل (xkb_level_اشاريو_t j = 0;
                             j < Xkb_ڪنجي_انگ_سطحون(ڪنجي, i);
                             j++)
                            clear_level(&ڪنجي->groups[i].سطحون[j]);
                        حرر(ڪنجي->groups[i].سطحون);
                    }
                }
                حرر(ڪنجي->groups);
            }
            إذا (!ڪنجي->overlays_inline && ڪنجي->overlays_ڪنجيون) {
                حرر((فراغ *)ڪنجي->overlays_ڪنجيون);
            }
        }
        حرر(keymap->ڪنجيون);
    }
    إذا (keymap->types) {
        لكل (darray_سائيز_t i = 0; i < keymap->انگ_types; i++) {
            حرر(keymap->types[i].entries);
            حرر(keymap->types[i].level_نالا);
        }
        حرر(keymap->types);
    }
    لكل (darray_سائيز_t k = 0; k < keymap->انگ_sym_interprets; k++) {
        clear_interpret(&keymap->sym_interprets[k]);
    }
    حرر(keymap->sym_interprets);
    حرر(keymap->modmaps);
    حرر(keymap->ڪنجي_aliases);
    حرر(keymap->گروهه_نالا);
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
        [XKB_KEYMAP_FORMAT_متن_V1] = &متن_v1_keymap_format_ops,
        [XKB_KEYMAP_FORMAT_متن_V2] = &متن_v1_keymap_format_ops,
    };

    إذا ((صحيح) format < 0 || (صحيح) format >= (صحيح) ARRAY_سائيز(keymap_format_ops))
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
                           ثابت بنية xkb_قاعدو_نالا *rmlvo_اندريون,
                           تعداد xkb_keymap_format format,
                           تعداد xkb_keymap_compile_flags flags)
{
    ثابت بنية xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    إذا (!ops || !ops->keymap_new_from_نالا) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        أرجع عدم;
    }

    بنية xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    إذا (!keymap)
        أرجع عدم;

    بنية xkb_قاعدو_نالا rmlvo = {0};
    إذا (rmlvo_اندريون)
        rmlvo = *rmlvo_اندريون;
    xkb_context_sanitize_قاعدو_نالا(ctx, &rmlvo);

    إذا (!ops->keymap_new_from_نالا(keymap, &rmlvo)) {
        xkb_keymap_unref(keymap);
        أرجع عدم;
    }

    أرجع keymap;
}


بنية xkb_keymap *
xkb_keymap_new_from_نالا(بنية xkb_context *ctx,
                          ثابت بنية xkb_قاعدو_نالا *rmlvo_اندريون,
                          تعداد xkb_keymap_compile_flags flags)
{
    أرجع xkb_keymap_new_from_names2(ctx, rmlvo_اندريون,
                                      XKB_KEYMAP_FORMAT_متن_V2, flags);
}

بنية xkb_keymap *
xkb_keymap_new_from_string(بنية xkb_context *ctx,
                           ثابت حرف *string,
                           تعداد xkb_keymap_format format,
                           تعداد xkb_keymap_compile_flags flags)
{
    أرجع xkb_keymap_new_from_بفر(ctx, string, لڙي_ڊيگهه(string),
                                      format, flags);
}

بنية xkb_keymap *
xkb_keymap_new_from_بفر(بنية xkb_context *ctx,
                           ثابت حرف *بفر, سائيز_t ڊيگهائي,
                           تعداد xkb_keymap_format format,
                           تعداد xkb_keymap_compile_flags flags)
{
    ثابت بنية xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    إذا (!ops || !ops->keymap_new_from_string) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        أرجع عدم;
    }

    إذا (!بفر) {
        log_err_func1(ctx, XKB_LOG_پيغام_NO_ID,
                      "no buffer specified\n");
        أرجع عدم;
    }

    بنية xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    إذا (!keymap)
        أرجع عدم;

    /* Allow a zero-terminated string as a buffer */
    إذا (ڊيگهائي > 0 && بفر[ڊيگهائي - 1] == '\0')
        ڊيگهائي--;

    إذا (!ops->keymap_new_from_string(keymap, بفر, ڊيگهائي)) {
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
        log_err_func1(ctx, XKB_LOG_پيغام_NO_ID,
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
ساكن تعداد xkb_حالت
check_keymap_serialize_abi(
    بنية xkb_context * محدود ctx,
    ثابت حرف * محدود func,
    ثابت بنية xkb_keymap_serialize_config * محدود config,
    ثابت بنية xkb_keymap_serialize_نتيجو * محدود نتيجو
)
{
    تعداد xkb_حالت حالت;
    إذا ((حالت = xkb_check_keymap_abi(config)) ||
        (حالت = xkb_check_keymap_abi(نتيجو))) {
        xkb_log_abi_error(ctx, func, حالت);
    }
    أرجع حالت;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

تعداد xkb_حالت
xkb_keymap_serialize(ثابت بنية xkb_keymap *keymap,
                     ثابت بنية xkb_keymap_serialize_config *config,
                     بنية xkb_keymap_serialize_نتيجو *نتيجو)
{
    /* Check ABI compatibility */
    ثابت تعداد xkb_حالت حالت =
        check_keymap_serialize_abi(keymap->ctx, __func__, config, نتيجو);
    إذا (حالت != XKB_SUCCESS)
        أرجع حالت;

    بنية xkb_keymap_serialize_config new_config = *config;

    ثابت تعداد xkb_keymap_serialize_flags invalid_flags = (
        new_config.flags &
        ~(تعداد xkb_keymap_serialize_flags)XKB_KEYMAP_SERIALIZE_FLAGS_قدرون
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

    ثابت xkb_layout_نقاب_t all_layouts =
        (xkb_layout_نقاب_t)((UINT64_C(1) << keymap->انگ_groups) - 1);
    إذا (!new_config.layouts) {
        new_config.layouts = all_layouts;
    }

    إذا (new_config.layouts & ~all_layouts) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو_2,
                     "unsupported layout mask: "
                     "expected subset of 0x%08"PRIx32", got: 0x%08"PRIx32"\n",
                     all_layouts, new_config.layouts);
        أرجع XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو;
    }

    ثابت xkb_layout_اشاريو_t max_groups = format_max_groups(config->format);
    ثابت xkb_layout_اشاريو_t انگ_groups =
        (xkb_layout_اشاريو_t)popcount32(new_config.layouts);

    إذا (انگ_groups > max_groups) {
        ثابت bool strict =
            (new_config.flags & XKB_KEYMAP_SERIALIZE_STRICT_گهڻائي);
        xkb_log_with_ڪوڊ(
            keymap->ctx,
            (strict ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
            XKB_LOG_VERBOSITY_گهٽتر,
            XKB_ERROR_LAYOUT_ڳڻپ_حد_بندي_EXCEEDED_2,
            "Cannot serialize %"PRIu32" groups in keymap format %d: "
            "maximum is %"PRIu32"%s\n",
            انگ_groups, config->format, max_groups,
            (strict ? "" : "; discarding unsupported groups")
        );
        إذا (strict) {
            أرجع XKB_ERROR_LAYOUT_ڳڻپ_حد_بندي_EXCEEDED;
        } وإلا {
            new_config.layouts =
                keep_سڀ_کان_هيٺ_n_سيٽ_بٽ(new_config.layouts, max_groups);
        }
    }

    log_dbg(keymap->ctx, XKB_LOG_پيغام_NO_ID,
            "Serializing group mask 0x%"PRIx32" (total: %"PRIu32")\n",
            new_config.layouts, انگ_groups);

    إذا (new_config.layouts != all_layouts) {
        /*
         * If some layouts are discarded, then interprets may not trigger
         * properly. Avoid this by requiring explicit values.
         */
        new_config.flags |= (XKB_KEYMAP_SERIALIZE_EXPLICIT_ڪنجي_قدرون |
                             XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS);
    }

    أرجع ops->keymap_serialize(keymap, &new_config, نتيجو);
}

حرف *
xkb_keymap_get_as_string2(بنية xkb_keymap *keymap,
                          تعداد xkb_keymap_format format,
                          تعداد xkb_keymap_serialize_flags flags)
{
    ثابت بنية xkb_keymap_serialize_config config = {
        .سائيز = حجم(config),
        .flags = flags,
        .format = format,
        .layouts = 0,
    };

    بنية xkb_keymap_serialize_نتيجو نتيجو = { .سائيز = حجم(نتيجو) };

    أرجع (xkb_keymap_serialize(keymap, &config, &نتيجو) == XKB_SUCCESS)
        ? نتيجو.serialized
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
xkb_mod_اشاريو_t
xkb_keymap_انگ_mods(بنية xkb_keymap *keymap)
{
    أرجع keymap->mods.انگ_mods;
}

/**
 * Return the name for a given modifier.
 */
ثابت حرف *
xkb_keymap_mod_get_name(بنية xkb_keymap *keymap, xkb_mod_اشاريو_t idx)
{
    إذا (idx >= keymap->mods.انگ_mods)
        أرجع عدم;

    أرجع xkb_atom_متن(keymap->ctx, keymap->mods.mods[idx].name);
}

/**
 * Returns the index for a named modifier.
 */
xkb_mod_اشاريو_t
xkb_keymap_mod_get_اشاريو(بنية xkb_keymap *keymap, ثابت حرف *name)
{
    ثابت xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    أرجع (atom == XKB_ATOM_NONE)
        ? XKB_MOD_INVALID
        : Xkb_Mod_Name_منزل_اشاريو(&keymap->mods, atom, MOD_BOTH);
}

/**
 * Return the canonical mapping of a named modifier.
 */
xkb_mod_نقاب_t
xkb_keymap_mod_get_نقاب(بنية xkb_keymap *keymap, ثابت حرف* name)
{
    ثابت xkb_mod_اشاريو_t idx = xkb_keymap_mod_get_اشاريو(keymap, name);
    أرجع (idx >= keymap->mods.انگ_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the canonical mapping of a given modifier.
 */
xkb_mod_نقاب_t
xkb_keymap_mod_get_mask2(بنية xkb_keymap *keymap, xkb_mod_اشاريو_t idx)
{
    أرجع (idx >= keymap->mods.انگ_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the total number of active groups in the keymap.
 */
xkb_layout_اشاريو_t
xkb_keymap_انگ_layouts(بنية xkb_keymap *keymap)
{
    أرجع keymap->انگ_groups;
}

/**
 * Returns the name for a given group.
 */
ثابت حرف *
xkb_keymap_layout_get_name(بنية xkb_keymap *keymap, xkb_layout_اشاريو_t idx)
{
    إذا (idx >= keymap->انگ_گروهه_نالا)
        أرجع عدم;

    أرجع xkb_atom_متن(keymap->ctx, keymap->گروهه_نالا[idx]);
}

/**
 * Returns the index for a named layout.
 */
xkb_layout_اشاريو_t
xkb_keymap_layout_get_اشاريو(بنية xkb_keymap *keymap, ثابت حرف *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_layout_اشاريو_t i;

    إذا (atom == XKB_ATOM_NONE)
        أرجع XKB_LAYOUT_INVALID;

    لكل (i = 0; i < keymap->انگ_گروهه_نالا; i++)
        إذا (keymap->گروهه_نالا[i] == atom)
            أرجع i;

    أرجع XKB_LAYOUT_INVALID;
}

/**
 * Returns the number of layouts active for a particular key.
 */
xkb_layout_اشاريو_t
xkb_keymap_انگ_layouts_for_ڪنجي(بنية xkb_keymap *keymap, xkb_keycode_t kc)
{
    ثابت بنية xkb_ڪنجي *ڪنجي = Xkb_ڪنجي(keymap, kc);

    إذا (!ڪنجي)
        أرجع 0;

    أرجع ڪنجي->انگ_groups;
}

/**
 * Returns the number of levels active for a particular key and layout.
 */
xkb_level_اشاريو_t
xkb_keymap_انگ_سطحون_for_ڪنجي(بنية xkb_keymap *keymap, xkb_keycode_t kc,
                              xkb_layout_اشاريو_t layout)
{
    ثابت بنية xkb_ڪنجي *ڪنجي = Xkb_ڪنجي(keymap, kc);

    إذا (!ڪنجي)
        أرجع 0;

    layout = xkb_keymap_ڪنجي_effective_layout(ڪنجي, layout);
    إذا (layout == XKB_LAYOUT_INVALID)
        أرجع 0;

    أرجع Xkb_ڪنجي_انگ_سطحون(ڪنجي, layout);
}

/**
 * Return the total number of LEDs in the keymap.
 */
xkb_led_اشاريو_t
xkb_keymap_انگ_leds(بنية xkb_keymap *keymap)
{
    أرجع keymap->انگ_leds;
}

/**
 * Returns the name for a given LED.
 */
ثابت حرف *
xkb_keymap_led_get_name(بنية xkb_keymap *keymap, xkb_led_اشاريو_t idx)
{
    إذا (idx >= keymap->انگ_leds)
        أرجع عدم;

    أرجع xkb_atom_متن(keymap->ctx, keymap->leds[idx].name);
}

/**
 * Returns the index for a named LED.
 */
xkb_led_اشاريو_t
xkb_keymap_led_get_اشاريو(بنية xkb_keymap *keymap, ثابت حرف *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_led_اشاريو_t i;
    ثابت بنية xkb_led *led;

    إذا (atom == XKB_ATOM_NONE)
        أرجع XKB_LED_INVALID;

    xkb_leds_enumerate(i, led, keymap)
        إذا (led->name == atom)
            أرجع i;

    أرجع XKB_LED_INVALID;
}

سائيز_t
xkb_keymap_ڪنجي_get_mods_for_level(بنية xkb_keymap *keymap,
                                  xkb_keycode_t kc,
                                  xkb_layout_اشاريو_t layout,
                                  xkb_level_اشاريو_t level,
                                  xkb_mod_نقاب_t *masks_ٻاهريون,
                                  سائيز_t masks_سائيز)
{
    ثابت بنية xkb_ڪنجي *ڪنجي = Xkb_ڪنجي(keymap, kc);
    إذا (!ڪنجي)
        أرجع 0;

    layout = xkb_keymap_ڪنجي_effective_layout(ڪنجي, layout);
    إذا (layout == XKB_LAYOUT_INVALID)
        أرجع 0;

    إذا (level >= Xkb_ڪنجي_انگ_سطحون(ڪنجي, layout))
        أرجع 0;

    ثابت بنية xkb_ڪنجي_type *type = ڪنجي->groups[layout].type;

    سائيز_t ڳڻپ = 0;

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
        bool خالي_جايون_mapped = false;
        لكل (darray_سائيز_t i = 0; i < type->انگ_entries && ڳڻپ < masks_سائيز; i++)
            إذا (داخلا_is_سرگرم(&type->entries[i]) &&
                type->entries[i].mods.نقاب == 0) {
                خالي_جايون_mapped = true;
                اكسر;
            }
        إذا (!خالي_جايون_mapped && ڳڻپ < masks_سائيز) {
            masks_ٻاهريون[ڳڻپ++] = 0;
        }
    }

    /* Now search explicit mappings. */
    لكل (darray_سائيز_t i = 0; i < type->انگ_entries && ڳڻپ < masks_سائيز; i++) {
        إذا (داخلا_is_سرگرم(&type->entries[i]) &&
            type->entries[i].level == level) {
            masks_ٻاهريون[ڳڻپ++] = type->entries[i].mods.نقاب;
        }
    }

    أرجع ڳڻپ;
}

بنية xkb_level *
xkb_keymap_ڪنجي_get_level(بنية xkb_keymap *keymap, ثابت بنية xkb_ڪنجي *ڪنجي,
                         xkb_layout_اشاريو_t layout, xkb_level_اشاريو_t level)
{
    layout = xkb_keymap_ڪنجي_effective_layout(ڪنجي, layout);
    إذا (layout == XKB_LAYOUT_INVALID)
        أرجع عدم;

    إذا (level >= Xkb_ڪنجي_انگ_سطحون(ڪنجي, layout))
        أرجع عدم;

    أرجع &ڪنجي->groups[layout].سطحون[level];
}

/**
 * As below, but takes an explicit layout/level rather than state.
 */
صحيح
xkb_keymap_ڪنجي_get_syms_by_level(بنية xkb_keymap *keymap,
                                 xkb_keycode_t kc,
                                 xkb_layout_اشاريو_t layout,
                                 xkb_level_اشاريو_t level,
                                 ثابت xkb_keysym_t **syms_ٻاهريون)
{
    ثابت بنية xkb_ڪنجي *ڪنجي = Xkb_ڪنجي(keymap, kc);

    إذا (!ڪنجي)
        اذهب err;

    ثابت بنية xkb_level* ثابت leveli =
        xkb_keymap_ڪنجي_get_level(keymap, ڪنجي, layout, level);

    إذا (!leveli)
        اذهب err;

    ثابت xkb_keysym_ڳڻپ_t انگ_syms = leveli->انگ_syms;
    إذا (انگ_syms == 0)
        اذهب err;

    إذا (انگ_syms == 1)
        *syms_ٻاهريون = &leveli->s.sym;
    وإلا
        *syms_ٻاهريون = leveli->s.syms;

    أرجع (صحيح) انگ_syms;

err:
    *syms_ٻاهريون = عدم;
    أرجع 0;
}

xkb_keycode_t
xkb_keymap_min_keycode(بنية xkb_keymap *keymap)
{
    أرجع keymap->min_ڪنجي_ڪوڊ;
}

xkb_keycode_t
xkb_keymap_max_keycode(بنية xkb_keymap *keymap)
{
    أرجع keymap->max_ڪنجي_ڪوڊ;
}

ساكن تعداد xkb_حالت
keymap_ڪنجي_iterator_config_check(
    بنية xkb_context * محدود ctx,
    ثابت حرف *func,
    ثابت بنية xkb_keymap_ڪنجي_iterator_config * محدود config
)
{
    /* Check ABI compatibility */
    ثابت تعداد xkb_حالت abi_error = xkb_check_keymap_abi(config);
    إذا (abi_error) {
        xkb_log_abi_error(ctx, func, abi_error);
        أرجع abi_error;
    }

    /* Sanitize input: flags */
    ثابت uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_KEYMAP_ڪنجي_ITERATOR_FLAGS_قدرون);
    إذا (invalid_flags) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_ڪنجي_ITERATOR_FLAGS_2,
                "%s: Unsupported keymap iterator flags: 0x%"PRIx32"\n",
                func, invalid_flags);
        أرجع XKB_ERROR_UNSUPPORTED_ڪنجي_ITERATOR_FLAGS;
    }

    /* Sanitize input: start */
    إذا (config->شروعات > XKB_KEYCODE_MAX)
        أرجع XKB_ERROR_INVALID_KEYCODE;

    أرجع XKB_SUCCESS;
}

static_assert(XKB_KEYMAP_ڪنجي_ITERATOR_FLAGS_قدرون <=
              (صحيح)XKB_اشارو_ٽيگ_نقاب,
              "Cannot tag cursor pointer with flags");

#وصف_ڪريو iter_init_keymap(i, ptr, flags) {          \
    (i)->opaque0 = xkb_اشارو_ٽيگ((ptr), (flags));\
}
#وصف_ڪريو iter_get_keymap(i) (                                  \
    (ثابت بنية xkb_keymap *)xkb_اشارو_untag((i)->opaque0)\
)
#وصف_ڪريو iter_get_ايندڙ(i) ((ثابت بنية xkb_ڪنجي *)(i)->opaque1)
#وصف_ڪريو iter_سيٽ_ايندڙ(i, ptr) {(i)->opaque1 = (ثابت فراغ *)(ptr);}
#وصف_ڪريو iter_get_descending_ترتيب(i) (                  \
    xkb_اشارو_get_ٽيگ((i)->opaque0) &                 \
    (uintptr_t)XKB_KEYMAP_ڪنجي_ITERATOR_DESCENDING_ترتيب \
)
#وصف_ڪريو iter_get_include_unbound(i) (                  \
    xkb_اشارو_get_ٽيگ((i)->opaque0) &                \
    (uintptr_t)XKB_KEYMAP_ڪنجي_ITERATOR_INCLUDE_UNBOUND \
)

تعداد xkb_حالت
xkb_keymap_ڪنجي_iterator_init(
    بنية xkb_keymap_ڪنجي_iterator * محدود iter,
    ثابت بنية xkb_keymap * محدود keymap,
    ثابت بنية xkb_keymap_ڪنجي_iterator_config * محدود config
)
{
    /* Handle default configuration */
    ساكن ثابت بنية xkb_keymap_ڪنجي_iterator_config default_config = {
        .سائيز = حجم(default_config)
    };
    إذا (!config)
        config = &default_config;

    /* Check input */
    ثابت تعداد xkb_حالت حالت =
        keymap_ڪنجي_iterator_config_check(keymap->ctx, __func__, config);
    إذا (حالت != XKB_SUCCESS) {
        أرجع حالت;
    }

    /*
     * Initialization
     */

    memset(iter, 0, حجم(*iter));

    /* Borrow keymap */
    iter_init_keymap(iter, keymap, config->flags);

    إذا (keymap->انگ_ڪنجيون == 0) {
        iter_سيٽ_ايندڙ(iter, عدم);
        أرجع XKB_SUCCESS;
    }

    /* Borrow key */
    ثابت بنية xkb_ڪنجي *ايندڙ;
    إذا (config->flags & XKB_KEYMAP_ڪنجي_ITERATOR_DESCENDING_ترتيب) {
        إذا (!config->شروعات || config->شروعات > keymap->max_ڪنجي_ڪوڊ) {
            ايندڙ = &keymap->ڪنجيون[keymap->انگ_ڪنجيون - 1];
        } وإلا {
            /*
             * May still fail *safely* if start < keymap->min_key_code:
             * there will be simply no key to iterate over.
             */
            ايندڙ = xkb_keymap_get_ايندڙ_defined_ڪنجي(keymap, false, config->شروعات);
        }
    } وإلا {
        إذا (!config->شروعات || config->شروعات < keymap->min_ڪنجي_ڪوڊ) {
            ايندڙ = (keymap->انگ_ڪنجيون_هيٺيون)
                ? &keymap->ڪنجيون[keymap->min_ڪنجي_ڪوڊ]
                : &keymap->ڪنجيون[0];
        } وإلا {
            /*
             * May still fail *safely* if start > keymap->max_key_code:
             * there will be simply no key to iterate over.
             */
            ايندڙ = xkb_keymap_get_ايندڙ_defined_ڪنجي(keymap, true, config->شروعات);
        }
    };
    iter_سيٽ_ايندڙ(iter, ايندڙ);

    أرجع XKB_SUCCESS;
}

xkb_keycode_t
xkb_keymap_ڪنجي_iterator_ايندڙ(بنية xkb_keymap_ڪنجي_iterator *iter)
{
    ثابت بنية xkb_ڪنجي *ايندڙ = iter_get_ايندڙ(iter);
    إذا (!ايندڙ)
        أرجع XKB_KEYCODE_INVALID;

    ثابت بنية xkb_keymap *keymap = iter_get_keymap(iter);
    أكد(keymap->انگ_ڪنجيون);

    ثابت bool descending = iter_get_descending_ترتيب(iter);
    ثابت bool skip_unbound = !iter_get_include_unbound(iter);
    ثابت بنية xkb_ڪنجي *آخري = descending
        ? (keymap->انگ_ڪنجيون_هيٺيون)
            ? &keymap->ڪنجيون[keymap->min_ڪنجي_ڪوڊ]
            : &keymap->ڪنجيون[0]
        : &keymap->ڪنجيون[keymap->انگ_ڪنجيون - 1];
    ثابت ptrdiff_t قدم = descending ? -1 : 1;

    /* Skip undefined keys (no name) and optionally unbound keys */
    لكل (;;) {
        ثابت bool iter_پڇاڙي = descending ? (ايندڙ <= آخري) : (ايندڙ >= آخري);
        ثابت bool skip = (
            ايندڙ->name == XKB_ATOM_NONE ||
            (skip_unbound && (ايندڙ->انگ_groups == 0))
        );
        إذا (!skip) {
            ثابت xkb_keycode_t ret = ايندڙ->keycode;
            iter_سيٽ_ايندڙ(iter, iter_پڇاڙي ? عدم : ايندڙ + قدم);
            أرجع ret;
        }
        إذا (iter_پڇاڙي) {
            iter_سيٽ_ايندڙ(iter, عدم);
            أرجع XKB_KEYCODE_INVALID;
        }
        ايندڙ += قدم;
    }
}

#وصف_رد iter_init_keymap
#وصف_رد iter_get_keymap
#وصف_رد iter_سيٽ_ايندڙ
#وصف_رد iter_get_ايندڙ
#وصف_رد iter_get_descending_ترتيب
#وصف_رد iter_get_include_unbound

فراغ
xkb_keymap_ڪنجي_for_each(بنية xkb_keymap *keymap, xkb_keymap_ڪنجي_iter_t iter,
                        فراغ *معلومات)
{
    بنية xkb_ڪنجي *ڪنجي;

    xkb_ڪنجيون_foreach(ڪنجي, keymap)
        iter(keymap, ڪنجي->keycode, معلومات);
}

ثابت حرف *
xkb_keymap_ڪنجي_get_name(بنية xkb_keymap *keymap, xkb_keycode_t kc)
{
    ثابت بنية xkb_ڪنجي *ڪنجي = Xkb_ڪنجي(keymap, kc);

    إذا (!ڪنجي)
        أرجع عدم;

    أرجع xkb_atom_متن(keymap->ctx, ڪنجي->name);
}

xkb_keycode_t
xkb_keymap_ڪنجي_by_name(بنية xkb_keymap *keymap, ثابت حرف *name)
{
    بنية xkb_ڪنجي *ڪنجي;
    xkb_atom_t atom;

    atom = xkb_atom_lookup(keymap->ctx, name);
    إذا (atom) {
        لكل (darray_سائيز_t i = 0; i < keymap->انگ_ڪنجي_aliases; i++)
            إذا (keymap->ڪنجي_aliases[i].alias == atom)
                atom = keymap->ڪنجي_aliases[i].real;
    }
    إذا (!atom)
        أرجع XKB_KEYCODE_INVALID;

    xkb_ڪنجيون_foreach(ڪنجي, keymap) {
        إذا (ڪنجي->name == atom)
            أرجع ڪنجي->keycode;
    }

    أرجع XKB_KEYCODE_INVALID;
}

/**
 * Simple boolean specifying whether or not the key should repeat.
 */
صحيح
xkb_keymap_ڪنجي_repeats(بنية xkb_keymap *keymap, xkb_keycode_t kc)
{
    ثابت بنية xkb_ڪنجي *ڪنجي = Xkb_ڪنجي(keymap, kc);

    إذا (!ڪنجي)
        أرجع 0;

    أرجع ڪنجي->repeats;
}
