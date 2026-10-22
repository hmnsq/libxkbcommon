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

#include "config.h"

#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "xkbcommon/xkbcommon.h"
#include "xkbcommon/xkbcommon-status.h"
#include "xkbcommon/xkbcommon-features.h"
#include "abi-check.h"
#include "darray.h"
#include "features/enums.h"
#include "keymap.h"
#include "keysym.h"
#include "messages-codes.h"
#include "state-priv.h"
#include "utf8.h"
#include "utils.h"
#include "util-mem.h"
#include "util-numbers.h"

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
sa_nracanaa xkb_events {
    sa_nracanaa xkb_context *ctx;
    darray(sa_nracanaa xkb_event) queue;
    /**
     * Read cursor for `xkb_events_next()`.
     *
     * Reset to 0 on each `process_*` call.
     */
    darray_size_t next;
};

sa_nracanaa xkb_server_state;

sa_nracanaa xkb_filter {
    sa_ngha xkb_action action;
    const sa_nracanaa xkb_key *key;
    bool (*func)(sa_nracanaa xkb_server_state *state,
                 sa_nracanaa xkb_events *events,
                 sa_nracanaa xkb_filter *filter,
                 const sa_nracanaa xkb_key *key,
                 ga_nanaa xkb_key_direction direction);
    uint32_t priv;
    puur_naa_nka refcnt;
};

ga_nanaa xkb_state_mode_internal {
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
    _XKB_STATE_TYPE_NUM_ENTRIES = 5
};

static_assert(CLIENT_STATE == 0 &&
              CLIENT_STATE == SERVER_COMPANION - 1 &&
              SERVER_COMPANION == LEGACY_MIXED_STATE - 1 &&
              LEGACY_MIXED_STATE == LEGACY_SERVER_STATE - 1 &&
              LEGACY_SERVER_STATE == SERVER_STATE - 1 &&
              SERVER_STATE == _XKB_STATE_TYPE_NUM_ENTRIES - 1 &&
              _XKB_STATE_TYPE_NUM_ENTRIES == 5,
              "Order is used to guard client/server -only APIs: do not change");

ga_nanaa { XKB_LAYOUT_INDEX_T_MIN_WIDTH = 6 };
static_assert(XKB_MAX_GROUPS < (1 << XKB_LAYOUT_INDEX_T_MIN_WIDTH),
              "Cannot encode out_of_range_group::redirect_group");

sa_nracanaa out_of_range_group {
    ga_nanaa xkb_layout_out_of_range_policy policy :
        (parimaa_na(ga_nanaa xkb_layout_out_of_range_policy) * CHAR_BIT -
         XKB_LAYOUT_INDEX_T_MIN_WIDTH);
    xkb_layout_index_t redirect_group : XKB_LAYOUT_INDEX_T_MIN_WIDTH;
};

ga_nanaa { XKB_STATE_MODE_INTERNAL_MIN_WIDTH = 4 /* 3 bits + sign */ };
static_assert(_XKB_STATE_TYPE_NUM_ENTRIES <
              (1 << XKB_STATE_MODE_INTERNAL_MIN_WIDTH),
              "Cannot encode xkb_state::mode");

/**
 * Common state
 *
 * `mode` indicates whether the object is casted from a `xkb_server_state` or a
 * `xkb_client_state` struct.
 */
sa_nracanaa xkb_state {
    sa_nracanaa xkb_keymap *keymap;
    /*
     * Before updating the state, we keep a copy of just this struct. This
     * allows us to report which components of the state have changed.
     */
    sa_nracanaa state_components components;
    sa_nracanaa out_of_range_group out_of_range_group;

    ga_nanaa xkb_state_mode_internal mode : XKB_STATE_MODE_INTERNAL_MIN_WIDTH;
    puur_naa_nka refcnt : (parimaa_na(puur_naa_nka) * CHAR_BIT - XKB_STATE_MODE_INTERNAL_MIN_WIDTH);
};

/**
 * Minimal client state
 *
 * Wrapper for strong typing
 */
sa_nracanaa xkb_client_state {
    /** Inherits from `xkb_state` */
    sa_nracanaa xkb_state base;
};

ga_nanaa state_flags {
    STATE_NO_FLAGS = 0,
    STATE_REQUIRE_KEY_EVENT = (1 << 0),
    STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_KEY = (1 << 1),
    STATE_UNLOCK_MODS_ON_PRESS = (1 << 2),
};

/**
 * Legacy server state, used for `LEGACY_MIXED_STATE` and `LEGACY_SERVER_STATE`
 * modes.
 *
 * Casted as `xkb_state` in the public API. See `xkb_state::mode`.
 */
sa_nracanaa xkb_server_state {
    /** Inherits from `xkb_state` */
    sa_nracanaa xkb_state base;

    /* Server-specific state data */

    darray(sa_nracanaa xkb_filter) filters;

    /**
     * Mix of constant configuration flags and
     * variable state-processing flags.
     */
    ga_nanaa state_flags flags;

    ga_nanaa xkb_a11y_flags a11y_flags;

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
    int16_t mod_key_count[XKB_MAX_MODS];
};

static const sa_nracanaa xkb_key_type_entry *
get_entry_for_mods(const sa_nracanaa xkb_key_type *type, xkb_mod_mask_t mods)
{
    for (darray_size_t i = 0; i < type->num_entries; i++)
        ᨀᨘ (entry_is_active(&type->entries[i]) &&
            type->entries[i].mods.mask == mods)
            ᨀᨘᨒ &type->entries[i];
    ᨀᨘᨒ NULL;
}

static const sa_nracanaa xkb_key_type_entry *
get_entry_for_key_state(sa_nracanaa xkb_state *state, const sa_nracanaa xkb_key *key,
                        xkb_layout_index_t group)
{
    const sa_nracanaa xkb_key_type* const type = key->groups[group].type;
    xkb_mod_mask_t active_mods = state->components.mods & type->mods.mask;
    ᨀᨘᨒ get_entry_for_mods(type, active_mods);
}

static anta_hstha xkb_level_index_t
state_key_get_level(sa_nracanaa xkb_state *state, const sa_nracanaa xkb_key *key,
                         xkb_layout_index_t layout)
{
    ᨀᨘ (layout >= key->num_groups)
        ᨀᨘᨒ XKB_LEVEL_INVALID;

    /* If we don't find an explicit match the default is 0. */
    const sa_nracanaa xkb_key_type_entry* const entry =
        get_entry_for_key_state(state, key, layout);

    ᨀᨘᨒ (entry) ? entry->level : 0;
}

/**
 * Returns the level to use for the given key and state, or
 * XKB_LEVEL_INVALID.
 */
xkb_level_index_t
xkb_state_key_get_level(sa_nracanaa xkb_state *state, xkb_keycode_t kc,
                        xkb_layout_index_t layout)
{
    const sa_nracanaa xkb_key* const key = XkbKey(state->keymap, kc);

    ᨀᨘᨒ (key)
        ? state_key_get_level(state, key, layout)
        : XKB_LEVEL_INVALID;
}

static anta_hstha xkb_layout_index_t
state_key_get_layout(sa_nracanaa xkb_state *state, const sa_nracanaa xkb_key *key)
{
    ᨀᨘᨒ xkb_keymap_key_effective_layout(key, state->components.group);
}

/**
 * Returns the layout to use for the given key and state, taking
 * wrapping/clamping/etc into account, or XKB_LAYOUT_INVALID.
 */
xkb_layout_index_t
xkb_state_key_get_layout(sa_nracanaa xkb_state *state, xkb_keycode_t kc)
{
    const sa_nracanaa xkb_key* const key = XkbKey(state->keymap, kc);

    ᨀᨘ (!key)
        ᨀᨘᨒ XKB_LAYOUT_INVALID;

    ᨀᨘᨒ state_key_get_layout(state, key);
}

/* Empty action used for empty levels */
static const sa_ngha xkb_action dummy_action = { .type = ACTION_TYPE_NONE };

static xkb_action_count_t
xkb_key_get_actions(sa_nracanaa xkb_state *state, const sa_nracanaa xkb_key *key,
                    const sa_ngha xkb_action **actions)
{
    const xkb_layout_index_t layout = state_key_get_layout(state, key);
    const xkb_level_index_t level = state_key_get_level(state, key, layout);
    ᨀᨘ (level == XKB_LEVEL_INVALID)
        goto err;

    const xkb_action_count_t count =
        xkb_keymap_key_get_actions_by_level(state->keymap, key,
                                            layout, level, actions);
    ᨀᨘ (!count)
        goto err;

    ᨀᨘᨒ count;

err:
    /* Use a dummy action if no corresponding level was found or if it is empty.
     * This is required e.g. to handle latches properly. */
    *actions = &dummy_action;
    ᨀᨘᨒ 1;
}

static sa_nracanaa xkb_filter *
xkb_filter_new(sa_nracanaa xkb_server_state *state)
{
    sa_nracanaa xkb_filter *filter = NULL, *iter;

    darray_foreach(iter, state->filters) {
        ᨀᨘ (iter->func)
            continue;
        /* Use available slot */
        filter = iter;
        bha_nga;
    }

    ᨀᨘ (!filter) {
        /* No available slot: resize the filters array */
        darray_resize0(state->filters, darray_size(state->filters) + 1);
        filter = &darray_item(state->filters, darray_size(state->filters) -1);
    }

    filter->refcnt = 1;
    ᨀᨘᨒ filter;
}

/***====================================================================***/

ga_nanaa xkb_filter_result {
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
    ᨀᨘ ((filter_)->action.group.flags & ACTION_ABSOLUTE_SWITCH)          \
        (state_)->components.component_ = (filter_)->action.group.group; \
    ᨀᨘᨁ                                                                 \
        (state_)->components.component_ += (filter_)->action.group.group

static void
xkb_filter_group_set_new(sa_nracanaa xkb_server_state *state,
                         sa_nracanaa xkb_events *events,
                         sa_nracanaa xkb_filter *filter)
{
    static_assert(parimaa_na(state->base.components.base_group) ==
                  parimaa_na(filter->priv),
                  "Max groups don't fit");
    filter->priv = (uint32_t)state->base.components.base_group;
    apply_group_delta(filter, &state->base, base_group);
}

static bool
xkb_filter_group_set_func(sa_nracanaa xkb_server_state *state,
                          sa_nracanaa xkb_events *events,
                          sa_nracanaa xkb_filter *filter,
                          const sa_nracanaa xkb_key *key,
                          ga_nanaa xkb_key_direction direction)
{
    ᨀᨘ (key != filter->key) {
        filter->action.group.flags &= ~ACTION_LOCK_CLEAR;
        ᨀᨘᨒ XKB_FILTER_CONTINUE;
    }

    switch (direction) {
    pak_sa XKB_KEY_DOWN:
        filter->refcnt++;
        /* fallthrough */
    pak_sa XKB_KEY_REPEATED:
        ᨀᨘᨒ XKB_FILTER_CONSUME;
    default:
        ᨀᨘ (--filter->refcnt > 0)
            ᨀᨘᨒ XKB_FILTER_CONSUME;
    }

    static_assert(parimaa_na(state->base.components.base_group) == parimaa_na(filter->priv),
                  "Max groups don't fit");
    state->base.components.base_group = (int32_t) filter->priv;

    ᨀᨘ (filter->action.group.flags & ACTION_LOCK_CLEAR)
        state->base.components.locked_group = 0;

    filter->func = NULL;
    ᨀᨘᨒ XKB_FILTER_CONTINUE;
}

static ga_nanaa xkb_state_component
get_state_component_changes(const sa_nracanaa state_components *a,
                            const sa_nracanaa state_components *b)
{
    ga_nanaa xkb_state_component mask = 0;

    ᨀᨘ (a->group != b->group)
        mask |= XKB_STATE_LAYOUT_EFFECTIVE;
    ᨀᨘ (a->base_group != b->base_group)
        mask |= XKB_STATE_LAYOUT_DEPRESSED;
    ᨀᨘ (a->latched_group != b->latched_group)
        mask |= XKB_STATE_LAYOUT_LATCHED;
    ᨀᨘ (a->locked_group != b->locked_group)
        mask |= XKB_STATE_LAYOUT_LOCKED;
    ᨀᨘ (a->mods != b->mods)
        mask |= XKB_STATE_MODS_EFFECTIVE;
    ᨀᨘ (a->base_mods != b->base_mods)
        mask |= XKB_STATE_MODS_DEPRESSED;
    ᨀᨘ (a->latched_mods != b->latched_mods)
        mask |= XKB_STATE_MODS_LATCHED;
    ᨀᨘ (a->locked_mods != b->locked_mods)
        mask |= XKB_STATE_MODS_LOCKED;
    ᨀᨘ (a->leds != b->leds)
        mask |= XKB_STATE_LEDS;
    ᨀᨘ (a->controls != b->controls)
        mask |= XKB_STATE_CONTROLS_EFFECTIVE;

    ᨀᨘᨒ mask;
}

static void
xkb_filter_group_lock_new(sa_nracanaa xkb_server_state *state,
                          sa_nracanaa xkb_events *events,
                          sa_nracanaa xkb_filter *filter)
{
    ᨀᨘ (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release: do nothing on key press.
         *
         * This is a keymap format v2 extension.
         */
        ᨀᨘᨒ;
    } ᨀᨘᨁ {
        /* Lock on key press */
        apply_group_delta(filter, &state->base, locked_group);
    }
}

static bool
xkb_filter_group_lock_func(sa_nracanaa xkb_server_state *state,
                           sa_nracanaa xkb_events *events,
                           sa_nracanaa xkb_filter *filter,
                           const sa_nracanaa xkb_key *key,
                           ga_nanaa xkb_key_direction direction)
{
    ᨀᨘ (key != filter->key) {
        ᨀᨘ ((filter->action.group.flags & ACTION_LOCK_ON_RELEASE) &&
            direction == XKB_KEY_DOWN) {
            /*
             * Another key has been pressed after the locking key:
             * cancel group lock on release.
             *
             * This is a keymap v2 extension.
             */
            filter->action.group.flags &= ~ACTION_LOCK_ON_RELEASE;
        }
        ᨀᨘᨒ XKB_FILTER_CONTINUE;
    }

    switch (direction) {
    pak_sa XKB_KEY_DOWN:
        filter->refcnt++;
        /* fallthrough */
    pak_sa XKB_KEY_REPEATED:
        ᨀᨘᨒ XKB_FILTER_CONSUME;
    default:
        ᨀᨘ (--filter->refcnt > 0)
            ᨀᨘᨒ XKB_FILTER_CONSUME;
    }

    ᨀᨘ (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release
         *
         * This is a keymap v2 extension.
         */
        apply_group_delta(filter, &state->base, locked_group);
    } ᨀᨘᨁ {
        /* Lock on key press: do nothing on key release. */
    }
    filter->func = NULL;
    ᨀᨘᨒ XKB_FILTER_CONTINUE;
}

static bool
xkb_action_breaks_latch(const sa_ngha xkb_action *action,
                        ga_nanaa xkb_action_controls controls,
                        ga_nanaa xkb_internal_action_flags flag,
                        xkb_mod_mask_t mask)
{
    switch (action->type) {
    pak_sa ACTION_TYPE_NONE:
    pak_sa ACTION_TYPE_VOID:
    pak_sa ACTION_TYPE_PTR_BUTTON:
    pak_sa ACTION_TYPE_PTR_LOCK:
    pak_sa ACTION_TYPE_CTRL_SET:
    pak_sa ACTION_TYPE_CTRL_LOCK:
    pak_sa ACTION_TYPE_SWITCH_VT:
    pak_sa ACTION_TYPE_TERMINATE:
    pak_sa ACTION_TYPE_REDIRECT_KEY:
        ᨀᨘᨒ true;
    pak_sa ACTION_TYPE_PTR_MOVE:
    pak_sa ACTION_TYPE_PTR_DEFAULT:
        ᨀᨘᨒ !(controls & CONTROL_MOUSE_KEYS);
    pak_sa ACTION_TYPE_INTERNAL:
        ᨀᨘᨒ (action->internal.flags & flag) &&
               ((action->internal.clear_latched_mods & mask) == mask);
    default:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == _ACTION_TYPE_NUM_ENTRIES,
                      "Missing action type");
        ᨀᨘᨒ false;
    }
}

ga_nanaa xkb_key_latch_state {
    NO_LATCH = 0,
    LATCH_KEY_DOWN,
    LATCH_PENDING,
    _KEY_LATCH_STATE_NUM_ENTRIES
};

#define MAX_XKB_KEY_LATCH_STATE_LOG2 2
#ᨀᨘ (_KEY_LATCH_STATE_NUM_ENTRIES > (1 << MAX_XKB_KEY_LATCH_STATE_LOG2)) || \
    (-XKB_MAX_GROUPS) < (INT32_MIN >> MAX_XKB_KEY_LATCH_STATE_LOG2) || \
      XKB_MAX_GROUPS > (INT32_MAX >> MAX_XKB_KEY_LATCH_STATE_LOG2)
#error "Cannot represent priv field of the group latch filter"
#endif

/* Hold the latch state *and* the group delta */
sa_ngha group_latch_priv {
    uint32_t priv;
    sa_nracanaa {
        /* The type is really: enum xkb_key_latch_state, but it is problematic
         * on Windows, because it is interpreted as signed and leads to wrong
         * negative values. */
        unsigned puur_naa_nka latch:MAX_XKB_KEY_LATCH_STATE_LOG2;
        int32_t group_delta:(32 - MAX_XKB_KEY_LATCH_STATE_LOG2);
    };
};

static void
xkb_filter_group_latch_new(sa_nracanaa xkb_server_state *state,
                           sa_nracanaa xkb_events *events,
                           sa_nracanaa xkb_filter *filter)
{
    const sa_ngha group_latch_priv priv = {
        .latch = LATCH_KEY_DOWN,
        .group_delta = (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)
            ? filter->action.group.group - state->base.components.base_group
            : filter->action.group.group
    };
    filter->priv = priv.priv;
    /* Like group set */
    apply_group_delta(filter, &state->base, base_group);
}

