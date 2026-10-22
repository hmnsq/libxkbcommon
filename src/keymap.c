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

ساخت xkb_keymap *
xkb_keymap_ref(ساخت xkb_keymap *keymap)
{
    أكد(keymap->refcnt > 0);
    keymap->refcnt++;
    واپس keymap;
}

خالی
clear_level(ساخت xkb_level *leveli)
{
    اگر (leveli->عدد_syms > 1)
        آزاد(leveli->s.syms);
    اگر (leveli->عدد_actions > 1)
        آزاد(leveli->a.actions);
}

ساکن خالی
clear_interpret(ساخت xkb_sym_interpret *interp)
{
    اگر (interp->عدد_actions > 1)
        آزاد(interp->a.actions);
}

خالی
xkb_keymap_unref(ساخت xkb_keymap *keymap)
{
    أكد(!keymap || keymap->refcnt > 0);
    اگر (!keymap || --keymap->refcnt > 0)
        واپس;

    اگر (keymap->کلیدیں) {
        ساخت xkb_کلید *کلید;
        xkb_کلیدیں_foreach(کلید, keymap) {
            اگر (کلید->groups) {
                برائے (xkb_layout_اشاریہ_t i = 0; i < کلید->عدد_groups; i++) {
                    اگر (کلید->groups[i].سطحیں) {
                        برائے (xkb_level_اشاریہ_t j = 0;
                             j < Xkb_کلید_عدد_سطحیں(کلید, i);
                             j++)
                            clear_level(&کلید->groups[i].سطحیں[j]);
                        آزاد(کلید->groups[i].سطحیں);
                    }
                }
                آزاد(کلید->groups);
            }
            اگر (!کلید->overlays_inline && کلید->overlays_کلیدیں) {
                آزاد((خالی *)کلید->overlays_کلیدیں);
            }
        }
        آزاد(keymap->کلیدیں);
    }
    اگر (keymap->types) {
        برائے (darray_سائز_t i = 0; i < keymap->عدد_types; i++) {
            آزاد(keymap->types[i].entries);
            آزاد(keymap->types[i].level_نام);
        }
        آزاد(keymap->types);
    }
    برائے (darray_سائز_t k = 0; k < keymap->عدد_sym_interprets; k++) {
        clear_interpret(&keymap->sym_interprets[k]);
    }
    آزاد(keymap->sym_interprets);
    آزاد(keymap->modmaps);
    آزاد(keymap->کلید_aliases);
    آزاد(keymap->گروہ_نام);
    آزاد(keymap->keycodes_section_name);
    آزاد(keymap->symbols_section_name);
    آزاد(keymap->types_section_name);
    آزاد(keymap->compat_section_name);
    xkb_context_unref(keymap->ctx);
    آزاد(keymap);
}

ساکن ثابت ساخت xkb_keymap_format_ops *
get_keymap_format_ops(شمار xkb_keymap_format format)
{
    ساکن ثابت ساخت xkb_keymap_format_ops *keymap_format_ops[] = {
        [XKB_KEYMAP_FORMAT_متن_V1] = &متن_v1_keymap_format_ops,
        [XKB_KEYMAP_FORMAT_متن_V2] = &متن_v1_keymap_format_ops,
    };

    اگر ((صحیح) format < 0 || (صحیح) format >= (صحیح) ARRAY_سائز(keymap_format_ops))
        واپس تہی;

    واپس keymap_format_ops[(صحیح) format];
}

