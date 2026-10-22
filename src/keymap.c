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

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "abi-check.h"
#ଅନ୍ତର୍ଭାଵ "atom.h"
#ଅନ୍ତର୍ଭାଵ "context.h"
#ଅନ୍ତର୍ଭାଵ "features/enums.h"
#ଅନ୍ତର୍ଭାଵ "keymap.h"
#ଅନ୍ତର୍ଭାଵ "keymap-priv.h"
#ଅନ୍ତର୍ଭାଵ "messages-codes.h"
#ଅନ୍ତର୍ଭାଵ "text.h"
#ଅନ୍ତର୍ଭାଵ "util-mem.h"
#ଅନ୍ତର୍ଭାଵ "util-numbers.h"

ସଂରଚନା xkb_keymap *
xkb_keymap_ref(ସଂରଚନା xkb_keymap *keymap)
{
    ପ୍ରତିଜ୍ଞା(keymap->refcnt > 0);
    keymap->refcnt++;
    ଫେରନ୍ତୁ keymap;
}

ଶୂନ୍ୟ
clear_level(ସଂରଚନା xkb_level *leveli)
{
    ଯଦି (leveli->ଅଙ୍କ_syms > 1)
        ମୁକ୍ତ(leveli->s.syms);
    ଯଦି (leveli->ଅଙ୍କ_actions > 1)
        ମୁକ୍ତ(leveli->a.actions);
}

ସ୍ଥିର ଶୂନ୍ୟ
clear_interpret(ସଂରଚନା xkb_sym_interpret *interp)
{
    ଯଦି (interp->ଅଙ୍କ_actions > 1)
        ମୁକ୍ତ(interp->a.actions);
}

ଶୂନ୍ୟ
xkb_keymap_unref(ସଂରଚନା xkb_keymap *keymap)
{
    ପ୍ରତିଜ୍ଞା(!keymap || keymap->refcnt > 0);
    ଯଦି (!keymap || --keymap->refcnt > 0)
        ଫେରନ୍ତୁ;

    ଯଦି (keymap->ଚାବିଗୁଡ଼ିକ) {
        ସଂରଚନା xkb_ଚାବି *ଚାବି;
        xkb_ଚାବିଗୁଡ଼ିକ_foreach(ଚାବି, keymap) {
            ଯଦି (ଚାବି->groups) {
                ପାଇଁ (xkb_layout_index_t i = 0; i < ଚାବି->ଅଙ୍କ_groups; i++) {
                    ଯଦି (ଚାବି->groups[i].levels) {
                        ପାଇଁ (xkb_level_index_t j = 0;
                             j < Xkb_ଚାବି_ଅଙ୍କ_Levels(ଚାବି, i);
                             j++)
                            clear_level(&ଚାବି->groups[i].levels[j]);
                        ମୁକ୍ତ(ଚାବି->groups[i].levels);
                    }
                }
                ମୁକ୍ତ(ଚାବି->groups);
            }
            ଯଦି (!ଚାବି->overlays_inline && ଚାବି->overlays_ଚାବିଗୁଡ଼ିକ) {
                ମୁକ୍ତ((ଶୂନ୍ୟ *)ଚାବି->overlays_ଚାବିଗୁଡ଼ିକ);
            }
        }
        ମୁକ୍ତ(keymap->ଚାବିଗୁଡ଼ିକ);
    }
    ଯଦି (keymap->types) {
        ପାଇଁ (darray_size_t i = 0; i < keymap->ଅଙ୍କ_types; i++) {
            ମୁକ୍ତ(keymap->types[i].entries);
            ମୁକ୍ତ(keymap->types[i].level_names);
        }
        ମୁକ୍ତ(keymap->types);
    }
    ପାଇଁ (darray_size_t k = 0; k < keymap->ଅଙ୍କ_sym_interprets; k++) {
        clear_interpret(&keymap->sym_interprets[k]);
    }
    ମୁକ୍ତ(keymap->sym_interprets);
    ମୁକ୍ତ(keymap->modmaps);
    ମୁକ୍ତ(keymap->ଚାବି_aliases);
    ମୁକ୍ତ(keymap->group_names);
    ମୁକ୍ତ(keymap->keycodes_section_name);
    ମୁକ୍ତ(keymap->symbols_section_name);
    ମୁକ୍ତ(keymap->types_section_name);
    ମୁକ୍ତ(keymap->compat_section_name);
    xkb_context_unref(keymap->ctx);
    ମୁକ୍ତ(keymap);
}

