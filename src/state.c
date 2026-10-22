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

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <limits.h>
#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon-status.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon-features.h"
#അന്തര്ഭാവ "abi-check.h"
#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "features/enums.h"
#അന്തര്ഭാവ "keymap.h"
#അന്തര്ഭാവ "keysym.h"
#അന്തര്ഭാവ "messages-codes.h"
#അന്തര്ഭാവ "state-priv.h"
#അന്തര്ഭാവ "utf8.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "util-mem.h"
#അന്തര്ഭാവ "util-numbers.h"

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
ഘടന xkb_events {
    ഘടന xkb_context *ctx;
    darray(ഘടന xkb_event) queue;
    /**
     * Read cursor for `xkb_events_next()`.
     *
     * Reset to 0 on each `process_*` call.
     */
    darray_size_t next;
};

ഘടന xkb_server_state;

ഘടന xkb_filter {
    സംഘം xkb_action action;
    സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ;
    bool (*func)(ഘടന xkb_server_state *state,
                 ഘടന xkb_events *events,
                 ഘടന xkb_filter *filter,
                 സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                 എണ്ണൽ xkb_താക്കോൽ_direction direction);
    uint32_t priv;
    പൂർണ്ണസംഖ്യ refcnt;
};

എണ്ണൽ xkb_state_mode_internal {
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
    XKB_STATE_TYPE_അക്കം_ENTRIES = 5
};

static_assert(CLIENT_STATE == 0 &&
              CLIENT_STATE == SERVER_COMPANION - 1 &&
              SERVER_COMPANION == LEGACY_MIXED_STATE - 1 &&
              LEGACY_MIXED_STATE == LEGACY_SERVER_STATE - 1 &&
              LEGACY_SERVER_STATE == SERVER_STATE - 1 &&
              SERVER_STATE == XKB_STATE_TYPE_അക്കം_ENTRIES - 1 &&
              XKB_STATE_TYPE_അക്കം_ENTRIES == 5,
              "Order is used to guard client/server -only APIs: do not change");

എണ്ണൽ { XKB_LAYOUT_INDEX_T_MIN_WIDTH = 6 };
static_assert(XKB_MAX_GROUPS < (1 << XKB_LAYOUT_INDEX_T_MIN_WIDTH),
              "Cannot encode out_of_range_group::redirect_group");

ഘടന out_of_range_group {
    എണ്ണൽ xkb_layout_out_of_range_policy policy :
        (വലുപ്പം(എണ്ണൽ xkb_layout_out_of_range_policy) * CHAR_BIT -
         XKB_LAYOUT_INDEX_T_MIN_WIDTH);
    xkb_layout_index_t redirect_group : XKB_LAYOUT_INDEX_T_MIN_WIDTH;
};

എണ്ണൽ { XKB_STATE_MODE_INTERNAL_MIN_WIDTH = 4 /* 3 bits + sign */ };
static_assert(XKB_STATE_TYPE_അക്കം_ENTRIES <
              (1 << XKB_STATE_MODE_INTERNAL_MIN_WIDTH),
              "Cannot encode xkb_state::mode");

/**
 * Common state
 *
 * `mode` indicates whether the object is casted from a `xkb_server_state` or a
 * `xkb_client_state` struct.
 */
ഘടന xkb_state {
    ഘടന xkb_keymap *keymap;
    /*
     * Before updating the state, we keep a copy of just this struct. This
     * allows us to report which components of the state have changed.
     */
    ഘടന state_components components;
    ഘടന out_of_range_group out_of_range_group;

    എണ്ണൽ xkb_state_mode_internal mode : XKB_STATE_MODE_INTERNAL_MIN_WIDTH;
    പൂർണ്ണസംഖ്യ refcnt : (വലുപ്പം(പൂർണ്ണസംഖ്യ) * CHAR_BIT - XKB_STATE_MODE_INTERNAL_MIN_WIDTH);
};

/**
 * Minimal client state
 *
 * Wrapper for strong typing
 */
ഘടന xkb_client_state {
    /** Inherits from `xkb_state` */
    ഘടന xkb_state base;
};

എണ്ണൽ state_flags {
    STATE_NO_FLAGS = 0,
    STATE_REQUIRE_താക്കോൽ_EVENT = (1 << 0),
    STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_താക്കോൽ = (1 << 1),
    STATE_UNLOCK_MODS_ON_PRESS = (1 << 2),
};

/**
 * Legacy server state, used for `LEGACY_MIXED_STATE` and `LEGACY_SERVER_STATE`
 * modes.
 *
 * Casted as `xkb_state` in the public API. See `xkb_state::mode`.
 */
ഘടന xkb_server_state {
    /** Inherits from `xkb_state` */
    ഘടന xkb_state base;

    /* Server-specific state data */

    darray(ഘടന xkb_filter) filters;

    /**
     * Mix of constant configuration flags and
     * variable state-processing flags.
     */
    എണ്ണൽ state_flags flags;

    എണ്ണൽ xkb_a11y_flags a11y_flags;

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
    int16_t mod_താക്കോൽ_എണ്ണം[XKB_MAX_MODS];
};

സ്ഥിതം സ്ഥിരം ഘടന xkb_താക്കോൽ_type_entry *
get_entry_for_mods(സ്ഥിരം ഘടന xkb_താക്കോൽ_type *type, xkb_mod_mask_t mods)
{
    വേണ്ടി (darray_size_t i = 0; i < type->അക്കം_entries; i++)
        എങ്കിൽ (entry_is_active(&type->entries[i]) &&
            type->entries[i].mods.mask == mods)
            തിരികെ &type->entries[i];
    തിരികെ ശൂന്യ;
}

സ്ഥിതം സ്ഥിരം ഘടന xkb_താക്കോൽ_type_entry *
get_entry_for_താക്കോൽ_state(ഘടന xkb_state *state, സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                        xkb_layout_index_t group)
{
    സ്ഥിരം ഘടന xkb_താക്കോൽ_type* സ്ഥിരം type = താക്കോൽ->groups[group].type;
    xkb_mod_mask_t active_mods = state->components.mods & type->mods.mask;
    തിരികെ get_entry_for_mods(type, active_mods);
}

സ്ഥിതം വരിയിൽ xkb_level_index_t
state_താക്കോൽ_get_level(ഘടന xkb_state *state, സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                         xkb_layout_index_t layout)
{
    എങ്കിൽ (layout >= താക്കോൽ->അക്കം_groups)
        തിരികെ XKB_LEVEL_INVALID;

    /* If we don't find an explicit match the default is 0. */
    സ്ഥിരം ഘടന xkb_താക്കോൽ_type_entry* സ്ഥിരം entry =
        get_entry_for_താക്കോൽ_state(state, താക്കോൽ, layout);

    തിരികെ (entry) ? entry->level : 0;
}

/**
 * Returns the level to use for the given key and state, or
 * XKB_LEVEL_INVALID.
 */
xkb_level_index_t
xkb_state_താക്കോൽ_get_level(ഘടന xkb_state *state, xkb_keycode_t kc,
                        xkb_layout_index_t layout)
{
    സ്ഥിരം ഘടന xkb_താക്കോൽ* സ്ഥിരം താക്കോൽ = Xkb_താക്കോൽ(state->keymap, kc);

    തിരികെ (താക്കോൽ)
        ? state_താക്കോൽ_get_level(state, താക്കോൽ, layout)
        : XKB_LEVEL_INVALID;
}

സ്ഥിതം വരിയിൽ xkb_layout_index_t
state_താക്കോൽ_get_layout(ഘടന xkb_state *state, സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ)
{
    തിരികെ xkb_keymap_താക്കോൽ_effective_layout(താക്കോൽ, state->components.group);
}

/**
 * Returns the layout to use for the given key and state, taking
 * wrapping/clamping/etc into account, or XKB_LAYOUT_INVALID.
 */
xkb_layout_index_t
xkb_state_താക്കോൽ_get_layout(ഘടന xkb_state *state, xkb_keycode_t kc)
{
    സ്ഥിരം ഘടന xkb_താക്കോൽ* സ്ഥിരം താക്കോൽ = Xkb_താക്കോൽ(state->keymap, kc);

    എങ്കിൽ (!താക്കോൽ)
        തിരികെ XKB_LAYOUT_INVALID;

    തിരികെ state_താക്കോൽ_get_layout(state, താക്കോൽ);
}

/* Empty action used for empty levels */
സ്ഥിതം സ്ഥിരം സംഘം xkb_action dummy_action = { .type = ACTION_TYPE_NONE };

സ്ഥിതം xkb_action_എണ്ണം_t
xkb_താക്കോൽ_get_actions(ഘടന xkb_state *state, സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                    സ്ഥിരം സംഘം xkb_action **actions)
{
    സ്ഥിരം xkb_layout_index_t layout = state_താക്കോൽ_get_layout(state, താക്കോൽ);
    സ്ഥിരം xkb_level_index_t level = state_താക്കോൽ_get_level(state, താക്കോൽ, layout);
    എങ്കിൽ (level == XKB_LEVEL_INVALID)
        പോകുക err;

    സ്ഥിരം xkb_action_എണ്ണം_t എണ്ണം =
        xkb_keymap_താക്കോൽ_get_actions_by_level(state->keymap, താക്കോൽ,
                                            layout, level, actions);
    എങ്കിൽ (!എണ്ണം)
        പോകുക err;

    തിരികെ എണ്ണം;

err:
    /* Use a dummy action if no corresponding level was found or if it is empty.
     * This is required e.g. to handle latches properly. */
    *actions = &dummy_action;
    തിരികെ 1;
}

സ്ഥിതം ഘടന xkb_filter *
xkb_filter_new(ഘടന xkb_server_state *state)
{
    ഘടന xkb_filter *filter = ശൂന്യ, *iter;

    darray_foreach(iter, state->filters) {
        എങ്കിൽ (iter->func)
            തുടരുക;
        /* Use available slot */
        filter = iter;
        നിർത്തുക;
    }

    എങ്കിൽ (!filter) {
        /* No available slot: resize the filters array */
        darray_resize0(state->filters, darray_size(state->filters) + 1);
        filter = &darray_item(state->filters, darray_size(state->filters) -1);
    }

    filter->refcnt = 1;
    തിരികെ filter;
}

/***====================================================================***/

എണ്ണൽ xkb_filter_result {
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
    എങ്കിൽ ((filter_)->action.group.flags & ACTION_ABSOLUTE_SWITCH)          \
        (state_)->components.component_ = (filter_)->action.group.group; \
    അല്ലെങ്കിൽ                                                                 \
        (state_)->components.component_ += (filter_)->action.group.group

സ്ഥിതം ശൂന്യം
xkb_filter_group_set_new(ഘടന xkb_server_state *state,
                         ഘടന xkb_events *events,
                         ഘടന xkb_filter *filter)
{
    static_assert(വലുപ്പം(state->base.components.base_group) ==
                  വലുപ്പം(filter->priv),
                  "Max groups don't fit");
    filter->priv = (uint32_t)state->base.components.base_group;
    apply_group_delta(filter, &state->base, base_group);
}

സ്ഥിതം bool
xkb_filter_group_set_func(ഘടന xkb_server_state *state,
                          ഘടന xkb_events *events,
                          ഘടന xkb_filter *filter,
                          സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                          എണ്ണൽ xkb_താക്കോൽ_direction direction)
{
    എങ്കിൽ (താക്കോൽ != filter->താക്കോൽ) {
        filter->action.group.flags &= ~ACTION_LOCK_CLEAR;
        തിരികെ XKB_FILTER_CONTINUE;
    }

    തിരഞ്ഞെടുപ്പ് (direction) {
    അവസ്ഥ XKB_താക്കോൽ_DOWN:
        filter->refcnt++;
        /* fallthrough */
    അവസ്ഥ XKB_താക്കോൽ_REPEATED:
        തിരികെ XKB_FILTER_CONSUME;
    സ്ഥിരസ്ഥിതി:
        എങ്കിൽ (--filter->refcnt > 0)
            തിരികെ XKB_FILTER_CONSUME;
    }

    static_assert(വലുപ്പം(state->base.components.base_group) == വലുപ്പം(filter->priv),
                  "Max groups don't fit");
    state->base.components.base_group = (int32_t) filter->priv;

    എങ്കിൽ (filter->action.group.flags & ACTION_LOCK_CLEAR)
        state->base.components.locked_group = 0;

    filter->func = ശൂന്യ;
    തിരികെ XKB_FILTER_CONTINUE;
}

സ്ഥിതം എണ്ണൽ xkb_state_component
get_state_component_changes(സ്ഥിരം ഘടന state_components *a,
                            സ്ഥിരം ഘടന state_components *b)
{
    എണ്ണൽ xkb_state_component mask = 0;

    എങ്കിൽ (a->group != b->group)
        mask |= XKB_STATE_LAYOUT_EFFECTIVE;
    എങ്കിൽ (a->base_group != b->base_group)
        mask |= XKB_STATE_LAYOUT_DEPRESSED;
    എങ്കിൽ (a->latched_group != b->latched_group)
        mask |= XKB_STATE_LAYOUT_LATCHED;
    എങ്കിൽ (a->locked_group != b->locked_group)
        mask |= XKB_STATE_LAYOUT_LOCKED;
    എങ്കിൽ (a->mods != b->mods)
        mask |= XKB_STATE_MODS_EFFECTIVE;
    എങ്കിൽ (a->base_mods != b->base_mods)
        mask |= XKB_STATE_MODS_DEPRESSED;
    എങ്കിൽ (a->latched_mods != b->latched_mods)
        mask |= XKB_STATE_MODS_LATCHED;
    എങ്കിൽ (a->locked_mods != b->locked_mods)
        mask |= XKB_STATE_MODS_LOCKED;
    എങ്കിൽ (a->leds != b->leds)
        mask |= XKB_STATE_LEDS;
    എങ്കിൽ (a->controls != b->controls)
        mask |= XKB_STATE_CONTROLS_EFFECTIVE;

    തിരികെ mask;
}

സ്ഥിതം ശൂന്യം
xkb_filter_group_lock_new(ഘടന xkb_server_state *state,
                          ഘടന xkb_events *events,
                          ഘടന xkb_filter *filter)
{
    എങ്കിൽ (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release: do nothing on key press.
         *
         * This is a keymap format v2 extension.
         */
        തിരികെ;
    } അല്ലെങ്കിൽ {
        /* Lock on key press */
        apply_group_delta(filter, &state->base, locked_group);
    }
}

സ്ഥിതം bool
xkb_filter_group_lock_func(ഘടന xkb_server_state *state,
                           ഘടന xkb_events *events,
                           ഘടന xkb_filter *filter,
                           സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                           എണ്ണൽ xkb_താക്കോൽ_direction direction)
{
    എങ്കിൽ (താക്കോൽ != filter->താക്കോൽ) {
        എങ്കിൽ ((filter->action.group.flags & ACTION_LOCK_ON_RELEASE) &&
            direction == XKB_താക്കോൽ_DOWN) {
            /*
             * Another key has been pressed after the locking key:
             * cancel group lock on release.
             *
             * This is a keymap v2 extension.
             */
            filter->action.group.flags &= ~ACTION_LOCK_ON_RELEASE;
        }
        തിരികെ XKB_FILTER_CONTINUE;
    }

    തിരഞ്ഞെടുപ്പ് (direction) {
    അവസ്ഥ XKB_താക്കോൽ_DOWN:
        filter->refcnt++;
        /* fallthrough */
    അവസ്ഥ XKB_താക്കോൽ_REPEATED:
        തിരികെ XKB_FILTER_CONSUME;
    സ്ഥിരസ്ഥിതി:
        എങ്കിൽ (--filter->refcnt > 0)
            തിരികെ XKB_FILTER_CONSUME;
    }

    എങ്കിൽ (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release
         *
         * This is a keymap v2 extension.
         */
        apply_group_delta(filter, &state->base, locked_group);
    } അല്ലെങ്കിൽ {
        /* Lock on key press: do nothing on key release. */
    }
    filter->func = ശൂന്യ;
    തിരികെ XKB_FILTER_CONTINUE;
}

സ്ഥിതം bool
xkb_action_breaks_latch(സ്ഥിരം സംഘം xkb_action *action,
                        എണ്ണൽ xkb_action_controls controls,
                        എണ്ണൽ xkb_internal_action_flags flag,
                        xkb_mod_mask_t mask)
{
    തിരഞ്ഞെടുപ്പ് (action->type) {
    അവസ്ഥ ACTION_TYPE_NONE:
    അവസ്ഥ ACTION_TYPE_VOID:
    അവസ്ഥ ACTION_TYPE_PTR_BUTTON:
    അവസ്ഥ ACTION_TYPE_PTR_LOCK:
    അവസ്ഥ ACTION_TYPE_CTRL_SET:
    അവസ്ഥ ACTION_TYPE_CTRL_LOCK:
    അവസ്ഥ ACTION_TYPE_SWITCH_VT:
    അവസ്ഥ ACTION_TYPE_TERMINATE:
    അവസ്ഥ ACTION_TYPE_REDIRECT_താക്കോൽ:
        തിരികെ true;
    അവസ്ഥ ACTION_TYPE_PTR_MOVE:
    അവസ്ഥ ACTION_TYPE_PTR_DEFAULT:
        തിരികെ !(controls & CONTROL_MOUSE_താക്കോലുകൾ);
    അവസ്ഥ ACTION_TYPE_INTERNAL:
        തിരികെ (action->internal.flags & flag) &&
               ((action->internal.clear_latched_mods & mask) == mask);
    സ്ഥിരസ്ഥിതി:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_അക്കം_ENTRIES,
                      "Missing action type");
        തിരികെ false;
    }
}

എണ്ണൽ xkb_താക്കോൽ_latch_state {
    NO_LATCH = 0,
    LATCH_താക്കോൽ_DOWN,
    LATCH_PENDING,
    താക്കോൽ_LATCH_STATE_അക്കം_ENTRIES
};

#define MAX_XKB_താക്കോൽ_LATCH_STATE_LOG2 2
#എങ്കിൽ (താക്കോൽ_LATCH_STATE_അക്കം_ENTRIES > (1 << MAX_XKB_താക്കോൽ_LATCH_STATE_LOG2)) || \
    (-XKB_MAX_GROUPS) < (INT32_MIN >> MAX_XKB_താക്കോൽ_LATCH_STATE_LOG2) || \
      XKB_MAX_GROUPS > (INT32_MAX >> MAX_XKB_താക്കോൽ_LATCH_STATE_LOG2)
#error "Cannot represent priv field of the group latch filter"
#endif

/* Hold the latch state *and* the group delta */
സംഘം group_latch_priv {
    uint32_t priv;
    ഘടന {
        /* The type is really: enum xkb_key_latch_state, but it is problematic
         * on Windows, because it is interpreted as signed and leads to wrong
         * negative values. */
        unsigned പൂർണ്ണസംഖ്യ latch:MAX_XKB_താക്കോൽ_LATCH_STATE_LOG2;
        int32_t group_delta:(32 - MAX_XKB_താക്കോൽ_LATCH_STATE_LOG2);
    };
};

സ്ഥിതം ശൂന്യം
xkb_filter_group_latch_new(ഘടന xkb_server_state *state,
                           ഘടന xkb_events *events,
                           ഘടന xkb_filter *filter)
{
    സ്ഥിരം സംഘം group_latch_priv priv = {
        .latch = LATCH_താക്കോൽ_DOWN,
        .group_delta = (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)
            ? filter->action.group.group - state->base.components.base_group
            : filter->action.group.group
    };
    filter->priv = priv.priv;
    /* Like group set */
    apply_group_delta(filter, &state->base, base_group);
}

