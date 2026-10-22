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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon-status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon-features.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abi-check.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "features/enums.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keysym.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "messages-codes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "state-priv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utf8.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-mem.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-numbers.h"

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
ਬਣਤਰ xkb_events {
    ਬਣਤਰ xkb_context *ctx;
    darray(ਬਣਤਰ xkb_event) ਕਤਾਰ;
    /**
     * Read cursor for `xkb_events_next()`.
     *
     * Reset to 0 on each `process_*` call.
     */
    darray_ਮਾਪ_t ਅਗਲਾ;
};

ਬਣਤਰ xkb_server_state;

ਬਣਤਰ xkb_filter {
    ਸੰਘ xkb_action action;
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ;
    bool (*func)(ਬਣਤਰ xkb_server_state *state,
                 ਬਣਤਰ xkb_events *events,
                 ਬਣਤਰ xkb_filter *filter,
                 ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                 ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction);
    uint32_t priv;
    ਪੂਰਨ refcnt;
};

ਗਿਣਤੀ xkb_state_ਬਹੁਲਕ_ਅੰਦਰੂਨੀ {
    /** `xkb_state`: Client-only state (server API raises errors) */
    CLIENT_STATE = XKB_STATE_ਬਹੁਲਕ_CLIENT,
    /** `xkb_state`: Server query companion */
    SERVER_COMPANION = XKB_STATE_ਬਹੁਲਕ_SERVER_QUERY,
    /** `xkb_state`: Legacy unsafe client + server state */
    LEGACY_MIXED_STATE = 2,
    /** `xkb_state`: Legacy server-only state (client-only API raises errors) */
    LEGACY_SERVER_STATE = 3,
    /** `xkb_machine`: Server-only state */
    SERVER_STATE = 4,
    XKB_STATE_TYPE_ਅੰਕ_ENTRIES = 5
};

static_assert(CLIENT_STATE == 0 &&
              CLIENT_STATE == SERVER_COMPANION - 1 &&
              SERVER_COMPANION == LEGACY_MIXED_STATE - 1 &&
              LEGACY_MIXED_STATE == LEGACY_SERVER_STATE - 1 &&
              LEGACY_SERVER_STATE == SERVER_STATE - 1 &&
              SERVER_STATE == XKB_STATE_TYPE_ਅੰਕ_ENTRIES - 1 &&
              XKB_STATE_TYPE_ਅੰਕ_ENTRIES == 5,
              "Order is used to guard client/server -only APIs: do not change");

ਗਿਣਤੀ { XKB_LAYOUT_ਸੂਚਕਾਂਕ_T_MIN_ਚੌੜਾਈ = 6 };
static_assert(XKB_MAX_GROUPS < (1 << XKB_LAYOUT_ਸੂਚਕਾਂਕ_T_MIN_ਚੌੜਾਈ),
              "Cannot encode out_of_range_group::redirect_group");

ਬਣਤਰ ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ {
    ਗਿਣਤੀ xkb_layout_ਬਾਹਰਲਾ_ਦਾ_range_policy policy :
        (ਆਕਾਰ(ਗਿਣਤੀ xkb_layout_ਬਾਹਰਲਾ_ਦਾ_range_policy) * CHAR_BIT -
         XKB_LAYOUT_ਸੂਚਕਾਂਕ_T_MIN_ਚੌੜਾਈ);
    xkb_layout_ਸੂਚਕਾਂਕ_t redirect_ਸਮੂਹ : XKB_LAYOUT_ਸੂਚਕਾਂਕ_T_MIN_ਚੌੜਾਈ;
};

ਗਿਣਤੀ { XKB_STATE_ਬਹੁਲਕ_ਅੰਦਰੂਨੀ_MIN_ਚੌੜਾਈ = 4 /* 3 bits + sign */ };
static_assert(XKB_STATE_TYPE_ਅੰਕ_ENTRIES <
              (1 << XKB_STATE_ਬਹੁਲਕ_ਅੰਦਰੂਨੀ_MIN_ਚੌੜਾਈ),
              "Cannot encode xkb_state::mode");

/**
 * Common state
 *
 * `mode` indicates whether the object is casted from a `xkb_server_state` or a
 * `xkb_client_state` struct.
 */
ਬਣਤਰ xkb_state {
    ਬਣਤਰ xkb_keymap *keymap;
    /*
     * Before updating the state, we keep a copy of just this struct. This
     * allows us to report which components of the state have changed.
     */
    ਬਣਤਰ state_components components;
    ਬਣਤਰ ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ;

    ਗਿਣਤੀ xkb_state_ਬਹੁਲਕ_ਅੰਦਰੂਨੀ ਬਹੁਲਕ : XKB_STATE_ਬਹੁਲਕ_ਅੰਦਰੂਨੀ_MIN_ਚੌੜਾਈ;
    ਪੂਰਨ refcnt : (ਆਕਾਰ(ਪੂਰਨ) * CHAR_BIT - XKB_STATE_ਬਹੁਲਕ_ਅੰਦਰੂਨੀ_MIN_ਚੌੜਾਈ);
};

/**
 * Minimal client state
 *
 * Wrapper for strong typing
 */
ਬਣਤਰ xkb_client_state {
    /** Inherits from `xkb_state` */
    ਬਣਤਰ xkb_state ਅਧਾਰ;
};

ਗਿਣਤੀ state_flags {
    STATE_NO_FLAGS = 0,
    STATE_REQUIRE_ਕੁੰਜੀ_EVENT = (1 << 0),
    STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_ਕੁੰਜੀ = (1 << 1),
    STATE_UNLOCK_MODS_ON_PRESS = (1 << 2),
};

/**
 * Legacy server state, used for `LEGACY_MIXED_STATE` and `LEGACY_SERVER_STATE`
 * modes.
 *
 * Casted as `xkb_state` in the public API. See `xkb_state::mode`.
 */
ਬਣਤਰ xkb_server_state {
    /** Inherits from `xkb_state` */
    ਬਣਤਰ xkb_state ਅਧਾਰ;

    /* Server-specific state data */

    darray(ਬਣਤਰ xkb_filter) filters;

    /**
     * Mix of constant configuration flags and
     * variable state-processing flags.
     */
    ਗਿਣਤੀ state_flags flags;

    ਗਿਣਤੀ xkb_a11y_flags a11y_flags;

    /*
     * At each event, we accumulate all the needed modifications to the base
     * modifiers, and apply them at the end. These keep track of this state.
     */
    xkb_mod_ਨਕਾਬ_t ਸੈੱਟ_mods;
    xkb_mod_ਨਕਾਬ_t clear_mods;

    /*
     * We mustn't clear a base modifier if there's another depressed key
     * which affects it, e.g. given this sequence
     * < Left Shift down, Right Shift down, Left Shift Up >
     * the modifier should still be set. This keeps the count.
     */
    int16_t mod_ਕੁੰਜੀ_ਗਿਣਤੀ[XKB_MAX_MODS];
};

