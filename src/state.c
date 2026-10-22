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

#شامل "config.h"

#شامل <assert.h>
#شامل <limits.h>
#شامل <stdint.h>
#شامل <stdlib.h>
#شامل <string.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "xkbcommon/xkbcommon-status.h"
#شامل "xkbcommon/xkbcommon-features.h"
#شامل "abi-check.h"
#شامل "darray.h"
#شامل "features/enums.h"
#شامل "keymap.h"
#شامل "keysym.h"
#شامل "messages-codes.h"
#شامل "state-priv.h"
#شامل "utf8.h"
#شامل "utils.h"
#شامل "util-mem.h"
#شامل "util-numbers.h"

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
ساخت xkb_events {
    ساخت xkb_context *ctx;
    darray(ساخت xkb_event) قطار;
    /**
     * Read cursor for `xkb_events_next()`.
     *
     * Reset to 0 on each `process_*` call.
     */
    darray_سائز_t اگلا;
};

ساخت xkb_server_state;

ساخت xkb_filter {
    اتحاد xkb_action action;
    ثابت ساخت xkb_کلید *کلید;
    bool (*func)(ساخت xkb_server_state *state,
                 ساخت xkb_events *events,
                 ساخت xkb_filter *filter,
                 ثابت ساخت xkb_کلید *کلید,
                 شمار xkb_کلید_direction direction);
    uint32_t priv;
    صحیح refcnt;
};

شمار xkb_state_منوال_اندرونی {
    /** `xkb_state`: Client-only state (server API raises errors) */
    CLIENT_STATE = XKB_STATE_منوال_CLIENT,
    /** `xkb_state`: Server query companion */
    SERVER_COMPANION = XKB_STATE_منوال_SERVER_QUERY,
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

شمار { XKB_LAYOUT_اشاریہ_T_MIN_چوڑائی = 6 };
static_assert(XKB_MAX_GROUPS < (1 << XKB_LAYOUT_اشاریہ_T_MIN_چوڑائی),
              "Cannot encode out_of_range_group::redirect_group");

ساخت برآمد_کا_range_گروہ {
    شمار xkb_layout_برآمد_کا_range_policy policy :
        (حجم(شمار xkb_layout_برآمد_کا_range_policy) * CHAR_BIT -
         XKB_LAYOUT_اشاریہ_T_MIN_چوڑائی);
    xkb_layout_اشاریہ_t redirect_گروہ : XKB_LAYOUT_اشاریہ_T_MIN_چوڑائی;
};

شمار { XKB_STATE_منوال_اندرونی_MIN_چوڑائی = 4 /* 3 bits + sign */ };
static_assert(XKB_STATE_TYPE_عدد_ENTRIES <
              (1 << XKB_STATE_منوال_اندرونی_MIN_چوڑائی),
              "Cannot encode xkb_state::mode");

/**
 * Common state
 *
 * `mode` indicates whether the object is casted from a `xkb_server_state` or a
 * `xkb_client_state` struct.
 */
ساخت xkb_state {
    ساخت xkb_keymap *keymap;
    /*
     * Before updating the state, we keep a copy of just this struct. This
     * allows us to report which components of the state have changed.
     */
    ساخت state_components components;
    ساخت برآمد_کا_range_گروہ برآمد_کا_range_گروہ;

    شمار xkb_state_منوال_اندرونی منوال : XKB_STATE_منوال_اندرونی_MIN_چوڑائی;
    صحیح refcnt : (حجم(صحیح) * CHAR_BIT - XKB_STATE_منوال_اندرونی_MIN_چوڑائی);
};

/**
 * Minimal client state
 *
 * Wrapper for strong typing
 */
ساخت xkb_client_state {
    /** Inherits from `xkb_state` */
    ساخت xkb_state بنیاد;
};

شمار state_flags {
    STATE_NO_FLAGS = 0,
    STATE_REQUIRE_کلید_EVENT = (1 << 0),
    STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_کلید = (1 << 1),
    STATE_UNLOCK_MODS_ON_PRESS = (1 << 2),
};

/**
 * Legacy server state, used for `LEGACY_MIXED_STATE` and `LEGACY_SERVER_STATE`
 * modes.
 *
 * Casted as `xkb_state` in the public API. See `xkb_state::mode`.
 */
ساخت xkb_server_state {
    /** Inherits from `xkb_state` */
    ساخت xkb_state بنیاد;

    /* Server-specific state data */

    darray(ساخت xkb_filter) filters;

    /**
     * Mix of constant configuration flags and
     * variable state-processing flags.
     */
    شمار state_flags flags;

    شمار xkb_a11y_flags a11y_flags;

    /*
     * At each event, we accumulate all the needed modifications to the base
     * modifiers, and apply them at the end. These keep track of this state.
     */
    xkb_mod_نقاب_t سیٹ_mods;
    xkb_mod_نقاب_t clear_mods;

    /*
     * We mustn't clear a base modifier if there's another depressed key
     * which affects it, e.g. given this sequence
     * < Left Shift down, Right Shift down, Left Shift Up >
     * the modifier should still be set. This keeps the count.
     */
    int16_t mod_کلید_گنتی[XKB_MAX_MODS];
};

ساکن ثابت ساخت xkb_کلید_type_اندراج *
get_اندراج_for_mods(ثابت ساخت xkb_کلید_type *type, xkb_mod_نقاب_t mods)
{
    برائے (darray_سائز_t i = 0; i < type->عدد_entries; i++)
        اگر (اندراج_is_فعال(&type->entries[i]) &&
            type->entries[i].mods.نقاب == mods)
            واپس &type->entries[i];
    واپس تہی;
}

ساکن ثابت ساخت xkb_کلید_type_اندراج *
get_اندراج_for_کلید_state(ساخت xkb_state *state, ثابت ساخت xkb_کلید *کلید,
                        xkb_layout_اشاریہ_t گروہ)
{
    ثابت ساخت xkb_کلید_type* ثابت type = کلید->groups[گروہ].type;
    xkb_mod_نقاب_t فعال_mods = state->components.mods & type->mods.نقاب;
    واپس get_اندراج_for_mods(type, فعال_mods);
}

ساکن اندرونی xkb_level_اشاریہ_t
state_کلید_get_level(ساخت xkb_state *state, ثابت ساخت xkb_کلید *کلید,
                         xkb_layout_اشاریہ_t layout)
{
    اگر (layout >= کلید->عدد_groups)
        واپس XKB_LEVEL_INVALID;

    /* If we don't find an explicit match the default is 0. */
    ثابت ساخت xkb_کلید_type_اندراج* ثابت اندراج_2 =
        get_اندراج_for_کلید_state(state, کلید, layout);

    واپس (اندراج_2) ? اندراج_2->level : 0;
}

/**
 * Returns the level to use for the given key and state, or
 * XKB_LEVEL_INVALID.
 */
xkb_level_اشاریہ_t
xkb_state_کلید_get_level(ساخت xkb_state *state, xkb_keycode_t kc,
                        xkb_layout_اشاریہ_t layout)
{
    ثابت ساخت xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);

    واپس (کلید)
        ? state_کلید_get_level(state, کلید, layout)
        : XKB_LEVEL_INVALID;
}

ساکن اندرونی xkb_layout_اشاریہ_t
state_کلید_get_layout(ساخت xkb_state *state, ثابت ساخت xkb_کلید *کلید)
{
    واپس xkb_keymap_کلید_effective_layout(کلید, state->components.گروہ);
}

/**
 * Returns the layout to use for the given key and state, taking
 * wrapping/clamping/etc into account, or XKB_LAYOUT_INVALID.
 */
xkb_layout_اشاریہ_t
xkb_state_کلید_get_layout(ساخت xkb_state *state, xkb_keycode_t kc)
{
    ثابت ساخت xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);

    اگر (!کلید)
        واپس XKB_LAYOUT_INVALID;

    واپس state_کلید_get_layout(state, کلید);
}

/* Empty action used for empty levels */
ساکن ثابت اتحاد xkb_action dummy_action = { .type = ACTION_TYPE_NONE };

ساکن xkb_action_گنتی_t
xkb_کلید_get_actions(ساخت xkb_state *state, ثابت ساخت xkb_کلید *کلید,
                    ثابت اتحاد xkb_action **actions)
{
    ثابت xkb_layout_اشاریہ_t layout = state_کلید_get_layout(state, کلید);
    ثابت xkb_level_اشاریہ_t level = state_کلید_get_level(state, کلید, layout);
    اگر (level == XKB_LEVEL_INVALID)
        جاؤ err;

    ثابت xkb_action_گنتی_t گنتی =
        xkb_keymap_کلید_get_actions_by_level(state->keymap, کلید,
                                            layout, level, actions);
    اگر (!گنتی)
        جاؤ err;

    واپس گنتی;

err:
    /* Use a dummy action if no corresponding level was found or if it is empty.
     * This is required e.g. to handle latches properly. */
    *actions = &dummy_action;
    واپس 1;
}

ساکن ساخت xkb_filter *
xkb_filter_new(ساخت xkb_server_state *state)
{
    ساخت xkb_filter *filter = تہی, *iter;

    darray_foreach(iter, state->filters) {
        اگر (iter->func)
            جاری;
        /* Use available slot */
        filter = iter;
        توڑو;
    }

    اگر (!filter) {
        /* No available slot: resize the filters array */
        darray_resize0(state->filters, darray_سائز(state->filters) + 1);
        filter = &darray_شے(state->filters, darray_سائز(state->filters) -1);
    }

    filter->refcnt = 1;
    واپس filter;
}

/***====================================================================***/

شمار xkb_filter_نتیجہ {
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
#تعریف_کرو apply_گروہ_delta(filter_, state_, component_)                   \
    اگر ((filter_)->action.گروہ.flags & ACTION_ABSOLUTE_SWITCH)          \
        (state_)->components.component_ = (filter_)->action.گروہ.گروہ; \
    ورنہ                                                                 \
        (state_)->components.component_ += (filter_)->action.گروہ.گروہ

ساکن خالی
xkb_filter_گروہ_سیٹ_new(ساخت xkb_server_state *state,
                         ساخت xkb_events *events,
                         ساخت xkb_filter *filter)
{
    static_assert(حجم(state->بنیاد.components.بنیاد_گروہ) ==
                  حجم(filter->priv),
                  "Max groups don't fit");
    filter->priv = (uint32_t)state->بنیاد.components.بنیاد_گروہ;
    apply_گروہ_delta(filter, &state->بنیاد, بنیاد_گروہ);
}

ساکن bool
xkb_filter_گروہ_سیٹ_func(ساخت xkb_server_state *state,
                          ساخت xkb_events *events,
                          ساخت xkb_filter *filter,
                          ثابت ساخت xkb_کلید *کلید,
                          شمار xkb_کلید_direction direction)
{
    اگر (کلید != filter->کلید) {
        filter->action.گروہ.flags &= ~ACTION_تالا_CLEAR;
        واپس XKB_FILTER_CONTINUE;
    }

    انتخاب (direction) {
    حالت XKB_کلید_نیچے_کو:
        filter->refcnt++;
        /* fallthrough */
    حالت XKB_کلید_REPEATED:
        واپس XKB_FILTER_CONSUME;
    طےشدہ:
        اگر (--filter->refcnt > 0)
            واپس XKB_FILTER_CONSUME;
    }

    static_assert(حجم(state->بنیاد.components.بنیاد_گروہ) == حجم(filter->priv),
                  "Max groups don't fit");
    state->بنیاد.components.بنیاد_گروہ = (int32_t) filter->priv;

    اگر (filter->action.گروہ.flags & ACTION_تالا_CLEAR)
        state->بنیاد.components.locked_گروہ = 0;

    filter->func = تہی;
    واپس XKB_FILTER_CONTINUE;
}

ساکن شمار xkb_state_component
get_state_component_changes(ثابت ساخت state_components *a,
                            ثابت ساخت state_components *b)
{
    شمار xkb_state_component نقاب = 0;

    اگر (a->گروہ != b->گروہ)
        نقاب |= XKB_STATE_LAYOUT_EFFECTIVE;
    اگر (a->بنیاد_گروہ != b->بنیاد_گروہ)
        نقاب |= XKB_STATE_LAYOUT_DEPRESSED;
    اگر (a->latched_گروہ != b->latched_گروہ)
        نقاب |= XKB_STATE_LAYOUT_LATCHED;
    اگر (a->locked_گروہ != b->locked_گروہ)
        نقاب |= XKB_STATE_LAYOUT_LOCKED;
    اگر (a->mods != b->mods)
        نقاب |= XKB_STATE_MODS_EFFECTIVE;
    اگر (a->بنیاد_mods != b->بنیاد_mods)
        نقاب |= XKB_STATE_MODS_DEPRESSED;
    اگر (a->latched_mods != b->latched_mods)
        نقاب |= XKB_STATE_MODS_LATCHED;
    اگر (a->locked_mods != b->locked_mods)
        نقاب |= XKB_STATE_MODS_LOCKED;
    اگر (a->leds != b->leds)
        نقاب |= XKB_STATE_LEDS;
    اگر (a->controls != b->controls)
        نقاب |= XKB_STATE_CONTROLS_EFFECTIVE;

    واپس نقاب;
}

ساکن خالی
xkb_filter_گروہ_تالا_new(ساخت xkb_server_state *state,
                          ساخت xkb_events *events,
                          ساخت xkb_filter *filter)
{
    اگر (filter->action.گروہ.flags & ACTION_تالا_ON_RELEASE) {
        /*
         * Lock on key release: do nothing on key press.
         *
         * This is a keymap format v2 extension.
         */
        واپس;
    } ورنہ {
        /* Lock on key press */
        apply_گروہ_delta(filter, &state->بنیاد, locked_گروہ);
    }
}

ساکن bool
xkb_filter_گروہ_تالا_func(ساخت xkb_server_state *state,
                           ساخت xkb_events *events,
                           ساخت xkb_filter *filter,
                           ثابت ساخت xkb_کلید *کلید,
                           شمار xkb_کلید_direction direction)
{
    اگر (کلید != filter->کلید) {
        اگر ((filter->action.گروہ.flags & ACTION_تالا_ON_RELEASE) &&
            direction == XKB_کلید_نیچے_کو) {
            /*
             * Another key has been pressed after the locking key:
             * cancel group lock on release.
             *
             * This is a keymap v2 extension.
             */
            filter->action.گروہ.flags &= ~ACTION_تالا_ON_RELEASE;
        }
        واپس XKB_FILTER_CONTINUE;
    }

    انتخاب (direction) {
    حالت XKB_کلید_نیچے_کو:
        filter->refcnt++;
        /* fallthrough */
    حالت XKB_کلید_REPEATED:
        واپس XKB_FILTER_CONSUME;
    طےشدہ:
        اگر (--filter->refcnt > 0)
            واپس XKB_FILTER_CONSUME;
    }

    اگر (filter->action.گروہ.flags & ACTION_تالا_ON_RELEASE) {
        /*
         * Lock on key release
         *
         * This is a keymap v2 extension.
         */
        apply_گروہ_delta(filter, &state->بنیاد, locked_گروہ);
    } ورنہ {
        /* Lock on key press: do nothing on key release. */
    }
    filter->func = تہی;
    واپس XKB_FILTER_CONTINUE;
}

ساکن bool
xkb_action_breaks_latch(ثابت اتحاد xkb_action *action,
                        شمار xkb_action_controls controls,
                        شمار xkb_اندرونی_action_flags flag,
                        xkb_mod_نقاب_t نقاب)
{
    انتخاب (action->type) {
    حالت ACTION_TYPE_NONE:
    حالت ACTION_TYPE_VOID:
    حالت ACTION_TYPE_PTR_BUTTON:
    حالت ACTION_TYPE_PTR_تالا:
    حالت ACTION_TYPE_CTRL_سیٹ:
    حالت ACTION_TYPE_CTRL_تالا:
    حالت ACTION_TYPE_SWITCH_VT:
    حالت ACTION_TYPE_TERMINATE:
    حالت ACTION_TYPE_REDIRECT_کلید:
        واپس true;
    حالت ACTION_TYPE_PTR_MOVE:
    حالت ACTION_TYPE_PTR_DEFAULT:
        واپس !(controls & CONTROL_MOUSE_کلیدیں);
    حالت ACTION_TYPE_اندرونی:
        واپس (action->اندرونی_2.flags & flag) &&
               ((action->اندرونی_2.clear_latched_mods & نقاب) == نقاب);
    طےشدہ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_اندرونی == 20 &&
                      ACTION_TYPE_اندرونی + 1 == ACTION_TYPE_عدد_ENTRIES,
                      "Missing action type");
        واپس false;
    }
}

شمار xkb_کلید_latch_state {
    NO_LATCH = 0,
    LATCH_کلید_نیچے_کو,
    LATCH_PENDING,
    کلید_LATCH_STATE_عدد_ENTRIES
};

#تعریف_کرو MAX_XKB_کلید_LATCH_STATE_LOG2 2
#اگر (کلید_LATCH_STATE_عدد_ENTRIES > (1 << MAX_XKB_کلید_LATCH_STATE_LOG2)) || \
    (-XKB_MAX_GROUPS) < (INT32_MIN >> MAX_XKB_کلید_LATCH_STATE_LOG2) || \
      XKB_MAX_GROUPS > (INT32_MAX >> MAX_XKB_کلید_LATCH_STATE_LOG2)
#غلطی "Cannot represent priv field of the group latch filter"
#اگر_ختم

/* Hold the latch state *and* the group delta */
اتحاد گروہ_latch_priv {
    uint32_t priv;
    ساخت {
        /* The type is really: enum xkb_key_latch_state, but it is problematic
         * on Windows, because it is interpreted as signed and leads to wrong
         * negative values. */
        غیرعلامتی صحیح latch:MAX_XKB_کلید_LATCH_STATE_LOG2;
        int32_t گروہ_delta:(32 - MAX_XKB_کلید_LATCH_STATE_LOG2);
    };
};

ساکن خالی
xkb_filter_گروہ_latch_new(ساخت xkb_server_state *state,
                           ساخت xkb_events *events,
                           ساخت xkb_filter *filter)
{
    ثابت اتحاد گروہ_latch_priv priv = {
        .latch = LATCH_کلید_نیچے_کو,
        .گروہ_delta = (filter->action.گروہ.flags & ACTION_ABSOLUTE_SWITCH)
            ? filter->action.گروہ.گروہ - state->بنیاد.components.بنیاد_گروہ
            : filter->action.گروہ.گروہ
    };
    filter->priv = priv.priv;
    /* Like group set */
    apply_گروہ_delta(filter, &state->بنیاد, بنیاد_گروہ);
}

