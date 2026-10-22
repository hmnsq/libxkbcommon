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

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <limits.h>
#অন্তর্ভাব <stdint.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <string.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "xkbcommon/xkbcommon-status.h"
#অন্তর্ভাব "xkbcommon/xkbcommon-features.h"
#অন্তর্ভাব "abi-check.h"
#অন্তর্ভাব "darray.h"
#অন্তর্ভাব "features/enums.h"
#অন্তর্ভাব "keymap.h"
#অন্তর্ভাব "keysym.h"
#অন্তর্ভাব "messages-codes.h"
#অন্তর্ভাব "state-priv.h"
#অন্তর্ভাব "utf8.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "util-mem.h"
#অন্তর্ভাব "util-numbers.h"

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
সংরচনা xkb_events {
    সংরচনা xkb_context *ctx;
    darray(সংরচনা xkb_event) queue;
    /**
     * Read cursor for `xkb_events_next()`.
     *
     * Reset to 0 on each `process_*` call.
     */
    darray_size_t next;
};

সংরচনা xkb_server_state;

সংরচনা xkb_filter {
    সঙ্ঘ xkb_action action;
    ধ্রুব সংরচনা xkb_চাবি *চাবি;
    bool (*func)(সংরচনা xkb_server_state *state,
                 সংরচনা xkb_events *events,
                 সংরচনা xkb_filter *filter,
                 ধ্রুব সংরচনা xkb_চাবি *চাবি,
                 গণনা xkb_চাবি_direction direction);
    uint32_t priv;
    পূর্ণাঙ্ক refcnt;
};

গণনা xkb_state_mode_internal {
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
    XKB_STATE_TYPE_অঙ্ক_ENTRIES = 5
};

static_assert(CLIENT_STATE == 0 &&
              CLIENT_STATE == SERVER_COMPANION - 1 &&
              SERVER_COMPANION == LEGACY_MIXED_STATE - 1 &&
              LEGACY_MIXED_STATE == LEGACY_SERVER_STATE - 1 &&
              LEGACY_SERVER_STATE == SERVER_STATE - 1 &&
              SERVER_STATE == XKB_STATE_TYPE_অঙ্ক_ENTRIES - 1 &&
              XKB_STATE_TYPE_অঙ্ক_ENTRIES == 5,
              "Order is used to guard client/server -only APIs: do not change");

গণনা { XKB_LAYOUT_INDEX_T_MIN_WIDTH = 6 };
static_assert(XKB_MAX_GROUPS < (1 << XKB_LAYOUT_INDEX_T_MIN_WIDTH),
              "Cannot encode out_of_range_group::redirect_group");

সংরচনা out_of_range_group {
    গণনা xkb_layout_out_of_range_policy policy :
        (পরিমাণ(গণনা xkb_layout_out_of_range_policy) * CHAR_BIT -
         XKB_LAYOUT_INDEX_T_MIN_WIDTH);
    xkb_layout_index_t redirect_group : XKB_LAYOUT_INDEX_T_MIN_WIDTH;
};

গণনা { XKB_STATE_MODE_INTERNAL_MIN_WIDTH = 4 /* 3 bits + sign */ };
static_assert(XKB_STATE_TYPE_অঙ্ক_ENTRIES <
              (1 << XKB_STATE_MODE_INTERNAL_MIN_WIDTH),
              "Cannot encode xkb_state::mode");

/**
 * Common state
 *
 * `mode` indicates whether the object is casted from a `xkb_server_state` or a
 * `xkb_client_state` struct.
 */
সংরচনা xkb_state {
    সংরচনা xkb_keymap *keymap;
    /*
     * Before updating the state, we keep a copy of just this struct. This
     * allows us to report which components of the state have changed.
     */
    সংরচনা state_components components;
    সংরচনা out_of_range_group out_of_range_group;

    গণনা xkb_state_mode_internal mode : XKB_STATE_MODE_INTERNAL_MIN_WIDTH;
    পূর্ণাঙ্ক refcnt : (পরিমাণ(পূর্ণাঙ্ক) * CHAR_BIT - XKB_STATE_MODE_INTERNAL_MIN_WIDTH);
};

/**
 * Minimal client state
 *
 * Wrapper for strong typing
 */
সংরচনা xkb_client_state {
    /** Inherits from `xkb_state` */
    সংরচনা xkb_state base;
};

গণনা state_flags {
    STATE_NO_FLAGS = 0,
    STATE_REQUIRE_চাবি_EVENT = (1 << 0),
    STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_চাবি = (1 << 1),
    STATE_UNLOCK_MODS_ON_PRESS = (1 << 2),
};

/**
 * Legacy server state, used for `LEGACY_MIXED_STATE` and `LEGACY_SERVER_STATE`
 * modes.
 *
 * Casted as `xkb_state` in the public API. See `xkb_state::mode`.
 */
সংরচনা xkb_server_state {
    /** Inherits from `xkb_state` */
    সংরচনা xkb_state base;

    /* Server-specific state data */

    darray(সংরচনা xkb_filter) filters;

    /**
     * Mix of constant configuration flags and
     * variable state-processing flags.
     */
    গণনা state_flags flags;

    গণনা xkb_a11y_flags a11y_flags;

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
    int16_t mod_চাবি_গুনতি[XKB_MAX_MODS];
};

স্থির ধ্রুব সংরচনা xkb_চাবি_type_entry *
get_entry_for_mods(ধ্রুব সংরচনা xkb_চাবি_type *type, xkb_mod_mask_t mods)
{
    প্রতি (darray_size_t i = 0; i < type->অঙ্ক_entries; i++)
        যদি (entry_is_active(&type->entries[i]) &&
            type->entries[i].mods.mask == mods)
            প্রত্যাগম &type->entries[i];
    প্রত্যাগম রিক্ত;
}

স্থির ধ্রুব সংরচনা xkb_চাবি_type_entry *
get_entry_for_চাবি_state(সংরচনা xkb_state *state, ধ্রুব সংরচনা xkb_চাবি *চাবি,
                        xkb_layout_index_t group)
{
    ধ্রুব সংরচনা xkb_চাবি_type* ধ্রুব type = চাবি->groups[group].type;
    xkb_mod_mask_t active_mods = state->components.mods & type->mods.mask;
    প্রত্যাগম get_entry_for_mods(type, active_mods);
}

স্থির অন্তঃস্থ xkb_level_index_t
state_চাবি_get_level(সংরচনা xkb_state *state, ধ্রুব সংরচনা xkb_চাবি *চাবি,
                         xkb_layout_index_t layout)
{
    যদি (layout >= চাবি->অঙ্ক_groups)
        প্রত্যাগম XKB_LEVEL_INVALID;

    /* If we don't find an explicit match the default is 0. */
    ধ্রুব সংরচনা xkb_চাবি_type_entry* ধ্রুব entry =
        get_entry_for_চাবি_state(state, চাবি, layout);

    প্রত্যাগম (entry) ? entry->level : 0;
}

/**
 * Returns the level to use for the given key and state, or
 * XKB_LEVEL_INVALID.
 */
xkb_level_index_t
xkb_state_চাবি_get_level(সংরচনা xkb_state *state, xkb_keycode_t kc,
                        xkb_layout_index_t layout)
{
    ধ্রুব সংরচনা xkb_চাবি* ধ্রুব চাবি = Xkb_চাবি(state->keymap, kc);

    প্রত্যাগম (চাবি)
        ? state_চাবি_get_level(state, চাবি, layout)
        : XKB_LEVEL_INVALID;
}

স্থির অন্তঃস্থ xkb_layout_index_t
state_চাবি_get_layout(সংরচনা xkb_state *state, ধ্রুব সংরচনা xkb_চাবি *চাবি)
{
    প্রত্যাগম xkb_keymap_চাবি_effective_layout(চাবি, state->components.group);
}

/**
 * Returns the layout to use for the given key and state, taking
 * wrapping/clamping/etc into account, or XKB_LAYOUT_INVALID.
 */
xkb_layout_index_t
xkb_state_চাবি_get_layout(সংরচনা xkb_state *state, xkb_keycode_t kc)
{
    ধ্রুব সংরচনা xkb_চাবি* ধ্রুব চাবি = Xkb_চাবি(state->keymap, kc);

    যদি (!চাবি)
        প্রত্যাগম XKB_LAYOUT_INVALID;

    প্রত্যাগম state_চাবি_get_layout(state, চাবি);
}

/* Empty action used for empty levels */
স্থির ধ্রুব সঙ্ঘ xkb_action dummy_action = { .type = ACTION_TYPE_NONE };

স্থির xkb_action_গুনতি_t
xkb_চাবি_get_actions(সংরচনা xkb_state *state, ধ্রুব সংরচনা xkb_চাবি *চাবি,
                    ধ্রুব সঙ্ঘ xkb_action **actions)
{
    ধ্রুব xkb_layout_index_t layout = state_চাবি_get_layout(state, চাবি);
    ধ্রুব xkb_level_index_t level = state_চাবি_get_level(state, চাবি, layout);
    যদি (level == XKB_LEVEL_INVALID)
        গচ্ছ err;

    ধ্রুব xkb_action_গুনতি_t গুনতি =
        xkb_keymap_চাবি_get_actions_by_level(state->keymap, চাবি,
                                            layout, level, actions);
    যদি (!গুনতি)
        গচ্ছ err;

    প্রত্যাগম গুনতি;

err:
    /* Use a dummy action if no corresponding level was found or if it is empty.
     * This is required e.g. to handle latches properly. */
    *actions = &dummy_action;
    প্রত্যাগম 1;
}

স্থির সংরচনা xkb_filter *
xkb_filter_new(সংরচনা xkb_server_state *state)
{
    সংরচনা xkb_filter *filter = রিক্ত, *iter;

    darray_foreach(iter, state->filters) {
        যদি (iter->func)
            অনুবর্ত;
        /* Use available slot */
        filter = iter;
        ভঙ্গ;
    }

    যদি (!filter) {
        /* No available slot: resize the filters array */
        darray_resize0(state->filters, darray_size(state->filters) + 1);
        filter = &darray_item(state->filters, darray_size(state->filters) -1);
    }

    filter->refcnt = 1;
    প্রত্যাগম filter;
}

/***====================================================================***/

গণনা xkb_filter_result {
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
    যদি ((filter_)->action.group.flags & ACTION_ABSOLUTE_SWITCH)          \
        (state_)->components.component_ = (filter_)->action.group.group; \
    অন্যথা                                                                 \
        (state_)->components.component_ += (filter_)->action.group.group

স্থির শূন্য
xkb_filter_group_set_new(সংরচনা xkb_server_state *state,
                         সংরচনা xkb_events *events,
                         সংরচনা xkb_filter *filter)
{
    static_assert(পরিমাণ(state->base.components.base_group) ==
                  পরিমাণ(filter->priv),
                  "Max groups don't fit");
    filter->priv = (uint32_t)state->base.components.base_group;
    apply_group_delta(filter, &state->base, base_group);
}

স্থির bool
xkb_filter_group_set_func(সংরচনা xkb_server_state *state,
                          সংরচনা xkb_events *events,
                          সংরচনা xkb_filter *filter,
                          ধ্রুব সংরচনা xkb_চাবি *চাবি,
                          গণনা xkb_চাবি_direction direction)
{
    যদি (চাবি != filter->চাবি) {
        filter->action.group.flags &= ~ACTION_LOCK_CLEAR;
        প্রত্যাগম XKB_FILTER_CONTINUE;
    }

    বিকল্প (direction) {
    পক্ষ XKB_চাবি_DOWN:
        filter->refcnt++;
        /* fallthrough */
    পক্ষ XKB_চাবি_REPEATED:
        প্রত্যাগম XKB_FILTER_CONSUME;
    স্বভাব:
        যদি (--filter->refcnt > 0)
            প্রত্যাগম XKB_FILTER_CONSUME;
    }

    static_assert(পরিমাণ(state->base.components.base_group) == পরিমাণ(filter->priv),
                  "Max groups don't fit");
    state->base.components.base_group = (int32_t) filter->priv;

    যদি (filter->action.group.flags & ACTION_LOCK_CLEAR)
        state->base.components.locked_group = 0;

    filter->func = রিক্ত;
    প্রত্যাগম XKB_FILTER_CONTINUE;
}

স্থির গণনা xkb_state_component
get_state_component_changes(ধ্রুব সংরচনা state_components *a,
                            ধ্রুব সংরচনা state_components *b)
{
    গণনা xkb_state_component mask = 0;

    যদি (a->group != b->group)
        mask |= XKB_STATE_LAYOUT_EFFECTIVE;
    যদি (a->base_group != b->base_group)
        mask |= XKB_STATE_LAYOUT_DEPRESSED;
    যদি (a->latched_group != b->latched_group)
        mask |= XKB_STATE_LAYOUT_LATCHED;
    যদি (a->locked_group != b->locked_group)
        mask |= XKB_STATE_LAYOUT_LOCKED;
    যদি (a->mods != b->mods)
        mask |= XKB_STATE_MODS_EFFECTIVE;
    যদি (a->base_mods != b->base_mods)
        mask |= XKB_STATE_MODS_DEPRESSED;
    যদি (a->latched_mods != b->latched_mods)
        mask |= XKB_STATE_MODS_LATCHED;
    যদি (a->locked_mods != b->locked_mods)
        mask |= XKB_STATE_MODS_LOCKED;
    যদি (a->leds != b->leds)
        mask |= XKB_STATE_LEDS;
    যদি (a->controls != b->controls)
        mask |= XKB_STATE_CONTROLS_EFFECTIVE;

    প্রত্যাগম mask;
}

স্থির শূন্য
xkb_filter_group_lock_new(সংরচনা xkb_server_state *state,
                          সংরচনা xkb_events *events,
                          সংরচনা xkb_filter *filter)
{
    যদি (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release: do nothing on key press.
         *
         * This is a keymap format v2 extension.
         */
        প্রত্যাগম;
    } অন্যথা {
        /* Lock on key press */
        apply_group_delta(filter, &state->base, locked_group);
    }
}

স্থির bool
xkb_filter_group_lock_func(সংরচনা xkb_server_state *state,
                           সংরচনা xkb_events *events,
                           সংরচনা xkb_filter *filter,
                           ধ্রুব সংরচনা xkb_চাবি *চাবি,
                           গণনা xkb_চাবি_direction direction)
{
    যদি (চাবি != filter->চাবি) {
        যদি ((filter->action.group.flags & ACTION_LOCK_ON_RELEASE) &&
            direction == XKB_চাবি_DOWN) {
            /*
             * Another key has been pressed after the locking key:
             * cancel group lock on release.
             *
             * This is a keymap v2 extension.
             */
            filter->action.group.flags &= ~ACTION_LOCK_ON_RELEASE;
        }
        প্রত্যাগম XKB_FILTER_CONTINUE;
    }

    বিকল্প (direction) {
    পক্ষ XKB_চাবি_DOWN:
        filter->refcnt++;
        /* fallthrough */
    পক্ষ XKB_চাবি_REPEATED:
        প্রত্যাগম XKB_FILTER_CONSUME;
    স্বভাব:
        যদি (--filter->refcnt > 0)
            প্রত্যাগম XKB_FILTER_CONSUME;
    }

    যদি (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release
         *
         * This is a keymap v2 extension.
         */
        apply_group_delta(filter, &state->base, locked_group);
    } অন্যথা {
        /* Lock on key press: do nothing on key release. */
    }
    filter->func = রিক্ত;
    প্রত্যাগম XKB_FILTER_CONTINUE;
}

স্থির bool
xkb_action_breaks_latch(ধ্রুব সঙ্ঘ xkb_action *action,
                        গণনা xkb_action_controls controls,
                        গণনা xkb_internal_action_flags flag,
                        xkb_mod_mask_t mask)
{
    বিকল্প (action->type) {
    পক্ষ ACTION_TYPE_NONE:
    পক্ষ ACTION_TYPE_VOID:
    পক্ষ ACTION_TYPE_PTR_BUTTON:
    পক্ষ ACTION_TYPE_PTR_LOCK:
    পক্ষ ACTION_TYPE_CTRL_SET:
    পক্ষ ACTION_TYPE_CTRL_LOCK:
    পক্ষ ACTION_TYPE_SWITCH_VT:
    পক্ষ ACTION_TYPE_TERMINATE:
    পক্ষ ACTION_TYPE_REDIRECT_চাবি:
        প্রত্যাগম true;
    পক্ষ ACTION_TYPE_PTR_MOVE:
    পক্ষ ACTION_TYPE_PTR_DEFAULT:
        প্রত্যাগম !(controls & CONTROL_MOUSE_চাবিগুলি);
    পক্ষ ACTION_TYPE_INTERNAL:
        প্রত্যাগম (action->internal.flags & flag) &&
               ((action->internal.clear_latched_mods & mask) == mask);
    স্বভাব:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_অঙ্ক_ENTRIES,
                      "Missing action type");
        প্রত্যাগম false;
    }
}

গণনা xkb_চাবি_latch_state {
    NO_LATCH = 0,
    LATCH_চাবি_DOWN,
    LATCH_PENDING,
    চাবি_LATCH_STATE_অঙ্ক_ENTRIES
};

#define MAX_XKB_চাবি_LATCH_STATE_LOG2 2
#যদি (চাবি_LATCH_STATE_অঙ্ক_ENTRIES > (1 << MAX_XKB_চাবি_LATCH_STATE_LOG2)) || \
    (-XKB_MAX_GROUPS) < (INT32_MIN >> MAX_XKB_চাবি_LATCH_STATE_LOG2) || \
      XKB_MAX_GROUPS > (INT32_MAX >> MAX_XKB_চাবি_LATCH_STATE_LOG2)
#error "Cannot represent priv field of the group latch filter"
#endif

/* Hold the latch state *and* the group delta */
সঙ্ঘ group_latch_priv {
    uint32_t priv;
    সংরচনা {
        /* The type is really: enum xkb_key_latch_state, but it is problematic
         * on Windows, because it is interpreted as signed and leads to wrong
         * negative values. */
        unsigned পূর্ণাঙ্ক latch:MAX_XKB_চাবি_LATCH_STATE_LOG2;
        int32_t group_delta:(32 - MAX_XKB_চাবি_LATCH_STATE_LOG2);
    };
};

স্থির শূন্য
xkb_filter_group_latch_new(সংরচনা xkb_server_state *state,
                           সংরচনা xkb_events *events,
                           সংরচনা xkb_filter *filter)
{
    ধ্রুব সঙ্ঘ group_latch_priv priv = {
        .latch = LATCH_চাবি_DOWN,
        .group_delta = (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)
            ? filter->action.group.group - state->base.components.base_group
            : filter->action.group.group
    };
    filter->priv = priv.priv;
    /* Like group set */
    apply_group_delta(filter, &state->base, base_group);
}

