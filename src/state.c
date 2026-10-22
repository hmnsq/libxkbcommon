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
ರಚನೆ xkb_events {
    ರಚನೆ xkb_context *ctx;
    darray(ರಚನೆ xkb_event) ಸರತಿ;
    /**
     * Read cursor for `xkb_events_next()`.
     *
     * Reset to 0 on each `process_*` call.
     */
    darray_ಗಾತ್ರ_t ಮುಂದಿನ;
};

ರಚನೆ xkb_server_state;

ರಚನೆ xkb_filter {
    ಒಕ್ಕೂಟ xkb_action action;
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ;
    bool (*func)(ರಚನೆ xkb_server_state *state,
                 ರಚನೆ xkb_events *events,
                 ರಚನೆ xkb_filter *filter,
                 ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                 ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction);
    uint32_t priv;
    ಪೂರ್ಣಾಂಕ refcnt;
};

ಎಣಿಕೆ xkb_state_ಬಹುಲಕ_ಆಂತರಿಕ {
    /** `xkb_state`: Client-only state (server API raises errors) */
    CLIENT_STATE = XKB_STATE_ಬಹುಲಕ_CLIENT,
    /** `xkb_state`: Server query companion */
    SERVER_COMPANION = XKB_STATE_ಬಹುಲಕ_SERVER_QUERY,
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

ಎಣಿಕೆ { XKB_LAYOUT_ಸೂಚ್ಯಂಕ_T_MIN_ಅಗಲ = 6 };
static_assert(XKB_MAX_GROUPS < (1 << XKB_LAYOUT_ಸೂಚ್ಯಂಕ_T_MIN_ಅಗಲ),
              "Cannot encode out_of_range_group::redirect_group");

ರಚನೆ ಹೊರಹೋಗು_ದ_range_ಗುಂಪು {
    ಎಣಿಕೆ xkb_layout_ಹೊರಹೋಗು_ದ_range_policy policy :
        (ಗಾತ್ರ(ಎಣಿಕೆ xkb_layout_ಹೊರಹೋಗು_ದ_range_policy) * CHAR_BIT -
         XKB_LAYOUT_ಸೂಚ್ಯಂಕ_T_MIN_ಅಗಲ);
    xkb_layout_ಸೂಚ್ಯಂಕ_t redirect_ಗುಂಪು : XKB_LAYOUT_ಸೂಚ್ಯಂಕ_T_MIN_ಅಗಲ;
};

ಎಣಿಕೆ { XKB_STATE_ಬಹುಲಕ_ಆಂತರಿಕ_MIN_ಅಗಲ = 4 /* 3 bits + sign */ };
static_assert(XKB_STATE_TYPE_ಅಂಕೆ_ENTRIES <
              (1 << XKB_STATE_ಬಹುಲಕ_ಆಂತರಿಕ_MIN_ಅಗಲ),
              "Cannot encode xkb_state::mode");

/**
 * Common state
 *
 * `mode` indicates whether the object is casted from a `xkb_server_state` or a
 * `xkb_client_state` struct.
 */
ರಚನೆ xkb_state {
    ರಚನೆ xkb_keymap *keymap;
    /*
     * Before updating the state, we keep a copy of just this struct. This
     * allows us to report which components of the state have changed.
     */
    ರಚನೆ state_components components;
    ರಚನೆ ಹೊರಹೋಗು_ದ_range_ಗುಂಪು ಹೊರಹೋಗು_ದ_range_ಗುಂಪು;

    ಎಣಿಕೆ xkb_state_ಬಹುಲಕ_ಆಂತರಿಕ ಬಹುಲಕ : XKB_STATE_ಬಹುಲಕ_ಆಂತರಿಕ_MIN_ಅಗಲ;
    ಪೂರ್ಣಾಂಕ refcnt : (ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ) * CHAR_BIT - XKB_STATE_ಬಹುಲಕ_ಆಂತರಿಕ_MIN_ಅಗಲ);
};

/**
 * Minimal client state
 *
 * Wrapper for strong typing
 */
ರಚನೆ xkb_client_state {
    /** Inherits from `xkb_state` */
    ರಚನೆ xkb_state ತಳ;
};

