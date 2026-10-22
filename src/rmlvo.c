/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <string.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon-status.h"
#അന്തര്ഭാവ "context.h"
#അന്തര്ഭാവ "features/enums.h"
#അന്തര്ഭാവ "keymap.h"
#അന്തര്ഭാവ "messages-codes.h"
#അന്തര്ഭാവ "rmlvo.h"
#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "xkbcomp/rules.h"

ഘടന xkb_rmlvo_builder*
xkb_rmlvo_builder_new(ഘടന xkb_context *context,
                      സ്ഥിരം അക്ഷരം *rules, സ്ഥിരം അക്ഷരം *model,
                      എണ്ണൽ xkb_rmlvo_builder_flags flags)
{
    സ്ഥിരം എണ്ണൽ xkb_rmlvo_builder_flags invalid_flags =
        (flags & ~(എണ്ണൽ xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_മൂല്യങ്ങൾ);
    എങ്കിൽ (invalid_flags) {
        log_err_func(context, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        തിരികെ ശൂന്യ;
    }

    ഘടന xkb_rmlvo_builder * സ്ഥിരം builder = calloc(1, വലുപ്പം(*builder));
    എങ്കിൽ (!builder)
        പോകുക error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->rules = strdup_safe(rules);
    എങ്കിൽ (!builder->rules && rules)
        പോകുക error;

    builder->model = strdup_safe(model);
    എങ്കിൽ (!builder->model && model)
        പോകുക error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    തിരികെ builder;

error:
    log_err(context, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    തിരികെ ശൂന്യ;
}

സ്ഥിതം എണ്ണൽ xkb_status
rmlvo_builder_append_option(ഘടന xkb_rmlvo_builder *rmlvo,
                            സ്ഥിരം അക്ഷരം *option, xkb_layout_mask_t layouts)
{
    എങ്കിൽ (isempty(option))
        തിരികെ XKB_ERROR_INVALID;

    /* Check for previous entry */
    ഘടന xkb_rmlvo_builder_option *prev;
    darray_foreach(prev, rmlvo->options) {
        എങ്കിൽ (streq(prev->option, option)) {
            /* Merge with previous entry */
            എങ്കിൽ (layouts == (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Global option overrides layout-specific ones */
                prev->layouts = layouts;
            } അല്ലെങ്കിൽ എങ്കിൽ (prev->layouts != (xkb_layout_mask_t)
                       XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                /* Layout-specific option cannot override global */
                prev->layouts |= layouts;
            }
            തിരികെ XKB_SUCCESS;
        }
    }

    /* Append new entry */
    സ്ഥിരം ഘടന xkb_rmlvo_builder_option new = {
        .option = strdup_safe(option),
        .layouts = layouts
    };
    എങ്കിൽ (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        തിരികെ XKB_ERROR_ALLOCATION_FAILURE;
    }
    darray_append(rmlvo->options, new);
    തിരികെ XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(ഘടന xkb_rmlvo_builder *rmlvo,
                                സ്ഥിരം അക്ഷരം *layout, സ്ഥിരം അക്ഷരം *variant,
                                സ്ഥിരം അക്ഷരം* സ്ഥിരം* options, size_t options_len)
{
    സ്ഥിരം xkb_layout_index_t idx = (xkb_layout_index_t)
                                   darray_size(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    എങ്കിൽ (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        തിരികെ false;
    }

    /* Append layout entry */
    സ്ഥിരം ഘടന xkb_rmlvo_builder_layout new = {
        .layout = strdup_safe(layout),
        .variant = strdup_safe(variant)
    };

    എങ്കിൽ (!new.layout || (!new.variant && variant)) {
        സ്വതന്ത്രം(new.layout);
        സ്വതന്ത്രം(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        തിരികെ false;
    }

    darray_append(rmlvo->layouts, new);

    എങ്കിൽ (!options)
        options_len = 0;

    സ്ഥിരം xkb_layout_mask_t layout_mask = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    വേണ്ടി (size_t k = 0; k < options_len; k++) {
        സ്ഥിരം എണ്ണൽ xkb_status status =
            rmlvo_builder_append_option(rmlvo, options[k], layout_mask);

        എങ്കിൽ (status == XKB_ERROR_ALLOCATION_FAILURE)
            തിരികെ false;
    }

    തിരികെ true;
}

bool
xkb_rmlvo_builder_append_option(ഘടന xkb_rmlvo_builder *rmlvo,
                                സ്ഥിരം അക്ഷരം *option)
{
    സ്ഥിരം എണ്ണൽ xkb_status status = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL
    );
    തിരികെ (status == XKB_SUCCESS);
}

ഘടന xkb_rmlvo_builder *
xkb_rmlvo_builder_ref(ഘടന xkb_rmlvo_builder *rmlvo)
{
    പ്രതിജ്ഞാ(rmlvo->refcnt > 0);
    rmlvo->refcnt++;
    തിരികെ rmlvo;
}

ശൂന്യം
xkb_rmlvo_builder_unref(ഘടന xkb_rmlvo_builder *rmlvo)
{
    പ്രതിജ്ഞാ(!rmlvo || rmlvo->refcnt > 0);
    എങ്കിൽ (!rmlvo || --rmlvo->refcnt > 0)
        തിരികെ;

    സ്വതന്ത്രം(rmlvo->rules);
    സ്വതന്ത്രം(rmlvo->model);

    സ്ഥിരം ഘടന xkb_rmlvo_builder_layout *layout;
    darray_foreach(layout, rmlvo->layouts) {
        സ്വതന്ത്രം(layout->layout);
        സ്വതന്ത്രം(layout->variant);
    }
    darray_free(rmlvo->layouts);

    സ്ഥിരം ഘടന xkb_rmlvo_builder_option *option;
    darray_foreach(option, rmlvo->options)
        സ്വതന്ത്രം(option->option);
    darray_free(rmlvo->options);

    xkb_context_unref(rmlvo->ctx);
    സ്വതന്ത്രം(rmlvo);
}

/* NOTE: the converse function, `xkb_rules_names_to_rmlvo_builder`, is currently
 * only used in tests */
bool
xkb_rmlvo_builder_to_rules_names(സ്ഥിരം ഘടന xkb_rmlvo_builder *builder,
                                 ഘടന xkb_rule_names *rmlvo,
                                 അക്ഷരം *buf, size_t buf_size)
{
    rmlvo->rules = builder->rules;
    rmlvo->model = builder->model;

    അക്ഷരം *start = buf;
    rmlvo->layout = start;
    darray_size_t k;
    സ്ഥിരം ഘടന xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        പൂർണ്ണസംഖ്യ എണ്ണം = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        എങ്കിൽ (എണ്ണം < 0 || (size_t)എണ്ണം >= buf_size)
            തിരികെ false;
        buf_size -= (size_t)എണ്ണം;
        start += എണ്ണം;
    }
    എങ്കിൽ (buf_size <= 1)
        തിരികെ false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->variant = start;
    darray_enumerate(k, layout, builder->layouts) {
        പൂർണ്ണസംഖ്യ എണ്ണം = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        എങ്കിൽ (എണ്ണം < 0 || (size_t) എണ്ണം >= buf_size)
            തിരികെ false;
        buf_size -= (size_t)എണ്ണം;
        start += എണ്ണം;
    }
    എങ്കിൽ (buf_size <= 1)
        തിരികെ false;
    *start = '\0';
    start++;
    buf_size--;

    rmlvo->options = start;
    സ്ഥിരം ഘടന xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        പൂർണ്ണസംഖ്യ എണ്ണം = snprintf(start, buf_size, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        എങ്കിൽ (എണ്ണം < 0 || (size_t) എണ്ണം >= buf_size)
            തിരികെ false;
        buf_size -= (size_t)എണ്ണം;
        start += എണ്ണം;
        എങ്കിൽ (option->layouts != XKB_LAYOUT_INVALID) {
            എണ്ണം = snprintf(start, buf_size, "%c%"PRIu32,
                             OPTIONS_GROUP_SPECIFIER_PREFIX, option->layouts);
            എങ്കിൽ (എണ്ണം < 0 || (size_t) എണ്ണം >= buf_size)
                തിരികെ false;
            buf_size -= (size_t)എണ്ണം;
            start += എണ്ണം;
        }
    }
    എങ്കിൽ (buf_size == 0)
        തിരികെ false;
    *start = '\0';
    തിരികെ true;
}