স্থির bool
xkb_filter_group_latch_func(সংরচনা xkb_server_state *state,
                            সংরচনা xkb_events *events,
                            সংরচনা xkb_filter *filter,
                            ধ্রুব সংরচনা xkb_চাবি *চাবি,
                            গণনা xkb_চাবি_direction direction)
{
    সঙ্ঘ group_latch_priv priv = {.priv = filter->priv};
    গণনা xkb_চাবি_latch_state latch = priv.latch;

    যদি (direction == XKB_চাবি_DOWN) {
        ধ্রুব সঙ্ঘ xkb_action *actions = রিক্ত;
        ধ্রুব xkb_action_গুনতি_t গুনতি = xkb_চাবি_get_actions(&state->base, চাবি,
                                                             &actions);

        যদি (latch == LATCH_চাবি_DOWN) {
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
            যদি (state->a11y_flags & XKB_A11_Y_LATCH_SIMULTANEOUS_চাবিগুলি) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                প্রতি (xkb_action_গুনতি_t k = 0; k < গুনতি; k++) {
                    যদি (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_GROUP_LATCH,
                                                0))
                    {
                        latch = NO_LATCH;
                        ভঙ্গ;
                    }
                }
            } অন্যথা {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        অন্যথা যদি (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is
             * pressed, or promote it to a lock if it's the same group delta &
             * flags and latchToLock option is enabled. */
            ধ্রুব bool sticky_চাবিগুলি = state->base.components.controls
                                   & CONTROL_STICKY_চাবিগুলি;
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
            প্রতি (xkb_action_গুনতি_t k = 0; k < গুনতি; k++) {
                যদি (((actions[k].type == ACTION_TYPE_GROUP_LATCH) ||
                     (actions[k].type == ACTION_TYPE_GROUP_SET && sticky_চাবিগুলি)) &&
                     actions[k].group.group == filter->action.group.group &&
                     (actions[k].group.flags & ACTION_ABSOLUTE_SWITCH) ==
                     (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)) {
                    যদি (filter->action.group.flags & ACTION_LATCH_TO_LOCK &&
                        filter->action.group.group != 0)
                    {
                        /* Promote to lock */
                        filter->action.type = ACTION_TYPE_GROUP_LOCK;
                        filter->func = xkb_filter_group_lock_func;
                        xkb_filter_group_lock_new(state, events, filter);
                        state->base.components.latched_group -= priv.group_delta;
                        filter->চাবি = চাবি;
                        /* XXX beep beep! */
                        প্রত্যাগম XKB_FILTER_CONSUME;
                    }
                    /* Do nothing if `latchToLock` option is not activated; if
                     * the latch is not broken by the following actions and the
                     * key is not consumed, then another latch filter will be
                     * created.
                     */
                    অনুবর্ত;
                }
                অন্যথা যদি (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_GROUP_LATCH,
                                                 0))
                {
                    /* Breaks the latch */
                    state->base.components.latched_group -= priv.group_delta;
                    filter->func = রিক্ত;
                    প্রত্যাগম XKB_FILTER_CONTINUE;
                }
            }
        }
        অন্যথা {
            /* Ignore press in NO_LATCH state */
            প্রতিজ্ঞা(latch == NO_LATCH);
        }
    }
    অন্যথা যদি (চাবি != filter->চাবি) {
        /* Ignore repeat/release of other keys */
    }
    অন্যথা যদি (direction == XKB_চাবি_REPEATED) {
        প্রত্যাগম XKB_FILTER_CONSUME; // TODO: check
    }
    অন্যথা {
        /* Our key got released.  If we've set it to clear locks, and we
         * currently have a group locked, then release it and
         * don't actually latch.  Else we've actually hit the latching
         * stage, so set PENDING and move our group from base to
         * latched. */
        যদি ((filter->action.group.flags & ACTION_LOCK_CLEAR) &&
             state->base.components.locked_group) {
            যদি (latch == LATCH_PENDING)
                state->base.components.latched_group -= priv.group_delta;
            অন্যথা
                state->base.components.base_group -= priv.group_delta;
            state->base.components.locked_group = 0;
            filter->func = রিক্ত;
        }
        /* Broken latch */
        অন্যথা যদি (latch == NO_LATCH) {
            state->base.components.base_group -= priv.group_delta;
            filter->func = রিক্ত;
        }
        /* We may already have reached the latch state if pressing the
         * key multiple times without latch-to-lock enabled. */
        অন্যথা যদি (latch == LATCH_চাবি_DOWN) {
            latch = LATCH_PENDING;
            /* Switch from set to latch */
            state->base.components.base_group -= priv.group_delta;
            state->base.components.latched_group += priv.group_delta;
            /* XXX beep beep! */
        }
    }

    priv.latch = latch;
    filter->priv = priv.priv;

    প্রত্যাগম XKB_FILTER_CONTINUE;
}

স্থির শূন্য
xkb_filter_mod_set_new(সংরচনা xkb_server_state *state,
                       সংরচনা xkb_events *events,
                       সংরচনা xkb_filter *filter)
{
    ধ্রুব গণনা xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    যদি ((filter->action.mods.flags & unlock) == unlock) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension.
         */
        filter->priv = filter->action.mods.mods.mask
                     & ~state->base.components.locked_mods;
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
    } অন্যথা {
        filter->priv = filter->action.mods.mods.mask;
    }

    state->set_mods |= (xkb_mod_mask_t) filter->priv;
}

স্থির bool
xkb_filter_mod_set_func(সংরচনা xkb_server_state *state,
                        সংরচনা xkb_events *events,
                        সংরচনা xkb_filter *filter,
                        ধ্রুব সংরচনা xkb_চাবি *চাবি,
                        গণনা xkb_চাবি_direction direction)
{

    যদি (চাবি != filter->চাবি) {
        filter->action.mods.flags &= ~ACTION_LOCK_CLEAR;
        প্রত্যাগম XKB_FILTER_CONTINUE;
    }

    বিকল্প (direction) {
    পক্ষ XKB_চাবি_DOWN:
        filter->refcnt++;
        /* fallthrough */
    পক্ষ XKB_চাবি_REPEATED:
        প্রত্যাগম XKB_FILTER_CONSUME;
    স্বভাব:
        যদি (--filter->refcnt > 0)
            প্রত্যাগম XKB_FILTER_CONSUME;
    }

    state->clear_mods |= (xkb_mod_mask_t) filter->priv;
    ধ্রুব গণনা xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    যদি ((filter->action.mods.flags & unlock) == ACTION_LOCK_CLEAR)
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;

    filter->func = রিক্ত;
    প্রত্যাগম XKB_FILTER_CONTINUE;
}

স্থির শূন্য
xkb_filter_mod_lock_new(সংরচনা xkb_server_state *state,
                        সংরচনা xkb_events *events,
                        সংরচনা xkb_filter *filter)
{
    filter->priv = (state->base.components.locked_mods &
                    filter->action.mods.mods.mask);

    যদি (filter->priv && (filter->action.mods.flags & ACTION_UNLOCK_ON_PRESS)) {
        /*
         * Some of the target modifiers were locked before key press: unlock.
         *
         * This is a keymap v2 extension: unlock-on-press.
         */
        যদি (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
            state->base.components.locked_mods &= ~filter->priv;
        /* No further action: cancel filter */
        filter->func = রিক্ত;
    } অন্যথা {
        /* Set base mods; lock mods if relevant (XKB 1.0 spec) */
        state->set_mods |= filter->action.mods.mods.mask;
        যদি (!(filter->action.mods.flags & ACTION_LOCK_NO_LOCK))
            state->base.components.locked_mods |= filter->action.mods.mods.mask;
    }
}

স্থির bool
xkb_filter_mod_lock_func(সংরচনা xkb_server_state *state,
                         সংরচনা xkb_events *events,
                         সংরচনা xkb_filter *filter,
                         ধ্রুব সংরচনা xkb_চাবি *চাবি,
                         গণনা xkb_চাবি_direction direction)
{
    যদি (চাবি != filter->চাবি)
        প্রত্যাগম XKB_FILTER_CONTINUE;

    বিকল্প (direction) {
    পক্ষ XKB_চাবি_DOWN:
        filter->refcnt++;
        /* fallthrough */
    পক্ষ XKB_চাবি_REPEATED:
        প্রত্যাগম XKB_FILTER_CONSUME;
    স্বভাব:
        যদি (--filter->refcnt > 0)
            প্রত্যাগম XKB_FILTER_CONSUME;
    }

    state->clear_mods |= filter->action.mods.mods.mask;
    যদি (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
        state->base.components.locked_mods &= ~filter->priv;

    filter->func = রিক্ত;
    প্রত্যাগম XKB_FILTER_CONTINUE;
}

স্থির শূন্য
xkb_filter_mod_latch_new(সংরচনা xkb_server_state *state,
                         সংরচনা xkb_events *events,
                         সংরচনা xkb_filter *filter)
{
    /* Latch-on-press + clear-locks imply unlock-on-press */
    ধ্রুব গণনা xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                              | ACTION_LATCH_ON_PRESS;

    যদি ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
        (filter->action.mods.flags & unlockOnPress) &&
        (state->base.components.locked_mods & filter->action.mods.mods.mask) ==
         filter->action.mods.mods.mask) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension: clear locks and do not latch.
         */
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
        filter->func = রিক্ত;
    } অন্যথা যদি (filter->action.mods.flags & ACTION_LATCH_ON_PRESS) {
        /*
         * Latch on key press
         *
         * This is a keymap format v2 extension.
         */
        filter->priv = LATCH_PENDING;
        state->base.components.latched_mods |= filter->action.mods.mods.mask;
        /* XXX beep beep! */
    } অন্যথা {
        /* XKB standard latch action */
        filter->priv = LATCH_চাবি_DOWN;
        state->set_mods |= filter->action.mods.mods.mask;
    }
}

স্থির bool
xkb_filter_mod_latch_func(সংরচনা xkb_server_state *state,
                          সংরচনা xkb_events *events,
                          সংরচনা xkb_filter *filter,
                          ধ্রুব সংরচনা xkb_চাবি *চাবি,
                          গণনা xkb_চাবি_direction direction)
{
    গণনা xkb_চাবি_latch_state latch = filter->priv;

    যদি (direction == XKB_চাবি_DOWN) {
        ধ্রুব সঙ্ঘ xkb_action *actions = রিক্ত;
        ধ্রুব xkb_action_গুনতি_t গুনতি = xkb_চাবি_get_actions(&state->base, চাবি,
                                                             &actions);

        যদি (latch == LATCH_চাবি_DOWN) {
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
            যদি (state->a11y_flags & XKB_A11_Y_LATCH_SIMULTANEOUS_চাবিগুলি) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                প্রতি (xkb_action_গুনতি_t k = 0; k < গুনতি; k++) {
                    যদি (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_MOD_LATCH,
                                                filter->action.mods.mods.mask))
                    {
                        latch = NO_LATCH;
                        ভঙ্গ;
                    }
                }
            } অন্যথা {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        অন্যথা যদি (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is pressed,
             * or promote it to a lock or plain base set if it's the same
             * modifier. */
            ধ্রুব bool sticky_চাবিগুলি = state->base.components.controls
                                   & CONTROL_STICKY_চাবিগুলি;
            /*
             * NOTE: Action’s flags are not compared:
             * - flags may have been modified by keyboard controls at the
             *   creation of the filter;
             * - it is unlikely to have two LatchMods() actions on the same
             *   key that differ only by their flags;
             * - even if it were the case, one would most likely want both
             *   actions to be considered compatible in this context.
             */
            প্রতি (xkb_action_গুনতি_t k = 0; k < গুনতি; k++) {
                যদি (((actions[k].type == ACTION_TYPE_MOD_LATCH) ||
                     (actions[k].type == ACTION_TYPE_MOD_SET && sticky_চাবিগুলি)) &&
                    actions[k].mods.mods.mask == filter->action.mods.mods.mask)
                {
                    যদি (filter->action.mods.flags & ACTION_LATCH_TO_LOCK) {
                        /* Mutate the action to LockMods() */
                        filter->action.type = ACTION_TYPE_MOD_LOCK;
                        filter->func = xkb_filter_mod_lock_func;
                        xkb_filter_mod_lock_new(state, events, filter);
                    }
                    অন্যথা {
                        /* Mutate the action to SetMods() */
                        filter->action.type = ACTION_TYPE_MOD_SET;
                        filter->func = xkb_filter_mod_set_func;
                        xkb_filter_mod_set_new(state, events, filter);
                    }
                    filter->চাবি = চাবি;
                    /* Clear latches */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    /* XXX beep beep! */
                    প্রত্যাগম XKB_FILTER_CONSUME;
                }
                অন্যথা যদি (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_MOD_LATCH,
                                                 filter->action.mods.mods.mask))
                {
                    /* XXX: This may be totally broken, we might need to break the
                     *      latch in the next run after this press? */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    filter->func = রিক্ত;
                    প্রত্যাগম XKB_FILTER_CONTINUE;
                }
            }
        }
        অন্যথা {
            /* Ignore press in NO_LATCH state */
            প্রতিজ্ঞা(latch == NO_LATCH);
        }
    }
    অন্যথা যদি (চাবি != filter->চাবি) {
        /* Ignore repeat/release of other keys */
    }
    অন্যথা যদি (direction == XKB_চাবি_REPEATED) {
        প্রত্যাগম XKB_FILTER_CONSUME; // TODO: check
    }
    অন্যথা {
        /* Our key got released.  If we’ve set it to clear locks, and we
         * currently have the same modifiers locked, then release them and
         * don't actually latch.  Else we’ve actually hit the latching
         * stage, so set PENDING and move our modifier from base to
         * latched. */

        /* Latch-on-press + clear-locks imply unlock-on-press */
        ধ্রুব গণনা xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                                  | ACTION_LATCH_ON_PRESS;

        যদি ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
            !(filter->action.mods.flags & unlockOnPress) &&
            (state->base.components.locked_mods &
             filter->action.mods.mods.mask) == filter->action.mods.mods.mask)
        {
            /* XXX: We might be a bit overenthusiastic about clearing
             *      mods other filters have set here? */
            যদি (latch == LATCH_PENDING)
                state->base.components.latched_mods &=
                    ~filter->action.mods.mods.mask;
            অন্যথা
                state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
            filter->func = রিক্ত;
        }
        অন্যথা যদি (latch == NO_LATCH) {
            /* Broken latch */
            state->clear_mods |= filter->action.mods.mods.mask;
            filter->func = রিক্ত;
        }
        অন্যথা যদি (!(filter->action.mods.flags & ACTION_LATCH_ON_PRESS)) {
            latch = LATCH_PENDING;
            state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.latched_mods |= filter->action.mods.mods.mask;
            /* XXX beep beep! */
        }
    }

    filter->priv = latch;

    প্রত্যাগম XKB_FILTER_CONTINUE;
}

স্থির শূন্য
append_pointer_move(সংরচনা xkb_events *events,
                    সংরচনা xkb_filter *filter)
{
    গণনা xkb_pointer_motion_flags flags = 0;
    যদি (filter->action.ptr.flags & ACTION_ABSOLUTE_X)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_X;
    যদি (filter->action.ptr.flags & ACTION_ABSOLUTE_Y)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_Y;
    যদি (filter->action.ptr.flags & ACTION_REPEAT)
        flags |= XKB_POINTER_MOTION_REPEATS;
    darray_append(events->queue, (সংরচনা xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_MOTION,
        .pointer_motion = {
            .size = পরিমাণ(((সংরচনা xkb_event*)0)->pointer_motion),
            .flags = flags,
            .x = filter->action.ptr.x,
            .y = filter->action.ptr.y,
        }
    });
}

স্থির শূন্য
xkb_filter_pointer_move_new(সংরচনা xkb_server_state *state,
                            সংরচনা xkb_events *events,
                            সংরচনা xkb_filter *filter)
{
    যদি (!events) {
        filter->func = রিক্ত;
        প্রত্যাগম;
    }

    প্রতিজ্ঞা(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_চাবিগুলি);
    append_pointer_move(events, filter);
    state->flags &= ~STATE_REQUIRE_চাবি_EVENT;
}

স্থির bool
xkb_filter_pointer_move_func(সংরচনা xkb_server_state *state,
                             সংরচনা xkb_events *events,
                             সংরচনা xkb_filter *filter,
                             ধ্রুব সংরচনা xkb_চাবি *চাবি,
                             গণনা xkb_চাবি_direction direction)
{
    যদি (চাবি != filter->চাবি || !events)
        প্রত্যাগম XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_চাবি_EVENT;

    বিকল্প (direction) {
    পক্ষ XKB_চাবি_DOWN:
        filter->refcnt++;
        append_pointer_move(events, filter);
        প্রত্যাগম XKB_FILTER_CONSUME;
    পক্ষ XKB_চাবি_REPEATED:
        যদি (filter->action.ptr.flags & ACTION_REPEAT)
            append_pointer_move(events, filter);
        প্রত্যাগম XKB_FILTER_CONSUME;
    স্বভাব:
        যদি (--filter->refcnt > 0)
            প্রত্যাগম XKB_FILTER_CONSUME;
    }

    filter->func = রিক্ত;
    প্রত্যাগম XKB_FILTER_CONSUME;
}

