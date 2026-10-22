/*
 * For HPND:
 * Copyright (c) 1993 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

/*
 * This is a bastardised version of xkbActions.c from the X server which
 * does not support, for the moment:
 *   - AccessX sticky/debounce/etc (will come later)
 *   - pointer keys (may come later)
 *   - key redirects (unlikely)
 *   - messages (very unlikely)
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <limits.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "xkbcommon/xkbcommon-status.h"
#अन्तर्भाव "xkbcommon/xkbcommon-features.h"
#अन्तर्भाव "abi-check.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "features/enums.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "keysym.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "state-priv.h"
#अन्तर्भाव "utf8.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "util-mem.h"
#अन्तर्भाव "util-numbers.h"

/*
 * @struct xkb_events
 * Keyboard event collection object.
 *
 * Implements a dynamically-allocated array of [keyboard events](@ref xkb_event)
 * that is reset and refilled atomically on each `process_*` call, then consumed
 * sequentially by the caller via `xkb_events_next()`.
 *
 * The `queue` array is reused across calls to avoid repeated allocation:
 * it grows as needed but never shrinks. The `next` index is reset to 0
 * on each `process_*` call.
 *
 * @warning Not thread-safe. Must only be used from a single thread.
 *
 * @todo For multi-threaded use, we need a future `xkb_event` **queue** to
 * provide a thread-safe implementation (e.g. circular buffer).
 */
संरचना xkb_events {
    संरचना xkb_context *ctx;
    darray(संरचना xkb_event) queue;
    /**
     * Read cursor for `xkb_events_next()`.
     *
     * Reset to 0 on each `process_*` call.
     */
    darray_आकार_t अग्र;
};

संरचना xkb_server_state;

संरचना xkb_filter {
    सङ्घ xkb_action action;
    ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका;
    bool (*func)(संरचना xkb_server_state *state,
                 संरचना xkb_events *events,
                 संरचना xkb_filter *filter,
                 ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                 गणना xkb_कुञ्चिका_direction direction);
    uint32_t priv;
    पूर्णाङ्क refcnt;
};

गणना xkb_state_बहुलकम्_आन्तरिकम् {
    /** `xkb_state`: Client-only state (server API raises errors) */
    CLIENT_STATE = XKB_STATE_बहुलकम्_CLIENT,
    /** `xkb_state`: Server query companion */
    SERVER_COMPANION = XKB_STATE_बहुलकम्_SERVER_QUERY,
    /** `xkb_state`: Legacy unsafe client + server state */
    LEGACY_MIXED_STATE = 2,
    /** `xkb_state`: Legacy server-only state (client-only API raises errors) */
    LEGACY_SERVER_STATE = 3,
    /** `xkb_machine`: Server-only state */
    SERVER_STATE = 4,
    XKB_STATE_TYPE_अङ्कः_ENTRIES = 5
};

static_assert(CLIENT_STATE == 0 &&
              CLIENT_STATE == SERVER_COMPANION - 1 &&
              SERVER_COMPANION == LEGACY_MIXED_STATE - 1 &&
              LEGACY_MIXED_STATE == LEGACY_SERVER_STATE - 1 &&
              LEGACY_SERVER_STATE == SERVER_STATE - 1 &&
              SERVER_STATE == XKB_STATE_TYPE_अङ्कः_ENTRIES - 1 &&
              XKB_STATE_TYPE_अङ्कः_ENTRIES == 5,
              "Order is used to guard client/server -only APIs: do not change");

गणना { XKB_LAYOUT_सूचकांक_T_MIN_विस्तारः = 6 };
static_assert(XKB_MAX_GROUPS < (1 << XKB_LAYOUT_सूचकांक_T_MIN_विस्तारः),
              "Cannot encode out_of_range_group::redirect_group");

संरचना बहिर्गमनम्_सम्बन्धि_range_गणः {
    गणना xkb_layout_बहिर्गमनम्_सम्बन्धि_range_policy policy :
        (परिमाण(गणना xkb_layout_बहिर्गमनम्_सम्बन्धि_range_policy) * CHAR_BIT -
         XKB_LAYOUT_सूचकांक_T_MIN_विस्तारः);
    xkb_layout_सूचकांक_t redirect_गणः : XKB_LAYOUT_सूचकांक_T_MIN_विस्तारः;
};

गणना { XKB_STATE_बहुलकम्_आन्तरिकम्_MIN_विस्तारः = 4 /* 3 bits + sign */ };
static_assert(XKB_STATE_TYPE_अङ्कः_ENTRIES <
              (1 << XKB_STATE_बहुलकम्_आन्तरिकम्_MIN_विस्तारः),
              "Cannot encode xkb_state::mode");

/**
 * Common state
 *
 * `mode` indicates whether the object is casted from a `xkb_server_state` or a
 * `xkb_client_state` struct.
 */
संरचना xkb_state {
    संरचना xkb_keymap *keymap;
    /*
     * Before updating the state, we keep a copy of just this struct. This
     * allows us to report which components of the state have changed.
     */
    संरचना state_components components;
    संरचना बहिर्गमनम्_सम्बन्धि_range_गणः बहिर्गमनम्_सम्बन्धि_range_गणः;

    गणना xkb_state_बहुलकम्_आन्तरिकम् बहुलकम् : XKB_STATE_बहुलकम्_आन्तरिकम्_MIN_विस्तारः;
    पूर्णाङ्क refcnt : (परिमाण(पूर्णाङ्क) * CHAR_BIT - XKB_STATE_बहुलकम्_आन्तरिकम्_MIN_विस्तारः);
};

/**
 * Minimal client state
 *
 * Wrapper for strong typing
 */
संरचना xkb_client_state {
    /** Inherits from `xkb_state` */
    संरचना xkb_state आधारः;
};

गणना state_flags {
    STATE_NO_FLAGS = 0,
    STATE_REQUIRE_कुञ्चिका_EVENT = (1 << 0),
    STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_कुञ्चिका = (1 << 1),
    STATE_UNLOCK_MODS_ON_PRESS = (1 << 2),
};

/**
 * Legacy server state, used for `LEGACY_MIXED_STATE` and `LEGACY_SERVER_STATE`
 * modes.
 *
 * Casted as `xkb_state` in the public API. See `xkb_state::mode`.
 */
संरचना xkb_server_state {
    /** Inherits from `xkb_state` */
    संरचना xkb_state आधारः;

    /* Server-specific state data */

    darray(संरचना xkb_filter) filters;

    /**
     * Mix of constant configuration flags and
     * variable state-processing flags.
     */
    गणना state_flags flags;

    गणना xkb_a11y_flags a11y_flags;

    /*
     * At each event, we accumulate all the needed modifications to the base
     * modifiers, and apply them at the end. These keep track of this state.
     */
    xkb_mod_आवरणम्_t समुच्चयः_mods;
    xkb_mod_आवरणम्_t clear_mods;

    /*
     * We mustn't clear a base modifier if there's another depressed key
     * which affects it, e.g. given this sequence
     * < Left Shift down, Right Shift down, Left Shift Up >
     * the modifier should still be set. This keeps the count.
     */
    int16_t mod_कुञ्चिका_गणनम्[XKB_MAX_MODS];
};

स्थिर ध्रुव संरचना xkb_कुञ्चिका_type_प्रविष्टिः *
get_प्रविष्टिः_for_mods(ध्रुव संरचना xkb_कुञ्चिका_type *type, xkb_mod_आवरणम्_t mods)
{
    प्रति (darray_आकार_t i = 0; i < type->अङ्कः_entries; i++)
        यदि (प्रविष्टिः_is_सक्रिय(&type->entries[i]) &&
            type->entries[i].mods.आवरणम् == mods)
            प्रत्यागम &type->entries[i];
    प्रत्यागम रिक्त;
}

स्थिर ध्रुव संरचना xkb_कुञ्चिका_type_प्रविष्टिः *
get_प्रविष्टिः_for_कुञ्चिका_state(संरचना xkb_state *state, ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                        xkb_layout_सूचकांक_t गणः)
{
    ध्रुव संरचना xkb_कुञ्चिका_type* ध्रुव type = कुञ्चिका->groups[गणः].type;
    xkb_mod_आवरणम्_t सक्रिय_mods = state->components.mods & type->mods.आवरणम्;
    प्रत्यागम get_प्रविष्टिः_for_mods(type, सक्रिय_mods);
}

स्थिर अन्तःस्थ xkb_level_सूचकांक_t
state_कुञ्चिका_get_level(संरचना xkb_state *state, ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                         xkb_layout_सूचकांक_t layout)
{
    यदि (layout >= कुञ्चिका->अङ्कः_groups)
        प्रत्यागम XKB_LEVEL_INVALID;

    /* If we don't find an explicit match the default is 0. */
    ध्रुव संरचना xkb_कुञ्चिका_type_प्रविष्टिः* ध्रुव प्रविष्टिः =
        get_प्रविष्टिः_for_कुञ्चिका_state(state, कुञ्चिका, layout);

    प्रत्यागम (प्रविष्टिः) ? प्रविष्टिः->level : 0;
}

/**
 * Returns the level to use for the given key and state, or
 * XKB_LEVEL_INVALID.
 */
xkb_level_सूचकांक_t
xkb_state_कुञ्चिका_get_level(संरचना xkb_state *state, xkb_keycode_t kc,
                        xkb_layout_सूचकांक_t layout)
{
    ध्रुव संरचना xkb_कुञ्चिका* ध्रुव कुञ्चिका = Xkb_कुञ्चिका(state->keymap, kc);

    प्रत्यागम (कुञ्चिका)
        ? state_कुञ्चिका_get_level(state, कुञ्चिका, layout)
        : XKB_LEVEL_INVALID;
}

स्थिर अन्तःस्थ xkb_layout_सूचकांक_t
state_कुञ्चिका_get_layout(संरचना xkb_state *state, ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका)
{
    प्रत्यागम xkb_keymap_कुञ्चिका_effective_layout(कुञ्चिका, state->components.गणः);
}

/**
 * Returns the layout to use for the given key and state, taking
 * wrapping/clamping/etc into account, or XKB_LAYOUT_INVALID.
 */
xkb_layout_सूचकांक_t
xkb_state_कुञ्चिका_get_layout(संरचना xkb_state *state, xkb_keycode_t kc)
{
    ध्रुव संरचना xkb_कुञ्चिका* ध्रुव कुञ्चिका = Xkb_कुञ्चिका(state->keymap, kc);

    यदि (!कुञ्चिका)
        प्रत्यागम XKB_LAYOUT_INVALID;

    प्रत्यागम state_कुञ्चिका_get_layout(state, कुञ्चिका);
}

/* Empty action used for empty levels */
स्थिर ध्रुव सङ्घ xkb_action dummy_action = { .type = ACTION_TYPE_NONE };

स्थिर xkb_action_गणनम्_t
xkb_कुञ्चिका_get_actions(संरचना xkb_state *state, ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                    ध्रुव सङ्घ xkb_action **actions)
{
    ध्रुव xkb_layout_सूचकांक_t layout = state_कुञ्चिका_get_layout(state, कुञ्चिका);
    ध्रुव xkb_level_सूचकांक_t level = state_कुञ्चिका_get_level(state, कुञ्चिका, layout);
    यदि (level == XKB_LEVEL_INVALID)
        गच्छ err;

    ध्रुव xkb_action_गणनम्_t गणनम् =
        xkb_keymap_कुञ्चिका_get_actions_by_level(state->keymap, कुञ्चिका,
                                            layout, level, actions);
    यदि (!गणनम्)
        गच्छ err;

    प्रत्यागम गणनम्;

err:
    /* Use a dummy action if no corresponding level was found or if it is empty.
     * This is required e.g. to handle latches properly. */
    *actions = &dummy_action;
    प्रत्यागम 1;
}

स्थिर संरचना xkb_filter *
xkb_filter_new(संरचना xkb_server_state *state)
{
    संरचना xkb_filter *filter = रिक्त, *iter;

    darray_foreach(iter, state->filters) {
        यदि (iter->func)
            अनुवर्त;
        /* Use available slot */
        filter = iter;
        भङ्ग;
    }

    यदि (!filter) {
        /* No available slot: resize the filters array */
        darray_resize0(state->filters, darray_आकार(state->filters) + 1);
        filter = &darray_item(state->filters, darray_आकार(state->filters) -1);
    }

    filter->refcnt = 1;
    प्रत्यागम filter;
}

/***====================================================================***/

गणना xkb_filter_परिणाम {
    /*
     * The event is consumed by the filters.
     *
     * An event is always processed by all filters, but any filter can
     * prevent it from being processed further by consuming it.
     */
    XKB_FILTER_CONSUME,
    /*
     * The event may continue to be processed as far as this filter is
     * concerned.
     */
    XKB_FILTER_CONTINUE,
};

/* Modify a group component, depending on the ACTION_ABSOLUTE_SWITCH flag */
#परिभाषय apply_गणः_delta(filter_, state_, component_)                   \
    यदि ((filter_)->action.गणः.flags & ACTION_ABSOLUTE_SWITCH)          \
        (state_)->components.component_ = (filter_)->action.गणः.गणः; \
    अन्यथा                                                                 \
        (state_)->components.component_ += (filter_)->action.गणः.गणः

स्थिर शून्य
xkb_filter_गणः_समुच्चयः_new(संरचना xkb_server_state *state,
                         संरचना xkb_events *events,
                         संरचना xkb_filter *filter)
{
    static_assert(परिमाण(state->आधारः.components.आधारः_गणः) ==
                  परिमाण(filter->priv),
                  "Max groups don't fit");
    filter->priv = (uint32_t)state->आधारः.components.आधारः_गणः;
    apply_गणः_delta(filter, &state->आधारः, आधारः_गणः);
}

स्थिर bool
xkb_filter_गणः_समुच्चयः_func(संरचना xkb_server_state *state,
                          संरचना xkb_events *events,
                          संरचना xkb_filter *filter,
                          ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                          गणना xkb_कुञ्चिका_direction direction)
{
    यदि (कुञ्चिका != filter->कुञ्चिका) {
        filter->action.गणः.flags &= ~ACTION_तालकम्_CLEAR;
        प्रत्यागम XKB_FILTER_CONTINUE;
    }

    विकल्प (direction) {
    पक्ष XKB_कुञ्चिका_अधस्तात्:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_कुञ्चिका_REPEATED:
        प्रत्यागम XKB_FILTER_CONSUME;
    स्वभाव:
        यदि (--filter->refcnt > 0)
            प्रत्यागम XKB_FILTER_CONSUME;
    }

    static_assert(परिमाण(state->आधारः.components.आधारः_गणः) == परिमाण(filter->priv),
                  "Max groups don't fit");
    state->आधारः.components.आधारः_गणः = (int32_t) filter->priv;

    यदि (filter->action.गणः.flags & ACTION_तालकम्_CLEAR)
        state->आधारः.components.locked_गणः = 0;

    filter->func = रिक्त;
    प्रत्यागम XKB_FILTER_CONTINUE;
}

स्थिर गणना xkb_state_component
get_state_component_changes(ध्रुव संरचना state_components *a,
                            ध्रुव संरचना state_components *b)
{
    गणना xkb_state_component आवरणम् = 0;

    यदि (a->गणः != b->गणः)
        आवरणम् |= XKB_STATE_LAYOUT_EFFECTIVE;
    यदि (a->आधारः_गणः != b->आधारः_गणः)
        आवरणम् |= XKB_STATE_LAYOUT_DEPRESSED;
    यदि (a->latched_गणः != b->latched_गणः)
        आवरणम् |= XKB_STATE_LAYOUT_LATCHED;
    यदि (a->locked_गणः != b->locked_गणः)
        आवरणम् |= XKB_STATE_LAYOUT_LOCKED;
    यदि (a->mods != b->mods)
        आवरणम् |= XKB_STATE_MODS_EFFECTIVE;
    यदि (a->आधारः_mods != b->आधारः_mods)
        आवरणम् |= XKB_STATE_MODS_DEPRESSED;
    यदि (a->latched_mods != b->latched_mods)
        आवरणम् |= XKB_STATE_MODS_LATCHED;
    यदि (a->locked_mods != b->locked_mods)
        आवरणम् |= XKB_STATE_MODS_LOCKED;
    यदि (a->leds != b->leds)
        आवरणम् |= XKB_STATE_LEDS;
    यदि (a->controls != b->controls)
        आवरणम् |= XKB_STATE_CONTROLS_EFFECTIVE;

    प्रत्यागम आवरणम्;
}

स्थिर शून्य
xkb_filter_गणः_तालकम्_new(संरचना xkb_server_state *state,
                          संरचना xkb_events *events,
                          संरचना xkb_filter *filter)
{
    यदि (filter->action.गणः.flags & ACTION_तालकम्_ON_RELEASE) {
        /*
         * Lock on key release: do nothing on key press.
         *
         * This is a keymap format v2 extension.
         */
        प्रत्यागम;
    } अन्यथा {
        /* Lock on key press */
        apply_गणः_delta(filter, &state->आधारः, locked_गणः);
    }
}

स्थिर bool
xkb_filter_गणः_तालकम्_func(संरचना xkb_server_state *state,
                           संरचना xkb_events *events,
                           संरचना xkb_filter *filter,
                           ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                           गणना xkb_कुञ्चिका_direction direction)
{
    यदि (कुञ्चिका != filter->कुञ्चिका) {
        यदि ((filter->action.गणः.flags & ACTION_तालकम्_ON_RELEASE) &&
            direction == XKB_कुञ्चिका_अधस्तात्) {
            /*
             * Another key has been pressed after the locking key:
             * cancel group lock on release.
             *
             * This is a keymap v2 extension.
             */
            filter->action.गणः.flags &= ~ACTION_तालकम्_ON_RELEASE;
        }
        प्रत्यागम XKB_FILTER_CONTINUE;
    }

    विकल्प (direction) {
    पक्ष XKB_कुञ्चिका_अधस्तात्:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_कुञ्चिका_REPEATED:
        प्रत्यागम XKB_FILTER_CONSUME;
    स्वभाव:
        यदि (--filter->refcnt > 0)
            प्रत्यागम XKB_FILTER_CONSUME;
    }

    यदि (filter->action.गणः.flags & ACTION_तालकम्_ON_RELEASE) {
        /*
         * Lock on key release
         *
         * This is a keymap v2 extension.
         */
        apply_गणः_delta(filter, &state->आधारः, locked_गणः);
    } अन्यथा {
        /* Lock on key press: do nothing on key release. */
    }
    filter->func = रिक्त;
    प्रत्यागम XKB_FILTER_CONTINUE;
}

स्थिर bool
xkb_action_breaks_latch(ध्रुव सङ्घ xkb_action *action,
                        गणना xkb_action_controls controls,
                        गणना xkb_आन्तरिकम्_action_flags flag,
                        xkb_mod_आवरणम्_t आवरणम्)
{
    विकल्प (action->type) {
    पक्ष ACTION_TYPE_NONE:
    पक्ष ACTION_TYPE_VOID:
    पक्ष ACTION_TYPE_PTR_BUTTON:
    पक्ष ACTION_TYPE_PTR_तालकम्:
    पक्ष ACTION_TYPE_CTRL_समुच्चयः:
    पक्ष ACTION_TYPE_CTRL_तालकम्:
    पक्ष ACTION_TYPE_SWITCH_VT:
    पक्ष ACTION_TYPE_TERMINATE:
    पक्ष ACTION_TYPE_REDIRECT_कुञ्चिका:
        प्रत्यागम true;
    पक्ष ACTION_TYPE_PTR_MOVE:
    पक्ष ACTION_TYPE_PTR_DEFAULT:
        प्रत्यागम !(controls & CONTROL_MOUSE_कुञ्चिकाः);
    पक्ष ACTION_TYPE_आन्तरिकम्:
        प्रत्यागम (action->आन्तरिकम्.flags & flag) &&
               ((action->आन्तरिकम्.clear_latched_mods & आवरणम्) == आवरणम्);
    स्वभाव:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_आन्तरिकम् == 20 &&
                      ACTION_TYPE_आन्तरिकम् + 1 == ACTION_TYPE_अङ्कः_ENTRIES,
                      "Missing action type");
        प्रत्यागम false;
    }
}

गणना xkb_कुञ्चिका_latch_state {
    NO_LATCH = 0,
    LATCH_कुञ्चिका_अधस्तात्,
    LATCH_PENDING,
    कुञ्चिका_LATCH_STATE_अङ्कः_ENTRIES
};

#परिभाषय MAX_XKB_कुञ्चिका_LATCH_STATE_LOG2 2
#यदि (कुञ्चिका_LATCH_STATE_अङ्कः_ENTRIES > (1 << MAX_XKB_कुञ्चिका_LATCH_STATE_LOG2)) || \
    (-XKB_MAX_GROUPS) < (INT32_MIN >> MAX_XKB_कुञ्चिका_LATCH_STATE_LOG2) || \
      XKB_MAX_GROUPS > (INT32_MAX >> MAX_XKB_कुञ्चिका_LATCH_STATE_LOG2)
#दोषः "Cannot represent priv field of the group latch filter"
#यदि_अन्तः

/* Hold the latch state *and* the group delta */
सङ्घ गणः_latch_priv {
    uint32_t priv;
    संरचना {
        /* The type is really: enum xkb_key_latch_state, but it is problematic
         * on Windows, because it is interpreted as signed and leads to wrong
         * negative values. */
        अचिह्नित पूर्णाङ्क latch:MAX_XKB_कुञ्चिका_LATCH_STATE_LOG2;
        int32_t गणः_delta:(32 - MAX_XKB_कुञ्चिका_LATCH_STATE_LOG2);
    };
};

स्थिर शून्य
xkb_filter_गणः_latch_new(संरचना xkb_server_state *state,
                           संरचना xkb_events *events,
                           संरचना xkb_filter *filter)
{
    ध्रुव सङ्घ गणः_latch_priv priv = {
        .latch = LATCH_कुञ्चिका_अधस्तात्,
        .गणः_delta = (filter->action.गणः.flags & ACTION_ABSOLUTE_SWITCH)
            ? filter->action.गणः.गणः - state->आधारः.components.आधारः_गणः
            : filter->action.गणः.गणः
    };
    filter->priv = priv.priv;
    /* Like group set */
    apply_गणः_delta(filter, &state->आधारः, आधारः_गणः);
}

