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

struct xkb_keymap *
xkb_keymap_ref(struct xkb_keymap *keymap)
{
    assert(keymap->refcnt > 0);
    keymap->refcnt++;
    trả_về keymap;
}

void
clear_level(struct xkb_level *leveli)
{
    nếu (leveli->num_syms > 1)
        free(leveli->s.syms);
    nếu (leveli->num_actions > 1)
        free(leveli->a.actions);
}

static void
clear_interpret(struct xkb_sym_interpret *interp)
{
    nếu (interp->num_actions > 1)
        free(interp->a.actions);
}

void
xkb_keymap_unref(struct xkb_keymap *keymap)
{
    assert(!keymap || keymap->refcnt > 0);
    nếu (!keymap || --keymap->refcnt > 0)
        trả_về;

    nếu (keymap->keys) {
        struct xkb_key *key;
        xkb_keys_foreach(key, keymap) {
            nếu (key->groups) {
                cho (xkb_layout_index_t i = 0; i < key->num_groups; i++) {
                    nếu (key->groups[i].levels) {
                        cho (xkb_level_index_t j = 0;
                             j < XkbKeyNumLevels(key, i);
                             j++)
                            clear_level(&key->groups[i].levels[j]);
                        free(key->groups[i].levels);
                    }
                }
                free(key->groups);
            }
            nếu (!key->overlays_inline && key->overlays_keys) {
                free((void *)key->overlays_keys);
            }
        }
        free(keymap->keys);
    }
    nếu (keymap->types) {
        cho (darray_size_t i = 0; i < keymap->num_types; i++) {
            free(keymap->types[i].entries);
            free(keymap->types[i].level_names);
        }
        free(keymap->types);
    }
    cho (darray_size_t k = 0; k < keymap->num_sym_interprets; k++) {
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

static const struct xkb_keymap_format_ops *
get_keymap_format_ops(enum xkb_keymap_format format)
{
    static const struct xkb_keymap_format_ops *keymap_format_ops[] = {
        [XKB_KEYMAP_FORMAT_TEXT_V1] = &text_v1_keymap_format_ops,
        [XKB_KEYMAP_FORMAT_TEXT_V2] = &text_v1_keymap_format_ops,
    };

    nếu ((số_nguyên) format < 0 || (số_nguyên) format >= (số_nguyên) ARRAY_SIZE(keymap_format_ops))
        trả_về NULL;

    trả_về keymap_format_ops[(số_nguyên) format];
}

struct xkb_keymap *
xkb_keymap_new_from_rmlvo(const struct xkb_rmlvo_builder *rmlvo,
                          enum xkb_keymap_format format,
                          enum xkb_keymap_compile_flags flags)
{
    const struct xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    nếu (!ops || !ops->keymap_new_from_rmlvo) {
        log_err_func(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        trả_về NULL;
    }

    struct xkb_keymap *keymap = xkb_keymap_new(rmlvo->ctx, __func__, format,
                                               flags);
    nếu (!keymap)
        trả_về NULL;

    nếu (!ops->keymap_new_from_rmlvo(keymap, rmlvo)) {
        xkb_keymap_unref(keymap);
        trả_về NULL;
    }

    trả_về keymap;
}

struct xkb_keymap *
xkb_keymap_new_from_names2(struct xkb_context *ctx,
                           const struct xkb_rule_names *rmlvo_in,
                           enum xkb_keymap_format format,
                           enum xkb_keymap_compile_flags flags)
{
    const struct xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    nếu (!ops || !ops->keymap_new_from_names) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        trả_về NULL;
    }

    struct xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    nếu (!keymap)
        trả_về NULL;

    struct xkb_rule_names rmlvo = {0};
    nếu (rmlvo_in)
        rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    nếu (!ops->keymap_new_from_names(keymap, &rmlvo)) {
        xkb_keymap_unref(keymap);
        trả_về NULL;
    }

    trả_về keymap;
}


struct xkb_keymap *
xkb_keymap_new_from_names(struct xkb_context *ctx,
                          const struct xkb_rule_names *rmlvo_in,
                          enum xkb_keymap_compile_flags flags)
{
    trả_về xkb_keymap_new_from_names2(ctx, rmlvo_in,
                                      XKB_KEYMAP_FORMAT_TEXT_V2, flags);
}

struct xkb_keymap *
xkb_keymap_new_from_string(struct xkb_context *ctx,
                           const char *string,
                           enum xkb_keymap_format format,
                           enum xkb_keymap_compile_flags flags)
{
    trả_về xkb_keymap_new_from_buffer(ctx, string, strlen(string),
                                      format, flags);
}

struct xkb_keymap *
xkb_keymap_new_from_buffer(struct xkb_context *ctx,
                           const char *buffer, size_t length,
                           enum xkb_keymap_format format,
                           enum xkb_keymap_compile_flags flags)
{
    const struct xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    nếu (!ops || !ops->keymap_new_from_string) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        trả_về NULL;
    }

    nếu (!buffer) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no buffer specified\n");
        trả_về NULL;
    }

    struct xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    nếu (!keymap)
        trả_về NULL;

    /* Allow a zero-terminated string as a buffer */
    nếu (length > 0 && buffer[length - 1] == '\0')
        length--;

    nếu (!ops->keymap_new_from_string(keymap, buffer, length)) {
        xkb_keymap_unref(keymap);
        trả_về NULL;
    }

    trả_về keymap;
}

