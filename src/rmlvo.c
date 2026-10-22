/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#include "config.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "xkbcommon/xkbcommon.h"
#include "xkbcommon/xkbcommon-status.h"
#include "context.h"
#include "features/enums.h"
#include "keymap.h"
#include "messages-codes.h"
#include "rmlvo.h"
#include "darray.h"
#include "utils.h"
#include "xkbcomp/rules.h"

sa_nracanaa xkb_rmlvo_builder*
xkb_rmlvo_builder_new(sa_nracanaa xkb_context *context,
                      const var_na *rules, const var_na *model,
                      ga_nanaa xkb_rmlvo_builder_flags flags)
{
    const ga_nanaa xkb_rmlvo_builder_flags invalid_flags =
        (flags & ~(ga_nanaa xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_VALUES);
    ᯀᯮᯝ (invalid_flags) {
        log_err_func(context, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        ᯀᯮᯞ NULL;
    }

    sa_nracanaa xkb_rmlvo_builder * const builder = calloc(1, parimaa_na(*builder));
    ᯀᯮᯝ (!builder)
        goto error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->rules = strdup_safe(rules);
    ᯀᯮᯝ (!builder->rules && rules)
        goto error;

    builder->model = strdup_safe(model);
    ᯀᯮᯝ (!builder->model && model)
        goto error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    ᯀᯮᯞ builder;

error:
    log_err(context, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    ᯀᯮᯞ NULL;
}

static ga_nanaa xkb_status
rmlvo_builder_append_option(sa_nracanaa xkb_rmlvo_builder *rmlvo,
                            const var_na *option, xkb_layout_mask_t layouts)
{
    ᯀᯮᯝ (isempty(option))
        ᯀᯮᯞ XKB_ERROR_INVALID;

    /* Check for previous entry */
    sa_nracanaa xkb_rmlvo_builder_option *prev;
    darray_foreach(prev, rmlvo->options) {
        ᯀᯮᯝ (streq(prev->option, option)) {
            /* Merge with previous entry */
            ᯀᯮᯝ (layouts == (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Global option overrides layout-specific ones */
                prev->layouts = layouts;
            } ᯀᯮᯝᯂᯪ ᯀᯮᯝ (prev->layouts != (xkb_layout_mask_t)
                       XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Layout-specific option cannot override global */
                prev->layouts |= layouts;
            }
            ᯀᯮᯞ XKB_SUCCESS;
        }
    }

    /* Append new entry */
    const sa_nracanaa xkb_rmlvo_builder_option new = {
        .option = strdup_safe(option),
        .layouts = layouts
    };
    ᯀᯮᯝ (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        ᯀᯮᯞ XKB_ERROR_ALLOCATION_FAILURE;
    }
    darray_append(rmlvo->options, new);
    ᯀᯮᯞ XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(sa_nracanaa xkb_rmlvo_builder *rmlvo,
                                const var_na *layout, const var_na *variant,
                                const var_na* const* options, size_t options_len)
{
    const xkb_layout_index_t idx = (xkb_layout_index_t)
                                   darray_size(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    ᯀᯮᯝ (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        ᯀᯮᯞ false;
    }

    /* Append layout entry */
    const sa_nracanaa xkb_rmlvo_builder_layout new = {
        .layout = strdup_safe(layout),
        .variant = strdup_safe(variant)
    };

    ᯀᯮᯝ (!new.layout || (!new.variant && variant)) {
        free(new.layout);
        free(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        ᯀᯮᯞ false;
    }

    darray_append(rmlvo->layouts, new);

    ᯀᯮᯝ (!options)
        options_len = 0;

    const xkb_layout_mask_t layout_mask = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    for (size_t k = 0; k < options_len; k++) {
        const ga_nanaa xkb_status status =
            rmlvo_builder_append_option(rmlvo, options[k], layout_mask);

        ᯀᯮᯝ (status == XKB_ERROR_ALLOCATION_FAILURE)
            ᯀᯮᯞ false;
    }

    ᯀᯮᯞ true;
}

bool
xkb_rmlvo_builder_append_option(sa_nracanaa xkb_rmlvo_builder *rmlvo,
                                const var_na *option)
{
    const ga_nanaa xkb_status status = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL
    );
    ᯀᯮᯞ (status == XKB_SUCCESS);
}

sa_nracanaa xkb_rmlvo_builder *
xkb_rmlvo_builder_ref(sa_nracanaa xkb_rmlvo_builder *rmlvo)
{
    assert(rmlvo->refcnt > 0);
    rmlvo->refcnt++;
    ᯀᯮᯞ rmlvo;
}

void
xkb_rmlvo_builder_unref(sa_nracanaa xkb_rmlvo_builder *rmlvo)
{
    assert(!rmlvo || rmlvo->refcnt > 0);
    ᯀᯮᯝ (!rmlvo || --rmlvo->refcnt > 0)
        ᯀᯮᯞ;

    free(rmlvo->rules);
    free(rmlvo->model);

    const sa_nracanaa xkb_rmlvo_builder_layout *layout;
    darray_foreach(layout, rmlvo->layouts) {
        free(layout->layout);
        free(layout->variant);
    }
    darray_free(rmlvo->layouts);

    const sa_nracanaa xkb_rmlvo_builder_option *option;
    darray_foreach(option, rmlvo->options)
        free(option->option);
    darray_free(rmlvo->options);

    xkb_context_unref(rmlvo->ctx);
    free(rmlvo);
}

/* NOTE: the converse function, `xkb_rules_names_to_rmlvo_builder`, is currently
 * only used in tests */
bool
xkb_rmlvo_builder_to_rules_names(const sa_nracanaa xkb_rmlvo_builder *builder,
                                 sa_nracanaa xkb_rule_names *rmlvo,
                                 var_na *buf, size_t buf_size)
{
    rmlvo->rules = builder->rules;
    rmlvo->model = builder->model;

    var_na *start = buf;
    rmlvo->layout = start;
    darray_size_t k;
    const sa_nracanaa xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        puur_naa_nka count = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        ᯀᯮᯝ (count < 0 || (size_t)count >= buf_size)
            ᯀᯮᯞ false;
        buf_size -= (size_t)count;
        start += count;
    }
    ᯀᯮᯝ (buf_size <= 1)
        ᯀᯮᯞ false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->variant = start;
    darray_enumerate(k, layout, builder->layouts) {
        puur_naa_nka count = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        ᯀᯮᯝ (count < 0 || (size_t) count >= buf_size)
            ᯀᯮᯞ false;
        buf_size -= (size_t)count;
        start += count;
    }
    ᯀᯮᯝ (buf_size <= 1)
        ᯀᯮᯞ false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->options = start;
    const sa_nracanaa xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        puur_naa_nka count = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        ᯀᯮᯝ (count < 0 || (size_t) count >= buf_size)
            ᯀᯮᯞ false;
        buf_size -= (size_t)count;
        start += count;
        ᯀᯮᯝ (option->layouts != XKB_LAYOUT_INVALID) {
            count = snprintf(start, buf_size, "%c%"PRIu32,
                             OPTIONS_GROUP_SPECIFIER_PREFIX, option->layouts);
            ᯀᯮᯝ (count < 0 || (size_t) count >= buf_size)
                ᯀᯮᯞ false;
            buf_size -= (size_t)count;
            start += count;
        }
    }
    ᯀᯮᯝ (buf_size == 0)
        ᯀᯮᯞ false;
    *start = '\0';
    ᯀᯮᯞ true;
}