ସ୍ଥିର ଅଚଳ ସଂରଚନା xkb_keymap_format_ops *
get_keymap_format_ops(ଗଣନା xkb_keymap_format format)
{
    ସ୍ଥିର ଅଚଳ ସଂରଚନା xkb_keymap_format_ops *keymap_format_ops[] = {
        [XKB_KEYMAP_FORMAT_TEXT_V1] = &text_v1_keymap_format_ops,
        [XKB_KEYMAP_FORMAT_TEXT_V2] = &text_v1_keymap_format_ops,
    };

    ଯଦି ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) format < 0 || (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) format >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ARRAY_SIZE(keymap_format_ops))
        ଫେରନ୍ତୁ ଖାଲି;

    ଫେରନ୍ତୁ keymap_format_ops[(ପୂର୍ଣ୍ଣସଂଖ୍ୟା) format];
}

ସଂରଚନା xkb_keymap *
xkb_keymap_new_from_rmlvo(ଅଚଳ ସଂରଚନା xkb_rmlvo_builder *rmlvo,
                          ଗଣନା xkb_keymap_format format,
                          ଗଣନା xkb_keymap_compile_flags flags)
{
    ଅଚଳ ସଂରଚନା xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    ଯଦି (!ops || !ops->keymap_new_from_rmlvo) {
        log_err_func(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ସଂରଚନା xkb_keymap *keymap = xkb_keymap_new(rmlvo->ctx, __func__, format,
                                               flags);
    ଯଦି (!keymap)
        ଫେରନ୍ତୁ ଖାଲି;

    ଯଦି (!ops->keymap_new_from_rmlvo(keymap, rmlvo)) {
        xkb_keymap_unref(keymap);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଫେରନ୍ତୁ keymap;
}

ସଂରଚନା xkb_keymap *
xkb_keymap_new_from_names2(ସଂରଚନା xkb_context *ctx,
                           ଅଚଳ ସଂରଚନା xkb_rule_names *rmlvo_in,
                           ଗଣନା xkb_keymap_format format,
                           ଗଣନା xkb_keymap_compile_flags flags)
{
    ଅଚଳ ସଂରଚନା xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    ଯଦି (!ops || !ops->keymap_new_from_names) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ସଂରଚନା xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    ଯଦି (!keymap)
        ଫେରନ୍ତୁ ଖାଲି;

    ସଂରଚନା xkb_rule_names rmlvo = {0};
    ଯଦି (rmlvo_in)
        rmlvo = *rmlvo_in;
    xkb_context_sanitize_rule_names(ctx, &rmlvo);

    ଯଦି (!ops->keymap_new_from_names(keymap, &rmlvo)) {
        xkb_keymap_unref(keymap);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଫେରନ୍ତୁ keymap;
}


ସଂରଚନା xkb_keymap *
xkb_keymap_new_from_names(ସଂରଚନା xkb_context *ctx,
                          ଅଚଳ ସଂରଚନା xkb_rule_names *rmlvo_in,
                          ଗଣନା xkb_keymap_compile_flags flags)
{
    ଫେରନ୍ତୁ xkb_keymap_new_from_names2(ctx, rmlvo_in,
                                      XKB_KEYMAP_FORMAT_TEXT_V2, flags);
}

ସଂରଚନା xkb_keymap *
xkb_keymap_new_from_string(ସଂରଚନା xkb_context *ctx,
                           ଅଚଳ ଅକ୍ଷର *string,
                           ଗଣନା xkb_keymap_format format,
                           ଗଣନା xkb_keymap_compile_flags flags)
{
    ଫେରନ୍ତୁ xkb_keymap_new_from_buffer(ctx, string, strlen(string),
                                      format, flags);
}

ସଂରଚନା xkb_keymap *
xkb_keymap_new_from_buffer(ସଂରଚନା xkb_context *ctx,
                           ଅଚଳ ଅକ୍ଷର *buffer, size_t length,
                           ଗଣନା xkb_keymap_format format,
                           ଗଣନା xkb_keymap_compile_flags flags)
{
    ଅଚଳ ସଂରଚନା xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    ଯଦି (!ops || !ops->keymap_new_from_string) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଯଦି (!buffer) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no buffer specified\n");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ସଂରଚନା xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    ଯଦି (!keymap)
        ଫେରନ୍ତୁ ଖାଲି;

    /* Allow a zero-terminated string as a buffer */
    ଯଦି (length > 0 && buffer[length - 1] == '\0')
        length--;

    ଯଦି (!ops->keymap_new_from_string(keymap, buffer, length)) {
        xkb_keymap_unref(keymap);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଫେରନ୍ତୁ keymap;
}

ସଂରଚନା xkb_keymap *
xkb_keymap_new_from_file(ସଂରଚନା xkb_context *ctx,
                         FILE *file,
                         ଗଣନା xkb_keymap_format format,
                         ଗଣନା xkb_keymap_compile_flags flags)
{
    ଅଚଳ ସଂରଚନା xkb_keymap_format_ops *ops = get_keymap_format_ops(format);
    ଯଦି (!ops || !ops->keymap_new_from_file) {
        log_err_func(ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", format);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଯଦି (!file) {
        log_err_func1(ctx, XKB_LOG_MESSAGE_NO_ID,
                      "no file specified\n");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ସଂରଚନା xkb_keymap *keymap = xkb_keymap_new(ctx, __func__, format, flags);
    ଯଦି (!keymap)
        ଫେରନ୍ତୁ ଖାଲି;

    ଯଦି (!ops->keymap_new_from_file(keymap, file)) {
        xkb_keymap_unref(keymap);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଫେରନ୍ତୁ keymap;
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
ସ୍ଥିର ଗଣନା xkb_status
check_keymap_serialize_abi(
    ସଂରଚନା xkb_context * restrict ctx,
    ଅଚଳ ଅକ୍ଷର * restrict func,
    ଅଚଳ ସଂରଚନା xkb_keymap_serialize_config * restrict config,
    ଅଚଳ ସଂରଚନା xkb_keymap_serialize_result * restrict result
)
{
    ଗଣନା xkb_status status;
    ଯଦି ((status = xkb_check_keymap_abi(config)) ||
        (status = xkb_check_keymap_abi(result))) {
        xkb_log_abi_error(ctx, func, status);
    }
    ଫେରନ୍ତୁ status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

ଗଣନା xkb_status
xkb_keymap_serialize(ଅଚଳ ସଂରଚନା xkb_keymap *keymap,
                     ଅଚଳ ସଂରଚନା xkb_keymap_serialize_config *config,
                     ସଂରଚନା xkb_keymap_serialize_result *result)
{
    /* Check ABI compatibility */
    ଅଚଳ ଗଣନା xkb_status status =
        check_keymap_serialize_abi(keymap->ctx, __func__, config, result);
    ଯଦି (status != XKB_SUCCESS)
        ଫେରନ୍ତୁ status;

    ସଂରଚନା xkb_keymap_serialize_config new_config = *config;

    ଅଚଳ ଗଣନା xkb_keymap_serialize_flags invalid_flags = (
        new_config.flags &
        ~(ଗଣନା xkb_keymap_serialize_flags)XKB_KEYMAP_SERIALIZE_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ
    );
    ଯଦି (invalid_flags) {
        log_err_func(keymap->ctx,
                     XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS_,
                     "unrecognized serialization flags: %#x\n",
                     invalid_flags);
        ଫେରନ୍ତୁ XKB_ERROR_UNSUPPORTED_KEYMAP_SERIALIZATION_FLAGS;
    }

    ଯଦି (new_config.format == XKB_KEYMAP_USE_ORIGINAL_FORMAT)
        new_config.format = keymap->format;

    ଅଚଳ ସଂରଚନା xkb_keymap_format_ops * ଅଚଳ ops =
        get_keymap_format_ops(new_config.format);
    ଯଦି (!ops || !ops->keymap_serialize) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT_,
                     "unsupported keymap format: %d\n", new_config.format);
        ଫେରନ୍ତୁ XKB_ERROR_UNSUPPORTED_KEYMAP_FORMAT;
    }

    ଅଚଳ xkb_layout_mask_t all_layouts =
        (xkb_layout_mask_t)((UINT64_C(1) << keymap->ଅଙ୍କ_groups) - 1);
    ଯଦି (!new_config.layouts) {
        new_config.layouts = all_layouts;
    }

    ଯଦି (new_config.layouts & ~all_layouts) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                     "unsupported layout mask: "
                     "expected subset of 0x%08"PRIx32", got: 0x%08"PRIx32"\n",
                     all_layouts, new_config.layouts);
        ଫେରନ୍ତୁ XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    ଅଚଳ xkb_layout_index_t max_groups = format_max_groups(config->format);
    ଅଚଳ xkb_layout_index_t ଅଙ୍କ_groups =
        (xkb_layout_index_t)popcount32(new_config.layouts);

    ଯଦି (ଅଙ୍କ_groups > max_groups) {
        ଅଚଳ bool strict =
            (new_config.flags & XKB_KEYMAP_SERIALIZE_STRICT_MODE);
        xkb_log_with_code(
            keymap->ctx,
            (strict ? XKB_LOG_LEVEL_ERROR : XKB_LOG_LEVEL_WARNING),
            XKB_LOG_VERBOSITY_MINIMAL,
            XKB_ERROR_LAYOUT_ଗଣନା_LIMIT_EXCEEDED_2,
            "Cannot serialize %"PRIu32" groups in keymap format %d: "
            "maximum is %"PRIu32"%s\n",
            ଅଙ୍କ_groups, config->format, max_groups,
            (strict ? "" : "; discarding unsupported groups")
        );
        ଯଦି (strict) {
            ଫେରନ୍ତୁ XKB_ERROR_LAYOUT_ଗଣନା_LIMIT_EXCEEDED;
        } ଅନ୍ୟଥା {
            new_config.layouts =
                keep_lowest_n_set_bits(new_config.layouts, max_groups);
        }
    }

    log_dbg(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Serializing group mask 0x%"PRIx32" (total: %"PRIu32")\n",
            new_config.layouts, ଅଙ୍କ_groups);

    ଯଦି (new_config.layouts != all_layouts) {
        /*
         * If some layouts are discarded, then interprets may not trigger
         * properly. Avoid this by requiring explicit values.
         */
        new_config.flags |= (XKB_KEYMAP_SERIALIZE_EXPLICIT_ଚାବି_ମୂଲ୍ୟଗୁଡ଼ିକ |
                             XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS);
    }

    ଫେରନ୍ତୁ ops->keymap_serialize(keymap, &new_config, result);
}

ଅକ୍ଷର *
xkb_keymap_get_as_string2(ସଂରଚନା xkb_keymap *keymap,
                          ଗଣନା xkb_keymap_format format,
                          ଗଣନା xkb_keymap_serialize_flags flags)
{
    ଅଚଳ ସଂରଚନା xkb_keymap_serialize_config config = {
        .size = ଆକାର(config),
        .flags = flags,
        .format = format,
        .layouts = 0,
    };

    ସଂରଚନା xkb_keymap_serialize_result result = { .size = ଆକାର(result) };

    ଫେରନ୍ତୁ (xkb_keymap_serialize(keymap, &config, &result) == XKB_SUCCESS)
        ? result.serialized
        : ଖାଲି;
}

ଅକ୍ଷର *
xkb_keymap_get_as_string(ସଂରଚନା xkb_keymap *keymap,
                         ଗଣନା xkb_keymap_format format)
{
    ଫେରନ୍ତୁ xkb_keymap_get_as_string2(keymap, format,
                                     XKB_KEYMAP_SERIALIZE_NO_FLAGS);
}

/**
 * Returns the total number of modifiers active in the keymap.
 */
xkb_mod_index_t
xkb_keymap_ଅଙ୍କ_mods(ସଂରଚନା xkb_keymap *keymap)
{
    ଫେରନ୍ତୁ keymap->mods.ଅଙ୍କ_mods;
}

/**
 * Return the name for a given modifier.
 */
ଅଚଳ ଅକ୍ଷର *
xkb_keymap_mod_get_name(ସଂରଚନା xkb_keymap *keymap, xkb_mod_index_t idx)
{
    ଯଦି (idx >= keymap->mods.ଅଙ୍କ_mods)
        ଫେରନ୍ତୁ ଖାଲି;

    ଫେରନ୍ତୁ xkb_atom_text(keymap->ctx, keymap->mods.mods[idx].name);
}

/**
 * Returns the index for a named modifier.
 */
xkb_mod_index_t
xkb_keymap_mod_get_index(ସଂରଚନା xkb_keymap *keymap, ଅଚଳ ଅକ୍ଷର *name)
{
    ଅଚଳ xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    ଫେରନ୍ତୁ (atom == XKB_ATOM_NONE)
        ? XKB_MOD_INVALID
        : XkbModNameToIndex(&keymap->mods, atom, MOD_BOTH);
}

/**
 * Return the canonical mapping of a named modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask(ସଂରଚନା xkb_keymap *keymap, ଅଚଳ ଅକ୍ଷର* name)
{
    ଅଚଳ xkb_mod_index_t idx = xkb_keymap_mod_get_index(keymap, name);
    ଫେରନ୍ତୁ (idx >= keymap->mods.ଅଙ୍କ_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the canonical mapping of a given modifier.
 */
xkb_mod_mask_t
xkb_keymap_mod_get_mask2(ସଂରଚନା xkb_keymap *keymap, xkb_mod_index_t idx)
{
    ଫେରନ୍ତୁ (idx >= keymap->mods.ଅଙ୍କ_mods)
        ? 0
        : keymap->mods.mods[idx].mapping;
}

/**
 * Return the total number of active groups in the keymap.
 */
xkb_layout_index_t
xkb_keymap_ଅଙ୍କ_layouts(ସଂରଚନା xkb_keymap *keymap)
{
    ଫେରନ୍ତୁ keymap->ଅଙ୍କ_groups;
}

/**
 * Returns the name for a given group.
 */
ଅଚଳ ଅକ୍ଷର *
xkb_keymap_layout_get_name(ସଂରଚନା xkb_keymap *keymap, xkb_layout_index_t idx)
{
    ଯଦି (idx >= keymap->ଅଙ୍କ_group_names)
        ଫେରନ୍ତୁ ଖାଲି;

    ଫେରନ୍ତୁ xkb_atom_text(keymap->ctx, keymap->group_names[idx]);
}

/**
 * Returns the index for a named layout.
 */
xkb_layout_index_t
xkb_keymap_layout_get_index(ସଂରଚନା xkb_keymap *keymap, ଅଚଳ ଅକ୍ଷର *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_layout_index_t i;

    ଯଦି (atom == XKB_ATOM_NONE)
        ଫେରନ୍ତୁ XKB_LAYOUT_INVALID;

    ପାଇଁ (i = 0; i < keymap->ଅଙ୍କ_group_names; i++)
        ଯଦି (keymap->group_names[i] == atom)
            ଫେରନ୍ତୁ i;

    ଫେରନ୍ତୁ XKB_LAYOUT_INVALID;
}

/**
 * Returns the number of layouts active for a particular key.
 */
xkb_layout_index_t
xkb_keymap_ଅଙ୍କ_layouts_for_ଚାବି(ସଂରଚନା xkb_keymap *keymap, xkb_keycode_t kc)
{
    ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି = Xkb_ଚାବି(keymap, kc);

    ଯଦି (!ଚାବି)
        ଫେରନ୍ତୁ 0;

    ଫେରନ୍ତୁ ଚାବି->ଅଙ୍କ_groups;
}

/**
 * Returns the number of levels active for a particular key and layout.
 */
xkb_level_index_t
xkb_keymap_ଅଙ୍କ_levels_for_ଚାବି(ସଂରଚନା xkb_keymap *keymap, xkb_keycode_t kc,
                              xkb_layout_index_t layout)
{
    ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି = Xkb_ଚାବି(keymap, kc);

    ଯଦି (!ଚାବି)
        ଫେରନ୍ତୁ 0;

    layout = xkb_keymap_ଚାବି_effective_layout(ଚାବି, layout);
    ଯଦି (layout == XKB_LAYOUT_INVALID)
        ଫେରନ୍ତୁ 0;

    ଫେରନ୍ତୁ Xkb_ଚାବି_ଅଙ୍କ_Levels(ଚାବି, layout);
}

/**
 * Return the total number of LEDs in the keymap.
 */
xkb_led_index_t
xkb_keymap_ଅଙ୍କ_leds(ସଂରଚନା xkb_keymap *keymap)
{
    ଫେରନ୍ତୁ keymap->ଅଙ୍କ_leds;
}

/**
 * Returns the name for a given LED.
 */
ଅଚଳ ଅକ୍ଷର *
xkb_keymap_led_get_name(ସଂରଚନା xkb_keymap *keymap, xkb_led_index_t idx)
{
    ଯଦି (idx >= keymap->ଅଙ୍କ_leds)
        ଫେରନ୍ତୁ ଖାଲି;

    ଫେରନ୍ତୁ xkb_atom_text(keymap->ctx, keymap->leds[idx].name);
}

/**
 * Returns the index for a named LED.
 */
xkb_led_index_t
xkb_keymap_led_get_index(ସଂରଚନା xkb_keymap *keymap, ଅଚଳ ଅକ୍ଷର *name)
{
    xkb_atom_t atom = xkb_atom_lookup(keymap->ctx, name);
    xkb_led_index_t i;
    ଅଚଳ ସଂରଚନା xkb_led *led;

    ଯଦି (atom == XKB_ATOM_NONE)
        ଫେରନ୍ତୁ XKB_LED_INVALID;

    xkb_leds_enumerate(i, led, keymap)
        ଯଦି (led->name == atom)
            ଫେରନ୍ତୁ i;

    ଫେରନ୍ତୁ XKB_LED_INVALID;
}

size_t
xkb_keymap_ଚାବି_get_mods_for_level(ସଂରଚନା xkb_keymap *keymap,
                                  xkb_keycode_t kc,
                                  xkb_layout_index_t layout,
                                  xkb_level_index_t level,
                                  xkb_mod_mask_t *masks_out,
                                  size_t masks_size)
{
    ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି = Xkb_ଚାବି(keymap, kc);
    ଯଦି (!ଚାବି)
        ଫେରନ୍ତୁ 0;

    layout = xkb_keymap_ଚାବି_effective_layout(ଚାବି, layout);
    ଯଦି (layout == XKB_LAYOUT_INVALID)
        ଫେରନ୍ତୁ 0;

    ଯଦି (level >= Xkb_ଚାବି_ଅଙ୍କ_Levels(ଚାବି, layout))
        ଫେରନ୍ତୁ 0;

    ଅଚଳ ସଂରଚନା xkb_ଚାବି_type *type = ଚାବି->groups[layout].type;

    size_t ଗଣନା_2 = 0;

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
    ଯଦି (level == 0) {
        bool empty_mapped = false;
        ପାଇଁ (darray_size_t i = 0; i < type->ଅଙ୍କ_entries && ଗଣନା_2 < masks_size; i++)
            ଯଦି (entry_is_active(&type->entries[i]) &&
                type->entries[i].mods.mask == 0) {
                empty_mapped = true;
                ଭାଙ୍ଗ;
            }
        ଯଦି (!empty_mapped && ଗଣନା_2 < masks_size) {
            masks_out[ଗଣନା_2++] = 0;
        }
    }

    /* Now search explicit mappings. */
    ପାଇଁ (darray_size_t i = 0; i < type->ଅଙ୍କ_entries && ଗଣନା_2 < masks_size; i++) {
        ଯଦି (entry_is_active(&type->entries[i]) &&
            type->entries[i].level == level) {
            masks_out[ଗଣନା_2++] = type->entries[i].mods.mask;
        }
    }

    ଫେରନ୍ତୁ ଗଣନା_2;
}

ସଂରଚନା xkb_level *
xkb_keymap_ଚାବି_get_level(ସଂରଚନା xkb_keymap *keymap, ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                         xkb_layout_index_t layout, xkb_level_index_t level)
{
    layout = xkb_keymap_ଚାବି_effective_layout(ଚାବି, layout);
    ଯଦି (layout == XKB_LAYOUT_INVALID)
        ଫେରନ୍ତୁ ଖାଲି;

    ଯଦି (level >= Xkb_ଚାବି_ଅଙ୍କ_Levels(ଚାବି, layout))
        ଫେରନ୍ତୁ ଖାଲି;

    ଫେରନ୍ତୁ &ଚାବି->groups[layout].levels[level];
}

/**
 * As below, but takes an explicit layout/level rather than state.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_keymap_ଚାବି_get_syms_by_level(ସଂରଚନା xkb_keymap *keymap,
                                 xkb_keycode_t kc,
                                 xkb_layout_index_t layout,
                                 xkb_level_index_t level,
                                 ଅଚଳ xkb_keysym_t **syms_out)
{
    ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି = Xkb_ଚାବି(keymap, kc);

    ଯଦି (!ଚାବି)
        ଯାଅ err;

    ଅଚଳ ସଂରଚନା xkb_level* ଅଚଳ leveli =
        xkb_keymap_ଚାବି_get_level(keymap, ଚାବି, layout, level);

    ଯଦି (!leveli)
        ଯାଅ err;

    ଅଚଳ xkb_keysym_ଗଣନା_t ଅଙ୍କ_syms = leveli->ଅଙ୍କ_syms;
    ଯଦି (ଅଙ୍କ_syms == 0)
        ଯାଅ err;

    ଯଦି (ଅଙ୍କ_syms == 1)
        *syms_out = &leveli->s.sym;
    ଅନ୍ୟଥା
        *syms_out = leveli->s.syms;

    ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଅଙ୍କ_syms;

err:
    *syms_out = ଖାଲି;
    ଫେରନ୍ତୁ 0;
}

xkb_keycode_t
xkb_keymap_min_keycode(ସଂରଚନା xkb_keymap *keymap)
{
    ଫେରନ୍ତୁ keymap->min_ଚାବି_code;
}

xkb_keycode_t
xkb_keymap_max_keycode(ସଂରଚନା xkb_keymap *keymap)
{
    ଫେରନ୍ତୁ keymap->max_ଚାବି_code;
}

ସ୍ଥିର ଗଣନା xkb_status
keymap_ଚାବି_iterator_config_check(
    ସଂରଚନା xkb_context * restrict ctx,
    ଅଚଳ ଅକ୍ଷର *func,
    ଅଚଳ ସଂରଚନା xkb_keymap_ଚାବି_iterator_config * restrict config
)
{
    /* Check ABI compatibility */
    ଅଚଳ ଗଣନା xkb_status abi_error = xkb_check_keymap_abi(config);
    ଯଦି (abi_error) {
        xkb_log_abi_error(ctx, func, abi_error);
        ଫେରନ୍ତୁ abi_error;
    }

    /* Sanitize input: flags */
    ଅଚଳ uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_KEYMAP_ଚାବି_ITERATOR_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ);
    ଯଦି (invalid_flags) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_ଚାବି_ITERATOR_FLAGS_2,
                "%s: Unsupported keymap iterator flags: 0x%"PRIx32"\n",
                func, invalid_flags);
        ଫେରନ୍ତୁ XKB_ERROR_UNSUPPORTED_ଚାବି_ITERATOR_FLAGS;
    }

    /* Sanitize input: start */
    ଯଦି (config->start > XKB_KEYCODE_MAX)
        ଫେରନ୍ତୁ XKB_ERROR_INVALID_KEYCODE;

    ଫେରନ୍ତୁ XKB_SUCCESS;
}

static_assert(XKB_KEYMAP_ଚାବି_ITERATOR_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ <=
              (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)XKB_POINTER_TAG_MASK,
              "Cannot tag cursor pointer with flags");

#define iter_init_keymap(i, ptr, flags) {          \
    (i)->opaque0 = xkb_pointer_tag((ptr), (flags));\
}
#define iter_get_keymap(i) (                                  \
    (ଅଚଳ ସଂରଚନା xkb_keymap *)xkb_pointer_untag((i)->opaque0)\
)
#define iter_get_next(i) ((ଅଚଳ ସଂରଚନା xkb_ଚାବି *)(i)->opaque1)
#define iter_set_next(i, ptr) {(i)->opaque1 = (ଅଚଳ ଶୂନ୍ୟ *)(ptr);}
#define iter_get_descending_order(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                 \
    (uintptr_t)XKB_KEYMAP_ଚାବି_ITERATOR_DESCENDING_ORDER \
)
#define iter_get_include_unbound(i) (                  \
    xkb_pointer_get_tag((i)->opaque0) &                \
    (uintptr_t)XKB_KEYMAP_ଚାବି_ITERATOR_INCLUDE_UNBOUND \
)

