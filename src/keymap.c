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
    إذا (leveli->عدد_syms > 1)
        حرر(leveli->s.syms);
    إذا (leveli->عدد_actions > 1)
        حرر(leveli->a.actions);
}

ساكن فراغ
clear_interpret(بنية xkb_sym_interpret *interp)
{
    إذا (interp->عدد_actions > 1)
        حرر(interp->a.actions);
}

فراغ
xkb_keymap_unref(بنية xkb_keymap *keymap)
{
    أكد(!keymap || keymap->refcnt > 0);
    إذا (!keymap || --keymap->refcnt > 0)
        أرجع;

    إذا (keymap->کلیدیں) {
        بنية xkb_کلید *کلید;
        xkb_کلیدیں_foreach(کلید, keymap) {
            إذا (کلید->groups) {
                لكل (xkb_layout_اشاریہ_t i = 0; i < کلید->عدد_groups; i++) {
                    إذا (کلید->groups[i].پدھراں) {
                        لكل (xkb_level_اشاریہ_t j = 0;
                             j < Xkb_کلید_عدد_پدھراں(کلید, i);
                             j++)
                            clear_level(&کلید->groups[i].پدھراں[j]);
                        حرر(کلید->groups[i].پدھراں);
                    }
                }
                حرر(کلید->groups);
            }
            إذا (!کلید->overlays_inline && کلید->overlays_کلیدیں) {
                حرر((فراغ *)کلید->overlays_کلیدیں);
            }
        }
        حرر(keymap->کلیدیں);
    }
    إذا (keymap->types) {
        لكل (darray_ناپ_t i = 0; i < keymap->عدد_types; i++) {
            حرر(keymap->types[i].entries);
            حرر(keymap->types[i].level_ناں);
        }
        حرر(keymap->types);
    }
    لكل (darray_ناپ_t k = 0; k < keymap->عدد_sym_interprets; k++) {
        clear_interpret(&keymap->sym_interprets[k]);
    }
    حرر(keymap->sym_interprets);
    حرر(keymap->modmaps);
    حرر(keymap->کلید_aliases);
    حرر(keymap->گروہ_ناں);
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

    إذا ((صحيح) format < 0 || (صحيح) format >= (صحيح) ARRAY_ناپ(keymap_format_ops))
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
                           ثابت بنية xkb_قاعدہ_ناں *rmlvo_وچ,
                           تعداد xkb_keymap_format format,
                           تعداد xkb_keymap_compile_flags flags)
{
    ثابت بنية xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    إذا (!ops || !ops->keymap_new_from_ناں) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        أرجع عدم;
    }

    بنية xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    إذا (!keymap)
        أرجع عدم;

    بنية xkb_قاعدہ_ناں rmlvo = {0};
    إذا (rmlvo_وچ)
        rmlvo = *rmlvo_وچ;
    xkb_context_sanitize_قاعدہ_ناں(ctx, &rmlvo);

    إذا (!ops->keymap_new_from_ناں(keymap, &rmlvo)) {
        xkb_keymap_unref(keymap);
        أرجع عدم;
    }

    أرجع keymap;
}


بنية xkb_keymap *
xkb_keymap_new_from_ناں(بنية xkb_context *ctx,
                          ثابت بنية xkb_قاعدہ_ناں *rmlvo_وچ,
                          تعداد xkb_keymap_compile_flags flags)
{
    أرجع xkb_keymap_new_from_names2(ctx, rmlvo_وچ,
                                      XKB_KEYMAP_FORMAT_متن_V2, flags);
}

بنية xkb_keymap *
xkb_keymap_new_from_string(بنية xkb_context *ctx,
                           ثابت حرف *string,
                           تعداد xkb_keymap_format format,
                           تعداد xkb_keymap_compile_flags flags)
{
    أرجع xkb_keymap_new_from_بفر(ctx, string, لڑی_لمبائی(string),
                                      format, flags);
}

