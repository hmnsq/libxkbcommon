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
    darray_size_t next;
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

गणना xkb_state_mode_internal {
    /** `xkb_state`: Client-only state (server API raises errors) */
    CLIENT_STATE = XKB_STATE_MODE_CLIENT,
    /** `xkb_state`: Server query companion */
    SERVER_COMPANION = XKB_STATE_MODE_SERVER_QUERY,
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

गणना { XKB_LAYOUT_INDEX_T_MIN_WIDTH = 6 };
static_assert(XKB_MAX_GROUPS < (1 << XKB_LAYOUT_INDEX_T_MIN_WIDTH),
              "Cannot encode out_of_range_group::redirect_group");

संरचना out_of_range_group {
    गणना xkb_layout_out_of_range_policy policy :
        (परिमाण(गणना xkb_layout_out_of_range_policy) * CHAR_BIT -
         XKB_LAYOUT_INDEX_T_MIN_WIDTH);
    xkb_layout_index_t redirect_group : XKB_LAYOUT_INDEX_T_MIN_WIDTH;
};

गणना { XKB_STATE_MODE_INTERNAL_MIN_WIDTH = 4 /* 3 bits + sign */ };
static_assert(XKB_STATE_TYPE_अङ्कः_ENTRIES <
              (1 << XKB_STATE_MODE_INTERNAL_MIN_WIDTH),
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
    संरचना out_of_range_group out_of_range_group;

    गणना xkb_state_mode_internal mode : XKB_STATE_MODE_INTERNAL_MIN_WIDTH;
    पूर्णाङ्क refcnt : (परिमाण(पूर्णाङ्क) * CHAR_BIT - XKB_STATE_MODE_INTERNAL_MIN_WIDTH);
};

/**
 * Minimal client state
 *
 * Wrapper for strong typing
 */
संरचना xkb_client_state {
    /** Inherits from `xkb_state` */
    संरचना xkb_state base;
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
    संरचना xkb_state base;

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
    xkb_mod_mask_t set_mods;
    xkb_mod_mask_t clear_mods;

    /*
     * We mustn't clear a base modifier if there's another depressed key
     * which affects it, e.g. given this sequence
     * < Left Shift down, Right Shift down, Left Shift Up >
     * the modifier should still be set. This keeps the count.
     */
    int16_t mod_कुञ्चिका_गणनम्[XKB_MAX_MODS];
};

स्थिर ध्रुव संरचना xkb_कुञ्चिका_type_entry *
get_entry_for_mods(ध्रुव संरचना xkb_कुञ्चिका_type *type, xkb_mod_mask_t mods)
{
    अत्थाय (darray_size_t i = 0; i < type->अङ्कः_entries; i++)
        सचे (entry_is_active(&type->entries[i]) &&
            type->entries[i].mods.mask == mods)
            पटिनिवत्त &type->entries[i];
    पटिनिवत्त रिक्त;
}

स्थिर ध्रुव संरचना xkb_कुञ्चिका_type_entry *
get_entry_for_कुञ्चिका_state(संरचना xkb_state *state, ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                        xkb_layout_index_t group)
{
    ध्रुव संरचना xkb_कुञ्चिका_type* ध्रुव type = कुञ्चिका->groups[group].type;
    xkb_mod_mask_t active_mods = state->components.mods & type->mods.mask;
    पटिनिवत्त get_entry_for_mods(type, active_mods);
}

स्थिर अन्तःस्थ xkb_level_index_t
state_कुञ्चिका_get_level(संरचना xkb_state *state, ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                         xkb_layout_index_t layout)
{
    सचे (layout >= कुञ्चिका->अङ्कः_groups)
        पटिनिवत्त XKB_LEVEL_INVALID;

    /* If we don't find an explicit match the default is 0. */
    ध्रुव संरचना xkb_कुञ्चिका_type_entry* ध्रुव entry =
        get_entry_for_कुञ्चिका_state(state, कुञ्चिका, layout);

    पटिनिवत्त (entry) ? entry->level : 0;
}

/**
 * Returns the level to use for the given key and state, or
 * XKB_LEVEL_INVALID.
 */
xkb_level_index_t
xkb_state_कुञ्चिका_get_level(संरचना xkb_state *state, xkb_keycode_t kc,
                        xkb_layout_index_t layout)
{
    ध्रुव संरचना xkb_कुञ्चिका* ध्रुव कुञ्चिका = Xkb_कुञ्चिका(state->keymap, kc);

    पटिनिवत्त (कुञ्चिका)
        ? state_कुञ्चिका_get_level(state, कुञ्चिका, layout)
        : XKB_LEVEL_INVALID;
}

स्थिर अन्तःस्थ xkb_layout_index_t
state_कुञ्चिका_get_layout(संरचना xkb_state *state, ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका)
{
    पटिनिवत्त xkb_keymap_कुञ्चिका_effective_layout(कुञ्चिका, state->components.group);
}

/**
 * Returns the layout to use for the given key and state, taking
 * wrapping/clamping/etc into account, or XKB_LAYOUT_INVALID.
 */
xkb_layout_index_t
xkb_state_कुञ्चिका_get_layout(संरचना xkb_state *state, xkb_keycode_t kc)
{
    ध्रुव संरचना xkb_कुञ्चिका* ध्रुव कुञ्चिका = Xkb_कुञ्चिका(state->keymap, kc);

    सचे (!कुञ्चिका)
        पटिनिवत्त XKB_LAYOUT_INVALID;

    पटिनिवत्त state_कुञ्चिका_get_layout(state, कुञ्चिका);
}

/* Empty action used for empty levels */
स्थिर ध्रुव सङ्घ xkb_action dummy_action = { .type = ACTION_TYPE_NONE };

स्थिर xkb_action_गणनम्_t
xkb_कुञ्चिका_get_actions(संरचना xkb_state *state, ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                    ध्रुव सङ्घ xkb_action **actions)
{
    ध्रुव xkb_layout_index_t layout = state_कुञ्चिका_get_layout(state, कुञ्चिका);
    ध्रुव xkb_level_index_t level = state_कुञ्चिका_get_level(state, कुञ्चिका, layout);
    सचे (level == XKB_LEVEL_INVALID)
        गच्छ err;

    ध्रुव xkb_action_गणनम्_t गणनम् =
        xkb_keymap_कुञ्चिका_get_actions_by_level(state->keymap, कुञ्चिका,
                                            layout, level, actions);
    सचे (!गणनम्)
        गच्छ err;

    पटिनिवत्त गणनम्;

err:
    /* Use a dummy action if no corresponding level was found or if it is empty.
     * This is required e.g. to handle latches properly. */
    *actions = &dummy_action;
    पटिनिवत्त 1;
}

स्थिर संरचना xkb_filter *
xkb_filter_new(संरचना xkb_server_state *state)
{
    संरचना xkb_filter *filter = रिक्त, *iter;

    darray_foreach(iter, state->filters) {
        सचे (iter->func)
            अनुवर्त;
        /* Use available slot */
        filter = iter;
        भङ्ग;
    }

    सचे (!filter) {
        /* No available slot: resize the filters array */
        darray_resize0(state->filters, darray_size(state->filters) + 1);
        filter = &darray_item(state->filters, darray_size(state->filters) -1);
    }

    filter->refcnt = 1;
    पटिनिवत्त filter;
}

/***====================================================================***/

गणना xkb_filter_result {
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
#define apply_group_delta(filter_, state_, component_)                   \
    सचे ((filter_)->action.group.flags & ACTION_ABSOLUTE_SWITCH)          \
        (state_)->components.component_ = (filter_)->action.group.group; \
    अञ्ञथा                                                                 \
        (state_)->components.component_ += (filter_)->action.group.group

स्थिर शून्य
xkb_filter_group_set_new(संरचना xkb_server_state *state,
                         संरचना xkb_events *events,
                         संरचना xkb_filter *filter)
{
    static_assert(परिमाण(state->base.components.base_group) ==
                  परिमाण(filter->priv),
                  "Max groups don't fit");
    filter->priv = (uint32_t)state->base.components.base_group;
    apply_group_delta(filter, &state->base, base_group);
}

स्थिर bool
xkb_filter_group_set_func(संरचना xkb_server_state *state,
                          संरचना xkb_events *events,
                          संरचना xkb_filter *filter,
                          ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                          गणना xkb_कुञ्चिका_direction direction)
{
    सचे (कुञ्चिका != filter->कुञ्चिका) {
        filter->action.group.flags &= ~ACTION_LOCK_CLEAR;
        पटिनिवत्त XKB_FILTER_CONTINUE;
    }

    विकल्प (direction) {
    पक्ष XKB_कुञ्चिका_DOWN:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_कुञ्चिका_REPEATED:
        पटिनिवत्त XKB_FILTER_CONSUME;
    स्वभाव:
        सचे (--filter->refcnt > 0)
            पटिनिवत्त XKB_FILTER_CONSUME;
    }

    static_assert(परिमाण(state->base.components.base_group) == परिमाण(filter->priv),
                  "Max groups don't fit");
    state->base.components.base_group = (int32_t) filter->priv;

    सचे (filter->action.group.flags & ACTION_LOCK_CLEAR)
        state->base.components.locked_group = 0;

    filter->func = रिक्त;
    पटिनिवत्त XKB_FILTER_CONTINUE;
}

स्थिर गणना xkb_state_component
get_state_component_changes(ध्रुव संरचना state_components *a,
                            ध्रुव संरचना state_components *b)
{
    गणना xkb_state_component mask = 0;

    सचे (a->group != b->group)
        mask |= XKB_STATE_LAYOUT_EFFECTIVE;
    सचे (a->base_group != b->base_group)
        mask |= XKB_STATE_LAYOUT_DEPRESSED;
    सचे (a->latched_group != b->latched_group)
        mask |= XKB_STATE_LAYOUT_LATCHED;
    सचे (a->locked_group != b->locked_group)
        mask |= XKB_STATE_LAYOUT_LOCKED;
    सचे (a->mods != b->mods)
        mask |= XKB_STATE_MODS_EFFECTIVE;
    सचे (a->base_mods != b->base_mods)
        mask |= XKB_STATE_MODS_DEPRESSED;
    सचे (a->latched_mods != b->latched_mods)
        mask |= XKB_STATE_MODS_LATCHED;
    सचे (a->locked_mods != b->locked_mods)
        mask |= XKB_STATE_MODS_LOCKED;
    सचे (a->leds != b->leds)
        mask |= XKB_STATE_LEDS;
    सचे (a->controls != b->controls)
        mask |= XKB_STATE_CONTROLS_EFFECTIVE;

    पटिनिवत्त mask;
}

स्थिर शून्य
xkb_filter_group_lock_new(संरचना xkb_server_state *state,
                          संरचना xkb_events *events,
                          संरचना xkb_filter *filter)
{
    सचे (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release: do nothing on key press.
         *
         * This is a keymap format v2 extension.
         */
        पटिनिवत्त;
    } अञ्ञथा {
        /* Lock on key press */
        apply_group_delta(filter, &state->base, locked_group);
    }
}

स्थिर bool
xkb_filter_group_lock_func(संरचना xkb_server_state *state,
                           संरचना xkb_events *events,
                           संरचना xkb_filter *filter,
                           ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                           गणना xkb_कुञ्चिका_direction direction)
{
    सचे (कुञ्चिका != filter->कुञ्चिका) {
        सचे ((filter->action.group.flags & ACTION_LOCK_ON_RELEASE) &&
            direction == XKB_कुञ्चिका_DOWN) {
            /*
             * Another key has been pressed after the locking key:
             * cancel group lock on release.
             *
             * This is a keymap v2 extension.
             */
            filter->action.group.flags &= ~ACTION_LOCK_ON_RELEASE;
        }
        पटिनिवत्त XKB_FILTER_CONTINUE;
    }

    विकल्प (direction) {
    पक्ष XKB_कुञ्चिका_DOWN:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_कुञ्चिका_REPEATED:
        पटिनिवत्त XKB_FILTER_CONSUME;
    स्वभाव:
        सचे (--filter->refcnt > 0)
            पटिनिवत्त XKB_FILTER_CONSUME;
    }

    सचे (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release
         *
         * This is a keymap v2 extension.
         */
        apply_group_delta(filter, &state->base, locked_group);
    } अञ्ञथा {
        /* Lock on key press: do nothing on key release. */
    }
    filter->func = रिक्त;
    पटिनिवत्त XKB_FILTER_CONTINUE;
}

स्थिर bool
xkb_action_breaks_latch(ध्रुव सङ्घ xkb_action *action,
                        गणना xkb_action_controls controls,
                        गणना xkb_internal_action_flags flag,
                        xkb_mod_mask_t mask)
{
    विकल्प (action->type) {
    पक्ष ACTION_TYPE_NONE:
    पक्ष ACTION_TYPE_VOID:
    पक्ष ACTION_TYPE_PTR_BUTTON:
    पक्ष ACTION_TYPE_PTR_LOCK:
    पक्ष ACTION_TYPE_CTRL_SET:
    पक्ष ACTION_TYPE_CTRL_LOCK:
    पक्ष ACTION_TYPE_SWITCH_VT:
    पक्ष ACTION_TYPE_TERMINATE:
    पक्ष ACTION_TYPE_REDIRECT_कुञ्चिका:
        पटिनिवत्त true;
    पक्ष ACTION_TYPE_PTR_MOVE:
    पक्ष ACTION_TYPE_PTR_DEFAULT:
        पटिनिवत्त !(controls & CONTROL_MOUSE_कुञ्चिकाः);
    पक्ष ACTION_TYPE_INTERNAL:
        पटिनिवत्त (action->internal.flags & flag) &&
               ((action->internal.clear_latched_mods & mask) == mask);
    स्वभाव:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_अङ्कः_ENTRIES,
                      "Missing action type");
        पटिनिवत्त false;
    }
}

गणना xkb_कुञ्चिका_latch_state {
    NO_LATCH = 0,
    LATCH_कुञ्चिका_DOWN,
    LATCH_PENDING,
    कुञ्चिका_LATCH_STATE_अङ्कः_ENTRIES
};

#define MAX_XKB_कुञ्चिका_LATCH_STATE_LOG2 2
#सचे (कुञ्चिका_LATCH_STATE_अङ्कः_ENTRIES > (1 << MAX_XKB_कुञ्चिका_LATCH_STATE_LOG2)) || \
    (-XKB_MAX_GROUPS) < (INT32_MIN >> MAX_XKB_कुञ्चिका_LATCH_STATE_LOG2) || \
      XKB_MAX_GROUPS > (INT32_MAX >> MAX_XKB_कुञ्चिका_LATCH_STATE_LOG2)
#error "Cannot represent priv field of the group latch filter"
#endif

/* Hold the latch state *and* the group delta */
सङ्घ group_latch_priv {
    uint32_t priv;
    संरचना {
        /* The type is really: enum xkb_key_latch_state, but it is problematic
         * on Windows, because it is interpreted as signed and leads to wrong
         * negative values. */
        unsigned पूर्णाङ्क latch:MAX_XKB_कुञ्चिका_LATCH_STATE_LOG2;
        int32_t group_delta:(32 - MAX_XKB_कुञ्चिका_LATCH_STATE_LOG2);
    };
};

स्थिर शून्य
xkb_filter_group_latch_new(संरचना xkb_server_state *state,
                           संरचना xkb_events *events,
                           संरचना xkb_filter *filter)
{
    ध्रुव सङ्घ group_latch_priv priv = {
        .latch = LATCH_कुञ्चिका_DOWN,
        .group_delta = (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)
            ? filter->action.group.group - state->base.components.base_group
            : filter->action.group.group
    };
    filter->priv = priv.priv;
    /* Like group set */
    apply_group_delta(filter, &state->base, base_group);
}

स्थिर bool
xkb_filter_group_latch_func(संरचना xkb_server_state *state,
                            संरचना xkb_events *events,
                            संरचना xkb_filter *filter,
                            ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                            गणना xkb_कुञ्चिका_direction direction)
{
    सङ्घ group_latch_priv priv = {.priv = filter->priv};
    गणना xkb_कुञ्चिका_latch_state latch = priv.latch;

    सचे (direction == XKB_कुञ्चिका_DOWN) {
        ध्रुव सङ्घ xkb_action *actions = रिक्त;
        ध्रुव xkb_action_गणनम्_t गणनम् = xkb_कुञ्चिका_get_actions(&state->base, कुञ्चिका,
                                                             &actions);

        सचे (latch == LATCH_कुञ्चिका_DOWN) {
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
            सचे (state->a11y_flags & XKB_A11_Y_LATCH_SIMULTANEOUS_कुञ्चिकाः) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                अत्थाय (xkb_action_गणनम्_t k = 0; k < गणनम्; k++) {
                    सचे (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_GROUP_LATCH,
                                                0))
                    {
                        latch = NO_LATCH;
                        भङ्ग;
                    }
                }
            } अञ्ञथा {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        अञ्ञथा सचे (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is
             * pressed, or promote it to a lock if it's the same group delta &
             * flags and latchToLock option is enabled. */
            ध्रुव bool sticky_कुञ्चिकाः = state->base.components.controls
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
            अत्थाय (xkb_action_गणनम्_t k = 0; k < गणनम्; k++) {
                सचे (((actions[k].type == ACTION_TYPE_GROUP_LATCH) ||
                     (actions[k].type == ACTION_TYPE_GROUP_SET && sticky_कुञ्चिकाः)) &&
                     actions[k].group.group == filter->action.group.group &&
                     (actions[k].group.flags & ACTION_ABSOLUTE_SWITCH) ==
                     (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)) {
                    सचे (filter->action.group.flags & ACTION_LATCH_TO_LOCK &&
                        filter->action.group.group != 0)
                    {
                        /* Promote to lock */
                        filter->action.type = ACTION_TYPE_GROUP_LOCK;
                        filter->func = xkb_filter_group_lock_func;
                        xkb_filter_group_lock_new(state, events, filter);
                        state->base.components.latched_group -= priv.group_delta;
                        filter->कुञ्चिका = कुञ्चिका;
                        /* XXX beep beep! */
                        पटिनिवत्त XKB_FILTER_CONSUME;
                    }
                    /* Do nothing if `latchToLock` option is not activated; if
                     * the latch is not broken by the following actions and the
                     * key is not consumed, then another latch filter will be
                     * created.
                     */
                    अनुवर्त;
                }
                अञ्ञथा सचे (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_GROUP_LATCH,
                                                 0))
                {
                    /* Breaks the latch */
                    state->base.components.latched_group -= priv.group_delta;
                    filter->func = रिक्त;
                    पटिनिवत्त XKB_FILTER_CONTINUE;
                }
            }
        }
        अञ्ञथा {
            /* Ignore press in NO_LATCH state */
            प्रतिज्ञा(latch == NO_LATCH);
        }
    }
    अञ्ञथा सचे (कुञ्चिका != filter->कुञ्चिका) {
        /* Ignore repeat/release of other keys */
    }
    अञ्ञथा सचे (direction == XKB_कुञ्चिका_REPEATED) {
        पटिनिवत्त XKB_FILTER_CONSUME; // TODO: check
    }
    अञ्ञथा {
        /* Our key got released.  If we've set it to clear locks, and we
         * currently have a group locked, then release it and
         * don't actually latch.  Else we've actually hit the latching
         * stage, so set PENDING and move our group from base to
         * latched. */
        सचे ((filter->action.group.flags & ACTION_LOCK_CLEAR) &&
             state->base.components.locked_group) {
            सचे (latch == LATCH_PENDING)
                state->base.components.latched_group -= priv.group_delta;
            अञ्ञथा
                state->base.components.base_group -= priv.group_delta;
            state->base.components.locked_group = 0;
            filter->func = रिक्त;
        }
        /* Broken latch */
        अञ्ञथा सचे (latch == NO_LATCH) {
            state->base.components.base_group -= priv.group_delta;
            filter->func = रिक्त;
        }
        /* We may already have reached the latch state if pressing the
         * key multiple times without latch-to-lock enabled. */
        अञ्ञथा सचे (latch == LATCH_कुञ्चिका_DOWN) {
            latch = LATCH_PENDING;
            /* Switch from set to latch */
            state->base.components.base_group -= priv.group_delta;
            state->base.components.latched_group += priv.group_delta;
            /* XXX beep beep! */
        }
    }

    priv.latch = latch;
    filter->priv = priv.priv;

    पटिनिवत्त XKB_FILTER_CONTINUE;
}

