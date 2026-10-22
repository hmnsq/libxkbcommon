/*
 * Copyright © 2025 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>
#అన్తర్భావ <stdint.h>
#అన్తర్భావ <string.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "xkbcommon/xkbcommon-status.h"
#అన్తర్భావ "context.h"
#అన్తర్భావ "features/enums.h"
#అన్తర్భావ "keymap.h"
#అన్తర్భావ "messages-codes.h"
#అన్తర్భావ "rmlvo.h"
#అన్తర్భావ "darray.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "xkbcomp/rules.h"

నిర్మాణం xkb_rmlvo_builder*
xkb_rmlvo_builder_new(నిర్మాణం xkb_context *context,
                      స్థిరరాశి అక్షరం *నియమాలు, స్థిరరాశి అక్షరం *model,
                      గణన xkb_rmlvo_builder_flags flags)
{
    స్థిరరాశి గణన xkb_rmlvo_builder_flags invalid_flags =
        (flags & ~(గణన xkb_rmlvo_builder_flags)XKB_RMLVO_BUILDER_FLAGS_విలువలు);
    ఒకవేళ (invalid_flags) {
        log_err_func(context, XKB_LOG_సందేశం_NO_ID,
                     "Unsupported RMLVO flags: 0x%x\n",
                     invalid_flags);
        తిరిగి ఖాళీ;
    }

    నిర్మాణం xkb_rmlvo_builder * స్థిరరాశి builder = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(*builder));
    ఒకవేళ (!builder)
        వెళ్ళు error;

    builder->refcnt = 1;
    builder->ctx = xkb_context_ref(context);

    builder->నియమాలు = strdup_సురక్షిత(నియమాలు);
    ఒకవేళ (!builder->నియమాలు && నియమాలు)
        వెళ్ళు error;

    builder->model = strdup_సురక్షిత(model);
    ఒకవేళ (!builder->model && model)
        వెళ్ళు error;

    darray_init(builder->layouts);
    darray_init(builder->options);

    తిరిగి builder;

error:
    log_err(context, XKB_ERROR_కేటాయింపు_FAILURE_2,
            "Cannot allocate a RMLVO builder.\n");
    xkb_rmlvo_builder_unref(builder);
    తిరిగి ఖాళీ;
}

స్థిర గణన xkb_స్థితి
rmlvo_builder_append_option(నిర్మాణం xkb_rmlvo_builder *rmlvo,
                            స్థిరరాశి అక్షరం *option, xkb_layout_ముసుగు_t layouts)
{
    ఒకవేళ (isempty(option))
        తిరిగి XKB_ERROR_INVALID;

    /* Check for previous entry */
    నిర్మాణం xkb_rmlvo_builder_option *prev;
    darray_foreach(prev, rmlvo->options) {
        ఒకవేళ (streq(prev->option, option)) {
            /* Merge with previous entry */
            ఒకవేళ (layouts == (xkb_layout_ముసుగు_t)XKB_OPTION_LAYOUT_ముసుగు_GLOBAL) {
                /* Global option overrides layout-specific ones */
                prev->layouts = layouts;
            } లేకపోతే ఒకవేళ (prev->layouts != (xkb_layout_ముసుగు_t)
                       XKB_OPTION_LAYOUT_ముసుగు_GLOBAL) {
                /* Layout-specific option cannot override global */
                prev->layouts |= layouts;
            }
            తిరిగి XKB_SUCCESS;
        }
    }

    /* Append new entry */
    స్థిరరాశి నిర్మాణం xkb_rmlvo_builder_option new = {
        .option = strdup_సురక్షిత(option),
        .layouts = layouts
    };
    ఒకవేళ (!new.option) {
        log_err(rmlvo->ctx, XKB_ERROR_కేటాయింపు_FAILURE_2,
                "Cannot allocate option \"%s\" to the RMLVO builder.\n",
                option);
        తిరిగి XKB_ERROR_కేటాయింపు_FAILURE;
    }
    darray_append(rmlvo->options, new);
    తిరిగి XKB_SUCCESS;
}

