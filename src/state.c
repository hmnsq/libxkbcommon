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

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <limits.h>
#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon-status.h"
#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon-features.h"
#ଅନ୍ତର୍ଭାଵ "abi-check.h"
#ଅନ୍ତର୍ଭାଵ "darray.h"
#ଅନ୍ତର୍ଭାଵ "features/enums.h"
#ଅନ୍ତର୍ଭାଵ "keymap.h"
#ଅନ୍ତର୍ଭାଵ "keysym.h"
#ଅନ୍ତର୍ଭାଵ "messages-codes.h"
#ଅନ୍ତର୍ଭାଵ "state-priv.h"
#ଅନ୍ତର୍ଭାଵ "utf8.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "util-mem.h"
#ଅନ୍ତର୍ଭାଵ "util-numbers.h"

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
ସଂରଚନା xkb_events {
    ସଂରଚନା xkb_context *ctx;
    darray(ସଂରଚନା xkb_event) queue;
    /**
     * Read cursor for `xkb_events_next()`.
     *
     * Reset to 0 on each `process_*` call.
     */
    darray_size_t next;
};

ସଂରଚନା xkb_server_state;

ସଂରଚନା xkb_filter {
    ସଂଘ xkb_action action;
    ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି;
    bool (*func)(ସଂରଚନା xkb_server_state *state,
                 ସଂରଚନା xkb_events *events,
                 ସଂରଚନା xkb_filter *filter,
                 ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                 ଗଣନା xkb_ଚାବି_direction direction);
    uint32_t priv;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା refcnt;
};

ଗଣନା xkb_state_mode_internal {
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
    XKB_STATE_TYPE_ଅଙ୍କ_ENTRIES = 5
};

static_assert(CLIENT_STATE == 0 &&
              CLIENT_STATE == SERVER_COMPANION - 1 &&
              SERVER_COMPANION == LEGACY_MIXED_STATE - 1 &&
              LEGACY_MIXED_STATE == LEGACY_SERVER_STATE - 1 &&
              LEGACY_SERVER_STATE == SERVER_STATE - 1 &&
              SERVER_STATE == XKB_STATE_TYPE_ଅଙ୍କ_ENTRIES - 1 &&
              XKB_STATE_TYPE_ଅଙ୍କ_ENTRIES == 5,
              "Order is used to guard client/server -only APIs: do not change");

ଗଣନା { XKB_LAYOUT_INDEX_T_MIN_WIDTH = 6 };
static_assert(XKB_MAX_GROUPS < (1 << XKB_LAYOUT_INDEX_T_MIN_WIDTH),
              "Cannot encode out_of_range_group::redirect_group");

ସଂରଚନା out_of_range_group {
    ଗଣନା xkb_layout_out_of_range_policy policy :
        (ଆକାର(ଗଣନା xkb_layout_out_of_range_policy) * CHAR_BIT -
         XKB_LAYOUT_INDEX_T_MIN_WIDTH);
    xkb_layout_index_t redirect_group : XKB_LAYOUT_INDEX_T_MIN_WIDTH;
};

ଗଣନା { XKB_STATE_MODE_INTERNAL_MIN_WIDTH = 4 /* 3 bits + sign */ };
static_assert(XKB_STATE_TYPE_ଅଙ୍କ_ENTRIES <
              (1 << XKB_STATE_MODE_INTERNAL_MIN_WIDTH),
              "Cannot encode xkb_state::mode");

/**
 * Common state
 *
 * `mode` indicates whether the object is casted from a `xkb_server_state` or a
 * `xkb_client_state` struct.
 */
ସଂରଚନା xkb_state {
    ସଂରଚନା xkb_keymap *keymap;
    /*
     * Before updating the state, we keep a copy of just this struct. This
     * allows us to report which components of the state have changed.
     */
    ସଂରଚନା state_components components;
    ସଂରଚନା out_of_range_group out_of_range_group;

    ଗଣନା xkb_state_mode_internal mode : XKB_STATE_MODE_INTERNAL_MIN_WIDTH;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା refcnt : (ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା) * CHAR_BIT - XKB_STATE_MODE_INTERNAL_MIN_WIDTH);
};

/**
 * Minimal client state
 *
 * Wrapper for strong typing
 */
ସଂରଚନା xkb_client_state {
    /** Inherits from `xkb_state` */
    ସଂରଚନା xkb_state base;
};

ଗଣନା state_flags {
    STATE_NO_FLAGS = 0,
    STATE_REQUIRE_ଚାବି_EVENT = (1 << 0),
    STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_ଚାବି = (1 << 1),
    STATE_UNLOCK_MODS_ON_PRESS = (1 << 2),
};

/**
 * Legacy server state, used for `LEGACY_MIXED_STATE` and `LEGACY_SERVER_STATE`
 * modes.
 *
 * Casted as `xkb_state` in the public API. See `xkb_state::mode`.
 */
ସଂରଚନା xkb_server_state {
    /** Inherits from `xkb_state` */
    ସଂରଚନା xkb_state base;

    /* Server-specific state data */

    darray(ସଂରଚନା xkb_filter) filters;

    /**
     * Mix of constant configuration flags and
     * variable state-processing flags.
     */
    ଗଣନା state_flags flags;

    ଗଣନା xkb_a11y_flags a11y_flags;

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
    int16_t mod_ଚାବି_ଗଣନା[XKB_MAX_MODS];
};

ସ୍ଥିର ଅଚଳ ସଂରଚନା xkb_ଚାବି_type_entry *
get_entry_for_mods(ଅଚଳ ସଂରଚନା xkb_ଚାବି_type *type, xkb_mod_mask_t mods)
{
    ପାଇଁ (darray_size_t i = 0; i < type->ଅଙ୍କ_entries; i++)
        ଯଦି (entry_is_active(&type->entries[i]) &&
            type->entries[i].mods.mask == mods)
            ଫେରନ୍ତୁ &type->entries[i];
    ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ଅଚଳ ସଂରଚନା xkb_ଚାବି_type_entry *
get_entry_for_ଚାବି_state(ସଂରଚନା xkb_state *state, ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                        xkb_layout_index_t group)
{
    ଅଚଳ ସଂରଚନା xkb_ଚାବି_type* ଅଚଳ type = ଚାବି->groups[group].type;
    xkb_mod_mask_t active_mods = state->components.mods & type->mods.mask;
    ଫେରନ୍ତୁ get_entry_for_mods(type, active_mods);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ xkb_level_index_t
state_ଚାବି_get_level(ସଂରଚନା xkb_state *state, ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                         xkb_layout_index_t layout)
{
    ଯଦି (layout >= ଚାବି->ଅଙ୍କ_groups)
        ଫେରନ୍ତୁ XKB_LEVEL_INVALID;

    /* If we don't find an explicit match the default is 0. */
    ଅଚଳ ସଂରଚନା xkb_ଚାବି_type_entry* ଅଚଳ entry =
        get_entry_for_ଚାବି_state(state, ଚାବି, layout);

    ଫେରନ୍ତୁ (entry) ? entry->level : 0;
}

/**
 * Returns the level to use for the given key and state, or
 * XKB_LEVEL_INVALID.
 */
xkb_level_index_t
xkb_state_ଚାବି_get_level(ସଂରଚନା xkb_state *state, xkb_keycode_t kc,
                        xkb_layout_index_t layout)
{
    ଅଚଳ ସଂରଚନା xkb_ଚାବି* ଅଚଳ ଚାବି = Xkb_ଚାବି(state->keymap, kc);

    ଫେରନ୍ତୁ (ଚାବି)
        ? state_ଚାବି_get_level(state, ଚାବି, layout)
        : XKB_LEVEL_INVALID;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ xkb_layout_index_t
state_ଚାବି_get_layout(ସଂରଚନା xkb_state *state, ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି)
{
    ଫେରନ୍ତୁ xkb_keymap_ଚାବି_effective_layout(ଚାବି, state->components.group);
}

/**
 * Returns the layout to use for the given key and state, taking
 * wrapping/clamping/etc into account, or XKB_LAYOUT_INVALID.
 */
xkb_layout_index_t
xkb_state_ଚାବି_get_layout(ସଂରଚନା xkb_state *state, xkb_keycode_t kc)
{
    ଅଚଳ ସଂରଚନା xkb_ଚାବି* ଅଚଳ ଚାବି = Xkb_ଚାବି(state->keymap, kc);

    ଯଦି (!ଚାବି)
        ଫେରନ୍ତୁ XKB_LAYOUT_INVALID;

    ଫେରନ୍ତୁ state_ଚାବି_get_layout(state, ଚାବି);
}

/* Empty action used for empty levels */
ସ୍ଥିର ଅଚଳ ସଂଘ xkb_action dummy_action = { .type = ACTION_TYPE_NONE };

ସ୍ଥିର xkb_action_ଗଣନା_t
xkb_ଚାବି_get_actions(ସଂରଚନା xkb_state *state, ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                    ଅଚଳ ସଂଘ xkb_action **actions)
{
    ଅଚଳ xkb_layout_index_t layout = state_ଚାବି_get_layout(state, ଚାବି);
    ଅଚଳ xkb_level_index_t level = state_ଚାବି_get_level(state, ଚାବି, layout);
    ଯଦି (level == XKB_LEVEL_INVALID)
        ଯାଅ err;

    ଅଚଳ xkb_action_ଗଣନା_t ଗଣନା_2 =
        xkb_keymap_ଚାବି_get_actions_by_level(state->keymap, ଚାବି,
                                            layout, level, actions);
    ଯଦି (!ଗଣନା_2)
        ଯାଅ err;

    ଫେରନ୍ତୁ ଗଣନା_2;

err:
    /* Use a dummy action if no corresponding level was found or if it is empty.
     * This is required e.g. to handle latches properly. */
    *actions = &dummy_action;
    ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ସଂରଚନା xkb_filter *
xkb_filter_new(ସଂରଚନା xkb_server_state *state)
{
    ସଂରଚନା xkb_filter *filter = ଖାଲି, *iter;

    darray_foreach(iter, state->filters) {
        ଯଦି (iter->func)
            ଜାରି;
        /* Use available slot */
        filter = iter;
        ଭାଙ୍ଗ;
    }

    ଯଦି (!filter) {
        /* No available slot: resize the filters array */
        darray_resize0(state->filters, darray_size(state->filters) + 1);
        filter = &darray_item(state->filters, darray_size(state->filters) -1);
    }

    filter->refcnt = 1;
    ଫେରନ୍ତୁ filter;
}

/***====================================================================***/

ଗଣନା xkb_filter_result {
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
    ଯଦି ((filter_)->action.group.flags & ACTION_ABSOLUTE_SWITCH)          \
        (state_)->components.component_ = (filter_)->action.group.group; \
    ଅନ୍ୟଥା                                                                 \
        (state_)->components.component_ += (filter_)->action.group.group

ସ୍ଥିର ଶୂନ୍ୟ
xkb_filter_group_set_new(ସଂରଚନା xkb_server_state *state,
                         ସଂରଚନା xkb_events *events,
                         ସଂରଚନା xkb_filter *filter)
{
    static_assert(ଆକାର(state->base.components.base_group) ==
                  ଆକାର(filter->priv),
                  "Max groups don't fit");
    filter->priv = (uint32_t)state->base.components.base_group;
    apply_group_delta(filter, &state->base, base_group);
}

ସ୍ଥିର bool
xkb_filter_group_set_func(ସଂରଚନା xkb_server_state *state,
                          ସଂରଚନା xkb_events *events,
                          ସଂରଚନା xkb_filter *filter,
                          ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                          ଗଣନା xkb_ଚାବି_direction direction)
{
    ଯଦି (ଚାବି != filter->ଚାବି) {
        filter->action.group.flags &= ~ACTION_LOCK_CLEAR;
        ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;
    }

    ବାଛ (direction) {
    ସ୍ଥିତି XKB_ଚାବି_DOWN:
        filter->refcnt++;
        /* fallthrough */
    ସ୍ଥିତି XKB_ଚାବି_REPEATED:
        ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯଦି (--filter->refcnt > 0)
            ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    }

    static_assert(ଆକାର(state->base.components.base_group) == ଆକାର(filter->priv),
                  "Max groups don't fit");
    state->base.components.base_group = (int32_t) filter->priv;

    ଯଦି (filter->action.group.flags & ACTION_LOCK_CLEAR)
        state->base.components.locked_group = 0;

    filter->func = ଖାଲି;
    ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;
}

ସ୍ଥିର ଗଣନା xkb_state_component
get_state_component_changes(ଅଚଳ ସଂରଚନା state_components *a,
                            ଅଚଳ ସଂରଚନା state_components *b)
{
    ଗଣନା xkb_state_component mask = 0;

    ଯଦି (a->group != b->group)
        mask |= XKB_STATE_LAYOUT_EFFECTIVE;
    ଯଦି (a->base_group != b->base_group)
        mask |= XKB_STATE_LAYOUT_DEPRESSED;
    ଯଦି (a->latched_group != b->latched_group)
        mask |= XKB_STATE_LAYOUT_LATCHED;
    ଯଦି (a->locked_group != b->locked_group)
        mask |= XKB_STATE_LAYOUT_LOCKED;
    ଯଦି (a->mods != b->mods)
        mask |= XKB_STATE_MODS_EFFECTIVE;
    ଯଦି (a->base_mods != b->base_mods)
        mask |= XKB_STATE_MODS_DEPRESSED;
    ଯଦି (a->latched_mods != b->latched_mods)
        mask |= XKB_STATE_MODS_LATCHED;
    ଯଦି (a->locked_mods != b->locked_mods)
        mask |= XKB_STATE_MODS_LOCKED;
    ଯଦି (a->leds != b->leds)
        mask |= XKB_STATE_LEDS;
    ଯଦି (a->controls != b->controls)
        mask |= XKB_STATE_CONTROLS_EFFECTIVE;

    ଫେରନ୍ତୁ mask;
}

ସ୍ଥିର ଶୂନ୍ୟ
xkb_filter_group_lock_new(ସଂରଚନା xkb_server_state *state,
                          ସଂରଚନା xkb_events *events,
                          ସଂରଚନା xkb_filter *filter)
{
    ଯଦି (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release: do nothing on key press.
         *
         * This is a keymap format v2 extension.
         */
        ଫେରନ୍ତୁ;
    } ଅନ୍ୟଥା {
        /* Lock on key press */
        apply_group_delta(filter, &state->base, locked_group);
    }
}

ସ୍ଥିର bool
xkb_filter_group_lock_func(ସଂରଚନା xkb_server_state *state,
                           ସଂରଚନା xkb_events *events,
                           ସଂରଚନା xkb_filter *filter,
                           ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                           ଗଣନା xkb_ଚାବି_direction direction)
{
    ଯଦି (ଚାବି != filter->ଚାବି) {
        ଯଦି ((filter->action.group.flags & ACTION_LOCK_ON_RELEASE) &&
            direction == XKB_ଚାବି_DOWN) {
            /*
             * Another key has been pressed after the locking key:
             * cancel group lock on release.
             *
             * This is a keymap v2 extension.
             */
            filter->action.group.flags &= ~ACTION_LOCK_ON_RELEASE;
        }
        ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;
    }

    ବାଛ (direction) {
    ସ୍ଥିତି XKB_ଚାବି_DOWN:
        filter->refcnt++;
        /* fallthrough */
    ସ୍ଥିତି XKB_ଚାବି_REPEATED:
        ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯଦି (--filter->refcnt > 0)
            ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    }

    ଯଦି (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release
         *
         * This is a keymap v2 extension.
         */
        apply_group_delta(filter, &state->base, locked_group);
    } ଅନ୍ୟଥା {
        /* Lock on key press: do nothing on key release. */
    }
    filter->func = ଖାଲି;
    ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;
}

ସ୍ଥିର bool
xkb_action_breaks_latch(ଅଚଳ ସଂଘ xkb_action *action,
                        ଗଣନା xkb_action_controls controls,
                        ଗଣନା xkb_internal_action_flags flag,
                        xkb_mod_mask_t mask)
{
    ବାଛ (action->type) {
    ସ୍ଥିତି ACTION_TYPE_NONE:
    ସ୍ଥିତି ACTION_TYPE_VOID:
    ସ୍ଥିତି ACTION_TYPE_PTR_BUTTON:
    ସ୍ଥିତି ACTION_TYPE_PTR_LOCK:
    ସ୍ଥିତି ACTION_TYPE_CTRL_SET:
    ସ୍ଥିତି ACTION_TYPE_CTRL_LOCK:
    ସ୍ଥିତି ACTION_TYPE_SWITCH_VT:
    ସ୍ଥିତି ACTION_TYPE_TERMINATE:
    ସ୍ଥିତି ACTION_TYPE_REDIRECT_ଚାବି:
        ଫେରନ୍ତୁ true;
    ସ୍ଥିତି ACTION_TYPE_PTR_MOVE:
    ସ୍ଥିତି ACTION_TYPE_PTR_DEFAULT:
        ଫେରନ୍ତୁ !(controls & CONTROL_MOUSE_ଚାବିଗୁଡ଼ିକ);
    ସ୍ଥିତି ACTION_TYPE_INTERNAL:
        ଫେରନ୍ତୁ (action->internal.flags & flag) &&
               ((action->internal.clear_latched_mods & mask) == mask);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_ଅଙ୍କ_ENTRIES,
                      "Missing action type");
        ଫେରନ୍ତୁ false;
    }
}

ଗଣନା xkb_ଚାବି_latch_state {
    NO_LATCH = 0,
    LATCH_ଚାବି_DOWN,
    LATCH_PENDING,
    ଚାବି_LATCH_STATE_ଅଙ୍କ_ENTRIES
};

#define MAX_XKB_ଚାବି_LATCH_STATE_LOG2 2
#ଯଦି (ଚାବି_LATCH_STATE_ଅଙ୍କ_ENTRIES > (1 << MAX_XKB_ଚାବି_LATCH_STATE_LOG2)) || \
    (-XKB_MAX_GROUPS) < (INT32_MIN >> MAX_XKB_ଚାବି_LATCH_STATE_LOG2) || \
      XKB_MAX_GROUPS > (INT32_MAX >> MAX_XKB_ଚାବି_LATCH_STATE_LOG2)
#error "Cannot represent priv field of the group latch filter"
#endif

/* Hold the latch state *and* the group delta */
ସଂଘ group_latch_priv {
    uint32_t priv;
    ସଂରଚନା {
        /* The type is really: enum xkb_key_latch_state, but it is problematic
         * on Windows, because it is interpreted as signed and leads to wrong
         * negative values. */
        unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା latch:MAX_XKB_ଚାବି_LATCH_STATE_LOG2;
        int32_t group_delta:(32 - MAX_XKB_ଚାବି_LATCH_STATE_LOG2);
    };
};

ସ୍ଥିର ଶୂନ୍ୟ
xkb_filter_group_latch_new(ସଂରଚନା xkb_server_state *state,
                           ସଂରଚନା xkb_events *events,
                           ସଂରଚନା xkb_filter *filter)
{
    ଅଚଳ ସଂଘ group_latch_priv priv = {
        .latch = LATCH_ଚାବି_DOWN,
        .group_delta = (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)
            ? filter->action.group.group - state->base.components.base_group
            : filter->action.group.group
    };
    filter->priv = priv.priv;
    /* Like group set */
    apply_group_delta(filter, &state->base, base_group);
}

ସ୍ଥିର bool
xkb_filter_group_latch_func(ସଂରଚନା xkb_server_state *state,
                            ସଂରଚନା xkb_events *events,
                            ସଂରଚନା xkb_filter *filter,
                            ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                            ଗଣନା xkb_ଚାବି_direction direction)
{
    ସଂଘ group_latch_priv priv = {.priv = filter->priv};
    ଗଣନା xkb_ଚାବି_latch_state latch = priv.latch;

    ଯଦି (direction == XKB_ଚାବି_DOWN) {
        ଅଚଳ ସଂଘ xkb_action *actions = ଖାଲି;
        ଅଚଳ xkb_action_ଗଣନା_t ଗଣନା_2 = xkb_ଚାବି_get_actions(&state->base, ଚାବି,
                                                             &actions);

        ଯଦି (latch == LATCH_ଚାବି_DOWN) {
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
            ଯଦି (state->a11y_flags & XKB_A11_Y_LATCH_SIMULTANEOUS_ଚାବିଗୁଡ଼ିକ) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                ପାଇଁ (xkb_action_ଗଣନା_t k = 0; k < ଗଣନା_2; k++) {
                    ଯଦି (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_GROUP_LATCH,
                                                0))
                    {
                        latch = NO_LATCH;
                        ଭାଙ୍ଗ;
                    }
                }
            } ଅନ୍ୟଥା {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        ଅନ୍ୟଥା ଯଦି (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is
             * pressed, or promote it to a lock if it's the same group delta &
             * flags and latchToLock option is enabled. */
            ଅଚଳ bool sticky_ଚାବିଗୁଡ଼ିକ = state->base.components.controls
                                   & CONTROL_STICKY_ଚାବିଗୁଡ଼ିକ;
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
            ପାଇଁ (xkb_action_ଗଣନା_t k = 0; k < ଗଣନା_2; k++) {
                ଯଦି (((actions[k].type == ACTION_TYPE_GROUP_LATCH) ||
                     (actions[k].type == ACTION_TYPE_GROUP_SET && sticky_ଚାବିଗୁଡ଼ିକ)) &&
                     actions[k].group.group == filter->action.group.group &&
                     (actions[k].group.flags & ACTION_ABSOLUTE_SWITCH) ==
                     (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)) {
                    ଯଦି (filter->action.group.flags & ACTION_LATCH_TO_LOCK &&
                        filter->action.group.group != 0)
                    {
                        /* Promote to lock */
                        filter->action.type = ACTION_TYPE_GROUP_LOCK;
                        filter->func = xkb_filter_group_lock_func;
                        xkb_filter_group_lock_new(state, events, filter);
                        state->base.components.latched_group -= priv.group_delta;
                        filter->ଚାବି = ଚାବି;
                        /* XXX beep beep! */
                        ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
                    }
                    /* Do nothing if `latchToLock` option is not activated; if
                     * the latch is not broken by the following actions and the
                     * key is not consumed, then another latch filter will be
                     * created.
                     */
                    ଜାରି;
                }
                ଅନ୍ୟଥା ଯଦି (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_GROUP_LATCH,
                                                 0))
                {
                    /* Breaks the latch */
                    state->base.components.latched_group -= priv.group_delta;
                    filter->func = ଖାଲି;
                    ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;
                }
            }
        }
        ଅନ୍ୟଥା {
            /* Ignore press in NO_LATCH state */
            ପ୍ରତିଜ୍ଞା(latch == NO_LATCH);
        }
    }
    ଅନ୍ୟଥା ଯଦି (ଚାବି != filter->ଚାବି) {
        /* Ignore repeat/release of other keys */
    }
    ଅନ୍ୟଥା ଯଦି (direction == XKB_ଚାବି_REPEATED) {
        ଫେରନ୍ତୁ XKB_FILTER_CONSUME; // TODO: check
    }
    ଅନ୍ୟଥା {
        /* Our key got released.  If we've set it to clear locks, and we
         * currently have a group locked, then release it and
         * don't actually latch.  Else we've actually hit the latching
         * stage, so set PENDING and move our group from base to
         * latched. */
        ଯଦି ((filter->action.group.flags & ACTION_LOCK_CLEAR) &&
             state->base.components.locked_group) {
            ଯଦି (latch == LATCH_PENDING)
                state->base.components.latched_group -= priv.group_delta;
            ଅନ୍ୟଥା
                state->base.components.base_group -= priv.group_delta;
            state->base.components.locked_group = 0;
            filter->func = ଖାଲି;
        }
        /* Broken latch */
        ଅନ୍ୟଥା ଯଦି (latch == NO_LATCH) {
            state->base.components.base_group -= priv.group_delta;
            filter->func = ଖାଲି;
        }
        /* We may already have reached the latch state if pressing the
         * key multiple times without latch-to-lock enabled. */
        ଅନ୍ୟଥା ଯଦି (latch == LATCH_ଚାବି_DOWN) {
            latch = LATCH_PENDING;
            /* Switch from set to latch */
            state->base.components.base_group -= priv.group_delta;
            state->base.components.latched_group += priv.group_delta;
            /* XXX beep beep! */
        }
    }

    priv.latch = latch;
    filter->priv = priv.priv;

    ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;
}

ସ୍ଥିର ଶୂନ୍ୟ
xkb_filter_mod_set_new(ସଂରଚନା xkb_server_state *state,
                       ସଂରଚନା xkb_events *events,
                       ସଂରଚନା xkb_filter *filter)
{
    ଅଚଳ ଗଣନା xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    ଯଦି ((filter->action.mods.flags & unlock) == unlock) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension.
         */
        filter->priv = filter->action.mods.mods.mask
                     & ~state->base.components.locked_mods;
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
    } ଅନ୍ୟଥା {
        filter->priv = filter->action.mods.mods.mask;
    }

    state->set_mods |= (xkb_mod_mask_t) filter->priv;
}

ସ୍ଥିର bool
xkb_filter_mod_set_func(ସଂରଚନା xkb_server_state *state,
                        ସଂରଚନା xkb_events *events,
                        ସଂରଚନା xkb_filter *filter,
                        ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                        ଗଣନା xkb_ଚାବି_direction direction)
{

    ଯଦି (ଚାବି != filter->ଚାବି) {
        filter->action.mods.flags &= ~ACTION_LOCK_CLEAR;
        ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;
    }

    ବାଛ (direction) {
    ସ୍ଥିତି XKB_ଚାବି_DOWN:
        filter->refcnt++;
        /* fallthrough */
    ସ୍ଥିତି XKB_ଚାବି_REPEATED:
        ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯଦି (--filter->refcnt > 0)
            ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    }

    state->clear_mods |= (xkb_mod_mask_t) filter->priv;
    ଅଚଳ ଗଣନା xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    ଯଦି ((filter->action.mods.flags & unlock) == ACTION_LOCK_CLEAR)
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;

    filter->func = ଖାଲି;
    ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;
}

ସ୍ଥିର ଶୂନ୍ୟ
xkb_filter_mod_lock_new(ସଂରଚନା xkb_server_state *state,
                        ସଂରଚନା xkb_events *events,
                        ସଂରଚନା xkb_filter *filter)
{
    filter->priv = (state->base.components.locked_mods &
                    filter->action.mods.mods.mask);

    ଯଦି (filter->priv && (filter->action.mods.flags & ACTION_UNLOCK_ON_PRESS)) {
        /*
         * Some of the target modifiers were locked before key press: unlock.
         *
         * This is a keymap v2 extension: unlock-on-press.
         */
        ଯଦି (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
            state->base.components.locked_mods &= ~filter->priv;
        /* No further action: cancel filter */
        filter->func = ଖାଲି;
    } ଅନ୍ୟଥା {
        /* Set base mods; lock mods if relevant (XKB 1.0 spec) */
        state->set_mods |= filter->action.mods.mods.mask;
        ଯଦି (!(filter->action.mods.flags & ACTION_LOCK_NO_LOCK))
            state->base.components.locked_mods |= filter->action.mods.mods.mask;
    }
}

ସ୍ଥିର bool
xkb_filter_mod_lock_func(ସଂରଚନା xkb_server_state *state,
                         ସଂରଚନା xkb_events *events,
                         ସଂରଚନା xkb_filter *filter,
                         ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                         ଗଣନା xkb_ଚାବି_direction direction)
{
    ଯଦି (ଚାବି != filter->ଚାବି)
        ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;

    ବାଛ (direction) {
    ସ୍ଥିତି XKB_ଚାବି_DOWN:
        filter->refcnt++;
        /* fallthrough */
    ସ୍ଥିତି XKB_ଚାବି_REPEATED:
        ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯଦି (--filter->refcnt > 0)
            ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    }

    state->clear_mods |= filter->action.mods.mods.mask;
    ଯଦି (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
        state->base.components.locked_mods &= ~filter->priv;

    filter->func = ଖାଲି;
    ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;
}

ସ୍ଥିର ଶୂନ୍ୟ
xkb_filter_mod_latch_new(ସଂରଚନା xkb_server_state *state,
                         ସଂରଚନା xkb_events *events,
                         ସଂରଚନା xkb_filter *filter)
{
    /* Latch-on-press + clear-locks imply unlock-on-press */
    ଅଚଳ ଗଣନା xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                              | ACTION_LATCH_ON_PRESS;

    ଯଦି ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
        (filter->action.mods.flags & unlockOnPress) &&
        (state->base.components.locked_mods & filter->action.mods.mods.mask) ==
         filter->action.mods.mods.mask) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension: clear locks and do not latch.
         */
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
        filter->func = ଖାଲି;
    } ଅନ୍ୟଥା ଯଦି (filter->action.mods.flags & ACTION_LATCH_ON_PRESS) {
        /*
         * Latch on key press
         *
         * This is a keymap format v2 extension.
         */
        filter->priv = LATCH_PENDING;
        state->base.components.latched_mods |= filter->action.mods.mods.mask;
        /* XXX beep beep! */
    } ଅନ୍ୟଥା {
        /* XKB standard latch action */
        filter->priv = LATCH_ଚାବି_DOWN;
        state->set_mods |= filter->action.mods.mods.mask;
    }
}

ସ୍ଥିର bool
xkb_filter_mod_latch_func(ସଂରଚନା xkb_server_state *state,
                          ସଂରଚନା xkb_events *events,
                          ସଂରଚନା xkb_filter *filter,
                          ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                          ଗଣନା xkb_ଚାବି_direction direction)
{
    ଗଣନା xkb_ଚାବି_latch_state latch = filter->priv;

    ଯଦି (direction == XKB_ଚାବି_DOWN) {
        ଅଚଳ ସଂଘ xkb_action *actions = ଖାଲି;
        ଅଚଳ xkb_action_ଗଣନା_t ଗଣନା_2 = xkb_ଚାବି_get_actions(&state->base, ଚାବି,
                                                             &actions);

        ଯଦି (latch == LATCH_ଚାବି_DOWN) {
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
            ଯଦି (state->a11y_flags & XKB_A11_Y_LATCH_SIMULTANEOUS_ଚାବିଗୁଡ଼ିକ) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                ପାଇଁ (xkb_action_ଗଣନା_t k = 0; k < ଗଣନା_2; k++) {
                    ଯଦି (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_MOD_LATCH,
                                                filter->action.mods.mods.mask))
                    {
                        latch = NO_LATCH;
                        ଭାଙ୍ଗ;
                    }
                }
            } ଅନ୍ୟଥା {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        ଅନ୍ୟଥା ଯଦି (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is pressed,
             * or promote it to a lock or plain base set if it's the same
             * modifier. */
            ଅଚଳ bool sticky_ଚାବିଗୁଡ଼ିକ = state->base.components.controls
                                   & CONTROL_STICKY_ଚାବିଗୁଡ଼ିକ;
            /*
             * NOTE: Action’s flags are not compared:
             * - flags may have been modified by keyboard controls at the
             *   creation of the filter;
             * - it is unlikely to have two LatchMods() actions on the same
             *   key that differ only by their flags;
             * - even if it were the case, one would most likely want both
             *   actions to be considered compatible in this context.
             */
            ପାଇଁ (xkb_action_ଗଣନା_t k = 0; k < ଗଣନା_2; k++) {
                ଯଦି (((actions[k].type == ACTION_TYPE_MOD_LATCH) ||
                     (actions[k].type == ACTION_TYPE_MOD_SET && sticky_ଚାବିଗୁଡ଼ିକ)) &&
                    actions[k].mods.mods.mask == filter->action.mods.mods.mask)
                {
                    ଯଦି (filter->action.mods.flags & ACTION_LATCH_TO_LOCK) {
                        /* Mutate the action to LockMods() */
                        filter->action.type = ACTION_TYPE_MOD_LOCK;
                        filter->func = xkb_filter_mod_lock_func;
                        xkb_filter_mod_lock_new(state, events, filter);
                    }
                    ଅନ୍ୟଥା {
                        /* Mutate the action to SetMods() */
                        filter->action.type = ACTION_TYPE_MOD_SET;
                        filter->func = xkb_filter_mod_set_func;
                        xkb_filter_mod_set_new(state, events, filter);
                    }
                    filter->ଚାବି = ଚାବି;
                    /* Clear latches */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    /* XXX beep beep! */
                    ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
                }
                ଅନ୍ୟଥା ଯଦି (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_MOD_LATCH,
                                                 filter->action.mods.mods.mask))
                {
                    /* XXX: This may be totally broken, we might need to break the
                     *      latch in the next run after this press? */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    filter->func = ଖାଲି;
                    ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;
                }
            }
        }
        ଅନ୍ୟଥା {
            /* Ignore press in NO_LATCH state */
            ପ୍ରତିଜ୍ଞା(latch == NO_LATCH);
        }
    }
    ଅନ୍ୟଥା ଯଦି (ଚାବି != filter->ଚାବି) {
        /* Ignore repeat/release of other keys */
    }
    ଅନ୍ୟଥା ଯଦି (direction == XKB_ଚାବି_REPEATED) {
        ଫେରନ୍ତୁ XKB_FILTER_CONSUME; // TODO: check
    }
    ଅନ୍ୟଥା {
        /* Our key got released.  If we’ve set it to clear locks, and we
         * currently have the same modifiers locked, then release them and
         * don't actually latch.  Else we’ve actually hit the latching
         * stage, so set PENDING and move our modifier from base to
         * latched. */

        /* Latch-on-press + clear-locks imply unlock-on-press */
        ଅଚଳ ଗଣନା xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                                  | ACTION_LATCH_ON_PRESS;

        ଯଦି ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
            !(filter->action.mods.flags & unlockOnPress) &&
            (state->base.components.locked_mods &
             filter->action.mods.mods.mask) == filter->action.mods.mods.mask)
        {
            /* XXX: We might be a bit overenthusiastic about clearing
             *      mods other filters have set here? */
            ଯଦି (latch == LATCH_PENDING)
                state->base.components.latched_mods &=
                    ~filter->action.mods.mods.mask;
            ଅନ୍ୟଥା
                state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
            filter->func = ଖାଲି;
        }
        ଅନ୍ୟଥା ଯଦି (latch == NO_LATCH) {
            /* Broken latch */
            state->clear_mods |= filter->action.mods.mods.mask;
            filter->func = ଖାଲି;
        }
        ଅନ୍ୟଥା ଯଦି (!(filter->action.mods.flags & ACTION_LATCH_ON_PRESS)) {
            latch = LATCH_PENDING;
            state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.latched_mods |= filter->action.mods.mods.mask;
            /* XXX beep beep! */
        }
    }

    filter->priv = latch;

    ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;
}

