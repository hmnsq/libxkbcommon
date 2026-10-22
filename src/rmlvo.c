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
                      अचर वर्ण *rules, अचर वर्ण *model,
                      गणना xkb_rmlvo_builder_flags flags)
{
    अचर गणना xkb_rmlvo_builder_flags invalid_flags =
        (flags & ~(गणना xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_मानहरू);
    यदि (invalid_flags) {
        log_err_func(context, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        फर्काऊ रिक्त;
    }

    संरचना xkb_rmlvo_builder * अचर builder = सुस्मृति(1, परिमाण(*builder));
    यदि (!builder)
        गच्छ error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->rules = strdup_safe(rules);
    यदि (!builder->rules && rules)
        गच्छ error;

    builder->model = strdup_safe(model);
    यदि (!builder->model && model)
        गच्छ error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    फर्काऊ builder;

error:
    log_err(context, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    फर्काऊ रिक्त;
}

स्थिर गणना xkb_status
rmlvo_builder_append_option(संरचना xkb_rmlvo_builder *rmlvo,
                            अचर वर्ण *option, xkb_layout_mask_t layouts)
{
    यदि (isempty(option))
        फर्काऊ XKB_ERROR_INVALID;

    /* Check for previous entry */
    संरचना xkb_rmlvo_builder_option *prev;
    darray_foreach(prev, rmlvo->options) {
        यदि (streq(prev->option, option)) {
            /* Merge with previous entry */
            यदि (layouts == (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Global option overrides layout-specific ones */
                prev->layouts = layouts;
            } अन्यथा यदि (prev->layouts != (xkb_layout_mask_t)
                       XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Layout-specific option cannot override global */
                prev->layouts |= layouts;
            }
            फर्काऊ XKB_SUCCESS;
        }
    }

    /* Append new entry */
    अचर संरचना xkb_rmlvo_builder_option new = {
        .option = strdup_safe(option),
        .layouts = layouts
    };
    यदि (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        फर्काऊ XKB_ERROR_ALLOCATION_FAILURE;
    }
    darray_append(rmlvo->options, new);
    फर्काऊ XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(संरचना xkb_rmlvo_builder *rmlvo,
                                अचर वर्ण *layout, अचर वर्ण *variant,
                                अचर वर्ण* अचर* options, माप_प्रकार options_len)
{
    अचर xkb_layout_index_t idx = (xkb_layout_index_t)
                                   darray_size(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    यदि (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        फर्काऊ false;
    }

    /* Append layout entry */
    अचर संरचना xkb_rmlvo_builder_layout new = {
        .layout = strdup_safe(layout),
        .variant = strdup_safe(variant)
    };

    यदि (!new.layout || (!new.variant && variant)) {
        मुक्त(new.layout);
        मुक्त(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        फर्काऊ false;
    }

    darray_append(rmlvo->layouts, new);

    यदि (!options)
        options_len = 0;

    अचर xkb_layout_mask_t layout_mask = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    लागि (माप_प्रकार k = 0; k < options_len; k++) {
        अचर गणना xkb_status status =
            rmlvo_builder_append_option(rmlvo, options[k], layout_mask);

        यदि (status == XKB_ERROR_ALLOCATION_FAILURE)
            फर्काऊ false;
    }

    फर्काऊ true;
}

bool
xkb_rmlvo_builder_append_option(संरचना xkb_rmlvo_builder *rmlvo,
                                अचर वर्ण *option)
{
    अचर गणना xkb_status status = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL
    );
    फर्काऊ (status == XKB_SUCCESS);
}

संरचना xkb_rmlvo_builder *
xkb_rmlvo_builder_ref(संरचना xkb_rmlvo_builder *rmlvo)
{
    प्रतिज्ञा(rmlvo->refcnt > 0);
    rmlvo->refcnt++;
    फर्काऊ rmlvo;
}

शून्य
xkb_rmlvo_builder_unref(संरचना xkb_rmlvo_builder *rmlvo)
{
    प्रतिज्ञा(!rmlvo || rmlvo->refcnt > 0);
    यदि (!rmlvo || --rmlvo->refcnt > 0)
        फर्काऊ;

    मुक्त(rmlvo->rules);
    मुक्त(rmlvo->model);

    अचर संरचना xkb_rmlvo_builder_layout *layout;
    darray_foreach(layout, rmlvo->layouts) {
        मुक्त(layout->layout);
        मुक्त(layout->variant);
    }
    darray_free(rmlvo->layouts);

    अचर संरचना xkb_rmlvo_builder_option *option;
    darray_foreach(option, rmlvo->options)
        मुक्त(option->option);
    darray_free(rmlvo->options);

    xkb_context_unref(rmlvo->ctx);
    मुक्त(rmlvo);
}

/* NOTE: the converse function, `xkb_rules_names_to_rmlvo_builder`, is currently
 * only used in tests */
bool
xkb_rmlvo_builder_to_rules_names(अचर संरचना xkb_rmlvo_builder *builder,
                                 संरचना xkb_rule_names *rmlvo,
                                 वर्ण *buf, माप_प्रकार buf_size)
{
    rmlvo->rules = builder->rules;
    rmlvo->model = builder->model;

    वर्ण *start = buf;
    rmlvo->layout = start;
    darray_size_t k;
    अचर संरचना xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        पूर्णाङ्क गन्ती = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        यदि (गन्ती < 0 || (माप_प्रकार)गन्ती >= buf_size)
            फर्काऊ false;
        buf_size -= (माप_प्रकार)गन्ती;
        start += गन्ती;
    }
    यदि (buf_size <= 1)
        फर्काऊ false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->variant = start;
    darray_enumerate(k, layout, builder->layouts) {
        पूर्णाङ्क गन्ती = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        यदि (गन्ती < 0 || (माप_प्रकार) गन्ती >= buf_size)
            फर्काऊ false;
        buf_size -= (माप_प्रकार)गन्ती;
        start += गन्ती;
    }
    यदि (buf_size <= 1)
        फर्काऊ false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->options = start;
    अचर संरचना xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        पूर्णाङ्क गन्ती = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        यदि (गन्ती < 0 || (माप_प्रकार) गन्ती >= buf_size)
            फर्काऊ false;
        buf_size -= (माप_प्रकार)गन्ती;
        start += गन्ती;
        यदि (option->layouts != XKB_LAYOUT_INVALID) {
            गन्ती = snprintf(start, buf_size, "%c%"PRIu32,
                             OPTIONS_GROUP_SPECIFIER_PREFIX, option->layouts);
            यदि (गन्ती < 0 || (माप_प्रकार) गन्ती >= buf_size)
                फर्काऊ false;
            buf_size -= (माप_प्रकार)गन्ती;
            start += गन्ती;
        }
    }
    यदि (buf_size == 0)
        फर्काऊ false;
    *start = '\0';
    फर्काऊ true;
}