بنية xkb_keymap *
xkb_keymap_new_from_بفر(بنية xkb_context *ctx,
                           ثابت حرف *بفر, ناپ_t لمائی,
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
        log_err_func1(ctx, XKB_LOG_سنیہا_NO_ID,
                      "no buffer specified\n");
        أرجع عدم;
    }

    بنية xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    إذا (!keymap)
        أرجع عدم;

    /* Allow a zero-terminated string as a buffer */
    إذا (لمائی > 0 && بفر[لمائی - 1] == '\0')
        لمائی--;

    إذا (!ops->keymap_new_from_string(keymap, بفر, لمائی)) {
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
        log_err_func1(ctx, XKB_LOG_سنیہا_NO_ID,
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
    ثابت بنية xkb_keymap_serialize_نتیجہ * محدود نتیجہ
)
{
    تعداد xkb_حالت حالت;
    إذا ((حالت = xkb_check_keymap_abi(config)) ||
        (حالت = xkb_check_keymap_abi(نتیجہ))) {
        xkb_log_abi_error(ctx, func, حالت);
    }
    أرجع حالت;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

تعداد xkb_حالت
xkb_keymap_serialize(ثابت بنية xkb_keymap *keymap,
                     ثابت بنية xkb_keymap_serialize_config *config,
                     بنية xkb_keymap_serialize_نتیجہ *نتیجہ)
{
    /* Check ABI compatibility */
    ثابت تعداد xkb_حالت حالت =
        check_keymap_serialize_abi(keymap->ctx, __func__, config, نتیجہ);
    إذا (حالت != XKB_SUCCESS)
        أرجع حالت;

    بنية xkb_keymap_serialize_config new_config = *config;

    ثابت تعداد xkb_keymap_serialize_flags invalid_flags = (
        new_config.flags &
        ~(تعداد xkb_keymap_serialize_flags)XKB_KEYMAP_SERIALIZE_FLAGS_اقدار
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
        (xkb_layout_نقاب_t)((UINT64_C(1) << keymap->عدد_groups) - 1);
    إذا (!new_config.layouts) {
        new_config.layouts = all_layouts;
    }

    إذا (new_config.layouts & ~all_layouts) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                     "unsupported layout mask: "
                     "expected subset of 0x%08"PRIx32", got: 0x%08"PRIx32"\n",
                     all_layouts, new_config.layouts);
        أرجع XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ;
    }

    ثابت xkb_layout_اشاریہ_t max_groups = format_max_groups(config->format);
    ثابت xkb_layout_اشاریہ_t عدد_groups =
        (xkb_layout_اشاریہ_t)popcount32(new_config.layouts);

    إذا (عدد_groups > max_groups) {
        ثابت bool strict =
            (new_config.flags & XKB_KEYMAP_SERIALIZE_STRICT_منوال);
        xkb_log_with_کوڈ(
            keymap->ctx,
            (strict ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
            XKB_LOG_VERBOSITY_اقل,
            XKB_ERROR_LAYOUT_گنتی_حد_بندی_EXCEEDED_2,
            "Cannot serialize %"PRIu32" groups in keymap format %d: "
            "maximum is %"PRIu32"%s\n",
            عدد_groups, config->format, max_groups,
            (strict ? "" : "; discarding unsupported groups")
        );
        إذا (strict) {
            أرجع XKB_ERROR_LAYOUT_گنتی_حد_بندی_EXCEEDED;
        } وإلا {
            new_config.layouts =
                keep_سبھ_توں_نیواں_n_سیٹ_بٹ(new_config.layouts, max_groups);
        }
    }

    log_dbg(keymap->ctx, XKB_LOG_سنیہا_NO_ID,
            "Serializing group mask 0x%"PRIx32" (total: %"PRIu32")\n",
            new_config.layouts, عدد_groups);

    إذا (new_config.layouts != all_layouts) {
        /*
         * If some layouts are discarded, then interprets may not trigger
         * properly. Avoid this by requiring explicit values.
         */
        new_config.flags |= (XKB_KEYMAP_SERIALIZE_EXPLICIT_کلید_اقدار |
                             XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS);
    }

    أرجع ops->keymap_serialize(keymap, &new_config, نتیجہ);
}

حرف *
xkb_keymap_get_as_string2(بنية xkb_keymap *keymap,
                          تعداد xkb_keymap_format format,
                          تعداد xkb_keymap_serialize_flags flags)
{
    ثابت بنية xkb_keymap_serialize_config config = {
        .ناپ = حجم(config),
        .flags = flags,
        .format = format,
        .layouts = 0,
    };

    بنية xkb_keymap_serialize_نتیجہ نتیجہ = { .ناپ = حجم(نتیجہ) };

    أرجع (xkb_keymap_serialize(keymap, &config, &نتیجہ) == XKB_SUCCESS)
        ? نتیجہ.serialized
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
xkb_mod_اشاریہ_t
xkb_keymap_عدد_mods(بنية xkb_keymap *keymap)
{
    أرجع keymap->mods.عدد_mods;
}

/**
 * Return the name for a given modifier.
 */
ثابت حرف *
xkb_keymap_mod_get_name(بنية xkb_keymap *keymap, xkb_mod_اشاریہ_t idx)
{
    إذا (idx >= keymap->mods.عدد_mods)
        أرجع عدم;

    أرجع xkb_atom_متن(keymap->ctx, keymap->mods.mods[idx].name);
}

/**
 * Returns the index for a named modifier.
 */
xkb_mod_اشاریہ_t
xkb_keymap_mod_get_اشاریہ(بنية xkb_keymap *keymap, ثابت حرف *name)
{
    ثابت xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    أرجع (atom == XKB_ATOM_NONE)
        ? XKB_MOD_INVALID
        : Xkb_Mod_Name_منزل_اشاریہ(&keymap->mods, atom, MOD_BOTH);
}

/**
 * Return the canonical mapping of a named modifier.
 */
xkb_mod_نقاب_t
xkb_keymap_mod_get_نقاب(بنية xkb_keymap *keymap, ثابت حرف* name)
{
    ثابت xkb_mod_اشاریہ_t idx = xkb_keymap_mod_get_اشاریہ(keymap, name);
    أرجع (idx >= keymap->mods.عدد_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the canonical mapping of a given modifier.
 */
xkb_mod_نقاب_t
xkb_keymap_mod_get_mask2(بنية xkb_keymap *keymap, xkb_mod_اشاریہ_t idx)
{
    أرجع (idx >= keymap->mods.عدد_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the total number of active groups in the keymap.
 */
xkb_layout_اشاریہ_t
xkb_keymap_عدد_layouts(بنية xkb_keymap *keymap)
{
    أرجع keymap->عدد_groups;
}

/**
 * Returns the name for a given group.
 */
ثابت حرف *
xkb_keymap_layout_get_name(بنية xkb_keymap *keymap, xkb_layout_اشاریہ_t idx)
{
    إذا (idx >= keymap->عدد_گروہ_ناں)
        أرجع عدم;

    أرجع xkb_atom_متن(keymap->ctx, keymap->گروہ_ناں[idx]);
}

/**
 * Returns the index for a named layout.
 */
xkb_layout_اشاریہ_t
xkb_keymap_layout_get_اشاریہ(بنية xkb_keymap *keymap, ثابت حرف *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_layout_اشاریہ_t i;

    إذا (atom == XKB_ATOM_NONE)
        أرجع XKB_LAYOUT_INVALID;

    لكل (i = 0; i < keymap->عدد_گروہ_ناں; i++)
        إذا (keymap->گروہ_ناں[i] == atom)
            أرجع i;

    أرجع XKB_LAYOUT_INVALID;
}

/**
 * Returns the number of layouts active for a particular key.
 */
xkb_layout_اشاریہ_t
xkb_keymap_عدد_layouts_for_کلید(بنية xkb_keymap *keymap, xkb_keycode_t kc)
{
    ثابت بنية xkb_کلید *کلید = Xkb_کلید(keymap, kc);

    إذا (!کلید)
        أرجع 0;

    أرجع کلید->عدد_groups;
}

/**
 * Returns the number of levels active for a particular key and layout.
 */
xkb_level_اشاریہ_t
xkb_keymap_عدد_پدھراں_for_کلید(بنية xkb_keymap *keymap, xkb_keycode_t kc,
                              xkb_layout_اشاریہ_t layout)
{
    ثابت بنية xkb_کلید *کلید = Xkb_کلید(keymap, kc);

    إذا (!کلید)
        أرجع 0;

    layout = xkb_keymap_کلید_effective_layout(کلید, layout);
    إذا (layout == XKB_LAYOUT_INVALID)
        أرجع 0;

    أرجع Xkb_کلید_عدد_پدھراں(کلید, layout);
}

/**
 * Return the total number of LEDs in the keymap.
 */
xkb_led_اشاریہ_t
xkb_keymap_عدد_leds(بنية xkb_keymap *keymap)
{
    أرجع keymap->عدد_leds;
}

/**
 * Returns the name for a given LED.
 */
ثابت حرف *
xkb_keymap_led_get_name(بنية xkb_keymap *keymap, xkb_led_اشاریہ_t idx)
{
    إذا (idx >= keymap->عدد_leds)
        أرجع عدم;

    أرجع xkb_atom_متن(keymap->ctx, keymap->leds[idx].name);
}

/**
 * Returns the index for a named LED.
 */
xkb_led_اشاریہ_t
xkb_keymap_led_get_اشاریہ(بنية xkb_keymap *keymap, ثابت حرف *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_led_اشاریہ_t i;
    ثابت بنية xkb_led *led;

    إذا (atom == XKB_ATOM_NONE)
        أرجع XKB_LED_INVALID;

    xkb_leds_enumerate(i, led, keymap)
        إذا (led->name == atom)
            أرجع i;

    أرجع XKB_LED_INVALID;
}

ناپ_t
xkb_keymap_کلید_get_mods_for_level(بنية xkb_keymap *keymap,
                                  xkb_keycode_t kc,
                                  xkb_layout_اشاریہ_t layout,
                                  xkb_level_اشاریہ_t level,
                                  xkb_mod_نقاب_t *masks_باہرلا,
                                  ناپ_t masks_ناپ)
{
    ثابت بنية xkb_کلید *کلید = Xkb_کلید(keymap, kc);
    إذا (!کلید)
        أرجع 0;

    layout = xkb_keymap_کلید_effective_layout(کلید, layout);
    إذا (layout == XKB_LAYOUT_INVALID)
        أرجع 0;

    إذا (level >= Xkb_کلید_عدد_پدھراں(کلید, layout))
        أرجع 0;

    ثابت بنية xkb_کلید_type *type = کلید->groups[layout].type;

    ناپ_t گنتی = 0;

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
        bool خالی_تھاواں_mapped = false;
        لكل (darray_ناپ_t i = 0; i < type->عدد_entries && گنتی < masks_ناپ; i++)
            إذا (اندراج_is_سرگرم(&type->entries[i]) &&
                type->entries[i].mods.نقاب == 0) {
                خالی_تھاواں_mapped = true;
                اكسر;
            }
        إذا (!خالی_تھاواں_mapped && گنتی < masks_ناپ) {
            masks_باہرلا[گنتی++] = 0;
        }
    }

    /* Now search explicit mappings. */
    لكل (darray_ناپ_t i = 0; i < type->عدد_entries && گنتی < masks_ناپ; i++) {
        إذا (اندراج_is_سرگرم(&type->entries[i]) &&
            type->entries[i].level == level) {
            masks_باہرلا[گنتی++] = type->entries[i].mods.نقاب;
        }
    }

    أرجع گنتی;
}

بنية xkb_level *
xkb_keymap_کلید_get_level(بنية xkb_keymap *keymap, ثابت بنية xkb_کلید *کلید,
                         xkb_layout_اشاریہ_t layout, xkb_level_اشاریہ_t level)
{
    layout = xkb_keymap_کلید_effective_layout(کلید, layout);
    إذا (layout == XKB_LAYOUT_INVALID)
        أرجع عدم;

    إذا (level >= Xkb_کلید_عدد_پدھراں(کلید, layout))
        أرجع عدم;

    أرجع &کلید->groups[layout].پدھراں[level];
}

/**
 * As below, but takes an explicit layout/level rather than state.
 */
صحيح
xkb_keymap_کلید_get_syms_by_level(بنية xkb_keymap *keymap,
                                 xkb_keycode_t kc,
                                 xkb_layout_اشاریہ_t layout,
                                 xkb_level_اشاریہ_t level,
                                 ثابت xkb_keysym_t **syms_باہرلا)
{
    ثابت بنية xkb_کلید *کلید = Xkb_کلید(keymap, kc);

    إذا (!کلید)
        اذهب err;

    ثابت بنية xkb_level* ثابت leveli =
        xkb_keymap_کلید_get_level(keymap, کلید, layout, level);

    إذا (!leveli)
        اذهب err;

    ثابت xkb_keysym_گنتی_t عدد_syms = leveli->عدد_syms;
    إذا (عدد_syms == 0)
        اذهب err;

    إذا (عدد_syms == 1)
        *syms_باہرلا = &leveli->s.sym;
    وإلا
        *syms_باہرلا = leveli->s.syms;

    أرجع (صحيح) عدد_syms;

err:
    *syms_باہرلا = عدم;
    أرجع 0;
}

xkb_keycode_t
xkb_keymap_min_keycode(بنية xkb_keymap *keymap)
{
    أرجع keymap->min_کلید_کوڈ;
}

xkb_keycode_t
xkb_keymap_max_keycode(بنية xkb_keymap *keymap)
{
    أرجع keymap->max_کلید_کوڈ;
}

ساكن تعداد xkb_حالت
keymap_کلید_iterator_config_check(
    بنية xkb_context * محدود ctx,
    ثابت حرف *func,
    ثابت بنية xkb_keymap_کلید_iterator_config * محدود config
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
        (config->flags & ~(uint32_t)XKB_KEYMAP_کلید_ITERATOR_FLAGS_اقدار);
    إذا (invalid_flags) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_کلید_ITERATOR_FLAGS_2,
                "%s: Unsupported keymap iterator flags: 0x%"PRIx32"\n",
                func, invalid_flags);
        أرجع XKB_ERROR_UNSUPPORTED_کلید_ITERATOR_FLAGS;
    }

    /* Sanitize input: start */
    إذا (config->شروع > XKB_KEYCODE_MAX)
        أرجع XKB_ERROR_INVALID_KEYCODE;

    أرجع XKB_SUCCESS;
}

static_assert(XKB_KEYMAP_کلید_ITERATOR_FLAGS_اقدار <=
              (صحيح)XKB_اشارہ_گر_ٹیگ_نقاب,
              "Cannot tag cursor pointer with flags");

#تعریف_کرو iter_init_keymap(i, ptr, flags) {          \
    (i)->opaque0 = xkb_اشارہ_گر_ٹیگ((ptr), (flags));\
}
#تعریف_کرو iter_get_keymap(i) (                                  \
    (ثابت بنية xkb_keymap *)xkb_اشارہ_گر_untag((i)->opaque0)\
)
#تعریف_کرو iter_get_اگلا(i) ((ثابت بنية xkb_کلید *)(i)->opaque1)
#تعریف_کرو iter_سیٹ_اگلا(i, ptr) {(i)->opaque1 = (ثابت فراغ *)(ptr);}
#تعریف_کرو iter_get_descending_ترتیب(i) (                  \
    xkb_اشارہ_گر_get_ٹیگ((i)->opaque0) &                 \
    (uintptr_t)XKB_KEYMAP_کلید_ITERATOR_DESCENDING_ترتیب \
)
#تعریف_کرو iter_get_include_unbound(i) (                  \
    xkb_اشارہ_گر_get_ٹیگ((i)->opaque0) &                \
    (uintptr_t)XKB_KEYMAP_کلید_ITERATOR_INCLUDE_UNBOUND \
)

