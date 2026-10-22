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

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <limits.h>
#ضمن <stdint.h>
#ضمن <stdlib.h>
#ضمن <string.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "xkbcommon/xkbcommon-status.h"
#ضمن "xkbcommon/xkbcommon-features.h"
#ضمن "abi-check.h"
#ضمن "darray.h"
#ضمن "features/enums.h"
#ضمن "keymap.h"
#ضمن "keysym.h"
#ضمن "messages-codes.h"
#ضمن "state-priv.h"
#ضمن "utf8.h"
#ضمن "utils.h"
#ضمن "util-mem.h"
#ضمن "util-numbers.h"

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
بنية xkb_events {
    بنية xkb_context *ctx;
    darray(بنية xkb_event) کتار;
    /**
     * Read cursor for `xkb_events_next()`.
     *
     * Reset to 0 on each `process_*` call.
     */
    darray_اندازه_t بل;
};

بنية xkb_server_state;

بنية xkb_filter {
    اتحاد xkb_action action;
    ثابت بنية xkb_کلی *کلی;
    bool (*func)(بنية xkb_server_state *state,
                 بنية xkb_events *events,
                 بنية xkb_filter *filter,
                 ثابت بنية xkb_کلی *کلی,
                 تعداد xkb_کلی_direction direction);
    uint32_t priv;
    صحيح refcnt;
};

تعداد xkb_state_ډېرپېښ_دننی {
    /** `xkb_state`: Client-only state (server API raises errors) */
    CLIENT_STATE = XKB_STATE_ډېرپېښ_CLIENT,
    /** `xkb_state`: Server query companion */
    SERVER_COMPANION = XKB_STATE_ډېرپېښ_SERVER_QUERY,
    /** `xkb_state`: Legacy unsafe client + server state */
    LEGACY_MIXED_STATE = 2,
    /** `xkb_state`: Legacy server-only state (client-only API raises errors) */
    LEGACY_SERVER_STATE = 3,
    /** `xkb_machine`: Server-only state */
    SERVER_STATE = 4,
    XKB_STATE_TYPE_عدد_ENTRIES = 5
};

static_assert(CLIENT_STATE == 0 &&
              CLIENT_STATE == SERVER_COMPANION - 1 &&
              SERVER_COMPANION == LEGACY_MIXED_STATE - 1 &&
              LEGACY_MIXED_STATE == LEGACY_SERVER_STATE - 1 &&
              LEGACY_SERVER_STATE == SERVER_STATE - 1 &&
              SERVER_STATE == XKB_STATE_TYPE_عدد_ENTRIES - 1 &&
              XKB_STATE_TYPE_عدد_ENTRIES == 5,
              "Order is used to guard client/server -only APIs: do not change");

تعداد { XKB_LAYOUT_شاخص_T_MIN_پلنوالی = 6 };
static_assert(XKB_MAX_GROUPS < (1 << XKB_LAYOUT_شاخص_T_MIN_پلنوالی),
              "Cannot encode out_of_range_group::redirect_group");

بنية بهر_د_range_ډله {
    تعداد xkb_layout_بهر_د_range_policy policy :
        (حجم(تعداد xkb_layout_بهر_د_range_policy) * CHAR_BIT -
         XKB_LAYOUT_شاخص_T_MIN_پلنوالی);
    xkb_layout_شاخص_t redirect_ډله : XKB_LAYOUT_شاخص_T_MIN_پلنوالی;
};

تعداد { XKB_STATE_ډېرپېښ_دننی_MIN_پلنوالی = 4 /* 3 bits + sign */ };
static_assert(XKB_STATE_TYPE_عدد_ENTRIES <
              (1 << XKB_STATE_ډېرپېښ_دننی_MIN_پلنوالی),
              "Cannot encode xkb_state::mode");

/**
 * Common state
 *
 * `mode` indicates whether the object is casted from a `xkb_server_state` or a
 * `xkb_client_state` struct.
 */
بنية xkb_state {
    بنية xkb_keymap *keymap;
    /*
     * Before updating the state, we keep a copy of just this struct. This
     * allows us to report which components of the state have changed.
     */
    بنية state_components components;
    بنية بهر_د_range_ډله بهر_د_range_ډله;

    تعداد xkb_state_ډېرپېښ_دننی ډېرپېښ : XKB_STATE_ډېرپېښ_دننی_MIN_پلنوالی;
    صحيح refcnt : (حجم(صحيح) * CHAR_BIT - XKB_STATE_ډېرپېښ_دننی_MIN_پلنوالی);
};

/**
 * Minimal client state
 *
 * Wrapper for strong typing
 */
بنية xkb_client_state {
    /** Inherits from `xkb_state` */
    بنية xkb_state بنسټ;
};

تعداد state_flags {
    STATE_NO_FLAGS = 0,
    STATE_REQUIRE_کلی_EVENT = (1 << 0),
    STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_کلی = (1 << 1),
    STATE_UNLOCK_MODS_ON_PRESS = (1 << 2),
};

/**
 * Legacy server state, used for `LEGACY_MIXED_STATE` and `LEGACY_SERVER_STATE`
 * modes.
 *
 * Casted as `xkb_state` in the public API. See `xkb_state::mode`.
 */
بنية xkb_server_state {
    /** Inherits from `xkb_state` */
    بنية xkb_state بنسټ;

    /* Server-specific state data */

    darray(بنية xkb_filter) filters;

    /**
     * Mix of constant configuration flags and
     * variable state-processing flags.
     */
    تعداد state_flags flags;

    تعداد xkb_a11y_flags a11y_flags;

    /*
     * At each event, we accumulate all the needed modifications to the base
     * modifiers, and apply them at the end. These keep track of this state.
     */
    xkb_mod_ماسک_t سېټ_mods;
    xkb_mod_ماسک_t clear_mods;

    /*
     * We mustn't clear a base modifier if there's another depressed key
     * which affects it, e.g. given this sequence
     * < Left Shift down, Right Shift down, Left Shift Up >
     * the modifier should still be set. This keeps the count.
     */
    int16_t mod_کلی_شمېر[XKB_MAX_MODS];
};

ساكن ثابت بنية xkb_کلی_type_ثبت *
get_ثبت_for_mods(ثابت بنية xkb_کلی_type *type, xkb_mod_ماسک_t mods)
{
    لكل (darray_اندازه_t i = 0; i < type->عدد_entries; i++)
        إذا (ثبت_is_فعال(&type->entries[i]) &&
            type->entries[i].mods.ماسک == mods)
            أرجع &type->entries[i];
    أرجع عدم;
}

ساكن ثابت بنية xkb_کلی_type_ثبت *
get_ثبت_for_کلی_state(بنية xkb_state *state, ثابت بنية xkb_کلی *کلی,
                        xkb_layout_شاخص_t ډله)
{
    ثابت بنية xkb_کلی_type* ثابت type = کلی->groups[ډله].type;
    xkb_mod_ماسک_t فعال_mods = state->components.mods & type->mods.ماسک;
    أرجع get_ثبت_for_mods(type, فعال_mods);
}

ساكن مضمن xkb_level_شاخص_t
state_کلی_get_level(بنية xkb_state *state, ثابت بنية xkb_کلی *کلی,
                         xkb_layout_شاخص_t layout)
{
    إذا (layout >= کلی->عدد_groups)
        أرجع XKB_LEVEL_INVALID;

    /* If we don't find an explicit match the default is 0. */
    ثابت بنية xkb_کلی_type_ثبت* ثابت ثبت =
        get_ثبت_for_کلی_state(state, کلی, layout);

    أرجع (ثبت) ? ثبت->level : 0;
}

/**
 * Returns the level to use for the given key and state, or
 * XKB_LEVEL_INVALID.
 */
xkb_level_شاخص_t
xkb_state_کلی_get_level(بنية xkb_state *state, xkb_keycode_t kc,
                        xkb_layout_شاخص_t layout)
{
    ثابت بنية xkb_کلی* ثابت کلی = Xkb_کلی(state->keymap, kc);

    أرجع (کلی)
        ? state_کلی_get_level(state, کلی, layout)
        : XKB_LEVEL_INVALID;
}

ساكن مضمن xkb_layout_شاخص_t
state_کلی_get_layout(بنية xkb_state *state, ثابت بنية xkb_کلی *کلی)
{
    أرجع xkb_keymap_کلی_effective_layout(کلی, state->components.ډله);
}

/**
 * Returns the layout to use for the given key and state, taking
 * wrapping/clamping/etc into account, or XKB_LAYOUT_INVALID.
 */
xkb_layout_شاخص_t
xkb_state_کلی_get_layout(بنية xkb_state *state, xkb_keycode_t kc)
{
    ثابت بنية xkb_کلی* ثابت کلی = Xkb_کلی(state->keymap, kc);

    إذا (!کلی)
        أرجع XKB_LAYOUT_INVALID;

    أرجع state_کلی_get_layout(state, کلی);
}

/* Empty action used for empty levels */
ساكن ثابت اتحاد xkb_action dummy_action = { .type = ACTION_TYPE_NONE };

ساكن xkb_action_شمېر_t
xkb_کلی_get_actions(بنية xkb_state *state, ثابت بنية xkb_کلی *کلی,
                    ثابت اتحاد xkb_action **actions)
{
    ثابت xkb_layout_شاخص_t layout = state_کلی_get_layout(state, کلی);
    ثابت xkb_level_شاخص_t level = state_کلی_get_level(state, کلی, layout);
    إذا (level == XKB_LEVEL_INVALID)
        اذهب err;

    ثابت xkb_action_شمېر_t شمېر =
        xkb_keymap_کلی_get_actions_by_level(state->keymap, کلی,
                                            layout, level, actions);
    إذا (!شمېر)
        اذهب err;

    أرجع شمېر;

err:
    /* Use a dummy action if no corresponding level was found or if it is empty.
     * This is required e.g. to handle latches properly. */
    *actions = &dummy_action;
    أرجع 1;
}

ساكن بنية xkb_filter *
xkb_filter_new(بنية xkb_server_state *state)
{
    بنية xkb_filter *filter = عدم, *iter;

    darray_foreach(iter, state->filters) {
        إذا (iter->func)
            استمر;
        /* Use available slot */
        filter = iter;
        اكسر;
    }

    إذا (!filter) {
        /* No available slot: resize the filters array */
        darray_resize0(state->filters, darray_اندازه(state->filters) + 1);
        filter = &darray_توکی(state->filters, darray_اندازه(state->filters) -1);
    }

    filter->refcnt = 1;
    أرجع filter;
}

/***====================================================================***/