bool
xkb_rmlvo_builder_append_layout(నిర్మాణం xkb_rmlvo_builder *rmlvo,
                                స్థిరరాశి అక్షరం *layout, స్థిరరాశి అక్షరం *variant,
                                స్థిరరాశి అక్షరం* స్థిరరాశి* options, కొలత_t options_len)
{
    స్థిరరాశి xkb_layout_సూచీ_t idx = (xkb_layout_సూచీ_t)
                                   darray_కొలత(rmlvo->layouts);

    static_assert(XKB_MAX_GROUPS == 32, "Invalid shift for layout mask");
    ఒకవేళ (idx >= XKB_MAX_GROUPS) {
        log_err(rmlvo->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ_2,
                "Maximum layout count reached: %u; "
                "cannot add layout \"%s(%s)\" to the RMLVO builder.\n",
                XKB_MAX_GROUPS, layout, (variant) ? variant : "");
        తిరిగి false;
    }

    /* Append layout entry */
    స్థిరరాశి నిర్మాణం xkb_rmlvo_builder_layout new = {
        .layout = strdup_సురక్షిత(layout),
        .variant = strdup_సురక్షిత(variant)
    };

    ఒకవేళ (!new.layout || (!new.variant && variant)) {
        విడుదల(new.layout);
        విడుదల(new.variant);
        log_err(rmlvo->ctx, XKB_ERROR_కేటాయింపు_FAILURE_2,
                "Cannot allocate layout \"%s(%s)\" to the RMLVO builder.\n",
                layout, (variant) ? variant : "");
        తిరిగి false;
    }

    darray_append(rmlvo->layouts, new);

    ఒకవేళ (!options)
        options_len = 0;

    స్థిరరాశి xkb_layout_ముసుగు_t layout_ముసుగు = (UINT32_C(1) << idx);

    /* Append layout-specific options entries */
    కొరకు (కొలత_t k = 0; k < options_len; k++) {
        స్థిరరాశి గణన xkb_స్థితి స్థితి_2 =
            rmlvo_builder_append_option(rmlvo, options[k], layout_ముసుగు);

        ఒకవేళ (స్థితి_2 == XKB_ERROR_కేటాయింపు_FAILURE)
            తిరిగి false;
    }

    తిరిగి true;
}

bool
xkb_rmlvo_builder_append_option(నిర్మాణం xkb_rmlvo_builder *rmlvo,
                                స్థిరరాశి అక్షరం *option)
{
    స్థిరరాశి గణన xkb_స్థితి స్థితి_2 = rmlvo_builder_append_option(
        rmlvo, option, (xkb_layout_ముసుగు_t)XKB_OPTION_LAYOUT_ముసుగు_GLOBAL
    );
    తిరిగి (స్థితి_2 == XKB_SUCCESS);
}

నిర్మాణం xkb_rmlvo_builder *
xkb_rmlvo_builder_ref(నిర్మాణం xkb_rmlvo_builder *rmlvo)
{
    ప్రతిజ్ఞా(rmlvo->refcnt > 0);
    rmlvo->refcnt++;
    తిరిగి rmlvo;
}

శూన్య
xkb_rmlvo_builder_unref(నిర్మాణం xkb_rmlvo_builder *rmlvo)
{
    ప్రతిజ్ఞా(!rmlvo || rmlvo->refcnt > 0);
    ఒకవేళ (!rmlvo || --rmlvo->refcnt > 0)
        తిరిగి;

    విడుదల(rmlvo->నియమాలు);
    విడుదల(rmlvo->model);

    స్థిరరాశి నిర్మాణం xkb_rmlvo_builder_layout *layout;
    darray_foreach(layout, rmlvo->layouts) {
        విడుదల(layout->layout);
        విడుదల(layout->variant);
    }
    darray_free(rmlvo->layouts);

    స్థిరరాశి నిర్మాణం xkb_rmlvo_builder_option *option;
    darray_foreach(option, rmlvo->options)
        విడుదల(option->option);
    darray_free(rmlvo->options);

    xkb_context_unref(rmlvo->ctx);
    విడుదల(rmlvo);
}