ساخت xkb_keymap *
xkb_keymap_new_from_rmlvo(ثابت ساخت xkb_rmlvo_builder *rmlvo,
                          شمار xkb_keymap_format format,
                          شمار xkb_keymap_compile_flags flags)
{
    ثابت ساخت xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    اگر (!ops || !ops->keymap_new_from_rmlvo) {
        log_err_func(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        واپس تہی;
    }

    ساخت xkb_keymap *keymap = xkb_keymap_new(rmlvo->ctx, __func__, format,
                                               flags);
    اگر (!keymap)
        واپس تہی;

    اگر (!ops->keymap_new_from_rmlvo(keymap, rmlvo)) {
        xkb_keymap_unref(keymap);
        واپس تہی;
    }

    واپس keymap;
}

ساخت xkb_keymap *
xkb_keymap_new_from_names2(ساخت xkb_context *ctx,
                           ثابت ساخت xkb_قاعدہ_نام *rmlvo_اندر,
                           شمار xkb_keymap_format format,
                           شمار xkb_keymap_compile_flags flags)
{
    ثابت ساخت xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    اگر (!ops || !ops->keymap_new_from_نام) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        واپس تہی;
    }

    ساخت xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    اگر (!keymap)
        واپس تہی;

    ساخت xkb_قاعدہ_نام rmlvo = {0};
    اگر (rmlvo_اندر)
        rmlvo = *rmlvo_اندر;
    xkb_context_sanitize_قاعدہ_نام(ctx, &rmlvo);

    اگر (!ops->keymap_new_from_نام(keymap, &rmlvo)) {
        xkb_keymap_unref(keymap);
        واپس تہی;
    }

    واپس keymap;
}


ساخت xkb_keymap *
xkb_keymap_new_from_نام(ساخت xkb_context *ctx,
                          ثابت ساخت xkb_قاعدہ_نام *rmlvo_اندر,
                          شمار xkb_keymap_compile_flags flags)
{
    واپس xkb_keymap_new_from_names2(ctx, rmlvo_اندر,
                                      XKB_KEYMAP_FORMAT_متن_V2, flags);
}

ساخت xkb_keymap *
xkb_keymap_new_from_string(ساخت xkb_context *ctx,
                           ثابت حرف *string,
                           شمار xkb_keymap_format format,
                           شمار xkb_keymap_compile_flags flags)
{
    واپس xkb_keymap_new_from_بفر(ctx, string, لڑی_لمبائی(string),
                                      format, flags);
}

ساخت xkb_keymap *
xkb_keymap_new_from_بفر(ساخت xkb_context *ctx,
                           ثابت حرف *بفر, سائز_t لمبائی,
                           شمار xkb_keymap_format format,
                           شمار xkb_keymap_compile_flags flags)
{
    ثابت ساخت xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    اگر (!ops || !ops->keymap_new_from_string) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        واپس تہی;
    }

    اگر (!بفر) {
        log_err_func1(ctx, XKB_LOG_پیغام_NO_ID,
                      "no buffer specified\n");
        واپس تہی;
    }

    ساخت xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    اگر (!keymap)
        واپس تہی;

    /* Allow a zero-terminated string as a buffer */
    اگر (لمبائی > 0 && بفر[لمبائی - 1] == '\0')
        لمبائی--;

    اگر (!ops->keymap_new_from_string(keymap, بفر, لمبائی)) {
        xkb_keymap_unref(keymap);
        واپس تہی;
    }

    واپس keymap;
}

