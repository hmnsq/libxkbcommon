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

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <limits.h>
#અન્તર્ભાવ <stdint.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <string.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "xkbcommon/xkbcommon-status.h"
#અન્તર્ભાવ "xkbcommon/xkbcommon-features.h"
#અન્તર્ભાવ "abi-check.h"
#અન્તર્ભાવ "darray.h"
#અન્તર્ભાવ "features/enums.h"
#અન્તર્ભાવ "keymap.h"
#અન્તર્ભાવ "keysym.h"
#અન્તર્ભાવ "messages-codes.h"
#અન્તર્ભાવ "state-priv.h"
#અન્તર્ભાવ "utf8.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "util-mem.h"
#અન્તર્ભાવ "util-numbers.h"

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
સંરચના xkb_events {
    સંરચના xkb_context *ctx;
    darray(સંરચના xkb_event) queue;
    /**
     * Read cursor for `xkb_events_next()`.
     *
     * Reset to 0 on each `process_*` call.
     */
    darray_size_t next;
};

સંરચના xkb_server_state;

સંરચના xkb_filter {
    સંઘ xkb_action action;
    અચલ સંરચના xkb_ચાવી *ચાવી;
    bool (*func)(સંરચના xkb_server_state *state,
                 સંરચના xkb_events *events,
                 સંરચના xkb_filter *filter,
                 અચલ સંરચના xkb_ચાવી *ચાવી,
                 ગણના xkb_ચાવી_direction direction);
    uint32_t priv;
    પૂર્ણાંક refcnt;
};

ગણના xkb_state_mode_internal {
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
    XKB_STATE_TYPE_અંક_ENTRIES = 5
};

static_assert(CLIENT_STATE == 0 &&
              CLIENT_STATE == SERVER_COMPANION - 1 &&
              SERVER_COMPANION == LEGACY_MIXED_STATE - 1 &&
              LEGACY_MIXED_STATE == LEGACY_SERVER_STATE - 1 &&
              LEGACY_SERVER_STATE == SERVER_STATE - 1 &&
              SERVER_STATE == XKB_STATE_TYPE_અંક_ENTRIES - 1 &&
              XKB_STATE_TYPE_અંક_ENTRIES == 5,
              "Order is used to guard client/server -only APIs: do not change");

ગણના { XKB_LAYOUT_INDEX_T_MIN_WIDTH = 6 };
static_assert(XKB_MAX_GROUPS < (1 << XKB_LAYOUT_INDEX_T_MIN_WIDTH),
              "Cannot encode out_of_range_group::redirect_group");

સંરચના out_of_range_group {
    ગણના xkb_layout_out_of_range_policy policy :
        (કદ(ગણના xkb_layout_out_of_range_policy) * CHAR_BIT -
         XKB_LAYOUT_INDEX_T_MIN_WIDTH);
    xkb_layout_index_t redirect_group : XKB_LAYOUT_INDEX_T_MIN_WIDTH;
};

ગણના { XKB_STATE_MODE_INTERNAL_MIN_WIDTH = 4 /* 3 bits + sign */ };
static_assert(XKB_STATE_TYPE_અંક_ENTRIES <
              (1 << XKB_STATE_MODE_INTERNAL_MIN_WIDTH),
              "Cannot encode xkb_state::mode");

/**
 * Common state
 *
 * `mode` indicates whether the object is casted from a `xkb_server_state` or a
 * `xkb_client_state` struct.
 */
સંરચના xkb_state {
    સંરચના xkb_keymap *keymap;
    /*
     * Before updating the state, we keep a copy of just this struct. This
     * allows us to report which components of the state have changed.
     */
    સંરચના state_components components;
    સંરચના out_of_range_group out_of_range_group;

    ગણના xkb_state_mode_internal mode : XKB_STATE_MODE_INTERNAL_MIN_WIDTH;
    પૂર્ણાંક refcnt : (કદ(પૂર્ણાંક) * CHAR_BIT - XKB_STATE_MODE_INTERNAL_MIN_WIDTH);
};

/**
 * Minimal client state
 *
 * Wrapper for strong typing
 */
સંરચના xkb_client_state {
    /** Inherits from `xkb_state` */
    સંરચના xkb_state base;
};

ગણના state_flags {
    STATE_NO_FLAGS = 0,
    STATE_REQUIRE_ચાવી_EVENT = (1 << 0),
    STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_ચાવી = (1 << 1),
    STATE_UNLOCK_MODS_ON_PRESS = (1 << 2),
};

/**
 * Legacy server state, used for `LEGACY_MIXED_STATE` and `LEGACY_SERVER_STATE`
 * modes.
 *
 * Casted as `xkb_state` in the public API. See `xkb_state::mode`.
 */
સંરચના xkb_server_state {
    /** Inherits from `xkb_state` */
    સંરચના xkb_state base;

    /* Server-specific state data */

    darray(સંરચના xkb_filter) filters;

    /**
     * Mix of constant configuration flags and
     * variable state-processing flags.
     */
    ગણના state_flags flags;

    ગણના xkb_a11y_flags a11y_flags;

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
    int16_t mod_ચાવી_ગણતરી[XKB_MAX_MODS];
};

સ્થિર અચલ સંરચના xkb_ચાવી_type_entry *
get_entry_for_mods(અચલ સંરચના xkb_ચાવી_type *type, xkb_mod_mask_t mods)
{
    માટે (darray_size_t i = 0; i < type->અંક_entries; i++)
        જો (entry_is_active(&type->entries[i]) &&
            type->entries[i].mods.mask == mods)
            પાછા &type->entries[i];
    પાછા ખાલી;
}

સ્થિર અચલ સંરચના xkb_ચાવી_type_entry *
get_entry_for_ચાવી_state(સંરચના xkb_state *state, અચલ સંરચના xkb_ચાવી *ચાવી,
                        xkb_layout_index_t group)
{
    અચલ સંરચના xkb_ચાવી_type* અચલ type = ચાવી->groups[group].type;
    xkb_mod_mask_t active_mods = state->components.mods & type->mods.mask;
    પાછા get_entry_for_mods(type, active_mods);
}

સ્થિર અંતઃરેખ xkb_level_index_t
state_ચાવી_get_level(સંરચના xkb_state *state, અચલ સંરચના xkb_ચાવી *ચાવી,
                         xkb_layout_index_t layout)
{
    જો (layout >= ચાવી->અંક_groups)
        પાછા XKB_LEVEL_INVALID;

    /* If we don't find an explicit match the default is 0. */
    અચલ સંરચના xkb_ચાવી_type_entry* અચલ entry =
        get_entry_for_ચાવી_state(state, ચાવી, layout);

    પાછા (entry) ? entry->level : 0;
}

/**
 * Returns the level to use for the given key and state, or
 * XKB_LEVEL_INVALID.
 */
xkb_level_index_t
xkb_state_ચાવી_get_level(સંરચના xkb_state *state, xkb_keycode_t kc,
                        xkb_layout_index_t layout)
{
    અચલ સંરચના xkb_ચાવી* અચલ ચાવી = Xkb_ચાવી(state->keymap, kc);

    પાછા (ચાવી)
        ? state_ચાવી_get_level(state, ચાવી, layout)
        : XKB_LEVEL_INVALID;
}

સ્થિર અંતઃરેખ xkb_layout_index_t
state_ચાવી_get_layout(સંરચના xkb_state *state, અચલ સંરચના xkb_ચાવી *ચાવી)
{
    પાછા xkb_keymap_ચાવી_effective_layout(ચાવી, state->components.group);
}

/**
 * Returns the layout to use for the given key and state, taking
 * wrapping/clamping/etc into account, or XKB_LAYOUT_INVALID.
 */
xkb_layout_index_t
xkb_state_ચાવી_get_layout(સંરચના xkb_state *state, xkb_keycode_t kc)
{
    અચલ સંરચના xkb_ચાવી* અચલ ચાવી = Xkb_ચાવી(state->keymap, kc);

    જો (!ચાવી)
        પાછા XKB_LAYOUT_INVALID;

    પાછા state_ચાવી_get_layout(state, ચાવી);
}

/* Empty action used for empty levels */
સ્થિર અચલ સંઘ xkb_action dummy_action = { .type = ACTION_TYPE_NONE };

સ્થિર xkb_action_ગણતરી_t
xkb_ચાવી_get_actions(સંરચના xkb_state *state, અચલ સંરચના xkb_ચાવી *ચાવી,
                    અચલ સંઘ xkb_action **actions)
{
    અચલ xkb_layout_index_t layout = state_ચાવી_get_layout(state, ચાવી);
    અચલ xkb_level_index_t level = state_ચાવી_get_level(state, ચાવી, layout);
    જો (level == XKB_LEVEL_INVALID)
        જાઓ err;

    અચલ xkb_action_ગણતરી_t ગણતરી =
        xkb_keymap_ચાવી_get_actions_by_level(state->keymap, ચાવી,
                                            layout, level, actions);
    જો (!ગણતરી)
        જાઓ err;

    પાછા ગણતરી;

err:
    /* Use a dummy action if no corresponding level was found or if it is empty.
     * This is required e.g. to handle latches properly. */
    *actions = &dummy_action;
    પાછા 1;
}

સ્થિર સંરચના xkb_filter *
xkb_filter_new(સંરચના xkb_server_state *state)
{
    સંરચના xkb_filter *filter = ખાલી, *iter;

    darray_foreach(iter, state->filters) {
        જો (iter->func)
            ચાલુ;
        /* Use available slot */
        filter = iter;
        તોડો;
    }

    જો (!filter) {
        /* No available slot: resize the filters array */
        darray_resize0(state->filters, darray_size(state->filters) + 1);
        filter = &darray_item(state->filters, darray_size(state->filters) -1);
    }

    filter->refcnt = 1;
    પાછા filter;
}

/***====================================================================***/

ગણના xkb_filter_result {
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
    જો ((filter_)->action.group.flags & ACTION_ABSOLUTE_SWITCH)          \
        (state_)->components.component_ = (filter_)->action.group.group; \
    નહીંતર                                                                 \
        (state_)->components.component_ += (filter_)->action.group.group

સ્થિર શૂન્ય
xkb_filter_group_set_new(સંરચના xkb_server_state *state,
                         સંરચના xkb_events *events,
                         સંરચના xkb_filter *filter)
{
    static_assert(કદ(state->base.components.base_group) ==
                  કદ(filter->priv),
                  "Max groups don't fit");
    filter->priv = (uint32_t)state->base.components.base_group;
    apply_group_delta(filter, &state->base, base_group);
}

સ્થિર bool
xkb_filter_group_set_func(સંરચના xkb_server_state *state,
                          સંરચના xkb_events *events,
                          સંરચના xkb_filter *filter,
                          અચલ સંરચના xkb_ચાવી *ચાવી,
                          ગણના xkb_ચાવી_direction direction)
{
    જો (ચાવી != filter->ચાવી) {
        filter->action.group.flags &= ~ACTION_LOCK_CLEAR;
        પાછા XKB_FILTER_CONTINUE;
    }

    પસંદગી (direction) {
    સ્થિતિ XKB_ચાવી_DOWN:
        filter->refcnt++;
        /* fallthrough */
    સ્થિતિ XKB_ચાવી_REPEATED:
        પાછા XKB_FILTER_CONSUME;
    મૂળભૂત:
        જો (--filter->refcnt > 0)
            પાછા XKB_FILTER_CONSUME;
    }

    static_assert(કદ(state->base.components.base_group) == કદ(filter->priv),
                  "Max groups don't fit");
    state->base.components.base_group = (int32_t) filter->priv;

    જો (filter->action.group.flags & ACTION_LOCK_CLEAR)
        state->base.components.locked_group = 0;

    filter->func = ખાલી;
    પાછા XKB_FILTER_CONTINUE;
}

સ્થિર ગણના xkb_state_component
get_state_component_changes(અચલ સંરચના state_components *a,
                            અચલ સંરચના state_components *b)
{
    ગણના xkb_state_component mask = 0;

    જો (a->group != b->group)
        mask |= XKB_STATE_LAYOUT_EFFECTIVE;
    જો (a->base_group != b->base_group)
        mask |= XKB_STATE_LAYOUT_DEPRESSED;
    જો (a->latched_group != b->latched_group)
        mask |= XKB_STATE_LAYOUT_LATCHED;
    જો (a->locked_group != b->locked_group)
        mask |= XKB_STATE_LAYOUT_LOCKED;
    જો (a->mods != b->mods)
        mask |= XKB_STATE_MODS_EFFECTIVE;
    જો (a->base_mods != b->base_mods)
        mask |= XKB_STATE_MODS_DEPRESSED;
    જો (a->latched_mods != b->latched_mods)
        mask |= XKB_STATE_MODS_LATCHED;
    જો (a->locked_mods != b->locked_mods)
        mask |= XKB_STATE_MODS_LOCKED;
    જો (a->leds != b->leds)
        mask |= XKB_STATE_LEDS;
    જો (a->controls != b->controls)
        mask |= XKB_STATE_CONTROLS_EFFECTIVE;

    પાછા mask;
}

સ્થિર શૂન્ય
xkb_filter_group_lock_new(સંરચના xkb_server_state *state,
                          સંરચના xkb_events *events,
                          સંરચના xkb_filter *filter)
{
    જો (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release: do nothing on key press.
         *
         * This is a keymap format v2 extension.
         */
        પાછા;
    } નહીંતર {
        /* Lock on key press */
        apply_group_delta(filter, &state->base, locked_group);
    }
}

સ્થિર bool
xkb_filter_group_lock_func(સંરચના xkb_server_state *state,
                           સંરચના xkb_events *events,
                           સંરચના xkb_filter *filter,
                           અચલ સંરચના xkb_ચાવી *ચાવી,
                           ગણના xkb_ચાવી_direction direction)
{
    જો (ચાવી != filter->ચાવી) {
        જો ((filter->action.group.flags & ACTION_LOCK_ON_RELEASE) &&
            direction == XKB_ચાવી_DOWN) {
            /*
             * Another key has been pressed after the locking key:
             * cancel group lock on release.
             *
             * This is a keymap v2 extension.
             */
            filter->action.group.flags &= ~ACTION_LOCK_ON_RELEASE;
        }
        પાછા XKB_FILTER_CONTINUE;
    }

    પસંદગી (direction) {
    સ્થિતિ XKB_ચાવી_DOWN:
        filter->refcnt++;
        /* fallthrough */
    સ્થિતિ XKB_ચાવી_REPEATED:
        પાછા XKB_FILTER_CONSUME;
    મૂળભૂત:
        જો (--filter->refcnt > 0)
            પાછા XKB_FILTER_CONSUME;
    }

    જો (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release
         *
         * This is a keymap v2 extension.
         */
        apply_group_delta(filter, &state->base, locked_group);
    } નહીંતર {
        /* Lock on key press: do nothing on key release. */
    }
    filter->func = ખાલી;
    પાછા XKB_FILTER_CONTINUE;
}

સ્થિર bool
xkb_action_breaks_latch(અચલ સંઘ xkb_action *action,
                        ગણના xkb_action_controls controls,
                        ગણના xkb_internal_action_flags flag,
                        xkb_mod_mask_t mask)
{
    પસંદગી (action->type) {
    સ્થિતિ ACTION_TYPE_NONE:
    સ્થિતિ ACTION_TYPE_VOID:
    સ્થિતિ ACTION_TYPE_PTR_BUTTON:
    સ્થિતિ ACTION_TYPE_PTR_LOCK:
    સ્થિતિ ACTION_TYPE_CTRL_SET:
    સ્થિતિ ACTION_TYPE_CTRL_LOCK:
    સ્થિતિ ACTION_TYPE_SWITCH_VT:
    સ્થિતિ ACTION_TYPE_TERMINATE:
    સ્થિતિ ACTION_TYPE_REDIRECT_ચાવી:
        પાછા true;
    સ્થિતિ ACTION_TYPE_PTR_MOVE:
    સ્થિતિ ACTION_TYPE_PTR_DEFAULT:
        પાછા !(controls & CONTROL_MOUSE_ચાવીઓ);
    સ્થિતિ ACTION_TYPE_INTERNAL:
        પાછા (action->internal.flags & flag) &&
               ((action->internal.clear_latched_mods & mask) == mask);
    મૂળભૂત:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_અંક_ENTRIES,
                      "Missing action type");
        પાછા false;
    }
}

ગણના xkb_ચાવી_latch_state {
    NO_LATCH = 0,
    LATCH_ચાવી_DOWN,
    LATCH_PENDING,
    ચાવી_LATCH_STATE_અંક_ENTRIES
};

#define MAX_XKB_ચાવી_LATCH_STATE_LOG2 2
#જો (ચાવી_LATCH_STATE_અંક_ENTRIES > (1 << MAX_XKB_ચાવી_LATCH_STATE_LOG2)) || \
    (-XKB_MAX_GROUPS) < (INT32_MIN >> MAX_XKB_ચાવી_LATCH_STATE_LOG2) || \
      XKB_MAX_GROUPS > (INT32_MAX >> MAX_XKB_ચાવી_LATCH_STATE_LOG2)
#error "Cannot represent priv field of the group latch filter"
#endif

/* Hold the latch state *and* the group delta */
સંઘ group_latch_priv {
    uint32_t priv;
    સંરચના {
        /* The type is really: enum xkb_key_latch_state, but it is problematic
         * on Windows, because it is interpreted as signed and leads to wrong
         * negative values. */
        unsigned પૂર્ણાંક latch:MAX_XKB_ચાવી_LATCH_STATE_LOG2;
        int32_t group_delta:(32 - MAX_XKB_ચાવી_LATCH_STATE_LOG2);
    };
};

સ્થિર શૂન્ય
xkb_filter_group_latch_new(સંરચના xkb_server_state *state,
                           સંરચના xkb_events *events,
                           સંરચના xkb_filter *filter)
{
    અચલ સંઘ group_latch_priv priv = {
        .latch = LATCH_ચાવી_DOWN,
        .group_delta = (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)
            ? filter->action.group.group - state->base.components.base_group
            : filter->action.group.group
    };
    filter->priv = priv.priv;
    /* Like group set */
    apply_group_delta(filter, &state->base, base_group);
}