ساکن bool
xkb_filter_گروہ_latch_func(ساخت xkb_server_state *state,
                            ساخت xkb_events *events,
                            ساخت xkb_filter *filter,
                            ثابت ساخت xkb_کلید *کلید,
                            شمار xkb_کلید_direction direction)
{
    اتحاد گروہ_latch_priv priv = {.priv = filter->priv};
    شمار xkb_کلید_latch_state latch = priv.latch;

    اگر (direction == XKB_کلید_نیچے_کو) {
        ثابت اتحاد xkb_action *actions = تہی;
        ثابت xkb_action_گنتی_t گنتی = xkb_کلید_get_actions(&state->بنیاد, کلید,
                                                             &actions);

        اگر (latch == LATCH_کلید_نیچے_کو) {
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
            اگر (state->a11y_flags & XKB_A11_Y_LATCH_بیک_وقت_کلیدیں) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                برائے (xkb_action_گنتی_t k = 0; k < گنتی; k++) {
                    اگر (xkb_action_breaks_latch(&(actions[k]),
                                                state->بنیاد.components.controls,
                                                اندرونی_BREAKS_گروہ_LATCH,
                                                0))
                    {
                        latch = NO_LATCH;
                        توڑو;
                    }
                }
            } ورنہ {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        ورنہ اگر (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is
             * pressed, or promote it to a lock if it's the same group delta &
             * flags and latchToLock option is enabled. */
            ثابت bool sticky_کلیدیں = state->بنیاد.components.controls
                                   & CONTROL_STICKY_کلیدیں;
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
            برائے (xkb_action_گنتی_t k = 0; k < گنتی; k++) {
                اگر (((actions[k].type == ACTION_TYPE_گروہ_LATCH) ||
                     (actions[k].type == ACTION_TYPE_گروہ_سیٹ && sticky_کلیدیں)) &&
                     actions[k].گروہ.گروہ == filter->action.گروہ.گروہ &&
                     (actions[k].گروہ.flags & ACTION_ABSOLUTE_SWITCH) ==
                     (filter->action.گروہ.flags & ACTION_ABSOLUTE_SWITCH)) {
                    اگر (filter->action.گروہ.flags & ACTION_LATCH_مقصد_تالا &&
                        filter->action.گروہ.گروہ != 0)
                    {
                        /* Promote to lock */
                        filter->action.type = ACTION_TYPE_گروہ_تالا;
                        filter->func = xkb_filter_گروہ_تالا_func;
                        xkb_filter_گروہ_تالا_new(state, events, filter);
                        state->بنیاد.components.latched_گروہ -= priv.گروہ_delta;
                        filter->کلید = کلید;
                        /* XXX beep beep! */
                        واپس XKB_FILTER_CONSUME;
                    }
                    /* Do nothing if `latchToLock` option is not activated; if
                     * the latch is not broken by the following actions and the
                     * key is not consumed, then another latch filter will be
                     * created.
                     */
                    جاری;
                }
                ورنہ اگر (xkb_action_breaks_latch(&(actions[k]),
                                                 state->بنیاد.components.controls,
                                                 اندرونی_BREAKS_گروہ_LATCH,
                                                 0))
                {
                    /* Breaks the latch */
                    state->بنیاد.components.latched_گروہ -= priv.گروہ_delta;
                    filter->func = تہی;
                    واپس XKB_FILTER_CONTINUE;
                }
            }
        }
        ورنہ {
            /* Ignore press in NO_LATCH state */
            أكد(latch == NO_LATCH);
        }
    }
    ورنہ اگر (کلید != filter->کلید) {
        /* Ignore repeat/release of other keys */
    }
    ورنہ اگر (direction == XKB_کلید_REPEATED) {
        واپس XKB_FILTER_CONSUME; // TODO: check
    }
    ورنہ {
        /* Our key got released.  If we've set it to clear locks, and we
         * currently have a group locked, then release it and
         * don't actually latch.  Else we've actually hit the latching
         * stage, so set PENDING and move our group from base to
         * latched. */
        اگر ((filter->action.گروہ.flags & ACTION_تالا_CLEAR) &&
             state->بنیاد.components.locked_گروہ) {
            اگر (latch == LATCH_PENDING)
                state->بنیاد.components.latched_گروہ -= priv.گروہ_delta;
            ورنہ
                state->بنیاد.components.بنیاد_گروہ -= priv.گروہ_delta;
            state->بنیاد.components.locked_گروہ = 0;
            filter->func = تہی;
        }
        /* Broken latch */
        ورنہ اگر (latch == NO_LATCH) {
            state->بنیاد.components.بنیاد_گروہ -= priv.گروہ_delta;
            filter->func = تہی;
        }
        /* We may already have reached the latch state if pressing the
         * key multiple times without latch-to-lock enabled. */
        ورنہ اگر (latch == LATCH_کلید_نیچے_کو) {
            latch = LATCH_PENDING;
            /* Switch from set to latch */
            state->بنیاد.components.بنیاد_گروہ -= priv.گروہ_delta;
            state->بنیاد.components.latched_گروہ += priv.گروہ_delta;
            /* XXX beep beep! */
        }
    }

    priv.latch = latch;
    filter->priv = priv.priv;

    واپس XKB_FILTER_CONTINUE;
}

ساکن خالی
xkb_filter_mod_سیٹ_new(ساخت xkb_server_state *state,
                       ساخت xkb_events *events,
                       ساخت xkb_filter *filter)
{
    ثابت شمار xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_تالا_CLEAR;
    اگر ((filter->action.mods.flags & unlock) == unlock) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension.
         */
        filter->priv = filter->action.mods.mods.نقاب
                     & ~state->بنیاد.components.locked_mods;
        state->بنیاد.components.locked_mods &= ~filter->action.mods.mods.نقاب;
    } ورنہ {
        filter->priv = filter->action.mods.mods.نقاب;
    }

    state->سیٹ_mods |= (xkb_mod_نقاب_t) filter->priv;
}

ساکن bool
xkb_filter_mod_سیٹ_func(ساخت xkb_server_state *state,
                        ساخت xkb_events *events,
                        ساخت xkb_filter *filter,
                        ثابت ساخت xkb_کلید *کلید,
                        شمار xkb_کلید_direction direction)
{

    اگر (کلید != filter->کلید) {
        filter->action.mods.flags &= ~ACTION_تالا_CLEAR;
        واپس XKB_FILTER_CONTINUE;
    }

    انتخاب (direction) {
    حالت XKB_کلید_نیچے_کو:
        filter->refcnt++;
        /* fallthrough */
    حالت XKB_کلید_REPEATED:
        واپس XKB_FILTER_CONSUME;
    طےشدہ:
        اگر (--filter->refcnt > 0)
            واپس XKB_FILTER_CONSUME;
    }

    state->clear_mods |= (xkb_mod_نقاب_t) filter->priv;
    ثابت شمار xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_تالا_CLEAR;
    اگر ((filter->action.mods.flags & unlock) == ACTION_تالا_CLEAR)
        state->بنیاد.components.locked_mods &= ~filter->action.mods.mods.نقاب;

    filter->func = تہی;
    واپس XKB_FILTER_CONTINUE;
}

ساکن خالی
xkb_filter_mod_تالا_new(ساخت xkb_server_state *state,
                        ساخت xkb_events *events,
                        ساخت xkb_filter *filter)
{
    filter->priv = (state->بنیاد.components.locked_mods &
                    filter->action.mods.mods.نقاب);

    اگر (filter->priv && (filter->action.mods.flags & ACTION_UNLOCK_ON_PRESS)) {
        /*
         * Some of the target modifiers were locked before key press: unlock.
         *
         * This is a keymap v2 extension: unlock-on-press.
         */
        اگر (!(filter->action.mods.flags & ACTION_تالا_NO_UNLOCK))
            state->بنیاد.components.locked_mods &= ~filter->priv;
        /* No further action: cancel filter */
        filter->func = تہی;
    } ورنہ {
        /* Set base mods; lock mods if relevant (XKB 1.0 spec) */
        state->سیٹ_mods |= filter->action.mods.mods.نقاب;
        اگر (!(filter->action.mods.flags & ACTION_تالا_NO_تالا))
            state->بنیاد.components.locked_mods |= filter->action.mods.mods.نقاب;
    }
}

ساکن bool
xkb_filter_mod_تالا_func(ساخت xkb_server_state *state,
                         ساخت xkb_events *events,
                         ساخت xkb_filter *filter,
                         ثابت ساخت xkb_کلید *کلید,
                         شمار xkb_کلید_direction direction)
{
    اگر (کلید != filter->کلید)
        واپس XKB_FILTER_CONTINUE;

    انتخاب (direction) {
    حالت XKB_کلید_نیچے_کو:
        filter->refcnt++;
        /* fallthrough */
    حالت XKB_کلید_REPEATED:
        واپس XKB_FILTER_CONSUME;
    طےشدہ:
        اگر (--filter->refcnt > 0)
            واپس XKB_FILTER_CONSUME;
    }

    state->clear_mods |= filter->action.mods.mods.نقاب;
    اگر (!(filter->action.mods.flags & ACTION_تالا_NO_UNLOCK))
        state->بنیاد.components.locked_mods &= ~filter->priv;

    filter->func = تہی;
    واپس XKB_FILTER_CONTINUE;
}

ساکن خالی
xkb_filter_mod_latch_new(ساخت xkb_server_state *state,
                         ساخت xkb_events *events,
                         ساخت xkb_filter *filter)
{
    /* Latch-on-press + clear-locks imply unlock-on-press */
    ثابت شمار xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                              | ACTION_LATCH_ON_PRESS;

    اگر ((filter->action.mods.flags & ACTION_تالا_CLEAR) &&
        (filter->action.mods.flags & unlockOnPress) &&
        (state->بنیاد.components.locked_mods & filter->action.mods.mods.نقاب) ==
         filter->action.mods.mods.نقاب) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension: clear locks and do not latch.
         */
        state->بنیاد.components.locked_mods &= ~filter->action.mods.mods.نقاب;
        filter->func = تہی;
    } ورنہ اگر (filter->action.mods.flags & ACTION_LATCH_ON_PRESS) {
        /*
         * Latch on key press
         *
         * This is a keymap format v2 extension.
         */
        filter->priv = LATCH_PENDING;
        state->بنیاد.components.latched_mods |= filter->action.mods.mods.نقاب;
        /* XXX beep beep! */
    } ورنہ {
        /* XKB standard latch action */
        filter->priv = LATCH_کلید_نیچے_کو;
        state->سیٹ_mods |= filter->action.mods.mods.نقاب;
    }
}

