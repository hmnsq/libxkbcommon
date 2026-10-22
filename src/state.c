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

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>
#అన్తర్భావ <limits.h>
#అన్తర్భావ <stdint.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "xkbcommon/xkbcommon-status.h"
#అన్తర్భావ "xkbcommon/xkbcommon-features.h"
#అన్తర్భావ "abi-check.h"
#అన్తర్భావ "darray.h"
#అన్తర్భావ "features/enums.h"
#అన్తర్భావ "keymap.h"
#అన్తర్భావ "keysym.h"
#అన్తర్భావ "messages-codes.h"
#అన్తర్భావ "state-priv.h"
#అన్తర్భావ "utf8.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "util-mem.h"
#అన్తర్భావ "util-numbers.h"

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
నిర్మాణం xkb_events {
    నిర్మాణం xkb_context *ctx;
    darray(నిర్మాణం xkb_event) వరుస;
    /**
     * Read cursor for `xkb_events_next()`.
     *
     * Reset to 0 on each `process_*` call.
     */
    darray_కొలత_t తదుపరి;
};

నిర్మాణం xkb_server_state;

నిర్మాణం xkb_filter {
    సంఘం xkb_action action;
    స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం;
    bool (*func)(నిర్మాణం xkb_server_state *state,
                 నిర్మాణం xkb_events *events,
                 నిర్మాణం xkb_filter *filter,
                 స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                 గణన xkb_కీలకం_direction direction);
    uint32_t priv;
    పూర్ణాంకం refcnt;
};

గణన xkb_state_బహుళకం_అంతర్గత {
    /** `xkb_state`: Client-only state (server API raises errors) */
    CLIENT_STATE = XKB_STATE_బహుళకం_CLIENT,
    /** `xkb_state`: Server query companion */
    SERVER_COMPANION = XKB_STATE_బహుళకం_SERVER_QUERY,
    /** `xkb_state`: Legacy unsafe client + server state */
    LEGACY_MIXED_STATE = 2,
    /** `xkb_state`: Legacy server-only state (client-only API raises errors) */
    LEGACY_SERVER_STATE = 3,
    /** `xkb_machine`: Server-only state */
    SERVER_STATE = 4,
    XKB_STATE_TYPE_అంకె_ENTRIES = 5
};

static_assert(CLIENT_STATE == 0 &&
              CLIENT_STATE == SERVER_COMPANION - 1 &&
              SERVER_COMPANION == LEGACY_MIXED_STATE - 1 &&
              LEGACY_MIXED_STATE == LEGACY_SERVER_STATE - 1 &&
              LEGACY_SERVER_STATE == SERVER_STATE - 1 &&
              SERVER_STATE == XKB_STATE_TYPE_అంకె_ENTRIES - 1 &&
              XKB_STATE_TYPE_అంకె_ENTRIES == 5,
              "Order is used to guard client/server -only APIs: do not change");

గణన { XKB_LAYOUT_సూచీ_T_MIN_వెడల్పు = 6 };
static_assert(XKB_MAX_GROUPS < (1 << XKB_LAYOUT_సూచీ_T_MIN_వెడల్పు),
              "Cannot encode out_of_range_group::redirect_group");

నిర్మాణం బహిర్గతం_యొక్క_range_సమూహం {
    గణన xkb_layout_బహిర్గతం_యొక్క_range_policy policy :
        (పరిమాణం(గణన xkb_layout_బహిర్గతం_యొక్క_range_policy) * CHAR_BIT -
         XKB_LAYOUT_సూచీ_T_MIN_వెడల్పు);
    xkb_layout_సూచీ_t redirect_సమూహం : XKB_LAYOUT_సూచీ_T_MIN_వెడల్పు;
};

గణన { XKB_STATE_బహుళకం_అంతర్గత_MIN_వెడల్పు = 4 /* 3 bits + sign */ };
static_assert(XKB_STATE_TYPE_అంకె_ENTRIES <
              (1 << XKB_STATE_బహుళకం_అంతర్గత_MIN_వెడల్పు),
              "Cannot encode xkb_state::mode");

/**
 * Common state
 *
 * `mode` indicates whether the object is casted from a `xkb_server_state` or a
 * `xkb_client_state` struct.
 */
నిర్మాణం xkb_state {
    నిర్మాణం xkb_keymap *keymap;
    /*
     * Before updating the state, we keep a copy of just this struct. This
     * allows us to report which components of the state have changed.
     */
    నిర్మాణం state_components components;
    నిర్మాణం బహిర్గతం_యొక్క_range_సమూహం బహిర్గతం_యొక్క_range_సమూహం;

    గణన xkb_state_బహుళకం_అంతర్గత బహుళకం : XKB_STATE_బహుళకం_అంతర్గత_MIN_వెడల్పు;
    పూర్ణాంకం refcnt : (పరిమాణం(పూర్ణాంకం) * CHAR_BIT - XKB_STATE_బహుళకం_అంతర్గత_MIN_వెడల్పు);
};

/**
 * Minimal client state
 *
 * Wrapper for strong typing
 */
నిర్మాణం xkb_client_state {
    /** Inherits from `xkb_state` */
    నిర్మాణం xkb_state పునాది;
};

గణన state_flags {
    STATE_NO_FLAGS = 0,
    STATE_REQUIRE_కీలకం_EVENT = (1 << 0),
    STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_కీలకం = (1 << 1),
    STATE_UNLOCK_MODS_ON_PRESS = (1 << 2),
};

/**
 * Legacy server state, used for `LEGACY_MIXED_STATE` and `LEGACY_SERVER_STATE`
 * modes.
 *
 * Casted as `xkb_state` in the public API. See `xkb_state::mode`.
 */
నిర్మాణం xkb_server_state {
    /** Inherits from `xkb_state` */
    నిర్మాణం xkb_state పునాది;

    /* Server-specific state data */

    darray(నిర్మాణం xkb_filter) filters;

    /**
     * Mix of constant configuration flags and
     * variable state-processing flags.
     */
    గణన state_flags flags;

    గణన xkb_a11y_flags a11y_flags;

    /*
     * At each event, we accumulate all the needed modifications to the base
     * modifiers, and apply them at the end. These keep track of this state.
     */
    xkb_mod_ముసుగు_t సమితి_mods;
    xkb_mod_ముసుగు_t clear_mods;

    /*
     * We mustn't clear a base modifier if there's another depressed key
     * which affects it, e.g. given this sequence
     * < Left Shift down, Right Shift down, Left Shift Up >
     * the modifier should still be set. This keeps the count.
     */
    int16_t mod_కీలకం_లెక్క[XKB_MAX_MODS];
};

స్థిర స్థిరరాశి నిర్మాణం xkb_కీలకం_type_నమోదు *
get_నమోదు_for_mods(స్థిరరాశి నిర్మాణం xkb_కీలకం_type *type, xkb_mod_ముసుగు_t mods)
{
    కొరకు (darray_కొలత_t i = 0; i < type->అంకె_entries; i++)
        ఒకవేళ (నమోదు_is_చురుకైన(&type->entries[i]) &&
            type->entries[i].mods.ముసుగు == mods)
            తిరిగి &type->entries[i];
    తిరిగి ఖాళీ;
}

స్థిర స్థిరరాశి నిర్మాణం xkb_కీలకం_type_నమోదు *
get_నమోదు_for_కీలకం_state(నిర్మాణం xkb_state *state, స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                        xkb_layout_సూచీ_t సమూహం)
{
    స్థిరరాశి నిర్మాణం xkb_కీలకం_type* స్థిరరాశి type = కీలకం->groups[సమూహం].type;
    xkb_mod_ముసుగు_t చురుకైన_mods = state->components.mods & type->mods.ముసుగు;
    తిరిగి get_నమోదు_for_mods(type, చురుకైన_mods);
}

స్థిర అంతఃవరుస xkb_level_సూచీ_t
state_కీలకం_get_level(నిర్మాణం xkb_state *state, స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                         xkb_layout_సూచీ_t layout)
{
    ఒకవేళ (layout >= కీలకం->అంకె_groups)
        తిరిగి XKB_LEVEL_INVALID;

    /* If we don't find an explicit match the default is 0. */
    స్థిరరాశి నిర్మాణం xkb_కీలకం_type_నమోదు* స్థిరరాశి నమోదు_2 =
        get_నమోదు_for_కీలకం_state(state, కీలకం, layout);

    తిరిగి (నమోదు_2) ? నమోదు_2->level : 0;
}

/**
 * Returns the level to use for the given key and state, or
 * XKB_LEVEL_INVALID.
 */
xkb_level_సూచీ_t
xkb_state_కీలకం_get_level(నిర్మాణం xkb_state *state, xkb_keycode_t kc,
                        xkb_layout_సూచీ_t layout)
{
    స్థిరరాశి నిర్మాణం xkb_కీలకం* స్థిరరాశి కీలకం = Xkb_కీలకం(state->keymap, kc);

    తిరిగి (కీలకం)
        ? state_కీలకం_get_level(state, కీలకం, layout)
        : XKB_LEVEL_INVALID;
}

స్థిర అంతఃవరుస xkb_layout_సూచీ_t
state_కీలకం_get_layout(నిర్మాణం xkb_state *state, స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం)
{
    తిరిగి xkb_keymap_కీలకం_effective_layout(కీలకం, state->components.సమూహం);
}

/**
 * Returns the layout to use for the given key and state, taking
 * wrapping/clamping/etc into account, or XKB_LAYOUT_INVALID.
 */
xkb_layout_సూచీ_t
xkb_state_కీలకం_get_layout(నిర్మాణం xkb_state *state, xkb_keycode_t kc)
{
    స్థిరరాశి నిర్మాణం xkb_కీలకం* స్థిరరాశి కీలకం = Xkb_కీలకం(state->keymap, kc);

    ఒకవేళ (!కీలకం)
        తిరిగి XKB_LAYOUT_INVALID;

    తిరిగి state_కీలకం_get_layout(state, కీలకం);
}

/* Empty action used for empty levels */
స్థిర స్థిరరాశి సంఘం xkb_action dummy_action = { .type = ACTION_TYPE_NONE };

స్థిర xkb_action_లెక్క_t
xkb_కీలకం_get_actions(నిర్మాణం xkb_state *state, స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                    స్థిరరాశి సంఘం xkb_action **actions)
{
    స్థిరరాశి xkb_layout_సూచీ_t layout = state_కీలకం_get_layout(state, కీలకం);
    స్థిరరాశి xkb_level_సూచీ_t level = state_కీలకం_get_level(state, కీలకం, layout);
    ఒకవేళ (level == XKB_LEVEL_INVALID)
        వెళ్ళు err;

    స్థిరరాశి xkb_action_లెక్క_t లెక్క =
        xkb_keymap_కీలకం_get_actions_by_level(state->keymap, కీలకం,
                                            layout, level, actions);
    ఒకవేళ (!లెక్క)
        వెళ్ళు err;

    తిరిగి లెక్క;

err:
    /* Use a dummy action if no corresponding level was found or if it is empty.
     * This is required e.g. to handle latches properly. */
    *actions = &dummy_action;
    తిరిగి 1;
}

స్థిర నిర్మాణం xkb_filter *
xkb_filter_new(నిర్మాణం xkb_server_state *state)
{
    నిర్మాణం xkb_filter *filter = ఖాళీ, *iter;

    darray_foreach(iter, state->filters) {
        ఒకవేళ (iter->func)
            కొనసాగు;
        /* Use available slot */
        filter = iter;
        ఆపు;
    }

    ఒకవేళ (!filter) {
        /* No available slot: resize the filters array */
        darray_resize0(state->filters, darray_కొలత(state->filters) + 1);
        filter = &darray_అంశం(state->filters, darray_కొలత(state->filters) -1);
    }

    filter->refcnt = 1;
    తిరిగి filter;
}

/***====================================================================***/

గణన xkb_filter_ఫలితం {
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
#నిర్వచించు apply_సమూహం_delta(filter_, state_, component_)                   \
    ఒకవేళ ((filter_)->action.సమూహం.flags & ACTION_ABSOLUTE_SWITCH)          \
        (state_)->components.component_ = (filter_)->action.సమూహం.సమూహం; \
    లేకపోతే                                                                 \
        (state_)->components.component_ += (filter_)->action.సమూహం.సమూహం

స్థిర శూన్య
xkb_filter_సమూహం_సమితి_new(నిర్మాణం xkb_server_state *state,
                         నిర్మాణం xkb_events *events,
                         నిర్మాణం xkb_filter *filter)
{
    static_assert(పరిమాణం(state->పునాది.components.పునాది_సమూహం) ==
                  పరిమాణం(filter->priv),
                  "Max groups don't fit");
    filter->priv = (uint32_t)state->పునాది.components.పునాది_సమూహం;
    apply_సమూహం_delta(filter, &state->పునాది, పునాది_సమూహం);
}

స్థిర bool
xkb_filter_సమూహం_సమితి_func(నిర్మాణం xkb_server_state *state,
                          నిర్మాణం xkb_events *events,
                          నిర్మాణం xkb_filter *filter,
                          స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                          గణన xkb_కీలకం_direction direction)
{
    ఒకవేళ (కీలకం != filter->కీలకం) {
        filter->action.సమూహం.flags &= ~ACTION_తాళం_CLEAR;
        తిరిగి XKB_FILTER_CONTINUE;
    }

    ఎంపిక (direction) {
    స్థితి XKB_కీలకం_కిందికి:
        filter->refcnt++;
        /* fallthrough */
    స్థితి XKB_కీలకం_REPEATED:
        తిరిగి XKB_FILTER_CONSUME;
    అప్రమేయ:
        ఒకవేళ (--filter->refcnt > 0)
            తిరిగి XKB_FILTER_CONSUME;
    }

    static_assert(పరిమాణం(state->పునాది.components.పునాది_సమూహం) == పరిమాణం(filter->priv),
                  "Max groups don't fit");
    state->పునాది.components.పునాది_సమూహం = (int32_t) filter->priv;

    ఒకవేళ (filter->action.సమూహం.flags & ACTION_తాళం_CLEAR)
        state->పునాది.components.locked_సమూహం = 0;

    filter->func = ఖాళీ;
    తిరిగి XKB_FILTER_CONTINUE;
}

స్థిర గణన xkb_state_component
get_state_component_changes(స్థిరరాశి నిర్మాణం state_components *a,
                            స్థిరరాశి నిర్మాణం state_components *b)
{
    గణన xkb_state_component ముసుగు = 0;

    ఒకవేళ (a->సమూహం != b->సమూహం)
        ముసుగు |= XKB_STATE_LAYOUT_EFFECTIVE;
    ఒకవేళ (a->పునాది_సమూహం != b->పునాది_సమూహం)
        ముసుగు |= XKB_STATE_LAYOUT_DEPRESSED;
    ఒకవేళ (a->latched_సమూహం != b->latched_సమూహం)
        ముసుగు |= XKB_STATE_LAYOUT_LATCHED;
    ఒకవేళ (a->locked_సమూహం != b->locked_సమూహం)
        ముసుగు |= XKB_STATE_LAYOUT_LOCKED;
    ఒకవేళ (a->mods != b->mods)
        ముసుగు |= XKB_STATE_MODS_EFFECTIVE;
    ఒకవేళ (a->పునాది_mods != b->పునాది_mods)
        ముసుగు |= XKB_STATE_MODS_DEPRESSED;
    ఒకవేళ (a->latched_mods != b->latched_mods)
        ముసుగు |= XKB_STATE_MODS_LATCHED;
    ఒకవేళ (a->locked_mods != b->locked_mods)
        ముసుగు |= XKB_STATE_MODS_LOCKED;
    ఒకవేళ (a->leds != b->leds)
        ముసుగు |= XKB_STATE_LEDS;
    ఒకవేళ (a->controls != b->controls)
        ముసుగు |= XKB_STATE_CONTROLS_EFFECTIVE;

    తిరిగి ముసుగు;
}

స్థిర శూన్య
xkb_filter_సమూహం_తాళం_new(నిర్మాణం xkb_server_state *state,
                          నిర్మాణం xkb_events *events,
                          నిర్మాణం xkb_filter *filter)
{
    ఒకవేళ (filter->action.సమూహం.flags & ACTION_తాళం_ON_RELEASE) {
        /*
         * Lock on key release: do nothing on key press.
         *
         * This is a keymap format v2 extension.
         */
        తిరిగి;
    } లేకపోతే {
        /* Lock on key press */
        apply_సమూహం_delta(filter, &state->పునాది, locked_సమూహం);
    }
}

స్థిర bool
xkb_filter_సమూహం_తాళం_func(నిర్మాణం xkb_server_state *state,
                           నిర్మాణం xkb_events *events,
                           నిర్మాణం xkb_filter *filter,
                           స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                           గణన xkb_కీలకం_direction direction)
{
    ఒకవేళ (కీలకం != filter->కీలకం) {
        ఒకవేళ ((filter->action.సమూహం.flags & ACTION_తాళం_ON_RELEASE) &&
            direction == XKB_కీలకం_కిందికి) {
            /*
             * Another key has been pressed after the locking key:
             * cancel group lock on release.
             *
             * This is a keymap v2 extension.
             */
            filter->action.సమూహం.flags &= ~ACTION_తాళం_ON_RELEASE;
        }
        తిరిగి XKB_FILTER_CONTINUE;
    }

    ఎంపిక (direction) {
    స్థితి XKB_కీలకం_కిందికి:
        filter->refcnt++;
        /* fallthrough */
    స్థితి XKB_కీలకం_REPEATED:
        తిరిగి XKB_FILTER_CONSUME;
    అప్రమేయ:
        ఒకవేళ (--filter->refcnt > 0)
            తిరిగి XKB_FILTER_CONSUME;
    }

    ఒకవేళ (filter->action.సమూహం.flags & ACTION_తాళం_ON_RELEASE) {
        /*
         * Lock on key release
         *
         * This is a keymap v2 extension.
         */
        apply_సమూహం_delta(filter, &state->పునాది, locked_సమూహం);
    } లేకపోతే {
        /* Lock on key press: do nothing on key release. */
    }
    filter->func = ఖాళీ;
    తిరిగి XKB_FILTER_CONTINUE;
}

స్థిర bool
xkb_action_breaks_latch(స్థిరరాశి సంఘం xkb_action *action,
                        గణన xkb_action_controls controls,
                        గణన xkb_అంతర్గత_action_flags flag,
                        xkb_mod_ముసుగు_t ముసుగు)
{
    ఎంపిక (action->type) {
    స్థితి ACTION_TYPE_NONE:
    స్థితి ACTION_TYPE_VOID:
    స్థితి ACTION_TYPE_PTR_BUTTON:
    స్థితి ACTION_TYPE_PTR_తాళం:
    స్థితి ACTION_TYPE_CTRL_సమితి:
    స్థితి ACTION_TYPE_CTRL_తాళం:
    స్థితి ACTION_TYPE_SWITCH_VT:
    స్థితి ACTION_TYPE_TERMINATE:
    స్థితి ACTION_TYPE_REDIRECT_కీలకం:
        తిరిగి true;
    స్థితి ACTION_TYPE_PTR_MOVE:
    స్థితి ACTION_TYPE_PTR_DEFAULT:
        తిరిగి !(controls & CONTROL_MOUSE_కీలకాలు);
    స్థితి ACTION_TYPE_అంతర్గత:
        తిరిగి (action->అంతర్గత.flags & flag) &&
               ((action->అంతర్గత.clear_latched_mods & ముసుగు) == ముసుగు);
    అప్రమేయ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_అంతర్గత == 20 &&
                      ACTION_TYPE_అంతర్గత + 1 == ACTION_TYPE_అంకె_ENTRIES,
                      "Missing action type");
        తిరిగి false;
    }
}

గణన xkb_కీలకం_latch_state {
    NO_LATCH = 0,
    LATCH_కీలకం_కిందికి,
    LATCH_PENDING,
    కీలకం_LATCH_STATE_అంకె_ENTRIES
};

#నిర్వచించు MAX_XKB_కీలకం_LATCH_STATE_LOG2 2
#ఒకవేళ (కీలకం_LATCH_STATE_అంకె_ENTRIES > (1 << MAX_XKB_కీలకం_LATCH_STATE_LOG2)) || \
    (-XKB_MAX_GROUPS) < (INT32_MIN >> MAX_XKB_కీలకం_LATCH_STATE_LOG2) || \
      XKB_MAX_GROUPS > (INT32_MAX >> MAX_XKB_కీలకం_LATCH_STATE_LOG2)
#దోషం "Cannot represent priv field of the group latch filter"
#అయితే_ముగింపు

/* Hold the latch state *and* the group delta */
సంఘం సమూహం_latch_priv {
    uint32_t priv;
    నిర్మాణం {
        /* The type is really: enum xkb_key_latch_state, but it is problematic
         * on Windows, because it is interpreted as signed and leads to wrong
         * negative values. */
        గుర్తులేని పూర్ణాంకం latch:MAX_XKB_కీలకం_LATCH_STATE_LOG2;
        int32_t సమూహం_delta:(32 - MAX_XKB_కీలకం_LATCH_STATE_LOG2);
    };
};

స్థిర శూన్య
xkb_filter_సమూహం_latch_new(నిర్మాణం xkb_server_state *state,
                           నిర్మాణం xkb_events *events,
                           నిర్మాణం xkb_filter *filter)
{
    స్థిరరాశి సంఘం సమూహం_latch_priv priv = {
        .latch = LATCH_కీలకం_కిందికి,
        .సమూహం_delta = (filter->action.సమూహం.flags & ACTION_ABSOLUTE_SWITCH)
            ? filter->action.సమూహం.సమూహం - state->పునాది.components.పునాది_సమూహం
            : filter->action.సమూహం.సమూహం
    };
    filter->priv = priv.priv;
    /* Like group set */
    apply_సమూహం_delta(filter, &state->పునాది, పునాది_సమూహం);
}

