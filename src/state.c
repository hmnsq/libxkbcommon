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

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-status.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-features.h"
#ಅನ್ತರ್ಭಾವ "abi-check.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "features/enums.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "keysym.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "state-priv.h"
#ಅನ್ತರ್ಭಾವ "utf8.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "util-mem.h"
#ಅನ್ತರ್ಭಾವ "util-numbers.h"

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
ಸಂರಚನಾ xkb_events {
    ಸಂರಚನಾ xkb_context *ctx;
    darray(ಸಂರಚನಾ xkb_event) queue;
    /**
     * Read cursor for `xkb_events_next()`.
     *
     * Reset to 0 on each `process_*` call.
     */
    darray_size_t next;
};

ಸಂರಚನಾ xkb_server_state;

ಸಂರಚನಾ xkb_filter {
    ಸಙ್ಘ xkb_action action;
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ;
    bool (*func)(ಸಂರಚನಾ xkb_server_state *state,
                 ಸಂರಚನಾ xkb_events *events,
                 ಸಂರಚನಾ xkb_filter *filter,
                 ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                 ಗಣನಾ xkb_ಕೀಲಿ_direction direction);
    uint32_t priv;
    ಪೂರ್ಣಾಙ್ಕ refcnt;
};

ಗಣನಾ xkb_state_mode_internal {
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
    XKB_STATE_TYPE_ಅಂಕೆ_ENTRIES = 5
};

static_assert(CLIENT_STATE == 0 &&
              CLIENT_STATE == SERVER_COMPANION - 1 &&
              SERVER_COMPANION == LEGACY_MIXED_STATE - 1 &&
              LEGACY_MIXED_STATE == LEGACY_SERVER_STATE - 1 &&
              LEGACY_SERVER_STATE == SERVER_STATE - 1 &&
              SERVER_STATE == XKB_STATE_TYPE_ಅಂಕೆ_ENTRIES - 1 &&
              XKB_STATE_TYPE_ಅಂಕೆ_ENTRIES == 5,
              "Order is used to guard client/server -only APIs: do not change");

ಗಣನಾ { XKB_LAYOUT_INDEX_T_MIN_WIDTH = 6 };
static_assert(XKB_MAX_GROUPS < (1 << XKB_LAYOUT_INDEX_T_MIN_WIDTH),
              "Cannot encode out_of_range_group::redirect_group");

ಸಂರಚನಾ out_of_range_group {
    ಗಣನಾ xkb_layout_out_of_range_policy policy :
        (ಪರಿಮಾಣ(ಗಣನಾ xkb_layout_out_of_range_policy) * CHAR_BIT -
         XKB_LAYOUT_INDEX_T_MIN_WIDTH);
    xkb_layout_index_t redirect_group : XKB_LAYOUT_INDEX_T_MIN_WIDTH;
};

ಗಣನಾ { XKB_STATE_MODE_INTERNAL_MIN_WIDTH = 4 /* 3 bits + sign */ };
static_assert(XKB_STATE_TYPE_ಅಂಕೆ_ENTRIES <
              (1 << XKB_STATE_MODE_INTERNAL_MIN_WIDTH),
              "Cannot encode xkb_state::mode");

/**
 * Common state
 *
 * `mode` indicates whether the object is casted from a `xkb_server_state` or a
 * `xkb_client_state` struct.
 */
ಸಂರಚನಾ xkb_state {
    ಸಂರಚನಾ xkb_keymap *keymap;
    /*
     * Before updating the state, we keep a copy of just this struct. This
     * allows us to report which components of the state have changed.
     */
    ಸಂರಚನಾ state_components components;
    ಸಂರಚನಾ out_of_range_group out_of_range_group;

    ಗಣನಾ xkb_state_mode_internal mode : XKB_STATE_MODE_INTERNAL_MIN_WIDTH;
    ಪೂರ್ಣಾಙ್ಕ refcnt : (ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ) * CHAR_BIT - XKB_STATE_MODE_INTERNAL_MIN_WIDTH);
};

/**
 * Minimal client state
 *
 * Wrapper for strong typing
 */
ಸಂರಚನಾ xkb_client_state {
    /** Inherits from `xkb_state` */
    ಸಂರಚನಾ xkb_state base;
};

ಗಣನಾ state_flags {
    STATE_NO_FLAGS = 0,
    STATE_REQUIRE_ಕೀಲಿ_EVENT = (1 << 0),
    STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_ಕೀಲಿ = (1 << 1),
    STATE_UNLOCK_MODS_ON_PRESS = (1 << 2),
};

/**
 * Legacy server state, used for `LEGACY_MIXED_STATE` and `LEGACY_SERVER_STATE`
 * modes.
 *
 * Casted as `xkb_state` in the public API. See `xkb_state::mode`.
 */