സ്ഥിതം bool
xkb_filter_group_latch_func(ഘടന xkb_server_state *state,
                            ഘടന xkb_events *events,
                            ഘടന xkb_filter *filter,
                            സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                            എണ്ണൽ xkb_താക്കോൽ_direction direction)
{
    സംഘം group_latch_priv priv = {.priv = filter->priv};
    എണ്ണൽ xkb_താക്കോൽ_latch_state latch = priv.latch;

    എങ്കിൽ (direction == XKB_താക്കോൽ_DOWN) {
        സ്ഥിരം സംഘം xkb_action *actions = ശൂന്യ;
        സ്ഥിരം xkb_action_എണ്ണം_t എണ്ണം = xkb_താക്കോൽ_get_actions(&state->base, താക്കോൽ,
                                                             &actions);

        എങ്കിൽ (latch == LATCH_താക്കോൽ_DOWN) {
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
            എങ്കിൽ (state->a11y_flags & XKB_A11_Y_LATCH_SIMULTANEOUS_താക്കോലുകൾ) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                വേണ്ടി (xkb_action_എണ്ണം_t k = 0; k < എണ്ണം; k++) {
                    എങ്കിൽ (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_GROUP_LATCH,
                                                0))
                    {
                        latch = NO_LATCH;
                        നിർത്തുക;
                    }
                }
            } അല്ലെങ്കിൽ {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        അല്ലെങ്കിൽ എങ്കിൽ (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is
             * pressed, or promote it to a lock if it's the same group delta &
             * flags and latchToLock option is enabled. */
            സ്ഥിരം bool sticky_താക്കോലുകൾ = state->base.components.controls
                                   & CONTROL_STICKY_താക്കോലുകൾ;
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
            വേണ്ടി (xkb_action_എണ്ണം_t k = 0; k < എണ്ണം; k++) {
                എങ്കിൽ (((actions[k].type == ACTION_TYPE_GROUP_LATCH) ||
                     (actions[k].type == ACTION_TYPE_GROUP_SET && sticky_താക്കോലുകൾ)) &&
                     actions[k].group.group == filter->action.group.group &&
                     (actions[k].group.flags & ACTION_ABSOLUTE_SWITCH) ==
                     (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)) {
                    എങ്കിൽ (filter->action.group.flags & ACTION_LATCH_TO_LOCK &&
                        filter->action.group.group != 0)
                    {
                        /* Promote to lock */
                        filter->action.type = ACTION_TYPE_GROUP_LOCK;
                        filter->func = xkb_filter_group_lock_func;
                        xkb_filter_group_lock_new(state, events, filter);
                        state->base.components.latched_group -= priv.group_delta;
                        filter->താക്കോൽ = താക്കോൽ;
                        /* XXX beep beep! */
                        തിരികെ XKB_FILTER_CONSUME;
                    }
                    /* Do nothing if `latchToLock` option is not activated; if
                     * the latch is not broken by the following actions and the
                     * key is not consumed, then another latch filter will be
                     * created.
                     */
                    തുടരുക;
                }
                അല്ലെങ്കിൽ എങ്കിൽ (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_GROUP_LATCH,
                                                 0))
                {
                    /* Breaks the latch */
                    state->base.components.latched_group -= priv.group_delta;
                    filter->func = ശൂന്യ;
                    തിരികെ XKB_FILTER_CONTINUE;
                }
            }
        }
        അല്ലെങ്കിൽ {
            /* Ignore press in NO_LATCH state */
            പ്രതിജ്ഞാ(latch == NO_LATCH);
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ (താക്കോൽ != filter->താക്കോൽ) {
        /* Ignore repeat/release of other keys */
    }
    അല്ലെങ്കിൽ എങ്കിൽ (direction == XKB_താക്കോൽ_REPEATED) {
        തിരികെ XKB_FILTER_CONSUME; // TODO: check
    }
    അല്ലെങ്കിൽ {
        /* Our key got released.  If we've set it to clear locks, and we
         * currently have a group locked, then release it and
         * don't actually latch.  Else we've actually hit the latching
         * stage, so set PENDING and move our group from base to
         * latched. */
        എങ്കിൽ ((filter->action.group.flags & ACTION_LOCK_CLEAR) &&
             state->base.components.locked_group) {
            എങ്കിൽ (latch == LATCH_PENDING)
                state->base.components.latched_group -= priv.group_delta;
            അല്ലെങ്കിൽ
                state->base.components.base_group -= priv.group_delta;
            state->base.components.locked_group = 0;
            filter->func = ശൂന്യ;
        }
        /* Broken latch */
        അല്ലെങ്കിൽ എങ്കിൽ (latch == NO_LATCH) {
            state->base.components.base_group -= priv.group_delta;
            filter->func = ശൂന്യ;
        }
        /* We may already have reached the latch state if pressing the
         * key multiple times without latch-to-lock enabled. */
        അല്ലെങ്കിൽ എങ്കിൽ (latch == LATCH_താക്കോൽ_DOWN) {
            latch = LATCH_PENDING;
            /* Switch from set to latch */
            state->base.components.base_group -= priv.group_delta;
            state->base.components.latched_group += priv.group_delta;
            /* XXX beep beep! */
        }
    }

    priv.latch = latch;
    filter->priv = priv.priv;

    തിരികെ XKB_FILTER_CONTINUE;
}

സ്ഥിതം ശൂന്യം
xkb_filter_mod_set_new(ഘടന xkb_server_state *state,
                       ഘടന xkb_events *events,
                       ഘടന xkb_filter *filter)
{
    സ്ഥിരം എണ്ണൽ xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    എങ്കിൽ ((filter->action.mods.flags & unlock) == unlock) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension.
         */
        filter->priv = filter->action.mods.mods.mask
                     & ~state->base.components.locked_mods;
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
    } അല്ലെങ്കിൽ {
        filter->priv = filter->action.mods.mods.mask;
    }

    state->set_mods |= (xkb_mod_mask_t) filter->priv;
}

സ്ഥിതം bool
xkb_filter_mod_set_func(ഘടന xkb_server_state *state,
                        ഘടന xkb_events *events,
                        ഘടന xkb_filter *filter,
                        സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                        എണ്ണൽ xkb_താക്കോൽ_direction direction)
{

    എങ്കിൽ (താക്കോൽ != filter->താക്കോൽ) {
        filter->action.mods.flags &= ~ACTION_LOCK_CLEAR;
        തിരികെ XKB_FILTER_CONTINUE;
    }

    തിരഞ്ഞെടുപ്പ് (direction) {
    അവസ്ഥ XKB_താക്കോൽ_DOWN:
        filter->refcnt++;
        /* fallthrough */
    അവസ്ഥ XKB_താക്കോൽ_REPEATED:
        തിരികെ XKB_FILTER_CONSUME;
    സ്ഥിരസ്ഥിതി:
        എങ്കിൽ (--filter->refcnt > 0)
            തിരികെ XKB_FILTER_CONSUME;
    }

    state->clear_mods |= (xkb_mod_mask_t) filter->priv;
    സ്ഥിരം എണ്ണൽ xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    എങ്കിൽ ((filter->action.mods.flags & unlock) == ACTION_LOCK_CLEAR)
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;

    filter->func = ശൂന്യ;
    തിരികെ XKB_FILTER_CONTINUE;
}

സ്ഥിതം ശൂന്യം
xkb_filter_mod_lock_new(ഘടന xkb_server_state *state,
                        ഘടന xkb_events *events,
                        ഘടന xkb_filter *filter)
{
    filter->priv = (state->base.components.locked_mods &
                    filter->action.mods.mods.mask);

    എങ്കിൽ (filter->priv && (filter->action.mods.flags & ACTION_UNLOCK_ON_PRESS)) {
        /*
         * Some of the target modifiers were locked before key press: unlock.
         *
         * This is a keymap v2 extension: unlock-on-press.
         */
        എങ്കിൽ (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
            state->base.components.locked_mods &= ~filter->priv;
        /* No further action: cancel filter */
        filter->func = ശൂന്യ;
    } അല്ലെങ്കിൽ {
        /* Set base mods; lock mods if relevant (XKB 1.0 spec) */
        state->set_mods |= filter->action.mods.mods.mask;
        എങ്കിൽ (!(filter->action.mods.flags & ACTION_LOCK_NO_LOCK))
            state->base.components.locked_mods |= filter->action.mods.mods.mask;
    }
}

സ്ഥിതം bool
xkb_filter_mod_lock_func(ഘടന xkb_server_state *state,
                         ഘടന xkb_events *events,
                         ഘടന xkb_filter *filter,
                         സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                         എണ്ണൽ xkb_താക്കോൽ_direction direction)
{
    എങ്കിൽ (താക്കോൽ != filter->താക്കോൽ)
        തിരികെ XKB_FILTER_CONTINUE;

    തിരഞ്ഞെടുപ്പ് (direction) {
    അവസ്ഥ XKB_താക്കോൽ_DOWN:
        filter->refcnt++;
        /* fallthrough */
    അവസ്ഥ XKB_താക്കോൽ_REPEATED:
        തിരികെ XKB_FILTER_CONSUME;
    സ്ഥിരസ്ഥിതി:
        എങ്കിൽ (--filter->refcnt > 0)
            തിരികെ XKB_FILTER_CONSUME;
    }

    state->clear_mods |= filter->action.mods.mods.mask;
    എങ്കിൽ (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
        state->base.components.locked_mods &= ~filter->priv;

    filter->func = ശൂന്യ;
    തിരികെ XKB_FILTER_CONTINUE;
}

സ്ഥിതം ശൂന്യം
xkb_filter_mod_latch_new(ഘടന xkb_server_state *state,
                         ഘടന xkb_events *events,
                         ഘടന xkb_filter *filter)
{
    /* Latch-on-press + clear-locks imply unlock-on-press */
    സ്ഥിരം എണ്ണൽ xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                              | ACTION_LATCH_ON_PRESS;

    എങ്കിൽ ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
        (filter->action.mods.flags & unlockOnPress) &&
        (state->base.components.locked_mods & filter->action.mods.mods.mask) ==
         filter->action.mods.mods.mask) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension: clear locks and do not latch.
         */
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
        filter->func = ശൂന്യ;
    } അല്ലെങ്കിൽ എങ്കിൽ (filter->action.mods.flags & ACTION_LATCH_ON_PRESS) {
        /*
         * Latch on key press
         *
         * This is a keymap format v2 extension.
         */
        filter->priv = LATCH_PENDING;
        state->base.components.latched_mods |= filter->action.mods.mods.mask;
        /* XXX beep beep! */
    } അല്ലെങ്കിൽ {
        /* XKB standard latch action */
        filter->priv = LATCH_താക്കോൽ_DOWN;
        state->set_mods |= filter->action.mods.mods.mask;
    }
}

സ്ഥിതം bool
xkb_filter_mod_latch_func(ഘടന xkb_server_state *state,
                          ഘടന xkb_events *events,
                          ഘടന xkb_filter *filter,
                          സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                          എണ്ണൽ xkb_താക്കോൽ_direction direction)
{
    എണ്ണൽ xkb_താക്കോൽ_latch_state latch = filter->priv;

    എങ്കിൽ (direction == XKB_താക്കോൽ_DOWN) {
        സ്ഥിരം സംഘം xkb_action *actions = ശൂന്യ;
        സ്ഥിരം xkb_action_എണ്ണം_t എണ്ണം = xkb_താക്കോൽ_get_actions(&state->base, താക്കോൽ,
                                                             &actions);

        എങ്കിൽ (latch == LATCH_താക്കോൽ_DOWN) {
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
            എങ്കിൽ (state->a11y_flags & XKB_A11_Y_LATCH_SIMULTANEOUS_താക്കോലുകൾ) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                വേണ്ടി (xkb_action_എണ്ണം_t k = 0; k < എണ്ണം; k++) {
                    എങ്കിൽ (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_MOD_LATCH,
                                                filter->action.mods.mods.mask))
                    {
                        latch = NO_LATCH;
                        നിർത്തുക;
                    }
                }
            } അല്ലെങ്കിൽ {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        അല്ലെങ്കിൽ എങ്കിൽ (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is pressed,
             * or promote it to a lock or plain base set if it's the same
             * modifier. */
            സ്ഥിരം bool sticky_താക്കോലുകൾ = state->base.components.controls
                                   & CONTROL_STICKY_താക്കോലുകൾ;
            /*
             * NOTE: Action’s flags are not compared:
             * - flags may have been modified by keyboard controls at the
             *   creation of the filter;
             * - it is unlikely to have two LatchMods() actions on the same
             *   key that differ only by their flags;
             * - even if it were the case, one would most likely want both
             *   actions to be considered compatible in this context.
             */
            വേണ്ടി (xkb_action_എണ്ണം_t k = 0; k < എണ്ണം; k++) {
                എങ്കിൽ (((actions[k].type == ACTION_TYPE_MOD_LATCH) ||
                     (actions[k].type == ACTION_TYPE_MOD_SET && sticky_താക്കോലുകൾ)) &&
                    actions[k].mods.mods.mask == filter->action.mods.mods.mask)
                {
                    എങ്കിൽ (filter->action.mods.flags & ACTION_LATCH_TO_LOCK) {
                        /* Mutate the action to LockMods() */
                        filter->action.type = ACTION_TYPE_MOD_LOCK;
                        filter->func = xkb_filter_mod_lock_func;
                        xkb_filter_mod_lock_new(state, events, filter);
                    }
                    അല്ലെങ്കിൽ {
                        /* Mutate the action to SetMods() */
                        filter->action.type = ACTION_TYPE_MOD_SET;
                        filter->func = xkb_filter_mod_set_func;
                        xkb_filter_mod_set_new(state, events, filter);
                    }
                    filter->താക്കോൽ = താക്കോൽ;
                    /* Clear latches */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    /* XXX beep beep! */
                    തിരികെ XKB_FILTER_CONSUME;
                }
                അല്ലെങ്കിൽ എങ്കിൽ (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_MOD_LATCH,
                                                 filter->action.mods.mods.mask))
                {
                    /* XXX: This may be totally broken, we might need to break the
                     *      latch in the next run after this press? */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    filter->func = ശൂന്യ;
                    തിരികെ XKB_FILTER_CONTINUE;
                }
            }
        }
        അല്ലെങ്കിൽ {
            /* Ignore press in NO_LATCH state */
            പ്രതിജ്ഞാ(latch == NO_LATCH);
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ (താക്കോൽ != filter->താക്കോൽ) {
        /* Ignore repeat/release of other keys */
    }
    അല്ലെങ്കിൽ എങ്കിൽ (direction == XKB_താക്കോൽ_REPEATED) {
        തിരികെ XKB_FILTER_CONSUME; // TODO: check
    }
    അല്ലെങ്കിൽ {
        /* Our key got released.  If we’ve set it to clear locks, and we
         * currently have the same modifiers locked, then release them and
         * don't actually latch.  Else we’ve actually hit the latching
         * stage, so set PENDING and move our modifier from base to
         * latched. */

        /* Latch-on-press + clear-locks imply unlock-on-press */
        സ്ഥിരം എണ്ണൽ xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                                  | ACTION_LATCH_ON_PRESS;

        എങ്കിൽ ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
            !(filter->action.mods.flags & unlockOnPress) &&
            (state->base.components.locked_mods &
             filter->action.mods.mods.mask) == filter->action.mods.mods.mask)
        {
            /* XXX: We might be a bit overenthusiastic about clearing
             *      mods other filters have set here? */
            എങ്കിൽ (latch == LATCH_PENDING)
                state->base.components.latched_mods &=
                    ~filter->action.mods.mods.mask;
            അല്ലെങ്കിൽ
                state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
            filter->func = ശൂന്യ;
        }
        അല്ലെങ്കിൽ എങ്കിൽ (latch == NO_LATCH) {
            /* Broken latch */
            state->clear_mods |= filter->action.mods.mods.mask;
            filter->func = ശൂന്യ;
        }
        അല്ലെങ്കിൽ എങ്കിൽ (!(filter->action.mods.flags & ACTION_LATCH_ON_PRESS)) {
            latch = LATCH_PENDING;
            state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.latched_mods |= filter->action.mods.mods.mask;
            /* XXX beep beep! */
        }
    }

    filter->priv = latch;

    തിരികെ XKB_FILTER_CONTINUE;
}

സ്ഥിതം ശൂന്യം
append_pointer_move(ഘടന xkb_events *events,
                    ഘടന xkb_filter *filter)
{
    എണ്ണൽ xkb_pointer_motion_flags flags = 0;
    എങ്കിൽ (filter->action.ptr.flags & ACTION_ABSOLUTE_X)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_X;
    എങ്കിൽ (filter->action.ptr.flags & ACTION_ABSOLUTE_Y)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_Y;
    എങ്കിൽ (filter->action.ptr.flags & ACTION_REPEAT)
        flags |= XKB_POINTER_MOTION_REPEATS;
    darray_append(events->queue, (ഘടന xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_MOTION,
        .pointer_motion = {
            .size = വലുപ്പം(((ഘടന xkb_event*)0)->pointer_motion),
            .flags = flags,
            .x = filter->action.ptr.x,
            .y = filter->action.ptr.y,
        }
    });
}

സ്ഥിതം ശൂന്യം
xkb_filter_pointer_move_new(ഘടന xkb_server_state *state,
                            ഘടന xkb_events *events,
                            ഘടന xkb_filter *filter)
{
    എങ്കിൽ (!events) {
        filter->func = ശൂന്യ;
        തിരികെ;
    }

    പ്രതിജ്ഞാ(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_താക്കോലുകൾ);
    append_pointer_move(events, filter);
    state->flags &= ~STATE_REQUIRE_താക്കോൽ_EVENT;
}

സ്ഥിതം bool
xkb_filter_pointer_move_func(ഘടന xkb_server_state *state,
                             ഘടന xkb_events *events,
                             ഘടന xkb_filter *filter,
                             സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                             എണ്ണൽ xkb_താക്കോൽ_direction direction)
{
    എങ്കിൽ (താക്കോൽ != filter->താക്കോൽ || !events)
        തിരികെ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_താക്കോൽ_EVENT;

    തിരഞ്ഞെടുപ്പ് (direction) {
    അവസ്ഥ XKB_താക്കോൽ_DOWN:
        filter->refcnt++;
        append_pointer_move(events, filter);
        തിരികെ XKB_FILTER_CONSUME;
    അവസ്ഥ XKB_താക്കോൽ_REPEATED:
        എങ്കിൽ (filter->action.ptr.flags & ACTION_REPEAT)
            append_pointer_move(events, filter);
        തിരികെ XKB_FILTER_CONSUME;
    സ്ഥിരസ്ഥിതി:
        എങ്കിൽ (--filter->refcnt > 0)
            തിരികെ XKB_FILTER_CONSUME;
    }

    filter->func = ശൂന്യ;
    തിരികെ XKB_FILTER_CONSUME;
}