ساخت xkb_keymap *
xkb_keymap_new_from_file(ساخت xkb_context *ctx,
                         FILE *file,
                         شمار xkb_keymap_format format,
                         شمار xkb_keymap_compile_flags flags)
{
    ثابت ساخت xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    اگر (!ops || !ops->keymap_new_from_file) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        واپس تہی;
    }

    اگر (!file) {
        log_err_func1(ctx, XKB_LOG_پیغام_NO_ID,
                      "no file specified\n");
        واپس تہی;
    }

    ساخت xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    اگر (!keymap)
        واپس تہی;

    اگر (!ops->keymap_new_from_file(keymap, file)) {
        xkb_keymap_unref(keymap);
        واپس تہی;
    }

    واپس keymap;
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
ساکن شمار xkb_حالت
check_keymap_serialize_abi(
    ساخت xkb_context * محدود ctx,
    ثابت حرف * محدود func,
    ثابت ساخت xkb_keymap_serialize_config * محدود config,
    ثابت ساخت xkb_keymap_serialize_نتیجہ * محدود نتیجہ
)
{
    شمار xkb_حالت حالت_2;
    اگر ((حالت_2 = xkb_check_keymap_abi(config)) ||
        (حالت_2 = xkb_check_keymap_abi(نتیجہ))) {
        xkb_log_abi_error(ctx, func, حالت_2);
    }
    واپس حالت_2;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

شمار xkb_حالت
xkb_keymap_serialize(ثابت ساخت xkb_keymap *keymap,
                     ثابت ساخت xkb_keymap_serialize_config *config,
                     ساخت xkb_keymap_serialize_نتیجہ *نتیجہ)
{
    /* Check ABI compatibility */
    ثابت شمار xkb_حالت حالت_2 =
        check_keymap_serialize_abi(keymap->ctx, __func__, config, نتیجہ);
    اگر (حالت_2 != XKB_SUCCESS)
        واپس حالت_2;

    ساخت xkb_keymap_serialize_config new_config = *config;

    ثابت شمار xkb_keymap_serialize_flags invalid_flags = (
        new_config.flags &
        ~(شمار xkb_keymap_serialize_flags)XKB_KEYMAP_SERIALIZE_FLAGS_اقدار
    );
    اگر (invalid_flags) {
        log_err_func(keymap->ctx,
                     XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS_,
                     "unrecognized serialization flags: %#x\n",
                     invalid_flags);
        واپس XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS;
    }

    اگر (new_config.format == XKB_KEYMAP_USE_ORIGINAL_FORMAT)
        new_config.format = keymap->format;

    ثابت ساخت xkb_keymap_format_ops * ثابت ops =
        get_keymap_format_ops(new_config.format);
    اگر (!ops || !ops->keymap_serialize) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", new_config.format);
        واپس XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT;
    }

    ثابت xkb_layout_نقاب_t all_layouts =
        (xkb_layout_نقاب_t)((UINT64_C(1) << keymap->عدد_groups) - 1);
    اگر (!new_config.layouts) {
        new_config.layouts = all_layouts;
    }

    اگر (new_config.layouts & ~all_layouts) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                     "unsupported layout mask: "
                     "expected subset of 0x%08"PRIx32", got: 0x%08"PRIx32"\n",
                     all_layouts, new_config.layouts);
        واپس XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ;
    }

    ثابت xkb_layout_اشاریہ_t max_groups = format_max_groups(config->format);
    ثابت xkb_layout_اشاریہ_t عدد_groups =
        (xkb_layout_اشاریہ_t)popcount32(new_config.layouts);

    اگر (عدد_groups > max_groups) {
        ثابت bool strict =
            (new_config.flags & XKB_KEYMAP_SERIALIZE_STRICT_منوال);
        xkb_log_with_رمز(
            keymap->ctx,
            (strict ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
            XKB_LOG_VERBOSITY_اقل,
            XKB_ERROR_LAYOUT_گنتی_حد_بندی_EXCEEDED_2,
            "Cannot serialize %"PRIu32" groups in keymap format %d: "
            "maximum is %"PRIu32"%s\n",
            عدد_groups, config->format, max_groups,
            (strict ? "" : "; discarding unsupported groups")
        );
        اگر (strict) {
            واپس XKB_ERROR_LAYOUT_گنتی_حد_بندی_EXCEEDED;
        } ورنہ {
            new_config.layouts =
                keep_سب_سے_نیچا_n_سیٹ_بٹ(new_config.layouts, max_groups);
        }
    }

    log_dbg(keymap->ctx, XKB_LOG_پیغام_NO_ID,
            "Serializing group mask 0x%"PRIx32" (total: %"PRIu32")\n",
            new_config.layouts, عدد_groups);

    اگر (new_config.layouts != all_layouts) {
        /*
         * If some layouts are discarded, then interprets may not trigger
         * properly. Avoid this by requiring explicit values.
         */
        new_config.flags |= (XKB_KEYMAP_SERIALIZE_EXPLICIT_کلید_اقدار |
                             XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS);
    }

    واپس ops->keymap_serialize(keymap, &new_config, نتیجہ);
}