స్థిర bool
xkb_filter_సమూహం_latch_func(నిర్మాణం xkb_server_state *state,
                            నిర్మాణం xkb_events *events,
                            నిర్మాణం xkb_filter *filter,
                            స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                            గణన xkb_కీలకం_direction direction)
{
    సంఘం సమూహం_latch_priv priv = {.priv = filter->priv};
    గణన xkb_కీలకం_latch_state latch = priv.latch;

    ఒకవేళ (direction == XKB_కీలకం_కిందికి) {
        స్థిరరాశి సంఘం xkb_action *actions = ఖాళీ;
        స్థిరరాశి xkb_action_లెక్క_t లెక్క = xkb_కీలకం_get_actions(&state->పునాది, కీలకం,
                                                             &actions);

        ఒకవేళ (latch == LATCH_కీలకం_కిందికి) {
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
            ఒకవేళ (state->a11y_flags & XKB_A11_Y_LATCH_ఏకకాల_కీలకాలు) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                కొరకు (xkb_action_లెక్క_t k = 0; k < లెక్క; k++) {
                    ఒకవేళ (xkb_action_breaks_latch(&(actions[k]),
                                                state->పునాది.components.controls,
                                                అంతర్గత_BREAKS_సమూహం_LATCH,
                                                0))
                    {
                        latch = NO_LATCH;
                        ఆపు;
                    }
                }
            } లేకపోతే {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        లేకపోతే ఒకవేళ (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is
             * pressed, or promote it to a lock if it's the same group delta &
             * flags and latchToLock option is enabled. */
            స్థిరరాశి bool sticky_కీలకాలు = state->పునాది.components.controls
                                   & CONTROL_STICKY_కీలకాలు;
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
            కొరకు (xkb_action_లెక్క_t k = 0; k < లెక్క; k++) {
                ఒకవేళ (((actions[k].type == ACTION_TYPE_సమూహం_LATCH) ||
                     (actions[k].type == ACTION_TYPE_సమూహం_సమితి && sticky_కీలకాలు)) &&
                     actions[k].సమూహం.సమూహం == filter->action.సమూహం.సమూహం &&
                     (actions[k].సమూహం.flags & ACTION_ABSOLUTE_SWITCH) ==
                     (filter->action.సమూహం.flags & ACTION_ABSOLUTE_SWITCH)) {
                    ఒకవేళ (filter->action.సమూహం.flags & ACTION_LATCH_గమ్యం_తాళం &&
                        filter->action.సమూహం.సమూహం != 0)
                    {
                        /* Promote to lock */
                        filter->action.type = ACTION_TYPE_సమూహం_తాళం;
                        filter->func = xkb_filter_సమూహం_తాళం_func;
                        xkb_filter_సమూహం_తాళం_new(state, events, filter);
                        state->పునాది.components.latched_సమూహం -= priv.సమూహం_delta;
                        filter->కీలకం = కీలకం;
                        /* XXX beep beep! */
                        తిరిగి XKB_FILTER_CONSUME;
                    }
                    /* Do nothing if `latchToLock` option is not activated; if
                     * the latch is not broken by the following actions and the
                     * key is not consumed, then another latch filter will be
                     * created.
                     */
                    కొనసాగు;
                }
                లేకపోతే ఒకవేళ (xkb_action_breaks_latch(&(actions[k]),
                                                 state->పునాది.components.controls,
                                                 అంతర్గత_BREAKS_సమూహం_LATCH,
                                                 0))
                {
                    /* Breaks the latch */
                    state->పునాది.components.latched_సమూహం -= priv.సమూహం_delta;
                    filter->func = ఖాళీ;
                    తిరిగి XKB_FILTER_CONTINUE;
                }
            }
        }
        లేకపోతే {
            /* Ignore press in NO_LATCH state */
            ప్రతిజ్ఞా(latch == NO_LATCH);
        }
    }
    లేకపోతే ఒకవేళ (కీలకం != filter->కీలకం) {
        /* Ignore repeat/release of other keys */
    }
    లేకపోతే ఒకవేళ (direction == XKB_కీలకం_REPEATED) {
        తిరిగి XKB_FILTER_CONSUME; // TODO: check
    }
    లేకపోతే {
        /* Our key got released.  If we've set it to clear locks, and we
         * currently have a group locked, then release it and
         * don't actually latch.  Else we've actually hit the latching
         * stage, so set PENDING and move our group from base to
         * latched. */
        ఒకవేళ ((filter->action.సమూహం.flags & ACTION_తాళం_CLEAR) &&
             state->పునాది.components.locked_సమూహం) {
            ఒకవేళ (latch == LATCH_PENDING)
                state->పునాది.components.latched_సమూహం -= priv.సమూహం_delta;
            లేకపోతే
                state->పునాది.components.పునాది_సమూహం -= priv.సమూహం_delta;
            state->పునాది.components.locked_సమూహం = 0;
            filter->func = ఖాళీ;
        }
        /* Broken latch */
        లేకపోతే ఒకవేళ (latch == NO_LATCH) {
            state->పునాది.components.పునాది_సమూహం -= priv.సమూహం_delta;
            filter->func = ఖాళీ;
        }
        /* We may already have reached the latch state if pressing the
         * key multiple times without latch-to-lock enabled. */
        లేకపోతే ఒకవేళ (latch == LATCH_కీలకం_కిందికి) {
            latch = LATCH_PENDING;
            /* Switch from set to latch */
            state->పునాది.components.పునాది_సమూహం -= priv.సమూహం_delta;
            state->పునాది.components.latched_సమూహం += priv.సమూహం_delta;
            /* XXX beep beep! */
        }
    }

    priv.latch = latch;
    filter->priv = priv.priv;

    తిరిగి XKB_FILTER_CONTINUE;
}

స్థిర శూన్య
xkb_filter_mod_సమితి_new(నిర్మాణం xkb_server_state *state,
                       నిర్మాణం xkb_events *events,
                       నిర్మాణం xkb_filter *filter)
{
    స్థిరరాశి గణన xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_తాళం_CLEAR;
    ఒకవేళ ((filter->action.mods.flags & unlock) == unlock) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension.
         */
        filter->priv = filter->action.mods.mods.ముసుగు
                     & ~state->పునాది.components.locked_mods;
        state->పునాది.components.locked_mods &= ~filter->action.mods.mods.ముసుగు;
    } లేకపోతే {
        filter->priv = filter->action.mods.mods.ముసుగు;
    }

    state->సమితి_mods |= (xkb_mod_ముసుగు_t) filter->priv;
}

స్థిర bool
xkb_filter_mod_సమితి_func(నిర్మాణం xkb_server_state *state,
                        నిర్మాణం xkb_events *events,
                        నిర్మాణం xkb_filter *filter,
                        స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                        గణన xkb_కీలకం_direction direction)
{

    ఒకవేళ (కీలకం != filter->కీలకం) {
        filter->action.mods.flags &= ~ACTION_తాళం_CLEAR;
        తిరిగి XKB_FILTER_CONTINUE;
    }

    ఎంపిక (direction) {
    స్థితి XKB_కీలకం_కిందికి:
        filter->refcnt++;
        /* fallthrough */
    స్థితి XKB_కీలకం_REPEATED:
        తిరిగి XKB_FILTER_CONSUME;
    అప్రమేయ:
        ఒకవేళ (--filter->refcnt > 0)
            తిరిగి XKB_FILTER_CONSUME;
    }

    state->clear_mods |= (xkb_mod_ముసుగు_t) filter->priv;
    స్థిరరాశి గణన xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_తాళం_CLEAR;
    ఒకవేళ ((filter->action.mods.flags & unlock) == ACTION_తాళం_CLEAR)
        state->పునాది.components.locked_mods &= ~filter->action.mods.mods.ముసుగు;

    filter->func = ఖాళీ;
    తిరిగి XKB_FILTER_CONTINUE;
}

స్థిర శూన్య
xkb_filter_mod_తాళం_new(నిర్మాణం xkb_server_state *state,
                        నిర్మాణం xkb_events *events,
                        నిర్మాణం xkb_filter *filter)
{
    filter->priv = (state->పునాది.components.locked_mods &
                    filter->action.mods.mods.ముసుగు);

    ఒకవేళ (filter->priv && (filter->action.mods.flags & ACTION_UNLOCK_ON_PRESS)) {
        /*
         * Some of the target modifiers were locked before key press: unlock.
         *
         * This is a keymap v2 extension: unlock-on-press.
         */
        ఒకవేళ (!(filter->action.mods.flags & ACTION_తాళం_NO_UNLOCK))
            state->పునాది.components.locked_mods &= ~filter->priv;
        /* No further action: cancel filter */
        filter->func = ఖాళీ;
    } లేకపోతే {
        /* Set base mods; lock mods if relevant (XKB 1.0 spec) */
        state->సమితి_mods |= filter->action.mods.mods.ముసుగు;
        ఒకవేళ (!(filter->action.mods.flags & ACTION_తాళం_NO_తాళం))
            state->పునాది.components.locked_mods |= filter->action.mods.mods.ముసుగు;
    }
}

స్థిర bool
xkb_filter_mod_తాళం_func(నిర్మాణం xkb_server_state *state,
                         నిర్మాణం xkb_events *events,
                         నిర్మాణం xkb_filter *filter,
                         స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                         గణన xkb_కీలకం_direction direction)
{
    ఒకవేళ (కీలకం != filter->కీలకం)
        తిరిగి XKB_FILTER_CONTINUE;

    ఎంపిక (direction) {
    స్థితి XKB_కీలకం_కిందికి:
        filter->refcnt++;
        /* fallthrough */
    స్థితి XKB_కీలకం_REPEATED:
        తిరిగి XKB_FILTER_CONSUME;
    అప్రమేయ:
        ఒకవేళ (--filter->refcnt > 0)
            తిరిగి XKB_FILTER_CONSUME;
    }

    state->clear_mods |= filter->action.mods.mods.ముసుగు;
    ఒకవేళ (!(filter->action.mods.flags & ACTION_తాళం_NO_UNLOCK))
        state->పునాది.components.locked_mods &= ~filter->priv;

    filter->func = ఖాళీ;
    తిరిగి XKB_FILTER_CONTINUE;
}

స్థిర శూన్య
xkb_filter_mod_latch_new(నిర్మాణం xkb_server_state *state,
                         నిర్మాణం xkb_events *events,
                         నిర్మాణం xkb_filter *filter)
{
    /* Latch-on-press + clear-locks imply unlock-on-press */
    స్థిరరాశి గణన xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                              | ACTION_LATCH_ON_PRESS;

    ఒకవేళ ((filter->action.mods.flags & ACTION_తాళం_CLEAR) &&
        (filter->action.mods.flags & unlockOnPress) &&
        (state->పునాది.components.locked_mods & filter->action.mods.mods.ముసుగు) ==
         filter->action.mods.mods.ముసుగు) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension: clear locks and do not latch.
         */
        state->పునాది.components.locked_mods &= ~filter->action.mods.mods.ముసుగు;
        filter->func = ఖాళీ;
    } లేకపోతే ఒకవేళ (filter->action.mods.flags & ACTION_LATCH_ON_PRESS) {
        /*
         * Latch on key press
         *
         * This is a keymap format v2 extension.
         */
        filter->priv = LATCH_PENDING;
        state->పునాది.components.latched_mods |= filter->action.mods.mods.ముసుగు;
        /* XXX beep beep! */
    } లేకపోతే {
        /* XKB standard latch action */
        filter->priv = LATCH_కీలకం_కిందికి;
        state->సమితి_mods |= filter->action.mods.mods.ముసుగు;
    }
}