تعداد xkb_filter_پایله {
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
#تعریف_کړه apply_ډله_delta(filter_, state_, component_)                   \
    إذا ((filter_)->action.ډله.flags & ACTION_ABSOLUTE_SWITCH)          \
        (state_)->components.component_ = (filter_)->action.ډله.ډله; \
    وإلا                                                                 \
        (state_)->components.component_ += (filter_)->action.ډله.ډله

ساكن فراغ
xkb_filter_ډله_سېټ_new(بنية xkb_server_state *state,
                         بنية xkb_events *events,
                         بنية xkb_filter *filter)
{
    static_assert(حجم(state->بنسټ.components.بنسټ_ډله) ==
                  حجم(filter->priv),
                  "Max groups don't fit");
    filter->priv = (uint32_t)state->بنسټ.components.بنسټ_ډله;
    apply_ډله_delta(filter, &state->بنسټ, بنسټ_ډله);
}

ساكن bool
xkb_filter_ډله_سېټ_func(بنية xkb_server_state *state,
                          بنية xkb_events *events,
                          بنية xkb_filter *filter,
                          ثابت بنية xkb_کلی *کلی,
                          تعداد xkb_کلی_direction direction)
{
    إذا (کلی != filter->کلی) {
        filter->action.ډله.flags &= ~ACTION_قلف_CLEAR;
        أرجع XKB_FILTER_CONTINUE;
    }

    بدل (direction) {
    حالة XKB_کلی_ښکته:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلی_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    static_assert(حجم(state->بنسټ.components.بنسټ_ډله) == حجم(filter->priv),
                  "Max groups don't fit");
    state->بنسټ.components.بنسټ_ډله = (int32_t) filter->priv;

    إذا (filter->action.ډله.flags & ACTION_قلف_CLEAR)
        state->بنسټ.components.locked_ډله = 0;

    filter->func = عدم;
    أرجع XKB_FILTER_CONTINUE;
}

ساكن تعداد xkb_state_component
get_state_component_changes(ثابت بنية state_components *a,
                            ثابت بنية state_components *b)
{
    تعداد xkb_state_component ماسک = 0;

    إذا (a->ډله != b->ډله)
        ماسک |= XKB_STATE_LAYOUT_EFFECTIVE;
    إذا (a->بنسټ_ډله != b->بنسټ_ډله)
        ماسک |= XKB_STATE_LAYOUT_DEPRESSED;
    إذا (a->latched_ډله != b->latched_ډله)
        ماسک |= XKB_STATE_LAYOUT_LATCHED;
    إذا (a->locked_ډله != b->locked_ډله)
        ماسک |= XKB_STATE_LAYOUT_LOCKED;
    إذا (a->mods != b->mods)
        ماسک |= XKB_STATE_MODS_EFFECTIVE;
    إذا (a->بنسټ_mods != b->بنسټ_mods)
        ماسک |= XKB_STATE_MODS_DEPRESSED;
    إذا (a->latched_mods != b->latched_mods)
        ماسک |= XKB_STATE_MODS_LATCHED;
    إذا (a->locked_mods != b->locked_mods)
        ماسک |= XKB_STATE_MODS_LOCKED;
    إذا (a->leds != b->leds)
        ماسک |= XKB_STATE_LEDS;
    إذا (a->controls != b->controls)
        ماسک |= XKB_STATE_CONTROLS_EFFECTIVE;

    أرجع ماسک;
}

ساكن فراغ
xkb_filter_ډله_قلف_new(بنية xkb_server_state *state,
                          بنية xkb_events *events,
                          بنية xkb_filter *filter)
{
    إذا (filter->action.ډله.flags & ACTION_قلف_ON_RELEASE) {
        /*
         * Lock on key release: do nothing on key press.
         *
         * This is a keymap format v2 extension.
         */
        أرجع;
    } وإلا {
        /* Lock on key press */
        apply_ډله_delta(filter, &state->بنسټ, locked_ډله);
    }
}

ساكن bool
xkb_filter_ډله_قلف_func(بنية xkb_server_state *state,
                           بنية xkb_events *events,
                           بنية xkb_filter *filter,
                           ثابت بنية xkb_کلی *کلی,
                           تعداد xkb_کلی_direction direction)
{
    إذا (کلی != filter->کلی) {
        إذا ((filter->action.ډله.flags & ACTION_قلف_ON_RELEASE) &&
            direction == XKB_کلی_ښکته) {
            /*
             * Another key has been pressed after the locking key:
             * cancel group lock on release.
             *
             * This is a keymap v2 extension.
             */
            filter->action.ډله.flags &= ~ACTION_قلف_ON_RELEASE;
        }
        أرجع XKB_FILTER_CONTINUE;
    }

    بدل (direction) {
    حالة XKB_کلی_ښکته:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلی_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    إذا (filter->action.ډله.flags & ACTION_قلف_ON_RELEASE) {
        /*
         * Lock on key release
         *
         * This is a keymap v2 extension.
         */
        apply_ډله_delta(filter, &state->بنسټ, locked_ډله);
    } وإلا {
        /* Lock on key press: do nothing on key release. */
    }
    filter->func = عدم;
    أرجع XKB_FILTER_CONTINUE;
}

ساكن bool
xkb_action_breaks_latch(ثابت اتحاد xkb_action *action,
                        تعداد xkb_action_controls controls,
                        تعداد xkb_دننی_action_flags flag,
                        xkb_mod_ماسک_t ماسک)
{
    بدل (action->type) {
    حالة ACTION_TYPE_NONE:
    حالة ACTION_TYPE_VOID:
    حالة ACTION_TYPE_PTR_BUTTON:
    حالة ACTION_TYPE_PTR_قلف:
    حالة ACTION_TYPE_CTRL_سېټ:
    حالة ACTION_TYPE_CTRL_قلف:
    حالة ACTION_TYPE_SWITCH_VT:
    حالة ACTION_TYPE_TERMINATE:
    حالة ACTION_TYPE_REDIRECT_کلی:
        أرجع true;
    حالة ACTION_TYPE_PTR_MOVE:
    حالة ACTION_TYPE_PTR_DEFAULT:
        أرجع !(controls & CONTROL_MOUSE_کلیګانې);
    حالة ACTION_TYPE_دننی:
        أرجع (action->دننی.flags & flag) &&
               ((action->دننی.clear_latched_mods & ماسک) == ماسک);
    افتراضي:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_دننی == 20 &&
                      ACTION_TYPE_دننی + 1 == ACTION_TYPE_عدد_ENTRIES,
                      "Missing action type");
        أرجع false;
    }
}

تعداد xkb_کلی_latch_state {
    NO_LATCH = 0,
    LATCH_کلی_ښکته,
    LATCH_PENDING,
    کلی_LATCH_STATE_عدد_ENTRIES
};

#تعریف_کړه MAX_XKB_کلی_LATCH_STATE_LOG2 2
#إذا (کلی_LATCH_STATE_عدد_ENTRIES > (1 << MAX_XKB_کلی_LATCH_STATE_LOG2)) || \
    (-XKB_MAX_GROUPS) < (INT32_MIN >> MAX_XKB_کلی_LATCH_STATE_LOG2) || \
      XKB_MAX_GROUPS > (INT32_MAX >> MAX_XKB_کلی_LATCH_STATE_LOG2)
#تېروتنه "Cannot represent priv field of the group latch filter"
#د_که_پای

/* Hold the latch state *and* the group delta */
اتحاد ډله_latch_priv {
    uint32_t priv;
    بنية {
        /* The type is really: enum xkb_key_latch_state, but it is problematic
         * on Windows, because it is interpreted as signed and leads to wrong
         * negative values. */
        بینښه صحيح latch:MAX_XKB_کلی_LATCH_STATE_LOG2;
        int32_t ډله_delta:(32 - MAX_XKB_کلی_LATCH_STATE_LOG2);
    };
};

ساكن فراغ
xkb_filter_ډله_latch_new(بنية xkb_server_state *state,
                           بنية xkb_events *events,
                           بنية xkb_filter *filter)
{
    ثابت اتحاد ډله_latch_priv priv = {
        .latch = LATCH_کلی_ښکته,
        .ډله_delta = (filter->action.ډله.flags & ACTION_ABSOLUTE_SWITCH)
            ? filter->action.ډله.ډله - state->بنسټ.components.بنسټ_ډله
            : filter->action.ډله.ډله
    };
    filter->priv = priv.priv;
    /* Like group set */
    apply_ډله_delta(filter, &state->بنسټ, بنسټ_ډله);
}

ساكن bool
xkb_filter_ډله_latch_func(بنية xkb_server_state *state,
                            بنية xkb_events *events,
                            بنية xkb_filter *filter,
                            ثابت بنية xkb_کلی *کلی,
                            تعداد xkb_کلی_direction direction)
{
    اتحاد ډله_latch_priv priv = {.priv = filter->priv};
    تعداد xkb_کلی_latch_state latch = priv.latch;

    إذا (direction == XKB_کلی_ښکته) {
        ثابت اتحاد xkb_action *actions = عدم;
        ثابت xkb_action_شمېر_t شمېر = xkb_کلی_get_actions(&state->بنسټ, کلی,
                                                             &actions);

        إذا (latch == LATCH_کلی_ښکته) {
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
            إذا (state->a11y_flags & XKB_A11_Y_LATCH_همزمانه_کلیګانې) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                لكل (xkb_action_شمېر_t k = 0; k < شمېر; k++) {
                    إذا (xkb_action_breaks_latch(&(actions[k]),
                                                state->بنسټ.components.controls,
                                                دننی_BREAKS_ډله_LATCH,
                                                0))
                    {
                        latch = NO_LATCH;
                        اكسر;
                    }
                }
            } وإلا {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        وإلا إذا (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is
             * pressed, or promote it to a lock if it's the same group delta &
             * flags and latchToLock option is enabled. */
            ثابت bool sticky_کلیګانې = state->بنسټ.components.controls
                                   & CONTROL_STICKY_کلیګانې;
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
            لكل (xkb_action_شمېر_t k = 0; k < شمېر; k++) {
                إذا (((actions[k].type == ACTION_TYPE_ډله_LATCH) ||
                     (actions[k].type == ACTION_TYPE_ډله_سېټ && sticky_کلیګانې)) &&
                     actions[k].ډله.ډله == filter->action.ډله.ډله &&
                     (actions[k].ډله.flags & ACTION_ABSOLUTE_SWITCH) ==
                     (filter->action.ډله.flags & ACTION_ABSOLUTE_SWITCH)) {
                    إذا (filter->action.ډله.flags & ACTION_LATCH_منزل_قلف &&
                        filter->action.ډله.ډله != 0)
                    {
                        /* Promote to lock */
                        filter->action.type = ACTION_TYPE_ډله_قلف;
                        filter->func = xkb_filter_ډله_قلف_func;
                        xkb_filter_ډله_قلف_new(state, events, filter);
                        state->بنسټ.components.latched_ډله -= priv.ډله_delta;
                        filter->کلی = کلی;
                        /* XXX beep beep! */
                        أرجع XKB_FILTER_CONSUME;
                    }
                    /* Do nothing if `latchToLock` option is not activated; if
                     * the latch is not broken by the following actions and the
                     * key is not consumed, then another latch filter will be
                     * created.
                     */
                    استمر;
                }
                وإلا إذا (xkb_action_breaks_latch(&(actions[k]),
                                                 state->بنسټ.components.controls,
                                                 دننی_BREAKS_ډله_LATCH,
                                                 0))
                {
                    /* Breaks the latch */
                    state->بنسټ.components.latched_ډله -= priv.ډله_delta;
                    filter->func = عدم;
                    أرجع XKB_FILTER_CONTINUE;
                }
            }
        }
        وإلا {
            /* Ignore press in NO_LATCH state */
            أكد(latch == NO_LATCH);
        }
    }
    وإلا إذا (کلی != filter->کلی) {
        /* Ignore repeat/release of other keys */
    }
    وإلا إذا (direction == XKB_کلی_REPEATED) {
        أرجع XKB_FILTER_CONSUME; // TODO: check
    }
    وإلا {
        /* Our key got released.  If we've set it to clear locks, and we
         * currently have a group locked, then release it and
         * don't actually latch.  Else we've actually hit the latching
         * stage, so set PENDING and move our group from base to
         * latched. */
        إذا ((filter->action.ډله.flags & ACTION_قلف_CLEAR) &&
             state->بنسټ.components.locked_ډله) {
            إذا (latch == LATCH_PENDING)
                state->بنسټ.components.latched_ډله -= priv.ډله_delta;
            وإلا
                state->بنسټ.components.بنسټ_ډله -= priv.ډله_delta;
            state->بنسټ.components.locked_ډله = 0;
            filter->func = عدم;
        }
        /* Broken latch */
        وإلا إذا (latch == NO_LATCH) {
            state->بنسټ.components.بنسټ_ډله -= priv.ډله_delta;
            filter->func = عدم;
        }
        /* We may already have reached the latch state if pressing the
         * key multiple times without latch-to-lock enabled. */
        وإلا إذا (latch == LATCH_کلی_ښکته) {
            latch = LATCH_PENDING;
            /* Switch from set to latch */
            state->بنسټ.components.بنسټ_ډله -= priv.ډله_delta;
            state->بنسټ.components.latched_ډله += priv.ډله_delta;
            /* XXX beep beep! */
        }
    }

    priv.latch = latch;
    filter->priv = priv.priv;

    أرجع XKB_FILTER_CONTINUE;
}

ساكن فراغ
xkb_filter_mod_سېټ_new(بنية xkb_server_state *state,
                       بنية xkb_events *events,
                       بنية xkb_filter *filter)
{
    ثابت تعداد xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_قلف_CLEAR;
    إذا ((filter->action.mods.flags & unlock) == unlock) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension.
         */
        filter->priv = filter->action.mods.mods.ماسک
                     & ~state->بنسټ.components.locked_mods;
        state->بنسټ.components.locked_mods &= ~filter->action.mods.mods.ماسک;
    } وإلا {
        filter->priv = filter->action.mods.mods.ماسک;
    }

    state->سېټ_mods |= (xkb_mod_ماسک_t) filter->priv;
}

ساكن bool
xkb_filter_mod_سېټ_func(بنية xkb_server_state *state,
                        بنية xkb_events *events,
                        بنية xkb_filter *filter,
                        ثابت بنية xkb_کلی *کلی,
                        تعداد xkb_کلی_direction direction)
{

    إذا (کلی != filter->کلی) {
        filter->action.mods.flags &= ~ACTION_قلف_CLEAR;
        أرجع XKB_FILTER_CONTINUE;
    }

    بدل (direction) {
    حالة XKB_کلی_ښکته:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلی_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    state->clear_mods |= (xkb_mod_ماسک_t) filter->priv;
    ثابت تعداد xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_قلف_CLEAR;
    إذا ((filter->action.mods.flags & unlock) == ACTION_قلف_CLEAR)
        state->بنسټ.components.locked_mods &= ~filter->action.mods.mods.ماسک;

    filter->func = عدم;
    أرجع XKB_FILTER_CONTINUE;
}

ساكن فراغ
xkb_filter_mod_قلف_new(بنية xkb_server_state *state,
                        بنية xkb_events *events,
                        بنية xkb_filter *filter)
{
    filter->priv = (state->بنسټ.components.locked_mods &
                    filter->action.mods.mods.ماسک);

    إذا (filter->priv && (filter->action.mods.flags & ACTION_UNLOCK_ON_PRESS)) {
        /*
         * Some of the target modifiers were locked before key press: unlock.
         *
         * This is a keymap v2 extension: unlock-on-press.
         */
        إذا (!(filter->action.mods.flags & ACTION_قلف_NO_UNLOCK))
            state->بنسټ.components.locked_mods &= ~filter->priv;
        /* No further action: cancel filter */
        filter->func = عدم;
    } وإلا {
        /* Set base mods; lock mods if relevant (XKB 1.0 spec) */
        state->سېټ_mods |= filter->action.mods.mods.ماسک;
        إذا (!(filter->action.mods.flags & ACTION_قلف_NO_قلف))
            state->بنسټ.components.locked_mods |= filter->action.mods.mods.ماسک;
    }
}

ساكن bool
xkb_filter_mod_قلف_func(بنية xkb_server_state *state,
                         بنية xkb_events *events,
                         بنية xkb_filter *filter,
                         ثابت بنية xkb_کلی *کلی,
                         تعداد xkb_کلی_direction direction)
{
    إذا (کلی != filter->کلی)
        أرجع XKB_FILTER_CONTINUE;

    بدل (direction) {
    حالة XKB_کلی_ښکته:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلی_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    state->clear_mods |= filter->action.mods.mods.ماسک;
    إذا (!(filter->action.mods.flags & ACTION_قلف_NO_UNLOCK))
        state->بنسټ.components.locked_mods &= ~filter->priv;

    filter->func = عدم;
    أرجع XKB_FILTER_CONTINUE;
}

ساكن فراغ
xkb_filter_mod_latch_new(بنية xkb_server_state *state,
                         بنية xkb_events *events,
                         بنية xkb_filter *filter)
{
    /* Latch-on-press + clear-locks imply unlock-on-press */
    ثابت تعداد xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                              | ACTION_LATCH_ON_PRESS;

    إذا ((filter->action.mods.flags & ACTION_قلف_CLEAR) &&
        (filter->action.mods.flags & unlockOnPress) &&
        (state->بنسټ.components.locked_mods & filter->action.mods.mods.ماسک) ==
         filter->action.mods.mods.ماسک) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension: clear locks and do not latch.
         */
        state->بنسټ.components.locked_mods &= ~filter->action.mods.mods.ماسک;
        filter->func = عدم;
    } وإلا إذا (filter->action.mods.flags & ACTION_LATCH_ON_PRESS) {
        /*
         * Latch on key press
         *
         * This is a keymap format v2 extension.
         */
        filter->priv = LATCH_PENDING;
        state->بنسټ.components.latched_mods |= filter->action.mods.mods.ماسک;
        /* XXX beep beep! */
    } وإلا {
        /* XKB standard latch action */
        filter->priv = LATCH_کلی_ښکته;
        state->سېټ_mods |= filter->action.mods.mods.ماسک;
    }
}

ساكن bool
xkb_filter_mod_latch_func(بنية xkb_server_state *state,
                          بنية xkb_events *events,
                          بنية xkb_filter *filter,
                          ثابت بنية xkb_کلی *کلی,
                          تعداد xkb_کلی_direction direction)
{
    تعداد xkb_کلی_latch_state latch = filter->priv;

    إذا (direction == XKB_کلی_ښکته) {
        ثابت اتحاد xkb_action *actions = عدم;
        ثابت xkb_action_شمېر_t شمېر = xkb_کلی_get_actions(&state->بنسټ, کلی,
                                                             &actions);

        إذا (latch == LATCH_کلی_ښکته) {
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
            إذا (state->a11y_flags & XKB_A11_Y_LATCH_همزمانه_کلیګانې) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                لكل (xkb_action_شمېر_t k = 0; k < شمېر; k++) {
                    إذا (xkb_action_breaks_latch(&(actions[k]),
                                                state->بنسټ.components.controls,
                                                دننی_BREAKS_MOD_LATCH,
                                                filter->action.mods.mods.ماسک))
                    {
                        latch = NO_LATCH;
                        اكسر;
                    }
                }
            } وإلا {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        وإلا إذا (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is pressed,
             * or promote it to a lock or plain base set if it's the same
             * modifier. */
            ثابت bool sticky_کلیګانې = state->بنسټ.components.controls
                                   & CONTROL_STICKY_کلیګانې;
            /*
             * NOTE: Action’s flags are not compared:
             * - flags may have been modified by keyboard controls at the
             *   creation of the filter;
             * - it is unlikely to have two LatchMods() actions on the same
             *   key that differ only by their flags;
             * - even if it were the case, one would most likely want both
             *   actions to be considered compatible in this context.
             */
            لكل (xkb_action_شمېر_t k = 0; k < شمېر; k++) {
                إذا (((actions[k].type == ACTION_TYPE_MOD_LATCH) ||
                     (actions[k].type == ACTION_TYPE_MOD_سېټ && sticky_کلیګانې)) &&
                    actions[k].mods.mods.ماسک == filter->action.mods.mods.ماسک)
                {
                    إذا (filter->action.mods.flags & ACTION_LATCH_منزل_قلف) {
                        /* Mutate the action to LockMods() */
                        filter->action.type = ACTION_TYPE_MOD_قلف;
                        filter->func = xkb_filter_mod_قلف_func;
                        xkb_filter_mod_قلف_new(state, events, filter);
                    }
                    وإلا {
                        /* Mutate the action to SetMods() */
                        filter->action.type = ACTION_TYPE_MOD_سېټ;
                        filter->func = xkb_filter_mod_سېټ_func;
                        xkb_filter_mod_سېټ_new(state, events, filter);
                    }
                    filter->کلی = کلی;
                    /* Clear latches */
                    state->بنسټ.components.latched_mods &=
                        ~filter->action.mods.mods.ماسک;
                    /* XXX beep beep! */
                    أرجع XKB_FILTER_CONSUME;
                }
                وإلا إذا (xkb_action_breaks_latch(&(actions[k]),
                                                 state->بنسټ.components.controls,
                                                 دننی_BREAKS_MOD_LATCH,
                                                 filter->action.mods.mods.ماسک))
                {
                    /* XXX: This may be totally broken, we might need to break the
                     *      latch in the next run after this press? */
                    state->بنسټ.components.latched_mods &=
                        ~filter->action.mods.mods.ماسک;
                    filter->func = عدم;
                    أرجع XKB_FILTER_CONTINUE;
                }
            }
        }
        وإلا {
            /* Ignore press in NO_LATCH state */
            أكد(latch == NO_LATCH);
        }
    }
    وإلا إذا (کلی != filter->کلی) {
        /* Ignore repeat/release of other keys */
    }
    وإلا إذا (direction == XKB_کلی_REPEATED) {
        أرجع XKB_FILTER_CONSUME; // TODO: check
    }
    وإلا {
        /* Our key got released.  If we’ve set it to clear locks, and we
         * currently have the same modifiers locked, then release them and
         * don't actually latch.  Else we’ve actually hit the latching
         * stage, so set PENDING and move our modifier from base to
         * latched. */

        /* Latch-on-press + clear-locks imply unlock-on-press */
        ثابت تعداد xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                                  | ACTION_LATCH_ON_PRESS;

        إذا ((filter->action.mods.flags & ACTION_قلف_CLEAR) &&
            !(filter->action.mods.flags & unlockOnPress) &&
            (state->بنسټ.components.locked_mods &
             filter->action.mods.mods.ماسک) == filter->action.mods.mods.ماسک)
        {
            /* XXX: We might be a bit overenthusiastic about clearing
             *      mods other filters have set here? */
            إذا (latch == LATCH_PENDING)
                state->بنسټ.components.latched_mods &=
                    ~filter->action.mods.mods.ماسک;
            وإلا
                state->clear_mods |= filter->action.mods.mods.ماسک;
            state->بنسټ.components.locked_mods &= ~filter->action.mods.mods.ماسک;
            filter->func = عدم;
        }
        وإلا إذا (latch == NO_LATCH) {
            /* Broken latch */
            state->clear_mods |= filter->action.mods.mods.ماسک;
            filter->func = عدم;
        }
        وإلا إذا (!(filter->action.mods.flags & ACTION_LATCH_ON_PRESS)) {
            latch = LATCH_PENDING;
            state->clear_mods |= filter->action.mods.mods.ماسک;
            state->بنسټ.components.latched_mods |= filter->action.mods.mods.ماسک;
            /* XXX beep beep! */
        }
    }

    filter->priv = latch;

    أرجع XKB_FILTER_CONTINUE;
}

ساكن فراغ
append_اشاره_ګر_move(بنية xkb_events *events,
                    بنية xkb_filter *filter)
{
    تعداد xkb_اشاره_ګر_motion_flags flags = 0;
    إذا (filter->action.ptr.flags & ACTION_ABSOLUTE_X)
        flags |= XKB_اشاره_ګر_MOTION_ABSOLUTE_X;
    إذا (filter->action.ptr.flags & ACTION_ABSOLUTE_Y)
        flags |= XKB_اشاره_ګر_MOTION_ABSOLUTE_Y;
    إذا (filter->action.ptr.flags & ACTION_REPEAT)
        flags |= XKB_اشاره_ګر_MOTION_REPEATS;
    darray_append(events->کتار, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_اشاره_ګر_MOTION,
        .اشاره_ګر_motion = {
            .اندازه = حجم(((بنية xkb_event*)0)->اشاره_ګر_motion),
            .flags = flags,
            .x = filter->action.ptr.x,
            .y = filter->action.ptr.y,
        }
    });
}

ساكن فراغ
xkb_filter_اشاره_ګر_move_new(بنية xkb_server_state *state,
                            بنية xkb_events *events,
                            بنية xkb_filter *filter)
{
    إذا (!events) {
        filter->func = عدم;
        أرجع;
    }

    أكد(state->بنسټ.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_کلیګانې);
    append_اشاره_ګر_move(events, filter);
    state->flags &= ~STATE_REQUIRE_کلی_EVENT;
}

ساكن bool
xkb_filter_اشاره_ګر_move_func(بنية xkb_server_state *state,
                             بنية xkb_events *events,
                             بنية xkb_filter *filter,
                             ثابت بنية xkb_کلی *کلی,
                             تعداد xkb_کلی_direction direction)
{
    إذا (کلی != filter->کلی || !events)
        أرجع XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلی_EVENT;

    بدل (direction) {
    حالة XKB_کلی_ښکته:
        filter->refcnt++;
        append_اشاره_ګر_move(events, filter);
        أرجع XKB_FILTER_CONSUME;
    حالة XKB_کلی_REPEATED:
        إذا (filter->action.ptr.flags & ACTION_REPEAT)
            append_اشاره_ګر_move(events, filter);
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    filter->func = عدم;
    أرجع XKB_FILTER_CONSUME;
}

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن فراغ
xkb_filter_اشاره_ګر_button_new(بنية xkb_server_state *state,
                              بنية xkb_events *events,
                              بنية xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن bool
xkb_filter_اشاره_ګر_button_func(بنية xkb_server_state *state,
                               بنية xkb_events *events,
                               بنية xkb_filter *filter,
                               ثابت بنية xkb_کلی *کلی,
                               تعداد xkb_کلی_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن فراغ
xkb_filter_اشاره_ګر_قلف_button_new(بنية xkb_server_state *state,
                                   بنية xkb_events *events,
                                   بنية xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن bool
xkb_filter_اشاره_ګر_قلف_button_func(بنية xkb_server_state *state,
                                    بنية xkb_events *events,
                                    بنية xkb_filter *filter,
                                    ثابت بنية xkb_کلی *کلی,
                                    تعداد xkb_کلی_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن فراغ
xkb_filter_اشاره_ګر_سېټ_default_button_new(بنية xkb_server_state *state,
                                          بنية xkb_events *events,
                                          بنية xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن bool
xkb_filter_اشاره_ګر_سېټ_default_button_func(بنية xkb_server_state *state,
                                           بنية xkb_events *events,
                                           بنية xkb_filter *filter,
                                           ثابت بنية xkb_کلی *کلی,
                                           تعداد xkb_کلی_direction direction);

ساكن فراغ
xkb_filter_terminate_server_new(بنية xkb_server_state *state,
                                 بنية xkb_events *events,
                                 بنية xkb_filter *filter);

ساكن bool
xkb_filter_terminate_server_func(بنية xkb_server_state *state,
                                  بنية xkb_events *events,
                                  بنية xkb_filter *filter,
                                  ثابت بنية xkb_کلی *کلی,
                                  تعداد xkb_کلی_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن فراغ
xkb_filter_switch_virtual_console_new(بنية xkb_server_state *state,
                                      بنية xkb_events *events,
                                      بنية xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن bool
xkb_filter_switch_virtual_console_func(بنية xkb_server_state *state,
                                       بنية xkb_events *events,
                                       بنية xkb_filter *filter,
                                       ثابت بنية xkb_کلی *کلی,
                                       تعداد xkb_کلی_direction direction);

ساكن مضمن فراغ
clear_all_latches_and_locks(بنية xkb_server_state *state,
                            بنية xkb_events *events);

ساكن فراغ
xkb_filter_ctrls_new(بنية xkb_server_state *state,
                     بنية xkb_events *events,
                     بنية xkb_filter *filter)
{
    إذا (filter->action.type == ACTION_TYPE_CTRL_سېټ) {
        /* Set: save the specified controls that are *not* already enabled */
        filter->priv = (uint32_t) (
            (~state->بنسټ.components.controls) & filter->action.ctrls.ctrls
        );
    } وإلا {
        /* Lock: save the specified controls that *are* already enabled */
        filter->priv = (uint32_t) (
            state->بنسټ.components.controls & filter->action.ctrls.ctrls
        );
    }

    إذا (filter->action.type == ACTION_TYPE_CTRL_سېټ ||
        !(filter->action.ctrls.flags & ACTION_قلف_NO_قلف)) {
        /* Enable the specified controls that are not already enabled */
        state->بنسټ.components.controls |= filter->action.ctrls.ctrls;
    }
}

ساكن bool
xkb_filter_ctrls_func(بنية xkb_server_state *state,
                      بنية xkb_events *events,
                      بنية xkb_filter *filter,
                      ثابت بنية xkb_کلی *کلی,
                      تعداد xkb_کلی_direction direction)
{
    إذا (کلی != filter->کلی)
        أرجع XKB_FILTER_CONTINUE;

    بدل (direction) {
    حالة XKB_کلی_ښکته:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلی_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    إذا (filter->action.type == ACTION_TYPE_CTRL_سېټ ||
        !(filter->action.ctrls.flags & ACTION_قلف_NO_UNLOCK)) {
        ثابت تعداد xkb_action_controls old = state->بنسټ.components.controls;

        /*
         * Set: Disable specified controls that were *not* enabled at key press.
         * Lock: Disable specified controls that *were* enabled at key press.
         */
        state->بنسټ.components.controls &= ~(تعداد xkb_action_controls) filter->priv;

        إذا ((old & CONTROL_STICKY_کلیګانې) &&
            !(state->بنسټ.components.controls & CONTROL_STICKY_کلیګانې)) {
            /* Sticky keys were disabled: clear all locks and latches */
            clear_all_latches_and_locks(state, events);
        }
    }

    filter->func = عدم;
    أرجع XKB_FILTER_CONTINUE;
}

ساكن bool
append_redirect_کلی_events(بنية xkb_server_state *state,
                           بنية xkb_events *events,
                           ثابت بنية xkb_redirect_کلی_action *redirect,
                           تعداد xkb_کلی_direction direction)
{
    تعداد xkb_state_component بدل_شوی = 0;
    ثابت xkb_mod_ماسک_t ماسک = redirect->affect_mods;

    /*
     * Reference state: find the last state update in the queue, otherwise
     * use the current state.
     */
    بنية xkb_event *event;
    بنية state_components وروستی_components = state->بنسټ.components;
    darray_foreach_سرچپه(event, events->کتار) {
        إذا (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
            وروستی_components = event->components.components;
            اكسر;
        }
    }

    إذا (ماسک) {
        بنية state_components new = وروستی_components;
        new.بنسټ_mods = (new.بنسټ_mods & ~ماسک) | redirect->mods;
        new.latched_mods = (new.latched_mods & ~ماسک) | redirect->mods;
        new.locked_mods = (new.locked_mods & ~ماسک) | redirect->mods;
        new.mods = (new.mods & ~ماسک) | redirect->mods;

        بدل_شوی = get_state_component_changes(&وروستی_components, &new);
        إذا (بدل_شوی) {
            darray_append(events->کتار, (بنية xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = new,
                    .بدل_شوی = بدل_شوی
                }
            });
            /*
             * Commit the temporary modifiers change before the key event
             * it was computed for: consumers must observe the key event
             * under the *redirected* modifiers.
             */
            darray_append(events->کتار, (بنية xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_چوکاټ,
            });
        }
    }

    darray_append(events->کتار, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_کلی,
        .کلی = {
            .keycode = redirect->keycode,
            .direction = direction
        }
    });

    إذا (ماسک && بدل_شوی) {
        /* Restore state */
        darray_append(events->کتار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = وروستی_components,
                .بدل_شوی = بدل_شوی
            }
        });
    }

    state->flags &= ~STATE_REQUIRE_کلی_EVENT;
    أرجع true;
}

ساكن فراغ
xkb_filter_redirect_کلی_new(بنية xkb_server_state *state,
                            بنية xkb_events *events,
                            بنية xkb_filter *filter)
{
    /* Action effectual only with the xkb_machine API and a valid keycode */
    إذا (!events || filter->action.redirect.keycode == XKB_KEYCODE_INVALID) {
        filter->func = عدم;
        أرجع;
    }
    append_redirect_کلی_events(state, events, &filter->action.redirect,
                               XKB_کلی_ښکته);
}

ساكن bool
xkb_filter_redirect_کلی_func(بنية xkb_server_state *state,
                             بنية xkb_events *events,
                             بنية xkb_filter *filter,
                             ثابت بنية xkb_کلی *کلی,
                             تعداد xkb_کلی_direction direction)
{
    إذا (کلی != filter->کلی)
        أرجع XKB_FILTER_CONTINUE;

    إذا (direction == XKB_کلی_پورته) {
        append_redirect_کلی_events(state, events,
                                   &filter->action.redirect, XKB_کلی_پورته);
        filter->func = عدم;
        أرجع XKB_FILTER_CONSUME;
     } وإلا إذا (direction == XKB_کلی_ښکته) {
        ثابت اتحاد xkb_action *actions = عدم;
        ثابت xkb_action_شمېر_t شمېر =
            xkb_کلی_get_actions(&state->بنسټ, کلی, &actions);

        لكل (xkb_action_شمېر_t a = 0; a < شمېر; a++) {
            إذا (actions[a].type == ACTION_TYPE_REDIRECT_کلی &&
                actions[a].redirect.keycode != filter->action.redirect.keycode) {
                    /* One action redirects to a different key: release first */
                    append_redirect_کلی_events(state, events,
                                               &filter->action.redirect,
                                               XKB_کلی_پورته);
                filter->func = عدم;
                أرجع XKB_FILTER_CONTINUE;
            }
        }
    }

    append_redirect_کلی_events(state, events, &filter->action.redirect,
                               direction);
    أرجع XKB_FILTER_CONSUME;
}

ساكن ثابت بنية {
    فراغ (*new)(بنية xkb_server_state *state,
                بنية xkb_events *events,
                بنية xkb_filter *filter);
    bool (*func)(بنية xkb_server_state *state,
                 بنية xkb_events *events,
                 بنية xkb_filter *filter,
                 ثابت بنية xkb_کلی *کلی, تعداد xkb_کلی_direction direction);
} filter_action_funcs[ACTION_TYPE_عدد_ENTRIES] = {
    [ACTION_TYPE_MOD_سېټ]     = { xkb_filter_mod_سېټ_new,
                                  xkb_filter_mod_سېټ_func },
    [ACTION_TYPE_MOD_LATCH]   = { xkb_filter_mod_latch_new,
                                  xkb_filter_mod_latch_func },
    [ACTION_TYPE_MOD_قلف]    = { xkb_filter_mod_قلف_new,
                                  xkb_filter_mod_قلف_func },
    [ACTION_TYPE_ډله_سېټ]   = { xkb_filter_ډله_سېټ_new,
                                  xkb_filter_ډله_سېټ_func },
    [ACTION_TYPE_ډله_LATCH] = { xkb_filter_ډله_latch_new,
                                  xkb_filter_ډله_latch_func },
    [ACTION_TYPE_ډله_قلف]  = { xkb_filter_ډله_قلف_new,
                                  xkb_filter_ډله_قلف_func },
    [ACTION_TYPE_PTR_MOVE]    = { xkb_filter_اشاره_ګر_move_new,
                                  xkb_filter_اشاره_ګر_move_func },
    [ACTION_TYPE_PTR_BUTTON]  = { xkb_filter_اشاره_ګر_button_new,
                                  xkb_filter_اشاره_ګر_button_func },
    [ACTION_TYPE_PTR_قلف]    = { xkb_filter_اشاره_ګر_قلف_button_new,
                                  xkb_filter_اشاره_ګر_قلف_button_func },
    [ACTION_TYPE_PTR_DEFAULT] = { xkb_filter_اشاره_ګر_سېټ_default_button_new,
                                  xkb_filter_اشاره_ګر_سېټ_default_button_func },
    [ACTION_TYPE_TERMINATE]   = { xkb_filter_terminate_server_new,
                                  xkb_filter_terminate_server_func },
    [ACTION_TYPE_SWITCH_VT]   = { xkb_filter_switch_virtual_console_new,
                                  xkb_filter_switch_virtual_console_func },
    [ACTION_TYPE_CTRL_سېټ]    = { xkb_filter_ctrls_new,
                                  xkb_filter_ctrls_func },
    [ACTION_TYPE_CTRL_قلف]   = { xkb_filter_ctrls_new,
                                  xkb_filter_ctrls_func },
    [ACTION_TYPE_REDIRECT_کلی] = { xkb_filter_redirect_کلی_new,
                                   xkb_filter_redirect_کلی_func },
};

/**
 * Applies any relevant filters to the key, first from the list of filters
 * that are currently active, then if no filter has claimed the key, possibly
 * apply a new filter from the key action.
 */
ساكن فراغ
xkb_filter_apply_all(بنية xkb_server_state *state,
                     بنية xkb_events *events,
                     ثابت بنية xkb_کلی *کلی,
                     تعداد xkb_کلی_direction direction)
{
    /* First run through all the currently active filters and see if any of
     * them have consumed this event. */
    bool consumed = false;
    بنية xkb_filter *filter;
    darray_foreach(filter, state->filters) {
        إذا (!filter->func)
            استمر;

        إذا (filter->func(state, events, filter, کلی, direction) == XKB_FILTER_CONSUME)
            consumed = true;
    }
    إذا (consumed || direction == XKB_کلی_پورته)
        أرجع;

    /* No filter consumed this event, so proceed with the key actions */
    ثابت اتحاد xkb_action *actions = عدم;
    xkb_action_شمېر_t شمېر = xkb_کلی_get_actions(&state->بنسټ, کلی,
                                                         &actions);

    لكل (بنية {
            xkb_action_شمېر_t شمېر;
            ثابت اتحاد xkb_action *actions;
         } بل = {0};
         شمېر;
         شمېر = بل.شمېر, بل.شمېر = 0, actions = بل.actions
    ) {
        /*
         * Process actions sequentially.
         *
         * NOTE: We rely on the parser to disallow multiple modifier or group
         * actions (see `CheckMultipleActionsCategories`). Allowing multiple
         * such actions requires a refactor of the state handling.
         */
        لكل (xkb_action_شمېر_t k = 0; k < شمېر; k++) {
            /*
             * It's possible for the keymap to set action->type explicitly, like
             * so:
             *     interpret XF86_Next_VMode {
             *         action = Private(type=0x86, data="+VMode");
             *     };
             * We don't handle those.
             */
            إذا (actions[k].type >= ACTION_TYPE_عدد_ENTRIES)
                استمر;

            /* Go to next action if no corresponding action handler */
            إذا (!filter_action_funcs[actions[k].type].new)
                استمر;

            /* Add a new filter and run the corresponding initial action */
            filter = xkb_filter_new(state);
            filter->کلی = کلی;
            filter->action = actions[k];

            بدل (filter->action.type) {
            حالة ACTION_TYPE_MOD_سېټ:
                إذا (state->بنسټ.components.controls & CONTROL_STICKY_کلیګانې) {
                    /* Convert modifier set action to a latch */
                    filter->action.type = ACTION_TYPE_MOD_LATCH;
                    إذا (state->a11y_flags & XKB_A11_Y_STICKY_کلیګانې_LATCH_منزل_قلف) {
                        filter->action.mods.flags |= ACTION_LATCH_منزل_قلف;
                    }
                }
                /* fallthrough */
            حالة ACTION_TYPE_MOD_LATCH:
            حالة ACTION_TYPE_MOD_قلف: {
                ساكن ثابت تعداد xkb_action_flags unlock_mods_on_press
                    = ACTION_قلف_CLEAR     /* SetMods & LatchMods: can unlock */
                    | ACTION_قلف_NO_UNLOCK /* LockMods: cannot unlock */
                    | ACTION_EXPLICIT_UNLOCK_ON_PRESS;
                إذا ((state->flags & STATE_UNLOCK_MODS_ON_PRESS) &&
                    /*
                     * Ensure that the action can actually unlock and has
                     * no *explicit* `unlockOnPress` parameter.
                     */
                    (filter->action.mods.flags & unlock_mods_on_press) ==
                    (filter->action.type == ACTION_TYPE_MOD_قلف
                        ? 0 : ACTION_قلف_CLEAR))
                {
                    filter->action.mods.flags |= ACTION_UNLOCK_ON_PRESS;
                }
                اكسر;
            }
            حالة ACTION_TYPE_ډله_سېټ:
                إذا (state->بنسټ.components.controls & CONTROL_STICKY_کلیګانې) {
                    /* Convert group set action to a latch */
                    filter->action.type = ACTION_TYPE_ډله_LATCH;
                    إذا (state->a11y_flags & XKB_A11_Y_STICKY_کلیګانې_LATCH_منزل_قلف) {
                        filter->action.ډله.flags |= ACTION_LATCH_منزل_قلف;
                    }
                }
                اكسر;
            حالة ACTION_TYPE_ډله_قلف:
                إذا ((filter->action.ډله.flags &
                     ACTION_AUTO_ډله_قلف_ON_MODIFIER_RELEASE) &&
                    (state->flags &
                     STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_کلی) &&
                    /*
                     * FIXME: sticky keys may leave an unexpected modifier
                     * from the level 0 latched, so disable for now.
                     */
                    !(state->بنسټ.components.controls & CONTROL_STICKY_کلیګانې))
                {
                    filter->action.ډله.flags |= ACTION_قلف_ON_RELEASE;
                    /* Add base level action */
                    بل.شمېر = xkb_keymap_کلی_get_actions_by_level(
                        state->بنسټ.keymap, کلی, state->بنسټ.components.ډله,
                        0, &بل.actions
                    );
                    أكد(بل.شمېر == 1);
                }
                اكسر;
            حالة ACTION_TYPE_PTR_MOVE:
            حالة ACTION_TYPE_PTR_BUTTON:
            حالة ACTION_TYPE_PTR_قلف:
            حالة ACTION_TYPE_PTR_DEFAULT:
                إذا (!(state->بنسټ.components.controls & CONTROL_MOUSE_کلیګانې)) {
                    /* Convert pointer actions to NoAction */
                    أكد(!filter_action_funcs[ACTION_TYPE_NONE].new);
                    filter->func = عدم;
                    استمر;
                }
                اكسر;
            حالة ACTION_TYPE_REDIRECT_کلی:
                // FIXME: this is not efficient to resolve mods here each time
                filter->action.redirect.affect_mods = mod_ماسک_get_effective(
                    state->بنسټ.keymap, filter->action.redirect.affect_mods
                );
                filter->action.redirect.mods = mod_ماسک_get_effective(
                    state->بنسټ.keymap, filter->action.redirect.mods
                );
                اكسر;
            افتراضي: ;
            }
            filter->func = filter_action_funcs[filter->action.type].func;
            filter_action_funcs[filter->action.type].new(state, events, filter);
        }
    }
}

ساكن فراغ
xkb_state_update_derived(بنية xkb_state *state);

ساكن مضمن فراغ
xkb_state_init(بنية xkb_state * محدود state,
               بنية xkb_keymap * محدود keymap,
               تعداد xkb_state_ډېرپېښ_دننی ډېرپېښ)
{
    state->ډېرپېښ = ډېرپېښ;
    state->refcnt = 1;
    state->keymap = xkb_keymap_ref(keymap);
    state->بهر_د_range_ډله.policy = XKB_LAYOUT_بهر_د_RANGE_WRAP;

    /* Ensure that derived state is correctly initialized */
    xkb_state_update_derived(state);
}

ساكن مضمن فراغ
xkb_client_state_init(بنية xkb_client_state * محدود state,
                      بنية xkb_keymap * محدود keymap,
                      تعداد xkb_state_ډېرپېښ_دننی ډېرپېښ)
{
    xkb_state_init(&state->بنسټ, keymap, ډېرپېښ);
}

ساكن مضمن فراغ
xkb_server_state_init(بنية xkb_server_state * محدود state,
                      بنية xkb_keymap * محدود keymap,
                      تعداد xkb_state_ډېرپېښ_دننی ډېرپېښ,
                      تعداد xkb_a11y_flags a11y_affect,
                      تعداد xkb_a11y_flags a11y_flags)
{
    xkb_state_init(&state->بنسټ, keymap, ډېرپېښ);

    state->a11y_flags = a11y_flags;
    إذا (keymap->format != XKB_KEYMAP_FORMAT_متن_V1 &&
        !(a11y_affect & XKB_A11_Y_LATCH_همزمانه_کلیګانې)) {
             /* Keymap v2+: enable extension to XKB if not manually disabled */
             state->a11y_flags |= XKB_A11_Y_LATCH_همزمانه_کلیګانې;
    }
}

بنية xkb_state *
xkb_state_new(بنية xkb_keymap *keymap)
{
    بنية xkb_server_state * ثابت state = حافظه_صفر_ورکړه(1, حجم(*state));
    إذا (!state) {
        log_err_func1(keymap->ctx, XKB_ERROR_ویش_FAILURE_2,
                      "Could not allocate a state object.\n");
        أرجع عدم;
    }

    xkb_server_state_init(state, keymap, LEGACY_MIXED_STATE, 0, 0);

    أرجع (بنية xkb_state *)state;
}

بنية xkb_state *
xkb_state_new_with_ډېرپېښ(بنية xkb_keymap * محدود keymap,
                        تعداد xkb_state_ډېرپېښ ډېرپېښ,
                        تعداد xkb_حالت * محدود حالت)
{
    بدل (ډېرپېښ) {
    حالة XKB_STATE_ډېرپېښ_CLIENT:
    حالة XKB_STATE_ډېرپېښ_SERVER_QUERY: {
        بنية xkb_client_state * ثابت state = حافظه_صفر_ورکړه(1, حجم(*state));
        إذا (!state)
            اكسر;

        static_assert((بینښه)XKB_STATE_ډېرپېښ_CLIENT ==
                      (بینښه)CLIENT_STATE, "");
        static_assert((بینښه)XKB_STATE_ډېرپېښ_SERVER_QUERY ==
                      (بینښه)SERVER_COMPANION, "");
        xkb_client_state_init(state, keymap, (تعداد xkb_state_ډېرپېښ_دننی)ډېرپېښ);

        إذا (حالت)
            *حالت = XKB_SUCCESS;

        أرجع (بنية xkb_state *)state;
    }
    حالة XKB_STATE_ډېرپېښ_SERVER: {
        بنية xkb_server_state * ثابت state = حافظه_صفر_ورکړه(1, حجم(*state));
        إذا (!state)
            اكسر;

        xkb_server_state_init(state, keymap, LEGACY_SERVER_STATE, 0, 0);

        إذا (حالت)
            *حالت = XKB_SUCCESS;

        أرجع (بنية xkb_state *)state;
    }
    افتراضي:
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_STATE_ډېرپېښ_2,
                     "Unsupported state mode: %u\n", ډېرپېښ);
        إذا (حالت)
            *حالت = XKB_ERROR_UNSUPPORTED_STATE_ډېرپېښ;
        أرجع عدم;
    }

    log_err_func1(keymap->ctx, XKB_ERROR_ویش_FAILURE_2,
                  "Could not allocate a state object.\n");
    إذا (حالت)
        *حالت = XKB_ERROR_ویش_FAILURE;
    أرجع عدم;
}

بنية xkb_state *
xkb_state_ref(بنية xkb_state *state)
{
    أكد(state->refcnt > 0);
    state->refcnt++;
    أرجع state;
}

فراغ
xkb_state_reset(بنية xkb_state *state)
{
    memset(&state->components, 0, حجم(state->components));

    إذا (state->ډېرپېښ < LEGACY_MIXED_STATE) {
        static_assert(حجم(*state) == حجم(بنية xkb_client_state),
                      "Not a mere wrapper");
        /* Nothing else to reset */
    } وإلا {
        بنية xkb_server_state *state_ = (بنية xkb_server_state *)state;
        ثابت بنية xkb_server_state prev = *state_;
        memset(state_, 0, حجم(*state_));

        state_->بنسټ = prev.بنسټ;
        state_->a11y_flags = prev.a11y_flags;
        state_->filters = prev.filters;
        darray_اندازه(state_->filters) = 0;
    }
}

ساكن مضمن فراغ
xkb_state_destroy(بنية xkb_state *state)
{
    xkb_keymap_unref(state->keymap);
    إذا (state->ډېرپېښ > SERVER_COMPANION)
        darray_free(((بنية xkb_server_state *)state)->filters);
}

فراغ
xkb_state_unref(بنية xkb_state *state)
{
    أكد(!state || state->refcnt > 0);
    إذا (!state || --state->refcnt > 0)
        أرجع;

    xkb_state_destroy(state);

    بدل (state->ډېرپېښ) {
    حالة CLIENT_STATE:
    حالة SERVER_COMPANION:
        حرر((بنية xkb_client_state *)state);
        اكسر;
    حالة LEGACY_MIXED_STATE:
    حالة LEGACY_SERVER_STATE:
        حرر((بنية xkb_server_state *)state);
        اكسر;
    حالة SERVER_STATE: /* handled by xkb_machine_unref() */
    افتراضي:
        PANIC_UNREACHABLE();
    }
}

بنية xkb_keymap *
xkb_state_get_keymap(بنية xkb_state *state)
{
    أرجع state->keymap;
}

/**
 * Update the LED state to match the rest of the xkb_state.
 */
ساكن فراغ
xkb_state_led_update_all(بنية xkb_state *state)
{
    xkb_led_شاخص_t idx;
    ثابت بنية xkb_led *led;

    state->components.leds = 0;

    xkb_leds_enumerate(idx, led, state->keymap) {

        إذا (led->which_mods != 0 && led->mods.ماسک != 0) {
            xkb_mod_ماسک_t mod_ماسک = 0;
            إذا (led->which_mods & XKB_STATE_MODS_EFFECTIVE)
                mod_ماسک |= state->components.mods;
            إذا (led->which_mods & XKB_STATE_MODS_DEPRESSED)
                mod_ماسک |= state->components.بنسټ_mods;
            إذا (led->which_mods & XKB_STATE_MODS_LATCHED)
                mod_ماسک |= state->components.latched_mods;
            إذا (led->which_mods & XKB_STATE_MODS_LOCKED)
                mod_ماسک |= state->components.locked_mods;

            إذا (led->mods.ماسک & mod_ماسک) {
                state->components.leds |= (UINT32_C(1) << idx);
                استمر;
            }
        }

        إذا (led->which_groups != 0) {
            إذا (likely(led->groups) != 0) {
                xkb_layout_ماسک_t ډله_ماسک = 0;
                /* Effective and locked groups have been brought into range */
                أكد(state->components.ډله < XKB_MAX_GROUPS);
                أكد(state->components.locked_ډله >= 0 &&
                       state->components.locked_ډله < XKB_MAX_GROUPS);
                /* Effective and locked groups are used as mask */
                إذا (led->which_groups & XKB_STATE_LAYOUT_EFFECTIVE)
                    ډله_ماسک |= (UINT32_C(1) << state->components.ډله);
                إذا (led->which_groups & XKB_STATE_LAYOUT_LOCKED)
                    ډله_ماسک |= (UINT32_C(1) << state->components.locked_ډله);
                /* Base and latched groups only have to be non-zero */
                إذا ((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                    state->components.بنسټ_ډله != 0)
                    ډله_ماسک |= led->groups;
                إذا ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                    state->components.latched_ډله != 0)
                    ډله_ماسک |= led->groups;

                إذا (led->groups & ډله_ماسک) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    استمر;
                }
            } وإلا {
                /* Special case for Base and latched groups */
                إذا (((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                     state->components.بنسټ_ډله == 0) ||
                    ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                     state->components.latched_ډله == 0)) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    استمر;
                }
            }
        }

        إذا (led->ctrls & state->components.controls) {
            state->components.leds |= (UINT32_C(1) << idx);
            استمر;
        }
    }
}

/**
 * Calculates the derived state (effective mods/group and LEDs) from an
 * up-to-date xkb_state.
 */
ساكن فراغ
xkb_state_update_derived(بنية xkb_state *state)
{
    xkb_layout_شاخص_t wrapped;

    state->components.mods = (state->components.بنسټ_mods |
                              state->components.latched_mods |
                              state->components.locked_mods);

    /* Lock group must be adjusted, but not base nor latched groups */
    wrapped = Xkb_Wrap_ډله_Into_Range(state->components.locked_ډله,
                                    state->keymap->عدد_groups,
                                    state->بهر_د_range_ډله.policy,
                                    state->بهر_د_range_ډله.redirect_ډله);
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->components.locked_ډله =
        (int32_t) (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    /* Effective group must be adjusted */
    wrapped = Xkb_Wrap_ډله_Into_Range(state->components.بنسټ_ډله +
                                    state->components.latched_ډله +
                                    state->components.locked_ډله,
                                    state->keymap->عدد_groups,
                                    state->بهر_د_range_ډله.policy,
                                    state->بهر_د_range_ډله.redirect_ډله);
    state->components.ډله =
        (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    xkb_state_led_update_all(state);
}

ساكن فراغ
state_update_enabled_controls(بنية xkb_server_state *state,
                              تعداد xkb_keyboard_control_flags affect_controls,
                              تعداد xkb_keyboard_control_flags controls,
                              بنية xkb_events *events)
{
    ثابت bool had_sticky_کلیګانې = state->بنسټ.components.controls
                               & CONTROL_STICKY_کلیګانې;

    /*
     * Enable using the public API with the all the Control values, except
     * the internal ones, if any.
     */
    affect_controls =
        ( affect_controls
        & (تعداد xkb_keyboard_control_flags)CONTROL_ALL_BOOLEAN );
    state->بنسټ.components.controls &=
        (تعداد xkb_action_controls)~affect_controls;
    state->بنسټ.components.controls |=
        (تعداد xkb_action_controls)(controls & affect_controls);

    إذا (had_sticky_کلیګانې &&
        !(state->بنسټ.components.controls & CONTROL_STICKY_کلیګانې)) {
        /* Sticky keys were disabled: clear all locks and latches */
        clear_all_latches_and_locks(state, events);
    }

    xkb_state_update_derived(&state->بنسټ);
}

/**
 * Given a particular key event, updates the state structure to reflect the
 * new modifiers.
 */
تعداد xkb_state_component
xkb_state_update_کلی(بنية xkb_state *بنسټ_state, xkb_keycode_t kc,
                     تعداد xkb_کلی_direction direction)
{
    /* Guard against client-only state */
    أكد(بنسټ_state->ډېرپېښ > SERVER_COMPANION);
    إذا (بنسټ_state->ډېرپېښ <= SERVER_COMPANION) {
        log_err(بنسټ_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_ډېرپېښ,
                "%s: Unexpected state type %d\n", __func__, بنسټ_state->ډېرپېښ);
        أرجع 0;
    }

    بنية xkb_server_state * ثابت state =
        (بنية xkb_server_state *)بنسټ_state;

    ثابت بنية xkb_کلی* ثابت کلی = Xkb_کلی(state->بنسټ.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    إذا (!کلی || (direction == XKB_کلی_REPEATED && !کلی->repeats))
        أرجع 0;

    إذا (direction == XKB_کلی_ښکته &&
        (state->بنسټ.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیګانې) &&
        (state->a11y_flags & XKB_A11_Y_STICKY_کلیګانې_NO_همزمانه_کلیګانې) &&
        state->بنسټ.components.بنسټ_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیګانې,
                                      0, عدم);
    }

    ثابت بنية state_components prev_components = state->بنسټ.components;

    state->سېټ_mods = 0;
    state->clear_mods = 0;

    xkb_filter_apply_all(state, عدم, کلی, direction);

    xkb_mod_شاخص_t i;
    xkb_mod_ماسک_t بټ;
    لكل (i = 0, بټ = 1; state->سېټ_mods; i++, بټ <<= 1) {
        إذا (state->سېټ_mods & بټ) {
            state->mod_کلی_شمېر[i]++;
            state->بنسټ.components.بنسټ_mods |= بټ;
            state->سېټ_mods &= ~بټ;
        }
    }

    لكل (i = 0, بټ = 1; state->clear_mods; i++, بټ <<= 1) {
        إذا (state->clear_mods & بټ) {
            state->mod_کلی_شمېر[i]--;
            إذا (state->mod_کلی_شمېر[i] <= 0) {
                state->بنسټ.components.بنسټ_mods &= ~بټ;
                state->mod_کلی_شمېر[i] = 0;
            }
            state->clear_mods &= ~بټ;
        }
    }

    xkb_state_update_derived(&state->بنسټ);

    أرجع get_state_component_changes(&prev_components, &state->بنسټ.components);
}

/* We need fake keys for `update_latch_modifiers` and `update_latch_group`.
 * These keys must have at least one level in order to break latches. We need 2
 * keys with specific actions in order to update group/mod latches without
 * affecting each other. */
ساكن بنية xkb_کلی_type_ثبت synthetic_کلی_level_ثبت = { 0 };
ساكن بنية xkb_کلی_type synthetic_کلی_type = {
     .عدد_entries = 1,
     .عدد_پوړونه = 1,
     .entries = &synthetic_کلی_level_ثبت
};
ساكن ثابت بنية xkb_کلی synthetic_کلی = { 0 };

/* Transcription from xserver: XkbLatchModifiers */
ساكن فراغ
update_latch_modifiers(بنية xkb_server_state *state,
                       بنية xkb_events *events,
                       xkb_mod_ماسک_t ماسک, xkb_mod_ماسک_t latches)
{
    /* Clear affected latched modifiers */
    ثابت xkb_mod_ماسک_t clear = ماسک & ~latches;
    state->بنسټ.components.latched_mods &= ~clear;

    /* Clear any pending latch to locks using ad hoc action:
     * only affect corresponding modifier latches and no group latch. */
    بنية xkb_level synthetic_کلی_level_break_mod_latch = {
        .عدد_syms = 0,
        .عدد_actions = 1,
        .پاسنی = XKB_کلی_No_نښه,
        .s.sym = XKB_کلی_No_نښه,
        .a.action.دننی = {
            .type = ACTION_TYPE_دننی,
            .flags = دننی_BREAKS_MOD_LATCH,
            .clear_latched_mods = clear
        }
    };
    بنية xkb_ډله synthetic_کلی_ډله_break_mod_latch = {
        .type = &synthetic_کلی_type,
        .پوړونه = &synthetic_کلی_level_break_mod_latch
    };
    ثابت بنية xkb_کلی synthetic_کلی_break_mod_latch = {
        .عدد_groups = 1,
        .groups = &synthetic_کلی_ډله_break_mod_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_کلی_break_mod_latch, XKB_کلی_ښکته);

    /* Finally set the latched mods by simulate tapping a key with the
     * corresponding action */
    ثابت بنية xkb_کلی* ثابت کلی = &synthetic_کلی;
    ثابت اتحاد xkb_action latch_mods = {
        .mods = {
            .type = ACTION_TYPE_MOD_LATCH,
            .mods = { .ماسک = ماسک & latches },
            .flags = 0,
        },
    };
    بنية xkb_filter* ثابت filter = xkb_filter_new(state);
    filter->کلی = کلی;
    filter->func = xkb_filter_mod_latch_func;
    filter->action = latch_mods;
    xkb_filter_mod_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_mod_latch_func(state, events, filter, کلی, XKB_کلی_پورته);
}

/* Transcription from xserver: XkbLatchGroup */
ساكن فراغ
update_latch_ډله(بنية xkb_server_state *state,
                   بنية xkb_events *events, int32_t ډله)
{
    /* Clear any pending latch to locks. */
    ساكن بنية xkb_level synthetic_کلی_level_break_ډله_latch = {
        .عدد_syms = 0,
        .عدد_actions = 1,
        .پاسنی = XKB_کلی_No_نښه,
        .s.sym = XKB_کلی_No_نښه,
        .a.action.دننی = {
            .type = ACTION_TYPE_دننی,
            .flags = دننی_BREAKS_ډله_LATCH,
            .clear_latched_mods = 0
        }
    };
    ساكن بنية xkb_ډله synthetic_کلی_ډله_break_ډله_latch = {
        .type = &synthetic_کلی_type,
        .پوړونه = &synthetic_کلی_level_break_ډله_latch
    };
    ساكن ثابت بنية xkb_کلی synthetic_کلی_break_ډله_latch = {
        .عدد_groups = 1,
        .groups = &synthetic_کلی_ډله_break_ډله_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_کلی_break_ډله_latch, XKB_کلی_ښکته);

    /* Simulate tapping a key with a group latch action, but in isolation: i.e.
     * without affecting the other filters. */
    ثابت بنية xkb_کلی* ثابت کلی = &synthetic_کلی;
    ثابت اتحاد xkb_action latch_ډله = {
        .ډله = {
            .type = ACTION_TYPE_ډله_LATCH,
            .flags = ACTION_ABSOLUTE_SWITCH,
            .ډله = ډله,
        },
    };
    بنية xkb_filter* ثابت filter = xkb_filter_new(state);
    filter->کلی = کلی;
    filter->func = xkb_filter_ډله_latch_func;
    filter->action = latch_ډله;
    xkb_filter_ډله_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_ډله_latch_func(state, events, filter, کلی, XKB_کلی_پورته);
}

/**
 * Compute the resolved effective mask of an arbitrary input.
 *
 * Contrary to `mod_mask_get_effective`, it resolves only modifiers not present
 * in the canonical mask, so that it enables `xkb_state_serialize_mods` to
 * round trip via `xkb_state_update_mask`.
 */
ساكن مضمن xkb_mod_ماسک_t
resolve_منزل_canonical_mods(بنية xkb_keymap *keymap, xkb_mod_ماسک_t mods)
{
    أرجع
        /*
         * Keep canonical modifier mask.
         * It contains either real modifiers or canonical virtual modifiers.
         */
        (mods & keymap->canonical_state_ماسک) |
        /* Resolve other modifiers */
        mod_ماسک_get_effective(keymap,
                               mods & ~keymap->canonical_state_ماسک);
}

ساكن فراغ
state_update_latched_locked(
    بنية xkb_server_state * محدود state,
    ثابت بنية xkb_state_components_update * محدود update,
    بنية xkb_events * محدود events
)
{
    /* Update locks */
    ثابت xkb_mod_ماسک_t affect_locked_mods =
        resolve_منزل_canonical_mods(state->بنسټ.keymap, update->affect_locked_mods);
    إذا ((update->components & XKB_STATE_MODS_LOCKED) &&
        affect_locked_mods) {
        ثابت xkb_mod_ماسک_t locked_mods =
            resolve_منزل_canonical_mods(state->بنسټ.keymap, update->locked_mods);
        state->بنسټ.components.locked_mods &= ~affect_locked_mods;
        state->بنسټ.components.locked_mods |= locked_mods & affect_locked_mods;
    }
    إذا (update->components & XKB_STATE_LAYOUT_LOCKED) {
        state->بنسټ.components.locked_ډله = update->locked_layout;
    }

    /* Update latches */
    ثابت xkb_mod_ماسک_t affect_latched_mods =
        resolve_منزل_canonical_mods(state->بنسټ.keymap, update->affect_latched_mods);
    إذا ((update->components & XKB_STATE_MODS_LATCHED) &&
        affect_latched_mods) {
        ثابت xkb_mod_ماسک_t latched_mods =
            resolve_منزل_canonical_mods(state->بنسټ.keymap, update->latched_mods);
        update_latch_modifiers(state, events, affect_latched_mods, latched_mods);
    }
    إذا (update->components & XKB_STATE_LAYOUT_LATCHED) {
        update_latch_ډله(state, events, update->latched_layout);
    }
}

تعداد xkb_state_component
xkb_state_update_latched_locked(بنية xkb_state *بنسټ_state,
                                xkb_mod_ماسک_t affect_latched_mods,
                                xkb_mod_ماسک_t latched_mods,
                                bool affect_latched_layout,
                                int32_t latched_layout,
                                xkb_mod_ماسک_t affect_locked_mods,
                                xkb_mod_ماسک_t locked_mods,
                                bool affect_locked_layout,
                                int32_t locked_layout)
{
    /* Guard against client-only state */
    أكد(بنسټ_state->ډېرپېښ > SERVER_COMPANION);
    إذا (بنسټ_state->ډېرپېښ <= SERVER_COMPANION) {
        log_err(بنسټ_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_ډېرپېښ,
                "%s: Unexpected state type %d\n", __func__, بنسټ_state->ډېرپېښ);
        أرجع 0;
    }

    بنية xkb_server_state * ثابت state =
        (بنية xkb_server_state *)بنسټ_state;

    ثابت بنية state_components پخوانی_components = state->بنسټ.components;
    ثابت تعداد xkb_state_component components
        = ((affect_latched_mods || latched_mods) ? XKB_STATE_MODS_LATCHED : 0)
        | ((affect_locked_mods || locked_mods) ? XKB_STATE_MODS_LOCKED : 0)
        | (affect_latched_layout ? XKB_STATE_LAYOUT_LATCHED : 0)
        | (affect_locked_layout ? XKB_STATE_LAYOUT_LOCKED : 0);
    ثابت بنية xkb_state_components_update update = {
        .اندازه = حجم(update),
        .components = components,
        .affect_latched_mods = affect_latched_mods,
        .latched_mods = latched_mods,
        .latched_layout = latched_layout,
        .affect_locked_mods = affect_locked_mods,
        .locked_mods = locked_mods,
        .locked_layout = locked_layout
    };
    state_update_latched_locked(state, &update, عدم);
    xkb_state_update_derived(&state->بنسټ);
    أرجع get_state_component_changes(&پخوانی_components,
                                       &state->بنسټ.components);
}

ساكن مضمن فراغ
clear_all_latches_and_locks(بنية xkb_server_state *state,
                            بنية xkb_events *events)
{
    ساكن ثابت تعداد xkb_state_component components
        = XKB_STATE_MODS_LATCHED | XKB_STATE_MODS_LOCKED
        | XKB_STATE_LAYOUT_LATCHED | XKB_STATE_LAYOUT_LOCKED;
    ثابت بنية xkb_state_components_update update = {
        .اندازه = حجم(update),
        .components = components,
        .affect_latched_mods = (xkb_mod_ماسک_t)XKB_MOD_ALL,
        .latched_mods = 0,
        .latched_layout = 0,
        .affect_locked_mods = (xkb_mod_ماسک_t)XKB_MOD_ALL,
        .locked_mods = 0,
        .locked_layout = 0
    };
    state_update_latched_locked(state, &update, events);
}

ساكن تعداد xkb_حالت
state_update_layout_policy(بنية xkb_server_state *state,
                           ثابت بنية xkb_layout_policy_update *update)
{
    إذا (xkb_feature_supported(XKB_FEATURE_ENUM_LAYOUT_بهر_د_RANGE_POLICY,
                              update->policy)) {
        إذا (update->policy == XKB_LAYOUT_بهر_د_RANGE_REDIRECT) {
            إذا (update->redirect < state->بنسټ.keymap->عدد_groups) {
                state->بنسټ.بهر_د_range_ډله.redirect_ډله =
                    update->redirect;
            } وإلا {
                log_err(state->بنسټ.keymap->ctx,
                        XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                        "Layout policy: "
                        "unsupported layout index %"PRIu32" > %"PRIu32"\n",
                        update->redirect + 1, state->بنسټ.keymap->عدد_groups);
                أرجع XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص;
            }
        }
        state->بنسټ.بهر_د_range_ډله.policy = update->policy;
        أرجع XKB_SUCCESS;
    } وإلا {
        log_err(state->بنسټ.keymap->ctx,
                XKB_ERROR_UNSUPPORTED_LAYOUT_بهر_د_RANGE_POLICY_2,
                "Unsupported layout policy: %d\n", update->policy);
        أرجع XKB_ERROR_UNSUPPORTED_LAYOUT_بهر_د_RANGE_POLICY;
    }
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
ساكن تعداد xkb_حالت
check_state_update_abi_(بنية xkb_context * محدود ctx,
                        ثابت حرف * محدود func,
                        ثابت بنية xkb_synthetic_update * محدود update)
{
    تعداد xkb_حالت حالت = XKB_SUCCESS;
    إذا ((حالت = xkb_check_state_abi(update)) ||
        (update->reserved0 != 0 &&
         (حالت = XKB_ERROR_ABI_FORWARD_COMPAT)) ||
        (update->components &&
         (حالت = xkb_check_state_abi(update->components))) ||
        (update->layout_policy &&
         (حالت = xkb_check_state_abi(update->layout_policy)))) {
        xkb_log_abi_error(ctx, func, حالت);
    }
    أرجع حالت;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

#تعریف_کړه check_state_update_abi(ctx, update) \
    check_state_update_abi_(ctx, __func__, update)

تعداد xkb_حالت
xkb_state_update_synthetic(بنية xkb_state * بنسټ_state,
                           ثابت بنية xkb_synthetic_update * update,
                           تعداد xkb_state_component *بدل_شوی)
{
    /* Guard against client-only state */
    إذا (بنسټ_state->ډېرپېښ <= SERVER_COMPANION) {
        log_err(بنسټ_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_ډېرپېښ,
                "%s: Unexpected state type %d\n", __func__, بنسټ_state->ډېرپېښ);
        أرجع XKB_ERROR_UNEXPECTED_STATE_ډېرپېښ;
    }

    بنية xkb_server_state * ثابت state =
        (بنية xkb_server_state *)بنسټ_state;

    /* Check ABI compatibility */
    تعداد xkb_حالت حالت = check_state_update_abi(state->بنسټ.keymap->ctx,
                                                    update);
    إذا (حالت != XKB_SUCCESS)
        أرجع حالت;

    ثابت بنية state_components پخوانی_components = state->بنسټ.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    /* Update parametrized controls first */
    إذا (update->layout_policy) {
        حالت = state_update_layout_policy(state, update->layout_policy);
        إذا (حالت != XKB_SUCCESS)
            أرجع حالت;
    }

    إذا (update->components) {
        ثابت بنية xkb_state_components_update * ثابت components =
            update->components;
        /* Update boolean controls first */
        إذا (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, عدم);
        }

        state_update_latched_locked(state, components, عدم);
    }

    xkb_state_update_derived(&state->بنسټ);

    إذا (بدل_شوی) {
        *بدل_شوی = get_state_component_changes(&پخوانی_components,
                                               &state->بنسټ.components);
    }
    أرجع XKB_SUCCESS;
}

/**
 * Updates the state from a set of explicit masks as gained from
 * xkb_state_serialize_mods and xkb_state_serialize_groups.  As noted in the
 * documentation for these functions in xkbcommon.h, this round-trip is
 * lossy, and should only be used to update a slave state mirroring the
 * master, e.g. in a client/server window system.
 */
تعداد xkb_state_component
xkb_state_update_ماسک(بنية xkb_state *بنسټ_state,
                      xkb_mod_ماسک_t بنسټ_mods,
                      xkb_mod_ماسک_t latched_mods,
                      xkb_mod_ماسک_t locked_mods,
                      xkb_layout_شاخص_t بنسټ_ډله,
                      xkb_layout_شاخص_t latched_ډله,
                      xkb_layout_شاخص_t locked_ډله)
{
    /* Guard against server-only and server companion state */
    أكد(بنسټ_state->ډېرپېښ == CLIENT_STATE ||
           بنسټ_state->ډېرپېښ == LEGACY_MIXED_STATE);
    إذا (بنسټ_state->ډېرپېښ != CLIENT_STATE &&
        بنسټ_state->ډېرپېښ != LEGACY_MIXED_STATE) {
        log_err(بنسټ_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_ډېرپېښ,
                "%s: Unexpected state type %d\n", __func__, بنسټ_state->ډېرپېښ);
        أرجع 0;
    }

    بنية xkb_client_state * ثابت state =
        (بنية xkb_client_state *)بنسټ_state;

    ثابت بنية state_components prev_components = state->بنسټ.components;

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
    state->بنسټ.components.بنسټ_mods =
        resolve_منزل_canonical_mods(state->بنسټ.keymap, بنسټ_mods);
    state->بنسټ.components.latched_mods =
        resolve_منزل_canonical_mods(state->بنسټ.keymap, latched_mods);
    state->بنسټ.components.locked_mods =
        resolve_منزل_canonical_mods(state->بنسټ.keymap, locked_mods);

    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->بنسټ.components.بنسټ_ډله = (int32_t) بنسټ_ډله;
    state->بنسټ.components.latched_ډله = (int32_t) latched_ډله;
    state->بنسټ.components.locked_ډله = (int32_t) locked_ډله;

    xkb_state_update_derived(&state->بنسټ);

    أرجع get_state_component_changes(&prev_components, &state->بنسټ.components);
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Lock_Modifier
 */
ساكن bool
should_do_caps_transformation(بنية xkb_state *state, xkb_keycode_t kc)
{
    أرجع
        xkb_state_mod_شاخص_is_فعال(state, XKB_MOD_شاخص_CAPS,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_شاخص_is_consumed(state, kc, XKB_MOD_شاخص_CAPS) == 0;
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Control_Modifier
 */
ساكن bool
should_do_ctrl_transformation(بنية xkb_state *state, xkb_keycode_t kc)
{
    أرجع
        xkb_state_mod_شاخص_is_فعال(state, XKB_MOD_شاخص_CTRL,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_شاخص_is_consumed(state, kc, XKB_MOD_شاخص_CTRL) == 0;
}

/**
 * Provides the symbols to use for the given key and state.  Returns the
 * number of symbols pointed to in syms_out.
 */
صحيح
xkb_state_کلی_get_syms(بنية xkb_state *state, xkb_keycode_t kc,
                       ثابت xkb_keysym_t **syms_بهر)
{
    ثابت xkb_layout_شاخص_t layout = xkb_state_کلی_get_layout(state, kc);
    إذا (layout == XKB_LAYOUT_INVALID)
        اذهب err;

    ثابت xkb_level_شاخص_t level = xkb_state_کلی_get_level(state, kc, layout);
    إذا (level == XKB_LEVEL_INVALID)
        اذهب err;

    ثابت بنية xkb_کلی* ثابت کلی = Xkb_کلی(state->keymap, kc);

    إذا (!کلی)
        اذهب err;

    ثابت بنية xkb_level* ثابت leveli =
        xkb_keymap_کلی_get_level(state->keymap, کلی, layout, level);

    إذا (!leveli)
        اذهب err;

    ثابت xkb_keysym_شمېر_t عدد_syms = leveli->عدد_syms;
    إذا (عدد_syms == 0)
        اذهب err;

    إذا (should_do_caps_transformation(state, kc)) {
        /* Only simple capitalization rules: keysyms count is unchanged. */
        إذا (عدد_syms > 1) {
            *syms_بهر = (leveli->has_پاسنی)
                      ? leveli->s.syms + عدد_syms
                      : leveli->s.syms;
        } وإلا {
            *syms_بهر = &leveli->پاسنی;
        }
    } وإلا {
        *syms_بهر = (عدد_syms > 1)
                  ? leveli->s.syms
                  : &leveli->s.sym;
    }
    أرجع (صحيح) عدد_syms;

err:
    *syms_بهر = عدم;
    أرجع 0;
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
ساكن حرف
Xkb_منزل_Control(حرف ch)
{
    حرف c = ch;

    إذا ((c >= '@' && c < '\177') || c == ' ')
        c &= 0x1F;
    وإلا إذا (c == '2')
        c = '\000';
    وإلا إذا (c >= '3' && c <= '7')
        c -= ('3' - '\033');
    وإلا إذا (c == '8')
        c = '\177';
    وإلا إذا (c == '/')
        c = '_' & 0x1F;
    أرجع c;
}

/**
 * Provides either exactly one symbol, or XKB_KEY_NoSymbol.
 */
xkb_keysym_t
xkb_state_کلی_get_one_sym(بنية xkb_state *state, xkb_keycode_t kc)
{
    ثابت xkb_keysym_t *syms = عدم;
    ثابت صحيح عدد_syms = xkb_state_کلی_get_syms(state, kc, &syms);

    إذا (عدد_syms != 1)
        أرجع XKB_کلی_No_نښه;
    وإلا
        أرجع syms[0];
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
ساكن xkb_keysym_t
get_one_sym_for_string(بنية xkb_state *state, xkb_keycode_t kc)
{
    ثابت xkb_layout_شاخص_t layout = xkb_state_کلی_get_layout(state, kc);
    ثابت xkb_layout_شاخص_t عدد_layouts =
        xkb_keymap_عدد_layouts_for_کلی(state->keymap, kc);
    xkb_level_شاخص_t level = xkb_state_کلی_get_level(state, kc, layout);
    إذا (layout == XKB_LAYOUT_INVALID || عدد_layouts == 0 ||
        level == XKB_LEVEL_INVALID)
        أرجع XKB_کلی_No_نښه;

    ثابت xkb_keysym_t *syms = عدم;
    صحيح nsyms = xkb_keymap_کلی_get_syms_by_level(state->keymap, kc,
                                                 layout, level, &syms);
    إذا (nsyms != 1)
        أرجع XKB_کلی_No_نښه;
    xkb_keysym_t sym = syms[0];

    إذا (should_do_ctrl_transformation(state, kc) && sym > 127u) {
        لكل (xkb_layout_شاخص_t i = 0; i < عدد_layouts; i++) {
            level = xkb_state_کلی_get_level(state, kc, i);
            إذا (level == XKB_LEVEL_INVALID)
                استمر;

            nsyms = xkb_keymap_کلی_get_syms_by_level(state->keymap, kc,
                                                     i, level, &syms);
            إذا (nsyms == 1 && syms[0] <= 127u) {
                sym = syms[0];
                اكسر;
            }
        }
    }

    إذا (should_do_caps_transformation(state, kc)) {
        sym = xkb_keysym_منزل_پاسنی(sym);
    }

    أرجع sym;
}

صحيح
xkb_state_کلی_get_utf8(بنية xkb_state *state, xkb_keycode_t kc,
                       حرف *بفر, اندازه_t اندازه)
{
    صحيح nsyms;
    ثابت xkb_keysym_t *syms = عدم;
    ثابت xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    إذا (sym != XKB_کلی_No_نښه) {
        nsyms = 1; syms = &sym;
    }
    وإلا {
        nsyms = xkb_state_کلی_get_syms(state, kc, &syms);
    }

    /* Make sure not to truncate in the middle of a UTF-8 sequence. */
    صحيح دننه_واټن = 0;
    حرف tmp[XKB_KEYSYM_UTF8_MAX_اندازه];
    لكل (صحيح i = 0; i < nsyms; i++) {
        صحيح ret = xkb_keysym_منزل_utf8(syms[i], tmp, حجم(tmp));
        إذا (ret <= 0)
            اذهب err_bad;

        ret--;
        إذا ((اندازه_t)دننه_واټن + (اندازه_t)ret <= اندازه)
            حافظه_نقل(بفر + دننه_واټن, tmp, (اندازه_t)ret);
        دننه_واټن += ret;
    }

    إذا ((اندازه_t)دننه_واټن >= اندازه)
        اذهب err_trunc;
    بفر[دننه_واټن] = '\0';

    إذا (!is_valid_utf8(بفر, (اندازه_t)دننه_واټن))
        اذهب err_bad;

    إذا (دننه_واټن == 1 && (بینښه صحيح) بفر[0] <= 127u &&
        should_do_ctrl_transformation(state, kc))
        بفر[0] = Xkb_منزل_Control(بفر[0]);

    أرجع دننه_واټن;

err_trunc:
    إذا (اندازه > 0)
        بفر[اندازه - 1] = '\0';
    أرجع دننه_واټن;

err_bad:
    إذا (اندازه > 0)
        بفر[0] = '\0';
    أرجع 0;
}

uint32_t
xkb_state_کلی_get_utf32(بنية xkb_state *state, xkb_keycode_t kc)
{
    ثابت xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    uint32_t cp = xkb_keysym_منزل_utf32(sym);

    إذا (cp <= 127u && should_do_ctrl_transformation(state, kc))
        cp = (uint32_t) Xkb_منزل_Control((حرف) cp);

    أرجع cp;
}

/**
 * Serializes the requested modifier state into an xkb_mod_mask_t, with all
 * the same disclaimers as in xkb_state_update_mask.
 */
ساكن مضمن xkb_mod_ماسک_t
serialize_mods(ثابت بنية state_components *components,
               تعداد xkb_state_component type)
{
    xkb_mod_ماسک_t ret = 0;

    إذا (type & XKB_STATE_MODS_EFFECTIVE)
        أرجع components->mods;

    إذا (type & XKB_STATE_MODS_DEPRESSED)
        ret |= components->بنسټ_mods;
    إذا (type & XKB_STATE_MODS_LATCHED)
        ret |= components->latched_mods;
    إذا (type & XKB_STATE_MODS_LOCKED)
        ret |= components->locked_mods;

    أرجع ret;
}

xkb_mod_ماسک_t
xkb_state_serialize_mods(بنية xkb_state *state,
                         تعداد xkb_state_component type)
{
    أرجع serialize_mods(&state->components, type);
}

/**
 * Serialises the requested group state, with all the same disclaimers as
 * in xkb_state_update_mask.
 */
ساكن مضمن xkb_layout_شاخص_t
serialize_layout(ثابت بنية state_components *components,
                 تعداد xkb_state_component type)
{
    إذا (type & XKB_STATE_LAYOUT_EFFECTIVE)
        أرجع components->ډله;

    int32_t ret = 0;
    إذا (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret += components->بنسټ_ډله;
    إذا (type & XKB_STATE_LAYOUT_LATCHED)
        ret += components->latched_ډله;
    إذا (type & XKB_STATE_LAYOUT_LOCKED)
        ret += components->locked_ډله;

    أرجع (xkb_layout_شاخص_t)ret;
}

xkb_layout_شاخص_t
xkb_state_serialize_layout(بنية xkb_state *state,
                           تعداد xkb_state_component type)
{
    أرجع serialize_layout(&state->components, type);
}

xkb_led_ماسک_t
xkb_state_serialize_leds(ثابت بنية xkb_state *state,
                         تعداد xkb_state_component type)
{
    أرجع (type & XKB_STATE_LEDS)
        ? state->components.leds
        : 0;
}

ساكن مضمن تعداد xkb_keyboard_control_flags
serialize_controls(ثابت بنية state_components *components,
                   تعداد xkb_state_component type)
{
    أرجع (type & XKB_STATE_CONTROLS_EFFECTIVE)
        /*
         * Enable using the public API with the all the Controls values, except
         * the internal ones, if any.
         */
        ? (تعداد xkb_keyboard_control_flags)
          (components->controls & CONTROL_ALL_BOOLEAN)
        : 0;
}

تعداد xkb_keyboard_control_flags
xkb_state_serialize_controls(ثابت بنية xkb_state *state,
                             تعداد xkb_state_component type)
{
    أرجع serialize_controls(&state->components, type);
}

/**
 * Gets a modifier mask and returns the resolved effective mask; this
 * is needed because some modifiers can also map to other modifiers, e.g.
 * the "NumLock" modifier usually also sets the "Mod2" modifier.
 */
xkb_mod_ماسک_t
mod_ماسک_get_effective(بنية xkb_keymap *keymap, xkb_mod_ماسک_t mods)
{
    /* Initialize the effective mask with its corresponding real mods. */
    xkb_mod_ماسک_t ماسک = mods & MOD_REAL_ماسک_ALL;

    /* Resolve the virtual modifiers */
    ثابت بنية xkb_mod *mod;
    xkb_mod_شاخص_t i;
    xkb_vmods_enumerate(i, mod, &keymap->mods)
        إذا (mods & (UINT32_C(1) << i))
            ماسک |= mod->mapping;

    أرجع ماسک;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
صحيح
xkb_state_mod_شاخص_is_فعال(بنية xkb_state *state,
                              xkb_mod_شاخص_t idx,
                              تعداد xkb_state_component type)
{
    إذا (unlikely(idx >= xkb_keymap_عدد_mods(state->keymap)))
        أرجع -1;

    ثابت xkb_mod_ماسک_t mapping = state->keymap->mods.mods[idx].mapping;
    إذا (!mapping) {
        /* Modifier not mapped */
        أرجع 0;
    }
    /* WARNING: this may overmatch for virtual modifiers */
    أرجع (xkb_state_serialize_mods(state, type) & mapping) == mapping;
}

تعداد {
    XKB_STATE_سمون_FLAGS = XKB_STATE_سمون_ANY
                          | XKB_STATE_سمون_ALL
                          | XKB_STATE_سمون_NON_EXCLUSIVE
};

/**
 * Helper function for xkb_state_mod_indices_are_active and
 * xkb_state_mod_names_are_active.
 */
ساكن bool
سمون_mod_masks(بنية xkb_state *state,
                تعداد xkb_state_component type,
                تعداد xkb_state_سمون سمون,
                xkb_mod_ماسک_t wanted)
{
    ثابت xkb_mod_ماسک_t فعال = xkb_state_serialize_mods(state, type);

    إذا (!(سمون & XKB_STATE_سمون_NON_EXCLUSIVE) && (فعال & ~wanted))
        أرجع false;

    إذا (سمون & XKB_STATE_سمون_ANY)
        أرجع فعال & wanted;

    /* Ensure all flags but XKB_STATE_MATCH_ALL are processed before */
    static_assert(XKB_STATE_سمون_ALL ==
                  ((تعداد xkb_state_سمون) XKB_STATE_سمون_FLAGS &
                   ~(XKB_STATE_سمون_ANY | XKB_STATE_سمون_NON_EXCLUSIVE)),
                  "");

    أرجع (فعال & wanted) == wanted;
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, or -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
صحيح
xkb_state_mod_indices_are_فعال(بنية xkb_state *state,
                                 تعداد xkb_state_component type,
                                 تعداد xkb_state_سمون سمون,
                                 ...)
{
    إذا (سمون & ~((تعداد xkb_state_سمون) XKB_STATE_سمون_FLAGS))
        أرجع -2;

    va_list ap;
    xkb_mod_ماسک_t wanted = 0;
    صحيح ret = 0;
    ثابت xkb_mod_شاخص_t عدد_mods = xkb_keymap_عدد_mods(state->keymap);

    va_پیل(ap, سمون);
    طالما (1) {
        xkb_mod_شاخص_t idx = va_arg(ap, xkb_mod_شاخص_t);
        إذا (idx == XKB_MOD_INVALID)
            اكسر;
        إذا (unlikely(idx >= عدد_mods)) {
            ret = -1;
            اكسر;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_پای_ټکی(ap);

    إذا (ret == -1)
        أرجع ret;

    إذا (!wanted) {
        /* Modifiers not mapped */
        أرجع 0;
    }

    أرجع سمون_mod_masks(state, type, سمون, wanted);
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
صحيح
xkb_state_mod_name_is_فعال(بنية xkb_state *state, ثابت حرف *name,
                             تعداد xkb_state_component type)
{
    ثابت xkb_mod_شاخص_t idx = xkb_keymap_mod_get_شاخص(state->keymap, name);

    إذا (idx == XKB_MOD_INVALID)
        أرجع -1;

    أرجع xkb_state_mod_شاخص_is_فعال(state, idx, type);
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ATTR_NULL_SENTINEL صحيح
xkb_state_mod_نومونه_are_فعال(بنية xkb_state *state,
                               تعداد xkb_state_component type,
                               تعداد xkb_state_سمون سمون,
                               ...)
{
    إذا (سمون & ~((تعداد xkb_state_سمون) XKB_STATE_سمون_FLAGS))
        أرجع -2;

    va_list ap;
    xkb_mod_ماسک_t wanted = 0;
    صحيح ret = 0;

    va_پیل(ap, سمون);
    طالما (1) {
        ثابت حرف *str = va_arg(ap, ثابت حرف *);
        إذا (str == عدم)
            اكسر;
        ثابت xkb_mod_شاخص_t idx = xkb_keymap_mod_get_شاخص(state->keymap, str);
        إذا (idx == XKB_MOD_INVALID) {
            ret = -1;
            اكسر;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_پای_ټکی(ap);

    إذا (ret == -1)
        أرجع ret;

    إذا (!wanted) {
        /* Modifiers not mapped */
        أرجع 0;
    }

    أرجع سمون_mod_masks(state, type, سمون, wanted);
}

/**
 * Returns 1 if the given group is active with the specified type(s), 0 if
 * not, or -1 if the group is invalid.
 */
صحيح
xkb_state_layout_شاخص_is_فعال(بنية xkb_state *state,
                                 xkb_layout_شاخص_t idx,
                                 تعداد xkb_state_component type)
{
    إذا (idx >= state->keymap->عدد_groups)
        أرجع -1;

    صحيح ret = 0;
    إذا (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ret |= (state->components.ډله == idx);
    إذا (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret |= (state->components.بنسټ_ډله == (int32_t) idx);
    إذا (type & XKB_STATE_LAYOUT_LATCHED)
        ret |= (state->components.latched_ډله == (int32_t) idx);
    إذا (type & XKB_STATE_LAYOUT_LOCKED)
        ret |= (state->components.locked_ډله == (int32_t) idx);

    أرجع ret;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
صحيح
xkb_state_layout_name_is_فعال(بنية xkb_state *state, ثابت حرف *name,
                                تعداد xkb_state_component type)
{
    ثابت xkb_layout_شاخص_t idx =
        xkb_keymap_layout_get_شاخص(state->keymap, name);

    إذا (idx == XKB_LAYOUT_INVALID)
        أرجع -1;

    أرجع xkb_state_layout_شاخص_is_فعال(state, idx, type);
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
صحيح
xkb_state_led_شاخص_is_فعال(بنية xkb_state *state, xkb_led_شاخص_t idx)
{
    إذا (idx >= state->keymap->عدد_leds ||
        state->keymap->leds[idx].name == XKB_ATOM_NONE)
        أرجع -1;

    أرجع !!(state->components.leds & (UINT32_C(1) << idx));
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
صحيح
xkb_state_led_name_is_فعال(بنية xkb_state *state, ثابت حرف *name)
{
    ثابت xkb_led_شاخص_t idx = xkb_keymap_led_get_شاخص(state->keymap, name);

    إذا (idx == XKB_LED_INVALID)
        أرجع -1;

    أرجع xkb_state_led_شاخص_is_فعال(state, idx);
}

/**
 * See:
 * - XkbTranslateKeyCode(3), mod_rtrn return value, from libX11.
 * - MyEnhancedXkbTranslateKeyCode(), a modification of the above, from GTK+.
 */
ساكن xkb_mod_ماسک_t
کلی_get_consumed(بنية xkb_state *state, ثابت بنية xkb_کلی *کلی,
                 تعداد xkb_consumed_ډېرپېښ ډېرپېښ)
{
    ثابت xkb_layout_شاخص_t ډله =
        xkb_state_کلی_get_layout(state, کلی->keycode);
    إذا (ډله == XKB_LAYOUT_INVALID)
        أرجع 0;

    xkb_mod_ماسک_t preserve = 0;
    xkb_mod_ماسک_t consumed = 0;

    ثابت بنية xkb_کلی_type_ثبت* ثابت matching_ثبت =
        get_ثبت_for_کلی_state(state, کلی, ډله);
    إذا (matching_ثبت)
        preserve = matching_ثبت->preserve.ماسک;

    ثابت بنية xkb_کلی_type* ثابت type = کلی->groups[ډله].type;
    بدل (ډېرپېښ) {
    حالة XKB_CONSUMED_ډېرپېښ_XKB:
        consumed = type->mods.ماسک;
        اكسر;

    حالة XKB_CONSUMED_ډېرپېښ_GTK: {
        ثابت بنية xkb_کلی_type_ثبت* ثابت no_mods_ثبت =
            get_ثبت_for_mods(type, 0);
        ثابت xkb_level_شاخص_t no_mods_leveli = no_mods_ثبت
                                               ? no_mods_ثبت->level
                                               : 0;
        ثابت بنية xkb_level* ثابت no_mods_level =
            &کلی->groups[ډله].پوړونه[no_mods_leveli];

        لكل (darray_اندازه_t i = 0; i < type->عدد_entries; i++) {
            ثابت بنية xkb_کلی_type_ثبت* ثابت ثبت = &type->entries[i];
            إذا (!ثبت_is_فعال(ثبت))
                استمر;

            ثابت بنية xkb_level* ثابت level =
                &کلی->groups[ډله].پوړونه[ثبت->level];
            إذا (Xkb_پوړونه_Same_Syms(level, no_mods_level))
                استمر;

            إذا (ثبت == matching_ثبت || one_بټ_سېټ(ثبت->mods.ماسک))
                consumed |= ثبت->mods.ماسک & ~ثبت->preserve.ماسک;
        }
        اكسر;
    }
    }

    أرجع consumed & ~preserve;
}

صحيح
xkb_state_mod_شاخص_is_consumed2(بنية xkb_state *state, xkb_keycode_t kc,
                                 xkb_mod_شاخص_t idx,
                                 تعداد xkb_consumed_ډېرپېښ ډېرپېښ)
{
    ثابت بنية xkb_کلی* ثابت کلی = Xkb_کلی(state->keymap, kc);

    إذا (unlikely(!کلی || idx >= xkb_keymap_عدد_mods(state->keymap)))
        أرجع -1;

    ثابت xkb_mod_ماسک_t mapping = state->keymap->mods.mods[idx].mapping;
    إذا (!mapping) {
        /* Modifier not mapped */
        أرجع 0;
    }
    أرجع (mapping & کلی_get_consumed(state, کلی, ډېرپېښ)) == mapping;
}

صحيح
xkb_state_mod_شاخص_is_consumed(بنية xkb_state *state, xkb_keycode_t kc,
                                xkb_mod_شاخص_t idx)
{
    أرجع xkb_state_mod_شاخص_is_consumed2(state, kc, idx,
                                            XKB_CONSUMED_ډېرپېښ_XKB);
}

xkb_mod_ماسک_t
xkb_state_mod_ماسک_remove_consumed(بنية xkb_state *state, xkb_keycode_t kc,
                                   xkb_mod_ماسک_t ماسک)
{
    ثابت بنية xkb_کلی* ثابت کلی = Xkb_کلی(state->keymap, kc);

    إذا (!کلی)
        أرجع 0;

    أرجع resolve_منزل_canonical_mods(state->keymap, ماسک) &
           ~کلی_get_consumed(state, کلی, XKB_CONSUMED_ډېرپېښ_XKB);
}

xkb_mod_ماسک_t
xkb_state_کلی_get_consumed_mods2(بنية xkb_state *state, xkb_keycode_t kc,
                                 تعداد xkb_consumed_ډېرپېښ ډېرپېښ)
{
    بدل (ډېرپېښ) {
    حالة XKB_CONSUMED_ډېرپېښ_XKB:
    حالة XKB_CONSUMED_ډېرپېښ_GTK:
        اكسر;
    افتراضي:
        log_err_func(state->keymap->ctx, XKB_LOG_پیغام_NO_ID,
                     "unrecognized consumed modifiers mode: %d\n", ډېرپېښ);
        أرجع 0;
    }

    ثابت بنية xkb_کلی* ثابت کلی = Xkb_کلی(state->keymap, kc);
    إذا (!کلی)
        أرجع 0;

    أرجع کلی_get_consumed(state, کلی, ډېرپېښ);
}

xkb_mod_ماسک_t
xkb_state_کلی_get_consumed_mods(بنية xkb_state *state, xkb_keycode_t kc)
{
    أرجع xkb_state_کلی_get_consumed_mods2(state, kc, XKB_CONSUMED_ډېرپېښ_XKB);
}

/*******************************************************************************
 * Server state machine API
 ******************************************************************************/

/** Entry to keep track of the state of an overlaid key */
بنية xkb_overlaid_کلی {
    /** The source key being overlaid */
    ثابت بنية xkb_کلی *old;
    /** The target key */
    ثابت بنية xkb_کلی *new;
    /** Counter of key currently down */
    صحيح refcnt;
};

بنية xkb_shortcuts_config_ثبت {
    /** Real modifier mask to trigger shortcuts tweaks */
    xkb_mod_ماسک_t mods;
    /** Target layout */
    xkb_layout_شاخص_t موخه;
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
بنية xkb_machine {
    /** Inherits from `xkb_server_state` */
    بنية xkb_server_state بنسټ;

    /* Extra server-specific state data */

    /** Keyboard overlays handling */
    بنية {
        /**
         * Current overlaid keys
         *
         * When a key is overlaid, it remained overlaid in this configuration
         * until totally released, i.e. every key press was matched with the
         * corresponding key release.
         */
        darray(بنية xkb_overlaid_کلی) کلیګانې;
        /**
         * Activation order of the overlay
         *
         * Overlays indices are stored 1-indexed in nibbles: the lowest
         * nibble corresponds to the latest activated index.
         */
        uint32_t ترتیب;
        /** Current enabled overlays mask */
        xkb_overlay_ماسک_t enabled;
    } overlays;

    /** Configuration */
    بنية machine_config {
        /** Modifiers remapping */
        بنية machine_modifiers_config {
            /** Real modifier mask */
            xkb_mod_ماسک_t ماسک;
            /** Modifiers re-mappings */
            darray_اندازه_t mappings_عدد;
            بنية machine_mods_mapping {
                xkb_mod_ماسک_t سرچینه;
                xkb_mod_ماسک_t موخه;
            } * mappings ATTR_COUNTED_BY(mappings_عدد);
        } modifiers;

        /** Shortcuts tweak */
        بنية machine_shortcuts_config {
            /** Substitution entries */
            بنية xkb_shortcuts_config_ثبت *entries;
        } shortcuts;
    } config;

    /** Mouse keys handling */
    بنية {
        /** Default pointer button */
        xkb_اشاره_ګر_button_شاخص_t default_button;
        /** Mask of the locked buttons */
        xkb_اشاره_ګر_button_ماسک_t locked_buttons;
    } mouse;

    تعداد xkb_machine_flags flags;
};

تعريفنوع darray(بنية machine_mods_mapping) machine_mods_mappings;

بنية xkb_machine_builder {
    بنية xkb_keymap *keymap;

    /** Controls */
    بنية {
        /** Accessibility flags */
        بنية {
            تعداد xkb_a11y_flags affect_flags;
            تعداد xkb_a11y_flags flags;
        } a11y;
    } controls;

    /** Modifiers re-mapping */
    machine_mods_mappings mods;

    /** Shortcuts tweak */
    بنية xkb_shortcuts_config_options {
        /** Substitution entries */
        darray(بنية xkb_shortcuts_config_ثبت) entries;
    } shortcuts;

    تعداد xkb_machine_flags machine_flags;
    تعداد xkb_machine_builder_flags builder_flags;
    صحيح refcnt;
};

بنية xkb_machine_builder *
xkb_machine_builder_new(
    بنية xkb_keymap * محدود keymap,
    ثابت بنية xkb_machine_builder_config * محدود config,
    تعداد xkb_حالت * محدود حالت
)
{
    /* Handle default configuration */
    ساكن ثابت بنية xkb_machine_builder_config default_config = {
        .اندازه = حجم(default_config)
    };
    إذا (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ثابت تعداد xkb_حالت حالت_2 = xkb_check_state_abi(config);
    إذا (حالت_2) {
        xkb_log_abi_error(keymap->ctx, __func__, حالت_2);
        إذا (حالت)
            *حالت = حالت_2;
        أرجع عدم;
    }

    /* Sanitize input */
    ثابت تعداد xkb_machine_builder_flags invalid_builder_flags
        = config->builder_flags
        & ~(تعداد xkb_machine_builder_flags)XKB_MACHINE_BUILDER_FLAGS_ارزښتونه;
    إذا (invalid_builder_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS_,
                     "unrecognized machine builder flags: 0x%x\n",
                     invalid_builder_flags);
        إذا (حالت)
            *حالت = XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS;
        أرجع عدم;
    }

    ثابت تعداد xkb_machine_flags invalid_machine_flags
        = (تعداد xkb_machine_flags)config->machine_flags
        & ~(تعداد xkb_machine_flags)XKB_MACHINE_FLAGS_ارزښتونه;
    إذا (invalid_machine_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS_,
                     "unrecognized state machine flags: 0x%x\n",
                     invalid_machine_flags);
        إذا (حالت)
            *حالت = XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS;
        أرجع عدم;
    }

    بنية xkb_machine_builder * ثابت builder = حافظه_صفر_ورکړه(1, حجم(*builder));
    إذا (!builder) {
        log_err_func1(keymap->ctx, XKB_ERROR_ویش_FAILURE_2,
                      "cannot allocate machine builder\n");
        إذا (حالت)
            *حالت = XKB_ERROR_ویش_FAILURE;
        أرجع عدم;
    }

    *builder = (بنية xkb_machine_builder) {
        .keymap = xkb_keymap_ref(keymap),
        .refcnt = 1,
        .builder_flags = (تعداد xkb_machine_builder_flags)config->builder_flags,
        .machine_flags = (تعداد xkb_machine_flags)config->machine_flags,
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

    إذا (حالت)
        *حالت = XKB_SUCCESS;

    أرجع builder;
}

بنية xkb_machine_builder *
xkb_machine_builder_ref(بنية xkb_machine_builder *builder)
{
    أكد(builder->refcnt > 0);
    builder->refcnt++;
    أرجع builder;
}

فراغ
xkb_machine_builder_unref(بنية xkb_machine_builder *builder)
{
    أكد(!builder || builder->refcnt > 0);
    إذا (!builder || --builder->refcnt > 0)
        أرجع;

    darray_free(builder->shortcuts.entries);
    darray_free(builder->mods);
    xkb_keymap_unref(builder->keymap);
    حرر(builder);
}

بنية xkb_keymap *
xkb_machine_builder_get_keymap(ثابت بنية xkb_machine_builder *builder)
{
    /* Reference count is not updated. See API doc. */
    أرجع builder->keymap;
}

تعداد xkb_حالت
xkb_machine_builder_update_a11y(
    بنية xkb_machine_builder * محدود builder,
    ثابت بنية xkb_machine_builder_a11y_update * محدود update)
{
    /* Check ABI compatibility */
    تعداد xkb_حالت حالت = xkb_check_state_abi(update);
    إذا (حالت != XKB_SUCCESS) {
        xkb_log_abi_error(builder->keymap->ctx, __func__, حالت);
        أرجع حالت;
    }

    ثابت تعداد xkb_a11y_flags invalid_flags =
        ~(تعداد xkb_a11y_flags)XKB_A11_Y_FLAGS_ارزښتونه;

    إذا (update->affect_flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y affected flags: %#x\n",
                __func__, update->affect_flags & invalid_flags);
        أرجع XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }
    إذا (update->flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y flags: %#x\n",
                __func__, update->flags & invalid_flags);
        أرجع XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }

    builder->controls.a11y.affect_flags |= update->affect_flags;
    builder->controls.a11y.flags &= ~update->affect_flags;
    builder->controls.a11y.flags |= (update->flags & update->affect_flags);

    أرجع XKB_SUCCESS;
}

تعداد xkb_حالت
xkb_machine_builder_update_mods_remap(
    بنية xkb_machine_builder * محدود builder,
    ثابت بنية xkb_machine_builder_mods_remap_update * محدود update
)
{
    إذا (!update->سرچینه) {
        إذا (!update->موخه) {
            /* Reset mappings */
            darray_resize(builder->mods, 0);
            أرجع XKB_SUCCESS;
        } وإلا {
            أرجع XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک;
        }
    }

    /* Check the modifiers against the keymap */
    ثابت xkb_mod_ماسک_t invalid = ~builder->keymap->canonical_state_ماسک;
    إذا ((update->سرچینه & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک,
                "%s: Invalid source modifiers: 0x%"PRIx32"\n",
                __func__, update->سرچینه);
        أرجع XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک;
    }
    إذا ((update->موخه & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک,
                "%s: Invalid target modifiers: 0x%"PRIx32"\n",
                __func__, update->موخه);
        أرجع XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک;
    }

    بنية machine_mods_mapping *mapping = عدم;
    darray_اندازه_t m = 0;
    darray_enumerate(m, mapping, builder->mods) {
        إذا (mapping->سرچینه == update->سرچینه) {
            إذا (!update->موخه) {
                /* Remove mapping */
                darray_remove(builder->mods, m);
            } وإلا {
                /* Update mapping */
                mapping->موخه = update->موخه;
            }
            أرجع XKB_SUCCESS;
        }
    }

    إذا (update->موخه) {
        /* Append new mapping */
        darray_append(
            builder->mods,
            (بنية machine_mods_mapping) {
                .سرچینه = update->سرچینه,
                .موخه = update->موخه
            }
        );
    } وإلا {
        /* Ignore missing mapping */
    }

    أرجع XKB_SUCCESS;
}

تعداد xkb_حالت
xkb_machine_builder_update_shortcut_override(
    بنية xkb_machine_builder * محدود builder,
    ثابت بنية xkb_machine_builder_shortcut_override_update * محدود update
)
{
    بنية xkb_keymap *keymap = builder->keymap;

    /* Check the modifiers against the keymap */
    ثابت xkb_mod_ماسک_t invalid = ~keymap->canonical_state_ماسک;
    إذا ((update->affect_mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک,
                "%s: Invalid affected modifiers: 0x%"PRIx32"\n",
                __func__, update->affect_mods);
        أرجع XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک;
    }
    إذا ((update->mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک,
                "%s: Invalid modifiers: 0x%"PRIx32"\n",
                __func__, update->mods);
        أرجع XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک;
    }

    /* Check the layout indices against the keymap */
    إذا (update->سرچینه >= keymap->عدد_groups &&
        update->سرچینه != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                "%s: Invalid source layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->عدد_groups, update->سرچینه + 1);
        أرجع XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص;
    }
    إذا (update->موخه >= keymap->عدد_groups &&
        update->موخه != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                "%s: Invalid target layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->عدد_groups, update->موخه + 1);
        أرجع XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص;
    }

    بنية xkb_shortcuts_config_options * ثابت config = &builder->shortcuts;

    /* Resize array & initialize new entries, if relevant */
    إذا (update->سرچینه >= darray_اندازه(config->entries) &&
        update->سرچینه != XKB_LAYOUT_INVALID) {
        إذا (update->موخه == update->سرچینه) {
            /* Skip default setting */
            أرجع XKB_SUCCESS;
        }
        xkb_layout_شاخص_t new = darray_اندازه(config->entries);
        darray_resize(config->entries, update->سرچینه + 1);
        لكل (; new <= update->سرچینه; new++) {
            darray_توکی(config->entries, new) =
                (بنية xkb_shortcuts_config_ثبت) {
                    .موخه = XKB_LAYOUT_INVALID,
                    .mods = 0
                };
        }
    }

    بنية xkb_shortcuts_config_ثبت *ثبت;
    xkb_layout_شاخص_t سرچینه = (update->سرچینه == XKB_LAYOUT_INVALID)
        ? 0               /* Loop over all entries */
        : update->سرچینه; /* Single entry */
    darray_foreach_from(ثبت, config->entries, سرچینه) {
        إذا (update->سرچینه == XKB_LAYOUT_INVALID &&
            ثبت->موخه == XKB_LAYOUT_INVALID) {
            /* Skip previous deactivated entry */
            سرچینه++;
            استمر;
        }

        إذا (update->affect_mods) {
            ثبت->mods &= ~update->affect_mods;
            ثبت->mods |= (update->affect_mods & update->mods);
        }

        إذا (update->سرچینه != XKB_LAYOUT_INVALID) {
            ثبت->موخه = (update->موخه == سرچینه)
                ? XKB_LAYOUT_INVALID
                : update->موخه;
            /* Single entry: exit loop */
            اكسر;
        }
        سرچینه++;
    }

    أرجع XKB_SUCCESS;
}

/** Compare 2 modifiers combinations */
ساكن صحيح
cmp_mod_masks(ثابت فراغ *a, ثابت فراغ *b)
{
    ثابت xkb_mod_ماسک_t m1 = *((xkb_mod_ماسک_t *) a);
    ثابت xkb_mod_ماسک_t m2 = *((xkb_mod_ماسک_t *) b);
    إذا (m1 == m2)
        أرجع 0;

    إذا (!m1)
        أرجع +1;
    إذا (!m2)
        أرجع -1;

    ثابت xkb_mod_ماسک_t overlap = (m1 & m2);
    إذا (overlap == m1)
        أرجع +1; /* m1 is a subset of m2 */
    إذا (overlap == m2)
        أرجع -1; /* m2 is a subset of m1 */

    /* Compare active modifiers numbers */
    ثابت بینښه صحيح count1 = popcount32(m1);
    ثابت بینښه صحيح count2 = popcount32(m2);
    إذا (count1 > count2)
        أرجع -1;
    إذا (count1 < count2)
        أرجع +1;

    /* Give priority to real modifiers */
    أرجع (m1 < m2) ? -1 : +1;
}

ساكن تعداد xkb_حالت
machine_سېټ_mods(بنية xkb_machine *sm,
                 ثابت machine_mods_mappings *raw_mappings)
{
    إذا (!darray_تشه(*raw_mappings)) {
        machine_mods_mappings mappings = darray_new();
        xkb_mod_ماسک_t ماسک = 0;
        ثابت بنية machine_mods_mapping *mapping;
        ثابت xkb_mod_ماسک_t invalid =
            ~sm->بنسټ.بنسټ.keymap->canonical_state_ماسک;
        darray_foreach(mapping, *raw_mappings) {
            إذا (!mapping->سرچینه || !mapping->موخه ||
                (mapping->سرچینه & invalid) || (mapping->موخه & invalid))
                استمر;
            darray_append(mappings, *mapping);
            ماسک |= mapping->سرچینه;
        }

        /*
         * Re-order the mappings, so that combinations have higher priority
         * if they are superset of other combinations.
         * For other type of overlaps, see the sorting function.
         */
        qsort(darray_توکي(mappings), darray_اندازه(mappings),
              حجم(*darray_توکي(mappings)), &cmp_mod_masks);

        darray_steal(
            mappings,
            &sm->config.modifiers.mappings,
            &sm->config.modifiers.mappings_عدد
        );
        sm->config.modifiers.ماسک = ماسک;
    } وإلا {
        sm->config.modifiers = (بنية machine_modifiers_config) {
            .mappings = عدم,
            .mappings_عدد = 0,
            .ماسک = 0
        };
    }

    أرجع XKB_SUCCESS;
}

ساكن تعداد xkb_حالت
machine_سېټ_shortcuts(
    بنية xkb_machine * محدود sm,
    ثابت بنية xkb_shortcuts_config_options * محدود options
)
{
    إذا (darray_تشه(options->entries)) {
        sm->config.shortcuts = (بنية machine_shortcuts_config) {
            .entries = عدم
        };
        أرجع XKB_SUCCESS;
    }

    بنية xkb_keymap * ثابت keymap = sm->بنسټ.بنسټ.keymap;

    بنية xkb_shortcuts_config_ثبت * entries =
        حافظه_صفر_ورکړه(keymap->عدد_groups, حجم(*entries));
    إذا (!entries)
        أرجع XKB_ERROR_ویش_FAILURE;

    ثابت xkb_layout_شاخص_t شمېر = MIN(
        keymap->عدد_groups,
        (xkb_layout_شاخص_t) darray_اندازه(options->entries)
    );

    bool some_entries = false;
    لكل (xkb_layout_شاخص_t l = 0; l < شمېر; l++) {
        /* Sanitize layout target */
        ثابت xkb_layout_شاخص_t موخه =
            darray_توکی(options->entries, l).موخه;
        /* Sanitize modifier mask */
        ثابت xkb_mod_ماسک_t mods = (
            darray_توکی(options->entries, l).mods &
            sm->بنسټ.بنسټ.keymap->canonical_state_ماسک
        );
        إذا (موخه < keymap->عدد_groups && موخه != l && mods) {
            entries[l] = (بنية xkb_shortcuts_config_ثبت) {
                .موخه = موخه,
                .mods = mods
            };
            some_entries = true;
        } وإلا {
            entries[l] = (بنية xkb_shortcuts_config_ثبت) {
                .موخه = XKB_LAYOUT_INVALID,
                .mods = 0
            };
            استمر;
        }
    }

    إذا (!some_entries) {
        حرر(entries);
        entries = عدم;
    } وإلا {
        /* Initialize groups with no entries */
        لكل (xkb_layout_شاخص_t l = شمېر; l < keymap->عدد_groups; l++) {
            entries[l] = (بنية xkb_shortcuts_config_ثبت) {
                .موخه = XKB_LAYOUT_INVALID,
                .mods = 0
            };
        }
    }

    sm->config.shortcuts = (بنية machine_shortcuts_config) {
        .entries = entries
    };
    أرجع XKB_SUCCESS;
}

بنية xkb_machine *
xkb_machine_new(ثابت بنية xkb_machine_builder * محدود builder,
                تعداد xkb_حالت * محدود حالت)
{
    بنية xkb_machine * ثابت machine = حافظه_صفر_ورکړه(1, حجم(*machine));
    إذا (!machine) {
        log_err_func1(builder->keymap->ctx, XKB_ERROR_ویش_FAILURE_2,
                      "cannot allocate machine\n");
        إذا (حالت)
            *حالت = XKB_ERROR_ویش_FAILURE;
        أرجع عدم;
    }

    xkb_server_state_init(&machine->بنسټ, builder->keymap, SERVER_STATE,
                          builder->controls.a11y.affect_flags,
                          builder->controls.a11y.flags);
    machine->flags = builder->machine_flags;
    إذا (!(machine->flags &
          XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_کلی_FIX))
    {
        machine->بنسټ.flags |= STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_کلی;
    }
    إذا (machine->flags & XKB_MACHINE_UNLOCK_MODS_ON_PRESS) {
        machine->بنسټ.flags |= STATE_UNLOCK_MODS_ON_PRESS;
    }

    تعداد xkb_حالت حالت_2;
    إذا ((حالت_2 = machine_سېټ_mods(machine, &builder->mods)) != XKB_SUCCESS ||
        (حالت_2 = machine_سېټ_shortcuts(machine, &builder->shortcuts)) !=
        XKB_SUCCESS) {
        *حالت = حالت_2;
        اذهب error;
    }

    darray_init(machine->overlays.کلیګانې);
    machine->mouse.default_button = XKB_اشاره_ګر_BUTTON_MIN;

    إذا (حالت)
        *حالت = XKB_SUCCESS;

    أرجع machine;

error:
    xkb_machine_unref(machine);
    أرجع عدم;
}

بنية xkb_machine *
xkb_machine_ref(بنية xkb_machine *sm)
{
    أكد(sm->بنسټ.بنسټ.refcnt > 0);
    sm->بنسټ.بنسټ.refcnt++;
    أرجع sm;
}

فراغ
xkb_machine_unref(بنية xkb_machine *sm)
{
    أكد(!sm || sm->بنسټ.بنسټ.refcnt > 0);
    إذا (!sm || --sm->بنسټ.بنسټ.refcnt > 0)
        أرجع;

    xkb_state_destroy(&sm->بنسټ.بنسټ);
    darray_free(sm->overlays.کلیګانې);
    حرر(sm->config.shortcuts.entries);
    حرر(sm->config.modifiers.mappings);
    حرر(sm);
}

بنية xkb_keymap *
xkb_machine_get_keymap(ثابت بنية xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    أرجع sm->بنسټ.بنسټ.keymap;
}

بنية xkb_state *
xkb_machine_get_state(بنية xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    أرجع (بنية xkb_state *)sm;
}

بنية xkb_state *
xkb_state_new_from_machine(ثابت بنية xkb_machine * محدود machine,
                           تعداد xkb_حالت * محدود حالت)
{
    /* Creation */
    تعداد xkb_حالت حالت_2;
    بنية xkb_state * ثابت state = xkb_state_new_with_ډېرپېښ(
        machine->بنسټ.بنسټ.keymap,
        XKB_STATE_ډېرپېښ_SERVER_QUERY,
        &حالت_2
    );
    إذا (حالت_2 != XKB_SUCCESS) {
        إذا (حالت)
            *حالت = حالت_2;
        أرجع عدم;
    }

    أكد(state->ډېرپېښ == SERVER_COMPANION);
    أكد(state->refcnt == 1);

    /* Copy machine’s state */
    *state = machine->بنسټ.بنسټ;

    /* Restore previous values */
    state->ډېرپېښ = SERVER_COMPANION;
    state->refcnt = 1;

    إذا (حالت)
        *حالت = XKB_SUCCESS;
    أرجع state;
}

ساكن فراغ
machine_update_overlays(بنية xkb_machine *sm)
{
    /*
     * Overlays indices are stored 1-indexed in nibbles: the lowest
     * nibble corresponds to the latest activated index.
     */

    ثابت xkb_overlay_ماسک_t ماسک =
        overlays_from_controls(sm->بنسټ.بنسټ.components.controls);
    xkb_overlay_ماسک_t added = ماسک & ~sm->overlays.enabled;

    /* Remove overlays no longer enabled and keep relative order */
    uint32_t ترتیب = sm->overlays.ترتیب;
    ثابت xkb_overlay_شاخص_t overlay_max =
        format_max_overlays(sm->بنسټ.بنسټ.keymap->format);
    لكل (uint8_t n = 0; n < overlay_max;) {
        xkb_overlay_شاخص_t overlay_idx = (ترتیب >> (n * 4)) & 0xf;
        إذا (!overlay_idx)
            اكسر;
        ثابت xkb_overlay_ماسک_t overlay_ماسک =
            (xkb_overlay_ماسک_t)(1u << (overlay_idx - 1u) /* k is 1-indexed */);
        إذا (overlay_ماسک & ماسک) {
            /* no duplicates */
            added &= ~overlay_ماسک;
            n++;
        } وإلا {
            /* remove overlay */
            ثابت uint32_t سر = (UINT32_C(1) << (n * 4)) - 1;
            ترتیب = (ترتیب & سر) | ((ترتیب >> 4) & ~سر);
        }
    }

    /* Add new overlays in ascending order */
    لكل (xkb_overlay_شاخص_t k = 0; added; k++, added >>= 1) {
        إذا (added & 0x1) {
            ترتیب <<= 4;
            ترتیب |= k + 1u; /* k is 0-indexed */
        }
    }

    sm->overlays.ترتیب = ترتیب;
    sm->overlays.enabled = ماسک;
}

تعداد xkb_حالت
xkb_machine_process_synthetic(بنية xkb_machine *sm,
                              ثابت بنية xkb_synthetic_update *update,
                              بنية xkb_events *events)
{
    /* Check ABI compatibility */
    تعداد xkb_حالت حالت =
        check_state_update_abi(sm->بنسټ.بنسټ.keymap->ctx, update);
    إذا (حالت != XKB_SUCCESS)
        أرجع حالت;

    بنية xkb_server_state * ثابت state = &sm->بنسټ;
    ثابت بنية state_components پخوانی_components = state->بنسټ.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    darray_اندازه(events->کتار) = 0;
    events->بل = 0;

    /* Update parametrized controls first */
    إذا (update->layout_policy) {
        حالت = state_update_layout_policy(state, update->layout_policy);
        إذا (حالت != XKB_SUCCESS)
            أرجع حالت;
    }

    إذا (update->components) {
        ثابت بنية xkb_state_components_update * ثابت components =
            update->components;
        /* Update boolean controls first */
        إذا (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, events);
        }

        state_update_latched_locked(state, components, events);
    }

    xkb_state_update_derived(&state->بنسټ);

    ثابت تعداد xkb_state_component بدل_شوی = get_state_component_changes(
        &پخوانی_components, &state->بنسټ.components
    );
    إذا (بدل_شوی) {
        // TODO: latch controls
        إذا (بدل_شوی & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        /* Create event only if some component actually changed */
        darray_append(events->کتار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .بدل_شوی = بدل_شوی,
                .components = state->بنسټ.components
            }
        });

        /* Terminate the frame */
        darray_append(events->کتار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_چوکاټ,
        });
    }

    أرجع XKB_SUCCESS;
}

ساكن ssize_t
do_remap_modifiers(ثابت بنية machine_modifiers_config * محدود mappings,
                   بنية xkb_state * محدود state,
                   بنية xkb_events * محدود events,
                   ثابت بنية xkb_کلی * محدود کلی)
{
    إذا (!(mappings->ماسک & state->components.mods))
        أرجع -1;

    ثابت xkb_layout_شاخص_t layout = state_کلی_get_layout(state, کلی);
    إذا (layout >= کلی->عدد_groups)
        أرجع -1;

    ثابت بنية xkb_کلی_type * ثابت type = کلی->groups[layout].type;
    xkb_mod_ماسک_t affect_mods = 0;
    xkb_mod_ماسک_t mods = 0;
    لكل (darray_اندازه_t m = 0; m < mappings->mappings_عدد; m++) {
        ثابت بنية machine_mods_mapping * ثابت mapping =
            &mappings->mappings[m];
        إذا (/* Skip not matching active mods */
            (mapping->سرچینه & state->components.mods) == mapping->سرچینه &&
            /* Skip overlap with previous active mappings */
            !(mapping->سرچینه & affect_mods) &&
            /* Skip if the key type uses some of the source modifiers */
            !(mapping->سرچینه & type->mods.ماسک)) {
            affect_mods |= mappings->mappings[m].سرچینه;
            mods |= mappings->mappings[m].موخه;
        }
    }

    إذا (!affect_mods)
        أرجع -1;

    بنية xkb_state new = *state;
    new.components.بنسټ_mods = (new.components.بنسټ_mods & ~affect_mods) | mods;
    new.components.latched_mods = (new.components.latched_mods & ~affect_mods);
    new.components.locked_mods = (new.components.locked_mods & ~affect_mods);
    xkb_state_update_derived(&new);

    ssize_t event_idx = -1;
    ثابت تعداد xkb_state_component بدل_شوی =
        get_state_component_changes(&state->components, &new.components);
    إذا (بدل_شوی) {
        event_idx = (ssize_t) darray_اندازه(events->کتار);
        darray_append(events->کتار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = new.components,
                .بدل_شوی = بدل_شوی
            }
        });
    }

    state->components.mods = new.components.mods;
    أرجع event_idx;
}

/**
 * Activate an alternative layout if some modifiers are active, typically
 * `Control`, `Alt` or `Super`.
 *
 * The tweak consists of computing a *base* layout such that the *effective*
 * layout is the corresponding target layout.
 */
ساكن ssize_t
do_shortcuts_tweak(ثابت بنية machine_shortcuts_config *config,
                   بنية xkb_state *state,
                   ثابت بنية state_components *پخوانی_components,
                   بنية xkb_events *events, ssize_t remap_event)
{
    إذا (!config->entries) {
        /* No shortcuts tweak */
        أرجع remap_event;
    }

    ثابت بنية xkb_shortcuts_config_ثبت * ثابت ثبت =
        &config->entries[state->components.ډله];

    إذا (ثبت->موخه != XKB_LAYOUT_INVALID &&
        (ثبت->mods & state->components.mods)) {
        /*
         * Activate shortcuts tweak:
         * 1. The real base group is saved by the caller, to be restored by
         *    later by a call to undo_tweaks().
         * 2. Compute the new base group so that the resulting effective group
         *    is the target.
         * 3. Recompute derived state components.
         */
        بنية xkb_state new = *state;
        إذا (remap_event < 0) {
            /* Create new event */
            remap_event = (ssize_t) darray_اندازه(events->کتار);
            darray_append(events->کتار, (بنية xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = {0},
                    .بدل_شوی = 0
                }
            });
        } وإلا {
            /* Merge with last remap event */
            new.components =
                darray_توکی(events->کتار, remap_event).components.components;
        }
        new.components.بنسټ_ډله
            = (int32_t) ثبت->موخه
            - state->components.latched_ډله
            - state->components.locked_ډله;
        xkb_state_update_derived(&new);

        ثابت تعداد xkb_state_component بدل_شوی =
            get_state_component_changes(پخوانی_components, &new.components);
        darray_توکی(events->کتار, remap_event).components.بدل_شوی = بدل_شوی;
        darray_توکی(events->کتار, remap_event).components.components =
            new.components;

        state->components.ډله = new.components.ډله;
    } وإلا {
        /* Deactivated entry or modifiers do not match */
    }
    أرجع remap_event;
}

ساكن فراغ
undo_tweaks(ثابت بنية xkb_state *state,
            ثابت بنية state_components *initial_components,
            بنية xkb_events *events)
{
    /* Get last component event */
    ثابت بنية xkb_event *event = عدم;
    darray_foreach_سرچپه(event, events->کتار) {
        إذا (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS)
            اكسر;
    }
    إذا (!event)
        أرجع;

    /* Restore state */
    ثابت تعداد xkb_state_component بدل_شوی =
        get_state_component_changes(initial_components,
                                    &event->components.components);
    إذا (بدل_شوی) {
        darray_append(events->کتار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = *initial_components,
                .بدل_شوی = بدل_شوی
            }
        });
    }
}

ساكن ثابت بنية xkb_کلی *
process_overlayable_کلی(بنية xkb_machine *sm,
                        ثابت بنية xkb_کلی * کلی,
                        تعداد xkb_کلی_direction direction)
{
    /* Check if key is currently overlaid */
    بنية xkb_overlaid_کلی *ثبت = عدم;
    بنية xkb_overlaid_کلی *شته_ثبت = عدم;
    darray_foreach(ثبت, sm->overlays.کلیګانې) {
        إذا (ثبت->old == کلی) {
            /* Maintain the key overlaid */
            بدل (direction) {
            حالة XKB_کلی_ښکته:
                ثبت->refcnt++;
                اكسر;
            حالة XKB_کلی_REPEATED:
                اكسر;
            حالة XKB_کلی_پورته:
            افتراضي:
                ثبت->refcnt--;
                اكسر;
            }
            إذا (ثبت->refcnt <= 0)
                ثبت->old = عدم;
            أرجع ثبت->new;
        } وإلا إذا (!ثبت->old && !شته_ثبت) {
            /* First free spot */
            شته_ثبت = ثبت;
        }
    }
    إذا (direction == XKB_کلی_ښکته) {
        /* Enable key overlay */

        /*
         * Get overlay index: select the latest activated overlay
         * containing the key.
         */
        ثابت بنية xkb_کلی *new = کلی;

        إذا (کلی->overlays & sm->overlays.enabled) {
            /* Some relevant overlay is active */
            لكل (uint32_t ډېری = sm->overlays.ترتیب; ډېری; ډېری >>= 4) {
                static_assert(XKB_OVERLAY_شمېر == 8, "");
                ثابت xkb_overlay_شاخص_t overlay = (ډېری & 0xf) - 1;
                ثابت xkb_overlay_ماسک_t ماسک =
                    (xkb_overlay_ماسک_t)(1u << overlay);
                إذا (کلی->overlays & ماسک) {
                    /* Get new key */
                    إذا (کلی->overlays_inline) {
                        /* Stored inline */
                        new = کلی->overlay_کلی;
                    } وإلا {
                        /* Stored in sparse array */
                        ثابت xkb_overlay_ماسک_t ټیټ = (xkb_overlay_ماسک_t)(
                            کلی->overlays & (xkb_overlay_ماسک_t)(ماسک - 1u)
                        );
                        /* index = number of set bits strictly below bit */
                        ثابت xkb_overlay_شاخص_t شاخص =
                            (xkb_overlay_شاخص_t)popcount32(ټیټ);
                        new = کلی->overlays_کلیګانې[شاخص];
                    }
                    اكسر;
                }
            }
        } وإلا {
            /*
             * No relevant overlay is active: block future overlays while
             * the key is pressed.
             */
        }

        /* Get new entry */
        إذا (شته_ثبت) {
            /* Re-use a free entry */
            ثبت = شته_ثبت;
        } وإلا {
            /* Append a new entry */
            ثابت darray_اندازه_t idx = darray_اندازه(sm->overlays.کلیګانې);
            darray_resize(sm->overlays.کلیګانې, idx + 1);
            ثبت = &darray_توکی(sm->overlays.کلیګانې, idx);
        }

        /* Overlay the key */
        ثبت->old = کلی;
        ثبت->new = new;
        ثبت->refcnt = 1;
        أرجع new;
    }

    /* No overlay */
    أرجع کلی;
}

تعداد xkb_حالت
xkb_machine_process_کلی(بنية xkb_machine *sm,
                        xkb_keycode_t kc, تعداد xkb_کلی_direction direction,
                        بنية xkb_events *events)
{
    darray_اندازه(events->کتار) = 0;
    events->بل = 0;

    بنية xkb_server_state * ثابت state = &sm->بنسټ;
    ثابت بنية xkb_کلی * کلی = Xkb_کلی(state->بنسټ.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    إذا (!کلی || (direction == XKB_کلی_REPEATED && !کلی->repeats))
        أرجع XKB_SUCCESS;

    ثابت بنية state_components initial_components = state->بنسټ.components;

    إذا (direction == XKB_کلی_ښکته &&
        (sm->بنسټ.بنسټ.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیګانې) &&
        (sm->بنسټ.a11y_flags & XKB_A11_Y_STICKY_کلیګانې_NO_همزمانه_کلیګانې) &&
        state->بنسټ.components.بنسټ_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیګانې,
                                      0, events);
    }

    إذا (کلی->overlays)
        کلی = process_overlayable_کلی(sm, کلی, direction);

    ssize_t remap_event = do_remap_modifiers(&sm->config.modifiers,
                                             &state->بنسټ, events, کلی);

    remap_event = do_shortcuts_tweak(&sm->config.shortcuts, &state->بنسټ,
                                     &initial_components, events, remap_event);

    إذا (remap_event >= 0) {
        // TODO: what about merging state event for RedirectKey?
        darray_append(events->کتار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_چوکاټ,
        });
    }

    state->flags |= STATE_REQUIRE_کلی_EVENT;
    state->سېټ_mods = 0;
    state->clear_mods = 0;

    /* Handle key actions */
    xkb_filter_apply_all(state, events, کلی, direction);

    xkb_mod_شاخص_t i;
    xkb_mod_ماسک_t بټ;
    لكل (i = 0, بټ = 1; state->سېټ_mods; i++, بټ <<= 1) {
        إذا (state->سېټ_mods & بټ) {
            state->mod_کلی_شمېر[i]++;
            state->بنسټ.components.بنسټ_mods |= بټ;
            state->سېټ_mods &= ~بټ;
        }
    }

    لكل (i = 0, بټ = 1; state->clear_mods; i++, بټ <<= 1) {
        إذا (state->clear_mods & بټ) {
            state->mod_کلی_شمېر[i]--;
            إذا (state->mod_کلی_شمېر[i] <= 0) {
                state->بنسټ.components.بنسټ_mods &= ~بټ;
                state->mod_کلی_شمېر[i] = 0;
            }
            state->clear_mods &= ~بټ;
        }
    }

    xkb_state_update_derived(&state->بنسټ);

    /*
     * Insert key event only if actions did not override it, e.g.:
     * - RedirectKey() append its own key event;
     * - MovePointer() and PointerButton() generate pointer events instead
     *   of key events;
     * - TerminateServer() generate a Server event instead of key events.
     */
    إذا (state->flags & STATE_REQUIRE_کلی_EVENT) {
        darray_append(events->کتار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_کلی,
            .کلی = {
                .keycode = کلی->keycode,
                .direction = direction
            }
        });
    }

    إذا (remap_event >= 0) {
        // FIXME: fragile if last state change does not restore the state to the remap event
        undo_tweaks(&state->بنسټ, &initial_components, events);
    }

    ثابت تعداد xkb_state_component بدل_شوی = get_state_component_changes(
        &initial_components, &state->بنسټ.components
    );
    إذا (بدل_شوی) {
        إذا (بدل_شوی & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        // TODO: merge with previous state event, if any

        darray_append(events->کتار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = state->بنسټ.components,
                .بدل_شوی = بدل_شوی
            }
        });
    }

    إذا (!darray_تشه(events->کتار) &&
        darray_توکی(events->کتار, darray_اندازه(events->کتار) - 1).type !=
        XKB_EVENT_TYPE_چوکاټ)
    {
        darray_append(events->کتار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_چوکاټ,
        });
    }

    أرجع XKB_SUCCESS;
}

بنية xkb_events *
xkb_events_new(بنية xkb_context * محدود context,
               ثابت بنية xkb_events_config * محدود config,
               تعداد xkb_حالت * محدود حالت)
{
    /* Handle default configuration */
    ساكن ثابت بنية xkb_events_config default_config = {
        .اندازه = حجم(default_config)
    };
    إذا (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ثابت تعداد xkb_حالت abi_error = xkb_check_state_abi(config);
    إذا (abi_error) {
        xkb_log_abi_error(context, __func__, abi_error);
        إذا (حالت)
            *حالت = abi_error;
        أرجع عدم;
    }

    /* Sanitize input */
    ثابت uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_EVENTS_FLAGS_ارزښتونه);
    إذا (invalid_flags) {
        log_err_func(context, XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS_,
                     "unrecognized events collection flags: 0x%"PRIx32"\n",
                     invalid_flags);
        إذا (حالت)
            *حالت = XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS;
        أرجع عدم;
    }

    بنية xkb_events *events = حافظه_صفر_ورکړه(1, حجم(*events));
    إذا (!events) {
        log_err_func1(context, XKB_ERROR_ویش_FAILURE_2,
                      "cannot allocate state events collection\n");
        إذا (حالت)
            *حالت = XKB_ERROR_ویش_FAILURE;
        أرجع عدم;
    }

    /* Pre-allocate event queue */
    darray_init(events->کتار);
    ساكن ثابت darray_اندازه_t کتار_initial_اندازه = 16;
    darray_resize(events->کتار, کتار_initial_اندازه);
    // TODO: proper safe darray_resize() with return value
    إذا (!darray_توکي(events->کتار)) {
        log_err_func1(context, XKB_ERROR_ویش_FAILURE_2,
                      "cannot allocate state events queue\n");
        xkb_events_destroy(events);
        إذا (حالت)
            *حالت = XKB_ERROR_ویش_FAILURE;
        أرجع عدم;
    }
    darray_اندازه(events->کتار) = 0;

    events->ctx = xkb_context_ref(context);
    events->بل = 0;

    إذا (حالت)
        *حالت = XKB_SUCCESS;
    أرجع events;
}

فراغ
xkb_events_destroy(بنية xkb_events *events)
{
    إذا (!events)
        أرجع;
    darray_free(events->کتار);
    xkb_context_unref(events->ctx);
    حرر(events);
}

ثابت بنية xkb_event *
xkb_events_بل(بنية xkb_events *events)
{
    إذا (events->بل < darray_اندازه(events->کتار)) {
        ثابت darray_اندازه_t شاخص = events->بل++;
        أرجع &darray_توکی(events->کتار, شاخص);
    } وإلا {
        أرجع عدم;
    }
}

تعداد xkb_event_type
xkb_event_get_type(ثابت بنية xkb_event *event)
{
    أرجع event->type;
}

تعداد xkb_حالت
xkb_event_get_keycode(ثابت بنية xkb_event *event,
                      xkb_keycode_t *keycode,
                      تعداد xkb_کلی_direction *direction)
{
    إذا (event->type == XKB_EVENT_TYPE_کلی) {
        *keycode = event->کلی.keycode;
        *direction = event->کلی.direction;
        أرجع XKB_SUCCESS;
    } وإلا {
        أرجع XKB_ERROR_EVENT_TYPE_MISMATCH;
    }
}

تعداد xkb_حالت
xkb_event_get_components(ثابت بنية xkb_event * محدود event,
                         بنية xkb_event_components * محدود components)
{
    إذا (event->type != XKB_EVENT_TYPE_STATE_COMPONENTS)
        أرجع XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ثابت تعداد xkb_حالت حالت = xkb_check_state_abi(components);
    إذا (حالت != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, حالت);
        أرجع حالت;
    }

    components->بدل_شوی = (uint32_t)event->components.بدل_شوی;
    components->depressed_mods = event->components.components.بنسټ_mods;
    components->latched_mods = event->components.components.latched_mods;
    components->locked_mods = event->components.components.locked_mods;
    components->mods = event->components.components.mods;
    components->depressed_layout = event->components.components.بنسټ_ډله;
    components->latched_layout = event->components.components.latched_ډله;
    components->locked_layout = event->components.components.locked_ډله;
    components->layout = event->components.components.ډله;
    components->leds = event->components.components.leds;
    components->controls = (uint32_t)event->components.components.controls;

    أرجع XKB_SUCCESS;
}

تعداد xkb_حالت
xkb_event_get_اشاره_ګر_motion(ثابت بنية xkb_event * محدود event,
                             بنية xkb_event_اشاره_ګر_motion * محدود motion)
{
    إذا (event->type != XKB_EVENT_TYPE_اشاره_ګر_MOTION)
        أرجع XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ثابت تعداد xkb_حالت حالت = xkb_check_state_abi(motion);
    إذا (حالت != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, حالت);
        أرجع حالت;
    }

    motion->flags = event->اشاره_ګر_motion.flags;
    motion->x = event->اشاره_ګر_motion.x;
    motion->y = event->اشاره_ګر_motion.y;

    أرجع XKB_SUCCESS;
}

تعداد xkb_حالت
xkb_event_get_اشاره_ګر_button(ثابت بنية xkb_event * محدود event,
                             بنية xkb_event_اشاره_ګر_button * محدود button)
{
    إذا (event->type != XKB_EVENT_TYPE_اشاره_ګر_BUTTON)
        أرجع XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ثابت تعداد xkb_حالت حالت = xkb_check_state_abi(button);
    إذا (حالت != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, حالت);
        أرجع حالت;
    }

    button->button = event->اشاره_ګر_button.button;
    button->state = event->اشاره_ګر_button.state;
    button->شمېر = event->اشاره_ګر_button.شمېر;

    أرجع XKB_SUCCESS;
}

تعداد xkb_حالت
xkb_event_get_virtual_console(ثابت بنية xkb_event * محدود event,
                              int8_t * محدود شاخص_or_دننه_واټن,
                              bool * محدود is_دننه_واټن)
{
    إذا (event->type != XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
        أرجع XKB_ERROR_EVENT_TYPE_MISMATCH;

    *شاخص_or_دننه_واټن = event->virtual_console.شاخص_or_دننه_واټن;
    *is_دننه_واټن = event->virtual_console.is_دننه_واټن;

    أرجع XKB_SUCCESS;
}

تعداد xkb_حالت
xkb_state_update_event(بنية xkb_state * محدود بنسټ_state,
                       ثابت بنية xkb_event * محدود event,
                       تعداد xkb_state_component * محدود بدل_شوی)
{
    /* Guard against server-only state */
    أكد(بنسټ_state->ډېرپېښ == SERVER_COMPANION ||
           بنسټ_state->ډېرپېښ == LEGACY_MIXED_STATE);
    إذا (بنسټ_state->ډېرپېښ != SERVER_COMPANION &&
        بنسټ_state->ډېرپېښ != LEGACY_MIXED_STATE) {
        log_err(بنسټ_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_ډېرپېښ_2,
                "%s: Unexpected state type %d\n", __func__, بنسټ_state->ډېرپېښ);
        أرجع XKB_ERROR_UNEXPECTED_STATE_ډېرپېښ;
    }

    بنية xkb_client_state * ثابت state =
        (بنية xkb_client_state *)بنسټ_state;

    إذا (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
        ثابت بنية state_components prev_components = state->بنسټ.components;
        state->بنسټ.components = event->components.components;
        /*
         * Recompute the changes instead of using the event value, because we do
         * not know if the event’s queue and the state are synced.
         */
        إذا (بدل_شوی)
            *بدل_شوی = get_state_component_changes(&prev_components,
                                                   &state->بنسټ.components);
    } وإلا {
        إذا (بدل_شوی)
            *بدل_شوی = 0;
    }
    أرجع XKB_SUCCESS;
}

ساكن فراغ
append_اشاره_ګر_button(بنية xkb_events * محدود events,
                      بنية xkb_filter * محدود filter,
                      تعداد xkb_اشاره_ګر_button_state button_state,
                      uint8_t شمېر)
{
    darray_append(events->کتار, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_اشاره_ګر_BUTTON,
        .اشاره_ګر_button = {
            .اندازه = حجم(((بنية xkb_event*)0)->اشاره_ګر_button),
            .button = filter->priv,
            .state = button_state,
            .شمېر = شمېر,
        }
    });
}

ساكن فراغ
xkb_filter_اشاره_ګر_button_new(بنية xkb_server_state *state,
                              بنية xkb_events *events,
                              بنية xkb_filter *filter)
{
    إذا (!events || state->بنسټ.ډېرپېښ != SERVER_STATE ||
        !(state->بنسټ.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_کلیګانې)) {
        filter->func = عدم;
        أرجع;
    }

    بنية xkb_machine * ثابت sm = (بنية xkb_machine *)state;

    /* Resolve button */
    ثابت xkb_اشاره_ګر_button_شاخص_t button =
        (filter->action.btn.button == XKB_اشاره_ګر_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;
    filter->priv = button;

    ثابت تعداد xkb_اشاره_ګر_button_state button_state =
        XKB_اشاره_ګر_BUTTON_PRESSED;
    append_اشاره_ګر_button(events, filter, button_state,
                          filter->action.btn.شمېر);
    state->flags &= ~STATE_REQUIRE_کلی_EVENT;
}

ساكن bool
xkb_filter_اشاره_ګر_button_func(بنية xkb_server_state *state,
                               بنية xkb_events *events,
                               بنية xkb_filter *filter,
                               ثابت بنية xkb_کلی *کلی,
                               تعداد xkb_کلی_direction direction)
{
    إذا (کلی != filter->کلی || !events)
        أرجع XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلی_EVENT;

    بدل (direction) {
    حالة XKB_کلی_ښکته:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلی_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    إذا (!filter->action.btn.شمېر)
        append_اشاره_ګر_button(events, filter, XKB_اشاره_ګر_BUTTON_RELEASED, 0);

    filter->func = عدم;
    أرجع XKB_FILTER_CONSUME;
}

بنية xkb_filter_اشاره_ګر_قلف_button_priv {
    uint32_t button:31;
#که_تعریف_شوی _WIN32
    uint32_t locked:1; // MSVC requires matching types to pack bitfields
#وإلا
    bool locked:1;
#د_که_پای
};

تعداد {
    XKB_FILTER_اشاره_ګر_قلف_BUTTON_PRIV_پلنوالی =
        حجم(بنية xkb_filter_اشاره_ګر_قلف_button_priv) * CHAR_BIT,
    XKB_FILTER_اشاره_ګر_قلف_BUTTON_PRIV_MAX =
        (UINT64_C(1) << XKB_FILTER_اشاره_ګر_قلف_BUTTON_PRIV_پلنوالی) - 1
};

static_assert(حجم(بنية xkb_filter_اشاره_ګر_قلف_button_priv) ==
              حجم(((بنية xkb_filter*)0)->priv),
              "");
static_assert((uint32_t)XKB_اشاره_ګر_BUTTON_MAX < (uint32_t)XKB_FILTER_اشاره_ګر_قلف_BUTTON_PRIV_MAX,
              "xkb_filter_pointer_lock_button_priv cannot store button");

ساكن فراغ
xkb_filter_اشاره_ګر_قلف_button_new(بنية xkb_server_state *state,
                                   بنية xkb_events *events,
                                   بنية xkb_filter *filter)
{
    إذا (!events || state->بنسټ.ډېرپېښ != SERVER_STATE ||
        !(state->بنسټ.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_کلیګانې)) {
        filter->func = عدم;
        أرجع;
    }

    state->flags &= ~STATE_REQUIRE_کلی_EVENT;

    بنية xkb_machine * ثابت sm = (بنية xkb_machine *)state;

    /* Resolve button */
    ثابت xkb_اشاره_ګر_button_شاخص_t button =
        (filter->action.btn.button == XKB_اشاره_ګر_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;

    static_assert(XKB_اشاره_ګر_BUTTON_ماسک_پلنوالی <= 8,
                  "invalid UINTx_C() macro");
    static_assert(XKB_اشاره_ګر_BUTTON_MAX <= XKB_اشاره_ګر_BUTTON_ماسک_پلنوالی,
                  "invalid left shift");
    ثابت xkb_اشاره_ګر_button_ماسک_t ماسک = (UINT8_C(1) << button);

    ثابت bool locked = (sm->mouse.locked_buttons & ماسک);

    إذا (!locked && !(filter->action.btn.flags & ACTION_قلف_NO_قلف)) {
        sm->mouse.locked_buttons |= ماسک;
        filter->priv = button;
        append_اشاره_ګر_button(events, filter, XKB_اشاره_ګر_BUTTON_PRESSED, 0);
    }

    بنية xkb_filter_اشاره_ګر_قلف_button_priv *priv =
        (بنية xkb_filter_اشاره_ګر_قلف_button_priv*)&filter->priv;
    priv->button = button,
    priv->locked = locked;
}

ساكن bool
xkb_filter_اشاره_ګر_قلف_button_func(بنية xkb_server_state *state,
                                    بنية xkb_events *events,
                                    بنية xkb_filter *filter,
                                    ثابت بنية xkb_کلی *کلی,
                                    تعداد xkb_کلی_direction direction)
{
    إذا (کلی != filter->کلی || !events || state->بنسټ.ډېرپېښ != SERVER_STATE)
        أرجع XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلی_EVENT;

    بدل (direction) {
    حالة XKB_کلی_ښکته:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلی_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    ثابت بنية xkb_filter_اشاره_ګر_قلف_button_priv *priv =
        (بنية xkb_filter_اشاره_ګر_قلف_button_priv*)&filter->priv;

    إذا (priv->locked && !(filter->action.btn.flags & ACTION_قلف_NO_UNLOCK)) {
        بنية xkb_machine * ثابت sm = (بنية xkb_machine *)state;

        static_assert(XKB_اشاره_ګر_BUTTON_ماسک_پلنوالی <= 8,
                    "invalid UINTx_C() macro");
        static_assert(XKB_اشاره_ګر_BUTTON_MAX <= XKB_اشاره_ګر_BUTTON_ماسک_پلنوالی,
                    "invalid left shift");
        ثابت xkb_اشاره_ګر_button_ماسک_t ماسک = (UINT8_C(1) << priv->button);

        sm->mouse.locked_buttons &= ~ماسک;
        filter->priv = priv->button;
        append_اشاره_ګر_button(events, filter, XKB_اشاره_ګر_BUTTON_RELEASED, 0);
    }

    filter->func = عدم;
    أرجع XKB_FILTER_CONSUME;
}

ساكن فراغ
xkb_filter_اشاره_ګر_سېټ_default_button_new(بنية xkb_server_state *state,
                                          بنية xkb_events *events,
                                          بنية xkb_filter *filter)
{
    إذا (!events || state->بنسټ.ډېرپېښ != SERVER_STATE ||
        !(state->بنسټ.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_کلیګانې)) {
        filter->func = عدم;
        أرجع;
    }

    state->flags &= ~STATE_REQUIRE_کلی_EVENT;

    بنية xkb_machine * ثابت sm = (بنية xkb_machine *)state;

    static_assert(2 * XKB_اشاره_ګر_BUTTON_MAX <= لوی_عدد, "lossy conversion");
    صحيح button = (filter->action.dflt.flags & ACTION_ABSOLUTE_SWITCH)
        ? filter->action.dflt.ارزښت
        : sm->mouse.default_button + filter->action.dflt.ارزښت;

    /* Wrap into range */
    ثابت صحيح rem = button % XKB_اشاره_ګر_BUTTON_MAX;
    sm->mouse.default_button = (rem >= 0)
        ? (xkb_اشاره_ګر_button_شاخص_t)rem
        : (xkb_اشاره_ګر_button_شاخص_t)(rem + XKB_اشاره_ګر_BUTTON_MAX);
}

ساكن bool
xkb_filter_اشاره_ګر_سېټ_default_button_func(بنية xkb_server_state *state,
                                           بنية xkb_events *events,
                                           بنية xkb_filter *filter,
                                           ثابت بنية xkb_کلی *کلی,
                                           تعداد xkb_کلی_direction direction)
{
    إذا (کلی != filter->کلی || !events)
        أرجع XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلی_EVENT;

    بدل (direction) {
    حالة XKB_کلی_ښکته:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلی_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    filter->func = عدم;
    أرجع XKB_FILTER_CONTINUE;
}

ساكن فراغ
xkb_filter_terminate_server_new(بنية xkb_server_state *state,
                                 بنية xkb_events *events,
                                 بنية xkb_filter *filter)
{
    إذا (!events || state->بنسټ.ډېرپېښ != SERVER_STATE ||
        !(((بنية xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = عدم;
        أرجع;
    }

    state->flags &= ~STATE_REQUIRE_کلی_EVENT;

    darray_append(events->کتار, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    });
}

ساكن bool
xkb_filter_terminate_server_func(بنية xkb_server_state *state,
                                 بنية xkb_events *events,
                                 بنية xkb_filter *filter,
                                 ثابت بنية xkb_کلی *کلی,
                                 تعداد xkb_کلی_direction direction)
{
    إذا (کلی != filter->کلی || !events)
        أرجع XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلی_EVENT;

    بدل (direction) {
    حالة XKB_کلی_ښکته:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلی_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    filter->func = عدم;
    أرجع XKB_FILTER_CONSUME;
}

ساكن فراغ
xkb_filter_switch_virtual_console_new(بنية xkb_server_state *state,
                                      بنية xkb_events *events,
                                      بنية xkb_filter *filter)
{
    إذا (!events || state->بنسټ.ډېرپېښ != SERVER_STATE ||
        !(((بنية xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = عدم;
        أرجع;
    }

    state->flags &= ~STATE_REQUIRE_کلی_EVENT;

    darray_append(events->کتار, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
        .virtual_console = {
            .شاخص_or_دننه_واټن = filter->action.screen.screen,
            .is_دننه_واټن = !(filter->action.screen.flags & ACTION_ABSOLUTE_SWITCH)
        }
    });
}

ساكن bool
xkb_filter_switch_virtual_console_func(بنية xkb_server_state *state,
                                       بنية xkb_events *events,
                                       بنية xkb_filter *filter,
                                       ثابت بنية xkb_کلی *کلی,
                                       تعداد xkb_کلی_direction direction)
{
    إذا (کلی != filter->کلی || !events)
        أرجع XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلی_EVENT;

    بدل (direction) {
    حالة XKB_کلی_ښکته:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلی_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    filter->func = عدم;
    أرجع XKB_FILTER_CONSUME;
}