स्थिर bool
xkb_filter_गणः_latch_func(संरचना xkb_server_state *state,
                            संरचना xkb_events *events,
                            संरचना xkb_filter *filter,
                            ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                            गणना xkb_कुञ्चिका_direction direction)
{
    सङ्घ गणः_latch_priv priv = {.priv = filter->priv};
    गणना xkb_कुञ्चिका_latch_state latch = priv.latch;

    यदि (direction == XKB_कुञ्चिका_अधस्तात्) {
        ध्रुव सङ्घ xkb_action *actions = रिक्त;
        ध्रुव xkb_action_गणनम्_t गणनम् = xkb_कुञ्चिका_get_actions(&state->आधारः, कुञ्चिका,
                                                             &actions);

        यदि (latch == LATCH_कुञ्चिका_अधस्तात्) {
            /*
             * Another key was pressed while we’ve still got the latching key
             * held down.
             *
             * The exact behavior depends on the accessibility flag:
             * XKB_A11Y_LATCH_SIMULTANEOUS_KEYS.
             *
             * It results in either:
             * • No change.
             * • Prevent the latch to trigger and keep the base group set by
             *   xkb_filter_group_latch_new(), until the latch key is
             *   released.
             */
            यदि (state->a11y_flags & XKB_A11_Y_LATCH_युगपत्_कुञ्चिकाः) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                प्रति (xkb_action_गणनम्_t k = 0; k < गणनम्; k++) {
                    यदि (xkb_action_breaks_latch(&(actions[k]),
                                                state->आधारः.components.controls,
                                                आन्तरिकम्_BREAKS_गणः_LATCH,
                                                0))
                    {
                        latch = NO_LATCH;
                        भङ्ग;
                    }
                }
            } अन्यथा {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        अन्यथा यदि (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is
             * pressed, or promote it to a lock if it's the same group delta &
             * flags and latchToLock option is enabled. */
            ध्रुव bool sticky_कुञ्चिकाः = state->आधारः.components.controls
                                   & CONTROL_STICKY_कुञ्चिकाः;
            /*
             * NOTE: Action’s flags other than `ACTION_ABSOLUTE_SWITCH` are
             * not compared:
             * - flags may have been modified by keyboard controls at the
             *   creation of the filter;
             * - it is unlikely to have two LatchGroup() actions on the same
             *   key that differ only by their flags;
             * - even if it were the case, one would most likely want both
             *   actions to be considered compatible in this context.
             */
            प्रति (xkb_action_गणनम्_t k = 0; k < गणनम्; k++) {
                यदि (((actions[k].type == ACTION_TYPE_गणः_LATCH) ||
                     (actions[k].type == ACTION_TYPE_गणः_समुच्चयः && sticky_कुञ्चिकाः)) &&
                     actions[k].गणः.गणः == filter->action.गणः.गणः &&
                     (actions[k].गणः.flags & ACTION_ABSOLUTE_SWITCH) ==
                     (filter->action.गणः.flags & ACTION_ABSOLUTE_SWITCH)) {
                    यदि (filter->action.गणः.flags & ACTION_LATCH_गन्तव्यम्_तालकम् &&
                        filter->action.गणः.गणः != 0)
                    {
                        /* Promote to lock */
                        filter->action.type = ACTION_TYPE_गणः_तालकम्;
                        filter->func = xkb_filter_गणः_तालकम्_func;
                        xkb_filter_गणः_तालकम्_new(state, events, filter);
                        state->आधारः.components.latched_गणः -= priv.गणः_delta;
                        filter->कुञ्चिका = कुञ्चिका;
                        /* XXX beep beep! */
                        प्रत्यागम XKB_FILTER_CONSUME;
                    }
                    /* Do nothing if `latchToLock` option is not activated; if
                     * the latch is not broken by the following actions and the
                     * key is not consumed, then another latch filter will be
                     * created.
                     */
                    अनुवर्त;
                }
                अन्यथा यदि (xkb_action_breaks_latch(&(actions[k]),
                                                 state->आधारः.components.controls,
                                                 आन्तरिकम्_BREAKS_गणः_LATCH,
                                                 0))
                {
                    /* Breaks the latch */
                    state->आधारः.components.latched_गणः -= priv.गणः_delta;
                    filter->func = रिक्त;
                    प्रत्यागम XKB_FILTER_CONTINUE;
                }
            }
        }
        अन्यथा {
            /* Ignore press in NO_LATCH state */
            प्रतिज्ञा(latch == NO_LATCH);
        }
    }
    अन्यथा यदि (कुञ्चिका != filter->कुञ्चिका) {
        /* Ignore repeat/release of other keys */
    }
    अन्यथा यदि (direction == XKB_कुञ्चिका_REPEATED) {
        प्रत्यागम XKB_FILTER_CONSUME; // TODO: check
    }
    अन्यथा {
        /* Our key got released.  If we've set it to clear locks, and we
         * currently have a group locked, then release it and
         * don't actually latch.  Else we've actually hit the latching
         * stage, so set PENDING and move our group from base to
         * latched. */
        यदि ((filter->action.गणः.flags & ACTION_तालकम्_CLEAR) &&
             state->आधारः.components.locked_गणः) {
            यदि (latch == LATCH_PENDING)
                state->आधारः.components.latched_गणः -= priv.गणः_delta;
            अन्यथा
                state->आधारः.components.आधारः_गणः -= priv.गणः_delta;
            state->आधारः.components.locked_गणः = 0;
            filter->func = रिक्त;
        }
        /* Broken latch */
        अन्यथा यदि (latch == NO_LATCH) {
            state->आधारः.components.आधारः_गणः -= priv.गणः_delta;
            filter->func = रिक्त;
        }
        /* We may already have reached the latch state if pressing the
         * key multiple times without latch-to-lock enabled. */
        अन्यथा यदि (latch == LATCH_कुञ्चिका_अधस्तात्) {
            latch = LATCH_PENDING;
            /* Switch from set to latch */
            state->आधारः.components.आधारः_गणः -= priv.गणः_delta;
            state->आधारः.components.latched_गणः += priv.गणः_delta;
            /* XXX beep beep! */
        }
    }

    priv.latch = latch;
    filter->priv = priv.priv;

    प्रत्यागम XKB_FILTER_CONTINUE;
}

स्थिर शून्य
xkb_filter_mod_समुच्चयः_new(संरचना xkb_server_state *state,
                       संरचना xkb_events *events,
                       संरचना xkb_filter *filter)
{
    ध्रुव गणना xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_तालकम्_CLEAR;
    यदि ((filter->action.mods.flags & unlock) == unlock) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension.
         */
        filter->priv = filter->action.mods.mods.आवरणम्
                     & ~state->आधारः.components.locked_mods;
        state->आधारः.components.locked_mods &= ~filter->action.mods.mods.आवरणम्;
    } अन्यथा {
        filter->priv = filter->action.mods.mods.आवरणम्;
    }

    state->समुच्चयः_mods |= (xkb_mod_आवरणम्_t) filter->priv;
}

स्थिर bool
xkb_filter_mod_समुच्चयः_func(संरचना xkb_server_state *state,
                        संरचना xkb_events *events,
                        संरचना xkb_filter *filter,
                        ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                        गणना xkb_कुञ्चिका_direction direction)
{

    यदि (कुञ्चिका != filter->कुञ्चिका) {
        filter->action.mods.flags &= ~ACTION_तालकम्_CLEAR;
        प्रत्यागम XKB_FILTER_CONTINUE;
    }

    विकल्प (direction) {
    पक्ष XKB_कुञ्चिका_अधस्तात्:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_कुञ्चिका_REPEATED:
        प्रत्यागम XKB_FILTER_CONSUME;
    स्वभाव:
        यदि (--filter->refcnt > 0)
            प्रत्यागम XKB_FILTER_CONSUME;
    }

    state->clear_mods |= (xkb_mod_आवरणम्_t) filter->priv;
    ध्रुव गणना xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_तालकम्_CLEAR;
    यदि ((filter->action.mods.flags & unlock) == ACTION_तालकम्_CLEAR)
        state->आधारः.components.locked_mods &= ~filter->action.mods.mods.आवरणम्;

    filter->func = रिक्त;
    प्रत्यागम XKB_FILTER_CONTINUE;
}

स्थिर शून्य
xkb_filter_mod_तालकम्_new(संरचना xkb_server_state *state,
                        संरचना xkb_events *events,
                        संरचना xkb_filter *filter)
{
    filter->priv = (state->आधारः.components.locked_mods &
                    filter->action.mods.mods.आवरणम्);

    यदि (filter->priv && (filter->action.mods.flags & ACTION_UNLOCK_ON_PRESS)) {
        /*
         * Some of the target modifiers were locked before key press: unlock.
         *
         * This is a keymap v2 extension: unlock-on-press.
         */
        यदि (!(filter->action.mods.flags & ACTION_तालकम्_NO_UNLOCK))
            state->आधारः.components.locked_mods &= ~filter->priv;
        /* No further action: cancel filter */
        filter->func = रिक्त;
    } अन्यथा {
        /* Set base mods; lock mods if relevant (XKB 1.0 spec) */
        state->समुच्चयः_mods |= filter->action.mods.mods.आवरणम्;
        यदि (!(filter->action.mods.flags & ACTION_तालकम्_NO_तालकम्))
            state->आधारः.components.locked_mods |= filter->action.mods.mods.आवरणम्;
    }
}

स्थिर bool
xkb_filter_mod_तालकम्_func(संरचना xkb_server_state *state,
                         संरचना xkb_events *events,
                         संरचना xkb_filter *filter,
                         ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                         गणना xkb_कुञ्चिका_direction direction)
{
    यदि (कुञ्चिका != filter->कुञ्चिका)
        प्रत्यागम XKB_FILTER_CONTINUE;

    विकल्प (direction) {
    पक्ष XKB_कुञ्चिका_अधस्तात्:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_कुञ्चिका_REPEATED:
        प्रत्यागम XKB_FILTER_CONSUME;
    स्वभाव:
        यदि (--filter->refcnt > 0)
            प्रत्यागम XKB_FILTER_CONSUME;
    }

    state->clear_mods |= filter->action.mods.mods.आवरणम्;
    यदि (!(filter->action.mods.flags & ACTION_तालकम्_NO_UNLOCK))
        state->आधारः.components.locked_mods &= ~filter->priv;

    filter->func = रिक्त;
    प्रत्यागम XKB_FILTER_CONTINUE;
}

स्थिर शून्य
xkb_filter_mod_latch_new(संरचना xkb_server_state *state,
                         संरचना xkb_events *events,
                         संरचना xkb_filter *filter)
{
    /* Latch-on-press + clear-locks imply unlock-on-press */
    ध्रुव गणना xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                              | ACTION_LATCH_ON_PRESS;

    यदि ((filter->action.mods.flags & ACTION_तालकम्_CLEAR) &&
        (filter->action.mods.flags & unlockOnPress) &&
        (state->आधारः.components.locked_mods & filter->action.mods.mods.आवरणम्) ==
         filter->action.mods.mods.आवरणम्) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension: clear locks and do not latch.
         */
        state->आधारः.components.locked_mods &= ~filter->action.mods.mods.आवरणम्;
        filter->func = रिक्त;
    } अन्यथा यदि (filter->action.mods.flags & ACTION_LATCH_ON_PRESS) {
        /*
         * Latch on key press
         *
         * This is a keymap format v2 extension.
         */
        filter->priv = LATCH_PENDING;
        state->आधारः.components.latched_mods |= filter->action.mods.mods.आवरणम्;
        /* XXX beep beep! */
    } अन्यथा {
        /* XKB standard latch action */
        filter->priv = LATCH_कुञ्चिका_अधस्तात्;
        state->समुच्चयः_mods |= filter->action.mods.mods.आवरणम्;
    }
}