/* NOTE: the converse function, `xkb_rules_names_to_rmlvo_builder`, is currently
 * only used in tests */
bool
xkb_rmlvo_builder_గమ్యం_నియమాలు_పేర్లు(స్థిరరాశి నిర్మాణం xkb_rmlvo_builder *builder,
                                 నిర్మాణం xkb_నియమం_పేర్లు *rmlvo,
                                 అక్షరం *buf, కొలత_t buf_కొలత)
{
    rmlvo->నియమాలు = builder->నియమాలు;
    rmlvo->model = builder->model;

    అక్షరం *ప్రారంభం = buf;
    rmlvo->layout = ప్రారంభం;
    darray_కొలత_t k;
    స్థిరరాశి నిర్మాణం xkb_rmlvo_builder_layout *layout;
    darray_enumerate(k, layout, builder->layouts) {
        పూర్ణాంకం లెక్క = snprintf(ప్రారంభం, buf_కొలత, "%s%s",
                             (k > 0 ? "," : ""), layout->layout);
        ఒకవేళ (లెక్క < 0 || (కొలత_t)లెక్క >= buf_కొలత)
            తిరిగి false;
        buf_కొలత -= (కొలత_t)లెక్క;
        ప్రారంభం += లెక్క;
    }
    ఒకవేళ (buf_కొలత <= 1)
        తిరిగి false;
    *ప్రారంభం = '\0';
    ప్రారంభం++;
    buf_కొలత--;

    rmlvo->variant = ప్రారంభం;
    darray_enumerate(k, layout, builder->layouts) {
        పూర్ణాంకం లెక్క = snprintf(ప్రారంభం, buf_కొలత, "%s%s",
                             (k > 0 ? "," : ""),
                             (layout->variant ? layout->variant : ""));
        ఒకవేళ (లెక్క < 0 || (కొలత_t) లెక్క >= buf_కొలత)
            తిరిగి false;
        buf_కొలత -= (కొలత_t)లెక్క;
        ప్రారంభం += లెక్క;
    }
    ఒకవేళ (buf_కొలత <= 1)
        తిరిగి false;
    *ప్రారంభం = '\0';
    ప్రారంభం++;
    buf_కొలత--;

    rmlvo->options = ప్రారంభం;
    స్థిరరాశి నిర్మాణం xkb_rmlvo_builder_option *option;
    darray_enumerate(k, option, builder->options) {
        పూర్ణాంకం లెక్క = snprintf(ప్రారంభం, buf_కొలత, "%s%s",
                             (k > 0 ? "," : ""), option->option);
        ఒకవేళ (లెక్క < 0 || (కొలత_t) లెక్క >= buf_కొలత)
            తిరిగి false;
        buf_కొలత -= (కొలత_t)లెక్క;
        ప్రారంభం += లెక్క;
        ఒకవేళ (option->layouts != XKB_LAYOUT_INVALID) {
            లెక్క = snprintf(ప్రారంభం, buf_కొలత, "%c%"PRIu32,
                             OPTIONS_సమూహం_SPECIFIER_ఉపసర్గ, option->layouts);
            ఒకవేళ (లెక్క < 0 || (కొలత_t) లెక్క >= buf_కొలత)
                తిరిగి false;
            buf_కొలత -= (కొలత_t)లెక్క;
            ప్రారంభం += లెక్క;
        }
    }
    ఒకవేళ (buf_కొలత == 0)
        తిరిగి false;
    *ప్రారంభం = '\0';
    తిరిగి true;
}