تعداد xkb_حالت
xkb_keymap_کلید_iterator_init(
    بنية xkb_keymap_کلید_iterator * محدود iter,
    ثابت بنية xkb_keymap * محدود keymap,
    ثابت بنية xkb_keymap_کلید_iterator_config * محدود config
)
{
    /* Handle default configuration */
    ساكن ثابت بنية xkb_keymap_کلید_iterator_config default_config = {
        .ناپ = حجم(default_config)
    };
    إذا (!config)
        config = &default_config;

    /* Check input */
    ثابت تعداد xkb_حالت حالت =
        keymap_کلید_iterator_config_check(keymap->ctx, __func__, config);
    إذا (حالت != XKB_SUCCESS) {
        أرجع حالت;
    }

    /*
     * Initialization
     */

    memset(iter, 0, حجم(*iter));

    /* Borrow keymap */
    iter_init_keymap(iter, keymap, config->flags);

    إذا (keymap->عدد_کلیدیں == 0) {
        iter_سیٹ_اگلا(iter, عدم);
        أرجع XKB_SUCCESS;
    }

    /* Borrow key */
    ثابت بنية xkb_کلید *اگلا;
    إذا (config->flags & XKB_KEYMAP_کلید_ITERATOR_DESCENDING_ترتیب) {
        إذا (!config->شروع || config->شروع > keymap->max_کلید_کوڈ) {
            اگلا = &keymap->کلیدیں[keymap->عدد_کلیدیں - 1];
        } وإلا {
            /*
             * May still fail *safely* if start < keymap->min_key_code:
             * there will be simply no key to iterate over.
             */
            اگلا = xkb_keymap_get_اگلا_defined_کلید(keymap, false, config->شروع);
        }
    } وإلا {
        إذا (!config->شروع || config->شروع < keymap->min_کلید_کوڈ) {
            اگلا = (keymap->عدد_کلیدیں_کم)
                ? &keymap->کلیدیں[keymap->min_کلید_کوڈ]
                : &keymap->کلیدیں[0];
        } وإلا {
            /*
             * May still fail *safely* if start > keymap->max_key_code:
             * there will be simply no key to iterate over.
             */
            اگلا = xkb_keymap_get_اگلا_defined_کلید(keymap, true, config->شروع);
        }
    };
    iter_سیٹ_اگلا(iter, اگلا);

    أرجع XKB_SUCCESS;
}

