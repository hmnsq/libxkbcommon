/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ <string.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "xkbcommon/xkbcommon-status.h"
#அந்தர்பாவ "context.h"
#அந்தர்பாவ "features/enums.h"
#அந்தர்பாவ "keymap.h"
#அந்தர்பாவ "messages-codes.h"
#அந்தர்பாவ "rmlvo.h"
#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "xkbcomp/rules.h"

கட்டமைப்பு xkb_rmlvo_builder*
xkb_rmlvo_builder_new(கட்டமைப்பு xkb_context *context,
                      மாறிலி எழுத்து *rules, மாறிலி எழுத்து *model,
                      எண்ணிக்கை xkb_rmlvo_builder_flags flags)
{
    மாறிலி எண்ணிக்கை xkb_rmlvo_builder_flags invalid_flags =
        (flags & ~(எண்ணிக்கை xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_மதிப்புகள்);
    எனில் (invalid_flags) {
        log_err_func(context, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        திரும்பு வெற்று;
    }

    கட்டமைப்பு xkb_rmlvo_builder * மாறிலி builder = calloc(1, அளவு(*builder));
    எனில் (!builder)
        செல் error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->rules = strdup_safe(rules);
    எனில் (!builder->rules && rules)
        செல் error;

    builder->model = strdup_safe(model);
    எனில் (!builder->model && model)
        செல் error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    திரும்பு builder;

error:
    log_err(context, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    திரும்பு வெற்று;
}

நிலையான எண்ணிக்கை xkb_status
rmlvo_builder_append_option(கட்டமைப்பு xkb_rmlvo_builder *rmlvo,
                            மாறிலி எழுத்து *option, xkb_layout_mask_t layouts)
{
    எனில் (isempty(option))
        திரும்பு XKB_ERROR_INVALID;

    /* Check for previous entry */
    கட்டமைப்பு xkb_rmlvo_builder_option *prev;
    darray_foreach(prev, rmlvo->options) {
        எனில் (streq(prev->option, option)) {
            /* Merge with previous entry */
            எனில் (layouts == (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Global option overrides layout-specific ones */
                prev->layouts = layouts;
            } இல்லையெனில் எனில் (prev->layouts != (xkb_layout_mask_t)
                       XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Layout-specific option cannot override global */
                prev->layouts |= layouts;
            }
            திரும்பு XKB_SUCCESS;
        }
    }

    /* Append new entry */
    மாறிலி கட்டமைப்பு xkb_rmlvo_builder_option new = {
        .option = strdup_safe(option),
        .layouts = layouts
    };
    எனில் (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        திரும்பு XKB_ERROR_ALLOCATION_FAILURE;
    }
    darray_append(rmlvo->options, new);
    திரும்பு XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(கட்டமைப்பு xkb_rmlvo_builder *rmlvo,
                                மாறிலி எழுத்து *layout, மாறிலி எழுத்து *variant,
                                மாறிலி எழுத்து* மாறிலி* options, size_t options_len)
{
    மாறிலி xkb_layout_index_t idx = (xkb_layout_index_t)
                                   darray_size(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    எனில் (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        திரும்பு false;
    }

    /* Append layout entry */
    மாறிலி கட்டமைப்பு xkb_rmlvo_builder_layout new = {
        .layout = strdup_safe(layout),
        .variant = strdup_safe(variant)
    };

    எனில் (!new.layout || (!new.variant && variant)) {
        விடுவி(new.layout);
        விடுவி(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        திரும்பு false;
    }

    darray_append(rmlvo->layouts, new);

    எனில் (!options)
        options_len = 0;

    மாறிலி xkb_layout_mask_t layout_mask = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    க்கு (size_t k = 0; k < options_len; k++) {
        மாறிலி எண்ணிக்கை xkb_status status =
            rmlvo_builder_append_option(rmlvo, options[k], layout_mask);

        எனில் (status == XKB_ERROR_ALLOCATION_FAILURE)
            திரும்பு false;
    }

    திரும்பு true;
}

bool
xkb_rmlvo_builder_append_option(கட்டமைப்பு xkb_rmlvo_builder *rmlvo,
                                மாறிலி எழுத்து *option)
{
    மாறிலி எண்ணிக்கை xkb_status status = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL
    );
    திரும்பு (status == XKB_SUCCESS);
}

கட்டமைப்பு xkb_rmlvo_builder *
xkb_rmlvo_builder_ref(கட்டமைப்பு xkb_rmlvo_builder *rmlvo)
{
    ப்ரதிஜ்ஞா(rmlvo->refcnt > 0);
    rmlvo->refcnt++;
    திரும்பு rmlvo;
}

வெறுமை
xkb_rmlvo_builder_unref(கட்டமைப்பு xkb_rmlvo_builder *rmlvo)
{
    ப்ரதிஜ்ஞா(!rmlvo || rmlvo->refcnt > 0);
    எனில் (!rmlvo || --rmlvo->refcnt > 0)
        திரும்பு;

    விடுவி(rmlvo->rules);
    விடுவி(rmlvo->model);

    மாறிலி கட்டமைப்பு xkb_rmlvo_builder_layout *layout;
    darray_foreach(layout, rmlvo->layouts) {
        விடுவி(layout->layout);
        விடுவி(layout->variant);
    }
    darray_free(rmlvo->layouts);

    மாறிலி கட்டமைப்பு xkb_rmlvo_builder_option *option;
    darray_foreach(option, rmlvo->options)
        விடுவி(option->option);
    darray_free(rmlvo->options);

    xkb_context_unref(rmlvo->ctx);
    விடுவி(rmlvo);
}

/* NOTE: the converse function, `xkb_rules_names_to_rmlvo_builder`, is currently
 * only used in tests */
bool
xkb_rmlvo_builder_to_rules_names(மாறிலி கட்டமைப்பு xkb_rmlvo_builder *builder,
                                 கட்டமைப்பு xkb_rule_names *rmlvo,
                                 எழுத்து *buf, size_t buf_size)
{
    rmlvo->rules = builder->rules;
    rmlvo->model = builder->model;

    எழுத்து *start = buf;
    rmlvo->layout = start;
    darray_size_t k;
    மாறிலி கட்டமைப்பு xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        முழு மொத்தம் = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        எனில் (மொத்தம் < 0 || (size_t)மொத்தம் >= buf_size)
            திரும்பு false;
        buf_size -= (size_t)மொத்தம்;
        start += மொத்தம்;
    }
    எனில் (buf_size <= 1)
        திரும்பு false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->variant = start;
    darray_enumerate(k, layout, builder->layouts) {
        முழு மொத்தம் = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        எனில் (மொத்தம் < 0 || (size_t) மொத்தம் >= buf_size)
            திரும்பு false;
        buf_size -= (size_t)மொத்தம்;
        start += மொத்தம்;
    }
    எனில் (buf_size <= 1)
        திரும்பு false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->options = start;
    மாறிலி கட்டமைப்பு xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        முழு மொத்தம் = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        எனில் (மொத்தம் < 0 || (size_t) மொத்தம் >= buf_size)
            திரும்பு false;
        buf_size -= (size_t)மொத்தம்;
        start += மொத்தம்;
        எனில் (option->layouts != XKB_LAYOUT_INVALID) {
            மொத்தம் = snprintf(start, buf_size, "%c%"PRIu32,
                             OPTIONS_GROUP_SPECIFIER_PREFIX, option->layouts);
            எனில் (மொத்தம் < 0 || (size_t) மொத்தம் >= buf_size)
                திரும்பு false;
            buf_size -= (size_t)மொத்தம்;
            start += மொத்தம்;
        }
    }
    எனில் (buf_size == 0)
        திரும்பு false;
    *start = '\0';
    திரும்பு true;
}