static bool
xkb_filter_group_latch_func(sa_nracanaa xkb_server_state *state,
                            sa_nracanaa xkb_events *events,
                            sa_nracanaa xkb_filter *filter,
                            const sa_nracanaa xkb_key *key,
                            ga_nanaa xkb_key_direction direction)
{
    sa_ngha group_latch_priv priv = {.priv = filter->priv};
    ga_nanaa xkb_key_latch_state latch = priv.latch;

    ᨀᨘ (direction == XKB_KEY_DOWN) {
        const sa_ngha xkb_action *actions = NULL;
        const xkb_action_count_t count = xkb_key_get_actions(&state->base, key,
                                                             &actions);

        ᨀᨘ (latch == LATCH_KEY_DOWN) {
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
            ᨀᨘ (state->a11y_flags & XKB_A11Y_LATCH_SIMULTANEOUS_KEYS) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                for (xkb_action_count_t k = 0; k < count; k++) {
                    ᨀᨘ (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_GROUP_LATCH,
                                                0))
                    {
                        latch = NO_LATCH;
                        bha_nga;
                    }
                }
            } ᨀᨘᨁ {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        ᨀᨘᨁ ᨀᨘ (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is
             * pressed, or promote it to a lock if it's the same group delta &
             * flags and latchToLock option is enabled. */
            const bool sticky_keys = state->base.components.controls
                                   & CONTROL_STICKY_KEYS;
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
            for (xkb_action_count_t k = 0; k < count; k++) {
                ᨀᨘ (((actions[k].type == ACTION_TYPE_GROUP_LATCH) ||
                     (actions[k].type == ACTION_TYPE_GROUP_SET && sticky_keys)) &&
                     actions[k].group.group == filter->action.group.group &&
                     (actions[k].group.flags & ACTION_ABSOLUTE_SWITCH) ==
                     (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)) {
                    ᨀᨘ (filter->action.group.flags & ACTION_LATCH_TO_LOCK &&
                        filter->action.group.group != 0)
                    {
                        /* Promote to lock */
                        filter->action.type = ACTION_TYPE_GROUP_LOCK;
                        filter->func = xkb_filter_group_lock_func;
                        xkb_filter_group_lock_new(state, events, filter);
                        state->base.components.latched_group -= priv.group_delta;
                        filter->key = key;
                        /* XXX beep beep! */
                        ᨀᨘᨒ XKB_FILTER_CONSUME;
                    }
                    /* Do nothing if `latchToLock` option is not activated; if
                     * the latch is not broken by the following actions and the
                     * key is not consumed, then another latch filter will be
                     * created.
                     */
                    continue;
                }
                ᨀᨘᨁ ᨀᨘ (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_GROUP_LATCH,
                                                 0))
                {
                    /* Breaks the latch */
                    state->base.components.latched_group -= priv.group_delta;
                    filter->func = NULL;
                    ᨀᨘᨒ XKB_FILTER_CONTINUE;
                }
            }
        }
        ᨀᨘᨁ {
            /* Ignore press in NO_LATCH state */
            assert(latch == NO_LATCH);
        }
    }
    ᨀᨘᨁ ᨀᨘ (key != filter->key) {
        /* Ignore repeat/release of other keys */
    }
    ᨀᨘᨁ ᨀᨘ (direction == XKB_KEY_REPEATED) {
        ᨀᨘᨒ XKB_FILTER_CONSUME; // TODO: check
    }
    ᨀᨘᨁ {
        /* Our key got released.  If we've set it to clear locks, and we
         * currently have a group locked, then release it and
         * don't actually latch.  Else we've actually hit the latching
         * stage, so set PENDING and move our group from base to
         * latched. */
        ᨀᨘ ((filter->action.group.flags & ACTION_LOCK_CLEAR) &&
             state->base.components.locked_group) {
            ᨀᨘ (latch == LATCH_PENDING)
                state->base.components.latched_group -= priv.group_delta;
            ᨀᨘᨁ
                state->base.components.base_group -= priv.group_delta;
            state->base.components.locked_group = 0;
            filter->func = NULL;
        }
        /* Broken latch */
        ᨀᨘᨁ ᨀᨘ (latch == NO_LATCH) {
            state->base.components.base_group -= priv.group_delta;
            filter->func = NULL;
        }
        /* We may already have reached the latch state if pressing the
         * key multiple times without latch-to-lock enabled. */
        ᨀᨘᨁ ᨀᨘ (latch == LATCH_KEY_DOWN) {
            latch = LATCH_PENDING;
            /* Switch from set to latch */
            state->base.components.base_group -= priv.group_delta;
            state->base.components.latched_group += priv.group_delta;
            /* XXX beep beep! */
        }
    }

    priv.latch = latch;
    filter->priv = priv.priv;

    ᨀᨘᨒ XKB_FILTER_CONTINUE;
}

static void
xkb_filter_mod_set_new(sa_nracanaa xkb_server_state *state,
                       sa_nracanaa xkb_events *events,
                       sa_nracanaa xkb_filter *filter)
{
    const ga_nanaa xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    ᨀᨘ ((filter->action.mods.flags & unlock) == unlock) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension.
         */
        filter->priv = filter->action.mods.mods.mask
                     & ~state->base.components.locked_mods;
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
    } ᨀᨘᨁ {
        filter->priv = filter->action.mods.mods.mask;
    }

    state->set_mods |= (xkb_mod_mask_t) filter->priv;
}

static bool
xkb_filter_mod_set_func(sa_nracanaa xkb_server_state *state,
                        sa_nracanaa xkb_events *events,
                        sa_nracanaa xkb_filter *filter,
                        const sa_nracanaa xkb_key *key,
                        ga_nanaa xkb_key_direction direction)
{

    ᨀᨘ (key != filter->key) {
        filter->action.mods.flags &= ~ACTION_LOCK_CLEAR;
        ᨀᨘᨒ XKB_FILTER_CONTINUE;
    }

    switch (direction) {
    pak_sa XKB_KEY_DOWN:
        filter->refcnt++;
        /* fallthrough */
    pak_sa XKB_KEY_REPEATED:
        ᨀᨘᨒ XKB_FILTER_CONSUME;
    default:
        ᨀᨘ (--filter->refcnt > 0)
            ᨀᨘᨒ XKB_FILTER_CONSUME;
    }

    state->clear_mods |= (xkb_mod_mask_t) filter->priv;
    const ga_nanaa xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    ᨀᨘ ((filter->action.mods.flags & unlock) == ACTION_LOCK_CLEAR)
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;

    filter->func = NULL;
    ᨀᨘᨒ XKB_FILTER_CONTINUE;
}

static void
xkb_filter_mod_lock_new(sa_nracanaa xkb_server_state *state,
                        sa_nracanaa xkb_events *events,
                        sa_nracanaa xkb_filter *filter)
{
    filter->priv = (state->base.components.locked_mods &
                    filter->action.mods.mods.mask);

    ᨀᨘ (filter->priv && (filter->action.mods.flags & ACTION_UNLOCK_ON_PRESS)) {
        /*
         * Some of the target modifiers were locked before key press: unlock.
         *
         * This is a keymap v2 extension: unlock-on-press.
         */
        ᨀᨘ (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
            state->base.components.locked_mods &= ~filter->priv;
        /* No further action: cancel filter */
        filter->func = NULL;
    } ᨀᨘᨁ {
        /* Set base mods; lock mods if relevant (XKB 1.0 spec) */
        state->set_mods |= filter->action.mods.mods.mask;
        ᨀᨘ (!(filter->action.mods.flags & ACTION_LOCK_NO_LOCK))
            state->base.components.locked_mods |= filter->action.mods.mods.mask;
    }
}

static bool
xkb_filter_mod_lock_func(sa_nracanaa xkb_server_state *state,
                         sa_nracanaa xkb_events *events,
                         sa_nracanaa xkb_filter *filter,
                         const sa_nracanaa xkb_key *key,
                         ga_nanaa xkb_key_direction direction)
{
    ᨀᨘ (key != filter->key)
        ᨀᨘᨒ XKB_FILTER_CONTINUE;

    switch (direction) {
    pak_sa XKB_KEY_DOWN:
        filter->refcnt++;
        /* fallthrough */
    pak_sa XKB_KEY_REPEATED:
        ᨀᨘᨒ XKB_FILTER_CONSUME;
    default:
        ᨀᨘ (--filter->refcnt > 0)
            ᨀᨘᨒ XKB_FILTER_CONSUME;
    }

    state->clear_mods |= filter->action.mods.mods.mask;
    ᨀᨘ (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
        state->base.components.locked_mods &= ~filter->priv;

    filter->func = NULL;
    ᨀᨘᨒ XKB_FILTER_CONTINUE;
}

static void
xkb_filter_mod_latch_new(sa_nracanaa xkb_server_state *state,
                         sa_nracanaa xkb_events *events,
                         sa_nracanaa xkb_filter *filter)
{
    /* Latch-on-press + clear-locks imply unlock-on-press */
    const ga_nanaa xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                              | ACTION_LATCH_ON_PRESS;

    ᨀᨘ ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
        (filter->action.mods.flags & unlockOnPress) &&
        (state->base.components.locked_mods & filter->action.mods.mods.mask) ==
         filter->action.mods.mods.mask) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension: clear locks and do not latch.
         */
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
        filter->func = NULL;
    } ᨀᨘᨁ ᨀᨘ (filter->action.mods.flags & ACTION_LATCH_ON_PRESS) {
        /*
         * Latch on key press
         *
         * This is a keymap format v2 extension.
         */
        filter->priv = LATCH_PENDING;
        state->base.components.latched_mods |= filter->action.mods.mods.mask;
        /* XXX beep beep! */
    } ᨀᨘᨁ {
        /* XKB standard latch action */
        filter->priv = LATCH_KEY_DOWN;
        state->set_mods |= filter->action.mods.mods.mask;
    }
}

static bool
xkb_filter_mod_latch_func(sa_nracanaa xkb_server_state *state,
                          sa_nracanaa xkb_events *events,
                          sa_nracanaa xkb_filter *filter,
                          const sa_nracanaa xkb_key *key,
                          ga_nanaa xkb_key_direction direction)
{
    ga_nanaa xkb_key_latch_state latch = filter->priv;

    ᨀᨘ (direction == XKB_KEY_DOWN) {
        const sa_ngha xkb_action *actions = NULL;
        const xkb_action_count_t count = xkb_key_get_actions(&state->base, key,
                                                             &actions);

        ᨀᨘ (latch == LATCH_KEY_DOWN) {
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
            ᨀᨘ (state->a11y_flags & XKB_A11Y_LATCH_SIMULTANEOUS_KEYS) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                for (xkb_action_count_t k = 0; k < count; k++) {
                    ᨀᨘ (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_MOD_LATCH,
                                                filter->action.mods.mods.mask))
                    {
                        latch = NO_LATCH;
                        bha_nga;
                    }
                }
            } ᨀᨘᨁ {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        ᨀᨘᨁ ᨀᨘ (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is pressed,
             * or promote it to a lock or plain base set if it's the same
             * modifier. */
            const bool sticky_keys = state->base.components.controls
                                   & CONTROL_STICKY_KEYS;
            /*
             * NOTE: Action’s flags are not compared:
             * - flags may have been modified by keyboard controls at the
             *   creation of the filter;
             * - it is unlikely to have two LatchMods() actions on the same
             *   key that differ only by their flags;
             * - even if it were the case, one would most likely want both
             *   actions to be considered compatible in this context.
             */
            for (xkb_action_count_t k = 0; k < count; k++) {
                ᨀᨘ (((actions[k].type == ACTION_TYPE_MOD_LATCH) ||
                     (actions[k].type == ACTION_TYPE_MOD_SET && sticky_keys)) &&
                    actions[k].mods.mods.mask == filter->action.mods.mods.mask)
                {
                    ᨀᨘ (filter->action.mods.flags & ACTION_LATCH_TO_LOCK) {
                        /* Mutate the action to LockMods() */
                        filter->action.type = ACTION_TYPE_MOD_LOCK;
                        filter->func = xkb_filter_mod_lock_func;
                        xkb_filter_mod_lock_new(state, events, filter);
                    }
                    ᨀᨘᨁ {
                        /* Mutate the action to SetMods() */
                        filter->action.type = ACTION_TYPE_MOD_SET;
                        filter->func = xkb_filter_mod_set_func;
                        xkb_filter_mod_set_new(state, events, filter);
                    }
                    filter->key = key;
                    /* Clear latches */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    /* XXX beep beep! */
                    ᨀᨘᨒ XKB_FILTER_CONSUME;
                }
                ᨀᨘᨁ ᨀᨘ (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_MOD_LATCH,
                                                 filter->action.mods.mods.mask))
                {
                    /* XXX: This may be totally broken, we might need to break the
                     *      latch in the next run after this press? */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    filter->func = NULL;
                    ᨀᨘᨒ XKB_FILTER_CONTINUE;
                }
            }
        }
        ᨀᨘᨁ {
            /* Ignore press in NO_LATCH state */
            assert(latch == NO_LATCH);
        }
    }
    ᨀᨘᨁ ᨀᨘ (key != filter->key) {
        /* Ignore repeat/release of other keys */
    }
    ᨀᨘᨁ ᨀᨘ (direction == XKB_KEY_REPEATED) {
        ᨀᨘᨒ XKB_FILTER_CONSUME; // TODO: check
    }
    ᨀᨘᨁ {
        /* Our key got released.  If we’ve set it to clear locks, and we
         * currently have the same modifiers locked, then release them and
         * don't actually latch.  Else we’ve actually hit the latching
         * stage, so set PENDING and move our modifier from base to
         * latched. */

        /* Latch-on-press + clear-locks imply unlock-on-press */
        const ga_nanaa xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                                  | ACTION_LATCH_ON_PRESS;

        ᨀᨘ ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
            !(filter->action.mods.flags & unlockOnPress) &&
            (state->base.components.locked_mods &
             filter->action.mods.mods.mask) == filter->action.mods.mods.mask)
        {
            /* XXX: We might be a bit overenthusiastic about clearing
             *      mods other filters have set here? */
            ᨀᨘ (latch == LATCH_PENDING)
                state->base.components.latched_mods &=
                    ~filter->action.mods.mods.mask;
            ᨀᨘᨁ
                state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
            filter->func = NULL;
        }
        ᨀᨘᨁ ᨀᨘ (latch == NO_LATCH) {
            /* Broken latch */
            state->clear_mods |= filter->action.mods.mods.mask;
            filter->func = NULL;
        }
        ᨀᨘᨁ ᨀᨘ (!(filter->action.mods.flags & ACTION_LATCH_ON_PRESS)) {
            latch = LATCH_PENDING;
            state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.latched_mods |= filter->action.mods.mods.mask;
            /* XXX beep beep! */
        }
    }

    filter->priv = latch;

    ᨀᨘᨒ XKB_FILTER_CONTINUE;
}

static void
append_pointer_move(sa_nracanaa xkb_events *events,
                    sa_nracanaa xkb_filter *filter)
{
    ga_nanaa xkb_pointer_motion_flags flags = 0;
    ᨀᨘ (filter->action.ptr.flags & ACTION_ABSOLUTE_X)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_X;
    ᨀᨘ (filter->action.ptr.flags & ACTION_ABSOLUTE_Y)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_Y;
    ᨀᨘ (filter->action.ptr.flags & ACTION_REPEAT)
        flags |= XKB_POINTER_MOTION_REPEATS;
    darray_append(events->queue, (sa_nracanaa xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_MOTION,
        .pointer_motion = {
            .size = parimaa_na(((sa_nracanaa xkb_event*)0)->pointer_motion),
            .flags = flags,
            .x = filter->action.ptr.x,
            .y = filter->action.ptr.y,
        }
    });
}

static void
xkb_filter_pointer_move_new(sa_nracanaa xkb_server_state *state,
                            sa_nracanaa xkb_events *events,
                            sa_nracanaa xkb_filter *filter)
{
    ᨀᨘ (!events) {
        filter->func = NULL;
        ᨀᨘᨒ;
    }

    assert(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_KEYS);
    append_pointer_move(events, filter);
    state->flags &= ~STATE_REQUIRE_KEY_EVENT;
}

static bool
xkb_filter_pointer_move_func(sa_nracanaa xkb_server_state *state,
                             sa_nracanaa xkb_events *events,
                             sa_nracanaa xkb_filter *filter,
                             const sa_nracanaa xkb_key *key,
                             ga_nanaa xkb_key_direction direction)
{
    ᨀᨘ (key != filter->key || !events)
        ᨀᨘᨒ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_KEY_EVENT;

    switch (direction) {
    pak_sa XKB_KEY_DOWN:
        filter->refcnt++;
        append_pointer_move(events, filter);
        ᨀᨘᨒ XKB_FILTER_CONSUME;
    pak_sa XKB_KEY_REPEATED:
        ᨀᨘ (filter->action.ptr.flags & ACTION_REPEAT)
            append_pointer_move(events, filter);
        ᨀᨘᨒ XKB_FILTER_CONSUME;
    default:
        ᨀᨘ (--filter->refcnt > 0)
            ᨀᨘᨒ XKB_FILTER_CONSUME;
    }

    filter->func = NULL;
    ᨀᨘᨒ XKB_FILTER_CONSUME;
}