ସ୍ଥିର ଶୂନ୍ୟ
append_pointer_move(ସଂରଚନା xkb_events *events,
                    ସଂରଚନା xkb_filter *filter)
{
    ଗଣନା xkb_pointer_motion_flags flags = 0;
    ଯଦି (filter->action.ptr.flags & ACTION_ABSOLUTE_X)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_X;
    ଯଦି (filter->action.ptr.flags & ACTION_ABSOLUTE_Y)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_Y;
    ଯଦି (filter->action.ptr.flags & ACTION_REPEAT)
        flags |= XKB_POINTER_MOTION_REPEATS;
    darray_append(events->queue, (ସଂରଚନା xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_MOTION,
        .pointer_motion = {
            .size = ଆକାର(((ସଂରଚନା xkb_event*)0)->pointer_motion),
            .flags = flags,
            .x = filter->action.ptr.x,
            .y = filter->action.ptr.y,
        }
    });
}

ସ୍ଥିର ଶୂନ୍ୟ
xkb_filter_pointer_move_new(ସଂରଚନା xkb_server_state *state,
                            ସଂରଚନା xkb_events *events,
                            ସଂରଚନା xkb_filter *filter)
{
    ଯଦି (!events) {
        filter->func = ଖାଲି;
        ଫେରନ୍ତୁ;
    }

    ପ୍ରତିଜ୍ଞା(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ଚାବିଗୁଡ଼ିକ);
    append_pointer_move(events, filter);
    state->flags &= ~STATE_REQUIRE_ଚାବି_EVENT;
}

