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
                      स्थिरांक अक्षर *rules, स्थिरांक अक्षर *model,
                      गणना xkb_rmlvo_builder_flags flags)
{
    स्थिरांक गणना xkb_rmlvo_builder_flags invalid_flags =
        (flags & ~(गणना xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_मूल्ये);
    जर (invalid_flags) {
        log_err_func(context, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        परत रिक्त;
    }

    संरचना xkb_rmlvo_builder * स्थिरांक builder = सुस्मृति(1, आकार(*builder));
    जर (!builder)
        जा error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->rules = strdup_safe(rules);
    जर (!builder->rules && rules)
        जा error;

    builder->model = strdup_safe(model);
    जर (!builder->model && model)
        जा error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    परत builder;

error:
    log_err(context, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    परत रिक्त;
}

स्थिर गणना xkb_status
rmlvo_builder_append_option(संरचना xkb_rmlvo_builder *rmlvo,
                            स्थिरांक अक्षर *option, xkb_layout_mask_t layouts)
{
    जर (isempty(option))
        परत XKB_ERROR_INVALID;

    /* Check for previous entry */
    संरचना xkb_rmlvo_builder_option *prev;
    darray_foreach(prev, rmlvo->options) {
        जर (streq(prev->option, option)) {
            /* Merge with previous entry */
            जर (layouts == (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Global option overrides layout-specific ones */
                prev->layouts = layouts;
            } अन्यथा जर (prev->layouts != (xkb_layout_mask_t)
                       XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Layout-specific option cannot override global */
                prev->layouts |= layouts;
            }
            परत XKB_SUCCESS;
        }
    }

    /* Append new entry */
    स्थिरांक संरचना xkb_rmlvo_builder_option new = {
        .option = strdup_safe(option),
        .layouts = layouts
    };
    जर (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        परत XKB_ERROR_ALLOCATION_FAILURE;
    }
    darray_append(rmlvo->options, new);
    परत XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(संरचना xkb_rmlvo_builder *rmlvo,
                                स्थिरांक अक्षर *layout, स्थिरांक अक्षर *variant,
                                स्थिरांक अक्षर* स्थिरांक* options, माप_प्रकार options_len)
{
    स्थिरांक xkb_layout_index_t idx = (xkb_layout_index_t)
                                   darray_size(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    जर (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        परत false;
    }

    /* Append layout entry */
    स्थिरांक संरचना xkb_rmlvo_builder_layout new = {
        .layout = strdup_safe(layout),
        .variant = strdup_safe(variant)
    };

    जर (!new.layout || (!new.variant && variant)) {
        मुक्त(new.layout);
        मुक्त(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        परत false;
    }

    darray_append(rmlvo->layouts, new);

    जर (!options)
        options_len = 0;

    स्थिरांक xkb_layout_mask_t layout_mask = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    साठी (माप_प्रकार k = 0; k < options_len; k++) {
        स्थिरांक गणना xkb_status status =
            rmlvo_builder_append_option(rmlvo, options[k], layout_mask);

        जर (status == XKB_ERROR_ALLOCATION_FAILURE)
            परत false;
    }

    परत true;
}

bool
xkb_rmlvo_builder_append_option(संरचना xkb_rmlvo_builder *rmlvo,
                                स्थिरांक अक्षर *option)
{
    स्थिरांक गणना xkb_status status = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL
    );
    परत (status == XKB_SUCCESS);
}

संरचना xkb_rmlvo_builder *
xkb_rmlvo_builder_ref(संरचना xkb_rmlvo_builder *rmlvo)
{
    प्रतिज्ञा(rmlvo->refcnt > 0);
    rmlvo->refcnt++;
    परत rmlvo;
}

शून्य
xkb_rmlvo_builder_unref(संरचना xkb_rmlvo_builder *rmlvo)
{
    प्रतिज्ञा(!rmlvo || rmlvo->refcnt > 0);
    जर (!rmlvo || --rmlvo->refcnt > 0)
        परत;

    मुक्त(rmlvo->rules);
    मुक्त(rmlvo->model);

    स्थिरांक संरचना xkb_rmlvo_builder_layout *layout;
    darray_foreach(layout, rmlvo->layouts) {
        मुक्त(layout->layout);
        मुक्त(layout->variant);
    }
    darray_free(rmlvo->layouts);

    स्थिरांक संरचना xkb_rmlvo_builder_option *option;
    darray_foreach(option, rmlvo->options)
        मुक्त(option->option);
    darray_free(rmlvo->options);

    xkb_context_unref(rmlvo->ctx);
    मुक्त(rmlvo);
}

/* NOTE: the converse function, `xkb_rules_names_to_rmlvo_builder`, is currently
 * only used in tests */
bool
xkb_rmlvo_builder_to_rules_names(स्थिरांक संरचना xkb_rmlvo_builder *builder,
                                 संरचना xkb_rule_names *rmlvo,
                                 अक्षर *buf, माप_प्रकार buf_size)
{
    rmlvo->rules = builder->rules;
    rmlvo->model = builder->model;

    अक्षर *start = buf;
    rmlvo->layout = start;
    darray_size_t k;
    स्थिरांक संरचना xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        पूर्णांक मोजणी = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        जर (मोजणी < 0 || (माप_प्रकार)मोजणी >= buf_size)
            परत false;
        buf_size -= (माप_प्रकार)मोजणी;
        start += मोजणी;
    }
    जर (buf_size <= 1)
        परत false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->variant = start;
    darray_enumerate(k, layout, builder->layouts) {
        पूर्णांक मोजणी = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        जर (मोजणी < 0 || (माप_प्रकार) मोजणी >= buf_size)
            परत false;
        buf_size -= (माप_प्रकार)मोजणी;
        start += मोजणी;
    }
    जर (buf_size <= 1)
        परत false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->options = start;
    स्थिरांक संरचना xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        पूर्णांक मोजणी = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        जर (मोजणी < 0 || (माप_प्रकार) मोजणी >= buf_size)
            परत false;
        buf_size -= (माप_प्रकार)मोजणी;
        start += मोजणी;
        जर (option->layouts != XKB_LAYOUT_INVALID) {
            मोजणी = snprintf(start, buf_size, "%c%"PRIu32,
                             OPTIONS_GROUP_SPECIFIER_PREFIX, option->layouts);
            जर (मोजणी < 0 || (माप_प्रकार) मोजणी >= buf_size)
                परत false;
            buf_size -= (माप_प्रकार)मोजणी;
            start += मोजणी;
        }
    }
    जर (buf_size == 0)
        परत false;
    *start = '\0';
    परत true;
}
