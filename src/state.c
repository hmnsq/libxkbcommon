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

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <limits.h>
#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <string.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "xkbcommon/xkbcommon-status.h"
#அந்தர்பாவ "xkbcommon/xkbcommon-features.h"
#அந்தர்பாவ "abi-check.h"
#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "features/enums.h"
#அந்தர்பாவ "keymap.h"
#அந்தர்பாவ "keysym.h"
#அந்தர்பாவ "messages-codes.h"
#அந்தர்பாவ "state-priv.h"
#அந்தர்பாவ "utf8.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "util-mem.h"
#அந்தர்பாவ "util-numbers.h"

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
கட்டமைப்பு xkb_events {
    கட்டமைப்பு xkb_context *ctx;
    darray(கட்டமைப்பு xkb_event) queue;
    /**
     * Read cursor for `xkb_events_next()`.
     *
     * Reset to 0 on each `process_*` call.
     */
    darray_size_t next;
};

கட்டமைப்பு xkb_server_state;

கட்டமைப்பு xkb_filter {
    ஒன்றிப்பு xkb_action action;
    மாறிலி கட்டமைப்பு xkb_சாவி *சாவி;
    bool (*func)(கட்டமைப்பு xkb_server_state *state,
                 கட்டமைப்பு xkb_events *events,
                 கட்டமைப்பு xkb_filter *filter,
                 மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                 எண்ணிக்கை xkb_சாவி_direction direction);
    uint32_t priv;
    முழு refcnt;
};

எண்ணிக்கை xkb_state_mode_internal {
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
    XKB_STATE_TYPE_இலக்கம்_ENTRIES = 5
};

static_assert(CLIENT_STATE == 0 &&
              CLIENT_STATE == SERVER_COMPANION - 1 &&
              SERVER_COMPANION == LEGACY_MIXED_STATE - 1 &&
              LEGACY_MIXED_STATE == LEGACY_SERVER_STATE - 1 &&
              LEGACY_SERVER_STATE == SERVER_STATE - 1 &&
              SERVER_STATE == XKB_STATE_TYPE_இலக்கம்_ENTRIES - 1 &&
              XKB_STATE_TYPE_இலக்கம்_ENTRIES == 5,
              "Order is used to guard client/server -only APIs: do not change");

எண்ணிக்கை { XKB_LAYOUT_INDEX_T_MIN_WIDTH = 6 };
static_assert(XKB_MAX_GROUPS < (1 << XKB_LAYOUT_INDEX_T_MIN_WIDTH),
              "Cannot encode out_of_range_group::redirect_group");

கட்டமைப்பு out_of_range_group {
    எண்ணிக்கை xkb_layout_out_of_range_policy policy :
        (அளவு(எண்ணிக்கை xkb_layout_out_of_range_policy) * CHAR_BIT -
         XKB_LAYOUT_INDEX_T_MIN_WIDTH);
    xkb_layout_index_t redirect_group : XKB_LAYOUT_INDEX_T_MIN_WIDTH;
};

எண்ணிக்கை { XKB_STATE_MODE_INTERNAL_MIN_WIDTH = 4 /* 3 bits + sign */ };
static_assert(XKB_STATE_TYPE_இலக்கம்_ENTRIES <
              (1 << XKB_STATE_MODE_INTERNAL_MIN_WIDTH),
              "Cannot encode xkb_state::mode");

/**
 * Common state
 *
 * `mode` indicates whether the object is casted from a `xkb_server_state` or a
 * `xkb_client_state` struct.
 */
கட்டமைப்பு xkb_state {
    கட்டமைப்பு xkb_keymap *keymap;
    /*
     * Before updating the state, we keep a copy of just this struct. This
     * allows us to report which components of the state have changed.
     */
    கட்டமைப்பு state_components components;
    கட்டமைப்பு out_of_range_group out_of_range_group;

    எண்ணிக்கை xkb_state_mode_internal mode : XKB_STATE_MODE_INTERNAL_MIN_WIDTH;
    முழு refcnt : (அளவு(முழு) * CHAR_BIT - XKB_STATE_MODE_INTERNAL_MIN_WIDTH);
};

/**
 * Minimal client state
 *
 * Wrapper for strong typing
 */
கட்டமைப்பு xkb_client_state {
    /** Inherits from `xkb_state` */
    கட்டமைப்பு xkb_state base;
};

எண்ணிக்கை state_flags {
    STATE_NO_FLAGS = 0,
    STATE_REQUIRE_சாவி_EVENT = (1 << 0),
    STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_சாவி = (1 << 1),
    STATE_UNLOCK_MODS_ON_PRESS = (1 << 2),
};

/**
 * Legacy server state, used for `LEGACY_MIXED_STATE` and `LEGACY_SERVER_STATE`
 * modes.
 *
 * Casted as `xkb_state` in the public API. See `xkb_state::mode`.
 */
கட்டமைப்பு xkb_server_state {
    /** Inherits from `xkb_state` */
    கட்டமைப்பு xkb_state base;

    /* Server-specific state data */

    darray(கட்டமைப்பு xkb_filter) filters;

    /**
     * Mix of constant configuration flags and
     * variable state-processing flags.
     */
    எண்ணிக்கை state_flags flags;

    எண்ணிக்கை xkb_a11y_flags a11y_flags;

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
    int16_t mod_சாவி_மொத்தம்[XKB_MAX_MODS];
};

நிலையான மாறிலி கட்டமைப்பு xkb_சாவி_type_entry *
get_entry_for_mods(மாறிலி கட்டமைப்பு xkb_சாவி_type *type, xkb_mod_mask_t mods)
{
    க்கு (darray_size_t i = 0; i < type->இலக்கம்_entries; i++)
        எனில் (entry_is_active(&type->entries[i]) &&
            type->entries[i].mods.mask == mods)
            திரும்பு &type->entries[i];
    திரும்பு வெற்று;
}

நிலையான மாறிலி கட்டமைப்பு xkb_சாவி_type_entry *
get_entry_for_சாவி_state(கட்டமைப்பு xkb_state *state, மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                        xkb_layout_index_t group)
{
    மாறிலி கட்டமைப்பு xkb_சாவி_type* மாறிலி type = சாவி->groups[group].type;
    xkb_mod_mask_t active_mods = state->components.mods & type->mods.mask;
    திரும்பு get_entry_for_mods(type, active_mods);
}

நிலையான உள்வரி xkb_level_index_t
state_சாவி_get_level(கட்டமைப்பு xkb_state *state, மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                         xkb_layout_index_t layout)
{
    எனில் (layout >= சாவி->இலக்கம்_groups)
        திரும்பு XKB_LEVEL_INVALID;

    /* If we don't find an explicit match the default is 0. */
    மாறிலி கட்டமைப்பு xkb_சாவி_type_entry* மாறிலி entry =
        get_entry_for_சாவி_state(state, சாவி, layout);

    திரும்பு (entry) ? entry->level : 0;
}

/**
 * Returns the level to use for the given key and state, or
 * XKB_LEVEL_INVALID.
 */
xkb_level_index_t
xkb_state_சாவி_get_level(கட்டமைப்பு xkb_state *state, xkb_keycode_t kc,
                        xkb_layout_index_t layout)
{
    மாறிலி கட்டமைப்பு xkb_சாவி* மாறிலி சாவி = Xkb_சாவி(state->keymap, kc);

    திரும்பு (சாவி)
        ? state_சாவி_get_level(state, சாவி, layout)
        : XKB_LEVEL_INVALID;
}

நிலையான உள்வரி xkb_layout_index_t
state_சாவி_get_layout(கட்டமைப்பு xkb_state *state, மாறிலி கட்டமைப்பு xkb_சாவி *சாவி)
{
    திரும்பு xkb_keymap_சாவி_effective_layout(சாவி, state->components.group);
}

/**
 * Returns the layout to use for the given key and state, taking
 * wrapping/clamping/etc into account, or XKB_LAYOUT_INVALID.
 */
xkb_layout_index_t
xkb_state_சாவி_get_layout(கட்டமைப்பு xkb_state *state, xkb_keycode_t kc)
{
    மாறிலி கட்டமைப்பு xkb_சாவி* மாறிலி சாவி = Xkb_சாவி(state->keymap, kc);

    எனில் (!சாவி)
        திரும்பு XKB_LAYOUT_INVALID;

    திரும்பு state_சாவி_get_layout(state, சாவி);
}

/* Empty action used for empty levels */
நிலையான மாறிலி ஒன்றிப்பு xkb_action dummy_action = { .type = ACTION_TYPE_NONE };

நிலையான xkb_action_மொத்தம்_t
xkb_சாவி_get_actions(கட்டமைப்பு xkb_state *state, மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                    மாறிலி ஒன்றிப்பு xkb_action **actions)
{
    மாறிலி xkb_layout_index_t layout = state_சாவி_get_layout(state, சாவி);
    மாறிலி xkb_level_index_t level = state_சாவி_get_level(state, சாவி, layout);
    எனில் (level == XKB_LEVEL_INVALID)
        செல் err;

    மாறிலி xkb_action_மொத்தம்_t மொத்தம் =
        xkb_keymap_சாவி_get_actions_by_level(state->keymap, சாவி,
                                            layout, level, actions);
    எனில் (!மொத்தம்)
        செல் err;

    திரும்பு மொத்தம்;

err:
    /* Use a dummy action if no corresponding level was found or if it is empty.
     * This is required e.g. to handle latches properly. */
    *actions = &dummy_action;
    திரும்பு 1;
}

நிலையான கட்டமைப்பு xkb_filter *
xkb_filter_new(கட்டமைப்பு xkb_server_state *state)
{
    கட்டமைப்பு xkb_filter *filter = வெற்று, *iter;

    darray_foreach(iter, state->filters) {
        எனில் (iter->func)
            தொடர்;
        /* Use available slot */
        filter = iter;
        நிறுத்து;
    }

    எனில் (!filter) {
        /* No available slot: resize the filters array */
        darray_resize0(state->filters, darray_size(state->filters) + 1);
        filter = &darray_item(state->filters, darray_size(state->filters) -1);
    }

    filter->refcnt = 1;
    திரும்பு filter;
}

/***====================================================================***/

எண்ணிக்கை xkb_filter_result {
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
    எனில் ((filter_)->action.group.flags & ACTION_ABSOLUTE_SWITCH)          \
        (state_)->components.component_ = (filter_)->action.group.group; \
    இல்லையெனில்                                                                 \
        (state_)->components.component_ += (filter_)->action.group.group

நிலையான வெறுமை
xkb_filter_group_set_new(கட்டமைப்பு xkb_server_state *state,
                         கட்டமைப்பு xkb_events *events,
                         கட்டமைப்பு xkb_filter *filter)
{
    static_assert(அளவு(state->base.components.base_group) ==
                  அளவு(filter->priv),
                  "Max groups don't fit");
    filter->priv = (uint32_t)state->base.components.base_group;
    apply_group_delta(filter, &state->base, base_group);
}

நிலையான bool
xkb_filter_group_set_func(கட்டமைப்பு xkb_server_state *state,
                          கட்டமைப்பு xkb_events *events,
                          கட்டமைப்பு xkb_filter *filter,
                          மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                          எண்ணிக்கை xkb_சாவி_direction direction)
{
    எனில் (சாவி != filter->சாவி) {
        filter->action.group.flags &= ~ACTION_LOCK_CLEAR;
        திரும்பு XKB_FILTER_CONTINUE;
    }

    தேர்வு (direction) {
    நிலை XKB_சாவி_DOWN:
        filter->refcnt++;
        /* fallthrough */
    நிலை XKB_சாவி_REPEATED:
        திரும்பு XKB_FILTER_CONSUME;
    இயல்பு:
        எனில் (--filter->refcnt > 0)
            திரும்பு XKB_FILTER_CONSUME;
    }

    static_assert(அளவு(state->base.components.base_group) == அளவு(filter->priv),
                  "Max groups don't fit");
    state->base.components.base_group = (int32_t) filter->priv;

    எனில் (filter->action.group.flags & ACTION_LOCK_CLEAR)
        state->base.components.locked_group = 0;

    filter->func = வெற்று;
    திரும்பு XKB_FILTER_CONTINUE;
}

நிலையான எண்ணிக்கை xkb_state_component
get_state_component_changes(மாறிலி கட்டமைப்பு state_components *a,
                            மாறிலி கட்டமைப்பு state_components *b)
{
    எண்ணிக்கை xkb_state_component mask = 0;

    எனில் (a->group != b->group)
        mask |= XKB_STATE_LAYOUT_EFFECTIVE;
    எனில் (a->base_group != b->base_group)
        mask |= XKB_STATE_LAYOUT_DEPRESSED;
    எனில் (a->latched_group != b->latched_group)
        mask |= XKB_STATE_LAYOUT_LATCHED;
    எனில் (a->locked_group != b->locked_group)
        mask |= XKB_STATE_LAYOUT_LOCKED;
    எனில் (a->mods != b->mods)
        mask |= XKB_STATE_MODS_EFFECTIVE;
    எனில் (a->base_mods != b->base_mods)
        mask |= XKB_STATE_MODS_DEPRESSED;
    எனில் (a->latched_mods != b->latched_mods)
        mask |= XKB_STATE_MODS_LATCHED;
    எனில் (a->locked_mods != b->locked_mods)
        mask |= XKB_STATE_MODS_LOCKED;
    எனில் (a->leds != b->leds)
        mask |= XKB_STATE_LEDS;
    எனில் (a->controls != b->controls)
        mask |= XKB_STATE_CONTROLS_EFFECTIVE;

    திரும்பு mask;
}

நிலையான வெறுமை
xkb_filter_group_lock_new(கட்டமைப்பு xkb_server_state *state,
                          கட்டமைப்பு xkb_events *events,
                          கட்டமைப்பு xkb_filter *filter)
{
    எனில் (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release: do nothing on key press.
         *
         * This is a keymap format v2 extension.
         */
        திரும்பு;
    } இல்லையெனில் {
        /* Lock on key press */
        apply_group_delta(filter, &state->base, locked_group);
    }
}

நிலையான bool
xkb_filter_group_lock_func(கட்டமைப்பு xkb_server_state *state,
                           கட்டமைப்பு xkb_events *events,
                           கட்டமைப்பு xkb_filter *filter,
                           மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                           எண்ணிக்கை xkb_சாவி_direction direction)
{
    எனில் (சாவி != filter->சாவி) {
        எனில் ((filter->action.group.flags & ACTION_LOCK_ON_RELEASE) &&
            direction == XKB_சாவி_DOWN) {
            /*
             * Another key has been pressed after the locking key:
             * cancel group lock on release.
             *
             * This is a keymap v2 extension.
             */
            filter->action.group.flags &= ~ACTION_LOCK_ON_RELEASE;
        }
        திரும்பு XKB_FILTER_CONTINUE;
    }

    தேர்வு (direction) {
    நிலை XKB_சாவி_DOWN:
        filter->refcnt++;
        /* fallthrough */
    நிலை XKB_சாவி_REPEATED:
        திரும்பு XKB_FILTER_CONSUME;
    இயல்பு:
        எனில் (--filter->refcnt > 0)
            திரும்பு XKB_FILTER_CONSUME;
    }

    எனில் (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release
         *
         * This is a keymap v2 extension.
         */
        apply_group_delta(filter, &state->base, locked_group);
    } இல்லையெனில் {
        /* Lock on key press: do nothing on key release. */
    }
    filter->func = வெற்று;
    திரும்பு XKB_FILTER_CONTINUE;
}

நிலையான bool
xkb_action_breaks_latch(மாறிலி ஒன்றிப்பு xkb_action *action,
                        எண்ணிக்கை xkb_action_controls controls,
                        எண்ணிக்கை xkb_internal_action_flags flag,
                        xkb_mod_mask_t mask)
{
    தேர்வு (action->type) {
    நிலை ACTION_TYPE_NONE:
    நிலை ACTION_TYPE_VOID:
    நிலை ACTION_TYPE_PTR_BUTTON:
    நிலை ACTION_TYPE_PTR_LOCK:
    நிலை ACTION_TYPE_CTRL_SET:
    நிலை ACTION_TYPE_CTRL_LOCK:
    நிலை ACTION_TYPE_SWITCH_VT:
    நிலை ACTION_TYPE_TERMINATE:
    நிலை ACTION_TYPE_REDIRECT_சாவி:
        திரும்பு true;
    நிலை ACTION_TYPE_PTR_MOVE:
    நிலை ACTION_TYPE_PTR_DEFAULT:
        திரும்பு !(controls & CONTROL_MOUSE_சாவிகள்);
    நிலை ACTION_TYPE_INTERNAL:
        திரும்பு (action->internal.flags & flag) &&
               ((action->internal.clear_latched_mods & mask) == mask);
    இயல்பு:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_இலக்கம்_ENTRIES,
                      "Missing action type");
        திரும்பு false;
    }
}

எண்ணிக்கை xkb_சாவி_latch_state {
    NO_LATCH = 0,
    LATCH_சாவி_DOWN,
    LATCH_PENDING,
    சாவி_LATCH_STATE_இலக்கம்_ENTRIES
};

#define MAX_XKB_சாவி_LATCH_STATE_LOG2 2
#எனில் (சாவி_LATCH_STATE_இலக்கம்_ENTRIES > (1 << MAX_XKB_சாவி_LATCH_STATE_LOG2)) || \
    (-XKB_MAX_GROUPS) < (INT32_MIN >> MAX_XKB_சாவி_LATCH_STATE_LOG2) || \
      XKB_MAX_GROUPS > (INT32_MAX >> MAX_XKB_சாவி_LATCH_STATE_LOG2)
#error "Cannot represent priv field of the group latch filter"
#endif

/* Hold the latch state *and* the group delta */
ஒன்றிப்பு group_latch_priv {
    uint32_t priv;
    கட்டமைப்பு {
        /* The type is really: enum xkb_key_latch_state, but it is problematic
         * on Windows, because it is interpreted as signed and leads to wrong
         * negative values. */
        unsigned முழு latch:MAX_XKB_சாவி_LATCH_STATE_LOG2;
        int32_t group_delta:(32 - MAX_XKB_சாவி_LATCH_STATE_LOG2);
    };
};

நிலையான வெறுமை
xkb_filter_group_latch_new(கட்டமைப்பு xkb_server_state *state,
                           கட்டமைப்பு xkb_events *events,
                           கட்டமைப்பு xkb_filter *filter)
{
    மாறிலி ஒன்றிப்பு group_latch_priv priv = {
        .latch = LATCH_சாவி_DOWN,
        .group_delta = (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)
            ? filter->action.group.group - state->base.components.base_group
            : filter->action.group.group
    };
    filter->priv = priv.priv;
    /* Like group set */
    apply_group_delta(filter, &state->base, base_group);
}