स्थिर शून्य
xkb_filter_mod_set_new(संरचना xkb_server_state *state,
                       संरचना xkb_events *events,
                       संरचना xkb_filter *filter)
{
    ध्रुव गणना xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    सचे ((filter->action.mods.flags & unlock) == unlock) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension.
         */
        filter->priv = filter->action.mods.mods.mask
                     & ~state->base.components.locked_mods;
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
    } अञ्ञथा {
        filter->priv = filter->action.mods.mods.mask;
    }

    state->set_mods |= (xkb_mod_mask_t) filter->priv;
}

स्थिर bool
xkb_filter_mod_set_func(संरचना xkb_server_state *state,
                        संरचना xkb_events *events,
                        संरचना xkb_filter *filter,
                        ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                        गणना xkb_कुञ्चिका_direction direction)
{

    सचे (कुञ्चिका != filter->कुञ्चिका) {
        filter->action.mods.flags &= ~ACTION_LOCK_CLEAR;
        पटिनिवत्त XKB_FILTER_CONTINUE;
    }

    विकल्प (direction) {
    पक्ष XKB_कुञ्चिका_DOWN:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_कुञ्चिका_REPEATED:
        पटिनिवत्त XKB_FILTER_CONSUME;
    स्वभाव:
        सचे (--filter->refcnt > 0)
            पटिनिवत्त XKB_FILTER_CONSUME;
    }

    state->clear_mods |= (xkb_mod_mask_t) filter->priv;
    ध्रुव गणना xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    सचे ((filter->action.mods.flags & unlock) == ACTION_LOCK_CLEAR)
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;

    filter->func = रिक्त;
    पटिनिवत्त XKB_FILTER_CONTINUE;
}

स्थिर शून्य
xkb_filter_mod_lock_new(संरचना xkb_server_state *state,
                        संरचना xkb_events *events,
                        संरचना xkb_filter *filter)
{
    filter->priv = (state->base.components.locked_mods &
                    filter->action.mods.mods.mask);

    सचे (filter->priv && (filter->action.mods.flags & ACTION_UNLOCK_ON_PRESS)) {
        /*
         * Some of the target modifiers were locked before key press: unlock.
         *
         * This is a keymap v2 extension: unlock-on-press.
         */
        सचे (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
            state->base.components.locked_mods &= ~filter->priv;
        /* No further action: cancel filter */
        filter->func = रिक्त;
    } अञ्ञथा {
        /* Set base mods; lock mods if relevant (XKB 1.0 spec) */
        state->set_mods |= filter->action.mods.mods.mask;
        सचे (!(filter->action.mods.flags & ACTION_LOCK_NO_LOCK))
            state->base.components.locked_mods |= filter->action.mods.mods.mask;
    }
}

स्थिर bool
xkb_filter_mod_lock_func(संरचना xkb_server_state *state,
                         संरचना xkb_events *events,
                         संरचना xkb_filter *filter,
                         ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                         गणना xkb_कुञ्चिका_direction direction)
{
    सचे (कुञ्चिका != filter->कुञ्चिका)
        पटिनिवत्त XKB_FILTER_CONTINUE;

    विकल्प (direction) {
    पक्ष XKB_कुञ्चिका_DOWN:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_कुञ्चिका_REPEATED:
        पटिनिवत्त XKB_FILTER_CONSUME;
    स्वभाव:
        सचे (--filter->refcnt > 0)
            पटिनिवत्त XKB_FILTER_CONSUME;
    }

    state->clear_mods |= filter->action.mods.mods.mask;
    सचे (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
        state->base.components.locked_mods &= ~filter->priv;

    filter->func = रिक्त;
    पटिनिवत्त XKB_FILTER_CONTINUE;
}

स्थिर शून्य
xkb_filter_mod_latch_new(संरचना xkb_server_state *state,
                         संरचना xkb_events *events,
                         संरचना xkb_filter *filter)
{
    /* Latch-on-press + clear-locks imply unlock-on-press */
    ध्रुव गणना xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                              | ACTION_LATCH_ON_PRESS;

    सचे ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
        (filter->action.mods.flags & unlockOnPress) &&
        (state->base.components.locked_mods & filter->action.mods.mods.mask) ==
         filter->action.mods.mods.mask) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension: clear locks and do not latch.
         */
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
        filter->func = रिक्त;
    } अञ्ञथा सचे (filter->action.mods.flags & ACTION_LATCH_ON_PRESS) {
        /*
         * Latch on key press
         *
         * This is a keymap format v2 extension.
         */
        filter->priv = LATCH_PENDING;
        state->base.components.latched_mods |= filter->action.mods.mods.mask;
        /* XXX beep beep! */
    } अञ्ञथा {
        /* XKB standard latch action */
        filter->priv = LATCH_कुञ्चिका_DOWN;
        state->set_mods |= filter->action.mods.mods.mask;
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

    सचे (direction == XKB_कुञ्चिका_DOWN) {
        ध्रुव सङ्घ xkb_action *actions = रिक्त;
        ध्रुव xkb_action_गणनम्_t गणनम् = xkb_कुञ्चिका_get_actions(&state->base, कुञ्चिका,
                                                             &actions);

        सचे (latch == LATCH_कुञ्चिका_DOWN) {
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
            सचे (state->a11y_flags & XKB_A11_Y_LATCH_SIMULTANEOUS_कुञ्चिकाः) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                अत्थाय (xkb_action_गणनम्_t k = 0; k < गणनम्; k++) {
                    सचे (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_MOD_LATCH,
                                                filter->action.mods.mods.mask))
                    {
                        latch = NO_LATCH;
                        भङ्ग;
                    }
                }
            } अञ्ञथा {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        अञ्ञथा सचे (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is pressed,
             * or promote it to a lock or plain base set if it's the same
             * modifier. */
            ध्रुव bool sticky_कुञ्चिकाः = state->base.components.controls
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
            अत्थाय (xkb_action_गणनम्_t k = 0; k < गणनम्; k++) {
                सचे (((actions[k].type == ACTION_TYPE_MOD_LATCH) ||
                     (actions[k].type == ACTION_TYPE_MOD_SET && sticky_कुञ्चिकाः)) &&
                    actions[k].mods.mods.mask == filter->action.mods.mods.mask)
                {
                    सचे (filter->action.mods.flags & ACTION_LATCH_TO_LOCK) {
                        /* Mutate the action to LockMods() */
                        filter->action.type = ACTION_TYPE_MOD_LOCK;
                        filter->func = xkb_filter_mod_lock_func;
                        xkb_filter_mod_lock_new(state, events, filter);
                    }
                    अञ्ञथा {
                        /* Mutate the action to SetMods() */
                        filter->action.type = ACTION_TYPE_MOD_SET;
                        filter->func = xkb_filter_mod_set_func;
                        xkb_filter_mod_set_new(state, events, filter);
                    }
                    filter->कुञ्चिका = कुञ्चिका;
                    /* Clear latches */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    /* XXX beep beep! */
                    पटिनिवत्त XKB_FILTER_CONSUME;
                }
                अञ्ञथा सचे (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_MOD_LATCH,
                                                 filter->action.mods.mods.mask))
                {
                    /* XXX: This may be totally broken, we might need to break the
                     *      latch in the next run after this press? */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    filter->func = रिक्त;
                    पटिनिवत्त XKB_FILTER_CONTINUE;
                }
            }
        }
        अञ्ञथा {
            /* Ignore press in NO_LATCH state */
            प्रतिज्ञा(latch == NO_LATCH);
        }
    }
    अञ्ञथा सचे (कुञ्चिका != filter->कुञ्चिका) {
        /* Ignore repeat/release of other keys */
    }
    अञ्ञथा सचे (direction == XKB_कुञ्चिका_REPEATED) {
        पटिनिवत्त XKB_FILTER_CONSUME; // TODO: check
    }
    अञ्ञथा {
        /* Our key got released.  If we’ve set it to clear locks, and we
         * currently have the same modifiers locked, then release them and
         * don't actually latch.  Else we’ve actually hit the latching
         * stage, so set PENDING and move our modifier from base to
         * latched. */

        /* Latch-on-press + clear-locks imply unlock-on-press */
        ध्रुव गणना xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                                  | ACTION_LATCH_ON_PRESS;

        सचे ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
            !(filter->action.mods.flags & unlockOnPress) &&
            (state->base.components.locked_mods &
             filter->action.mods.mods.mask) == filter->action.mods.mods.mask)
        {
            /* XXX: We might be a bit overenthusiastic about clearing
             *      mods other filters have set here? */
            सचे (latch == LATCH_PENDING)
                state->base.components.latched_mods &=
                    ~filter->action.mods.mods.mask;
            अञ्ञथा
                state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
            filter->func = रिक्त;
        }
        अञ्ञथा सचे (latch == NO_LATCH) {
            /* Broken latch */
            state->clear_mods |= filter->action.mods.mods.mask;
            filter->func = रिक्त;
        }
        अञ्ञथा सचे (!(filter->action.mods.flags & ACTION_LATCH_ON_PRESS)) {
            latch = LATCH_PENDING;
            state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.latched_mods |= filter->action.mods.mods.mask;
            /* XXX beep beep! */
        }
    }

    filter->priv = latch;

    पटिनिवत्त XKB_FILTER_CONTINUE;
}