/* Implemented with xkb_machine API because it requires struct xkb_machine */
স্থির শূন্য
xkb_filter_pointer_button_new(সংরচনা xkb_server_state *state,
                              সংরচনা xkb_events *events,
                              সংরচনা xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
স্থির bool
xkb_filter_pointer_button_func(সংরচনা xkb_server_state *state,
                               সংরচনা xkb_events *events,
                               সংরচনা xkb_filter *filter,
                               ধ্রুব সংরচনা xkb_চাবি *চাবি,
                               গণনা xkb_চাবি_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
স্থির শূন্য
xkb_filter_pointer_lock_button_new(সংরচনা xkb_server_state *state,
                                   সংরচনা xkb_events *events,
                                   সংরচনা xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
স্থির bool
xkb_filter_pointer_lock_button_func(সংরচনা xkb_server_state *state,
                                    সংরচনা xkb_events *events,
                                    সংরচনা xkb_filter *filter,
                                    ধ্রুব সংরচনা xkb_চাবি *চাবি,
                                    গণনা xkb_চাবি_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
স্থির শূন্য
xkb_filter_pointer_set_default_button_new(সংরচনা xkb_server_state *state,
                                          সংরচনা xkb_events *events,
                                          সংরচনা xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
স্থির bool
xkb_filter_pointer_set_default_button_func(সংরচনা xkb_server_state *state,
                                           সংরচনা xkb_events *events,
                                           সংরচনা xkb_filter *filter,
                                           ধ্রুব সংরচনা xkb_চাবি *চাবি,
                                           গণনা xkb_চাবি_direction direction);

স্থির শূন্য
xkb_filter_terminate_server_new(সংরচনা xkb_server_state *state,
                                 সংরচনা xkb_events *events,
                                 সংরচনা xkb_filter *filter);

স্থির bool
xkb_filter_terminate_server_func(সংরচনা xkb_server_state *state,
                                  সংরচনা xkb_events *events,
                                  সংরচনা xkb_filter *filter,
                                  ধ্রুব সংরচনা xkb_চাবি *চাবি,
                                  গণনা xkb_চাবি_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
স্থির শূন্য
xkb_filter_switch_virtual_console_new(সংরচনা xkb_server_state *state,
                                      সংরচনা xkb_events *events,
                                      সংরচনা xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
স্থির bool
xkb_filter_switch_virtual_console_func(সংরচনা xkb_server_state *state,
                                       সংরচনা xkb_events *events,
                                       সংরচনা xkb_filter *filter,
                                       ধ্রুব সংরচনা xkb_চাবি *চাবি,
                                       গণনা xkb_চাবি_direction direction);

স্থির অন্তঃস্থ শূন্য
clear_all_latches_and_locks(সংরচনা xkb_server_state *state,
                            সংরচনা xkb_events *events);

স্থির শূন্য
xkb_filter_ctrls_new(সংরচনা xkb_server_state *state,
                     সংরচনা xkb_events *events,
                     সংরচনা xkb_filter *filter)
{
    যদি (filter->action.type == ACTION_TYPE_CTRL_SET) {
        /* Set: save the specified controls that are *not* already enabled */
        filter->priv = (uint32_t) (
            (~state->base.components.controls) & filter->action.ctrls.ctrls
        );
    } অন্যথা {
        /* Lock: save the specified controls that *are* already enabled */
        filter->priv = (uint32_t) (
            state->base.components.controls & filter->action.ctrls.ctrls
        );
    }

    যদি (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_LOCK)) {
        /* Enable the specified controls that are not already enabled */
        state->base.components.controls |= filter->action.ctrls.ctrls;
    }
}

স্থির bool
xkb_filter_ctrls_func(সংরচনা xkb_server_state *state,
                      সংরচনা xkb_events *events,
                      সংরচনা xkb_filter *filter,
                      ধ্রুব সংরচনা xkb_চাবি *চাবি,
                      গণনা xkb_চাবি_direction direction)
{
    যদি (চাবি != filter->চাবি)
        প্রত্যাগম XKB_FILTER_CONTINUE;

    বিকল্প (direction) {
    পক্ষ XKB_চাবি_DOWN:
        filter->refcnt++;
        /* fallthrough */
    পক্ষ XKB_চাবি_REPEATED:
        প্রত্যাগম XKB_FILTER_CONSUME;
    স্বভাব:
        যদি (--filter->refcnt > 0)
            প্রত্যাগম XKB_FILTER_CONSUME;
    }

    যদি (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_UNLOCK)) {
        ধ্রুব গণনা xkb_action_controls old = state->base.components.controls;

        /*
         * Set: Disable specified controls that were *not* enabled at key press.
         * Lock: Disable specified controls that *were* enabled at key press.
         */
        state->base.components.controls &= ~(গণনা xkb_action_controls) filter->priv;

        যদি ((old & CONTROL_STICKY_চাবিগুলি) &&
            !(state->base.components.controls & CONTROL_STICKY_চাবিগুলি)) {
            /* Sticky keys were disabled: clear all locks and latches */
            clear_all_latches_and_locks(state, events);
        }
    }

    filter->func = রিক্ত;
    প্রত্যাগম XKB_FILTER_CONTINUE;
}

স্থির bool
append_redirect_চাবি_events(সংরচনা xkb_server_state *state,
                           সংরচনা xkb_events *events,
                           ধ্রুব সংরচনা xkb_redirect_চাবি_action *redirect,
                           গণনা xkb_চাবি_direction direction)
{
    গণনা xkb_state_component changed = 0;
    ধ্রুব xkb_mod_mask_t mask = redirect->affect_mods;

    /*
     * Reference state: find the last state update in the queue, otherwise
     * use the current state.
     */
    সংরচনা xkb_event *event;
    সংরচনা state_components last_components = state->base.components;
    darray_foreach_reverse(event, events->queue) {
        যদি (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
            last_components = event->components.components;
            ভঙ্গ;
        }
    }

    যদি (mask) {
        সংরচনা state_components new = last_components;
        new.base_mods = (new.base_mods & ~mask) | redirect->mods;
        new.latched_mods = (new.latched_mods & ~mask) | redirect->mods;
        new.locked_mods = (new.locked_mods & ~mask) | redirect->mods;
        new.mods = (new.mods & ~mask) | redirect->mods;

        changed = get_state_component_changes(&last_components, &new);
        যদি (changed) {
            darray_append(events->queue, (সংরচনা xkb_event) {
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
            darray_append(events->queue, (সংরচনা xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_FRAME,
            });
        }
    }

    darray_append(events->queue, (সংরচনা xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_চাবি,
        .চাবি = {
            .keycode = redirect->keycode,
            .direction = direction
        }
    });

    যদি (mask && changed) {
        /* Restore state */
        darray_append(events->queue, (সংরচনা xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = last_components,
                .changed = changed
            }
        });
    }

    state->flags &= ~STATE_REQUIRE_চাবি_EVENT;
    প্রত্যাগম true;
}

স্থির শূন্য
xkb_filter_redirect_চাবি_new(সংরচনা xkb_server_state *state,
                            সংরচনা xkb_events *events,
                            সংরচনা xkb_filter *filter)
{
    /* Action effectual only with the xkb_machine API and a valid keycode */
    যদি (!events || filter->action.redirect.keycode == XKB_KEYCODE_INVALID) {
        filter->func = রিক্ত;
        প্রত্যাগম;
    }
    append_redirect_চাবি_events(state, events, &filter->action.redirect,
                               XKB_চাবি_DOWN);
}

স্থির bool
xkb_filter_redirect_চাবি_func(সংরচনা xkb_server_state *state,
                             সংরচনা xkb_events *events,
                             সংরচনা xkb_filter *filter,
                             ধ্রুব সংরচনা xkb_চাবি *চাবি,
                             গণনা xkb_চাবি_direction direction)
{
    যদি (চাবি != filter->চাবি)
        প্রত্যাগম XKB_FILTER_CONTINUE;

    যদি (direction == XKB_চাবি_UP) {
        append_redirect_চাবি_events(state, events,
                                   &filter->action.redirect, XKB_চাবি_UP);
        filter->func = রিক্ত;
        প্রত্যাগম XKB_FILTER_CONSUME;
     } অন্যথা যদি (direction == XKB_চাবি_DOWN) {
        ধ্রুব সঙ্ঘ xkb_action *actions = রিক্ত;
        ধ্রুব xkb_action_গুনতি_t গুনতি =
            xkb_চাবি_get_actions(&state->base, চাবি, &actions);

        প্রতি (xkb_action_গুনতি_t a = 0; a < গুনতি; a++) {
            যদি (actions[a].type == ACTION_TYPE_REDIRECT_চাবি &&
                actions[a].redirect.keycode != filter->action.redirect.keycode) {
                    /* One action redirects to a different key: release first */
                    append_redirect_চাবি_events(state, events,
                                               &filter->action.redirect,
                                               XKB_চাবি_UP);
                filter->func = রিক্ত;
                প্রত্যাগম XKB_FILTER_CONTINUE;
            }
        }
    }

    append_redirect_চাবি_events(state, events, &filter->action.redirect,
                               direction);
    প্রত্যাগম XKB_FILTER_CONSUME;
}

স্থির ধ্রুব সংরচনা {
    শূন্য (*new)(সংরচনা xkb_server_state *state,
                সংরচনা xkb_events *events,
                সংরচনা xkb_filter *filter);
    bool (*func)(সংরচনা xkb_server_state *state,
                 সংরচনা xkb_events *events,
                 সংরচনা xkb_filter *filter,
                 ধ্রুব সংরচনা xkb_চাবি *চাবি, গণনা xkb_চাবি_direction direction);
} filter_action_funcs[ACTION_TYPE_অঙ্ক_ENTRIES] = {
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
    [ACTION_TYPE_REDIRECT_চাবি] = { xkb_filter_redirect_চাবি_new,
                                   xkb_filter_redirect_চাবি_func },
};

/**
 * Applies any relevant filters to the key, first from the list of filters
 * that are currently active, then if no filter has claimed the key, possibly
 * apply a new filter from the key action.
 */
স্থির শূন্য
xkb_filter_apply_all(সংরচনা xkb_server_state *state,
                     সংরচনা xkb_events *events,
                     ধ্রুব সংরচনা xkb_চাবি *চাবি,
                     গণনা xkb_চাবি_direction direction)
{
    /* First run through all the currently active filters and see if any of
     * them have consumed this event. */
    bool consumed = false;
    সংরচনা xkb_filter *filter;
    darray_foreach(filter, state->filters) {
        যদি (!filter->func)
            অনুবর্ত;

        যদি (filter->func(state, events, filter, চাবি, direction) == XKB_FILTER_CONSUME)
            consumed = true;
    }
    যদি (consumed || direction == XKB_চাবি_UP)
        প্রত্যাগম;

    /* No filter consumed this event, so proceed with the key actions */
    ধ্রুব সঙ্ঘ xkb_action *actions = রিক্ত;
    xkb_action_গুনতি_t গুনতি = xkb_চাবি_get_actions(&state->base, চাবি,
                                                         &actions);

    প্রতি (সংরচনা {
            xkb_action_গুনতি_t গুনতি;
            ধ্রুব সঙ্ঘ xkb_action *actions;
         } next = {0};
         গুনতি;
         গুনতি = next.গুনতি, next.গুনতি = 0, actions = next.actions
    ) {
        /*
         * Process actions sequentially.
         *
         * NOTE: We rely on the parser to disallow multiple modifier or group
         * actions (see `CheckMultipleActionsCategories`). Allowing multiple
         * such actions requires a refactor of the state handling.
         */
        প্রতি (xkb_action_গুনতি_t k = 0; k < গুনতি; k++) {
            /*
             * It's possible for the keymap to set action->type explicitly, like
             * so:
             *     interpret XF86_Next_VMode {
             *         action = Private(type=0x86, data="+VMode");
             *     };
             * We don't handle those.
             */
            যদি (actions[k].type >= ACTION_TYPE_অঙ্ক_ENTRIES)
                অনুবর্ত;

            /* Go to next action if no corresponding action handler */
            যদি (!filter_action_funcs[actions[k].type].new)
                অনুবর্ত;

            /* Add a new filter and run the corresponding initial action */
            filter = xkb_filter_new(state);
            filter->চাবি = চাবি;
            filter->action = actions[k];

            বিকল্প (filter->action.type) {
            পক্ষ ACTION_TYPE_MOD_SET:
                যদি (state->base.components.controls & CONTROL_STICKY_চাবিগুলি) {
                    /* Convert modifier set action to a latch */
                    filter->action.type = ACTION_TYPE_MOD_LATCH;
                    যদি (state->a11y_flags & XKB_A11_Y_STICKY_চাবিগুলি_LATCH_TO_LOCK) {
                        filter->action.mods.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                /* fallthrough */
            পক্ষ ACTION_TYPE_MOD_LATCH:
            পক্ষ ACTION_TYPE_MOD_LOCK: {
                স্থির ধ্রুব গণনা xkb_action_flags unlock_mods_on_press
                    = ACTION_LOCK_CLEAR     /* SetMods & LatchMods: can unlock */
                    | ACTION_LOCK_NO_UNLOCK /* LockMods: cannot unlock */
                    | ACTION_EXPLICIT_UNLOCK_ON_PRESS;
                যদি ((state->flags & STATE_UNLOCK_MODS_ON_PRESS) &&
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
                ভঙ্গ;
            }
            পক্ষ ACTION_TYPE_GROUP_SET:
                যদি (state->base.components.controls & CONTROL_STICKY_চাবিগুলি) {
                    /* Convert group set action to a latch */
                    filter->action.type = ACTION_TYPE_GROUP_LATCH;
                    যদি (state->a11y_flags & XKB_A11_Y_STICKY_চাবিগুলি_LATCH_TO_LOCK) {
                        filter->action.group.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                ভঙ্গ;
            পক্ষ ACTION_TYPE_GROUP_LOCK:
                যদি ((filter->action.group.flags &
                     ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE) &&
                    (state->flags &
                     STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_চাবি) &&
                    /*
                     * FIXME: sticky keys may leave an unexpected modifier
                     * from the level 0 latched, so disable for now.
                     */
                    !(state->base.components.controls & CONTROL_STICKY_চাবিগুলি))
                {
                    filter->action.group.flags |= ACTION_LOCK_ON_RELEASE;
                    /* Add base level action */
                    next.গুনতি = xkb_keymap_চাবি_get_actions_by_level(
                        state->base.keymap, চাবি, state->base.components.group,
                        0, &next.actions
                    );
                    প্রতিজ্ঞা(next.গুনতি == 1);
                }
                ভঙ্গ;
            পক্ষ ACTION_TYPE_PTR_MOVE:
            পক্ষ ACTION_TYPE_PTR_BUTTON:
            পক্ষ ACTION_TYPE_PTR_LOCK:
            পক্ষ ACTION_TYPE_PTR_DEFAULT:
                যদি (!(state->base.components.controls & CONTROL_MOUSE_চাবিগুলি)) {
                    /* Convert pointer actions to NoAction */
                    প্রতিজ্ঞা(!filter_action_funcs[ACTION_TYPE_NONE].new);
                    filter->func = রিক্ত;
                    অনুবর্ত;
                }
                ভঙ্গ;
            পক্ষ ACTION_TYPE_REDIRECT_চাবি:
                // FIXME: this is not efficient to resolve mods here each time
                filter->action.redirect.affect_mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.affect_mods
                );
                filter->action.redirect.mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.mods
                );
                ভঙ্গ;
            স্বভাব: ;
            }
            filter->func = filter_action_funcs[filter->action.type].func;
            filter_action_funcs[filter->action.type].new(state, events, filter);
        }
    }
}

স্থির শূন্য
xkb_state_update_derived(সংরচনা xkb_state *state);

স্থির অন্তঃস্থ শূন্য
xkb_state_init(সংরচনা xkb_state * restrict state,
               সংরচনা xkb_keymap * restrict keymap,
               গণনা xkb_state_mode_internal mode)
{
    state->mode = mode;
    state->refcnt = 1;
    state->keymap = xkb_keymap_ref(keymap);
    state->out_of_range_group.policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

    /* Ensure that derived state is correctly initialized */
    xkb_state_update_derived(state);
}

স্থির অন্তঃস্থ শূন্য
xkb_client_state_init(সংরচনা xkb_client_state * restrict state,
                      সংরচনা xkb_keymap * restrict keymap,
                      গণনা xkb_state_mode_internal mode)
{
    xkb_state_init(&state->base, keymap, mode);
}

স্থির অন্তঃস্থ শূন্য
xkb_server_state_init(সংরচনা xkb_server_state * restrict state,
                      সংরচনা xkb_keymap * restrict keymap,
                      গণনা xkb_state_mode_internal mode,
                      গণনা xkb_a11y_flags a11y_affect,
                      গণনা xkb_a11y_flags a11y_flags)
{
    xkb_state_init(&state->base, keymap, mode);

    state->a11y_flags = a11y_flags;
    যদি (keymap->format != XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !(a11y_affect & XKB_A11_Y_LATCH_SIMULTANEOUS_চাবিগুলি)) {
             /* Keymap v2+: enable extension to XKB if not manually disabled */
             state->a11y_flags |= XKB_A11_Y_LATCH_SIMULTANEOUS_চাবিগুলি;
    }
}

সংরচনা xkb_state *
xkb_state_new(সংরচনা xkb_keymap *keymap)
{
    সংরচনা xkb_server_state * ধ্রুব state = calloc(1, পরিমাণ(*state));
    যদি (!state) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "Could not allocate a state object.\n");
        প্রত্যাগম রিক্ত;
    }

    xkb_server_state_init(state, keymap, LEGACY_MIXED_STATE, 0, 0);

    প্রত্যাগম (সংরচনা xkb_state *)state;
}

সংরচনা xkb_state *
xkb_state_new_with_mode(সংরচনা xkb_keymap * restrict keymap,
                        গণনা xkb_state_mode mode,
                        গণনা xkb_status * restrict status)
{
    বিকল্প (mode) {
    পক্ষ XKB_STATE_MODE_CLIENT:
    পক্ষ XKB_STATE_MODE_SERVER_QUERY: {
        সংরচনা xkb_client_state * ধ্রুব state = calloc(1, পরিমাণ(*state));
        যদি (!state)
            ভঙ্গ;

        static_assert((unsigned)XKB_STATE_MODE_CLIENT ==
                      (unsigned)CLIENT_STATE, "");
        static_assert((unsigned)XKB_STATE_MODE_SERVER_QUERY ==
                      (unsigned)SERVER_COMPANION, "");
        xkb_client_state_init(state, keymap, (গণনা xkb_state_mode_internal)mode);

        যদি (status)
            *status = XKB_SUCCESS;

        প্রত্যাগম (সংরচনা xkb_state *)state;
    }
    পক্ষ XKB_STATE_MODE_SERVER: {
        সংরচনা xkb_server_state * ধ্রুব state = calloc(1, পরিমাণ(*state));
        যদি (!state)
            ভঙ্গ;

        xkb_server_state_init(state, keymap, LEGACY_SERVER_STATE, 0, 0);

        যদি (status)
            *status = XKB_SUCCESS;

        প্রত্যাগম (সংরচনা xkb_state *)state;
    }
    স্বভাব:
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_STATE_MODE_,
                     "Unsupported state mode: %u\n", mode);
        যদি (status)
            *status = XKB_ERROR_UNSUPPORTED_STATE_MODE;
        প্রত্যাগম রিক্ত;
    }

    log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                  "Could not allocate a state object.\n");
    যদি (status)
        *status = XKB_ERROR_ALLOCATION_FAILURE;
    প্রত্যাগম রিক্ত;
}

সংরচনা xkb_state *
xkb_state_ref(সংরচনা xkb_state *state)
{
    প্রতিজ্ঞা(state->refcnt > 0);
    state->refcnt++;
    প্রত্যাগম state;
}

শূন্য
xkb_state_reset(সংরচনা xkb_state *state)
{
    memset(&state->components, 0, পরিমাণ(state->components));

    যদি (state->mode < LEGACY_MIXED_STATE) {
        static_assert(পরিমাণ(*state) == পরিমাণ(সংরচনা xkb_client_state),
                      "Not a mere wrapper");
        /* Nothing else to reset */
    } অন্যথা {
        সংরচনা xkb_server_state *state_ = (সংরচনা xkb_server_state *)state;
        ধ্রুব সংরচনা xkb_server_state prev = *state_;
        memset(state_, 0, পরিমাণ(*state_));

        state_->base = prev.base;
        state_->a11y_flags = prev.a11y_flags;
        state_->filters = prev.filters;
        darray_size(state_->filters) = 0;
    }
}

স্থির অন্তঃস্থ শূন্য
xkb_state_destroy(সংরচনা xkb_state *state)
{
    xkb_keymap_unref(state->keymap);
    যদি (state->mode > SERVER_COMPANION)
        darray_free(((সংরচনা xkb_server_state *)state)->filters);
}

শূন্য
xkb_state_unref(সংরচনা xkb_state *state)
{
    প্রতিজ্ঞা(!state || state->refcnt > 0);
    যদি (!state || --state->refcnt > 0)
        প্রত্যাগম;

    xkb_state_destroy(state);

    বিকল্প (state->mode) {
    পক্ষ CLIENT_STATE:
    পক্ষ SERVER_COMPANION:
        মুক্ত((সংরচনা xkb_client_state *)state);
        ভঙ্গ;
    পক্ষ LEGACY_MIXED_STATE:
    পক্ষ LEGACY_SERVER_STATE:
        মুক্ত((সংরচনা xkb_server_state *)state);
        ভঙ্গ;
    পক্ষ SERVER_STATE: /* handled by xkb_machine_unref() */
    স্বভাব:
        PANIC_UNREACHABLE();
    }
}

সংরচনা xkb_keymap *
xkb_state_get_keymap(সংরচনা xkb_state *state)
{
    প্রত্যাগম state->keymap;
}

/**
 * Update the LED state to match the rest of the xkb_state.
 */
স্থির শূন্য
xkb_state_led_update_all(সংরচনা xkb_state *state)
{
    xkb_led_index_t idx;
    ধ্রুব সংরচনা xkb_led *led;

    state->components.leds = 0;

    xkb_leds_enumerate(idx, led, state->keymap) {

        যদি (led->which_mods != 0 && led->mods.mask != 0) {
            xkb_mod_mask_t mod_mask = 0;
            যদি (led->which_mods & XKB_STATE_MODS_EFFECTIVE)
                mod_mask |= state->components.mods;
            যদি (led->which_mods & XKB_STATE_MODS_DEPRESSED)
                mod_mask |= state->components.base_mods;
            যদি (led->which_mods & XKB_STATE_MODS_LATCHED)
                mod_mask |= state->components.latched_mods;
            যদি (led->which_mods & XKB_STATE_MODS_LOCKED)
                mod_mask |= state->components.locked_mods;

            যদি (led->mods.mask & mod_mask) {
                state->components.leds |= (UINT32_C(1) << idx);
                অনুবর্ত;
            }
        }

        যদি (led->which_groups != 0) {
            যদি (likely(led->groups) != 0) {
                xkb_layout_mask_t group_mask = 0;
                /* Effective and locked groups have been brought into range */
                প্রতিজ্ঞা(state->components.group < XKB_MAX_GROUPS);
                প্রতিজ্ঞা(state->components.locked_group >= 0 &&
                       state->components.locked_group < XKB_MAX_GROUPS);
                /* Effective and locked groups are used as mask */
                যদি (led->which_groups & XKB_STATE_LAYOUT_EFFECTIVE)
                    group_mask |= (UINT32_C(1) << state->components.group);
                যদি (led->which_groups & XKB_STATE_LAYOUT_LOCKED)
                    group_mask |= (UINT32_C(1) << state->components.locked_group);
                /* Base and latched groups only have to be non-zero */
                যদি ((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                    state->components.base_group != 0)
                    group_mask |= led->groups;
                যদি ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                    state->components.latched_group != 0)
                    group_mask |= led->groups;

                যদি (led->groups & group_mask) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    অনুবর্ত;
                }
            } অন্যথা {
                /* Special case for Base and latched groups */
                যদি (((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                     state->components.base_group == 0) ||
                    ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                     state->components.latched_group == 0)) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    অনুবর্ত;
                }
            }
        }

        যদি (led->ctrls & state->components.controls) {
            state->components.leds |= (UINT32_C(1) << idx);
            অনুবর্ত;
        }
    }
}

/**
 * Calculates the derived state (effective mods/group and LEDs) from an
 * up-to-date xkb_state.
 */
স্থির শূন্য
xkb_state_update_derived(সংরচনা xkb_state *state)
{
    xkb_layout_index_t wrapped;

    state->components.mods = (state->components.base_mods |
                              state->components.latched_mods |
                              state->components.locked_mods);

    /* Lock group must be adjusted, but not base nor latched groups */
    wrapped = XkbWrapGroupIntoRange(state->components.locked_group,
                                    state->keymap->অঙ্ক_groups,
                                    state->out_of_range_group.policy,
                                    state->out_of_range_group.redirect_group);
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->components.locked_group =
        (int32_t) (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    /* Effective group must be adjusted */
    wrapped = XkbWrapGroupIntoRange(state->components.base_group +
                                    state->components.latched_group +
                                    state->components.locked_group,
                                    state->keymap->অঙ্ক_groups,
                                    state->out_of_range_group.policy,
                                    state->out_of_range_group.redirect_group);
    state->components.group =
        (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    xkb_state_led_update_all(state);
}

স্থির শূন্য
state_update_enabled_controls(সংরচনা xkb_server_state *state,
                              গণনা xkb_keyboard_control_flags affect_controls,
                              গণনা xkb_keyboard_control_flags controls,
                              সংরচনা xkb_events *events)
{
    ধ্রুব bool had_sticky_চাবিগুলি = state->base.components.controls
                               & CONTROL_STICKY_চাবিগুলি;

    /*
     * Enable using the public API with the all the Control values, except
     * the internal ones, if any.
     */
    affect_controls =
        ( affect_controls
        & (গণনা xkb_keyboard_control_flags)CONTROL_ALL_BOOLEAN );
    state->base.components.controls &=
        (গণনা xkb_action_controls)~affect_controls;
    state->base.components.controls |=
        (গণনা xkb_action_controls)(controls & affect_controls);

    যদি (had_sticky_চাবিগুলি &&
        !(state->base.components.controls & CONTROL_STICKY_চাবিগুলি)) {
        /* Sticky keys were disabled: clear all locks and latches */
        clear_all_latches_and_locks(state, events);
    }

    xkb_state_update_derived(&state->base);
}

/**
 * Given a particular key event, updates the state structure to reflect the
 * new modifiers.
 */
গণনা xkb_state_component
xkb_state_update_চাবি(সংরচনা xkb_state *base_state, xkb_keycode_t kc,
                     গণনা xkb_চাবি_direction direction)
{
    /* Guard against client-only state */
    প্রতিজ্ঞা(base_state->mode > SERVER_COMPANION);
    যদি (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        প্রত্যাগম 0;
    }

    সংরচনা xkb_server_state * ধ্রুব state =
        (সংরচনা xkb_server_state *)base_state;

    ধ্রুব সংরচনা xkb_চাবি* ধ্রুব চাবি = Xkb_চাবি(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    যদি (!চাবি || (direction == XKB_চাবি_REPEATED && !চাবি->repeats))
        প্রত্যাগম 0;

    যদি (direction == XKB_চাবি_DOWN &&
        (state->base.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_চাবিগুলি) &&
        (state->a11y_flags & XKB_A11_Y_STICKY_চাবিগুলি_NO_SIMULTANEOUS_চাবিগুলি) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_চাবিগুলি,
                                      0, রিক্ত);
    }

    ধ্রুব সংরচনা state_components prev_components = state->base.components;

    state->set_mods = 0;
    state->clear_mods = 0;

    xkb_filter_apply_all(state, রিক্ত, চাবি, direction);

    xkb_mod_index_t i;
    xkb_mod_mask_t bit;
    প্রতি (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        যদি (state->set_mods & bit) {
            state->mod_চাবি_গুনতি[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    প্রতি (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        যদি (state->clear_mods & bit) {
            state->mod_চাবি_গুনতি[i]--;
            যদি (state->mod_চাবি_গুনতি[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_চাবি_গুনতি[i] = 0;
            }
            state->clear_mods &= ~bit;
        }
    }

    xkb_state_update_derived(&state->base);

    প্রত্যাগম get_state_component_changes(&prev_components, &state->base.components);
}

/* We need fake keys for `update_latch_modifiers` and `update_latch_group`.
 * These keys must have at least one level in order to break latches. We need 2
 * keys with specific actions in order to update group/mod latches without
 * affecting each other. */
স্থির সংরচনা xkb_চাবি_type_entry synthetic_চাবি_level_entry = { 0 };
স্থির সংরচনা xkb_চাবি_type synthetic_চাবি_type = {
     .অঙ্ক_entries = 1,
     .অঙ্ক_levels = 1,
     .entries = &synthetic_চাবি_level_entry
};
স্থির ধ্রুব সংরচনা xkb_চাবি synthetic_চাবি = { 0 };

/* Transcription from xserver: XkbLatchModifiers */
স্থির শূন্য
update_latch_modifiers(সংরচনা xkb_server_state *state,
                       সংরচনা xkb_events *events,
                       xkb_mod_mask_t mask, xkb_mod_mask_t latches)
{
    /* Clear affected latched modifiers */
    ধ্রুব xkb_mod_mask_t clear = mask & ~latches;
    state->base.components.latched_mods &= ~clear;

    /* Clear any pending latch to locks using ad hoc action:
     * only affect corresponding modifier latches and no group latch. */
    সংরচনা xkb_level synthetic_চাবি_level_break_mod_latch = {
        .অঙ্ক_syms = 0,
        .অঙ্ক_actions = 1,
        .upper = XKB_চাবি_No_প্রতীক,
        .s.sym = XKB_চাবি_No_প্রতীক,
        .a.action.internal = {
            .type = ACTION_TYPE_INTERNAL,
            .flags = INTERNAL_BREAKS_MOD_LATCH,
            .clear_latched_mods = clear
        }
    };
    সংরচনা xkb_group synthetic_চাবি_group_break_mod_latch = {
        .type = &synthetic_চাবি_type,
        .levels = &synthetic_চাবি_level_break_mod_latch
    };
    ধ্রুব সংরচনা xkb_চাবি synthetic_চাবি_break_mod_latch = {
        .অঙ্ক_groups = 1,
        .groups = &synthetic_চাবি_group_break_mod_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_চাবি_break_mod_latch, XKB_চাবি_DOWN);

    /* Finally set the latched mods by simulate tapping a key with the
     * corresponding action */
    ধ্রুব সংরচনা xkb_চাবি* ধ্রুব চাবি = &synthetic_চাবি;
    ধ্রুব সঙ্ঘ xkb_action latch_mods = {
        .mods = {
            .type = ACTION_TYPE_MOD_LATCH,
            .mods = { .mask = mask & latches },
            .flags = 0,
        },
    };
    সংরচনা xkb_filter* ধ্রুব filter = xkb_filter_new(state);
    filter->চাবি = চাবি;
    filter->func = xkb_filter_mod_latch_func;
    filter->action = latch_mods;
    xkb_filter_mod_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_mod_latch_func(state, events, filter, চাবি, XKB_চাবি_UP);
}

/* Transcription from xserver: XkbLatchGroup */
স্থির শূন্য
update_latch_group(সংরচনা xkb_server_state *state,
                   সংরচনা xkb_events *events, int32_t group)
{
    /* Clear any pending latch to locks. */
    স্থির সংরচনা xkb_level synthetic_চাবি_level_break_group_latch = {
        .অঙ্ক_syms = 0,
        .অঙ্ক_actions = 1,
        .upper = XKB_চাবি_No_প্রতীক,
        .s.sym = XKB_চাবি_No_প্রতীক,
        .a.action.internal = {
            .type = ACTION_TYPE_INTERNAL,
            .flags = INTERNAL_BREAKS_GROUP_LATCH,
            .clear_latched_mods = 0
        }
    };
    স্থির সংরচনা xkb_group synthetic_চাবি_group_break_group_latch = {
        .type = &synthetic_চাবি_type,
        .levels = &synthetic_চাবি_level_break_group_latch
    };
    স্থির ধ্রুব সংরচনা xkb_চাবি synthetic_চাবি_break_group_latch = {
        .অঙ্ক_groups = 1,
        .groups = &synthetic_চাবি_group_break_group_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_চাবি_break_group_latch, XKB_চাবি_DOWN);

    /* Simulate tapping a key with a group latch action, but in isolation: i.e.
     * without affecting the other filters. */
    ধ্রুব সংরচনা xkb_চাবি* ধ্রুব চাবি = &synthetic_চাবি;
    ধ্রুব সঙ্ঘ xkb_action latch_group = {
        .group = {
            .type = ACTION_TYPE_GROUP_LATCH,
            .flags = ACTION_ABSOLUTE_SWITCH,
            .group = group,
        },
    };
    সংরচনা xkb_filter* ধ্রুব filter = xkb_filter_new(state);
    filter->চাবি = চাবি;
    filter->func = xkb_filter_group_latch_func;
    filter->action = latch_group;
    xkb_filter_group_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_group_latch_func(state, events, filter, চাবি, XKB_চাবি_UP);
}

/**
 * Compute the resolved effective mask of an arbitrary input.
 *
 * Contrary to `mod_mask_get_effective`, it resolves only modifiers not present
 * in the canonical mask, so that it enables `xkb_state_serialize_mods` to
 * round trip via `xkb_state_update_mask`.
 */
স্থির অন্তঃস্থ xkb_mod_mask_t
resolve_to_canonical_mods(সংরচনা xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    প্রত্যাগম
        /*
         * Keep canonical modifier mask.
         * It contains either real modifiers or canonical virtual modifiers.
         */
        (mods & keymap->canonical_state_mask) |
        /* Resolve other modifiers */
        mod_mask_get_effective(keymap,
                               mods & ~keymap->canonical_state_mask);
}

স্থির শূন্য
state_update_latched_locked(
    সংরচনা xkb_server_state * restrict state,
    ধ্রুব সংরচনা xkb_state_components_update * restrict update,
    সংরচনা xkb_events * restrict events
)
{
    /* Update locks */
    ধ্রুব xkb_mod_mask_t affect_locked_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_locked_mods);
    যদি ((update->components & XKB_STATE_MODS_LOCKED) &&
        affect_locked_mods) {
        ধ্রুব xkb_mod_mask_t locked_mods =
            resolve_to_canonical_mods(state->base.keymap, update->locked_mods);
        state->base.components.locked_mods &= ~affect_locked_mods;
        state->base.components.locked_mods |= locked_mods & affect_locked_mods;
    }
    যদি (update->components & XKB_STATE_LAYOUT_LOCKED) {
        state->base.components.locked_group = update->locked_layout;
    }

    /* Update latches */
    ধ্রুব xkb_mod_mask_t affect_latched_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_latched_mods);
    যদি ((update->components & XKB_STATE_MODS_LATCHED) &&
        affect_latched_mods) {
        ধ্রুব xkb_mod_mask_t latched_mods =
            resolve_to_canonical_mods(state->base.keymap, update->latched_mods);
        update_latch_modifiers(state, events, affect_latched_mods, latched_mods);
    }
    যদি (update->components & XKB_STATE_LAYOUT_LATCHED) {
        update_latch_group(state, events, update->latched_layout);
    }
}

গণনা xkb_state_component
xkb_state_update_latched_locked(সংরচনা xkb_state *base_state,
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
    প্রতিজ্ঞা(base_state->mode > SERVER_COMPANION);
    যদি (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        প্রত্যাগম 0;
    }

    সংরচনা xkb_server_state * ধ্রুব state =
        (সংরচনা xkb_server_state *)base_state;

    ধ্রুব সংরচনা state_components previous_components = state->base.components;
    ধ্রুব গণনা xkb_state_component components
        = ((affect_latched_mods || latched_mods) ? XKB_STATE_MODS_LATCHED : 0)
        | ((affect_locked_mods || locked_mods) ? XKB_STATE_MODS_LOCKED : 0)
        | (affect_latched_layout ? XKB_STATE_LAYOUT_LATCHED : 0)
        | (affect_locked_layout ? XKB_STATE_LAYOUT_LOCKED : 0);
    ধ্রুব সংরচনা xkb_state_components_update update = {
        .size = পরিমাণ(update),
        .components = components,
        .affect_latched_mods = affect_latched_mods,
        .latched_mods = latched_mods,
        .latched_layout = latched_layout,
        .affect_locked_mods = affect_locked_mods,
        .locked_mods = locked_mods,
        .locked_layout = locked_layout
    };
    state_update_latched_locked(state, &update, রিক্ত);
    xkb_state_update_derived(&state->base);
    প্রত্যাগম get_state_component_changes(&previous_components,
                                       &state->base.components);
}

স্থির অন্তঃস্থ শূন্য
clear_all_latches_and_locks(সংরচনা xkb_server_state *state,
                            সংরচনা xkb_events *events)
{
    স্থির ধ্রুব গণনা xkb_state_component components
        = XKB_STATE_MODS_LATCHED | XKB_STATE_MODS_LOCKED
        | XKB_STATE_LAYOUT_LATCHED | XKB_STATE_LAYOUT_LOCKED;
    ধ্রুব সংরচনা xkb_state_components_update update = {
        .size = পরিমাণ(update),
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

স্থির গণনা xkb_status
state_update_layout_policy(সংরচনা xkb_server_state *state,
                           ধ্রুব সংরচনা xkb_layout_policy_update *update)
{
    যদি (xkb_feature_supported(XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY,
                              update->policy)) {
        যদি (update->policy == XKB_LAYOUT_OUT_OF_RANGE_REDIRECT) {
            যদি (update->redirect < state->base.keymap->অঙ্ক_groups) {
                state->base.out_of_range_group.redirect_group =
                    update->redirect;
            } অন্যথা {
                log_err(state->base.keymap->ctx,
                        XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Layout policy: "
                        "unsupported layout index %"PRIu32" > %"PRIu32"\n",
                        update->redirect + 1, state->base.keymap->অঙ্ক_groups);
                প্রত্যাগম XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
            }
        }
        state->base.out_of_range_group.policy = update->policy;
        প্রত্যাগম XKB_SUCCESS;
    } অন্যথা {
        log_err(state->base.keymap->ctx,
                XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY_,
                "Unsupported layout policy: %d\n", update->policy);
        প্রত্যাগম XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY;
    }
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
স্থির গণনা xkb_status
check_state_update_abi_(সংরচনা xkb_context * restrict ctx,
                        ধ্রুব বর্ণ * restrict func,
                        ধ্রুব সংরচনা xkb_synthetic_update * restrict update)
{
    গণনা xkb_status status = XKB_SUCCESS;
    যদি ((status = xkb_check_state_abi(update)) ||
        (update->reserved0 != 0 &&
         (status = XKB_ERROR_ABI_FORWARD_COMPAT)) ||
        (update->components &&
         (status = xkb_check_state_abi(update->components))) ||
        (update->layout_policy &&
         (status = xkb_check_state_abi(update->layout_policy)))) {
        xkb_log_abi_error(ctx, func, status);
    }
    প্রত্যাগম status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

#define check_state_update_abi(ctx, update) \
    check_state_update_abi_(ctx, __func__, update)

গণনা xkb_status
xkb_state_update_synthetic(সংরচনা xkb_state * base_state,
                           ধ্রুব সংরচনা xkb_synthetic_update * update,
                           গণনা xkb_state_component *changed)
{
    /* Guard against client-only state */
    যদি (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        প্রত্যাগম XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    সংরচনা xkb_server_state * ধ্রুব state =
        (সংরচনা xkb_server_state *)base_state;

    /* Check ABI compatibility */
    গণনা xkb_status status = check_state_update_abi(state->base.keymap->ctx,
                                                    update);
    যদি (status != XKB_SUCCESS)
        প্রত্যাগম status;

    ধ্রুব সংরচনা state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    /* Update parametrized controls first */
    যদি (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        যদি (status != XKB_SUCCESS)
            প্রত্যাগম status;
    }

    যদি (update->components) {
        ধ্রুব সংরচনা xkb_state_components_update * ধ্রুব components =
            update->components;
        /* Update boolean controls first */
        যদি (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, রিক্ত);
        }

        state_update_latched_locked(state, components, রিক্ত);
    }

    xkb_state_update_derived(&state->base);

    যদি (changed) {
        *changed = get_state_component_changes(&previous_components,
                                               &state->base.components);
    }
    প্রত্যাগম XKB_SUCCESS;
}

/**
 * Updates the state from a set of explicit masks as gained from
 * xkb_state_serialize_mods and xkb_state_serialize_groups.  As noted in the
 * documentation for these functions in xkbcommon.h, this round-trip is
 * lossy, and should only be used to update a slave state mirroring the
 * master, e.g. in a client/server window system.
 */
গণনা xkb_state_component
xkb_state_update_mask(সংরচনা xkb_state *base_state,
                      xkb_mod_mask_t base_mods,
                      xkb_mod_mask_t latched_mods,
                      xkb_mod_mask_t locked_mods,
                      xkb_layout_index_t base_group,
                      xkb_layout_index_t latched_group,
                      xkb_layout_index_t locked_group)
{
    /* Guard against server-only and server companion state */
    প্রতিজ্ঞা(base_state->mode == CLIENT_STATE ||
           base_state->mode == LEGACY_MIXED_STATE);
    যদি (base_state->mode != CLIENT_STATE &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        প্রত্যাগম 0;
    }

    সংরচনা xkb_client_state * ধ্রুব state =
        (সংরচনা xkb_client_state *)base_state;

    ধ্রুব সংরচনা state_components prev_components = state->base.components;

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

    প্রত্যাগম get_state_component_changes(&prev_components, &state->base.components);
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Lock_Modifier
 */
স্থির bool
should_do_caps_transformation(সংরচনা xkb_state *state, xkb_keycode_t kc)
{
    প্রত্যাগম
        xkb_state_mod_index_is_active(state, XKB_MOD_INDEX_CAPS,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_index_is_consumed(state, kc, XKB_MOD_INDEX_CAPS) == 0;
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Control_Modifier
 */
স্থির bool
should_do_ctrl_transformation(সংরচনা xkb_state *state, xkb_keycode_t kc)
{
    প্রত্যাগম
        xkb_state_mod_index_is_active(state, XKB_MOD_INDEX_CTRL,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_index_is_consumed(state, kc, XKB_MOD_INDEX_CTRL) == 0;
}

/**
 * Provides the symbols to use for the given key and state.  Returns the
 * number of symbols pointed to in syms_out.
 */
পূর্ণাঙ্ক
xkb_state_চাবি_get_syms(সংরচনা xkb_state *state, xkb_keycode_t kc,
                       ধ্রুব xkb_keysym_t **syms_out)
{
    ধ্রুব xkb_layout_index_t layout = xkb_state_চাবি_get_layout(state, kc);
    যদি (layout == XKB_LAYOUT_INVALID)
        গচ্ছ err;

    ধ্রুব xkb_level_index_t level = xkb_state_চাবি_get_level(state, kc, layout);
    যদি (level == XKB_LEVEL_INVALID)
        গচ্ছ err;

    ধ্রুব সংরচনা xkb_চাবি* ধ্রুব চাবি = Xkb_চাবি(state->keymap, kc);

    যদি (!চাবি)
        গচ্ছ err;

    ধ্রুব সংরচনা xkb_level* ধ্রুব leveli =
        xkb_keymap_চাবি_get_level(state->keymap, চাবি, layout, level);

    যদি (!leveli)
        গচ্ছ err;

    ধ্রুব xkb_keysym_গুনতি_t অঙ্ক_syms = leveli->অঙ্ক_syms;
    যদি (অঙ্ক_syms == 0)
        গচ্ছ err;

    যদি (should_do_caps_transformation(state, kc)) {
        /* Only simple capitalization rules: keysyms count is unchanged. */
        যদি (অঙ্ক_syms > 1) {
            *syms_out = (leveli->has_upper)
                      ? leveli->s.syms + অঙ্ক_syms
                      : leveli->s.syms;
        } অন্যথা {
            *syms_out = &leveli->upper;
        }
    } অন্যথা {
        *syms_out = (অঙ্ক_syms > 1)
                  ? leveli->s.syms
                  : &leveli->s.sym;
    }
    প্রত্যাগম (পূর্ণাঙ্ক) অঙ্ক_syms;

err:
    *syms_out = রিক্ত;
    প্রত্যাগম 0;
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
স্থির বর্ণ
XkbToControl(বর্ণ ch)
{
    বর্ণ c = ch;

    যদি ((c >= '@' && c < '\177') || c == ' ')
        c &= 0x1F;
    অন্যথা যদি (c == '2')
        c = '\000';
    অন্যথা যদি (c >= '3' && c <= '7')
        c -= ('3' - '\033');
    অন্যথা যদি (c == '8')
        c = '\177';
    অন্যথা যদি (c == '/')
        c = '_' & 0x1F;
    প্রত্যাগম c;
}

/**
 * Provides either exactly one symbol, or XKB_KEY_NoSymbol.
 */
xkb_keysym_t
xkb_state_চাবি_get_one_sym(সংরচনা xkb_state *state, xkb_keycode_t kc)
{
    ধ্রুব xkb_keysym_t *syms = রিক্ত;
    ধ্রুব পূর্ণাঙ্ক অঙ্ক_syms = xkb_state_চাবি_get_syms(state, kc, &syms);

    যদি (অঙ্ক_syms != 1)
        প্রত্যাগম XKB_চাবি_No_প্রতীক;
    অন্যথা
        প্রত্যাগম syms[0];
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
স্থির xkb_keysym_t
get_one_sym_for_string(সংরচনা xkb_state *state, xkb_keycode_t kc)
{
    ধ্রুব xkb_layout_index_t layout = xkb_state_চাবি_get_layout(state, kc);
    ধ্রুব xkb_layout_index_t অঙ্ক_layouts =
        xkb_keymap_অঙ্ক_layouts_for_চাবি(state->keymap, kc);
    xkb_level_index_t level = xkb_state_চাবি_get_level(state, kc, layout);
    যদি (layout == XKB_LAYOUT_INVALID || অঙ্ক_layouts == 0 ||
        level == XKB_LEVEL_INVALID)
        প্রত্যাগম XKB_চাবি_No_প্রতীক;

    ধ্রুব xkb_keysym_t *syms = রিক্ত;
    পূর্ণাঙ্ক nsyms = xkb_keymap_চাবি_get_syms_by_level(state->keymap, kc,
                                                 layout, level, &syms);
    যদি (nsyms != 1)
        প্রত্যাগম XKB_চাবি_No_প্রতীক;
    xkb_keysym_t sym = syms[0];

    যদি (should_do_ctrl_transformation(state, kc) && sym > 127u) {
        প্রতি (xkb_layout_index_t i = 0; i < অঙ্ক_layouts; i++) {
            level = xkb_state_চাবি_get_level(state, kc, i);
            যদি (level == XKB_LEVEL_INVALID)
                অনুবর্ত;

            nsyms = xkb_keymap_চাবি_get_syms_by_level(state->keymap, kc,
                                                     i, level, &syms);
            যদি (nsyms == 1 && syms[0] <= 127u) {
                sym = syms[0];
                ভঙ্গ;
            }
        }
    }

    যদি (should_do_caps_transformation(state, kc)) {
        sym = xkb_keysym_to_upper(sym);
    }

    প্রত্যাগম sym;
}

পূর্ণাঙ্ক
xkb_state_চাবি_get_utf8(সংরচনা xkb_state *state, xkb_keycode_t kc,
                       বর্ণ *buffer, size_t size)
{
    পূর্ণাঙ্ক nsyms;
    ধ্রুব xkb_keysym_t *syms = রিক্ত;
    ধ্রুব xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    যদি (sym != XKB_চাবি_No_প্রতীক) {
        nsyms = 1; syms = &sym;
    }
    অন্যথা {
        nsyms = xkb_state_চাবি_get_syms(state, kc, &syms);
    }

    /* Make sure not to truncate in the middle of a UTF-8 sequence. */
    পূর্ণাঙ্ক offset = 0;
    বর্ণ tmp[XKB_KEYSYM_UTF8_MAX_SIZE];
    প্রতি (পূর্ণাঙ্ক i = 0; i < nsyms; i++) {
        পূর্ণাঙ্ক ret = xkb_keysym_to_utf8(syms[i], tmp, পরিমাণ(tmp));
        যদি (ret <= 0)
            গচ্ছ err_bad;

        ret--;
        যদি ((size_t)offset + (size_t)ret <= size)
            memcpy(buffer + offset, tmp, (size_t)ret);
        offset += ret;
    }

    যদি ((size_t)offset >= size)
        গচ্ছ err_trunc;
    buffer[offset] = '\0';

    যদি (!is_valid_utf8(buffer, (size_t)offset))
        গচ্ছ err_bad;

    যদি (offset == 1 && (unsigned পূর্ণাঙ্ক) buffer[0] <= 127u &&
        should_do_ctrl_transformation(state, kc))
        buffer[0] = XkbToControl(buffer[0]);

    প্রত্যাগম offset;

err_trunc:
    যদি (size > 0)
        buffer[size - 1] = '\0';
    প্রত্যাগম offset;

err_bad:
    যদি (size > 0)
        buffer[0] = '\0';
    প্রত্যাগম 0;
}

uint32_t
xkb_state_চাবি_get_utf32(সংরচনা xkb_state *state, xkb_keycode_t kc)
{
    ধ্রুব xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    uint32_t cp = xkb_keysym_to_utf32(sym);

    যদি (cp <= 127u && should_do_ctrl_transformation(state, kc))
        cp = (uint32_t) XkbToControl((বর্ণ) cp);

    প্রত্যাগম cp;
}

/**
 * Serializes the requested modifier state into an xkb_mod_mask_t, with all
 * the same disclaimers as in xkb_state_update_mask.
 */
স্থির অন্তঃস্থ xkb_mod_mask_t
serialize_mods(ধ্রুব সংরচনা state_components *components,
               গণনা xkb_state_component type)
{
    xkb_mod_mask_t ret = 0;

    যদি (type & XKB_STATE_MODS_EFFECTIVE)
        প্রত্যাগম components->mods;

    যদি (type & XKB_STATE_MODS_DEPRESSED)
        ret |= components->base_mods;
    যদি (type & XKB_STATE_MODS_LATCHED)
        ret |= components->latched_mods;
    যদি (type & XKB_STATE_MODS_LOCKED)
        ret |= components->locked_mods;

    প্রত্যাগম ret;
}

xkb_mod_mask_t
xkb_state_serialize_mods(সংরচনা xkb_state *state,
                         গণনা xkb_state_component type)
{
    প্রত্যাগম serialize_mods(&state->components, type);
}

/**
 * Serialises the requested group state, with all the same disclaimers as
 * in xkb_state_update_mask.
 */
স্থির অন্তঃস্থ xkb_layout_index_t
serialize_layout(ধ্রুব সংরচনা state_components *components,
                 গণনা xkb_state_component type)
{
    যদি (type & XKB_STATE_LAYOUT_EFFECTIVE)
        প্রত্যাগম components->group;

    int32_t ret = 0;
    যদি (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret += components->base_group;
    যদি (type & XKB_STATE_LAYOUT_LATCHED)
        ret += components->latched_group;
    যদি (type & XKB_STATE_LAYOUT_LOCKED)
        ret += components->locked_group;

    প্রত্যাগম (xkb_layout_index_t)ret;
}

xkb_layout_index_t
xkb_state_serialize_layout(সংরচনা xkb_state *state,
                           গণনা xkb_state_component type)
{
    প্রত্যাগম serialize_layout(&state->components, type);
}

xkb_led_mask_t
xkb_state_serialize_leds(ধ্রুব সংরচনা xkb_state *state,
                         গণনা xkb_state_component type)
{
    প্রত্যাগম (type & XKB_STATE_LEDS)
        ? state->components.leds
        : 0;
}

স্থির অন্তঃস্থ গণনা xkb_keyboard_control_flags
serialize_controls(ধ্রুব সংরচনা state_components *components,
                   গণনা xkb_state_component type)
{
    প্রত্যাগম (type & XKB_STATE_CONTROLS_EFFECTIVE)
        /*
         * Enable using the public API with the all the Controls values, except
         * the internal ones, if any.
         */
        ? (গণনা xkb_keyboard_control_flags)
          (components->controls & CONTROL_ALL_BOOLEAN)
        : 0;
}

গণনা xkb_keyboard_control_flags
xkb_state_serialize_controls(ধ্রুব সংরচনা xkb_state *state,
                             গণনা xkb_state_component type)
{
    প্রত্যাগম serialize_controls(&state->components, type);
}

/**
 * Gets a modifier mask and returns the resolved effective mask; this
 * is needed because some modifiers can also map to other modifiers, e.g.
 * the "NumLock" modifier usually also sets the "Mod2" modifier.
 */
xkb_mod_mask_t
mod_mask_get_effective(সংরচনা xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    /* Initialize the effective mask with its corresponding real mods. */
    xkb_mod_mask_t mask = mods & MOD_REAL_MASK_ALL;

    /* Resolve the virtual modifiers */
    ধ্রুব সংরচনা xkb_mod *mod;
    xkb_mod_index_t i;
    xkb_vmods_enumerate(i, mod, &keymap->mods)
        যদি (mods & (UINT32_C(1) << i))
            mask |= mod->mapping;

    প্রত্যাগম mask;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
পূর্ণাঙ্ক
xkb_state_mod_index_is_active(সংরচনা xkb_state *state,
                              xkb_mod_index_t idx,
                              গণনা xkb_state_component type)
{
    যদি (unlikely(idx >= xkb_keymap_অঙ্ক_mods(state->keymap)))
        প্রত্যাগম -1;

    ধ্রুব xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    যদি (!mapping) {
        /* Modifier not mapped */
        প্রত্যাগম 0;
    }
    /* WARNING: this may overmatch for virtual modifiers */
    প্রত্যাগম (xkb_state_serialize_mods(state, type) & mapping) == mapping;
}

গণনা {
    XKB_STATE_MATCH_FLAGS = XKB_STATE_MATCH_ANY
                          | XKB_STATE_MATCH_ALL
                          | XKB_STATE_MATCH_NON_EXCLUSIVE
};

/**
 * Helper function for xkb_state_mod_indices_are_active and
 * xkb_state_mod_names_are_active.
 */
স্থির bool
match_mod_masks(সংরচনা xkb_state *state,
                গণনা xkb_state_component type,
                গণনা xkb_state_match match,
                xkb_mod_mask_t wanted)
{
    ধ্রুব xkb_mod_mask_t active = xkb_state_serialize_mods(state, type);

    যদি (!(match & XKB_STATE_MATCH_NON_EXCLUSIVE) && (active & ~wanted))
        প্রত্যাগম false;

    যদি (match & XKB_STATE_MATCH_ANY)
        প্রত্যাগম active & wanted;

    /* Ensure all flags but XKB_STATE_MATCH_ALL are processed before */
    static_assert(XKB_STATE_MATCH_ALL ==
                  ((গণনা xkb_state_match) XKB_STATE_MATCH_FLAGS &
                   ~(XKB_STATE_MATCH_ANY | XKB_STATE_MATCH_NON_EXCLUSIVE)),
                  "");

    প্রত্যাগম (active & wanted) == wanted;
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, or -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
পূর্ণাঙ্ক
xkb_state_mod_indices_are_active(সংরচনা xkb_state *state,
                                 গণনা xkb_state_component type,
                                 গণনা xkb_state_match match,
                                 ...)
{
    যদি (match & ~((গণনা xkb_state_match) XKB_STATE_MATCH_FLAGS))
        প্রত্যাগম -2;

    va_list ap;
    xkb_mod_mask_t wanted = 0;
    পূর্ণাঙ্ক ret = 0;
    ধ্রুব xkb_mod_index_t অঙ্ক_mods = xkb_keymap_অঙ্ক_mods(state->keymap);

    va_start(ap, match);
    যাবত্ (1) {
        xkb_mod_index_t idx = va_arg(ap, xkb_mod_index_t);
        যদি (idx == XKB_MOD_INVALID)
            ভঙ্গ;
        যদি (unlikely(idx >= অঙ্ক_mods)) {
            ret = -1;
            ভঙ্গ;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_end(ap);

    যদি (ret == -1)
        প্রত্যাগম ret;

    যদি (!wanted) {
        /* Modifiers not mapped */
        প্রত্যাগম 0;
    }

    প্রত্যাগম match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
পূর্ণাঙ্ক
xkb_state_mod_name_is_active(সংরচনা xkb_state *state, ধ্রুব বর্ণ *name,
                             গণনা xkb_state_component type)
{
    ধ্রুব xkb_mod_index_t idx = xkb_keymap_mod_get_index(state->keymap, name);

    যদি (idx == XKB_MOD_INVALID)
        প্রত্যাগম -1;

    প্রত্যাগম xkb_state_mod_index_is_active(state, idx, type);
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ATTR_NULL_SENTINEL পূর্ণাঙ্ক
xkb_state_mod_names_are_active(সংরচনা xkb_state *state,
                               গণনা xkb_state_component type,
                               গণনা xkb_state_match match,
                               ...)
{
    যদি (match & ~((গণনা xkb_state_match) XKB_STATE_MATCH_FLAGS))
        প্রত্যাগম -2;

    va_list ap;
    xkb_mod_mask_t wanted = 0;
    পূর্ণাঙ্ক ret = 0;

    va_start(ap, match);
    যাবত্ (1) {
        ধ্রুব বর্ণ *str = va_arg(ap, ধ্রুব বর্ণ *);
        যদি (str == রিক্ত)
            ভঙ্গ;
        ধ্রুব xkb_mod_index_t idx = xkb_keymap_mod_get_index(state->keymap, str);
        যদি (idx == XKB_MOD_INVALID) {
            ret = -1;
            ভঙ্গ;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_end(ap);

    যদি (ret == -1)
        প্রত্যাগম ret;

    যদি (!wanted) {
        /* Modifiers not mapped */
        প্রত্যাগম 0;
    }

    প্রত্যাগম match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given group is active with the specified type(s), 0 if
 * not, or -1 if the group is invalid.
 */
পূর্ণাঙ্ক
xkb_state_layout_index_is_active(সংরচনা xkb_state *state,
                                 xkb_layout_index_t idx,
                                 গণনা xkb_state_component type)
{
    যদি (idx >= state->keymap->অঙ্ক_groups)
        প্রত্যাগম -1;

    পূর্ণাঙ্ক ret = 0;
    যদি (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ret |= (state->components.group == idx);
    যদি (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret |= (state->components.base_group == (int32_t) idx);
    যদি (type & XKB_STATE_LAYOUT_LATCHED)
        ret |= (state->components.latched_group == (int32_t) idx);
    যদি (type & XKB_STATE_LAYOUT_LOCKED)
        ret |= (state->components.locked_group == (int32_t) idx);

    প্রত্যাগম ret;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
পূর্ণাঙ্ক
xkb_state_layout_name_is_active(সংরচনা xkb_state *state, ধ্রুব বর্ণ *name,
                                গণনা xkb_state_component type)
{
    ধ্রুব xkb_layout_index_t idx =
        xkb_keymap_layout_get_index(state->keymap, name);

    যদি (idx == XKB_LAYOUT_INVALID)
        প্রত্যাগম -1;

    প্রত্যাগম xkb_state_layout_index_is_active(state, idx, type);
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
পূর্ণাঙ্ক
xkb_state_led_index_is_active(সংরচনা xkb_state *state, xkb_led_index_t idx)
{
    যদি (idx >= state->keymap->অঙ্ক_leds ||
        state->keymap->leds[idx].name == XKB_ATOM_NONE)
        প্রত্যাগম -1;

    প্রত্যাগম !!(state->components.leds & (UINT32_C(1) << idx));
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
পূর্ণাঙ্ক
xkb_state_led_name_is_active(সংরচনা xkb_state *state, ধ্রুব বর্ণ *name)
{
    ধ্রুব xkb_led_index_t idx = xkb_keymap_led_get_index(state->keymap, name);

    যদি (idx == XKB_LED_INVALID)
        প্রত্যাগম -1;

    প্রত্যাগম xkb_state_led_index_is_active(state, idx);
}

/**
 * See:
 * - XkbTranslateKeyCode(3), mod_rtrn return value, from libX11.
 * - MyEnhancedXkbTranslateKeyCode(), a modification of the above, from GTK+.
 */
স্থির xkb_mod_mask_t
চাবি_get_consumed(সংরচনা xkb_state *state, ধ্রুব সংরচনা xkb_চাবি *চাবি,
                 গণনা xkb_consumed_mode mode)
{
    ধ্রুব xkb_layout_index_t group =
        xkb_state_চাবি_get_layout(state, চাবি->keycode);
    যদি (group == XKB_LAYOUT_INVALID)
        প্রত্যাগম 0;

    xkb_mod_mask_t preserve = 0;
    xkb_mod_mask_t consumed = 0;

    ধ্রুব সংরচনা xkb_চাবি_type_entry* ধ্রুব matching_entry =
        get_entry_for_চাবি_state(state, চাবি, group);
    যদি (matching_entry)
        preserve = matching_entry->preserve.mask;

    ধ্রুব সংরচনা xkb_চাবি_type* ধ্রুব type = চাবি->groups[group].type;
    বিকল্প (mode) {
    পক্ষ XKB_CONSUMED_MODE_XKB:
        consumed = type->mods.mask;
        ভঙ্গ;

    পক্ষ XKB_CONSUMED_MODE_GTK: {
        ধ্রুব সংরচনা xkb_চাবি_type_entry* ধ্রুব no_mods_entry =
            get_entry_for_mods(type, 0);
        ধ্রুব xkb_level_index_t no_mods_leveli = no_mods_entry
                                               ? no_mods_entry->level
                                               : 0;
        ধ্রুব সংরচনা xkb_level* ধ্রুব no_mods_level =
            &চাবি->groups[group].levels[no_mods_leveli];

        প্রতি (darray_size_t i = 0; i < type->অঙ্ক_entries; i++) {
            ধ্রুব সংরচনা xkb_চাবি_type_entry* ধ্রুব entry = &type->entries[i];
            যদি (!entry_is_active(entry))
                অনুবর্ত;

            ধ্রুব সংরচনা xkb_level* ধ্রুব level =
                &চাবি->groups[group].levels[entry->level];
            যদি (XkbLevelsSameSyms(level, no_mods_level))
                অনুবর্ত;

            যদি (entry == matching_entry || one_bit_set(entry->mods.mask))
                consumed |= entry->mods.mask & ~entry->preserve.mask;
        }
        ভঙ্গ;
    }
    }

    প্রত্যাগম consumed & ~preserve;
}

পূর্ণাঙ্ক
xkb_state_mod_index_is_consumed2(সংরচনা xkb_state *state, xkb_keycode_t kc,
                                 xkb_mod_index_t idx,
                                 গণনা xkb_consumed_mode mode)
{
    ধ্রুব সংরচনা xkb_চাবি* ধ্রুব চাবি = Xkb_চাবি(state->keymap, kc);

    যদি (unlikely(!চাবি || idx >= xkb_keymap_অঙ্ক_mods(state->keymap)))
        প্রত্যাগম -1;

    ধ্রুব xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    যদি (!mapping) {
        /* Modifier not mapped */
        প্রত্যাগম 0;
    }
    প্রত্যাগম (mapping & চাবি_get_consumed(state, চাবি, mode)) == mapping;
}

পূর্ণাঙ্ক
xkb_state_mod_index_is_consumed(সংরচনা xkb_state *state, xkb_keycode_t kc,
                                xkb_mod_index_t idx)
{
    প্রত্যাগম xkb_state_mod_index_is_consumed2(state, kc, idx,
                                            XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_mod_mask_remove_consumed(সংরচনা xkb_state *state, xkb_keycode_t kc,
                                   xkb_mod_mask_t mask)
{
    ধ্রুব সংরচনা xkb_চাবি* ধ্রুব চাবি = Xkb_চাবি(state->keymap, kc);

    যদি (!চাবি)
        প্রত্যাগম 0;

    প্রত্যাগম resolve_to_canonical_mods(state->keymap, mask) &
           ~চাবি_get_consumed(state, চাবি, XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_চাবি_get_consumed_mods2(সংরচনা xkb_state *state, xkb_keycode_t kc,
                                 গণনা xkb_consumed_mode mode)
{
    বিকল্প (mode) {
    পক্ষ XKB_CONSUMED_MODE_XKB:
    পক্ষ XKB_CONSUMED_MODE_GTK:
        ভঙ্গ;
    স্বভাব:
        log_err_func(state->keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "unrecognized consumed modifiers mode: %d\n", mode);
        প্রত্যাগম 0;
    }

    ধ্রুব সংরচনা xkb_চাবি* ধ্রুব চাবি = Xkb_চাবি(state->keymap, kc);
    যদি (!চাবি)
        প্রত্যাগম 0;

    প্রত্যাগম চাবি_get_consumed(state, চাবি, mode);
}

xkb_mod_mask_t
xkb_state_চাবি_get_consumed_mods(সংরচনা xkb_state *state, xkb_keycode_t kc)
{
    প্রত্যাগম xkb_state_চাবি_get_consumed_mods2(state, kc, XKB_CONSUMED_MODE_XKB);
}

/*******************************************************************************
 * Server state machine API
 ******************************************************************************/

/** Entry to keep track of the state of an overlaid key */
সংরচনা xkb_overlaid_চাবি {
    /** The source key being overlaid */
    ধ্রুব সংরচনা xkb_চাবি *old;
    /** The target key */
    ধ্রুব সংরচনা xkb_চাবি *new;
    /** Counter of key currently down */
    পূর্ণাঙ্ক refcnt;
};

সংরচনা xkb_shortcuts_config_entry {
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
সংরচনা xkb_machine {
    /** Inherits from `xkb_server_state` */
    সংরচনা xkb_server_state base;

    /* Extra server-specific state data */

    /** Keyboard overlays handling */
    সংরচনা {
        /**
         * Current overlaid keys
         *
         * When a key is overlaid, it remained overlaid in this configuration
         * until totally released, i.e. every key press was matched with the
         * corresponding key release.
         */
        darray(সংরচনা xkb_overlaid_চাবি) চাবিগুলি;
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
    সংরচনা machine_config {
        /** Modifiers remapping */
        সংরচনা machine_modifiers_config {
            /** Real modifier mask */
            xkb_mod_mask_t mask;
            /** Modifiers re-mappings */
            darray_size_t mappings_অঙ্ক;
            সংরচনা machine_mods_mapping {
                xkb_mod_mask_t source;
                xkb_mod_mask_t target;
            } * mappings ATTR_COUNTED_BY(mappings_অঙ্ক);
        } modifiers;

        /** Shortcuts tweak */
        সংরচনা machine_shortcuts_config {
            /** Substitution entries */
            সংরচনা xkb_shortcuts_config_entry *entries;
        } shortcuts;
    } config;

    /** Mouse keys handling */
    সংরচনা {
        /** Default pointer button */
        xkb_pointer_button_index_t default_button;
        /** Mask of the locked buttons */
        xkb_pointer_button_mask_t locked_buttons;
    } mouse;

    গণনা xkb_machine_flags flags;
};

প্রকারনির্দেশ darray(সংরচনা machine_mods_mapping) machine_mods_mappings;

সংরচনা xkb_machine_builder {
    সংরচনা xkb_keymap *keymap;

    /** Controls */
    সংরচনা {
        /** Accessibility flags */
        সংরচনা {
            গণনা xkb_a11y_flags affect_flags;
            গণনা xkb_a11y_flags flags;
        } a11y;
    } controls;

    /** Modifiers re-mapping */
    machine_mods_mappings mods;

    /** Shortcuts tweak */
    সংরচনা xkb_shortcuts_config_options {
        /** Substitution entries */
        darray(সংরচনা xkb_shortcuts_config_entry) entries;
    } shortcuts;

    গণনা xkb_machine_flags machine_flags;
    গণনা xkb_machine_builder_flags builder_flags;
    পূর্ণাঙ্ক refcnt;
};

সংরচনা xkb_machine_builder *
xkb_machine_builder_new(
    সংরচনা xkb_keymap * restrict keymap,
    ধ্রুব সংরচনা xkb_machine_builder_config * restrict config,
    গণনা xkb_status * restrict status
)
{
    /* Handle default configuration */
    স্থির ধ্রুব সংরচনা xkb_machine_builder_config default_config = {
        .size = পরিমাণ(default_config)
    };
    যদি (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ধ্রুব গণনা xkb_status status_ = xkb_check_state_abi(config);
    যদি (status_) {
        xkb_log_abi_error(keymap->ctx, __func__, status_);
        যদি (status)
            *status = status_;
        প্রত্যাগম রিক্ত;
    }

    /* Sanitize input */
    ধ্রুব গণনা xkb_machine_builder_flags invalid_builder_flags
        = config->builder_flags
        & ~(গণনা xkb_machine_builder_flags)XKB_MACHINE_BUILDER_FLAGS_মানসমূহ;
    যদি (invalid_builder_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS_,
                     "unrecognized machine builder flags: 0x%x\n",
                     invalid_builder_flags);
        যদি (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS;
        প্রত্যাগম রিক্ত;
    }

    ধ্রুব গণনা xkb_machine_flags invalid_machine_flags
        = (গণনা xkb_machine_flags)config->machine_flags
        & ~(গণনা xkb_machine_flags)XKB_MACHINE_FLAGS_মানসমূহ;
    যদি (invalid_machine_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS_,
                     "unrecognized state machine flags: 0x%x\n",
                     invalid_machine_flags);
        যদি (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS;
        প্রত্যাগম রিক্ত;
    }

    সংরচনা xkb_machine_builder * ধ্রুব builder = calloc(1, পরিমাণ(*builder));
    যদি (!builder) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine builder\n");
        যদি (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        প্রত্যাগম রিক্ত;
    }

    *builder = (সংরচনা xkb_machine_builder) {
        .keymap = xkb_keymap_ref(keymap),
        .refcnt = 1,
        .builder_flags = (গণনা xkb_machine_builder_flags)config->builder_flags,
        .machine_flags = (গণনা xkb_machine_flags)config->machine_flags,
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

    যদি (status)
        *status = XKB_SUCCESS;

    প্রত্যাগম builder;
}

সংরচনা xkb_machine_builder *
xkb_machine_builder_ref(সংরচনা xkb_machine_builder *builder)
{
    প্রতিজ্ঞা(builder->refcnt > 0);
    builder->refcnt++;
    প্রত্যাগম builder;
}

শূন্য
xkb_machine_builder_unref(সংরচনা xkb_machine_builder *builder)
{
    প্রতিজ্ঞা(!builder || builder->refcnt > 0);
    যদি (!builder || --builder->refcnt > 0)
        প্রত্যাগম;

    darray_free(builder->shortcuts.entries);
    darray_free(builder->mods);
    xkb_keymap_unref(builder->keymap);
    মুক্ত(builder);
}

সংরচনা xkb_keymap *
xkb_machine_builder_get_keymap(ধ্রুব সংরচনা xkb_machine_builder *builder)
{
    /* Reference count is not updated. See API doc. */
    প্রত্যাগম builder->keymap;
}

গণনা xkb_status
xkb_machine_builder_update_a11y(
    সংরচনা xkb_machine_builder * restrict builder,
    ধ্রুব সংরচনা xkb_machine_builder_a11y_update * restrict update)
{
    /* Check ABI compatibility */
    গণনা xkb_status status = xkb_check_state_abi(update);
    যদি (status != XKB_SUCCESS) {
        xkb_log_abi_error(builder->keymap->ctx, __func__, status);
        প্রত্যাগম status;
    }

    ধ্রুব গণনা xkb_a11y_flags invalid_flags =
        ~(গণনা xkb_a11y_flags)XKB_A11_Y_FLAGS_মানসমূহ;

    যদি (update->affect_flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y affected flags: %#x\n",
                __func__, update->affect_flags & invalid_flags);
        প্রত্যাগম XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }
    যদি (update->flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y flags: %#x\n",
                __func__, update->flags & invalid_flags);
        প্রত্যাগম XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }

    builder->controls.a11y.affect_flags |= update->affect_flags;
    builder->controls.a11y.flags &= ~update->affect_flags;
    builder->controls.a11y.flags |= (update->flags & update->affect_flags);

    প্রত্যাগম XKB_SUCCESS;
}

গণনা xkb_status
xkb_machine_builder_update_mods_remap(
    সংরচনা xkb_machine_builder * restrict builder,
    ধ্রুব সংরচনা xkb_machine_builder_mods_remap_update * restrict update
)
{
    যদি (!update->source) {
        যদি (!update->target) {
            /* Reset mappings */
            darray_resize(builder->mods, 0);
            প্রত্যাগম XKB_SUCCESS;
        } অন্যথা {
            প্রত্যাগম XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
        }
    }

    /* Check the modifiers against the keymap */
    ধ্রুব xkb_mod_mask_t invalid = ~builder->keymap->canonical_state_mask;
    যদি ((update->source & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid source modifiers: 0x%"PRIx32"\n",
                __func__, update->source);
        প্রত্যাগম XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    যদি ((update->target & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid target modifiers: 0x%"PRIx32"\n",
                __func__, update->target);
        প্রত্যাগম XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    সংরচনা machine_mods_mapping *mapping = রিক্ত;
    darray_size_t m = 0;
    darray_enumerate(m, mapping, builder->mods) {
        যদি (mapping->source == update->source) {
            যদি (!update->target) {
                /* Remove mapping */
                darray_remove(builder->mods, m);
            } অন্যথা {
                /* Update mapping */
                mapping->target = update->target;
            }
            প্রত্যাগম XKB_SUCCESS;
        }
    }

    যদি (update->target) {
        /* Append new mapping */
        darray_append(
            builder->mods,
            (সংরচনা machine_mods_mapping) {
                .source = update->source,
                .target = update->target
            }
        );
    } অন্যথা {
        /* Ignore missing mapping */
    }

    প্রত্যাগম XKB_SUCCESS;
}

গণনা xkb_status
xkb_machine_builder_update_shortcut_override(
    সংরচনা xkb_machine_builder * restrict builder,
    ধ্রুব সংরচনা xkb_machine_builder_shortcut_override_update * restrict update
)
{
    সংরচনা xkb_keymap *keymap = builder->keymap;

    /* Check the modifiers against the keymap */
    ধ্রুব xkb_mod_mask_t invalid = ~keymap->canonical_state_mask;
    যদি ((update->affect_mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid affected modifiers: 0x%"PRIx32"\n",
                __func__, update->affect_mods);
        প্রত্যাগম XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    যদি ((update->mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid modifiers: 0x%"PRIx32"\n",
                __func__, update->mods);
        প্রত্যাগম XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    /* Check the layout indices against the keymap */
    যদি (update->source >= keymap->অঙ্ক_groups &&
        update->source != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "%s: Invalid source layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->অঙ্ক_groups, update->source + 1);
        প্রত্যাগম XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }
    যদি (update->target >= keymap->অঙ্ক_groups &&
        update->target != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "%s: Invalid target layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->অঙ্ক_groups, update->target + 1);
        প্রত্যাগম XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    সংরচনা xkb_shortcuts_config_options * ধ্রুব config = &builder->shortcuts;

    /* Resize array & initialize new entries, if relevant */
    যদি (update->source >= darray_size(config->entries) &&
        update->source != XKB_LAYOUT_INVALID) {
        যদি (update->target == update->source) {
            /* Skip default setting */
            প্রত্যাগম XKB_SUCCESS;
        }
        xkb_layout_index_t new = darray_size(config->entries);
        darray_resize(config->entries, update->source + 1);
        প্রতি (; new <= update->source; new++) {
            darray_item(config->entries, new) =
                (সংরচনা xkb_shortcuts_config_entry) {
                    .target = XKB_LAYOUT_INVALID,
                    .mods = 0
                };
        }
    }

    সংরচনা xkb_shortcuts_config_entry *entry;
    xkb_layout_index_t source = (update->source == XKB_LAYOUT_INVALID)
        ? 0               /* Loop over all entries */
        : update->source; /* Single entry */
    darray_foreach_from(entry, config->entries, source) {
        যদি (update->source == XKB_LAYOUT_INVALID &&
            entry->target == XKB_LAYOUT_INVALID) {
            /* Skip previous deactivated entry */
            source++;
            অনুবর্ত;
        }

        যদি (update->affect_mods) {
            entry->mods &= ~update->affect_mods;
            entry->mods |= (update->affect_mods & update->mods);
        }

        যদি (update->source != XKB_LAYOUT_INVALID) {
            entry->target = (update->target == source)
                ? XKB_LAYOUT_INVALID
                : update->target;
            /* Single entry: exit loop */
            ভঙ্গ;
        }
        source++;
    }

    প্রত্যাগম XKB_SUCCESS;
}

/** Compare 2 modifiers combinations */
স্থির পূর্ণাঙ্ক
cmp_mod_masks(ধ্রুব শূন্য *a, ধ্রুব শূন্য *b)
{
    ধ্রুব xkb_mod_mask_t m1 = *((xkb_mod_mask_t *) a);
    ধ্রুব xkb_mod_mask_t m2 = *((xkb_mod_mask_t *) b);
    যদি (m1 == m2)
        প্রত্যাগম 0;

    যদি (!m1)
        প্রত্যাগম +1;
    যদি (!m2)
        প্রত্যাগম -1;

    ধ্রুব xkb_mod_mask_t overlap = (m1 & m2);
    যদি (overlap == m1)
        প্রত্যাগম +1; /* m1 is a subset of m2 */
    যদি (overlap == m2)
        প্রত্যাগম -1; /* m2 is a subset of m1 */

    /* Compare active modifiers numbers */
    ধ্রুব unsigned পূর্ণাঙ্ক count1 = popcount32(m1);
    ধ্রুব unsigned পূর্ণাঙ্ক count2 = popcount32(m2);
    যদি (count1 > count2)
        প্রত্যাগম -1;
    যদি (count1 < count2)
        প্রত্যাগম +1;

    /* Give priority to real modifiers */
    প্রত্যাগম (m1 < m2) ? -1 : +1;
}

স্থির গণনা xkb_status
machine_set_mods(সংরচনা xkb_machine *sm,
                 ধ্রুব machine_mods_mappings *raw_mappings)
{
    যদি (!darray_empty(*raw_mappings)) {
        machine_mods_mappings mappings = darray_new();
        xkb_mod_mask_t mask = 0;
        ধ্রুব সংরচনা machine_mods_mapping *mapping;
        ধ্রুব xkb_mod_mask_t invalid =
            ~sm->base.base.keymap->canonical_state_mask;
        darray_foreach(mapping, *raw_mappings) {
            যদি (!mapping->source || !mapping->target ||
                (mapping->source & invalid) || (mapping->target & invalid))
                অনুবর্ত;
            darray_append(mappings, *mapping);
            mask |= mapping->source;
        }

        /*
         * Re-order the mappings, so that combinations have higher priority
         * if they are superset of other combinations.
         * For other type of overlaps, see the sorting function.
         */
        qsort(darray_বস্তুসমূহ(mappings), darray_size(mappings),
              পরিমাণ(*darray_বস্তুসমূহ(mappings)), &cmp_mod_masks);

        darray_steal(
            mappings,
            &sm->config.modifiers.mappings,
            &sm->config.modifiers.mappings_অঙ্ক
        );
        sm->config.modifiers.mask = mask;
    } অন্যথা {
        sm->config.modifiers = (সংরচনা machine_modifiers_config) {
            .mappings = রিক্ত,
            .mappings_অঙ্ক = 0,
            .mask = 0
        };
    }

    প্রত্যাগম XKB_SUCCESS;
}

স্থির গণনা xkb_status
machine_set_shortcuts(
    সংরচনা xkb_machine * restrict sm,
    ধ্রুব সংরচনা xkb_shortcuts_config_options * restrict options
)
{
    যদি (darray_empty(options->entries)) {
        sm->config.shortcuts = (সংরচনা machine_shortcuts_config) {
            .entries = রিক্ত
        };
        প্রত্যাগম XKB_SUCCESS;
    }

    সংরচনা xkb_keymap * ধ্রুব keymap = sm->base.base.keymap;

    সংরচনা xkb_shortcuts_config_entry * entries =
        calloc(keymap->অঙ্ক_groups, পরিমাণ(*entries));
    যদি (!entries)
        প্রত্যাগম XKB_ERROR_ALLOCATION_FAILURE;

    ধ্রুব xkb_layout_index_t গুনতি = MIN(
        keymap->অঙ্ক_groups,
        (xkb_layout_index_t) darray_size(options->entries)
    );

    bool some_entries = false;
    প্রতি (xkb_layout_index_t l = 0; l < গুনতি; l++) {
        /* Sanitize layout target */
        ধ্রুব xkb_layout_index_t target =
            darray_item(options->entries, l).target;
        /* Sanitize modifier mask */
        ধ্রুব xkb_mod_mask_t mods = (
            darray_item(options->entries, l).mods &
            sm->base.base.keymap->canonical_state_mask
        );
        যদি (target < keymap->অঙ্ক_groups && target != l && mods) {
            entries[l] = (সংরচনা xkb_shortcuts_config_entry) {
                .target = target,
                .mods = mods
            };
            some_entries = true;
        } অন্যথা {
            entries[l] = (সংরচনা xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
            অনুবর্ত;
        }
    }

    যদি (!some_entries) {
        মুক্ত(entries);
        entries = রিক্ত;
    } অন্যথা {
        /* Initialize groups with no entries */
        প্রতি (xkb_layout_index_t l = গুনতি; l < keymap->অঙ্ক_groups; l++) {
            entries[l] = (সংরচনা xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
        }
    }

    sm->config.shortcuts = (সংরচনা machine_shortcuts_config) {
        .entries = entries
    };
    প্রত্যাগম XKB_SUCCESS;
}

সংরচনা xkb_machine *
xkb_machine_new(ধ্রুব সংরচনা xkb_machine_builder * restrict builder,
                গণনা xkb_status * restrict status)
{
    সংরচনা xkb_machine * ধ্রুব machine = calloc(1, পরিমাণ(*machine));
    যদি (!machine) {
        log_err_func1(builder->keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine\n");
        যদি (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        প্রত্যাগম রিক্ত;
    }

    xkb_server_state_init(&machine->base, builder->keymap, SERVER_STATE,
                          builder->controls.a11y.affect_flags,
                          builder->controls.a11y.flags);
    machine->flags = builder->machine_flags;
    যদি (!(machine->flags &
          XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_চাবি_FIX))
    {
        machine->base.flags |= STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_চাবি;
    }
    যদি (machine->flags & XKB_MACHINE_UNLOCK_MODS_ON_PRESS) {
        machine->base.flags |= STATE_UNLOCK_MODS_ON_PRESS;
    }

    গণনা xkb_status status_;
    যদি ((status_ = machine_set_mods(machine, &builder->mods)) != XKB_SUCCESS ||
        (status_ = machine_set_shortcuts(machine, &builder->shortcuts)) !=
        XKB_SUCCESS) {
        *status = status_;
        গচ্ছ error;
    }

    darray_init(machine->overlays.চাবিগুলি);
    machine->mouse.default_button = XKB_POINTER_BUTTON_MIN;

    যদি (status)
        *status = XKB_SUCCESS;

    প্রত্যাগম machine;

error:
    xkb_machine_unref(machine);
    প্রত্যাগম রিক্ত;
}

সংরচনা xkb_machine *
xkb_machine_ref(সংরচনা xkb_machine *sm)
{
    প্রতিজ্ঞা(sm->base.base.refcnt > 0);
    sm->base.base.refcnt++;
    প্রত্যাগম sm;
}

শূন্য
xkb_machine_unref(সংরচনা xkb_machine *sm)
{
    প্রতিজ্ঞা(!sm || sm->base.base.refcnt > 0);
    যদি (!sm || --sm->base.base.refcnt > 0)
        প্রত্যাগম;

    xkb_state_destroy(&sm->base.base);
    darray_free(sm->overlays.চাবিগুলি);
    মুক্ত(sm->config.shortcuts.entries);
    মুক্ত(sm->config.modifiers.mappings);
    মুক্ত(sm);
}

সংরচনা xkb_keymap *
xkb_machine_get_keymap(ধ্রুব সংরচনা xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    প্রত্যাগম sm->base.base.keymap;
}

সংরচনা xkb_state *
xkb_machine_get_state(সংরচনা xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    প্রত্যাগম (সংরচনা xkb_state *)sm;
}

সংরচনা xkb_state *
xkb_state_new_from_machine(ধ্রুব সংরচনা xkb_machine * restrict machine,
                           গণনা xkb_status * restrict status)
{
    /* Creation */
    গণনা xkb_status status_;
    সংরচনা xkb_state * ধ্রুব state = xkb_state_new_with_mode(
        machine->base.base.keymap,
        XKB_STATE_MODE_SERVER_QUERY,
        &status_
    );
    যদি (status_ != XKB_SUCCESS) {
        যদি (status)
            *status = status_;
        প্রত্যাগম রিক্ত;
    }

    প্রতিজ্ঞা(state->mode == SERVER_COMPANION);
    প্রতিজ্ঞা(state->refcnt == 1);

    /* Copy machine’s state */
    *state = machine->base.base;

    /* Restore previous values */
    state->mode = SERVER_COMPANION;
    state->refcnt = 1;

    যদি (status)
        *status = XKB_SUCCESS;
    প্রত্যাগম state;
}

স্থির শূন্য
machine_update_overlays(সংরচনা xkb_machine *sm)
{
    /*
     * Overlays indices are stored 1-indexed in nibbles: the lowest
     * nibble corresponds to the latest activated index.
     */

    ধ্রুব xkb_overlay_mask_t mask =
        overlays_from_controls(sm->base.base.components.controls);
    xkb_overlay_mask_t added = mask & ~sm->overlays.enabled;

    /* Remove overlays no longer enabled and keep relative order */
    uint32_t order = sm->overlays.order;
    ধ্রুব xkb_overlay_index_t overlay_max =
        format_max_overlays(sm->base.base.keymap->format);
    প্রতি (uint8_t n = 0; n < overlay_max;) {
        xkb_overlay_index_t overlay_idx = (order >> (n * 4)) & 0xf;
        যদি (!overlay_idx)
            ভঙ্গ;
        ধ্রুব xkb_overlay_mask_t overlay_mask =
            (xkb_overlay_mask_t)(1u << (overlay_idx - 1u) /* k is 1-indexed */);
        যদি (overlay_mask & mask) {
            /* no duplicates */
            added &= ~overlay_mask;
            n++;
        } অন্যথা {
            /* remove overlay */
            ধ্রুব uint32_t head = (UINT32_C(1) << (n * 4)) - 1;
            order = (order & head) | ((order >> 4) & ~head);
        }
    }

    /* Add new overlays in ascending order */
    প্রতি (xkb_overlay_index_t k = 0; added; k++, added >>= 1) {
        যদি (added & 0x1) {
            order <<= 4;
            order |= k + 1u; /* k is 0-indexed */
        }
    }

    sm->overlays.order = order;
    sm->overlays.enabled = mask;
}

গণনা xkb_status
xkb_machine_process_synthetic(সংরচনা xkb_machine *sm,
                              ধ্রুব সংরচনা xkb_synthetic_update *update,
                              সংরচনা xkb_events *events)
{
    /* Check ABI compatibility */
    গণনা xkb_status status =
        check_state_update_abi(sm->base.base.keymap->ctx, update);
    যদি (status != XKB_SUCCESS)
        প্রত্যাগম status;

    সংরচনা xkb_server_state * ধ্রুব state = &sm->base;
    ধ্রুব সংরচনা state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    darray_size(events->queue) = 0;
    events->next = 0;

    /* Update parametrized controls first */
    যদি (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        যদি (status != XKB_SUCCESS)
            প্রত্যাগম status;
    }

    যদি (update->components) {
        ধ্রুব সংরচনা xkb_state_components_update * ধ্রুব components =
            update->components;
        /* Update boolean controls first */
        যদি (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, events);
        }

        state_update_latched_locked(state, components, events);
    }

    xkb_state_update_derived(&state->base);

    ধ্রুব গণনা xkb_state_component changed = get_state_component_changes(
        &previous_components, &state->base.components
    );
    যদি (changed) {
        // TODO: latch controls
        যদি (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        /* Create event only if some component actually changed */
        darray_append(events->queue, (সংরচনা xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .changed = changed,
                .components = state->base.components
            }
        });

        /* Terminate the frame */
        darray_append(events->queue, (সংরচনা xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    প্রত্যাগম XKB_SUCCESS;
}

স্থির ssize_t
do_remap_modifiers(ধ্রুব সংরচনা machine_modifiers_config * restrict mappings,
                   সংরচনা xkb_state * restrict state,
                   সংরচনা xkb_events * restrict events,
                   ধ্রুব সংরচনা xkb_চাবি * restrict চাবি)
{
    যদি (!(mappings->mask & state->components.mods))
        প্রত্যাগম -1;

    ধ্রুব xkb_layout_index_t layout = state_চাবি_get_layout(state, চাবি);
    যদি (layout >= চাবি->অঙ্ক_groups)
        প্রত্যাগম -1;

    ধ্রুব সংরচনা xkb_চাবি_type * ধ্রুব type = চাবি->groups[layout].type;
    xkb_mod_mask_t affect_mods = 0;
    xkb_mod_mask_t mods = 0;
    প্রতি (darray_size_t m = 0; m < mappings->mappings_অঙ্ক; m++) {
        ধ্রুব সংরচনা machine_mods_mapping * ধ্রুব mapping =
            &mappings->mappings[m];
        যদি (/* Skip not matching active mods */
            (mapping->source & state->components.mods) == mapping->source &&
            /* Skip overlap with previous active mappings */
            !(mapping->source & affect_mods) &&
            /* Skip if the key type uses some of the source modifiers */
            !(mapping->source & type->mods.mask)) {
            affect_mods |= mappings->mappings[m].source;
            mods |= mappings->mappings[m].target;
        }
    }

    যদি (!affect_mods)
        প্রত্যাগম -1;

    সংরচনা xkb_state new = *state;
    new.components.base_mods = (new.components.base_mods & ~affect_mods) | mods;
    new.components.latched_mods = (new.components.latched_mods & ~affect_mods);
    new.components.locked_mods = (new.components.locked_mods & ~affect_mods);
    xkb_state_update_derived(&new);

    ssize_t event_idx = -1;
    ধ্রুব গণনা xkb_state_component changed =
        get_state_component_changes(&state->components, &new.components);
    যদি (changed) {
        event_idx = (ssize_t) darray_size(events->queue);
        darray_append(events->queue, (সংরচনা xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = new.components,
                .changed = changed
            }
        });
    }

    state->components.mods = new.components.mods;
    প্রত্যাগম event_idx;
}

/**
 * Activate an alternative layout if some modifiers are active, typically
 * `Control`, `Alt` or `Super`.
 *
 * The tweak consists of computing a *base* layout such that the *effective*
 * layout is the corresponding target layout.
 */
স্থির ssize_t
do_shortcuts_tweak(ধ্রুব সংরচনা machine_shortcuts_config *config,
                   সংরচনা xkb_state *state,
                   ধ্রুব সংরচনা state_components *previous_components,
                   সংরচনা xkb_events *events, ssize_t remap_event)
{
    যদি (!config->entries) {
        /* No shortcuts tweak */
        প্রত্যাগম remap_event;
    }

    ধ্রুব সংরচনা xkb_shortcuts_config_entry * ধ্রুব entry =
        &config->entries[state->components.group];

    যদি (entry->target != XKB_LAYOUT_INVALID &&
        (entry->mods & state->components.mods)) {
        /*
         * Activate shortcuts tweak:
         * 1. The real base group is saved by the caller, to be restored by
         *    later by a call to undo_tweaks().
         * 2. Compute the new base group so that the resulting effective group
         *    is the target.
         * 3. Recompute derived state components.
         */
        সংরচনা xkb_state new = *state;
        যদি (remap_event < 0) {
            /* Create new event */
            remap_event = (ssize_t) darray_size(events->queue);
            darray_append(events->queue, (সংরচনা xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = {0},
                    .changed = 0
                }
            });
        } অন্যথা {
            /* Merge with last remap event */
            new.components =
                darray_item(events->queue, remap_event).components.components;
        }
        new.components.base_group
            = (int32_t) entry->target
            - state->components.latched_group
            - state->components.locked_group;
        xkb_state_update_derived(&new);

        ধ্রুব গণনা xkb_state_component changed =
            get_state_component_changes(previous_components, &new.components);
        darray_item(events->queue, remap_event).components.changed = changed;
        darray_item(events->queue, remap_event).components.components =
            new.components;

        state->components.group = new.components.group;
    } অন্যথা {
        /* Deactivated entry or modifiers do not match */
    }
    প্রত্যাগম remap_event;
}

স্থির শূন্য
undo_tweaks(ধ্রুব সংরচনা xkb_state *state,
            ধ্রুব সংরচনা state_components *initial_components,
            সংরচনা xkb_events *events)
{
    /* Get last component event */
    ধ্রুব সংরচনা xkb_event *event = রিক্ত;
    darray_foreach_reverse(event, events->queue) {
        যদি (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS)
            ভঙ্গ;
    }
    যদি (!event)
        প্রত্যাগম;

    /* Restore state */
    ধ্রুব গণনা xkb_state_component changed =
        get_state_component_changes(initial_components,
                                    &event->components.components);
    যদি (changed) {
        darray_append(events->queue, (সংরচনা xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = *initial_components,
                .changed = changed
            }
        });
    }
}

স্থির ধ্রুব সংরচনা xkb_চাবি *
process_overlayable_চাবি(সংরচনা xkb_machine *sm,
                        ধ্রুব সংরচনা xkb_চাবি * চাবি,
                        গণনা xkb_চাবি_direction direction)
{
    /* Check if key is currently overlaid */
    সংরচনা xkb_overlaid_চাবি *entry = রিক্ত;
    সংরচনা xkb_overlaid_চাবি *available_entry = রিক্ত;
    darray_foreach(entry, sm->overlays.চাবিগুলি) {
        যদি (entry->old == চাবি) {
            /* Maintain the key overlaid */
            বিকল্প (direction) {
            পক্ষ XKB_চাবি_DOWN:
                entry->refcnt++;
                ভঙ্গ;
            পক্ষ XKB_চাবি_REPEATED:
                ভঙ্গ;
            পক্ষ XKB_চাবি_UP:
            স্বভাব:
                entry->refcnt--;
                ভঙ্গ;
            }
            যদি (entry->refcnt <= 0)
                entry->old = রিক্ত;
            প্রত্যাগম entry->new;
        } অন্যথা যদি (!entry->old && !available_entry) {
            /* First free spot */
            available_entry = entry;
        }
    }
    যদি (direction == XKB_চাবি_DOWN) {
        /* Enable key overlay */

        /*
         * Get overlay index: select the latest activated overlay
         * containing the key.
         */
        ধ্রুব সংরচনা xkb_চাবি *new = চাবি;

        যদি (চাবি->overlays & sm->overlays.enabled) {
            /* Some relevant overlay is active */
            প্রতি (uint32_t স্তূপ = sm->overlays.order; স্তূপ; স্তূপ >>= 4) {
                static_assert(XKB_OVERLAY_গুনতি == 8, "");
                ধ্রুব xkb_overlay_index_t overlay = (স্তূপ & 0xf) - 1;
                ধ্রুব xkb_overlay_mask_t mask =
                    (xkb_overlay_mask_t)(1u << overlay);
                যদি (চাবি->overlays & mask) {
                    /* Get new key */
                    যদি (চাবি->overlays_inline) {
                        /* Stored inline */
                        new = চাবি->overlay_চাবি;
                    } অন্যথা {
                        /* Stored in sparse array */
                        ধ্রুব xkb_overlay_mask_t নিচের = (xkb_overlay_mask_t)(
                            চাবি->overlays & (xkb_overlay_mask_t)(mask - 1u)
                        );
                        /* index = number of set bits strictly below bit */
                        ধ্রুব xkb_overlay_index_t index =
                            (xkb_overlay_index_t)popcount32(নিচের);
                        new = চাবি->overlays_চাবিগুলি[index];
                    }
                    ভঙ্গ;
                }
            }
        } অন্যথা {
            /*
             * No relevant overlay is active: block future overlays while
             * the key is pressed.
             */
        }

        /* Get new entry */
        যদি (available_entry) {
            /* Re-use a free entry */
            entry = available_entry;
        } অন্যথা {
            /* Append a new entry */
            ধ্রুব darray_size_t idx = darray_size(sm->overlays.চাবিগুলি);
            darray_resize(sm->overlays.চাবিগুলি, idx + 1);
            entry = &darray_item(sm->overlays.চাবিগুলি, idx);
        }

        /* Overlay the key */
        entry->old = চাবি;
        entry->new = new;
        entry->refcnt = 1;
        প্রত্যাগম new;
    }

    /* No overlay */
    প্রত্যাগম চাবি;
}

গণনা xkb_status
xkb_machine_process_চাবি(সংরচনা xkb_machine *sm,
                        xkb_keycode_t kc, গণনা xkb_চাবি_direction direction,
                        সংরচনা xkb_events *events)
{
    darray_size(events->queue) = 0;
    events->next = 0;

    সংরচনা xkb_server_state * ধ্রুব state = &sm->base;
    ধ্রুব সংরচনা xkb_চাবি * চাবি = Xkb_চাবি(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    যদি (!চাবি || (direction == XKB_চাবি_REPEATED && !চাবি->repeats))
        প্রত্যাগম XKB_SUCCESS;

    ধ্রুব সংরচনা state_components initial_components = state->base.components;

    যদি (direction == XKB_চাবি_DOWN &&
        (sm->base.base.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_চাবিগুলি) &&
        (sm->base.a11y_flags & XKB_A11_Y_STICKY_চাবিগুলি_NO_SIMULTANEOUS_চাবিগুলি) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_চাবিগুলি,
                                      0, events);
    }

    যদি (চাবি->overlays)
        চাবি = process_overlayable_চাবি(sm, চাবি, direction);

    ssize_t remap_event = do_remap_modifiers(&sm->config.modifiers,
                                             &state->base, events, চাবি);

    remap_event = do_shortcuts_tweak(&sm->config.shortcuts, &state->base,
                                     &initial_components, events, remap_event);

    যদি (remap_event >= 0) {
        // TODO: what about merging state event for RedirectKey?
        darray_append(events->queue, (সংরচনা xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    state->flags |= STATE_REQUIRE_চাবি_EVENT;
    state->set_mods = 0;
    state->clear_mods = 0;

    /* Handle key actions */
    xkb_filter_apply_all(state, events, চাবি, direction);

    xkb_mod_index_t i;
    xkb_mod_mask_t bit;
    প্রতি (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        যদি (state->set_mods & bit) {
            state->mod_চাবি_গুনতি[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    প্রতি (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        যদি (state->clear_mods & bit) {
            state->mod_চাবি_গুনতি[i]--;
            যদি (state->mod_চাবি_গুনতি[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_চাবি_গুনতি[i] = 0;
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
    যদি (state->flags & STATE_REQUIRE_চাবি_EVENT) {
        darray_append(events->queue, (সংরচনা xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_চাবি,
            .চাবি = {
                .keycode = চাবি->keycode,
                .direction = direction
            }
        });
    }

    যদি (remap_event >= 0) {
        // FIXME: fragile if last state change does not restore the state to the remap event
        undo_tweaks(&state->base, &initial_components, events);
    }

    ধ্রুব গণনা xkb_state_component changed = get_state_component_changes(
        &initial_components, &state->base.components
    );
    যদি (changed) {
        যদি (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        // TODO: merge with previous state event, if any

        darray_append(events->queue, (সংরচনা xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = state->base.components,
                .changed = changed
            }
        });
    }

    যদি (!darray_empty(events->queue) &&
        darray_item(events->queue, darray_size(events->queue) - 1).type !=
        XKB_EVENT_TYPE_FRAME)
    {
        darray_append(events->queue, (সংরচনা xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    প্রত্যাগম XKB_SUCCESS;
}

সংরচনা xkb_events *
xkb_events_new(সংরচনা xkb_context * restrict context,
               ধ্রুব সংরচনা xkb_events_config * restrict config,
               গণনা xkb_status * restrict status)
{
    /* Handle default configuration */
    স্থির ধ্রুব সংরচনা xkb_events_config default_config = {
        .size = পরিমাণ(default_config)
    };
    যদি (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ধ্রুব গণনা xkb_status abi_error = xkb_check_state_abi(config);
    যদি (abi_error) {
        xkb_log_abi_error(context, __func__, abi_error);
        যদি (status)
            *status = abi_error;
        প্রত্যাগম রিক্ত;
    }

    /* Sanitize input */
    ধ্রুব uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_EVENTS_FLAGS_মানসমূহ);
    যদি (invalid_flags) {
        log_err_func(context, XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS_,
                     "unrecognized events collection flags: 0x%"PRIx32"\n",
                     invalid_flags);
        যদি (status)
            *status = XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS;
        প্রত্যাগম রিক্ত;
    }

    সংরচনা xkb_events *events = calloc(1, পরিমাণ(*events));
    যদি (!events) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events collection\n");
        যদি (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        প্রত্যাগম রিক্ত;
    }

    /* Pre-allocate event queue */
    darray_init(events->queue);
    স্থির ধ্রুব darray_size_t queue_initial_size = 16;
    darray_resize(events->queue, queue_initial_size);
    // TODO: proper safe darray_resize() with return value
    যদি (!darray_বস্তুসমূহ(events->queue)) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events queue\n");
        xkb_events_destroy(events);
        যদি (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        প্রত্যাগম রিক্ত;
    }
    darray_size(events->queue) = 0;

    events->ctx = xkb_context_ref(context);
    events->next = 0;

    যদি (status)
        *status = XKB_SUCCESS;
    প্রত্যাগম events;
}

শূন্য
xkb_events_destroy(সংরচনা xkb_events *events)
{
    যদি (!events)
        প্রত্যাগম;
    darray_free(events->queue);
    xkb_context_unref(events->ctx);
    মুক্ত(events);
}

ধ্রুব সংরচনা xkb_event *
xkb_events_next(সংরচনা xkb_events *events)
{
    যদি (events->next < darray_size(events->queue)) {
        ধ্রুব darray_size_t index = events->next++;
        প্রত্যাগম &darray_item(events->queue, index);
    } অন্যথা {
        প্রত্যাগম রিক্ত;
    }
}

গণনা xkb_event_type
xkb_event_get_type(ধ্রুব সংরচনা xkb_event *event)
{
    প্রত্যাগম event->type;
}

গণনা xkb_status
xkb_event_get_keycode(ধ্রুব সংরচনা xkb_event *event,
                      xkb_keycode_t *keycode,
                      গণনা xkb_চাবি_direction *direction)
{
    যদি (event->type == XKB_EVENT_TYPE_চাবি) {
        *keycode = event->চাবি.keycode;
        *direction = event->চাবি.direction;
        প্রত্যাগম XKB_SUCCESS;
    } অন্যথা {
        প্রত্যাগম XKB_ERROR_EVENT_TYPE_MISMATCH;
    }
}

গণনা xkb_status
xkb_event_get_components(ধ্রুব সংরচনা xkb_event * restrict event,
                         সংরচনা xkb_event_components * restrict components)
{
    যদি (event->type != XKB_EVENT_TYPE_STATE_COMPONENTS)
        প্রত্যাগম XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ধ্রুব গণনা xkb_status status = xkb_check_state_abi(components);
    যদি (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        প্রত্যাগম status;
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

    প্রত্যাগম XKB_SUCCESS;
}

গণনা xkb_status
xkb_event_get_pointer_motion(ধ্রুব সংরচনা xkb_event * restrict event,
                             সংরচনা xkb_event_pointer_motion * restrict motion)
{
    যদি (event->type != XKB_EVENT_TYPE_POINTER_MOTION)
        প্রত্যাগম XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ধ্রুব গণনা xkb_status status = xkb_check_state_abi(motion);
    যদি (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        প্রত্যাগম status;
    }

    motion->flags = event->pointer_motion.flags;
    motion->x = event->pointer_motion.x;
    motion->y = event->pointer_motion.y;

    প্রত্যাগম XKB_SUCCESS;
}

গণনা xkb_status
xkb_event_get_pointer_button(ধ্রুব সংরচনা xkb_event * restrict event,
                             সংরচনা xkb_event_pointer_button * restrict button)
{
    যদি (event->type != XKB_EVENT_TYPE_POINTER_BUTTON)
        প্রত্যাগম XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ধ্রুব গণনা xkb_status status = xkb_check_state_abi(button);
    যদি (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        প্রত্যাগম status;
    }

    button->button = event->pointer_button.button;
    button->state = event->pointer_button.state;
    button->গুনতি = event->pointer_button.গুনতি;

    প্রত্যাগম XKB_SUCCESS;
}

গণনা xkb_status
xkb_event_get_virtual_console(ধ্রুব সংরচনা xkb_event * restrict event,
                              int8_t * restrict index_or_offset,
                              bool * restrict is_offset)
{
    যদি (event->type != XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
        প্রত্যাগম XKB_ERROR_EVENT_TYPE_MISMATCH;

    *index_or_offset = event->virtual_console.index_or_offset;
    *is_offset = event->virtual_console.is_offset;

    প্রত্যাগম XKB_SUCCESS;
}

গণনা xkb_status
xkb_state_update_event(সংরচনা xkb_state * restrict base_state,
                       ধ্রুব সংরচনা xkb_event * restrict event,
                       গণনা xkb_state_component * restrict changed)
{
    /* Guard against server-only state */
    প্রতিজ্ঞা(base_state->mode == SERVER_COMPANION ||
           base_state->mode == LEGACY_MIXED_STATE);
    যদি (base_state->mode != SERVER_COMPANION &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE_,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        প্রত্যাগম XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    সংরচনা xkb_client_state * ধ্রুব state =
        (সংরচনা xkb_client_state *)base_state;

    যদি (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
        ধ্রুব সংরচনা state_components prev_components = state->base.components;
        state->base.components = event->components.components;
        /*
         * Recompute the changes instead of using the event value, because we do
         * not know if the event’s queue and the state are synced.
         */
        যদি (changed)
            *changed = get_state_component_changes(&prev_components,
                                                   &state->base.components);
    } অন্যথা {
        যদি (changed)
            *changed = 0;
    }
    প্রত্যাগম XKB_SUCCESS;
}

স্থির শূন্য
append_pointer_button(সংরচনা xkb_events * restrict events,
                      সংরচনা xkb_filter * restrict filter,
                      গণনা xkb_pointer_button_state button_state,
                      uint8_t গুনতি)
{
    darray_append(events->queue, (সংরচনা xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_BUTTON,
        .pointer_button = {
            .size = পরিমাণ(((সংরচনা xkb_event*)0)->pointer_button),
            .button = filter->priv,
            .state = button_state,
            .গুনতি = গুনতি,
        }
    });
}

স্থির শূন্য
xkb_filter_pointer_button_new(সংরচনা xkb_server_state *state,
                              সংরচনা xkb_events *events,
                              সংরচনা xkb_filter *filter)
{
    যদি (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_চাবিগুলি)) {
        filter->func = রিক্ত;
        প্রত্যাগম;
    }

    সংরচনা xkb_machine * ধ্রুব sm = (সংরচনা xkb_machine *)state;

    /* Resolve button */
    ধ্রুব xkb_pointer_button_index_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;
    filter->priv = button;

    ধ্রুব গণনা xkb_pointer_button_state button_state =
        XKB_POINTER_BUTTON_PRESSED;
    append_pointer_button(events, filter, button_state,
                          filter->action.btn.গুনতি);
    state->flags &= ~STATE_REQUIRE_চাবি_EVENT;
}

স্থির bool
xkb_filter_pointer_button_func(সংরচনা xkb_server_state *state,
                               সংরচনা xkb_events *events,
                               সংরচনা xkb_filter *filter,
                               ধ্রুব সংরচনা xkb_চাবি *চাবি,
                               গণনা xkb_চাবি_direction direction)
{
    যদি (চাবি != filter->চাবি || !events)
        প্রত্যাগম XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_চাবি_EVENT;

    বিকল্প (direction) {
    পক্ষ XKB_চাবি_DOWN:
        filter->refcnt++;
        /* fallthrough */
    পক্ষ XKB_চাবি_REPEATED:
        প্রত্যাগম XKB_FILTER_CONSUME;
    স্বভাব:
        যদি (--filter->refcnt > 0)
            প্রত্যাগম XKB_FILTER_CONSUME;
    }

    যদি (!filter->action.btn.গুনতি)
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);

    filter->func = রিক্ত;
    প্রত্যাগম XKB_FILTER_CONSUME;
}

সংরচনা xkb_filter_pointer_lock_button_priv {
    uint32_t button:31;
#ifdef _WIN32
    uint32_t locked:1; // MSVC requires matching types to pack bitfields
#অন্যথা
    bool locked:1;
#endif
};

গণনা {
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH =
        পরিমাণ(সংরচনা xkb_filter_pointer_lock_button_priv) * CHAR_BIT,
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX =
        (UINT64_C(1) << XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH) - 1
};

static_assert(পরিমাণ(সংরচনা xkb_filter_pointer_lock_button_priv) ==
              পরিমাণ(((সংরচনা xkb_filter*)0)->priv),
              "");
static_assert((uint32_t)XKB_POINTER_BUTTON_MAX < (uint32_t)XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX,
              "xkb_filter_pointer_lock_button_priv cannot store button");

স্থির শূন্য
xkb_filter_pointer_lock_button_new(সংরচনা xkb_server_state *state,
                                   সংরচনা xkb_events *events,
                                   সংরচনা xkb_filter *filter)
{
    যদি (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_চাবিগুলি)) {
        filter->func = রিক্ত;
        প্রত্যাগম;
    }

    state->flags &= ~STATE_REQUIRE_চাবি_EVENT;

    সংরচনা xkb_machine * ধ্রুব sm = (সংরচনা xkb_machine *)state;

    /* Resolve button */
    ধ্রুব xkb_pointer_button_index_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;

    static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                  "invalid UINTx_C() macro");
    static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                  "invalid left shift");
    ধ্রুব xkb_pointer_button_mask_t mask = (UINT8_C(1) << button);

    ধ্রুব bool locked = (sm->mouse.locked_buttons & mask);

    যদি (!locked && !(filter->action.btn.flags & ACTION_LOCK_NO_LOCK)) {
        sm->mouse.locked_buttons |= mask;
        filter->priv = button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_PRESSED, 0);
    }

    সংরচনা xkb_filter_pointer_lock_button_priv *priv =
        (সংরচনা xkb_filter_pointer_lock_button_priv*)&filter->priv;
    priv->button = button,
    priv->locked = locked;
}

স্থির bool
xkb_filter_pointer_lock_button_func(সংরচনা xkb_server_state *state,
                                    সংরচনা xkb_events *events,
                                    সংরচনা xkb_filter *filter,
                                    ধ্রুব সংরচনা xkb_চাবি *চাবি,
                                    গণনা xkb_চাবি_direction direction)
{
    যদি (চাবি != filter->চাবি || !events || state->base.mode != SERVER_STATE)
        প্রত্যাগম XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_চাবি_EVENT;

    বিকল্প (direction) {
    পক্ষ XKB_চাবি_DOWN:
        filter->refcnt++;
        /* fallthrough */
    পক্ষ XKB_চাবি_REPEATED:
        প্রত্যাগম XKB_FILTER_CONSUME;
    স্বভাব:
        যদি (--filter->refcnt > 0)
            প্রত্যাগম XKB_FILTER_CONSUME;
    }

    ধ্রুব সংরচনা xkb_filter_pointer_lock_button_priv *priv =
        (সংরচনা xkb_filter_pointer_lock_button_priv*)&filter->priv;

    যদি (priv->locked && !(filter->action.btn.flags & ACTION_LOCK_NO_UNLOCK)) {
        সংরচনা xkb_machine * ধ্রুব sm = (সংরচনা xkb_machine *)state;

        static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                    "invalid UINTx_C() macro");
        static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                    "invalid left shift");
        ধ্রুব xkb_pointer_button_mask_t mask = (UINT8_C(1) << priv->button);

        sm->mouse.locked_buttons &= ~mask;
        filter->priv = priv->button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);
    }

    filter->func = রিক্ত;
    প্রত্যাগম XKB_FILTER_CONSUME;
}

স্থির শূন্য
xkb_filter_pointer_set_default_button_new(সংরচনা xkb_server_state *state,
                                          সংরচনা xkb_events *events,
                                          সংরচনা xkb_filter *filter)
{
    যদি (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_চাবিগুলি)) {
        filter->func = রিক্ত;
        প্রত্যাগম;
    }

    state->flags &= ~STATE_REQUIRE_চাবি_EVENT;

    সংরচনা xkb_machine * ধ্রুব sm = (সংরচনা xkb_machine *)state;

    static_assert(2 * XKB_POINTER_BUTTON_MAX <= INT_MAX, "lossy conversion");
    পূর্ণাঙ্ক button = (filter->action.dflt.flags & ACTION_ABSOLUTE_SWITCH)
        ? filter->action.dflt.মান
        : sm->mouse.default_button + filter->action.dflt.মান;

    /* Wrap into range */
    ধ্রুব পূর্ণাঙ্ক rem = button % XKB_POINTER_BUTTON_MAX;
    sm->mouse.default_button = (rem >= 0)
        ? (xkb_pointer_button_index_t)rem
        : (xkb_pointer_button_index_t)(rem + XKB_POINTER_BUTTON_MAX);
}

স্থির bool
xkb_filter_pointer_set_default_button_func(সংরচনা xkb_server_state *state,
                                           সংরচনা xkb_events *events,
                                           সংরচনা xkb_filter *filter,
                                           ধ্রুব সংরচনা xkb_চাবি *চাবি,
                                           গণনা xkb_চাবি_direction direction)
{
    যদি (চাবি != filter->চাবি || !events)
        প্রত্যাগম XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_চাবি_EVENT;

    বিকল্প (direction) {
    পক্ষ XKB_চাবি_DOWN:
        filter->refcnt++;
        /* fallthrough */
    পক্ষ XKB_চাবি_REPEATED:
        প্রত্যাগম XKB_FILTER_CONSUME;
    স্বভাব:
        যদি (--filter->refcnt > 0)
            প্রত্যাগম XKB_FILTER_CONSUME;
    }

    filter->func = রিক্ত;
    প্রত্যাগম XKB_FILTER_CONTINUE;
}

স্থির শূন্য
xkb_filter_terminate_server_new(সংরচনা xkb_server_state *state,
                                 সংরচনা xkb_events *events,
                                 সংরচনা xkb_filter *filter)
{
    যদি (!events || state->base.mode != SERVER_STATE ||
        !(((সংরচনা xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = রিক্ত;
        প্রত্যাগম;
    }

    state->flags &= ~STATE_REQUIRE_চাবি_EVENT;

    darray_append(events->queue, (সংরচনা xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    });
}

স্থির bool
xkb_filter_terminate_server_func(সংরচনা xkb_server_state *state,
                                 সংরচনা xkb_events *events,
                                 সংরচনা xkb_filter *filter,
                                 ধ্রুব সংরচনা xkb_চাবি *চাবি,
                                 গণনা xkb_চাবি_direction direction)
{
    যদি (চাবি != filter->চাবি || !events)
        প্রত্যাগম XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_চাবি_EVENT;

    বিকল্প (direction) {
    পক্ষ XKB_চাবি_DOWN:
        filter->refcnt++;
        /* fallthrough */
    পক্ষ XKB_চাবি_REPEATED:
        প্রত্যাগম XKB_FILTER_CONSUME;
    স্বভাব:
        যদি (--filter->refcnt > 0)
            প্রত্যাগম XKB_FILTER_CONSUME;
    }

    filter->func = রিক্ত;
    প্রত্যাগম XKB_FILTER_CONSUME;
}

স্থির শূন্য
xkb_filter_switch_virtual_console_new(সংরচনা xkb_server_state *state,
                                      সংরচনা xkb_events *events,
                                      সংরচনা xkb_filter *filter)
{
    যদি (!events || state->base.mode != SERVER_STATE ||
        !(((সংরচনা xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = রিক্ত;
        প্রত্যাগম;
    }

    state->flags &= ~STATE_REQUIRE_চাবি_EVENT;

    darray_append(events->queue, (সংরচনা xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
        .virtual_console = {
            .index_or_offset = filter->action.screen.screen,
            .is_offset = !(filter->action.screen.flags & ACTION_ABSOLUTE_SWITCH)
        }
    });
}

স্থির bool
xkb_filter_switch_virtual_console_func(সংরচনা xkb_server_state *state,
                                       সংরচনা xkb_events *events,
                                       সংরচনা xkb_filter *filter,
                                       ধ্রুব সংরচনা xkb_চাবি *চাবি,
                                       গণনা xkb_চাবি_direction direction)
{
    যদি (চাবি != filter->চাবি || !events)
        প্রত্যাগম XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_চাবি_EVENT;

    বিকল্প (direction) {
    পক্ষ XKB_চাবি_DOWN:
        filter->refcnt++;
        /* fallthrough */
    পক্ষ XKB_চাবি_REPEATED:
        প্রত্যাগম XKB_FILTER_CONSUME;
    স্বভাব:
        যদি (--filter->refcnt > 0)
            প্রত্যাগম XKB_FILTER_CONSUME;
    }

    filter->func = রিক্ত;
    প্রত্যাগম XKB_FILTER_CONSUME;
}
