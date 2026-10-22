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

ರಚನೆ xkb_rmlvo_builder*
xkb_rmlvo_builder_new(ರಚನೆ xkb_context *context,
                      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ನಿಯಮಗಳು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *model,
                      ಎಣಿಕೆ xkb_rmlvo_builder_flags flags)
{
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_rmlvo_builder_flags invalid_flags =
        (flags & ~(ಎಣಿಕೆ xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_ಮೌಲ್ಯಗಳು);
    ಆದರೆ (invalid_flags) {
        log_err_func(context, XKB_LOG_ಸಂದೇಶ_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ರಚನೆ xkb_rmlvo_builder * ಸ್ಥಿರಾಂಕ builder = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(*builder));
    ಆದರೆ (!builder)
        ಹೋಗು error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->ನಿಯಮಗಳು = strdup_ಸುರಕ್ಷಿತ(ನಿಯಮಗಳು);
    ಆದರೆ (!builder->ನಿಯಮಗಳು && ನಿಯಮಗಳು)
        ಹೋಗು error;

    builder->model = strdup_ಸುರಕ್ಷಿತ(model);
    ಆದರೆ (!builder->model && model)
        ಹೋಗು error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    ಹಿಂತಿರುಗಿ builder;

error:
    log_err(context, XKB_ERROR_ಹಂಚಿಕೆ_FAILURE_2,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
rmlvo_builder_append_option(ರಚನೆ xkb_rmlvo_builder *rmlvo,
                            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *option, xkb_layout_ಮುಖವಾಡ_t layouts)
{
    ಆದರೆ (isempty(option))
        ಹಿಂತಿರುಗಿ XKB_ERROR_INVALID;

    /* Check for previous entry */
    ರಚನೆ xkb_rmlvo_builder_option *prev;
    darray_foreach(prev, rmlvo->options) {
        ಆದರೆ (streq(prev->option, option)) {
            /* Merge with previous entry */
            ಆದರೆ (layouts == (xkb_layout_ಮುಖವಾಡ_t)XKB_OPTION_LAYOUT_ಮುಖವಾಡ_GLOBAL) {
                /* Global option overrides layout-specific ones */
                prev->layouts = layouts;
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (prev->layouts != (xkb_layout_ಮುಖವಾಡ_t)
                       XKB_OPTION_LAYOUT_ಮುಖವಾಡ_GLOBAL) {
                /* Layout-specific option cannot override global */
                prev->layouts |= layouts;
            }
            ಹಿಂತಿರುಗಿ XKB_SUCCESS;
        }
    }

    /* Append new entry */
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_rmlvo_builder_option new = {
        .option = strdup_ಸುರಕ್ಷಿತ(option),
        .layouts = layouts
    };
    ಆದರೆ (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_ಹಂಚಿಕೆ_FAILURE_2,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        ಹಿಂತಿರುಗಿ XKB_ERROR_ಹಂಚಿಕೆ_FAILURE;
    }
    darray_append(rmlvo->options, new);
    ಹಿಂತಿರುಗಿ XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(ರಚನೆ xkb_rmlvo_builder *rmlvo,
                                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *layout, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *variant,
                                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಸ್ಥಿರಾಂಕ* options, ಗಾತ್ರ_t options_len)
{
    ಸ್ಥಿರಾಂಕ xkb_layout_ಸೂಚ್ಯಂಕ_t idx = (xkb_layout_ಸೂಚ್ಯಂಕ_t)
                                   darray_ಗಾತ್ರ(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    ಆದರೆ (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ_2,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        ಹಿಂತಿರುಗಿ false;
    }

    /* Append layout entry */
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_rmlvo_builder_layout new = {
        .layout = strdup_ಸುರಕ್ಷಿತ(layout),
        .variant = strdup_ಸುರಕ್ಷಿತ(variant)
    };

    ಆದರೆ (!new.layout || (!new.variant && variant)) {
        ಬಿಡುಗಡೆ(new.layout);
        ಬಿಡುಗಡೆ(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_ಹಂಚಿಕೆ_FAILURE_2,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        ಹಿಂತಿರುಗಿ false;
    }

    darray_append(rmlvo->layouts, new);

    ಆದರೆ (!options)
        options_len = 0;

    ಸ್ಥಿರಾಂಕ xkb_layout_ಮುಖವಾಡ_t layout_ಮುಖವಾಡ = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    ಗಾಗಿ (ಗಾತ್ರ_t k = 0; k < options_len; k++) {
        ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2 =
            rmlvo_builder_append_option(rmlvo, options[k], layout_ಮುಖವಾಡ);

        ಆದರೆ (ಸ್ಥಿತಿ_2 == XKB_ERROR_ಹಂಚಿಕೆ_FAILURE)
            ಹಿಂತಿರುಗಿ false;
    }

    ಹಿಂತಿರುಗಿ true;
}

bool
xkb_rmlvo_builder_append_option(ರಚನೆ xkb_rmlvo_builder *rmlvo,
                                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *option)
{
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2 = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_ಮುಖವಾಡ_t)XKB_OPTION_LAYOUT_ಮುಖವಾಡ_GLOBAL
    );
    ಹಿಂತಿರುಗಿ (ಸ್ಥಿತಿ_2 == XKB_SUCCESS);
}

ರಚನೆ xkb_rmlvo_builder *
xkb_rmlvo_builder_ref(ರಚನೆ xkb_rmlvo_builder *rmlvo)
{
    ಪ್ರತಿಜ್ಞಾ(rmlvo->refcnt > 0);
    rmlvo->refcnt++;
    ಹಿಂತಿರುಗಿ rmlvo;
}

ಶೂನ್ಯ
xkb_rmlvo_builder_unref(ರಚನೆ xkb_rmlvo_builder *rmlvo)
{
    ಪ್ರತಿಜ್ಞಾ(!rmlvo || rmlvo->refcnt > 0);
    ಆದರೆ (!rmlvo || --rmlvo->refcnt > 0)
        ಹಿಂತಿರುಗಿ;

    ಬಿಡುಗಡೆ(rmlvo->ನಿಯಮಗಳು);
    ಬಿಡುಗಡೆ(rmlvo->model);

    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_rmlvo_builder_layout *layout;
    darray_foreach(layout, rmlvo->layouts) {
        ಬಿಡುಗಡೆ(layout->layout);
        ಬಿಡುಗಡೆ(layout->variant);
    }
    darray_free(rmlvo->layouts);

    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_rmlvo_builder_option *option;
    darray_foreach(option, rmlvo->options)
        ಬಿಡುಗಡೆ(option->option);
    darray_free(rmlvo->options);

    xkb_context_unref(rmlvo->ctx);
    ಬಿಡುಗಡೆ(rmlvo);
}

/* NOTE: the converse function, `xkb_rules_names_to_rmlvo_builder`, is currently
 * only used in tests */
bool
xkb_rmlvo_builder_ಗಮ್ಯ_ನಿಯಮಗಳು_ಹೆಸರುಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_rmlvo_builder *builder,
                                 ರಚನೆ xkb_ನಿಯಮ_ಹೆಸರುಗಳು *rmlvo,
                                 ಅಕ್ಷರ *buf, ಗಾತ್ರ_t buf_ಗಾತ್ರ)
{
    rmlvo->ನಿಯಮಗಳು = builder->ನಿಯಮಗಳು;
    rmlvo->model = builder->model;

    ಅಕ್ಷರ *ಆರಂಭ = buf;
    rmlvo->layout = ಆರಂಭ;
    darray_ಗಾತ್ರ_t k;
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = snprintf(ಆರಂಭ, buf_ಗಾತ್ರ, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        ಆದರೆ (ಎಣಿಕೆ_2 < 0 || (ಗಾತ್ರ_t)ಎಣಿಕೆ_2 >= buf_ಗಾತ್ರ)
            ಹಿಂತಿರುಗಿ false;
        buf_ಗಾತ್ರ -= (ಗಾತ್ರ_t)ಎಣಿಕೆ_2;
        ಆರಂಭ += ಎಣಿಕೆ_2;
    }
    ಆದರೆ (buf_ಗಾತ್ರ <= 1)
        ಹಿಂತಿರುಗಿ false;
    *ಆರಂಭ = '\0';
    ಆರಂಭ++;
    buf_ಗಾತ್ರ--;

    rmlvo->variant = ಆರಂಭ;
    darray_enumerate(k, layout, builder->layouts) {
        ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = snprintf(ಆರಂಭ, buf_ಗಾತ್ರ, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        ಆದರೆ (ಎಣಿಕೆ_2 < 0 || (ಗಾತ್ರ_t) ಎಣಿಕೆ_2 >= buf_ಗಾತ್ರ)
            ಹಿಂತಿರುಗಿ false;
        buf_ಗಾತ್ರ -= (ಗಾತ್ರ_t)ಎಣಿಕೆ_2;
        ಆರಂಭ += ಎಣಿಕೆ_2;
    }
    ಆದರೆ (buf_ಗಾತ್ರ <= 1)
        ಹಿಂತಿರುಗಿ false;
    *ಆರಂಭ = '\0';
    ಆರಂಭ++;
    buf_ಗಾತ್ರ--;

    rmlvo->options = ಆರಂಭ;
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = snprintf(ಆರಂಭ, buf_ಗಾತ್ರ, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        ಆದರೆ (ಎಣಿಕೆ_2 < 0 || (ಗಾತ್ರ_t) ಎಣಿಕೆ_2 >= buf_ಗಾತ್ರ)
            ಹಿಂತಿರುಗಿ false;
        buf_ಗಾತ್ರ -= (ಗಾತ್ರ_t)ಎಣಿಕೆ_2;
        ಆರಂಭ += ಎಣಿಕೆ_2;
        ಆದರೆ (option->layouts != XKB_LAYOUT_INVALID) {
            ಎಣಿಕೆ_2 = snprintf(ಆರಂಭ, buf_ಗಾತ್ರ, "%c%"PRIu32,
                             OPTIONS_ಗುಂಪು_SPECIFIER_ಪೂರ್ವಪ್ರತ್ಯಯ, option->layouts);
            ಆದರೆ (ಎಣಿಕೆ_2 < 0 || (ಗಾತ್ರ_t) ಎಣಿಕೆ_2 >= buf_ಗಾತ್ರ)
                ಹಿಂತಿರುಗಿ false;
            buf_ಗಾತ್ರ -= (ಗಾತ್ರ_t)ಎಣಿಕೆ_2;
            ಆರಂಭ += ಎಣಿಕೆ_2;
        }
    }
    ಆದರೆ (buf_ಗಾತ್ರ == 0)
        ಹಿಂತಿರುಗಿ false;
    *ಆರಂಭ = '\0';
    ಹಿಂತಿರುಗಿ true;
}