સ્થિર bool
xkb_filter_group_latch_func(સંરચના xkb_server_state *state,
                            સંરચના xkb_events *events,
                            સંરચના xkb_filter *filter,
                            અચલ સંરચના xkb_ચાવી *ચાવી,
                            ગણના xkb_ચાવી_direction direction)
{
    સંઘ group_latch_priv priv = {.priv = filter->priv};
    ગણના xkb_ચાવી_latch_state latch = priv.latch;

    જો (direction == XKB_ચાવી_DOWN) {
        અચલ સંઘ xkb_action *actions = ખાલી;
        અચલ xkb_action_ગણતરી_t ગણતરી = xkb_ચાવી_get_actions(&state->base, ચાવી,
                                                             &actions);

        જો (latch == LATCH_ચાવી_DOWN) {
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
            જો (state->a11y_flags & XKB_A11_Y_LATCH_SIMULTANEOUS_ચાવીઓ) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                માટે (xkb_action_ગણતરી_t k = 0; k < ગણતરી; k++) {
                    જો (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_GROUP_LATCH,
                                                0))
                    {
                        latch = NO_LATCH;
                        તોડો;
                    }
                }
            } નહીંતર {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        નહીંતર જો (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is
             * pressed, or promote it to a lock if it's the same group delta &
             * flags and latchToLock option is enabled. */
            અચલ bool sticky_ચાવીઓ = state->base.components.controls
                                   & CONTROL_STICKY_ચાવીઓ;
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
            માટે (xkb_action_ગણતરી_t k = 0; k < ગણતરી; k++) {
                જો (((actions[k].type == ACTION_TYPE_GROUP_LATCH) ||
                     (actions[k].type == ACTION_TYPE_GROUP_SET && sticky_ચાવીઓ)) &&
                     actions[k].group.group == filter->action.group.group &&
                     (actions[k].group.flags & ACTION_ABSOLUTE_SWITCH) ==
                     (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)) {
                    જો (filter->action.group.flags & ACTION_LATCH_TO_LOCK &&
                        filter->action.group.group != 0)
                    {
                        /* Promote to lock */
                        filter->action.type = ACTION_TYPE_GROUP_LOCK;
                        filter->func = xkb_filter_group_lock_func;
                        xkb_filter_group_lock_new(state, events, filter);
                        state->base.components.latched_group -= priv.group_delta;
                        filter->ચાવી = ચાવી;
                        /* XXX beep beep! */
                        પાછા XKB_FILTER_CONSUME;
                    }
                    /* Do nothing if `latchToLock` option is not activated; if
                     * the latch is not broken by the following actions and the
                     * key is not consumed, then another latch filter will be
                     * created.
                     */
                    ચાલુ;
                }
                નહીંતર જો (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_GROUP_LATCH,
                                                 0))
                {
                    /* Breaks the latch */
                    state->base.components.latched_group -= priv.group_delta;
                    filter->func = ખાલી;
                    પાછા XKB_FILTER_CONTINUE;
                }
            }
        }
        નહીંતર {
            /* Ignore press in NO_LATCH state */
            પ્રતિજ્ઞા(latch == NO_LATCH);
        }
    }
    નહીંતર જો (ચાવી != filter->ચાવી) {
        /* Ignore repeat/release of other keys */
    }
    નહીંતર જો (direction == XKB_ચાવી_REPEATED) {
        પાછા XKB_FILTER_CONSUME; // TODO: check
    }
    નહીંતર {
        /* Our key got released.  If we've set it to clear locks, and we
         * currently have a group locked, then release it and
         * don't actually latch.  Else we've actually hit the latching
         * stage, so set PENDING and move our group from base to
         * latched. */
        જો ((filter->action.group.flags & ACTION_LOCK_CLEAR) &&
             state->base.components.locked_group) {
            જો (latch == LATCH_PENDING)
                state->base.components.latched_group -= priv.group_delta;
            નહીંતર
                state->base.components.base_group -= priv.group_delta;
            state->base.components.locked_group = 0;
            filter->func = ખાલી;
        }
        /* Broken latch */
        નહીંતર જો (latch == NO_LATCH) {
            state->base.components.base_group -= priv.group_delta;
            filter->func = ખાલી;
        }
        /* We may already have reached the latch state if pressing the
         * key multiple times without latch-to-lock enabled. */
        નહીંતર જો (latch == LATCH_ચાવી_DOWN) {
            latch = LATCH_PENDING;
            /* Switch from set to latch */
            state->base.components.base_group -= priv.group_delta;
            state->base.components.latched_group += priv.group_delta;
            /* XXX beep beep! */
        }
    }

    priv.latch = latch;
    filter->priv = priv.priv;

    પાછા XKB_FILTER_CONTINUE;
}

સ્થિર શૂન્ય
xkb_filter_mod_set_new(સંરચના xkb_server_state *state,
                       સંરચના xkb_events *events,
                       સંરચના xkb_filter *filter)
{
    અચલ ગણના xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    જો ((filter->action.mods.flags & unlock) == unlock) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension.
         */
        filter->priv = filter->action.mods.mods.mask
                     & ~state->base.components.locked_mods;
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
    } નહીંતર {
        filter->priv = filter->action.mods.mods.mask;
    }

    state->set_mods |= (xkb_mod_mask_t) filter->priv;
}

સ્થિર bool
xkb_filter_mod_set_func(સંરચના xkb_server_state *state,
                        સંરચના xkb_events *events,
                        સંરચના xkb_filter *filter,
                        અચલ સંરચના xkb_ચાવી *ચાવી,
                        ગણના xkb_ચાવી_direction direction)
{

    જો (ચાવી != filter->ચાવી) {
        filter->action.mods.flags &= ~ACTION_LOCK_CLEAR;
        પાછા XKB_FILTER_CONTINUE;
    }

    પસંદગી (direction) {
    સ્થિતિ XKB_ચાવી_DOWN:
        filter->refcnt++;
        /* fallthrough */
    સ્થિતિ XKB_ચાવી_REPEATED:
        પાછા XKB_FILTER_CONSUME;
    મૂળભૂત:
        જો (--filter->refcnt > 0)
            પાછા XKB_FILTER_CONSUME;
    }

    state->clear_mods |= (xkb_mod_mask_t) filter->priv;
    અચલ ગણના xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    જો ((filter->action.mods.flags & unlock) == ACTION_LOCK_CLEAR)
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;

    filter->func = ખાલી;
    પાછા XKB_FILTER_CONTINUE;
}

સ્થિર શૂન્ય
xkb_filter_mod_lock_new(સંરચના xkb_server_state *state,
                        સંરચના xkb_events *events,
                        સંરચના xkb_filter *filter)
{
    filter->priv = (state->base.components.locked_mods &
                    filter->action.mods.mods.mask);

    જો (filter->priv && (filter->action.mods.flags & ACTION_UNLOCK_ON_PRESS)) {
        /*
         * Some of the target modifiers were locked before key press: unlock.
         *
         * This is a keymap v2 extension: unlock-on-press.
         */
        જો (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
            state->base.components.locked_mods &= ~filter->priv;
        /* No further action: cancel filter */
        filter->func = ખાલી;
    } નહીંતર {
        /* Set base mods; lock mods if relevant (XKB 1.0 spec) */
        state->set_mods |= filter->action.mods.mods.mask;
        જો (!(filter->action.mods.flags & ACTION_LOCK_NO_LOCK))
            state->base.components.locked_mods |= filter->action.mods.mods.mask;
    }
}

સ્થિર bool
xkb_filter_mod_lock_func(સંરચના xkb_server_state *state,
                         સંરચના xkb_events *events,
                         સંરચના xkb_filter *filter,
                         અચલ સંરચના xkb_ચાવી *ચાવી,
                         ગણના xkb_ચાવી_direction direction)
{
    જો (ચાવી != filter->ચાવી)
        પાછા XKB_FILTER_CONTINUE;

    પસંદગી (direction) {
    સ્થિતિ XKB_ચાવી_DOWN:
        filter->refcnt++;
        /* fallthrough */
    સ્થિતિ XKB_ચાવી_REPEATED:
        પાછા XKB_FILTER_CONSUME;
    મૂળભૂત:
        જો (--filter->refcnt > 0)
            પાછા XKB_FILTER_CONSUME;
    }

    state->clear_mods |= filter->action.mods.mods.mask;
    જો (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
        state->base.components.locked_mods &= ~filter->priv;

    filter->func = ખાલી;
    પાછા XKB_FILTER_CONTINUE;
}

સ્થિર શૂન્ય
xkb_filter_mod_latch_new(સંરચના xkb_server_state *state,
                         સંરચના xkb_events *events,
                         સંરચના xkb_filter *filter)
{
    /* Latch-on-press + clear-locks imply unlock-on-press */
    અચલ ગણના xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                              | ACTION_LATCH_ON_PRESS;

    જો ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
        (filter->action.mods.flags & unlockOnPress) &&
        (state->base.components.locked_mods & filter->action.mods.mods.mask) ==
         filter->action.mods.mods.mask) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension: clear locks and do not latch.
         */
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
        filter->func = ખાલી;
    } નહીંતર જો (filter->action.mods.flags & ACTION_LATCH_ON_PRESS) {
        /*
         * Latch on key press
         *
         * This is a keymap format v2 extension.
         */
        filter->priv = LATCH_PENDING;
        state->base.components.latched_mods |= filter->action.mods.mods.mask;
        /* XXX beep beep! */
    } નહીંતર {
        /* XKB standard latch action */
        filter->priv = LATCH_ચાવી_DOWN;
        state->set_mods |= filter->action.mods.mods.mask;
    }
}

સ્થિર bool
xkb_filter_mod_latch_func(સંરચના xkb_server_state *state,
                          સંરચના xkb_events *events,
                          સંરચના xkb_filter *filter,
                          અચલ સંરચના xkb_ચાવી *ચાવી,
                          ગણના xkb_ચાવી_direction direction)
{
    ગણના xkb_ચાવી_latch_state latch = filter->priv;

    જો (direction == XKB_ચાવી_DOWN) {
        અચલ સંઘ xkb_action *actions = ખાલી;
        અચલ xkb_action_ગણતરી_t ગણતરી = xkb_ચાવી_get_actions(&state->base, ચાવી,
                                                             &actions);

        જો (latch == LATCH_ચાવી_DOWN) {
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
            જો (state->a11y_flags & XKB_A11_Y_LATCH_SIMULTANEOUS_ચાવીઓ) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                માટે (xkb_action_ગણતરી_t k = 0; k < ગણતરી; k++) {
                    જો (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_MOD_LATCH,
                                                filter->action.mods.mods.mask))
                    {
                        latch = NO_LATCH;
                        તોડો;
                    }
                }
            } નહીંતર {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        નહીંતર જો (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is pressed,
             * or promote it to a lock or plain base set if it's the same
             * modifier. */
            અચલ bool sticky_ચાવીઓ = state->base.components.controls
                                   & CONTROL_STICKY_ચાવીઓ;
            /*
             * NOTE: Action’s flags are not compared:
             * - flags may have been modified by keyboard controls at the
             *   creation of the filter;
             * - it is unlikely to have two LatchMods() actions on the same
             *   key that differ only by their flags;
             * - even if it were the case, one would most likely want both
             *   actions to be considered compatible in this context.
             */
            માટે (xkb_action_ગણતરી_t k = 0; k < ગણતરી; k++) {
                જો (((actions[k].type == ACTION_TYPE_MOD_LATCH) ||
                     (actions[k].type == ACTION_TYPE_MOD_SET && sticky_ચાવીઓ)) &&
                    actions[k].mods.mods.mask == filter->action.mods.mods.mask)
                {
                    જો (filter->action.mods.flags & ACTION_LATCH_TO_LOCK) {
                        /* Mutate the action to LockMods() */
                        filter->action.type = ACTION_TYPE_MOD_LOCK;
                        filter->func = xkb_filter_mod_lock_func;
                        xkb_filter_mod_lock_new(state, events, filter);
                    }
                    નહીંતર {
                        /* Mutate the action to SetMods() */
                        filter->action.type = ACTION_TYPE_MOD_SET;
                        filter->func = xkb_filter_mod_set_func;
                        xkb_filter_mod_set_new(state, events, filter);
                    }
                    filter->ચાવી = ચાવી;
                    /* Clear latches */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    /* XXX beep beep! */
                    પાછા XKB_FILTER_CONSUME;
                }
                નહીંતર જો (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_MOD_LATCH,
                                                 filter->action.mods.mods.mask))
                {
                    /* XXX: This may be totally broken, we might need to break the
                     *      latch in the next run after this press? */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    filter->func = ખાલી;
                    પાછા XKB_FILTER_CONTINUE;
                }
            }
        }
        નહીંતર {
            /* Ignore press in NO_LATCH state */
            પ્રતિજ્ઞા(latch == NO_LATCH);
        }
    }
    નહીંતર જો (ચાવી != filter->ચાવી) {
        /* Ignore repeat/release of other keys */
    }
    નહીંતર જો (direction == XKB_ચાવી_REPEATED) {
        પાછા XKB_FILTER_CONSUME; // TODO: check
    }
    નહીંતર {
        /* Our key got released.  If we’ve set it to clear locks, and we
         * currently have the same modifiers locked, then release them and
         * don't actually latch.  Else we’ve actually hit the latching
         * stage, so set PENDING and move our modifier from base to
         * latched. */

        /* Latch-on-press + clear-locks imply unlock-on-press */
        અચલ ગણના xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                                  | ACTION_LATCH_ON_PRESS;

        જો ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
            !(filter->action.mods.flags & unlockOnPress) &&
            (state->base.components.locked_mods &
             filter->action.mods.mods.mask) == filter->action.mods.mods.mask)
        {
            /* XXX: We might be a bit overenthusiastic about clearing
             *      mods other filters have set here? */
            જો (latch == LATCH_PENDING)
                state->base.components.latched_mods &=
                    ~filter->action.mods.mods.mask;
            નહીંતર
                state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
            filter->func = ખાલી;
        }
        નહીંતર જો (latch == NO_LATCH) {
            /* Broken latch */
            state->clear_mods |= filter->action.mods.mods.mask;
            filter->func = ખાલી;
        }
        નહીંતર જો (!(filter->action.mods.flags & ACTION_LATCH_ON_PRESS)) {
            latch = LATCH_PENDING;
            state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.latched_mods |= filter->action.mods.mods.mask;
            /* XXX beep beep! */
        }
    }

    filter->priv = latch;

    પાછા XKB_FILTER_CONTINUE;
}

સ્થિર શૂન્ય
append_pointer_move(સંરચના xkb_events *events,
                    સંરચના xkb_filter *filter)
{
    ગણના xkb_pointer_motion_flags flags = 0;
    જો (filter->action.ptr.flags & ACTION_ABSOLUTE_X)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_X;
    જો (filter->action.ptr.flags & ACTION_ABSOLUTE_Y)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_Y;
    જો (filter->action.ptr.flags & ACTION_REPEAT)
        flags |= XKB_POINTER_MOTION_REPEATS;
    darray_append(events->queue, (સંરચના xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_MOTION,
        .pointer_motion = {
            .size = કદ(((સંરચના xkb_event*)0)->pointer_motion),
            .flags = flags,
            .x = filter->action.ptr.x,
            .y = filter->action.ptr.y,
        }
    });
}

સ્થિર શૂન્ય
xkb_filter_pointer_move_new(સંરચના xkb_server_state *state,
                            સંરચના xkb_events *events,
                            સંરચના xkb_filter *filter)
{
    જો (!events) {
        filter->func = ખાલી;
        પાછા;
    }

    પ્રતિજ્ઞા(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ચાવીઓ);
    append_pointer_move(events, filter);
    state->flags &= ~STATE_REQUIRE_ચાવી_EVENT;
}

સ્થિર bool
xkb_filter_pointer_move_func(સંરચના xkb_server_state *state,
                             સંરચના xkb_events *events,
                             સંરચના xkb_filter *filter,
                             અચલ સંરચના xkb_ચાવી *ચાવી,
                             ગણના xkb_ચાવી_direction direction)
{
    જો (ચાવી != filter->ચાવી || !events)
        પાછા XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ચાવી_EVENT;

    પસંદગી (direction) {
    સ્થિતિ XKB_ચાવી_DOWN:
        filter->refcnt++;
        append_pointer_move(events, filter);
        પાછા XKB_FILTER_CONSUME;
    સ્થિતિ XKB_ચાવી_REPEATED:
        જો (filter->action.ptr.flags & ACTION_REPEAT)
            append_pointer_move(events, filter);
        પાછા XKB_FILTER_CONSUME;
    મૂળભૂત:
        જો (--filter->refcnt > 0)
            પાછા XKB_FILTER_CONSUME;
    }

    filter->func = ખાલી;
    પાછા XKB_FILTER_CONSUME;
}

