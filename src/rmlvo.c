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
                      ध्रुव वर्ण *नियमाः, ध्रुव वर्ण *model,
                      गणना xkb_rmlvo_builder_flags flags)
{
    ध्रुव गणना xkb_rmlvo_builder_flags invalid_flags =
        (flags & ~(गणना xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_मानानि);
    यदि (invalid_flags) {
        log_err_func(context, XKB_LOG_सन्देशः_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        प्रत्यागम रिक्त;
    }

    संरचना xkb_rmlvo_builder * ध्रुव builder = शून्यस्मृतिं_देहि(1, परिमाण(*builder));
    यदि (!builder)
        गच्छ error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->नियमाः = strdup_safe(नियमाः);
    यदि (!builder->नियमाः && नियमाः)
        गच्छ error;

    builder->model = strdup_safe(model);
    यदि (!builder->model && model)
        गच्छ error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    प्रत्यागम builder;

error:
    log_err(context, XKB_ERROR_आवण्टनम्_FAILURE_2,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    प्रत्यागम रिक्त;
}

स्थिर गणना xkb_स्थितिः
rmlvo_builder_append_option(संरचना xkb_rmlvo_builder *rmlvo,
                            ध्रुव वर्ण *option, xkb_layout_आवरणम्_t layouts)
{
    यदि (isempty(option))
        प्रत्यागम XKB_ERROR_INVALID;

    /* Check for previous entry */
    संरचना xkb_rmlvo_builder_option *पूर्व;
    darray_foreach(पूर्व, rmlvo->options) {
        यदि (streq(पूर्व->option, option)) {
            /* Merge with previous entry */
            यदि (layouts == (xkb_layout_आवरणम्_t)XKB_OPTION_LAYOUT_आवरणम्_GLOBAL) {
                /* Global option overrides layout-specific ones */
                पूर्व->layouts = layouts;
            } अन्यथा यदि (पूर्व->layouts != (xkb_layout_आवरणम्_t)
                       XKB_OPTION_LAYOUT_आवरणम्_GLOBAL) {
                /* Layout-specific option cannot override global */
                पूर्व->layouts |= layouts;
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
        log_err(rmlvo->ctx, XKB_ERROR_आवण्टनम्_FAILURE_2,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        प्रत्यागम XKB_ERROR_आवण्टनम्_FAILURE;
    }
    darray_append(rmlvo->options, new);
    प्रत्यागम XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(संरचना xkb_rmlvo_builder *rmlvo,
                                ध्रुव वर्ण *layout, ध्रुव वर्ण *variant,
                                ध्रुव वर्ण* ध्रुव* options, आकार_t options_len)
{
    ध्रुव xkb_layout_सूचकांक_t idx = (xkb_layout_सूचकांक_t)
                                   darray_आकार(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    यदि (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
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
        log_err(rmlvo->ctx, XKB_ERROR_आवण्टनम्_FAILURE_2,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        प्रत्यागम false;
    }

    darray_append(rmlvo->layouts, new);

    यदि (!options)
        options_len = 0;

    ध्रुव xkb_layout_आवरणम्_t layout_आवरणम् = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    प्रति (आकार_t k = 0; k < options_len; k++) {
        ध्रुव गणना xkb_स्थितिः स्थितिः =
            rmlvo_builder_append_option(rmlvo, options[k], layout_आवरणम्);

        यदि (स्थितिः == XKB_ERROR_आवण्टनम्_FAILURE)
            प्रत्यागम false;
    }

    प्रत्यागम true;
}

bool
xkb_rmlvo_builder_append_option(संरचना xkb_rmlvo_builder *rmlvo,
                                ध्रुव वर्ण *option)
{
    ध्रुव गणना xkb_स्थितिः स्थितिः = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_आवरणम्_t)XKB_OPTION_LAYOUT_आवरणम्_GLOBAL
    );
    प्रत्यागम (स्थितिः == XKB_SUCCESS);
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

    मुक्त(rmlvo->नियमाः);
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
xkb_rmlvo_builder_गन्तव्यम्_नियमाः_नामानि(ध्रुव संरचना xkb_rmlvo_builder *builder,
                                 संरचना xkb_नियमः_नामानि *rmlvo,
                                 वर्ण *buf, आकार_t buf_आकार)
{
    rmlvo->नियमाः = builder->नियमाः;
    rmlvo->model = builder->model;

    वर्ण *start = buf;
    rmlvo->layout = start;
    darray_आकार_t k;
    ध्रुव संरचना xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        पूर्णाङ्क गणनम् = snprintf(start, buf_आकार, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        यदि (गणनम् < 0 || (आकार_t)गणनम् >= buf_आकार)
            प्रत्यागम false;
        buf_आकार -= (आकार_t)गणनम्;
        start += गणनम्;
    }
    यदि (buf_आकार <= 1)
        प्रत्यागम false;
    *start = '\0';
    start++;
    buf_आकार--;

    rmlvo->variant = start;
    darray_enumerate(k, layout, builder->layouts) {
        पूर्णाङ्क गणनम् = snprintf(start, buf_आकार, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        यदि (गणनम् < 0 || (आकार_t) गणनम् >= buf_आकार)
            प्रत्यागम false;
        buf_आकार -= (आकार_t)गणनम्;
        start += गणनम्;
    }
    यदि (buf_आकार <= 1)
        प्रत्यागम false;
    *start = '\0';
    start++;
    buf_आकार--;

    rmlvo->options = start;
    ध्रुव संरचना xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        पूर्णाङ्क गणनम् = snprintf(start, buf_आकार, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        यदि (गणनम् < 0 || (आकार_t) गणनम् >= buf_आकार)
            प्रत्यागम false;
        buf_आकार -= (आकार_t)गणनम्;
        start += गणनम्;
        यदि (option->layouts != XKB_LAYOUT_INVALID) {
            गणनम् = snprintf(start, buf_आकार, "%c%"PRIu32,
                             OPTIONS_गणः_SPECIFIER_उपसर्गः, option->layouts);
            यदि (गणनम् < 0 || (आकार_t) गणनम् >= buf_आकार)
                प्रत्यागम false;
            buf_आकार -= (आकार_t)गणनम्;
            start += गणनम्;
        }
    }
    यदि (buf_आकार == 0)
        प्रत्यागम false;
    *start = '\0';
    प्रत्यागम true;
}
