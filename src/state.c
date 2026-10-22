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
    darray(بنية xkb_event) صف;
    /**
     * Read cursor for `xkb_events_next()`.
     *
     * Reset to 0 on each `process_*` call.
     */
    darray_اندازه_t بعدی;
};

بنية xkb_server_state;

بنية xkb_filter {
    اتحاد xkb_action action;
    ثابت بنية xkb_کلید *کلید;
    bool (*func)(بنية xkb_server_state *state,
                 بنية xkb_events *events,
                 بنية xkb_filter *filter,
                 ثابت بنية xkb_کلید *کلید,
                 تعداد xkb_کلید_direction direction);
    uint32_t priv;
    صحيح refcnt;
};

تعداد xkb_state_نما_داخلی {
    /** `xkb_state`: Client-only state (server API raises errors) */
    CLIENT_STATE = XKB_STATE_نما_CLIENT,
    /** `xkb_state`: Server query companion */
    SERVER_COMPANION = XKB_STATE_نما_SERVER_QUERY,
    /** `xkb_state`: Legacy unsafe client + server state */
    LEGACY_MIXED_STATE = 2,
    /** `xkb_state`: Legacy server-only state (client-only API raises errors) */
    LEGACY_SERVER_STATE = 3,
    /** `xkb_machine`: Server-only state */
    SERVER_STATE = 4,
    XKB_STATE_TYPE_رقم_ENTRIES = 5
};

static_assert(CLIENT_STATE == 0 &&
              CLIENT_STATE == SERVER_COMPANION - 1 &&
              SERVER_COMPANION == LEGACY_MIXED_STATE - 1 &&
              LEGACY_MIXED_STATE == LEGACY_SERVER_STATE - 1 &&
              LEGACY_SERVER_STATE == SERVER_STATE - 1 &&
              SERVER_STATE == XKB_STATE_TYPE_رقم_ENTRIES - 1 &&
              XKB_STATE_TYPE_رقم_ENTRIES == 5,
              "Order is used to guard client/server -only APIs: do not change");

تعداد { XKB_LAYOUT_شاخص_T_MIN_عرض = 6 };
static_assert(XKB_MAX_GROUPS < (1 << XKB_LAYOUT_شاخص_T_MIN_عرض),
              "Cannot encode out_of_range_group::redirect_group");

بنية بیرون_از_range_گروه {
    تعداد xkb_layout_بیرون_از_range_policy policy :
        (حجم(تعداد xkb_layout_بیرون_از_range_policy) * CHAR_BIT -
         XKB_LAYOUT_شاخص_T_MIN_عرض);
    xkb_layout_شاخص_t redirect_گروه : XKB_LAYOUT_شاخص_T_MIN_عرض;
};

تعداد { XKB_STATE_نما_داخلی_MIN_عرض = 4 /* 3 bits + sign */ };
static_assert(XKB_STATE_TYPE_رقم_ENTRIES <
              (1 << XKB_STATE_نما_داخلی_MIN_عرض),
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
    بنية بیرون_از_range_گروه بیرون_از_range_گروه;

    تعداد xkb_state_نما_داخلی نما : XKB_STATE_نما_داخلی_MIN_عرض;
    صحيح refcnt : (حجم(صحيح) * CHAR_BIT - XKB_STATE_نما_داخلی_MIN_عرض);
};

/**
 * Minimal client state
 *
 * Wrapper for strong typing
 */
بنية xkb_client_state {
    /** Inherits from `xkb_state` */
    بنية xkb_state پایه;
};

تعداد state_flags {
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
بنية xkb_server_state {
    /** Inherits from `xkb_state` */
    بنية xkb_state پایه;

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
    xkb_mod_ماسک_t مجموعه_mods;
    xkb_mod_ماسک_t clear_mods;

    /*
     * We mustn't clear a base modifier if there's another depressed key
     * which affects it, e.g. given this sequence
     * < Left Shift down, Right Shift down, Left Shift Up >
     * the modifier should still be set. This keeps the count.
     */
    int16_t mod_کلید_شمار[XKB_MAX_MODS];
};

ساكن ثابت بنية xkb_کلید_type_مدخل *
get_مدخل_for_mods(ثابت بنية xkb_کلید_type *type, xkb_mod_ماسک_t mods)
{
    لكل (darray_اندازه_t i = 0; i < type->رقم_entries; i++)
        إذا (مدخل_is_فعال(&type->entries[i]) &&
            type->entries[i].mods.ماسک == mods)
            أرجع &type->entries[i];
    أرجع عدم;
}

ساكن ثابت بنية xkb_کلید_type_مدخل *
get_مدخل_for_کلید_state(بنية xkb_state *state, ثابت بنية xkb_کلید *کلید,
                        xkb_layout_شاخص_t گروه)
{
    ثابت بنية xkb_کلید_type* ثابت type = کلید->groups[گروه].type;
    xkb_mod_ماسک_t فعال_mods = state->components.mods & type->mods.ماسک;
    أرجع get_مدخل_for_mods(type, فعال_mods);
}

ساكن مضمن xkb_level_شاخص_t
state_کلید_get_level(بنية xkb_state *state, ثابت بنية xkb_کلید *کلید,
                         xkb_layout_شاخص_t layout)
{
    إذا (layout >= کلید->رقم_groups)
        أرجع XKB_LEVEL_INVALID;

    /* If we don't find an explicit match the default is 0. */
    ثابت بنية xkb_کلید_type_مدخل* ثابت مدخل =
        get_مدخل_for_کلید_state(state, کلید, layout);

    أرجع (مدخل) ? مدخل->level : 0;
}

/**
 * Returns the level to use for the given key and state, or
 * XKB_LEVEL_INVALID.
 */
xkb_level_شاخص_t
xkb_state_کلید_get_level(بنية xkb_state *state, xkb_keycode_t kc,
                        xkb_layout_شاخص_t layout)
{
    ثابت بنية xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);

    أرجع (کلید)
        ? state_کلید_get_level(state, کلید, layout)
        : XKB_LEVEL_INVALID;
}

ساكن مضمن xkb_layout_شاخص_t
state_کلید_get_layout(بنية xkb_state *state, ثابت بنية xkb_کلید *کلید)
{
    أرجع xkb_keymap_کلید_effective_layout(کلید, state->components.گروه);
}

/**
 * Returns the layout to use for the given key and state, taking
 * wrapping/clamping/etc into account, or XKB_LAYOUT_INVALID.
 */
xkb_layout_شاخص_t
xkb_state_کلید_get_layout(بنية xkb_state *state, xkb_keycode_t kc)
{
    ثابت بنية xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);

    إذا (!کلید)
        أرجع XKB_LAYOUT_INVALID;

    أرجع state_کلید_get_layout(state, کلید);
}

/* Empty action used for empty levels */
ساكن ثابت اتحاد xkb_action dummy_action = { .type = ACTION_TYPE_NONE };

ساكن xkb_action_شمار_t
xkb_کلید_get_actions(بنية xkb_state *state, ثابت بنية xkb_کلید *کلید,
                    ثابت اتحاد xkb_action **actions)
{
    ثابت xkb_layout_شاخص_t layout = state_کلید_get_layout(state, کلید);
    ثابت xkb_level_شاخص_t level = state_کلید_get_level(state, کلید, layout);
    إذا (level == XKB_LEVEL_INVALID)
        اذهب err;

    ثابت xkb_action_شمار_t شمار =
        xkb_keymap_کلید_get_actions_by_level(state->keymap, کلید,
                                            layout, level, actions);
    إذا (!شمار)
        اذهب err;

    أرجع شمار;

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
        filter = &darray_قلم(state->filters, darray_اندازه(state->filters) -1);
    }

    filter->refcnt = 1;
    أرجع filter;
}

/***====================================================================***/