struct xkb_keymap *
xkb_keymap_new_from_file(struct xkb_context *ctx,
                         FILE *file,
                         enum xkb_keymap_format format,
                         enum xkb_keymap_compile_flags flags)
{
    const struct xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    nếu (!ops || !ops->keymap_new_from_file) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        trả_về NULL;
    }

    nếu (!file) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no file specified\n");
        trả_về NULL;
    }

    struct xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    nếu (!keymap)
        trả_về NULL;

    nếu (!ops->keymap_new_from_file(keymap, file)) {
        xkb_keymap_unref(keymap);
        trả_về NULL;
    }

    trả_về keymap;
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
static enum xkb_status
check_keymap_serialize_abi(
    struct xkb_context * restrict ctx,
    const char * restrict func,
    const struct xkb_keymap_serialize_config * restrict config,
    const struct xkb_keymap_serialize_result * restrict result
)
{
    enum xkb_status status;
    nếu ((status = xkb_check_keymap_abi(config)) ||
        (status = xkb_check_keymap_abi(result))) {
        xkb_log_abi_error(ctx, func, status);
    }
    trả_về status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

enum xkb_status
xkb_keymap_serialize(const struct xkb_keymap *keymap,
                     const struct xkb_keymap_serialize_config *config,
                     struct xkb_keymap_serialize_result *result)
{
    /* Check ABI compatibility */
    const enum xkb_status status =
        check_keymap_serialize_abi(keymap->ctx, __func__, config, result);
    nếu (status != XKB_SUCCESS)
        trả_về status;

    struct xkb_keymap_serialize_config new_config = *config;

    const enum xkb_keymap_serialize_flags invalid_flags = (
        new_config.flags &
        ~(enum xkb_keymap_serialize_flags)XKB_KEYMAP_SERIALIZE_FLAGS_VALUES
    );
    nếu (invalid_flags) {
        log_err_func(keymap->ctx,
                     XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS_,
                     "unrecognized serialization flags: %#x\n",
                     invalid_flags);
        trả_về XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS;
    }

    nếu (new_config.format == XKB_KEYMAP_USE_ORIGINAL_FORMAT)
        new_config.format = keymap->format;

    const struct xkb_keymap_format_ops * const ops =
        get_keymap_format_ops(new_config.format);
    nếu (!ops || !ops->keymap_serialize) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", new_config.format);
        trả_về XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT;
    }

    const xkb_layout_mask_t all_layouts =
        (xkb_layout_mask_t)((UINT64_C(1) << keymap->num_groups) - 1);
    nếu (!new_config.layouts) {
        new_config.layouts = all_layouts;
    }

    nếu (new_config.layouts & ~all_layouts) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                     "unsupported layout mask: "
                     "expected subset of 0x%08"PRIx32", got: 0x%08"PRIx32"\n",
                     all_layouts, new_config.layouts);
        trả_về XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    const xkb_layout_index_t max_groups = format_max_groups(config->format);
    const xkb_layout_index_t num_groups =
        (xkb_layout_index_t)popcount32(new_config.layouts);

    nếu (num_groups > max_groups) {
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
        nếu (strict) {
            trả_về XKB_ERROR_LAYOUT_COUNT_LIMIT_EXCEEDED;
        } không_thì {
            new_config.layouts =
                keep_lowest_n_set_bits(new_config.layouts, max_groups);
        }
    }

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Serializing group mask 0x%"PRIx32" (total: %"PRIu32")\n",
            new_config.layouts, num_groups);

    nếu (new_config.layouts != all_layouts) {
        /*
         * If some layouts are discarded, then interprets may not trigger
         * properly. Avoid this by requiring explicit values.
         */
        new_config.flags |= (XKB_KEYMAP_SERIALIZE_EXPLICIT_KEY_VALUES |
                             XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS);
    }

    trả_về ops->keymap_serialize(keymap, &new_config, result);
}