حرف *
xkb_keymap_get_as_string2(ساخت xkb_keymap *keymap,
                          شمار xkb_keymap_format format,
                          شمار xkb_keymap_serialize_flags flags)
{
    ثابت ساخت xkb_keymap_serialize_config config = {
        .سائز = حجم(config),
        .flags = flags,
        .format = format,
        .layouts = 0,
    };

    ساخت xkb_keymap_serialize_نتیجہ نتیجہ = { .سائز = حجم(نتیجہ) };

    واپس (xkb_keymap_serialize(keymap, &config, &نتیجہ) == XKB_SUCCESS)
        ? نتیجہ.serialized
        : تہی;
}

حرف *
xkb_keymap_get_as_string(ساخت xkb_keymap *keymap,
                         شمار xkb_keymap_format format)
{
    واپس xkb_keymap_get_as_string2(keymap, format,
                                     XKB_KEYMAP_SERIALIZE_NO_FLAGS);
}

/**
 * Returns the total number of modifiers active in the keymap.
 */
xkb_mod_اشاریہ_t
xkb_keymap_عدد_mods(ساخت xkb_keymap *keymap)
{
    واپس keymap->mods.عدد_mods;
}

/**
 * Return the name for a given modifier.
 */
ثابت حرف *
xkb_keymap_mod_get_name(ساخت xkb_keymap *keymap, xkb_mod_اشاریہ_t idx)
{
    اگر (idx >= keymap->mods.عدد_mods)
        واپس تہی;

    واپس xkb_atom_متن(keymap->ctx, keymap->mods.mods[idx].name);
}

/**
 * Returns the index for a named modifier.
 */
xkb_mod_اشاریہ_t
xkb_keymap_mod_get_اشاریہ(ساخت xkb_keymap *keymap, ثابت حرف *name)
{
    ثابت xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    واپس (atom == XKB_ATOM_NONE)
        ? XKB_MOD_INVALID
        : Xkb_Mod_Name_مقصد_اشاریہ(&keymap->mods, atom, MOD_BOTH);
}

/**
 * Return the canonical mapping of a named modifier.
 */