تعداد xkb_filter_نتیجه {
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
#تعریف_کن apply_گروه_delta(filter_, state_, component_)                   \
    إذا ((filter_)->action.گروه.flags & ACTION_ABSOLUTE_SWITCH)          \
        (state_)->components.component_ = (filter_)->action.گروه.گروه; \
    وإلا                                                                 \
        (state_)->components.component_ += (filter_)->action.گروه.گروه

ساكن فراغ
xkb_filter_گروه_مجموعه_new(بنية xkb_server_state *state,
                         بنية xkb_events *events,
                         بنية xkb_filter *filter)
{
    static_assert(حجم(state->پایه.components.پایه_گروه) ==
                  حجم(filter->priv),
                  "Max groups don't fit");
    filter->priv = (uint32_t)state->پایه.components.پایه_گروه;
    apply_گروه_delta(filter, &state->پایه, پایه_گروه);
}

ساكن bool
xkb_filter_گروه_مجموعه_func(بنية xkb_server_state *state,
                          بنية xkb_events *events,
                          بنية xkb_filter *filter,
                          ثابت بنية xkb_کلید *کلید,
                          تعداد xkb_کلید_direction direction)
{
    إذا (کلید != filter->کلید) {
        filter->action.گروه.flags &= ~ACTION_قفل_CLEAR;
        أرجع XKB_FILTER_CONTINUE;
    }

    بدل (direction) {
    حالة XKB_کلید_پایین:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلید_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    static_assert(حجم(state->پایه.components.پایه_گروه) == حجم(filter->priv),
                  "Max groups don't fit");
    state->پایه.components.پایه_گروه = (int32_t) filter->priv;

    إذا (filter->action.گروه.flags & ACTION_قفل_CLEAR)
        state->پایه.components.locked_گروه = 0;

    filter->func = عدم;
    أرجع XKB_FILTER_CONTINUE;
}

ساكن تعداد xkb_state_component
get_state_component_changes(ثابت بنية state_components *a,
                            ثابت بنية state_components *b)
{
    تعداد xkb_state_component ماسک = 0;

    إذا (a->گروه != b->گروه)
        ماسک |= XKB_STATE_LAYOUT_EFFECTIVE;
    إذا (a->پایه_گروه != b->پایه_گروه)
        ماسک |= XKB_STATE_LAYOUT_DEPRESSED;
    إذا (a->latched_گروه != b->latched_گروه)
        ماسک |= XKB_STATE_LAYOUT_LATCHED;
    إذا (a->locked_گروه != b->locked_گروه)
        ماسک |= XKB_STATE_LAYOUT_LOCKED;
    إذا (a->mods != b->mods)
        ماسک |= XKB_STATE_MODS_EFFECTIVE;
    إذا (a->پایه_mods != b->پایه_mods)
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
xkb_filter_گروه_قفل_new(بنية xkb_server_state *state,
                          بنية xkb_events *events,
                          بنية xkb_filter *filter)
{
    إذا (filter->action.گروه.flags & ACTION_قفل_ON_RELEASE) {
        /*
         * Lock on key release: do nothing on key press.
         *
         * This is a keymap format v2 extension.
         */
        أرجع;
    } وإلا {
        /* Lock on key press */
        apply_گروه_delta(filter, &state->پایه, locked_گروه);
    }
}

ساكن bool
xkb_filter_گروه_قفل_func(بنية xkb_server_state *state,
                           بنية xkb_events *events,
                           بنية xkb_filter *filter,
                           ثابت بنية xkb_کلید *کلید,
                           تعداد xkb_کلید_direction direction)
{
    إذا (کلید != filter->کلید) {
        إذا ((filter->action.گروه.flags & ACTION_قفل_ON_RELEASE) &&
            direction == XKB_کلید_پایین) {
            /*
             * Another key has been pressed after the locking key:
             * cancel group lock on release.
             *
             * This is a keymap v2 extension.
             */
            filter->action.گروه.flags &= ~ACTION_قفل_ON_RELEASE;
        }
        أرجع XKB_FILTER_CONTINUE;
    }

    بدل (direction) {
    حالة XKB_کلید_پایین:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلید_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    إذا (filter->action.گروه.flags & ACTION_قفل_ON_RELEASE) {
        /*
         * Lock on key release
         *
         * This is a keymap v2 extension.
         */
        apply_گروه_delta(filter, &state->پایه, locked_گروه);
    } وإلا {
        /* Lock on key press: do nothing on key release. */
    }
    filter->func = عدم;
    أرجع XKB_FILTER_CONTINUE;
}

ساكن bool
xkb_action_breaks_latch(ثابت اتحاد xkb_action *action,
                        تعداد xkb_action_controls controls,
                        تعداد xkb_داخلی_action_flags flag,
                        xkb_mod_ماسک_t ماسک)
{
    بدل (action->type) {
    حالة ACTION_TYPE_NONE:
    حالة ACTION_TYPE_VOID:
    حالة ACTION_TYPE_PTR_BUTTON:
    حالة ACTION_TYPE_PTR_قفل:
    حالة ACTION_TYPE_CTRL_مجموعه:
    حالة ACTION_TYPE_CTRL_قفل:
    حالة ACTION_TYPE_SWITCH_VT:
    حالة ACTION_TYPE_TERMINATE:
    حالة ACTION_TYPE_REDIRECT_کلید:
        أرجع true;
    حالة ACTION_TYPE_PTR_MOVE:
    حالة ACTION_TYPE_PTR_DEFAULT:
        أرجع !(controls & CONTROL_MOUSE_کلیدها);
    حالة ACTION_TYPE_داخلی:
        أرجع (action->داخلی.flags & flag) &&
               ((action->داخلی.clear_latched_mods & ماسک) == ماسک);
    افتراضي:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_داخلی == 20 &&
                      ACTION_TYPE_داخلی + 1 == ACTION_TYPE_رقم_ENTRIES,
                      "Missing action type");
        أرجع false;
    }
}

تعداد xkb_کلید_latch_state {
    NO_LATCH = 0,
    LATCH_کلید_پایین,
    LATCH_PENDING,
    کلید_LATCH_STATE_رقم_ENTRIES
};

#تعریف_کن MAX_XKB_کلید_LATCH_STATE_LOG2 2
#إذا (کلید_LATCH_STATE_رقم_ENTRIES > (1 << MAX_XKB_کلید_LATCH_STATE_LOG2)) || \
    (-XKB_MAX_GROUPS) < (INT32_MIN >> MAX_XKB_کلید_LATCH_STATE_LOG2) || \
      XKB_MAX_GROUPS > (INT32_MAX >> MAX_XKB_کلید_LATCH_STATE_LOG2)
#خطا "Cannot represent priv field of the group latch filter"
#پایان_اگر

/* Hold the latch state *and* the group delta */
اتحاد گروه_latch_priv {
    uint32_t priv;
    بنية {
        /* The type is really: enum xkb_key_latch_state, but it is problematic
         * on Windows, because it is interpreted as signed and leads to wrong
         * negative values. */
        بیعلامت صحيح latch:MAX_XKB_کلید_LATCH_STATE_LOG2;
        int32_t گروه_delta:(32 - MAX_XKB_کلید_LATCH_STATE_LOG2);
    };
};

ساكن فراغ
xkb_filter_گروه_latch_new(بنية xkb_server_state *state,
                           بنية xkb_events *events,
                           بنية xkb_filter *filter)
{
    ثابت اتحاد گروه_latch_priv priv = {
        .latch = LATCH_کلید_پایین,
        .گروه_delta = (filter->action.گروه.flags & ACTION_ABSOLUTE_SWITCH)
            ? filter->action.گروه.گروه - state->پایه.components.پایه_گروه
            : filter->action.گروه.گروه
    };
    filter->priv = priv.priv;
    /* Like group set */
    apply_گروه_delta(filter, &state->پایه, پایه_گروه);
}

ساكن bool
xkb_filter_گروه_latch_func(بنية xkb_server_state *state,
                            بنية xkb_events *events,
                            بنية xkb_filter *filter,
                            ثابت بنية xkb_کلید *کلید,
                            تعداد xkb_کلید_direction direction)
{
    اتحاد گروه_latch_priv priv = {.priv = filter->priv};
    تعداد xkb_کلید_latch_state latch = priv.latch;

    إذا (direction == XKB_کلید_پایین) {
        ثابت اتحاد xkb_action *actions = عدم;
        ثابت xkb_action_شمار_t شمار = xkb_کلید_get_actions(&state->پایه, کلید,
                                                             &actions);

        إذا (latch == LATCH_کلید_پایین) {
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
            إذا (state->a11y_flags & XKB_A11_Y_LATCH_همزمان_کلیدها) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                لكل (xkb_action_شمار_t k = 0; k < شمار; k++) {
                    إذا (xkb_action_breaks_latch(&(actions[k]),
                                                state->پایه.components.controls,
                                                داخلی_BREAKS_گروه_LATCH,
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
            ثابت bool sticky_کلیدها = state->پایه.components.controls
                                   & CONTROL_STICKY_کلیدها;
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
            لكل (xkb_action_شمار_t k = 0; k < شمار; k++) {
                إذا (((actions[k].type == ACTION_TYPE_گروه_LATCH) ||
                     (actions[k].type == ACTION_TYPE_گروه_مجموعه && sticky_کلیدها)) &&
                     actions[k].گروه.گروه == filter->action.گروه.گروه &&
                     (actions[k].گروه.flags & ACTION_ABSOLUTE_SWITCH) ==
                     (filter->action.گروه.flags & ACTION_ABSOLUTE_SWITCH)) {
                    إذا (filter->action.گروه.flags & ACTION_LATCH_مقصد_قفل &&
                        filter->action.گروه.گروه != 0)
                    {
                        /* Promote to lock */
                        filter->action.type = ACTION_TYPE_گروه_قفل;
                        filter->func = xkb_filter_گروه_قفل_func;
                        xkb_filter_گروه_قفل_new(state, events, filter);
                        state->پایه.components.latched_گروه -= priv.گروه_delta;
                        filter->کلید = کلید;
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
                                                 state->پایه.components.controls,
                                                 داخلی_BREAKS_گروه_LATCH,
                                                 0))
                {
                    /* Breaks the latch */
                    state->پایه.components.latched_گروه -= priv.گروه_delta;
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
    وإلا إذا (کلید != filter->کلید) {
        /* Ignore repeat/release of other keys */
    }
    وإلا إذا (direction == XKB_کلید_REPEATED) {
        أرجع XKB_FILTER_CONSUME; // TODO: check
    }
    وإلا {
        /* Our key got released.  If we've set it to clear locks, and we
         * currently have a group locked, then release it and
         * don't actually latch.  Else we've actually hit the latching
         * stage, so set PENDING and move our group from base to
         * latched. */
        إذا ((filter->action.گروه.flags & ACTION_قفل_CLEAR) &&
             state->پایه.components.locked_گروه) {
            إذا (latch == LATCH_PENDING)
                state->پایه.components.latched_گروه -= priv.گروه_delta;
            وإلا
                state->پایه.components.پایه_گروه -= priv.گروه_delta;
            state->پایه.components.locked_گروه = 0;
            filter->func = عدم;
        }
        /* Broken latch */
        وإلا إذا (latch == NO_LATCH) {
            state->پایه.components.پایه_گروه -= priv.گروه_delta;
            filter->func = عدم;
        }
        /* We may already have reached the latch state if pressing the
         * key multiple times without latch-to-lock enabled. */
        وإلا إذا (latch == LATCH_کلید_پایین) {
            latch = LATCH_PENDING;
            /* Switch from set to latch */
            state->پایه.components.پایه_گروه -= priv.گروه_delta;
            state->پایه.components.latched_گروه += priv.گروه_delta;
            /* XXX beep beep! */
        }
    }

    priv.latch = latch;
    filter->priv = priv.priv;

    أرجع XKB_FILTER_CONTINUE;
}

ساكن فراغ
xkb_filter_mod_مجموعه_new(بنية xkb_server_state *state,
                       بنية xkb_events *events,
                       بنية xkb_filter *filter)
{
    ثابت تعداد xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_قفل_CLEAR;
    إذا ((filter->action.mods.flags & unlock) == unlock) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension.
         */
        filter->priv = filter->action.mods.mods.ماسک
                     & ~state->پایه.components.locked_mods;
        state->پایه.components.locked_mods &= ~filter->action.mods.mods.ماسک;
    } وإلا {
        filter->priv = filter->action.mods.mods.ماسک;
    }

    state->مجموعه_mods |= (xkb_mod_ماسک_t) filter->priv;
}

ساكن bool
xkb_filter_mod_مجموعه_func(بنية xkb_server_state *state,
                        بنية xkb_events *events,
                        بنية xkb_filter *filter,
                        ثابت بنية xkb_کلید *کلید,
                        تعداد xkb_کلید_direction direction)
{

    إذا (کلید != filter->کلید) {
        filter->action.mods.flags &= ~ACTION_قفل_CLEAR;
        أرجع XKB_FILTER_CONTINUE;
    }

    بدل (direction) {
    حالة XKB_کلید_پایین:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلید_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    state->clear_mods |= (xkb_mod_ماسک_t) filter->priv;
    ثابت تعداد xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_قفل_CLEAR;
    إذا ((filter->action.mods.flags & unlock) == ACTION_قفل_CLEAR)
        state->پایه.components.locked_mods &= ~filter->action.mods.mods.ماسک;

    filter->func = عدم;
    أرجع XKB_FILTER_CONTINUE;
}

ساكن فراغ
xkb_filter_mod_قفل_new(بنية xkb_server_state *state,
                        بنية xkb_events *events,
                        بنية xkb_filter *filter)
{
    filter->priv = (state->پایه.components.locked_mods &
                    filter->action.mods.mods.ماسک);

    إذا (filter->priv && (filter->action.mods.flags & ACTION_UNLOCK_ON_PRESS)) {
        /*
         * Some of the target modifiers were locked before key press: unlock.
         *
         * This is a keymap v2 extension: unlock-on-press.
         */
        إذا (!(filter->action.mods.flags & ACTION_قفل_NO_UNLOCK))
            state->پایه.components.locked_mods &= ~filter->priv;
        /* No further action: cancel filter */
        filter->func = عدم;
    } وإلا {
        /* Set base mods; lock mods if relevant (XKB 1.0 spec) */
        state->مجموعه_mods |= filter->action.mods.mods.ماسک;
        إذا (!(filter->action.mods.flags & ACTION_قفل_NO_قفل))
            state->پایه.components.locked_mods |= filter->action.mods.mods.ماسک;
    }
}

ساكن bool
xkb_filter_mod_قفل_func(بنية xkb_server_state *state,
                         بنية xkb_events *events,
                         بنية xkb_filter *filter,
                         ثابت بنية xkb_کلید *کلید,
                         تعداد xkb_کلید_direction direction)
{
    إذا (کلید != filter->کلید)
        أرجع XKB_FILTER_CONTINUE;

    بدل (direction) {
    حالة XKB_کلید_پایین:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلید_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    state->clear_mods |= filter->action.mods.mods.ماسک;
    إذا (!(filter->action.mods.flags & ACTION_قفل_NO_UNLOCK))
        state->پایه.components.locked_mods &= ~filter->priv;

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

    إذا ((filter->action.mods.flags & ACTION_قفل_CLEAR) &&
        (filter->action.mods.flags & unlockOnPress) &&
        (state->پایه.components.locked_mods & filter->action.mods.mods.ماسک) ==
         filter->action.mods.mods.ماسک) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension: clear locks and do not latch.
         */
        state->پایه.components.locked_mods &= ~filter->action.mods.mods.ماسک;
        filter->func = عدم;
    } وإلا إذا (filter->action.mods.flags & ACTION_LATCH_ON_PRESS) {
        /*
         * Latch on key press
         *
         * This is a keymap format v2 extension.
         */
        filter->priv = LATCH_PENDING;
        state->پایه.components.latched_mods |= filter->action.mods.mods.ماسک;
        /* XXX beep beep! */
    } وإلا {
        /* XKB standard latch action */
        filter->priv = LATCH_کلید_پایین;
        state->مجموعه_mods |= filter->action.mods.mods.ماسک;
    }
}

ساكن bool
xkb_filter_mod_latch_func(بنية xkb_server_state *state,
                          بنية xkb_events *events,
                          بنية xkb_filter *filter,
                          ثابت بنية xkb_کلید *کلید,
                          تعداد xkb_کلید_direction direction)
{
    تعداد xkb_کلید_latch_state latch = filter->priv;

    إذا (direction == XKB_کلید_پایین) {
        ثابت اتحاد xkb_action *actions = عدم;
        ثابت xkb_action_شمار_t شمار = xkb_کلید_get_actions(&state->پایه, کلید,
                                                             &actions);

        إذا (latch == LATCH_کلید_پایین) {
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
            إذا (state->a11y_flags & XKB_A11_Y_LATCH_همزمان_کلیدها) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                لكل (xkb_action_شمار_t k = 0; k < شمار; k++) {
                    إذا (xkb_action_breaks_latch(&(actions[k]),
                                                state->پایه.components.controls,
                                                داخلی_BREAKS_MOD_LATCH,
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
            ثابت bool sticky_کلیدها = state->پایه.components.controls
                                   & CONTROL_STICKY_کلیدها;
            /*
             * NOTE: Action’s flags are not compared:
             * - flags may have been modified by keyboard controls at the
             *   creation of the filter;
             * - it is unlikely to have two LatchMods() actions on the same
             *   key that differ only by their flags;
             * - even if it were the case, one would most likely want both
             *   actions to be considered compatible in this context.
             */
            لكل (xkb_action_شمار_t k = 0; k < شمار; k++) {
                إذا (((actions[k].type == ACTION_TYPE_MOD_LATCH) ||
                     (actions[k].type == ACTION_TYPE_MOD_مجموعه && sticky_کلیدها)) &&
                    actions[k].mods.mods.ماسک == filter->action.mods.mods.ماسک)
                {
                    إذا (filter->action.mods.flags & ACTION_LATCH_مقصد_قفل) {
                        /* Mutate the action to LockMods() */
                        filter->action.type = ACTION_TYPE_MOD_قفل;
                        filter->func = xkb_filter_mod_قفل_func;
                        xkb_filter_mod_قفل_new(state, events, filter);
                    }
                    وإلا {
                        /* Mutate the action to SetMods() */
                        filter->action.type = ACTION_TYPE_MOD_مجموعه;
                        filter->func = xkb_filter_mod_مجموعه_func;
                        xkb_filter_mod_مجموعه_new(state, events, filter);
                    }
                    filter->کلید = کلید;
                    /* Clear latches */
                    state->پایه.components.latched_mods &=
                        ~filter->action.mods.mods.ماسک;
                    /* XXX beep beep! */
                    أرجع XKB_FILTER_CONSUME;
                }
                وإلا إذا (xkb_action_breaks_latch(&(actions[k]),
                                                 state->پایه.components.controls,
                                                 داخلی_BREAKS_MOD_LATCH,
                                                 filter->action.mods.mods.ماسک))
                {
                    /* XXX: This may be totally broken, we might need to break the
                     *      latch in the next run after this press? */
                    state->پایه.components.latched_mods &=
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
    وإلا إذا (کلید != filter->کلید) {
        /* Ignore repeat/release of other keys */
    }
    وإلا إذا (direction == XKB_کلید_REPEATED) {
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

        إذا ((filter->action.mods.flags & ACTION_قفل_CLEAR) &&
            !(filter->action.mods.flags & unlockOnPress) &&
            (state->پایه.components.locked_mods &
             filter->action.mods.mods.ماسک) == filter->action.mods.mods.ماسک)
        {
            /* XXX: We might be a bit overenthusiastic about clearing
             *      mods other filters have set here? */
            إذا (latch == LATCH_PENDING)
                state->پایه.components.latched_mods &=
                    ~filter->action.mods.mods.ماسک;
            وإلا
                state->clear_mods |= filter->action.mods.mods.ماسک;
            state->پایه.components.locked_mods &= ~filter->action.mods.mods.ماسک;
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
            state->پایه.components.latched_mods |= filter->action.mods.mods.ماسک;
            /* XXX beep beep! */
        }
    }

    filter->priv = latch;

    أرجع XKB_FILTER_CONTINUE;
}

ساكن فراغ
append_اشاره_گر_move(بنية xkb_events *events,
                    بنية xkb_filter *filter)
{
    تعداد xkb_اشاره_گر_motion_flags flags = 0;
    إذا (filter->action.ptr.flags & ACTION_ABSOLUTE_X)
        flags |= XKB_اشاره_گر_MOTION_ABSOLUTE_X;
    إذا (filter->action.ptr.flags & ACTION_ABSOLUTE_Y)
        flags |= XKB_اشاره_گر_MOTION_ABSOLUTE_Y;
    إذا (filter->action.ptr.flags & ACTION_REPEAT)
        flags |= XKB_اشاره_گر_MOTION_REPEATS;
    darray_append(events->صف, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_اشاره_گر_MOTION,
        .اشاره_گر_motion = {
            .اندازه = حجم(((بنية xkb_event*)0)->اشاره_گر_motion),
            .flags = flags,
            .x = filter->action.ptr.x,
            .y = filter->action.ptr.y,
        }
    });
}

ساكن فراغ
xkb_filter_اشاره_گر_move_new(بنية xkb_server_state *state,
                            بنية xkb_events *events,
                            بنية xkb_filter *filter)
{
    إذا (!events) {
        filter->func = عدم;
        أرجع;
    }

    أكد(state->پایه.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_کلیدها);
    append_اشاره_گر_move(events, filter);
    state->flags &= ~STATE_REQUIRE_کلید_EVENT;
}

ساكن bool
xkb_filter_اشاره_گر_move_func(بنية xkb_server_state *state,
                             بنية xkb_events *events,
                             بنية xkb_filter *filter,
                             ثابت بنية xkb_کلید *کلید,
                             تعداد xkb_کلید_direction direction)
{
    إذا (کلید != filter->کلید || !events)
        أرجع XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    بدل (direction) {
    حالة XKB_کلید_پایین:
        filter->refcnt++;
        append_اشاره_گر_move(events, filter);
        أرجع XKB_FILTER_CONSUME;
    حالة XKB_کلید_REPEATED:
        إذا (filter->action.ptr.flags & ACTION_REPEAT)
            append_اشاره_گر_move(events, filter);
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
xkb_filter_اشاره_گر_button_new(بنية xkb_server_state *state,
                              بنية xkb_events *events,
                              بنية xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن bool
xkb_filter_اشاره_گر_button_func(بنية xkb_server_state *state,
                               بنية xkb_events *events,
                               بنية xkb_filter *filter,
                               ثابت بنية xkb_کلید *کلید,
                               تعداد xkb_کلید_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن فراغ
xkb_filter_اشاره_گر_قفل_button_new(بنية xkb_server_state *state,
                                   بنية xkb_events *events,
                                   بنية xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن bool
xkb_filter_اشاره_گر_قفل_button_func(بنية xkb_server_state *state,
                                    بنية xkb_events *events,
                                    بنية xkb_filter *filter,
                                    ثابت بنية xkb_کلید *کلید,
                                    تعداد xkb_کلید_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن فراغ
xkb_filter_اشاره_گر_مجموعه_default_button_new(بنية xkb_server_state *state,
                                          بنية xkb_events *events,
                                          بنية xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن bool
xkb_filter_اشاره_گر_مجموعه_default_button_func(بنية xkb_server_state *state,
                                           بنية xkb_events *events,
                                           بنية xkb_filter *filter,
                                           ثابت بنية xkb_کلید *کلید,
                                           تعداد xkb_کلید_direction direction);

ساكن فراغ
xkb_filter_terminate_server_new(بنية xkb_server_state *state,
                                 بنية xkb_events *events,
                                 بنية xkb_filter *filter);

ساكن bool
xkb_filter_terminate_server_func(بنية xkb_server_state *state,
                                  بنية xkb_events *events,
                                  بنية xkb_filter *filter,
                                  ثابت بنية xkb_کلید *کلید,
                                  تعداد xkb_کلید_direction direction);

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
                                       ثابت بنية xkb_کلید *کلید,
                                       تعداد xkb_کلید_direction direction);

ساكن مضمن فراغ
clear_all_latches_and_locks(بنية xkb_server_state *state,
                            بنية xkb_events *events);

ساكن فراغ
xkb_filter_ctrls_new(بنية xkb_server_state *state,
                     بنية xkb_events *events,
                     بنية xkb_filter *filter)
{
    إذا (filter->action.type == ACTION_TYPE_CTRL_مجموعه) {
        /* Set: save the specified controls that are *not* already enabled */
        filter->priv = (uint32_t) (
            (~state->پایه.components.controls) & filter->action.ctrls.ctrls
        );
    } وإلا {
        /* Lock: save the specified controls that *are* already enabled */
        filter->priv = (uint32_t) (
            state->پایه.components.controls & filter->action.ctrls.ctrls
        );
    }

    إذا (filter->action.type == ACTION_TYPE_CTRL_مجموعه ||
        !(filter->action.ctrls.flags & ACTION_قفل_NO_قفل)) {
        /* Enable the specified controls that are not already enabled */
        state->پایه.components.controls |= filter->action.ctrls.ctrls;
    }
}

ساكن bool
xkb_filter_ctrls_func(بنية xkb_server_state *state,
                      بنية xkb_events *events,
                      بنية xkb_filter *filter,
                      ثابت بنية xkb_کلید *کلید,
                      تعداد xkb_کلید_direction direction)
{
    إذا (کلید != filter->کلید)
        أرجع XKB_FILTER_CONTINUE;

    بدل (direction) {
    حالة XKB_کلید_پایین:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلید_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    إذا (filter->action.type == ACTION_TYPE_CTRL_مجموعه ||
        !(filter->action.ctrls.flags & ACTION_قفل_NO_UNLOCK)) {
        ثابت تعداد xkb_action_controls old = state->پایه.components.controls;

        /*
         * Set: Disable specified controls that were *not* enabled at key press.
         * Lock: Disable specified controls that *were* enabled at key press.
         */
        state->پایه.components.controls &= ~(تعداد xkb_action_controls) filter->priv;

        إذا ((old & CONTROL_STICKY_کلیدها) &&
            !(state->پایه.components.controls & CONTROL_STICKY_کلیدها)) {
            /* Sticky keys were disabled: clear all locks and latches */
            clear_all_latches_and_locks(state, events);
        }
    }

    filter->func = عدم;
    أرجع XKB_FILTER_CONTINUE;
}

ساكن bool
append_redirect_کلید_events(بنية xkb_server_state *state,
                           بنية xkb_events *events,
                           ثابت بنية xkb_redirect_کلید_action *redirect,
                           تعداد xkb_کلید_direction direction)
{
    تعداد xkb_state_component تغییریافته = 0;
    ثابت xkb_mod_ماسک_t ماسک = redirect->affect_mods;

    /*
     * Reference state: find the last state update in the queue, otherwise
     * use the current state.
     */
    بنية xkb_event *event;
    بنية state_components آخرین_components = state->پایه.components;
    darray_foreach_معکوس(event, events->صف) {
        إذا (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
            آخرین_components = event->components.components;
            اكسر;
        }
    }

    إذا (ماسک) {
        بنية state_components new = آخرین_components;
        new.پایه_mods = (new.پایه_mods & ~ماسک) | redirect->mods;
        new.latched_mods = (new.latched_mods & ~ماسک) | redirect->mods;
        new.locked_mods = (new.locked_mods & ~ماسک) | redirect->mods;
        new.mods = (new.mods & ~ماسک) | redirect->mods;

        تغییریافته = get_state_component_changes(&آخرین_components, &new);
        إذا (تغییریافته) {
            darray_append(events->صف, (بنية xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = new,
                    .تغییریافته = تغییریافته
                }
            });
            /*
             * Commit the temporary modifiers change before the key event
             * it was computed for: consumers must observe the key event
             * under the *redirected* modifiers.
             */
            darray_append(events->صف, (بنية xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_قاب,
            });
        }
    }

    darray_append(events->صف, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_کلید,
        .کلید = {
            .keycode = redirect->keycode,
            .direction = direction
        }
    });

    إذا (ماسک && تغییریافته) {
        /* Restore state */
        darray_append(events->صف, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = آخرین_components,
                .تغییریافته = تغییریافته
            }
        });
    }

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;
    أرجع true;
}

ساكن فراغ
xkb_filter_redirect_کلید_new(بنية xkb_server_state *state,
                            بنية xkb_events *events,
                            بنية xkb_filter *filter)
{
    /* Action effectual only with the xkb_machine API and a valid keycode */
    إذا (!events || filter->action.redirect.keycode == XKB_KEYCODE_INVALID) {
        filter->func = عدم;
        أرجع;
    }
    append_redirect_کلید_events(state, events, &filter->action.redirect,
                               XKB_کلید_پایین);
}

ساكن bool
xkb_filter_redirect_کلید_func(بنية xkb_server_state *state,
                             بنية xkb_events *events,
                             بنية xkb_filter *filter,
                             ثابت بنية xkb_کلید *کلید,
                             تعداد xkb_کلید_direction direction)
{
    إذا (کلید != filter->کلید)
        أرجع XKB_FILTER_CONTINUE;

    إذا (direction == XKB_کلید_به_بالا) {
        append_redirect_کلید_events(state, events,
                                   &filter->action.redirect, XKB_کلید_به_بالا);
        filter->func = عدم;
        أرجع XKB_FILTER_CONSUME;
     } وإلا إذا (direction == XKB_کلید_پایین) {
        ثابت اتحاد xkb_action *actions = عدم;
        ثابت xkb_action_شمار_t شمار =
            xkb_کلید_get_actions(&state->پایه, کلید, &actions);

        لكل (xkb_action_شمار_t a = 0; a < شمار; a++) {
            إذا (actions[a].type == ACTION_TYPE_REDIRECT_کلید &&
                actions[a].redirect.keycode != filter->action.redirect.keycode) {
                    /* One action redirects to a different key: release first */
                    append_redirect_کلید_events(state, events,
                                               &filter->action.redirect,
                                               XKB_کلید_به_بالا);
                filter->func = عدم;
                أرجع XKB_FILTER_CONTINUE;
            }
        }
    }

    append_redirect_کلید_events(state, events, &filter->action.redirect,
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
                 ثابت بنية xkb_کلید *کلید, تعداد xkb_کلید_direction direction);
} filter_action_funcs[ACTION_TYPE_رقم_ENTRIES] = {
    [ACTION_TYPE_MOD_مجموعه]     = { xkb_filter_mod_مجموعه_new,
                                  xkb_filter_mod_مجموعه_func },
    [ACTION_TYPE_MOD_LATCH]   = { xkb_filter_mod_latch_new,
                                  xkb_filter_mod_latch_func },
    [ACTION_TYPE_MOD_قفل]    = { xkb_filter_mod_قفل_new,
                                  xkb_filter_mod_قفل_func },
    [ACTION_TYPE_گروه_مجموعه]   = { xkb_filter_گروه_مجموعه_new,
                                  xkb_filter_گروه_مجموعه_func },
    [ACTION_TYPE_گروه_LATCH] = { xkb_filter_گروه_latch_new,
                                  xkb_filter_گروه_latch_func },
    [ACTION_TYPE_گروه_قفل]  = { xkb_filter_گروه_قفل_new,
                                  xkb_filter_گروه_قفل_func },
    [ACTION_TYPE_PTR_MOVE]    = { xkb_filter_اشاره_گر_move_new,
                                  xkb_filter_اشاره_گر_move_func },
    [ACTION_TYPE_PTR_BUTTON]  = { xkb_filter_اشاره_گر_button_new,
                                  xkb_filter_اشاره_گر_button_func },
    [ACTION_TYPE_PTR_قفل]    = { xkb_filter_اشاره_گر_قفل_button_new,
                                  xkb_filter_اشاره_گر_قفل_button_func },
    [ACTION_TYPE_PTR_DEFAULT] = { xkb_filter_اشاره_گر_مجموعه_default_button_new,
                                  xkb_filter_اشاره_گر_مجموعه_default_button_func },
    [ACTION_TYPE_TERMINATE]   = { xkb_filter_terminate_server_new,
                                  xkb_filter_terminate_server_func },
    [ACTION_TYPE_SWITCH_VT]   = { xkb_filter_switch_virtual_console_new,
                                  xkb_filter_switch_virtual_console_func },
    [ACTION_TYPE_CTRL_مجموعه]    = { xkb_filter_ctrls_new,
                                  xkb_filter_ctrls_func },
    [ACTION_TYPE_CTRL_قفل]   = { xkb_filter_ctrls_new,
                                  xkb_filter_ctrls_func },
    [ACTION_TYPE_REDIRECT_کلید] = { xkb_filter_redirect_کلید_new,
                                   xkb_filter_redirect_کلید_func },
};

/**
 * Applies any relevant filters to the key, first from the list of filters
 * that are currently active, then if no filter has claimed the key, possibly
 * apply a new filter from the key action.
 */
ساكن فراغ
xkb_filter_apply_all(بنية xkb_server_state *state,
                     بنية xkb_events *events,
                     ثابت بنية xkb_کلید *کلید,
                     تعداد xkb_کلید_direction direction)
{
    /* First run through all the currently active filters and see if any of
     * them have consumed this event. */
    bool consumed = false;
    بنية xkb_filter *filter;
    darray_foreach(filter, state->filters) {
        إذا (!filter->func)
            استمر;

        إذا (filter->func(state, events, filter, کلید, direction) == XKB_FILTER_CONSUME)
            consumed = true;
    }
    إذا (consumed || direction == XKB_کلید_به_بالا)
        أرجع;

    /* No filter consumed this event, so proceed with the key actions */
    ثابت اتحاد xkb_action *actions = عدم;
    xkb_action_شمار_t شمار = xkb_کلید_get_actions(&state->پایه, کلید,
                                                         &actions);

    لكل (بنية {
            xkb_action_شمار_t شمار;
            ثابت اتحاد xkb_action *actions;
         } بعدی = {0};
         شمار;
         شمار = بعدی.شمار, بعدی.شمار = 0, actions = بعدی.actions
    ) {
        /*
         * Process actions sequentially.
         *
         * NOTE: We rely on the parser to disallow multiple modifier or group
         * actions (see `CheckMultipleActionsCategories`). Allowing multiple
         * such actions requires a refactor of the state handling.
         */
        لكل (xkb_action_شمار_t k = 0; k < شمار; k++) {
            /*
             * It's possible for the keymap to set action->type explicitly, like
             * so:
             *     interpret XF86_Next_VMode {
             *         action = Private(type=0x86, data="+VMode");
             *     };
             * We don't handle those.
             */
            إذا (actions[k].type >= ACTION_TYPE_رقم_ENTRIES)
                استمر;

            /* Go to next action if no corresponding action handler */
            إذا (!filter_action_funcs[actions[k].type].new)
                استمر;

            /* Add a new filter and run the corresponding initial action */
            filter = xkb_filter_new(state);
            filter->کلید = کلید;
            filter->action = actions[k];

            بدل (filter->action.type) {
            حالة ACTION_TYPE_MOD_مجموعه:
                إذا (state->پایه.components.controls & CONTROL_STICKY_کلیدها) {
                    /* Convert modifier set action to a latch */
                    filter->action.type = ACTION_TYPE_MOD_LATCH;
                    إذا (state->a11y_flags & XKB_A11_Y_STICKY_کلیدها_LATCH_مقصد_قفل) {
                        filter->action.mods.flags |= ACTION_LATCH_مقصد_قفل;
                    }
                }
                /* fallthrough */
            حالة ACTION_TYPE_MOD_LATCH:
            حالة ACTION_TYPE_MOD_قفل: {
                ساكن ثابت تعداد xkb_action_flags unlock_mods_on_press
                    = ACTION_قفل_CLEAR     /* SetMods & LatchMods: can unlock */
                    | ACTION_قفل_NO_UNLOCK /* LockMods: cannot unlock */
                    | ACTION_EXPLICIT_UNLOCK_ON_PRESS;
                إذا ((state->flags & STATE_UNLOCK_MODS_ON_PRESS) &&
                    /*
                     * Ensure that the action can actually unlock and has
                     * no *explicit* `unlockOnPress` parameter.
                     */
                    (filter->action.mods.flags & unlock_mods_on_press) ==
                    (filter->action.type == ACTION_TYPE_MOD_قفل
                        ? 0 : ACTION_قفل_CLEAR))
                {
                    filter->action.mods.flags |= ACTION_UNLOCK_ON_PRESS;
                }
                اكسر;
            }
            حالة ACTION_TYPE_گروه_مجموعه:
                إذا (state->پایه.components.controls & CONTROL_STICKY_کلیدها) {
                    /* Convert group set action to a latch */
                    filter->action.type = ACTION_TYPE_گروه_LATCH;
                    إذا (state->a11y_flags & XKB_A11_Y_STICKY_کلیدها_LATCH_مقصد_قفل) {
                        filter->action.گروه.flags |= ACTION_LATCH_مقصد_قفل;
                    }
                }
                اكسر;
            حالة ACTION_TYPE_گروه_قفل:
                إذا ((filter->action.گروه.flags &
                     ACTION_AUTO_گروه_قفل_ON_MODIFIER_RELEASE) &&
                    (state->flags &
                     STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_کلید) &&
                    /*
                     * FIXME: sticky keys may leave an unexpected modifier
                     * from the level 0 latched, so disable for now.
                     */
                    !(state->پایه.components.controls & CONTROL_STICKY_کلیدها))
                {
                    filter->action.گروه.flags |= ACTION_قفل_ON_RELEASE;
                    /* Add base level action */
                    بعدی.شمار = xkb_keymap_کلید_get_actions_by_level(
                        state->پایه.keymap, کلید, state->پایه.components.گروه,
                        0, &بعدی.actions
                    );
                    أكد(بعدی.شمار == 1);
                }
                اكسر;
            حالة ACTION_TYPE_PTR_MOVE:
            حالة ACTION_TYPE_PTR_BUTTON:
            حالة ACTION_TYPE_PTR_قفل:
            حالة ACTION_TYPE_PTR_DEFAULT:
                إذا (!(state->پایه.components.controls & CONTROL_MOUSE_کلیدها)) {
                    /* Convert pointer actions to NoAction */
                    أكد(!filter_action_funcs[ACTION_TYPE_NONE].new);
                    filter->func = عدم;
                    استمر;
                }
                اكسر;
            حالة ACTION_TYPE_REDIRECT_کلید:
                // FIXME: this is not efficient to resolve mods here each time
                filter->action.redirect.affect_mods = mod_ماسک_get_effective(
                    state->پایه.keymap, filter->action.redirect.affect_mods
                );
                filter->action.redirect.mods = mod_ماسک_get_effective(
                    state->پایه.keymap, filter->action.redirect.mods
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
               تعداد xkb_state_نما_داخلی نما)
{
    state->نما = نما;
    state->refcnt = 1;
    state->keymap = xkb_keymap_ref(keymap);
    state->بیرون_از_range_گروه.policy = XKB_LAYOUT_بیرون_از_RANGE_WRAP;

    /* Ensure that derived state is correctly initialized */
    xkb_state_update_derived(state);
}

ساكن مضمن فراغ
xkb_client_state_init(بنية xkb_client_state * محدود state,
                      بنية xkb_keymap * محدود keymap,
                      تعداد xkb_state_نما_داخلی نما)
{
    xkb_state_init(&state->پایه, keymap, نما);
}

ساكن مضمن فراغ
xkb_server_state_init(بنية xkb_server_state * محدود state,
                      بنية xkb_keymap * محدود keymap,
                      تعداد xkb_state_نما_داخلی نما,
                      تعداد xkb_a11y_flags a11y_affect,
                      تعداد xkb_a11y_flags a11y_flags)
{
    xkb_state_init(&state->پایه, keymap, نما);

    state->a11y_flags = a11y_flags;
    إذا (keymap->format != XKB_KEYMAP_FORMAT_متن_V1 &&
        !(a11y_affect & XKB_A11_Y_LATCH_همزمان_کلیدها)) {
             /* Keymap v2+: enable extension to XKB if not manually disabled */
             state->a11y_flags |= XKB_A11_Y_LATCH_همزمان_کلیدها;
    }
}

بنية xkb_state *
xkb_state_new(بنية xkb_keymap *keymap)
{
    بنية xkb_server_state * ثابت state = حافظه_صفر_بده(1, حجم(*state));
    إذا (!state) {
        log_err_func1(keymap->ctx, XKB_ERROR_تخصیص_FAILURE_2,
                      "Could not allocate a state object.\n");
        أرجع عدم;
    }

    xkb_server_state_init(state, keymap, LEGACY_MIXED_STATE, 0, 0);

    أرجع (بنية xkb_state *)state;
}

بنية xkb_state *
xkb_state_new_with_نما(بنية xkb_keymap * محدود keymap,
                        تعداد xkb_state_نما نما,
                        تعداد xkb_وضعیت * محدود وضعیت)
{
    بدل (نما) {
    حالة XKB_STATE_نما_CLIENT:
    حالة XKB_STATE_نما_SERVER_QUERY: {
        بنية xkb_client_state * ثابت state = حافظه_صفر_بده(1, حجم(*state));
        إذا (!state)
            اكسر;

        static_assert((بیعلامت)XKB_STATE_نما_CLIENT ==
                      (بیعلامت)CLIENT_STATE, "");
        static_assert((بیعلامت)XKB_STATE_نما_SERVER_QUERY ==
                      (بیعلامت)SERVER_COMPANION, "");
        xkb_client_state_init(state, keymap, (تعداد xkb_state_نما_داخلی)نما);

        إذا (وضعیت)
            *وضعیت = XKB_SUCCESS;

        أرجع (بنية xkb_state *)state;
    }
    حالة XKB_STATE_نما_SERVER: {
        بنية xkb_server_state * ثابت state = حافظه_صفر_بده(1, حجم(*state));
        إذا (!state)
            اكسر;

        xkb_server_state_init(state, keymap, LEGACY_SERVER_STATE, 0, 0);

        إذا (وضعیت)
            *وضعیت = XKB_SUCCESS;

        أرجع (بنية xkb_state *)state;
    }
    افتراضي:
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_STATE_نما_2,
                     "Unsupported state mode: %u\n", نما);
        إذا (وضعیت)
            *وضعیت = XKB_ERROR_UNSUPPORTED_STATE_نما;
        أرجع عدم;
    }

    log_err_func1(keymap->ctx, XKB_ERROR_تخصیص_FAILURE_2,
                  "Could not allocate a state object.\n");
    إذا (وضعیت)
        *وضعیت = XKB_ERROR_تخصیص_FAILURE;
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

    إذا (state->نما < LEGACY_MIXED_STATE) {
        static_assert(حجم(*state) == حجم(بنية xkb_client_state),
                      "Not a mere wrapper");
        /* Nothing else to reset */
    } وإلا {
        بنية xkb_server_state *state_ = (بنية xkb_server_state *)state;
        ثابت بنية xkb_server_state prev = *state_;
        memset(state_, 0, حجم(*state_));

        state_->پایه = prev.پایه;
        state_->a11y_flags = prev.a11y_flags;
        state_->filters = prev.filters;
        darray_اندازه(state_->filters) = 0;
    }
}

ساكن مضمن فراغ
xkb_state_destroy(بنية xkb_state *state)
{
    xkb_keymap_unref(state->keymap);
    إذا (state->نما > SERVER_COMPANION)
        darray_free(((بنية xkb_server_state *)state)->filters);
}

فراغ
xkb_state_unref(بنية xkb_state *state)
{
    أكد(!state || state->refcnt > 0);
    إذا (!state || --state->refcnt > 0)
        أرجع;

    xkb_state_destroy(state);

    بدل (state->نما) {
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
                mod_ماسک |= state->components.پایه_mods;
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
                xkb_layout_ماسک_t گروه_ماسک = 0;
                /* Effective and locked groups have been brought into range */
                أكد(state->components.گروه < XKB_MAX_GROUPS);
                أكد(state->components.locked_گروه >= 0 &&
                       state->components.locked_گروه < XKB_MAX_GROUPS);
                /* Effective and locked groups are used as mask */
                إذا (led->which_groups & XKB_STATE_LAYOUT_EFFECTIVE)
                    گروه_ماسک |= (UINT32_C(1) << state->components.گروه);
                إذا (led->which_groups & XKB_STATE_LAYOUT_LOCKED)
                    گروه_ماسک |= (UINT32_C(1) << state->components.locked_گروه);
                /* Base and latched groups only have to be non-zero */
                إذا ((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                    state->components.پایه_گروه != 0)
                    گروه_ماسک |= led->groups;
                إذا ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                    state->components.latched_گروه != 0)
                    گروه_ماسک |= led->groups;

                إذا (led->groups & گروه_ماسک) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    استمر;
                }
            } وإلا {
                /* Special case for Base and latched groups */
                إذا (((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                     state->components.پایه_گروه == 0) ||
                    ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                     state->components.latched_گروه == 0)) {
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

    state->components.mods = (state->components.پایه_mods |
                              state->components.latched_mods |
                              state->components.locked_mods);

    /* Lock group must be adjusted, but not base nor latched groups */
    wrapped = Xkb_Wrap_گروه_Into_Range(state->components.locked_گروه,
                                    state->keymap->رقم_groups,
                                    state->بیرون_از_range_گروه.policy,
                                    state->بیرون_از_range_گروه.redirect_گروه);
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->components.locked_گروه =
        (int32_t) (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    /* Effective group must be adjusted */
    wrapped = Xkb_Wrap_گروه_Into_Range(state->components.پایه_گروه +
                                    state->components.latched_گروه +
                                    state->components.locked_گروه,
                                    state->keymap->رقم_groups,
                                    state->بیرون_از_range_گروه.policy,
                                    state->بیرون_از_range_گروه.redirect_گروه);
    state->components.گروه =
        (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    xkb_state_led_update_all(state);
}

ساكن فراغ
state_update_enabled_controls(بنية xkb_server_state *state,
                              تعداد xkb_keyboard_control_flags affect_controls,
                              تعداد xkb_keyboard_control_flags controls,
                              بنية xkb_events *events)
{
    ثابت bool had_sticky_کلیدها = state->پایه.components.controls
                               & CONTROL_STICKY_کلیدها;

    /*
     * Enable using the public API with the all the Control values, except
     * the internal ones, if any.
     */
    affect_controls =
        ( affect_controls
        & (تعداد xkb_keyboard_control_flags)CONTROL_ALL_BOOLEAN );
    state->پایه.components.controls &=
        (تعداد xkb_action_controls)~affect_controls;
    state->پایه.components.controls |=
        (تعداد xkb_action_controls)(controls & affect_controls);

    إذا (had_sticky_کلیدها &&
        !(state->پایه.components.controls & CONTROL_STICKY_کلیدها)) {
        /* Sticky keys were disabled: clear all locks and latches */
        clear_all_latches_and_locks(state, events);
    }

    xkb_state_update_derived(&state->پایه);
}

/**
 * Given a particular key event, updates the state structure to reflect the
 * new modifiers.
 */
تعداد xkb_state_component
xkb_state_update_کلید(بنية xkb_state *پایه_state, xkb_keycode_t kc,
                     تعداد xkb_کلید_direction direction)
{
    /* Guard against client-only state */
    أكد(پایه_state->نما > SERVER_COMPANION);
    إذا (پایه_state->نما <= SERVER_COMPANION) {
        log_err(پایه_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_نما,
                "%s: Unexpected state type %d\n", __func__, پایه_state->نما);
        أرجع 0;
    }

    بنية xkb_server_state * ثابت state =
        (بنية xkb_server_state *)پایه_state;

    ثابت بنية xkb_کلید* ثابت کلید = Xkb_کلید(state->پایه.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    إذا (!کلید || (direction == XKB_کلید_REPEATED && !کلید->repeats))
        أرجع 0;

    إذا (direction == XKB_کلید_پایین &&
        (state->پایه.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیدها) &&
        (state->a11y_flags & XKB_A11_Y_STICKY_کلیدها_NO_همزمان_کلیدها) &&
        state->پایه.components.پایه_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیدها,
                                      0, عدم);
    }

    ثابت بنية state_components prev_components = state->پایه.components;

    state->مجموعه_mods = 0;
    state->clear_mods = 0;

    xkb_filter_apply_all(state, عدم, کلید, direction);

    xkb_mod_شاخص_t i;
    xkb_mod_ماسک_t بیت;
    لكل (i = 0, بیت = 1; state->مجموعه_mods; i++, بیت <<= 1) {
        إذا (state->مجموعه_mods & بیت) {
            state->mod_کلید_شمار[i]++;
            state->پایه.components.پایه_mods |= بیت;
            state->مجموعه_mods &= ~بیت;
        }
    }

    لكل (i = 0, بیت = 1; state->clear_mods; i++, بیت <<= 1) {
        إذا (state->clear_mods & بیت) {
            state->mod_کلید_شمار[i]--;
            إذا (state->mod_کلید_شمار[i] <= 0) {
                state->پایه.components.پایه_mods &= ~بیت;
                state->mod_کلید_شمار[i] = 0;
            }
            state->clear_mods &= ~بیت;
        }
    }

    xkb_state_update_derived(&state->پایه);

    أرجع get_state_component_changes(&prev_components, &state->پایه.components);
}

/* We need fake keys for `update_latch_modifiers` and `update_latch_group`.
 * These keys must have at least one level in order to break latches. We need 2
 * keys with specific actions in order to update group/mod latches without
 * affecting each other. */
ساكن بنية xkb_کلید_type_مدخل synthetic_کلید_level_مدخل = { 0 };
ساكن بنية xkb_کلید_type synthetic_کلید_type = {
     .رقم_entries = 1,
     .رقم_سطوح = 1,
     .entries = &synthetic_کلید_level_مدخل
};
ساكن ثابت بنية xkb_کلید synthetic_کلید = { 0 };

/* Transcription from xserver: XkbLatchModifiers */
ساكن فراغ
update_latch_modifiers(بنية xkb_server_state *state,
                       بنية xkb_events *events,
                       xkb_mod_ماسک_t ماسک, xkb_mod_ماسک_t latches)
{
    /* Clear affected latched modifiers */
    ثابت xkb_mod_ماسک_t clear = ماسک & ~latches;
    state->پایه.components.latched_mods &= ~clear;

    /* Clear any pending latch to locks using ad hoc action:
     * only affect corresponding modifier latches and no group latch. */
    بنية xkb_level synthetic_کلید_level_break_mod_latch = {
        .رقم_syms = 0,
        .رقم_actions = 1,
        .فوقانی = XKB_کلید_No_نماد,
        .s.sym = XKB_کلید_No_نماد,
        .a.action.داخلی = {
            .type = ACTION_TYPE_داخلی,
            .flags = داخلی_BREAKS_MOD_LATCH,
            .clear_latched_mods = clear
        }
    };
    بنية xkb_گروه synthetic_کلید_گروه_break_mod_latch = {
        .type = &synthetic_کلید_type,
        .سطوح = &synthetic_کلید_level_break_mod_latch
    };
    ثابت بنية xkb_کلید synthetic_کلید_break_mod_latch = {
        .رقم_groups = 1,
        .groups = &synthetic_کلید_گروه_break_mod_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_کلید_break_mod_latch, XKB_کلید_پایین);

    /* Finally set the latched mods by simulate tapping a key with the
     * corresponding action */
    ثابت بنية xkb_کلید* ثابت کلید = &synthetic_کلید;
    ثابت اتحاد xkb_action latch_mods = {
        .mods = {
            .type = ACTION_TYPE_MOD_LATCH,
            .mods = { .ماسک = ماسک & latches },
            .flags = 0,
        },
    };
    بنية xkb_filter* ثابت filter = xkb_filter_new(state);
    filter->کلید = کلید;
    filter->func = xkb_filter_mod_latch_func;
    filter->action = latch_mods;
    xkb_filter_mod_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_mod_latch_func(state, events, filter, کلید, XKB_کلید_به_بالا);
}

/* Transcription from xserver: XkbLatchGroup */
ساكن فراغ
update_latch_گروه(بنية xkb_server_state *state,
                   بنية xkb_events *events, int32_t گروه)
{
    /* Clear any pending latch to locks. */
    ساكن بنية xkb_level synthetic_کلید_level_break_گروه_latch = {
        .رقم_syms = 0,
        .رقم_actions = 1,
        .فوقانی = XKB_کلید_No_نماد,
        .s.sym = XKB_کلید_No_نماد,
        .a.action.داخلی = {
            .type = ACTION_TYPE_داخلی,
            .flags = داخلی_BREAKS_گروه_LATCH,
            .clear_latched_mods = 0
        }
    };
    ساكن بنية xkb_گروه synthetic_کلید_گروه_break_گروه_latch = {
        .type = &synthetic_کلید_type,
        .سطوح = &synthetic_کلید_level_break_گروه_latch
    };
    ساكن ثابت بنية xkb_کلید synthetic_کلید_break_گروه_latch = {
        .رقم_groups = 1,
        .groups = &synthetic_کلید_گروه_break_گروه_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_کلید_break_گروه_latch, XKB_کلید_پایین);

    /* Simulate tapping a key with a group latch action, but in isolation: i.e.
     * without affecting the other filters. */
    ثابت بنية xkb_کلید* ثابت کلید = &synthetic_کلید;
    ثابت اتحاد xkb_action latch_گروه = {
        .گروه = {
            .type = ACTION_TYPE_گروه_LATCH,
            .flags = ACTION_ABSOLUTE_SWITCH,
            .گروه = گروه,
        },
    };
    بنية xkb_filter* ثابت filter = xkb_filter_new(state);
    filter->کلید = کلید;
    filter->func = xkb_filter_گروه_latch_func;
    filter->action = latch_گروه;
    xkb_filter_گروه_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_گروه_latch_func(state, events, filter, کلید, XKB_کلید_به_بالا);
}

/**
 * Compute the resolved effective mask of an arbitrary input.
 *
 * Contrary to `mod_mask_get_effective`, it resolves only modifiers not present
 * in the canonical mask, so that it enables `xkb_state_serialize_mods` to
 * round trip via `xkb_state_update_mask`.
 */
ساكن مضمن xkb_mod_ماسک_t
resolve_مقصد_canonical_mods(بنية xkb_keymap *keymap, xkb_mod_ماسک_t mods)
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
        resolve_مقصد_canonical_mods(state->پایه.keymap, update->affect_locked_mods);
    إذا ((update->components & XKB_STATE_MODS_LOCKED) &&
        affect_locked_mods) {
        ثابت xkb_mod_ماسک_t locked_mods =
            resolve_مقصد_canonical_mods(state->پایه.keymap, update->locked_mods);
        state->پایه.components.locked_mods &= ~affect_locked_mods;
        state->پایه.components.locked_mods |= locked_mods & affect_locked_mods;
    }
    إذا (update->components & XKB_STATE_LAYOUT_LOCKED) {
        state->پایه.components.locked_گروه = update->locked_layout;
    }

    /* Update latches */
    ثابت xkb_mod_ماسک_t affect_latched_mods =
        resolve_مقصد_canonical_mods(state->پایه.keymap, update->affect_latched_mods);
    إذا ((update->components & XKB_STATE_MODS_LATCHED) &&
        affect_latched_mods) {
        ثابت xkb_mod_ماسک_t latched_mods =
            resolve_مقصد_canonical_mods(state->پایه.keymap, update->latched_mods);
        update_latch_modifiers(state, events, affect_latched_mods, latched_mods);
    }
    إذا (update->components & XKB_STATE_LAYOUT_LATCHED) {
        update_latch_گروه(state, events, update->latched_layout);
    }
}

تعداد xkb_state_component
xkb_state_update_latched_locked(بنية xkb_state *پایه_state,
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
    أكد(پایه_state->نما > SERVER_COMPANION);
    إذا (پایه_state->نما <= SERVER_COMPANION) {
        log_err(پایه_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_نما,
                "%s: Unexpected state type %d\n", __func__, پایه_state->نما);
        أرجع 0;
    }

    بنية xkb_server_state * ثابت state =
        (بنية xkb_server_state *)پایه_state;

    ثابت بنية state_components قبلی_components = state->پایه.components;
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
    xkb_state_update_derived(&state->پایه);
    أرجع get_state_component_changes(&قبلی_components,
                                       &state->پایه.components);
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

ساكن تعداد xkb_وضعیت
state_update_layout_policy(بنية xkb_server_state *state,
                           ثابت بنية xkb_layout_policy_update *update)
{
    إذا (xkb_feature_supported(XKB_FEATURE_ENUM_LAYOUT_بیرون_از_RANGE_POLICY,
                              update->policy)) {
        إذا (update->policy == XKB_LAYOUT_بیرون_از_RANGE_REDIRECT) {
            إذا (update->redirect < state->پایه.keymap->رقم_groups) {
                state->پایه.بیرون_از_range_گروه.redirect_گروه =
                    update->redirect;
            } وإلا {
                log_err(state->پایه.keymap->ctx,
                        XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                        "Layout policy: "
                        "unsupported layout index %"PRIu32" > %"PRIu32"\n",
                        update->redirect + 1, state->پایه.keymap->رقم_groups);
                أرجع XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص;
            }
        }
        state->پایه.بیرون_از_range_گروه.policy = update->policy;
        أرجع XKB_SUCCESS;
    } وإلا {
        log_err(state->پایه.keymap->ctx,
                XKB_ERROR_UNSUPPORTED_LAYOUT_بیرون_از_RANGE_POLICY_2,
                "Unsupported layout policy: %d\n", update->policy);
        أرجع XKB_ERROR_UNSUPPORTED_LAYOUT_بیرون_از_RANGE_POLICY;
    }
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
ساكن تعداد xkb_وضعیت
check_state_update_abi_(بنية xkb_context * محدود ctx,
                        ثابت حرف * محدود func,
                        ثابت بنية xkb_synthetic_update * محدود update)
{
    تعداد xkb_وضعیت وضعیت = XKB_SUCCESS;
    إذا ((وضعیت = xkb_check_state_abi(update)) ||
        (update->reserved0 != 0 &&
         (وضعیت = XKB_ERROR_ABI_FORWARD_COMPAT)) ||
        (update->components &&
         (وضعیت = xkb_check_state_abi(update->components))) ||
        (update->layout_policy &&
         (وضعیت = xkb_check_state_abi(update->layout_policy)))) {
        xkb_log_abi_error(ctx, func, وضعیت);
    }
    أرجع وضعیت;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

#تعریف_کن check_state_update_abi(ctx, update) \
    check_state_update_abi_(ctx, __func__, update)

تعداد xkb_وضعیت
xkb_state_update_synthetic(بنية xkb_state * پایه_state,
                           ثابت بنية xkb_synthetic_update * update,
                           تعداد xkb_state_component *تغییریافته)
{
    /* Guard against client-only state */
    إذا (پایه_state->نما <= SERVER_COMPANION) {
        log_err(پایه_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_نما,
                "%s: Unexpected state type %d\n", __func__, پایه_state->نما);
        أرجع XKB_ERROR_UNEXPECTED_STATE_نما;
    }

    بنية xkb_server_state * ثابت state =
        (بنية xkb_server_state *)پایه_state;

    /* Check ABI compatibility */
    تعداد xkb_وضعیت وضعیت = check_state_update_abi(state->پایه.keymap->ctx,
                                                    update);
    إذا (وضعیت != XKB_SUCCESS)
        أرجع وضعیت;

    ثابت بنية state_components قبلی_components = state->پایه.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    /* Update parametrized controls first */
    إذا (update->layout_policy) {
        وضعیت = state_update_layout_policy(state, update->layout_policy);
        إذا (وضعیت != XKB_SUCCESS)
            أرجع وضعیت;
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

    xkb_state_update_derived(&state->پایه);

    إذا (تغییریافته) {
        *تغییریافته = get_state_component_changes(&قبلی_components,
                                               &state->پایه.components);
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
xkb_state_update_ماسک(بنية xkb_state *پایه_state,
                      xkb_mod_ماسک_t پایه_mods,
                      xkb_mod_ماسک_t latched_mods,
                      xkb_mod_ماسک_t locked_mods,
                      xkb_layout_شاخص_t پایه_گروه,
                      xkb_layout_شاخص_t latched_گروه,
                      xkb_layout_شاخص_t locked_گروه)
{
    /* Guard against server-only and server companion state */
    أكد(پایه_state->نما == CLIENT_STATE ||
           پایه_state->نما == LEGACY_MIXED_STATE);
    إذا (پایه_state->نما != CLIENT_STATE &&
        پایه_state->نما != LEGACY_MIXED_STATE) {
        log_err(پایه_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_نما,
                "%s: Unexpected state type %d\n", __func__, پایه_state->نما);
        أرجع 0;
    }

    بنية xkb_client_state * ثابت state =
        (بنية xkb_client_state *)پایه_state;

    ثابت بنية state_components prev_components = state->پایه.components;

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
    state->پایه.components.پایه_mods =
        resolve_مقصد_canonical_mods(state->پایه.keymap, پایه_mods);
    state->پایه.components.latched_mods =
        resolve_مقصد_canonical_mods(state->پایه.keymap, latched_mods);
    state->پایه.components.locked_mods =
        resolve_مقصد_canonical_mods(state->پایه.keymap, locked_mods);

    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->پایه.components.پایه_گروه = (int32_t) پایه_گروه;
    state->پایه.components.latched_گروه = (int32_t) latched_گروه;
    state->پایه.components.locked_گروه = (int32_t) locked_گروه;

    xkb_state_update_derived(&state->پایه);

    أرجع get_state_component_changes(&prev_components, &state->پایه.components);
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
xkb_state_کلید_get_syms(بنية xkb_state *state, xkb_keycode_t kc,
                       ثابت xkb_keysym_t **syms_بیرون)
{
    ثابت xkb_layout_شاخص_t layout = xkb_state_کلید_get_layout(state, kc);
    إذا (layout == XKB_LAYOUT_INVALID)
        اذهب err;

    ثابت xkb_level_شاخص_t level = xkb_state_کلید_get_level(state, kc, layout);
    إذا (level == XKB_LEVEL_INVALID)
        اذهب err;

    ثابت بنية xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);

    إذا (!کلید)
        اذهب err;

    ثابت بنية xkb_level* ثابت leveli =
        xkb_keymap_کلید_get_level(state->keymap, کلید, layout, level);

    إذا (!leveli)
        اذهب err;

    ثابت xkb_keysym_شمار_t رقم_syms = leveli->رقم_syms;
    إذا (رقم_syms == 0)
        اذهب err;

    إذا (should_do_caps_transformation(state, kc)) {
        /* Only simple capitalization rules: keysyms count is unchanged. */
        إذا (رقم_syms > 1) {
            *syms_بیرون = (leveli->has_فوقانی)
                      ? leveli->s.syms + رقم_syms
                      : leveli->s.syms;
        } وإلا {
            *syms_بیرون = &leveli->فوقانی;
        }
    } وإلا {
        *syms_بیرون = (رقم_syms > 1)
                  ? leveli->s.syms
                  : &leveli->s.sym;
    }
    أرجع (صحيح) رقم_syms;

err:
    *syms_بیرون = عدم;
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
Xkb_مقصد_Control(حرف ch)
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
xkb_state_کلید_get_one_sym(بنية xkb_state *state, xkb_keycode_t kc)
{
    ثابت xkb_keysym_t *syms = عدم;
    ثابت صحيح رقم_syms = xkb_state_کلید_get_syms(state, kc, &syms);

    إذا (رقم_syms != 1)
        أرجع XKB_کلید_No_نماد;
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
    ثابت xkb_layout_شاخص_t layout = xkb_state_کلید_get_layout(state, kc);
    ثابت xkb_layout_شاخص_t رقم_layouts =
        xkb_keymap_رقم_layouts_for_کلید(state->keymap, kc);
    xkb_level_شاخص_t level = xkb_state_کلید_get_level(state, kc, layout);
    إذا (layout == XKB_LAYOUT_INVALID || رقم_layouts == 0 ||
        level == XKB_LEVEL_INVALID)
        أرجع XKB_کلید_No_نماد;

    ثابت xkb_keysym_t *syms = عدم;
    صحيح nsyms = xkb_keymap_کلید_get_syms_by_level(state->keymap, kc,
                                                 layout, level, &syms);
    إذا (nsyms != 1)
        أرجع XKB_کلید_No_نماد;
    xkb_keysym_t sym = syms[0];

    إذا (should_do_ctrl_transformation(state, kc) && sym > 127u) {
        لكل (xkb_layout_شاخص_t i = 0; i < رقم_layouts; i++) {
            level = xkb_state_کلید_get_level(state, kc, i);
            إذا (level == XKB_LEVEL_INVALID)
                استمر;

            nsyms = xkb_keymap_کلید_get_syms_by_level(state->keymap, kc,
                                                     i, level, &syms);
            إذا (nsyms == 1 && syms[0] <= 127u) {
                sym = syms[0];
                اكسر;
            }
        }
    }

    إذا (should_do_caps_transformation(state, kc)) {
        sym = xkb_keysym_مقصد_فوقانی(sym);
    }

    أرجع sym;
}

صحيح
xkb_state_کلید_get_utf8(بنية xkb_state *state, xkb_keycode_t kc,
                       حرف *میانگیر, اندازه_t اندازه)
{
    صحيح nsyms;
    ثابت xkb_keysym_t *syms = عدم;
    ثابت xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    إذا (sym != XKB_کلید_No_نماد) {
        nsyms = 1; syms = &sym;
    }
    وإلا {
        nsyms = xkb_state_کلید_get_syms(state, kc, &syms);
    }

    /* Make sure not to truncate in the middle of a UTF-8 sequence. */
    صحيح جابه_جایی = 0;
    حرف tmp[XKB_KEYSYM_UTF8_MAX_اندازه];
    لكل (صحيح i = 0; i < nsyms; i++) {
        صحيح ret = xkb_keysym_مقصد_utf8(syms[i], tmp, حجم(tmp));
        إذا (ret <= 0)
            اذهب err_bad;

        ret--;
        إذا ((اندازه_t)جابه_جایی + (اندازه_t)ret <= اندازه)
            کپی_حافظه(میانگیر + جابه_جایی, tmp, (اندازه_t)ret);
        جابه_جایی += ret;
    }

    إذا ((اندازه_t)جابه_جایی >= اندازه)
        اذهب err_trunc;
    میانگیر[جابه_جایی] = '\0';

    إذا (!is_valid_utf8(میانگیر, (اندازه_t)جابه_جایی))
        اذهب err_bad;

    إذا (جابه_جایی == 1 && (بیعلامت صحيح) میانگیر[0] <= 127u &&
        should_do_ctrl_transformation(state, kc))
        میانگیر[0] = Xkb_مقصد_Control(میانگیر[0]);

    أرجع جابه_جایی;

err_trunc:
    إذا (اندازه > 0)
        میانگیر[اندازه - 1] = '\0';
    أرجع جابه_جایی;

err_bad:
    إذا (اندازه > 0)
        میانگیر[0] = '\0';
    أرجع 0;
}

uint32_t
xkb_state_کلید_get_utf32(بنية xkb_state *state, xkb_keycode_t kc)
{
    ثابت xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    uint32_t cp = xkb_keysym_مقصد_utf32(sym);

    إذا (cp <= 127u && should_do_ctrl_transformation(state, kc))
        cp = (uint32_t) Xkb_مقصد_Control((حرف) cp);

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
        ret |= components->پایه_mods;
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
        أرجع components->گروه;

    int32_t ret = 0;
    إذا (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret += components->پایه_گروه;
    إذا (type & XKB_STATE_LAYOUT_LATCHED)
        ret += components->latched_گروه;
    إذا (type & XKB_STATE_LAYOUT_LOCKED)
        ret += components->locked_گروه;

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
    إذا (unlikely(idx >= xkb_keymap_رقم_mods(state->keymap)))
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
    XKB_STATE_تطابق_FLAGS = XKB_STATE_تطابق_ANY
                          | XKB_STATE_تطابق_ALL
                          | XKB_STATE_تطابق_NON_EXCLUSIVE
};

/**
 * Helper function for xkb_state_mod_indices_are_active and
 * xkb_state_mod_names_are_active.
 */
ساكن bool
تطابق_mod_masks(بنية xkb_state *state,
                تعداد xkb_state_component type,
                تعداد xkb_state_تطابق تطابق,
                xkb_mod_ماسک_t wanted)
{
    ثابت xkb_mod_ماسک_t فعال = xkb_state_serialize_mods(state, type);

    إذا (!(تطابق & XKB_STATE_تطابق_NON_EXCLUSIVE) && (فعال & ~wanted))
        أرجع false;

    إذا (تطابق & XKB_STATE_تطابق_ANY)
        أرجع فعال & wanted;

    /* Ensure all flags but XKB_STATE_MATCH_ALL are processed before */
    static_assert(XKB_STATE_تطابق_ALL ==
                  ((تعداد xkb_state_تطابق) XKB_STATE_تطابق_FLAGS &
                   ~(XKB_STATE_تطابق_ANY | XKB_STATE_تطابق_NON_EXCLUSIVE)),
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
                                 تعداد xkb_state_تطابق تطابق,
                                 ...)
{
    إذا (تطابق & ~((تعداد xkb_state_تطابق) XKB_STATE_تطابق_FLAGS))
        أرجع -2;

    va_list ap;
    xkb_mod_ماسک_t wanted = 0;
    صحيح ret = 0;
    ثابت xkb_mod_شاخص_t رقم_mods = xkb_keymap_رقم_mods(state->keymap);

    va_شروع(ap, تطابق);
    طالما (1) {
        xkb_mod_شاخص_t idx = va_arg(ap, xkb_mod_شاخص_t);
        إذا (idx == XKB_MOD_INVALID)
            اكسر;
        إذا (unlikely(idx >= رقم_mods)) {
            ret = -1;
            اكسر;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_انتها(ap);

    إذا (ret == -1)
        أرجع ret;

    إذا (!wanted) {
        /* Modifiers not mapped */
        أرجع 0;
    }

    أرجع تطابق_mod_masks(state, type, تطابق, wanted);
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
xkb_state_mod_نامها_are_فعال(بنية xkb_state *state,
                               تعداد xkb_state_component type,
                               تعداد xkb_state_تطابق تطابق,
                               ...)
{
    إذا (تطابق & ~((تعداد xkb_state_تطابق) XKB_STATE_تطابق_FLAGS))
        أرجع -2;

    va_list ap;
    xkb_mod_ماسک_t wanted = 0;
    صحيح ret = 0;

    va_شروع(ap, تطابق);
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
    va_انتها(ap);

    إذا (ret == -1)
        أرجع ret;

    إذا (!wanted) {
        /* Modifiers not mapped */
        أرجع 0;
    }

    أرجع تطابق_mod_masks(state, type, تطابق, wanted);
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
    إذا (idx >= state->keymap->رقم_groups)
        أرجع -1;

    صحيح ret = 0;
    إذا (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ret |= (state->components.گروه == idx);
    إذا (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret |= (state->components.پایه_گروه == (int32_t) idx);
    إذا (type & XKB_STATE_LAYOUT_LATCHED)
        ret |= (state->components.latched_گروه == (int32_t) idx);
    إذا (type & XKB_STATE_LAYOUT_LOCKED)
        ret |= (state->components.locked_گروه == (int32_t) idx);

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
    إذا (idx >= state->keymap->رقم_leds ||
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
کلید_get_consumed(بنية xkb_state *state, ثابت بنية xkb_کلید *کلید,
                 تعداد xkb_consumed_نما نما)
{
    ثابت xkb_layout_شاخص_t گروه =
        xkb_state_کلید_get_layout(state, کلید->keycode);
    إذا (گروه == XKB_LAYOUT_INVALID)
        أرجع 0;

    xkb_mod_ماسک_t preserve = 0;
    xkb_mod_ماسک_t consumed = 0;

    ثابت بنية xkb_کلید_type_مدخل* ثابت matching_مدخل =
        get_مدخل_for_کلید_state(state, کلید, گروه);
    إذا (matching_مدخل)
        preserve = matching_مدخل->preserve.ماسک;

    ثابت بنية xkb_کلید_type* ثابت type = کلید->groups[گروه].type;
    بدل (نما) {
    حالة XKB_CONSUMED_نما_XKB:
        consumed = type->mods.ماسک;
        اكسر;

    حالة XKB_CONSUMED_نما_GTK: {
        ثابت بنية xkb_کلید_type_مدخل* ثابت no_mods_مدخل =
            get_مدخل_for_mods(type, 0);
        ثابت xkb_level_شاخص_t no_mods_leveli = no_mods_مدخل
                                               ? no_mods_مدخل->level
                                               : 0;
        ثابت بنية xkb_level* ثابت no_mods_level =
            &کلید->groups[گروه].سطوح[no_mods_leveli];

        لكل (darray_اندازه_t i = 0; i < type->رقم_entries; i++) {
            ثابت بنية xkb_کلید_type_مدخل* ثابت مدخل = &type->entries[i];
            إذا (!مدخل_is_فعال(مدخل))
                استمر;

            ثابت بنية xkb_level* ثابت level =
                &کلید->groups[گروه].سطوح[مدخل->level];
            إذا (Xkb_سطوح_Same_Syms(level, no_mods_level))
                استمر;

            إذا (مدخل == matching_مدخل || one_بیت_مجموعه(مدخل->mods.ماسک))
                consumed |= مدخل->mods.ماسک & ~مدخل->preserve.ماسک;
        }
        اكسر;
    }
    }

    أرجع consumed & ~preserve;
}

صحيح
xkb_state_mod_شاخص_is_consumed2(بنية xkb_state *state, xkb_keycode_t kc,
                                 xkb_mod_شاخص_t idx,
                                 تعداد xkb_consumed_نما نما)
{
    ثابت بنية xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);

    إذا (unlikely(!کلید || idx >= xkb_keymap_رقم_mods(state->keymap)))
        أرجع -1;

    ثابت xkb_mod_ماسک_t mapping = state->keymap->mods.mods[idx].mapping;
    إذا (!mapping) {
        /* Modifier not mapped */
        أرجع 0;
    }
    أرجع (mapping & کلید_get_consumed(state, کلید, نما)) == mapping;
}

صحيح
xkb_state_mod_شاخص_is_consumed(بنية xkb_state *state, xkb_keycode_t kc,
                                xkb_mod_شاخص_t idx)
{
    أرجع xkb_state_mod_شاخص_is_consumed2(state, kc, idx,
                                            XKB_CONSUMED_نما_XKB);
}

xkb_mod_ماسک_t
xkb_state_mod_ماسک_remove_consumed(بنية xkb_state *state, xkb_keycode_t kc,
                                   xkb_mod_ماسک_t ماسک)
{
    ثابت بنية xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);

    إذا (!کلید)
        أرجع 0;

    أرجع resolve_مقصد_canonical_mods(state->keymap, ماسک) &
           ~کلید_get_consumed(state, کلید, XKB_CONSUMED_نما_XKB);
}

xkb_mod_ماسک_t
xkb_state_کلید_get_consumed_mods2(بنية xkb_state *state, xkb_keycode_t kc,
                                 تعداد xkb_consumed_نما نما)
{
    بدل (نما) {
    حالة XKB_CONSUMED_نما_XKB:
    حالة XKB_CONSUMED_نما_GTK:
        اكسر;
    افتراضي:
        log_err_func(state->keymap->ctx, XKB_LOG_پیام_NO_ID,
                     "unrecognized consumed modifiers mode: %d\n", نما);
        أرجع 0;
    }

    ثابت بنية xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);
    إذا (!کلید)
        أرجع 0;

    أرجع کلید_get_consumed(state, کلید, نما);
}

xkb_mod_ماسک_t
xkb_state_کلید_get_consumed_mods(بنية xkb_state *state, xkb_keycode_t kc)
{
    أرجع xkb_state_کلید_get_consumed_mods2(state, kc, XKB_CONSUMED_نما_XKB);
}

/*******************************************************************************
 * Server state machine API
 ******************************************************************************/

/** Entry to keep track of the state of an overlaid key */
بنية xkb_overlaid_کلید {
    /** The source key being overlaid */
    ثابت بنية xkb_کلید *old;
    /** The target key */
    ثابت بنية xkb_کلید *new;
    /** Counter of key currently down */
    صحيح refcnt;
};

بنية xkb_shortcuts_config_مدخل {
    /** Real modifier mask to trigger shortcuts tweaks */
    xkb_mod_ماسک_t mods;
    /** Target layout */
    xkb_layout_شاخص_t هدف;
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
    بنية xkb_server_state پایه;

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
        darray(بنية xkb_overlaid_کلید) کلیدها;
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
            darray_اندازه_t mappings_رقم;
            بنية machine_mods_mapping {
                xkb_mod_ماسک_t منبع;
                xkb_mod_ماسک_t هدف;
            } * mappings ATTR_COUNTED_BY(mappings_رقم);
        } modifiers;

        /** Shortcuts tweak */
        بنية machine_shortcuts_config {
            /** Substitution entries */
            بنية xkb_shortcuts_config_مدخل *entries;
        } shortcuts;
    } config;

    /** Mouse keys handling */
    بنية {
        /** Default pointer button */
        xkb_اشاره_گر_button_شاخص_t default_button;
        /** Mask of the locked buttons */
        xkb_اشاره_گر_button_ماسک_t locked_buttons;
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
        darray(بنية xkb_shortcuts_config_مدخل) entries;
    } shortcuts;

    تعداد xkb_machine_flags machine_flags;
    تعداد xkb_machine_builder_flags builder_flags;
    صحيح refcnt;
};

بنية xkb_machine_builder *
xkb_machine_builder_new(
    بنية xkb_keymap * محدود keymap,
    ثابت بنية xkb_machine_builder_config * محدود config,
    تعداد xkb_وضعیت * محدود وضعیت
)
{
    /* Handle default configuration */
    ساكن ثابت بنية xkb_machine_builder_config default_config = {
        .اندازه = حجم(default_config)
    };
    إذا (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ثابت تعداد xkb_وضعیت وضعیت_2 = xkb_check_state_abi(config);
    إذا (وضعیت_2) {
        xkb_log_abi_error(keymap->ctx, __func__, وضعیت_2);
        إذا (وضعیت)
            *وضعیت = وضعیت_2;
        أرجع عدم;
    }

    /* Sanitize input */
    ثابت تعداد xkb_machine_builder_flags invalid_builder_flags
        = config->builder_flags
        & ~(تعداد xkb_machine_builder_flags)XKB_MACHINE_BUILDER_FLAGS_مقادیر;
    إذا (invalid_builder_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS_,
                     "unrecognized machine builder flags: 0x%x\n",
                     invalid_builder_flags);
        إذا (وضعیت)
            *وضعیت = XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS;
        أرجع عدم;
    }

    ثابت تعداد xkb_machine_flags invalid_machine_flags
        = (تعداد xkb_machine_flags)config->machine_flags
        & ~(تعداد xkb_machine_flags)XKB_MACHINE_FLAGS_مقادیر;
    إذا (invalid_machine_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS_,
                     "unrecognized state machine flags: 0x%x\n",
                     invalid_machine_flags);
        إذا (وضعیت)
            *وضعیت = XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS;
        أرجع عدم;
    }

    بنية xkb_machine_builder * ثابت builder = حافظه_صفر_بده(1, حجم(*builder));
    إذا (!builder) {
        log_err_func1(keymap->ctx, XKB_ERROR_تخصیص_FAILURE_2,
                      "cannot allocate machine builder\n");
        إذا (وضعیت)
            *وضعیت = XKB_ERROR_تخصیص_FAILURE;
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

    إذا (وضعیت)
        *وضعیت = XKB_SUCCESS;

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

تعداد xkb_وضعیت
xkb_machine_builder_update_a11y(
    بنية xkb_machine_builder * محدود builder,
    ثابت بنية xkb_machine_builder_a11y_update * محدود update)
{
    /* Check ABI compatibility */
    تعداد xkb_وضعیت وضعیت = xkb_check_state_abi(update);
    إذا (وضعیت != XKB_SUCCESS) {
        xkb_log_abi_error(builder->keymap->ctx, __func__, وضعیت);
        أرجع وضعیت;
    }

    ثابت تعداد xkb_a11y_flags invalid_flags =
        ~(تعداد xkb_a11y_flags)XKB_A11_Y_FLAGS_مقادیر;

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

تعداد xkb_وضعیت
xkb_machine_builder_update_mods_remap(
    بنية xkb_machine_builder * محدود builder,
    ثابت بنية xkb_machine_builder_mods_remap_update * محدود update
)
{
    إذا (!update->منبع) {
        إذا (!update->هدف) {
            /* Reset mappings */
            darray_resize(builder->mods, 0);
            أرجع XKB_SUCCESS;
        } وإلا {
            أرجع XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک;
        }
    }

    /* Check the modifiers against the keymap */
    ثابت xkb_mod_ماسک_t invalid = ~builder->keymap->canonical_state_ماسک;
    إذا ((update->منبع & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک,
                "%s: Invalid source modifiers: 0x%"PRIx32"\n",
                __func__, update->منبع);
        أرجع XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک;
    }
    إذا ((update->هدف & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک,
                "%s: Invalid target modifiers: 0x%"PRIx32"\n",
                __func__, update->هدف);
        أرجع XKB_ERROR_UNSUPPORTED_MODIFIER_ماسک;
    }

    بنية machine_mods_mapping *mapping = عدم;
    darray_اندازه_t m = 0;
    darray_enumerate(m, mapping, builder->mods) {
        إذا (mapping->منبع == update->منبع) {
            إذا (!update->هدف) {
                /* Remove mapping */
                darray_remove(builder->mods, m);
            } وإلا {
                /* Update mapping */
                mapping->هدف = update->هدف;
            }
            أرجع XKB_SUCCESS;
        }
    }

    إذا (update->هدف) {
        /* Append new mapping */
        darray_append(
            builder->mods,
            (بنية machine_mods_mapping) {
                .منبع = update->منبع,
                .هدف = update->هدف
            }
        );
    } وإلا {
        /* Ignore missing mapping */
    }

    أرجع XKB_SUCCESS;
}

تعداد xkb_وضعیت
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
    إذا (update->منبع >= keymap->رقم_groups &&
        update->منبع != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                "%s: Invalid source layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->رقم_groups, update->منبع + 1);
        أرجع XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص;
    }
    إذا (update->هدف >= keymap->رقم_groups &&
        update->هدف != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                "%s: Invalid target layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->رقم_groups, update->هدف + 1);
        أرجع XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص;
    }

    بنية xkb_shortcuts_config_options * ثابت config = &builder->shortcuts;

    /* Resize array & initialize new entries, if relevant */
    إذا (update->منبع >= darray_اندازه(config->entries) &&
        update->منبع != XKB_LAYOUT_INVALID) {
        إذا (update->هدف == update->منبع) {
            /* Skip default setting */
            أرجع XKB_SUCCESS;
        }
        xkb_layout_شاخص_t new = darray_اندازه(config->entries);
        darray_resize(config->entries, update->منبع + 1);
        لكل (; new <= update->منبع; new++) {
            darray_قلم(config->entries, new) =
                (بنية xkb_shortcuts_config_مدخل) {
                    .هدف = XKB_LAYOUT_INVALID,
                    .mods = 0
                };
        }
    }

    بنية xkb_shortcuts_config_مدخل *مدخل;
    xkb_layout_شاخص_t منبع = (update->منبع == XKB_LAYOUT_INVALID)
        ? 0               /* Loop over all entries */
        : update->منبع; /* Single entry */
    darray_foreach_from(مدخل, config->entries, منبع) {
        إذا (update->منبع == XKB_LAYOUT_INVALID &&
            مدخل->هدف == XKB_LAYOUT_INVALID) {
            /* Skip previous deactivated entry */
            منبع++;
            استمر;
        }

        إذا (update->affect_mods) {
            مدخل->mods &= ~update->affect_mods;
            مدخل->mods |= (update->affect_mods & update->mods);
        }

        إذا (update->منبع != XKB_LAYOUT_INVALID) {
            مدخل->هدف = (update->هدف == منبع)
                ? XKB_LAYOUT_INVALID
                : update->هدف;
            /* Single entry: exit loop */
            اكسر;
        }
        منبع++;
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
    ثابت بیعلامت صحيح count1 = popcount32(m1);
    ثابت بیعلامت صحيح count2 = popcount32(m2);
    إذا (count1 > count2)
        أرجع -1;
    إذا (count1 < count2)
        أرجع +1;

    /* Give priority to real modifiers */
    أرجع (m1 < m2) ? -1 : +1;
}

ساكن تعداد xkb_وضعیت
machine_مجموعه_mods(بنية xkb_machine *sm,
                 ثابت machine_mods_mappings *raw_mappings)
{
    إذا (!darray_خالی(*raw_mappings)) {
        machine_mods_mappings mappings = darray_new();
        xkb_mod_ماسک_t ماسک = 0;
        ثابت بنية machine_mods_mapping *mapping;
        ثابت xkb_mod_ماسک_t invalid =
            ~sm->پایه.پایه.keymap->canonical_state_ماسک;
        darray_foreach(mapping, *raw_mappings) {
            إذا (!mapping->منبع || !mapping->هدف ||
                (mapping->منبع & invalid) || (mapping->هدف & invalid))
                استمر;
            darray_append(mappings, *mapping);
            ماسک |= mapping->منبع;
        }

        /*
         * Re-order the mappings, so that combinations have higher priority
         * if they are superset of other combinations.
         * For other type of overlaps, see the sorting function.
         */
        qsort(darray_اقلام(mappings), darray_اندازه(mappings),
              حجم(*darray_اقلام(mappings)), &cmp_mod_masks);

        darray_steal(
            mappings,
            &sm->config.modifiers.mappings,
            &sm->config.modifiers.mappings_رقم
        );
        sm->config.modifiers.ماسک = ماسک;
    } وإلا {
        sm->config.modifiers = (بنية machine_modifiers_config) {
            .mappings = عدم,
            .mappings_رقم = 0,
            .ماسک = 0
        };
    }

    أرجع XKB_SUCCESS;
}

ساكن تعداد xkb_وضعیت
machine_مجموعه_shortcuts(
    بنية xkb_machine * محدود sm,
    ثابت بنية xkb_shortcuts_config_options * محدود options
)
{
    إذا (darray_خالی(options->entries)) {
        sm->config.shortcuts = (بنية machine_shortcuts_config) {
            .entries = عدم
        };
        أرجع XKB_SUCCESS;
    }

    بنية xkb_keymap * ثابت keymap = sm->پایه.پایه.keymap;

    بنية xkb_shortcuts_config_مدخل * entries =
        حافظه_صفر_بده(keymap->رقم_groups, حجم(*entries));
    إذا (!entries)
        أرجع XKB_ERROR_تخصیص_FAILURE;

    ثابت xkb_layout_شاخص_t شمار = MIN(
        keymap->رقم_groups,
        (xkb_layout_شاخص_t) darray_اندازه(options->entries)
    );

    bool some_entries = false;
    لكل (xkb_layout_شاخص_t l = 0; l < شمار; l++) {
        /* Sanitize layout target */
        ثابت xkb_layout_شاخص_t هدف =
            darray_قلم(options->entries, l).هدف;
        /* Sanitize modifier mask */
        ثابت xkb_mod_ماسک_t mods = (
            darray_قلم(options->entries, l).mods &
            sm->پایه.پایه.keymap->canonical_state_ماسک
        );
        إذا (هدف < keymap->رقم_groups && هدف != l && mods) {
            entries[l] = (بنية xkb_shortcuts_config_مدخل) {
                .هدف = هدف,
                .mods = mods
            };
            some_entries = true;
        } وإلا {
            entries[l] = (بنية xkb_shortcuts_config_مدخل) {
                .هدف = XKB_LAYOUT_INVALID,
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
        لكل (xkb_layout_شاخص_t l = شمار; l < keymap->رقم_groups; l++) {
            entries[l] = (بنية xkb_shortcuts_config_مدخل) {
                .هدف = XKB_LAYOUT_INVALID,
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
                تعداد xkb_وضعیت * محدود وضعیت)
{
    بنية xkb_machine * ثابت machine = حافظه_صفر_بده(1, حجم(*machine));
    إذا (!machine) {
        log_err_func1(builder->keymap->ctx, XKB_ERROR_تخصیص_FAILURE_2,
                      "cannot allocate machine\n");
        إذا (وضعیت)
            *وضعیت = XKB_ERROR_تخصیص_FAILURE;
        أرجع عدم;
    }

    xkb_server_state_init(&machine->پایه, builder->keymap, SERVER_STATE,
                          builder->controls.a11y.affect_flags,
                          builder->controls.a11y.flags);
    machine->flags = builder->machine_flags;
    إذا (!(machine->flags &
          XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_کلید_FIX))
    {
        machine->پایه.flags |= STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_کلید;
    }
    إذا (machine->flags & XKB_MACHINE_UNLOCK_MODS_ON_PRESS) {
        machine->پایه.flags |= STATE_UNLOCK_MODS_ON_PRESS;
    }

    تعداد xkb_وضعیت وضعیت_2;
    إذا ((وضعیت_2 = machine_مجموعه_mods(machine, &builder->mods)) != XKB_SUCCESS ||
        (وضعیت_2 = machine_مجموعه_shortcuts(machine, &builder->shortcuts)) !=
        XKB_SUCCESS) {
        *وضعیت = وضعیت_2;
        اذهب error;
    }

    darray_init(machine->overlays.کلیدها);
    machine->mouse.default_button = XKB_اشاره_گر_BUTTON_MIN;

    إذا (وضعیت)
        *وضعیت = XKB_SUCCESS;

    أرجع machine;

error:
    xkb_machine_unref(machine);
    أرجع عدم;
}

بنية xkb_machine *
xkb_machine_ref(بنية xkb_machine *sm)
{
    أكد(sm->پایه.پایه.refcnt > 0);
    sm->پایه.پایه.refcnt++;
    أرجع sm;
}

فراغ
xkb_machine_unref(بنية xkb_machine *sm)
{
    أكد(!sm || sm->پایه.پایه.refcnt > 0);
    إذا (!sm || --sm->پایه.پایه.refcnt > 0)
        أرجع;

    xkb_state_destroy(&sm->پایه.پایه);
    darray_free(sm->overlays.کلیدها);
    حرر(sm->config.shortcuts.entries);
    حرر(sm->config.modifiers.mappings);
    حرر(sm);
}

بنية xkb_keymap *
xkb_machine_get_keymap(ثابت بنية xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    أرجع sm->پایه.پایه.keymap;
}

بنية xkb_state *
xkb_machine_get_state(بنية xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    أرجع (بنية xkb_state *)sm;
}

بنية xkb_state *
xkb_state_new_from_machine(ثابت بنية xkb_machine * محدود machine,
                           تعداد xkb_وضعیت * محدود وضعیت)
{
    /* Creation */
    تعداد xkb_وضعیت وضعیت_2;
    بنية xkb_state * ثابت state = xkb_state_new_with_نما(
        machine->پایه.پایه.keymap,
        XKB_STATE_نما_SERVER_QUERY,
        &وضعیت_2
    );
    إذا (وضعیت_2 != XKB_SUCCESS) {
        إذا (وضعیت)
            *وضعیت = وضعیت_2;
        أرجع عدم;
    }

    أكد(state->نما == SERVER_COMPANION);
    أكد(state->refcnt == 1);

    /* Copy machine’s state */
    *state = machine->پایه.پایه;

    /* Restore previous values */
    state->نما = SERVER_COMPANION;
    state->refcnt = 1;

    إذا (وضعیت)
        *وضعیت = XKB_SUCCESS;
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
        overlays_from_controls(sm->پایه.پایه.components.controls);
    xkb_overlay_ماسک_t added = ماسک & ~sm->overlays.enabled;

    /* Remove overlays no longer enabled and keep relative order */
    uint32_t ترتیب = sm->overlays.ترتیب;
    ثابت xkb_overlay_شاخص_t overlay_max =
        format_max_overlays(sm->پایه.پایه.keymap->format);
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

تعداد xkb_وضعیت
xkb_machine_process_synthetic(بنية xkb_machine *sm,
                              ثابت بنية xkb_synthetic_update *update,
                              بنية xkb_events *events)
{
    /* Check ABI compatibility */
    تعداد xkb_وضعیت وضعیت =
        check_state_update_abi(sm->پایه.پایه.keymap->ctx, update);
    إذا (وضعیت != XKB_SUCCESS)
        أرجع وضعیت;

    بنية xkb_server_state * ثابت state = &sm->پایه;
    ثابت بنية state_components قبلی_components = state->پایه.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    darray_اندازه(events->صف) = 0;
    events->بعدی = 0;

    /* Update parametrized controls first */
    إذا (update->layout_policy) {
        وضعیت = state_update_layout_policy(state, update->layout_policy);
        إذا (وضعیت != XKB_SUCCESS)
            أرجع وضعیت;
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

    xkb_state_update_derived(&state->پایه);

    ثابت تعداد xkb_state_component تغییریافته = get_state_component_changes(
        &قبلی_components, &state->پایه.components
    );
    إذا (تغییریافته) {
        // TODO: latch controls
        إذا (تغییریافته & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        /* Create event only if some component actually changed */
        darray_append(events->صف, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .تغییریافته = تغییریافته,
                .components = state->پایه.components
            }
        });

        /* Terminate the frame */
        darray_append(events->صف, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_قاب,
        });
    }

    أرجع XKB_SUCCESS;
}

ساكن ssize_t
do_remap_modifiers(ثابت بنية machine_modifiers_config * محدود mappings,
                   بنية xkb_state * محدود state,
                   بنية xkb_events * محدود events,
                   ثابت بنية xkb_کلید * محدود کلید)
{
    إذا (!(mappings->ماسک & state->components.mods))
        أرجع -1;

    ثابت xkb_layout_شاخص_t layout = state_کلید_get_layout(state, کلید);
    إذا (layout >= کلید->رقم_groups)
        أرجع -1;

    ثابت بنية xkb_کلید_type * ثابت type = کلید->groups[layout].type;
    xkb_mod_ماسک_t affect_mods = 0;
    xkb_mod_ماسک_t mods = 0;
    لكل (darray_اندازه_t m = 0; m < mappings->mappings_رقم; m++) {
        ثابت بنية machine_mods_mapping * ثابت mapping =
            &mappings->mappings[m];
        إذا (/* Skip not matching active mods */
            (mapping->منبع & state->components.mods) == mapping->منبع &&
            /* Skip overlap with previous active mappings */
            !(mapping->منبع & affect_mods) &&
            /* Skip if the key type uses some of the source modifiers */
            !(mapping->منبع & type->mods.ماسک)) {
            affect_mods |= mappings->mappings[m].منبع;
            mods |= mappings->mappings[m].هدف;
        }
    }

    إذا (!affect_mods)
        أرجع -1;

    بنية xkb_state new = *state;
    new.components.پایه_mods = (new.components.پایه_mods & ~affect_mods) | mods;
    new.components.latched_mods = (new.components.latched_mods & ~affect_mods);
    new.components.locked_mods = (new.components.locked_mods & ~affect_mods);
    xkb_state_update_derived(&new);

    ssize_t event_idx = -1;
    ثابت تعداد xkb_state_component تغییریافته =
        get_state_component_changes(&state->components, &new.components);
    إذا (تغییریافته) {
        event_idx = (ssize_t) darray_اندازه(events->صف);
        darray_append(events->صف, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = new.components,
                .تغییریافته = تغییریافته
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
                   ثابت بنية state_components *قبلی_components,
                   بنية xkb_events *events, ssize_t remap_event)
{
    إذا (!config->entries) {
        /* No shortcuts tweak */
        أرجع remap_event;
    }

    ثابت بنية xkb_shortcuts_config_مدخل * ثابت مدخل =
        &config->entries[state->components.گروه];

    إذا (مدخل->هدف != XKB_LAYOUT_INVALID &&
        (مدخل->mods & state->components.mods)) {
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
            remap_event = (ssize_t) darray_اندازه(events->صف);
            darray_append(events->صف, (بنية xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = {0},
                    .تغییریافته = 0
                }
            });
        } وإلا {
            /* Merge with last remap event */
            new.components =
                darray_قلم(events->صف, remap_event).components.components;
        }
        new.components.پایه_گروه
            = (int32_t) مدخل->هدف
            - state->components.latched_گروه
            - state->components.locked_گروه;
        xkb_state_update_derived(&new);

        ثابت تعداد xkb_state_component تغییریافته =
            get_state_component_changes(قبلی_components, &new.components);
        darray_قلم(events->صف, remap_event).components.تغییریافته = تغییریافته;
        darray_قلم(events->صف, remap_event).components.components =
            new.components;

        state->components.گروه = new.components.گروه;
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
    darray_foreach_معکوس(event, events->صف) {
        إذا (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS)
            اكسر;
    }
    إذا (!event)
        أرجع;

    /* Restore state */
    ثابت تعداد xkb_state_component تغییریافته =
        get_state_component_changes(initial_components,
                                    &event->components.components);
    إذا (تغییریافته) {
        darray_append(events->صف, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = *initial_components,
                .تغییریافته = تغییریافته
            }
        });
    }
}

ساكن ثابت بنية xkb_کلید *
process_overlayable_کلید(بنية xkb_machine *sm,
                        ثابت بنية xkb_کلید * کلید,
                        تعداد xkb_کلید_direction direction)
{
    /* Check if key is currently overlaid */
    بنية xkb_overlaid_کلید *مدخل = عدم;
    بنية xkb_overlaid_کلید *موجود_مدخل = عدم;
    darray_foreach(مدخل, sm->overlays.کلیدها) {
        إذا (مدخل->old == کلید) {
            /* Maintain the key overlaid */
            بدل (direction) {
            حالة XKB_کلید_پایین:
                مدخل->refcnt++;
                اكسر;
            حالة XKB_کلید_REPEATED:
                اكسر;
            حالة XKB_کلید_به_بالا:
            افتراضي:
                مدخل->refcnt--;
                اكسر;
            }
            إذا (مدخل->refcnt <= 0)
                مدخل->old = عدم;
            أرجع مدخل->new;
        } وإلا إذا (!مدخل->old && !موجود_مدخل) {
            /* First free spot */
            موجود_مدخل = مدخل;
        }
    }
    إذا (direction == XKB_کلید_پایین) {
        /* Enable key overlay */

        /*
         * Get overlay index: select the latest activated overlay
         * containing the key.
         */
        ثابت بنية xkb_کلید *new = کلید;

        إذا (کلید->overlays & sm->overlays.enabled) {
            /* Some relevant overlay is active */
            لكل (uint32_t پشته = sm->overlays.ترتیب; پشته; پشته >>= 4) {
                static_assert(XKB_OVERLAY_شمار == 8, "");
                ثابت xkb_overlay_شاخص_t overlay = (پشته & 0xf) - 1;
                ثابت xkb_overlay_ماسک_t ماسک =
                    (xkb_overlay_ماسک_t)(1u << overlay);
                إذا (کلید->overlays & ماسک) {
                    /* Get new key */
                    إذا (کلید->overlays_inline) {
                        /* Stored inline */
                        new = کلید->overlay_کلید;
                    } وإلا {
                        /* Stored in sparse array */
                        ثابت xkb_overlay_ماسک_t کم = (xkb_overlay_ماسک_t)(
                            کلید->overlays & (xkb_overlay_ماسک_t)(ماسک - 1u)
                        );
                        /* index = number of set bits strictly below bit */
                        ثابت xkb_overlay_شاخص_t شاخص =
                            (xkb_overlay_شاخص_t)popcount32(کم);
                        new = کلید->overlays_کلیدها[شاخص];
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
        إذا (موجود_مدخل) {
            /* Re-use a free entry */
            مدخل = موجود_مدخل;
        } وإلا {
            /* Append a new entry */
            ثابت darray_اندازه_t idx = darray_اندازه(sm->overlays.کلیدها);
            darray_resize(sm->overlays.کلیدها, idx + 1);
            مدخل = &darray_قلم(sm->overlays.کلیدها, idx);
        }

        /* Overlay the key */
        مدخل->old = کلید;
        مدخل->new = new;
        مدخل->refcnt = 1;
        أرجع new;
    }

    /* No overlay */
    أرجع کلید;
}

تعداد xkb_وضعیت
xkb_machine_process_کلید(بنية xkb_machine *sm,
                        xkb_keycode_t kc, تعداد xkb_کلید_direction direction,
                        بنية xkb_events *events)
{
    darray_اندازه(events->صف) = 0;
    events->بعدی = 0;

    بنية xkb_server_state * ثابت state = &sm->پایه;
    ثابت بنية xkb_کلید * کلید = Xkb_کلید(state->پایه.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    إذا (!کلید || (direction == XKB_کلید_REPEATED && !کلید->repeats))
        أرجع XKB_SUCCESS;

    ثابت بنية state_components initial_components = state->پایه.components;

    إذا (direction == XKB_کلید_پایین &&
        (sm->پایه.پایه.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیدها) &&
        (sm->پایه.a11y_flags & XKB_A11_Y_STICKY_کلیدها_NO_همزمان_کلیدها) &&
        state->پایه.components.پایه_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیدها,
                                      0, events);
    }

    إذا (کلید->overlays)
        کلید = process_overlayable_کلید(sm, کلید, direction);

    ssize_t remap_event = do_remap_modifiers(&sm->config.modifiers,
                                             &state->پایه, events, کلید);

    remap_event = do_shortcuts_tweak(&sm->config.shortcuts, &state->پایه,
                                     &initial_components, events, remap_event);

    إذا (remap_event >= 0) {
        // TODO: what about merging state event for RedirectKey?
        darray_append(events->صف, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_قاب,
        });
    }

    state->flags |= STATE_REQUIRE_کلید_EVENT;
    state->مجموعه_mods = 0;
    state->clear_mods = 0;

    /* Handle key actions */
    xkb_filter_apply_all(state, events, کلید, direction);

    xkb_mod_شاخص_t i;
    xkb_mod_ماسک_t بیت;
    لكل (i = 0, بیت = 1; state->مجموعه_mods; i++, بیت <<= 1) {
        إذا (state->مجموعه_mods & بیت) {
            state->mod_کلید_شمار[i]++;
            state->پایه.components.پایه_mods |= بیت;
            state->مجموعه_mods &= ~بیت;
        }
    }

    لكل (i = 0, بیت = 1; state->clear_mods; i++, بیت <<= 1) {
        إذا (state->clear_mods & بیت) {
            state->mod_کلید_شمار[i]--;
            إذا (state->mod_کلید_شمار[i] <= 0) {
                state->پایه.components.پایه_mods &= ~بیت;
                state->mod_کلید_شمار[i] = 0;
            }
            state->clear_mods &= ~بیت;
        }
    }

    xkb_state_update_derived(&state->پایه);

    /*
     * Insert key event only if actions did not override it, e.g.:
     * - RedirectKey() append its own key event;
     * - MovePointer() and PointerButton() generate pointer events instead
     *   of key events;
     * - TerminateServer() generate a Server event instead of key events.
     */
    إذا (state->flags & STATE_REQUIRE_کلید_EVENT) {
        darray_append(events->صف, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_کلید,
            .کلید = {
                .keycode = کلید->keycode,
                .direction = direction
            }
        });
    }

    إذا (remap_event >= 0) {
        // FIXME: fragile if last state change does not restore the state to the remap event
        undo_tweaks(&state->پایه, &initial_components, events);
    }

    ثابت تعداد xkb_state_component تغییریافته = get_state_component_changes(
        &initial_components, &state->پایه.components
    );
    إذا (تغییریافته) {
        إذا (تغییریافته & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        // TODO: merge with previous state event, if any

        darray_append(events->صف, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = state->پایه.components,
                .تغییریافته = تغییریافته
            }
        });
    }

    إذا (!darray_خالی(events->صف) &&
        darray_قلم(events->صف, darray_اندازه(events->صف) - 1).type !=
        XKB_EVENT_TYPE_قاب)
    {
        darray_append(events->صف, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_قاب,
        });
    }

    أرجع XKB_SUCCESS;
}

بنية xkb_events *
xkb_events_new(بنية xkb_context * محدود context,
               ثابت بنية xkb_events_config * محدود config,
               تعداد xkb_وضعیت * محدود وضعیت)
{
    /* Handle default configuration */
    ساكن ثابت بنية xkb_events_config default_config = {
        .اندازه = حجم(default_config)
    };
    إذا (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ثابت تعداد xkb_وضعیت abi_error = xkb_check_state_abi(config);
    إذا (abi_error) {
        xkb_log_abi_error(context, __func__, abi_error);
        إذا (وضعیت)
            *وضعیت = abi_error;
        أرجع عدم;
    }

    /* Sanitize input */
    ثابت uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_EVENTS_FLAGS_مقادیر);
    إذا (invalid_flags) {
        log_err_func(context, XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS_,
                     "unrecognized events collection flags: 0x%"PRIx32"\n",
                     invalid_flags);
        إذا (وضعیت)
            *وضعیت = XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS;
        أرجع عدم;
    }

    بنية xkb_events *events = حافظه_صفر_بده(1, حجم(*events));
    إذا (!events) {
        log_err_func1(context, XKB_ERROR_تخصیص_FAILURE_2,
                      "cannot allocate state events collection\n");
        إذا (وضعیت)
            *وضعیت = XKB_ERROR_تخصیص_FAILURE;
        أرجع عدم;
    }

    /* Pre-allocate event queue */
    darray_init(events->صف);
    ساكن ثابت darray_اندازه_t صف_initial_اندازه = 16;
    darray_resize(events->صف, صف_initial_اندازه);
    // TODO: proper safe darray_resize() with return value
    إذا (!darray_اقلام(events->صف)) {
        log_err_func1(context, XKB_ERROR_تخصیص_FAILURE_2,
                      "cannot allocate state events queue\n");
        xkb_events_destroy(events);
        إذا (وضعیت)
            *وضعیت = XKB_ERROR_تخصیص_FAILURE;
        أرجع عدم;
    }
    darray_اندازه(events->صف) = 0;

    events->ctx = xkb_context_ref(context);
    events->بعدی = 0;

    إذا (وضعیت)
        *وضعیت = XKB_SUCCESS;
    أرجع events;
}

فراغ
xkb_events_destroy(بنية xkb_events *events)
{
    إذا (!events)
        أرجع;
    darray_free(events->صف);
    xkb_context_unref(events->ctx);
    حرر(events);
}

ثابت بنية xkb_event *
xkb_events_بعدی(بنية xkb_events *events)
{
    إذا (events->بعدی < darray_اندازه(events->صف)) {
        ثابت darray_اندازه_t شاخص = events->بعدی++;
        أرجع &darray_قلم(events->صف, شاخص);
    } وإلا {
        أرجع عدم;
    }
}

تعداد xkb_event_type
xkb_event_get_type(ثابت بنية xkb_event *event)
{
    أرجع event->type;
}

تعداد xkb_وضعیت
xkb_event_get_keycode(ثابت بنية xkb_event *event,
                      xkb_keycode_t *keycode,
                      تعداد xkb_کلید_direction *direction)
{
    إذا (event->type == XKB_EVENT_TYPE_کلید) {
        *keycode = event->کلید.keycode;
        *direction = event->کلید.direction;
        أرجع XKB_SUCCESS;
    } وإلا {
        أرجع XKB_ERROR_EVENT_TYPE_MISMATCH;
    }
}

تعداد xkb_وضعیت
xkb_event_get_components(ثابت بنية xkb_event * محدود event,
                         بنية xkb_event_components * محدود components)
{
    إذا (event->type != XKB_EVENT_TYPE_STATE_COMPONENTS)
        أرجع XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ثابت تعداد xkb_وضعیت وضعیت = xkb_check_state_abi(components);
    إذا (وضعیت != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, وضعیت);
        أرجع وضعیت;
    }

    components->تغییریافته = (uint32_t)event->components.تغییریافته;
    components->depressed_mods = event->components.components.پایه_mods;
    components->latched_mods = event->components.components.latched_mods;
    components->locked_mods = event->components.components.locked_mods;
    components->mods = event->components.components.mods;
    components->depressed_layout = event->components.components.پایه_گروه;
    components->latched_layout = event->components.components.latched_گروه;
    components->locked_layout = event->components.components.locked_گروه;
    components->layout = event->components.components.گروه;
    components->leds = event->components.components.leds;
    components->controls = (uint32_t)event->components.components.controls;

    أرجع XKB_SUCCESS;
}

تعداد xkb_وضعیت
xkb_event_get_اشاره_گر_motion(ثابت بنية xkb_event * محدود event,
                             بنية xkb_event_اشاره_گر_motion * محدود motion)
{
    إذا (event->type != XKB_EVENT_TYPE_اشاره_گر_MOTION)
        أرجع XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ثابت تعداد xkb_وضعیت وضعیت = xkb_check_state_abi(motion);
    إذا (وضعیت != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, وضعیت);
        أرجع وضعیت;
    }

    motion->flags = event->اشاره_گر_motion.flags;
    motion->x = event->اشاره_گر_motion.x;
    motion->y = event->اشاره_گر_motion.y;

    أرجع XKB_SUCCESS;
}

تعداد xkb_وضعیت
xkb_event_get_اشاره_گر_button(ثابت بنية xkb_event * محدود event,
                             بنية xkb_event_اشاره_گر_button * محدود button)
{
    إذا (event->type != XKB_EVENT_TYPE_اشاره_گر_BUTTON)
        أرجع XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ثابت تعداد xkb_وضعیت وضعیت = xkb_check_state_abi(button);
    إذا (وضعیت != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, وضعیت);
        أرجع وضعیت;
    }

    button->button = event->اشاره_گر_button.button;
    button->state = event->اشاره_گر_button.state;
    button->شمار = event->اشاره_گر_button.شمار;

    أرجع XKB_SUCCESS;
}

تعداد xkb_وضعیت
xkb_event_get_virtual_console(ثابت بنية xkb_event * محدود event,
                              int8_t * محدود شاخص_or_جابه_جایی,
                              bool * محدود is_جابه_جایی)
{
    إذا (event->type != XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
        أرجع XKB_ERROR_EVENT_TYPE_MISMATCH;

    *شاخص_or_جابه_جایی = event->virtual_console.شاخص_or_جابه_جایی;
    *is_جابه_جایی = event->virtual_console.is_جابه_جایی;

    أرجع XKB_SUCCESS;
}

تعداد xkb_وضعیت
xkb_state_update_event(بنية xkb_state * محدود پایه_state,
                       ثابت بنية xkb_event * محدود event,
                       تعداد xkb_state_component * محدود تغییریافته)
{
    /* Guard against server-only state */
    أكد(پایه_state->نما == SERVER_COMPANION ||
           پایه_state->نما == LEGACY_MIXED_STATE);
    إذا (پایه_state->نما != SERVER_COMPANION &&
        پایه_state->نما != LEGACY_MIXED_STATE) {
        log_err(پایه_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_نما_2,
                "%s: Unexpected state type %d\n", __func__, پایه_state->نما);
        أرجع XKB_ERROR_UNEXPECTED_STATE_نما;
    }

    بنية xkb_client_state * ثابت state =
        (بنية xkb_client_state *)پایه_state;

    إذا (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
        ثابت بنية state_components prev_components = state->پایه.components;
        state->پایه.components = event->components.components;
        /*
         * Recompute the changes instead of using the event value, because we do
         * not know if the event’s queue and the state are synced.
         */
        إذا (تغییریافته)
            *تغییریافته = get_state_component_changes(&prev_components,
                                                   &state->پایه.components);
    } وإلا {
        إذا (تغییریافته)
            *تغییریافته = 0;
    }
    أرجع XKB_SUCCESS;
}

ساكن فراغ
append_اشاره_گر_button(بنية xkb_events * محدود events,
                      بنية xkb_filter * محدود filter,
                      تعداد xkb_اشاره_گر_button_state button_state,
                      uint8_t شمار)
{
    darray_append(events->صف, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_اشاره_گر_BUTTON,
        .اشاره_گر_button = {
            .اندازه = حجم(((بنية xkb_event*)0)->اشاره_گر_button),
            .button = filter->priv,
            .state = button_state,
            .شمار = شمار,
        }
    });
}

ساكن فراغ
xkb_filter_اشاره_گر_button_new(بنية xkb_server_state *state,
                              بنية xkb_events *events,
                              بنية xkb_filter *filter)
{
    إذا (!events || state->پایه.نما != SERVER_STATE ||
        !(state->پایه.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_کلیدها)) {
        filter->func = عدم;
        أرجع;
    }

    بنية xkb_machine * ثابت sm = (بنية xkb_machine *)state;

    /* Resolve button */
    ثابت xkb_اشاره_گر_button_شاخص_t button =
        (filter->action.btn.button == XKB_اشاره_گر_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;
    filter->priv = button;

    ثابت تعداد xkb_اشاره_گر_button_state button_state =
        XKB_اشاره_گر_BUTTON_PRESSED;
    append_اشاره_گر_button(events, filter, button_state,
                          filter->action.btn.شمار);
    state->flags &= ~STATE_REQUIRE_کلید_EVENT;
}

ساكن bool
xkb_filter_اشاره_گر_button_func(بنية xkb_server_state *state,
                               بنية xkb_events *events,
                               بنية xkb_filter *filter,
                               ثابت بنية xkb_کلید *کلید,
                               تعداد xkb_کلید_direction direction)
{
    إذا (کلید != filter->کلید || !events)
        أرجع XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    بدل (direction) {
    حالة XKB_کلید_پایین:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلید_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    إذا (!filter->action.btn.شمار)
        append_اشاره_گر_button(events, filter, XKB_اشاره_گر_BUTTON_RELEASED, 0);

    filter->func = عدم;
    أرجع XKB_FILTER_CONSUME;
}

بنية xkb_filter_اشاره_گر_قفل_button_priv {
    uint32_t button:31;
#اگر_تعریف_شده _WIN32
    uint32_t locked:1; // MSVC requires matching types to pack bitfields
#وإلا
    bool locked:1;
#پایان_اگر
};

تعداد {
    XKB_FILTER_اشاره_گر_قفل_BUTTON_PRIV_عرض =
        حجم(بنية xkb_filter_اشاره_گر_قفل_button_priv) * CHAR_BIT,
    XKB_FILTER_اشاره_گر_قفل_BUTTON_PRIV_MAX =
        (UINT64_C(1) << XKB_FILTER_اشاره_گر_قفل_BUTTON_PRIV_عرض) - 1
};

static_assert(حجم(بنية xkb_filter_اشاره_گر_قفل_button_priv) ==
              حجم(((بنية xkb_filter*)0)->priv),
              "");
static_assert((uint32_t)XKB_اشاره_گر_BUTTON_MAX < (uint32_t)XKB_FILTER_اشاره_گر_قفل_BUTTON_PRIV_MAX,
              "xkb_filter_pointer_lock_button_priv cannot store button");

ساكن فراغ
xkb_filter_اشاره_گر_قفل_button_new(بنية xkb_server_state *state,
                                   بنية xkb_events *events,
                                   بنية xkb_filter *filter)
{
    إذا (!events || state->پایه.نما != SERVER_STATE ||
        !(state->پایه.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_کلیدها)) {
        filter->func = عدم;
        أرجع;
    }

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    بنية xkb_machine * ثابت sm = (بنية xkb_machine *)state;

    /* Resolve button */
    ثابت xkb_اشاره_گر_button_شاخص_t button =
        (filter->action.btn.button == XKB_اشاره_گر_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;

    static_assert(XKB_اشاره_گر_BUTTON_ماسک_عرض <= 8,
                  "invalid UINTx_C() macro");
    static_assert(XKB_اشاره_گر_BUTTON_MAX <= XKB_اشاره_گر_BUTTON_ماسک_عرض,
                  "invalid left shift");
    ثابت xkb_اشاره_گر_button_ماسک_t ماسک = (UINT8_C(1) << button);

    ثابت bool locked = (sm->mouse.locked_buttons & ماسک);

    إذا (!locked && !(filter->action.btn.flags & ACTION_قفل_NO_قفل)) {
        sm->mouse.locked_buttons |= ماسک;
        filter->priv = button;
        append_اشاره_گر_button(events, filter, XKB_اشاره_گر_BUTTON_PRESSED, 0);
    }

    بنية xkb_filter_اشاره_گر_قفل_button_priv *priv =
        (بنية xkb_filter_اشاره_گر_قفل_button_priv*)&filter->priv;
    priv->button = button,
    priv->locked = locked;
}

ساكن bool
xkb_filter_اشاره_گر_قفل_button_func(بنية xkb_server_state *state,
                                    بنية xkb_events *events,
                                    بنية xkb_filter *filter,
                                    ثابت بنية xkb_کلید *کلید,
                                    تعداد xkb_کلید_direction direction)
{
    إذا (کلید != filter->کلید || !events || state->پایه.نما != SERVER_STATE)
        أرجع XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    بدل (direction) {
    حالة XKB_کلید_پایین:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلید_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    ثابت بنية xkb_filter_اشاره_گر_قفل_button_priv *priv =
        (بنية xkb_filter_اشاره_گر_قفل_button_priv*)&filter->priv;

    إذا (priv->locked && !(filter->action.btn.flags & ACTION_قفل_NO_UNLOCK)) {
        بنية xkb_machine * ثابت sm = (بنية xkb_machine *)state;

        static_assert(XKB_اشاره_گر_BUTTON_ماسک_عرض <= 8,
                    "invalid UINTx_C() macro");
        static_assert(XKB_اشاره_گر_BUTTON_MAX <= XKB_اشاره_گر_BUTTON_ماسک_عرض,
                    "invalid left shift");
        ثابت xkb_اشاره_گر_button_ماسک_t ماسک = (UINT8_C(1) << priv->button);

        sm->mouse.locked_buttons &= ~ماسک;
        filter->priv = priv->button;
        append_اشاره_گر_button(events, filter, XKB_اشاره_گر_BUTTON_RELEASED, 0);
    }

    filter->func = عدم;
    أرجع XKB_FILTER_CONSUME;
}

ساكن فراغ
xkb_filter_اشاره_گر_مجموعه_default_button_new(بنية xkb_server_state *state,
                                          بنية xkb_events *events,
                                          بنية xkb_filter *filter)
{
    إذا (!events || state->پایه.نما != SERVER_STATE ||
        !(state->پایه.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_کلیدها)) {
        filter->func = عدم;
        أرجع;
    }

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    بنية xkb_machine * ثابت sm = (بنية xkb_machine *)state;

    static_assert(2 * XKB_اشاره_گر_BUTTON_MAX <= بیشترین_صحیح, "lossy conversion");
    صحيح button = (filter->action.dflt.flags & ACTION_ABSOLUTE_SWITCH)
        ? filter->action.dflt.مقدار
        : sm->mouse.default_button + filter->action.dflt.مقدار;

    /* Wrap into range */
    ثابت صحيح rem = button % XKB_اشاره_گر_BUTTON_MAX;
    sm->mouse.default_button = (rem >= 0)
        ? (xkb_اشاره_گر_button_شاخص_t)rem
        : (xkb_اشاره_گر_button_شاخص_t)(rem + XKB_اشاره_گر_BUTTON_MAX);
}

ساكن bool
xkb_filter_اشاره_گر_مجموعه_default_button_func(بنية xkb_server_state *state,
                                           بنية xkb_events *events,
                                           بنية xkb_filter *filter,
                                           ثابت بنية xkb_کلید *کلید,
                                           تعداد xkb_کلید_direction direction)
{
    إذا (کلید != filter->کلید || !events)
        أرجع XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    بدل (direction) {
    حالة XKB_کلید_پایین:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلید_REPEATED:
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
    إذا (!events || state->پایه.نما != SERVER_STATE ||
        !(((بنية xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = عدم;
        أرجع;
    }

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    darray_append(events->صف, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    });
}

ساكن bool
xkb_filter_terminate_server_func(بنية xkb_server_state *state,
                                 بنية xkb_events *events,
                                 بنية xkb_filter *filter,
                                 ثابت بنية xkb_کلید *کلید,
                                 تعداد xkb_کلید_direction direction)
{
    إذا (کلید != filter->کلید || !events)
        أرجع XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    بدل (direction) {
    حالة XKB_کلید_پایین:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلید_REPEATED:
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
    إذا (!events || state->پایه.نما != SERVER_STATE ||
        !(((بنية xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = عدم;
        أرجع;
    }

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    darray_append(events->صف, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
        .virtual_console = {
            .شاخص_or_جابه_جایی = filter->action.screen.screen,
            .is_جابه_جایی = !(filter->action.screen.flags & ACTION_ABSOLUTE_SWITCH)
        }
    });
}

ساكن bool
xkb_filter_switch_virtual_console_func(بنية xkb_server_state *state,
                                       بنية xkb_events *events,
                                       بنية xkb_filter *filter,
                                       ثابت بنية xkb_کلید *کلید,
                                       تعداد xkb_کلید_direction direction)
{
    إذا (کلید != filter->کلید || !events)
        أرجع XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    بدل (direction) {
    حالة XKB_کلید_پایین:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلید_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    filter->func = عدم;
    أرجع XKB_FILTER_CONSUME;
}