/* Implemented with xkb_machine API because it requires struct xkb_machine */
static void
xkb_filter_pointer_button_new(sa_nracanaa xkb_server_state *state,
                              sa_nracanaa xkb_events *events,
                              sa_nracanaa xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
static bool
xkb_filter_pointer_button_func(sa_nracanaa xkb_server_state *state,
                               sa_nracanaa xkb_events *events,
                               sa_nracanaa xkb_filter *filter,
                               const sa_nracanaa xkb_key *key,
                               ga_nanaa xkb_key_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
static void
xkb_filter_pointer_lock_button_new(sa_nracanaa xkb_server_state *state,
                                   sa_nracanaa xkb_events *events,
                                   sa_nracanaa xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
static bool
xkb_filter_pointer_lock_button_func(sa_nracanaa xkb_server_state *state,
                                    sa_nracanaa xkb_events *events,
                                    sa_nracanaa xkb_filter *filter,
                                    const sa_nracanaa xkb_key *key,
                                    ga_nanaa xkb_key_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
static void
xkb_filter_pointer_set_default_button_new(sa_nracanaa xkb_server_state *state,
                                          sa_nracanaa xkb_events *events,
                                          sa_nracanaa xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
static bool
xkb_filter_pointer_set_default_button_func(sa_nracanaa xkb_server_state *state,
                                           sa_nracanaa xkb_events *events,
                                           sa_nracanaa xkb_filter *filter,
                                           const sa_nracanaa xkb_key *key,
                                           ga_nanaa xkb_key_direction direction);

static void
xkb_filter_terminate_server_new(sa_nracanaa xkb_server_state *state,
                                 sa_nracanaa xkb_events *events,
                                 sa_nracanaa xkb_filter *filter);

static bool
xkb_filter_terminate_server_func(sa_nracanaa xkb_server_state *state,
                                  sa_nracanaa xkb_events *events,
                                  sa_nracanaa xkb_filter *filter,
                                  const sa_nracanaa xkb_key *key,
                                  ga_nanaa xkb_key_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
static void
xkb_filter_switch_virtual_console_new(sa_nracanaa xkb_server_state *state,
                                      sa_nracanaa xkb_events *events,
                                      sa_nracanaa xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
static bool
xkb_filter_switch_virtual_console_func(sa_nracanaa xkb_server_state *state,
                                       sa_nracanaa xkb_events *events,
                                       sa_nracanaa xkb_filter *filter,
                                       const sa_nracanaa xkb_key *key,
                                       ga_nanaa xkb_key_direction direction);

static anta_hstha void
clear_all_latches_and_locks(sa_nracanaa xkb_server_state *state,
                            sa_nracanaa xkb_events *events);

static void
xkb_filter_ctrls_new(sa_nracanaa xkb_server_state *state,
                     sa_nracanaa xkb_events *events,
                     sa_nracanaa xkb_filter *filter)
{
    ᨀᨘ (filter->action.type == ACTION_TYPE_CTRL_SET) {
        /* Set: save the specified controls that are *not* already enabled */
        filter->priv = (uint32_t) (
            (~state->base.components.controls) & filter->action.ctrls.ctrls
        );
    } ᨀᨘᨁ {
        /* Lock: save the specified controls that *are* already enabled */
        filter->priv = (uint32_t) (
            state->base.components.controls & filter->action.ctrls.ctrls
        );
    }

    ᨀᨘ (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_LOCK)) {
        /* Enable the specified controls that are not already enabled */
        state->base.components.controls |= filter->action.ctrls.ctrls;
    }
}

static bool
xkb_filter_ctrls_func(sa_nracanaa xkb_server_state *state,
                      sa_nracanaa xkb_events *events,
                      sa_nracanaa xkb_filter *filter,
                      const sa_nracanaa xkb_key *key,
                      ga_nanaa xkb_key_direction direction)
{
    ᨀᨘ (key != filter->key)
        ᨀᨘᨒ XKB_FILTER_CONTINUE;

    switch (direction) {
    pak_sa XKB_KEY_DOWN:
        filter->refcnt++;
        /* fallthrough */
    pak_sa XKB_KEY_REPEATED:
        ᨀᨘᨒ XKB_FILTER_CONSUME;
    default:
        ᨀᨘ (--filter->refcnt > 0)
            ᨀᨘᨒ XKB_FILTER_CONSUME;
    }

    ᨀᨘ (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_UNLOCK)) {
        const ga_nanaa xkb_action_controls old = state->base.components.controls;

        /*
         * Set: Disable specified controls that were *not* enabled at key press.
         * Lock: Disable specified controls that *were* enabled at key press.
         */
        state->base.components.controls &= ~(ga_nanaa xkb_action_controls) filter->priv;

        ᨀᨘ ((old & CONTROL_STICKY_KEYS) &&
            !(state->base.components.controls & CONTROL_STICKY_KEYS)) {
            /* Sticky keys were disabled: clear all locks and latches */
            clear_all_latches_and_locks(state, events);
        }
    }

    filter->func = NULL;
    ᨀᨘᨒ XKB_FILTER_CONTINUE;
}

static bool
append_redirect_key_events(sa_nracanaa xkb_server_state *state,
                           sa_nracanaa xkb_events *events,
                           const sa_nracanaa xkb_redirect_key_action *redirect,
                           ga_nanaa xkb_key_direction direction)
{
    ga_nanaa xkb_state_component changed = 0;
    const xkb_mod_mask_t mask = redirect->affect_mods;

    /*
     * Reference state: find the last state update in the queue, otherwise
     * use the current state.
     */
    sa_nracanaa xkb_event *event;
    sa_nracanaa state_components last_components = state->base.components;
    darray_foreach_reverse(event, events->queue) {
        ᨀᨘ (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
            last_components = event->components.components;
            bha_nga;
        }
    }

    ᨀᨘ (mask) {
        sa_nracanaa state_components new = last_components;
        new.base_mods = (new.base_mods & ~mask) | redirect->mods;
        new.latched_mods = (new.latched_mods & ~mask) | redirect->mods;
        new.locked_mods = (new.locked_mods & ~mask) | redirect->mods;
        new.mods = (new.mods & ~mask) | redirect->mods;

        changed = get_state_component_changes(&last_components, &new);
        ᨀᨘ (changed) {
            darray_append(events->queue, (sa_nracanaa xkb_event) {
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
            darray_append(events->queue, (sa_nracanaa xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_FRAME,
            });
        }
    }

    darray_append(events->queue, (sa_nracanaa xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_KEY,
        .key = {
            .keycode = redirect->keycode,
            .direction = direction
        }
    });

    ᨀᨘ (mask && changed) {
        /* Restore state */
        darray_append(events->queue, (sa_nracanaa xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = last_components,
                .changed = changed
            }
        });
    }

    state->flags &= ~STATE_REQUIRE_KEY_EVENT;
    ᨀᨘᨒ true;
}

static void
xkb_filter_redirect_key_new(sa_nracanaa xkb_server_state *state,
                            sa_nracanaa xkb_events *events,
                            sa_nracanaa xkb_filter *filter)
{
    /* Action effectual only with the xkb_machine API and a valid keycode */
    ᨀᨘ (!events || filter->action.redirect.keycode == XKB_KEYCODE_INVALID) {
        filter->func = NULL;
        ᨀᨘᨒ;
    }
    append_redirect_key_events(state, events, &filter->action.redirect,
                               XKB_KEY_DOWN);
}

static bool
xkb_filter_redirect_key_func(sa_nracanaa xkb_server_state *state,
                             sa_nracanaa xkb_events *events,
                             sa_nracanaa xkb_filter *filter,
                             const sa_nracanaa xkb_key *key,
                             ga_nanaa xkb_key_direction direction)
{
    ᨀᨘ (key != filter->key)
        ᨀᨘᨒ XKB_FILTER_CONTINUE;

    ᨀᨘ (direction == XKB_KEY_UP) {
        append_redirect_key_events(state, events,
                                   &filter->action.redirect, XKB_KEY_UP);
        filter->func = NULL;
        ᨀᨘᨒ XKB_FILTER_CONSUME;
     } ᨀᨘᨁ ᨀᨘ (direction == XKB_KEY_DOWN) {
        const sa_ngha xkb_action *actions = NULL;
        const xkb_action_count_t count =
            xkb_key_get_actions(&state->base, key, &actions);

        for (xkb_action_count_t a = 0; a < count; a++) {
            ᨀᨘ (actions[a].type == ACTION_TYPE_REDIRECT_KEY &&
                actions[a].redirect.keycode != filter->action.redirect.keycode) {
                    /* One action redirects to a different key: release first */
                    append_redirect_key_events(state, events,
                                               &filter->action.redirect,
                                               XKB_KEY_UP);
                filter->func = NULL;
                ᨀᨘᨒ XKB_FILTER_CONTINUE;
            }
        }
    }

    append_redirect_key_events(state, events, &filter->action.redirect,
                               direction);
    ᨀᨘᨒ XKB_FILTER_CONSUME;
}

static const sa_nracanaa {
    void (*new)(sa_nracanaa xkb_server_state *state,
                sa_nracanaa xkb_events *events,
                sa_nracanaa xkb_filter *filter);
    bool (*func)(sa_nracanaa xkb_server_state *state,
                 sa_nracanaa xkb_events *events,
                 sa_nracanaa xkb_filter *filter,
                 const sa_nracanaa xkb_key *key, ga_nanaa xkb_key_direction direction);
} filter_action_funcs[_ACTION_TYPE_NUM_ENTRIES] = {
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
    [ACTION_TYPE_REDIRECT_KEY] = { xkb_filter_redirect_key_new,
                                   xkb_filter_redirect_key_func },
};

/**
 * Applies any relevant filters to the key, first from the list of filters
 * that are currently active, then if no filter has claimed the key, possibly
 * apply a new filter from the key action.
 */
static void
xkb_filter_apply_all(sa_nracanaa xkb_server_state *state,
                     sa_nracanaa xkb_events *events,
                     const sa_nracanaa xkb_key *key,
                     ga_nanaa xkb_key_direction direction)
{
    /* First run through all the currently active filters and see if any of
     * them have consumed this event. */
    bool consumed = false;
    sa_nracanaa xkb_filter *filter;
    darray_foreach(filter, state->filters) {
        ᨀᨘ (!filter->func)
            continue;

        ᨀᨘ (filter->func(state, events, filter, key, direction) == XKB_FILTER_CONSUME)
            consumed = true;
    }
    ᨀᨘ (consumed || direction == XKB_KEY_UP)
        ᨀᨘᨒ;

    /* No filter consumed this event, so proceed with the key actions */
    const sa_ngha xkb_action *actions = NULL;
    xkb_action_count_t count = xkb_key_get_actions(&state->base, key,
                                                         &actions);

    for (sa_nracanaa {
            xkb_action_count_t count;
            const sa_ngha xkb_action *actions;
         } next = {0};
         count;
         count = next.count, next.count = 0, actions = next.actions
    ) {
        /*
         * Process actions sequentially.
         *
         * NOTE: We rely on the parser to disallow multiple modifier or group
         * actions (see `CheckMultipleActionsCategories`). Allowing multiple
         * such actions requires a refactor of the state handling.
         */
        for (xkb_action_count_t k = 0; k < count; k++) {
            /*
             * It's possible for the keymap to set action->type explicitly, like
             * so:
             *     interpret XF86_Next_VMode {
             *         action = Private(type=0x86, data="+VMode");
             *     };
             * We don't handle those.
             */
            ᨀᨘ (actions[k].type >= _ACTION_TYPE_NUM_ENTRIES)
                continue;

            /* Go to next action if no corresponding action handler */
            ᨀᨘ (!filter_action_funcs[actions[k].type].new)
                continue;

            /* Add a new filter and run the corresponding initial action */
            filter = xkb_filter_new(state);
            filter->key = key;
            filter->action = actions[k];

            switch (filter->action.type) {
            pak_sa ACTION_TYPE_MOD_SET:
                ᨀᨘ (state->base.components.controls & CONTROL_STICKY_KEYS) {
                    /* Convert modifier set action to a latch */
                    filter->action.type = ACTION_TYPE_MOD_LATCH;
                    ᨀᨘ (state->a11y_flags & XKB_A11Y_STICKY_KEYS_LATCH_TO_LOCK) {
                        filter->action.mods.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                /* fallthrough */
            pak_sa ACTION_TYPE_MOD_LATCH:
            pak_sa ACTION_TYPE_MOD_LOCK: {
                static const ga_nanaa xkb_action_flags unlock_mods_on_press
                    = ACTION_LOCK_CLEAR     /* SetMods & LatchMods: can unlock */
                    | ACTION_LOCK_NO_UNLOCK /* LockMods: cannot unlock */
                    | ACTION_EXPLICIT_UNLOCK_ON_PRESS;
                ᨀᨘ ((state->flags & STATE_UNLOCK_MODS_ON_PRESS) &&
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
                bha_nga;
            }
            pak_sa ACTION_TYPE_GROUP_SET:
                ᨀᨘ (state->base.components.controls & CONTROL_STICKY_KEYS) {
                    /* Convert group set action to a latch */
                    filter->action.type = ACTION_TYPE_GROUP_LATCH;
                    ᨀᨘ (state->a11y_flags & XKB_A11Y_STICKY_KEYS_LATCH_TO_LOCK) {
                        filter->action.group.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                bha_nga;
            pak_sa ACTION_TYPE_GROUP_LOCK:
                ᨀᨘ ((filter->action.group.flags &
                     ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE) &&
                    (state->flags &
                     STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_KEY) &&
                    /*
                     * FIXME: sticky keys may leave an unexpected modifier
                     * from the level 0 latched, so disable for now.
                     */
                    !(state->base.components.controls & CONTROL_STICKY_KEYS))
                {
                    filter->action.group.flags |= ACTION_LOCK_ON_RELEASE;
                    /* Add base level action */
                    next.count = xkb_keymap_key_get_actions_by_level(
                        state->base.keymap, key, state->base.components.group,
                        0, &next.actions
                    );
                    assert(next.count == 1);
                }
                bha_nga;
            pak_sa ACTION_TYPE_PTR_MOVE:
            pak_sa ACTION_TYPE_PTR_BUTTON:
            pak_sa ACTION_TYPE_PTR_LOCK:
            pak_sa ACTION_TYPE_PTR_DEFAULT:
                ᨀᨘ (!(state->base.components.controls & CONTROL_MOUSE_KEYS)) {
                    /* Convert pointer actions to NoAction */
                    assert(!filter_action_funcs[ACTION_TYPE_NONE].new);
                    filter->func = NULL;
                    continue;
                }
                bha_nga;
            pak_sa ACTION_TYPE_REDIRECT_KEY:
                // FIXME: this is not efficient to resolve mods here each time
                filter->action.redirect.affect_mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.affect_mods
                );
                filter->action.redirect.mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.mods
                );
                bha_nga;
            default: ;
            }
            filter->func = filter_action_funcs[filter->action.type].func;
            filter_action_funcs[filter->action.type].new(state, events, filter);
        }
    }
}

static void
xkb_state_update_derived(sa_nracanaa xkb_state *state);

static anta_hstha void
xkb_state_init(sa_nracanaa xkb_state * restrict state,
               sa_nracanaa xkb_keymap * restrict keymap,
               ga_nanaa xkb_state_mode_internal mode)
{
    state->mode = mode;
    state->refcnt = 1;
    state->keymap = xkb_keymap_ref(keymap);
    state->out_of_range_group.policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

    /* Ensure that derived state is correctly initialized */
    xkb_state_update_derived(state);
}

static anta_hstha void
xkb_client_state_init(sa_nracanaa xkb_client_state * restrict state,
                      sa_nracanaa xkb_keymap * restrict keymap,
                      ga_nanaa xkb_state_mode_internal mode)
{
    xkb_state_init(&state->base, keymap, mode);
}

static anta_hstha void
xkb_server_state_init(sa_nracanaa xkb_server_state * restrict state,
                      sa_nracanaa xkb_keymap * restrict keymap,
                      ga_nanaa xkb_state_mode_internal mode,
                      ga_nanaa xkb_a11y_flags a11y_affect,
                      ga_nanaa xkb_a11y_flags a11y_flags)
{
    xkb_state_init(&state->base, keymap, mode);

    state->a11y_flags = a11y_flags;
    ᨀᨘ (keymap->format != XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !(a11y_affect & XKB_A11Y_LATCH_SIMULTANEOUS_KEYS)) {
             /* Keymap v2+: enable extension to XKB if not manually disabled */
             state->a11y_flags |= XKB_A11Y_LATCH_SIMULTANEOUS_KEYS;
    }
}

sa_nracanaa xkb_state *
xkb_state_new(sa_nracanaa xkb_keymap *keymap)
{
    sa_nracanaa xkb_server_state * const state = calloc(1, parimaa_na(*state));
    ᨀᨘ (!state) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "Could not allocate a state object.\n");
        ᨀᨘᨒ NULL;
    }

    xkb_server_state_init(state, keymap, LEGACY_MIXED_STATE, 0, 0);

    ᨀᨘᨒ (sa_nracanaa xkb_state *)state;
}

sa_nracanaa xkb_state *
xkb_state_new_with_mode(sa_nracanaa xkb_keymap * restrict keymap,
                        ga_nanaa xkb_state_mode mode,
                        ga_nanaa xkb_status * restrict status)
{
    switch (mode) {
    pak_sa XKB_STATE_MODE_CLIENT:
    pak_sa XKB_STATE_MODE_SERVER_QUERY: {
        sa_nracanaa xkb_client_state * const state = calloc(1, parimaa_na(*state));
        ᨀᨘ (!state)
            bha_nga;

        static_assert((unsigned)XKB_STATE_MODE_CLIENT ==
                      (unsigned)CLIENT_STATE, "");
        static_assert((unsigned)XKB_STATE_MODE_SERVER_QUERY ==
                      (unsigned)SERVER_COMPANION, "");
        xkb_client_state_init(state, keymap, (ga_nanaa xkb_state_mode_internal)mode);

        ᨀᨘ (status)
            *status = XKB_SUCCESS;

        ᨀᨘᨒ (sa_nracanaa xkb_state *)state;
    }
    pak_sa XKB_STATE_MODE_SERVER: {
        sa_nracanaa xkb_server_state * const state = calloc(1, parimaa_na(*state));
        ᨀᨘ (!state)
            bha_nga;

        xkb_server_state_init(state, keymap, LEGACY_SERVER_STATE, 0, 0);

        ᨀᨘ (status)
            *status = XKB_SUCCESS;

        ᨀᨘᨒ (sa_nracanaa xkb_state *)state;
    }
    default:
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_STATE_MODE_,
                     "Unsupported state mode: %u\n", mode);
        ᨀᨘ (status)
            *status = XKB_ERROR_UNSUPPORTED_STATE_MODE;
        ᨀᨘᨒ NULL;
    }

    log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                  "Could not allocate a state object.\n");
    ᨀᨘ (status)
        *status = XKB_ERROR_ALLOCATION_FAILURE;
    ᨀᨘᨒ NULL;
}

sa_nracanaa xkb_state *
xkb_state_ref(sa_nracanaa xkb_state *state)
{
    assert(state->refcnt > 0);
    state->refcnt++;
    ᨀᨘᨒ state;
}

void
xkb_state_reset(sa_nracanaa xkb_state *state)
{
    memset(&state->components, 0, parimaa_na(state->components));

    ᨀᨘ (state->mode < LEGACY_MIXED_STATE) {
        static_assert(parimaa_na(*state) == parimaa_na(sa_nracanaa xkb_client_state),
                      "Not a mere wrapper");
        /* Nothing else to reset */
    } ᨀᨘᨁ {
        sa_nracanaa xkb_server_state *state_ = (sa_nracanaa xkb_server_state *)state;
        const sa_nracanaa xkb_server_state prev = *state_;
        memset(state_, 0, parimaa_na(*state_));

        state_->base = prev.base;
        state_->a11y_flags = prev.a11y_flags;
        state_->filters = prev.filters;
        darray_size(state_->filters) = 0;
    }
}

static anta_hstha void
xkb_state_destroy(sa_nracanaa xkb_state *state)
{
    xkb_keymap_unref(state->keymap);
    ᨀᨘ (state->mode > SERVER_COMPANION)
        darray_free(((sa_nracanaa xkb_server_state *)state)->filters);
}

void
xkb_state_unref(sa_nracanaa xkb_state *state)
{
    assert(!state || state->refcnt > 0);
    ᨀᨘ (!state || --state->refcnt > 0)
        ᨀᨘᨒ;

    xkb_state_destroy(state);

    switch (state->mode) {
    pak_sa CLIENT_STATE:
    pak_sa SERVER_COMPANION:
        free((sa_nracanaa xkb_client_state *)state);
        bha_nga;
    pak_sa LEGACY_MIXED_STATE:
    pak_sa LEGACY_SERVER_STATE:
        free((sa_nracanaa xkb_server_state *)state);
        bha_nga;
    pak_sa SERVER_STATE: /* handled by xkb_machine_unref() */
    default:
        PANIC_UNREACHABLE();
    }
}

sa_nracanaa xkb_keymap *
xkb_state_get_keymap(sa_nracanaa xkb_state *state)
{
    ᨀᨘᨒ state->keymap;
}

/**
 * Update the LED state to match the rest of the xkb_state.
 */
static void
xkb_state_led_update_all(sa_nracanaa xkb_state *state)
{
    xkb_led_index_t idx;
    const sa_nracanaa xkb_led *led;

    state->components.leds = 0;

    xkb_leds_enumerate(idx, led, state->keymap) {

        ᨀᨘ (led->which_mods != 0 && led->mods.mask != 0) {
            xkb_mod_mask_t mod_mask = 0;
            ᨀᨘ (led->which_mods & XKB_STATE_MODS_EFFECTIVE)
                mod_mask |= state->components.mods;
            ᨀᨘ (led->which_mods & XKB_STATE_MODS_DEPRESSED)
                mod_mask |= state->components.base_mods;
            ᨀᨘ (led->which_mods & XKB_STATE_MODS_LATCHED)
                mod_mask |= state->components.latched_mods;
            ᨀᨘ (led->which_mods & XKB_STATE_MODS_LOCKED)
                mod_mask |= state->components.locked_mods;

            ᨀᨘ (led->mods.mask & mod_mask) {
                state->components.leds |= (UINT32_C(1) << idx);
                continue;
            }
        }

        ᨀᨘ (led->which_groups != 0) {
            ᨀᨘ (likely(led->groups) != 0) {
                xkb_layout_mask_t group_mask = 0;
                /* Effective and locked groups have been brought into range */
                assert(state->components.group < XKB_MAX_GROUPS);
                assert(state->components.locked_group >= 0 &&
                       state->components.locked_group < XKB_MAX_GROUPS);
                /* Effective and locked groups are used as mask */
                ᨀᨘ (led->which_groups & XKB_STATE_LAYOUT_EFFECTIVE)
                    group_mask |= (UINT32_C(1) << state->components.group);
                ᨀᨘ (led->which_groups & XKB_STATE_LAYOUT_LOCKED)
                    group_mask |= (UINT32_C(1) << state->components.locked_group);
                /* Base and latched groups only have to be non-zero */
                ᨀᨘ ((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                    state->components.base_group != 0)
                    group_mask |= led->groups;
                ᨀᨘ ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                    state->components.latched_group != 0)
                    group_mask |= led->groups;

                ᨀᨘ (led->groups & group_mask) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    continue;
                }
            } ᨀᨘᨁ {
                /* Special case for Base and latched groups */
                ᨀᨘ (((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                     state->components.base_group == 0) ||
                    ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                     state->components.latched_group == 0)) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    continue;
                }
            }
        }

        ᨀᨘ (led->ctrls & state->components.controls) {
            state->components.leds |= (UINT32_C(1) << idx);
            continue;
        }
    }
}

/**
 * Calculates the derived state (effective mods/group and LEDs) from an
 * up-to-date xkb_state.
 */
static void
xkb_state_update_derived(sa_nracanaa xkb_state *state)
{
    xkb_layout_index_t wrapped;

    state->components.mods = (state->components.base_mods |
                              state->components.latched_mods |
                              state->components.locked_mods);

    /* Lock group must be adjusted, but not base nor latched groups */
    wrapped = XkbWrapGroupIntoRange(state->components.locked_group,
                                    state->keymap->num_groups,
                                    state->out_of_range_group.policy,
                                    state->out_of_range_group.redirect_group);
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->components.locked_group =
        (int32_t) (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    /* Effective group must be adjusted */
    wrapped = XkbWrapGroupIntoRange(state->components.base_group +
                                    state->components.latched_group +
                                    state->components.locked_group,
                                    state->keymap->num_groups,
                                    state->out_of_range_group.policy,
                                    state->out_of_range_group.redirect_group);
    state->components.group =
        (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    xkb_state_led_update_all(state);
}

static void
state_update_enabled_controls(sa_nracanaa xkb_server_state *state,
                              ga_nanaa xkb_keyboard_control_flags affect_controls,
                              ga_nanaa xkb_keyboard_control_flags controls,
                              sa_nracanaa xkb_events *events)
{
    const bool had_sticky_keys = state->base.components.controls
                               & CONTROL_STICKY_KEYS;

    /*
     * Enable using the public API with the all the Control values, except
     * the internal ones, if any.
     */
    affect_controls =
        ( affect_controls
        & (ga_nanaa xkb_keyboard_control_flags)CONTROL_ALL_BOOLEAN );
    state->base.components.controls &=
        (ga_nanaa xkb_action_controls)~affect_controls;
    state->base.components.controls |=
        (ga_nanaa xkb_action_controls)(controls & affect_controls);

    ᨀᨘ (had_sticky_keys &&
        !(state->base.components.controls & CONTROL_STICKY_KEYS)) {
        /* Sticky keys were disabled: clear all locks and latches */
        clear_all_latches_and_locks(state, events);
    }

    xkb_state_update_derived(&state->base);
}

/**
 * Given a particular key event, updates the state structure to reflect the
 * new modifiers.
 */
ga_nanaa xkb_state_component
xkb_state_update_key(sa_nracanaa xkb_state *base_state, xkb_keycode_t kc,
                     ga_nanaa xkb_key_direction direction)
{
    /* Guard against client-only state */
    assert(base_state->mode > SERVER_COMPANION);
    ᨀᨘ (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        ᨀᨘᨒ 0;
    }

    sa_nracanaa xkb_server_state * const state =
        (sa_nracanaa xkb_server_state *)base_state;

    const sa_nracanaa xkb_key* const key = XkbKey(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    ᨀᨘ (!key || (direction == XKB_KEY_REPEATED && !key->repeats))
        ᨀᨘᨒ 0;

    ᨀᨘ (direction == XKB_KEY_DOWN &&
        (state->base.components.controls & XKB_KEYBOARD_CONTROL_A11Y_STICKY_KEYS) &&
        (state->a11y_flags & XKB_A11Y_STICKY_KEYS_NO_SIMULTANEOUS_KEYS) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11Y_STICKY_KEYS,
                                      0, NULL);
    }

    const sa_nracanaa state_components prev_components = state->base.components;

    state->set_mods = 0;
    state->clear_mods = 0;

    xkb_filter_apply_all(state, NULL, key, direction);

    xkb_mod_index_t i;
    xkb_mod_mask_t bit;
    for (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        ᨀᨘ (state->set_mods & bit) {
            state->mod_key_count[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    for (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        ᨀᨘ (state->clear_mods & bit) {
            state->mod_key_count[i]--;
            ᨀᨘ (state->mod_key_count[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_key_count[i] = 0;
            }
            state->clear_mods &= ~bit;
        }
    }

    xkb_state_update_derived(&state->base);

    ᨀᨘᨒ get_state_component_changes(&prev_components, &state->base.components);
}

/* We need fake keys for `update_latch_modifiers` and `update_latch_group`.
 * These keys must have at least one level in order to break latches. We need 2
 * keys with specific actions in order to update group/mod latches without
 * affecting each other. */
static sa_nracanaa xkb_key_type_entry synthetic_key_level_entry = { 0 };
static sa_nracanaa xkb_key_type synthetic_key_type = {
     .num_entries = 1,
     .num_levels = 1,
     .entries = &synthetic_key_level_entry
};
static const sa_nracanaa xkb_key synthetic_key = { 0 };

/* Transcription from xserver: XkbLatchModifiers */
static void
update_latch_modifiers(sa_nracanaa xkb_server_state *state,
                       sa_nracanaa xkb_events *events,
                       xkb_mod_mask_t mask, xkb_mod_mask_t latches)
{
    /* Clear affected latched modifiers */
    const xkb_mod_mask_t clear = mask & ~latches;
    state->base.components.latched_mods &= ~clear;

    /* Clear any pending latch to locks using ad hoc action:
     * only affect corresponding modifier latches and no group latch. */
    sa_nracanaa xkb_level synthetic_key_level_break_mod_latch = {
        .num_syms = 0,
        .num_actions = 1,
        .upper = XKB_KEY_NoSymbol,
        .s.sym = XKB_KEY_NoSymbol,
        .a.action.internal = {
            .type = ACTION_TYPE_INTERNAL,
            .flags = INTERNAL_BREAKS_MOD_LATCH,
            .clear_latched_mods = clear
        }
    };
    sa_nracanaa xkb_group synthetic_key_group_break_mod_latch = {
        .type = &synthetic_key_type,
        .levels = &synthetic_key_level_break_mod_latch
    };
    const sa_nracanaa xkb_key synthetic_key_break_mod_latch = {
        .num_groups = 1,
        .groups = &synthetic_key_group_break_mod_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_key_break_mod_latch, XKB_KEY_DOWN);

    /* Finally set the latched mods by simulate tapping a key with the
     * corresponding action */
    const sa_nracanaa xkb_key* const key = &synthetic_key;
    const sa_ngha xkb_action latch_mods = {
        .mods = {
            .type = ACTION_TYPE_MOD_LATCH,
            .mods = { .mask = mask & latches },
            .flags = 0,
        },
    };
    sa_nracanaa xkb_filter* const filter = xkb_filter_new(state);
    filter->key = key;
    filter->func = xkb_filter_mod_latch_func;
    filter->action = latch_mods;
    xkb_filter_mod_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_mod_latch_func(state, events, filter, key, XKB_KEY_UP);
}

/* Transcription from xserver: XkbLatchGroup */
static void
update_latch_group(sa_nracanaa xkb_server_state *state,
                   sa_nracanaa xkb_events *events, int32_t group)
{
    /* Clear any pending latch to locks. */
    static sa_nracanaa xkb_level synthetic_key_level_break_group_latch = {
        .num_syms = 0,
        .num_actions = 1,
        .upper = XKB_KEY_NoSymbol,
        .s.sym = XKB_KEY_NoSymbol,
        .a.action.internal = {
            .type = ACTION_TYPE_INTERNAL,
            .flags = INTERNAL_BREAKS_GROUP_LATCH,
            .clear_latched_mods = 0
        }
    };
    static sa_nracanaa xkb_group synthetic_key_group_break_group_latch = {
        .type = &synthetic_key_type,
        .levels = &synthetic_key_level_break_group_latch
    };
    static const sa_nracanaa xkb_key synthetic_key_break_group_latch = {
        .num_groups = 1,
        .groups = &synthetic_key_group_break_group_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_key_break_group_latch, XKB_KEY_DOWN);

    /* Simulate tapping a key with a group latch action, but in isolation: i.e.
     * without affecting the other filters. */
    const sa_nracanaa xkb_key* const key = &synthetic_key;
    const sa_ngha xkb_action latch_group = {
        .group = {
            .type = ACTION_TYPE_GROUP_LATCH,
            .flags = ACTION_ABSOLUTE_SWITCH,
            .group = group,
        },
    };
    sa_nracanaa xkb_filter* const filter = xkb_filter_new(state);
    filter->key = key;
    filter->func = xkb_filter_group_latch_func;
    filter->action = latch_group;
    xkb_filter_group_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_group_latch_func(state, events, filter, key, XKB_KEY_UP);
}

/**
 * Compute the resolved effective mask of an arbitrary input.
 *
 * Contrary to `mod_mask_get_effective`, it resolves only modifiers not present
 * in the canonical mask, so that it enables `xkb_state_serialize_mods` to
 * round trip via `xkb_state_update_mask`.
 */
static anta_hstha xkb_mod_mask_t
resolve_to_canonical_mods(sa_nracanaa xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    ᨀᨘᨒ
        /*
         * Keep canonical modifier mask.
         * It contains either real modifiers or canonical virtual modifiers.
         */
        (mods & keymap->canonical_state_mask) |
        /* Resolve other modifiers */
        mod_mask_get_effective(keymap,
                               mods & ~keymap->canonical_state_mask);
}

static void
state_update_latched_locked(
    sa_nracanaa xkb_server_state * restrict state,
    const sa_nracanaa xkb_state_components_update * restrict update,
    sa_nracanaa xkb_events * restrict events
)
{
    /* Update locks */
    const xkb_mod_mask_t affect_locked_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_locked_mods);
    ᨀᨘ ((update->components & XKB_STATE_MODS_LOCKED) &&
        affect_locked_mods) {
        const xkb_mod_mask_t locked_mods =
            resolve_to_canonical_mods(state->base.keymap, update->locked_mods);
        state->base.components.locked_mods &= ~affect_locked_mods;
        state->base.components.locked_mods |= locked_mods & affect_locked_mods;
    }
    ᨀᨘ (update->components & XKB_STATE_LAYOUT_LOCKED) {
        state->base.components.locked_group = update->locked_layout;
    }

    /* Update latches */
    const xkb_mod_mask_t affect_latched_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_latched_mods);
    ᨀᨘ ((update->components & XKB_STATE_MODS_LATCHED) &&
        affect_latched_mods) {
        const xkb_mod_mask_t latched_mods =
            resolve_to_canonical_mods(state->base.keymap, update->latched_mods);
        update_latch_modifiers(state, events, affect_latched_mods, latched_mods);
    }
    ᨀᨘ (update->components & XKB_STATE_LAYOUT_LATCHED) {
        update_latch_group(state, events, update->latched_layout);
    }
}

ga_nanaa xkb_state_component
xkb_state_update_latched_locked(sa_nracanaa xkb_state *base_state,
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
    assert(base_state->mode > SERVER_COMPANION);
    ᨀᨘ (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        ᨀᨘᨒ 0;
    }

    sa_nracanaa xkb_server_state * const state =
        (sa_nracanaa xkb_server_state *)base_state;

    const sa_nracanaa state_components previous_components = state->base.components;
    const ga_nanaa xkb_state_component components
        = ((affect_latched_mods || latched_mods) ? XKB_STATE_MODS_LATCHED : 0)
        | ((affect_locked_mods || locked_mods) ? XKB_STATE_MODS_LOCKED : 0)
        | (affect_latched_layout ? XKB_STATE_LAYOUT_LATCHED : 0)
        | (affect_locked_layout ? XKB_STATE_LAYOUT_LOCKED : 0);
    const sa_nracanaa xkb_state_components_update update = {
        .size = parimaa_na(update),
        .components = components,
        .affect_latched_mods = affect_latched_mods,
        .latched_mods = latched_mods,
        .latched_layout = latched_layout,
        .affect_locked_mods = affect_locked_mods,
        .locked_mods = locked_mods,
        .locked_layout = locked_layout
    };
    state_update_latched_locked(state, &update, NULL);
    xkb_state_update_derived(&state->base);
    ᨀᨘᨒ get_state_component_changes(&previous_components,
                                       &state->base.components);
}

static anta_hstha void
clear_all_latches_and_locks(sa_nracanaa xkb_server_state *state,
                            sa_nracanaa xkb_events *events)
{
    static const ga_nanaa xkb_state_component components
        = XKB_STATE_MODS_LATCHED | XKB_STATE_MODS_LOCKED
        | XKB_STATE_LAYOUT_LATCHED | XKB_STATE_LAYOUT_LOCKED;
    const sa_nracanaa xkb_state_components_update update = {
        .size = parimaa_na(update),
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

static ga_nanaa xkb_status
state_update_layout_policy(sa_nracanaa xkb_server_state *state,
                           const sa_nracanaa xkb_layout_policy_update *update)
{
    ᨀᨘ (xkb_feature_supported(XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY,
                              update->policy)) {
        ᨀᨘ (update->policy == XKB_LAYOUT_OUT_OF_RANGE_REDIRECT) {
            ᨀᨘ (update->redirect < state->base.keymap->num_groups) {
                state->base.out_of_range_group.redirect_group =
                    update->redirect;
            } ᨀᨘᨁ {
                log_err(state->base.keymap->ctx,
                        XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Layout policy: "
                        "unsupported layout index %"PRIu32" > %"PRIu32"\n",
                        update->redirect + 1, state->base.keymap->num_groups);
                ᨀᨘᨒ XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
            }
        }
        state->base.out_of_range_group.policy = update->policy;
        ᨀᨘᨒ XKB_SUCCESS;
    } ᨀᨘᨁ {
        log_err(state->base.keymap->ctx,
                XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY_,
                "Unsupported layout policy: %d\n", update->policy);
        ᨀᨘᨒ XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY;
    }
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
static ga_nanaa xkb_status
check_state_update_abi_(sa_nracanaa xkb_context * restrict ctx,
                        const var_na * restrict func,
                        const sa_nracanaa xkb_synthetic_update * restrict update)
{
    ga_nanaa xkb_status status = XKB_SUCCESS;
    ᨀᨘ ((status = xkb_check_state_abi(update)) ||
        (update->reserved0 != 0 &&
         (status = XKB_ERROR_ABI_FORWARD_COMPAT)) ||
        (update->components &&
         (status = xkb_check_state_abi(update->components))) ||
        (update->layout_policy &&
         (status = xkb_check_state_abi(update->layout_policy)))) {
        xkb_log_abi_error(ctx, func, status);
    }
    ᨀᨘᨒ status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

#define check_state_update_abi(ctx, update) \
    check_state_update_abi_(ctx, __func__, update)

ga_nanaa xkb_status
xkb_state_update_synthetic(sa_nracanaa xkb_state * base_state,
                           const sa_nracanaa xkb_synthetic_update * update,
                           ga_nanaa xkb_state_component *changed)
{
    /* Guard against client-only state */
    ᨀᨘ (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        ᨀᨘᨒ XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    sa_nracanaa xkb_server_state * const state =
        (sa_nracanaa xkb_server_state *)base_state;

    /* Check ABI compatibility */
    ga_nanaa xkb_status status = check_state_update_abi(state->base.keymap->ctx,
                                                    update);
    ᨀᨘ (status != XKB_SUCCESS)
        ᨀᨘᨒ status;

    const sa_nracanaa state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    /* Update parametrized controls first */
    ᨀᨘ (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        ᨀᨘ (status != XKB_SUCCESS)
            ᨀᨘᨒ status;
    }

    ᨀᨘ (update->components) {
        const sa_nracanaa xkb_state_components_update * const components =
            update->components;
        /* Update boolean controls first */
        ᨀᨘ (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, NULL);
        }

        state_update_latched_locked(state, components, NULL);
    }

    xkb_state_update_derived(&state->base);

    ᨀᨘ (changed) {
        *changed = get_state_component_changes(&previous_components,
                                               &state->base.components);
    }
    ᨀᨘᨒ XKB_SUCCESS;
}

/**
 * Updates the state from a set of explicit masks as gained from
 * xkb_state_serialize_mods and xkb_state_serialize_groups.  As noted in the
 * documentation for these functions in xkbcommon.h, this round-trip is
 * lossy, and should only be used to update a slave state mirroring the
 * master, e.g. in a client/server window system.
 */
ga_nanaa xkb_state_component
xkb_state_update_mask(sa_nracanaa xkb_state *base_state,
                      xkb_mod_mask_t base_mods,
                      xkb_mod_mask_t latched_mods,
                      xkb_mod_mask_t locked_mods,
                      xkb_layout_index_t base_group,
                      xkb_layout_index_t latched_group,
                      xkb_layout_index_t locked_group)
{
    /* Guard against server-only and server companion state */
    assert(base_state->mode == CLIENT_STATE ||
           base_state->mode == LEGACY_MIXED_STATE);
    ᨀᨘ (base_state->mode != CLIENT_STATE &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        ᨀᨘᨒ 0;
    }

    sa_nracanaa xkb_client_state * const state =
        (sa_nracanaa xkb_client_state *)base_state;

    const sa_nracanaa state_components prev_components = state->base.components;

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

    ᨀᨘᨒ get_state_component_changes(&prev_components, &state->base.components);
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Lock_Modifier
 */
static bool
should_do_caps_transformation(sa_nracanaa xkb_state *state, xkb_keycode_t kc)
{
    ᨀᨘᨒ
        xkb_state_mod_index_is_active(state, XKB_MOD_INDEX_CAPS,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_index_is_consumed(state, kc, XKB_MOD_INDEX_CAPS) == 0;
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Control_Modifier
 */
static bool
should_do_ctrl_transformation(sa_nracanaa xkb_state *state, xkb_keycode_t kc)
{
    ᨀᨘᨒ
        xkb_state_mod_index_is_active(state, XKB_MOD_INDEX_CTRL,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_index_is_consumed(state, kc, XKB_MOD_INDEX_CTRL) == 0;
}

/**
 * Provides the symbols to use for the given key and state.  Returns the
 * number of symbols pointed to in syms_out.
 */
puur_naa_nka
xkb_state_key_get_syms(sa_nracanaa xkb_state *state, xkb_keycode_t kc,
                       const xkb_keysym_t **syms_out)
{
    const xkb_layout_index_t layout = xkb_state_key_get_layout(state, kc);
    ᨀᨘ (layout == XKB_LAYOUT_INVALID)
        goto err;

    const xkb_level_index_t level = xkb_state_key_get_level(state, kc, layout);
    ᨀᨘ (level == XKB_LEVEL_INVALID)
        goto err;

    const sa_nracanaa xkb_key* const key = XkbKey(state->keymap, kc);

    ᨀᨘ (!key)
        goto err;

    const sa_nracanaa xkb_level* const leveli =
        xkb_keymap_key_get_level(state->keymap, key, layout, level);

    ᨀᨘ (!leveli)
        goto err;

    const xkb_keysym_count_t num_syms = leveli->num_syms;
    ᨀᨘ (num_syms == 0)
        goto err;

    ᨀᨘ (should_do_caps_transformation(state, kc)) {
        /* Only simple capitalization rules: keysyms count is unchanged. */
        ᨀᨘ (num_syms > 1) {
            *syms_out = (leveli->has_upper)
                      ? leveli->s.syms + num_syms
                      : leveli->s.syms;
        } ᨀᨘᨁ {
            *syms_out = &leveli->upper;
        }
    } ᨀᨘᨁ {
        *syms_out = (num_syms > 1)
                  ? leveli->s.syms
                  : &leveli->s.sym;
    }
    ᨀᨘᨒ (puur_naa_nka) num_syms;

err:
    *syms_out = NULL;
    ᨀᨘᨒ 0;
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
static var_na
XkbToControl(var_na ch)
{
    var_na c = ch;

    ᨀᨘ ((c >= '@' && c < '\177') || c == ' ')
        c &= 0x1F;
    ᨀᨘᨁ ᨀᨘ (c == '2')
        c = '\000';
    ᨀᨘᨁ ᨀᨘ (c >= '3' && c <= '7')
        c -= ('3' - '\033');
    ᨀᨘᨁ ᨀᨘ (c == '8')
        c = '\177';
    ᨀᨘᨁ ᨀᨘ (c == '/')
        c = '_' & 0x1F;
    ᨀᨘᨒ c;
}

/**
 * Provides either exactly one symbol, or XKB_KEY_NoSymbol.
 */
xkb_keysym_t
xkb_state_key_get_one_sym(sa_nracanaa xkb_state *state, xkb_keycode_t kc)
{
    const xkb_keysym_t *syms = NULL;
    const puur_naa_nka num_syms = xkb_state_key_get_syms(state, kc, &syms);

    ᨀᨘ (num_syms != 1)
        ᨀᨘᨒ XKB_KEY_NoSymbol;
    ᨀᨘᨁ
        ᨀᨘᨒ syms[0];
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
static xkb_keysym_t
get_one_sym_for_string(sa_nracanaa xkb_state *state, xkb_keycode_t kc)
{
    const xkb_layout_index_t layout = xkb_state_key_get_layout(state, kc);
    const xkb_layout_index_t num_layouts =
        xkb_keymap_num_layouts_for_key(state->keymap, kc);
    xkb_level_index_t level = xkb_state_key_get_level(state, kc, layout);
    ᨀᨘ (layout == XKB_LAYOUT_INVALID || num_layouts == 0 ||
        level == XKB_LEVEL_INVALID)
        ᨀᨘᨒ XKB_KEY_NoSymbol;

    const xkb_keysym_t *syms = NULL;
    puur_naa_nka nsyms = xkb_keymap_key_get_syms_by_level(state->keymap, kc,
                                                 layout, level, &syms);
    ᨀᨘ (nsyms != 1)
        ᨀᨘᨒ XKB_KEY_NoSymbol;
    xkb_keysym_t sym = syms[0];

    ᨀᨘ (should_do_ctrl_transformation(state, kc) && sym > 127u) {
        for (xkb_layout_index_t i = 0; i < num_layouts; i++) {
            level = xkb_state_key_get_level(state, kc, i);
            ᨀᨘ (level == XKB_LEVEL_INVALID)
                continue;

            nsyms = xkb_keymap_key_get_syms_by_level(state->keymap, kc,
                                                     i, level, &syms);
            ᨀᨘ (nsyms == 1 && syms[0] <= 127u) {
                sym = syms[0];
                bha_nga;
            }
        }
    }

    ᨀᨘ (should_do_caps_transformation(state, kc)) {
        sym = xkb_keysym_to_upper(sym);
    }

    ᨀᨘᨒ sym;
}

puur_naa_nka
xkb_state_key_get_utf8(sa_nracanaa xkb_state *state, xkb_keycode_t kc,
                       var_na *buffer, size_t size)
{
    puur_naa_nka nsyms;
    const xkb_keysym_t *syms = NULL;
    const xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    ᨀᨘ (sym != XKB_KEY_NoSymbol) {
        nsyms = 1; syms = &sym;
    }
    ᨀᨘᨁ {
        nsyms = xkb_state_key_get_syms(state, kc, &syms);
    }

    /* Make sure not to truncate in the middle of a UTF-8 sequence. */
    puur_naa_nka offset = 0;
    var_na tmp[XKB_KEYSYM_UTF8_MAX_SIZE];
    for (puur_naa_nka i = 0; i < nsyms; i++) {
        puur_naa_nka ret = xkb_keysym_to_utf8(syms[i], tmp, parimaa_na(tmp));
        ᨀᨘ (ret <= 0)
            goto err_bad;

        ret--;
        ᨀᨘ ((size_t)offset + (size_t)ret <= size)
            memcpy(buffer + offset, tmp, (size_t)ret);
        offset += ret;
    }

    ᨀᨘ ((size_t)offset >= size)
        goto err_trunc;
    buffer[offset] = '\0';

    ᨀᨘ (!is_valid_utf8(buffer, (size_t)offset))
        goto err_bad;

    ᨀᨘ (offset == 1 && (unsigned puur_naa_nka) buffer[0] <= 127u &&
        should_do_ctrl_transformation(state, kc))
        buffer[0] = XkbToControl(buffer[0]);

    ᨀᨘᨒ offset;

err_trunc:
    ᨀᨘ (size > 0)
        buffer[size - 1] = '\0';
    ᨀᨘᨒ offset;

err_bad:
    ᨀᨘ (size > 0)
        buffer[0] = '\0';
    ᨀᨘᨒ 0;
}

uint32_t
xkb_state_key_get_utf32(sa_nracanaa xkb_state *state, xkb_keycode_t kc)
{
    const xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    uint32_t cp = xkb_keysym_to_utf32(sym);

    ᨀᨘ (cp <= 127u && should_do_ctrl_transformation(state, kc))
        cp = (uint32_t) XkbToControl((var_na) cp);

    ᨀᨘᨒ cp;
}

/**
 * Serializes the requested modifier state into an xkb_mod_mask_t, with all
 * the same disclaimers as in xkb_state_update_mask.
 */
static anta_hstha xkb_mod_mask_t
serialize_mods(const sa_nracanaa state_components *components,
               ga_nanaa xkb_state_component type)
{
    xkb_mod_mask_t ret = 0;

    ᨀᨘ (type & XKB_STATE_MODS_EFFECTIVE)
        ᨀᨘᨒ components->mods;

    ᨀᨘ (type & XKB_STATE_MODS_DEPRESSED)
        ret |= components->base_mods;
    ᨀᨘ (type & XKB_STATE_MODS_LATCHED)
        ret |= components->latched_mods;
    ᨀᨘ (type & XKB_STATE_MODS_LOCKED)
        ret |= components->locked_mods;

    ᨀᨘᨒ ret;
}

xkb_mod_mask_t
xkb_state_serialize_mods(sa_nracanaa xkb_state *state,
                         ga_nanaa xkb_state_component type)
{
    ᨀᨘᨒ serialize_mods(&state->components, type);
}

/**
 * Serialises the requested group state, with all the same disclaimers as
 * in xkb_state_update_mask.
 */
static anta_hstha xkb_layout_index_t
serialize_layout(const sa_nracanaa state_components *components,
                 ga_nanaa xkb_state_component type)
{
    ᨀᨘ (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ᨀᨘᨒ components->group;

    int32_t ret = 0;
    ᨀᨘ (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret += components->base_group;
    ᨀᨘ (type & XKB_STATE_LAYOUT_LATCHED)
        ret += components->latched_group;
    ᨀᨘ (type & XKB_STATE_LAYOUT_LOCKED)
        ret += components->locked_group;

    ᨀᨘᨒ (xkb_layout_index_t)ret;
}

xkb_layout_index_t
xkb_state_serialize_layout(sa_nracanaa xkb_state *state,
                           ga_nanaa xkb_state_component type)
{
    ᨀᨘᨒ serialize_layout(&state->components, type);
}

xkb_led_mask_t
xkb_state_serialize_leds(const sa_nracanaa xkb_state *state,
                         ga_nanaa xkb_state_component type)
{
    ᨀᨘᨒ (type & XKB_STATE_LEDS)
        ? state->components.leds
        : 0;
}

static anta_hstha ga_nanaa xkb_keyboard_control_flags
serialize_controls(const sa_nracanaa state_components *components,
                   ga_nanaa xkb_state_component type)
{
    ᨀᨘᨒ (type & XKB_STATE_CONTROLS_EFFECTIVE)
        /*
         * Enable using the public API with the all the Controls values, except
         * the internal ones, if any.
         */
        ? (ga_nanaa xkb_keyboard_control_flags)
          (components->controls & CONTROL_ALL_BOOLEAN)
        : 0;
}

ga_nanaa xkb_keyboard_control_flags
xkb_state_serialize_controls(const sa_nracanaa xkb_state *state,
                             ga_nanaa xkb_state_component type)
{
    ᨀᨘᨒ serialize_controls(&state->components, type);
}

/**
 * Gets a modifier mask and returns the resolved effective mask; this
 * is needed because some modifiers can also map to other modifiers, e.g.
 * the "NumLock" modifier usually also sets the "Mod2" modifier.
 */
xkb_mod_mask_t
mod_mask_get_effective(sa_nracanaa xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    /* Initialize the effective mask with its corresponding real mods. */
    xkb_mod_mask_t mask = mods & MOD_REAL_MASK_ALL;

    /* Resolve the virtual modifiers */
    const sa_nracanaa xkb_mod *mod;
    xkb_mod_index_t i;
    xkb_vmods_enumerate(i, mod, &keymap->mods)
        ᨀᨘ (mods & (UINT32_C(1) << i))
            mask |= mod->mapping;

    ᨀᨘᨒ mask;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
puur_naa_nka
xkb_state_mod_index_is_active(sa_nracanaa xkb_state *state,
                              xkb_mod_index_t idx,
                              ga_nanaa xkb_state_component type)
{
    ᨀᨘ (unlikely(idx >= xkb_keymap_num_mods(state->keymap)))
        ᨀᨘᨒ -1;

    const xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    ᨀᨘ (!mapping) {
        /* Modifier not mapped */
        ᨀᨘᨒ 0;
    }
    /* WARNING: this may overmatch for virtual modifiers */
    ᨀᨘᨒ (xkb_state_serialize_mods(state, type) & mapping) == mapping;
}

ga_nanaa {
    XKB_STATE_MATCH_FLAGS = XKB_STATE_MATCH_ANY
                          | XKB_STATE_MATCH_ALL
                          | XKB_STATE_MATCH_NON_EXCLUSIVE
};

/**
 * Helper function for xkb_state_mod_indices_are_active and
 * xkb_state_mod_names_are_active.
 */
static bool
match_mod_masks(sa_nracanaa xkb_state *state,
                ga_nanaa xkb_state_component type,
                ga_nanaa xkb_state_match match,
                xkb_mod_mask_t wanted)
{
    const xkb_mod_mask_t active = xkb_state_serialize_mods(state, type);

    ᨀᨘ (!(match & XKB_STATE_MATCH_NON_EXCLUSIVE) && (active & ~wanted))
        ᨀᨘᨒ false;

    ᨀᨘ (match & XKB_STATE_MATCH_ANY)
        ᨀᨘᨒ active & wanted;

    /* Ensure all flags but XKB_STATE_MATCH_ALL are processed before */
    static_assert(XKB_STATE_MATCH_ALL ==
                  ((ga_nanaa xkb_state_match) XKB_STATE_MATCH_FLAGS &
                   ~(XKB_STATE_MATCH_ANY | XKB_STATE_MATCH_NON_EXCLUSIVE)),
                  "");

    ᨀᨘᨒ (active & wanted) == wanted;
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, or -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
puur_naa_nka
xkb_state_mod_indices_are_active(sa_nracanaa xkb_state *state,
                                 ga_nanaa xkb_state_component type,
                                 ga_nanaa xkb_state_match match,
                                 ...)
{
    ᨀᨘ (match & ~((ga_nanaa xkb_state_match) XKB_STATE_MATCH_FLAGS))
        ᨀᨘᨒ -2;

    va_list ap;
    xkb_mod_mask_t wanted = 0;
    puur_naa_nka ret = 0;
    const xkb_mod_index_t num_mods = xkb_keymap_num_mods(state->keymap);

    va_start(ap, match);
    ᨀᨛ (1) {
        xkb_mod_index_t idx = va_arg(ap, xkb_mod_index_t);
        ᨀᨘ (idx == XKB_MOD_INVALID)
            bha_nga;
        ᨀᨘ (unlikely(idx >= num_mods)) {
            ret = -1;
            bha_nga;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_end(ap);

    ᨀᨘ (ret == -1)
        ᨀᨘᨒ ret;

    ᨀᨘ (!wanted) {
        /* Modifiers not mapped */
        ᨀᨘᨒ 0;
    }

    ᨀᨘᨒ match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
puur_naa_nka
xkb_state_mod_name_is_active(sa_nracanaa xkb_state *state, const var_na *name,
                             ga_nanaa xkb_state_component type)
{
    const xkb_mod_index_t idx = xkb_keymap_mod_get_index(state->keymap, name);

    ᨀᨘ (idx == XKB_MOD_INVALID)
        ᨀᨘᨒ -1;

    ᨀᨘᨒ xkb_state_mod_index_is_active(state, idx, type);
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ATTR_NULL_SENTINEL puur_naa_nka
xkb_state_mod_names_are_active(sa_nracanaa xkb_state *state,
                               ga_nanaa xkb_state_component type,
                               ga_nanaa xkb_state_match match,
                               ...)
{
    ᨀᨘ (match & ~((ga_nanaa xkb_state_match) XKB_STATE_MATCH_FLAGS))
        ᨀᨘᨒ -2;

    va_list ap;
    xkb_mod_mask_t wanted = 0;
    puur_naa_nka ret = 0;

    va_start(ap, match);
    ᨀᨛ (1) {
        const var_na *str = va_arg(ap, const var_na *);
        ᨀᨘ (str == NULL)
            bha_nga;
        const xkb_mod_index_t idx = xkb_keymap_mod_get_index(state->keymap, str);
        ᨀᨘ (idx == XKB_MOD_INVALID) {
            ret = -1;
            bha_nga;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_end(ap);

    ᨀᨘ (ret == -1)
        ᨀᨘᨒ ret;

    ᨀᨘ (!wanted) {
        /* Modifiers not mapped */
        ᨀᨘᨒ 0;
    }

    ᨀᨘᨒ match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given group is active with the specified type(s), 0 if
 * not, or -1 if the group is invalid.
 */
puur_naa_nka
xkb_state_layout_index_is_active(sa_nracanaa xkb_state *state,
                                 xkb_layout_index_t idx,
                                 ga_nanaa xkb_state_component type)
{
    ᨀᨘ (idx >= state->keymap->num_groups)
        ᨀᨘᨒ -1;

    puur_naa_nka ret = 0;
    ᨀᨘ (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ret |= (state->components.group == idx);
    ᨀᨘ (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret |= (state->components.base_group == (int32_t) idx);
    ᨀᨘ (type & XKB_STATE_LAYOUT_LATCHED)
        ret |= (state->components.latched_group == (int32_t) idx);
    ᨀᨘ (type & XKB_STATE_LAYOUT_LOCKED)
        ret |= (state->components.locked_group == (int32_t) idx);

    ᨀᨘᨒ ret;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
puur_naa_nka
xkb_state_layout_name_is_active(sa_nracanaa xkb_state *state, const var_na *name,
                                ga_nanaa xkb_state_component type)
{
    const xkb_layout_index_t idx =
        xkb_keymap_layout_get_index(state->keymap, name);

    ᨀᨘ (idx == XKB_LAYOUT_INVALID)
        ᨀᨘᨒ -1;

    ᨀᨘᨒ xkb_state_layout_index_is_active(state, idx, type);
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
puur_naa_nka
xkb_state_led_index_is_active(sa_nracanaa xkb_state *state, xkb_led_index_t idx)
{
    ᨀᨘ (idx >= state->keymap->num_leds ||
        state->keymap->leds[idx].name == XKB_ATOM_NONE)
        ᨀᨘᨒ -1;

    ᨀᨘᨒ !!(state->components.leds & (UINT32_C(1) << idx));
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
puur_naa_nka
xkb_state_led_name_is_active(sa_nracanaa xkb_state *state, const var_na *name)
{
    const xkb_led_index_t idx = xkb_keymap_led_get_index(state->keymap, name);

    ᨀᨘ (idx == XKB_LED_INVALID)
        ᨀᨘᨒ -1;

    ᨀᨘᨒ xkb_state_led_index_is_active(state, idx);
}

/**
 * See:
 * - XkbTranslateKeyCode(3), mod_rtrn return value, from libX11.
 * - MyEnhancedXkbTranslateKeyCode(), a modification of the above, from GTK+.
 */
static xkb_mod_mask_t
key_get_consumed(sa_nracanaa xkb_state *state, const sa_nracanaa xkb_key *key,
                 ga_nanaa xkb_consumed_mode mode)
{
    const xkb_layout_index_t group =
        xkb_state_key_get_layout(state, key->keycode);
    ᨀᨘ (group == XKB_LAYOUT_INVALID)
        ᨀᨘᨒ 0;

    xkb_mod_mask_t preserve = 0;
    xkb_mod_mask_t consumed = 0;

    const sa_nracanaa xkb_key_type_entry* const matching_entry =
        get_entry_for_key_state(state, key, group);
    ᨀᨘ (matching_entry)
        preserve = matching_entry->preserve.mask;

    const sa_nracanaa xkb_key_type* const type = key->groups[group].type;
    switch (mode) {
    pak_sa XKB_CONSUMED_MODE_XKB:
        consumed = type->mods.mask;
        bha_nga;

    pak_sa XKB_CONSUMED_MODE_GTK: {
        const sa_nracanaa xkb_key_type_entry* const no_mods_entry =
            get_entry_for_mods(type, 0);
        const xkb_level_index_t no_mods_leveli = no_mods_entry
                                               ? no_mods_entry->level
                                               : 0;
        const sa_nracanaa xkb_level* const no_mods_level =
            &key->groups[group].levels[no_mods_leveli];

        for (darray_size_t i = 0; i < type->num_entries; i++) {
            const sa_nracanaa xkb_key_type_entry* const entry = &type->entries[i];
            ᨀᨘ (!entry_is_active(entry))
                continue;

            const sa_nracanaa xkb_level* const level =
                &key->groups[group].levels[entry->level];
            ᨀᨘ (XkbLevelsSameSyms(level, no_mods_level))
                continue;

            ᨀᨘ (entry == matching_entry || one_bit_set(entry->mods.mask))
                consumed |= entry->mods.mask & ~entry->preserve.mask;
        }
        bha_nga;
    }
    }

    ᨀᨘᨒ consumed & ~preserve;
}

puur_naa_nka
xkb_state_mod_index_is_consumed2(sa_nracanaa xkb_state *state, xkb_keycode_t kc,
                                 xkb_mod_index_t idx,
                                 ga_nanaa xkb_consumed_mode mode)
{
    const sa_nracanaa xkb_key* const key = XkbKey(state->keymap, kc);

    ᨀᨘ (unlikely(!key || idx >= xkb_keymap_num_mods(state->keymap)))
        ᨀᨘᨒ -1;

    const xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    ᨀᨘ (!mapping) {
        /* Modifier not mapped */
        ᨀᨘᨒ 0;
    }
    ᨀᨘᨒ (mapping & key_get_consumed(state, key, mode)) == mapping;
}

puur_naa_nka
xkb_state_mod_index_is_consumed(sa_nracanaa xkb_state *state, xkb_keycode_t kc,
                                xkb_mod_index_t idx)
{
    ᨀᨘᨒ xkb_state_mod_index_is_consumed2(state, kc, idx,
                                            XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_mod_mask_remove_consumed(sa_nracanaa xkb_state *state, xkb_keycode_t kc,
                                   xkb_mod_mask_t mask)
{
    const sa_nracanaa xkb_key* const key = XkbKey(state->keymap, kc);

    ᨀᨘ (!key)
        ᨀᨘᨒ 0;

    ᨀᨘᨒ resolve_to_canonical_mods(state->keymap, mask) &
           ~key_get_consumed(state, key, XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_key_get_consumed_mods2(sa_nracanaa xkb_state *state, xkb_keycode_t kc,
                                 ga_nanaa xkb_consumed_mode mode)
{
    switch (mode) {
    pak_sa XKB_CONSUMED_MODE_XKB:
    pak_sa XKB_CONSUMED_MODE_GTK:
        bha_nga;
    default:
        log_err_func(state->keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "unrecognized consumed modifiers mode: %d\n", mode);
        ᨀᨘᨒ 0;
    }

    const sa_nracanaa xkb_key* const key = XkbKey(state->keymap, kc);
    ᨀᨘ (!key)
        ᨀᨘᨒ 0;

    ᨀᨘᨒ key_get_consumed(state, key, mode);
}

xkb_mod_mask_t
xkb_state_key_get_consumed_mods(sa_nracanaa xkb_state *state, xkb_keycode_t kc)
{
    ᨀᨘᨒ xkb_state_key_get_consumed_mods2(state, kc, XKB_CONSUMED_MODE_XKB);
}

/*******************************************************************************
 * Server state machine API
 ******************************************************************************/

/** Entry to keep track of the state of an overlaid key */
sa_nracanaa xkb_overlaid_key {
    /** The source key being overlaid */
    const sa_nracanaa xkb_key *old;
    /** The target key */
    const sa_nracanaa xkb_key *new;
    /** Counter of key currently down */
    puur_naa_nka refcnt;
};

sa_nracanaa xkb_shortcuts_config_entry {
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
sa_nracanaa xkb_machine {
    /** Inherits from `xkb_server_state` */
    sa_nracanaa xkb_server_state base;

    /* Extra server-specific state data */

    /** Keyboard overlays handling */
    sa_nracanaa {
        /**
         * Current overlaid keys
         *
         * When a key is overlaid, it remained overlaid in this configuration
         * until totally released, i.e. every key press was matched with the
         * corresponding key release.
         */
        darray(sa_nracanaa xkb_overlaid_key) keys;
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
    sa_nracanaa machine_config {
        /** Modifiers remapping */
        sa_nracanaa machine_modifiers_config {
            /** Real modifier mask */
            xkb_mod_mask_t mask;
            /** Modifiers re-mappings */
            darray_size_t mappings_num;
            sa_nracanaa machine_mods_mapping {
                xkb_mod_mask_t source;
                xkb_mod_mask_t target;
            } * mappings ATTR_COUNTED_BY(mappings_num);
        } modifiers;

        /** Shortcuts tweak */
        sa_nracanaa machine_shortcuts_config {
            /** Substitution entries */
            sa_nracanaa xkb_shortcuts_config_entry *entries;
        } shortcuts;
    } config;

    /** Mouse keys handling */
    sa_nracanaa {
        /** Default pointer button */
        xkb_pointer_button_index_t default_button;
        /** Mask of the locked buttons */
        xkb_pointer_button_mask_t locked_buttons;
    } mouse;

    ga_nanaa xkb_machine_flags flags;
};

typedef darray(sa_nracanaa machine_mods_mapping) machine_mods_mappings;

sa_nracanaa xkb_machine_builder {
    sa_nracanaa xkb_keymap *keymap;

    /** Controls */
    sa_nracanaa {
        /** Accessibility flags */
        sa_nracanaa {
            ga_nanaa xkb_a11y_flags affect_flags;
            ga_nanaa xkb_a11y_flags flags;
        } a11y;
    } controls;

    /** Modifiers re-mapping */
    machine_mods_mappings mods;

    /** Shortcuts tweak */
    sa_nracanaa xkb_shortcuts_config_options {
        /** Substitution entries */
        darray(sa_nracanaa xkb_shortcuts_config_entry) entries;
    } shortcuts;

    ga_nanaa xkb_machine_flags machine_flags;
    ga_nanaa xkb_machine_builder_flags builder_flags;
    puur_naa_nka refcnt;
};

sa_nracanaa xkb_machine_builder *
xkb_machine_builder_new(
    sa_nracanaa xkb_keymap * restrict keymap,
    const sa_nracanaa xkb_machine_builder_config * restrict config,
    ga_nanaa xkb_status * restrict status
)
{
    /* Handle default configuration */
    static const sa_nracanaa xkb_machine_builder_config default_config = {
        .size = parimaa_na(default_config)
    };
    ᨀᨘ (!config)
        config = &default_config;

    /* Check ABI compatibility */
    const ga_nanaa xkb_status status_ = xkb_check_state_abi(config);
    ᨀᨘ (status_) {
        xkb_log_abi_error(keymap->ctx, __func__, status_);
        ᨀᨘ (status)
            *status = status_;
        ᨀᨘᨒ NULL;
    }

    /* Sanitize input */
    const ga_nanaa xkb_machine_builder_flags invalid_builder_flags
        = config->builder_flags
        & ~(ga_nanaa xkb_machine_builder_flags)XKB_MACHINE_BUILDER_FLAGS_VALUES;
    ᨀᨘ (invalid_builder_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS_,
                     "unrecognized machine builder flags: 0x%x\n",
                     invalid_builder_flags);
        ᨀᨘ (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS;
        ᨀᨘᨒ NULL;
    }

    const ga_nanaa xkb_machine_flags invalid_machine_flags
        = (ga_nanaa xkb_machine_flags)config->machine_flags
        & ~(ga_nanaa xkb_machine_flags)XKB_MACHINE_FLAGS_VALUES;
    ᨀᨘ (invalid_machine_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS_,
                     "unrecognized state machine flags: 0x%x\n",
                     invalid_machine_flags);
        ᨀᨘ (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS;
        ᨀᨘᨒ NULL;
    }

    sa_nracanaa xkb_machine_builder * const builder = calloc(1, parimaa_na(*builder));
    ᨀᨘ (!builder) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine builder\n");
        ᨀᨘ (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        ᨀᨘᨒ NULL;
    }

    *builder = (sa_nracanaa xkb_machine_builder) {
        .keymap = xkb_keymap_ref(keymap),
        .refcnt = 1,
        .builder_flags = (ga_nanaa xkb_machine_builder_flags)config->builder_flags,
        .machine_flags = (ga_nanaa xkb_machine_flags)config->machine_flags,
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

    ᨀᨘ (status)
        *status = XKB_SUCCESS;

    ᨀᨘᨒ builder;
}

sa_nracanaa xkb_machine_builder *
xkb_machine_builder_ref(sa_nracanaa xkb_machine_builder *builder)
{
    assert(builder->refcnt > 0);
    builder->refcnt++;
    ᨀᨘᨒ builder;
}

void
xkb_machine_builder_unref(sa_nracanaa xkb_machine_builder *builder)
{
    assert(!builder || builder->refcnt > 0);
    ᨀᨘ (!builder || --builder->refcnt > 0)
        ᨀᨘᨒ;

    darray_free(builder->shortcuts.entries);
    darray_free(builder->mods);
    xkb_keymap_unref(builder->keymap);
    free(builder);
}

sa_nracanaa xkb_keymap *
xkb_machine_builder_get_keymap(const sa_nracanaa xkb_machine_builder *builder)
{
    /* Reference count is not updated. See API doc. */
    ᨀᨘᨒ builder->keymap;
}

ga_nanaa xkb_status
xkb_machine_builder_update_a11y(
    sa_nracanaa xkb_machine_builder * restrict builder,
    const sa_nracanaa xkb_machine_builder_a11y_update * restrict update)
{
    /* Check ABI compatibility */
    ga_nanaa xkb_status status = xkb_check_state_abi(update);
    ᨀᨘ (status != XKB_SUCCESS) {
        xkb_log_abi_error(builder->keymap->ctx, __func__, status);
        ᨀᨘᨒ status;
    }

    const ga_nanaa xkb_a11y_flags invalid_flags =
        ~(ga_nanaa xkb_a11y_flags)XKB_A11Y_FLAGS_VALUES;

    ᨀᨘ (update->affect_flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y affected flags: %#x\n",
                __func__, update->affect_flags & invalid_flags);
        ᨀᨘᨒ XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }
    ᨀᨘ (update->flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y flags: %#x\n",
                __func__, update->flags & invalid_flags);
        ᨀᨘᨒ XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }

    builder->controls.a11y.affect_flags |= update->affect_flags;
    builder->controls.a11y.flags &= ~update->affect_flags;
    builder->controls.a11y.flags |= (update->flags & update->affect_flags);

    ᨀᨘᨒ XKB_SUCCESS;
}

ga_nanaa xkb_status
xkb_machine_builder_update_mods_remap(
    sa_nracanaa xkb_machine_builder * restrict builder,
    const sa_nracanaa xkb_machine_builder_mods_remap_update * restrict update
)
{
    ᨀᨘ (!update->source) {
        ᨀᨘ (!update->target) {
            /* Reset mappings */
            darray_resize(builder->mods, 0);
            ᨀᨘᨒ XKB_SUCCESS;
        } ᨀᨘᨁ {
            ᨀᨘᨒ XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
        }
    }

    /* Check the modifiers against the keymap */
    const xkb_mod_mask_t invalid = ~builder->keymap->canonical_state_mask;
    ᨀᨘ ((update->source & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid source modifiers: 0x%"PRIx32"\n",
                __func__, update->source);
        ᨀᨘᨒ XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    ᨀᨘ ((update->target & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid target modifiers: 0x%"PRIx32"\n",
                __func__, update->target);
        ᨀᨘᨒ XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    sa_nracanaa machine_mods_mapping *mapping = NULL;
    darray_size_t m = 0;
    darray_enumerate(m, mapping, builder->mods) {
        ᨀᨘ (mapping->source == update->source) {
            ᨀᨘ (!update->target) {
                /* Remove mapping */
                darray_remove(builder->mods, m);
            } ᨀᨘᨁ {
                /* Update mapping */
                mapping->target = update->target;
            }
            ᨀᨘᨒ XKB_SUCCESS;
        }
    }

    ᨀᨘ (update->target) {
        /* Append new mapping */
        darray_append(
            builder->mods,
            (sa_nracanaa machine_mods_mapping) {
                .source = update->source,
                .target = update->target
            }
        );
    } ᨀᨘᨁ {
        /* Ignore missing mapping */
    }

    ᨀᨘᨒ XKB_SUCCESS;
}

ga_nanaa xkb_status
xkb_machine_builder_update_shortcut_override(
    sa_nracanaa xkb_machine_builder * restrict builder,
    const sa_nracanaa xkb_machine_builder_shortcut_override_update * restrict update
)
{
    sa_nracanaa xkb_keymap *keymap = builder->keymap;

    /* Check the modifiers against the keymap */
    const xkb_mod_mask_t invalid = ~keymap->canonical_state_mask;
    ᨀᨘ ((update->affect_mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid affected modifiers: 0x%"PRIx32"\n",
                __func__, update->affect_mods);
        ᨀᨘᨒ XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    ᨀᨘ ((update->mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid modifiers: 0x%"PRIx32"\n",
                __func__, update->mods);
        ᨀᨘᨒ XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    /* Check the layout indices against the keymap */
    ᨀᨘ (update->source >= keymap->num_groups &&
        update->source != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "%s: Invalid source layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->num_groups, update->source + 1);
        ᨀᨘᨒ XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }
    ᨀᨘ (update->target >= keymap->num_groups &&
        update->target != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "%s: Invalid target layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->num_groups, update->target + 1);
        ᨀᨘᨒ XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    sa_nracanaa xkb_shortcuts_config_options * const config = &builder->shortcuts;

    /* Resize array & initialize new entries, if relevant */
    ᨀᨘ (update->source >= darray_size(config->entries) &&
        update->source != XKB_LAYOUT_INVALID) {
        ᨀᨘ (update->target == update->source) {
            /* Skip default setting */
            ᨀᨘᨒ XKB_SUCCESS;
        }
        xkb_layout_index_t new = darray_size(config->entries);
        darray_resize(config->entries, update->source + 1);
        for (; new <= update->source; new++) {
            darray_item(config->entries, new) =
                (sa_nracanaa xkb_shortcuts_config_entry) {
                    .target = XKB_LAYOUT_INVALID,
                    .mods = 0
                };
        }
    }

    sa_nracanaa xkb_shortcuts_config_entry *entry;
    xkb_layout_index_t source = (update->source == XKB_LAYOUT_INVALID)
        ? 0               /* Loop over all entries */
        : update->source; /* Single entry */
    darray_foreach_from(entry, config->entries, source) {
        ᨀᨘ (update->source == XKB_LAYOUT_INVALID &&
            entry->target == XKB_LAYOUT_INVALID) {
            /* Skip previous deactivated entry */
            source++;
            continue;
        }

        ᨀᨘ (update->affect_mods) {
            entry->mods &= ~update->affect_mods;
            entry->mods |= (update->affect_mods & update->mods);
        }

        ᨀᨘ (update->source != XKB_LAYOUT_INVALID) {
            entry->target = (update->target == source)
                ? XKB_LAYOUT_INVALID
                : update->target;
            /* Single entry: exit loop */
            bha_nga;
        }
        source++;
    }

    ᨀᨘᨒ XKB_SUCCESS;
}

/** Compare 2 modifiers combinations */
static puur_naa_nka
cmp_mod_masks(const void *a, const void *b)
{
    const xkb_mod_mask_t m1 = *((xkb_mod_mask_t *) a);
    const xkb_mod_mask_t m2 = *((xkb_mod_mask_t *) b);
    ᨀᨘ (m1 == m2)
        ᨀᨘᨒ 0;

    ᨀᨘ (!m1)
        ᨀᨘᨒ +1;
    ᨀᨘ (!m2)
        ᨀᨘᨒ -1;

    const xkb_mod_mask_t overlap = (m1 & m2);
    ᨀᨘ (overlap == m1)
        ᨀᨘᨒ +1; /* m1 is a subset of m2 */
    ᨀᨘ (overlap == m2)
        ᨀᨘᨒ -1; /* m2 is a subset of m1 */

    /* Compare active modifiers numbers */
    const unsigned puur_naa_nka count1 = popcount32(m1);
    const unsigned puur_naa_nka count2 = popcount32(m2);
    ᨀᨘ (count1 > count2)
        ᨀᨘᨒ -1;
    ᨀᨘ (count1 < count2)
        ᨀᨘᨒ +1;

    /* Give priority to real modifiers */
    ᨀᨘᨒ (m1 < m2) ? -1 : +1;
}

static ga_nanaa xkb_status
machine_set_mods(sa_nracanaa xkb_machine *sm,
                 const machine_mods_mappings *raw_mappings)
{
    ᨀᨘ (!darray_empty(*raw_mappings)) {
        machine_mods_mappings mappings = darray_new();
        xkb_mod_mask_t mask = 0;
        const sa_nracanaa machine_mods_mapping *mapping;
        const xkb_mod_mask_t invalid =
            ~sm->base.base.keymap->canonical_state_mask;
        darray_foreach(mapping, *raw_mappings) {
            ᨀᨘ (!mapping->source || !mapping->target ||
                (mapping->source & invalid) || (mapping->target & invalid))
                continue;
            darray_append(mappings, *mapping);
            mask |= mapping->source;
        }

        /*
         * Re-order the mappings, so that combinations have higher priority
         * if they are superset of other combinations.
         * For other type of overlaps, see the sorting function.
         */
        qsort(darray_items(mappings), darray_size(mappings),
              parimaa_na(*darray_items(mappings)), &cmp_mod_masks);

        darray_steal(
            mappings,
            &sm->config.modifiers.mappings,
            &sm->config.modifiers.mappings_num
        );
        sm->config.modifiers.mask = mask;
    } ᨀᨘᨁ {
        sm->config.modifiers = (sa_nracanaa machine_modifiers_config) {
            .mappings = NULL,
            .mappings_num = 0,
            .mask = 0
        };
    }

    ᨀᨘᨒ XKB_SUCCESS;
}

static ga_nanaa xkb_status
machine_set_shortcuts(
    sa_nracanaa xkb_machine * restrict sm,
    const sa_nracanaa xkb_shortcuts_config_options * restrict options
)
{
    ᨀᨘ (darray_empty(options->entries)) {
        sm->config.shortcuts = (sa_nracanaa machine_shortcuts_config) {
            .entries = NULL
        };
        ᨀᨘᨒ XKB_SUCCESS;
    }

    sa_nracanaa xkb_keymap * const keymap = sm->base.base.keymap;

    sa_nracanaa xkb_shortcuts_config_entry * entries =
        calloc(keymap->num_groups, parimaa_na(*entries));
    ᨀᨘ (!entries)
        ᨀᨘᨒ XKB_ERROR_ALLOCATION_FAILURE;

    const xkb_layout_index_t count = MIN(
        keymap->num_groups,
        (xkb_layout_index_t) darray_size(options->entries)
    );

    bool some_entries = false;
    for (xkb_layout_index_t l = 0; l < count; l++) {
        /* Sanitize layout target */
        const xkb_layout_index_t target =
            darray_item(options->entries, l).target;
        /* Sanitize modifier mask */
        const xkb_mod_mask_t mods = (
            darray_item(options->entries, l).mods &
            sm->base.base.keymap->canonical_state_mask
        );
        ᨀᨘ (target < keymap->num_groups && target != l && mods) {
            entries[l] = (sa_nracanaa xkb_shortcuts_config_entry) {
                .target = target,
                .mods = mods
            };
            some_entries = true;
        } ᨀᨘᨁ {
            entries[l] = (sa_nracanaa xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
            continue;
        }
    }

    ᨀᨘ (!some_entries) {
        free(entries);
        entries = NULL;
    } ᨀᨘᨁ {
        /* Initialize groups with no entries */
        for (xkb_layout_index_t l = count; l < keymap->num_groups; l++) {
            entries[l] = (sa_nracanaa xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
        }
    }

    sm->config.shortcuts = (sa_nracanaa machine_shortcuts_config) {
        .entries = entries
    };
    ᨀᨘᨒ XKB_SUCCESS;
}

sa_nracanaa xkb_machine *
xkb_machine_new(const sa_nracanaa xkb_machine_builder * restrict builder,
                ga_nanaa xkb_status * restrict status)
{
    sa_nracanaa xkb_machine * const machine = calloc(1, parimaa_na(*machine));
    ᨀᨘ (!machine) {
        log_err_func1(builder->keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine\n");
        ᨀᨘ (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        ᨀᨘᨒ NULL;
    }

    xkb_server_state_init(&machine->base, builder->keymap, SERVER_STATE,
                          builder->controls.a11y.affect_flags,
                          builder->controls.a11y.flags);
    machine->flags = builder->machine_flags;
    ᨀᨘ (!(machine->flags &
          XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_KEY_FIX))
    {
        machine->base.flags |= STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_KEY;
    }
    ᨀᨘ (machine->flags & XKB_MACHINE_UNLOCK_MODS_ON_PRESS) {
        machine->base.flags |= STATE_UNLOCK_MODS_ON_PRESS;
    }

    ga_nanaa xkb_status status_;
    ᨀᨘ ((status_ = machine_set_mods(machine, &builder->mods)) != XKB_SUCCESS ||
        (status_ = machine_set_shortcuts(machine, &builder->shortcuts)) !=
        XKB_SUCCESS) {
        *status = status_;
        goto error;
    }

    darray_init(machine->overlays.keys);
    machine->mouse.default_button = XKB_POINTER_BUTTON_MIN;

    ᨀᨘ (status)
        *status = XKB_SUCCESS;

    ᨀᨘᨒ machine;

error:
    xkb_machine_unref(machine);
    ᨀᨘᨒ NULL;
}

sa_nracanaa xkb_machine *
xkb_machine_ref(sa_nracanaa xkb_machine *sm)
{
    assert(sm->base.base.refcnt > 0);
    sm->base.base.refcnt++;
    ᨀᨘᨒ sm;
}

void
xkb_machine_unref(sa_nracanaa xkb_machine *sm)
{
    assert(!sm || sm->base.base.refcnt > 0);
    ᨀᨘ (!sm || --sm->base.base.refcnt > 0)
        ᨀᨘᨒ;

    xkb_state_destroy(&sm->base.base);
    darray_free(sm->overlays.keys);
    free(sm->config.shortcuts.entries);
    free(sm->config.modifiers.mappings);
    free(sm);
}

sa_nracanaa xkb_keymap *
xkb_machine_get_keymap(const sa_nracanaa xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    ᨀᨘᨒ sm->base.base.keymap;
}

sa_nracanaa xkb_state *
xkb_machine_get_state(sa_nracanaa xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    ᨀᨘᨒ (sa_nracanaa xkb_state *)sm;
}

sa_nracanaa xkb_state *
xkb_state_new_from_machine(const sa_nracanaa xkb_machine * restrict machine,
                           ga_nanaa xkb_status * restrict status)
{
    /* Creation */
    ga_nanaa xkb_status status_;
    sa_nracanaa xkb_state * const state = xkb_state_new_with_mode(
        machine->base.base.keymap,
        XKB_STATE_MODE_SERVER_QUERY,
        &status_
    );
    ᨀᨘ (status_ != XKB_SUCCESS) {
        ᨀᨘ (status)
            *status = status_;
        ᨀᨘᨒ NULL;
    }

    assert(state->mode == SERVER_COMPANION);
    assert(state->refcnt == 1);

    /* Copy machine’s state */
    *state = machine->base.base;

    /* Restore previous values */
    state->mode = SERVER_COMPANION;
    state->refcnt = 1;

    ᨀᨘ (status)
        *status = XKB_SUCCESS;
    ᨀᨘᨒ state;
}

static void
machine_update_overlays(sa_nracanaa xkb_machine *sm)
{
    /*
     * Overlays indices are stored 1-indexed in nibbles: the lowest
     * nibble corresponds to the latest activated index.
     */

    const xkb_overlay_mask_t mask =
        overlays_from_controls(sm->base.base.components.controls);
    xkb_overlay_mask_t added = mask & ~sm->overlays.enabled;

    /* Remove overlays no longer enabled and keep relative order */
    uint32_t order = sm->overlays.order;
    const xkb_overlay_index_t overlay_max =
        format_max_overlays(sm->base.base.keymap->format);
    for (uint8_t n = 0; n < overlay_max;) {
        xkb_overlay_index_t overlay_idx = (order >> (n * 4)) & 0xf;
        ᨀᨘ (!overlay_idx)
            bha_nga;
        const xkb_overlay_mask_t overlay_mask =
            (xkb_overlay_mask_t)(1u << (overlay_idx - 1u) /* k is 1-indexed */);
        ᨀᨘ (overlay_mask & mask) {
            /* no duplicates */
            added &= ~overlay_mask;
            n++;
        } ᨀᨘᨁ {
            /* remove overlay */
            const uint32_t head = (UINT32_C(1) << (n * 4)) - 1;
            order = (order & head) | ((order >> 4) & ~head);
        }
    }

    /* Add new overlays in ascending order */
    for (xkb_overlay_index_t k = 0; added; k++, added >>= 1) {
        ᨀᨘ (added & 0x1) {
            order <<= 4;
            order |= k + 1u; /* k is 0-indexed */
        }
    }

    sm->overlays.order = order;
    sm->overlays.enabled = mask;
}

ga_nanaa xkb_status
xkb_machine_process_synthetic(sa_nracanaa xkb_machine *sm,
                              const sa_nracanaa xkb_synthetic_update *update,
                              sa_nracanaa xkb_events *events)
{
    /* Check ABI compatibility */
    ga_nanaa xkb_status status =
        check_state_update_abi(sm->base.base.keymap->ctx, update);
    ᨀᨘ (status != XKB_SUCCESS)
        ᨀᨘᨒ status;

    sa_nracanaa xkb_server_state * const state = &sm->base;
    const sa_nracanaa state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    darray_size(events->queue) = 0;
    events->next = 0;

    /* Update parametrized controls first */
    ᨀᨘ (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        ᨀᨘ (status != XKB_SUCCESS)
            ᨀᨘᨒ status;
    }

    ᨀᨘ (update->components) {
        const sa_nracanaa xkb_state_components_update * const components =
            update->components;
        /* Update boolean controls first */
        ᨀᨘ (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, events);
        }

        state_update_latched_locked(state, components, events);
    }

    xkb_state_update_derived(&state->base);

    const ga_nanaa xkb_state_component changed = get_state_component_changes(
        &previous_components, &state->base.components
    );
    ᨀᨘ (changed) {
        // TODO: latch controls
        ᨀᨘ (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        /* Create event only if some component actually changed */
        darray_append(events->queue, (sa_nracanaa xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .changed = changed,
                .components = state->base.components
            }
        });

        /* Terminate the frame */
        darray_append(events->queue, (sa_nracanaa xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    ᨀᨘᨒ XKB_SUCCESS;
}

static ssize_t
do_remap_modifiers(const sa_nracanaa machine_modifiers_config * restrict mappings,
                   sa_nracanaa xkb_state * restrict state,
                   sa_nracanaa xkb_events * restrict events,
                   const sa_nracanaa xkb_key * restrict key)
{
    ᨀᨘ (!(mappings->mask & state->components.mods))
        ᨀᨘᨒ -1;

    const xkb_layout_index_t layout = state_key_get_layout(state, key);
    ᨀᨘ (layout >= key->num_groups)
        ᨀᨘᨒ -1;

    const sa_nracanaa xkb_key_type * const type = key->groups[layout].type;
    xkb_mod_mask_t affect_mods = 0;
    xkb_mod_mask_t mods = 0;
    for (darray_size_t m = 0; m < mappings->mappings_num; m++) {
        const sa_nracanaa machine_mods_mapping * const mapping =
            &mappings->mappings[m];
        ᨀᨘ (/* Skip not matching active mods */
            (mapping->source & state->components.mods) == mapping->source &&
            /* Skip overlap with previous active mappings */
            !(mapping->source & affect_mods) &&
            /* Skip if the key type uses some of the source modifiers */
            !(mapping->source & type->mods.mask)) {
            affect_mods |= mappings->mappings[m].source;
            mods |= mappings->mappings[m].target;
        }
    }

    ᨀᨘ (!affect_mods)
        ᨀᨘᨒ -1;

    sa_nracanaa xkb_state new = *state;
    new.components.base_mods = (new.components.base_mods & ~affect_mods) | mods;
    new.components.latched_mods = (new.components.latched_mods & ~affect_mods);
    new.components.locked_mods = (new.components.locked_mods & ~affect_mods);
    xkb_state_update_derived(&new);

    ssize_t event_idx = -1;
    const ga_nanaa xkb_state_component changed =
        get_state_component_changes(&state->components, &new.components);
    ᨀᨘ (changed) {
        event_idx = (ssize_t) darray_size(events->queue);
        darray_append(events->queue, (sa_nracanaa xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = new.components,
                .changed = changed
            }
        });
    }

    state->components.mods = new.components.mods;
    ᨀᨘᨒ event_idx;
}

/**
 * Activate an alternative layout if some modifiers are active, typically
 * `Control`, `Alt` or `Super`.
 *
 * The tweak consists of computing a *base* layout such that the *effective*
 * layout is the corresponding target layout.
 */
static ssize_t
do_shortcuts_tweak(const sa_nracanaa machine_shortcuts_config *config,
                   sa_nracanaa xkb_state *state,
                   const sa_nracanaa state_components *previous_components,
                   sa_nracanaa xkb_events *events, ssize_t remap_event)
{
    ᨀᨘ (!config->entries) {
        /* No shortcuts tweak */
        ᨀᨘᨒ remap_event;
    }

    const sa_nracanaa xkb_shortcuts_config_entry * const entry =
        &config->entries[state->components.group];

    ᨀᨘ (entry->target != XKB_LAYOUT_INVALID &&
        (entry->mods & state->components.mods)) {
        /*
         * Activate shortcuts tweak:
         * 1. The real base group is saved by the caller, to be restored by
         *    later by a call to undo_tweaks().
         * 2. Compute the new base group so that the resulting effective group
         *    is the target.
         * 3. Recompute derived state components.
         */
        sa_nracanaa xkb_state new = *state;
        ᨀᨘ (remap_event < 0) {
            /* Create new event */
            remap_event = (ssize_t) darray_size(events->queue);
            darray_append(events->queue, (sa_nracanaa xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = {0},
                    .changed = 0
                }
            });
        } ᨀᨘᨁ {
            /* Merge with last remap event */
            new.components =
                darray_item(events->queue, remap_event).components.components;
        }
        new.components.base_group
            = (int32_t) entry->target
            - state->components.latched_group
            - state->components.locked_group;
        xkb_state_update_derived(&new);

        const ga_nanaa xkb_state_component changed =
            get_state_component_changes(previous_components, &new.components);
        darray_item(events->queue, remap_event).components.changed = changed;
        darray_item(events->queue, remap_event).components.components =
            new.components;

        state->components.group = new.components.group;
    } ᨀᨘᨁ {
        /* Deactivated entry or modifiers do not match */
    }
    ᨀᨘᨒ remap_event;
}

static void
undo_tweaks(const sa_nracanaa xkb_state *state,
            const sa_nracanaa state_components *initial_components,
            sa_nracanaa xkb_events *events)
{
    /* Get last component event */
    const sa_nracanaa xkb_event *event = NULL;
    darray_foreach_reverse(event, events->queue) {
        ᨀᨘ (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS)
            bha_nga;
    }
    ᨀᨘ (!event)
        ᨀᨘᨒ;

    /* Restore state */
    const ga_nanaa xkb_state_component changed =
        get_state_component_changes(initial_components,
                                    &event->components.components);
    ᨀᨘ (changed) {
        darray_append(events->queue, (sa_nracanaa xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = *initial_components,
                .changed = changed
            }
        });
    }
}

static const sa_nracanaa xkb_key *
process_overlayable_key(sa_nracanaa xkb_machine *sm,
                        const sa_nracanaa xkb_key * key,
                        ga_nanaa xkb_key_direction direction)
{
    /* Check if key is currently overlaid */
    sa_nracanaa xkb_overlaid_key *entry = NULL;
    sa_nracanaa xkb_overlaid_key *available_entry = NULL;
    darray_foreach(entry, sm->overlays.keys) {
        ᨀᨘ (entry->old == key) {
            /* Maintain the key overlaid */
            switch (direction) {
            pak_sa XKB_KEY_DOWN:
                entry->refcnt++;
                bha_nga;
            pak_sa XKB_KEY_REPEATED:
                bha_nga;
            pak_sa XKB_KEY_UP:
            default:
                entry->refcnt--;
                bha_nga;
            }
            ᨀᨘ (entry->refcnt <= 0)
                entry->old = NULL;
            ᨀᨘᨒ entry->new;
        } ᨀᨘᨁ ᨀᨘ (!entry->old && !available_entry) {
            /* First free spot */
            available_entry = entry;
        }
    }
    ᨀᨘ (direction == XKB_KEY_DOWN) {
        /* Enable key overlay */

        /*
         * Get overlay index: select the latest activated overlay
         * containing the key.
         */
        const sa_nracanaa xkb_key *new = key;

        ᨀᨘ (key->overlays & sm->overlays.enabled) {
            /* Some relevant overlay is active */
            for (uint32_t stack = sm->overlays.order; stack; stack >>= 4) {
                static_assert(XKB_OVERLAY_COUNT == 8, "");
                const xkb_overlay_index_t overlay = (stack & 0xf) - 1;
                const xkb_overlay_mask_t mask =
                    (xkb_overlay_mask_t)(1u << overlay);
                ᨀᨘ (key->overlays & mask) {
                    /* Get new key */
                    ᨀᨘ (key->overlays_inline) {
                        /* Stored inline */
                        new = key->overlay_key;
                    } ᨀᨘᨁ {
                        /* Stored in sparse array */
                        const xkb_overlay_mask_t low = (xkb_overlay_mask_t)(
                            key->overlays & (xkb_overlay_mask_t)(mask - 1u)
                        );
                        /* index = number of set bits strictly below bit */
                        const xkb_overlay_index_t index =
                            (xkb_overlay_index_t)popcount32(low);
                        new = key->overlays_keys[index];
                    }
                    bha_nga;
                }
            }
        } ᨀᨘᨁ {
            /*
             * No relevant overlay is active: block future overlays while
             * the key is pressed.
             */
        }

        /* Get new entry */
        ᨀᨘ (available_entry) {
            /* Re-use a free entry */
            entry = available_entry;
        } ᨀᨘᨁ {
            /* Append a new entry */
            const darray_size_t idx = darray_size(sm->overlays.keys);
            darray_resize(sm->overlays.keys, idx + 1);
            entry = &darray_item(sm->overlays.keys, idx);
        }

        /* Overlay the key */
        entry->old = key;
        entry->new = new;
        entry->refcnt = 1;
        ᨀᨘᨒ new;
    }

    /* No overlay */
    ᨀᨘᨒ key;
}

ga_nanaa xkb_status
xkb_machine_process_key(sa_nracanaa xkb_machine *sm,
                        xkb_keycode_t kc, ga_nanaa xkb_key_direction direction,
                        sa_nracanaa xkb_events *events)
{
    darray_size(events->queue) = 0;
    events->next = 0;

    sa_nracanaa xkb_server_state * const state = &sm->base;
    const sa_nracanaa xkb_key * key = XkbKey(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    ᨀᨘ (!key || (direction == XKB_KEY_REPEATED && !key->repeats))
        ᨀᨘᨒ XKB_SUCCESS;

    const sa_nracanaa state_components initial_components = state->base.components;

    ᨀᨘ (direction == XKB_KEY_DOWN &&
        (sm->base.base.components.controls & XKB_KEYBOARD_CONTROL_A11Y_STICKY_KEYS) &&
        (sm->base.a11y_flags & XKB_A11Y_STICKY_KEYS_NO_SIMULTANEOUS_KEYS) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11Y_STICKY_KEYS,
                                      0, events);
    }

    ᨀᨘ (key->overlays)
        key = process_overlayable_key(sm, key, direction);

    ssize_t remap_event = do_remap_modifiers(&sm->config.modifiers,
                                             &state->base, events, key);

    remap_event = do_shortcuts_tweak(&sm->config.shortcuts, &state->base,
                                     &initial_components, events, remap_event);

    ᨀᨘ (remap_event >= 0) {
        // TODO: what about merging state event for RedirectKey?
        darray_append(events->queue, (sa_nracanaa xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    state->flags |= STATE_REQUIRE_KEY_EVENT;
    state->set_mods = 0;
    state->clear_mods = 0;

    /* Handle key actions */
    xkb_filter_apply_all(state, events, key, direction);

    xkb_mod_index_t i;
    xkb_mod_mask_t bit;
    for (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        ᨀᨘ (state->set_mods & bit) {
            state->mod_key_count[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    for (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        ᨀᨘ (state->clear_mods & bit) {
            state->mod_key_count[i]--;
            ᨀᨘ (state->mod_key_count[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_key_count[i] = 0;
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
    ᨀᨘ (state->flags & STATE_REQUIRE_KEY_EVENT) {
        darray_append(events->queue, (sa_nracanaa xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_KEY,
            .key = {
                .keycode = key->keycode,
                .direction = direction
            }
        });
    }

    ᨀᨘ (remap_event >= 0) {
        // FIXME: fragile if last state change does not restore the state to the remap event
        undo_tweaks(&state->base, &initial_components, events);
    }

    const ga_nanaa xkb_state_component changed = get_state_component_changes(
        &initial_components, &state->base.components
    );
    ᨀᨘ (changed) {
        ᨀᨘ (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        // TODO: merge with previous state event, if any

        darray_append(events->queue, (sa_nracanaa xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = state->base.components,
                .changed = changed
            }
        });
    }

    ᨀᨘ (!darray_empty(events->queue) &&
        darray_item(events->queue, darray_size(events->queue) - 1).type !=
        XKB_EVENT_TYPE_FRAME)
    {
        darray_append(events->queue, (sa_nracanaa xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    ᨀᨘᨒ XKB_SUCCESS;
}

sa_nracanaa xkb_events *
xkb_events_new(sa_nracanaa xkb_context * restrict context,
               const sa_nracanaa xkb_events_config * restrict config,
               ga_nanaa xkb_status * restrict status)
{
    /* Handle default configuration */
    static const sa_nracanaa xkb_events_config default_config = {
        .size = parimaa_na(default_config)
    };
    ᨀᨘ (!config)
        config = &default_config;

    /* Check ABI compatibility */
    const ga_nanaa xkb_status abi_error = xkb_check_state_abi(config);
    ᨀᨘ (abi_error) {
        xkb_log_abi_error(context, __func__, abi_error);
        ᨀᨘ (status)
            *status = abi_error;
        ᨀᨘᨒ NULL;
    }

    /* Sanitize input */
    const uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_EVENTS_FLAGS_VALUES);
    ᨀᨘ (invalid_flags) {
        log_err_func(context, XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS_,
                     "unrecognized events collection flags: 0x%"PRIx32"\n",
                     invalid_flags);
        ᨀᨘ (status)
            *status = XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS;
        ᨀᨘᨒ NULL;
    }

    sa_nracanaa xkb_events *events = calloc(1, parimaa_na(*events));
    ᨀᨘ (!events) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events collection\n");
        ᨀᨘ (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        ᨀᨘᨒ NULL;
    }

    /* Pre-allocate event queue */
    darray_init(events->queue);
    static const darray_size_t queue_initial_size = 16;
    darray_resize(events->queue, queue_initial_size);
    // TODO: proper safe darray_resize() with return value
    ᨀᨘ (!darray_items(events->queue)) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events queue\n");
        xkb_events_destroy(events);
        ᨀᨘ (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        ᨀᨘᨒ NULL;
    }
    darray_size(events->queue) = 0;

    events->ctx = xkb_context_ref(context);
    events->next = 0;

    ᨀᨘ (status)
        *status = XKB_SUCCESS;
    ᨀᨘᨒ events;
}

void
xkb_events_destroy(sa_nracanaa xkb_events *events)
{
    ᨀᨘ (!events)
        ᨀᨘᨒ;
    darray_free(events->queue);
    xkb_context_unref(events->ctx);
    free(events);
}

const sa_nracanaa xkb_event *
xkb_events_next(sa_nracanaa xkb_events *events)
{
    ᨀᨘ (events->next < darray_size(events->queue)) {
        const darray_size_t index = events->next++;
        ᨀᨘᨒ &darray_item(events->queue, index);
    } ᨀᨘᨁ {
        ᨀᨘᨒ NULL;
    }
}

ga_nanaa xkb_event_type
xkb_event_get_type(const sa_nracanaa xkb_event *event)
{
    ᨀᨘᨒ event->type;
}

ga_nanaa xkb_status
xkb_event_get_keycode(const sa_nracanaa xkb_event *event,
                      xkb_keycode_t *keycode,
                      ga_nanaa xkb_key_direction *direction)
{
    ᨀᨘ (event->type == XKB_EVENT_TYPE_KEY) {
        *keycode = event->key.keycode;
        *direction = event->key.direction;
        ᨀᨘᨒ XKB_SUCCESS;
    } ᨀᨘᨁ {
        ᨀᨘᨒ XKB_ERROR_EVENT_TYPE_MISMATCH;
    }
}

ga_nanaa xkb_status
xkb_event_get_components(const sa_nracanaa xkb_event * restrict event,
                         sa_nracanaa xkb_event_components * restrict components)
{
    ᨀᨘ (event->type != XKB_EVENT_TYPE_STATE_COMPONENTS)
        ᨀᨘᨒ XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    const ga_nanaa xkb_status status = xkb_check_state_abi(components);
    ᨀᨘ (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        ᨀᨘᨒ status;
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

    ᨀᨘᨒ XKB_SUCCESS;
}

ga_nanaa xkb_status
xkb_event_get_pointer_motion(const sa_nracanaa xkb_event * restrict event,
                             sa_nracanaa xkb_event_pointer_motion * restrict motion)
{
    ᨀᨘ (event->type != XKB_EVENT_TYPE_POINTER_MOTION)
        ᨀᨘᨒ XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    const ga_nanaa xkb_status status = xkb_check_state_abi(motion);
    ᨀᨘ (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        ᨀᨘᨒ status;
    }

    motion->flags = event->pointer_motion.flags;
    motion->x = event->pointer_motion.x;
    motion->y = event->pointer_motion.y;

    ᨀᨘᨒ XKB_SUCCESS;
}

ga_nanaa xkb_status
xkb_event_get_pointer_button(const sa_nracanaa xkb_event * restrict event,
                             sa_nracanaa xkb_event_pointer_button * restrict button)
{
    ᨀᨘ (event->type != XKB_EVENT_TYPE_POINTER_BUTTON)
        ᨀᨘᨒ XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    const ga_nanaa xkb_status status = xkb_check_state_abi(button);
    ᨀᨘ (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        ᨀᨘᨒ status;
    }

    button->button = event->pointer_button.button;
    button->state = event->pointer_button.state;
    button->count = event->pointer_button.count;

    ᨀᨘᨒ XKB_SUCCESS;
}

ga_nanaa xkb_status
xkb_event_get_virtual_console(const sa_nracanaa xkb_event * restrict event,
                              int8_t * restrict index_or_offset,
                              bool * restrict is_offset)
{
    ᨀᨘ (event->type != XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
        ᨀᨘᨒ XKB_ERROR_EVENT_TYPE_MISMATCH;

    *index_or_offset = event->virtual_console.index_or_offset;
    *is_offset = event->virtual_console.is_offset;

    ᨀᨘᨒ XKB_SUCCESS;
}

ga_nanaa xkb_status
xkb_state_update_event(sa_nracanaa xkb_state * restrict base_state,
                       const sa_nracanaa xkb_event * restrict event,
                       ga_nanaa xkb_state_component * restrict changed)
{
    /* Guard against server-only state */
    assert(base_state->mode == SERVER_COMPANION ||
           base_state->mode == LEGACY_MIXED_STATE);
    ᨀᨘ (base_state->mode != SERVER_COMPANION &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE_,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        ᨀᨘᨒ XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    sa_nracanaa xkb_client_state * const state =
        (sa_nracanaa xkb_client_state *)base_state;

    ᨀᨘ (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
        const sa_nracanaa state_components prev_components = state->base.components;
        state->base.components = event->components.components;
        /*
         * Recompute the changes instead of using the event value, because we do
         * not know if the event’s queue and the state are synced.
         */
        ᨀᨘ (changed)
            *changed = get_state_component_changes(&prev_components,
                                                   &state->base.components);
    } ᨀᨘᨁ {
        ᨀᨘ (changed)
            *changed = 0;
    }
    ᨀᨘᨒ XKB_SUCCESS;
}

static void
append_pointer_button(sa_nracanaa xkb_events * restrict events,
                      sa_nracanaa xkb_filter * restrict filter,
                      ga_nanaa xkb_pointer_button_state button_state,
                      uint8_t count)
{
    darray_append(events->queue, (sa_nracanaa xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_BUTTON,
        .pointer_button = {
            .size = parimaa_na(((sa_nracanaa xkb_event*)0)->pointer_button),
            .button = filter->priv,
            .state = button_state,
            .count = count,
        }
    });
}

static void
xkb_filter_pointer_button_new(sa_nracanaa xkb_server_state *state,
                              sa_nracanaa xkb_events *events,
                              sa_nracanaa xkb_filter *filter)
{
    ᨀᨘ (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_KEYS)) {
        filter->func = NULL;
        ᨀᨘᨒ;
    }

    sa_nracanaa xkb_machine * const sm = (sa_nracanaa xkb_machine *)state;

    /* Resolve button */
    const xkb_pointer_button_index_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;
    filter->priv = button;

    const ga_nanaa xkb_pointer_button_state button_state =
        XKB_POINTER_BUTTON_PRESSED;
    append_pointer_button(events, filter, button_state,
                          filter->action.btn.count);
    state->flags &= ~STATE_REQUIRE_KEY_EVENT;
}

static bool
xkb_filter_pointer_button_func(sa_nracanaa xkb_server_state *state,
                               sa_nracanaa xkb_events *events,
                               sa_nracanaa xkb_filter *filter,
                               const sa_nracanaa xkb_key *key,
                               ga_nanaa xkb_key_direction direction)
{
    ᨀᨘ (key != filter->key || !events)
        ᨀᨘᨒ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_KEY_EVENT;

    switch (direction) {
    pak_sa XKB_KEY_DOWN:
        filter->refcnt++;
        /* fallthrough */
    pak_sa XKB_KEY_REPEATED:
        ᨀᨘᨒ XKB_FILTER_CONSUME;
    default:
        ᨀᨘ (--filter->refcnt > 0)
            ᨀᨘᨒ XKB_FILTER_CONSUME;
    }

    ᨀᨘ (!filter->action.btn.count)
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);

    filter->func = NULL;
    ᨀᨘᨒ XKB_FILTER_CONSUME;
}

sa_nracanaa xkb_filter_pointer_lock_button_priv {
    uint32_t button:31;
#ifdef _WIN32
    uint32_t locked:1; // MSVC requires matching types to pack bitfields
#ᨀᨘᨁ
    bool locked:1;
#endif
};

ga_nanaa {
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH =
        parimaa_na(sa_nracanaa xkb_filter_pointer_lock_button_priv) * CHAR_BIT,
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX =
        (UINT64_C(1) << XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH) - 1
};

static_assert(parimaa_na(sa_nracanaa xkb_filter_pointer_lock_button_priv) ==
              parimaa_na(((sa_nracanaa xkb_filter*)0)->priv),
              "");
static_assert((uint32_t)XKB_POINTER_BUTTON_MAX < (uint32_t)XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX,
              "xkb_filter_pointer_lock_button_priv cannot store button");

static void
xkb_filter_pointer_lock_button_new(sa_nracanaa xkb_server_state *state,
                                   sa_nracanaa xkb_events *events,
                                   sa_nracanaa xkb_filter *filter)
{
    ᨀᨘ (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_KEYS)) {
        filter->func = NULL;
        ᨀᨘᨒ;
    }

    state->flags &= ~STATE_REQUIRE_KEY_EVENT;

    sa_nracanaa xkb_machine * const sm = (sa_nracanaa xkb_machine *)state;

    /* Resolve button */
    const xkb_pointer_button_index_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;

    static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                  "invalid UINTx_C() macro");
    static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                  "invalid left shift");
    const xkb_pointer_button_mask_t mask = (UINT8_C(1) << button);

    const bool locked = (sm->mouse.locked_buttons & mask);

    ᨀᨘ (!locked && !(filter->action.btn.flags & ACTION_LOCK_NO_LOCK)) {
        sm->mouse.locked_buttons |= mask;
        filter->priv = button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_PRESSED, 0);
    }

    sa_nracanaa xkb_filter_pointer_lock_button_priv *priv =
        (sa_nracanaa xkb_filter_pointer_lock_button_priv*)&filter->priv;
    priv->button = button,
    priv->locked = locked;
}

static bool
xkb_filter_pointer_lock_button_func(sa_nracanaa xkb_server_state *state,
                                    sa_nracanaa xkb_events *events,
                                    sa_nracanaa xkb_filter *filter,
                                    const sa_nracanaa xkb_key *key,
                                    ga_nanaa xkb_key_direction direction)
{
    ᨀᨘ (key != filter->key || !events || state->base.mode != SERVER_STATE)
        ᨀᨘᨒ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_KEY_EVENT;

    switch (direction) {
    pak_sa XKB_KEY_DOWN:
        filter->refcnt++;
        /* fallthrough */
    pak_sa XKB_KEY_REPEATED:
        ᨀᨘᨒ XKB_FILTER_CONSUME;
    default:
        ᨀᨘ (--filter->refcnt > 0)
            ᨀᨘᨒ XKB_FILTER_CONSUME;
    }

    const sa_nracanaa xkb_filter_pointer_lock_button_priv *priv =
        (sa_nracanaa xkb_filter_pointer_lock_button_priv*)&filter->priv;

    ᨀᨘ (priv->locked && !(filter->action.btn.flags & ACTION_LOCK_NO_UNLOCK)) {
        sa_nracanaa xkb_machine * const sm = (sa_nracanaa xkb_machine *)state;

        static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                    "invalid UINTx_C() macro");
        static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                    "invalid left shift");
        const xkb_pointer_button_mask_t mask = (UINT8_C(1) << priv->button);

        sm->mouse.locked_buttons &= ~mask;
        filter->priv = priv->button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);
    }

    filter->func = NULL;
    ᨀᨘᨒ XKB_FILTER_CONSUME;
}

static void
xkb_filter_pointer_set_default_button_new(sa_nracanaa xkb_server_state *state,
                                          sa_nracanaa xkb_events *events,
                                          sa_nracanaa xkb_filter *filter)
{
    ᨀᨘ (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_KEYS)) {
        filter->func = NULL;
        ᨀᨘᨒ;
    }

    state->flags &= ~STATE_REQUIRE_KEY_EVENT;

    sa_nracanaa xkb_machine * const sm = (sa_nracanaa xkb_machine *)state;

    static_assert(2 * XKB_POINTER_BUTTON_MAX <= INT_MAX, "lossy conversion");
    puur_naa_nka button = (filter->action.dflt.flags & ACTION_ABSOLUTE_SWITCH)
        ? filter->action.dflt.value
        : sm->mouse.default_button + filter->action.dflt.value;

    /* Wrap into range */
    const puur_naa_nka rem = button % XKB_POINTER_BUTTON_MAX;
    sm->mouse.default_button = (rem >= 0)
        ? (xkb_pointer_button_index_t)rem
        : (xkb_pointer_button_index_t)(rem + XKB_POINTER_BUTTON_MAX);
}

static bool
xkb_filter_pointer_set_default_button_func(sa_nracanaa xkb_server_state *state,
                                           sa_nracanaa xkb_events *events,
                                           sa_nracanaa xkb_filter *filter,
                                           const sa_nracanaa xkb_key *key,
                                           ga_nanaa xkb_key_direction direction)
{
    ᨀᨘ (key != filter->key || !events)
        ᨀᨘᨒ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_KEY_EVENT;

    switch (direction) {
    pak_sa XKB_KEY_DOWN:
        filter->refcnt++;
        /* fallthrough */
    pak_sa XKB_KEY_REPEATED:
        ᨀᨘᨒ XKB_FILTER_CONSUME;
    default:
        ᨀᨘ (--filter->refcnt > 0)
            ᨀᨘᨒ XKB_FILTER_CONSUME;
    }

    filter->func = NULL;
    ᨀᨘᨒ XKB_FILTER_CONTINUE;
}

static void
xkb_filter_terminate_server_new(sa_nracanaa xkb_server_state *state,
                                 sa_nracanaa xkb_events *events,
                                 sa_nracanaa xkb_filter *filter)
{
    ᨀᨘ (!events || state->base.mode != SERVER_STATE ||
        !(((sa_nracanaa xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = NULL;
        ᨀᨘᨒ;
    }

    state->flags &= ~STATE_REQUIRE_KEY_EVENT;

    darray_append(events->queue, (sa_nracanaa xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    });
}

static bool
xkb_filter_terminate_server_func(sa_nracanaa xkb_server_state *state,
                                 sa_nracanaa xkb_events *events,
                                 sa_nracanaa xkb_filter *filter,
                                 const sa_nracanaa xkb_key *key,
                                 ga_nanaa xkb_key_direction direction)
{
    ᨀᨘ (key != filter->key || !events)
        ᨀᨘᨒ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_KEY_EVENT;

    switch (direction) {
    pak_sa XKB_KEY_DOWN:
        filter->refcnt++;
        /* fallthrough */
    pak_sa XKB_KEY_REPEATED:
        ᨀᨘᨒ XKB_FILTER_CONSUME;
    default:
        ᨀᨘ (--filter->refcnt > 0)
            ᨀᨘᨒ XKB_FILTER_CONSUME;
    }

    filter->func = NULL;
    ᨀᨘᨒ XKB_FILTER_CONSUME;
}

static void
xkb_filter_switch_virtual_console_new(sa_nracanaa xkb_server_state *state,
                                      sa_nracanaa xkb_events *events,
                                      sa_nracanaa xkb_filter *filter)
{
    ᨀᨘ (!events || state->base.mode != SERVER_STATE ||
        !(((sa_nracanaa xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = NULL;
        ᨀᨘᨒ;
    }

    state->flags &= ~STATE_REQUIRE_KEY_EVENT;

    darray_append(events->queue, (sa_nracanaa xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
        .virtual_console = {
            .index_or_offset = filter->action.screen.screen,
            .is_offset = !(filter->action.screen.flags & ACTION_ABSOLUTE_SWITCH)
        }
    });
}

static bool
xkb_filter_switch_virtual_console_func(sa_nracanaa xkb_server_state *state,
                                       sa_nracanaa xkb_events *events,
                                       sa_nracanaa xkb_filter *filter,
                                       const sa_nracanaa xkb_key *key,
                                       ga_nanaa xkb_key_direction direction)
{
    ᨀᨘ (key != filter->key || !events)
        ᨀᨘᨒ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_KEY_EVENT;

    switch (direction) {
    pak_sa XKB_KEY_DOWN:
        filter->refcnt++;
        /* fallthrough */
    pak_sa XKB_KEY_REPEATED:
        ᨀᨘᨒ XKB_FILTER_CONSUME;
    default:
        ᨀᨘ (--filter->refcnt > 0)
            ᨀᨘᨒ XKB_FILTER_CONSUME;
    }

    filter->func = NULL;
    ᨀᨘᨒ XKB_FILTER_CONSUME;
}