ಎಣಿಕೆ state_flags {
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
ರಚನೆ xkb_server_state {
    /** Inherits from `xkb_state` */
    ರಚನೆ xkb_state ತಳ;

    /* Server-specific state data */

    darray(ರಚನೆ xkb_filter) filters;

    /**
     * Mix of constant configuration flags and
     * variable state-processing flags.
     */
    ಎಣಿಕೆ state_flags flags;

    ಎಣಿಕೆ xkb_a11y_flags a11y_flags;

    /*
     * At each event, we accumulate all the needed modifications to the base
     * modifiers, and apply them at the end. These keep track of this state.
     */
    xkb_mod_ಮುಖವಾಡ_t ಗಣ_mods;
    xkb_mod_ಮುಖವಾಡ_t clear_mods;

    /*
     * We mustn't clear a base modifier if there's another depressed key
     * which affects it, e.g. given this sequence
     * < Left Shift down, Right Shift down, Left Shift Up >
     * the modifier should still be set. This keeps the count.
     */
    int16_t mod_ಕೀಲಿ_ಎಣಿಕೆ[XKB_MAX_MODS];
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು *
get_ನಮೂದು_for_mods(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type *type, xkb_mod_ಮುಖವಾಡ_t mods)
{
    ಗಾಗಿ (darray_ಗಾತ್ರ_t i = 0; i < type->ಅಂಕೆ_entries; i++)
        ಆದರೆ (ನಮೂದು_is_ಸಕ್ರಿಯ(&type->entries[i]) &&
            type->entries[i].mods.ಮುಖವಾಡ == mods)
            ಹಿಂತಿರುಗಿ &type->entries[i];
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು *
get_ನಮೂದು_for_ಕೀಲಿ_state(ರಚನೆ xkb_state *state, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                        xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುಂಪು)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type* ಸ್ಥಿರಾಂಕ type = ಕೀಲಿ->groups[ಗುಂಪು].type;
    xkb_mod_ಮುಖವಾಡ_t ಸಕ್ರಿಯ_mods = state->components.mods & type->mods.ಮುಖವಾಡ;
    ಹಿಂತಿರುಗಿ get_ನಮೂದು_for_mods(type, ಸಕ್ರಿಯ_mods);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ xkb_level_ಸೂಚ್ಯಂಕ_t
state_ಕೀಲಿ_get_level(ರಚನೆ xkb_state *state, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                         xkb_layout_ಸೂಚ್ಯಂಕ_t layout)
{
    ಆದರೆ (layout >= ಕೀಲಿ->ಅಂಕೆ_groups)
        ಹಿಂತಿರುಗಿ XKB_LEVEL_INVALID;

    /* If we don't find an explicit match the default is 0. */
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು* ಸ್ಥಿರಾಂಕ ನಮೂದು =
        get_ನಮೂದು_for_ಕೀಲಿ_state(state, ಕೀಲಿ, layout);

    ಹಿಂತಿರುಗಿ (ನಮೂದು) ? ನಮೂದು->level : 0;
}

/**
 * Returns the level to use for the given key and state, or
 * XKB_LEVEL_INVALID.
 */
xkb_level_ಸೂಚ್ಯಂಕ_t
xkb_state_ಕೀಲಿ_get_level(ರಚನೆ xkb_state *state, xkb_keycode_t kc,
                        xkb_layout_ಸೂಚ್ಯಂಕ_t layout)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ* ಸ್ಥಿರಾಂಕ ಕೀಲಿ = Xkb_ಕೀಲಿ(state->keymap, kc);

    ಹಿಂತಿರುಗಿ (ಕೀಲಿ)
        ? state_ಕೀಲಿ_get_level(state, ಕೀಲಿ, layout)
        : XKB_LEVEL_INVALID;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ xkb_layout_ಸೂಚ್ಯಂಕ_t
state_ಕೀಲಿ_get_layout(ರಚನೆ xkb_state *state, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ)
{
    ಹಿಂತಿರುಗಿ xkb_keymap_ಕೀಲಿ_effective_layout(ಕೀಲಿ, state->components.ಗುಂಪು);
}

/**
 * Returns the layout to use for the given key and state, taking
 * wrapping/clamping/etc into account, or XKB_LAYOUT_INVALID.
 */
xkb_layout_ಸೂಚ್ಯಂಕ_t
xkb_state_ಕೀಲಿ_get_layout(ರಚನೆ xkb_state *state, xkb_keycode_t kc)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ* ಸ್ಥಿರಾಂಕ ಕೀಲಿ = Xkb_ಕೀಲಿ(state->keymap, kc);

    ಆದರೆ (!ಕೀಲಿ)
        ಹಿಂತಿರುಗಿ XKB_LAYOUT_INVALID;

    ಹಿಂತಿರುಗಿ state_ಕೀಲಿ_get_layout(state, ಕೀಲಿ);
}

/* Empty action used for empty levels */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action dummy_action = { .type = ACTION_TYPE_NONE };

ಸ್ಥಿರ xkb_action_ಎಣಿಕೆ_t
xkb_ಕೀಲಿ_get_actions(ರಚನೆ xkb_state *state, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                    ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action **actions)
{
    ಸ್ಥಿರಾಂಕ xkb_layout_ಸೂಚ್ಯಂಕ_t layout = state_ಕೀಲಿ_get_layout(state, ಕೀಲಿ);
    ಸ್ಥಿರಾಂಕ xkb_level_ಸೂಚ್ಯಂಕ_t level = state_ಕೀಲಿ_get_level(state, ಕೀಲಿ, layout);
    ಆದರೆ (level == XKB_LEVEL_INVALID)
        ಹೋಗು err;

    ಸ್ಥಿರಾಂಕ xkb_action_ಎಣಿಕೆ_t ಎಣಿಕೆ_2 =
        xkb_keymap_ಕೀಲಿ_get_actions_by_level(state->keymap, ಕೀಲಿ,
                                            layout, level, actions);
    ಆದರೆ (!ಎಣಿಕೆ_2)
        ಹೋಗು err;

    ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_2;

err:
    /* Use a dummy action if no corresponding level was found or if it is empty.
     * This is required e.g. to handle latches properly. */
    *actions = &dummy_action;
    ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ರಚನೆ xkb_filter *
xkb_filter_new(ರಚನೆ xkb_server_state *state)
{
    ರಚನೆ xkb_filter *filter = ಖಾಲಿ, *iter;

    darray_foreach(iter, state->filters) {
        ಆದರೆ (iter->func)
            ಮುಂದುವರಿ;
        /* Use available slot */
        filter = iter;
        ನಿಲ್ಲಿಸು;
    }

    ಆದರೆ (!filter) {
        /* No available slot: resize the filters array */
        darray_resize0(state->filters, darray_ಗಾತ್ರ(state->filters) + 1);
        filter = &darray_ಅಂಶ(state->filters, darray_ಗಾತ್ರ(state->filters) -1);
    }

    filter->refcnt = 1;
    ಹಿಂತಿರುಗಿ filter;
}

/***====================================================================***/

ಎಣಿಕೆ xkb_filter_ಫಲಿತಾಂಶ {
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
#ವ್ಯಾಖ್ಯಾನಿಸು apply_ಗುಂಪು_delta(filter_, state_, component_)                   \
    ಆದರೆ ((filter_)->action.ಗುಂಪು.flags & ACTION_ABSOLUTE_SWITCH)          \
        (state_)->components.component_ = (filter_)->action.ಗುಂಪು.ಗುಂಪು; \
    ಇಲ್ಲದಿದ್ದರೆ                                                                 \
        (state_)->components.component_ += (filter_)->action.ಗುಂಪು.ಗುಂಪು

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_ಗುಂಪು_ಗಣ_new(ರಚನೆ xkb_server_state *state,
                         ರಚನೆ xkb_events *events,
                         ರಚನೆ xkb_filter *filter)
{
    static_assert(ಗಾತ್ರ(state->ತಳ.components.ತಳ_ಗುಂಪು) ==
                  ಗಾತ್ರ(filter->priv),
                  "Max groups don't fit");
    filter->priv = (uint32_t)state->ತಳ.components.ತಳ_ಗುಂಪು;
    apply_ಗುಂಪು_delta(filter, &state->ತಳ, ತಳ_ಗುಂಪು);
}

ಸ್ಥಿರ bool
xkb_filter_ಗುಂಪು_ಗಣ_func(ರಚನೆ xkb_server_state *state,
                          ರಚನೆ xkb_events *events,
                          ರಚನೆ xkb_filter *filter,
                          ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                          ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction)
{
    ಆದರೆ (ಕೀಲಿ != filter->ಕೀಲಿ) {
        filter->action.ಗುಂಪು.flags &= ~ACTION_ಬೀಗ_CLEAR;
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;
    }

    ಆಯ್ಕೆ (direction) {
    ಸ್ಥಿತಿ XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ:
        filter->refcnt++;
        /* fallthrough */
    ಸ್ಥಿತಿ XKB_ಕೀಲಿ_REPEATED:
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಆದರೆ (--filter->refcnt > 0)
            ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    }

    static_assert(ಗಾತ್ರ(state->ತಳ.components.ತಳ_ಗುಂಪು) == ಗಾತ್ರ(filter->priv),
                  "Max groups don't fit");
    state->ತಳ.components.ತಳ_ಗುಂಪು = (int32_t) filter->priv;

    ಆದರೆ (filter->action.ಗುಂಪು.flags & ACTION_ಬೀಗ_CLEAR)
        state->ತಳ.components.locked_ಗುಂಪು = 0;

    filter->func = ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_state_component
get_state_component_changes(ಸ್ಥಿರಾಂಕ ರಚನೆ state_components *a,
                            ಸ್ಥಿರಾಂಕ ರಚನೆ state_components *b)
{
    ಎಣಿಕೆ xkb_state_component ಮುಖವಾಡ = 0;

    ಆದರೆ (a->ಗುಂಪು != b->ಗುಂಪು)
        ಮುಖವಾಡ |= XKB_STATE_LAYOUT_EFFECTIVE;
    ಆದರೆ (a->ತಳ_ಗುಂಪು != b->ತಳ_ಗುಂಪು)
        ಮುಖವಾಡ |= XKB_STATE_LAYOUT_DEPRESSED;
    ಆದರೆ (a->latched_ಗುಂಪು != b->latched_ಗುಂಪು)
        ಮುಖವಾಡ |= XKB_STATE_LAYOUT_LATCHED;
    ಆದರೆ (a->locked_ಗುಂಪು != b->locked_ಗುಂಪು)
        ಮುಖವಾಡ |= XKB_STATE_LAYOUT_LOCKED;
    ಆದರೆ (a->mods != b->mods)
        ಮುಖವಾಡ |= XKB_STATE_MODS_EFFECTIVE;
    ಆದರೆ (a->ತಳ_mods != b->ತಳ_mods)
        ಮುಖವಾಡ |= XKB_STATE_MODS_DEPRESSED;
    ಆದರೆ (a->latched_mods != b->latched_mods)
        ಮುಖವಾಡ |= XKB_STATE_MODS_LATCHED;
    ಆದರೆ (a->locked_mods != b->locked_mods)
        ಮುಖವಾಡ |= XKB_STATE_MODS_LOCKED;
    ಆದರೆ (a->leds != b->leds)
        ಮುಖವಾಡ |= XKB_STATE_LEDS;
    ಆದರೆ (a->controls != b->controls)
        ಮುಖವಾಡ |= XKB_STATE_CONTROLS_EFFECTIVE;

    ಹಿಂತಿರುಗಿ ಮುಖವಾಡ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_ಗುಂಪು_ಬೀಗ_new(ರಚನೆ xkb_server_state *state,
                          ರಚನೆ xkb_events *events,
                          ರಚನೆ xkb_filter *filter)
{
    ಆದರೆ (filter->action.ಗುಂಪು.flags & ACTION_ಬೀಗ_ON_RELEASE) {
        /*
         * Lock on key release: do nothing on key press.
         *
         * This is a keymap format v2 extension.
         */
        ಹಿಂತಿರುಗಿ;
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* Lock on key press */
        apply_ಗುಂಪು_delta(filter, &state->ತಳ, locked_ಗುಂಪು);
    }
}

ಸ್ಥಿರ bool
xkb_filter_ಗುಂಪು_ಬೀಗ_func(ರಚನೆ xkb_server_state *state,
                           ರಚನೆ xkb_events *events,
                           ರಚನೆ xkb_filter *filter,
                           ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                           ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction)
{
    ಆದರೆ (ಕೀಲಿ != filter->ಕೀಲಿ) {
        ಆದರೆ ((filter->action.ಗುಂಪು.flags & ACTION_ಬೀಗ_ON_RELEASE) &&
            direction == XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ) {
            /*
             * Another key has been pressed after the locking key:
             * cancel group lock on release.
             *
             * This is a keymap v2 extension.
             */
            filter->action.ಗುಂಪು.flags &= ~ACTION_ಬೀಗ_ON_RELEASE;
        }
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;
    }

    ಆಯ್ಕೆ (direction) {
    ಸ್ಥಿತಿ XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ:
        filter->refcnt++;
        /* fallthrough */
    ಸ್ಥಿತಿ XKB_ಕೀಲಿ_REPEATED:
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಆದರೆ (--filter->refcnt > 0)
            ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    }

    ಆದರೆ (filter->action.ಗುಂಪು.flags & ACTION_ಬೀಗ_ON_RELEASE) {
        /*
         * Lock on key release
         *
         * This is a keymap v2 extension.
         */
        apply_ಗುಂಪು_delta(filter, &state->ತಳ, locked_ಗುಂಪು);
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* Lock on key press: do nothing on key release. */
    }
    filter->func = ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;
}

ಸ್ಥಿರ bool
xkb_action_breaks_latch(ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action *action,
                        ಎಣಿಕೆ xkb_action_controls controls,
                        ಎಣಿಕೆ xkb_ಆಂತರಿಕ_action_flags flag,
                        xkb_mod_ಮುಖವಾಡ_t ಮುಖವಾಡ)
{
    ಆಯ್ಕೆ (action->type) {
    ಸ್ಥಿತಿ ACTION_TYPE_NONE:
    ಸ್ಥಿತಿ ACTION_TYPE_VOID:
    ಸ್ಥಿತಿ ACTION_TYPE_PTR_BUTTON:
    ಸ್ಥಿತಿ ACTION_TYPE_PTR_ಬೀಗ:
    ಸ್ಥಿತಿ ACTION_TYPE_CTRL_ಗಣ:
    ಸ್ಥಿತಿ ACTION_TYPE_CTRL_ಬೀಗ:
    ಸ್ಥಿತಿ ACTION_TYPE_SWITCH_VT:
    ಸ್ಥಿತಿ ACTION_TYPE_TERMINATE:
    ಸ್ಥಿತಿ ACTION_TYPE_REDIRECT_ಕೀಲಿ:
        ಹಿಂತಿರುಗಿ true;
    ಸ್ಥಿತಿ ACTION_TYPE_PTR_MOVE:
    ಸ್ಥಿತಿ ACTION_TYPE_PTR_DEFAULT:
        ಹಿಂತಿರುಗಿ !(controls & CONTROL_MOUSE_ಕೀಲಿಗಳು);
    ಸ್ಥಿತಿ ACTION_TYPE_ಆಂತರಿಕ:
        ಹಿಂತಿರುಗಿ (action->ಆಂತರಿಕ.flags & flag) &&
               ((action->ಆಂತರಿಕ.clear_latched_mods & ಮುಖವಾಡ) == ಮುಖವಾಡ);
    ಪೂರ್ವನಿಯೋಜಿತ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_ಆಂತರಿಕ == 20 &&
                      ACTION_TYPE_ಆಂತರಿಕ + 1 == ACTION_TYPE_ಅಂಕೆ_ENTRIES,
                      "Missing action type");
        ಹಿಂತಿರುಗಿ false;
    }
}

ಎಣಿಕೆ xkb_ಕೀಲಿ_latch_state {
    NO_LATCH = 0,
    LATCH_ಕೀಲಿ_ಕೆಳಕ್ಕೆ,
    LATCH_PENDING,
    ಕೀಲಿ_LATCH_STATE_ಅಂಕೆ_ENTRIES
};

#ವ್ಯಾಖ್ಯಾನಿಸು MAX_XKB_ಕೀಲಿ_LATCH_STATE_LOG2 2
#ಆದರೆ (ಕೀಲಿ_LATCH_STATE_ಅಂಕೆ_ENTRIES > (1 << MAX_XKB_ಕೀಲಿ_LATCH_STATE_LOG2)) || \
    (-XKB_MAX_GROUPS) < (INT32_MIN >> MAX_XKB_ಕೀಲಿ_LATCH_STATE_LOG2) || \
      XKB_MAX_GROUPS > (INT32_MAX >> MAX_XKB_ಕೀಲಿ_LATCH_STATE_LOG2)
#ದೋಷ "Cannot represent priv field of the group latch filter"
#ಆದರೆ_ಅಂತ್ಯ

/* Hold the latch state *and* the group delta */
ಒಕ್ಕೂಟ ಗುಂಪು_latch_priv {
    uint32_t priv;
    ರಚನೆ {
        /* The type is really: enum xkb_key_latch_state, but it is problematic
         * on Windows, because it is interpreted as signed and leads to wrong
         * negative values. */
        ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ latch:MAX_XKB_ಕೀಲಿ_LATCH_STATE_LOG2;
        int32_t ಗುಂಪು_delta:(32 - MAX_XKB_ಕೀಲಿ_LATCH_STATE_LOG2);
    };
};

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_ಗುಂಪು_latch_new(ರಚನೆ xkb_server_state *state,
                           ರಚನೆ xkb_events *events,
                           ರಚನೆ xkb_filter *filter)
{
    ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ ಗುಂಪು_latch_priv priv = {
        .latch = LATCH_ಕೀಲಿ_ಕೆಳಕ್ಕೆ,
        .ಗುಂಪು_delta = (filter->action.ಗುಂಪು.flags & ACTION_ABSOLUTE_SWITCH)
            ? filter->action.ಗುಂಪು.ಗುಂಪು - state->ತಳ.components.ತಳ_ಗುಂಪು
            : filter->action.ಗುಂಪು.ಗುಂಪು
    };
    filter->priv = priv.priv;
    /* Like group set */
    apply_ಗುಂಪು_delta(filter, &state->ತಳ, ತಳ_ಗುಂಪು);
}

ಸ್ಥಿರ bool
xkb_filter_ಗುಂಪು_latch_func(ರಚನೆ xkb_server_state *state,
                            ರಚನೆ xkb_events *events,
                            ರಚನೆ xkb_filter *filter,
                            ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                            ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction)
{
    ಒಕ್ಕೂಟ ಗುಂಪು_latch_priv priv = {.priv = filter->priv};
    ಎಣಿಕೆ xkb_ಕೀಲಿ_latch_state latch = priv.latch;

    ಆದರೆ (direction == XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ) {
        ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action *actions = ಖಾಲಿ;
        ಸ್ಥಿರಾಂಕ xkb_action_ಎಣಿಕೆ_t ಎಣಿಕೆ_2 = xkb_ಕೀಲಿ_get_actions(&state->ತಳ, ಕೀಲಿ,
                                                             &actions);

        ಆದರೆ (latch == LATCH_ಕೀಲಿ_ಕೆಳಕ್ಕೆ) {
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
            ಆದರೆ (state->a11y_flags & XKB_A11_Y_LATCH_ಏಕಕಾಲಿಕ_ಕೀಲಿಗಳು) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                ಗಾಗಿ (xkb_action_ಎಣಿಕೆ_t k = 0; k < ಎಣಿಕೆ_2; k++) {
                    ಆದರೆ (xkb_action_breaks_latch(&(actions[k]),
                                                state->ತಳ.components.controls,
                                                ಆಂತರಿಕ_BREAKS_ಗುಂಪು_LATCH,
                                                0))
                    {
                        latch = NO_LATCH;
                        ನಿಲ್ಲಿಸು;
                    }
                }
            } ಇಲ್ಲದಿದ್ದರೆ {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is
             * pressed, or promote it to a lock if it's the same group delta &
             * flags and latchToLock option is enabled. */
            ಸ್ಥಿರಾಂಕ bool sticky_ಕೀಲಿಗಳು = state->ತಳ.components.controls
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
            ಗಾಗಿ (xkb_action_ಎಣಿಕೆ_t k = 0; k < ಎಣಿಕೆ_2; k++) {
                ಆದರೆ (((actions[k].type == ACTION_TYPE_ಗುಂಪು_LATCH) ||
                     (actions[k].type == ACTION_TYPE_ಗುಂಪು_ಗಣ && sticky_ಕೀಲಿಗಳು)) &&
                     actions[k].ಗುಂಪು.ಗುಂಪು == filter->action.ಗುಂಪು.ಗುಂಪು &&
                     (actions[k].ಗುಂಪು.flags & ACTION_ABSOLUTE_SWITCH) ==
                     (filter->action.ಗುಂಪು.flags & ACTION_ABSOLUTE_SWITCH)) {
                    ಆದರೆ (filter->action.ಗುಂಪು.flags & ACTION_LATCH_ಗಮ್ಯ_ಬೀಗ &&
                        filter->action.ಗುಂಪು.ಗುಂಪು != 0)
                    {
                        /* Promote to lock */
                        filter->action.type = ACTION_TYPE_ಗುಂಪು_ಬೀಗ;
                        filter->func = xkb_filter_ಗುಂಪು_ಬೀಗ_func;
                        xkb_filter_ಗುಂಪು_ಬೀಗ_new(state, events, filter);
                        state->ತಳ.components.latched_ಗುಂಪು -= priv.ಗುಂಪು_delta;
                        filter->ಕೀಲಿ = ಕೀಲಿ;
                        /* XXX beep beep! */
                        ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
                    }
                    /* Do nothing if `latchToLock` option is not activated; if
                     * the latch is not broken by the following actions and the
                     * key is not consumed, then another latch filter will be
                     * created.
                     */
                    ಮುಂದುವರಿ;
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (xkb_action_breaks_latch(&(actions[k]),
                                                 state->ತಳ.components.controls,
                                                 ಆಂತರಿಕ_BREAKS_ಗುಂಪು_LATCH,
                                                 0))
                {
                    /* Breaks the latch */
                    state->ತಳ.components.latched_ಗುಂಪು -= priv.ಗುಂಪು_delta;
                    filter->func = ಖಾಲಿ;
                    ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;
                }
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            /* Ignore press in NO_LATCH state */
            ಪ್ರತಿಜ್ಞಾ(latch == NO_LATCH);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕೀಲಿ != filter->ಕೀಲಿ) {
        /* Ignore repeat/release of other keys */
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (direction == XKB_ಕೀಲಿ_REPEATED) {
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME; // TODO: check
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        /* Our key got released.  If we've set it to clear locks, and we
         * currently have a group locked, then release it and
         * don't actually latch.  Else we've actually hit the latching
         * stage, so set PENDING and move our group from base to
         * latched. */
        ಆದರೆ ((filter->action.ಗುಂಪು.flags & ACTION_ಬೀಗ_CLEAR) &&
             state->ತಳ.components.locked_ಗುಂಪು) {
            ಆದರೆ (latch == LATCH_PENDING)
                state->ತಳ.components.latched_ಗುಂಪು -= priv.ಗುಂಪು_delta;
            ಇಲ್ಲದಿದ್ದರೆ
                state->ತಳ.components.ತಳ_ಗುಂಪು -= priv.ಗುಂಪು_delta;
            state->ತಳ.components.locked_ಗುಂಪು = 0;
            filter->func = ಖಾಲಿ;
        }
        /* Broken latch */
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (latch == NO_LATCH) {
            state->ತಳ.components.ತಳ_ಗುಂಪು -= priv.ಗುಂಪು_delta;
            filter->func = ಖಾಲಿ;
        }
        /* We may already have reached the latch state if pressing the
         * key multiple times without latch-to-lock enabled. */
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (latch == LATCH_ಕೀಲಿ_ಕೆಳಕ್ಕೆ) {
            latch = LATCH_PENDING;
            /* Switch from set to latch */
            state->ತಳ.components.ತಳ_ಗುಂಪು -= priv.ಗುಂಪು_delta;
            state->ತಳ.components.latched_ಗುಂಪು += priv.ಗುಂಪು_delta;
            /* XXX beep beep! */
        }
    }

    priv.latch = latch;
    filter->priv = priv.priv;

    ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_mod_ಗಣ_new(ರಚನೆ xkb_server_state *state,
                       ರಚನೆ xkb_events *events,
                       ರಚನೆ xkb_filter *filter)
{
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_ಬೀಗ_CLEAR;
    ಆದರೆ ((filter->action.mods.flags & unlock) == unlock) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension.
         */
        filter->priv = filter->action.mods.mods.ಮುಖವಾಡ
                     & ~state->ತಳ.components.locked_mods;
        state->ತಳ.components.locked_mods &= ~filter->action.mods.mods.ಮುಖವಾಡ;
    } ಇಲ್ಲದಿದ್ದರೆ {
        filter->priv = filter->action.mods.mods.ಮುಖವಾಡ;
    }

    state->ಗಣ_mods |= (xkb_mod_ಮುಖವಾಡ_t) filter->priv;
}

ಸ್ಥಿರ bool
xkb_filter_mod_ಗಣ_func(ರಚನೆ xkb_server_state *state,
                        ರಚನೆ xkb_events *events,
                        ರಚನೆ xkb_filter *filter,
                        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                        ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction)
{

    ಆದರೆ (ಕೀಲಿ != filter->ಕೀಲಿ) {
        filter->action.mods.flags &= ~ACTION_ಬೀಗ_CLEAR;
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;
    }

    ಆಯ್ಕೆ (direction) {
    ಸ್ಥಿತಿ XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ:
        filter->refcnt++;
        /* fallthrough */
    ಸ್ಥಿತಿ XKB_ಕೀಲಿ_REPEATED:
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಆದರೆ (--filter->refcnt > 0)
            ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    }

    state->clear_mods |= (xkb_mod_ಮುಖವಾಡ_t) filter->priv;
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_ಬೀಗ_CLEAR;
    ಆದರೆ ((filter->action.mods.flags & unlock) == ACTION_ಬೀಗ_CLEAR)
        state->ತಳ.components.locked_mods &= ~filter->action.mods.mods.ಮುಖವಾಡ;

    filter->func = ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_mod_ಬೀಗ_new(ರಚನೆ xkb_server_state *state,
                        ರಚನೆ xkb_events *events,
                        ರಚನೆ xkb_filter *filter)
{
    filter->priv = (state->ತಳ.components.locked_mods &
                    filter->action.mods.mods.ಮುಖವಾಡ);

    ಆದರೆ (filter->priv && (filter->action.mods.flags & ACTION_UNLOCK_ON_PRESS)) {
        /*
         * Some of the target modifiers were locked before key press: unlock.
         *
         * This is a keymap v2 extension: unlock-on-press.
         */
        ಆದರೆ (!(filter->action.mods.flags & ACTION_ಬೀಗ_NO_UNLOCK))
            state->ತಳ.components.locked_mods &= ~filter->priv;
        /* No further action: cancel filter */
        filter->func = ಖಾಲಿ;
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* Set base mods; lock mods if relevant (XKB 1.0 spec) */
        state->ಗಣ_mods |= filter->action.mods.mods.ಮುಖವಾಡ;
        ಆದರೆ (!(filter->action.mods.flags & ACTION_ಬೀಗ_NO_ಬೀಗ))
            state->ತಳ.components.locked_mods |= filter->action.mods.mods.ಮುಖವಾಡ;
    }
}

ಸ್ಥಿರ bool
xkb_filter_mod_ಬೀಗ_func(ರಚನೆ xkb_server_state *state,
                         ರಚನೆ xkb_events *events,
                         ರಚನೆ xkb_filter *filter,
                         ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                         ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction)
{
    ಆದರೆ (ಕೀಲಿ != filter->ಕೀಲಿ)
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;

    ಆಯ್ಕೆ (direction) {
    ಸ್ಥಿತಿ XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ:
        filter->refcnt++;
        /* fallthrough */
    ಸ್ಥಿತಿ XKB_ಕೀಲಿ_REPEATED:
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಆದರೆ (--filter->refcnt > 0)
            ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    }

    state->clear_mods |= filter->action.mods.mods.ಮುಖವಾಡ;
    ಆದರೆ (!(filter->action.mods.flags & ACTION_ಬೀಗ_NO_UNLOCK))
        state->ತಳ.components.locked_mods &= ~filter->priv;

    filter->func = ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_mod_latch_new(ರಚನೆ xkb_server_state *state,
                         ರಚನೆ xkb_events *events,
                         ರಚನೆ xkb_filter *filter)
{
    /* Latch-on-press + clear-locks imply unlock-on-press */
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                              | ACTION_LATCH_ON_PRESS;

    ಆದರೆ ((filter->action.mods.flags & ACTION_ಬೀಗ_CLEAR) &&
        (filter->action.mods.flags & unlockOnPress) &&
        (state->ತಳ.components.locked_mods & filter->action.mods.mods.ಮುಖವಾಡ) ==
         filter->action.mods.mods.ಮುಖವಾಡ) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension: clear locks and do not latch.
         */
        state->ತಳ.components.locked_mods &= ~filter->action.mods.mods.ಮುಖವಾಡ;
        filter->func = ಖಾಲಿ;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (filter->action.mods.flags & ACTION_LATCH_ON_PRESS) {
        /*
         * Latch on key press
         *
         * This is a keymap format v2 extension.
         */
        filter->priv = LATCH_PENDING;
        state->ತಳ.components.latched_mods |= filter->action.mods.mods.ಮುಖವಾಡ;
        /* XXX beep beep! */
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* XKB standard latch action */
        filter->priv = LATCH_ಕೀಲಿ_ಕೆಳಕ್ಕೆ;
        state->ಗಣ_mods |= filter->action.mods.mods.ಮುಖವಾಡ;
    }
}

ಸ್ಥಿರ bool
xkb_filter_mod_latch_func(ರಚನೆ xkb_server_state *state,
                          ರಚನೆ xkb_events *events,
                          ರಚನೆ xkb_filter *filter,
                          ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                          ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction)
{
    ಎಣಿಕೆ xkb_ಕೀಲಿ_latch_state latch = filter->priv;

    ಆದರೆ (direction == XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ) {
        ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action *actions = ಖಾಲಿ;
        ಸ್ಥಿರಾಂಕ xkb_action_ಎಣಿಕೆ_t ಎಣಿಕೆ_2 = xkb_ಕೀಲಿ_get_actions(&state->ತಳ, ಕೀಲಿ,
                                                             &actions);

        ಆದರೆ (latch == LATCH_ಕೀಲಿ_ಕೆಳಕ್ಕೆ) {
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
            ಆದರೆ (state->a11y_flags & XKB_A11_Y_LATCH_ಏಕಕಾಲಿಕ_ಕೀಲಿಗಳು) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                ಗಾಗಿ (xkb_action_ಎಣಿಕೆ_t k = 0; k < ಎಣಿಕೆ_2; k++) {
                    ಆದರೆ (xkb_action_breaks_latch(&(actions[k]),
                                                state->ತಳ.components.controls,
                                                ಆಂತರಿಕ_BREAKS_MOD_LATCH,
                                                filter->action.mods.mods.ಮುಖವಾಡ))
                    {
                        latch = NO_LATCH;
                        ನಿಲ್ಲಿಸು;
                    }
                }
            } ಇಲ್ಲದಿದ್ದರೆ {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is pressed,
             * or promote it to a lock or plain base set if it's the same
             * modifier. */
            ಸ್ಥಿರಾಂಕ bool sticky_ಕೀಲಿಗಳು = state->ತಳ.components.controls
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
            ಗಾಗಿ (xkb_action_ಎಣಿಕೆ_t k = 0; k < ಎಣಿಕೆ_2; k++) {
                ಆದರೆ (((actions[k].type == ACTION_TYPE_MOD_LATCH) ||
                     (actions[k].type == ACTION_TYPE_MOD_ಗಣ && sticky_ಕೀಲಿಗಳು)) &&
                    actions[k].mods.mods.ಮುಖವಾಡ == filter->action.mods.mods.ಮುಖವಾಡ)
                {
                    ಆದರೆ (filter->action.mods.flags & ACTION_LATCH_ಗಮ್ಯ_ಬೀಗ) {
                        /* Mutate the action to LockMods() */
                        filter->action.type = ACTION_TYPE_MOD_ಬೀಗ;
                        filter->func = xkb_filter_mod_ಬೀಗ_func;
                        xkb_filter_mod_ಬೀಗ_new(state, events, filter);
                    }
                    ಇಲ್ಲದಿದ್ದರೆ {
                        /* Mutate the action to SetMods() */
                        filter->action.type = ACTION_TYPE_MOD_ಗಣ;
                        filter->func = xkb_filter_mod_ಗಣ_func;
                        xkb_filter_mod_ಗಣ_new(state, events, filter);
                    }
                    filter->ಕೀಲಿ = ಕೀಲಿ;
                    /* Clear latches */
                    state->ತಳ.components.latched_mods &=
                        ~filter->action.mods.mods.ಮುಖವಾಡ;
                    /* XXX beep beep! */
                    ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (xkb_action_breaks_latch(&(actions[k]),
                                                 state->ತಳ.components.controls,
                                                 ಆಂತರಿಕ_BREAKS_MOD_LATCH,
                                                 filter->action.mods.mods.ಮುಖವಾಡ))
                {
                    /* XXX: This may be totally broken, we might need to break the
                     *      latch in the next run after this press? */
                    state->ತಳ.components.latched_mods &=
                        ~filter->action.mods.mods.ಮುಖವಾಡ;
                    filter->func = ಖಾಲಿ;
                    ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;
                }
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            /* Ignore press in NO_LATCH state */
            ಪ್ರತಿಜ್ಞಾ(latch == NO_LATCH);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕೀಲಿ != filter->ಕೀಲಿ) {
        /* Ignore repeat/release of other keys */
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (direction == XKB_ಕೀಲಿ_REPEATED) {
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME; // TODO: check
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        /* Our key got released.  If we’ve set it to clear locks, and we
         * currently have the same modifiers locked, then release them and
         * don't actually latch.  Else we’ve actually hit the latching
         * stage, so set PENDING and move our modifier from base to
         * latched. */

        /* Latch-on-press + clear-locks imply unlock-on-press */
        ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                                  | ACTION_LATCH_ON_PRESS;

        ಆದರೆ ((filter->action.mods.flags & ACTION_ಬೀಗ_CLEAR) &&
            !(filter->action.mods.flags & unlockOnPress) &&
            (state->ತಳ.components.locked_mods &
             filter->action.mods.mods.ಮುಖವಾಡ) == filter->action.mods.mods.ಮುಖವಾಡ)
        {
            /* XXX: We might be a bit overenthusiastic about clearing
             *      mods other filters have set here? */
            ಆದರೆ (latch == LATCH_PENDING)
                state->ತಳ.components.latched_mods &=
                    ~filter->action.mods.mods.ಮುಖವಾಡ;
            ಇಲ್ಲದಿದ್ದರೆ
                state->clear_mods |= filter->action.mods.mods.ಮುಖವಾಡ;
            state->ತಳ.components.locked_mods &= ~filter->action.mods.mods.ಮುಖವಾಡ;
            filter->func = ಖಾಲಿ;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (latch == NO_LATCH) {
            /* Broken latch */
            state->clear_mods |= filter->action.mods.mods.ಮುಖವಾಡ;
            filter->func = ಖಾಲಿ;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!(filter->action.mods.flags & ACTION_LATCH_ON_PRESS)) {
            latch = LATCH_PENDING;
            state->clear_mods |= filter->action.mods.mods.ಮುಖವಾಡ;
            state->ತಳ.components.latched_mods |= filter->action.mods.mods.ಮುಖವಾಡ;
            /* XXX beep beep! */
        }
    }

    filter->priv = latch;

    ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;
}

ಸ್ಥಿರ ಶೂನ್ಯ
append_ಸೂಚಕ_move(ರಚನೆ xkb_events *events,
                    ರಚನೆ xkb_filter *filter)
{
    ಎಣಿಕೆ xkb_ಸೂಚಕ_motion_flags flags = 0;
    ಆದರೆ (filter->action.ptr.flags & ACTION_ABSOLUTE_X)
        flags |= XKB_ಸೂಚಕ_MOTION_ABSOLUTE_X;
    ಆದರೆ (filter->action.ptr.flags & ACTION_ABSOLUTE_Y)
        flags |= XKB_ಸೂಚಕ_MOTION_ABSOLUTE_Y;
    ಆದರೆ (filter->action.ptr.flags & ACTION_REPEAT)
        flags |= XKB_ಸೂಚಕ_MOTION_REPEATS;
    darray_append(events->ಸರತಿ, (ರಚನೆ xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_ಸೂಚಕ_MOTION,
        .ಸೂಚಕ_motion = {
            .ಗಾತ್ರ_2 = ಗಾತ್ರ(((ರಚನೆ xkb_event*)0)->ಸೂಚಕ_motion),
            .flags = flags,
            .x = filter->action.ptr.x,
            .y = filter->action.ptr.y,
        }
    });
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_ಸೂಚಕ_move_new(ರಚನೆ xkb_server_state *state,
                            ರಚನೆ xkb_events *events,
                            ರಚನೆ xkb_filter *filter)
{
    ಆದರೆ (!events) {
        filter->func = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ;
    }

    ಪ್ರತಿಜ್ಞಾ(state->ತಳ.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ಕೀಲಿಗಳು);
    append_ಸೂಚಕ_move(events, filter);
    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;
}

ಸ್ಥಿರ bool
xkb_filter_ಸೂಚಕ_move_func(ರಚನೆ xkb_server_state *state,
                             ರಚನೆ xkb_events *events,
                             ರಚನೆ xkb_filter *filter,
                             ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                             ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction)
{
    ಆದರೆ (ಕೀಲಿ != filter->ಕೀಲಿ || !events)
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;

    ಆಯ್ಕೆ (direction) {
    ಸ್ಥಿತಿ XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ:
        filter->refcnt++;
        append_ಸೂಚಕ_move(events, filter);
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    ಸ್ಥಿತಿ XKB_ಕೀಲಿ_REPEATED:
        ಆದರೆ (filter->action.ptr.flags & ACTION_REPEAT)
            append_ಸೂಚಕ_move(events, filter);
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಆದರೆ (--filter->refcnt > 0)
            ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    }

    filter->func = ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
}

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_ಸೂಚಕ_button_new(ರಚನೆ xkb_server_state *state,
                              ರಚನೆ xkb_events *events,
                              ರಚನೆ xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ಸ್ಥಿರ bool
xkb_filter_ಸೂಚಕ_button_func(ರಚನೆ xkb_server_state *state,
                               ರಚನೆ xkb_events *events,
                               ರಚನೆ xkb_filter *filter,
                               ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                               ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_ಸೂಚಕ_ಬೀಗ_button_new(ರಚನೆ xkb_server_state *state,
                                   ರಚನೆ xkb_events *events,
                                   ರಚನೆ xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ಸ್ಥಿರ bool
xkb_filter_ಸೂಚಕ_ಬೀಗ_button_func(ರಚನೆ xkb_server_state *state,
                                    ರಚನೆ xkb_events *events,
                                    ರಚನೆ xkb_filter *filter,
                                    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                                    ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_ಸೂಚಕ_ಗಣ_default_button_new(ರಚನೆ xkb_server_state *state,
                                          ರಚನೆ xkb_events *events,
                                          ರಚನೆ xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ಸ್ಥಿರ bool
xkb_filter_ಸೂಚಕ_ಗಣ_default_button_func(ರಚನೆ xkb_server_state *state,
                                           ರಚನೆ xkb_events *events,
                                           ರಚನೆ xkb_filter *filter,
                                           ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                                           ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction);

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_terminate_server_new(ರಚನೆ xkb_server_state *state,
                                 ರಚನೆ xkb_events *events,
                                 ರಚನೆ xkb_filter *filter);

ಸ್ಥಿರ bool
xkb_filter_terminate_server_func(ರಚನೆ xkb_server_state *state,
                                  ರಚನೆ xkb_events *events,
                                  ರಚನೆ xkb_filter *filter,
                                  ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                                  ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_switch_virtual_console_new(ರಚನೆ xkb_server_state *state,
                                      ರಚನೆ xkb_events *events,
                                      ರಚನೆ xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ಸ್ಥಿರ bool
xkb_filter_switch_virtual_console_func(ರಚನೆ xkb_server_state *state,
                                       ರಚನೆ xkb_events *events,
                                       ರಚನೆ xkb_filter *filter,
                                       ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                                       ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
clear_all_latches_and_locks(ರಚನೆ xkb_server_state *state,
                            ರಚನೆ xkb_events *events);

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_ctrls_new(ರಚನೆ xkb_server_state *state,
                     ರಚನೆ xkb_events *events,
                     ರಚನೆ xkb_filter *filter)
{
    ಆದರೆ (filter->action.type == ACTION_TYPE_CTRL_ಗಣ) {
        /* Set: save the specified controls that are *not* already enabled */
        filter->priv = (uint32_t) (
            (~state->ತಳ.components.controls) & filter->action.ctrls.ctrls
        );
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* Lock: save the specified controls that *are* already enabled */
        filter->priv = (uint32_t) (
            state->ತಳ.components.controls & filter->action.ctrls.ctrls
        );
    }

    ಆದರೆ (filter->action.type == ACTION_TYPE_CTRL_ಗಣ ||
        !(filter->action.ctrls.flags & ACTION_ಬೀಗ_NO_ಬೀಗ)) {
        /* Enable the specified controls that are not already enabled */
        state->ತಳ.components.controls |= filter->action.ctrls.ctrls;
    }
}

ಸ್ಥಿರ bool
xkb_filter_ctrls_func(ರಚನೆ xkb_server_state *state,
                      ರಚನೆ xkb_events *events,
                      ರಚನೆ xkb_filter *filter,
                      ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                      ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction)
{
    ಆದರೆ (ಕೀಲಿ != filter->ಕೀಲಿ)
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;

    ಆಯ್ಕೆ (direction) {
    ಸ್ಥಿತಿ XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ:
        filter->refcnt++;
        /* fallthrough */
    ಸ್ಥಿತಿ XKB_ಕೀಲಿ_REPEATED:
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಆದರೆ (--filter->refcnt > 0)
            ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    }

    ಆದರೆ (filter->action.type == ACTION_TYPE_CTRL_ಗಣ ||
        !(filter->action.ctrls.flags & ACTION_ಬೀಗ_NO_UNLOCK)) {
        ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_action_controls old = state->ತಳ.components.controls;

        /*
         * Set: Disable specified controls that were *not* enabled at key press.
         * Lock: Disable specified controls that *were* enabled at key press.
         */
        state->ತಳ.components.controls &= ~(ಎಣಿಕೆ xkb_action_controls) filter->priv;

        ಆದರೆ ((old & CONTROL_STICKY_ಕೀಲಿಗಳು) &&
            !(state->ತಳ.components.controls & CONTROL_STICKY_ಕೀಲಿಗಳು)) {
            /* Sticky keys were disabled: clear all locks and latches */
            clear_all_latches_and_locks(state, events);
        }
    }

    filter->func = ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;
}

ಸ್ಥಿರ bool
append_redirect_ಕೀಲಿ_events(ರಚನೆ xkb_server_state *state,
                           ರಚನೆ xkb_events *events,
                           ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_redirect_ಕೀಲಿ_action *redirect,
                           ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction)
{
    ಎಣಿಕೆ xkb_state_component ಬದಲಾಯಿತು = 0;
    ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t ಮುಖವಾಡ = redirect->affect_mods;

    /*
     * Reference state: find the last state update in the queue, otherwise
     * use the current state.
     */
    ರಚನೆ xkb_event *event;
    ರಚನೆ state_components ಕೊನೆಯ_components = state->ತಳ.components;
    darray_foreach_ಹಿಮ್ಮುಖ(event, events->ಸರತಿ) {
        ಆದರೆ (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
            ಕೊನೆಯ_components = event->components.components;
            ನಿಲ್ಲಿಸು;
        }
    }

    ಆದರೆ (ಮುಖವಾಡ) {
        ರಚನೆ state_components new = ಕೊನೆಯ_components;
        new.ತಳ_mods = (new.ತಳ_mods & ~ಮುಖವಾಡ) | redirect->mods;
        new.latched_mods = (new.latched_mods & ~ಮುಖವಾಡ) | redirect->mods;
        new.locked_mods = (new.locked_mods & ~ಮುಖವಾಡ) | redirect->mods;
        new.mods = (new.mods & ~ಮುಖವಾಡ) | redirect->mods;

        ಬದಲಾಯಿತು = get_state_component_changes(&ಕೊನೆಯ_components, &new);
        ಆದರೆ (ಬದಲಾಯಿತು) {
            darray_append(events->ಸರತಿ, (ರಚನೆ xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = new,
                    .ಬದಲಾಯಿತು = ಬದಲಾಯಿತು
                }
            });
            /*
             * Commit the temporary modifiers change before the key event
             * it was computed for: consumers must observe the key event
             * under the *redirected* modifiers.
             */
            darray_append(events->ಸರತಿ, (ರಚನೆ xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_ಚೌಕಟ್ಟು,
            });
        }
    }

    darray_append(events->ಸರತಿ, (ರಚನೆ xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_ಕೀಲಿ,
        .ಕೀಲಿ = {
            .keycode = redirect->keycode,
            .direction = direction
        }
    });

    ಆದರೆ (ಮುಖವಾಡ && ಬದಲಾಯಿತು) {
        /* Restore state */
        darray_append(events->ಸರತಿ, (ರಚನೆ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = ಕೊನೆಯ_components,
                .ಬದಲಾಯಿತು = ಬದಲಾಯಿತು
            }
        });
    }

    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_redirect_ಕೀಲಿ_new(ರಚನೆ xkb_server_state *state,
                            ರಚನೆ xkb_events *events,
                            ರಚನೆ xkb_filter *filter)
{
    /* Action effectual only with the xkb_machine API and a valid keycode */
    ಆದರೆ (!events || filter->action.redirect.keycode == XKB_KEYCODE_INVALID) {
        filter->func = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ;
    }
    append_redirect_ಕೀಲಿ_events(state, events, &filter->action.redirect,
                               XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ);
}

ಸ್ಥಿರ bool
xkb_filter_redirect_ಕೀಲಿ_func(ರಚನೆ xkb_server_state *state,
                             ರಚನೆ xkb_events *events,
                             ರಚನೆ xkb_filter *filter,
                             ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                             ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction)
{
    ಆದರೆ (ಕೀಲಿ != filter->ಕೀಲಿ)
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;

    ಆದರೆ (direction == XKB_ಕೀಲಿ_ಮೇಲೆ) {
        append_redirect_ಕೀಲಿ_events(state, events,
                                   &filter->action.redirect, XKB_ಕೀಲಿ_ಮೇಲೆ);
        filter->func = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
     } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (direction == XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ) {
        ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action *actions = ಖಾಲಿ;
        ಸ್ಥಿರಾಂಕ xkb_action_ಎಣಿಕೆ_t ಎಣಿಕೆ_2 =
            xkb_ಕೀಲಿ_get_actions(&state->ತಳ, ಕೀಲಿ, &actions);

        ಗಾಗಿ (xkb_action_ಎಣಿಕೆ_t a = 0; a < ಎಣಿಕೆ_2; a++) {
            ಆದರೆ (actions[a].type == ACTION_TYPE_REDIRECT_ಕೀಲಿ &&
                actions[a].redirect.keycode != filter->action.redirect.keycode) {
                    /* One action redirects to a different key: release first */
                    append_redirect_ಕೀಲಿ_events(state, events,
                                               &filter->action.redirect,
                                               XKB_ಕೀಲಿ_ಮೇಲೆ);
                filter->func = ಖಾಲಿ;
                ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;
            }
        }
    }

    append_redirect_ಕೀಲಿ_events(state, events, &filter->action.redirect,
                               direction);
    ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ {
    ಶೂನ್ಯ (*new)(ರಚನೆ xkb_server_state *state,
                ರಚನೆ xkb_events *events,
                ರಚನೆ xkb_filter *filter);
    bool (*func)(ರಚನೆ xkb_server_state *state,
                 ರಚನೆ xkb_events *events,
                 ರಚನೆ xkb_filter *filter,
                 ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ, ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction);
} filter_action_funcs[ACTION_TYPE_ಅಂಕೆ_ENTRIES] = {
    [ACTION_TYPE_MOD_ಗಣ]     = { xkb_filter_mod_ಗಣ_new,
                                  xkb_filter_mod_ಗಣ_func },
    [ACTION_TYPE_MOD_LATCH]   = { xkb_filter_mod_latch_new,
                                  xkb_filter_mod_latch_func },
    [ACTION_TYPE_MOD_ಬೀಗ]    = { xkb_filter_mod_ಬೀಗ_new,
                                  xkb_filter_mod_ಬೀಗ_func },
    [ACTION_TYPE_ಗುಂಪು_ಗಣ]   = { xkb_filter_ಗುಂಪು_ಗಣ_new,
                                  xkb_filter_ಗುಂಪು_ಗಣ_func },
    [ACTION_TYPE_ಗುಂಪು_LATCH] = { xkb_filter_ಗುಂಪು_latch_new,
                                  xkb_filter_ಗುಂಪು_latch_func },
    [ACTION_TYPE_ಗುಂಪು_ಬೀಗ]  = { xkb_filter_ಗುಂಪು_ಬೀಗ_new,
                                  xkb_filter_ಗುಂಪು_ಬೀಗ_func },
    [ACTION_TYPE_PTR_MOVE]    = { xkb_filter_ಸೂಚಕ_move_new,
                                  xkb_filter_ಸೂಚಕ_move_func },
    [ACTION_TYPE_PTR_BUTTON]  = { xkb_filter_ಸೂಚಕ_button_new,
                                  xkb_filter_ಸೂಚಕ_button_func },
    [ACTION_TYPE_PTR_ಬೀಗ]    = { xkb_filter_ಸೂಚಕ_ಬೀಗ_button_new,
                                  xkb_filter_ಸೂಚಕ_ಬೀಗ_button_func },
    [ACTION_TYPE_PTR_DEFAULT] = { xkb_filter_ಸೂಚಕ_ಗಣ_default_button_new,
                                  xkb_filter_ಸೂಚಕ_ಗಣ_default_button_func },
    [ACTION_TYPE_TERMINATE]   = { xkb_filter_terminate_server_new,
                                  xkb_filter_terminate_server_func },
    [ACTION_TYPE_SWITCH_VT]   = { xkb_filter_switch_virtual_console_new,
                                  xkb_filter_switch_virtual_console_func },
    [ACTION_TYPE_CTRL_ಗಣ]    = { xkb_filter_ctrls_new,
                                  xkb_filter_ctrls_func },
    [ACTION_TYPE_CTRL_ಬೀಗ]   = { xkb_filter_ctrls_new,
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
xkb_filter_apply_all(ರಚನೆ xkb_server_state *state,
                     ರಚನೆ xkb_events *events,
                     ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                     ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction)
{
    /* First run through all the currently active filters and see if any of
     * them have consumed this event. */
    bool consumed = false;
    ರಚನೆ xkb_filter *filter;
    darray_foreach(filter, state->filters) {
        ಆದರೆ (!filter->func)
            ಮುಂದುವರಿ;

        ಆದರೆ (filter->func(state, events, filter, ಕೀಲಿ, direction) == XKB_FILTER_CONSUME)
            consumed = true;
    }
    ಆದರೆ (consumed || direction == XKB_ಕೀಲಿ_ಮೇಲೆ)
        ಹಿಂತಿರುಗಿ;

    /* No filter consumed this event, so proceed with the key actions */
    ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action *actions = ಖಾಲಿ;
    xkb_action_ಎಣಿಕೆ_t ಎಣಿಕೆ_2 = xkb_ಕೀಲಿ_get_actions(&state->ತಳ, ಕೀಲಿ,
                                                         &actions);

    ಗಾಗಿ (ರಚನೆ {
            xkb_action_ಎಣಿಕೆ_t ಎಣಿಕೆ_2;
            ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action *actions;
         } ಮುಂದಿನ = {0};
         ಎಣಿಕೆ_2;
         ಎಣಿಕೆ_2 = ಮುಂದಿನ.ಎಣಿಕೆ_2, ಮುಂದಿನ.ಎಣಿಕೆ_2 = 0, actions = ಮುಂದಿನ.actions
    ) {
        /*
         * Process actions sequentially.
         *
         * NOTE: We rely on the parser to disallow multiple modifier or group
         * actions (see `CheckMultipleActionsCategories`). Allowing multiple
         * such actions requires a refactor of the state handling.
         */
        ಗಾಗಿ (xkb_action_ಎಣಿಕೆ_t k = 0; k < ಎಣಿಕೆ_2; k++) {
            /*
             * It's possible for the keymap to set action->type explicitly, like
             * so:
             *     interpret XF86_Next_VMode {
             *         action = Private(type=0x86, data="+VMode");
             *     };
             * We don't handle those.
             */
            ಆದರೆ (actions[k].type >= ACTION_TYPE_ಅಂಕೆ_ENTRIES)
                ಮುಂದುವರಿ;

            /* Go to next action if no corresponding action handler */
            ಆದರೆ (!filter_action_funcs[actions[k].type].new)
                ಮುಂದುವರಿ;

            /* Add a new filter and run the corresponding initial action */
            filter = xkb_filter_new(state);
            filter->ಕೀಲಿ = ಕೀಲಿ;
            filter->action = actions[k];

            ಆಯ್ಕೆ (filter->action.type) {
            ಸ್ಥಿತಿ ACTION_TYPE_MOD_ಗಣ:
                ಆದರೆ (state->ತಳ.components.controls & CONTROL_STICKY_ಕೀಲಿಗಳು) {
                    /* Convert modifier set action to a latch */
                    filter->action.type = ACTION_TYPE_MOD_LATCH;
                    ಆದರೆ (state->a11y_flags & XKB_A11_Y_STICKY_ಕೀಲಿಗಳು_LATCH_ಗಮ್ಯ_ಬೀಗ) {
                        filter->action.mods.flags |= ACTION_LATCH_ಗಮ್ಯ_ಬೀಗ;
                    }
                }
                /* fallthrough */
            ಸ್ಥಿತಿ ACTION_TYPE_MOD_LATCH:
            ಸ್ಥಿತಿ ACTION_TYPE_MOD_ಬೀಗ: {
                ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_action_flags unlock_mods_on_press
                    = ACTION_ಬೀಗ_CLEAR     /* SetMods & LatchMods: can unlock */
                    | ACTION_ಬೀಗ_NO_UNLOCK /* LockMods: cannot unlock */
                    | ACTION_EXPLICIT_UNLOCK_ON_PRESS;
                ಆದರೆ ((state->flags & STATE_UNLOCK_MODS_ON_PRESS) &&
                    /*
                     * Ensure that the action can actually unlock and has
                     * no *explicit* `unlockOnPress` parameter.
                     */
                    (filter->action.mods.flags & unlock_mods_on_press) ==
                    (filter->action.type == ACTION_TYPE_MOD_ಬೀಗ
                        ? 0 : ACTION_ಬೀಗ_CLEAR))
                {
                    filter->action.mods.flags |= ACTION_UNLOCK_ON_PRESS;
                }
                ನಿಲ್ಲಿಸು;
            }
            ಸ್ಥಿತಿ ACTION_TYPE_ಗುಂಪು_ಗಣ:
                ಆದರೆ (state->ತಳ.components.controls & CONTROL_STICKY_ಕೀಲಿಗಳು) {
                    /* Convert group set action to a latch */
                    filter->action.type = ACTION_TYPE_ಗುಂಪು_LATCH;
                    ಆದರೆ (state->a11y_flags & XKB_A11_Y_STICKY_ಕೀಲಿಗಳು_LATCH_ಗಮ್ಯ_ಬೀಗ) {
                        filter->action.ಗುಂಪು.flags |= ACTION_LATCH_ಗಮ್ಯ_ಬೀಗ;
                    }
                }
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ ACTION_TYPE_ಗುಂಪು_ಬೀಗ:
                ಆದರೆ ((filter->action.ಗುಂಪು.flags &
                     ACTION_AUTO_ಗುಂಪು_ಬೀಗ_ON_MODIFIER_RELEASE) &&
                    (state->flags &
                     STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_ಕೀಲಿ) &&
                    /*
                     * FIXME: sticky keys may leave an unexpected modifier
                     * from the level 0 latched, so disable for now.
                     */
                    !(state->ತಳ.components.controls & CONTROL_STICKY_ಕೀಲಿಗಳು))
                {
                    filter->action.ಗುಂಪು.flags |= ACTION_ಬೀಗ_ON_RELEASE;
                    /* Add base level action */
                    ಮುಂದಿನ.ಎಣಿಕೆ_2 = xkb_keymap_ಕೀಲಿ_get_actions_by_level(
                        state->ತಳ.keymap, ಕೀಲಿ, state->ತಳ.components.ಗುಂಪು,
                        0, &ಮುಂದಿನ.actions
                    );
                    ಪ್ರತಿಜ್ಞಾ(ಮುಂದಿನ.ಎಣಿಕೆ_2 == 1);
                }
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ ACTION_TYPE_PTR_MOVE:
            ಸ್ಥಿತಿ ACTION_TYPE_PTR_BUTTON:
            ಸ್ಥಿತಿ ACTION_TYPE_PTR_ಬೀಗ:
            ಸ್ಥಿತಿ ACTION_TYPE_PTR_DEFAULT:
                ಆದರೆ (!(state->ತಳ.components.controls & CONTROL_MOUSE_ಕೀಲಿಗಳು)) {
                    /* Convert pointer actions to NoAction */
                    ಪ್ರತಿಜ್ಞಾ(!filter_action_funcs[ACTION_TYPE_NONE].new);
                    filter->func = ಖಾಲಿ;
                    ಮುಂದುವರಿ;
                }
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ ACTION_TYPE_REDIRECT_ಕೀಲಿ:
                // FIXME: this is not efficient to resolve mods here each time
                filter->action.redirect.affect_mods = mod_ಮುಖವಾಡ_get_effective(
                    state->ತಳ.keymap, filter->action.redirect.affect_mods
                );
                filter->action.redirect.mods = mod_ಮುಖವಾಡ_get_effective(
                    state->ತಳ.keymap, filter->action.redirect.mods
                );
                ನಿಲ್ಲಿಸು;
            ಪೂರ್ವನಿಯೋಜಿತ: ;
            }
            filter->func = filter_action_funcs[filter->action.type].func;
            filter_action_funcs[filter->action.type].new(state, events, filter);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_state_update_derived(ರಚನೆ xkb_state *state);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
xkb_state_init(ರಚನೆ xkb_state * ಸೀಮಿತ state,
               ರಚನೆ xkb_keymap * ಸೀಮಿತ keymap,
               ಎಣಿಕೆ xkb_state_ಬಹುಲಕ_ಆಂತರಿಕ ಬಹುಲಕ)
{
    state->ಬಹುಲಕ = ಬಹುಲಕ;
    state->refcnt = 1;
    state->keymap = xkb_keymap_ref(keymap);
    state->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು.policy = XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_WRAP;

    /* Ensure that derived state is correctly initialized */
    xkb_state_update_derived(state);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
xkb_client_state_init(ರಚನೆ xkb_client_state * ಸೀಮಿತ state,
                      ರಚನೆ xkb_keymap * ಸೀಮಿತ keymap,
                      ಎಣಿಕೆ xkb_state_ಬಹುಲಕ_ಆಂತರಿಕ ಬಹುಲಕ)
{
    xkb_state_init(&state->ತಳ, keymap, ಬಹುಲಕ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
xkb_server_state_init(ರಚನೆ xkb_server_state * ಸೀಮಿತ state,
                      ರಚನೆ xkb_keymap * ಸೀಮಿತ keymap,
                      ಎಣಿಕೆ xkb_state_ಬಹುಲಕ_ಆಂತರಿಕ ಬಹುಲಕ,
                      ಎಣಿಕೆ xkb_a11y_flags a11y_affect,
                      ಎಣಿಕೆ xkb_a11y_flags a11y_flags)
{
    xkb_state_init(&state->ತಳ, keymap, ಬಹುಲಕ);

    state->a11y_flags = a11y_flags;
    ಆದರೆ (keymap->format != XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1 &&
        !(a11y_affect & XKB_A11_Y_LATCH_ಏಕಕಾಲಿಕ_ಕೀಲಿಗಳು)) {
             /* Keymap v2+: enable extension to XKB if not manually disabled */
             state->a11y_flags |= XKB_A11_Y_LATCH_ಏಕಕಾಲಿಕ_ಕೀಲಿಗಳು;
    }
}

ರಚನೆ xkb_state *
xkb_state_new(ರಚನೆ xkb_keymap *keymap)
{
    ರಚನೆ xkb_server_state * ಸ್ಥಿರಾಂಕ state = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(*state));
    ಆದರೆ (!state) {
        log_err_func1(keymap->ctx, XKB_ERROR_ಹಂಚಿಕೆ_FAILURE_2,
                      "Could not allocate a state object.\n");
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    xkb_server_state_init(state, keymap, LEGACY_MIXED_STATE, 0, 0);

    ಹಿಂತಿರುಗಿ (ರಚನೆ xkb_state *)state;
}

ರಚನೆ xkb_state *
xkb_state_new_with_ಬಹುಲಕ(ರಚನೆ xkb_keymap * ಸೀಮಿತ keymap,
                        ಎಣಿಕೆ xkb_state_ಬಹುಲಕ ಬಹುಲಕ,
                        ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ * ಸೀಮಿತ ಸ್ಥಿತಿ_2)
{
    ಆಯ್ಕೆ (ಬಹುಲಕ) {
    ಸ್ಥಿತಿ XKB_STATE_ಬಹುಲಕ_CLIENT:
    ಸ್ಥಿತಿ XKB_STATE_ಬಹುಲಕ_SERVER_QUERY: {
        ರಚನೆ xkb_client_state * ಸ್ಥಿರಾಂಕ state = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(*state));
        ಆದರೆ (!state)
            ನಿಲ್ಲಿಸು;

        static_assert((ಚಿಹ್ನೆರಹಿತ)XKB_STATE_ಬಹುಲಕ_CLIENT ==
                      (ಚಿಹ್ನೆರಹಿತ)CLIENT_STATE, "");
        static_assert((ಚಿಹ್ನೆರಹಿತ)XKB_STATE_ಬಹುಲಕ_SERVER_QUERY ==
                      (ಚಿಹ್ನೆರಹಿತ)SERVER_COMPANION, "");
        xkb_client_state_init(state, keymap, (ಎಣಿಕೆ xkb_state_ಬಹುಲಕ_ಆಂತರಿಕ)ಬಹುಲಕ);

        ಆದರೆ (ಸ್ಥಿತಿ_2)
            *ಸ್ಥಿತಿ_2 = XKB_SUCCESS;

        ಹಿಂತಿರುಗಿ (ರಚನೆ xkb_state *)state;
    }
    ಸ್ಥಿತಿ XKB_STATE_ಬಹುಲಕ_SERVER: {
        ರಚನೆ xkb_server_state * ಸ್ಥಿರಾಂಕ state = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(*state));
        ಆದರೆ (!state)
            ನಿಲ್ಲಿಸು;

        xkb_server_state_init(state, keymap, LEGACY_SERVER_STATE, 0, 0);

        ಆದರೆ (ಸ್ಥಿತಿ_2)
            *ಸ್ಥಿತಿ_2 = XKB_SUCCESS;

        ಹಿಂತಿರುಗಿ (ರಚನೆ xkb_state *)state;
    }
    ಪೂರ್ವನಿಯೋಜಿತ:
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_STATE_ಬಹುಲಕ_2,
                     "Unsupported state mode: %u\n", ಬಹುಲಕ);
        ಆದರೆ (ಸ್ಥಿತಿ_2)
            *ಸ್ಥಿತಿ_2 = XKB_ERROR_UNSUPPORTED_STATE_ಬಹುಲಕ;
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    log_err_func1(keymap->ctx, XKB_ERROR_ಹಂಚಿಕೆ_FAILURE_2,
                  "Could not allocate a state object.\n");
    ಆದರೆ (ಸ್ಥಿತಿ_2)
        *ಸ್ಥಿತಿ_2 = XKB_ERROR_ಹಂಚಿಕೆ_FAILURE;
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ರಚನೆ xkb_state *
xkb_state_ref(ರಚನೆ xkb_state *state)
{
    ಪ್ರತಿಜ್ಞಾ(state->refcnt > 0);
    state->refcnt++;
    ಹಿಂತಿರುಗಿ state;
}

ಶೂನ್ಯ
xkb_state_reset(ರಚನೆ xkb_state *state)
{
    memset(&state->components, 0, ಗಾತ್ರ(state->components));

    ಆದರೆ (state->ಬಹುಲಕ < LEGACY_MIXED_STATE) {
        static_assert(ಗಾತ್ರ(*state) == ಗಾತ್ರ(ರಚನೆ xkb_client_state),
                      "Not a mere wrapper");
        /* Nothing else to reset */
    } ಇಲ್ಲದಿದ್ದರೆ {
        ರಚನೆ xkb_server_state *state_ = (ರಚನೆ xkb_server_state *)state;
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_server_state prev = *state_;
        memset(state_, 0, ಗಾತ್ರ(*state_));

        state_->ತಳ = prev.ತಳ;
        state_->a11y_flags = prev.a11y_flags;
        state_->filters = prev.filters;
        darray_ಗಾತ್ರ(state_->filters) = 0;
    }
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
xkb_state_destroy(ರಚನೆ xkb_state *state)
{
    xkb_keymap_unref(state->keymap);
    ಆದರೆ (state->ಬಹುಲಕ > SERVER_COMPANION)
        darray_free(((ರಚನೆ xkb_server_state *)state)->filters);
}

ಶೂನ್ಯ
xkb_state_unref(ರಚನೆ xkb_state *state)
{
    ಪ್ರತಿಜ್ಞಾ(!state || state->refcnt > 0);
    ಆದರೆ (!state || --state->refcnt > 0)
        ಹಿಂತಿರುಗಿ;

    xkb_state_destroy(state);

    ಆಯ್ಕೆ (state->ಬಹುಲಕ) {
    ಸ್ಥಿತಿ CLIENT_STATE:
    ಸ್ಥಿತಿ SERVER_COMPANION:
        ಬಿಡುಗಡೆ((ರಚನೆ xkb_client_state *)state);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ LEGACY_MIXED_STATE:
    ಸ್ಥಿತಿ LEGACY_SERVER_STATE:
        ಬಿಡುಗಡೆ((ರಚನೆ xkb_server_state *)state);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ SERVER_STATE: /* handled by xkb_machine_unref() */
    ಪೂರ್ವನಿಯೋಜಿತ:
        PANIC_UNREACHABLE();
    }
}

ರಚನೆ xkb_keymap *
xkb_state_get_keymap(ರಚನೆ xkb_state *state)
{
    ಹಿಂತಿರುಗಿ state->keymap;
}

/**
 * Update the LED state to match the rest of the xkb_state.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
xkb_state_led_update_all(ರಚನೆ xkb_state *state)
{
    xkb_led_ಸೂಚ್ಯಂಕ_t idx;
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_led *led;

    state->components.leds = 0;

    xkb_leds_enumerate(idx, led, state->keymap) {

        ಆದರೆ (led->which_mods != 0 && led->mods.ಮುಖವಾಡ != 0) {
            xkb_mod_ಮುಖವಾಡ_t mod_ಮುಖವಾಡ = 0;
            ಆದರೆ (led->which_mods & XKB_STATE_MODS_EFFECTIVE)
                mod_ಮುಖವಾಡ |= state->components.mods;
            ಆದರೆ (led->which_mods & XKB_STATE_MODS_DEPRESSED)
                mod_ಮುಖವಾಡ |= state->components.ತಳ_mods;
            ಆದರೆ (led->which_mods & XKB_STATE_MODS_LATCHED)
                mod_ಮುಖವಾಡ |= state->components.latched_mods;
            ಆದರೆ (led->which_mods & XKB_STATE_MODS_LOCKED)
                mod_ಮುಖವಾಡ |= state->components.locked_mods;

            ಆದರೆ (led->mods.ಮುಖವಾಡ & mod_ಮುಖವಾಡ) {
                state->components.leds |= (UINT32_C(1) << idx);
                ಮುಂದುವರಿ;
            }
        }

        ಆದರೆ (led->which_groups != 0) {
            ಆದರೆ (likely(led->groups) != 0) {
                xkb_layout_ಮುಖವಾಡ_t ಗುಂಪು_ಮುಖವಾಡ = 0;
                /* Effective and locked groups have been brought into range */
                ಪ್ರತಿಜ್ಞಾ(state->components.ಗುಂಪು < XKB_MAX_GROUPS);
                ಪ್ರತಿಜ್ಞಾ(state->components.locked_ಗುಂಪು >= 0 &&
                       state->components.locked_ಗುಂಪು < XKB_MAX_GROUPS);
                /* Effective and locked groups are used as mask */
                ಆದರೆ (led->which_groups & XKB_STATE_LAYOUT_EFFECTIVE)
                    ಗುಂಪು_ಮುಖವಾಡ |= (UINT32_C(1) << state->components.ಗುಂಪು);
                ಆದರೆ (led->which_groups & XKB_STATE_LAYOUT_LOCKED)
                    ಗುಂಪು_ಮುಖವಾಡ |= (UINT32_C(1) << state->components.locked_ಗುಂಪು);
                /* Base and latched groups only have to be non-zero */
                ಆದರೆ ((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                    state->components.ತಳ_ಗುಂಪು != 0)
                    ಗುಂಪು_ಮುಖವಾಡ |= led->groups;
                ಆದರೆ ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                    state->components.latched_ಗುಂಪು != 0)
                    ಗುಂಪು_ಮುಖವಾಡ |= led->groups;

                ಆದರೆ (led->groups & ಗುಂಪು_ಮುಖವಾಡ) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    ಮುಂದುವರಿ;
                }
            } ಇಲ್ಲದಿದ್ದರೆ {
                /* Special case for Base and latched groups */
                ಆದರೆ (((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                     state->components.ತಳ_ಗುಂಪು == 0) ||
                    ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                     state->components.latched_ಗುಂಪು == 0)) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    ಮುಂದುವರಿ;
                }
            }
        }

        ಆದರೆ (led->ctrls & state->components.controls) {
            state->components.leds |= (UINT32_C(1) << idx);
            ಮುಂದುವರಿ;
        }
    }
}

/**
 * Calculates the derived state (effective mods/group and LEDs) from an
 * up-to-date xkb_state.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
xkb_state_update_derived(ರಚನೆ xkb_state *state)
{
    xkb_layout_ಸೂಚ್ಯಂಕ_t wrapped;

    state->components.mods = (state->components.ತಳ_mods |
                              state->components.latched_mods |
                              state->components.locked_mods);

    /* Lock group must be adjusted, but not base nor latched groups */
    wrapped = Xkb_Wrap_ಗುಂಪು_Into_Range(state->components.locked_ಗುಂಪು,
                                    state->keymap->ಅಂಕೆ_groups,
                                    state->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು.policy,
                                    state->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು.redirect_ಗುಂಪು);
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->components.locked_ಗುಂಪು =
        (int32_t) (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    /* Effective group must be adjusted */
    wrapped = Xkb_Wrap_ಗುಂಪು_Into_Range(state->components.ತಳ_ಗುಂಪು +
                                    state->components.latched_ಗುಂಪು +
                                    state->components.locked_ಗುಂಪು,
                                    state->keymap->ಅಂಕೆ_groups,
                                    state->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು.policy,
                                    state->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು.redirect_ಗುಂಪು);
    state->components.ಗುಂಪು =
        (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    xkb_state_led_update_all(state);
}

ಸ್ಥಿರ ಶೂನ್ಯ
state_update_enabled_controls(ರಚನೆ xkb_server_state *state,
                              ಎಣಿಕೆ xkb_keyboard_control_flags affect_controls,
                              ಎಣಿಕೆ xkb_keyboard_control_flags controls,
                              ರಚನೆ xkb_events *events)
{
    ಸ್ಥಿರಾಂಕ bool had_sticky_ಕೀಲಿಗಳು = state->ತಳ.components.controls
                               & CONTROL_STICKY_ಕೀಲಿಗಳು;

    /*
     * Enable using the public API with the all the Control values, except
     * the internal ones, if any.
     */
    affect_controls =
        ( affect_controls
        & (ಎಣಿಕೆ xkb_keyboard_control_flags)CONTROL_ALL_BOOLEAN );
    state->ತಳ.components.controls &=
        (ಎಣಿಕೆ xkb_action_controls)~affect_controls;
    state->ತಳ.components.controls |=
        (ಎಣಿಕೆ xkb_action_controls)(controls & affect_controls);

    ಆದರೆ (had_sticky_ಕೀಲಿಗಳು &&
        !(state->ತಳ.components.controls & CONTROL_STICKY_ಕೀಲಿಗಳು)) {
        /* Sticky keys were disabled: clear all locks and latches */
        clear_all_latches_and_locks(state, events);
    }

    xkb_state_update_derived(&state->ತಳ);
}

/**
 * Given a particular key event, updates the state structure to reflect the
 * new modifiers.
 */
ಎಣಿಕೆ xkb_state_component
xkb_state_update_ಕೀಲಿ(ರಚನೆ xkb_state *ತಳ_state, xkb_keycode_t kc,
                     ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction)
{
    /* Guard against client-only state */
    ಪ್ರತಿಜ್ಞಾ(ತಳ_state->ಬಹುಲಕ > SERVER_COMPANION);
    ಆದರೆ (ತಳ_state->ಬಹುಲಕ <= SERVER_COMPANION) {
        log_err(ತಳ_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_ಬಹುಲಕ,
                "%s: Unexpected state type %d\n", __func__, ತಳ_state->ಬಹುಲಕ);
        ಹಿಂತಿರುಗಿ 0;
    }

    ರಚನೆ xkb_server_state * ಸ್ಥಿರಾಂಕ state =
        (ರಚನೆ xkb_server_state *)ತಳ_state;

    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ* ಸ್ಥಿರಾಂಕ ಕೀಲಿ = Xkb_ಕೀಲಿ(state->ತಳ.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    ಆದರೆ (!ಕೀಲಿ || (direction == XKB_ಕೀಲಿ_REPEATED && !ಕೀಲಿ->repeats))
        ಹಿಂತಿರುಗಿ 0;

    ಆದರೆ (direction == XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ &&
        (state->ತಳ.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ಕೀಲಿಗಳು) &&
        (state->a11y_flags & XKB_A11_Y_STICKY_ಕೀಲಿಗಳು_NO_ಏಕಕಾಲಿಕ_ಕೀಲಿಗಳು) &&
        state->ತಳ.components.ತಳ_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ಕೀಲಿಗಳು,
                                      0, ಖಾಲಿ);
    }

    ಸ್ಥಿರಾಂಕ ರಚನೆ state_components prev_components = state->ತಳ.components;

    state->ಗಣ_mods = 0;
    state->clear_mods = 0;

    xkb_filter_apply_all(state, ಖಾಲಿ, ಕೀಲಿ, direction);

    xkb_mod_ಸೂಚ್ಯಂಕ_t i;
    xkb_mod_ಮುಖವಾಡ_t ಬಿಟ್;
    ಗಾಗಿ (i = 0, ಬಿಟ್ = 1; state->ಗಣ_mods; i++, ಬಿಟ್ <<= 1) {
        ಆದರೆ (state->ಗಣ_mods & ಬಿಟ್) {
            state->mod_ಕೀಲಿ_ಎಣಿಕೆ[i]++;
            state->ತಳ.components.ತಳ_mods |= ಬಿಟ್;
            state->ಗಣ_mods &= ~ಬಿಟ್;
        }
    }

    ಗಾಗಿ (i = 0, ಬಿಟ್ = 1; state->clear_mods; i++, ಬಿಟ್ <<= 1) {
        ಆದರೆ (state->clear_mods & ಬಿಟ್) {
            state->mod_ಕೀಲಿ_ಎಣಿಕೆ[i]--;
            ಆದರೆ (state->mod_ಕೀಲಿ_ಎಣಿಕೆ[i] <= 0) {
                state->ತಳ.components.ತಳ_mods &= ~ಬಿಟ್;
                state->mod_ಕೀಲಿ_ಎಣಿಕೆ[i] = 0;
            }
            state->clear_mods &= ~ಬಿಟ್;
        }
    }

    xkb_state_update_derived(&state->ತಳ);

    ಹಿಂತಿರುಗಿ get_state_component_changes(&prev_components, &state->ತಳ.components);
}

/* We need fake keys for `update_latch_modifiers` and `update_latch_group`.
 * These keys must have at least one level in order to break latches. We need 2
 * keys with specific actions in order to update group/mod latches without
 * affecting each other. */
ಸ್ಥಿರ ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು synthetic_ಕೀಲಿ_level_ನಮೂದು = { 0 };
ಸ್ಥಿರ ರಚನೆ xkb_ಕೀಲಿ_type synthetic_ಕೀಲಿ_type = {
     .ಅಂಕೆ_entries = 1,
     .ಅಂಕೆ_ಮಟ್ಟಗಳು = 1,
     .entries = &synthetic_ಕೀಲಿ_level_ನಮೂದು
};
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ synthetic_ಕೀಲಿ = { 0 };

/* Transcription from xserver: XkbLatchModifiers */
ಸ್ಥಿರ ಶೂನ್ಯ
update_latch_modifiers(ರಚನೆ xkb_server_state *state,
                       ರಚನೆ xkb_events *events,
                       xkb_mod_ಮುಖವಾಡ_t ಮುಖವಾಡ, xkb_mod_ಮುಖವಾಡ_t latches)
{
    /* Clear affected latched modifiers */
    ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t clear = ಮುಖವಾಡ & ~latches;
    state->ತಳ.components.latched_mods &= ~clear;

    /* Clear any pending latch to locks using ad hoc action:
     * only affect corresponding modifier latches and no group latch. */
    ರಚನೆ xkb_level synthetic_ಕೀಲಿ_level_break_mod_latch = {
        .ಅಂಕೆ_syms = 0,
        .ಅಂಕೆ_actions = 1,
        .ಊರ್ಧ್ವ = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ,
        .s.sym = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ,
        .a.action.ಆಂತರಿಕ = {
            .type = ACTION_TYPE_ಆಂತರಿಕ,
            .flags = ಆಂತರಿಕ_BREAKS_MOD_LATCH,
            .clear_latched_mods = clear
        }
    };
    ರಚನೆ xkb_ಗುಂಪು synthetic_ಕೀಲಿ_ಗುಂಪು_break_mod_latch = {
        .type = &synthetic_ಕೀಲಿ_type,
        .ಮಟ್ಟಗಳು = &synthetic_ಕೀಲಿ_level_break_mod_latch
    };
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ synthetic_ಕೀಲಿ_break_mod_latch = {
        .ಅಂಕೆ_groups = 1,
        .groups = &synthetic_ಕೀಲಿ_ಗುಂಪು_break_mod_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_ಕೀಲಿ_break_mod_latch, XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ);

    /* Finally set the latched mods by simulate tapping a key with the
     * corresponding action */
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ* ಸ್ಥಿರಾಂಕ ಕೀಲಿ = &synthetic_ಕೀಲಿ;
    ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action latch_mods = {
        .mods = {
            .type = ACTION_TYPE_MOD_LATCH,
            .mods = { .ಮುಖವಾಡ = ಮುಖವಾಡ & latches },
            .flags = 0,
        },
    };
    ರಚನೆ xkb_filter* ಸ್ಥಿರಾಂಕ filter = xkb_filter_new(state);
    filter->ಕೀಲಿ = ಕೀಲಿ;
    filter->func = xkb_filter_mod_latch_func;
    filter->action = latch_mods;
    xkb_filter_mod_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_mod_latch_func(state, events, filter, ಕೀಲಿ, XKB_ಕೀಲಿ_ಮೇಲೆ);
}

/* Transcription from xserver: XkbLatchGroup */
ಸ್ಥಿರ ಶೂನ್ಯ
update_latch_ಗುಂಪು(ರಚನೆ xkb_server_state *state,
                   ರಚನೆ xkb_events *events, int32_t ಗುಂಪು)
{
    /* Clear any pending latch to locks. */
    ಸ್ಥಿರ ರಚನೆ xkb_level synthetic_ಕೀಲಿ_level_break_ಗುಂಪು_latch = {
        .ಅಂಕೆ_syms = 0,
        .ಅಂಕೆ_actions = 1,
        .ಊರ್ಧ್ವ = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ,
        .s.sym = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ,
        .a.action.ಆಂತರಿಕ = {
            .type = ACTION_TYPE_ಆಂತರಿಕ,
            .flags = ಆಂತರಿಕ_BREAKS_ಗುಂಪು_LATCH,
            .clear_latched_mods = 0
        }
    };
    ಸ್ಥಿರ ರಚನೆ xkb_ಗುಂಪು synthetic_ಕೀಲಿ_ಗುಂಪು_break_ಗುಂಪು_latch = {
        .type = &synthetic_ಕೀಲಿ_type,
        .ಮಟ್ಟಗಳು = &synthetic_ಕೀಲಿ_level_break_ಗುಂಪು_latch
    };
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ synthetic_ಕೀಲಿ_break_ಗುಂಪು_latch = {
        .ಅಂಕೆ_groups = 1,
        .groups = &synthetic_ಕೀಲಿ_ಗುಂಪು_break_ಗುಂಪು_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_ಕೀಲಿ_break_ಗುಂಪು_latch, XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ);

    /* Simulate tapping a key with a group latch action, but in isolation: i.e.
     * without affecting the other filters. */
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ* ಸ್ಥಿರಾಂಕ ಕೀಲಿ = &synthetic_ಕೀಲಿ;
    ಸ್ಥಿರಾಂಕ ಒಕ್ಕೂಟ xkb_action latch_ಗುಂಪು = {
        .ಗುಂಪು = {
            .type = ACTION_TYPE_ಗುಂಪು_LATCH,
            .flags = ACTION_ABSOLUTE_SWITCH,
            .ಗುಂಪು = ಗುಂಪು,
        },
    };
    ರಚನೆ xkb_filter* ಸ್ಥಿರಾಂಕ filter = xkb_filter_new(state);
    filter->ಕೀಲಿ = ಕೀಲಿ;
    filter->func = xkb_filter_ಗುಂಪು_latch_func;
    filter->action = latch_ಗುಂಪು;
    xkb_filter_ಗುಂಪು_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_ಗುಂಪು_latch_func(state, events, filter, ಕೀಲಿ, XKB_ಕೀಲಿ_ಮೇಲೆ);
}

/**
 * Compute the resolved effective mask of an arbitrary input.
 *
 * Contrary to `mod_mask_get_effective`, it resolves only modifiers not present
 * in the canonical mask, so that it enables `xkb_state_serialize_mods` to
 * round trip via `xkb_state_update_mask`.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ xkb_mod_ಮುಖವಾಡ_t
resolve_ಗಮ್ಯ_canonical_mods(ರಚನೆ xkb_keymap *keymap, xkb_mod_ಮುಖವಾಡ_t mods)
{
    ಹಿಂತಿರುಗಿ
        /*
         * Keep canonical modifier mask.
         * It contains either real modifiers or canonical virtual modifiers.
         */
        (mods & keymap->canonical_state_ಮುಖವಾಡ) |
        /* Resolve other modifiers */
        mod_ಮುಖವಾಡ_get_effective(keymap,
                               mods & ~keymap->canonical_state_ಮುಖವಾಡ);
}

ಸ್ಥಿರ ಶೂನ್ಯ
state_update_latched_locked(
    ರಚನೆ xkb_server_state * ಸೀಮಿತ state,
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_state_components_update * ಸೀಮಿತ update,
    ರಚನೆ xkb_events * ಸೀಮಿತ events
)
{
    /* Update locks */
    ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t affect_locked_mods =
        resolve_ಗಮ್ಯ_canonical_mods(state->ತಳ.keymap, update->affect_locked_mods);
    ಆದರೆ ((update->components & XKB_STATE_MODS_LOCKED) &&
        affect_locked_mods) {
        ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t locked_mods =
            resolve_ಗಮ್ಯ_canonical_mods(state->ತಳ.keymap, update->locked_mods);
        state->ತಳ.components.locked_mods &= ~affect_locked_mods;
        state->ತಳ.components.locked_mods |= locked_mods & affect_locked_mods;
    }
    ಆದರೆ (update->components & XKB_STATE_LAYOUT_LOCKED) {
        state->ತಳ.components.locked_ಗುಂಪು = update->locked_layout;
    }

    /* Update latches */
    ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t affect_latched_mods =
        resolve_ಗಮ್ಯ_canonical_mods(state->ತಳ.keymap, update->affect_latched_mods);
    ಆದರೆ ((update->components & XKB_STATE_MODS_LATCHED) &&
        affect_latched_mods) {
        ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t latched_mods =
            resolve_ಗಮ್ಯ_canonical_mods(state->ತಳ.keymap, update->latched_mods);
        update_latch_modifiers(state, events, affect_latched_mods, latched_mods);
    }
    ಆದರೆ (update->components & XKB_STATE_LAYOUT_LATCHED) {
        update_latch_ಗುಂಪು(state, events, update->latched_layout);
    }
}

ಎಣಿಕೆ xkb_state_component
xkb_state_update_latched_locked(ರಚನೆ xkb_state *ತಳ_state,
                                xkb_mod_ಮುಖವಾಡ_t affect_latched_mods,
                                xkb_mod_ಮುಖವಾಡ_t latched_mods,
                                bool affect_latched_layout,
                                int32_t latched_layout,
                                xkb_mod_ಮುಖವಾಡ_t affect_locked_mods,
                                xkb_mod_ಮುಖವಾಡ_t locked_mods,
                                bool affect_locked_layout,
                                int32_t locked_layout)
{
    /* Guard against client-only state */
    ಪ್ರತಿಜ್ಞಾ(ತಳ_state->ಬಹುಲಕ > SERVER_COMPANION);
    ಆದರೆ (ತಳ_state->ಬಹುಲಕ <= SERVER_COMPANION) {
        log_err(ತಳ_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_ಬಹುಲಕ,
                "%s: Unexpected state type %d\n", __func__, ತಳ_state->ಬಹುಲಕ);
        ಹಿಂತಿರುಗಿ 0;
    }

    ರಚನೆ xkb_server_state * ಸ್ಥಿರಾಂಕ state =
        (ರಚನೆ xkb_server_state *)ತಳ_state;

    ಸ್ಥಿರಾಂಕ ರಚನೆ state_components ಹಿಂದಿನ_components = state->ತಳ.components;
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_state_component components
        = ((affect_latched_mods || latched_mods) ? XKB_STATE_MODS_LATCHED : 0)
        | ((affect_locked_mods || locked_mods) ? XKB_STATE_MODS_LOCKED : 0)
        | (affect_latched_layout ? XKB_STATE_LAYOUT_LATCHED : 0)
        | (affect_locked_layout ? XKB_STATE_LAYOUT_LOCKED : 0);
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_state_components_update update = {
        .ಗಾತ್ರ_2 = ಗಾತ್ರ(update),
        .components = components,
        .affect_latched_mods = affect_latched_mods,
        .latched_mods = latched_mods,
        .latched_layout = latched_layout,
        .affect_locked_mods = affect_locked_mods,
        .locked_mods = locked_mods,
        .locked_layout = locked_layout
    };
    state_update_latched_locked(state, &update, ಖಾಲಿ);
    xkb_state_update_derived(&state->ತಳ);
    ಹಿಂತಿರುಗಿ get_state_component_changes(&ಹಿಂದಿನ_components,
                                       &state->ತಳ.components);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
clear_all_latches_and_locks(ರಚನೆ xkb_server_state *state,
                            ರಚನೆ xkb_events *events)
{
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_state_component components
        = XKB_STATE_MODS_LATCHED | XKB_STATE_MODS_LOCKED
        | XKB_STATE_LAYOUT_LATCHED | XKB_STATE_LAYOUT_LOCKED;
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_state_components_update update = {
        .ಗಾತ್ರ_2 = ಗಾತ್ರ(update),
        .components = components,
        .affect_latched_mods = (xkb_mod_ಮುಖವಾಡ_t)XKB_MOD_ALL,
        .latched_mods = 0,
        .latched_layout = 0,
        .affect_locked_mods = (xkb_mod_ಮುಖವಾಡ_t)XKB_MOD_ALL,
        .locked_mods = 0,
        .locked_layout = 0
    };
    state_update_latched_locked(state, &update, events);
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
state_update_layout_policy(ರಚನೆ xkb_server_state *state,
                           ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_layout_policy_update *update)
{
    ಆದರೆ (xkb_feature_supported(XKB_FEATURE_ENUM_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_POLICY,
                              update->policy)) {
        ಆದರೆ (update->policy == XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_REDIRECT) {
            ಆದರೆ (update->redirect < state->ತಳ.keymap->ಅಂಕೆ_groups) {
                state->ತಳ.ಹೊರಹೋಗು_ದ_range_ಗುಂಪು.redirect_ಗುಂಪು =
                    update->redirect;
            } ಇಲ್ಲದಿದ್ದರೆ {
                log_err(state->ತಳ.keymap->ctx,
                        XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ_2,
                        "Layout policy: "
                        "unsupported layout index %"PRIu32" > %"PRIu32"\n",
                        update->redirect + 1, state->ತಳ.keymap->ಅಂಕೆ_groups);
                ಹಿಂತಿರುಗಿ XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ;
            }
        }
        state->ತಳ.ಹೊರಹೋಗು_ದ_range_ಗುಂಪು.policy = update->policy;
        ಹಿಂತಿರುಗಿ XKB_SUCCESS;
    } ಇಲ್ಲದಿದ್ದರೆ {
        log_err(state->ತಳ.keymap->ctx,
                XKB_ERROR_UNSUPPORTED_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_POLICY_2,
                "Unsupported layout policy: %d\n", update->policy);
        ಹಿಂತಿರುಗಿ XKB_ERROR_UNSUPPORTED_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_POLICY;
    }
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
ಸ್ಥಿರ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
check_state_update_abi_(ರಚನೆ xkb_context * ಸೀಮಿತ ctx,
                        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸೀಮಿತ func,
                        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_synthetic_update * ಸೀಮಿತ update)
{
    ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2 = XKB_SUCCESS;
    ಆದರೆ ((ಸ್ಥಿತಿ_2 = xkb_check_state_abi(update)) ||
        (update->reserved0 != 0 &&
         (ಸ್ಥಿತಿ_2 = XKB_ERROR_ABI_FORWARD_COMPAT)) ||
        (update->components &&
         (ಸ್ಥಿತಿ_2 = xkb_check_state_abi(update->components))) ||
        (update->layout_policy &&
         (ಸ್ಥಿತಿ_2 = xkb_check_state_abi(update->layout_policy)))) {
        xkb_log_abi_error(ctx, func, ಸ್ಥಿತಿ_2);
    }
    ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

#ವ್ಯಾಖ್ಯಾನಿಸು check_state_update_abi(ctx, update) \
    check_state_update_abi_(ctx, __func__, update)

ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
xkb_state_update_synthetic(ರಚನೆ xkb_state * ತಳ_state,
                           ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_synthetic_update * update,
                           ಎಣಿಕೆ xkb_state_component *ಬದಲಾಯಿತು)
{
    /* Guard against client-only state */
    ಆದರೆ (ತಳ_state->ಬಹುಲಕ <= SERVER_COMPANION) {
        log_err(ತಳ_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_ಬಹುಲಕ,
                "%s: Unexpected state type %d\n", __func__, ತಳ_state->ಬಹುಲಕ);
        ಹಿಂತಿರುಗಿ XKB_ERROR_UNEXPECTED_STATE_ಬಹುಲಕ;
    }

    ರಚನೆ xkb_server_state * ಸ್ಥಿರಾಂಕ state =
        (ರಚನೆ xkb_server_state *)ತಳ_state;

    /* Check ABI compatibility */
    ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2 = check_state_update_abi(state->ತಳ.keymap->ctx,
                                                    update);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != XKB_SUCCESS)
        ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;

    ಸ್ಥಿರಾಂಕ ರಚನೆ state_components ಹಿಂದಿನ_components = state->ತಳ.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    /* Update parametrized controls first */
    ಆದರೆ (update->layout_policy) {
        ಸ್ಥಿತಿ_2 = state_update_layout_policy(state, update->layout_policy);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != XKB_SUCCESS)
            ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
    }

    ಆದರೆ (update->components) {
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_state_components_update * ಸ್ಥಿರಾಂಕ components =
            update->components;
        /* Update boolean controls first */
        ಆದರೆ (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, ಖಾಲಿ);
        }

        state_update_latched_locked(state, components, ಖಾಲಿ);
    }

    xkb_state_update_derived(&state->ತಳ);

    ಆದರೆ (ಬದಲಾಯಿತು) {
        *ಬದಲಾಯಿತು = get_state_component_changes(&ಹಿಂದಿನ_components,
                                               &state->ತಳ.components);
    }
    ಹಿಂತಿರುಗಿ XKB_SUCCESS;
}

/**
 * Updates the state from a set of explicit masks as gained from
 * xkb_state_serialize_mods and xkb_state_serialize_groups.  As noted in the
 * documentation for these functions in xkbcommon.h, this round-trip is
 * lossy, and should only be used to update a slave state mirroring the
 * master, e.g. in a client/server window system.
 */
ಎಣಿಕೆ xkb_state_component
xkb_state_update_ಮುಖವಾಡ(ರಚನೆ xkb_state *ತಳ_state,
                      xkb_mod_ಮುಖವಾಡ_t ತಳ_mods,
                      xkb_mod_ಮುಖವಾಡ_t latched_mods,
                      xkb_mod_ಮುಖವಾಡ_t locked_mods,
                      xkb_layout_ಸೂಚ್ಯಂಕ_t ತಳ_ಗುಂಪು,
                      xkb_layout_ಸೂಚ್ಯಂಕ_t latched_ಗುಂಪು,
                      xkb_layout_ಸೂಚ್ಯಂಕ_t locked_ಗುಂಪು)
{
    /* Guard against server-only and server companion state */
    ಪ್ರತಿಜ್ಞಾ(ತಳ_state->ಬಹುಲಕ == CLIENT_STATE ||
           ತಳ_state->ಬಹುಲಕ == LEGACY_MIXED_STATE);
    ಆದರೆ (ತಳ_state->ಬಹುಲಕ != CLIENT_STATE &&
        ತಳ_state->ಬಹುಲಕ != LEGACY_MIXED_STATE) {
        log_err(ತಳ_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_ಬಹುಲಕ,
                "%s: Unexpected state type %d\n", __func__, ತಳ_state->ಬಹುಲಕ);
        ಹಿಂತಿರುಗಿ 0;
    }

    ರಚನೆ xkb_client_state * ಸ್ಥಿರಾಂಕ state =
        (ರಚನೆ xkb_client_state *)ತಳ_state;

    ಸ್ಥಿರಾಂಕ ರಚನೆ state_components prev_components = state->ತಳ.components;

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
    state->ತಳ.components.ತಳ_mods =
        resolve_ಗಮ್ಯ_canonical_mods(state->ತಳ.keymap, ತಳ_mods);
    state->ತಳ.components.latched_mods =
        resolve_ಗಮ್ಯ_canonical_mods(state->ತಳ.keymap, latched_mods);
    state->ತಳ.components.locked_mods =
        resolve_ಗಮ್ಯ_canonical_mods(state->ತಳ.keymap, locked_mods);

    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->ತಳ.components.ತಳ_ಗುಂಪು = (int32_t) ತಳ_ಗುಂಪು;
    state->ತಳ.components.latched_ಗುಂಪು = (int32_t) latched_ಗುಂಪು;
    state->ತಳ.components.locked_ಗುಂಪು = (int32_t) locked_ಗುಂಪು;

    xkb_state_update_derived(&state->ತಳ);

    ಹಿಂತಿರುಗಿ get_state_component_changes(&prev_components, &state->ತಳ.components);
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Lock_Modifier
 */
ಸ್ಥಿರ bool
should_do_caps_transformation(ರಚನೆ xkb_state *state, xkb_keycode_t kc)
{
    ಹಿಂತಿರುಗಿ
        xkb_state_mod_ಸೂಚ್ಯಂಕ_is_ಸಕ್ರಿಯ(state, XKB_MOD_ಸೂಚ್ಯಂಕ_CAPS,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_ಸೂಚ್ಯಂಕ_is_consumed(state, kc, XKB_MOD_ಸೂಚ್ಯಂಕ_CAPS) == 0;
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Control_Modifier
 */
ಸ್ಥಿರ bool
should_do_ctrl_transformation(ರಚನೆ xkb_state *state, xkb_keycode_t kc)
{
    ಹಿಂತಿರುಗಿ
        xkb_state_mod_ಸೂಚ್ಯಂಕ_is_ಸಕ್ರಿಯ(state, XKB_MOD_ಸೂಚ್ಯಂಕ_CTRL,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_ಸೂಚ್ಯಂಕ_is_consumed(state, kc, XKB_MOD_ಸೂಚ್ಯಂಕ_CTRL) == 0;
}

/**
 * Provides the symbols to use for the given key and state.  Returns the
 * number of symbols pointed to in syms_out.
 */
ಪೂರ್ಣಾಂಕ
xkb_state_ಕೀಲಿ_get_syms(ರಚನೆ xkb_state *state, xkb_keycode_t kc,
                       ಸ್ಥಿರಾಂಕ xkb_keysym_t **syms_ಹೊರಹೋಗು)
{
    ಸ್ಥಿರಾಂಕ xkb_layout_ಸೂಚ್ಯಂಕ_t layout = xkb_state_ಕೀಲಿ_get_layout(state, kc);
    ಆದರೆ (layout == XKB_LAYOUT_INVALID)
        ಹೋಗು err;

    ಸ್ಥಿರಾಂಕ xkb_level_ಸೂಚ್ಯಂಕ_t level = xkb_state_ಕೀಲಿ_get_level(state, kc, layout);
    ಆದರೆ (level == XKB_LEVEL_INVALID)
        ಹೋಗು err;

    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ* ಸ್ಥಿರಾಂಕ ಕೀಲಿ = Xkb_ಕೀಲಿ(state->keymap, kc);

    ಆದರೆ (!ಕೀಲಿ)
        ಹೋಗು err;

    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_level* ಸ್ಥಿರಾಂಕ leveli =
        xkb_keymap_ಕೀಲಿ_get_level(state->keymap, ಕೀಲಿ, layout, level);

    ಆದರೆ (!leveli)
        ಹೋಗು err;

    ಸ್ಥಿರಾಂಕ xkb_keysym_ಎಣಿಕೆ_t ಅಂಕೆ_syms = leveli->ಅಂಕೆ_syms;
    ಆದರೆ (ಅಂಕೆ_syms == 0)
        ಹೋಗು err;

    ಆದರೆ (should_do_caps_transformation(state, kc)) {
        /* Only simple capitalization rules: keysyms count is unchanged. */
        ಆದರೆ (ಅಂಕೆ_syms > 1) {
            *syms_ಹೊರಹೋಗು = (leveli->has_ಊರ್ಧ್ವ)
                      ? leveli->s.syms + ಅಂಕೆ_syms
                      : leveli->s.syms;
        } ಇಲ್ಲದಿದ್ದರೆ {
            *syms_ಹೊರಹೋಗು = &leveli->ಊರ್ಧ್ವ;
        }
    } ಇಲ್ಲದಿದ್ದರೆ {
        *syms_ಹೊರಹೋಗು = (ಅಂಕೆ_syms > 1)
                  ? leveli->s.syms
                  : &leveli->s.sym;
    }
    ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) ಅಂಕೆ_syms;

err:
    *syms_ಹೊರಹೋಗು = ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ 0;
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
ಸ್ಥಿರ ಅಕ್ಷರ
Xkb_ಗಮ್ಯ_Control(ಅಕ್ಷರ ch)
{
    ಅಕ್ಷರ c = ch;

    ಆದರೆ ((c >= '@' && c < '\177') || c == ' ')
        c &= 0x1F;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c == '2')
        c = '\000';
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c >= '3' && c <= '7')
        c -= ('3' - '\033');
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c == '8')
        c = '\177';
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c == '/')
        c = '_' & 0x1F;
    ಹಿಂತಿರುಗಿ c;
}

/**
 * Provides either exactly one symbol, or XKB_KEY_NoSymbol.
 */
xkb_keysym_t
xkb_state_ಕೀಲಿ_get_one_sym(ರಚನೆ xkb_state *state, xkb_keycode_t kc)
{
    ಸ್ಥಿರಾಂಕ xkb_keysym_t *syms = ಖಾಲಿ;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಂಕೆ_syms = xkb_state_ಕೀಲಿ_get_syms(state, kc, &syms);

    ಆದರೆ (ಅಂಕೆ_syms != 1)
        ಹಿಂತಿರುಗಿ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
    ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ syms[0];
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
get_one_sym_for_string(ರಚನೆ xkb_state *state, xkb_keycode_t kc)
{
    ಸ್ಥಿರಾಂಕ xkb_layout_ಸೂಚ್ಯಂಕ_t layout = xkb_state_ಕೀಲಿ_get_layout(state, kc);
    ಸ್ಥಿರಾಂಕ xkb_layout_ಸೂಚ್ಯಂಕ_t ಅಂಕೆ_layouts =
        xkb_keymap_ಅಂಕೆ_layouts_for_ಕೀಲಿ(state->keymap, kc);
    xkb_level_ಸೂಚ್ಯಂಕ_t level = xkb_state_ಕೀಲಿ_get_level(state, kc, layout);
    ಆದರೆ (layout == XKB_LAYOUT_INVALID || ಅಂಕೆ_layouts == 0 ||
        level == XKB_LEVEL_INVALID)
        ಹಿಂತಿರುಗಿ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;

    ಸ್ಥಿರಾಂಕ xkb_keysym_t *syms = ಖಾಲಿ;
    ಪೂರ್ಣಾಂಕ nsyms = xkb_keymap_ಕೀಲಿ_get_syms_by_level(state->keymap, kc,
                                                 layout, level, &syms);
    ಆದರೆ (nsyms != 1)
        ಹಿಂತಿರುಗಿ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
    xkb_keysym_t sym = syms[0];

    ಆದರೆ (should_do_ctrl_transformation(state, kc) && sym > 127u) {
        ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t i = 0; i < ಅಂಕೆ_layouts; i++) {
            level = xkb_state_ಕೀಲಿ_get_level(state, kc, i);
            ಆದರೆ (level == XKB_LEVEL_INVALID)
                ಮುಂದುವರಿ;

            nsyms = xkb_keymap_ಕೀಲಿ_get_syms_by_level(state->keymap, kc,
                                                     i, level, &syms);
            ಆದರೆ (nsyms == 1 && syms[0] <= 127u) {
                sym = syms[0];
                ನಿಲ್ಲಿಸು;
            }
        }
    }

    ಆದರೆ (should_do_caps_transformation(state, kc)) {
        sym = xkb_keysym_ಗಮ್ಯ_ಊರ್ಧ್ವ(sym);
    }

    ಹಿಂತಿರುಗಿ sym;
}

ಪೂರ್ಣಾಂಕ
xkb_state_ಕೀಲಿ_get_utf8(ರಚನೆ xkb_state *state, xkb_keycode_t kc,
                       ಅಕ್ಷರ *ಬಫರ್, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ಪೂರ್ಣಾಂಕ nsyms;
    ಸ್ಥಿರಾಂಕ xkb_keysym_t *syms = ಖಾಲಿ;
    ಸ್ಥಿರಾಂಕ xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    ಆದರೆ (sym != XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
        nsyms = 1; syms = &sym;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        nsyms = xkb_state_ಕೀಲಿ_get_syms(state, kc, &syms);
    }

    /* Make sure not to truncate in the middle of a UTF-8 sequence. */
    ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ = 0;
    ಅಕ್ಷರ tmp[XKB_KEYSYM_UTF8_MAX_ಗಾತ್ರ];
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nsyms; i++) {
        ಪೂರ್ಣಾಂಕ ret = xkb_keysym_ಗಮ್ಯ_utf8(syms[i], tmp, ಗಾತ್ರ(tmp));
        ಆದರೆ (ret <= 0)
            ಹೋಗು err_bad;

        ret--;
        ಆದರೆ ((ಗಾತ್ರ_t)ಸ್ಥಾನಾಂತರ + (ಗಾತ್ರ_t)ret <= ಗಾತ್ರ_2)
            ಸ್ಮರಣೆ_ನಕಲು(ಬಫರ್ + ಸ್ಥಾನಾಂತರ, tmp, (ಗಾತ್ರ_t)ret);
        ಸ್ಥಾನಾಂತರ += ret;
    }

    ಆದರೆ ((ಗಾತ್ರ_t)ಸ್ಥಾನಾಂತರ >= ಗಾತ್ರ_2)
        ಹೋಗು err_trunc;
    ಬಫರ್[ಸ್ಥಾನಾಂತರ] = '\0';

    ಆದರೆ (!is_valid_utf8(ಬಫರ್, (ಗಾತ್ರ_t)ಸ್ಥಾನಾಂತರ))
        ಹೋಗು err_bad;

    ಆದರೆ (ಸ್ಥಾನಾಂತರ == 1 && (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) ಬಫರ್[0] <= 127u &&
        should_do_ctrl_transformation(state, kc))
        ಬಫರ್[0] = Xkb_ಗಮ್ಯ_Control(ಬಫರ್[0]);

    ಹಿಂತಿರುಗಿ ಸ್ಥಾನಾಂತರ;

err_trunc:
    ಆದರೆ (ಗಾತ್ರ_2 > 0)
        ಬಫರ್[ಗಾತ್ರ_2 - 1] = '\0';
    ಹಿಂತಿರುಗಿ ಸ್ಥಾನಾಂತರ;

err_bad:
    ಆದರೆ (ಗಾತ್ರ_2 > 0)
        ಬಫರ್[0] = '\0';
    ಹಿಂತಿರುಗಿ 0;
}

uint32_t
xkb_state_ಕೀಲಿ_get_utf32(ರಚನೆ xkb_state *state, xkb_keycode_t kc)
{
    ಸ್ಥಿರಾಂಕ xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    uint32_t cp = xkb_keysym_ಗಮ್ಯ_utf32(sym);

    ಆದರೆ (cp <= 127u && should_do_ctrl_transformation(state, kc))
        cp = (uint32_t) Xkb_ಗಮ್ಯ_Control((ಅಕ್ಷರ) cp);

    ಹಿಂತಿರುಗಿ cp;
}

/**
 * Serializes the requested modifier state into an xkb_mod_mask_t, with all
 * the same disclaimers as in xkb_state_update_mask.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ xkb_mod_ಮುಖವಾಡ_t
serialize_mods(ಸ್ಥಿರಾಂಕ ರಚನೆ state_components *components,
               ಎಣಿಕೆ xkb_state_component type)
{
    xkb_mod_ಮುಖವಾಡ_t ret = 0;

    ಆದರೆ (type & XKB_STATE_MODS_EFFECTIVE)
        ಹಿಂತಿರುಗಿ components->mods;

    ಆದರೆ (type & XKB_STATE_MODS_DEPRESSED)
        ret |= components->ತಳ_mods;
    ಆದರೆ (type & XKB_STATE_MODS_LATCHED)
        ret |= components->latched_mods;
    ಆದರೆ (type & XKB_STATE_MODS_LOCKED)
        ret |= components->locked_mods;

    ಹಿಂತಿರುಗಿ ret;
}

xkb_mod_ಮುಖವಾಡ_t
xkb_state_serialize_mods(ರಚನೆ xkb_state *state,
                         ಎಣಿಕೆ xkb_state_component type)
{
    ಹಿಂತಿರುಗಿ serialize_mods(&state->components, type);
}

/**
 * Serialises the requested group state, with all the same disclaimers as
 * in xkb_state_update_mask.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ xkb_layout_ಸೂಚ್ಯಂಕ_t
serialize_layout(ಸ್ಥಿರಾಂಕ ರಚನೆ state_components *components,
                 ಎಣಿಕೆ xkb_state_component type)
{
    ಆದರೆ (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ಹಿಂತಿರುಗಿ components->ಗುಂಪು;

    int32_t ret = 0;
    ಆದರೆ (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret += components->ತಳ_ಗುಂಪು;
    ಆದರೆ (type & XKB_STATE_LAYOUT_LATCHED)
        ret += components->latched_ಗುಂಪು;
    ಆದರೆ (type & XKB_STATE_LAYOUT_LOCKED)
        ret += components->locked_ಗುಂಪು;

    ಹಿಂತಿರುಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t)ret;
}

xkb_layout_ಸೂಚ್ಯಂಕ_t
xkb_state_serialize_layout(ರಚನೆ xkb_state *state,
                           ಎಣಿಕೆ xkb_state_component type)
{
    ಹಿಂತಿರುಗಿ serialize_layout(&state->components, type);
}

xkb_led_ಮುಖವಾಡ_t
xkb_state_serialize_leds(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_state *state,
                         ಎಣಿಕೆ xkb_state_component type)
{
    ಹಿಂತಿರುಗಿ (type & XKB_STATE_LEDS)
        ? state->components.leds
        : 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಎಣಿಕೆ xkb_keyboard_control_flags
serialize_controls(ಸ್ಥಿರಾಂಕ ರಚನೆ state_components *components,
                   ಎಣಿಕೆ xkb_state_component type)
{
    ಹಿಂತಿರುಗಿ (type & XKB_STATE_CONTROLS_EFFECTIVE)
        /*
         * Enable using the public API with the all the Controls values, except
         * the internal ones, if any.
         */
        ? (ಎಣಿಕೆ xkb_keyboard_control_flags)
          (components->controls & CONTROL_ALL_BOOLEAN)
        : 0;
}

ಎಣಿಕೆ xkb_keyboard_control_flags
xkb_state_serialize_controls(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_state *state,
                             ಎಣಿಕೆ xkb_state_component type)
{
    ಹಿಂತಿರುಗಿ serialize_controls(&state->components, type);
}

/**
 * Gets a modifier mask and returns the resolved effective mask; this
 * is needed because some modifiers can also map to other modifiers, e.g.
 * the "NumLock" modifier usually also sets the "Mod2" modifier.
 */
xkb_mod_ಮುಖವಾಡ_t
mod_ಮುಖವಾಡ_get_effective(ರಚನೆ xkb_keymap *keymap, xkb_mod_ಮುಖವಾಡ_t mods)
{
    /* Initialize the effective mask with its corresponding real mods. */
    xkb_mod_ಮುಖವಾಡ_t ಮುಖವಾಡ = mods & MOD_REAL_ಮುಖವಾಡ_ALL;

    /* Resolve the virtual modifiers */
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod *mod;
    xkb_mod_ಸೂಚ್ಯಂಕ_t i;
    xkb_vmods_enumerate(i, mod, &keymap->mods)
        ಆದರೆ (mods & (UINT32_C(1) << i))
            ಮುಖವಾಡ |= mod->mapping;

    ಹಿಂತಿರುಗಿ ಮುಖವಾಡ;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
ಪೂರ್ಣಾಂಕ
xkb_state_mod_ಸೂಚ್ಯಂಕ_is_ಸಕ್ರಿಯ(ರಚನೆ xkb_state *state,
                              xkb_mod_ಸೂಚ್ಯಂಕ_t idx,
                              ಎಣಿಕೆ xkb_state_component type)
{
    ಆದರೆ (unlikely(idx >= xkb_keymap_ಅಂಕೆ_mods(state->keymap)))
        ಹಿಂತಿರುಗಿ -1;

    ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t mapping = state->keymap->mods.mods[idx].mapping;
    ಆದರೆ (!mapping) {
        /* Modifier not mapped */
        ಹಿಂತಿರುಗಿ 0;
    }
    /* WARNING: this may overmatch for virtual modifiers */
    ಹಿಂತಿರುಗಿ (xkb_state_serialize_mods(state, type) & mapping) == mapping;
}

ಎಣಿಕೆ {
    XKB_STATE_ಹೊಂದಿಕೆ_FLAGS = XKB_STATE_ಹೊಂದಿಕೆ_ANY
                          | XKB_STATE_ಹೊಂದಿಕೆ_ALL
                          | XKB_STATE_ಹೊಂದಿಕೆ_NON_EXCLUSIVE
};

/**
 * Helper function for xkb_state_mod_indices_are_active and
 * xkb_state_mod_names_are_active.
 */
ಸ್ಥಿರ bool
ಹೊಂದಿಕೆ_mod_masks(ರಚನೆ xkb_state *state,
                ಎಣಿಕೆ xkb_state_component type,
                ಎಣಿಕೆ xkb_state_ಹೊಂದಿಕೆ ಹೊಂದಿಕೆ,
                xkb_mod_ಮುಖವಾಡ_t wanted)
{
    ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t ಸಕ್ರಿಯ = xkb_state_serialize_mods(state, type);

    ಆದರೆ (!(ಹೊಂದಿಕೆ & XKB_STATE_ಹೊಂದಿಕೆ_NON_EXCLUSIVE) && (ಸಕ್ರಿಯ & ~wanted))
        ಹಿಂತಿರುಗಿ false;

    ಆದರೆ (ಹೊಂದಿಕೆ & XKB_STATE_ಹೊಂದಿಕೆ_ANY)
        ಹಿಂತಿರುಗಿ ಸಕ್ರಿಯ & wanted;

    /* Ensure all flags but XKB_STATE_MATCH_ALL are processed before */
    static_assert(XKB_STATE_ಹೊಂದಿಕೆ_ALL ==
                  ((ಎಣಿಕೆ xkb_state_ಹೊಂದಿಕೆ) XKB_STATE_ಹೊಂದಿಕೆ_FLAGS &
                   ~(XKB_STATE_ಹೊಂದಿಕೆ_ANY | XKB_STATE_ಹೊಂದಿಕೆ_NON_EXCLUSIVE)),
                  "");

    ಹಿಂತಿರುಗಿ (ಸಕ್ರಿಯ & wanted) == wanted;
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, or -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ಪೂರ್ಣಾಂಕ
xkb_state_mod_indices_are_ಸಕ್ರಿಯ(ರಚನೆ xkb_state *state,
                                 ಎಣಿಕೆ xkb_state_component type,
                                 ಎಣಿಕೆ xkb_state_ಹೊಂದಿಕೆ ಹೊಂದಿಕೆ,
                                 ...)
{
    ಆದರೆ (ಹೊಂದಿಕೆ & ~((ಎಣಿಕೆ xkb_state_ಹೊಂದಿಕೆ) XKB_STATE_ಹೊಂದಿಕೆ_FLAGS))
        ಹಿಂತಿರುಗಿ -2;

    va_list ap;
    xkb_mod_ಮುಖವಾಡ_t wanted = 0;
    ಪೂರ್ಣಾಂಕ ret = 0;
    ಸ್ಥಿರಾಂಕ xkb_mod_ಸೂಚ್ಯಂಕ_t ಅಂಕೆ_mods = xkb_keymap_ಅಂಕೆ_mods(state->keymap);

    va_ಆರಂಭ(ap, ಹೊಂದಿಕೆ);
    ಆಗ (1) {
        xkb_mod_ಸೂಚ್ಯಂಕ_t idx = va_arg(ap, xkb_mod_ಸೂಚ್ಯಂಕ_t);
        ಆದರೆ (idx == XKB_MOD_INVALID)
            ನಿಲ್ಲಿಸು;
        ಆದರೆ (unlikely(idx >= ಅಂಕೆ_mods)) {
            ret = -1;
            ನಿಲ್ಲಿಸು;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_ಕೊನೆ(ap);

    ಆದರೆ (ret == -1)
        ಹಿಂತಿರುಗಿ ret;

    ಆದರೆ (!wanted) {
        /* Modifiers not mapped */
        ಹಿಂತಿರುಗಿ 0;
    }

    ಹಿಂತಿರುಗಿ ಹೊಂದಿಕೆ_mod_masks(state, type, ಹೊಂದಿಕೆ, wanted);
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
ಪೂರ್ಣಾಂಕ
xkb_state_mod_name_is_ಸಕ್ರಿಯ(ರಚನೆ xkb_state *state, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
                             ಎಣಿಕೆ xkb_state_component type)
{
    ಸ್ಥಿರಾಂಕ xkb_mod_ಸೂಚ್ಯಂಕ_t idx = xkb_keymap_mod_get_ಸೂಚ್ಯಂಕ(state->keymap, name);

    ಆದರೆ (idx == XKB_MOD_INVALID)
        ಹಿಂತಿರುಗಿ -1;

    ಹಿಂತಿರುಗಿ xkb_state_mod_ಸೂಚ್ಯಂಕ_is_ಸಕ್ರಿಯ(state, idx, type);
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ATTR_NULL_SENTINEL ಪೂರ್ಣಾಂಕ
xkb_state_mod_ಹೆಸರುಗಳು_are_ಸಕ್ರಿಯ(ರಚನೆ xkb_state *state,
                               ಎಣಿಕೆ xkb_state_component type,
                               ಎಣಿಕೆ xkb_state_ಹೊಂದಿಕೆ ಹೊಂದಿಕೆ,
                               ...)
{
    ಆದರೆ (ಹೊಂದಿಕೆ & ~((ಎಣಿಕೆ xkb_state_ಹೊಂದಿಕೆ) XKB_STATE_ಹೊಂದಿಕೆ_FLAGS))
        ಹಿಂತಿರುಗಿ -2;

    va_list ap;
    xkb_mod_ಮುಖವಾಡ_t wanted = 0;
    ಪೂರ್ಣಾಂಕ ret = 0;

    va_ಆರಂಭ(ap, ಹೊಂದಿಕೆ);
    ಆಗ (1) {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str = va_arg(ap, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *);
        ಆದರೆ (str == ಖಾಲಿ)
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿರಾಂಕ xkb_mod_ಸೂಚ್ಯಂಕ_t idx = xkb_keymap_mod_get_ಸೂಚ್ಯಂಕ(state->keymap, str);
        ಆದರೆ (idx == XKB_MOD_INVALID) {
            ret = -1;
            ನಿಲ್ಲಿಸು;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_ಕೊನೆ(ap);

    ಆದರೆ (ret == -1)
        ಹಿಂತಿರುಗಿ ret;

    ಆದರೆ (!wanted) {
        /* Modifiers not mapped */
        ಹಿಂತಿರುಗಿ 0;
    }

    ಹಿಂತಿರುಗಿ ಹೊಂದಿಕೆ_mod_masks(state, type, ಹೊಂದಿಕೆ, wanted);
}

/**
 * Returns 1 if the given group is active with the specified type(s), 0 if
 * not, or -1 if the group is invalid.
 */
ಪೂರ್ಣಾಂಕ
xkb_state_layout_ಸೂಚ್ಯಂಕ_is_ಸಕ್ರಿಯ(ರಚನೆ xkb_state *state,
                                 xkb_layout_ಸೂಚ್ಯಂಕ_t idx,
                                 ಎಣಿಕೆ xkb_state_component type)
{
    ಆದರೆ (idx >= state->keymap->ಅಂಕೆ_groups)
        ಹಿಂತಿರುಗಿ -1;

    ಪೂರ್ಣಾಂಕ ret = 0;
    ಆದರೆ (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ret |= (state->components.ಗುಂಪು == idx);
    ಆದರೆ (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret |= (state->components.ತಳ_ಗುಂಪು == (int32_t) idx);
    ಆದರೆ (type & XKB_STATE_LAYOUT_LATCHED)
        ret |= (state->components.latched_ಗುಂಪು == (int32_t) idx);
    ಆದರೆ (type & XKB_STATE_LAYOUT_LOCKED)
        ret |= (state->components.locked_ಗುಂಪು == (int32_t) idx);

    ಹಿಂತಿರುಗಿ ret;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
ಪೂರ್ಣಾಂಕ
xkb_state_layout_name_is_ಸಕ್ರಿಯ(ರಚನೆ xkb_state *state, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
                                ಎಣಿಕೆ xkb_state_component type)
{
    ಸ್ಥಿರಾಂಕ xkb_layout_ಸೂಚ್ಯಂಕ_t idx =
        xkb_keymap_layout_get_ಸೂಚ್ಯಂಕ(state->keymap, name);

    ಆದರೆ (idx == XKB_LAYOUT_INVALID)
        ಹಿಂತಿರುಗಿ -1;

    ಹಿಂತಿರುಗಿ xkb_state_layout_ಸೂಚ್ಯಂಕ_is_ಸಕ್ರಿಯ(state, idx, type);
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
ಪೂರ್ಣಾಂಕ
xkb_state_led_ಸೂಚ್ಯಂಕ_is_ಸಕ್ರಿಯ(ರಚನೆ xkb_state *state, xkb_led_ಸೂಚ್ಯಂಕ_t idx)
{
    ಆದರೆ (idx >= state->keymap->ಅಂಕೆ_leds ||
        state->keymap->leds[idx].name == XKB_ATOM_NONE)
        ಹಿಂತಿರುಗಿ -1;

    ಹಿಂತಿರುಗಿ !!(state->components.leds & (UINT32_C(1) << idx));
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
ಪೂರ್ಣಾಂಕ
xkb_state_led_name_is_ಸಕ್ರಿಯ(ರಚನೆ xkb_state *state, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
    ಸ್ಥಿರಾಂಕ xkb_led_ಸೂಚ್ಯಂಕ_t idx = xkb_keymap_led_get_ಸೂಚ್ಯಂಕ(state->keymap, name);

    ಆದರೆ (idx == XKB_LED_INVALID)
        ಹಿಂತಿರುಗಿ -1;

    ಹಿಂತಿರುಗಿ xkb_state_led_ಸೂಚ್ಯಂಕ_is_ಸಕ್ರಿಯ(state, idx);
}

/**
 * See:
 * - XkbTranslateKeyCode(3), mod_rtrn return value, from libX11.
 * - MyEnhancedXkbTranslateKeyCode(), a modification of the above, from GTK+.
 */
ಸ್ಥಿರ xkb_mod_ಮುಖವಾಡ_t
ಕೀಲಿ_get_consumed(ರಚನೆ xkb_state *state, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                 ಎಣಿಕೆ xkb_consumed_ಬಹುಲಕ ಬಹುಲಕ)
{
    ಸ್ಥಿರಾಂಕ xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುಂಪು =
        xkb_state_ಕೀಲಿ_get_layout(state, ಕೀಲಿ->keycode);
    ಆದರೆ (ಗುಂಪು == XKB_LAYOUT_INVALID)
        ಹಿಂತಿರುಗಿ 0;

    xkb_mod_ಮುಖವಾಡ_t preserve = 0;
    xkb_mod_ಮುಖವಾಡ_t consumed = 0;

    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು* ಸ್ಥಿರಾಂಕ matching_ನಮೂದು =
        get_ನಮೂದು_for_ಕೀಲಿ_state(state, ಕೀಲಿ, ಗುಂಪು);
    ಆದರೆ (matching_ನಮೂದು)
        preserve = matching_ನಮೂದು->preserve.ಮುಖವಾಡ;

    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type* ಸ್ಥಿರಾಂಕ type = ಕೀಲಿ->groups[ಗುಂಪು].type;
    ಆಯ್ಕೆ (ಬಹುಲಕ) {
    ಸ್ಥಿತಿ XKB_CONSUMED_ಬಹುಲಕ_XKB:
        consumed = type->mods.ಮುಖವಾಡ;
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ XKB_CONSUMED_ಬಹುಲಕ_GTK: {
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು* ಸ್ಥಿರಾಂಕ no_mods_ನಮೂದು =
            get_ನಮೂದು_for_mods(type, 0);
        ಸ್ಥಿರಾಂಕ xkb_level_ಸೂಚ್ಯಂಕ_t no_mods_leveli = no_mods_ನಮೂದು
                                               ? no_mods_ನಮೂದು->level
                                               : 0;
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_level* ಸ್ಥಿರಾಂಕ no_mods_level =
            &ಕೀಲಿ->groups[ಗುಂಪು].ಮಟ್ಟಗಳು[no_mods_leveli];

        ಗಾಗಿ (darray_ಗಾತ್ರ_t i = 0; i < type->ಅಂಕೆ_entries; i++) {
            ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು* ಸ್ಥಿರಾಂಕ ನಮೂದು = &type->entries[i];
            ಆದರೆ (!ನಮೂದು_is_ಸಕ್ರಿಯ(ನಮೂದು))
                ಮುಂದುವರಿ;

            ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_level* ಸ್ಥಿರಾಂಕ level =
                &ಕೀಲಿ->groups[ಗುಂಪು].ಮಟ್ಟಗಳು[ನಮೂದು->level];
            ಆದರೆ (Xkb_ಮಟ್ಟಗಳು_Same_Syms(level, no_mods_level))
                ಮುಂದುವರಿ;

            ಆದರೆ (ನಮೂದು == matching_ನಮೂದು || one_ಬಿಟ್_ಗಣ(ನಮೂದು->mods.ಮುಖವಾಡ))
                consumed |= ನಮೂದು->mods.ಮುಖವಾಡ & ~ನಮೂದು->preserve.ಮುಖವಾಡ;
        }
        ನಿಲ್ಲಿಸು;
    }
    }

    ಹಿಂತಿರುಗಿ consumed & ~preserve;
}

ಪೂರ್ಣಾಂಕ
xkb_state_mod_ಸೂಚ್ಯಂಕ_is_consumed2(ರಚನೆ xkb_state *state, xkb_keycode_t kc,
                                 xkb_mod_ಸೂಚ್ಯಂಕ_t idx,
                                 ಎಣಿಕೆ xkb_consumed_ಬಹುಲಕ ಬಹುಲಕ)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ* ಸ್ಥಿರಾಂಕ ಕೀಲಿ = Xkb_ಕೀಲಿ(state->keymap, kc);

    ಆದರೆ (unlikely(!ಕೀಲಿ || idx >= xkb_keymap_ಅಂಕೆ_mods(state->keymap)))
        ಹಿಂತಿರುಗಿ -1;

    ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t mapping = state->keymap->mods.mods[idx].mapping;
    ಆದರೆ (!mapping) {
        /* Modifier not mapped */
        ಹಿಂತಿರುಗಿ 0;
    }
    ಹಿಂತಿರುಗಿ (mapping & ಕೀಲಿ_get_consumed(state, ಕೀಲಿ, ಬಹುಲಕ)) == mapping;
}

ಪೂರ್ಣಾಂಕ
xkb_state_mod_ಸೂಚ್ಯಂಕ_is_consumed(ರಚನೆ xkb_state *state, xkb_keycode_t kc,
                                xkb_mod_ಸೂಚ್ಯಂಕ_t idx)
{
    ಹಿಂತಿರುಗಿ xkb_state_mod_ಸೂಚ್ಯಂಕ_is_consumed2(state, kc, idx,
                                            XKB_CONSUMED_ಬಹುಲಕ_XKB);
}

xkb_mod_ಮುಖವಾಡ_t
xkb_state_mod_ಮುಖವಾಡ_remove_consumed(ರಚನೆ xkb_state *state, xkb_keycode_t kc,
                                   xkb_mod_ಮುಖವಾಡ_t ಮುಖವಾಡ)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ* ಸ್ಥಿರಾಂಕ ಕೀಲಿ = Xkb_ಕೀಲಿ(state->keymap, kc);

    ಆದರೆ (!ಕೀಲಿ)
        ಹಿಂತಿರುಗಿ 0;

    ಹಿಂತಿರುಗಿ resolve_ಗಮ್ಯ_canonical_mods(state->keymap, ಮುಖವಾಡ) &
           ~ಕೀಲಿ_get_consumed(state, ಕೀಲಿ, XKB_CONSUMED_ಬಹುಲಕ_XKB);
}

xkb_mod_ಮುಖವಾಡ_t
xkb_state_ಕೀಲಿ_get_consumed_mods2(ರಚನೆ xkb_state *state, xkb_keycode_t kc,
                                 ಎಣಿಕೆ xkb_consumed_ಬಹುಲಕ ಬಹುಲಕ)
{
    ಆಯ್ಕೆ (ಬಹುಲಕ) {
    ಸ್ಥಿತಿ XKB_CONSUMED_ಬಹುಲಕ_XKB:
    ಸ್ಥಿತಿ XKB_CONSUMED_ಬಹುಲಕ_GTK:
        ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
        log_err_func(state->keymap->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                     "unrecognized consumed modifiers mode: %d\n", ಬಹುಲಕ);
        ಹಿಂತಿರುಗಿ 0;
    }

    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ* ಸ್ಥಿರಾಂಕ ಕೀಲಿ = Xkb_ಕೀಲಿ(state->keymap, kc);
    ಆದರೆ (!ಕೀಲಿ)
        ಹಿಂತಿರುಗಿ 0;

    ಹಿಂತಿರುಗಿ ಕೀಲಿ_get_consumed(state, ಕೀಲಿ, ಬಹುಲಕ);
}

xkb_mod_ಮುಖವಾಡ_t
xkb_state_ಕೀಲಿ_get_consumed_mods(ರಚನೆ xkb_state *state, xkb_keycode_t kc)
{
    ಹಿಂತಿರುಗಿ xkb_state_ಕೀಲಿ_get_consumed_mods2(state, kc, XKB_CONSUMED_ಬಹುಲಕ_XKB);
}

/*******************************************************************************
 * Server state machine API
 ******************************************************************************/

/** Entry to keep track of the state of an overlaid key */
ರಚನೆ xkb_overlaid_ಕೀಲಿ {
    /** The source key being overlaid */
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *old;
    /** The target key */
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *new;
    /** Counter of key currently down */
    ಪೂರ್ಣಾಂಕ refcnt;
};

ರಚನೆ xkb_shortcuts_config_ನಮೂದು {
    /** Real modifier mask to trigger shortcuts tweaks */
    xkb_mod_ಮುಖವಾಡ_t mods;
    /** Target layout */
    xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುರಿ;
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
ರಚನೆ xkb_machine {
    /** Inherits from `xkb_server_state` */
    ರಚನೆ xkb_server_state ತಳ;

    /* Extra server-specific state data */

    /** Keyboard overlays handling */
    ರಚನೆ {
        /**
         * Current overlaid keys
         *
         * When a key is overlaid, it remained overlaid in this configuration
         * until totally released, i.e. every key press was matched with the
         * corresponding key release.
         */
        darray(ರಚನೆ xkb_overlaid_ಕೀಲಿ) ಕೀಲಿಗಳು;
        /**
         * Activation order of the overlay
         *
         * Overlays indices are stored 1-indexed in nibbles: the lowest
         * nibble corresponds to the latest activated index.
         */
        uint32_t ಕ್ರಮ;
        /** Current enabled overlays mask */
        xkb_overlay_ಮುಖವಾಡ_t enabled;
    } overlays;

    /** Configuration */
    ರಚನೆ machine_config {
        /** Modifiers remapping */
        ರಚನೆ machine_modifiers_config {
            /** Real modifier mask */
            xkb_mod_ಮುಖವಾಡ_t ಮುಖವಾಡ;
            /** Modifiers re-mappings */
            darray_ಗಾತ್ರ_t mappings_ಅಂಕೆ;
            ರಚನೆ machine_mods_mapping {
                xkb_mod_ಮುಖವಾಡ_t ಮೂಲ;
                xkb_mod_ಮುಖವಾಡ_t ಗುರಿ;
            } * mappings ATTR_COUNTED_BY(mappings_ಅಂಕೆ);
        } modifiers;

        /** Shortcuts tweak */
        ರಚನೆ machine_shortcuts_config {
            /** Substitution entries */
            ರಚನೆ xkb_shortcuts_config_ನಮೂದು *entries;
        } shortcuts;
    } config;

    /** Mouse keys handling */
    ರಚನೆ {
        /** Default pointer button */
        xkb_ಸೂಚಕ_button_ಸೂಚ್ಯಂಕ_t default_button;
        /** Mask of the locked buttons */
        xkb_ಸೂಚಕ_button_ಮುಖವಾಡ_t locked_buttons;
    } mouse;

    ಎಣಿಕೆ xkb_machine_flags flags;
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ darray(ರಚನೆ machine_mods_mapping) machine_mods_mappings;

ರಚನೆ xkb_machine_builder {
    ರಚನೆ xkb_keymap *keymap;

    /** Controls */
    ರಚನೆ {
        /** Accessibility flags */
        ರಚನೆ {
            ಎಣಿಕೆ xkb_a11y_flags affect_flags;
            ಎಣಿಕೆ xkb_a11y_flags flags;
        } a11y;
    } controls;

    /** Modifiers re-mapping */
    machine_mods_mappings mods;

    /** Shortcuts tweak */
    ರಚನೆ xkb_shortcuts_config_options {
        /** Substitution entries */
        darray(ರಚನೆ xkb_shortcuts_config_ನಮೂದು) entries;
    } shortcuts;

    ಎಣಿಕೆ xkb_machine_flags machine_flags;
    ಎಣಿಕೆ xkb_machine_builder_flags builder_flags;
    ಪೂರ್ಣಾಂಕ refcnt;
};

ರಚನೆ xkb_machine_builder *
xkb_machine_builder_new(
    ರಚನೆ xkb_keymap * ಸೀಮಿತ keymap,
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_config * ಸೀಮಿತ config,
    ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ * ಸೀಮಿತ ಸ್ಥಿತಿ_2
)
{
    /* Handle default configuration */
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_config default_config = {
        .ಗಾತ್ರ_2 = ಗಾತ್ರ(default_config)
    };
    ಆದರೆ (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_3 = xkb_check_state_abi(config);
    ಆದರೆ (ಸ್ಥಿತಿ_3) {
        xkb_log_abi_error(keymap->ctx, __func__, ಸ್ಥಿತಿ_3);
        ಆದರೆ (ಸ್ಥಿತಿ_2)
            *ಸ್ಥಿತಿ_2 = ಸ್ಥಿತಿ_3;
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    /* Sanitize input */
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_machine_builder_flags invalid_builder_flags
        = config->builder_flags
        & ~(ಎಣಿಕೆ xkb_machine_builder_flags)XKB_MACHINE_BUILDER_FLAGS_ಮೌಲ್ಯಗಳು;
    ಆದರೆ (invalid_builder_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS_,
                     "unrecognized machine builder flags: 0x%x\n",
                     invalid_builder_flags);
        ಆದರೆ (ಸ್ಥಿತಿ_2)
            *ಸ್ಥಿತಿ_2 = XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS;
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_machine_flags invalid_machine_flags
        = (ಎಣಿಕೆ xkb_machine_flags)config->machine_flags
        & ~(ಎಣಿಕೆ xkb_machine_flags)XKB_MACHINE_FLAGS_ಮೌಲ್ಯಗಳು;
    ಆದರೆ (invalid_machine_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS_,
                     "unrecognized state machine flags: 0x%x\n",
                     invalid_machine_flags);
        ಆದರೆ (ಸ್ಥಿತಿ_2)
            *ಸ್ಥಿತಿ_2 = XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS;
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ರಚನೆ xkb_machine_builder * ಸ್ಥಿರಾಂಕ builder = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(*builder));
    ಆದರೆ (!builder) {
        log_err_func1(keymap->ctx, XKB_ERROR_ಹಂಚಿಕೆ_FAILURE_2,
                      "cannot allocate machine builder\n");
        ಆದರೆ (ಸ್ಥಿತಿ_2)
            *ಸ್ಥಿತಿ_2 = XKB_ERROR_ಹಂಚಿಕೆ_FAILURE;
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    *builder = (ರಚನೆ xkb_machine_builder) {
        .keymap = xkb_keymap_ref(keymap),
        .refcnt = 1,
        .builder_flags = (ಎಣಿಕೆ xkb_machine_builder_flags)config->builder_flags,
        .machine_flags = (ಎಣಿಕೆ xkb_machine_flags)config->machine_flags,
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

    ಆದರೆ (ಸ್ಥಿತಿ_2)
        *ಸ್ಥಿತಿ_2 = XKB_SUCCESS;

    ಹಿಂತಿರುಗಿ builder;
}

ರಚನೆ xkb_machine_builder *
xkb_machine_builder_ref(ರಚನೆ xkb_machine_builder *builder)
{
    ಪ್ರತಿಜ್ಞಾ(builder->refcnt > 0);
    builder->refcnt++;
    ಹಿಂತಿರುಗಿ builder;
}

ಶೂನ್ಯ
xkb_machine_builder_unref(ರಚನೆ xkb_machine_builder *builder)
{
    ಪ್ರತಿಜ್ಞಾ(!builder || builder->refcnt > 0);
    ಆದರೆ (!builder || --builder->refcnt > 0)
        ಹಿಂತಿರುಗಿ;

    darray_free(builder->shortcuts.entries);
    darray_free(builder->mods);
    xkb_keymap_unref(builder->keymap);
    ಬಿಡುಗಡೆ(builder);
}

ರಚನೆ xkb_keymap *
xkb_machine_builder_get_keymap(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder *builder)
{
    /* Reference count is not updated. See API doc. */
    ಹಿಂತಿರುಗಿ builder->keymap;
}

ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
xkb_machine_builder_update_a11y(
    ರಚನೆ xkb_machine_builder * ಸೀಮಿತ builder,
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_a11y_update * ಸೀಮಿತ update)
{
    /* Check ABI compatibility */
    ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2 = xkb_check_state_abi(update);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != XKB_SUCCESS) {
        xkb_log_abi_error(builder->keymap->ctx, __func__, ಸ್ಥಿತಿ_2);
        ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
    }

    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_a11y_flags invalid_flags =
        ~(ಎಣಿಕೆ xkb_a11y_flags)XKB_A11_Y_FLAGS_ಮೌಲ್ಯಗಳು;

    ಆದರೆ (update->affect_flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y affected flags: %#x\n",
                __func__, update->affect_flags & invalid_flags);
        ಹಿಂತಿರುಗಿ XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }
    ಆದರೆ (update->flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y flags: %#x\n",
                __func__, update->flags & invalid_flags);
        ಹಿಂತಿರುಗಿ XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }

    builder->controls.a11y.affect_flags |= update->affect_flags;
    builder->controls.a11y.flags &= ~update->affect_flags;
    builder->controls.a11y.flags |= (update->flags & update->affect_flags);

    ಹಿಂತಿರುಗಿ XKB_SUCCESS;
}

ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
xkb_machine_builder_update_mods_remap(
    ರಚನೆ xkb_machine_builder * ಸೀಮಿತ builder,
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_mods_remap_update * ಸೀಮಿತ update
)
{
    ಆದರೆ (!update->ಮೂಲ) {
        ಆದರೆ (!update->ಗುರಿ) {
            /* Reset mappings */
            darray_resize(builder->mods, 0);
            ಹಿಂತಿರುಗಿ XKB_SUCCESS;
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಹಿಂತಿರುಗಿ XKB_ERROR_UNSUPPORTED_MODIFIER_ಮುಖವಾಡ;
        }
    }

    /* Check the modifiers against the keymap */
    ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t invalid = ~builder->keymap->canonical_state_ಮುಖವಾಡ;
    ಆದರೆ ((update->ಮೂಲ & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ಮುಖವಾಡ,
                "%s: Invalid source modifiers: 0x%"PRIx32"\n",
                __func__, update->ಮೂಲ);
        ಹಿಂತಿರುಗಿ XKB_ERROR_UNSUPPORTED_MODIFIER_ಮುಖವಾಡ;
    }
    ಆದರೆ ((update->ಗುರಿ & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ಮುಖವಾಡ,
                "%s: Invalid target modifiers: 0x%"PRIx32"\n",
                __func__, update->ಗುರಿ);
        ಹಿಂತಿರುಗಿ XKB_ERROR_UNSUPPORTED_MODIFIER_ಮುಖವಾಡ;
    }

    ರಚನೆ machine_mods_mapping *mapping = ಖಾಲಿ;
    darray_ಗಾತ್ರ_t m = 0;
    darray_enumerate(m, mapping, builder->mods) {
        ಆದರೆ (mapping->ಮೂಲ == update->ಮೂಲ) {
            ಆದರೆ (!update->ಗುರಿ) {
                /* Remove mapping */
                darray_remove(builder->mods, m);
            } ಇಲ್ಲದಿದ್ದರೆ {
                /* Update mapping */
                mapping->ಗುರಿ = update->ಗುರಿ;
            }
            ಹಿಂತಿರುಗಿ XKB_SUCCESS;
        }
    }

    ಆದರೆ (update->ಗುರಿ) {
        /* Append new mapping */
        darray_append(
            builder->mods,
            (ರಚನೆ machine_mods_mapping) {
                .ಮೂಲ = update->ಮೂಲ,
                .ಗುರಿ = update->ಗುರಿ
            }
        );
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* Ignore missing mapping */
    }

    ಹಿಂತಿರುಗಿ XKB_SUCCESS;
}

ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
xkb_machine_builder_update_shortcut_override(
    ರಚನೆ xkb_machine_builder * ಸೀಮಿತ builder,
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder_shortcut_override_update * ಸೀಮಿತ update
)
{
    ರಚನೆ xkb_keymap *keymap = builder->keymap;

    /* Check the modifiers against the keymap */
    ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t invalid = ~keymap->canonical_state_ಮುಖವಾಡ;
    ಆದರೆ ((update->affect_mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ಮುಖವಾಡ,
                "%s: Invalid affected modifiers: 0x%"PRIx32"\n",
                __func__, update->affect_mods);
        ಹಿಂತಿರುಗಿ XKB_ERROR_UNSUPPORTED_MODIFIER_ಮುಖವಾಡ;
    }
    ಆದರೆ ((update->mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ಮುಖವಾಡ,
                "%s: Invalid modifiers: 0x%"PRIx32"\n",
                __func__, update->mods);
        ಹಿಂತಿರುಗಿ XKB_ERROR_UNSUPPORTED_MODIFIER_ಮುಖವಾಡ;
    }

    /* Check the layout indices against the keymap */
    ಆದರೆ (update->ಮೂಲ >= keymap->ಅಂಕೆ_groups &&
        update->ಮೂಲ != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ_2,
                "%s: Invalid source layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->ಅಂಕೆ_groups, update->ಮೂಲ + 1);
        ಹಿಂತಿರುಗಿ XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ;
    }
    ಆದರೆ (update->ಗುರಿ >= keymap->ಅಂಕೆ_groups &&
        update->ಗುರಿ != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ_2,
                "%s: Invalid target layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->ಅಂಕೆ_groups, update->ಗುರಿ + 1);
        ಹಿಂತಿರುಗಿ XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ;
    }

    ರಚನೆ xkb_shortcuts_config_options * ಸ್ಥಿರಾಂಕ config = &builder->shortcuts;

    /* Resize array & initialize new entries, if relevant */
    ಆದರೆ (update->ಮೂಲ >= darray_ಗಾತ್ರ(config->entries) &&
        update->ಮೂಲ != XKB_LAYOUT_INVALID) {
        ಆದರೆ (update->ಗುರಿ == update->ಮೂಲ) {
            /* Skip default setting */
            ಹಿಂತಿರುಗಿ XKB_SUCCESS;
        }
        xkb_layout_ಸೂಚ್ಯಂಕ_t new = darray_ಗಾತ್ರ(config->entries);
        darray_resize(config->entries, update->ಮೂಲ + 1);
        ಗಾಗಿ (; new <= update->ಮೂಲ; new++) {
            darray_ಅಂಶ(config->entries, new) =
                (ರಚನೆ xkb_shortcuts_config_ನಮೂದು) {
                    .ಗುರಿ = XKB_LAYOUT_INVALID,
                    .mods = 0
                };
        }
    }

    ರಚನೆ xkb_shortcuts_config_ನಮೂದು *ನಮೂದು;
    xkb_layout_ಸೂಚ್ಯಂಕ_t ಮೂಲ = (update->ಮೂಲ == XKB_LAYOUT_INVALID)
        ? 0               /* Loop over all entries */
        : update->ಮೂಲ; /* Single entry */
    darray_foreach_from(ನಮೂದು, config->entries, ಮೂಲ) {
        ಆದರೆ (update->ಮೂಲ == XKB_LAYOUT_INVALID &&
            ನಮೂದು->ಗುರಿ == XKB_LAYOUT_INVALID) {
            /* Skip previous deactivated entry */
            ಮೂಲ++;
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (update->affect_mods) {
            ನಮೂದು->mods &= ~update->affect_mods;
            ನಮೂದು->mods |= (update->affect_mods & update->mods);
        }

        ಆದರೆ (update->ಮೂಲ != XKB_LAYOUT_INVALID) {
            ನಮೂದು->ಗುರಿ = (update->ಗುರಿ == ಮೂಲ)
                ? XKB_LAYOUT_INVALID
                : update->ಗುರಿ;
            /* Single entry: exit loop */
            ನಿಲ್ಲಿಸು;
        }
        ಮೂಲ++;
    }

    ಹಿಂತಿರುಗಿ XKB_SUCCESS;
}

/** Compare 2 modifiers combinations */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
cmp_mod_masks(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b)
{
    ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t m1 = *((xkb_mod_ಮುಖವಾಡ_t *) a);
    ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t m2 = *((xkb_mod_ಮುಖವಾಡ_t *) b);
    ಆದರೆ (m1 == m2)
        ಹಿಂತಿರುಗಿ 0;

    ಆದರೆ (!m1)
        ಹಿಂತಿರುಗಿ +1;
    ಆದರೆ (!m2)
        ಹಿಂತಿರುಗಿ -1;

    ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t overlap = (m1 & m2);
    ಆದರೆ (overlap == m1)
        ಹಿಂತಿರುಗಿ +1; /* m1 is a subset of m2 */
    ಆದರೆ (overlap == m2)
        ಹಿಂತಿರುಗಿ -1; /* m2 is a subset of m1 */

    /* Compare active modifiers numbers */
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ count1 = popcount32(m1);
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ count2 = popcount32(m2);
    ಆದರೆ (count1 > count2)
        ಹಿಂತಿರುಗಿ -1;
    ಆದರೆ (count1 < count2)
        ಹಿಂತಿರುಗಿ +1;

    /* Give priority to real modifiers */
    ಹಿಂತಿರುಗಿ (m1 < m2) ? -1 : +1;
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
machine_ಗಣ_mods(ರಚನೆ xkb_machine *sm,
                 ಸ್ಥಿರಾಂಕ machine_mods_mappings *raw_mappings)
{
    ಆದರೆ (!darray_ಖಾಲಿಗಳು(*raw_mappings)) {
        machine_mods_mappings mappings = darray_new();
        xkb_mod_ಮುಖವಾಡ_t ಮುಖವಾಡ = 0;
        ಸ್ಥಿರಾಂಕ ರಚನೆ machine_mods_mapping *mapping;
        ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t invalid =
            ~sm->ತಳ.ತಳ.keymap->canonical_state_ಮುಖವಾಡ;
        darray_foreach(mapping, *raw_mappings) {
            ಆದರೆ (!mapping->ಮೂಲ || !mapping->ಗುರಿ ||
                (mapping->ಮೂಲ & invalid) || (mapping->ಗುರಿ & invalid))
                ಮುಂದುವರಿ;
            darray_append(mappings, *mapping);
            ಮುಖವಾಡ |= mapping->ಮೂಲ;
        }

        /*
         * Re-order the mappings, so that combinations have higher priority
         * if they are superset of other combinations.
         * For other type of overlaps, see the sorting function.
         */
        qsort(darray_ವಸ್ತುಗಳು(mappings), darray_ಗಾತ್ರ(mappings),
              ಗಾತ್ರ(*darray_ವಸ್ತುಗಳು(mappings)), &cmp_mod_masks);

        darray_steal(
            mappings,
            &sm->config.modifiers.mappings,
            &sm->config.modifiers.mappings_ಅಂಕೆ
        );
        sm->config.modifiers.ಮುಖವಾಡ = ಮುಖವಾಡ;
    } ಇಲ್ಲದಿದ್ದರೆ {
        sm->config.modifiers = (ರಚನೆ machine_modifiers_config) {
            .mappings = ಖಾಲಿ,
            .mappings_ಅಂಕೆ = 0,
            .ಮುಖವಾಡ = 0
        };
    }

    ಹಿಂತಿರುಗಿ XKB_SUCCESS;
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
machine_ಗಣ_shortcuts(
    ರಚನೆ xkb_machine * ಸೀಮಿತ sm,
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_shortcuts_config_options * ಸೀಮಿತ options
)
{
    ಆದರೆ (darray_ಖಾಲಿಗಳು(options->entries)) {
        sm->config.shortcuts = (ರಚನೆ machine_shortcuts_config) {
            .entries = ಖಾಲಿ
        };
        ಹಿಂತಿರುಗಿ XKB_SUCCESS;
    }

    ರಚನೆ xkb_keymap * ಸ್ಥಿರಾಂಕ keymap = sm->ತಳ.ತಳ.keymap;

    ರಚನೆ xkb_shortcuts_config_ನಮೂದು * entries =
        ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(keymap->ಅಂಕೆ_groups, ಗಾತ್ರ(*entries));
    ಆದರೆ (!entries)
        ಹಿಂತಿರುಗಿ XKB_ERROR_ಹಂಚಿಕೆ_FAILURE;

    ಸ್ಥಿರಾಂಕ xkb_layout_ಸೂಚ್ಯಂಕ_t ಎಣಿಕೆ_2 = MIN(
        keymap->ಅಂಕೆ_groups,
        (xkb_layout_ಸೂಚ್ಯಂಕ_t) darray_ಗಾತ್ರ(options->entries)
    );

    bool some_entries = false;
    ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t l = 0; l < ಎಣಿಕೆ_2; l++) {
        /* Sanitize layout target */
        ಸ್ಥಿರಾಂಕ xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುರಿ =
            darray_ಅಂಶ(options->entries, l).ಗುರಿ;
        /* Sanitize modifier mask */
        ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t mods = (
            darray_ಅಂಶ(options->entries, l).mods &
            sm->ತಳ.ತಳ.keymap->canonical_state_ಮುಖವಾಡ
        );
        ಆದರೆ (ಗುರಿ < keymap->ಅಂಕೆ_groups && ಗುರಿ != l && mods) {
            entries[l] = (ರಚನೆ xkb_shortcuts_config_ನಮೂದು) {
                .ಗುರಿ = ಗುರಿ,
                .mods = mods
            };
            some_entries = true;
        } ಇಲ್ಲದಿದ್ದರೆ {
            entries[l] = (ರಚನೆ xkb_shortcuts_config_ನಮೂದು) {
                .ಗುರಿ = XKB_LAYOUT_INVALID,
                .mods = 0
            };
            ಮುಂದುವರಿ;
        }
    }

    ಆದರೆ (!some_entries) {
        ಬಿಡುಗಡೆ(entries);
        entries = ಖಾಲಿ;
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* Initialize groups with no entries */
        ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t l = ಎಣಿಕೆ_2; l < keymap->ಅಂಕೆ_groups; l++) {
            entries[l] = (ರಚನೆ xkb_shortcuts_config_ನಮೂದು) {
                .ಗುರಿ = XKB_LAYOUT_INVALID,
                .mods = 0
            };
        }
    }

    sm->config.shortcuts = (ರಚನೆ machine_shortcuts_config) {
        .entries = entries
    };
    ಹಿಂತಿರುಗಿ XKB_SUCCESS;
}

ರಚನೆ xkb_machine *
xkb_machine_new(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine_builder * ಸೀಮಿತ builder,
                ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ * ಸೀಮಿತ ಸ್ಥಿತಿ_2)
{
    ರಚನೆ xkb_machine * ಸ್ಥಿರಾಂಕ machine = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(*machine));
    ಆದರೆ (!machine) {
        log_err_func1(builder->keymap->ctx, XKB_ERROR_ಹಂಚಿಕೆ_FAILURE_2,
                      "cannot allocate machine\n");
        ಆದರೆ (ಸ್ಥಿತಿ_2)
            *ಸ್ಥಿತಿ_2 = XKB_ERROR_ಹಂಚಿಕೆ_FAILURE;
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    xkb_server_state_init(&machine->ತಳ, builder->keymap, SERVER_STATE,
                          builder->controls.a11y.affect_flags,
                          builder->controls.a11y.flags);
    machine->flags = builder->machine_flags;
    ಆದರೆ (!(machine->flags &
          XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_ಕೀಲಿ_FIX))
    {
        machine->ತಳ.flags |= STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_ಕೀಲಿ;
    }
    ಆದರೆ (machine->flags & XKB_MACHINE_UNLOCK_MODS_ON_PRESS) {
        machine->ತಳ.flags |= STATE_UNLOCK_MODS_ON_PRESS;
    }

    ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_3;
    ಆದರೆ ((ಸ್ಥಿತಿ_3 = machine_ಗಣ_mods(machine, &builder->mods)) != XKB_SUCCESS ||
        (ಸ್ಥಿತಿ_3 = machine_ಗಣ_shortcuts(machine, &builder->shortcuts)) !=
        XKB_SUCCESS) {
        *ಸ್ಥಿತಿ_2 = ಸ್ಥಿತಿ_3;
        ಹೋಗು error;
    }

    darray_init(machine->overlays.ಕೀಲಿಗಳು);
    machine->mouse.default_button = XKB_ಸೂಚಕ_BUTTON_MIN;

    ಆದರೆ (ಸ್ಥಿತಿ_2)
        *ಸ್ಥಿತಿ_2 = XKB_SUCCESS;

    ಹಿಂತಿರುಗಿ machine;

error:
    xkb_machine_unref(machine);
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ರಚನೆ xkb_machine *
xkb_machine_ref(ರಚನೆ xkb_machine *sm)
{
    ಪ್ರತಿಜ್ಞಾ(sm->ತಳ.ತಳ.refcnt > 0);
    sm->ತಳ.ತಳ.refcnt++;
    ಹಿಂತಿರುಗಿ sm;
}

ಶೂನ್ಯ
xkb_machine_unref(ರಚನೆ xkb_machine *sm)
{
    ಪ್ರತಿಜ್ಞಾ(!sm || sm->ತಳ.ತಳ.refcnt > 0);
    ಆದರೆ (!sm || --sm->ತಳ.ತಳ.refcnt > 0)
        ಹಿಂತಿರುಗಿ;

    xkb_state_destroy(&sm->ತಳ.ತಳ);
    darray_free(sm->overlays.ಕೀಲಿಗಳು);
    ಬಿಡುಗಡೆ(sm->config.shortcuts.entries);
    ಬಿಡುಗಡೆ(sm->config.modifiers.mappings);
    ಬಿಡುಗಡೆ(sm);
}

ರಚನೆ xkb_keymap *
xkb_machine_get_keymap(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    ಹಿಂತಿರುಗಿ sm->ತಳ.ತಳ.keymap;
}

ರಚನೆ xkb_state *
xkb_machine_get_state(ರಚನೆ xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    ಹಿಂತಿರುಗಿ (ರಚನೆ xkb_state *)sm;
}

ರಚನೆ xkb_state *
xkb_state_new_from_machine(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_machine * ಸೀಮಿತ machine,
                           ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ * ಸೀಮಿತ ಸ್ಥಿತಿ_2)
{
    /* Creation */
    ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_3;
    ರಚನೆ xkb_state * ಸ್ಥಿರಾಂಕ state = xkb_state_new_with_ಬಹುಲಕ(
        machine->ತಳ.ತಳ.keymap,
        XKB_STATE_ಬಹುಲಕ_SERVER_QUERY,
        &ಸ್ಥಿತಿ_3
    );
    ಆದರೆ (ಸ್ಥಿತಿ_3 != XKB_SUCCESS) {
        ಆದರೆ (ಸ್ಥಿತಿ_2)
            *ಸ್ಥಿತಿ_2 = ಸ್ಥಿತಿ_3;
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಪ್ರತಿಜ್ಞಾ(state->ಬಹುಲಕ == SERVER_COMPANION);
    ಪ್ರತಿಜ್ಞಾ(state->refcnt == 1);

    /* Copy machine’s state */
    *state = machine->ತಳ.ತಳ;

    /* Restore previous values */
    state->ಬಹುಲಕ = SERVER_COMPANION;
    state->refcnt = 1;

    ಆದರೆ (ಸ್ಥಿತಿ_2)
        *ಸ್ಥಿತಿ_2 = XKB_SUCCESS;
    ಹಿಂತಿರುಗಿ state;
}

ಸ್ಥಿರ ಶೂನ್ಯ
machine_update_overlays(ರಚನೆ xkb_machine *sm)
{
    /*
     * Overlays indices are stored 1-indexed in nibbles: the lowest
     * nibble corresponds to the latest activated index.
     */

    ಸ್ಥಿರಾಂಕ xkb_overlay_ಮುಖವಾಡ_t ಮುಖವಾಡ =
        overlays_from_controls(sm->ತಳ.ತಳ.components.controls);
    xkb_overlay_ಮುಖವಾಡ_t added = ಮುಖವಾಡ & ~sm->overlays.enabled;

    /* Remove overlays no longer enabled and keep relative order */
    uint32_t ಕ್ರಮ = sm->overlays.ಕ್ರಮ;
    ಸ್ಥಿರಾಂಕ xkb_overlay_ಸೂಚ್ಯಂಕ_t overlay_max =
        format_max_overlays(sm->ತಳ.ತಳ.keymap->format);
    ಗಾಗಿ (uint8_t n = 0; n < overlay_max;) {
        xkb_overlay_ಸೂಚ್ಯಂಕ_t overlay_idx = (ಕ್ರಮ >> (n * 4)) & 0xf;
        ಆದರೆ (!overlay_idx)
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿರಾಂಕ xkb_overlay_ಮುಖವಾಡ_t overlay_ಮುಖವಾಡ =
            (xkb_overlay_ಮುಖವಾಡ_t)(1u << (overlay_idx - 1u) /* k is 1-indexed */);
        ಆದರೆ (overlay_ಮುಖವಾಡ & ಮುಖವಾಡ) {
            /* no duplicates */
            added &= ~overlay_ಮುಖವಾಡ;
            n++;
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* remove overlay */
            ಸ್ಥಿರಾಂಕ uint32_t ತಲೆ = (UINT32_C(1) << (n * 4)) - 1;
            ಕ್ರಮ = (ಕ್ರಮ & ತಲೆ) | ((ಕ್ರಮ >> 4) & ~ತಲೆ);
        }
    }

    /* Add new overlays in ascending order */
    ಗಾಗಿ (xkb_overlay_ಸೂಚ್ಯಂಕ_t k = 0; added; k++, added >>= 1) {
        ಆದರೆ (added & 0x1) {
            ಕ್ರಮ <<= 4;
            ಕ್ರಮ |= k + 1u; /* k is 0-indexed */
        }
    }

    sm->overlays.ಕ್ರಮ = ಕ್ರಮ;
    sm->overlays.enabled = ಮುಖವಾಡ;
}

ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
xkb_machine_process_synthetic(ರಚನೆ xkb_machine *sm,
                              ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_synthetic_update *update,
                              ರಚನೆ xkb_events *events)
{
    /* Check ABI compatibility */
    ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2 =
        check_state_update_abi(sm->ತಳ.ತಳ.keymap->ctx, update);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != XKB_SUCCESS)
        ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;

    ರಚನೆ xkb_server_state * ಸ್ಥಿರಾಂಕ state = &sm->ತಳ;
    ಸ್ಥಿರಾಂಕ ರಚನೆ state_components ಹಿಂದಿನ_components = state->ತಳ.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    darray_ಗಾತ್ರ(events->ಸರತಿ) = 0;
    events->ಮುಂದಿನ = 0;

    /* Update parametrized controls first */
    ಆದರೆ (update->layout_policy) {
        ಸ್ಥಿತಿ_2 = state_update_layout_policy(state, update->layout_policy);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != XKB_SUCCESS)
            ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
    }

    ಆದರೆ (update->components) {
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_state_components_update * ಸ್ಥಿರಾಂಕ components =
            update->components;
        /* Update boolean controls first */
        ಆದರೆ (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, events);
        }

        state_update_latched_locked(state, components, events);
    }

    xkb_state_update_derived(&state->ತಳ);

    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_state_component ಬದಲಾಯಿತು = get_state_component_changes(
        &ಹಿಂದಿನ_components, &state->ತಳ.components
    );
    ಆದರೆ (ಬದಲಾಯಿತು) {
        // TODO: latch controls
        ಆದರೆ (ಬದಲಾಯಿತು & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        /* Create event only if some component actually changed */
        darray_append(events->ಸರತಿ, (ರಚನೆ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .ಬದಲಾಯಿತು = ಬದಲಾಯಿತು,
                .components = state->ತಳ.components
            }
        });

        /* Terminate the frame */
        darray_append(events->ಸರತಿ, (ರಚನೆ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_ಚೌಕಟ್ಟು,
        });
    }

    ಹಿಂತಿರುಗಿ XKB_SUCCESS;
}

ಸ್ಥಿರ ssize_t
do_remap_modifiers(ಸ್ಥಿರಾಂಕ ರಚನೆ machine_modifiers_config * ಸೀಮಿತ mappings,
                   ರಚನೆ xkb_state * ಸೀಮಿತ state,
                   ರಚನೆ xkb_events * ಸೀಮಿತ events,
                   ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ * ಸೀಮಿತ ಕೀಲಿ)
{
    ಆದರೆ (!(mappings->ಮುಖವಾಡ & state->components.mods))
        ಹಿಂತಿರುಗಿ -1;

    ಸ್ಥಿರಾಂಕ xkb_layout_ಸೂಚ್ಯಂಕ_t layout = state_ಕೀಲಿ_get_layout(state, ಕೀಲಿ);
    ಆದರೆ (layout >= ಕೀಲಿ->ಅಂಕೆ_groups)
        ಹಿಂತಿರುಗಿ -1;

    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type * ಸ್ಥಿರಾಂಕ type = ಕೀಲಿ->groups[layout].type;
    xkb_mod_ಮುಖವಾಡ_t affect_mods = 0;
    xkb_mod_ಮುಖವಾಡ_t mods = 0;
    ಗಾಗಿ (darray_ಗಾತ್ರ_t m = 0; m < mappings->mappings_ಅಂಕೆ; m++) {
        ಸ್ಥಿರಾಂಕ ರಚನೆ machine_mods_mapping * ಸ್ಥಿರಾಂಕ mapping =
            &mappings->mappings[m];
        ಆದರೆ (/* Skip not matching active mods */
            (mapping->ಮೂಲ & state->components.mods) == mapping->ಮೂಲ &&
            /* Skip overlap with previous active mappings */
            !(mapping->ಮೂಲ & affect_mods) &&
            /* Skip if the key type uses some of the source modifiers */
            !(mapping->ಮೂಲ & type->mods.ಮುಖವಾಡ)) {
            affect_mods |= mappings->mappings[m].ಮೂಲ;
            mods |= mappings->mappings[m].ಗುರಿ;
        }
    }

    ಆದರೆ (!affect_mods)
        ಹಿಂತಿರುಗಿ -1;

    ರಚನೆ xkb_state new = *state;
    new.components.ತಳ_mods = (new.components.ತಳ_mods & ~affect_mods) | mods;
    new.components.latched_mods = (new.components.latched_mods & ~affect_mods);
    new.components.locked_mods = (new.components.locked_mods & ~affect_mods);
    xkb_state_update_derived(&new);

    ssize_t event_idx = -1;
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_state_component ಬದಲಾಯಿತು =
        get_state_component_changes(&state->components, &new.components);
    ಆದರೆ (ಬದಲಾಯಿತು) {
        event_idx = (ssize_t) darray_ಗಾತ್ರ(events->ಸರತಿ);
        darray_append(events->ಸರತಿ, (ರಚನೆ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = new.components,
                .ಬದಲಾಯಿತು = ಬದಲಾಯಿತು
            }
        });
    }

    state->components.mods = new.components.mods;
    ಹಿಂತಿರುಗಿ event_idx;
}

/**
 * Activate an alternative layout if some modifiers are active, typically
 * `Control`, `Alt` or `Super`.
 *
 * The tweak consists of computing a *base* layout such that the *effective*
 * layout is the corresponding target layout.
 */
ಸ್ಥಿರ ssize_t
do_shortcuts_tweak(ಸ್ಥಿರಾಂಕ ರಚನೆ machine_shortcuts_config *config,
                   ರಚನೆ xkb_state *state,
                   ಸ್ಥಿರಾಂಕ ರಚನೆ state_components *ಹಿಂದಿನ_components,
                   ರಚನೆ xkb_events *events, ssize_t remap_event)
{
    ಆದರೆ (!config->entries) {
        /* No shortcuts tweak */
        ಹಿಂತಿರುಗಿ remap_event;
    }

    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_shortcuts_config_ನಮೂದು * ಸ್ಥಿರಾಂಕ ನಮೂದು =
        &config->entries[state->components.ಗುಂಪು];

    ಆದರೆ (ನಮೂದು->ಗುರಿ != XKB_LAYOUT_INVALID &&
        (ನಮೂದು->mods & state->components.mods)) {
        /*
         * Activate shortcuts tweak:
         * 1. The real base group is saved by the caller, to be restored by
         *    later by a call to undo_tweaks().
         * 2. Compute the new base group so that the resulting effective group
         *    is the target.
         * 3. Recompute derived state components.
         */
        ರಚನೆ xkb_state new = *state;
        ಆದರೆ (remap_event < 0) {
            /* Create new event */
            remap_event = (ssize_t) darray_ಗಾತ್ರ(events->ಸರತಿ);
            darray_append(events->ಸರತಿ, (ರಚನೆ xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = {0},
                    .ಬದಲಾಯಿತು = 0
                }
            });
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* Merge with last remap event */
            new.components =
                darray_ಅಂಶ(events->ಸರತಿ, remap_event).components.components;
        }
        new.components.ತಳ_ಗುಂಪು
            = (int32_t) ನಮೂದು->ಗುರಿ
            - state->components.latched_ಗುಂಪು
            - state->components.locked_ಗುಂಪು;
        xkb_state_update_derived(&new);

        ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_state_component ಬದಲಾಯಿತು =
            get_state_component_changes(ಹಿಂದಿನ_components, &new.components);
        darray_ಅಂಶ(events->ಸರತಿ, remap_event).components.ಬದಲಾಯಿತು = ಬದಲಾಯಿತು;
        darray_ಅಂಶ(events->ಸರತಿ, remap_event).components.components =
            new.components;

        state->components.ಗುಂಪು = new.components.ಗುಂಪು;
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* Deactivated entry or modifiers do not match */
    }
    ಹಿಂತಿರುಗಿ remap_event;
}

ಸ್ಥಿರ ಶೂನ್ಯ
undo_tweaks(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_state *state,
            ಸ್ಥಿರಾಂಕ ರಚನೆ state_components *initial_components,
            ರಚನೆ xkb_events *events)
{
    /* Get last component event */
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_event *event = ಖಾಲಿ;
    darray_foreach_ಹಿಮ್ಮುಖ(event, events->ಸರತಿ) {
        ಆದರೆ (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS)
            ನಿಲ್ಲಿಸು;
    }
    ಆದರೆ (!event)
        ಹಿಂತಿರುಗಿ;

    /* Restore state */
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_state_component ಬದಲಾಯಿತು =
        get_state_component_changes(initial_components,
                                    &event->components.components);
    ಆದರೆ (ಬದಲಾಯಿತು) {
        darray_append(events->ಸರತಿ, (ರಚನೆ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = *initial_components,
                .ಬದಲಾಯಿತು = ಬದಲಾಯಿತು
            }
        });
    }
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *
process_overlayable_ಕೀಲಿ(ರಚನೆ xkb_machine *sm,
                        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ * ಕೀಲಿ,
                        ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction)
{
    /* Check if key is currently overlaid */
    ರಚನೆ xkb_overlaid_ಕೀಲಿ *ನಮೂದು = ಖಾಲಿ;
    ರಚನೆ xkb_overlaid_ಕೀಲಿ *ಲಭ್ಯ_ನಮೂದು = ಖಾಲಿ;
    darray_foreach(ನಮೂದು, sm->overlays.ಕೀಲಿಗಳು) {
        ಆದರೆ (ನಮೂದು->old == ಕೀಲಿ) {
            /* Maintain the key overlaid */
            ಆಯ್ಕೆ (direction) {
            ಸ್ಥಿತಿ XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ:
                ನಮೂದು->refcnt++;
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ XKB_ಕೀಲಿ_REPEATED:
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ XKB_ಕೀಲಿ_ಮೇಲೆ:
            ಪೂರ್ವನಿಯೋಜಿತ:
                ನಮೂದು->refcnt--;
                ನಿಲ್ಲಿಸು;
            }
            ಆದರೆ (ನಮೂದು->refcnt <= 0)
                ನಮೂದು->old = ಖಾಲಿ;
            ಹಿಂತಿರುಗಿ ನಮೂದು->new;
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ನಮೂದು->old && !ಲಭ್ಯ_ನಮೂದು) {
            /* First free spot */
            ಲಭ್ಯ_ನಮೂದು = ನಮೂದು;
        }
    }
    ಆದರೆ (direction == XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ) {
        /* Enable key overlay */

        /*
         * Get overlay index: select the latest activated overlay
         * containing the key.
         */
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *new = ಕೀಲಿ;

        ಆದರೆ (ಕೀಲಿ->overlays & sm->overlays.enabled) {
            /* Some relevant overlay is active */
            ಗಾಗಿ (uint32_t ರಾಶಿ = sm->overlays.ಕ್ರಮ; ರಾಶಿ; ರಾಶಿ >>= 4) {
                static_assert(XKB_OVERLAY_ಎಣಿಕೆ == 8, "");
                ಸ್ಥಿರಾಂಕ xkb_overlay_ಸೂಚ್ಯಂಕ_t overlay = (ರಾಶಿ & 0xf) - 1;
                ಸ್ಥಿರಾಂಕ xkb_overlay_ಮುಖವಾಡ_t ಮುಖವಾಡ =
                    (xkb_overlay_ಮುಖವಾಡ_t)(1u << overlay);
                ಆದರೆ (ಕೀಲಿ->overlays & ಮುಖವಾಡ) {
                    /* Get new key */
                    ಆದರೆ (ಕೀಲಿ->overlays_inline) {
                        /* Stored inline */
                        new = ಕೀಲಿ->overlay_ಕೀಲಿ;
                    } ಇಲ್ಲದಿದ್ದರೆ {
                        /* Stored in sparse array */
                        ಸ್ಥಿರಾಂಕ xkb_overlay_ಮುಖವಾಡ_t ಕೆಳಗಿನ = (xkb_overlay_ಮುಖವಾಡ_t)(
                            ಕೀಲಿ->overlays & (xkb_overlay_ಮುಖವಾಡ_t)(ಮುಖವಾಡ - 1u)
                        );
                        /* index = number of set bits strictly below bit */
                        ಸ್ಥಿರಾಂಕ xkb_overlay_ಸೂಚ್ಯಂಕ_t ಸೂಚ್ಯಂಕ =
                            (xkb_overlay_ಸೂಚ್ಯಂಕ_t)popcount32(ಕೆಳಗಿನ);
                        new = ಕೀಲಿ->overlays_ಕೀಲಿಗಳು[ಸೂಚ್ಯಂಕ];
                    }
                    ನಿಲ್ಲಿಸು;
                }
            }
        } ಇಲ್ಲದಿದ್ದರೆ {
            /*
             * No relevant overlay is active: block future overlays while
             * the key is pressed.
             */
        }

        /* Get new entry */
        ಆದರೆ (ಲಭ್ಯ_ನಮೂದು) {
            /* Re-use a free entry */
            ನಮೂದು = ಲಭ್ಯ_ನಮೂದು;
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* Append a new entry */
            ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t idx = darray_ಗಾತ್ರ(sm->overlays.ಕೀಲಿಗಳು);
            darray_resize(sm->overlays.ಕೀಲಿಗಳು, idx + 1);
            ನಮೂದು = &darray_ಅಂಶ(sm->overlays.ಕೀಲಿಗಳು, idx);
        }

        /* Overlay the key */
        ನಮೂದು->old = ಕೀಲಿ;
        ನಮೂದು->new = new;
        ನಮೂದು->refcnt = 1;
        ಹಿಂತಿರುಗಿ new;
    }

    /* No overlay */
    ಹಿಂತಿರುಗಿ ಕೀಲಿ;
}

ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
xkb_machine_process_ಕೀಲಿ(ರಚನೆ xkb_machine *sm,
                        xkb_keycode_t kc, ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction,
                        ರಚನೆ xkb_events *events)
{
    darray_ಗಾತ್ರ(events->ಸರತಿ) = 0;
    events->ಮುಂದಿನ = 0;

    ರಚನೆ xkb_server_state * ಸ್ಥಿರಾಂಕ state = &sm->ತಳ;
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ * ಕೀಲಿ = Xkb_ಕೀಲಿ(state->ತಳ.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    ಆದರೆ (!ಕೀಲಿ || (direction == XKB_ಕೀಲಿ_REPEATED && !ಕೀಲಿ->repeats))
        ಹಿಂತಿರುಗಿ XKB_SUCCESS;

    ಸ್ಥಿರಾಂಕ ರಚನೆ state_components initial_components = state->ತಳ.components;

    ಆದರೆ (direction == XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ &&
        (sm->ತಳ.ತಳ.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ಕೀಲಿಗಳು) &&
        (sm->ತಳ.a11y_flags & XKB_A11_Y_STICKY_ಕೀಲಿಗಳು_NO_ಏಕಕಾಲಿಕ_ಕೀಲಿಗಳು) &&
        state->ತಳ.components.ತಳ_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ಕೀಲಿಗಳು,
                                      0, events);
    }

    ಆದರೆ (ಕೀಲಿ->overlays)
        ಕೀಲಿ = process_overlayable_ಕೀಲಿ(sm, ಕೀಲಿ, direction);

    ssize_t remap_event = do_remap_modifiers(&sm->config.modifiers,
                                             &state->ತಳ, events, ಕೀಲಿ);

    remap_event = do_shortcuts_tweak(&sm->config.shortcuts, &state->ತಳ,
                                     &initial_components, events, remap_event);

    ಆದರೆ (remap_event >= 0) {
        // TODO: what about merging state event for RedirectKey?
        darray_append(events->ಸರತಿ, (ರಚನೆ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_ಚೌಕಟ್ಟು,
        });
    }

    state->flags |= STATE_REQUIRE_ಕೀಲಿ_EVENT;
    state->ಗಣ_mods = 0;
    state->clear_mods = 0;

    /* Handle key actions */
    xkb_filter_apply_all(state, events, ಕೀಲಿ, direction);

    xkb_mod_ಸೂಚ್ಯಂಕ_t i;
    xkb_mod_ಮುಖವಾಡ_t ಬಿಟ್;
    ಗಾಗಿ (i = 0, ಬಿಟ್ = 1; state->ಗಣ_mods; i++, ಬಿಟ್ <<= 1) {
        ಆದರೆ (state->ಗಣ_mods & ಬಿಟ್) {
            state->mod_ಕೀಲಿ_ಎಣಿಕೆ[i]++;
            state->ತಳ.components.ತಳ_mods |= ಬಿಟ್;
            state->ಗಣ_mods &= ~ಬಿಟ್;
        }
    }

    ಗಾಗಿ (i = 0, ಬಿಟ್ = 1; state->clear_mods; i++, ಬಿಟ್ <<= 1) {
        ಆದರೆ (state->clear_mods & ಬಿಟ್) {
            state->mod_ಕೀಲಿ_ಎಣಿಕೆ[i]--;
            ಆದರೆ (state->mod_ಕೀಲಿ_ಎಣಿಕೆ[i] <= 0) {
                state->ತಳ.components.ತಳ_mods &= ~ಬಿಟ್;
                state->mod_ಕೀಲಿ_ಎಣಿಕೆ[i] = 0;
            }
            state->clear_mods &= ~ಬಿಟ್;
        }
    }

    xkb_state_update_derived(&state->ತಳ);

    /*
     * Insert key event only if actions did not override it, e.g.:
     * - RedirectKey() append its own key event;
     * - MovePointer() and PointerButton() generate pointer events instead
     *   of key events;
     * - TerminateServer() generate a Server event instead of key events.
     */
    ಆದರೆ (state->flags & STATE_REQUIRE_ಕೀಲಿ_EVENT) {
        darray_append(events->ಸರತಿ, (ರಚನೆ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_ಕೀಲಿ,
            .ಕೀಲಿ = {
                .keycode = ಕೀಲಿ->keycode,
                .direction = direction
            }
        });
    }

    ಆದರೆ (remap_event >= 0) {
        // FIXME: fragile if last state change does not restore the state to the remap event
        undo_tweaks(&state->ತಳ, &initial_components, events);
    }

    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_state_component ಬದಲಾಯಿತು = get_state_component_changes(
        &initial_components, &state->ತಳ.components
    );
    ಆದರೆ (ಬದಲಾಯಿತು) {
        ಆದರೆ (ಬದಲಾಯಿತು & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        // TODO: merge with previous state event, if any

        darray_append(events->ಸರತಿ, (ರಚನೆ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = state->ತಳ.components,
                .ಬದಲಾಯಿತು = ಬದಲಾಯಿತು
            }
        });
    }

    ಆದರೆ (!darray_ಖಾಲಿಗಳು(events->ಸರತಿ) &&
        darray_ಅಂಶ(events->ಸರತಿ, darray_ಗಾತ್ರ(events->ಸರತಿ) - 1).type !=
        XKB_EVENT_TYPE_ಚೌಕಟ್ಟು)
    {
        darray_append(events->ಸರತಿ, (ರಚನೆ xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_ಚೌಕಟ್ಟು,
        });
    }

    ಹಿಂತಿರುಗಿ XKB_SUCCESS;
}

ರಚನೆ xkb_events *
xkb_events_new(ರಚನೆ xkb_context * ಸೀಮಿತ context,
               ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_events_config * ಸೀಮಿತ config,
               ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ * ಸೀಮಿತ ಸ್ಥಿತಿ_2)
{
    /* Handle default configuration */
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_events_config default_config = {
        .ಗಾತ್ರ_2 = ಗಾತ್ರ(default_config)
    };
    ಆದರೆ (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ abi_error = xkb_check_state_abi(config);
    ಆದರೆ (abi_error) {
        xkb_log_abi_error(context, __func__, abi_error);
        ಆದರೆ (ಸ್ಥಿತಿ_2)
            *ಸ್ಥಿತಿ_2 = abi_error;
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    /* Sanitize input */
    ಸ್ಥಿರಾಂಕ uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_EVENTS_FLAGS_ಮೌಲ್ಯಗಳು);
    ಆದರೆ (invalid_flags) {
        log_err_func(context, XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS_,
                     "unrecognized events collection flags: 0x%"PRIx32"\n",
                     invalid_flags);
        ಆದರೆ (ಸ್ಥಿತಿ_2)
            *ಸ್ಥಿತಿ_2 = XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS;
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ರಚನೆ xkb_events *events = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(*events));
    ಆದರೆ (!events) {
        log_err_func1(context, XKB_ERROR_ಹಂಚಿಕೆ_FAILURE_2,
                      "cannot allocate state events collection\n");
        ಆದರೆ (ಸ್ಥಿತಿ_2)
            *ಸ್ಥಿತಿ_2 = XKB_ERROR_ಹಂಚಿಕೆ_FAILURE;
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    /* Pre-allocate event queue */
    darray_init(events->ಸರತಿ);
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t ಸರತಿ_initial_ಗಾತ್ರ = 16;
    darray_resize(events->ಸರತಿ, ಸರತಿ_initial_ಗಾತ್ರ);
    // TODO: proper safe darray_resize() with return value
    ಆದರೆ (!darray_ವಸ್ತುಗಳು(events->ಸರತಿ)) {
        log_err_func1(context, XKB_ERROR_ಹಂಚಿಕೆ_FAILURE_2,
                      "cannot allocate state events queue\n");
        xkb_events_destroy(events);
        ಆದರೆ (ಸ್ಥಿತಿ_2)
            *ಸ್ಥಿತಿ_2 = XKB_ERROR_ಹಂಚಿಕೆ_FAILURE;
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }
    darray_ಗಾತ್ರ(events->ಸರತಿ) = 0;

    events->ctx = xkb_context_ref(context);
    events->ಮುಂದಿನ = 0;

    ಆದರೆ (ಸ್ಥಿತಿ_2)
        *ಸ್ಥಿತಿ_2 = XKB_SUCCESS;
    ಹಿಂತಿರುಗಿ events;
}

ಶೂನ್ಯ
xkb_events_destroy(ರಚನೆ xkb_events *events)
{
    ಆದರೆ (!events)
        ಹಿಂತಿರುಗಿ;
    darray_free(events->ಸರತಿ);
    xkb_context_unref(events->ctx);
    ಬಿಡುಗಡೆ(events);
}

ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_event *
xkb_events_ಮುಂದಿನ(ರಚನೆ xkb_events *events)
{
    ಆದರೆ (events->ಮುಂದಿನ < darray_ಗಾತ್ರ(events->ಸರತಿ)) {
        ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t ಸೂಚ್ಯಂಕ = events->ಮುಂದಿನ++;
        ಹಿಂತಿರುಗಿ &darray_ಅಂಶ(events->ಸರತಿ, ಸೂಚ್ಯಂಕ);
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }
}

ಎಣಿಕೆ xkb_event_type
xkb_event_get_type(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_event *event)
{
    ಹಿಂತಿರುಗಿ event->type;
}

ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
xkb_event_get_keycode(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_event *event,
                      xkb_keycode_t *keycode,
                      ಎಣಿಕೆ xkb_ಕೀಲಿ_direction *direction)
{
    ಆದರೆ (event->type == XKB_EVENT_TYPE_ಕೀಲಿ) {
        *keycode = event->ಕೀಲಿ.keycode;
        *direction = event->ಕೀಲಿ.direction;
        ಹಿಂತಿರುಗಿ XKB_SUCCESS;
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಹಿಂತಿರುಗಿ XKB_ERROR_EVENT_TYPE_MISMATCH;
    }
}

ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
xkb_event_get_components(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_event * ಸೀಮಿತ event,
                         ರಚನೆ xkb_event_components * ಸೀಮಿತ components)
{
    ಆದರೆ (event->type != XKB_EVENT_TYPE_STATE_COMPONENTS)
        ಹಿಂತಿರುಗಿ XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2 = xkb_check_state_abi(components);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, ಸ್ಥಿತಿ_2);
        ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
    }

    components->ಬದಲಾಯಿತು = (uint32_t)event->components.ಬದಲಾಯಿತು;
    components->depressed_mods = event->components.components.ತಳ_mods;
    components->latched_mods = event->components.components.latched_mods;
    components->locked_mods = event->components.components.locked_mods;
    components->mods = event->components.components.mods;
    components->depressed_layout = event->components.components.ತಳ_ಗುಂಪು;
    components->latched_layout = event->components.components.latched_ಗುಂಪು;
    components->locked_layout = event->components.components.locked_ಗುಂಪು;
    components->layout = event->components.components.ಗುಂಪು;
    components->leds = event->components.components.leds;
    components->controls = (uint32_t)event->components.components.controls;

    ಹಿಂತಿರುಗಿ XKB_SUCCESS;
}

ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
xkb_event_get_ಸೂಚಕ_motion(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_event * ಸೀಮಿತ event,
                             ರಚನೆ xkb_event_ಸೂಚಕ_motion * ಸೀಮಿತ motion)
{
    ಆದರೆ (event->type != XKB_EVENT_TYPE_ಸೂಚಕ_MOTION)
        ಹಿಂತಿರುಗಿ XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2 = xkb_check_state_abi(motion);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, ಸ್ಥಿತಿ_2);
        ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
    }

    motion->flags = event->ಸೂಚಕ_motion.flags;
    motion->x = event->ಸೂಚಕ_motion.x;
    motion->y = event->ಸೂಚಕ_motion.y;

    ಹಿಂತಿರುಗಿ XKB_SUCCESS;
}

ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
xkb_event_get_ಸೂಚಕ_button(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_event * ಸೀಮಿತ event,
                             ರಚನೆ xkb_event_ಸೂಚಕ_button * ಸೀಮಿತ button)
{
    ಆದರೆ (event->type != XKB_EVENT_TYPE_ಸೂಚಕ_BUTTON)
        ಹಿಂತಿರುಗಿ XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2 = xkb_check_state_abi(button);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, ಸ್ಥಿತಿ_2);
        ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
    }

    button->button = event->ಸೂಚಕ_button.button;
    button->state = event->ಸೂಚಕ_button.state;
    button->ಎಣಿಕೆ_2 = event->ಸೂಚಕ_button.ಎಣಿಕೆ_2;

    ಹಿಂತಿರುಗಿ XKB_SUCCESS;
}

ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
xkb_event_get_virtual_console(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_event * ಸೀಮಿತ event,
                              int8_t * ಸೀಮಿತ ಸೂಚ್ಯಂಕ_or_ಸ್ಥಾನಾಂತರ,
                              bool * ಸೀಮಿತ is_ಸ್ಥಾನಾಂತರ)
{
    ಆದರೆ (event->type != XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
        ಹಿಂತಿರುಗಿ XKB_ERROR_EVENT_TYPE_MISMATCH;

    *ಸೂಚ್ಯಂಕ_or_ಸ್ಥಾನಾಂತರ = event->virtual_console.ಸೂಚ್ಯಂಕ_or_ಸ್ಥಾನಾಂತರ;
    *is_ಸ್ಥಾನಾಂತರ = event->virtual_console.is_ಸ್ಥಾನಾಂತರ;

    ಹಿಂತಿರುಗಿ XKB_SUCCESS;
}

ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
xkb_state_update_event(ರಚನೆ xkb_state * ಸೀಮಿತ ತಳ_state,
                       ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_event * ಸೀಮಿತ event,
                       ಎಣಿಕೆ xkb_state_component * ಸೀಮಿತ ಬದಲಾಯಿತು)
{
    /* Guard against server-only state */
    ಪ್ರತಿಜ್ಞಾ(ತಳ_state->ಬಹುಲಕ == SERVER_COMPANION ||
           ತಳ_state->ಬಹುಲಕ == LEGACY_MIXED_STATE);
    ಆದರೆ (ತಳ_state->ಬಹುಲಕ != SERVER_COMPANION &&
        ತಳ_state->ಬಹುಲಕ != LEGACY_MIXED_STATE) {
        log_err(ತಳ_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_ಬಹುಲಕ_2,
                "%s: Unexpected state type %d\n", __func__, ತಳ_state->ಬಹುಲಕ);
        ಹಿಂತಿರುಗಿ XKB_ERROR_UNEXPECTED_STATE_ಬಹುಲಕ;
    }

    ರಚನೆ xkb_client_state * ಸ್ಥಿರಾಂಕ state =
        (ರಚನೆ xkb_client_state *)ತಳ_state;

    ಆದರೆ (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
        ಸ್ಥಿರಾಂಕ ರಚನೆ state_components prev_components = state->ತಳ.components;
        state->ತಳ.components = event->components.components;
        /*
         * Recompute the changes instead of using the event value, because we do
         * not know if the event’s queue and the state are synced.
         */
        ಆದರೆ (ಬದಲಾಯಿತು)
            *ಬದಲಾಯಿತು = get_state_component_changes(&prev_components,
                                                   &state->ತಳ.components);
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಆದರೆ (ಬದಲಾಯಿತು)
            *ಬದಲಾಯಿತು = 0;
    }
    ಹಿಂತಿರುಗಿ XKB_SUCCESS;
}

ಸ್ಥಿರ ಶೂನ್ಯ
append_ಸೂಚಕ_button(ರಚನೆ xkb_events * ಸೀಮಿತ events,
                      ರಚನೆ xkb_filter * ಸೀಮಿತ filter,
                      ಎಣಿಕೆ xkb_ಸೂಚಕ_button_state button_state,
                      uint8_t ಎಣಿಕೆ_2)
{
    darray_append(events->ಸರತಿ, (ರಚನೆ xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_ಸೂಚಕ_BUTTON,
        .ಸೂಚಕ_button = {
            .ಗಾತ್ರ_2 = ಗಾತ್ರ(((ರಚನೆ xkb_event*)0)->ಸೂಚಕ_button),
            .button = filter->priv,
            .state = button_state,
            .ಎಣಿಕೆ_2 = ಎಣಿಕೆ_2,
        }
    });
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_ಸೂಚಕ_button_new(ರಚನೆ xkb_server_state *state,
                              ರಚನೆ xkb_events *events,
                              ರಚನೆ xkb_filter *filter)
{
    ಆದರೆ (!events || state->ತಳ.ಬಹುಲಕ != SERVER_STATE ||
        !(state->ತಳ.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ಕೀಲಿಗಳು)) {
        filter->func = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ;
    }

    ರಚನೆ xkb_machine * ಸ್ಥಿರಾಂಕ sm = (ರಚನೆ xkb_machine *)state;

    /* Resolve button */
    ಸ್ಥಿರಾಂಕ xkb_ಸೂಚಕ_button_ಸೂಚ್ಯಂಕ_t button =
        (filter->action.btn.button == XKB_ಸೂಚಕ_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;
    filter->priv = button;

    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_ಸೂಚಕ_button_state button_state =
        XKB_ಸೂಚಕ_BUTTON_PRESSED;
    append_ಸೂಚಕ_button(events, filter, button_state,
                          filter->action.btn.ಎಣಿಕೆ_2);
    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;
}

ಸ್ಥಿರ bool
xkb_filter_ಸೂಚಕ_button_func(ರಚನೆ xkb_server_state *state,
                               ರಚನೆ xkb_events *events,
                               ರಚನೆ xkb_filter *filter,
                               ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                               ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction)
{
    ಆದರೆ (ಕೀಲಿ != filter->ಕೀಲಿ || !events)
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;

    ಆಯ್ಕೆ (direction) {
    ಸ್ಥಿತಿ XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ:
        filter->refcnt++;
        /* fallthrough */
    ಸ್ಥಿತಿ XKB_ಕೀಲಿ_REPEATED:
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಆದರೆ (--filter->refcnt > 0)
            ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    }

    ಆದರೆ (!filter->action.btn.ಎಣಿಕೆ_2)
        append_ಸೂಚಕ_button(events, filter, XKB_ಸೂಚಕ_BUTTON_RELEASED, 0);

    filter->func = ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
}

ರಚನೆ xkb_filter_ಸೂಚಕ_ಬೀಗ_button_priv {
    uint32_t button:31;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
    uint32_t locked:1; // MSVC requires matching types to pack bitfields
#ಇಲ್ಲದಿದ್ದರೆ
    bool locked:1;
#ಆದರೆ_ಅಂತ್ಯ
};

ಎಣಿಕೆ {
    XKB_FILTER_ಸೂಚಕ_ಬೀಗ_BUTTON_PRIV_ಅಗಲ =
        ಗಾತ್ರ(ರಚನೆ xkb_filter_ಸೂಚಕ_ಬೀಗ_button_priv) * CHAR_BIT,
    XKB_FILTER_ಸೂಚಕ_ಬೀಗ_BUTTON_PRIV_MAX =
        (UINT64_C(1) << XKB_FILTER_ಸೂಚಕ_ಬೀಗ_BUTTON_PRIV_ಅಗಲ) - 1
};

static_assert(ಗಾತ್ರ(ರಚನೆ xkb_filter_ಸೂಚಕ_ಬೀಗ_button_priv) ==
              ಗಾತ್ರ(((ರಚನೆ xkb_filter*)0)->priv),
              "");
static_assert((uint32_t)XKB_ಸೂಚಕ_BUTTON_MAX < (uint32_t)XKB_FILTER_ಸೂಚಕ_ಬೀಗ_BUTTON_PRIV_MAX,
              "xkb_filter_pointer_lock_button_priv cannot store button");

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_ಸೂಚಕ_ಬೀಗ_button_new(ರಚನೆ xkb_server_state *state,
                                   ರಚನೆ xkb_events *events,
                                   ರಚನೆ xkb_filter *filter)
{
    ಆದರೆ (!events || state->ತಳ.ಬಹುಲಕ != SERVER_STATE ||
        !(state->ತಳ.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ಕೀಲಿಗಳು)) {
        filter->func = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ;
    }

    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;

    ರಚನೆ xkb_machine * ಸ್ಥಿರಾಂಕ sm = (ರಚನೆ xkb_machine *)state;

    /* Resolve button */
    ಸ್ಥಿರಾಂಕ xkb_ಸೂಚಕ_button_ಸೂಚ್ಯಂಕ_t button =
        (filter->action.btn.button == XKB_ಸೂಚಕ_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;

    static_assert(XKB_ಸೂಚಕ_BUTTON_ಮುಖವಾಡ_ಅಗಲ <= 8,
                  "invalid UINTx_C() macro");
    static_assert(XKB_ಸೂಚಕ_BUTTON_MAX <= XKB_ಸೂಚಕ_BUTTON_ಮುಖವಾಡ_ಅಗಲ,
                  "invalid left shift");
    ಸ್ಥಿರಾಂಕ xkb_ಸೂಚಕ_button_ಮುಖವಾಡ_t ಮುಖವಾಡ = (UINT8_C(1) << button);

    ಸ್ಥಿರಾಂಕ bool locked = (sm->mouse.locked_buttons & ಮುಖವಾಡ);

    ಆದರೆ (!locked && !(filter->action.btn.flags & ACTION_ಬೀಗ_NO_ಬೀಗ)) {
        sm->mouse.locked_buttons |= ಮುಖವಾಡ;
        filter->priv = button;
        append_ಸೂಚಕ_button(events, filter, XKB_ಸೂಚಕ_BUTTON_PRESSED, 0);
    }

    ರಚನೆ xkb_filter_ಸೂಚಕ_ಬೀಗ_button_priv *priv =
        (ರಚನೆ xkb_filter_ಸೂಚಕ_ಬೀಗ_button_priv*)&filter->priv;
    priv->button = button,
    priv->locked = locked;
}

ಸ್ಥಿರ bool
xkb_filter_ಸೂಚಕ_ಬೀಗ_button_func(ರಚನೆ xkb_server_state *state,
                                    ರಚನೆ xkb_events *events,
                                    ರಚನೆ xkb_filter *filter,
                                    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                                    ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction)
{
    ಆದರೆ (ಕೀಲಿ != filter->ಕೀಲಿ || !events || state->ತಳ.ಬಹುಲಕ != SERVER_STATE)
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;

    ಆಯ್ಕೆ (direction) {
    ಸ್ಥಿತಿ XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ:
        filter->refcnt++;
        /* fallthrough */
    ಸ್ಥಿತಿ XKB_ಕೀಲಿ_REPEATED:
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಆದರೆ (--filter->refcnt > 0)
            ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    }

    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_filter_ಸೂಚಕ_ಬೀಗ_button_priv *priv =
        (ರಚನೆ xkb_filter_ಸೂಚಕ_ಬೀಗ_button_priv*)&filter->priv;

    ಆದರೆ (priv->locked && !(filter->action.btn.flags & ACTION_ಬೀಗ_NO_UNLOCK)) {
        ರಚನೆ xkb_machine * ಸ್ಥಿರಾಂಕ sm = (ರಚನೆ xkb_machine *)state;

        static_assert(XKB_ಸೂಚಕ_BUTTON_ಮುಖವಾಡ_ಅಗಲ <= 8,
                    "invalid UINTx_C() macro");
        static_assert(XKB_ಸೂಚಕ_BUTTON_MAX <= XKB_ಸೂಚಕ_BUTTON_ಮುಖವಾಡ_ಅಗಲ,
                    "invalid left shift");
        ಸ್ಥಿರಾಂಕ xkb_ಸೂಚಕ_button_ಮುಖವಾಡ_t ಮುಖವಾಡ = (UINT8_C(1) << priv->button);

        sm->mouse.locked_buttons &= ~ಮುಖವಾಡ;
        filter->priv = priv->button;
        append_ಸೂಚಕ_button(events, filter, XKB_ಸೂಚಕ_BUTTON_RELEASED, 0);
    }

    filter->func = ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_ಸೂಚಕ_ಗಣ_default_button_new(ರಚನೆ xkb_server_state *state,
                                          ರಚನೆ xkb_events *events,
                                          ರಚನೆ xkb_filter *filter)
{
    ಆದರೆ (!events || state->ತಳ.ಬಹುಲಕ != SERVER_STATE ||
        !(state->ತಳ.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ಕೀಲಿಗಳು)) {
        filter->func = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ;
    }

    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;

    ರಚನೆ xkb_machine * ಸ್ಥಿರಾಂಕ sm = (ರಚನೆ xkb_machine *)state;

    static_assert(2 * XKB_ಸೂಚಕ_BUTTON_MAX <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ, "lossy conversion");
    ಪೂರ್ಣಾಂಕ button = (filter->action.dflt.flags & ACTION_ABSOLUTE_SWITCH)
        ? filter->action.dflt.ಮೌಲ್ಯ
        : sm->mouse.default_button + filter->action.dflt.ಮೌಲ್ಯ;

    /* Wrap into range */
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ rem = button % XKB_ಸೂಚಕ_BUTTON_MAX;
    sm->mouse.default_button = (rem >= 0)
        ? (xkb_ಸೂಚಕ_button_ಸೂಚ್ಯಂಕ_t)rem
        : (xkb_ಸೂಚಕ_button_ಸೂಚ್ಯಂಕ_t)(rem + XKB_ಸೂಚಕ_BUTTON_MAX);
}

ಸ್ಥಿರ bool
xkb_filter_ಸೂಚಕ_ಗಣ_default_button_func(ರಚನೆ xkb_server_state *state,
                                           ರಚನೆ xkb_events *events,
                                           ರಚನೆ xkb_filter *filter,
                                           ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                                           ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction)
{
    ಆದರೆ (ಕೀಲಿ != filter->ಕೀಲಿ || !events)
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;

    ಆಯ್ಕೆ (direction) {
    ಸ್ಥಿತಿ XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ:
        filter->refcnt++;
        /* fallthrough */
    ಸ್ಥಿತಿ XKB_ಕೀಲಿ_REPEATED:
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಆದರೆ (--filter->refcnt > 0)
            ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    }

    filter->func = ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_terminate_server_new(ರಚನೆ xkb_server_state *state,
                                 ರಚನೆ xkb_events *events,
                                 ರಚನೆ xkb_filter *filter)
{
    ಆದರೆ (!events || state->ತಳ.ಬಹುಲಕ != SERVER_STATE ||
        !(((ರಚನೆ xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ;
    }

    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;

    darray_append(events->ಸರತಿ, (ರಚನೆ xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    });
}

ಸ್ಥಿರ bool
xkb_filter_terminate_server_func(ರಚನೆ xkb_server_state *state,
                                 ರಚನೆ xkb_events *events,
                                 ರಚನೆ xkb_filter *filter,
                                 ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                                 ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction)
{
    ಆದರೆ (ಕೀಲಿ != filter->ಕೀಲಿ || !events)
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;

    ಆಯ್ಕೆ (direction) {
    ಸ್ಥಿತಿ XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ:
        filter->refcnt++;
        /* fallthrough */
    ಸ್ಥಿತಿ XKB_ಕೀಲಿ_REPEATED:
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಆದರೆ (--filter->refcnt > 0)
            ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    }

    filter->func = ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_filter_switch_virtual_console_new(ರಚನೆ xkb_server_state *state,
                                      ರಚನೆ xkb_events *events,
                                      ರಚನೆ xkb_filter *filter)
{
    ಆದರೆ (!events || state->ತಳ.ಬಹುಲಕ != SERVER_STATE ||
        !(((ರಚನೆ xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ;
    }

    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;

    darray_append(events->ಸರತಿ, (ರಚನೆ xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
        .virtual_console = {
            .ಸೂಚ್ಯಂಕ_or_ಸ್ಥಾನಾಂತರ = filter->action.screen.screen,
            .is_ಸ್ಥಾನಾಂತರ = !(filter->action.screen.flags & ACTION_ABSOLUTE_SWITCH)
        }
    });
}

ಸ್ಥಿರ bool
xkb_filter_switch_virtual_console_func(ರಚನೆ xkb_server_state *state,
                                       ರಚನೆ xkb_events *events,
                                       ರಚನೆ xkb_filter *filter,
                                       ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ,
                                       ಎಣಿಕೆ xkb_ಕೀಲಿ_direction direction)
{
    ಆದರೆ (ಕೀಲಿ != filter->ಕೀಲಿ || !events)
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ಕೀಲಿ_EVENT;

    ಆಯ್ಕೆ (direction) {
    ಸ್ಥಿತಿ XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ:
        filter->refcnt++;
        /* fallthrough */
    ಸ್ಥಿತಿ XKB_ಕೀಲಿ_REPEATED:
        ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಆದರೆ (--filter->refcnt > 0)
            ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
    }

    filter->func = ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ XKB_FILTER_CONSUME;
}