/* Implemented with xkb_machine API because it requires struct xkb_machine */
സ്ഥിതം ശൂന്യം
xkb_filter_pointer_button_new(ഘടന xkb_server_state *state,
                              ഘടന xkb_events *events,
                              ഘടന xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
സ്ഥിതം bool
xkb_filter_pointer_button_func(ഘടന xkb_server_state *state,
                               ഘടന xkb_events *events,
                               ഘടന xkb_filter *filter,
                               സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                               എണ്ണൽ xkb_താക്കോൽ_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
സ്ഥിതം ശൂന്യം
xkb_filter_pointer_lock_button_new(ഘടന xkb_server_state *state,
                                   ഘടന xkb_events *events,
                                   ഘടന xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
സ്ഥിതം bool
xkb_filter_pointer_lock_button_func(ഘടന xkb_server_state *state,
                                    ഘടന xkb_events *events,
                                    ഘടന xkb_filter *filter,
                                    സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                                    എണ്ണൽ xkb_താക്കോൽ_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
സ്ഥിതം ശൂന്യം
xkb_filter_pointer_set_default_button_new(ഘടന xkb_server_state *state,
                                          ഘടന xkb_events *events,
                                          ഘടന xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
സ്ഥിതം bool
xkb_filter_pointer_set_default_button_func(ഘടന xkb_server_state *state,
                                           ഘടന xkb_events *events,
                                           ഘടന xkb_filter *filter,
                                           സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                                           എണ്ണൽ xkb_താക്കോൽ_direction direction);

സ്ഥിതം ശൂന്യം
xkb_filter_terminate_server_new(ഘടന xkb_server_state *state,
                                 ഘടന xkb_events *events,
                                 ഘടന xkb_filter *filter);

സ്ഥിതം bool
xkb_filter_terminate_server_func(ഘടന xkb_server_state *state,
                                  ഘടന xkb_events *events,
                                  ഘടന xkb_filter *filter,
                                  സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                                  എണ്ണൽ xkb_താക്കോൽ_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
സ്ഥിതം ശൂന്യം
xkb_filter_switch_virtual_console_new(ഘടന xkb_server_state *state,
                                      ഘടന xkb_events *events,
                                      ഘടന xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
സ്ഥിതം bool
xkb_filter_switch_virtual_console_func(ഘടന xkb_server_state *state,
                                       ഘടന xkb_events *events,
                                       ഘടന xkb_filter *filter,
                                       സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                                       എണ്ണൽ xkb_താക്കോൽ_direction direction);

സ്ഥിതം വരിയിൽ ശൂന്യം
clear_all_latches_and_locks(ഘടന xkb_server_state *state,
                            ഘടന xkb_events *events);

സ്ഥിതം ശൂന്യം
xkb_filter_ctrls_new(ഘടന xkb_server_state *state,
                     ഘടന xkb_events *events,
                     ഘടന xkb_filter *filter)
{
    എങ്കിൽ (filter->action.type == ACTION_TYPE_CTRL_SET) {
        /* Set: save the specified controls that are *not* already enabled */
        filter->priv = (uint32_t) (
            (~state->base.components.controls) & filter->action.ctrls.ctrls
        );
    } അല്ലെങ്കിൽ {
        /* Lock: save the specified controls that *are* already enabled */
        filter->priv = (uint32_t) (
            state->base.components.controls & filter->action.ctrls.ctrls
        );
    }

    എങ്കിൽ (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_LOCK)) {
        /* Enable the specified controls that are not already enabled */
        state->base.components.controls |= filter->action.ctrls.ctrls;
    }
}

സ്ഥിതം bool
xkb_filter_ctrls_func(ഘടന xkb_server_state *state,
                      ഘടന xkb_events *events,
                      ഘടന xkb_filter *filter,
                      സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                      എണ്ണൽ xkb_താക്കോൽ_direction direction)
{
    എങ്കിൽ (താക്കോൽ != filter->താക്കോൽ)
        തിരികെ XKB_FILTER_CONTINUE;

    തിരഞ്ഞെടുപ്പ് (direction) {
    അവസ്ഥ XKB_താക്കോൽ_DOWN:
        filter->refcnt++;
        /* fallthrough */
    അവസ്ഥ XKB_താക്കോൽ_REPEATED:
        തിരികെ XKB_FILTER_CONSUME;
    സ്ഥിരസ്ഥിതി:
        എങ്കിൽ (--filter->refcnt > 0)
            തിരികെ XKB_FILTER_CONSUME;
    }

    എങ്കിൽ (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_UNLOCK)) {
        സ്ഥിരം എണ്ണൽ xkb_action_controls old = state->base.components.controls;

        /*
         * Set: Disable specified controls that were *not* enabled at key press.
         * Lock: Disable specified controls that *were* enabled at key press.
         */
        state->base.components.controls &= ~(എണ്ണൽ xkb_action_controls) filter->priv;

        എങ്കിൽ ((old & CONTROL_STICKY_താക്കോലുകൾ) &&
            !(state->base.components.controls & CONTROL_STICKY_താക്കോലുകൾ)) {
            /* Sticky keys were disabled: clear all locks and latches */
            clear_all_latches_and_locks(state, events);
        }
    }

    filter->func = ശൂന്യ;
    തിരികെ XKB_FILTER_CONTINUE;
}

സ്ഥിതം bool
append_redirect_താക്കോൽ_events(ഘടന xkb_server_state *state,
                           ഘടന xkb_events *events,
                           സ്ഥിരം ഘടന xkb_redirect_താക്കോൽ_action *redirect,
                           എണ്ണൽ xkb_താക്കോൽ_direction direction)
{
    എണ്ണൽ xkb_state_component changed = 0;
    സ്ഥിരം xkb_mod_mask_t mask = redirect->affect_mods;

    /*
     * Reference state: find the last state update in the queue, otherwise
     * use the current state.
     */
    ഘടന xkb_event *event;
    ഘടന state_components last_components = state->base.components;
    darray_foreach_reverse(event, events->queue) {
        എങ്കിൽ (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
            last_components = event->components.components;
            നിർത്തുക;
        }
    }

    എങ്കിൽ (mask) {
        ഘടന state_components new = last_components;
        new.base_mods = (new.base_mods & ~mask) | redirect->mods;
        new.latched_mods = (new.latched_mods & ~mask) | redirect->mods;
        new.locked_mods = (new.locked_mods & ~mask) | redirect->mods;
        new.mods = (new.mods & ~mask) | redirect->mods;

        changed = get_state_component_changes(&last_components, &new);
        എങ്കിൽ (changed) {
            darray_append(events->queue, (ഘടന xkb_event) {
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
            darray_append(events->queue, (ഘടന xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_FRAME,
            });
        }
    }

    darray_append(events->queue, (ഘടന xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_താക്കോൽ,
        .താക്കോൽ = {
            .keycode = redirect->keycode,
            .direction = direction
        }
    });

    എങ്കിൽ (mask && changed) {
        /* Restore state */
        darray_append(events->queue, (ഘടന xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = last_components,
                .changed = changed
            }
        });
    }

    state->flags &= ~STATE_REQUIRE_താക്കോൽ_EVENT;
    തിരികെ true;
}

സ്ഥിതം ശൂന്യം
xkb_filter_redirect_താക്കോൽ_new(ഘടന xkb_server_state *state,
                            ഘടന xkb_events *events,
                            ഘടന xkb_filter *filter)
{
    /* Action effectual only with the xkb_machine API and a valid keycode */
    എങ്കിൽ (!events || filter->action.redirect.keycode == XKB_KEYCODE_INVALID) {
        filter->func = ശൂന്യ;
        തിരികെ;
    }
    append_redirect_താക്കോൽ_events(state, events, &filter->action.redirect,
                               XKB_താക്കോൽ_DOWN);
}

സ്ഥിതം bool
xkb_filter_redirect_താക്കോൽ_func(ഘടന xkb_server_state *state,
                             ഘടന xkb_events *events,
                             ഘടന xkb_filter *filter,
                             സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                             എണ്ണൽ xkb_താക്കോൽ_direction direction)
{
    എങ്കിൽ (താക്കോൽ != filter->താക്കോൽ)
        തിരികെ XKB_FILTER_CONTINUE;

    എങ്കിൽ (direction == XKB_താക്കോൽ_UP) {
        append_redirect_താക്കോൽ_events(state, events,
                                   &filter->action.redirect, XKB_താക്കോൽ_UP);
        filter->func = ശൂന്യ;
        തിരികെ XKB_FILTER_CONSUME;
     } അല്ലെങ്കിൽ എങ്കിൽ (direction == XKB_താക്കോൽ_DOWN) {
        സ്ഥിരം സംഘം xkb_action *actions = ശൂന്യ;
        സ്ഥിരം xkb_action_എണ്ണം_t എണ്ണം =
            xkb_താക്കോൽ_get_actions(&state->base, താക്കോൽ, &actions);

        വേണ്ടി (xkb_action_എണ്ണം_t a = 0; a < എണ്ണം; a++) {
            എങ്കിൽ (actions[a].type == ACTION_TYPE_REDIRECT_താക്കോൽ &&
                actions[a].redirect.keycode != filter->action.redirect.keycode) {
                    /* One action redirects to a different key: release first */
                    append_redirect_താക്കോൽ_events(state, events,
                                               &filter->action.redirect,
                                               XKB_താക്കോൽ_UP);
                filter->func = ശൂന്യ;
                തിരികെ XKB_FILTER_CONTINUE;
            }
        }
    }

    append_redirect_താക്കോൽ_events(state, events, &filter->action.redirect,
                               direction);
    തിരികെ XKB_FILTER_CONSUME;
}

സ്ഥിതം സ്ഥിരം ഘടന {
    ശൂന്യം (*new)(ഘടന xkb_server_state *state,
                ഘടന xkb_events *events,
                ഘടന xkb_filter *filter);
    bool (*func)(ഘടന xkb_server_state *state,
                 ഘടന xkb_events *events,
                 ഘടന xkb_filter *filter,
                 സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ, എണ്ണൽ xkb_താക്കോൽ_direction direction);
} filter_action_funcs[ACTION_TYPE_അക്കം_ENTRIES] = {
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
    [ACTION_TYPE_REDIRECT_താക്കോൽ] = { xkb_filter_redirect_താക്കോൽ_new,
                                   xkb_filter_redirect_താക്കോൽ_func },
};

/**
 * Applies any relevant filters to the key, first from the list of filters
 * that are currently active, then if no filter has claimed the key, possibly
 * apply a new filter from the key action.
 */
സ്ഥിതം ശൂന്യം
xkb_filter_apply_all(ഘടന xkb_server_state *state,
                     ഘടന xkb_events *events,
                     സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                     എണ്ണൽ xkb_താക്കോൽ_direction direction)
{
    /* First run through all the currently active filters and see if any of
     * them have consumed this event. */
    bool consumed = false;
    ഘടന xkb_filter *filter;
    darray_foreach(filter, state->filters) {
        എങ്കിൽ (!filter->func)
            തുടരുക;

        എങ്കിൽ (filter->func(state, events, filter, താക്കോൽ, direction) == XKB_FILTER_CONSUME)
            consumed = true;
    }
    എങ്കിൽ (consumed || direction == XKB_താക്കോൽ_UP)
        തിരികെ;

    /* No filter consumed this event, so proceed with the key actions */
    സ്ഥിരം സംഘം xkb_action *actions = ശൂന്യ;
    xkb_action_എണ്ണം_t എണ്ണം = xkb_താക്കോൽ_get_actions(&state->base, താക്കോൽ,
                                                         &actions);

    വേണ്ടി (ഘടന {
            xkb_action_എണ്ണം_t എണ്ണം;
            സ്ഥിരം സംഘം xkb_action *actions;
         } next = {0};
         എണ്ണം;
         എണ്ണം = next.എണ്ണം, next.എണ്ണം = 0, actions = next.actions
    ) {
        /*
         * Process actions sequentially.
         *
         * NOTE: We rely on the parser to disallow multiple modifier or group
         * actions (see `CheckMultipleActionsCategories`). Allowing multiple
         * such actions requires a refactor of the state handling.
         */
        വേണ്ടി (xkb_action_എണ്ണം_t k = 0; k < എണ്ണം; k++) {
            /*
             * It's possible for the keymap to set action->type explicitly, like
             * so:
             *     interpret XF86_Next_VMode {
             *         action = Private(type=0x86, data="+VMode");
             *     };
             * We don't handle those.
             */
            എങ്കിൽ (actions[k].type >= ACTION_TYPE_അക്കം_ENTRIES)
                തുടരുക;

            /* Go to next action if no corresponding action handler */
            എങ്കിൽ (!filter_action_funcs[actions[k].type].new)
                തുടരുക;

            /* Add a new filter and run the corresponding initial action */
            filter = xkb_filter_new(state);
            filter->താക്കോൽ = താക്കോൽ;
            filter->action = actions[k];

            തിരഞ്ഞെടുപ്പ് (filter->action.type) {
            അവസ്ഥ ACTION_TYPE_MOD_SET:
                എങ്കിൽ (state->base.components.controls & CONTROL_STICKY_താക്കോലുകൾ) {
                    /* Convert modifier set action to a latch */
                    filter->action.type = ACTION_TYPE_MOD_LATCH;
                    എങ്കിൽ (state->a11y_flags & XKB_A11_Y_STICKY_താക്കോലുകൾ_LATCH_TO_LOCK) {
                        filter->action.mods.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                /* fallthrough */
            അവസ്ഥ ACTION_TYPE_MOD_LATCH:
            അവസ്ഥ ACTION_TYPE_MOD_LOCK: {
                സ്ഥിതം സ്ഥിരം എണ്ണൽ xkb_action_flags unlock_mods_on_press
                    = ACTION_LOCK_CLEAR     /* SetMods & LatchMods: can unlock */
                    | ACTION_LOCK_NO_UNLOCK /* LockMods: cannot unlock */
                    | ACTION_EXPLICIT_UNLOCK_ON_PRESS;
                എങ്കിൽ ((state->flags & STATE_UNLOCK_MODS_ON_PRESS) &&
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
                നിർത്തുക;
            }
            അവസ്ഥ ACTION_TYPE_GROUP_SET:
                എങ്കിൽ (state->base.components.controls & CONTROL_STICKY_താക്കോലുകൾ) {
                    /* Convert group set action to a latch */
                    filter->action.type = ACTION_TYPE_GROUP_LATCH;
                    എങ്കിൽ (state->a11y_flags & XKB_A11_Y_STICKY_താക്കോലുകൾ_LATCH_TO_LOCK) {
                        filter->action.group.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                നിർത്തുക;
            അവസ്ഥ ACTION_TYPE_GROUP_LOCK:
                എങ്കിൽ ((filter->action.group.flags &
                     ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE) &&
                    (state->flags &
                     STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_താക്കോൽ) &&
                    /*
                     * FIXME: sticky keys may leave an unexpected modifier
                     * from the level 0 latched, so disable for now.
                     */
                    !(state->base.components.controls & CONTROL_STICKY_താക്കോലുകൾ))
                {
                    filter->action.group.flags |= ACTION_LOCK_ON_RELEASE;
                    /* Add base level action */
                    next.എണ്ണം = xkb_keymap_താക്കോൽ_get_actions_by_level(
                        state->base.keymap, താക്കോൽ, state->base.components.group,
                        0, &next.actions
                    );
                    പ്രതിജ്ഞാ(next.എണ്ണം == 1);
                }
                നിർത്തുക;
            അവസ്ഥ ACTION_TYPE_PTR_MOVE:
            അവസ്ഥ ACTION_TYPE_PTR_BUTTON:
            അവസ്ഥ ACTION_TYPE_PTR_LOCK:
            അവസ്ഥ ACTION_TYPE_PTR_DEFAULT:
                എങ്കിൽ (!(state->base.components.controls & CONTROL_MOUSE_താക്കോലുകൾ)) {
                    /* Convert pointer actions to NoAction */
                    പ്രതിജ്ഞാ(!filter_action_funcs[ACTION_TYPE_NONE].new);
                    filter->func = ശൂന്യ;
                    തുടരുക;
                }
                നിർത്തുക;
            അവസ്ഥ ACTION_TYPE_REDIRECT_താക്കോൽ:
                // FIXME: this is not efficient to resolve mods here each time
                filter->action.redirect.affect_mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.affect_mods
                );
                filter->action.redirect.mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.mods
                );
                നിർത്തുക;
            സ്ഥിരസ്ഥിതി: ;
            }
            filter->func = filter_action_funcs[filter->action.type].func;
            filter_action_funcs[filter->action.type].new(state, events, filter);
        }
    }
}

സ്ഥിതം ശൂന്യം
xkb_state_update_derived(ഘടന xkb_state *state);

സ്ഥിതം വരിയിൽ ശൂന്യം
xkb_state_init(ഘടന xkb_state * restrict state,
               ഘടന xkb_keymap * restrict keymap,
               എണ്ണൽ xkb_state_mode_internal mode)
{
    state->mode = mode;
    state->refcnt = 1;
    state->keymap = xkb_keymap_ref(keymap);
    state->out_of_range_group.policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

    /* Ensure that derived state is correctly initialized */
    xkb_state_update_derived(state);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
xkb_client_state_init(ഘടന xkb_client_state * restrict state,
                      ഘടന xkb_keymap * restrict keymap,
                      എണ്ണൽ xkb_state_mode_internal mode)
{
    xkb_state_init(&state->base, keymap, mode);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
xkb_server_state_init(ഘടന xkb_server_state * restrict state,
                      ഘടന xkb_keymap * restrict keymap,
                      എണ്ണൽ xkb_state_mode_internal mode,
                      എണ്ണൽ xkb_a11y_flags a11y_affect,
                      എണ്ണൽ xkb_a11y_flags a11y_flags)
{
    xkb_state_init(&state->base, keymap, mode);

    state->a11y_flags = a11y_flags;
    എങ്കിൽ (keymap->format != XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !(a11y_affect & XKB_A11_Y_LATCH_SIMULTANEOUS_താക്കോലുകൾ)) {
             /* Keymap v2+: enable extension to XKB if not manually disabled */
             state->a11y_flags |= XKB_A11_Y_LATCH_SIMULTANEOUS_താക്കോലുകൾ;
    }
}

ഘടന xkb_state *
xkb_state_new(ഘടന xkb_keymap *keymap)
{
    ഘടന xkb_server_state * സ്ഥിരം state = calloc(1, വലുപ്പം(*state));
    എങ്കിൽ (!state) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "Could not allocate a state object.\n");
        തിരികെ ശൂന്യ;
    }

    xkb_server_state_init(state, keymap, LEGACY_MIXED_STATE, 0, 0);

    തിരികെ (ഘടന xkb_state *)state;
}

ഘടന xkb_state *
xkb_state_new_with_mode(ഘടന xkb_keymap * restrict keymap,
                        എണ്ണൽ xkb_state_mode mode,
                        എണ്ണൽ xkb_status * restrict status)
{
    തിരഞ്ഞെടുപ്പ് (mode) {
    അവസ്ഥ XKB_STATE_MODE_CLIENT:
    അവസ്ഥ XKB_STATE_MODE_SERVER_QUERY: {
        ഘടന xkb_client_state * സ്ഥിരം state = calloc(1, വലുപ്പം(*state));
        എങ്കിൽ (!state)
            നിർത്തുക;

        static_assert((unsigned)XKB_STATE_MODE_CLIENT ==
                      (unsigned)CLIENT_STATE, "");
        static_assert((unsigned)XKB_STATE_MODE_SERVER_QUERY ==
                      (unsigned)SERVER_COMPANION, "");
        xkb_client_state_init(state, keymap, (എണ്ണൽ xkb_state_mode_internal)mode);

        എങ്കിൽ (status)
            *status = XKB_SUCCESS;

        തിരികെ (ഘടന xkb_state *)state;
    }
    അവസ്ഥ XKB_STATE_MODE_SERVER: {
        ഘടന xkb_server_state * സ്ഥിരം state = calloc(1, വലുപ്പം(*state));
        എങ്കിൽ (!state)
            നിർത്തുക;

        xkb_server_state_init(state, keymap, LEGACY_SERVER_STATE, 0, 0);

        എങ്കിൽ (status)
            *status = XKB_SUCCESS;

        തിരികെ (ഘടന xkb_state *)state;
    }
    സ്ഥിരസ്ഥിതി:
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_STATE_MODE_,
                     "Unsupported state mode: %u\n", mode);
        എങ്കിൽ (status)
            *status = XKB_ERROR_UNSUPPORTED_STATE_MODE;
        തിരികെ ശൂന്യ;
    }

    log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                  "Could not allocate a state object.\n");
    എങ്കിൽ (status)
        *status = XKB_ERROR_ALLOCATION_FAILURE;
    തിരികെ ശൂന്യ;
}

ഘടന xkb_state *
xkb_state_ref(ഘടന xkb_state *state)
{
    പ്രതിജ്ഞാ(state->refcnt > 0);
    state->refcnt++;
    തിരികെ state;
}

ശൂന്യം
xkb_state_reset(ഘടന xkb_state *state)
{
    memset(&state->components, 0, വലുപ്പം(state->components));

    എങ്കിൽ (state->mode < LEGACY_MIXED_STATE) {
        static_assert(വലുപ്പം(*state) == വലുപ്പം(ഘടന xkb_client_state),
                      "Not a mere wrapper");
        /* Nothing else to reset */
    } അല്ലെങ്കിൽ {
        ഘടന xkb_server_state *state_ = (ഘടന xkb_server_state *)state;
        സ്ഥിരം ഘടന xkb_server_state prev = *state_;
        memset(state_, 0, വലുപ്പം(*state_));

        state_->base = prev.base;
        state_->a11y_flags = prev.a11y_flags;
        state_->filters = prev.filters;
        darray_size(state_->filters) = 0;
    }
}

സ്ഥിതം വരിയിൽ ശൂന്യം
xkb_state_destroy(ഘടന xkb_state *state)
{
    xkb_keymap_unref(state->keymap);
    എങ്കിൽ (state->mode > SERVER_COMPANION)
        darray_free(((ഘടന xkb_server_state *)state)->filters);
}

ശൂന്യം
xkb_state_unref(ഘടന xkb_state *state)
{
    പ്രതിജ്ഞാ(!state || state->refcnt > 0);
    എങ്കിൽ (!state || --state->refcnt > 0)
        തിരികെ;

    xkb_state_destroy(state);

    തിരഞ്ഞെടുപ്പ് (state->mode) {
    അവസ്ഥ CLIENT_STATE:
    അവസ്ഥ SERVER_COMPANION:
        സ്വതന്ത്രം((ഘടന xkb_client_state *)state);
        നിർത്തുക;
    അവസ്ഥ LEGACY_MIXED_STATE:
    അവസ്ഥ LEGACY_SERVER_STATE:
        സ്വതന്ത്രം((ഘടന xkb_server_state *)state);
        നിർത്തുക;
    അവസ്ഥ SERVER_STATE: /* handled by xkb_machine_unref() */
    സ്ഥിരസ്ഥിതി:
        PANIC_UNREACHABLE();
    }
}

ഘടന xkb_keymap *
xkb_state_get_keymap(ഘടന xkb_state *state)
{
    തിരികെ state->keymap;
}

/**
 * Update the LED state to match the rest of the xkb_state.
 */
സ്ഥിതം ശൂന്യം
xkb_state_led_update_all(ഘടന xkb_state *state)
{
    xkb_led_index_t idx;
    സ്ഥിരം ഘടന xkb_led *led;

    state->components.leds = 0;

    xkb_leds_enumerate(idx, led, state->keymap) {

        എങ്കിൽ (led->which_mods != 0 && led->mods.mask != 0) {
            xkb_mod_mask_t mod_mask = 0;
            എങ്കിൽ (led->which_mods & XKB_STATE_MODS_EFFECTIVE)
                mod_mask |= state->components.mods;
            എങ്കിൽ (led->which_mods & XKB_STATE_MODS_DEPRESSED)
                mod_mask |= state->components.base_mods;
            എങ്കിൽ (led->which_mods & XKB_STATE_MODS_LATCHED)
                mod_mask |= state->components.latched_mods;
            എങ്കിൽ (led->which_mods & XKB_STATE_MODS_LOCKED)
                mod_mask |= state->components.locked_mods;

            എങ്കിൽ (led->mods.mask & mod_mask) {
                state->components.leds |= (UINT32_C(1) << idx);
                തുടരുക;
            }
        }

        എങ്കിൽ (led->which_groups != 0) {
            എങ്കിൽ (likely(led->groups) != 0) {
                xkb_layout_mask_t group_mask = 0;
                /* Effective and locked groups have been brought into range */
                പ്രതിജ്ഞാ(state->components.group < XKB_MAX_GROUPS);
                പ്രതിജ്ഞാ(state->components.locked_group >= 0 &&
                       state->components.locked_group < XKB_MAX_GROUPS);
                /* Effective and locked groups are used as mask */
                എങ്കിൽ (led->which_groups & XKB_STATE_LAYOUT_EFFECTIVE)
                    group_mask |= (UINT32_C(1) << state->components.group);
                എങ്കിൽ (led->which_groups & XKB_STATE_LAYOUT_LOCKED)
                    group_mask |= (UINT32_C(1) << state->components.locked_group);
                /* Base and latched groups only have to be non-zero */
                എങ്കിൽ ((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                    state->components.base_group != 0)
                    group_mask |= led->groups;
                എങ്കിൽ ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                    state->components.latched_group != 0)
                    group_mask |= led->groups;

                എങ്കിൽ (led->groups & group_mask) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    തുടരുക;
                }
            } അല്ലെങ്കിൽ {
                /* Special case for Base and latched groups */
                എങ്കിൽ (((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                     state->components.base_group == 0) ||
                    ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                     state->components.latched_group == 0)) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    തുടരുക;
                }
            }
        }

        എങ്കിൽ (led->ctrls & state->components.controls) {
            state->components.leds |= (UINT32_C(1) << idx);
            തുടരുക;
        }
    }
}

/**
 * Calculates the derived state (effective mods/group and LEDs) from an
 * up-to-date xkb_state.
 */
സ്ഥിതം ശൂന്യം
xkb_state_update_derived(ഘടന xkb_state *state)
{
    xkb_layout_index_t wrapped;

    state->components.mods = (state->components.base_mods |
                              state->components.latched_mods |
                              state->components.locked_mods);

    /* Lock group must be adjusted, but not base nor latched groups */
    wrapped = XkbWrapGroupIntoRange(state->components.locked_group,
                                    state->keymap->അക്കം_groups,
                                    state->out_of_range_group.policy,
                                    state->out_of_range_group.redirect_group);
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->components.locked_group =
        (int32_t) (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    /* Effective group must be adjusted */
    wrapped = XkbWrapGroupIntoRange(state->components.base_group +
                                    state->components.latched_group +
                                    state->components.locked_group,
                                    state->keymap->അക്കം_groups,
                                    state->out_of_range_group.policy,
                                    state->out_of_range_group.redirect_group);
    state->components.group =
        (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    xkb_state_led_update_all(state);
}

സ്ഥിതം ശൂന്യം
state_update_enabled_controls(ഘടന xkb_server_state *state,
                              എണ്ണൽ xkb_keyboard_control_flags affect_controls,
                              എണ്ണൽ xkb_keyboard_control_flags controls,
                              ഘടന xkb_events *events)
{
    സ്ഥിരം bool had_sticky_താക്കോലുകൾ = state->base.components.controls
                               & CONTROL_STICKY_താക്കോലുകൾ;

    /*
     * Enable using the public API with the all the Control values, except
     * the internal ones, if any.
     */
    affect_controls =
        ( affect_controls
        & (എണ്ണൽ xkb_keyboard_control_flags)CONTROL_ALL_BOOLEAN );
    state->base.components.controls &=
        (എണ്ണൽ xkb_action_controls)~affect_controls;
    state->base.components.controls |=
        (എണ്ണൽ xkb_action_controls)(controls & affect_controls);

    എങ്കിൽ (had_sticky_താക്കോലുകൾ &&
        !(state->base.components.controls & CONTROL_STICKY_താക്കോലുകൾ)) {
        /* Sticky keys were disabled: clear all locks and latches */
        clear_all_latches_and_locks(state, events);
    }

    xkb_state_update_derived(&state->base);
}

/**
 * Given a particular key event, updates the state structure to reflect the
 * new modifiers.
 */
എണ്ണൽ xkb_state_component
xkb_state_update_താക്കോൽ(ഘടന xkb_state *base_state, xkb_keycode_t kc,
                     എണ്ണൽ xkb_താക്കോൽ_direction direction)
{
    /* Guard against client-only state */
    പ്രതിജ്ഞാ(base_state->mode > SERVER_COMPANION);
    എങ്കിൽ (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        തിരികെ 0;
    }

    ഘടന xkb_server_state * സ്ഥിരം state =
        (ഘടന xkb_server_state *)base_state;

    സ്ഥിരം ഘടന xkb_താക്കോൽ* സ്ഥിരം താക്കോൽ = Xkb_താക്കോൽ(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    എങ്കിൽ (!താക്കോൽ || (direction == XKB_താക്കോൽ_REPEATED && !താക്കോൽ->repeats))
        തിരികെ 0;

    എങ്കിൽ (direction == XKB_താക്കോൽ_DOWN &&
        (state->base.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_താക്കോലുകൾ) &&
        (state->a11y_flags & XKB_A11_Y_STICKY_താക്കോലുകൾ_NO_SIMULTANEOUS_താക്കോലുകൾ) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_താക്കോലുകൾ,
                                      0, ശൂന്യ);
    }

    സ്ഥിരം ഘടന state_components prev_components = state->base.components;

    state->set_mods = 0;
    state->clear_mods = 0;

    xkb_filter_apply_all(state, ശൂന്യ, താക്കോൽ, direction);

    xkb_mod_index_t i;
    xkb_mod_mask_t bit;
    വേണ്ടി (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        എങ്കിൽ (state->set_mods & bit) {
            state->mod_താക്കോൽ_എണ്ണം[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    വേണ്ടി (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        എങ്കിൽ (state->clear_mods & bit) {
            state->mod_താക്കോൽ_എണ്ണം[i]--;
            എങ്കിൽ (state->mod_താക്കോൽ_എണ്ണം[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_താക്കോൽ_എണ്ണം[i] = 0;
            }
            state->clear_mods &= ~bit;
        }
    }

    xkb_state_update_derived(&state->base);

    തിരികെ get_state_component_changes(&prev_components, &state->base.components);
}

/* We need fake keys for `update_latch_modifiers` and `update_latch_group`.
 * These keys must have at least one level in order to break latches. We need 2
 * keys with specific actions in order to update group/mod latches without
 * affecting each other. */
സ്ഥിതം ഘടന xkb_താക്കോൽ_type_entry synthetic_താക്കോൽ_level_entry = { 0 };
സ്ഥിതം ഘടന xkb_താക്കോൽ_type synthetic_താക്കോൽ_type = {
     .അക്കം_entries = 1,
     .അക്കം_levels = 1,
     .entries = &synthetic_താക്കോൽ_level_entry
};
സ്ഥിതം സ്ഥിരം ഘടന xkb_താക്കോൽ synthetic_താക്കോൽ = { 0 };

/* Transcription from xserver: XkbLatchModifiers */
സ്ഥിതം ശൂന്യം
update_latch_modifiers(ഘടന xkb_server_state *state,
                       ഘടന xkb_events *events,
                       xkb_mod_mask_t mask, xkb_mod_mask_t latches)
{
    /* Clear affected latched modifiers */
    സ്ഥിരം xkb_mod_mask_t clear = mask & ~latches;
    state->base.components.latched_mods &= ~clear;

    /* Clear any pending latch to locks using ad hoc action:
     * only affect corresponding modifier latches and no group latch. */
    ഘടന xkb_level synthetic_താക്കോൽ_level_break_mod_latch = {
        .അക്കം_syms = 0,
        .അക്കം_actions = 1,
        .upper = XKB_താക്കോൽ_No_ചിഹ്നം,
        .s.sym = XKB_താക്കോൽ_No_ചിഹ്നം,
        .a.action.internal = {
            .type = ACTION_TYPE_INTERNAL,
            .flags = INTERNAL_BREAKS_MOD_LATCH,
            .clear_latched_mods = clear
        }
    };
    ഘടന xkb_group synthetic_താക്കോൽ_group_break_mod_latch = {
        .type = &synthetic_താക്കോൽ_type,
        .levels = &synthetic_താക്കോൽ_level_break_mod_latch
    };
    സ്ഥിരം ഘടന xkb_താക്കോൽ synthetic_താക്കോൽ_break_mod_latch = {
        .അക്കം_groups = 1,
        .groups = &synthetic_താക്കോൽ_group_break_mod_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_താക്കോൽ_break_mod_latch, XKB_താക്കോൽ_DOWN);

    /* Finally set the latched mods by simulate tapping a key with the
     * corresponding action */
    സ്ഥിരം ഘടന xkb_താക്കോൽ* സ്ഥിരം താക്കോൽ = &synthetic_താക്കോൽ;
    സ്ഥിരം സംഘം xkb_action latch_mods = {
        .mods = {
            .type = ACTION_TYPE_MOD_LATCH,
            .mods = { .mask = mask & latches },
            .flags = 0,
        },
    };
    ഘടന xkb_filter* സ്ഥിരം filter = xkb_filter_new(state);
    filter->താക്കോൽ = താക്കോൽ;
    filter->func = xkb_filter_mod_latch_func;
    filter->action = latch_mods;
    xkb_filter_mod_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_mod_latch_func(state, events, filter, താക്കോൽ, XKB_താക്കോൽ_UP);
}

/* Transcription from xserver: XkbLatchGroup */
സ്ഥിതം ശൂന്യം
update_latch_group(ഘടന xkb_server_state *state,
                   ഘടന xkb_events *events, int32_t group)
{
    /* Clear any pending latch to locks. */
    സ്ഥിതം ഘടന xkb_level synthetic_താക്കോൽ_level_break_group_latch = {
        .അക്കം_syms = 0,
        .അക്കം_actions = 1,
        .upper = XKB_താക്കോൽ_No_ചിഹ്നം,
        .s.sym = XKB_താക്കോൽ_No_ചിഹ്നം,
        .a.action.internal = {
            .type = ACTION_TYPE_INTERNAL,
            .flags = INTERNAL_BREAKS_GROUP_LATCH,
            .clear_latched_mods = 0
        }
    };
    സ്ഥിതം ഘടന xkb_group synthetic_താക്കോൽ_group_break_group_latch = {
        .type = &synthetic_താക്കോൽ_type,
        .levels = &synthetic_താക്കോൽ_level_break_group_latch
    };
    സ്ഥിതം സ്ഥിരം ഘടന xkb_താക്കോൽ synthetic_താക്കോൽ_break_group_latch = {
        .അക്കം_groups = 1,
        .groups = &synthetic_താക്കോൽ_group_break_group_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_താക്കോൽ_break_group_latch, XKB_താക്കോൽ_DOWN);

    /* Simulate tapping a key with a group latch action, but in isolation: i.e.
     * without affecting the other filters. */
    സ്ഥിരം ഘടന xkb_താക്കോൽ* സ്ഥിരം താക്കോൽ = &synthetic_താക്കോൽ;
    സ്ഥിരം സംഘം xkb_action latch_group = {
        .group = {
            .type = ACTION_TYPE_GROUP_LATCH,
            .flags = ACTION_ABSOLUTE_SWITCH,
            .group = group,
        },
    };
    ഘടന xkb_filter* സ്ഥിരം filter = xkb_filter_new(state);
    filter->താക്കോൽ = താക്കോൽ;
    filter->func = xkb_filter_group_latch_func;
    filter->action = latch_group;
    xkb_filter_group_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_group_latch_func(state, events, filter, താക്കോൽ, XKB_താക്കോൽ_UP);
}

/**
 * Compute the resolved effective mask of an arbitrary input.
 *
 * Contrary to `mod_mask_get_effective`, it resolves only modifiers not present
 * in the canonical mask, so that it enables `xkb_state_serialize_mods` to
 * round trip via `xkb_state_update_mask`.
 */
സ്ഥിതം വരിയിൽ xkb_mod_mask_t
resolve_to_canonical_mods(ഘടന xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    തിരികെ
        /*
         * Keep canonical modifier mask.
         * It contains either real modifiers or canonical virtual modifiers.
         */
        (mods & keymap->canonical_state_mask) |
        /* Resolve other modifiers */
        mod_mask_get_effective(keymap,
                               mods & ~keymap->canonical_state_mask);
}

സ്ഥിതം ശൂന്യം
state_update_latched_locked(
    ഘടന xkb_server_state * restrict state,
    സ്ഥിരം ഘടന xkb_state_components_update * restrict update,
    ഘടന xkb_events * restrict events
)
{
    /* Update locks */
    സ്ഥിരം xkb_mod_mask_t affect_locked_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_locked_mods);
    എങ്കിൽ ((update->components & XKB_STATE_MODS_LOCKED) &&
        affect_locked_mods) {
        സ്ഥിരം xkb_mod_mask_t locked_mods =
            resolve_to_canonical_mods(state->base.keymap, update->locked_mods);
        state->base.components.locked_mods &= ~affect_locked_mods;
        state->base.components.locked_mods |= locked_mods & affect_locked_mods;
    }
    എങ്കിൽ (update->components & XKB_STATE_LAYOUT_LOCKED) {
        state->base.components.locked_group = update->locked_layout;
    }

    /* Update latches */
    സ്ഥിരം xkb_mod_mask_t affect_latched_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_latched_mods);
    എങ്കിൽ ((update->components & XKB_STATE_MODS_LATCHED) &&
        affect_latched_mods) {
        സ്ഥിരം xkb_mod_mask_t latched_mods =
            resolve_to_canonical_mods(state->base.keymap, update->latched_mods);
        update_latch_modifiers(state, events, affect_latched_mods, latched_mods);
    }
    എങ്കിൽ (update->components & XKB_STATE_LAYOUT_LATCHED) {
        update_latch_group(state, events, update->latched_layout);
    }
}

എണ്ണൽ xkb_state_component
xkb_state_update_latched_locked(ഘടന xkb_state *base_state,
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
    പ്രതിജ്ഞാ(base_state->mode > SERVER_COMPANION);
    എങ്കിൽ (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        തിരികെ 0;
    }

    ഘടന xkb_server_state * സ്ഥിരം state =
        (ഘടന xkb_server_state *)base_state;

    സ്ഥിരം ഘടന state_components previous_components = state->base.components;
    സ്ഥിരം എണ്ണൽ xkb_state_component components
        = ((affect_latched_mods || latched_mods) ? XKB_STATE_MODS_LATCHED : 0)
        | ((affect_locked_mods || locked_mods) ? XKB_STATE_MODS_LOCKED : 0)
        | (affect_latched_layout ? XKB_STATE_LAYOUT_LATCHED : 0)
        | (affect_locked_layout ? XKB_STATE_LAYOUT_LOCKED : 0);
    സ്ഥിരം ഘടന xkb_state_components_update update = {
        .size = വലുപ്പം(update),
        .components = components,
        .affect_latched_mods = affect_latched_mods,
        .latched_mods = latched_mods,
        .latched_layout = latched_layout,
        .affect_locked_mods = affect_locked_mods,
        .locked_mods = locked_mods,
        .locked_layout = locked_layout
    };
    state_update_latched_locked(state, &update, ശൂന്യ);
    xkb_state_update_derived(&state->base);
    തിരികെ get_state_component_changes(&previous_components,
                                       &state->base.components);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
clear_all_latches_and_locks(ഘടന xkb_server_state *state,
                            ഘടന xkb_events *events)
{
    സ്ഥിതം സ്ഥിരം എണ്ണൽ xkb_state_component components
        = XKB_STATE_MODS_LATCHED | XKB_STATE_MODS_LOCKED
        | XKB_STATE_LAYOUT_LATCHED | XKB_STATE_LAYOUT_LOCKED;
    സ്ഥിരം ഘടന xkb_state_components_update update = {
        .size = വലുപ്പം(update),
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

സ്ഥിതം എണ്ണൽ xkb_status
state_update_layout_policy(ഘടന xkb_server_state *state,
                           സ്ഥിരം ഘടന xkb_layout_policy_update *update)
{
    എങ്കിൽ (xkb_feature_supported(XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY,
                              update->policy)) {
        എങ്കിൽ (update->policy == XKB_LAYOUT_OUT_OF_RANGE_REDIRECT) {
            എങ്കിൽ (update->redirect < state->base.keymap->അക്കം_groups) {
                state->base.out_of_range_group.redirect_group =
                    update->redirect;
            } അല്ലെങ്കിൽ {
                log_err(state->base.keymap->ctx,
                        XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Layout policy: "
                        "unsupported layout index %"PRIu32" > %"PRIu32"\n",
                        update->redirect + 1, state->base.keymap->അക്കം_groups);
                തിരികെ XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
            }
        }
        state->base.out_of_range_group.policy = update->policy;
        തിരികെ XKB_SUCCESS;
    } അല്ലെങ്കിൽ {
        log_err(state->base.keymap->ctx,
                XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY_,
                "Unsupported layout policy: %d\n", update->policy);
        തിരികെ XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY;
    }
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
സ്ഥിതം എണ്ണൽ xkb_status
check_state_update_abi_(ഘടന xkb_context * restrict ctx,
                        സ്ഥിരം അക്ഷരം * restrict func,
                        സ്ഥിരം ഘടന xkb_synthetic_update * restrict update)
{
    എണ്ണൽ xkb_status status = XKB_SUCCESS;
    എങ്കിൽ ((status = xkb_check_state_abi(update)) ||
        (update->reserved0 != 0 &&
         (status = XKB_ERROR_ABI_FORWARD_COMPAT)) ||
        (update->components &&
         (status = xkb_check_state_abi(update->components))) ||
        (update->layout_policy &&
         (status = xkb_check_state_abi(update->layout_policy)))) {
        xkb_log_abi_error(ctx, func, status);
    }
    തിരികെ status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

#define check_state_update_abi(ctx, update) \
    check_state_update_abi_(ctx, __func__, update)

എണ്ണൽ xkb_status
xkb_state_update_synthetic(ഘടന xkb_state * base_state,
                           സ്ഥിരം ഘടന xkb_synthetic_update * update,
                           എണ്ണൽ xkb_state_component *changed)
{
    /* Guard against client-only state */
    എങ്കിൽ (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        തിരികെ XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    ഘടന xkb_server_state * സ്ഥിരം state =
        (ഘടന xkb_server_state *)base_state;

    /* Check ABI compatibility */
    എണ്ണൽ xkb_status status = check_state_update_abi(state->base.keymap->ctx,
                                                    update);
    എങ്കിൽ (status != XKB_SUCCESS)
        തിരികെ status;

    സ്ഥിരം ഘടന state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    /* Update parametrized controls first */
    എങ്കിൽ (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        എങ്കിൽ (status != XKB_SUCCESS)
            തിരികെ status;
    }

    എങ്കിൽ (update->components) {
        സ്ഥിരം ഘടന xkb_state_components_update * സ്ഥിരം components =
            update->components;
        /* Update boolean controls first */
        എങ്കിൽ (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, ശൂന്യ);
        }

        state_update_latched_locked(state, components, ശൂന്യ);
    }

    xkb_state_update_derived(&state->base);

    എങ്കിൽ (changed) {
        *changed = get_state_component_changes(&previous_components,
                                               &state->base.components);
    }
    തിരികെ XKB_SUCCESS;
}

/**
 * Updates the state from a set of explicit masks as gained from
 * xkb_state_serialize_mods and xkb_state_serialize_groups.  As noted in the
 * documentation for these functions in xkbcommon.h, this round-trip is
 * lossy, and should only be used to update a slave state mirroring the
 * master, e.g. in a client/server window system.
 */
എണ്ണൽ xkb_state_component
xkb_state_update_mask(ഘടന xkb_state *base_state,
                      xkb_mod_mask_t base_mods,
                      xkb_mod_mask_t latched_mods,
                      xkb_mod_mask_t locked_mods,
                      xkb_layout_index_t base_group,
                      xkb_layout_index_t latched_group,
                      xkb_layout_index_t locked_group)
{
    /* Guard against server-only and server companion state */
    പ്രതിജ്ഞാ(base_state->mode == CLIENT_STATE ||
           base_state->mode == LEGACY_MIXED_STATE);
    എങ്കിൽ (base_state->mode != CLIENT_STATE &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        തിരികെ 0;
    }

    ഘടന xkb_client_state * സ്ഥിരം state =
        (ഘടന xkb_client_state *)base_state;

    സ്ഥിരം ഘടന state_components prev_components = state->base.components;

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

    തിരികെ get_state_component_changes(&prev_components, &state->base.components);
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Lock_Modifier
 */
സ്ഥിതം bool
should_do_caps_transformation(ഘടന xkb_state *state, xkb_keycode_t kc)
{
    തിരികെ
        xkb_state_mod_index_is_active(state, XKB_MOD_INDEX_CAPS,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_index_is_consumed(state, kc, XKB_MOD_INDEX_CAPS) == 0;
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Control_Modifier
 */
സ്ഥിതം bool
should_do_ctrl_transformation(ഘടന xkb_state *state, xkb_keycode_t kc)
{
    തിരികെ
        xkb_state_mod_index_is_active(state, XKB_MOD_INDEX_CTRL,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_index_is_consumed(state, kc, XKB_MOD_INDEX_CTRL) == 0;
}

/**
 * Provides the symbols to use for the given key and state.  Returns the
 * number of symbols pointed to in syms_out.
 */
പൂർണ്ണസംഖ്യ
xkb_state_താക്കോൽ_get_syms(ഘടന xkb_state *state, xkb_keycode_t kc,
                       സ്ഥിരം xkb_keysym_t **syms_out)
{
    സ്ഥിരം xkb_layout_index_t layout = xkb_state_താക്കോൽ_get_layout(state, kc);
    എങ്കിൽ (layout == XKB_LAYOUT_INVALID)
        പോകുക err;

    സ്ഥിരം xkb_level_index_t level = xkb_state_താക്കോൽ_get_level(state, kc, layout);
    എങ്കിൽ (level == XKB_LEVEL_INVALID)
        പോകുക err;

    സ്ഥിരം ഘടന xkb_താക്കോൽ* സ്ഥിരം താക്കോൽ = Xkb_താക്കോൽ(state->keymap, kc);

    എങ്കിൽ (!താക്കോൽ)
        പോകുക err;

    സ്ഥിരം ഘടന xkb_level* സ്ഥിരം leveli =
        xkb_keymap_താക്കോൽ_get_level(state->keymap, താക്കോൽ, layout, level);

    എങ്കിൽ (!leveli)
        പോകുക err;

    സ്ഥിരം xkb_keysym_എണ്ണം_t അക്കം_syms = leveli->അക്കം_syms;
    എങ്കിൽ (അക്കം_syms == 0)
        പോകുക err;

    എങ്കിൽ (should_do_caps_transformation(state, kc)) {
        /* Only simple capitalization rules: keysyms count is unchanged. */
        എങ്കിൽ (അക്കം_syms > 1) {
            *syms_out = (leveli->has_upper)
                      ? leveli->s.syms + അക്കം_syms
                      : leveli->s.syms;
        } അല്ലെങ്കിൽ {
            *syms_out = &leveli->upper;
        }
    } അല്ലെങ്കിൽ {
        *syms_out = (അക്കം_syms > 1)
                  ? leveli->s.syms
                  : &leveli->s.sym;
    }
    തിരികെ (പൂർണ്ണസംഖ്യ) അക്കം_syms;

err:
    *syms_out = ശൂന്യ;
    തിരികെ 0;
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
സ്ഥിതം അക്ഷരം
XkbToControl(അക്ഷരം ch)
{
    അക്ഷരം c = ch;

    എങ്കിൽ ((c >= '@' && c < '\177') || c == ' ')
        c &= 0x1F;
    അല്ലെങ്കിൽ എങ്കിൽ (c == '2')
        c = '\000';
    അല്ലെങ്കിൽ എങ്കിൽ (c >= '3' && c <= '7')
        c -= ('3' - '\033');
    അല്ലെങ്കിൽ എങ്കിൽ (c == '8')
        c = '\177';
    അല്ലെങ്കിൽ എങ്കിൽ (c == '/')
        c = '_' & 0x1F;
    തിരികെ c;
}

/**
 * Provides either exactly one symbol, or XKB_KEY_NoSymbol.
 */
xkb_keysym_t
xkb_state_താക്കോൽ_get_one_sym(ഘടന xkb_state *state, xkb_keycode_t kc)
{
    സ്ഥിരം xkb_keysym_t *syms = ശൂന്യ;
    സ്ഥിരം പൂർണ്ണസംഖ്യ അക്കം_syms = xkb_state_താക്കോൽ_get_syms(state, kc, &syms);

    എങ്കിൽ (അക്കം_syms != 1)
        തിരികെ XKB_താക്കോൽ_No_ചിഹ്നം;
    അല്ലെങ്കിൽ
        തിരികെ syms[0];
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
സ്ഥിതം xkb_keysym_t
get_one_sym_for_string(ഘടന xkb_state *state, xkb_keycode_t kc)
{
    സ്ഥിരം xkb_layout_index_t layout = xkb_state_താക്കോൽ_get_layout(state, kc);
    സ്ഥിരം xkb_layout_index_t അക്കം_layouts =
        xkb_keymap_അക്കം_layouts_for_താക്കോൽ(state->keymap, kc);
    xkb_level_index_t level = xkb_state_താക്കോൽ_get_level(state, kc, layout);
    എങ്കിൽ (layout == XKB_LAYOUT_INVALID || അക്കം_layouts == 0 ||
        level == XKB_LEVEL_INVALID)
        തിരികെ XKB_താക്കോൽ_No_ചിഹ്നം;

    സ്ഥിരം xkb_keysym_t *syms = ശൂന്യ;
    പൂർണ്ണസംഖ്യ nsyms = xkb_keymap_താക്കോൽ_get_syms_by_level(state->keymap, kc,
                                                 layout, level, &syms);
    എങ്കിൽ (nsyms != 1)
        തിരികെ XKB_താക്കോൽ_No_ചിഹ്നം;
    xkb_keysym_t sym = syms[0];

    എങ്കിൽ (should_do_ctrl_transformation(state, kc) && sym > 127u) {
        വേണ്ടി (xkb_layout_index_t i = 0; i < അക്കം_layouts; i++) {
            level = xkb_state_താക്കോൽ_get_level(state, kc, i);
            എങ്കിൽ (level == XKB_LEVEL_INVALID)
                തുടരുക;

            nsyms = xkb_keymap_താക്കോൽ_get_syms_by_level(state->keymap, kc,
                                                     i, level, &syms);
            എങ്കിൽ (nsyms == 1 && syms[0] <= 127u) {
                sym = syms[0];
                നിർത്തുക;
            }
        }
    }

    എങ്കിൽ (should_do_caps_transformation(state, kc)) {
        sym = xkb_keysym_to_upper(sym);
    }

    തിരികെ sym;
}

പൂർണ്ണസംഖ്യ
xkb_state_താക്കോൽ_get_utf8(ഘടന xkb_state *state, xkb_keycode_t kc,
                       അക്ഷരം *buffer, size_t size)
{
    പൂർണ്ണസംഖ്യ nsyms;
    സ്ഥിരം xkb_keysym_t *syms = ശൂന്യ;
    സ്ഥിരം xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    എങ്കിൽ (sym != XKB_താക്കോൽ_No_ചിഹ്നം) {
        nsyms = 1; syms = &sym;
    }
    അല്ലെങ്കിൽ {
        nsyms = xkb_state_താക്കോൽ_get_syms(state, kc, &syms);
    }

    /* Make sure not to truncate in the middle of a UTF-8 sequence. */
    പൂർണ്ണസംഖ്യ offset = 0;
    അക്ഷരം tmp[XKB_KEYSYM_UTF8_MAX_SIZE];
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nsyms; i++) {
        പൂർണ്ണസംഖ്യ ret = xkb_keysym_to_utf8(syms[i], tmp, വലുപ്പം(tmp));
        എങ്കിൽ (ret <= 0)
            പോകുക err_bad;

        ret--;
        എങ്കിൽ ((size_t)offset + (size_t)ret <= size)
            memcpy(buffer + offset, tmp, (size_t)ret);
        offset += ret;
    }

    എങ്കിൽ ((size_t)offset >= size)
        പോകുക err_trunc;
    buffer[offset] = '\0';

    എങ്കിൽ (!is_valid_utf8(buffer, (size_t)offset))
        പോകുക err_bad;

    എങ്കിൽ (offset == 1 && (unsigned പൂർണ്ണസംഖ്യ) buffer[0] <= 127u &&
        should_do_ctrl_transformation(state, kc))
        buffer[0] = XkbToControl(buffer[0]);

    തിരികെ offset;

err_trunc:
    എങ്കിൽ (size > 0)
        buffer[size - 1] = '\0';
    തിരികെ offset;

err_bad:
    എങ്കിൽ (size > 0)
        buffer[0] = '\0';
    തിരികെ 0;
}

uint32_t
xkb_state_താക്കോൽ_get_utf32(ഘടന xkb_state *state, xkb_keycode_t kc)
{
    സ്ഥിരം xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    uint32_t cp = xkb_keysym_to_utf32(sym);

    എങ്കിൽ (cp <= 127u && should_do_ctrl_transformation(state, kc))
        cp = (uint32_t) XkbToControl((അക്ഷരം) cp);

    തിരികെ cp;
}

/**
 * Serializes the requested modifier state into an xkb_mod_mask_t, with all
 * the same disclaimers as in xkb_state_update_mask.
 */
സ്ഥിതം വരിയിൽ xkb_mod_mask_t
serialize_mods(സ്ഥിരം ഘടന state_components *components,
               എണ്ണൽ xkb_state_component type)
{
    xkb_mod_mask_t ret = 0;

    എങ്കിൽ (type & XKB_STATE_MODS_EFFECTIVE)
        തിരികെ components->mods;

    എങ്കിൽ (type & XKB_STATE_MODS_DEPRESSED)
        ret |= components->base_mods;
    എങ്കിൽ (type & XKB_STATE_MODS_LATCHED)
        ret |= components->latched_mods;
    എങ്കിൽ (type & XKB_STATE_MODS_LOCKED)
        ret |= components->locked_mods;

    തിരികെ ret;
}

xkb_mod_mask_t
xkb_state_serialize_mods(ഘടന xkb_state *state,
                         എണ്ണൽ xkb_state_component type)
{
    തിരികെ serialize_mods(&state->components, type);
}

/**
 * Serialises the requested group state, with all the same disclaimers as
 * in xkb_state_update_mask.
 */
സ്ഥിതം വരിയിൽ xkb_layout_index_t
serialize_layout(സ്ഥിരം ഘടന state_components *components,
                 എണ്ണൽ xkb_state_component type)
{
    എങ്കിൽ (type & XKB_STATE_LAYOUT_EFFECTIVE)
        തിരികെ components->group;

    int32_t ret = 0;
    എങ്കിൽ (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret += components->base_group;
    എങ്കിൽ (type & XKB_STATE_LAYOUT_LATCHED)
        ret += components->latched_group;
    എങ്കിൽ (type & XKB_STATE_LAYOUT_LOCKED)
        ret += components->locked_group;

    തിരികെ (xkb_layout_index_t)ret;
}

xkb_layout_index_t
xkb_state_serialize_layout(ഘടന xkb_state *state,
                           എണ്ണൽ xkb_state_component type)
{
    തിരികെ serialize_layout(&state->components, type);
}

xkb_led_mask_t
xkb_state_serialize_leds(സ്ഥിരം ഘടന xkb_state *state,
                         എണ്ണൽ xkb_state_component type)
{
    തിരികെ (type & XKB_STATE_LEDS)
        ? state->components.leds
        : 0;
}

സ്ഥിതം വരിയിൽ എണ്ണൽ xkb_keyboard_control_flags
serialize_controls(സ്ഥിരം ഘടന state_components *components,
                   എണ്ണൽ xkb_state_component type)
{
    തിരികെ (type & XKB_STATE_CONTROLS_EFFECTIVE)
        /*
         * Enable using the public API with the all the Controls values, except
         * the internal ones, if any.
         */
        ? (എണ്ണൽ xkb_keyboard_control_flags)
          (components->controls & CONTROL_ALL_BOOLEAN)
        : 0;
}

എണ്ണൽ xkb_keyboard_control_flags
xkb_state_serialize_controls(സ്ഥിരം ഘടന xkb_state *state,
                             എണ്ണൽ xkb_state_component type)
{
    തിരികെ serialize_controls(&state->components, type);
}

/**
 * Gets a modifier mask and returns the resolved effective mask; this
 * is needed because some modifiers can also map to other modifiers, e.g.
 * the "NumLock" modifier usually also sets the "Mod2" modifier.
 */
xkb_mod_mask_t
mod_mask_get_effective(ഘടന xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    /* Initialize the effective mask with its corresponding real mods. */
    xkb_mod_mask_t mask = mods & MOD_REAL_MASK_ALL;

    /* Resolve the virtual modifiers */
    സ്ഥിരം ഘടന xkb_mod *mod;
    xkb_mod_index_t i;
    xkb_vmods_enumerate(i, mod, &keymap->mods)
        എങ്കിൽ (mods & (UINT32_C(1) << i))
            mask |= mod->mapping;

    തിരികെ mask;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
പൂർണ്ണസംഖ്യ
xkb_state_mod_index_is_active(ഘടന xkb_state *state,
                              xkb_mod_index_t idx,
                              എണ്ണൽ xkb_state_component type)
{
    എങ്കിൽ (unlikely(idx >= xkb_keymap_അക്കം_mods(state->keymap)))
        തിരികെ -1;

    സ്ഥിരം xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    എങ്കിൽ (!mapping) {
        /* Modifier not mapped */
        തിരികെ 0;
    }
    /* WARNING: this may overmatch for virtual modifiers */
    തിരികെ (xkb_state_serialize_mods(state, type) & mapping) == mapping;
}

എണ്ണൽ {
    XKB_STATE_MATCH_FLAGS = XKB_STATE_MATCH_ANY
                          | XKB_STATE_MATCH_ALL
                          | XKB_STATE_MATCH_NON_EXCLUSIVE
};

/**
 * Helper function for xkb_state_mod_indices_are_active and
 * xkb_state_mod_names_are_active.
 */
സ്ഥിതം bool
match_mod_masks(ഘടന xkb_state *state,
                എണ്ണൽ xkb_state_component type,
                എണ്ണൽ xkb_state_match match,
                xkb_mod_mask_t wanted)
{
    സ്ഥിരം xkb_mod_mask_t active = xkb_state_serialize_mods(state, type);

    എങ്കിൽ (!(match & XKB_STATE_MATCH_NON_EXCLUSIVE) && (active & ~wanted))
        തിരികെ false;

    എങ്കിൽ (match & XKB_STATE_MATCH_ANY)
        തിരികെ active & wanted;

    /* Ensure all flags but XKB_STATE_MATCH_ALL are processed before */
    static_assert(XKB_STATE_MATCH_ALL ==
                  ((എണ്ണൽ xkb_state_match) XKB_STATE_MATCH_FLAGS &
                   ~(XKB_STATE_MATCH_ANY | XKB_STATE_MATCH_NON_EXCLUSIVE)),
                  "");

    തിരികെ (active & wanted) == wanted;
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, or -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
പൂർണ്ണസംഖ്യ
xkb_state_mod_indices_are_active(ഘടന xkb_state *state,
                                 എണ്ണൽ xkb_state_component type,
                                 എണ്ണൽ xkb_state_match match,
                                 ...)
{
    എങ്കിൽ (match & ~((എണ്ണൽ xkb_state_match) XKB_STATE_MATCH_FLAGS))
        തിരികെ -2;

    va_list ap;
    xkb_mod_mask_t wanted = 0;
    പൂർണ്ണസംഖ്യ ret = 0;
    സ്ഥിരം xkb_mod_index_t അക്കം_mods = xkb_keymap_അക്കം_mods(state->keymap);

    va_start(ap, match);
    ആകുമ്പോൾ (1) {
        xkb_mod_index_t idx = va_arg(ap, xkb_mod_index_t);
        എങ്കിൽ (idx == XKB_MOD_INVALID)
            നിർത്തുക;
        എങ്കിൽ (unlikely(idx >= അക്കം_mods)) {
            ret = -1;
            നിർത്തുക;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_end(ap);

    എങ്കിൽ (ret == -1)
        തിരികെ ret;

    എങ്കിൽ (!wanted) {
        /* Modifiers not mapped */
        തിരികെ 0;
    }

    തിരികെ match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
പൂർണ്ണസംഖ്യ
xkb_state_mod_name_is_active(ഘടന xkb_state *state, സ്ഥിരം അക്ഷരം *name,
                             എണ്ണൽ xkb_state_component type)
{
    സ്ഥിരം xkb_mod_index_t idx = xkb_keymap_mod_get_index(state->keymap, name);

    എങ്കിൽ (idx == XKB_MOD_INVALID)
        തിരികെ -1;

    തിരികെ xkb_state_mod_index_is_active(state, idx, type);
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ATTR_NULL_SENTINEL പൂർണ്ണസംഖ്യ
xkb_state_mod_names_are_active(ഘടന xkb_state *state,
                               എണ്ണൽ xkb_state_component type,
                               എണ്ണൽ xkb_state_match match,
                               ...)
{
    എങ്കിൽ (match & ~((എണ്ണൽ xkb_state_match) XKB_STATE_MATCH_FLAGS))
        തിരികെ -2;

    va_list ap;
    xkb_mod_mask_t wanted = 0;
    പൂർണ്ണസംഖ്യ ret = 0;

    va_start(ap, match);
    ആകുമ്പോൾ (1) {
        സ്ഥിരം അക്ഷരം *str = va_arg(ap, സ്ഥിരം അക്ഷരം *);
        എങ്കിൽ (str == ശൂന്യ)
            നിർത്തുക;
        സ്ഥിരം xkb_mod_index_t idx = xkb_keymap_mod_get_index(state->keymap, str);
        എങ്കിൽ (idx == XKB_MOD_INVALID) {
            ret = -1;
            നിർത്തുക;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_end(ap);

    എങ്കിൽ (ret == -1)
        തിരികെ ret;

    എങ്കിൽ (!wanted) {
        /* Modifiers not mapped */
        തിരികെ 0;
    }

    തിരികെ match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given group is active with the specified type(s), 0 if
 * not, or -1 if the group is invalid.
 */
പൂർണ്ണസംഖ്യ
xkb_state_layout_index_is_active(ഘടന xkb_state *state,
                                 xkb_layout_index_t idx,
                                 എണ്ണൽ xkb_state_component type)
{
    എങ്കിൽ (idx >= state->keymap->അക്കം_groups)
        തിരികെ -1;

    പൂർണ്ണസംഖ്യ ret = 0;
    എങ്കിൽ (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ret |= (state->components.group == idx);
    എങ്കിൽ (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret |= (state->components.base_group == (int32_t) idx);
    എങ്കിൽ (type & XKB_STATE_LAYOUT_LATCHED)
        ret |= (state->components.latched_group == (int32_t) idx);
    എങ്കിൽ (type & XKB_STATE_LAYOUT_LOCKED)
        ret |= (state->components.locked_group == (int32_t) idx);

    തിരികെ ret;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
പൂർണ്ണസംഖ്യ
xkb_state_layout_name_is_active(ഘടന xkb_state *state, സ്ഥിരം അക്ഷരം *name,
                                എണ്ണൽ xkb_state_component type)
{
    സ്ഥിരം xkb_layout_index_t idx =
        xkb_keymap_layout_get_index(state->keymap, name);

    എങ്കിൽ (idx == XKB_LAYOUT_INVALID)
        തിരികെ -1;

    തിരികെ xkb_state_layout_index_is_active(state, idx, type);
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
പൂർണ്ണസംഖ്യ
xkb_state_led_index_is_active(ഘടന xkb_state *state, xkb_led_index_t idx)
{
    എങ്കിൽ (idx >= state->keymap->അക്കം_leds ||
        state->keymap->leds[idx].name == XKB_ATOM_NONE)
        തിരികെ -1;

    തിരികെ !!(state->components.leds & (UINT32_C(1) << idx));
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
പൂർണ്ണസംഖ്യ
xkb_state_led_name_is_active(ഘടന xkb_state *state, സ്ഥിരം അക്ഷരം *name)
{
    സ്ഥിരം xkb_led_index_t idx = xkb_keymap_led_get_index(state->keymap, name);

    എങ്കിൽ (idx == XKB_LED_INVALID)
        തിരികെ -1;

    തിരികെ xkb_state_led_index_is_active(state, idx);
}

/**
 * See:
 * - XkbTranslateKeyCode(3), mod_rtrn return value, from libX11.
 * - MyEnhancedXkbTranslateKeyCode(), a modification of the above, from GTK+.
 */
സ്ഥിതം xkb_mod_mask_t
താക്കോൽ_get_consumed(ഘടന xkb_state *state, സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                 എണ്ണൽ xkb_consumed_mode mode)
{
    സ്ഥിരം xkb_layout_index_t group =
        xkb_state_താക്കോൽ_get_layout(state, താക്കോൽ->keycode);
    എങ്കിൽ (group == XKB_LAYOUT_INVALID)
        തിരികെ 0;

    xkb_mod_mask_t preserve = 0;
    xkb_mod_mask_t consumed = 0;

    സ്ഥിരം ഘടന xkb_താക്കോൽ_type_entry* സ്ഥിരം matching_entry =
        get_entry_for_താക്കോൽ_state(state, താക്കോൽ, group);
    എങ്കിൽ (matching_entry)
        preserve = matching_entry->preserve.mask;

    സ്ഥിരം ഘടന xkb_താക്കോൽ_type* സ്ഥിരം type = താക്കോൽ->groups[group].type;
    തിരഞ്ഞെടുപ്പ് (mode) {
    അവസ്ഥ XKB_CONSUMED_MODE_XKB:
        consumed = type->mods.mask;
        നിർത്തുക;

    അവസ്ഥ XKB_CONSUMED_MODE_GTK: {
        സ്ഥിരം ഘടന xkb_താക്കോൽ_type_entry* സ്ഥിരം no_mods_entry =
            get_entry_for_mods(type, 0);
        സ്ഥിരം xkb_level_index_t no_mods_leveli = no_mods_entry
                                               ? no_mods_entry->level
                                               : 0;
        സ്ഥിരം ഘടന xkb_level* സ്ഥിരം no_mods_level =
            &താക്കോൽ->groups[group].levels[no_mods_leveli];

        വേണ്ടി (darray_size_t i = 0; i < type->അക്കം_entries; i++) {
            സ്ഥിരം ഘടന xkb_താക്കോൽ_type_entry* സ്ഥിരം entry = &type->entries[i];
            എങ്കിൽ (!entry_is_active(entry))
                തുടരുക;

            സ്ഥിരം ഘടന xkb_level* സ്ഥിരം level =
                &താക്കോൽ->groups[group].levels[entry->level];
            എങ്കിൽ (XkbLevelsSameSyms(level, no_mods_level))
                തുടരുക;

            എങ്കിൽ (entry == matching_entry || one_bit_set(entry->mods.mask))
                consumed |= entry->mods.mask & ~entry->preserve.mask;
        }
        നിർത്തുക;
    }
    }

    തിരികെ consumed & ~preserve;
}

പൂർണ്ണസംഖ്യ
xkb_state_mod_index_is_consumed2(ഘടന xkb_state *state, xkb_keycode_t kc,
                                 xkb_mod_index_t idx,
                                 എണ്ണൽ xkb_consumed_mode mode)
{
    സ്ഥിരം ഘടന xkb_താക്കോൽ* സ്ഥിരം താക്കോൽ = Xkb_താക്കോൽ(state->keymap, kc);

    എങ്കിൽ (unlikely(!താക്കോൽ || idx >= xkb_keymap_അക്കം_mods(state->keymap)))
        തിരികെ -1;

    സ്ഥിരം xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    എങ്കിൽ (!mapping) {
        /* Modifier not mapped */
        തിരികെ 0;
    }
    തിരികെ (mapping & താക്കോൽ_get_consumed(state, താക്കോൽ, mode)) == mapping;
}

പൂർണ്ണസംഖ്യ
xkb_state_mod_index_is_consumed(ഘടന xkb_state *state, xkb_keycode_t kc,
                                xkb_mod_index_t idx)
{
    തിരികെ xkb_state_mod_index_is_consumed2(state, kc, idx,
                                            XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_mod_mask_remove_consumed(ഘടന xkb_state *state, xkb_keycode_t kc,
                                   xkb_mod_mask_t mask)
{
    സ്ഥിരം ഘടന xkb_താക്കോൽ* സ്ഥിരം താക്കോൽ = Xkb_താക്കോൽ(state->keymap, kc);

    എങ്കിൽ (!താക്കോൽ)
        തിരികെ 0;

    തിരികെ resolve_to_canonical_mods(state->keymap, mask) &
           ~താക്കോൽ_get_consumed(state, താക്കോൽ, XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_താക്കോൽ_get_consumed_mods2(ഘടന xkb_state *state, xkb_keycode_t kc,
                                 എണ്ണൽ xkb_consumed_mode mode)
{
    തിരഞ്ഞെടുപ്പ് (mode) {
    അവസ്ഥ XKB_CONSUMED_MODE_XKB:
    അവസ്ഥ XKB_CONSUMED_MODE_GTK:
        നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
        log_err_func(state->keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "unrecognized consumed modifiers mode: %d\n", mode);
        തിരികെ 0;
    }

    സ്ഥിരം ഘടന xkb_താക്കോൽ* സ്ഥിരം താക്കോൽ = Xkb_താക്കോൽ(state->keymap, kc);
    എങ്കിൽ (!താക്കോൽ)
        തിരികെ 0;

    തിരികെ താക്കോൽ_get_consumed(state, താക്കോൽ, mode);
}

xkb_mod_mask_t
xkb_state_താക്കോൽ_get_consumed_mods(ഘടന xkb_state *state, xkb_keycode_t kc)
{
    തിരികെ xkb_state_താക്കോൽ_get_consumed_mods2(state, kc, XKB_CONSUMED_MODE_XKB);
}

/*******************************************************************************
 * Server state machine API
 ******************************************************************************/

/** Entry to keep track of the state of an overlaid key */
ഘടന xkb_overlaid_താക്കോൽ {
    /** The source key being overlaid */
    സ്ഥിരം ഘടന xkb_താക്കോൽ *old;
    /** The target key */
    സ്ഥിരം ഘടന xkb_താക്കോൽ *new;
    /** Counter of key currently down */
    പൂർണ്ണസംഖ്യ refcnt;
};

ഘടന xkb_shortcuts_config_entry {
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
ഘടന xkb_machine {
    /** Inherits from `xkb_server_state` */
    ഘടന xkb_server_state base;

    /* Extra server-specific state data */

    /** Keyboard overlays handling */
    ഘടന {
        /**
         * Current overlaid keys
         *
         * When a key is overlaid, it remained overlaid in this configuration
         * until totally released, i.e. every key press was matched with the
         * corresponding key release.
         */
        darray(ഘടന xkb_overlaid_താക്കോൽ) താക്കോലുകൾ;
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
    ഘടന machine_config {
        /** Modifiers remapping */
        ഘടന machine_modifiers_config {
            /** Real modifier mask */
            xkb_mod_mask_t mask;
            /** Modifiers re-mappings */
            darray_size_t mappings_അക്കം;
            ഘടന machine_mods_mapping {
                xkb_mod_mask_t source;
                xkb_mod_mask_t target;
            } * mappings ATTR_COUNTED_BY(mappings_അക്കം);
        } modifiers;

        /** Shortcuts tweak */
        ഘടന machine_shortcuts_config {
            /** Substitution entries */
            ഘടന xkb_shortcuts_config_entry *entries;
        } shortcuts;
    } config;

    /** Mouse keys handling */
    ഘടന {
        /** Default pointer button */
        xkb_pointer_button_index_t default_button;
        /** Mask of the locked buttons */
        xkb_pointer_button_mask_t locked_buttons;
    } mouse;

    എണ്ണൽ xkb_machine_flags flags;
};

തരനിർവചനം darray(ഘടന machine_mods_mapping) machine_mods_mappings;

ഘടന xkb_machine_builder {
    ഘടന xkb_keymap *keymap;

    /** Controls */
    ഘടന {
        /** Accessibility flags */
        ഘടന {
            എണ്ണൽ xkb_a11y_flags affect_flags;
            എണ്ണൽ xkb_a11y_flags flags;
        } a11y;
    } controls;

    /** Modifiers re-mapping */
    machine_mods_mappings mods;

    /** Shortcuts tweak */
    ഘടന xkb_shortcuts_config_options {
        /** Substitution entries */
        darray(ഘടന xkb_shortcuts_config_entry) entries;
    } shortcuts;

    എണ്ണൽ xkb_machine_flags machine_flags;
    എണ്ണൽ xkb_machine_builder_flags builder_flags;
    പൂർണ്ണസംഖ്യ refcnt;
};

ഘടന xkb_machine_builder *
xkb_machine_builder_new(
    ഘടന xkb_keymap * restrict keymap,
    സ്ഥിരം ഘടന xkb_machine_builder_config * restrict config,
    എണ്ണൽ xkb_status * restrict status
)
{
    /* Handle default configuration */
    സ്ഥിതം സ്ഥിരം ഘടന xkb_machine_builder_config default_config = {
        .size = വലുപ്പം(default_config)
    };
    എങ്കിൽ (!config)
        config = &default_config;

    /* Check ABI compatibility */
    സ്ഥിരം എണ്ണൽ xkb_status status_ = xkb_check_state_abi(config);
    എങ്കിൽ (status_) {
        xkb_log_abi_error(keymap->ctx, __func__, status_);
        എങ്കിൽ (status)
            *status = status_;
        തിരികെ ശൂന്യ;
    }

    /* Sanitize input */
    സ്ഥിരം എണ്ണൽ xkb_machine_builder_flags invalid_builder_flags
        = config->builder_flags
        & ~(എണ്ണൽ xkb_machine_builder_flags)XKB_MACHINE_BUILDER_FLAGS_മൂല്യങ്ങൾ;
    എങ്കിൽ (invalid_builder_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS_,
                     "unrecognized machine builder flags: 0x%x\n",
                     invalid_builder_flags);
        എങ്കിൽ (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS;
        തിരികെ ശൂന്യ;
    }

    സ്ഥിരം എണ്ണൽ xkb_machine_flags invalid_machine_flags
        = (എണ്ണൽ xkb_machine_flags)config->machine_flags
        & ~(എണ്ണൽ xkb_machine_flags)XKB_MACHINE_FLAGS_മൂല്യങ്ങൾ;
    എങ്കിൽ (invalid_machine_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS_,
                     "unrecognized state machine flags: 0x%x\n",
                     invalid_machine_flags);
        എങ്കിൽ (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS;
        തിരികെ ശൂന്യ;
    }

    ഘടന xkb_machine_builder * സ്ഥിരം builder = calloc(1, വലുപ്പം(*builder));
    എങ്കിൽ (!builder) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine builder\n");
        എങ്കിൽ (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        തിരികെ ശൂന്യ;
    }

    *builder = (ഘടന xkb_machine_builder) {
        .keymap = xkb_keymap_ref(keymap),
        .refcnt = 1,
        .builder_flags = (എണ്ണൽ xkb_machine_builder_flags)config->builder_flags,
        .machine_flags = (എണ്ണൽ xkb_machine_flags)config->machine_flags,
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

    എങ്കിൽ (status)
        *status = XKB_SUCCESS;

    തിരികെ builder;
}

ഘടന xkb_machine_builder *
xkb_machine_builder_ref(ഘടന xkb_machine_builder *builder)
{
    പ്രതിജ്ഞാ(builder->refcnt > 0);
    builder->refcnt++;
    തിരികെ builder;
}

ശൂന്യം
xkb_machine_builder_unref(ഘടന xkb_machine_builder *builder)
{
    പ്രതിജ്ഞാ(!builder || builder->refcnt > 0);
    എങ്കിൽ (!builder || --builder->refcnt > 0)
        തിരികെ;

    darray_free(builder->shortcuts.entries);
    darray_free(builder->mods);
    xkb_keymap_unref(builder->keymap);
    സ്വതന്ത്രം(builder);
}

ഘടന xkb_keymap *
xkb_machine_builder_get_keymap(സ്ഥിരം ഘടന xkb_machine_builder *builder)
{
    /* Reference count is not updated. See API doc. */
    തിരികെ builder->keymap;
}

എണ്ണൽ xkb_status
xkb_machine_builder_update_a11y(
    ഘടന xkb_machine_builder * restrict builder,
    സ്ഥിരം ഘടന xkb_machine_builder_a11y_update * restrict update)
{
    /* Check ABI compatibility */
    എണ്ണൽ xkb_status status = xkb_check_state_abi(update);
    എങ്കിൽ (status != XKB_SUCCESS) {
        xkb_log_abi_error(builder->keymap->ctx, __func__, status);
        തിരികെ status;
    }

    സ്ഥിരം എണ്ണൽ xkb_a11y_flags invalid_flags =
        ~(എണ്ണൽ xkb_a11y_flags)XKB_A11_Y_FLAGS_മൂല്യങ്ങൾ;

    എങ്കിൽ (update->affect_flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y affected flags: %#x\n",
                __func__, update->affect_flags & invalid_flags);
        തിരികെ XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }
    എങ്കിൽ (update->flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y flags: %#x\n",
                __func__, update->flags & invalid_flags);
        തിരികെ XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }

    builder->controls.a11y.affect_flags |= update->affect_flags;
    builder->controls.a11y.flags &= ~update->affect_flags;
    builder->controls.a11y.flags |= (update->flags & update->affect_flags);

    തിരികെ XKB_SUCCESS;
}

എണ്ണൽ xkb_status
xkb_machine_builder_update_mods_remap(
    ഘടന xkb_machine_builder * restrict builder,
    സ്ഥിരം ഘടന xkb_machine_builder_mods_remap_update * restrict update
)
{
    എങ്കിൽ (!update->source) {
        എങ്കിൽ (!update->target) {
            /* Reset mappings */
            darray_resize(builder->mods, 0);
            തിരികെ XKB_SUCCESS;
        } അല്ലെങ്കിൽ {
            തിരികെ XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
        }
    }

    /* Check the modifiers against the keymap */
    സ്ഥിരം xkb_mod_mask_t invalid = ~builder->keymap->canonical_state_mask;
    എങ്കിൽ ((update->source & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid source modifiers: 0x%"PRIx32"\n",
                __func__, update->source);
        തിരികെ XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    എങ്കിൽ ((update->target & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid target modifiers: 0x%"PRIx32"\n",
                __func__, update->target);
        തിരികെ XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    ഘടന machine_mods_mapping *mapping = ശൂന്യ;
    darray_size_t m = 0;
    darray_enumerate(m, mapping, builder->mods) {
        എങ്കിൽ (mapping->source == update->source) {
            എങ്കിൽ (!update->target) {
                /* Remove mapping */
                darray_remove(builder->mods, m);
            } അല്ലെങ്കിൽ {
                /* Update mapping */
                mapping->target = update->target;
            }
            തിരികെ XKB_SUCCESS;
        }
    }

    എങ്കിൽ (update->target) {
        /* Append new mapping */
        darray_append(
            builder->mods,
            (ഘടന machine_mods_mapping) {
                .source = update->source,
                .target = update->target
            }
        );
    } അല്ലെങ്കിൽ {
        /* Ignore missing mapping */
    }

    തിരികെ XKB_SUCCESS;
}

എണ്ണൽ xkb_status
xkb_machine_builder_update_shortcut_override(
    ഘടന xkb_machine_builder * restrict builder,
    സ്ഥിരം ഘടന xkb_machine_builder_shortcut_override_update * restrict update
)
{
    ഘടന xkb_keymap *keymap = builder->keymap;

    /* Check the modifiers against the keymap */
    സ്ഥിരം xkb_mod_mask_t invalid = ~keymap->canonical_state_mask;
    എങ്കിൽ ((update->affect_mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid affected modifiers: 0x%"PRIx32"\n",
                __func__, update->affect_mods);
        തിരികെ XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    എങ്കിൽ ((update->mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid modifiers: 0x%"PRIx32"\n",
                __func__, update->mods);
        തിരികെ XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    /* Check the layout indices against the keymap */
    എങ്കിൽ (update->source >= keymap->അക്കം_groups &&
        update->source != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "%s: Invalid source layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->അക്കം_groups, update->source + 1);
        തിരികെ XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }
    എങ്കിൽ (update->target >= keymap->അക്കം_groups &&
        update->target != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "%s: Invalid target layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->അക്കം_groups, update->target + 1);
        തിരികെ XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    ഘടന xkb_shortcuts_config_options * സ്ഥിരം config = &builder->shortcuts;

    /* Resize array & initialize new entries, if relevant */
    എങ്കിൽ (update->source >= darray_size(config->entries) &&
        update->source != XKB_LAYOUT_INVALID) {
        എങ്കിൽ (update->target == update->source) {
            /* Skip default setting */
            തിരികെ XKB_SUCCESS;
        }
        xkb_layout_index_t new = darray_size(config->entries);
        darray_resize(config->entries, update->source + 1);
        വേണ്ടി (; new <= update->source; new++) {
            darray_item(config->entries, new) =
                (ഘടന xkb_shortcuts_config_entry) {
                    .target = XKB_LAYOUT_INVALID,
                    .mods = 0
                };
        }
    }

    ഘടന xkb_shortcuts_config_entry *entry;
    xkb_layout_index_t source = (update->source == XKB_LAYOUT_INVALID)
        ? 0               /* Loop over all entries */
        : update->source; /* Single entry */
    darray_foreach_from(entry, config->entries, source) {
        എങ്കിൽ (update->source == XKB_LAYOUT_INVALID &&
            entry->target == XKB_LAYOUT_INVALID) {
            /* Skip previous deactivated entry */
            source++;
            തുടരുക;
        }

        എങ്കിൽ (update->affect_mods) {
            entry->mods &= ~update->affect_mods;
            entry->mods |= (update->affect_mods & update->mods);
        }

        എങ്കിൽ (update->source != XKB_LAYOUT_INVALID) {
            entry->target = (update->target == source)
                ? XKB_LAYOUT_INVALID
                : update->target;
            /* Single entry: exit loop */
            നിർത്തുക;
        }
        source++;
    }

    തിരികെ XKB_SUCCESS;
}

/** Compare 2 modifiers combinations */
സ്ഥിതം പൂർണ്ണസംഖ്യ
cmp_mod_masks(സ്ഥിരം ശൂന്യം *a, സ്ഥിരം ശൂന്യം *b)
{
    സ്ഥിരം xkb_mod_mask_t m1 = *((xkb_mod_mask_t *) a);
    സ്ഥിരം xkb_mod_mask_t m2 = *((xkb_mod_mask_t *) b);
    എങ്കിൽ (m1 == m2)
        തിരികെ 0;

    എങ്കിൽ (!m1)
        തിരികെ +1;
    എങ്കിൽ (!m2)
        തിരികെ -1;

    സ്ഥിരം xkb_mod_mask_t overlap = (m1 & m2);
    എങ്കിൽ (overlap == m1)
        തിരികെ +1; /* m1 is a subset of m2 */
    എങ്കിൽ (overlap == m2)
        തിരികെ -1; /* m2 is a subset of m1 */

    /* Compare active modifiers numbers */
    സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ count1 = popcount32(m1);
    സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ count2 = popcount32(m2);
    എങ്കിൽ (count1 > count2)
        തിരികെ -1;
    എങ്കിൽ (count1 < count2)
        തിരികെ +1;

    /* Give priority to real modifiers */
    തിരികെ (m1 < m2) ? -1 : +1;
}

സ്ഥിതം എണ്ണൽ xkb_status
machine_set_mods(ഘടന xkb_machine *sm,
                 സ്ഥിരം machine_mods_mappings *raw_mappings)
{
    എങ്കിൽ (!darray_empty(*raw_mappings)) {
        machine_mods_mappings mappings = darray_new();
        xkb_mod_mask_t mask = 0;
        സ്ഥിരം ഘടന machine_mods_mapping *mapping;
        സ്ഥിരം xkb_mod_mask_t invalid =
            ~sm->base.base.keymap->canonical_state_mask;
        darray_foreach(mapping, *raw_mappings) {
            എങ്കിൽ (!mapping->source || !mapping->target ||
                (mapping->source & invalid) || (mapping->target & invalid))
                തുടരുക;
            darray_append(mappings, *mapping);
            mask |= mapping->source;
        }

        /*
         * Re-order the mappings, so that combinations have higher priority
         * if they are superset of other combinations.
         * For other type of overlaps, see the sorting function.
         */
        qsort(darray_ഇനങ്ങൾ(mappings), darray_size(mappings),
              വലുപ്പം(*darray_ഇനങ്ങൾ(mappings)), &cmp_mod_masks);

        darray_steal(
            mappings,
            &sm->config.modifiers.mappings,
            &sm->config.modifiers.mappings_അക്കം
        );
        sm->config.modifiers.mask = mask;
    } അല്ലെങ്കിൽ {
        sm->config.modifiers = (ഘടന machine_modifiers_config) {
            .mappings = ശൂന്യ,
            .mappings_അക്കം = 0,
            .mask = 0
        };
    }

    തിരികെ XKB_SUCCESS;
}

സ്ഥിതം എണ്ണൽ xkb_status
machine_set_shortcuts(
    ഘടന xkb_machine * restrict sm,
    സ്ഥിരം ഘടന xkb_shortcuts_config_options * restrict options
)
{
    എങ്കിൽ (darray_empty(options->entries)) {
        sm->config.shortcuts = (ഘടന machine_shortcuts_config) {
            .entries = ശൂന്യ
        };
        തിരികെ XKB_SUCCESS;
    }

    ഘടന xkb_keymap * സ്ഥിരം keymap = sm->base.base.keymap;

    ഘടന xkb_shortcuts_config_entry * entries =
        calloc(keymap->അക്കം_groups, വലുപ്പം(*entries));
    എങ്കിൽ (!entries)
        തിരികെ XKB_ERROR_ALLOCATION_FAILURE;

    സ്ഥിരം xkb_layout_index_t എണ്ണം = MIN(
        keymap->അക്കം_groups,
        (xkb_layout_index_t) darray_size(options->entries)
    );

    bool some_entries = false;
    വേണ്ടി (xkb_layout_index_t l = 0; l < എണ്ണം; l++) {
        /* Sanitize layout target */
        സ്ഥിരം xkb_layout_index_t target =
            darray_item(options->entries, l).target;
        /* Sanitize modifier mask */
        സ്ഥിരം xkb_mod_mask_t mods = (
            darray_item(options->entries, l).mods &
            sm->base.base.keymap->canonical_state_mask
        );
        എങ്കിൽ (target < keymap->അക്കം_groups && target != l && mods) {
            entries[l] = (ഘടന xkb_shortcuts_config_entry) {
                .target = target,
                .mods = mods
            };
            some_entries = true;
        } അല്ലെങ്കിൽ {
            entries[l] = (ഘടന xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
            തുടരുക;
        }
    }

    എങ്കിൽ (!some_entries) {
        സ്വതന്ത്രം(entries);
        entries = ശൂന്യ;
    } അല്ലെങ്കിൽ {
        /* Initialize groups with no entries */
        വേണ്ടി (xkb_layout_index_t l = എണ്ണം; l < keymap->അക്കം_groups; l++) {
            entries[l] = (ഘടന xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
        }
    }

    sm->config.shortcuts = (ഘടന machine_shortcuts_config) {
        .entries = entries
    };
    തിരികെ XKB_SUCCESS;
}

ഘടന xkb_machine *
xkb_machine_new(സ്ഥിരം ഘടന xkb_machine_builder * restrict builder,
                എണ്ണൽ xkb_status * restrict status)
{
    ഘടന xkb_machine * സ്ഥിരം machine = calloc(1, വലുപ്പം(*machine));
    എങ്കിൽ (!machine) {
        log_err_func1(builder->keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine\n");
        എങ്കിൽ (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        തിരികെ ശൂന്യ;
    }

    xkb_server_state_init(&machine->base, builder->keymap, SERVER_STATE,
                          builder->controls.a11y.affect_flags,
                          builder->controls.a11y.flags);
    machine->flags = builder->machine_flags;
    എങ്കിൽ (!(machine->flags &
          XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_താക്കോൽ_FIX))
    {
        machine->base.flags |= STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_താക്കോൽ;
    }
    എങ്കിൽ (machine->flags & XKB_MACHINE_UNLOCK_MODS_ON_PRESS) {
        machine->base.flags |= STATE_UNLOCK_MODS_ON_PRESS;
    }

    എണ്ണൽ xkb_status status_;
    എങ്കിൽ ((status_ = machine_set_mods(machine, &builder->mods)) != XKB_SUCCESS ||
        (status_ = machine_set_shortcuts(machine, &builder->shortcuts)) !=
        XKB_SUCCESS) {
        *status = status_;
        പോകുക error;
    }

    darray_init(machine->overlays.താക്കോലുകൾ);
    machine->mouse.default_button = XKB_POINTER_BUTTON_MIN;

    എങ്കിൽ (status)
        *status = XKB_SUCCESS;

    തിരികെ machine;

error:
    xkb_machine_unref(machine);
    തിരികെ ശൂന്യ;
}

ഘടന xkb_machine *
xkb_machine_ref(ഘടന xkb_machine *sm)
{
    പ്രതിജ്ഞാ(sm->base.base.refcnt > 0);
    sm->base.base.refcnt++;
    തിരികെ sm;
}

ശൂന്യം
xkb_machine_unref(ഘടന xkb_machine *sm)
{
    പ്രതിജ്ഞാ(!sm || sm->base.base.refcnt > 0);
    എങ്കിൽ (!sm || --sm->base.base.refcnt > 0)
        തിരികെ;

    xkb_state_destroy(&sm->base.base);
    darray_free(sm->overlays.താക്കോലുകൾ);
    സ്വതന്ത്രം(sm->config.shortcuts.entries);
    സ്വതന്ത്രം(sm->config.modifiers.mappings);
    സ്വതന്ത്രം(sm);
}

ഘടന xkb_keymap *
xkb_machine_get_keymap(സ്ഥിരം ഘടന xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    തിരികെ sm->base.base.keymap;
}

ഘടന xkb_state *
xkb_machine_get_state(ഘടന xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    തിരികെ (ഘടന xkb_state *)sm;
}

ഘടന xkb_state *
xkb_state_new_from_machine(സ്ഥിരം ഘടന xkb_machine * restrict machine,
                           എണ്ണൽ xkb_status * restrict status)
{
    /* Creation */
    എണ്ണൽ xkb_status status_;
    ഘടന xkb_state * സ്ഥിരം state = xkb_state_new_with_mode(
        machine->base.base.keymap,
        XKB_STATE_MODE_SERVER_QUERY,
        &status_
    );
    എങ്കിൽ (status_ != XKB_SUCCESS) {
        എങ്കിൽ (status)
            *status = status_;
        തിരികെ ശൂന്യ;
    }

    പ്രതിജ്ഞാ(state->mode == SERVER_COMPANION);
    പ്രതിജ്ഞാ(state->refcnt == 1);

    /* Copy machine’s state */
    *state = machine->base.base;

    /* Restore previous values */
    state->mode = SERVER_COMPANION;
    state->refcnt = 1;

    എങ്കിൽ (status)
        *status = XKB_SUCCESS;
    തിരികെ state;
}

സ്ഥിതം ശൂന്യം
machine_update_overlays(ഘടന xkb_machine *sm)
{
    /*
     * Overlays indices are stored 1-indexed in nibbles: the lowest
     * nibble corresponds to the latest activated index.
     */

    സ്ഥിരം xkb_overlay_mask_t mask =
        overlays_from_controls(sm->base.base.components.controls);
    xkb_overlay_mask_t added = mask & ~sm->overlays.enabled;

    /* Remove overlays no longer enabled and keep relative order */
    uint32_t order = sm->overlays.order;
    സ്ഥിരം xkb_overlay_index_t overlay_max =
        format_max_overlays(sm->base.base.keymap->format);
    വേണ്ടി (uint8_t n = 0; n < overlay_max;) {
        xkb_overlay_index_t overlay_idx = (order >> (n * 4)) & 0xf;
        എങ്കിൽ (!overlay_idx)
            നിർത്തുക;
        സ്ഥിരം xkb_overlay_mask_t overlay_mask =
            (xkb_overlay_mask_t)(1u << (overlay_idx - 1u) /* k is 1-indexed */);
        എങ്കിൽ (overlay_mask & mask) {
            /* no duplicates */
            added &= ~overlay_mask;
            n++;
        } അല്ലെങ്കിൽ {
            /* remove overlay */
            സ്ഥിരം uint32_t head = (UINT32_C(1) << (n * 4)) - 1;
            order = (order & head) | ((order >> 4) & ~head);
        }
    }

    /* Add new overlays in ascending order */
    വേണ്ടി (xkb_overlay_index_t k = 0; added; k++, added >>= 1) {
        എങ്കിൽ (added & 0x1) {
            order <<= 4;
            order |= k + 1u; /* k is 0-indexed */
        }
    }

    sm->overlays.order = order;
    sm->overlays.enabled = mask;
}

എണ്ണൽ xkb_status
xkb_machine_process_synthetic(ഘടന xkb_machine *sm,
                              സ്ഥിരം ഘടന xkb_synthetic_update *update,
                              ഘടന xkb_events *events)
{
    /* Check ABI compatibility */
    എണ്ണൽ xkb_status status =
        check_state_update_abi(sm->base.base.keymap->ctx, update);
    എങ്കിൽ (status != XKB_SUCCESS)
        തിരികെ status;

    ഘടന xkb_server_state * സ്ഥിരം state = &sm->base;
    സ്ഥിരം ഘടന state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    darray_size(events->queue) = 0;
    events->next = 0;

    /* Update parametrized controls first */
    എങ്കിൽ (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        എങ്കിൽ (status != XKB_SUCCESS)
            തിരികെ status;
    }

    എങ്കിൽ (update->components) {
        സ്ഥിരം ഘടന xkb_state_components_update * സ്ഥിരം components =
            update->components;
        /* Update boolean controls first */
        എങ്കിൽ (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, events);
        }

        state_update_latched_locked(state, components, events);
    }

    xkb_state_update_derived(&state->base);

    സ്ഥിരം എണ്ണൽ xkb_state_component changed = get_state_component_changes(
        &previous_components, &state->base.components
    );
    എങ്കിൽ (changed) {
        // TODO: latch controls
        എങ്കിൽ (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        /* Create event only if some component actually changed */
        darray_append(events->queue, (ഘടന xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .changed = changed,
                .components = state->base.components
            }
        });

        /* Terminate the frame */
        darray_append(events->queue, (ഘടന xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    തിരികെ XKB_SUCCESS;
}

സ്ഥിതം ssize_t
do_remap_modifiers(സ്ഥിരം ഘടന machine_modifiers_config * restrict mappings,
                   ഘടന xkb_state * restrict state,
                   ഘടന xkb_events * restrict events,
                   സ്ഥിരം ഘടന xkb_താക്കോൽ * restrict താക്കോൽ)
{
    എങ്കിൽ (!(mappings->mask & state->components.mods))
        തിരികെ -1;

    സ്ഥിരം xkb_layout_index_t layout = state_താക്കോൽ_get_layout(state, താക്കോൽ);
    എങ്കിൽ (layout >= താക്കോൽ->അക്കം_groups)
        തിരികെ -1;

    സ്ഥിരം ഘടന xkb_താക്കോൽ_type * സ്ഥിരം type = താക്കോൽ->groups[layout].type;
    xkb_mod_mask_t affect_mods = 0;
    xkb_mod_mask_t mods = 0;
    വേണ്ടി (darray_size_t m = 0; m < mappings->mappings_അക്കം; m++) {
        സ്ഥിരം ഘടന machine_mods_mapping * സ്ഥിരം mapping =
            &mappings->mappings[m];
        എങ്കിൽ (/* Skip not matching active mods */
            (mapping->source & state->components.mods) == mapping->source &&
            /* Skip overlap with previous active mappings */
            !(mapping->source & affect_mods) &&
            /* Skip if the key type uses some of the source modifiers */
            !(mapping->source & type->mods.mask)) {
            affect_mods |= mappings->mappings[m].source;
            mods |= mappings->mappings[m].target;
        }
    }

    എങ്കിൽ (!affect_mods)
        തിരികെ -1;

    ഘടന xkb_state new = *state;
    new.components.base_mods = (new.components.base_mods & ~affect_mods) | mods;
    new.components.latched_mods = (new.components.latched_mods & ~affect_mods);
    new.components.locked_mods = (new.components.locked_mods & ~affect_mods);
    xkb_state_update_derived(&new);

    ssize_t event_idx = -1;
    സ്ഥിരം എണ്ണൽ xkb_state_component changed =
        get_state_component_changes(&state->components, &new.components);
    എങ്കിൽ (changed) {
        event_idx = (ssize_t) darray_size(events->queue);
        darray_append(events->queue, (ഘടന xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = new.components,
                .changed = changed
            }
        });
    }

    state->components.mods = new.components.mods;
    തിരികെ event_idx;
}

/**
 * Activate an alternative layout if some modifiers are active, typically
 * `Control`, `Alt` or `Super`.
 *
 * The tweak consists of computing a *base* layout such that the *effective*
 * layout is the corresponding target layout.
 */
സ്ഥിതം ssize_t
do_shortcuts_tweak(സ്ഥിരം ഘടന machine_shortcuts_config *config,
                   ഘടന xkb_state *state,
                   സ്ഥിരം ഘടന state_components *previous_components,
                   ഘടന xkb_events *events, ssize_t remap_event)
{
    എങ്കിൽ (!config->entries) {
        /* No shortcuts tweak */
        തിരികെ remap_event;
    }

    സ്ഥിരം ഘടന xkb_shortcuts_config_entry * സ്ഥിരം entry =
        &config->entries[state->components.group];

    എങ്കിൽ (entry->target != XKB_LAYOUT_INVALID &&
        (entry->mods & state->components.mods)) {
        /*
         * Activate shortcuts tweak:
         * 1. The real base group is saved by the caller, to be restored by
         *    later by a call to undo_tweaks().
         * 2. Compute the new base group so that the resulting effective group
         *    is the target.
         * 3. Recompute derived state components.
         */
        ഘടന xkb_state new = *state;
        എങ്കിൽ (remap_event < 0) {
            /* Create new event */
            remap_event = (ssize_t) darray_size(events->queue);
            darray_append(events->queue, (ഘടന xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = {0},
                    .changed = 0
                }
            });
        } അല്ലെങ്കിൽ {
            /* Merge with last remap event */
            new.components =
                darray_item(events->queue, remap_event).components.components;
        }
        new.components.base_group
            = (int32_t) entry->target
            - state->components.latched_group
            - state->components.locked_group;
        xkb_state_update_derived(&new);

        സ്ഥിരം എണ്ണൽ xkb_state_component changed =
            get_state_component_changes(previous_components, &new.components);
        darray_item(events->queue, remap_event).components.changed = changed;
        darray_item(events->queue, remap_event).components.components =
            new.components;

        state->components.group = new.components.group;
    } അല്ലെങ്കിൽ {
        /* Deactivated entry or modifiers do not match */
    }
    തിരികെ remap_event;
}

സ്ഥിതം ശൂന്യം
undo_tweaks(സ്ഥിരം ഘടന xkb_state *state,
            സ്ഥിരം ഘടന state_components *initial_components,
            ഘടന xkb_events *events)
{
    /* Get last component event */
    സ്ഥിരം ഘടന xkb_event *event = ശൂന്യ;
    darray_foreach_reverse(event, events->queue) {
        എങ്കിൽ (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS)
            നിർത്തുക;
    }
    എങ്കിൽ (!event)
        തിരികെ;

    /* Restore state */
    സ്ഥിരം എണ്ണൽ xkb_state_component changed =
        get_state_component_changes(initial_components,
                                    &event->components.components);
    എങ്കിൽ (changed) {
        darray_append(events->queue, (ഘടന xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = *initial_components,
                .changed = changed
            }
        });
    }
}

സ്ഥിതം സ്ഥിരം ഘടന xkb_താക്കോൽ *
process_overlayable_താക്കോൽ(ഘടന xkb_machine *sm,
                        സ്ഥിരം ഘടന xkb_താക്കോൽ * താക്കോൽ,
                        എണ്ണൽ xkb_താക്കോൽ_direction direction)
{
    /* Check if key is currently overlaid */
    ഘടന xkb_overlaid_താക്കോൽ *entry = ശൂന്യ;
    ഘടന xkb_overlaid_താക്കോൽ *available_entry = ശൂന്യ;
    darray_foreach(entry, sm->overlays.താക്കോലുകൾ) {
        എങ്കിൽ (entry->old == താക്കോൽ) {
            /* Maintain the key overlaid */
            തിരഞ്ഞെടുപ്പ് (direction) {
            അവസ്ഥ XKB_താക്കോൽ_DOWN:
                entry->refcnt++;
                നിർത്തുക;
            അവസ്ഥ XKB_താക്കോൽ_REPEATED:
                നിർത്തുക;
            അവസ്ഥ XKB_താക്കോൽ_UP:
            സ്ഥിരസ്ഥിതി:
                entry->refcnt--;
                നിർത്തുക;
            }
            എങ്കിൽ (entry->refcnt <= 0)
                entry->old = ശൂന്യ;
            തിരികെ entry->new;
        } അല്ലെങ്കിൽ എങ്കിൽ (!entry->old && !available_entry) {
            /* First free spot */
            available_entry = entry;
        }
    }
    എങ്കിൽ (direction == XKB_താക്കോൽ_DOWN) {
        /* Enable key overlay */

        /*
         * Get overlay index: select the latest activated overlay
         * containing the key.
         */
        സ്ഥിരം ഘടന xkb_താക്കോൽ *new = താക്കോൽ;

        എങ്കിൽ (താക്കോൽ->overlays & sm->overlays.enabled) {
            /* Some relevant overlay is active */
            വേണ്ടി (uint32_t അട്ടി = sm->overlays.order; അട്ടി; അട്ടി >>= 4) {
                static_assert(XKB_OVERLAY_എണ്ണം == 8, "");
                സ്ഥിരം xkb_overlay_index_t overlay = (അട്ടി & 0xf) - 1;
                സ്ഥിരം xkb_overlay_mask_t mask =
                    (xkb_overlay_mask_t)(1u << overlay);
                എങ്കിൽ (താക്കോൽ->overlays & mask) {
                    /* Get new key */
                    എങ്കിൽ (താക്കോൽ->overlays_inline) {
                        /* Stored inline */
                        new = താക്കോൽ->overlay_താക്കോൽ;
                    } അല്ലെങ്കിൽ {
                        /* Stored in sparse array */
                        സ്ഥിരം xkb_overlay_mask_t താഴെയുള്ള = (xkb_overlay_mask_t)(
                            താക്കോൽ->overlays & (xkb_overlay_mask_t)(mask - 1u)
                        );
                        /* index = number of set bits strictly below bit */
                        സ്ഥിരം xkb_overlay_index_t index =
                            (xkb_overlay_index_t)popcount32(താഴെയുള്ള);
                        new = താക്കോൽ->overlays_താക്കോലുകൾ[index];
                    }
                    നിർത്തുക;
                }
            }
        } അല്ലെങ്കിൽ {
            /*
             * No relevant overlay is active: block future overlays while
             * the key is pressed.
             */
        }

        /* Get new entry */
        എങ്കിൽ (available_entry) {
            /* Re-use a free entry */
            entry = available_entry;
        } അല്ലെങ്കിൽ {
            /* Append a new entry */
            സ്ഥിരം darray_size_t idx = darray_size(sm->overlays.താക്കോലുകൾ);
            darray_resize(sm->overlays.താക്കോലുകൾ, idx + 1);
            entry = &darray_item(sm->overlays.താക്കോലുകൾ, idx);
        }

        /* Overlay the key */
        entry->old = താക്കോൽ;
        entry->new = new;
        entry->refcnt = 1;
        തിരികെ new;
    }

    /* No overlay */
    തിരികെ താക്കോൽ;
}

എണ്ണൽ xkb_status
xkb_machine_process_താക്കോൽ(ഘടന xkb_machine *sm,
                        xkb_keycode_t kc, എണ്ണൽ xkb_താക്കോൽ_direction direction,
                        ഘടന xkb_events *events)
{
    darray_size(events->queue) = 0;
    events->next = 0;

    ഘടന xkb_server_state * സ്ഥിരം state = &sm->base;
    സ്ഥിരം ഘടന xkb_താക്കോൽ * താക്കോൽ = Xkb_താക്കോൽ(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    എങ്കിൽ (!താക്കോൽ || (direction == XKB_താക്കോൽ_REPEATED && !താക്കോൽ->repeats))
        തിരികെ XKB_SUCCESS;

    സ്ഥിരം ഘടന state_components initial_components = state->base.components;

    എങ്കിൽ (direction == XKB_താക്കോൽ_DOWN &&
        (sm->base.base.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_താക്കോലുകൾ) &&
        (sm->base.a11y_flags & XKB_A11_Y_STICKY_താക്കോലുകൾ_NO_SIMULTANEOUS_താക്കോലുകൾ) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_താക്കോലുകൾ,
                                      0, events);
    }

    എങ്കിൽ (താക്കോൽ->overlays)
        താക്കോൽ = process_overlayable_താക്കോൽ(sm, താക്കോൽ, direction);

    ssize_t remap_event = do_remap_modifiers(&sm->config.modifiers,
                                             &state->base, events, താക്കോൽ);

    remap_event = do_shortcuts_tweak(&sm->config.shortcuts, &state->base,
                                     &initial_components, events, remap_event);

    എങ്കിൽ (remap_event >= 0) {
        // TODO: what about merging state event for RedirectKey?
        darray_append(events->queue, (ഘടന xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    state->flags |= STATE_REQUIRE_താക്കോൽ_EVENT;
    state->set_mods = 0;
    state->clear_mods = 0;

    /* Handle key actions */
    xkb_filter_apply_all(state, events, താക്കോൽ, direction);

    xkb_mod_index_t i;
    xkb_mod_mask_t bit;
    വേണ്ടി (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        എങ്കിൽ (state->set_mods & bit) {
            state->mod_താക്കോൽ_എണ്ണം[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    വേണ്ടി (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        എങ്കിൽ (state->clear_mods & bit) {
            state->mod_താക്കോൽ_എണ്ണം[i]--;
            എങ്കിൽ (state->mod_താക്കോൽ_എണ്ണം[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_താക്കോൽ_എണ്ണം[i] = 0;
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
    എങ്കിൽ (state->flags & STATE_REQUIRE_താക്കോൽ_EVENT) {
        darray_append(events->queue, (ഘടന xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_താക്കോൽ,
            .താക്കോൽ = {
                .keycode = താക്കോൽ->keycode,
                .direction = direction
            }
        });
    }

    എങ്കിൽ (remap_event >= 0) {
        // FIXME: fragile if last state change does not restore the state to the remap event
        undo_tweaks(&state->base, &initial_components, events);
    }

    സ്ഥിരം എണ്ണൽ xkb_state_component changed = get_state_component_changes(
        &initial_components, &state->base.components
    );
    എങ്കിൽ (changed) {
        എങ്കിൽ (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        // TODO: merge with previous state event, if any

        darray_append(events->queue, (ഘടന xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = state->base.components,
                .changed = changed
            }
        });
    }

    എങ്കിൽ (!darray_empty(events->queue) &&
        darray_item(events->queue, darray_size(events->queue) - 1).type !=
        XKB_EVENT_TYPE_FRAME)
    {
        darray_append(events->queue, (ഘടന xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    തിരികെ XKB_SUCCESS;
}

ഘടന xkb_events *
xkb_events_new(ഘടന xkb_context * restrict context,
               സ്ഥിരം ഘടന xkb_events_config * restrict config,
               എണ്ണൽ xkb_status * restrict status)
{
    /* Handle default configuration */
    സ്ഥിതം സ്ഥിരം ഘടന xkb_events_config default_config = {
        .size = വലുപ്പം(default_config)
    };
    എങ്കിൽ (!config)
        config = &default_config;

    /* Check ABI compatibility */
    സ്ഥിരം എണ്ണൽ xkb_status abi_error = xkb_check_state_abi(config);
    എങ്കിൽ (abi_error) {
        xkb_log_abi_error(context, __func__, abi_error);
        എങ്കിൽ (status)
            *status = abi_error;
        തിരികെ ശൂന്യ;
    }

    /* Sanitize input */
    സ്ഥിരം uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_EVENTS_FLAGS_മൂല്യങ്ങൾ);
    എങ്കിൽ (invalid_flags) {
        log_err_func(context, XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS_,
                     "unrecognized events collection flags: 0x%"PRIx32"\n",
                     invalid_flags);
        എങ്കിൽ (status)
            *status = XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS;
        തിരികെ ശൂന്യ;
    }

    ഘടന xkb_events *events = calloc(1, വലുപ്പം(*events));
    എങ്കിൽ (!events) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events collection\n");
        എങ്കിൽ (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        തിരികെ ശൂന്യ;
    }

    /* Pre-allocate event queue */
    darray_init(events->queue);
    സ്ഥിതം സ്ഥിരം darray_size_t queue_initial_size = 16;
    darray_resize(events->queue, queue_initial_size);
    // TODO: proper safe darray_resize() with return value
    എങ്കിൽ (!darray_ഇനങ്ങൾ(events->queue)) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events queue\n");
        xkb_events_destroy(events);
        എങ്കിൽ (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        തിരികെ ശൂന്യ;
    }
    darray_size(events->queue) = 0;

    events->ctx = xkb_context_ref(context);
    events->next = 0;

    എങ്കിൽ (status)
        *status = XKB_SUCCESS;
    തിരികെ events;
}

ശൂന്യം
xkb_events_destroy(ഘടന xkb_events *events)
{
    എങ്കിൽ (!events)
        തിരികെ;
    darray_free(events->queue);
    xkb_context_unref(events->ctx);
    സ്വതന്ത്രം(events);
}

സ്ഥിരം ഘടന xkb_event *
xkb_events_next(ഘടന xkb_events *events)
{
    എങ്കിൽ (events->next < darray_size(events->queue)) {
        സ്ഥിരം darray_size_t index = events->next++;
        തിരികെ &darray_item(events->queue, index);
    } അല്ലെങ്കിൽ {
        തിരികെ ശൂന്യ;
    }
}

എണ്ണൽ xkb_event_type
xkb_event_get_type(സ്ഥിരം ഘടന xkb_event *event)
{
    തിരികെ event->type;
}

എണ്ണൽ xkb_status
xkb_event_get_keycode(സ്ഥിരം ഘടന xkb_event *event,
                      xkb_keycode_t *keycode,
                      എണ്ണൽ xkb_താക്കോൽ_direction *direction)
{
    എങ്കിൽ (event->type == XKB_EVENT_TYPE_താക്കോൽ) {
        *keycode = event->താക്കോൽ.keycode;
        *direction = event->താക്കോൽ.direction;
        തിരികെ XKB_SUCCESS;
    } അല്ലെങ്കിൽ {
        തിരികെ XKB_ERROR_EVENT_TYPE_MISMATCH;
    }
}

എണ്ണൽ xkb_status
xkb_event_get_components(സ്ഥിരം ഘടന xkb_event * restrict event,
                         ഘടന xkb_event_components * restrict components)
{
    എങ്കിൽ (event->type != XKB_EVENT_TYPE_STATE_COMPONENTS)
        തിരികെ XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    സ്ഥിരം എണ്ണൽ xkb_status status = xkb_check_state_abi(components);
    എങ്കിൽ (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        തിരികെ status;
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

    തിരികെ XKB_SUCCESS;
}

എണ്ണൽ xkb_status
xkb_event_get_pointer_motion(സ്ഥിരം ഘടന xkb_event * restrict event,
                             ഘടന xkb_event_pointer_motion * restrict motion)
{
    എങ്കിൽ (event->type != XKB_EVENT_TYPE_POINTER_MOTION)
        തിരികെ XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    സ്ഥിരം എണ്ണൽ xkb_status status = xkb_check_state_abi(motion);
    എങ്കിൽ (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        തിരികെ status;
    }

    motion->flags = event->pointer_motion.flags;
    motion->x = event->pointer_motion.x;
    motion->y = event->pointer_motion.y;

    തിരികെ XKB_SUCCESS;
}

എണ്ണൽ xkb_status
xkb_event_get_pointer_button(സ്ഥിരം ഘടന xkb_event * restrict event,
                             ഘടന xkb_event_pointer_button * restrict button)
{
    എങ്കിൽ (event->type != XKB_EVENT_TYPE_POINTER_BUTTON)
        തിരികെ XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    സ്ഥിരം എണ്ണൽ xkb_status status = xkb_check_state_abi(button);
    എങ്കിൽ (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        തിരികെ status;
    }

    button->button = event->pointer_button.button;
    button->state = event->pointer_button.state;
    button->എണ്ണം = event->pointer_button.എണ്ണം;

    തിരികെ XKB_SUCCESS;
}

എണ്ണൽ xkb_status
xkb_event_get_virtual_console(സ്ഥിരം ഘടന xkb_event * restrict event,
                              int8_t * restrict index_or_offset,
                              bool * restrict is_offset)
{
    എങ്കിൽ (event->type != XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
        തിരികെ XKB_ERROR_EVENT_TYPE_MISMATCH;

    *index_or_offset = event->virtual_console.index_or_offset;
    *is_offset = event->virtual_console.is_offset;

    തിരികെ XKB_SUCCESS;
}

എണ്ണൽ xkb_status
xkb_state_update_event(ഘടന xkb_state * restrict base_state,
                       സ്ഥിരം ഘടന xkb_event * restrict event,
                       എണ്ണൽ xkb_state_component * restrict changed)
{
    /* Guard against server-only state */
    പ്രതിജ്ഞാ(base_state->mode == SERVER_COMPANION ||
           base_state->mode == LEGACY_MIXED_STATE);
    എങ്കിൽ (base_state->mode != SERVER_COMPANION &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE_,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        തിരികെ XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    ഘടന xkb_client_state * സ്ഥിരം state =
        (ഘടന xkb_client_state *)base_state;

    എങ്കിൽ (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
        സ്ഥിരം ഘടന state_components prev_components = state->base.components;
        state->base.components = event->components.components;
        /*
         * Recompute the changes instead of using the event value, because we do
         * not know if the event’s queue and the state are synced.
         */
        എങ്കിൽ (changed)
            *changed = get_state_component_changes(&prev_components,
                                                   &state->base.components);
    } അല്ലെങ്കിൽ {
        എങ്കിൽ (changed)
            *changed = 0;
    }
    തിരികെ XKB_SUCCESS;
}

സ്ഥിതം ശൂന്യം
append_pointer_button(ഘടന xkb_events * restrict events,
                      ഘടന xkb_filter * restrict filter,
                      എണ്ണൽ xkb_pointer_button_state button_state,
                      uint8_t എണ്ണം)
{
    darray_append(events->queue, (ഘടന xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_BUTTON,
        .pointer_button = {
            .size = വലുപ്പം(((ഘടന xkb_event*)0)->pointer_button),
            .button = filter->priv,
            .state = button_state,
            .എണ്ണം = എണ്ണം,
        }
    });
}

സ്ഥിതം ശൂന്യം
xkb_filter_pointer_button_new(ഘടന xkb_server_state *state,
                              ഘടന xkb_events *events,
                              ഘടന xkb_filter *filter)
{
    എങ്കിൽ (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_താക്കോലുകൾ)) {
        filter->func = ശൂന്യ;
        തിരികെ;
    }

    ഘടന xkb_machine * സ്ഥിരം sm = (ഘടന xkb_machine *)state;

    /* Resolve button */
    സ്ഥിരം xkb_pointer_button_index_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;
    filter->priv = button;

    സ്ഥിരം എണ്ണൽ xkb_pointer_button_state button_state =
        XKB_POINTER_BUTTON_PRESSED;
    append_pointer_button(events, filter, button_state,
                          filter->action.btn.എണ്ണം);
    state->flags &= ~STATE_REQUIRE_താക്കോൽ_EVENT;
}

സ്ഥിതം bool
xkb_filter_pointer_button_func(ഘടന xkb_server_state *state,
                               ഘടന xkb_events *events,
                               ഘടന xkb_filter *filter,
                               സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                               എണ്ണൽ xkb_താക്കോൽ_direction direction)
{
    എങ്കിൽ (താക്കോൽ != filter->താക്കോൽ || !events)
        തിരികെ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_താക്കോൽ_EVENT;

    തിരഞ്ഞെടുപ്പ് (direction) {
    അവസ്ഥ XKB_താക്കോൽ_DOWN:
        filter->refcnt++;
        /* fallthrough */
    അവസ്ഥ XKB_താക്കോൽ_REPEATED:
        തിരികെ XKB_FILTER_CONSUME;
    സ്ഥിരസ്ഥിതി:
        എങ്കിൽ (--filter->refcnt > 0)
            തിരികെ XKB_FILTER_CONSUME;
    }

    എങ്കിൽ (!filter->action.btn.എണ്ണം)
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);

    filter->func = ശൂന്യ;
    തിരികെ XKB_FILTER_CONSUME;
}

ഘടന xkb_filter_pointer_lock_button_priv {
    uint32_t button:31;
#ifdef _WIN32
    uint32_t locked:1; // MSVC requires matching types to pack bitfields
#അല്ലെങ്കിൽ
    bool locked:1;
#endif
};

എണ്ണൽ {
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH =
        വലുപ്പം(ഘടന xkb_filter_pointer_lock_button_priv) * CHAR_BIT,
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX =
        (UINT64_C(1) << XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH) - 1
};

static_assert(വലുപ്പം(ഘടന xkb_filter_pointer_lock_button_priv) ==
              വലുപ്പം(((ഘടന xkb_filter*)0)->priv),
              "");
static_assert((uint32_t)XKB_POINTER_BUTTON_MAX < (uint32_t)XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX,
              "xkb_filter_pointer_lock_button_priv cannot store button");

സ്ഥിതം ശൂന്യം
xkb_filter_pointer_lock_button_new(ഘടന xkb_server_state *state,
                                   ഘടന xkb_events *events,
                                   ഘടന xkb_filter *filter)
{
    എങ്കിൽ (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_താക്കോലുകൾ)) {
        filter->func = ശൂന്യ;
        തിരികെ;
    }

    state->flags &= ~STATE_REQUIRE_താക്കോൽ_EVENT;

    ഘടന xkb_machine * സ്ഥിരം sm = (ഘടന xkb_machine *)state;

    /* Resolve button */
    സ്ഥിരം xkb_pointer_button_index_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;

    static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                  "invalid UINTx_C() macro");
    static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                  "invalid left shift");
    സ്ഥിരം xkb_pointer_button_mask_t mask = (UINT8_C(1) << button);

    സ്ഥിരം bool locked = (sm->mouse.locked_buttons & mask);

    എങ്കിൽ (!locked && !(filter->action.btn.flags & ACTION_LOCK_NO_LOCK)) {
        sm->mouse.locked_buttons |= mask;
        filter->priv = button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_PRESSED, 0);
    }

    ഘടന xkb_filter_pointer_lock_button_priv *priv =
        (ഘടന xkb_filter_pointer_lock_button_priv*)&filter->priv;
    priv->button = button,
    priv->locked = locked;
}

സ്ഥിതം bool
xkb_filter_pointer_lock_button_func(ഘടന xkb_server_state *state,
                                    ഘടന xkb_events *events,
                                    ഘടന xkb_filter *filter,
                                    സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                                    എണ്ണൽ xkb_താക്കോൽ_direction direction)
{
    എങ്കിൽ (താക്കോൽ != filter->താക്കോൽ || !events || state->base.mode != SERVER_STATE)
        തിരികെ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_താക്കോൽ_EVENT;

    തിരഞ്ഞെടുപ്പ് (direction) {
    അവസ്ഥ XKB_താക്കോൽ_DOWN:
        filter->refcnt++;
        /* fallthrough */
    അവസ്ഥ XKB_താക്കോൽ_REPEATED:
        തിരികെ XKB_FILTER_CONSUME;
    സ്ഥിരസ്ഥിതി:
        എങ്കിൽ (--filter->refcnt > 0)
            തിരികെ XKB_FILTER_CONSUME;
    }

    സ്ഥിരം ഘടന xkb_filter_pointer_lock_button_priv *priv =
        (ഘടന xkb_filter_pointer_lock_button_priv*)&filter->priv;

    എങ്കിൽ (priv->locked && !(filter->action.btn.flags & ACTION_LOCK_NO_UNLOCK)) {
        ഘടന xkb_machine * സ്ഥിരം sm = (ഘടന xkb_machine *)state;

        static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                    "invalid UINTx_C() macro");
        static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                    "invalid left shift");
        സ്ഥിരം xkb_pointer_button_mask_t mask = (UINT8_C(1) << priv->button);

        sm->mouse.locked_buttons &= ~mask;
        filter->priv = priv->button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);
    }

    filter->func = ശൂന്യ;
    തിരികെ XKB_FILTER_CONSUME;
}

സ്ഥിതം ശൂന്യം
xkb_filter_pointer_set_default_button_new(ഘടന xkb_server_state *state,
                                          ഘടന xkb_events *events,
                                          ഘടന xkb_filter *filter)
{
    എങ്കിൽ (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_താക്കോലുകൾ)) {
        filter->func = ശൂന്യ;
        തിരികെ;
    }

    state->flags &= ~STATE_REQUIRE_താക്കോൽ_EVENT;

    ഘടന xkb_machine * സ്ഥിരം sm = (ഘടന xkb_machine *)state;

    static_assert(2 * XKB_POINTER_BUTTON_MAX <= INT_MAX, "lossy conversion");
    പൂർണ്ണസംഖ്യ button = (filter->action.dflt.flags & ACTION_ABSOLUTE_SWITCH)
        ? filter->action.dflt.മൂല്യം
        : sm->mouse.default_button + filter->action.dflt.മൂല്യം;

    /* Wrap into range */
    സ്ഥിരം പൂർണ്ണസംഖ്യ rem = button % XKB_POINTER_BUTTON_MAX;
    sm->mouse.default_button = (rem >= 0)
        ? (xkb_pointer_button_index_t)rem
        : (xkb_pointer_button_index_t)(rem + XKB_POINTER_BUTTON_MAX);
}

സ്ഥിതം bool
xkb_filter_pointer_set_default_button_func(ഘടന xkb_server_state *state,
                                           ഘടന xkb_events *events,
                                           ഘടന xkb_filter *filter,
                                           സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                                           എണ്ണൽ xkb_താക്കോൽ_direction direction)
{
    എങ്കിൽ (താക്കോൽ != filter->താക്കോൽ || !events)
        തിരികെ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_താക്കോൽ_EVENT;

    തിരഞ്ഞെടുപ്പ് (direction) {
    അവസ്ഥ XKB_താക്കോൽ_DOWN:
        filter->refcnt++;
        /* fallthrough */
    അവസ്ഥ XKB_താക്കോൽ_REPEATED:
        തിരികെ XKB_FILTER_CONSUME;
    സ്ഥിരസ്ഥിതി:
        എങ്കിൽ (--filter->refcnt > 0)
            തിരികെ XKB_FILTER_CONSUME;
    }

    filter->func = ശൂന്യ;
    തിരികെ XKB_FILTER_CONTINUE;
}

സ്ഥിതം ശൂന്യം
xkb_filter_terminate_server_new(ഘടന xkb_server_state *state,
                                 ഘടന xkb_events *events,
                                 ഘടന xkb_filter *filter)
{
    എങ്കിൽ (!events || state->base.mode != SERVER_STATE ||
        !(((ഘടന xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = ശൂന്യ;
        തിരികെ;
    }

    state->flags &= ~STATE_REQUIRE_താക്കോൽ_EVENT;

    darray_append(events->queue, (ഘടന xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    });
}

സ്ഥിതം bool
xkb_filter_terminate_server_func(ഘടന xkb_server_state *state,
                                 ഘടന xkb_events *events,
                                 ഘടന xkb_filter *filter,
                                 സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                                 എണ്ണൽ xkb_താക്കോൽ_direction direction)
{
    എങ്കിൽ (താക്കോൽ != filter->താക്കോൽ || !events)
        തിരികെ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_താക്കോൽ_EVENT;

    തിരഞ്ഞെടുപ്പ് (direction) {
    അവസ്ഥ XKB_താക്കോൽ_DOWN:
        filter->refcnt++;
        /* fallthrough */
    അവസ്ഥ XKB_താക്കോൽ_REPEATED:
        തിരികെ XKB_FILTER_CONSUME;
    സ്ഥിരസ്ഥിതി:
        എങ്കിൽ (--filter->refcnt > 0)
            തിരികെ XKB_FILTER_CONSUME;
    }

    filter->func = ശൂന്യ;
    തിരികെ XKB_FILTER_CONSUME;
}

സ്ഥിതം ശൂന്യം
xkb_filter_switch_virtual_console_new(ഘടന xkb_server_state *state,
                                      ഘടന xkb_events *events,
                                      ഘടന xkb_filter *filter)
{
    എങ്കിൽ (!events || state->base.mode != SERVER_STATE ||
        !(((ഘടന xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = ശൂന്യ;
        തിരികെ;
    }

    state->flags &= ~STATE_REQUIRE_താക്കോൽ_EVENT;

    darray_append(events->queue, (ഘടന xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
        .virtual_console = {
            .index_or_offset = filter->action.screen.screen,
            .is_offset = !(filter->action.screen.flags & ACTION_ABSOLUTE_SWITCH)
        }
    });
}

സ്ഥിതം bool
xkb_filter_switch_virtual_console_func(ഘടന xkb_server_state *state,
                                       ഘടന xkb_events *events,
                                       ഘടന xkb_filter *filter,
                                       സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ,
                                       എണ്ണൽ xkb_താക്കോൽ_direction direction)
{
    എങ്കിൽ (താക്കോൽ != filter->താക്കോൽ || !events)
        തിരികെ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_താക്കോൽ_EVENT;

    തിരഞ്ഞെടുപ്പ് (direction) {
    അവസ്ഥ XKB_താക്കോൽ_DOWN:
        filter->refcnt++;
        /* fallthrough */
    അവസ്ഥ XKB_താക്കോൽ_REPEATED:
        തിരികെ XKB_FILTER_CONSUME;
    സ്ഥിരസ്ഥിതി:
        എങ്കിൽ (--filter->refcnt > 0)
            തിരികെ XKB_FILTER_CONSUME;
    }

    filter->func = ശൂന്യ;
    തിരികെ XKB_FILTER_CONSUME;
}