char *
xkb_keymap_get_as_string2(struct xkb_keymap *keymap,
                          enum xkb_keymap_format format,
                          enum xkb_keymap_serialize_flags flags)
{
    const struct xkb_keymap_serialize_config config = {
        .size = sizeof(config),
        .flags = flags,
        .format = format,
        .layouts = 0,
    };

    struct xkb_keymap_serialize_result result = { .size = sizeof(result) };

    trả_về (xkb_keymap_serialize(keymap, &config, &result) == XKB_SUCCESS)
        ? result.serialized
        : NULL;
}

char *
xkb_keymap_get_as_string(struct xkb_keymap *keymap,
                         enum xkb_keymap_format format)
{
    trả_về xkb_keymap_get_as_string2(keymap, format,
                                     XKB_KEYMAP_SERIALIZE_NO_FLAGS);
}

/**
 * Returns the total number of modifiers active in the keymap.
 */
xkb_mod_index_t
xkb_keymap_num_mods(struct xkb_keymap *keymap)
{
    trả_về keymap->mods.num_mods;
}

/**
 * Return the name for a given modifier.
 */
const char *
xkb_keymap_mod_get_name(struct xkb_keymap *keymap, xkb_mod_index_t idx)
{
    nếu (idx >= keymap->mods.num_mods)
        trả_về NULL;

    trả_về xkb_atom_text(keymap->ctx, keymap->mods.mods[idx].name);
}

/**
 * Returns the index for a named modifier.
 */
xkb_mod_index_t
xkb_keymap_mod_get_index(struct xkb_keymap *keymap, const char *name)
{
    const xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    trả_về (atom == XKB_ATOM_NONE)
        ? XKB_MOD_INVALID
        : XkbModNameToIndex(&keymap->mods, atom, MOD_BOTH);
}

/**
 * Return the canonical mapping of a named modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask(struct xkb_keymap *keymap, const char* name)
{
    const xkb_mod_index_t idx = xkb_keymap_mod_get_index(keymap, name);
    trả_về (idx >= keymap->mods.num_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the canonical mapping of a given modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask2(struct xkb_keymap *keymap, xkb_mod_index_t idx)
{
    trả_về (idx >= keymap->mods.num_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the total number of active groups in the keymap.
 */
xkb_layout_index_t
xkb_keymap_num_layouts(struct xkb_keymap *keymap)
{
    trả_về keymap->num_groups;
}

/**
 * Returns the name for a given group.
 */
const char *
xkb_keymap_layout_get_name(struct xkb_keymap *keymap, xkb_layout_index_t idx)
{
    nếu (idx >= keymap->num_group_names)
        trả_về NULL;

    trả_về xkb_atom_text(keymap->ctx, keymap->group_names[idx]);
}

/**
 * Returns the index for a named layout.
 */