ସ୍ଥିର bool
xkb_filter_pointer_move_func(ସଂରଚନା xkb_server_state *state,
                             ସଂରଚନା xkb_events *events,
                             ସଂରଚନା xkb_filter *filter,
                             ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                             ଗଣନା xkb_ଚାବି_direction direction)
{
    ଯଦି (ଚାବି != filter->ଚାବି || !events)
        ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ଚାବି_EVENT;

    ବାଛ (direction) {
    ସ୍ଥିତି XKB_ଚାବି_DOWN:
        filter->refcnt++;
        append_pointer_move(events, filter);
        ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    ସ୍ଥିତି XKB_ଚାବି_REPEATED:
        ଯଦି (filter->action.ptr.flags & ACTION_REPEAT)
            append_pointer_move(events, filter);
        ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯଦି (--filter->refcnt > 0)
            ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    }

    filter->func = ଖାଲି;
    ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
}

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ସ୍ଥିର ଶୂନ୍ୟ
xkb_filter_pointer_button_new(ସଂରଚନା xkb_server_state *state,
                              ସଂରଚନା xkb_events *events,
                              ସଂରଚନା xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ସ୍ଥିର bool
xkb_filter_pointer_button_func(ସଂରଚନା xkb_server_state *state,
                               ସଂରଚନା xkb_events *events,
                               ସଂରଚନା xkb_filter *filter,
                               ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                               ଗଣନା xkb_ଚାବି_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ସ୍ଥିର ଶୂନ୍ୟ
xkb_filter_pointer_lock_button_new(ସଂରଚନା xkb_server_state *state,
                                   ସଂରଚନା xkb_events *events,
                                   ସଂରଚନା xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ସ୍ଥିର bool
xkb_filter_pointer_lock_button_func(ସଂରଚନା xkb_server_state *state,
                                    ସଂରଚନା xkb_events *events,
                                    ସଂରଚନା xkb_filter *filter,
                                    ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                                    ଗଣନା xkb_ଚାବି_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ସ୍ଥିର ଶୂନ୍ୟ
xkb_filter_pointer_set_default_button_new(ସଂରଚନା xkb_server_state *state,
                                          ସଂରଚନା xkb_events *events,
                                          ସଂରଚନା xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ସ୍ଥିର bool
xkb_filter_pointer_set_default_button_func(ସଂରଚନା xkb_server_state *state,
                                           ସଂରଚନା xkb_events *events,
                                           ସଂରଚନା xkb_filter *filter,
                                           ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                                           ଗଣନା xkb_ଚାବି_direction direction);

ସ୍ଥିର ଶୂନ୍ୟ
xkb_filter_terminate_server_new(ସଂରଚନା xkb_server_state *state,
                                 ସଂରଚନା xkb_events *events,
                                 ସଂରଚନା xkb_filter *filter);

ସ୍ଥିର bool
xkb_filter_terminate_server_func(ସଂରଚନା xkb_server_state *state,
                                  ସଂରଚନା xkb_events *events,
                                  ସଂରଚନା xkb_filter *filter,
                                  ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                                  ଗଣନା xkb_ଚାବି_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ସ୍ଥିର ଶୂନ୍ୟ
xkb_filter_switch_virtual_console_new(ସଂରଚନା xkb_server_state *state,
                                      ସଂରଚନା xkb_events *events,
                                      ସଂରଚନା xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ସ୍ଥିର bool
xkb_filter_switch_virtual_console_func(ସଂରଚନା xkb_server_state *state,
                                       ସଂରଚନା xkb_events *events,
                                       ସଂରଚନା xkb_filter *filter,
                                       ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                                       ଗଣନା xkb_ଚାବି_direction direction);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
clear_all_latches_and_locks(ସଂରଚନା xkb_server_state *state,
                            ସଂରଚନା xkb_events *events);

ସ୍ଥିର ଶୂନ୍ୟ
xkb_filter_ctrls_new(ସଂରଚନା xkb_server_state *state,
                     ସଂରଚନା xkb_events *events,
                     ସଂରଚନା xkb_filter *filter)
{
    ଯଦି (filter->action.type == ACTION_TYPE_CTRL_SET) {
        /* Set: save the specified controls that are *not* already enabled */
        filter->priv = (uint32_t) (
            (~state->base.components.controls) & filter->action.ctrls.ctrls
        );
    } ଅନ୍ୟଥା {
        /* Lock: save the specified controls that *are* already enabled */
        filter->priv = (uint32_t) (
            state->base.components.controls & filter->action.ctrls.ctrls
        );
    }

    ଯଦି (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_LOCK)) {
        /* Enable the specified controls that are not already enabled */
        state->base.components.controls |= filter->action.ctrls.ctrls;
    }
}

ସ୍ଥିର bool
xkb_filter_ctrls_func(ସଂରଚନା xkb_server_state *state,
                      ସଂରଚନା xkb_events *events,
                      ସଂରଚନା xkb_filter *filter,
                      ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                      ଗଣନା xkb_ଚାବି_direction direction)
{
    ଯଦି (ଚାବି != filter->ଚାବି)
        ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;

    ବାଛ (direction) {
    ସ୍ଥିତି XKB_ଚାବି_DOWN:
        filter->refcnt++;
        /* fallthrough */
    ସ୍ଥିତି XKB_ଚାବି_REPEATED:
        ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯଦି (--filter->refcnt > 0)
            ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    }

    ଯଦି (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_UNLOCK)) {
        ଅଚଳ ଗଣନା xkb_action_controls old = state->base.components.controls;

        /*
         * Set: Disable specified controls that were *not* enabled at key press.
         * Lock: Disable specified controls that *were* enabled at key press.
         */
        state->base.components.controls &= ~(ଗଣନା xkb_action_controls) filter->priv;

        ଯଦି ((old & CONTROL_STICKY_ଚାବିଗୁଡ଼ିକ) &&
            !(state->base.components.controls & CONTROL_STICKY_ଚାବିଗୁଡ଼ିକ)) {
            /* Sticky keys were disabled: clear all locks and latches */
            clear_all_latches_and_locks(state, events);
        }
    }

    filter->func = ଖାଲି;
    ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;
}

ସ୍ଥିର bool
append_redirect_ଚାବି_events(ସଂରଚନା xkb_server_state *state,
                           ସଂରଚନା xkb_events *events,
                           ଅଚଳ ସଂରଚନା xkb_redirect_ଚାବି_action *redirect,
                           ଗଣନା xkb_ଚାବି_direction direction)
{
    ଗଣନା xkb_state_component changed = 0;
    ଅଚଳ xkb_mod_mask_t mask = redirect->affect_mods;

    /*
     * Reference state: find the last state update in the queue, otherwise
     * use the current state.
     */
    ସଂରଚନା xkb_event *event;
    ସଂରଚନା state_components last_components = state->base.components;
    darray_foreach_reverse(event, events->queue) {
        ଯଦି (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
            last_components = event->components.components;
            ଭାଙ୍ଗ;
        }
    }

    ଯଦି (mask) {
        ସଂରଚନା state_components new = last_components;
        new.base_mods = (new.base_mods & ~mask) | redirect->mods;
        new.latched_mods = (new.latched_mods & ~mask) | redirect->mods;
        new.locked_mods = (new.locked_mods & ~mask) | redirect->mods;
        new.mods = (new.mods & ~mask) | redirect->mods;

        changed = get_state_component_changes(&last_components, &new);
        ଯଦି (changed) {
            darray_append(events->queue, (ସଂରଚନା xkb_event) {
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
            darray_append(events->queue, (ସଂରଚନା xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_FRAME,
            });
        }
    }

    darray_append(events->queue, (ସଂରଚନା xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_ଚାବି,
        .ଚାବି = {
            .keycode = redirect->keycode,
            .direction = direction
        }
    });

    ଯଦି (mask && changed) {
        /* Restore state */
        darray_append(events->queue, (ସଂରଚନା xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = last_components,
                .changed = changed
            }
        });
    }

    state->flags &= ~STATE_REQUIRE_ଚାବି_EVENT;
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଶୂନ୍ୟ
xkb_filter_redirect_ଚାବି_new(ସଂରଚନା xkb_server_state *state,
                            ସଂରଚନା xkb_events *events,
                            ସଂରଚନା xkb_filter *filter)
{
    /* Action effectual only with the xkb_machine API and a valid keycode */
    ଯଦି (!events || filter->action.redirect.keycode == XKB_KEYCODE_INVALID) {
        filter->func = ଖାଲି;
        ଫେରନ୍ତୁ;
    }
    append_redirect_ଚାବି_events(state, events, &filter->action.redirect,
                               XKB_ଚାବି_DOWN);
}

ସ୍ଥିର bool
xkb_filter_redirect_ଚାବି_func(ସଂରଚନା xkb_server_state *state,
                             ସଂରଚନା xkb_events *events,
                             ସଂରଚନା xkb_filter *filter,
                             ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                             ଗଣନା xkb_ଚାବି_direction direction)
{
    ଯଦି (ଚାବି != filter->ଚାବି)
        ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;

    ଯଦି (direction == XKB_ଚାବି_UP) {
        append_redirect_ଚାବି_events(state, events,
                                   &filter->action.redirect, XKB_ଚାବି_UP);
        filter->func = ଖାଲି;
        ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
     } ଅନ୍ୟଥା ଯଦି (direction == XKB_ଚାବି_DOWN) {
        ଅଚଳ ସଂଘ xkb_action *actions = ଖାଲି;
        ଅଚଳ xkb_action_ଗଣନା_t ଗଣନା_2 =
            xkb_ଚାବି_get_actions(&state->base, ଚାବି, &actions);

        ପାଇଁ (xkb_action_ଗଣନା_t a = 0; a < ଗଣନା_2; a++) {
            ଯଦି (actions[a].type == ACTION_TYPE_REDIRECT_ଚାବି &&
                actions[a].redirect.keycode != filter->action.redirect.keycode) {
                    /* One action redirects to a different key: release first */
                    append_redirect_ଚାବି_events(state, events,
                                               &filter->action.redirect,
                                               XKB_ଚାବି_UP);
                filter->func = ଖାଲି;
                ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;
            }
        }
    }

    append_redirect_ଚାବି_events(state, events, &filter->action.redirect,
                               direction);
    ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
}

ସ୍ଥିର ଅଚଳ ସଂରଚନା {
    ଶୂନ୍ୟ (*new)(ସଂରଚନା xkb_server_state *state,
                ସଂରଚନା xkb_events *events,
                ସଂରଚନା xkb_filter *filter);
    bool (*func)(ସଂରଚନା xkb_server_state *state,
                 ସଂରଚନା xkb_events *events,
                 ସଂରଚନା xkb_filter *filter,
                 ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି, ଗଣନା xkb_ଚାବି_direction direction);
} filter_action_funcs[ACTION_TYPE_ଅଙ୍କ_ENTRIES] = {
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
    [ACTION_TYPE_REDIRECT_ଚାବି] = { xkb_filter_redirect_ଚାବି_new,
                                   xkb_filter_redirect_ଚାବି_func },
};

/**
 * Applies any relevant filters to the key, first from the list of filters
 * that are currently active, then if no filter has claimed the key, possibly
 * apply a new filter from the key action.
 */
ସ୍ଥିର ଶୂନ୍ୟ
xkb_filter_apply_all(ସଂରଚନା xkb_server_state *state,
                     ସଂରଚନା xkb_events *events,
                     ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                     ଗଣନା xkb_ଚାବି_direction direction)
{
    /* First run through all the currently active filters and see if any of
     * them have consumed this event. */
    bool consumed = false;
    ସଂରଚନା xkb_filter *filter;
    darray_foreach(filter, state->filters) {
        ଯଦି (!filter->func)
            ଜାରି;

        ଯଦି (filter->func(state, events, filter, ଚାବି, direction) == XKB_FILTER_CONSUME)
            consumed = true;
    }
    ଯଦି (consumed || direction == XKB_ଚାବି_UP)
        ଫେରନ୍ତୁ;

    /* No filter consumed this event, so proceed with the key actions */
    ଅଚଳ ସଂଘ xkb_action *actions = ଖାଲି;
    xkb_action_ଗଣନା_t ଗଣନା_2 = xkb_ଚାବି_get_actions(&state->base, ଚାବି,
                                                         &actions);

    ପାଇଁ (ସଂରଚନା {
            xkb_action_ଗଣନା_t ଗଣନା_2;
            ଅଚଳ ସଂଘ xkb_action *actions;
         } next = {0};
         ଗଣନା_2;
         ଗଣନା_2 = next.ଗଣନା_2, next.ଗଣନା_2 = 0, actions = next.actions
    ) {
        /*
         * Process actions sequentially.
         *
         * NOTE: We rely on the parser to disallow multiple modifier or group
         * actions (see `CheckMultipleActionsCategories`). Allowing multiple
         * such actions requires a refactor of the state handling.
         */
        ପାଇଁ (xkb_action_ଗଣନା_t k = 0; k < ଗଣନା_2; k++) {
            /*
             * It's possible for the keymap to set action->type explicitly, like
             * so:
             *     interpret XF86_Next_VMode {
             *         action = Private(type=0x86, data="+VMode");
             *     };
             * We don't handle those.
             */
            ଯଦି (actions[k].type >= ACTION_TYPE_ଅଙ୍କ_ENTRIES)
                ଜାରି;

            /* Go to next action if no corresponding action handler */
            ଯଦି (!filter_action_funcs[actions[k].type].new)
                ଜାରି;

            /* Add a new filter and run the corresponding initial action */
            filter = xkb_filter_new(state);
            filter->ଚାବି = ଚାବି;
            filter->action = actions[k];

            ବାଛ (filter->action.type) {
            ସ୍ଥିତି ACTION_TYPE_MOD_SET:
                ଯଦି (state->base.components.controls & CONTROL_STICKY_ଚାବିଗୁଡ଼ିକ) {
                    /* Convert modifier set action to a latch */
                    filter->action.type = ACTION_TYPE_MOD_LATCH;
                    ଯଦି (state->a11y_flags & XKB_A11_Y_STICKY_ଚାବିଗୁଡ଼ିକ_LATCH_TO_LOCK) {
                        filter->action.mods.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                /* fallthrough */
            ସ୍ଥିତି ACTION_TYPE_MOD_LATCH:
            ସ୍ଥିତି ACTION_TYPE_MOD_LOCK: {
                ସ୍ଥିର ଅଚଳ ଗଣନା xkb_action_flags unlock_mods_on_press
                    = ACTION_LOCK_CLEAR     /* SetMods & LatchMods: can unlock */
                    | ACTION_LOCK_NO_UNLOCK /* LockMods: cannot unlock */
                    | ACTION_EXPLICIT_UNLOCK_ON_PRESS;
                ଯଦି ((state->flags & STATE_UNLOCK_MODS_ON_PRESS) &&
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
                ଭାଙ୍ଗ;
            }
            ସ୍ଥିତି ACTION_TYPE_GROUP_SET:
                ଯଦି (state->base.components.controls & CONTROL_STICKY_ଚାବିଗୁଡ଼ିକ) {
                    /* Convert group set action to a latch */
                    filter->action.type = ACTION_TYPE_GROUP_LATCH;
                    ଯଦି (state->a11y_flags & XKB_A11_Y_STICKY_ଚାବିଗୁଡ଼ିକ_LATCH_TO_LOCK) {
                        filter->action.group.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                ଭାଙ୍ଗ;
            ସ୍ଥିତି ACTION_TYPE_GROUP_LOCK:
                ଯଦି ((filter->action.group.flags &
                     ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE) &&
                    (state->flags &
                     STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_ଚାବି) &&
                    /*
                     * FIXME: sticky keys may leave an unexpected modifier
                     * from the level 0 latched, so disable for now.
                     */
                    !(state->base.components.controls & CONTROL_STICKY_ଚାବିଗୁଡ଼ିକ))
                {
                    filter->action.group.flags |= ACTION_LOCK_ON_RELEASE;
                    /* Add base level action */
                    next.ଗଣନା_2 = xkb_keymap_ଚାବି_get_actions_by_level(
                        state->base.keymap, ଚାବି, state->base.components.group,
                        0, &next.actions
                    );
                    ପ୍ରତିଜ୍ଞା(next.ଗଣନା_2 == 1);
                }
                ଭାଙ୍ଗ;
            ସ୍ଥିତି ACTION_TYPE_PTR_MOVE:
            ସ୍ଥିତି ACTION_TYPE_PTR_BUTTON:
            ସ୍ଥିତି ACTION_TYPE_PTR_LOCK:
            ସ୍ଥିତି ACTION_TYPE_PTR_DEFAULT:
                ଯଦି (!(state->base.components.controls & CONTROL_MOUSE_ଚାବିଗୁଡ଼ିକ)) {
                    /* Convert pointer actions to NoAction */
                    ପ୍ରତିଜ୍ଞା(!filter_action_funcs[ACTION_TYPE_NONE].new);
                    filter->func = ଖାଲି;
                    ଜାରି;
                }
                ଭାଙ୍ଗ;
            ସ୍ଥିତି ACTION_TYPE_REDIRECT_ଚାବି:
                // FIXME: this is not efficient to resolve mods here each time
                filter->action.redirect.affect_mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.affect_mods
                );
                filter->action.redirect.mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.mods
                );
                ଭାଙ୍ଗ;
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ;
            }
            filter->func = filter_action_funcs[filter->action.type].func;
            filter_action_funcs[filter->action.type].new(state, events, filter);
        }
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
xkb_state_update_derived(ସଂରଚନା xkb_state *state);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
xkb_state_init(ସଂରଚନା xkb_state * restrict state,
               ସଂରଚନା xkb_keymap * restrict keymap,
               ଗଣନା xkb_state_mode_internal mode)
{
    state->mode = mode;
    state->refcnt = 1;
    state->keymap = xkb_keymap_ref(keymap);
    state->out_of_range_group.policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

    /* Ensure that derived state is correctly initialized */
    xkb_state_update_derived(state);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
xkb_client_state_init(ସଂରଚନା xkb_client_state * restrict state,
                      ସଂରଚନା xkb_keymap * restrict keymap,
                      ଗଣନା xkb_state_mode_internal mode)
{
    xkb_state_init(&state->base, keymap, mode);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
xkb_server_state_init(ସଂରଚନା xkb_server_state * restrict state,
                      ସଂରଚନା xkb_keymap * restrict keymap,
                      ଗଣନା xkb_state_mode_internal mode,
                      ଗଣନା xkb_a11y_flags a11y_affect,
                      ଗଣନା xkb_a11y_flags a11y_flags)
{
    xkb_state_init(&state->base, keymap, mode);

    state->a11y_flags = a11y_flags;
    ଯଦି (keymap->format != XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !(a11y_affect & XKB_A11_Y_LATCH_SIMULTANEOUS_ଚାବିଗୁଡ଼ିକ)) {
             /* Keymap v2+: enable extension to XKB if not manually disabled */
             state->a11y_flags |= XKB_A11_Y_LATCH_SIMULTANEOUS_ଚାବିଗୁଡ଼ିକ;
    }
}

ସଂରଚନା xkb_state *
xkb_state_new(ସଂରଚନା xkb_keymap *keymap)
{
    ସଂରଚନା xkb_server_state * ଅଚଳ state = calloc(1, ଆକାର(*state));
    ଯଦି (!state) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "Could not allocate a state object.\n");
        ଫେରନ୍ତୁ ଖାଲି;
    }

    xkb_server_state_init(state, keymap, LEGACY_MIXED_STATE, 0, 0);

    ଫେରନ୍ତୁ (ସଂରଚନା xkb_state *)state;
}

ସଂରଚନା xkb_state *
xkb_state_new_with_mode(ସଂରଚନା xkb_keymap * restrict keymap,
                        ଗଣନା xkb_state_mode mode,
                        ଗଣନା xkb_status * restrict status)
{
    ବାଛ (mode) {
    ସ୍ଥିତି XKB_STATE_MODE_CLIENT:
    ସ୍ଥିତି XKB_STATE_MODE_SERVER_QUERY: {
        ସଂରଚନା xkb_client_state * ଅଚଳ state = calloc(1, ଆକାର(*state));
        ଯଦି (!state)
            ଭାଙ୍ଗ;

        static_assert((unsigned)XKB_STATE_MODE_CLIENT ==
                      (unsigned)CLIENT_STATE, "");
        static_assert((unsigned)XKB_STATE_MODE_SERVER_QUERY ==
                      (unsigned)SERVER_COMPANION, "");
        xkb_client_state_init(state, keymap, (ଗଣନା xkb_state_mode_internal)mode);

        ଯଦି (status)
            *status = XKB_SUCCESS;

        ଫେରନ୍ତୁ (ସଂରଚନା xkb_state *)state;
    }
    ସ୍ଥିତି XKB_STATE_MODE_SERVER: {
        ସଂରଚନା xkb_server_state * ଅଚଳ state = calloc(1, ଆକାର(*state));
        ଯଦି (!state)
            ଭାଙ୍ଗ;

        xkb_server_state_init(state, keymap, LEGACY_SERVER_STATE, 0, 0);

        ଯଦି (status)
            *status = XKB_SUCCESS;

        ଫେରନ୍ତୁ (ସଂରଚନା xkb_state *)state;
    }
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_STATE_MODE_,
                     "Unsupported state mode: %u\n", mode);
        ଯଦି (status)
            *status = XKB_ERROR_UNSUPPORTED_STATE_MODE;
        ଫେରନ୍ତୁ ଖାଲି;
    }

    log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                  "Could not allocate a state object.\n");
    ଯଦି (status)
        *status = XKB_ERROR_ALLOCATION_FAILURE;
    ଫେରନ୍ତୁ ଖାଲି;
}

ସଂରଚନା xkb_state *
xkb_state_ref(ସଂରଚନା xkb_state *state)
{
    ପ୍ରତିଜ୍ଞା(state->refcnt > 0);
    state->refcnt++;
    ଫେରନ୍ତୁ state;
}

ଶୂନ୍ୟ
xkb_state_reset(ସଂରଚନା xkb_state *state)
{
    memset(&state->components, 0, ଆକାର(state->components));

    ଯଦି (state->mode < LEGACY_MIXED_STATE) {
        static_assert(ଆକାର(*state) == ଆକାର(ସଂରଚନା xkb_client_state),
                      "Not a mere wrapper");
        /* Nothing else to reset */
    } ଅନ୍ୟଥା {
        ସଂରଚନା xkb_server_state *state_ = (ସଂରଚନା xkb_server_state *)state;
        ଅଚଳ ସଂରଚନା xkb_server_state prev = *state_;
        memset(state_, 0, ଆକାର(*state_));

        state_->base = prev.base;
        state_->a11y_flags = prev.a11y_flags;
        state_->filters = prev.filters;
        darray_size(state_->filters) = 0;
    }
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
xkb_state_destroy(ସଂରଚନା xkb_state *state)
{
    xkb_keymap_unref(state->keymap);
    ଯଦି (state->mode > SERVER_COMPANION)
        darray_free(((ସଂରଚନା xkb_server_state *)state)->filters);
}

ଶୂନ୍ୟ
xkb_state_unref(ସଂରଚନା xkb_state *state)
{
    ପ୍ରତିଜ୍ଞା(!state || state->refcnt > 0);
    ଯଦି (!state || --state->refcnt > 0)
        ଫେରନ୍ତୁ;

    xkb_state_destroy(state);

    ବାଛ (state->mode) {
    ସ୍ଥିତି CLIENT_STATE:
    ସ୍ଥିତି SERVER_COMPANION:
        ମୁକ୍ତ((ସଂରଚନା xkb_client_state *)state);
        ଭାଙ୍ଗ;
    ସ୍ଥିତି LEGACY_MIXED_STATE:
    ସ୍ଥିତି LEGACY_SERVER_STATE:
        ମୁକ୍ତ((ସଂରଚନା xkb_server_state *)state);
        ଭାଙ୍ଗ;
    ସ୍ଥିତି SERVER_STATE: /* handled by xkb_machine_unref() */
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        PANIC_UNREACHABLE();
    }
}

ସଂରଚନା xkb_keymap *
xkb_state_get_keymap(ସଂରଚନା xkb_state *state)
{
    ଫେରନ୍ତୁ state->keymap;
}

/**
 * Update the LED state to match the rest of the xkb_state.
 */
ସ୍ଥିର ଶୂନ୍ୟ
xkb_state_led_update_all(ସଂରଚନା xkb_state *state)
{
    xkb_led_index_t idx;
    ଅଚଳ ସଂରଚନା xkb_led *led;

    state->components.leds = 0;

    xkb_leds_enumerate(idx, led, state->keymap) {

        ଯଦି (led->which_mods != 0 && led->mods.mask != 0) {
            xkb_mod_mask_t mod_mask = 0;
            ଯଦି (led->which_mods & XKB_STATE_MODS_EFFECTIVE)
                mod_mask |= state->components.mods;
            ଯଦି (led->which_mods & XKB_STATE_MODS_DEPRESSED)
                mod_mask |= state->components.base_mods;
            ଯଦି (led->which_mods & XKB_STATE_MODS_LATCHED)
                mod_mask |= state->components.latched_mods;
            ଯଦି (led->which_mods & XKB_STATE_MODS_LOCKED)
                mod_mask |= state->components.locked_mods;

            ଯଦି (led->mods.mask & mod_mask) {
                state->components.leds |= (UINT32_C(1) << idx);
                ଜାରି;
            }
        }

        ଯଦି (led->which_groups != 0) {
            ଯଦି (likely(led->groups) != 0) {
                xkb_layout_mask_t group_mask = 0;
                /* Effective and locked groups have been brought into range */
                ପ୍ରତିଜ୍ଞା(state->components.group < XKB_MAX_GROUPS);
                ପ୍ରତିଜ୍ଞା(state->components.locked_group >= 0 &&
                       state->components.locked_group < XKB_MAX_GROUPS);
                /* Effective and locked groups are used as mask */
                ଯଦି (led->which_groups & XKB_STATE_LAYOUT_EFFECTIVE)
                    group_mask |= (UINT32_C(1) << state->components.group);
                ଯଦି (led->which_groups & XKB_STATE_LAYOUT_LOCKED)
                    group_mask |= (UINT32_C(1) << state->components.locked_group);
                /* Base and latched groups only have to be non-zero */
                ଯଦି ((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                    state->components.base_group != 0)
                    group_mask |= led->groups;
                ଯଦି ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                    state->components.latched_group != 0)
                    group_mask |= led->groups;

                ଯଦି (led->groups & group_mask) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    ଜାରି;
                }
            } ଅନ୍ୟଥା {
                /* Special case for Base and latched groups */
                ଯଦି (((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                     state->components.base_group == 0) ||
                    ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                     state->components.latched_group == 0)) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    ଜାରି;
                }
            }
        }

        ଯଦି (led->ctrls & state->components.controls) {
            state->components.leds |= (UINT32_C(1) << idx);
            ଜାରି;
        }
    }
}

/**
 * Calculates the derived state (effective mods/group and LEDs) from an
 * up-to-date xkb_state.
 */
ସ୍ଥିର ଶୂନ୍ୟ
xkb_state_update_derived(ସଂରଚନା xkb_state *state)
{
    xkb_layout_index_t wrapped;

    state->components.mods = (state->components.base_mods |
                              state->components.latched_mods |
                              state->components.locked_mods);

    /* Lock group must be adjusted, but not base nor latched groups */
    wrapped = XkbWrapGroupIntoRange(state->components.locked_group,
                                    state->keymap->ଅଙ୍କ_groups,
                                    state->out_of_range_group.policy,
                                    state->out_of_range_group.redirect_group);
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->components.locked_group =
        (int32_t) (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    /* Effective group must be adjusted */
    wrapped = XkbWrapGroupIntoRange(state->components.base_group +
                                    state->components.latched_group +
                                    state->components.locked_group,
                                    state->keymap->ଅଙ୍କ_groups,
                                    state->out_of_range_group.policy,
                                    state->out_of_range_group.redirect_group);
    state->components.group =
        (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    xkb_state_led_update_all(state);
}

ସ୍ଥିର ଶୂନ୍ୟ
state_update_enabled_controls(ସଂରଚନା xkb_server_state *state,
                              ଗଣନା xkb_keyboard_control_flags affect_controls,
                              ଗଣନା xkb_keyboard_control_flags controls,
                              ସଂରଚନା xkb_events *events)
{
    ଅଚଳ bool had_sticky_ଚାବିଗୁଡ଼ିକ = state->base.components.controls
                               & CONTROL_STICKY_ଚାବିଗୁଡ଼ିକ;

    /*
     * Enable using the public API with the all the Control values, except
     * the internal ones, if any.
     */
    affect_controls =
        ( affect_controls
        & (ଗଣନା xkb_keyboard_control_flags)CONTROL_ALL_BOOLEAN );
    state->base.components.controls &=
        (ଗଣନା xkb_action_controls)~affect_controls;
    state->base.components.controls |=
        (ଗଣନା xkb_action_controls)(controls & affect_controls);

    ଯଦି (had_sticky_ଚାବିଗୁଡ଼ିକ &&
        !(state->base.components.controls & CONTROL_STICKY_ଚାବିଗୁଡ଼ିକ)) {
        /* Sticky keys were disabled: clear all locks and latches */
        clear_all_latches_and_locks(state, events);
    }

    xkb_state_update_derived(&state->base);
}

/**
 * Given a particular key event, updates the state structure to reflect the
 * new modifiers.
 */
ଗଣନା xkb_state_component
xkb_state_update_ଚାବି(ସଂରଚନା xkb_state *base_state, xkb_keycode_t kc,
                     ଗଣନା xkb_ଚାବି_direction direction)
{
    /* Guard against client-only state */
    ପ୍ରତିଜ୍ଞା(base_state->mode > SERVER_COMPANION);
    ଯଦି (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        ଫେରନ୍ତୁ 0;
    }

    ସଂରଚନା xkb_server_state * ଅଚଳ state =
        (ସଂରଚନା xkb_server_state *)base_state;

    ଅଚଳ ସଂରଚନା xkb_ଚାବି* ଅଚଳ ଚାବି = Xkb_ଚାବି(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    ଯଦି (!ଚାବି || (direction == XKB_ଚାବି_REPEATED && !ଚାବି->repeats))
        ଫେରନ୍ତୁ 0;

    ଯଦି (direction == XKB_ଚାବି_DOWN &&
        (state->base.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ଚାବିଗୁଡ଼ିକ) &&
        (state->a11y_flags & XKB_A11_Y_STICKY_ଚାବିଗୁଡ଼ିକ_NO_SIMULTANEOUS_ଚାବିଗୁଡ଼ିକ) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ଚାବିଗୁଡ଼ିକ,
                                      0, ଖାଲି);
    }

    ଅଚଳ ସଂରଚନା state_components prev_components = state->base.components;

    state->set_mods = 0;
    state->clear_mods = 0;

    xkb_filter_apply_all(state, ଖାଲି, ଚାବି, direction);

    xkb_mod_index_t i;
    xkb_mod_mask_t bit;
    ପାଇଁ (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        ଯଦି (state->set_mods & bit) {
            state->mod_ଚାବି_ଗଣନା[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    ପାଇଁ (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        ଯଦି (state->clear_mods & bit) {
            state->mod_ଚାବି_ଗଣନା[i]--;
            ଯଦି (state->mod_ଚାବି_ଗଣନା[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_ଚାବି_ଗଣନା[i] = 0;
            }
            state->clear_mods &= ~bit;
        }
    }

    xkb_state_update_derived(&state->base);

    ଫେରନ୍ତୁ get_state_component_changes(&prev_components, &state->base.components);
}

/* We need fake keys for `update_latch_modifiers` and `update_latch_group`.
 * These keys must have at least one level in order to break latches. We need 2
 * keys with specific actions in order to update group/mod latches without
 * affecting each other. */
ସ୍ଥିର ସଂରଚନା xkb_ଚାବି_type_entry synthetic_ଚାବି_level_entry = { 0 };
ସ୍ଥିର ସଂରଚନା xkb_ଚାବି_type synthetic_ଚାବି_type = {
     .ଅଙ୍କ_entries = 1,
     .ଅଙ୍କ_levels = 1,
     .entries = &synthetic_ଚାବି_level_entry
};
ସ୍ଥିର ଅଚଳ ସଂରଚନା xkb_ଚାବି synthetic_ଚାବି = { 0 };

/* Transcription from xserver: XkbLatchModifiers */
ସ୍ଥିର ଶୂନ୍ୟ
update_latch_modifiers(ସଂରଚନା xkb_server_state *state,
                       ସଂରଚନା xkb_events *events,
                       xkb_mod_mask_t mask, xkb_mod_mask_t latches)
{
    /* Clear affected latched modifiers */
    ଅଚଳ xkb_mod_mask_t clear = mask & ~latches;
    state->base.components.latched_mods &= ~clear;

    /* Clear any pending latch to locks using ad hoc action:
     * only affect corresponding modifier latches and no group latch. */
    ସଂରଚନା xkb_level synthetic_ଚାବି_level_break_mod_latch = {
        .ଅଙ୍କ_syms = 0,
        .ଅଙ୍କ_actions = 1,
        .upper = XKB_ଚାବି_No_ପ୍ରତୀକ,
        .s.sym = XKB_ଚାବି_No_ପ୍ରତୀକ,
        .a.action.internal = {
            .type = ACTION_TYPE_INTERNAL,
            .flags = INTERNAL_BREAKS_MOD_LATCH,
            .clear_latched_mods = clear
        }
    };
    ସଂରଚନା xkb_group synthetic_ଚାବି_group_break_mod_latch = {
        .type = &synthetic_ଚାବି_type,
        .levels = &synthetic_ଚାବି_level_break_mod_latch
    };
    ଅଚଳ ସଂରଚନା xkb_ଚାବି synthetic_ଚାବି_break_mod_latch = {
        .ଅଙ୍କ_groups = 1,
        .groups = &synthetic_ଚାବି_group_break_mod_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_ଚାବି_break_mod_latch, XKB_ଚାବି_DOWN);

    /* Finally set the latched mods by simulate tapping a key with the
     * corresponding action */
    ଅଚଳ ସଂରଚନା xkb_ଚାବି* ଅଚଳ ଚାବି = &synthetic_ଚାବି;
    ଅଚଳ ସଂଘ xkb_action latch_mods = {
        .mods = {
            .type = ACTION_TYPE_MOD_LATCH,
            .mods = { .mask = mask & latches },
            .flags = 0,
        },
    };
    ସଂରଚନା xkb_filter* ଅଚଳ filter = xkb_filter_new(state);
    filter->ଚାବି = ଚାବି;
    filter->func = xkb_filter_mod_latch_func;
    filter->action = latch_mods;
    xkb_filter_mod_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_mod_latch_func(state, events, filter, ଚାବି, XKB_ଚାବି_UP);
}

/* Transcription from xserver: XkbLatchGroup */
ସ୍ଥିର ଶୂନ୍ୟ
update_latch_group(ସଂରଚନା xkb_server_state *state,
                   ସଂରଚନା xkb_events *events, int32_t group)
{
    /* Clear any pending latch to locks. */
    ସ୍ଥିର ସଂରଚନା xkb_level synthetic_ଚାବି_level_break_group_latch = {
        .ଅଙ୍କ_syms = 0,
        .ଅଙ୍କ_actions = 1,
        .upper = XKB_ଚାବି_No_ପ୍ରତୀକ,
        .s.sym = XKB_ଚାବି_No_ପ୍ରତୀକ,
        .a.action.internal = {
            .type = ACTION_TYPE_INTERNAL,
            .flags = INTERNAL_BREAKS_GROUP_LATCH,
            .clear_latched_mods = 0
        }
    };
    ସ୍ଥିର ସଂରଚନା xkb_group synthetic_ଚାବି_group_break_group_latch = {
        .type = &synthetic_ଚାବି_type,
        .levels = &synthetic_ଚାବି_level_break_group_latch
    };
    ସ୍ଥିର ଅଚଳ ସଂରଚନା xkb_ଚାବି synthetic_ଚାବି_break_group_latch = {
        .ଅଙ୍କ_groups = 1,
        .groups = &synthetic_ଚାବି_group_break_group_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_ଚାବି_break_group_latch, XKB_ଚାବି_DOWN);

    /* Simulate tapping a key with a group latch action, but in isolation: i.e.
     * without affecting the other filters. */
    ଅଚଳ ସଂରଚନା xkb_ଚାବି* ଅଚଳ ଚାବି = &synthetic_ଚାବି;
    ଅଚଳ ସଂଘ xkb_action latch_group = {
        .group = {
            .type = ACTION_TYPE_GROUP_LATCH,
            .flags = ACTION_ABSOLUTE_SWITCH,
            .group = group,
        },
    };
    ସଂରଚନା xkb_filter* ଅଚଳ filter = xkb_filter_new(state);
    filter->ଚାବି = ଚାବି;
    filter->func = xkb_filter_group_latch_func;
    filter->action = latch_group;
    xkb_filter_group_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_group_latch_func(state, events, filter, ଚାବି, XKB_ଚାବି_UP);
}

/**
 * Compute the resolved effective mask of an arbitrary input.
 *
 * Contrary to `mod_mask_get_effective`, it resolves only modifiers not present
 * in the canonical mask, so that it enables `xkb_state_serialize_mods` to
 * round trip via `xkb_state_update_mask`.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ xkb_mod_mask_t
resolve_to_canonical_mods(ସଂରଚନା xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    ଫେରନ୍ତୁ
        /*
         * Keep canonical modifier mask.
         * It contains either real modifiers or canonical virtual modifiers.
         */
        (mods & keymap->canonical_state_mask) |
        /* Resolve other modifiers */
        mod_mask_get_effective(keymap,
                               mods & ~keymap->canonical_state_mask);
}

ସ୍ଥିର ଶୂନ୍ୟ
state_update_latched_locked(
    ସଂରଚନା xkb_server_state * restrict state,
    ଅଚଳ ସଂରଚନା xkb_state_components_update * restrict update,
    ସଂରଚନା xkb_events * restrict events
)
{
    /* Update locks */
    ଅଚଳ xkb_mod_mask_t affect_locked_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_locked_mods);
    ଯଦି ((update->components & XKB_STATE_MODS_LOCKED) &&
        affect_locked_mods) {
        ଅଚଳ xkb_mod_mask_t locked_mods =
            resolve_to_canonical_mods(state->base.keymap, update->locked_mods);
        state->base.components.locked_mods &= ~affect_locked_mods;
        state->base.components.locked_mods |= locked_mods & affect_locked_mods;
    }
    ଯଦି (update->components & XKB_STATE_LAYOUT_LOCKED) {
        state->base.components.locked_group = update->locked_layout;
    }

    /* Update latches */
    ଅଚଳ xkb_mod_mask_t affect_latched_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_latched_mods);
    ଯଦି ((update->components & XKB_STATE_MODS_LATCHED) &&
        affect_latched_mods) {
        ଅଚଳ xkb_mod_mask_t latched_mods =
            resolve_to_canonical_mods(state->base.keymap, update->latched_mods);
        update_latch_modifiers(state, events, affect_latched_mods, latched_mods);
    }
    ଯଦି (update->components & XKB_STATE_LAYOUT_LATCHED) {
        update_latch_group(state, events, update->latched_layout);
    }
}

ଗଣନା xkb_state_component
xkb_state_update_latched_locked(ସଂରଚନା xkb_state *base_state,
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
    ପ୍ରତିଜ୍ଞା(base_state->mode > SERVER_COMPANION);
    ଯଦି (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        ଫେରନ୍ତୁ 0;
    }

    ସଂରଚନା xkb_server_state * ଅଚଳ state =
        (ସଂରଚନା xkb_server_state *)base_state;

    ଅଚଳ ସଂରଚନା state_components previous_components = state->base.components;
    ଅଚଳ ଗଣନା xkb_state_component components
        = ((affect_latched_mods || latched_mods) ? XKB_STATE_MODS_LATCHED : 0)
        | ((affect_locked_mods || locked_mods) ? XKB_STATE_MODS_LOCKED : 0)
        | (affect_latched_layout ? XKB_STATE_LAYOUT_LATCHED : 0)
        | (affect_locked_layout ? XKB_STATE_LAYOUT_LOCKED : 0);
    ଅଚଳ ସଂରଚନା xkb_state_components_update update = {
        .size = ଆକାର(update),
        .components = components,
        .affect_latched_mods = affect_latched_mods,
        .latched_mods = latched_mods,
        .latched_layout = latched_layout,
        .affect_locked_mods = affect_locked_mods,
        .locked_mods = locked_mods,
        .locked_layout = locked_layout
    };
    state_update_latched_locked(state, &update, ଖାଲି);
    xkb_state_update_derived(&state->base);
    ଫେରନ୍ତୁ get_state_component_changes(&previous_components,
                                       &state->base.components);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
clear_all_latches_and_locks(ସଂରଚନା xkb_server_state *state,
                            ସଂରଚନା xkb_events *events)
{
    ସ୍ଥିର ଅଚଳ ଗଣନା xkb_state_component components
        = XKB_STATE_MODS_LATCHED | XKB_STATE_MODS_LOCKED
        | XKB_STATE_LAYOUT_LATCHED | XKB_STATE_LAYOUT_LOCKED;
    ଅଚଳ ସଂରଚନା xkb_state_components_update update = {
        .size = ଆକାର(update),
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

ସ୍ଥିର ଗଣନା xkb_status
state_update_layout_policy(ସଂରଚନା xkb_server_state *state,
                           ଅଚଳ ସଂରଚନା xkb_layout_policy_update *update)
{
    ଯଦି (xkb_feature_supported(XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY,
                              update->policy)) {
        ଯଦି (update->policy == XKB_LAYOUT_OUT_OF_RANGE_REDIRECT) {
            ଯଦି (update->redirect < state->base.keymap->ଅଙ୍କ_groups) {
                state->base.out_of_range_group.redirect_group =
                    update->redirect;
            } ଅନ୍ୟଥା {
                log_err(state->base.keymap->ctx,
                        XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Layout policy: "
                        "unsupported layout index %"PRIu32" > %"PRIu32"\n",
                        update->redirect + 1, state->base.keymap->ଅଙ୍କ_groups);
                ଫେରନ୍ତୁ XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
            }
        }
        state->base.out_of_range_group.policy = update->policy;
        ଫେରନ୍ତୁ XKB_SUCCESS;
    } ଅନ୍ୟଥା {
        log_err(state->base.keymap->ctx,
                XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY_,
                "Unsupported layout policy: %d\n", update->policy);
        ଫେରନ୍ତୁ XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY;
    }
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
ସ୍ଥିର ଗଣନା xkb_status
check_state_update_abi_(ସଂରଚନା xkb_context * restrict ctx,
                        ଅଚଳ ଅକ୍ଷର * restrict func,
                        ଅଚଳ ସଂରଚନା xkb_synthetic_update * restrict update)
{
    ଗଣନା xkb_status status = XKB_SUCCESS;
    ଯଦି ((status = xkb_check_state_abi(update)) ||
        (update->reserved0 != 0 &&
         (status = XKB_ERROR_ABI_FORWARD_COMPAT)) ||
        (update->components &&
         (status = xkb_check_state_abi(update->components))) ||
        (update->layout_policy &&
         (status = xkb_check_state_abi(update->layout_policy)))) {
        xkb_log_abi_error(ctx, func, status);
    }
    ଫେରନ୍ତୁ status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

#define check_state_update_abi(ctx, update) \
    check_state_update_abi_(ctx, __func__, update)

ଗଣନା xkb_status
xkb_state_update_synthetic(ସଂରଚନା xkb_state * base_state,
                           ଅଚଳ ସଂରଚନା xkb_synthetic_update * update,
                           ଗଣନା xkb_state_component *changed)
{
    /* Guard against client-only state */
    ଯଦି (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        ଫେରନ୍ତୁ XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    ସଂରଚନା xkb_server_state * ଅଚଳ state =
        (ସଂରଚନା xkb_server_state *)base_state;

    /* Check ABI compatibility */
    ଗଣନା xkb_status status = check_state_update_abi(state->base.keymap->ctx,
                                                    update);
    ଯଦି (status != XKB_SUCCESS)
        ଫେରନ୍ତୁ status;

    ଅଚଳ ସଂରଚନା state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    /* Update parametrized controls first */
    ଯଦି (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        ଯଦି (status != XKB_SUCCESS)
            ଫେରନ୍ତୁ status;
    }

    ଯଦି (update->components) {
        ଅଚଳ ସଂରଚନା xkb_state_components_update * ଅଚଳ components =
            update->components;
        /* Update boolean controls first */
        ଯଦି (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, ଖାଲି);
        }

        state_update_latched_locked(state, components, ଖାଲି);
    }

    xkb_state_update_derived(&state->base);

    ଯଦି (changed) {
        *changed = get_state_component_changes(&previous_components,
                                               &state->base.components);
    }
    ଫେରନ୍ତୁ XKB_SUCCESS;
}

/**
 * Updates the state from a set of explicit masks as gained from
 * xkb_state_serialize_mods and xkb_state_serialize_groups.  As noted in the
 * documentation for these functions in xkbcommon.h, this round-trip is
 * lossy, and should only be used to update a slave state mirroring the
 * master, e.g. in a client/server window system.
 */
ଗଣନା xkb_state_component
xkb_state_update_mask(ସଂରଚନା xkb_state *base_state,
                      xkb_mod_mask_t base_mods,
                      xkb_mod_mask_t latched_mods,
                      xkb_mod_mask_t locked_mods,
                      xkb_layout_index_t base_group,
                      xkb_layout_index_t latched_group,
                      xkb_layout_index_t locked_group)
{
    /* Guard against server-only and server companion state */
    ପ୍ରତିଜ୍ଞା(base_state->mode == CLIENT_STATE ||
           base_state->mode == LEGACY_MIXED_STATE);
    ଯଦି (base_state->mode != CLIENT_STATE &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        ଫେରନ୍ତୁ 0;
    }

    ସଂରଚନା xkb_client_state * ଅଚଳ state =
        (ସଂରଚନା xkb_client_state *)base_state;

    ଅଚଳ ସଂରଚନା state_components prev_components = state->base.components;

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

    ଫେରନ୍ତୁ get_state_component_changes(&prev_components, &state->base.components);
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Lock_Modifier
 */
ସ୍ଥିର bool
should_do_caps_transformation(ସଂରଚନା xkb_state *state, xkb_keycode_t kc)
{
    ଫେରନ୍ତୁ
        xkb_state_mod_index_is_active(state, XKB_MOD_INDEX_CAPS,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_index_is_consumed(state, kc, XKB_MOD_INDEX_CAPS) == 0;
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Control_Modifier
 */
ସ୍ଥିର bool
should_do_ctrl_transformation(ସଂରଚନା xkb_state *state, xkb_keycode_t kc)
{
    ଫେରନ୍ତୁ
        xkb_state_mod_index_is_active(state, XKB_MOD_INDEX_CTRL,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_index_is_consumed(state, kc, XKB_MOD_INDEX_CTRL) == 0;
}

/**
 * Provides the symbols to use for the given key and state.  Returns the
 * number of symbols pointed to in syms_out.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_state_ଚାବି_get_syms(ସଂରଚନା xkb_state *state, xkb_keycode_t kc,
                       ଅଚଳ xkb_keysym_t **syms_out)
{
    ଅଚଳ xkb_layout_index_t layout = xkb_state_ଚାବି_get_layout(state, kc);
    ଯଦି (layout == XKB_LAYOUT_INVALID)
        ଯାଅ err;

    ଅଚଳ xkb_level_index_t level = xkb_state_ଚାବି_get_level(state, kc, layout);
    ଯଦି (level == XKB_LEVEL_INVALID)
        ଯାଅ err;

    ଅଚଳ ସଂରଚନା xkb_ଚାବି* ଅଚଳ ଚାବି = Xkb_ଚାବି(state->keymap, kc);

    ଯଦି (!ଚାବି)
        ଯାଅ err;

    ଅଚଳ ସଂରଚନା xkb_level* ଅଚଳ leveli =
        xkb_keymap_ଚାବି_get_level(state->keymap, ଚାବି, layout, level);

    ଯଦି (!leveli)
        ଯାଅ err;

    ଅଚଳ xkb_keysym_ଗଣନା_t ଅଙ୍କ_syms = leveli->ଅଙ୍କ_syms;
    ଯଦି (ଅଙ୍କ_syms == 0)
        ଯାଅ err;

    ଯଦି (should_do_caps_transformation(state, kc)) {
        /* Only simple capitalization rules: keysyms count is unchanged. */
        ଯଦି (ଅଙ୍କ_syms > 1) {
            *syms_out = (leveli->has_upper)
                      ? leveli->s.syms + ଅଙ୍କ_syms
                      : leveli->s.syms;
        } ଅନ୍ୟଥା {
            *syms_out = &leveli->upper;
        }
    } ଅନ୍ୟଥା {
        *syms_out = (ଅଙ୍କ_syms > 1)
                  ? leveli->s.syms
                  : &leveli->s.sym;
    }
    ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଅଙ୍କ_syms;

err:
    *syms_out = ଖାଲି;
    ଫେରନ୍ତୁ 0;
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
ସ୍ଥିର ଅକ୍ଷର
XkbToControl(ଅକ୍ଷର ch)
{
    ଅକ୍ଷର c = ch;

    ଯଦି ((c >= '@' && c < '\177') || c == ' ')
        c &= 0x1F;
    ଅନ୍ୟଥା ଯଦି (c == '2')
        c = '\000';
    ଅନ୍ୟଥା ଯଦି (c >= '3' && c <= '7')
        c -= ('3' - '\033');
    ଅନ୍ୟଥା ଯଦି (c == '8')
        c = '\177';
    ଅନ୍ୟଥା ଯଦି (c == '/')
        c = '_' & 0x1F;
    ଫେରନ୍ତୁ c;
}

/**
 * Provides either exactly one symbol, or XKB_KEY_NoSymbol.
 */
xkb_keysym_t
xkb_state_ଚାବି_get_one_sym(ସଂରଚନା xkb_state *state, xkb_keycode_t kc)
{
    ଅଚଳ xkb_keysym_t *syms = ଖାଲି;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_syms = xkb_state_ଚାବି_get_syms(state, kc, &syms);

    ଯଦି (ଅଙ୍କ_syms != 1)
        ଫେରନ୍ତୁ XKB_ଚାବି_No_ପ୍ରତୀକ;
    ଅନ୍ୟଥା
        ଫେରନ୍ତୁ syms[0];
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
ସ୍ଥିର xkb_keysym_t
get_one_sym_for_string(ସଂରଚନା xkb_state *state, xkb_keycode_t kc)
{
    ଅଚଳ xkb_layout_index_t layout = xkb_state_ଚାବି_get_layout(state, kc);
    ଅଚଳ xkb_layout_index_t ଅଙ୍କ_layouts =
        xkb_keymap_ଅଙ୍କ_layouts_for_ଚାବି(state->keymap, kc);
    xkb_level_index_t level = xkb_state_ଚାବି_get_level(state, kc, layout);
    ଯଦି (layout == XKB_LAYOUT_INVALID || ଅଙ୍କ_layouts == 0 ||
        level == XKB_LEVEL_INVALID)
        ଫେରନ୍ତୁ XKB_ଚାବି_No_ପ୍ରତୀକ;

    ଅଚଳ xkb_keysym_t *syms = ଖାଲି;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nsyms = xkb_keymap_ଚାବି_get_syms_by_level(state->keymap, kc,
                                                 layout, level, &syms);
    ଯଦି (nsyms != 1)
        ଫେରନ୍ତୁ XKB_ଚାବି_No_ପ୍ରତୀକ;
    xkb_keysym_t sym = syms[0];

    ଯଦି (should_do_ctrl_transformation(state, kc) && sym > 127u) {
        ପାଇଁ (xkb_layout_index_t i = 0; i < ଅଙ୍କ_layouts; i++) {
            level = xkb_state_ଚାବି_get_level(state, kc, i);
            ଯଦି (level == XKB_LEVEL_INVALID)
                ଜାରି;

            nsyms = xkb_keymap_ଚାବି_get_syms_by_level(state->keymap, kc,
                                                     i, level, &syms);
            ଯଦି (nsyms == 1 && syms[0] <= 127u) {
                sym = syms[0];
                ଭାଙ୍ଗ;
            }
        }
    }

    ଯଦି (should_do_caps_transformation(state, kc)) {
        sym = xkb_keysym_to_upper(sym);
    }

    ଫେରନ୍ତୁ sym;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_state_ଚାବି_get_utf8(ସଂରଚନା xkb_state *state, xkb_keycode_t kc,
                       ଅକ୍ଷର *buffer, size_t size)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nsyms;
    ଅଚଳ xkb_keysym_t *syms = ଖାଲି;
    ଅଚଳ xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    ଯଦି (sym != XKB_ଚାବି_No_ପ୍ରତୀକ) {
        nsyms = 1; syms = &sym;
    }
    ଅନ୍ୟଥା {
        nsyms = xkb_state_ଚାବି_get_syms(state, kc, &syms);
    }

    /* Make sure not to truncate in the middle of a UTF-8 sequence. */
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = 0;
    ଅକ୍ଷର tmp[XKB_KEYSYM_UTF8_MAX_SIZE];
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nsyms; i++) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = xkb_keysym_to_utf8(syms[i], tmp, ଆକାର(tmp));
        ଯଦି (ret <= 0)
            ଯାଅ err_bad;

        ret--;
        ଯଦି ((size_t)offset + (size_t)ret <= size)
            memcpy(buffer + offset, tmp, (size_t)ret);
        offset += ret;
    }

    ଯଦି ((size_t)offset >= size)
        ଯାଅ err_trunc;
    buffer[offset] = '\0';

    ଯଦି (!is_valid_utf8(buffer, (size_t)offset))
        ଯାଅ err_bad;

    ଯଦି (offset == 1 && (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) buffer[0] <= 127u &&
        should_do_ctrl_transformation(state, kc))
        buffer[0] = XkbToControl(buffer[0]);

    ଫେରନ୍ତୁ offset;

err_trunc:
    ଯଦି (size > 0)
        buffer[size - 1] = '\0';
    ଫେରନ୍ତୁ offset;

err_bad:
    ଯଦି (size > 0)
        buffer[0] = '\0';
    ଫେରନ୍ତୁ 0;
}

uint32_t
xkb_state_ଚାବି_get_utf32(ସଂରଚନା xkb_state *state, xkb_keycode_t kc)
{
    ଅଚଳ xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    uint32_t cp = xkb_keysym_to_utf32(sym);

    ଯଦି (cp <= 127u && should_do_ctrl_transformation(state, kc))
        cp = (uint32_t) XkbToControl((ଅକ୍ଷର) cp);

    ଫେରନ୍ତୁ cp;
}

/**
 * Serializes the requested modifier state into an xkb_mod_mask_t, with all
 * the same disclaimers as in xkb_state_update_mask.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ xkb_mod_mask_t
serialize_mods(ଅଚଳ ସଂରଚନା state_components *components,
               ଗଣନା xkb_state_component type)
{
    xkb_mod_mask_t ret = 0;

    ଯଦି (type & XKB_STATE_MODS_EFFECTIVE)
        ଫେରନ୍ତୁ components->mods;

    ଯଦି (type & XKB_STATE_MODS_DEPRESSED)
        ret |= components->base_mods;
    ଯଦି (type & XKB_STATE_MODS_LATCHED)
        ret |= components->latched_mods;
    ଯଦି (type & XKB_STATE_MODS_LOCKED)
        ret |= components->locked_mods;

    ଫେରନ୍ତୁ ret;
}

xkb_mod_mask_t
xkb_state_serialize_mods(ସଂରଚନା xkb_state *state,
                         ଗଣନା xkb_state_component type)
{
    ଫେରନ୍ତୁ serialize_mods(&state->components, type);
}

/**
 * Serialises the requested group state, with all the same disclaimers as
 * in xkb_state_update_mask.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ xkb_layout_index_t
serialize_layout(ଅଚଳ ସଂରଚନା state_components *components,
                 ଗଣନା xkb_state_component type)
{
    ଯଦି (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ଫେରନ୍ତୁ components->group;

    int32_t ret = 0;
    ଯଦି (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret += components->base_group;
    ଯଦି (type & XKB_STATE_LAYOUT_LATCHED)
        ret += components->latched_group;
    ଯଦି (type & XKB_STATE_LAYOUT_LOCKED)
        ret += components->locked_group;

    ଫେରନ୍ତୁ (xkb_layout_index_t)ret;
}

xkb_layout_index_t
xkb_state_serialize_layout(ସଂରଚନା xkb_state *state,
                           ଗଣନା xkb_state_component type)
{
    ଫେରନ୍ତୁ serialize_layout(&state->components, type);
}

xkb_led_mask_t
xkb_state_serialize_leds(ଅଚଳ ସଂରଚନା xkb_state *state,
                         ଗଣନା xkb_state_component type)
{
    ଫେରନ୍ତୁ (type & XKB_STATE_LEDS)
        ? state->components.leds
        : 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଗଣନା xkb_keyboard_control_flags
serialize_controls(ଅଚଳ ସଂରଚନା state_components *components,
                   ଗଣନା xkb_state_component type)
{
    ଫେରନ୍ତୁ (type & XKB_STATE_CONTROLS_EFFECTIVE)
        /*
         * Enable using the public API with the all the Controls values, except
         * the internal ones, if any.
         */
        ? (ଗଣନା xkb_keyboard_control_flags)
          (components->controls & CONTROL_ALL_BOOLEAN)
        : 0;
}

ଗଣନା xkb_keyboard_control_flags
xkb_state_serialize_controls(ଅଚଳ ସଂରଚନା xkb_state *state,
                             ଗଣନା xkb_state_component type)
{
    ଫେରନ୍ତୁ serialize_controls(&state->components, type);
}

/**
 * Gets a modifier mask and returns the resolved effective mask; this
 * is needed because some modifiers can also map to other modifiers, e.g.
 * the "NumLock" modifier usually also sets the "Mod2" modifier.
 */
xkb_mod_mask_t
mod_mask_get_effective(ସଂରଚନା xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    /* Initialize the effective mask with its corresponding real mods. */
    xkb_mod_mask_t mask = mods & MOD_REAL_MASK_ALL;

    /* Resolve the virtual modifiers */
    ଅଚଳ ସଂରଚନା xkb_mod *mod;
    xkb_mod_index_t i;
    xkb_vmods_enumerate(i, mod, &keymap->mods)
        ଯଦି (mods & (UINT32_C(1) << i))
            mask |= mod->mapping;

    ଫେରନ୍ତୁ mask;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_state_mod_index_is_active(ସଂରଚନା xkb_state *state,
                              xkb_mod_index_t idx,
                              ଗଣନା xkb_state_component type)
{
    ଯଦି (unlikely(idx >= xkb_keymap_ଅଙ୍କ_mods(state->keymap)))
        ଫେରନ୍ତୁ -1;

    ଅଚଳ xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    ଯଦି (!mapping) {
        /* Modifier not mapped */
        ଫେରନ୍ତୁ 0;
    }
    /* WARNING: this may overmatch for virtual modifiers */
    ଫେରନ୍ତୁ (xkb_state_serialize_mods(state, type) & mapping) == mapping;
}

ଗଣନା {
    XKB_STATE_MATCH_FLAGS = XKB_STATE_MATCH_ANY
                          | XKB_STATE_MATCH_ALL
                          | XKB_STATE_MATCH_NON_EXCLUSIVE
};

/**
 * Helper function for xkb_state_mod_indices_are_active and
 * xkb_state_mod_names_are_active.
 */
ସ୍ଥିର bool
match_mod_masks(ସଂରଚନା xkb_state *state,
                ଗଣନା xkb_state_component type,
                ଗଣନା xkb_state_match match,
                xkb_mod_mask_t wanted)
{
    ଅଚଳ xkb_mod_mask_t active = xkb_state_serialize_mods(state, type);

    ଯଦି (!(match & XKB_STATE_MATCH_NON_EXCLUSIVE) && (active & ~wanted))
        ଫେରନ୍ତୁ false;

    ଯଦି (match & XKB_STATE_MATCH_ANY)
        ଫେରନ୍ତୁ active & wanted;

    /* Ensure all flags but XKB_STATE_MATCH_ALL are processed before */
    static_assert(XKB_STATE_MATCH_ALL ==
                  ((ଗଣନା xkb_state_match) XKB_STATE_MATCH_FLAGS &
                   ~(XKB_STATE_MATCH_ANY | XKB_STATE_MATCH_NON_EXCLUSIVE)),
                  "");

    ଫେରନ୍ତୁ (active & wanted) == wanted;
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, or -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_state_mod_indices_are_active(ସଂରଚନା xkb_state *state,
                                 ଗଣନା xkb_state_component type,
                                 ଗଣନା xkb_state_match match,
                                 ...)
{
    ଯଦି (match & ~((ଗଣନା xkb_state_match) XKB_STATE_MATCH_FLAGS))
        ଫେରନ୍ତୁ -2;

    va_list ap;
    xkb_mod_mask_t wanted = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
    ଅଚଳ xkb_mod_index_t ଅଙ୍କ_mods = xkb_keymap_ଅଙ୍କ_mods(state->keymap);

    va_start(ap, match);
    ଯେତେବେଳେ (1) {
        xkb_mod_index_t idx = va_arg(ap, xkb_mod_index_t);
        ଯଦି (idx == XKB_MOD_INVALID)
            ଭାଙ୍ଗ;
        ଯଦି (unlikely(idx >= ଅଙ୍କ_mods)) {
            ret = -1;
            ଭାଙ୍ଗ;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_end(ap);

    ଯଦି (ret == -1)
        ଫେରନ୍ତୁ ret;

    ଯଦି (!wanted) {
        /* Modifiers not mapped */
        ଫେରନ୍ତୁ 0;
    }

    ଫେରନ୍ତୁ match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_state_mod_name_is_active(ସଂରଚନା xkb_state *state, ଅଚଳ ଅକ୍ଷର *name,
                             ଗଣନା xkb_state_component type)
{
    ଅଚଳ xkb_mod_index_t idx = xkb_keymap_mod_get_index(state->keymap, name);

    ଯଦି (idx == XKB_MOD_INVALID)
        ଫେରନ୍ତୁ -1;

    ଫେରନ୍ତୁ xkb_state_mod_index_is_active(state, idx, type);
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ATTR_NULL_SENTINEL ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_state_mod_names_are_active(ସଂରଚନା xkb_state *state,
                               ଗଣନା xkb_state_component type,
                               ଗଣନା xkb_state_match match,
                               ...)
{
    ଯଦି (match & ~((ଗଣନା xkb_state_match) XKB_STATE_MATCH_FLAGS))
        ଫେରନ୍ତୁ -2;

    va_list ap;
    xkb_mod_mask_t wanted = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

    va_start(ap, match);
    ଯେତେବେଳେ (1) {
        ଅଚଳ ଅକ୍ଷର *str = va_arg(ap, ଅଚଳ ଅକ୍ଷର *);
        ଯଦି (str == ଖାଲି)
            ଭାଙ୍ଗ;
        ଅଚଳ xkb_mod_index_t idx = xkb_keymap_mod_get_index(state->keymap, str);
        ଯଦି (idx == XKB_MOD_INVALID) {
            ret = -1;
            ଭାଙ୍ଗ;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_end(ap);

    ଯଦି (ret == -1)
        ଫେରନ୍ତୁ ret;

    ଯଦି (!wanted) {
        /* Modifiers not mapped */
        ଫେରନ୍ତୁ 0;
    }

    ଫେରନ୍ତୁ match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given group is active with the specified type(s), 0 if
 * not, or -1 if the group is invalid.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_state_layout_index_is_active(ସଂରଚନା xkb_state *state,
                                 xkb_layout_index_t idx,
                                 ଗଣନା xkb_state_component type)
{
    ଯଦି (idx >= state->keymap->ଅଙ୍କ_groups)
        ଫେରନ୍ତୁ -1;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
    ଯଦି (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ret |= (state->components.group == idx);
    ଯଦି (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret |= (state->components.base_group == (int32_t) idx);
    ଯଦି (type & XKB_STATE_LAYOUT_LATCHED)
        ret |= (state->components.latched_group == (int32_t) idx);
    ଯଦି (type & XKB_STATE_LAYOUT_LOCKED)
        ret |= (state->components.locked_group == (int32_t) idx);

    ଫେରନ୍ତୁ ret;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_state_layout_name_is_active(ସଂରଚନା xkb_state *state, ଅଚଳ ଅକ୍ଷର *name,
                                ଗଣନା xkb_state_component type)
{
    ଅଚଳ xkb_layout_index_t idx =
        xkb_keymap_layout_get_index(state->keymap, name);

    ଯଦି (idx == XKB_LAYOUT_INVALID)
        ଫେରନ୍ତୁ -1;

    ଫେରନ୍ତୁ xkb_state_layout_index_is_active(state, idx, type);
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_state_led_index_is_active(ସଂରଚନା xkb_state *state, xkb_led_index_t idx)
{
    ଯଦି (idx >= state->keymap->ଅଙ୍କ_leds ||
        state->keymap->leds[idx].name == XKB_ATOM_NONE)
        ଫେରନ୍ତୁ -1;

    ଫେରନ୍ତୁ !!(state->components.leds & (UINT32_C(1) << idx));
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_state_led_name_is_active(ସଂରଚନା xkb_state *state, ଅଚଳ ଅକ୍ଷର *name)
{
    ଅଚଳ xkb_led_index_t idx = xkb_keymap_led_get_index(state->keymap, name);

    ଯଦି (idx == XKB_LED_INVALID)
        ଫେରନ୍ତୁ -1;

    ଫେରନ୍ତୁ xkb_state_led_index_is_active(state, idx);
}

/**
 * See:
 * - XkbTranslateKeyCode(3), mod_rtrn return value, from libX11.
 * - MyEnhancedXkbTranslateKeyCode(), a modification of the above, from GTK+.
 */
ସ୍ଥିର xkb_mod_mask_t
ଚାବି_get_consumed(ସଂରଚନା xkb_state *state, ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                 ଗଣନା xkb_consumed_mode mode)
{
    ଅଚଳ xkb_layout_index_t group =
        xkb_state_ଚାବି_get_layout(state, ଚାବି->keycode);
    ଯଦି (group == XKB_LAYOUT_INVALID)
        ଫେରନ୍ତୁ 0;

    xkb_mod_mask_t preserve = 0;
    xkb_mod_mask_t consumed = 0;

    ଅଚଳ ସଂରଚନା xkb_ଚାବି_type_entry* ଅଚଳ matching_entry =
        get_entry_for_ଚାବି_state(state, ଚାବି, group);
    ଯଦି (matching_entry)
        preserve = matching_entry->preserve.mask;

    ଅଚଳ ସଂରଚନା xkb_ଚାବି_type* ଅଚଳ type = ଚାବି->groups[group].type;
    ବାଛ (mode) {
    ସ୍ଥିତି XKB_CONSUMED_MODE_XKB:
        consumed = type->mods.mask;
        ଭାଙ୍ଗ;

    ସ୍ଥିତି XKB_CONSUMED_MODE_GTK: {
        ଅଚଳ ସଂରଚନା xkb_ଚାବି_type_entry* ଅଚଳ no_mods_entry =
            get_entry_for_mods(type, 0);
        ଅଚଳ xkb_level_index_t no_mods_leveli = no_mods_entry
                                               ? no_mods_entry->level
                                               : 0;
        ଅଚଳ ସଂରଚନା xkb_level* ଅଚଳ no_mods_level =
            &ଚାବି->groups[group].levels[no_mods_leveli];

        ପାଇଁ (darray_size_t i = 0; i < type->ଅଙ୍କ_entries; i++) {
            ଅଚଳ ସଂରଚନା xkb_ଚାବି_type_entry* ଅଚଳ entry = &type->entries[i];
            ଯଦି (!entry_is_active(entry))
                ଜାରି;

            ଅଚଳ ସଂରଚନା xkb_level* ଅଚଳ level =
                &ଚାବି->groups[group].levels[entry->level];
            ଯଦି (XkbLevelsSameSyms(level, no_mods_level))
                ଜାରି;

            ଯଦି (entry == matching_entry || one_bit_set(entry->mods.mask))
                consumed |= entry->mods.mask & ~entry->preserve.mask;
        }
        ଭାଙ୍ଗ;
    }
    }

    ଫେରନ୍ତୁ consumed & ~preserve;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_state_mod_index_is_consumed2(ସଂରଚନା xkb_state *state, xkb_keycode_t kc,
                                 xkb_mod_index_t idx,
                                 ଗଣନା xkb_consumed_mode mode)
{
    ଅଚଳ ସଂରଚନା xkb_ଚାବି* ଅଚଳ ଚାବି = Xkb_ଚାବି(state->keymap, kc);

    ଯଦି (unlikely(!ଚାବି || idx >= xkb_keymap_ଅଙ୍କ_mods(state->keymap)))
        ଫେରନ୍ତୁ -1;

    ଅଚଳ xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    ଯଦି (!mapping) {
        /* Modifier not mapped */
        ଫେରନ୍ତୁ 0;
    }
    ଫେରନ୍ତୁ (mapping & ଚାବି_get_consumed(state, ଚାବି, mode)) == mapping;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_state_mod_index_is_consumed(ସଂରଚନା xkb_state *state, xkb_keycode_t kc,
                                xkb_mod_index_t idx)
{
    ଫେରନ୍ତୁ xkb_state_mod_index_is_consumed2(state, kc, idx,
                                            XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_mod_mask_remove_consumed(ସଂରଚନା xkb_state *state, xkb_keycode_t kc,
                                   xkb_mod_mask_t mask)
{
    ଅଚଳ ସଂରଚନା xkb_ଚାବି* ଅଚଳ ଚାବି = Xkb_ଚାବି(state->keymap, kc);

    ଯଦି (!ଚାବି)
        ଫେରନ୍ତୁ 0;

    ଫେରନ୍ତୁ resolve_to_canonical_mods(state->keymap, mask) &
           ~ଚାବି_get_consumed(state, ଚାବି, XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_ଚାବି_get_consumed_mods2(ସଂରଚନା xkb_state *state, xkb_keycode_t kc,
                                 ଗଣନା xkb_consumed_mode mode)
{
    ବାଛ (mode) {
    ସ୍ଥିତି XKB_CONSUMED_MODE_XKB:
    ସ୍ଥିତି XKB_CONSUMED_MODE_GTK:
        ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        log_err_func(state->keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "unrecognized consumed modifiers mode: %d\n", mode);
        ଫେରନ୍ତୁ 0;
    }

    ଅଚଳ ସଂରଚନା xkb_ଚାବି* ଅଚଳ ଚାବି = Xkb_ଚାବି(state->keymap, kc);
    ଯଦି (!ଚାବି)
        ଫେରନ୍ତୁ 0;

    ଫେରନ୍ତୁ ଚାବି_get_consumed(state, ଚାବି, mode);
}

xkb_mod_mask_t
xkb_state_ଚାବି_get_consumed_mods(ସଂରଚନା xkb_state *state, xkb_keycode_t kc)
{
    ଫେରନ୍ତୁ xkb_state_ଚାବି_get_consumed_mods2(state, kc, XKB_CONSUMED_MODE_XKB);
}

/*******************************************************************************
 * Server state machine API
 ******************************************************************************/

/** Entry to keep track of the state of an overlaid key */
ସଂରଚନା xkb_overlaid_ଚାବି {
    /** The source key being overlaid */
    ଅଚଳ ସଂରଚନା xkb_ଚାବି *old;
    /** The target key */
    ଅଚଳ ସଂରଚନା xkb_ଚାବି *new;
    /** Counter of key currently down */
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା refcnt;
};

ସଂରଚନା xkb_shortcuts_config_entry {
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
ସଂରଚନା xkb_machine {
    /** Inherits from `xkb_server_state` */
    ସଂରଚନା xkb_server_state base;

    /* Extra server-specific state data */

    /** Keyboard overlays handling */
    ସଂରଚନା {
        /**
         * Current overlaid keys
         *
         * When a key is overlaid, it remained overlaid in this configuration
         * until totally released, i.e. every key press was matched with the
         * corresponding key release.
         */
        darray(ସଂରଚନା xkb_overlaid_ଚାବି) ଚାବିଗୁଡ଼ିକ;
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
    ସଂରଚନା machine_config {
        /** Modifiers remapping */
        ସଂରଚନା machine_modifiers_config {
            /** Real modifier mask */
            xkb_mod_mask_t mask;
            /** Modifiers re-mappings */
            darray_size_t mappings_ଅଙ୍କ;
            ସଂରଚନା machine_mods_mapping {
                xkb_mod_mask_t source;
                xkb_mod_mask_t target;
            } * mappings ATTR_COUNTED_BY(mappings_ଅଙ୍କ);
        } modifiers;

        /** Shortcuts tweak */
        ସଂରଚନା machine_shortcuts_config {
            /** Substitution entries */
            ସଂରଚନା xkb_shortcuts_config_entry *entries;
        } shortcuts;
    } config;

    /** Mouse keys handling */
    ସଂରଚନା {
        /** Default pointer button */
        xkb_pointer_button_index_t default_button;
        /** Mask of the locked buttons */
        xkb_pointer_button_mask_t locked_buttons;
    } mouse;

    ଗଣନା xkb_machine_flags flags;
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା darray(ସଂରଚନା machine_mods_mapping) machine_mods_mappings;

ସଂରଚନା xkb_machine_builder {
    ସଂରଚନା xkb_keymap *keymap;

    /** Controls */
    ସଂରଚନା {
        /** Accessibility flags */
        ସଂରଚନା {
            ଗଣନା xkb_a11y_flags affect_flags;
            ଗଣନା xkb_a11y_flags flags;
        } a11y;
    } controls;

    /** Modifiers re-mapping */
    machine_mods_mappings mods;

    /** Shortcuts tweak */
    ସଂରଚନା xkb_shortcuts_config_options {
        /** Substitution entries */
        darray(ସଂରଚନା xkb_shortcuts_config_entry) entries;
    } shortcuts;

    ଗଣନା xkb_machine_flags machine_flags;
    ଗଣନା xkb_machine_builder_flags builder_flags;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା refcnt;
};

ସଂରଚନା xkb_machine_builder *
xkb_machine_builder_new(
    ସଂରଚନା xkb_keymap * restrict keymap,
    ଅଚଳ ସଂରଚନା xkb_machine_builder_config * restrict config,
    ଗଣନା xkb_status * restrict status
)
{
    /* Handle default configuration */
    ସ୍ଥିର ଅଚଳ ସଂରଚନା xkb_machine_builder_config default_config = {
        .size = ଆକାର(default_config)
    };
    ଯଦି (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ଅଚଳ ଗଣନା xkb_status status_ = xkb_check_state_abi(config);
    ଯଦି (status_) {
        xkb_log_abi_error(keymap->ctx, __func__, status_);
        ଯଦି (status)
            *status = status_;
        ଫେରନ୍ତୁ ଖାଲି;
    }

    /* Sanitize input */
    ଅଚଳ ଗଣନା xkb_machine_builder_flags invalid_builder_flags
        = config->builder_flags
        & ~(ଗଣନା xkb_machine_builder_flags)XKB_MACHINE_BUILDER_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ;
    ଯଦି (invalid_builder_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS_,
                     "unrecognized machine builder flags: 0x%x\n",
                     invalid_builder_flags);
        ଯଦି (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS;
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଅଚଳ ଗଣନା xkb_machine_flags invalid_machine_flags
        = (ଗଣନା xkb_machine_flags)config->machine_flags
        & ~(ଗଣନା xkb_machine_flags)XKB_MACHINE_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ;
    ଯଦି (invalid_machine_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS_,
                     "unrecognized state machine flags: 0x%x\n",
                     invalid_machine_flags);
        ଯଦି (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS;
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ସଂରଚନା xkb_machine_builder * ଅଚଳ builder = calloc(1, ଆକାର(*builder));
    ଯଦି (!builder) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine builder\n");
        ଯଦି (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        ଫେରନ୍ତୁ ଖାଲି;
    }

    *builder = (ସଂରଚନା xkb_machine_builder) {
        .keymap = xkb_keymap_ref(keymap),
        .refcnt = 1,
        .builder_flags = (ଗଣନା xkb_machine_builder_flags)config->builder_flags,
        .machine_flags = (ଗଣନା xkb_machine_flags)config->machine_flags,
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

    ଯଦି (status)
        *status = XKB_SUCCESS;

    ଫେରନ୍ତୁ builder;
}

ସଂରଚନା xkb_machine_builder *
xkb_machine_builder_ref(ସଂରଚନା xkb_machine_builder *builder)
{
    ପ୍ରତିଜ୍ଞା(builder->refcnt > 0);
    builder->refcnt++;
    ଫେରନ୍ତୁ builder;
}

ଶୂନ୍ୟ
xkb_machine_builder_unref(ସଂରଚନା xkb_machine_builder *builder)
{
    ପ୍ରତିଜ୍ଞା(!builder || builder->refcnt > 0);
    ଯଦି (!builder || --builder->refcnt > 0)
        ଫେରନ୍ତୁ;

    darray_free(builder->shortcuts.entries);
    darray_free(builder->mods);
    xkb_keymap_unref(builder->keymap);
    ମୁକ୍ତ(builder);
}

ସଂରଚନା xkb_keymap *
xkb_machine_builder_get_keymap(ଅଚଳ ସଂରଚନା xkb_machine_builder *builder)
{
    /* Reference count is not updated. See API doc. */
    ଫେରନ୍ତୁ builder->keymap;
}

ଗଣନା xkb_status
xkb_machine_builder_update_a11y(
    ସଂରଚନା xkb_machine_builder * restrict builder,
    ଅଚଳ ସଂରଚନା xkb_machine_builder_a11y_update * restrict update)
{
    /* Check ABI compatibility */
    ଗଣନା xkb_status status = xkb_check_state_abi(update);
    ଯଦି (status != XKB_SUCCESS) {
        xkb_log_abi_error(builder->keymap->ctx, __func__, status);
        ଫେରନ୍ତୁ status;
    }

    ଅଚଳ ଗଣନା xkb_a11y_flags invalid_flags =
        ~(ଗଣନା xkb_a11y_flags)XKB_A11_Y_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ;

    ଯଦି (update->affect_flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y affected flags: %#x\n",
                __func__, update->affect_flags & invalid_flags);
        ଫେରନ୍ତୁ XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }
    ଯଦି (update->flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y flags: %#x\n",
                __func__, update->flags & invalid_flags);
        ଫେରନ୍ତୁ XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }

    builder->controls.a11y.affect_flags |= update->affect_flags;
    builder->controls.a11y.flags &= ~update->affect_flags;
    builder->controls.a11y.flags |= (update->flags & update->affect_flags);

    ଫେରନ୍ତୁ XKB_SUCCESS;
}

ଗଣନା xkb_status
xkb_machine_builder_update_mods_remap(
    ସଂରଚନା xkb_machine_builder * restrict builder,
    ଅଚଳ ସଂରଚନା xkb_machine_builder_mods_remap_update * restrict update
)
{
    ଯଦି (!update->source) {
        ଯଦି (!update->target) {
            /* Reset mappings */
            darray_resize(builder->mods, 0);
            ଫେରନ୍ତୁ XKB_SUCCESS;
        } ଅନ୍ୟଥା {
            ଫେରନ୍ତୁ XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
        }
    }

    /* Check the modifiers against the keymap */
    ଅଚଳ xkb_mod_mask_t invalid = ~builder->keymap->canonical_state_mask;
    ଯଦି ((update->source & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid source modifiers: 0x%"PRIx32"\n",
                __func__, update->source);
        ଫେରନ୍ତୁ XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    ଯଦି ((update->target & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid target modifiers: 0x%"PRIx32"\n",
                __func__, update->target);
        ଫେରନ୍ତୁ XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    ସଂରଚନା machine_mods_mapping *mapping = ଖାଲି;
    darray_size_t m = 0;
    darray_enumerate(m, mapping, builder->mods) {
        ଯଦି (mapping->source == update->source) {
            ଯଦି (!update->target) {
                /* Remove mapping */
                darray_remove(builder->mods, m);
            } ଅନ୍ୟଥା {
                /* Update mapping */
                mapping->target = update->target;
            }
            ଫେରନ୍ତୁ XKB_SUCCESS;
        }
    }

    ଯଦି (update->target) {
        /* Append new mapping */
        darray_append(
            builder->mods,
            (ସଂରଚନା machine_mods_mapping) {
                .source = update->source,
                .target = update->target
            }
        );
    } ଅନ୍ୟଥା {
        /* Ignore missing mapping */
    }

    ଫେରନ୍ତୁ XKB_SUCCESS;
}

ଗଣନା xkb_status
xkb_machine_builder_update_shortcut_override(
    ସଂରଚନା xkb_machine_builder * restrict builder,
    ଅଚଳ ସଂରଚନା xkb_machine_builder_shortcut_override_update * restrict update
)
{
    ସଂରଚନା xkb_keymap *keymap = builder->keymap;

    /* Check the modifiers against the keymap */
    ଅଚଳ xkb_mod_mask_t invalid = ~keymap->canonical_state_mask;
    ଯଦି ((update->affect_mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid affected modifiers: 0x%"PRIx32"\n",
                __func__, update->affect_mods);
        ଫେରନ୍ତୁ XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    ଯଦି ((update->mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid modifiers: 0x%"PRIx32"\n",
                __func__, update->mods);
        ଫେରନ୍ତୁ XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    /* Check the layout indices against the keymap */
    ଯଦି (update->source >= keymap->ଅଙ୍କ_groups &&
        update->source != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "%s: Invalid source layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->ଅଙ୍କ_groups, update->source + 1);
        ଫେରନ୍ତୁ XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }
    ଯଦି (update->target >= keymap->ଅଙ୍କ_groups &&
        update->target != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "%s: Invalid target layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->ଅଙ୍କ_groups, update->target + 1);
        ଫେରନ୍ତୁ XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    ସଂରଚନା xkb_shortcuts_config_options * ଅଚଳ config = &builder->shortcuts;

    /* Resize array & initialize new entries, if relevant */
    ଯଦି (update->source >= darray_size(config->entries) &&
        update->source != XKB_LAYOUT_INVALID) {
        ଯଦି (update->target == update->source) {
            /* Skip default setting */
            ଫେରନ୍ତୁ XKB_SUCCESS;
        }
        xkb_layout_index_t new = darray_size(config->entries);
        darray_resize(config->entries, update->source + 1);
        ପାଇଁ (; new <= update->source; new++) {
            darray_item(config->entries, new) =
                (ସଂରଚନା xkb_shortcuts_config_entry) {
                    .target = XKB_LAYOUT_INVALID,
                    .mods = 0
                };
        }
    }

    ସଂରଚନା xkb_shortcuts_config_entry *entry;
    xkb_layout_index_t source = (update->source == XKB_LAYOUT_INVALID)
        ? 0               /* Loop over all entries */
        : update->source; /* Single entry */
    darray_foreach_from(entry, config->entries, source) {
        ଯଦି (update->source == XKB_LAYOUT_INVALID &&
            entry->target == XKB_LAYOUT_INVALID) {
            /* Skip previous deactivated entry */
            source++;
            ଜାରି;
        }

        ଯଦି (update->affect_mods) {
            entry->mods &= ~update->affect_mods;
            entry->mods |= (update->affect_mods & update->mods);
        }

        ଯଦି (update->source != XKB_LAYOUT_INVALID) {
            entry->target = (update->target == source)
                ? XKB_LAYOUT_INVALID
                : update->target;
            /* Single entry: exit loop */
            ଭାଙ୍ଗ;
        }
        source++;
    }

    ଫେରନ୍ତୁ XKB_SUCCESS;
}

/** Compare 2 modifiers combinations */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
cmp_mod_masks(ଅଚଳ ଶୂନ୍ୟ *a, ଅଚଳ ଶୂନ୍ୟ *b)
{
    ଅଚଳ xkb_mod_mask_t m1 = *((xkb_mod_mask_t *) a);
    ଅଚଳ xkb_mod_mask_t m2 = *((xkb_mod_mask_t *) b);
    ଯଦି (m1 == m2)
        ଫେରନ୍ତୁ 0;

    ଯଦି (!m1)
        ଫେରନ୍ତୁ +1;
    ଯଦି (!m2)
        ଫେରନ୍ତୁ -1;

    ଅଚଳ xkb_mod_mask_t overlap = (m1 & m2);
    ଯଦି (overlap == m1)
        ଫେରନ୍ତୁ +1; /* m1 is a subset of m2 */
    ଯଦି (overlap == m2)
        ଫେରନ୍ତୁ -1; /* m2 is a subset of m1 */

    /* Compare active modifiers numbers */
    ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା count1 = popcount32(m1);
    ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା count2 = popcount32(m2);
    ଯଦି (count1 > count2)
        ଫେରନ୍ତୁ -1;
    ଯଦି (count1 < count2)
        ଫେରନ୍ତୁ +1;

    /* Give priority to real modifiers */
    ଫେରନ୍ତୁ (m1 < m2) ? -1 : +1;
}

ସ୍ଥିର ଗଣନା xkb_status
machine_set_mods(ସଂରଚନା xkb_machine *sm,
                 ଅଚଳ machine_mods_mappings *raw_mappings)
{
    ଯଦି (!darray_empty(*raw_mappings)) {
        machine_mods_mappings mappings = darray_new();
        xkb_mod_mask_t mask = 0;
        ଅଚଳ ସଂରଚନା machine_mods_mapping *mapping;
        ଅଚଳ xkb_mod_mask_t invalid =
            ~sm->base.base.keymap->canonical_state_mask;
        darray_foreach(mapping, *raw_mappings) {
            ଯଦି (!mapping->source || !mapping->target ||
                (mapping->source & invalid) || (mapping->target & invalid))
                ଜାରି;
            darray_append(mappings, *mapping);
            mask |= mapping->source;
        }

        /*
         * Re-order the mappings, so that combinations have higher priority
         * if they are superset of other combinations.
         * For other type of overlaps, see the sorting function.
         */
        qsort(darray_ବସ୍ତୁଗୁଡ଼ିକ(mappings), darray_size(mappings),
              ଆକାର(*darray_ବସ୍ତୁଗୁଡ଼ିକ(mappings)), &cmp_mod_masks);

        darray_steal(
            mappings,
            &sm->config.modifiers.mappings,
            &sm->config.modifiers.mappings_ଅଙ୍କ
        );
        sm->config.modifiers.mask = mask;
    } ଅନ୍ୟଥା {
        sm->config.modifiers = (ସଂରଚନା machine_modifiers_config) {
            .mappings = ଖାଲି,
            .mappings_ଅଙ୍କ = 0,
            .mask = 0
        };
    }

    ଫେରନ୍ତୁ XKB_SUCCESS;
}

ସ୍ଥିର ଗଣନା xkb_status
machine_set_shortcuts(
    ସଂରଚନା xkb_machine * restrict sm,
    ଅଚଳ ସଂରଚନା xkb_shortcuts_config_options * restrict options
)
{
    ଯଦି (darray_empty(options->entries)) {
        sm->config.shortcuts = (ସଂରଚନା machine_shortcuts_config) {
            .entries = ଖାଲି
        };
        ଫେରନ୍ତୁ XKB_SUCCESS;
    }

    ସଂରଚନା xkb_keymap * ଅଚଳ keymap = sm->base.base.keymap;

    ସଂରଚନା xkb_shortcuts_config_entry * entries =
        calloc(keymap->ଅଙ୍କ_groups, ଆକାର(*entries));
    ଯଦି (!entries)
        ଫେରନ୍ତୁ XKB_ERROR_ALLOCATION_FAILURE;

    ଅଚଳ xkb_layout_index_t ଗଣନା_2 = MIN(
        keymap->ଅଙ୍କ_groups,
        (xkb_layout_index_t) darray_size(options->entries)
    );

    bool some_entries = false;
    ପାଇଁ (xkb_layout_index_t l = 0; l < ଗଣନା_2; l++) {
        /* Sanitize layout target */
        ଅଚଳ xkb_layout_index_t target =
            darray_item(options->entries, l).target;
        /* Sanitize modifier mask */
        ଅଚଳ xkb_mod_mask_t mods = (
            darray_item(options->entries, l).mods &
            sm->base.base.keymap->canonical_state_mask
        );
        ଯଦି (target < keymap->ଅଙ୍କ_groups && target != l && mods) {
            entries[l] = (ସଂରଚନା xkb_shortcuts_config_entry) {
                .target = target,
                .mods = mods
            };
            some_entries = true;
        } ଅନ୍ୟଥା {
            entries[l] = (ସଂରଚନା xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
            ଜାରି;
        }
    }

    ଯଦି (!some_entries) {
        ମୁକ୍ତ(entries);
        entries = ଖାଲି;
    } ଅନ୍ୟଥା {
        /* Initialize groups with no entries */
        ପାଇଁ (xkb_layout_index_t l = ଗଣନା_2; l < keymap->ଅଙ୍କ_groups; l++) {
            entries[l] = (ସଂରଚନା xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
        }
    }

    sm->config.shortcuts = (ସଂରଚନା machine_shortcuts_config) {
        .entries = entries
    };
    ଫେରନ୍ତୁ XKB_SUCCESS;
}

ସଂରଚନା xkb_machine *
xkb_machine_new(ଅଚଳ ସଂରଚନା xkb_machine_builder * restrict builder,
                ଗଣନା xkb_status * restrict status)
{
    ସଂରଚନା xkb_machine * ଅଚଳ machine = calloc(1, ଆକାର(*machine));
    ଯଦି (!machine) {
        log_err_func1(builder->keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine\n");
        ଯଦି (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        ଫେରନ୍ତୁ ଖାଲି;
    }

    xkb_server_state_init(&machine->base, builder->keymap, SERVER_STATE,
                          builder->controls.a11y.affect_flags,
                          builder->controls.a11y.flags);
    machine->flags = builder->machine_flags;
    ଯଦି (!(machine->flags &
          XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_ଚାବି_FIX))
    {
        machine->base.flags |= STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_ଚାବି;
    }
    ଯଦି (machine->flags & XKB_MACHINE_UNLOCK_MODS_ON_PRESS) {
        machine->base.flags |= STATE_UNLOCK_MODS_ON_PRESS;
    }

    ଗଣନା xkb_status status_;
    ଯଦି ((status_ = machine_set_mods(machine, &builder->mods)) != XKB_SUCCESS ||
        (status_ = machine_set_shortcuts(machine, &builder->shortcuts)) !=
        XKB_SUCCESS) {
        *status = status_;
        ଯାଅ error;
    }

    darray_init(machine->overlays.ଚାବିଗୁଡ଼ିକ);
    machine->mouse.default_button = XKB_POINTER_BUTTON_MIN;

    ଯଦି (status)
        *status = XKB_SUCCESS;

    ଫେରନ୍ତୁ machine;

error:
    xkb_machine_unref(machine);
    ଫେରନ୍ତୁ ଖାଲି;
}

ସଂରଚନା xkb_machine *
xkb_machine_ref(ସଂରଚନା xkb_machine *sm)
{
    ପ୍ରତିଜ୍ଞା(sm->base.base.refcnt > 0);
    sm->base.base.refcnt++;
    ଫେରନ୍ତୁ sm;
}

ଶୂନ୍ୟ
xkb_machine_unref(ସଂରଚନା xkb_machine *sm)
{
    ପ୍ରତିଜ୍ଞା(!sm || sm->base.base.refcnt > 0);
    ଯଦି (!sm || --sm->base.base.refcnt > 0)
        ଫେରନ୍ତୁ;

    xkb_state_destroy(&sm->base.base);
    darray_free(sm->overlays.ଚାବିଗୁଡ଼ିକ);
    ମୁକ୍ତ(sm->config.shortcuts.entries);
    ମୁକ୍ତ(sm->config.modifiers.mappings);
    ମୁକ୍ତ(sm);
}

ସଂରଚନା xkb_keymap *
xkb_machine_get_keymap(ଅଚଳ ସଂରଚନା xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    ଫେରନ୍ତୁ sm->base.base.keymap;
}

ସଂରଚନା xkb_state *
xkb_machine_get_state(ସଂରଚନା xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    ଫେରନ୍ତୁ (ସଂରଚନା xkb_state *)sm;
}

ସଂରଚନା xkb_state *
xkb_state_new_from_machine(ଅଚଳ ସଂରଚନା xkb_machine * restrict machine,
                           ଗଣନା xkb_status * restrict status)
{
    /* Creation */
    ଗଣନା xkb_status status_;
    ସଂରଚନା xkb_state * ଅଚଳ state = xkb_state_new_with_mode(
        machine->base.base.keymap,
        XKB_STATE_MODE_SERVER_QUERY,
        &status_
    );
    ଯଦି (status_ != XKB_SUCCESS) {
        ଯଦି (status)
            *status = status_;
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ପ୍ରତିଜ୍ଞା(state->mode == SERVER_COMPANION);
    ପ୍ରତିଜ୍ଞା(state->refcnt == 1);

    /* Copy machine’s state */
    *state = machine->base.base;

    /* Restore previous values */
    state->mode = SERVER_COMPANION;
    state->refcnt = 1;

    ଯଦି (status)
        *status = XKB_SUCCESS;
    ଫେରନ୍ତୁ state;
}

ସ୍ଥିର ଶୂନ୍ୟ
machine_update_overlays(ସଂରଚନା xkb_machine *sm)
{
    /*
     * Overlays indices are stored 1-indexed in nibbles: the lowest
     * nibble corresponds to the latest activated index.
     */

    ଅଚଳ xkb_overlay_mask_t mask =
        overlays_from_controls(sm->base.base.components.controls);
    xkb_overlay_mask_t added = mask & ~sm->overlays.enabled;

    /* Remove overlays no longer enabled and keep relative order */
    uint32_t order = sm->overlays.order;
    ଅଚଳ xkb_overlay_index_t overlay_max =
        format_max_overlays(sm->base.base.keymap->format);
    ପାଇଁ (uint8_t n = 0; n < overlay_max;) {
        xkb_overlay_index_t overlay_idx = (order >> (n * 4)) & 0xf;
        ଯଦି (!overlay_idx)
            ଭାଙ୍ଗ;
        ଅଚଳ xkb_overlay_mask_t overlay_mask =
            (xkb_overlay_mask_t)(1u << (overlay_idx - 1u) /* k is 1-indexed */);
        ଯଦି (overlay_mask & mask) {
            /* no duplicates */
            added &= ~overlay_mask;
            n++;
        } ଅନ୍ୟଥା {
            /* remove overlay */
            ଅଚଳ uint32_t head = (UINT32_C(1) << (n * 4)) - 1;
            order = (order & head) | ((order >> 4) & ~head);
        }
    }

    /* Add new overlays in ascending order */
    ପାଇଁ (xkb_overlay_index_t k = 0; added; k++, added >>= 1) {
        ଯଦି (added & 0x1) {
            order <<= 4;
            order |= k + 1u; /* k is 0-indexed */
        }
    }

    sm->overlays.order = order;
    sm->overlays.enabled = mask;
}

ଗଣନା xkb_status
xkb_machine_process_synthetic(ସଂରଚନା xkb_machine *sm,
                              ଅଚଳ ସଂରଚନା xkb_synthetic_update *update,
                              ସଂରଚନା xkb_events *events)
{
    /* Check ABI compatibility */
    ଗଣନା xkb_status status =
        check_state_update_abi(sm->base.base.keymap->ctx, update);
    ଯଦି (status != XKB_SUCCESS)
        ଫେରନ୍ତୁ status;

    ସଂରଚନା xkb_server_state * ଅଚଳ state = &sm->base;
    ଅଚଳ ସଂରଚନା state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    darray_size(events->queue) = 0;
    events->next = 0;

    /* Update parametrized controls first */
    ଯଦି (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        ଯଦି (status != XKB_SUCCESS)
            ଫେରନ୍ତୁ status;
    }

    ଯଦି (update->components) {
        ଅଚଳ ସଂରଚନା xkb_state_components_update * ଅଚଳ components =
            update->components;
        /* Update boolean controls first */
        ଯଦି (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, events);
        }

        state_update_latched_locked(state, components, events);
    }

    xkb_state_update_derived(&state->base);

    ଅଚଳ ଗଣନା xkb_state_component changed = get_state_component_changes(
        &previous_components, &state->base.components
    );
    ଯଦି (changed) {
        // TODO: latch controls
        ଯଦି (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        /* Create event only if some component actually changed */
        darray_append(events->queue, (ସଂରଚନା xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .changed = changed,
                .components = state->base.components
            }
        });

        /* Terminate the frame */
        darray_append(events->queue, (ସଂରଚନା xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    ଫେରନ୍ତୁ XKB_SUCCESS;
}

ସ୍ଥିର ssize_t
do_remap_modifiers(ଅଚଳ ସଂରଚନା machine_modifiers_config * restrict mappings,
                   ସଂରଚନା xkb_state * restrict state,
                   ସଂରଚନା xkb_events * restrict events,
                   ଅଚଳ ସଂରଚନା xkb_ଚାବି * restrict ଚାବି)
{
    ଯଦି (!(mappings->mask & state->components.mods))
        ଫେରନ୍ତୁ -1;

    ଅଚଳ xkb_layout_index_t layout = state_ଚାବି_get_layout(state, ଚାବି);
    ଯଦି (layout >= ଚାବି->ଅଙ୍କ_groups)
        ଫେରନ୍ତୁ -1;

    ଅଚଳ ସଂରଚନା xkb_ଚାବି_type * ଅଚଳ type = ଚାବି->groups[layout].type;
    xkb_mod_mask_t affect_mods = 0;
    xkb_mod_mask_t mods = 0;
    ପାଇଁ (darray_size_t m = 0; m < mappings->mappings_ଅଙ୍କ; m++) {
        ଅଚଳ ସଂରଚନା machine_mods_mapping * ଅଚଳ mapping =
            &mappings->mappings[m];
        ଯଦି (/* Skip not matching active mods */
            (mapping->source & state->components.mods) == mapping->source &&
            /* Skip overlap with previous active mappings */
            !(mapping->source & affect_mods) &&
            /* Skip if the key type uses some of the source modifiers */
            !(mapping->source & type->mods.mask)) {
            affect_mods |= mappings->mappings[m].source;
            mods |= mappings->mappings[m].target;
        }
    }

    ଯଦି (!affect_mods)
        ଫେରନ୍ତୁ -1;

    ସଂରଚନା xkb_state new = *state;
    new.components.base_mods = (new.components.base_mods & ~affect_mods) | mods;
    new.components.latched_mods = (new.components.latched_mods & ~affect_mods);
    new.components.locked_mods = (new.components.locked_mods & ~affect_mods);
    xkb_state_update_derived(&new);

    ssize_t event_idx = -1;
    ଅଚଳ ଗଣନା xkb_state_component changed =
        get_state_component_changes(&state->components, &new.components);
    ଯଦି (changed) {
        event_idx = (ssize_t) darray_size(events->queue);
        darray_append(events->queue, (ସଂରଚନା xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = new.components,
                .changed = changed
            }
        });
    }

    state->components.mods = new.components.mods;
    ଫେରନ୍ତୁ event_idx;
}

/**
 * Activate an alternative layout if some modifiers are active, typically
 * `Control`, `Alt` or `Super`.
 *
 * The tweak consists of computing a *base* layout such that the *effective*
 * layout is the corresponding target layout.
 */
ସ୍ଥିର ssize_t
do_shortcuts_tweak(ଅଚଳ ସଂରଚନା machine_shortcuts_config *config,
                   ସଂରଚନା xkb_state *state,
                   ଅଚଳ ସଂରଚନା state_components *previous_components,
                   ସଂରଚନା xkb_events *events, ssize_t remap_event)
{
    ଯଦି (!config->entries) {
        /* No shortcuts tweak */
        ଫେରନ୍ତୁ remap_event;
    }

    ଅଚଳ ସଂରଚନା xkb_shortcuts_config_entry * ଅଚଳ entry =
        &config->entries[state->components.group];

    ଯଦି (entry->target != XKB_LAYOUT_INVALID &&
        (entry->mods & state->components.mods)) {
        /*
         * Activate shortcuts tweak:
         * 1. The real base group is saved by the caller, to be restored by
         *    later by a call to undo_tweaks().
         * 2. Compute the new base group so that the resulting effective group
         *    is the target.
         * 3. Recompute derived state components.
         */
        ସଂରଚନା xkb_state new = *state;
        ଯଦି (remap_event < 0) {
            /* Create new event */
            remap_event = (ssize_t) darray_size(events->queue);
            darray_append(events->queue, (ସଂରଚନା xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = {0},
                    .changed = 0
                }
            });
        } ଅନ୍ୟଥା {
            /* Merge with last remap event */
            new.components =
                darray_item(events->queue, remap_event).components.components;
        }
        new.components.base_group
            = (int32_t) entry->target
            - state->components.latched_group
            - state->components.locked_group;
        xkb_state_update_derived(&new);

        ଅଚଳ ଗଣନା xkb_state_component changed =
            get_state_component_changes(previous_components, &new.components);
        darray_item(events->queue, remap_event).components.changed = changed;
        darray_item(events->queue, remap_event).components.components =
            new.components;

        state->components.group = new.components.group;
    } ଅନ୍ୟଥା {
        /* Deactivated entry or modifiers do not match */
    }
    ଫେରନ୍ତୁ remap_event;
}

ସ୍ଥିର ଶୂନ୍ୟ
undo_tweaks(ଅଚଳ ସଂରଚନା xkb_state *state,
            ଅଚଳ ସଂରଚନା state_components *initial_components,
            ସଂରଚନା xkb_events *events)
{
    /* Get last component event */
    ଅଚଳ ସଂରଚନା xkb_event *event = ଖାଲି;
    darray_foreach_reverse(event, events->queue) {
        ଯଦି (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS)
            ଭାଙ୍ଗ;
    }
    ଯଦି (!event)
        ଫେରନ୍ତୁ;

    /* Restore state */
    ଅଚଳ ଗଣନା xkb_state_component changed =
        get_state_component_changes(initial_components,
                                    &event->components.components);
    ଯଦି (changed) {
        darray_append(events->queue, (ସଂରଚନା xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = *initial_components,
                .changed = changed
            }
        });
    }
}

ସ୍ଥିର ଅଚଳ ସଂରଚନା xkb_ଚାବି *
process_overlayable_ଚାବି(ସଂରଚନା xkb_machine *sm,
                        ଅଚଳ ସଂରଚନା xkb_ଚାବି * ଚାବି,
                        ଗଣନା xkb_ଚାବି_direction direction)
{
    /* Check if key is currently overlaid */
    ସଂରଚନା xkb_overlaid_ଚାବି *entry = ଖାଲି;
    ସଂରଚନା xkb_overlaid_ଚାବି *available_entry = ଖାଲି;
    darray_foreach(entry, sm->overlays.ଚାବିଗୁଡ଼ିକ) {
        ଯଦି (entry->old == ଚାବି) {
            /* Maintain the key overlaid */
            ବାଛ (direction) {
            ସ୍ଥିତି XKB_ଚାବି_DOWN:
                entry->refcnt++;
                ଭାଙ୍ଗ;
            ସ୍ଥିତି XKB_ଚାବି_REPEATED:
                ଭାଙ୍ଗ;
            ସ୍ଥିତି XKB_ଚାବି_UP:
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                entry->refcnt--;
                ଭାଙ୍ଗ;
            }
            ଯଦି (entry->refcnt <= 0)
                entry->old = ଖାଲି;
            ଫେରନ୍ତୁ entry->new;
        } ଅନ୍ୟଥା ଯଦି (!entry->old && !available_entry) {
            /* First free spot */
            available_entry = entry;
        }
    }
    ଯଦି (direction == XKB_ଚାବି_DOWN) {
        /* Enable key overlay */

        /*
         * Get overlay index: select the latest activated overlay
         * containing the key.
         */
        ଅଚଳ ସଂରଚନା xkb_ଚାବି *new = ଚାବି;

        ଯଦି (ଚାବି->overlays & sm->overlays.enabled) {
            /* Some relevant overlay is active */
            ପାଇଁ (uint32_t ଗଦା = sm->overlays.order; ଗଦା; ଗଦା >>= 4) {
                static_assert(XKB_OVERLAY_ଗଣନା == 8, "");
                ଅଚଳ xkb_overlay_index_t overlay = (ଗଦା & 0xf) - 1;
                ଅଚଳ xkb_overlay_mask_t mask =
                    (xkb_overlay_mask_t)(1u << overlay);
                ଯଦି (ଚାବି->overlays & mask) {
                    /* Get new key */
                    ଯଦି (ଚାବି->overlays_inline) {
                        /* Stored inline */
                        new = ଚାବି->overlay_ଚାବି;
                    } ଅନ୍ୟଥା {
                        /* Stored in sparse array */
                        ଅଚଳ xkb_overlay_mask_t ତଳ = (xkb_overlay_mask_t)(
                            ଚାବି->overlays & (xkb_overlay_mask_t)(mask - 1u)
                        );
                        /* index = number of set bits strictly below bit */
                        ଅଚଳ xkb_overlay_index_t index =
                            (xkb_overlay_index_t)popcount32(ତଳ);
                        new = ଚାବି->overlays_ଚାବିଗୁଡ଼ିକ[index];
                    }
                    ଭାଙ୍ଗ;
                }
            }
        } ଅନ୍ୟଥା {
            /*
             * No relevant overlay is active: block future overlays while
             * the key is pressed.
             */
        }

        /* Get new entry */
        ଯଦି (available_entry) {
            /* Re-use a free entry */
            entry = available_entry;
        } ଅନ୍ୟଥା {
            /* Append a new entry */
            ଅଚଳ darray_size_t idx = darray_size(sm->overlays.ଚାବିଗୁଡ଼ିକ);
            darray_resize(sm->overlays.ଚାବିଗୁଡ଼ିକ, idx + 1);
            entry = &darray_item(sm->overlays.ଚାବିଗୁଡ଼ିକ, idx);
        }

        /* Overlay the key */
        entry->old = ଚାବି;
        entry->new = new;
        entry->refcnt = 1;
        ଫେରନ୍ତୁ new;
    }

    /* No overlay */
    ଫେରନ୍ତୁ ଚାବି;
}

ଗଣନା xkb_status
xkb_machine_process_ଚାବି(ସଂରଚନା xkb_machine *sm,
                        xkb_keycode_t kc, ଗଣନା xkb_ଚାବି_direction direction,
                        ସଂରଚନା xkb_events *events)
{
    darray_size(events->queue) = 0;
    events->next = 0;

    ସଂରଚନା xkb_server_state * ଅଚଳ state = &sm->base;
    ଅଚଳ ସଂରଚନା xkb_ଚାବି * ଚାବି = Xkb_ଚାବି(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    ଯଦି (!ଚାବି || (direction == XKB_ଚାବି_REPEATED && !ଚାବି->repeats))
        ଫେରନ୍ତୁ XKB_SUCCESS;

    ଅଚଳ ସଂରଚନା state_components initial_components = state->base.components;

    ଯଦି (direction == XKB_ଚାବି_DOWN &&
        (sm->base.base.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ଚାବିଗୁଡ଼ିକ) &&
        (sm->base.a11y_flags & XKB_A11_Y_STICKY_ଚାବିଗୁଡ଼ିକ_NO_SIMULTANEOUS_ଚାବିଗୁଡ଼ିକ) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ଚାବିଗୁଡ଼ିକ,
                                      0, events);
    }

    ଯଦି (ଚାବି->overlays)
        ଚାବି = process_overlayable_ଚାବି(sm, ଚାବି, direction);

    ssize_t remap_event = do_remap_modifiers(&sm->config.modifiers,
                                             &state->base, events, ଚାବି);

    remap_event = do_shortcuts_tweak(&sm->config.shortcuts, &state->base,
                                     &initial_components, events, remap_event);

    ଯଦି (remap_event >= 0) {
        // TODO: what about merging state event for RedirectKey?
        darray_append(events->queue, (ସଂରଚନା xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    state->flags |= STATE_REQUIRE_ଚାବି_EVENT;
    state->set_mods = 0;
    state->clear_mods = 0;

    /* Handle key actions */
    xkb_filter_apply_all(state, events, ଚାବି, direction);

    xkb_mod_index_t i;
    xkb_mod_mask_t bit;
    ପାଇଁ (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        ଯଦି (state->set_mods & bit) {
            state->mod_ଚାବି_ଗଣନା[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    ପାଇଁ (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        ଯଦି (state->clear_mods & bit) {
            state->mod_ଚାବି_ଗଣନା[i]--;
            ଯଦି (state->mod_ଚାବି_ଗଣନା[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_ଚାବି_ଗଣନା[i] = 0;
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
    ଯଦି (state->flags & STATE_REQUIRE_ଚାବି_EVENT) {
        darray_append(events->queue, (ସଂରଚନା xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_ଚାବି,
            .ଚାବି = {
                .keycode = ଚାବି->keycode,
                .direction = direction
            }
        });
    }

    ଯଦି (remap_event >= 0) {
        // FIXME: fragile if last state change does not restore the state to the remap event
        undo_tweaks(&state->base, &initial_components, events);
    }

    ଅଚଳ ଗଣନା xkb_state_component changed = get_state_component_changes(
        &initial_components, &state->base.components
    );
    ଯଦି (changed) {
        ଯଦି (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        // TODO: merge with previous state event, if any

        darray_append(events->queue, (ସଂରଚନା xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = state->base.components,
                .changed = changed
            }
        });
    }

    ଯଦି (!darray_empty(events->queue) &&
        darray_item(events->queue, darray_size(events->queue) - 1).type !=
        XKB_EVENT_TYPE_FRAME)
    {
        darray_append(events->queue, (ସଂରଚନା xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    ଫେରନ୍ତୁ XKB_SUCCESS;
}

ସଂରଚନା xkb_events *
xkb_events_new(ସଂରଚନା xkb_context * restrict context,
               ଅଚଳ ସଂରଚନା xkb_events_config * restrict config,
               ଗଣନା xkb_status * restrict status)
{
    /* Handle default configuration */
    ସ୍ଥିର ଅଚଳ ସଂରଚନା xkb_events_config default_config = {
        .size = ଆକାର(default_config)
    };
    ଯଦି (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ଅଚଳ ଗଣନା xkb_status abi_error = xkb_check_state_abi(config);
    ଯଦି (abi_error) {
        xkb_log_abi_error(context, __func__, abi_error);
        ଯଦି (status)
            *status = abi_error;
        ଫେରନ୍ତୁ ଖାଲି;
    }

    /* Sanitize input */
    ଅଚଳ uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_EVENTS_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ);
    ଯଦି (invalid_flags) {
        log_err_func(context, XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS_,
                     "unrecognized events collection flags: 0x%"PRIx32"\n",
                     invalid_flags);
        ଯଦି (status)
            *status = XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS;
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ସଂରଚନା xkb_events *events = calloc(1, ଆକାର(*events));
    ଯଦି (!events) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events collection\n");
        ଯଦି (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        ଫେରନ୍ତୁ ଖାଲି;
    }

    /* Pre-allocate event queue */
    darray_init(events->queue);
    ସ୍ଥିର ଅଚଳ darray_size_t queue_initial_size = 16;
    darray_resize(events->queue, queue_initial_size);
    // TODO: proper safe darray_resize() with return value
    ଯଦି (!darray_ବସ୍ତୁଗୁଡ଼ିକ(events->queue)) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events queue\n");
        xkb_events_destroy(events);
        ଯଦି (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        ଫେରନ୍ତୁ ଖାଲି;
    }
    darray_size(events->queue) = 0;

    events->ctx = xkb_context_ref(context);
    events->next = 0;

    ଯଦି (status)
        *status = XKB_SUCCESS;
    ଫେରନ୍ତୁ events;
}

ଶୂନ୍ୟ
xkb_events_destroy(ସଂରଚନା xkb_events *events)
{
    ଯଦି (!events)
        ଫେରନ୍ତୁ;
    darray_free(events->queue);
    xkb_context_unref(events->ctx);
    ମୁକ୍ତ(events);
}

ଅଚଳ ସଂରଚନା xkb_event *
xkb_events_next(ସଂରଚନା xkb_events *events)
{
    ଯଦି (events->next < darray_size(events->queue)) {
        ଅଚଳ darray_size_t index = events->next++;
        ଫେରନ୍ତୁ &darray_item(events->queue, index);
    } ଅନ୍ୟଥା {
        ଫେରନ୍ତୁ ଖାଲି;
    }
}

ଗଣନା xkb_event_type
xkb_event_get_type(ଅଚଳ ସଂରଚନା xkb_event *event)
{
    ଫେରନ୍ତୁ event->type;
}

ଗଣନା xkb_status
xkb_event_get_keycode(ଅଚଳ ସଂରଚନା xkb_event *event,
                      xkb_keycode_t *keycode,
                      ଗଣନା xkb_ଚାବି_direction *direction)
{
    ଯଦି (event->type == XKB_EVENT_TYPE_ଚାବି) {
        *keycode = event->ଚାବି.keycode;
        *direction = event->ଚାବି.direction;
        ଫେରନ୍ତୁ XKB_SUCCESS;
    } ଅନ୍ୟଥା {
        ଫେରନ୍ତୁ XKB_ERROR_EVENT_TYPE_MISMATCH;
    }
}

ଗଣନା xkb_status
xkb_event_get_components(ଅଚଳ ସଂରଚନା xkb_event * restrict event,
                         ସଂରଚନା xkb_event_components * restrict components)
{
    ଯଦି (event->type != XKB_EVENT_TYPE_STATE_COMPONENTS)
        ଫେରନ୍ତୁ XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ଅଚଳ ଗଣନା xkb_status status = xkb_check_state_abi(components);
    ଯଦି (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        ଫେରନ୍ତୁ status;
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

    ଫେରନ୍ତୁ XKB_SUCCESS;
}

ଗଣନା xkb_status
xkb_event_get_pointer_motion(ଅଚଳ ସଂରଚନା xkb_event * restrict event,
                             ସଂରଚନା xkb_event_pointer_motion * restrict motion)
{
    ଯଦି (event->type != XKB_EVENT_TYPE_POINTER_MOTION)
        ଫେରନ୍ତୁ XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ଅଚଳ ଗଣନା xkb_status status = xkb_check_state_abi(motion);
    ଯଦି (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        ଫେରନ୍ତୁ status;
    }

    motion->flags = event->pointer_motion.flags;
    motion->x = event->pointer_motion.x;
    motion->y = event->pointer_motion.y;

    ଫେରନ୍ତୁ XKB_SUCCESS;
}

ଗଣନା xkb_status
xkb_event_get_pointer_button(ଅଚଳ ସଂରଚନା xkb_event * restrict event,
                             ସଂରଚନା xkb_event_pointer_button * restrict button)
{
    ଯଦି (event->type != XKB_EVENT_TYPE_POINTER_BUTTON)
        ଫେରନ୍ତୁ XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ଅଚଳ ଗଣନା xkb_status status = xkb_check_state_abi(button);
    ଯଦି (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        ଫେରନ୍ତୁ status;
    }

    button->button = event->pointer_button.button;
    button->state = event->pointer_button.state;
    button->ଗଣନା_2 = event->pointer_button.ଗଣନା_2;

    ଫେରନ୍ତୁ XKB_SUCCESS;
}

ଗଣନା xkb_status
xkb_event_get_virtual_console(ଅଚଳ ସଂରଚନା xkb_event * restrict event,
                              int8_t * restrict index_or_offset,
                              bool * restrict is_offset)
{
    ଯଦି (event->type != XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
        ଫେରନ୍ତୁ XKB_ERROR_EVENT_TYPE_MISMATCH;

    *index_or_offset = event->virtual_console.index_or_offset;
    *is_offset = event->virtual_console.is_offset;

    ଫେରନ୍ତୁ XKB_SUCCESS;
}

ଗଣନା xkb_status
xkb_state_update_event(ସଂରଚନା xkb_state * restrict base_state,
                       ଅଚଳ ସଂରଚନା xkb_event * restrict event,
                       ଗଣନା xkb_state_component * restrict changed)
{
    /* Guard against server-only state */
    ପ୍ରତିଜ୍ଞା(base_state->mode == SERVER_COMPANION ||
           base_state->mode == LEGACY_MIXED_STATE);
    ଯଦି (base_state->mode != SERVER_COMPANION &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE_,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        ଫେରନ୍ତୁ XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    ସଂରଚନା xkb_client_state * ଅଚଳ state =
        (ସଂରଚନା xkb_client_state *)base_state;

    ଯଦି (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
        ଅଚଳ ସଂରଚନା state_components prev_components = state->base.components;
        state->base.components = event->components.components;
        /*
         * Recompute the changes instead of using the event value, because we do
         * not know if the event’s queue and the state are synced.
         */
        ଯଦି (changed)
            *changed = get_state_component_changes(&prev_components,
                                                   &state->base.components);
    } ଅନ୍ୟଥା {
        ଯଦି (changed)
            *changed = 0;
    }
    ଫେରନ୍ତୁ XKB_SUCCESS;
}

ସ୍ଥିର ଶୂନ୍ୟ
append_pointer_button(ସଂରଚନା xkb_events * restrict events,
                      ସଂରଚନା xkb_filter * restrict filter,
                      ଗଣନା xkb_pointer_button_state button_state,
                      uint8_t ଗଣନା_2)
{
    darray_append(events->queue, (ସଂରଚନା xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_BUTTON,
        .pointer_button = {
            .size = ଆକାର(((ସଂରଚନା xkb_event*)0)->pointer_button),
            .button = filter->priv,
            .state = button_state,
            .ଗଣନା_2 = ଗଣନା_2,
        }
    });
}

ସ୍ଥିର ଶୂନ୍ୟ
xkb_filter_pointer_button_new(ସଂରଚନା xkb_server_state *state,
                              ସଂରଚନା xkb_events *events,
                              ସଂରଚନା xkb_filter *filter)
{
    ଯଦି (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ଚାବିଗୁଡ଼ିକ)) {
        filter->func = ଖାଲି;
        ଫେରନ୍ତୁ;
    }

    ସଂରଚନା xkb_machine * ଅଚଳ sm = (ସଂରଚନା xkb_machine *)state;

    /* Resolve button */
    ଅଚଳ xkb_pointer_button_index_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;
    filter->priv = button;

    ଅଚଳ ଗଣନା xkb_pointer_button_state button_state =
        XKB_POINTER_BUTTON_PRESSED;
    append_pointer_button(events, filter, button_state,
                          filter->action.btn.ଗଣନା_2);
    state->flags &= ~STATE_REQUIRE_ଚାବି_EVENT;
}

ସ୍ଥିର bool
xkb_filter_pointer_button_func(ସଂରଚନା xkb_server_state *state,
                               ସଂରଚନା xkb_events *events,
                               ସଂରଚନା xkb_filter *filter,
                               ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                               ଗଣନା xkb_ଚାବି_direction direction)
{
    ଯଦି (ଚାବି != filter->ଚାବି || !events)
        ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ଚାବି_EVENT;

    ବାଛ (direction) {
    ସ୍ଥିତି XKB_ଚାବି_DOWN:
        filter->refcnt++;
        /* fallthrough */
    ସ୍ଥିତି XKB_ଚାବି_REPEATED:
        ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯଦି (--filter->refcnt > 0)
            ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    }

    ଯଦି (!filter->action.btn.ଗଣନା_2)
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);

    filter->func = ଖାଲି;
    ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
}

ସଂରଚନା xkb_filter_pointer_lock_button_priv {
    uint32_t button:31;
#ifdef _WIN32
    uint32_t locked:1; // MSVC requires matching types to pack bitfields
#ଅନ୍ୟଥା
    bool locked:1;
#endif
};

ଗଣନା {
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH =
        ଆକାର(ସଂରଚନା xkb_filter_pointer_lock_button_priv) * CHAR_BIT,
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX =
        (UINT64_C(1) << XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH) - 1
};

static_assert(ଆକାର(ସଂରଚନା xkb_filter_pointer_lock_button_priv) ==
              ଆକାର(((ସଂରଚନା xkb_filter*)0)->priv),
              "");
static_assert((uint32_t)XKB_POINTER_BUTTON_MAX < (uint32_t)XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX,
              "xkb_filter_pointer_lock_button_priv cannot store button");

ସ୍ଥିର ଶୂନ୍ୟ
xkb_filter_pointer_lock_button_new(ସଂରଚନା xkb_server_state *state,
                                   ସଂରଚନା xkb_events *events,
                                   ସଂରଚନା xkb_filter *filter)
{
    ଯଦି (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ଚାବିଗୁଡ଼ିକ)) {
        filter->func = ଖାଲି;
        ଫେରନ୍ତୁ;
    }

    state->flags &= ~STATE_REQUIRE_ଚାବି_EVENT;

    ସଂରଚନା xkb_machine * ଅଚଳ sm = (ସଂରଚନା xkb_machine *)state;

    /* Resolve button */
    ଅଚଳ xkb_pointer_button_index_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;

    static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                  "invalid UINTx_C() macro");
    static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                  "invalid left shift");
    ଅଚଳ xkb_pointer_button_mask_t mask = (UINT8_C(1) << button);

    ଅଚଳ bool locked = (sm->mouse.locked_buttons & mask);

    ଯଦି (!locked && !(filter->action.btn.flags & ACTION_LOCK_NO_LOCK)) {
        sm->mouse.locked_buttons |= mask;
        filter->priv = button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_PRESSED, 0);
    }

    ସଂରଚନା xkb_filter_pointer_lock_button_priv *priv =
        (ସଂରଚନା xkb_filter_pointer_lock_button_priv*)&filter->priv;
    priv->button = button,
    priv->locked = locked;
}

ସ୍ଥିର bool
xkb_filter_pointer_lock_button_func(ସଂରଚନା xkb_server_state *state,
                                    ସଂରଚନା xkb_events *events,
                                    ସଂରଚନା xkb_filter *filter,
                                    ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                                    ଗଣନା xkb_ଚାବି_direction direction)
{
    ଯଦି (ଚାବି != filter->ଚାବି || !events || state->base.mode != SERVER_STATE)
        ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ଚାବି_EVENT;

    ବାଛ (direction) {
    ସ୍ଥିତି XKB_ଚାବି_DOWN:
        filter->refcnt++;
        /* fallthrough */
    ସ୍ଥିତି XKB_ଚାବି_REPEATED:
        ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯଦି (--filter->refcnt > 0)
            ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    }

    ଅଚଳ ସଂରଚନା xkb_filter_pointer_lock_button_priv *priv =
        (ସଂରଚନା xkb_filter_pointer_lock_button_priv*)&filter->priv;

    ଯଦି (priv->locked && !(filter->action.btn.flags & ACTION_LOCK_NO_UNLOCK)) {
        ସଂରଚନା xkb_machine * ଅଚଳ sm = (ସଂରଚନା xkb_machine *)state;

        static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                    "invalid UINTx_C() macro");
        static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                    "invalid left shift");
        ଅଚଳ xkb_pointer_button_mask_t mask = (UINT8_C(1) << priv->button);

        sm->mouse.locked_buttons &= ~mask;
        filter->priv = priv->button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);
    }

    filter->func = ଖାଲି;
    ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
}

ସ୍ଥିର ଶୂନ୍ୟ
xkb_filter_pointer_set_default_button_new(ସଂରଚନା xkb_server_state *state,
                                          ସଂରଚନା xkb_events *events,
                                          ସଂରଚନା xkb_filter *filter)
{
    ଯଦି (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ଚାବିଗୁଡ଼ିକ)) {
        filter->func = ଖାଲି;
        ଫେରନ୍ତୁ;
    }

    state->flags &= ~STATE_REQUIRE_ଚାବି_EVENT;

    ସଂରଚନା xkb_machine * ଅଚଳ sm = (ସଂରଚନା xkb_machine *)state;

    static_assert(2 * XKB_POINTER_BUTTON_MAX <= INT_MAX, "lossy conversion");
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା button = (filter->action.dflt.flags & ACTION_ABSOLUTE_SWITCH)
        ? filter->action.dflt.ମୂଲ୍ୟ
        : sm->mouse.default_button + filter->action.dflt.ମୂଲ୍ୟ;

    /* Wrap into range */
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା rem = button % XKB_POINTER_BUTTON_MAX;
    sm->mouse.default_button = (rem >= 0)
        ? (xkb_pointer_button_index_t)rem
        : (xkb_pointer_button_index_t)(rem + XKB_POINTER_BUTTON_MAX);
}

ସ୍ଥିର bool
xkb_filter_pointer_set_default_button_func(ସଂରଚନା xkb_server_state *state,
                                           ସଂରଚନା xkb_events *events,
                                           ସଂରଚନା xkb_filter *filter,
                                           ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                                           ଗଣନା xkb_ଚାବି_direction direction)
{
    ଯଦି (ଚାବି != filter->ଚାବି || !events)
        ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ଚାବି_EVENT;

    ବାଛ (direction) {
    ସ୍ଥିତି XKB_ଚାବି_DOWN:
        filter->refcnt++;
        /* fallthrough */
    ସ୍ଥିତି XKB_ଚାବି_REPEATED:
        ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯଦି (--filter->refcnt > 0)
            ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    }

    filter->func = ଖାଲି;
    ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;
}

ସ୍ଥିର ଶୂନ୍ୟ
xkb_filter_terminate_server_new(ସଂରଚନା xkb_server_state *state,
                                 ସଂରଚନା xkb_events *events,
                                 ସଂରଚନା xkb_filter *filter)
{
    ଯଦି (!events || state->base.mode != SERVER_STATE ||
        !(((ସଂରଚନା xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = ଖାଲି;
        ଫେରନ୍ତୁ;
    }

    state->flags &= ~STATE_REQUIRE_ଚାବି_EVENT;

    darray_append(events->queue, (ସଂରଚନା xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    });
}

ସ୍ଥିର bool
xkb_filter_terminate_server_func(ସଂରଚନା xkb_server_state *state,
                                 ସଂରଚନା xkb_events *events,
                                 ସଂରଚନା xkb_filter *filter,
                                 ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                                 ଗଣନା xkb_ଚାବି_direction direction)
{
    ଯଦି (ଚାବି != filter->ଚାବି || !events)
        ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ଚାବି_EVENT;

    ବାଛ (direction) {
    ସ୍ଥିତି XKB_ଚାବି_DOWN:
        filter->refcnt++;
        /* fallthrough */
    ସ୍ଥିତି XKB_ଚାବି_REPEATED:
        ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯଦି (--filter->refcnt > 0)
            ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    }

    filter->func = ଖାଲି;
    ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
}

ସ୍ଥିର ଶୂନ୍ୟ
xkb_filter_switch_virtual_console_new(ସଂରଚନା xkb_server_state *state,
                                      ସଂରଚନା xkb_events *events,
                                      ସଂରଚନା xkb_filter *filter)
{
    ଯଦି (!events || state->base.mode != SERVER_STATE ||
        !(((ସଂରଚନା xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = ଖାଲି;
        ଫେରନ୍ତୁ;
    }

    state->flags &= ~STATE_REQUIRE_ଚାବି_EVENT;

    darray_append(events->queue, (ସଂରଚନା xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
        .virtual_console = {
            .index_or_offset = filter->action.screen.screen,
            .is_offset = !(filter->action.screen.flags & ACTION_ABSOLUTE_SWITCH)
        }
    });
}

ସ୍ଥିର bool
xkb_filter_switch_virtual_console_func(ସଂରଚନା xkb_server_state *state,
                                       ସଂରଚନା xkb_events *events,
                                       ସଂରଚନା xkb_filter *filter,
                                       ଅଚଳ ସଂରଚନା xkb_ଚାବି *ଚାବି,
                                       ଗଣନା xkb_ଚାବି_direction direction)
{
    ଯଦି (ଚାବି != filter->ଚାବି || !events)
        ଫେରନ୍ତୁ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ଚାବି_EVENT;

    ବାଛ (direction) {
    ସ୍ଥିତି XKB_ଚାବି_DOWN:
        filter->refcnt++;
        /* fallthrough */
    ସ୍ଥିତି XKB_ଚାବି_REPEATED:
        ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯଦି (--filter->refcnt > 0)
            ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
    }

    filter->func = ଖାଲି;
    ଫେରନ୍ତୁ XKB_FILTER_CONSUME;
}