ଗଣନା xkb_status
xkb_keymap_ଚାବି_iterator_init(
    ସଂରଚନା xkb_keymap_ଚାବି_iterator * restrict iter,
    ଅଚଳ ସଂରଚନା xkb_keymap * restrict keymap,
    ଅଚଳ ସଂରଚନା xkb_keymap_ଚାବି_iterator_config * restrict config
)
{
    /* Handle default configuration */
    ସ୍ଥିର ଅଚଳ ସଂରଚନା xkb_keymap_ଚାବି_iterator_config default_config = {
        .size = ଆକାର(default_config)
    };
    ଯଦି (!config)
        config = &default_config;

    /* Check input */
    ଅଚଳ ଗଣନା xkb_status status =
        keymap_ଚାବି_iterator_config_check(keymap->ctx, __func__, config);
    ଯଦି (status != XKB_SUCCESS) {
        ଫେରନ୍ତୁ status;
    }

    /*
     * Initialization
     */

    memset(iter, 0, ଆକାର(*iter));

    /* Borrow keymap */
    iter_init_keymap(iter, keymap, config->flags);

    ଯଦି (keymap->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ == 0) {
        iter_set_next(iter, ଖାଲି);
        ଫେରନ୍ତୁ XKB_SUCCESS;
    }

    /* Borrow key */
    ଅଚଳ ସଂରଚନା xkb_ଚାବି *next;
    ଯଦି (config->flags & XKB_KEYMAP_ଚାବି_ITERATOR_DESCENDING_ORDER) {
        ଯଦି (!config->start || config->start > keymap->max_ଚାବି_code) {
            next = &keymap->ଚାବିଗୁଡ଼ିକ[keymap->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ - 1];
        } ଅନ୍ୟଥା {
            /*
             * May still fail *safely* if start < keymap->min_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_ଚାବି(keymap, false, config->start);
        }
    } ଅନ୍ୟଥା {
        ଯଦି (!config->start || config->start < keymap->min_ଚାବି_code) {
            next = (keymap->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ_ତଳ)
                ? &keymap->ଚାବିଗୁଡ଼ିକ[keymap->min_ଚାବି_code]
                : &keymap->ଚାବିଗୁଡ଼ିକ[0];
        } ଅନ୍ୟଥା {
            /*
             * May still fail *safely* if start > keymap->max_key_code:
             * there will be simply no key to iterate over.
             */
            next = xkb_keymap_get_next_defined_ଚାବି(keymap, true, config->start);
        }
    };
    iter_set_next(iter, next);

    ଫେରନ୍ତୁ XKB_SUCCESS;
}

