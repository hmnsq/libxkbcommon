/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "xkbcommon/xkbcommon-status.h"
#अन्तर्भाव "context.h"
#अन्तर्भाव "features/enums.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "rmlvo.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "xkbcomp/rules.h"

संरचना xkb_rmlvo_builder*
xkb_rmlvo_builder_new(संरचना xkb_context *context,
                      ध्रुव वर्ण *rules, ध्रुव वर्ण *model,
                      गणना xkb_rmlvo_builder_flags flags)
{
    ध्रुव गणना xkb_rmlvo_builder_flags invalid_flags =
        (flags & ~(गणना xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_मानानि);
    सचे (invalid_flags) {
        log_err_func(context, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        पटिनिवत्त रिक्त;
    }

    संरचना xkb_rmlvo_builder * ध्रुव builder = calloc(1, परिमाण(*builder));
    सचे (!builder)
        गच्छ error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->rules = strdup_safe(rules);
    सचे (!builder->rules && rules)
        गच्छ error;

    builder->model = strdup_safe(model);
    सचे (!builder->model && model)
        गच्छ error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    पटिनिवत्त builder;

error:
    log_err(context, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    पटिनिवत्त रिक्त;
}

स्थिर गणना xkb_status
rmlvo_builder_append_option(संरचना xkb_rmlvo_builder *rmlvo,
                            ध्रुव वर्ण *option, xkb_layout_mask_t layouts)
{
    सचे (isempty(option))
        पटिनिवत्त XKB_ERROR_INVALID;

    /* Check for previous entry */
    संरचना xkb_rmlvo_builder_option *prev;
    darray_foreach(prev, rmlvo->options) {
        सचे (streq(prev->option, option)) {
            /* Merge with previous entry */
            सचे (layouts == (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Global option overrides layout-specific ones */
                prev->layouts = layouts;
            } अञ्ञथा सचे (prev->layouts != (xkb_layout_mask_t)
                       XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Layout-specific option cannot override global */
                prev->layouts |= layouts;
            }
            पटिनिवत्त XKB_SUCCESS;
        }
    }

    /* Append new entry */
    ध्रुव संरचना xkb_rmlvo_builder_option new = {
        .option = strdup_safe(option),
        .layouts = layouts
    };
    सचे (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        पटिनिवत्त XKB_ERROR_ALLOCATION_FAILURE;
    }
    darray_append(rmlvo->options, new);
    पटिनिवत्त XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(संरचना xkb_rmlvo_builder *rmlvo,
                                ध्रुव वर्ण *layout, ध्रुव वर्ण *variant,
                                ध्रुव वर्ण* ध्रुव* options, size_t options_len)
{
    ध्रुव xkb_layout_index_t idx = (xkb_layout_index_t)
                                   darray_size(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    सचे (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        पटिनिवत्त false;
    }

    /* Append layout entry */
    ध्रुव संरचना xkb_rmlvo_builder_layout new = {
        .layout = strdup_safe(layout),
        .variant = strdup_safe(variant)
    };

    सचे (!new.layout || (!new.variant && variant)) {
        मुक्त(new.layout);
        मुक्त(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        पटिनिवत्त false;
    }

    darray_append(rmlvo->layouts, new);

    सचे (!options)
        options_len = 0;

    ध्रुव xkb_layout_mask_t layout_mask = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    अत्थाय (size_t k = 0; k < options_len; k++) {
        ध्रुव गणना xkb_status status =
            rmlvo_builder_append_option(rmlvo, options[k], layout_mask);

        सचे (status == XKB_ERROR_ALLOCATION_FAILURE)
            पटिनिवत्त false;
    }

    पटिनिवत्त true;
}

bool
xkb_rmlvo_builder_append_option(संरचना xkb_rmlvo_builder *rmlvo,
                                ध्रुव वर्ण *option)
{
    ध्रुव गणना xkb_status status = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL
    );
    पटिनिवत्त (status == XKB_SUCCESS);
}

संरचना xkb_rmlvo_builder *
xkb_rmlvo_builder_ref(संरचना xkb_rmlvo_builder *rmlvo)
{
    प्रतिज्ञा(rmlvo->refcnt > 0);
    rmlvo->refcnt++;
    पटिनिवत्त rmlvo;
}

शून्य
xkb_rmlvo_builder_unref(संरचना xkb_rmlvo_builder *rmlvo)
{
    प्रतिज्ञा(!rmlvo || rmlvo->refcnt > 0);
    सचे (!rmlvo || --rmlvo->refcnt > 0)
        पटिनिवत्त;

    मुक्त(rmlvo->rules);
    मुक्त(rmlvo->model);

    ध्रुव संरचना xkb_rmlvo_builder_layout *layout;
    darray_foreach(layout, rmlvo->layouts) {
        मुक्त(layout->layout);
        मुक्त(layout->variant);
    }
    darray_free(rmlvo->layouts);

    ध्रुव संरचना xkb_rmlvo_builder_option *option;
    darray_foreach(option, rmlvo->options)
        मुक्त(option->option);
    darray_free(rmlvo->options);

    xkb_context_unref(rmlvo->ctx);
    मुक्त(rmlvo);
}

/* NOTE: the converse function, `xkb_rules_names_to_rmlvo_builder`, is currently
 * only used in tests */
bool
xkb_rmlvo_builder_to_rules_names(ध्रुव संरचना xkb_rmlvo_builder *builder,
                                 संरचना xkb_rule_names *rmlvo,
                                 वर्ण *buf, size_t buf_size)
{
    rmlvo->rules = builder->rules;
    rmlvo->model = builder->model;

    वर्ण *start = buf;
    rmlvo->layout = start;
    darray_size_t k;
    ध्रुव संरचना xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        पूर्णाङ्क गणनम् = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        सचे (गणनम् < 0 || (size_t)गणनम् >= buf_size)
            पटिनिवत्त false;
        buf_size -= (size_t)गणनम्;
        start += गणनम्;
    }
    सचे (buf_size <= 1)
        पटिनिवत्त false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->variant = start;
    darray_enumerate(k, layout, builder->layouts) {
        पूर्णाङ्क गणनम् = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        सचे (गणनम् < 0 || (size_t) गणनम् >= buf_size)
            पटिनिवत्त false;
        buf_size -= (size_t)गणनम्;
        start += गणनम्;
    }
    सचे (buf_size <= 1)
        पटिनिवत्त false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->options = start;
    ध्रुव संरचना xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        पूर्णाङ्क गणनम् = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        सचे (गणनम् < 0 || (size_t) गणनम् >= buf_size)
            पटिनिवत्त false;
        buf_size -= (size_t)गणनम्;
        start += गणनम्;
        सचे (option->layouts != XKB_LAYOUT_INVALID) {
            गणनम् = snprintf(start, buf_size, "%c%"PRIu32,
                             OPTIONS_GROUP_SPECIFIER_PREFIX, option->layouts);
            सचे (गणनम् < 0 || (size_t) गणनम् >= buf_size)
                पटिनिवत्त false;
            buf_size -= (size_t)गणनम्;
            start += गणनम्;
        }
    }
    सचे (buf_size == 0)
        पटिनिवत्त false;
    *start = '\0';
    पटिनिवत्त true;
}