ਸਥਿਰ ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ *
get_ਇੰਦਰਾਜ_for_mods(ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type *type, xkb_mod_ਨਕਾਬ_t mods)
{
    ਲਈ (darray_ਮਾਪ_t i = 0; i < type->ਅੰਕ_entries; i++)
        ਜੇ (ਇੰਦਰਾਜ_is_ਸਰਗਰਮ(&type->entries[i]) &&
            type->entries[i].mods.ਨਕਾਬ == mods)
            ਵਾਪਸ &type->entries[i];
    ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ *
get_ਇੰਦਰਾਜ_for_ਕੁੰਜੀ_state(ਬਣਤਰ xkb_state *state, ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                        xkb_layout_ਸੂਚਕਾਂਕ_t ਸਮੂਹ)
{
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type* ਅਚਲ type = ਕੁੰਜੀ->groups[ਸਮੂਹ].type;
    xkb_mod_ਨਕਾਬ_t ਸਰਗਰਮ_mods = state->components.mods & type->mods.ਨਕਾਬ;
    ਵਾਪਸ get_ਇੰਦਰਾਜ_for_mods(type, ਸਰਗਰਮ_mods);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ xkb_level_ਸੂਚਕਾਂਕ_t
state_ਕੁੰਜੀ_get_level(ਬਣਤਰ xkb_state *state, ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                         xkb_layout_ਸੂਚਕਾਂਕ_t layout)
{
    ਜੇ (layout >= ਕੁੰਜੀ->ਅੰਕ_groups)
        ਵਾਪਸ XKB_LEVEL_INVALID;

    /* If we don't find an explicit match the default is 0. */
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ* ਅਚਲ ਇੰਦਰਾਜ =
        get_ਇੰਦਰਾਜ_for_ਕੁੰਜੀ_state(state, ਕੁੰਜੀ, layout);

    ਵਾਪਸ (ਇੰਦਰਾਜ) ? ਇੰਦਰਾਜ->level : 0;
}

/**
 * Returns the level to use for the given key and state, or
 * XKB_LEVEL_INVALID.
 */
xkb_level_ਸੂਚਕਾਂਕ_t
xkb_state_ਕੁੰਜੀ_get_level(ਬਣਤਰ xkb_state *state, xkb_keycode_t kc,
                        xkb_layout_ਸੂਚਕਾਂਕ_t layout)
{
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ* ਅਚਲ ਕੁੰਜੀ = Xkb_ਕੁੰਜੀ(state->keymap, kc);

    ਵਾਪਸ (ਕੁੰਜੀ)
        ? state_ਕੁੰਜੀ_get_level(state, ਕੁੰਜੀ, layout)
        : XKB_LEVEL_INVALID;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ xkb_layout_ਸੂਚਕਾਂਕ_t
state_ਕੁੰਜੀ_get_layout(ਬਣਤਰ xkb_state *state, ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ)
{
    ਵਾਪਸ xkb_keymap_ਕੁੰਜੀ_effective_layout(ਕੁੰਜੀ, state->components.ਸਮੂਹ);
}

/**
 * Returns the layout to use for the given key and state, taking
 * wrapping/clamping/etc into account, or XKB_LAYOUT_INVALID.
 */
xkb_layout_ਸੂਚਕਾਂਕ_t
xkb_state_ਕੁੰਜੀ_get_layout(ਬਣਤਰ xkb_state *state, xkb_keycode_t kc)
{
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ* ਅਚਲ ਕੁੰਜੀ = Xkb_ਕੁੰਜੀ(state->keymap, kc);

    ਜੇ (!ਕੁੰਜੀ)
        ਵਾਪਸ XKB_LAYOUT_INVALID;

    ਵਾਪਸ state_ਕੁੰਜੀ_get_layout(state, ਕੁੰਜੀ);
}

/* Empty action used for empty levels */
ਸਥਿਰ ਅਚਲ ਸੰਘ xkb_action dummy_action = { .type = ACTION_TYPE_NONE };

ਸਥਿਰ xkb_action_ਗਿਣਤੀ_t
xkb_ਕੁੰਜੀ_get_actions(ਬਣਤਰ xkb_state *state, ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                    ਅਚਲ ਸੰਘ xkb_action **actions)
{
    ਅਚਲ xkb_layout_ਸੂਚਕਾਂਕ_t layout = state_ਕੁੰਜੀ_get_layout(state, ਕੁੰਜੀ);
    ਅਚਲ xkb_level_ਸੂਚਕਾਂਕ_t level = state_ਕੁੰਜੀ_get_level(state, ਕੁੰਜੀ, layout);
    ਜੇ (level == XKB_LEVEL_INVALID)
        ਜਾਓ err;

    ਅਚਲ xkb_action_ਗਿਣਤੀ_t ਗਿਣਤੀ_2 =
        xkb_keymap_ਕੁੰਜੀ_get_actions_by_level(state->keymap, ਕੁੰਜੀ,
                                            layout, level, actions);
    ਜੇ (!ਗਿਣਤੀ_2)
        ਜਾਓ err;

    ਵਾਪਸ ਗਿਣਤੀ_2;

err:
    /* Use a dummy action if no corresponding level was found or if it is empty.
     * This is required e.g. to handle latches properly. */
    *actions = &dummy_action;
    ਵਾਪਸ 1;
}

ਸਥਿਰ ਬਣਤਰ xkb_filter *
xkb_filter_new(ਬਣਤਰ xkb_server_state *state)
{
    ਬਣਤਰ xkb_filter *filter = ਨਿਰਾ, *iter;

    darray_foreach(iter, state->filters) {
        ਜੇ (iter->func)
            ਜਾਰੀ;
        /* Use available slot */
        filter = iter;
        ਰੋਕੋ;
    }

    ਜੇ (!filter) {
        /* No available slot: resize the filters array */
        darray_resize0(state->filters, darray_ਮਾਪ(state->filters) + 1);
        filter = &darray_ਵਸਤੂ(state->filters, darray_ਮਾਪ(state->filters) -1);
    }

    filter->refcnt = 1;
    ਵਾਪਸ filter;
}

/***====================================================================***/

ਗਿਣਤੀ xkb_filter_ਨਤੀਜਾ {
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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ apply_ਸਮੂਹ_delta(filter_, state_, component_)                   \
    ਜੇ ((filter_)->action.ਸਮੂਹ.flags & ACTION_ABSOLUTE_SWITCH)          \
        (state_)->components.component_ = (filter_)->action.ਸਮੂਹ.ਸਮੂਹ; \
    ਨਹੀਂਤਾਂ                                                                 \
        (state_)->components.component_ += (filter_)->action.ਸਮੂਹ.ਸਮੂਹ

ਸਥਿਰ ਖਾਲੀ
xkb_filter_ਸਮੂਹ_ਸੈੱਟ_new(ਬਣਤਰ xkb_server_state *state,
                         ਬਣਤਰ xkb_events *events,
                         ਬਣਤਰ xkb_filter *filter)
{
    static_assert(ਆਕਾਰ(state->ਅਧਾਰ.components.ਅਧਾਰ_ਸਮੂਹ) ==
                  ਆਕਾਰ(filter->priv),
                  "Max groups don't fit");
    filter->priv = (uint32_t)state->ਅਧਾਰ.components.ਅਧਾਰ_ਸਮੂਹ;
    apply_ਸਮੂਹ_delta(filter, &state->ਅਧਾਰ, ਅਧਾਰ_ਸਮੂਹ);
}

ਸਥਿਰ bool
xkb_filter_ਸਮੂਹ_ਸੈੱਟ_func(ਬਣਤਰ xkb_server_state *state,
                          ਬਣਤਰ xkb_events *events,
                          ਬਣਤਰ xkb_filter *filter,
                          ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                          ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction)
{
    ਜੇ (ਕੁੰਜੀ != filter->ਕੁੰਜੀ) {
        filter->action.ਸਮੂਹ.flags &= ~ACTION_ਤਾਲਾ_CLEAR;
        ਵਾਪਸ XKB_FILTER_CONTINUE;
    }

    ਚੋਣ (direction) {
    ਹਾਲਤ XKB_ਕੁੰਜੀ_ਥੱਲੇ:
        filter->refcnt++;
        /* fallthrough */
    ਹਾਲਤ XKB_ਕੁੰਜੀ_REPEATED:
        ਵਾਪਸ XKB_FILTER_CONSUME;
    ਮੂਲ:
        ਜੇ (--filter->refcnt > 0)
            ਵਾਪਸ XKB_FILTER_CONSUME;
    }

    static_assert(ਆਕਾਰ(state->ਅਧਾਰ.components.ਅਧਾਰ_ਸਮੂਹ) == ਆਕਾਰ(filter->priv),
                  "Max groups don't fit");
    state->ਅਧਾਰ.components.ਅਧਾਰ_ਸਮੂਹ = (int32_t) filter->priv;

    ਜੇ (filter->action.ਸਮੂਹ.flags & ACTION_ਤਾਲਾ_CLEAR)
        state->ਅਧਾਰ.components.locked_ਸਮੂਹ = 0;

    filter->func = ਨਿਰਾ;
    ਵਾਪਸ XKB_FILTER_CONTINUE;
}

ਸਥਿਰ ਗਿਣਤੀ xkb_state_component
get_state_component_changes(ਅਚਲ ਬਣਤਰ state_components *a,
                            ਅਚਲ ਬਣਤਰ state_components *b)
{
    ਗਿਣਤੀ xkb_state_component ਨਕਾਬ = 0;

    ਜੇ (a->ਸਮੂਹ != b->ਸਮੂਹ)
        ਨਕਾਬ |= XKB_STATE_LAYOUT_EFFECTIVE;
    ਜੇ (a->ਅਧਾਰ_ਸਮੂਹ != b->ਅਧਾਰ_ਸਮੂਹ)
        ਨਕਾਬ |= XKB_STATE_LAYOUT_DEPRESSED;
    ਜੇ (a->latched_ਸਮੂਹ != b->latched_ਸਮੂਹ)
        ਨਕਾਬ |= XKB_STATE_LAYOUT_LATCHED;
    ਜੇ (a->locked_ਸਮੂਹ != b->locked_ਸਮੂਹ)
        ਨਕਾਬ |= XKB_STATE_LAYOUT_LOCKED;
    ਜੇ (a->mods != b->mods)
        ਨਕਾਬ |= XKB_STATE_MODS_EFFECTIVE;
    ਜੇ (a->ਅਧਾਰ_mods != b->ਅਧਾਰ_mods)
        ਨਕਾਬ |= XKB_STATE_MODS_DEPRESSED;
    ਜੇ (a->latched_mods != b->latched_mods)
        ਨਕਾਬ |= XKB_STATE_MODS_LATCHED;
    ਜੇ (a->locked_mods != b->locked_mods)
        ਨਕਾਬ |= XKB_STATE_MODS_LOCKED;
    ਜੇ (a->leds != b->leds)
        ਨਕਾਬ |= XKB_STATE_LEDS;
    ਜੇ (a->controls != b->controls)
        ਨਕਾਬ |= XKB_STATE_CONTROLS_EFFECTIVE;

    ਵਾਪਸ ਨਕਾਬ;
}

ਸਥਿਰ ਖਾਲੀ
xkb_filter_ਸਮੂਹ_ਤਾਲਾ_new(ਬਣਤਰ xkb_server_state *state,
                          ਬਣਤਰ xkb_events *events,
                          ਬਣਤਰ xkb_filter *filter)
{
    ਜੇ (filter->action.ਸਮੂਹ.flags & ACTION_ਤਾਲਾ_ON_RELEASE) {
        /*
         * Lock on key release: do nothing on key press.
         *
         * This is a keymap format v2 extension.
         */
        ਵਾਪਸ;
    } ਨਹੀਂਤਾਂ {
        /* Lock on key press */
        apply_ਸਮੂਹ_delta(filter, &state->ਅਧਾਰ, locked_ਸਮੂਹ);
    }
}

ਸਥਿਰ bool
xkb_filter_ਸਮੂਹ_ਤਾਲਾ_func(ਬਣਤਰ xkb_server_state *state,
                           ਬਣਤਰ xkb_events *events,
                           ਬਣਤਰ xkb_filter *filter,
                           ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                           ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction)
{
    ਜੇ (ਕੁੰਜੀ != filter->ਕੁੰਜੀ) {
        ਜੇ ((filter->action.ਸਮੂਹ.flags & ACTION_ਤਾਲਾ_ON_RELEASE) &&
            direction == XKB_ਕੁੰਜੀ_ਥੱਲੇ) {
            /*
             * Another key has been pressed after the locking key:
             * cancel group lock on release.
             *
             * This is a keymap v2 extension.
             */
            filter->action.ਸਮੂਹ.flags &= ~ACTION_ਤਾਲਾ_ON_RELEASE;
        }
        ਵਾਪਸ XKB_FILTER_CONTINUE;
    }

    ਚੋਣ (direction) {
    ਹਾਲਤ XKB_ਕੁੰਜੀ_ਥੱਲੇ:
        filter->refcnt++;
        /* fallthrough */
    ਹਾਲਤ XKB_ਕੁੰਜੀ_REPEATED:
        ਵਾਪਸ XKB_FILTER_CONSUME;
    ਮੂਲ:
        ਜੇ (--filter->refcnt > 0)
            ਵਾਪਸ XKB_FILTER_CONSUME;
    }

    ਜੇ (filter->action.ਸਮੂਹ.flags & ACTION_ਤਾਲਾ_ON_RELEASE) {
        /*
         * Lock on key release
         *
         * This is a keymap v2 extension.
         */
        apply_ਸਮੂਹ_delta(filter, &state->ਅਧਾਰ, locked_ਸਮੂਹ);
    } ਨਹੀਂਤਾਂ {
        /* Lock on key press: do nothing on key release. */
    }
    filter->func = ਨਿਰਾ;
    ਵਾਪਸ XKB_FILTER_CONTINUE;
}

ਸਥਿਰ bool
xkb_action_breaks_latch(ਅਚਲ ਸੰਘ xkb_action *action,
                        ਗਿਣਤੀ xkb_action_controls controls,
                        ਗਿਣਤੀ xkb_ਅੰਦਰੂਨੀ_action_flags flag,
                        xkb_mod_ਨਕਾਬ_t ਨਕਾਬ)
{
    ਚੋਣ (action->type) {
    ਹਾਲਤ ACTION_TYPE_NONE:
    ਹਾਲਤ ACTION_TYPE_VOID:
    ਹਾਲਤ ACTION_TYPE_PTR_BUTTON:
    ਹਾਲਤ ACTION_TYPE_PTR_ਤਾਲਾ:
    ਹਾਲਤ ACTION_TYPE_CTRL_ਸੈੱਟ:
    ਹਾਲਤ ACTION_TYPE_CTRL_ਤਾਲਾ:
    ਹਾਲਤ ACTION_TYPE_SWITCH_VT:
    ਹਾਲਤ ACTION_TYPE_TERMINATE:
    ਹਾਲਤ ACTION_TYPE_REDIRECT_ਕੁੰਜੀ:
        ਵਾਪਸ true;
    ਹਾਲਤ ACTION_TYPE_PTR_MOVE:
    ਹਾਲਤ ACTION_TYPE_PTR_DEFAULT:
        ਵਾਪਸ !(controls & CONTROL_MOUSE_ਕੁੰਜੀਆਂ);
    ਹਾਲਤ ACTION_TYPE_ਅੰਦਰੂਨੀ:
        ਵਾਪਸ (action->ਅੰਦਰੂਨੀ.flags & flag) &&
               ((action->ਅੰਦਰੂਨੀ.clear_latched_mods & ਨਕਾਬ) == ਨਕਾਬ);
    ਮੂਲ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_ਅੰਦਰੂਨੀ == 20 &&
                      ACTION_TYPE_ਅੰਦਰੂਨੀ + 1 == ACTION_TYPE_ਅੰਕ_ENTRIES,
                      "Missing action type");
        ਵਾਪਸ false;
    }
}

ਗਿਣਤੀ xkb_ਕੁੰਜੀ_latch_state {
    NO_LATCH = 0,
    LATCH_ਕੁੰਜੀ_ਥੱਲੇ,
    LATCH_PENDING,
    ਕੁੰਜੀ_LATCH_STATE_ਅੰਕ_ENTRIES
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_XKB_ਕੁੰਜੀ_LATCH_STATE_LOG2 2
#ਜੇ (ਕੁੰਜੀ_LATCH_STATE_ਅੰਕ_ENTRIES > (1 << MAX_XKB_ਕੁੰਜੀ_LATCH_STATE_LOG2)) || \
    (-XKB_MAX_GROUPS) < (INT32_MIN >> MAX_XKB_ਕੁੰਜੀ_LATCH_STATE_LOG2) || \
      XKB_MAX_GROUPS > (INT32_MAX >> MAX_XKB_ਕੁੰਜੀ_LATCH_STATE_LOG2)
#ਗਲਤੀ "Cannot represent priv field of the group latch filter"
#ਜੇ_ਅੰਤ

/* Hold the latch state *and* the group delta */
ਸੰਘ ਸਮੂਹ_latch_priv {
    uint32_t priv;
    ਬਣਤਰ {
        /* The type is really: enum xkb_key_latch_state, but it is problematic
         * on Windows, because it is interpreted as signed and leads to wrong
         * negative values. */
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ latch:MAX_XKB_ਕੁੰਜੀ_LATCH_STATE_LOG2;
        int32_t ਸਮੂਹ_delta:(32 - MAX_XKB_ਕੁੰਜੀ_LATCH_STATE_LOG2);
    };
};

ਸਥਿਰ ਖਾਲੀ
xkb_filter_ਸਮੂਹ_latch_new(ਬਣਤਰ xkb_server_state *state,
                           ਬਣਤਰ xkb_events *events,
                           ਬਣਤਰ xkb_filter *filter)
{
    ਅਚਲ ਸੰਘ ਸਮੂਹ_latch_priv priv = {
        .latch = LATCH_ਕੁੰਜੀ_ਥੱਲੇ,
        .ਸਮੂਹ_delta = (filter->action.ਸਮੂਹ.flags & ACTION_ABSOLUTE_SWITCH)
            ? filter->action.ਸਮੂਹ.ਸਮੂਹ - state->ਅਧਾਰ.components.ਅਧਾਰ_ਸਮੂਹ
            : filter->action.ਸਮੂਹ.ਸਮੂਹ
    };
    filter->priv = priv.priv;
    /* Like group set */
    apply_ਸਮੂਹ_delta(filter, &state->ਅਧਾਰ, ਅਧਾਰ_ਸਮੂਹ);
}

ਸਥਿਰ bool
xkb_filter_ਸਮੂਹ_latch_func(ਬਣਤਰ xkb_server_state *state,
                            ਬਣਤਰ xkb_events *events,
                            ਬਣਤਰ xkb_filter *filter,
                            ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                            ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction)
{
    ਸੰਘ ਸਮੂਹ_latch_priv priv = {.priv = filter->priv};
    ਗਿਣਤੀ xkb_ਕੁੰਜੀ_latch_state latch = priv.latch;

    ਜੇ (direction == XKB_ਕੁੰਜੀ_ਥੱਲੇ) {
        ਅਚਲ ਸੰਘ xkb_action *actions = ਨਿਰਾ;
        ਅਚਲ xkb_action_ਗਿਣਤੀ_t ਗਿਣਤੀ_2 = xkb_ਕੁੰਜੀ_get_actions(&state->ਅਧਾਰ, ਕੁੰਜੀ,
                                                             &actions);

        ਜੇ (latch == LATCH_ਕੁੰਜੀ_ਥੱਲੇ) {
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
            ਜੇ (state->a11y_flags & XKB_A11_Y_LATCH_ਇੱਕੋ_ਵੇਲੇ_ਕੁੰਜੀਆਂ) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                ਲਈ (xkb_action_ਗਿਣਤੀ_t k = 0; k < ਗਿਣਤੀ_2; k++) {
                    ਜੇ (xkb_action_breaks_latch(&(actions[k]),
                                                state->ਅਧਾਰ.components.controls,
                                                ਅੰਦਰੂਨੀ_BREAKS_ਸਮੂਹ_LATCH,
                                                0))
                    {
                        latch = NO_LATCH;
                        ਰੋਕੋ;
                    }
                }
            } ਨਹੀਂਤਾਂ {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        ਨਹੀਂਤਾਂ ਜੇ (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is
             * pressed, or promote it to a lock if it's the same group delta &
             * flags and latchToLock option is enabled. */
            ਅਚਲ bool sticky_ਕੁੰਜੀਆਂ = state->ਅਧਾਰ.components.controls
                                   & CONTROL_STICKY_ਕੁੰਜੀਆਂ;
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
            ਲਈ (xkb_action_ਗਿਣਤੀ_t k = 0; k < ਗਿਣਤੀ_2; k++) {
                ਜੇ (((actions[k].type == ACTION_TYPE_ਸਮੂਹ_LATCH) ||
                     (actions[k].type == ACTION_TYPE_ਸਮੂਹ_ਸੈੱਟ && sticky_ਕੁੰਜੀਆਂ)) &&
                     actions[k].ਸਮੂਹ.ਸਮੂਹ == filter->action.ਸਮੂਹ.ਸਮੂਹ &&
                     (actions[k].ਸਮੂਹ.flags & ACTION_ABSOLUTE_SWITCH) ==
                     (filter->action.ਸਮੂਹ.flags & ACTION_ABSOLUTE_SWITCH)) {
                    ਜੇ (filter->action.ਸਮੂਹ.flags & ACTION_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ &&
                        filter->action.ਸਮੂਹ.ਸਮੂਹ != 0)
                    {
                        /* Promote to lock */
                        filter->action.type = ACTION_TYPE_ਸਮੂਹ_ਤਾਲਾ;
                        filter->func = xkb_filter_ਸਮੂਹ_ਤਾਲਾ_func;
                        xkb_filter_ਸਮੂਹ_ਤਾਲਾ_new(state, events, filter);
                        state->ਅਧਾਰ.components.latched_ਸਮੂਹ -= priv.ਸਮੂਹ_delta;
                        filter->ਕੁੰਜੀ = ਕੁੰਜੀ;
                        /* XXX beep beep! */
                        ਵਾਪਸ XKB_FILTER_CONSUME;
                    }
                    /* Do nothing if `latchToLock` option is not activated; if
                     * the latch is not broken by the following actions and the
                     * key is not consumed, then another latch filter will be
                     * created.
                     */
                    ਜਾਰੀ;
                }
                ਨਹੀਂਤਾਂ ਜੇ (xkb_action_breaks_latch(&(actions[k]),
                                                 state->ਅਧਾਰ.components.controls,
                                                 ਅੰਦਰੂਨੀ_BREAKS_ਸਮੂਹ_LATCH,
                                                 0))
                {
                    /* Breaks the latch */
                    state->ਅਧਾਰ.components.latched_ਸਮੂਹ -= priv.ਸਮੂਹ_delta;
                    filter->func = ਨਿਰਾ;
                    ਵਾਪਸ XKB_FILTER_CONTINUE;
                }
            }
        }
        ਨਹੀਂਤਾਂ {
            /* Ignore press in NO_LATCH state */
            ਪ੍ਰਤਿਜ੍ਞਾ(latch == NO_LATCH);
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ (ਕੁੰਜੀ != filter->ਕੁੰਜੀ) {
        /* Ignore repeat/release of other keys */
    }
    ਨਹੀਂਤਾਂ ਜੇ (direction == XKB_ਕੁੰਜੀ_REPEATED) {
        ਵਾਪਸ XKB_FILTER_CONSUME; // TODO: check
    }
    ਨਹੀਂਤਾਂ {
        /* Our key got released.  If we've set it to clear locks, and we
         * currently have a group locked, then release it and
         * don't actually latch.  Else we've actually hit the latching
         * stage, so set PENDING and move our group from base to
         * latched. */
        ਜੇ ((filter->action.ਸਮੂਹ.flags & ACTION_ਤਾਲਾ_CLEAR) &&
             state->ਅਧਾਰ.components.locked_ਸਮੂਹ) {
            ਜੇ (latch == LATCH_PENDING)
                state->ਅਧਾਰ.components.latched_ਸਮੂਹ -= priv.ਸਮੂਹ_delta;
            ਨਹੀਂਤਾਂ
                state->ਅਧਾਰ.components.ਅਧਾਰ_ਸਮੂਹ -= priv.ਸਮੂਹ_delta;
            state->ਅਧਾਰ.components.locked_ਸਮੂਹ = 0;
            filter->func = ਨਿਰਾ;
        }
        /* Broken latch */
        ਨਹੀਂਤਾਂ ਜੇ (latch == NO_LATCH) {
            state->ਅਧਾਰ.components.ਅਧਾਰ_ਸਮੂਹ -= priv.ਸਮੂਹ_delta;
            filter->func = ਨਿਰਾ;
        }
        /* We may already have reached the latch state if pressing the
         * key multiple times without latch-to-lock enabled. */
        ਨਹੀਂਤਾਂ ਜੇ (latch == LATCH_ਕੁੰਜੀ_ਥੱਲੇ) {
            latch = LATCH_PENDING;
            /* Switch from set to latch */
            state->ਅਧਾਰ.components.ਅਧਾਰ_ਸਮੂਹ -= priv.ਸਮੂਹ_delta;
            state->ਅਧਾਰ.components.latched_ਸਮੂਹ += priv.ਸਮੂਹ_delta;
            /* XXX beep beep! */
        }
    }

    priv.latch = latch;
    filter->priv = priv.priv;

    ਵਾਪਸ XKB_FILTER_CONTINUE;
}

ਸਥਿਰ ਖਾਲੀ
xkb_filter_mod_ਸੈੱਟ_new(ਬਣਤਰ xkb_server_state *state,
                       ਬਣਤਰ xkb_events *events,
                       ਬਣਤਰ xkb_filter *filter)
{
    ਅਚਲ ਗਿਣਤੀ xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_ਤਾਲਾ_CLEAR;
    ਜੇ ((filter->action.mods.flags & unlock) == unlock) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension.
         */
        filter->priv = filter->action.mods.mods.ਨਕਾਬ
                     & ~state->ਅਧਾਰ.components.locked_mods;
        state->ਅਧਾਰ.components.locked_mods &= ~filter->action.mods.mods.ਨਕਾਬ;
    } ਨਹੀਂਤਾਂ {
        filter->priv = filter->action.mods.mods.ਨਕਾਬ;
    }

    state->ਸੈੱਟ_mods |= (xkb_mod_ਨਕਾਬ_t) filter->priv;
}

ਸਥਿਰ bool
xkb_filter_mod_ਸੈੱਟ_func(ਬਣਤਰ xkb_server_state *state,
                        ਬਣਤਰ xkb_events *events,
                        ਬਣਤਰ xkb_filter *filter,
                        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                        ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction)
{

    ਜੇ (ਕੁੰਜੀ != filter->ਕੁੰਜੀ) {
        filter->action.mods.flags &= ~ACTION_ਤਾਲਾ_CLEAR;
        ਵਾਪਸ XKB_FILTER_CONTINUE;
    }

    ਚੋਣ (direction) {
    ਹਾਲਤ XKB_ਕੁੰਜੀ_ਥੱਲੇ:
        filter->refcnt++;
        /* fallthrough */
    ਹਾਲਤ XKB_ਕੁੰਜੀ_REPEATED:
        ਵਾਪਸ XKB_FILTER_CONSUME;
    ਮੂਲ:
        ਜੇ (--filter->refcnt > 0)
            ਵਾਪਸ XKB_FILTER_CONSUME;
    }

    state->clear_mods |= (xkb_mod_ਨਕਾਬ_t) filter->priv;
    ਅਚਲ ਗਿਣਤੀ xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_ਤਾਲਾ_CLEAR;
    ਜੇ ((filter->action.mods.flags & unlock) == ACTION_ਤਾਲਾ_CLEAR)
        state->ਅਧਾਰ.components.locked_mods &= ~filter->action.mods.mods.ਨਕਾਬ;

    filter->func = ਨਿਰਾ;
    ਵਾਪਸ XKB_FILTER_CONTINUE;
}

ਸਥਿਰ ਖਾਲੀ
xkb_filter_mod_ਤਾਲਾ_new(ਬਣਤਰ xkb_server_state *state,
                        ਬਣਤਰ xkb_events *events,
                        ਬਣਤਰ xkb_filter *filter)
{
    filter->priv = (state->ਅਧਾਰ.components.locked_mods &
                    filter->action.mods.mods.ਨਕਾਬ);

    ਜੇ (filter->priv && (filter->action.mods.flags & ACTION_UNLOCK_ON_PRESS)) {
        /*
         * Some of the target modifiers were locked before key press: unlock.
         *
         * This is a keymap v2 extension: unlock-on-press.
         */
        ਜੇ (!(filter->action.mods.flags & ACTION_ਤਾਲਾ_NO_UNLOCK))
            state->ਅਧਾਰ.components.locked_mods &= ~filter->priv;
        /* No further action: cancel filter */
        filter->func = ਨਿਰਾ;
    } ਨਹੀਂਤਾਂ {
        /* Set base mods; lock mods if relevant (XKB 1.0 spec) */
        state->ਸੈੱਟ_mods |= filter->action.mods.mods.ਨਕਾਬ;
        ਜੇ (!(filter->action.mods.flags & ACTION_ਤਾਲਾ_NO_ਤਾਲਾ))
            state->ਅਧਾਰ.components.locked_mods |= filter->action.mods.mods.ਨਕਾਬ;
    }
}

ਸਥਿਰ bool
xkb_filter_mod_ਤਾਲਾ_func(ਬਣਤਰ xkb_server_state *state,
                         ਬਣਤਰ xkb_events *events,
                         ਬਣਤਰ xkb_filter *filter,
                         ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                         ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction)
{
    ਜੇ (ਕੁੰਜੀ != filter->ਕੁੰਜੀ)
        ਵਾਪਸ XKB_FILTER_CONTINUE;

    ਚੋਣ (direction) {
    ਹਾਲਤ XKB_ਕੁੰਜੀ_ਥੱਲੇ:
        filter->refcnt++;
        /* fallthrough */
    ਹਾਲਤ XKB_ਕੁੰਜੀ_REPEATED:
        ਵਾਪਸ XKB_FILTER_CONSUME;
    ਮੂਲ:
        ਜੇ (--filter->refcnt > 0)
            ਵਾਪਸ XKB_FILTER_CONSUME;
    }

    state->clear_mods |= filter->action.mods.mods.ਨਕਾਬ;
    ਜੇ (!(filter->action.mods.flags & ACTION_ਤਾਲਾ_NO_UNLOCK))
        state->ਅਧਾਰ.components.locked_mods &= ~filter->priv;

    filter->func = ਨਿਰਾ;
    ਵਾਪਸ XKB_FILTER_CONTINUE;
}

ਸਥਿਰ ਖਾਲੀ
xkb_filter_mod_latch_new(ਬਣਤਰ xkb_server_state *state,
                         ਬਣਤਰ xkb_events *events,
                         ਬਣਤਰ xkb_filter *filter)
{
    /* Latch-on-press + clear-locks imply unlock-on-press */
    ਅਚਲ ਗਿਣਤੀ xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                              | ACTION_LATCH_ON_PRESS;

    ਜੇ ((filter->action.mods.flags & ACTION_ਤਾਲਾ_CLEAR) &&
        (filter->action.mods.flags & unlockOnPress) &&
        (state->ਅਧਾਰ.components.locked_mods & filter->action.mods.mods.ਨਕਾਬ) ==
         filter->action.mods.mods.ਨਕਾਬ) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension: clear locks and do not latch.
         */
        state->ਅਧਾਰ.components.locked_mods &= ~filter->action.mods.mods.ਨਕਾਬ;
        filter->func = ਨਿਰਾ;
    } ਨਹੀਂਤਾਂ ਜੇ (filter->action.mods.flags & ACTION_LATCH_ON_PRESS) {
        /*
         * Latch on key press
         *
         * This is a keymap format v2 extension.
         */
        filter->priv = LATCH_PENDING;
        state->ਅਧਾਰ.components.latched_mods |= filter->action.mods.mods.ਨਕਾਬ;
        /* XXX beep beep! */
    } ਨਹੀਂਤਾਂ {
        /* XKB standard latch action */
        filter->priv = LATCH_ਕੁੰਜੀ_ਥੱਲੇ;
        state->ਸੈੱਟ_mods |= filter->action.mods.mods.ਨਕਾਬ;
    }
}

ਸਥਿਰ bool
xkb_filter_mod_latch_func(ਬਣਤਰ xkb_server_state *state,
                          ਬਣਤਰ xkb_events *events,
                          ਬਣਤਰ xkb_filter *filter,
                          ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                          ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction)
{
    ਗਿਣਤੀ xkb_ਕੁੰਜੀ_latch_state latch = filter->priv;

    ਜੇ (direction == XKB_ਕੁੰਜੀ_ਥੱਲੇ) {
        ਅਚਲ ਸੰਘ xkb_action *actions = ਨਿਰਾ;
        ਅਚਲ xkb_action_ਗਿਣਤੀ_t ਗਿਣਤੀ_2 = xkb_ਕੁੰਜੀ_get_actions(&state->ਅਧਾਰ, ਕੁੰਜੀ,
                                                             &actions);

        ਜੇ (latch == LATCH_ਕੁੰਜੀ_ਥੱਲੇ) {
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
            ਜੇ (state->a11y_flags & XKB_A11_Y_LATCH_ਇੱਕੋ_ਵੇਲੇ_ਕੁੰਜੀਆਂ) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                ਲਈ (xkb_action_ਗਿਣਤੀ_t k = 0; k < ਗਿਣਤੀ_2; k++) {
                    ਜੇ (xkb_action_breaks_latch(&(actions[k]),
                                                state->ਅਧਾਰ.components.controls,
                                                ਅੰਦਰੂਨੀ_BREAKS_MOD_LATCH,
                                                filter->action.mods.mods.ਨਕਾਬ))
                    {
                        latch = NO_LATCH;
                        ਰੋਕੋ;
                    }
                }
            } ਨਹੀਂਤਾਂ {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        ਨਹੀਂਤਾਂ ਜੇ (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is pressed,
             * or promote it to a lock or plain base set if it's the same
             * modifier. */
            ਅਚਲ bool sticky_ਕੁੰਜੀਆਂ = state->ਅਧਾਰ.components.controls
                                   & CONTROL_STICKY_ਕੁੰਜੀਆਂ;
            /*
             * NOTE: Action’s flags are not compared:
             * - flags may have been modified by keyboard controls at the
             *   creation of the filter;
             * - it is unlikely to have two LatchMods() actions on the same
             *   key that differ only by their flags;
             * - even if it were the case, one would most likely want both
             *   actions to be considered compatible in this context.
             */
            ਲਈ (xkb_action_ਗਿਣਤੀ_t k = 0; k < ਗਿਣਤੀ_2; k++) {
                ਜੇ (((actions[k].type == ACTION_TYPE_MOD_LATCH) ||
                     (actions[k].type == ACTION_TYPE_MOD_ਸੈੱਟ && sticky_ਕੁੰਜੀਆਂ)) &&
                    actions[k].mods.mods.ਨਕਾਬ == filter->action.mods.mods.ਨਕਾਬ)
                {
                    ਜੇ (filter->action.mods.flags & ACTION_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ) {
                        /* Mutate the action to LockMods() */
                        filter->action.type = ACTION_TYPE_MOD_ਤਾਲਾ;
                        filter->func = xkb_filter_mod_ਤਾਲਾ_func;
                        xkb_filter_mod_ਤਾਲਾ_new(state, events, filter);
                    }
                    ਨਹੀਂਤਾਂ {
                        /* Mutate the action to SetMods() */
                        filter->action.type = ACTION_TYPE_MOD_ਸੈੱਟ;
                        filter->func = xkb_filter_mod_ਸੈੱਟ_func;
                        xkb_filter_mod_ਸੈੱਟ_new(state, events, filter);
                    }
                    filter->ਕੁੰਜੀ = ਕੁੰਜੀ;
                    /* Clear latches */
                    state->ਅਧਾਰ.components.latched_mods &=
                        ~filter->action.mods.mods.ਨਕਾਬ;
                    /* XXX beep beep! */
                    ਵਾਪਸ XKB_FILTER_CONSUME;
                }
                ਨਹੀਂਤਾਂ ਜੇ (xkb_action_breaks_latch(&(actions[k]),
                                                 state->ਅਧਾਰ.components.controls,
                                                 ਅੰਦਰੂਨੀ_BREAKS_MOD_LATCH,
                                                 filter->action.mods.mods.ਨਕਾਬ))
                {
                    /* XXX: This may be totally broken, we might need to break the
                     *      latch in the next run after this press? */
                    state->ਅਧਾਰ.components.latched_mods &=
                        ~filter->action.mods.mods.ਨਕਾਬ;
                    filter->func = ਨਿਰਾ;
                    ਵਾਪਸ XKB_FILTER_CONTINUE;
                }
            }
        }
        ਨਹੀਂਤਾਂ {
            /* Ignore press in NO_LATCH state */
            ਪ੍ਰਤਿਜ੍ਞਾ(latch == NO_LATCH);
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ (ਕੁੰਜੀ != filter->ਕੁੰਜੀ) {
        /* Ignore repeat/release of other keys */
    }
    ਨਹੀਂਤਾਂ ਜੇ (direction == XKB_ਕੁੰਜੀ_REPEATED) {
        ਵਾਪਸ XKB_FILTER_CONSUME; // TODO: check
    }
    ਨਹੀਂਤਾਂ {
        /* Our key got released.  If we’ve set it to clear locks, and we
         * currently have the same modifiers locked, then release them and
         * don't actually latch.  Else we’ve actually hit the latching
         * stage, so set PENDING and move our modifier from base to
         * latched. */

        /* Latch-on-press + clear-locks imply unlock-on-press */
        ਅਚਲ ਗਿਣਤੀ xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                                  | ACTION_LATCH_ON_PRESS;

        ਜੇ ((filter->action.mods.flags & ACTION_ਤਾਲਾ_CLEAR) &&
            !(filter->action.mods.flags & unlockOnPress) &&
            (state->ਅਧਾਰ.components.locked_mods &
             filter->action.mods.mods.ਨਕਾਬ) == filter->action.mods.mods.ਨਕਾਬ)
        {
            /* XXX: We might be a bit overenthusiastic about clearing
             *      mods other filters have set here? */
            ਜੇ (latch == LATCH_PENDING)
                state->ਅਧਾਰ.components.latched_mods &=
                    ~filter->action.mods.mods.ਨਕਾਬ;
            ਨਹੀਂਤਾਂ
                state->clear_mods |= filter->action.mods.mods.ਨਕਾਬ;
            state->ਅਧਾਰ.components.locked_mods &= ~filter->action.mods.mods.ਨਕਾਬ;
            filter->func = ਨਿਰਾ;
        }
        ਨਹੀਂਤਾਂ ਜੇ (latch == NO_LATCH) {
            /* Broken latch */
            state->clear_mods |= filter->action.mods.mods.ਨਕਾਬ;
            filter->func = ਨਿਰਾ;
        }
        ਨਹੀਂਤਾਂ ਜੇ (!(filter->action.mods.flags & ACTION_LATCH_ON_PRESS)) {
            latch = LATCH_PENDING;
            state->clear_mods |= filter->action.mods.mods.ਨਕਾਬ;
            state->ਅਧਾਰ.components.latched_mods |= filter->action.mods.mods.ਨਕਾਬ;
            /* XXX beep beep! */
        }
    }

    filter->priv = latch;

    ਵਾਪਸ XKB_FILTER_CONTINUE;
}

ਸਥਿਰ ਖਾਲੀ
append_ਸੂਚਕ_move(ਬਣਤਰ xkb_events *events,
                    ਬਣਤਰ xkb_filter *filter)
{
    ਗਿਣਤੀ xkb_ਸੂਚਕ_motion_flags flags = 0;
    ਜੇ (filter->action.ptr.flags & ACTION_ABSOLUTE_X)
        flags |= XKB_ਸੂਚਕ_MOTION_ABSOLUTE_X;
    ਜੇ (filter->action.ptr.flags & ACTION_ABSOLUTE_Y)
        flags |= XKB_ਸੂਚਕ_MOTION_ABSOLUTE_Y;
    ਜੇ (filter->action.ptr.flags & ACTION_REPEAT)
        flags |= XKB_ਸੂਚਕ_MOTION_REPEATS;
    darray_append(events->ਕਤਾਰ, (ਬਣਤਰ xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_ਸੂਚਕ_MOTION,
        .ਸੂਚਕ_motion = {
            .ਮਾਪ = ਆਕਾਰ(((ਬਣਤਰ xkb_event*)0)->ਸੂਚਕ_motion),
            .flags = flags,
            .x = filter->action.ptr.x,
            .y = filter->action.ptr.y,
        }
    });
}

ਸਥਿਰ ਖਾਲੀ
xkb_filter_ਸੂਚਕ_move_new(ਬਣਤਰ xkb_server_state *state,
                            ਬਣਤਰ xkb_events *events,
                            ਬਣਤਰ xkb_filter *filter)
{
    ਜੇ (!events) {
        filter->func = ਨਿਰਾ;
        ਵਾਪਸ;
    }

    ਪ੍ਰਤਿਜ੍ਞਾ(state->ਅਧਾਰ.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ਕੁੰਜੀਆਂ);
    append_ਸੂਚਕ_move(events, filter);
    state->flags &= ~STATE_REQUIRE_ਕੁੰਜੀ_EVENT;
}

ਸਥਿਰ bool
xkb_filter_ਸੂਚਕ_move_func(ਬਣਤਰ xkb_server_state *state,
                             ਬਣਤਰ xkb_events *events,
                             ਬਣਤਰ xkb_filter *filter,
                             ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                             ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction)
{
    ਜੇ (ਕੁੰਜੀ != filter->ਕੁੰਜੀ || !events)
        ਵਾਪਸ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ਕੁੰਜੀ_EVENT;

    ਚੋਣ (direction) {
    ਹਾਲਤ XKB_ਕੁੰਜੀ_ਥੱਲੇ:
        filter->refcnt++;
        append_ਸੂਚਕ_move(events, filter);
        ਵਾਪਸ XKB_FILTER_CONSUME;
    ਹਾਲਤ XKB_ਕੁੰਜੀ_REPEATED:
        ਜੇ (filter->action.ptr.flags & ACTION_REPEAT)
            append_ਸੂਚਕ_move(events, filter);
        ਵਾਪਸ XKB_FILTER_CONSUME;
    ਮੂਲ:
        ਜੇ (--filter->refcnt > 0)
            ਵਾਪਸ XKB_FILTER_CONSUME;
    }

    filter->func = ਨਿਰਾ;
    ਵਾਪਸ XKB_FILTER_CONSUME;
}

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ਸਥਿਰ ਖਾਲੀ
xkb_filter_ਸੂਚਕ_button_new(ਬਣਤਰ xkb_server_state *state,
                              ਬਣਤਰ xkb_events *events,
                              ਬਣਤਰ xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ਸਥਿਰ bool
xkb_filter_ਸੂਚਕ_button_func(ਬਣਤਰ xkb_server_state *state,
                               ਬਣਤਰ xkb_events *events,
                               ਬਣਤਰ xkb_filter *filter,
                               ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                               ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ਸਥਿਰ ਖਾਲੀ
xkb_filter_ਸੂਚਕ_ਤਾਲਾ_button_new(ਬਣਤਰ xkb_server_state *state,
                                   ਬਣਤਰ xkb_events *events,
                                   ਬਣਤਰ xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ਸਥਿਰ bool
xkb_filter_ਸੂਚਕ_ਤਾਲਾ_button_func(ਬਣਤਰ xkb_server_state *state,
                                    ਬਣਤਰ xkb_events *events,
                                    ਬਣਤਰ xkb_filter *filter,
                                    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                                    ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ਸਥਿਰ ਖਾਲੀ
xkb_filter_ਸੂਚਕ_ਸੈੱਟ_default_button_new(ਬਣਤਰ xkb_server_state *state,
                                          ਬਣਤਰ xkb_events *events,
                                          ਬਣਤਰ xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ਸਥਿਰ bool
xkb_filter_ਸੂਚਕ_ਸੈੱਟ_default_button_func(ਬਣਤਰ xkb_server_state *state,
                                           ਬਣਤਰ xkb_events *events,
                                           ਬਣਤਰ xkb_filter *filter,
                                           ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                                           ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction);

ਸਥਿਰ ਖਾਲੀ
xkb_filter_terminate_server_new(ਬਣਤਰ xkb_server_state *state,
                                 ਬਣਤਰ xkb_events *events,
                                 ਬਣਤਰ xkb_filter *filter);

ਸਥਿਰ bool
xkb_filter_terminate_server_func(ਬਣਤਰ xkb_server_state *state,
                                  ਬਣਤਰ xkb_events *events,
                                  ਬਣਤਰ xkb_filter *filter,
                                  ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                                  ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ਸਥਿਰ ਖਾਲੀ
xkb_filter_switch_virtual_console_new(ਬਣਤਰ xkb_server_state *state,
                                      ਬਣਤਰ xkb_events *events,
                                      ਬਣਤਰ xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ਸਥਿਰ bool
xkb_filter_switch_virtual_console_func(ਬਣਤਰ xkb_server_state *state,
                                       ਬਣਤਰ xkb_events *events,
                                       ਬਣਤਰ xkb_filter *filter,
                                       ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                                       ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
clear_all_latches_and_locks(ਬਣਤਰ xkb_server_state *state,
                            ਬਣਤਰ xkb_events *events);

ਸਥਿਰ ਖਾਲੀ
xkb_filter_ctrls_new(ਬਣਤਰ xkb_server_state *state,
                     ਬਣਤਰ xkb_events *events,
                     ਬਣਤਰ xkb_filter *filter)
{
    ਜੇ (filter->action.type == ACTION_TYPE_CTRL_ਸੈੱਟ) {
        /* Set: save the specified controls that are *not* already enabled */
        filter->priv = (uint32_t) (
            (~state->ਅਧਾਰ.components.controls) & filter->action.ctrls.ctrls
        );
    } ਨਹੀਂਤਾਂ {
        /* Lock: save the specified controls that *are* already enabled */
        filter->priv = (uint32_t) (
            state->ਅਧਾਰ.components.controls & filter->action.ctrls.ctrls
        );
    }

    ਜੇ (filter->action.type == ACTION_TYPE_CTRL_ਸੈੱਟ ||
        !(filter->action.ctrls.flags & ACTION_ਤਾਲਾ_NO_ਤਾਲਾ)) {
        /* Enable the specified controls that are not already enabled */
        state->ਅਧਾਰ.components.controls |= filter->action.ctrls.ctrls;
    }
}

ਸਥਿਰ bool
xkb_filter_ctrls_func(ਬਣਤਰ xkb_server_state *state,
                      ਬਣਤਰ xkb_events *events,
                      ਬਣਤਰ xkb_filter *filter,
                      ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                      ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction)
{
    ਜੇ (ਕੁੰਜੀ != filter->ਕੁੰਜੀ)
        ਵਾਪਸ XKB_FILTER_CONTINUE;

    ਚੋਣ (direction) {
    ਹਾਲਤ XKB_ਕੁੰਜੀ_ਥੱਲੇ:
        filter->refcnt++;
        /* fallthrough */
    ਹਾਲਤ XKB_ਕੁੰਜੀ_REPEATED:
        ਵਾਪਸ XKB_FILTER_CONSUME;
    ਮੂਲ:
        ਜੇ (--filter->refcnt > 0)
            ਵਾਪਸ XKB_FILTER_CONSUME;
    }

    ਜੇ (filter->action.type == ACTION_TYPE_CTRL_ਸੈੱਟ ||
        !(filter->action.ctrls.flags & ACTION_ਤਾਲਾ_NO_UNLOCK)) {
        ਅਚਲ ਗਿਣਤੀ xkb_action_controls old = state->ਅਧਾਰ.components.controls;

        /*
         * Set: Disable specified controls that were *not* enabled at key press.
         * Lock: Disable specified controls that *were* enabled at key press.
         */
        state->ਅਧਾਰ.components.controls &= ~(ਗਿਣਤੀ xkb_action_controls) filter->priv;

        ਜੇ ((old & CONTROL_STICKY_ਕੁੰਜੀਆਂ) &&
            !(state->ਅਧਾਰ.components.controls & CONTROL_STICKY_ਕੁੰਜੀਆਂ)) {
            /* Sticky keys were disabled: clear all locks and latches */
            clear_all_latches_and_locks(state, events);
        }
    }

    filter->func = ਨਿਰਾ;
    ਵਾਪਸ XKB_FILTER_CONTINUE;
}

ਸਥਿਰ bool
append_redirect_ਕੁੰਜੀ_events(ਬਣਤਰ xkb_server_state *state,
                           ਬਣਤਰ xkb_events *events,
                           ਅਚਲ ਬਣਤਰ xkb_redirect_ਕੁੰਜੀ_action *redirect,
                           ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction)
{
    ਗਿਣਤੀ xkb_state_component ਬਦਲਿਆ = 0;
    ਅਚਲ xkb_mod_ਨਕਾਬ_t ਨਕਾਬ = redirect->affect_mods;

    /*
     * Reference state: find the last state update in the queue, otherwise
     * use the current state.
     */
    ਬਣਤਰ xkb_event *event;
    ਬਣਤਰ state_components ਆਖਰੀ_components = state->ਅਧਾਰ.components;
    darray_foreach_ਉਲਟ(event, events->ਕਤਾਰ) {
        ਜੇ (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
            ਆਖਰੀ_components = event->components.components;
            ਰੋਕੋ;
        }
    }

    ਜੇ (ਨਕਾਬ) {
        ਬਣਤਰ state_components new = ਆਖਰੀ_components;
        new.ਅਧਾਰ_mods = (new.ਅਧਾਰ_mods & ~ਨਕਾਬ) | redirect->mods;
        new.latched_mods = (new.latched_mods & ~ਨਕਾਬ) | redirect->mods;
        new.locked_mods = (new.locked_mods & ~ਨਕਾਬ) | redirect->mods;
        new.mods = (new.mods & ~ਨਕਾਬ) | redirect->mods;

        ਬਦਲਿਆ = get_state_component_changes(&ਆਖਰੀ_components, &new);
        ਜੇ (ਬਦਲਿਆ) {
            darray_append(events->ਕਤਾਰ, (ਬਣਤਰ xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = new,
                    .ਬਦਲਿਆ = ਬਦਲਿਆ
                }
            });
            /*
             * Commit the temporary modifiers change before the key event
             * it was computed for: consumers must observe the key event
             * under the *redirected* modifiers.
             */
            darray_append(events->ਕਤਾਰ, (ਬਣਤਰ xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_ਚੌਖਟਾ,
            });
        }
    }

    darray_append(events->ਕਤਾਰ, (ਬਣਤਰ xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_ਕੁੰਜੀ,
        .ਕੁੰਜੀ = {
            .keycode = redirect->keycode,
            .direction = direction
        }
    });

    ਜੇ (ਨਕਾਬ && ਬਦਲਿਆ) {
        /* Restore state */
        darray_append(events->ਕਤਾਰ, (ਬਣਤਰ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = ਆਖਰੀ_components,
                .ਬਦਲਿਆ = ਬਦਲਿਆ
            }
        });
    }

    state->flags &= ~STATE_REQUIRE_ਕੁੰਜੀ_EVENT;
    ਵਾਪਸ true;
}

ਸਥਿਰ ਖਾਲੀ
xkb_filter_redirect_ਕੁੰਜੀ_new(ਬਣਤਰ xkb_server_state *state,
                            ਬਣਤਰ xkb_events *events,
                            ਬਣਤਰ xkb_filter *filter)
{
    /* Action effectual only with the xkb_machine API and a valid keycode */
    ਜੇ (!events || filter->action.redirect.keycode == XKB_KEYCODE_INVALID) {
        filter->func = ਨਿਰਾ;
        ਵਾਪਸ;
    }
    append_redirect_ਕੁੰਜੀ_events(state, events, &filter->action.redirect,
                               XKB_ਕੁੰਜੀ_ਥੱਲੇ);
}

ਸਥਿਰ bool
xkb_filter_redirect_ਕੁੰਜੀ_func(ਬਣਤਰ xkb_server_state *state,
                             ਬਣਤਰ xkb_events *events,
                             ਬਣਤਰ xkb_filter *filter,
                             ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                             ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction)
{
    ਜੇ (ਕੁੰਜੀ != filter->ਕੁੰਜੀ)
        ਵਾਪਸ XKB_FILTER_CONTINUE;

    ਜੇ (direction == XKB_ਕੁੰਜੀ_ਉੱਪਰ) {
        append_redirect_ਕੁੰਜੀ_events(state, events,
                                   &filter->action.redirect, XKB_ਕੁੰਜੀ_ਉੱਪਰ);
        filter->func = ਨਿਰਾ;
        ਵਾਪਸ XKB_FILTER_CONSUME;
     } ਨਹੀਂਤਾਂ ਜੇ (direction == XKB_ਕੁੰਜੀ_ਥੱਲੇ) {
        ਅਚਲ ਸੰਘ xkb_action *actions = ਨਿਰਾ;
        ਅਚਲ xkb_action_ਗਿਣਤੀ_t ਗਿਣਤੀ_2 =
            xkb_ਕੁੰਜੀ_get_actions(&state->ਅਧਾਰ, ਕੁੰਜੀ, &actions);

        ਲਈ (xkb_action_ਗਿਣਤੀ_t a = 0; a < ਗਿਣਤੀ_2; a++) {
            ਜੇ (actions[a].type == ACTION_TYPE_REDIRECT_ਕੁੰਜੀ &&
                actions[a].redirect.keycode != filter->action.redirect.keycode) {
                    /* One action redirects to a different key: release first */
                    append_redirect_ਕੁੰਜੀ_events(state, events,
                                               &filter->action.redirect,
                                               XKB_ਕੁੰਜੀ_ਉੱਪਰ);
                filter->func = ਨਿਰਾ;
                ਵਾਪਸ XKB_FILTER_CONTINUE;
            }
        }
    }

    append_redirect_ਕੁੰਜੀ_events(state, events, &filter->action.redirect,
                               direction);
    ਵਾਪਸ XKB_FILTER_CONSUME;
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ {
    ਖਾਲੀ (*new)(ਬਣਤਰ xkb_server_state *state,
                ਬਣਤਰ xkb_events *events,
                ਬਣਤਰ xkb_filter *filter);
    bool (*func)(ਬਣਤਰ xkb_server_state *state,
                 ਬਣਤਰ xkb_events *events,
                 ਬਣਤਰ xkb_filter *filter,
                 ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ, ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction);
} filter_action_funcs[ACTION_TYPE_ਅੰਕ_ENTRIES] = {
    [ACTION_TYPE_MOD_ਸੈੱਟ]     = { xkb_filter_mod_ਸੈੱਟ_new,
                                  xkb_filter_mod_ਸੈੱਟ_func },
    [ACTION_TYPE_MOD_LATCH]   = { xkb_filter_mod_latch_new,
                                  xkb_filter_mod_latch_func },
    [ACTION_TYPE_MOD_ਤਾਲਾ]    = { xkb_filter_mod_ਤਾਲਾ_new,
                                  xkb_filter_mod_ਤਾਲਾ_func },
    [ACTION_TYPE_ਸਮੂਹ_ਸੈੱਟ]   = { xkb_filter_ਸਮੂਹ_ਸੈੱਟ_new,
                                  xkb_filter_ਸਮੂਹ_ਸੈੱਟ_func },
    [ACTION_TYPE_ਸਮੂਹ_LATCH] = { xkb_filter_ਸਮੂਹ_latch_new,
                                  xkb_filter_ਸਮੂਹ_latch_func },
    [ACTION_TYPE_ਸਮੂਹ_ਤਾਲਾ]  = { xkb_filter_ਸਮੂਹ_ਤਾਲਾ_new,
                                  xkb_filter_ਸਮੂਹ_ਤਾਲਾ_func },
    [ACTION_TYPE_PTR_MOVE]    = { xkb_filter_ਸੂਚਕ_move_new,
                                  xkb_filter_ਸੂਚਕ_move_func },
    [ACTION_TYPE_PTR_BUTTON]  = { xkb_filter_ਸੂਚਕ_button_new,
                                  xkb_filter_ਸੂਚਕ_button_func },
    [ACTION_TYPE_PTR_ਤਾਲਾ]    = { xkb_filter_ਸੂਚਕ_ਤਾਲਾ_button_new,
                                  xkb_filter_ਸੂਚਕ_ਤਾਲਾ_button_func },
    [ACTION_TYPE_PTR_DEFAULT] = { xkb_filter_ਸੂਚਕ_ਸੈੱਟ_default_button_new,
                                  xkb_filter_ਸੂਚਕ_ਸੈੱਟ_default_button_func },
    [ACTION_TYPE_TERMINATE]   = { xkb_filter_terminate_server_new,
                                  xkb_filter_terminate_server_func },
    [ACTION_TYPE_SWITCH_VT]   = { xkb_filter_switch_virtual_console_new,
                                  xkb_filter_switch_virtual_console_func },
    [ACTION_TYPE_CTRL_ਸੈੱਟ]    = { xkb_filter_ctrls_new,
                                  xkb_filter_ctrls_func },
    [ACTION_TYPE_CTRL_ਤਾਲਾ]   = { xkb_filter_ctrls_new,
                                  xkb_filter_ctrls_func },
    [ACTION_TYPE_REDIRECT_ਕੁੰਜੀ] = { xkb_filter_redirect_ਕੁੰਜੀ_new,
                                   xkb_filter_redirect_ਕੁੰਜੀ_func },
};

/**
 * Applies any relevant filters to the key, first from the list of filters
 * that are currently active, then if no filter has claimed the key, possibly
 * apply a new filter from the key action.
 */
ਸਥਿਰ ਖਾਲੀ
xkb_filter_apply_all(ਬਣਤਰ xkb_server_state *state,
                     ਬਣਤਰ xkb_events *events,
                     ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                     ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction)
{
    /* First run through all the currently active filters and see if any of
     * them have consumed this event. */
    bool consumed = false;
    ਬਣਤਰ xkb_filter *filter;
    darray_foreach(filter, state->filters) {
        ਜੇ (!filter->func)
            ਜਾਰੀ;

        ਜੇ (filter->func(state, events, filter, ਕੁੰਜੀ, direction) == XKB_FILTER_CONSUME)
            consumed = true;
    }
    ਜੇ (consumed || direction == XKB_ਕੁੰਜੀ_ਉੱਪਰ)
        ਵਾਪਸ;

    /* No filter consumed this event, so proceed with the key actions */
    ਅਚਲ ਸੰਘ xkb_action *actions = ਨਿਰਾ;
    xkb_action_ਗਿਣਤੀ_t ਗਿਣਤੀ_2 = xkb_ਕੁੰਜੀ_get_actions(&state->ਅਧਾਰ, ਕੁੰਜੀ,
                                                         &actions);

    ਲਈ (ਬਣਤਰ {
            xkb_action_ਗਿਣਤੀ_t ਗਿਣਤੀ_2;
            ਅਚਲ ਸੰਘ xkb_action *actions;
         } ਅਗਲਾ = {0};
         ਗਿਣਤੀ_2;
         ਗਿਣਤੀ_2 = ਅਗਲਾ.ਗਿਣਤੀ_2, ਅਗਲਾ.ਗਿਣਤੀ_2 = 0, actions = ਅਗਲਾ.actions
    ) {
        /*
         * Process actions sequentially.
         *
         * NOTE: We rely on the parser to disallow multiple modifier or group
         * actions (see `CheckMultipleActionsCategories`). Allowing multiple
         * such actions requires a refactor of the state handling.
         */
        ਲਈ (xkb_action_ਗਿਣਤੀ_t k = 0; k < ਗਿਣਤੀ_2; k++) {
            /*
             * It's possible for the keymap to set action->type explicitly, like
             * so:
             *     interpret XF86_Next_VMode {
             *         action = Private(type=0x86, data="+VMode");
             *     };
             * We don't handle those.
             */
            ਜੇ (actions[k].type >= ACTION_TYPE_ਅੰਕ_ENTRIES)
                ਜਾਰੀ;

            /* Go to next action if no corresponding action handler */
            ਜੇ (!filter_action_funcs[actions[k].type].new)
                ਜਾਰੀ;

            /* Add a new filter and run the corresponding initial action */
            filter = xkb_filter_new(state);
            filter->ਕੁੰਜੀ = ਕੁੰਜੀ;
            filter->action = actions[k];

            ਚੋਣ (filter->action.type) {
            ਹਾਲਤ ACTION_TYPE_MOD_ਸੈੱਟ:
                ਜੇ (state->ਅਧਾਰ.components.controls & CONTROL_STICKY_ਕੁੰਜੀਆਂ) {
                    /* Convert modifier set action to a latch */
                    filter->action.type = ACTION_TYPE_MOD_LATCH;
                    ਜੇ (state->a11y_flags & XKB_A11_Y_STICKY_ਕੁੰਜੀਆਂ_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ) {
                        filter->action.mods.flags |= ACTION_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ;
                    }
                }
                /* fallthrough */
            ਹਾਲਤ ACTION_TYPE_MOD_LATCH:
            ਹਾਲਤ ACTION_TYPE_MOD_ਤਾਲਾ: {
                ਸਥਿਰ ਅਚਲ ਗਿਣਤੀ xkb_action_flags unlock_mods_on_press
                    = ACTION_ਤਾਲਾ_CLEAR     /* SetMods & LatchMods: can unlock */
                    | ACTION_ਤਾਲਾ_NO_UNLOCK /* LockMods: cannot unlock */
                    | ACTION_EXPLICIT_UNLOCK_ON_PRESS;
                ਜੇ ((state->flags & STATE_UNLOCK_MODS_ON_PRESS) &&
                    /*
                     * Ensure that the action can actually unlock and has
                     * no *explicit* `unlockOnPress` parameter.
                     */
                    (filter->action.mods.flags & unlock_mods_on_press) ==
                    (filter->action.type == ACTION_TYPE_MOD_ਤਾਲਾ
                        ? 0 : ACTION_ਤਾਲਾ_CLEAR))
                {
                    filter->action.mods.flags |= ACTION_UNLOCK_ON_PRESS;
                }
                ਰੋਕੋ;
            }
            ਹਾਲਤ ACTION_TYPE_ਸਮੂਹ_ਸੈੱਟ:
                ਜੇ (state->ਅਧਾਰ.components.controls & CONTROL_STICKY_ਕੁੰਜੀਆਂ) {
                    /* Convert group set action to a latch */
                    filter->action.type = ACTION_TYPE_ਸਮੂਹ_LATCH;
                    ਜੇ (state->a11y_flags & XKB_A11_Y_STICKY_ਕੁੰਜੀਆਂ_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ) {
                        filter->action.ਸਮੂਹ.flags |= ACTION_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ;
                    }
                }
                ਰੋਕੋ;
            ਹਾਲਤ ACTION_TYPE_ਸਮੂਹ_ਤਾਲਾ:
                ਜੇ ((filter->action.ਸਮੂਹ.flags &
                     ACTION_AUTO_ਸਮੂਹ_ਤਾਲਾ_ON_MODIFIER_RELEASE) &&
                    (state->flags &
                     STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_ਕੁੰਜੀ) &&
                    /*
                     * FIXME: sticky keys may leave an unexpected modifier
                     * from the level 0 latched, so disable for now.
                     */
                    !(state->ਅਧਾਰ.components.controls & CONTROL_STICKY_ਕੁੰਜੀਆਂ))
                {
                    filter->action.ਸਮੂਹ.flags |= ACTION_ਤਾਲਾ_ON_RELEASE;
                    /* Add base level action */
                    ਅਗਲਾ.ਗਿਣਤੀ_2 = xkb_keymap_ਕੁੰਜੀ_get_actions_by_level(
                        state->ਅਧਾਰ.keymap, ਕੁੰਜੀ, state->ਅਧਾਰ.components.ਸਮੂਹ,
                        0, &ਅਗਲਾ.actions
                    );
                    ਪ੍ਰਤਿਜ੍ਞਾ(ਅਗਲਾ.ਗਿਣਤੀ_2 == 1);
                }
                ਰੋਕੋ;
            ਹਾਲਤ ACTION_TYPE_PTR_MOVE:
            ਹਾਲਤ ACTION_TYPE_PTR_BUTTON:
            ਹਾਲਤ ACTION_TYPE_PTR_ਤਾਲਾ:
            ਹਾਲਤ ACTION_TYPE_PTR_DEFAULT:
                ਜੇ (!(state->ਅਧਾਰ.components.controls & CONTROL_MOUSE_ਕੁੰਜੀਆਂ)) {
                    /* Convert pointer actions to NoAction */
                    ਪ੍ਰਤਿਜ੍ਞਾ(!filter_action_funcs[ACTION_TYPE_NONE].new);
                    filter->func = ਨਿਰਾ;
                    ਜਾਰੀ;
                }
                ਰੋਕੋ;
            ਹਾਲਤ ACTION_TYPE_REDIRECT_ਕੁੰਜੀ:
                // FIXME: this is not efficient to resolve mods here each time
                filter->action.redirect.affect_mods = mod_ਨਕਾਬ_get_effective(
                    state->ਅਧਾਰ.keymap, filter->action.redirect.affect_mods
                );
                filter->action.redirect.mods = mod_ਨਕਾਬ_get_effective(
                    state->ਅਧਾਰ.keymap, filter->action.redirect.mods
                );
                ਰੋਕੋ;
            ਮੂਲ: ;
            }
            filter->func = filter_action_funcs[filter->action.type].func;
            filter_action_funcs[filter->action.type].new(state, events, filter);
        }
    }
}

ਸਥਿਰ ਖਾਲੀ
xkb_state_update_derived(ਬਣਤਰ xkb_state *state);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
xkb_state_init(ਬਣਤਰ xkb_state * ਸੀਮਿਤ state,
               ਬਣਤਰ xkb_keymap * ਸੀਮਿਤ keymap,
               ਗਿਣਤੀ xkb_state_ਬਹੁਲਕ_ਅੰਦਰੂਨੀ ਬਹੁਲਕ)
{
    state->ਬਹੁਲਕ = ਬਹੁਲਕ;
    state->refcnt = 1;
    state->keymap = xkb_keymap_ref(keymap);
    state->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ.policy = XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_WRAP;

    /* Ensure that derived state is correctly initialized */
    xkb_state_update_derived(state);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
xkb_client_state_init(ਬਣਤਰ xkb_client_state * ਸੀਮਿਤ state,
                      ਬਣਤਰ xkb_keymap * ਸੀਮਿਤ keymap,
                      ਗਿਣਤੀ xkb_state_ਬਹੁਲਕ_ਅੰਦਰੂਨੀ ਬਹੁਲਕ)
{
    xkb_state_init(&state->ਅਧਾਰ, keymap, ਬਹੁਲਕ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
xkb_server_state_init(ਬਣਤਰ xkb_server_state * ਸੀਮਿਤ state,
                      ਬਣਤਰ xkb_keymap * ਸੀਮਿਤ keymap,
                      ਗਿਣਤੀ xkb_state_ਬਹੁਲਕ_ਅੰਦਰੂਨੀ ਬਹੁਲਕ,
                      ਗਿਣਤੀ xkb_a11y_flags a11y_affect,
                      ਗਿਣਤੀ xkb_a11y_flags a11y_flags)
{
    xkb_state_init(&state->ਅਧਾਰ, keymap, ਬਹੁਲਕ);

    state->a11y_flags = a11y_flags;
    ਜੇ (keymap->format != XKB_KEYMAP_FORMAT_ਪਾਠ_V1 &&
        !(a11y_affect & XKB_A11_Y_LATCH_ਇੱਕੋ_ਵੇਲੇ_ਕੁੰਜੀਆਂ)) {
             /* Keymap v2+: enable extension to XKB if not manually disabled */
             state->a11y_flags |= XKB_A11_Y_LATCH_ਇੱਕੋ_ਵੇਲੇ_ਕੁੰਜੀਆਂ;
    }
}

ਬਣਤਰ xkb_state *
xkb_state_new(ਬਣਤਰ xkb_keymap *keymap)
{
    ਬਣਤਰ xkb_server_state * ਅਚਲ state = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(*state));
    ਜੇ (!state) {
        log_err_func1(keymap->ctx, XKB_ERROR_ਵੰਡ_FAILURE_2,
                      "Could not allocate a state object.\n");
        ਵਾਪਸ ਨਿਰਾ;
    }

    xkb_server_state_init(state, keymap, LEGACY_MIXED_STATE, 0, 0);

    ਵਾਪਸ (ਬਣਤਰ xkb_state *)state;
}

ਬਣਤਰ xkb_state *
xkb_state_new_with_ਬਹੁਲਕ(ਬਣਤਰ xkb_keymap * ਸੀਮਿਤ keymap,
                        ਗਿਣਤੀ xkb_state_ਬਹੁਲਕ ਬਹੁਲਕ,
                        ਗਿਣਤੀ xkb_ਸਥਿਤੀ * ਸੀਮਿਤ ਸਥਿਤੀ)
{
    ਚੋਣ (ਬਹੁਲਕ) {
    ਹਾਲਤ XKB_STATE_ਬਹੁਲਕ_CLIENT:
    ਹਾਲਤ XKB_STATE_ਬਹੁਲਕ_SERVER_QUERY: {
        ਬਣਤਰ xkb_client_state * ਅਚਲ state = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(*state));
        ਜੇ (!state)
            ਰੋਕੋ;

        static_assert((ਬਿਨਾਨਿਸ਼ਾਨੀ)XKB_STATE_ਬਹੁਲਕ_CLIENT ==
                      (ਬਿਨਾਨਿਸ਼ਾਨੀ)CLIENT_STATE, "");
        static_assert((ਬਿਨਾਨਿਸ਼ਾਨੀ)XKB_STATE_ਬਹੁਲਕ_SERVER_QUERY ==
                      (ਬਿਨਾਨਿਸ਼ਾਨੀ)SERVER_COMPANION, "");
        xkb_client_state_init(state, keymap, (ਗਿਣਤੀ xkb_state_ਬਹੁਲਕ_ਅੰਦਰੂਨੀ)ਬਹੁਲਕ);

        ਜੇ (ਸਥਿਤੀ)
            *ਸਥਿਤੀ = XKB_SUCCESS;

        ਵਾਪਸ (ਬਣਤਰ xkb_state *)state;
    }
    ਹਾਲਤ XKB_STATE_ਬਹੁਲਕ_SERVER: {
        ਬਣਤਰ xkb_server_state * ਅਚਲ state = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(*state));
        ਜੇ (!state)
            ਰੋਕੋ;

        xkb_server_state_init(state, keymap, LEGACY_SERVER_STATE, 0, 0);

        ਜੇ (ਸਥਿਤੀ)
            *ਸਥਿਤੀ = XKB_SUCCESS;

        ਵਾਪਸ (ਬਣਤਰ xkb_state *)state;
    }
    ਮੂਲ:
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_STATE_ਬਹੁਲਕ_2,
                     "Unsupported state mode: %u\n", ਬਹੁਲਕ);
        ਜੇ (ਸਥਿਤੀ)
            *ਸਥਿਤੀ = XKB_ERROR_UNSUPPORTED_STATE_ਬਹੁਲਕ;
        ਵਾਪਸ ਨਿਰਾ;
    }

    log_err_func1(keymap->ctx, XKB_ERROR_ਵੰਡ_FAILURE_2,
                  "Could not allocate a state object.\n");
    ਜੇ (ਸਥਿਤੀ)
        *ਸਥਿਤੀ = XKB_ERROR_ਵੰਡ_FAILURE;
    ਵਾਪਸ ਨਿਰਾ;
}

ਬਣਤਰ xkb_state *
xkb_state_ref(ਬਣਤਰ xkb_state *state)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(state->refcnt > 0);
    state->refcnt++;
    ਵਾਪਸ state;
}

ਖਾਲੀ
xkb_state_reset(ਬਣਤਰ xkb_state *state)
{
    memset(&state->components, 0, ਆਕਾਰ(state->components));

    ਜੇ (state->ਬਹੁਲਕ < LEGACY_MIXED_STATE) {
        static_assert(ਆਕਾਰ(*state) == ਆਕਾਰ(ਬਣਤਰ xkb_client_state),
                      "Not a mere wrapper");
        /* Nothing else to reset */
    } ਨਹੀਂਤਾਂ {
        ਬਣਤਰ xkb_server_state *state_ = (ਬਣਤਰ xkb_server_state *)state;
        ਅਚਲ ਬਣਤਰ xkb_server_state prev = *state_;
        memset(state_, 0, ਆਕਾਰ(*state_));

        state_->ਅਧਾਰ = prev.ਅਧਾਰ;
        state_->a11y_flags = prev.a11y_flags;
        state_->filters = prev.filters;
        darray_ਮਾਪ(state_->filters) = 0;
    }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
xkb_state_destroy(ਬਣਤਰ xkb_state *state)
{
    xkb_keymap_unref(state->keymap);
    ਜੇ (state->ਬਹੁਲਕ > SERVER_COMPANION)
        darray_free(((ਬਣਤਰ xkb_server_state *)state)->filters);
}

ਖਾਲੀ
xkb_state_unref(ਬਣਤਰ xkb_state *state)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(!state || state->refcnt > 0);
    ਜੇ (!state || --state->refcnt > 0)
        ਵਾਪਸ;

    xkb_state_destroy(state);

    ਚੋਣ (state->ਬਹੁਲਕ) {
    ਹਾਲਤ CLIENT_STATE:
    ਹਾਲਤ SERVER_COMPANION:
        ਮੁਕਤ((ਬਣਤਰ xkb_client_state *)state);
        ਰੋਕੋ;
    ਹਾਲਤ LEGACY_MIXED_STATE:
    ਹਾਲਤ LEGACY_SERVER_STATE:
        ਮੁਕਤ((ਬਣਤਰ xkb_server_state *)state);
        ਰੋਕੋ;
    ਹਾਲਤ SERVER_STATE: /* handled by xkb_machine_unref() */
    ਮੂਲ:
        PANIC_UNREACHABLE();
    }
}

ਬਣਤਰ xkb_keymap *
xkb_state_get_keymap(ਬਣਤਰ xkb_state *state)
{
    ਵਾਪਸ state->keymap;
}

/**
 * Update the LED state to match the rest of the xkb_state.
 */
ਸਥਿਰ ਖਾਲੀ
xkb_state_led_update_all(ਬਣਤਰ xkb_state *state)
{
    xkb_led_ਸੂਚਕਾਂਕ_t idx;
    ਅਚਲ ਬਣਤਰ xkb_led *led;

    state->components.leds = 0;

    xkb_leds_enumerate(idx, led, state->keymap) {

        ਜੇ (led->which_mods != 0 && led->mods.ਨਕਾਬ != 0) {
            xkb_mod_ਨਕਾਬ_t mod_ਨਕਾਬ = 0;
            ਜੇ (led->which_mods & XKB_STATE_MODS_EFFECTIVE)
                mod_ਨਕਾਬ |= state->components.mods;
            ਜੇ (led->which_mods & XKB_STATE_MODS_DEPRESSED)
                mod_ਨਕਾਬ |= state->components.ਅਧਾਰ_mods;
            ਜੇ (led->which_mods & XKB_STATE_MODS_LATCHED)
                mod_ਨਕਾਬ |= state->components.latched_mods;
            ਜੇ (led->which_mods & XKB_STATE_MODS_LOCKED)
                mod_ਨਕਾਬ |= state->components.locked_mods;

            ਜੇ (led->mods.ਨਕਾਬ & mod_ਨਕਾਬ) {
                state->components.leds |= (UINT32_C(1) << idx);
                ਜਾਰੀ;
            }
        }

        ਜੇ (led->which_groups != 0) {
            ਜੇ (likely(led->groups) != 0) {
                xkb_layout_ਨਕਾਬ_t ਸਮੂਹ_ਨਕਾਬ = 0;
                /* Effective and locked groups have been brought into range */
                ਪ੍ਰਤਿਜ੍ਞਾ(state->components.ਸਮੂਹ < XKB_MAX_GROUPS);
                ਪ੍ਰਤਿਜ੍ਞਾ(state->components.locked_ਸਮੂਹ >= 0 &&
                       state->components.locked_ਸਮੂਹ < XKB_MAX_GROUPS);
                /* Effective and locked groups are used as mask */
                ਜੇ (led->which_groups & XKB_STATE_LAYOUT_EFFECTIVE)
                    ਸਮੂਹ_ਨਕਾਬ |= (UINT32_C(1) << state->components.ਸਮੂਹ);
                ਜੇ (led->which_groups & XKB_STATE_LAYOUT_LOCKED)
                    ਸਮੂਹ_ਨਕਾਬ |= (UINT32_C(1) << state->components.locked_ਸਮੂਹ);
                /* Base and latched groups only have to be non-zero */
                ਜੇ ((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                    state->components.ਅਧਾਰ_ਸਮੂਹ != 0)
                    ਸਮੂਹ_ਨਕਾਬ |= led->groups;
                ਜੇ ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                    state->components.latched_ਸਮੂਹ != 0)
                    ਸਮੂਹ_ਨਕਾਬ |= led->groups;

                ਜੇ (led->groups & ਸਮੂਹ_ਨਕਾਬ) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    ਜਾਰੀ;
                }
            } ਨਹੀਂਤਾਂ {
                /* Special case for Base and latched groups */
                ਜੇ (((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                     state->components.ਅਧਾਰ_ਸਮੂਹ == 0) ||
                    ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                     state->components.latched_ਸਮੂਹ == 0)) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    ਜਾਰੀ;
                }
            }
        }

        ਜੇ (led->ctrls & state->components.controls) {
            state->components.leds |= (UINT32_C(1) << idx);
            ਜਾਰੀ;
        }
    }
}

/**
 * Calculates the derived state (effective mods/group and LEDs) from an
 * up-to-date xkb_state.
 */
ਸਥਿਰ ਖਾਲੀ
xkb_state_update_derived(ਬਣਤਰ xkb_state *state)
{
    xkb_layout_ਸੂਚਕਾਂਕ_t wrapped;

    state->components.mods = (state->components.ਅਧਾਰ_mods |
                              state->components.latched_mods |
                              state->components.locked_mods);

    /* Lock group must be adjusted, but not base nor latched groups */
    wrapped = Xkb_Wrap_ਸਮੂਹ_Into_Range(state->components.locked_ਸਮੂਹ,
                                    state->keymap->ਅੰਕ_groups,
                                    state->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ.policy,
                                    state->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ.redirect_ਸਮੂਹ);
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->components.locked_ਸਮੂਹ =
        (int32_t) (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    /* Effective group must be adjusted */
    wrapped = Xkb_Wrap_ਸਮੂਹ_Into_Range(state->components.ਅਧਾਰ_ਸਮੂਹ +
                                    state->components.latched_ਸਮੂਹ +
                                    state->components.locked_ਸਮੂਹ,
                                    state->keymap->ਅੰਕ_groups,
                                    state->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ.policy,
                                    state->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ.redirect_ਸਮੂਹ);
    state->components.ਸਮੂਹ =
        (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    xkb_state_led_update_all(state);
}

ਸਥਿਰ ਖਾਲੀ
state_update_enabled_controls(ਬਣਤਰ xkb_server_state *state,
                              ਗਿਣਤੀ xkb_keyboard_control_flags affect_controls,
                              ਗਿਣਤੀ xkb_keyboard_control_flags controls,
                              ਬਣਤਰ xkb_events *events)
{
    ਅਚਲ bool had_sticky_ਕੁੰਜੀਆਂ = state->ਅਧਾਰ.components.controls
                               & CONTROL_STICKY_ਕੁੰਜੀਆਂ;

    /*
     * Enable using the public API with the all the Control values, except
     * the internal ones, if any.
     */
    affect_controls =
        ( affect_controls
        & (ਗਿਣਤੀ xkb_keyboard_control_flags)CONTROL_ALL_BOOLEAN );
    state->ਅਧਾਰ.components.controls &=
        (ਗਿਣਤੀ xkb_action_controls)~affect_controls;
    state->ਅਧਾਰ.components.controls |=
        (ਗਿਣਤੀ xkb_action_controls)(controls & affect_controls);

    ਜੇ (had_sticky_ਕੁੰਜੀਆਂ &&
        !(state->ਅਧਾਰ.components.controls & CONTROL_STICKY_ਕੁੰਜੀਆਂ)) {
        /* Sticky keys were disabled: clear all locks and latches */
        clear_all_latches_and_locks(state, events);
    }

    xkb_state_update_derived(&state->ਅਧਾਰ);
}

/**
 * Given a particular key event, updates the state structure to reflect the
 * new modifiers.
 */
ਗਿਣਤੀ xkb_state_component
xkb_state_update_ਕੁੰਜੀ(ਬਣਤਰ xkb_state *ਅਧਾਰ_state, xkb_keycode_t kc,
                     ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction)
{
    /* Guard against client-only state */
    ਪ੍ਰਤਿਜ੍ਞਾ(ਅਧਾਰ_state->ਬਹੁਲਕ > SERVER_COMPANION);
    ਜੇ (ਅਧਾਰ_state->ਬਹੁਲਕ <= SERVER_COMPANION) {
        log_err(ਅਧਾਰ_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_ਬਹੁਲਕ,
                "%s: Unexpected state type %d\n", __func__, ਅਧਾਰ_state->ਬਹੁਲਕ);
        ਵਾਪਸ 0;
    }

    ਬਣਤਰ xkb_server_state * ਅਚਲ state =
        (ਬਣਤਰ xkb_server_state *)ਅਧਾਰ_state;

    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ* ਅਚਲ ਕੁੰਜੀ = Xkb_ਕੁੰਜੀ(state->ਅਧਾਰ.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    ਜੇ (!ਕੁੰਜੀ || (direction == XKB_ਕੁੰਜੀ_REPEATED && !ਕੁੰਜੀ->repeats))
        ਵਾਪਸ 0;

    ਜੇ (direction == XKB_ਕੁੰਜੀ_ਥੱਲੇ &&
        (state->ਅਧਾਰ.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ਕੁੰਜੀਆਂ) &&
        (state->a11y_flags & XKB_A11_Y_STICKY_ਕੁੰਜੀਆਂ_NO_ਇੱਕੋ_ਵੇਲੇ_ਕੁੰਜੀਆਂ) &&
        state->ਅਧਾਰ.components.ਅਧਾਰ_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ਕੁੰਜੀਆਂ,
                                      0, ਨਿਰਾ);
    }

    ਅਚਲ ਬਣਤਰ state_components prev_components = state->ਅਧਾਰ.components;

    state->ਸੈੱਟ_mods = 0;
    state->clear_mods = 0;

    xkb_filter_apply_all(state, ਨਿਰਾ, ਕੁੰਜੀ, direction);

    xkb_mod_ਸੂਚਕਾਂਕ_t i;
    xkb_mod_ਨਕਾਬ_t ਦੋਆਧਾਰੀ_ਅੰਕ;
    ਲਈ (i = 0, ਦੋਆਧਾਰੀ_ਅੰਕ = 1; state->ਸੈੱਟ_mods; i++, ਦੋਆਧਾਰੀ_ਅੰਕ <<= 1) {
        ਜੇ (state->ਸੈੱਟ_mods & ਦੋਆਧਾਰੀ_ਅੰਕ) {
            state->mod_ਕੁੰਜੀ_ਗਿਣਤੀ[i]++;
            state->ਅਧਾਰ.components.ਅਧਾਰ_mods |= ਦੋਆਧਾਰੀ_ਅੰਕ;
            state->ਸੈੱਟ_mods &= ~ਦੋਆਧਾਰੀ_ਅੰਕ;
        }
    }

    ਲਈ (i = 0, ਦੋਆਧਾਰੀ_ਅੰਕ = 1; state->clear_mods; i++, ਦੋਆਧਾਰੀ_ਅੰਕ <<= 1) {
        ਜੇ (state->clear_mods & ਦੋਆਧਾਰੀ_ਅੰਕ) {
            state->mod_ਕੁੰਜੀ_ਗਿਣਤੀ[i]--;
            ਜੇ (state->mod_ਕੁੰਜੀ_ਗਿਣਤੀ[i] <= 0) {
                state->ਅਧਾਰ.components.ਅਧਾਰ_mods &= ~ਦੋਆਧਾਰੀ_ਅੰਕ;
                state->mod_ਕੁੰਜੀ_ਗਿਣਤੀ[i] = 0;
            }
            state->clear_mods &= ~ਦੋਆਧਾਰੀ_ਅੰਕ;
        }
    }

    xkb_state_update_derived(&state->ਅਧਾਰ);

    ਵਾਪਸ get_state_component_changes(&prev_components, &state->ਅਧਾਰ.components);
}

/* We need fake keys for `update_latch_modifiers` and `update_latch_group`.
 * These keys must have at least one level in order to break latches. We need 2
 * keys with specific actions in order to update group/mod latches without
 * affecting each other. */
ਸਥਿਰ ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ synthetic_ਕੁੰਜੀ_level_ਇੰਦਰਾਜ = { 0 };
ਸਥਿਰ ਬਣਤਰ xkb_ਕੁੰਜੀ_type synthetic_ਕੁੰਜੀ_type = {
     .ਅੰਕ_entries = 1,
     .ਅੰਕ_ਪੱਧਰ = 1,
     .entries = &synthetic_ਕੁੰਜੀ_level_ਇੰਦਰਾਜ
};
ਸਥਿਰ ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ synthetic_ਕੁੰਜੀ = { 0 };

/* Transcription from xserver: XkbLatchModifiers */
ਸਥਿਰ ਖਾਲੀ
update_latch_modifiers(ਬਣਤਰ xkb_server_state *state,
                       ਬਣਤਰ xkb_events *events,
                       xkb_mod_ਨਕਾਬ_t ਨਕਾਬ, xkb_mod_ਨਕਾਬ_t latches)
{
    /* Clear affected latched modifiers */
    ਅਚਲ xkb_mod_ਨਕਾਬ_t clear = ਨਕਾਬ & ~latches;
    state->ਅਧਾਰ.components.latched_mods &= ~clear;

    /* Clear any pending latch to locks using ad hoc action:
     * only affect corresponding modifier latches and no group latch. */
    ਬਣਤਰ xkb_level synthetic_ਕੁੰਜੀ_level_break_mod_latch = {
        .ਅੰਕ_syms = 0,
        .ਅੰਕ_actions = 1,
        .ਉੱਤਲਾ = XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ,
        .s.sym = XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ,
        .a.action.ਅੰਦਰੂਨੀ = {
            .type = ACTION_TYPE_ਅੰਦਰੂਨੀ,
            .flags = ਅੰਦਰੂਨੀ_BREAKS_MOD_LATCH,
            .clear_latched_mods = clear
        }
    };
    ਬਣਤਰ xkb_ਸਮੂਹ synthetic_ਕੁੰਜੀ_ਸਮੂਹ_break_mod_latch = {
        .type = &synthetic_ਕੁੰਜੀ_type,
        .ਪੱਧਰ = &synthetic_ਕੁੰਜੀ_level_break_mod_latch
    };
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ synthetic_ਕੁੰਜੀ_break_mod_latch = {
        .ਅੰਕ_groups = 1,
        .groups = &synthetic_ਕੁੰਜੀ_ਸਮੂਹ_break_mod_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_ਕੁੰਜੀ_break_mod_latch, XKB_ਕੁੰਜੀ_ਥੱਲੇ);

    /* Finally set the latched mods by simulate tapping a key with the
     * corresponding action */
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ* ਅਚਲ ਕੁੰਜੀ = &synthetic_ਕੁੰਜੀ;
    ਅਚਲ ਸੰਘ xkb_action latch_mods = {
        .mods = {
            .type = ACTION_TYPE_MOD_LATCH,
            .mods = { .ਨਕਾਬ = ਨਕਾਬ & latches },
            .flags = 0,
        },
    };
    ਬਣਤਰ xkb_filter* ਅਚਲ filter = xkb_filter_new(state);
    filter->ਕੁੰਜੀ = ਕੁੰਜੀ;
    filter->func = xkb_filter_mod_latch_func;
    filter->action = latch_mods;
    xkb_filter_mod_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_mod_latch_func(state, events, filter, ਕੁੰਜੀ, XKB_ਕੁੰਜੀ_ਉੱਪਰ);
}

/* Transcription from xserver: XkbLatchGroup */
ਸਥਿਰ ਖਾਲੀ
update_latch_ਸਮੂਹ(ਬਣਤਰ xkb_server_state *state,
                   ਬਣਤਰ xkb_events *events, int32_t ਸਮੂਹ)
{
    /* Clear any pending latch to locks. */
    ਸਥਿਰ ਬਣਤਰ xkb_level synthetic_ਕੁੰਜੀ_level_break_ਸਮੂਹ_latch = {
        .ਅੰਕ_syms = 0,
        .ਅੰਕ_actions = 1,
        .ਉੱਤਲਾ = XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ,
        .s.sym = XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ,
        .a.action.ਅੰਦਰੂਨੀ = {
            .type = ACTION_TYPE_ਅੰਦਰੂਨੀ,
            .flags = ਅੰਦਰੂਨੀ_BREAKS_ਸਮੂਹ_LATCH,
            .clear_latched_mods = 0
        }
    };
    ਸਥਿਰ ਬਣਤਰ xkb_ਸਮੂਹ synthetic_ਕੁੰਜੀ_ਸਮੂਹ_break_ਸਮੂਹ_latch = {
        .type = &synthetic_ਕੁੰਜੀ_type,
        .ਪੱਧਰ = &synthetic_ਕੁੰਜੀ_level_break_ਸਮੂਹ_latch
    };
    ਸਥਿਰ ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ synthetic_ਕੁੰਜੀ_break_ਸਮੂਹ_latch = {
        .ਅੰਕ_groups = 1,
        .groups = &synthetic_ਕੁੰਜੀ_ਸਮੂਹ_break_ਸਮੂਹ_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_ਕੁੰਜੀ_break_ਸਮੂਹ_latch, XKB_ਕੁੰਜੀ_ਥੱਲੇ);

    /* Simulate tapping a key with a group latch action, but in isolation: i.e.
     * without affecting the other filters. */
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ* ਅਚਲ ਕੁੰਜੀ = &synthetic_ਕੁੰਜੀ;
    ਅਚਲ ਸੰਘ xkb_action latch_ਸਮੂਹ = {
        .ਸਮੂਹ = {
            .type = ACTION_TYPE_ਸਮੂਹ_LATCH,
            .flags = ACTION_ABSOLUTE_SWITCH,
            .ਸਮੂਹ = ਸਮੂਹ,
        },
    };
    ਬਣਤਰ xkb_filter* ਅਚਲ filter = xkb_filter_new(state);
    filter->ਕੁੰਜੀ = ਕੁੰਜੀ;
    filter->func = xkb_filter_ਸਮੂਹ_latch_func;
    filter->action = latch_ਸਮੂਹ;
    xkb_filter_ਸਮੂਹ_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_ਸਮੂਹ_latch_func(state, events, filter, ਕੁੰਜੀ, XKB_ਕੁੰਜੀ_ਉੱਪਰ);
}

/**
 * Compute the resolved effective mask of an arbitrary input.
 *
 * Contrary to `mod_mask_get_effective`, it resolves only modifiers not present
 * in the canonical mask, so that it enables `xkb_state_serialize_mods` to
 * round trip via `xkb_state_update_mask`.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ xkb_mod_ਨਕਾਬ_t
resolve_ਮੰਜ਼ਿਲ_canonical_mods(ਬਣਤਰ xkb_keymap *keymap, xkb_mod_ਨਕਾਬ_t mods)
{
    ਵਾਪਸ
        /*
         * Keep canonical modifier mask.
         * It contains either real modifiers or canonical virtual modifiers.
         */
        (mods & keymap->canonical_state_ਨਕਾਬ) |
        /* Resolve other modifiers */
        mod_ਨਕਾਬ_get_effective(keymap,
                               mods & ~keymap->canonical_state_ਨਕਾਬ);
}

ਸਥਿਰ ਖਾਲੀ
state_update_latched_locked(
    ਬਣਤਰ xkb_server_state * ਸੀਮਿਤ state,
    ਅਚਲ ਬਣਤਰ xkb_state_components_update * ਸੀਮਿਤ update,
    ਬਣਤਰ xkb_events * ਸੀਮਿਤ events
)
{
    /* Update locks */
    ਅਚਲ xkb_mod_ਨਕਾਬ_t affect_locked_mods =
        resolve_ਮੰਜ਼ਿਲ_canonical_mods(state->ਅਧਾਰ.keymap, update->affect_locked_mods);
    ਜੇ ((update->components & XKB_STATE_MODS_LOCKED) &&
        affect_locked_mods) {
        ਅਚਲ xkb_mod_ਨਕਾਬ_t locked_mods =
            resolve_ਮੰਜ਼ਿਲ_canonical_mods(state->ਅਧਾਰ.keymap, update->locked_mods);
        state->ਅਧਾਰ.components.locked_mods &= ~affect_locked_mods;
        state->ਅਧਾਰ.components.locked_mods |= locked_mods & affect_locked_mods;
    }
    ਜੇ (update->components & XKB_STATE_LAYOUT_LOCKED) {
        state->ਅਧਾਰ.components.locked_ਸਮੂਹ = update->locked_layout;
    }

    /* Update latches */
    ਅਚਲ xkb_mod_ਨਕਾਬ_t affect_latched_mods =
        resolve_ਮੰਜ਼ਿਲ_canonical_mods(state->ਅਧਾਰ.keymap, update->affect_latched_mods);
    ਜੇ ((update->components & XKB_STATE_MODS_LATCHED) &&
        affect_latched_mods) {
        ਅਚਲ xkb_mod_ਨਕਾਬ_t latched_mods =
            resolve_ਮੰਜ਼ਿਲ_canonical_mods(state->ਅਧਾਰ.keymap, update->latched_mods);
        update_latch_modifiers(state, events, affect_latched_mods, latched_mods);
    }
    ਜੇ (update->components & XKB_STATE_LAYOUT_LATCHED) {
        update_latch_ਸਮੂਹ(state, events, update->latched_layout);
    }
}

ਗਿਣਤੀ xkb_state_component
xkb_state_update_latched_locked(ਬਣਤਰ xkb_state *ਅਧਾਰ_state,
                                xkb_mod_ਨਕਾਬ_t affect_latched_mods,
                                xkb_mod_ਨਕਾਬ_t latched_mods,
                                bool affect_latched_layout,
                                int32_t latched_layout,
                                xkb_mod_ਨਕਾਬ_t affect_locked_mods,
                                xkb_mod_ਨਕਾਬ_t locked_mods,
                                bool affect_locked_layout,
                                int32_t locked_layout)
{
    /* Guard against client-only state */
    ਪ੍ਰਤਿਜ੍ਞਾ(ਅਧਾਰ_state->ਬਹੁਲਕ > SERVER_COMPANION);
    ਜੇ (ਅਧਾਰ_state->ਬਹੁਲਕ <= SERVER_COMPANION) {
        log_err(ਅਧਾਰ_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_ਬਹੁਲਕ,
                "%s: Unexpected state type %d\n", __func__, ਅਧਾਰ_state->ਬਹੁਲਕ);
        ਵਾਪਸ 0;
    }

    ਬਣਤਰ xkb_server_state * ਅਚਲ state =
        (ਬਣਤਰ xkb_server_state *)ਅਧਾਰ_state;

    ਅਚਲ ਬਣਤਰ state_components ਪਿਛਲਾ_components = state->ਅਧਾਰ.components;
    ਅਚਲ ਗਿਣਤੀ xkb_state_component components
        = ((affect_latched_mods || latched_mods) ? XKB_STATE_MODS_LATCHED : 0)
        | ((affect_locked_mods || locked_mods) ? XKB_STATE_MODS_LOCKED : 0)
        | (affect_latched_layout ? XKB_STATE_LAYOUT_LATCHED : 0)
        | (affect_locked_layout ? XKB_STATE_LAYOUT_LOCKED : 0);
    ਅਚਲ ਬਣਤਰ xkb_state_components_update update = {
        .ਮਾਪ = ਆਕਾਰ(update),
        .components = components,
        .affect_latched_mods = affect_latched_mods,
        .latched_mods = latched_mods,
        .latched_layout = latched_layout,
        .affect_locked_mods = affect_locked_mods,
        .locked_mods = locked_mods,
        .locked_layout = locked_layout
    };
    state_update_latched_locked(state, &update, ਨਿਰਾ);
    xkb_state_update_derived(&state->ਅਧਾਰ);
    ਵਾਪਸ get_state_component_changes(&ਪਿਛਲਾ_components,
                                       &state->ਅਧਾਰ.components);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
clear_all_latches_and_locks(ਬਣਤਰ xkb_server_state *state,
                            ਬਣਤਰ xkb_events *events)
{
    ਸਥਿਰ ਅਚਲ ਗਿਣਤੀ xkb_state_component components
        = XKB_STATE_MODS_LATCHED | XKB_STATE_MODS_LOCKED
        | XKB_STATE_LAYOUT_LATCHED | XKB_STATE_LAYOUT_LOCKED;
    ਅਚਲ ਬਣਤਰ xkb_state_components_update update = {
        .ਮਾਪ = ਆਕਾਰ(update),
        .components = components,
        .affect_latched_mods = (xkb_mod_ਨਕਾਬ_t)XKB_MOD_ALL,
        .latched_mods = 0,
        .latched_layout = 0,
        .affect_locked_mods = (xkb_mod_ਨਕਾਬ_t)XKB_MOD_ALL,
        .locked_mods = 0,
        .locked_layout = 0
    };
    state_update_latched_locked(state, &update, events);
}

ਸਥਿਰ ਗਿਣਤੀ xkb_ਸਥਿਤੀ
state_update_layout_policy(ਬਣਤਰ xkb_server_state *state,
                           ਅਚਲ ਬਣਤਰ xkb_layout_policy_update *update)
{
    ਜੇ (xkb_feature_supported(XKB_FEATURE_ENUM_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_POLICY,
                              update->policy)) {
        ਜੇ (update->policy == XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_REDIRECT) {
            ਜੇ (update->redirect < state->ਅਧਾਰ.keymap->ਅੰਕ_groups) {
                state->ਅਧਾਰ.ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ.redirect_ਸਮੂਹ =
                    update->redirect;
            } ਨਹੀਂਤਾਂ {
                log_err(state->ਅਧਾਰ.keymap->ctx,
                        XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ_2,
                        "Layout policy: "
                        "unsupported layout index %"PRIu32" > %"PRIu32"\n",
                        update->redirect + 1, state->ਅਧਾਰ.keymap->ਅੰਕ_groups);
                ਵਾਪਸ XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ;
            }
        }
        state->ਅਧਾਰ.ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ.policy = update->policy;
        ਵਾਪਸ XKB_SUCCESS;
    } ਨਹੀਂਤਾਂ {
        log_err(state->ਅਧਾਰ.keymap->ctx,
                XKB_ERROR_UNSUPPORTED_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_POLICY_2,
                "Unsupported layout policy: %d\n", update->policy);
        ਵਾਪਸ XKB_ERROR_UNSUPPORTED_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_POLICY;
    }
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
ਸਥਿਰ ਗਿਣਤੀ xkb_ਸਥਿਤੀ
check_state_update_abi_(ਬਣਤਰ xkb_context * ਸੀਮਿਤ ctx,
                        ਅਚਲ ਅੱਖਰ * ਸੀਮਿਤ func,
                        ਅਚਲ ਬਣਤਰ xkb_synthetic_update * ਸੀਮਿਤ update)
{
    ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ = XKB_SUCCESS;
    ਜੇ ((ਸਥਿਤੀ = xkb_check_state_abi(update)) ||
        (update->reserved0 != 0 &&
         (ਸਥਿਤੀ = XKB_ERROR_ABI_FORWARD_COMPAT)) ||
        (update->components &&
         (ਸਥਿਤੀ = xkb_check_state_abi(update->components))) ||
        (update->layout_policy &&
         (ਸਥਿਤੀ = xkb_check_state_abi(update->layout_policy)))) {
        xkb_log_abi_error(ctx, func, ਸਥਿਤੀ);
    }
    ਵਾਪਸ ਸਥਿਤੀ;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ check_state_update_abi(ctx, update) \
    check_state_update_abi_(ctx, __func__, update)

ਗਿਣਤੀ xkb_ਸਥਿਤੀ
xkb_state_update_synthetic(ਬਣਤਰ xkb_state * ਅਧਾਰ_state,
                           ਅਚਲ ਬਣਤਰ xkb_synthetic_update * update,
                           ਗਿਣਤੀ xkb_state_component *ਬਦਲਿਆ)
{
    /* Guard against client-only state */
    ਜੇ (ਅਧਾਰ_state->ਬਹੁਲਕ <= SERVER_COMPANION) {
        log_err(ਅਧਾਰ_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_ਬਹੁਲਕ,
                "%s: Unexpected state type %d\n", __func__, ਅਧਾਰ_state->ਬਹੁਲਕ);
        ਵਾਪਸ XKB_ERROR_UNEXPECTED_STATE_ਬਹੁਲਕ;
    }

    ਬਣਤਰ xkb_server_state * ਅਚਲ state =
        (ਬਣਤਰ xkb_server_state *)ਅਧਾਰ_state;

    /* Check ABI compatibility */
    ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ = check_state_update_abi(state->ਅਧਾਰ.keymap->ctx,
                                                    update);
    ਜੇ (ਸਥਿਤੀ != XKB_SUCCESS)
        ਵਾਪਸ ਸਥਿਤੀ;

    ਅਚਲ ਬਣਤਰ state_components ਪਿਛਲਾ_components = state->ਅਧਾਰ.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    /* Update parametrized controls first */
    ਜੇ (update->layout_policy) {
        ਸਥਿਤੀ = state_update_layout_policy(state, update->layout_policy);
        ਜੇ (ਸਥਿਤੀ != XKB_SUCCESS)
            ਵਾਪਸ ਸਥਿਤੀ;
    }

    ਜੇ (update->components) {
        ਅਚਲ ਬਣਤਰ xkb_state_components_update * ਅਚਲ components =
            update->components;
        /* Update boolean controls first */
        ਜੇ (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, ਨਿਰਾ);
        }

        state_update_latched_locked(state, components, ਨਿਰਾ);
    }

    xkb_state_update_derived(&state->ਅਧਾਰ);

    ਜੇ (ਬਦਲਿਆ) {
        *ਬਦਲਿਆ = get_state_component_changes(&ਪਿਛਲਾ_components,
                                               &state->ਅਧਾਰ.components);
    }
    ਵਾਪਸ XKB_SUCCESS;
}

/**
 * Updates the state from a set of explicit masks as gained from
 * xkb_state_serialize_mods and xkb_state_serialize_groups.  As noted in the
 * documentation for these functions in xkbcommon.h, this round-trip is
 * lossy, and should only be used to update a slave state mirroring the
 * master, e.g. in a client/server window system.
 */
ਗਿਣਤੀ xkb_state_component
xkb_state_update_ਨਕਾਬ(ਬਣਤਰ xkb_state *ਅਧਾਰ_state,
                      xkb_mod_ਨਕਾਬ_t ਅਧਾਰ_mods,
                      xkb_mod_ਨਕਾਬ_t latched_mods,
                      xkb_mod_ਨਕਾਬ_t locked_mods,
                      xkb_layout_ਸੂਚਕਾਂਕ_t ਅਧਾਰ_ਸਮੂਹ,
                      xkb_layout_ਸੂਚਕਾਂਕ_t latched_ਸਮੂਹ,
                      xkb_layout_ਸੂਚਕਾਂਕ_t locked_ਸਮੂਹ)
{
    /* Guard against server-only and server companion state */
    ਪ੍ਰਤਿਜ੍ਞਾ(ਅਧਾਰ_state->ਬਹੁਲਕ == CLIENT_STATE ||
           ਅਧਾਰ_state->ਬਹੁਲਕ == LEGACY_MIXED_STATE);
    ਜੇ (ਅਧਾਰ_state->ਬਹੁਲਕ != CLIENT_STATE &&
        ਅਧਾਰ_state->ਬਹੁਲਕ != LEGACY_MIXED_STATE) {
        log_err(ਅਧਾਰ_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_ਬਹੁਲਕ,
                "%s: Unexpected state type %d\n", __func__, ਅਧਾਰ_state->ਬਹੁਲਕ);
        ਵਾਪਸ 0;
    }

    ਬਣਤਰ xkb_client_state * ਅਚਲ state =
        (ਬਣਤਰ xkb_client_state *)ਅਧਾਰ_state;

    ਅਚਲ ਬਣਤਰ state_components prev_components = state->ਅਧਾਰ.components;

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
    state->ਅਧਾਰ.components.ਅਧਾਰ_mods =
        resolve_ਮੰਜ਼ਿਲ_canonical_mods(state->ਅਧਾਰ.keymap, ਅਧਾਰ_mods);
    state->ਅਧਾਰ.components.latched_mods =
        resolve_ਮੰਜ਼ਿਲ_canonical_mods(state->ਅਧਾਰ.keymap, latched_mods);
    state->ਅਧਾਰ.components.locked_mods =
        resolve_ਮੰਜ਼ਿਲ_canonical_mods(state->ਅਧਾਰ.keymap, locked_mods);

    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->ਅਧਾਰ.components.ਅਧਾਰ_ਸਮੂਹ = (int32_t) ਅਧਾਰ_ਸਮੂਹ;
    state->ਅਧਾਰ.components.latched_ਸਮੂਹ = (int32_t) latched_ਸਮੂਹ;
    state->ਅਧਾਰ.components.locked_ਸਮੂਹ = (int32_t) locked_ਸਮੂਹ;

    xkb_state_update_derived(&state->ਅਧਾਰ);

    ਵਾਪਸ get_state_component_changes(&prev_components, &state->ਅਧਾਰ.components);
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Lock_Modifier
 */
ਸਥਿਰ bool
should_do_caps_transformation(ਬਣਤਰ xkb_state *state, xkb_keycode_t kc)
{
    ਵਾਪਸ
        xkb_state_mod_ਸੂਚਕਾਂਕ_is_ਸਰਗਰਮ(state, XKB_MOD_ਸੂਚਕਾਂਕ_CAPS,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_ਸੂਚਕਾਂਕ_is_consumed(state, kc, XKB_MOD_ਸੂਚਕਾਂਕ_CAPS) == 0;
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Control_Modifier
 */
ਸਥਿਰ bool
should_do_ctrl_transformation(ਬਣਤਰ xkb_state *state, xkb_keycode_t kc)
{
    ਵਾਪਸ
        xkb_state_mod_ਸੂਚਕਾਂਕ_is_ਸਰਗਰਮ(state, XKB_MOD_ਸੂਚਕਾਂਕ_CTRL,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_ਸੂਚਕਾਂਕ_is_consumed(state, kc, XKB_MOD_ਸੂਚਕਾਂਕ_CTRL) == 0;
}

/**
 * Provides the symbols to use for the given key and state.  Returns the
 * number of symbols pointed to in syms_out.
 */
ਪੂਰਨ
xkb_state_ਕੁੰਜੀ_get_syms(ਬਣਤਰ xkb_state *state, xkb_keycode_t kc,
                       ਅਚਲ xkb_keysym_t **syms_ਬਾਹਰਲਾ)
{
    ਅਚਲ xkb_layout_ਸੂਚਕਾਂਕ_t layout = xkb_state_ਕੁੰਜੀ_get_layout(state, kc);
    ਜੇ (layout == XKB_LAYOUT_INVALID)
        ਜਾਓ err;

    ਅਚਲ xkb_level_ਸੂਚਕਾਂਕ_t level = xkb_state_ਕੁੰਜੀ_get_level(state, kc, layout);
    ਜੇ (level == XKB_LEVEL_INVALID)
        ਜਾਓ err;

    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ* ਅਚਲ ਕੁੰਜੀ = Xkb_ਕੁੰਜੀ(state->keymap, kc);

    ਜੇ (!ਕੁੰਜੀ)
        ਜਾਓ err;

    ਅਚਲ ਬਣਤਰ xkb_level* ਅਚਲ leveli =
        xkb_keymap_ਕੁੰਜੀ_get_level(state->keymap, ਕੁੰਜੀ, layout, level);

    ਜੇ (!leveli)
        ਜਾਓ err;

    ਅਚਲ xkb_keysym_ਗਿਣਤੀ_t ਅੰਕ_syms = leveli->ਅੰਕ_syms;
    ਜੇ (ਅੰਕ_syms == 0)
        ਜਾਓ err;

    ਜੇ (should_do_caps_transformation(state, kc)) {
        /* Only simple capitalization rules: keysyms count is unchanged. */
        ਜੇ (ਅੰਕ_syms > 1) {
            *syms_ਬਾਹਰਲਾ = (leveli->has_ਉੱਤਲਾ)
                      ? leveli->s.syms + ਅੰਕ_syms
                      : leveli->s.syms;
        } ਨਹੀਂਤਾਂ {
            *syms_ਬਾਹਰਲਾ = &leveli->ਉੱਤਲਾ;
        }
    } ਨਹੀਂਤਾਂ {
        *syms_ਬਾਹਰਲਾ = (ਅੰਕ_syms > 1)
                  ? leveli->s.syms
                  : &leveli->s.sym;
    }
    ਵਾਪਸ (ਪੂਰਨ) ਅੰਕ_syms;

err:
    *syms_ਬਾਹਰਲਾ = ਨਿਰਾ;
    ਵਾਪਸ 0;
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
ਸਥਿਰ ਅੱਖਰ
Xkb_ਮੰਜ਼ਿਲ_Control(ਅੱਖਰ ch)
{
    ਅੱਖਰ c = ch;

    ਜੇ ((c >= '@' && c < '\177') || c == ' ')
        c &= 0x1F;
    ਨਹੀਂਤਾਂ ਜੇ (c == '2')
        c = '\000';
    ਨਹੀਂਤਾਂ ਜੇ (c >= '3' && c <= '7')
        c -= ('3' - '\033');
    ਨਹੀਂਤਾਂ ਜੇ (c == '8')
        c = '\177';
    ਨਹੀਂਤਾਂ ਜੇ (c == '/')
        c = '_' & 0x1F;
    ਵਾਪਸ c;
}

/**
 * Provides either exactly one symbol, or XKB_KEY_NoSymbol.
 */
xkb_keysym_t
xkb_state_ਕੁੰਜੀ_get_one_sym(ਬਣਤਰ xkb_state *state, xkb_keycode_t kc)
{
    ਅਚਲ xkb_keysym_t *syms = ਨਿਰਾ;
    ਅਚਲ ਪੂਰਨ ਅੰਕ_syms = xkb_state_ਕੁੰਜੀ_get_syms(state, kc, &syms);

    ਜੇ (ਅੰਕ_syms != 1)
        ਵਾਪਸ XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
    ਨਹੀਂਤਾਂ
        ਵਾਪਸ syms[0];
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
ਸਥਿਰ xkb_keysym_t
get_one_sym_for_string(ਬਣਤਰ xkb_state *state, xkb_keycode_t kc)
{
    ਅਚਲ xkb_layout_ਸੂਚਕਾਂਕ_t layout = xkb_state_ਕੁੰਜੀ_get_layout(state, kc);
    ਅਚਲ xkb_layout_ਸੂਚਕਾਂਕ_t ਅੰਕ_layouts =
        xkb_keymap_ਅੰਕ_layouts_for_ਕੁੰਜੀ(state->keymap, kc);
    xkb_level_ਸੂਚਕਾਂਕ_t level = xkb_state_ਕੁੰਜੀ_get_level(state, kc, layout);
    ਜੇ (layout == XKB_LAYOUT_INVALID || ਅੰਕ_layouts == 0 ||
        level == XKB_LEVEL_INVALID)
        ਵਾਪਸ XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;

    ਅਚਲ xkb_keysym_t *syms = ਨਿਰਾ;
    ਪੂਰਨ nsyms = xkb_keymap_ਕੁੰਜੀ_get_syms_by_level(state->keymap, kc,
                                                 layout, level, &syms);
    ਜੇ (nsyms != 1)
        ਵਾਪਸ XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
    xkb_keysym_t sym = syms[0];

    ਜੇ (should_do_ctrl_transformation(state, kc) && sym > 127u) {
        ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t i = 0; i < ਅੰਕ_layouts; i++) {
            level = xkb_state_ਕੁੰਜੀ_get_level(state, kc, i);
            ਜੇ (level == XKB_LEVEL_INVALID)
                ਜਾਰੀ;

            nsyms = xkb_keymap_ਕੁੰਜੀ_get_syms_by_level(state->keymap, kc,
                                                     i, level, &syms);
            ਜੇ (nsyms == 1 && syms[0] <= 127u) {
                sym = syms[0];
                ਰੋਕੋ;
            }
        }
    }

    ਜੇ (should_do_caps_transformation(state, kc)) {
        sym = xkb_keysym_ਮੰਜ਼ਿਲ_ਉੱਤਲਾ(sym);
    }

    ਵਾਪਸ sym;
}

ਪੂਰਨ
xkb_state_ਕੁੰਜੀ_get_utf8(ਬਣਤਰ xkb_state *state, xkb_keycode_t kc,
                       ਅੱਖਰ *ਬਫਰ, ਮਾਪ_t ਮਾਪ)
{
    ਪੂਰਨ nsyms;
    ਅਚਲ xkb_keysym_t *syms = ਨਿਰਾ;
    ਅਚਲ xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    ਜੇ (sym != XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ) {
        nsyms = 1; syms = &sym;
    }
    ਨਹੀਂਤਾਂ {
        nsyms = xkb_state_ਕੁੰਜੀ_get_syms(state, kc, &syms);
    }

    /* Make sure not to truncate in the middle of a UTF-8 sequence. */
    ਪੂਰਨ ਵਿੱਥ = 0;
    ਅੱਖਰ tmp[XKB_KEYSYM_UTF8_MAX_ਮਾਪ];
    ਲਈ (ਪੂਰਨ i = 0; i < nsyms; i++) {
        ਪੂਰਨ ret = xkb_keysym_ਮੰਜ਼ਿਲ_utf8(syms[i], tmp, ਆਕਾਰ(tmp));
        ਜੇ (ret <= 0)
            ਜਾਓ err_bad;

        ret--;
        ਜੇ ((ਮਾਪ_t)ਵਿੱਥ + (ਮਾਪ_t)ret <= ਮਾਪ)
            ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਬਫਰ + ਵਿੱਥ, tmp, (ਮਾਪ_t)ret);
        ਵਿੱਥ += ret;
    }

    ਜੇ ((ਮਾਪ_t)ਵਿੱਥ >= ਮਾਪ)
        ਜਾਓ err_trunc;
    ਬਫਰ[ਵਿੱਥ] = '\0';

    ਜੇ (!is_valid_utf8(ਬਫਰ, (ਮਾਪ_t)ਵਿੱਥ))
        ਜਾਓ err_bad;

    ਜੇ (ਵਿੱਥ == 1 && (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) ਬਫਰ[0] <= 127u &&
        should_do_ctrl_transformation(state, kc))
        ਬਫਰ[0] = Xkb_ਮੰਜ਼ਿਲ_Control(ਬਫਰ[0]);

    ਵਾਪਸ ਵਿੱਥ;

err_trunc:
    ਜੇ (ਮਾਪ > 0)
        ਬਫਰ[ਮਾਪ - 1] = '\0';
    ਵਾਪਸ ਵਿੱਥ;

err_bad:
    ਜੇ (ਮਾਪ > 0)
        ਬਫਰ[0] = '\0';
    ਵਾਪਸ 0;
}

uint32_t
xkb_state_ਕੁੰਜੀ_get_utf32(ਬਣਤਰ xkb_state *state, xkb_keycode_t kc)
{
    ਅਚਲ xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    uint32_t cp = xkb_keysym_ਮੰਜ਼ਿਲ_utf32(sym);

    ਜੇ (cp <= 127u && should_do_ctrl_transformation(state, kc))
        cp = (uint32_t) Xkb_ਮੰਜ਼ਿਲ_Control((ਅੱਖਰ) cp);

    ਵਾਪਸ cp;
}

/**
 * Serializes the requested modifier state into an xkb_mod_mask_t, with all
 * the same disclaimers as in xkb_state_update_mask.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ xkb_mod_ਨਕਾਬ_t
serialize_mods(ਅਚਲ ਬਣਤਰ state_components *components,
               ਗਿਣਤੀ xkb_state_component type)
{
    xkb_mod_ਨਕਾਬ_t ret = 0;

    ਜੇ (type & XKB_STATE_MODS_EFFECTIVE)
        ਵਾਪਸ components->mods;

    ਜੇ (type & XKB_STATE_MODS_DEPRESSED)
        ret |= components->ਅਧਾਰ_mods;
    ਜੇ (type & XKB_STATE_MODS_LATCHED)
        ret |= components->latched_mods;
    ਜੇ (type & XKB_STATE_MODS_LOCKED)
        ret |= components->locked_mods;

    ਵਾਪਸ ret;
}

xkb_mod_ਨਕਾਬ_t
xkb_state_serialize_mods(ਬਣਤਰ xkb_state *state,
                         ਗਿਣਤੀ xkb_state_component type)
{
    ਵਾਪਸ serialize_mods(&state->components, type);
}

/**
 * Serialises the requested group state, with all the same disclaimers as
 * in xkb_state_update_mask.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ xkb_layout_ਸੂਚਕਾਂਕ_t
serialize_layout(ਅਚਲ ਬਣਤਰ state_components *components,
                 ਗਿਣਤੀ xkb_state_component type)
{
    ਜੇ (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ਵਾਪਸ components->ਸਮੂਹ;

    int32_t ret = 0;
    ਜੇ (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret += components->ਅਧਾਰ_ਸਮੂਹ;
    ਜੇ (type & XKB_STATE_LAYOUT_LATCHED)
        ret += components->latched_ਸਮੂਹ;
    ਜੇ (type & XKB_STATE_LAYOUT_LOCKED)
        ret += components->locked_ਸਮੂਹ;

    ਵਾਪਸ (xkb_layout_ਸੂਚਕਾਂਕ_t)ret;
}

xkb_layout_ਸੂਚਕਾਂਕ_t
xkb_state_serialize_layout(ਬਣਤਰ xkb_state *state,
                           ਗਿਣਤੀ xkb_state_component type)
{
    ਵਾਪਸ serialize_layout(&state->components, type);
}

xkb_led_ਨਕਾਬ_t
xkb_state_serialize_leds(ਅਚਲ ਬਣਤਰ xkb_state *state,
                         ਗਿਣਤੀ xkb_state_component type)
{
    ਵਾਪਸ (type & XKB_STATE_LEDS)
        ? state->components.leds
        : 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਗਿਣਤੀ xkb_keyboard_control_flags
serialize_controls(ਅਚਲ ਬਣਤਰ state_components *components,
                   ਗਿਣਤੀ xkb_state_component type)
{
    ਵਾਪਸ (type & XKB_STATE_CONTROLS_EFFECTIVE)
        /*
         * Enable using the public API with the all the Controls values, except
         * the internal ones, if any.
         */
        ? (ਗਿਣਤੀ xkb_keyboard_control_flags)
          (components->controls & CONTROL_ALL_BOOLEAN)
        : 0;
}

ਗਿਣਤੀ xkb_keyboard_control_flags
xkb_state_serialize_controls(ਅਚਲ ਬਣਤਰ xkb_state *state,
                             ਗਿਣਤੀ xkb_state_component type)
{
    ਵਾਪਸ serialize_controls(&state->components, type);
}

/**
 * Gets a modifier mask and returns the resolved effective mask; this
 * is needed because some modifiers can also map to other modifiers, e.g.
 * the "NumLock" modifier usually also sets the "Mod2" modifier.
 */
xkb_mod_ਨਕਾਬ_t
mod_ਨਕਾਬ_get_effective(ਬਣਤਰ xkb_keymap *keymap, xkb_mod_ਨਕਾਬ_t mods)
{
    /* Initialize the effective mask with its corresponding real mods. */
    xkb_mod_ਨਕਾਬ_t ਨਕਾਬ = mods & MOD_REAL_ਨਕਾਬ_ALL;

    /* Resolve the virtual modifiers */
    ਅਚਲ ਬਣਤਰ xkb_mod *mod;
    xkb_mod_ਸੂਚਕਾਂਕ_t i;
    xkb_vmods_enumerate(i, mod, &keymap->mods)
        ਜੇ (mods & (UINT32_C(1) << i))
            ਨਕਾਬ |= mod->mapping;

    ਵਾਪਸ ਨਕਾਬ;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
ਪੂਰਨ
xkb_state_mod_ਸੂਚਕਾਂਕ_is_ਸਰਗਰਮ(ਬਣਤਰ xkb_state *state,
                              xkb_mod_ਸੂਚਕਾਂਕ_t idx,
                              ਗਿਣਤੀ xkb_state_component type)
{
    ਜੇ (unlikely(idx >= xkb_keymap_ਅੰਕ_mods(state->keymap)))
        ਵਾਪਸ -1;

    ਅਚਲ xkb_mod_ਨਕਾਬ_t mapping = state->keymap->mods.mods[idx].mapping;
    ਜੇ (!mapping) {
        /* Modifier not mapped */
        ਵਾਪਸ 0;
    }
    /* WARNING: this may overmatch for virtual modifiers */
    ਵਾਪਸ (xkb_state_serialize_mods(state, type) & mapping) == mapping;
}

ਗਿਣਤੀ {
    XKB_STATE_ਮੇਲ_FLAGS = XKB_STATE_ਮੇਲ_ANY
                          | XKB_STATE_ਮੇਲ_ALL
                          | XKB_STATE_ਮੇਲ_NON_EXCLUSIVE
};

/**
 * Helper function for xkb_state_mod_indices_are_active and
 * xkb_state_mod_names_are_active.
 */
ਸਥਿਰ bool
ਮੇਲ_mod_masks(ਬਣਤਰ xkb_state *state,
                ਗਿਣਤੀ xkb_state_component type,
                ਗਿਣਤੀ xkb_state_ਮੇਲ ਮੇਲ,
                xkb_mod_ਨਕਾਬ_t wanted)
{
    ਅਚਲ xkb_mod_ਨਕਾਬ_t ਸਰਗਰਮ = xkb_state_serialize_mods(state, type);

    ਜੇ (!(ਮੇਲ & XKB_STATE_ਮੇਲ_NON_EXCLUSIVE) && (ਸਰਗਰਮ & ~wanted))
        ਵਾਪਸ false;

    ਜੇ (ਮੇਲ & XKB_STATE_ਮੇਲ_ANY)
        ਵਾਪਸ ਸਰਗਰਮ & wanted;

    /* Ensure all flags but XKB_STATE_MATCH_ALL are processed before */
    static_assert(XKB_STATE_ਮੇਲ_ALL ==
                  ((ਗਿਣਤੀ xkb_state_ਮੇਲ) XKB_STATE_ਮੇਲ_FLAGS &
                   ~(XKB_STATE_ਮੇਲ_ANY | XKB_STATE_ਮੇਲ_NON_EXCLUSIVE)),
                  "");

    ਵਾਪਸ (ਸਰਗਰਮ & wanted) == wanted;
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, or -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ਪੂਰਨ
xkb_state_mod_indices_are_ਸਰਗਰਮ(ਬਣਤਰ xkb_state *state,
                                 ਗਿਣਤੀ xkb_state_component type,
                                 ਗਿਣਤੀ xkb_state_ਮੇਲ ਮੇਲ,
                                 ...)
{
    ਜੇ (ਮੇਲ & ~((ਗਿਣਤੀ xkb_state_ਮੇਲ) XKB_STATE_ਮੇਲ_FLAGS))
        ਵਾਪਸ -2;

    va_list ap;
    xkb_mod_ਨਕਾਬ_t wanted = 0;
    ਪੂਰਨ ret = 0;
    ਅਚਲ xkb_mod_ਸੂਚਕਾਂਕ_t ਅੰਕ_mods = xkb_keymap_ਅੰਕ_mods(state->keymap);

    va_ਸ਼ੁਰੂ(ap, ਮੇਲ);
    ਜਦੋਂ (1) {
        xkb_mod_ਸੂਚਕਾਂਕ_t idx = va_arg(ap, xkb_mod_ਸੂਚਕਾਂਕ_t);
        ਜੇ (idx == XKB_MOD_INVALID)
            ਰੋਕੋ;
        ਜੇ (unlikely(idx >= ਅੰਕ_mods)) {
            ret = -1;
            ਰੋਕੋ;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_ਸਿਰਾ(ap);

    ਜੇ (ret == -1)
        ਵਾਪਸ ret;

    ਜੇ (!wanted) {
        /* Modifiers not mapped */
        ਵਾਪਸ 0;
    }

    ਵਾਪਸ ਮੇਲ_mod_masks(state, type, ਮੇਲ, wanted);
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
ਪੂਰਨ
xkb_state_mod_name_is_ਸਰਗਰਮ(ਬਣਤਰ xkb_state *state, ਅਚਲ ਅੱਖਰ *name,
                             ਗਿਣਤੀ xkb_state_component type)
{
    ਅਚਲ xkb_mod_ਸੂਚਕਾਂਕ_t idx = xkb_keymap_mod_get_ਸੂਚਕਾਂਕ(state->keymap, name);

    ਜੇ (idx == XKB_MOD_INVALID)
        ਵਾਪਸ -1;

    ਵਾਪਸ xkb_state_mod_ਸੂਚਕਾਂਕ_is_ਸਰਗਰਮ(state, idx, type);
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ATTR_NULL_SENTINEL ਪੂਰਨ
xkb_state_mod_ਨਾਂ_are_ਸਰਗਰਮ(ਬਣਤਰ xkb_state *state,
                               ਗਿਣਤੀ xkb_state_component type,
                               ਗਿਣਤੀ xkb_state_ਮੇਲ ਮੇਲ,
                               ...)
{
    ਜੇ (ਮੇਲ & ~((ਗਿਣਤੀ xkb_state_ਮੇਲ) XKB_STATE_ਮੇਲ_FLAGS))
        ਵਾਪਸ -2;

    va_list ap;
    xkb_mod_ਨਕਾਬ_t wanted = 0;
    ਪੂਰਨ ret = 0;

    va_ਸ਼ੁਰੂ(ap, ਮੇਲ);
    ਜਦੋਂ (1) {
        ਅਚਲ ਅੱਖਰ *str = va_arg(ap, ਅਚਲ ਅੱਖਰ *);
        ਜੇ (str == ਨਿਰਾ)
            ਰੋਕੋ;
        ਅਚਲ xkb_mod_ਸੂਚਕਾਂਕ_t idx = xkb_keymap_mod_get_ਸੂਚਕਾਂਕ(state->keymap, str);
        ਜੇ (idx == XKB_MOD_INVALID) {
            ret = -1;
            ਰੋਕੋ;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_ਸਿਰਾ(ap);

    ਜੇ (ret == -1)
        ਵਾਪਸ ret;

    ਜੇ (!wanted) {
        /* Modifiers not mapped */
        ਵਾਪਸ 0;
    }

    ਵਾਪਸ ਮੇਲ_mod_masks(state, type, ਮੇਲ, wanted);
}

/**
 * Returns 1 if the given group is active with the specified type(s), 0 if
 * not, or -1 if the group is invalid.
 */
ਪੂਰਨ
xkb_state_layout_ਸੂਚਕਾਂਕ_is_ਸਰਗਰਮ(ਬਣਤਰ xkb_state *state,
                                 xkb_layout_ਸੂਚਕਾਂਕ_t idx,
                                 ਗਿਣਤੀ xkb_state_component type)
{
    ਜੇ (idx >= state->keymap->ਅੰਕ_groups)
        ਵਾਪਸ -1;

    ਪੂਰਨ ret = 0;
    ਜੇ (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ret |= (state->components.ਸਮੂਹ == idx);
    ਜੇ (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret |= (state->components.ਅਧਾਰ_ਸਮੂਹ == (int32_t) idx);
    ਜੇ (type & XKB_STATE_LAYOUT_LATCHED)
        ret |= (state->components.latched_ਸਮੂਹ == (int32_t) idx);
    ਜੇ (type & XKB_STATE_LAYOUT_LOCKED)
        ret |= (state->components.locked_ਸਮੂਹ == (int32_t) idx);

    ਵਾਪਸ ret;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
ਪੂਰਨ
xkb_state_layout_name_is_ਸਰਗਰਮ(ਬਣਤਰ xkb_state *state, ਅਚਲ ਅੱਖਰ *name,
                                ਗਿਣਤੀ xkb_state_component type)
{
    ਅਚਲ xkb_layout_ਸੂਚਕਾਂਕ_t idx =
        xkb_keymap_layout_get_ਸੂਚਕਾਂਕ(state->keymap, name);

    ਜੇ (idx == XKB_LAYOUT_INVALID)
        ਵਾਪਸ -1;

    ਵਾਪਸ xkb_state_layout_ਸੂਚਕਾਂਕ_is_ਸਰਗਰਮ(state, idx, type);
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
ਪੂਰਨ
xkb_state_led_ਸੂਚਕਾਂਕ_is_ਸਰਗਰਮ(ਬਣਤਰ xkb_state *state, xkb_led_ਸੂਚਕਾਂਕ_t idx)
{
    ਜੇ (idx >= state->keymap->ਅੰਕ_leds ||
        state->keymap->leds[idx].name == XKB_ATOM_NONE)
        ਵਾਪਸ -1;

    ਵਾਪਸ !!(state->components.leds & (UINT32_C(1) << idx));
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
ਪੂਰਨ
xkb_state_led_name_is_ਸਰਗਰਮ(ਬਣਤਰ xkb_state *state, ਅਚਲ ਅੱਖਰ *name)
{
    ਅਚਲ xkb_led_ਸੂਚਕਾਂਕ_t idx = xkb_keymap_led_get_ਸੂਚਕਾਂਕ(state->keymap, name);

    ਜੇ (idx == XKB_LED_INVALID)
        ਵਾਪਸ -1;

    ਵਾਪਸ xkb_state_led_ਸੂਚਕਾਂਕ_is_ਸਰਗਰਮ(state, idx);
}

/**
 * See:
 * - XkbTranslateKeyCode(3), mod_rtrn return value, from libX11.
 * - MyEnhancedXkbTranslateKeyCode(), a modification of the above, from GTK+.
 */
ਸਥਿਰ xkb_mod_ਨਕਾਬ_t
ਕੁੰਜੀ_get_consumed(ਬਣਤਰ xkb_state *state, ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                 ਗਿਣਤੀ xkb_consumed_ਬਹੁਲਕ ਬਹੁਲਕ)
{
    ਅਚਲ xkb_layout_ਸੂਚਕਾਂਕ_t ਸਮੂਹ =
        xkb_state_ਕੁੰਜੀ_get_layout(state, ਕੁੰਜੀ->keycode);
    ਜੇ (ਸਮੂਹ == XKB_LAYOUT_INVALID)
        ਵਾਪਸ 0;

    xkb_mod_ਨਕਾਬ_t preserve = 0;
    xkb_mod_ਨਕਾਬ_t consumed = 0;

    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ* ਅਚਲ matching_ਇੰਦਰਾਜ =
        get_ਇੰਦਰਾਜ_for_ਕੁੰਜੀ_state(state, ਕੁੰਜੀ, ਸਮੂਹ);
    ਜੇ (matching_ਇੰਦਰਾਜ)
        preserve = matching_ਇੰਦਰਾਜ->preserve.ਨਕਾਬ;

    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type* ਅਚਲ type = ਕੁੰਜੀ->groups[ਸਮੂਹ].type;
    ਚੋਣ (ਬਹੁਲਕ) {
    ਹਾਲਤ XKB_CONSUMED_ਬਹੁਲਕ_XKB:
        consumed = type->mods.ਨਕਾਬ;
        ਰੋਕੋ;

    ਹਾਲਤ XKB_CONSUMED_ਬਹੁਲਕ_GTK: {
        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ* ਅਚਲ no_mods_ਇੰਦਰਾਜ =
            get_ਇੰਦਰਾਜ_for_mods(type, 0);
        ਅਚਲ xkb_level_ਸੂਚਕਾਂਕ_t no_mods_leveli = no_mods_ਇੰਦਰਾਜ
                                               ? no_mods_ਇੰਦਰਾਜ->level
                                               : 0;
        ਅਚਲ ਬਣਤਰ xkb_level* ਅਚਲ no_mods_level =
            &ਕੁੰਜੀ->groups[ਸਮੂਹ].ਪੱਧਰ[no_mods_leveli];

        ਲਈ (darray_ਮਾਪ_t i = 0; i < type->ਅੰਕ_entries; i++) {
            ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ* ਅਚਲ ਇੰਦਰਾਜ = &type->entries[i];
            ਜੇ (!ਇੰਦਰਾਜ_is_ਸਰਗਰਮ(ਇੰਦਰਾਜ))
                ਜਾਰੀ;

            ਅਚਲ ਬਣਤਰ xkb_level* ਅਚਲ level =
                &ਕੁੰਜੀ->groups[ਸਮੂਹ].ਪੱਧਰ[ਇੰਦਰਾਜ->level];
            ਜੇ (Xkb_ਪੱਧਰ_Same_Syms(level, no_mods_level))
                ਜਾਰੀ;

            ਜੇ (ਇੰਦਰਾਜ == matching_ਇੰਦਰਾਜ || one_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ(ਇੰਦਰਾਜ->mods.ਨਕਾਬ))
                consumed |= ਇੰਦਰਾਜ->mods.ਨਕਾਬ & ~ਇੰਦਰਾਜ->preserve.ਨਕਾਬ;
        }
        ਰੋਕੋ;
    }
    }

    ਵਾਪਸ consumed & ~preserve;
}

ਪੂਰਨ
xkb_state_mod_ਸੂਚਕਾਂਕ_is_consumed2(ਬਣਤਰ xkb_state *state, xkb_keycode_t kc,
                                 xkb_mod_ਸੂਚਕਾਂਕ_t idx,
                                 ਗਿਣਤੀ xkb_consumed_ਬਹੁਲਕ ਬਹੁਲਕ)
{
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ* ਅਚਲ ਕੁੰਜੀ = Xkb_ਕੁੰਜੀ(state->keymap, kc);

    ਜੇ (unlikely(!ਕੁੰਜੀ || idx >= xkb_keymap_ਅੰਕ_mods(state->keymap)))
        ਵਾਪਸ -1;

    ਅਚਲ xkb_mod_ਨਕਾਬ_t mapping = state->keymap->mods.mods[idx].mapping;
    ਜੇ (!mapping) {
        /* Modifier not mapped */
        ਵਾਪਸ 0;
    }
    ਵਾਪਸ (mapping & ਕੁੰਜੀ_get_consumed(state, ਕੁੰਜੀ, ਬਹੁਲਕ)) == mapping;
}

ਪੂਰਨ
xkb_state_mod_ਸੂਚਕਾਂਕ_is_consumed(ਬਣਤਰ xkb_state *state, xkb_keycode_t kc,
                                xkb_mod_ਸੂਚਕਾਂਕ_t idx)
{
    ਵਾਪਸ xkb_state_mod_ਸੂਚਕਾਂਕ_is_consumed2(state, kc, idx,
                                            XKB_CONSUMED_ਬਹੁਲਕ_XKB);
}

xkb_mod_ਨਕਾਬ_t
xkb_state_mod_ਨਕਾਬ_remove_consumed(ਬਣਤਰ xkb_state *state, xkb_keycode_t kc,
                                   xkb_mod_ਨਕਾਬ_t ਨਕਾਬ)
{
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ* ਅਚਲ ਕੁੰਜੀ = Xkb_ਕੁੰਜੀ(state->keymap, kc);

    ਜੇ (!ਕੁੰਜੀ)
        ਵਾਪਸ 0;

    ਵਾਪਸ resolve_ਮੰਜ਼ਿਲ_canonical_mods(state->keymap, ਨਕਾਬ) &
           ~ਕੁੰਜੀ_get_consumed(state, ਕੁੰਜੀ, XKB_CONSUMED_ਬਹੁਲਕ_XKB);
}

xkb_mod_ਨਕਾਬ_t
xkb_state_ਕੁੰਜੀ_get_consumed_mods2(ਬਣਤਰ xkb_state *state, xkb_keycode_t kc,
                                 ਗਿਣਤੀ xkb_consumed_ਬਹੁਲਕ ਬਹੁਲਕ)
{
    ਚੋਣ (ਬਹੁਲਕ) {
    ਹਾਲਤ XKB_CONSUMED_ਬਹੁਲਕ_XKB:
    ਹਾਲਤ XKB_CONSUMED_ਬਹੁਲਕ_GTK:
        ਰੋਕੋ;
    ਮੂਲ:
        log_err_func(state->keymap->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                     "unrecognized consumed modifiers mode: %d\n", ਬਹੁਲਕ);
        ਵਾਪਸ 0;
    }

    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ* ਅਚਲ ਕੁੰਜੀ = Xkb_ਕੁੰਜੀ(state->keymap, kc);
    ਜੇ (!ਕੁੰਜੀ)
        ਵਾਪਸ 0;

    ਵਾਪਸ ਕੁੰਜੀ_get_consumed(state, ਕੁੰਜੀ, ਬਹੁਲਕ);
}

xkb_mod_ਨਕਾਬ_t
xkb_state_ਕੁੰਜੀ_get_consumed_mods(ਬਣਤਰ xkb_state *state, xkb_keycode_t kc)
{
    ਵਾਪਸ xkb_state_ਕੁੰਜੀ_get_consumed_mods2(state, kc, XKB_CONSUMED_ਬਹੁਲਕ_XKB);
}

/*******************************************************************************
 * Server state machine API
 ******************************************************************************/

/** Entry to keep track of the state of an overlaid key */
ਬਣਤਰ xkb_overlaid_ਕੁੰਜੀ {
    /** The source key being overlaid */
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *old;
    /** The target key */
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *new;
    /** Counter of key currently down */
    ਪੂਰਨ refcnt;
};

ਬਣਤਰ xkb_shortcuts_config_ਇੰਦਰਾਜ {
    /** Real modifier mask to trigger shortcuts tweaks */
    xkb_mod_ਨਕਾਬ_t mods;
    /** Target layout */
    xkb_layout_ਸੂਚਕਾਂਕ_t ਨਿਸ਼ਾਨਾ;
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
ਬਣਤਰ xkb_machine {
    /** Inherits from `xkb_server_state` */
    ਬਣਤਰ xkb_server_state ਅਧਾਰ;

    /* Extra server-specific state data */

    /** Keyboard overlays handling */
    ਬਣਤਰ {
        /**
         * Current overlaid keys
         *
         * When a key is overlaid, it remained overlaid in this configuration
         * until totally released, i.e. every key press was matched with the
         * corresponding key release.
         */
        darray(ਬਣਤਰ xkb_overlaid_ਕੁੰਜੀ) ਕੁੰਜੀਆਂ;
        /**
         * Activation order of the overlay
         *
         * Overlays indices are stored 1-indexed in nibbles: the lowest
         * nibble corresponds to the latest activated index.
         */
        uint32_t ਕ੍ਰਮ;
        /** Current enabled overlays mask */
        xkb_overlay_ਨਕਾਬ_t enabled;
    } overlays;

    /** Configuration */
    ਬਣਤਰ machine_config {
        /** Modifiers remapping */
        ਬਣਤਰ machine_modifiers_config {
            /** Real modifier mask */
            xkb_mod_ਨਕਾਬ_t ਨਕਾਬ;
            /** Modifiers re-mappings */
            darray_ਮਾਪ_t mappings_ਅੰਕ;
            ਬਣਤਰ machine_mods_mapping {
                xkb_mod_ਨਕਾਬ_t ਸਰੋਤ;
                xkb_mod_ਨਕਾਬ_t ਨਿਸ਼ਾਨਾ;
            } * mappings ATTR_COUNTED_BY(mappings_ਅੰਕ);
        } modifiers;

        /** Shortcuts tweak */
        ਬਣਤਰ machine_shortcuts_config {
            /** Substitution entries */
            ਬਣਤਰ xkb_shortcuts_config_ਇੰਦਰਾਜ *entries;
        } shortcuts;
    } config;

    /** Mouse keys handling */
    ਬਣਤਰ {
        /** Default pointer button */
        xkb_ਸੂਚਕ_button_ਸੂਚਕਾਂਕ_t default_button;
        /** Mask of the locked buttons */
        xkb_ਸੂਚਕ_button_ਨਕਾਬ_t locked_buttons;
    } mouse;

    ਗਿਣਤੀ xkb_machine_flags flags;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ darray(ਬਣਤਰ machine_mods_mapping) machine_mods_mappings;

ਬਣਤਰ xkb_machine_builder {
    ਬਣਤਰ xkb_keymap *keymap;

    /** Controls */
    ਬਣਤਰ {
        /** Accessibility flags */
        ਬਣਤਰ {
            ਗਿਣਤੀ xkb_a11y_flags affect_flags;
            ਗਿਣਤੀ xkb_a11y_flags flags;
        } a11y;
    } controls;

    /** Modifiers re-mapping */
    machine_mods_mappings mods;

    /** Shortcuts tweak */
    ਬਣਤਰ xkb_shortcuts_config_options {
        /** Substitution entries */
        darray(ਬਣਤਰ xkb_shortcuts_config_ਇੰਦਰਾਜ) entries;
    } shortcuts;

    ਗਿਣਤੀ xkb_machine_flags machine_flags;
    ਗਿਣਤੀ xkb_machine_builder_flags builder_flags;
    ਪੂਰਨ refcnt;
};

ਬਣਤਰ xkb_machine_builder *
xkb_machine_builder_new(
    ਬਣਤਰ xkb_keymap * ਸੀਮਿਤ keymap,
    ਅਚਲ ਬਣਤਰ xkb_machine_builder_config * ਸੀਮਿਤ config,
    ਗਿਣਤੀ xkb_ਸਥਿਤੀ * ਸੀਮਿਤ ਸਥਿਤੀ
)
{
    /* Handle default configuration */
    ਸਥਿਰ ਅਚਲ ਬਣਤਰ xkb_machine_builder_config default_config = {
        .ਮਾਪ = ਆਕਾਰ(default_config)
    };
    ਜੇ (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ਅਚਲ ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ_2 = xkb_check_state_abi(config);
    ਜੇ (ਸਥਿਤੀ_2) {
        xkb_log_abi_error(keymap->ctx, __func__, ਸਥਿਤੀ_2);
        ਜੇ (ਸਥਿਤੀ)
            *ਸਥਿਤੀ = ਸਥਿਤੀ_2;
        ਵਾਪਸ ਨਿਰਾ;
    }

    /* Sanitize input */
    ਅਚਲ ਗਿਣਤੀ xkb_machine_builder_flags invalid_builder_flags
        = config->builder_flags
        & ~(ਗਿਣਤੀ xkb_machine_builder_flags)XKB_MACHINE_BUILDER_FLAGS_ਮੁੱਲਾਂ;
    ਜੇ (invalid_builder_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS_,
                     "unrecognized machine builder flags: 0x%x\n",
                     invalid_builder_flags);
        ਜੇ (ਸਥਿਤੀ)
            *ਸਥਿਤੀ = XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS;
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਅਚਲ ਗਿਣਤੀ xkb_machine_flags invalid_machine_flags
        = (ਗਿਣਤੀ xkb_machine_flags)config->machine_flags
        & ~(ਗਿਣਤੀ xkb_machine_flags)XKB_MACHINE_FLAGS_ਮੁੱਲਾਂ;
    ਜੇ (invalid_machine_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS_,
                     "unrecognized state machine flags: 0x%x\n",
                     invalid_machine_flags);
        ਜੇ (ਸਥਿਤੀ)
            *ਸਥਿਤੀ = XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS;
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਬਣਤਰ xkb_machine_builder * ਅਚਲ builder = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(*builder));
    ਜੇ (!builder) {
        log_err_func1(keymap->ctx, XKB_ERROR_ਵੰਡ_FAILURE_2,
                      "cannot allocate machine builder\n");
        ਜੇ (ਸਥਿਤੀ)
            *ਸਥਿਤੀ = XKB_ERROR_ਵੰਡ_FAILURE;
        ਵਾਪਸ ਨਿਰਾ;
    }

    *builder = (ਬਣਤਰ xkb_machine_builder) {
        .keymap = xkb_keymap_ref(keymap),
        .refcnt = 1,
        .builder_flags = (ਗਿਣਤੀ xkb_machine_builder_flags)config->builder_flags,
        .machine_flags = (ਗਿਣਤੀ xkb_machine_flags)config->machine_flags,
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

    ਜੇ (ਸਥਿਤੀ)
        *ਸਥਿਤੀ = XKB_SUCCESS;

    ਵਾਪਸ builder;
}

ਬਣਤਰ xkb_machine_builder *
xkb_machine_builder_ref(ਬਣਤਰ xkb_machine_builder *builder)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(builder->refcnt > 0);
    builder->refcnt++;
    ਵਾਪਸ builder;
}

ਖਾਲੀ
xkb_machine_builder_unref(ਬਣਤਰ xkb_machine_builder *builder)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(!builder || builder->refcnt > 0);
    ਜੇ (!builder || --builder->refcnt > 0)
        ਵਾਪਸ;

    darray_free(builder->shortcuts.entries);
    darray_free(builder->mods);
    xkb_keymap_unref(builder->keymap);
    ਮੁਕਤ(builder);
}

ਬਣਤਰ xkb_keymap *
xkb_machine_builder_get_keymap(ਅਚਲ ਬਣਤਰ xkb_machine_builder *builder)
{
    /* Reference count is not updated. See API doc. */
    ਵਾਪਸ builder->keymap;
}

ਗਿਣਤੀ xkb_ਸਥਿਤੀ
xkb_machine_builder_update_a11y(
    ਬਣਤਰ xkb_machine_builder * ਸੀਮਿਤ builder,
    ਅਚਲ ਬਣਤਰ xkb_machine_builder_a11y_update * ਸੀਮਿਤ update)
{
    /* Check ABI compatibility */
    ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ = xkb_check_state_abi(update);
    ਜੇ (ਸਥਿਤੀ != XKB_SUCCESS) {
        xkb_log_abi_error(builder->keymap->ctx, __func__, ਸਥਿਤੀ);
        ਵਾਪਸ ਸਥਿਤੀ;
    }

    ਅਚਲ ਗਿਣਤੀ xkb_a11y_flags invalid_flags =
        ~(ਗਿਣਤੀ xkb_a11y_flags)XKB_A11_Y_FLAGS_ਮੁੱਲਾਂ;

    ਜੇ (update->affect_flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y affected flags: %#x\n",
                __func__, update->affect_flags & invalid_flags);
        ਵਾਪਸ XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }
    ਜੇ (update->flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y flags: %#x\n",
                __func__, update->flags & invalid_flags);
        ਵਾਪਸ XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }

    builder->controls.a11y.affect_flags |= update->affect_flags;
    builder->controls.a11y.flags &= ~update->affect_flags;
    builder->controls.a11y.flags |= (update->flags & update->affect_flags);

    ਵਾਪਸ XKB_SUCCESS;
}

ਗਿਣਤੀ xkb_ਸਥਿਤੀ
xkb_machine_builder_update_mods_remap(
    ਬਣਤਰ xkb_machine_builder * ਸੀਮਿਤ builder,
    ਅਚਲ ਬਣਤਰ xkb_machine_builder_mods_remap_update * ਸੀਮਿਤ update
)
{
    ਜੇ (!update->ਸਰੋਤ) {
        ਜੇ (!update->ਨਿਸ਼ਾਨਾ) {
            /* Reset mappings */
            darray_resize(builder->mods, 0);
            ਵਾਪਸ XKB_SUCCESS;
        } ਨਹੀਂਤਾਂ {
            ਵਾਪਸ XKB_ERROR_UNSUPPORTED_MODIFIER_ਨਕਾਬ;
        }
    }

    /* Check the modifiers against the keymap */
    ਅਚਲ xkb_mod_ਨਕਾਬ_t invalid = ~builder->keymap->canonical_state_ਨਕਾਬ;
    ਜੇ ((update->ਸਰੋਤ & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ਨਕਾਬ,
                "%s: Invalid source modifiers: 0x%"PRIx32"\n",
                __func__, update->ਸਰੋਤ);
        ਵਾਪਸ XKB_ERROR_UNSUPPORTED_MODIFIER_ਨਕਾਬ;
    }
    ਜੇ ((update->ਨਿਸ਼ਾਨਾ & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ਨਕਾਬ,
                "%s: Invalid target modifiers: 0x%"PRIx32"\n",
                __func__, update->ਨਿਸ਼ਾਨਾ);
        ਵਾਪਸ XKB_ERROR_UNSUPPORTED_MODIFIER_ਨਕਾਬ;
    }

    ਬਣਤਰ machine_mods_mapping *mapping = ਨਿਰਾ;
    darray_ਮਾਪ_t m = 0;
    darray_enumerate(m, mapping, builder->mods) {
        ਜੇ (mapping->ਸਰੋਤ == update->ਸਰੋਤ) {
            ਜੇ (!update->ਨਿਸ਼ਾਨਾ) {
                /* Remove mapping */
                darray_remove(builder->mods, m);
            } ਨਹੀਂਤਾਂ {
                /* Update mapping */
                mapping->ਨਿਸ਼ਾਨਾ = update->ਨਿਸ਼ਾਨਾ;
            }
            ਵਾਪਸ XKB_SUCCESS;
        }
    }

    ਜੇ (update->ਨਿਸ਼ਾਨਾ) {
        /* Append new mapping */
        darray_append(
            builder->mods,
            (ਬਣਤਰ machine_mods_mapping) {
                .ਸਰੋਤ = update->ਸਰੋਤ,
                .ਨਿਸ਼ਾਨਾ = update->ਨਿਸ਼ਾਨਾ
            }
        );
    } ਨਹੀਂਤਾਂ {
        /* Ignore missing mapping */
    }

    ਵਾਪਸ XKB_SUCCESS;
}

ਗਿਣਤੀ xkb_ਸਥਿਤੀ
xkb_machine_builder_update_shortcut_override(
    ਬਣਤਰ xkb_machine_builder * ਸੀਮਿਤ builder,
    ਅਚਲ ਬਣਤਰ xkb_machine_builder_shortcut_override_update * ਸੀਮਿਤ update
)
{
    ਬਣਤਰ xkb_keymap *keymap = builder->keymap;

    /* Check the modifiers against the keymap */
    ਅਚਲ xkb_mod_ਨਕਾਬ_t invalid = ~keymap->canonical_state_ਨਕਾਬ;
    ਜੇ ((update->affect_mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ਨਕਾਬ,
                "%s: Invalid affected modifiers: 0x%"PRIx32"\n",
                __func__, update->affect_mods);
        ਵਾਪਸ XKB_ERROR_UNSUPPORTED_MODIFIER_ਨਕਾਬ;
    }
    ਜੇ ((update->mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ਨਕਾਬ,
                "%s: Invalid modifiers: 0x%"PRIx32"\n",
                __func__, update->mods);
        ਵਾਪਸ XKB_ERROR_UNSUPPORTED_MODIFIER_ਨਕਾਬ;
    }

    /* Check the layout indices against the keymap */
    ਜੇ (update->ਸਰੋਤ >= keymap->ਅੰਕ_groups &&
        update->ਸਰੋਤ != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ_2,
                "%s: Invalid source layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->ਅੰਕ_groups, update->ਸਰੋਤ + 1);
        ਵਾਪਸ XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ;
    }
    ਜੇ (update->ਨਿਸ਼ਾਨਾ >= keymap->ਅੰਕ_groups &&
        update->ਨਿਸ਼ਾਨਾ != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ_2,
                "%s: Invalid target layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->ਅੰਕ_groups, update->ਨਿਸ਼ਾਨਾ + 1);
        ਵਾਪਸ XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ;
    }

    ਬਣਤਰ xkb_shortcuts_config_options * ਅਚਲ config = &builder->shortcuts;

    /* Resize array & initialize new entries, if relevant */
    ਜੇ (update->ਸਰੋਤ >= darray_ਮਾਪ(config->entries) &&
        update->ਸਰੋਤ != XKB_LAYOUT_INVALID) {
        ਜੇ (update->ਨਿਸ਼ਾਨਾ == update->ਸਰੋਤ) {
            /* Skip default setting */
            ਵਾਪਸ XKB_SUCCESS;
        }
        xkb_layout_ਸੂਚਕਾਂਕ_t new = darray_ਮਾਪ(config->entries);
        darray_resize(config->entries, update->ਸਰੋਤ + 1);
        ਲਈ (; new <= update->ਸਰੋਤ; new++) {
            darray_ਵਸਤੂ(config->entries, new) =
                (ਬਣਤਰ xkb_shortcuts_config_ਇੰਦਰਾਜ) {
                    .ਨਿਸ਼ਾਨਾ = XKB_LAYOUT_INVALID,
                    .mods = 0
                };
        }
    }

    ਬਣਤਰ xkb_shortcuts_config_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;
    xkb_layout_ਸੂਚਕਾਂਕ_t ਸਰੋਤ = (update->ਸਰੋਤ == XKB_LAYOUT_INVALID)
        ? 0               /* Loop over all entries */
        : update->ਸਰੋਤ; /* Single entry */
    darray_foreach_from(ਇੰਦਰਾਜ, config->entries, ਸਰੋਤ) {
        ਜੇ (update->ਸਰੋਤ == XKB_LAYOUT_INVALID &&
            ਇੰਦਰਾਜ->ਨਿਸ਼ਾਨਾ == XKB_LAYOUT_INVALID) {
            /* Skip previous deactivated entry */
            ਸਰੋਤ++;
            ਜਾਰੀ;
        }

        ਜੇ (update->affect_mods) {
            ਇੰਦਰਾਜ->mods &= ~update->affect_mods;
            ਇੰਦਰਾਜ->mods |= (update->affect_mods & update->mods);
        }

        ਜੇ (update->ਸਰੋਤ != XKB_LAYOUT_INVALID) {
            ਇੰਦਰਾਜ->ਨਿਸ਼ਾਨਾ = (update->ਨਿਸ਼ਾਨਾ == ਸਰੋਤ)
                ? XKB_LAYOUT_INVALID
                : update->ਨਿਸ਼ਾਨਾ;
            /* Single entry: exit loop */
            ਰੋਕੋ;
        }
        ਸਰੋਤ++;
    }

    ਵਾਪਸ XKB_SUCCESS;
}

/** Compare 2 modifiers combinations */
ਸਥਿਰ ਪੂਰਨ
cmp_mod_masks(ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b)
{
    ਅਚਲ xkb_mod_ਨਕਾਬ_t m1 = *((xkb_mod_ਨਕਾਬ_t *) a);
    ਅਚਲ xkb_mod_ਨਕਾਬ_t m2 = *((xkb_mod_ਨਕਾਬ_t *) b);
    ਜੇ (m1 == m2)
        ਵਾਪਸ 0;

    ਜੇ (!m1)
        ਵਾਪਸ +1;
    ਜੇ (!m2)
        ਵਾਪਸ -1;

    ਅਚਲ xkb_mod_ਨਕਾਬ_t overlap = (m1 & m2);
    ਜੇ (overlap == m1)
        ਵਾਪਸ +1; /* m1 is a subset of m2 */
    ਜੇ (overlap == m2)
        ਵਾਪਸ -1; /* m2 is a subset of m1 */

    /* Compare active modifiers numbers */
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ count1 = popcount32(m1);
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ count2 = popcount32(m2);
    ਜੇ (count1 > count2)
        ਵਾਪਸ -1;
    ਜੇ (count1 < count2)
        ਵਾਪਸ +1;

    /* Give priority to real modifiers */
    ਵਾਪਸ (m1 < m2) ? -1 : +1;
}

ਸਥਿਰ ਗਿਣਤੀ xkb_ਸਥਿਤੀ
machine_ਸੈੱਟ_mods(ਬਣਤਰ xkb_machine *sm,
                 ਅਚਲ machine_mods_mappings *raw_mappings)
{
    ਜੇ (!darray_ਖਾਲੀ_ਥਾਵਾਂ(*raw_mappings)) {
        machine_mods_mappings mappings = darray_new();
        xkb_mod_ਨਕਾਬ_t ਨਕਾਬ = 0;
        ਅਚਲ ਬਣਤਰ machine_mods_mapping *mapping;
        ਅਚਲ xkb_mod_ਨਕਾਬ_t invalid =
            ~sm->ਅਧਾਰ.ਅਧਾਰ.keymap->canonical_state_ਨਕਾਬ;
        darray_foreach(mapping, *raw_mappings) {
            ਜੇ (!mapping->ਸਰੋਤ || !mapping->ਨਿਸ਼ਾਨਾ ||
                (mapping->ਸਰੋਤ & invalid) || (mapping->ਨਿਸ਼ਾਨਾ & invalid))
                ਜਾਰੀ;
            darray_append(mappings, *mapping);
            ਨਕਾਬ |= mapping->ਸਰੋਤ;
        }

        /*
         * Re-order the mappings, so that combinations have higher priority
         * if they are superset of other combinations.
         * For other type of overlaps, see the sorting function.
         */
        qsort(darray_ਚੀਜ਼ਾਂ(mappings), darray_ਮਾਪ(mappings),
              ਆਕਾਰ(*darray_ਚੀਜ਼ਾਂ(mappings)), &cmp_mod_masks);

        darray_steal(
            mappings,
            &sm->config.modifiers.mappings,
            &sm->config.modifiers.mappings_ਅੰਕ
        );
        sm->config.modifiers.ਨਕਾਬ = ਨਕਾਬ;
    } ਨਹੀਂਤਾਂ {
        sm->config.modifiers = (ਬਣਤਰ machine_modifiers_config) {
            .mappings = ਨਿਰਾ,
            .mappings_ਅੰਕ = 0,
            .ਨਕਾਬ = 0
        };
    }

    ਵਾਪਸ XKB_SUCCESS;
}

ਸਥਿਰ ਗਿਣਤੀ xkb_ਸਥਿਤੀ
machine_ਸੈੱਟ_shortcuts(
    ਬਣਤਰ xkb_machine * ਸੀਮਿਤ sm,
    ਅਚਲ ਬਣਤਰ xkb_shortcuts_config_options * ਸੀਮਿਤ options
)
{
    ਜੇ (darray_ਖਾਲੀ_ਥਾਵਾਂ(options->entries)) {
        sm->config.shortcuts = (ਬਣਤਰ machine_shortcuts_config) {
            .entries = ਨਿਰਾ
        };
        ਵਾਪਸ XKB_SUCCESS;
    }

    ਬਣਤਰ xkb_keymap * ਅਚਲ keymap = sm->ਅਧਾਰ.ਅਧਾਰ.keymap;

    ਬਣਤਰ xkb_shortcuts_config_ਇੰਦਰਾਜ * entries =
        ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(keymap->ਅੰਕ_groups, ਆਕਾਰ(*entries));
    ਜੇ (!entries)
        ਵਾਪਸ XKB_ERROR_ਵੰਡ_FAILURE;

    ਅਚਲ xkb_layout_ਸੂਚਕਾਂਕ_t ਗਿਣਤੀ_2 = MIN(
        keymap->ਅੰਕ_groups,
        (xkb_layout_ਸੂਚਕਾਂਕ_t) darray_ਮਾਪ(options->entries)
    );

    bool some_entries = false;
    ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t l = 0; l < ਗਿਣਤੀ_2; l++) {
        /* Sanitize layout target */
        ਅਚਲ xkb_layout_ਸੂਚਕਾਂਕ_t ਨਿਸ਼ਾਨਾ =
            darray_ਵਸਤੂ(options->entries, l).ਨਿਸ਼ਾਨਾ;
        /* Sanitize modifier mask */
        ਅਚਲ xkb_mod_ਨਕਾਬ_t mods = (
            darray_ਵਸਤੂ(options->entries, l).mods &
            sm->ਅਧਾਰ.ਅਧਾਰ.keymap->canonical_state_ਨਕਾਬ
        );
        ਜੇ (ਨਿਸ਼ਾਨਾ < keymap->ਅੰਕ_groups && ਨਿਸ਼ਾਨਾ != l && mods) {
            entries[l] = (ਬਣਤਰ xkb_shortcuts_config_ਇੰਦਰਾਜ) {
                .ਨਿਸ਼ਾਨਾ = ਨਿਸ਼ਾਨਾ,
                .mods = mods
            };
            some_entries = true;
        } ਨਹੀਂਤਾਂ {
            entries[l] = (ਬਣਤਰ xkb_shortcuts_config_ਇੰਦਰਾਜ) {
                .ਨਿਸ਼ਾਨਾ = XKB_LAYOUT_INVALID,
                .mods = 0
            };
            ਜਾਰੀ;
        }
    }

    ਜੇ (!some_entries) {
        ਮੁਕਤ(entries);
        entries = ਨਿਰਾ;
    } ਨਹੀਂਤਾਂ {
        /* Initialize groups with no entries */
        ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t l = ਗਿਣਤੀ_2; l < keymap->ਅੰਕ_groups; l++) {
            entries[l] = (ਬਣਤਰ xkb_shortcuts_config_ਇੰਦਰਾਜ) {
                .ਨਿਸ਼ਾਨਾ = XKB_LAYOUT_INVALID,
                .mods = 0
            };
        }
    }

    sm->config.shortcuts = (ਬਣਤਰ machine_shortcuts_config) {
        .entries = entries
    };
    ਵਾਪਸ XKB_SUCCESS;
}

ਬਣਤਰ xkb_machine *
xkb_machine_new(ਅਚਲ ਬਣਤਰ xkb_machine_builder * ਸੀਮਿਤ builder,
                ਗਿਣਤੀ xkb_ਸਥਿਤੀ * ਸੀਮਿਤ ਸਥਿਤੀ)
{
    ਬਣਤਰ xkb_machine * ਅਚਲ machine = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(*machine));
    ਜੇ (!machine) {
        log_err_func1(builder->keymap->ctx, XKB_ERROR_ਵੰਡ_FAILURE_2,
                      "cannot allocate machine\n");
        ਜੇ (ਸਥਿਤੀ)
            *ਸਥਿਤੀ = XKB_ERROR_ਵੰਡ_FAILURE;
        ਵਾਪਸ ਨਿਰਾ;
    }

    xkb_server_state_init(&machine->ਅਧਾਰ, builder->keymap, SERVER_STATE,
                          builder->controls.a11y.affect_flags,
                          builder->controls.a11y.flags);
    machine->flags = builder->machine_flags;
    ਜੇ (!(machine->flags &
          XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_ਕੁੰਜੀ_FIX))
    {
        machine->ਅਧਾਰ.flags |= STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_ਕੁੰਜੀ;
    }
    ਜੇ (machine->flags & XKB_MACHINE_UNLOCK_MODS_ON_PRESS) {
        machine->ਅਧਾਰ.flags |= STATE_UNLOCK_MODS_ON_PRESS;
    }

    ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ_2;
    ਜੇ ((ਸਥਿਤੀ_2 = machine_ਸੈੱਟ_mods(machine, &builder->mods)) != XKB_SUCCESS ||
        (ਸਥਿਤੀ_2 = machine_ਸੈੱਟ_shortcuts(machine, &builder->shortcuts)) !=
        XKB_SUCCESS) {
        *ਸਥਿਤੀ = ਸਥਿਤੀ_2;
        ਜਾਓ error;
    }

    darray_init(machine->overlays.ਕੁੰਜੀਆਂ);
    machine->mouse.default_button = XKB_ਸੂਚਕ_BUTTON_MIN;

    ਜੇ (ਸਥਿਤੀ)
        *ਸਥਿਤੀ = XKB_SUCCESS;

    ਵਾਪਸ machine;

error:
    xkb_machine_unref(machine);
    ਵਾਪਸ ਨਿਰਾ;
}

ਬਣਤਰ xkb_machine *
xkb_machine_ref(ਬਣਤਰ xkb_machine *sm)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(sm->ਅਧਾਰ.ਅਧਾਰ.refcnt > 0);
    sm->ਅਧਾਰ.ਅਧਾਰ.refcnt++;
    ਵਾਪਸ sm;
}

ਖਾਲੀ
xkb_machine_unref(ਬਣਤਰ xkb_machine *sm)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(!sm || sm->ਅਧਾਰ.ਅਧਾਰ.refcnt > 0);
    ਜੇ (!sm || --sm->ਅਧਾਰ.ਅਧਾਰ.refcnt > 0)
        ਵਾਪਸ;

    xkb_state_destroy(&sm->ਅਧਾਰ.ਅਧਾਰ);
    darray_free(sm->overlays.ਕੁੰਜੀਆਂ);
    ਮੁਕਤ(sm->config.shortcuts.entries);
    ਮੁਕਤ(sm->config.modifiers.mappings);
    ਮੁਕਤ(sm);
}

ਬਣਤਰ xkb_keymap *
xkb_machine_get_keymap(ਅਚਲ ਬਣਤਰ xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    ਵਾਪਸ sm->ਅਧਾਰ.ਅਧਾਰ.keymap;
}

ਬਣਤਰ xkb_state *
xkb_machine_get_state(ਬਣਤਰ xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    ਵਾਪਸ (ਬਣਤਰ xkb_state *)sm;
}

ਬਣਤਰ xkb_state *
xkb_state_new_from_machine(ਅਚਲ ਬਣਤਰ xkb_machine * ਸੀਮਿਤ machine,
                           ਗਿਣਤੀ xkb_ਸਥਿਤੀ * ਸੀਮਿਤ ਸਥਿਤੀ)
{
    /* Creation */
    ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ_2;
    ਬਣਤਰ xkb_state * ਅਚਲ state = xkb_state_new_with_ਬਹੁਲਕ(
        machine->ਅਧਾਰ.ਅਧਾਰ.keymap,
        XKB_STATE_ਬਹੁਲਕ_SERVER_QUERY,
        &ਸਥਿਤੀ_2
    );
    ਜੇ (ਸਥਿਤੀ_2 != XKB_SUCCESS) {
        ਜੇ (ਸਥਿਤੀ)
            *ਸਥਿਤੀ = ਸਥਿਤੀ_2;
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਪ੍ਰਤਿਜ੍ਞਾ(state->ਬਹੁਲਕ == SERVER_COMPANION);
    ਪ੍ਰਤਿਜ੍ਞਾ(state->refcnt == 1);

    /* Copy machine’s state */
    *state = machine->ਅਧਾਰ.ਅਧਾਰ;

    /* Restore previous values */
    state->ਬਹੁਲਕ = SERVER_COMPANION;
    state->refcnt = 1;

    ਜੇ (ਸਥਿਤੀ)
        *ਸਥਿਤੀ = XKB_SUCCESS;
    ਵਾਪਸ state;
}

ਸਥਿਰ ਖਾਲੀ
machine_update_overlays(ਬਣਤਰ xkb_machine *sm)
{
    /*
     * Overlays indices are stored 1-indexed in nibbles: the lowest
     * nibble corresponds to the latest activated index.
     */

    ਅਚਲ xkb_overlay_ਨਕਾਬ_t ਨਕਾਬ =
        overlays_from_controls(sm->ਅਧਾਰ.ਅਧਾਰ.components.controls);
    xkb_overlay_ਨਕਾਬ_t added = ਨਕਾਬ & ~sm->overlays.enabled;

    /* Remove overlays no longer enabled and keep relative order */
    uint32_t ਕ੍ਰਮ = sm->overlays.ਕ੍ਰਮ;
    ਅਚਲ xkb_overlay_ਸੂਚਕਾਂਕ_t overlay_max =
        format_max_overlays(sm->ਅਧਾਰ.ਅਧਾਰ.keymap->format);
    ਲਈ (uint8_t n = 0; n < overlay_max;) {
        xkb_overlay_ਸੂਚਕਾਂਕ_t overlay_idx = (ਕ੍ਰਮ >> (n * 4)) & 0xf;
        ਜੇ (!overlay_idx)
            ਰੋਕੋ;
        ਅਚਲ xkb_overlay_ਨਕਾਬ_t overlay_ਨਕਾਬ =
            (xkb_overlay_ਨਕਾਬ_t)(1u << (overlay_idx - 1u) /* k is 1-indexed */);
        ਜੇ (overlay_ਨਕਾਬ & ਨਕਾਬ) {
            /* no duplicates */
            added &= ~overlay_ਨਕਾਬ;
            n++;
        } ਨਹੀਂਤਾਂ {
            /* remove overlay */
            ਅਚਲ uint32_t ਸਿਰ = (UINT32_C(1) << (n * 4)) - 1;
            ਕ੍ਰਮ = (ਕ੍ਰਮ & ਸਿਰ) | ((ਕ੍ਰਮ >> 4) & ~ਸਿਰ);
        }
    }

    /* Add new overlays in ascending order */
    ਲਈ (xkb_overlay_ਸੂਚਕਾਂਕ_t k = 0; added; k++, added >>= 1) {
        ਜੇ (added & 0x1) {
            ਕ੍ਰਮ <<= 4;
            ਕ੍ਰਮ |= k + 1u; /* k is 0-indexed */
        }
    }

    sm->overlays.ਕ੍ਰਮ = ਕ੍ਰਮ;
    sm->overlays.enabled = ਨਕਾਬ;
}

ਗਿਣਤੀ xkb_ਸਥਿਤੀ
xkb_machine_process_synthetic(ਬਣਤਰ xkb_machine *sm,
                              ਅਚਲ ਬਣਤਰ xkb_synthetic_update *update,
                              ਬਣਤਰ xkb_events *events)
{
    /* Check ABI compatibility */
    ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ =
        check_state_update_abi(sm->ਅਧਾਰ.ਅਧਾਰ.keymap->ctx, update);
    ਜੇ (ਸਥਿਤੀ != XKB_SUCCESS)
        ਵਾਪਸ ਸਥਿਤੀ;

    ਬਣਤਰ xkb_server_state * ਅਚਲ state = &sm->ਅਧਾਰ;
    ਅਚਲ ਬਣਤਰ state_components ਪਿਛਲਾ_components = state->ਅਧਾਰ.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    darray_ਮਾਪ(events->ਕਤਾਰ) = 0;
    events->ਅਗਲਾ = 0;

    /* Update parametrized controls first */
    ਜੇ (update->layout_policy) {
        ਸਥਿਤੀ = state_update_layout_policy(state, update->layout_policy);
        ਜੇ (ਸਥਿਤੀ != XKB_SUCCESS)
            ਵਾਪਸ ਸਥਿਤੀ;
    }

    ਜੇ (update->components) {
        ਅਚਲ ਬਣਤਰ xkb_state_components_update * ਅਚਲ components =
            update->components;
        /* Update boolean controls first */
        ਜੇ (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, events);
        }

        state_update_latched_locked(state, components, events);
    }

    xkb_state_update_derived(&state->ਅਧਾਰ);

    ਅਚਲ ਗਿਣਤੀ xkb_state_component ਬਦਲਿਆ = get_state_component_changes(
        &ਪਿਛਲਾ_components, &state->ਅਧਾਰ.components
    );
    ਜੇ (ਬਦਲਿਆ) {
        // TODO: latch controls
        ਜੇ (ਬਦਲਿਆ & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        /* Create event only if some component actually changed */
        darray_append(events->ਕਤਾਰ, (ਬਣਤਰ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .ਬਦਲਿਆ = ਬਦਲਿਆ,
                .components = state->ਅਧਾਰ.components
            }
        });

        /* Terminate the frame */
        darray_append(events->ਕਤਾਰ, (ਬਣਤਰ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_ਚੌਖਟਾ,
        });
    }

    ਵਾਪਸ XKB_SUCCESS;
}

ਸਥਿਰ ssize_t
do_remap_modifiers(ਅਚਲ ਬਣਤਰ machine_modifiers_config * ਸੀਮਿਤ mappings,
                   ਬਣਤਰ xkb_state * ਸੀਮਿਤ state,
                   ਬਣਤਰ xkb_events * ਸੀਮਿਤ events,
                   ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ * ਸੀਮਿਤ ਕੁੰਜੀ)
{
    ਜੇ (!(mappings->ਨਕਾਬ & state->components.mods))
        ਵਾਪਸ -1;

    ਅਚਲ xkb_layout_ਸੂਚਕਾਂਕ_t layout = state_ਕੁੰਜੀ_get_layout(state, ਕੁੰਜੀ);
    ਜੇ (layout >= ਕੁੰਜੀ->ਅੰਕ_groups)
        ਵਾਪਸ -1;

    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type * ਅਚਲ type = ਕੁੰਜੀ->groups[layout].type;
    xkb_mod_ਨਕਾਬ_t affect_mods = 0;
    xkb_mod_ਨਕਾਬ_t mods = 0;
    ਲਈ (darray_ਮਾਪ_t m = 0; m < mappings->mappings_ਅੰਕ; m++) {
        ਅਚਲ ਬਣਤਰ machine_mods_mapping * ਅਚਲ mapping =
            &mappings->mappings[m];
        ਜੇ (/* Skip not matching active mods */
            (mapping->ਸਰੋਤ & state->components.mods) == mapping->ਸਰੋਤ &&
            /* Skip overlap with previous active mappings */
            !(mapping->ਸਰੋਤ & affect_mods) &&
            /* Skip if the key type uses some of the source modifiers */
            !(mapping->ਸਰੋਤ & type->mods.ਨਕਾਬ)) {
            affect_mods |= mappings->mappings[m].ਸਰੋਤ;
            mods |= mappings->mappings[m].ਨਿਸ਼ਾਨਾ;
        }
    }

    ਜੇ (!affect_mods)
        ਵਾਪਸ -1;

    ਬਣਤਰ xkb_state new = *state;
    new.components.ਅਧਾਰ_mods = (new.components.ਅਧਾਰ_mods & ~affect_mods) | mods;
    new.components.latched_mods = (new.components.latched_mods & ~affect_mods);
    new.components.locked_mods = (new.components.locked_mods & ~affect_mods);
    xkb_state_update_derived(&new);

    ssize_t event_idx = -1;
    ਅਚਲ ਗਿਣਤੀ xkb_state_component ਬਦਲਿਆ =
        get_state_component_changes(&state->components, &new.components);
    ਜੇ (ਬਦਲਿਆ) {
        event_idx = (ssize_t) darray_ਮਾਪ(events->ਕਤਾਰ);
        darray_append(events->ਕਤਾਰ, (ਬਣਤਰ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = new.components,
                .ਬਦਲਿਆ = ਬਦਲਿਆ
            }
        });
    }

    state->components.mods = new.components.mods;
    ਵਾਪਸ event_idx;
}

/**
 * Activate an alternative layout if some modifiers are active, typically
 * `Control`, `Alt` or `Super`.
 *
 * The tweak consists of computing a *base* layout such that the *effective*
 * layout is the corresponding target layout.
 */
ਸਥਿਰ ssize_t
do_shortcuts_tweak(ਅਚਲ ਬਣਤਰ machine_shortcuts_config *config,
                   ਬਣਤਰ xkb_state *state,
                   ਅਚਲ ਬਣਤਰ state_components *ਪਿਛਲਾ_components,
                   ਬਣਤਰ xkb_events *events, ssize_t remap_event)
{
    ਜੇ (!config->entries) {
        /* No shortcuts tweak */
        ਵਾਪਸ remap_event;
    }

    ਅਚਲ ਬਣਤਰ xkb_shortcuts_config_ਇੰਦਰਾਜ * ਅਚਲ ਇੰਦਰਾਜ =
        &config->entries[state->components.ਸਮੂਹ];

    ਜੇ (ਇੰਦਰਾਜ->ਨਿਸ਼ਾਨਾ != XKB_LAYOUT_INVALID &&
        (ਇੰਦਰਾਜ->mods & state->components.mods)) {
        /*
         * Activate shortcuts tweak:
         * 1. The real base group is saved by the caller, to be restored by
         *    later by a call to undo_tweaks().
         * 2. Compute the new base group so that the resulting effective group
         *    is the target.
         * 3. Recompute derived state components.
         */
        ਬਣਤਰ xkb_state new = *state;
        ਜੇ (remap_event < 0) {
            /* Create new event */
            remap_event = (ssize_t) darray_ਮਾਪ(events->ਕਤਾਰ);
            darray_append(events->ਕਤਾਰ, (ਬਣਤਰ xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = {0},
                    .ਬਦਲਿਆ = 0
                }
            });
        } ਨਹੀਂਤਾਂ {
            /* Merge with last remap event */
            new.components =
                darray_ਵਸਤੂ(events->ਕਤਾਰ, remap_event).components.components;
        }
        new.components.ਅਧਾਰ_ਸਮੂਹ
            = (int32_t) ਇੰਦਰਾਜ->ਨਿਸ਼ਾਨਾ
            - state->components.latched_ਸਮੂਹ
            - state->components.locked_ਸਮੂਹ;
        xkb_state_update_derived(&new);

        ਅਚਲ ਗਿਣਤੀ xkb_state_component ਬਦਲਿਆ =
            get_state_component_changes(ਪਿਛਲਾ_components, &new.components);
        darray_ਵਸਤੂ(events->ਕਤਾਰ, remap_event).components.ਬਦਲਿਆ = ਬਦਲਿਆ;
        darray_ਵਸਤੂ(events->ਕਤਾਰ, remap_event).components.components =
            new.components;

        state->components.ਸਮੂਹ = new.components.ਸਮੂਹ;
    } ਨਹੀਂਤਾਂ {
        /* Deactivated entry or modifiers do not match */
    }
    ਵਾਪਸ remap_event;
}

ਸਥਿਰ ਖਾਲੀ
undo_tweaks(ਅਚਲ ਬਣਤਰ xkb_state *state,
            ਅਚਲ ਬਣਤਰ state_components *initial_components,
            ਬਣਤਰ xkb_events *events)
{
    /* Get last component event */
    ਅਚਲ ਬਣਤਰ xkb_event *event = ਨਿਰਾ;
    darray_foreach_ਉਲਟ(event, events->ਕਤਾਰ) {
        ਜੇ (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS)
            ਰੋਕੋ;
    }
    ਜੇ (!event)
        ਵਾਪਸ;

    /* Restore state */
    ਅਚਲ ਗਿਣਤੀ xkb_state_component ਬਦਲਿਆ =
        get_state_component_changes(initial_components,
                                    &event->components.components);
    ਜੇ (ਬਦਲਿਆ) {
        darray_append(events->ਕਤਾਰ, (ਬਣਤਰ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = *initial_components,
                .ਬਦਲਿਆ = ਬਦਲਿਆ
            }
        });
    }
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *
process_overlayable_ਕੁੰਜੀ(ਬਣਤਰ xkb_machine *sm,
                        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ * ਕੁੰਜੀ,
                        ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction)
{
    /* Check if key is currently overlaid */
    ਬਣਤਰ xkb_overlaid_ਕੁੰਜੀ *ਇੰਦਰਾਜ = ਨਿਰਾ;
    ਬਣਤਰ xkb_overlaid_ਕੁੰਜੀ *ਉਪਲਬਧ_ਇੰਦਰਾਜ = ਨਿਰਾ;
    darray_foreach(ਇੰਦਰਾਜ, sm->overlays.ਕੁੰਜੀਆਂ) {
        ਜੇ (ਇੰਦਰਾਜ->old == ਕੁੰਜੀ) {
            /* Maintain the key overlaid */
            ਚੋਣ (direction) {
            ਹਾਲਤ XKB_ਕੁੰਜੀ_ਥੱਲੇ:
                ਇੰਦਰਾਜ->refcnt++;
                ਰੋਕੋ;
            ਹਾਲਤ XKB_ਕੁੰਜੀ_REPEATED:
                ਰੋਕੋ;
            ਹਾਲਤ XKB_ਕੁੰਜੀ_ਉੱਪਰ:
            ਮੂਲ:
                ਇੰਦਰਾਜ->refcnt--;
                ਰੋਕੋ;
            }
            ਜੇ (ਇੰਦਰਾਜ->refcnt <= 0)
                ਇੰਦਰਾਜ->old = ਨਿਰਾ;
            ਵਾਪਸ ਇੰਦਰਾਜ->new;
        } ਨਹੀਂਤਾਂ ਜੇ (!ਇੰਦਰਾਜ->old && !ਉਪਲਬਧ_ਇੰਦਰਾਜ) {
            /* First free spot */
            ਉਪਲਬਧ_ਇੰਦਰਾਜ = ਇੰਦਰਾਜ;
        }
    }
    ਜੇ (direction == XKB_ਕੁੰਜੀ_ਥੱਲੇ) {
        /* Enable key overlay */

        /*
         * Get overlay index: select the latest activated overlay
         * containing the key.
         */
        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *new = ਕੁੰਜੀ;

        ਜੇ (ਕੁੰਜੀ->overlays & sm->overlays.enabled) {
            /* Some relevant overlay is active */
            ਲਈ (uint32_t ਢੇਰ = sm->overlays.ਕ੍ਰਮ; ਢੇਰ; ਢੇਰ >>= 4) {
                static_assert(XKB_OVERLAY_ਗਿਣਤੀ == 8, "");
                ਅਚਲ xkb_overlay_ਸੂਚਕਾਂਕ_t overlay = (ਢੇਰ & 0xf) - 1;
                ਅਚਲ xkb_overlay_ਨਕਾਬ_t ਨਕਾਬ =
                    (xkb_overlay_ਨਕਾਬ_t)(1u << overlay);
                ਜੇ (ਕੁੰਜੀ->overlays & ਨਕਾਬ) {
                    /* Get new key */
                    ਜੇ (ਕੁੰਜੀ->overlays_inline) {
                        /* Stored inline */
                        new = ਕੁੰਜੀ->overlay_ਕੁੰਜੀ;
                    } ਨਹੀਂਤਾਂ {
                        /* Stored in sparse array */
                        ਅਚਲ xkb_overlay_ਨਕਾਬ_t ਨੀਵਾਂ = (xkb_overlay_ਨਕਾਬ_t)(
                            ਕੁੰਜੀ->overlays & (xkb_overlay_ਨਕਾਬ_t)(ਨਕਾਬ - 1u)
                        );
                        /* index = number of set bits strictly below bit */
                        ਅਚਲ xkb_overlay_ਸੂਚਕਾਂਕ_t ਸੂਚਕਾਂਕ =
                            (xkb_overlay_ਸੂਚਕਾਂਕ_t)popcount32(ਨੀਵਾਂ);
                        new = ਕੁੰਜੀ->overlays_ਕੁੰਜੀਆਂ[ਸੂਚਕਾਂਕ];
                    }
                    ਰੋਕੋ;
                }
            }
        } ਨਹੀਂਤਾਂ {
            /*
             * No relevant overlay is active: block future overlays while
             * the key is pressed.
             */
        }

        /* Get new entry */
        ਜੇ (ਉਪਲਬਧ_ਇੰਦਰਾਜ) {
            /* Re-use a free entry */
            ਇੰਦਰਾਜ = ਉਪਲਬਧ_ਇੰਦਰਾਜ;
        } ਨਹੀਂਤਾਂ {
            /* Append a new entry */
            ਅਚਲ darray_ਮਾਪ_t idx = darray_ਮਾਪ(sm->overlays.ਕੁੰਜੀਆਂ);
            darray_resize(sm->overlays.ਕੁੰਜੀਆਂ, idx + 1);
            ਇੰਦਰਾਜ = &darray_ਵਸਤੂ(sm->overlays.ਕੁੰਜੀਆਂ, idx);
        }

        /* Overlay the key */
        ਇੰਦਰਾਜ->old = ਕੁੰਜੀ;
        ਇੰਦਰਾਜ->new = new;
        ਇੰਦਰਾਜ->refcnt = 1;
        ਵਾਪਸ new;
    }

    /* No overlay */
    ਵਾਪਸ ਕੁੰਜੀ;
}

ਗਿਣਤੀ xkb_ਸਥਿਤੀ
xkb_machine_process_ਕੁੰਜੀ(ਬਣਤਰ xkb_machine *sm,
                        xkb_keycode_t kc, ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction,
                        ਬਣਤਰ xkb_events *events)
{
    darray_ਮਾਪ(events->ਕਤਾਰ) = 0;
    events->ਅਗਲਾ = 0;

    ਬਣਤਰ xkb_server_state * ਅਚਲ state = &sm->ਅਧਾਰ;
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ * ਕੁੰਜੀ = Xkb_ਕੁੰਜੀ(state->ਅਧਾਰ.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    ਜੇ (!ਕੁੰਜੀ || (direction == XKB_ਕੁੰਜੀ_REPEATED && !ਕੁੰਜੀ->repeats))
        ਵਾਪਸ XKB_SUCCESS;

    ਅਚਲ ਬਣਤਰ state_components initial_components = state->ਅਧਾਰ.components;

    ਜੇ (direction == XKB_ਕੁੰਜੀ_ਥੱਲੇ &&
        (sm->ਅਧਾਰ.ਅਧਾਰ.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ਕੁੰਜੀਆਂ) &&
        (sm->ਅਧਾਰ.a11y_flags & XKB_A11_Y_STICKY_ਕੁੰਜੀਆਂ_NO_ਇੱਕੋ_ਵੇਲੇ_ਕੁੰਜੀਆਂ) &&
        state->ਅਧਾਰ.components.ਅਧਾਰ_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ਕੁੰਜੀਆਂ,
                                      0, events);
    }

    ਜੇ (ਕੁੰਜੀ->overlays)
        ਕੁੰਜੀ = process_overlayable_ਕੁੰਜੀ(sm, ਕੁੰਜੀ, direction);

    ssize_t remap_event = do_remap_modifiers(&sm->config.modifiers,
                                             &state->ਅਧਾਰ, events, ਕੁੰਜੀ);

    remap_event = do_shortcuts_tweak(&sm->config.shortcuts, &state->ਅਧਾਰ,
                                     &initial_components, events, remap_event);

    ਜੇ (remap_event >= 0) {
        // TODO: what about merging state event for RedirectKey?
        darray_append(events->ਕਤਾਰ, (ਬਣਤਰ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_ਚੌਖਟਾ,
        });
    }

    state->flags |= STATE_REQUIRE_ਕੁੰਜੀ_EVENT;
    state->ਸੈੱਟ_mods = 0;
    state->clear_mods = 0;

    /* Handle key actions */
    xkb_filter_apply_all(state, events, ਕੁੰਜੀ, direction);

    xkb_mod_ਸੂਚਕਾਂਕ_t i;
    xkb_mod_ਨਕਾਬ_t ਦੋਆਧਾਰੀ_ਅੰਕ;
    ਲਈ (i = 0, ਦੋਆਧਾਰੀ_ਅੰਕ = 1; state->ਸੈੱਟ_mods; i++, ਦੋਆਧਾਰੀ_ਅੰਕ <<= 1) {
        ਜੇ (state->ਸੈੱਟ_mods & ਦੋਆਧਾਰੀ_ਅੰਕ) {
            state->mod_ਕੁੰਜੀ_ਗਿਣਤੀ[i]++;
            state->ਅਧਾਰ.components.ਅਧਾਰ_mods |= ਦੋਆਧਾਰੀ_ਅੰਕ;
            state->ਸੈੱਟ_mods &= ~ਦੋਆਧਾਰੀ_ਅੰਕ;
        }
    }

    ਲਈ (i = 0, ਦੋਆਧਾਰੀ_ਅੰਕ = 1; state->clear_mods; i++, ਦੋਆਧਾਰੀ_ਅੰਕ <<= 1) {
        ਜੇ (state->clear_mods & ਦੋਆਧਾਰੀ_ਅੰਕ) {
            state->mod_ਕੁੰਜੀ_ਗਿਣਤੀ[i]--;
            ਜੇ (state->mod_ਕੁੰਜੀ_ਗਿਣਤੀ[i] <= 0) {
                state->ਅਧਾਰ.components.ਅਧਾਰ_mods &= ~ਦੋਆਧਾਰੀ_ਅੰਕ;
                state->mod_ਕੁੰਜੀ_ਗਿਣਤੀ[i] = 0;
            }
            state->clear_mods &= ~ਦੋਆਧਾਰੀ_ਅੰਕ;
        }
    }

    xkb_state_update_derived(&state->ਅਧਾਰ);

    /*
     * Insert key event only if actions did not override it, e.g.:
     * - RedirectKey() append its own key event;
     * - MovePointer() and PointerButton() generate pointer events instead
     *   of key events;
     * - TerminateServer() generate a Server event instead of key events.
     */
    ਜੇ (state->flags & STATE_REQUIRE_ਕੁੰਜੀ_EVENT) {
        darray_append(events->ਕਤਾਰ, (ਬਣਤਰ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_ਕੁੰਜੀ,
            .ਕੁੰਜੀ = {
                .keycode = ਕੁੰਜੀ->keycode,
                .direction = direction
            }
        });
    }

    ਜੇ (remap_event >= 0) {
        // FIXME: fragile if last state change does not restore the state to the remap event
        undo_tweaks(&state->ਅਧਾਰ, &initial_components, events);
    }

    ਅਚਲ ਗਿਣਤੀ xkb_state_component ਬਦਲਿਆ = get_state_component_changes(
        &initial_components, &state->ਅਧਾਰ.components
    );
    ਜੇ (ਬਦਲਿਆ) {
        ਜੇ (ਬਦਲਿਆ & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        // TODO: merge with previous state event, if any

        darray_append(events->ਕਤਾਰ, (ਬਣਤਰ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = state->ਅਧਾਰ.components,
                .ਬਦਲਿਆ = ਬਦਲਿਆ
            }
        });
    }

    ਜੇ (!darray_ਖਾਲੀ_ਥਾਵਾਂ(events->ਕਤਾਰ) &&
        darray_ਵਸਤੂ(events->ਕਤਾਰ, darray_ਮਾਪ(events->ਕਤਾਰ) - 1).type !=
        XKB_EVENT_TYPE_ਚੌਖਟਾ)
    {
        darray_append(events->ਕਤਾਰ, (ਬਣਤਰ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_ਚੌਖਟਾ,
        });
    }

    ਵਾਪਸ XKB_SUCCESS;
}

ਬਣਤਰ xkb_events *
xkb_events_new(ਬਣਤਰ xkb_context * ਸੀਮਿਤ context,
               ਅਚਲ ਬਣਤਰ xkb_events_config * ਸੀਮਿਤ config,
               ਗਿਣਤੀ xkb_ਸਥਿਤੀ * ਸੀਮਿਤ ਸਥਿਤੀ)
{
    /* Handle default configuration */
    ਸਥਿਰ ਅਚਲ ਬਣਤਰ xkb_events_config default_config = {
        .ਮਾਪ = ਆਕਾਰ(default_config)
    };
    ਜੇ (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ਅਚਲ ਗਿਣਤੀ xkb_ਸਥਿਤੀ abi_error = xkb_check_state_abi(config);
    ਜੇ (abi_error) {
        xkb_log_abi_error(context, __func__, abi_error);
        ਜੇ (ਸਥਿਤੀ)
            *ਸਥਿਤੀ = abi_error;
        ਵਾਪਸ ਨਿਰਾ;
    }

    /* Sanitize input */
    ਅਚਲ uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_EVENTS_FLAGS_ਮੁੱਲਾਂ);
    ਜੇ (invalid_flags) {
        log_err_func(context, XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS_,
                     "unrecognized events collection flags: 0x%"PRIx32"\n",
                     invalid_flags);
        ਜੇ (ਸਥਿਤੀ)
            *ਸਥਿਤੀ = XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS;
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਬਣਤਰ xkb_events *events = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(*events));
    ਜੇ (!events) {
        log_err_func1(context, XKB_ERROR_ਵੰਡ_FAILURE_2,
                      "cannot allocate state events collection\n");
        ਜੇ (ਸਥਿਤੀ)
            *ਸਥਿਤੀ = XKB_ERROR_ਵੰਡ_FAILURE;
        ਵਾਪਸ ਨਿਰਾ;
    }

    /* Pre-allocate event queue */
    darray_init(events->ਕਤਾਰ);
    ਸਥਿਰ ਅਚਲ darray_ਮਾਪ_t ਕਤਾਰ_initial_ਮਾਪ = 16;
    darray_resize(events->ਕਤਾਰ, ਕਤਾਰ_initial_ਮਾਪ);
    // TODO: proper safe darray_resize() with return value
    ਜੇ (!darray_ਚੀਜ਼ਾਂ(events->ਕਤਾਰ)) {
        log_err_func1(context, XKB_ERROR_ਵੰਡ_FAILURE_2,
                      "cannot allocate state events queue\n");
        xkb_events_destroy(events);
        ਜੇ (ਸਥਿਤੀ)
            *ਸਥਿਤੀ = XKB_ERROR_ਵੰਡ_FAILURE;
        ਵਾਪਸ ਨਿਰਾ;
    }
    darray_ਮਾਪ(events->ਕਤਾਰ) = 0;

    events->ctx = xkb_context_ref(context);
    events->ਅਗਲਾ = 0;

    ਜੇ (ਸਥਿਤੀ)
        *ਸਥਿਤੀ = XKB_SUCCESS;
    ਵਾਪਸ events;
}

ਖਾਲੀ
xkb_events_destroy(ਬਣਤਰ xkb_events *events)
{
    ਜੇ (!events)
        ਵਾਪਸ;
    darray_free(events->ਕਤਾਰ);
    xkb_context_unref(events->ctx);
    ਮੁਕਤ(events);
}

ਅਚਲ ਬਣਤਰ xkb_event *
xkb_events_ਅਗਲਾ(ਬਣਤਰ xkb_events *events)
{
    ਜੇ (events->ਅਗਲਾ < darray_ਮਾਪ(events->ਕਤਾਰ)) {
        ਅਚਲ darray_ਮਾਪ_t ਸੂਚਕਾਂਕ = events->ਅਗਲਾ++;
        ਵਾਪਸ &darray_ਵਸਤੂ(events->ਕਤਾਰ, ਸੂਚਕਾਂਕ);
    } ਨਹੀਂਤਾਂ {
        ਵਾਪਸ ਨਿਰਾ;
    }
}

ਗਿਣਤੀ xkb_event_type
xkb_event_get_type(ਅਚਲ ਬਣਤਰ xkb_event *event)
{
    ਵਾਪਸ event->type;
}

ਗਿਣਤੀ xkb_ਸਥਿਤੀ
xkb_event_get_keycode(ਅਚਲ ਬਣਤਰ xkb_event *event,
                      xkb_keycode_t *keycode,
                      ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction *direction)
{
    ਜੇ (event->type == XKB_EVENT_TYPE_ਕੁੰਜੀ) {
        *keycode = event->ਕੁੰਜੀ.keycode;
        *direction = event->ਕੁੰਜੀ.direction;
        ਵਾਪਸ XKB_SUCCESS;
    } ਨਹੀਂਤਾਂ {
        ਵਾਪਸ XKB_ERROR_EVENT_TYPE_MISMATCH;
    }
}

ਗਿਣਤੀ xkb_ਸਥਿਤੀ
xkb_event_get_components(ਅਚਲ ਬਣਤਰ xkb_event * ਸੀਮਿਤ event,
                         ਬਣਤਰ xkb_event_components * ਸੀਮਿਤ components)
{
    ਜੇ (event->type != XKB_EVENT_TYPE_STATE_COMPONENTS)
        ਵਾਪਸ XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ਅਚਲ ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ = xkb_check_state_abi(components);
    ਜੇ (ਸਥਿਤੀ != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, ਸਥਿਤੀ);
        ਵਾਪਸ ਸਥਿਤੀ;
    }

    components->ਬਦਲਿਆ = (uint32_t)event->components.ਬਦਲਿਆ;
    components->depressed_mods = event->components.components.ਅਧਾਰ_mods;
    components->latched_mods = event->components.components.latched_mods;
    components->locked_mods = event->components.components.locked_mods;
    components->mods = event->components.components.mods;
    components->depressed_layout = event->components.components.ਅਧਾਰ_ਸਮੂਹ;
    components->latched_layout = event->components.components.latched_ਸਮੂਹ;
    components->locked_layout = event->components.components.locked_ਸਮੂਹ;
    components->layout = event->components.components.ਸਮੂਹ;
    components->leds = event->components.components.leds;
    components->controls = (uint32_t)event->components.components.controls;

    ਵਾਪਸ XKB_SUCCESS;
}

ਗਿਣਤੀ xkb_ਸਥਿਤੀ
xkb_event_get_ਸੂਚਕ_motion(ਅਚਲ ਬਣਤਰ xkb_event * ਸੀਮਿਤ event,
                             ਬਣਤਰ xkb_event_ਸੂਚਕ_motion * ਸੀਮਿਤ motion)
{
    ਜੇ (event->type != XKB_EVENT_TYPE_ਸੂਚਕ_MOTION)
        ਵਾਪਸ XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ਅਚਲ ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ = xkb_check_state_abi(motion);
    ਜੇ (ਸਥਿਤੀ != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, ਸਥਿਤੀ);
        ਵਾਪਸ ਸਥਿਤੀ;
    }

    motion->flags = event->ਸੂਚਕ_motion.flags;
    motion->x = event->ਸੂਚਕ_motion.x;
    motion->y = event->ਸੂਚਕ_motion.y;

    ਵਾਪਸ XKB_SUCCESS;
}

ਗਿਣਤੀ xkb_ਸਥਿਤੀ
xkb_event_get_ਸੂਚਕ_button(ਅਚਲ ਬਣਤਰ xkb_event * ਸੀਮਿਤ event,
                             ਬਣਤਰ xkb_event_ਸੂਚਕ_button * ਸੀਮਿਤ button)
{
    ਜੇ (event->type != XKB_EVENT_TYPE_ਸੂਚਕ_BUTTON)
        ਵਾਪਸ XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ਅਚਲ ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ = xkb_check_state_abi(button);
    ਜੇ (ਸਥਿਤੀ != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, ਸਥਿਤੀ);
        ਵਾਪਸ ਸਥਿਤੀ;
    }

    button->button = event->ਸੂਚਕ_button.button;
    button->state = event->ਸੂਚਕ_button.state;
    button->ਗਿਣਤੀ_2 = event->ਸੂਚਕ_button.ਗਿਣਤੀ_2;

    ਵਾਪਸ XKB_SUCCESS;
}

ਗਿਣਤੀ xkb_ਸਥਿਤੀ
xkb_event_get_virtual_console(ਅਚਲ ਬਣਤਰ xkb_event * ਸੀਮਿਤ event,
                              int8_t * ਸੀਮਿਤ ਸੂਚਕਾਂਕ_or_ਵਿੱਥ,
                              bool * ਸੀਮਿਤ is_ਵਿੱਥ)
{
    ਜੇ (event->type != XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
        ਵਾਪਸ XKB_ERROR_EVENT_TYPE_MISMATCH;

    *ਸੂਚਕਾਂਕ_or_ਵਿੱਥ = event->virtual_console.ਸੂਚਕਾਂਕ_or_ਵਿੱਥ;
    *is_ਵਿੱਥ = event->virtual_console.is_ਵਿੱਥ;

    ਵਾਪਸ XKB_SUCCESS;
}

ਗਿਣਤੀ xkb_ਸਥਿਤੀ
xkb_state_update_event(ਬਣਤਰ xkb_state * ਸੀਮਿਤ ਅਧਾਰ_state,
                       ਅਚਲ ਬਣਤਰ xkb_event * ਸੀਮਿਤ event,
                       ਗਿਣਤੀ xkb_state_component * ਸੀਮਿਤ ਬਦਲਿਆ)
{
    /* Guard against server-only state */
    ਪ੍ਰਤਿਜ੍ਞਾ(ਅਧਾਰ_state->ਬਹੁਲਕ == SERVER_COMPANION ||
           ਅਧਾਰ_state->ਬਹੁਲਕ == LEGACY_MIXED_STATE);
    ਜੇ (ਅਧਾਰ_state->ਬਹੁਲਕ != SERVER_COMPANION &&
        ਅਧਾਰ_state->ਬਹੁਲਕ != LEGACY_MIXED_STATE) {
        log_err(ਅਧਾਰ_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_ਬਹੁਲਕ_2,
                "%s: Unexpected state type %d\n", __func__, ਅਧਾਰ_state->ਬਹੁਲਕ);
        ਵਾਪਸ XKB_ERROR_UNEXPECTED_STATE_ਬਹੁਲਕ;
    }

    ਬਣਤਰ xkb_client_state * ਅਚਲ state =
        (ਬਣਤਰ xkb_client_state *)ਅਧਾਰ_state;

    ਜੇ (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
        ਅਚਲ ਬਣਤਰ state_components prev_components = state->ਅਧਾਰ.components;
        state->ਅਧਾਰ.components = event->components.components;
        /*
         * Recompute the changes instead of using the event value, because we do
         * not know if the event’s queue and the state are synced.
         */
        ਜੇ (ਬਦਲਿਆ)
            *ਬਦਲਿਆ = get_state_component_changes(&prev_components,
                                                   &state->ਅਧਾਰ.components);
    } ਨਹੀਂਤਾਂ {
        ਜੇ (ਬਦਲਿਆ)
            *ਬਦਲਿਆ = 0;
    }
    ਵਾਪਸ XKB_SUCCESS;
}

ਸਥਿਰ ਖਾਲੀ
append_ਸੂਚਕ_button(ਬਣਤਰ xkb_events * ਸੀਮਿਤ events,
                      ਬਣਤਰ xkb_filter * ਸੀਮਿਤ filter,
                      ਗਿਣਤੀ xkb_ਸੂਚਕ_button_state button_state,
                      uint8_t ਗਿਣਤੀ_2)
{
    darray_append(events->ਕਤਾਰ, (ਬਣਤਰ xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_ਸੂਚਕ_BUTTON,
        .ਸੂਚਕ_button = {
            .ਮਾਪ = ਆਕਾਰ(((ਬਣਤਰ xkb_event*)0)->ਸੂਚਕ_button),
            .button = filter->priv,
            .state = button_state,
            .ਗਿਣਤੀ_2 = ਗਿਣਤੀ_2,
        }
    });
}

ਸਥਿਰ ਖਾਲੀ
xkb_filter_ਸੂਚਕ_button_new(ਬਣਤਰ xkb_server_state *state,
                              ਬਣਤਰ xkb_events *events,
                              ਬਣਤਰ xkb_filter *filter)
{
    ਜੇ (!events || state->ਅਧਾਰ.ਬਹੁਲਕ != SERVER_STATE ||
        !(state->ਅਧਾਰ.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ਕੁੰਜੀਆਂ)) {
        filter->func = ਨਿਰਾ;
        ਵਾਪਸ;
    }

    ਬਣਤਰ xkb_machine * ਅਚਲ sm = (ਬਣਤਰ xkb_machine *)state;

    /* Resolve button */
    ਅਚਲ xkb_ਸੂਚਕ_button_ਸੂਚਕਾਂਕ_t button =
        (filter->action.btn.button == XKB_ਸੂਚਕ_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;
    filter->priv = button;

    ਅਚਲ ਗਿਣਤੀ xkb_ਸੂਚਕ_button_state button_state =
        XKB_ਸੂਚਕ_BUTTON_PRESSED;
    append_ਸੂਚਕ_button(events, filter, button_state,
                          filter->action.btn.ਗਿਣਤੀ_2);
    state->flags &= ~STATE_REQUIRE_ਕੁੰਜੀ_EVENT;
}

ਸਥਿਰ bool
xkb_filter_ਸੂਚਕ_button_func(ਬਣਤਰ xkb_server_state *state,
                               ਬਣਤਰ xkb_events *events,
                               ਬਣਤਰ xkb_filter *filter,
                               ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                               ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction)
{
    ਜੇ (ਕੁੰਜੀ != filter->ਕੁੰਜੀ || !events)
        ਵਾਪਸ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ਕੁੰਜੀ_EVENT;

    ਚੋਣ (direction) {
    ਹਾਲਤ XKB_ਕੁੰਜੀ_ਥੱਲੇ:
        filter->refcnt++;
        /* fallthrough */
    ਹਾਲਤ XKB_ਕੁੰਜੀ_REPEATED:
        ਵਾਪਸ XKB_FILTER_CONSUME;
    ਮੂਲ:
        ਜੇ (--filter->refcnt > 0)
            ਵਾਪਸ XKB_FILTER_CONSUME;
    }

    ਜੇ (!filter->action.btn.ਗਿਣਤੀ_2)
        append_ਸੂਚਕ_button(events, filter, XKB_ਸੂਚਕ_BUTTON_RELEASED, 0);

    filter->func = ਨਿਰਾ;
    ਵਾਪਸ XKB_FILTER_CONSUME;
}

ਬਣਤਰ xkb_filter_ਸੂਚਕ_ਤਾਲਾ_button_priv {
    uint32_t button:31;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
    uint32_t locked:1; // MSVC requires matching types to pack bitfields
#ਨਹੀਂਤਾਂ
    bool locked:1;
#ਜੇ_ਅੰਤ
};

ਗਿਣਤੀ {
    XKB_FILTER_ਸੂਚਕ_ਤਾਲਾ_BUTTON_PRIV_ਚੌੜਾਈ =
        ਆਕਾਰ(ਬਣਤਰ xkb_filter_ਸੂਚਕ_ਤਾਲਾ_button_priv) * CHAR_BIT,
    XKB_FILTER_ਸੂਚਕ_ਤਾਲਾ_BUTTON_PRIV_MAX =
        (UINT64_C(1) << XKB_FILTER_ਸੂਚਕ_ਤਾਲਾ_BUTTON_PRIV_ਚੌੜਾਈ) - 1
};

static_assert(ਆਕਾਰ(ਬਣਤਰ xkb_filter_ਸੂਚਕ_ਤਾਲਾ_button_priv) ==
              ਆਕਾਰ(((ਬਣਤਰ xkb_filter*)0)->priv),
              "");
static_assert((uint32_t)XKB_ਸੂਚਕ_BUTTON_MAX < (uint32_t)XKB_FILTER_ਸੂਚਕ_ਤਾਲਾ_BUTTON_PRIV_MAX,
              "xkb_filter_pointer_lock_button_priv cannot store button");

ਸਥਿਰ ਖਾਲੀ
xkb_filter_ਸੂਚਕ_ਤਾਲਾ_button_new(ਬਣਤਰ xkb_server_state *state,
                                   ਬਣਤਰ xkb_events *events,
                                   ਬਣਤਰ xkb_filter *filter)
{
    ਜੇ (!events || state->ਅਧਾਰ.ਬਹੁਲਕ != SERVER_STATE ||
        !(state->ਅਧਾਰ.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ਕੁੰਜੀਆਂ)) {
        filter->func = ਨਿਰਾ;
        ਵਾਪਸ;
    }

    state->flags &= ~STATE_REQUIRE_ਕੁੰਜੀ_EVENT;

    ਬਣਤਰ xkb_machine * ਅਚਲ sm = (ਬਣਤਰ xkb_machine *)state;

    /* Resolve button */
    ਅਚਲ xkb_ਸੂਚਕ_button_ਸੂਚਕਾਂਕ_t button =
        (filter->action.btn.button == XKB_ਸੂਚਕ_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;

    static_assert(XKB_ਸੂਚਕ_BUTTON_ਨਕਾਬ_ਚੌੜਾਈ <= 8,
                  "invalid UINTx_C() macro");
    static_assert(XKB_ਸੂਚਕ_BUTTON_MAX <= XKB_ਸੂਚਕ_BUTTON_ਨਕਾਬ_ਚੌੜਾਈ,
                  "invalid left shift");
    ਅਚਲ xkb_ਸੂਚਕ_button_ਨਕਾਬ_t ਨਕਾਬ = (UINT8_C(1) << button);

    ਅਚਲ bool locked = (sm->mouse.locked_buttons & ਨਕਾਬ);

    ਜੇ (!locked && !(filter->action.btn.flags & ACTION_ਤਾਲਾ_NO_ਤਾਲਾ)) {
        sm->mouse.locked_buttons |= ਨਕਾਬ;
        filter->priv = button;
        append_ਸੂਚਕ_button(events, filter, XKB_ਸੂਚਕ_BUTTON_PRESSED, 0);
    }

    ਬਣਤਰ xkb_filter_ਸੂਚਕ_ਤਾਲਾ_button_priv *priv =
        (ਬਣਤਰ xkb_filter_ਸੂਚਕ_ਤਾਲਾ_button_priv*)&filter->priv;
    priv->button = button,
    priv->locked = locked;
}

ਸਥਿਰ bool
xkb_filter_ਸੂਚਕ_ਤਾਲਾ_button_func(ਬਣਤਰ xkb_server_state *state,
                                    ਬਣਤਰ xkb_events *events,
                                    ਬਣਤਰ xkb_filter *filter,
                                    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                                    ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction)
{
    ਜੇ (ਕੁੰਜੀ != filter->ਕੁੰਜੀ || !events || state->ਅਧਾਰ.ਬਹੁਲਕ != SERVER_STATE)
        ਵਾਪਸ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ਕੁੰਜੀ_EVENT;

    ਚੋਣ (direction) {
    ਹਾਲਤ XKB_ਕੁੰਜੀ_ਥੱਲੇ:
        filter->refcnt++;
        /* fallthrough */
    ਹਾਲਤ XKB_ਕੁੰਜੀ_REPEATED:
        ਵਾਪਸ XKB_FILTER_CONSUME;
    ਮੂਲ:
        ਜੇ (--filter->refcnt > 0)
            ਵਾਪਸ XKB_FILTER_CONSUME;
    }

    ਅਚਲ ਬਣਤਰ xkb_filter_ਸੂਚਕ_ਤਾਲਾ_button_priv *priv =
        (ਬਣਤਰ xkb_filter_ਸੂਚਕ_ਤਾਲਾ_button_priv*)&filter->priv;

    ਜੇ (priv->locked && !(filter->action.btn.flags & ACTION_ਤਾਲਾ_NO_UNLOCK)) {
        ਬਣਤਰ xkb_machine * ਅਚਲ sm = (ਬਣਤਰ xkb_machine *)state;

        static_assert(XKB_ਸੂਚਕ_BUTTON_ਨਕਾਬ_ਚੌੜਾਈ <= 8,
                    "invalid UINTx_C() macro");
        static_assert(XKB_ਸੂਚਕ_BUTTON_MAX <= XKB_ਸੂਚਕ_BUTTON_ਨਕਾਬ_ਚੌੜਾਈ,
                    "invalid left shift");
        ਅਚਲ xkb_ਸੂਚਕ_button_ਨਕਾਬ_t ਨਕਾਬ = (UINT8_C(1) << priv->button);

        sm->mouse.locked_buttons &= ~ਨਕਾਬ;
        filter->priv = priv->button;
        append_ਸੂਚਕ_button(events, filter, XKB_ਸੂਚਕ_BUTTON_RELEASED, 0);
    }

    filter->func = ਨਿਰਾ;
    ਵਾਪਸ XKB_FILTER_CONSUME;
}

ਸਥਿਰ ਖਾਲੀ
xkb_filter_ਸੂਚਕ_ਸੈੱਟ_default_button_new(ਬਣਤਰ xkb_server_state *state,
                                          ਬਣਤਰ xkb_events *events,
                                          ਬਣਤਰ xkb_filter *filter)
{
    ਜੇ (!events || state->ਅਧਾਰ.ਬਹੁਲਕ != SERVER_STATE ||
        !(state->ਅਧਾਰ.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ਕੁੰਜੀਆਂ)) {
        filter->func = ਨਿਰਾ;
        ਵਾਪਸ;
    }

    state->flags &= ~STATE_REQUIRE_ਕੁੰਜੀ_EVENT;

    ਬਣਤਰ xkb_machine * ਅਚਲ sm = (ਬਣਤਰ xkb_machine *)state;

    static_assert(2 * XKB_ਸੂਚਕ_BUTTON_MAX <= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ, "lossy conversion");
    ਪੂਰਨ button = (filter->action.dflt.flags & ACTION_ABSOLUTE_SWITCH)
        ? filter->action.dflt.ਮੁੱਲ
        : sm->mouse.default_button + filter->action.dflt.ਮੁੱਲ;

    /* Wrap into range */
    ਅਚਲ ਪੂਰਨ rem = button % XKB_ਸੂਚਕ_BUTTON_MAX;
    sm->mouse.default_button = (rem >= 0)
        ? (xkb_ਸੂਚਕ_button_ਸੂਚਕਾਂਕ_t)rem
        : (xkb_ਸੂਚਕ_button_ਸੂਚਕਾਂਕ_t)(rem + XKB_ਸੂਚਕ_BUTTON_MAX);
}

ਸਥਿਰ bool
xkb_filter_ਸੂਚਕ_ਸੈੱਟ_default_button_func(ਬਣਤਰ xkb_server_state *state,
                                           ਬਣਤਰ xkb_events *events,
                                           ਬਣਤਰ xkb_filter *filter,
                                           ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                                           ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction)
{
    ਜੇ (ਕੁੰਜੀ != filter->ਕੁੰਜੀ || !events)
        ਵਾਪਸ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ਕੁੰਜੀ_EVENT;

    ਚੋਣ (direction) {
    ਹਾਲਤ XKB_ਕੁੰਜੀ_ਥੱਲੇ:
        filter->refcnt++;
        /* fallthrough */
    ਹਾਲਤ XKB_ਕੁੰਜੀ_REPEATED:
        ਵਾਪਸ XKB_FILTER_CONSUME;
    ਮੂਲ:
        ਜੇ (--filter->refcnt > 0)
            ਵਾਪਸ XKB_FILTER_CONSUME;
    }

    filter->func = ਨਿਰਾ;
    ਵਾਪਸ XKB_FILTER_CONTINUE;
}

ਸਥਿਰ ਖਾਲੀ
xkb_filter_terminate_server_new(ਬਣਤਰ xkb_server_state *state,
                                 ਬਣਤਰ xkb_events *events,
                                 ਬਣਤਰ xkb_filter *filter)
{
    ਜੇ (!events || state->ਅਧਾਰ.ਬਹੁਲਕ != SERVER_STATE ||
        !(((ਬਣਤਰ xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = ਨਿਰਾ;
        ਵਾਪਸ;
    }

    state->flags &= ~STATE_REQUIRE_ਕੁੰਜੀ_EVENT;

    darray_append(events->ਕਤਾਰ, (ਬਣਤਰ xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    });
}

ਸਥਿਰ bool
xkb_filter_terminate_server_func(ਬਣਤਰ xkb_server_state *state,
                                 ਬਣਤਰ xkb_events *events,
                                 ਬਣਤਰ xkb_filter *filter,
                                 ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                                 ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction)
{
    ਜੇ (ਕੁੰਜੀ != filter->ਕੁੰਜੀ || !events)
        ਵਾਪਸ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ਕੁੰਜੀ_EVENT;

    ਚੋਣ (direction) {
    ਹਾਲਤ XKB_ਕੁੰਜੀ_ਥੱਲੇ:
        filter->refcnt++;
        /* fallthrough */
    ਹਾਲਤ XKB_ਕੁੰਜੀ_REPEATED:
        ਵਾਪਸ XKB_FILTER_CONSUME;
    ਮੂਲ:
        ਜੇ (--filter->refcnt > 0)
            ਵਾਪਸ XKB_FILTER_CONSUME;
    }

    filter->func = ਨਿਰਾ;
    ਵਾਪਸ XKB_FILTER_CONSUME;
}

ਸਥਿਰ ਖਾਲੀ
xkb_filter_switch_virtual_console_new(ਬਣਤਰ xkb_server_state *state,
                                      ਬਣਤਰ xkb_events *events,
                                      ਬਣਤਰ xkb_filter *filter)
{
    ਜੇ (!events || state->ਅਧਾਰ.ਬਹੁਲਕ != SERVER_STATE ||
        !(((ਬਣਤਰ xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = ਨਿਰਾ;
        ਵਾਪਸ;
    }

    state->flags &= ~STATE_REQUIRE_ਕੁੰਜੀ_EVENT;

    darray_append(events->ਕਤਾਰ, (ਬਣਤਰ xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
        .virtual_console = {
            .ਸੂਚਕਾਂਕ_or_ਵਿੱਥ = filter->action.screen.screen,
            .is_ਵਿੱਥ = !(filter->action.screen.flags & ACTION_ABSOLUTE_SWITCH)
        }
    });
}

ਸਥਿਰ bool
xkb_filter_switch_virtual_console_func(ਬਣਤਰ xkb_server_state *state,
                                       ਬਣਤਰ xkb_events *events,
                                       ਬਣਤਰ xkb_filter *filter,
                                       ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ,
                                       ਗਿਣਤੀ xkb_ਕੁੰਜੀ_direction direction)
{
    ਜੇ (ਕੁੰਜੀ != filter->ਕੁੰਜੀ || !events)
        ਵਾਪਸ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ਕੁੰਜੀ_EVENT;

    ਚੋਣ (direction) {
    ਹਾਲਤ XKB_ਕੁੰਜੀ_ਥੱਲੇ:
        filter->refcnt++;
        /* fallthrough */
    ਹਾਲਤ XKB_ਕੁੰਜੀ_REPEATED:
        ਵਾਪਸ XKB_FILTER_CONSUME;
    ਮੂਲ:
        ਜੇ (--filter->refcnt > 0)
            ਵਾਪਸ XKB_FILTER_CONSUME;
    }

    filter->func = ਨਿਰਾ;
    ਵਾਪਸ XKB_FILTER_CONSUME;
}