நிலையான bool
xkb_filter_group_latch_func(கட்டமைப்பு xkb_server_state *state,
                            கட்டமைப்பு xkb_events *events,
                            கட்டமைப்பு xkb_filter *filter,
                            மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                            எண்ணிக்கை xkb_சாவி_direction direction)
{
    ஒன்றிப்பு group_latch_priv priv = {.priv = filter->priv};
    எண்ணிக்கை xkb_சாவி_latch_state latch = priv.latch;

    எனில் (direction == XKB_சாவி_DOWN) {
        மாறிலி ஒன்றிப்பு xkb_action *actions = வெற்று;
        மாறிலி xkb_action_மொத்தம்_t மொத்தம் = xkb_சாவி_get_actions(&state->base, சாவி,
                                                             &actions);

        எனில் (latch == LATCH_சாவி_DOWN) {
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
            எனில் (state->a11y_flags & XKB_A11_Y_LATCH_SIMULTANEOUS_சாவிகள்) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                க்கு (xkb_action_மொத்தம்_t k = 0; k < மொத்தம்; k++) {
                    எனில் (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_GROUP_LATCH,
                                                0))
                    {
                        latch = NO_LATCH;
                        நிறுத்து;
                    }
                }
            } இல்லையெனில் {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        இல்லையெனில் எனில் (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is
             * pressed, or promote it to a lock if it's the same group delta &
             * flags and latchToLock option is enabled. */
            மாறிலி bool sticky_சாவிகள் = state->base.components.controls
                                   & CONTROL_STICKY_சாவிகள்;
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
            க்கு (xkb_action_மொத்தம்_t k = 0; k < மொத்தம்; k++) {
                எனில் (((actions[k].type == ACTION_TYPE_GROUP_LATCH) ||
                     (actions[k].type == ACTION_TYPE_GROUP_SET && sticky_சாவிகள்)) &&
                     actions[k].group.group == filter->action.group.group &&
                     (actions[k].group.flags & ACTION_ABSOLUTE_SWITCH) ==
                     (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)) {
                    எனில் (filter->action.group.flags & ACTION_LATCH_TO_LOCK &&
                        filter->action.group.group != 0)
                    {
                        /* Promote to lock */
                        filter->action.type = ACTION_TYPE_GROUP_LOCK;
                        filter->func = xkb_filter_group_lock_func;
                        xkb_filter_group_lock_new(state, events, filter);
                        state->base.components.latched_group -= priv.group_delta;
                        filter->சாவி = சாவி;
                        /* XXX beep beep! */
                        திரும்பு XKB_FILTER_CONSUME;
                    }
                    /* Do nothing if `latchToLock` option is not activated; if
                     * the latch is not broken by the following actions and the
                     * key is not consumed, then another latch filter will be
                     * created.
                     */
                    தொடர்;
                }
                இல்லையெனில் எனில் (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_GROUP_LATCH,
                                                 0))
                {
                    /* Breaks the latch */
                    state->base.components.latched_group -= priv.group_delta;
                    filter->func = வெற்று;
                    திரும்பு XKB_FILTER_CONTINUE;
                }
            }
        }
        இல்லையெனில் {
            /* Ignore press in NO_LATCH state */
            ப்ரதிஜ்ஞா(latch == NO_LATCH);
        }
    }
    இல்லையெனில் எனில் (சாவி != filter->சாவி) {
        /* Ignore repeat/release of other keys */
    }
    இல்லையெனில் எனில் (direction == XKB_சாவி_REPEATED) {
        திரும்பு XKB_FILTER_CONSUME; // TODO: check
    }
    இல்லையெனில் {
        /* Our key got released.  If we've set it to clear locks, and we
         * currently have a group locked, then release it and
         * don't actually latch.  Else we've actually hit the latching
         * stage, so set PENDING and move our group from base to
         * latched. */
        எனில் ((filter->action.group.flags & ACTION_LOCK_CLEAR) &&
             state->base.components.locked_group) {
            எனில் (latch == LATCH_PENDING)
                state->base.components.latched_group -= priv.group_delta;
            இல்லையெனில்
                state->base.components.base_group -= priv.group_delta;
            state->base.components.locked_group = 0;
            filter->func = வெற்று;
        }
        /* Broken latch */
        இல்லையெனில் எனில் (latch == NO_LATCH) {
            state->base.components.base_group -= priv.group_delta;
            filter->func = வெற்று;
        }
        /* We may already have reached the latch state if pressing the
         * key multiple times without latch-to-lock enabled. */
        இல்லையெனில் எனில் (latch == LATCH_சாவி_DOWN) {
            latch = LATCH_PENDING;
            /* Switch from set to latch */
            state->base.components.base_group -= priv.group_delta;
            state->base.components.latched_group += priv.group_delta;
            /* XXX beep beep! */
        }
    }

    priv.latch = latch;
    filter->priv = priv.priv;

    திரும்பு XKB_FILTER_CONTINUE;
}

நிலையான வெறுமை
xkb_filter_mod_set_new(கட்டமைப்பு xkb_server_state *state,
                       கட்டமைப்பு xkb_events *events,
                       கட்டமைப்பு xkb_filter *filter)
{
    மாறிலி எண்ணிக்கை xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    எனில் ((filter->action.mods.flags & unlock) == unlock) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension.
         */
        filter->priv = filter->action.mods.mods.mask
                     & ~state->base.components.locked_mods;
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
    } இல்லையெனில் {
        filter->priv = filter->action.mods.mods.mask;
    }

    state->set_mods |= (xkb_mod_mask_t) filter->priv;
}

நிலையான bool
xkb_filter_mod_set_func(கட்டமைப்பு xkb_server_state *state,
                        கட்டமைப்பு xkb_events *events,
                        கட்டமைப்பு xkb_filter *filter,
                        மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                        எண்ணிக்கை xkb_சாவி_direction direction)
{

    எனில் (சாவி != filter->சாவி) {
        filter->action.mods.flags &= ~ACTION_LOCK_CLEAR;
        திரும்பு XKB_FILTER_CONTINUE;
    }

    தேர்வு (direction) {
    நிலை XKB_சாவி_DOWN:
        filter->refcnt++;
        /* fallthrough */
    நிலை XKB_சாவி_REPEATED:
        திரும்பு XKB_FILTER_CONSUME;
    இயல்பு:
        எனில் (--filter->refcnt > 0)
            திரும்பு XKB_FILTER_CONSUME;
    }

    state->clear_mods |= (xkb_mod_mask_t) filter->priv;
    மாறிலி எண்ணிக்கை xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    எனில் ((filter->action.mods.flags & unlock) == ACTION_LOCK_CLEAR)
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;

    filter->func = வெற்று;
    திரும்பு XKB_FILTER_CONTINUE;
}

நிலையான வெறுமை
xkb_filter_mod_lock_new(கட்டமைப்பு xkb_server_state *state,
                        கட்டமைப்பு xkb_events *events,
                        கட்டமைப்பு xkb_filter *filter)
{
    filter->priv = (state->base.components.locked_mods &
                    filter->action.mods.mods.mask);

    எனில் (filter->priv && (filter->action.mods.flags & ACTION_UNLOCK_ON_PRESS)) {
        /*
         * Some of the target modifiers were locked before key press: unlock.
         *
         * This is a keymap v2 extension: unlock-on-press.
         */
        எனில் (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
            state->base.components.locked_mods &= ~filter->priv;
        /* No further action: cancel filter */
        filter->func = வெற்று;
    } இல்லையெனில் {
        /* Set base mods; lock mods if relevant (XKB 1.0 spec) */
        state->set_mods |= filter->action.mods.mods.mask;
        எனில் (!(filter->action.mods.flags & ACTION_LOCK_NO_LOCK))
            state->base.components.locked_mods |= filter->action.mods.mods.mask;
    }
}

நிலையான bool
xkb_filter_mod_lock_func(கட்டமைப்பு xkb_server_state *state,
                         கட்டமைப்பு xkb_events *events,
                         கட்டமைப்பு xkb_filter *filter,
                         மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                         எண்ணிக்கை xkb_சாவி_direction direction)
{
    எனில் (சாவி != filter->சாவி)
        திரும்பு XKB_FILTER_CONTINUE;

    தேர்வு (direction) {
    நிலை XKB_சாவி_DOWN:
        filter->refcnt++;
        /* fallthrough */
    நிலை XKB_சாவி_REPEATED:
        திரும்பு XKB_FILTER_CONSUME;
    இயல்பு:
        எனில் (--filter->refcnt > 0)
            திரும்பு XKB_FILTER_CONSUME;
    }

    state->clear_mods |= filter->action.mods.mods.mask;
    எனில் (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
        state->base.components.locked_mods &= ~filter->priv;

    filter->func = வெற்று;
    திரும்பு XKB_FILTER_CONTINUE;
}

நிலையான வெறுமை
xkb_filter_mod_latch_new(கட்டமைப்பு xkb_server_state *state,
                         கட்டமைப்பு xkb_events *events,
                         கட்டமைப்பு xkb_filter *filter)
{
    /* Latch-on-press + clear-locks imply unlock-on-press */
    மாறிலி எண்ணிக்கை xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                              | ACTION_LATCH_ON_PRESS;

    எனில் ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
        (filter->action.mods.flags & unlockOnPress) &&
        (state->base.components.locked_mods & filter->action.mods.mods.mask) ==
         filter->action.mods.mods.mask) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension: clear locks and do not latch.
         */
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
        filter->func = வெற்று;
    } இல்லையெனில் எனில் (filter->action.mods.flags & ACTION_LATCH_ON_PRESS) {
        /*
         * Latch on key press
         *
         * This is a keymap format v2 extension.
         */
        filter->priv = LATCH_PENDING;
        state->base.components.latched_mods |= filter->action.mods.mods.mask;
        /* XXX beep beep! */
    } இல்லையெனில் {
        /* XKB standard latch action */
        filter->priv = LATCH_சாவி_DOWN;
        state->set_mods |= filter->action.mods.mods.mask;
    }
}

நிலையான bool
xkb_filter_mod_latch_func(கட்டமைப்பு xkb_server_state *state,
                          கட்டமைப்பு xkb_events *events,
                          கட்டமைப்பு xkb_filter *filter,
                          மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                          எண்ணிக்கை xkb_சாவி_direction direction)
{
    எண்ணிக்கை xkb_சாவி_latch_state latch = filter->priv;

    எனில் (direction == XKB_சாவி_DOWN) {
        மாறிலி ஒன்றிப்பு xkb_action *actions = வெற்று;
        மாறிலி xkb_action_மொத்தம்_t மொத்தம் = xkb_சாவி_get_actions(&state->base, சாவி,
                                                             &actions);

        எனில் (latch == LATCH_சாவி_DOWN) {
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
            எனில் (state->a11y_flags & XKB_A11_Y_LATCH_SIMULTANEOUS_சாவிகள்) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                க்கு (xkb_action_மொத்தம்_t k = 0; k < மொத்தம்; k++) {
                    எனில் (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_MOD_LATCH,
                                                filter->action.mods.mods.mask))
                    {
                        latch = NO_LATCH;
                        நிறுத்து;
                    }
                }
            } இல்லையெனில் {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        இல்லையெனில் எனில் (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is pressed,
             * or promote it to a lock or plain base set if it's the same
             * modifier. */
            மாறிலி bool sticky_சாவிகள் = state->base.components.controls
                                   & CONTROL_STICKY_சாவிகள்;
            /*
             * NOTE: Action’s flags are not compared:
             * - flags may have been modified by keyboard controls at the
             *   creation of the filter;
             * - it is unlikely to have two LatchMods() actions on the same
             *   key that differ only by their flags;
             * - even if it were the case, one would most likely want both
             *   actions to be considered compatible in this context.
             */
            க்கு (xkb_action_மொத்தம்_t k = 0; k < மொத்தம்; k++) {
                எனில் (((actions[k].type == ACTION_TYPE_MOD_LATCH) ||
                     (actions[k].type == ACTION_TYPE_MOD_SET && sticky_சாவிகள்)) &&
                    actions[k].mods.mods.mask == filter->action.mods.mods.mask)
                {
                    எனில் (filter->action.mods.flags & ACTION_LATCH_TO_LOCK) {
                        /* Mutate the action to LockMods() */
                        filter->action.type = ACTION_TYPE_MOD_LOCK;
                        filter->func = xkb_filter_mod_lock_func;
                        xkb_filter_mod_lock_new(state, events, filter);
                    }
                    இல்லையெனில் {
                        /* Mutate the action to SetMods() */
                        filter->action.type = ACTION_TYPE_MOD_SET;
                        filter->func = xkb_filter_mod_set_func;
                        xkb_filter_mod_set_new(state, events, filter);
                    }
                    filter->சாவி = சாவி;
                    /* Clear latches */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    /* XXX beep beep! */
                    திரும்பு XKB_FILTER_CONSUME;
                }
                இல்லையெனில் எனில் (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_MOD_LATCH,
                                                 filter->action.mods.mods.mask))
                {
                    /* XXX: This may be totally broken, we might need to break the
                     *      latch in the next run after this press? */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    filter->func = வெற்று;
                    திரும்பு XKB_FILTER_CONTINUE;
                }
            }
        }
        இல்லையெனில் {
            /* Ignore press in NO_LATCH state */
            ப்ரதிஜ்ஞா(latch == NO_LATCH);
        }
    }
    இல்லையெனில் எனில் (சாவி != filter->சாவி) {
        /* Ignore repeat/release of other keys */
    }
    இல்லையெனில் எனில் (direction == XKB_சாவி_REPEATED) {
        திரும்பு XKB_FILTER_CONSUME; // TODO: check
    }
    இல்லையெனில் {
        /* Our key got released.  If we’ve set it to clear locks, and we
         * currently have the same modifiers locked, then release them and
         * don't actually latch.  Else we’ve actually hit the latching
         * stage, so set PENDING and move our modifier from base to
         * latched. */

        /* Latch-on-press + clear-locks imply unlock-on-press */
        மாறிலி எண்ணிக்கை xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                                  | ACTION_LATCH_ON_PRESS;

        எனில் ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
            !(filter->action.mods.flags & unlockOnPress) &&
            (state->base.components.locked_mods &
             filter->action.mods.mods.mask) == filter->action.mods.mods.mask)
        {
            /* XXX: We might be a bit overenthusiastic about clearing
             *      mods other filters have set here? */
            எனில் (latch == LATCH_PENDING)
                state->base.components.latched_mods &=
                    ~filter->action.mods.mods.mask;
            இல்லையெனில்
                state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
            filter->func = வெற்று;
        }
        இல்லையெனில் எனில் (latch == NO_LATCH) {
            /* Broken latch */
            state->clear_mods |= filter->action.mods.mods.mask;
            filter->func = வெற்று;
        }
        இல்லையெனில் எனில் (!(filter->action.mods.flags & ACTION_LATCH_ON_PRESS)) {
            latch = LATCH_PENDING;
            state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.latched_mods |= filter->action.mods.mods.mask;
            /* XXX beep beep! */
        }
    }

    filter->priv = latch;

    திரும்பு XKB_FILTER_CONTINUE;
}

நிலையான வெறுமை
append_pointer_move(கட்டமைப்பு xkb_events *events,
                    கட்டமைப்பு xkb_filter *filter)
{
    எண்ணிக்கை xkb_pointer_motion_flags flags = 0;
    எனில் (filter->action.ptr.flags & ACTION_ABSOLUTE_X)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_X;
    எனில் (filter->action.ptr.flags & ACTION_ABSOLUTE_Y)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_Y;
    எனில் (filter->action.ptr.flags & ACTION_REPEAT)
        flags |= XKB_POINTER_MOTION_REPEATS;
    darray_append(events->queue, (கட்டமைப்பு xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_MOTION,
        .pointer_motion = {
            .size = அளவு(((கட்டமைப்பு xkb_event*)0)->pointer_motion),
            .flags = flags,
            .x = filter->action.ptr.x,
            .y = filter->action.ptr.y,
        }
    });
}

நிலையான வெறுமை
xkb_filter_pointer_move_new(கட்டமைப்பு xkb_server_state *state,
                            கட்டமைப்பு xkb_events *events,
                            கட்டமைப்பு xkb_filter *filter)
{
    எனில் (!events) {
        filter->func = வெற்று;
        திரும்பு;
    }

    ப்ரதிஜ்ஞா(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_சாவிகள்);
    append_pointer_move(events, filter);
    state->flags &= ~STATE_REQUIRE_சாவி_EVENT;
}

நிலையான bool
xkb_filter_pointer_move_func(கட்டமைப்பு xkb_server_state *state,
                             கட்டமைப்பு xkb_events *events,
                             கட்டமைப்பு xkb_filter *filter,
                             மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                             எண்ணிக்கை xkb_சாவி_direction direction)
{
    எனில் (சாவி != filter->சாவி || !events)
        திரும்பு XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_சாவி_EVENT;

    தேர்வு (direction) {
    நிலை XKB_சாவி_DOWN:
        filter->refcnt++;
        append_pointer_move(events, filter);
        திரும்பு XKB_FILTER_CONSUME;
    நிலை XKB_சாவி_REPEATED:
        எனில் (filter->action.ptr.flags & ACTION_REPEAT)
            append_pointer_move(events, filter);
        திரும்பு XKB_FILTER_CONSUME;
    இயல்பு:
        எனில் (--filter->refcnt > 0)
            திரும்பு XKB_FILTER_CONSUME;
    }

    filter->func = வெற்று;
    திரும்பு XKB_FILTER_CONSUME;
}