xkb_mod_نقاب_t
xkb_keymap_mod_get_نقاب(ساخت xkb_keymap *keymap, ثابت حرف* name)
{
    ثابت xkb_mod_اشاریہ_t idx = xkb_keymap_mod_get_اشاریہ(keymap, name);
    واپس (idx >= keymap->mods.عدد_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the canonical mapping of a given modifier.
 */
xkb_mod_نقاب_t
xkb_keymap_mod_get_mask2(ساخت xkb_keymap *keymap, xkb_mod_اشاریہ_t idx)
{
    واپس (idx >= keymap->mods.عدد_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the total number of active groups in the keymap.
 */
xkb_layout_اشاریہ_t
xkb_keymap_عدد_layouts(ساخت xkb_keymap *keymap)
{
    واپس keymap->عدد_groups;
}

/**
 * Returns the name for a given group.
 */
ثابت حرف *
xkb_keymap_layout_get_name(ساخت xkb_keymap *keymap, xkb_layout_اشاریہ_t idx)
{
    اگر (idx >= keymap->عدد_گروہ_نام)
        واپس تہی;

    واپس xkb_atom_متن(keymap->ctx, keymap->گروہ_نام[idx]);
}

/**
 * Returns the index for a named layout.
 */
xkb_layout_اشاریہ_t
xkb_keymap_layout_get_اشاریہ(ساخت xkb_keymap *keymap, ثابت حرف *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_layout_اشاریہ_t i;

    اگر (atom == XKB_ATOM_NONE)
        واپس XKB_LAYOUT_INVALID;

    برائے (i = 0; i < keymap->عدد_گروہ_نام; i++)
        اگر (keymap->گروہ_نام[i] == atom)
            واپس i;

    واپس XKB_LAYOUT_INVALID;
}

/**
 * Returns the number of layouts active for a particular key.
 */
xkb_layout_اشاریہ_t
xkb_keymap_عدد_layouts_for_کلید(ساخت xkb_keymap *keymap, xkb_keycode_t kc)
{
    ثابت ساخت xkb_کلید *کلید = Xkb_کلید(keymap, kc);

    اگر (!کلید)
        واپس 0;

    واپس کلید->عدد_groups;
}

/**
 * Returns the number of levels active for a particular key and layout.
 */
xkb_level_اشاریہ_t
xkb_keymap_عدد_سطحیں_for_کلید(ساخت xkb_keymap *keymap, xkb_keycode_t kc,
                              xkb_layout_اشاریہ_t layout)
{
    ثابت ساخت xkb_کلید *کلید = Xkb_کلید(keymap, kc);

    اگر (!کلید)
        واپس 0;

    layout = xkb_keymap_کلید_effective_layout(کلید, layout);
    اگر (layout == XKB_LAYOUT_INVALID)
        واپس 0;

    واپس Xkb_کلید_عدد_سطحیں(کلید, layout);
}

/**
 * Return the total number of LEDs in the keymap.
 */
xkb_led_اشاریہ_t
xkb_keymap_عدد_leds(ساخت xkb_keymap *keymap)
{
    واپس keymap->عدد_leds;
}

/**
 * Returns the name for a given LED.
 */
ثابت حرف *
xkb_keymap_led_get_name(ساخت xkb_keymap *keymap, xkb_led_اشاریہ_t idx)
{
    اگر (idx >= keymap->عدد_leds)
        واپس تہی;

    واپس xkb_atom_متن(keymap->ctx, keymap->leds[idx].name);
}

/**
 * Returns the index for a named LED.
 */
xkb_led_اشاریہ_t
xkb_keymap_led_get_اشاریہ(ساخت xkb_keymap *keymap, ثابت حرف *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_led_اشاریہ_t i;
    ثابت ساخت xkb_led *led;

    اگر (atom == XKB_ATOM_NONE)
        واپس XKB_LED_INVALID;

    xkb_leds_enumerate(i, led, keymap)
        اگر (led->name == atom)
            واپس i;

    واپس XKB_LED_INVALID;
}

سائز_t
xkb_keymap_کلید_get_mods_for_level(ساخت xkb_keymap *keymap,
                                  xkb_keycode_t kc,
                                  xkb_layout_اشاریہ_t layout,
                                  xkb_level_اشاریہ_t level,
                                  xkb_mod_نقاب_t *masks_برآمد,
                                  سائز_t masks_سائز)
{
    ثابت ساخت xkb_کلید *کلید = Xkb_کلید(keymap, kc);
    اگر (!کلید)
        واپس 0;

    layout = xkb_keymap_کلید_effective_layout(کلید, layout);
    اگر (layout == XKB_LAYOUT_INVALID)
        واپس 0;

    اگر (level >= Xkb_کلید_عدد_سطحیں(کلید, layout))
        واپس 0;

    ثابت ساخت xkb_کلید_type *type = کلید->groups[layout].type;

    سائز_t گنتی = 0;

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
        bool خالی_خانے_mapped = false;
        برائے (darray_سائز_t i = 0; i < type->عدد_entries && گنتی < masks_سائز; i++)
            اگر (اندراج_is_فعال(&type->entries[i]) &&
                type->entries[i].mods.نقاب == 0) {
                خالی_خانے_mapped = true;
                توڑو;
            }
        اگر (!خالی_خانے_mapped && گنتی < masks_سائز) {
            masks_برآمد[گنتی++] = 0;
        }
    }

    /* Now search explicit mappings. */
    برائے (darray_سائز_t i = 0; i < type->عدد_entries && گنتی < masks_سائز; i++) {
        اگر (اندراج_is_فعال(&type->entries[i]) &&
            type->entries[i].level == level) {
            masks_برآمد[گنتی++] = type->entries[i].mods.نقاب;
        }
    }

    واپس گنتی;
}

ساخت xkb_level *
xkb_keymap_کلید_get_level(ساخت xkb_keymap *keymap, ثابت ساخت xkb_کلید *کلید,
                         xkb_layout_اشاریہ_t layout, xkb_level_اشاریہ_t level)
{
    layout = xkb_keymap_کلید_effective_layout(کلید, layout);
    اگر (layout == XKB_LAYOUT_INVALID)
        واپس تہی;

    اگر (level >= Xkb_کلید_عدد_سطحیں(کلید, layout))
        واپس تہی;

    واپس &کلید->groups[layout].سطحیں[level];
}

/**
 * As below, but takes an explicit layout/level rather than state.
 */
صحیح
xkb_keymap_کلید_get_syms_by_level(ساخت xkb_keymap *keymap,
                                 xkb_keycode_t kc,
                                 xkb_layout_اشاریہ_t layout,
                                 xkb_level_اشاریہ_t level,
                                 ثابت xkb_keysym_t **syms_برآمد)
{
    ثابت ساخت xkb_کلید *کلید = Xkb_کلید(keymap, kc);

    اگر (!کلید)
        جاؤ err;

    ثابت ساخت xkb_level* ثابت leveli =
        xkb_keymap_کلید_get_level(keymap, کلید, layout, level);

    اگر (!leveli)
        جاؤ err;

    ثابت xkb_keysym_گنتی_t عدد_syms = leveli->عدد_syms;
    اگر (عدد_syms == 0)
        جاؤ err;

    اگر (عدد_syms == 1)
        *syms_برآمد = &leveli->s.sym;
    ورنہ
        *syms_برآمد = leveli->s.syms;

    واپس (صحیح) عدد_syms;

err:
    *syms_برآمد = تہی;
    واپس 0;
}

xkb_keycode_t
xkb_keymap_min_keycode(ساخت xkb_keymap *keymap)
{
    واپس keymap->min_کلید_رمز;
}

xkb_keycode_t
xkb_keymap_max_keycode(ساخت xkb_keymap *keymap)
{
    واپس keymap->max_کلید_رمز;
}

ساکن شمار xkb_حالت
keymap_کلید_iterator_config_check(
    ساخت xkb_context * محدود ctx,
    ثابت حرف *func,
    ثابت ساخت xkb_keymap_کلید_iterator_config * محدود config
)
{
    /* Check ABI compatibility */
    ثابت شمار xkb_حالت abi_error = xkb_check_keymap_abi(config);
    اگر (abi_error) {
        xkb_log_abi_error(ctx, func, abi_error);
        واپس abi_error;
    }

    /* Sanitize input: flags */
    ثابت uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_KEYMAP_کلید_ITERATOR_FLAGS_اقدار);
    اگر (invalid_flags) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_کلید_ITERATOR_FLAGS_2,
                "%s: Unsupported keymap iterator flags: 0x%"PRIx32"\n",
                func, invalid_flags);
        واپس XKB_ERROR_UNSUPPORTED_کلید_ITERATOR_FLAGS;
    }

    /* Sanitize input: start */
    اگر (config->آغاز > XKB_KEYCODE_MAX)
        واپس XKB_ERROR_INVALID_KEYCODE;

    واپس XKB_SUCCESS;
}

