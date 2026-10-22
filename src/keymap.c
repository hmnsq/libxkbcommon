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
    إذا (leveli->رقم_syms > 1)
        حرر(leveli->s.syms);
    إذا (leveli->رقم_actions > 1)
        حرر(leveli->a.actions);
}

ساكن فراغ
clear_interpret(بنية xkb_sym_interpret *interp)
{
    إذا (interp->رقم_actions > 1)
        حرر(interp->a.actions);
}

فراغ
xkb_keymap_unref(بنية xkb_keymap *keymap)
{
    أكد(!keymap || keymap->refcnt > 0);
    إذا (!keymap || --keymap->refcnt > 0)
        أرجع;

    إذا (keymap->کلیدها) {
        بنية xkb_کلید *کلید;
        xkb_کلیدها_foreach(کلید, keymap) {
            إذا (کلید->groups) {
                لكل (xkb_layout_شاخص_t i = 0; i < کلید->رقم_groups; i++) {
                    إذا (کلید->groups[i].سطوح) {
                        لكل (xkb_level_شاخص_t j = 0;
                             j < Xkb_کلید_رقم_سطوح(کلید, i);
                             j++)
                            clear_level(&کلید->groups[i].سطوح[j]);
                        حرر(کلید->groups[i].سطوح);
                    }
                }
                حرر(کلید->groups);
            }
            إذا (!کلید->overlays_inline && کلید->overlays_کلیدها) {
                حرر((فراغ *)کلید->overlays_کلیدها);
            }
        }
        حرر(keymap->کلیدها);
    }
    إذا (keymap->types) {
        لكل (darray_اندازه_t i = 0; i < keymap->رقم_types; i++) {
            حرر(keymap->types[i].entries);
            حرر(keymap->types[i].level_نامها);
        }
        حرر(keymap->types);
    }
    لكل (darray_اندازه_t k = 0; k < keymap->رقم_sym_interprets; k++) {
        clear_interpret(&keymap->sym_interprets[k]);
    }
    حرر(keymap->sym_interprets);
    حرر(keymap->modmaps);
    حرر(keymap->کلید_aliases);
    حرر(keymap->گروه_نامها);
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

    إذا ((صحيح) format < 0 || (صحيح) format >= (صحيح) ARRAY_اندازه(keymap_format_ops))
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
                           ثابت بنية xkb_قاعده_نامها *rmlvo_درون,
                           تعداد xkb_keymap_format format,
                           تعداد xkb_keymap_compile_flags flags)
{
    ثابت بنية xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    إذا (!ops || !ops->keymap_new_from_نامها) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        أرجع عدم;
    }

    بنية xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    إذا (!keymap)
        أرجع عدم;

    بنية xkb_قاعده_نامها rmlvo = {0};
    إذا (rmlvo_درون)
        rmlvo = *rmlvo_درون;
    xkb_context_sanitize_قاعده_نامها(ctx, &rmlvo);

    إذا (!ops->keymap_new_from_نامها(keymap, &rmlvo)) {
        xkb_keymap_unref(keymap);
        أرجع عدم;
    }

    أرجع keymap;
}


بنية xkb_keymap *
xkb_keymap_new_from_نامها(بنية xkb_context *ctx,
                          ثابت بنية xkb_قاعده_نامها *rmlvo_درون,
                          تعداد xkb_keymap_compile_flags flags)
{
    أرجع xkb_keymap_new_from_names2(ctx, rmlvo_درون,
                                      XKB_KEYMAP_FORMAT_متن_V2, flags);
}

بنية xkb_keymap *
xkb_keymap_new_from_string(بنية xkb_context *ctx,
                           ثابت حرف *string,
                           تعداد xkb_keymap_format format,
                           تعداد xkb_keymap_compile_flags flags)
{
    أرجع xkb_keymap_new_from_میانگیر(ctx, string, طول_رشته(string),
                                      format, flags);
}

