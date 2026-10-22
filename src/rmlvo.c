/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <string.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-status.h"
#ಅನ್ತರ್ಭಾವ "context.h"
#ಅನ್ತರ್ಭಾವ "features/enums.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "rmlvo.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp/rules.h"

ಸಂರಚನಾ xkb_rmlvo_builder*
xkb_rmlvo_builder_new(ಸಂರಚನಾ xkb_context *context,
                      ಧ್ರುವ ವರ್ಣ *rules, ಧ್ರುವ ವರ್ಣ *model,
                      ಗಣನಾ xkb_rmlvo_builder_flags flags)
{
    ಧ್ರುವ ಗಣನಾ xkb_rmlvo_builder_flags invalid_flags =
        (flags & ~(ಗಣನಾ xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_ಮೌಲ್ಯಗಳು);
    ಯದಿ (invalid_flags) {
        log_err_func(context, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಸಂರಚನಾ xkb_rmlvo_builder * ಧ್ರುವ builder = calloc(1, ಪರಿಮಾಣ(*builder));
    ಯದಿ (!builder)
        ಗಚ್ಛ error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->rules = strdup_safe(rules);
    ಯದಿ (!builder->rules && rules)
        ಗಚ್ಛ error;

    builder->model = strdup_safe(model);
    ಯದಿ (!builder->model && model)
        ಗಚ್ಛ error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    ಪ್ರತ್ಯಾಗಮ builder;

error:
    log_err(context, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಗಣನಾ xkb_status
rmlvo_builder_append_option(ಸಂರಚನಾ xkb_rmlvo_builder *rmlvo,
                            ಧ್ರುವ ವರ್ಣ *option, xkb_layout_mask_t layouts)
{
    ಯದಿ (isempty(option))
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_INVALID;

    /* Check for previous entry */
    ಸಂರಚನಾ xkb_rmlvo_builder_option *prev;
    darray_foreach(prev, rmlvo->options) {
        ಯದಿ (streq(prev->option, option)) {
            /* Merge with previous entry */
            ಯದಿ (layouts == (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Global option overrides layout-specific ones */
                prev->layouts = layouts;
            } ಅನ್ಯಥಾ ಯದಿ (prev->layouts != (xkb_layout_mask_t)
                       XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Layout-specific option cannot override global */
                prev->layouts |= layouts;
            }
            ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
        }
    }

    /* Append new entry */
    ಧ್ರುವ ಸಂರಚನಾ xkb_rmlvo_builder_option new = {
        .option = strdup_safe(option),
        .layouts = layouts
    };
    ಯದಿ (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_ALLOCATION_FAILURE;
    }
    darray_append(rmlvo->options, new);
    ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(ಸಂರಚನಾ xkb_rmlvo_builder *rmlvo,
                                ಧ್ರುವ ವರ್ಣ *layout, ಧ್ರುವ ವರ್ಣ *variant,
                                ಧ್ರುವ ವರ್ಣ* ಧ್ರುವ* options, size_t options_len)
{
    ಧ್ರುವ xkb_layout_index_t idx = (xkb_layout_index_t)
                                   darray_size(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    ಯದಿ (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        ಪ್ರತ್ಯಾಗಮ false;
    }

    /* Append layout entry */
    ಧ್ರುವ ಸಂರಚನಾ xkb_rmlvo_builder_layout new = {
        .layout = strdup_safe(layout),
        .variant = strdup_safe(variant)
    };

    ಯದಿ (!new.layout || (!new.variant && variant)) {
        ಮುಕ್ತ(new.layout);
        ಮುಕ್ತ(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        ಪ್ರತ್ಯಾಗಮ false;
    }

    darray_append(rmlvo->layouts, new);

    ಯದಿ (!options)
        options_len = 0;

    ಧ್ರುವ xkb_layout_mask_t layout_mask = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    ಪ್ರತಿ (size_t k = 0; k < options_len; k++) {
        ಧ್ರುವ ಗಣನಾ xkb_status status =
            rmlvo_builder_append_option(rmlvo, options[k], layout_mask);

        ಯದಿ (status == XKB_ERROR_ALLOCATION_FAILURE)
            ಪ್ರತ್ಯಾಗಮ false;
    }

    ಪ್ರತ್ಯಾಗಮ true;
}

bool
xkb_rmlvo_builder_append_option(ಸಂರಚನಾ xkb_rmlvo_builder *rmlvo,
                                ಧ್ರುವ ವರ್ಣ *option)
{
    ಧ್ರುವ ಗಣನಾ xkb_status status = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL
    );
    ಪ್ರತ್ಯಾಗಮ (status == XKB_SUCCESS);
}

ಸಂರಚನಾ xkb_rmlvo_builder *
xkb_rmlvo_builder_ref(ಸಂರಚನಾ xkb_rmlvo_builder *rmlvo)
{
    ಪ್ರತಿಜ್ಞಾ(rmlvo->refcnt > 0);
    rmlvo->refcnt++;
    ಪ್ರತ್ಯಾಗಮ rmlvo;
}

ಶೂನ್ಯ
xkb_rmlvo_builder_unref(ಸಂರಚನಾ xkb_rmlvo_builder *rmlvo)
{
    ಪ್ರತಿಜ್ಞಾ(!rmlvo || rmlvo->refcnt > 0);
    ಯದಿ (!rmlvo || --rmlvo->refcnt > 0)
        ಪ್ರತ್ಯಾಗಮ;

    ಮುಕ್ತ(rmlvo->rules);
    ಮುಕ್ತ(rmlvo->model);

    ಧ್ರುವ ಸಂರಚನಾ xkb_rmlvo_builder_layout *layout;
    darray_foreach(layout, rmlvo->layouts) {
        ಮುಕ್ತ(layout->layout);
        ಮುಕ್ತ(layout->variant);
    }
    darray_free(rmlvo->layouts);

    ಧ್ರುವ ಸಂರಚನಾ xkb_rmlvo_builder_option *option;
    darray_foreach(option, rmlvo->options)
        ಮುಕ್ತ(option->option);
    darray_free(rmlvo->options);

    xkb_context_unref(rmlvo->ctx);
    ಮುಕ್ತ(rmlvo);
}

/* NOTE: the converse function, `xkb_rules_names_to_rmlvo_builder`, is currently
 * only used in tests */
bool
xkb_rmlvo_builder_to_rules_names(ಧ್ರುವ ಸಂರಚನಾ xkb_rmlvo_builder *builder,
                                 ಸಂರಚನಾ xkb_rule_names *rmlvo,
                                 ವರ್ಣ *buf, size_t buf_size)
{
    rmlvo->rules = builder->rules;
    rmlvo->model = builder->model;

    ವರ್ಣ *start = buf;
    rmlvo->layout = start;
    darray_size_t k;
    ಧ್ರುವ ಸಂರಚನಾ xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        ಯದಿ (ಎಣಿಕೆ < 0 || (size_t)ಎಣಿಕೆ >= buf_size)
            ಪ್ರತ್ಯಾಗಮ false;
        buf_size -= (size_t)ಎಣಿಕೆ;
        start += ಎಣಿಕೆ;
    }
    ಯದಿ (buf_size <= 1)
        ಪ್ರತ್ಯಾಗಮ false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->variant = start;
    darray_enumerate(k, layout, builder->layouts) {
        ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        ಯದಿ (ಎಣಿಕೆ < 0 || (size_t) ಎಣಿಕೆ >= buf_size)
            ಪ್ರತ್ಯಾಗಮ false;
        buf_size -= (size_t)ಎಣಿಕೆ;
        start += ಎಣಿಕೆ;
    }
    ಯದಿ (buf_size <= 1)
        ಪ್ರತ್ಯಾಗಮ false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->options = start;
    ಧ್ರುವ ಸಂರಚನಾ xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        ಯದಿ (ಎಣಿಕೆ < 0 || (size_t) ಎಣಿಕೆ >= buf_size)
            ಪ್ರತ್ಯಾಗಮ false;
        buf_size -= (size_t)ಎಣಿಕೆ;
        start += ಎಣಿಕೆ;
        ಯದಿ (option->layouts != XKB_LAYOUT_INVALID) {
            ಎಣಿಕೆ = snprintf(start, buf_size, "%c%"PRIu32,
                             OPTIONS_GROUP_SPECIFIER_PREFIX, option->layouts);
            ಯದಿ (ಎಣಿಕೆ < 0 || (size_t) ಎಣಿಕೆ >= buf_size)
                ಪ್ರತ್ಯಾಗಮ false;
            buf_size -= (size_t)ಎಣಿಕೆ;
            start += ಎಣಿಕೆ;
        }
    }
    ಯದಿ (buf_size == 0)
        ಪ್ರತ್ಯಾಗಮ false;
    *start = '\0';
    ಪ್ರತ್ಯಾಗಮ true;
}