స్థిర bool
xkb_filter_mod_latch_func(నిర్మాణం xkb_server_state *state,
                          నిర్మాణం xkb_events *events,
                          నిర్మాణం xkb_filter *filter,
                          స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                          గణన xkb_కీలకం_direction direction)
{
    గణన xkb_కీలకం_latch_state latch = filter->priv;

    ఒకవేళ (direction == XKB_కీలకం_కిందికి) {
        స్థిరరాశి సంఘం xkb_action *actions = ఖాళీ;
        స్థిరరాశి xkb_action_లెక్క_t లెక్క = xkb_కీలకం_get_actions(&state->పునాది, కీలకం,
                                                             &actions);

        ఒకవేళ (latch == LATCH_కీలకం_కిందికి) {
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
            ఒకవేళ (state->a11y_flags & XKB_A11_Y_LATCH_ఏకకాల_కీలకాలు) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                కొరకు (xkb_action_లెక్క_t k = 0; k < లెక్క; k++) {
                    ఒకవేళ (xkb_action_breaks_latch(&(actions[k]),
                                                state->పునాది.components.controls,
                                                అంతర్గత_BREAKS_MOD_LATCH,
                                                filter->action.mods.mods.ముసుగు))
                    {
                        latch = NO_LATCH;
                        ఆపు;
                    }
                }
            } లేకపోతే {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        లేకపోతే ఒకవేళ (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is pressed,
             * or promote it to a lock or plain base set if it's the same
             * modifier. */
            స్థిరరాశి bool sticky_కీలకాలు = state->పునాది.components.controls
                                   & CONTROL_STICKY_కీలకాలు;
            /*
             * NOTE: Action’s flags are not compared:
             * - flags may have been modified by keyboard controls at the
             *   creation of the filter;
             * - it is unlikely to have two LatchMods() actions on the same
             *   key that differ only by their flags;
             * - even if it were the case, one would most likely want both
             *   actions to be considered compatible in this context.
             */
            కొరకు (xkb_action_లెక్క_t k = 0; k < లెక్క; k++) {
                ఒకవేళ (((actions[k].type == ACTION_TYPE_MOD_LATCH) ||
                     (actions[k].type == ACTION_TYPE_MOD_సమితి && sticky_కీలకాలు)) &&
                    actions[k].mods.mods.ముసుగు == filter->action.mods.mods.ముసుగు)
                {
                    ఒకవేళ (filter->action.mods.flags & ACTION_LATCH_గమ్యం_తాళం) {
                        /* Mutate the action to LockMods() */
                        filter->action.type = ACTION_TYPE_MOD_తాళం;
                        filter->func = xkb_filter_mod_తాళం_func;
                        xkb_filter_mod_తాళం_new(state, events, filter);
                    }
                    లేకపోతే {
                        /* Mutate the action to SetMods() */
                        filter->action.type = ACTION_TYPE_MOD_సమితి;
                        filter->func = xkb_filter_mod_సమితి_func;
                        xkb_filter_mod_సమితి_new(state, events, filter);
                    }
                    filter->కీలకం = కీలకం;
                    /* Clear latches */
                    state->పునాది.components.latched_mods &=
                        ~filter->action.mods.mods.ముసుగు;
                    /* XXX beep beep! */
                    తిరిగి XKB_FILTER_CONSUME;
                }
                లేకపోతే ఒకవేళ (xkb_action_breaks_latch(&(actions[k]),
                                                 state->పునాది.components.controls,
                                                 అంతర్గత_BREAKS_MOD_LATCH,
                                                 filter->action.mods.mods.ముసుగు))
                {
                    /* XXX: This may be totally broken, we might need to break the
                     *      latch in the next run after this press? */
                    state->పునాది.components.latched_mods &=
                        ~filter->action.mods.mods.ముసుగు;
                    filter->func = ఖాళీ;
                    తిరిగి XKB_FILTER_CONTINUE;
                }
            }
        }
        లేకపోతే {
            /* Ignore press in NO_LATCH state */
            ప్రతిజ్ఞా(latch == NO_LATCH);
        }
    }
    లేకపోతే ఒకవేళ (కీలకం != filter->కీలకం) {
        /* Ignore repeat/release of other keys */
    }
    లేకపోతే ఒకవేళ (direction == XKB_కీలకం_REPEATED) {
        తిరిగి XKB_FILTER_CONSUME; // TODO: check
    }
    లేకపోతే {
        /* Our key got released.  If we’ve set it to clear locks, and we
         * currently have the same modifiers locked, then release them and
         * don't actually latch.  Else we’ve actually hit the latching
         * stage, so set PENDING and move our modifier from base to
         * latched. */

        /* Latch-on-press + clear-locks imply unlock-on-press */
        స్థిరరాశి గణన xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                                  | ACTION_LATCH_ON_PRESS;

        ఒకవేళ ((filter->action.mods.flags & ACTION_తాళం_CLEAR) &&
            !(filter->action.mods.flags & unlockOnPress) &&
            (state->పునాది.components.locked_mods &
             filter->action.mods.mods.ముసుగు) == filter->action.mods.mods.ముసుగు)
        {
            /* XXX: We might be a bit overenthusiastic about clearing
             *      mods other filters have set here? */
            ఒకవేళ (latch == LATCH_PENDING)
                state->పునాది.components.latched_mods &=
                    ~filter->action.mods.mods.ముసుగు;
            లేకపోతే
                state->clear_mods |= filter->action.mods.mods.ముసుగు;
            state->పునాది.components.locked_mods &= ~filter->action.mods.mods.ముసుగు;
            filter->func = ఖాళీ;
        }
        లేకపోతే ఒకవేళ (latch == NO_LATCH) {
            /* Broken latch */
            state->clear_mods |= filter->action.mods.mods.ముసుగు;
            filter->func = ఖాళీ;
        }
        లేకపోతే ఒకవేళ (!(filter->action.mods.flags & ACTION_LATCH_ON_PRESS)) {
            latch = LATCH_PENDING;
            state->clear_mods |= filter->action.mods.mods.ముసుగు;
            state->పునాది.components.latched_mods |= filter->action.mods.mods.ముసుగు;
            /* XXX beep beep! */
        }
    }

    filter->priv = latch;

    తిరిగి XKB_FILTER_CONTINUE;
}

స్థిర శూన్య
append_సూచిక_move(నిర్మాణం xkb_events *events,
                    నిర్మాణం xkb_filter *filter)
{
    గణన xkb_సూచిక_motion_flags flags = 0;
    ఒకవేళ (filter->action.ptr.flags & ACTION_ABSOLUTE_X)
        flags |= XKB_సూచిక_MOTION_ABSOLUTE_X;
    ఒకవేళ (filter->action.ptr.flags & ACTION_ABSOLUTE_Y)
        flags |= XKB_సూచిక_MOTION_ABSOLUTE_Y;
    ఒకవేళ (filter->action.ptr.flags & ACTION_REPEAT)
        flags |= XKB_సూచిక_MOTION_REPEATS;
    darray_append(events->వరుస, (నిర్మాణం xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_సూచిక_MOTION,
        .సూచిక_motion = {
            .కొలత = పరిమాణం(((నిర్మాణం xkb_event*)0)->సూచిక_motion),
            .flags = flags,
            .x = filter->action.ptr.x,
            .y = filter->action.ptr.y,
        }
    });
}

స్థిర శూన్య
xkb_filter_సూచిక_move_new(నిర్మాణం xkb_server_state *state,
                            నిర్మాణం xkb_events *events,
                            నిర్మాణం xkb_filter *filter)
{
    ఒకవేళ (!events) {
        filter->func = ఖాళీ;
        తిరిగి;
    }

    ప్రతిజ్ఞా(state->పునాది.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_కీలకాలు);
    append_సూచిక_move(events, filter);
    state->flags &= ~STATE_REQUIRE_కీలకం_EVENT;
}

స్థిర bool
xkb_filter_సూచిక_move_func(నిర్మాణం xkb_server_state *state,
                             నిర్మాణం xkb_events *events,
                             నిర్మాణం xkb_filter *filter,
                             స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                             గణన xkb_కీలకం_direction direction)
{
    ఒకవేళ (కీలకం != filter->కీలకం || !events)
        తిరిగి XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_కీలకం_EVENT;

    ఎంపిక (direction) {
    స్థితి XKB_కీలకం_కిందికి:
        filter->refcnt++;
        append_సూచిక_move(events, filter);
        తిరిగి XKB_FILTER_CONSUME;
    స్థితి XKB_కీలకం_REPEATED:
        ఒకవేళ (filter->action.ptr.flags & ACTION_REPEAT)
            append_సూచిక_move(events, filter);
        తిరిగి XKB_FILTER_CONSUME;
    అప్రమేయ:
        ఒకవేళ (--filter->refcnt > 0)
            తిరిగి XKB_FILTER_CONSUME;
    }

    filter->func = ఖాళీ;
    తిరిగి XKB_FILTER_CONSUME;
}

/* Implemented with xkb_machine API because it requires struct xkb_machine */
స్థిర శూన్య
xkb_filter_సూచిక_button_new(నిర్మాణం xkb_server_state *state,
                              నిర్మాణం xkb_events *events,
                              నిర్మాణం xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
స్థిర bool
xkb_filter_సూచిక_button_func(నిర్మాణం xkb_server_state *state,
                               నిర్మాణం xkb_events *events,
                               నిర్మాణం xkb_filter *filter,
                               స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                               గణన xkb_కీలకం_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
స్థిర శూన్య
xkb_filter_సూచిక_తాళం_button_new(నిర్మాణం xkb_server_state *state,
                                   నిర్మాణం xkb_events *events,
                                   నిర్మాణం xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
స్థిర bool
xkb_filter_సూచిక_తాళం_button_func(నిర్మాణం xkb_server_state *state,
                                    నిర్మాణం xkb_events *events,
                                    నిర్మాణం xkb_filter *filter,
                                    స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                                    గణన xkb_కీలకం_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
స్థిర శూన్య
xkb_filter_సూచిక_సమితి_default_button_new(నిర్మాణం xkb_server_state *state,
                                          నిర్మాణం xkb_events *events,
                                          నిర్మాణం xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
స్థిర bool
xkb_filter_సూచిక_సమితి_default_button_func(నిర్మాణం xkb_server_state *state,
                                           నిర్మాణం xkb_events *events,
                                           నిర్మాణం xkb_filter *filter,
                                           స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                                           గణన xkb_కీలకం_direction direction);

స్థిర శూన్య
xkb_filter_terminate_server_new(నిర్మాణం xkb_server_state *state,
                                 నిర్మాణం xkb_events *events,
                                 నిర్మాణం xkb_filter *filter);

స్థిర bool
xkb_filter_terminate_server_func(నిర్మాణం xkb_server_state *state,
                                  నిర్మాణం xkb_events *events,
                                  నిర్మాణం xkb_filter *filter,
                                  స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                                  గణన xkb_కీలకం_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
స్థిర శూన్య
xkb_filter_switch_virtual_console_new(నిర్మాణం xkb_server_state *state,
                                      నిర్మాణం xkb_events *events,
                                      నిర్మాణం xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
స్థిర bool
xkb_filter_switch_virtual_console_func(నిర్మాణం xkb_server_state *state,
                                       నిర్మాణం xkb_events *events,
                                       నిర్మాణం xkb_filter *filter,
                                       స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                                       గణన xkb_కీలకం_direction direction);

స్థిర అంతఃవరుస శూన్య
clear_all_latches_and_locks(నిర్మాణం xkb_server_state *state,
                            నిర్మాణం xkb_events *events);

స్థిర శూన్య
xkb_filter_ctrls_new(నిర్మాణం xkb_server_state *state,
                     నిర్మాణం xkb_events *events,
                     నిర్మాణం xkb_filter *filter)
{
    ఒకవేళ (filter->action.type == ACTION_TYPE_CTRL_సమితి) {
        /* Set: save the specified controls that are *not* already enabled */
        filter->priv = (uint32_t) (
            (~state->పునాది.components.controls) & filter->action.ctrls.ctrls
        );
    } లేకపోతే {
        /* Lock: save the specified controls that *are* already enabled */
        filter->priv = (uint32_t) (
            state->పునాది.components.controls & filter->action.ctrls.ctrls
        );
    }

    ఒకవేళ (filter->action.type == ACTION_TYPE_CTRL_సమితి ||
        !(filter->action.ctrls.flags & ACTION_తాళం_NO_తాళం)) {
        /* Enable the specified controls that are not already enabled */
        state->పునాది.components.controls |= filter->action.ctrls.ctrls;
    }
}

స్థిర bool
xkb_filter_ctrls_func(నిర్మాణం xkb_server_state *state,
                      నిర్మాణం xkb_events *events,
                      నిర్మాణం xkb_filter *filter,
                      స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                      గణన xkb_కీలకం_direction direction)
{
    ఒకవేళ (కీలకం != filter->కీలకం)
        తిరిగి XKB_FILTER_CONTINUE;

    ఎంపిక (direction) {
    స్థితి XKB_కీలకం_కిందికి:
        filter->refcnt++;
        /* fallthrough */
    స్థితి XKB_కీలకం_REPEATED:
        తిరిగి XKB_FILTER_CONSUME;
    అప్రమేయ:
        ఒకవేళ (--filter->refcnt > 0)
            తిరిగి XKB_FILTER_CONSUME;
    }

    ఒకవేళ (filter->action.type == ACTION_TYPE_CTRL_సమితి ||
        !(filter->action.ctrls.flags & ACTION_తాళం_NO_UNLOCK)) {
        స్థిరరాశి గణన xkb_action_controls old = state->పునాది.components.controls;

        /*
         * Set: Disable specified controls that were *not* enabled at key press.
         * Lock: Disable specified controls that *were* enabled at key press.
         */
        state->పునాది.components.controls &= ~(గణన xkb_action_controls) filter->priv;

        ఒకవేళ ((old & CONTROL_STICKY_కీలకాలు) &&
            !(state->పునాది.components.controls & CONTROL_STICKY_కీలకాలు)) {
            /* Sticky keys were disabled: clear all locks and latches */
            clear_all_latches_and_locks(state, events);
        }
    }

    filter->func = ఖాళీ;
    తిరిగి XKB_FILTER_CONTINUE;
}

స్థిర bool
append_redirect_కీలకం_events(నిర్మాణం xkb_server_state *state,
                           నిర్మాణం xkb_events *events,
                           స్థిరరాశి నిర్మాణం xkb_redirect_కీలకం_action *redirect,
                           గణన xkb_కీలకం_direction direction)
{
    గణన xkb_state_component మారింది = 0;
    స్థిరరాశి xkb_mod_ముసుగు_t ముసుగు = redirect->affect_mods;

    /*
     * Reference state: find the last state update in the queue, otherwise
     * use the current state.
     */
    నిర్మాణం xkb_event *event;
    నిర్మాణం state_components చివరి_components = state->పునాది.components;
    darray_foreach_తిరగేయి(event, events->వరుస) {
        ఒకవేళ (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
            చివరి_components = event->components.components;
            ఆపు;
        }
    }

    ఒకవేళ (ముసుగు) {
        నిర్మాణం state_components new = చివరి_components;
        new.పునాది_mods = (new.పునాది_mods & ~ముసుగు) | redirect->mods;
        new.latched_mods = (new.latched_mods & ~ముసుగు) | redirect->mods;
        new.locked_mods = (new.locked_mods & ~ముసుగు) | redirect->mods;
        new.mods = (new.mods & ~ముసుగు) | redirect->mods;

        మారింది = get_state_component_changes(&చివరి_components, &new);
        ఒకవేళ (మారింది) {
            darray_append(events->వరుస, (నిర్మాణం xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = new,
                    .మారింది = మారింది
                }
            });
            /*
             * Commit the temporary modifiers change before the key event
             * it was computed for: consumers must observe the key event
             * under the *redirected* modifiers.
             */
            darray_append(events->వరుస, (నిర్మాణం xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_చట్రం,
            });
        }
    }

    darray_append(events->వరుస, (నిర్మాణం xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_కీలకం,
        .కీలకం = {
            .keycode = redirect->keycode,
            .direction = direction
        }
    });

    ఒకవేళ (ముసుగు && మారింది) {
        /* Restore state */
        darray_append(events->వరుస, (నిర్మాణం xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = చివరి_components,
                .మారింది = మారింది
            }
        });
    }

    state->flags &= ~STATE_REQUIRE_కీలకం_EVENT;
    తిరిగి true;
}

స్థిర శూన్య
xkb_filter_redirect_కీలకం_new(నిర్మాణం xkb_server_state *state,
                            నిర్మాణం xkb_events *events,
                            నిర్మాణం xkb_filter *filter)
{
    /* Action effectual only with the xkb_machine API and a valid keycode */
    ఒకవేళ (!events || filter->action.redirect.keycode == XKB_KEYCODE_INVALID) {
        filter->func = ఖాళీ;
        తిరిగి;
    }
    append_redirect_కీలకం_events(state, events, &filter->action.redirect,
                               XKB_కీలకం_కిందికి);
}

స్థిర bool
xkb_filter_redirect_కీలకం_func(నిర్మాణం xkb_server_state *state,
                             నిర్మాణం xkb_events *events,
                             నిర్మాణం xkb_filter *filter,
                             స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                             గణన xkb_కీలకం_direction direction)
{
    ఒకవేళ (కీలకం != filter->కీలకం)
        తిరిగి XKB_FILTER_CONTINUE;

    ఒకవేళ (direction == XKB_కీలకం_పైకి) {
        append_redirect_కీలకం_events(state, events,
                                   &filter->action.redirect, XKB_కీలకం_పైకి);
        filter->func = ఖాళీ;
        తిరిగి XKB_FILTER_CONSUME;
     } లేకపోతే ఒకవేళ (direction == XKB_కీలకం_కిందికి) {
        స్థిరరాశి సంఘం xkb_action *actions = ఖాళీ;
        స్థిరరాశి xkb_action_లెక్క_t లెక్క =
            xkb_కీలకం_get_actions(&state->పునాది, కీలకం, &actions);

        కొరకు (xkb_action_లెక్క_t a = 0; a < లెక్క; a++) {
            ఒకవేళ (actions[a].type == ACTION_TYPE_REDIRECT_కీలకం &&
                actions[a].redirect.keycode != filter->action.redirect.keycode) {
                    /* One action redirects to a different key: release first */
                    append_redirect_కీలకం_events(state, events,
                                               &filter->action.redirect,
                                               XKB_కీలకం_పైకి);
                filter->func = ఖాళీ;
                తిరిగి XKB_FILTER_CONTINUE;
            }
        }
    }

    append_redirect_కీలకం_events(state, events, &filter->action.redirect,
                               direction);
    తిరిగి XKB_FILTER_CONSUME;
}

స్థిర స్థిరరాశి నిర్మాణం {
    శూన్య (*new)(నిర్మాణం xkb_server_state *state,
                నిర్మాణం xkb_events *events,
                నిర్మాణం xkb_filter *filter);
    bool (*func)(నిర్మాణం xkb_server_state *state,
                 నిర్మాణం xkb_events *events,
                 నిర్మాణం xkb_filter *filter,
                 స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం, గణన xkb_కీలకం_direction direction);
} filter_action_funcs[ACTION_TYPE_అంకె_ENTRIES] = {
    [ACTION_TYPE_MOD_సమితి]     = { xkb_filter_mod_సమితి_new,
                                  xkb_filter_mod_సమితి_func },
    [ACTION_TYPE_MOD_LATCH]   = { xkb_filter_mod_latch_new,
                                  xkb_filter_mod_latch_func },
    [ACTION_TYPE_MOD_తాళం]    = { xkb_filter_mod_తాళం_new,
                                  xkb_filter_mod_తాళం_func },
    [ACTION_TYPE_సమూహం_సమితి]   = { xkb_filter_సమూహం_సమితి_new,
                                  xkb_filter_సమూహం_సమితి_func },
    [ACTION_TYPE_సమూహం_LATCH] = { xkb_filter_సమూహం_latch_new,
                                  xkb_filter_సమూహం_latch_func },
    [ACTION_TYPE_సమూహం_తాళం]  = { xkb_filter_సమూహం_తాళం_new,
                                  xkb_filter_సమూహం_తాళం_func },
    [ACTION_TYPE_PTR_MOVE]    = { xkb_filter_సూచిక_move_new,
                                  xkb_filter_సూచిక_move_func },
    [ACTION_TYPE_PTR_BUTTON]  = { xkb_filter_సూచిక_button_new,
                                  xkb_filter_సూచిక_button_func },
    [ACTION_TYPE_PTR_తాళం]    = { xkb_filter_సూచిక_తాళం_button_new,
                                  xkb_filter_సూచిక_తాళం_button_func },
    [ACTION_TYPE_PTR_DEFAULT] = { xkb_filter_సూచిక_సమితి_default_button_new,
                                  xkb_filter_సూచిక_సమితి_default_button_func },
    [ACTION_TYPE_TERMINATE]   = { xkb_filter_terminate_server_new,
                                  xkb_filter_terminate_server_func },
    [ACTION_TYPE_SWITCH_VT]   = { xkb_filter_switch_virtual_console_new,
                                  xkb_filter_switch_virtual_console_func },
    [ACTION_TYPE_CTRL_సమితి]    = { xkb_filter_ctrls_new,
                                  xkb_filter_ctrls_func },
    [ACTION_TYPE_CTRL_తాళం]   = { xkb_filter_ctrls_new,
                                  xkb_filter_ctrls_func },
    [ACTION_TYPE_REDIRECT_కీలకం] = { xkb_filter_redirect_కీలకం_new,
                                   xkb_filter_redirect_కీలకం_func },
};

/**
 * Applies any relevant filters to the key, first from the list of filters
 * that are currently active, then if no filter has claimed the key, possibly
 * apply a new filter from the key action.
 */
స్థిర శూన్య
xkb_filter_apply_all(నిర్మాణం xkb_server_state *state,
                     నిర్మాణం xkb_events *events,
                     స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                     గణన xkb_కీలకం_direction direction)
{
    /* First run through all the currently active filters and see if any of
     * them have consumed this event. */
    bool consumed = false;
    నిర్మాణం xkb_filter *filter;
    darray_foreach(filter, state->filters) {
        ఒకవేళ (!filter->func)
            కొనసాగు;

        ఒకవేళ (filter->func(state, events, filter, కీలకం, direction) == XKB_FILTER_CONSUME)
            consumed = true;
    }
    ఒకవేళ (consumed || direction == XKB_కీలకం_పైకి)
        తిరిగి;

    /* No filter consumed this event, so proceed with the key actions */
    స్థిరరాశి సంఘం xkb_action *actions = ఖాళీ;
    xkb_action_లెక్క_t లెక్క = xkb_కీలకం_get_actions(&state->పునాది, కీలకం,
                                                         &actions);

    కొరకు (నిర్మాణం {
            xkb_action_లెక్క_t లెక్క;
            స్థిరరాశి సంఘం xkb_action *actions;
         } తదుపరి = {0};
         లెక్క;
         లెక్క = తదుపరి.లెక్క, తదుపరి.లెక్క = 0, actions = తదుపరి.actions
    ) {
        /*
         * Process actions sequentially.
         *
         * NOTE: We rely on the parser to disallow multiple modifier or group
         * actions (see `CheckMultipleActionsCategories`). Allowing multiple
         * such actions requires a refactor of the state handling.
         */
        కొరకు (xkb_action_లెక్క_t k = 0; k < లెక్క; k++) {
            /*
             * It's possible for the keymap to set action->type explicitly, like
             * so:
             *     interpret XF86_Next_VMode {
             *         action = Private(type=0x86, data="+VMode");
             *     };
             * We don't handle those.
             */
            ఒకవేళ (actions[k].type >= ACTION_TYPE_అంకె_ENTRIES)
                కొనసాగు;

            /* Go to next action if no corresponding action handler */
            ఒకవేళ (!filter_action_funcs[actions[k].type].new)
                కొనసాగు;

            /* Add a new filter and run the corresponding initial action */
            filter = xkb_filter_new(state);
            filter->కీలకం = కీలకం;
            filter->action = actions[k];

            ఎంపిక (filter->action.type) {
            స్థితి ACTION_TYPE_MOD_సమితి:
                ఒకవేళ (state->పునాది.components.controls & CONTROL_STICKY_కీలకాలు) {
                    /* Convert modifier set action to a latch */
                    filter->action.type = ACTION_TYPE_MOD_LATCH;
                    ఒకవేళ (state->a11y_flags & XKB_A11_Y_STICKY_కీలకాలు_LATCH_గమ్యం_తాళం) {
                        filter->action.mods.flags |= ACTION_LATCH_గమ్యం_తాళం;
                    }
                }
                /* fallthrough */
            స్థితి ACTION_TYPE_MOD_LATCH:
            స్థితి ACTION_TYPE_MOD_తాళం: {
                స్థిర స్థిరరాశి గణన xkb_action_flags unlock_mods_on_press
                    = ACTION_తాళం_CLEAR     /* SetMods & LatchMods: can unlock */
                    | ACTION_తాళం_NO_UNLOCK /* LockMods: cannot unlock */
                    | ACTION_EXPLICIT_UNLOCK_ON_PRESS;
                ఒకవేళ ((state->flags & STATE_UNLOCK_MODS_ON_PRESS) &&
                    /*
                     * Ensure that the action can actually unlock and has
                     * no *explicit* `unlockOnPress` parameter.
                     */
                    (filter->action.mods.flags & unlock_mods_on_press) ==
                    (filter->action.type == ACTION_TYPE_MOD_తాళం
                        ? 0 : ACTION_తాళం_CLEAR))
                {
                    filter->action.mods.flags |= ACTION_UNLOCK_ON_PRESS;
                }
                ఆపు;
            }
            స్థితి ACTION_TYPE_సమూహం_సమితి:
                ఒకవేళ (state->పునాది.components.controls & CONTROL_STICKY_కీలకాలు) {
                    /* Convert group set action to a latch */
                    filter->action.type = ACTION_TYPE_సమూహం_LATCH;
                    ఒకవేళ (state->a11y_flags & XKB_A11_Y_STICKY_కీలకాలు_LATCH_గమ్యం_తాళం) {
                        filter->action.సమూహం.flags |= ACTION_LATCH_గమ్యం_తాళం;
                    }
                }
                ఆపు;
            స్థితి ACTION_TYPE_సమూహం_తాళం:
                ఒకవేళ ((filter->action.సమూహం.flags &
                     ACTION_AUTO_సమూహం_తాళం_ON_MODIFIER_RELEASE) &&
                    (state->flags &
                     STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_కీలకం) &&
                    /*
                     * FIXME: sticky keys may leave an unexpected modifier
                     * from the level 0 latched, so disable for now.
                     */
                    !(state->పునాది.components.controls & CONTROL_STICKY_కీలకాలు))
                {
                    filter->action.సమూహం.flags |= ACTION_తాళం_ON_RELEASE;
                    /* Add base level action */
                    తదుపరి.లెక్క = xkb_keymap_కీలకం_get_actions_by_level(
                        state->పునాది.keymap, కీలకం, state->పునాది.components.సమూహం,
                        0, &తదుపరి.actions
                    );
                    ప్రతిజ్ఞా(తదుపరి.లెక్క == 1);
                }
                ఆపు;
            స్థితి ACTION_TYPE_PTR_MOVE:
            స్థితి ACTION_TYPE_PTR_BUTTON:
            స్థితి ACTION_TYPE_PTR_తాళం:
            స్థితి ACTION_TYPE_PTR_DEFAULT:
                ఒకవేళ (!(state->పునాది.components.controls & CONTROL_MOUSE_కీలకాలు)) {
                    /* Convert pointer actions to NoAction */
                    ప్రతిజ్ఞా(!filter_action_funcs[ACTION_TYPE_NONE].new);
                    filter->func = ఖాళీ;
                    కొనసాగు;
                }
                ఆపు;
            స్థితి ACTION_TYPE_REDIRECT_కీలకం:
                // FIXME: this is not efficient to resolve mods here each time
                filter->action.redirect.affect_mods = mod_ముసుగు_get_effective(
                    state->పునాది.keymap, filter->action.redirect.affect_mods
                );
                filter->action.redirect.mods = mod_ముసుగు_get_effective(
                    state->పునాది.keymap, filter->action.redirect.mods
                );
                ఆపు;
            అప్రమేయ: ;
            }
            filter->func = filter_action_funcs[filter->action.type].func;
            filter_action_funcs[filter->action.type].new(state, events, filter);
        }
    }
}

స్థిర శూన్య
xkb_state_update_derived(నిర్మాణం xkb_state *state);

స్థిర అంతఃవరుస శూన్య
xkb_state_init(నిర్మాణం xkb_state * పరిమిత state,
               నిర్మాణం xkb_keymap * పరిమిత keymap,
               గణన xkb_state_బహుళకం_అంతర్గత బహుళకం)
{
    state->బహుళకం = బహుళకం;
    state->refcnt = 1;
    state->keymap = xkb_keymap_ref(keymap);
    state->బహిర్గతం_యొక్క_range_సమూహం.policy = XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_WRAP;

    /* Ensure that derived state is correctly initialized */
    xkb_state_update_derived(state);
}

స్థిర అంతఃవరుస శూన్య
xkb_client_state_init(నిర్మాణం xkb_client_state * పరిమిత state,
                      నిర్మాణం xkb_keymap * పరిమిత keymap,
                      గణన xkb_state_బహుళకం_అంతర్గత బహుళకం)
{
    xkb_state_init(&state->పునాది, keymap, బహుళకం);
}

స్థిర అంతఃవరుస శూన్య
xkb_server_state_init(నిర్మాణం xkb_server_state * పరిమిత state,
                      నిర్మాణం xkb_keymap * పరిమిత keymap,
                      గణన xkb_state_బహుళకం_అంతర్గత బహుళకం,
                      గణన xkb_a11y_flags a11y_affect,
                      గణన xkb_a11y_flags a11y_flags)
{
    xkb_state_init(&state->పునాది, keymap, బహుళకం);

    state->a11y_flags = a11y_flags;
    ఒకవేళ (keymap->format != XKB_KEYMAP_FORMAT_పాఠ్యం_V1 &&
        !(a11y_affect & XKB_A11_Y_LATCH_ఏకకాల_కీలకాలు)) {
             /* Keymap v2+: enable extension to XKB if not manually disabled */
             state->a11y_flags |= XKB_A11_Y_LATCH_ఏకకాల_కీలకాలు;
    }
}

నిర్మాణం xkb_state *
xkb_state_new(నిర్మాణం xkb_keymap *keymap)
{
    నిర్మాణం xkb_server_state * స్థిరరాశి state = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(*state));
    ఒకవేళ (!state) {
        log_err_func1(keymap->ctx, XKB_ERROR_కేటాయింపు_FAILURE_2,
                      "Could not allocate a state object.\n");
        తిరిగి ఖాళీ;
    }

    xkb_server_state_init(state, keymap, LEGACY_MIXED_STATE, 0, 0);

    తిరిగి (నిర్మాణం xkb_state *)state;
}

నిర్మాణం xkb_state *
xkb_state_new_with_బహుళకం(నిర్మాణం xkb_keymap * పరిమిత keymap,
                        గణన xkb_state_బహుళకం బహుళకం,
                        గణన xkb_స్థితి * పరిమిత స్థితి_2)
{
    ఎంపిక (బహుళకం) {
    స్థితి XKB_STATE_బహుళకం_CLIENT:
    స్థితి XKB_STATE_బహుళకం_SERVER_QUERY: {
        నిర్మాణం xkb_client_state * స్థిరరాశి state = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(*state));
        ఒకవేళ (!state)
            ఆపు;

        static_assert((గుర్తులేని)XKB_STATE_బహుళకం_CLIENT ==
                      (గుర్తులేని)CLIENT_STATE, "");
        static_assert((గుర్తులేని)XKB_STATE_బహుళకం_SERVER_QUERY ==
                      (గుర్తులేని)SERVER_COMPANION, "");
        xkb_client_state_init(state, keymap, (గణన xkb_state_బహుళకం_అంతర్గత)బహుళకం);

        ఒకవేళ (స్థితి_2)
            *స్థితి_2 = XKB_SUCCESS;

        తిరిగి (నిర్మాణం xkb_state *)state;
    }
    స్థితి XKB_STATE_బహుళకం_SERVER: {
        నిర్మాణం xkb_server_state * స్థిరరాశి state = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(*state));
        ఒకవేళ (!state)
            ఆపు;

        xkb_server_state_init(state, keymap, LEGACY_SERVER_STATE, 0, 0);

        ఒకవేళ (స్థితి_2)
            *స్థితి_2 = XKB_SUCCESS;

        తిరిగి (నిర్మాణం xkb_state *)state;
    }
    అప్రమేయ:
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_STATE_బహుళకం_2,
                     "Unsupported state mode: %u\n", బహుళకం);
        ఒకవేళ (స్థితి_2)
            *స్థితి_2 = XKB_ERROR_UNSUPPORTED_STATE_బహుళకం;
        తిరిగి ఖాళీ;
    }

    log_err_func1(keymap->ctx, XKB_ERROR_కేటాయింపు_FAILURE_2,
                  "Could not allocate a state object.\n");
    ఒకవేళ (స్థితి_2)
        *స్థితి_2 = XKB_ERROR_కేటాయింపు_FAILURE;
    తిరిగి ఖాళీ;
}

నిర్మాణం xkb_state *
xkb_state_ref(నిర్మాణం xkb_state *state)
{
    ప్రతిజ్ఞా(state->refcnt > 0);
    state->refcnt++;
    తిరిగి state;
}

శూన్య
xkb_state_reset(నిర్మాణం xkb_state *state)
{
    memset(&state->components, 0, పరిమాణం(state->components));

    ఒకవేళ (state->బహుళకం < LEGACY_MIXED_STATE) {
        static_assert(పరిమాణం(*state) == పరిమాణం(నిర్మాణం xkb_client_state),
                      "Not a mere wrapper");
        /* Nothing else to reset */
    } లేకపోతే {
        నిర్మాణం xkb_server_state *state_ = (నిర్మాణం xkb_server_state *)state;
        స్థిరరాశి నిర్మాణం xkb_server_state prev = *state_;
        memset(state_, 0, పరిమాణం(*state_));

        state_->పునాది = prev.పునాది;
        state_->a11y_flags = prev.a11y_flags;
        state_->filters = prev.filters;
        darray_కొలత(state_->filters) = 0;
    }
}

స్థిర అంతఃవరుస శూన్య
xkb_state_destroy(నిర్మాణం xkb_state *state)
{
    xkb_keymap_unref(state->keymap);
    ఒకవేళ (state->బహుళకం > SERVER_COMPANION)
        darray_free(((నిర్మాణం xkb_server_state *)state)->filters);
}

శూన్య
xkb_state_unref(నిర్మాణం xkb_state *state)
{
    ప్రతిజ్ఞా(!state || state->refcnt > 0);
    ఒకవేళ (!state || --state->refcnt > 0)
        తిరిగి;

    xkb_state_destroy(state);

    ఎంపిక (state->బహుళకం) {
    స్థితి CLIENT_STATE:
    స్థితి SERVER_COMPANION:
        విడుదల((నిర్మాణం xkb_client_state *)state);
        ఆపు;
    స్థితి LEGACY_MIXED_STATE:
    స్థితి LEGACY_SERVER_STATE:
        విడుదల((నిర్మాణం xkb_server_state *)state);
        ఆపు;
    స్థితి SERVER_STATE: /* handled by xkb_machine_unref() */
    అప్రమేయ:
        PANIC_UNREACHABLE();
    }
}

నిర్మాణం xkb_keymap *
xkb_state_get_keymap(నిర్మాణం xkb_state *state)
{
    తిరిగి state->keymap;
}

/**
 * Update the LED state to match the rest of the xkb_state.
 */
స్థిర శూన్య
xkb_state_led_update_all(నిర్మాణం xkb_state *state)
{
    xkb_led_సూచీ_t idx;
    స్థిరరాశి నిర్మాణం xkb_led *led;

    state->components.leds = 0;

    xkb_leds_enumerate(idx, led, state->keymap) {

        ఒకవేళ (led->which_mods != 0 && led->mods.ముసుగు != 0) {
            xkb_mod_ముసుగు_t mod_ముసుగు = 0;
            ఒకవేళ (led->which_mods & XKB_STATE_MODS_EFFECTIVE)
                mod_ముసుగు |= state->components.mods;
            ఒకవేళ (led->which_mods & XKB_STATE_MODS_DEPRESSED)
                mod_ముసుగు |= state->components.పునాది_mods;
            ఒకవేళ (led->which_mods & XKB_STATE_MODS_LATCHED)
                mod_ముసుగు |= state->components.latched_mods;
            ఒకవేళ (led->which_mods & XKB_STATE_MODS_LOCKED)
                mod_ముసుగు |= state->components.locked_mods;

            ఒకవేళ (led->mods.ముసుగు & mod_ముసుగు) {
                state->components.leds |= (UINT32_C(1) << idx);
                కొనసాగు;
            }
        }

        ఒకవేళ (led->which_groups != 0) {
            ఒకవేళ (likely(led->groups) != 0) {
                xkb_layout_ముసుగు_t సమూహం_ముసుగు = 0;
                /* Effective and locked groups have been brought into range */
                ప్రతిజ్ఞా(state->components.సమూహం < XKB_MAX_GROUPS);
                ప్రతిజ్ఞా(state->components.locked_సమూహం >= 0 &&
                       state->components.locked_సమూహం < XKB_MAX_GROUPS);
                /* Effective and locked groups are used as mask */
                ఒకవేళ (led->which_groups & XKB_STATE_LAYOUT_EFFECTIVE)
                    సమూహం_ముసుగు |= (UINT32_C(1) << state->components.సమూహం);
                ఒకవేళ (led->which_groups & XKB_STATE_LAYOUT_LOCKED)
                    సమూహం_ముసుగు |= (UINT32_C(1) << state->components.locked_సమూహం);
                /* Base and latched groups only have to be non-zero */
                ఒకవేళ ((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                    state->components.పునాది_సమూహం != 0)
                    సమూహం_ముసుగు |= led->groups;
                ఒకవేళ ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                    state->components.latched_సమూహం != 0)
                    సమూహం_ముసుగు |= led->groups;

                ఒకవేళ (led->groups & సమూహం_ముసుగు) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    కొనసాగు;
                }
            } లేకపోతే {
                /* Special case for Base and latched groups */
                ఒకవేళ (((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                     state->components.పునాది_సమూహం == 0) ||
                    ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                     state->components.latched_సమూహం == 0)) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    కొనసాగు;
                }
            }
        }

        ఒకవేళ (led->ctrls & state->components.controls) {
            state->components.leds |= (UINT32_C(1) << idx);
            కొనసాగు;
        }
    }
}