xkb_keycode_t
xkb_keymap_کلید_iterator_اگلا(بنية xkb_keymap_کلید_iterator *iter)
{
    ثابت بنية xkb_کلید *اگلا = iter_get_اگلا(iter);
    إذا (!اگلا)
        أرجع XKB_KEYCODE_INVALID;

    ثابت بنية xkb_keymap *keymap = iter_get_keymap(iter);
    أكد(keymap->عدد_کلیدیں);

    ثابت bool descending = iter_get_descending_ترتیب(iter);
    ثابت bool skip_unbound = !iter_get_include_unbound(iter);
    ثابت بنية xkb_کلید *آخری = descending
        ? (keymap->عدد_کلیدیں_کم)
            ? &keymap->کلیدیں[keymap->min_کلید_کوڈ]
            : &keymap->کلیدیں[0]
        : &keymap->کلیدیں[keymap->عدد_کلیدیں - 1];
    ثابت ptrdiff_t قدم = descending ? -1 : 1;

    /* Skip undefined keys (no name) and optionally unbound keys */
    لكل (;;) {
        ثابت bool iter_سرا = descending ? (اگلا <= آخری) : (اگلا >= آخری);
        ثابت bool skip = (
            اگلا->name == XKB_ATOM_NONE ||
            (skip_unbound && (اگلا->عدد_groups == 0))
        );
        إذا (!skip) {
            ثابت xkb_keycode_t ret = اگلا->keycode;
            iter_سیٹ_اگلا(iter, iter_سرا ? عدم : اگلا + قدم);
            أرجع ret;
        }
        إذا (iter_سرا) {
            iter_سیٹ_اگلا(iter, عدم);
            أرجع XKB_KEYCODE_INVALID;
        }
        اگلا += قدم;
    }
}