स्थिर शून्य
append_pointer_move(संरचना xkb_events *events,
                    संरचना xkb_filter *filter)
{
    गणना xkb_pointer_motion_flags flags = 0;
    सचे (filter->action.ptr.flags & ACTION_ABSOLUTE_X)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_X;
    सचे (filter->action.ptr.flags & ACTION_ABSOLUTE_Y)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_Y;
    सचे (filter->action.ptr.flags & ACTION_REPEAT)
        flags |= XKB_POINTER_MOTION_REPEATS;
    darray_append(events->queue, (संरचना xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_MOTION,
        .pointer_motion = {
            .size = परिमाण(((संरचना xkb_event*)0)->pointer_motion),
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
    सचे (!events) {
        filter->func = रिक्त;
        पटिनिवत्त;
    }

    प्रतिज्ञा(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_कुञ्चिकाः);
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
    सचे (कुञ्चिका != filter->कुञ्चिका || !events)
        पटिनिवत्त XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;

    विकल्प (direction) {
    पक्ष XKB_कुञ्चिका_DOWN:
        filter->refcnt++;
        append_pointer_move(events, filter);
        पटिनिवत्त XKB_FILTER_CONSUME;
    पक्ष XKB_कुञ्चिका_REPEATED:
        सचे (filter->action.ptr.flags & ACTION_REPEAT)
            append_pointer_move(events, filter);
        पटिनिवत्त XKB_FILTER_CONSUME;
    स्वभाव:
        सचे (--filter->refcnt > 0)
            पटिनिवत्त XKB_FILTER_CONSUME;
    }

    filter->func = रिक्त;
    पटिनिवत्त XKB_FILTER_CONSUME;
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
xkb_filter_pointer_lock_button_new(संरचना xkb_server_state *state,
                                   संरचना xkb_events *events,
                                   संरचना xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
स्थिर bool
xkb_filter_pointer_lock_button_func(संरचना xkb_server_state *state,
                                    संरचना xkb_events *events,
                                    संरचना xkb_filter *filter,
                                    ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                                    गणना xkb_कुञ्चिका_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
स्थिर शून्य
xkb_filter_pointer_set_default_button_new(संरचना xkb_server_state *state,
                                          संरचना xkb_events *events,
                                          संरचना xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
स्थिर bool
xkb_filter_pointer_set_default_button_func(संरचना xkb_server_state *state,
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
    सचे (filter->action.type == ACTION_TYPE_CTRL_SET) {
        /* Set: save the specified controls that are *not* already enabled */
        filter->priv = (uint32_t) (
            (~state->base.components.controls) & filter->action.ctrls.ctrls
        );
    } अञ्ञथा {
        /* Lock: save the specified controls that *are* already enabled */
        filter->priv = (uint32_t) (
            state->base.components.controls & filter->action.ctrls.ctrls
        );
    }

    सचे (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_LOCK)) {
        /* Enable the specified controls that are not already enabled */
        state->base.components.controls |= filter->action.ctrls.ctrls;
    }
}

स्थिर bool
xkb_filter_ctrls_func(संरचना xkb_server_state *state,
                      संरचना xkb_events *events,
                      संरचना xkb_filter *filter,
                      ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                      गणना xkb_कुञ्चिका_direction direction)
{
    सचे (कुञ्चिका != filter->कुञ्चिका)
        पटिनिवत्त XKB_FILTER_CONTINUE;

    विकल्प (direction) {
    पक्ष XKB_कुञ्चिका_DOWN:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_कुञ्चिका_REPEATED:
        पटिनिवत्त XKB_FILTER_CONSUME;
    स्वभाव:
        सचे (--filter->refcnt > 0)
            पटिनिवत्त XKB_FILTER_CONSUME;
    }

    सचे (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_UNLOCK)) {
        ध्रुव गणना xkb_action_controls old = state->base.components.controls;

        /*
         * Set: Disable specified controls that were *not* enabled at key press.
         * Lock: Disable specified controls that *were* enabled at key press.
         */
        state->base.components.controls &= ~(गणना xkb_action_controls) filter->priv;

        सचे ((old & CONTROL_STICKY_कुञ्चिकाः) &&
            !(state->base.components.controls & CONTROL_STICKY_कुञ्चिकाः)) {
            /* Sticky keys were disabled: clear all locks and latches */
            clear_all_latches_and_locks(state, events);
        }
    }

    filter->func = रिक्त;
    पटिनिवत्त XKB_FILTER_CONTINUE;
}

स्थिर bool
append_redirect_कुञ्चिका_events(संरचना xkb_server_state *state,
                           संरचना xkb_events *events,
                           ध्रुव संरचना xkb_redirect_कुञ्चिका_action *redirect,
                           गणना xkb_कुञ्चिका_direction direction)
{
    गणना xkb_state_component changed = 0;
    ध्रुव xkb_mod_mask_t mask = redirect->affect_mods;

    /*
     * Reference state: find the last state update in the queue, otherwise
     * use the current state.
     */
    संरचना xkb_event *event;
    संरचना state_components last_components = state->base.components;
    darray_foreach_reverse(event, events->queue) {
        सचे (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
            last_components = event->components.components;
            भङ्ग;
        }
    }

    सचे (mask) {
        संरचना state_components new = last_components;
        new.base_mods = (new.base_mods & ~mask) | redirect->mods;
        new.latched_mods = (new.latched_mods & ~mask) | redirect->mods;
        new.locked_mods = (new.locked_mods & ~mask) | redirect->mods;
        new.mods = (new.mods & ~mask) | redirect->mods;

        changed = get_state_component_changes(&last_components, &new);
        सचे (changed) {
            darray_append(events->queue, (संरचना xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = new,
                    .changed = changed
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

    सचे (mask && changed) {
        /* Restore state */
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = last_components,
                .changed = changed
            }
        });
    }

    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;
    पटिनिवत्त true;
}

स्थिर शून्य
xkb_filter_redirect_कुञ्चिका_new(संरचना xkb_server_state *state,
                            संरचना xkb_events *events,
                            संरचना xkb_filter *filter)
{
    /* Action effectual only with the xkb_machine API and a valid keycode */
    सचे (!events || filter->action.redirect.keycode == XKB_KEYCODE_INVALID) {
        filter->func = रिक्त;
        पटिनिवत्त;
    }
    append_redirect_कुञ्चिका_events(state, events, &filter->action.redirect,
                               XKB_कुञ्चिका_DOWN);
}

स्थिर bool
xkb_filter_redirect_कुञ्चिका_func(संरचना xkb_server_state *state,
                             संरचना xkb_events *events,
                             संरचना xkb_filter *filter,
                             ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                             गणना xkb_कुञ्चिका_direction direction)
{
    सचे (कुञ्चिका != filter->कुञ्चिका)
        पटिनिवत्त XKB_FILTER_CONTINUE;

    सचे (direction == XKB_कुञ्चिका_UP) {
        append_redirect_कुञ्चिका_events(state, events,
                                   &filter->action.redirect, XKB_कुञ्चिका_UP);
        filter->func = रिक्त;
        पटिनिवत्त XKB_FILTER_CONSUME;
     } अञ्ञथा सचे (direction == XKB_कुञ्चिका_DOWN) {
        ध्रुव सङ्घ xkb_action *actions = रिक्त;
        ध्रुव xkb_action_गणनम्_t गणनम् =
            xkb_कुञ्चिका_get_actions(&state->base, कुञ्चिका, &actions);

        अत्थाय (xkb_action_गणनम्_t a = 0; a < गणनम्; a++) {
            सचे (actions[a].type == ACTION_TYPE_REDIRECT_कुञ्चिका &&
                actions[a].redirect.keycode != filter->action.redirect.keycode) {
                    /* One action redirects to a different key: release first */
                    append_redirect_कुञ्चिका_events(state, events,
                                               &filter->action.redirect,
                                               XKB_कुञ्चिका_UP);
                filter->func = रिक्त;
                पटिनिवत्त XKB_FILTER_CONTINUE;
            }
        }
    }

    append_redirect_कुञ्चिका_events(state, events, &filter->action.redirect,
                               direction);
    पटिनिवत्त XKB_FILTER_CONSUME;
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
    [ACTION_TYPE_MOD_SET]     = { xkb_filter_mod_set_new,
                                  xkb_filter_mod_set_func },
    [ACTION_TYPE_MOD_LATCH]   = { xkb_filter_mod_latch_new,
                                  xkb_filter_mod_latch_func },
    [ACTION_TYPE_MOD_LOCK]    = { xkb_filter_mod_lock_new,
                                  xkb_filter_mod_lock_func },
    [ACTION_TYPE_GROUP_SET]   = { xkb_filter_group_set_new,
                                  xkb_filter_group_set_func },
    [ACTION_TYPE_GROUP_LATCH] = { xkb_filter_group_latch_new,
                                  xkb_filter_group_latch_func },
    [ACTION_TYPE_GROUP_LOCK]  = { xkb_filter_group_lock_new,
                                  xkb_filter_group_lock_func },
    [ACTION_TYPE_PTR_MOVE]    = { xkb_filter_pointer_move_new,
                                  xkb_filter_pointer_move_func },
    [ACTION_TYPE_PTR_BUTTON]  = { xkb_filter_pointer_button_new,
                                  xkb_filter_pointer_button_func },
    [ACTION_TYPE_PTR_LOCK]    = { xkb_filter_pointer_lock_button_new,
                                  xkb_filter_pointer_lock_button_func },
    [ACTION_TYPE_PTR_DEFAULT] = { xkb_filter_pointer_set_default_button_new,
                                  xkb_filter_pointer_set_default_button_func },
    [ACTION_TYPE_TERMINATE]   = { xkb_filter_terminate_server_new,
                                  xkb_filter_terminate_server_func },
    [ACTION_TYPE_SWITCH_VT]   = { xkb_filter_switch_virtual_console_new,
                                  xkb_filter_switch_virtual_console_func },
    [ACTION_TYPE_CTRL_SET]    = { xkb_filter_ctrls_new,
                                  xkb_filter_ctrls_func },
    [ACTION_TYPE_CTRL_LOCK]   = { xkb_filter_ctrls_new,
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
        सचे (!filter->func)
            अनुवर्त;

        सचे (filter->func(state, events, filter, कुञ्चिका, direction) == XKB_FILTER_CONSUME)
            consumed = true;
    }
    सचे (consumed || direction == XKB_कुञ्चिका_UP)
        पटिनिवत्त;

    /* No filter consumed this event, so proceed with the key actions */
    ध्रुव सङ्घ xkb_action *actions = रिक्त;
    xkb_action_गणनम्_t गणनम् = xkb_कुञ्चिका_get_actions(&state->base, कुञ्चिका,
                                                         &actions);

    अत्थाय (संरचना {
            xkb_action_गणनम्_t गणनम्;
            ध्रुव सङ्घ xkb_action *actions;
         } next = {0};
         गणनम्;
         गणनम् = next.गणनम्, next.गणनम् = 0, actions = next.actions
    ) {
        /*
         * Process actions sequentially.
         *
         * NOTE: We rely on the parser to disallow multiple modifier or group
         * actions (see `CheckMultipleActionsCategories`). Allowing multiple
         * such actions requires a refactor of the state handling.
         */
        अत्थाय (xkb_action_गणनम्_t k = 0; k < गणनम्; k++) {
            /*
             * It's possible for the keymap to set action->type explicitly, like
             * so:
             *     interpret XF86_Next_VMode {
             *         action = Private(type=0x86, data="+VMode");
             *     };
             * We don't handle those.
             */
            सचे (actions[k].type >= ACTION_TYPE_अङ्कः_ENTRIES)
                अनुवर्त;

            /* Go to next action if no corresponding action handler */
            सचे (!filter_action_funcs[actions[k].type].new)
                अनुवर्त;

            /* Add a new filter and run the corresponding initial action */
            filter = xkb_filter_new(state);
            filter->कुञ्चिका = कुञ्चिका;
            filter->action = actions[k];

            विकल्प (filter->action.type) {
            पक्ष ACTION_TYPE_MOD_SET:
                सचे (state->base.components.controls & CONTROL_STICKY_कुञ्चिकाः) {
                    /* Convert modifier set action to a latch */
                    filter->action.type = ACTION_TYPE_MOD_LATCH;
                    सचे (state->a11y_flags & XKB_A11_Y_STICKY_कुञ्चिकाः_LATCH_TO_LOCK) {
                        filter->action.mods.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                /* fallthrough */
            पक्ष ACTION_TYPE_MOD_LATCH:
            पक्ष ACTION_TYPE_MOD_LOCK: {
                स्थिर ध्रुव गणना xkb_action_flags unlock_mods_on_press
                    = ACTION_LOCK_CLEAR     /* SetMods & LatchMods: can unlock */
                    | ACTION_LOCK_NO_UNLOCK /* LockMods: cannot unlock */
                    | ACTION_EXPLICIT_UNLOCK_ON_PRESS;
                सचे ((state->flags & STATE_UNLOCK_MODS_ON_PRESS) &&
                    /*
                     * Ensure that the action can actually unlock and has
                     * no *explicit* `unlockOnPress` parameter.
                     */
                    (filter->action.mods.flags & unlock_mods_on_press) ==
                    (filter->action.type == ACTION_TYPE_MOD_LOCK
                        ? 0 : ACTION_LOCK_CLEAR))
                {
                    filter->action.mods.flags |= ACTION_UNLOCK_ON_PRESS;
                }
                भङ्ग;
            }
            पक्ष ACTION_TYPE_GROUP_SET:
                सचे (state->base.components.controls & CONTROL_STICKY_कुञ्चिकाः) {
                    /* Convert group set action to a latch */
                    filter->action.type = ACTION_TYPE_GROUP_LATCH;
                    सचे (state->a11y_flags & XKB_A11_Y_STICKY_कुञ्चिकाः_LATCH_TO_LOCK) {
                        filter->action.group.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                भङ्ग;
            पक्ष ACTION_TYPE_GROUP_LOCK:
                सचे ((filter->action.group.flags &
                     ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE) &&
                    (state->flags &
                     STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_कुञ्चिका) &&
                    /*
                     * FIXME: sticky keys may leave an unexpected modifier
                     * from the level 0 latched, so disable for now.
                     */
                    !(state->base.components.controls & CONTROL_STICKY_कुञ्चिकाः))
                {
                    filter->action.group.flags |= ACTION_LOCK_ON_RELEASE;
                    /* Add base level action */
                    next.गणनम् = xkb_keymap_कुञ्चिका_get_actions_by_level(
                        state->base.keymap, कुञ्चिका, state->base.components.group,
                        0, &next.actions
                    );
                    प्रतिज्ञा(next.गणनम् == 1);
                }
                भङ्ग;
            पक्ष ACTION_TYPE_PTR_MOVE:
            पक्ष ACTION_TYPE_PTR_BUTTON:
            पक्ष ACTION_TYPE_PTR_LOCK:
            पक्ष ACTION_TYPE_PTR_DEFAULT:
                सचे (!(state->base.components.controls & CONTROL_MOUSE_कुञ्चिकाः)) {
                    /* Convert pointer actions to NoAction */
                    प्रतिज्ञा(!filter_action_funcs[ACTION_TYPE_NONE].new);
                    filter->func = रिक्त;
                    अनुवर्त;
                }
                भङ्ग;
            पक्ष ACTION_TYPE_REDIRECT_कुञ्चिका:
                // FIXME: this is not efficient to resolve mods here each time
                filter->action.redirect.affect_mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.affect_mods
                );
                filter->action.redirect.mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.mods
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
xkb_state_init(संरचना xkb_state * restrict state,
               संरचना xkb_keymap * restrict keymap,
               गणना xkb_state_mode_internal mode)
{
    state->mode = mode;
    state->refcnt = 1;
    state->keymap = xkb_keymap_ref(keymap);
    state->out_of_range_group.policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

    /* Ensure that derived state is correctly initialized */
    xkb_state_update_derived(state);
}

स्थिर अन्तःस्थ शून्य
xkb_client_state_init(संरचना xkb_client_state * restrict state,
                      संरचना xkb_keymap * restrict keymap,
                      गणना xkb_state_mode_internal mode)
{
    xkb_state_init(&state->base, keymap, mode);
}

स्थिर अन्तःस्थ शून्य
xkb_server_state_init(संरचना xkb_server_state * restrict state,
                      संरचना xkb_keymap * restrict keymap,
                      गणना xkb_state_mode_internal mode,
                      गणना xkb_a11y_flags a11y_affect,
                      गणना xkb_a11y_flags a11y_flags)
{
    xkb_state_init(&state->base, keymap, mode);

    state->a11y_flags = a11y_flags;
    सचे (keymap->format != XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !(a11y_affect & XKB_A11_Y_LATCH_SIMULTANEOUS_कुञ्चिकाः)) {
             /* Keymap v2+: enable extension to XKB if not manually disabled */
             state->a11y_flags |= XKB_A11_Y_LATCH_SIMULTANEOUS_कुञ्चिकाः;
    }
}

संरचना xkb_state *
xkb_state_new(संरचना xkb_keymap *keymap)
{
    संरचना xkb_server_state * ध्रुव state = calloc(1, परिमाण(*state));
    सचे (!state) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "Could not allocate a state object.\n");
        पटिनिवत्त रिक्त;
    }

    xkb_server_state_init(state, keymap, LEGACY_MIXED_STATE, 0, 0);

    पटिनिवत्त (संरचना xkb_state *)state;
}

संरचना xkb_state *
xkb_state_new_with_mode(संरचना xkb_keymap * restrict keymap,
                        गणना xkb_state_mode mode,
                        गणना xkb_status * restrict status)
{
    विकल्प (mode) {
    पक्ष XKB_STATE_MODE_CLIENT:
    पक्ष XKB_STATE_MODE_SERVER_QUERY: {
        संरचना xkb_client_state * ध्रुव state = calloc(1, परिमाण(*state));
        सचे (!state)
            भङ्ग;

        static_assert((unsigned)XKB_STATE_MODE_CLIENT ==
                      (unsigned)CLIENT_STATE, "");
        static_assert((unsigned)XKB_STATE_MODE_SERVER_QUERY ==
                      (unsigned)SERVER_COMPANION, "");
        xkb_client_state_init(state, keymap, (गणना xkb_state_mode_internal)mode);

        सचे (status)
            *status = XKB_SUCCESS;

        पटिनिवत्त (संरचना xkb_state *)state;
    }
    पक्ष XKB_STATE_MODE_SERVER: {
        संरचना xkb_server_state * ध्रुव state = calloc(1, परिमाण(*state));
        सचे (!state)
            भङ्ग;

        xkb_server_state_init(state, keymap, LEGACY_SERVER_STATE, 0, 0);

        सचे (status)
            *status = XKB_SUCCESS;

        पटिनिवत्त (संरचना xkb_state *)state;
    }
    स्वभाव:
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_STATE_MODE_,
                     "Unsupported state mode: %u\n", mode);
        सचे (status)
            *status = XKB_ERROR_UNSUPPORTED_STATE_MODE;
        पटिनिवत्त रिक्त;
    }

    log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                  "Could not allocate a state object.\n");
    सचे (status)
        *status = XKB_ERROR_ALLOCATION_FAILURE;
    पटिनिवत्त रिक्त;
}

संरचना xkb_state *
xkb_state_ref(संरचना xkb_state *state)
{
    प्रतिज्ञा(state->refcnt > 0);
    state->refcnt++;
    पटिनिवत्त state;
}

शून्य
xkb_state_reset(संरचना xkb_state *state)
{
    memset(&state->components, 0, परिमाण(state->components));

    सचे (state->mode < LEGACY_MIXED_STATE) {
        static_assert(परिमाण(*state) == परिमाण(संरचना xkb_client_state),
                      "Not a mere wrapper");
        /* Nothing else to reset */
    } अञ्ञथा {
        संरचना xkb_server_state *state_ = (संरचना xkb_server_state *)state;
        ध्रुव संरचना xkb_server_state prev = *state_;
        memset(state_, 0, परिमाण(*state_));

        state_->base = prev.base;
        state_->a11y_flags = prev.a11y_flags;
        state_->filters = prev.filters;
        darray_size(state_->filters) = 0;
    }
}

स्थिर अन्तःस्थ शून्य
xkb_state_destroy(संरचना xkb_state *state)
{
    xkb_keymap_unref(state->keymap);
    सचे (state->mode > SERVER_COMPANION)
        darray_free(((संरचना xkb_server_state *)state)->filters);
}

शून्य
xkb_state_unref(संरचना xkb_state *state)
{
    प्रतिज्ञा(!state || state->refcnt > 0);
    सचे (!state || --state->refcnt > 0)
        पटिनिवत्त;

    xkb_state_destroy(state);

    विकल्प (state->mode) {
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
    पटिनिवत्त state->keymap;
}

/**
 * Update the LED state to match the rest of the xkb_state.
 */
स्थिर शून्य
xkb_state_led_update_all(संरचना xkb_state *state)
{
    xkb_led_index_t idx;
    ध्रुव संरचना xkb_led *led;

    state->components.leds = 0;

    xkb_leds_enumerate(idx, led, state->keymap) {

        सचे (led->which_mods != 0 && led->mods.mask != 0) {
            xkb_mod_mask_t mod_mask = 0;
            सचे (led->which_mods & XKB_STATE_MODS_EFFECTIVE)
                mod_mask |= state->components.mods;
            सचे (led->which_mods & XKB_STATE_MODS_DEPRESSED)
                mod_mask |= state->components.base_mods;
            सचे (led->which_mods & XKB_STATE_MODS_LATCHED)
                mod_mask |= state->components.latched_mods;
            सचे (led->which_mods & XKB_STATE_MODS_LOCKED)
                mod_mask |= state->components.locked_mods;

            सचे (led->mods.mask & mod_mask) {
                state->components.leds |= (UINT32_C(1) << idx);
                अनुवर्त;
            }
        }

        सचे (led->which_groups != 0) {
            सचे (likely(led->groups) != 0) {
                xkb_layout_mask_t group_mask = 0;
                /* Effective and locked groups have been brought into range */
                प्रतिज्ञा(state->components.group < XKB_MAX_GROUPS);
                प्रतिज्ञा(state->components.locked_group >= 0 &&
                       state->components.locked_group < XKB_MAX_GROUPS);
                /* Effective and locked groups are used as mask */
                सचे (led->which_groups & XKB_STATE_LAYOUT_EFFECTIVE)
                    group_mask |= (UINT32_C(1) << state->components.group);
                सचे (led->which_groups & XKB_STATE_LAYOUT_LOCKED)
                    group_mask |= (UINT32_C(1) << state->components.locked_group);
                /* Base and latched groups only have to be non-zero */
                सचे ((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                    state->components.base_group != 0)
                    group_mask |= led->groups;
                सचे ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                    state->components.latched_group != 0)
                    group_mask |= led->groups;

                सचे (led->groups & group_mask) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    अनुवर्त;
                }
            } अञ्ञथा {
                /* Special case for Base and latched groups */
                सचे (((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                     state->components.base_group == 0) ||
                    ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                     state->components.latched_group == 0)) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    अनुवर्त;
                }
            }
        }

        सचे (led->ctrls & state->components.controls) {
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
    xkb_layout_index_t wrapped;

    state->components.mods = (state->components.base_mods |
                              state->components.latched_mods |
                              state->components.locked_mods);

    /* Lock group must be adjusted, but not base nor latched groups */
    wrapped = XkbWrapGroupIntoRange(state->components.locked_group,
                                    state->keymap->अङ्कः_groups,
                                    state->out_of_range_group.policy,
                                    state->out_of_range_group.redirect_group);
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->components.locked_group =
        (int32_t) (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    /* Effective group must be adjusted */
    wrapped = XkbWrapGroupIntoRange(state->components.base_group +
                                    state->components.latched_group +
                                    state->components.locked_group,
                                    state->keymap->अङ्कः_groups,
                                    state->out_of_range_group.policy,
                                    state->out_of_range_group.redirect_group);
    state->components.group =
        (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    xkb_state_led_update_all(state);
}

स्थिर शून्य
state_update_enabled_controls(संरचना xkb_server_state *state,
                              गणना xkb_keyboard_control_flags affect_controls,
                              गणना xkb_keyboard_control_flags controls,
                              संरचना xkb_events *events)
{
    ध्रुव bool had_sticky_कुञ्चिकाः = state->base.components.controls
                               & CONTROL_STICKY_कुञ्चिकाः;

    /*
     * Enable using the public API with the all the Control values, except
     * the internal ones, if any.
     */
    affect_controls =
        ( affect_controls
        & (गणना xkb_keyboard_control_flags)CONTROL_ALL_BOOLEAN );
    state->base.components.controls &=
        (गणना xkb_action_controls)~affect_controls;
    state->base.components.controls |=
        (गणना xkb_action_controls)(controls & affect_controls);

    सचे (had_sticky_कुञ्चिकाः &&
        !(state->base.components.controls & CONTROL_STICKY_कुञ्चिकाः)) {
        /* Sticky keys were disabled: clear all locks and latches */
        clear_all_latches_and_locks(state, events);
    }

    xkb_state_update_derived(&state->base);
}

/**
 * Given a particular key event, updates the state structure to reflect the
 * new modifiers.
 */
गणना xkb_state_component
xkb_state_update_कुञ्चिका(संरचना xkb_state *base_state, xkb_keycode_t kc,
                     गणना xkb_कुञ्चिका_direction direction)
{
    /* Guard against client-only state */
    प्रतिज्ञा(base_state->mode > SERVER_COMPANION);
    सचे (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        पटिनिवत्त 0;
    }

    संरचना xkb_server_state * ध्रुव state =
        (संरचना xkb_server_state *)base_state;

    ध्रुव संरचना xkb_कुञ्चिका* ध्रुव कुञ्चिका = Xkb_कुञ्चिका(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    सचे (!कुञ्चिका || (direction == XKB_कुञ्चिका_REPEATED && !कुञ्चिका->repeats))
        पटिनिवत्त 0;

    सचे (direction == XKB_कुञ्चिका_DOWN &&
        (state->base.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_कुञ्चिकाः) &&
        (state->a11y_flags & XKB_A11_Y_STICKY_कुञ्चिकाः_NO_SIMULTANEOUS_कुञ्चिकाः) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_कुञ्चिकाः,
                                      0, रिक्त);
    }

    ध्रुव संरचना state_components prev_components = state->base.components;

    state->set_mods = 0;
    state->clear_mods = 0;

    xkb_filter_apply_all(state, रिक्त, कुञ्चिका, direction);

    xkb_mod_index_t i;
    xkb_mod_mask_t bit;
    अत्थाय (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        सचे (state->set_mods & bit) {
            state->mod_कुञ्चिका_गणनम्[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    अत्थाय (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        सचे (state->clear_mods & bit) {
            state->mod_कुञ्चिका_गणनम्[i]--;
            सचे (state->mod_कुञ्चिका_गणनम्[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_कुञ्चिका_गणनम्[i] = 0;
            }
            state->clear_mods &= ~bit;
        }
    }

    xkb_state_update_derived(&state->base);

    पटिनिवत्त get_state_component_changes(&prev_components, &state->base.components);
}

/* We need fake keys for `update_latch_modifiers` and `update_latch_group`.
 * These keys must have at least one level in order to break latches. We need 2
 * keys with specific actions in order to update group/mod latches without
 * affecting each other. */
स्थिर संरचना xkb_कुञ्चिका_type_entry synthetic_कुञ्चिका_level_entry = { 0 };
स्थिर संरचना xkb_कुञ्चिका_type synthetic_कुञ्चिका_type = {
     .अङ्कः_entries = 1,
     .अङ्कः_levels = 1,
     .entries = &synthetic_कुञ्चिका_level_entry
};
स्थिर ध्रुव संरचना xkb_कुञ्चिका synthetic_कुञ्चिका = { 0 };

/* Transcription from xserver: XkbLatchModifiers */
स्थिर शून्य
update_latch_modifiers(संरचना xkb_server_state *state,
                       संरचना xkb_events *events,
                       xkb_mod_mask_t mask, xkb_mod_mask_t latches)
{
    /* Clear affected latched modifiers */
    ध्रुव xkb_mod_mask_t clear = mask & ~latches;
    state->base.components.latched_mods &= ~clear;

    /* Clear any pending latch to locks using ad hoc action:
     * only affect corresponding modifier latches and no group latch. */
    संरचना xkb_level synthetic_कुञ्चिका_level_break_mod_latch = {
        .अङ्कः_syms = 0,
        .अङ्कः_actions = 1,
        .upper = XKB_कुञ्चिका_No_चिह्नम्,
        .s.sym = XKB_कुञ्चिका_No_चिह्नम्,
        .a.action.internal = {
            .type = ACTION_TYPE_INTERNAL,
            .flags = INTERNAL_BREAKS_MOD_LATCH,
            .clear_latched_mods = clear
        }
    };
    संरचना xkb_group synthetic_कुञ्चिका_group_break_mod_latch = {
        .type = &synthetic_कुञ्चिका_type,
        .levels = &synthetic_कुञ्चिका_level_break_mod_latch
    };
    ध्रुव संरचना xkb_कुञ्चिका synthetic_कुञ्चिका_break_mod_latch = {
        .अङ्कः_groups = 1,
        .groups = &synthetic_कुञ्चिका_group_break_mod_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_कुञ्चिका_break_mod_latch, XKB_कुञ्चिका_DOWN);

    /* Finally set the latched mods by simulate tapping a key with the
     * corresponding action */
    ध्रुव संरचना xkb_कुञ्चिका* ध्रुव कुञ्चिका = &synthetic_कुञ्चिका;
    ध्रुव सङ्घ xkb_action latch_mods = {
        .mods = {
            .type = ACTION_TYPE_MOD_LATCH,
            .mods = { .mask = mask & latches },
            .flags = 0,
        },
    };
    संरचना xkb_filter* ध्रुव filter = xkb_filter_new(state);
    filter->कुञ्चिका = कुञ्चिका;
    filter->func = xkb_filter_mod_latch_func;
    filter->action = latch_mods;
    xkb_filter_mod_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_mod_latch_func(state, events, filter, कुञ्चिका, XKB_कुञ्चिका_UP);
}

/* Transcription from xserver: XkbLatchGroup */
स्थिर शून्य
update_latch_group(संरचना xkb_server_state *state,
                   संरचना xkb_events *events, int32_t group)
{
    /* Clear any pending latch to locks. */
    स्थिर संरचना xkb_level synthetic_कुञ्चिका_level_break_group_latch = {
        .अङ्कः_syms = 0,
        .अङ्कः_actions = 1,
        .upper = XKB_कुञ्चिका_No_चिह्नम्,
        .s.sym = XKB_कुञ्चिका_No_चिह्नम्,
        .a.action.internal = {
            .type = ACTION_TYPE_INTERNAL,
            .flags = INTERNAL_BREAKS_GROUP_LATCH,
            .clear_latched_mods = 0
        }
    };
    स्थिर संरचना xkb_group synthetic_कुञ्चिका_group_break_group_latch = {
        .type = &synthetic_कुञ्चिका_type,
        .levels = &synthetic_कुञ्चिका_level_break_group_latch
    };
    स्थिर ध्रुव संरचना xkb_कुञ्चिका synthetic_कुञ्चिका_break_group_latch = {
        .अङ्कः_groups = 1,
        .groups = &synthetic_कुञ्चिका_group_break_group_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_कुञ्चिका_break_group_latch, XKB_कुञ्चिका_DOWN);

    /* Simulate tapping a key with a group latch action, but in isolation: i.e.
     * without affecting the other filters. */
    ध्रुव संरचना xkb_कुञ्चिका* ध्रुव कुञ्चिका = &synthetic_कुञ्चिका;
    ध्रुव सङ्घ xkb_action latch_group = {
        .group = {
            .type = ACTION_TYPE_GROUP_LATCH,
            .flags = ACTION_ABSOLUTE_SWITCH,
            .group = group,
        },
    };
    संरचना xkb_filter* ध्रुव filter = xkb_filter_new(state);
    filter->कुञ्चिका = कुञ्चिका;
    filter->func = xkb_filter_group_latch_func;
    filter->action = latch_group;
    xkb_filter_group_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_group_latch_func(state, events, filter, कुञ्चिका, XKB_कुञ्चिका_UP);
}

/**
 * Compute the resolved effective mask of an arbitrary input.
 *
 * Contrary to `mod_mask_get_effective`, it resolves only modifiers not present
 * in the canonical mask, so that it enables `xkb_state_serialize_mods` to
 * round trip via `xkb_state_update_mask`.
 */
स्थिर अन्तःस्थ xkb_mod_mask_t
resolve_to_canonical_mods(संरचना xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    पटिनिवत्त
        /*
         * Keep canonical modifier mask.
         * It contains either real modifiers or canonical virtual modifiers.
         */
        (mods & keymap->canonical_state_mask) |
        /* Resolve other modifiers */
        mod_mask_get_effective(keymap,
                               mods & ~keymap->canonical_state_mask);
}

स्थिर शून्य
state_update_latched_locked(
    संरचना xkb_server_state * restrict state,
    ध्रुव संरचना xkb_state_components_update * restrict update,
    संरचना xkb_events * restrict events
)
{
    /* Update locks */
    ध्रुव xkb_mod_mask_t affect_locked_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_locked_mods);
    सचे ((update->components & XKB_STATE_MODS_LOCKED) &&
        affect_locked_mods) {
        ध्रुव xkb_mod_mask_t locked_mods =
            resolve_to_canonical_mods(state->base.keymap, update->locked_mods);
        state->base.components.locked_mods &= ~affect_locked_mods;
        state->base.components.locked_mods |= locked_mods & affect_locked_mods;
    }
    सचे (update->components & XKB_STATE_LAYOUT_LOCKED) {
        state->base.components.locked_group = update->locked_layout;
    }

    /* Update latches */
    ध्रुव xkb_mod_mask_t affect_latched_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_latched_mods);
    सचे ((update->components & XKB_STATE_MODS_LATCHED) &&
        affect_latched_mods) {
        ध्रुव xkb_mod_mask_t latched_mods =
            resolve_to_canonical_mods(state->base.keymap, update->latched_mods);
        update_latch_modifiers(state, events, affect_latched_mods, latched_mods);
    }
    सचे (update->components & XKB_STATE_LAYOUT_LATCHED) {
        update_latch_group(state, events, update->latched_layout);
    }
}

गणना xkb_state_component
xkb_state_update_latched_locked(संरचना xkb_state *base_state,
                                xkb_mod_mask_t affect_latched_mods,
                                xkb_mod_mask_t latched_mods,
                                bool affect_latched_layout,
                                int32_t latched_layout,
                                xkb_mod_mask_t affect_locked_mods,
                                xkb_mod_mask_t locked_mods,
                                bool affect_locked_layout,
                                int32_t locked_layout)
{
    /* Guard against client-only state */
    प्रतिज्ञा(base_state->mode > SERVER_COMPANION);
    सचे (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        पटिनिवत्त 0;
    }

    संरचना xkb_server_state * ध्रुव state =
        (संरचना xkb_server_state *)base_state;

    ध्रुव संरचना state_components previous_components = state->base.components;
    ध्रुव गणना xkb_state_component components
        = ((affect_latched_mods || latched_mods) ? XKB_STATE_MODS_LATCHED : 0)
        | ((affect_locked_mods || locked_mods) ? XKB_STATE_MODS_LOCKED : 0)
        | (affect_latched_layout ? XKB_STATE_LAYOUT_LATCHED : 0)
        | (affect_locked_layout ? XKB_STATE_LAYOUT_LOCKED : 0);
    ध्रुव संरचना xkb_state_components_update update = {
        .size = परिमाण(update),
        .components = components,
        .affect_latched_mods = affect_latched_mods,
        .latched_mods = latched_mods,
        .latched_layout = latched_layout,
        .affect_locked_mods = affect_locked_mods,
        .locked_mods = locked_mods,
        .locked_layout = locked_layout
    };
    state_update_latched_locked(state, &update, रिक्त);
    xkb_state_update_derived(&state->base);
    पटिनिवत्त get_state_component_changes(&previous_components,
                                       &state->base.components);
}

स्थिर अन्तःस्थ शून्य
clear_all_latches_and_locks(संरचना xkb_server_state *state,
                            संरचना xkb_events *events)
{
    स्थिर ध्रुव गणना xkb_state_component components
        = XKB_STATE_MODS_LATCHED | XKB_STATE_MODS_LOCKED
        | XKB_STATE_LAYOUT_LATCHED | XKB_STATE_LAYOUT_LOCKED;
    ध्रुव संरचना xkb_state_components_update update = {
        .size = परिमाण(update),
        .components = components,
        .affect_latched_mods = (xkb_mod_mask_t)XKB_MOD_ALL,
        .latched_mods = 0,
        .latched_layout = 0,
        .affect_locked_mods = (xkb_mod_mask_t)XKB_MOD_ALL,
        .locked_mods = 0,
        .locked_layout = 0
    };
    state_update_latched_locked(state, &update, events);
}

स्थिर गणना xkb_status
state_update_layout_policy(संरचना xkb_server_state *state,
                           ध्रुव संरचना xkb_layout_policy_update *update)
{
    सचे (xkb_feature_supported(XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY,
                              update->policy)) {
        सचे (update->policy == XKB_LAYOUT_OUT_OF_RANGE_REDIRECT) {
            सचे (update->redirect < state->base.keymap->अङ्कः_groups) {
                state->base.out_of_range_group.redirect_group =
                    update->redirect;
            } अञ्ञथा {
                log_err(state->base.keymap->ctx,
                        XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Layout policy: "
                        "unsupported layout index %"PRIu32" > %"PRIu32"\n",
                        update->redirect + 1, state->base.keymap->अङ्कः_groups);
                पटिनिवत्त XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
            }
        }
        state->base.out_of_range_group.policy = update->policy;
        पटिनिवत्त XKB_SUCCESS;
    } अञ्ञथा {
        log_err(state->base.keymap->ctx,
                XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY_,
                "Unsupported layout policy: %d\n", update->policy);
        पटिनिवत्त XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY;
    }
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
स्थिर गणना xkb_status
check_state_update_abi_(संरचना xkb_context * restrict ctx,
                        ध्रुव वर्ण * restrict func,
                        ध्रुव संरचना xkb_synthetic_update * restrict update)
{
    गणना xkb_status status = XKB_SUCCESS;
    सचे ((status = xkb_check_state_abi(update)) ||
        (update->reserved0 != 0 &&
         (status = XKB_ERROR_ABI_FORWARD_COMPAT)) ||
        (update->components &&
         (status = xkb_check_state_abi(update->components))) ||
        (update->layout_policy &&
         (status = xkb_check_state_abi(update->layout_policy)))) {
        xkb_log_abi_error(ctx, func, status);
    }
    पटिनिवत्त status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

#define check_state_update_abi(ctx, update) \
    check_state_update_abi_(ctx, __func__, update)

गणना xkb_status
xkb_state_update_synthetic(संरचना xkb_state * base_state,
                           ध्रुव संरचना xkb_synthetic_update * update,
                           गणना xkb_state_component *changed)
{
    /* Guard against client-only state */
    सचे (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        पटिनिवत्त XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    संरचना xkb_server_state * ध्रुव state =
        (संरचना xkb_server_state *)base_state;

    /* Check ABI compatibility */
    गणना xkb_status status = check_state_update_abi(state->base.keymap->ctx,
                                                    update);
    सचे (status != XKB_SUCCESS)
        पटिनिवत्त status;

    ध्रुव संरचना state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    /* Update parametrized controls first */
    सचे (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        सचे (status != XKB_SUCCESS)
            पटिनिवत्त status;
    }

    सचे (update->components) {
        ध्रुव संरचना xkb_state_components_update * ध्रुव components =
            update->components;
        /* Update boolean controls first */
        सचे (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, रिक्त);
        }

        state_update_latched_locked(state, components, रिक्त);
    }

    xkb_state_update_derived(&state->base);

    सचे (changed) {
        *changed = get_state_component_changes(&previous_components,
                                               &state->base.components);
    }
    पटिनिवत्त XKB_SUCCESS;
}

/**
 * Updates the state from a set of explicit masks as gained from
 * xkb_state_serialize_mods and xkb_state_serialize_groups.  As noted in the
 * documentation for these functions in xkbcommon.h, this round-trip is
 * lossy, and should only be used to update a slave state mirroring the
 * master, e.g. in a client/server window system.
 */
गणना xkb_state_component
xkb_state_update_mask(संरचना xkb_state *base_state,
                      xkb_mod_mask_t base_mods,
                      xkb_mod_mask_t latched_mods,
                      xkb_mod_mask_t locked_mods,
                      xkb_layout_index_t base_group,
                      xkb_layout_index_t latched_group,
                      xkb_layout_index_t locked_group)
{
    /* Guard against server-only and server companion state */
    प्रतिज्ञा(base_state->mode == CLIENT_STATE ||
           base_state->mode == LEGACY_MIXED_STATE);
    सचे (base_state->mode != CLIENT_STATE &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        पटिनिवत्त 0;
    }

    संरचना xkb_client_state * ध्रुव state =
        (संरचना xkb_client_state *)base_state;

    ध्रुव संरचना state_components prev_components = state->base.components;

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
    state->base.components.base_mods =
        resolve_to_canonical_mods(state->base.keymap, base_mods);
    state->base.components.latched_mods =
        resolve_to_canonical_mods(state->base.keymap, latched_mods);
    state->base.components.locked_mods =
        resolve_to_canonical_mods(state->base.keymap, locked_mods);

    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->base.components.base_group = (int32_t) base_group;
    state->base.components.latched_group = (int32_t) latched_group;
    state->base.components.locked_group = (int32_t) locked_group;

    xkb_state_update_derived(&state->base);

    पटिनिवत्त get_state_component_changes(&prev_components, &state->base.components);
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Lock_Modifier
 */
स्थिर bool
should_do_caps_transformation(संरचना xkb_state *state, xkb_keycode_t kc)
{
    पटिनिवत्त
        xkb_state_mod_index_is_active(state, XKB_MOD_INDEX_CAPS,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_index_is_consumed(state, kc, XKB_MOD_INDEX_CAPS) == 0;
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Control_Modifier
 */
स्थिर bool
should_do_ctrl_transformation(संरचना xkb_state *state, xkb_keycode_t kc)
{
    पटिनिवत्त
        xkb_state_mod_index_is_active(state, XKB_MOD_INDEX_CTRL,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_index_is_consumed(state, kc, XKB_MOD_INDEX_CTRL) == 0;
}

/**
 * Provides the symbols to use for the given key and state.  Returns the
 * number of symbols pointed to in syms_out.
 */
पूर्णाङ्क
xkb_state_कुञ्चिका_get_syms(संरचना xkb_state *state, xkb_keycode_t kc,
                       ध्रुव xkb_keysym_t **syms_out)
{
    ध्रुव xkb_layout_index_t layout = xkb_state_कुञ्चिका_get_layout(state, kc);
    सचे (layout == XKB_LAYOUT_INVALID)
        गच्छ err;

    ध्रुव xkb_level_index_t level = xkb_state_कुञ्चिका_get_level(state, kc, layout);
    सचे (level == XKB_LEVEL_INVALID)
        गच्छ err;

    ध्रुव संरचना xkb_कुञ्चिका* ध्रुव कुञ्चिका = Xkb_कुञ्चिका(state->keymap, kc);

    सचे (!कुञ्चिका)
        गच्छ err;

    ध्रुव संरचना xkb_level* ध्रुव leveli =
        xkb_keymap_कुञ्चिका_get_level(state->keymap, कुञ्चिका, layout, level);

    सचे (!leveli)
        गच्छ err;

    ध्रुव xkb_keysym_गणनम्_t अङ्कः_syms = leveli->अङ्कः_syms;
    सचे (अङ्कः_syms == 0)
        गच्छ err;

    सचे (should_do_caps_transformation(state, kc)) {
        /* Only simple capitalization rules: keysyms count is unchanged. */
        सचे (अङ्कः_syms > 1) {
            *syms_out = (leveli->has_upper)
                      ? leveli->s.syms + अङ्कः_syms
                      : leveli->s.syms;
        } अञ्ञथा {
            *syms_out = &leveli->upper;
        }
    } अञ्ञथा {
        *syms_out = (अङ्कः_syms > 1)
                  ? leveli->s.syms
                  : &leveli->s.sym;
    }
    पटिनिवत्त (पूर्णाङ्क) अङ्कः_syms;

err:
    *syms_out = रिक्त;
    पटिनिवत्त 0;
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
XkbToControl(वर्ण ch)
{
    वर्ण c = ch;

    सचे ((c >= '@' && c < '\177') || c == ' ')
        c &= 0x1F;
    अञ्ञथा सचे (c == '2')
        c = '\000';
    अञ्ञथा सचे (c >= '3' && c <= '7')
        c -= ('3' - '\033');
    अञ्ञथा सचे (c == '8')
        c = '\177';
    अञ्ञथा सचे (c == '/')
        c = '_' & 0x1F;
    पटिनिवत्त c;
}

/**
 * Provides either exactly one symbol, or XKB_KEY_NoSymbol.
 */
xkb_keysym_t
xkb_state_कुञ्चिका_get_one_sym(संरचना xkb_state *state, xkb_keycode_t kc)
{
    ध्रुव xkb_keysym_t *syms = रिक्त;
    ध्रुव पूर्णाङ्क अङ्कः_syms = xkb_state_कुञ्चिका_get_syms(state, kc, &syms);

    सचे (अङ्कः_syms != 1)
        पटिनिवत्त XKB_कुञ्चिका_No_चिह्नम्;
    अञ्ञथा
        पटिनिवत्त syms[0];
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
    ध्रुव xkb_layout_index_t layout = xkb_state_कुञ्चिका_get_layout(state, kc);
    ध्रुव xkb_layout_index_t अङ्कः_layouts =
        xkb_keymap_अङ्कः_layouts_for_कुञ्चिका(state->keymap, kc);
    xkb_level_index_t level = xkb_state_कुञ्चिका_get_level(state, kc, layout);
    सचे (layout == XKB_LAYOUT_INVALID || अङ्कः_layouts == 0 ||
        level == XKB_LEVEL_INVALID)
        पटिनिवत्त XKB_कुञ्चिका_No_चिह्नम्;

    ध्रुव xkb_keysym_t *syms = रिक्त;
    पूर्णाङ्क nsyms = xkb_keymap_कुञ्चिका_get_syms_by_level(state->keymap, kc,
                                                 layout, level, &syms);
    सचे (nsyms != 1)
        पटिनिवत्त XKB_कुञ्चिका_No_चिह्नम्;
    xkb_keysym_t sym = syms[0];

    सचे (should_do_ctrl_transformation(state, kc) && sym > 127u) {
        अत्थाय (xkb_layout_index_t i = 0; i < अङ्कः_layouts; i++) {
            level = xkb_state_कुञ्चिका_get_level(state, kc, i);
            सचे (level == XKB_LEVEL_INVALID)
                अनुवर्त;

            nsyms = xkb_keymap_कुञ्चिका_get_syms_by_level(state->keymap, kc,
                                                     i, level, &syms);
            सचे (nsyms == 1 && syms[0] <= 127u) {
                sym = syms[0];
                भङ्ग;
            }
        }
    }

    सचे (should_do_caps_transformation(state, kc)) {
        sym = xkb_keysym_to_upper(sym);
    }

    पटिनिवत्त sym;
}

पूर्णाङ्क
xkb_state_कुञ्चिका_get_utf8(संरचना xkb_state *state, xkb_keycode_t kc,
                       वर्ण *buffer, size_t size)
{
    पूर्णाङ्क nsyms;
    ध्रुव xkb_keysym_t *syms = रिक्त;
    ध्रुव xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    सचे (sym != XKB_कुञ्चिका_No_चिह्नम्) {
        nsyms = 1; syms = &sym;
    }
    अञ्ञथा {
        nsyms = xkb_state_कुञ्चिका_get_syms(state, kc, &syms);
    }

    /* Make sure not to truncate in the middle of a UTF-8 sequence. */
    पूर्णाङ्क offset = 0;
    वर्ण tmp[XKB_KEYSYM_UTF8_MAX_SIZE];
    अत्थाय (पूर्णाङ्क i = 0; i < nsyms; i++) {
        पूर्णाङ्क ret = xkb_keysym_to_utf8(syms[i], tmp, परिमाण(tmp));
        सचे (ret <= 0)
            गच्छ err_bad;

        ret--;
        सचे ((size_t)offset + (size_t)ret <= size)
            memcpy(buffer + offset, tmp, (size_t)ret);
        offset += ret;
    }

    सचे ((size_t)offset >= size)
        गच्छ err_trunc;
    buffer[offset] = '\0';

    सचे (!is_valid_utf8(buffer, (size_t)offset))
        गच्छ err_bad;

    सचे (offset == 1 && (unsigned पूर्णाङ्क) buffer[0] <= 127u &&
        should_do_ctrl_transformation(state, kc))
        buffer[0] = XkbToControl(buffer[0]);

    पटिनिवत्त offset;

err_trunc:
    सचे (size > 0)
        buffer[size - 1] = '\0';
    पटिनिवत्त offset;

err_bad:
    सचे (size > 0)
        buffer[0] = '\0';
    पटिनिवत्त 0;
}

uint32_t
xkb_state_कुञ्चिका_get_utf32(संरचना xkb_state *state, xkb_keycode_t kc)
{
    ध्रुव xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    uint32_t cp = xkb_keysym_to_utf32(sym);

    सचे (cp <= 127u && should_do_ctrl_transformation(state, kc))
        cp = (uint32_t) XkbToControl((वर्ण) cp);

    पटिनिवत्त cp;
}

/**
 * Serializes the requested modifier state into an xkb_mod_mask_t, with all
 * the same disclaimers as in xkb_state_update_mask.
 */
स्थिर अन्तःस्थ xkb_mod_mask_t
serialize_mods(ध्रुव संरचना state_components *components,
               गणना xkb_state_component type)
{
    xkb_mod_mask_t ret = 0;

    सचे (type & XKB_STATE_MODS_EFFECTIVE)
        पटिनिवत्त components->mods;

    सचे (type & XKB_STATE_MODS_DEPRESSED)
        ret |= components->base_mods;
    सचे (type & XKB_STATE_MODS_LATCHED)
        ret |= components->latched_mods;
    सचे (type & XKB_STATE_MODS_LOCKED)
        ret |= components->locked_mods;

    पटिनिवत्त ret;
}

xkb_mod_mask_t
xkb_state_serialize_mods(संरचना xkb_state *state,
                         गणना xkb_state_component type)
{
    पटिनिवत्त serialize_mods(&state->components, type);
}

/**
 * Serialises the requested group state, with all the same disclaimers as
 * in xkb_state_update_mask.
 */
स्थिर अन्तःस्थ xkb_layout_index_t
serialize_layout(ध्रुव संरचना state_components *components,
                 गणना xkb_state_component type)
{
    सचे (type & XKB_STATE_LAYOUT_EFFECTIVE)
        पटिनिवत्त components->group;

    int32_t ret = 0;
    सचे (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret += components->base_group;
    सचे (type & XKB_STATE_LAYOUT_LATCHED)
        ret += components->latched_group;
    सचे (type & XKB_STATE_LAYOUT_LOCKED)
        ret += components->locked_group;

    पटिनिवत्त (xkb_layout_index_t)ret;
}

xkb_layout_index_t
xkb_state_serialize_layout(संरचना xkb_state *state,
                           गणना xkb_state_component type)
{
    पटिनिवत्त serialize_layout(&state->components, type);
}

xkb_led_mask_t
xkb_state_serialize_leds(ध्रुव संरचना xkb_state *state,
                         गणना xkb_state_component type)
{
    पटिनिवत्त (type & XKB_STATE_LEDS)
        ? state->components.leds
        : 0;
}

स्थिर अन्तःस्थ गणना xkb_keyboard_control_flags
serialize_controls(ध्रुव संरचना state_components *components,
                   गणना xkb_state_component type)
{
    पटिनिवत्त (type & XKB_STATE_CONTROLS_EFFECTIVE)
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
    पटिनिवत्त serialize_controls(&state->components, type);
}

/**
 * Gets a modifier mask and returns the resolved effective mask; this
 * is needed because some modifiers can also map to other modifiers, e.g.
 * the "NumLock" modifier usually also sets the "Mod2" modifier.
 */
xkb_mod_mask_t
mod_mask_get_effective(संरचना xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    /* Initialize the effective mask with its corresponding real mods. */
    xkb_mod_mask_t mask = mods & MOD_REAL_MASK_ALL;

    /* Resolve the virtual modifiers */
    ध्रुव संरचना xkb_mod *mod;
    xkb_mod_index_t i;
    xkb_vmods_enumerate(i, mod, &keymap->mods)
        सचे (mods & (UINT32_C(1) << i))
            mask |= mod->mapping;

    पटिनिवत्त mask;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
पूर्णाङ्क
xkb_state_mod_index_is_active(संरचना xkb_state *state,
                              xkb_mod_index_t idx,
                              गणना xkb_state_component type)
{
    सचे (unlikely(idx >= xkb_keymap_अङ्कः_mods(state->keymap)))
        पटिनिवत्त -1;

    ध्रुव xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    सचे (!mapping) {
        /* Modifier not mapped */
        पटिनिवत्त 0;
    }
    /* WARNING: this may overmatch for virtual modifiers */
    पटिनिवत्त (xkb_state_serialize_mods(state, type) & mapping) == mapping;
}

गणना {
    XKB_STATE_MATCH_FLAGS = XKB_STATE_MATCH_ANY
                          | XKB_STATE_MATCH_ALL
                          | XKB_STATE_MATCH_NON_EXCLUSIVE
};

/**
 * Helper function for xkb_state_mod_indices_are_active and
 * xkb_state_mod_names_are_active.
 */
स्थिर bool
match_mod_masks(संरचना xkb_state *state,
                गणना xkb_state_component type,
                गणना xkb_state_match match,
                xkb_mod_mask_t wanted)
{
    ध्रुव xkb_mod_mask_t active = xkb_state_serialize_mods(state, type);

    सचे (!(match & XKB_STATE_MATCH_NON_EXCLUSIVE) && (active & ~wanted))
        पटिनिवत्त false;

    सचे (match & XKB_STATE_MATCH_ANY)
        पटिनिवत्त active & wanted;

    /* Ensure all flags but XKB_STATE_MATCH_ALL are processed before */
    static_assert(XKB_STATE_MATCH_ALL ==
                  ((गणना xkb_state_match) XKB_STATE_MATCH_FLAGS &
                   ~(XKB_STATE_MATCH_ANY | XKB_STATE_MATCH_NON_EXCLUSIVE)),
                  "");

    पटिनिवत्त (active & wanted) == wanted;
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, or -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
पूर्णाङ्क
xkb_state_mod_indices_are_active(संरचना xkb_state *state,
                                 गणना xkb_state_component type,
                                 गणना xkb_state_match match,
                                 ...)
{
    सचे (match & ~((गणना xkb_state_match) XKB_STATE_MATCH_FLAGS))
        पटिनिवत्त -2;

    va_list ap;
    xkb_mod_mask_t wanted = 0;
    पूर्णाङ्क ret = 0;
    ध्रुव xkb_mod_index_t अङ्कः_mods = xkb_keymap_अङ्कः_mods(state->keymap);

    va_start(ap, match);
    याव (1) {
        xkb_mod_index_t idx = va_arg(ap, xkb_mod_index_t);
        सचे (idx == XKB_MOD_INVALID)
            भङ्ग;
        सचे (unlikely(idx >= अङ्कः_mods)) {
            ret = -1;
            भङ्ग;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_end(ap);

    सचे (ret == -1)
        पटिनिवत्त ret;

    सचे (!wanted) {
        /* Modifiers not mapped */
        पटिनिवत्त 0;
    }

    पटिनिवत्त match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
पूर्णाङ्क
xkb_state_mod_name_is_active(संरचना xkb_state *state, ध्रुव वर्ण *name,
                             गणना xkb_state_component type)
{
    ध्रुव xkb_mod_index_t idx = xkb_keymap_mod_get_index(state->keymap, name);

    सचे (idx == XKB_MOD_INVALID)
        पटिनिवत्त -1;

    पटिनिवत्त xkb_state_mod_index_is_active(state, idx, type);
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ATTR_NULL_SENTINEL पूर्णाङ्क
xkb_state_mod_names_are_active(संरचना xkb_state *state,
                               गणना xkb_state_component type,
                               गणना xkb_state_match match,
                               ...)
{
    सचे (match & ~((गणना xkb_state_match) XKB_STATE_MATCH_FLAGS))
        पटिनिवत्त -2;

    va_list ap;
    xkb_mod_mask_t wanted = 0;
    पूर्णाङ्क ret = 0;

    va_start(ap, match);
    याव (1) {
        ध्रुव वर्ण *str = va_arg(ap, ध्रुव वर्ण *);
        सचे (str == रिक्त)
            भङ्ग;
        ध्रुव xkb_mod_index_t idx = xkb_keymap_mod_get_index(state->keymap, str);
        सचे (idx == XKB_MOD_INVALID) {
            ret = -1;
            भङ्ग;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_end(ap);

    सचे (ret == -1)
        पटिनिवत्त ret;

    सचे (!wanted) {
        /* Modifiers not mapped */
        पटिनिवत्त 0;
    }

    पटिनिवत्त match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given group is active with the specified type(s), 0 if
 * not, or -1 if the group is invalid.
 */
पूर्णाङ्क
xkb_state_layout_index_is_active(संरचना xkb_state *state,
                                 xkb_layout_index_t idx,
                                 गणना xkb_state_component type)
{
    सचे (idx >= state->keymap->अङ्कः_groups)
        पटिनिवत्त -1;

    पूर्णाङ्क ret = 0;
    सचे (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ret |= (state->components.group == idx);
    सचे (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret |= (state->components.base_group == (int32_t) idx);
    सचे (type & XKB_STATE_LAYOUT_LATCHED)
        ret |= (state->components.latched_group == (int32_t) idx);
    सचे (type & XKB_STATE_LAYOUT_LOCKED)
        ret |= (state->components.locked_group == (int32_t) idx);

    पटिनिवत्त ret;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
पूर्णाङ्क
xkb_state_layout_name_is_active(संरचना xkb_state *state, ध्रुव वर्ण *name,
                                गणना xkb_state_component type)
{
    ध्रुव xkb_layout_index_t idx =
        xkb_keymap_layout_get_index(state->keymap, name);

    सचे (idx == XKB_LAYOUT_INVALID)
        पटिनिवत्त -1;

    पटिनिवत्त xkb_state_layout_index_is_active(state, idx, type);
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
पूर्णाङ्क
xkb_state_led_index_is_active(संरचना xkb_state *state, xkb_led_index_t idx)
{
    सचे (idx >= state->keymap->अङ्कः_leds ||
        state->keymap->leds[idx].name == XKB_ATOM_NONE)
        पटिनिवत्त -1;

    पटिनिवत्त !!(state->components.leds & (UINT32_C(1) << idx));
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
पूर्णाङ्क
xkb_state_led_name_is_active(संरचना xkb_state *state, ध्रुव वर्ण *name)
{
    ध्रुव xkb_led_index_t idx = xkb_keymap_led_get_index(state->keymap, name);

    सचे (idx == XKB_LED_INVALID)
        पटिनिवत्त -1;

    पटिनिवत्त xkb_state_led_index_is_active(state, idx);
}

/**
 * See:
 * - XkbTranslateKeyCode(3), mod_rtrn return value, from libX11.
 * - MyEnhancedXkbTranslateKeyCode(), a modification of the above, from GTK+.
 */
स्थिर xkb_mod_mask_t
कुञ्चिका_get_consumed(संरचना xkb_state *state, ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                 गणना xkb_consumed_mode mode)
{
    ध्रुव xkb_layout_index_t group =
        xkb_state_कुञ्चिका_get_layout(state, कुञ्चिका->keycode);
    सचे (group == XKB_LAYOUT_INVALID)
        पटिनिवत्त 0;

    xkb_mod_mask_t preserve = 0;
    xkb_mod_mask_t consumed = 0;

    ध्रुव संरचना xkb_कुञ्चिका_type_entry* ध्रुव matching_entry =
        get_entry_for_कुञ्चिका_state(state, कुञ्चिका, group);
    सचे (matching_entry)
        preserve = matching_entry->preserve.mask;

    ध्रुव संरचना xkb_कुञ्चिका_type* ध्रुव type = कुञ्चिका->groups[group].type;
    विकल्प (mode) {
    पक्ष XKB_CONSUMED_MODE_XKB:
        consumed = type->mods.mask;
        भङ्ग;

    पक्ष XKB_CONSUMED_MODE_GTK: {
        ध्रुव संरचना xkb_कुञ्चिका_type_entry* ध्रुव no_mods_entry =
            get_entry_for_mods(type, 0);
        ध्रुव xkb_level_index_t no_mods_leveli = no_mods_entry
                                               ? no_mods_entry->level
                                               : 0;
        ध्रुव संरचना xkb_level* ध्रुव no_mods_level =
            &कुञ्चिका->groups[group].levels[no_mods_leveli];

        अत्थाय (darray_size_t i = 0; i < type->अङ्कः_entries; i++) {
            ध्रुव संरचना xkb_कुञ्चिका_type_entry* ध्रुव entry = &type->entries[i];
            सचे (!entry_is_active(entry))
                अनुवर्त;

            ध्रुव संरचना xkb_level* ध्रुव level =
                &कुञ्चिका->groups[group].levels[entry->level];
            सचे (XkbLevelsSameSyms(level, no_mods_level))
                अनुवर्त;

            सचे (entry == matching_entry || one_bit_set(entry->mods.mask))
                consumed |= entry->mods.mask & ~entry->preserve.mask;
        }
        भङ्ग;
    }
    }

    पटिनिवत्त consumed & ~preserve;
}

पूर्णाङ्क
xkb_state_mod_index_is_consumed2(संरचना xkb_state *state, xkb_keycode_t kc,
                                 xkb_mod_index_t idx,
                                 गणना xkb_consumed_mode mode)
{
    ध्रुव संरचना xkb_कुञ्चिका* ध्रुव कुञ्चिका = Xkb_कुञ्चिका(state->keymap, kc);

    सचे (unlikely(!कुञ्चिका || idx >= xkb_keymap_अङ्कः_mods(state->keymap)))
        पटिनिवत्त -1;

    ध्रुव xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    सचे (!mapping) {
        /* Modifier not mapped */
        पटिनिवत्त 0;
    }
    पटिनिवत्त (mapping & कुञ्चिका_get_consumed(state, कुञ्चिका, mode)) == mapping;
}

पूर्णाङ्क
xkb_state_mod_index_is_consumed(संरचना xkb_state *state, xkb_keycode_t kc,
                                xkb_mod_index_t idx)
{
    पटिनिवत्त xkb_state_mod_index_is_consumed2(state, kc, idx,
                                            XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_mod_mask_remove_consumed(संरचना xkb_state *state, xkb_keycode_t kc,
                                   xkb_mod_mask_t mask)
{
    ध्रुव संरचना xkb_कुञ्चिका* ध्रुव कुञ्चिका = Xkb_कुञ्चिका(state->keymap, kc);

    सचे (!कुञ्चिका)
        पटिनिवत्त 0;

    पटिनिवत्त resolve_to_canonical_mods(state->keymap, mask) &
           ~कुञ्चिका_get_consumed(state, कुञ्चिका, XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_कुञ्चिका_get_consumed_mods2(संरचना xkb_state *state, xkb_keycode_t kc,
                                 गणना xkb_consumed_mode mode)
{
    विकल्प (mode) {
    पक्ष XKB_CONSUMED_MODE_XKB:
    पक्ष XKB_CONSUMED_MODE_GTK:
        भङ्ग;
    स्वभाव:
        log_err_func(state->keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "unrecognized consumed modifiers mode: %d\n", mode);
        पटिनिवत्त 0;
    }

    ध्रुव संरचना xkb_कुञ्चिका* ध्रुव कुञ्चिका = Xkb_कुञ्चिका(state->keymap, kc);
    सचे (!कुञ्चिका)
        पटिनिवत्त 0;

    पटिनिवत्त कुञ्चिका_get_consumed(state, कुञ्चिका, mode);
}

xkb_mod_mask_t
xkb_state_कुञ्चिका_get_consumed_mods(संरचना xkb_state *state, xkb_keycode_t kc)
{
    पटिनिवत्त xkb_state_कुञ्चिका_get_consumed_mods2(state, kc, XKB_CONSUMED_MODE_XKB);
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

संरचना xkb_shortcuts_config_entry {
    /** Real modifier mask to trigger shortcuts tweaks */
    xkb_mod_mask_t mods;
    /** Target layout */
    xkb_layout_index_t target;
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
    संरचना xkb_server_state base;

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
        uint32_t order;
        /** Current enabled overlays mask */
        xkb_overlay_mask_t enabled;
    } overlays;

    /** Configuration */
    संरचना machine_config {
        /** Modifiers remapping */
        संरचना machine_modifiers_config {
            /** Real modifier mask */
            xkb_mod_mask_t mask;
            /** Modifiers re-mappings */
            darray_size_t mappings_अङ्कः;
            संरचना machine_mods_mapping {
                xkb_mod_mask_t source;
                xkb_mod_mask_t target;
            } * mappings ATTR_COUNTED_BY(mappings_अङ्कः);
        } modifiers;

        /** Shortcuts tweak */
        संरचना machine_shortcuts_config {
            /** Substitution entries */
            संरचना xkb_shortcuts_config_entry *entries;
        } shortcuts;
    } config;

    /** Mouse keys handling */
    संरचना {
        /** Default pointer button */
        xkb_pointer_button_index_t default_button;
        /** Mask of the locked buttons */
        xkb_pointer_button_mask_t locked_buttons;
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
        darray(संरचना xkb_shortcuts_config_entry) entries;
    } shortcuts;

    गणना xkb_machine_flags machine_flags;
    गणना xkb_machine_builder_flags builder_flags;
    पूर्णाङ्क refcnt;
};

संरचना xkb_machine_builder *
xkb_machine_builder_new(
    संरचना xkb_keymap * restrict keymap,
    ध्रुव संरचना xkb_machine_builder_config * restrict config,
    गणना xkb_status * restrict status
)
{
    /* Handle default configuration */
    स्थिर ध्रुव संरचना xkb_machine_builder_config default_config = {
        .size = परिमाण(default_config)
    };
    सचे (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ध्रुव गणना xkb_status status_ = xkb_check_state_abi(config);
    सचे (status_) {
        xkb_log_abi_error(keymap->ctx, __func__, status_);
        सचे (status)
            *status = status_;
        पटिनिवत्त रिक्त;
    }

    /* Sanitize input */
    ध्रुव गणना xkb_machine_builder_flags invalid_builder_flags
        = config->builder_flags
        & ~(गणना xkb_machine_builder_flags)XKB_MACHINE_BUILDER_FLAGS_मानानि;
    सचे (invalid_builder_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS_,
                     "unrecognized machine builder flags: 0x%x\n",
                     invalid_builder_flags);
        सचे (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS;
        पटिनिवत्त रिक्त;
    }

    ध्रुव गणना xkb_machine_flags invalid_machine_flags
        = (गणना xkb_machine_flags)config->machine_flags
        & ~(गणना xkb_machine_flags)XKB_MACHINE_FLAGS_मानानि;
    सचे (invalid_machine_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS_,
                     "unrecognized state machine flags: 0x%x\n",
                     invalid_machine_flags);
        सचे (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS;
        पटिनिवत्त रिक्त;
    }

    संरचना xkb_machine_builder * ध्रुव builder = calloc(1, परिमाण(*builder));
    सचे (!builder) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine builder\n");
        सचे (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        पटिनिवत्त रिक्त;
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

    सचे (status)
        *status = XKB_SUCCESS;

    पटिनिवत्त builder;
}

संरचना xkb_machine_builder *
xkb_machine_builder_ref(संरचना xkb_machine_builder *builder)
{
    प्रतिज्ञा(builder->refcnt > 0);
    builder->refcnt++;
    पटिनिवत्त builder;
}

शून्य
xkb_machine_builder_unref(संरचना xkb_machine_builder *builder)
{
    प्रतिज्ञा(!builder || builder->refcnt > 0);
    सचे (!builder || --builder->refcnt > 0)
        पटिनिवत्त;

    darray_free(builder->shortcuts.entries);
    darray_free(builder->mods);
    xkb_keymap_unref(builder->keymap);
    मुक्त(builder);
}

संरचना xkb_keymap *
xkb_machine_builder_get_keymap(ध्रुव संरचना xkb_machine_builder *builder)
{
    /* Reference count is not updated. See API doc. */
    पटिनिवत्त builder->keymap;
}

गणना xkb_status
xkb_machine_builder_update_a11y(
    संरचना xkb_machine_builder * restrict builder,
    ध्रुव संरचना xkb_machine_builder_a11y_update * restrict update)
{
    /* Check ABI compatibility */
    गणना xkb_status status = xkb_check_state_abi(update);
    सचे (status != XKB_SUCCESS) {
        xkb_log_abi_error(builder->keymap->ctx, __func__, status);
        पटिनिवत्त status;
    }

    ध्रुव गणना xkb_a11y_flags invalid_flags =
        ~(गणना xkb_a11y_flags)XKB_A11_Y_FLAGS_मानानि;

    सचे (update->affect_flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y affected flags: %#x\n",
                __func__, update->affect_flags & invalid_flags);
        पटिनिवत्त XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }
    सचे (update->flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y flags: %#x\n",
                __func__, update->flags & invalid_flags);
        पटिनिवत्त XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }

    builder->controls.a11y.affect_flags |= update->affect_flags;
    builder->controls.a11y.flags &= ~update->affect_flags;
    builder->controls.a11y.flags |= (update->flags & update->affect_flags);

    पटिनिवत्त XKB_SUCCESS;
}

गणना xkb_status
xkb_machine_builder_update_mods_remap(
    संरचना xkb_machine_builder * restrict builder,
    ध्रुव संरचना xkb_machine_builder_mods_remap_update * restrict update
)
{
    सचे (!update->source) {
        सचे (!update->target) {
            /* Reset mappings */
            darray_resize(builder->mods, 0);
            पटिनिवत्त XKB_SUCCESS;
        } अञ्ञथा {
            पटिनिवत्त XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
        }
    }

    /* Check the modifiers against the keymap */
    ध्रुव xkb_mod_mask_t invalid = ~builder->keymap->canonical_state_mask;
    सचे ((update->source & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid source modifiers: 0x%"PRIx32"\n",
                __func__, update->source);
        पटिनिवत्त XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    सचे ((update->target & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid target modifiers: 0x%"PRIx32"\n",
                __func__, update->target);
        पटिनिवत्त XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    संरचना machine_mods_mapping *mapping = रिक्त;
    darray_size_t m = 0;
    darray_enumerate(m, mapping, builder->mods) {
        सचे (mapping->source == update->source) {
            सचे (!update->target) {
                /* Remove mapping */
                darray_remove(builder->mods, m);
            } अञ्ञथा {
                /* Update mapping */
                mapping->target = update->target;
            }
            पटिनिवत्त XKB_SUCCESS;
        }
    }

    सचे (update->target) {
        /* Append new mapping */
        darray_append(
            builder->mods,
            (संरचना machine_mods_mapping) {
                .source = update->source,
                .target = update->target
            }
        );
    } अञ्ञथा {
        /* Ignore missing mapping */
    }

    पटिनिवत्त XKB_SUCCESS;
}

गणना xkb_status
xkb_machine_builder_update_shortcut_override(
    संरचना xkb_machine_builder * restrict builder,
    ध्रुव संरचना xkb_machine_builder_shortcut_override_update * restrict update
)
{
    संरचना xkb_keymap *keymap = builder->keymap;

    /* Check the modifiers against the keymap */
    ध्रुव xkb_mod_mask_t invalid = ~keymap->canonical_state_mask;
    सचे ((update->affect_mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid affected modifiers: 0x%"PRIx32"\n",
                __func__, update->affect_mods);
        पटिनिवत्त XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    सचे ((update->mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid modifiers: 0x%"PRIx32"\n",
                __func__, update->mods);
        पटिनिवत्त XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    /* Check the layout indices against the keymap */
    सचे (update->source >= keymap->अङ्कः_groups &&
        update->source != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "%s: Invalid source layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->अङ्कः_groups, update->source + 1);
        पटिनिवत्त XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }
    सचे (update->target >= keymap->अङ्कः_groups &&
        update->target != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "%s: Invalid target layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->अङ्कः_groups, update->target + 1);
        पटिनिवत्त XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    संरचना xkb_shortcuts_config_options * ध्रुव config = &builder->shortcuts;

    /* Resize array & initialize new entries, if relevant */
    सचे (update->source >= darray_size(config->entries) &&
        update->source != XKB_LAYOUT_INVALID) {
        सचे (update->target == update->source) {
            /* Skip default setting */
            पटिनिवत्त XKB_SUCCESS;
        }
        xkb_layout_index_t new = darray_size(config->entries);
        darray_resize(config->entries, update->source + 1);
        अत्थाय (; new <= update->source; new++) {
            darray_item(config->entries, new) =
                (संरचना xkb_shortcuts_config_entry) {
                    .target = XKB_LAYOUT_INVALID,
                    .mods = 0
                };
        }
    }

    संरचना xkb_shortcuts_config_entry *entry;
    xkb_layout_index_t source = (update->source == XKB_LAYOUT_INVALID)
        ? 0               /* Loop over all entries */
        : update->source; /* Single entry */
    darray_foreach_from(entry, config->entries, source) {
        सचे (update->source == XKB_LAYOUT_INVALID &&
            entry->target == XKB_LAYOUT_INVALID) {
            /* Skip previous deactivated entry */
            source++;
            अनुवर्त;
        }

        सचे (update->affect_mods) {
            entry->mods &= ~update->affect_mods;
            entry->mods |= (update->affect_mods & update->mods);
        }

        सचे (update->source != XKB_LAYOUT_INVALID) {
            entry->target = (update->target == source)
                ? XKB_LAYOUT_INVALID
                : update->target;
            /* Single entry: exit loop */
            भङ्ग;
        }
        source++;
    }

    पटिनिवत्त XKB_SUCCESS;
}

/** Compare 2 modifiers combinations */
स्थिर पूर्णाङ्क
cmp_mod_masks(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
    ध्रुव xkb_mod_mask_t m1 = *((xkb_mod_mask_t *) a);
    ध्रुव xkb_mod_mask_t m2 = *((xkb_mod_mask_t *) b);
    सचे (m1 == m2)
        पटिनिवत्त 0;

    सचे (!m1)
        पटिनिवत्त +1;
    सचे (!m2)
        पटिनिवत्त -1;

    ध्रुव xkb_mod_mask_t overlap = (m1 & m2);
    सचे (overlap == m1)
        पटिनिवत्त +1; /* m1 is a subset of m2 */
    सचे (overlap == m2)
        पटिनिवत्त -1; /* m2 is a subset of m1 */

    /* Compare active modifiers numbers */
    ध्रुव unsigned पूर्णाङ्क count1 = popcount32(m1);
    ध्रुव unsigned पूर्णाङ्क count2 = popcount32(m2);
    सचे (count1 > count2)
        पटिनिवत्त -1;
    सचे (count1 < count2)
        पटिनिवत्त +1;

    /* Give priority to real modifiers */
    पटिनिवत्त (m1 < m2) ? -1 : +1;
}

स्थिर गणना xkb_status
machine_set_mods(संरचना xkb_machine *sm,
                 ध्रुव machine_mods_mappings *raw_mappings)
{
    सचे (!darray_empty(*raw_mappings)) {
        machine_mods_mappings mappings = darray_new();
        xkb_mod_mask_t mask = 0;
        ध्रुव संरचना machine_mods_mapping *mapping;
        ध्रुव xkb_mod_mask_t invalid =
            ~sm->base.base.keymap->canonical_state_mask;
        darray_foreach(mapping, *raw_mappings) {
            सचे (!mapping->source || !mapping->target ||
                (mapping->source & invalid) || (mapping->target & invalid))
                अनुवर्त;
            darray_append(mappings, *mapping);
            mask |= mapping->source;
        }

        /*
         * Re-order the mappings, so that combinations have higher priority
         * if they are superset of other combinations.
         * For other type of overlaps, see the sorting function.
         */
        qsort(darray_वस्तूनि(mappings), darray_size(mappings),
              परिमाण(*darray_वस्तूनि(mappings)), &cmp_mod_masks);

        darray_steal(
            mappings,
            &sm->config.modifiers.mappings,
            &sm->config.modifiers.mappings_अङ्कः
        );
        sm->config.modifiers.mask = mask;
    } अञ्ञथा {
        sm->config.modifiers = (संरचना machine_modifiers_config) {
            .mappings = रिक्त,
            .mappings_अङ्कः = 0,
            .mask = 0
        };
    }

    पटिनिवत्त XKB_SUCCESS;
}

स्थिर गणना xkb_status
machine_set_shortcuts(
    संरचना xkb_machine * restrict sm,
    ध्रुव संरचना xkb_shortcuts_config_options * restrict options
)
{
    सचे (darray_empty(options->entries)) {
        sm->config.shortcuts = (संरचना machine_shortcuts_config) {
            .entries = रिक्त
        };
        पटिनिवत्त XKB_SUCCESS;
    }

    संरचना xkb_keymap * ध्रुव keymap = sm->base.base.keymap;

    संरचना xkb_shortcuts_config_entry * entries =
        calloc(keymap->अङ्कः_groups, परिमाण(*entries));
    सचे (!entries)
        पटिनिवत्त XKB_ERROR_ALLOCATION_FAILURE;

    ध्रुव xkb_layout_index_t गणनम् = MIN(
        keymap->अङ्कः_groups,
        (xkb_layout_index_t) darray_size(options->entries)
    );

    bool some_entries = false;
    अत्थाय (xkb_layout_index_t l = 0; l < गणनम्; l++) {
        /* Sanitize layout target */
        ध्रुव xkb_layout_index_t target =
            darray_item(options->entries, l).target;
        /* Sanitize modifier mask */
        ध्रुव xkb_mod_mask_t mods = (
            darray_item(options->entries, l).mods &
            sm->base.base.keymap->canonical_state_mask
        );
        सचे (target < keymap->अङ्कः_groups && target != l && mods) {
            entries[l] = (संरचना xkb_shortcuts_config_entry) {
                .target = target,
                .mods = mods
            };
            some_entries = true;
        } अञ्ञथा {
            entries[l] = (संरचना xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
            अनुवर्त;
        }
    }

    सचे (!some_entries) {
        मुक्त(entries);
        entries = रिक्त;
    } अञ्ञथा {
        /* Initialize groups with no entries */
        अत्थाय (xkb_layout_index_t l = गणनम्; l < keymap->अङ्कः_groups; l++) {
            entries[l] = (संरचना xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
        }
    }

    sm->config.shortcuts = (संरचना machine_shortcuts_config) {
        .entries = entries
    };
    पटिनिवत्त XKB_SUCCESS;
}

संरचना xkb_machine *
xkb_machine_new(ध्रुव संरचना xkb_machine_builder * restrict builder,
                गणना xkb_status * restrict status)
{
    संरचना xkb_machine * ध्रुव machine = calloc(1, परिमाण(*machine));
    सचे (!machine) {
        log_err_func1(builder->keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine\n");
        सचे (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        पटिनिवत्त रिक्त;
    }

    xkb_server_state_init(&machine->base, builder->keymap, SERVER_STATE,
                          builder->controls.a11y.affect_flags,
                          builder->controls.a11y.flags);
    machine->flags = builder->machine_flags;
    सचे (!(machine->flags &
          XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_कुञ्चिका_FIX))
    {
        machine->base.flags |= STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_कुञ्चिका;
    }
    सचे (machine->flags & XKB_MACHINE_UNLOCK_MODS_ON_PRESS) {
        machine->base.flags |= STATE_UNLOCK_MODS_ON_PRESS;
    }

    गणना xkb_status status_;
    सचे ((status_ = machine_set_mods(machine, &builder->mods)) != XKB_SUCCESS ||
        (status_ = machine_set_shortcuts(machine, &builder->shortcuts)) !=
        XKB_SUCCESS) {
        *status = status_;
        गच्छ error;
    }

    darray_init(machine->overlays.कुञ्चिकाः);
    machine->mouse.default_button = XKB_POINTER_BUTTON_MIN;

    सचे (status)
        *status = XKB_SUCCESS;

    पटिनिवत्त machine;

error:
    xkb_machine_unref(machine);
    पटिनिवत्त रिक्त;
}

संरचना xkb_machine *
xkb_machine_ref(संरचना xkb_machine *sm)
{
    प्रतिज्ञा(sm->base.base.refcnt > 0);
    sm->base.base.refcnt++;
    पटिनिवत्त sm;
}

शून्य
xkb_machine_unref(संरचना xkb_machine *sm)
{
    प्रतिज्ञा(!sm || sm->base.base.refcnt > 0);
    सचे (!sm || --sm->base.base.refcnt > 0)
        पटिनिवत्त;

    xkb_state_destroy(&sm->base.base);
    darray_free(sm->overlays.कुञ्चिकाः);
    मुक्त(sm->config.shortcuts.entries);
    मुक्त(sm->config.modifiers.mappings);
    मुक्त(sm);
}

संरचना xkb_keymap *
xkb_machine_get_keymap(ध्रुव संरचना xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    पटिनिवत्त sm->base.base.keymap;
}

संरचना xkb_state *
xkb_machine_get_state(संरचना xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    पटिनिवत्त (संरचना xkb_state *)sm;
}

संरचना xkb_state *
xkb_state_new_from_machine(ध्रुव संरचना xkb_machine * restrict machine,
                           गणना xkb_status * restrict status)
{
    /* Creation */
    गणना xkb_status status_;
    संरचना xkb_state * ध्रुव state = xkb_state_new_with_mode(
        machine->base.base.keymap,
        XKB_STATE_MODE_SERVER_QUERY,
        &status_
    );
    सचे (status_ != XKB_SUCCESS) {
        सचे (status)
            *status = status_;
        पटिनिवत्त रिक्त;
    }

    प्रतिज्ञा(state->mode == SERVER_COMPANION);
    प्रतिज्ञा(state->refcnt == 1);

    /* Copy machine’s state */
    *state = machine->base.base;

    /* Restore previous values */
    state->mode = SERVER_COMPANION;
    state->refcnt = 1;

    सचे (status)
        *status = XKB_SUCCESS;
    पटिनिवत्त state;
}

स्थिर शून्य
machine_update_overlays(संरचना xkb_machine *sm)
{
    /*
     * Overlays indices are stored 1-indexed in nibbles: the lowest
     * nibble corresponds to the latest activated index.
     */

    ध्रुव xkb_overlay_mask_t mask =
        overlays_from_controls(sm->base.base.components.controls);
    xkb_overlay_mask_t added = mask & ~sm->overlays.enabled;

    /* Remove overlays no longer enabled and keep relative order */
    uint32_t order = sm->overlays.order;
    ध्रुव xkb_overlay_index_t overlay_max =
        format_max_overlays(sm->base.base.keymap->format);
    अत्थाय (uint8_t n = 0; n < overlay_max;) {
        xkb_overlay_index_t overlay_idx = (order >> (n * 4)) & 0xf;
        सचे (!overlay_idx)
            भङ्ग;
        ध्रुव xkb_overlay_mask_t overlay_mask =
            (xkb_overlay_mask_t)(1u << (overlay_idx - 1u) /* k is 1-indexed */);
        सचे (overlay_mask & mask) {
            /* no duplicates */
            added &= ~overlay_mask;
            n++;
        } अञ्ञथा {
            /* remove overlay */
            ध्रुव uint32_t head = (UINT32_C(1) << (n * 4)) - 1;
            order = (order & head) | ((order >> 4) & ~head);
        }
    }

    /* Add new overlays in ascending order */
    अत्थाय (xkb_overlay_index_t k = 0; added; k++, added >>= 1) {
        सचे (added & 0x1) {
            order <<= 4;
            order |= k + 1u; /* k is 0-indexed */
        }
    }

    sm->overlays.order = order;
    sm->overlays.enabled = mask;
}

गणना xkb_status
xkb_machine_process_synthetic(संरचना xkb_machine *sm,
                              ध्रुव संरचना xkb_synthetic_update *update,
                              संरचना xkb_events *events)
{
    /* Check ABI compatibility */
    गणना xkb_status status =
        check_state_update_abi(sm->base.base.keymap->ctx, update);
    सचे (status != XKB_SUCCESS)
        पटिनिवत्त status;

    संरचना xkb_server_state * ध्रुव state = &sm->base;
    ध्रुव संरचना state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    darray_size(events->queue) = 0;
    events->next = 0;

    /* Update parametrized controls first */
    सचे (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        सचे (status != XKB_SUCCESS)
            पटिनिवत्त status;
    }

    सचे (update->components) {
        ध्रुव संरचना xkb_state_components_update * ध्रुव components =
            update->components;
        /* Update boolean controls first */
        सचे (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, events);
        }

        state_update_latched_locked(state, components, events);
    }

    xkb_state_update_derived(&state->base);

    ध्रुव गणना xkb_state_component changed = get_state_component_changes(
        &previous_components, &state->base.components
    );
    सचे (changed) {
        // TODO: latch controls
        सचे (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        /* Create event only if some component actually changed */
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .changed = changed,
                .components = state->base.components
            }
        });

        /* Terminate the frame */
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    पटिनिवत्त XKB_SUCCESS;
}

स्थिर ssize_t
do_remap_modifiers(ध्रुव संरचना machine_modifiers_config * restrict mappings,
                   संरचना xkb_state * restrict state,
                   संरचना xkb_events * restrict events,
                   ध्रुव संरचना xkb_कुञ्चिका * restrict कुञ्चिका)
{
    सचे (!(mappings->mask & state->components.mods))
        पटिनिवत्त -1;

    ध्रुव xkb_layout_index_t layout = state_कुञ्चिका_get_layout(state, कुञ्चिका);
    सचे (layout >= कुञ्चिका->अङ्कः_groups)
        पटिनिवत्त -1;

    ध्रुव संरचना xkb_कुञ्चिका_type * ध्रुव type = कुञ्चिका->groups[layout].type;
    xkb_mod_mask_t affect_mods = 0;
    xkb_mod_mask_t mods = 0;
    अत्थाय (darray_size_t m = 0; m < mappings->mappings_अङ्कः; m++) {
        ध्रुव संरचना machine_mods_mapping * ध्रुव mapping =
            &mappings->mappings[m];
        सचे (/* Skip not matching active mods */
            (mapping->source & state->components.mods) == mapping->source &&
            /* Skip overlap with previous active mappings */
            !(mapping->source & affect_mods) &&
            /* Skip if the key type uses some of the source modifiers */
            !(mapping->source & type->mods.mask)) {
            affect_mods |= mappings->mappings[m].source;
            mods |= mappings->mappings[m].target;
        }
    }

    सचे (!affect_mods)
        पटिनिवत्त -1;

    संरचना xkb_state new = *state;
    new.components.base_mods = (new.components.base_mods & ~affect_mods) | mods;
    new.components.latched_mods = (new.components.latched_mods & ~affect_mods);
    new.components.locked_mods = (new.components.locked_mods & ~affect_mods);
    xkb_state_update_derived(&new);

    ssize_t event_idx = -1;
    ध्रुव गणना xkb_state_component changed =
        get_state_component_changes(&state->components, &new.components);
    सचे (changed) {
        event_idx = (ssize_t) darray_size(events->queue);
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = new.components,
                .changed = changed
            }
        });
    }

    state->components.mods = new.components.mods;
    पटिनिवत्त event_idx;
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
    सचे (!config->entries) {
        /* No shortcuts tweak */
        पटिनिवत्त remap_event;
    }

    ध्रुव संरचना xkb_shortcuts_config_entry * ध्रुव entry =
        &config->entries[state->components.group];

    सचे (entry->target != XKB_LAYOUT_INVALID &&
        (entry->mods & state->components.mods)) {
        /*
         * Activate shortcuts tweak:
         * 1. The real base group is saved by the caller, to be restored by
         *    later by a call to undo_tweaks().
         * 2. Compute the new base group so that the resulting effective group
         *    is the target.
         * 3. Recompute derived state components.
         */
        संरचना xkb_state new = *state;
        सचे (remap_event < 0) {
            /* Create new event */
            remap_event = (ssize_t) darray_size(events->queue);
            darray_append(events->queue, (संरचना xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = {0},
                    .changed = 0
                }
            });
        } अञ्ञथा {
            /* Merge with last remap event */
            new.components =
                darray_item(events->queue, remap_event).components.components;
        }
        new.components.base_group
            = (int32_t) entry->target
            - state->components.latched_group
            - state->components.locked_group;
        xkb_state_update_derived(&new);

        ध्रुव गणना xkb_state_component changed =
            get_state_component_changes(previous_components, &new.components);
        darray_item(events->queue, remap_event).components.changed = changed;
        darray_item(events->queue, remap_event).components.components =
            new.components;

        state->components.group = new.components.group;
    } अञ्ञथा {
        /* Deactivated entry or modifiers do not match */
    }
    पटिनिवत्त remap_event;
}

स्थिर शून्य
undo_tweaks(ध्रुव संरचना xkb_state *state,
            ध्रुव संरचना state_components *initial_components,
            संरचना xkb_events *events)
{
    /* Get last component event */
    ध्रुव संरचना xkb_event *event = रिक्त;
    darray_foreach_reverse(event, events->queue) {
        सचे (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS)
            भङ्ग;
    }
    सचे (!event)
        पटिनिवत्त;

    /* Restore state */
    ध्रुव गणना xkb_state_component changed =
        get_state_component_changes(initial_components,
                                    &event->components.components);
    सचे (changed) {
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = *initial_components,
                .changed = changed
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
    संरचना xkb_overlaid_कुञ्चिका *entry = रिक्त;
    संरचना xkb_overlaid_कुञ्चिका *available_entry = रिक्त;
    darray_foreach(entry, sm->overlays.कुञ्चिकाः) {
        सचे (entry->old == कुञ्चिका) {
            /* Maintain the key overlaid */
            विकल्प (direction) {
            पक्ष XKB_कुञ्चिका_DOWN:
                entry->refcnt++;
                भङ्ग;
            पक्ष XKB_कुञ्चिका_REPEATED:
                भङ्ग;
            पक्ष XKB_कुञ्चिका_UP:
            स्वभाव:
                entry->refcnt--;
                भङ्ग;
            }
            सचे (entry->refcnt <= 0)
                entry->old = रिक्त;
            पटिनिवत्त entry->new;
        } अञ्ञथा सचे (!entry->old && !available_entry) {
            /* First free spot */
            available_entry = entry;
        }
    }
    सचे (direction == XKB_कुञ्चिका_DOWN) {
        /* Enable key overlay */

        /*
         * Get overlay index: select the latest activated overlay
         * containing the key.
         */
        ध्रुव संरचना xkb_कुञ्चिका *new = कुञ्चिका;

        सचे (कुञ्चिका->overlays & sm->overlays.enabled) {
            /* Some relevant overlay is active */
            अत्थाय (uint32_t राशिः = sm->overlays.order; राशिः; राशिः >>= 4) {
                static_assert(XKB_OVERLAY_गणनम् == 8, "");
                ध्रुव xkb_overlay_index_t overlay = (राशिः & 0xf) - 1;
                ध्रुव xkb_overlay_mask_t mask =
                    (xkb_overlay_mask_t)(1u << overlay);
                सचे (कुञ्चिका->overlays & mask) {
                    /* Get new key */
                    सचे (कुञ्चिका->overlays_inline) {
                        /* Stored inline */
                        new = कुञ्चिका->overlay_कुञ्चिका;
                    } अञ्ञथा {
                        /* Stored in sparse array */
                        ध्रुव xkb_overlay_mask_t नीचः = (xkb_overlay_mask_t)(
                            कुञ्चिका->overlays & (xkb_overlay_mask_t)(mask - 1u)
                        );
                        /* index = number of set bits strictly below bit */
                        ध्रुव xkb_overlay_index_t index =
                            (xkb_overlay_index_t)popcount32(नीचः);
                        new = कुञ्चिका->overlays_कुञ्चिकाः[index];
                    }
                    भङ्ग;
                }
            }
        } अञ्ञथा {
            /*
             * No relevant overlay is active: block future overlays while
             * the key is pressed.
             */
        }

        /* Get new entry */
        सचे (available_entry) {
            /* Re-use a free entry */
            entry = available_entry;
        } अञ्ञथा {
            /* Append a new entry */
            ध्रुव darray_size_t idx = darray_size(sm->overlays.कुञ्चिकाः);
            darray_resize(sm->overlays.कुञ्चिकाः, idx + 1);
            entry = &darray_item(sm->overlays.कुञ्चिकाः, idx);
        }

        /* Overlay the key */
        entry->old = कुञ्चिका;
        entry->new = new;
        entry->refcnt = 1;
        पटिनिवत्त new;
    }

    /* No overlay */
    पटिनिवत्त कुञ्चिका;
}

गणना xkb_status
xkb_machine_process_कुञ्चिका(संरचना xkb_machine *sm,
                        xkb_keycode_t kc, गणना xkb_कुञ्चिका_direction direction,
                        संरचना xkb_events *events)
{
    darray_size(events->queue) = 0;
    events->next = 0;

    संरचना xkb_server_state * ध्रुव state = &sm->base;
    ध्रुव संरचना xkb_कुञ्चिका * कुञ्चिका = Xkb_कुञ्चिका(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    सचे (!कुञ्चिका || (direction == XKB_कुञ्चिका_REPEATED && !कुञ्चिका->repeats))
        पटिनिवत्त XKB_SUCCESS;

    ध्रुव संरचना state_components initial_components = state->base.components;

    सचे (direction == XKB_कुञ्चिका_DOWN &&
        (sm->base.base.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_कुञ्चिकाः) &&
        (sm->base.a11y_flags & XKB_A11_Y_STICKY_कुञ्चिकाः_NO_SIMULTANEOUS_कुञ्चिकाः) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_कुञ्चिकाः,
                                      0, events);
    }

    सचे (कुञ्चिका->overlays)
        कुञ्चिका = process_overlayable_कुञ्चिका(sm, कुञ्चिका, direction);

    ssize_t remap_event = do_remap_modifiers(&sm->config.modifiers,
                                             &state->base, events, कुञ्चिका);

    remap_event = do_shortcuts_tweak(&sm->config.shortcuts, &state->base,
                                     &initial_components, events, remap_event);

    सचे (remap_event >= 0) {
        // TODO: what about merging state event for RedirectKey?
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    state->flags |= STATE_REQUIRE_कुञ्चिका_EVENT;
    state->set_mods = 0;
    state->clear_mods = 0;

    /* Handle key actions */
    xkb_filter_apply_all(state, events, कुञ्चिका, direction);

    xkb_mod_index_t i;
    xkb_mod_mask_t bit;
    अत्थाय (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        सचे (state->set_mods & bit) {
            state->mod_कुञ्चिका_गणनम्[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    अत्थाय (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        सचे (state->clear_mods & bit) {
            state->mod_कुञ्चिका_गणनम्[i]--;
            सचे (state->mod_कुञ्चिका_गणनम्[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_कुञ्चिका_गणनम्[i] = 0;
            }
            state->clear_mods &= ~bit;
        }
    }

    xkb_state_update_derived(&state->base);

    /*
     * Insert key event only if actions did not override it, e.g.:
     * - RedirectKey() append its own key event;
     * - MovePointer() and PointerButton() generate pointer events instead
     *   of key events;
     * - TerminateServer() generate a Server event instead of key events.
     */
    सचे (state->flags & STATE_REQUIRE_कुञ्चिका_EVENT) {
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_कुञ्चिका,
            .कुञ्चिका = {
                .keycode = कुञ्चिका->keycode,
                .direction = direction
            }
        });
    }

    सचे (remap_event >= 0) {
        // FIXME: fragile if last state change does not restore the state to the remap event
        undo_tweaks(&state->base, &initial_components, events);
    }

    ध्रुव गणना xkb_state_component changed = get_state_component_changes(
        &initial_components, &state->base.components
    );
    सचे (changed) {
        सचे (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        // TODO: merge with previous state event, if any

        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = state->base.components,
                .changed = changed
            }
        });
    }

    सचे (!darray_empty(events->queue) &&
        darray_item(events->queue, darray_size(events->queue) - 1).type !=
        XKB_EVENT_TYPE_FRAME)
    {
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    पटिनिवत्त XKB_SUCCESS;
}

संरचना xkb_events *
xkb_events_new(संरचना xkb_context * restrict context,
               ध्रुव संरचना xkb_events_config * restrict config,
               गणना xkb_status * restrict status)
{
    /* Handle default configuration */
    स्थिर ध्रुव संरचना xkb_events_config default_config = {
        .size = परिमाण(default_config)
    };
    सचे (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ध्रुव गणना xkb_status abi_error = xkb_check_state_abi(config);
    सचे (abi_error) {
        xkb_log_abi_error(context, __func__, abi_error);
        सचे (status)
            *status = abi_error;
        पटिनिवत्त रिक्त;
    }

    /* Sanitize input */
    ध्रुव uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_EVENTS_FLAGS_मानानि);
    सचे (invalid_flags) {
        log_err_func(context, XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS_,
                     "unrecognized events collection flags: 0x%"PRIx32"\n",
                     invalid_flags);
        सचे (status)
            *status = XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS;
        पटिनिवत्त रिक्त;
    }

    संरचना xkb_events *events = calloc(1, परिमाण(*events));
    सचे (!events) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events collection\n");
        सचे (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        पटिनिवत्त रिक्त;
    }

    /* Pre-allocate event queue */
    darray_init(events->queue);
    स्थिर ध्रुव darray_size_t queue_initial_size = 16;
    darray_resize(events->queue, queue_initial_size);
    // TODO: proper safe darray_resize() with return value
    सचे (!darray_वस्तूनि(events->queue)) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events queue\n");
        xkb_events_destroy(events);
        सचे (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        पटिनिवत्त रिक्त;
    }
    darray_size(events->queue) = 0;

    events->ctx = xkb_context_ref(context);
    events->next = 0;

    सचे (status)
        *status = XKB_SUCCESS;
    पटिनिवत्त events;
}

शून्य
xkb_events_destroy(संरचना xkb_events *events)
{
    सचे (!events)
        पटिनिवत्त;
    darray_free(events->queue);
    xkb_context_unref(events->ctx);
    मुक्त(events);
}

ध्रुव संरचना xkb_event *
xkb_events_next(संरचना xkb_events *events)
{
    सचे (events->next < darray_size(events->queue)) {
        ध्रुव darray_size_t index = events->next++;
        पटिनिवत्त &darray_item(events->queue, index);
    } अञ्ञथा {
        पटिनिवत्त रिक्त;
    }
}

गणना xkb_event_type
xkb_event_get_type(ध्रुव संरचना xkb_event *event)
{
    पटिनिवत्त event->type;
}

गणना xkb_status
xkb_event_get_keycode(ध्रुव संरचना xkb_event *event,
                      xkb_keycode_t *keycode,
                      गणना xkb_कुञ्चिका_direction *direction)
{
    सचे (event->type == XKB_EVENT_TYPE_कुञ्चिका) {
        *keycode = event->कुञ्चिका.keycode;
        *direction = event->कुञ्चिका.direction;
        पटिनिवत्त XKB_SUCCESS;
    } अञ्ञथा {
        पटिनिवत्त XKB_ERROR_EVENT_TYPE_MISMATCH;
    }
}

गणना xkb_status
xkb_event_get_components(ध्रुव संरचना xkb_event * restrict event,
                         संरचना xkb_event_components * restrict components)
{
    सचे (event->type != XKB_EVENT_TYPE_STATE_COMPONENTS)
        पटिनिवत्त XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ध्रुव गणना xkb_status status = xkb_check_state_abi(components);
    सचे (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        पटिनिवत्त status;
    }

    components->changed = (uint32_t)event->components.changed;
    components->depressed_mods = event->components.components.base_mods;
    components->latched_mods = event->components.components.latched_mods;
    components->locked_mods = event->components.components.locked_mods;
    components->mods = event->components.components.mods;
    components->depressed_layout = event->components.components.base_group;
    components->latched_layout = event->components.components.latched_group;
    components->locked_layout = event->components.components.locked_group;
    components->layout = event->components.components.group;
    components->leds = event->components.components.leds;
    components->controls = (uint32_t)event->components.components.controls;

    पटिनिवत्त XKB_SUCCESS;
}

गणना xkb_status
xkb_event_get_pointer_motion(ध्रुव संरचना xkb_event * restrict event,
                             संरचना xkb_event_pointer_motion * restrict motion)
{
    सचे (event->type != XKB_EVENT_TYPE_POINTER_MOTION)
        पटिनिवत्त XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ध्रुव गणना xkb_status status = xkb_check_state_abi(motion);
    सचे (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        पटिनिवत्त status;
    }

    motion->flags = event->pointer_motion.flags;
    motion->x = event->pointer_motion.x;
    motion->y = event->pointer_motion.y;

    पटिनिवत्त XKB_SUCCESS;
}

गणना xkb_status
xkb_event_get_pointer_button(ध्रुव संरचना xkb_event * restrict event,
                             संरचना xkb_event_pointer_button * restrict button)
{
    सचे (event->type != XKB_EVENT_TYPE_POINTER_BUTTON)
        पटिनिवत्त XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ध्रुव गणना xkb_status status = xkb_check_state_abi(button);
    सचे (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        पटिनिवत्त status;
    }

    button->button = event->pointer_button.button;
    button->state = event->pointer_button.state;
    button->गणनम् = event->pointer_button.गणनम्;

    पटिनिवत्त XKB_SUCCESS;
}

गणना xkb_status
xkb_event_get_virtual_console(ध्रुव संरचना xkb_event * restrict event,
                              int8_t * restrict index_or_offset,
                              bool * restrict is_offset)
{
    सचे (event->type != XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
        पटिनिवत्त XKB_ERROR_EVENT_TYPE_MISMATCH;

    *index_or_offset = event->virtual_console.index_or_offset;
    *is_offset = event->virtual_console.is_offset;

    पटिनिवत्त XKB_SUCCESS;
}

गणना xkb_status
xkb_state_update_event(संरचना xkb_state * restrict base_state,
                       ध्रुव संरचना xkb_event * restrict event,
                       गणना xkb_state_component * restrict changed)
{
    /* Guard against server-only state */
    प्रतिज्ञा(base_state->mode == SERVER_COMPANION ||
           base_state->mode == LEGACY_MIXED_STATE);
    सचे (base_state->mode != SERVER_COMPANION &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE_,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        पटिनिवत्त XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    संरचना xkb_client_state * ध्रुव state =
        (संरचना xkb_client_state *)base_state;

    सचे (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
        ध्रुव संरचना state_components prev_components = state->base.components;
        state->base.components = event->components.components;
        /*
         * Recompute the changes instead of using the event value, because we do
         * not know if the event’s queue and the state are synced.
         */
        सचे (changed)
            *changed = get_state_component_changes(&prev_components,
                                                   &state->base.components);
    } अञ्ञथा {
        सचे (changed)
            *changed = 0;
    }
    पटिनिवत्त XKB_SUCCESS;
}

स्थिर शून्य
append_pointer_button(संरचना xkb_events * restrict events,
                      संरचना xkb_filter * restrict filter,
                      गणना xkb_pointer_button_state button_state,
                      uint8_t गणनम्)
{
    darray_append(events->queue, (संरचना xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_BUTTON,
        .pointer_button = {
            .size = परिमाण(((संरचना xkb_event*)0)->pointer_button),
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
    सचे (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_कुञ्चिकाः)) {
        filter->func = रिक्त;
        पटिनिवत्त;
    }

    संरचना xkb_machine * ध्रुव sm = (संरचना xkb_machine *)state;

    /* Resolve button */
    ध्रुव xkb_pointer_button_index_t button =
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
    सचे (कुञ्चिका != filter->कुञ्चिका || !events)
        पटिनिवत्त XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;

    विकल्प (direction) {
    पक्ष XKB_कुञ्चिका_DOWN:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_कुञ्चिका_REPEATED:
        पटिनिवत्त XKB_FILTER_CONSUME;
    स्वभाव:
        सचे (--filter->refcnt > 0)
            पटिनिवत्त XKB_FILTER_CONSUME;
    }

    सचे (!filter->action.btn.गणनम्)
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);

    filter->func = रिक्त;
    पटिनिवत्त XKB_FILTER_CONSUME;
}

संरचना xkb_filter_pointer_lock_button_priv {
    uint32_t button:31;
#ifdef _WIN32
    uint32_t locked:1; // MSVC requires matching types to pack bitfields
#अञ्ञथा
    bool locked:1;
#endif
};

गणना {
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH =
        परिमाण(संरचना xkb_filter_pointer_lock_button_priv) * CHAR_BIT,
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX =
        (UINT64_C(1) << XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH) - 1
};

static_assert(परिमाण(संरचना xkb_filter_pointer_lock_button_priv) ==
              परिमाण(((संरचना xkb_filter*)0)->priv),
              "");
static_assert((uint32_t)XKB_POINTER_BUTTON_MAX < (uint32_t)XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX,
              "xkb_filter_pointer_lock_button_priv cannot store button");

स्थिर शून्य
xkb_filter_pointer_lock_button_new(संरचना xkb_server_state *state,
                                   संरचना xkb_events *events,
                                   संरचना xkb_filter *filter)
{
    सचे (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_कुञ्चिकाः)) {
        filter->func = रिक्त;
        पटिनिवत्त;
    }

    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;

    संरचना xkb_machine * ध्रुव sm = (संरचना xkb_machine *)state;

    /* Resolve button */
    ध्रुव xkb_pointer_button_index_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;

    static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                  "invalid UINTx_C() macro");
    static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                  "invalid left shift");
    ध्रुव xkb_pointer_button_mask_t mask = (UINT8_C(1) << button);

    ध्रुव bool locked = (sm->mouse.locked_buttons & mask);

    सचे (!locked && !(filter->action.btn.flags & ACTION_LOCK_NO_LOCK)) {
        sm->mouse.locked_buttons |= mask;
        filter->priv = button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_PRESSED, 0);
    }

    संरचना xkb_filter_pointer_lock_button_priv *priv =
        (संरचना xkb_filter_pointer_lock_button_priv*)&filter->priv;
    priv->button = button,
    priv->locked = locked;
}

स्थिर bool
xkb_filter_pointer_lock_button_func(संरचना xkb_server_state *state,
                                    संरचना xkb_events *events,
                                    संरचना xkb_filter *filter,
                                    ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                                    गणना xkb_कुञ्चिका_direction direction)
{
    सचे (कुञ्चिका != filter->कुञ्चिका || !events || state->base.mode != SERVER_STATE)
        पटिनिवत्त XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;

    विकल्प (direction) {
    पक्ष XKB_कुञ्चिका_DOWN:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_कुञ्चिका_REPEATED:
        पटिनिवत्त XKB_FILTER_CONSUME;
    स्वभाव:
        सचे (--filter->refcnt > 0)
            पटिनिवत्त XKB_FILTER_CONSUME;
    }

    ध्रुव संरचना xkb_filter_pointer_lock_button_priv *priv =
        (संरचना xkb_filter_pointer_lock_button_priv*)&filter->priv;

    सचे (priv->locked && !(filter->action.btn.flags & ACTION_LOCK_NO_UNLOCK)) {
        संरचना xkb_machine * ध्रुव sm = (संरचना xkb_machine *)state;

        static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                    "invalid UINTx_C() macro");
        static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                    "invalid left shift");
        ध्रुव xkb_pointer_button_mask_t mask = (UINT8_C(1) << priv->button);

        sm->mouse.locked_buttons &= ~mask;
        filter->priv = priv->button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);
    }

    filter->func = रिक्त;
    पटिनिवत्त XKB_FILTER_CONSUME;
}

स्थिर शून्य
xkb_filter_pointer_set_default_button_new(संरचना xkb_server_state *state,
                                          संरचना xkb_events *events,
                                          संरचना xkb_filter *filter)
{
    सचे (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_कुञ्चिकाः)) {
        filter->func = रिक्त;
        पटिनिवत्त;
    }

    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;

    संरचना xkb_machine * ध्रुव sm = (संरचना xkb_machine *)state;

    static_assert(2 * XKB_POINTER_BUTTON_MAX <= INT_MAX, "lossy conversion");
    पूर्णाङ्क button = (filter->action.dflt.flags & ACTION_ABSOLUTE_SWITCH)
        ? filter->action.dflt.मानम्
        : sm->mouse.default_button + filter->action.dflt.मानम्;

    /* Wrap into range */
    ध्रुव पूर्णाङ्क rem = button % XKB_POINTER_BUTTON_MAX;
    sm->mouse.default_button = (rem >= 0)
        ? (xkb_pointer_button_index_t)rem
        : (xkb_pointer_button_index_t)(rem + XKB_POINTER_BUTTON_MAX);
}

स्थिर bool
xkb_filter_pointer_set_default_button_func(संरचना xkb_server_state *state,
                                           संरचना xkb_events *events,
                                           संरचना xkb_filter *filter,
                                           ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका,
                                           गणना xkb_कुञ्चिका_direction direction)
{
    सचे (कुञ्चिका != filter->कुञ्चिका || !events)
        पटिनिवत्त XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;

    विकल्प (direction) {
    पक्ष XKB_कुञ्चिका_DOWN:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_कुञ्चिका_REPEATED:
        पटिनिवत्त XKB_FILTER_CONSUME;
    स्वभाव:
        सचे (--filter->refcnt > 0)
            पटिनिवत्त XKB_FILTER_CONSUME;
    }

    filter->func = रिक्त;
    पटिनिवत्त XKB_FILTER_CONTINUE;
}

स्थिर शून्य
xkb_filter_terminate_server_new(संरचना xkb_server_state *state,
                                 संरचना xkb_events *events,
                                 संरचना xkb_filter *filter)
{
    सचे (!events || state->base.mode != SERVER_STATE ||
        !(((संरचना xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = रिक्त;
        पटिनिवत्त;
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
    सचे (कुञ्चिका != filter->कुञ्चिका || !events)
        पटिनिवत्त XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;

    विकल्प (direction) {
    पक्ष XKB_कुञ्चिका_DOWN:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_कुञ्चिका_REPEATED:
        पटिनिवत्त XKB_FILTER_CONSUME;
    स्वभाव:
        सचे (--filter->refcnt > 0)
            पटिनिवत्त XKB_FILTER_CONSUME;
    }

    filter->func = रिक्त;
    पटिनिवत्त XKB_FILTER_CONSUME;
}

स्थिर शून्य
xkb_filter_switch_virtual_console_new(संरचना xkb_server_state *state,
                                      संरचना xkb_events *events,
                                      संरचना xkb_filter *filter)
{
    सचे (!events || state->base.mode != SERVER_STATE ||
        !(((संरचना xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = रिक्त;
        पटिनिवत्त;
    }

    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;

    darray_append(events->queue, (संरचना xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
        .virtual_console = {
            .index_or_offset = filter->action.screen.screen,
            .is_offset = !(filter->action.screen.flags & ACTION_ABSOLUTE_SWITCH)
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
    सचे (कुञ्चिका != filter->कुञ्चिका || !events)
        पटिनिवत्त XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_कुञ्चिका_EVENT;

    विकल्प (direction) {
    पक्ष XKB_कुञ्चिका_DOWN:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_कुञ्चिका_REPEATED:
        पटिनिवत्त XKB_FILTER_CONSUME;
    स्वभाव:
        सचे (--filter->refcnt > 0)
            पटिनिवत्त XKB_FILTER_CONSUME;
    }

    filter->func = रिक्त;
    पटिनिवत्त XKB_FILTER_CONSUME;
}