static_assert(XKB_KEYMAP_کلید_ITERATOR_FLAGS_اقدار <=
              (صحیح)XKB_اشارہ_گر_لیبل_نقاب,
              "Cannot tag cursor pointer with flags");

#تعریف_کرو iter_init_keymap(i, ptr, flags) {          \
    (i)->opaque0 = xkb_اشارہ_گر_لیبل((ptr), (flags));\
}
#تعریف_کرو iter_get_keymap(i) (                                  \
    (ثابت ساخت xkb_keymap *)xkb_اشارہ_گر_untag((i)->opaque0)\
)
#تعریف_کرو iter_get_اگلا(i) ((ثابت ساخت xkb_کلید *)(i)->opaque1)
#تعریف_کرو iter_سیٹ_اگلا(i, ptr) {(i)->opaque1 = (ثابت خالی *)(ptr);}
#تعریف_کرو iter_get_descending_ترتیب(i) (                  \
    xkb_اشارہ_گر_get_لیبل((i)->opaque0) &                 \
    (uintptr_t)XKB_KEYMAP_کلید_ITERATOR_DESCENDING_ترتیب \
)
#تعریف_کرو iter_get_include_unbound(i) (                  \
    xkb_اشارہ_گر_get_لیبل((i)->opaque0) &                \
    (uintptr_t)XKB_KEYMAP_کلید_ITERATOR_INCLUDE_UNBOUND \
)