ساکن bool
xkb_filter_mod_latch_func(ساخت xkb_server_state *state,
                          ساخت xkb_events *events,
                          ساخت xkb_filter *filter,
                          ثابت ساخت xkb_کلید *کلید,
                          شمار xkb_کلید_direction direction)
{
    شمار xkb_کلید_latch_state latch = filter->priv;

    اگر (direction == XKB_کلید_نیچے_کو) {
        ثابت اتحاد xkb_action *actions = تہی;
        ثابت xkb_action_گنتی_t گنتی = xkb_کلید_get_actions(&state->بنیاد, کلید,
                                                             &actions);

        اگر (latch == LATCH_کلید_نیچے_کو) {
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
            اگر (state->a11y_flags & XKB_A11_Y_LATCH_بیک_وقت_کلیدیں) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                برائے (xkb_action_گنتی_t k = 0; k < گنتی; k++) {
                    اگر (xkb_action_breaks_latch(&(actions[k]),
                                                state->بنیاد.components.controls,
                                                اندرونی_BREAKS_MOD_LATCH,
                                                filter->action.mods.mods.نقاب))
                    {
                        latch = NO_LATCH;
                        توڑو;
                    }
                }
            } ورنہ {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        ورنہ اگر (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is pressed,
             * or promote it to a lock or plain base set if it's the same
             * modifier. */
            ثابت bool sticky_کلیدیں = state->بنیاد.components.controls
                                   & CONTROL_STICKY_کلیدیں;
            /*
             * NOTE: Action’s flags are not compared:
             * - flags may have been modified by keyboard controls at the
             *   creation of the filter;
             * - it is unlikely to have two LatchMods() actions on the same
             *   key that differ only by their flags;
             * - even if it were the case, one would most likely want both
             *   actions to be considered compatible in this context.
             */
            برائے (xkb_action_گنتی_t k = 0; k < گنتی; k++) {
                اگر (((actions[k].type == ACTION_TYPE_MOD_LATCH) ||
                     (actions[k].type == ACTION_TYPE_MOD_سیٹ && sticky_کلیدیں)) &&
                    actions[k].mods.mods.نقاب == filter->action.mods.mods.نقاب)
                {
                    اگر (filter->action.mods.flags & ACTION_LATCH_مقصد_تالا) {
                        /* Mutate the action to LockMods() */
                        filter->action.type = ACTION_TYPE_MOD_تالا;
                        filter->func = xkb_filter_mod_تالا_func;
                        xkb_filter_mod_تالا_new(state, events, filter);
                    }
                    ورنہ {
                        /* Mutate the action to SetMods() */
                        filter->action.type = ACTION_TYPE_MOD_سیٹ;
                        filter->func = xkb_filter_mod_سیٹ_func;
                        xkb_filter_mod_سیٹ_new(state, events, filter);
                    }
                    filter->کلید = کلید;
                    /* Clear latches */
                    state->بنیاد.components.latched_mods &=
                        ~filter->action.mods.mods.نقاب;
                    /* XXX beep beep! */
                    واپس XKB_FILTER_CONSUME;
                }
                ورنہ اگر (xkb_action_breaks_latch(&(actions[k]),
                                                 state->بنیاد.components.controls,
                                                 اندرونی_BREAKS_MOD_LATCH,
                                                 filter->action.mods.mods.نقاب))
                {
                    /* XXX: This may be totally broken, we might need to break the
                     *      latch in the next run after this press? */
                    state->بنیاد.components.latched_mods &=
                        ~filter->action.mods.mods.نقاب;
                    filter->func = تہی;
                    واپس XKB_FILTER_CONTINUE;
                }
            }
        }
        ورنہ {
            /* Ignore press in NO_LATCH state */
            أكد(latch == NO_LATCH);
        }
    }
    ورنہ اگر (کلید != filter->کلید) {
        /* Ignore repeat/release of other keys */
    }
    ورنہ اگر (direction == XKB_کلید_REPEATED) {
        واپس XKB_FILTER_CONSUME; // TODO: check
    }
    ورنہ {
        /* Our key got released.  If we’ve set it to clear locks, and we
         * currently have the same modifiers locked, then release them and
         * don't actually latch.  Else we’ve actually hit the latching
         * stage, so set PENDING and move our modifier from base to
         * latched. */

        /* Latch-on-press + clear-locks imply unlock-on-press */
        ثابت شمار xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                                  | ACTION_LATCH_ON_PRESS;

        اگر ((filter->action.mods.flags & ACTION_تالا_CLEAR) &&
            !(filter->action.mods.flags & unlockOnPress) &&
            (state->بنیاد.components.locked_mods &
             filter->action.mods.mods.نقاب) == filter->action.mods.mods.نقاب)
        {
            /* XXX: We might be a bit overenthusiastic about clearing
             *      mods other filters have set here? */
            اگر (latch == LATCH_PENDING)
                state->بنیاد.components.latched_mods &=
                    ~filter->action.mods.mods.نقاب;
            ورنہ
                state->clear_mods |= filter->action.mods.mods.نقاب;
            state->بنیاد.components.locked_mods &= ~filter->action.mods.mods.نقاب;
            filter->func = تہی;
        }
        ورنہ اگر (latch == NO_LATCH) {
            /* Broken latch */
            state->clear_mods |= filter->action.mods.mods.نقاب;
            filter->func = تہی;
        }
        ورنہ اگر (!(filter->action.mods.flags & ACTION_LATCH_ON_PRESS)) {
            latch = LATCH_PENDING;
            state->clear_mods |= filter->action.mods.mods.نقاب;
            state->بنیاد.components.latched_mods |= filter->action.mods.mods.نقاب;
            /* XXX beep beep! */
        }
    }

    filter->priv = latch;

    واپس XKB_FILTER_CONTINUE;
}

ساکن خالی
append_اشارہ_گر_move(ساخت xkb_events *events,
                    ساخت xkb_filter *filter)
{
    شمار xkb_اشارہ_گر_motion_flags flags = 0;
    اگر (filter->action.ptr.flags & ACTION_ABSOLUTE_X)
        flags |= XKB_اشارہ_گر_MOTION_ABSOLUTE_X;
    اگر (filter->action.ptr.flags & ACTION_ABSOLUTE_Y)
        flags |= XKB_اشارہ_گر_MOTION_ABSOLUTE_Y;
    اگر (filter->action.ptr.flags & ACTION_REPEAT)
        flags |= XKB_اشارہ_گر_MOTION_REPEATS;
    darray_append(events->قطار, (ساخت xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_اشارہ_گر_MOTION,
        .اشارہ_گر_motion = {
            .سائز = حجم(((ساخت xkb_event*)0)->اشارہ_گر_motion),
            .flags = flags,
            .x = filter->action.ptr.x,
            .y = filter->action.ptr.y,
        }
    });
}

ساکن خالی
xkb_filter_اشارہ_گر_move_new(ساخت xkb_server_state *state,
                            ساخت xkb_events *events,
                            ساخت xkb_filter *filter)
{
    اگر (!events) {
        filter->func = تہی;
        واپس;
    }

    أكد(state->بنیاد.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_کلیدیں);
    append_اشارہ_گر_move(events, filter);
    state->flags &= ~STATE_REQUIRE_کلید_EVENT;
}

ساکن bool
xkb_filter_اشارہ_گر_move_func(ساخت xkb_server_state *state,
                             ساخت xkb_events *events,
                             ساخت xkb_filter *filter,
                             ثابت ساخت xkb_کلید *کلید,
                             شمار xkb_کلید_direction direction)
{
    اگر (کلید != filter->کلید || !events)
        واپس XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    انتخاب (direction) {
    حالت XKB_کلید_نیچے_کو:
        filter->refcnt++;
        append_اشارہ_گر_move(events, filter);
        واپس XKB_FILTER_CONSUME;
    حالت XKB_کلید_REPEATED:
        اگر (filter->action.ptr.flags & ACTION_REPEAT)
            append_اشارہ_گر_move(events, filter);
        واپس XKB_FILTER_CONSUME;
    طےشدہ:
        اگر (--filter->refcnt > 0)
            واپس XKB_FILTER_CONSUME;
    }

    filter->func = تہی;
    واپس XKB_FILTER_CONSUME;
}

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساکن خالی
xkb_filter_اشارہ_گر_button_new(ساخت xkb_server_state *state,
                              ساخت xkb_events *events,
                              ساخت xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساکن bool
xkb_filter_اشارہ_گر_button_func(ساخت xkb_server_state *state,
                               ساخت xkb_events *events,
                               ساخت xkb_filter *filter,
                               ثابت ساخت xkb_کلید *کلید,
                               شمار xkb_کلید_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساکن خالی
xkb_filter_اشارہ_گر_تالا_button_new(ساخت xkb_server_state *state,
                                   ساخت xkb_events *events,
                                   ساخت xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساکن bool
xkb_filter_اشارہ_گر_تالا_button_func(ساخت xkb_server_state *state,
                                    ساخت xkb_events *events,
                                    ساخت xkb_filter *filter,
                                    ثابت ساخت xkb_کلید *کلید,
                                    شمار xkb_کلید_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساکن خالی
xkb_filter_اشارہ_گر_سیٹ_default_button_new(ساخت xkb_server_state *state,
                                          ساخت xkb_events *events,
                                          ساخت xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساکن bool
xkb_filter_اشارہ_گر_سیٹ_default_button_func(ساخت xkb_server_state *state,
                                           ساخت xkb_events *events,
                                           ساخت xkb_filter *filter,
                                           ثابت ساخت xkb_کلید *کلید,
                                           شمار xkb_کلید_direction direction);

ساکن خالی
xkb_filter_terminate_server_new(ساخت xkb_server_state *state,
                                 ساخت xkb_events *events,
                                 ساخت xkb_filter *filter);

ساکن bool
xkb_filter_terminate_server_func(ساخت xkb_server_state *state,
                                  ساخت xkb_events *events,
                                  ساخت xkb_filter *filter,
                                  ثابت ساخت xkb_کلید *کلید,
                                  شمار xkb_کلید_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساکن خالی
xkb_filter_switch_virtual_console_new(ساخت xkb_server_state *state,
                                      ساخت xkb_events *events,
                                      ساخت xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساکن bool
xkb_filter_switch_virtual_console_func(ساخت xkb_server_state *state,
                                       ساخت xkb_events *events,
                                       ساخت xkb_filter *filter,
                                       ثابت ساخت xkb_کلید *کلید,
                                       شمار xkb_کلید_direction direction);

ساکن اندرونی خالی
clear_all_latches_and_locks(ساخت xkb_server_state *state,
                            ساخت xkb_events *events);

ساکن خالی
xkb_filter_ctrls_new(ساخت xkb_server_state *state,
                     ساخت xkb_events *events,
                     ساخت xkb_filter *filter)
{
    اگر (filter->action.type == ACTION_TYPE_CTRL_سیٹ) {
        /* Set: save the specified controls that are *not* already enabled */
        filter->priv = (uint32_t) (
            (~state->بنیاد.components.controls) & filter->action.ctrls.ctrls
        );
    } ورنہ {
        /* Lock: save the specified controls that *are* already enabled */
        filter->priv = (uint32_t) (
            state->بنیاد.components.controls & filter->action.ctrls.ctrls
        );
    }

    اگر (filter->action.type == ACTION_TYPE_CTRL_سیٹ ||
        !(filter->action.ctrls.flags & ACTION_تالا_NO_تالا)) {
        /* Enable the specified controls that are not already enabled */
        state->بنیاد.components.controls |= filter->action.ctrls.ctrls;
    }
}

ساکن bool
xkb_filter_ctrls_func(ساخت xkb_server_state *state,
                      ساخت xkb_events *events,
                      ساخت xkb_filter *filter,
                      ثابت ساخت xkb_کلید *کلید,
                      شمار xkb_کلید_direction direction)
{
    اگر (کلید != filter->کلید)
        واپس XKB_FILTER_CONTINUE;

    انتخاب (direction) {
    حالت XKB_کلید_نیچے_کو:
        filter->refcnt++;
        /* fallthrough */
    حالت XKB_کلید_REPEATED:
        واپس XKB_FILTER_CONSUME;
    طےشدہ:
        اگر (--filter->refcnt > 0)
            واپس XKB_FILTER_CONSUME;
    }

    اگر (filter->action.type == ACTION_TYPE_CTRL_سیٹ ||
        !(filter->action.ctrls.flags & ACTION_تالا_NO_UNLOCK)) {
        ثابت شمار xkb_action_controls old = state->بنیاد.components.controls;

        /*
         * Set: Disable specified controls that were *not* enabled at key press.
         * Lock: Disable specified controls that *were* enabled at key press.
         */
        state->بنیاد.components.controls &= ~(شمار xkb_action_controls) filter->priv;

        اگر ((old & CONTROL_STICKY_کلیدیں) &&
            !(state->بنیاد.components.controls & CONTROL_STICKY_کلیدیں)) {
            /* Sticky keys were disabled: clear all locks and latches */
            clear_all_latches_and_locks(state, events);
        }
    }

    filter->func = تہی;
    واپس XKB_FILTER_CONTINUE;
}

ساکن bool
append_redirect_کلید_events(ساخت xkb_server_state *state,
                           ساخت xkb_events *events,
                           ثابت ساخت xkb_redirect_کلید_action *redirect,
                           شمار xkb_کلید_direction direction)
{
    شمار xkb_state_component بدلا = 0;
    ثابت xkb_mod_نقاب_t نقاب = redirect->affect_mods;

    /*
     * Reference state: find the last state update in the queue, otherwise
     * use the current state.
     */
    ساخت xkb_event *event;
    ساخت state_components آخری_components = state->بنیاد.components;
    darray_foreach_الٹ(event, events->قطار) {
        اگر (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
            آخری_components = event->components.components;
            توڑو;
        }
    }

    اگر (نقاب) {
        ساخت state_components new = آخری_components;
        new.بنیاد_mods = (new.بنیاد_mods & ~نقاب) | redirect->mods;
        new.latched_mods = (new.latched_mods & ~نقاب) | redirect->mods;
        new.locked_mods = (new.locked_mods & ~نقاب) | redirect->mods;
        new.mods = (new.mods & ~نقاب) | redirect->mods;

        بدلا = get_state_component_changes(&آخری_components, &new);
        اگر (بدلا) {
            darray_append(events->قطار, (ساخت xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = new,
                    .بدلا = بدلا
                }
            });
            /*
             * Commit the temporary modifiers change before the key event
             * it was computed for: consumers must observe the key event
             * under the *redirected* modifiers.
             */
            darray_append(events->قطار, (ساخت xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_چوکھٹا,
            });
        }
    }

    darray_append(events->قطار, (ساخت xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_کلید,
        .کلید = {
            .keycode = redirect->keycode,
            .direction = direction
        }
    });

    اگر (نقاب && بدلا) {
        /* Restore state */
        darray_append(events->قطار, (ساخت xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = آخری_components,
                .بدلا = بدلا
            }
        });
    }

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;
    واپس true;
}

ساکن خالی
xkb_filter_redirect_کلید_new(ساخت xkb_server_state *state,
                            ساخت xkb_events *events,
                            ساخت xkb_filter *filter)
{
    /* Action effectual only with the xkb_machine API and a valid keycode */
    اگر (!events || filter->action.redirect.keycode == XKB_KEYCODE_INVALID) {
        filter->func = تہی;
        واپس;
    }
    append_redirect_کلید_events(state, events, &filter->action.redirect,
                               XKB_کلید_نیچے_کو);
}

ساکن bool
xkb_filter_redirect_کلید_func(ساخت xkb_server_state *state,
                             ساخت xkb_events *events,
                             ساخت xkb_filter *filter,
                             ثابت ساخت xkb_کلید *کلید,
                             شمار xkb_کلید_direction direction)
{
    اگر (کلید != filter->کلید)
        واپس XKB_FILTER_CONTINUE;

    اگر (direction == XKB_کلید_اوپر) {
        append_redirect_کلید_events(state, events,
                                   &filter->action.redirect, XKB_کلید_اوپر);
        filter->func = تہی;
        واپس XKB_FILTER_CONSUME;
     } ورنہ اگر (direction == XKB_کلید_نیچے_کو) {
        ثابت اتحاد xkb_action *actions = تہی;
        ثابت xkb_action_گنتی_t گنتی =
            xkb_کلید_get_actions(&state->بنیاد, کلید, &actions);

        برائے (xkb_action_گنتی_t a = 0; a < گنتی; a++) {
            اگر (actions[a].type == ACTION_TYPE_REDIRECT_کلید &&
                actions[a].redirect.keycode != filter->action.redirect.keycode) {
                    /* One action redirects to a different key: release first */
                    append_redirect_کلید_events(state, events,
                                               &filter->action.redirect,
                                               XKB_کلید_اوپر);
                filter->func = تہی;
                واپس XKB_FILTER_CONTINUE;
            }
        }
    }

    append_redirect_کلید_events(state, events, &filter->action.redirect,
                               direction);
    واپس XKB_FILTER_CONSUME;
}

ساکن ثابت ساخت {
    خالی (*new)(ساخت xkb_server_state *state,
                ساخت xkb_events *events,
                ساخت xkb_filter *filter);
    bool (*func)(ساخت xkb_server_state *state,
                 ساخت xkb_events *events,
                 ساخت xkb_filter *filter,
                 ثابت ساخت xkb_کلید *کلید, شمار xkb_کلید_direction direction);
} filter_action_funcs[ACTION_TYPE_عدد_ENTRIES] = {
    [ACTION_TYPE_MOD_سیٹ]     = { xkb_filter_mod_سیٹ_new,
                                  xkb_filter_mod_سیٹ_func },
    [ACTION_TYPE_MOD_LATCH]   = { xkb_filter_mod_latch_new,
                                  xkb_filter_mod_latch_func },
    [ACTION_TYPE_MOD_تالا]    = { xkb_filter_mod_تالا_new,
                                  xkb_filter_mod_تالا_func },
    [ACTION_TYPE_گروہ_سیٹ]   = { xkb_filter_گروہ_سیٹ_new,
                                  xkb_filter_گروہ_سیٹ_func },
    [ACTION_TYPE_گروہ_LATCH] = { xkb_filter_گروہ_latch_new,
                                  xkb_filter_گروہ_latch_func },
    [ACTION_TYPE_گروہ_تالا]  = { xkb_filter_گروہ_تالا_new,
                                  xkb_filter_گروہ_تالا_func },
    [ACTION_TYPE_PTR_MOVE]    = { xkb_filter_اشارہ_گر_move_new,
                                  xkb_filter_اشارہ_گر_move_func },
    [ACTION_TYPE_PTR_BUTTON]  = { xkb_filter_اشارہ_گر_button_new,
                                  xkb_filter_اشارہ_گر_button_func },
    [ACTION_TYPE_PTR_تالا]    = { xkb_filter_اشارہ_گر_تالا_button_new,
                                  xkb_filter_اشارہ_گر_تالا_button_func },
    [ACTION_TYPE_PTR_DEFAULT] = { xkb_filter_اشارہ_گر_سیٹ_default_button_new,
                                  xkb_filter_اشارہ_گر_سیٹ_default_button_func },
    [ACTION_TYPE_TERMINATE]   = { xkb_filter_terminate_server_new,
                                  xkb_filter_terminate_server_func },
    [ACTION_TYPE_SWITCH_VT]   = { xkb_filter_switch_virtual_console_new,
                                  xkb_filter_switch_virtual_console_func },
    [ACTION_TYPE_CTRL_سیٹ]    = { xkb_filter_ctrls_new,
                                  xkb_filter_ctrls_func },
    [ACTION_TYPE_CTRL_تالا]   = { xkb_filter_ctrls_new,
                                  xkb_filter_ctrls_func },
    [ACTION_TYPE_REDIRECT_کلید] = { xkb_filter_redirect_کلید_new,
                                   xkb_filter_redirect_کلید_func },
};

/**
 * Applies any relevant filters to the key, first from the list of filters
 * that are currently active, then if no filter has claimed the key, possibly
 * apply a new filter from the key action.
 */
ساکن خالی
xkb_filter_apply_all(ساخت xkb_server_state *state,
                     ساخت xkb_events *events,
                     ثابت ساخت xkb_کلید *کلید,
                     شمار xkb_کلید_direction direction)
{
    /* First run through all the currently active filters and see if any of
     * them have consumed this event. */
    bool consumed = false;
    ساخت xkb_filter *filter;
    darray_foreach(filter, state->filters) {
        اگر (!filter->func)
            جاری;

        اگر (filter->func(state, events, filter, کلید, direction) == XKB_FILTER_CONSUME)
            consumed = true;
    }
    اگر (consumed || direction == XKB_کلید_اوپر)
        واپس;

    /* No filter consumed this event, so proceed with the key actions */
    ثابت اتحاد xkb_action *actions = تہی;
    xkb_action_گنتی_t گنتی = xkb_کلید_get_actions(&state->بنیاد, کلید,
                                                         &actions);

    برائے (ساخت {
            xkb_action_گنتی_t گنتی;
            ثابت اتحاد xkb_action *actions;
         } اگلا = {0};
         گنتی;
         گنتی = اگلا.گنتی, اگلا.گنتی = 0, actions = اگلا.actions
    ) {
        /*
         * Process actions sequentially.
         *
         * NOTE: We rely on the parser to disallow multiple modifier or group
         * actions (see `CheckMultipleActionsCategories`). Allowing multiple
         * such actions requires a refactor of the state handling.
         */
        برائے (xkb_action_گنتی_t k = 0; k < گنتی; k++) {
            /*
             * It's possible for the keymap to set action->type explicitly, like
             * so:
             *     interpret XF86_Next_VMode {
             *         action = Private(type=0x86, data="+VMode");
             *     };
             * We don't handle those.
             */
            اگر (actions[k].type >= ACTION_TYPE_عدد_ENTRIES)
                جاری;

            /* Go to next action if no corresponding action handler */
            اگر (!filter_action_funcs[actions[k].type].new)
                جاری;

            /* Add a new filter and run the corresponding initial action */
            filter = xkb_filter_new(state);
            filter->کلید = کلید;
            filter->action = actions[k];

            انتخاب (filter->action.type) {
            حالت ACTION_TYPE_MOD_سیٹ:
                اگر (state->بنیاد.components.controls & CONTROL_STICKY_کلیدیں) {
                    /* Convert modifier set action to a latch */
                    filter->action.type = ACTION_TYPE_MOD_LATCH;
                    اگر (state->a11y_flags & XKB_A11_Y_STICKY_کلیدیں_LATCH_مقصد_تالا) {
                        filter->action.mods.flags |= ACTION_LATCH_مقصد_تالا;
                    }
                }
                /* fallthrough */
            حالت ACTION_TYPE_MOD_LATCH:
            حالت ACTION_TYPE_MOD_تالا: {
                ساکن ثابت شمار xkb_action_flags unlock_mods_on_press
                    = ACTION_تالا_CLEAR     /* SetMods & LatchMods: can unlock */
                    | ACTION_تالا_NO_UNLOCK /* LockMods: cannot unlock */
                    | ACTION_EXPLICIT_UNLOCK_ON_PRESS;
                اگر ((state->flags & STATE_UNLOCK_MODS_ON_PRESS) &&
                    /*
                     * Ensure that the action can actually unlock and has
                     * no *explicit* `unlockOnPress` parameter.
                     */
                    (filter->action.mods.flags & unlock_mods_on_press) ==
                    (filter->action.type == ACTION_TYPE_MOD_تالا
                        ? 0 : ACTION_تالا_CLEAR))
                {
                    filter->action.mods.flags |= ACTION_UNLOCK_ON_PRESS;
                }
                توڑو;
            }
            حالت ACTION_TYPE_گروہ_سیٹ:
                اگر (state->بنیاد.components.controls & CONTROL_STICKY_کلیدیں) {
                    /* Convert group set action to a latch */
                    filter->action.type = ACTION_TYPE_گروہ_LATCH;
                    اگر (state->a11y_flags & XKB_A11_Y_STICKY_کلیدیں_LATCH_مقصد_تالا) {
                        filter->action.گروہ.flags |= ACTION_LATCH_مقصد_تالا;
                    }
                }
                توڑو;
            حالت ACTION_TYPE_گروہ_تالا:
                اگر ((filter->action.گروہ.flags &
                     ACTION_AUTO_گروہ_تالا_ON_MODIFIER_RELEASE) &&
                    (state->flags &
                     STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_کلید) &&
                    /*
                     * FIXME: sticky keys may leave an unexpected modifier
                     * from the level 0 latched, so disable for now.
                     */
                    !(state->بنیاد.components.controls & CONTROL_STICKY_کلیدیں))
                {
                    filter->action.گروہ.flags |= ACTION_تالا_ON_RELEASE;
                    /* Add base level action */
                    اگلا.گنتی = xkb_keymap_کلید_get_actions_by_level(
                        state->بنیاد.keymap, کلید, state->بنیاد.components.گروہ,
                        0, &اگلا.actions
                    );
                    أكد(اگلا.گنتی == 1);
                }
                توڑو;
            حالت ACTION_TYPE_PTR_MOVE:
            حالت ACTION_TYPE_PTR_BUTTON:
            حالت ACTION_TYPE_PTR_تالا:
            حالت ACTION_TYPE_PTR_DEFAULT:
                اگر (!(state->بنیاد.components.controls & CONTROL_MOUSE_کلیدیں)) {
                    /* Convert pointer actions to NoAction */
                    أكد(!filter_action_funcs[ACTION_TYPE_NONE].new);
                    filter->func = تہی;
                    جاری;
                }
                توڑو;
            حالت ACTION_TYPE_REDIRECT_کلید:
                // FIXME: this is not efficient to resolve mods here each time
                filter->action.redirect.affect_mods = mod_نقاب_get_effective(
                    state->بنیاد.keymap, filter->action.redirect.affect_mods
                );
                filter->action.redirect.mods = mod_نقاب_get_effective(
                    state->بنیاد.keymap, filter->action.redirect.mods
                );
                توڑو;
            طےشدہ: ;
            }
            filter->func = filter_action_funcs[filter->action.type].func;
            filter_action_funcs[filter->action.type].new(state, events, filter);
        }
    }
}

ساکن خالی
xkb_state_update_derived(ساخت xkb_state *state);

ساکن اندرونی خالی
xkb_state_init(ساخت xkb_state * محدود state,
               ساخت xkb_keymap * محدود keymap,
               شمار xkb_state_منوال_اندرونی منوال)
{
    state->منوال = منوال;
    state->refcnt = 1;
    state->keymap = xkb_keymap_ref(keymap);
    state->برآمد_کا_range_گروہ.policy = XKB_LAYOUT_برآمد_کا_RANGE_WRAP;

    /* Ensure that derived state is correctly initialized */
    xkb_state_update_derived(state);
}

ساکن اندرونی خالی
xkb_client_state_init(ساخت xkb_client_state * محدود state,
                      ساخت xkb_keymap * محدود keymap,
                      شمار xkb_state_منوال_اندرونی منوال)
{
    xkb_state_init(&state->بنیاد, keymap, منوال);
}

ساکن اندرونی خالی
xkb_server_state_init(ساخت xkb_server_state * محدود state,
                      ساخت xkb_keymap * محدود keymap,
                      شمار xkb_state_منوال_اندرونی منوال,
                      شمار xkb_a11y_flags a11y_affect,
                      شمار xkb_a11y_flags a11y_flags)
{
    xkb_state_init(&state->بنیاد, keymap, منوال);

    state->a11y_flags = a11y_flags;
    اگر (keymap->format != XKB_KEYMAP_FORMAT_متن_V1 &&
        !(a11y_affect & XKB_A11_Y_LATCH_بیک_وقت_کلیدیں)) {
             /* Keymap v2+: enable extension to XKB if not manually disabled */
             state->a11y_flags |= XKB_A11_Y_LATCH_بیک_وقت_کلیدیں;
    }
}

ساخت xkb_state *
xkb_state_new(ساخت xkb_keymap *keymap)
{
    ساخت xkb_server_state * ثابت state = صفر_حافظہ_دو(1, حجم(*state));
    اگر (!state) {
        log_err_func1(keymap->ctx, XKB_ERROR_تخصیص_FAILURE_2,
                      "Could not allocate a state object.\n");
        واپس تہی;
    }

    xkb_server_state_init(state, keymap, LEGACY_MIXED_STATE, 0, 0);

    واپس (ساخت xkb_state *)state;
}

ساخت xkb_state *
xkb_state_new_with_منوال(ساخت xkb_keymap * محدود keymap,
                        شمار xkb_state_منوال منوال,
                        شمار xkb_حالت * محدود حالت_2)
{
    انتخاب (منوال) {
    حالت XKB_STATE_منوال_CLIENT:
    حالت XKB_STATE_منوال_SERVER_QUERY: {
        ساخت xkb_client_state * ثابت state = صفر_حافظہ_دو(1, حجم(*state));
        اگر (!state)
            توڑو;

        static_assert((غیرعلامتی)XKB_STATE_منوال_CLIENT ==
                      (غیرعلامتی)CLIENT_STATE, "");
        static_assert((غیرعلامتی)XKB_STATE_منوال_SERVER_QUERY ==
                      (غیرعلامتی)SERVER_COMPANION, "");
        xkb_client_state_init(state, keymap, (شمار xkb_state_منوال_اندرونی)منوال);

        اگر (حالت_2)
            *حالت_2 = XKB_SUCCESS;

        واپس (ساخت xkb_state *)state;
    }
    حالت XKB_STATE_منوال_SERVER: {
        ساخت xkb_server_state * ثابت state = صفر_حافظہ_دو(1, حجم(*state));
        اگر (!state)
            توڑو;

        xkb_server_state_init(state, keymap, LEGACY_SERVER_STATE, 0, 0);

        اگر (حالت_2)
            *حالت_2 = XKB_SUCCESS;

        واپس (ساخت xkb_state *)state;
    }
    طےشدہ:
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_STATE_منوال_2,
                     "Unsupported state mode: %u\n", منوال);
        اگر (حالت_2)
            *حالت_2 = XKB_ERROR_UNSUPPORTED_STATE_منوال;
        واپس تہی;
    }

    log_err_func1(keymap->ctx, XKB_ERROR_تخصیص_FAILURE_2,
                  "Could not allocate a state object.\n");
    اگر (حالت_2)
        *حالت_2 = XKB_ERROR_تخصیص_FAILURE;
    واپس تہی;
}

ساخت xkb_state *
xkb_state_ref(ساخت xkb_state *state)
{
    أكد(state->refcnt > 0);
    state->refcnt++;
    واپس state;
}

خالی
xkb_state_reset(ساخت xkb_state *state)
{
    memset(&state->components, 0, حجم(state->components));

    اگر (state->منوال < LEGACY_MIXED_STATE) {
        static_assert(حجم(*state) == حجم(ساخت xkb_client_state),
                      "Not a mere wrapper");
        /* Nothing else to reset */
    } ورنہ {
        ساخت xkb_server_state *state_ = (ساخت xkb_server_state *)state;
        ثابت ساخت xkb_server_state پچھلا = *state_;
        memset(state_, 0, حجم(*state_));

        state_->بنیاد = پچھلا.بنیاد;
        state_->a11y_flags = پچھلا.a11y_flags;
        state_->filters = پچھلا.filters;
        darray_سائز(state_->filters) = 0;
    }
}

ساکن اندرونی خالی
xkb_state_destroy(ساخت xkb_state *state)
{
    xkb_keymap_unref(state->keymap);
    اگر (state->منوال > SERVER_COMPANION)
        darray_free(((ساخت xkb_server_state *)state)->filters);
}

خالی
xkb_state_unref(ساخت xkb_state *state)
{
    أكد(!state || state->refcnt > 0);
    اگر (!state || --state->refcnt > 0)
        واپس;

    xkb_state_destroy(state);

    انتخاب (state->منوال) {
    حالت CLIENT_STATE:
    حالت SERVER_COMPANION:
        آزاد((ساخت xkb_client_state *)state);
        توڑو;
    حالت LEGACY_MIXED_STATE:
    حالت LEGACY_SERVER_STATE:
        آزاد((ساخت xkb_server_state *)state);
        توڑو;
    حالت SERVER_STATE: /* handled by xkb_machine_unref() */
    طےشدہ:
        PANIC_UNREACHABLE();
    }
}

ساخت xkb_keymap *
xkb_state_get_keymap(ساخت xkb_state *state)
{
    واپس state->keymap;
}

/**
 * Update the LED state to match the rest of the xkb_state.
 */
ساکن خالی
xkb_state_led_update_all(ساخت xkb_state *state)
{
    xkb_led_اشاریہ_t idx;
    ثابت ساخت xkb_led *led;

    state->components.leds = 0;

    xkb_leds_enumerate(idx, led, state->keymap) {

        اگر (led->which_mods != 0 && led->mods.نقاب != 0) {
            xkb_mod_نقاب_t mod_نقاب = 0;
            اگر (led->which_mods & XKB_STATE_MODS_EFFECTIVE)
                mod_نقاب |= state->components.mods;
            اگر (led->which_mods & XKB_STATE_MODS_DEPRESSED)
                mod_نقاب |= state->components.بنیاد_mods;
            اگر (led->which_mods & XKB_STATE_MODS_LATCHED)
                mod_نقاب |= state->components.latched_mods;
            اگر (led->which_mods & XKB_STATE_MODS_LOCKED)
                mod_نقاب |= state->components.locked_mods;

            اگر (led->mods.نقاب & mod_نقاب) {
                state->components.leds |= (UINT32_C(1) << idx);
                جاری;
            }
        }

        اگر (led->which_groups != 0) {
            اگر (likely(led->groups) != 0) {
                xkb_layout_نقاب_t گروہ_نقاب = 0;
                /* Effective and locked groups have been brought into range */
                أكد(state->components.گروہ < XKB_MAX_GROUPS);
                أكد(state->components.locked_گروہ >= 0 &&
                       state->components.locked_گروہ < XKB_MAX_GROUPS);
                /* Effective and locked groups are used as mask */
                اگر (led->which_groups & XKB_STATE_LAYOUT_EFFECTIVE)
                    گروہ_نقاب |= (UINT32_C(1) << state->components.گروہ);
                اگر (led->which_groups & XKB_STATE_LAYOUT_LOCKED)
                    گروہ_نقاب |= (UINT32_C(1) << state->components.locked_گروہ);
                /* Base and latched groups only have to be non-zero */
                اگر ((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                    state->components.بنیاد_گروہ != 0)
                    گروہ_نقاب |= led->groups;
                اگر ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                    state->components.latched_گروہ != 0)
                    گروہ_نقاب |= led->groups;

                اگر (led->groups & گروہ_نقاب) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    جاری;
                }
            } ورنہ {
                /* Special case for Base and latched groups */
                اگر (((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                     state->components.بنیاد_گروہ == 0) ||
                    ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                     state->components.latched_گروہ == 0)) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    جاری;
                }
            }
        }

        اگر (led->ctrls & state->components.controls) {
            state->components.leds |= (UINT32_C(1) << idx);
            جاری;
        }
    }
}

/**
 * Calculates the derived state (effective mods/group and LEDs) from an
 * up-to-date xkb_state.
 */
ساکن خالی
xkb_state_update_derived(ساخت xkb_state *state)
{
    xkb_layout_اشاریہ_t wrapped;

    state->components.mods = (state->components.بنیاد_mods |
                              state->components.latched_mods |
                              state->components.locked_mods);

    /* Lock group must be adjusted, but not base nor latched groups */
    wrapped = Xkb_Wrap_گروہ_Into_Range(state->components.locked_گروہ,
                                    state->keymap->عدد_groups,
                                    state->برآمد_کا_range_گروہ.policy,
                                    state->برآمد_کا_range_گروہ.redirect_گروہ);
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->components.locked_گروہ =
        (int32_t) (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    /* Effective group must be adjusted */
    wrapped = Xkb_Wrap_گروہ_Into_Range(state->components.بنیاد_گروہ +
                                    state->components.latched_گروہ +
                                    state->components.locked_گروہ,
                                    state->keymap->عدد_groups,
                                    state->برآمد_کا_range_گروہ.policy,
                                    state->برآمد_کا_range_گروہ.redirect_گروہ);
    state->components.گروہ =
        (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    xkb_state_led_update_all(state);
}

ساکن خالی
state_update_enabled_controls(ساخت xkb_server_state *state,
                              شمار xkb_keyboard_control_flags affect_controls,
                              شمار xkb_keyboard_control_flags controls,
                              ساخت xkb_events *events)
{
    ثابت bool had_sticky_کلیدیں = state->بنیاد.components.controls
                               & CONTROL_STICKY_کلیدیں;

    /*
     * Enable using the public API with the all the Control values, except
     * the internal ones, if any.
     */
    affect_controls =
        ( affect_controls
        & (شمار xkb_keyboard_control_flags)CONTROL_ALL_BOOLEAN );
    state->بنیاد.components.controls &=
        (شمار xkb_action_controls)~affect_controls;
    state->بنیاد.components.controls |=
        (شمار xkb_action_controls)(controls & affect_controls);

    اگر (had_sticky_کلیدیں &&
        !(state->بنیاد.components.controls & CONTROL_STICKY_کلیدیں)) {
        /* Sticky keys were disabled: clear all locks and latches */
        clear_all_latches_and_locks(state, events);
    }

    xkb_state_update_derived(&state->بنیاد);
}

/**
 * Given a particular key event, updates the state structure to reflect the
 * new modifiers.
 */
شمار xkb_state_component
xkb_state_update_کلید(ساخت xkb_state *بنیاد_state, xkb_keycode_t kc,
                     شمار xkb_کلید_direction direction)
{
    /* Guard against client-only state */
    أكد(بنیاد_state->منوال > SERVER_COMPANION);
    اگر (بنیاد_state->منوال <= SERVER_COMPANION) {
        log_err(بنیاد_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_منوال,
                "%s: Unexpected state type %d\n", __func__, بنیاد_state->منوال);
        واپس 0;
    }

    ساخت xkb_server_state * ثابت state =
        (ساخت xkb_server_state *)بنیاد_state;

    ثابت ساخت xkb_کلید* ثابت کلید = Xkb_کلید(state->بنیاد.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    اگر (!کلید || (direction == XKB_کلید_REPEATED && !کلید->repeats))
        واپس 0;

    اگر (direction == XKB_کلید_نیچے_کو &&
        (state->بنیاد.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیدیں) &&
        (state->a11y_flags & XKB_A11_Y_STICKY_کلیدیں_NO_بیک_وقت_کلیدیں) &&
        state->بنیاد.components.بنیاد_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیدیں,
                                      0, تہی);
    }

    ثابت ساخت state_components پچھلا_components = state->بنیاد.components;

    state->سیٹ_mods = 0;
    state->clear_mods = 0;

    xkb_filter_apply_all(state, تہی, کلید, direction);

    xkb_mod_اشاریہ_t i;
    xkb_mod_نقاب_t ثنائی_ہندسہ;
    برائے (i = 0, ثنائی_ہندسہ = 1; state->سیٹ_mods; i++, ثنائی_ہندسہ <<= 1) {
        اگر (state->سیٹ_mods & ثنائی_ہندسہ) {
            state->mod_کلید_گنتی[i]++;
            state->بنیاد.components.بنیاد_mods |= ثنائی_ہندسہ;
            state->سیٹ_mods &= ~ثنائی_ہندسہ;
        }
    }

    برائے (i = 0, ثنائی_ہندسہ = 1; state->clear_mods; i++, ثنائی_ہندسہ <<= 1) {
        اگر (state->clear_mods & ثنائی_ہندسہ) {
            state->mod_کلید_گنتی[i]--;
            اگر (state->mod_کلید_گنتی[i] <= 0) {
                state->بنیاد.components.بنیاد_mods &= ~ثنائی_ہندسہ;
                state->mod_کلید_گنتی[i] = 0;
            }
            state->clear_mods &= ~ثنائی_ہندسہ;
        }
    }

    xkb_state_update_derived(&state->بنیاد);

    واپس get_state_component_changes(&پچھلا_components, &state->بنیاد.components);
}

/* We need fake keys for `update_latch_modifiers` and `update_latch_group`.
 * These keys must have at least one level in order to break latches. We need 2
 * keys with specific actions in order to update group/mod latches without
 * affecting each other. */
ساکن ساخت xkb_کلید_type_اندراج synthetic_کلید_level_اندراج = { 0 };
ساکن ساخت xkb_کلید_type synthetic_کلید_type = {
     .عدد_entries = 1,
     .عدد_سطحیں = 1,
     .entries = &synthetic_کلید_level_اندراج
};
ساکن ثابت ساخت xkb_کلید synthetic_کلید = { 0 };

/* Transcription from xserver: XkbLatchModifiers */
ساکن خالی
update_latch_modifiers(ساخت xkb_server_state *state,
                       ساخت xkb_events *events,
                       xkb_mod_نقاب_t نقاب, xkb_mod_نقاب_t latches)
{
    /* Clear affected latched modifiers */
    ثابت xkb_mod_نقاب_t clear = نقاب & ~latches;
    state->بنیاد.components.latched_mods &= ~clear;

    /* Clear any pending latch to locks using ad hoc action:
     * only affect corresponding modifier latches and no group latch. */
    ساخت xkb_level synthetic_کلید_level_break_mod_latch = {
        .عدد_syms = 0,
        .عدد_actions = 1,
        .بالائی = XKB_کلید_No_علامت,
        .s.sym = XKB_کلید_No_علامت,
        .a.action.اندرونی_2 = {
            .type = ACTION_TYPE_اندرونی,
            .flags = اندرونی_BREAKS_MOD_LATCH,
            .clear_latched_mods = clear
        }
    };
    ساخت xkb_گروہ synthetic_کلید_گروہ_break_mod_latch = {
        .type = &synthetic_کلید_type,
        .سطحیں = &synthetic_کلید_level_break_mod_latch
    };
    ثابت ساخت xkb_کلید synthetic_کلید_break_mod_latch = {
        .عدد_groups = 1,
        .groups = &synthetic_کلید_گروہ_break_mod_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_کلید_break_mod_latch, XKB_کلید_نیچے_کو);

    /* Finally set the latched mods by simulate tapping a key with the
     * corresponding action */
    ثابت ساخت xkb_کلید* ثابت کلید = &synthetic_کلید;
    ثابت اتحاد xkb_action latch_mods = {
        .mods = {
            .type = ACTION_TYPE_MOD_LATCH,
            .mods = { .نقاب = نقاب & latches },
            .flags = 0,
        },
    };
    ساخت xkb_filter* ثابت filter = xkb_filter_new(state);
    filter->کلید = کلید;
    filter->func = xkb_filter_mod_latch_func;
    filter->action = latch_mods;
    xkb_filter_mod_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_mod_latch_func(state, events, filter, کلید, XKB_کلید_اوپر);
}

/* Transcription from xserver: XkbLatchGroup */
ساکن خالی
update_latch_گروہ(ساخت xkb_server_state *state,
                   ساخت xkb_events *events, int32_t گروہ)
{
    /* Clear any pending latch to locks. */
    ساکن ساخت xkb_level synthetic_کلید_level_break_گروہ_latch = {
        .عدد_syms = 0,
        .عدد_actions = 1,
        .بالائی = XKB_کلید_No_علامت,
        .s.sym = XKB_کلید_No_علامت,
        .a.action.اندرونی_2 = {
            .type = ACTION_TYPE_اندرونی,
            .flags = اندرونی_BREAKS_گروہ_LATCH,
            .clear_latched_mods = 0
        }
    };
    ساکن ساخت xkb_گروہ synthetic_کلید_گروہ_break_گروہ_latch = {
        .type = &synthetic_کلید_type,
        .سطحیں = &synthetic_کلید_level_break_گروہ_latch
    };
    ساکن ثابت ساخت xkb_کلید synthetic_کلید_break_گروہ_latch = {
        .عدد_groups = 1,
        .groups = &synthetic_کلید_گروہ_break_گروہ_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_کلید_break_گروہ_latch, XKB_کلید_نیچے_کو);

    /* Simulate tapping a key with a group latch action, but in isolation: i.e.
     * without affecting the other filters. */
    ثابت ساخت xkb_کلید* ثابت کلید = &synthetic_کلید;
    ثابت اتحاد xkb_action latch_گروہ = {
        .گروہ = {
            .type = ACTION_TYPE_گروہ_LATCH,
            .flags = ACTION_ABSOLUTE_SWITCH,
            .گروہ = گروہ,
        },
    };
    ساخت xkb_filter* ثابت filter = xkb_filter_new(state);
    filter->کلید = کلید;
    filter->func = xkb_filter_گروہ_latch_func;
    filter->action = latch_گروہ;
    xkb_filter_گروہ_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_گروہ_latch_func(state, events, filter, کلید, XKB_کلید_اوپر);
}

/**
 * Compute the resolved effective mask of an arbitrary input.
 *
 * Contrary to `mod_mask_get_effective`, it resolves only modifiers not present
 * in the canonical mask, so that it enables `xkb_state_serialize_mods` to
 * round trip via `xkb_state_update_mask`.
 */
ساکن اندرونی xkb_mod_نقاب_t
resolve_مقصد_canonical_mods(ساخت xkb_keymap *keymap, xkb_mod_نقاب_t mods)
{
    واپس
        /*
         * Keep canonical modifier mask.
         * It contains either real modifiers or canonical virtual modifiers.
         */
        (mods & keymap->canonical_state_نقاب) |
        /* Resolve other modifiers */
        mod_نقاب_get_effective(keymap,
                               mods & ~keymap->canonical_state_نقاب);
}

ساکن خالی
state_update_latched_locked(
    ساخت xkb_server_state * محدود state,
    ثابت ساخت xkb_state_components_update * محدود update,
    ساخت xkb_events * محدود events
)
{
    /* Update locks */
    ثابت xkb_mod_نقاب_t affect_locked_mods =
        resolve_مقصد_canonical_mods(state->بنیاد.keymap, update->affect_locked_mods);
    اگر ((update->components & XKB_STATE_MODS_LOCKED) &&
        affect_locked_mods) {
        ثابت xkb_mod_نقاب_t locked_mods =
            resolve_مقصد_canonical_mods(state->بنیاد.keymap, update->locked_mods);
        state->بنیاد.components.locked_mods &= ~affect_locked_mods;
        state->بنیاد.components.locked_mods |= locked_mods & affect_locked_mods;
    }
    اگر (update->components & XKB_STATE_LAYOUT_LOCKED) {
        state->بنیاد.components.locked_گروہ = update->locked_layout;
    }

    /* Update latches */
    ثابت xkb_mod_نقاب_t affect_latched_mods =
        resolve_مقصد_canonical_mods(state->بنیاد.keymap, update->affect_latched_mods);
    اگر ((update->components & XKB_STATE_MODS_LATCHED) &&
        affect_latched_mods) {
        ثابت xkb_mod_نقاب_t latched_mods =
            resolve_مقصد_canonical_mods(state->بنیاد.keymap, update->latched_mods);
        update_latch_modifiers(state, events, affect_latched_mods, latched_mods);
    }
    اگر (update->components & XKB_STATE_LAYOUT_LATCHED) {
        update_latch_گروہ(state, events, update->latched_layout);
    }
}

شمار xkb_state_component
xkb_state_update_latched_locked(ساخت xkb_state *بنیاد_state,
                                xkb_mod_نقاب_t affect_latched_mods,
                                xkb_mod_نقاب_t latched_mods,
                                bool affect_latched_layout,
                                int32_t latched_layout,
                                xkb_mod_نقاب_t affect_locked_mods,
                                xkb_mod_نقاب_t locked_mods,
                                bool affect_locked_layout,
                                int32_t locked_layout)
{
    /* Guard against client-only state */
    أكد(بنیاد_state->منوال > SERVER_COMPANION);
    اگر (بنیاد_state->منوال <= SERVER_COMPANION) {
        log_err(بنیاد_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_منوال,
                "%s: Unexpected state type %d\n", __func__, بنیاد_state->منوال);
        واپس 0;
    }

    ساخت xkb_server_state * ثابت state =
        (ساخت xkb_server_state *)بنیاد_state;

    ثابت ساخت state_components پچھلا_components_2 = state->بنیاد.components;
    ثابت شمار xkb_state_component components
        = ((affect_latched_mods || latched_mods) ? XKB_STATE_MODS_LATCHED : 0)
        | ((affect_locked_mods || locked_mods) ? XKB_STATE_MODS_LOCKED : 0)
        | (affect_latched_layout ? XKB_STATE_LAYOUT_LATCHED : 0)
        | (affect_locked_layout ? XKB_STATE_LAYOUT_LOCKED : 0);
    ثابت ساخت xkb_state_components_update update = {
        .سائز = حجم(update),
        .components = components,
        .affect_latched_mods = affect_latched_mods,
        .latched_mods = latched_mods,
        .latched_layout = latched_layout,
        .affect_locked_mods = affect_locked_mods,
        .locked_mods = locked_mods,
        .locked_layout = locked_layout
    };
    state_update_latched_locked(state, &update, تہی);
    xkb_state_update_derived(&state->بنیاد);
    واپس get_state_component_changes(&پچھلا_components_2,
                                       &state->بنیاد.components);
}

ساکن اندرونی خالی
clear_all_latches_and_locks(ساخت xkb_server_state *state,
                            ساخت xkb_events *events)
{
    ساکن ثابت شمار xkb_state_component components
        = XKB_STATE_MODS_LATCHED | XKB_STATE_MODS_LOCKED
        | XKB_STATE_LAYOUT_LATCHED | XKB_STATE_LAYOUT_LOCKED;
    ثابت ساخت xkb_state_components_update update = {
        .سائز = حجم(update),
        .components = components,
        .affect_latched_mods = (xkb_mod_نقاب_t)XKB_MOD_ALL,
        .latched_mods = 0,
        .latched_layout = 0,
        .affect_locked_mods = (xkb_mod_نقاب_t)XKB_MOD_ALL,
        .locked_mods = 0,
        .locked_layout = 0
    };
    state_update_latched_locked(state, &update, events);
}

ساکن شمار xkb_حالت
state_update_layout_policy(ساخت xkb_server_state *state,
                           ثابت ساخت xkb_layout_policy_update *update)
{
    اگر (xkb_feature_supported(XKB_FEATURE_ENUM_LAYOUT_برآمد_کا_RANGE_POLICY,
                              update->policy)) {
        اگر (update->policy == XKB_LAYOUT_برآمد_کا_RANGE_REDIRECT) {
            اگر (update->redirect < state->بنیاد.keymap->عدد_groups) {
                state->بنیاد.برآمد_کا_range_گروہ.redirect_گروہ =
                    update->redirect;
            } ورنہ {
                log_err(state->بنیاد.keymap->ctx,
                        XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                        "Layout policy: "
                        "unsupported layout index %"PRIu32" > %"PRIu32"\n",
                        update->redirect + 1, state->بنیاد.keymap->عدد_groups);
                واپس XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ;
            }
        }
        state->بنیاد.برآمد_کا_range_گروہ.policy = update->policy;
        واپس XKB_SUCCESS;
    } ورنہ {
        log_err(state->بنیاد.keymap->ctx,
                XKB_ERROR_UNSUPPORTED_LAYOUT_برآمد_کا_RANGE_POLICY_2,
                "Unsupported layout policy: %d\n", update->policy);
        واپس XKB_ERROR_UNSUPPORTED_LAYOUT_برآمد_کا_RANGE_POLICY;
    }
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
ساکن شمار xkb_حالت
check_state_update_abi_(ساخت xkb_context * محدود ctx,
                        ثابت حرف * محدود func,
                        ثابت ساخت xkb_synthetic_update * محدود update)
{
    شمار xkb_حالت حالت_2 = XKB_SUCCESS;
    اگر ((حالت_2 = xkb_check_state_abi(update)) ||
        (update->reserved0 != 0 &&
         (حالت_2 = XKB_ERROR_ABI_FORWARD_COMPAT)) ||
        (update->components &&
         (حالت_2 = xkb_check_state_abi(update->components))) ||
        (update->layout_policy &&
         (حالت_2 = xkb_check_state_abi(update->layout_policy)))) {
        xkb_log_abi_error(ctx, func, حالت_2);
    }
    واپس حالت_2;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

#تعریف_کرو check_state_update_abi(ctx, update) \
    check_state_update_abi_(ctx, __func__, update)

شمار xkb_حالت
xkb_state_update_synthetic(ساخت xkb_state * بنیاد_state,
                           ثابت ساخت xkb_synthetic_update * update,
                           شمار xkb_state_component *بدلا)
{
    /* Guard against client-only state */
    اگر (بنیاد_state->منوال <= SERVER_COMPANION) {
        log_err(بنیاد_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_منوال,
                "%s: Unexpected state type %d\n", __func__, بنیاد_state->منوال);
        واپس XKB_ERROR_UNEXPECTED_STATE_منوال;
    }

    ساخت xkb_server_state * ثابت state =
        (ساخت xkb_server_state *)بنیاد_state;

    /* Check ABI compatibility */
    شمار xkb_حالت حالت_2 = check_state_update_abi(state->بنیاد.keymap->ctx,
                                                    update);
    اگر (حالت_2 != XKB_SUCCESS)
        واپس حالت_2;

    ثابت ساخت state_components پچھلا_components_2 = state->بنیاد.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    /* Update parametrized controls first */
    اگر (update->layout_policy) {
        حالت_2 = state_update_layout_policy(state, update->layout_policy);
        اگر (حالت_2 != XKB_SUCCESS)
            واپس حالت_2;
    }

    اگر (update->components) {
        ثابت ساخت xkb_state_components_update * ثابت components =
            update->components;
        /* Update boolean controls first */
        اگر (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, تہی);
        }

        state_update_latched_locked(state, components, تہی);
    }

    xkb_state_update_derived(&state->بنیاد);

    اگر (بدلا) {
        *بدلا = get_state_component_changes(&پچھلا_components_2,
                                               &state->بنیاد.components);
    }
    واپس XKB_SUCCESS;
}

/**
 * Updates the state from a set of explicit masks as gained from
 * xkb_state_serialize_mods and xkb_state_serialize_groups.  As noted in the
 * documentation for these functions in xkbcommon.h, this round-trip is
 * lossy, and should only be used to update a slave state mirroring the
 * master, e.g. in a client/server window system.
 */
شمار xkb_state_component
xkb_state_update_نقاب(ساخت xkb_state *بنیاد_state,
                      xkb_mod_نقاب_t بنیاد_mods,
                      xkb_mod_نقاب_t latched_mods,
                      xkb_mod_نقاب_t locked_mods,
                      xkb_layout_اشاریہ_t بنیاد_گروہ,
                      xkb_layout_اشاریہ_t latched_گروہ,
                      xkb_layout_اشاریہ_t locked_گروہ)
{
    /* Guard against server-only and server companion state */
    أكد(بنیاد_state->منوال == CLIENT_STATE ||
           بنیاد_state->منوال == LEGACY_MIXED_STATE);
    اگر (بنیاد_state->منوال != CLIENT_STATE &&
        بنیاد_state->منوال != LEGACY_MIXED_STATE) {
        log_err(بنیاد_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_منوال,
                "%s: Unexpected state type %d\n", __func__, بنیاد_state->منوال);
        واپس 0;
    }

    ساخت xkb_client_state * ثابت state =
        (ساخت xkb_client_state *)بنیاد_state;

    ثابت ساخت state_components پچھلا_components = state->بنیاد.components;

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
    state->بنیاد.components.بنیاد_mods =
        resolve_مقصد_canonical_mods(state->بنیاد.keymap, بنیاد_mods);
    state->بنیاد.components.latched_mods =
        resolve_مقصد_canonical_mods(state->بنیاد.keymap, latched_mods);
    state->بنیاد.components.locked_mods =
        resolve_مقصد_canonical_mods(state->بنیاد.keymap, locked_mods);

    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->بنیاد.components.بنیاد_گروہ = (int32_t) بنیاد_گروہ;
    state->بنیاد.components.latched_گروہ = (int32_t) latched_گروہ;
    state->بنیاد.components.locked_گروہ = (int32_t) locked_گروہ;

    xkb_state_update_derived(&state->بنیاد);

    واپس get_state_component_changes(&پچھلا_components, &state->بنیاد.components);
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Lock_Modifier
 */
ساکن bool
should_do_caps_transformation(ساخت xkb_state *state, xkb_keycode_t kc)
{
    واپس
        xkb_state_mod_اشاریہ_is_فعال(state, XKB_MOD_اشاریہ_CAPS,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_اشاریہ_is_consumed(state, kc, XKB_MOD_اشاریہ_CAPS) == 0;
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Control_Modifier
 */
ساکن bool
should_do_ctrl_transformation(ساخت xkb_state *state, xkb_keycode_t kc)
{
    واپس
        xkb_state_mod_اشاریہ_is_فعال(state, XKB_MOD_اشاریہ_CTRL,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_اشاریہ_is_consumed(state, kc, XKB_MOD_اشاریہ_CTRL) == 0;
}

/**
 * Provides the symbols to use for the given key and state.  Returns the
 * number of symbols pointed to in syms_out.
 */
صحیح
xkb_state_کلید_get_syms(ساخت xkb_state *state, xkb_keycode_t kc,
                       ثابت xkb_keysym_t **syms_برآمد)
{
    ثابت xkb_layout_اشاریہ_t layout = xkb_state_کلید_get_layout(state, kc);
    اگر (layout == XKB_LAYOUT_INVALID)
        جاؤ err;

    ثابت xkb_level_اشاریہ_t level = xkb_state_کلید_get_level(state, kc, layout);
    اگر (level == XKB_LEVEL_INVALID)
        جاؤ err;

    ثابت ساخت xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);

    اگر (!کلید)
        جاؤ err;

    ثابت ساخت xkb_level* ثابت leveli =
        xkb_keymap_کلید_get_level(state->keymap, کلید, layout, level);

    اگر (!leveli)
        جاؤ err;

    ثابت xkb_keysym_گنتی_t عدد_syms = leveli->عدد_syms;
    اگر (عدد_syms == 0)
        جاؤ err;

    اگر (should_do_caps_transformation(state, kc)) {
        /* Only simple capitalization rules: keysyms count is unchanged. */
        اگر (عدد_syms > 1) {
            *syms_برآمد = (leveli->has_بالائی)
                      ? leveli->s.syms + عدد_syms
                      : leveli->s.syms;
        } ورنہ {
            *syms_برآمد = &leveli->بالائی;
        }
    } ورنہ {
        *syms_برآمد = (عدد_syms > 1)
                  ? leveli->s.syms
                  : &leveli->s.sym;
    }
    واپس (صحیح) عدد_syms;

err:
    *syms_برآمد = تہی;
    واپس 0;
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
ساکن حرف
Xkb_مقصد_Control(حرف ch)
{
    حرف c = ch;

    اگر ((c >= '@' && c < '\177') || c == ' ')
        c &= 0x1F;
    ورنہ اگر (c == '2')
        c = '\000';
    ورنہ اگر (c >= '3' && c <= '7')
        c -= ('3' - '\033');
    ورنہ اگر (c == '8')
        c = '\177';
    ورنہ اگر (c == '/')
        c = '_' & 0x1F;
    واپس c;
}

/**
 * Provides either exactly one symbol, or XKB_KEY_NoSymbol.
 */
xkb_keysym_t
xkb_state_کلید_get_one_sym(ساخت xkb_state *state, xkb_keycode_t kc)
{
    ثابت xkb_keysym_t *syms = تہی;
    ثابت صحیح عدد_syms = xkb_state_کلید_get_syms(state, kc, &syms);

    اگر (عدد_syms != 1)
        واپس XKB_کلید_No_علامت;
    ورنہ
        واپس syms[0];
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
ساکن xkb_keysym_t
get_one_sym_for_string(ساخت xkb_state *state, xkb_keycode_t kc)
{
    ثابت xkb_layout_اشاریہ_t layout = xkb_state_کلید_get_layout(state, kc);
    ثابت xkb_layout_اشاریہ_t عدد_layouts =
        xkb_keymap_عدد_layouts_for_کلید(state->keymap, kc);
    xkb_level_اشاریہ_t level = xkb_state_کلید_get_level(state, kc, layout);
    اگر (layout == XKB_LAYOUT_INVALID || عدد_layouts == 0 ||
        level == XKB_LEVEL_INVALID)
        واپس XKB_کلید_No_علامت;

    ثابت xkb_keysym_t *syms = تہی;
    صحیح nsyms = xkb_keymap_کلید_get_syms_by_level(state->keymap, kc,
                                                 layout, level, &syms);
    اگر (nsyms != 1)
        واپس XKB_کلید_No_علامت;
    xkb_keysym_t sym = syms[0];

    اگر (should_do_ctrl_transformation(state, kc) && sym > 127u) {
        برائے (xkb_layout_اشاریہ_t i = 0; i < عدد_layouts; i++) {
            level = xkb_state_کلید_get_level(state, kc, i);
            اگر (level == XKB_LEVEL_INVALID)
                جاری;

            nsyms = xkb_keymap_کلید_get_syms_by_level(state->keymap, kc,
                                                     i, level, &syms);
            اگر (nsyms == 1 && syms[0] <= 127u) {
                sym = syms[0];
                توڑو;
            }
        }
    }

    اگر (should_do_caps_transformation(state, kc)) {
        sym = xkb_keysym_مقصد_بالائی(sym);
    }

    واپس sym;
}

صحیح
xkb_state_کلید_get_utf8(ساخت xkb_state *state, xkb_keycode_t kc,
                       حرف *بفر, سائز_t سائز)
{
    صحیح nsyms;
    ثابت xkb_keysym_t *syms = تہی;
    ثابت xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    اگر (sym != XKB_کلید_No_علامت) {
        nsyms = 1; syms = &sym;
    }
    ورنہ {
        nsyms = xkb_state_کلید_get_syms(state, kc, &syms);
    }

    /* Make sure not to truncate in the middle of a UTF-8 sequence. */
    صحیح انحراف = 0;
    حرف tmp[XKB_KEYSYM_UTF8_MAX_سائز];
    برائے (صحیح i = 0; i < nsyms; i++) {
        صحیح ret = xkb_keysym_مقصد_utf8(syms[i], tmp, حجم(tmp));
        اگر (ret <= 0)
            جاؤ err_bad;

        ret--;
        اگر ((سائز_t)انحراف + (سائز_t)ret <= سائز)
            حافظہ_نقل(بفر + انحراف, tmp, (سائز_t)ret);
        انحراف += ret;
    }

    اگر ((سائز_t)انحراف >= سائز)
        جاؤ err_trunc;
    بفر[انحراف] = '\0';

    اگر (!is_valid_utf8(بفر, (سائز_t)انحراف))
        جاؤ err_bad;

    اگر (انحراف == 1 && (غیرعلامتی صحیح) بفر[0] <= 127u &&
        should_do_ctrl_transformation(state, kc))
        بفر[0] = Xkb_مقصد_Control(بفر[0]);

    واپس انحراف;

err_trunc:
    اگر (سائز > 0)
        بفر[سائز - 1] = '\0';
    واپس انحراف;

err_bad:
    اگر (سائز > 0)
        بفر[0] = '\0';
    واپس 0;
}

uint32_t
xkb_state_کلید_get_utf32(ساخت xkb_state *state, xkb_keycode_t kc)
{
    ثابت xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    uint32_t cp = xkb_keysym_مقصد_utf32(sym);

    اگر (cp <= 127u && should_do_ctrl_transformation(state, kc))
        cp = (uint32_t) Xkb_مقصد_Control((حرف) cp);

    واپس cp;
}

/**
 * Serializes the requested modifier state into an xkb_mod_mask_t, with all
 * the same disclaimers as in xkb_state_update_mask.
 */
ساکن اندرونی xkb_mod_نقاب_t
serialize_mods(ثابت ساخت state_components *components,
               شمار xkb_state_component type)
{
    xkb_mod_نقاب_t ret = 0;

    اگر (type & XKB_STATE_MODS_EFFECTIVE)
        واپس components->mods;

    اگر (type & XKB_STATE_MODS_DEPRESSED)
        ret |= components->بنیاد_mods;
    اگر (type & XKB_STATE_MODS_LATCHED)
        ret |= components->latched_mods;
    اگر (type & XKB_STATE_MODS_LOCKED)
        ret |= components->locked_mods;

    واپس ret;
}

xkb_mod_نقاب_t
xkb_state_serialize_mods(ساخت xkb_state *state,
                         شمار xkb_state_component type)
{
    واپس serialize_mods(&state->components, type);
}

/**
 * Serialises the requested group state, with all the same disclaimers as
 * in xkb_state_update_mask.
 */
ساکن اندرونی xkb_layout_اشاریہ_t
serialize_layout(ثابت ساخت state_components *components,
                 شمار xkb_state_component type)
{
    اگر (type & XKB_STATE_LAYOUT_EFFECTIVE)
        واپس components->گروہ;

    int32_t ret = 0;
    اگر (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret += components->بنیاد_گروہ;
    اگر (type & XKB_STATE_LAYOUT_LATCHED)
        ret += components->latched_گروہ;
    اگر (type & XKB_STATE_LAYOUT_LOCKED)
        ret += components->locked_گروہ;

    واپس (xkb_layout_اشاریہ_t)ret;
}

xkb_layout_اشاریہ_t
xkb_state_serialize_layout(ساخت xkb_state *state,
                           شمار xkb_state_component type)
{
    واپس serialize_layout(&state->components, type);
}

xkb_led_نقاب_t
xkb_state_serialize_leds(ثابت ساخت xkb_state *state,
                         شمار xkb_state_component type)
{
    واپس (type & XKB_STATE_LEDS)
        ? state->components.leds
        : 0;
}

ساکن اندرونی شمار xkb_keyboard_control_flags
serialize_controls(ثابت ساخت state_components *components,
                   شمار xkb_state_component type)
{
    واپس (type & XKB_STATE_CONTROLS_EFFECTIVE)
        /*
         * Enable using the public API with the all the Controls values, except
         * the internal ones, if any.
         */
        ? (شمار xkb_keyboard_control_flags)
          (components->controls & CONTROL_ALL_BOOLEAN)
        : 0;
}

شمار xkb_keyboard_control_flags
xkb_state_serialize_controls(ثابت ساخت xkb_state *state,
                             شمار xkb_state_component type)
{
    واپس serialize_controls(&state->components, type);
}

/**
 * Gets a modifier mask and returns the resolved effective mask; this
 * is needed because some modifiers can also map to other modifiers, e.g.
 * the "NumLock" modifier usually also sets the "Mod2" modifier.
 */
xkb_mod_نقاب_t
mod_نقاب_get_effective(ساخت xkb_keymap *keymap, xkb_mod_نقاب_t mods)
{
    /* Initialize the effective mask with its corresponding real mods. */
    xkb_mod_نقاب_t نقاب = mods & MOD_REAL_نقاب_ALL;

    /* Resolve the virtual modifiers */
    ثابت ساخت xkb_mod *mod;
    xkb_mod_اشاریہ_t i;
    xkb_vmods_enumerate(i, mod, &keymap->mods)
        اگر (mods & (UINT32_C(1) << i))
            نقاب |= mod->mapping;

    واپس نقاب;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
صحیح
xkb_state_mod_اشاریہ_is_فعال(ساخت xkb_state *state,
                              xkb_mod_اشاریہ_t idx,
                              شمار xkb_state_component type)
{
    اگر (unlikely(idx >= xkb_keymap_عدد_mods(state->keymap)))
        واپس -1;

    ثابت xkb_mod_نقاب_t mapping = state->keymap->mods.mods[idx].mapping;
    اگر (!mapping) {
        /* Modifier not mapped */
        واپس 0;
    }
    /* WARNING: this may overmatch for virtual modifiers */
    واپس (xkb_state_serialize_mods(state, type) & mapping) == mapping;
}

شمار {
    XKB_STATE_مطابقت_FLAGS = XKB_STATE_مطابقت_ANY
                          | XKB_STATE_مطابقت_ALL
                          | XKB_STATE_مطابقت_NON_EXCLUSIVE
};

/**
 * Helper function for xkb_state_mod_indices_are_active and
 * xkb_state_mod_names_are_active.
 */
ساکن bool
مطابقت_mod_masks(ساخت xkb_state *state,
                شمار xkb_state_component type,
                شمار xkb_state_مطابقت مطابقت,
                xkb_mod_نقاب_t wanted)
{
    ثابت xkb_mod_نقاب_t فعال = xkb_state_serialize_mods(state, type);

    اگر (!(مطابقت & XKB_STATE_مطابقت_NON_EXCLUSIVE) && (فعال & ~wanted))
        واپس false;

    اگر (مطابقت & XKB_STATE_مطابقت_ANY)
        واپس فعال & wanted;

    /* Ensure all flags but XKB_STATE_MATCH_ALL are processed before */
    static_assert(XKB_STATE_مطابقت_ALL ==
                  ((شمار xkb_state_مطابقت) XKB_STATE_مطابقت_FLAGS &
                   ~(XKB_STATE_مطابقت_ANY | XKB_STATE_مطابقت_NON_EXCLUSIVE)),
                  "");

    واپس (فعال & wanted) == wanted;
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, or -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
صحیح
xkb_state_mod_indices_are_فعال(ساخت xkb_state *state,
                                 شمار xkb_state_component type,
                                 شمار xkb_state_مطابقت مطابقت,
                                 ...)
{
    اگر (مطابقت & ~((شمار xkb_state_مطابقت) XKB_STATE_مطابقت_FLAGS))
        واپس -2;

    va_list ap;
    xkb_mod_نقاب_t wanted = 0;
    صحیح ret = 0;
    ثابت xkb_mod_اشاریہ_t عدد_mods = xkb_keymap_عدد_mods(state->keymap);

    va_آغاز(ap, مطابقت);
    جب (1) {
        xkb_mod_اشاریہ_t idx = va_arg(ap, xkb_mod_اشاریہ_t);
        اگر (idx == XKB_MOD_INVALID)
            توڑو;
        اگر (unlikely(idx >= عدد_mods)) {
            ret = -1;
            توڑو;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_انتہا(ap);

    اگر (ret == -1)
        واپس ret;

    اگر (!wanted) {
        /* Modifiers not mapped */
        واپس 0;
    }

    واپس مطابقت_mod_masks(state, type, مطابقت, wanted);
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
صحیح
xkb_state_mod_name_is_فعال(ساخت xkb_state *state, ثابت حرف *name,
                             شمار xkb_state_component type)
{
    ثابت xkb_mod_اشاریہ_t idx = xkb_keymap_mod_get_اشاریہ(state->keymap, name);

    اگر (idx == XKB_MOD_INVALID)
        واپس -1;

    واپس xkb_state_mod_اشاریہ_is_فعال(state, idx, type);
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ATTR_NULL_SENTINEL صحیح
xkb_state_mod_نام_are_فعال(ساخت xkb_state *state,
                               شمار xkb_state_component type,
                               شمار xkb_state_مطابقت مطابقت,
                               ...)
{
    اگر (مطابقت & ~((شمار xkb_state_مطابقت) XKB_STATE_مطابقت_FLAGS))
        واپس -2;

    va_list ap;
    xkb_mod_نقاب_t wanted = 0;
    صحیح ret = 0;

    va_آغاز(ap, مطابقت);
    جب (1) {
        ثابت حرف *str = va_arg(ap, ثابت حرف *);
        اگر (str == تہی)
            توڑو;
        ثابت xkb_mod_اشاریہ_t idx = xkb_keymap_mod_get_اشاریہ(state->keymap, str);
        اگر (idx == XKB_MOD_INVALID) {
            ret = -1;
            توڑو;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_انتہا(ap);

    اگر (ret == -1)
        واپس ret;

    اگر (!wanted) {
        /* Modifiers not mapped */
        واپس 0;
    }

    واپس مطابقت_mod_masks(state, type, مطابقت, wanted);
}

/**
 * Returns 1 if the given group is active with the specified type(s), 0 if
 * not, or -1 if the group is invalid.
 */
صحیح
xkb_state_layout_اشاریہ_is_فعال(ساخت xkb_state *state,
                                 xkb_layout_اشاریہ_t idx,
                                 شمار xkb_state_component type)
{
    اگر (idx >= state->keymap->عدد_groups)
        واپس -1;

    صحیح ret = 0;
    اگر (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ret |= (state->components.گروہ == idx);
    اگر (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret |= (state->components.بنیاد_گروہ == (int32_t) idx);
    اگر (type & XKB_STATE_LAYOUT_LATCHED)
        ret |= (state->components.latched_گروہ == (int32_t) idx);
    اگر (type & XKB_STATE_LAYOUT_LOCKED)
        ret |= (state->components.locked_گروہ == (int32_t) idx);

    واپس ret;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
صحیح
xkb_state_layout_name_is_فعال(ساخت xkb_state *state, ثابت حرف *name,
                                شمار xkb_state_component type)
{
    ثابت xkb_layout_اشاریہ_t idx =
        xkb_keymap_layout_get_اشاریہ(state->keymap, name);

    اگر (idx == XKB_LAYOUT_INVALID)
        واپس -1;

    واپس xkb_state_layout_اشاریہ_is_فعال(state, idx, type);
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
صحیح
xkb_state_led_اشاریہ_is_فعال(ساخت xkb_state *state, xkb_led_اشاریہ_t idx)
{
    اگر (idx >= state->keymap->عدد_leds ||
        state->keymap->leds[idx].name == XKB_ATOM_NONE)
        واپس -1;

    واپس !!(state->components.leds & (UINT32_C(1) << idx));
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
صحیح
xkb_state_led_name_is_فعال(ساخت xkb_state *state, ثابت حرف *name)
{
    ثابت xkb_led_اشاریہ_t idx = xkb_keymap_led_get_اشاریہ(state->keymap, name);

    اگر (idx == XKB_LED_INVALID)
        واپس -1;

    واپس xkb_state_led_اشاریہ_is_فعال(state, idx);
}

/**
 * See:
 * - XkbTranslateKeyCode(3), mod_rtrn return value, from libX11.
 * - MyEnhancedXkbTranslateKeyCode(), a modification of the above, from GTK+.
 */
ساکن xkb_mod_نقاب_t
کلید_get_consumed(ساخت xkb_state *state, ثابت ساخت xkb_کلید *کلید,
                 شمار xkb_consumed_منوال منوال)
{
    ثابت xkb_layout_اشاریہ_t گروہ =
        xkb_state_کلید_get_layout(state, کلید->keycode);
    اگر (گروہ == XKB_LAYOUT_INVALID)
        واپس 0;

    xkb_mod_نقاب_t preserve = 0;
    xkb_mod_نقاب_t consumed = 0;

    ثابت ساخت xkb_کلید_type_اندراج* ثابت matching_اندراج =
        get_اندراج_for_کلید_state(state, کلید, گروہ);
    اگر (matching_اندراج)
        preserve = matching_اندراج->preserve.نقاب;

    ثابت ساخت xkb_کلید_type* ثابت type = کلید->groups[گروہ].type;
    انتخاب (منوال) {
    حالت XKB_CONSUMED_منوال_XKB:
        consumed = type->mods.نقاب;
        توڑو;

    حالت XKB_CONSUMED_منوال_GTK: {
        ثابت ساخت xkb_کلید_type_اندراج* ثابت no_mods_اندراج =
            get_اندراج_for_mods(type, 0);
        ثابت xkb_level_اشاریہ_t no_mods_leveli = no_mods_اندراج
                                               ? no_mods_اندراج->level
                                               : 0;
        ثابت ساخت xkb_level* ثابت no_mods_level =
            &کلید->groups[گروہ].سطحیں[no_mods_leveli];

        برائے (darray_سائز_t i = 0; i < type->عدد_entries; i++) {
            ثابت ساخت xkb_کلید_type_اندراج* ثابت اندراج_2 = &type->entries[i];
            اگر (!اندراج_is_فعال(اندراج_2))
                جاری;

            ثابت ساخت xkb_level* ثابت level =
                &کلید->groups[گروہ].سطحیں[اندراج_2->level];
            اگر (Xkb_سطحیں_Same_Syms(level, no_mods_level))
                جاری;

            اگر (اندراج_2 == matching_اندراج || one_ثنائی_ہندسہ_سیٹ(اندراج_2->mods.نقاب))
                consumed |= اندراج_2->mods.نقاب & ~اندراج_2->preserve.نقاب;
        }
        توڑو;
    }
    }

    واپس consumed & ~preserve;
}

صحیح
xkb_state_mod_اشاریہ_is_consumed2(ساخت xkb_state *state, xkb_keycode_t kc,
                                 xkb_mod_اشاریہ_t idx,
                                 شمار xkb_consumed_منوال منوال)
{
    ثابت ساخت xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);

    اگر (unlikely(!کلید || idx >= xkb_keymap_عدد_mods(state->keymap)))
        واپس -1;

    ثابت xkb_mod_نقاب_t mapping = state->keymap->mods.mods[idx].mapping;
    اگر (!mapping) {
        /* Modifier not mapped */
        واپس 0;
    }
    واپس (mapping & کلید_get_consumed(state, کلید, منوال)) == mapping;
}

صحیح
xkb_state_mod_اشاریہ_is_consumed(ساخت xkb_state *state, xkb_keycode_t kc,
                                xkb_mod_اشاریہ_t idx)
{
    واپس xkb_state_mod_اشاریہ_is_consumed2(state, kc, idx,
                                            XKB_CONSUMED_منوال_XKB);
}

xkb_mod_نقاب_t
xkb_state_mod_نقاب_remove_consumed(ساخت xkb_state *state, xkb_keycode_t kc,
                                   xkb_mod_نقاب_t نقاب)
{
    ثابت ساخت xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);

    اگر (!کلید)
        واپس 0;

    واپس resolve_مقصد_canonical_mods(state->keymap, نقاب) &
           ~کلید_get_consumed(state, کلید, XKB_CONSUMED_منوال_XKB);
}

xkb_mod_نقاب_t
xkb_state_کلید_get_consumed_mods2(ساخت xkb_state *state, xkb_keycode_t kc,
                                 شمار xkb_consumed_منوال منوال)
{
    انتخاب (منوال) {
    حالت XKB_CONSUMED_منوال_XKB:
    حالت XKB_CONSUMED_منوال_GTK:
        توڑو;
    طےشدہ:
        log_err_func(state->keymap->ctx, XKB_LOG_پیغام_NO_ID,
                     "unrecognized consumed modifiers mode: %d\n", منوال);
        واپس 0;
    }

    ثابت ساخت xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);
    اگر (!کلید)
        واپس 0;

    واپس کلید_get_consumed(state, کلید, منوال);
}

xkb_mod_نقاب_t
xkb_state_کلید_get_consumed_mods(ساخت xkb_state *state, xkb_keycode_t kc)
{
    واپس xkb_state_کلید_get_consumed_mods2(state, kc, XKB_CONSUMED_منوال_XKB);
}

/*******************************************************************************
 * Server state machine API
 ******************************************************************************/

/** Entry to keep track of the state of an overlaid key */
ساخت xkb_overlaid_کلید {
    /** The source key being overlaid */
    ثابت ساخت xkb_کلید *old;
    /** The target key */
    ثابت ساخت xkb_کلید *new;
    /** Counter of key currently down */
    صحیح refcnt;
};

ساخت xkb_shortcuts_config_اندراج {
    /** Real modifier mask to trigger shortcuts tweaks */
    xkb_mod_نقاب_t mods;
    /** Target layout */
    xkb_layout_اشاریہ_t ہدف;
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
ساخت xkb_machine {
    /** Inherits from `xkb_server_state` */
    ساخت xkb_server_state بنیاد;

    /* Extra server-specific state data */

    /** Keyboard overlays handling */
    ساخت {
        /**
         * Current overlaid keys
         *
         * When a key is overlaid, it remained overlaid in this configuration
         * until totally released, i.e. every key press was matched with the
         * corresponding key release.
         */
        darray(ساخت xkb_overlaid_کلید) کلیدیں;
        /**
         * Activation order of the overlay
         *
         * Overlays indices are stored 1-indexed in nibbles: the lowest
         * nibble corresponds to the latest activated index.
         */
        uint32_t ترتیب;
        /** Current enabled overlays mask */
        xkb_overlay_نقاب_t enabled;
    } overlays;

    /** Configuration */
    ساخت machine_config {
        /** Modifiers remapping */
        ساخت machine_modifiers_config {
            /** Real modifier mask */
            xkb_mod_نقاب_t نقاب;
            /** Modifiers re-mappings */
            darray_سائز_t mappings_عدد;
            ساخت machine_mods_mapping {
                xkb_mod_نقاب_t ماخذ;
                xkb_mod_نقاب_t ہدف;
            } * mappings ATTR_COUNTED_BY(mappings_عدد);
        } modifiers;

        /** Shortcuts tweak */
        ساخت machine_shortcuts_config {
            /** Substitution entries */
            ساخت xkb_shortcuts_config_اندراج *entries;
        } shortcuts;
    } config;

    /** Mouse keys handling */
    ساخت {
        /** Default pointer button */
        xkb_اشارہ_گر_button_اشاریہ_t default_button;
        /** Mask of the locked buttons */
        xkb_اشارہ_گر_button_نقاب_t locked_buttons;
    } mouse;

    شمار xkb_machine_flags flags;
};

قسمتعریف darray(ساخت machine_mods_mapping) machine_mods_mappings;

ساخت xkb_machine_builder {
    ساخت xkb_keymap *keymap;

    /** Controls */
    ساخت {
        /** Accessibility flags */
        ساخت {
            شمار xkb_a11y_flags affect_flags;
            شمار xkb_a11y_flags flags;
        } a11y;
    } controls;

    /** Modifiers re-mapping */
    machine_mods_mappings mods;

    /** Shortcuts tweak */
    ساخت xkb_shortcuts_config_options {
        /** Substitution entries */
        darray(ساخت xkb_shortcuts_config_اندراج) entries;
    } shortcuts;

    شمار xkb_machine_flags machine_flags;
    شمار xkb_machine_builder_flags builder_flags;
    صحیح refcnt;
};

ساخت xkb_machine_builder *
xkb_machine_builder_new(
    ساخت xkb_keymap * محدود keymap,
    ثابت ساخت xkb_machine_builder_config * محدود config,
    شمار xkb_حالت * محدود حالت_2
)
{
    /* Handle default configuration */
    ساکن ثابت ساخت xkb_machine_builder_config default_config = {
        .سائز = حجم(default_config)
    };
    اگر (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ثابت شمار xkb_حالت حالت_3 = xkb_check_state_abi(config);
    اگر (حالت_3) {
        xkb_log_abi_error(keymap->ctx, __func__, حالت_3);
        اگر (حالت_2)
            *حالت_2 = حالت_3;
        واپس تہی;
    }

    /* Sanitize input */
    ثابت شمار xkb_machine_builder_flags invalid_builder_flags
        = config->builder_flags
        & ~(شمار xkb_machine_builder_flags)XKB_MACHINE_BUILDER_FLAGS_اقدار;
    اگر (invalid_builder_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS_,
                     "unrecognized machine builder flags: 0x%x\n",
                     invalid_builder_flags);
        اگر (حالت_2)
            *حالت_2 = XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS;
        واپس تہی;
    }

    ثابت شمار xkb_machine_flags invalid_machine_flags
        = (شمار xkb_machine_flags)config->machine_flags
        & ~(شمار xkb_machine_flags)XKB_MACHINE_FLAGS_اقدار;
    اگر (invalid_machine_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS_,
                     "unrecognized state machine flags: 0x%x\n",
                     invalid_machine_flags);
        اگر (حالت_2)
            *حالت_2 = XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS;
        واپس تہی;
    }

    ساخت xkb_machine_builder * ثابت builder = صفر_حافظہ_دو(1, حجم(*builder));
    اگر (!builder) {
        log_err_func1(keymap->ctx, XKB_ERROR_تخصیص_FAILURE_2,
                      "cannot allocate machine builder\n");
        اگر (حالت_2)
            *حالت_2 = XKB_ERROR_تخصیص_FAILURE;
        واپس تہی;
    }

    *builder = (ساخت xkb_machine_builder) {
        .keymap = xkb_keymap_ref(keymap),
        .refcnt = 1,
        .builder_flags = (شمار xkb_machine_builder_flags)config->builder_flags,
        .machine_flags = (شمار xkb_machine_flags)config->machine_flags,
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

    اگر (حالت_2)
        *حالت_2 = XKB_SUCCESS;

    واپس builder;
}

ساخت xkb_machine_builder *
xkb_machine_builder_ref(ساخت xkb_machine_builder *builder)
{
    أكد(builder->refcnt > 0);
    builder->refcnt++;
    واپس builder;
}

خالی
xkb_machine_builder_unref(ساخت xkb_machine_builder *builder)
{
    أكد(!builder || builder->refcnt > 0);
    اگر (!builder || --builder->refcnt > 0)
        واپس;

    darray_free(builder->shortcuts.entries);
    darray_free(builder->mods);
    xkb_keymap_unref(builder->keymap);
    آزاد(builder);
}

ساخت xkb_keymap *
xkb_machine_builder_get_keymap(ثابت ساخت xkb_machine_builder *builder)
{
    /* Reference count is not updated. See API doc. */
    واپس builder->keymap;
}

شمار xkb_حالت
xkb_machine_builder_update_a11y(
    ساخت xkb_machine_builder * محدود builder,
    ثابت ساخت xkb_machine_builder_a11y_update * محدود update)
{
    /* Check ABI compatibility */
    شمار xkb_حالت حالت_2 = xkb_check_state_abi(update);
    اگر (حالت_2 != XKB_SUCCESS) {
        xkb_log_abi_error(builder->keymap->ctx, __func__, حالت_2);
        واپس حالت_2;
    }

    ثابت شمار xkb_a11y_flags invalid_flags =
        ~(شمار xkb_a11y_flags)XKB_A11_Y_FLAGS_اقدار;

    اگر (update->affect_flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y affected flags: %#x\n",
                __func__, update->affect_flags & invalid_flags);
        واپس XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }
    اگر (update->flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y flags: %#x\n",
                __func__, update->flags & invalid_flags);
        واپس XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }

    builder->controls.a11y.affect_flags |= update->affect_flags;
    builder->controls.a11y.flags &= ~update->affect_flags;
    builder->controls.a11y.flags |= (update->flags & update->affect_flags);

    واپس XKB_SUCCESS;
}

شمار xkb_حالت
xkb_machine_builder_update_mods_remap(
    ساخت xkb_machine_builder * محدود builder,
    ثابت ساخت xkb_machine_builder_mods_remap_update * محدود update
)
{
    اگر (!update->ماخذ) {
        اگر (!update->ہدف) {
            /* Reset mappings */
            darray_resize(builder->mods, 0);
            واپس XKB_SUCCESS;
        } ورنہ {
            واپس XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب;
        }
    }

    /* Check the modifiers against the keymap */
    ثابت xkb_mod_نقاب_t invalid = ~builder->keymap->canonical_state_نقاب;
    اگر ((update->ماخذ & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب,
                "%s: Invalid source modifiers: 0x%"PRIx32"\n",
                __func__, update->ماخذ);
        واپس XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب;
    }
    اگر ((update->ہدف & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب,
                "%s: Invalid target modifiers: 0x%"PRIx32"\n",
                __func__, update->ہدف);
        واپس XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب;
    }

    ساخت machine_mods_mapping *mapping = تہی;
    darray_سائز_t m = 0;
    darray_enumerate(m, mapping, builder->mods) {
        اگر (mapping->ماخذ == update->ماخذ) {
            اگر (!update->ہدف) {
                /* Remove mapping */
                darray_remove(builder->mods, m);
            } ورنہ {
                /* Update mapping */
                mapping->ہدف = update->ہدف;
            }
            واپس XKB_SUCCESS;
        }
    }

    اگر (update->ہدف) {
        /* Append new mapping */
        darray_append(
            builder->mods,
            (ساخت machine_mods_mapping) {
                .ماخذ = update->ماخذ,
                .ہدف = update->ہدف
            }
        );
    } ورنہ {
        /* Ignore missing mapping */
    }

    واپس XKB_SUCCESS;
}

شمار xkb_حالت
xkb_machine_builder_update_shortcut_override(
    ساخت xkb_machine_builder * محدود builder,
    ثابت ساخت xkb_machine_builder_shortcut_override_update * محدود update
)
{
    ساخت xkb_keymap *keymap = builder->keymap;

    /* Check the modifiers against the keymap */
    ثابت xkb_mod_نقاب_t invalid = ~keymap->canonical_state_نقاب;
    اگر ((update->affect_mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب,
                "%s: Invalid affected modifiers: 0x%"PRIx32"\n",
                __func__, update->affect_mods);
        واپس XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب;
    }
    اگر ((update->mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب,
                "%s: Invalid modifiers: 0x%"PRIx32"\n",
                __func__, update->mods);
        واپس XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب;
    }

    /* Check the layout indices against the keymap */
    اگر (update->ماخذ >= keymap->عدد_groups &&
        update->ماخذ != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                "%s: Invalid source layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->عدد_groups, update->ماخذ + 1);
        واپس XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ;
    }
    اگر (update->ہدف >= keymap->عدد_groups &&
        update->ہدف != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                "%s: Invalid target layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->عدد_groups, update->ہدف + 1);
        واپس XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ;
    }

    ساخت xkb_shortcuts_config_options * ثابت config = &builder->shortcuts;

    /* Resize array & initialize new entries, if relevant */
    اگر (update->ماخذ >= darray_سائز(config->entries) &&
        update->ماخذ != XKB_LAYOUT_INVALID) {
        اگر (update->ہدف == update->ماخذ) {
            /* Skip default setting */
            واپس XKB_SUCCESS;
        }
        xkb_layout_اشاریہ_t new = darray_سائز(config->entries);
        darray_resize(config->entries, update->ماخذ + 1);
        برائے (; new <= update->ماخذ; new++) {
            darray_شے(config->entries, new) =
                (ساخت xkb_shortcuts_config_اندراج) {
                    .ہدف = XKB_LAYOUT_INVALID,
                    .mods = 0
                };
        }
    }

    ساخت xkb_shortcuts_config_اندراج *اندراج_2;
    xkb_layout_اشاریہ_t ماخذ = (update->ماخذ == XKB_LAYOUT_INVALID)
        ? 0               /* Loop over all entries */
        : update->ماخذ; /* Single entry */
    darray_foreach_from(اندراج_2, config->entries, ماخذ) {
        اگر (update->ماخذ == XKB_LAYOUT_INVALID &&
            اندراج_2->ہدف == XKB_LAYOUT_INVALID) {
            /* Skip previous deactivated entry */
            ماخذ++;
            جاری;
        }

        اگر (update->affect_mods) {
            اندراج_2->mods &= ~update->affect_mods;
            اندراج_2->mods |= (update->affect_mods & update->mods);
        }

        اگر (update->ماخذ != XKB_LAYOUT_INVALID) {
            اندراج_2->ہدف = (update->ہدف == ماخذ)
                ? XKB_LAYOUT_INVALID
                : update->ہدف;
            /* Single entry: exit loop */
            توڑو;
        }
        ماخذ++;
    }

    واپس XKB_SUCCESS;
}

/** Compare 2 modifiers combinations */
ساکن صحیح
cmp_mod_masks(ثابت خالی *a, ثابت خالی *b)
{
    ثابت xkb_mod_نقاب_t m1 = *((xkb_mod_نقاب_t *) a);
    ثابت xkb_mod_نقاب_t m2 = *((xkb_mod_نقاب_t *) b);
    اگر (m1 == m2)
        واپس 0;

    اگر (!m1)
        واپس +1;
    اگر (!m2)
        واپس -1;

    ثابت xkb_mod_نقاب_t overlap = (m1 & m2);
    اگر (overlap == m1)
        واپس +1; /* m1 is a subset of m2 */
    اگر (overlap == m2)
        واپس -1; /* m2 is a subset of m1 */

    /* Compare active modifiers numbers */
    ثابت غیرعلامتی صحیح count1 = popcount32(m1);
    ثابت غیرعلامتی صحیح count2 = popcount32(m2);
    اگر (count1 > count2)
        واپس -1;
    اگر (count1 < count2)
        واپس +1;

    /* Give priority to real modifiers */
    واپس (m1 < m2) ? -1 : +1;
}

ساکن شمار xkb_حالت
machine_سیٹ_mods(ساخت xkb_machine *sm,
                 ثابت machine_mods_mappings *raw_mappings)
{
    اگر (!darray_خالی_خانے(*raw_mappings)) {
        machine_mods_mappings mappings = darray_new();
        xkb_mod_نقاب_t نقاب = 0;
        ثابت ساخت machine_mods_mapping *mapping;
        ثابت xkb_mod_نقاب_t invalid =
            ~sm->بنیاد.بنیاد.keymap->canonical_state_نقاب;
        darray_foreach(mapping, *raw_mappings) {
            اگر (!mapping->ماخذ || !mapping->ہدف ||
                (mapping->ماخذ & invalid) || (mapping->ہدف & invalid))
                جاری;
            darray_append(mappings, *mapping);
            نقاب |= mapping->ماخذ;
        }

        /*
         * Re-order the mappings, so that combinations have higher priority
         * if they are superset of other combinations.
         * For other type of overlaps, see the sorting function.
         */
        qsort(darray_اشیاء(mappings), darray_سائز(mappings),
              حجم(*darray_اشیاء(mappings)), &cmp_mod_masks);

        darray_steal(
            mappings,
            &sm->config.modifiers.mappings,
            &sm->config.modifiers.mappings_عدد
        );
        sm->config.modifiers.نقاب = نقاب;
    } ورنہ {
        sm->config.modifiers = (ساخت machine_modifiers_config) {
            .mappings = تہی,
            .mappings_عدد = 0,
            .نقاب = 0
        };
    }

    واپس XKB_SUCCESS;
}

ساکن شمار xkb_حالت
machine_سیٹ_shortcuts(
    ساخت xkb_machine * محدود sm,
    ثابت ساخت xkb_shortcuts_config_options * محدود options
)
{
    اگر (darray_خالی_خانے(options->entries)) {
        sm->config.shortcuts = (ساخت machine_shortcuts_config) {
            .entries = تہی
        };
        واپس XKB_SUCCESS;
    }

    ساخت xkb_keymap * ثابت keymap = sm->بنیاد.بنیاد.keymap;

    ساخت xkb_shortcuts_config_اندراج * entries =
        صفر_حافظہ_دو(keymap->عدد_groups, حجم(*entries));
    اگر (!entries)
        واپس XKB_ERROR_تخصیص_FAILURE;

    ثابت xkb_layout_اشاریہ_t گنتی = MIN(
        keymap->عدد_groups,
        (xkb_layout_اشاریہ_t) darray_سائز(options->entries)
    );

    bool some_entries = false;
    برائے (xkb_layout_اشاریہ_t l = 0; l < گنتی; l++) {
        /* Sanitize layout target */
        ثابت xkb_layout_اشاریہ_t ہدف =
            darray_شے(options->entries, l).ہدف;
        /* Sanitize modifier mask */
        ثابت xkb_mod_نقاب_t mods = (
            darray_شے(options->entries, l).mods &
            sm->بنیاد.بنیاد.keymap->canonical_state_نقاب
        );
        اگر (ہدف < keymap->عدد_groups && ہدف != l && mods) {
            entries[l] = (ساخت xkb_shortcuts_config_اندراج) {
                .ہدف = ہدف,
                .mods = mods
            };
            some_entries = true;
        } ورنہ {
            entries[l] = (ساخت xkb_shortcuts_config_اندراج) {
                .ہدف = XKB_LAYOUT_INVALID,
                .mods = 0
            };
            جاری;
        }
    }

    اگر (!some_entries) {
        آزاد(entries);
        entries = تہی;
    } ورنہ {
        /* Initialize groups with no entries */
        برائے (xkb_layout_اشاریہ_t l = گنتی; l < keymap->عدد_groups; l++) {
            entries[l] = (ساخت xkb_shortcuts_config_اندراج) {
                .ہدف = XKB_LAYOUT_INVALID,
                .mods = 0
            };
        }
    }

    sm->config.shortcuts = (ساخت machine_shortcuts_config) {
        .entries = entries
    };
    واپس XKB_SUCCESS;
}

ساخت xkb_machine *
xkb_machine_new(ثابت ساخت xkb_machine_builder * محدود builder,
                شمار xkb_حالت * محدود حالت_2)
{
    ساخت xkb_machine * ثابت machine = صفر_حافظہ_دو(1, حجم(*machine));
    اگر (!machine) {
        log_err_func1(builder->keymap->ctx, XKB_ERROR_تخصیص_FAILURE_2,
                      "cannot allocate machine\n");
        اگر (حالت_2)
            *حالت_2 = XKB_ERROR_تخصیص_FAILURE;
        واپس تہی;
    }

    xkb_server_state_init(&machine->بنیاد, builder->keymap, SERVER_STATE,
                          builder->controls.a11y.affect_flags,
                          builder->controls.a11y.flags);
    machine->flags = builder->machine_flags;
    اگر (!(machine->flags &
          XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_کلید_FIX))
    {
        machine->بنیاد.flags |= STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_کلید;
    }
    اگر (machine->flags & XKB_MACHINE_UNLOCK_MODS_ON_PRESS) {
        machine->بنیاد.flags |= STATE_UNLOCK_MODS_ON_PRESS;
    }

    شمار xkb_حالت حالت_3;
    اگر ((حالت_3 = machine_سیٹ_mods(machine, &builder->mods)) != XKB_SUCCESS ||
        (حالت_3 = machine_سیٹ_shortcuts(machine, &builder->shortcuts)) !=
        XKB_SUCCESS) {
        *حالت_2 = حالت_3;
        جاؤ error;
    }

    darray_init(machine->overlays.کلیدیں);
    machine->mouse.default_button = XKB_اشارہ_گر_BUTTON_MIN;

    اگر (حالت_2)
        *حالت_2 = XKB_SUCCESS;

    واپس machine;

error:
    xkb_machine_unref(machine);
    واپس تہی;
}

ساخت xkb_machine *
xkb_machine_ref(ساخت xkb_machine *sm)
{
    أكد(sm->بنیاد.بنیاد.refcnt > 0);
    sm->بنیاد.بنیاد.refcnt++;
    واپس sm;
}

خالی
xkb_machine_unref(ساخت xkb_machine *sm)
{
    أكد(!sm || sm->بنیاد.بنیاد.refcnt > 0);
    اگر (!sm || --sm->بنیاد.بنیاد.refcnt > 0)
        واپس;

    xkb_state_destroy(&sm->بنیاد.بنیاد);
    darray_free(sm->overlays.کلیدیں);
    آزاد(sm->config.shortcuts.entries);
    آزاد(sm->config.modifiers.mappings);
    آزاد(sm);
}

ساخت xkb_keymap *
xkb_machine_get_keymap(ثابت ساخت xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    واپس sm->بنیاد.بنیاد.keymap;
}

ساخت xkb_state *
xkb_machine_get_state(ساخت xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    واپس (ساخت xkb_state *)sm;
}

ساخت xkb_state *
xkb_state_new_from_machine(ثابت ساخت xkb_machine * محدود machine,
                           شمار xkb_حالت * محدود حالت_2)
{
    /* Creation */
    شمار xkb_حالت حالت_3;
    ساخت xkb_state * ثابت state = xkb_state_new_with_منوال(
        machine->بنیاد.بنیاد.keymap,
        XKB_STATE_منوال_SERVER_QUERY,
        &حالت_3
    );
    اگر (حالت_3 != XKB_SUCCESS) {
        اگر (حالت_2)
            *حالت_2 = حالت_3;
        واپس تہی;
    }

    أكد(state->منوال == SERVER_COMPANION);
    أكد(state->refcnt == 1);

    /* Copy machine’s state */
    *state = machine->بنیاد.بنیاد;

    /* Restore previous values */
    state->منوال = SERVER_COMPANION;
    state->refcnt = 1;

    اگر (حالت_2)
        *حالت_2 = XKB_SUCCESS;
    واپس state;
}

ساکن خالی
machine_update_overlays(ساخت xkb_machine *sm)
{
    /*
     * Overlays indices are stored 1-indexed in nibbles: the lowest
     * nibble corresponds to the latest activated index.
     */

    ثابت xkb_overlay_نقاب_t نقاب =
        overlays_from_controls(sm->بنیاد.بنیاد.components.controls);
    xkb_overlay_نقاب_t added = نقاب & ~sm->overlays.enabled;

    /* Remove overlays no longer enabled and keep relative order */
    uint32_t ترتیب = sm->overlays.ترتیب;
    ثابت xkb_overlay_اشاریہ_t overlay_max =
        format_max_overlays(sm->بنیاد.بنیاد.keymap->format);
    برائے (uint8_t n = 0; n < overlay_max;) {
        xkb_overlay_اشاریہ_t overlay_idx = (ترتیب >> (n * 4)) & 0xf;
        اگر (!overlay_idx)
            توڑو;
        ثابت xkb_overlay_نقاب_t overlay_نقاب =
            (xkb_overlay_نقاب_t)(1u << (overlay_idx - 1u) /* k is 1-indexed */);
        اگر (overlay_نقاب & نقاب) {
            /* no duplicates */
            added &= ~overlay_نقاب;
            n++;
        } ورنہ {
            /* remove overlay */
            ثابت uint32_t سر = (UINT32_C(1) << (n * 4)) - 1;
            ترتیب = (ترتیب & سر) | ((ترتیب >> 4) & ~سر);
        }
    }

    /* Add new overlays in ascending order */
    برائے (xkb_overlay_اشاریہ_t k = 0; added; k++, added >>= 1) {
        اگر (added & 0x1) {
            ترتیب <<= 4;
            ترتیب |= k + 1u; /* k is 0-indexed */
        }
    }

    sm->overlays.ترتیب = ترتیب;
    sm->overlays.enabled = نقاب;
}

شمار xkb_حالت
xkb_machine_process_synthetic(ساخت xkb_machine *sm,
                              ثابت ساخت xkb_synthetic_update *update,
                              ساخت xkb_events *events)
{
    /* Check ABI compatibility */
    شمار xkb_حالت حالت_2 =
        check_state_update_abi(sm->بنیاد.بنیاد.keymap->ctx, update);
    اگر (حالت_2 != XKB_SUCCESS)
        واپس حالت_2;

    ساخت xkb_server_state * ثابت state = &sm->بنیاد;
    ثابت ساخت state_components پچھلا_components_2 = state->بنیاد.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    darray_سائز(events->قطار) = 0;
    events->اگلا = 0;

    /* Update parametrized controls first */
    اگر (update->layout_policy) {
        حالت_2 = state_update_layout_policy(state, update->layout_policy);
        اگر (حالت_2 != XKB_SUCCESS)
            واپس حالت_2;
    }

    اگر (update->components) {
        ثابت ساخت xkb_state_components_update * ثابت components =
            update->components;
        /* Update boolean controls first */
        اگر (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, events);
        }

        state_update_latched_locked(state, components, events);
    }

    xkb_state_update_derived(&state->بنیاد);

    ثابت شمار xkb_state_component بدلا = get_state_component_changes(
        &پچھلا_components_2, &state->بنیاد.components
    );
    اگر (بدلا) {
        // TODO: latch controls
        اگر (بدلا & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        /* Create event only if some component actually changed */
        darray_append(events->قطار, (ساخت xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .بدلا = بدلا,
                .components = state->بنیاد.components
            }
        });

        /* Terminate the frame */
        darray_append(events->قطار, (ساخت xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_چوکھٹا,
        });
    }

    واپس XKB_SUCCESS;
}

ساکن ssize_t
do_remap_modifiers(ثابت ساخت machine_modifiers_config * محدود mappings,
                   ساخت xkb_state * محدود state,
                   ساخت xkb_events * محدود events,
                   ثابت ساخت xkb_کلید * محدود کلید)
{
    اگر (!(mappings->نقاب & state->components.mods))
        واپس -1;

    ثابت xkb_layout_اشاریہ_t layout = state_کلید_get_layout(state, کلید);
    اگر (layout >= کلید->عدد_groups)
        واپس -1;

    ثابت ساخت xkb_کلید_type * ثابت type = کلید->groups[layout].type;
    xkb_mod_نقاب_t affect_mods = 0;
    xkb_mod_نقاب_t mods = 0;
    برائے (darray_سائز_t m = 0; m < mappings->mappings_عدد; m++) {
        ثابت ساخت machine_mods_mapping * ثابت mapping =
            &mappings->mappings[m];
        اگر (/* Skip not matching active mods */
            (mapping->ماخذ & state->components.mods) == mapping->ماخذ &&
            /* Skip overlap with previous active mappings */
            !(mapping->ماخذ & affect_mods) &&
            /* Skip if the key type uses some of the source modifiers */
            !(mapping->ماخذ & type->mods.نقاب)) {
            affect_mods |= mappings->mappings[m].ماخذ;
            mods |= mappings->mappings[m].ہدف;
        }
    }

    اگر (!affect_mods)
        واپس -1;

    ساخت xkb_state new = *state;
    new.components.بنیاد_mods = (new.components.بنیاد_mods & ~affect_mods) | mods;
    new.components.latched_mods = (new.components.latched_mods & ~affect_mods);
    new.components.locked_mods = (new.components.locked_mods & ~affect_mods);
    xkb_state_update_derived(&new);

    ssize_t event_idx = -1;
    ثابت شمار xkb_state_component بدلا =
        get_state_component_changes(&state->components, &new.components);
    اگر (بدلا) {
        event_idx = (ssize_t) darray_سائز(events->قطار);
        darray_append(events->قطار, (ساخت xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = new.components,
                .بدلا = بدلا
            }
        });
    }

    state->components.mods = new.components.mods;
    واپس event_idx;
}

/**
 * Activate an alternative layout if some modifiers are active, typically
 * `Control`, `Alt` or `Super`.
 *
 * The tweak consists of computing a *base* layout such that the *effective*
 * layout is the corresponding target layout.
 */
ساکن ssize_t
do_shortcuts_tweak(ثابت ساخت machine_shortcuts_config *config,
                   ساخت xkb_state *state,
                   ثابت ساخت state_components *پچھلا_components_2,
                   ساخت xkb_events *events, ssize_t remap_event)
{
    اگر (!config->entries) {
        /* No shortcuts tweak */
        واپس remap_event;
    }

    ثابت ساخت xkb_shortcuts_config_اندراج * ثابت اندراج_2 =
        &config->entries[state->components.گروہ];

    اگر (اندراج_2->ہدف != XKB_LAYOUT_INVALID &&
        (اندراج_2->mods & state->components.mods)) {
        /*
         * Activate shortcuts tweak:
         * 1. The real base group is saved by the caller, to be restored by
         *    later by a call to undo_tweaks().
         * 2. Compute the new base group so that the resulting effective group
         *    is the target.
         * 3. Recompute derived state components.
         */
        ساخت xkb_state new = *state;
        اگر (remap_event < 0) {
            /* Create new event */
            remap_event = (ssize_t) darray_سائز(events->قطار);
            darray_append(events->قطار, (ساخت xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = {0},
                    .بدلا = 0
                }
            });
        } ورنہ {
            /* Merge with last remap event */
            new.components =
                darray_شے(events->قطار, remap_event).components.components;
        }
        new.components.بنیاد_گروہ
            = (int32_t) اندراج_2->ہدف
            - state->components.latched_گروہ
            - state->components.locked_گروہ;
        xkb_state_update_derived(&new);

        ثابت شمار xkb_state_component بدلا =
            get_state_component_changes(پچھلا_components_2, &new.components);
        darray_شے(events->قطار, remap_event).components.بدلا = بدلا;
        darray_شے(events->قطار, remap_event).components.components =
            new.components;

        state->components.گروہ = new.components.گروہ;
    } ورنہ {
        /* Deactivated entry or modifiers do not match */
    }
    واپس remap_event;
}

ساکن خالی
undo_tweaks(ثابت ساخت xkb_state *state,
            ثابت ساخت state_components *initial_components,
            ساخت xkb_events *events)
{
    /* Get last component event */
    ثابت ساخت xkb_event *event = تہی;
    darray_foreach_الٹ(event, events->قطار) {
        اگر (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS)
            توڑو;
    }
    اگر (!event)
        واپس;

    /* Restore state */
    ثابت شمار xkb_state_component بدلا =
        get_state_component_changes(initial_components,
                                    &event->components.components);
    اگر (بدلا) {
        darray_append(events->قطار, (ساخت xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = *initial_components,
                .بدلا = بدلا
            }
        });
    }
}

ساکن ثابت ساخت xkb_کلید *
process_overlayable_کلید(ساخت xkb_machine *sm,
                        ثابت ساخت xkb_کلید * کلید,
                        شمار xkb_کلید_direction direction)
{
    /* Check if key is currently overlaid */
    ساخت xkb_overlaid_کلید *اندراج_2 = تہی;
    ساخت xkb_overlaid_کلید *دستیاب_اندراج = تہی;
    darray_foreach(اندراج_2, sm->overlays.کلیدیں) {
        اگر (اندراج_2->old == کلید) {
            /* Maintain the key overlaid */
            انتخاب (direction) {
            حالت XKB_کلید_نیچے_کو:
                اندراج_2->refcnt++;
                توڑو;
            حالت XKB_کلید_REPEATED:
                توڑو;
            حالت XKB_کلید_اوپر:
            طےشدہ:
                اندراج_2->refcnt--;
                توڑو;
            }
            اگر (اندراج_2->refcnt <= 0)
                اندراج_2->old = تہی;
            واپس اندراج_2->new;
        } ورنہ اگر (!اندراج_2->old && !دستیاب_اندراج) {
            /* First free spot */
            دستیاب_اندراج = اندراج_2;
        }
    }
    اگر (direction == XKB_کلید_نیچے_کو) {
        /* Enable key overlay */

        /*
         * Get overlay index: select the latest activated overlay
         * containing the key.
         */
        ثابت ساخت xkb_کلید *new = کلید;

        اگر (کلید->overlays & sm->overlays.enabled) {
            /* Some relevant overlay is active */
            برائے (uint32_t ڈھیر = sm->overlays.ترتیب; ڈھیر; ڈھیر >>= 4) {
                static_assert(XKB_OVERLAY_گنتی == 8, "");
                ثابت xkb_overlay_اشاریہ_t overlay = (ڈھیر & 0xf) - 1;
                ثابت xkb_overlay_نقاب_t نقاب =
                    (xkb_overlay_نقاب_t)(1u << overlay);
                اگر (کلید->overlays & نقاب) {
                    /* Get new key */
                    اگر (کلید->overlays_inline) {
                        /* Stored inline */
                        new = کلید->overlay_کلید;
                    } ورنہ {
                        /* Stored in sparse array */
                        ثابت xkb_overlay_نقاب_t کم = (xkb_overlay_نقاب_t)(
                            کلید->overlays & (xkb_overlay_نقاب_t)(نقاب - 1u)
                        );
                        /* index = number of set bits strictly below bit */
                        ثابت xkb_overlay_اشاریہ_t اشاریہ =
                            (xkb_overlay_اشاریہ_t)popcount32(کم);
                        new = کلید->overlays_کلیدیں[اشاریہ];
                    }
                    توڑو;
                }
            }
        } ورنہ {
            /*
             * No relevant overlay is active: block future overlays while
             * the key is pressed.
             */
        }

        /* Get new entry */
        اگر (دستیاب_اندراج) {
            /* Re-use a free entry */
            اندراج_2 = دستیاب_اندراج;
        } ورنہ {
            /* Append a new entry */
            ثابت darray_سائز_t idx = darray_سائز(sm->overlays.کلیدیں);
            darray_resize(sm->overlays.کلیدیں, idx + 1);
            اندراج_2 = &darray_شے(sm->overlays.کلیدیں, idx);
        }

        /* Overlay the key */
        اندراج_2->old = کلید;
        اندراج_2->new = new;
        اندراج_2->refcnt = 1;
        واپس new;
    }

    /* No overlay */
    واپس کلید;
}

شمار xkb_حالت
xkb_machine_process_کلید(ساخت xkb_machine *sm,
                        xkb_keycode_t kc, شمار xkb_کلید_direction direction,
                        ساخت xkb_events *events)
{
    darray_سائز(events->قطار) = 0;
    events->اگلا = 0;

    ساخت xkb_server_state * ثابت state = &sm->بنیاد;
    ثابت ساخت xkb_کلید * کلید = Xkb_کلید(state->بنیاد.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    اگر (!کلید || (direction == XKB_کلید_REPEATED && !کلید->repeats))
        واپس XKB_SUCCESS;

    ثابت ساخت state_components initial_components = state->بنیاد.components;

    اگر (direction == XKB_کلید_نیچے_کو &&
        (sm->بنیاد.بنیاد.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیدیں) &&
        (sm->بنیاد.a11y_flags & XKB_A11_Y_STICKY_کلیدیں_NO_بیک_وقت_کلیدیں) &&
        state->بنیاد.components.بنیاد_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیدیں,
                                      0, events);
    }

    اگر (کلید->overlays)
        کلید = process_overlayable_کلید(sm, کلید, direction);

    ssize_t remap_event = do_remap_modifiers(&sm->config.modifiers,
                                             &state->بنیاد, events, کلید);

    remap_event = do_shortcuts_tweak(&sm->config.shortcuts, &state->بنیاد,
                                     &initial_components, events, remap_event);

    اگر (remap_event >= 0) {
        // TODO: what about merging state event for RedirectKey?
        darray_append(events->قطار, (ساخت xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_چوکھٹا,
        });
    }

    state->flags |= STATE_REQUIRE_کلید_EVENT;
    state->سیٹ_mods = 0;
    state->clear_mods = 0;

    /* Handle key actions */
    xkb_filter_apply_all(state, events, کلید, direction);

    xkb_mod_اشاریہ_t i;
    xkb_mod_نقاب_t ثنائی_ہندسہ;
    برائے (i = 0, ثنائی_ہندسہ = 1; state->سیٹ_mods; i++, ثنائی_ہندسہ <<= 1) {
        اگر (state->سیٹ_mods & ثنائی_ہندسہ) {
            state->mod_کلید_گنتی[i]++;
            state->بنیاد.components.بنیاد_mods |= ثنائی_ہندسہ;
            state->سیٹ_mods &= ~ثنائی_ہندسہ;
        }
    }

    برائے (i = 0, ثنائی_ہندسہ = 1; state->clear_mods; i++, ثنائی_ہندسہ <<= 1) {
        اگر (state->clear_mods & ثنائی_ہندسہ) {
            state->mod_کلید_گنتی[i]--;
            اگر (state->mod_کلید_گنتی[i] <= 0) {
                state->بنیاد.components.بنیاد_mods &= ~ثنائی_ہندسہ;
                state->mod_کلید_گنتی[i] = 0;
            }
            state->clear_mods &= ~ثنائی_ہندسہ;
        }
    }

    xkb_state_update_derived(&state->بنیاد);

    /*
     * Insert key event only if actions did not override it, e.g.:
     * - RedirectKey() append its own key event;
     * - MovePointer() and PointerButton() generate pointer events instead
     *   of key events;
     * - TerminateServer() generate a Server event instead of key events.
     */
    اگر (state->flags & STATE_REQUIRE_کلید_EVENT) {
        darray_append(events->قطار, (ساخت xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_کلید,
            .کلید = {
                .keycode = کلید->keycode,
                .direction = direction
            }
        });
    }

    اگر (remap_event >= 0) {
        // FIXME: fragile if last state change does not restore the state to the remap event
        undo_tweaks(&state->بنیاد, &initial_components, events);
    }

    ثابت شمار xkb_state_component بدلا = get_state_component_changes(
        &initial_components, &state->بنیاد.components
    );
    اگر (بدلا) {
        اگر (بدلا & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        // TODO: merge with previous state event, if any

        darray_append(events->قطار, (ساخت xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = state->بنیاد.components,
                .بدلا = بدلا
            }
        });
    }

    اگر (!darray_خالی_خانے(events->قطار) &&
        darray_شے(events->قطار, darray_سائز(events->قطار) - 1).type !=
        XKB_EVENT_TYPE_چوکھٹا)
    {
        darray_append(events->قطار, (ساخت xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_چوکھٹا,
        });
    }

    واپس XKB_SUCCESS;
}

ساخت xkb_events *
xkb_events_new(ساخت xkb_context * محدود context,
               ثابت ساخت xkb_events_config * محدود config,
               شمار xkb_حالت * محدود حالت_2)
{
    /* Handle default configuration */
    ساکن ثابت ساخت xkb_events_config default_config = {
        .سائز = حجم(default_config)
    };
    اگر (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ثابت شمار xkb_حالت abi_error = xkb_check_state_abi(config);
    اگر (abi_error) {
        xkb_log_abi_error(context, __func__, abi_error);
        اگر (حالت_2)
            *حالت_2 = abi_error;
        واپس تہی;
    }

    /* Sanitize input */
    ثابت uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_EVENTS_FLAGS_اقدار);
    اگر (invalid_flags) {
        log_err_func(context, XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS_,
                     "unrecognized events collection flags: 0x%"PRIx32"\n",
                     invalid_flags);
        اگر (حالت_2)
            *حالت_2 = XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS;
        واپس تہی;
    }

    ساخت xkb_events *events = صفر_حافظہ_دو(1, حجم(*events));
    اگر (!events) {
        log_err_func1(context, XKB_ERROR_تخصیص_FAILURE_2,
                      "cannot allocate state events collection\n");
        اگر (حالت_2)
            *حالت_2 = XKB_ERROR_تخصیص_FAILURE;
        واپس تہی;
    }

    /* Pre-allocate event queue */
    darray_init(events->قطار);
    ساکن ثابت darray_سائز_t قطار_initial_سائز = 16;
    darray_resize(events->قطار, قطار_initial_سائز);
    // TODO: proper safe darray_resize() with return value
    اگر (!darray_اشیاء(events->قطار)) {
        log_err_func1(context, XKB_ERROR_تخصیص_FAILURE_2,
                      "cannot allocate state events queue\n");
        xkb_events_destroy(events);
        اگر (حالت_2)
            *حالت_2 = XKB_ERROR_تخصیص_FAILURE;
        واپس تہی;
    }
    darray_سائز(events->قطار) = 0;

    events->ctx = xkb_context_ref(context);
    events->اگلا = 0;

    اگر (حالت_2)
        *حالت_2 = XKB_SUCCESS;
    واپس events;
}

خالی
xkb_events_destroy(ساخت xkb_events *events)
{
    اگر (!events)
        واپس;
    darray_free(events->قطار);
    xkb_context_unref(events->ctx);
    آزاد(events);
}

ثابت ساخت xkb_event *
xkb_events_اگلا(ساخت xkb_events *events)
{
    اگر (events->اگلا < darray_سائز(events->قطار)) {
        ثابت darray_سائز_t اشاریہ = events->اگلا++;
        واپس &darray_شے(events->قطار, اشاریہ);
    } ورنہ {
        واپس تہی;
    }
}

شمار xkb_event_type
xkb_event_get_type(ثابت ساخت xkb_event *event)
{
    واپس event->type;
}

شمار xkb_حالت
xkb_event_get_keycode(ثابت ساخت xkb_event *event,
                      xkb_keycode_t *keycode,
                      شمار xkb_کلید_direction *direction)
{
    اگر (event->type == XKB_EVENT_TYPE_کلید) {
        *keycode = event->کلید.keycode;
        *direction = event->کلید.direction;
        واپس XKB_SUCCESS;
    } ورنہ {
        واپس XKB_ERROR_EVENT_TYPE_MISMATCH;
    }
}

شمار xkb_حالت
xkb_event_get_components(ثابت ساخت xkb_event * محدود event,
                         ساخت xkb_event_components * محدود components)
{
    اگر (event->type != XKB_EVENT_TYPE_STATE_COMPONENTS)
        واپس XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ثابت شمار xkb_حالت حالت_2 = xkb_check_state_abi(components);
    اگر (حالت_2 != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, حالت_2);
        واپس حالت_2;
    }

    components->بدلا = (uint32_t)event->components.بدلا;
    components->depressed_mods = event->components.components.بنیاد_mods;
    components->latched_mods = event->components.components.latched_mods;
    components->locked_mods = event->components.components.locked_mods;
    components->mods = event->components.components.mods;
    components->depressed_layout = event->components.components.بنیاد_گروہ;
    components->latched_layout = event->components.components.latched_گروہ;
    components->locked_layout = event->components.components.locked_گروہ;
    components->layout = event->components.components.گروہ;
    components->leds = event->components.components.leds;
    components->controls = (uint32_t)event->components.components.controls;

    واپس XKB_SUCCESS;
}

شمار xkb_حالت
xkb_event_get_اشارہ_گر_motion(ثابت ساخت xkb_event * محدود event,
                             ساخت xkb_event_اشارہ_گر_motion * محدود motion)
{
    اگر (event->type != XKB_EVENT_TYPE_اشارہ_گر_MOTION)
        واپس XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ثابت شمار xkb_حالت حالت_2 = xkb_check_state_abi(motion);
    اگر (حالت_2 != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, حالت_2);
        واپس حالت_2;
    }

    motion->flags = event->اشارہ_گر_motion.flags;
    motion->x = event->اشارہ_گر_motion.x;
    motion->y = event->اشارہ_گر_motion.y;

    واپس XKB_SUCCESS;
}

شمار xkb_حالت
xkb_event_get_اشارہ_گر_button(ثابت ساخت xkb_event * محدود event,
                             ساخت xkb_event_اشارہ_گر_button * محدود button)
{
    اگر (event->type != XKB_EVENT_TYPE_اشارہ_گر_BUTTON)
        واپس XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ثابت شمار xkb_حالت حالت_2 = xkb_check_state_abi(button);
    اگر (حالت_2 != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, حالت_2);
        واپس حالت_2;
    }

    button->button = event->اشارہ_گر_button.button;
    button->state = event->اشارہ_گر_button.state;
    button->گنتی = event->اشارہ_گر_button.گنتی;

    واپس XKB_SUCCESS;
}

شمار xkb_حالت
xkb_event_get_virtual_console(ثابت ساخت xkb_event * محدود event,
                              int8_t * محدود اشاریہ_or_انحراف,
                              bool * محدود is_انحراف)
{
    اگر (event->type != XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
        واپس XKB_ERROR_EVENT_TYPE_MISMATCH;

    *اشاریہ_or_انحراف = event->virtual_console.اشاریہ_or_انحراف;
    *is_انحراف = event->virtual_console.is_انحراف;

    واپس XKB_SUCCESS;
}

شمار xkb_حالت
xkb_state_update_event(ساخت xkb_state * محدود بنیاد_state,
                       ثابت ساخت xkb_event * محدود event,
                       شمار xkb_state_component * محدود بدلا)
{
    /* Guard against server-only state */
    أكد(بنیاد_state->منوال == SERVER_COMPANION ||
           بنیاد_state->منوال == LEGACY_MIXED_STATE);
    اگر (بنیاد_state->منوال != SERVER_COMPANION &&
        بنیاد_state->منوال != LEGACY_MIXED_STATE) {
        log_err(بنیاد_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_منوال_2,
                "%s: Unexpected state type %d\n", __func__, بنیاد_state->منوال);
        واپس XKB_ERROR_UNEXPECTED_STATE_منوال;
    }

    ساخت xkb_client_state * ثابت state =
        (ساخت xkb_client_state *)بنیاد_state;

    اگر (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
        ثابت ساخت state_components پچھلا_components = state->بنیاد.components;
        state->بنیاد.components = event->components.components;
        /*
         * Recompute the changes instead of using the event value, because we do
         * not know if the event’s queue and the state are synced.
         */
        اگر (بدلا)
            *بدلا = get_state_component_changes(&پچھلا_components,
                                                   &state->بنیاد.components);
    } ورنہ {
        اگر (بدلا)
            *بدلا = 0;
    }
    واپس XKB_SUCCESS;
}

ساکن خالی
append_اشارہ_گر_button(ساخت xkb_events * محدود events,
                      ساخت xkb_filter * محدود filter,
                      شمار xkb_اشارہ_گر_button_state button_state,
                      uint8_t گنتی)
{
    darray_append(events->قطار, (ساخت xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_اشارہ_گر_BUTTON,
        .اشارہ_گر_button = {
            .سائز = حجم(((ساخت xkb_event*)0)->اشارہ_گر_button),
            .button = filter->priv,
            .state = button_state,
            .گنتی = گنتی,
        }
    });
}

ساکن خالی
xkb_filter_اشارہ_گر_button_new(ساخت xkb_server_state *state,
                              ساخت xkb_events *events,
                              ساخت xkb_filter *filter)
{
    اگر (!events || state->بنیاد.منوال != SERVER_STATE ||
        !(state->بنیاد.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_کلیدیں)) {
        filter->func = تہی;
        واپس;
    }

    ساخت xkb_machine * ثابت sm = (ساخت xkb_machine *)state;

    /* Resolve button */
    ثابت xkb_اشارہ_گر_button_اشاریہ_t button =
        (filter->action.btn.button == XKB_اشارہ_گر_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;
    filter->priv = button;

    ثابت شمار xkb_اشارہ_گر_button_state button_state =
        XKB_اشارہ_گر_BUTTON_PRESSED;
    append_اشارہ_گر_button(events, filter, button_state,
                          filter->action.btn.گنتی);
    state->flags &= ~STATE_REQUIRE_کلید_EVENT;
}

ساکن bool
xkb_filter_اشارہ_گر_button_func(ساخت xkb_server_state *state,
                               ساخت xkb_events *events,
                               ساخت xkb_filter *filter,
                               ثابت ساخت xkb_کلید *کلید,
                               شمار xkb_کلید_direction direction)
{
    اگر (کلید != filter->کلید || !events)
        واپس XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    انتخاب (direction) {
    حالت XKB_کلید_نیچے_کو:
        filter->refcnt++;
        /* fallthrough */
    حالت XKB_کلید_REPEATED:
        واپس XKB_FILTER_CONSUME;
    طےشدہ:
        اگر (--filter->refcnt > 0)
            واپس XKB_FILTER_CONSUME;
    }

    اگر (!filter->action.btn.گنتی)
        append_اشارہ_گر_button(events, filter, XKB_اشارہ_گر_BUTTON_RELEASED, 0);

    filter->func = تہی;
    واپس XKB_FILTER_CONSUME;
}

ساخت xkb_filter_اشارہ_گر_تالا_button_priv {
    uint32_t button:31;
#اگر_تعریف_شدہ _WIN32
    uint32_t locked:1; // MSVC requires matching types to pack bitfields
#ورنہ
    bool locked:1;
#اگر_ختم
};

شمار {
    XKB_FILTER_اشارہ_گر_تالا_BUTTON_PRIV_چوڑائی =
        حجم(ساخت xkb_filter_اشارہ_گر_تالا_button_priv) * CHAR_BIT,
    XKB_FILTER_اشارہ_گر_تالا_BUTTON_PRIV_MAX =
        (UINT64_C(1) << XKB_FILTER_اشارہ_گر_تالا_BUTTON_PRIV_چوڑائی) - 1
};

static_assert(حجم(ساخت xkb_filter_اشارہ_گر_تالا_button_priv) ==
              حجم(((ساخت xkb_filter*)0)->priv),
              "");
static_assert((uint32_t)XKB_اشارہ_گر_BUTTON_MAX < (uint32_t)XKB_FILTER_اشارہ_گر_تالا_BUTTON_PRIV_MAX,
              "xkb_filter_pointer_lock_button_priv cannot store button");

ساکن خالی
xkb_filter_اشارہ_گر_تالا_button_new(ساخت xkb_server_state *state,
                                   ساخت xkb_events *events,
                                   ساخت xkb_filter *filter)
{
    اگر (!events || state->بنیاد.منوال != SERVER_STATE ||
        !(state->بنیاد.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_کلیدیں)) {
        filter->func = تہی;
        واپس;
    }

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    ساخت xkb_machine * ثابت sm = (ساخت xkb_machine *)state;

    /* Resolve button */
    ثابت xkb_اشارہ_گر_button_اشاریہ_t button =
        (filter->action.btn.button == XKB_اشارہ_گر_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;

    static_assert(XKB_اشارہ_گر_BUTTON_نقاب_چوڑائی <= 8,
                  "invalid UINTx_C() macro");
    static_assert(XKB_اشارہ_گر_BUTTON_MAX <= XKB_اشارہ_گر_BUTTON_نقاب_چوڑائی,
                  "invalid left shift");
    ثابت xkb_اشارہ_گر_button_نقاب_t نقاب = (UINT8_C(1) << button);

    ثابت bool locked = (sm->mouse.locked_buttons & نقاب);

    اگر (!locked && !(filter->action.btn.flags & ACTION_تالا_NO_تالا)) {
        sm->mouse.locked_buttons |= نقاب;
        filter->priv = button;
        append_اشارہ_گر_button(events, filter, XKB_اشارہ_گر_BUTTON_PRESSED, 0);
    }

    ساخت xkb_filter_اشارہ_گر_تالا_button_priv *priv =
        (ساخت xkb_filter_اشارہ_گر_تالا_button_priv*)&filter->priv;
    priv->button = button,
    priv->locked = locked;
}

ساکن bool
xkb_filter_اشارہ_گر_تالا_button_func(ساخت xkb_server_state *state,
                                    ساخت xkb_events *events,
                                    ساخت xkb_filter *filter,
                                    ثابت ساخت xkb_کلید *کلید,
                                    شمار xkb_کلید_direction direction)
{
    اگر (کلید != filter->کلید || !events || state->بنیاد.منوال != SERVER_STATE)
        واپس XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    انتخاب (direction) {
    حالت XKB_کلید_نیچے_کو:
        filter->refcnt++;
        /* fallthrough */
    حالت XKB_کلید_REPEATED:
        واپس XKB_FILTER_CONSUME;
    طےشدہ:
        اگر (--filter->refcnt > 0)
            واپس XKB_FILTER_CONSUME;
    }

    ثابت ساخت xkb_filter_اشارہ_گر_تالا_button_priv *priv =
        (ساخت xkb_filter_اشارہ_گر_تالا_button_priv*)&filter->priv;

    اگر (priv->locked && !(filter->action.btn.flags & ACTION_تالا_NO_UNLOCK)) {
        ساخت xkb_machine * ثابت sm = (ساخت xkb_machine *)state;

        static_assert(XKB_اشارہ_گر_BUTTON_نقاب_چوڑائی <= 8,
                    "invalid UINTx_C() macro");
        static_assert(XKB_اشارہ_گر_BUTTON_MAX <= XKB_اشارہ_گر_BUTTON_نقاب_چوڑائی,
                    "invalid left shift");
        ثابت xkb_اشارہ_گر_button_نقاب_t نقاب = (UINT8_C(1) << priv->button);

        sm->mouse.locked_buttons &= ~نقاب;
        filter->priv = priv->button;
        append_اشارہ_گر_button(events, filter, XKB_اشارہ_گر_BUTTON_RELEASED, 0);
    }

    filter->func = تہی;
    واپس XKB_FILTER_CONSUME;
}

ساکن خالی
xkb_filter_اشارہ_گر_سیٹ_default_button_new(ساخت xkb_server_state *state,
                                          ساخت xkb_events *events,
                                          ساخت xkb_filter *filter)
{
    اگر (!events || state->بنیاد.منوال != SERVER_STATE ||
        !(state->بنیاد.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_کلیدیں)) {
        filter->func = تہی;
        واپس;
    }

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    ساخت xkb_machine * ثابت sm = (ساخت xkb_machine *)state;

    static_assert(2 * XKB_اشارہ_گر_BUTTON_MAX <= زیادہ_سے_زیادہ_عدد, "lossy conversion");
    صحیح button = (filter->action.dflt.flags & ACTION_ABSOLUTE_SWITCH)
        ? filter->action.dflt.قدر
        : sm->mouse.default_button + filter->action.dflt.قدر;

    /* Wrap into range */
    ثابت صحیح rem = button % XKB_اشارہ_گر_BUTTON_MAX;
    sm->mouse.default_button = (rem >= 0)
        ? (xkb_اشارہ_گر_button_اشاریہ_t)rem
        : (xkb_اشارہ_گر_button_اشاریہ_t)(rem + XKB_اشارہ_گر_BUTTON_MAX);
}

ساکن bool
xkb_filter_اشارہ_گر_سیٹ_default_button_func(ساخت xkb_server_state *state,
                                           ساخت xkb_events *events,
                                           ساخت xkb_filter *filter,
                                           ثابت ساخت xkb_کلید *کلید,
                                           شمار xkb_کلید_direction direction)
{
    اگر (کلید != filter->کلید || !events)
        واپس XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    انتخاب (direction) {
    حالت XKB_کلید_نیچے_کو:
        filter->refcnt++;
        /* fallthrough */
    حالت XKB_کلید_REPEATED:
        واپس XKB_FILTER_CONSUME;
    طےشدہ:
        اگر (--filter->refcnt > 0)
            واپس XKB_FILTER_CONSUME;
    }

    filter->func = تہی;
    واپس XKB_FILTER_CONTINUE;
}

ساکن خالی
xkb_filter_terminate_server_new(ساخت xkb_server_state *state,
                                 ساخت xkb_events *events,
                                 ساخت xkb_filter *filter)
{
    اگر (!events || state->بنیاد.منوال != SERVER_STATE ||
        !(((ساخت xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = تہی;
        واپس;
    }

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    darray_append(events->قطار, (ساخت xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    });
}

ساکن bool
xkb_filter_terminate_server_func(ساخت xkb_server_state *state,
                                 ساخت xkb_events *events,
                                 ساخت xkb_filter *filter,
                                 ثابت ساخت xkb_کلید *کلید,
                                 شمار xkb_کلید_direction direction)
{
    اگر (کلید != filter->کلید || !events)
        واپس XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    انتخاب (direction) {
    حالت XKB_کلید_نیچے_کو:
        filter->refcnt++;
        /* fallthrough */
    حالت XKB_کلید_REPEATED:
        واپس XKB_FILTER_CONSUME;
    طےشدہ:
        اگر (--filter->refcnt > 0)
            واپس XKB_FILTER_CONSUME;
    }

    filter->func = تہی;
    واپس XKB_FILTER_CONSUME;
}

ساکن خالی
xkb_filter_switch_virtual_console_new(ساخت xkb_server_state *state,
                                      ساخت xkb_events *events,
                                      ساخت xkb_filter *filter)
{
    اگر (!events || state->بنیاد.منوال != SERVER_STATE ||
        !(((ساخت xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = تہی;
        واپس;
    }

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    darray_append(events->قطار, (ساخت xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
        .virtual_console = {
            .اشاریہ_or_انحراف = filter->action.screen.screen,
            .is_انحراف = !(filter->action.screen.flags & ACTION_ABSOLUTE_SWITCH)
        }
    });
}

ساکن bool
xkb_filter_switch_virtual_console_func(ساخت xkb_server_state *state,
                                       ساخت xkb_events *events,
                                       ساخت xkb_filter *filter,
                                       ثابت ساخت xkb_کلید *کلید,
                                       شمار xkb_کلید_direction direction)
{
    اگر (کلید != filter->کلید || !events)
        واپس XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    انتخاب (direction) {
    حالت XKB_کلید_نیچے_کو:
        filter->refcnt++;
        /* fallthrough */
    حالت XKB_کلید_REPEATED:
        واپس XKB_FILTER_CONSUME;
    طےشدہ:
        اگر (--filter->refcnt > 0)
            واپس XKB_FILTER_CONSUME;
    }

    filter->func = تہی;
    واپس XKB_FILTER_CONSUME;
}