स्थिर bool
xkb_filter_mod_latch_func(संरचना xkb_server_state *state,
                          संरचना xkb_events *events,
                          संरचना xkb_filter *filter,
                          ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                          गणना xkb_कुञ्चिका_direction direction)
{
    गणना xkb_कुञ्चिका_latch_state latch = filter->priv;

    यदि (direction == XKB_कुञ्चिका_अधस्तात्) {
        ध्रुव सङ्घ xkb_action *actions = रिक्त;
        ध्रुव xkb_action_गणनम्_t गणनम् = xkb_कुञ्चिका_get_actions(&state->आधारः, कुञ्चिका,
                                                             &actions);

        यदि (latch == LATCH_कुञ्चिका_अधस्तात्) {
            /*
             * Another key was pressed while we’ve still got the latching key
             * held down.
             *
             * The exact behavior depends on the accessibility flag:
             * XKB_A11Y_LATCH_SIMULTANEOUS_KEYS.
             *
             * It results in either:
             * • No change.
             * • Prevent the latch to trigger and keep the base modifiers set
             *   by xkb_filter_mod_latch_new(), until the latch key is released.
             */
            यदि (state->a11y_flags & XKB_A11_Y_LATCH_युगपत्_कुञ्चिकाः) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                प्रति (xkb_action_गणनम्_t k = 0; k < गणनम्; k++) {
                    यदि (xkb_action_breaks_latch(&(actions[k]),
                                                state->आधारः.components.controls,
                                                आन्तरिकम्_BREAKS_MOD_LATCH,
                                                filter->action.mods.mods.आवरणम्))
                    {
                        latch = NO_LATCH;
                        भङ्ग;
                    }
                }
            } अन्यथा {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        अन्यथा यदि (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is pressed,
             * or promote it to a lock or plain base set if it's the same
             * modifier. */
            ध्रुव bool sticky_कुञ्चिकाः = state->आधारः.components.controls
                                   & CONTROL_STICKY_कुञ्चिकाः;
            /*
             * NOTE: Action’s flags are not compared:
             * - flags may have been modified by keyboard controls at the
             *   creation of the filter;
             * - it is unlikely to have two LatchMods() actions on the same
             *   key that differ only by their flags;
             * - even if it were the case, one would most likely want both
             *   actions to be considered compatible in this context.
             */
            प्रति (xkb_action_गणनम्_t k = 0; k < गणनम्; k++) {
                यदि (((actions[k].type == ACTION_TYPE_MOD_LATCH) ||
                     (actions[k].type == ACTION_TYPE_MOD_समुच्चयः && sticky_कुञ्चिकाः)) &&
                    actions[k].mods.mods.आवरणम् == filter->action.mods.mods.आवरणम्)
                {
                    यदि (filter->action.mods.flags & ACTION_LATCH_गन्तव्यम्_तालकम्) {
                        /* Mutate the action to LockMods() */
                        filter->action.type = ACTION_TYPE_MOD_तालकम्;
                        filter->func = xkb_filter_mod_तालकम्_func;
                        xkb_filter_mod_तालकम्_new(state, events, filter);
                    }
                    अन्यथा {
                        /* Mutate the action to SetMods() */
                        filter->action.type = ACTION_TYPE_MOD_समुच्चयः;
                        filter->func = xkb_filter_mod_समुच्चयः_func;
                        xkb_filter_mod_समुच्चयः_new(state, events, filter);
                    }
                    filter->कुञ्चिका = कुञ्चिका;
                    /* Clear latches */
                    state->आधारः.components.latched_mods &=
                        ~filter->action.mods.mods.आवरणम्;
                    /* XXX beep beep! */
                    प्रत्यागम XKB_FILTER_CONSUME;
                }
                अन्यथा यदि (xkb_action_breaks_latch(&(actions[k]),
                                                 state->आधारः.components.controls,
                                                 आन्तरिकम्_BREAKS_MOD_LATCH,
                                                 filter->action.mods.mods.आवरणम्))
                {
                    /* XXX: This may be totally broken, we might need to break the
                     *      latch in the next run after this press? */
                    state->आधारः.components.latched_mods &=
                        ~filter->action.mods.mods.आवरणम्;
                    filter->func = रिक्त;
                    प्रत्यागम XKB_FILTER_CONTINUE;
                }
            }
        }
        अन्यथा {
            /* Ignore press in NO_LATCH state */
            प्रतिज्ञा(latch == NO_LATCH);
        }
    }
    अन्यथा यदि (कुञ्चिका != filter->कुञ्चिका) {
        /* Ignore repeat/release of other keys */
    }
    अन्यथा यदि (direction == XKB_कुञ्चिका_REPEATED) {
        प्रत्यागम XKB_FILTER_CONSUME; // TODO: check
    }
    अन्यथा {
        /* Our key got released.  If we’ve set it to clear locks, and we
         * currently have the same modifiers locked, then release them and
         * don't actually latch.  Else we’ve actually hit the latching
         * stage, so set PENDING and move our modifier from base to
         * latched. */

        /* Latch-on-press + clear-locks imply unlock-on-press */
        ध्रुव गणना xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                                  | ACTION_LATCH_ON_PRESS;

        यदि ((filter->action.mods.flags & ACTION_तालकम्_CLEAR) &&
            !(filter->action.mods.flags & unlockOnPress) &&
            (state->आधारः.components.locked_mods &
             filter->action.mods.mods.आवरणम्) == filter->action.mods.mods.आवरणम्)
        {
            /* XXX: We might be a bit overenthusiastic about clearing
             *      mods other filters have set here? */
            यदि (latch == LATCH_PENDING)
                state->आधारः.components.latched_mods &=
                    ~filter->action.mods.mods.आवरणम्;
            अन्यथा
                state->clear_mods |= filter->action.mods.mods.आवरणम्;
            state->आधारः.components.locked_mods &= ~filter->action.mods.mods.आवरणम्;
            filter->func = रिक्त;
        }
        अन्यथा यदि (latch == NO_LATCH) {
            /* Broken latch */
            state->clear_mods |= filter->action.mods.mods.आवरणम्;
            filter->func = रिक्त;
        }
        अन्यथा यदि (!(filter->action.mods.flags & ACTION_LATCH_ON_PRESS)) {
            latch = LATCH_PENDING;
            state->clear_mods |= filter->action.mods.mods.आवरणम्;
            state->आधारः.components.latched_mods |= filter->action.mods.mods.आवरणम्;
            /* XXX beep beep! */
        }
    }

    filter->priv = latch;

    प्रत्यागम XKB_FILTER_CONTINUE;
}

स्थिर शून्य
append_pointer_move(संरचना xkb_events *events,
                    संरचना xkb_filter *filter)
{
    गणना xkb_pointer_motion_flags flags = 0;
    यदि (filter->action.ptr.flags & ACTION_ABSOLUTE_X)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_X;
    यदि (filter->action.ptr.flags & ACTION_ABSOLUTE_Y)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_Y;
    यदि (filter->action.ptr.flags & ACTION_REPEAT)
        flags |= XKB_POINTER_MOTION_REPEATS;
    darray_append(events->queue, (संरचना xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_MOTION,
        .pointer_motion = {
            .आकार = परिमाण(((संरचना xkb_event*)0)->pointer_motion),
            .flags = flags,
            .x = filter->action.ptr.x,
            .y = filter->action.ptr.y,
        }
    });
}

स्थिर शून्य
xkb_filter_pointer_move_new(संरचना xkb_server_state *state,
                            संरचना xkb_events *events,
                            संरचना xkb_filter *filter)
{
    यदि (!events) {
        filter->func = रिक्त;
        प्रत्यागम;
    }

    प्रतिज्ञा(state->आधारः.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_कुञ्चिकाः);
    append_pointer_move(events, filter);
    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;
}

स्थिर bool
xkb_filter_pointer_move_func(संरचना xkb_server_state *state,
                             संरचना xkb_events *events,
                             संरचना xkb_filter *filter,
                             ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                             गणना xkb_कुञ्चिका_direction direction)
{
    यदि (कुञ्चिका != filter->कुञ्चिका || !events)
        प्रत्यागम XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;

    विकल्प (direction) {
    पक्ष XKB_कुञ्चिका_अधस्तात्:
        filter->refcnt++;
        append_pointer_move(events, filter);
        प्रत्यागम XKB_FILTER_CONSUME;
    पक्ष XKB_कुञ्चिका_REPEATED:
        यदि (filter->action.ptr.flags & ACTION_REPEAT)
            append_pointer_move(events, filter);
        प्रत्यागम XKB_FILTER_CONSUME;
    स्वभाव:
        यदि (--filter->refcnt > 0)
            प्रत्यागम XKB_FILTER_CONSUME;
    }

    filter->func = रिक्त;
    प्रत्यागम XKB_FILTER_CONSUME;
}

/* Implemented with xkb_machine API because it requires struct xkb_machine */
स्थिर शून्य
xkb_filter_pointer_button_new(संरचना xkb_server_state *state,
                              संरचना xkb_events *events,
                              संरचना xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
स्थिर bool
xkb_filter_pointer_button_func(संरचना xkb_server_state *state,
                               संरचना xkb_events *events,
                               संरचना xkb_filter *filter,
                               ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                               गणना xkb_कुञ्चिका_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
स्थिर शून्य
xkb_filter_pointer_तालकम्_button_new(संरचना xkb_server_state *state,
                                   संरचना xkb_events *events,
                                   संरचना xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
स्थिर bool
xkb_filter_pointer_तालकम्_button_func(संरचना xkb_server_state *state,
                                    संरचना xkb_events *events,
                                    संरचना xkb_filter *filter,
                                    ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                                    गणना xkb_कुञ्चिका_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
स्थिर शून्य
xkb_filter_pointer_समुच्चयः_default_button_new(संरचना xkb_server_state *state,
                                          संरचना xkb_events *events,
                                          संरचना xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
स्थिर bool
xkb_filter_pointer_समुच्चयः_default_button_func(संरचना xkb_server_state *state,
                                           संरचना xkb_events *events,
                                           संरचना xkb_filter *filter,
                                           ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                                           गणना xkb_कुञ्चिका_direction direction);

स्थिर शून्य
xkb_filter_terminate_server_new(संरचना xkb_server_state *state,
                                 संरचना xkb_events *events,
                                 संरचना xkb_filter *filter);

स्थिर bool
xkb_filter_terminate_server_func(संरचना xkb_server_state *state,
                                  संरचना xkb_events *events,
                                  संरचना xkb_filter *filter,
                                  ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                                  गणना xkb_कुञ्चिका_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
स्थिर शून्य
xkb_filter_switch_virtual_console_new(संरचना xkb_server_state *state,
                                      संरचना xkb_events *events,
                                      संरचना xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
स्थिर bool
xkb_filter_switch_virtual_console_func(संरचना xkb_server_state *state,
                                       संरचना xkb_events *events,
                                       संरचना xkb_filter *filter,
                                       ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                                       गणना xkb_कुञ्चिका_direction direction);

स्थिर अन्तःस्थ शून्य
clear_all_latches_and_locks(संरचना xkb_server_state *state,
                            संरचना xkb_events *events);

स्थिर शून्य
xkb_filter_ctrls_new(संरचना xkb_server_state *state,
                     संरचना xkb_events *events,
                     संरचना xkb_filter *filter)
{
    यदि (filter->action.type == ACTION_TYPE_CTRL_समुच्चयः) {
        /* Set: save the specified controls that are *not* already enabled */
        filter->priv = (uint32_t) (
            (~state->आधारः.components.controls) & filter->action.ctrls.ctrls
        );
    } अन्यथा {
        /* Lock: save the specified controls that *are* already enabled */
        filter->priv = (uint32_t) (
            state->आधारः.components.controls & filter->action.ctrls.ctrls
        );
    }

    यदि (filter->action.type == ACTION_TYPE_CTRL_समुच्चयः ||
        !(filter->action.ctrls.flags & ACTION_तालकम्_NO_तालकम्)) {
        /* Enable the specified controls that are not already enabled */
        state->आधारः.components.controls |= filter->action.ctrls.ctrls;
    }
}

स्थिर bool
xkb_filter_ctrls_func(संरचना xkb_server_state *state,
                      संरचना xkb_events *events,
                      संरचना xkb_filter *filter,
                      ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                      गणना xkb_कुञ्चिका_direction direction)
{
    यदि (कुञ्चिका != filter->कुञ्चिका)
        प्रत्यागम XKB_FILTER_CONTINUE;

    विकल्प (direction) {
    पक्ष XKB_कुञ्चिका_अधस्तात्:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_कुञ्चिका_REPEATED:
        प्रत्यागम XKB_FILTER_CONSUME;
    स्वभाव:
        यदि (--filter->refcnt > 0)
            प्रत्यागम XKB_FILTER_CONSUME;
    }

    यदि (filter->action.type == ACTION_TYPE_CTRL_समुच्चयः ||
        !(filter->action.ctrls.flags & ACTION_तालकम्_NO_UNLOCK)) {
        ध्रुव गणना xkb_action_controls old = state->आधारः.components.controls;

        /*
         * Set: Disable specified controls that were *not* enabled at key press.
         * Lock: Disable specified controls that *were* enabled at key press.
         */
        state->आधारः.components.controls &= ~(गणना xkb_action_controls) filter->priv;

        यदि ((old & CONTROL_STICKY_कुञ्चिकाः) &&
            !(state->आधारः.components.controls & CONTROL_STICKY_कुञ्चिकाः)) {
            /* Sticky keys were disabled: clear all locks and latches */
            clear_all_latches_and_locks(state, events);
        }
    }

    filter->func = रिक्त;
    प्रत्यागम XKB_FILTER_CONTINUE;
}

स्थिर bool
append_redirect_कुञ्चिका_events(संरचना xkb_server_state *state,
                           संरचना xkb_events *events,
                           ध्रुव संरचना xkb_redirect_कुञ्चिका_action *redirect,
                           गणना xkb_कुञ्चिका_direction direction)
{
    गणना xkb_state_component परिवर्तितम् = 0;
    ध्रुव xkb_mod_आवरणम्_t आवरणम् = redirect->affect_mods;

    /*
     * Reference state: find the last state update in the queue, otherwise
     * use the current state.
     */
    संरचना xkb_event *event;
    संरचना state_components चरमः_components = state->आधारः.components;
    darray_foreach_reverse(event, events->queue) {
        यदि (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
            चरमः_components = event->components.components;
            भङ्ग;
        }
    }

    यदि (आवरणम्) {
        संरचना state_components new = चरमः_components;
        new.आधारः_mods = (new.आधारः_mods & ~आवरणम्) | redirect->mods;
        new.latched_mods = (new.latched_mods & ~आवरणम्) | redirect->mods;
        new.locked_mods = (new.locked_mods & ~आवरणम्) | redirect->mods;
        new.mods = (new.mods & ~आवरणम्) | redirect->mods;

        परिवर्तितम् = get_state_component_changes(&चरमः_components, &new);
        यदि (परिवर्तितम्) {
            darray_append(events->queue, (संरचना xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = new,
                    .परिवर्तितम् = परिवर्तितम्
                }
            });
            /*
             * Commit the temporary modifiers change before the key event
             * it was computed for: consumers must observe the key event
             * under the *redirected* modifiers.
             */
            darray_append(events->queue, (संरचना xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_FRAME,
            });
        }
    }

    darray_append(events->queue, (संरचना xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_कुञ्चिका,
        .कुञ्चिका = {
            .keycode = redirect->keycode,
            .direction = direction
        }
    });

    यदि (आवरणम् && परिवर्तितम्) {
        /* Restore state */
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = चरमः_components,
                .परिवर्तितम् = परिवर्तितम्
            }
        });
    }

    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;
    प्रत्यागम true;
}

स्थिर शून्य
xkb_filter_redirect_कुञ्चिका_new(संरचना xkb_server_state *state,
                            संरचना xkb_events *events,
                            संरचना xkb_filter *filter)
{
    /* Action effectual only with the xkb_machine API and a valid keycode */
    यदि (!events || filter->action.redirect.keycode == XKB_KEYCODE_INVALID) {
        filter->func = रिक्त;
        प्रत्यागम;
    }
    append_redirect_कुञ्चिका_events(state, events, &filter->action.redirect,
                               XKB_कुञ्चिका_अधस्तात्);
}

स्थिर bool
xkb_filter_redirect_कुञ्चिका_func(संरचना xkb_server_state *state,
                             संरचना xkb_events *events,
                             संरचना xkb_filter *filter,
                             ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                             गणना xkb_कुञ्चिका_direction direction)
{
    यदि (कुञ्चिका != filter->कुञ्चिका)
        प्रत्यागम XKB_FILTER_CONTINUE;

    यदि (direction == XKB_कुञ्चिका_ऊर्ध्वम्) {
        append_redirect_कुञ्चिका_events(state, events,
                                   &filter->action.redirect, XKB_कुञ्चिका_ऊर्ध्वम्);
        filter->func = रिक्त;
        प्रत्यागम XKB_FILTER_CONSUME;
     } अन्यथा यदि (direction == XKB_कुञ्चिका_अधस्तात्) {
        ध्रुव सङ्घ xkb_action *actions = रिक्त;
        ध्रुव xkb_action_गणनम्_t गणनम् =
            xkb_कुञ्चिका_get_actions(&state->आधारः, कुञ्चिका, &actions);

        प्रति (xkb_action_गणनम्_t a = 0; a < गणनम्; a++) {
            यदि (actions[a].type == ACTION_TYPE_REDIRECT_कुञ्चिका &&
                actions[a].redirect.keycode != filter->action.redirect.keycode) {
                    /* One action redirects to a different key: release first */
                    append_redirect_कुञ्चिका_events(state, events,
                                               &filter->action.redirect,
                                               XKB_कुञ्चिका_ऊर्ध्वम्);
                filter->func = रिक्त;
                प्रत्यागम XKB_FILTER_CONTINUE;
            }
        }
    }

    append_redirect_कुञ्चिका_events(state, events, &filter->action.redirect,
                               direction);
    प्रत्यागम XKB_FILTER_CONSUME;
}

स्थिर ध्रुव संरचना {
    शून्य (*new)(संरचना xkb_server_state *state,
                संरचना xkb_events *events,
                संरचना xkb_filter *filter);
    bool (*func)(संरचना xkb_server_state *state,
                 संरचना xkb_events *events,
                 संरचना xkb_filter *filter,
                 ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका, गणना xkb_कुञ्चिका_direction direction);
} filter_action_funcs[ACTION_TYPE_अङ्कः_ENTRIES] = {
    [ACTION_TYPE_MOD_समुच्चयः]     = { xkb_filter_mod_समुच्चयः_new,
                                  xkb_filter_mod_समुच्चयः_func },
    [ACTION_TYPE_MOD_LATCH]   = { xkb_filter_mod_latch_new,
                                  xkb_filter_mod_latch_func },
    [ACTION_TYPE_MOD_तालकम्]    = { xkb_filter_mod_तालकम्_new,
                                  xkb_filter_mod_तालकम्_func },
    [ACTION_TYPE_गणः_समुच्चयः]   = { xkb_filter_गणः_समुच्चयः_new,
                                  xkb_filter_गणः_समुच्चयः_func },
    [ACTION_TYPE_गणः_LATCH] = { xkb_filter_गणः_latch_new,
                                  xkb_filter_गणः_latch_func },
    [ACTION_TYPE_गणः_तालकम्]  = { xkb_filter_गणः_तालकम्_new,
                                  xkb_filter_गणः_तालकम्_func },
    [ACTION_TYPE_PTR_MOVE]    = { xkb_filter_pointer_move_new,
                                  xkb_filter_pointer_move_func },
    [ACTION_TYPE_PTR_BUTTON]  = { xkb_filter_pointer_button_new,
                                  xkb_filter_pointer_button_func },
    [ACTION_TYPE_PTR_तालकम्]    = { xkb_filter_pointer_तालकम्_button_new,
                                  xkb_filter_pointer_तालकम्_button_func },
    [ACTION_TYPE_PTR_DEFAULT] = { xkb_filter_pointer_समुच्चयः_default_button_new,
                                  xkb_filter_pointer_समुच्चयः_default_button_func },
    [ACTION_TYPE_TERMINATE]   = { xkb_filter_terminate_server_new,
                                  xkb_filter_terminate_server_func },
    [ACTION_TYPE_SWITCH_VT]   = { xkb_filter_switch_virtual_console_new,
                                  xkb_filter_switch_virtual_console_func },
    [ACTION_TYPE_CTRL_समुच्चयः]    = { xkb_filter_ctrls_new,
                                  xkb_filter_ctrls_func },
    [ACTION_TYPE_CTRL_तालकम्]   = { xkb_filter_ctrls_new,
                                  xkb_filter_ctrls_func },
    [ACTION_TYPE_REDIRECT_कुञ्चिका] = { xkb_filter_redirect_कुञ्चिका_new,
                                   xkb_filter_redirect_कुञ्चिका_func },
};

/**
 * Applies any relevant filters to the key, first from the list of filters
 * that are currently active, then if no filter has claimed the key, possibly
 * apply a new filter from the key action.
 */
स्थिर शून्य
xkb_filter_apply_all(संरचना xkb_server_state *state,
                     संरचना xkb_events *events,
                     ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                     गणना xkb_कुञ्चिका_direction direction)
{
    /* First run through all the currently active filters and see if any of
     * them have consumed this event. */
    bool consumed = false;
    संरचना xkb_filter *filter;
    darray_foreach(filter, state->filters) {
        यदि (!filter->func)
            अनुवर्त;

        यदि (filter->func(state, events, filter, कुञ्चिका, direction) == XKB_FILTER_CONSUME)
            consumed = true;
    }
    यदि (consumed || direction == XKB_कुञ्चिका_ऊर्ध्वम्)
        प्रत्यागम;

    /* No filter consumed this event, so proceed with the key actions */
    ध्रुव सङ्घ xkb_action *actions = रिक्त;
    xkb_action_गणनम्_t गणनम् = xkb_कुञ्चिका_get_actions(&state->आधारः, कुञ्चिका,
                                                         &actions);

    प्रति (संरचना {
            xkb_action_गणनम्_t गणनम्;
            ध्रुव सङ्घ xkb_action *actions;
         } अग्र = {0};
         गणनम्;
         गणनम् = अग्र.गणनम्, अग्र.गणनम् = 0, actions = अग्र.actions
    ) {
        /*
         * Process actions sequentially.
         *
         * NOTE: We rely on the parser to disallow multiple modifier or group
         * actions (see `CheckMultipleActionsCategories`). Allowing multiple
         * such actions requires a refactor of the state handling.
         */
        प्रति (xkb_action_गणनम्_t k = 0; k < गणनम्; k++) {
            /*
             * It's possible for the keymap to set action->type explicitly, like
             * so:
             *     interpret XF86_Next_VMode {
             *         action = Private(type=0x86, data="+VMode");
             *     };
             * We don't handle those.
             */
            यदि (actions[k].type >= ACTION_TYPE_अङ्कः_ENTRIES)
                अनुवर्त;

            /* Go to next action if no corresponding action handler */
            यदि (!filter_action_funcs[actions[k].type].new)
                अनुवर्त;

            /* Add a new filter and run the corresponding initial action */
            filter = xkb_filter_new(state);
            filter->कुञ्चिका = कुञ्चिका;
            filter->action = actions[k];

            विकल्प (filter->action.type) {
            पक्ष ACTION_TYPE_MOD_समुच्चयः:
                यदि (state->आधारः.components.controls & CONTROL_STICKY_कुञ्चिकाः) {
                    /* Convert modifier set action to a latch */
                    filter->action.type = ACTION_TYPE_MOD_LATCH;
                    यदि (state->a11y_flags & XKB_A11_Y_STICKY_कुञ्चिकाः_LATCH_गन्तव्यम्_तालकम्) {
                        filter->action.mods.flags |= ACTION_LATCH_गन्तव्यम्_तालकम्;
                    }
                }
                /* fallthrough */
            पक्ष ACTION_TYPE_MOD_LATCH:
            पक्ष ACTION_TYPE_MOD_तालकम्: {
                स्थिर ध्रुव गणना xkb_action_flags unlock_mods_on_press
                    = ACTION_तालकम्_CLEAR     /* SetMods & LatchMods: can unlock */
                    | ACTION_तालकम्_NO_UNLOCK /* LockMods: cannot unlock */
                    | ACTION_EXPLICIT_UNLOCK_ON_PRESS;
                यदि ((state->flags & STATE_UNLOCK_MODS_ON_PRESS) &&
                    /*
                     * Ensure that the action can actually unlock and has
                     * no *explicit* `unlockOnPress` parameter.
                     */
                    (filter->action.mods.flags & unlock_mods_on_press) ==
                    (filter->action.type == ACTION_TYPE_MOD_तालकम्
                        ? 0 : ACTION_तालकम्_CLEAR))
                {
                    filter->action.mods.flags |= ACTION_UNLOCK_ON_PRESS;
                }
                भङ्ग;
            }
            पक्ष ACTION_TYPE_गणः_समुच्चयः:
                यदि (state->आधारः.components.controls & CONTROL_STICKY_कुञ्चिकाः) {
                    /* Convert group set action to a latch */
                    filter->action.type = ACTION_TYPE_गणः_LATCH;
                    यदि (state->a11y_flags & XKB_A11_Y_STICKY_कुञ्चिकाः_LATCH_गन्तव्यम्_तालकम्) {
                        filter->action.गणः.flags |= ACTION_LATCH_गन्तव्यम्_तालकम्;
                    }
                }
                भङ्ग;
            पक्ष ACTION_TYPE_गणः_तालकम्:
                यदि ((filter->action.गणः.flags &
                     ACTION_AUTO_गणः_तालकम्_ON_MODIFIER_RELEASE) &&
                    (state->flags &
                     STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_कुञ्चिका) &&
                    /*
                     * FIXME: sticky keys may leave an unexpected modifier
                     * from the level 0 latched, so disable for now.
                     */
                    !(state->आधारः.components.controls & CONTROL_STICKY_कुञ्चिकाः))
                {
                    filter->action.गणः.flags |= ACTION_तालकम्_ON_RELEASE;
                    /* Add base level action */
                    अग्र.गणनम् = xkb_keymap_कुञ्चिका_get_actions_by_level(
                        state->आधारः.keymap, कुञ्चिका, state->आधारः.components.गणः,
                        0, &अग्र.actions
                    );
                    प्रतिज्ञा(अग्र.गणनम् == 1);
                }
                भङ्ग;
            पक्ष ACTION_TYPE_PTR_MOVE:
            पक्ष ACTION_TYPE_PTR_BUTTON:
            पक्ष ACTION_TYPE_PTR_तालकम्:
            पक्ष ACTION_TYPE_PTR_DEFAULT:
                यदि (!(state->आधारः.components.controls & CONTROL_MOUSE_कुञ्चिकाः)) {
                    /* Convert pointer actions to NoAction */
                    प्रतिज्ञा(!filter_action_funcs[ACTION_TYPE_NONE].new);
                    filter->func = रिक्त;
                    अनुवर्त;
                }
                भङ्ग;
            पक्ष ACTION_TYPE_REDIRECT_कुञ्चिका:
                // FIXME: this is not efficient to resolve mods here each time
                filter->action.redirect.affect_mods = mod_आवरणम्_get_effective(
                    state->आधारः.keymap, filter->action.redirect.affect_mods
                );
                filter->action.redirect.mods = mod_आवरणम्_get_effective(
                    state->आधारः.keymap, filter->action.redirect.mods
                );
                भङ्ग;
            स्वभाव: ;
            }
            filter->func = filter_action_funcs[filter->action.type].func;
            filter_action_funcs[filter->action.type].new(state, events, filter);
        }
    }
}

स्थिर शून्य
xkb_state_update_derived(संरचना xkb_state *state);

स्थिर अन्तःस्थ शून्य
xkb_state_init(संरचना xkb_state * सीमित state,
               संरचना xkb_keymap * सीमित keymap,
               गणना xkb_state_बहुलकम्_आन्तरिकम् बहुलकम्)
{
    state->बहुलकम् = बहुलकम्;
    state->refcnt = 1;
    state->keymap = xkb_keymap_ref(keymap);
    state->बहिर्गमनम्_सम्बन्धि_range_गणः.policy = XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_WRAP;

    /* Ensure that derived state is correctly initialized */
    xkb_state_update_derived(state);
}

स्थिर अन्तःस्थ शून्य
xkb_client_state_init(संरचना xkb_client_state * सीमित state,
                      संरचना xkb_keymap * सीमित keymap,
                      गणना xkb_state_बहुलकम्_आन्तरिकम् बहुलकम्)
{
    xkb_state_init(&state->आधारः, keymap, बहुलकम्);
}

स्थिर अन्तःस्थ शून्य
xkb_server_state_init(संरचना xkb_server_state * सीमित state,
                      संरचना xkb_keymap * सीमित keymap,
                      गणना xkb_state_बहुलकम्_आन्तरिकम् बहुलकम्,
                      गणना xkb_a11y_flags a11y_affect,
                      गणना xkb_a11y_flags a11y_flags)
{
    xkb_state_init(&state->आधारः, keymap, बहुलकम्);

    state->a11y_flags = a11y_flags;
    यदि (keymap->format != XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !(a11y_affect & XKB_A11_Y_LATCH_युगपत्_कुञ्चिकाः)) {
             /* Keymap v2+: enable extension to XKB if not manually disabled */
             state->a11y_flags |= XKB_A11_Y_LATCH_युगपत्_कुञ्चिकाः;
    }
}

संरचना xkb_state *
xkb_state_new(संरचना xkb_keymap *keymap)
{
    संरचना xkb_server_state * ध्रुव state = शून्यस्मृतिं_देहि(1, परिमाण(*state));
    यदि (!state) {
        log_err_func1(keymap->ctx, XKB_ERROR_आवण्टनम्_FAILURE_2,
                      "Could not allocate a state object.\n");
        प्रत्यागम रिक्त;
    }

    xkb_server_state_init(state, keymap, LEGACY_MIXED_STATE, 0, 0);

    प्रत्यागम (संरचना xkb_state *)state;
}

संरचना xkb_state *
xkb_state_new_with_बहुलकम्(संरचना xkb_keymap * सीमित keymap,
                        गणना xkb_state_बहुलकम् बहुलकम्,
                        गणना xkb_स्थितिः * सीमित स्थितिः)
{
    विकल्प (बहुलकम्) {
    पक्ष XKB_STATE_बहुलकम्_CLIENT:
    पक्ष XKB_STATE_बहुलकम्_SERVER_QUERY: {
        संरचना xkb_client_state * ध्रुव state = शून्यस्मृतिं_देहि(1, परिमाण(*state));
        यदि (!state)
            भङ्ग;

        static_assert((अचिह्नित)XKB_STATE_बहुलकम्_CLIENT ==
                      (अचिह्नित)CLIENT_STATE, "");
        static_assert((अचिह्नित)XKB_STATE_बहुलकम्_SERVER_QUERY ==
                      (अचिह्नित)SERVER_COMPANION, "");
        xkb_client_state_init(state, keymap, (गणना xkb_state_बहुलकम्_आन्तरिकम्)बहुलकम्);

        यदि (स्थितिः)
            *स्थितिः = XKB_SUCCESS;

        प्रत्यागम (संरचना xkb_state *)state;
    }
    पक्ष XKB_STATE_बहुलकम्_SERVER: {
        संरचना xkb_server_state * ध्रुव state = शून्यस्मृतिं_देहि(1, परिमाण(*state));
        यदि (!state)
            भङ्ग;

        xkb_server_state_init(state, keymap, LEGACY_SERVER_STATE, 0, 0);

        यदि (स्थितिः)
            *स्थितिः = XKB_SUCCESS;

        प्रत्यागम (संरचना xkb_state *)state;
    }
    स्वभाव:
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_STATE_बहुलकम्_2,
                     "Unsupported state mode: %u\n", बहुलकम्);
        यदि (स्थितिः)
            *स्थितिः = XKB_ERROR_UNSUPPORTED_STATE_बहुलकम्;
        प्रत्यागम रिक्त;
    }

    log_err_func1(keymap->ctx, XKB_ERROR_आवण्टनम्_FAILURE_2,
                  "Could not allocate a state object.\n");
    यदि (स्थितिः)
        *स्थितिः = XKB_ERROR_आवण्टनम्_FAILURE;
    प्रत्यागम रिक्त;
}

संरचना xkb_state *
xkb_state_ref(संरचना xkb_state *state)
{
    प्रतिज्ञा(state->refcnt > 0);
    state->refcnt++;
    प्रत्यागम state;
}

शून्य
xkb_state_reset(संरचना xkb_state *state)
{
    स्मृतिं_पूरय(&state->components, 0, परिमाण(state->components));

    यदि (state->बहुलकम् < LEGACY_MIXED_STATE) {
        static_assert(परिमाण(*state) == परिमाण(संरचना xkb_client_state),
                      "Not a mere wrapper");
        /* Nothing else to reset */
    } अन्यथा {
        संरचना xkb_server_state *state_ = (संरचना xkb_server_state *)state;
        ध्रुव संरचना xkb_server_state पूर्व = *state_;
        स्मृतिं_पूरय(state_, 0, परिमाण(*state_));

        state_->आधारः = पूर्व.आधारः;
        state_->a11y_flags = पूर्व.a11y_flags;
        state_->filters = पूर्व.filters;
        darray_आकार(state_->filters) = 0;
    }
}

स्थिर अन्तःस्थ शून्य
xkb_state_destroy(संरचना xkb_state *state)
{
    xkb_keymap_unref(state->keymap);
    यदि (state->बहुलकम् > SERVER_COMPANION)
        darray_free(((संरचना xkb_server_state *)state)->filters);
}

शून्य
xkb_state_unref(संरचना xkb_state *state)
{
    प्रतिज्ञा(!state || state->refcnt > 0);
    यदि (!state || --state->refcnt > 0)
        प्रत्यागम;

    xkb_state_destroy(state);

    विकल्प (state->बहुलकम्) {
    पक्ष CLIENT_STATE:
    पक्ष SERVER_COMPANION:
        मुक्त((संरचना xkb_client_state *)state);
        भङ्ग;
    पक्ष LEGACY_MIXED_STATE:
    पक्ष LEGACY_SERVER_STATE:
        मुक्त((संरचना xkb_server_state *)state);
        भङ्ग;
    पक्ष SERVER_STATE: /* handled by xkb_machine_unref() */
    स्वभाव:
        PANIC_UNREACHABLE();
    }
}

संरचना xkb_keymap *
xkb_state_get_keymap(संरचना xkb_state *state)
{
    प्रत्यागम state->keymap;
}

/**
 * Update the LED state to match the rest of the xkb_state.
 */
स्थिर शून्य
xkb_state_led_update_all(संरचना xkb_state *state)
{
    xkb_led_सूचकांक_t idx;
    ध्रुव संरचना xkb_led *led;

    state->components.leds = 0;

    xkb_leds_enumerate(idx, led, state->keymap) {

        यदि (led->which_mods != 0 && led->mods.आवरणम् != 0) {
            xkb_mod_आवरणम्_t mod_आवरणम् = 0;
            यदि (led->which_mods & XKB_STATE_MODS_EFFECTIVE)
                mod_आवरणम् |= state->components.mods;
            यदि (led->which_mods & XKB_STATE_MODS_DEPRESSED)
                mod_आवरणम् |= state->components.आधारः_mods;
            यदि (led->which_mods & XKB_STATE_MODS_LATCHED)
                mod_आवरणम् |= state->components.latched_mods;
            यदि (led->which_mods & XKB_STATE_MODS_LOCKED)
                mod_आवरणम् |= state->components.locked_mods;

            यदि (led->mods.आवरणम् & mod_आवरणम्) {
                state->components.leds |= (UINT32_C(1) << idx);
                अनुवर्त;
            }
        }

        यदि (led->which_groups != 0) {
            यदि (likely(led->groups) != 0) {
                xkb_layout_आवरणम्_t गणः_आवरणम् = 0;
                /* Effective and locked groups have been brought into range */
                प्रतिज्ञा(state->components.गणः < XKB_MAX_GROUPS);
                प्रतिज्ञा(state->components.locked_गणः >= 0 &&
                       state->components.locked_गणः < XKB_MAX_GROUPS);
                /* Effective and locked groups are used as mask */
                यदि (led->which_groups & XKB_STATE_LAYOUT_EFFECTIVE)
                    गणः_आवरणम् |= (UINT32_C(1) << state->components.गणः);
                यदि (led->which_groups & XKB_STATE_LAYOUT_LOCKED)
                    गणः_आवरणम् |= (UINT32_C(1) << state->components.locked_गणः);
                /* Base and latched groups only have to be non-zero */
                यदि ((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                    state->components.आधारः_गणः != 0)
                    गणः_आवरणम् |= led->groups;
                यदि ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                    state->components.latched_गणः != 0)
                    गणः_आवरणम् |= led->groups;

                यदि (led->groups & गणः_आवरणम्) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    अनुवर्त;
                }
            } अन्यथा {
                /* Special case for Base and latched groups */
                यदि (((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                     state->components.आधारः_गणः == 0) ||
                    ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                     state->components.latched_गणः == 0)) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    अनुवर्त;
                }
            }
        }

        यदि (led->ctrls & state->components.controls) {
            state->components.leds |= (UINT32_C(1) << idx);
            अनुवर्त;
        }
    }
}

/**
 * Calculates the derived state (effective mods/group and LEDs) from an
 * up-to-date xkb_state.
 */
स्थिर शून्य
xkb_state_update_derived(संरचना xkb_state *state)
{
    xkb_layout_सूचकांक_t wrapped;

    state->components.mods = (state->components.आधारः_mods |
                              state->components.latched_mods |
                              state->components.locked_mods);

    /* Lock group must be adjusted, but not base nor latched groups */
    wrapped = Xkb_Wrap_गणः_Into_Range(state->components.locked_गणः,
                                    state->keymap->अङ्कः_groups,
                                    state->बहिर्गमनम्_सम्बन्धि_range_गणः.policy,
                                    state->बहिर्गमनम्_सम्बन्धि_range_गणः.redirect_गणः);
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->components.locked_गणः =
        (int32_t) (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    /* Effective group must be adjusted */
    wrapped = Xkb_Wrap_गणः_Into_Range(state->components.आधारः_गणः +
                                    state->components.latched_गणः +
                                    state->components.locked_गणः,
                                    state->keymap->अङ्कः_groups,
                                    state->बहिर्गमनम्_सम्बन्धि_range_गणः.policy,
                                    state->बहिर्गमनम्_सम्बन्धि_range_गणः.redirect_गणः);
    state->components.गणः =
        (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    xkb_state_led_update_all(state);
}

स्थिर शून्य
state_update_enabled_controls(संरचना xkb_server_state *state,
                              गणना xkb_keyboard_control_flags affect_controls,
                              गणना xkb_keyboard_control_flags controls,
                              संरचना xkb_events *events)
{
    ध्रुव bool had_sticky_कुञ्चिकाः = state->आधारः.components.controls
                               & CONTROL_STICKY_कुञ्चिकाः;

    /*
     * Enable using the public API with the all the Control values, except
     * the internal ones, if any.
     */
    affect_controls =
        ( affect_controls
        & (गणना xkb_keyboard_control_flags)CONTROL_ALL_BOOLEAN );
    state->आधारः.components.controls &=
        (गणना xkb_action_controls)~affect_controls;
    state->आधारः.components.controls |=
        (गणना xkb_action_controls)(controls & affect_controls);

    यदि (had_sticky_कुञ्चिकाः &&
        !(state->आधारः.components.controls & CONTROL_STICKY_कुञ्चिकाः)) {
        /* Sticky keys were disabled: clear all locks and latches */
        clear_all_latches_and_locks(state, events);
    }

    xkb_state_update_derived(&state->आधारः);
}

/**
 * Given a particular key event, updates the state structure to reflect the
 * new modifiers.
 */
गणना xkb_state_component
xkb_state_update_कुञ्चिका(संरचना xkb_state *आधारः_state, xkb_keycode_t kc,
                     गणना xkb_कुञ्चिका_direction direction)
{
    /* Guard against client-only state */
    प्रतिज्ञा(आधारः_state->बहुलकम् > SERVER_COMPANION);
    यदि (आधारः_state->बहुलकम् <= SERVER_COMPANION) {
        log_err(आधारः_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_बहुलकम्,
                "%s: Unexpected state type %d\n", __func__, आधारः_state->बहुलकम्);
        प्रत्यागम 0;
    }

    संरचना xkb_server_state * ध्रुव state =
        (संरचना xkb_server_state *)आधारः_state;

    ध्रुव संरचना xkb_कुञ्चिका* ध्रुव कुञ्चिका = Xkb_कुञ्चिका(state->आधारः.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    यदि (!कुञ्चिका || (direction == XKB_कुञ्चिका_REPEATED && !कुञ्चिका->repeats))
        प्रत्यागम 0;

    यदि (direction == XKB_कुञ्चिका_अधस्तात् &&
        (state->आधारः.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_कुञ्चिकाः) &&
        (state->a11y_flags & XKB_A11_Y_STICKY_कुञ्चिकाः_NO_युगपत्_कुञ्चिकाः) &&
        state->आधारः.components.आधारः_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_कुञ्चिकाः,
                                      0, रिक्त);
    }

    ध्रुव संरचना state_components पूर्व_components = state->आधारः.components;

    state->समुच्चयः_mods = 0;
    state->clear_mods = 0;

    xkb_filter_apply_all(state, रिक्त, कुञ्चिका, direction);

    xkb_mod_सूचकांक_t i;
    xkb_mod_आवरणम्_t bit;
    प्रति (i = 0, bit = 1; state->समुच्चयः_mods; i++, bit <<= 1) {
        यदि (state->समुच्चयः_mods & bit) {
            state->mod_कुञ्चिका_गणनम्[i]++;
            state->आधारः.components.आधारः_mods |= bit;
            state->समुच्चयः_mods &= ~bit;
        }
    }

    प्रति (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        यदि (state->clear_mods & bit) {
            state->mod_कुञ्चिका_गणनम्[i]--;
            यदि (state->mod_कुञ्चिका_गणनम्[i] <= 0) {
                state->आधारः.components.आधारः_mods &= ~bit;
                state->mod_कुञ्चिका_गणनम्[i] = 0;
            }
            state->clear_mods &= ~bit;
        }
    }

    xkb_state_update_derived(&state->आधारः);

    प्रत्यागम get_state_component_changes(&पूर्व_components, &state->आधारः.components);
}

/* We need fake keys for `update_latch_modifiers` and `update_latch_group`.
 * These keys must have at least one level in order to break latches. We need 2
 * keys with specific actions in order to update group/mod latches without
 * affecting each other. */
स्थिर संरचना xkb_कुञ्चिका_type_प्रविष्टिः synthetic_कुञ्चिका_level_प्रविष्टिः = { 0 };
स्थिर संरचना xkb_कुञ्चिका_type synthetic_कुञ्चिका_type = {
     .अङ्कः_entries = 1,
     .अङ्कः_स्तराः = 1,
     .entries = &synthetic_कुञ्चिका_level_प्रविष्टिः
};
स्थिर ध्रुव संरचना xkb_कुञ्चिका synthetic_कुञ्चिका = { 0 };

/* Transcription from xserver: XkbLatchModifiers */
स्थिर शून्य
update_latch_modifiers(संरचना xkb_server_state *state,
                       संरचना xkb_events *events,
                       xkb_mod_आवरणम्_t आवरणम्, xkb_mod_आवरणम्_t latches)
{
    /* Clear affected latched modifiers */
    ध्रुव xkb_mod_आवरणम्_t clear = आवरणम् & ~latches;
    state->आधारः.components.latched_mods &= ~clear;

    /* Clear any pending latch to locks using ad hoc action:
     * only affect corresponding modifier latches and no group latch. */
    संरचना xkb_level synthetic_कुञ्चिका_level_break_mod_latch = {
        .अङ्कः_syms = 0,
        .अङ्कः_actions = 1,
        .ऊर्ध्वस्थम् = XKB_कुञ्चिका_No_चिह्नम्,
        .s.sym = XKB_कुञ्चिका_No_चिह्नम्,
        .a.action.आन्तरिकम् = {
            .type = ACTION_TYPE_आन्तरिकम्,
            .flags = आन्तरिकम्_BREAKS_MOD_LATCH,
            .clear_latched_mods = clear
        }
    };
    संरचना xkb_गणः synthetic_कुञ्चिका_गणः_break_mod_latch = {
        .type = &synthetic_कुञ्चिका_type,
        .स्तराः = &synthetic_कुञ्चिका_level_break_mod_latch
    };
    ध्रुव संरचना xkb_कुञ्चिका synthetic_कुञ्चिका_break_mod_latch = {
        .अङ्कः_groups = 1,
        .groups = &synthetic_कुञ्चिका_गणः_break_mod_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_कुञ्चिका_break_mod_latch, XKB_कुञ्चिका_अधस्तात्);

    /* Finally set the latched mods by simulate tapping a key with the
     * corresponding action */
    ध्रुव संरचना xkb_कुञ्चिका* ध्रुव कुञ्चिका = &synthetic_कुञ्चिका;
    ध्रुव सङ्घ xkb_action latch_mods = {
        .mods = {
            .type = ACTION_TYPE_MOD_LATCH,
            .mods = { .आवरणम् = आवरणम् & latches },
            .flags = 0,
        },
    };
    संरचना xkb_filter* ध्रुव filter = xkb_filter_new(state);
    filter->कुञ्चिका = कुञ्चिका;
    filter->func = xkb_filter_mod_latch_func;
    filter->action = latch_mods;
    xkb_filter_mod_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_mod_latch_func(state, events, filter, कुञ्चिका, XKB_कुञ्चिका_ऊर्ध्वम्);
}

/* Transcription from xserver: XkbLatchGroup */
स्थिर शून्य
update_latch_गणः(संरचना xkb_server_state *state,
                   संरचना xkb_events *events, int32_t गणः)
{
    /* Clear any pending latch to locks. */
    स्थिर संरचना xkb_level synthetic_कुञ्चिका_level_break_गणः_latch = {
        .अङ्कः_syms = 0,
        .अङ्कः_actions = 1,
        .ऊर्ध्वस्थम् = XKB_कुञ्चिका_No_चिह्नम्,
        .s.sym = XKB_कुञ्चिका_No_चिह्नम्,
        .a.action.आन्तरिकम् = {
            .type = ACTION_TYPE_आन्तरिकम्,
            .flags = आन्तरिकम्_BREAKS_गणः_LATCH,
            .clear_latched_mods = 0
        }
    };
    स्थिर संरचना xkb_गणः synthetic_कुञ्चिका_गणः_break_गणः_latch = {
        .type = &synthetic_कुञ्चिका_type,
        .स्तराः = &synthetic_कुञ्चिका_level_break_गणः_latch
    };
    स्थिर ध्रुव संरचना xkb_कुञ्चिका synthetic_कुञ्चिका_break_गणः_latch = {
        .अङ्कः_groups = 1,
        .groups = &synthetic_कुञ्चिका_गणः_break_गणः_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_कुञ्चिका_break_गणः_latch, XKB_कुञ्चिका_अधस्तात्);

    /* Simulate tapping a key with a group latch action, but in isolation: i.e.
     * without affecting the other filters. */
    ध्रुव संरचना xkb_कुञ्चिका* ध्रुव कुञ्चिका = &synthetic_कुञ्चिका;
    ध्रुव सङ्घ xkb_action latch_गणः = {
        .गणः = {
            .type = ACTION_TYPE_गणः_LATCH,
            .flags = ACTION_ABSOLUTE_SWITCH,
            .गणः = गणः,
        },
    };
    संरचना xkb_filter* ध्रुव filter = xkb_filter_new(state);
    filter->कुञ्चिका = कुञ्चिका;
    filter->func = xkb_filter_गणः_latch_func;
    filter->action = latch_गणः;
    xkb_filter_गणः_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_गणः_latch_func(state, events, filter, कुञ्चिका, XKB_कुञ्चिका_ऊर्ध्वम्);
}

/**
 * Compute the resolved effective mask of an arbitrary input.
 *
 * Contrary to `mod_mask_get_effective`, it resolves only modifiers not present
 * in the canonical mask, so that it enables `xkb_state_serialize_mods` to
 * round trip via `xkb_state_update_mask`.
 */
स्थिर अन्तःस्थ xkb_mod_आवरणम्_t
resolve_गन्तव्यम्_canonical_mods(संरचना xkb_keymap *keymap, xkb_mod_आवरणम्_t mods)
{
    प्रत्यागम
        /*
         * Keep canonical modifier mask.
         * It contains either real modifiers or canonical virtual modifiers.
         */
        (mods & keymap->canonical_state_आवरणम्) |
        /* Resolve other modifiers */
        mod_आवरणम्_get_effective(keymap,
                               mods & ~keymap->canonical_state_आवरणम्);
}

स्थिर शून्य
state_update_latched_locked(
    संरचना xkb_server_state * सीमित state,
    ध्रुव संरचना xkb_state_components_update * सीमित update,
    संरचना xkb_events * सीमित events
)
{
    /* Update locks */
    ध्रुव xkb_mod_आवरणम्_t affect_locked_mods =
        resolve_गन्तव्यम्_canonical_mods(state->आधारः.keymap, update->affect_locked_mods);
    यदि ((update->components & XKB_STATE_MODS_LOCKED) &&
        affect_locked_mods) {
        ध्रुव xkb_mod_आवरणम्_t locked_mods =
            resolve_गन्तव्यम्_canonical_mods(state->आधारः.keymap, update->locked_mods);
        state->आधारः.components.locked_mods &= ~affect_locked_mods;
        state->आधारः.components.locked_mods |= locked_mods & affect_locked_mods;
    }
    यदि (update->components & XKB_STATE_LAYOUT_LOCKED) {
        state->आधारः.components.locked_गणः = update->locked_layout;
    }

    /* Update latches */
    ध्रुव xkb_mod_आवरणम्_t affect_latched_mods =
        resolve_गन्तव्यम्_canonical_mods(state->आधारः.keymap, update->affect_latched_mods);
    यदि ((update->components & XKB_STATE_MODS_LATCHED) &&
        affect_latched_mods) {
        ध्रुव xkb_mod_आवरणम्_t latched_mods =
            resolve_गन्तव्यम्_canonical_mods(state->आधारः.keymap, update->latched_mods);
        update_latch_modifiers(state, events, affect_latched_mods, latched_mods);
    }
    यदि (update->components & XKB_STATE_LAYOUT_LATCHED) {
        update_latch_गणः(state, events, update->latched_layout);
    }
}

गणना xkb_state_component
xkb_state_update_latched_locked(संरचना xkb_state *आधारः_state,
                                xkb_mod_आवरणम्_t affect_latched_mods,
                                xkb_mod_आवरणम्_t latched_mods,
                                bool affect_latched_layout,
                                int32_t latched_layout,
                                xkb_mod_आवरणम्_t affect_locked_mods,
                                xkb_mod_आवरणम्_t locked_mods,
                                bool affect_locked_layout,
                                int32_t locked_layout)
{
    /* Guard against client-only state */
    प्रतिज्ञा(आधारः_state->बहुलकम् > SERVER_COMPANION);
    यदि (आधारः_state->बहुलकम् <= SERVER_COMPANION) {
        log_err(आधारः_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_बहुलकम्,
                "%s: Unexpected state type %d\n", __func__, आधारः_state->बहुलकम्);
        प्रत्यागम 0;
    }

    संरचना xkb_server_state * ध्रुव state =
        (संरचना xkb_server_state *)आधारः_state;

    ध्रुव संरचना state_components previous_components = state->आधारः.components;
    ध्रुव गणना xkb_state_component components
        = ((affect_latched_mods || latched_mods) ? XKB_STATE_MODS_LATCHED : 0)
        | ((affect_locked_mods || locked_mods) ? XKB_STATE_MODS_LOCKED : 0)
        | (affect_latched_layout ? XKB_STATE_LAYOUT_LATCHED : 0)
        | (affect_locked_layout ? XKB_STATE_LAYOUT_LOCKED : 0);
    ध्रुव संरचना xkb_state_components_update update = {
        .आकार = परिमाण(update),
        .components = components,
        .affect_latched_mods = affect_latched_mods,
        .latched_mods = latched_mods,
        .latched_layout = latched_layout,
        .affect_locked_mods = affect_locked_mods,
        .locked_mods = locked_mods,
        .locked_layout = locked_layout
    };
    state_update_latched_locked(state, &update, रिक्त);
    xkb_state_update_derived(&state->आधारः);
    प्रत्यागम get_state_component_changes(&previous_components,
                                       &state->आधारः.components);
}

स्थिर अन्तःस्थ शून्य
clear_all_latches_and_locks(संरचना xkb_server_state *state,
                            संरचना xkb_events *events)
{
    स्थिर ध्रुव गणना xkb_state_component components
        = XKB_STATE_MODS_LATCHED | XKB_STATE_MODS_LOCKED
        | XKB_STATE_LAYOUT_LATCHED | XKB_STATE_LAYOUT_LOCKED;
    ध्रुव संरचना xkb_state_components_update update = {
        .आकार = परिमाण(update),
        .components = components,
        .affect_latched_mods = (xkb_mod_आवरणम्_t)XKB_MOD_ALL,
        .latched_mods = 0,
        .latched_layout = 0,
        .affect_locked_mods = (xkb_mod_आवरणम्_t)XKB_MOD_ALL,
        .locked_mods = 0,
        .locked_layout = 0
    };
    state_update_latched_locked(state, &update, events);
}

स्थिर गणना xkb_स्थितिः
state_update_layout_policy(संरचना xkb_server_state *state,
                           ध्रुव संरचना xkb_layout_policy_update *update)
{
    यदि (xkb_feature_supported(XKB_FEATURE_ENUM_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_POLICY,
                              update->policy)) {
        यदि (update->policy == XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_REDIRECT) {
            यदि (update->redirect < state->आधारः.keymap->अङ्कः_groups) {
                state->आधारः.बहिर्गमनम्_सम्बन्धि_range_गणः.redirect_गणः =
                    update->redirect;
            } अन्यथा {
                log_err(state->आधारः.keymap->ctx,
                        XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                        "Layout policy: "
                        "unsupported layout index %"PRIu32" > %"PRIu32"\n",
                        update->redirect + 1, state->आधारः.keymap->अङ्कः_groups);
                प्रत्यागम XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक;
            }
        }
        state->आधारः.बहिर्गमनम्_सम्बन्धि_range_गणः.policy = update->policy;
        प्रत्यागम XKB_SUCCESS;
    } अन्यथा {
        log_err(state->आधारः.keymap->ctx,
                XKB_ERROR_UNSUPPORTED_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_POLICY_2,
                "Unsupported layout policy: %d\n", update->policy);
        प्रत्यागम XKB_ERROR_UNSUPPORTED_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_POLICY;
    }
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
स्थिर गणना xkb_स्थितिः
check_state_update_abi_(संरचना xkb_context * सीमित ctx,
                        ध्रुव वर्ण * सीमित func,
                        ध्रुव संरचना xkb_synthetic_update * सीमित update)
{
    गणना xkb_स्थितिः स्थितिः = XKB_SUCCESS;
    यदि ((स्थितिः = xkb_check_state_abi(update)) ||
        (update->reserved0 != 0 &&
         (स्थितिः = XKB_ERROR_ABI_FORWARD_COMPAT)) ||
        (update->components &&
         (स्थितिः = xkb_check_state_abi(update->components))) ||
        (update->layout_policy &&
         (स्थितिः = xkb_check_state_abi(update->layout_policy)))) {
        xkb_log_abi_error(ctx, func, स्थितिः);
    }
    प्रत्यागम स्थितिः;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

#परिभाषय check_state_update_abi(ctx, update) \
    check_state_update_abi_(ctx, __func__, update)

गणना xkb_स्थितिः
xkb_state_update_synthetic(संरचना xkb_state * आधारः_state,
                           ध्रुव संरचना xkb_synthetic_update * update,
                           गणना xkb_state_component *परिवर्तितम्)
{
    /* Guard against client-only state */
    यदि (आधारः_state->बहुलकम् <= SERVER_COMPANION) {
        log_err(आधारः_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_बहुलकम्,
                "%s: Unexpected state type %d\n", __func__, आधारः_state->बहुलकम्);
        प्रत्यागम XKB_ERROR_UNEXPECTED_STATE_बहुलकम्;
    }

    संरचना xkb_server_state * ध्रुव state =
        (संरचना xkb_server_state *)आधारः_state;

    /* Check ABI compatibility */
    गणना xkb_स्थितिः स्थितिः = check_state_update_abi(state->आधारः.keymap->ctx,
                                                    update);
    यदि (स्थितिः != XKB_SUCCESS)
        प्रत्यागम स्थितिः;

    ध्रुव संरचना state_components previous_components = state->आधारः.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    /* Update parametrized controls first */
    यदि (update->layout_policy) {
        स्थितिः = state_update_layout_policy(state, update->layout_policy);
        यदि (स्थितिः != XKB_SUCCESS)
            प्रत्यागम स्थितिः;
    }

    यदि (update->components) {
        ध्रुव संरचना xkb_state_components_update * ध्रुव components =
            update->components;
        /* Update boolean controls first */
        यदि (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, रिक्त);
        }

        state_update_latched_locked(state, components, रिक्त);
    }

    xkb_state_update_derived(&state->आधारः);

    यदि (परिवर्तितम्) {
        *परिवर्तितम् = get_state_component_changes(&previous_components,
                                               &state->आधारः.components);
    }
    प्रत्यागम XKB_SUCCESS;
}

/**
 * Updates the state from a set of explicit masks as gained from
 * xkb_state_serialize_mods and xkb_state_serialize_groups.  As noted in the
 * documentation for these functions in xkbcommon.h, this round-trip is
 * lossy, and should only be used to update a slave state mirroring the
 * master, e.g. in a client/server window system.
 */
गणना xkb_state_component
xkb_state_update_आवरणम्(संरचना xkb_state *आधारः_state,
                      xkb_mod_आवरणम्_t आधारः_mods,
                      xkb_mod_आवरणम्_t latched_mods,
                      xkb_mod_आवरणम्_t locked_mods,
                      xkb_layout_सूचकांक_t आधारः_गणः,
                      xkb_layout_सूचकांक_t latched_गणः,
                      xkb_layout_सूचकांक_t locked_गणः)
{
    /* Guard against server-only and server companion state */
    प्रतिज्ञा(आधारः_state->बहुलकम् == CLIENT_STATE ||
           आधारः_state->बहुलकम् == LEGACY_MIXED_STATE);
    यदि (आधारः_state->बहुलकम् != CLIENT_STATE &&
        आधारः_state->बहुलकम् != LEGACY_MIXED_STATE) {
        log_err(आधारः_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_बहुलकम्,
                "%s: Unexpected state type %d\n", __func__, आधारः_state->बहुलकम्);
        प्रत्यागम 0;
    }

    संरचना xkb_client_state * ध्रुव state =
        (संरचना xkb_client_state *)आधारः_state;

    ध्रुव संरचना state_components पूर्व_components = state->आधारः.components;

    /*
     * Make sure the mods are fully resolved - since we get arbitrary
     * input, they might not be.
     *
     * It might seem more reasonable to do this only for components.mods
     * in xkb_state_update_derived(), rather than for each component
     * separately.  That would allow to distinguish between "really"
     * depressed mods (would be in MODS_DEPRESSED) and indirectly
     * depressed to a mapping (would only be in MODS_EFFECTIVE).
     * However, the traditional behavior of xkb_state_update_key() is that
     * if a vmod is depressed, its mappings are depressed with it; so we're
     * expected to do the same here.  Also, LEDs (usually) look if a real
     * mod is locked, not just effective; otherwise it won't be lit.
     */
    state->आधारः.components.आधारः_mods =
        resolve_गन्तव्यम्_canonical_mods(state->आधारः.keymap, आधारः_mods);
    state->आधारः.components.latched_mods =
        resolve_गन्तव्यम्_canonical_mods(state->आधारः.keymap, latched_mods);
    state->आधारः.components.locked_mods =
        resolve_गन्तव्यम्_canonical_mods(state->आधारः.keymap, locked_mods);

    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->आधारः.components.आधारः_गणः = (int32_t) आधारः_गणः;
    state->आधारः.components.latched_गणः = (int32_t) latched_गणः;
    state->आधारः.components.locked_गणः = (int32_t) locked_गणः;

    xkb_state_update_derived(&state->आधारः);

    प्रत्यागम get_state_component_changes(&पूर्व_components, &state->आधारः.components);
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Lock_Modifier
 */
स्थिर bool
should_do_caps_transformation(संरचना xkb_state *state, xkb_keycode_t kc)
{
    प्रत्यागम
        xkb_state_mod_सूचकांक_is_सक्रिय(state, XKB_MOD_सूचकांक_CAPS,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_सूचकांक_is_consumed(state, kc, XKB_MOD_सूचकांक_CAPS) == 0;
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Control_Modifier
 */
स्थिर bool
should_do_ctrl_transformation(संरचना xkb_state *state, xkb_keycode_t kc)
{
    प्रत्यागम
        xkb_state_mod_सूचकांक_is_सक्रिय(state, XKB_MOD_सूचकांक_CTRL,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_सूचकांक_is_consumed(state, kc, XKB_MOD_सूचकांक_CTRL) == 0;
}

/**
 * Provides the symbols to use for the given key and state.  Returns the
 * number of symbols pointed to in syms_out.
 */
पूर्णाङ्क
xkb_state_कुञ्चिका_get_syms(संरचना xkb_state *state, xkb_keycode_t kc,
                       ध्रुव xkb_keysym_t **syms_बहिर्गमनम्)
{
    ध्रुव xkb_layout_सूचकांक_t layout = xkb_state_कुञ्चिका_get_layout(state, kc);
    यदि (layout == XKB_LAYOUT_INVALID)
        गच्छ err;

    ध्रुव xkb_level_सूचकांक_t level = xkb_state_कुञ्चिका_get_level(state, kc, layout);
    यदि (level == XKB_LEVEL_INVALID)
        गच्छ err;

    ध्रुव संरचना xkb_कुञ्चिका* ध्रुव कुञ्चिका = Xkb_कुञ्चिका(state->keymap, kc);

    यदि (!कुञ्चिका)
        गच्छ err;

    ध्रुव संरचना xkb_level* ध्रुव leveli =
        xkb_keymap_कुञ्चिका_get_level(state->keymap, कुञ्चिका, layout, level);

    यदि (!leveli)
        गच्छ err;

    ध्रुव xkb_keysym_गणनम्_t अङ्कः_syms = leveli->अङ्कः_syms;
    यदि (अङ्कः_syms == 0)
        गच्छ err;

    यदि (should_do_caps_transformation(state, kc)) {
        /* Only simple capitalization rules: keysyms count is unchanged. */
        यदि (अङ्कः_syms > 1) {
            *syms_बहिर्गमनम् = (leveli->has_ऊर्ध्वस्थम्)
                      ? leveli->s.syms + अङ्कः_syms
                      : leveli->s.syms;
        } अन्यथा {
            *syms_बहिर्गमनम् = &leveli->ऊर्ध्वस्थम्;
        }
    } अन्यथा {
        *syms_बहिर्गमनम् = (अङ्कः_syms > 1)
                  ? leveli->s.syms
                  : &leveli->s.sym;
    }
    प्रत्यागम (पूर्णाङ्क) अङ्कः_syms;

err:
    *syms_बहिर्गमनम् = रिक्त;
    प्रत्यागम 0;
}

/*
 * Verbatim from `libX11:src/xkb/XKBBind.c`.
 *
 * The basic transformations are defined in “[Interpreting the Control Modifier]”.
 * They correspond to the [caret notation], which maps the characters
 * `@ABC...XYZ[\]^_` by masking them with `0x1f`. Note that there is no
 * transformation for `?`, although `^?` is defined in the [caret notation].
 *
 * For convenience, the range ```abc...xyz{|}~`` and the space character ` `
 * are processed the same way. This allow to produce control characters without
 * requiring the use of the `Shift` modifier for letters.
 *
 * The transformation of the digits seems to originate from the [VT220 terminal],
 * as a compatibility for non-US keyboards. Indeed, these keyboards may not have
 * the punctuation characters available or in a convenient position. Some mnemonics:
 *
 * - ^2 maps to ^@ because @ is on the key 2 in the US layout.
 * - ^6 maps to ^^ because ^ is on the key 6 in the US layout.
 * - characters 3, 4, 5, 6, and 7 seems to align with the sequence `[\]^_`.
 * - 8 closes the sequence and so maps to the last control character.
 *
 * The `/` transformation seems to be defined for compatibility or convenience.
 *
 * [Interpreting the Control Modifier]: https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Control_Modifier
 * [caret notation]: https://en.wikipedia.org/wiki/Caret_notation
 * [VT220 terminal]: https://vt100.net/docs/vt220-rm/chapter3.html#T3-5
 */
स्थिर वर्ण
Xkb_गन्तव्यम्_Control(वर्ण ch)
{
    वर्ण c = ch;

    यदि ((c >= '@' && c < '\177') || c == ' ')
        c &= 0x1F;
    अन्यथा यदि (c == '2')
        c = '\000';
    अन्यथा यदि (c >= '3' && c <= '7')
        c -= ('3' - '\033');
    अन्यथा यदि (c == '8')
        c = '\177';
    अन्यथा यदि (c == '/')
        c = '_' & 0x1F;
    प्रत्यागम c;
}

/**
 * Provides either exactly one symbol, or XKB_KEY_NoSymbol.
 */
xkb_keysym_t
xkb_state_कुञ्चिका_get_one_sym(संरचना xkb_state *state, xkb_keycode_t kc)
{
    ध्रुव xkb_keysym_t *syms = रिक्त;
    ध्रुव पूर्णाङ्क अङ्कः_syms = xkb_state_कुञ्चिका_get_syms(state, kc, &syms);

    यदि (अङ्कः_syms != 1)
        प्रत्यागम XKB_कुञ्चिका_No_चिह्नम्;
    अन्यथा
        प्रत्यागम syms[0];
}

/*
 * The caps and ctrl transformations require some special handling,
 * so we cannot simply use xkb_state_get_one_sym() for them.
 * In particular, if Control is set, we must try very hard to find
 * some layout in which the keysym is ASCII and thus can be (maybe)
 * converted to a control character. libX11 allows to disable this
 * behavior with the XkbLC_ControlFallback (see XkbSetXlibControls(3)),
 * but it is enabled by default, yippee.
 */
स्थिर xkb_keysym_t
get_one_sym_for_string(संरचना xkb_state *state, xkb_keycode_t kc)
{
    ध्रुव xkb_layout_सूचकांक_t layout = xkb_state_कुञ्चिका_get_layout(state, kc);
    ध्रुव xkb_layout_सूचकांक_t अङ्कः_layouts =
        xkb_keymap_अङ्कः_layouts_for_कुञ्चिका(state->keymap, kc);
    xkb_level_सूचकांक_t level = xkb_state_कुञ्चिका_get_level(state, kc, layout);
    यदि (layout == XKB_LAYOUT_INVALID || अङ्कः_layouts == 0 ||
        level == XKB_LEVEL_INVALID)
        प्रत्यागम XKB_कुञ्चिका_No_चिह्नम्;

    ध्रुव xkb_keysym_t *syms = रिक्त;
    पूर्णाङ्क nsyms = xkb_keymap_कुञ्चिका_get_syms_by_level(state->keymap, kc,
                                                 layout, level, &syms);
    यदि (nsyms != 1)
        प्रत्यागम XKB_कुञ्चिका_No_चिह्नम्;
    xkb_keysym_t sym = syms[0];

    यदि (should_do_ctrl_transformation(state, kc) && sym > 127u) {
        प्रति (xkb_layout_सूचकांक_t i = 0; i < अङ्कः_layouts; i++) {
            level = xkb_state_कुञ्चिका_get_level(state, kc, i);
            यदि (level == XKB_LEVEL_INVALID)
                अनुवर्त;

            nsyms = xkb_keymap_कुञ्चिका_get_syms_by_level(state->keymap, kc,
                                                     i, level, &syms);
            यदि (nsyms == 1 && syms[0] <= 127u) {
                sym = syms[0];
                भङ्ग;
            }
        }
    }

    यदि (should_do_caps_transformation(state, kc)) {
        sym = xkb_keysym_गन्तव्यम्_ऊर्ध्वस्थम्(sym);
    }

    प्रत्यागम sym;
}

पूर्णाङ्क
xkb_state_कुञ्चिका_get_utf8(संरचना xkb_state *state, xkb_keycode_t kc,
                       वर्ण *बफर, आकार_t आकार)
{
    पूर्णाङ्क nsyms;
    ध्रुव xkb_keysym_t *syms = रिक्त;
    ध्रुव xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    यदि (sym != XKB_कुञ्चिका_No_चिह्नम्) {
        nsyms = 1; syms = &sym;
    }
    अन्यथा {
        nsyms = xkb_state_कुञ्चिका_get_syms(state, kc, &syms);
    }

    /* Make sure not to truncate in the middle of a UTF-8 sequence. */
    पूर्णाङ्क विस्थापनम् = 0;
    वर्ण tmp[XKB_KEYSYM_UTF8_MAX_आकार];
    प्रति (पूर्णाङ्क i = 0; i < nsyms; i++) {
        पूर्णाङ्क ret = xkb_keysym_गन्तव्यम्_utf8(syms[i], tmp, परिमाण(tmp));
        यदि (ret <= 0)
            गच्छ err_bad;

        ret--;
        यदि ((आकार_t)विस्थापनम् + (आकार_t)ret <= आकार)
            स्मृतिं_प्रतिलिख(बफर + विस्थापनम्, tmp, (आकार_t)ret);
        विस्थापनम् += ret;
    }

    यदि ((आकार_t)विस्थापनम् >= आकार)
        गच्छ err_trunc;
    बफर[विस्थापनम्] = '\0';

    यदि (!is_valid_utf8(बफर, (आकार_t)विस्थापनम्))
        गच्छ err_bad;

    यदि (विस्थापनम् == 1 && (अचिह्नित पूर्णाङ्क) बफर[0] <= 127u &&
        should_do_ctrl_transformation(state, kc))
        बफर[0] = Xkb_गन्तव्यम्_Control(बफर[0]);

    प्रत्यागम विस्थापनम्;

err_trunc:
    यदि (आकार > 0)
        बफर[आकार - 1] = '\0';
    प्रत्यागम विस्थापनम्;

err_bad:
    यदि (आकार > 0)
        बफर[0] = '\0';
    प्रत्यागम 0;
}

uint32_t
xkb_state_कुञ्चिका_get_utf32(संरचना xkb_state *state, xkb_keycode_t kc)
{
    ध्रुव xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    uint32_t cp = xkb_keysym_गन्तव्यम्_utf32(sym);

    यदि (cp <= 127u && should_do_ctrl_transformation(state, kc))
        cp = (uint32_t) Xkb_गन्तव्यम्_Control((वर्ण) cp);

    प्रत्यागम cp;
}

/**
 * Serializes the requested modifier state into an xkb_mod_mask_t, with all
 * the same disclaimers as in xkb_state_update_mask.
 */
स्थिर अन्तःस्थ xkb_mod_आवरणम्_t
serialize_mods(ध्रुव संरचना state_components *components,
               गणना xkb_state_component type)
{
    xkb_mod_आवरणम्_t ret = 0;

    यदि (type & XKB_STATE_MODS_EFFECTIVE)
        प्रत्यागम components->mods;

    यदि (type & XKB_STATE_MODS_DEPRESSED)
        ret |= components->आधारः_mods;
    यदि (type & XKB_STATE_MODS_LATCHED)
        ret |= components->latched_mods;
    यदि (type & XKB_STATE_MODS_LOCKED)
        ret |= components->locked_mods;

    प्रत्यागम ret;
}

xkb_mod_आवरणम्_t
xkb_state_serialize_mods(संरचना xkb_state *state,
                         गणना xkb_state_component type)
{
    प्रत्यागम serialize_mods(&state->components, type);
}

/**
 * Serialises the requested group state, with all the same disclaimers as
 * in xkb_state_update_mask.
 */
स्थिर अन्तःस्थ xkb_layout_सूचकांक_t
serialize_layout(ध्रुव संरचना state_components *components,
                 गणना xkb_state_component type)
{
    यदि (type & XKB_STATE_LAYOUT_EFFECTIVE)
        प्रत्यागम components->गणः;

    int32_t ret = 0;
    यदि (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret += components->आधारः_गणः;
    यदि (type & XKB_STATE_LAYOUT_LATCHED)
        ret += components->latched_गणः;
    यदि (type & XKB_STATE_LAYOUT_LOCKED)
        ret += components->locked_गणः;

    प्रत्यागम (xkb_layout_सूचकांक_t)ret;
}

xkb_layout_सूचकांक_t
xkb_state_serialize_layout(संरचना xkb_state *state,
                           गणना xkb_state_component type)
{
    प्रत्यागम serialize_layout(&state->components, type);
}

xkb_led_आवरणम्_t
xkb_state_serialize_leds(ध्रुव संरचना xkb_state *state,
                         गणना xkb_state_component type)
{
    प्रत्यागम (type & XKB_STATE_LEDS)
        ? state->components.leds
        : 0;
}

स्थिर अन्तःस्थ गणना xkb_keyboard_control_flags
serialize_controls(ध्रुव संरचना state_components *components,
                   गणना xkb_state_component type)
{
    प्रत्यागम (type & XKB_STATE_CONTROLS_EFFECTIVE)
        /*
         * Enable using the public API with the all the Controls values, except
         * the internal ones, if any.
         */
        ? (गणना xkb_keyboard_control_flags)
          (components->controls & CONTROL_ALL_BOOLEAN)
        : 0;
}

गणना xkb_keyboard_control_flags
xkb_state_serialize_controls(ध्रुव संरचना xkb_state *state,
                             गणना xkb_state_component type)
{
    प्रत्यागम serialize_controls(&state->components, type);
}

/**
 * Gets a modifier mask and returns the resolved effective mask; this
 * is needed because some modifiers can also map to other modifiers, e.g.
 * the "NumLock" modifier usually also sets the "Mod2" modifier.
 */
xkb_mod_आवरणम्_t
mod_आवरणम्_get_effective(संरचना xkb_keymap *keymap, xkb_mod_आवरणम्_t mods)
{
    /* Initialize the effective mask with its corresponding real mods. */
    xkb_mod_आवरणम्_t आवरणम् = mods & MOD_REAL_आवरणम्_ALL;

    /* Resolve the virtual modifiers */
    ध्रुव संरचना xkb_mod *mod;
    xkb_mod_सूचकांक_t i;
    xkb_vmods_enumerate(i, mod, &keymap->mods)
        यदि (mods & (UINT32_C(1) << i))
            आवरणम् |= mod->mapping;

    प्रत्यागम आवरणम्;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
पूर्णाङ्क
xkb_state_mod_सूचकांक_is_सक्रिय(संरचना xkb_state *state,
                              xkb_mod_सूचकांक_t idx,
                              गणना xkb_state_component type)
{
    यदि (unlikely(idx >= xkb_keymap_अङ्कः_mods(state->keymap)))
        प्रत्यागम -1;

    ध्रुव xkb_mod_आवरणम्_t mapping = state->keymap->mods.mods[idx].mapping;
    यदि (!mapping) {
        /* Modifier not mapped */
        प्रत्यागम 0;
    }
    /* WARNING: this may overmatch for virtual modifiers */
    प्रत्यागम (xkb_state_serialize_mods(state, type) & mapping) == mapping;
}

गणना {
    XKB_STATE_मेलनम्_FLAGS = XKB_STATE_मेलनम्_ANY
                          | XKB_STATE_मेलनम्_ALL
                          | XKB_STATE_मेलनम्_NON_EXCLUSIVE
};

/**
 * Helper function for xkb_state_mod_indices_are_active and
 * xkb_state_mod_names_are_active.
 */
स्थिर bool
मेलनम्_mod_masks(संरचना xkb_state *state,
                गणना xkb_state_component type,
                गणना xkb_state_मेलनम् मेलनम्,
                xkb_mod_आवरणम्_t wanted)
{
    ध्रुव xkb_mod_आवरणम्_t सक्रिय = xkb_state_serialize_mods(state, type);

    यदि (!(मेलनम् & XKB_STATE_मेलनम्_NON_EXCLUSIVE) && (सक्रिय & ~wanted))
        प्रत्यागम false;

    यदि (मेलनम् & XKB_STATE_मेलनम्_ANY)
        प्रत्यागम सक्रिय & wanted;

    /* Ensure all flags but XKB_STATE_MATCH_ALL are processed before */
    static_assert(XKB_STATE_मेलनम्_ALL ==
                  ((गणना xkb_state_मेलनम्) XKB_STATE_मेलनम्_FLAGS &
                   ~(XKB_STATE_मेलनम्_ANY | XKB_STATE_मेलनम्_NON_EXCLUSIVE)),
                  "");

    प्रत्यागम (सक्रिय & wanted) == wanted;
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, or -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
पूर्णाङ्क
xkb_state_mod_indices_are_सक्रिय(संरचना xkb_state *state,
                                 गणना xkb_state_component type,
                                 गणना xkb_state_मेलनम् मेलनम्,
                                 ...)
{
    यदि (मेलनम् & ~((गणना xkb_state_मेलनम्) XKB_STATE_मेलनम्_FLAGS))
        प्रत्यागम -2;

    va_list ap;
    xkb_mod_आवरणम्_t wanted = 0;
    पूर्णाङ्क ret = 0;
    ध्रुव xkb_mod_सूचकांक_t अङ्कः_mods = xkb_keymap_अङ्कः_mods(state->keymap);

    va_start(ap, मेलनम्);
    यावत् (1) {
        xkb_mod_सूचकांक_t idx = va_arg(ap, xkb_mod_सूचकांक_t);
        यदि (idx == XKB_MOD_INVALID)
            भङ्ग;
        यदि (unlikely(idx >= अङ्कः_mods)) {
            ret = -1;
            भङ्ग;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_अन्तः(ap);

    यदि (ret == -1)
        प्रत्यागम ret;

    यदि (!wanted) {
        /* Modifiers not mapped */
        प्रत्यागम 0;
    }

    प्रत्यागम मेलनम्_mod_masks(state, type, मेलनम्, wanted);
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
पूर्णाङ्क
xkb_state_mod_name_is_सक्रिय(संरचना xkb_state *state, ध्रुव वर्ण *name,
                             गणना xkb_state_component type)
{
    ध्रुव xkb_mod_सूचकांक_t idx = xkb_keymap_mod_get_सूचकांक(state->keymap, name);

    यदि (idx == XKB_MOD_INVALID)
        प्रत्यागम -1;

    प्रत्यागम xkb_state_mod_सूचकांक_is_सक्रिय(state, idx, type);
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ATTR_NULL_SENTINEL पूर्णाङ्क
xkb_state_mod_नामानि_are_सक्रिय(संरचना xkb_state *state,
                               गणना xkb_state_component type,
                               गणना xkb_state_मेलनम् मेलनम्,
                               ...)
{
    यदि (मेलनम् & ~((गणना xkb_state_मेलनम्) XKB_STATE_मेलनम्_FLAGS))
        प्रत्यागम -2;

    va_list ap;
    xkb_mod_आवरणम्_t wanted = 0;
    पूर्णाङ्क ret = 0;

    va_start(ap, मेलनम्);
    यावत् (1) {
        ध्रुव वर्ण *str = va_arg(ap, ध्रुव वर्ण *);
        यदि (str == रिक्त)
            भङ्ग;
        ध्रुव xkb_mod_सूचकांक_t idx = xkb_keymap_mod_get_सूचकांक(state->keymap, str);
        यदि (idx == XKB_MOD_INVALID) {
            ret = -1;
            भङ्ग;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_अन्तः(ap);

    यदि (ret == -1)
        प्रत्यागम ret;

    यदि (!wanted) {
        /* Modifiers not mapped */
        प्रत्यागम 0;
    }

    प्रत्यागम मेलनम्_mod_masks(state, type, मेलनम्, wanted);
}

/**
 * Returns 1 if the given group is active with the specified type(s), 0 if
 * not, or -1 if the group is invalid.
 */
पूर्णाङ्क
xkb_state_layout_सूचकांक_is_सक्रिय(संरचना xkb_state *state,
                                 xkb_layout_सूचकांक_t idx,
                                 गणना xkb_state_component type)
{
    यदि (idx >= state->keymap->अङ्कः_groups)
        प्रत्यागम -1;

    पूर्णाङ्क ret = 0;
    यदि (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ret |= (state->components.गणः == idx);
    यदि (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret |= (state->components.आधारः_गणः == (int32_t) idx);
    यदि (type & XKB_STATE_LAYOUT_LATCHED)
        ret |= (state->components.latched_गणः == (int32_t) idx);
    यदि (type & XKB_STATE_LAYOUT_LOCKED)
        ret |= (state->components.locked_गणः == (int32_t) idx);

    प्रत्यागम ret;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
पूर्णाङ्क
xkb_state_layout_name_is_सक्रिय(संरचना xkb_state *state, ध्रुव वर्ण *name,
                                गणना xkb_state_component type)
{
    ध्रुव xkb_layout_सूचकांक_t idx =
        xkb_keymap_layout_get_सूचकांक(state->keymap, name);

    यदि (idx == XKB_LAYOUT_INVALID)
        प्रत्यागम -1;

    प्रत्यागम xkb_state_layout_सूचकांक_is_सक्रिय(state, idx, type);
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
पूर्णाङ्क
xkb_state_led_सूचकांक_is_सक्रिय(संरचना xkb_state *state, xkb_led_सूचकांक_t idx)
{
    यदि (idx >= state->keymap->अङ्कः_leds ||
        state->keymap->leds[idx].name == XKB_ATOM_NONE)
        प्रत्यागम -1;

    प्रत्यागम !!(state->components.leds & (UINT32_C(1) << idx));
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
पूर्णाङ्क
xkb_state_led_name_is_सक्रिय(संरचना xkb_state *state, ध्रुव वर्ण *name)
{
    ध्रुव xkb_led_सूचकांक_t idx = xkb_keymap_led_get_सूचकांक(state->keymap, name);

    यदि (idx == XKB_LED_INVALID)
        प्रत्यागम -1;

    प्रत्यागम xkb_state_led_सूचकांक_is_सक्रिय(state, idx);
}

/**
 * See:
 * - XkbTranslateKeyCode(3), mod_rtrn return value, from libX11.
 * - MyEnhancedXkbTranslateKeyCode(), a modification of the above, from GTK+.
 */
स्थिर xkb_mod_आवरणम्_t
कुञ्चिका_get_consumed(संरचना xkb_state *state, ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                 गणना xkb_consumed_बहुलकम् बहुलकम्)
{
    ध्रुव xkb_layout_सूचकांक_t गणः =
        xkb_state_कुञ्चिका_get_layout(state, कुञ्चिका->keycode);
    यदि (गणः == XKB_LAYOUT_INVALID)
        प्रत्यागम 0;

    xkb_mod_आवरणम्_t preserve = 0;
    xkb_mod_आवरणम्_t consumed = 0;

    ध्रुव संरचना xkb_कुञ्चिका_type_प्रविष्टिः* ध्रुव matching_प्रविष्टिः =
        get_प्रविष्टिः_for_कुञ्चिका_state(state, कुञ्चिका, गणः);
    यदि (matching_प्रविष्टिः)
        preserve = matching_प्रविष्टिः->preserve.आवरणम्;

    ध्रुव संरचना xkb_कुञ्चिका_type* ध्रुव type = कुञ्चिका->groups[गणः].type;
    विकल्प (बहुलकम्) {
    पक्ष XKB_CONSUMED_बहुलकम्_XKB:
        consumed = type->mods.आवरणम्;
        भङ्ग;

    पक्ष XKB_CONSUMED_बहुलकम्_GTK: {
        ध्रुव संरचना xkb_कुञ्चिका_type_प्रविष्टिः* ध्रुव no_mods_प्रविष्टिः =
            get_प्रविष्टिः_for_mods(type, 0);
        ध्रुव xkb_level_सूचकांक_t no_mods_leveli = no_mods_प्रविष्टिः
                                               ? no_mods_प्रविष्टिः->level
                                               : 0;
        ध्रुव संरचना xkb_level* ध्रुव no_mods_level =
            &कुञ्चिका->groups[गणः].स्तराः[no_mods_leveli];

        प्रति (darray_आकार_t i = 0; i < type->अङ्कः_entries; i++) {
            ध्रुव संरचना xkb_कुञ्चिका_type_प्रविष्टिः* ध्रुव प्रविष्टिः = &type->entries[i];
            यदि (!प्रविष्टिः_is_सक्रिय(प्रविष्टिः))
                अनुवर्त;

            ध्रुव संरचना xkb_level* ध्रुव level =
                &कुञ्चिका->groups[गणः].स्तराः[प्रविष्टिः->level];
            यदि (Xkb_स्तराः_Same_Syms(level, no_mods_level))
                अनुवर्त;

            यदि (प्रविष्टिः == matching_प्रविष्टिः || one_bit_समुच्चयः(प्रविष्टिः->mods.आवरणम्))
                consumed |= प्रविष्टिः->mods.आवरणम् & ~प्रविष्टिः->preserve.आवरणम्;
        }
        भङ्ग;
    }
    }

    प्रत्यागम consumed & ~preserve;
}

पूर्णाङ्क
xkb_state_mod_सूचकांक_is_consumed2(संरचना xkb_state *state, xkb_keycode_t kc,
                                 xkb_mod_सूचकांक_t idx,
                                 गणना xkb_consumed_बहुलकम् बहुलकम्)
{
    ध्रुव संरचना xkb_कुञ्चिका* ध्रुव कुञ्चिका = Xkb_कुञ्चिका(state->keymap, kc);

    यदि (unlikely(!कुञ्चिका || idx >= xkb_keymap_अङ्कः_mods(state->keymap)))
        प्रत्यागम -1;

    ध्रुव xkb_mod_आवरणम्_t mapping = state->keymap->mods.mods[idx].mapping;
    यदि (!mapping) {
        /* Modifier not mapped */
        प्रत्यागम 0;
    }
    प्रत्यागम (mapping & कुञ्चिका_get_consumed(state, कुञ्चिका, बहुलकम्)) == mapping;
}

पूर्णाङ्क
xkb_state_mod_सूचकांक_is_consumed(संरचना xkb_state *state, xkb_keycode_t kc,
                                xkb_mod_सूचकांक_t idx)
{
    प्रत्यागम xkb_state_mod_सूचकांक_is_consumed2(state, kc, idx,
                                            XKB_CONSUMED_बहुलकम्_XKB);
}

xkb_mod_आवरणम्_t
xkb_state_mod_आवरणम्_remove_consumed(संरचना xkb_state *state, xkb_keycode_t kc,
                                   xkb_mod_आवरणम्_t आवरणम्)
{
    ध्रुव संरचना xkb_कुञ्चिका* ध्रुव कुञ्चिका = Xkb_कुञ्चिका(state->keymap, kc);

    यदि (!कुञ्चिका)
        प्रत्यागम 0;

    प्रत्यागम resolve_गन्तव्यम्_canonical_mods(state->keymap, आवरणम्) &
           ~कुञ्चिका_get_consumed(state, कुञ्चिका, XKB_CONSUMED_बहुलकम्_XKB);
}

xkb_mod_आवरणम्_t
xkb_state_कुञ्चिका_get_consumed_mods2(संरचना xkb_state *state, xkb_keycode_t kc,
                                 गणना xkb_consumed_बहुलकम् बहुलकम्)
{
    विकल्प (बहुलकम्) {
    पक्ष XKB_CONSUMED_बहुलकम्_XKB:
    पक्ष XKB_CONSUMED_बहुलकम्_GTK:
        भङ्ग;
    स्वभाव:
        log_err_func(state->keymap->ctx, XKB_LOG_सन्देशः_NO_ID,
                     "unrecognized consumed modifiers mode: %d\n", बहुलकम्);
        प्रत्यागम 0;
    }

    ध्रुव संरचना xkb_कुञ्चिका* ध्रुव कुञ्चिका = Xkb_कुञ्चिका(state->keymap, kc);
    यदि (!कुञ्चिका)
        प्रत्यागम 0;

    प्रत्यागम कुञ्चिका_get_consumed(state, कुञ्चिका, बहुलकम्);
}

xkb_mod_आवरणम्_t
xkb_state_कुञ्चिका_get_consumed_mods(संरचना xkb_state *state, xkb_keycode_t kc)
{
    प्रत्यागम xkb_state_कुञ्चिका_get_consumed_mods2(state, kc, XKB_CONSUMED_बहुलकम्_XKB);
}

/*******************************************************************************
 * Server state machine API
 ******************************************************************************/

/** Entry to keep track of the state of an overlaid key */
संरचना xkb_overlaid_कुञ्चिका {
    /** The source key being overlaid */
    ध्रुव संरचना xkb_कुञ्चिका *old;
    /** The target key */
    ध्रुव संरचना xkb_कुञ्चिका *new;
    /** Counter of key currently down */
    पूर्णाङ्क refcnt;
};

संरचना xkb_shortcuts_config_प्रविष्टिः {
    /** Real modifier mask to trigger shortcuts tweaks */
    xkb_mod_आवरणम्_t mods;
    /** Target layout */
    xkb_layout_सूचकांक_t target;
};

/*
 * `xkb_machine` have a similar role as the `xkb_state` state machine and is
 * indeed currently only a simple wrapper. However, having a separate type:
 *
 * - Dissociates the *observable* state associated to an atomic event from
 *   the *internal* state of the machine that generated it. Indeed, the later
 *   may have been updated since the event generation.
 * - Clearly draw a limit between the client and server APIs.
 * - Add further features without modifying `xkb_state`, which is already
 *   bloated for *clients* applications.
 */
संरचना xkb_machine {
    /** Inherits from `xkb_server_state` */
    संरचना xkb_server_state आधारः;

    /* Extra server-specific state data */

    /** Keyboard overlays handling */
    संरचना {
        /**
         * Current overlaid keys
         *
         * When a key is overlaid, it remained overlaid in this configuration
         * until totally released, i.e. every key press was matched with the
         * corresponding key release.
         */
        darray(संरचना xkb_overlaid_कुञ्चिका) कुञ्चिकाः;
        /**
         * Activation order of the overlay
         *
         * Overlays indices are stored 1-indexed in nibbles: the lowest
         * nibble corresponds to the latest activated index.
         */
        uint32_t क्रमः;
        /** Current enabled overlays mask */
        xkb_overlay_आवरणम्_t enabled;
    } overlays;

    /** Configuration */
    संरचना machine_config {
        /** Modifiers remapping */
        संरचना machine_modifiers_config {
            /** Real modifier mask */
            xkb_mod_आवरणम्_t आवरणम्;
            /** Modifiers re-mappings */
            darray_आकार_t mappings_अङ्कः;
            संरचना machine_mods_mapping {
                xkb_mod_आवरणम्_t source;
                xkb_mod_आवरणम्_t target;
            } * mappings ATTR_COUNTED_BY(mappings_अङ्कः);
        } modifiers;

        /** Shortcuts tweak */
        संरचना machine_shortcuts_config {
            /** Substitution entries */
            संरचना xkb_shortcuts_config_प्रविष्टिः *entries;
        } shortcuts;
    } config;

    /** Mouse keys handling */
    संरचना {
        /** Default pointer button */
        xkb_pointer_button_सूचकांक_t default_button;
        /** Mask of the locked buttons */
        xkb_pointer_button_आवरणम्_t locked_buttons;
    } mouse;

    गणना xkb_machine_flags flags;
};

प्रकारनिर्देश darray(संरचना machine_mods_mapping) machine_mods_mappings;

संरचना xkb_machine_builder {
    संरचना xkb_keymap *keymap;

    /** Controls */
    संरचना {
        /** Accessibility flags */
        संरचना {
            गणना xkb_a11y_flags affect_flags;
            गणना xkb_a11y_flags flags;
        } a11y;
    } controls;

    /** Modifiers re-mapping */
    machine_mods_mappings mods;

    /** Shortcuts tweak */
    संरचना xkb_shortcuts_config_options {
        /** Substitution entries */
        darray(संरचना xkb_shortcuts_config_प्रविष्टिः) entries;
    } shortcuts;

    गणना xkb_machine_flags machine_flags;
    गणना xkb_machine_builder_flags builder_flags;
    पूर्णाङ्क refcnt;
};

संरचना xkb_machine_builder *
xkb_machine_builder_new(
    संरचना xkb_keymap * सीमित keymap,
    ध्रुव संरचना xkb_machine_builder_config * सीमित config,
    गणना xkb_स्थितिः * सीमित स्थितिः
)
{
    /* Handle default configuration */
    स्थिर ध्रुव संरचना xkb_machine_builder_config default_config = {
        .आकार = परिमाण(default_config)
    };
    यदि (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ध्रुव गणना xkb_स्थितिः स्थितिः_2 = xkb_check_state_abi(config);
    यदि (स्थितिः_2) {
        xkb_log_abi_error(keymap->ctx, __func__, स्थितिः_2);
        यदि (स्थितिः)
            *स्थितिः = स्थितिः_2;
        प्रत्यागम रिक्त;
    }

    /* Sanitize input */
    ध्रुव गणना xkb_machine_builder_flags invalid_builder_flags
        = config->builder_flags
        & ~(गणना xkb_machine_builder_flags)XKB_MACHINE_BUILDER_FLAGS_मानानि;
    यदि (invalid_builder_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS_,
                     "unrecognized machine builder flags: 0x%x\n",
                     invalid_builder_flags);
        यदि (स्थितिः)
            *स्थितिः = XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS;
        प्रत्यागम रिक्त;
    }

    ध्रुव गणना xkb_machine_flags invalid_machine_flags
        = (गणना xkb_machine_flags)config->machine_flags
        & ~(गणना xkb_machine_flags)XKB_MACHINE_FLAGS_मानानि;
    यदि (invalid_machine_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS_,
                     "unrecognized state machine flags: 0x%x\n",
                     invalid_machine_flags);
        यदि (स्थितिः)
            *स्थितिः = XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS;
        प्रत्यागम रिक्त;
    }

    संरचना xkb_machine_builder * ध्रुव builder = शून्यस्मृतिं_देहि(1, परिमाण(*builder));
    यदि (!builder) {
        log_err_func1(keymap->ctx, XKB_ERROR_आवण्टनम्_FAILURE_2,
                      "cannot allocate machine builder\n");
        यदि (स्थितिः)
            *स्थितिः = XKB_ERROR_आवण्टनम्_FAILURE;
        प्रत्यागम रिक्त;
    }

    *builder = (संरचना xkb_machine_builder) {
        .keymap = xkb_keymap_ref(keymap),
        .refcnt = 1,
        .builder_flags = (गणना xkb_machine_builder_flags)config->builder_flags,
        .machine_flags = (गणना xkb_machine_flags)config->machine_flags,
        .controls = {
            .a11y = {
                .affect_flags = XKB_A11Y_NO_FLAGS,
                .flags = XKB_A11Y_NO_FLAGS,
            },
        },
        .mods = darray_new(),
        .shortcuts = {
            .entries = darray_new(),
        },
    };

    यदि (स्थितिः)
        *स्थितिः = XKB_SUCCESS;

    प्रत्यागम builder;
}

संरचना xkb_machine_builder *
xkb_machine_builder_ref(संरचना xkb_machine_builder *builder)
{
    प्रतिज्ञा(builder->refcnt > 0);
    builder->refcnt++;
    प्रत्यागम builder;
}

शून्य
xkb_machine_builder_unref(संरचना xkb_machine_builder *builder)
{
    प्रतिज्ञा(!builder || builder->refcnt > 0);
    यदि (!builder || --builder->refcnt > 0)
        प्रत्यागम;

    darray_free(builder->shortcuts.entries);
    darray_free(builder->mods);
    xkb_keymap_unref(builder->keymap);
    मुक्त(builder);
}

संरचना xkb_keymap *
xkb_machine_builder_get_keymap(ध्रुव संरचना xkb_machine_builder *builder)
{
    /* Reference count is not updated. See API doc. */
    प्रत्यागम builder->keymap;
}

गणना xkb_स्थितिः
xkb_machine_builder_update_a11y(
    संरचना xkb_machine_builder * सीमित builder,
    ध्रुव संरचना xkb_machine_builder_a11y_update * सीमित update)
{
    /* Check ABI compatibility */
    गणना xkb_स्थितिः स्थितिः = xkb_check_state_abi(update);
    यदि (स्थितिः != XKB_SUCCESS) {
        xkb_log_abi_error(builder->keymap->ctx, __func__, स्थितिः);
        प्रत्यागम स्थितिः;
    }

    ध्रुव गणना xkb_a11y_flags invalid_flags =
        ~(गणना xkb_a11y_flags)XKB_A11_Y_FLAGS_मानानि;

    यदि (update->affect_flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y affected flags: %#x\n",
                __func__, update->affect_flags & invalid_flags);
        प्रत्यागम XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }
    यदि (update->flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y flags: %#x\n",
                __func__, update->flags & invalid_flags);
        प्रत्यागम XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }

    builder->controls.a11y.affect_flags |= update->affect_flags;
    builder->controls.a11y.flags &= ~update->affect_flags;
    builder->controls.a11y.flags |= (update->flags & update->affect_flags);

    प्रत्यागम XKB_SUCCESS;
}

गणना xkb_स्थितिः
xkb_machine_builder_update_mods_remap(
    संरचना xkb_machine_builder * सीमित builder,
    ध्रुव संरचना xkb_machine_builder_mods_remap_update * सीमित update
)
{
    यदि (!update->source) {
        यदि (!update->target) {
            /* Reset mappings */
            darray_resize(builder->mods, 0);
            प्रत्यागम XKB_SUCCESS;
        } अन्यथा {
            प्रत्यागम XKB_ERROR_UNSUPPORTED_MODIFIER_आवरणम्;
        }
    }

    /* Check the modifiers against the keymap */
    ध्रुव xkb_mod_आवरणम्_t invalid = ~builder->keymap->canonical_state_आवरणम्;
    यदि ((update->source & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_आवरणम्,
                "%s: Invalid source modifiers: 0x%"PRIx32"\n",
                __func__, update->source);
        प्रत्यागम XKB_ERROR_UNSUPPORTED_MODIFIER_आवरणम्;
    }
    यदि ((update->target & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_आवरणम्,
                "%s: Invalid target modifiers: 0x%"PRIx32"\n",
                __func__, update->target);
        प्रत्यागम XKB_ERROR_UNSUPPORTED_MODIFIER_आवरणम्;
    }

    संरचना machine_mods_mapping *mapping = रिक्त;
    darray_आकार_t m = 0;
    darray_enumerate(m, mapping, builder->mods) {
        यदि (mapping->source == update->source) {
            यदि (!update->target) {
                /* Remove mapping */
                darray_remove(builder->mods, m);
            } अन्यथा {
                /* Update mapping */
                mapping->target = update->target;
            }
            प्रत्यागम XKB_SUCCESS;
        }
    }

    यदि (update->target) {
        /* Append new mapping */
        darray_append(
            builder->mods,
            (संरचना machine_mods_mapping) {
                .source = update->source,
                .target = update->target
            }
        );
    } अन्यथा {
        /* Ignore missing mapping */
    }

    प्रत्यागम XKB_SUCCESS;
}

गणना xkb_स्थितिः
xkb_machine_builder_update_shortcut_override(
    संरचना xkb_machine_builder * सीमित builder,
    ध्रुव संरचना xkb_machine_builder_shortcut_override_update * सीमित update
)
{
    संरचना xkb_keymap *keymap = builder->keymap;

    /* Check the modifiers against the keymap */
    ध्रुव xkb_mod_आवरणम्_t invalid = ~keymap->canonical_state_आवरणम्;
    यदि ((update->affect_mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_आवरणम्,
                "%s: Invalid affected modifiers: 0x%"PRIx32"\n",
                __func__, update->affect_mods);
        प्रत्यागम XKB_ERROR_UNSUPPORTED_MODIFIER_आवरणम्;
    }
    यदि ((update->mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_आवरणम्,
                "%s: Invalid modifiers: 0x%"PRIx32"\n",
                __func__, update->mods);
        प्रत्यागम XKB_ERROR_UNSUPPORTED_MODIFIER_आवरणम्;
    }

    /* Check the layout indices against the keymap */
    यदि (update->source >= keymap->अङ्कः_groups &&
        update->source != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                "%s: Invalid source layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->अङ्कः_groups, update->source + 1);
        प्रत्यागम XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक;
    }
    यदि (update->target >= keymap->अङ्कः_groups &&
        update->target != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                "%s: Invalid target layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->अङ्कः_groups, update->target + 1);
        प्रत्यागम XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक;
    }

    संरचना xkb_shortcuts_config_options * ध्रुव config = &builder->shortcuts;

    /* Resize array & initialize new entries, if relevant */
    यदि (update->source >= darray_आकार(config->entries) &&
        update->source != XKB_LAYOUT_INVALID) {
        यदि (update->target == update->source) {
            /* Skip default setting */
            प्रत्यागम XKB_SUCCESS;
        }
        xkb_layout_सूचकांक_t new = darray_आकार(config->entries);
        darray_resize(config->entries, update->source + 1);
        प्रति (; new <= update->source; new++) {
            darray_item(config->entries, new) =
                (संरचना xkb_shortcuts_config_प्रविष्टिः) {
                    .target = XKB_LAYOUT_INVALID,
                    .mods = 0
                };
        }
    }

    संरचना xkb_shortcuts_config_प्रविष्टिः *प्रविष्टिः;
    xkb_layout_सूचकांक_t source = (update->source == XKB_LAYOUT_INVALID)
        ? 0               /* Loop over all entries */
        : update->source; /* Single entry */
    darray_foreach_from(प्रविष्टिः, config->entries, source) {
        यदि (update->source == XKB_LAYOUT_INVALID &&
            प्रविष्टिः->target == XKB_LAYOUT_INVALID) {
            /* Skip previous deactivated entry */
            source++;
            अनुवर्त;
        }

        यदि (update->affect_mods) {
            प्रविष्टिः->mods &= ~update->affect_mods;
            प्रविष्टिः->mods |= (update->affect_mods & update->mods);
        }

        यदि (update->source != XKB_LAYOUT_INVALID) {
            प्रविष्टिः->target = (update->target == source)
                ? XKB_LAYOUT_INVALID
                : update->target;
            /* Single entry: exit loop */
            भङ्ग;
        }
        source++;
    }

    प्रत्यागम XKB_SUCCESS;
}

/** Compare 2 modifiers combinations */
स्थिर पूर्णाङ्क
cmp_mod_masks(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
    ध्रुव xkb_mod_आवरणम्_t m1 = *((xkb_mod_आवरणम्_t *) a);
    ध्रुव xkb_mod_आवरणम्_t m2 = *((xkb_mod_आवरणम्_t *) b);
    यदि (m1 == m2)
        प्रत्यागम 0;

    यदि (!m1)
        प्रत्यागम +1;
    यदि (!m2)
        प्रत्यागम -1;

    ध्रुव xkb_mod_आवरणम्_t overlap = (m1 & m2);
    यदि (overlap == m1)
        प्रत्यागम +1; /* m1 is a subset of m2 */
    यदि (overlap == m2)
        प्रत्यागम -1; /* m2 is a subset of m1 */

    /* Compare active modifiers numbers */
    ध्रुव अचिह्नित पूर्णाङ्क count1 = popcount32(m1);
    ध्रुव अचिह्नित पूर्णाङ्क count2 = popcount32(m2);
    यदि (count1 > count2)
        प्रत्यागम -1;
    यदि (count1 < count2)
        प्रत्यागम +1;

    /* Give priority to real modifiers */
    प्रत्यागम (m1 < m2) ? -1 : +1;
}

स्थिर गणना xkb_स्थितिः
machine_समुच्चयः_mods(संरचना xkb_machine *sm,
                 ध्रुव machine_mods_mappings *raw_mappings)
{
    यदि (!darray_रिक्तानि(*raw_mappings)) {
        machine_mods_mappings mappings = darray_new();
        xkb_mod_आवरणम्_t आवरणम् = 0;
        ध्रुव संरचना machine_mods_mapping *mapping;
        ध्रुव xkb_mod_आवरणम्_t invalid =
            ~sm->आधारः.आधारः.keymap->canonical_state_आवरणम्;
        darray_foreach(mapping, *raw_mappings) {
            यदि (!mapping->source || !mapping->target ||
                (mapping->source & invalid) || (mapping->target & invalid))
                अनुवर्त;
            darray_append(mappings, *mapping);
            आवरणम् |= mapping->source;
        }

        /*
         * Re-order the mappings, so that combinations have higher priority
         * if they are superset of other combinations.
         * For other type of overlaps, see the sorting function.
         */
        शीघ्रं_क्रमय(darray_वस्तूनि(mappings), darray_आकार(mappings),
              परिमाण(*darray_वस्तूनि(mappings)), &cmp_mod_masks);

        darray_steal(
            mappings,
            &sm->config.modifiers.mappings,
            &sm->config.modifiers.mappings_अङ्कः
        );
        sm->config.modifiers.आवरणम् = आवरणम्;
    } अन्यथा {
        sm->config.modifiers = (संरचना machine_modifiers_config) {
            .mappings = रिक्त,
            .mappings_अङ्कः = 0,
            .आवरणम् = 0
        };
    }

    प्रत्यागम XKB_SUCCESS;
}

स्थिर गणना xkb_स्थितिः
machine_समुच्चयः_shortcuts(
    संरचना xkb_machine * सीमित sm,
    ध्रुव संरचना xkb_shortcuts_config_options * सीमित options
)
{
    यदि (darray_रिक्तानि(options->entries)) {
        sm->config.shortcuts = (संरचना machine_shortcuts_config) {
            .entries = रिक्त
        };
        प्रत्यागम XKB_SUCCESS;
    }

    संरचना xkb_keymap * ध्रुव keymap = sm->आधारः.आधारः.keymap;

    संरचना xkb_shortcuts_config_प्रविष्टिः * entries =
        शून्यस्मृतिं_देहि(keymap->अङ्कः_groups, परिमाण(*entries));
    यदि (!entries)
        प्रत्यागम XKB_ERROR_आवण्टनम्_FAILURE;

    ध्रुव xkb_layout_सूचकांक_t गणनम् = MIN(
        keymap->अङ्कः_groups,
        (xkb_layout_सूचकांक_t) darray_आकार(options->entries)
    );

    bool some_entries = false;
    प्रति (xkb_layout_सूचकांक_t l = 0; l < गणनम्; l++) {
        /* Sanitize layout target */
        ध्रुव xkb_layout_सूचकांक_t target =
            darray_item(options->entries, l).target;
        /* Sanitize modifier mask */
        ध्रुव xkb_mod_आवरणम्_t mods = (
            darray_item(options->entries, l).mods &
            sm->आधारः.आधारः.keymap->canonical_state_आवरणम्
        );
        यदि (target < keymap->अङ्कः_groups && target != l && mods) {
            entries[l] = (संरचना xkb_shortcuts_config_प्रविष्टिः) {
                .target = target,
                .mods = mods
            };
            some_entries = true;
        } अन्यथा {
            entries[l] = (संरचना xkb_shortcuts_config_प्रविष्टिः) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
            अनुवर्त;
        }
    }

    यदि (!some_entries) {
        मुक्त(entries);
        entries = रिक्त;
    } अन्यथा {
        /* Initialize groups with no entries */
        प्रति (xkb_layout_सूचकांक_t l = गणनम्; l < keymap->अङ्कः_groups; l++) {
            entries[l] = (संरचना xkb_shortcuts_config_प्रविष्टिः) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
        }
    }

    sm->config.shortcuts = (संरचना machine_shortcuts_config) {
        .entries = entries
    };
    प्रत्यागम XKB_SUCCESS;
}

संरचना xkb_machine *
xkb_machine_new(ध्रुव संरचना xkb_machine_builder * सीमित builder,
                गणना xkb_स्थितिः * सीमित स्थितिः)
{
    संरचना xkb_machine * ध्रुव machine = शून्यस्मृतिं_देहि(1, परिमाण(*machine));
    यदि (!machine) {
        log_err_func1(builder->keymap->ctx, XKB_ERROR_आवण्टनम्_FAILURE_2,
                      "cannot allocate machine\n");
        यदि (स्थितिः)
            *स्थितिः = XKB_ERROR_आवण्टनम्_FAILURE;
        प्रत्यागम रिक्त;
    }

    xkb_server_state_init(&machine->आधारः, builder->keymap, SERVER_STATE,
                          builder->controls.a11y.affect_flags,
                          builder->controls.a11y.flags);
    machine->flags = builder->machine_flags;
    यदि (!(machine->flags &
          XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_कुञ्चिका_FIX))
    {
        machine->आधारः.flags |= STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_कुञ्चिका;
    }
    यदि (machine->flags & XKB_MACHINE_UNLOCK_MODS_ON_PRESS) {
        machine->आधारः.flags |= STATE_UNLOCK_MODS_ON_PRESS;
    }

    गणना xkb_स्थितिः स्थितिः_2;
    यदि ((स्थितिः_2 = machine_समुच्चयः_mods(machine, &builder->mods)) != XKB_SUCCESS ||
        (स्थितिः_2 = machine_समुच्चयः_shortcuts(machine, &builder->shortcuts)) !=
        XKB_SUCCESS) {
        *स्थितिः = स्थितिः_2;
        गच्छ error;
    }

    darray_init(machine->overlays.कुञ्चिकाः);
    machine->mouse.default_button = XKB_POINTER_BUTTON_MIN;

    यदि (स्थितिः)
        *स्थितिः = XKB_SUCCESS;

    प्रत्यागम machine;

error:
    xkb_machine_unref(machine);
    प्रत्यागम रिक्त;
}

संरचना xkb_machine *
xkb_machine_ref(संरचना xkb_machine *sm)
{
    प्रतिज्ञा(sm->आधारः.आधारः.refcnt > 0);
    sm->आधारः.आधारः.refcnt++;
    प्रत्यागम sm;
}

शून्य
xkb_machine_unref(संरचना xkb_machine *sm)
{
    प्रतिज्ञा(!sm || sm->आधारः.आधारः.refcnt > 0);
    यदि (!sm || --sm->आधारः.आधारः.refcnt > 0)
        प्रत्यागम;

    xkb_state_destroy(&sm->आधारः.आधारः);
    darray_free(sm->overlays.कुञ्चिकाः);
    मुक्त(sm->config.shortcuts.entries);
    मुक्त(sm->config.modifiers.mappings);
    मुक्त(sm);
}

संरचना xkb_keymap *
xkb_machine_get_keymap(ध्रुव संरचना xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    प्रत्यागम sm->आधारः.आधारः.keymap;
}

संरचना xkb_state *
xkb_machine_get_state(संरचना xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    प्रत्यागम (संरचना xkb_state *)sm;
}

संरचना xkb_state *
xkb_state_new_from_machine(ध्रुव संरचना xkb_machine * सीमित machine,
                           गणना xkb_स्थितिः * सीमित स्थितिः)
{
    /* Creation */
    गणना xkb_स्थितिः स्थितिः_2;
    संरचना xkb_state * ध्रुव state = xkb_state_new_with_बहुलकम्(
        machine->आधारः.आधारः.keymap,
        XKB_STATE_बहुलकम्_SERVER_QUERY,
        &स्थितिः_2
    );
    यदि (स्थितिः_2 != XKB_SUCCESS) {
        यदि (स्थितिः)
            *स्थितिः = स्थितिः_2;
        प्रत्यागम रिक्त;
    }

    प्रतिज्ञा(state->बहुलकम् == SERVER_COMPANION);
    प्रतिज्ञा(state->refcnt == 1);

    /* Copy machine’s state */
    *state = machine->आधारः.आधारः;

    /* Restore previous values */
    state->बहुलकम् = SERVER_COMPANION;
    state->refcnt = 1;

    यदि (स्थितिः)
        *स्थितिः = XKB_SUCCESS;
    प्रत्यागम state;
}

स्थिर शून्य
machine_update_overlays(संरचना xkb_machine *sm)
{
    /*
     * Overlays indices are stored 1-indexed in nibbles: the lowest
     * nibble corresponds to the latest activated index.
     */

    ध्रुव xkb_overlay_आवरणम्_t आवरणम् =
        overlays_from_controls(sm->आधारः.आधारः.components.controls);
    xkb_overlay_आवरणम्_t added = आवरणम् & ~sm->overlays.enabled;

    /* Remove overlays no longer enabled and keep relative order */
    uint32_t क्रमः = sm->overlays.क्रमः;
    ध्रुव xkb_overlay_सूचकांक_t overlay_max =
        format_max_overlays(sm->आधारः.आधारः.keymap->format);
    प्रति (uint8_t n = 0; n < overlay_max;) {
        xkb_overlay_सूचकांक_t overlay_idx = (क्रमः >> (n * 4)) & 0xf;
        यदि (!overlay_idx)
            भङ्ग;
        ध्रुव xkb_overlay_आवरणम्_t overlay_आवरणम् =
            (xkb_overlay_आवरणम्_t)(1u << (overlay_idx - 1u) /* k is 1-indexed */);
        यदि (overlay_आवरणम् & आवरणम्) {
            /* no duplicates */
            added &= ~overlay_आवरणम्;
            n++;
        } अन्यथा {
            /* remove overlay */
            ध्रुव uint32_t शीर्ष = (UINT32_C(1) << (n * 4)) - 1;
            क्रमः = (क्रमः & शीर्ष) | ((क्रमः >> 4) & ~शीर्ष);
        }
    }

    /* Add new overlays in ascending order */
    प्रति (xkb_overlay_सूचकांक_t k = 0; added; k++, added >>= 1) {
        यदि (added & 0x1) {
            क्रमः <<= 4;
            क्रमः |= k + 1u; /* k is 0-indexed */
        }
    }

    sm->overlays.क्रमः = क्रमः;
    sm->overlays.enabled = आवरणम्;
}

गणना xkb_स्थितिः
xkb_machine_process_synthetic(संरचना xkb_machine *sm,
                              ध्रुव संरचना xkb_synthetic_update *update,
                              संरचना xkb_events *events)
{
    /* Check ABI compatibility */
    गणना xkb_स्थितिः स्थितिः =
        check_state_update_abi(sm->आधारः.आधारः.keymap->ctx, update);
    यदि (स्थितिः != XKB_SUCCESS)
        प्रत्यागम स्थितिः;

    संरचना xkb_server_state * ध्रुव state = &sm->आधारः;
    ध्रुव संरचना state_components previous_components = state->आधारः.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    darray_आकार(events->queue) = 0;
    events->अग्र = 0;

    /* Update parametrized controls first */
    यदि (update->layout_policy) {
        स्थितिः = state_update_layout_policy(state, update->layout_policy);
        यदि (स्थितिः != XKB_SUCCESS)
            प्रत्यागम स्थितिः;
    }

    यदि (update->components) {
        ध्रुव संरचना xkb_state_components_update * ध्रुव components =
            update->components;
        /* Update boolean controls first */
        यदि (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, events);
        }

        state_update_latched_locked(state, components, events);
    }

    xkb_state_update_derived(&state->आधारः);

    ध्रुव गणना xkb_state_component परिवर्तितम् = get_state_component_changes(
        &previous_components, &state->आधारः.components
    );
    यदि (परिवर्तितम्) {
        // TODO: latch controls
        यदि (परिवर्तितम् & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        /* Create event only if some component actually changed */
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .परिवर्तितम् = परिवर्तितम्,
                .components = state->आधारः.components
            }
        });

        /* Terminate the frame */
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    प्रत्यागम XKB_SUCCESS;
}

स्थिर ssize_t
do_remap_modifiers(ध्रुव संरचना machine_modifiers_config * सीमित mappings,
                   संरचना xkb_state * सीमित state,
                   संरचना xkb_events * सीमित events,
                   ध्रुव संरचना xkb_कुञ्चिका * सीमित कुञ्चिका)
{
    यदि (!(mappings->आवरणम् & state->components.mods))
        प्रत्यागम -1;

    ध्रुव xkb_layout_सूचकांक_t layout = state_कुञ्चिका_get_layout(state, कुञ्चिका);
    यदि (layout >= कुञ्चिका->अङ्कः_groups)
        प्रत्यागम -1;

    ध्रुव संरचना xkb_कुञ्चिका_type * ध्रुव type = कुञ्चिका->groups[layout].type;
    xkb_mod_आवरणम्_t affect_mods = 0;
    xkb_mod_आवरणम्_t mods = 0;
    प्रति (darray_आकार_t m = 0; m < mappings->mappings_अङ्कः; m++) {
        ध्रुव संरचना machine_mods_mapping * ध्रुव mapping =
            &mappings->mappings[m];
        यदि (/* Skip not matching active mods */
            (mapping->source & state->components.mods) == mapping->source &&
            /* Skip overlap with previous active mappings */
            !(mapping->source & affect_mods) &&
            /* Skip if the key type uses some of the source modifiers */
            !(mapping->source & type->mods.आवरणम्)) {
            affect_mods |= mappings->mappings[m].source;
            mods |= mappings->mappings[m].target;
        }
    }

    यदि (!affect_mods)
        प्रत्यागम -1;

    संरचना xkb_state new = *state;
    new.components.आधारः_mods = (new.components.आधारः_mods & ~affect_mods) | mods;
    new.components.latched_mods = (new.components.latched_mods & ~affect_mods);
    new.components.locked_mods = (new.components.locked_mods & ~affect_mods);
    xkb_state_update_derived(&new);

    ssize_t event_idx = -1;
    ध्रुव गणना xkb_state_component परिवर्तितम् =
        get_state_component_changes(&state->components, &new.components);
    यदि (परिवर्तितम्) {
        event_idx = (ssize_t) darray_आकार(events->queue);
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = new.components,
                .परिवर्तितम् = परिवर्तितम्
            }
        });
    }

    state->components.mods = new.components.mods;
    प्रत्यागम event_idx;
}

/**
 * Activate an alternative layout if some modifiers are active, typically
 * `Control`, `Alt` or `Super`.
 *
 * The tweak consists of computing a *base* layout such that the *effective*
 * layout is the corresponding target layout.
 */
स्थिर ssize_t
do_shortcuts_tweak(ध्रुव संरचना machine_shortcuts_config *config,
                   संरचना xkb_state *state,
                   ध्रुव संरचना state_components *previous_components,
                   संरचना xkb_events *events, ssize_t remap_event)
{
    यदि (!config->entries) {
        /* No shortcuts tweak */
        प्रत्यागम remap_event;
    }

    ध्रुव संरचना xkb_shortcuts_config_प्रविष्टिः * ध्रुव प्रविष्टिः =
        &config->entries[state->components.गणः];

    यदि (प्रविष्टिः->target != XKB_LAYOUT_INVALID &&
        (प्रविष्टिः->mods & state->components.mods)) {
        /*
         * Activate shortcuts tweak:
         * 1. The real base group is saved by the caller, to be restored by
         *    later by a call to undo_tweaks().
         * 2. Compute the new base group so that the resulting effective group
         *    is the target.
         * 3. Recompute derived state components.
         */
        संरचना xkb_state new = *state;
        यदि (remap_event < 0) {
            /* Create new event */
            remap_event = (ssize_t) darray_आकार(events->queue);
            darray_append(events->queue, (संरचना xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = {0},
                    .परिवर्तितम् = 0
                }
            });
        } अन्यथा {
            /* Merge with last remap event */
            new.components =
                darray_item(events->queue, remap_event).components.components;
        }
        new.components.आधारः_गणः
            = (int32_t) प्रविष्टिः->target
            - state->components.latched_गणः
            - state->components.locked_गणः;
        xkb_state_update_derived(&new);

        ध्रुव गणना xkb_state_component परिवर्तितम् =
            get_state_component_changes(previous_components, &new.components);
        darray_item(events->queue, remap_event).components.परिवर्तितम् = परिवर्तितम्;
        darray_item(events->queue, remap_event).components.components =
            new.components;

        state->components.गणः = new.components.गणः;
    } अन्यथा {
        /* Deactivated entry or modifiers do not match */
    }
    प्रत्यागम remap_event;
}

स्थिर शून्य
undo_tweaks(ध्रुव संरचना xkb_state *state,
            ध्रुव संरचना state_components *initial_components,
            संरचना xkb_events *events)
{
    /* Get last component event */
    ध्रुव संरचना xkb_event *event = रिक्त;
    darray_foreach_reverse(event, events->queue) {
        यदि (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS)
            भङ्ग;
    }
    यदि (!event)
        प्रत्यागम;

    /* Restore state */
    ध्रुव गणना xkb_state_component परिवर्तितम् =
        get_state_component_changes(initial_components,
                                    &event->components.components);
    यदि (परिवर्तितम्) {
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = *initial_components,
                .परिवर्तितम् = परिवर्तितम्
            }
        });
    }
}

स्थिर ध्रुव संरचना xkb_कुञ्चिका *
process_overlayable_कुञ्चिका(संरचना xkb_machine *sm,
                        ध्रुव संरचना xkb_कुञ्चिका * कुञ्चिका,
                        गणना xkb_कुञ्चिका_direction direction)
{
    /* Check if key is currently overlaid */
    संरचना xkb_overlaid_कुञ्चिका *प्रविष्टिः = रिक्त;
    संरचना xkb_overlaid_कुञ्चिका *उपलब्धम्_प्रविष्टिः = रिक्त;
    darray_foreach(प्रविष्टिः, sm->overlays.कुञ्चिकाः) {
        यदि (प्रविष्टिः->old == कुञ्चिका) {
            /* Maintain the key overlaid */
            विकल्प (direction) {
            पक्ष XKB_कुञ्चिका_अधस्तात्:
                प्रविष्टिः->refcnt++;
                भङ्ग;
            पक्ष XKB_कुञ्चिका_REPEATED:
                भङ्ग;
            पक्ष XKB_कुञ्चिका_ऊर्ध्वम्:
            स्वभाव:
                प्रविष्टिः->refcnt--;
                भङ्ग;
            }
            यदि (प्रविष्टिः->refcnt <= 0)
                प्रविष्टिः->old = रिक्त;
            प्रत्यागम प्रविष्टिः->new;
        } अन्यथा यदि (!प्रविष्टिः->old && !उपलब्धम्_प्रविष्टिः) {
            /* First free spot */
            उपलब्धम्_प्रविष्टिः = प्रविष्टिः;
        }
    }
    यदि (direction == XKB_कुञ्चिका_अधस्तात्) {
        /* Enable key overlay */

        /*
         * Get overlay index: select the latest activated overlay
         * containing the key.
         */
        ध्रुव संरचना xkb_कुञ्चिका *new = कुञ्चिका;

        यदि (कुञ्चिका->overlays & sm->overlays.enabled) {
            /* Some relevant overlay is active */
            प्रति (uint32_t राशिः = sm->overlays.क्रमः; राशिः; राशिः >>= 4) {
                static_assert(XKB_OVERLAY_गणनम् == 8, "");
                ध्रुव xkb_overlay_सूचकांक_t overlay = (राशिः & 0xf) - 1;
                ध्रुव xkb_overlay_आवरणम्_t आवरणम् =
                    (xkb_overlay_आवरणम्_t)(1u << overlay);
                यदि (कुञ्चिका->overlays & आवरणम्) {
                    /* Get new key */
                    यदि (कुञ्चिका->overlays_inline) {
                        /* Stored inline */
                        new = कुञ्चिका->overlay_कुञ्चिका;
                    } अन्यथा {
                        /* Stored in sparse array */
                        ध्रुव xkb_overlay_आवरणम्_t नीचः = (xkb_overlay_आवरणम्_t)(
                            कुञ्चिका->overlays & (xkb_overlay_आवरणम्_t)(आवरणम् - 1u)
                        );
                        /* index = number of set bits strictly below bit */
                        ध्रुव xkb_overlay_सूचकांक_t सूचकांक =
                            (xkb_overlay_सूचकांक_t)popcount32(नीचः);
                        new = कुञ्चिका->overlays_कुञ्चिकाः[सूचकांक];
                    }
                    भङ्ग;
                }
            }
        } अन्यथा {
            /*
             * No relevant overlay is active: block future overlays while
             * the key is pressed.
             */
        }

        /* Get new entry */
        यदि (उपलब्धम्_प्रविष्टिः) {
            /* Re-use a free entry */
            प्रविष्टिः = उपलब्धम्_प्रविष्टिः;
        } अन्यथा {
            /* Append a new entry */
            ध्रुव darray_आकार_t idx = darray_आकार(sm->overlays.कुञ्चिकाः);
            darray_resize(sm->overlays.कुञ्चिकाः, idx + 1);
            प्रविष्टिः = &darray_item(sm->overlays.कुञ्चिकाः, idx);
        }

        /* Overlay the key */
        प्रविष्टिः->old = कुञ्चिका;
        प्रविष्टिः->new = new;
        प्रविष्टिः->refcnt = 1;
        प्रत्यागम new;
    }

    /* No overlay */
    प्रत्यागम कुञ्चिका;
}

गणना xkb_स्थितिः
xkb_machine_process_कुञ्चिका(संरचना xkb_machine *sm,
                        xkb_keycode_t kc, गणना xkb_कुञ्चिका_direction direction,
                        संरचना xkb_events *events)
{
    darray_आकार(events->queue) = 0;
    events->अग्र = 0;

    संरचना xkb_server_state * ध्रुव state = &sm->आधारः;
    ध्रुव संरचना xkb_कुञ्चिका * कुञ्चिका = Xkb_कुञ्चिका(state->आधारः.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    यदि (!कुञ्चिका || (direction == XKB_कुञ्चिका_REPEATED && !कुञ्चिका->repeats))
        प्रत्यागम XKB_SUCCESS;

    ध्रुव संरचना state_components initial_components = state->आधारः.components;

    यदि (direction == XKB_कुञ्चिका_अधस्तात् &&
        (sm->आधारः.आधारः.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_कुञ्चिकाः) &&
        (sm->आधारः.a11y_flags & XKB_A11_Y_STICKY_कुञ्चिकाः_NO_युगपत्_कुञ्चिकाः) &&
        state->आधारः.components.आधारः_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_कुञ्चिकाः,
                                      0, events);
    }

    यदि (कुञ्चिका->overlays)
        कुञ्चिका = process_overlayable_कुञ्चिका(sm, कुञ्चिका, direction);

    ssize_t remap_event = do_remap_modifiers(&sm->config.modifiers,
                                             &state->आधारः, events, कुञ्चिका);

    remap_event = do_shortcuts_tweak(&sm->config.shortcuts, &state->आधारः,
                                     &initial_components, events, remap_event);

    यदि (remap_event >= 0) {
        // TODO: what about merging state event for RedirectKey?
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    state->flags |= STATE_REQUIRE_कुञ्चिका_EVENT;
    state->समुच्चयः_mods = 0;
    state->clear_mods = 0;

    /* Handle key actions */
    xkb_filter_apply_all(state, events, कुञ्चिका, direction);

    xkb_mod_सूचकांक_t i;
    xkb_mod_आवरणम्_t bit;
    प्रति (i = 0, bit = 1; state->समुच्चयः_mods; i++, bit <<= 1) {
        यदि (state->समुच्चयः_mods & bit) {
            state->mod_कुञ्चिका_गणनम्[i]++;
            state->आधारः.components.आधारः_mods |= bit;
            state->समुच्चयः_mods &= ~bit;
        }
    }

    प्रति (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        यदि (state->clear_mods & bit) {
            state->mod_कुञ्चिका_गणनम्[i]--;
            यदि (state->mod_कुञ्चिका_गणनम्[i] <= 0) {
                state->आधारः.components.आधारः_mods &= ~bit;
                state->mod_कुञ्चिका_गणनम्[i] = 0;
            }
            state->clear_mods &= ~bit;
        }
    }

    xkb_state_update_derived(&state->आधारः);

    /*
     * Insert key event only if actions did not override it, e.g.:
     * - RedirectKey() append its own key event;
     * - MovePointer() and PointerButton() generate pointer events instead
     *   of key events;
     * - TerminateServer() generate a Server event instead of key events.
     */
    यदि (state->flags & STATE_REQUIRE_कुञ्चिका_EVENT) {
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_कुञ्चिका,
            .कुञ्चिका = {
                .keycode = कुञ्चिका->keycode,
                .direction = direction
            }
        });
    }

    यदि (remap_event >= 0) {
        // FIXME: fragile if last state change does not restore the state to the remap event
        undo_tweaks(&state->आधारः, &initial_components, events);
    }

    ध्रुव गणना xkb_state_component परिवर्तितम् = get_state_component_changes(
        &initial_components, &state->आधारः.components
    );
    यदि (परिवर्तितम्) {
        यदि (परिवर्तितम् & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        // TODO: merge with previous state event, if any

        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = state->आधारः.components,
                .परिवर्तितम् = परिवर्तितम्
            }
        });
    }

    यदि (!darray_रिक्तानि(events->queue) &&
        darray_item(events->queue, darray_आकार(events->queue) - 1).type !=
        XKB_EVENT_TYPE_FRAME)
    {
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    प्रत्यागम XKB_SUCCESS;
}

संरचना xkb_events *
xkb_events_new(संरचना xkb_context * सीमित context,
               ध्रुव संरचना xkb_events_config * सीमित config,
               गणना xkb_स्थितिः * सीमित स्थितिः)
{
    /* Handle default configuration */
    स्थिर ध्रुव संरचना xkb_events_config default_config = {
        .आकार = परिमाण(default_config)
    };
    यदि (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ध्रुव गणना xkb_स्थितिः abi_error = xkb_check_state_abi(config);
    यदि (abi_error) {
        xkb_log_abi_error(context, __func__, abi_error);
        यदि (स्थितिः)
            *स्थितिः = abi_error;
        प्रत्यागम रिक्त;
    }

    /* Sanitize input */
    ध्रुव uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_EVENTS_FLAGS_मानानि);
    यदि (invalid_flags) {
        log_err_func(context, XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS_,
                     "unrecognized events collection flags: 0x%"PRIx32"\n",
                     invalid_flags);
        यदि (स्थितिः)
            *स्थितिः = XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS;
        प्रत्यागम रिक्त;
    }

    संरचना xkb_events *events = शून्यस्मृतिं_देहि(1, परिमाण(*events));
    यदि (!events) {
        log_err_func1(context, XKB_ERROR_आवण्टनम्_FAILURE_2,
                      "cannot allocate state events collection\n");
        यदि (स्थितिः)
            *स्थितिः = XKB_ERROR_आवण्टनम्_FAILURE;
        प्रत्यागम रिक्त;
    }

    /* Pre-allocate event queue */
    darray_init(events->queue);
    स्थिर ध्रुव darray_आकार_t queue_initial_आकार = 16;
    darray_resize(events->queue, queue_initial_आकार);
    // TODO: proper safe darray_resize() with return value
    यदि (!darray_वस्तूनि(events->queue)) {
        log_err_func1(context, XKB_ERROR_आवण्टनम्_FAILURE_2,
                      "cannot allocate state events queue\n");
        xkb_events_destroy(events);
        यदि (स्थितिः)
            *स्थितिः = XKB_ERROR_आवण्टनम्_FAILURE;
        प्रत्यागम रिक्त;
    }
    darray_आकार(events->queue) = 0;

    events->ctx = xkb_context_ref(context);
    events->अग्र = 0;

    यदि (स्थितिः)
        *स्थितिः = XKB_SUCCESS;
    प्रत्यागम events;
}

शून्य
xkb_events_destroy(संरचना xkb_events *events)
{
    यदि (!events)
        प्रत्यागम;
    darray_free(events->queue);
    xkb_context_unref(events->ctx);
    मुक्त(events);
}

ध्रुव संरचना xkb_event *
xkb_events_अग्र(संरचना xkb_events *events)
{
    यदि (events->अग्र < darray_आकार(events->queue)) {
        ध्रुव darray_आकार_t सूचकांक = events->अग्र++;
        प्रत्यागम &darray_item(events->queue, सूचकांक);
    } अन्यथा {
        प्रत्यागम रिक्त;
    }
}

गणना xkb_event_type
xkb_event_get_type(ध्रुव संरचना xkb_event *event)
{
    प्रत्यागम event->type;
}

गणना xkb_स्थितिः
xkb_event_get_keycode(ध्रुव संरचना xkb_event *event,
                      xkb_keycode_t *keycode,
                      गणना xkb_कुञ्चिका_direction *direction)
{
    यदि (event->type == XKB_EVENT_TYPE_कुञ्चिका) {
        *keycode = event->कुञ्चिका.keycode;
        *direction = event->कुञ्चिका.direction;
        प्रत्यागम XKB_SUCCESS;
    } अन्यथा {
        प्रत्यागम XKB_ERROR_EVENT_TYPE_MISMATCH;
    }
}

गणना xkb_स्थितिः
xkb_event_get_components(ध्रुव संरचना xkb_event * सीमित event,
                         संरचना xkb_event_components * सीमित components)
{
    यदि (event->type != XKB_EVENT_TYPE_STATE_COMPONENTS)
        प्रत्यागम XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ध्रुव गणना xkb_स्थितिः स्थितिः = xkb_check_state_abi(components);
    यदि (स्थितिः != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, स्थितिः);
        प्रत्यागम स्थितिः;
    }

    components->परिवर्तितम् = (uint32_t)event->components.परिवर्तितम्;
    components->depressed_mods = event->components.components.आधारः_mods;
    components->latched_mods = event->components.components.latched_mods;
    components->locked_mods = event->components.components.locked_mods;
    components->mods = event->components.components.mods;
    components->depressed_layout = event->components.components.आधारः_गणः;
    components->latched_layout = event->components.components.latched_गणः;
    components->locked_layout = event->components.components.locked_गणः;
    components->layout = event->components.components.गणः;
    components->leds = event->components.components.leds;
    components->controls = (uint32_t)event->components.components.controls;

    प्रत्यागम XKB_SUCCESS;
}

गणना xkb_स्थितिः
xkb_event_get_pointer_motion(ध्रुव संरचना xkb_event * सीमित event,
                             संरचना xkb_event_pointer_motion * सीमित motion)
{
    यदि (event->type != XKB_EVENT_TYPE_POINTER_MOTION)
        प्रत्यागम XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ध्रुव गणना xkb_स्थितिः स्थितिः = xkb_check_state_abi(motion);
    यदि (स्थितिः != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, स्थितिः);
        प्रत्यागम स्थितिः;
    }

    motion->flags = event->pointer_motion.flags;
    motion->x = event->pointer_motion.x;
    motion->y = event->pointer_motion.y;

    प्रत्यागम XKB_SUCCESS;
}

गणना xkb_स्थितिः
xkb_event_get_pointer_button(ध्रुव संरचना xkb_event * सीमित event,
                             संरचना xkb_event_pointer_button * सीमित button)
{
    यदि (event->type != XKB_EVENT_TYPE_POINTER_BUTTON)
        प्रत्यागम XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ध्रुव गणना xkb_स्थितिः स्थितिः = xkb_check_state_abi(button);
    यदि (स्थितिः != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, स्थितिः);
        प्रत्यागम स्थितिः;
    }

    button->button = event->pointer_button.button;
    button->state = event->pointer_button.state;
    button->गणनम् = event->pointer_button.गणनम्;

    प्रत्यागम XKB_SUCCESS;
}

गणना xkb_स्थितिः
xkb_event_get_virtual_console(ध्रुव संरचना xkb_event * सीमित event,
                              int8_t * सीमित सूचकांक_or_विस्थापनम्,
                              bool * सीमित is_विस्थापनम्)
{
    यदि (event->type != XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
        प्रत्यागम XKB_ERROR_EVENT_TYPE_MISMATCH;

    *सूचकांक_or_विस्थापनम् = event->virtual_console.सूचकांक_or_विस्थापनम्;
    *is_विस्थापनम् = event->virtual_console.is_विस्थापनम्;

    प्रत्यागम XKB_SUCCESS;
}

गणना xkb_स्थितिः
xkb_state_update_event(संरचना xkb_state * सीमित आधारः_state,
                       ध्रुव संरचना xkb_event * सीमित event,
                       गणना xkb_state_component * सीमित परिवर्तितम्)
{
    /* Guard against server-only state */
    प्रतिज्ञा(आधारः_state->बहुलकम् == SERVER_COMPANION ||
           आधारः_state->बहुलकम् == LEGACY_MIXED_STATE);
    यदि (आधारः_state->बहुलकम् != SERVER_COMPANION &&
        आधारः_state->बहुलकम् != LEGACY_MIXED_STATE) {
        log_err(आधारः_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_बहुलकम्_2,
                "%s: Unexpected state type %d\n", __func__, आधारः_state->बहुलकम्);
        प्रत्यागम XKB_ERROR_UNEXPECTED_STATE_बहुलकम्;
    }

    संरचना xkb_client_state * ध्रुव state =
        (संरचना xkb_client_state *)आधारः_state;

    यदि (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
        ध्रुव संरचना state_components पूर्व_components = state->आधारः.components;
        state->आधारः.components = event->components.components;
        /*
         * Recompute the changes instead of using the event value, because we do
         * not know if the event’s queue and the state are synced.
         */
        यदि (परिवर्तितम्)
            *परिवर्तितम् = get_state_component_changes(&पूर्व_components,
                                                   &state->आधारः.components);
    } अन्यथा {
        यदि (परिवर्तितम्)
            *परिवर्तितम् = 0;
    }
    प्रत्यागम XKB_SUCCESS;
}

स्थिर शून्य
append_pointer_button(संरचना xkb_events * सीमित events,
                      संरचना xkb_filter * सीमित filter,
                      गणना xkb_pointer_button_state button_state,
                      uint8_t गणनम्)
{
    darray_append(events->queue, (संरचना xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_BUTTON,
        .pointer_button = {
            .आकार = परिमाण(((संरचना xkb_event*)0)->pointer_button),
            .button = filter->priv,
            .state = button_state,
            .गणनम् = गणनम्,
        }
    });
}

स्थिर शून्य
xkb_filter_pointer_button_new(संरचना xkb_server_state *state,
                              संरचना xkb_events *events,
                              संरचना xkb_filter *filter)
{
    यदि (!events || state->आधारः.बहुलकम् != SERVER_STATE ||
        !(state->आधारः.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_कुञ्चिकाः)) {
        filter->func = रिक्त;
        प्रत्यागम;
    }

    संरचना xkb_machine * ध्रुव sm = (संरचना xkb_machine *)state;

    /* Resolve button */
    ध्रुव xkb_pointer_button_सूचकांक_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;
    filter->priv = button;

    ध्रुव गणना xkb_pointer_button_state button_state =
        XKB_POINTER_BUTTON_PRESSED;
    append_pointer_button(events, filter, button_state,
                          filter->action.btn.गणनम्);
    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;
}

स्थिर bool
xkb_filter_pointer_button_func(संरचना xkb_server_state *state,
                               संरचना xkb_events *events,
                               संरचना xkb_filter *filter,
                               ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                               गणना xkb_कुञ्चिका_direction direction)
{
    यदि (कुञ्चिका != filter->कुञ्चिका || !events)
        प्रत्यागम XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;

    विकल्प (direction) {
    पक्ष XKB_कुञ्चिका_अधस्तात्:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_कुञ्चिका_REPEATED:
        प्रत्यागम XKB_FILTER_CONSUME;
    स्वभाव:
        यदि (--filter->refcnt > 0)
            प्रत्यागम XKB_FILTER_CONSUME;
    }

    यदि (!filter->action.btn.गणनम्)
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);

    filter->func = रिक्त;
    प्रत्यागम XKB_FILTER_CONSUME;
}

संरचना xkb_filter_pointer_तालकम्_button_priv {
    uint32_t button:31;
#यदि_परिभाषितम् _WIN32
    uint32_t locked:1; // MSVC requires matching types to pack bitfields
#अन्यथा
    bool locked:1;
#यदि_अन्तः
};

गणना {
    XKB_FILTER_POINTER_तालकम्_BUTTON_PRIV_विस्तारः =
        परिमाण(संरचना xkb_filter_pointer_तालकम्_button_priv) * CHAR_BIT,
    XKB_FILTER_POINTER_तालकम्_BUTTON_PRIV_MAX =
        (UINT64_C(1) << XKB_FILTER_POINTER_तालकम्_BUTTON_PRIV_विस्तारः) - 1
};

static_assert(परिमाण(संरचना xkb_filter_pointer_तालकम्_button_priv) ==
              परिमाण(((संरचना xkb_filter*)0)->priv),
              "");
static_assert((uint32_t)XKB_POINTER_BUTTON_MAX < (uint32_t)XKB_FILTER_POINTER_तालकम्_BUTTON_PRIV_MAX,
              "xkb_filter_pointer_lock_button_priv cannot store button");

स्थिर शून्य
xkb_filter_pointer_तालकम्_button_new(संरचना xkb_server_state *state,
                                   संरचना xkb_events *events,
                                   संरचना xkb_filter *filter)
{
    यदि (!events || state->आधारः.बहुलकम् != SERVER_STATE ||
        !(state->आधारः.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_कुञ्चिकाः)) {
        filter->func = रिक्त;
        प्रत्यागम;
    }

    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;

    संरचना xkb_machine * ध्रुव sm = (संरचना xkb_machine *)state;

    /* Resolve button */
    ध्रुव xkb_pointer_button_सूचकांक_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;

    static_assert(XKB_POINTER_BUTTON_आवरणम्_विस्तारः <= 8,
                  "invalid UINTx_C() macro");
    static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_आवरणम्_विस्तारः,
                  "invalid left shift");
    ध्रुव xkb_pointer_button_आवरणम्_t आवरणम् = (UINT8_C(1) << button);

    ध्रुव bool locked = (sm->mouse.locked_buttons & आवरणम्);

    यदि (!locked && !(filter->action.btn.flags & ACTION_तालकम्_NO_तालकम्)) {
        sm->mouse.locked_buttons |= आवरणम्;
        filter->priv = button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_PRESSED, 0);
    }

    संरचना xkb_filter_pointer_तालकम्_button_priv *priv =
        (संरचना xkb_filter_pointer_तालकम्_button_priv*)&filter->priv;
    priv->button = button,
    priv->locked = locked;
}

स्थिर bool
xkb_filter_pointer_तालकम्_button_func(संरचना xkb_server_state *state,
                                    संरचना xkb_events *events,
                                    संरचना xkb_filter *filter,
                                    ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                                    गणना xkb_कुञ्चिका_direction direction)
{
    यदि (कुञ्चिका != filter->कुञ्चिका || !events || state->आधारः.बहुलकम् != SERVER_STATE)
        प्रत्यागम XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;

    विकल्प (direction) {
    पक्ष XKB_कुञ्चिका_अधस्तात्:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_कुञ्चिका_REPEATED:
        प्रत्यागम XKB_FILTER_CONSUME;
    स्वभाव:
        यदि (--filter->refcnt > 0)
            प्रत्यागम XKB_FILTER_CONSUME;
    }

    ध्रुव संरचना xkb_filter_pointer_तालकम्_button_priv *priv =
        (संरचना xkb_filter_pointer_तालकम्_button_priv*)&filter->priv;

    यदि (priv->locked && !(filter->action.btn.flags & ACTION_तालकम्_NO_UNLOCK)) {
        संरचना xkb_machine * ध्रुव sm = (संरचना xkb_machine *)state;

        static_assert(XKB_POINTER_BUTTON_आवरणम्_विस्तारः <= 8,
                    "invalid UINTx_C() macro");
        static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_आवरणम्_विस्तारः,
                    "invalid left shift");
        ध्रुव xkb_pointer_button_आवरणम्_t आवरणम् = (UINT8_C(1) << priv->button);

        sm->mouse.locked_buttons &= ~आवरणम्;
        filter->priv = priv->button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);
    }

    filter->func = रिक्त;
    प्रत्यागम XKB_FILTER_CONSUME;
}

स्थिर शून्य
xkb_filter_pointer_समुच्चयः_default_button_new(संरचना xkb_server_state *state,
                                          संरचना xkb_events *events,
                                          संरचना xkb_filter *filter)
{
    यदि (!events || state->आधारः.बहुलकम् != SERVER_STATE ||
        !(state->आधारः.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_कुञ्चिकाः)) {
        filter->func = रिक्त;
        प्रत्यागम;
    }

    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;

    संरचना xkb_machine * ध्रुव sm = (संरचना xkb_machine *)state;

    static_assert(2 * XKB_POINTER_BUTTON_MAX <= अधिकतम_पूर्णाङ्कः, "lossy conversion");
    पूर्णाङ्क button = (filter->action.dflt.flags & ACTION_ABSOLUTE_SWITCH)
        ? filter->action.dflt.मानम्
        : sm->mouse.default_button + filter->action.dflt.मानम्;

    /* Wrap into range */
    ध्रुव पूर्णाङ्क rem = button % XKB_POINTER_BUTTON_MAX;
    sm->mouse.default_button = (rem >= 0)
        ? (xkb_pointer_button_सूचकांक_t)rem
        : (xkb_pointer_button_सूचकांक_t)(rem + XKB_POINTER_BUTTON_MAX);
}

स्थिर bool
xkb_filter_pointer_समुच्चयः_default_button_func(संरचना xkb_server_state *state,
                                           संरचना xkb_events *events,
                                           संरचना xkb_filter *filter,
                                           ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                                           गणना xkb_कुञ्चिका_direction direction)
{
    यदि (कुञ्चिका != filter->कुञ्चिका || !events)
        प्रत्यागम XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;

    विकल्प (direction) {
    पक्ष XKB_कुञ्चिका_अधस्तात्:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_कुञ्चिका_REPEATED:
        प्रत्यागम XKB_FILTER_CONSUME;
    स्वभाव:
        यदि (--filter->refcnt > 0)
            प्रत्यागम XKB_FILTER_CONSUME;
    }

    filter->func = रिक्त;
    प्रत्यागम XKB_FILTER_CONTINUE;
}

स्थिर शून्य
xkb_filter_terminate_server_new(संरचना xkb_server_state *state,
                                 संरचना xkb_events *events,
                                 संरचना xkb_filter *filter)
{
    यदि (!events || state->आधारः.बहुलकम् != SERVER_STATE ||
        !(((संरचना xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = रिक्त;
        प्रत्यागम;
    }

    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;

    darray_append(events->queue, (संरचना xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    });
}

स्थिर bool
xkb_filter_terminate_server_func(संरचना xkb_server_state *state,
                                 संरचना xkb_events *events,
                                 संरचना xkb_filter *filter,
                                 ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                                 गणना xkb_कुञ्चिका_direction direction)
{
    यदि (कुञ्चिका != filter->कुञ्चिका || !events)
        प्रत्यागम XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;

    विकल्प (direction) {
    पक्ष XKB_कुञ्चिका_अधस्तात्:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_कुञ्चिका_REPEATED:
        प्रत्यागम XKB_FILTER_CONSUME;
    स्वभाव:
        यदि (--filter->refcnt > 0)
            प्रत्यागम XKB_FILTER_CONSUME;
    }

    filter->func = रिक्त;
    प्रत्यागम XKB_FILTER_CONSUME;
}

स्थिर शून्य
xkb_filter_switch_virtual_console_new(संरचना xkb_server_state *state,
                                      संरचना xkb_events *events,
                                      संरचना xkb_filter *filter)
{
    यदि (!events || state->आधारः.बहुलकम् != SERVER_STATE ||
        !(((संरचना xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = रिक्त;
        प्रत्यागम;
    }

    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;

    darray_append(events->queue, (संरचना xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
        .virtual_console = {
            .सूचकांक_or_विस्थापनम् = filter->action.screen.screen,
            .is_विस्थापनम् = !(filter->action.screen.flags & ACTION_ABSOLUTE_SWITCH)
        }
    });
}

स्थिर bool
xkb_filter_switch_virtual_console_func(संरचना xkb_server_state *state,
                                       संरचना xkb_events *events,
                                       संरचना xkb_filter *filter,
                                       ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                                       गणना xkb_कुञ्चिका_direction direction)
{
    यदि (कुञ्चिका != filter->कुञ्चिका || !events)
        प्रत्यागम XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;

    विकल्प (direction) {
    पक्ष XKB_कुञ्चिका_अधस्तात्:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_कुञ्चिका_REPEATED:
        प्रत्यागम XKB_FILTER_CONSUME;
    स्वभाव:
        यदि (--filter->refcnt > 0)
            प्रत्यागम XKB_FILTER_CONSUME;
    }

    filter->func = रिक्त;
    प्रत्यागम XKB_FILTER_CONSUME;
}