xkb_layout_index_t
xkb_keymap_layout_get_index(struct xkb_keymap *keymap, const char *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_layout_index_t i;

    nếu (atom == XKB_ATOM_NONE)
        trả_về XKB_LAYOUT_INVALID;

    cho (i = 0; i < keymap->num_group_names; i++)
        nếu (keymap->group_names[i] == atom)
            trả_về i;

    trả_về XKB_LAYOUT_INVALID;
}

/**
 * Returns the number of layouts active for a particular key.
 */
xkb_layout_index_t
xkb_keymap_num_layouts_for_key(struct xkb_keymap *keymap, xkb_keycode_t kc)
{
    const struct xkb_key *key = XkbKey(keymap, kc);

    nếu (!key)
        trả_về 0;

    trả_về key->num_groups;
}

/**
 * Returns the number of levels active for a particular key and layout.
 */
xkb_level_index_t
xkb_keymap_num_levels_for_key(struct xkb_keymap *keymap, xkb_keycode_t kc,
                              xkb_layout_index_t layout)
{
    const struct xkb_key *key = XkbKey(keymap, kc);

    nếu (!key)
        trả_về 0;

    layout = xkb_keymap_key_effective_layout(key, layout);
    nếu (layout == XKB_LAYOUT_INVALID)
        trả_về 0;

    trả_về XkbKeyNumLevels(key, layout);
}

/**
 * Return the total number of LEDs in the keymap.
 */
xkb_led_index_t
xkb_keymap_num_leds(struct xkb_keymap *keymap)
{
    trả_về keymap->num_leds;
}

/**
 * Returns the name for a given LED.
 */
const char *
xkb_keymap_led_get_name(struct xkb_keymap *keymap, xkb_led_index_t idx)
{
    nếu (idx >= keymap->num_leds)
        trả_về NULL;

    trả_về xkb_atom_text(keymap->ctx, keymap->leds[idx].name);
}

/**
 * Returns the index for a named LED.
 */
xkb_led_index_t
xkb_keymap_led_get_index(struct xkb_keymap *keymap, const char *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_led_index_t i;
    const struct xkb_led *led;

    nếu (atom == XKB_ATOM_NONE)
        trả_về XKB_LED_INVALID;

    xkb_leds_enumerate(i, led, keymap)
        nếu (led->name == atom)
            trả_về i;

    trả_về XKB_LED_INVALID;
}

size_t
xkb_keymap_key_get_mods_for_level(struct xkb_keymap *keymap,
                                  xkb_keycode_t kc,
                                  xkb_layout_index_t layout,
                                  xkb_level_index_t level,
                                  xkb_mod_mask_t *masks_out,
                                  size_t masks_size)
{
    const struct xkb_key *key = XkbKey(keymap, kc);
    nếu (!key)
        trả_về 0;

    layout = xkb_keymap_key_effective_layout(key, layout);
    nếu (layout == XKB_LAYOUT_INVALID)
        trả_về 0;

    nếu (level >= XkbKeyNumLevels(key, layout))
        trả_về 0;

    const struct xkb_key_type *type = key->groups[layout].type;

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
    nếu (level == 0) {
        bool empty_mapped = false;
        cho (darray_size_t i = 0; i < type->num_entries && count < masks_size; i++)
            nếu (entry_is_active(&type->entries[i]) &&
                type->entries[i].mods.mask == 0) {
                empty_mapped = true;
                break;
            }
        nếu (!empty_mapped && count < masks_size) {
            masks_out[count++] = 0;
        }
    }

    /* Now search explicit mappings. */
    cho (darray_size_t i = 0; i < type->num_entries && count < masks_size; i++) {
        nếu (entry_is_active(&type->entries[i]) &&
            type->entries[i].level == level) {
            masks_out[count++] = type->entries[i].mods.mask;
        }
    }

    trả_về count;
}

struct xkb_level *
xkb_keymap_key_get_level(struct xkb_keymap *keymap, const struct xkb_key *key,
                         xkb_layout_index_t layout, xkb_level_index_t level)
{
    layout = xkb_keymap_key_effective_layout(key, layout);
    nếu (layout == XKB_LAYOUT_INVALID)
        trả_về NULL;

    nếu (level >= XkbKeyNumLevels(key, layout))
        trả_về NULL;

    trả_về &key->groups[layout].levels[level];
}

