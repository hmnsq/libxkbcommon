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
        (flags & ~(गणना xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_मूल्यावली);
    यदि (invalid_flags) {
        log_err_func(context, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        प्रत्यागम रिक्त;
    }

    संरचना xkb_rmlvo_builder * ध्रुव builder = सुस्मृति(1, परिमाण(*builder));
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

    प्रत्यागम builder;

error:
    log_err(context, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    प्रत्यागम रिक्त;
}

स्थिर गणना xkb_status
rmlvo_builder_append_option(संरचना xkb_rmlvo_builder *rmlvo,
                            ध्रुव वर्ण *option, xkb_layout_mask_t layouts)
{
    यदि (isempty(option))
        प्रत्यागम XKB_ERROR_INVALID;

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
            प्रत्यागम XKB_SUCCESS;
        }
    }

    /* Append new entry */
    ध्रुव संरचना xkb_rmlvo_builder_option new = {
        .option = strdup_safe(option),
        .layouts = layouts
    };
    यदि (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        प्रत्यागम XKB_ERROR_ALLOCATION_FAILURE;
    }
    darray_append(rmlvo->options, new);
    प्रत्यागम XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(संरचना xkb_rmlvo_builder *rmlvo,
                                ध्रुव वर्ण *layout, ध्रुव वर्ण *variant,
                                ध्रुव वर्ण* ध्रुव* options, माप_प्रकार options_len)
{
    ध्रुव xkb_layout_index_t idx = (xkb_layout_index_t)
                                   darray_size(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    यदि (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        प्रत्यागम false;
    }

    /* Append layout entry */
    ध्रुव संरचना xkb_rmlvo_builder_layout new = {
        .layout = strdup_safe(layout),
        .variant = strdup_safe(variant)
    };

    यदि (!new.layout || (!new.variant && variant)) {
        मुक्त(new.layout);
        मुक्त(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        प्रत्यागम false;
    }

    darray_append(rmlvo->layouts, new);

    यदि (!options)
        options_len = 0;

    ध्रुव xkb_layout_mask_t layout_mask = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    प्रति (माप_प्रकार k = 0; k < options_len; k++) {
        ध्रुव गणना xkb_status status =
            rmlvo_builder_append_option(rmlvo, options[k], layout_mask);

        यदि (status == XKB_ERROR_ALLOCATION_FAILURE)
            प्रत्यागम false;
    }

    प्रत्यागम true;
}

bool
xkb_rmlvo_builder_append_option(संरचना xkb_rmlvo_builder *rmlvo,
                                ध्रुव वर्ण *option)
{
    ध्रुव गणना xkb_status status = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL
    );
    प्रत्यागम (status == XKB_SUCCESS);
}

संरचना xkb_rmlvo_builder *
xkb_rmlvo_builder_ref(संरचना xkb_rmlvo_builder *rmlvo)
{
    प्रतिज्ञा(rmlvo->refcnt > 0);
    rmlvo->refcnt++;
    प्रत्यागम rmlvo;
}

शून्य
xkb_rmlvo_builder_unref(संरचना xkb_rmlvo_builder *rmlvo)
{
    प्रतिज्ञा(!rmlvo || rmlvo->refcnt > 0);
    यदि (!rmlvo || --rmlvo->refcnt > 0)
        प्रत्यागम;

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
                                 वर्ण *buf, माप_प्रकार buf_size)
{
    rmlvo->rules = builder->rules;
    rmlvo->model = builder->model;

    वर्ण *start = buf;
    rmlvo->layout = start;
    darray_size_t k;
    ध्रुव संरचना xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        पूर्णाङ्क गिनती = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        यदि (गिनती < 0 || (माप_प्रकार)गिनती >= buf_size)
            प्रत्यागम false;
        buf_size -= (माप_प्रकार)गिनती;
        start += गिनती;
    }
    यदि (buf_size <= 1)
        प्रत्यागम false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->variant = start;
    darray_enumerate(k, layout, builder->layouts) {
        पूर्णाङ्क गिनती = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        यदि (गिनती < 0 || (माप_प्रकार) गिनती >= buf_size)
            प्रत्यागम false;
        buf_size -= (माप_प्रकार)गिनती;
        start += गिनती;
    }
    यदि (buf_size <= 1)
        प्रत्यागम false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->options = start;
    ध्रुव संरचना xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        पूर्णाङ्क गिनती = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        यदि (गिनती < 0 || (माप_प्रकार) गिनती >= buf_size)
            प्रत्यागम false;
        buf_size -= (माप_प्रकार)गिनती;
        start += गिनती;
        यदि (option->layouts != XKB_LAYOUT_INVALID) {
            गिनती = snprintf(start, buf_size, "%c%"PRIu32,
                             OPTIONS_GROUP_SPECIFIER_PREFIX, option->layouts);
            यदि (गिनती < 0 || (माप_प्रकार) गिनती >= buf_size)
                प्रत्यागम false;
            buf_size -= (माप_प्रकार)गिनती;
            start += गिनती;
        }
    }
    यदि (buf_size == 0)
        प्रत्यागम false;
    *start = '\0';
    प्रत्यागम true;
}