/* Implemented with xkb_machine API because it requires struct xkb_machine */
સ્થિર શૂન્ય
xkb_filter_pointer_button_new(સંરચના xkb_server_state *state,
                              સંરચના xkb_events *events,
                              સંરચના xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
સ્થિર bool
xkb_filter_pointer_button_func(સંરચના xkb_server_state *state,
                               સંરચના xkb_events *events,
                               સંરચના xkb_filter *filter,
                               અચલ સંરચના xkb_ચાવી *ચાવી,
                               ગણના xkb_ચાવી_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
સ્થિર શૂન્ય
xkb_filter_pointer_lock_button_new(સંરચના xkb_server_state *state,
                                   સંરચના xkb_events *events,
                                   સંરચના xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
સ્થિર bool
xkb_filter_pointer_lock_button_func(સંરચના xkb_server_state *state,
                                    સંરચના xkb_events *events,
                                    સંરચના xkb_filter *filter,
                                    અચલ સંરચના xkb_ચાવી *ચાવી,
                                    ગણના xkb_ચાવી_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
સ્થિર શૂન્ય
xkb_filter_pointer_set_default_button_new(સંરચના xkb_server_state *state,
                                          સંરચના xkb_events *events,
                                          સંરચના xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
સ્થિર bool
xkb_filter_pointer_set_default_button_func(સંરચના xkb_server_state *state,
                                           સંરચના xkb_events *events,
                                           સંરચના xkb_filter *filter,
                                           અચલ સંરચના xkb_ચાવી *ચાવી,
                                           ગણના xkb_ચાવી_direction direction);

સ્થિર શૂન્ય
xkb_filter_terminate_server_new(સંરચના xkb_server_state *state,
                                 સંરચના xkb_events *events,
                                 સંરચના xkb_filter *filter);

સ્થિર bool
xkb_filter_terminate_server_func(સંરચના xkb_server_state *state,
                                  સંરચના xkb_events *events,
                                  સંરચના xkb_filter *filter,
                                  અચલ સંરચના xkb_ચાવી *ચાવી,
                                  ગણના xkb_ચાવી_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
સ્થિર શૂન્ય
xkb_filter_switch_virtual_console_new(સંરચના xkb_server_state *state,
                                      સંરચના xkb_events *events,
                                      સંરચના xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
સ્થિર bool
xkb_filter_switch_virtual_console_func(સંરચના xkb_server_state *state,
                                       સંરચના xkb_events *events,
                                       સંરચના xkb_filter *filter,
                                       અચલ સંરચના xkb_ચાવી *ચાવી,
                                       ગણના xkb_ચાવી_direction direction);

સ્થિર અંતઃરેખ શૂન્ય
clear_all_latches_and_locks(સંરચના xkb_server_state *state,
                            સંરચના xkb_events *events);

સ્થિર શૂન્ય
xkb_filter_ctrls_new(સંરચના xkb_server_state *state,
                     સંરચના xkb_events *events,
                     સંરચના xkb_filter *filter)
{
    જો (filter->action.type == ACTION_TYPE_CTRL_SET) {
        /* Set: save the specified controls that are *not* already enabled */
        filter->priv = (uint32_t) (
            (~state->base.components.controls) & filter->action.ctrls.ctrls
        );
    } નહીંતર {
        /* Lock: save the specified controls that *are* already enabled */
        filter->priv = (uint32_t) (
            state->base.components.controls & filter->action.ctrls.ctrls
        );
    }

    જો (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_LOCK)) {
        /* Enable the specified controls that are not already enabled */
        state->base.components.controls |= filter->action.ctrls.ctrls;
    }
}

સ્થિર bool
xkb_filter_ctrls_func(સંરચના xkb_server_state *state,
                      સંરચના xkb_events *events,
                      સંરચના xkb_filter *filter,
                      અચલ સંરચના xkb_ચાવી *ચાવી,
                      ગણના xkb_ચાવી_direction direction)
{
    જો (ચાવી != filter->ચાવી)
        પાછા XKB_FILTER_CONTINUE;

    પસંદગી (direction) {
    સ્થિતિ XKB_ચાવી_DOWN:
        filter->refcnt++;
        /* fallthrough */
    સ્થિતિ XKB_ચાવી_REPEATED:
        પાછા XKB_FILTER_CONSUME;
    મૂળભૂત:
        જો (--filter->refcnt > 0)
            પાછા XKB_FILTER_CONSUME;
    }

    જો (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_UNLOCK)) {
        અચલ ગણના xkb_action_controls old = state->base.components.controls;

        /*
         * Set: Disable specified controls that were *not* enabled at key press.
         * Lock: Disable specified controls that *were* enabled at key press.
         */
        state->base.components.controls &= ~(ગણના xkb_action_controls) filter->priv;

        જો ((old & CONTROL_STICKY_ચાવીઓ) &&
            !(state->base.components.controls & CONTROL_STICKY_ચાવીઓ)) {
            /* Sticky keys were disabled: clear all locks and latches */
            clear_all_latches_and_locks(state, events);
        }
    }

    filter->func = ખાલી;
    પાછા XKB_FILTER_CONTINUE;
}

સ્થિર bool
append_redirect_ચાવી_events(સંરચના xkb_server_state *state,
                           સંરચના xkb_events *events,
                           અચલ સંરચના xkb_redirect_ચાવી_action *redirect,
                           ગણના xkb_ચાવી_direction direction)
{
    ગણના xkb_state_component changed = 0;
    અચલ xkb_mod_mask_t mask = redirect->affect_mods;

    /*
     * Reference state: find the last state update in the queue, otherwise
     * use the current state.
     */
    સંરચના xkb_event *event;
    સંરચના state_components last_components = state->base.components;
    darray_foreach_reverse(event, events->queue) {
        જો (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
            last_components = event->components.components;
            તોડો;
        }
    }

    જો (mask) {
        સંરચના state_components new = last_components;
        new.base_mods = (new.base_mods & ~mask) | redirect->mods;
        new.latched_mods = (new.latched_mods & ~mask) | redirect->mods;
        new.locked_mods = (new.locked_mods & ~mask) | redirect->mods;
        new.mods = (new.mods & ~mask) | redirect->mods;

        changed = get_state_component_changes(&last_components, &new);
        જો (changed) {
            darray_append(events->queue, (સંરચના xkb_event) {
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
            darray_append(events->queue, (સંરચના xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_FRAME,
            });
        }
    }

    darray_append(events->queue, (સંરચના xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_ચાવી,
        .ચાવી = {
            .keycode = redirect->keycode,
            .direction = direction
        }
    });

    જો (mask && changed) {
        /* Restore state */
        darray_append(events->queue, (સંરચના xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = last_components,
                .changed = changed
            }
        });
    }

    state->flags &= ~STATE_REQUIRE_ચાવી_EVENT;
    પાછા true;
}

સ્થિર શૂન્ય
xkb_filter_redirect_ચાવી_new(સંરચના xkb_server_state *state,
                            સંરચના xkb_events *events,
                            સંરચના xkb_filter *filter)
{
    /* Action effectual only with the xkb_machine API and a valid keycode */
    જો (!events || filter->action.redirect.keycode == XKB_KEYCODE_INVALID) {
        filter->func = ખાલી;
        પાછા;
    }
    append_redirect_ચાવી_events(state, events, &filter->action.redirect,
                               XKB_ચાવી_DOWN);
}

સ્થિર bool
xkb_filter_redirect_ચાવી_func(સંરચના xkb_server_state *state,
                             સંરચના xkb_events *events,
                             સંરચના xkb_filter *filter,
                             અચલ સંરચના xkb_ચાવી *ચાવી,
                             ગણના xkb_ચાવી_direction direction)
{
    જો (ચાવી != filter->ચાવી)
        પાછા XKB_FILTER_CONTINUE;

    જો (direction == XKB_ચાવી_UP) {
        append_redirect_ચાવી_events(state, events,
                                   &filter->action.redirect, XKB_ચાવી_UP);
        filter->func = ખાલી;
        પાછા XKB_FILTER_CONSUME;
     } નહીંતર જો (direction == XKB_ચાવી_DOWN) {
        અચલ સંઘ xkb_action *actions = ખાલી;
        અચલ xkb_action_ગણતરી_t ગણતરી =
            xkb_ચાવી_get_actions(&state->base, ચાવી, &actions);

        માટે (xkb_action_ગણતરી_t a = 0; a < ગણતરી; a++) {
            જો (actions[a].type == ACTION_TYPE_REDIRECT_ચાવી &&
                actions[a].redirect.keycode != filter->action.redirect.keycode) {
                    /* One action redirects to a different key: release first */
                    append_redirect_ચાવી_events(state, events,
                                               &filter->action.redirect,
                                               XKB_ચાવી_UP);
                filter->func = ખાલી;
                પાછા XKB_FILTER_CONTINUE;
            }
        }
    }

    append_redirect_ચાવી_events(state, events, &filter->action.redirect,
                               direction);
    પાછા XKB_FILTER_CONSUME;
}

સ્થિર અચલ સંરચના {
    શૂન્ય (*new)(સંરચના xkb_server_state *state,
                સંરચના xkb_events *events,
                સંરચના xkb_filter *filter);
    bool (*func)(સંરચના xkb_server_state *state,
                 સંરચના xkb_events *events,
                 સંરચના xkb_filter *filter,
                 અચલ સંરચના xkb_ચાવી *ચાવી, ગણના xkb_ચાવી_direction direction);
} filter_action_funcs[ACTION_TYPE_અંક_ENTRIES] = {
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
    [ACTION_TYPE_REDIRECT_ચાવી] = { xkb_filter_redirect_ચાવી_new,
                                   xkb_filter_redirect_ચાવી_func },
};

/**
 * Applies any relevant filters to the key, first from the list of filters
 * that are currently active, then if no filter has claimed the key, possibly
 * apply a new filter from the key action.
 */
સ્થિર શૂન્ય
xkb_filter_apply_all(સંરચના xkb_server_state *state,
                     સંરચના xkb_events *events,
                     અચલ સંરચના xkb_ચાવી *ચાવી,
                     ગણના xkb_ચાવી_direction direction)
{
    /* First run through all the currently active filters and see if any of
     * them have consumed this event. */
    bool consumed = false;
    સંરચના xkb_filter *filter;
    darray_foreach(filter, state->filters) {
        જો (!filter->func)
            ચાલુ;

        જો (filter->func(state, events, filter, ચાવી, direction) == XKB_FILTER_CONSUME)
            consumed = true;
    }
    જો (consumed || direction == XKB_ચાવી_UP)
        પાછા;

    /* No filter consumed this event, so proceed with the key actions */
    અચલ સંઘ xkb_action *actions = ખાલી;
    xkb_action_ગણતરી_t ગણતરી = xkb_ચાવી_get_actions(&state->base, ચાવી,
                                                         &actions);

    માટે (સંરચના {
            xkb_action_ગણતરી_t ગણતરી;
            અચલ સંઘ xkb_action *actions;
         } next = {0};
         ગણતરી;
         ગણતરી = next.ગણતરી, next.ગણતરી = 0, actions = next.actions
    ) {
        /*
         * Process actions sequentially.
         *
         * NOTE: We rely on the parser to disallow multiple modifier or group
         * actions (see `CheckMultipleActionsCategories`). Allowing multiple
         * such actions requires a refactor of the state handling.
         */
        માટે (xkb_action_ગણતરી_t k = 0; k < ગણતરી; k++) {
            /*
             * It's possible for the keymap to set action->type explicitly, like
             * so:
             *     interpret XF86_Next_VMode {
             *         action = Private(type=0x86, data="+VMode");
             *     };
             * We don't handle those.
             */
            જો (actions[k].type >= ACTION_TYPE_અંક_ENTRIES)
                ચાલુ;

            /* Go to next action if no corresponding action handler */
            જો (!filter_action_funcs[actions[k].type].new)
                ચાલુ;

            /* Add a new filter and run the corresponding initial action */
            filter = xkb_filter_new(state);
            filter->ચાવી = ચાવી;
            filter->action = actions[k];

            પસંદગી (filter->action.type) {
            સ્થિતિ ACTION_TYPE_MOD_SET:
                જો (state->base.components.controls & CONTROL_STICKY_ચાવીઓ) {
                    /* Convert modifier set action to a latch */
                    filter->action.type = ACTION_TYPE_MOD_LATCH;
                    જો (state->a11y_flags & XKB_A11_Y_STICKY_ચાવીઓ_LATCH_TO_LOCK) {
                        filter->action.mods.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                /* fallthrough */
            સ્થિતિ ACTION_TYPE_MOD_LATCH:
            સ્થિતિ ACTION_TYPE_MOD_LOCK: {
                સ્થિર અચલ ગણના xkb_action_flags unlock_mods_on_press
                    = ACTION_LOCK_CLEAR     /* SetMods & LatchMods: can unlock */
                    | ACTION_LOCK_NO_UNLOCK /* LockMods: cannot unlock */
                    | ACTION_EXPLICIT_UNLOCK_ON_PRESS;
                જો ((state->flags & STATE_UNLOCK_MODS_ON_PRESS) &&
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
                તોડો;
            }
            સ્થિતિ ACTION_TYPE_GROUP_SET:
                જો (state->base.components.controls & CONTROL_STICKY_ચાવીઓ) {
                    /* Convert group set action to a latch */
                    filter->action.type = ACTION_TYPE_GROUP_LATCH;
                    જો (state->a11y_flags & XKB_A11_Y_STICKY_ચાવીઓ_LATCH_TO_LOCK) {
                        filter->action.group.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                તોડો;
            સ્થિતિ ACTION_TYPE_GROUP_LOCK:
                જો ((filter->action.group.flags &
                     ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE) &&
                    (state->flags &
                     STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_ચાવી) &&
                    /*
                     * FIXME: sticky keys may leave an unexpected modifier
                     * from the level 0 latched, so disable for now.
                     */
                    !(state->base.components.controls & CONTROL_STICKY_ચાવીઓ))
                {
                    filter->action.group.flags |= ACTION_LOCK_ON_RELEASE;
                    /* Add base level action */
                    next.ગણતરી = xkb_keymap_ચાવી_get_actions_by_level(
                        state->base.keymap, ચાવી, state->base.components.group,
                        0, &next.actions
                    );
                    પ્રતિજ્ઞા(next.ગણતરી == 1);
                }
                તોડો;
            સ્થિતિ ACTION_TYPE_PTR_MOVE:
            સ્થિતિ ACTION_TYPE_PTR_BUTTON:
            સ્થિતિ ACTION_TYPE_PTR_LOCK:
            સ્થિતિ ACTION_TYPE_PTR_DEFAULT:
                જો (!(state->base.components.controls & CONTROL_MOUSE_ચાવીઓ)) {
                    /* Convert pointer actions to NoAction */
                    પ્રતિજ્ઞા(!filter_action_funcs[ACTION_TYPE_NONE].new);
                    filter->func = ખાલી;
                    ચાલુ;
                }
                તોડો;
            સ્થિતિ ACTION_TYPE_REDIRECT_ચાવી:
                // FIXME: this is not efficient to resolve mods here each time
                filter->action.redirect.affect_mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.affect_mods
                );
                filter->action.redirect.mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.mods
                );
                તોડો;
            મૂળભૂત: ;
            }
            filter->func = filter_action_funcs[filter->action.type].func;
            filter_action_funcs[filter->action.type].new(state, events, filter);
        }
    }
}

સ્થિર શૂન્ય
xkb_state_update_derived(સંરચના xkb_state *state);

સ્થિર અંતઃરેખ શૂન્ય
xkb_state_init(સંરચના xkb_state * restrict state,
               સંરચના xkb_keymap * restrict keymap,
               ગણના xkb_state_mode_internal mode)
{
    state->mode = mode;
    state->refcnt = 1;
    state->keymap = xkb_keymap_ref(keymap);
    state->out_of_range_group.policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

    /* Ensure that derived state is correctly initialized */
    xkb_state_update_derived(state);
}

સ્થિર અંતઃરેખ શૂન્ય
xkb_client_state_init(સંરચના xkb_client_state * restrict state,
                      સંરચના xkb_keymap * restrict keymap,
                      ગણના xkb_state_mode_internal mode)
{
    xkb_state_init(&state->base, keymap, mode);
}

સ્થિર અંતઃરેખ શૂન્ય
xkb_server_state_init(સંરચના xkb_server_state * restrict state,
                      સંરચના xkb_keymap * restrict keymap,
                      ગણના xkb_state_mode_internal mode,
                      ગણના xkb_a11y_flags a11y_affect,
                      ગણના xkb_a11y_flags a11y_flags)
{
    xkb_state_init(&state->base, keymap, mode);

    state->a11y_flags = a11y_flags;
    જો (keymap->format != XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !(a11y_affect & XKB_A11_Y_LATCH_SIMULTANEOUS_ચાવીઓ)) {
             /* Keymap v2+: enable extension to XKB if not manually disabled */
             state->a11y_flags |= XKB_A11_Y_LATCH_SIMULTANEOUS_ચાવીઓ;
    }
}

સંરચના xkb_state *
xkb_state_new(સંરચના xkb_keymap *keymap)
{
    સંરચના xkb_server_state * અચલ state = calloc(1, કદ(*state));
    જો (!state) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "Could not allocate a state object.\n");
        પાછા ખાલી;
    }

    xkb_server_state_init(state, keymap, LEGACY_MIXED_STATE, 0, 0);

    પાછા (સંરચના xkb_state *)state;
}

સંરચના xkb_state *
xkb_state_new_with_mode(સંરચના xkb_keymap * restrict keymap,
                        ગણના xkb_state_mode mode,
                        ગણના xkb_status * restrict status)
{
    પસંદગી (mode) {
    સ્થિતિ XKB_STATE_MODE_CLIENT:
    સ્થિતિ XKB_STATE_MODE_SERVER_QUERY: {
        સંરચના xkb_client_state * અચલ state = calloc(1, કદ(*state));
        જો (!state)
            તોડો;

        static_assert((unsigned)XKB_STATE_MODE_CLIENT ==
                      (unsigned)CLIENT_STATE, "");
        static_assert((unsigned)XKB_STATE_MODE_SERVER_QUERY ==
                      (unsigned)SERVER_COMPANION, "");
        xkb_client_state_init(state, keymap, (ગણના xkb_state_mode_internal)mode);

        જો (status)
            *status = XKB_SUCCESS;

        પાછા (સંરચના xkb_state *)state;
    }
    સ્થિતિ XKB_STATE_MODE_SERVER: {
        સંરચના xkb_server_state * અચલ state = calloc(1, કદ(*state));
        જો (!state)
            તોડો;

        xkb_server_state_init(state, keymap, LEGACY_SERVER_STATE, 0, 0);

        જો (status)
            *status = XKB_SUCCESS;

        પાછા (સંરચના xkb_state *)state;
    }
    મૂળભૂત:
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_STATE_MODE_,
                     "Unsupported state mode: %u\n", mode);
        જો (status)
            *status = XKB_ERROR_UNSUPPORTED_STATE_MODE;
        પાછા ખાલી;
    }

    log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                  "Could not allocate a state object.\n");
    જો (status)
        *status = XKB_ERROR_ALLOCATION_FAILURE;
    પાછા ખાલી;
}

સંરચના xkb_state *
xkb_state_ref(સંરચના xkb_state *state)
{
    પ્રતિજ્ઞા(state->refcnt > 0);
    state->refcnt++;
    પાછા state;
}

શૂન્ય
xkb_state_reset(સંરચના xkb_state *state)
{
    memset(&state->components, 0, કદ(state->components));

    જો (state->mode < LEGACY_MIXED_STATE) {
        static_assert(કદ(*state) == કદ(સંરચના xkb_client_state),
                      "Not a mere wrapper");
        /* Nothing else to reset */
    } નહીંતર {
        સંરચના xkb_server_state *state_ = (સંરચના xkb_server_state *)state;
        અચલ સંરચના xkb_server_state prev = *state_;
        memset(state_, 0, કદ(*state_));

        state_->base = prev.base;
        state_->a11y_flags = prev.a11y_flags;
        state_->filters = prev.filters;
        darray_size(state_->filters) = 0;
    }
}

સ્થિર અંતઃરેખ શૂન્ય
xkb_state_destroy(સંરચના xkb_state *state)
{
    xkb_keymap_unref(state->keymap);
    જો (state->mode > SERVER_COMPANION)
        darray_free(((સંરચના xkb_server_state *)state)->filters);
}

શૂન્ય
xkb_state_unref(સંરચના xkb_state *state)
{
    પ્રતિજ્ઞા(!state || state->refcnt > 0);
    જો (!state || --state->refcnt > 0)
        પાછા;

    xkb_state_destroy(state);

    પસંદગી (state->mode) {
    સ્થિતિ CLIENT_STATE:
    સ્થિતિ SERVER_COMPANION:
        મુક્ત((સંરચના xkb_client_state *)state);
        તોડો;
    સ્થિતિ LEGACY_MIXED_STATE:
    સ્થિતિ LEGACY_SERVER_STATE:
        મુક્ત((સંરચના xkb_server_state *)state);
        તોડો;
    સ્થિતિ SERVER_STATE: /* handled by xkb_machine_unref() */
    મૂળભૂત:
        PANIC_UNREACHABLE();
    }
}

સંરચના xkb_keymap *
xkb_state_get_keymap(સંરચના xkb_state *state)
{
    પાછા state->keymap;
}

/**
 * Update the LED state to match the rest of the xkb_state.
 */
સ્થિર શૂન્ય
xkb_state_led_update_all(સંરચના xkb_state *state)
{
    xkb_led_index_t idx;
    અચલ સંરચના xkb_led *led;

    state->components.leds = 0;

    xkb_leds_enumerate(idx, led, state->keymap) {

        જો (led->which_mods != 0 && led->mods.mask != 0) {
            xkb_mod_mask_t mod_mask = 0;
            જો (led->which_mods & XKB_STATE_MODS_EFFECTIVE)
                mod_mask |= state->components.mods;
            જો (led->which_mods & XKB_STATE_MODS_DEPRESSED)
                mod_mask |= state->components.base_mods;
            જો (led->which_mods & XKB_STATE_MODS_LATCHED)
                mod_mask |= state->components.latched_mods;
            જો (led->which_mods & XKB_STATE_MODS_LOCKED)
                mod_mask |= state->components.locked_mods;

            જો (led->mods.mask & mod_mask) {
                state->components.leds |= (UINT32_C(1) << idx);
                ચાલુ;
            }
        }

        જો (led->which_groups != 0) {
            જો (likely(led->groups) != 0) {
                xkb_layout_mask_t group_mask = 0;
                /* Effective and locked groups have been brought into range */
                પ્રતિજ્ઞા(state->components.group < XKB_MAX_GROUPS);
                પ્રતિજ્ઞા(state->components.locked_group >= 0 &&
                       state->components.locked_group < XKB_MAX_GROUPS);
                /* Effective and locked groups are used as mask */
                જો (led->which_groups & XKB_STATE_LAYOUT_EFFECTIVE)
                    group_mask |= (UINT32_C(1) << state->components.group);
                જો (led->which_groups & XKB_STATE_LAYOUT_LOCKED)
                    group_mask |= (UINT32_C(1) << state->components.locked_group);
                /* Base and latched groups only have to be non-zero */
                જો ((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                    state->components.base_group != 0)
                    group_mask |= led->groups;
                જો ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                    state->components.latched_group != 0)
                    group_mask |= led->groups;

                જો (led->groups & group_mask) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    ચાલુ;
                }
            } નહીંતર {
                /* Special case for Base and latched groups */
                જો (((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                     state->components.base_group == 0) ||
                    ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                     state->components.latched_group == 0)) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    ચાલુ;
                }
            }
        }

        જો (led->ctrls & state->components.controls) {
            state->components.leds |= (UINT32_C(1) << idx);
            ચાલુ;
        }
    }
}

/**
 * Calculates the derived state (effective mods/group and LEDs) from an
 * up-to-date xkb_state.
 */
સ્થિર શૂન્ય
xkb_state_update_derived(સંરચના xkb_state *state)
{
    xkb_layout_index_t wrapped;

    state->components.mods = (state->components.base_mods |
                              state->components.latched_mods |
                              state->components.locked_mods);

    /* Lock group must be adjusted, but not base nor latched groups */
    wrapped = XkbWrapGroupIntoRange(state->components.locked_group,
                                    state->keymap->અંક_groups,
                                    state->out_of_range_group.policy,
                                    state->out_of_range_group.redirect_group);
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->components.locked_group =
        (int32_t) (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    /* Effective group must be adjusted */
    wrapped = XkbWrapGroupIntoRange(state->components.base_group +
                                    state->components.latched_group +
                                    state->components.locked_group,
                                    state->keymap->અંક_groups,
                                    state->out_of_range_group.policy,
                                    state->out_of_range_group.redirect_group);
    state->components.group =
        (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    xkb_state_led_update_all(state);
}

સ્થિર શૂન્ય
state_update_enabled_controls(સંરચના xkb_server_state *state,
                              ગણના xkb_keyboard_control_flags affect_controls,
                              ગણના xkb_keyboard_control_flags controls,
                              સંરચના xkb_events *events)
{
    અચલ bool had_sticky_ચાવીઓ = state->base.components.controls
                               & CONTROL_STICKY_ચાવીઓ;

    /*
     * Enable using the public API with the all the Control values, except
     * the internal ones, if any.
     */
    affect_controls =
        ( affect_controls
        & (ગણના xkb_keyboard_control_flags)CONTROL_ALL_BOOLEAN );
    state->base.components.controls &=
        (ગણના xkb_action_controls)~affect_controls;
    state->base.components.controls |=
        (ગણના xkb_action_controls)(controls & affect_controls);

    જો (had_sticky_ચાવીઓ &&
        !(state->base.components.controls & CONTROL_STICKY_ચાવીઓ)) {
        /* Sticky keys were disabled: clear all locks and latches */
        clear_all_latches_and_locks(state, events);
    }

    xkb_state_update_derived(&state->base);
}

/**
 * Given a particular key event, updates the state structure to reflect the
 * new modifiers.
 */
ગણના xkb_state_component
xkb_state_update_ચાવી(સંરચના xkb_state *base_state, xkb_keycode_t kc,
                     ગણના xkb_ચાવી_direction direction)
{
    /* Guard against client-only state */
    પ્રતિજ્ઞા(base_state->mode > SERVER_COMPANION);
    જો (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        પાછા 0;
    }

    સંરચના xkb_server_state * અચલ state =
        (સંરચના xkb_server_state *)base_state;

    અચલ સંરચના xkb_ચાવી* અચલ ચાવી = Xkb_ચાવી(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    જો (!ચાવી || (direction == XKB_ચાવી_REPEATED && !ચાવી->repeats))
        પાછા 0;

    જો (direction == XKB_ચાવી_DOWN &&
        (state->base.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ચાવીઓ) &&
        (state->a11y_flags & XKB_A11_Y_STICKY_ચાવીઓ_NO_SIMULTANEOUS_ચાવીઓ) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ચાવીઓ,
                                      0, ખાલી);
    }

    અચલ સંરચના state_components prev_components = state->base.components;

    state->set_mods = 0;
    state->clear_mods = 0;

    xkb_filter_apply_all(state, ખાલી, ચાવી, direction);

    xkb_mod_index_t i;
    xkb_mod_mask_t bit;
    માટે (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        જો (state->set_mods & bit) {
            state->mod_ચાવી_ગણતરી[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    માટે (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        જો (state->clear_mods & bit) {
            state->mod_ચાવી_ગણતરી[i]--;
            જો (state->mod_ચાવી_ગણતરી[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_ચાવી_ગણતરી[i] = 0;
            }
            state->clear_mods &= ~bit;
        }
    }

    xkb_state_update_derived(&state->base);

    પાછા get_state_component_changes(&prev_components, &state->base.components);
}

/* We need fake keys for `update_latch_modifiers` and `update_latch_group`.
 * These keys must have at least one level in order to break latches. We need 2
 * keys with specific actions in order to update group/mod latches without
 * affecting each other. */
સ્થિર સંરચના xkb_ચાવી_type_entry synthetic_ચાવી_level_entry = { 0 };
સ્થિર સંરચના xkb_ચાવી_type synthetic_ચાવી_type = {
     .અંક_entries = 1,
     .અંક_levels = 1,
     .entries = &synthetic_ચાવી_level_entry
};
સ્થિર અચલ સંરચના xkb_ચાવી synthetic_ચાવી = { 0 };

/* Transcription from xserver: XkbLatchModifiers */
સ્થિર શૂન્ય
update_latch_modifiers(સંરચના xkb_server_state *state,
                       સંરચના xkb_events *events,
                       xkb_mod_mask_t mask, xkb_mod_mask_t latches)
{
    /* Clear affected latched modifiers */
    અચલ xkb_mod_mask_t clear = mask & ~latches;
    state->base.components.latched_mods &= ~clear;

    /* Clear any pending latch to locks using ad hoc action:
     * only affect corresponding modifier latches and no group latch. */
    સંરચના xkb_level synthetic_ચાવી_level_break_mod_latch = {
        .અંક_syms = 0,
        .અંક_actions = 1,
        .upper = XKB_ચાવી_No_પ્રતીક,
        .s.sym = XKB_ચાવી_No_પ્રતીક,
        .a.action.internal = {
            .type = ACTION_TYPE_INTERNAL,
            .flags = INTERNAL_BREAKS_MOD_LATCH,
            .clear_latched_mods = clear
        }
    };
    સંરચના xkb_group synthetic_ચાવી_group_break_mod_latch = {
        .type = &synthetic_ચાવી_type,
        .levels = &synthetic_ચાવી_level_break_mod_latch
    };
    અચલ સંરચના xkb_ચાવી synthetic_ચાવી_break_mod_latch = {
        .અંક_groups = 1,
        .groups = &synthetic_ચાવી_group_break_mod_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_ચાવી_break_mod_latch, XKB_ચાવી_DOWN);

    /* Finally set the latched mods by simulate tapping a key with the
     * corresponding action */
    અચલ સંરચના xkb_ચાવી* અચલ ચાવી = &synthetic_ચાવી;
    અચલ સંઘ xkb_action latch_mods = {
        .mods = {
            .type = ACTION_TYPE_MOD_LATCH,
            .mods = { .mask = mask & latches },
            .flags = 0,
        },
    };
    સંરચના xkb_filter* અચલ filter = xkb_filter_new(state);
    filter->ચાવી = ચાવી;
    filter->func = xkb_filter_mod_latch_func;
    filter->action = latch_mods;
    xkb_filter_mod_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_mod_latch_func(state, events, filter, ચાવી, XKB_ચાવી_UP);
}

/* Transcription from xserver: XkbLatchGroup */
સ્થિર શૂન્ય
update_latch_group(સંરચના xkb_server_state *state,
                   સંરચના xkb_events *events, int32_t group)
{
    /* Clear any pending latch to locks. */
    સ્થિર સંરચના xkb_level synthetic_ચાવી_level_break_group_latch = {
        .અંક_syms = 0,
        .અંક_actions = 1,
        .upper = XKB_ચાવી_No_પ્રતીક,
        .s.sym = XKB_ચાવી_No_પ્રતીક,
        .a.action.internal = {
            .type = ACTION_TYPE_INTERNAL,
            .flags = INTERNAL_BREAKS_GROUP_LATCH,
            .clear_latched_mods = 0
        }
    };
    સ્થિર સંરચના xkb_group synthetic_ચાવી_group_break_group_latch = {
        .type = &synthetic_ચાવી_type,
        .levels = &synthetic_ચાવી_level_break_group_latch
    };
    સ્થિર અચલ સંરચના xkb_ચાવી synthetic_ચાવી_break_group_latch = {
        .અંક_groups = 1,
        .groups = &synthetic_ચાવી_group_break_group_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_ચાવી_break_group_latch, XKB_ચાવી_DOWN);

    /* Simulate tapping a key with a group latch action, but in isolation: i.e.
     * without affecting the other filters. */
    અચલ સંરચના xkb_ચાવી* અચલ ચાવી = &synthetic_ચાવી;
    અચલ સંઘ xkb_action latch_group = {
        .group = {
            .type = ACTION_TYPE_GROUP_LATCH,
            .flags = ACTION_ABSOLUTE_SWITCH,
            .group = group,
        },
    };
    સંરચના xkb_filter* અચલ filter = xkb_filter_new(state);
    filter->ચાવી = ચાવી;
    filter->func = xkb_filter_group_latch_func;
    filter->action = latch_group;
    xkb_filter_group_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_group_latch_func(state, events, filter, ચાવી, XKB_ચાવી_UP);
}

/**
 * Compute the resolved effective mask of an arbitrary input.
 *
 * Contrary to `mod_mask_get_effective`, it resolves only modifiers not present
 * in the canonical mask, so that it enables `xkb_state_serialize_mods` to
 * round trip via `xkb_state_update_mask`.
 */
સ્થિર અંતઃરેખ xkb_mod_mask_t
resolve_to_canonical_mods(સંરચના xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    પાછા
        /*
         * Keep canonical modifier mask.
         * It contains either real modifiers or canonical virtual modifiers.
         */
        (mods & keymap->canonical_state_mask) |
        /* Resolve other modifiers */
        mod_mask_get_effective(keymap,
                               mods & ~keymap->canonical_state_mask);
}

સ્થિર શૂન્ય
state_update_latched_locked(
    સંરચના xkb_server_state * restrict state,
    અચલ સંરચના xkb_state_components_update * restrict update,
    સંરચના xkb_events * restrict events
)
{
    /* Update locks */
    અચલ xkb_mod_mask_t affect_locked_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_locked_mods);
    જો ((update->components & XKB_STATE_MODS_LOCKED) &&
        affect_locked_mods) {
        અચલ xkb_mod_mask_t locked_mods =
            resolve_to_canonical_mods(state->base.keymap, update->locked_mods);
        state->base.components.locked_mods &= ~affect_locked_mods;
        state->base.components.locked_mods |= locked_mods & affect_locked_mods;
    }
    જો (update->components & XKB_STATE_LAYOUT_LOCKED) {
        state->base.components.locked_group = update->locked_layout;
    }

    /* Update latches */
    અચલ xkb_mod_mask_t affect_latched_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_latched_mods);
    જો ((update->components & XKB_STATE_MODS_LATCHED) &&
        affect_latched_mods) {
        અચલ xkb_mod_mask_t latched_mods =
            resolve_to_canonical_mods(state->base.keymap, update->latched_mods);
        update_latch_modifiers(state, events, affect_latched_mods, latched_mods);
    }
    જો (update->components & XKB_STATE_LAYOUT_LATCHED) {
        update_latch_group(state, events, update->latched_layout);
    }
}

ગણના xkb_state_component
xkb_state_update_latched_locked(સંરચના xkb_state *base_state,
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
    પ્રતિજ્ઞા(base_state->mode > SERVER_COMPANION);
    જો (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        પાછા 0;
    }

    સંરચના xkb_server_state * અચલ state =
        (સંરચના xkb_server_state *)base_state;

    અચલ સંરચના state_components previous_components = state->base.components;
    અચલ ગણના xkb_state_component components
        = ((affect_latched_mods || latched_mods) ? XKB_STATE_MODS_LATCHED : 0)
        | ((affect_locked_mods || locked_mods) ? XKB_STATE_MODS_LOCKED : 0)
        | (affect_latched_layout ? XKB_STATE_LAYOUT_LATCHED : 0)
        | (affect_locked_layout ? XKB_STATE_LAYOUT_LOCKED : 0);
    અચલ સંરચના xkb_state_components_update update = {
        .size = કદ(update),
        .components = components,
        .affect_latched_mods = affect_latched_mods,
        .latched_mods = latched_mods,
        .latched_layout = latched_layout,
        .affect_locked_mods = affect_locked_mods,
        .locked_mods = locked_mods,
        .locked_layout = locked_layout
    };
    state_update_latched_locked(state, &update, ખાલી);
    xkb_state_update_derived(&state->base);
    પાછા get_state_component_changes(&previous_components,
                                       &state->base.components);
}

સ્થિર અંતઃરેખ શૂન્ય
clear_all_latches_and_locks(સંરચના xkb_server_state *state,
                            સંરચના xkb_events *events)
{
    સ્થિર અચલ ગણના xkb_state_component components
        = XKB_STATE_MODS_LATCHED | XKB_STATE_MODS_LOCKED
        | XKB_STATE_LAYOUT_LATCHED | XKB_STATE_LAYOUT_LOCKED;
    અચલ સંરચના xkb_state_components_update update = {
        .size = કદ(update),
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

સ્થિર ગણના xkb_status
state_update_layout_policy(સંરચના xkb_server_state *state,
                           અચલ સંરચના xkb_layout_policy_update *update)
{
    જો (xkb_feature_supported(XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY,
                              update->policy)) {
        જો (update->policy == XKB_LAYOUT_OUT_OF_RANGE_REDIRECT) {
            જો (update->redirect < state->base.keymap->અંક_groups) {
                state->base.out_of_range_group.redirect_group =
                    update->redirect;
            } નહીંતર {
                log_err(state->base.keymap->ctx,
                        XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Layout policy: "
                        "unsupported layout index %"PRIu32" > %"PRIu32"\n",
                        update->redirect + 1, state->base.keymap->અંક_groups);
                પાછા XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
            }
        }
        state->base.out_of_range_group.policy = update->policy;
        પાછા XKB_SUCCESS;
    } નહીંતર {
        log_err(state->base.keymap->ctx,
                XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY_,
                "Unsupported layout policy: %d\n", update->policy);
        પાછા XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY;
    }
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
સ્થિર ગણના xkb_status
check_state_update_abi_(સંરચના xkb_context * restrict ctx,
                        અચલ અક્ષર * restrict func,
                        અચલ સંરચના xkb_synthetic_update * restrict update)
{
    ગણના xkb_status status = XKB_SUCCESS;
    જો ((status = xkb_check_state_abi(update)) ||
        (update->reserved0 != 0 &&
         (status = XKB_ERROR_ABI_FORWARD_COMPAT)) ||
        (update->components &&
         (status = xkb_check_state_abi(update->components))) ||
        (update->layout_policy &&
         (status = xkb_check_state_abi(update->layout_policy)))) {
        xkb_log_abi_error(ctx, func, status);
    }
    પાછા status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

#define check_state_update_abi(ctx, update) \
    check_state_update_abi_(ctx, __func__, update)

ગણના xkb_status
xkb_state_update_synthetic(સંરચના xkb_state * base_state,
                           અચલ સંરચના xkb_synthetic_update * update,
                           ગણના xkb_state_component *changed)
{
    /* Guard against client-only state */
    જો (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        પાછા XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    સંરચના xkb_server_state * અચલ state =
        (સંરચના xkb_server_state *)base_state;

    /* Check ABI compatibility */
    ગણના xkb_status status = check_state_update_abi(state->base.keymap->ctx,
                                                    update);
    જો (status != XKB_SUCCESS)
        પાછા status;

    અચલ સંરચના state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    /* Update parametrized controls first */
    જો (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        જો (status != XKB_SUCCESS)
            પાછા status;
    }

    જો (update->components) {
        અચલ સંરચના xkb_state_components_update * અચલ components =
            update->components;
        /* Update boolean controls first */
        જો (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, ખાલી);
        }

        state_update_latched_locked(state, components, ખાલી);
    }

    xkb_state_update_derived(&state->base);

    જો (changed) {
        *changed = get_state_component_changes(&previous_components,
                                               &state->base.components);
    }
    પાછા XKB_SUCCESS;
}

/**
 * Updates the state from a set of explicit masks as gained from
 * xkb_state_serialize_mods and xkb_state_serialize_groups.  As noted in the
 * documentation for these functions in xkbcommon.h, this round-trip is
 * lossy, and should only be used to update a slave state mirroring the
 * master, e.g. in a client/server window system.
 */
ગણના xkb_state_component
xkb_state_update_mask(સંરચના xkb_state *base_state,
                      xkb_mod_mask_t base_mods,
                      xkb_mod_mask_t latched_mods,
                      xkb_mod_mask_t locked_mods,
                      xkb_layout_index_t base_group,
                      xkb_layout_index_t latched_group,
                      xkb_layout_index_t locked_group)
{
    /* Guard against server-only and server companion state */
    પ્રતિજ્ઞા(base_state->mode == CLIENT_STATE ||
           base_state->mode == LEGACY_MIXED_STATE);
    જો (base_state->mode != CLIENT_STATE &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        પાછા 0;
    }

    સંરચના xkb_client_state * અચલ state =
        (સંરચના xkb_client_state *)base_state;

    અચલ સંરચના state_components prev_components = state->base.components;

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

    પાછા get_state_component_changes(&prev_components, &state->base.components);
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Lock_Modifier
 */
સ્થિર bool
should_do_caps_transformation(સંરચના xkb_state *state, xkb_keycode_t kc)
{
    પાછા
        xkb_state_mod_index_is_active(state, XKB_MOD_INDEX_CAPS,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_index_is_consumed(state, kc, XKB_MOD_INDEX_CAPS) == 0;
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Control_Modifier
 */
સ્થિર bool
should_do_ctrl_transformation(સંરચના xkb_state *state, xkb_keycode_t kc)
{
    પાછા
        xkb_state_mod_index_is_active(state, XKB_MOD_INDEX_CTRL,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_index_is_consumed(state, kc, XKB_MOD_INDEX_CTRL) == 0;
}

/**
 * Provides the symbols to use for the given key and state.  Returns the
 * number of symbols pointed to in syms_out.
 */
પૂર્ણાંક
xkb_state_ચાવી_get_syms(સંરચના xkb_state *state, xkb_keycode_t kc,
                       અચલ xkb_keysym_t **syms_out)
{
    અચલ xkb_layout_index_t layout = xkb_state_ચાવી_get_layout(state, kc);
    જો (layout == XKB_LAYOUT_INVALID)
        જાઓ err;

    અચલ xkb_level_index_t level = xkb_state_ચાવી_get_level(state, kc, layout);
    જો (level == XKB_LEVEL_INVALID)
        જાઓ err;

    અચલ સંરચના xkb_ચાવી* અચલ ચાવી = Xkb_ચાવી(state->keymap, kc);

    જો (!ચાવી)
        જાઓ err;

    અચલ સંરચના xkb_level* અચલ leveli =
        xkb_keymap_ચાવી_get_level(state->keymap, ચાવી, layout, level);

    જો (!leveli)
        જાઓ err;

    અચલ xkb_keysym_ગણતરી_t અંક_syms = leveli->અંક_syms;
    જો (અંક_syms == 0)
        જાઓ err;

    જો (should_do_caps_transformation(state, kc)) {
        /* Only simple capitalization rules: keysyms count is unchanged. */
        જો (અંક_syms > 1) {
            *syms_out = (leveli->has_upper)
                      ? leveli->s.syms + અંક_syms
                      : leveli->s.syms;
        } નહીંતર {
            *syms_out = &leveli->upper;
        }
    } નહીંતર {
        *syms_out = (અંક_syms > 1)
                  ? leveli->s.syms
                  : &leveli->s.sym;
    }
    પાછા (પૂર્ણાંક) અંક_syms;

err:
    *syms_out = ખાલી;
    પાછા 0;
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
સ્થિર અક્ષર
XkbToControl(અક્ષર ch)
{
    અક્ષર c = ch;

    જો ((c >= '@' && c < '\177') || c == ' ')
        c &= 0x1F;
    નહીંતર જો (c == '2')
        c = '\000';
    નહીંતર જો (c >= '3' && c <= '7')
        c -= ('3' - '\033');
    નહીંતર જો (c == '8')
        c = '\177';
    નહીંતર જો (c == '/')
        c = '_' & 0x1F;
    પાછા c;
}

/**
 * Provides either exactly one symbol, or XKB_KEY_NoSymbol.
 */
xkb_keysym_t
xkb_state_ચાવી_get_one_sym(સંરચના xkb_state *state, xkb_keycode_t kc)
{
    અચલ xkb_keysym_t *syms = ખાલી;
    અચલ પૂર્ણાંક અંક_syms = xkb_state_ચાવી_get_syms(state, kc, &syms);

    જો (અંક_syms != 1)
        પાછા XKB_ચાવી_No_પ્રતીક;
    નહીંતર
        પાછા syms[0];
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
સ્થિર xkb_keysym_t
get_one_sym_for_string(સંરચના xkb_state *state, xkb_keycode_t kc)
{
    અચલ xkb_layout_index_t layout = xkb_state_ચાવી_get_layout(state, kc);
    અચલ xkb_layout_index_t અંક_layouts =
        xkb_keymap_અંક_layouts_for_ચાવી(state->keymap, kc);
    xkb_level_index_t level = xkb_state_ચાવી_get_level(state, kc, layout);
    જો (layout == XKB_LAYOUT_INVALID || અંક_layouts == 0 ||
        level == XKB_LEVEL_INVALID)
        પાછા XKB_ચાવી_No_પ્રતીક;

    અચલ xkb_keysym_t *syms = ખાલી;
    પૂર્ણાંક nsyms = xkb_keymap_ચાવી_get_syms_by_level(state->keymap, kc,
                                                 layout, level, &syms);
    જો (nsyms != 1)
        પાછા XKB_ચાવી_No_પ્રતીક;
    xkb_keysym_t sym = syms[0];

    જો (should_do_ctrl_transformation(state, kc) && sym > 127u) {
        માટે (xkb_layout_index_t i = 0; i < અંક_layouts; i++) {
            level = xkb_state_ચાવી_get_level(state, kc, i);
            જો (level == XKB_LEVEL_INVALID)
                ચાલુ;

            nsyms = xkb_keymap_ચાવી_get_syms_by_level(state->keymap, kc,
                                                     i, level, &syms);
            જો (nsyms == 1 && syms[0] <= 127u) {
                sym = syms[0];
                તોડો;
            }
        }
    }

    જો (should_do_caps_transformation(state, kc)) {
        sym = xkb_keysym_to_upper(sym);
    }

    પાછા sym;
}

પૂર્ણાંક
xkb_state_ચાવી_get_utf8(સંરચના xkb_state *state, xkb_keycode_t kc,
                       અક્ષર *buffer, size_t size)
{
    પૂર્ણાંક nsyms;
    અચલ xkb_keysym_t *syms = ખાલી;
    અચલ xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    જો (sym != XKB_ચાવી_No_પ્રતીક) {
        nsyms = 1; syms = &sym;
    }
    નહીંતર {
        nsyms = xkb_state_ચાવી_get_syms(state, kc, &syms);
    }

    /* Make sure not to truncate in the middle of a UTF-8 sequence. */
    પૂર્ણાંક offset = 0;
    અક્ષર tmp[XKB_KEYSYM_UTF8_MAX_SIZE];
    માટે (પૂર્ણાંક i = 0; i < nsyms; i++) {
        પૂર્ણાંક ret = xkb_keysym_to_utf8(syms[i], tmp, કદ(tmp));
        જો (ret <= 0)
            જાઓ err_bad;

        ret--;
        જો ((size_t)offset + (size_t)ret <= size)
            memcpy(buffer + offset, tmp, (size_t)ret);
        offset += ret;
    }

    જો ((size_t)offset >= size)
        જાઓ err_trunc;
    buffer[offset] = '\0';

    જો (!is_valid_utf8(buffer, (size_t)offset))
        જાઓ err_bad;

    જો (offset == 1 && (unsigned પૂર્ણાંક) buffer[0] <= 127u &&
        should_do_ctrl_transformation(state, kc))
        buffer[0] = XkbToControl(buffer[0]);

    પાછા offset;

err_trunc:
    જો (size > 0)
        buffer[size - 1] = '\0';
    પાછા offset;

err_bad:
    જો (size > 0)
        buffer[0] = '\0';
    પાછા 0;
}

uint32_t
xkb_state_ચાવી_get_utf32(સંરચના xkb_state *state, xkb_keycode_t kc)
{
    અચલ xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    uint32_t cp = xkb_keysym_to_utf32(sym);

    જો (cp <= 127u && should_do_ctrl_transformation(state, kc))
        cp = (uint32_t) XkbToControl((અક્ષર) cp);

    પાછા cp;
}

/**
 * Serializes the requested modifier state into an xkb_mod_mask_t, with all
 * the same disclaimers as in xkb_state_update_mask.
 */
સ્થિર અંતઃરેખ xkb_mod_mask_t
serialize_mods(અચલ સંરચના state_components *components,
               ગણના xkb_state_component type)
{
    xkb_mod_mask_t ret = 0;

    જો (type & XKB_STATE_MODS_EFFECTIVE)
        પાછા components->mods;

    જો (type & XKB_STATE_MODS_DEPRESSED)
        ret |= components->base_mods;
    જો (type & XKB_STATE_MODS_LATCHED)
        ret |= components->latched_mods;
    જો (type & XKB_STATE_MODS_LOCKED)
        ret |= components->locked_mods;

    પાછા ret;
}

xkb_mod_mask_t
xkb_state_serialize_mods(સંરચના xkb_state *state,
                         ગણના xkb_state_component type)
{
    પાછા serialize_mods(&state->components, type);
}

/**
 * Serialises the requested group state, with all the same disclaimers as
 * in xkb_state_update_mask.
 */
સ્થિર અંતઃરેખ xkb_layout_index_t
serialize_layout(અચલ સંરચના state_components *components,
                 ગણના xkb_state_component type)
{
    જો (type & XKB_STATE_LAYOUT_EFFECTIVE)
        પાછા components->group;

    int32_t ret = 0;
    જો (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret += components->base_group;
    જો (type & XKB_STATE_LAYOUT_LATCHED)
        ret += components->latched_group;
    જો (type & XKB_STATE_LAYOUT_LOCKED)
        ret += components->locked_group;

    પાછા (xkb_layout_index_t)ret;
}

xkb_layout_index_t
xkb_state_serialize_layout(સંરચના xkb_state *state,
                           ગણના xkb_state_component type)
{
    પાછા serialize_layout(&state->components, type);
}

xkb_led_mask_t
xkb_state_serialize_leds(અચલ સંરચના xkb_state *state,
                         ગણના xkb_state_component type)
{
    પાછા (type & XKB_STATE_LEDS)
        ? state->components.leds
        : 0;
}

સ્થિર અંતઃરેખ ગણના xkb_keyboard_control_flags
serialize_controls(અચલ સંરચના state_components *components,
                   ગણના xkb_state_component type)
{
    પાછા (type & XKB_STATE_CONTROLS_EFFECTIVE)
        /*
         * Enable using the public API with the all the Controls values, except
         * the internal ones, if any.
         */
        ? (ગણના xkb_keyboard_control_flags)
          (components->controls & CONTROL_ALL_BOOLEAN)
        : 0;
}

ગણના xkb_keyboard_control_flags
xkb_state_serialize_controls(અચલ સંરચના xkb_state *state,
                             ગણના xkb_state_component type)
{
    પાછા serialize_controls(&state->components, type);
}

/**
 * Gets a modifier mask and returns the resolved effective mask; this
 * is needed because some modifiers can also map to other modifiers, e.g.
 * the "NumLock" modifier usually also sets the "Mod2" modifier.
 */
xkb_mod_mask_t
mod_mask_get_effective(સંરચના xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    /* Initialize the effective mask with its corresponding real mods. */
    xkb_mod_mask_t mask = mods & MOD_REAL_MASK_ALL;

    /* Resolve the virtual modifiers */
    અચલ સંરચના xkb_mod *mod;
    xkb_mod_index_t i;
    xkb_vmods_enumerate(i, mod, &keymap->mods)
        જો (mods & (UINT32_C(1) << i))
            mask |= mod->mapping;

    પાછા mask;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
પૂર્ણાંક
xkb_state_mod_index_is_active(સંરચના xkb_state *state,
                              xkb_mod_index_t idx,
                              ગણના xkb_state_component type)
{
    જો (unlikely(idx >= xkb_keymap_અંક_mods(state->keymap)))
        પાછા -1;

    અચલ xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    જો (!mapping) {
        /* Modifier not mapped */
        પાછા 0;
    }
    /* WARNING: this may overmatch for virtual modifiers */
    પાછા (xkb_state_serialize_mods(state, type) & mapping) == mapping;
}

ગણના {
    XKB_STATE_MATCH_FLAGS = XKB_STATE_MATCH_ANY
                          | XKB_STATE_MATCH_ALL
                          | XKB_STATE_MATCH_NON_EXCLUSIVE
};

/**
 * Helper function for xkb_state_mod_indices_are_active and
 * xkb_state_mod_names_are_active.
 */
સ્થિર bool
match_mod_masks(સંરચના xkb_state *state,
                ગણના xkb_state_component type,
                ગણના xkb_state_match match,
                xkb_mod_mask_t wanted)
{
    અચલ xkb_mod_mask_t active = xkb_state_serialize_mods(state, type);

    જો (!(match & XKB_STATE_MATCH_NON_EXCLUSIVE) && (active & ~wanted))
        પાછા false;

    જો (match & XKB_STATE_MATCH_ANY)
        પાછા active & wanted;

    /* Ensure all flags but XKB_STATE_MATCH_ALL are processed before */
    static_assert(XKB_STATE_MATCH_ALL ==
                  ((ગણના xkb_state_match) XKB_STATE_MATCH_FLAGS &
                   ~(XKB_STATE_MATCH_ANY | XKB_STATE_MATCH_NON_EXCLUSIVE)),
                  "");

    પાછા (active & wanted) == wanted;
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, or -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
પૂર્ણાંક
xkb_state_mod_indices_are_active(સંરચના xkb_state *state,
                                 ગણના xkb_state_component type,
                                 ગણના xkb_state_match match,
                                 ...)
{
    જો (match & ~((ગણના xkb_state_match) XKB_STATE_MATCH_FLAGS))
        પાછા -2;

    va_list ap;
    xkb_mod_mask_t wanted = 0;
    પૂર્ણાંક ret = 0;
    અચલ xkb_mod_index_t અંક_mods = xkb_keymap_અંક_mods(state->keymap);

    va_start(ap, match);
    જ્યારે (1) {
        xkb_mod_index_t idx = va_arg(ap, xkb_mod_index_t);
        જો (idx == XKB_MOD_INVALID)
            તોડો;
        જો (unlikely(idx >= અંક_mods)) {
            ret = -1;
            તોડો;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_end(ap);

    જો (ret == -1)
        પાછા ret;

    જો (!wanted) {
        /* Modifiers not mapped */
        પાછા 0;
    }

    પાછા match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
પૂર્ણાંક
xkb_state_mod_name_is_active(સંરચના xkb_state *state, અચલ અક્ષર *name,
                             ગણના xkb_state_component type)
{
    અચલ xkb_mod_index_t idx = xkb_keymap_mod_get_index(state->keymap, name);

    જો (idx == XKB_MOD_INVALID)
        પાછા -1;

    પાછા xkb_state_mod_index_is_active(state, idx, type);
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ATTR_NULL_SENTINEL પૂર્ણાંક
xkb_state_mod_names_are_active(સંરચના xkb_state *state,
                               ગણના xkb_state_component type,
                               ગણના xkb_state_match match,
                               ...)
{
    જો (match & ~((ગણના xkb_state_match) XKB_STATE_MATCH_FLAGS))
        પાછા -2;

    va_list ap;
    xkb_mod_mask_t wanted = 0;
    પૂર્ણાંક ret = 0;

    va_start(ap, match);
    જ્યારે (1) {
        અચલ અક્ષર *str = va_arg(ap, અચલ અક્ષર *);
        જો (str == ખાલી)
            તોડો;
        અચલ xkb_mod_index_t idx = xkb_keymap_mod_get_index(state->keymap, str);
        જો (idx == XKB_MOD_INVALID) {
            ret = -1;
            તોડો;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_end(ap);

    જો (ret == -1)
        પાછા ret;

    જો (!wanted) {
        /* Modifiers not mapped */
        પાછા 0;
    }

    પાછા match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given group is active with the specified type(s), 0 if
 * not, or -1 if the group is invalid.
 */
પૂર્ણાંક
xkb_state_layout_index_is_active(સંરચના xkb_state *state,
                                 xkb_layout_index_t idx,
                                 ગણના xkb_state_component type)
{
    જો (idx >= state->keymap->અંક_groups)
        પાછા -1;

    પૂર્ણાંક ret = 0;
    જો (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ret |= (state->components.group == idx);
    જો (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret |= (state->components.base_group == (int32_t) idx);
    જો (type & XKB_STATE_LAYOUT_LATCHED)
        ret |= (state->components.latched_group == (int32_t) idx);
    જો (type & XKB_STATE_LAYOUT_LOCKED)
        ret |= (state->components.locked_group == (int32_t) idx);

    પાછા ret;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
પૂર્ણાંક
xkb_state_layout_name_is_active(સંરચના xkb_state *state, અચલ અક્ષર *name,
                                ગણના xkb_state_component type)
{
    અચલ xkb_layout_index_t idx =
        xkb_keymap_layout_get_index(state->keymap, name);

    જો (idx == XKB_LAYOUT_INVALID)
        પાછા -1;

    પાછા xkb_state_layout_index_is_active(state, idx, type);
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
પૂર્ણાંક
xkb_state_led_index_is_active(સંરચના xkb_state *state, xkb_led_index_t idx)
{
    જો (idx >= state->keymap->અંક_leds ||
        state->keymap->leds[idx].name == XKB_ATOM_NONE)
        પાછા -1;

    પાછા !!(state->components.leds & (UINT32_C(1) << idx));
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
પૂર્ણાંક
xkb_state_led_name_is_active(સંરચના xkb_state *state, અચલ અક્ષર *name)
{
    અચલ xkb_led_index_t idx = xkb_keymap_led_get_index(state->keymap, name);

    જો (idx == XKB_LED_INVALID)
        પાછા -1;

    પાછા xkb_state_led_index_is_active(state, idx);
}

/**
 * See:
 * - XkbTranslateKeyCode(3), mod_rtrn return value, from libX11.
 * - MyEnhancedXkbTranslateKeyCode(), a modification of the above, from GTK+.
 */
સ્થિર xkb_mod_mask_t
ચાવી_get_consumed(સંરચના xkb_state *state, અચલ સંરચના xkb_ચાવી *ચાવી,
                 ગણના xkb_consumed_mode mode)
{
    અચલ xkb_layout_index_t group =
        xkb_state_ચાવી_get_layout(state, ચાવી->keycode);
    જો (group == XKB_LAYOUT_INVALID)
        પાછા 0;

    xkb_mod_mask_t preserve = 0;
    xkb_mod_mask_t consumed = 0;

    અચલ સંરચના xkb_ચાવી_type_entry* અચલ matching_entry =
        get_entry_for_ચાવી_state(state, ચાવી, group);
    જો (matching_entry)
        preserve = matching_entry->preserve.mask;

    અચલ સંરચના xkb_ચાવી_type* અચલ type = ચાવી->groups[group].type;
    પસંદગી (mode) {
    સ્થિતિ XKB_CONSUMED_MODE_XKB:
        consumed = type->mods.mask;
        તોડો;

    સ્થિતિ XKB_CONSUMED_MODE_GTK: {
        અચલ સંરચના xkb_ચાવી_type_entry* અચલ no_mods_entry =
            get_entry_for_mods(type, 0);
        અચલ xkb_level_index_t no_mods_leveli = no_mods_entry
                                               ? no_mods_entry->level
                                               : 0;
        અચલ સંરચના xkb_level* અચલ no_mods_level =
            &ચાવી->groups[group].levels[no_mods_leveli];

        માટે (darray_size_t i = 0; i < type->અંક_entries; i++) {
            અચલ સંરચના xkb_ચાવી_type_entry* અચલ entry = &type->entries[i];
            જો (!entry_is_active(entry))
                ચાલુ;

            અચલ સંરચના xkb_level* અચલ level =
                &ચાવી->groups[group].levels[entry->level];
            જો (XkbLevelsSameSyms(level, no_mods_level))
                ચાલુ;

            જો (entry == matching_entry || one_bit_set(entry->mods.mask))
                consumed |= entry->mods.mask & ~entry->preserve.mask;
        }
        તોડો;
    }
    }

    પાછા consumed & ~preserve;
}

પૂર્ણાંક
xkb_state_mod_index_is_consumed2(સંરચના xkb_state *state, xkb_keycode_t kc,
                                 xkb_mod_index_t idx,
                                 ગણના xkb_consumed_mode mode)
{
    અચલ સંરચના xkb_ચાવી* અચલ ચાવી = Xkb_ચાવી(state->keymap, kc);

    જો (unlikely(!ચાવી || idx >= xkb_keymap_અંક_mods(state->keymap)))
        પાછા -1;

    અચલ xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    જો (!mapping) {
        /* Modifier not mapped */
        પાછા 0;
    }
    પાછા (mapping & ચાવી_get_consumed(state, ચાવી, mode)) == mapping;
}

પૂર્ણાંક
xkb_state_mod_index_is_consumed(સંરચના xkb_state *state, xkb_keycode_t kc,
                                xkb_mod_index_t idx)
{
    પાછા xkb_state_mod_index_is_consumed2(state, kc, idx,
                                            XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_mod_mask_remove_consumed(સંરચના xkb_state *state, xkb_keycode_t kc,
                                   xkb_mod_mask_t mask)
{
    અચલ સંરચના xkb_ચાવી* અચલ ચાવી = Xkb_ચાવી(state->keymap, kc);

    જો (!ચાવી)
        પાછા 0;

    પાછા resolve_to_canonical_mods(state->keymap, mask) &
           ~ચાવી_get_consumed(state, ચાવી, XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_ચાવી_get_consumed_mods2(સંરચના xkb_state *state, xkb_keycode_t kc,
                                 ગણના xkb_consumed_mode mode)
{
    પસંદગી (mode) {
    સ્થિતિ XKB_CONSUMED_MODE_XKB:
    સ્થિતિ XKB_CONSUMED_MODE_GTK:
        તોડો;
    મૂળભૂત:
        log_err_func(state->keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "unrecognized consumed modifiers mode: %d\n", mode);
        પાછા 0;
    }

    અચલ સંરચના xkb_ચાવી* અચલ ચાવી = Xkb_ચાવી(state->keymap, kc);
    જો (!ચાવી)
        પાછા 0;

    પાછા ચાવી_get_consumed(state, ચાવી, mode);
}

xkb_mod_mask_t
xkb_state_ચાવી_get_consumed_mods(સંરચના xkb_state *state, xkb_keycode_t kc)
{
    પાછા xkb_state_ચાવી_get_consumed_mods2(state, kc, XKB_CONSUMED_MODE_XKB);
}

/*******************************************************************************
 * Server state machine API
 ******************************************************************************/

/** Entry to keep track of the state of an overlaid key */
સંરચના xkb_overlaid_ચાવી {
    /** The source key being overlaid */
    અચલ સંરચના xkb_ચાવી *old;
    /** The target key */
    અચલ સંરચના xkb_ચાવી *new;
    /** Counter of key currently down */
    પૂર્ણાંક refcnt;
};

સંરચના xkb_shortcuts_config_entry {
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
સંરચના xkb_machine {
    /** Inherits from `xkb_server_state` */
    સંરચના xkb_server_state base;

    /* Extra server-specific state data */

    /** Keyboard overlays handling */
    સંરચના {
        /**
         * Current overlaid keys
         *
         * When a key is overlaid, it remained overlaid in this configuration
         * until totally released, i.e. every key press was matched with the
         * corresponding key release.
         */
        darray(સંરચના xkb_overlaid_ચાવી) ચાવીઓ;
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
    સંરચના machine_config {
        /** Modifiers remapping */
        સંરચના machine_modifiers_config {
            /** Real modifier mask */
            xkb_mod_mask_t mask;
            /** Modifiers re-mappings */
            darray_size_t mappings_અંક;
            સંરચના machine_mods_mapping {
                xkb_mod_mask_t source;
                xkb_mod_mask_t target;
            } * mappings ATTR_COUNTED_BY(mappings_અંક);
        } modifiers;

        /** Shortcuts tweak */
        સંરચના machine_shortcuts_config {
            /** Substitution entries */
            સંરચના xkb_shortcuts_config_entry *entries;
        } shortcuts;
    } config;

    /** Mouse keys handling */
    સંરચના {
        /** Default pointer button */
        xkb_pointer_button_index_t default_button;
        /** Mask of the locked buttons */
        xkb_pointer_button_mask_t locked_buttons;
    } mouse;

    ગણના xkb_machine_flags flags;
};

પ્રકારવ્યાખ્યા darray(સંરચના machine_mods_mapping) machine_mods_mappings;

સંરચના xkb_machine_builder {
    સંરચના xkb_keymap *keymap;

    /** Controls */
    સંરચના {
        /** Accessibility flags */
        સંરચના {
            ગણના xkb_a11y_flags affect_flags;
            ગણના xkb_a11y_flags flags;
        } a11y;
    } controls;

    /** Modifiers re-mapping */
    machine_mods_mappings mods;

    /** Shortcuts tweak */
    સંરચના xkb_shortcuts_config_options {
        /** Substitution entries */
        darray(સંરચના xkb_shortcuts_config_entry) entries;
    } shortcuts;

    ગણના xkb_machine_flags machine_flags;
    ગણના xkb_machine_builder_flags builder_flags;
    પૂર્ણાંક refcnt;
};

સંરચના xkb_machine_builder *
xkb_machine_builder_new(
    સંરચના xkb_keymap * restrict keymap,
    અચલ સંરચના xkb_machine_builder_config * restrict config,
    ગણના xkb_status * restrict status
)
{
    /* Handle default configuration */
    સ્થિર અચલ સંરચના xkb_machine_builder_config default_config = {
        .size = કદ(default_config)
    };
    જો (!config)
        config = &default_config;

    /* Check ABI compatibility */
    અચલ ગણના xkb_status status_ = xkb_check_state_abi(config);
    જો (status_) {
        xkb_log_abi_error(keymap->ctx, __func__, status_);
        જો (status)
            *status = status_;
        પાછા ખાલી;
    }

    /* Sanitize input */
    અચલ ગણના xkb_machine_builder_flags invalid_builder_flags
        = config->builder_flags
        & ~(ગણના xkb_machine_builder_flags)XKB_MACHINE_BUILDER_FLAGS_કિંમતો;
    જો (invalid_builder_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS_,
                     "unrecognized machine builder flags: 0x%x\n",
                     invalid_builder_flags);
        જો (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS;
        પાછા ખાલી;
    }

    અચલ ગણના xkb_machine_flags invalid_machine_flags
        = (ગણના xkb_machine_flags)config->machine_flags
        & ~(ગણના xkb_machine_flags)XKB_MACHINE_FLAGS_કિંમતો;
    જો (invalid_machine_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS_,
                     "unrecognized state machine flags: 0x%x\n",
                     invalid_machine_flags);
        જો (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS;
        પાછા ખાલી;
    }

    સંરચના xkb_machine_builder * અચલ builder = calloc(1, કદ(*builder));
    જો (!builder) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine builder\n");
        જો (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        પાછા ખાલી;
    }

    *builder = (સંરચના xkb_machine_builder) {
        .keymap = xkb_keymap_ref(keymap),
        .refcnt = 1,
        .builder_flags = (ગણના xkb_machine_builder_flags)config->builder_flags,
        .machine_flags = (ગણના xkb_machine_flags)config->machine_flags,
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

    જો (status)
        *status = XKB_SUCCESS;

    પાછા builder;
}

સંરચના xkb_machine_builder *
xkb_machine_builder_ref(સંરચના xkb_machine_builder *builder)
{
    પ્રતિજ્ઞા(builder->refcnt > 0);
    builder->refcnt++;
    પાછા builder;
}

શૂન્ય
xkb_machine_builder_unref(સંરચના xkb_machine_builder *builder)
{
    પ્રતિજ્ઞા(!builder || builder->refcnt > 0);
    જો (!builder || --builder->refcnt > 0)
        પાછા;

    darray_free(builder->shortcuts.entries);
    darray_free(builder->mods);
    xkb_keymap_unref(builder->keymap);
    મુક્ત(builder);
}

સંરચના xkb_keymap *
xkb_machine_builder_get_keymap(અચલ સંરચના xkb_machine_builder *builder)
{
    /* Reference count is not updated. See API doc. */
    પાછા builder->keymap;
}

ગણના xkb_status
xkb_machine_builder_update_a11y(
    સંરચના xkb_machine_builder * restrict builder,
    અચલ સંરચના xkb_machine_builder_a11y_update * restrict update)
{
    /* Check ABI compatibility */
    ગણના xkb_status status = xkb_check_state_abi(update);
    જો (status != XKB_SUCCESS) {
        xkb_log_abi_error(builder->keymap->ctx, __func__, status);
        પાછા status;
    }

    અચલ ગણના xkb_a11y_flags invalid_flags =
        ~(ગણના xkb_a11y_flags)XKB_A11_Y_FLAGS_કિંમતો;

    જો (update->affect_flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y affected flags: %#x\n",
                __func__, update->affect_flags & invalid_flags);
        પાછા XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }
    જો (update->flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y flags: %#x\n",
                __func__, update->flags & invalid_flags);
        પાછા XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }

    builder->controls.a11y.affect_flags |= update->affect_flags;
    builder->controls.a11y.flags &= ~update->affect_flags;
    builder->controls.a11y.flags |= (update->flags & update->affect_flags);

    પાછા XKB_SUCCESS;
}

ગણના xkb_status
xkb_machine_builder_update_mods_remap(
    સંરચના xkb_machine_builder * restrict builder,
    અચલ સંરચના xkb_machine_builder_mods_remap_update * restrict update
)
{
    જો (!update->source) {
        જો (!update->target) {
            /* Reset mappings */
            darray_resize(builder->mods, 0);
            પાછા XKB_SUCCESS;
        } નહીંતર {
            પાછા XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
        }
    }

    /* Check the modifiers against the keymap */
    અચલ xkb_mod_mask_t invalid = ~builder->keymap->canonical_state_mask;
    જો ((update->source & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid source modifiers: 0x%"PRIx32"\n",
                __func__, update->source);
        પાછા XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    જો ((update->target & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid target modifiers: 0x%"PRIx32"\n",
                __func__, update->target);
        પાછા XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    સંરચના machine_mods_mapping *mapping = ખાલી;
    darray_size_t m = 0;
    darray_enumerate(m, mapping, builder->mods) {
        જો (mapping->source == update->source) {
            જો (!update->target) {
                /* Remove mapping */
                darray_remove(builder->mods, m);
            } નહીંતર {
                /* Update mapping */
                mapping->target = update->target;
            }
            પાછા XKB_SUCCESS;
        }
    }

    જો (update->target) {
        /* Append new mapping */
        darray_append(
            builder->mods,
            (સંરચના machine_mods_mapping) {
                .source = update->source,
                .target = update->target
            }
        );
    } નહીંતર {
        /* Ignore missing mapping */
    }

    પાછા XKB_SUCCESS;
}

ગણના xkb_status
xkb_machine_builder_update_shortcut_override(
    સંરચના xkb_machine_builder * restrict builder,
    અચલ સંરચના xkb_machine_builder_shortcut_override_update * restrict update
)
{
    સંરચના xkb_keymap *keymap = builder->keymap;

    /* Check the modifiers against the keymap */
    અચલ xkb_mod_mask_t invalid = ~keymap->canonical_state_mask;
    જો ((update->affect_mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid affected modifiers: 0x%"PRIx32"\n",
                __func__, update->affect_mods);
        પાછા XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    જો ((update->mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid modifiers: 0x%"PRIx32"\n",
                __func__, update->mods);
        પાછા XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    /* Check the layout indices against the keymap */
    જો (update->source >= keymap->અંક_groups &&
        update->source != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "%s: Invalid source layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->અંક_groups, update->source + 1);
        પાછા XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }
    જો (update->target >= keymap->અંક_groups &&
        update->target != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "%s: Invalid target layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->અંક_groups, update->target + 1);
        પાછા XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    સંરચના xkb_shortcuts_config_options * અચલ config = &builder->shortcuts;

    /* Resize array & initialize new entries, if relevant */
    જો (update->source >= darray_size(config->entries) &&
        update->source != XKB_LAYOUT_INVALID) {
        જો (update->target == update->source) {
            /* Skip default setting */
            પાછા XKB_SUCCESS;
        }
        xkb_layout_index_t new = darray_size(config->entries);
        darray_resize(config->entries, update->source + 1);
        માટે (; new <= update->source; new++) {
            darray_item(config->entries, new) =
                (સંરચના xkb_shortcuts_config_entry) {
                    .target = XKB_LAYOUT_INVALID,
                    .mods = 0
                };
        }
    }

    સંરચના xkb_shortcuts_config_entry *entry;
    xkb_layout_index_t source = (update->source == XKB_LAYOUT_INVALID)
        ? 0               /* Loop over all entries */
        : update->source; /* Single entry */
    darray_foreach_from(entry, config->entries, source) {
        જો (update->source == XKB_LAYOUT_INVALID &&
            entry->target == XKB_LAYOUT_INVALID) {
            /* Skip previous deactivated entry */
            source++;
            ચાલુ;
        }

        જો (update->affect_mods) {
            entry->mods &= ~update->affect_mods;
            entry->mods |= (update->affect_mods & update->mods);
        }

        જો (update->source != XKB_LAYOUT_INVALID) {
            entry->target = (update->target == source)
                ? XKB_LAYOUT_INVALID
                : update->target;
            /* Single entry: exit loop */
            તોડો;
        }
        source++;
    }

    પાછા XKB_SUCCESS;
}

/** Compare 2 modifiers combinations */
સ્થિર પૂર્ણાંક
cmp_mod_masks(અચલ શૂન્ય *a, અચલ શૂન્ય *b)
{
    અચલ xkb_mod_mask_t m1 = *((xkb_mod_mask_t *) a);
    અચલ xkb_mod_mask_t m2 = *((xkb_mod_mask_t *) b);
    જો (m1 == m2)
        પાછા 0;

    જો (!m1)
        પાછા +1;
    જો (!m2)
        પાછા -1;

    અચલ xkb_mod_mask_t overlap = (m1 & m2);
    જો (overlap == m1)
        પાછા +1; /* m1 is a subset of m2 */
    જો (overlap == m2)
        પાછા -1; /* m2 is a subset of m1 */

    /* Compare active modifiers numbers */
    અચલ unsigned પૂર્ણાંક count1 = popcount32(m1);
    અચલ unsigned પૂર્ણાંક count2 = popcount32(m2);
    જો (count1 > count2)
        પાછા -1;
    જો (count1 < count2)
        પાછા +1;

    /* Give priority to real modifiers */
    પાછા (m1 < m2) ? -1 : +1;
}

સ્થિર ગણના xkb_status
machine_set_mods(સંરચના xkb_machine *sm,
                 અચલ machine_mods_mappings *raw_mappings)
{
    જો (!darray_empty(*raw_mappings)) {
        machine_mods_mappings mappings = darray_new();
        xkb_mod_mask_t mask = 0;
        અચલ સંરચના machine_mods_mapping *mapping;
        અચલ xkb_mod_mask_t invalid =
            ~sm->base.base.keymap->canonical_state_mask;
        darray_foreach(mapping, *raw_mappings) {
            જો (!mapping->source || !mapping->target ||
                (mapping->source & invalid) || (mapping->target & invalid))
                ચાલુ;
            darray_append(mappings, *mapping);
            mask |= mapping->source;
        }

        /*
         * Re-order the mappings, so that combinations have higher priority
         * if they are superset of other combinations.
         * For other type of overlaps, see the sorting function.
         */
        qsort(darray_વસ્તુઓ(mappings), darray_size(mappings),
              કદ(*darray_વસ્તુઓ(mappings)), &cmp_mod_masks);

        darray_steal(
            mappings,
            &sm->config.modifiers.mappings,
            &sm->config.modifiers.mappings_અંક
        );
        sm->config.modifiers.mask = mask;
    } નહીંતર {
        sm->config.modifiers = (સંરચના machine_modifiers_config) {
            .mappings = ખાલી,
            .mappings_અંક = 0,
            .mask = 0
        };
    }

    પાછા XKB_SUCCESS;
}

સ્થિર ગણના xkb_status
machine_set_shortcuts(
    સંરચના xkb_machine * restrict sm,
    અચલ સંરચના xkb_shortcuts_config_options * restrict options
)
{
    જો (darray_empty(options->entries)) {
        sm->config.shortcuts = (સંરચના machine_shortcuts_config) {
            .entries = ખાલી
        };
        પાછા XKB_SUCCESS;
    }

    સંરચના xkb_keymap * અચલ keymap = sm->base.base.keymap;

    સંરચના xkb_shortcuts_config_entry * entries =
        calloc(keymap->અંક_groups, કદ(*entries));
    જો (!entries)
        પાછા XKB_ERROR_ALLOCATION_FAILURE;

    અચલ xkb_layout_index_t ગણતરી = MIN(
        keymap->અંક_groups,
        (xkb_layout_index_t) darray_size(options->entries)
    );

    bool some_entries = false;
    માટે (xkb_layout_index_t l = 0; l < ગણતરી; l++) {
        /* Sanitize layout target */
        અચલ xkb_layout_index_t target =
            darray_item(options->entries, l).target;
        /* Sanitize modifier mask */
        અચલ xkb_mod_mask_t mods = (
            darray_item(options->entries, l).mods &
            sm->base.base.keymap->canonical_state_mask
        );
        જો (target < keymap->અંક_groups && target != l && mods) {
            entries[l] = (સંરચના xkb_shortcuts_config_entry) {
                .target = target,
                .mods = mods
            };
            some_entries = true;
        } નહીંતર {
            entries[l] = (સંરચના xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
            ચાલુ;
        }
    }

    જો (!some_entries) {
        મુક્ત(entries);
        entries = ખાલી;
    } નહીંતર {
        /* Initialize groups with no entries */
        માટે (xkb_layout_index_t l = ગણતરી; l < keymap->અંક_groups; l++) {
            entries[l] = (સંરચના xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
        }
    }

    sm->config.shortcuts = (સંરચના machine_shortcuts_config) {
        .entries = entries
    };
    પાછા XKB_SUCCESS;
}

સંરચના xkb_machine *
xkb_machine_new(અચલ સંરચના xkb_machine_builder * restrict builder,
                ગણના xkb_status * restrict status)
{
    સંરચના xkb_machine * અચલ machine = calloc(1, કદ(*machine));
    જો (!machine) {
        log_err_func1(builder->keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine\n");
        જો (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        પાછા ખાલી;
    }

    xkb_server_state_init(&machine->base, builder->keymap, SERVER_STATE,
                          builder->controls.a11y.affect_flags,
                          builder->controls.a11y.flags);
    machine->flags = builder->machine_flags;
    જો (!(machine->flags &
          XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_ચાવી_FIX))
    {
        machine->base.flags |= STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_ચાવી;
    }
    જો (machine->flags & XKB_MACHINE_UNLOCK_MODS_ON_PRESS) {
        machine->base.flags |= STATE_UNLOCK_MODS_ON_PRESS;
    }

    ગણના xkb_status status_;
    જો ((status_ = machine_set_mods(machine, &builder->mods)) != XKB_SUCCESS ||
        (status_ = machine_set_shortcuts(machine, &builder->shortcuts)) !=
        XKB_SUCCESS) {
        *status = status_;
        જાઓ error;
    }

    darray_init(machine->overlays.ચાવીઓ);
    machine->mouse.default_button = XKB_POINTER_BUTTON_MIN;

    જો (status)
        *status = XKB_SUCCESS;

    પાછા machine;

error:
    xkb_machine_unref(machine);
    પાછા ખાલી;
}

સંરચના xkb_machine *
xkb_machine_ref(સંરચના xkb_machine *sm)
{
    પ્રતિજ્ઞા(sm->base.base.refcnt > 0);
    sm->base.base.refcnt++;
    પાછા sm;
}

શૂન્ય
xkb_machine_unref(સંરચના xkb_machine *sm)
{
    પ્રતિજ્ઞા(!sm || sm->base.base.refcnt > 0);
    જો (!sm || --sm->base.base.refcnt > 0)
        પાછા;

    xkb_state_destroy(&sm->base.base);
    darray_free(sm->overlays.ચાવીઓ);
    મુક્ત(sm->config.shortcuts.entries);
    મુક્ત(sm->config.modifiers.mappings);
    મુક્ત(sm);
}

સંરચના xkb_keymap *
xkb_machine_get_keymap(અચલ સંરચના xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    પાછા sm->base.base.keymap;
}

સંરચના xkb_state *
xkb_machine_get_state(સંરચના xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    પાછા (સંરચના xkb_state *)sm;
}

સંરચના xkb_state *
xkb_state_new_from_machine(અચલ સંરચના xkb_machine * restrict machine,
                           ગણના xkb_status * restrict status)
{
    /* Creation */
    ગણના xkb_status status_;
    સંરચના xkb_state * અચલ state = xkb_state_new_with_mode(
        machine->base.base.keymap,
        XKB_STATE_MODE_SERVER_QUERY,
        &status_
    );
    જો (status_ != XKB_SUCCESS) {
        જો (status)
            *status = status_;
        પાછા ખાલી;
    }

    પ્રતિજ્ઞા(state->mode == SERVER_COMPANION);
    પ્રતિજ્ઞા(state->refcnt == 1);

    /* Copy machine’s state */
    *state = machine->base.base;

    /* Restore previous values */
    state->mode = SERVER_COMPANION;
    state->refcnt = 1;

    જો (status)
        *status = XKB_SUCCESS;
    પાછા state;
}

સ્થિર શૂન્ય
machine_update_overlays(સંરચના xkb_machine *sm)
{
    /*
     * Overlays indices are stored 1-indexed in nibbles: the lowest
     * nibble corresponds to the latest activated index.
     */

    અચલ xkb_overlay_mask_t mask =
        overlays_from_controls(sm->base.base.components.controls);
    xkb_overlay_mask_t added = mask & ~sm->overlays.enabled;

    /* Remove overlays no longer enabled and keep relative order */
    uint32_t order = sm->overlays.order;
    અચલ xkb_overlay_index_t overlay_max =
        format_max_overlays(sm->base.base.keymap->format);
    માટે (uint8_t n = 0; n < overlay_max;) {
        xkb_overlay_index_t overlay_idx = (order >> (n * 4)) & 0xf;
        જો (!overlay_idx)
            તોડો;
        અચલ xkb_overlay_mask_t overlay_mask =
            (xkb_overlay_mask_t)(1u << (overlay_idx - 1u) /* k is 1-indexed */);
        જો (overlay_mask & mask) {
            /* no duplicates */
            added &= ~overlay_mask;
            n++;
        } નહીંતર {
            /* remove overlay */
            અચલ uint32_t head = (UINT32_C(1) << (n * 4)) - 1;
            order = (order & head) | ((order >> 4) & ~head);
        }
    }

    /* Add new overlays in ascending order */
    માટે (xkb_overlay_index_t k = 0; added; k++, added >>= 1) {
        જો (added & 0x1) {
            order <<= 4;
            order |= k + 1u; /* k is 0-indexed */
        }
    }

    sm->overlays.order = order;
    sm->overlays.enabled = mask;
}

ગણના xkb_status
xkb_machine_process_synthetic(સંરચના xkb_machine *sm,
                              અચલ સંરચના xkb_synthetic_update *update,
                              સંરચના xkb_events *events)
{
    /* Check ABI compatibility */
    ગણના xkb_status status =
        check_state_update_abi(sm->base.base.keymap->ctx, update);
    જો (status != XKB_SUCCESS)
        પાછા status;

    સંરચના xkb_server_state * અચલ state = &sm->base;
    અચલ સંરચના state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    darray_size(events->queue) = 0;
    events->next = 0;

    /* Update parametrized controls first */
    જો (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        જો (status != XKB_SUCCESS)
            પાછા status;
    }

    જો (update->components) {
        અચલ સંરચના xkb_state_components_update * અચલ components =
            update->components;
        /* Update boolean controls first */
        જો (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, events);
        }

        state_update_latched_locked(state, components, events);
    }

    xkb_state_update_derived(&state->base);

    અચલ ગણના xkb_state_component changed = get_state_component_changes(
        &previous_components, &state->base.components
    );
    જો (changed) {
        // TODO: latch controls
        જો (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        /* Create event only if some component actually changed */
        darray_append(events->queue, (સંરચના xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .changed = changed,
                .components = state->base.components
            }
        });

        /* Terminate the frame */
        darray_append(events->queue, (સંરચના xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    પાછા XKB_SUCCESS;
}

સ્થિર ssize_t
do_remap_modifiers(અચલ સંરચના machine_modifiers_config * restrict mappings,
                   સંરચના xkb_state * restrict state,
                   સંરચના xkb_events * restrict events,
                   અચલ સંરચના xkb_ચાવી * restrict ચાવી)
{
    જો (!(mappings->mask & state->components.mods))
        પાછા -1;

    અચલ xkb_layout_index_t layout = state_ચાવી_get_layout(state, ચાવી);
    જો (layout >= ચાવી->અંક_groups)
        પાછા -1;

    અચલ સંરચના xkb_ચાવી_type * અચલ type = ચાવી->groups[layout].type;
    xkb_mod_mask_t affect_mods = 0;
    xkb_mod_mask_t mods = 0;
    માટે (darray_size_t m = 0; m < mappings->mappings_અંક; m++) {
        અચલ સંરચના machine_mods_mapping * અચલ mapping =
            &mappings->mappings[m];
        જો (/* Skip not matching active mods */
            (mapping->source & state->components.mods) == mapping->source &&
            /* Skip overlap with previous active mappings */
            !(mapping->source & affect_mods) &&
            /* Skip if the key type uses some of the source modifiers */
            !(mapping->source & type->mods.mask)) {
            affect_mods |= mappings->mappings[m].source;
            mods |= mappings->mappings[m].target;
        }
    }

    જો (!affect_mods)
        પાછા -1;

    સંરચના xkb_state new = *state;
    new.components.base_mods = (new.components.base_mods & ~affect_mods) | mods;
    new.components.latched_mods = (new.components.latched_mods & ~affect_mods);
    new.components.locked_mods = (new.components.locked_mods & ~affect_mods);
    xkb_state_update_derived(&new);

    ssize_t event_idx = -1;
    અચલ ગણના xkb_state_component changed =
        get_state_component_changes(&state->components, &new.components);
    જો (changed) {
        event_idx = (ssize_t) darray_size(events->queue);
        darray_append(events->queue, (સંરચના xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = new.components,
                .changed = changed
            }
        });
    }

    state->components.mods = new.components.mods;
    પાછા event_idx;
}

/**
 * Activate an alternative layout if some modifiers are active, typically
 * `Control`, `Alt` or `Super`.
 *
 * The tweak consists of computing a *base* layout such that the *effective*
 * layout is the corresponding target layout.
 */
સ્થિર ssize_t
do_shortcuts_tweak(અચલ સંરચના machine_shortcuts_config *config,
                   સંરચના xkb_state *state,
                   અચલ સંરચના state_components *previous_components,
                   સંરચના xkb_events *events, ssize_t remap_event)
{
    જો (!config->entries) {
        /* No shortcuts tweak */
        પાછા remap_event;
    }

    અચલ સંરચના xkb_shortcuts_config_entry * અચલ entry =
        &config->entries[state->components.group];

    જો (entry->target != XKB_LAYOUT_INVALID &&
        (entry->mods & state->components.mods)) {
        /*
         * Activate shortcuts tweak:
         * 1. The real base group is saved by the caller, to be restored by
         *    later by a call to undo_tweaks().
         * 2. Compute the new base group so that the resulting effective group
         *    is the target.
         * 3. Recompute derived state components.
         */
        સંરચના xkb_state new = *state;
        જો (remap_event < 0) {
            /* Create new event */
            remap_event = (ssize_t) darray_size(events->queue);
            darray_append(events->queue, (સંરચના xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = {0},
                    .changed = 0
                }
            });
        } નહીંતર {
            /* Merge with last remap event */
            new.components =
                darray_item(events->queue, remap_event).components.components;
        }
        new.components.base_group
            = (int32_t) entry->target
            - state->components.latched_group
            - state->components.locked_group;
        xkb_state_update_derived(&new);

        અચલ ગણના xkb_state_component changed =
            get_state_component_changes(previous_components, &new.components);
        darray_item(events->queue, remap_event).components.changed = changed;
        darray_item(events->queue, remap_event).components.components =
            new.components;

        state->components.group = new.components.group;
    } નહીંતર {
        /* Deactivated entry or modifiers do not match */
    }
    પાછા remap_event;
}

સ્થિર શૂન્ય
undo_tweaks(અચલ સંરચના xkb_state *state,
            અચલ સંરચના state_components *initial_components,
            સંરચના xkb_events *events)
{
    /* Get last component event */
    અચલ સંરચના xkb_event *event = ખાલી;
    darray_foreach_reverse(event, events->queue) {
        જો (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS)
            તોડો;
    }
    જો (!event)
        પાછા;

    /* Restore state */
    અચલ ગણના xkb_state_component changed =
        get_state_component_changes(initial_components,
                                    &event->components.components);
    જો (changed) {
        darray_append(events->queue, (સંરચના xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = *initial_components,
                .changed = changed
            }
        });
    }
}

સ્થિર અચલ સંરચના xkb_ચાવી *
process_overlayable_ચાવી(સંરચના xkb_machine *sm,
                        અચલ સંરચના xkb_ચાવી * ચાવી,
                        ગણના xkb_ચાવી_direction direction)
{
    /* Check if key is currently overlaid */
    સંરચના xkb_overlaid_ચાવી *entry = ખાલી;
    સંરચના xkb_overlaid_ચાવી *available_entry = ખાલી;
    darray_foreach(entry, sm->overlays.ચાવીઓ) {
        જો (entry->old == ચાવી) {
            /* Maintain the key overlaid */
            પસંદગી (direction) {
            સ્થિતિ XKB_ચાવી_DOWN:
                entry->refcnt++;
                તોડો;
            સ્થિતિ XKB_ચાવી_REPEATED:
                તોડો;
            સ્થિતિ XKB_ચાવી_UP:
            મૂળભૂત:
                entry->refcnt--;
                તોડો;
            }
            જો (entry->refcnt <= 0)
                entry->old = ખાલી;
            પાછા entry->new;
        } નહીંતર જો (!entry->old && !available_entry) {
            /* First free spot */
            available_entry = entry;
        }
    }
    જો (direction == XKB_ચાવી_DOWN) {
        /* Enable key overlay */

        /*
         * Get overlay index: select the latest activated overlay
         * containing the key.
         */
        અચલ સંરચના xkb_ચાવી *new = ચાવી;

        જો (ચાવી->overlays & sm->overlays.enabled) {
            /* Some relevant overlay is active */
            માટે (uint32_t ઢગલો = sm->overlays.order; ઢગલો; ઢગલો >>= 4) {
                static_assert(XKB_OVERLAY_ગણતરી == 8, "");
                અચલ xkb_overlay_index_t overlay = (ઢગલો & 0xf) - 1;
                અચલ xkb_overlay_mask_t mask =
                    (xkb_overlay_mask_t)(1u << overlay);
                જો (ચાવી->overlays & mask) {
                    /* Get new key */
                    જો (ચાવી->overlays_inline) {
                        /* Stored inline */
                        new = ચાવી->overlay_ચાવી;
                    } નહીંતર {
                        /* Stored in sparse array */
                        અચલ xkb_overlay_mask_t નીચલું = (xkb_overlay_mask_t)(
                            ચાવી->overlays & (xkb_overlay_mask_t)(mask - 1u)
                        );
                        /* index = number of set bits strictly below bit */
                        અચલ xkb_overlay_index_t index =
                            (xkb_overlay_index_t)popcount32(નીચલું);
                        new = ચાવી->overlays_ચાવીઓ[index];
                    }
                    તોડો;
                }
            }
        } નહીંતર {
            /*
             * No relevant overlay is active: block future overlays while
             * the key is pressed.
             */
        }

        /* Get new entry */
        જો (available_entry) {
            /* Re-use a free entry */
            entry = available_entry;
        } નહીંતર {
            /* Append a new entry */
            અચલ darray_size_t idx = darray_size(sm->overlays.ચાવીઓ);
            darray_resize(sm->overlays.ચાવીઓ, idx + 1);
            entry = &darray_item(sm->overlays.ચાવીઓ, idx);
        }

        /* Overlay the key */
        entry->old = ચાવી;
        entry->new = new;
        entry->refcnt = 1;
        પાછા new;
    }

    /* No overlay */
    પાછા ચાવી;
}

ગણના xkb_status
xkb_machine_process_ચાવી(સંરચના xkb_machine *sm,
                        xkb_keycode_t kc, ગણના xkb_ચાવી_direction direction,
                        સંરચના xkb_events *events)
{
    darray_size(events->queue) = 0;
    events->next = 0;

    સંરચના xkb_server_state * અચલ state = &sm->base;
    અચલ સંરચના xkb_ચાવી * ચાવી = Xkb_ચાવી(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    જો (!ચાવી || (direction == XKB_ચાવી_REPEATED && !ચાવી->repeats))
        પાછા XKB_SUCCESS;

    અચલ સંરચના state_components initial_components = state->base.components;

    જો (direction == XKB_ચાવી_DOWN &&
        (sm->base.base.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ચાવીઓ) &&
        (sm->base.a11y_flags & XKB_A11_Y_STICKY_ચાવીઓ_NO_SIMULTANEOUS_ચાવીઓ) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ચાવીઓ,
                                      0, events);
    }

    જો (ચાવી->overlays)
        ચાવી = process_overlayable_ચાવી(sm, ચાવી, direction);

    ssize_t remap_event = do_remap_modifiers(&sm->config.modifiers,
                                             &state->base, events, ચાવી);

    remap_event = do_shortcuts_tweak(&sm->config.shortcuts, &state->base,
                                     &initial_components, events, remap_event);

    જો (remap_event >= 0) {
        // TODO: what about merging state event for RedirectKey?
        darray_append(events->queue, (સંરચના xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    state->flags |= STATE_REQUIRE_ચાવી_EVENT;
    state->set_mods = 0;
    state->clear_mods = 0;

    /* Handle key actions */
    xkb_filter_apply_all(state, events, ચાવી, direction);

    xkb_mod_index_t i;
    xkb_mod_mask_t bit;
    માટે (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        જો (state->set_mods & bit) {
            state->mod_ચાવી_ગણતરી[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    માટે (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        જો (state->clear_mods & bit) {
            state->mod_ચાવી_ગણતરી[i]--;
            જો (state->mod_ચાવી_ગણતરી[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_ચાવી_ગણતરી[i] = 0;
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
    જો (state->flags & STATE_REQUIRE_ચાવી_EVENT) {
        darray_append(events->queue, (સંરચના xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_ચાવી,
            .ચાવી = {
                .keycode = ચાવી->keycode,
                .direction = direction
            }
        });
    }

    જો (remap_event >= 0) {
        // FIXME: fragile if last state change does not restore the state to the remap event
        undo_tweaks(&state->base, &initial_components, events);
    }

    અચલ ગણના xkb_state_component changed = get_state_component_changes(
        &initial_components, &state->base.components
    );
    જો (changed) {
        જો (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        // TODO: merge with previous state event, if any

        darray_append(events->queue, (સંરચના xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = state->base.components,
                .changed = changed
            }
        });
    }

    જો (!darray_empty(events->queue) &&
        darray_item(events->queue, darray_size(events->queue) - 1).type !=
        XKB_EVENT_TYPE_FRAME)
    {
        darray_append(events->queue, (સંરચના xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    પાછા XKB_SUCCESS;
}

સંરચના xkb_events *
xkb_events_new(સંરચના xkb_context * restrict context,
               અચલ સંરચના xkb_events_config * restrict config,
               ગણના xkb_status * restrict status)
{
    /* Handle default configuration */
    સ્થિર અચલ સંરચના xkb_events_config default_config = {
        .size = કદ(default_config)
    };
    જો (!config)
        config = &default_config;

    /* Check ABI compatibility */
    અચલ ગણના xkb_status abi_error = xkb_check_state_abi(config);
    જો (abi_error) {
        xkb_log_abi_error(context, __func__, abi_error);
        જો (status)
            *status = abi_error;
        પાછા ખાલી;
    }

    /* Sanitize input */
    અચલ uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_EVENTS_FLAGS_કિંમતો);
    જો (invalid_flags) {
        log_err_func(context, XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS_,
                     "unrecognized events collection flags: 0x%"PRIx32"\n",
                     invalid_flags);
        જો (status)
            *status = XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS;
        પાછા ખાલી;
    }

    સંરચના xkb_events *events = calloc(1, કદ(*events));
    જો (!events) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events collection\n");
        જો (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        પાછા ખાલી;
    }

    /* Pre-allocate event queue */
    darray_init(events->queue);
    સ્થિર અચલ darray_size_t queue_initial_size = 16;
    darray_resize(events->queue, queue_initial_size);
    // TODO: proper safe darray_resize() with return value
    જો (!darray_વસ્તુઓ(events->queue)) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events queue\n");
        xkb_events_destroy(events);
        જો (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        પાછા ખાલી;
    }
    darray_size(events->queue) = 0;

    events->ctx = xkb_context_ref(context);
    events->next = 0;

    જો (status)
        *status = XKB_SUCCESS;
    પાછા events;
}

શૂન્ય
xkb_events_destroy(સંરચના xkb_events *events)
{
    જો (!events)
        પાછા;
    darray_free(events->queue);
    xkb_context_unref(events->ctx);
    મુક્ત(events);
}

અચલ સંરચના xkb_event *
xkb_events_next(સંરચના xkb_events *events)
{
    જો (events->next < darray_size(events->queue)) {
        અચલ darray_size_t index = events->next++;
        પાછા &darray_item(events->queue, index);
    } નહીંતર {
        પાછા ખાલી;
    }
}

ગણના xkb_event_type
xkb_event_get_type(અચલ સંરચના xkb_event *event)
{
    પાછા event->type;
}

ગણના xkb_status
xkb_event_get_keycode(અચલ સંરચના xkb_event *event,
                      xkb_keycode_t *keycode,
                      ગણના xkb_ચાવી_direction *direction)
{
    જો (event->type == XKB_EVENT_TYPE_ચાવી) {
        *keycode = event->ચાવી.keycode;
        *direction = event->ચાવી.direction;
        પાછા XKB_SUCCESS;
    } નહીંતર {
        પાછા XKB_ERROR_EVENT_TYPE_MISMATCH;
    }
}

ગણના xkb_status
xkb_event_get_components(અચલ સંરચના xkb_event * restrict event,
                         સંરચના xkb_event_components * restrict components)
{
    જો (event->type != XKB_EVENT_TYPE_STATE_COMPONENTS)
        પાછા XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    અચલ ગણના xkb_status status = xkb_check_state_abi(components);
    જો (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        પાછા status;
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

    પાછા XKB_SUCCESS;
}

ગણના xkb_status
xkb_event_get_pointer_motion(અચલ સંરચના xkb_event * restrict event,
                             સંરચના xkb_event_pointer_motion * restrict motion)
{
    જો (event->type != XKB_EVENT_TYPE_POINTER_MOTION)
        પાછા XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    અચલ ગણના xkb_status status = xkb_check_state_abi(motion);
    જો (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        પાછા status;
    }

    motion->flags = event->pointer_motion.flags;
    motion->x = event->pointer_motion.x;
    motion->y = event->pointer_motion.y;

    પાછા XKB_SUCCESS;
}

ગણના xkb_status
xkb_event_get_pointer_button(અચલ સંરચના xkb_event * restrict event,
                             સંરચના xkb_event_pointer_button * restrict button)
{
    જો (event->type != XKB_EVENT_TYPE_POINTER_BUTTON)
        પાછા XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    અચલ ગણના xkb_status status = xkb_check_state_abi(button);
    જો (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        પાછા status;
    }

    button->button = event->pointer_button.button;
    button->state = event->pointer_button.state;
    button->ગણતરી = event->pointer_button.ગણતરી;

    પાછા XKB_SUCCESS;
}

ગણના xkb_status
xkb_event_get_virtual_console(અચલ સંરચના xkb_event * restrict event,
                              int8_t * restrict index_or_offset,
                              bool * restrict is_offset)
{
    જો (event->type != XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
        પાછા XKB_ERROR_EVENT_TYPE_MISMATCH;

    *index_or_offset = event->virtual_console.index_or_offset;
    *is_offset = event->virtual_console.is_offset;

    પાછા XKB_SUCCESS;
}

ગણના xkb_status
xkb_state_update_event(સંરચના xkb_state * restrict base_state,
                       અચલ સંરચના xkb_event * restrict event,
                       ગણના xkb_state_component * restrict changed)
{
    /* Guard against server-only state */
    પ્રતિજ્ઞા(base_state->mode == SERVER_COMPANION ||
           base_state->mode == LEGACY_MIXED_STATE);
    જો (base_state->mode != SERVER_COMPANION &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE_,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        પાછા XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    સંરચના xkb_client_state * અચલ state =
        (સંરચના xkb_client_state *)base_state;

    જો (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
        અચલ સંરચના state_components prev_components = state->base.components;
        state->base.components = event->components.components;
        /*
         * Recompute the changes instead of using the event value, because we do
         * not know if the event’s queue and the state are synced.
         */
        જો (changed)
            *changed = get_state_component_changes(&prev_components,
                                                   &state->base.components);
    } નહીંતર {
        જો (changed)
            *changed = 0;
    }
    પાછા XKB_SUCCESS;
}

સ્થિર શૂન્ય
append_pointer_button(સંરચના xkb_events * restrict events,
                      સંરચના xkb_filter * restrict filter,
                      ગણના xkb_pointer_button_state button_state,
                      uint8_t ગણતરી)
{
    darray_append(events->queue, (સંરચના xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_BUTTON,
        .pointer_button = {
            .size = કદ(((સંરચના xkb_event*)0)->pointer_button),
            .button = filter->priv,
            .state = button_state,
            .ગણતરી = ગણતરી,
        }
    });
}

સ્થિર શૂન્ય
xkb_filter_pointer_button_new(સંરચના xkb_server_state *state,
                              સંરચના xkb_events *events,
                              સંરચના xkb_filter *filter)
{
    જો (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ચાવીઓ)) {
        filter->func = ખાલી;
        પાછા;
    }

    સંરચના xkb_machine * અચલ sm = (સંરચના xkb_machine *)state;

    /* Resolve button */
    અચલ xkb_pointer_button_index_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;
    filter->priv = button;

    અચલ ગણના xkb_pointer_button_state button_state =
        XKB_POINTER_BUTTON_PRESSED;
    append_pointer_button(events, filter, button_state,
                          filter->action.btn.ગણતરી);
    state->flags &= ~STATE_REQUIRE_ચાવી_EVENT;
}

સ્થિર bool
xkb_filter_pointer_button_func(સંરચના xkb_server_state *state,
                               સંરચના xkb_events *events,
                               સંરચના xkb_filter *filter,
                               અચલ સંરચના xkb_ચાવી *ચાવી,
                               ગણના xkb_ચાવી_direction direction)
{
    જો (ચાવી != filter->ચાવી || !events)
        પાછા XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ચાવી_EVENT;

    પસંદગી (direction) {
    સ્થિતિ XKB_ચાવી_DOWN:
        filter->refcnt++;
        /* fallthrough */
    સ્થિતિ XKB_ચાવી_REPEATED:
        પાછા XKB_FILTER_CONSUME;
    મૂળભૂત:
        જો (--filter->refcnt > 0)
            પાછા XKB_FILTER_CONSUME;
    }

    જો (!filter->action.btn.ગણતરી)
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);

    filter->func = ખાલી;
    પાછા XKB_FILTER_CONSUME;
}

સંરચના xkb_filter_pointer_lock_button_priv {
    uint32_t button:31;
#ifdef _WIN32
    uint32_t locked:1; // MSVC requires matching types to pack bitfields
#નહીંતર
    bool locked:1;
#endif
};

ગણના {
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH =
        કદ(સંરચના xkb_filter_pointer_lock_button_priv) * CHAR_BIT,
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX =
        (UINT64_C(1) << XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH) - 1
};

static_assert(કદ(સંરચના xkb_filter_pointer_lock_button_priv) ==
              કદ(((સંરચના xkb_filter*)0)->priv),
              "");
static_assert((uint32_t)XKB_POINTER_BUTTON_MAX < (uint32_t)XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX,
              "xkb_filter_pointer_lock_button_priv cannot store button");

સ્થિર શૂન્ય
xkb_filter_pointer_lock_button_new(સંરચના xkb_server_state *state,
                                   સંરચના xkb_events *events,
                                   સંરચના xkb_filter *filter)
{
    જો (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ચાવીઓ)) {
        filter->func = ખાલી;
        પાછા;
    }

    state->flags &= ~STATE_REQUIRE_ચાવી_EVENT;

    સંરચના xkb_machine * અચલ sm = (સંરચના xkb_machine *)state;

    /* Resolve button */
    અચલ xkb_pointer_button_index_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;

    static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                  "invalid UINTx_C() macro");
    static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                  "invalid left shift");
    અચલ xkb_pointer_button_mask_t mask = (UINT8_C(1) << button);

    અચલ bool locked = (sm->mouse.locked_buttons & mask);

    જો (!locked && !(filter->action.btn.flags & ACTION_LOCK_NO_LOCK)) {
        sm->mouse.locked_buttons |= mask;
        filter->priv = button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_PRESSED, 0);
    }

    સંરચના xkb_filter_pointer_lock_button_priv *priv =
        (સંરચના xkb_filter_pointer_lock_button_priv*)&filter->priv;
    priv->button = button,
    priv->locked = locked;
}

સ્થિર bool
xkb_filter_pointer_lock_button_func(સંરચના xkb_server_state *state,
                                    સંરચના xkb_events *events,
                                    સંરચના xkb_filter *filter,
                                    અચલ સંરચના xkb_ચાવી *ચાવી,
                                    ગણના xkb_ચાવી_direction direction)
{
    જો (ચાવી != filter->ચાવી || !events || state->base.mode != SERVER_STATE)
        પાછા XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ચાવી_EVENT;

    પસંદગી (direction) {
    સ્થિતિ XKB_ચાવી_DOWN:
        filter->refcnt++;
        /* fallthrough */
    સ્થિતિ XKB_ચાવી_REPEATED:
        પાછા XKB_FILTER_CONSUME;
    મૂળભૂત:
        જો (--filter->refcnt > 0)
            પાછા XKB_FILTER_CONSUME;
    }

    અચલ સંરચના xkb_filter_pointer_lock_button_priv *priv =
        (સંરચના xkb_filter_pointer_lock_button_priv*)&filter->priv;

    જો (priv->locked && !(filter->action.btn.flags & ACTION_LOCK_NO_UNLOCK)) {
        સંરચના xkb_machine * અચલ sm = (સંરચના xkb_machine *)state;

        static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                    "invalid UINTx_C() macro");
        static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                    "invalid left shift");
        અચલ xkb_pointer_button_mask_t mask = (UINT8_C(1) << priv->button);

        sm->mouse.locked_buttons &= ~mask;
        filter->priv = priv->button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);
    }

    filter->func = ખાલી;
    પાછા XKB_FILTER_CONSUME;
}

સ્થિર શૂન્ય
xkb_filter_pointer_set_default_button_new(સંરચના xkb_server_state *state,
                                          સંરચના xkb_events *events,
                                          સંરચના xkb_filter *filter)
{
    જો (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ચાવીઓ)) {
        filter->func = ખાલી;
        પાછા;
    }

    state->flags &= ~STATE_REQUIRE_ચાવી_EVENT;

    સંરચના xkb_machine * અચલ sm = (સંરચના xkb_machine *)state;

    static_assert(2 * XKB_POINTER_BUTTON_MAX <= INT_MAX, "lossy conversion");
    પૂર્ણાંક button = (filter->action.dflt.flags & ACTION_ABSOLUTE_SWITCH)
        ? filter->action.dflt.કિંમત
        : sm->mouse.default_button + filter->action.dflt.કિંમત;

    /* Wrap into range */
    અચલ પૂર્ણાંક rem = button % XKB_POINTER_BUTTON_MAX;
    sm->mouse.default_button = (rem >= 0)
        ? (xkb_pointer_button_index_t)rem
        : (xkb_pointer_button_index_t)(rem + XKB_POINTER_BUTTON_MAX);
}

સ્થિર bool
xkb_filter_pointer_set_default_button_func(સંરચના xkb_server_state *state,
                                           સંરચના xkb_events *events,
                                           સંરચના xkb_filter *filter,
                                           અચલ સંરચના xkb_ચાવી *ચાવી,
                                           ગણના xkb_ચાવી_direction direction)
{
    જો (ચાવી != filter->ચાવી || !events)
        પાછા XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ચાવી_EVENT;

    પસંદગી (direction) {
    સ્થિતિ XKB_ચાવી_DOWN:
        filter->refcnt++;
        /* fallthrough */
    સ્થિતિ XKB_ચાવી_REPEATED:
        પાછા XKB_FILTER_CONSUME;
    મૂળભૂત:
        જો (--filter->refcnt > 0)
            પાછા XKB_FILTER_CONSUME;
    }

    filter->func = ખાલી;
    પાછા XKB_FILTER_CONTINUE;
}

સ્થિર શૂન્ય
xkb_filter_terminate_server_new(સંરચના xkb_server_state *state,
                                 સંરચના xkb_events *events,
                                 સંરચના xkb_filter *filter)
{
    જો (!events || state->base.mode != SERVER_STATE ||
        !(((સંરચના xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = ખાલી;
        પાછા;
    }

    state->flags &= ~STATE_REQUIRE_ચાવી_EVENT;

    darray_append(events->queue, (સંરચના xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    });
}

સ્થિર bool
xkb_filter_terminate_server_func(સંરચના xkb_server_state *state,
                                 સંરચના xkb_events *events,
                                 સંરચના xkb_filter *filter,
                                 અચલ સંરચના xkb_ચાવી *ચાવી,
                                 ગણના xkb_ચાવી_direction direction)
{
    જો (ચાવી != filter->ચાવી || !events)
        પાછા XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ચાવી_EVENT;

    પસંદગી (direction) {
    સ્થિતિ XKB_ચાવી_DOWN:
        filter->refcnt++;
        /* fallthrough */
    સ્થિતિ XKB_ચાવી_REPEATED:
        પાછા XKB_FILTER_CONSUME;
    મૂળભૂત:
        જો (--filter->refcnt > 0)
            પાછા XKB_FILTER_CONSUME;
    }

    filter->func = ખાલી;
    પાછા XKB_FILTER_CONSUME;
}

સ્થિર શૂન્ય
xkb_filter_switch_virtual_console_new(સંરચના xkb_server_state *state,
                                      સંરચના xkb_events *events,
                                      સંરચના xkb_filter *filter)
{
    જો (!events || state->base.mode != SERVER_STATE ||
        !(((સંરચના xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = ખાલી;
        પાછા;
    }

    state->flags &= ~STATE_REQUIRE_ચાવી_EVENT;

    darray_append(events->queue, (સંરચના xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
        .virtual_console = {
            .index_or_offset = filter->action.screen.screen,
            .is_offset = !(filter->action.screen.flags & ACTION_ABSOLUTE_SWITCH)
        }
    });
}

સ્થિર bool
xkb_filter_switch_virtual_console_func(સંરચના xkb_server_state *state,
                                       સંરચના xkb_events *events,
                                       સંરચના xkb_filter *filter,
                                       અચલ સંરચના xkb_ચાવી *ચાવી,
                                       ગણના xkb_ચાવી_direction direction)
{
    જો (ચાવી != filter->ચાવી || !events)
        પાછા XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ચાવી_EVENT;

    પસંદગી (direction) {
    સ્થિતિ XKB_ચાવી_DOWN:
        filter->refcnt++;
        /* fallthrough */
    સ્થિતિ XKB_ચાવી_REPEATED:
        પાછા XKB_FILTER_CONSUME;
    મૂળભૂત:
        જો (--filter->refcnt > 0)
            પાછા XKB_FILTER_CONSUME;
    }

    filter->func = ખાલી;
    પાછા XKB_FILTER_CONSUME;
}