/**
 * Calculates the derived state (effective mods/group and LEDs) from an
 * up-to-date xkb_state.
 */
స్థిర శూన్య
xkb_state_update_derived(నిర్మాణం xkb_state *state)
{
    xkb_layout_సూచీ_t wrapped;

    state->components.mods = (state->components.పునాది_mods |
                              state->components.latched_mods |
                              state->components.locked_mods);

    /* Lock group must be adjusted, but not base nor latched groups */
    wrapped = Xkb_Wrap_సమూహం_Into_Range(state->components.locked_సమూహం,
                                    state->keymap->అంకె_groups,
                                    state->బహిర్గతం_యొక్క_range_సమూహం.policy,
                                    state->బహిర్గతం_యొక్క_range_సమూహం.redirect_సమూహం);
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->components.locked_సమూహం =
        (int32_t) (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    /* Effective group must be adjusted */
    wrapped = Xkb_Wrap_సమూహం_Into_Range(state->components.పునాది_సమూహం +
                                    state->components.latched_సమూహం +
                                    state->components.locked_సమూహం,
                                    state->keymap->అంకె_groups,
                                    state->బహిర్గతం_యొక్క_range_సమూహం.policy,
                                    state->బహిర్గతం_యొక్క_range_సమూహం.redirect_సమూహం);
    state->components.సమూహం =
        (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    xkb_state_led_update_all(state);
}

స్థిర శూన్య
state_update_enabled_controls(నిర్మాణం xkb_server_state *state,
                              గణన xkb_keyboard_control_flags affect_controls,
                              గణన xkb_keyboard_control_flags controls,
                              నిర్మాణం xkb_events *events)
{
    స్థిరరాశి bool had_sticky_కీలకాలు = state->పునాది.components.controls
                               & CONTROL_STICKY_కీలకాలు;

    /*
     * Enable using the public API with the all the Control values, except
     * the internal ones, if any.
     */
    affect_controls =
        ( affect_controls
        & (గణన xkb_keyboard_control_flags)CONTROL_ALL_BOOLEAN );
    state->పునాది.components.controls &=
        (గణన xkb_action_controls)~affect_controls;
    state->పునాది.components.controls |=
        (గణన xkb_action_controls)(controls & affect_controls);

    ఒకవేళ (had_sticky_కీలకాలు &&
        !(state->పునాది.components.controls & CONTROL_STICKY_కీలకాలు)) {
        /* Sticky keys were disabled: clear all locks and latches */
        clear_all_latches_and_locks(state, events);
    }

    xkb_state_update_derived(&state->పునాది);
}

/**
 * Given a particular key event, updates the state structure to reflect the
 * new modifiers.
 */
గణన xkb_state_component
xkb_state_update_కీలకం(నిర్మాణం xkb_state *పునాది_state, xkb_keycode_t kc,
                     గణన xkb_కీలకం_direction direction)
{
    /* Guard against client-only state */
    ప్రతిజ్ఞా(పునాది_state->బహుళకం > SERVER_COMPANION);
    ఒకవేళ (పునాది_state->బహుళకం <= SERVER_COMPANION) {
        log_err(పునాది_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_బహుళకం,
                "%s: Unexpected state type %d\n", __func__, పునాది_state->బహుళకం);
        తిరిగి 0;
    }

    నిర్మాణం xkb_server_state * స్థిరరాశి state =
        (నిర్మాణం xkb_server_state *)పునాది_state;

    స్థిరరాశి నిర్మాణం xkb_కీలకం* స్థిరరాశి కీలకం = Xkb_కీలకం(state->పునాది.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    ఒకవేళ (!కీలకం || (direction == XKB_కీలకం_REPEATED && !కీలకం->repeats))
        తిరిగి 0;

    ఒకవేళ (direction == XKB_కీలకం_కిందికి &&
        (state->పునాది.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_కీలకాలు) &&
        (state->a11y_flags & XKB_A11_Y_STICKY_కీలకాలు_NO_ఏకకాల_కీలకాలు) &&
        state->పునాది.components.పునాది_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_కీలకాలు,
                                      0, ఖాళీ);
    }

    స్థిరరాశి నిర్మాణం state_components prev_components = state->పునాది.components;

    state->సమితి_mods = 0;
    state->clear_mods = 0;

    xkb_filter_apply_all(state, ఖాళీ, కీలకం, direction);

    xkb_mod_సూచీ_t i;
    xkb_mod_ముసుగు_t బిట్టు;
    కొరకు (i = 0, బిట్టు = 1; state->సమితి_mods; i++, బిట్టు <<= 1) {
        ఒకవేళ (state->సమితి_mods & బిట్టు) {
            state->mod_కీలకం_లెక్క[i]++;
            state->పునాది.components.పునాది_mods |= బిట్టు;
            state->సమితి_mods &= ~బిట్టు;
        }
    }

    కొరకు (i = 0, బిట్టు = 1; state->clear_mods; i++, బిట్టు <<= 1) {
        ఒకవేళ (state->clear_mods & బిట్టు) {
            state->mod_కీలకం_లెక్క[i]--;
            ఒకవేళ (state->mod_కీలకం_లెక్క[i] <= 0) {
                state->పునాది.components.పునాది_mods &= ~బిట్టు;
                state->mod_కీలకం_లెక్క[i] = 0;
            }
            state->clear_mods &= ~బిట్టు;
        }
    }

    xkb_state_update_derived(&state->పునాది);

    తిరిగి get_state_component_changes(&prev_components, &state->పునాది.components);
}

/* We need fake keys for `update_latch_modifiers` and `update_latch_group`.
 * These keys must have at least one level in order to break latches. We need 2
 * keys with specific actions in order to update group/mod latches without
 * affecting each other. */
స్థిర నిర్మాణం xkb_కీలకం_type_నమోదు synthetic_కీలకం_level_నమోదు = { 0 };
స్థిర నిర్మాణం xkb_కీలకం_type synthetic_కీలకం_type = {
     .అంకె_entries = 1,
     .అంకె_స్థాయిలు = 1,
     .entries = &synthetic_కీలకం_level_నమోదు
};
స్థిర స్థిరరాశి నిర్మాణం xkb_కీలకం synthetic_కీలకం = { 0 };

/* Transcription from xserver: XkbLatchModifiers */
స్థిర శూన్య
update_latch_modifiers(నిర్మాణం xkb_server_state *state,
                       నిర్మాణం xkb_events *events,
                       xkb_mod_ముసుగు_t ముసుగు, xkb_mod_ముసుగు_t latches)
{
    /* Clear affected latched modifiers */
    స్థిరరాశి xkb_mod_ముసుగు_t clear = ముసుగు & ~latches;
    state->పునాది.components.latched_mods &= ~clear;

    /* Clear any pending latch to locks using ad hoc action:
     * only affect corresponding modifier latches and no group latch. */
    నిర్మాణం xkb_level synthetic_కీలకం_level_break_mod_latch = {
        .అంకె_syms = 0,
        .అంకె_actions = 1,
        .పై = XKB_కీలకం_No_చిహ్నం,
        .s.sym = XKB_కీలకం_No_చిహ్నం,
        .a.action.అంతర్గత = {
            .type = ACTION_TYPE_అంతర్గత,
            .flags = అంతర్గత_BREAKS_MOD_LATCH,
            .clear_latched_mods = clear
        }
    };
    నిర్మాణం xkb_సమూహం synthetic_కీలకం_సమూహం_break_mod_latch = {
        .type = &synthetic_కీలకం_type,
        .స్థాయిలు = &synthetic_కీలకం_level_break_mod_latch
    };
    స్థిరరాశి నిర్మాణం xkb_కీలకం synthetic_కీలకం_break_mod_latch = {
        .అంకె_groups = 1,
        .groups = &synthetic_కీలకం_సమూహం_break_mod_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_కీలకం_break_mod_latch, XKB_కీలకం_కిందికి);

    /* Finally set the latched mods by simulate tapping a key with the
     * corresponding action */
    స్థిరరాశి నిర్మాణం xkb_కీలకం* స్థిరరాశి కీలకం = &synthetic_కీలకం;
    స్థిరరాశి సంఘం xkb_action latch_mods = {
        .mods = {
            .type = ACTION_TYPE_MOD_LATCH,
            .mods = { .ముసుగు = ముసుగు & latches },
            .flags = 0,
        },
    };
    నిర్మాణం xkb_filter* స్థిరరాశి filter = xkb_filter_new(state);
    filter->కీలకం = కీలకం;
    filter->func = xkb_filter_mod_latch_func;
    filter->action = latch_mods;
    xkb_filter_mod_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_mod_latch_func(state, events, filter, కీలకం, XKB_కీలకం_పైకి);
}

/* Transcription from xserver: XkbLatchGroup */
స్థిర శూన్య
update_latch_సమూహం(నిర్మాణం xkb_server_state *state,
                   నిర్మాణం xkb_events *events, int32_t సమూహం)
{
    /* Clear any pending latch to locks. */
    స్థిర నిర్మాణం xkb_level synthetic_కీలకం_level_break_సమూహం_latch = {
        .అంకె_syms = 0,
        .అంకె_actions = 1,
        .పై = XKB_కీలకం_No_చిహ్నం,
        .s.sym = XKB_కీలకం_No_చిహ్నం,
        .a.action.అంతర్గత = {
            .type = ACTION_TYPE_అంతర్గత,
            .flags = అంతర్గత_BREAKS_సమూహం_LATCH,
            .clear_latched_mods = 0
        }
    };
    స్థిర నిర్మాణం xkb_సమూహం synthetic_కీలకం_సమూహం_break_సమూహం_latch = {
        .type = &synthetic_కీలకం_type,
        .స్థాయిలు = &synthetic_కీలకం_level_break_సమూహం_latch
    };
    స్థిర స్థిరరాశి నిర్మాణం xkb_కీలకం synthetic_కీలకం_break_సమూహం_latch = {
        .అంకె_groups = 1,
        .groups = &synthetic_కీలకం_సమూహం_break_సమూహం_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_కీలకం_break_సమూహం_latch, XKB_కీలకం_కిందికి);

    /* Simulate tapping a key with a group latch action, but in isolation: i.e.
     * without affecting the other filters. */
    స్థిరరాశి నిర్మాణం xkb_కీలకం* స్థిరరాశి కీలకం = &synthetic_కీలకం;
    స్థిరరాశి సంఘం xkb_action latch_సమూహం = {
        .సమూహం = {
            .type = ACTION_TYPE_సమూహం_LATCH,
            .flags = ACTION_ABSOLUTE_SWITCH,
            .సమూహం = సమూహం,
        },
    };
    నిర్మాణం xkb_filter* స్థిరరాశి filter = xkb_filter_new(state);
    filter->కీలకం = కీలకం;
    filter->func = xkb_filter_సమూహం_latch_func;
    filter->action = latch_సమూహం;
    xkb_filter_సమూహం_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_సమూహం_latch_func(state, events, filter, కీలకం, XKB_కీలకం_పైకి);
}

/**
 * Compute the resolved effective mask of an arbitrary input.
 *
 * Contrary to `mod_mask_get_effective`, it resolves only modifiers not present
 * in the canonical mask, so that it enables `xkb_state_serialize_mods` to
 * round trip via `xkb_state_update_mask`.
 */
స్థిర అంతఃవరుస xkb_mod_ముసుగు_t
resolve_గమ్యం_canonical_mods(నిర్మాణం xkb_keymap *keymap, xkb_mod_ముసుగు_t mods)
{
    తిరిగి
        /*
         * Keep canonical modifier mask.
         * It contains either real modifiers or canonical virtual modifiers.
         */
        (mods & keymap->canonical_state_ముసుగు) |
        /* Resolve other modifiers */
        mod_ముసుగు_get_effective(keymap,
                               mods & ~keymap->canonical_state_ముసుగు);
}

స్థిర శూన్య
state_update_latched_locked(
    నిర్మాణం xkb_server_state * పరిమిత state,
    స్థిరరాశి నిర్మాణం xkb_state_components_update * పరిమిత update,
    నిర్మాణం xkb_events * పరిమిత events
)
{
    /* Update locks */
    స్థిరరాశి xkb_mod_ముసుగు_t affect_locked_mods =
        resolve_గమ్యం_canonical_mods(state->పునాది.keymap, update->affect_locked_mods);
    ఒకవేళ ((update->components & XKB_STATE_MODS_LOCKED) &&
        affect_locked_mods) {
        స్థిరరాశి xkb_mod_ముసుగు_t locked_mods =
            resolve_గమ్యం_canonical_mods(state->పునాది.keymap, update->locked_mods);
        state->పునాది.components.locked_mods &= ~affect_locked_mods;
        state->పునాది.components.locked_mods |= locked_mods & affect_locked_mods;
    }
    ఒకవేళ (update->components & XKB_STATE_LAYOUT_LOCKED) {
        state->పునాది.components.locked_సమూహం = update->locked_layout;
    }

    /* Update latches */
    స్థిరరాశి xkb_mod_ముసుగు_t affect_latched_mods =
        resolve_గమ్యం_canonical_mods(state->పునాది.keymap, update->affect_latched_mods);
    ఒకవేళ ((update->components & XKB_STATE_MODS_LATCHED) &&
        affect_latched_mods) {
        స్థిరరాశి xkb_mod_ముసుగు_t latched_mods =
            resolve_గమ్యం_canonical_mods(state->పునాది.keymap, update->latched_mods);
        update_latch_modifiers(state, events, affect_latched_mods, latched_mods);
    }
    ఒకవేళ (update->components & XKB_STATE_LAYOUT_LATCHED) {
        update_latch_సమూహం(state, events, update->latched_layout);
    }
}

గణన xkb_state_component
xkb_state_update_latched_locked(నిర్మాణం xkb_state *పునాది_state,
                                xkb_mod_ముసుగు_t affect_latched_mods,
                                xkb_mod_ముసుగు_t latched_mods,
                                bool affect_latched_layout,
                                int32_t latched_layout,
                                xkb_mod_ముసుగు_t affect_locked_mods,
                                xkb_mod_ముసుగు_t locked_mods,
                                bool affect_locked_layout,
                                int32_t locked_layout)
{
    /* Guard against client-only state */
    ప్రతిజ్ఞా(పునాది_state->బహుళకం > SERVER_COMPANION);
    ఒకవేళ (పునాది_state->బహుళకం <= SERVER_COMPANION) {
        log_err(పునాది_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_బహుళకం,
                "%s: Unexpected state type %d\n", __func__, పునాది_state->బహుళకం);
        తిరిగి 0;
    }

    నిర్మాణం xkb_server_state * స్థిరరాశి state =
        (నిర్మాణం xkb_server_state *)పునాది_state;

    స్థిరరాశి నిర్మాణం state_components మునుపటి_components = state->పునాది.components;
    స్థిరరాశి గణన xkb_state_component components
        = ((affect_latched_mods || latched_mods) ? XKB_STATE_MODS_LATCHED : 0)
        | ((affect_locked_mods || locked_mods) ? XKB_STATE_MODS_LOCKED : 0)
        | (affect_latched_layout ? XKB_STATE_LAYOUT_LATCHED : 0)
        | (affect_locked_layout ? XKB_STATE_LAYOUT_LOCKED : 0);
    స్థిరరాశి నిర్మాణం xkb_state_components_update update = {
        .కొలత = పరిమాణం(update),
        .components = components,
        .affect_latched_mods = affect_latched_mods,
        .latched_mods = latched_mods,
        .latched_layout = latched_layout,
        .affect_locked_mods = affect_locked_mods,
        .locked_mods = locked_mods,
        .locked_layout = locked_layout
    };
    state_update_latched_locked(state, &update, ఖాళీ);
    xkb_state_update_derived(&state->పునాది);
    తిరిగి get_state_component_changes(&మునుపటి_components,
                                       &state->పునాది.components);
}

స్థిర అంతఃవరుస శూన్య
clear_all_latches_and_locks(నిర్మాణం xkb_server_state *state,
                            నిర్మాణం xkb_events *events)
{
    స్థిర స్థిరరాశి గణన xkb_state_component components
        = XKB_STATE_MODS_LATCHED | XKB_STATE_MODS_LOCKED
        | XKB_STATE_LAYOUT_LATCHED | XKB_STATE_LAYOUT_LOCKED;
    స్థిరరాశి నిర్మాణం xkb_state_components_update update = {
        .కొలత = పరిమాణం(update),
        .components = components,
        .affect_latched_mods = (xkb_mod_ముసుగు_t)XKB_MOD_ALL,
        .latched_mods = 0,
        .latched_layout = 0,
        .affect_locked_mods = (xkb_mod_ముసుగు_t)XKB_MOD_ALL,
        .locked_mods = 0,
        .locked_layout = 0
    };
    state_update_latched_locked(state, &update, events);
}

స్థిర గణన xkb_స్థితి
state_update_layout_policy(నిర్మాణం xkb_server_state *state,
                           స్థిరరాశి నిర్మాణం xkb_layout_policy_update *update)
{
    ఒకవేళ (xkb_feature_supported(XKB_FEATURE_ENUM_LAYOUT_బహిర్గతం_యొక్క_RANGE_POLICY,
                              update->policy)) {
        ఒకవేళ (update->policy == XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_REDIRECT) {
            ఒకవేళ (update->redirect < state->పునాది.keymap->అంకె_groups) {
                state->పునాది.బహిర్గతం_యొక్క_range_సమూహం.redirect_సమూహం =
                    update->redirect;
            } లేకపోతే {
                log_err(state->పునాది.keymap->ctx,
                        XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ_2,
                        "Layout policy: "
                        "unsupported layout index %"PRIu32" > %"PRIu32"\n",
                        update->redirect + 1, state->పునాది.keymap->అంకె_groups);
                తిరిగి XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ;
            }
        }
        state->పునాది.బహిర్గతం_యొక్క_range_సమూహం.policy = update->policy;
        తిరిగి XKB_SUCCESS;
    } లేకపోతే {
        log_err(state->పునాది.keymap->ctx,
                XKB_ERROR_UNSUPPORTED_LAYOUT_బహిర్గతం_యొక్క_RANGE_POLICY_2,
                "Unsupported layout policy: %d\n", update->policy);
        తిరిగి XKB_ERROR_UNSUPPORTED_LAYOUT_బహిర్గతం_యొక్క_RANGE_POLICY;
    }
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
స్థిర గణన xkb_స్థితి
check_state_update_abi_(నిర్మాణం xkb_context * పరిమిత ctx,
                        స్థిరరాశి అక్షరం * పరిమిత func,
                        స్థిరరాశి నిర్మాణం xkb_synthetic_update * పరిమిత update)
{
    గణన xkb_స్థితి స్థితి_2 = XKB_SUCCESS;
    ఒకవేళ ((స్థితి_2 = xkb_check_state_abi(update)) ||
        (update->reserved0 != 0 &&
         (స్థితి_2 = XKB_ERROR_ABI_FORWARD_COMPAT)) ||
        (update->components &&
         (స్థితి_2 = xkb_check_state_abi(update->components))) ||
        (update->layout_policy &&
         (స్థితి_2 = xkb_check_state_abi(update->layout_policy)))) {
        xkb_log_abi_error(ctx, func, స్థితి_2);
    }
    తిరిగి స్థితి_2;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

#నిర్వచించు check_state_update_abi(ctx, update) \
    check_state_update_abi_(ctx, __func__, update)

గణన xkb_స్థితి
xkb_state_update_synthetic(నిర్మాణం xkb_state * పునాది_state,
                           స్థిరరాశి నిర్మాణం xkb_synthetic_update * update,
                           గణన xkb_state_component *మారింది)
{
    /* Guard against client-only state */
    ఒకవేళ (పునాది_state->బహుళకం <= SERVER_COMPANION) {
        log_err(పునాది_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_బహుళకం,
                "%s: Unexpected state type %d\n", __func__, పునాది_state->బహుళకం);
        తిరిగి XKB_ERROR_UNEXPECTED_STATE_బహుళకం;
    }

    నిర్మాణం xkb_server_state * స్థిరరాశి state =
        (నిర్మాణం xkb_server_state *)పునాది_state;

    /* Check ABI compatibility */
    గణన xkb_స్థితి స్థితి_2 = check_state_update_abi(state->పునాది.keymap->ctx,
                                                    update);
    ఒకవేళ (స్థితి_2 != XKB_SUCCESS)
        తిరిగి స్థితి_2;

    స్థిరరాశి నిర్మాణం state_components మునుపటి_components = state->పునాది.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    /* Update parametrized controls first */
    ఒకవేళ (update->layout_policy) {
        స్థితి_2 = state_update_layout_policy(state, update->layout_policy);
        ఒకవేళ (స్థితి_2 != XKB_SUCCESS)
            తిరిగి స్థితి_2;
    }

    ఒకవేళ (update->components) {
        స్థిరరాశి నిర్మాణం xkb_state_components_update * స్థిరరాశి components =
            update->components;
        /* Update boolean controls first */
        ఒకవేళ (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, ఖాళీ);
        }

        state_update_latched_locked(state, components, ఖాళీ);
    }

    xkb_state_update_derived(&state->పునాది);

    ఒకవేళ (మారింది) {
        *మారింది = get_state_component_changes(&మునుపటి_components,
                                               &state->పునాది.components);
    }
    తిరిగి XKB_SUCCESS;
}

/**
 * Updates the state from a set of explicit masks as gained from
 * xkb_state_serialize_mods and xkb_state_serialize_groups.  As noted in the
 * documentation for these functions in xkbcommon.h, this round-trip is
 * lossy, and should only be used to update a slave state mirroring the
 * master, e.g. in a client/server window system.
 */
గణన xkb_state_component
xkb_state_update_ముసుగు(నిర్మాణం xkb_state *పునాది_state,
                      xkb_mod_ముసుగు_t పునాది_mods,
                      xkb_mod_ముసుగు_t latched_mods,
                      xkb_mod_ముసుగు_t locked_mods,
                      xkb_layout_సూచీ_t పునాది_సమూహం,
                      xkb_layout_సూచీ_t latched_సమూహం,
                      xkb_layout_సూచీ_t locked_సమూహం)
{
    /* Guard against server-only and server companion state */
    ప్రతిజ్ఞా(పునాది_state->బహుళకం == CLIENT_STATE ||
           పునాది_state->బహుళకం == LEGACY_MIXED_STATE);
    ఒకవేళ (పునాది_state->బహుళకం != CLIENT_STATE &&
        పునాది_state->బహుళకం != LEGACY_MIXED_STATE) {
        log_err(పునాది_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_బహుళకం,
                "%s: Unexpected state type %d\n", __func__, పునాది_state->బహుళకం);
        తిరిగి 0;
    }

    నిర్మాణం xkb_client_state * స్థిరరాశి state =
        (నిర్మాణం xkb_client_state *)పునాది_state;

    స్థిరరాశి నిర్మాణం state_components prev_components = state->పునాది.components;

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
    state->పునాది.components.పునాది_mods =
        resolve_గమ్యం_canonical_mods(state->పునాది.keymap, పునాది_mods);
    state->పునాది.components.latched_mods =
        resolve_గమ్యం_canonical_mods(state->పునాది.keymap, latched_mods);
    state->పునాది.components.locked_mods =
        resolve_గమ్యం_canonical_mods(state->పునాది.keymap, locked_mods);

    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->పునాది.components.పునాది_సమూహం = (int32_t) పునాది_సమూహం;
    state->పునాది.components.latched_సమూహం = (int32_t) latched_సమూహం;
    state->పునాది.components.locked_సమూహం = (int32_t) locked_సమూహం;

    xkb_state_update_derived(&state->పునాది);

    తిరిగి get_state_component_changes(&prev_components, &state->పునాది.components);
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Lock_Modifier
 */
స్థిర bool
should_do_caps_transformation(నిర్మాణం xkb_state *state, xkb_keycode_t kc)
{
    తిరిగి
        xkb_state_mod_సూచీ_is_చురుకైన(state, XKB_MOD_సూచీ_CAPS,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_సూచీ_is_consumed(state, kc, XKB_MOD_సూచీ_CAPS) == 0;
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Control_Modifier
 */
స్థిర bool
should_do_ctrl_transformation(నిర్మాణం xkb_state *state, xkb_keycode_t kc)
{
    తిరిగి
        xkb_state_mod_సూచీ_is_చురుకైన(state, XKB_MOD_సూచీ_CTRL,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_సూచీ_is_consumed(state, kc, XKB_MOD_సూచీ_CTRL) == 0;
}

/**
 * Provides the symbols to use for the given key and state.  Returns the
 * number of symbols pointed to in syms_out.
 */
పూర్ణాంకం
xkb_state_కీలకం_get_syms(నిర్మాణం xkb_state *state, xkb_keycode_t kc,
                       స్థిరరాశి xkb_keysym_t **syms_బహిర్గతం)
{
    స్థిరరాశి xkb_layout_సూచీ_t layout = xkb_state_కీలకం_get_layout(state, kc);
    ఒకవేళ (layout == XKB_LAYOUT_INVALID)
        వెళ్ళు err;

    స్థిరరాశి xkb_level_సూచీ_t level = xkb_state_కీలకం_get_level(state, kc, layout);
    ఒకవేళ (level == XKB_LEVEL_INVALID)
        వెళ్ళు err;

    స్థిరరాశి నిర్మాణం xkb_కీలకం* స్థిరరాశి కీలకం = Xkb_కీలకం(state->keymap, kc);

    ఒకవేళ (!కీలకం)
        వెళ్ళు err;

    స్థిరరాశి నిర్మాణం xkb_level* స్థిరరాశి leveli =
        xkb_keymap_కీలకం_get_level(state->keymap, కీలకం, layout, level);

    ఒకవేళ (!leveli)
        వెళ్ళు err;

    స్థిరరాశి xkb_keysym_లెక్క_t అంకె_syms = leveli->అంకె_syms;
    ఒకవేళ (అంకె_syms == 0)
        వెళ్ళు err;

    ఒకవేళ (should_do_caps_transformation(state, kc)) {
        /* Only simple capitalization rules: keysyms count is unchanged. */
        ఒకవేళ (అంకె_syms > 1) {
            *syms_బహిర్గతం = (leveli->has_పై)
                      ? leveli->s.syms + అంకె_syms
                      : leveli->s.syms;
        } లేకపోతే {
            *syms_బహిర్గతం = &leveli->పై;
        }
    } లేకపోతే {
        *syms_బహిర్గతం = (అంకె_syms > 1)
                  ? leveli->s.syms
                  : &leveli->s.sym;
    }
    తిరిగి (పూర్ణాంకం) అంకె_syms;

err:
    *syms_బహిర్గతం = ఖాళీ;
    తిరిగి 0;
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
స్థిర అక్షరం
Xkb_గమ్యం_Control(అక్షరం ch)
{
    అక్షరం c = ch;

    ఒకవేళ ((c >= '@' && c < '\177') || c == ' ')
        c &= 0x1F;
    లేకపోతే ఒకవేళ (c == '2')
        c = '\000';
    లేకపోతే ఒకవేళ (c >= '3' && c <= '7')
        c -= ('3' - '\033');
    లేకపోతే ఒకవేళ (c == '8')
        c = '\177';
    లేకపోతే ఒకవేళ (c == '/')
        c = '_' & 0x1F;
    తిరిగి c;
}

/**
 * Provides either exactly one symbol, or XKB_KEY_NoSymbol.
 */
xkb_keysym_t
xkb_state_కీలకం_get_one_sym(నిర్మాణం xkb_state *state, xkb_keycode_t kc)
{
    స్థిరరాశి xkb_keysym_t *syms = ఖాళీ;
    స్థిరరాశి పూర్ణాంకం అంకె_syms = xkb_state_కీలకం_get_syms(state, kc, &syms);

    ఒకవేళ (అంకె_syms != 1)
        తిరిగి XKB_కీలకం_No_చిహ్నం;
    లేకపోతే
        తిరిగి syms[0];
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
స్థిర xkb_keysym_t
get_one_sym_for_string(నిర్మాణం xkb_state *state, xkb_keycode_t kc)
{
    స్థిరరాశి xkb_layout_సూచీ_t layout = xkb_state_కీలకం_get_layout(state, kc);
    స్థిరరాశి xkb_layout_సూచీ_t అంకె_layouts =
        xkb_keymap_అంకె_layouts_for_కీలకం(state->keymap, kc);
    xkb_level_సూచీ_t level = xkb_state_కీలకం_get_level(state, kc, layout);
    ఒకవేళ (layout == XKB_LAYOUT_INVALID || అంకె_layouts == 0 ||
        level == XKB_LEVEL_INVALID)
        తిరిగి XKB_కీలకం_No_చిహ్నం;

    స్థిరరాశి xkb_keysym_t *syms = ఖాళీ;
    పూర్ణాంకం nsyms = xkb_keymap_కీలకం_get_syms_by_level(state->keymap, kc,
                                                 layout, level, &syms);
    ఒకవేళ (nsyms != 1)
        తిరిగి XKB_కీలకం_No_చిహ్నం;
    xkb_keysym_t sym = syms[0];

    ఒకవేళ (should_do_ctrl_transformation(state, kc) && sym > 127u) {
        కొరకు (xkb_layout_సూచీ_t i = 0; i < అంకె_layouts; i++) {
            level = xkb_state_కీలకం_get_level(state, kc, i);
            ఒకవేళ (level == XKB_LEVEL_INVALID)
                కొనసాగు;

            nsyms = xkb_keymap_కీలకం_get_syms_by_level(state->keymap, kc,
                                                     i, level, &syms);
            ఒకవేళ (nsyms == 1 && syms[0] <= 127u) {
                sym = syms[0];
                ఆపు;
            }
        }
    }

    ఒకవేళ (should_do_caps_transformation(state, kc)) {
        sym = xkb_keysym_గమ్యం_పై(sym);
    }

    తిరిగి sym;
}

పూర్ణాంకం
xkb_state_కీలకం_get_utf8(నిర్మాణం xkb_state *state, xkb_keycode_t kc,
                       అక్షరం *బఫరు, కొలత_t కొలత)
{
    పూర్ణాంకం nsyms;
    స్థిరరాశి xkb_keysym_t *syms = ఖాళీ;
    స్థిరరాశి xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    ఒకవేళ (sym != XKB_కీలకం_No_చిహ్నం) {
        nsyms = 1; syms = &sym;
    }
    లేకపోతే {
        nsyms = xkb_state_కీలకం_get_syms(state, kc, &syms);
    }

    /* Make sure not to truncate in the middle of a UTF-8 sequence. */
    పూర్ణాంకం స్థానభ్రంశం = 0;
    అక్షరం tmp[XKB_KEYSYM_UTF8_MAX_కొలత];
    కొరకు (పూర్ణాంకం i = 0; i < nsyms; i++) {
        పూర్ణాంకం ret = xkb_keysym_గమ్యం_utf8(syms[i], tmp, పరిమాణం(tmp));
        ఒకవేళ (ret <= 0)
            వెళ్ళు err_bad;

        ret--;
        ఒకవేళ ((కొలత_t)స్థానభ్రంశం + (కొలత_t)ret <= కొలత)
            జ్ఞాపకం_నకలు(బఫరు + స్థానభ్రంశం, tmp, (కొలత_t)ret);
        స్థానభ్రంశం += ret;
    }

    ఒకవేళ ((కొలత_t)స్థానభ్రంశం >= కొలత)
        వెళ్ళు err_trunc;
    బఫరు[స్థానభ్రంశం] = '\0';

    ఒకవేళ (!is_valid_utf8(బఫరు, (కొలత_t)స్థానభ్రంశం))
        వెళ్ళు err_bad;

    ఒకవేళ (స్థానభ్రంశం == 1 && (గుర్తులేని పూర్ణాంకం) బఫరు[0] <= 127u &&
        should_do_ctrl_transformation(state, kc))
        బఫరు[0] = Xkb_గమ్యం_Control(బఫరు[0]);

    తిరిగి స్థానభ్రంశం;

err_trunc:
    ఒకవేళ (కొలత > 0)
        బఫరు[కొలత - 1] = '\0';
    తిరిగి స్థానభ్రంశం;

err_bad:
    ఒకవేళ (కొలత > 0)
        బఫరు[0] = '\0';
    తిరిగి 0;
}

uint32_t
xkb_state_కీలకం_get_utf32(నిర్మాణం xkb_state *state, xkb_keycode_t kc)
{
    స్థిరరాశి xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    uint32_t cp = xkb_keysym_గమ్యం_utf32(sym);

    ఒకవేళ (cp <= 127u && should_do_ctrl_transformation(state, kc))
        cp = (uint32_t) Xkb_గమ్యం_Control((అక్షరం) cp);

    తిరిగి cp;
}

/**
 * Serializes the requested modifier state into an xkb_mod_mask_t, with all
 * the same disclaimers as in xkb_state_update_mask.
 */
స్థిర అంతఃవరుస xkb_mod_ముసుగు_t
serialize_mods(స్థిరరాశి నిర్మాణం state_components *components,
               గణన xkb_state_component type)
{
    xkb_mod_ముసుగు_t ret = 0;

    ఒకవేళ (type & XKB_STATE_MODS_EFFECTIVE)
        తిరిగి components->mods;

    ఒకవేళ (type & XKB_STATE_MODS_DEPRESSED)
        ret |= components->పునాది_mods;
    ఒకవేళ (type & XKB_STATE_MODS_LATCHED)
        ret |= components->latched_mods;
    ఒకవేళ (type & XKB_STATE_MODS_LOCKED)
        ret |= components->locked_mods;

    తిరిగి ret;
}

xkb_mod_ముసుగు_t
xkb_state_serialize_mods(నిర్మాణం xkb_state *state,
                         గణన xkb_state_component type)
{
    తిరిగి serialize_mods(&state->components, type);
}

/**
 * Serialises the requested group state, with all the same disclaimers as
 * in xkb_state_update_mask.
 */
స్థిర అంతఃవరుస xkb_layout_సూచీ_t
serialize_layout(స్థిరరాశి నిర్మాణం state_components *components,
                 గణన xkb_state_component type)
{
    ఒకవేళ (type & XKB_STATE_LAYOUT_EFFECTIVE)
        తిరిగి components->సమూహం;

    int32_t ret = 0;
    ఒకవేళ (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret += components->పునాది_సమూహం;
    ఒకవేళ (type & XKB_STATE_LAYOUT_LATCHED)
        ret += components->latched_సమూహం;
    ఒకవేళ (type & XKB_STATE_LAYOUT_LOCKED)
        ret += components->locked_సమూహం;

    తిరిగి (xkb_layout_సూచీ_t)ret;
}

xkb_layout_సూచీ_t
xkb_state_serialize_layout(నిర్మాణం xkb_state *state,
                           గణన xkb_state_component type)
{
    తిరిగి serialize_layout(&state->components, type);
}

xkb_led_ముసుగు_t
xkb_state_serialize_leds(స్థిరరాశి నిర్మాణం xkb_state *state,
                         గణన xkb_state_component type)
{
    తిరిగి (type & XKB_STATE_LEDS)
        ? state->components.leds
        : 0;
}

స్థిర అంతఃవరుస గణన xkb_keyboard_control_flags
serialize_controls(స్థిరరాశి నిర్మాణం state_components *components,
                   గణన xkb_state_component type)
{
    తిరిగి (type & XKB_STATE_CONTROLS_EFFECTIVE)
        /*
         * Enable using the public API with the all the Controls values, except
         * the internal ones, if any.
         */
        ? (గణన xkb_keyboard_control_flags)
          (components->controls & CONTROL_ALL_BOOLEAN)
        : 0;
}

గణన xkb_keyboard_control_flags
xkb_state_serialize_controls(స్థిరరాశి నిర్మాణం xkb_state *state,
                             గణన xkb_state_component type)
{
    తిరిగి serialize_controls(&state->components, type);
}

/**
 * Gets a modifier mask and returns the resolved effective mask; this
 * is needed because some modifiers can also map to other modifiers, e.g.
 * the "NumLock" modifier usually also sets the "Mod2" modifier.
 */
xkb_mod_ముసుగు_t
mod_ముసుగు_get_effective(నిర్మాణం xkb_keymap *keymap, xkb_mod_ముసుగు_t mods)
{
    /* Initialize the effective mask with its corresponding real mods. */
    xkb_mod_ముసుగు_t ముసుగు = mods & MOD_REAL_ముసుగు_ALL;

    /* Resolve the virtual modifiers */
    స్థిరరాశి నిర్మాణం xkb_mod *mod;
    xkb_mod_సూచీ_t i;
    xkb_vmods_enumerate(i, mod, &keymap->mods)
        ఒకవేళ (mods & (UINT32_C(1) << i))
            ముసుగు |= mod->mapping;

    తిరిగి ముసుగు;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
పూర్ణాంకం
xkb_state_mod_సూచీ_is_చురుకైన(నిర్మాణం xkb_state *state,
                              xkb_mod_సూచీ_t idx,
                              గణన xkb_state_component type)
{
    ఒకవేళ (unlikely(idx >= xkb_keymap_అంకె_mods(state->keymap)))
        తిరిగి -1;

    స్థిరరాశి xkb_mod_ముసుగు_t mapping = state->keymap->mods.mods[idx].mapping;
    ఒకవేళ (!mapping) {
        /* Modifier not mapped */
        తిరిగి 0;
    }
    /* WARNING: this may overmatch for virtual modifiers */
    తిరిగి (xkb_state_serialize_mods(state, type) & mapping) == mapping;
}

గణన {
    XKB_STATE_సరిపోలిక_FLAGS = XKB_STATE_సరిపోలిక_ANY
                          | XKB_STATE_సరిపోలిక_ALL
                          | XKB_STATE_సరిపోలిక_NON_EXCLUSIVE
};

/**
 * Helper function for xkb_state_mod_indices_are_active and
 * xkb_state_mod_names_are_active.
 */
స్థిర bool
సరిపోలిక_mod_masks(నిర్మాణం xkb_state *state,
                గణన xkb_state_component type,
                గణన xkb_state_సరిపోలిక సరిపోలిక,
                xkb_mod_ముసుగు_t wanted)
{
    స్థిరరాశి xkb_mod_ముసుగు_t చురుకైన = xkb_state_serialize_mods(state, type);

    ఒకవేళ (!(సరిపోలిక & XKB_STATE_సరిపోలిక_NON_EXCLUSIVE) && (చురుకైన & ~wanted))
        తిరిగి false;

    ఒకవేళ (సరిపోలిక & XKB_STATE_సరిపోలిక_ANY)
        తిరిగి చురుకైన & wanted;

    /* Ensure all flags but XKB_STATE_MATCH_ALL are processed before */
    static_assert(XKB_STATE_సరిపోలిక_ALL ==
                  ((గణన xkb_state_సరిపోలిక) XKB_STATE_సరిపోలిక_FLAGS &
                   ~(XKB_STATE_సరిపోలిక_ANY | XKB_STATE_సరిపోలిక_NON_EXCLUSIVE)),
                  "");

    తిరిగి (చురుకైన & wanted) == wanted;
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, or -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
పూర్ణాంకం
xkb_state_mod_indices_are_చురుకైన(నిర్మాణం xkb_state *state,
                                 గణన xkb_state_component type,
                                 గణన xkb_state_సరిపోలిక సరిపోలిక,
                                 ...)
{
    ఒకవేళ (సరిపోలిక & ~((గణన xkb_state_సరిపోలిక) XKB_STATE_సరిపోలిక_FLAGS))
        తిరిగి -2;

    va_list ap;
    xkb_mod_ముసుగు_t wanted = 0;
    పూర్ణాంకం ret = 0;
    స్థిరరాశి xkb_mod_సూచీ_t అంకె_mods = xkb_keymap_అంకె_mods(state->keymap);

    va_ప్రారంభం(ap, సరిపోలిక);
    అయితే (1) {
        xkb_mod_సూచీ_t idx = va_arg(ap, xkb_mod_సూచీ_t);
        ఒకవేళ (idx == XKB_MOD_INVALID)
            ఆపు;
        ఒకవేళ (unlikely(idx >= అంకె_mods)) {
            ret = -1;
            ఆపు;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_చివర(ap);

    ఒకవేళ (ret == -1)
        తిరిగి ret;

    ఒకవేళ (!wanted) {
        /* Modifiers not mapped */
        తిరిగి 0;
    }

    తిరిగి సరిపోలిక_mod_masks(state, type, సరిపోలిక, wanted);
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
పూర్ణాంకం
xkb_state_mod_name_is_చురుకైన(నిర్మాణం xkb_state *state, స్థిరరాశి అక్షరం *name,
                             గణన xkb_state_component type)
{
    స్థిరరాశి xkb_mod_సూచీ_t idx = xkb_keymap_mod_get_సూచీ(state->keymap, name);

    ఒకవేళ (idx == XKB_MOD_INVALID)
        తిరిగి -1;

    తిరిగి xkb_state_mod_సూచీ_is_చురుకైన(state, idx, type);
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ATTR_NULL_SENTINEL పూర్ణాంకం
xkb_state_mod_పేర్లు_are_చురుకైన(నిర్మాణం xkb_state *state,
                               గణన xkb_state_component type,
                               గణన xkb_state_సరిపోలిక సరిపోలిక,
                               ...)
{
    ఒకవేళ (సరిపోలిక & ~((గణన xkb_state_సరిపోలిక) XKB_STATE_సరిపోలిక_FLAGS))
        తిరిగి -2;

    va_list ap;
    xkb_mod_ముసుగు_t wanted = 0;
    పూర్ణాంకం ret = 0;

    va_ప్రారంభం(ap, సరిపోలిక);
    అయితే (1) {
        స్థిరరాశి అక్షరం *str = va_arg(ap, స్థిరరాశి అక్షరం *);
        ఒకవేళ (str == ఖాళీ)
            ఆపు;
        స్థిరరాశి xkb_mod_సూచీ_t idx = xkb_keymap_mod_get_సూచీ(state->keymap, str);
        ఒకవేళ (idx == XKB_MOD_INVALID) {
            ret = -1;
            ఆపు;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_చివర(ap);

    ఒకవేళ (ret == -1)
        తిరిగి ret;

    ఒకవేళ (!wanted) {
        /* Modifiers not mapped */
        తిరిగి 0;
    }

    తిరిగి సరిపోలిక_mod_masks(state, type, సరిపోలిక, wanted);
}

/**
 * Returns 1 if the given group is active with the specified type(s), 0 if
 * not, or -1 if the group is invalid.
 */
పూర్ణాంకం
xkb_state_layout_సూచీ_is_చురుకైన(నిర్మాణం xkb_state *state,
                                 xkb_layout_సూచీ_t idx,
                                 గణన xkb_state_component type)
{
    ఒకవేళ (idx >= state->keymap->అంకె_groups)
        తిరిగి -1;

    పూర్ణాంకం ret = 0;
    ఒకవేళ (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ret |= (state->components.సమూహం == idx);
    ఒకవేళ (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret |= (state->components.పునాది_సమూహం == (int32_t) idx);
    ఒకవేళ (type & XKB_STATE_LAYOUT_LATCHED)
        ret |= (state->components.latched_సమూహం == (int32_t) idx);
    ఒకవేళ (type & XKB_STATE_LAYOUT_LOCKED)
        ret |= (state->components.locked_సమూహం == (int32_t) idx);

    తిరిగి ret;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
పూర్ణాంకం
xkb_state_layout_name_is_చురుకైన(నిర్మాణం xkb_state *state, స్థిరరాశి అక్షరం *name,
                                గణన xkb_state_component type)
{
    స్థిరరాశి xkb_layout_సూచీ_t idx =
        xkb_keymap_layout_get_సూచీ(state->keymap, name);

    ఒకవేళ (idx == XKB_LAYOUT_INVALID)
        తిరిగి -1;

    తిరిగి xkb_state_layout_సూచీ_is_చురుకైన(state, idx, type);
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
పూర్ణాంకం
xkb_state_led_సూచీ_is_చురుకైన(నిర్మాణం xkb_state *state, xkb_led_సూచీ_t idx)
{
    ఒకవేళ (idx >= state->keymap->అంకె_leds ||
        state->keymap->leds[idx].name == XKB_ATOM_NONE)
        తిరిగి -1;

    తిరిగి !!(state->components.leds & (UINT32_C(1) << idx));
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
పూర్ణాంకం
xkb_state_led_name_is_చురుకైన(నిర్మాణం xkb_state *state, స్థిరరాశి అక్షరం *name)
{
    స్థిరరాశి xkb_led_సూచీ_t idx = xkb_keymap_led_get_సూచీ(state->keymap, name);

    ఒకవేళ (idx == XKB_LED_INVALID)
        తిరిగి -1;

    తిరిగి xkb_state_led_సూచీ_is_చురుకైన(state, idx);
}

/**
 * See:
 * - XkbTranslateKeyCode(3), mod_rtrn return value, from libX11.
 * - MyEnhancedXkbTranslateKeyCode(), a modification of the above, from GTK+.
 */
స్థిర xkb_mod_ముసుగు_t
కీలకం_get_consumed(నిర్మాణం xkb_state *state, స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                 గణన xkb_consumed_బహుళకం బహుళకం)
{
    స్థిరరాశి xkb_layout_సూచీ_t సమూహం =
        xkb_state_కీలకం_get_layout(state, కీలకం->keycode);
    ఒకవేళ (సమూహం == XKB_LAYOUT_INVALID)
        తిరిగి 0;

    xkb_mod_ముసుగు_t preserve = 0;
    xkb_mod_ముసుగు_t consumed = 0;

    స్థిరరాశి నిర్మాణం xkb_కీలకం_type_నమోదు* స్థిరరాశి matching_నమోదు =
        get_నమోదు_for_కీలకం_state(state, కీలకం, సమూహం);
    ఒకవేళ (matching_నమోదు)
        preserve = matching_నమోదు->preserve.ముసుగు;

    స్థిరరాశి నిర్మాణం xkb_కీలకం_type* స్థిరరాశి type = కీలకం->groups[సమూహం].type;
    ఎంపిక (బహుళకం) {
    స్థితి XKB_CONSUMED_బహుళకం_XKB:
        consumed = type->mods.ముసుగు;
        ఆపు;

    స్థితి XKB_CONSUMED_బహుళకం_GTK: {
        స్థిరరాశి నిర్మాణం xkb_కీలకం_type_నమోదు* స్థిరరాశి no_mods_నమోదు =
            get_నమోదు_for_mods(type, 0);
        స్థిరరాశి xkb_level_సూచీ_t no_mods_leveli = no_mods_నమోదు
                                               ? no_mods_నమోదు->level
                                               : 0;
        స్థిరరాశి నిర్మాణం xkb_level* స్థిరరాశి no_mods_level =
            &కీలకం->groups[సమూహం].స్థాయిలు[no_mods_leveli];

        కొరకు (darray_కొలత_t i = 0; i < type->అంకె_entries; i++) {
            స్థిరరాశి నిర్మాణం xkb_కీలకం_type_నమోదు* స్థిరరాశి నమోదు_2 = &type->entries[i];
            ఒకవేళ (!నమోదు_is_చురుకైన(నమోదు_2))
                కొనసాగు;

            స్థిరరాశి నిర్మాణం xkb_level* స్థిరరాశి level =
                &కీలకం->groups[సమూహం].స్థాయిలు[నమోదు_2->level];
            ఒకవేళ (Xkb_స్థాయిలు_Same_Syms(level, no_mods_level))
                కొనసాగు;

            ఒకవేళ (నమోదు_2 == matching_నమోదు || one_బిట్టు_సమితి(నమోదు_2->mods.ముసుగు))
                consumed |= నమోదు_2->mods.ముసుగు & ~నమోదు_2->preserve.ముసుగు;
        }
        ఆపు;
    }
    }

    తిరిగి consumed & ~preserve;
}

పూర్ణాంకం
xkb_state_mod_సూచీ_is_consumed2(నిర్మాణం xkb_state *state, xkb_keycode_t kc,
                                 xkb_mod_సూచీ_t idx,
                                 గణన xkb_consumed_బహుళకం బహుళకం)
{
    స్థిరరాశి నిర్మాణం xkb_కీలకం* స్థిరరాశి కీలకం = Xkb_కీలకం(state->keymap, kc);

    ఒకవేళ (unlikely(!కీలకం || idx >= xkb_keymap_అంకె_mods(state->keymap)))
        తిరిగి -1;

    స్థిరరాశి xkb_mod_ముసుగు_t mapping = state->keymap->mods.mods[idx].mapping;
    ఒకవేళ (!mapping) {
        /* Modifier not mapped */
        తిరిగి 0;
    }
    తిరిగి (mapping & కీలకం_get_consumed(state, కీలకం, బహుళకం)) == mapping;
}

పూర్ణాంకం
xkb_state_mod_సూచీ_is_consumed(నిర్మాణం xkb_state *state, xkb_keycode_t kc,
                                xkb_mod_సూచీ_t idx)
{
    తిరిగి xkb_state_mod_సూచీ_is_consumed2(state, kc, idx,
                                            XKB_CONSUMED_బహుళకం_XKB);
}

xkb_mod_ముసుగు_t
xkb_state_mod_ముసుగు_remove_consumed(నిర్మాణం xkb_state *state, xkb_keycode_t kc,
                                   xkb_mod_ముసుగు_t ముసుగు)
{
    స్థిరరాశి నిర్మాణం xkb_కీలకం* స్థిరరాశి కీలకం = Xkb_కీలకం(state->keymap, kc);

    ఒకవేళ (!కీలకం)
        తిరిగి 0;

    తిరిగి resolve_గమ్యం_canonical_mods(state->keymap, ముసుగు) &
           ~కీలకం_get_consumed(state, కీలకం, XKB_CONSUMED_బహుళకం_XKB);
}

xkb_mod_ముసుగు_t
xkb_state_కీలకం_get_consumed_mods2(నిర్మాణం xkb_state *state, xkb_keycode_t kc,
                                 గణన xkb_consumed_బహుళకం బహుళకం)
{
    ఎంపిక (బహుళకం) {
    స్థితి XKB_CONSUMED_బహుళకం_XKB:
    స్థితి XKB_CONSUMED_బహుళకం_GTK:
        ఆపు;
    అప్రమేయ:
        log_err_func(state->keymap->ctx, XKB_LOG_సందేశం_NO_ID,
                     "unrecognized consumed modifiers mode: %d\n", బహుళకం);
        తిరిగి 0;
    }

    స్థిరరాశి నిర్మాణం xkb_కీలకం* స్థిరరాశి కీలకం = Xkb_కీలకం(state->keymap, kc);
    ఒకవేళ (!కీలకం)
        తిరిగి 0;

    తిరిగి కీలకం_get_consumed(state, కీలకం, బహుళకం);
}

xkb_mod_ముసుగు_t
xkb_state_కీలకం_get_consumed_mods(నిర్మాణం xkb_state *state, xkb_keycode_t kc)
{
    తిరిగి xkb_state_కీలకం_get_consumed_mods2(state, kc, XKB_CONSUMED_బహుళకం_XKB);
}

/*******************************************************************************
 * Server state machine API
 ******************************************************************************/

/** Entry to keep track of the state of an overlaid key */
నిర్మాణం xkb_overlaid_కీలకం {
    /** The source key being overlaid */
    స్థిరరాశి నిర్మాణం xkb_కీలకం *old;
    /** The target key */
    స్థిరరాశి నిర్మాణం xkb_కీలకం *new;
    /** Counter of key currently down */
    పూర్ణాంకం refcnt;
};

నిర్మాణం xkb_shortcuts_config_నమోదు {
    /** Real modifier mask to trigger shortcuts tweaks */
    xkb_mod_ముసుగు_t mods;
    /** Target layout */
    xkb_layout_సూచీ_t లక్ష్యం;
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
నిర్మాణం xkb_machine {
    /** Inherits from `xkb_server_state` */
    నిర్మాణం xkb_server_state పునాది;

    /* Extra server-specific state data */

    /** Keyboard overlays handling */
    నిర్మాణం {
        /**
         * Current overlaid keys
         *
         * When a key is overlaid, it remained overlaid in this configuration
         * until totally released, i.e. every key press was matched with the
         * corresponding key release.
         */
        darray(నిర్మాణం xkb_overlaid_కీలకం) కీలకాలు;
        /**
         * Activation order of the overlay
         *
         * Overlays indices are stored 1-indexed in nibbles: the lowest
         * nibble corresponds to the latest activated index.
         */
        uint32_t క్రమం;
        /** Current enabled overlays mask */
        xkb_overlay_ముసుగు_t enabled;
    } overlays;

    /** Configuration */
    నిర్మాణం machine_config {
        /** Modifiers remapping */
        నిర్మాణం machine_modifiers_config {
            /** Real modifier mask */
            xkb_mod_ముసుగు_t ముసుగు;
            /** Modifiers re-mappings */
            darray_కొలత_t mappings_అంకె;
            నిర్మాణం machine_mods_mapping {
                xkb_mod_ముసుగు_t ఆధారం;
                xkb_mod_ముసుగు_t లక్ష్యం;
            } * mappings ATTR_COUNTED_BY(mappings_అంకె);
        } modifiers;

        /** Shortcuts tweak */
        నిర్మాణం machine_shortcuts_config {
            /** Substitution entries */
            నిర్మాణం xkb_shortcuts_config_నమోదు *entries;
        } shortcuts;
    } config;

    /** Mouse keys handling */
    నిర్మాణం {
        /** Default pointer button */
        xkb_సూచిక_button_సూచీ_t default_button;
        /** Mask of the locked buttons */
        xkb_సూచిక_button_ముసుగు_t locked_buttons;
    } mouse;

    గణన xkb_machine_flags flags;
};

రకనిర్వచనం darray(నిర్మాణం machine_mods_mapping) machine_mods_mappings;

నిర్మాణం xkb_machine_builder {
    నిర్మాణం xkb_keymap *keymap;

    /** Controls */
    నిర్మాణం {
        /** Accessibility flags */
        నిర్మాణం {
            గణన xkb_a11y_flags affect_flags;
            గణన xkb_a11y_flags flags;
        } a11y;
    } controls;

    /** Modifiers re-mapping */
    machine_mods_mappings mods;

    /** Shortcuts tweak */
    నిర్మాణం xkb_shortcuts_config_options {
        /** Substitution entries */
        darray(నిర్మాణం xkb_shortcuts_config_నమోదు) entries;
    } shortcuts;

    గణన xkb_machine_flags machine_flags;
    గణన xkb_machine_builder_flags builder_flags;
    పూర్ణాంకం refcnt;
};

నిర్మాణం xkb_machine_builder *
xkb_machine_builder_new(
    నిర్మాణం xkb_keymap * పరిమిత keymap,
    స్థిరరాశి నిర్మాణం xkb_machine_builder_config * పరిమిత config,
    గణన xkb_స్థితి * పరిమిత స్థితి_2
)
{
    /* Handle default configuration */
    స్థిర స్థిరరాశి నిర్మాణం xkb_machine_builder_config default_config = {
        .కొలత = పరిమాణం(default_config)
    };
    ఒకవేళ (!config)
        config = &default_config;

    /* Check ABI compatibility */
    స్థిరరాశి గణన xkb_స్థితి స్థితి_3 = xkb_check_state_abi(config);
    ఒకవేళ (స్థితి_3) {
        xkb_log_abi_error(keymap->ctx, __func__, స్థితి_3);
        ఒకవేళ (స్థితి_2)
            *స్థితి_2 = స్థితి_3;
        తిరిగి ఖాళీ;
    }

    /* Sanitize input */
    స్థిరరాశి గణన xkb_machine_builder_flags invalid_builder_flags
        = config->builder_flags
        & ~(గణన xkb_machine_builder_flags)XKB_MACHINE_BUILDER_FLAGS_విలువలు;
    ఒకవేళ (invalid_builder_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS_,
                     "unrecognized machine builder flags: 0x%x\n",
                     invalid_builder_flags);
        ఒకవేళ (స్థితి_2)
            *స్థితి_2 = XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS;
        తిరిగి ఖాళీ;
    }

    స్థిరరాశి గణన xkb_machine_flags invalid_machine_flags
        = (గణన xkb_machine_flags)config->machine_flags
        & ~(గణన xkb_machine_flags)XKB_MACHINE_FLAGS_విలువలు;
    ఒకవేళ (invalid_machine_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS_,
                     "unrecognized state machine flags: 0x%x\n",
                     invalid_machine_flags);
        ఒకవేళ (స్థితి_2)
            *స్థితి_2 = XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS;
        తిరిగి ఖాళీ;
    }

    నిర్మాణం xkb_machine_builder * స్థిరరాశి builder = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(*builder));
    ఒకవేళ (!builder) {
        log_err_func1(keymap->ctx, XKB_ERROR_కేటాయింపు_FAILURE_2,
                      "cannot allocate machine builder\n");
        ఒకవేళ (స్థితి_2)
            *స్థితి_2 = XKB_ERROR_కేటాయింపు_FAILURE;
        తిరిగి ఖాళీ;
    }

    *builder = (నిర్మాణం xkb_machine_builder) {
        .keymap = xkb_keymap_ref(keymap),
        .refcnt = 1,
        .builder_flags = (గణన xkb_machine_builder_flags)config->builder_flags,
        .machine_flags = (గణన xkb_machine_flags)config->machine_flags,
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

    ఒకవేళ (స్థితి_2)
        *స్థితి_2 = XKB_SUCCESS;

    తిరిగి builder;
}

నిర్మాణం xkb_machine_builder *
xkb_machine_builder_ref(నిర్మాణం xkb_machine_builder *builder)
{
    ప్రతిజ్ఞా(builder->refcnt > 0);
    builder->refcnt++;
    తిరిగి builder;
}

శూన్య
xkb_machine_builder_unref(నిర్మాణం xkb_machine_builder *builder)
{
    ప్రతిజ్ఞా(!builder || builder->refcnt > 0);
    ఒకవేళ (!builder || --builder->refcnt > 0)
        తిరిగి;

    darray_free(builder->shortcuts.entries);
    darray_free(builder->mods);
    xkb_keymap_unref(builder->keymap);
    విడుదల(builder);
}

నిర్మాణం xkb_keymap *
xkb_machine_builder_get_keymap(స్థిరరాశి నిర్మాణం xkb_machine_builder *builder)
{
    /* Reference count is not updated. See API doc. */
    తిరిగి builder->keymap;
}

గణన xkb_స్థితి
xkb_machine_builder_update_a11y(
    నిర్మాణం xkb_machine_builder * పరిమిత builder,
    స్థిరరాశి నిర్మాణం xkb_machine_builder_a11y_update * పరిమిత update)
{
    /* Check ABI compatibility */
    గణన xkb_స్థితి స్థితి_2 = xkb_check_state_abi(update);
    ఒకవేళ (స్థితి_2 != XKB_SUCCESS) {
        xkb_log_abi_error(builder->keymap->ctx, __func__, స్థితి_2);
        తిరిగి స్థితి_2;
    }

    స్థిరరాశి గణన xkb_a11y_flags invalid_flags =
        ~(గణన xkb_a11y_flags)XKB_A11_Y_FLAGS_విలువలు;

    ఒకవేళ (update->affect_flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y affected flags: %#x\n",
                __func__, update->affect_flags & invalid_flags);
        తిరిగి XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }
    ఒకవేళ (update->flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y flags: %#x\n",
                __func__, update->flags & invalid_flags);
        తిరిగి XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }

    builder->controls.a11y.affect_flags |= update->affect_flags;
    builder->controls.a11y.flags &= ~update->affect_flags;
    builder->controls.a11y.flags |= (update->flags & update->affect_flags);

    తిరిగి XKB_SUCCESS;
}

గణన xkb_స్థితి
xkb_machine_builder_update_mods_remap(
    నిర్మాణం xkb_machine_builder * పరిమిత builder,
    స్థిరరాశి నిర్మాణం xkb_machine_builder_mods_remap_update * పరిమిత update
)
{
    ఒకవేళ (!update->ఆధారం) {
        ఒకవేళ (!update->లక్ష్యం) {
            /* Reset mappings */
            darray_resize(builder->mods, 0);
            తిరిగి XKB_SUCCESS;
        } లేకపోతే {
            తిరిగి XKB_ERROR_UNSUPPORTED_MODIFIER_ముసుగు;
        }
    }

    /* Check the modifiers against the keymap */
    స్థిరరాశి xkb_mod_ముసుగు_t invalid = ~builder->keymap->canonical_state_ముసుగు;
    ఒకవేళ ((update->ఆధారం & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ముసుగు,
                "%s: Invalid source modifiers: 0x%"PRIx32"\n",
                __func__, update->ఆధారం);
        తిరిగి XKB_ERROR_UNSUPPORTED_MODIFIER_ముసుగు;
    }
    ఒకవేళ ((update->లక్ష్యం & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ముసుగు,
                "%s: Invalid target modifiers: 0x%"PRIx32"\n",
                __func__, update->లక్ష్యం);
        తిరిగి XKB_ERROR_UNSUPPORTED_MODIFIER_ముసుగు;
    }

    నిర్మాణం machine_mods_mapping *mapping = ఖాళీ;
    darray_కొలత_t m = 0;
    darray_enumerate(m, mapping, builder->mods) {
        ఒకవేళ (mapping->ఆధారం == update->ఆధారం) {
            ఒకవేళ (!update->లక్ష్యం) {
                /* Remove mapping */
                darray_remove(builder->mods, m);
            } లేకపోతే {
                /* Update mapping */
                mapping->లక్ష్యం = update->లక్ష్యం;
            }
            తిరిగి XKB_SUCCESS;
        }
    }

    ఒకవేళ (update->లక్ష్యం) {
        /* Append new mapping */
        darray_append(
            builder->mods,
            (నిర్మాణం machine_mods_mapping) {
                .ఆధారం = update->ఆధారం,
                .లక్ష్యం = update->లక్ష్యం
            }
        );
    } లేకపోతే {
        /* Ignore missing mapping */
    }

    తిరిగి XKB_SUCCESS;
}

గణన xkb_స్థితి
xkb_machine_builder_update_shortcut_override(
    నిర్మాణం xkb_machine_builder * పరిమిత builder,
    స్థిరరాశి నిర్మాణం xkb_machine_builder_shortcut_override_update * పరిమిత update
)
{
    నిర్మాణం xkb_keymap *keymap = builder->keymap;

    /* Check the modifiers against the keymap */
    స్థిరరాశి xkb_mod_ముసుగు_t invalid = ~keymap->canonical_state_ముసుగు;
    ఒకవేళ ((update->affect_mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ముసుగు,
                "%s: Invalid affected modifiers: 0x%"PRIx32"\n",
                __func__, update->affect_mods);
        తిరిగి XKB_ERROR_UNSUPPORTED_MODIFIER_ముసుగు;
    }
    ఒకవేళ ((update->mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ముసుగు,
                "%s: Invalid modifiers: 0x%"PRIx32"\n",
                __func__, update->mods);
        తిరిగి XKB_ERROR_UNSUPPORTED_MODIFIER_ముసుగు;
    }

    /* Check the layout indices against the keymap */
    ఒకవేళ (update->ఆధారం >= keymap->అంకె_groups &&
        update->ఆధారం != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ_2,
                "%s: Invalid source layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->అంకె_groups, update->ఆధారం + 1);
        తిరిగి XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ;
    }
    ఒకవేళ (update->లక్ష్యం >= keymap->అంకె_groups &&
        update->లక్ష్యం != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ_2,
                "%s: Invalid target layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->అంకె_groups, update->లక్ష్యం + 1);
        తిరిగి XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ;
    }

    నిర్మాణం xkb_shortcuts_config_options * స్థిరరాశి config = &builder->shortcuts;

    /* Resize array & initialize new entries, if relevant */
    ఒకవేళ (update->ఆధారం >= darray_కొలత(config->entries) &&
        update->ఆధారం != XKB_LAYOUT_INVALID) {
        ఒకవేళ (update->లక్ష్యం == update->ఆధారం) {
            /* Skip default setting */
            తిరిగి XKB_SUCCESS;
        }
        xkb_layout_సూచీ_t new = darray_కొలత(config->entries);
        darray_resize(config->entries, update->ఆధారం + 1);
        కొరకు (; new <= update->ఆధారం; new++) {
            darray_అంశం(config->entries, new) =
                (నిర్మాణం xkb_shortcuts_config_నమోదు) {
                    .లక్ష్యం = XKB_LAYOUT_INVALID,
                    .mods = 0
                };
        }
    }

    నిర్మాణం xkb_shortcuts_config_నమోదు *నమోదు_2;
    xkb_layout_సూచీ_t ఆధారం = (update->ఆధారం == XKB_LAYOUT_INVALID)
        ? 0               /* Loop over all entries */
        : update->ఆధారం; /* Single entry */
    darray_foreach_from(నమోదు_2, config->entries, ఆధారం) {
        ఒకవేళ (update->ఆధారం == XKB_LAYOUT_INVALID &&
            నమోదు_2->లక్ష్యం == XKB_LAYOUT_INVALID) {
            /* Skip previous deactivated entry */
            ఆధారం++;
            కొనసాగు;
        }

        ఒకవేళ (update->affect_mods) {
            నమోదు_2->mods &= ~update->affect_mods;
            నమోదు_2->mods |= (update->affect_mods & update->mods);
        }

        ఒకవేళ (update->ఆధారం != XKB_LAYOUT_INVALID) {
            నమోదు_2->లక్ష్యం = (update->లక్ష్యం == ఆధారం)
                ? XKB_LAYOUT_INVALID
                : update->లక్ష్యం;
            /* Single entry: exit loop */
            ఆపు;
        }
        ఆధారం++;
    }

    తిరిగి XKB_SUCCESS;
}

/** Compare 2 modifiers combinations */
స్థిర పూర్ణాంకం
cmp_mod_masks(స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b)
{
    స్థిరరాశి xkb_mod_ముసుగు_t m1 = *((xkb_mod_ముసుగు_t *) a);
    స్థిరరాశి xkb_mod_ముసుగు_t m2 = *((xkb_mod_ముసుగు_t *) b);
    ఒకవేళ (m1 == m2)
        తిరిగి 0;

    ఒకవేళ (!m1)
        తిరిగి +1;
    ఒకవేళ (!m2)
        తిరిగి -1;

    స్థిరరాశి xkb_mod_ముసుగు_t overlap = (m1 & m2);
    ఒకవేళ (overlap == m1)
        తిరిగి +1; /* m1 is a subset of m2 */
    ఒకవేళ (overlap == m2)
        తిరిగి -1; /* m2 is a subset of m1 */

    /* Compare active modifiers numbers */
    స్థిరరాశి గుర్తులేని పూర్ణాంకం count1 = popcount32(m1);
    స్థిరరాశి గుర్తులేని పూర్ణాంకం count2 = popcount32(m2);
    ఒకవేళ (count1 > count2)
        తిరిగి -1;
    ఒకవేళ (count1 < count2)
        తిరిగి +1;

    /* Give priority to real modifiers */
    తిరిగి (m1 < m2) ? -1 : +1;
}

స్థిర గణన xkb_స్థితి
machine_సమితి_mods(నిర్మాణం xkb_machine *sm,
                 స్థిరరాశి machine_mods_mappings *raw_mappings)
{
    ఒకవేళ (!darray_ఖాళీలు(*raw_mappings)) {
        machine_mods_mappings mappings = darray_new();
        xkb_mod_ముసుగు_t ముసుగు = 0;
        స్థిరరాశి నిర్మాణం machine_mods_mapping *mapping;
        స్థిరరాశి xkb_mod_ముసుగు_t invalid =
            ~sm->పునాది.పునాది.keymap->canonical_state_ముసుగు;
        darray_foreach(mapping, *raw_mappings) {
            ఒకవేళ (!mapping->ఆధారం || !mapping->లక్ష్యం ||
                (mapping->ఆధారం & invalid) || (mapping->లక్ష్యం & invalid))
                కొనసాగు;
            darray_append(mappings, *mapping);
            ముసుగు |= mapping->ఆధారం;
        }

        /*
         * Re-order the mappings, so that combinations have higher priority
         * if they are superset of other combinations.
         * For other type of overlaps, see the sorting function.
         */
        qsort(darray_వస్తువులు(mappings), darray_కొలత(mappings),
              పరిమాణం(*darray_వస్తువులు(mappings)), &cmp_mod_masks);

        darray_steal(
            mappings,
            &sm->config.modifiers.mappings,
            &sm->config.modifiers.mappings_అంకె
        );
        sm->config.modifiers.ముసుగు = ముసుగు;
    } లేకపోతే {
        sm->config.modifiers = (నిర్మాణం machine_modifiers_config) {
            .mappings = ఖాళీ,
            .mappings_అంకె = 0,
            .ముసుగు = 0
        };
    }

    తిరిగి XKB_SUCCESS;
}

స్థిర గణన xkb_స్థితి
machine_సమితి_shortcuts(
    నిర్మాణం xkb_machine * పరిమిత sm,
    స్థిరరాశి నిర్మాణం xkb_shortcuts_config_options * పరిమిత options
)
{
    ఒకవేళ (darray_ఖాళీలు(options->entries)) {
        sm->config.shortcuts = (నిర్మాణం machine_shortcuts_config) {
            .entries = ఖాళీ
        };
        తిరిగి XKB_SUCCESS;
    }

    నిర్మాణం xkb_keymap * స్థిరరాశి keymap = sm->పునాది.పునాది.keymap;

    నిర్మాణం xkb_shortcuts_config_నమోదు * entries =
        సున్నా_జ్ఞాపకం_ఇవ్వు(keymap->అంకె_groups, పరిమాణం(*entries));
    ఒకవేళ (!entries)
        తిరిగి XKB_ERROR_కేటాయింపు_FAILURE;

    స్థిరరాశి xkb_layout_సూచీ_t లెక్క = MIN(
        keymap->అంకె_groups,
        (xkb_layout_సూచీ_t) darray_కొలత(options->entries)
    );

    bool some_entries = false;
    కొరకు (xkb_layout_సూచీ_t l = 0; l < లెక్క; l++) {
        /* Sanitize layout target */
        స్థిరరాశి xkb_layout_సూచీ_t లక్ష్యం =
            darray_అంశం(options->entries, l).లక్ష్యం;
        /* Sanitize modifier mask */
        స్థిరరాశి xkb_mod_ముసుగు_t mods = (
            darray_అంశం(options->entries, l).mods &
            sm->పునాది.పునాది.keymap->canonical_state_ముసుగు
        );
        ఒకవేళ (లక్ష్యం < keymap->అంకె_groups && లక్ష్యం != l && mods) {
            entries[l] = (నిర్మాణం xkb_shortcuts_config_నమోదు) {
                .లక్ష్యం = లక్ష్యం,
                .mods = mods
            };
            some_entries = true;
        } లేకపోతే {
            entries[l] = (నిర్మాణం xkb_shortcuts_config_నమోదు) {
                .లక్ష్యం = XKB_LAYOUT_INVALID,
                .mods = 0
            };
            కొనసాగు;
        }
    }

    ఒకవేళ (!some_entries) {
        విడుదల(entries);
        entries = ఖాళీ;
    } లేకపోతే {
        /* Initialize groups with no entries */
        కొరకు (xkb_layout_సూచీ_t l = లెక్క; l < keymap->అంకె_groups; l++) {
            entries[l] = (నిర్మాణం xkb_shortcuts_config_నమోదు) {
                .లక్ష్యం = XKB_LAYOUT_INVALID,
                .mods = 0
            };
        }
    }

    sm->config.shortcuts = (నిర్మాణం machine_shortcuts_config) {
        .entries = entries
    };
    తిరిగి XKB_SUCCESS;
}

నిర్మాణం xkb_machine *
xkb_machine_new(స్థిరరాశి నిర్మాణం xkb_machine_builder * పరిమిత builder,
                గణన xkb_స్థితి * పరిమిత స్థితి_2)
{
    నిర్మాణం xkb_machine * స్థిరరాశి machine = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(*machine));
    ఒకవేళ (!machine) {
        log_err_func1(builder->keymap->ctx, XKB_ERROR_కేటాయింపు_FAILURE_2,
                      "cannot allocate machine\n");
        ఒకవేళ (స్థితి_2)
            *స్థితి_2 = XKB_ERROR_కేటాయింపు_FAILURE;
        తిరిగి ఖాళీ;
    }

    xkb_server_state_init(&machine->పునాది, builder->keymap, SERVER_STATE,
                          builder->controls.a11y.affect_flags,
                          builder->controls.a11y.flags);
    machine->flags = builder->machine_flags;
    ఒకవేళ (!(machine->flags &
          XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_కీలకం_FIX))
    {
        machine->పునాది.flags |= STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_కీలకం;
    }
    ఒకవేళ (machine->flags & XKB_MACHINE_UNLOCK_MODS_ON_PRESS) {
        machine->పునాది.flags |= STATE_UNLOCK_MODS_ON_PRESS;
    }

    గణన xkb_స్థితి స్థితి_3;
    ఒకవేళ ((స్థితి_3 = machine_సమితి_mods(machine, &builder->mods)) != XKB_SUCCESS ||
        (స్థితి_3 = machine_సమితి_shortcuts(machine, &builder->shortcuts)) !=
        XKB_SUCCESS) {
        *స్థితి_2 = స్థితి_3;
        వెళ్ళు error;
    }

    darray_init(machine->overlays.కీలకాలు);
    machine->mouse.default_button = XKB_సూచిక_BUTTON_MIN;

    ఒకవేళ (స్థితి_2)
        *స్థితి_2 = XKB_SUCCESS;

    తిరిగి machine;

error:
    xkb_machine_unref(machine);
    తిరిగి ఖాళీ;
}

నిర్మాణం xkb_machine *
xkb_machine_ref(నిర్మాణం xkb_machine *sm)
{
    ప్రతిజ్ఞా(sm->పునాది.పునాది.refcnt > 0);
    sm->పునాది.పునాది.refcnt++;
    తిరిగి sm;
}

శూన్య
xkb_machine_unref(నిర్మాణం xkb_machine *sm)
{
    ప్రతిజ్ఞా(!sm || sm->పునాది.పునాది.refcnt > 0);
    ఒకవేళ (!sm || --sm->పునాది.పునాది.refcnt > 0)
        తిరిగి;

    xkb_state_destroy(&sm->పునాది.పునాది);
    darray_free(sm->overlays.కీలకాలు);
    విడుదల(sm->config.shortcuts.entries);
    విడుదల(sm->config.modifiers.mappings);
    విడుదల(sm);
}

నిర్మాణం xkb_keymap *
xkb_machine_get_keymap(స్థిరరాశి నిర్మాణం xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    తిరిగి sm->పునాది.పునాది.keymap;
}

నిర్మాణం xkb_state *
xkb_machine_get_state(నిర్మాణం xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    తిరిగి (నిర్మాణం xkb_state *)sm;
}

నిర్మాణం xkb_state *
xkb_state_new_from_machine(స్థిరరాశి నిర్మాణం xkb_machine * పరిమిత machine,
                           గణన xkb_స్థితి * పరిమిత స్థితి_2)
{
    /* Creation */
    గణన xkb_స్థితి స్థితి_3;
    నిర్మాణం xkb_state * స్థిరరాశి state = xkb_state_new_with_బహుళకం(
        machine->పునాది.పునాది.keymap,
        XKB_STATE_బహుళకం_SERVER_QUERY,
        &స్థితి_3
    );
    ఒకవేళ (స్థితి_3 != XKB_SUCCESS) {
        ఒకవేళ (స్థితి_2)
            *స్థితి_2 = స్థితి_3;
        తిరిగి ఖాళీ;
    }

    ప్రతిజ్ఞా(state->బహుళకం == SERVER_COMPANION);
    ప్రతిజ్ఞా(state->refcnt == 1);

    /* Copy machine’s state */
    *state = machine->పునాది.పునాది;

    /* Restore previous values */
    state->బహుళకం = SERVER_COMPANION;
    state->refcnt = 1;

    ఒకవేళ (స్థితి_2)
        *స్థితి_2 = XKB_SUCCESS;
    తిరిగి state;
}

స్థిర శూన్య
machine_update_overlays(నిర్మాణం xkb_machine *sm)
{
    /*
     * Overlays indices are stored 1-indexed in nibbles: the lowest
     * nibble corresponds to the latest activated index.
     */

    స్థిరరాశి xkb_overlay_ముసుగు_t ముసుగు =
        overlays_from_controls(sm->పునాది.పునాది.components.controls);
    xkb_overlay_ముసుగు_t added = ముసుగు & ~sm->overlays.enabled;

    /* Remove overlays no longer enabled and keep relative order */
    uint32_t క్రమం = sm->overlays.క్రమం;
    స్థిరరాశి xkb_overlay_సూచీ_t overlay_max =
        format_max_overlays(sm->పునాది.పునాది.keymap->format);
    కొరకు (uint8_t n = 0; n < overlay_max;) {
        xkb_overlay_సూచీ_t overlay_idx = (క్రమం >> (n * 4)) & 0xf;
        ఒకవేళ (!overlay_idx)
            ఆపు;
        స్థిరరాశి xkb_overlay_ముసుగు_t overlay_ముసుగు =
            (xkb_overlay_ముసుగు_t)(1u << (overlay_idx - 1u) /* k is 1-indexed */);
        ఒకవేళ (overlay_ముసుగు & ముసుగు) {
            /* no duplicates */
            added &= ~overlay_ముసుగు;
            n++;
        } లేకపోతే {
            /* remove overlay */
            స్థిరరాశి uint32_t తల = (UINT32_C(1) << (n * 4)) - 1;
            క్రమం = (క్రమం & తల) | ((క్రమం >> 4) & ~తల);
        }
    }

    /* Add new overlays in ascending order */
    కొరకు (xkb_overlay_సూచీ_t k = 0; added; k++, added >>= 1) {
        ఒకవేళ (added & 0x1) {
            క్రమం <<= 4;
            క్రమం |= k + 1u; /* k is 0-indexed */
        }
    }

    sm->overlays.క్రమం = క్రమం;
    sm->overlays.enabled = ముసుగు;
}

గణన xkb_స్థితి
xkb_machine_process_synthetic(నిర్మాణం xkb_machine *sm,
                              స్థిరరాశి నిర్మాణం xkb_synthetic_update *update,
                              నిర్మాణం xkb_events *events)
{
    /* Check ABI compatibility */
    గణన xkb_స్థితి స్థితి_2 =
        check_state_update_abi(sm->పునాది.పునాది.keymap->ctx, update);
    ఒకవేళ (స్థితి_2 != XKB_SUCCESS)
        తిరిగి స్థితి_2;

    నిర్మాణం xkb_server_state * స్థిరరాశి state = &sm->పునాది;
    స్థిరరాశి నిర్మాణం state_components మునుపటి_components = state->పునాది.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    darray_కొలత(events->వరుస) = 0;
    events->తదుపరి = 0;

    /* Update parametrized controls first */
    ఒకవేళ (update->layout_policy) {
        స్థితి_2 = state_update_layout_policy(state, update->layout_policy);
        ఒకవేళ (స్థితి_2 != XKB_SUCCESS)
            తిరిగి స్థితి_2;
    }

    ఒకవేళ (update->components) {
        స్థిరరాశి నిర్మాణం xkb_state_components_update * స్థిరరాశి components =
            update->components;
        /* Update boolean controls first */
        ఒకవేళ (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, events);
        }

        state_update_latched_locked(state, components, events);
    }

    xkb_state_update_derived(&state->పునాది);

    స్థిరరాశి గణన xkb_state_component మారింది = get_state_component_changes(
        &మునుపటి_components, &state->పునాది.components
    );
    ఒకవేళ (మారింది) {
        // TODO: latch controls
        ఒకవేళ (మారింది & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        /* Create event only if some component actually changed */
        darray_append(events->వరుస, (నిర్మాణం xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .మారింది = మారింది,
                .components = state->పునాది.components
            }
        });

        /* Terminate the frame */
        darray_append(events->వరుస, (నిర్మాణం xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_చట్రం,
        });
    }

    తిరిగి XKB_SUCCESS;
}

స్థిర ssize_t
do_remap_modifiers(స్థిరరాశి నిర్మాణం machine_modifiers_config * పరిమిత mappings,
                   నిర్మాణం xkb_state * పరిమిత state,
                   నిర్మాణం xkb_events * పరిమిత events,
                   స్థిరరాశి నిర్మాణం xkb_కీలకం * పరిమిత కీలకం)
{
    ఒకవేళ (!(mappings->ముసుగు & state->components.mods))
        తిరిగి -1;

    స్థిరరాశి xkb_layout_సూచీ_t layout = state_కీలకం_get_layout(state, కీలకం);
    ఒకవేళ (layout >= కీలకం->అంకె_groups)
        తిరిగి -1;

    స్థిరరాశి నిర్మాణం xkb_కీలకం_type * స్థిరరాశి type = కీలకం->groups[layout].type;
    xkb_mod_ముసుగు_t affect_mods = 0;
    xkb_mod_ముసుగు_t mods = 0;
    కొరకు (darray_కొలత_t m = 0; m < mappings->mappings_అంకె; m++) {
        స్థిరరాశి నిర్మాణం machine_mods_mapping * స్థిరరాశి mapping =
            &mappings->mappings[m];
        ఒకవేళ (/* Skip not matching active mods */
            (mapping->ఆధారం & state->components.mods) == mapping->ఆధారం &&
            /* Skip overlap with previous active mappings */
            !(mapping->ఆధారం & affect_mods) &&
            /* Skip if the key type uses some of the source modifiers */
            !(mapping->ఆధారం & type->mods.ముసుగు)) {
            affect_mods |= mappings->mappings[m].ఆధారం;
            mods |= mappings->mappings[m].లక్ష్యం;
        }
    }

    ఒకవేళ (!affect_mods)
        తిరిగి -1;

    నిర్మాణం xkb_state new = *state;
    new.components.పునాది_mods = (new.components.పునాది_mods & ~affect_mods) | mods;
    new.components.latched_mods = (new.components.latched_mods & ~affect_mods);
    new.components.locked_mods = (new.components.locked_mods & ~affect_mods);
    xkb_state_update_derived(&new);

    ssize_t event_idx = -1;
    స్థిరరాశి గణన xkb_state_component మారింది =
        get_state_component_changes(&state->components, &new.components);
    ఒకవేళ (మారింది) {
        event_idx = (ssize_t) darray_కొలత(events->వరుస);
        darray_append(events->వరుస, (నిర్మాణం xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = new.components,
                .మారింది = మారింది
            }
        });
    }

    state->components.mods = new.components.mods;
    తిరిగి event_idx;
}

/**
 * Activate an alternative layout if some modifiers are active, typically
 * `Control`, `Alt` or `Super`.
 *
 * The tweak consists of computing a *base* layout such that the *effective*
 * layout is the corresponding target layout.
 */
స్థిర ssize_t
do_shortcuts_tweak(స్థిరరాశి నిర్మాణం machine_shortcuts_config *config,
                   నిర్మాణం xkb_state *state,
                   స్థిరరాశి నిర్మాణం state_components *మునుపటి_components,
                   నిర్మాణం xkb_events *events, ssize_t remap_event)
{
    ఒకవేళ (!config->entries) {
        /* No shortcuts tweak */
        తిరిగి remap_event;
    }

    స్థిరరాశి నిర్మాణం xkb_shortcuts_config_నమోదు * స్థిరరాశి నమోదు_2 =
        &config->entries[state->components.సమూహం];

    ఒకవేళ (నమోదు_2->లక్ష్యం != XKB_LAYOUT_INVALID &&
        (నమోదు_2->mods & state->components.mods)) {
        /*
         * Activate shortcuts tweak:
         * 1. The real base group is saved by the caller, to be restored by
         *    later by a call to undo_tweaks().
         * 2. Compute the new base group so that the resulting effective group
         *    is the target.
         * 3. Recompute derived state components.
         */
        నిర్మాణం xkb_state new = *state;
        ఒకవేళ (remap_event < 0) {
            /* Create new event */
            remap_event = (ssize_t) darray_కొలత(events->వరుస);
            darray_append(events->వరుస, (నిర్మాణం xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = {0},
                    .మారింది = 0
                }
            });
        } లేకపోతే {
            /* Merge with last remap event */
            new.components =
                darray_అంశం(events->వరుస, remap_event).components.components;
        }
        new.components.పునాది_సమూహం
            = (int32_t) నమోదు_2->లక్ష్యం
            - state->components.latched_సమూహం
            - state->components.locked_సమూహం;
        xkb_state_update_derived(&new);

        స్థిరరాశి గణన xkb_state_component మారింది =
            get_state_component_changes(మునుపటి_components, &new.components);
        darray_అంశం(events->వరుస, remap_event).components.మారింది = మారింది;
        darray_అంశం(events->వరుస, remap_event).components.components =
            new.components;

        state->components.సమూహం = new.components.సమూహం;
    } లేకపోతే {
        /* Deactivated entry or modifiers do not match */
    }
    తిరిగి remap_event;
}

స్థిర శూన్య
undo_tweaks(స్థిరరాశి నిర్మాణం xkb_state *state,
            స్థిరరాశి నిర్మాణం state_components *initial_components,
            నిర్మాణం xkb_events *events)
{
    /* Get last component event */
    స్థిరరాశి నిర్మాణం xkb_event *event = ఖాళీ;
    darray_foreach_తిరగేయి(event, events->వరుస) {
        ఒకవేళ (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS)
            ఆపు;
    }
    ఒకవేళ (!event)
        తిరిగి;

    /* Restore state */
    స్థిరరాశి గణన xkb_state_component మారింది =
        get_state_component_changes(initial_components,
                                    &event->components.components);
    ఒకవేళ (మారింది) {
        darray_append(events->వరుస, (నిర్మాణం xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = *initial_components,
                .మారింది = మారింది
            }
        });
    }
}

స్థిర స్థిరరాశి నిర్మాణం xkb_కీలకం *
process_overlayable_కీలకం(నిర్మాణం xkb_machine *sm,
                        స్థిరరాశి నిర్మాణం xkb_కీలకం * కీలకం,
                        గణన xkb_కీలకం_direction direction)
{
    /* Check if key is currently overlaid */
    నిర్మాణం xkb_overlaid_కీలకం *నమోదు_2 = ఖాళీ;
    నిర్మాణం xkb_overlaid_కీలకం *అందుబాటు_నమోదు = ఖాళీ;
    darray_foreach(నమోదు_2, sm->overlays.కీలకాలు) {
        ఒకవేళ (నమోదు_2->old == కీలకం) {
            /* Maintain the key overlaid */
            ఎంపిక (direction) {
            స్థితి XKB_కీలకం_కిందికి:
                నమోదు_2->refcnt++;
                ఆపు;
            స్థితి XKB_కీలకం_REPEATED:
                ఆపు;
            స్థితి XKB_కీలకం_పైకి:
            అప్రమేయ:
                నమోదు_2->refcnt--;
                ఆపు;
            }
            ఒకవేళ (నమోదు_2->refcnt <= 0)
                నమోదు_2->old = ఖాళీ;
            తిరిగి నమోదు_2->new;
        } లేకపోతే ఒకవేళ (!నమోదు_2->old && !అందుబాటు_నమోదు) {
            /* First free spot */
            అందుబాటు_నమోదు = నమోదు_2;
        }
    }
    ఒకవేళ (direction == XKB_కీలకం_కిందికి) {
        /* Enable key overlay */

        /*
         * Get overlay index: select the latest activated overlay
         * containing the key.
         */
        స్థిరరాశి నిర్మాణం xkb_కీలకం *new = కీలకం;

        ఒకవేళ (కీలకం->overlays & sm->overlays.enabled) {
            /* Some relevant overlay is active */
            కొరకు (uint32_t దొంతర = sm->overlays.క్రమం; దొంతర; దొంతర >>= 4) {
                static_assert(XKB_OVERLAY_లెక్క == 8, "");
                స్థిరరాశి xkb_overlay_సూచీ_t overlay = (దొంతర & 0xf) - 1;
                స్థిరరాశి xkb_overlay_ముసుగు_t ముసుగు =
                    (xkb_overlay_ముసుగు_t)(1u << overlay);
                ఒకవేళ (కీలకం->overlays & ముసుగు) {
                    /* Get new key */
                    ఒకవేళ (కీలకం->overlays_inline) {
                        /* Stored inline */
                        new = కీలకం->overlay_కీలకం;
                    } లేకపోతే {
                        /* Stored in sparse array */
                        స్థిరరాశి xkb_overlay_ముసుగు_t దిగువ = (xkb_overlay_ముసుగు_t)(
                            కీలకం->overlays & (xkb_overlay_ముసుగు_t)(ముసుగు - 1u)
                        );
                        /* index = number of set bits strictly below bit */
                        స్థిరరాశి xkb_overlay_సూచీ_t సూచీ =
                            (xkb_overlay_సూచీ_t)popcount32(దిగువ);
                        new = కీలకం->overlays_కీలకాలు[సూచీ];
                    }
                    ఆపు;
                }
            }
        } లేకపోతే {
            /*
             * No relevant overlay is active: block future overlays while
             * the key is pressed.
             */
        }

        /* Get new entry */
        ఒకవేళ (అందుబాటు_నమోదు) {
            /* Re-use a free entry */
            నమోదు_2 = అందుబాటు_నమోదు;
        } లేకపోతే {
            /* Append a new entry */
            స్థిరరాశి darray_కొలత_t idx = darray_కొలత(sm->overlays.కీలకాలు);
            darray_resize(sm->overlays.కీలకాలు, idx + 1);
            నమోదు_2 = &darray_అంశం(sm->overlays.కీలకాలు, idx);
        }

        /* Overlay the key */
        నమోదు_2->old = కీలకం;
        నమోదు_2->new = new;
        నమోదు_2->refcnt = 1;
        తిరిగి new;
    }

    /* No overlay */
    తిరిగి కీలకం;
}

గణన xkb_స్థితి
xkb_machine_process_కీలకం(నిర్మాణం xkb_machine *sm,
                        xkb_keycode_t kc, గణన xkb_కీలకం_direction direction,
                        నిర్మాణం xkb_events *events)
{
    darray_కొలత(events->వరుస) = 0;
    events->తదుపరి = 0;

    నిర్మాణం xkb_server_state * స్థిరరాశి state = &sm->పునాది;
    స్థిరరాశి నిర్మాణం xkb_కీలకం * కీలకం = Xkb_కీలకం(state->పునాది.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    ఒకవేళ (!కీలకం || (direction == XKB_కీలకం_REPEATED && !కీలకం->repeats))
        తిరిగి XKB_SUCCESS;

    స్థిరరాశి నిర్మాణం state_components initial_components = state->పునాది.components;

    ఒకవేళ (direction == XKB_కీలకం_కిందికి &&
        (sm->పునాది.పునాది.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_కీలకాలు) &&
        (sm->పునాది.a11y_flags & XKB_A11_Y_STICKY_కీలకాలు_NO_ఏకకాల_కీలకాలు) &&
        state->పునాది.components.పునాది_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_కీలకాలు,
                                      0, events);
    }

    ఒకవేళ (కీలకం->overlays)
        కీలకం = process_overlayable_కీలకం(sm, కీలకం, direction);

    ssize_t remap_event = do_remap_modifiers(&sm->config.modifiers,
                                             &state->పునాది, events, కీలకం);

    remap_event = do_shortcuts_tweak(&sm->config.shortcuts, &state->పునాది,
                                     &initial_components, events, remap_event);

    ఒకవేళ (remap_event >= 0) {
        // TODO: what about merging state event for RedirectKey?
        darray_append(events->వరుస, (నిర్మాణం xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_చట్రం,
        });
    }

    state->flags |= STATE_REQUIRE_కీలకం_EVENT;
    state->సమితి_mods = 0;
    state->clear_mods = 0;

    /* Handle key actions */
    xkb_filter_apply_all(state, events, కీలకం, direction);

    xkb_mod_సూచీ_t i;
    xkb_mod_ముసుగు_t బిట్టు;
    కొరకు (i = 0, బిట్టు = 1; state->సమితి_mods; i++, బిట్టు <<= 1) {
        ఒకవేళ (state->సమితి_mods & బిట్టు) {
            state->mod_కీలకం_లెక్క[i]++;
            state->పునాది.components.పునాది_mods |= బిట్టు;
            state->సమితి_mods &= ~బిట్టు;
        }
    }

    కొరకు (i = 0, బిట్టు = 1; state->clear_mods; i++, బిట్టు <<= 1) {
        ఒకవేళ (state->clear_mods & బిట్టు) {
            state->mod_కీలకం_లెక్క[i]--;
            ఒకవేళ (state->mod_కీలకం_లెక్క[i] <= 0) {
                state->పునాది.components.పునాది_mods &= ~బిట్టు;
                state->mod_కీలకం_లెక్క[i] = 0;
            }
            state->clear_mods &= ~బిట్టు;
        }
    }

    xkb_state_update_derived(&state->పునాది);

    /*
     * Insert key event only if actions did not override it, e.g.:
     * - RedirectKey() append its own key event;
     * - MovePointer() and PointerButton() generate pointer events instead
     *   of key events;
     * - TerminateServer() generate a Server event instead of key events.
     */
    ఒకవేళ (state->flags & STATE_REQUIRE_కీలకం_EVENT) {
        darray_append(events->వరుస, (నిర్మాణం xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_కీలకం,
            .కీలకం = {
                .keycode = కీలకం->keycode,
                .direction = direction
            }
        });
    }

    ఒకవేళ (remap_event >= 0) {
        // FIXME: fragile if last state change does not restore the state to the remap event
        undo_tweaks(&state->పునాది, &initial_components, events);
    }

    స్థిరరాశి గణన xkb_state_component మారింది = get_state_component_changes(
        &initial_components, &state->పునాది.components
    );
    ఒకవేళ (మారింది) {
        ఒకవేళ (మారింది & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        // TODO: merge with previous state event, if any

        darray_append(events->వరుస, (నిర్మాణం xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = state->పునాది.components,
                .మారింది = మారింది
            }
        });
    }

    ఒకవేళ (!darray_ఖాళీలు(events->వరుస) &&
        darray_అంశం(events->వరుస, darray_కొలత(events->వరుస) - 1).type !=
        XKB_EVENT_TYPE_చట్రం)
    {
        darray_append(events->వరుస, (నిర్మాణం xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_చట్రం,
        });
    }

    తిరిగి XKB_SUCCESS;
}

నిర్మాణం xkb_events *
xkb_events_new(నిర్మాణం xkb_context * పరిమిత context,
               స్థిరరాశి నిర్మాణం xkb_events_config * పరిమిత config,
               గణన xkb_స్థితి * పరిమిత స్థితి_2)
{
    /* Handle default configuration */
    స్థిర స్థిరరాశి నిర్మాణం xkb_events_config default_config = {
        .కొలత = పరిమాణం(default_config)
    };
    ఒకవేళ (!config)
        config = &default_config;

    /* Check ABI compatibility */
    స్థిరరాశి గణన xkb_స్థితి abi_error = xkb_check_state_abi(config);
    ఒకవేళ (abi_error) {
        xkb_log_abi_error(context, __func__, abi_error);
        ఒకవేళ (స్థితి_2)
            *స్థితి_2 = abi_error;
        తిరిగి ఖాళీ;
    }

    /* Sanitize input */
    స్థిరరాశి uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_EVENTS_FLAGS_విలువలు);
    ఒకవేళ (invalid_flags) {
        log_err_func(context, XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS_,
                     "unrecognized events collection flags: 0x%"PRIx32"\n",
                     invalid_flags);
        ఒకవేళ (స్థితి_2)
            *స్థితి_2 = XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS;
        తిరిగి ఖాళీ;
    }

    నిర్మాణం xkb_events *events = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(*events));
    ఒకవేళ (!events) {
        log_err_func1(context, XKB_ERROR_కేటాయింపు_FAILURE_2,
                      "cannot allocate state events collection\n");
        ఒకవేళ (స్థితి_2)
            *స్థితి_2 = XKB_ERROR_కేటాయింపు_FAILURE;
        తిరిగి ఖాళీ;
    }

    /* Pre-allocate event queue */
    darray_init(events->వరుస);
    స్థిర స్థిరరాశి darray_కొలత_t వరుస_initial_కొలత = 16;
    darray_resize(events->వరుస, వరుస_initial_కొలత);
    // TODO: proper safe darray_resize() with return value
    ఒకవేళ (!darray_వస్తువులు(events->వరుస)) {
        log_err_func1(context, XKB_ERROR_కేటాయింపు_FAILURE_2,
                      "cannot allocate state events queue\n");
        xkb_events_destroy(events);
        ఒకవేళ (స్థితి_2)
            *స్థితి_2 = XKB_ERROR_కేటాయింపు_FAILURE;
        తిరిగి ఖాళీ;
    }
    darray_కొలత(events->వరుస) = 0;

    events->ctx = xkb_context_ref(context);
    events->తదుపరి = 0;

    ఒకవేళ (స్థితి_2)
        *స్థితి_2 = XKB_SUCCESS;
    తిరిగి events;
}

శూన్య
xkb_events_destroy(నిర్మాణం xkb_events *events)
{
    ఒకవేళ (!events)
        తిరిగి;
    darray_free(events->వరుస);
    xkb_context_unref(events->ctx);
    విడుదల(events);
}

స్థిరరాశి నిర్మాణం xkb_event *
xkb_events_తదుపరి(నిర్మాణం xkb_events *events)
{
    ఒకవేళ (events->తదుపరి < darray_కొలత(events->వరుస)) {
        స్థిరరాశి darray_కొలత_t సూచీ = events->తదుపరి++;
        తిరిగి &darray_అంశం(events->వరుస, సూచీ);
    } లేకపోతే {
        తిరిగి ఖాళీ;
    }
}

గణన xkb_event_type
xkb_event_get_type(స్థిరరాశి నిర్మాణం xkb_event *event)
{
    తిరిగి event->type;
}

గణన xkb_స్థితి
xkb_event_get_keycode(స్థిరరాశి నిర్మాణం xkb_event *event,
                      xkb_keycode_t *keycode,
                      గణన xkb_కీలకం_direction *direction)
{
    ఒకవేళ (event->type == XKB_EVENT_TYPE_కీలకం) {
        *keycode = event->కీలకం.keycode;
        *direction = event->కీలకం.direction;
        తిరిగి XKB_SUCCESS;
    } లేకపోతే {
        తిరిగి XKB_ERROR_EVENT_TYPE_MISMATCH;
    }
}

గణన xkb_స్థితి
xkb_event_get_components(స్థిరరాశి నిర్మాణం xkb_event * పరిమిత event,
                         నిర్మాణం xkb_event_components * పరిమిత components)
{
    ఒకవేళ (event->type != XKB_EVENT_TYPE_STATE_COMPONENTS)
        తిరిగి XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    స్థిరరాశి గణన xkb_స్థితి స్థితి_2 = xkb_check_state_abi(components);
    ఒకవేళ (స్థితి_2 != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, స్థితి_2);
        తిరిగి స్థితి_2;
    }

    components->మారింది = (uint32_t)event->components.మారింది;
    components->depressed_mods = event->components.components.పునాది_mods;
    components->latched_mods = event->components.components.latched_mods;
    components->locked_mods = event->components.components.locked_mods;
    components->mods = event->components.components.mods;
    components->depressed_layout = event->components.components.పునాది_సమూహం;
    components->latched_layout = event->components.components.latched_సమూహం;
    components->locked_layout = event->components.components.locked_సమూహం;
    components->layout = event->components.components.సమూహం;
    components->leds = event->components.components.leds;
    components->controls = (uint32_t)event->components.components.controls;

    తిరిగి XKB_SUCCESS;
}

గణన xkb_స్థితి
xkb_event_get_సూచిక_motion(స్థిరరాశి నిర్మాణం xkb_event * పరిమిత event,
                             నిర్మాణం xkb_event_సూచిక_motion * పరిమిత motion)
{
    ఒకవేళ (event->type != XKB_EVENT_TYPE_సూచిక_MOTION)
        తిరిగి XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    స్థిరరాశి గణన xkb_స్థితి స్థితి_2 = xkb_check_state_abi(motion);
    ఒకవేళ (స్థితి_2 != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, స్థితి_2);
        తిరిగి స్థితి_2;
    }

    motion->flags = event->సూచిక_motion.flags;
    motion->x = event->సూచిక_motion.x;
    motion->y = event->సూచిక_motion.y;

    తిరిగి XKB_SUCCESS;
}

గణన xkb_స్థితి
xkb_event_get_సూచిక_button(స్థిరరాశి నిర్మాణం xkb_event * పరిమిత event,
                             నిర్మాణం xkb_event_సూచిక_button * పరిమిత button)
{
    ఒకవేళ (event->type != XKB_EVENT_TYPE_సూచిక_BUTTON)
        తిరిగి XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    స్థిరరాశి గణన xkb_స్థితి స్థితి_2 = xkb_check_state_abi(button);
    ఒకవేళ (స్థితి_2 != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, స్థితి_2);
        తిరిగి స్థితి_2;
    }

    button->button = event->సూచిక_button.button;
    button->state = event->సూచిక_button.state;
    button->లెక్క = event->సూచిక_button.లెక్క;

    తిరిగి XKB_SUCCESS;
}

గణన xkb_స్థితి
xkb_event_get_virtual_console(స్థిరరాశి నిర్మాణం xkb_event * పరిమిత event,
                              int8_t * పరిమిత సూచీ_or_స్థానభ్రంశం,
                              bool * పరిమిత is_స్థానభ్రంశం)
{
    ఒకవేళ (event->type != XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
        తిరిగి XKB_ERROR_EVENT_TYPE_MISMATCH;

    *సూచీ_or_స్థానభ్రంశం = event->virtual_console.సూచీ_or_స్థానభ్రంశం;
    *is_స్థానభ్రంశం = event->virtual_console.is_స్థానభ్రంశం;

    తిరిగి XKB_SUCCESS;
}

గణన xkb_స్థితి
xkb_state_update_event(నిర్మాణం xkb_state * పరిమిత పునాది_state,
                       స్థిరరాశి నిర్మాణం xkb_event * పరిమిత event,
                       గణన xkb_state_component * పరిమిత మారింది)
{
    /* Guard against server-only state */
    ప్రతిజ్ఞా(పునాది_state->బహుళకం == SERVER_COMPANION ||
           పునాది_state->బహుళకం == LEGACY_MIXED_STATE);
    ఒకవేళ (పునాది_state->బహుళకం != SERVER_COMPANION &&
        పునాది_state->బహుళకం != LEGACY_MIXED_STATE) {
        log_err(పునాది_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_బహుళకం_2,
                "%s: Unexpected state type %d\n", __func__, పునాది_state->బహుళకం);
        తిరిగి XKB_ERROR_UNEXPECTED_STATE_బహుళకం;
    }

    నిర్మాణం xkb_client_state * స్థిరరాశి state =
        (నిర్మాణం xkb_client_state *)పునాది_state;

    ఒకవేళ (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
        స్థిరరాశి నిర్మాణం state_components prev_components = state->పునాది.components;
        state->పునాది.components = event->components.components;
        /*
         * Recompute the changes instead of using the event value, because we do
         * not know if the event’s queue and the state are synced.
         */
        ఒకవేళ (మారింది)
            *మారింది = get_state_component_changes(&prev_components,
                                                   &state->పునాది.components);
    } లేకపోతే {
        ఒకవేళ (మారింది)
            *మారింది = 0;
    }
    తిరిగి XKB_SUCCESS;
}

స్థిర శూన్య
append_సూచిక_button(నిర్మాణం xkb_events * పరిమిత events,
                      నిర్మాణం xkb_filter * పరిమిత filter,
                      గణన xkb_సూచిక_button_state button_state,
                      uint8_t లెక్క)
{
    darray_append(events->వరుస, (నిర్మాణం xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_సూచిక_BUTTON,
        .సూచిక_button = {
            .కొలత = పరిమాణం(((నిర్మాణం xkb_event*)0)->సూచిక_button),
            .button = filter->priv,
            .state = button_state,
            .లెక్క = లెక్క,
        }
    });
}

స్థిర శూన్య
xkb_filter_సూచిక_button_new(నిర్మాణం xkb_server_state *state,
                              నిర్మాణం xkb_events *events,
                              నిర్మాణం xkb_filter *filter)
{
    ఒకవేళ (!events || state->పునాది.బహుళకం != SERVER_STATE ||
        !(state->పునాది.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_కీలకాలు)) {
        filter->func = ఖాళీ;
        తిరిగి;
    }

    నిర్మాణం xkb_machine * స్థిరరాశి sm = (నిర్మాణం xkb_machine *)state;

    /* Resolve button */
    స్థిరరాశి xkb_సూచిక_button_సూచీ_t button =
        (filter->action.btn.button == XKB_సూచిక_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;
    filter->priv = button;

    స్థిరరాశి గణన xkb_సూచిక_button_state button_state =
        XKB_సూచిక_BUTTON_PRESSED;
    append_సూచిక_button(events, filter, button_state,
                          filter->action.btn.లెక్క);
    state->flags &= ~STATE_REQUIRE_కీలకం_EVENT;
}

స్థిర bool
xkb_filter_సూచిక_button_func(నిర్మాణం xkb_server_state *state,
                               నిర్మాణం xkb_events *events,
                               నిర్మాణం xkb_filter *filter,
                               స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                               గణన xkb_కీలకం_direction direction)
{
    ఒకవేళ (కీలకం != filter->కీలకం || !events)
        తిరిగి XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_కీలకం_EVENT;

    ఎంపిక (direction) {
    స్థితి XKB_కీలకం_కిందికి:
        filter->refcnt++;
        /* fallthrough */
    స్థితి XKB_కీలకం_REPEATED:
        తిరిగి XKB_FILTER_CONSUME;
    అప్రమేయ:
        ఒకవేళ (--filter->refcnt > 0)
            తిరిగి XKB_FILTER_CONSUME;
    }

    ఒకవేళ (!filter->action.btn.లెక్క)
        append_సూచిక_button(events, filter, XKB_సూచిక_BUTTON_RELEASED, 0);

    filter->func = ఖాళీ;
    తిరిగి XKB_FILTER_CONSUME;
}

నిర్మాణం xkb_filter_సూచిక_తాళం_button_priv {
    uint32_t button:31;
#నిర్వచితమైతే _WIN32
    uint32_t locked:1; // MSVC requires matching types to pack bitfields
#లేకపోతే
    bool locked:1;
#అయితే_ముగింపు
};

గణన {
    XKB_FILTER_సూచిక_తాళం_BUTTON_PRIV_వెడల్పు =
        పరిమాణం(నిర్మాణం xkb_filter_సూచిక_తాళం_button_priv) * CHAR_BIT,
    XKB_FILTER_సూచిక_తాళం_BUTTON_PRIV_MAX =
        (UINT64_C(1) << XKB_FILTER_సూచిక_తాళం_BUTTON_PRIV_వెడల్పు) - 1
};

static_assert(పరిమాణం(నిర్మాణం xkb_filter_సూచిక_తాళం_button_priv) ==
              పరిమాణం(((నిర్మాణం xkb_filter*)0)->priv),
              "");
static_assert((uint32_t)XKB_సూచిక_BUTTON_MAX < (uint32_t)XKB_FILTER_సూచిక_తాళం_BUTTON_PRIV_MAX,
              "xkb_filter_pointer_lock_button_priv cannot store button");

స్థిర శూన్య
xkb_filter_సూచిక_తాళం_button_new(నిర్మాణం xkb_server_state *state,
                                   నిర్మాణం xkb_events *events,
                                   నిర్మాణం xkb_filter *filter)
{
    ఒకవేళ (!events || state->పునాది.బహుళకం != SERVER_STATE ||
        !(state->పునాది.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_కీలకాలు)) {
        filter->func = ఖాళీ;
        తిరిగి;
    }

    state->flags &= ~STATE_REQUIRE_కీలకం_EVENT;

    నిర్మాణం xkb_machine * స్థిరరాశి sm = (నిర్మాణం xkb_machine *)state;

    /* Resolve button */
    స్థిరరాశి xkb_సూచిక_button_సూచీ_t button =
        (filter->action.btn.button == XKB_సూచిక_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;

    static_assert(XKB_సూచిక_BUTTON_ముసుగు_వెడల్పు <= 8,
                  "invalid UINTx_C() macro");
    static_assert(XKB_సూచిక_BUTTON_MAX <= XKB_సూచిక_BUTTON_ముసుగు_వెడల్పు,
                  "invalid left shift");
    స్థిరరాశి xkb_సూచిక_button_ముసుగు_t ముసుగు = (UINT8_C(1) << button);

    స్థిరరాశి bool locked = (sm->mouse.locked_buttons & ముసుగు);

    ఒకవేళ (!locked && !(filter->action.btn.flags & ACTION_తాళం_NO_తాళం)) {
        sm->mouse.locked_buttons |= ముసుగు;
        filter->priv = button;
        append_సూచిక_button(events, filter, XKB_సూచిక_BUTTON_PRESSED, 0);
    }

    నిర్మాణం xkb_filter_సూచిక_తాళం_button_priv *priv =
        (నిర్మాణం xkb_filter_సూచిక_తాళం_button_priv*)&filter->priv;
    priv->button = button,
    priv->locked = locked;
}

స్థిర bool
xkb_filter_సూచిక_తాళం_button_func(నిర్మాణం xkb_server_state *state,
                                    నిర్మాణం xkb_events *events,
                                    నిర్మాణం xkb_filter *filter,
                                    స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                                    గణన xkb_కీలకం_direction direction)
{
    ఒకవేళ (కీలకం != filter->కీలకం || !events || state->పునాది.బహుళకం != SERVER_STATE)
        తిరిగి XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_కీలకం_EVENT;

    ఎంపిక (direction) {
    స్థితి XKB_కీలకం_కిందికి:
        filter->refcnt++;
        /* fallthrough */
    స్థితి XKB_కీలకం_REPEATED:
        తిరిగి XKB_FILTER_CONSUME;
    అప్రమేయ:
        ఒకవేళ (--filter->refcnt > 0)
            తిరిగి XKB_FILTER_CONSUME;
    }

    స్థిరరాశి నిర్మాణం xkb_filter_సూచిక_తాళం_button_priv *priv =
        (నిర్మాణం xkb_filter_సూచిక_తాళం_button_priv*)&filter->priv;

    ఒకవేళ (priv->locked && !(filter->action.btn.flags & ACTION_తాళం_NO_UNLOCK)) {
        నిర్మాణం xkb_machine * స్థిరరాశి sm = (నిర్మాణం xkb_machine *)state;

        static_assert(XKB_సూచిక_BUTTON_ముసుగు_వెడల్పు <= 8,
                    "invalid UINTx_C() macro");
        static_assert(XKB_సూచిక_BUTTON_MAX <= XKB_సూచిక_BUTTON_ముసుగు_వెడల్పు,
                    "invalid left shift");
        స్థిరరాశి xkb_సూచిక_button_ముసుగు_t ముసుగు = (UINT8_C(1) << priv->button);

        sm->mouse.locked_buttons &= ~ముసుగు;
        filter->priv = priv->button;
        append_సూచిక_button(events, filter, XKB_సూచిక_BUTTON_RELEASED, 0);
    }

    filter->func = ఖాళీ;
    తిరిగి XKB_FILTER_CONSUME;
}

స్థిర శూన్య
xkb_filter_సూచిక_సమితి_default_button_new(నిర్మాణం xkb_server_state *state,
                                          నిర్మాణం xkb_events *events,
                                          నిర్మాణం xkb_filter *filter)
{
    ఒకవేళ (!events || state->పునాది.బహుళకం != SERVER_STATE ||
        !(state->పునాది.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_కీలకాలు)) {
        filter->func = ఖాళీ;
        తిరిగి;
    }

    state->flags &= ~STATE_REQUIRE_కీలకం_EVENT;

    నిర్మాణం xkb_machine * స్థిరరాశి sm = (నిర్మాణం xkb_machine *)state;

    static_assert(2 * XKB_సూచిక_BUTTON_MAX <= గరిష్ఠ_పూర్ణాంకం, "lossy conversion");
    పూర్ణాంకం button = (filter->action.dflt.flags & ACTION_ABSOLUTE_SWITCH)
        ? filter->action.dflt.విలువ
        : sm->mouse.default_button + filter->action.dflt.విలువ;

    /* Wrap into range */
    స్థిరరాశి పూర్ణాంకం rem = button % XKB_సూచిక_BUTTON_MAX;
    sm->mouse.default_button = (rem >= 0)
        ? (xkb_సూచిక_button_సూచీ_t)rem
        : (xkb_సూచిక_button_సూచీ_t)(rem + XKB_సూచిక_BUTTON_MAX);
}

స్థిర bool
xkb_filter_సూచిక_సమితి_default_button_func(నిర్మాణం xkb_server_state *state,
                                           నిర్మాణం xkb_events *events,
                                           నిర్మాణం xkb_filter *filter,
                                           స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                                           గణన xkb_కీలకం_direction direction)
{
    ఒకవేళ (కీలకం != filter->కీలకం || !events)
        తిరిగి XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_కీలకం_EVENT;

    ఎంపిక (direction) {
    స్థితి XKB_కీలకం_కిందికి:
        filter->refcnt++;
        /* fallthrough */
    స్థితి XKB_కీలకం_REPEATED:
        తిరిగి XKB_FILTER_CONSUME;
    అప్రమేయ:
        ఒకవేళ (--filter->refcnt > 0)
            తిరిగి XKB_FILTER_CONSUME;
    }

    filter->func = ఖాళీ;
    తిరిగి XKB_FILTER_CONTINUE;
}

స్థిర శూన్య
xkb_filter_terminate_server_new(నిర్మాణం xkb_server_state *state,
                                 నిర్మాణం xkb_events *events,
                                 నిర్మాణం xkb_filter *filter)
{
    ఒకవేళ (!events || state->పునాది.బహుళకం != SERVER_STATE ||
        !(((నిర్మాణం xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = ఖాళీ;
        తిరిగి;
    }

    state->flags &= ~STATE_REQUIRE_కీలకం_EVENT;

    darray_append(events->వరుస, (నిర్మాణం xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    });
}

స్థిర bool
xkb_filter_terminate_server_func(నిర్మాణం xkb_server_state *state,
                                 నిర్మాణం xkb_events *events,
                                 నిర్మాణం xkb_filter *filter,
                                 స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                                 గణన xkb_కీలకం_direction direction)
{
    ఒకవేళ (కీలకం != filter->కీలకం || !events)
        తిరిగి XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_కీలకం_EVENT;

    ఎంపిక (direction) {
    స్థితి XKB_కీలకం_కిందికి:
        filter->refcnt++;
        /* fallthrough */
    స్థితి XKB_కీలకం_REPEATED:
        తిరిగి XKB_FILTER_CONSUME;
    అప్రమేయ:
        ఒకవేళ (--filter->refcnt > 0)
            తిరిగి XKB_FILTER_CONSUME;
    }

    filter->func = ఖాళీ;
    తిరిగి XKB_FILTER_CONSUME;
}

స్థిర శూన్య
xkb_filter_switch_virtual_console_new(నిర్మాణం xkb_server_state *state,
                                      నిర్మాణం xkb_events *events,
                                      నిర్మాణం xkb_filter *filter)
{
    ఒకవేళ (!events || state->పునాది.బహుళకం != SERVER_STATE ||
        !(((నిర్మాణం xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = ఖాళీ;
        తిరిగి;
    }

    state->flags &= ~STATE_REQUIRE_కీలకం_EVENT;

    darray_append(events->వరుస, (నిర్మాణం xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
        .virtual_console = {
            .సూచీ_or_స్థానభ్రంశం = filter->action.screen.screen,
            .is_స్థానభ్రంశం = !(filter->action.screen.flags & ACTION_ABSOLUTE_SWITCH)
        }
    });
}

స్థిర bool
xkb_filter_switch_virtual_console_func(నిర్మాణం xkb_server_state *state,
                                       నిర్మాణం xkb_events *events,
                                       నిర్మాణం xkb_filter *filter,
                                       స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                                       గణన xkb_కీలకం_direction direction)
{
    ఒకవేళ (కీలకం != filter->కీలకం || !events)
        తిరిగి XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_కీలకం_EVENT;

    ఎంపిక (direction) {
    స్థితి XKB_కీలకం_కిందికి:
        filter->refcnt++;
        /* fallthrough */
    స్థితి XKB_కీలకం_REPEATED:
        తిరిగి XKB_FILTER_CONSUME;
    అప్రమేయ:
        ఒకవేళ (--filter->refcnt > 0)
            తిరిగి XKB_FILTER_CONSUME;
    }

    filter->func = ఖాళీ;
    తిరిగి XKB_FILTER_CONSUME;
}