/**
 * As below, but takes an explicit layout/level rather than state.
 */
số_nguyên
xkb_keymap_key_get_syms_by_level(struct xkb_keymap *keymap,
                                 xkb_keycode_t kc,
                                 xkb_layout_index_t layout,
                                 xkb_level_index_t level,
                                 const xkb_keysym_t **syms_out)
{
    const struct xkb_key *key = XkbKey(keymap, kc);

    nếu (!key)
        goto err;

    const struct xkb_level* const leveli =
        xkb_keymap_key_get_level(keymap, key, layout, level);

    nếu (!leveli)
        goto err;

    const xkb_keysym_count_t num_syms = leveli->num_syms;
    nếu (num_syms == 0)
        goto err;

    nếu (num_syms == 1)
        *syms_out = &leveli->s.sym;
    không_thì
        *syms_out = leveli->s.syms;

    trả_về (số_nguyên) num_syms;

err:
    *syms_out = NULL;
    trả_về 0;
}

xkb_keycode_t
xkb_keymap_min_keycode(struct xkb_keymap *keymap)
{
    trả_về keymap->min_key_code;
}

xkb_keycode_t
xkb_keymap_max_keycode(struct xkb_keymap *keymap)
{
    trả_về keymap->max_key_code;
}

static enum xkb_status
keymap_key_iterator_config_check(
    struct xkb_context * restrict ctx,
    const char *func,
    const struct xkb_keymap_key_iterator_config * restrict config
)
{
    /* Check ABI compatibility */
    const enum xkb_status abi_error = xkb_check_keymap_abi(config);
    nếu (abi_error) {
        xkb_log_abi_error(ctx, func, abi_error);
        trả_về abi_error;
    }

    /* Sanitize input: flags */
    const uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_KEYMAP_KEY_ITERATOR_FLAGS_VALUES);
    nếu (invalid_flags) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_KEY_ITERATOR_FLAGS_,
                "%s: Unsupported keymap iterator flags: 0x%"PRIx32"\n",
                func, invalid_flags);
        trả_về XKB_ERROR_UNSUPPORTED_KEY_ITERATOR_FLAGS;
    }

    /* Sanitize input: start */
    nếu (config->start > XKB_KEYCODE_MAX)
        trả_về XKB_ERROR_INVALID_KEYCODE;

    trả_về XKB_SUCCESS;
}

static_assert(XKB_KEYMAP_KEY_ITERATOR_FLAGS_VALUES <=
              (số_nguyên)XKB_POINTER_TAG_MASK,
              "Cannot tag cursor pointer with flags");

#define iter_init_keymap(i, ptr, flags) {          \
    (i)->opaque0 = xkb_pointer_tag((ptr), (flags));\
}
#define iter_get_keymap(i) (                                  \
    (const struct xkb_keymap *)xkb_pointer_untag((i)->opaque0)\
)
#define iter_get_next(i) ((const struct xkb_key *)(i)->opaque1)
#define iter_set_next(i, ptr) {(i)->opaque1 = (const void *)(ptr);}
#define iter_get_descending_order(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                 \
    (uintptr_t)XKB_KEYMAP_KEY_ITERATOR_DESCENDING_ORDER \
)
#define iter_get_include_unbound(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                \
    (uintptr_t)XKB_KEYMAP_KEY_ITERATOR_INCLUDE_UNBOUND \
)

