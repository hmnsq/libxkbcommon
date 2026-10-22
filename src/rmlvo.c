/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <stdint.h>
#અન્તર્ભાવ <string.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "xkbcommon/xkbcommon-status.h"
#અન્તર્ભાવ "context.h"
#અન્તર્ભાવ "features/enums.h"
#અન્તર્ભાવ "keymap.h"
#અન્તર્ભાવ "messages-codes.h"
#અન્તર્ભાવ "rmlvo.h"
#અન્તર્ભાવ "darray.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "xkbcomp/rules.h"

સંરચના xkb_rmlvo_builder*
xkb_rmlvo_builder_new(સંરચના xkb_context *context,
                      અચલ અક્ષર *rules, અચલ અક્ષર *model,
                      ગણના xkb_rmlvo_builder_flags flags)
{
    અચલ ગણના xkb_rmlvo_builder_flags invalid_flags =
        (flags & ~(ગણના xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_કિંમતો);
    જો (invalid_flags) {
        log_err_func(context, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        પાછા ખાલી;
    }

    સંરચના xkb_rmlvo_builder * અચલ builder = calloc(1, કદ(*builder));
    જો (!builder)
        જાઓ error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->rules = strdup_safe(rules);
    જો (!builder->rules && rules)
        જાઓ error;

    builder->model = strdup_safe(model);
    જો (!builder->model && model)
        જાઓ error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    પાછા builder;

error:
    log_err(context, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    પાછા ખાલી;
}

સ્થિર ગણના xkb_status
rmlvo_builder_append_option(સંરચના xkb_rmlvo_builder *rmlvo,
                            અચલ અક્ષર *option, xkb_layout_mask_t layouts)
{
    જો (isempty(option))
        પાછા XKB_ERROR_INVALID;

    /* Check for previous entry */
    સંરચના xkb_rmlvo_builder_option *prev;
    darray_foreach(prev, rmlvo->options) {
        જો (streq(prev->option, option)) {
            /* Merge with previous entry */
            જો (layouts == (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Global option overrides layout-specific ones */
                prev->layouts = layouts;
            } નહીંતર જો (prev->layouts != (xkb_layout_mask_t)
                       XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Layout-specific option cannot override global */
                prev->layouts |= layouts;
            }
            પાછા XKB_SUCCESS;
        }
    }

    /* Append new entry */
    અચલ સંરચના xkb_rmlvo_builder_option new = {
        .option = strdup_safe(option),
        .layouts = layouts
    };
    જો (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        પાછા XKB_ERROR_ALLOCATION_FAILURE;
    }
    darray_append(rmlvo->options, new);
    પાછા XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(સંરચના xkb_rmlvo_builder *rmlvo,
                                અચલ અક્ષર *layout, અચલ અક્ષર *variant,
                                અચલ અક્ષર* અચલ* options, size_t options_len)
{
    અચલ xkb_layout_index_t idx = (xkb_layout_index_t)
                                   darray_size(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    જો (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        પાછા false;
    }

    /* Append layout entry */
    અચલ સંરચના xkb_rmlvo_builder_layout new = {
        .layout = strdup_safe(layout),
        .variant = strdup_safe(variant)
    };

    જો (!new.layout || (!new.variant && variant)) {
        મુક્ત(new.layout);
        મુક્ત(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        પાછા false;
    }

    darray_append(rmlvo->layouts, new);

    જો (!options)
        options_len = 0;

    અચલ xkb_layout_mask_t layout_mask = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    માટે (size_t k = 0; k < options_len; k++) {
        અચલ ગણના xkb_status status =
            rmlvo_builder_append_option(rmlvo, options[k], layout_mask);

        જો (status == XKB_ERROR_ALLOCATION_FAILURE)
            પાછા false;
    }

    પાછા true;
}

bool
xkb_rmlvo_builder_append_option(સંરચના xkb_rmlvo_builder *rmlvo,
                                અચલ અક્ષર *option)
{
    અચલ ગણના xkb_status status = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL
    );
    પાછા (status == XKB_SUCCESS);
}

સંરચના xkb_rmlvo_builder *
xkb_rmlvo_builder_ref(સંરચના xkb_rmlvo_builder *rmlvo)
{
    પ્રતિજ્ઞા(rmlvo->refcnt > 0);
    rmlvo->refcnt++;
    પાછા rmlvo;
}

શૂન્ય
xkb_rmlvo_builder_unref(સંરચના xkb_rmlvo_builder *rmlvo)
{
    પ્રતિજ્ઞા(!rmlvo || rmlvo->refcnt > 0);
    જો (!rmlvo || --rmlvo->refcnt > 0)
        પાછા;

    મુક્ત(rmlvo->rules);
    મુક્ત(rmlvo->model);

    અચલ સંરચના xkb_rmlvo_builder_layout *layout;
    darray_foreach(layout, rmlvo->layouts) {
        મુક્ત(layout->layout);
        મુક્ત(layout->variant);
    }
    darray_free(rmlvo->layouts);

    અચલ સંરચના xkb_rmlvo_builder_option *option;
    darray_foreach(option, rmlvo->options)
        મુક્ત(option->option);
    darray_free(rmlvo->options);

    xkb_context_unref(rmlvo->ctx);
    મુક્ત(rmlvo);
}

/* NOTE: the converse function, `xkb_rules_names_to_rmlvo_builder`, is currently
 * only used in tests */
bool
xkb_rmlvo_builder_to_rules_names(અચલ સંરચના xkb_rmlvo_builder *builder,
                                 સંરચના xkb_rule_names *rmlvo,
                                 અક્ષર *buf, size_t buf_size)
{
    rmlvo->rules = builder->rules;
    rmlvo->model = builder->model;

    અક્ષર *start = buf;
    rmlvo->layout = start;
    darray_size_t k;
    અચલ સંરચના xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        પૂર્ણાંક ગણતરી = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        જો (ગણતરી < 0 || (size_t)ગણતરી >= buf_size)
            પાછા false;
        buf_size -= (size_t)ગણતરી;
        start += ગણતરી;
    }
    જો (buf_size <= 1)
        પાછા false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->variant = start;
    darray_enumerate(k, layout, builder->layouts) {
        પૂર્ણાંક ગણતરી = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        જો (ગણતરી < 0 || (size_t) ગણતરી >= buf_size)
            પાછા false;
        buf_size -= (size_t)ગણતરી;
        start += ગણતરી;
    }
    જો (buf_size <= 1)
        પાછા false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->options = start;
    અચલ સંરચના xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        પૂર્ણાંક ગણતરી = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        જો (ગણતરી < 0 || (size_t) ગણતરી >= buf_size)
            પાછા false;
        buf_size -= (size_t)ગણતરી;
        start += ગણતરી;
        જો (option->layouts != XKB_LAYOUT_INVALID) {
            ગણતરી = snprintf(start, buf_size, "%c%"PRIu32,
                             OPTIONS_GROUP_SPECIFIER_PREFIX, option->layouts);
            જો (ગણતરી < 0 || (size_t) ગણતરી >= buf_size)
                પાછા false;
            buf_size -= (size_t)ગણતરી;
            start += ગણતરી;
        }
    }
    જો (buf_size == 0)
        પાછા false;
    *start = '\0';
    પાછા true;
}