xkb_keycode_t
xkb_keymap_ଚାବି_iterator_next(ସଂରଚନା xkb_keymap_ଚାବି_iterator *iter)
{
    ଅଚଳ ସଂରଚନା xkb_ଚାବି *next = iter_get_next(iter);
    ଯଦି (!next)
        ଫେରନ୍ତୁ XKB_KEYCODE_INVALID;

    ଅଚଳ ସଂରଚନା xkb_keymap *keymap = iter_get_keymap(iter);
    ପ୍ରତିଜ୍ଞା(keymap->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ);

    ଅଚଳ bool descending = iter_get_descending_order(iter);
    ଅଚଳ bool skip_unbound = !iter_get_include_unbound(iter);
    ଅଚଳ ସଂରଚନା xkb_ଚାବି *last = descending
        ? (keymap->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ_ତଳ)
            ? &keymap->ଚାବିଗୁଡ଼ିକ[keymap->min_ଚାବି_code]
            : &keymap->ଚାବିଗୁଡ଼ିକ[0]
        : &keymap->ଚାବିଗୁଡ଼ିକ[keymap->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ - 1];
    ଅଚଳ ptrdiff_t step = descending ? -1 : 1;

    /* Skip undefined keys (no name) and optionally unbound keys */
    ପାଇଁ (;;) {
        ଅଚଳ bool iter_end = descending ? (next <= last) : (next >= last);
        ଅଚଳ bool skip = (
            next->name == XKB_ATOM_NONE ||
            (skip_unbound && (next->ଅଙ୍କ_groups == 0))
        );
        ଯଦି (!skip) {
            ଅଚଳ xkb_keycode_t ret = next->keycode;
            iter_set_next(iter, iter_end ? ଖାଲି : next + step);
            ଫେରନ୍ତୁ ret;
        }
        ଯଦି (iter_end) {
            iter_set_next(iter, ଖାଲି);
            ଫେରନ୍ତୁ XKB_KEYCODE_INVALID;
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

ଶୂନ୍ୟ
xkb_keymap_ଚାବି_for_each(ସଂରଚନା xkb_keymap *keymap, xkb_keymap_ଚାବି_iter_t iter,
                        ଶୂନ୍ୟ *data)
{
    ସଂରଚନା xkb_ଚାବି *ଚାବି;

    xkb_ଚାବିଗୁଡ଼ିକ_foreach(ଚାବି, keymap)
        iter(keymap, ଚାବି->keycode, data);
}

ଅଚଳ ଅକ୍ଷର *
xkb_keymap_ଚାବି_get_name(ସଂରଚନା xkb_keymap *keymap, xkb_keycode_t kc)
{
    ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି = Xkb_ଚାବି(keymap, kc);

    ଯଦି (!ଚାବି)
        ଫେରନ୍ତୁ ଖାଲି;

    ଫେରନ୍ତୁ xkb_atom_text(keymap->ctx, ଚାବି->name);
}

xkb_keycode_t
xkb_keymap_ଚାବି_by_name(ସଂରଚନା xkb_keymap *keymap, ଅଚଳ ଅକ୍ଷର *name)
{
    ସଂରଚନା xkb_ଚାବି *ଚାବି;
    xkb_atom_t atom;

    atom = xkb_atom_lookup(keymap->ctx, name);
    ଯଦି (atom) {
        ପାଇଁ (darray_size_t i = 0; i < keymap->ଅଙ୍କ_ଚାବି_aliases; i++)
            ଯଦି (keymap->ଚାବି_aliases[i].alias == atom)
                atom = keymap->ଚାବି_aliases[i].real;
    }
    ଯଦି (!atom)
        ଫେରନ୍ତୁ XKB_KEYCODE_INVALID;

    xkb_ଚାବିଗୁଡ଼ିକ_foreach(ଚାବି, keymap) {
        ଯଦି (ଚାବି->name == atom)
            ଫେରନ୍ତୁ ଚାବି->keycode;
    }

    ଫେରନ୍ତୁ XKB_KEYCODE_INVALID;
}

/**
 * Simple boolean specifying whether or not the key should repeat.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_keymap_ଚାବି_repeats(ସଂରଚନା xkb_keymap *keymap, xkb_keycode_t kc)
{
    ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି = Xkb_ଚାବି(keymap, kc);

    ଯଦି (!ଚାବି)
        ଫେରନ୍ତୁ 0;

    ଫେରନ୍ତୁ ଚାବି->repeats;
}