شمار xkb_حالت
xkb_keymap_کلید_iterator_init(
    ساخت xkb_keymap_کلید_iterator * محدود iter,
    ثابت ساخت xkb_keymap * محدود keymap,
    ثابت ساخت xkb_keymap_کلید_iterator_config * محدود config
)
{
    /* Handle default configuration */
    ساکن ثابت ساخت xkb_keymap_کلید_iterator_config default_config = {
        .سائز = حجم(default_config)
    };
    اگر (!config)
        config = &default_config;

    /* Check input */
    ثابت شمار xkb_حالت حالت_2 =
        keymap_کلید_iterator_config_check(keymap->ctx, __func__, config);
    اگر (حالت_2 != XKB_SUCCESS) {
        واپس حالت_2;
    }

    /*
     * Initialization
     */

    memset(iter, 0, حجم(*iter));

    /* Borrow keymap */
    iter_init_keymap(iter, keymap, config->flags);

    اگر (keymap->عدد_کلیدیں == 0) {
        iter_سیٹ_اگلا(iter, تہی);
        واپس XKB_SUCCESS;
    }

    /* Borrow key */
    ثابت ساخت xkb_کلید *اگلا;
    اگر (config->flags & XKB_KEYMAP_کلید_ITERATOR_DESCENDING_ترتیب) {
        اگر (!config->آغاز || config->آغاز > keymap->max_کلید_رمز) {
            اگلا = &keymap->کلیدیں[keymap->عدد_کلیدیں - 1];
        } ورنہ {
            /*
             * May still fail *safely* if start < keymap->min_key_code:
             * there will be simply no key to iterate over.
             */
            اگلا = xkb_keymap_get_اگلا_defined_کلید(keymap, false, config->آغاز);
        }
    } ورنہ {
        اگر (!config->آغاز || config->آغاز < keymap->min_کلید_رمز) {
            اگلا = (keymap->عدد_کلیدیں_کم)
                ? &keymap->کلیدیں[keymap->min_کلید_رمز]
                : &keymap->کلیدیں[0];
        } ورنہ {
            /*
             * May still fail *safely* if start > keymap->max_key_code:
             * there will be simply no key to iterate over.
             */
            اگلا = xkb_keymap_get_اگلا_defined_کلید(keymap, true, config->آغاز);
        }
    };
    iter_سیٹ_اگلا(iter, اگلا);

    واپس XKB_SUCCESS;
}

