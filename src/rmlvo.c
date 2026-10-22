/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#समावेश "config.h"

#समावेश <assert.h>
#समावेश <stdint.h>
#समावेश <string.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "xkbcommon/xkbcommon-status.h"
#समावेश "context.h"
#समावेश "features/enums.h"
#समावेश "keymap.h"
#समावेश "messages-codes.h"
#समावेश "rmlvo.h"
#समावेश "darray.h"
#समावेश "utils.h"
#समावेश "xkbcomp/rules.h"

काष्ठा xkb_rmlvo_builder*
xkb_rmlvo_builder_new(काष्ठा xkb_context *context,
                      स्थिर अक्षर *rules, स्थिर अक्षर *model,
                      क्रमागत xkb_rmlvo_builder_flags flags)
{
    स्थिर क्रमागत xkb_rmlvo_builder_flags invalid_flags =
        (flags & ~(क्रमागत xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_मूल्यावली);
    अगर (invalid_flags) {
        log_err_func(context, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        वापस शून्य;
    }

    काष्ठा xkb_rmlvo_builder * स्थिर builder = सुस्मृति(1, माप(*builder));
    अगर (!builder)
        जाओ error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->rules = strdup_safe(rules);
    अगर (!builder->rules && rules)
        जाओ error;

    builder->model = strdup_safe(model);
    अगर (!builder->model && model)
        जाओ error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    वापस builder;

error:
    log_err(context, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    वापस शून्य;
}

अटल क्रमागत xkb_status
rmlvo_builder_append_option(काष्ठा xkb_rmlvo_builder *rmlvo,
                            स्थिर अक्षर *option, xkb_layout_mask_t layouts)
{
    अगर (isempty(option))
        वापस XKB_ERROR_INVALID;

    /* Check for previous entry */
    काष्ठा xkb_rmlvo_builder_option *पूर्व;
    darray_foreach(पूर्व, rmlvo->options) {
        अगर (streq(पूर्व->option, option)) {
            /* Merge with previous entry */
            अगर (layouts == (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Global option overrides layout-specific ones */
                पूर्व->layouts = layouts;
            } अन्यथा अगर (पूर्व->layouts != (xkb_layout_mask_t)
                       XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Layout-specific option cannot override global */
                पूर्व->layouts |= layouts;
            }
            वापस XKB_SUCCESS;
        }
    }

    /* Append new entry */
    स्थिर काष्ठा xkb_rmlvo_builder_option new = {
        .option = strdup_safe(option),
        .layouts = layouts
    };
    अगर (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        वापस XKB_ERROR_ALLOCATION_FAILURE;
    }
    darray_append(rmlvo->options, new);
    वापस XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(काष्ठा xkb_rmlvo_builder *rmlvo,
                                स्थिर अक्षर *layout, स्थिर अक्षर *variant,
                                स्थिर अक्षर* स्थिर* options, माप_प्रकार options_len)
{
    स्थिर xkb_layout_सूचकांक_t idx = (xkb_layout_सूचकांक_t)
                                   darray_आकार(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    अगर (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        वापस false;
    }

    /* Append layout entry */
    स्थिर काष्ठा xkb_rmlvo_builder_layout new = {
        .layout = strdup_safe(layout),
        .variant = strdup_safe(variant)
    };

    अगर (!new.layout || (!new.variant && variant)) {
        मुक्त(new.layout);
        मुक्त(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        वापस false;
    }

    darray_append(rmlvo->layouts, new);

    अगर (!options)
        options_len = 0;

    स्थिर xkb_layout_mask_t layout_mask = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    क्रम (माप_प्रकार k = 0; k < options_len; k++) {
        स्थिर क्रमागत xkb_status status =
            rmlvo_builder_append_option(rmlvo, options[k], layout_mask);

        अगर (status == XKB_ERROR_ALLOCATION_FAILURE)
            वापस false;
    }

    वापस true;
}

bool
xkb_rmlvo_builder_append_option(काष्ठा xkb_rmlvo_builder *rmlvo,
                                स्थिर अक्षर *option)
{
    स्थिर क्रमागत xkb_status status = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL
    );
    वापस (status == XKB_SUCCESS);
}

काष्ठा xkb_rmlvo_builder *
xkb_rmlvo_builder_ref(काष्ठा xkb_rmlvo_builder *rmlvo)
{
    निश्चित(rmlvo->refcnt > 0);
    rmlvo->refcnt++;
    वापस rmlvo;
}

व्योम
xkb_rmlvo_builder_unref(काष्ठा xkb_rmlvo_builder *rmlvo)
{
    निश्चित(!rmlvo || rmlvo->refcnt > 0);
    अगर (!rmlvo || --rmlvo->refcnt > 0)
        वापस;

    मुक्त(rmlvo->rules);
    मुक्त(rmlvo->model);

    स्थिर काष्ठा xkb_rmlvo_builder_layout *layout;
    darray_foreach(layout, rmlvo->layouts) {
        मुक्त(layout->layout);
        मुक्त(layout->variant);
    }
    darray_free(rmlvo->layouts);

    स्थिर काष्ठा xkb_rmlvo_builder_option *option;
    darray_foreach(option, rmlvo->options)
        मुक्त(option->option);
    darray_free(rmlvo->options);

    xkb_context_unref(rmlvo->ctx);
    मुक्त(rmlvo);
}

/* NOTE: the converse function, `xkb_rules_names_to_rmlvo_builder`, is currently
 * only used in tests */
bool
xkb_rmlvo_builder_to_rules_names(स्थिर काष्ठा xkb_rmlvo_builder *builder,
                                 काष्ठा xkb_rule_names *rmlvo,
                                 अक्षर *buf, माप_प्रकार buf_आकार)
{
    rmlvo->rules = builder->rules;
    rmlvo->model = builder->model;

    अक्षर *start = buf;
    rmlvo->layout = start;
    darray_आकार_t k;
    स्थिर काष्ठा xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        पूर्णांक गिनती = snprintf(start, buf_आकार, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        अगर (गिनती < 0 || (माप_प्रकार)गिनती >= buf_आकार)
            वापस false;
        buf_आकार -= (माप_प्रकार)गिनती;
        start += गिनती;
    }
    अगर (buf_आकार <= 1)
        वापस false;
    *start = '\0';
    start++;
    buf_आकार--;

    rmlvo->variant = start;
    darray_enumerate(k, layout, builder->layouts) {
        पूर्णांक गिनती = snprintf(start, buf_आकार, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        अगर (गिनती < 0 || (माप_प्रकार) गिनती >= buf_आकार)
            वापस false;
        buf_आकार -= (माप_प्रकार)गिनती;
        start += गिनती;
    }
    अगर (buf_आकार <= 1)
        वापस false;
    *start = '\0';
    start++;
    buf_आकार--;

    rmlvo->options = start;
    स्थिर काष्ठा xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        पूर्णांक गिनती = snprintf(start, buf_आकार, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        अगर (गिनती < 0 || (माप_प्रकार) गिनती >= buf_आकार)
            वापस false;
        buf_आकार -= (माप_प्रकार)गिनती;
        start += गिनती;
        अगर (option->layouts != XKB_LAYOUT_INVALID) {
            गिनती = snprintf(start, buf_आकार, "%c%"PRIu32,
                             OPTIONS_GROUP_SPECIFIER_PREFIX, option->layouts);
            अगर (गिनती < 0 || (माप_प्रकार) गिनती >= buf_आकार)
                वापस false;
            buf_आकार -= (माप_प्रकार)गिनती;
            start += गिनती;
        }
    }
    अगर (buf_आकार == 0)
        वापस false;
    *start = '\0';
    वापस true;
}