ಸಂರಚನಾ xkb_server_state {
    /** Inherits from `xkb_state` */
    ಸಂರಚನಾ xkb_state base;

    /* Server-specific state data */

    darray(ಸಂರಚನಾ xkb_filter) filters;

    /**
     * Mix of constant configuration flags and
     * variable state-processing flags.
     */
    ಗಣನಾ state_flags flags;

    ಗಣನಾ xkb_a11y_flags a11y_flags;

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
    int16_t mod_ಕೀಲಿ_ಎಣಿಕೆ[XKB_MAX_MODS];
};

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry *
get_entry_for_mods(ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type *type, xkb_mod_mask_t mods)
{
    ಪ್ರತಿ (darray_size_t i = 0; i < type->ಅಂಕೆ_entries; i++)
        ಯದಿ (entry_is_active(&type->entries[i]) &&
            type->entries[i].mods.mask == mods)
            ಪ್ರತ್ಯಾಗಮ &type->entries[i];
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry *
get_entry_for_ಕೀಲಿ_state(ಸಂರಚನಾ xkb_state *state, ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                        xkb_layout_index_t group)
{
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type* ಧ್ರುವ type = ಕೀಲಿ->groups[group].type;
    xkb_mod_mask_t active_mods = state->components.mods & type->mods.mask;
    ಪ್ರತ್ಯಾಗಮ get_entry_for_mods(type, active_mods);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ xkb_level_index_t
state_ಕೀಲಿ_get_level(ಸಂರಚನಾ xkb_state *state, ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                         xkb_layout_index_t layout)
{
    ಯದಿ (layout >= ಕೀಲಿ->ಅಂಕೆ_groups)
        ಪ್ರತ್ಯಾಗಮ XKB_LEVEL_INVALID;

    /* If we don't find an explicit match the default is 0. */
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry* ಧ್ರುವ entry =
        get_entry_for_ಕೀಲಿ_state(state, ಕೀಲಿ, layout);

    ಪ್ರತ್ಯಾಗಮ (entry) ? entry->level : 0;
}

/**
 * Returns the level to use for the given key and state, or
 * XKB_LEVEL_INVALID.
 */
xkb_level_index_t
xkb_state_ಕೀಲಿ_get_level(ಸಂರಚನಾ xkb_state *state, xkb_keycode_t kc,
                        xkb_layout_index_t layout)
{
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ* ಧ್ರುವ ಕೀಲಿ = Xkb_ಕೀಲಿ(state->keymap, kc);

    ಪ್ರತ್ಯಾಗಮ (ಕೀಲಿ)
        ? state_ಕೀಲಿ_get_level(state, ಕೀಲಿ, layout)
        : XKB_LEVEL_INVALID;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ xkb_layout_index_t
state_ಕೀಲಿ_get_layout(ಸಂರಚನಾ xkb_state *state, ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ)
{
    ಪ್ರತ್ಯಾಗಮ xkb_keymap_ಕೀಲಿ_effective_layout(ಕೀಲಿ, state->components.group);
}

/**
 * Returns the layout to use for the given key and state, taking
 * wrapping/clamping/etc into account, or XKB_LAYOUT_INVALID.
 */
xkb_layout_index_t
xkb_state_ಕೀಲಿ_get_layout(ಸಂರಚನಾ xkb_state *state, xkb_keycode_t kc)
{
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ* ಧ್ರುವ ಕೀಲಿ = Xkb_ಕೀಲಿ(state->keymap, kc);

    ಯದಿ (!ಕೀಲಿ)
        ಪ್ರತ್ಯಾಗಮ XKB_LAYOUT_INVALID;

    ಪ್ರತ್ಯಾಗಮ state_ಕೀಲಿ_get_layout(state, ಕೀಲಿ);
}

/* Empty action used for empty levels */
ಸ್ಥಿರ ಧ್ರುವ ಸಙ್ಘ xkb_action dummy_action = { .type = ACTION_TYPE_NONE };

ಸ್ಥಿರ xkb_action_ಎಣಿಕೆ_t
xkb_ಕೀಲಿ_get_actions(ಸಂರಚನಾ xkb_state *state, ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                    ಧ್ರುವ ಸಙ್ಘ xkb_action **actions)
{
    ಧ್ರುವ xkb_layout_index_t layout = state_ಕೀಲಿ_get_layout(state, ಕೀಲಿ);
    ಧ್ರುವ xkb_level_index_t level = state_ಕೀಲಿ_get_level(state, ಕೀಲಿ, layout);
    ಯದಿ (level == XKB_LEVEL_INVALID)
        ಗಚ್ಛ err;

    ಧ್ರುವ xkb_action_ಎಣಿಕೆ_t ಎಣಿಕೆ =
        xkb_keymap_ಕೀಲಿ_get_actions_by_level(state->keymap, ಕೀಲಿ,
                                            layout, level, actions);
    ಯದಿ (!ಎಣಿಕೆ)
        ಗಚ್ಛ err;

    ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;

err:
    /* Use a dummy action if no corresponding level was found or if it is empty.
     * This is required e.g. to handle latches properly. */
    *actions = &dummy_action;
    ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಸಂರಚನಾ xkb_filter *
xkb_filter_new(ಸಂರಚನಾ xkb_server_state *state)
{
    ಸಂರಚನಾ xkb_filter *filter = ರಿಕ್ತ, *iter;

    darray_foreach(iter, state->filters) {
        ಯದಿ (iter->func)
            ಅನುವರ್ತ;
        /* Use available slot */
        filter = iter;
        ಭಙ್ಗ;
    }

    ಯದಿ (!filter) {
        /* No available slot: resize the filters array */
        darray_resize0(state->filters, darray_size(state->filters) + 1);
        filter = &darray_item(state->filters, darray_size(state->filters) -1);
    }

    filter->refcnt = 1;
    ಪ್ರತ್ಯಾಗಮ filter;
}

/***====================================================================***/

ಗಣನಾ xkb_filter_result {
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
    ಯದಿ ((filter_)->action.group.flags & ACTION_ABSOLUTE_SWITCH)          \
        (state_)->components.component_ = (filter_)->action.group.group; \
    ಅನ್ಯಥಾ                                                                 \
        (state_)->components.component_ += (filter_)->action.group.group

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_group_set_new(ಸಂರಚನಾ xkb_server_state *state,
                         ಸಂರಚನಾ xkb_events *events,
                         ಸಂರಚನಾ xkb_filter *filter)
{
    static_assert(ಪರಿಮಾಣ(state->base.components.base_group) ==
                  ಪರಿಮಾಣ(filter->priv),
                  "Max groups don't fit");
    filter->priv = (uint32_t)state->base.components.base_group;
    apply_group_delta(filter, &state->base, base_group);
}

ಸ್ಥಿರ bool
xkb_filter_group_set_func(ಸಂರಚನಾ xkb_server_state *state,
                          ಸಂರಚನಾ xkb_events *events,
                          ಸಂರಚನಾ xkb_filter *filter,
                          ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                          ಗಣನಾ xkb_ಕೀಲಿ_direction direction)
{
    ಯದಿ (ಕೀಲಿ != filter->ಕೀಲಿ) {
        filter->action.group.flags &= ~ACTION_LOCK_CLEAR;
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;
    }

    ವಿಕಲ್ಪ (direction) {
    ಪಕ್ಷ XKB_ಕೀಲಿ_DOWN:
        filter->refcnt++;
        /* fallthrough */
    ಪಕ್ಷ XKB_ಕೀಲಿ_REPEATED:
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    ಸ್ವಭಾವ:
        ಯದಿ (--filter->refcnt > 0)
            ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    }

    static_assert(ಪರಿಮಾಣ(state->base.components.base_group) == ಪರಿಮಾಣ(filter->priv),
                  "Max groups don't fit");
    state->base.components.base_group = (int32_t) filter->priv;

    ಯದಿ (filter->action.group.flags & ACTION_LOCK_CLEAR)
        state->base.components.locked_group = 0;

    filter->func = ರಿಕ್ತ;
    ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;
}

ಸ್ಥಿರ ಗಣನಾ xkb_state_component
get_state_component_changes(ಧ್ರುವ ಸಂರಚನಾ state_components *a,
                            ಧ್ರುವ ಸಂರಚನಾ state_components *b)
{
    ಗಣನಾ xkb_state_component mask = 0;

    ಯದಿ (a->group != b->group)
        mask |= XKB_STATE_LAYOUT_EFFECTIVE;
    ಯದಿ (a->base_group != b->base_group)
        mask |= XKB_STATE_LAYOUT_DEPRESSED;
    ಯದಿ (a->latched_group != b->latched_group)
        mask |= XKB_STATE_LAYOUT_LATCHED;
    ಯದಿ (a->locked_group != b->locked_group)
        mask |= XKB_STATE_LAYOUT_LOCKED;
    ಯದಿ (a->mods != b->mods)
        mask |= XKB_STATE_MODS_EFFECTIVE;
    ಯದಿ (a->base_mods != b->base_mods)
        mask |= XKB_STATE_MODS_DEPRESSED;
    ಯದಿ (a->latched_mods != b->latched_mods)
        mask |= XKB_STATE_MODS_LATCHED;
    ಯದಿ (a->locked_mods != b->locked_mods)
        mask |= XKB_STATE_MODS_LOCKED;
    ಯದಿ (a->leds != b->leds)
        mask |= XKB_STATE_LEDS;
    ಯದಿ (a->controls != b->controls)
        mask |= XKB_STATE_CONTROLS_EFFECTIVE;

    ಪ್ರತ್ಯಾಗಮ mask;
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_group_lock_new(ಸಂರಚನಾ xkb_server_state *state,
                          ಸಂರಚನಾ xkb_events *events,
                          ಸಂರಚನಾ xkb_filter *filter)
{
    ಯದಿ (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release: do nothing on key press.
         *
         * This is a keymap format v2 extension.
         */
        ಪ್ರತ್ಯಾಗಮ;
    } ಅನ್ಯಥಾ {
        /* Lock on key press */
        apply_group_delta(filter, &state->base, locked_group);
    }
}

ಸ್ಥಿರ bool
xkb_filter_group_lock_func(ಸಂರಚನಾ xkb_server_state *state,
                           ಸಂರಚನಾ xkb_events *events,
                           ಸಂರಚನಾ xkb_filter *filter,
                           ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                           ಗಣನಾ xkb_ಕೀಲಿ_direction direction)
{
    ಯದಿ (ಕೀಲಿ != filter->ಕೀಲಿ) {
        ಯದಿ ((filter->action.group.flags & ACTION_LOCK_ON_RELEASE) &&
            direction == XKB_ಕೀಲಿ_DOWN) {
            /*
             * Another key has been pressed after the locking key:
             * cancel group lock on release.
             *
             * This is a keymap v2 extension.
             */
            filter->action.group.flags &= ~ACTION_LOCK_ON_RELEASE;
        }
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;
    }

    ವಿಕಲ್ಪ (direction) {
    ಪಕ್ಷ XKB_ಕೀಲಿ_DOWN:
        filter->refcnt++;
        /* fallthrough */
    ಪಕ್ಷ XKB_ಕೀಲಿ_REPEATED:
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    ಸ್ವಭಾವ:
        ಯದಿ (--filter->refcnt > 0)
            ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    }

    ಯದಿ (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release
         *
         * This is a keymap v2 extension.
         */
        apply_group_delta(filter, &state->base, locked_group);
    } ಅನ್ಯಥಾ {
        /* Lock on key press: do nothing on key release. */
    }
    filter->func = ರಿಕ್ತ;
    ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;
}

ಸ್ಥಿರ bool
xkb_action_breaks_latch(ಧ್ರುವ ಸಙ್ಘ xkb_action *action,
                        ಗಣನಾ xkb_action_controls controls,
                        ಗಣನಾ xkb_internal_action_flags flag,
                        xkb_mod_mask_t mask)
{
    ವಿಕಲ್ಪ (action->type) {
    ಪಕ್ಷ ACTION_TYPE_NONE:
    ಪಕ್ಷ ACTION_TYPE_VOID:
    ಪಕ್ಷ ACTION_TYPE_PTR_BUTTON:
    ಪಕ್ಷ ACTION_TYPE_PTR_LOCK:
    ಪಕ್ಷ ACTION_TYPE_CTRL_SET:
    ಪಕ್ಷ ACTION_TYPE_CTRL_LOCK:
    ಪಕ್ಷ ACTION_TYPE_SWITCH_VT:
    ಪಕ್ಷ ACTION_TYPE_TERMINATE:
    ಪಕ್ಷ ACTION_TYPE_REDIRECT_ಕೀಲಿ:
        ಪ್ರತ್ಯಾಗಮ true;
    ಪಕ್ಷ ACTION_TYPE_PTR_MOVE:
    ಪಕ್ಷ ACTION_TYPE_PTR_DEFAULT:
        ಪ್ರತ್ಯಾಗಮ !(controls & CONTROL_MOUSE_ಕೀಲಿಗಳು);
    ಪಕ್ಷ ACTION_TYPE_INTERNAL:
        ಪ್ರತ್ಯಾಗಮ (action->internal.flags & flag) &&
               ((action->internal.clear_latched_mods & mask) == mask);
    ಸ್ವಭಾವ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_ಅಂಕೆ_ENTRIES,
                      "Missing action type");
        ಪ್ರತ್ಯಾಗಮ false;
    }
}

ಗಣನಾ xkb_ಕೀಲಿ_latch_state {
    NO_LATCH = 0,
    LATCH_ಕೀಲಿ_DOWN,
    LATCH_PENDING,
    ಕೀಲಿ_LATCH_STATE_ಅಂಕೆ_ENTRIES
};

#define MAX_XKB_ಕೀಲಿ_LATCH_STATE_LOG2 2
#ಯದಿ (ಕೀಲಿ_LATCH_STATE_ಅಂಕೆ_ENTRIES > (1 << MAX_XKB_ಕೀಲಿ_LATCH_STATE_LOG2)) || \
    (-XKB_MAX_GROUPS) < (INT32_MIN >> MAX_XKB_ಕೀಲಿ_LATCH_STATE_LOG2) || \
      XKB_MAX_GROUPS > (INT32_MAX >> MAX_XKB_ಕೀಲಿ_LATCH_STATE_LOG2)
#error "Cannot represent priv field of the group latch filter"
#endif

/* Hold the latch state *and* the group delta */
ಸಙ್ಘ group_latch_priv {
    uint32_t priv;
    ಸಂರಚನಾ {
        /* The type is really: enum xkb_key_latch_state, but it is problematic
         * on Windows, because it is interpreted as signed and leads to wrong
         * negative values. */
        unsigned ಪೂರ್ಣಾಙ್ಕ latch:MAX_XKB_ಕೀಲಿ_LATCH_STATE_LOG2;
        int32_t group_delta:(32 - MAX_XKB_ಕೀಲಿ_LATCH_STATE_LOG2);
    };
};

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_group_latch_new(ಸಂರಚನಾ xkb_server_state *state,
                           ಸಂರಚನಾ xkb_events *events,
                           ಸಂರಚನಾ xkb_filter *filter)
{
    ಧ್ರುವ ಸಙ್ಘ group_latch_priv priv = {
        .latch = LATCH_ಕೀಲಿ_DOWN,
        .group_delta = (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)
            ? filter->action.group.group - state->base.components.base_group
            : filter->action.group.group
    };
    filter->priv = priv.priv;
    /* Like group set */
    apply_group_delta(filter, &state->base, base_group);
}

ಸ್ಥಿರ bool
xkb_filter_group_latch_func(ಸಂರಚನಾ xkb_server_state *state,
                            ಸಂರಚನಾ xkb_events *events,
                            ಸಂರಚನಾ xkb_filter *filter,
                            ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                            ಗಣನಾ xkb_ಕೀಲಿ_direction direction)
{
    ಸಙ್ಘ group_latch_priv priv = {.priv = filter->priv};
    ಗಣನಾ xkb_ಕೀಲಿ_latch_state latch = priv.latch;

    ಯದಿ (direction == XKB_ಕೀಲಿ_DOWN) {
        ಧ್ರುವ ಸಙ್ಘ xkb_action *actions = ರಿಕ್ತ;
        ಧ್ರುವ xkb_action_ಎಣಿಕೆ_t ಎಣಿಕೆ = xkb_ಕೀಲಿ_get_actions(&state->base, ಕೀಲಿ,
                                                             &actions);

        ಯದಿ (latch == LATCH_ಕೀಲಿ_DOWN) {
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
            ಯದಿ (state->a11y_flags & XKB_A11_Y_LATCH_SIMULTANEOUS_ಕೀಲಿಗಳು) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                ಪ್ರತಿ (xkb_action_ಎಣಿಕೆ_t k = 0; k < ಎಣಿಕೆ; k++) {
                    ಯದಿ (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_GROUP_LATCH,
                                                0))
                    {
                        latch = NO_LATCH;
                        ಭಙ್ಗ;
                    }
                }
            } ಅನ್ಯಥಾ {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        ಅನ್ಯಥಾ ಯದಿ (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is
             * pressed, or promote it to a lock if it's the same group delta &
             * flags and latchToLock option is enabled. */
            ಧ್ರುವ bool sticky_ಕೀಲಿಗಳು = state->base.components.controls
                                   & CONTROL_STICKY_ಕೀಲಿಗಳು;
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
            ಪ್ರತಿ (xkb_action_ಎಣಿಕೆ_t k = 0; k < ಎಣಿಕೆ; k++) {
                ಯದಿ (((actions[k].type == ACTION_TYPE_GROUP_LATCH) ||
                     (actions[k].type == ACTION_TYPE_GROUP_SET && sticky_ಕೀಲಿಗಳು)) &&
                     actions[k].group.group == filter->action.group.group &&
                     (actions[k].group.flags & ACTION_ABSOLUTE_SWITCH) ==
                     (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)) {
                    ಯದಿ (filter->action.group.flags & ACTION_LATCH_TO_LOCK &&
                        filter->action.group.group != 0)
                    {
                        /* Promote to lock */
                        filter->action.type = ACTION_TYPE_GROUP_LOCK;
                        filter->func = xkb_filter_group_lock_func;
                        xkb_filter_group_lock_new(state, events, filter);
                        state->base.components.latched_group -= priv.group_delta;
                        filter->ಕೀಲಿ = ಕೀಲಿ;
                        /* XXX beep beep! */
                        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
                    }
                    /* Do nothing if `latchToLock` option is not activated; if
                     * the latch is not broken by the following actions and the
                     * key is not consumed, then another latch filter will be
                     * created.
                     */
                    ಅನುವರ್ತ;
                }
                ಅನ್ಯಥಾ ಯದಿ (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_GROUP_LATCH,
                                                 0))
                {
                    /* Breaks the latch */
                    state->base.components.latched_group -= priv.group_delta;
                    filter->func = ರಿಕ್ತ;
                    ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;
                }
            }
        }
        ಅನ್ಯಥಾ {
            /* Ignore press in NO_LATCH state */
            ಪ್ರತಿಜ್ಞಾ(latch == NO_LATCH);
        }
    }
    ಅನ್ಯಥಾ ಯದಿ (ಕೀಲಿ != filter->ಕೀಲಿ) {
        /* Ignore repeat/release of other keys */
    }
    ಅನ್ಯಥಾ ಯದಿ (direction == XKB_ಕೀಲಿ_REPEATED) {
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME; // TODO: check
    }
    ಅನ್ಯಥಾ {
        /* Our key got released.  If we've set it to clear locks, and we
         * currently have a group locked, then release it and
         * don't actually latch.  Else we've actually hit the latching
         * stage, so set PENDING and move our group from base to
         * latched. */
        ಯದಿ ((filter->action.group.flags & ACTION_LOCK_CLEAR) &&
             state->base.components.locked_group) {
            ಯದಿ (latch == LATCH_PENDING)
                state->base.components.latched_group -= priv.group_delta;
            ಅನ್ಯಥಾ
                state->base.components.base_group -= priv.group_delta;
            state->base.components.locked_group = 0;
            filter->func = ರಿಕ್ತ;
        }
        /* Broken latch */
        ಅನ್ಯಥಾ ಯದಿ (latch == NO_LATCH) {
            state->base.components.base_group -= priv.group_delta;
            filter->func = ರಿಕ್ತ;
        }
        /* We may already have reached the latch state if pressing the
         * key multiple times without latch-to-lock enabled. */
        ಅನ್ಯಥಾ ಯದಿ (latch == LATCH_ಕೀಲಿ_DOWN) {
            latch = LATCH_PENDING;
            /* Switch from set to latch */
            state->base.components.base_group -= priv.group_delta;
            state->base.components.latched_group += priv.group_delta;
            /* XXX beep beep! */
        }
    }

    priv.latch = latch;
    filter->priv = priv.priv;

    ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_mod_set_new(ಸಂರಚನಾ xkb_server_state *state,
                       ಸಂರಚನಾ xkb_events *events,
                       ಸಂರಚನಾ xkb_filter *filter)
{
    ಧ್ರುವ ಗಣನಾ xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    ಯದಿ ((filter->action.mods.flags & unlock) == unlock) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension.
         */
        filter->priv = filter->action.mods.mods.mask
                     & ~state->base.components.locked_mods;
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
    } ಅನ್ಯಥಾ {
        filter->priv = filter->action.mods.mods.mask;
    }

    state->set_mods |= (xkb_mod_mask_t) filter->priv;
}

ಸ್ಥಿರ bool
xkb_filter_mod_set_func(ಸಂರಚನಾ xkb_server_state *state,
                        ಸಂರಚನಾ xkb_events *events,
                        ಸಂರಚನಾ xkb_filter *filter,
                        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                        ಗಣನಾ xkb_ಕೀಲಿ_direction direction)
{

    ಯದಿ (ಕೀಲಿ != filter->ಕೀಲಿ) {
        filter->action.mods.flags &= ~ACTION_LOCK_CLEAR;
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;
    }

    ವಿಕಲ್ಪ (direction) {
    ಪಕ್ಷ XKB_ಕೀಲಿ_DOWN:
        filter->refcnt++;
        /* fallthrough */
    ಪಕ್ಷ XKB_ಕೀಲಿ_REPEATED:
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    ಸ್ವಭಾವ:
        ಯದಿ (--filter->refcnt > 0)
            ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    }

    state->clear_mods |= (xkb_mod_mask_t) filter->priv;
    ಧ್ರುವ ಗಣನಾ xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    ಯದಿ ((filter->action.mods.flags & unlock) == ACTION_LOCK_CLEAR)
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;

    filter->func = ರಿಕ್ತ;
    ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_mod_lock_new(ಸಂರಚನಾ xkb_server_state *state,
                        ಸಂರಚನಾ xkb_events *events,
                        ಸಂರಚನಾ xkb_filter *filter)
{
    filter->priv = (state->base.components.locked_mods &
                    filter->action.mods.mods.mask);

    ಯದಿ (filter->priv && (filter->action.mods.flags & ACTION_UNLOCK_ON_PRESS)) {
        /*
         * Some of the target modifiers were locked before key press: unlock.
         *
         * This is a keymap v2 extension: unlock-on-press.
         */
        ಯದಿ (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
            state->base.components.locked_mods &= ~filter->priv;
        /* No further action: cancel filter */
        filter->func = ರಿಕ್ತ;
    } ಅನ್ಯಥಾ {
        /* Set base mods; lock mods if relevant (XKB 1.0 spec) */
        state->set_mods |= filter->action.mods.mods.mask;
        ಯದಿ (!(filter->action.mods.flags & ACTION_LOCK_NO_LOCK))
            state->base.components.locked_mods |= filter->action.mods.mods.mask;
    }
}

ಸ್ಥಿರ bool
xkb_filter_mod_lock_func(ಸಂರಚನಾ xkb_server_state *state,
                         ಸಂರಚನಾ xkb_events *events,
                         ಸಂರಚನಾ xkb_filter *filter,
                         ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                         ಗಣನಾ xkb_ಕೀಲಿ_direction direction)
{
    ಯದಿ (ಕೀಲಿ != filter->ಕೀಲಿ)
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;

    ವಿಕಲ್ಪ (direction) {
    ಪಕ್ಷ XKB_ಕೀಲಿ_DOWN:
        filter->refcnt++;
        /* fallthrough */
    ಪಕ್ಷ XKB_ಕೀಲಿ_REPEATED:
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    ಸ್ವಭಾವ:
        ಯದಿ (--filter->refcnt > 0)
            ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    }

    state->clear_mods |= filter->action.mods.mods.mask;
    ಯದಿ (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
        state->base.components.locked_mods &= ~filter->priv;

    filter->func = ರಿಕ್ತ;
    ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_mod_latch_new(ಸಂರಚನಾ xkb_server_state *state,
                         ಸಂರಚನಾ xkb_events *events,
                         ಸಂರಚನಾ xkb_filter *filter)
{
    /* Latch-on-press + clear-locks imply unlock-on-press */
    ಧ್ರುವ ಗಣನಾ xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                              | ACTION_LATCH_ON_PRESS;

    ಯದಿ ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
        (filter->action.mods.flags & unlockOnPress) &&
        (state->base.components.locked_mods & filter->action.mods.mods.mask) ==
         filter->action.mods.mods.mask) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension: clear locks and do not latch.
         */
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
        filter->func = ರಿಕ್ತ;
    } ಅನ್ಯಥಾ ಯದಿ (filter->action.mods.flags & ACTION_LATCH_ON_PRESS) {
        /*
         * Latch on key press
         *
         * This is a keymap format v2 extension.
         */
        filter->priv = LATCH_PENDING;
        state->base.components.latched_mods |= filter->action.mods.mods.mask;
        /* XXX beep beep! */
    } ಅನ್ಯಥಾ {
        /* XKB standard latch action */
        filter->priv = LATCH_ಕೀಲಿ_DOWN;
        state->set_mods |= filter->action.mods.mods.mask;
    }
}

ಸ್ಥಿರ bool
xkb_filter_mod_latch_func(ಸಂರಚನಾ xkb_server_state *state,
                          ಸಂರಚನಾ xkb_events *events,
                          ಸಂರಚನಾ xkb_filter *filter,
                          ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                          ಗಣನಾ xkb_ಕೀಲಿ_direction direction)
{
    ಗಣನಾ xkb_ಕೀಲಿ_latch_state latch = filter->priv;

    ಯದಿ (direction == XKB_ಕೀಲಿ_DOWN) {
        ಧ್ರುವ ಸಙ್ಘ xkb_action *actions = ರಿಕ್ತ;
        ಧ್ರುವ xkb_action_ಎಣಿಕೆ_t ಎಣಿಕೆ = xkb_ಕೀಲಿ_get_actions(&state->base, ಕೀಲಿ,
                                                             &actions);

        ಯದಿ (latch == LATCH_ಕೀಲಿ_DOWN) {
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
            ಯದಿ (state->a11y_flags & XKB_A11_Y_LATCH_SIMULTANEOUS_ಕೀಲಿಗಳು) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                ಪ್ರತಿ (xkb_action_ಎಣಿಕೆ_t k = 0; k < ಎಣಿಕೆ; k++) {
                    ಯದಿ (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_MOD_LATCH,
                                                filter->action.mods.mods.mask))
                    {
                        latch = NO_LATCH;
                        ಭಙ್ಗ;
                    }
                }
            } ಅನ್ಯಥಾ {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        ಅನ್ಯಥಾ ಯದಿ (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is pressed,
             * or promote it to a lock or plain base set if it's the same
             * modifier. */
            ಧ್ರುವ bool sticky_ಕೀಲಿಗಳು = state->base.components.controls
                                   & CONTROL_STICKY_ಕೀಲಿಗಳು;
            /*
             * NOTE: Action’s flags are not compared:
             * - flags may have been modified by keyboard controls at the
             *   creation of the filter;
             * - it is unlikely to have two LatchMods() actions on the same
             *   key that differ only by their flags;
             * - even if it were the case, one would most likely want both
             *   actions to be considered compatible in this context.
             */
            ಪ್ರತಿ (xkb_action_ಎಣಿಕೆ_t k = 0; k < ಎಣಿಕೆ; k++) {
                ಯದಿ (((actions[k].type == ACTION_TYPE_MOD_LATCH) ||
                     (actions[k].type == ACTION_TYPE_MOD_SET && sticky_ಕೀಲಿಗಳು)) &&
                    actions[k].mods.mods.mask == filter->action.mods.mods.mask)
                {
                    ಯದಿ (filter->action.mods.flags & ACTION_LATCH_TO_LOCK) {
                        /* Mutate the action to LockMods() */
                        filter->action.type = ACTION_TYPE_MOD_LOCK;
                        filter->func = xkb_filter_mod_lock_func;
                        xkb_filter_mod_lock_new(state, events, filter);
                    }
                    ಅನ್ಯಥಾ {
                        /* Mutate the action to SetMods() */
                        filter->action.type = ACTION_TYPE_MOD_SET;
                        filter->func = xkb_filter_mod_set_func;
                        xkb_filter_mod_set_new(state, events, filter);
                    }
                    filter->ಕೀಲಿ = ಕೀಲಿ;
                    /* Clear latches */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    /* XXX beep beep! */
                    ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
                }
                ಅನ್ಯಥಾ ಯದಿ (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_MOD_LATCH,
                                                 filter->action.mods.mods.mask))
                {
                    /* XXX: This may be totally broken, we might need to break the
                     *      latch in the next run after this press? */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    filter->func = ರಿಕ್ತ;
                    ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;
                }
            }
        }
        ಅನ್ಯಥಾ {
            /* Ignore press in NO_LATCH state */
            ಪ್ರತಿಜ್ಞಾ(latch == NO_LATCH);
        }
    }
    ಅನ್ಯಥಾ ಯದಿ (ಕೀಲಿ != filter->ಕೀಲಿ) {
        /* Ignore repeat/release of other keys */
    }
    ಅನ್ಯಥಾ ಯದಿ (direction == XKB_ಕೀಲಿ_REPEATED) {
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME; // TODO: check
    }
    ಅನ್ಯಥಾ {
        /* Our key got released.  If we’ve set it to clear locks, and we
         * currently have the same modifiers locked, then release them and
         * don't actually latch.  Else we’ve actually hit the latching
         * stage, so set PENDING and move our modifier from base to
         * latched. */

        /* Latch-on-press + clear-locks imply unlock-on-press */
        ಧ್ರುವ ಗಣನಾ xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                                  | ACTION_LATCH_ON_PRESS;

        ಯದಿ ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
            !(filter->action.mods.flags & unlockOnPress) &&
            (state->base.components.locked_mods &
             filter->action.mods.mods.mask) == filter->action.mods.mods.mask)
        {
            /* XXX: We might be a bit overenthusiastic about clearing
             *      mods other filters have set here? */
            ಯದಿ (latch == LATCH_PENDING)
                state->base.components.latched_mods &=
                    ~filter->action.mods.mods.mask;
            ಅನ್ಯಥಾ
                state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
            filter->func = ರಿಕ್ತ;
        }
        ಅನ್ಯಥಾ ಯದಿ (latch == NO_LATCH) {
            /* Broken latch */
            state->clear_mods |= filter->action.mods.mods.mask;
            filter->func = ರಿಕ್ತ;
        }
        ಅನ್ಯಥಾ ಯದಿ (!(filter->action.mods.flags & ACTION_LATCH_ON_PRESS)) {
            latch = LATCH_PENDING;
            state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.latched_mods |= filter->action.mods.mods.mask;
            /* XXX beep beep! */
        }
    }

    filter->priv = latch;

    ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;
}

ಸ್ಥಿರ ಶೂನ್ಯ
append_pointer_move(ಸಂರಚನಾ xkb_events *events,
                    ಸಂರಚನಾ xkb_filter *filter)
{
    ಗಣನಾ xkb_pointer_motion_flags flags = 0;
    ಯದಿ (filter->action.ptr.flags & ACTION_ABSOLUTE_X)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_X;
    ಯದಿ (filter->action.ptr.flags & ACTION_ABSOLUTE_Y)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_Y;
    ಯದಿ (filter->action.ptr.flags & ACTION_REPEAT)
        flags |= XKB_POINTER_MOTION_REPEATS;
    darray_append(events->queue, (ಸಂರಚನಾ xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_MOTION,
        .pointer_motion = {
            .size = ಪರಿಮಾಣ(((ಸಂರಚನಾ xkb_event*)0)->pointer_motion),
            .flags = flags,
            .x = filter->action.ptr.x,
            .y = filter->action.ptr.y,
        }
    });
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_pointer_move_new(ಸಂರಚನಾ xkb_server_state *state,
                            ಸಂರಚನಾ xkb_events *events,
                            ಸಂರಚನಾ xkb_filter *filter)
{
    ಯದಿ (!events) {
        filter->func = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಪ್ರತಿಜ್ಞಾ(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ಕೀಲಿಗಳು);
    append_pointer_move(events, filter);
    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;
}

ಸ್ಥಿರ bool
xkb_filter_pointer_move_func(ಸಂರಚನಾ xkb_server_state *state,
                             ಸಂರಚನಾ xkb_events *events,
                             ಸಂರಚನಾ xkb_filter *filter,
                             ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                             ಗಣನಾ xkb_ಕೀಲಿ_direction direction)
{
    ಯದಿ (ಕೀಲಿ != filter->ಕೀಲಿ || !events)
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;

    ವಿಕಲ್ಪ (direction) {
    ಪಕ್ಷ XKB_ಕೀಲಿ_DOWN:
        filter->refcnt++;
        append_pointer_move(events, filter);
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    ಪಕ್ಷ XKB_ಕೀಲಿ_REPEATED:
        ಯದಿ (filter->action.ptr.flags & ACTION_REPEAT)
            append_pointer_move(events, filter);
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    ಸ್ವಭಾವ:
        ಯದಿ (--filter->refcnt > 0)
            ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    }

    filter->func = ರಿಕ್ತ;
    ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
}

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_pointer_button_new(ಸಂರಚನಾ xkb_server_state *state,
                              ಸಂರಚನಾ xkb_events *events,
                              ಸಂರಚನಾ xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ಸ್ಥಿರ bool
xkb_filter_pointer_button_func(ಸಂರಚನಾ xkb_server_state *state,
                               ಸಂರಚನಾ xkb_events *events,
                               ಸಂರಚನಾ xkb_filter *filter,
                               ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                               ಗಣನಾ xkb_ಕೀಲಿ_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_pointer_lock_button_new(ಸಂರಚನಾ xkb_server_state *state,
                                   ಸಂರಚನಾ xkb_events *events,
                                   ಸಂರಚನಾ xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ಸ್ಥಿರ bool
xkb_filter_pointer_lock_button_func(ಸಂರಚನಾ xkb_server_state *state,
                                    ಸಂರಚನಾ xkb_events *events,
                                    ಸಂರಚನಾ xkb_filter *filter,
                                    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                                    ಗಣನಾ xkb_ಕೀಲಿ_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_pointer_set_default_button_new(ಸಂರಚನಾ xkb_server_state *state,
                                          ಸಂರಚನಾ xkb_events *events,
                                          ಸಂರಚನಾ xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ಸ್ಥಿರ bool
xkb_filter_pointer_set_default_button_func(ಸಂರಚನಾ xkb_server_state *state,
                                           ಸಂರಚನಾ xkb_events *events,
                                           ಸಂರಚನಾ xkb_filter *filter,
                                           ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                                           ಗಣನಾ xkb_ಕೀಲಿ_direction direction);

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_terminate_server_new(ಸಂರಚನಾ xkb_server_state *state,
                                 ಸಂರಚನಾ xkb_events *events,
                                 ಸಂರಚನಾ xkb_filter *filter);

ಸ್ಥಿರ bool
xkb_filter_terminate_server_func(ಸಂರಚನಾ xkb_server_state *state,
                                  ಸಂರಚನಾ xkb_events *events,
                                  ಸಂರಚನಾ xkb_filter *filter,
                                  ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                                  ಗಣನಾ xkb_ಕೀಲಿ_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_switch_virtual_console_new(ಸಂರಚನಾ xkb_server_state *state,
                                      ಸಂರಚನಾ xkb_events *events,
                                      ಸಂರಚನಾ xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ಸ್ಥಿರ bool
xkb_filter_switch_virtual_console_func(ಸಂರಚನಾ xkb_server_state *state,
                                       ಸಂರಚನಾ xkb_events *events,
                                       ಸಂರಚನಾ xkb_filter *filter,
                                       ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                                       ಗಣನಾ xkb_ಕೀಲಿ_direction direction);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
clear_all_latches_and_locks(ಸಂರಚನಾ xkb_server_state *state,
                            ಸಂರಚನಾ xkb_events *events);

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_ctrls_new(ಸಂರಚನಾ xkb_server_state *state,
                     ಸಂರಚನಾ xkb_events *events,
                     ಸಂರಚನಾ xkb_filter *filter)
{
    ಯದಿ (filter->action.type == ACTION_TYPE_CTRL_SET) {
        /* Set: save the specified controls that are *not* already enabled */
        filter->priv = (uint32_t) (
            (~state->base.components.controls) & filter->action.ctrls.ctrls
        );
    } ಅನ್ಯಥಾ {
        /* Lock: save the specified controls that *are* already enabled */
        filter->priv = (uint32_t) (
            state->base.components.controls & filter->action.ctrls.ctrls
        );
    }

    ಯದಿ (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_LOCK)) {
        /* Enable the specified controls that are not already enabled */
        state->base.components.controls |= filter->action.ctrls.ctrls;
    }
}

ಸ್ಥಿರ bool
xkb_filter_ctrls_func(ಸಂರಚನಾ xkb_server_state *state,
                      ಸಂರಚನಾ xkb_events *events,
                      ಸಂರಚನಾ xkb_filter *filter,
                      ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                      ಗಣನಾ xkb_ಕೀಲಿ_direction direction)
{
    ಯದಿ (ಕೀಲಿ != filter->ಕೀಲಿ)
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;

    ವಿಕಲ್ಪ (direction) {
    ಪಕ್ಷ XKB_ಕೀಲಿ_DOWN:
        filter->refcnt++;
        /* fallthrough */
    ಪಕ್ಷ XKB_ಕೀಲಿ_REPEATED:
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    ಸ್ವಭಾವ:
        ಯದಿ (--filter->refcnt > 0)
            ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    }

    ಯದಿ (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_UNLOCK)) {
        ಧ್ರುವ ಗಣನಾ xkb_action_controls old = state->base.components.controls;

        /*
         * Set: Disable specified controls that were *not* enabled at key press.
         * Lock: Disable specified controls that *were* enabled at key press.
         */
        state->base.components.controls &= ~(ಗಣನಾ xkb_action_controls) filter->priv;

        ಯದಿ ((old & CONTROL_STICKY_ಕೀಲಿಗಳು) &&
            !(state->base.components.controls & CONTROL_STICKY_ಕೀಲಿಗಳು)) {
            /* Sticky keys were disabled: clear all locks and latches */
            clear_all_latches_and_locks(state, events);
        }
    }

    filter->func = ರಿಕ್ತ;
    ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;
}

ಸ್ಥಿರ bool
append_redirect_ಕೀಲಿ_events(ಸಂರಚನಾ xkb_server_state *state,
                           ಸಂರಚನಾ xkb_events *events,
                           ಧ್ರುವ ಸಂರಚನಾ xkb_redirect_ಕೀಲಿ_action *redirect,
                           ಗಣನಾ xkb_ಕೀಲಿ_direction direction)
{
    ಗಣನಾ xkb_state_component changed = 0;
    ಧ್ರುವ xkb_mod_mask_t mask = redirect->affect_mods;

    /*
     * Reference state: find the last state update in the queue, otherwise
     * use the current state.
     */
    ಸಂರಚನಾ xkb_event *event;
    ಸಂರಚನಾ state_components last_components = state->base.components;
    darray_foreach_reverse(event, events->queue) {
        ಯದಿ (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
            last_components = event->components.components;
            ಭಙ್ಗ;
        }
    }

    ಯದಿ (mask) {
        ಸಂರಚನಾ state_components new = last_components;
        new.base_mods = (new.base_mods & ~mask) | redirect->mods;
        new.latched_mods = (new.latched_mods & ~mask) | redirect->mods;
        new.locked_mods = (new.locked_mods & ~mask) | redirect->mods;
        new.mods = (new.mods & ~mask) | redirect->mods;

        changed = get_state_component_changes(&last_components, &new);
        ಯದಿ (changed) {
            darray_append(events->queue, (ಸಂರಚನಾ xkb_event) {
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
            darray_append(events->queue, (ಸಂರಚನಾ xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_FRAME,
            });
        }
    }

    darray_append(events->queue, (ಸಂರಚನಾ xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_ಕೀಲಿ,
        .ಕೀಲಿ = {
            .keycode = redirect->keycode,
            .direction = direction
        }
    });

    ಯದಿ (mask && changed) {
        /* Restore state */
        darray_append(events->queue, (ಸಂರಚನಾ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = last_components,
                .changed = changed
            }
        });
    }

    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_redirect_ಕೀಲಿ_new(ಸಂರಚನಾ xkb_server_state *state,
                            ಸಂರಚನಾ xkb_events *events,
                            ಸಂರಚನಾ xkb_filter *filter)
{
    /* Action effectual only with the xkb_machine API and a valid keycode */
    ಯದಿ (!events || filter->action.redirect.keycode == XKB_KEYCODE_INVALID) {
        filter->func = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ;
    }
    append_redirect_ಕೀಲಿ_events(state, events, &filter->action.redirect,
                               XKB_ಕೀಲಿ_DOWN);
}

ಸ್ಥಿರ bool
xkb_filter_redirect_ಕೀಲಿ_func(ಸಂರಚನಾ xkb_server_state *state,
                             ಸಂರಚನಾ xkb_events *events,
                             ಸಂರಚನಾ xkb_filter *filter,
                             ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                             ಗಣನಾ xkb_ಕೀಲಿ_direction direction)
{
    ಯದಿ (ಕೀಲಿ != filter->ಕೀಲಿ)
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;

    ಯದಿ (direction == XKB_ಕೀಲಿ_UP) {
        append_redirect_ಕೀಲಿ_events(state, events,
                                   &filter->action.redirect, XKB_ಕೀಲಿ_UP);
        filter->func = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
     } ಅನ್ಯಥಾ ಯದಿ (direction == XKB_ಕೀಲಿ_DOWN) {
        ಧ್ರುವ ಸಙ್ಘ xkb_action *actions = ರಿಕ್ತ;
        ಧ್ರುವ xkb_action_ಎಣಿಕೆ_t ಎಣಿಕೆ =
            xkb_ಕೀಲಿ_get_actions(&state->base, ಕೀಲಿ, &actions);

        ಪ್ರತಿ (xkb_action_ಎಣಿಕೆ_t a = 0; a < ಎಣಿಕೆ; a++) {
            ಯದಿ (actions[a].type == ACTION_TYPE_REDIRECT_ಕೀಲಿ &&
                actions[a].redirect.keycode != filter->action.redirect.keycode) {
                    /* One action redirects to a different key: release first */
                    append_redirect_ಕೀಲಿ_events(state, events,
                                               &filter->action.redirect,
                                               XKB_ಕೀಲಿ_UP);
                filter->func = ರಿಕ್ತ;
                ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;
            }
        }
    }

    append_redirect_ಕೀಲಿ_events(state, events, &filter->action.redirect,
                               direction);
    ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ {
    ಶೂನ್ಯ (*new)(ಸಂರಚನಾ xkb_server_state *state,
                ಸಂರಚನಾ xkb_events *events,
                ಸಂರಚನಾ xkb_filter *filter);
    bool (*func)(ಸಂರಚನಾ xkb_server_state *state,
                 ಸಂರಚನಾ xkb_events *events,
                 ಸಂರಚನಾ xkb_filter *filter,
                 ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ, ಗಣನಾ xkb_ಕೀಲಿ_direction direction);
} filter_action_funcs[ACTION_TYPE_ಅಂಕೆ_ENTRIES] = {
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
    [ACTION_TYPE_REDIRECT_ಕೀಲಿ] = { xkb_filter_redirect_ಕೀಲಿ_new,
                                   xkb_filter_redirect_ಕೀಲಿ_func },
};

/**
 * Applies any relevant filters to the key, first from the list of filters
 * that are currently active, then if no filter has claimed the key, possibly
 * apply a new filter from the key action.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_apply_all(ಸಂರಚನಾ xkb_server_state *state,
                     ಸಂರಚನಾ xkb_events *events,
                     ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                     ಗಣನಾ xkb_ಕೀಲಿ_direction direction)
{
    /* First run through all the currently active filters and see if any of
     * them have consumed this event. */
    bool consumed = false;
    ಸಂರಚನಾ xkb_filter *filter;
    darray_foreach(filter, state->filters) {
        ಯದಿ (!filter->func)
            ಅನುವರ್ತ;

        ಯದಿ (filter->func(state, events, filter, ಕೀಲಿ, direction) == XKB_FILTER_CONSUME)
            consumed = true;
    }
    ಯದಿ (consumed || direction == XKB_ಕೀಲಿ_UP)
        ಪ್ರತ್ಯಾಗಮ;

    /* No filter consumed this event, so proceed with the key actions */
    ಧ್ರುವ ಸಙ್ಘ xkb_action *actions = ರಿಕ್ತ;
    xkb_action_ಎಣಿಕೆ_t ಎಣಿಕೆ = xkb_ಕೀಲಿ_get_actions(&state->base, ಕೀಲಿ,
                                                         &actions);

    ಪ್ರತಿ (ಸಂರಚನಾ {
            xkb_action_ಎಣಿಕೆ_t ಎಣಿಕೆ;
            ಧ್ರುವ ಸಙ್ಘ xkb_action *actions;
         } next = {0};
         ಎಣಿಕೆ;
         ಎಣಿಕೆ = next.ಎಣಿಕೆ, next.ಎಣಿಕೆ = 0, actions = next.actions
    ) {
        /*
         * Process actions sequentially.
         *
         * NOTE: We rely on the parser to disallow multiple modifier or group
         * actions (see `CheckMultipleActionsCategories`). Allowing multiple
         * such actions requires a refactor of the state handling.
         */
        ಪ್ರತಿ (xkb_action_ಎಣಿಕೆ_t k = 0; k < ಎಣಿಕೆ; k++) {
            /*
             * It's possible for the keymap to set action->type explicitly, like
             * so:
             *     interpret XF86_Next_VMode {
             *         action = Private(type=0x86, data="+VMode");
             *     };
             * We don't handle those.
             */
            ಯದಿ (actions[k].type >= ACTION_TYPE_ಅಂಕೆ_ENTRIES)
                ಅನುವರ್ತ;

            /* Go to next action if no corresponding action handler */
            ಯದಿ (!filter_action_funcs[actions[k].type].new)
                ಅನುವರ್ತ;

            /* Add a new filter and run the corresponding initial action */
            filter = xkb_filter_new(state);
            filter->ಕೀಲಿ = ಕೀಲಿ;
            filter->action = actions[k];

            ವಿಕಲ್ಪ (filter->action.type) {
            ಪಕ್ಷ ACTION_TYPE_MOD_SET:
                ಯದಿ (state->base.components.controls & CONTROL_STICKY_ಕೀಲಿಗಳು) {
                    /* Convert modifier set action to a latch */
                    filter->action.type = ACTION_TYPE_MOD_LATCH;
                    ಯದಿ (state->a11y_flags & XKB_A11_Y_STICKY_ಕೀಲಿಗಳು_LATCH_TO_LOCK) {
                        filter->action.mods.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                /* fallthrough */
            ಪಕ್ಷ ACTION_TYPE_MOD_LATCH:
            ಪಕ್ಷ ACTION_TYPE_MOD_LOCK: {
                ಸ್ಥಿರ ಧ್ರುವ ಗಣನಾ xkb_action_flags unlock_mods_on_press
                    = ACTION_LOCK_CLEAR     /* SetMods & LatchMods: can unlock */
                    | ACTION_LOCK_NO_UNLOCK /* LockMods: cannot unlock */
                    | ACTION_EXPLICIT_UNLOCK_ON_PRESS;
                ಯದಿ ((state->flags & STATE_UNLOCK_MODS_ON_PRESS) &&
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
                ಭಙ್ಗ;
            }
            ಪಕ್ಷ ACTION_TYPE_GROUP_SET:
                ಯದಿ (state->base.components.controls & CONTROL_STICKY_ಕೀಲಿಗಳು) {
                    /* Convert group set action to a latch */
                    filter->action.type = ACTION_TYPE_GROUP_LATCH;
                    ಯದಿ (state->a11y_flags & XKB_A11_Y_STICKY_ಕೀಲಿಗಳು_LATCH_TO_LOCK) {
                        filter->action.group.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                ಭಙ್ಗ;
            ಪಕ್ಷ ACTION_TYPE_GROUP_LOCK:
                ಯದಿ ((filter->action.group.flags &
                     ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE) &&
                    (state->flags &
                     STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_ಕೀಲಿ) &&
                    /*
                     * FIXME: sticky keys may leave an unexpected modifier
                     * from the level 0 latched, so disable for now.
                     */
                    !(state->base.components.controls & CONTROL_STICKY_ಕೀಲಿಗಳು))
                {
                    filter->action.group.flags |= ACTION_LOCK_ON_RELEASE;
                    /* Add base level action */
                    next.ಎಣಿಕೆ = xkb_keymap_ಕೀಲಿ_get_actions_by_level(
                        state->base.keymap, ಕೀಲಿ, state->base.components.group,
                        0, &next.actions
                    );
                    ಪ್ರತಿಜ್ಞಾ(next.ಎಣಿಕೆ == 1);
                }
                ಭಙ್ಗ;
            ಪಕ್ಷ ACTION_TYPE_PTR_MOVE:
            ಪಕ್ಷ ACTION_TYPE_PTR_BUTTON:
            ಪಕ್ಷ ACTION_TYPE_PTR_LOCK:
            ಪಕ್ಷ ACTION_TYPE_PTR_DEFAULT:
                ಯದಿ (!(state->base.components.controls & CONTROL_MOUSE_ಕೀಲಿಗಳು)) {
                    /* Convert pointer actions to NoAction */
                    ಪ್ರತಿಜ್ಞಾ(!filter_action_funcs[ACTION_TYPE_NONE].new);
                    filter->func = ರಿಕ್ತ;
                    ಅನುವರ್ತ;
                }
                ಭಙ್ಗ;
            ಪಕ್ಷ ACTION_TYPE_REDIRECT_ಕೀಲಿ:
                // FIXME: this is not efficient to resolve mods here each time
                filter->action.redirect.affect_mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.affect_mods
                );
                filter->action.redirect.mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.mods
                );
                ಭಙ್ಗ;
            ಸ್ವಭಾವ: ;
            }
            filter->func = filter_action_funcs[filter->action.type].func;
            filter_action_funcs[filter->action.type].new(state, events, filter);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_state_update_derived(ಸಂರಚನಾ xkb_state *state);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
xkb_state_init(ಸಂರಚನಾ xkb_state * restrict state,
               ಸಂರಚನಾ xkb_keymap * restrict keymap,
               ಗಣನಾ xkb_state_mode_internal mode)
{
    state->mode = mode;
    state->refcnt = 1;
    state->keymap = xkb_keymap_ref(keymap);
    state->out_of_range_group.policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

    /* Ensure that derived state is correctly initialized */
    xkb_state_update_derived(state);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
xkb_client_state_init(ಸಂರಚನಾ xkb_client_state * restrict state,
                      ಸಂರಚನಾ xkb_keymap * restrict keymap,
                      ಗಣನಾ xkb_state_mode_internal mode)
{
    xkb_state_init(&state->base, keymap, mode);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
xkb_server_state_init(ಸಂರಚನಾ xkb_server_state * restrict state,
                      ಸಂರಚನಾ xkb_keymap * restrict keymap,
                      ಗಣನಾ xkb_state_mode_internal mode,
                      ಗಣನಾ xkb_a11y_flags a11y_affect,
                      ಗಣನಾ xkb_a11y_flags a11y_flags)
{
    xkb_state_init(&state->base, keymap, mode);

    state->a11y_flags = a11y_flags;
    ಯದಿ (keymap->format != XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !(a11y_affect & XKB_A11_Y_LATCH_SIMULTANEOUS_ಕೀಲಿಗಳು)) {
             /* Keymap v2+: enable extension to XKB if not manually disabled */
             state->a11y_flags |= XKB_A11_Y_LATCH_SIMULTANEOUS_ಕೀಲಿಗಳು;
    }
}

ಸಂರಚನಾ xkb_state *
xkb_state_new(ಸಂರಚನಾ xkb_keymap *keymap)
{
    ಸಂರಚನಾ xkb_server_state * ಧ್ರುವ state = calloc(1, ಪರಿಮಾಣ(*state));
    ಯದಿ (!state) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "Could not allocate a state object.\n");
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    xkb_server_state_init(state, keymap, LEGACY_MIXED_STATE, 0, 0);

    ಪ್ರತ್ಯಾಗಮ (ಸಂರಚನಾ xkb_state *)state;
}

ಸಂರಚನಾ xkb_state *
xkb_state_new_with_mode(ಸಂರಚನಾ xkb_keymap * restrict keymap,
                        ಗಣನಾ xkb_state_mode mode,
                        ಗಣನಾ xkb_status * restrict status)
{
    ವಿಕಲ್ಪ (mode) {
    ಪಕ್ಷ XKB_STATE_MODE_CLIENT:
    ಪಕ್ಷ XKB_STATE_MODE_SERVER_QUERY: {
        ಸಂರಚನಾ xkb_client_state * ಧ್ರುವ state = calloc(1, ಪರಿಮಾಣ(*state));
        ಯದಿ (!state)
            ಭಙ್ಗ;

        static_assert((unsigned)XKB_STATE_MODE_CLIENT ==
                      (unsigned)CLIENT_STATE, "");
        static_assert((unsigned)XKB_STATE_MODE_SERVER_QUERY ==
                      (unsigned)SERVER_COMPANION, "");
        xkb_client_state_init(state, keymap, (ಗಣನಾ xkb_state_mode_internal)mode);

        ಯದಿ (status)
            *status = XKB_SUCCESS;

        ಪ್ರತ್ಯಾಗಮ (ಸಂರಚನಾ xkb_state *)state;
    }
    ಪಕ್ಷ XKB_STATE_MODE_SERVER: {
        ಸಂರಚನಾ xkb_server_state * ಧ್ರುವ state = calloc(1, ಪರಿಮಾಣ(*state));
        ಯದಿ (!state)
            ಭಙ್ಗ;

        xkb_server_state_init(state, keymap, LEGACY_SERVER_STATE, 0, 0);

        ಯದಿ (status)
            *status = XKB_SUCCESS;

        ಪ್ರತ್ಯಾಗಮ (ಸಂರಚನಾ xkb_state *)state;
    }
    ಸ್ವಭಾವ:
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_STATE_MODE_,
                     "Unsupported state mode: %u\n", mode);
        ಯದಿ (status)
            *status = XKB_ERROR_UNSUPPORTED_STATE_MODE;
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                  "Could not allocate a state object.\n");
    ಯದಿ (status)
        *status = XKB_ERROR_ALLOCATION_FAILURE;
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸಂರಚನಾ xkb_state *
xkb_state_ref(ಸಂರಚನಾ xkb_state *state)
{
    ಪ್ರತಿಜ್ಞಾ(state->refcnt > 0);
    state->refcnt++;
    ಪ್ರತ್ಯಾಗಮ state;
}

ಶೂನ್ಯ
xkb_state_reset(ಸಂರಚನಾ xkb_state *state)
{
    memset(&state->components, 0, ಪರಿಮಾಣ(state->components));

    ಯದಿ (state->mode < LEGACY_MIXED_STATE) {
        static_assert(ಪರಿಮಾಣ(*state) == ಪರಿಮಾಣ(ಸಂರಚನಾ xkb_client_state),
                      "Not a mere wrapper");
        /* Nothing else to reset */
    } ಅನ್ಯಥಾ {
        ಸಂರಚನಾ xkb_server_state *state_ = (ಸಂರಚನಾ xkb_server_state *)state;
        ಧ್ರುವ ಸಂರಚನಾ xkb_server_state prev = *state_;
        memset(state_, 0, ಪರಿಮಾಣ(*state_));

        state_->base = prev.base;
        state_->a11y_flags = prev.a11y_flags;
        state_->filters = prev.filters;
        darray_size(state_->filters) = 0;
    }
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
xkb_state_destroy(ಸಂರಚನಾ xkb_state *state)
{
    xkb_keymap_unref(state->keymap);
    ಯದಿ (state->mode > SERVER_COMPANION)
        darray_free(((ಸಂರಚನಾ xkb_server_state *)state)->filters);
}

ಶೂನ್ಯ
xkb_state_unref(ಸಂರಚನಾ xkb_state *state)
{
    ಪ್ರತಿಜ್ಞಾ(!state || state->refcnt > 0);
    ಯದಿ (!state || --state->refcnt > 0)
        ಪ್ರತ್ಯಾಗಮ;

    xkb_state_destroy(state);

    ವಿಕಲ್ಪ (state->mode) {
    ಪಕ್ಷ CLIENT_STATE:
    ಪಕ್ಷ SERVER_COMPANION:
        ಮುಕ್ತ((ಸಂರಚನಾ xkb_client_state *)state);
        ಭಙ್ಗ;
    ಪಕ್ಷ LEGACY_MIXED_STATE:
    ಪಕ್ಷ LEGACY_SERVER_STATE:
        ಮುಕ್ತ((ಸಂರಚನಾ xkb_server_state *)state);
        ಭಙ್ಗ;
    ಪಕ್ಷ SERVER_STATE: /* handled by xkb_machine_unref() */
    ಸ್ವಭಾವ:
        PANIC_UNREACHABLE();
    }
}

ಸಂರಚನಾ xkb_keymap *
xkb_state_get_keymap(ಸಂರಚನಾ xkb_state *state)
{
    ಪ್ರತ್ಯಾಗಮ state->keymap;
}

/**
 * Update the LED state to match the rest of the xkb_state.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
xkb_state_led_update_all(ಸಂರಚನಾ xkb_state *state)
{
    xkb_led_index_t idx;
    ಧ್ರುವ ಸಂರಚನಾ xkb_led *led;

    state->components.leds = 0;

    xkb_leds_enumerate(idx, led, state->keymap) {

        ಯದಿ (led->which_mods != 0 && led->mods.mask != 0) {
            xkb_mod_mask_t mod_mask = 0;
            ಯದಿ (led->which_mods & XKB_STATE_MODS_EFFECTIVE)
                mod_mask |= state->components.mods;
            ಯದಿ (led->which_mods & XKB_STATE_MODS_DEPRESSED)
                mod_mask |= state->components.base_mods;
            ಯದಿ (led->which_mods & XKB_STATE_MODS_LATCHED)
                mod_mask |= state->components.latched_mods;
            ಯದಿ (led->which_mods & XKB_STATE_MODS_LOCKED)
                mod_mask |= state->components.locked_mods;

            ಯದಿ (led->mods.mask & mod_mask) {
                state->components.leds |= (UINT32_C(1) << idx);
                ಅನುವರ್ತ;
            }
        }

        ಯದಿ (led->which_groups != 0) {
            ಯದಿ (likely(led->groups) != 0) {
                xkb_layout_mask_t group_mask = 0;
                /* Effective and locked groups have been brought into range */
                ಪ್ರತಿಜ್ಞಾ(state->components.group < XKB_MAX_GROUPS);
                ಪ್ರತಿಜ್ಞಾ(state->components.locked_group >= 0 &&
                       state->components.locked_group < XKB_MAX_GROUPS);
                /* Effective and locked groups are used as mask */
                ಯದಿ (led->which_groups & XKB_STATE_LAYOUT_EFFECTIVE)
                    group_mask |= (UINT32_C(1) << state->components.group);
                ಯದಿ (led->which_groups & XKB_STATE_LAYOUT_LOCKED)
                    group_mask |= (UINT32_C(1) << state->components.locked_group);
                /* Base and latched groups only have to be non-zero */
                ಯದಿ ((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                    state->components.base_group != 0)
                    group_mask |= led->groups;
                ಯದಿ ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                    state->components.latched_group != 0)
                    group_mask |= led->groups;

                ಯದಿ (led->groups & group_mask) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    ಅನುವರ್ತ;
                }
            } ಅನ್ಯಥಾ {
                /* Special case for Base and latched groups */
                ಯದಿ (((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                     state->components.base_group == 0) ||
                    ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                     state->components.latched_group == 0)) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    ಅನುವರ್ತ;
                }
            }
        }

        ಯದಿ (led->ctrls & state->components.controls) {
            state->components.leds |= (UINT32_C(1) << idx);
            ಅನುವರ್ತ;
        }
    }
}

/**
 * Calculates the derived state (effective mods/group and LEDs) from an
 * up-to-date xkb_state.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
xkb_state_update_derived(ಸಂರಚನಾ xkb_state *state)
{
    xkb_layout_index_t wrapped;

    state->components.mods = (state->components.base_mods |
                              state->components.latched_mods |
                              state->components.locked_mods);

    /* Lock group must be adjusted, but not base nor latched groups */
    wrapped = XkbWrapGroupIntoRange(state->components.locked_group,
                                    state->keymap->ಅಂಕೆ_groups,
                                    state->out_of_range_group.policy,
                                    state->out_of_range_group.redirect_group);
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->components.locked_group =
        (int32_t) (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    /* Effective group must be adjusted */
    wrapped = XkbWrapGroupIntoRange(state->components.base_group +
                                    state->components.latched_group +
                                    state->components.locked_group,
                                    state->keymap->ಅಂಕೆ_groups,
                                    state->out_of_range_group.policy,
                                    state->out_of_range_group.redirect_group);
    state->components.group =
        (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    xkb_state_led_update_all(state);
}

ಸ್ಥಿರ ಶೂನ್ಯ
state_update_enabled_controls(ಸಂರಚನಾ xkb_server_state *state,
                              ಗಣನಾ xkb_keyboard_control_flags affect_controls,
                              ಗಣನಾ xkb_keyboard_control_flags controls,
                              ಸಂರಚನಾ xkb_events *events)
{
    ಧ್ರುವ bool had_sticky_ಕೀಲಿಗಳು = state->base.components.controls
                               & CONTROL_STICKY_ಕೀಲಿಗಳು;

    /*
     * Enable using the public API with the all the Control values, except
     * the internal ones, if any.
     */
    affect_controls =
        ( affect_controls
        & (ಗಣನಾ xkb_keyboard_control_flags)CONTROL_ALL_BOOLEAN );
    state->base.components.controls &=
        (ಗಣನಾ xkb_action_controls)~affect_controls;
    state->base.components.controls |=
        (ಗಣನಾ xkb_action_controls)(controls & affect_controls);

    ಯದಿ (had_sticky_ಕೀಲಿಗಳು &&
        !(state->base.components.controls & CONTROL_STICKY_ಕೀಲಿಗಳು)) {
        /* Sticky keys were disabled: clear all locks and latches */
        clear_all_latches_and_locks(state, events);
    }

    xkb_state_update_derived(&state->base);
}

/**
 * Given a particular key event, updates the state structure to reflect the
 * new modifiers.
 */
ಗಣನಾ xkb_state_component
xkb_state_update_ಕೀಲಿ(ಸಂರಚನಾ xkb_state *base_state, xkb_keycode_t kc,
                     ಗಣನಾ xkb_ಕೀಲಿ_direction direction)
{
    /* Guard against client-only state */
    ಪ್ರತಿಜ್ಞಾ(base_state->mode > SERVER_COMPANION);
    ಯದಿ (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        ಪ್ರತ್ಯಾಗಮ 0;
    }

    ಸಂರಚನಾ xkb_server_state * ಧ್ರುವ state =
        (ಸಂರಚನಾ xkb_server_state *)base_state;

    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ* ಧ್ರುವ ಕೀಲಿ = Xkb_ಕೀಲಿ(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    ಯದಿ (!ಕೀಲಿ || (direction == XKB_ಕೀಲಿ_REPEATED && !ಕೀಲಿ->repeats))
        ಪ್ರತ್ಯಾಗಮ 0;

    ಯದಿ (direction == XKB_ಕೀಲಿ_DOWN &&
        (state->base.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ಕೀಲಿಗಳು) &&
        (state->a11y_flags & XKB_A11_Y_STICKY_ಕೀಲಿಗಳು_NO_SIMULTANEOUS_ಕೀಲಿಗಳು) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ಕೀಲಿಗಳು,
                                      0, ರಿಕ್ತ);
    }

    ಧ್ರುವ ಸಂರಚನಾ state_components prev_components = state->base.components;

    state->set_mods = 0;
    state->clear_mods = 0;

    xkb_filter_apply_all(state, ರಿಕ್ತ, ಕೀಲಿ, direction);

    xkb_mod_index_t i;
    xkb_mod_mask_t bit;
    ಪ್ರತಿ (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        ಯದಿ (state->set_mods & bit) {
            state->mod_ಕೀಲಿ_ಎಣಿಕೆ[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    ಪ್ರತಿ (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        ಯದಿ (state->clear_mods & bit) {
            state->mod_ಕೀಲಿ_ಎಣಿಕೆ[i]--;
            ಯದಿ (state->mod_ಕೀಲಿ_ಎಣಿಕೆ[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_ಕೀಲಿ_ಎಣಿಕೆ[i] = 0;
            }
            state->clear_mods &= ~bit;
        }
    }

    xkb_state_update_derived(&state->base);

    ಪ್ರತ್ಯಾಗಮ get_state_component_changes(&prev_components, &state->base.components);
}

/* We need fake keys for `update_latch_modifiers` and `update_latch_group`.
 * These keys must have at least one level in order to break latches. We need 2
 * keys with specific actions in order to update group/mod latches without
 * affecting each other. */
ಸ್ಥಿರ ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry synthetic_ಕೀಲಿ_level_entry = { 0 };
ಸ್ಥಿರ ಸಂರಚನಾ xkb_ಕೀಲಿ_type synthetic_ಕೀಲಿ_type = {
     .ಅಂಕೆ_entries = 1,
     .ಅಂಕೆ_levels = 1,
     .entries = &synthetic_ಕೀಲಿ_level_entry
};
ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ synthetic_ಕೀಲಿ = { 0 };

/* Transcription from xserver: XkbLatchModifiers */
ಸ್ಥಿರ ಶೂನ್ಯ
update_latch_modifiers(ಸಂರಚನಾ xkb_server_state *state,
                       ಸಂರಚನಾ xkb_events *events,
                       xkb_mod_mask_t mask, xkb_mod_mask_t latches)
{
    /* Clear affected latched modifiers */
    ಧ್ರುವ xkb_mod_mask_t clear = mask & ~latches;
    state->base.components.latched_mods &= ~clear;

    /* Clear any pending latch to locks using ad hoc action:
     * only affect corresponding modifier latches and no group latch. */
    ಸಂರಚನಾ xkb_level synthetic_ಕೀಲಿ_level_break_mod_latch = {
        .ಅಂಕೆ_syms = 0,
        .ಅಂಕೆ_actions = 1,
        .upper = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ,
        .s.sym = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ,
        .a.action.internal = {
            .type = ACTION_TYPE_INTERNAL,
            .flags = INTERNAL_BREAKS_MOD_LATCH,
            .clear_latched_mods = clear
        }
    };
    ಸಂರಚನಾ xkb_group synthetic_ಕೀಲಿ_group_break_mod_latch = {
        .type = &synthetic_ಕೀಲಿ_type,
        .levels = &synthetic_ಕೀಲಿ_level_break_mod_latch
    };
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ synthetic_ಕೀಲಿ_break_mod_latch = {
        .ಅಂಕೆ_groups = 1,
        .groups = &synthetic_ಕೀಲಿ_group_break_mod_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_ಕೀಲಿ_break_mod_latch, XKB_ಕೀಲಿ_DOWN);

    /* Finally set the latched mods by simulate tapping a key with the
     * corresponding action */
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ* ಧ್ರುವ ಕೀಲಿ = &synthetic_ಕೀಲಿ;
    ಧ್ರುವ ಸಙ್ಘ xkb_action latch_mods = {
        .mods = {
            .type = ACTION_TYPE_MOD_LATCH,
            .mods = { .mask = mask & latches },
            .flags = 0,
        },
    };
    ಸಂರಚನಾ xkb_filter* ಧ್ರುವ filter = xkb_filter_new(state);
    filter->ಕೀಲಿ = ಕೀಲಿ;
    filter->func = xkb_filter_mod_latch_func;
    filter->action = latch_mods;
    xkb_filter_mod_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_mod_latch_func(state, events, filter, ಕೀಲಿ, XKB_ಕೀಲಿ_UP);
}

/* Transcription from xserver: XkbLatchGroup */
ಸ್ಥಿರ ಶೂನ್ಯ
update_latch_group(ಸಂರಚನಾ xkb_server_state *state,
                   ಸಂರಚನಾ xkb_events *events, int32_t group)
{
    /* Clear any pending latch to locks. */
    ಸ್ಥಿರ ಸಂರಚನಾ xkb_level synthetic_ಕೀಲಿ_level_break_group_latch = {
        .ಅಂಕೆ_syms = 0,
        .ಅಂಕೆ_actions = 1,
        .upper = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ,
        .s.sym = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ,
        .a.action.internal = {
            .type = ACTION_TYPE_INTERNAL,
            .flags = INTERNAL_BREAKS_GROUP_LATCH,
            .clear_latched_mods = 0
        }
    };
    ಸ್ಥಿರ ಸಂರಚನಾ xkb_group synthetic_ಕೀಲಿ_group_break_group_latch = {
        .type = &synthetic_ಕೀಲಿ_type,
        .levels = &synthetic_ಕೀಲಿ_level_break_group_latch
    };
    ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ synthetic_ಕೀಲಿ_break_group_latch = {
        .ಅಂಕೆ_groups = 1,
        .groups = &synthetic_ಕೀಲಿ_group_break_group_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_ಕೀಲಿ_break_group_latch, XKB_ಕೀಲಿ_DOWN);

    /* Simulate tapping a key with a group latch action, but in isolation: i.e.
     * without affecting the other filters. */
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ* ಧ್ರುವ ಕೀಲಿ = &synthetic_ಕೀಲಿ;
    ಧ್ರುವ ಸಙ್ಘ xkb_action latch_group = {
        .group = {
            .type = ACTION_TYPE_GROUP_LATCH,
            .flags = ACTION_ABSOLUTE_SWITCH,
            .group = group,
        },
    };
    ಸಂರಚನಾ xkb_filter* ಧ್ರುವ filter = xkb_filter_new(state);
    filter->ಕೀಲಿ = ಕೀಲಿ;
    filter->func = xkb_filter_group_latch_func;
    filter->action = latch_group;
    xkb_filter_group_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_group_latch_func(state, events, filter, ಕೀಲಿ, XKB_ಕೀಲಿ_UP);
}

/**
 * Compute the resolved effective mask of an arbitrary input.
 *
 * Contrary to `mod_mask_get_effective`, it resolves only modifiers not present
 * in the canonical mask, so that it enables `xkb_state_serialize_mods` to
 * round trip via `xkb_state_update_mask`.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ xkb_mod_mask_t
resolve_to_canonical_mods(ಸಂರಚನಾ xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    ಪ್ರತ್ಯಾಗಮ
        /*
         * Keep canonical modifier mask.
         * It contains either real modifiers or canonical virtual modifiers.
         */
        (mods & keymap->canonical_state_mask) |
        /* Resolve other modifiers */
        mod_mask_get_effective(keymap,
                               mods & ~keymap->canonical_state_mask);
}

ಸ್ಥಿರ ಶೂನ್ಯ
state_update_latched_locked(
    ಸಂರಚನಾ xkb_server_state * restrict state,
    ಧ್ರುವ ಸಂರಚನಾ xkb_state_components_update * restrict update,
    ಸಂರಚನಾ xkb_events * restrict events
)
{
    /* Update locks */
    ಧ್ರುವ xkb_mod_mask_t affect_locked_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_locked_mods);
    ಯದಿ ((update->components & XKB_STATE_MODS_LOCKED) &&
        affect_locked_mods) {
        ಧ್ರುವ xkb_mod_mask_t locked_mods =
            resolve_to_canonical_mods(state->base.keymap, update->locked_mods);
        state->base.components.locked_mods &= ~affect_locked_mods;
        state->base.components.locked_mods |= locked_mods & affect_locked_mods;
    }
    ಯದಿ (update->components & XKB_STATE_LAYOUT_LOCKED) {
        state->base.components.locked_group = update->locked_layout;
    }

    /* Update latches */
    ಧ್ರುವ xkb_mod_mask_t affect_latched_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_latched_mods);
    ಯದಿ ((update->components & XKB_STATE_MODS_LATCHED) &&
        affect_latched_mods) {
        ಧ್ರುವ xkb_mod_mask_t latched_mods =
            resolve_to_canonical_mods(state->base.keymap, update->latched_mods);
        update_latch_modifiers(state, events, affect_latched_mods, latched_mods);
    }
    ಯದಿ (update->components & XKB_STATE_LAYOUT_LATCHED) {
        update_latch_group(state, events, update->latched_layout);
    }
}

ಗಣನಾ xkb_state_component
xkb_state_update_latched_locked(ಸಂರಚನಾ xkb_state *base_state,
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
    ಪ್ರತಿಜ್ಞಾ(base_state->mode > SERVER_COMPANION);
    ಯದಿ (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        ಪ್ರತ್ಯಾಗಮ 0;
    }

    ಸಂರಚನಾ xkb_server_state * ಧ್ರುವ state =
        (ಸಂರಚನಾ xkb_server_state *)base_state;

    ಧ್ರುವ ಸಂರಚನಾ state_components previous_components = state->base.components;
    ಧ್ರುವ ಗಣನಾ xkb_state_component components
        = ((affect_latched_mods || latched_mods) ? XKB_STATE_MODS_LATCHED : 0)
        | ((affect_locked_mods || locked_mods) ? XKB_STATE_MODS_LOCKED : 0)
        | (affect_latched_layout ? XKB_STATE_LAYOUT_LATCHED : 0)
        | (affect_locked_layout ? XKB_STATE_LAYOUT_LOCKED : 0);
    ಧ್ರುವ ಸಂರಚನಾ xkb_state_components_update update = {
        .size = ಪರಿಮಾಣ(update),
        .components = components,
        .affect_latched_mods = affect_latched_mods,
        .latched_mods = latched_mods,
        .latched_layout = latched_layout,
        .affect_locked_mods = affect_locked_mods,
        .locked_mods = locked_mods,
        .locked_layout = locked_layout
    };
    state_update_latched_locked(state, &update, ರಿಕ್ತ);
    xkb_state_update_derived(&state->base);
    ಪ್ರತ್ಯಾಗಮ get_state_component_changes(&previous_components,
                                       &state->base.components);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
clear_all_latches_and_locks(ಸಂರಚನಾ xkb_server_state *state,
                            ಸಂರಚನಾ xkb_events *events)
{
    ಸ್ಥಿರ ಧ್ರುವ ಗಣನಾ xkb_state_component components
        = XKB_STATE_MODS_LATCHED | XKB_STATE_MODS_LOCKED
        | XKB_STATE_LAYOUT_LATCHED | XKB_STATE_LAYOUT_LOCKED;
    ಧ್ರುವ ಸಂರಚನಾ xkb_state_components_update update = {
        .size = ಪರಿಮಾಣ(update),
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

ಸ್ಥಿರ ಗಣನಾ xkb_status
state_update_layout_policy(ಸಂರಚನಾ xkb_server_state *state,
                           ಧ್ರುವ ಸಂರಚನಾ xkb_layout_policy_update *update)
{
    ಯದಿ (xkb_feature_supported(XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY,
                              update->policy)) {
        ಯದಿ (update->policy == XKB_LAYOUT_OUT_OF_RANGE_REDIRECT) {
            ಯದಿ (update->redirect < state->base.keymap->ಅಂಕೆ_groups) {
                state->base.out_of_range_group.redirect_group =
                    update->redirect;
            } ಅನ್ಯಥಾ {
                log_err(state->base.keymap->ctx,
                        XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Layout policy: "
                        "unsupported layout index %"PRIu32" > %"PRIu32"\n",
                        update->redirect + 1, state->base.keymap->ಅಂಕೆ_groups);
                ಪ್ರತ್ಯಾಗಮ XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
            }
        }
        state->base.out_of_range_group.policy = update->policy;
        ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
    } ಅನ್ಯಥಾ {
        log_err(state->base.keymap->ctx,
                XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY_,
                "Unsupported layout policy: %d\n", update->policy);
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY;
    }
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
ಸ್ಥಿರ ಗಣನಾ xkb_status
check_state_update_abi_(ಸಂರಚನಾ xkb_context * restrict ctx,
                        ಧ್ರುವ ವರ್ಣ * restrict func,
                        ಧ್ರುವ ಸಂರಚನಾ xkb_synthetic_update * restrict update)
{
    ಗಣನಾ xkb_status status = XKB_SUCCESS;
    ಯದಿ ((status = xkb_check_state_abi(update)) ||
        (update->reserved0 != 0 &&
         (status = XKB_ERROR_ABI_FORWARD_COMPAT)) ||
        (update->components &&
         (status = xkb_check_state_abi(update->components))) ||
        (update->layout_policy &&
         (status = xkb_check_state_abi(update->layout_policy)))) {
        xkb_log_abi_error(ctx, func, status);
    }
    ಪ್ರತ್ಯಾಗಮ status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

#define check_state_update_abi(ctx, update) \
    check_state_update_abi_(ctx, __func__, update)

ಗಣನಾ xkb_status
xkb_state_update_synthetic(ಸಂರಚನಾ xkb_state * base_state,
                           ಧ್ರುವ ಸಂರಚನಾ xkb_synthetic_update * update,
                           ಗಣನಾ xkb_state_component *changed)
{
    /* Guard against client-only state */
    ಯದಿ (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    ಸಂರಚನಾ xkb_server_state * ಧ್ರುವ state =
        (ಸಂರಚನಾ xkb_server_state *)base_state;

    /* Check ABI compatibility */
    ಗಣನಾ xkb_status status = check_state_update_abi(state->base.keymap->ctx,
                                                    update);
    ಯದಿ (status != XKB_SUCCESS)
        ಪ್ರತ್ಯಾಗಮ status;

    ಧ್ರುವ ಸಂರಚನಾ state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    /* Update parametrized controls first */
    ಯದಿ (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        ಯದಿ (status != XKB_SUCCESS)
            ಪ್ರತ್ಯಾಗಮ status;
    }

    ಯದಿ (update->components) {
        ಧ್ರುವ ಸಂರಚನಾ xkb_state_components_update * ಧ್ರುವ components =
            update->components;
        /* Update boolean controls first */
        ಯದಿ (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, ರಿಕ್ತ);
        }

        state_update_latched_locked(state, components, ರಿಕ್ತ);
    }

    xkb_state_update_derived(&state->base);

    ಯದಿ (changed) {
        *changed = get_state_component_changes(&previous_components,
                                               &state->base.components);
    }
    ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
}

/**
 * Updates the state from a set of explicit masks as gained from
 * xkb_state_serialize_mods and xkb_state_serialize_groups.  As noted in the
 * documentation for these functions in xkbcommon.h, this round-trip is
 * lossy, and should only be used to update a slave state mirroring the
 * master, e.g. in a client/server window system.
 */
ಗಣನಾ xkb_state_component
xkb_state_update_mask(ಸಂರಚನಾ xkb_state *base_state,
                      xkb_mod_mask_t base_mods,
                      xkb_mod_mask_t latched_mods,
                      xkb_mod_mask_t locked_mods,
                      xkb_layout_index_t base_group,
                      xkb_layout_index_t latched_group,
                      xkb_layout_index_t locked_group)
{
    /* Guard against server-only and server companion state */
    ಪ್ರತಿಜ್ಞಾ(base_state->mode == CLIENT_STATE ||
           base_state->mode == LEGACY_MIXED_STATE);
    ಯದಿ (base_state->mode != CLIENT_STATE &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        ಪ್ರತ್ಯಾಗಮ 0;
    }

    ಸಂರಚನಾ xkb_client_state * ಧ್ರುವ state =
        (ಸಂರಚನಾ xkb_client_state *)base_state;

    ಧ್ರುವ ಸಂರಚನಾ state_components prev_components = state->base.components;

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

    ಪ್ರತ್ಯಾಗಮ get_state_component_changes(&prev_components, &state->base.components);
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Lock_Modifier
 */
ಸ್ಥಿರ bool
should_do_caps_transformation(ಸಂರಚನಾ xkb_state *state, xkb_keycode_t kc)
{
    ಪ್ರತ್ಯಾಗಮ
        xkb_state_mod_index_is_active(state, XKB_MOD_INDEX_CAPS,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_index_is_consumed(state, kc, XKB_MOD_INDEX_CAPS) == 0;
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Control_Modifier
 */
ಸ್ಥಿರ bool
should_do_ctrl_transformation(ಸಂರಚನಾ xkb_state *state, xkb_keycode_t kc)
{
    ಪ್ರತ್ಯಾಗಮ
        xkb_state_mod_index_is_active(state, XKB_MOD_INDEX_CTRL,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_index_is_consumed(state, kc, XKB_MOD_INDEX_CTRL) == 0;
}

/**
 * Provides the symbols to use for the given key and state.  Returns the
 * number of symbols pointed to in syms_out.
 */
ಪೂರ್ಣಾಙ್ಕ
xkb_state_ಕೀಲಿ_get_syms(ಸಂರಚನಾ xkb_state *state, xkb_keycode_t kc,
                       ಧ್ರುವ xkb_keysym_t **syms_out)
{
    ಧ್ರುವ xkb_layout_index_t layout = xkb_state_ಕೀಲಿ_get_layout(state, kc);
    ಯದಿ (layout == XKB_LAYOUT_INVALID)
        ಗಚ್ಛ err;

    ಧ್ರುವ xkb_level_index_t level = xkb_state_ಕೀಲಿ_get_level(state, kc, layout);
    ಯದಿ (level == XKB_LEVEL_INVALID)
        ಗಚ್ಛ err;

    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ* ಧ್ರುವ ಕೀಲಿ = Xkb_ಕೀಲಿ(state->keymap, kc);

    ಯದಿ (!ಕೀಲಿ)
        ಗಚ್ಛ err;

    ಧ್ರುವ ಸಂರಚನಾ xkb_level* ಧ್ರುವ leveli =
        xkb_keymap_ಕೀಲಿ_get_level(state->keymap, ಕೀಲಿ, layout, level);

    ಯದಿ (!leveli)
        ಗಚ್ಛ err;

    ಧ್ರುವ xkb_keysym_ಎಣಿಕೆ_t ಅಂಕೆ_syms = leveli->ಅಂಕೆ_syms;
    ಯದಿ (ಅಂಕೆ_syms == 0)
        ಗಚ್ಛ err;

    ಯದಿ (should_do_caps_transformation(state, kc)) {
        /* Only simple capitalization rules: keysyms count is unchanged. */
        ಯದಿ (ಅಂಕೆ_syms > 1) {
            *syms_out = (leveli->has_upper)
                      ? leveli->s.syms + ಅಂಕೆ_syms
                      : leveli->s.syms;
        } ಅನ್ಯಥಾ {
            *syms_out = &leveli->upper;
        }
    } ಅನ್ಯಥಾ {
        *syms_out = (ಅಂಕೆ_syms > 1)
                  ? leveli->s.syms
                  : &leveli->s.sym;
    }
    ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) ಅಂಕೆ_syms;

err:
    *syms_out = ರಿಕ್ತ;
    ಪ್ರತ್ಯಾಗಮ 0;
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
ಸ್ಥಿರ ವರ್ಣ
XkbToControl(ವರ್ಣ ch)
{
    ವರ್ಣ c = ch;

    ಯದಿ ((c >= '@' && c < '\177') || c == ' ')
        c &= 0x1F;
    ಅನ್ಯಥಾ ಯದಿ (c == '2')
        c = '\000';
    ಅನ್ಯಥಾ ಯದಿ (c >= '3' && c <= '7')
        c -= ('3' - '\033');
    ಅನ್ಯಥಾ ಯದಿ (c == '8')
        c = '\177';
    ಅನ್ಯಥಾ ಯದಿ (c == '/')
        c = '_' & 0x1F;
    ಪ್ರತ್ಯಾಗಮ c;
}

/**
 * Provides either exactly one symbol, or XKB_KEY_NoSymbol.
 */
xkb_keysym_t
xkb_state_ಕೀಲಿ_get_one_sym(ಸಂರಚನಾ xkb_state *state, xkb_keycode_t kc)
{
    ಧ್ರುವ xkb_keysym_t *syms = ರಿಕ್ತ;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_syms = xkb_state_ಕೀಲಿ_get_syms(state, kc, &syms);

    ಯದಿ (ಅಂಕೆ_syms != 1)
        ಪ್ರತ್ಯಾಗಮ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
    ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ syms[0];
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
ಸ್ಥಿರ xkb_keysym_t
get_one_sym_for_string(ಸಂರಚನಾ xkb_state *state, xkb_keycode_t kc)
{
    ಧ್ರುವ xkb_layout_index_t layout = xkb_state_ಕೀಲಿ_get_layout(state, kc);
    ಧ್ರುವ xkb_layout_index_t ಅಂಕೆ_layouts =
        xkb_keymap_ಅಂಕೆ_layouts_for_ಕೀಲಿ(state->keymap, kc);
    xkb_level_index_t level = xkb_state_ಕೀಲಿ_get_level(state, kc, layout);
    ಯದಿ (layout == XKB_LAYOUT_INVALID || ಅಂಕೆ_layouts == 0 ||
        level == XKB_LEVEL_INVALID)
        ಪ್ರತ್ಯಾಗಮ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;

    ಧ್ರುವ xkb_keysym_t *syms = ರಿಕ್ತ;
    ಪೂರ್ಣಾಙ್ಕ nsyms = xkb_keymap_ಕೀಲಿ_get_syms_by_level(state->keymap, kc,
                                                 layout, level, &syms);
    ಯದಿ (nsyms != 1)
        ಪ್ರತ್ಯಾಗಮ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
    xkb_keysym_t sym = syms[0];

    ಯದಿ (should_do_ctrl_transformation(state, kc) && sym > 127u) {
        ಪ್ರತಿ (xkb_layout_index_t i = 0; i < ಅಂಕೆ_layouts; i++) {
            level = xkb_state_ಕೀಲಿ_get_level(state, kc, i);
            ಯದಿ (level == XKB_LEVEL_INVALID)
                ಅನುವರ್ತ;

            nsyms = xkb_keymap_ಕೀಲಿ_get_syms_by_level(state->keymap, kc,
                                                     i, level, &syms);
            ಯದಿ (nsyms == 1 && syms[0] <= 127u) {
                sym = syms[0];
                ಭಙ್ಗ;
            }
        }
    }

    ಯದಿ (should_do_caps_transformation(state, kc)) {
        sym = xkb_keysym_to_upper(sym);
    }

    ಪ್ರತ್ಯಾಗಮ sym;
}

ಪೂರ್ಣಾಙ್ಕ
xkb_state_ಕೀಲಿ_get_utf8(ಸಂರಚನಾ xkb_state *state, xkb_keycode_t kc,
                       ವರ್ಣ *buffer, size_t size)
{
    ಪೂರ್ಣಾಙ್ಕ nsyms;
    ಧ್ರುವ xkb_keysym_t *syms = ರಿಕ್ತ;
    ಧ್ರುವ xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    ಯದಿ (sym != XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
        nsyms = 1; syms = &sym;
    }
    ಅನ್ಯಥಾ {
        nsyms = xkb_state_ಕೀಲಿ_get_syms(state, kc, &syms);
    }

    /* Make sure not to truncate in the middle of a UTF-8 sequence. */
    ಪೂರ್ಣಾಙ್ಕ offset = 0;
    ವರ್ಣ tmp[XKB_KEYSYM_UTF8_MAX_SIZE];
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nsyms; i++) {
        ಪೂರ್ಣಾಙ್ಕ ret = xkb_keysym_to_utf8(syms[i], tmp, ಪರಿಮಾಣ(tmp));
        ಯದಿ (ret <= 0)
            ಗಚ್ಛ err_bad;

        ret--;
        ಯದಿ ((size_t)offset + (size_t)ret <= size)
            memcpy(buffer + offset, tmp, (size_t)ret);
        offset += ret;
    }

    ಯದಿ ((size_t)offset >= size)
        ಗಚ್ಛ err_trunc;
    buffer[offset] = '\0';

    ಯದಿ (!is_valid_utf8(buffer, (size_t)offset))
        ಗಚ್ಛ err_bad;

    ಯದಿ (offset == 1 && (unsigned ಪೂರ್ಣಾಙ್ಕ) buffer[0] <= 127u &&
        should_do_ctrl_transformation(state, kc))
        buffer[0] = XkbToControl(buffer[0]);

    ಪ್ರತ್ಯಾಗಮ offset;

err_trunc:
    ಯದಿ (size > 0)
        buffer[size - 1] = '\0';
    ಪ್ರತ್ಯಾಗಮ offset;

err_bad:
    ಯದಿ (size > 0)
        buffer[0] = '\0';
    ಪ್ರತ್ಯಾಗಮ 0;
}

uint32_t
xkb_state_ಕೀಲಿ_get_utf32(ಸಂರಚನಾ xkb_state *state, xkb_keycode_t kc)
{
    ಧ್ರುವ xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    uint32_t cp = xkb_keysym_to_utf32(sym);

    ಯದಿ (cp <= 127u && should_do_ctrl_transformation(state, kc))
        cp = (uint32_t) XkbToControl((ವರ್ಣ) cp);

    ಪ್ರತ್ಯಾಗಮ cp;
}

/**
 * Serializes the requested modifier state into an xkb_mod_mask_t, with all
 * the same disclaimers as in xkb_state_update_mask.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ xkb_mod_mask_t
serialize_mods(ಧ್ರುವ ಸಂರಚನಾ state_components *components,
               ಗಣನಾ xkb_state_component type)
{
    xkb_mod_mask_t ret = 0;

    ಯದಿ (type & XKB_STATE_MODS_EFFECTIVE)
        ಪ್ರತ್ಯಾಗಮ components->mods;

    ಯದಿ (type & XKB_STATE_MODS_DEPRESSED)
        ret |= components->base_mods;
    ಯದಿ (type & XKB_STATE_MODS_LATCHED)
        ret |= components->latched_mods;
    ಯದಿ (type & XKB_STATE_MODS_LOCKED)
        ret |= components->locked_mods;

    ಪ್ರತ್ಯಾಗಮ ret;
}

xkb_mod_mask_t
xkb_state_serialize_mods(ಸಂರಚನಾ xkb_state *state,
                         ಗಣನಾ xkb_state_component type)
{
    ಪ್ರತ್ಯಾಗಮ serialize_mods(&state->components, type);
}

/**
 * Serialises the requested group state, with all the same disclaimers as
 * in xkb_state_update_mask.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ xkb_layout_index_t
serialize_layout(ಧ್ರುವ ಸಂರಚನಾ state_components *components,
                 ಗಣನಾ xkb_state_component type)
{
    ಯದಿ (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ಪ್ರತ್ಯಾಗಮ components->group;

    int32_t ret = 0;
    ಯದಿ (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret += components->base_group;
    ಯದಿ (type & XKB_STATE_LAYOUT_LATCHED)
        ret += components->latched_group;
    ಯದಿ (type & XKB_STATE_LAYOUT_LOCKED)
        ret += components->locked_group;

    ಪ್ರತ್ಯಾಗಮ (xkb_layout_index_t)ret;
}

xkb_layout_index_t
xkb_state_serialize_layout(ಸಂರಚನಾ xkb_state *state,
                           ಗಣನಾ xkb_state_component type)
{
    ಪ್ರತ್ಯಾಗಮ serialize_layout(&state->components, type);
}

xkb_led_mask_t
xkb_state_serialize_leds(ಧ್ರುವ ಸಂರಚನಾ xkb_state *state,
                         ಗಣನಾ xkb_state_component type)
{
    ಪ್ರತ್ಯಾಗಮ (type & XKB_STATE_LEDS)
        ? state->components.leds
        : 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಗಣನಾ xkb_keyboard_control_flags
serialize_controls(ಧ್ರುವ ಸಂರಚನಾ state_components *components,
                   ಗಣನಾ xkb_state_component type)
{
    ಪ್ರತ್ಯಾಗಮ (type & XKB_STATE_CONTROLS_EFFECTIVE)
        /*
         * Enable using the public API with the all the Controls values, except
         * the internal ones, if any.
         */
        ? (ಗಣನಾ xkb_keyboard_control_flags)
          (components->controls & CONTROL_ALL_BOOLEAN)
        : 0;
}

ಗಣನಾ xkb_keyboard_control_flags
xkb_state_serialize_controls(ಧ್ರುವ ಸಂರಚನಾ xkb_state *state,
                             ಗಣನಾ xkb_state_component type)
{
    ಪ್ರತ್ಯಾಗಮ serialize_controls(&state->components, type);
}

/**
 * Gets a modifier mask and returns the resolved effective mask; this
 * is needed because some modifiers can also map to other modifiers, e.g.
 * the "NumLock" modifier usually also sets the "Mod2" modifier.
 */
xkb_mod_mask_t
mod_mask_get_effective(ಸಂರಚನಾ xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    /* Initialize the effective mask with its corresponding real mods. */
    xkb_mod_mask_t mask = mods & MOD_REAL_MASK_ALL;

    /* Resolve the virtual modifiers */
    ಧ್ರುವ ಸಂರಚನಾ xkb_mod *mod;
    xkb_mod_index_t i;
    xkb_vmods_enumerate(i, mod, &keymap->mods)
        ಯದಿ (mods & (UINT32_C(1) << i))
            mask |= mod->mapping;

    ಪ್ರತ್ಯಾಗಮ mask;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
ಪೂರ್ಣಾಙ್ಕ
xkb_state_mod_index_is_active(ಸಂರಚನಾ xkb_state *state,
                              xkb_mod_index_t idx,
                              ಗಣನಾ xkb_state_component type)
{
    ಯದಿ (unlikely(idx >= xkb_keymap_ಅಂಕೆ_mods(state->keymap)))
        ಪ್ರತ್ಯಾಗಮ -1;

    ಧ್ರುವ xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    ಯದಿ (!mapping) {
        /* Modifier not mapped */
        ಪ್ರತ್ಯಾಗಮ 0;
    }
    /* WARNING: this may overmatch for virtual modifiers */
    ಪ್ರತ್ಯಾಗಮ (xkb_state_serialize_mods(state, type) & mapping) == mapping;
}

ಗಣನಾ {
    XKB_STATE_MATCH_FLAGS = XKB_STATE_MATCH_ANY
                          | XKB_STATE_MATCH_ALL
                          | XKB_STATE_MATCH_NON_EXCLUSIVE
};

/**
 * Helper function for xkb_state_mod_indices_are_active and
 * xkb_state_mod_names_are_active.
 */
ಸ್ಥಿರ bool
match_mod_masks(ಸಂರಚನಾ xkb_state *state,
                ಗಣನಾ xkb_state_component type,
                ಗಣನಾ xkb_state_match match,
                xkb_mod_mask_t wanted)
{
    ಧ್ರುವ xkb_mod_mask_t active = xkb_state_serialize_mods(state, type);

    ಯದಿ (!(match & XKB_STATE_MATCH_NON_EXCLUSIVE) && (active & ~wanted))
        ಪ್ರತ್ಯಾಗಮ false;

    ಯದಿ (match & XKB_STATE_MATCH_ANY)
        ಪ್ರತ್ಯಾಗಮ active & wanted;

    /* Ensure all flags but XKB_STATE_MATCH_ALL are processed before */
    static_assert(XKB_STATE_MATCH_ALL ==
                  ((ಗಣನಾ xkb_state_match) XKB_STATE_MATCH_FLAGS &
                   ~(XKB_STATE_MATCH_ANY | XKB_STATE_MATCH_NON_EXCLUSIVE)),
                  "");

    ಪ್ರತ್ಯಾಗಮ (active & wanted) == wanted;
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, or -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ಪೂರ್ಣಾಙ್ಕ
xkb_state_mod_indices_are_active(ಸಂರಚನಾ xkb_state *state,
                                 ಗಣನಾ xkb_state_component type,
                                 ಗಣನಾ xkb_state_match match,
                                 ...)
{
    ಯದಿ (match & ~((ಗಣನಾ xkb_state_match) XKB_STATE_MATCH_FLAGS))
        ಪ್ರತ್ಯಾಗಮ -2;

    va_list ap;
    xkb_mod_mask_t wanted = 0;
    ಪೂರ್ಣಾಙ್ಕ ret = 0;
    ಧ್ರುವ xkb_mod_index_t ಅಂಕೆ_mods = xkb_keymap_ಅಂಕೆ_mods(state->keymap);

    va_start(ap, match);
    ಯಾವತ್ (1) {
        xkb_mod_index_t idx = va_arg(ap, xkb_mod_index_t);
        ಯದಿ (idx == XKB_MOD_INVALID)
            ಭಙ್ಗ;
        ಯದಿ (unlikely(idx >= ಅಂಕೆ_mods)) {
            ret = -1;
            ಭಙ್ಗ;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_end(ap);

    ಯದಿ (ret == -1)
        ಪ್ರತ್ಯಾಗಮ ret;

    ಯದಿ (!wanted) {
        /* Modifiers not mapped */
        ಪ್ರತ್ಯಾಗಮ 0;
    }

    ಪ್ರತ್ಯಾಗಮ match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
ಪೂರ್ಣಾಙ್ಕ
xkb_state_mod_name_is_active(ಸಂರಚನಾ xkb_state *state, ಧ್ರುವ ವರ್ಣ *name,
                             ಗಣನಾ xkb_state_component type)
{
    ಧ್ರುವ xkb_mod_index_t idx = xkb_keymap_mod_get_index(state->keymap, name);

    ಯದಿ (idx == XKB_MOD_INVALID)
        ಪ್ರತ್ಯಾಗಮ -1;

    ಪ್ರತ್ಯಾಗಮ xkb_state_mod_index_is_active(state, idx, type);
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ATTR_NULL_SENTINEL ಪೂರ್ಣಾಙ್ಕ
xkb_state_mod_names_are_active(ಸಂರಚನಾ xkb_state *state,
                               ಗಣನಾ xkb_state_component type,
                               ಗಣನಾ xkb_state_match match,
                               ...)
{
    ಯದಿ (match & ~((ಗಣನಾ xkb_state_match) XKB_STATE_MATCH_FLAGS))
        ಪ್ರತ್ಯಾಗಮ -2;

    va_list ap;
    xkb_mod_mask_t wanted = 0;
    ಪೂರ್ಣಾಙ್ಕ ret = 0;

    va_start(ap, match);
    ಯಾವತ್ (1) {
        ಧ್ರುವ ವರ್ಣ *str = va_arg(ap, ಧ್ರುವ ವರ್ಣ *);
        ಯದಿ (str == ರಿಕ್ತ)
            ಭಙ್ಗ;
        ಧ್ರುವ xkb_mod_index_t idx = xkb_keymap_mod_get_index(state->keymap, str);
        ಯದಿ (idx == XKB_MOD_INVALID) {
            ret = -1;
            ಭಙ್ಗ;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_end(ap);

    ಯದಿ (ret == -1)
        ಪ್ರತ್ಯಾಗಮ ret;

    ಯದಿ (!wanted) {
        /* Modifiers not mapped */
        ಪ್ರತ್ಯಾಗಮ 0;
    }

    ಪ್ರತ್ಯಾಗಮ match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given group is active with the specified type(s), 0 if
 * not, or -1 if the group is invalid.
 */
ಪೂರ್ಣಾಙ್ಕ
xkb_state_layout_index_is_active(ಸಂರಚನಾ xkb_state *state,
                                 xkb_layout_index_t idx,
                                 ಗಣನಾ xkb_state_component type)
{
    ಯದಿ (idx >= state->keymap->ಅಂಕೆ_groups)
        ಪ್ರತ್ಯಾಗಮ -1;

    ಪೂರ್ಣಾಙ್ಕ ret = 0;
    ಯದಿ (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ret |= (state->components.group == idx);
    ಯದಿ (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret |= (state->components.base_group == (int32_t) idx);
    ಯದಿ (type & XKB_STATE_LAYOUT_LATCHED)
        ret |= (state->components.latched_group == (int32_t) idx);
    ಯದಿ (type & XKB_STATE_LAYOUT_LOCKED)
        ret |= (state->components.locked_group == (int32_t) idx);

    ಪ್ರತ್ಯಾಗಮ ret;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
ಪೂರ್ಣಾಙ್ಕ
xkb_state_layout_name_is_active(ಸಂರಚನಾ xkb_state *state, ಧ್ರುವ ವರ್ಣ *name,
                                ಗಣನಾ xkb_state_component type)
{
    ಧ್ರುವ xkb_layout_index_t idx =
        xkb_keymap_layout_get_index(state->keymap, name);

    ಯದಿ (idx == XKB_LAYOUT_INVALID)
        ಪ್ರತ್ಯಾಗಮ -1;

    ಪ್ರತ್ಯಾಗಮ xkb_state_layout_index_is_active(state, idx, type);
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
ಪೂರ್ಣಾಙ್ಕ
xkb_state_led_index_is_active(ಸಂರಚನಾ xkb_state *state, xkb_led_index_t idx)
{
    ಯದಿ (idx >= state->keymap->ಅಂಕೆ_leds ||
        state->keymap->leds[idx].name == XKB_ATOM_NONE)
        ಪ್ರತ್ಯಾಗಮ -1;

    ಪ್ರತ್ಯಾಗಮ !!(state->components.leds & (UINT32_C(1) << idx));
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
ಪೂರ್ಣಾಙ್ಕ
xkb_state_led_name_is_active(ಸಂರಚನಾ xkb_state *state, ಧ್ರುವ ವರ್ಣ *name)
{
    ಧ್ರುವ xkb_led_index_t idx = xkb_keymap_led_get_index(state->keymap, name);

    ಯದಿ (idx == XKB_LED_INVALID)
        ಪ್ರತ್ಯಾಗಮ -1;

    ಪ್ರತ್ಯಾಗಮ xkb_state_led_index_is_active(state, idx);
}

/**
 * See:
 * - XkbTranslateKeyCode(3), mod_rtrn return value, from libX11.
 * - MyEnhancedXkbTranslateKeyCode(), a modification of the above, from GTK+.
 */
ಸ್ಥಿರ xkb_mod_mask_t
ಕೀಲಿ_get_consumed(ಸಂರಚನಾ xkb_state *state, ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                 ಗಣನಾ xkb_consumed_mode mode)
{
    ಧ್ರುವ xkb_layout_index_t group =
        xkb_state_ಕೀಲಿ_get_layout(state, ಕೀಲಿ->keycode);
    ಯದಿ (group == XKB_LAYOUT_INVALID)
        ಪ್ರತ್ಯಾಗಮ 0;

    xkb_mod_mask_t preserve = 0;
    xkb_mod_mask_t consumed = 0;

    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry* ಧ್ರುವ matching_entry =
        get_entry_for_ಕೀಲಿ_state(state, ಕೀಲಿ, group);
    ಯದಿ (matching_entry)
        preserve = matching_entry->preserve.mask;

    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type* ಧ್ರುವ type = ಕೀಲಿ->groups[group].type;
    ವಿಕಲ್ಪ (mode) {
    ಪಕ್ಷ XKB_CONSUMED_MODE_XKB:
        consumed = type->mods.mask;
        ಭಙ್ಗ;

    ಪಕ್ಷ XKB_CONSUMED_MODE_GTK: {
        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry* ಧ್ರುವ no_mods_entry =
            get_entry_for_mods(type, 0);
        ಧ್ರುವ xkb_level_index_t no_mods_leveli = no_mods_entry
                                               ? no_mods_entry->level
                                               : 0;
        ಧ್ರುವ ಸಂರಚನಾ xkb_level* ಧ್ರುವ no_mods_level =
            &ಕೀಲಿ->groups[group].levels[no_mods_leveli];

        ಪ್ರತಿ (darray_size_t i = 0; i < type->ಅಂಕೆ_entries; i++) {
            ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry* ಧ್ರುವ entry = &type->entries[i];
            ಯದಿ (!entry_is_active(entry))
                ಅನುವರ್ತ;

            ಧ್ರುವ ಸಂರಚನಾ xkb_level* ಧ್ರುವ level =
                &ಕೀಲಿ->groups[group].levels[entry->level];
            ಯದಿ (XkbLevelsSameSyms(level, no_mods_level))
                ಅನುವರ್ತ;

            ಯದಿ (entry == matching_entry || one_bit_set(entry->mods.mask))
                consumed |= entry->mods.mask & ~entry->preserve.mask;
        }
        ಭಙ್ಗ;
    }
    }

    ಪ್ರತ್ಯಾಗಮ consumed & ~preserve;
}

ಪೂರ್ಣಾಙ್ಕ
xkb_state_mod_index_is_consumed2(ಸಂರಚನಾ xkb_state *state, xkb_keycode_t kc,
                                 xkb_mod_index_t idx,
                                 ಗಣನಾ xkb_consumed_mode mode)
{
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ* ಧ್ರುವ ಕೀಲಿ = Xkb_ಕೀಲಿ(state->keymap, kc);

    ಯದಿ (unlikely(!ಕೀಲಿ || idx >= xkb_keymap_ಅಂಕೆ_mods(state->keymap)))
        ಪ್ರತ್ಯಾಗಮ -1;

    ಧ್ರುವ xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    ಯದಿ (!mapping) {
        /* Modifier not mapped */
        ಪ್ರತ್ಯಾಗಮ 0;
    }
    ಪ್ರತ್ಯಾಗಮ (mapping & ಕೀಲಿ_get_consumed(state, ಕೀಲಿ, mode)) == mapping;
}

ಪೂರ್ಣಾಙ್ಕ
xkb_state_mod_index_is_consumed(ಸಂರಚನಾ xkb_state *state, xkb_keycode_t kc,
                                xkb_mod_index_t idx)
{
    ಪ್ರತ್ಯಾಗಮ xkb_state_mod_index_is_consumed2(state, kc, idx,
                                            XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_mod_mask_remove_consumed(ಸಂರಚನಾ xkb_state *state, xkb_keycode_t kc,
                                   xkb_mod_mask_t mask)
{
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ* ಧ್ರುವ ಕೀಲಿ = Xkb_ಕೀಲಿ(state->keymap, kc);

    ಯದಿ (!ಕೀಲಿ)
        ಪ್ರತ್ಯಾಗಮ 0;

    ಪ್ರತ್ಯಾಗಮ resolve_to_canonical_mods(state->keymap, mask) &
           ~ಕೀಲಿ_get_consumed(state, ಕೀಲಿ, XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_ಕೀಲಿ_get_consumed_mods2(ಸಂರಚನಾ xkb_state *state, xkb_keycode_t kc,
                                 ಗಣನಾ xkb_consumed_mode mode)
{
    ವಿಕಲ್ಪ (mode) {
    ಪಕ್ಷ XKB_CONSUMED_MODE_XKB:
    ಪಕ್ಷ XKB_CONSUMED_MODE_GTK:
        ಭಙ್ಗ;
    ಸ್ವಭಾವ:
        log_err_func(state->keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "unrecognized consumed modifiers mode: %d\n", mode);
        ಪ್ರತ್ಯಾಗಮ 0;
    }

    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ* ಧ್ರುವ ಕೀಲಿ = Xkb_ಕೀಲಿ(state->keymap, kc);
    ಯದಿ (!ಕೀಲಿ)
        ಪ್ರತ್ಯಾಗಮ 0;

    ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ_get_consumed(state, ಕೀಲಿ, mode);
}

xkb_mod_mask_t
xkb_state_ಕೀಲಿ_get_consumed_mods(ಸಂರಚನಾ xkb_state *state, xkb_keycode_t kc)
{
    ಪ್ರತ್ಯಾಗಮ xkb_state_ಕೀಲಿ_get_consumed_mods2(state, kc, XKB_CONSUMED_MODE_XKB);
}

/*******************************************************************************
 * Server state machine API
 ******************************************************************************/

/** Entry to keep track of the state of an overlaid key */
ಸಂರಚನಾ xkb_overlaid_ಕೀಲಿ {
    /** The source key being overlaid */
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *old;
    /** The target key */
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *new;
    /** Counter of key currently down */
    ಪೂರ್ಣಾಙ್ಕ refcnt;
};

ಸಂರಚನಾ xkb_shortcuts_config_entry {
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
ಸಂರಚನಾ xkb_machine {
    /** Inherits from `xkb_server_state` */
    ಸಂರಚನಾ xkb_server_state base;

    /* Extra server-specific state data */

    /** Keyboard overlays handling */
    ಸಂರಚನಾ {
        /**
         * Current overlaid keys
         *
         * When a key is overlaid, it remained overlaid in this configuration
         * until totally released, i.e. every key press was matched with the
         * corresponding key release.
         */
        darray(ಸಂರಚನಾ xkb_overlaid_ಕೀಲಿ) ಕೀಲಿಗಳು;
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
    ಸಂರಚನಾ machine_config {
        /** Modifiers remapping */
        ಸಂರಚನಾ machine_modifiers_config {
            /** Real modifier mask */
            xkb_mod_mask_t mask;
            /** Modifiers re-mappings */
            darray_size_t mappings_ಅಂಕೆ;
            ಸಂರಚನಾ machine_mods_mapping {
                xkb_mod_mask_t source;
                xkb_mod_mask_t target;
            } * mappings ATTR_COUNTED_BY(mappings_ಅಂಕೆ);
        } modifiers;

        /** Shortcuts tweak */
        ಸಂರಚನಾ machine_shortcuts_config {
            /** Substitution entries */
            ಸಂರಚನಾ xkb_shortcuts_config_entry *entries;
        } shortcuts;
    } config;

    /** Mouse keys handling */
    ಸಂರಚನಾ {
        /** Default pointer button */
        xkb_pointer_button_index_t default_button;
        /** Mask of the locked buttons */
        xkb_pointer_button_mask_t locked_buttons;
    } mouse;

    ಗಣನಾ xkb_machine_flags flags;
};

ಪ್ರಕಾರನಿರ್ದೇಶ darray(ಸಂರಚನಾ machine_mods_mapping) machine_mods_mappings;

ಸಂರಚನಾ xkb_machine_builder {
    ಸಂರಚನಾ xkb_keymap *keymap;

    /** Controls */
    ಸಂರಚನಾ {
        /** Accessibility flags */
        ಸಂರಚನಾ {
            ಗಣನಾ xkb_a11y_flags affect_flags;
            ಗಣನಾ xkb_a11y_flags flags;
        } a11y;
    } controls;

    /** Modifiers re-mapping */
    machine_mods_mappings mods;

    /** Shortcuts tweak */
    ಸಂರಚನಾ xkb_shortcuts_config_options {
        /** Substitution entries */
        darray(ಸಂರಚನಾ xkb_shortcuts_config_entry) entries;
    } shortcuts;

    ಗಣನಾ xkb_machine_flags machine_flags;
    ಗಣನಾ xkb_machine_builder_flags builder_flags;
    ಪೂರ್ಣಾಙ್ಕ refcnt;
};

ಸಂರಚನಾ xkb_machine_builder *
xkb_machine_builder_new(
    ಸಂರಚನಾ xkb_keymap * restrict keymap,
    ಧ್ರುವ ಸಂರಚನಾ xkb_machine_builder_config * restrict config,
    ಗಣನಾ xkb_status * restrict status
)
{
    /* Handle default configuration */
    ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ xkb_machine_builder_config default_config = {
        .size = ಪರಿಮಾಣ(default_config)
    };
    ಯದಿ (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ಧ್ರುವ ಗಣನಾ xkb_status status_ = xkb_check_state_abi(config);
    ಯದಿ (status_) {
        xkb_log_abi_error(keymap->ctx, __func__, status_);
        ಯದಿ (status)
            *status = status_;
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    /* Sanitize input */
    ಧ್ರುವ ಗಣನಾ xkb_machine_builder_flags invalid_builder_flags
        = config->builder_flags
        & ~(ಗಣನಾ xkb_machine_builder_flags)XKB_MACHINE_BUILDER_FLAGS_ಮೌಲ್ಯಗಳು;
    ಯದಿ (invalid_builder_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS_,
                     "unrecognized machine builder flags: 0x%x\n",
                     invalid_builder_flags);
        ಯದಿ (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS;
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಧ್ರುವ ಗಣನಾ xkb_machine_flags invalid_machine_flags
        = (ಗಣನಾ xkb_machine_flags)config->machine_flags
        & ~(ಗಣನಾ xkb_machine_flags)XKB_MACHINE_FLAGS_ಮೌಲ್ಯಗಳು;
    ಯದಿ (invalid_machine_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS_,
                     "unrecognized state machine flags: 0x%x\n",
                     invalid_machine_flags);
        ಯದಿ (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS;
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಸಂರಚನಾ xkb_machine_builder * ಧ್ರುವ builder = calloc(1, ಪರಿಮಾಣ(*builder));
    ಯದಿ (!builder) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine builder\n");
        ಯದಿ (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    *builder = (ಸಂರಚನಾ xkb_machine_builder) {
        .keymap = xkb_keymap_ref(keymap),
        .refcnt = 1,
        .builder_flags = (ಗಣನಾ xkb_machine_builder_flags)config->builder_flags,
        .machine_flags = (ಗಣನಾ xkb_machine_flags)config->machine_flags,
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

    ಯದಿ (status)
        *status = XKB_SUCCESS;

    ಪ್ರತ್ಯಾಗಮ builder;
}

ಸಂರಚನಾ xkb_machine_builder *
xkb_machine_builder_ref(ಸಂರಚನಾ xkb_machine_builder *builder)
{
    ಪ್ರತಿಜ್ಞಾ(builder->refcnt > 0);
    builder->refcnt++;
    ಪ್ರತ್ಯಾಗಮ builder;
}

ಶೂನ್ಯ
xkb_machine_builder_unref(ಸಂರಚನಾ xkb_machine_builder *builder)
{
    ಪ್ರತಿಜ್ಞಾ(!builder || builder->refcnt > 0);
    ಯದಿ (!builder || --builder->refcnt > 0)
        ಪ್ರತ್ಯಾಗಮ;

    darray_free(builder->shortcuts.entries);
    darray_free(builder->mods);
    xkb_keymap_unref(builder->keymap);
    ಮುಕ್ತ(builder);
}

ಸಂರಚನಾ xkb_keymap *
xkb_machine_builder_get_keymap(ಧ್ರುವ ಸಂರಚನಾ xkb_machine_builder *builder)
{
    /* Reference count is not updated. See API doc. */
    ಪ್ರತ್ಯಾಗಮ builder->keymap;
}

ಗಣನಾ xkb_status
xkb_machine_builder_update_a11y(
    ಸಂರಚನಾ xkb_machine_builder * restrict builder,
    ಧ್ರುವ ಸಂರಚನಾ xkb_machine_builder_a11y_update * restrict update)
{
    /* Check ABI compatibility */
    ಗಣನಾ xkb_status status = xkb_check_state_abi(update);
    ಯದಿ (status != XKB_SUCCESS) {
        xkb_log_abi_error(builder->keymap->ctx, __func__, status);
        ಪ್ರತ್ಯಾಗಮ status;
    }

    ಧ್ರುವ ಗಣನಾ xkb_a11y_flags invalid_flags =
        ~(ಗಣನಾ xkb_a11y_flags)XKB_A11_Y_FLAGS_ಮೌಲ್ಯಗಳು;

    ಯದಿ (update->affect_flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y affected flags: %#x\n",
                __func__, update->affect_flags & invalid_flags);
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }
    ಯದಿ (update->flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y flags: %#x\n",
                __func__, update->flags & invalid_flags);
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }

    builder->controls.a11y.affect_flags |= update->affect_flags;
    builder->controls.a11y.flags &= ~update->affect_flags;
    builder->controls.a11y.flags |= (update->flags & update->affect_flags);

    ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
}

ಗಣನಾ xkb_status
xkb_machine_builder_update_mods_remap(
    ಸಂರಚನಾ xkb_machine_builder * restrict builder,
    ಧ್ರುವ ಸಂರಚನಾ xkb_machine_builder_mods_remap_update * restrict update
)
{
    ಯದಿ (!update->source) {
        ಯದಿ (!update->target) {
            /* Reset mappings */
            darray_resize(builder->mods, 0);
            ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
        } ಅನ್ಯಥಾ {
            ಪ್ರತ್ಯಾಗಮ XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
        }
    }

    /* Check the modifiers against the keymap */
    ಧ್ರುವ xkb_mod_mask_t invalid = ~builder->keymap->canonical_state_mask;
    ಯದಿ ((update->source & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid source modifiers: 0x%"PRIx32"\n",
                __func__, update->source);
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    ಯದಿ ((update->target & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid target modifiers: 0x%"PRIx32"\n",
                __func__, update->target);
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    ಸಂರಚನಾ machine_mods_mapping *mapping = ರಿಕ್ತ;
    darray_size_t m = 0;
    darray_enumerate(m, mapping, builder->mods) {
        ಯದಿ (mapping->source == update->source) {
            ಯದಿ (!update->target) {
                /* Remove mapping */
                darray_remove(builder->mods, m);
            } ಅನ್ಯಥಾ {
                /* Update mapping */
                mapping->target = update->target;
            }
            ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
        }
    }

    ಯದಿ (update->target) {
        /* Append new mapping */
        darray_append(
            builder->mods,
            (ಸಂರಚನಾ machine_mods_mapping) {
                .source = update->source,
                .target = update->target
            }
        );
    } ಅನ್ಯಥಾ {
        /* Ignore missing mapping */
    }

    ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
}

ಗಣನಾ xkb_status
xkb_machine_builder_update_shortcut_override(
    ಸಂರಚನಾ xkb_machine_builder * restrict builder,
    ಧ್ರುವ ಸಂರಚನಾ xkb_machine_builder_shortcut_override_update * restrict update
)
{
    ಸಂರಚನಾ xkb_keymap *keymap = builder->keymap;

    /* Check the modifiers against the keymap */
    ಧ್ರುವ xkb_mod_mask_t invalid = ~keymap->canonical_state_mask;
    ಯದಿ ((update->affect_mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid affected modifiers: 0x%"PRIx32"\n",
                __func__, update->affect_mods);
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    ಯದಿ ((update->mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid modifiers: 0x%"PRIx32"\n",
                __func__, update->mods);
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    /* Check the layout indices against the keymap */
    ಯದಿ (update->source >= keymap->ಅಂಕೆ_groups &&
        update->source != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "%s: Invalid source layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->ಅಂಕೆ_groups, update->source + 1);
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }
    ಯದಿ (update->target >= keymap->ಅಂಕೆ_groups &&
        update->target != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "%s: Invalid target layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->ಅಂಕೆ_groups, update->target + 1);
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    ಸಂರಚನಾ xkb_shortcuts_config_options * ಧ್ರುವ config = &builder->shortcuts;

    /* Resize array & initialize new entries, if relevant */
    ಯದಿ (update->source >= darray_size(config->entries) &&
        update->source != XKB_LAYOUT_INVALID) {
        ಯದಿ (update->target == update->source) {
            /* Skip default setting */
            ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
        }
        xkb_layout_index_t new = darray_size(config->entries);
        darray_resize(config->entries, update->source + 1);
        ಪ್ರತಿ (; new <= update->source; new++) {
            darray_item(config->entries, new) =
                (ಸಂರಚನಾ xkb_shortcuts_config_entry) {
                    .target = XKB_LAYOUT_INVALID,
                    .mods = 0
                };
        }
    }

    ಸಂರಚನಾ xkb_shortcuts_config_entry *entry;
    xkb_layout_index_t source = (update->source == XKB_LAYOUT_INVALID)
        ? 0               /* Loop over all entries */
        : update->source; /* Single entry */
    darray_foreach_from(entry, config->entries, source) {
        ಯದಿ (update->source == XKB_LAYOUT_INVALID &&
            entry->target == XKB_LAYOUT_INVALID) {
            /* Skip previous deactivated entry */
            source++;
            ಅನುವರ್ತ;
        }

        ಯದಿ (update->affect_mods) {
            entry->mods &= ~update->affect_mods;
            entry->mods |= (update->affect_mods & update->mods);
        }

        ಯದಿ (update->source != XKB_LAYOUT_INVALID) {
            entry->target = (update->target == source)
                ? XKB_LAYOUT_INVALID
                : update->target;
            /* Single entry: exit loop */
            ಭಙ್ಗ;
        }
        source++;
    }

    ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
}

/** Compare 2 modifiers combinations */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
cmp_mod_masks(ಧ್ರುವ ಶೂನ್ಯ *a, ಧ್ರುವ ಶೂನ್ಯ *b)
{
    ಧ್ರುವ xkb_mod_mask_t m1 = *((xkb_mod_mask_t *) a);
    ಧ್ರುವ xkb_mod_mask_t m2 = *((xkb_mod_mask_t *) b);
    ಯದಿ (m1 == m2)
        ಪ್ರತ್ಯಾಗಮ 0;

    ಯದಿ (!m1)
        ಪ್ರತ್ಯಾಗಮ +1;
    ಯದಿ (!m2)
        ಪ್ರತ್ಯಾಗಮ -1;

    ಧ್ರುವ xkb_mod_mask_t overlap = (m1 & m2);
    ಯದಿ (overlap == m1)
        ಪ್ರತ್ಯಾಗಮ +1; /* m1 is a subset of m2 */
    ಯದಿ (overlap == m2)
        ಪ್ರತ್ಯಾಗಮ -1; /* m2 is a subset of m1 */

    /* Compare active modifiers numbers */
    ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ count1 = popcount32(m1);
    ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ count2 = popcount32(m2);
    ಯದಿ (count1 > count2)
        ಪ್ರತ್ಯಾಗಮ -1;
    ಯದಿ (count1 < count2)
        ಪ್ರತ್ಯಾಗಮ +1;

    /* Give priority to real modifiers */
    ಪ್ರತ್ಯಾಗಮ (m1 < m2) ? -1 : +1;
}

ಸ್ಥಿರ ಗಣನಾ xkb_status
machine_set_mods(ಸಂರಚನಾ xkb_machine *sm,
                 ಧ್ರುವ machine_mods_mappings *raw_mappings)
{
    ಯದಿ (!darray_empty(*raw_mappings)) {
        machine_mods_mappings mappings = darray_new();
        xkb_mod_mask_t mask = 0;
        ಧ್ರುವ ಸಂರಚನಾ machine_mods_mapping *mapping;
        ಧ್ರುವ xkb_mod_mask_t invalid =
            ~sm->base.base.keymap->canonical_state_mask;
        darray_foreach(mapping, *raw_mappings) {
            ಯದಿ (!mapping->source || !mapping->target ||
                (mapping->source & invalid) || (mapping->target & invalid))
                ಅನುವರ್ತ;
            darray_append(mappings, *mapping);
            mask |= mapping->source;
        }

        /*
         * Re-order the mappings, so that combinations have higher priority
         * if they are superset of other combinations.
         * For other type of overlaps, see the sorting function.
         */
        qsort(darray_ವಸ್ತುಗಳು(mappings), darray_size(mappings),
              ಪರಿಮಾಣ(*darray_ವಸ್ತುಗಳು(mappings)), &cmp_mod_masks);

        darray_steal(
            mappings,
            &sm->config.modifiers.mappings,
            &sm->config.modifiers.mappings_ಅಂಕೆ
        );
        sm->config.modifiers.mask = mask;
    } ಅನ್ಯಥಾ {
        sm->config.modifiers = (ಸಂರಚನಾ machine_modifiers_config) {
            .mappings = ರಿಕ್ತ,
            .mappings_ಅಂಕೆ = 0,
            .mask = 0
        };
    }

    ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
}

ಸ್ಥಿರ ಗಣನಾ xkb_status
machine_set_shortcuts(
    ಸಂರಚನಾ xkb_machine * restrict sm,
    ಧ್ರುವ ಸಂರಚನಾ xkb_shortcuts_config_options * restrict options
)
{
    ಯದಿ (darray_empty(options->entries)) {
        sm->config.shortcuts = (ಸಂರಚನಾ machine_shortcuts_config) {
            .entries = ರಿಕ್ತ
        };
        ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
    }

    ಸಂರಚನಾ xkb_keymap * ಧ್ರುವ keymap = sm->base.base.keymap;

    ಸಂರಚನಾ xkb_shortcuts_config_entry * entries =
        calloc(keymap->ಅಂಕೆ_groups, ಪರಿಮಾಣ(*entries));
    ಯದಿ (!entries)
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_ALLOCATION_FAILURE;

    ಧ್ರುವ xkb_layout_index_t ಎಣಿಕೆ = MIN(
        keymap->ಅಂಕೆ_groups,
        (xkb_layout_index_t) darray_size(options->entries)
    );

    bool some_entries = false;
    ಪ್ರತಿ (xkb_layout_index_t l = 0; l < ಎಣಿಕೆ; l++) {
        /* Sanitize layout target */
        ಧ್ರುವ xkb_layout_index_t target =
            darray_item(options->entries, l).target;
        /* Sanitize modifier mask */
        ಧ್ರುವ xkb_mod_mask_t mods = (
            darray_item(options->entries, l).mods &
            sm->base.base.keymap->canonical_state_mask
        );
        ಯದಿ (target < keymap->ಅಂಕೆ_groups && target != l && mods) {
            entries[l] = (ಸಂರಚನಾ xkb_shortcuts_config_entry) {
                .target = target,
                .mods = mods
            };
            some_entries = true;
        } ಅನ್ಯಥಾ {
            entries[l] = (ಸಂರಚನಾ xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
            ಅನುವರ್ತ;
        }
    }

    ಯದಿ (!some_entries) {
        ಮುಕ್ತ(entries);
        entries = ರಿಕ್ತ;
    } ಅನ್ಯಥಾ {
        /* Initialize groups with no entries */
        ಪ್ರತಿ (xkb_layout_index_t l = ಎಣಿಕೆ; l < keymap->ಅಂಕೆ_groups; l++) {
            entries[l] = (ಸಂರಚನಾ xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
        }
    }

    sm->config.shortcuts = (ಸಂರಚನಾ machine_shortcuts_config) {
        .entries = entries
    };
    ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
}

ಸಂರಚನಾ xkb_machine *
xkb_machine_new(ಧ್ರುವ ಸಂರಚನಾ xkb_machine_builder * restrict builder,
                ಗಣನಾ xkb_status * restrict status)
{
    ಸಂರಚನಾ xkb_machine * ಧ್ರುವ machine = calloc(1, ಪರಿಮಾಣ(*machine));
    ಯದಿ (!machine) {
        log_err_func1(builder->keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine\n");
        ಯದಿ (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    xkb_server_state_init(&machine->base, builder->keymap, SERVER_STATE,
                          builder->controls.a11y.affect_flags,
                          builder->controls.a11y.flags);
    machine->flags = builder->machine_flags;
    ಯದಿ (!(machine->flags &
          XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_ಕೀಲಿ_FIX))
    {
        machine->base.flags |= STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_ಕೀಲಿ;
    }
    ಯದಿ (machine->flags & XKB_MACHINE_UNLOCK_MODS_ON_PRESS) {
        machine->base.flags |= STATE_UNLOCK_MODS_ON_PRESS;
    }

    ಗಣನಾ xkb_status status_;
    ಯದಿ ((status_ = machine_set_mods(machine, &builder->mods)) != XKB_SUCCESS ||
        (status_ = machine_set_shortcuts(machine, &builder->shortcuts)) !=
        XKB_SUCCESS) {
        *status = status_;
        ಗಚ್ಛ error;
    }

    darray_init(machine->overlays.ಕೀಲಿಗಳು);
    machine->mouse.default_button = XKB_POINTER_BUTTON_MIN;

    ಯದಿ (status)
        *status = XKB_SUCCESS;

    ಪ್ರತ್ಯಾಗಮ machine;

error:
    xkb_machine_unref(machine);
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸಂರಚನಾ xkb_machine *
xkb_machine_ref(ಸಂರಚನಾ xkb_machine *sm)
{
    ಪ್ರತಿಜ್ಞಾ(sm->base.base.refcnt > 0);
    sm->base.base.refcnt++;
    ಪ್ರತ್ಯಾಗಮ sm;
}

ಶೂನ್ಯ
xkb_machine_unref(ಸಂರಚನಾ xkb_machine *sm)
{
    ಪ್ರತಿಜ್ಞಾ(!sm || sm->base.base.refcnt > 0);
    ಯದಿ (!sm || --sm->base.base.refcnt > 0)
        ಪ್ರತ್ಯಾಗಮ;

    xkb_state_destroy(&sm->base.base);
    darray_free(sm->overlays.ಕೀಲಿಗಳು);
    ಮುಕ್ತ(sm->config.shortcuts.entries);
    ಮುಕ್ತ(sm->config.modifiers.mappings);
    ಮುಕ್ತ(sm);
}

ಸಂರಚನಾ xkb_keymap *
xkb_machine_get_keymap(ಧ್ರುವ ಸಂರಚನಾ xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    ಪ್ರತ್ಯಾಗಮ sm->base.base.keymap;
}

ಸಂರಚನಾ xkb_state *
xkb_machine_get_state(ಸಂರಚನಾ xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    ಪ್ರತ್ಯಾಗಮ (ಸಂರಚನಾ xkb_state *)sm;
}

ಸಂರಚನಾ xkb_state *
xkb_state_new_from_machine(ಧ್ರುವ ಸಂರಚನಾ xkb_machine * restrict machine,
                           ಗಣನಾ xkb_status * restrict status)
{
    /* Creation */
    ಗಣನಾ xkb_status status_;
    ಸಂರಚನಾ xkb_state * ಧ್ರುವ state = xkb_state_new_with_mode(
        machine->base.base.keymap,
        XKB_STATE_MODE_SERVER_QUERY,
        &status_
    );
    ಯದಿ (status_ != XKB_SUCCESS) {
        ಯದಿ (status)
            *status = status_;
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಪ್ರತಿಜ್ಞಾ(state->mode == SERVER_COMPANION);
    ಪ್ರತಿಜ್ಞಾ(state->refcnt == 1);

    /* Copy machine’s state */
    *state = machine->base.base;

    /* Restore previous values */
    state->mode = SERVER_COMPANION;
    state->refcnt = 1;

    ಯದಿ (status)
        *status = XKB_SUCCESS;
    ಪ್ರತ್ಯಾಗಮ state;
}

ಸ್ಥಿರ ಶೂನ್ಯ
machine_update_overlays(ಸಂರಚನಾ xkb_machine *sm)
{
    /*
     * Overlays indices are stored 1-indexed in nibbles: the lowest
     * nibble corresponds to the latest activated index.
     */

    ಧ್ರುವ xkb_overlay_mask_t mask =
        overlays_from_controls(sm->base.base.components.controls);
    xkb_overlay_mask_t added = mask & ~sm->overlays.enabled;

    /* Remove overlays no longer enabled and keep relative order */
    uint32_t order = sm->overlays.order;
    ಧ್ರುವ xkb_overlay_index_t overlay_max =
        format_max_overlays(sm->base.base.keymap->format);
    ಪ್ರತಿ (uint8_t n = 0; n < overlay_max;) {
        xkb_overlay_index_t overlay_idx = (order >> (n * 4)) & 0xf;
        ಯದಿ (!overlay_idx)
            ಭಙ್ಗ;
        ಧ್ರುವ xkb_overlay_mask_t overlay_mask =
            (xkb_overlay_mask_t)(1u << (overlay_idx - 1u) /* k is 1-indexed */);
        ಯದಿ (overlay_mask & mask) {
            /* no duplicates */
            added &= ~overlay_mask;
            n++;
        } ಅನ್ಯಥಾ {
            /* remove overlay */
            ಧ್ರುವ uint32_t head = (UINT32_C(1) << (n * 4)) - 1;
            order = (order & head) | ((order >> 4) & ~head);
        }
    }

    /* Add new overlays in ascending order */
    ಪ್ರತಿ (xkb_overlay_index_t k = 0; added; k++, added >>= 1) {
        ಯದಿ (added & 0x1) {
            order <<= 4;
            order |= k + 1u; /* k is 0-indexed */
        }
    }

    sm->overlays.order = order;
    sm->overlays.enabled = mask;
}

ಗಣನಾ xkb_status
xkb_machine_process_synthetic(ಸಂರಚನಾ xkb_machine *sm,
                              ಧ್ರುವ ಸಂರಚನಾ xkb_synthetic_update *update,
                              ಸಂರಚನಾ xkb_events *events)
{
    /* Check ABI compatibility */
    ಗಣನಾ xkb_status status =
        check_state_update_abi(sm->base.base.keymap->ctx, update);
    ಯದಿ (status != XKB_SUCCESS)
        ಪ್ರತ್ಯಾಗಮ status;

    ಸಂರಚನಾ xkb_server_state * ಧ್ರುವ state = &sm->base;
    ಧ್ರುವ ಸಂರಚನಾ state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    darray_size(events->queue) = 0;
    events->next = 0;

    /* Update parametrized controls first */
    ಯದಿ (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        ಯದಿ (status != XKB_SUCCESS)
            ಪ್ರತ್ಯಾಗಮ status;
    }

    ಯದಿ (update->components) {
        ಧ್ರುವ ಸಂರಚನಾ xkb_state_components_update * ಧ್ರುವ components =
            update->components;
        /* Update boolean controls first */
        ಯದಿ (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, events);
        }

        state_update_latched_locked(state, components, events);
    }

    xkb_state_update_derived(&state->base);

    ಧ್ರುವ ಗಣನಾ xkb_state_component changed = get_state_component_changes(
        &previous_components, &state->base.components
    );
    ಯದಿ (changed) {
        // TODO: latch controls
        ಯದಿ (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        /* Create event only if some component actually changed */
        darray_append(events->queue, (ಸಂರಚನಾ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .changed = changed,
                .components = state->base.components
            }
        });

        /* Terminate the frame */
        darray_append(events->queue, (ಸಂರಚನಾ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
}

ಸ್ಥಿರ ssize_t
do_remap_modifiers(ಧ್ರುವ ಸಂರಚನಾ machine_modifiers_config * restrict mappings,
                   ಸಂರಚನಾ xkb_state * restrict state,
                   ಸಂರಚನಾ xkb_events * restrict events,
                   ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ * restrict ಕೀಲಿ)
{
    ಯದಿ (!(mappings->mask & state->components.mods))
        ಪ್ರತ್ಯಾಗಮ -1;

    ಧ್ರುವ xkb_layout_index_t layout = state_ಕೀಲಿ_get_layout(state, ಕೀಲಿ);
    ಯದಿ (layout >= ಕೀಲಿ->ಅಂಕೆ_groups)
        ಪ್ರತ್ಯಾಗಮ -1;

    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type * ಧ್ರುವ type = ಕೀಲಿ->groups[layout].type;
    xkb_mod_mask_t affect_mods = 0;
    xkb_mod_mask_t mods = 0;
    ಪ್ರತಿ (darray_size_t m = 0; m < mappings->mappings_ಅಂಕೆ; m++) {
        ಧ್ರುವ ಸಂರಚನಾ machine_mods_mapping * ಧ್ರುವ mapping =
            &mappings->mappings[m];
        ಯದಿ (/* Skip not matching active mods */
            (mapping->source & state->components.mods) == mapping->source &&
            /* Skip overlap with previous active mappings */
            !(mapping->source & affect_mods) &&
            /* Skip if the key type uses some of the source modifiers */
            !(mapping->source & type->mods.mask)) {
            affect_mods |= mappings->mappings[m].source;
            mods |= mappings->mappings[m].target;
        }
    }

    ಯದಿ (!affect_mods)
        ಪ್ರತ್ಯಾಗಮ -1;

    ಸಂರಚನಾ xkb_state new = *state;
    new.components.base_mods = (new.components.base_mods & ~affect_mods) | mods;
    new.components.latched_mods = (new.components.latched_mods & ~affect_mods);
    new.components.locked_mods = (new.components.locked_mods & ~affect_mods);
    xkb_state_update_derived(&new);

    ssize_t event_idx = -1;
    ಧ್ರುವ ಗಣನಾ xkb_state_component changed =
        get_state_component_changes(&state->components, &new.components);
    ಯದಿ (changed) {
        event_idx = (ssize_t) darray_size(events->queue);
        darray_append(events->queue, (ಸಂರಚನಾ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = new.components,
                .changed = changed
            }
        });
    }

    state->components.mods = new.components.mods;
    ಪ್ರತ್ಯಾಗಮ event_idx;
}

/**
 * Activate an alternative layout if some modifiers are active, typically
 * `Control`, `Alt` or `Super`.
 *
 * The tweak consists of computing a *base* layout such that the *effective*
 * layout is the corresponding target layout.
 */
ಸ್ಥಿರ ssize_t
do_shortcuts_tweak(ಧ್ರುವ ಸಂರಚನಾ machine_shortcuts_config *config,
                   ಸಂರಚನಾ xkb_state *state,
                   ಧ್ರುವ ಸಂರಚನಾ state_components *previous_components,
                   ಸಂರಚನಾ xkb_events *events, ssize_t remap_event)
{
    ಯದಿ (!config->entries) {
        /* No shortcuts tweak */
        ಪ್ರತ್ಯಾಗಮ remap_event;
    }

    ಧ್ರುವ ಸಂರಚನಾ xkb_shortcuts_config_entry * ಧ್ರುವ entry =
        &config->entries[state->components.group];

    ಯದಿ (entry->target != XKB_LAYOUT_INVALID &&
        (entry->mods & state->components.mods)) {
        /*
         * Activate shortcuts tweak:
         * 1. The real base group is saved by the caller, to be restored by
         *    later by a call to undo_tweaks().
         * 2. Compute the new base group so that the resulting effective group
         *    is the target.
         * 3. Recompute derived state components.
         */
        ಸಂರಚನಾ xkb_state new = *state;
        ಯದಿ (remap_event < 0) {
            /* Create new event */
            remap_event = (ssize_t) darray_size(events->queue);
            darray_append(events->queue, (ಸಂರಚನಾ xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = {0},
                    .changed = 0
                }
            });
        } ಅನ್ಯಥಾ {
            /* Merge with last remap event */
            new.components =
                darray_item(events->queue, remap_event).components.components;
        }
        new.components.base_group
            = (int32_t) entry->target
            - state->components.latched_group
            - state->components.locked_group;
        xkb_state_update_derived(&new);

        ಧ್ರುವ ಗಣನಾ xkb_state_component changed =
            get_state_component_changes(previous_components, &new.components);
        darray_item(events->queue, remap_event).components.changed = changed;
        darray_item(events->queue, remap_event).components.components =
            new.components;

        state->components.group = new.components.group;
    } ಅನ್ಯಥಾ {
        /* Deactivated entry or modifiers do not match */
    }
    ಪ್ರತ್ಯಾಗಮ remap_event;
}

ಸ್ಥಿರ ಶೂನ್ಯ
undo_tweaks(ಧ್ರುವ ಸಂರಚನಾ xkb_state *state,
            ಧ್ರುವ ಸಂರಚನಾ state_components *initial_components,
            ಸಂರಚನಾ xkb_events *events)
{
    /* Get last component event */
    ಧ್ರುವ ಸಂರಚನಾ xkb_event *event = ರಿಕ್ತ;
    darray_foreach_reverse(event, events->queue) {
        ಯದಿ (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS)
            ಭಙ್ಗ;
    }
    ಯದಿ (!event)
        ಪ್ರತ್ಯಾಗಮ;

    /* Restore state */
    ಧ್ರುವ ಗಣನಾ xkb_state_component changed =
        get_state_component_changes(initial_components,
                                    &event->components.components);
    ಯದಿ (changed) {
        darray_append(events->queue, (ಸಂರಚನಾ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = *initial_components,
                .changed = changed
            }
        });
    }
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *
process_overlayable_ಕೀಲಿ(ಸಂರಚನಾ xkb_machine *sm,
                        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ * ಕೀಲಿ,
                        ಗಣನಾ xkb_ಕೀಲಿ_direction direction)
{
    /* Check if key is currently overlaid */
    ಸಂರಚನಾ xkb_overlaid_ಕೀಲಿ *entry = ರಿಕ್ತ;
    ಸಂರಚನಾ xkb_overlaid_ಕೀಲಿ *available_entry = ರಿಕ್ತ;
    darray_foreach(entry, sm->overlays.ಕೀಲಿಗಳು) {
        ಯದಿ (entry->old == ಕೀಲಿ) {
            /* Maintain the key overlaid */
            ವಿಕಲ್ಪ (direction) {
            ಪಕ್ಷ XKB_ಕೀಲಿ_DOWN:
                entry->refcnt++;
                ಭಙ್ಗ;
            ಪಕ್ಷ XKB_ಕೀಲಿ_REPEATED:
                ಭಙ್ಗ;
            ಪಕ್ಷ XKB_ಕೀಲಿ_UP:
            ಸ್ವಭಾವ:
                entry->refcnt--;
                ಭಙ್ಗ;
            }
            ಯದಿ (entry->refcnt <= 0)
                entry->old = ರಿಕ್ತ;
            ಪ್ರತ್ಯಾಗಮ entry->new;
        } ಅನ್ಯಥಾ ಯದಿ (!entry->old && !available_entry) {
            /* First free spot */
            available_entry = entry;
        }
    }
    ಯದಿ (direction == XKB_ಕೀಲಿ_DOWN) {
        /* Enable key overlay */

        /*
         * Get overlay index: select the latest activated overlay
         * containing the key.
         */
        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *new = ಕೀಲಿ;

        ಯದಿ (ಕೀಲಿ->overlays & sm->overlays.enabled) {
            /* Some relevant overlay is active */
            ಪ್ರತಿ (uint32_t ರಾಶಿ = sm->overlays.order; ರಾಶಿ; ರಾಶಿ >>= 4) {
                static_assert(XKB_OVERLAY_ಎಣಿಕೆ == 8, "");
                ಧ್ರುವ xkb_overlay_index_t overlay = (ರಾಶಿ & 0xf) - 1;
                ಧ್ರುವ xkb_overlay_mask_t mask =
                    (xkb_overlay_mask_t)(1u << overlay);
                ಯದಿ (ಕೀಲಿ->overlays & mask) {
                    /* Get new key */
                    ಯದಿ (ಕೀಲಿ->overlays_inline) {
                        /* Stored inline */
                        new = ಕೀಲಿ->overlay_ಕೀಲಿ;
                    } ಅನ್ಯಥಾ {
                        /* Stored in sparse array */
                        ಧ್ರುವ xkb_overlay_mask_t ಕೆಳಗಿನ = (xkb_overlay_mask_t)(
                            ಕೀಲಿ->overlays & (xkb_overlay_mask_t)(mask - 1u)
                        );
                        /* index = number of set bits strictly below bit */
                        ಧ್ರುವ xkb_overlay_index_t index =
                            (xkb_overlay_index_t)popcount32(ಕೆಳಗಿನ);
                        new = ಕೀಲಿ->overlays_ಕೀಲಿಗಳು[index];
                    }
                    ಭಙ್ಗ;
                }
            }
        } ಅನ್ಯಥಾ {
            /*
             * No relevant overlay is active: block future overlays while
             * the key is pressed.
             */
        }

        /* Get new entry */
        ಯದಿ (available_entry) {
            /* Re-use a free entry */
            entry = available_entry;
        } ಅನ್ಯಥಾ {
            /* Append a new entry */
            ಧ್ರುವ darray_size_t idx = darray_size(sm->overlays.ಕೀಲಿಗಳು);
            darray_resize(sm->overlays.ಕೀಲಿಗಳು, idx + 1);
            entry = &darray_item(sm->overlays.ಕೀಲಿಗಳು, idx);
        }

        /* Overlay the key */
        entry->old = ಕೀಲಿ;
        entry->new = new;
        entry->refcnt = 1;
        ಪ್ರತ್ಯಾಗಮ new;
    }

    /* No overlay */
    ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ;
}

ಗಣನಾ xkb_status
xkb_machine_process_ಕೀಲಿ(ಸಂರಚನಾ xkb_machine *sm,
                        xkb_keycode_t kc, ಗಣನಾ xkb_ಕೀಲಿ_direction direction,
                        ಸಂರಚನಾ xkb_events *events)
{
    darray_size(events->queue) = 0;
    events->next = 0;

    ಸಂರಚನಾ xkb_server_state * ಧ್ರುವ state = &sm->base;
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ * ಕೀಲಿ = Xkb_ಕೀಲಿ(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    ಯದಿ (!ಕೀಲಿ || (direction == XKB_ಕೀಲಿ_REPEATED && !ಕೀಲಿ->repeats))
        ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;

    ಧ್ರುವ ಸಂರಚನಾ state_components initial_components = state->base.components;

    ಯದಿ (direction == XKB_ಕೀಲಿ_DOWN &&
        (sm->base.base.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ಕೀಲಿಗಳು) &&
        (sm->base.a11y_flags & XKB_A11_Y_STICKY_ಕೀಲಿಗಳು_NO_SIMULTANEOUS_ಕೀಲಿಗಳು) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ಕೀಲಿಗಳು,
                                      0, events);
    }

    ಯದಿ (ಕೀಲಿ->overlays)
        ಕೀಲಿ = process_overlayable_ಕೀಲಿ(sm, ಕೀಲಿ, direction);

    ssize_t remap_event = do_remap_modifiers(&sm->config.modifiers,
                                             &state->base, events, ಕೀಲಿ);

    remap_event = do_shortcuts_tweak(&sm->config.shortcuts, &state->base,
                                     &initial_components, events, remap_event);

    ಯದಿ (remap_event >= 0) {
        // TODO: what about merging state event for RedirectKey?
        darray_append(events->queue, (ಸಂರಚನಾ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    state->flags |= STATE_REQUIRE_ಕೀಲಿ_EVENT;
    state->set_mods = 0;
    state->clear_mods = 0;

    /* Handle key actions */
    xkb_filter_apply_all(state, events, ಕೀಲಿ, direction);

    xkb_mod_index_t i;
    xkb_mod_mask_t bit;
    ಪ್ರತಿ (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        ಯದಿ (state->set_mods & bit) {
            state->mod_ಕೀಲಿ_ಎಣಿಕೆ[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    ಪ್ರತಿ (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        ಯದಿ (state->clear_mods & bit) {
            state->mod_ಕೀಲಿ_ಎಣಿಕೆ[i]--;
            ಯದಿ (state->mod_ಕೀಲಿ_ಎಣಿಕೆ[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_ಕೀಲಿ_ಎಣಿಕೆ[i] = 0;
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
    ಯದಿ (state->flags & STATE_REQUIRE_ಕೀಲಿ_EVENT) {
        darray_append(events->queue, (ಸಂರಚನಾ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_ಕೀಲಿ,
            .ಕೀಲಿ = {
                .keycode = ಕೀಲಿ->keycode,
                .direction = direction
            }
        });
    }

    ಯದಿ (remap_event >= 0) {
        // FIXME: fragile if last state change does not restore the state to the remap event
        undo_tweaks(&state->base, &initial_components, events);
    }

    ಧ್ರುವ ಗಣನಾ xkb_state_component changed = get_state_component_changes(
        &initial_components, &state->base.components
    );
    ಯದಿ (changed) {
        ಯದಿ (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        // TODO: merge with previous state event, if any

        darray_append(events->queue, (ಸಂರಚನಾ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = state->base.components,
                .changed = changed
            }
        });
    }

    ಯದಿ (!darray_empty(events->queue) &&
        darray_item(events->queue, darray_size(events->queue) - 1).type !=
        XKB_EVENT_TYPE_FRAME)
    {
        darray_append(events->queue, (ಸಂರಚನಾ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
}

ಸಂರಚನಾ xkb_events *
xkb_events_new(ಸಂರಚನಾ xkb_context * restrict context,
               ಧ್ರುವ ಸಂರಚನಾ xkb_events_config * restrict config,
               ಗಣನಾ xkb_status * restrict status)
{
    /* Handle default configuration */
    ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ xkb_events_config default_config = {
        .size = ಪರಿಮಾಣ(default_config)
    };
    ಯದಿ (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ಧ್ರುವ ಗಣನಾ xkb_status abi_error = xkb_check_state_abi(config);
    ಯದಿ (abi_error) {
        xkb_log_abi_error(context, __func__, abi_error);
        ಯದಿ (status)
            *status = abi_error;
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    /* Sanitize input */
    ಧ್ರುವ uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_EVENTS_FLAGS_ಮೌಲ್ಯಗಳು);
    ಯದಿ (invalid_flags) {
        log_err_func(context, XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS_,
                     "unrecognized events collection flags: 0x%"PRIx32"\n",
                     invalid_flags);
        ಯದಿ (status)
            *status = XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS;
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಸಂರಚನಾ xkb_events *events = calloc(1, ಪರಿಮಾಣ(*events));
    ಯದಿ (!events) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events collection\n");
        ಯದಿ (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    /* Pre-allocate event queue */
    darray_init(events->queue);
    ಸ್ಥಿರ ಧ್ರುವ darray_size_t queue_initial_size = 16;
    darray_resize(events->queue, queue_initial_size);
    // TODO: proper safe darray_resize() with return value
    ಯದಿ (!darray_ವಸ್ತುಗಳು(events->queue)) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events queue\n");
        xkb_events_destroy(events);
        ಯದಿ (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }
    darray_size(events->queue) = 0;

    events->ctx = xkb_context_ref(context);
    events->next = 0;

    ಯದಿ (status)
        *status = XKB_SUCCESS;
    ಪ್ರತ್ಯಾಗಮ events;
}

ಶೂನ್ಯ
xkb_events_destroy(ಸಂರಚನಾ xkb_events *events)
{
    ಯದಿ (!events)
        ಪ್ರತ್ಯಾಗಮ;
    darray_free(events->queue);
    xkb_context_unref(events->ctx);
    ಮುಕ್ತ(events);
}

ಧ್ರುವ ಸಂರಚನಾ xkb_event *
xkb_events_next(ಸಂರಚನಾ xkb_events *events)
{
    ಯದಿ (events->next < darray_size(events->queue)) {
        ಧ್ರುವ darray_size_t index = events->next++;
        ಪ್ರತ್ಯಾಗಮ &darray_item(events->queue, index);
    } ಅನ್ಯಥಾ {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }
}

ಗಣನಾ xkb_event_type
xkb_event_get_type(ಧ್ರುವ ಸಂರಚನಾ xkb_event *event)
{
    ಪ್ರತ್ಯಾಗಮ event->type;
}

ಗಣನಾ xkb_status
xkb_event_get_keycode(ಧ್ರುವ ಸಂರಚನಾ xkb_event *event,
                      xkb_keycode_t *keycode,
                      ಗಣನಾ xkb_ಕೀಲಿ_direction *direction)
{
    ಯದಿ (event->type == XKB_EVENT_TYPE_ಕೀಲಿ) {
        *keycode = event->ಕೀಲಿ.keycode;
        *direction = event->ಕೀಲಿ.direction;
        ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
    } ಅನ್ಯಥಾ {
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_EVENT_TYPE_MISMATCH;
    }
}

ಗಣನಾ xkb_status
xkb_event_get_components(ಧ್ರುವ ಸಂರಚನಾ xkb_event * restrict event,
                         ಸಂರಚನಾ xkb_event_components * restrict components)
{
    ಯದಿ (event->type != XKB_EVENT_TYPE_STATE_COMPONENTS)
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ಧ್ರುವ ಗಣನಾ xkb_status status = xkb_check_state_abi(components);
    ಯದಿ (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        ಪ್ರತ್ಯಾಗಮ status;
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

    ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
}

ಗಣನಾ xkb_status
xkb_event_get_pointer_motion(ಧ್ರುವ ಸಂರಚನಾ xkb_event * restrict event,
                             ಸಂರಚನಾ xkb_event_pointer_motion * restrict motion)
{
    ಯದಿ (event->type != XKB_EVENT_TYPE_POINTER_MOTION)
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ಧ್ರುವ ಗಣನಾ xkb_status status = xkb_check_state_abi(motion);
    ಯದಿ (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        ಪ್ರತ್ಯಾಗಮ status;
    }

    motion->flags = event->pointer_motion.flags;
    motion->x = event->pointer_motion.x;
    motion->y = event->pointer_motion.y;

    ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
}

ಗಣನಾ xkb_status
xkb_event_get_pointer_button(ಧ್ರುವ ಸಂರಚನಾ xkb_event * restrict event,
                             ಸಂರಚನಾ xkb_event_pointer_button * restrict button)
{
    ಯದಿ (event->type != XKB_EVENT_TYPE_POINTER_BUTTON)
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ಧ್ರುವ ಗಣನಾ xkb_status status = xkb_check_state_abi(button);
    ಯದಿ (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        ಪ್ರತ್ಯಾಗಮ status;
    }

    button->button = event->pointer_button.button;
    button->state = event->pointer_button.state;
    button->ಎಣಿಕೆ = event->pointer_button.ಎಣಿಕೆ;

    ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
}

ಗಣನಾ xkb_status
xkb_event_get_virtual_console(ಧ್ರುವ ಸಂರಚನಾ xkb_event * restrict event,
                              int8_t * restrict index_or_offset,
                              bool * restrict is_offset)
{
    ಯದಿ (event->type != XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_EVENT_TYPE_MISMATCH;

    *index_or_offset = event->virtual_console.index_or_offset;
    *is_offset = event->virtual_console.is_offset;

    ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
}

ಗಣನಾ xkb_status
xkb_state_update_event(ಸಂರಚನಾ xkb_state * restrict base_state,
                       ಧ್ರುವ ಸಂರಚನಾ xkb_event * restrict event,
                       ಗಣನಾ xkb_state_component * restrict changed)
{
    /* Guard against server-only state */
    ಪ್ರತಿಜ್ಞಾ(base_state->mode == SERVER_COMPANION ||
           base_state->mode == LEGACY_MIXED_STATE);
    ಯದಿ (base_state->mode != SERVER_COMPANION &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE_,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    ಸಂರಚನಾ xkb_client_state * ಧ್ರುವ state =
        (ಸಂರಚನಾ xkb_client_state *)base_state;

    ಯದಿ (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
        ಧ್ರುವ ಸಂರಚನಾ state_components prev_components = state->base.components;
        state->base.components = event->components.components;
        /*
         * Recompute the changes instead of using the event value, because we do
         * not know if the event’s queue and the state are synced.
         */
        ಯದಿ (changed)
            *changed = get_state_component_changes(&prev_components,
                                                   &state->base.components);
    } ಅನ್ಯಥಾ {
        ಯದಿ (changed)
            *changed = 0;
    }
    ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
}

ಸ್ಥಿರ ಶೂನ್ಯ
append_pointer_button(ಸಂರಚನಾ xkb_events * restrict events,
                      ಸಂರಚನಾ xkb_filter * restrict filter,
                      ಗಣನಾ xkb_pointer_button_state button_state,
                      uint8_t ಎಣಿಕೆ)
{
    darray_append(events->queue, (ಸಂರಚನಾ xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_BUTTON,
        .pointer_button = {
            .size = ಪರಿಮಾಣ(((ಸಂರಚನಾ xkb_event*)0)->pointer_button),
            .button = filter->priv,
            .state = button_state,
            .ಎಣಿಕೆ = ಎಣಿಕೆ,
        }
    });
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_pointer_button_new(ಸಂರಚನಾ xkb_server_state *state,
                              ಸಂರಚನಾ xkb_events *events,
                              ಸಂರಚನಾ xkb_filter *filter)
{
    ಯದಿ (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ಕೀಲಿಗಳು)) {
        filter->func = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಸಂರಚನಾ xkb_machine * ಧ್ರುವ sm = (ಸಂರಚನಾ xkb_machine *)state;

    /* Resolve button */
    ಧ್ರುವ xkb_pointer_button_index_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;
    filter->priv = button;

    ಧ್ರುವ ಗಣನಾ xkb_pointer_button_state button_state =
        XKB_POINTER_BUTTON_PRESSED;
    append_pointer_button(events, filter, button_state,
                          filter->action.btn.ಎಣಿಕೆ);
    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;
}

ಸ್ಥಿರ bool
xkb_filter_pointer_button_func(ಸಂರಚನಾ xkb_server_state *state,
                               ಸಂರಚನಾ xkb_events *events,
                               ಸಂರಚನಾ xkb_filter *filter,
                               ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                               ಗಣನಾ xkb_ಕೀಲಿ_direction direction)
{
    ಯದಿ (ಕೀಲಿ != filter->ಕೀಲಿ || !events)
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;

    ವಿಕಲ್ಪ (direction) {
    ಪಕ್ಷ XKB_ಕೀಲಿ_DOWN:
        filter->refcnt++;
        /* fallthrough */
    ಪಕ್ಷ XKB_ಕೀಲಿ_REPEATED:
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    ಸ್ವಭಾವ:
        ಯದಿ (--filter->refcnt > 0)
            ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    }

    ಯದಿ (!filter->action.btn.ಎಣಿಕೆ)
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);

    filter->func = ರಿಕ್ತ;
    ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
}

ಸಂರಚನಾ xkb_filter_pointer_lock_button_priv {
    uint32_t button:31;
#ifdef _WIN32
    uint32_t locked:1; // MSVC requires matching types to pack bitfields
#ಅನ್ಯಥಾ
    bool locked:1;
#endif
};

ಗಣನಾ {
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH =
        ಪರಿಮಾಣ(ಸಂರಚನಾ xkb_filter_pointer_lock_button_priv) * CHAR_BIT,
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX =
        (UINT64_C(1) << XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH) - 1
};

static_assert(ಪರಿಮಾಣ(ಸಂರಚನಾ xkb_filter_pointer_lock_button_priv) ==
              ಪರಿಮಾಣ(((ಸಂರಚನಾ xkb_filter*)0)->priv),
              "");
static_assert((uint32_t)XKB_POINTER_BUTTON_MAX < (uint32_t)XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX,
              "xkb_filter_pointer_lock_button_priv cannot store button");

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_pointer_lock_button_new(ಸಂರಚನಾ xkb_server_state *state,
                                   ಸಂರಚನಾ xkb_events *events,
                                   ಸಂರಚನಾ xkb_filter *filter)
{
    ಯದಿ (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ಕೀಲಿಗಳು)) {
        filter->func = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ;
    }

    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;

    ಸಂರಚನಾ xkb_machine * ಧ್ರುವ sm = (ಸಂರಚನಾ xkb_machine *)state;

    /* Resolve button */
    ಧ್ರುವ xkb_pointer_button_index_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;

    static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                  "invalid UINTx_C() macro");
    static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                  "invalid left shift");
    ಧ್ರುವ xkb_pointer_button_mask_t mask = (UINT8_C(1) << button);

    ಧ್ರುವ bool locked = (sm->mouse.locked_buttons & mask);

    ಯದಿ (!locked && !(filter->action.btn.flags & ACTION_LOCK_NO_LOCK)) {
        sm->mouse.locked_buttons |= mask;
        filter->priv = button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_PRESSED, 0);
    }

    ಸಂರಚನಾ xkb_filter_pointer_lock_button_priv *priv =
        (ಸಂರಚನಾ xkb_filter_pointer_lock_button_priv*)&filter->priv;
    priv->button = button,
    priv->locked = locked;
}

ಸ್ಥಿರ bool
xkb_filter_pointer_lock_button_func(ಸಂರಚನಾ xkb_server_state *state,
                                    ಸಂರಚನಾ xkb_events *events,
                                    ಸಂರಚನಾ xkb_filter *filter,
                                    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                                    ಗಣನಾ xkb_ಕೀಲಿ_direction direction)
{
    ಯದಿ (ಕೀಲಿ != filter->ಕೀಲಿ || !events || state->base.mode != SERVER_STATE)
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;

    ವಿಕಲ್ಪ (direction) {
    ಪಕ್ಷ XKB_ಕೀಲಿ_DOWN:
        filter->refcnt++;
        /* fallthrough */
    ಪಕ್ಷ XKB_ಕೀಲಿ_REPEATED:
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    ಸ್ವಭಾವ:
        ಯದಿ (--filter->refcnt > 0)
            ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    }

    ಧ್ರುವ ಸಂರಚನಾ xkb_filter_pointer_lock_button_priv *priv =
        (ಸಂರಚನಾ xkb_filter_pointer_lock_button_priv*)&filter->priv;

    ಯದಿ (priv->locked && !(filter->action.btn.flags & ACTION_LOCK_NO_UNLOCK)) {
        ಸಂರಚನಾ xkb_machine * ಧ್ರುವ sm = (ಸಂರಚನಾ xkb_machine *)state;

        static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                    "invalid UINTx_C() macro");
        static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                    "invalid left shift");
        ಧ್ರುವ xkb_pointer_button_mask_t mask = (UINT8_C(1) << priv->button);

        sm->mouse.locked_buttons &= ~mask;
        filter->priv = priv->button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);
    }

    filter->func = ರಿಕ್ತ;
    ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_pointer_set_default_button_new(ಸಂರಚನಾ xkb_server_state *state,
                                          ಸಂರಚನಾ xkb_events *events,
                                          ಸಂರಚನಾ xkb_filter *filter)
{
    ಯದಿ (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ಕೀಲಿಗಳು)) {
        filter->func = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ;
    }

    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;

    ಸಂರಚನಾ xkb_machine * ಧ್ರುವ sm = (ಸಂರಚನಾ xkb_machine *)state;

    static_assert(2 * XKB_POINTER_BUTTON_MAX <= INT_MAX, "lossy conversion");
    ಪೂರ್ಣಾಙ್ಕ button = (filter->action.dflt.flags & ACTION_ABSOLUTE_SWITCH)
        ? filter->action.dflt.ಮೌಲ್ಯ
        : sm->mouse.default_button + filter->action.dflt.ಮೌಲ್ಯ;

    /* Wrap into range */
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ rem = button % XKB_POINTER_BUTTON_MAX;
    sm->mouse.default_button = (rem >= 0)
        ? (xkb_pointer_button_index_t)rem
        : (xkb_pointer_button_index_t)(rem + XKB_POINTER_BUTTON_MAX);
}

ಸ್ಥಿರ bool
xkb_filter_pointer_set_default_button_func(ಸಂರಚನಾ xkb_server_state *state,
                                           ಸಂರಚನಾ xkb_events *events,
                                           ಸಂರಚನಾ xkb_filter *filter,
                                           ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                                           ಗಣನಾ xkb_ಕೀಲಿ_direction direction)
{
    ಯದಿ (ಕೀಲಿ != filter->ಕೀಲಿ || !events)
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;

    ವಿಕಲ್ಪ (direction) {
    ಪಕ್ಷ XKB_ಕೀಲಿ_DOWN:
        filter->refcnt++;
        /* fallthrough */
    ಪಕ್ಷ XKB_ಕೀಲಿ_REPEATED:
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    ಸ್ವಭಾವ:
        ಯದಿ (--filter->refcnt > 0)
            ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    }

    filter->func = ರಿಕ್ತ;
    ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_terminate_server_new(ಸಂರಚನಾ xkb_server_state *state,
                                 ಸಂರಚನಾ xkb_events *events,
                                 ಸಂರಚನಾ xkb_filter *filter)
{
    ಯದಿ (!events || state->base.mode != SERVER_STATE ||
        !(((ಸಂರಚನಾ xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ;
    }

    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;

    darray_append(events->queue, (ಸಂರಚನಾ xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    });
}

ಸ್ಥಿರ bool
xkb_filter_terminate_server_func(ಸಂರಚನಾ xkb_server_state *state,
                                 ಸಂರಚನಾ xkb_events *events,
                                 ಸಂರಚನಾ xkb_filter *filter,
                                 ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                                 ಗಣನಾ xkb_ಕೀಲಿ_direction direction)
{
    ಯದಿ (ಕೀಲಿ != filter->ಕೀಲಿ || !events)
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;

    ವಿಕಲ್ಪ (direction) {
    ಪಕ್ಷ XKB_ಕೀಲಿ_DOWN:
        filter->refcnt++;
        /* fallthrough */
    ಪಕ್ಷ XKB_ಕೀಲಿ_REPEATED:
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    ಸ್ವಭಾವ:
        ಯದಿ (--filter->refcnt > 0)
            ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    }

    filter->func = ರಿಕ್ತ;
    ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_switch_virtual_console_new(ಸಂರಚನಾ xkb_server_state *state,
                                      ಸಂರಚನಾ xkb_events *events,
                                      ಸಂರಚನಾ xkb_filter *filter)
{
    ಯದಿ (!events || state->base.mode != SERVER_STATE ||
        !(((ಸಂರಚನಾ xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ;
    }

    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;

    darray_append(events->queue, (ಸಂರಚನಾ xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
        .virtual_console = {
            .index_or_offset = filter->action.screen.screen,
            .is_offset = !(filter->action.screen.flags & ACTION_ABSOLUTE_SWITCH)
        }
    });
}

ಸ್ಥಿರ bool
xkb_filter_switch_virtual_console_func(ಸಂರಚನಾ xkb_server_state *state,
                                       ಸಂರಚನಾ xkb_events *events,
                                       ಸಂರಚನಾ xkb_filter *filter,
                                       ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ,
                                       ಗಣನಾ xkb_ಕೀಲಿ_direction direction)
{
    ಯದಿ (ಕೀಲಿ != filter->ಕೀಲಿ || !events)
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;

    ವಿಕಲ್ಪ (direction) {
    ಪಕ್ಷ XKB_ಕೀಲಿ_DOWN:
        filter->refcnt++;
        /* fallthrough */
    ಪಕ್ಷ XKB_ಕೀಲಿ_REPEATED:
        ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    ಸ್ವಭಾವ:
        ಯದಿ (--filter->refcnt > 0)
            ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
    }

    filter->func = ರಿಕ್ತ;
    ಪ್ರತ್ಯಾಗಮ XKB_FILTER_CONSUME;
}