/* Implemented with xkb_machine API because it requires struct xkb_machine */
நிலையான வெறுமை
xkb_filter_pointer_button_new(கட்டமைப்பு xkb_server_state *state,
                              கட்டமைப்பு xkb_events *events,
                              கட்டமைப்பு xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
நிலையான bool
xkb_filter_pointer_button_func(கட்டமைப்பு xkb_server_state *state,
                               கட்டமைப்பு xkb_events *events,
                               கட்டமைப்பு xkb_filter *filter,
                               மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                               எண்ணிக்கை xkb_சாவி_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
நிலையான வெறுமை
xkb_filter_pointer_lock_button_new(கட்டமைப்பு xkb_server_state *state,
                                   கட்டமைப்பு xkb_events *events,
                                   கட்டமைப்பு xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
நிலையான bool
xkb_filter_pointer_lock_button_func(கட்டமைப்பு xkb_server_state *state,
                                    கட்டமைப்பு xkb_events *events,
                                    கட்டமைப்பு xkb_filter *filter,
                                    மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                                    எண்ணிக்கை xkb_சாவி_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
நிலையான வெறுமை
xkb_filter_pointer_set_default_button_new(கட்டமைப்பு xkb_server_state *state,
                                          கட்டமைப்பு xkb_events *events,
                                          கட்டமைப்பு xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
நிலையான bool
xkb_filter_pointer_set_default_button_func(கட்டமைப்பு xkb_server_state *state,
                                           கட்டமைப்பு xkb_events *events,
                                           கட்டமைப்பு xkb_filter *filter,
                                           மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                                           எண்ணிக்கை xkb_சாவி_direction direction);

நிலையான வெறுமை
xkb_filter_terminate_server_new(கட்டமைப்பு xkb_server_state *state,
                                 கட்டமைப்பு xkb_events *events,
                                 கட்டமைப்பு xkb_filter *filter);

நிலையான bool
xkb_filter_terminate_server_func(கட்டமைப்பு xkb_server_state *state,
                                  கட்டமைப்பு xkb_events *events,
                                  கட்டமைப்பு xkb_filter *filter,
                                  மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                                  எண்ணிக்கை xkb_சாவி_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
நிலையான வெறுமை
xkb_filter_switch_virtual_console_new(கட்டமைப்பு xkb_server_state *state,
                                      கட்டமைப்பு xkb_events *events,
                                      கட்டமைப்பு xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
நிலையான bool
xkb_filter_switch_virtual_console_func(கட்டமைப்பு xkb_server_state *state,
                                       கட்டமைப்பு xkb_events *events,
                                       கட்டமைப்பு xkb_filter *filter,
                                       மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                                       எண்ணிக்கை xkb_சாவி_direction direction);

நிலையான உள்வரி வெறுமை
clear_all_latches_and_locks(கட்டமைப்பு xkb_server_state *state,
                            கட்டமைப்பு xkb_events *events);

நிலையான வெறுமை
xkb_filter_ctrls_new(கட்டமைப்பு xkb_server_state *state,
                     கட்டமைப்பு xkb_events *events,
                     கட்டமைப்பு xkb_filter *filter)
{
    எனில் (filter->action.type == ACTION_TYPE_CTRL_SET) {
        /* Set: save the specified controls that are *not* already enabled */
        filter->priv = (uint32_t) (
            (~state->base.components.controls) & filter->action.ctrls.ctrls
        );
    } இல்லையெனில் {
        /* Lock: save the specified controls that *are* already enabled */
        filter->priv = (uint32_t) (
            state->base.components.controls & filter->action.ctrls.ctrls
        );
    }

    எனில் (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_LOCK)) {
        /* Enable the specified controls that are not already enabled */
        state->base.components.controls |= filter->action.ctrls.ctrls;
    }
}

நிலையான bool
xkb_filter_ctrls_func(கட்டமைப்பு xkb_server_state *state,
                      கட்டமைப்பு xkb_events *events,
                      கட்டமைப்பு xkb_filter *filter,
                      மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                      எண்ணிக்கை xkb_சாவி_direction direction)
{
    எனில் (சாவி != filter->சாவி)
        திரும்பு XKB_FILTER_CONTINUE;

    தேர்வு (direction) {
    நிலை XKB_சாவி_DOWN:
        filter->refcnt++;
        /* fallthrough */
    நிலை XKB_சாவி_REPEATED:
        திரும்பு XKB_FILTER_CONSUME;
    இயல்பு:
        எனில் (--filter->refcnt > 0)
            திரும்பு XKB_FILTER_CONSUME;
    }

    எனில் (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_UNLOCK)) {
        மாறிலி எண்ணிக்கை xkb_action_controls old = state->base.components.controls;

        /*
         * Set: Disable specified controls that were *not* enabled at key press.
         * Lock: Disable specified controls that *were* enabled at key press.
         */
        state->base.components.controls &= ~(எண்ணிக்கை xkb_action_controls) filter->priv;

        எனில் ((old & CONTROL_STICKY_சாவிகள்) &&
            !(state->base.components.controls & CONTROL_STICKY_சாவிகள்)) {
            /* Sticky keys were disabled: clear all locks and latches */
            clear_all_latches_and_locks(state, events);
        }
    }

    filter->func = வெற்று;
    திரும்பு XKB_FILTER_CONTINUE;
}

நிலையான bool
append_redirect_சாவி_events(கட்டமைப்பு xkb_server_state *state,
                           கட்டமைப்பு xkb_events *events,
                           மாறிலி கட்டமைப்பு xkb_redirect_சாவி_action *redirect,
                           எண்ணிக்கை xkb_சாவி_direction direction)
{
    எண்ணிக்கை xkb_state_component changed = 0;
    மாறிலி xkb_mod_mask_t mask = redirect->affect_mods;

    /*
     * Reference state: find the last state update in the queue, otherwise
     * use the current state.
     */
    கட்டமைப்பு xkb_event *event;
    கட்டமைப்பு state_components last_components = state->base.components;
    darray_foreach_reverse(event, events->queue) {
        எனில் (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
            last_components = event->components.components;
            நிறுத்து;
        }
    }

    எனில் (mask) {
        கட்டமைப்பு state_components new = last_components;
        new.base_mods = (new.base_mods & ~mask) | redirect->mods;
        new.latched_mods = (new.latched_mods & ~mask) | redirect->mods;
        new.locked_mods = (new.locked_mods & ~mask) | redirect->mods;
        new.mods = (new.mods & ~mask) | redirect->mods;

        changed = get_state_component_changes(&last_components, &new);
        எனில் (changed) {
            darray_append(events->queue, (கட்டமைப்பு xkb_event) {
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
            darray_append(events->queue, (கட்டமைப்பு xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_FRAME,
            });
        }
    }

    darray_append(events->queue, (கட்டமைப்பு xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_சாவி,
        .சாவி = {
            .keycode = redirect->keycode,
            .direction = direction
        }
    });

    எனில் (mask && changed) {
        /* Restore state */
        darray_append(events->queue, (கட்டமைப்பு xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = last_components,
                .changed = changed
            }
        });
    }

    state->flags &= ~STATE_REQUIRE_சாவி_EVENT;
    திரும்பு true;
}

நிலையான வெறுமை
xkb_filter_redirect_சாவி_new(கட்டமைப்பு xkb_server_state *state,
                            கட்டமைப்பு xkb_events *events,
                            கட்டமைப்பு xkb_filter *filter)
{
    /* Action effectual only with the xkb_machine API and a valid keycode */
    எனில் (!events || filter->action.redirect.keycode == XKB_KEYCODE_INVALID) {
        filter->func = வெற்று;
        திரும்பு;
    }
    append_redirect_சாவி_events(state, events, &filter->action.redirect,
                               XKB_சாவி_DOWN);
}

நிலையான bool
xkb_filter_redirect_சாவி_func(கட்டமைப்பு xkb_server_state *state,
                             கட்டமைப்பு xkb_events *events,
                             கட்டமைப்பு xkb_filter *filter,
                             மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                             எண்ணிக்கை xkb_சாவி_direction direction)
{
    எனில் (சாவி != filter->சாவி)
        திரும்பு XKB_FILTER_CONTINUE;

    எனில் (direction == XKB_சாவி_UP) {
        append_redirect_சாவி_events(state, events,
                                   &filter->action.redirect, XKB_சாவி_UP);
        filter->func = வெற்று;
        திரும்பு XKB_FILTER_CONSUME;
     } இல்லையெனில் எனில் (direction == XKB_சாவி_DOWN) {
        மாறிலி ஒன்றிப்பு xkb_action *actions = வெற்று;
        மாறிலி xkb_action_மொத்தம்_t மொத்தம் =
            xkb_சாவி_get_actions(&state->base, சாவி, &actions);

        க்கு (xkb_action_மொத்தம்_t a = 0; a < மொத்தம்; a++) {
            எனில் (actions[a].type == ACTION_TYPE_REDIRECT_சாவி &&
                actions[a].redirect.keycode != filter->action.redirect.keycode) {
                    /* One action redirects to a different key: release first */
                    append_redirect_சாவி_events(state, events,
                                               &filter->action.redirect,
                                               XKB_சாவி_UP);
                filter->func = வெற்று;
                திரும்பு XKB_FILTER_CONTINUE;
            }
        }
    }

    append_redirect_சாவி_events(state, events, &filter->action.redirect,
                               direction);
    திரும்பு XKB_FILTER_CONSUME;
}

நிலையான மாறிலி கட்டமைப்பு {
    வெறுமை (*new)(கட்டமைப்பு xkb_server_state *state,
                கட்டமைப்பு xkb_events *events,
                கட்டமைப்பு xkb_filter *filter);
    bool (*func)(கட்டமைப்பு xkb_server_state *state,
                 கட்டமைப்பு xkb_events *events,
                 கட்டமைப்பு xkb_filter *filter,
                 மாறிலி கட்டமைப்பு xkb_சாவி *சாவி, எண்ணிக்கை xkb_சாவி_direction direction);
} filter_action_funcs[ACTION_TYPE_இலக்கம்_ENTRIES] = {
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
    [ACTION_TYPE_REDIRECT_சாவி] = { xkb_filter_redirect_சாவி_new,
                                   xkb_filter_redirect_சாவி_func },
};

/**
 * Applies any relevant filters to the key, first from the list of filters
 * that are currently active, then if no filter has claimed the key, possibly
 * apply a new filter from the key action.
 */
நிலையான வெறுமை
xkb_filter_apply_all(கட்டமைப்பு xkb_server_state *state,
                     கட்டமைப்பு xkb_events *events,
                     மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                     எண்ணிக்கை xkb_சாவி_direction direction)
{
    /* First run through all the currently active filters and see if any of
     * them have consumed this event. */
    bool consumed = false;
    கட்டமைப்பு xkb_filter *filter;
    darray_foreach(filter, state->filters) {
        எனில் (!filter->func)
            தொடர்;

        எனில் (filter->func(state, events, filter, சாவி, direction) == XKB_FILTER_CONSUME)
            consumed = true;
    }
    எனில் (consumed || direction == XKB_சாவி_UP)
        திரும்பு;

    /* No filter consumed this event, so proceed with the key actions */
    மாறிலி ஒன்றிப்பு xkb_action *actions = வெற்று;
    xkb_action_மொத்தம்_t மொத்தம் = xkb_சாவி_get_actions(&state->base, சாவி,
                                                         &actions);

    க்கு (கட்டமைப்பு {
            xkb_action_மொத்தம்_t மொத்தம்;
            மாறிலி ஒன்றிப்பு xkb_action *actions;
         } next = {0};
         மொத்தம்;
         மொத்தம் = next.மொத்தம், next.மொத்தம் = 0, actions = next.actions
    ) {
        /*
         * Process actions sequentially.
         *
         * NOTE: We rely on the parser to disallow multiple modifier or group
         * actions (see `CheckMultipleActionsCategories`). Allowing multiple
         * such actions requires a refactor of the state handling.
         */
        க்கு (xkb_action_மொத்தம்_t k = 0; k < மொத்தம்; k++) {
            /*
             * It's possible for the keymap to set action->type explicitly, like
             * so:
             *     interpret XF86_Next_VMode {
             *         action = Private(type=0x86, data="+VMode");
             *     };
             * We don't handle those.
             */
            எனில் (actions[k].type >= ACTION_TYPE_இலக்கம்_ENTRIES)
                தொடர்;

            /* Go to next action if no corresponding action handler */
            எனில் (!filter_action_funcs[actions[k].type].new)
                தொடர்;

            /* Add a new filter and run the corresponding initial action */
            filter = xkb_filter_new(state);
            filter->சாவி = சாவி;
            filter->action = actions[k];

            தேர்வு (filter->action.type) {
            நிலை ACTION_TYPE_MOD_SET:
                எனில் (state->base.components.controls & CONTROL_STICKY_சாவிகள்) {
                    /* Convert modifier set action to a latch */
                    filter->action.type = ACTION_TYPE_MOD_LATCH;
                    எனில் (state->a11y_flags & XKB_A11_Y_STICKY_சாவிகள்_LATCH_TO_LOCK) {
                        filter->action.mods.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                /* fallthrough */
            நிலை ACTION_TYPE_MOD_LATCH:
            நிலை ACTION_TYPE_MOD_LOCK: {
                நிலையான மாறிலி எண்ணிக்கை xkb_action_flags unlock_mods_on_press
                    = ACTION_LOCK_CLEAR     /* SetMods & LatchMods: can unlock */
                    | ACTION_LOCK_NO_UNLOCK /* LockMods: cannot unlock */
                    | ACTION_EXPLICIT_UNLOCK_ON_PRESS;
                எனில் ((state->flags & STATE_UNLOCK_MODS_ON_PRESS) &&
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
                நிறுத்து;
            }
            நிலை ACTION_TYPE_GROUP_SET:
                எனில் (state->base.components.controls & CONTROL_STICKY_சாவிகள்) {
                    /* Convert group set action to a latch */
                    filter->action.type = ACTION_TYPE_GROUP_LATCH;
                    எனில் (state->a11y_flags & XKB_A11_Y_STICKY_சாவிகள்_LATCH_TO_LOCK) {
                        filter->action.group.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                நிறுத்து;
            நிலை ACTION_TYPE_GROUP_LOCK:
                எனில் ((filter->action.group.flags &
                     ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE) &&
                    (state->flags &
                     STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_சாவி) &&
                    /*
                     * FIXME: sticky keys may leave an unexpected modifier
                     * from the level 0 latched, so disable for now.
                     */
                    !(state->base.components.controls & CONTROL_STICKY_சாவிகள்))
                {
                    filter->action.group.flags |= ACTION_LOCK_ON_RELEASE;
                    /* Add base level action */
                    next.மொத்தம் = xkb_keymap_சாவி_get_actions_by_level(
                        state->base.keymap, சாவி, state->base.components.group,
                        0, &next.actions
                    );
                    ப்ரதிஜ்ஞா(next.மொத்தம் == 1);
                }
                நிறுத்து;
            நிலை ACTION_TYPE_PTR_MOVE:
            நிலை ACTION_TYPE_PTR_BUTTON:
            நிலை ACTION_TYPE_PTR_LOCK:
            நிலை ACTION_TYPE_PTR_DEFAULT:
                எனில் (!(state->base.components.controls & CONTROL_MOUSE_சாவிகள்)) {
                    /* Convert pointer actions to NoAction */
                    ப்ரதிஜ்ஞா(!filter_action_funcs[ACTION_TYPE_NONE].new);
                    filter->func = வெற்று;
                    தொடர்;
                }
                நிறுத்து;
            நிலை ACTION_TYPE_REDIRECT_சாவி:
                // FIXME: this is not efficient to resolve mods here each time
                filter->action.redirect.affect_mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.affect_mods
                );
                filter->action.redirect.mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.mods
                );
                நிறுத்து;
            இயல்பு: ;
            }
            filter->func = filter_action_funcs[filter->action.type].func;
            filter_action_funcs[filter->action.type].new(state, events, filter);
        }
    }
}

நிலையான வெறுமை
xkb_state_update_derived(கட்டமைப்பு xkb_state *state);

நிலையான உள்வரி வெறுமை
xkb_state_init(கட்டமைப்பு xkb_state * restrict state,
               கட்டமைப்பு xkb_keymap * restrict keymap,
               எண்ணிக்கை xkb_state_mode_internal mode)
{
    state->mode = mode;
    state->refcnt = 1;
    state->keymap = xkb_keymap_ref(keymap);
    state->out_of_range_group.policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

    /* Ensure that derived state is correctly initialized */
    xkb_state_update_derived(state);
}

நிலையான உள்வரி வெறுமை
xkb_client_state_init(கட்டமைப்பு xkb_client_state * restrict state,
                      கட்டமைப்பு xkb_keymap * restrict keymap,
                      எண்ணிக்கை xkb_state_mode_internal mode)
{
    xkb_state_init(&state->base, keymap, mode);
}

நிலையான உள்வரி வெறுமை
xkb_server_state_init(கட்டமைப்பு xkb_server_state * restrict state,
                      கட்டமைப்பு xkb_keymap * restrict keymap,
                      எண்ணிக்கை xkb_state_mode_internal mode,
                      எண்ணிக்கை xkb_a11y_flags a11y_affect,
                      எண்ணிக்கை xkb_a11y_flags a11y_flags)
{
    xkb_state_init(&state->base, keymap, mode);

    state->a11y_flags = a11y_flags;
    எனில் (keymap->format != XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !(a11y_affect & XKB_A11_Y_LATCH_SIMULTANEOUS_சாவிகள்)) {
             /* Keymap v2+: enable extension to XKB if not manually disabled */
             state->a11y_flags |= XKB_A11_Y_LATCH_SIMULTANEOUS_சாவிகள்;
    }
}

கட்டமைப்பு xkb_state *
xkb_state_new(கட்டமைப்பு xkb_keymap *keymap)
{
    கட்டமைப்பு xkb_server_state * மாறிலி state = calloc(1, அளவு(*state));
    எனில் (!state) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "Could not allocate a state object.\n");
        திரும்பு வெற்று;
    }

    xkb_server_state_init(state, keymap, LEGACY_MIXED_STATE, 0, 0);

    திரும்பு (கட்டமைப்பு xkb_state *)state;
}

கட்டமைப்பு xkb_state *
xkb_state_new_with_mode(கட்டமைப்பு xkb_keymap * restrict keymap,
                        எண்ணிக்கை xkb_state_mode mode,
                        எண்ணிக்கை xkb_status * restrict status)
{
    தேர்வு (mode) {
    நிலை XKB_STATE_MODE_CLIENT:
    நிலை XKB_STATE_MODE_SERVER_QUERY: {
        கட்டமைப்பு xkb_client_state * மாறிலி state = calloc(1, அளவு(*state));
        எனில் (!state)
            நிறுத்து;

        static_assert((unsigned)XKB_STATE_MODE_CLIENT ==
                      (unsigned)CLIENT_STATE, "");
        static_assert((unsigned)XKB_STATE_MODE_SERVER_QUERY ==
                      (unsigned)SERVER_COMPANION, "");
        xkb_client_state_init(state, keymap, (எண்ணிக்கை xkb_state_mode_internal)mode);

        எனில் (status)
            *status = XKB_SUCCESS;

        திரும்பு (கட்டமைப்பு xkb_state *)state;
    }
    நிலை XKB_STATE_MODE_SERVER: {
        கட்டமைப்பு xkb_server_state * மாறிலி state = calloc(1, அளவு(*state));
        எனில் (!state)
            நிறுத்து;

        xkb_server_state_init(state, keymap, LEGACY_SERVER_STATE, 0, 0);

        எனில் (status)
            *status = XKB_SUCCESS;

        திரும்பு (கட்டமைப்பு xkb_state *)state;
    }
    இயல்பு:
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_STATE_MODE_,
                     "Unsupported state mode: %u\n", mode);
        எனில் (status)
            *status = XKB_ERROR_UNSUPPORTED_STATE_MODE;
        திரும்பு வெற்று;
    }

    log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                  "Could not allocate a state object.\n");
    எனில் (status)
        *status = XKB_ERROR_ALLOCATION_FAILURE;
    திரும்பு வெற்று;
}

கட்டமைப்பு xkb_state *
xkb_state_ref(கட்டமைப்பு xkb_state *state)
{
    ப்ரதிஜ்ஞா(state->refcnt > 0);
    state->refcnt++;
    திரும்பு state;
}

வெறுமை
xkb_state_reset(கட்டமைப்பு xkb_state *state)
{
    memset(&state->components, 0, அளவு(state->components));

    எனில் (state->mode < LEGACY_MIXED_STATE) {
        static_assert(அளவு(*state) == அளவு(கட்டமைப்பு xkb_client_state),
                      "Not a mere wrapper");
        /* Nothing else to reset */
    } இல்லையெனில் {
        கட்டமைப்பு xkb_server_state *state_ = (கட்டமைப்பு xkb_server_state *)state;
        மாறிலி கட்டமைப்பு xkb_server_state prev = *state_;
        memset(state_, 0, அளவு(*state_));

        state_->base = prev.base;
        state_->a11y_flags = prev.a11y_flags;
        state_->filters = prev.filters;
        darray_size(state_->filters) = 0;
    }
}

நிலையான உள்வரி வெறுமை
xkb_state_destroy(கட்டமைப்பு xkb_state *state)
{
    xkb_keymap_unref(state->keymap);
    எனில் (state->mode > SERVER_COMPANION)
        darray_free(((கட்டமைப்பு xkb_server_state *)state)->filters);
}

வெறுமை
xkb_state_unref(கட்டமைப்பு xkb_state *state)
{
    ப்ரதிஜ்ஞா(!state || state->refcnt > 0);
    எனில் (!state || --state->refcnt > 0)
        திரும்பு;

    xkb_state_destroy(state);

    தேர்வு (state->mode) {
    நிலை CLIENT_STATE:
    நிலை SERVER_COMPANION:
        விடுவி((கட்டமைப்பு xkb_client_state *)state);
        நிறுத்து;
    நிலை LEGACY_MIXED_STATE:
    நிலை LEGACY_SERVER_STATE:
        விடுவி((கட்டமைப்பு xkb_server_state *)state);
        நிறுத்து;
    நிலை SERVER_STATE: /* handled by xkb_machine_unref() */
    இயல்பு:
        PANIC_UNREACHABLE();
    }
}

கட்டமைப்பு xkb_keymap *
xkb_state_get_keymap(கட்டமைப்பு xkb_state *state)
{
    திரும்பு state->keymap;
}

/**
 * Update the LED state to match the rest of the xkb_state.
 */
நிலையான வெறுமை
xkb_state_led_update_all(கட்டமைப்பு xkb_state *state)
{
    xkb_led_index_t idx;
    மாறிலி கட்டமைப்பு xkb_led *led;

    state->components.leds = 0;

    xkb_leds_enumerate(idx, led, state->keymap) {

        எனில் (led->which_mods != 0 && led->mods.mask != 0) {
            xkb_mod_mask_t mod_mask = 0;
            எனில் (led->which_mods & XKB_STATE_MODS_EFFECTIVE)
                mod_mask |= state->components.mods;
            எனில் (led->which_mods & XKB_STATE_MODS_DEPRESSED)
                mod_mask |= state->components.base_mods;
            எனில் (led->which_mods & XKB_STATE_MODS_LATCHED)
                mod_mask |= state->components.latched_mods;
            எனில் (led->which_mods & XKB_STATE_MODS_LOCKED)
                mod_mask |= state->components.locked_mods;

            எனில் (led->mods.mask & mod_mask) {
                state->components.leds |= (UINT32_C(1) << idx);
                தொடர்;
            }
        }

        எனில் (led->which_groups != 0) {
            எனில் (likely(led->groups) != 0) {
                xkb_layout_mask_t group_mask = 0;
                /* Effective and locked groups have been brought into range */
                ப்ரதிஜ்ஞா(state->components.group < XKB_MAX_GROUPS);
                ப்ரதிஜ்ஞா(state->components.locked_group >= 0 &&
                       state->components.locked_group < XKB_MAX_GROUPS);
                /* Effective and locked groups are used as mask */
                எனில் (led->which_groups & XKB_STATE_LAYOUT_EFFECTIVE)
                    group_mask |= (UINT32_C(1) << state->components.group);
                எனில் (led->which_groups & XKB_STATE_LAYOUT_LOCKED)
                    group_mask |= (UINT32_C(1) << state->components.locked_group);
                /* Base and latched groups only have to be non-zero */
                எனில் ((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                    state->components.base_group != 0)
                    group_mask |= led->groups;
                எனில் ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                    state->components.latched_group != 0)
                    group_mask |= led->groups;

                எனில் (led->groups & group_mask) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    தொடர்;
                }
            } இல்லையெனில் {
                /* Special case for Base and latched groups */
                எனில் (((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                     state->components.base_group == 0) ||
                    ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                     state->components.latched_group == 0)) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    தொடர்;
                }
            }
        }

        எனில் (led->ctrls & state->components.controls) {
            state->components.leds |= (UINT32_C(1) << idx);
            தொடர்;
        }
    }
}

/**
 * Calculates the derived state (effective mods/group and LEDs) from an
 * up-to-date xkb_state.
 */
நிலையான வெறுமை
xkb_state_update_derived(கட்டமைப்பு xkb_state *state)
{
    xkb_layout_index_t wrapped;

    state->components.mods = (state->components.base_mods |
                              state->components.latched_mods |
                              state->components.locked_mods);

    /* Lock group must be adjusted, but not base nor latched groups */
    wrapped = XkbWrapGroupIntoRange(state->components.locked_group,
                                    state->keymap->இலக்கம்_groups,
                                    state->out_of_range_group.policy,
                                    state->out_of_range_group.redirect_group);
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->components.locked_group =
        (int32_t) (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    /* Effective group must be adjusted */
    wrapped = XkbWrapGroupIntoRange(state->components.base_group +
                                    state->components.latched_group +
                                    state->components.locked_group,
                                    state->keymap->இலக்கம்_groups,
                                    state->out_of_range_group.policy,
                                    state->out_of_range_group.redirect_group);
    state->components.group =
        (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    xkb_state_led_update_all(state);
}

நிலையான வெறுமை
state_update_enabled_controls(கட்டமைப்பு xkb_server_state *state,
                              எண்ணிக்கை xkb_keyboard_control_flags affect_controls,
                              எண்ணிக்கை xkb_keyboard_control_flags controls,
                              கட்டமைப்பு xkb_events *events)
{
    மாறிலி bool had_sticky_சாவிகள் = state->base.components.controls
                               & CONTROL_STICKY_சாவிகள்;

    /*
     * Enable using the public API with the all the Control values, except
     * the internal ones, if any.
     */
    affect_controls =
        ( affect_controls
        & (எண்ணிக்கை xkb_keyboard_control_flags)CONTROL_ALL_BOOLEAN );
    state->base.components.controls &=
        (எண்ணிக்கை xkb_action_controls)~affect_controls;
    state->base.components.controls |=
        (எண்ணிக்கை xkb_action_controls)(controls & affect_controls);

    எனில் (had_sticky_சாவிகள் &&
        !(state->base.components.controls & CONTROL_STICKY_சாவிகள்)) {
        /* Sticky keys were disabled: clear all locks and latches */
        clear_all_latches_and_locks(state, events);
    }

    xkb_state_update_derived(&state->base);
}

/**
 * Given a particular key event, updates the state structure to reflect the
 * new modifiers.
 */
எண்ணிக்கை xkb_state_component
xkb_state_update_சாவி(கட்டமைப்பு xkb_state *base_state, xkb_keycode_t kc,
                     எண்ணிக்கை xkb_சாவி_direction direction)
{
    /* Guard against client-only state */
    ப்ரதிஜ்ஞா(base_state->mode > SERVER_COMPANION);
    எனில் (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        திரும்பு 0;
    }

    கட்டமைப்பு xkb_server_state * மாறிலி state =
        (கட்டமைப்பு xkb_server_state *)base_state;

    மாறிலி கட்டமைப்பு xkb_சாவி* மாறிலி சாவி = Xkb_சாவி(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    எனில் (!சாவி || (direction == XKB_சாவி_REPEATED && !சாவி->repeats))
        திரும்பு 0;

    எனில் (direction == XKB_சாவி_DOWN &&
        (state->base.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_சாவிகள்) &&
        (state->a11y_flags & XKB_A11_Y_STICKY_சாவிகள்_NO_SIMULTANEOUS_சாவிகள்) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_சாவிகள்,
                                      0, வெற்று);
    }

    மாறிலி கட்டமைப்பு state_components prev_components = state->base.components;

    state->set_mods = 0;
    state->clear_mods = 0;

    xkb_filter_apply_all(state, வெற்று, சாவி, direction);

    xkb_mod_index_t i;
    xkb_mod_mask_t bit;
    க்கு (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        எனில் (state->set_mods & bit) {
            state->mod_சாவி_மொத்தம்[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    க்கு (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        எனில் (state->clear_mods & bit) {
            state->mod_சாவி_மொத்தம்[i]--;
            எனில் (state->mod_சாவி_மொத்தம்[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_சாவி_மொத்தம்[i] = 0;
            }
            state->clear_mods &= ~bit;
        }
    }

    xkb_state_update_derived(&state->base);

    திரும்பு get_state_component_changes(&prev_components, &state->base.components);
}

/* We need fake keys for `update_latch_modifiers` and `update_latch_group`.
 * These keys must have at least one level in order to break latches. We need 2
 * keys with specific actions in order to update group/mod latches without
 * affecting each other. */
நிலையான கட்டமைப்பு xkb_சாவி_type_entry synthetic_சாவி_level_entry = { 0 };
நிலையான கட்டமைப்பு xkb_சாவி_type synthetic_சாவி_type = {
     .இலக்கம்_entries = 1,
     .இலக்கம்_levels = 1,
     .entries = &synthetic_சாவி_level_entry
};
நிலையான மாறிலி கட்டமைப்பு xkb_சாவி synthetic_சாவி = { 0 };

/* Transcription from xserver: XkbLatchModifiers */
நிலையான வெறுமை
update_latch_modifiers(கட்டமைப்பு xkb_server_state *state,
                       கட்டமைப்பு xkb_events *events,
                       xkb_mod_mask_t mask, xkb_mod_mask_t latches)
{
    /* Clear affected latched modifiers */
    மாறிலி xkb_mod_mask_t clear = mask & ~latches;
    state->base.components.latched_mods &= ~clear;

    /* Clear any pending latch to locks using ad hoc action:
     * only affect corresponding modifier latches and no group latch. */
    கட்டமைப்பு xkb_level synthetic_சாவி_level_break_mod_latch = {
        .இலக்கம்_syms = 0,
        .இலக்கம்_actions = 1,
        .upper = XKB_சாவி_No_குறியீடு,
        .s.sym = XKB_சாவி_No_குறியீடு,
        .a.action.internal = {
            .type = ACTION_TYPE_INTERNAL,
            .flags = INTERNAL_BREAKS_MOD_LATCH,
            .clear_latched_mods = clear
        }
    };
    கட்டமைப்பு xkb_group synthetic_சாவி_group_break_mod_latch = {
        .type = &synthetic_சாவி_type,
        .levels = &synthetic_சாவி_level_break_mod_latch
    };
    மாறிலி கட்டமைப்பு xkb_சாவி synthetic_சாவி_break_mod_latch = {
        .இலக்கம்_groups = 1,
        .groups = &synthetic_சாவி_group_break_mod_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_சாவி_break_mod_latch, XKB_சாவி_DOWN);

    /* Finally set the latched mods by simulate tapping a key with the
     * corresponding action */
    மாறிலி கட்டமைப்பு xkb_சாவி* மாறிலி சாவி = &synthetic_சாவி;
    மாறிலி ஒன்றிப்பு xkb_action latch_mods = {
        .mods = {
            .type = ACTION_TYPE_MOD_LATCH,
            .mods = { .mask = mask & latches },
            .flags = 0,
        },
    };
    கட்டமைப்பு xkb_filter* மாறிலி filter = xkb_filter_new(state);
    filter->சாவி = சாவி;
    filter->func = xkb_filter_mod_latch_func;
    filter->action = latch_mods;
    xkb_filter_mod_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_mod_latch_func(state, events, filter, சாவி, XKB_சாவி_UP);
}

/* Transcription from xserver: XkbLatchGroup */
நிலையான வெறுமை
update_latch_group(கட்டமைப்பு xkb_server_state *state,
                   கட்டமைப்பு xkb_events *events, int32_t group)
{
    /* Clear any pending latch to locks. */
    நிலையான கட்டமைப்பு xkb_level synthetic_சாவி_level_break_group_latch = {
        .இலக்கம்_syms = 0,
        .இலக்கம்_actions = 1,
        .upper = XKB_சாவி_No_குறியீடு,
        .s.sym = XKB_சாவி_No_குறியீடு,
        .a.action.internal = {
            .type = ACTION_TYPE_INTERNAL,
            .flags = INTERNAL_BREAKS_GROUP_LATCH,
            .clear_latched_mods = 0
        }
    };
    நிலையான கட்டமைப்பு xkb_group synthetic_சாவி_group_break_group_latch = {
        .type = &synthetic_சாவி_type,
        .levels = &synthetic_சாவி_level_break_group_latch
    };
    நிலையான மாறிலி கட்டமைப்பு xkb_சாவி synthetic_சாவி_break_group_latch = {
        .இலக்கம்_groups = 1,
        .groups = &synthetic_சாவி_group_break_group_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_சாவி_break_group_latch, XKB_சாவி_DOWN);

    /* Simulate tapping a key with a group latch action, but in isolation: i.e.
     * without affecting the other filters. */
    மாறிலி கட்டமைப்பு xkb_சாவி* மாறிலி சாவி = &synthetic_சாவி;
    மாறிலி ஒன்றிப்பு xkb_action latch_group = {
        .group = {
            .type = ACTION_TYPE_GROUP_LATCH,
            .flags = ACTION_ABSOLUTE_SWITCH,
            .group = group,
        },
    };
    கட்டமைப்பு xkb_filter* மாறிலி filter = xkb_filter_new(state);
    filter->சாவி = சாவி;
    filter->func = xkb_filter_group_latch_func;
    filter->action = latch_group;
    xkb_filter_group_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_group_latch_func(state, events, filter, சாவி, XKB_சாவி_UP);
}

/**
 * Compute the resolved effective mask of an arbitrary input.
 *
 * Contrary to `mod_mask_get_effective`, it resolves only modifiers not present
 * in the canonical mask, so that it enables `xkb_state_serialize_mods` to
 * round trip via `xkb_state_update_mask`.
 */
நிலையான உள்வரி xkb_mod_mask_t
resolve_to_canonical_mods(கட்டமைப்பு xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    திரும்பு
        /*
         * Keep canonical modifier mask.
         * It contains either real modifiers or canonical virtual modifiers.
         */
        (mods & keymap->canonical_state_mask) |
        /* Resolve other modifiers */
        mod_mask_get_effective(keymap,
                               mods & ~keymap->canonical_state_mask);
}

நிலையான வெறுமை
state_update_latched_locked(
    கட்டமைப்பு xkb_server_state * restrict state,
    மாறிலி கட்டமைப்பு xkb_state_components_update * restrict update,
    கட்டமைப்பு xkb_events * restrict events
)
{
    /* Update locks */
    மாறிலி xkb_mod_mask_t affect_locked_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_locked_mods);
    எனில் ((update->components & XKB_STATE_MODS_LOCKED) &&
        affect_locked_mods) {
        மாறிலி xkb_mod_mask_t locked_mods =
            resolve_to_canonical_mods(state->base.keymap, update->locked_mods);
        state->base.components.locked_mods &= ~affect_locked_mods;
        state->base.components.locked_mods |= locked_mods & affect_locked_mods;
    }
    எனில் (update->components & XKB_STATE_LAYOUT_LOCKED) {
        state->base.components.locked_group = update->locked_layout;
    }

    /* Update latches */
    மாறிலி xkb_mod_mask_t affect_latched_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_latched_mods);
    எனில் ((update->components & XKB_STATE_MODS_LATCHED) &&
        affect_latched_mods) {
        மாறிலி xkb_mod_mask_t latched_mods =
            resolve_to_canonical_mods(state->base.keymap, update->latched_mods);
        update_latch_modifiers(state, events, affect_latched_mods, latched_mods);
    }
    எனில் (update->components & XKB_STATE_LAYOUT_LATCHED) {
        update_latch_group(state, events, update->latched_layout);
    }
}

எண்ணிக்கை xkb_state_component
xkb_state_update_latched_locked(கட்டமைப்பு xkb_state *base_state,
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
    ப்ரதிஜ்ஞா(base_state->mode > SERVER_COMPANION);
    எனில் (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        திரும்பு 0;
    }

    கட்டமைப்பு xkb_server_state * மாறிலி state =
        (கட்டமைப்பு xkb_server_state *)base_state;

    மாறிலி கட்டமைப்பு state_components previous_components = state->base.components;
    மாறிலி எண்ணிக்கை xkb_state_component components
        = ((affect_latched_mods || latched_mods) ? XKB_STATE_MODS_LATCHED : 0)
        | ((affect_locked_mods || locked_mods) ? XKB_STATE_MODS_LOCKED : 0)
        | (affect_latched_layout ? XKB_STATE_LAYOUT_LATCHED : 0)
        | (affect_locked_layout ? XKB_STATE_LAYOUT_LOCKED : 0);
    மாறிலி கட்டமைப்பு xkb_state_components_update update = {
        .size = அளவு(update),
        .components = components,
        .affect_latched_mods = affect_latched_mods,
        .latched_mods = latched_mods,
        .latched_layout = latched_layout,
        .affect_locked_mods = affect_locked_mods,
        .locked_mods = locked_mods,
        .locked_layout = locked_layout
    };
    state_update_latched_locked(state, &update, வெற்று);
    xkb_state_update_derived(&state->base);
    திரும்பு get_state_component_changes(&previous_components,
                                       &state->base.components);
}

நிலையான உள்வரி வெறுமை
clear_all_latches_and_locks(கட்டமைப்பு xkb_server_state *state,
                            கட்டமைப்பு xkb_events *events)
{
    நிலையான மாறிலி எண்ணிக்கை xkb_state_component components
        = XKB_STATE_MODS_LATCHED | XKB_STATE_MODS_LOCKED
        | XKB_STATE_LAYOUT_LATCHED | XKB_STATE_LAYOUT_LOCKED;
    மாறிலி கட்டமைப்பு xkb_state_components_update update = {
        .size = அளவு(update),
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

நிலையான எண்ணிக்கை xkb_status
state_update_layout_policy(கட்டமைப்பு xkb_server_state *state,
                           மாறிலி கட்டமைப்பு xkb_layout_policy_update *update)
{
    எனில் (xkb_feature_supported(XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY,
                              update->policy)) {
        எனில் (update->policy == XKB_LAYOUT_OUT_OF_RANGE_REDIRECT) {
            எனில் (update->redirect < state->base.keymap->இலக்கம்_groups) {
                state->base.out_of_range_group.redirect_group =
                    update->redirect;
            } இல்லையெனில் {
                log_err(state->base.keymap->ctx,
                        XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Layout policy: "
                        "unsupported layout index %"PRIu32" > %"PRIu32"\n",
                        update->redirect + 1, state->base.keymap->இலக்கம்_groups);
                திரும்பு XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
            }
        }
        state->base.out_of_range_group.policy = update->policy;
        திரும்பு XKB_SUCCESS;
    } இல்லையெனில் {
        log_err(state->base.keymap->ctx,
                XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY_,
                "Unsupported layout policy: %d\n", update->policy);
        திரும்பு XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY;
    }
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
நிலையான எண்ணிக்கை xkb_status
check_state_update_abi_(கட்டமைப்பு xkb_context * restrict ctx,
                        மாறிலி எழுத்து * restrict func,
                        மாறிலி கட்டமைப்பு xkb_synthetic_update * restrict update)
{
    எண்ணிக்கை xkb_status status = XKB_SUCCESS;
    எனில் ((status = xkb_check_state_abi(update)) ||
        (update->reserved0 != 0 &&
         (status = XKB_ERROR_ABI_FORWARD_COMPAT)) ||
        (update->components &&
         (status = xkb_check_state_abi(update->components))) ||
        (update->layout_policy &&
         (status = xkb_check_state_abi(update->layout_policy)))) {
        xkb_log_abi_error(ctx, func, status);
    }
    திரும்பு status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

#define check_state_update_abi(ctx, update) \
    check_state_update_abi_(ctx, __func__, update)

எண்ணிக்கை xkb_status
xkb_state_update_synthetic(கட்டமைப்பு xkb_state * base_state,
                           மாறிலி கட்டமைப்பு xkb_synthetic_update * update,
                           எண்ணிக்கை xkb_state_component *changed)
{
    /* Guard against client-only state */
    எனில் (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        திரும்பு XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    கட்டமைப்பு xkb_server_state * மாறிலி state =
        (கட்டமைப்பு xkb_server_state *)base_state;

    /* Check ABI compatibility */
    எண்ணிக்கை xkb_status status = check_state_update_abi(state->base.keymap->ctx,
                                                    update);
    எனில் (status != XKB_SUCCESS)
        திரும்பு status;

    மாறிலி கட்டமைப்பு state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    /* Update parametrized controls first */
    எனில் (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        எனில் (status != XKB_SUCCESS)
            திரும்பு status;
    }

    எனில் (update->components) {
        மாறிலி கட்டமைப்பு xkb_state_components_update * மாறிலி components =
            update->components;
        /* Update boolean controls first */
        எனில் (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, வெற்று);
        }

        state_update_latched_locked(state, components, வெற்று);
    }

    xkb_state_update_derived(&state->base);

    எனில் (changed) {
        *changed = get_state_component_changes(&previous_components,
                                               &state->base.components);
    }
    திரும்பு XKB_SUCCESS;
}

/**
 * Updates the state from a set of explicit masks as gained from
 * xkb_state_serialize_mods and xkb_state_serialize_groups.  As noted in the
 * documentation for these functions in xkbcommon.h, this round-trip is
 * lossy, and should only be used to update a slave state mirroring the
 * master, e.g. in a client/server window system.
 */
எண்ணிக்கை xkb_state_component
xkb_state_update_mask(கட்டமைப்பு xkb_state *base_state,
                      xkb_mod_mask_t base_mods,
                      xkb_mod_mask_t latched_mods,
                      xkb_mod_mask_t locked_mods,
                      xkb_layout_index_t base_group,
                      xkb_layout_index_t latched_group,
                      xkb_layout_index_t locked_group)
{
    /* Guard against server-only and server companion state */
    ப்ரதிஜ்ஞா(base_state->mode == CLIENT_STATE ||
           base_state->mode == LEGACY_MIXED_STATE);
    எனில் (base_state->mode != CLIENT_STATE &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        திரும்பு 0;
    }

    கட்டமைப்பு xkb_client_state * மாறிலி state =
        (கட்டமைப்பு xkb_client_state *)base_state;

    மாறிலி கட்டமைப்பு state_components prev_components = state->base.components;

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

    திரும்பு get_state_component_changes(&prev_components, &state->base.components);
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Lock_Modifier
 */
நிலையான bool
should_do_caps_transformation(கட்டமைப்பு xkb_state *state, xkb_keycode_t kc)
{
    திரும்பு
        xkb_state_mod_index_is_active(state, XKB_MOD_INDEX_CAPS,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_index_is_consumed(state, kc, XKB_MOD_INDEX_CAPS) == 0;
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Control_Modifier
 */
நிலையான bool
should_do_ctrl_transformation(கட்டமைப்பு xkb_state *state, xkb_keycode_t kc)
{
    திரும்பு
        xkb_state_mod_index_is_active(state, XKB_MOD_INDEX_CTRL,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_index_is_consumed(state, kc, XKB_MOD_INDEX_CTRL) == 0;
}

/**
 * Provides the symbols to use for the given key and state.  Returns the
 * number of symbols pointed to in syms_out.
 */
முழு
xkb_state_சாவி_get_syms(கட்டமைப்பு xkb_state *state, xkb_keycode_t kc,
                       மாறிலி xkb_keysym_t **syms_out)
{
    மாறிலி xkb_layout_index_t layout = xkb_state_சாவி_get_layout(state, kc);
    எனில் (layout == XKB_LAYOUT_INVALID)
        செல் err;

    மாறிலி xkb_level_index_t level = xkb_state_சாவி_get_level(state, kc, layout);
    எனில் (level == XKB_LEVEL_INVALID)
        செல் err;

    மாறிலி கட்டமைப்பு xkb_சாவி* மாறிலி சாவி = Xkb_சாவி(state->keymap, kc);

    எனில் (!சாவி)
        செல் err;

    மாறிலி கட்டமைப்பு xkb_level* மாறிலி leveli =
        xkb_keymap_சாவி_get_level(state->keymap, சாவி, layout, level);

    எனில் (!leveli)
        செல் err;

    மாறிலி xkb_keysym_மொத்தம்_t இலக்கம்_syms = leveli->இலக்கம்_syms;
    எனில் (இலக்கம்_syms == 0)
        செல் err;

    எனில் (should_do_caps_transformation(state, kc)) {
        /* Only simple capitalization rules: keysyms count is unchanged. */
        எனில் (இலக்கம்_syms > 1) {
            *syms_out = (leveli->has_upper)
                      ? leveli->s.syms + இலக்கம்_syms
                      : leveli->s.syms;
        } இல்லையெனில் {
            *syms_out = &leveli->upper;
        }
    } இல்லையெனில் {
        *syms_out = (இலக்கம்_syms > 1)
                  ? leveli->s.syms
                  : &leveli->s.sym;
    }
    திரும்பு (முழு) இலக்கம்_syms;

err:
    *syms_out = வெற்று;
    திரும்பு 0;
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
நிலையான எழுத்து
XkbToControl(எழுத்து ch)
{
    எழுத்து c = ch;

    எனில் ((c >= '@' && c < '\177') || c == ' ')
        c &= 0x1F;
    இல்லையெனில் எனில் (c == '2')
        c = '\000';
    இல்லையெனில் எனில் (c >= '3' && c <= '7')
        c -= ('3' - '\033');
    இல்லையெனில் எனில் (c == '8')
        c = '\177';
    இல்லையெனில் எனில் (c == '/')
        c = '_' & 0x1F;
    திரும்பு c;
}

/**
 * Provides either exactly one symbol, or XKB_KEY_NoSymbol.
 */
xkb_keysym_t
xkb_state_சாவி_get_one_sym(கட்டமைப்பு xkb_state *state, xkb_keycode_t kc)
{
    மாறிலி xkb_keysym_t *syms = வெற்று;
    மாறிலி முழு இலக்கம்_syms = xkb_state_சாவி_get_syms(state, kc, &syms);

    எனில் (இலக்கம்_syms != 1)
        திரும்பு XKB_சாவி_No_குறியீடு;
    இல்லையெனில்
        திரும்பு syms[0];
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
நிலையான xkb_keysym_t
get_one_sym_for_string(கட்டமைப்பு xkb_state *state, xkb_keycode_t kc)
{
    மாறிலி xkb_layout_index_t layout = xkb_state_சாவி_get_layout(state, kc);
    மாறிலி xkb_layout_index_t இலக்கம்_layouts =
        xkb_keymap_இலக்கம்_layouts_for_சாவி(state->keymap, kc);
    xkb_level_index_t level = xkb_state_சாவி_get_level(state, kc, layout);
    எனில் (layout == XKB_LAYOUT_INVALID || இலக்கம்_layouts == 0 ||
        level == XKB_LEVEL_INVALID)
        திரும்பு XKB_சாவி_No_குறியீடு;

    மாறிலி xkb_keysym_t *syms = வெற்று;
    முழு nsyms = xkb_keymap_சாவி_get_syms_by_level(state->keymap, kc,
                                                 layout, level, &syms);
    எனில் (nsyms != 1)
        திரும்பு XKB_சாவி_No_குறியீடு;
    xkb_keysym_t sym = syms[0];

    எனில் (should_do_ctrl_transformation(state, kc) && sym > 127u) {
        க்கு (xkb_layout_index_t i = 0; i < இலக்கம்_layouts; i++) {
            level = xkb_state_சாவி_get_level(state, kc, i);
            எனில் (level == XKB_LEVEL_INVALID)
                தொடர்;

            nsyms = xkb_keymap_சாவி_get_syms_by_level(state->keymap, kc,
                                                     i, level, &syms);
            எனில் (nsyms == 1 && syms[0] <= 127u) {
                sym = syms[0];
                நிறுத்து;
            }
        }
    }

    எனில் (should_do_caps_transformation(state, kc)) {
        sym = xkb_keysym_to_upper(sym);
    }

    திரும்பு sym;
}

முழு
xkb_state_சாவி_get_utf8(கட்டமைப்பு xkb_state *state, xkb_keycode_t kc,
                       எழுத்து *buffer, size_t size)
{
    முழு nsyms;
    மாறிலி xkb_keysym_t *syms = வெற்று;
    மாறிலி xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    எனில் (sym != XKB_சாவி_No_குறியீடு) {
        nsyms = 1; syms = &sym;
    }
    இல்லையெனில் {
        nsyms = xkb_state_சாவி_get_syms(state, kc, &syms);
    }

    /* Make sure not to truncate in the middle of a UTF-8 sequence. */
    முழு offset = 0;
    எழுத்து tmp[XKB_KEYSYM_UTF8_MAX_SIZE];
    க்கு (முழு i = 0; i < nsyms; i++) {
        முழு ret = xkb_keysym_to_utf8(syms[i], tmp, அளவு(tmp));
        எனில் (ret <= 0)
            செல் err_bad;

        ret--;
        எனில் ((size_t)offset + (size_t)ret <= size)
            memcpy(buffer + offset, tmp, (size_t)ret);
        offset += ret;
    }

    எனில் ((size_t)offset >= size)
        செல் err_trunc;
    buffer[offset] = '\0';

    எனில் (!is_valid_utf8(buffer, (size_t)offset))
        செல் err_bad;

    எனில் (offset == 1 && (unsigned முழு) buffer[0] <= 127u &&
        should_do_ctrl_transformation(state, kc))
        buffer[0] = XkbToControl(buffer[0]);

    திரும்பு offset;

err_trunc:
    எனில் (size > 0)
        buffer[size - 1] = '\0';
    திரும்பு offset;

err_bad:
    எனில் (size > 0)
        buffer[0] = '\0';
    திரும்பு 0;
}

uint32_t
xkb_state_சாவி_get_utf32(கட்டமைப்பு xkb_state *state, xkb_keycode_t kc)
{
    மாறிலி xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    uint32_t cp = xkb_keysym_to_utf32(sym);

    எனில் (cp <= 127u && should_do_ctrl_transformation(state, kc))
        cp = (uint32_t) XkbToControl((எழுத்து) cp);

    திரும்பு cp;
}

/**
 * Serializes the requested modifier state into an xkb_mod_mask_t, with all
 * the same disclaimers as in xkb_state_update_mask.
 */
நிலையான உள்வரி xkb_mod_mask_t
serialize_mods(மாறிலி கட்டமைப்பு state_components *components,
               எண்ணிக்கை xkb_state_component type)
{
    xkb_mod_mask_t ret = 0;

    எனில் (type & XKB_STATE_MODS_EFFECTIVE)
        திரும்பு components->mods;

    எனில் (type & XKB_STATE_MODS_DEPRESSED)
        ret |= components->base_mods;
    எனில் (type & XKB_STATE_MODS_LATCHED)
        ret |= components->latched_mods;
    எனில் (type & XKB_STATE_MODS_LOCKED)
        ret |= components->locked_mods;

    திரும்பு ret;
}

xkb_mod_mask_t
xkb_state_serialize_mods(கட்டமைப்பு xkb_state *state,
                         எண்ணிக்கை xkb_state_component type)
{
    திரும்பு serialize_mods(&state->components, type);
}

/**
 * Serialises the requested group state, with all the same disclaimers as
 * in xkb_state_update_mask.
 */
நிலையான உள்வரி xkb_layout_index_t
serialize_layout(மாறிலி கட்டமைப்பு state_components *components,
                 எண்ணிக்கை xkb_state_component type)
{
    எனில் (type & XKB_STATE_LAYOUT_EFFECTIVE)
        திரும்பு components->group;

    int32_t ret = 0;
    எனில் (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret += components->base_group;
    எனில் (type & XKB_STATE_LAYOUT_LATCHED)
        ret += components->latched_group;
    எனில் (type & XKB_STATE_LAYOUT_LOCKED)
        ret += components->locked_group;

    திரும்பு (xkb_layout_index_t)ret;
}

xkb_layout_index_t
xkb_state_serialize_layout(கட்டமைப்பு xkb_state *state,
                           எண்ணிக்கை xkb_state_component type)
{
    திரும்பு serialize_layout(&state->components, type);
}

xkb_led_mask_t
xkb_state_serialize_leds(மாறிலி கட்டமைப்பு xkb_state *state,
                         எண்ணிக்கை xkb_state_component type)
{
    திரும்பு (type & XKB_STATE_LEDS)
        ? state->components.leds
        : 0;
}

நிலையான உள்வரி எண்ணிக்கை xkb_keyboard_control_flags
serialize_controls(மாறிலி கட்டமைப்பு state_components *components,
                   எண்ணிக்கை xkb_state_component type)
{
    திரும்பு (type & XKB_STATE_CONTROLS_EFFECTIVE)
        /*
         * Enable using the public API with the all the Controls values, except
         * the internal ones, if any.
         */
        ? (எண்ணிக்கை xkb_keyboard_control_flags)
          (components->controls & CONTROL_ALL_BOOLEAN)
        : 0;
}

எண்ணிக்கை xkb_keyboard_control_flags
xkb_state_serialize_controls(மாறிலி கட்டமைப்பு xkb_state *state,
                             எண்ணிக்கை xkb_state_component type)
{
    திரும்பு serialize_controls(&state->components, type);
}

/**
 * Gets a modifier mask and returns the resolved effective mask; this
 * is needed because some modifiers can also map to other modifiers, e.g.
 * the "NumLock" modifier usually also sets the "Mod2" modifier.
 */
xkb_mod_mask_t
mod_mask_get_effective(கட்டமைப்பு xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    /* Initialize the effective mask with its corresponding real mods. */
    xkb_mod_mask_t mask = mods & MOD_REAL_MASK_ALL;

    /* Resolve the virtual modifiers */
    மாறிலி கட்டமைப்பு xkb_mod *mod;
    xkb_mod_index_t i;
    xkb_vmods_enumerate(i, mod, &keymap->mods)
        எனில் (mods & (UINT32_C(1) << i))
            mask |= mod->mapping;

    திரும்பு mask;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
முழு
xkb_state_mod_index_is_active(கட்டமைப்பு xkb_state *state,
                              xkb_mod_index_t idx,
                              எண்ணிக்கை xkb_state_component type)
{
    எனில் (unlikely(idx >= xkb_keymap_இலக்கம்_mods(state->keymap)))
        திரும்பு -1;

    மாறிலி xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    எனில் (!mapping) {
        /* Modifier not mapped */
        திரும்பு 0;
    }
    /* WARNING: this may overmatch for virtual modifiers */
    திரும்பு (xkb_state_serialize_mods(state, type) & mapping) == mapping;
}

எண்ணிக்கை {
    XKB_STATE_MATCH_FLAGS = XKB_STATE_MATCH_ANY
                          | XKB_STATE_MATCH_ALL
                          | XKB_STATE_MATCH_NON_EXCLUSIVE
};

/**
 * Helper function for xkb_state_mod_indices_are_active and
 * xkb_state_mod_names_are_active.
 */
நிலையான bool
match_mod_masks(கட்டமைப்பு xkb_state *state,
                எண்ணிக்கை xkb_state_component type,
                எண்ணிக்கை xkb_state_match match,
                xkb_mod_mask_t wanted)
{
    மாறிலி xkb_mod_mask_t active = xkb_state_serialize_mods(state, type);

    எனில் (!(match & XKB_STATE_MATCH_NON_EXCLUSIVE) && (active & ~wanted))
        திரும்பு false;

    எனில் (match & XKB_STATE_MATCH_ANY)
        திரும்பு active & wanted;

    /* Ensure all flags but XKB_STATE_MATCH_ALL are processed before */
    static_assert(XKB_STATE_MATCH_ALL ==
                  ((எண்ணிக்கை xkb_state_match) XKB_STATE_MATCH_FLAGS &
                   ~(XKB_STATE_MATCH_ANY | XKB_STATE_MATCH_NON_EXCLUSIVE)),
                  "");

    திரும்பு (active & wanted) == wanted;
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, or -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
முழு
xkb_state_mod_indices_are_active(கட்டமைப்பு xkb_state *state,
                                 எண்ணிக்கை xkb_state_component type,
                                 எண்ணிக்கை xkb_state_match match,
                                 ...)
{
    எனில் (match & ~((எண்ணிக்கை xkb_state_match) XKB_STATE_MATCH_FLAGS))
        திரும்பு -2;

    va_list ap;
    xkb_mod_mask_t wanted = 0;
    முழு ret = 0;
    மாறிலி xkb_mod_index_t இலக்கம்_mods = xkb_keymap_இலக்கம்_mods(state->keymap);

    va_start(ap, match);
    போது (1) {
        xkb_mod_index_t idx = va_arg(ap, xkb_mod_index_t);
        எனில் (idx == XKB_MOD_INVALID)
            நிறுத்து;
        எனில் (unlikely(idx >= இலக்கம்_mods)) {
            ret = -1;
            நிறுத்து;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_end(ap);

    எனில் (ret == -1)
        திரும்பு ret;

    எனில் (!wanted) {
        /* Modifiers not mapped */
        திரும்பு 0;
    }

    திரும்பு match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
முழு
xkb_state_mod_name_is_active(கட்டமைப்பு xkb_state *state, மாறிலி எழுத்து *name,
                             எண்ணிக்கை xkb_state_component type)
{
    மாறிலி xkb_mod_index_t idx = xkb_keymap_mod_get_index(state->keymap, name);

    எனில் (idx == XKB_MOD_INVALID)
        திரும்பு -1;

    திரும்பு xkb_state_mod_index_is_active(state, idx, type);
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ATTR_NULL_SENTINEL முழு
xkb_state_mod_names_are_active(கட்டமைப்பு xkb_state *state,
                               எண்ணிக்கை xkb_state_component type,
                               எண்ணிக்கை xkb_state_match match,
                               ...)
{
    எனில் (match & ~((எண்ணிக்கை xkb_state_match) XKB_STATE_MATCH_FLAGS))
        திரும்பு -2;

    va_list ap;
    xkb_mod_mask_t wanted = 0;
    முழு ret = 0;

    va_start(ap, match);
    போது (1) {
        மாறிலி எழுத்து *str = va_arg(ap, மாறிலி எழுத்து *);
        எனில் (str == வெற்று)
            நிறுத்து;
        மாறிலி xkb_mod_index_t idx = xkb_keymap_mod_get_index(state->keymap, str);
        எனில் (idx == XKB_MOD_INVALID) {
            ret = -1;
            நிறுத்து;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_end(ap);

    எனில் (ret == -1)
        திரும்பு ret;

    எனில் (!wanted) {
        /* Modifiers not mapped */
        திரும்பு 0;
    }

    திரும்பு match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given group is active with the specified type(s), 0 if
 * not, or -1 if the group is invalid.
 */
முழு
xkb_state_layout_index_is_active(கட்டமைப்பு xkb_state *state,
                                 xkb_layout_index_t idx,
                                 எண்ணிக்கை xkb_state_component type)
{
    எனில் (idx >= state->keymap->இலக்கம்_groups)
        திரும்பு -1;

    முழு ret = 0;
    எனில் (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ret |= (state->components.group == idx);
    எனில் (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret |= (state->components.base_group == (int32_t) idx);
    எனில் (type & XKB_STATE_LAYOUT_LATCHED)
        ret |= (state->components.latched_group == (int32_t) idx);
    எனில் (type & XKB_STATE_LAYOUT_LOCKED)
        ret |= (state->components.locked_group == (int32_t) idx);

    திரும்பு ret;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
முழு
xkb_state_layout_name_is_active(கட்டமைப்பு xkb_state *state, மாறிலி எழுத்து *name,
                                எண்ணிக்கை xkb_state_component type)
{
    மாறிலி xkb_layout_index_t idx =
        xkb_keymap_layout_get_index(state->keymap, name);

    எனில் (idx == XKB_LAYOUT_INVALID)
        திரும்பு -1;

    திரும்பு xkb_state_layout_index_is_active(state, idx, type);
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
முழு
xkb_state_led_index_is_active(கட்டமைப்பு xkb_state *state, xkb_led_index_t idx)
{
    எனில் (idx >= state->keymap->இலக்கம்_leds ||
        state->keymap->leds[idx].name == XKB_ATOM_NONE)
        திரும்பு -1;

    திரும்பு !!(state->components.leds & (UINT32_C(1) << idx));
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
முழு
xkb_state_led_name_is_active(கட்டமைப்பு xkb_state *state, மாறிலி எழுத்து *name)
{
    மாறிலி xkb_led_index_t idx = xkb_keymap_led_get_index(state->keymap, name);

    எனில் (idx == XKB_LED_INVALID)
        திரும்பு -1;

    திரும்பு xkb_state_led_index_is_active(state, idx);
}

/**
 * See:
 * - XkbTranslateKeyCode(3), mod_rtrn return value, from libX11.
 * - MyEnhancedXkbTranslateKeyCode(), a modification of the above, from GTK+.
 */
நிலையான xkb_mod_mask_t
சாவி_get_consumed(கட்டமைப்பு xkb_state *state, மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                 எண்ணிக்கை xkb_consumed_mode mode)
{
    மாறிலி xkb_layout_index_t group =
        xkb_state_சாவி_get_layout(state, சாவி->keycode);
    எனில் (group == XKB_LAYOUT_INVALID)
        திரும்பு 0;

    xkb_mod_mask_t preserve = 0;
    xkb_mod_mask_t consumed = 0;

    மாறிலி கட்டமைப்பு xkb_சாவி_type_entry* மாறிலி matching_entry =
        get_entry_for_சாவி_state(state, சாவி, group);
    எனில் (matching_entry)
        preserve = matching_entry->preserve.mask;

    மாறிலி கட்டமைப்பு xkb_சாவி_type* மாறிலி type = சாவி->groups[group].type;
    தேர்வு (mode) {
    நிலை XKB_CONSUMED_MODE_XKB:
        consumed = type->mods.mask;
        நிறுத்து;

    நிலை XKB_CONSUMED_MODE_GTK: {
        மாறிலி கட்டமைப்பு xkb_சாவி_type_entry* மாறிலி no_mods_entry =
            get_entry_for_mods(type, 0);
        மாறிலி xkb_level_index_t no_mods_leveli = no_mods_entry
                                               ? no_mods_entry->level
                                               : 0;
        மாறிலி கட்டமைப்பு xkb_level* மாறிலி no_mods_level =
            &சாவி->groups[group].levels[no_mods_leveli];

        க்கு (darray_size_t i = 0; i < type->இலக்கம்_entries; i++) {
            மாறிலி கட்டமைப்பு xkb_சாவி_type_entry* மாறிலி entry = &type->entries[i];
            எனில் (!entry_is_active(entry))
                தொடர்;

            மாறிலி கட்டமைப்பு xkb_level* மாறிலி level =
                &சாவி->groups[group].levels[entry->level];
            எனில் (XkbLevelsSameSyms(level, no_mods_level))
                தொடர்;

            எனில் (entry == matching_entry || one_bit_set(entry->mods.mask))
                consumed |= entry->mods.mask & ~entry->preserve.mask;
        }
        நிறுத்து;
    }
    }

    திரும்பு consumed & ~preserve;
}

முழு
xkb_state_mod_index_is_consumed2(கட்டமைப்பு xkb_state *state, xkb_keycode_t kc,
                                 xkb_mod_index_t idx,
                                 எண்ணிக்கை xkb_consumed_mode mode)
{
    மாறிலி கட்டமைப்பு xkb_சாவி* மாறிலி சாவி = Xkb_சாவி(state->keymap, kc);

    எனில் (unlikely(!சாவி || idx >= xkb_keymap_இலக்கம்_mods(state->keymap)))
        திரும்பு -1;

    மாறிலி xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    எனில் (!mapping) {
        /* Modifier not mapped */
        திரும்பு 0;
    }
    திரும்பு (mapping & சாவி_get_consumed(state, சாவி, mode)) == mapping;
}

முழு
xkb_state_mod_index_is_consumed(கட்டமைப்பு xkb_state *state, xkb_keycode_t kc,
                                xkb_mod_index_t idx)
{
    திரும்பு xkb_state_mod_index_is_consumed2(state, kc, idx,
                                            XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_mod_mask_remove_consumed(கட்டமைப்பு xkb_state *state, xkb_keycode_t kc,
                                   xkb_mod_mask_t mask)
{
    மாறிலி கட்டமைப்பு xkb_சாவி* மாறிலி சாவி = Xkb_சாவி(state->keymap, kc);

    எனில் (!சாவி)
        திரும்பு 0;

    திரும்பு resolve_to_canonical_mods(state->keymap, mask) &
           ~சாவி_get_consumed(state, சாவி, XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_சாவி_get_consumed_mods2(கட்டமைப்பு xkb_state *state, xkb_keycode_t kc,
                                 எண்ணிக்கை xkb_consumed_mode mode)
{
    தேர்வு (mode) {
    நிலை XKB_CONSUMED_MODE_XKB:
    நிலை XKB_CONSUMED_MODE_GTK:
        நிறுத்து;
    இயல்பு:
        log_err_func(state->keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "unrecognized consumed modifiers mode: %d\n", mode);
        திரும்பு 0;
    }

    மாறிலி கட்டமைப்பு xkb_சாவி* மாறிலி சாவி = Xkb_சாவி(state->keymap, kc);
    எனில் (!சாவி)
        திரும்பு 0;

    திரும்பு சாவி_get_consumed(state, சாவி, mode);
}

xkb_mod_mask_t
xkb_state_சாவி_get_consumed_mods(கட்டமைப்பு xkb_state *state, xkb_keycode_t kc)
{
    திரும்பு xkb_state_சாவி_get_consumed_mods2(state, kc, XKB_CONSUMED_MODE_XKB);
}

/*******************************************************************************
 * Server state machine API
 ******************************************************************************/

/** Entry to keep track of the state of an overlaid key */
கட்டமைப்பு xkb_overlaid_சாவி {
    /** The source key being overlaid */
    மாறிலி கட்டமைப்பு xkb_சாவி *old;
    /** The target key */
    மாறிலி கட்டமைப்பு xkb_சாவி *new;
    /** Counter of key currently down */
    முழு refcnt;
};

கட்டமைப்பு xkb_shortcuts_config_entry {
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
கட்டமைப்பு xkb_machine {
    /** Inherits from `xkb_server_state` */
    கட்டமைப்பு xkb_server_state base;

    /* Extra server-specific state data */

    /** Keyboard overlays handling */
    கட்டமைப்பு {
        /**
         * Current overlaid keys
         *
         * When a key is overlaid, it remained overlaid in this configuration
         * until totally released, i.e. every key press was matched with the
         * corresponding key release.
         */
        darray(கட்டமைப்பு xkb_overlaid_சாவி) சாவிகள்;
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
    கட்டமைப்பு machine_config {
        /** Modifiers remapping */
        கட்டமைப்பு machine_modifiers_config {
            /** Real modifier mask */
            xkb_mod_mask_t mask;
            /** Modifiers re-mappings */
            darray_size_t mappings_இலக்கம்;
            கட்டமைப்பு machine_mods_mapping {
                xkb_mod_mask_t source;
                xkb_mod_mask_t target;
            } * mappings ATTR_COUNTED_BY(mappings_இலக்கம்);
        } modifiers;

        /** Shortcuts tweak */
        கட்டமைப்பு machine_shortcuts_config {
            /** Substitution entries */
            கட்டமைப்பு xkb_shortcuts_config_entry *entries;
        } shortcuts;
    } config;

    /** Mouse keys handling */
    கட்டமைப்பு {
        /** Default pointer button */
        xkb_pointer_button_index_t default_button;
        /** Mask of the locked buttons */
        xkb_pointer_button_mask_t locked_buttons;
    } mouse;

    எண்ணிக்கை xkb_machine_flags flags;
};

வகைவரை darray(கட்டமைப்பு machine_mods_mapping) machine_mods_mappings;

கட்டமைப்பு xkb_machine_builder {
    கட்டமைப்பு xkb_keymap *keymap;

    /** Controls */
    கட்டமைப்பு {
        /** Accessibility flags */
        கட்டமைப்பு {
            எண்ணிக்கை xkb_a11y_flags affect_flags;
            எண்ணிக்கை xkb_a11y_flags flags;
        } a11y;
    } controls;

    /** Modifiers re-mapping */
    machine_mods_mappings mods;

    /** Shortcuts tweak */
    கட்டமைப்பு xkb_shortcuts_config_options {
        /** Substitution entries */
        darray(கட்டமைப்பு xkb_shortcuts_config_entry) entries;
    } shortcuts;

    எண்ணிக்கை xkb_machine_flags machine_flags;
    எண்ணிக்கை xkb_machine_builder_flags builder_flags;
    முழு refcnt;
};

கட்டமைப்பு xkb_machine_builder *
xkb_machine_builder_new(
    கட்டமைப்பு xkb_keymap * restrict keymap,
    மாறிலி கட்டமைப்பு xkb_machine_builder_config * restrict config,
    எண்ணிக்கை xkb_status * restrict status
)
{
    /* Handle default configuration */
    நிலையான மாறிலி கட்டமைப்பு xkb_machine_builder_config default_config = {
        .size = அளவு(default_config)
    };
    எனில் (!config)
        config = &default_config;

    /* Check ABI compatibility */
    மாறிலி எண்ணிக்கை xkb_status status_ = xkb_check_state_abi(config);
    எனில் (status_) {
        xkb_log_abi_error(keymap->ctx, __func__, status_);
        எனில் (status)
            *status = status_;
        திரும்பு வெற்று;
    }

    /* Sanitize input */
    மாறிலி எண்ணிக்கை xkb_machine_builder_flags invalid_builder_flags
        = config->builder_flags
        & ~(எண்ணிக்கை xkb_machine_builder_flags)XKB_MACHINE_BUILDER_FLAGS_மதிப்புகள்;
    எனில் (invalid_builder_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS_,
                     "unrecognized machine builder flags: 0x%x\n",
                     invalid_builder_flags);
        எனில் (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS;
        திரும்பு வெற்று;
    }

    மாறிலி எண்ணிக்கை xkb_machine_flags invalid_machine_flags
        = (எண்ணிக்கை xkb_machine_flags)config->machine_flags
        & ~(எண்ணிக்கை xkb_machine_flags)XKB_MACHINE_FLAGS_மதிப்புகள்;
    எனில் (invalid_machine_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS_,
                     "unrecognized state machine flags: 0x%x\n",
                     invalid_machine_flags);
        எனில் (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS;
        திரும்பு வெற்று;
    }

    கட்டமைப்பு xkb_machine_builder * மாறிலி builder = calloc(1, அளவு(*builder));
    எனில் (!builder) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine builder\n");
        எனில் (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        திரும்பு வெற்று;
    }

    *builder = (கட்டமைப்பு xkb_machine_builder) {
        .keymap = xkb_keymap_ref(keymap),
        .refcnt = 1,
        .builder_flags = (எண்ணிக்கை xkb_machine_builder_flags)config->builder_flags,
        .machine_flags = (எண்ணிக்கை xkb_machine_flags)config->machine_flags,
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

    எனில் (status)
        *status = XKB_SUCCESS;

    திரும்பு builder;
}

கட்டமைப்பு xkb_machine_builder *
xkb_machine_builder_ref(கட்டமைப்பு xkb_machine_builder *builder)
{
    ப்ரதிஜ்ஞா(builder->refcnt > 0);
    builder->refcnt++;
    திரும்பு builder;
}

வெறுமை
xkb_machine_builder_unref(கட்டமைப்பு xkb_machine_builder *builder)
{
    ப்ரதிஜ்ஞா(!builder || builder->refcnt > 0);
    எனில் (!builder || --builder->refcnt > 0)
        திரும்பு;

    darray_free(builder->shortcuts.entries);
    darray_free(builder->mods);
    xkb_keymap_unref(builder->keymap);
    விடுவி(builder);
}

கட்டமைப்பு xkb_keymap *
xkb_machine_builder_get_keymap(மாறிலி கட்டமைப்பு xkb_machine_builder *builder)
{
    /* Reference count is not updated. See API doc. */
    திரும்பு builder->keymap;
}

எண்ணிக்கை xkb_status
xkb_machine_builder_update_a11y(
    கட்டமைப்பு xkb_machine_builder * restrict builder,
    மாறிலி கட்டமைப்பு xkb_machine_builder_a11y_update * restrict update)
{
    /* Check ABI compatibility */
    எண்ணிக்கை xkb_status status = xkb_check_state_abi(update);
    எனில் (status != XKB_SUCCESS) {
        xkb_log_abi_error(builder->keymap->ctx, __func__, status);
        திரும்பு status;
    }

    மாறிலி எண்ணிக்கை xkb_a11y_flags invalid_flags =
        ~(எண்ணிக்கை xkb_a11y_flags)XKB_A11_Y_FLAGS_மதிப்புகள்;

    எனில் (update->affect_flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y affected flags: %#x\n",
                __func__, update->affect_flags & invalid_flags);
        திரும்பு XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }
    எனில் (update->flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y flags: %#x\n",
                __func__, update->flags & invalid_flags);
        திரும்பு XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }

    builder->controls.a11y.affect_flags |= update->affect_flags;
    builder->controls.a11y.flags &= ~update->affect_flags;
    builder->controls.a11y.flags |= (update->flags & update->affect_flags);

    திரும்பு XKB_SUCCESS;
}

எண்ணிக்கை xkb_status
xkb_machine_builder_update_mods_remap(
    கட்டமைப்பு xkb_machine_builder * restrict builder,
    மாறிலி கட்டமைப்பு xkb_machine_builder_mods_remap_update * restrict update
)
{
    எனில் (!update->source) {
        எனில் (!update->target) {
            /* Reset mappings */
            darray_resize(builder->mods, 0);
            திரும்பு XKB_SUCCESS;
        } இல்லையெனில் {
            திரும்பு XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
        }
    }

    /* Check the modifiers against the keymap */
    மாறிலி xkb_mod_mask_t invalid = ~builder->keymap->canonical_state_mask;
    எனில் ((update->source & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid source modifiers: 0x%"PRIx32"\n",
                __func__, update->source);
        திரும்பு XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    எனில் ((update->target & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid target modifiers: 0x%"PRIx32"\n",
                __func__, update->target);
        திரும்பு XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    கட்டமைப்பு machine_mods_mapping *mapping = வெற்று;
    darray_size_t m = 0;
    darray_enumerate(m, mapping, builder->mods) {
        எனில் (mapping->source == update->source) {
            எனில் (!update->target) {
                /* Remove mapping */
                darray_remove(builder->mods, m);
            } இல்லையெனில் {
                /* Update mapping */
                mapping->target = update->target;
            }
            திரும்பு XKB_SUCCESS;
        }
    }

    எனில் (update->target) {
        /* Append new mapping */
        darray_append(
            builder->mods,
            (கட்டமைப்பு machine_mods_mapping) {
                .source = update->source,
                .target = update->target
            }
        );
    } இல்லையெனில் {
        /* Ignore missing mapping */
    }

    திரும்பு XKB_SUCCESS;
}

எண்ணிக்கை xkb_status
xkb_machine_builder_update_shortcut_override(
    கட்டமைப்பு xkb_machine_builder * restrict builder,
    மாறிலி கட்டமைப்பு xkb_machine_builder_shortcut_override_update * restrict update
)
{
    கட்டமைப்பு xkb_keymap *keymap = builder->keymap;

    /* Check the modifiers against the keymap */
    மாறிலி xkb_mod_mask_t invalid = ~keymap->canonical_state_mask;
    எனில் ((update->affect_mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid affected modifiers: 0x%"PRIx32"\n",
                __func__, update->affect_mods);
        திரும்பு XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    எனில் ((update->mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid modifiers: 0x%"PRIx32"\n",
                __func__, update->mods);
        திரும்பு XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    /* Check the layout indices against the keymap */
    எனில் (update->source >= keymap->இலக்கம்_groups &&
        update->source != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "%s: Invalid source layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->இலக்கம்_groups, update->source + 1);
        திரும்பு XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }
    எனில் (update->target >= keymap->இலக்கம்_groups &&
        update->target != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "%s: Invalid target layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->இலக்கம்_groups, update->target + 1);
        திரும்பு XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    கட்டமைப்பு xkb_shortcuts_config_options * மாறிலி config = &builder->shortcuts;

    /* Resize array & initialize new entries, if relevant */
    எனில் (update->source >= darray_size(config->entries) &&
        update->source != XKB_LAYOUT_INVALID) {
        எனில் (update->target == update->source) {
            /* Skip default setting */
            திரும்பு XKB_SUCCESS;
        }
        xkb_layout_index_t new = darray_size(config->entries);
        darray_resize(config->entries, update->source + 1);
        க்கு (; new <= update->source; new++) {
            darray_item(config->entries, new) =
                (கட்டமைப்பு xkb_shortcuts_config_entry) {
                    .target = XKB_LAYOUT_INVALID,
                    .mods = 0
                };
        }
    }

    கட்டமைப்பு xkb_shortcuts_config_entry *entry;
    xkb_layout_index_t source = (update->source == XKB_LAYOUT_INVALID)
        ? 0               /* Loop over all entries */
        : update->source; /* Single entry */
    darray_foreach_from(entry, config->entries, source) {
        எனில் (update->source == XKB_LAYOUT_INVALID &&
            entry->target == XKB_LAYOUT_INVALID) {
            /* Skip previous deactivated entry */
            source++;
            தொடர்;
        }

        எனில் (update->affect_mods) {
            entry->mods &= ~update->affect_mods;
            entry->mods |= (update->affect_mods & update->mods);
        }

        எனில் (update->source != XKB_LAYOUT_INVALID) {
            entry->target = (update->target == source)
                ? XKB_LAYOUT_INVALID
                : update->target;
            /* Single entry: exit loop */
            நிறுத்து;
        }
        source++;
    }

    திரும்பு XKB_SUCCESS;
}

/** Compare 2 modifiers combinations */
நிலையான முழு
cmp_mod_masks(மாறிலி வெறுமை *a, மாறிலி வெறுமை *b)
{
    மாறிலி xkb_mod_mask_t m1 = *((xkb_mod_mask_t *) a);
    மாறிலி xkb_mod_mask_t m2 = *((xkb_mod_mask_t *) b);
    எனில் (m1 == m2)
        திரும்பு 0;

    எனில் (!m1)
        திரும்பு +1;
    எனில் (!m2)
        திரும்பு -1;

    மாறிலி xkb_mod_mask_t overlap = (m1 & m2);
    எனில் (overlap == m1)
        திரும்பு +1; /* m1 is a subset of m2 */
    எனில் (overlap == m2)
        திரும்பு -1; /* m2 is a subset of m1 */

    /* Compare active modifiers numbers */
    மாறிலி unsigned முழு count1 = popcount32(m1);
    மாறிலி unsigned முழு count2 = popcount32(m2);
    எனில் (count1 > count2)
        திரும்பு -1;
    எனில் (count1 < count2)
        திரும்பு +1;

    /* Give priority to real modifiers */
    திரும்பு (m1 < m2) ? -1 : +1;
}

நிலையான எண்ணிக்கை xkb_status
machine_set_mods(கட்டமைப்பு xkb_machine *sm,
                 மாறிலி machine_mods_mappings *raw_mappings)
{
    எனில் (!darray_empty(*raw_mappings)) {
        machine_mods_mappings mappings = darray_new();
        xkb_mod_mask_t mask = 0;
        மாறிலி கட்டமைப்பு machine_mods_mapping *mapping;
        மாறிலி xkb_mod_mask_t invalid =
            ~sm->base.base.keymap->canonical_state_mask;
        darray_foreach(mapping, *raw_mappings) {
            எனில் (!mapping->source || !mapping->target ||
                (mapping->source & invalid) || (mapping->target & invalid))
                தொடர்;
            darray_append(mappings, *mapping);
            mask |= mapping->source;
        }

        /*
         * Re-order the mappings, so that combinations have higher priority
         * if they are superset of other combinations.
         * For other type of overlaps, see the sorting function.
         */
        qsort(darray_பொருட்கள்(mappings), darray_size(mappings),
              அளவு(*darray_பொருட்கள்(mappings)), &cmp_mod_masks);

        darray_steal(
            mappings,
            &sm->config.modifiers.mappings,
            &sm->config.modifiers.mappings_இலக்கம்
        );
        sm->config.modifiers.mask = mask;
    } இல்லையெனில் {
        sm->config.modifiers = (கட்டமைப்பு machine_modifiers_config) {
            .mappings = வெற்று,
            .mappings_இலக்கம் = 0,
            .mask = 0
        };
    }

    திரும்பு XKB_SUCCESS;
}

நிலையான எண்ணிக்கை xkb_status
machine_set_shortcuts(
    கட்டமைப்பு xkb_machine * restrict sm,
    மாறிலி கட்டமைப்பு xkb_shortcuts_config_options * restrict options
)
{
    எனில் (darray_empty(options->entries)) {
        sm->config.shortcuts = (கட்டமைப்பு machine_shortcuts_config) {
            .entries = வெற்று
        };
        திரும்பு XKB_SUCCESS;
    }

    கட்டமைப்பு xkb_keymap * மாறிலி keymap = sm->base.base.keymap;

    கட்டமைப்பு xkb_shortcuts_config_entry * entries =
        calloc(keymap->இலக்கம்_groups, அளவு(*entries));
    எனில் (!entries)
        திரும்பு XKB_ERROR_ALLOCATION_FAILURE;

    மாறிலி xkb_layout_index_t மொத்தம் = MIN(
        keymap->இலக்கம்_groups,
        (xkb_layout_index_t) darray_size(options->entries)
    );

    bool some_entries = false;
    க்கு (xkb_layout_index_t l = 0; l < மொத்தம்; l++) {
        /* Sanitize layout target */
        மாறிலி xkb_layout_index_t target =
            darray_item(options->entries, l).target;
        /* Sanitize modifier mask */
        மாறிலி xkb_mod_mask_t mods = (
            darray_item(options->entries, l).mods &
            sm->base.base.keymap->canonical_state_mask
        );
        எனில் (target < keymap->இலக்கம்_groups && target != l && mods) {
            entries[l] = (கட்டமைப்பு xkb_shortcuts_config_entry) {
                .target = target,
                .mods = mods
            };
            some_entries = true;
        } இல்லையெனில் {
            entries[l] = (கட்டமைப்பு xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
            தொடர்;
        }
    }

    எனில் (!some_entries) {
        விடுவி(entries);
        entries = வெற்று;
    } இல்லையெனில் {
        /* Initialize groups with no entries */
        க்கு (xkb_layout_index_t l = மொத்தம்; l < keymap->இலக்கம்_groups; l++) {
            entries[l] = (கட்டமைப்பு xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
        }
    }

    sm->config.shortcuts = (கட்டமைப்பு machine_shortcuts_config) {
        .entries = entries
    };
    திரும்பு XKB_SUCCESS;
}

கட்டமைப்பு xkb_machine *
xkb_machine_new(மாறிலி கட்டமைப்பு xkb_machine_builder * restrict builder,
                எண்ணிக்கை xkb_status * restrict status)
{
    கட்டமைப்பு xkb_machine * மாறிலி machine = calloc(1, அளவு(*machine));
    எனில் (!machine) {
        log_err_func1(builder->keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine\n");
        எனில் (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        திரும்பு வெற்று;
    }

    xkb_server_state_init(&machine->base, builder->keymap, SERVER_STATE,
                          builder->controls.a11y.affect_flags,
                          builder->controls.a11y.flags);
    machine->flags = builder->machine_flags;
    எனில் (!(machine->flags &
          XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_சாவி_FIX))
    {
        machine->base.flags |= STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_சாவி;
    }
    எனில் (machine->flags & XKB_MACHINE_UNLOCK_MODS_ON_PRESS) {
        machine->base.flags |= STATE_UNLOCK_MODS_ON_PRESS;
    }

    எண்ணிக்கை xkb_status status_;
    எனில் ((status_ = machine_set_mods(machine, &builder->mods)) != XKB_SUCCESS ||
        (status_ = machine_set_shortcuts(machine, &builder->shortcuts)) !=
        XKB_SUCCESS) {
        *status = status_;
        செல் error;
    }

    darray_init(machine->overlays.சாவிகள்);
    machine->mouse.default_button = XKB_POINTER_BUTTON_MIN;

    எனில் (status)
        *status = XKB_SUCCESS;

    திரும்பு machine;

error:
    xkb_machine_unref(machine);
    திரும்பு வெற்று;
}

கட்டமைப்பு xkb_machine *
xkb_machine_ref(கட்டமைப்பு xkb_machine *sm)
{
    ப்ரதிஜ்ஞா(sm->base.base.refcnt > 0);
    sm->base.base.refcnt++;
    திரும்பு sm;
}

வெறுமை
xkb_machine_unref(கட்டமைப்பு xkb_machine *sm)
{
    ப்ரதிஜ்ஞா(!sm || sm->base.base.refcnt > 0);
    எனில் (!sm || --sm->base.base.refcnt > 0)
        திரும்பு;

    xkb_state_destroy(&sm->base.base);
    darray_free(sm->overlays.சாவிகள்);
    விடுவி(sm->config.shortcuts.entries);
    விடுவி(sm->config.modifiers.mappings);
    விடுவி(sm);
}

கட்டமைப்பு xkb_keymap *
xkb_machine_get_keymap(மாறிலி கட்டமைப்பு xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    திரும்பு sm->base.base.keymap;
}

கட்டமைப்பு xkb_state *
xkb_machine_get_state(கட்டமைப்பு xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    திரும்பு (கட்டமைப்பு xkb_state *)sm;
}

கட்டமைப்பு xkb_state *
xkb_state_new_from_machine(மாறிலி கட்டமைப்பு xkb_machine * restrict machine,
                           எண்ணிக்கை xkb_status * restrict status)
{
    /* Creation */
    எண்ணிக்கை xkb_status status_;
    கட்டமைப்பு xkb_state * மாறிலி state = xkb_state_new_with_mode(
        machine->base.base.keymap,
        XKB_STATE_MODE_SERVER_QUERY,
        &status_
    );
    எனில் (status_ != XKB_SUCCESS) {
        எனில் (status)
            *status = status_;
        திரும்பு வெற்று;
    }

    ப்ரதிஜ்ஞா(state->mode == SERVER_COMPANION);
    ப்ரதிஜ்ஞா(state->refcnt == 1);

    /* Copy machine’s state */
    *state = machine->base.base;

    /* Restore previous values */
    state->mode = SERVER_COMPANION;
    state->refcnt = 1;

    எனில் (status)
        *status = XKB_SUCCESS;
    திரும்பு state;
}

நிலையான வெறுமை
machine_update_overlays(கட்டமைப்பு xkb_machine *sm)
{
    /*
     * Overlays indices are stored 1-indexed in nibbles: the lowest
     * nibble corresponds to the latest activated index.
     */

    மாறிலி xkb_overlay_mask_t mask =
        overlays_from_controls(sm->base.base.components.controls);
    xkb_overlay_mask_t added = mask & ~sm->overlays.enabled;

    /* Remove overlays no longer enabled and keep relative order */
    uint32_t order = sm->overlays.order;
    மாறிலி xkb_overlay_index_t overlay_max =
        format_max_overlays(sm->base.base.keymap->format);
    க்கு (uint8_t n = 0; n < overlay_max;) {
        xkb_overlay_index_t overlay_idx = (order >> (n * 4)) & 0xf;
        எனில் (!overlay_idx)
            நிறுத்து;
        மாறிலி xkb_overlay_mask_t overlay_mask =
            (xkb_overlay_mask_t)(1u << (overlay_idx - 1u) /* k is 1-indexed */);
        எனில் (overlay_mask & mask) {
            /* no duplicates */
            added &= ~overlay_mask;
            n++;
        } இல்லையெனில் {
            /* remove overlay */
            மாறிலி uint32_t head = (UINT32_C(1) << (n * 4)) - 1;
            order = (order & head) | ((order >> 4) & ~head);
        }
    }

    /* Add new overlays in ascending order */
    க்கு (xkb_overlay_index_t k = 0; added; k++, added >>= 1) {
        எனில் (added & 0x1) {
            order <<= 4;
            order |= k + 1u; /* k is 0-indexed */
        }
    }

    sm->overlays.order = order;
    sm->overlays.enabled = mask;
}

எண்ணிக்கை xkb_status
xkb_machine_process_synthetic(கட்டமைப்பு xkb_machine *sm,
                              மாறிலி கட்டமைப்பு xkb_synthetic_update *update,
                              கட்டமைப்பு xkb_events *events)
{
    /* Check ABI compatibility */
    எண்ணிக்கை xkb_status status =
        check_state_update_abi(sm->base.base.keymap->ctx, update);
    எனில் (status != XKB_SUCCESS)
        திரும்பு status;

    கட்டமைப்பு xkb_server_state * மாறிலி state = &sm->base;
    மாறிலி கட்டமைப்பு state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    darray_size(events->queue) = 0;
    events->next = 0;

    /* Update parametrized controls first */
    எனில் (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        எனில் (status != XKB_SUCCESS)
            திரும்பு status;
    }

    எனில் (update->components) {
        மாறிலி கட்டமைப்பு xkb_state_components_update * மாறிலி components =
            update->components;
        /* Update boolean controls first */
        எனில் (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, events);
        }

        state_update_latched_locked(state, components, events);
    }

    xkb_state_update_derived(&state->base);

    மாறிலி எண்ணிக்கை xkb_state_component changed = get_state_component_changes(
        &previous_components, &state->base.components
    );
    எனில் (changed) {
        // TODO: latch controls
        எனில் (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        /* Create event only if some component actually changed */
        darray_append(events->queue, (கட்டமைப்பு xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .changed = changed,
                .components = state->base.components
            }
        });

        /* Terminate the frame */
        darray_append(events->queue, (கட்டமைப்பு xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    திரும்பு XKB_SUCCESS;
}

நிலையான ssize_t
do_remap_modifiers(மாறிலி கட்டமைப்பு machine_modifiers_config * restrict mappings,
                   கட்டமைப்பு xkb_state * restrict state,
                   கட்டமைப்பு xkb_events * restrict events,
                   மாறிலி கட்டமைப்பு xkb_சாவி * restrict சாவி)
{
    எனில் (!(mappings->mask & state->components.mods))
        திரும்பு -1;

    மாறிலி xkb_layout_index_t layout = state_சாவி_get_layout(state, சாவி);
    எனில் (layout >= சாவி->இலக்கம்_groups)
        திரும்பு -1;

    மாறிலி கட்டமைப்பு xkb_சாவி_type * மாறிலி type = சாவி->groups[layout].type;
    xkb_mod_mask_t affect_mods = 0;
    xkb_mod_mask_t mods = 0;
    க்கு (darray_size_t m = 0; m < mappings->mappings_இலக்கம்; m++) {
        மாறிலி கட்டமைப்பு machine_mods_mapping * மாறிலி mapping =
            &mappings->mappings[m];
        எனில் (/* Skip not matching active mods */
            (mapping->source & state->components.mods) == mapping->source &&
            /* Skip overlap with previous active mappings */
            !(mapping->source & affect_mods) &&
            /* Skip if the key type uses some of the source modifiers */
            !(mapping->source & type->mods.mask)) {
            affect_mods |= mappings->mappings[m].source;
            mods |= mappings->mappings[m].target;
        }
    }

    எனில் (!affect_mods)
        திரும்பு -1;

    கட்டமைப்பு xkb_state new = *state;
    new.components.base_mods = (new.components.base_mods & ~affect_mods) | mods;
    new.components.latched_mods = (new.components.latched_mods & ~affect_mods);
    new.components.locked_mods = (new.components.locked_mods & ~affect_mods);
    xkb_state_update_derived(&new);

    ssize_t event_idx = -1;
    மாறிலி எண்ணிக்கை xkb_state_component changed =
        get_state_component_changes(&state->components, &new.components);
    எனில் (changed) {
        event_idx = (ssize_t) darray_size(events->queue);
        darray_append(events->queue, (கட்டமைப்பு xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = new.components,
                .changed = changed
            }
        });
    }

    state->components.mods = new.components.mods;
    திரும்பு event_idx;
}

/**
 * Activate an alternative layout if some modifiers are active, typically
 * `Control`, `Alt` or `Super`.
 *
 * The tweak consists of computing a *base* layout such that the *effective*
 * layout is the corresponding target layout.
 */
நிலையான ssize_t
do_shortcuts_tweak(மாறிலி கட்டமைப்பு machine_shortcuts_config *config,
                   கட்டமைப்பு xkb_state *state,
                   மாறிலி கட்டமைப்பு state_components *previous_components,
                   கட்டமைப்பு xkb_events *events, ssize_t remap_event)
{
    எனில் (!config->entries) {
        /* No shortcuts tweak */
        திரும்பு remap_event;
    }

    மாறிலி கட்டமைப்பு xkb_shortcuts_config_entry * மாறிலி entry =
        &config->entries[state->components.group];

    எனில் (entry->target != XKB_LAYOUT_INVALID &&
        (entry->mods & state->components.mods)) {
        /*
         * Activate shortcuts tweak:
         * 1. The real base group is saved by the caller, to be restored by
         *    later by a call to undo_tweaks().
         * 2. Compute the new base group so that the resulting effective group
         *    is the target.
         * 3. Recompute derived state components.
         */
        கட்டமைப்பு xkb_state new = *state;
        எனில் (remap_event < 0) {
            /* Create new event */
            remap_event = (ssize_t) darray_size(events->queue);
            darray_append(events->queue, (கட்டமைப்பு xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = {0},
                    .changed = 0
                }
            });
        } இல்லையெனில் {
            /* Merge with last remap event */
            new.components =
                darray_item(events->queue, remap_event).components.components;
        }
        new.components.base_group
            = (int32_t) entry->target
            - state->components.latched_group
            - state->components.locked_group;
        xkb_state_update_derived(&new);

        மாறிலி எண்ணிக்கை xkb_state_component changed =
            get_state_component_changes(previous_components, &new.components);
        darray_item(events->queue, remap_event).components.changed = changed;
        darray_item(events->queue, remap_event).components.components =
            new.components;

        state->components.group = new.components.group;
    } இல்லையெனில் {
        /* Deactivated entry or modifiers do not match */
    }
    திரும்பு remap_event;
}

நிலையான வெறுமை
undo_tweaks(மாறிலி கட்டமைப்பு xkb_state *state,
            மாறிலி கட்டமைப்பு state_components *initial_components,
            கட்டமைப்பு xkb_events *events)
{
    /* Get last component event */
    மாறிலி கட்டமைப்பு xkb_event *event = வெற்று;
    darray_foreach_reverse(event, events->queue) {
        எனில் (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS)
            நிறுத்து;
    }
    எனில் (!event)
        திரும்பு;

    /* Restore state */
    மாறிலி எண்ணிக்கை xkb_state_component changed =
        get_state_component_changes(initial_components,
                                    &event->components.components);
    எனில் (changed) {
        darray_append(events->queue, (கட்டமைப்பு xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = *initial_components,
                .changed = changed
            }
        });
    }
}

நிலையான மாறிலி கட்டமைப்பு xkb_சாவி *
process_overlayable_சாவி(கட்டமைப்பு xkb_machine *sm,
                        மாறிலி கட்டமைப்பு xkb_சாவி * சாவி,
                        எண்ணிக்கை xkb_சாவி_direction direction)
{
    /* Check if key is currently overlaid */
    கட்டமைப்பு xkb_overlaid_சாவி *entry = வெற்று;
    கட்டமைப்பு xkb_overlaid_சாவி *available_entry = வெற்று;
    darray_foreach(entry, sm->overlays.சாவிகள்) {
        எனில் (entry->old == சாவி) {
            /* Maintain the key overlaid */
            தேர்வு (direction) {
            நிலை XKB_சாவி_DOWN:
                entry->refcnt++;
                நிறுத்து;
            நிலை XKB_சாவி_REPEATED:
                நிறுத்து;
            நிலை XKB_சாவி_UP:
            இயல்பு:
                entry->refcnt--;
                நிறுத்து;
            }
            எனில் (entry->refcnt <= 0)
                entry->old = வெற்று;
            திரும்பு entry->new;
        } இல்லையெனில் எனில் (!entry->old && !available_entry) {
            /* First free spot */
            available_entry = entry;
        }
    }
    எனில் (direction == XKB_சாவி_DOWN) {
        /* Enable key overlay */

        /*
         * Get overlay index: select the latest activated overlay
         * containing the key.
         */
        மாறிலி கட்டமைப்பு xkb_சாவி *new = சாவி;

        எனில் (சாவி->overlays & sm->overlays.enabled) {
            /* Some relevant overlay is active */
            க்கு (uint32_t அடுக்கு = sm->overlays.order; அடுக்கு; அடுக்கு >>= 4) {
                static_assert(XKB_OVERLAY_மொத்தம் == 8, "");
                மாறிலி xkb_overlay_index_t overlay = (அடுக்கு & 0xf) - 1;
                மாறிலி xkb_overlay_mask_t mask =
                    (xkb_overlay_mask_t)(1u << overlay);
                எனில் (சாவி->overlays & mask) {
                    /* Get new key */
                    எனில் (சாவி->overlays_inline) {
                        /* Stored inline */
                        new = சாவி->overlay_சாவி;
                    } இல்லையெனில் {
                        /* Stored in sparse array */
                        மாறிலி xkb_overlay_mask_t கீழ் = (xkb_overlay_mask_t)(
                            சாவி->overlays & (xkb_overlay_mask_t)(mask - 1u)
                        );
                        /* index = number of set bits strictly below bit */
                        மாறிலி xkb_overlay_index_t index =
                            (xkb_overlay_index_t)popcount32(கீழ்);
                        new = சாவி->overlays_சாவிகள்[index];
                    }
                    நிறுத்து;
                }
            }
        } இல்லையெனில் {
            /*
             * No relevant overlay is active: block future overlays while
             * the key is pressed.
             */
        }

        /* Get new entry */
        எனில் (available_entry) {
            /* Re-use a free entry */
            entry = available_entry;
        } இல்லையெனில் {
            /* Append a new entry */
            மாறிலி darray_size_t idx = darray_size(sm->overlays.சாவிகள்);
            darray_resize(sm->overlays.சாவிகள், idx + 1);
            entry = &darray_item(sm->overlays.சாவிகள், idx);
        }

        /* Overlay the key */
        entry->old = சாவி;
        entry->new = new;
        entry->refcnt = 1;
        திரும்பு new;
    }

    /* No overlay */
    திரும்பு சாவி;
}

எண்ணிக்கை xkb_status
xkb_machine_process_சாவி(கட்டமைப்பு xkb_machine *sm,
                        xkb_keycode_t kc, எண்ணிக்கை xkb_சாவி_direction direction,
                        கட்டமைப்பு xkb_events *events)
{
    darray_size(events->queue) = 0;
    events->next = 0;

    கட்டமைப்பு xkb_server_state * மாறிலி state = &sm->base;
    மாறிலி கட்டமைப்பு xkb_சாவி * சாவி = Xkb_சாவி(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    எனில் (!சாவி || (direction == XKB_சாவி_REPEATED && !சாவி->repeats))
        திரும்பு XKB_SUCCESS;

    மாறிலி கட்டமைப்பு state_components initial_components = state->base.components;

    எனில் (direction == XKB_சாவி_DOWN &&
        (sm->base.base.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_சாவிகள்) &&
        (sm->base.a11y_flags & XKB_A11_Y_STICKY_சாவிகள்_NO_SIMULTANEOUS_சாவிகள்) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_சாவிகள்,
                                      0, events);
    }

    எனில் (சாவி->overlays)
        சாவி = process_overlayable_சாவி(sm, சாவி, direction);

    ssize_t remap_event = do_remap_modifiers(&sm->config.modifiers,
                                             &state->base, events, சாவி);

    remap_event = do_shortcuts_tweak(&sm->config.shortcuts, &state->base,
                                     &initial_components, events, remap_event);

    எனில் (remap_event >= 0) {
        // TODO: what about merging state event for RedirectKey?
        darray_append(events->queue, (கட்டமைப்பு xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    state->flags |= STATE_REQUIRE_சாவி_EVENT;
    state->set_mods = 0;
    state->clear_mods = 0;

    /* Handle key actions */
    xkb_filter_apply_all(state, events, சாவி, direction);

    xkb_mod_index_t i;
    xkb_mod_mask_t bit;
    க்கு (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        எனில் (state->set_mods & bit) {
            state->mod_சாவி_மொத்தம்[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    க்கு (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        எனில் (state->clear_mods & bit) {
            state->mod_சாவி_மொத்தம்[i]--;
            எனில் (state->mod_சாவி_மொத்தம்[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_சாவி_மொத்தம்[i] = 0;
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
    எனில் (state->flags & STATE_REQUIRE_சாவி_EVENT) {
        darray_append(events->queue, (கட்டமைப்பு xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_சாவி,
            .சாவி = {
                .keycode = சாவி->keycode,
                .direction = direction
            }
        });
    }

    எனில் (remap_event >= 0) {
        // FIXME: fragile if last state change does not restore the state to the remap event
        undo_tweaks(&state->base, &initial_components, events);
    }

    மாறிலி எண்ணிக்கை xkb_state_component changed = get_state_component_changes(
        &initial_components, &state->base.components
    );
    எனில் (changed) {
        எனில் (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        // TODO: merge with previous state event, if any

        darray_append(events->queue, (கட்டமைப்பு xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = state->base.components,
                .changed = changed
            }
        });
    }

    எனில் (!darray_empty(events->queue) &&
        darray_item(events->queue, darray_size(events->queue) - 1).type !=
        XKB_EVENT_TYPE_FRAME)
    {
        darray_append(events->queue, (கட்டமைப்பு xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    திரும்பு XKB_SUCCESS;
}

கட்டமைப்பு xkb_events *
xkb_events_new(கட்டமைப்பு xkb_context * restrict context,
               மாறிலி கட்டமைப்பு xkb_events_config * restrict config,
               எண்ணிக்கை xkb_status * restrict status)
{
    /* Handle default configuration */
    நிலையான மாறிலி கட்டமைப்பு xkb_events_config default_config = {
        .size = அளவு(default_config)
    };
    எனில் (!config)
        config = &default_config;

    /* Check ABI compatibility */
    மாறிலி எண்ணிக்கை xkb_status abi_error = xkb_check_state_abi(config);
    எனில் (abi_error) {
        xkb_log_abi_error(context, __func__, abi_error);
        எனில் (status)
            *status = abi_error;
        திரும்பு வெற்று;
    }

    /* Sanitize input */
    மாறிலி uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_EVENTS_FLAGS_மதிப்புகள்);
    எனில் (invalid_flags) {
        log_err_func(context, XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS_,
                     "unrecognized events collection flags: 0x%"PRIx32"\n",
                     invalid_flags);
        எனில் (status)
            *status = XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS;
        திரும்பு வெற்று;
    }

    கட்டமைப்பு xkb_events *events = calloc(1, அளவு(*events));
    எனில் (!events) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events collection\n");
        எனில் (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        திரும்பு வெற்று;
    }

    /* Pre-allocate event queue */
    darray_init(events->queue);
    நிலையான மாறிலி darray_size_t queue_initial_size = 16;
    darray_resize(events->queue, queue_initial_size);
    // TODO: proper safe darray_resize() with return value
    எனில் (!darray_பொருட்கள்(events->queue)) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events queue\n");
        xkb_events_destroy(events);
        எனில் (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        திரும்பு வெற்று;
    }
    darray_size(events->queue) = 0;

    events->ctx = xkb_context_ref(context);
    events->next = 0;

    எனில் (status)
        *status = XKB_SUCCESS;
    திரும்பு events;
}

வெறுமை
xkb_events_destroy(கட்டமைப்பு xkb_events *events)
{
    எனில் (!events)
        திரும்பு;
    darray_free(events->queue);
    xkb_context_unref(events->ctx);
    விடுவி(events);
}

மாறிலி கட்டமைப்பு xkb_event *
xkb_events_next(கட்டமைப்பு xkb_events *events)
{
    எனில் (events->next < darray_size(events->queue)) {
        மாறிலி darray_size_t index = events->next++;
        திரும்பு &darray_item(events->queue, index);
    } இல்லையெனில் {
        திரும்பு வெற்று;
    }
}

எண்ணிக்கை xkb_event_type
xkb_event_get_type(மாறிலி கட்டமைப்பு xkb_event *event)
{
    திரும்பு event->type;
}

எண்ணிக்கை xkb_status
xkb_event_get_keycode(மாறிலி கட்டமைப்பு xkb_event *event,
                      xkb_keycode_t *keycode,
                      எண்ணிக்கை xkb_சாவி_direction *direction)
{
    எனில் (event->type == XKB_EVENT_TYPE_சாவி) {
        *keycode = event->சாவி.keycode;
        *direction = event->சாவி.direction;
        திரும்பு XKB_SUCCESS;
    } இல்லையெனில் {
        திரும்பு XKB_ERROR_EVENT_TYPE_MISMATCH;
    }
}

எண்ணிக்கை xkb_status
xkb_event_get_components(மாறிலி கட்டமைப்பு xkb_event * restrict event,
                         கட்டமைப்பு xkb_event_components * restrict components)
{
    எனில் (event->type != XKB_EVENT_TYPE_STATE_COMPONENTS)
        திரும்பு XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    மாறிலி எண்ணிக்கை xkb_status status = xkb_check_state_abi(components);
    எனில் (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        திரும்பு status;
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

    திரும்பு XKB_SUCCESS;
}

எண்ணிக்கை xkb_status
xkb_event_get_pointer_motion(மாறிலி கட்டமைப்பு xkb_event * restrict event,
                             கட்டமைப்பு xkb_event_pointer_motion * restrict motion)
{
    எனில் (event->type != XKB_EVENT_TYPE_POINTER_MOTION)
        திரும்பு XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    மாறிலி எண்ணிக்கை xkb_status status = xkb_check_state_abi(motion);
    எனில் (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        திரும்பு status;
    }

    motion->flags = event->pointer_motion.flags;
    motion->x = event->pointer_motion.x;
    motion->y = event->pointer_motion.y;

    திரும்பு XKB_SUCCESS;
}

எண்ணிக்கை xkb_status
xkb_event_get_pointer_button(மாறிலி கட்டமைப்பு xkb_event * restrict event,
                             கட்டமைப்பு xkb_event_pointer_button * restrict button)
{
    எனில் (event->type != XKB_EVENT_TYPE_POINTER_BUTTON)
        திரும்பு XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    மாறிலி எண்ணிக்கை xkb_status status = xkb_check_state_abi(button);
    எனில் (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        திரும்பு status;
    }

    button->button = event->pointer_button.button;
    button->state = event->pointer_button.state;
    button->மொத்தம் = event->pointer_button.மொத்தம்;

    திரும்பு XKB_SUCCESS;
}

எண்ணிக்கை xkb_status
xkb_event_get_virtual_console(மாறிலி கட்டமைப்பு xkb_event * restrict event,
                              int8_t * restrict index_or_offset,
                              bool * restrict is_offset)
{
    எனில் (event->type != XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
        திரும்பு XKB_ERROR_EVENT_TYPE_MISMATCH;

    *index_or_offset = event->virtual_console.index_or_offset;
    *is_offset = event->virtual_console.is_offset;

    திரும்பு XKB_SUCCESS;
}

எண்ணிக்கை xkb_status
xkb_state_update_event(கட்டமைப்பு xkb_state * restrict base_state,
                       மாறிலி கட்டமைப்பு xkb_event * restrict event,
                       எண்ணிக்கை xkb_state_component * restrict changed)
{
    /* Guard against server-only state */
    ப்ரதிஜ்ஞா(base_state->mode == SERVER_COMPANION ||
           base_state->mode == LEGACY_MIXED_STATE);
    எனில் (base_state->mode != SERVER_COMPANION &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE_,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        திரும்பு XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    கட்டமைப்பு xkb_client_state * மாறிலி state =
        (கட்டமைப்பு xkb_client_state *)base_state;

    எனில் (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
        மாறிலி கட்டமைப்பு state_components prev_components = state->base.components;
        state->base.components = event->components.components;
        /*
         * Recompute the changes instead of using the event value, because we do
         * not know if the event’s queue and the state are synced.
         */
        எனில் (changed)
            *changed = get_state_component_changes(&prev_components,
                                                   &state->base.components);
    } இல்லையெனில் {
        எனில் (changed)
            *changed = 0;
    }
    திரும்பு XKB_SUCCESS;
}

நிலையான வெறுமை
append_pointer_button(கட்டமைப்பு xkb_events * restrict events,
                      கட்டமைப்பு xkb_filter * restrict filter,
                      எண்ணிக்கை xkb_pointer_button_state button_state,
                      uint8_t மொத்தம்)
{
    darray_append(events->queue, (கட்டமைப்பு xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_BUTTON,
        .pointer_button = {
            .size = அளவு(((கட்டமைப்பு xkb_event*)0)->pointer_button),
            .button = filter->priv,
            .state = button_state,
            .மொத்தம் = மொத்தம்,
        }
    });
}

நிலையான வெறுமை
xkb_filter_pointer_button_new(கட்டமைப்பு xkb_server_state *state,
                              கட்டமைப்பு xkb_events *events,
                              கட்டமைப்பு xkb_filter *filter)
{
    எனில் (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_சாவிகள்)) {
        filter->func = வெற்று;
        திரும்பு;
    }

    கட்டமைப்பு xkb_machine * மாறிலி sm = (கட்டமைப்பு xkb_machine *)state;

    /* Resolve button */
    மாறிலி xkb_pointer_button_index_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;
    filter->priv = button;

    மாறிலி எண்ணிக்கை xkb_pointer_button_state button_state =
        XKB_POINTER_BUTTON_PRESSED;
    append_pointer_button(events, filter, button_state,
                          filter->action.btn.மொத்தம்);
    state->flags &= ~STATE_REQUIRE_சாவி_EVENT;
}

நிலையான bool
xkb_filter_pointer_button_func(கட்டமைப்பு xkb_server_state *state,
                               கட்டமைப்பு xkb_events *events,
                               கட்டமைப்பு xkb_filter *filter,
                               மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                               எண்ணிக்கை xkb_சாவி_direction direction)
{
    எனில் (சாவி != filter->சாவி || !events)
        திரும்பு XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_சாவி_EVENT;

    தேர்வு (direction) {
    நிலை XKB_சாவி_DOWN:
        filter->refcnt++;
        /* fallthrough */
    நிலை XKB_சாவி_REPEATED:
        திரும்பு XKB_FILTER_CONSUME;
    இயல்பு:
        எனில் (--filter->refcnt > 0)
            திரும்பு XKB_FILTER_CONSUME;
    }

    எனில் (!filter->action.btn.மொத்தம்)
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);

    filter->func = வெற்று;
    திரும்பு XKB_FILTER_CONSUME;
}

கட்டமைப்பு xkb_filter_pointer_lock_button_priv {
    uint32_t button:31;
#ifdef _WIN32
    uint32_t locked:1; // MSVC requires matching types to pack bitfields
#இல்லையெனில்
    bool locked:1;
#endif
};

எண்ணிக்கை {
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH =
        அளவு(கட்டமைப்பு xkb_filter_pointer_lock_button_priv) * CHAR_BIT,
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX =
        (UINT64_C(1) << XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH) - 1
};

static_assert(அளவு(கட்டமைப்பு xkb_filter_pointer_lock_button_priv) ==
              அளவு(((கட்டமைப்பு xkb_filter*)0)->priv),
              "");
static_assert((uint32_t)XKB_POINTER_BUTTON_MAX < (uint32_t)XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX,
              "xkb_filter_pointer_lock_button_priv cannot store button");

நிலையான வெறுமை
xkb_filter_pointer_lock_button_new(கட்டமைப்பு xkb_server_state *state,
                                   கட்டமைப்பு xkb_events *events,
                                   கட்டமைப்பு xkb_filter *filter)
{
    எனில் (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_சாவிகள்)) {
        filter->func = வெற்று;
        திரும்பு;
    }

    state->flags &= ~STATE_REQUIRE_சாவி_EVENT;

    கட்டமைப்பு xkb_machine * மாறிலி sm = (கட்டமைப்பு xkb_machine *)state;

    /* Resolve button */
    மாறிலி xkb_pointer_button_index_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;

    static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                  "invalid UINTx_C() macro");
    static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                  "invalid left shift");
    மாறிலி xkb_pointer_button_mask_t mask = (UINT8_C(1) << button);

    மாறிலி bool locked = (sm->mouse.locked_buttons & mask);

    எனில் (!locked && !(filter->action.btn.flags & ACTION_LOCK_NO_LOCK)) {
        sm->mouse.locked_buttons |= mask;
        filter->priv = button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_PRESSED, 0);
    }

    கட்டமைப்பு xkb_filter_pointer_lock_button_priv *priv =
        (கட்டமைப்பு xkb_filter_pointer_lock_button_priv*)&filter->priv;
    priv->button = button,
    priv->locked = locked;
}

நிலையான bool
xkb_filter_pointer_lock_button_func(கட்டமைப்பு xkb_server_state *state,
                                    கட்டமைப்பு xkb_events *events,
                                    கட்டமைப்பு xkb_filter *filter,
                                    மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                                    எண்ணிக்கை xkb_சாவி_direction direction)
{
    எனில் (சாவி != filter->சாவி || !events || state->base.mode != SERVER_STATE)
        திரும்பு XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_சாவி_EVENT;

    தேர்வு (direction) {
    நிலை XKB_சாவி_DOWN:
        filter->refcnt++;
        /* fallthrough */
    நிலை XKB_சாவி_REPEATED:
        திரும்பு XKB_FILTER_CONSUME;
    இயல்பு:
        எனில் (--filter->refcnt > 0)
            திரும்பு XKB_FILTER_CONSUME;
    }

    மாறிலி கட்டமைப்பு xkb_filter_pointer_lock_button_priv *priv =
        (கட்டமைப்பு xkb_filter_pointer_lock_button_priv*)&filter->priv;

    எனில் (priv->locked && !(filter->action.btn.flags & ACTION_LOCK_NO_UNLOCK)) {
        கட்டமைப்பு xkb_machine * மாறிலி sm = (கட்டமைப்பு xkb_machine *)state;

        static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                    "invalid UINTx_C() macro");
        static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                    "invalid left shift");
        மாறிலி xkb_pointer_button_mask_t mask = (UINT8_C(1) << priv->button);

        sm->mouse.locked_buttons &= ~mask;
        filter->priv = priv->button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);
    }

    filter->func = வெற்று;
    திரும்பு XKB_FILTER_CONSUME;
}

நிலையான வெறுமை
xkb_filter_pointer_set_default_button_new(கட்டமைப்பு xkb_server_state *state,
                                          கட்டமைப்பு xkb_events *events,
                                          கட்டமைப்பு xkb_filter *filter)
{
    எனில் (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_சாவிகள்)) {
        filter->func = வெற்று;
        திரும்பு;
    }

    state->flags &= ~STATE_REQUIRE_சாவி_EVENT;

    கட்டமைப்பு xkb_machine * மாறிலி sm = (கட்டமைப்பு xkb_machine *)state;

    static_assert(2 * XKB_POINTER_BUTTON_MAX <= INT_MAX, "lossy conversion");
    முழு button = (filter->action.dflt.flags & ACTION_ABSOLUTE_SWITCH)
        ? filter->action.dflt.மதிப்பு
        : sm->mouse.default_button + filter->action.dflt.மதிப்பு;

    /* Wrap into range */
    மாறிலி முழு rem = button % XKB_POINTER_BUTTON_MAX;
    sm->mouse.default_button = (rem >= 0)
        ? (xkb_pointer_button_index_t)rem
        : (xkb_pointer_button_index_t)(rem + XKB_POINTER_BUTTON_MAX);
}

நிலையான bool
xkb_filter_pointer_set_default_button_func(கட்டமைப்பு xkb_server_state *state,
                                           கட்டமைப்பு xkb_events *events,
                                           கட்டமைப்பு xkb_filter *filter,
                                           மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                                           எண்ணிக்கை xkb_சாவி_direction direction)
{
    எனில் (சாவி != filter->சாவி || !events)
        திரும்பு XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_சாவி_EVENT;

    தேர்வு (direction) {
    நிலை XKB_சாவி_DOWN:
        filter->refcnt++;
        /* fallthrough */
    நிலை XKB_சாவி_REPEATED:
        திரும்பு XKB_FILTER_CONSUME;
    இயல்பு:
        எனில் (--filter->refcnt > 0)
            திரும்பு XKB_FILTER_CONSUME;
    }

    filter->func = வெற்று;
    திரும்பு XKB_FILTER_CONTINUE;
}

நிலையான வெறுமை
xkb_filter_terminate_server_new(கட்டமைப்பு xkb_server_state *state,
                                 கட்டமைப்பு xkb_events *events,
                                 கட்டமைப்பு xkb_filter *filter)
{
    எனில் (!events || state->base.mode != SERVER_STATE ||
        !(((கட்டமைப்பு xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = வெற்று;
        திரும்பு;
    }

    state->flags &= ~STATE_REQUIRE_சாவி_EVENT;

    darray_append(events->queue, (கட்டமைப்பு xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    });
}

நிலையான bool
xkb_filter_terminate_server_func(கட்டமைப்பு xkb_server_state *state,
                                 கட்டமைப்பு xkb_events *events,
                                 கட்டமைப்பு xkb_filter *filter,
                                 மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                                 எண்ணிக்கை xkb_சாவி_direction direction)
{
    எனில் (சாவி != filter->சாவி || !events)
        திரும்பு XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_சாவி_EVENT;

    தேர்வு (direction) {
    நிலை XKB_சாவி_DOWN:
        filter->refcnt++;
        /* fallthrough */
    நிலை XKB_சாவி_REPEATED:
        திரும்பு XKB_FILTER_CONSUME;
    இயல்பு:
        எனில் (--filter->refcnt > 0)
            திரும்பு XKB_FILTER_CONSUME;
    }

    filter->func = வெற்று;
    திரும்பு XKB_FILTER_CONSUME;
}

நிலையான வெறுமை
xkb_filter_switch_virtual_console_new(கட்டமைப்பு xkb_server_state *state,
                                      கட்டமைப்பு xkb_events *events,
                                      கட்டமைப்பு xkb_filter *filter)
{
    எனில் (!events || state->base.mode != SERVER_STATE ||
        !(((கட்டமைப்பு xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = வெற்று;
        திரும்பு;
    }

    state->flags &= ~STATE_REQUIRE_சாவி_EVENT;

    darray_append(events->queue, (கட்டமைப்பு xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
        .virtual_console = {
            .index_or_offset = filter->action.screen.screen,
            .is_offset = !(filter->action.screen.flags & ACTION_ABSOLUTE_SWITCH)
        }
    });
}

நிலையான bool
xkb_filter_switch_virtual_console_func(கட்டமைப்பு xkb_server_state *state,
                                       கட்டமைப்பு xkb_events *events,
                                       கட்டமைப்பு xkb_filter *filter,
                                       மாறிலி கட்டமைப்பு xkb_சாவி *சாவி,
                                       எண்ணிக்கை xkb_சாவி_direction direction)
{
    எனில் (சாவி != filter->சாவி || !events)
        திரும்பு XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_சாவி_EVENT;

    தேர்வு (direction) {
    நிலை XKB_சாவி_DOWN:
        filter->refcnt++;
        /* fallthrough */
    நிலை XKB_சாவி_REPEATED:
        திரும்பு XKB_FILTER_CONSUME;
    இயல்பு:
        எனில் (--filter->refcnt > 0)
            திரும்பு XKB_FILTER_CONSUME;
    }

    filter->func = வெற்று;
    திரும்பு XKB_FILTER_CONSUME;
}