بنية xkb_keymap *
xkb_keymap_new_from_میانگیر(بنية xkb_context *ctx,
                           ثابت حرف *میانگیر, اندازه_t درازی,
                           تعداد xkb_keymap_format format,
                           تعداد xkb_keymap_compile_flags flags)
{
    ثابت بنية xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    إذا (!ops || !ops->keymap_new_from_string) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        أرجع عدم;
    }

    إذا (!میانگیر) {
        log_err_func1(ctx, XKB_LOG_پیام_NO_ID,
                      "no buffer specified\n");
        أرجع عدم;
    }

    بنية xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    إذا (!keymap)
        أرجع عدم;

    /* Allow a zero-terminated string as a buffer */
    إذا (درازی > 0 && میانگیر[درازی - 1] == '\0')
        درازی--;

    إذا (!ops->keymap_new_from_string(keymap, میانگیر, درازی)) {
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
        log_err_func1(ctx, XKB_LOG_پیام_NO_ID,
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
ساكن تعداد xkb_وضعیت
check_keymap_serialize_abi(
    بنية xkb_context * محدود ctx,
    ثابت حرف * محدود func,
    ثابت بنية xkb_keymap_serialize_config * محدود config,
    ثابت بنية xkb_keymap_serialize_نتیجه * محدود نتیجه
)
{
    تعداد xkb_وضعیت وضعیت;
    إذا ((وضعیت = xkb_check_keymap_abi(config)) ||
        (وضعیت = xkb_check_keymap_abi(نتیجه))) {
        xkb_log_abi_error(ctx, func, وضعیت);
    }
    أرجع وضعیت;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

تعداد xkb_وضعیت
xkb_keymap_serialize(ثابت بنية xkb_keymap *keymap,
                     ثابت بنية xkb_keymap_serialize_config *config,
                     بنية xkb_keymap_serialize_نتیجه *نتیجه)
{
    /* Check ABI compatibility */
    ثابت تعداد xkb_وضعیت وضعیت =
        check_keymap_serialize_abi(keymap->ctx, __func__, config, نتیجه);
    إذا (وضعیت != XKB_SUCCESS)
        أرجع وضعیت;

    بنية xkb_keymap_serialize_config new_config = *config;

    ثابت تعداد xkb_keymap_serialize_flags invalid_flags = (
        new_config.flags &
        ~(تعداد xkb_keymap_serialize_flags)XKB_KEYMAP_SERIALIZE_FLAGS_مقادیر
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

    ثابت xkb_layout_ماسک_t all_layouts =
        (xkb_layout_ماسک_t)((UINT64_C(1) << keymap->رقم_groups) - 1);
    إذا (!new_config.layouts) {
        new_config.layouts = all_layouts;
    }

    إذا (new_config.layouts & ~all_layouts) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                     "unsupported layout mask: "
                     "expected subset of 0x%08"PRIx32", got: 0x%08"PRIx32"\n",
                     all_layouts, new_config.layouts);
        أرجع XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص;
    }

    ثابت xkb_layout_شاخص_t max_groups = format_max_groups(config->format);
    ثابت xkb_layout_شاخص_t رقم_groups =
        (xkb_layout_شاخص_t)popcount32(new_config.layouts);

    إذا (رقم_groups > max_groups) {
        ثابت bool strict =
            (new_config.flags & XKB_KEYMAP_SERIALIZE_STRICT_نما);
        xkb_log_with_کد(
            keymap->ctx,
            (strict ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
            XKB_LOG_VERBOSITY_کمینه,
            XKB_ERROR_LAYOUT_شمار_حد_EXCEEDED_2,
            "Cannot serialize %"PRIu32" groups in keymap format %d: "
            "maximum is %"PRIu32"%s\n",
            رقم_groups, config->format, max_groups,
            (strict ? "" : "; discarding unsupported groups")
        );
        إذا (strict) {
            أرجع XKB_ERROR_LAYOUT_شمار_حد_EXCEEDED;
        } وإلا {
            new_config.layouts =
                keep_پایینترین_n_مجموعه_بیتها(new_config.layouts, max_groups);
        }
    }

    log_dbg(keymap->ctx, XKB_LOG_پیام_NO_ID,
            "Serializing group mask 0x%"PRIx32" (total: %"PRIu32")\n",
            new_config.layouts, رقم_groups);

    إذا (new_config.layouts != all_layouts) {
        /*
         * If some layouts are discarded, then interprets may not trigger
         * properly. Avoid this by requiring explicit values.
         */
        new_config.flags |= (XKB_KEYMAP_SERIALIZE_EXPLICIT_کلید_مقادیر |
                             XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS);
    }

    أرجع ops->keymap_serialize(keymap, &new_config, نتیجه);
}

حرف *
xkb_keymap_get_as_string2(بنية xkb_keymap *keymap,
                          تعداد xkb_keymap_format format,
                          تعداد xkb_keymap_serialize_flags flags)
{
    ثابت بنية xkb_keymap_serialize_config config = {
        .اندازه = حجم(config),
        .flags = flags,
        .format = format,
        .layouts = 0,
    };

    بنية xkb_keymap_serialize_نتیجه نتیجه = { .اندازه = حجم(نتیجه) };

    أرجع (xkb_keymap_serialize(keymap, &config, &نتیجه) == XKB_SUCCESS)
        ? نتیجه.serialized
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
xkb_mod_شاخص_t
xkb_keymap_رقم_mods(بنية xkb_keymap *keymap)
{
    أرجع keymap->mods.رقم_mods;
}

/**
 * Return the name for a given modifier.
 */
ثابت حرف *
xkb_keymap_mod_get_name(بنية xkb_keymap *keymap, xkb_mod_شاخص_t idx)
{
    إذا (idx >= keymap->mods.رقم_mods)
        أرجع عدم;

    أرجع xkb_atom_متن(keymap->ctx, keymap->mods.mods[idx].name);
}

/**
 * Returns the index for a named modifier.
 */
xkb_mod_شاخص_t
xkb_keymap_mod_get_شاخص(بنية xkb_keymap *keymap, ثابت حرف *name)
{
    ثابت xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    أرجع (atom == XKB_ATOM_NONE)
        ? XKB_MOD_INVALID
        : Xkb_Mod_Name_مقصد_شاخص(&keymap->mods, atom, MOD_BOTH);
}

/**
 * Return the canonical mapping of a named modifier.
 */
xkb_mod_ماسک_t
xkb_keymap_mod_get_ماسک(بنية xkb_keymap *keymap, ثابت حرف* name)
{
    ثابت xkb_mod_شاخص_t idx = xkb_keymap_mod_get_شاخص(keymap, name);
    أرجع (idx >= keymap->mods.رقم_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the canonical mapping of a given modifier.
 */
xkb_mod_ماسک_t
xkb_keymap_mod_get_mask2(بنية xkb_keymap *keymap, xkb_mod_شاخص_t idx)
{
    أرجع (idx >= keymap->mods.رقم_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the total number of active groups in the keymap.
 */
xkb_layout_شاخص_t
xkb_keymap_رقم_layouts(بنية xkb_keymap *keymap)
{
    أرجع keymap->رقم_groups;
}

/**
 * Returns the name for a given group.
 */
ثابت حرف *
xkb_keymap_layout_get_name(بنية xkb_keymap *keymap, xkb_layout_شاخص_t idx)
{
    إذا (idx >= keymap->رقم_گروه_نامها)
        أرجع عدم;

    أرجع xkb_atom_متن(keymap->ctx, keymap->گروه_نامها[idx]);
}

/**
 * Returns the index for a named layout.
 */
xkb_layout_شاخص_t
xkb_keymap_layout_get_شاخص(بنية xkb_keymap *keymap, ثابت حرف *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_layout_شاخص_t i;

    إذا (atom == XKB_ATOM_NONE)
        أرجع XKB_LAYOUT_INVALID;

    لكل (i = 0; i < keymap->رقم_گروه_نامها; i++)
        إذا (keymap->گروه_نامها[i] == atom)
            أرجع i;

    أرجع XKB_LAYOUT_INVALID;
}

/**
 * Returns the number of layouts active for a particular key.
 */
xkb_layout_شاخص_t
xkb_keymap_رقم_layouts_for_کلید(بنية xkb_keymap *keymap, xkb_keycode_t kc)
{
    ثابت بنية xkb_کلید *کلید = Xkb_کلید(keymap, kc);

    إذا (!کلید)
        أرجع 0;

    أرجع کلید->رقم_groups;
}

/**
 * Returns the number of levels active for a particular key and layout.
 */
xkb_level_شاخص_t
xkb_keymap_رقم_سطوح_for_کلید(بنية xkb_keymap *keymap, xkb_keycode_t kc,
                              xkb_layout_شاخص_t layout)
{
    ثابت بنية xkb_کلید *کلید = Xkb_کلید(keymap, kc);

    إذا (!کلید)
        أرجع 0;

    layout = xkb_keymap_کلید_effective_layout(کلید, layout);
    إذا (layout == XKB_LAYOUT_INVALID)
        أرجع 0;

    أرجع Xkb_کلید_رقم_سطوح(کلید, layout);
}

/**
 * Return the total number of LEDs in the keymap.
 */
xkb_led_شاخص_t
xkb_keymap_رقم_leds(بنية xkb_keymap *keymap)
{
    أرجع keymap->رقم_leds;
}

/**
 * Returns the name for a given LED.
 */
ثابت حرف *
xkb_keymap_led_get_name(بنية xkb_keymap *keymap, xkb_led_شاخص_t idx)
{
    إذا (idx >= keymap->رقم_leds)
        أرجع عدم;

    أرجع xkb_atom_متن(keymap->ctx, keymap->leds[idx].name);
}

/**
 * Returns the index for a named LED.
 */
xkb_led_شاخص_t
xkb_keymap_led_get_شاخص(بنية xkb_keymap *keymap, ثابت حرف *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_led_شاخص_t i;
    ثابت بنية xkb_led *led;

    إذا (atom == XKB_ATOM_NONE)
        أرجع XKB_LED_INVALID;

    xkb_leds_enumerate(i, led, keymap)
        إذا (led->name == atom)
            أرجع i;

    أرجع XKB_LED_INVALID;
}

اندازه_t
xkb_keymap_کلید_get_mods_for_level(بنية xkb_keymap *keymap,
                                  xkb_keycode_t kc,
                                  xkb_layout_شاخص_t layout,
                                  xkb_level_شاخص_t level,
                                  xkb_mod_ماسک_t *masks_بیرون,
                                  اندازه_t masks_اندازه)
{
    ثابت بنية xkb_کلید *کلید = Xkb_کلید(keymap, kc);
    إذا (!کلید)
        أرجع 0;

    layout = xkb_keymap_کلید_effective_layout(کلید, layout);
    إذا (layout == XKB_LAYOUT_INVALID)
        أرجع 0;

    إذا (level >= Xkb_کلید_رقم_سطوح(کلید, layout))
        أرجع 0;

    ثابت بنية xkb_کلید_type *type = کلید->groups[layout].type;

    اندازه_t شمار = 0;

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
        bool خالی_mapped = false;
        لكل (darray_اندازه_t i = 0; i < type->رقم_entries && شمار < masks_اندازه; i++)
            إذا (مدخل_is_فعال(&type->entries[i]) &&
                type->entries[i].mods.ماسک == 0) {
                خالی_mapped = true;
                اكسر;
            }
        إذا (!خالی_mapped && شمار < masks_اندازه) {
            masks_بیرون[شمار++] = 0;
        }
    }

    /* Now search explicit mappings. */
    لكل (darray_اندازه_t i = 0; i < type->رقم_entries && شمار < masks_اندازه; i++) {
        إذا (مدخل_is_فعال(&type->entries[i]) &&
            type->entries[i].level == level) {
            masks_بیرون[شمار++] = type->entries[i].mods.ماسک;
        }
    }

    أرجع شمار;
}

بنية xkb_level *
xkb_keymap_کلید_get_level(بنية xkb_keymap *keymap, ثابت بنية xkb_کلید *کلید,
                         xkb_layout_شاخص_t layout, xkb_level_شاخص_t level)
{
    layout = xkb_keymap_کلید_effective_layout(کلید, layout);
    إذا (layout == XKB_LAYOUT_INVALID)
        أرجع عدم;

    إذا (level >= Xkb_کلید_رقم_سطوح(کلید, layout))
        أرجع عدم;

    أرجع &کلید->groups[layout].سطوح[level];
}

/**
 * As below, but takes an explicit layout/level rather than state.
 */
صحيح
xkb_keymap_کلید_get_syms_by_level(بنية xkb_keymap *keymap,
                                 xkb_keycode_t kc,
                                 xkb_layout_شاخص_t layout,
                                 xkb_level_شاخص_t level,
                                 ثابت xkb_keysym_t **syms_بیرون)
{
    ثابت بنية xkb_کلید *کلید = Xkb_کلید(keymap, kc);

    إذا (!کلید)
        اذهب err;

    ثابت بنية xkb_level* ثابت leveli =
        xkb_keymap_کلید_get_level(keymap, کلید, layout, level);

    إذا (!leveli)
        اذهب err;

    ثابت xkb_keysym_شمار_t رقم_syms = leveli->رقم_syms;
    إذا (رقم_syms == 0)
        اذهب err;

    إذا (رقم_syms == 1)
        *syms_بیرون = &leveli->s.sym;
    وإلا
        *syms_بیرون = leveli->s.syms;

    أرجع (صحيح) رقم_syms;

err:
    *syms_بیرون = عدم;
    أرجع 0;
}

xkb_keycode_t
xkb_keymap_min_keycode(بنية xkb_keymap *keymap)
{
    أرجع keymap->min_کلید_کد;
}

xkb_keycode_t
xkb_keymap_max_keycode(بنية xkb_keymap *keymap)
{
    أرجع keymap->max_کلید_کد;
}

ساكن تعداد xkb_وضعیت
keymap_کلید_iterator_config_check(
    بنية xkb_context * محدود ctx,
    ثابت حرف *func,
    ثابت بنية xkb_keymap_کلید_iterator_config * محدود config
)
{
    /* Check ABI compatibility */
    ثابت تعداد xkb_وضعیت abi_error = xkb_check_keymap_abi(config);
    إذا (abi_error) {
        xkb_log_abi_error(ctx, func, abi_error);
        أرجع abi_error;
    }

    /* Sanitize input: flags */
    ثابت uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_KEYMAP_کلید_ITERATOR_FLAGS_مقادیر);
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

static_assert(XKB_KEYMAP_کلید_ITERATOR_FLAGS_مقادیر <=
              (صحيح)XKB_اشاره_گر_برچسب_ماسک,
              "Cannot tag cursor pointer with flags");

#تعریف_کن iter_init_keymap(i, ptr, flags) {          \
    (i)->opaque0 = xkb_اشاره_گر_برچسب((ptr), (flags));\
}
#تعریف_کن iter_get_keymap(i) (                                  \
    (ثابت بنية xkb_keymap *)xkb_اشاره_گر_untag((i)->opaque0)\
)
#تعریف_کن iter_get_بعدی(i) ((ثابت بنية xkb_کلید *)(i)->opaque1)
#تعریف_کن iter_مجموعه_بعدی(i, ptr) {(i)->opaque1 = (ثابت فراغ *)(ptr);}
#تعریف_کن iter_get_descending_ترتیب(i) (                  \
    xkb_اشاره_گر_get_برچسب((i)->opaque0) &                 \
    (uintptr_t)XKB_KEYMAP_کلید_ITERATOR_DESCENDING_ترتیب \
)
#تعریف_کن iter_get_include_unbound(i) (                  \
    xkb_اشاره_گر_get_برچسب((i)->opaque0) &                \
    (uintptr_t)XKB_KEYMAP_کلید_ITERATOR_INCLUDE_UNBOUND \
)

تعداد xkb_وضعیت
xkb_keymap_کلید_iterator_init(
    بنية xkb_keymap_کلید_iterator * محدود iter,
    ثابت بنية xkb_keymap * محدود keymap,
    ثابت بنية xkb_keymap_کلید_iterator_config * محدود config
)
{
    /* Handle default configuration */
    ساكن ثابت بنية xkb_keymap_کلید_iterator_config default_config = {
        .اندازه = حجم(default_config)
    };
    إذا (!config)
        config = &default_config;

    /* Check input */
    ثابت تعداد xkb_وضعیت وضعیت =
        keymap_کلید_iterator_config_check(keymap->ctx, __func__, config);
    إذا (وضعیت != XKB_SUCCESS) {
        أرجع وضعیت;
    }

    /*
     * Initialization
     */

    memset(iter, 0, حجم(*iter));

    /* Borrow keymap */
    iter_init_keymap(iter, keymap, config->flags);

    إذا (keymap->رقم_کلیدها == 0) {
        iter_مجموعه_بعدی(iter, عدم);
        أرجع XKB_SUCCESS;
    }

    /* Borrow key */
    ثابت بنية xkb_کلید *بعدی;
    إذا (config->flags & XKB_KEYMAP_کلید_ITERATOR_DESCENDING_ترتیب) {
        إذا (!config->شروع || config->شروع > keymap->max_کلید_کد) {
            بعدی = &keymap->کلیدها[keymap->رقم_کلیدها - 1];
        } وإلا {
            /*
             * May still fail *safely* if start < keymap->min_key_code:
             * there will be simply no key to iterate over.
             */
            بعدی = xkb_keymap_get_بعدی_defined_کلید(keymap, false, config->شروع);
        }
    } وإلا {
        إذا (!config->شروع || config->شروع < keymap->min_کلید_کد) {
            بعدی = (keymap->رقم_کلیدها_کم)
                ? &keymap->کلیدها[keymap->min_کلید_کد]
                : &keymap->کلیدها[0];
        } وإلا {
            /*
             * May still fail *safely* if start > keymap->max_key_code:
             * there will be simply no key to iterate over.
             */
            بعدی = xkb_keymap_get_بعدی_defined_کلید(keymap, true, config->شروع);
        }
    };
    iter_مجموعه_بعدی(iter, بعدی);

    أرجع XKB_SUCCESS;
}

xkb_keycode_t
xkb_keymap_کلید_iterator_بعدی(بنية xkb_keymap_کلید_iterator *iter)
{
    ثابت بنية xkb_کلید *بعدی = iter_get_بعدی(iter);
    إذا (!بعدی)
        أرجع XKB_KEYCODE_INVALID;

    ثابت بنية xkb_keymap *keymap = iter_get_keymap(iter);
    أكد(keymap->رقم_کلیدها);

    ثابت bool descending = iter_get_descending_ترتیب(iter);
    ثابت bool skip_unbound = !iter_get_include_unbound(iter);
    ثابت بنية xkb_کلید *آخرین = descending
        ? (keymap->رقم_کلیدها_کم)
            ? &keymap->کلیدها[keymap->min_کلید_کد]
            : &keymap->کلیدها[0]
        : &keymap->کلیدها[keymap->رقم_کلیدها - 1];
    ثابت ptrdiff_t گام = descending ? -1 : 1;

    /* Skip undefined keys (no name) and optionally unbound keys */
    لكل (;;) {
        ثابت bool iter_انتها = descending ? (بعدی <= آخرین) : (بعدی >= آخرین);
        ثابت bool skip = (
            بعدی->name == XKB_ATOM_NONE ||
            (skip_unbound && (بعدی->رقم_groups == 0))
        );
        إذا (!skip) {
            ثابت xkb_keycode_t ret = بعدی->keycode;
            iter_مجموعه_بعدی(iter, iter_انتها ? عدم : بعدی + گام);
            أرجع ret;
        }
        إذا (iter_انتها) {
            iter_مجموعه_بعدی(iter, عدم);
            أرجع XKB_KEYCODE_INVALID;
        }
        بعدی += گام;
    }
}

#لغو_تعریف iter_init_keymap
#لغو_تعریف iter_get_keymap
#لغو_تعریف iter_مجموعه_بعدی
#لغو_تعریف iter_get_بعدی
#لغو_تعریف iter_get_descending_ترتیب
#لغو_تعریف iter_get_include_unbound

فراغ
xkb_keymap_کلید_for_each(بنية xkb_keymap *keymap, xkb_keymap_کلید_iter_t iter,
                        فراغ *داده)
{
    بنية xkb_کلید *کلید;

    xkb_کلیدها_foreach(کلید, keymap)
        iter(keymap, کلید->keycode, داده);
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
        لكل (darray_اندازه_t i = 0; i < keymap->رقم_کلید_aliases; i++)
            إذا (keymap->کلید_aliases[i].alias == atom)
                atom = keymap->کلید_aliases[i].real;
    }
    إذا (!atom)
        أرجع XKB_KEYCODE_INVALID;

    xkb_کلیدها_foreach(کلید, keymap) {
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