xkb_keycode_t
xkb_keymap_کلید_iterator_اگلا(ساخت xkb_keymap_کلید_iterator *iter)
{
    ثابت ساخت xkb_کلید *اگلا = iter_get_اگلا(iter);
    اگر (!اگلا)
        واپس XKB_KEYCODE_INVALID;

    ثابت ساخت xkb_keymap *keymap = iter_get_keymap(iter);
    أكد(keymap->عدد_کلیدیں);

    ثابت bool descending = iter_get_descending_ترتیب(iter);
    ثابت bool skip_unbound = !iter_get_include_unbound(iter);
    ثابت ساخت xkb_کلید *آخری = descending
        ? (keymap->عدد_کلیدیں_کم)
            ? &keymap->کلیدیں[keymap->min_کلید_رمز]
            : &keymap->کلیدیں[0]
        : &keymap->کلیدیں[keymap->عدد_کلیدیں - 1];
    ثابت ptrdiff_t قدم = descending ? -1 : 1;

    /* Skip undefined keys (no name) and optionally unbound keys */
    برائے (;;) {
        ثابت bool iter_انتہا = descending ? (اگلا <= آخری) : (اگلا >= آخری);
        ثابت bool skip = (
            اگلا->name == XKB_ATOM_NONE ||
            (skip_unbound && (اگلا->عدد_groups == 0))
        );
        اگر (!skip) {
            ثابت xkb_keycode_t ret = اگلا->keycode;
            iter_سیٹ_اگلا(iter, iter_انتہا ? تہی : اگلا + قدم);
            واپس ret;
        }
        اگر (iter_انتہا) {
            iter_سیٹ_اگلا(iter, تہی);
            واپس XKB_KEYCODE_INVALID;
        }
        اگلا += قدم;
    }
}

#تعریف_منسوخ iter_init_keymap
#تعریف_منسوخ iter_get_keymap
#تعریف_منسوخ iter_سیٹ_اگلا
#تعریف_منسوخ iter_get_اگلا
#تعریف_منسوخ iter_get_descending_ترتیب
#تعریف_منسوخ iter_get_include_unbound

خالی
xkb_keymap_کلید_for_each(ساخت xkb_keymap *keymap, xkb_keymap_کلید_iter_t iter,
                        خالی *ڈیٹا)
{
    ساخت xkb_کلید *کلید;

    xkb_کلیدیں_foreach(کلید, keymap)
        iter(keymap, کلید->keycode, ڈیٹا);
}

ثابت حرف *
xkb_keymap_کلید_get_name(ساخت xkb_keymap *keymap, xkb_keycode_t kc)
{
    ثابت ساخت xkb_کلید *کلید = Xkb_کلید(keymap, kc);

    اگر (!کلید)
        واپس تہی;

    واپس xkb_atom_متن(keymap->ctx, کلید->name);
}

xkb_keycode_t
xkb_keymap_کلید_by_name(ساخت xkb_keymap *keymap, ثابت حرف *name)
{
    ساخت xkb_کلید *کلید;
    xkb_atom_t atom;

    atom = xkb_atom_lookup(keymap->ctx, name);
    اگر (atom) {
        برائے (darray_سائز_t i = 0; i < keymap->عدد_کلید_aliases; i++)
            اگر (keymap->کلید_aliases[i].alias == atom)
                atom = keymap->کلید_aliases[i].real;
    }
    اگر (!atom)
        واپس XKB_KEYCODE_INVALID;

    xkb_کلیدیں_foreach(کلید, keymap) {
        اگر (کلید->name == atom)
            واپس کلید->keycode;
    }

    واپس XKB_KEYCODE_INVALID;
}

/**
 * Simple boolean specifying whether or not the key should repeat.
 */
صحیح
xkb_keymap_کلید_repeats(ساخت xkb_keymap *keymap, xkb_keycode_t kc)
{
    ثابت ساخت xkb_کلید *کلید = Xkb_کلید(keymap, kc);

    اگر (!کلید)
        واپس 0;

    واپس کلید->repeats;
}