enum xkb_status
xkb_keymap_key_iterator_init(
    struct xkb_keymap_key_iterator * restrict iter,
    const struct xkb_keymap * restrict keymap,
    const struct xkb_keymap_key_iterator_config * restrict config
)
{
    /* Handle default configuration */
    static const struct xkb_keymap_key_iterator_config default_config = {
        .size = sizeof(default_config)
    };
    nếu (!config)
        config = &default_config;

    /* Check input */
    const enum xkb_status status =
        keymap_key_iterator_config_check(keymap->ctx, __func__, config);
    nếu (status != XKB_SUCCESS) {
        trả_về status;
    }

    /*
     * Initialization
     */

    memset(iter, 0, sizeof(*iter));

    /* Borrow keymap */
    iter_init_keymap(iter, keymap, config->flags);

    nếu (keymap->num_keys == 0) {
        iter_set_next(iter, NULL);
        trả_về XKB_SUCCESS;
    }

    /* Borrow key */
    const struct xkb_key *next;
    nếu (config->flags & XKB_KEYMAP_KEY_ITERATOR_DESCENDING_ORDER) {
        nếu (!config->start || config->start > keymap->max_key_code) {
            next = &keymap->keys[keymap->num_keys - 1];
        } không_thì {
            /*
             * May still fail *safely* if start < keymap->min_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_key(keymap, false, config->start);
        }
    } không_thì {
        nếu (!config->start || config->start < keymap->min_key_code) {
            next = (keymap->num_keys_low)
                ? &keymap->keys[keymap->min_key_code]
                : &keymap->keys[0];
        } không_thì {
            /*
             * May still fail *safely* if start > keymap->max_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_key(keymap, true, config->start);
        }
    };
    iter_set_next(iter, next);

    trả_về XKB_SUCCESS;
}

xkb_keycode_t
xkb_keymap_key_iterator_next(struct xkb_keymap_key_iterator *iter)
{
    const struct xkb_key *next = iter_get_next(iter);
    nếu (!next)
        trả_về XKB_KEYCODE_INVALID;

    const struct xkb_keymap *keymap = iter_get_keymap(iter);
    assert(keymap->num_keys);

    const bool descending = iter_get_descending_order(iter);
    const bool skip_unbound = !iter_get_include_unbound(iter);
    const struct xkb_key *last = descending
        ? (keymap->num_keys_low)
            ? &keymap->keys[keymap->min_key_code]
            : &keymap->keys[0]
        : &keymap->keys[keymap->num_keys - 1];
    const ptrdiff_t step = descending ? -1 : 1;

    /* Skip undefined keys (no name) and optionally unbound keys */
    cho (;;) {
        const bool iter_end = descending ? (next <= last) : (next >= last);
        const bool skip = (
            next->name == XKB_ATOM_NONE ||
            (skip_unbound && (next->num_groups == 0))
        );
        nếu (!skip) {
            const xkb_keycode_t ret = next->keycode;
            iter_set_next(iter, iter_end ? NULL : next + step);
            trả_về ret;
        }
        nếu (iter_end) {
            iter_set_next(iter, NULL);
            trả_về XKB_KEYCODE_INVALID;
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
xkb_keymap_key_for_each(struct xkb_keymap *keymap, xkb_keymap_key_iter_t iter,
                        void *data)
{
    struct xkb_key *key;

    xkb_keys_foreach(key, keymap)
        iter(keymap, key->keycode, data);
}

const char *
xkb_keymap_key_get_name(struct xkb_keymap *keymap, xkb_keycode_t kc)
{
    const struct xkb_key *key = XkbKey(keymap, kc);

    nếu (!key)
        trả_về NULL;

    trả_về xkb_atom_text(keymap->ctx, key->name);
}

xkb_keycode_t
xkb_keymap_key_by_name(struct xkb_keymap *keymap, const char *name)
{
    struct xkb_key *key;
    xkb_atom_t atom;

    atom = xkb_atom_lookup(keymap->ctx, name);
    nếu (atom) {
        cho (darray_size_t i = 0; i < keymap->num_key_aliases; i++)
            nếu (keymap->key_aliases[i].alias == atom)
                atom = keymap->key_aliases[i].real;
    }
    nếu (!atom)
        trả_về XKB_KEYCODE_INVALID;

    xkb_keys_foreach(key, keymap) {
        nếu (key->name == atom)
            trả_về key->keycode;
    }

    trả_về XKB_KEYCODE_INVALID;
}

/**
 * Simple boolean specifying whether or not the key should repeat.
 */
số_nguyên
xkb_keymap_key_repeats(struct xkb_keymap *keymap, xkb_keycode_t kc)
{
    const struct xkb_key *key = XkbKey(keymap, kc);

    nếu (!key)
        trả_về 0;

    trả_về key->repeats;
}
