/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon-status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "context.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "features/enums.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "messages-codes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rmlvo.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp/rules.h"

ਬਣਤਰ xkb_rmlvo_builder*
xkb_rmlvo_builder_new(ਬਣਤਰ xkb_context *context,
                      ਅਚਲ ਅੱਖਰ *ਨਿਯਮਾਂ, ਅਚਲ ਅੱਖਰ *model,
                      ਗਿਣਤੀ xkb_rmlvo_builder_flags flags)
{
    ਅਚਲ ਗਿਣਤੀ xkb_rmlvo_builder_flags invalid_flags =
        (flags & ~(ਗਿਣਤੀ xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_ਮੁੱਲਾਂ);
    ਜੇ (invalid_flags) {
        log_err_func(context, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਬਣਤਰ xkb_rmlvo_builder * ਅਚਲ builder = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(*builder));
    ਜੇ (!builder)
        ਜਾਓ error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->ਨਿਯਮਾਂ = strdup_ਸੁਰੱਖਿਅਤ(ਨਿਯਮਾਂ);
    ਜੇ (!builder->ਨਿਯਮਾਂ && ਨਿਯਮਾਂ)
        ਜਾਓ error;

    builder->model = strdup_ਸੁਰੱਖਿਅਤ(model);
    ਜੇ (!builder->model && model)
        ਜਾਓ error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    ਵਾਪਸ builder;

error:
    log_err(context, XKB_ERROR_ਵੰਡ_FAILURE_2,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਗਿਣਤੀ xkb_ਸਥਿਤੀ
rmlvo_builder_append_option(ਬਣਤਰ xkb_rmlvo_builder *rmlvo,
                            ਅਚਲ ਅੱਖਰ *option, xkb_layout_ਨਕਾਬ_t layouts)
{
    ਜੇ (isempty(option))
        ਵਾਪਸ XKB_ERROR_INVALID;

    /* Check for previous entry */
    ਬਣਤਰ xkb_rmlvo_builder_option *prev;
    darray_foreach(prev, rmlvo->options) {
        ਜੇ (streq(prev->option, option)) {
            /* Merge with previous entry */
            ਜੇ (layouts == (xkb_layout_ਨਕਾਬ_t)XKB_OPTION_LAYOUT_ਨਕਾਬ_GLOBAL) {
                /* Global option overrides layout-specific ones */
                prev->layouts = layouts;
            } ਨਹੀਂਤਾਂ ਜੇ (prev->layouts != (xkb_layout_ਨਕਾਬ_t)
                       XKB_OPTION_LAYOUT_ਨਕਾਬ_GLOBAL) {
                /* Layout-specific option cannot override global */
                prev->layouts |= layouts;
            }
            ਵਾਪਸ XKB_SUCCESS;
        }
    }

    /* Append new entry */
    ਅਚਲ ਬਣਤਰ xkb_rmlvo_builder_option new = {
        .option = strdup_ਸੁਰੱਖਿਅਤ(option),
        .layouts = layouts
    };
    ਜੇ (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_ਵੰਡ_FAILURE_2,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        ਵਾਪਸ XKB_ERROR_ਵੰਡ_FAILURE;
    }
    darray_append(rmlvo->options, new);
    ਵਾਪਸ XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(ਬਣਤਰ xkb_rmlvo_builder *rmlvo,
                                ਅਚਲ ਅੱਖਰ *layout, ਅਚਲ ਅੱਖਰ *variant,
                                ਅਚਲ ਅੱਖਰ* ਅਚਲ* options, ਮਾਪ_t options_len)
{
    ਅਚਲ xkb_layout_ਸੂਚਕਾਂਕ_t idx = (xkb_layout_ਸੂਚਕਾਂਕ_t)
                                   darray_ਮਾਪ(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    ਜੇ (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ_2,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        ਵਾਪਸ false;
    }

    /* Append layout entry */
    ਅਚਲ ਬਣਤਰ xkb_rmlvo_builder_layout new = {
        .layout = strdup_ਸੁਰੱਖਿਅਤ(layout),
        .variant = strdup_ਸੁਰੱਖਿਅਤ(variant)
    };

    ਜੇ (!new.layout || (!new.variant && variant)) {
        ਮੁਕਤ(new.layout);
        ਮੁਕਤ(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_ਵੰਡ_FAILURE_2,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        ਵਾਪਸ false;
    }

    darray_append(rmlvo->layouts, new);

    ਜੇ (!options)
        options_len = 0;

    ਅਚਲ xkb_layout_ਨਕਾਬ_t layout_ਨਕਾਬ = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    ਲਈ (ਮਾਪ_t k = 0; k < options_len; k++) {
        ਅਚਲ ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ =
            rmlvo_builder_append_option(rmlvo, options[k], layout_ਨਕਾਬ);

        ਜੇ (ਸਥਿਤੀ == XKB_ERROR_ਵੰਡ_FAILURE)
            ਵਾਪਸ false;
    }

    ਵਾਪਸ true;
}

bool
xkb_rmlvo_builder_append_option(ਬਣਤਰ xkb_rmlvo_builder *rmlvo,
                                ਅਚਲ ਅੱਖਰ *option)
{
    ਅਚਲ ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_ਨਕਾਬ_t)XKB_OPTION_LAYOUT_ਨਕਾਬ_GLOBAL
    );
    ਵਾਪਸ (ਸਥਿਤੀ == XKB_SUCCESS);
}

ਬਣਤਰ xkb_rmlvo_builder *
xkb_rmlvo_builder_ref(ਬਣਤਰ xkb_rmlvo_builder *rmlvo)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(rmlvo->refcnt > 0);
    rmlvo->refcnt++;
    ਵਾਪਸ rmlvo;
}

ਖਾਲੀ
xkb_rmlvo_builder_unref(ਬਣਤਰ xkb_rmlvo_builder *rmlvo)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(!rmlvo || rmlvo->refcnt > 0);
    ਜੇ (!rmlvo || --rmlvo->refcnt > 0)
        ਵਾਪਸ;

    ਮੁਕਤ(rmlvo->ਨਿਯਮਾਂ);
    ਮੁਕਤ(rmlvo->model);

    ਅਚਲ ਬਣਤਰ xkb_rmlvo_builder_layout *layout;
    darray_foreach(layout, rmlvo->layouts) {
        ਮੁਕਤ(layout->layout);
        ਮੁਕਤ(layout->variant);
    }
    darray_free(rmlvo->layouts);

    ਅਚਲ ਬਣਤਰ xkb_rmlvo_builder_option *option;
    darray_foreach(option, rmlvo->options)
        ਮੁਕਤ(option->option);
    darray_free(rmlvo->options);

    xkb_context_unref(rmlvo->ctx);
    ਮੁਕਤ(rmlvo);
}

/* NOTE: the converse function, `xkb_rules_names_to_rmlvo_builder`, is currently
 * only used in tests */
bool
xkb_rmlvo_builder_ਮੰਜ਼ਿਲ_ਨਿਯਮਾਂ_ਨਾਂ(ਅਚਲ ਬਣਤਰ xkb_rmlvo_builder *builder,
                                 ਬਣਤਰ xkb_ਨਿਯਮ_ਨਾਂ *rmlvo,
                                 ਅੱਖਰ *buf, ਮਾਪ_t buf_ਮਾਪ)
{
    rmlvo->ਨਿਯਮਾਂ = builder->ਨਿਯਮਾਂ;
    rmlvo->model = builder->model;

    ਅੱਖਰ *ਸ਼ੁਰੂ = buf;
    rmlvo->layout = ਸ਼ੁਰੂ;
    darray_ਮਾਪ_t k;
    ਅਚਲ ਬਣਤਰ xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        ਪੂਰਨ ਗਿਣਤੀ_2 = snprintf(ਸ਼ੁਰੂ, buf_ਮਾਪ, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        ਜੇ (ਗਿਣਤੀ_2 < 0 || (ਮਾਪ_t)ਗਿਣਤੀ_2 >= buf_ਮਾਪ)
            ਵਾਪਸ false;
        buf_ਮਾਪ -= (ਮਾਪ_t)ਗਿਣਤੀ_2;
        ਸ਼ੁਰੂ += ਗਿਣਤੀ_2;
    }
    ਜੇ (buf_ਮਾਪ <= 1)
        ਵਾਪਸ false;
    *ਸ਼ੁਰੂ = '\0';
    ਸ਼ੁਰੂ++;
    buf_ਮਾਪ--;

    rmlvo->variant = ਸ਼ੁਰੂ;
    darray_enumerate(k, layout, builder->layouts) {
        ਪੂਰਨ ਗਿਣਤੀ_2 = snprintf(ਸ਼ੁਰੂ, buf_ਮਾਪ, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        ਜੇ (ਗਿਣਤੀ_2 < 0 || (ਮਾਪ_t) ਗਿਣਤੀ_2 >= buf_ਮਾਪ)
            ਵਾਪਸ false;
        buf_ਮਾਪ -= (ਮਾਪ_t)ਗਿਣਤੀ_2;
        ਸ਼ੁਰੂ += ਗਿਣਤੀ_2;
    }
    ਜੇ (buf_ਮਾਪ <= 1)
        ਵਾਪਸ false;
    *ਸ਼ੁਰੂ = '\0';
    ਸ਼ੁਰੂ++;
    buf_ਮਾਪ--;

    rmlvo->options = ਸ਼ੁਰੂ;
    ਅਚਲ ਬਣਤਰ xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        ਪੂਰਨ ਗਿਣਤੀ_2 = snprintf(ਸ਼ੁਰੂ, buf_ਮਾਪ, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        ਜੇ (ਗਿਣਤੀ_2 < 0 || (ਮਾਪ_t) ਗਿਣਤੀ_2 >= buf_ਮਾਪ)
            ਵਾਪਸ false;
        buf_ਮਾਪ -= (ਮਾਪ_t)ਗਿਣਤੀ_2;
        ਸ਼ੁਰੂ += ਗਿਣਤੀ_2;
        ਜੇ (option->layouts != XKB_LAYOUT_INVALID) {
            ਗਿਣਤੀ_2 = snprintf(ਸ਼ੁਰੂ, buf_ਮਾਪ, "%c%"PRIu32,
                             OPTIONS_ਸਮੂਹ_SPECIFIER_ਅਗੇਤਰ, option->layouts);
            ਜੇ (ਗਿਣਤੀ_2 < 0 || (ਮਾਪ_t) ਗਿਣਤੀ_2 >= buf_ਮਾਪ)
                ਵਾਪਸ false;
            buf_ਮਾਪ -= (ਮਾਪ_t)ਗਿਣਤੀ_2;
            ਸ਼ੁਰੂ += ਗਿਣਤੀ_2;
        }
    }
    ਜੇ (buf_ਮਾਪ == 0)
        ਵਾਪਸ false;
    *ਸ਼ੁਰੂ = '\0';
    ਵਾਪਸ true;
}