#تعریف_رد iter_init_keymap
#تعریف_رد iter_get_keymap
#تعریف_رد iter_سیٹ_اگلا
#تعریف_رد iter_get_اگلا
#تعریف_رد iter_get_descending_ترتیب
#تعریف_رد iter_get_include_unbound

فراغ
xkb_keymap_کلید_for_each(بنية xkb_keymap *keymap, xkb_keymap_کلید_iter_t iter,
                        فراغ *انکڑے)
{
    بنية xkb_کلید *کلید;

    xkb_کلیدیں_foreach(کلید, keymap)
        iter(keymap, کلید->keycode, انکڑے);
}

ثابت حرف *
xkb_keymap_کلید_get_name(بنية xkb_keymap *keymap, xkb_keycode_t kc)
{
    ثابت بنية xkb_کلید *کلید = Xkb_کلید(keymap, kc);

    إذا (!کلید)
        أرجع عدم;

    أرجع xkb_atom_متن(keymap->ctx, کلید->name);
}

xkb_keycode_t
xkb_keymap_کلید_by_name(بنية xkb_keymap *keymap, ثابت حرف *name)
{
    بنية xkb_کلید *کلید;
    xkb_atom_t atom;

    atom = xkb_atom_lookup(keymap->ctx, name);
    إذا (atom) {
        لكل (darray_ناپ_t i = 0; i < keymap->عدد_کلید_aliases; i++)
            إذا (keymap->کلید_aliases[i].alias == atom)
                atom = keymap->کلید_aliases[i].real;
    }
    إذا (!atom)
        أرجع XKB_KEYCODE_INVALID;

    xkb_کلیدیں_foreach(کلید, keymap) {
        إذا (کلید->name == atom)
            أرجع کلید->keycode;
    }

    أرجع XKB_KEYCODE_INVALID;
}

/**
 * Simple boolean specifying whether or not the key should repeat.
 */
صحيح
xkb_keymap_کلید_repeats(بنية xkb_keymap *keymap, xkb_keycode_t kc)
{
    ثابت بنية xkb_کلید *کلید = Xkb_کلید(keymap, kc);

    إذا (!کلید)
        أرجع 0;

    أرجع کلید->repeats;
}
