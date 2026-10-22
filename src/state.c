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
ساختار xkb_events {
    ساختار xkb_context *ctx;
    darray(ساختار xkb_event) queue;
    /**
     * Read cursor for `xkb_events_next()`.
     *
     * Reset to 0 on each `process_*` call.
     */
    darray_size_t next;
};

ساختار xkb_server_state;

ساختار xkb_filter {
    اجتماع xkb_action action;
    ثابت ساختار xkb_کلید *کلید;
    bool (*func)(ساختار xkb_server_state *state,
                 ساختار xkb_events *events,
                 ساختار xkb_filter *filter,
                 ثابت ساختار xkb_کلید *کلید,
                 شمارش xkb_کلید_direction direction);
    uint32_t priv;
    صحیح refcnt;
};

شمارش xkb_state_mode_internal {
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

شمارش { XKB_LAYOUT_INDEX_T_MIN_WIDTH = 6 };
static_assert(XKB_MAX_GROUPS < (1 << XKB_LAYOUT_INDEX_T_MIN_WIDTH),
              "Cannot encode out_of_range_group::redirect_group");

ساختار out_of_range_group {
    شمارش xkb_layout_out_of_range_policy policy :
        (اندازه(شمارش xkb_layout_out_of_range_policy) * CHAR_BIT -
         XKB_LAYOUT_INDEX_T_MIN_WIDTH);
    xkb_layout_index_t redirect_group : XKB_LAYOUT_INDEX_T_MIN_WIDTH;
};

شمارش { XKB_STATE_MODE_INTERNAL_MIN_WIDTH = 4 /* 3 bits + sign */ };
static_assert(XKB_STATE_TYPE_رقم_ENTRIES <
              (1 << XKB_STATE_MODE_INTERNAL_MIN_WIDTH),
              "Cannot encode xkb_state::mode");

/**
 * Common state
 *
 * `mode` indicates whether the object is casted from a `xkb_server_state` or a
 * `xkb_client_state` struct.
 */
ساختار xkb_state {
    ساختار xkb_keymap *keymap;
    /*
     * Before updating the state, we keep a copy of just this struct. This
     * allows us to report which components of the state have changed.
     */
    ساختار state_components components;
    ساختار out_of_range_group out_of_range_group;

    شمارش xkb_state_mode_internal mode : XKB_STATE_MODE_INTERNAL_MIN_WIDTH;
    صحیح refcnt : (اندازه(صحیح) * CHAR_BIT - XKB_STATE_MODE_INTERNAL_MIN_WIDTH);
};

/**
 * Minimal client state
 *
 * Wrapper for strong typing
 */
ساختار xkb_client_state {
    /** Inherits from `xkb_state` */
    ساختار xkb_state base;
};

شمارش state_flags {
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
ساختار xkb_server_state {
    /** Inherits from `xkb_state` */
    ساختار xkb_state base;

    /* Server-specific state data */

    darray(ساختار xkb_filter) filters;

    /**
     * Mix of constant configuration flags and
     * variable state-processing flags.
     */
    شمارش state_flags flags;

    شمارش xkb_a11y_flags a11y_flags;

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
    int16_t mod_کلید_شمار[XKB_MAX_MODS];
};

ایستا ثابت ساختار xkb_کلید_type_entry *
get_entry_for_mods(ثابت ساختار xkb_کلید_type *type, xkb_mod_mask_t mods)
{
    برای (darray_size_t i = 0; i < type->رقم_entries; i++)
        اگر (entry_is_active(&type->entries[i]) &&
            type->entries[i].mods.mask == mods)
            بازگشت &type->entries[i];
    بازگشت پوچ;
}

ایستا ثابت ساختار xkb_کلید_type_entry *
get_entry_for_کلید_state(ساختار xkb_state *state, ثابت ساختار xkb_کلید *کلید,
                        xkb_layout_index_t group)
{
    ثابت ساختار xkb_کلید_type* ثابت type = کلید->groups[group].type;
    xkb_mod_mask_t active_mods = state->components.mods & type->mods.mask;
    بازگشت get_entry_for_mods(type, active_mods);
}

ایستا درونخطی xkb_level_index_t
state_کلید_get_level(ساختار xkb_state *state, ثابت ساختار xkb_کلید *کلید,
                         xkb_layout_index_t layout)
{
    اگر (layout >= کلید->رقم_groups)
        بازگشت XKB_LEVEL_INVALID;

    /* If we don't find an explicit match the default is 0. */
    ثابت ساختار xkb_کلید_type_entry* ثابت entry =
        get_entry_for_کلید_state(state, کلید, layout);

    بازگشت (entry) ? entry->level : 0;
}

/**
 * Returns the level to use for the given key and state, or
 * XKB_LEVEL_INVALID.
 */
xkb_level_index_t
xkb_state_کلید_get_level(ساختار xkb_state *state, xkb_keycode_t kc,
                        xkb_layout_index_t layout)
{
    ثابت ساختار xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);

    بازگشت (کلید)
        ? state_کلید_get_level(state, کلید, layout)
        : XKB_LEVEL_INVALID;
}

ایستا درونخطی xkb_layout_index_t
state_کلید_get_layout(ساختار xkb_state *state, ثابت ساختار xkb_کلید *کلید)
{
    بازگشت xkb_keymap_کلید_effective_layout(کلید, state->components.group);
}

/**
 * Returns the layout to use for the given key and state, taking
 * wrapping/clamping/etc into account, or XKB_LAYOUT_INVALID.
 */
xkb_layout_index_t
xkb_state_کلید_get_layout(ساختار xkb_state *state, xkb_keycode_t kc)
{
    ثابت ساختار xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);

    اگر (!کلید)
        بازگشت XKB_LAYOUT_INVALID;

    بازگشت state_کلید_get_layout(state, کلید);
}

/* Empty action used for empty levels */
ایستا ثابت اجتماع xkb_action dummy_action = { .type = ACTION_TYPE_NONE };

ایستا xkb_action_شمار_t
xkb_کلید_get_actions(ساختار xkb_state *state, ثابت ساختار xkb_کلید *کلید,
                    ثابت اجتماع xkb_action **actions)
{
    ثابت xkb_layout_index_t layout = state_کلید_get_layout(state, کلید);
    ثابت xkb_level_index_t level = state_کلید_get_level(state, کلید, layout);
    اگر (level == XKB_LEVEL_INVALID)
        برو err;

    ثابت xkb_action_شمار_t شمار =
        xkb_keymap_کلید_get_actions_by_level(state->keymap, کلید,
                                            layout, level, actions);
    اگر (!شمار)
        برو err;

    بازگشت شمار;

err:
    /* Use a dummy action if no corresponding level was found or if it is empty.
     * This is required e.g. to handle latches properly. */
    *actions = &dummy_action;
    بازگشت 1;
}

ایستا ساختار xkb_filter *
xkb_filter_new(ساختار xkb_server_state *state)
{
    ساختار xkb_filter *filter = پوچ, *iter;

    darray_foreach(iter, state->filters) {
        اگر (iter->func)
            ادامه;
        /* Use available slot */
        filter = iter;
        بشکن;
    }

    اگر (!filter) {
        /* No available slot: resize the filters array */
        darray_resize0(state->filters, darray_size(state->filters) + 1);
        filter = &darray_item(state->filters, darray_size(state->filters) -1);
    }

    filter->refcnt = 1;
    بازگشت filter;
}

/***====================================================================***/

شمارش xkb_filter_result {
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
    اگر ((filter_)->action.group.flags & ACTION_ABSOLUTE_SWITCH)          \
        (state_)->components.component_ = (filter_)->action.group.group; \
    وگرنه                                                                 \
        (state_)->components.component_ += (filter_)->action.group.group

ایستا تهی
xkb_filter_group_set_new(ساختار xkb_server_state *state,
                         ساختار xkb_events *events,
                         ساختار xkb_filter *filter)
{
    static_assert(اندازه(state->base.components.base_group) ==
                  اندازه(filter->priv),
                  "Max groups don't fit");
    filter->priv = (uint32_t)state->base.components.base_group;
    apply_group_delta(filter, &state->base, base_group);
}

ایستا bool
xkb_filter_group_set_func(ساختار xkb_server_state *state,
                          ساختار xkb_events *events,
                          ساختار xkb_filter *filter,
                          ثابت ساختار xkb_کلید *کلید,
                          شمارش xkb_کلید_direction direction)
{
    اگر (کلید != filter->کلید) {
        filter->action.group.flags &= ~ACTION_LOCK_CLEAR;
        بازگشت XKB_FILTER_CONTINUE;
    }

    انتخاب (direction) {
    حالت XKB_کلید_DOWN:
        filter->refcnt++;
        /* fallthrough */
    حالت XKB_کلید_REPEATED:
        بازگشت XKB_FILTER_CONSUME;
    پیشفرض:
        اگر (--filter->refcnt > 0)
            بازگشت XKB_FILTER_CONSUME;
    }

    static_assert(اندازه(state->base.components.base_group) == اندازه(filter->priv),
                  "Max groups don't fit");
    state->base.components.base_group = (int32_t) filter->priv;

    اگر (filter->action.group.flags & ACTION_LOCK_CLEAR)
        state->base.components.locked_group = 0;

    filter->func = پوچ;
    بازگشت XKB_FILTER_CONTINUE;
}

ایستا شمارش xkb_state_component
get_state_component_changes(ثابت ساختار state_components *a,
                            ثابت ساختار state_components *b)
{
    شمارش xkb_state_component mask = 0;

    اگر (a->group != b->group)
        mask |= XKB_STATE_LAYOUT_EFFECTIVE;
    اگر (a->base_group != b->base_group)
        mask |= XKB_STATE_LAYOUT_DEPRESSED;
    اگر (a->latched_group != b->latched_group)
        mask |= XKB_STATE_LAYOUT_LATCHED;
    اگر (a->locked_group != b->locked_group)
        mask |= XKB_STATE_LAYOUT_LOCKED;
    اگر (a->mods != b->mods)
        mask |= XKB_STATE_MODS_EFFECTIVE;
    اگر (a->base_mods != b->base_mods)
        mask |= XKB_STATE_MODS_DEPRESSED;
    اگر (a->latched_mods != b->latched_mods)
        mask |= XKB_STATE_MODS_LATCHED;
    اگر (a->locked_mods != b->locked_mods)
        mask |= XKB_STATE_MODS_LOCKED;
    اگر (a->leds != b->leds)
        mask |= XKB_STATE_LEDS;
    اگر (a->controls != b->controls)
        mask |= XKB_STATE_CONTROLS_EFFECTIVE;

    بازگشت mask;
}

ایستا تهی
xkb_filter_group_lock_new(ساختار xkb_server_state *state,
                          ساختار xkb_events *events,
                          ساختار xkb_filter *filter)
{
    اگر (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release: do nothing on key press.
         *
         * This is a keymap format v2 extension.
         */
        بازگشت;
    } وگرنه {
        /* Lock on key press */
        apply_group_delta(filter, &state->base, locked_group);
    }
}

ایستا bool
xkb_filter_group_lock_func(ساختار xkb_server_state *state,
                           ساختار xkb_events *events,
                           ساختار xkb_filter *filter,
                           ثابت ساختار xkb_کلید *کلید,
                           شمارش xkb_کلید_direction direction)
{
    اگر (کلید != filter->کلید) {
        اگر ((filter->action.group.flags & ACTION_LOCK_ON_RELEASE) &&
            direction == XKB_کلید_DOWN) {
            /*
             * Another key has been pressed after the locking key:
             * cancel group lock on release.
             *
             * This is a keymap v2 extension.
             */
            filter->action.group.flags &= ~ACTION_LOCK_ON_RELEASE;
        }
        بازگشت XKB_FILTER_CONTINUE;
    }

    انتخاب (direction) {
    حالت XKB_کلید_DOWN:
        filter->refcnt++;
        /* fallthrough */
    حالت XKB_کلید_REPEATED:
        بازگشت XKB_FILTER_CONSUME;
    پیشفرض:
        اگر (--filter->refcnt > 0)
            بازگشت XKB_FILTER_CONSUME;
    }

    اگر (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release
         *
         * This is a keymap v2 extension.
         */
        apply_group_delta(filter, &state->base, locked_group);
    } وگرنه {
        /* Lock on key press: do nothing on key release. */
    }
    filter->func = پوچ;
    بازگشت XKB_FILTER_CONTINUE;
}

ایستا bool
xkb_action_breaks_latch(ثابت اجتماع xkb_action *action,
                        شمارش xkb_action_controls controls,
                        شمارش xkb_internal_action_flags flag,
                        xkb_mod_mask_t mask)
{
    انتخاب (action->type) {
    حالت ACTION_TYPE_NONE:
    حالت ACTION_TYPE_VOID:
    حالت ACTION_TYPE_PTR_BUTTON:
    حالت ACTION_TYPE_PTR_LOCK:
    حالت ACTION_TYPE_CTRL_SET:
    حالت ACTION_TYPE_CTRL_LOCK:
    حالت ACTION_TYPE_SWITCH_VT:
    حالت ACTION_TYPE_TERMINATE:
    حالت ACTION_TYPE_REDIRECT_کلید:
        بازگشت true;
    حالت ACTION_TYPE_PTR_MOVE:
    حالت ACTION_TYPE_PTR_DEFAULT:
        بازگشت !(controls & CONTROL_MOUSE_کلیدها);
    حالت ACTION_TYPE_INTERNAL:
        بازگشت (action->internal.flags & flag) &&
               ((action->internal.clear_latched_mods & mask) == mask);
    پیشفرض:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_رقم_ENTRIES,
                      "Missing action type");
        بازگشت false;
    }
}

شمارش xkb_کلید_latch_state {
    NO_LATCH = 0,
    LATCH_کلید_DOWN,
    LATCH_PENDING,
    کلید_LATCH_STATE_رقم_ENTRIES
};

#define MAX_XKB_کلید_LATCH_STATE_LOG2 2
#اگر (کلید_LATCH_STATE_رقم_ENTRIES > (1 << MAX_XKB_کلید_LATCH_STATE_LOG2)) || \
    (-XKB_MAX_GROUPS) < (INT32_MIN >> MAX_XKB_کلید_LATCH_STATE_LOG2) || \
      XKB_MAX_GROUPS > (INT32_MAX >> MAX_XKB_کلید_LATCH_STATE_LOG2)
#error "Cannot represent priv field of the group latch filter"
#endif

/* Hold the latch state *and* the group delta */
اجتماع group_latch_priv {
    uint32_t priv;
    ساختار {
        /* The type is really: enum xkb_key_latch_state, but it is problematic
         * on Windows, because it is interpreted as signed and leads to wrong
         * negative values. */
        unsigned صحیح latch:MAX_XKB_کلید_LATCH_STATE_LOG2;
        int32_t group_delta:(32 - MAX_XKB_کلید_LATCH_STATE_LOG2);
    };
};

ایستا تهی
xkb_filter_group_latch_new(ساختار xkb_server_state *state,
                           ساختار xkb_events *events,
                           ساختار xkb_filter *filter)
{
    ثابت اجتماع group_latch_priv priv = {
        .latch = LATCH_کلید_DOWN,
        .group_delta = (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)
            ? filter->action.group.group - state->base.components.base_group
            : filter->action.group.group
    };
    filter->priv = priv.priv;
    /* Like group set */
    apply_group_delta(filter, &state->base, base_group);
}

ایستا bool
xkb_filter_group_latch_func(ساختار xkb_server_state *state,
                            ساختار xkb_events *events,
                            ساختار xkb_filter *filter,
                            ثابت ساختار xkb_کلید *کلید,
                            شمارش xkb_کلید_direction direction)
{
    اجتماع group_latch_priv priv = {.priv = filter->priv};
    شمارش xkb_کلید_latch_state latch = priv.latch;

    اگر (direction == XKB_کلید_DOWN) {
        ثابت اجتماع xkb_action *actions = پوچ;
        ثابت xkb_action_شمار_t شمار = xkb_کلید_get_actions(&state->base, کلید,
                                                             &actions);

        اگر (latch == LATCH_کلید_DOWN) {
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
            اگر (state->a11y_flags & XKB_A11_Y_LATCH_SIMULTANEOUS_کلیدها) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                برای (xkb_action_شمار_t k = 0; k < شمار; k++) {
                    اگر (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_GROUP_LATCH,
                                                0))
                    {
                        latch = NO_LATCH;
                        بشکن;
                    }
                }
            } وگرنه {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        وگرنه اگر (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is
             * pressed, or promote it to a lock if it's the same group delta &
             * flags and latchToLock option is enabled. */
            ثابت bool sticky_کلیدها = state->base.components.controls
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
            برای (xkb_action_شمار_t k = 0; k < شمار; k++) {
                اگر (((actions[k].type == ACTION_TYPE_GROUP_LATCH) ||
                     (actions[k].type == ACTION_TYPE_GROUP_SET && sticky_کلیدها)) &&
                     actions[k].group.group == filter->action.group.group &&
                     (actions[k].group.flags & ACTION_ABSOLUTE_SWITCH) ==
                     (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)) {
                    اگر (filter->action.group.flags & ACTION_LATCH_TO_LOCK &&
                        filter->action.group.group != 0)
                    {
                        /* Promote to lock */
                        filter->action.type = ACTION_TYPE_GROUP_LOCK;
                        filter->func = xkb_filter_group_lock_func;
                        xkb_filter_group_lock_new(state, events, filter);
                        state->base.components.latched_group -= priv.group_delta;
                        filter->کلید = کلید;
                        /* XXX beep beep! */
                        بازگشت XKB_FILTER_CONSUME;
                    }
                    /* Do nothing if `latchToLock` option is not activated; if
                     * the latch is not broken by the following actions and the
                     * key is not consumed, then another latch filter will be
                     * created.
                     */
                    ادامه;
                }
                وگرنه اگر (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_GROUP_LATCH,
                                                 0))
                {
                    /* Breaks the latch */
                    state->base.components.latched_group -= priv.group_delta;
                    filter->func = پوچ;
                    بازگشت XKB_FILTER_CONTINUE;
                }
            }
        }
        وگرنه {
            /* Ignore press in NO_LATCH state */
            أكد(latch == NO_LATCH);
        }
    }
    وگرنه اگر (کلید != filter->کلید) {
        /* Ignore repeat/release of other keys */
    }
    وگرنه اگر (direction == XKB_کلید_REPEATED) {
        بازگشت XKB_FILTER_CONSUME; // TODO: check
    }
    وگرنه {
        /* Our key got released.  If we've set it to clear locks, and we
         * currently have a group locked, then release it and
         * don't actually latch.  Else we've actually hit the latching
         * stage, so set PENDING and move our group from base to
         * latched. */
        اگر ((filter->action.group.flags & ACTION_LOCK_CLEAR) &&
             state->base.components.locked_group) {
            اگر (latch == LATCH_PENDING)
                state->base.components.latched_group -= priv.group_delta;
            وگرنه
                state->base.components.base_group -= priv.group_delta;
            state->base.components.locked_group = 0;
            filter->func = پوچ;
        }
        /* Broken latch */
        وگرنه اگر (latch == NO_LATCH) {
            state->base.components.base_group -= priv.group_delta;
            filter->func = پوچ;
        }
        /* We may already have reached the latch state if pressing the
         * key multiple times without latch-to-lock enabled. */
        وگرنه اگر (latch == LATCH_کلید_DOWN) {
            latch = LATCH_PENDING;
            /* Switch from set to latch */
            state->base.components.base_group -= priv.group_delta;
            state->base.components.latched_group += priv.group_delta;
            /* XXX beep beep! */
        }
    }

    priv.latch = latch;
    filter->priv = priv.priv;

    بازگشت XKB_FILTER_CONTINUE;
}

ایستا تهی
xkb_filter_mod_set_new(ساختار xkb_server_state *state,
                       ساختار xkb_events *events,
                       ساختار xkb_filter *filter)
{
    ثابت شمارش xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    اگر ((filter->action.mods.flags & unlock) == unlock) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension.
         */
        filter->priv = filter->action.mods.mods.mask
                     & ~state->base.components.locked_mods;
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
    } وگرنه {
        filter->priv = filter->action.mods.mods.mask;
    }

    state->set_mods |= (xkb_mod_mask_t) filter->priv;
}

ایستا bool
xkb_filter_mod_set_func(ساختار xkb_server_state *state,
                        ساختار xkb_events *events,
                        ساختار xkb_filter *filter,
                        ثابت ساختار xkb_کلید *کلید,
                        شمارش xkb_کلید_direction direction)
{

    اگر (کلید != filter->کلید) {
        filter->action.mods.flags &= ~ACTION_LOCK_CLEAR;
        بازگشت XKB_FILTER_CONTINUE;
    }

    انتخاب (direction) {
    حالت XKB_کلید_DOWN:
        filter->refcnt++;
        /* fallthrough */
    حالت XKB_کلید_REPEATED:
        بازگشت XKB_FILTER_CONSUME;
    پیشفرض:
        اگر (--filter->refcnt > 0)
            بازگشت XKB_FILTER_CONSUME;
    }

    state->clear_mods |= (xkb_mod_mask_t) filter->priv;
    ثابت شمارش xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    اگر ((filter->action.mods.flags & unlock) == ACTION_LOCK_CLEAR)
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;

    filter->func = پوچ;
    بازگشت XKB_FILTER_CONTINUE;
}

ایستا تهی
xkb_filter_mod_lock_new(ساختار xkb_server_state *state,
                        ساختار xkb_events *events,
                        ساختار xkb_filter *filter)
{
    filter->priv = (state->base.components.locked_mods &
                    filter->action.mods.mods.mask);

    اگر (filter->priv && (filter->action.mods.flags & ACTION_UNLOCK_ON_PRESS)) {
        /*
         * Some of the target modifiers were locked before key press: unlock.
         *
         * This is a keymap v2 extension: unlock-on-press.
         */
        اگر (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
            state->base.components.locked_mods &= ~filter->priv;
        /* No further action: cancel filter */
        filter->func = پوچ;
    } وگرنه {
        /* Set base mods; lock mods if relevant (XKB 1.0 spec) */
        state->set_mods |= filter->action.mods.mods.mask;
        اگر (!(filter->action.mods.flags & ACTION_LOCK_NO_LOCK))
            state->base.components.locked_mods |= filter->action.mods.mods.mask;
    }
}

ایستا bool
xkb_filter_mod_lock_func(ساختار xkb_server_state *state,
                         ساختار xkb_events *events,
                         ساختار xkb_filter *filter,
                         ثابت ساختار xkb_کلید *کلید,
                         شمارش xkb_کلید_direction direction)
{
    اگر (کلید != filter->کلید)
        بازگشت XKB_FILTER_CONTINUE;

    انتخاب (direction) {
    حالت XKB_کلید_DOWN:
        filter->refcnt++;
        /* fallthrough */
    حالت XKB_کلید_REPEATED:
        بازگشت XKB_FILTER_CONSUME;
    پیشفرض:
        اگر (--filter->refcnt > 0)
            بازگشت XKB_FILTER_CONSUME;
    }

    state->clear_mods |= filter->action.mods.mods.mask;
    اگر (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
        state->base.components.locked_mods &= ~filter->priv;

    filter->func = پوچ;
    بازگشت XKB_FILTER_CONTINUE;
}

ایستا تهی
xkb_filter_mod_latch_new(ساختار xkb_server_state *state,
                         ساختار xkb_events *events,
                         ساختار xkb_filter *filter)
{
    /* Latch-on-press + clear-locks imply unlock-on-press */
    ثابت شمارش xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                              | ACTION_LATCH_ON_PRESS;

    اگر ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
        (filter->action.mods.flags & unlockOnPress) &&
        (state->base.components.locked_mods & filter->action.mods.mods.mask) ==
         filter->action.mods.mods.mask) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension: clear locks and do not latch.
         */
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
        filter->func = پوچ;
    } وگرنه اگر (filter->action.mods.flags & ACTION_LATCH_ON_PRESS) {
        /*
         * Latch on key press
         *
         * This is a keymap format v2 extension.
         */
        filter->priv = LATCH_PENDING;
        state->base.components.latched_mods |= filter->action.mods.mods.mask;
        /* XXX beep beep! */
    } وگرنه {
        /* XKB standard latch action */
        filter->priv = LATCH_کلید_DOWN;
        state->set_mods |= filter->action.mods.mods.mask;
    }
}

ایستا bool
xkb_filter_mod_latch_func(ساختار xkb_server_state *state,
                          ساختار xkb_events *events,
                          ساختار xkb_filter *filter,
                          ثابت ساختار xkb_کلید *کلید,
                          شمارش xkb_کلید_direction direction)
{
    شمارش xkb_کلید_latch_state latch = filter->priv;

    اگر (direction == XKB_کلید_DOWN) {
        ثابت اجتماع xkb_action *actions = پوچ;
        ثابت xkb_action_شمار_t شمار = xkb_کلید_get_actions(&state->base, کلید,
                                                             &actions);

        اگر (latch == LATCH_کلید_DOWN) {
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
            اگر (state->a11y_flags & XKB_A11_Y_LATCH_SIMULTANEOUS_کلیدها) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                برای (xkb_action_شمار_t k = 0; k < شمار; k++) {
                    اگر (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_MOD_LATCH,
                                                filter->action.mods.mods.mask))
                    {
                        latch = NO_LATCH;
                        بشکن;
                    }
                }
            } وگرنه {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        وگرنه اگر (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is pressed,
             * or promote it to a lock or plain base set if it's the same
             * modifier. */
            ثابت bool sticky_کلیدها = state->base.components.controls
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
            برای (xkb_action_شمار_t k = 0; k < شمار; k++) {
                اگر (((actions[k].type == ACTION_TYPE_MOD_LATCH) ||
                     (actions[k].type == ACTION_TYPE_MOD_SET && sticky_کلیدها)) &&
                    actions[k].mods.mods.mask == filter->action.mods.mods.mask)
                {
                    اگر (filter->action.mods.flags & ACTION_LATCH_TO_LOCK) {
                        /* Mutate the action to LockMods() */
                        filter->action.type = ACTION_TYPE_MOD_LOCK;
                        filter->func = xkb_filter_mod_lock_func;
                        xkb_filter_mod_lock_new(state, events, filter);
                    }
                    وگرنه {
                        /* Mutate the action to SetMods() */
                        filter->action.type = ACTION_TYPE_MOD_SET;
                        filter->func = xkb_filter_mod_set_func;
                        xkb_filter_mod_set_new(state, events, filter);
                    }
                    filter->کلید = کلید;
                    /* Clear latches */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    /* XXX beep beep! */
                    بازگشت XKB_FILTER_CONSUME;
                }
                وگرنه اگر (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_MOD_LATCH,
                                                 filter->action.mods.mods.mask))
                {
                    /* XXX: This may be totally broken, we might need to break the
                     *      latch in the next run after this press? */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    filter->func = پوچ;
                    بازگشت XKB_FILTER_CONTINUE;
                }
            }
        }
        وگرنه {
            /* Ignore press in NO_LATCH state */
            أكد(latch == NO_LATCH);
        }
    }
    وگرنه اگر (کلید != filter->کلید) {
        /* Ignore repeat/release of other keys */
    }
    وگرنه اگر (direction == XKB_کلید_REPEATED) {
        بازگشت XKB_FILTER_CONSUME; // TODO: check
    }
    وگرنه {
        /* Our key got released.  If we’ve set it to clear locks, and we
         * currently have the same modifiers locked, then release them and
         * don't actually latch.  Else we’ve actually hit the latching
         * stage, so set PENDING and move our modifier from base to
         * latched. */

        /* Latch-on-press + clear-locks imply unlock-on-press */
        ثابت شمارش xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                                  | ACTION_LATCH_ON_PRESS;

        اگر ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
            !(filter->action.mods.flags & unlockOnPress) &&
            (state->base.components.locked_mods &
             filter->action.mods.mods.mask) == filter->action.mods.mods.mask)
        {
            /* XXX: We might be a bit overenthusiastic about clearing
             *      mods other filters have set here? */
            اگر (latch == LATCH_PENDING)
                state->base.components.latched_mods &=
                    ~filter->action.mods.mods.mask;
            وگرنه
                state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
            filter->func = پوچ;
        }
        وگرنه اگر (latch == NO_LATCH) {
            /* Broken latch */
            state->clear_mods |= filter->action.mods.mods.mask;
            filter->func = پوچ;
        }
        وگرنه اگر (!(filter->action.mods.flags & ACTION_LATCH_ON_PRESS)) {
            latch = LATCH_PENDING;
            state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.latched_mods |= filter->action.mods.mods.mask;
            /* XXX beep beep! */
        }
    }

    filter->priv = latch;

    بازگشت XKB_FILTER_CONTINUE;
}

ایستا تهی
append_pointer_move(ساختار xkb_events *events,
                    ساختار xkb_filter *filter)
{
    شمارش xkb_pointer_motion_flags flags = 0;
    اگر (filter->action.ptr.flags & ACTION_ABSOLUTE_X)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_X;
    اگر (filter->action.ptr.flags & ACTION_ABSOLUTE_Y)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_Y;
    اگر (filter->action.ptr.flags & ACTION_REPEAT)
        flags |= XKB_POINTER_MOTION_REPEATS;
    darray_append(events->queue, (ساختار xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_MOTION,
        .pointer_motion = {
            .size = اندازه(((ساختار xkb_event*)0)->pointer_motion),
            .flags = flags,
            .x = filter->action.ptr.x,
            .y = filter->action.ptr.y,
        }
    });
}

ایستا تهی
xkb_filter_pointer_move_new(ساختار xkb_server_state *state,
                            ساختار xkb_events *events,
                            ساختار xkb_filter *filter)
{
    اگر (!events) {
        filter->func = پوچ;
        بازگشت;
    }

    أكد(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_کلیدها);
    append_pointer_move(events, filter);
    state->flags &= ~STATE_REQUIRE_کلید_EVENT;
}

ایستا bool
xkb_filter_pointer_move_func(ساختار xkb_server_state *state,
                             ساختار xkb_events *events,
                             ساختار xkb_filter *filter,
                             ثابت ساختار xkb_کلید *کلید,
                             شمارش xkb_کلید_direction direction)
{
    اگر (کلید != filter->کلید || !events)
        بازگشت XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    انتخاب (direction) {
    حالت XKB_کلید_DOWN:
        filter->refcnt++;
        append_pointer_move(events, filter);
        بازگشت XKB_FILTER_CONSUME;
    حالت XKB_کلید_REPEATED:
        اگر (filter->action.ptr.flags & ACTION_REPEAT)
            append_pointer_move(events, filter);
        بازگشت XKB_FILTER_CONSUME;
    پیشفرض:
        اگر (--filter->refcnt > 0)
            بازگشت XKB_FILTER_CONSUME;
    }

    filter->func = پوچ;
    بازگشت XKB_FILTER_CONSUME;
}

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ایستا تهی
xkb_filter_pointer_button_new(ساختار xkb_server_state *state,
                              ساختار xkb_events *events,
                              ساختار xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ایستا bool
xkb_filter_pointer_button_func(ساختار xkb_server_state *state,
                               ساختار xkb_events *events,
                               ساختار xkb_filter *filter,
                               ثابت ساختار xkb_کلید *کلید,
                               شمارش xkb_کلید_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ایستا تهی
xkb_filter_pointer_lock_button_new(ساختار xkb_server_state *state,
                                   ساختار xkb_events *events,
                                   ساختار xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ایستا bool
xkb_filter_pointer_lock_button_func(ساختار xkb_server_state *state,
                                    ساختار xkb_events *events,
                                    ساختار xkb_filter *filter,
                                    ثابت ساختار xkb_کلید *کلید,
                                    شمارش xkb_کلید_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ایستا تهی
xkb_filter_pointer_set_default_button_new(ساختار xkb_server_state *state,
                                          ساختار xkb_events *events,
                                          ساختار xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ایستا bool
xkb_filter_pointer_set_default_button_func(ساختار xkb_server_state *state,
                                           ساختار xkb_events *events,
                                           ساختار xkb_filter *filter,
                                           ثابت ساختار xkb_کلید *کلید,
                                           شمارش xkb_کلید_direction direction);

ایستا تهی
xkb_filter_terminate_server_new(ساختار xkb_server_state *state,
                                 ساختار xkb_events *events,
                                 ساختار xkb_filter *filter);

ایستا bool
xkb_filter_terminate_server_func(ساختار xkb_server_state *state,
                                  ساختار xkb_events *events,
                                  ساختار xkb_filter *filter,
                                  ثابت ساختار xkb_کلید *کلید,
                                  شمارش xkb_کلید_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ایستا تهی
xkb_filter_switch_virtual_console_new(ساختار xkb_server_state *state,
                                      ساختار xkb_events *events,
                                      ساختار xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ایستا bool
xkb_filter_switch_virtual_console_func(ساختار xkb_server_state *state,
                                       ساختار xkb_events *events,
                                       ساختار xkb_filter *filter,
                                       ثابت ساختار xkb_کلید *کلید,
                                       شمارش xkb_کلید_direction direction);

ایستا درونخطی تهی
clear_all_latches_and_locks(ساختار xkb_server_state *state,
                            ساختار xkb_events *events);

ایستا تهی
xkb_filter_ctrls_new(ساختار xkb_server_state *state,
                     ساختار xkb_events *events,
                     ساختار xkb_filter *filter)
{
    اگر (filter->action.type == ACTION_TYPE_CTRL_SET) {
        /* Set: save the specified controls that are *not* already enabled */
        filter->priv = (uint32_t) (
            (~state->base.components.controls) & filter->action.ctrls.ctrls
        );
    } وگرنه {
        /* Lock: save the specified controls that *are* already enabled */
        filter->priv = (uint32_t) (
            state->base.components.controls & filter->action.ctrls.ctrls
        );
    }

    اگر (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_LOCK)) {
        /* Enable the specified controls that are not already enabled */
        state->base.components.controls |= filter->action.ctrls.ctrls;
    }
}

ایستا bool
xkb_filter_ctrls_func(ساختار xkb_server_state *state,
                      ساختار xkb_events *events,
                      ساختار xkb_filter *filter,
                      ثابت ساختار xkb_کلید *کلید,
                      شمارش xkb_کلید_direction direction)
{
    اگر (کلید != filter->کلید)
        بازگشت XKB_FILTER_CONTINUE;

    انتخاب (direction) {
    حالت XKB_کلید_DOWN:
        filter->refcnt++;
        /* fallthrough */
    حالت XKB_کلید_REPEATED:
        بازگشت XKB_FILTER_CONSUME;
    پیشفرض:
        اگر (--filter->refcnt > 0)
            بازگشت XKB_FILTER_CONSUME;
    }

    اگر (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_UNLOCK)) {
        ثابت شمارش xkb_action_controls old = state->base.components.controls;

        /*
         * Set: Disable specified controls that were *not* enabled at key press.
         * Lock: Disable specified controls that *were* enabled at key press.
         */
        state->base.components.controls &= ~(شمارش xkb_action_controls) filter->priv;

        اگر ((old & CONTROL_STICKY_کلیدها) &&
            !(state->base.components.controls & CONTROL_STICKY_کلیدها)) {
            /* Sticky keys were disabled: clear all locks and latches */
            clear_all_latches_and_locks(state, events);
        }
    }

    filter->func = پوچ;
    بازگشت XKB_FILTER_CONTINUE;
}

ایستا bool
append_redirect_کلید_events(ساختار xkb_server_state *state,
                           ساختار xkb_events *events,
                           ثابت ساختار xkb_redirect_کلید_action *redirect,
                           شمارش xkb_کلید_direction direction)
{
    شمارش xkb_state_component changed = 0;
    ثابت xkb_mod_mask_t mask = redirect->affect_mods;

    /*
     * Reference state: find the last state update in the queue, otherwise
     * use the current state.
     */
    ساختار xkb_event *event;
    ساختار state_components last_components = state->base.components;
    darray_foreach_reverse(event, events->queue) {
        اگر (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
            last_components = event->components.components;
            بشکن;
        }
    }

    اگر (mask) {
        ساختار state_components new = last_components;
        new.base_mods = (new.base_mods & ~mask) | redirect->mods;
        new.latched_mods = (new.latched_mods & ~mask) | redirect->mods;
        new.locked_mods = (new.locked_mods & ~mask) | redirect->mods;
        new.mods = (new.mods & ~mask) | redirect->mods;

        changed = get_state_component_changes(&last_components, &new);
        اگر (changed) {
            darray_append(events->queue, (ساختار xkb_event) {
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
            darray_append(events->queue, (ساختار xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_FRAME,
            });
        }
    }

    darray_append(events->queue, (ساختار xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_کلید,
        .کلید = {
            .keycode = redirect->keycode,
            .direction = direction
        }
    });

    اگر (mask && changed) {
        /* Restore state */
        darray_append(events->queue, (ساختار xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = last_components,
                .changed = changed
            }
        });
    }

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;
    بازگشت true;
}

ایستا تهی
xkb_filter_redirect_کلید_new(ساختار xkb_server_state *state,
                            ساختار xkb_events *events,
                            ساختار xkb_filter *filter)
{
    /* Action effectual only with the xkb_machine API and a valid keycode */
    اگر (!events || filter->action.redirect.keycode == XKB_KEYCODE_INVALID) {
        filter->func = پوچ;
        بازگشت;
    }
    append_redirect_کلید_events(state, events, &filter->action.redirect,
                               XKB_کلید_DOWN);
}

ایستا bool
xkb_filter_redirect_کلید_func(ساختار xkb_server_state *state,
                             ساختار xkb_events *events,
                             ساختار xkb_filter *filter,
                             ثابت ساختار xkb_کلید *کلید,
                             شمارش xkb_کلید_direction direction)
{
    اگر (کلید != filter->کلید)
        بازگشت XKB_FILTER_CONTINUE;

    اگر (direction == XKB_کلید_UP) {
        append_redirect_کلید_events(state, events,
                                   &filter->action.redirect, XKB_کلید_UP);
        filter->func = پوچ;
        بازگشت XKB_FILTER_CONSUME;
     } وگرنه اگر (direction == XKB_کلید_DOWN) {
        ثابت اجتماع xkb_action *actions = پوچ;
        ثابت xkb_action_شمار_t شمار =
            xkb_کلید_get_actions(&state->base, کلید, &actions);

        برای (xkb_action_شمار_t a = 0; a < شمار; a++) {
            اگر (actions[a].type == ACTION_TYPE_REDIRECT_کلید &&
                actions[a].redirect.keycode != filter->action.redirect.keycode) {
                    /* One action redirects to a different key: release first */
                    append_redirect_کلید_events(state, events,
                                               &filter->action.redirect,
                                               XKB_کلید_UP);
                filter->func = پوچ;
                بازگشت XKB_FILTER_CONTINUE;
            }
        }
    }

    append_redirect_کلید_events(state, events, &filter->action.redirect,
                               direction);
    بازگشت XKB_FILTER_CONSUME;
}

ایستا ثابت ساختار {
    تهی (*new)(ساختار xkb_server_state *state,
                ساختار xkb_events *events,
                ساختار xkb_filter *filter);
    bool (*func)(ساختار xkb_server_state *state,
                 ساختار xkb_events *events,
                 ساختار xkb_filter *filter,
                 ثابت ساختار xkb_کلید *کلید, شمارش xkb_کلید_direction direction);
} filter_action_funcs[ACTION_TYPE_رقم_ENTRIES] = {
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
    [ACTION_TYPE_REDIRECT_کلید] = { xkb_filter_redirect_کلید_new,
                                   xkb_filter_redirect_کلید_func },
};

/**
 * Applies any relevant filters to the key, first from the list of filters
 * that are currently active, then if no filter has claimed the key, possibly
 * apply a new filter from the key action.
 */
ایستا تهی
xkb_filter_apply_all(ساختار xkb_server_state *state,
                     ساختار xkb_events *events,
                     ثابت ساختار xkb_کلید *کلید,
                     شمارش xkb_کلید_direction direction)
{
    /* First run through all the currently active filters and see if any of
     * them have consumed this event. */
    bool consumed = false;
    ساختار xkb_filter *filter;
    darray_foreach(filter, state->filters) {
        اگر (!filter->func)
            ادامه;

        اگر (filter->func(state, events, filter, کلید, direction) == XKB_FILTER_CONSUME)
            consumed = true;
    }
    اگر (consumed || direction == XKB_کلید_UP)
        بازگشت;

    /* No filter consumed this event, so proceed with the key actions */
    ثابت اجتماع xkb_action *actions = پوچ;
    xkb_action_شمار_t شمار = xkb_کلید_get_actions(&state->base, کلید,
                                                         &actions);

    برای (ساختار {
            xkb_action_شمار_t شمار;
            ثابت اجتماع xkb_action *actions;
         } next = {0};
         شمار;
         شمار = next.شمار, next.شمار = 0, actions = next.actions
    ) {
        /*
         * Process actions sequentially.
         *
         * NOTE: We rely on the parser to disallow multiple modifier or group
         * actions (see `CheckMultipleActionsCategories`). Allowing multiple
         * such actions requires a refactor of the state handling.
         */
        برای (xkb_action_شمار_t k = 0; k < شمار; k++) {
            /*
             * It's possible for the keymap to set action->type explicitly, like
             * so:
             *     interpret XF86_Next_VMode {
             *         action = Private(type=0x86, data="+VMode");
             *     };
             * We don't handle those.
             */
            اگر (actions[k].type >= ACTION_TYPE_رقم_ENTRIES)
                ادامه;

            /* Go to next action if no corresponding action handler */
            اگر (!filter_action_funcs[actions[k].type].new)
                ادامه;

            /* Add a new filter and run the corresponding initial action */
            filter = xkb_filter_new(state);
            filter->کلید = کلید;
            filter->action = actions[k];

            انتخاب (filter->action.type) {
            حالت ACTION_TYPE_MOD_SET:
                اگر (state->base.components.controls & CONTROL_STICKY_کلیدها) {
                    /* Convert modifier set action to a latch */
                    filter->action.type = ACTION_TYPE_MOD_LATCH;
                    اگر (state->a11y_flags & XKB_A11_Y_STICKY_کلیدها_LATCH_TO_LOCK) {
                        filter->action.mods.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                /* fallthrough */
            حالت ACTION_TYPE_MOD_LATCH:
            حالت ACTION_TYPE_MOD_LOCK: {
                ایستا ثابت شمارش xkb_action_flags unlock_mods_on_press
                    = ACTION_LOCK_CLEAR     /* SetMods & LatchMods: can unlock */
                    | ACTION_LOCK_NO_UNLOCK /* LockMods: cannot unlock */
                    | ACTION_EXPLICIT_UNLOCK_ON_PRESS;
                اگر ((state->flags & STATE_UNLOCK_MODS_ON_PRESS) &&
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
                بشکن;
            }
            حالت ACTION_TYPE_GROUP_SET:
                اگر (state->base.components.controls & CONTROL_STICKY_کلیدها) {
                    /* Convert group set action to a latch */
                    filter->action.type = ACTION_TYPE_GROUP_LATCH;
                    اگر (state->a11y_flags & XKB_A11_Y_STICKY_کلیدها_LATCH_TO_LOCK) {
                        filter->action.group.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                بشکن;
            حالت ACTION_TYPE_GROUP_LOCK:
                اگر ((filter->action.group.flags &
                     ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE) &&
                    (state->flags &
                     STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_کلید) &&
                    /*
                     * FIXME: sticky keys may leave an unexpected modifier
                     * from the level 0 latched, so disable for now.
                     */
                    !(state->base.components.controls & CONTROL_STICKY_کلیدها))
                {
                    filter->action.group.flags |= ACTION_LOCK_ON_RELEASE;
                    /* Add base level action */
                    next.شمار = xkb_keymap_کلید_get_actions_by_level(
                        state->base.keymap, کلید, state->base.components.group,
                        0, &next.actions
                    );
                    أكد(next.شمار == 1);
                }
                بشکن;
            حالت ACTION_TYPE_PTR_MOVE:
            حالت ACTION_TYPE_PTR_BUTTON:
            حالت ACTION_TYPE_PTR_LOCK:
            حالت ACTION_TYPE_PTR_DEFAULT:
                اگر (!(state->base.components.controls & CONTROL_MOUSE_کلیدها)) {
                    /* Convert pointer actions to NoAction */
                    أكد(!filter_action_funcs[ACTION_TYPE_NONE].new);
                    filter->func = پوچ;
                    ادامه;
                }
                بشکن;
            حالت ACTION_TYPE_REDIRECT_کلید:
                // FIXME: this is not efficient to resolve mods here each time
                filter->action.redirect.affect_mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.affect_mods
                );
                filter->action.redirect.mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.mods
                );
                بشکن;
            پیشفرض: ;
            }
            filter->func = filter_action_funcs[filter->action.type].func;
            filter_action_funcs[filter->action.type].new(state, events, filter);
        }
    }
}

ایستا تهی
xkb_state_update_derived(ساختار xkb_state *state);

ایستا درونخطی تهی
xkb_state_init(ساختار xkb_state * restrict state,
               ساختار xkb_keymap * restrict keymap,
               شمارش xkb_state_mode_internal mode)
{
    state->mode = mode;
    state->refcnt = 1;
    state->keymap = xkb_keymap_ref(keymap);
    state->out_of_range_group.policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

    /* Ensure that derived state is correctly initialized */
    xkb_state_update_derived(state);
}

ایستا درونخطی تهی
xkb_client_state_init(ساختار xkb_client_state * restrict state,
                      ساختار xkb_keymap * restrict keymap,
                      شمارش xkb_state_mode_internal mode)
{
    xkb_state_init(&state->base, keymap, mode);
}

ایستا درونخطی تهی
xkb_server_state_init(ساختار xkb_server_state * restrict state,
                      ساختار xkb_keymap * restrict keymap,
                      شمارش xkb_state_mode_internal mode,
                      شمارش xkb_a11y_flags a11y_affect,
                      شمارش xkb_a11y_flags a11y_flags)
{
    xkb_state_init(&state->base, keymap, mode);

    state->a11y_flags = a11y_flags;
    اگر (keymap->format != XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !(a11y_affect & XKB_A11_Y_LATCH_SIMULTANEOUS_کلیدها)) {
             /* Keymap v2+: enable extension to XKB if not manually disabled */
             state->a11y_flags |= XKB_A11_Y_LATCH_SIMULTANEOUS_کلیدها;
    }
}

ساختار xkb_state *
xkb_state_new(ساختار xkb_keymap *keymap)
{
    ساختار xkb_server_state * ثابت state = calloc(1, اندازه(*state));
    اگر (!state) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "Could not allocate a state object.\n");
        بازگشت پوچ;
    }

    xkb_server_state_init(state, keymap, LEGACY_MIXED_STATE, 0, 0);

    بازگشت (ساختار xkb_state *)state;
}

ساختار xkb_state *
xkb_state_new_with_mode(ساختار xkb_keymap * restrict keymap,
                        شمارش xkb_state_mode mode,
                        شمارش xkb_status * restrict status)
{
    انتخاب (mode) {
    حالت XKB_STATE_MODE_CLIENT:
    حالت XKB_STATE_MODE_SERVER_QUERY: {
        ساختار xkb_client_state * ثابت state = calloc(1, اندازه(*state));
        اگر (!state)
            بشکن;

        static_assert((unsigned)XKB_STATE_MODE_CLIENT ==
                      (unsigned)CLIENT_STATE, "");
        static_assert((unsigned)XKB_STATE_MODE_SERVER_QUERY ==
                      (unsigned)SERVER_COMPANION, "");
        xkb_client_state_init(state, keymap, (شمارش xkb_state_mode_internal)mode);

        اگر (status)
            *status = XKB_SUCCESS;

        بازگشت (ساختار xkb_state *)state;
    }
    حالت XKB_STATE_MODE_SERVER: {
        ساختار xkb_server_state * ثابت state = calloc(1, اندازه(*state));
        اگر (!state)
            بشکن;

        xkb_server_state_init(state, keymap, LEGACY_SERVER_STATE, 0, 0);

        اگر (status)
            *status = XKB_SUCCESS;

        بازگشت (ساختار xkb_state *)state;
    }
    پیشفرض:
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_STATE_MODE_,
                     "Unsupported state mode: %u\n", mode);
        اگر (status)
            *status = XKB_ERROR_UNSUPPORTED_STATE_MODE;
        بازگشت پوچ;
    }

    log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                  "Could not allocate a state object.\n");
    اگر (status)
        *status = XKB_ERROR_ALLOCATION_FAILURE;
    بازگشت پوچ;
}

ساختار xkb_state *
xkb_state_ref(ساختار xkb_state *state)
{
    أكد(state->refcnt > 0);
    state->refcnt++;
    بازگشت state;
}

تهی
xkb_state_reset(ساختار xkb_state *state)
{
    memset(&state->components, 0, اندازه(state->components));

    اگر (state->mode < LEGACY_MIXED_STATE) {
        static_assert(اندازه(*state) == اندازه(ساختار xkb_client_state),
                      "Not a mere wrapper");
        /* Nothing else to reset */
    } وگرنه {
        ساختار xkb_server_state *state_ = (ساختار xkb_server_state *)state;
        ثابت ساختار xkb_server_state prev = *state_;
        memset(state_, 0, اندازه(*state_));

        state_->base = prev.base;
        state_->a11y_flags = prev.a11y_flags;
        state_->filters = prev.filters;
        darray_size(state_->filters) = 0;
    }
}

ایستا درونخطی تهی
xkb_state_destroy(ساختار xkb_state *state)
{
    xkb_keymap_unref(state->keymap);
    اگر (state->mode > SERVER_COMPANION)
        darray_free(((ساختار xkb_server_state *)state)->filters);
}

تهی
xkb_state_unref(ساختار xkb_state *state)
{
    أكد(!state || state->refcnt > 0);
    اگر (!state || --state->refcnt > 0)
        بازگشت;

    xkb_state_destroy(state);

    انتخاب (state->mode) {
    حالت CLIENT_STATE:
    حالت SERVER_COMPANION:
        آزاد((ساختار xkb_client_state *)state);
        بشکن;
    حالت LEGACY_MIXED_STATE:
    حالت LEGACY_SERVER_STATE:
        آزاد((ساختار xkb_server_state *)state);
        بشکن;
    حالت SERVER_STATE: /* handled by xkb_machine_unref() */
    پیشفرض:
        PANIC_UNREACHABLE();
    }
}

ساختار xkb_keymap *
xkb_state_get_keymap(ساختار xkb_state *state)
{
    بازگشت state->keymap;
}

/**
 * Update the LED state to match the rest of the xkb_state.
 */
ایستا تهی
xkb_state_led_update_all(ساختار xkb_state *state)
{
    xkb_led_index_t idx;
    ثابت ساختار xkb_led *led;

    state->components.leds = 0;

    xkb_leds_enumerate(idx, led, state->keymap) {

        اگر (led->which_mods != 0 && led->mods.mask != 0) {
            xkb_mod_mask_t mod_mask = 0;
            اگر (led->which_mods & XKB_STATE_MODS_EFFECTIVE)
                mod_mask |= state->components.mods;
            اگر (led->which_mods & XKB_STATE_MODS_DEPRESSED)
                mod_mask |= state->components.base_mods;
            اگر (led->which_mods & XKB_STATE_MODS_LATCHED)
                mod_mask |= state->components.latched_mods;
            اگر (led->which_mods & XKB_STATE_MODS_LOCKED)
                mod_mask |= state->components.locked_mods;

            اگر (led->mods.mask & mod_mask) {
                state->components.leds |= (UINT32_C(1) << idx);
                ادامه;
            }
        }

        اگر (led->which_groups != 0) {
            اگر (likely(led->groups) != 0) {
                xkb_layout_mask_t group_mask = 0;
                /* Effective and locked groups have been brought into range */
                أكد(state->components.group < XKB_MAX_GROUPS);
                أكد(state->components.locked_group >= 0 &&
                       state->components.locked_group < XKB_MAX_GROUPS);
                /* Effective and locked groups are used as mask */
                اگر (led->which_groups & XKB_STATE_LAYOUT_EFFECTIVE)
                    group_mask |= (UINT32_C(1) << state->components.group);
                اگر (led->which_groups & XKB_STATE_LAYOUT_LOCKED)
                    group_mask |= (UINT32_C(1) << state->components.locked_group);
                /* Base and latched groups only have to be non-zero */
                اگر ((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                    state->components.base_group != 0)
                    group_mask |= led->groups;
                اگر ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                    state->components.latched_group != 0)
                    group_mask |= led->groups;

                اگر (led->groups & group_mask) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    ادامه;
                }
            } وگرنه {
                /* Special case for Base and latched groups */
                اگر (((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                     state->components.base_group == 0) ||
                    ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                     state->components.latched_group == 0)) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    ادامه;
                }
            }
        }

        اگر (led->ctrls & state->components.controls) {
            state->components.leds |= (UINT32_C(1) << idx);
            ادامه;
        }
    }
}

/**
 * Calculates the derived state (effective mods/group and LEDs) from an
 * up-to-date xkb_state.
 */
ایستا تهی
xkb_state_update_derived(ساختار xkb_state *state)
{
    xkb_layout_index_t wrapped;

    state->components.mods = (state->components.base_mods |
                              state->components.latched_mods |
                              state->components.locked_mods);

    /* Lock group must be adjusted, but not base nor latched groups */
    wrapped = XkbWrapGroupIntoRange(state->components.locked_group,
                                    state->keymap->رقم_groups,
                                    state->out_of_range_group.policy,
                                    state->out_of_range_group.redirect_group);
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->components.locked_group =
        (int32_t) (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    /* Effective group must be adjusted */
    wrapped = XkbWrapGroupIntoRange(state->components.base_group +
                                    state->components.latched_group +
                                    state->components.locked_group,
                                    state->keymap->رقم_groups,
                                    state->out_of_range_group.policy,
                                    state->out_of_range_group.redirect_group);
    state->components.group =
        (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    xkb_state_led_update_all(state);
}

ایستا تهی
state_update_enabled_controls(ساختار xkb_server_state *state,
                              شمارش xkb_keyboard_control_flags affect_controls,
                              شمارش xkb_keyboard_control_flags controls,
                              ساختار xkb_events *events)
{
    ثابت bool had_sticky_کلیدها = state->base.components.controls
                               & CONTROL_STICKY_کلیدها;

    /*
     * Enable using the public API with the all the Control values, except
     * the internal ones, if any.
     */
    affect_controls =
        ( affect_controls
        & (شمارش xkb_keyboard_control_flags)CONTROL_ALL_BOOLEAN );
    state->base.components.controls &=
        (شمارش xkb_action_controls)~affect_controls;
    state->base.components.controls |=
        (شمارش xkb_action_controls)(controls & affect_controls);

    اگر (had_sticky_کلیدها &&
        !(state->base.components.controls & CONTROL_STICKY_کلیدها)) {
        /* Sticky keys were disabled: clear all locks and latches */
        clear_all_latches_and_locks(state, events);
    }

    xkb_state_update_derived(&state->base);
}

/**
 * Given a particular key event, updates the state structure to reflect the
 * new modifiers.
 */
شمارش xkb_state_component
xkb_state_update_کلید(ساختار xkb_state *base_state, xkb_keycode_t kc,
                     شمارش xkb_کلید_direction direction)
{
    /* Guard against client-only state */
    أكد(base_state->mode > SERVER_COMPANION);
    اگر (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        بازگشت 0;
    }

    ساختار xkb_server_state * ثابت state =
        (ساختار xkb_server_state *)base_state;

    ثابت ساختار xkb_کلید* ثابت کلید = Xkb_کلید(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    اگر (!کلید || (direction == XKB_کلید_REPEATED && !کلید->repeats))
        بازگشت 0;

    اگر (direction == XKB_کلید_DOWN &&
        (state->base.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیدها) &&
        (state->a11y_flags & XKB_A11_Y_STICKY_کلیدها_NO_SIMULTANEOUS_کلیدها) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیدها,
                                      0, پوچ);
    }

    ثابت ساختار state_components prev_components = state->base.components;

    state->set_mods = 0;
    state->clear_mods = 0;

    xkb_filter_apply_all(state, پوچ, کلید, direction);

    xkb_mod_index_t i;
    xkb_mod_mask_t bit;
    برای (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        اگر (state->set_mods & bit) {
            state->mod_کلید_شمار[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    برای (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        اگر (state->clear_mods & bit) {
            state->mod_کلید_شمار[i]--;
            اگر (state->mod_کلید_شمار[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_کلید_شمار[i] = 0;
            }
            state->clear_mods &= ~bit;
        }
    }

    xkb_state_update_derived(&state->base);

    بازگشت get_state_component_changes(&prev_components, &state->base.components);
}

/* We need fake keys for `update_latch_modifiers` and `update_latch_group`.
 * These keys must have at least one level in order to break latches. We need 2
 * keys with specific actions in order to update group/mod latches without
 * affecting each other. */
ایستا ساختار xkb_کلید_type_entry synthetic_کلید_level_entry = { 0 };
ایستا ساختار xkb_کلید_type synthetic_کلید_type = {
     .رقم_entries = 1,
     .رقم_levels = 1,
     .entries = &synthetic_کلید_level_entry
};
ایستا ثابت ساختار xkb_کلید synthetic_کلید = { 0 };

/* Transcription from xserver: XkbLatchModifiers */
ایستا تهی
update_latch_modifiers(ساختار xkb_server_state *state,
                       ساختار xkb_events *events,
                       xkb_mod_mask_t mask, xkb_mod_mask_t latches)
{
    /* Clear affected latched modifiers */
    ثابت xkb_mod_mask_t clear = mask & ~latches;
    state->base.components.latched_mods &= ~clear;

    /* Clear any pending latch to locks using ad hoc action:
     * only affect corresponding modifier latches and no group latch. */
    ساختار xkb_level synthetic_کلید_level_break_mod_latch = {
        .رقم_syms = 0,
        .رقم_actions = 1,
        .upper = XKB_کلید_No_نماد,
        .s.sym = XKB_کلید_No_نماد,
        .a.action.internal = {
            .type = ACTION_TYPE_INTERNAL,
            .flags = INTERNAL_BREAKS_MOD_LATCH,
            .clear_latched_mods = clear
        }
    };
    ساختار xkb_group synthetic_کلید_group_break_mod_latch = {
        .type = &synthetic_کلید_type,
        .levels = &synthetic_کلید_level_break_mod_latch
    };
    ثابت ساختار xkb_کلید synthetic_کلید_break_mod_latch = {
        .رقم_groups = 1,
        .groups = &synthetic_کلید_group_break_mod_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_کلید_break_mod_latch, XKB_کلید_DOWN);

    /* Finally set the latched mods by simulate tapping a key with the
     * corresponding action */
    ثابت ساختار xkb_کلید* ثابت کلید = &synthetic_کلید;
    ثابت اجتماع xkb_action latch_mods = {
        .mods = {
            .type = ACTION_TYPE_MOD_LATCH,
            .mods = { .mask = mask & latches },
            .flags = 0,
        },
    };
    ساختار xkb_filter* ثابت filter = xkb_filter_new(state);
    filter->کلید = کلید;
    filter->func = xkb_filter_mod_latch_func;
    filter->action = latch_mods;
    xkb_filter_mod_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_mod_latch_func(state, events, filter, کلید, XKB_کلید_UP);
}

/* Transcription from xserver: XkbLatchGroup */
ایستا تهی
update_latch_group(ساختار xkb_server_state *state,
                   ساختار xkb_events *events, int32_t group)
{
    /* Clear any pending latch to locks. */
    ایستا ساختار xkb_level synthetic_کلید_level_break_group_latch = {
        .رقم_syms = 0,
        .رقم_actions = 1,
        .upper = XKB_کلید_No_نماد,
        .s.sym = XKB_کلید_No_نماد,
        .a.action.internal = {
            .type = ACTION_TYPE_INTERNAL,
            .flags = INTERNAL_BREAKS_GROUP_LATCH,
            .clear_latched_mods = 0
        }
    };
    ایستا ساختار xkb_group synthetic_کلید_group_break_group_latch = {
        .type = &synthetic_کلید_type,
        .levels = &synthetic_کلید_level_break_group_latch
    };
    ایستا ثابت ساختار xkb_کلید synthetic_کلید_break_group_latch = {
        .رقم_groups = 1,
        .groups = &synthetic_کلید_group_break_group_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_کلید_break_group_latch, XKB_کلید_DOWN);

    /* Simulate tapping a key with a group latch action, but in isolation: i.e.
     * without affecting the other filters. */
    ثابت ساختار xkb_کلید* ثابت کلید = &synthetic_کلید;
    ثابت اجتماع xkb_action latch_group = {
        .group = {
            .type = ACTION_TYPE_GROUP_LATCH,
            .flags = ACTION_ABSOLUTE_SWITCH,
            .group = group,
        },
    };
    ساختار xkb_filter* ثابت filter = xkb_filter_new(state);
    filter->کلید = کلید;
    filter->func = xkb_filter_group_latch_func;
    filter->action = latch_group;
    xkb_filter_group_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_group_latch_func(state, events, filter, کلید, XKB_کلید_UP);
}

/**
 * Compute the resolved effective mask of an arbitrary input.
 *
 * Contrary to `mod_mask_get_effective`, it resolves only modifiers not present
 * in the canonical mask, so that it enables `xkb_state_serialize_mods` to
 * round trip via `xkb_state_update_mask`.
 */
ایستا درونخطی xkb_mod_mask_t
resolve_to_canonical_mods(ساختار xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    بازگشت
        /*
         * Keep canonical modifier mask.
         * It contains either real modifiers or canonical virtual modifiers.
         */
        (mods & keymap->canonical_state_mask) |
        /* Resolve other modifiers */
        mod_mask_get_effective(keymap,
                               mods & ~keymap->canonical_state_mask);
}

ایستا تهی
state_update_latched_locked(
    ساختار xkb_server_state * restrict state,
    ثابت ساختار xkb_state_components_update * restrict update,
    ساختار xkb_events * restrict events
)
{
    /* Update locks */
    ثابت xkb_mod_mask_t affect_locked_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_locked_mods);
    اگر ((update->components & XKB_STATE_MODS_LOCKED) &&
        affect_locked_mods) {
        ثابت xkb_mod_mask_t locked_mods =
            resolve_to_canonical_mods(state->base.keymap, update->locked_mods);
        state->base.components.locked_mods &= ~affect_locked_mods;
        state->base.components.locked_mods |= locked_mods & affect_locked_mods;
    }
    اگر (update->components & XKB_STATE_LAYOUT_LOCKED) {
        state->base.components.locked_group = update->locked_layout;
    }

    /* Update latches */
    ثابت xkb_mod_mask_t affect_latched_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_latched_mods);
    اگر ((update->components & XKB_STATE_MODS_LATCHED) &&
        affect_latched_mods) {
        ثابت xkb_mod_mask_t latched_mods =
            resolve_to_canonical_mods(state->base.keymap, update->latched_mods);
        update_latch_modifiers(state, events, affect_latched_mods, latched_mods);
    }
    اگر (update->components & XKB_STATE_LAYOUT_LATCHED) {
        update_latch_group(state, events, update->latched_layout);
    }
}

شمارش xkb_state_component
xkb_state_update_latched_locked(ساختار xkb_state *base_state,
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
    أكد(base_state->mode > SERVER_COMPANION);
    اگر (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        بازگشت 0;
    }

    ساختار xkb_server_state * ثابت state =
        (ساختار xkb_server_state *)base_state;

    ثابت ساختار state_components previous_components = state->base.components;
    ثابت شمارش xkb_state_component components
        = ((affect_latched_mods || latched_mods) ? XKB_STATE_MODS_LATCHED : 0)
        | ((affect_locked_mods || locked_mods) ? XKB_STATE_MODS_LOCKED : 0)
        | (affect_latched_layout ? XKB_STATE_LAYOUT_LATCHED : 0)
        | (affect_locked_layout ? XKB_STATE_LAYOUT_LOCKED : 0);
    ثابت ساختار xkb_state_components_update update = {
        .size = اندازه(update),
        .components = components,
        .affect_latched_mods = affect_latched_mods,
        .latched_mods = latched_mods,
        .latched_layout = latched_layout,
        .affect_locked_mods = affect_locked_mods,
        .locked_mods = locked_mods,
        .locked_layout = locked_layout
    };
    state_update_latched_locked(state, &update, پوچ);
    xkb_state_update_derived(&state->base);
    بازگشت get_state_component_changes(&previous_components,
                                       &state->base.components);
}

ایستا درونخطی تهی
clear_all_latches_and_locks(ساختار xkb_server_state *state,
                            ساختار xkb_events *events)
{
    ایستا ثابت شمارش xkb_state_component components
        = XKB_STATE_MODS_LATCHED | XKB_STATE_MODS_LOCKED
        | XKB_STATE_LAYOUT_LATCHED | XKB_STATE_LAYOUT_LOCKED;
    ثابت ساختار xkb_state_components_update update = {
        .size = اندازه(update),
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

ایستا شمارش xkb_status
state_update_layout_policy(ساختار xkb_server_state *state,
                           ثابت ساختار xkb_layout_policy_update *update)
{
    اگر (xkb_feature_supported(XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY,
                              update->policy)) {
        اگر (update->policy == XKB_LAYOUT_OUT_OF_RANGE_REDIRECT) {
            اگر (update->redirect < state->base.keymap->رقم_groups) {
                state->base.out_of_range_group.redirect_group =
                    update->redirect;
            } وگرنه {
                log_err(state->base.keymap->ctx,
                        XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Layout policy: "
                        "unsupported layout index %"PRIu32" > %"PRIu32"\n",
                        update->redirect + 1, state->base.keymap->رقم_groups);
                بازگشت XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
            }
        }
        state->base.out_of_range_group.policy = update->policy;
        بازگشت XKB_SUCCESS;
    } وگرنه {
        log_err(state->base.keymap->ctx,
                XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY_,
                "Unsupported layout policy: %d\n", update->policy);
        بازگشت XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY;
    }
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
ایستا شمارش xkb_status
check_state_update_abi_(ساختار xkb_context * restrict ctx,
                        ثابت نویسه * restrict func,
                        ثابت ساختار xkb_synthetic_update * restrict update)
{
    شمارش xkb_status status = XKB_SUCCESS;
    اگر ((status = xkb_check_state_abi(update)) ||
        (update->reserved0 != 0 &&
         (status = XKB_ERROR_ABI_FORWARD_COMPAT)) ||
        (update->components &&
         (status = xkb_check_state_abi(update->components))) ||
        (update->layout_policy &&
         (status = xkb_check_state_abi(update->layout_policy)))) {
        xkb_log_abi_error(ctx, func, status);
    }
    بازگشت status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

#define check_state_update_abi(ctx, update) \
    check_state_update_abi_(ctx, __func__, update)

شمارش xkb_status
xkb_state_update_synthetic(ساختار xkb_state * base_state,
                           ثابت ساختار xkb_synthetic_update * update,
                           شمارش xkb_state_component *changed)
{
    /* Guard against client-only state */
    اگر (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        بازگشت XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    ساختار xkb_server_state * ثابت state =
        (ساختار xkb_server_state *)base_state;

    /* Check ABI compatibility */
    شمارش xkb_status status = check_state_update_abi(state->base.keymap->ctx,
                                                    update);
    اگر (status != XKB_SUCCESS)
        بازگشت status;

    ثابت ساختار state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    /* Update parametrized controls first */
    اگر (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        اگر (status != XKB_SUCCESS)
            بازگشت status;
    }

    اگر (update->components) {
        ثابت ساختار xkb_state_components_update * ثابت components =
            update->components;
        /* Update boolean controls first */
        اگر (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, پوچ);
        }

        state_update_latched_locked(state, components, پوچ);
    }

    xkb_state_update_derived(&state->base);

    اگر (changed) {
        *changed = get_state_component_changes(&previous_components,
                                               &state->base.components);
    }
    بازگشت XKB_SUCCESS;
}

/**
 * Updates the state from a set of explicit masks as gained from
 * xkb_state_serialize_mods and xkb_state_serialize_groups.  As noted in the
 * documentation for these functions in xkbcommon.h, this round-trip is
 * lossy, and should only be used to update a slave state mirroring the
 * master, e.g. in a client/server window system.
 */
شمارش xkb_state_component
xkb_state_update_mask(ساختار xkb_state *base_state,
                      xkb_mod_mask_t base_mods,
                      xkb_mod_mask_t latched_mods,
                      xkb_mod_mask_t locked_mods,
                      xkb_layout_index_t base_group,
                      xkb_layout_index_t latched_group,
                      xkb_layout_index_t locked_group)
{
    /* Guard against server-only and server companion state */
    أكد(base_state->mode == CLIENT_STATE ||
           base_state->mode == LEGACY_MIXED_STATE);
    اگر (base_state->mode != CLIENT_STATE &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        بازگشت 0;
    }

    ساختار xkb_client_state * ثابت state =
        (ساختار xkb_client_state *)base_state;

    ثابت ساختار state_components prev_components = state->base.components;

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

    بازگشت get_state_component_changes(&prev_components, &state->base.components);
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Lock_Modifier
 */
ایستا bool
should_do_caps_transformation(ساختار xkb_state *state, xkb_keycode_t kc)
{
    بازگشت
        xkb_state_mod_index_is_active(state, XKB_MOD_INDEX_CAPS,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_index_is_consumed(state, kc, XKB_MOD_INDEX_CAPS) == 0;
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Control_Modifier
 */
ایستا bool
should_do_ctrl_transformation(ساختار xkb_state *state, xkb_keycode_t kc)
{
    بازگشت
        xkb_state_mod_index_is_active(state, XKB_MOD_INDEX_CTRL,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_index_is_consumed(state, kc, XKB_MOD_INDEX_CTRL) == 0;
}

/**
 * Provides the symbols to use for the given key and state.  Returns the
 * number of symbols pointed to in syms_out.
 */
صحیح
xkb_state_کلید_get_syms(ساختار xkb_state *state, xkb_keycode_t kc,
                       ثابت xkb_keysym_t **syms_out)
{
    ثابت xkb_layout_index_t layout = xkb_state_کلید_get_layout(state, kc);
    اگر (layout == XKB_LAYOUT_INVALID)
        برو err;

    ثابت xkb_level_index_t level = xkb_state_کلید_get_level(state, kc, layout);
    اگر (level == XKB_LEVEL_INVALID)
        برو err;

    ثابت ساختار xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);

    اگر (!کلید)
        برو err;

    ثابت ساختار xkb_level* ثابت leveli =
        xkb_keymap_کلید_get_level(state->keymap, کلید, layout, level);

    اگر (!leveli)
        برو err;

    ثابت xkb_keysym_شمار_t رقم_syms = leveli->رقم_syms;
    اگر (رقم_syms == 0)
        برو err;

    اگر (should_do_caps_transformation(state, kc)) {
        /* Only simple capitalization rules: keysyms count is unchanged. */
        اگر (رقم_syms > 1) {
            *syms_out = (leveli->has_upper)
                      ? leveli->s.syms + رقم_syms
                      : leveli->s.syms;
        } وگرنه {
            *syms_out = &leveli->upper;
        }
    } وگرنه {
        *syms_out = (رقم_syms > 1)
                  ? leveli->s.syms
                  : &leveli->s.sym;
    }
    بازگشت (صحیح) رقم_syms;

err:
    *syms_out = پوچ;
    بازگشت 0;
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
ایستا نویسه
XkbToControl(نویسه ch)
{
    نویسه c = ch;

    اگر ((c >= '@' && c < '\177') || c == ' ')
        c &= 0x1F;
    وگرنه اگر (c == '2')
        c = '\000';
    وگرنه اگر (c >= '3' && c <= '7')
        c -= ('3' - '\033');
    وگرنه اگر (c == '8')
        c = '\177';
    وگرنه اگر (c == '/')
        c = '_' & 0x1F;
    بازگشت c;
}

/**
 * Provides either exactly one symbol, or XKB_KEY_NoSymbol.
 */
xkb_keysym_t
xkb_state_کلید_get_one_sym(ساختار xkb_state *state, xkb_keycode_t kc)
{
    ثابت xkb_keysym_t *syms = پوچ;
    ثابت صحیح رقم_syms = xkb_state_کلید_get_syms(state, kc, &syms);

    اگر (رقم_syms != 1)
        بازگشت XKB_کلید_No_نماد;
    وگرنه
        بازگشت syms[0];
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
ایستا xkb_keysym_t
get_one_sym_for_string(ساختار xkb_state *state, xkb_keycode_t kc)
{
    ثابت xkb_layout_index_t layout = xkb_state_کلید_get_layout(state, kc);
    ثابت xkb_layout_index_t رقم_layouts =
        xkb_keymap_رقم_layouts_for_کلید(state->keymap, kc);
    xkb_level_index_t level = xkb_state_کلید_get_level(state, kc, layout);
    اگر (layout == XKB_LAYOUT_INVALID || رقم_layouts == 0 ||
        level == XKB_LEVEL_INVALID)
        بازگشت XKB_کلید_No_نماد;

    ثابت xkb_keysym_t *syms = پوچ;
    صحیح nsyms = xkb_keymap_کلید_get_syms_by_level(state->keymap, kc,
                                                 layout, level, &syms);
    اگر (nsyms != 1)
        بازگشت XKB_کلید_No_نماد;
    xkb_keysym_t sym = syms[0];

    اگر (should_do_ctrl_transformation(state, kc) && sym > 127u) {
        برای (xkb_layout_index_t i = 0; i < رقم_layouts; i++) {
            level = xkb_state_کلید_get_level(state, kc, i);
            اگر (level == XKB_LEVEL_INVALID)
                ادامه;

            nsyms = xkb_keymap_کلید_get_syms_by_level(state->keymap, kc,
                                                     i, level, &syms);
            اگر (nsyms == 1 && syms[0] <= 127u) {
                sym = syms[0];
                بشکن;
            }
        }
    }

    اگر (should_do_caps_transformation(state, kc)) {
        sym = xkb_keysym_to_upper(sym);
    }

    بازگشت sym;
}

صحیح
xkb_state_کلید_get_utf8(ساختار xkb_state *state, xkb_keycode_t kc,
                       نویسه *buffer, size_t size)
{
    صحیح nsyms;
    ثابت xkb_keysym_t *syms = پوچ;
    ثابت xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    اگر (sym != XKB_کلید_No_نماد) {
        nsyms = 1; syms = &sym;
    }
    وگرنه {
        nsyms = xkb_state_کلید_get_syms(state, kc, &syms);
    }

    /* Make sure not to truncate in the middle of a UTF-8 sequence. */
    صحیح offset = 0;
    نویسه tmp[XKB_KEYSYM_UTF8_MAX_SIZE];
    برای (صحیح i = 0; i < nsyms; i++) {
        صحیح ret = xkb_keysym_to_utf8(syms[i], tmp, اندازه(tmp));
        اگر (ret <= 0)
            برو err_bad;

        ret--;
        اگر ((size_t)offset + (size_t)ret <= size)
            memcpy(buffer + offset, tmp, (size_t)ret);
        offset += ret;
    }

    اگر ((size_t)offset >= size)
        برو err_trunc;
    buffer[offset] = '\0';

    اگر (!is_valid_utf8(buffer, (size_t)offset))
        برو err_bad;

    اگر (offset == 1 && (unsigned صحیح) buffer[0] <= 127u &&
        should_do_ctrl_transformation(state, kc))
        buffer[0] = XkbToControl(buffer[0]);

    بازگشت offset;

err_trunc:
    اگر (size > 0)
        buffer[size - 1] = '\0';
    بازگشت offset;

err_bad:
    اگر (size > 0)
        buffer[0] = '\0';
    بازگشت 0;
}

uint32_t
xkb_state_کلید_get_utf32(ساختار xkb_state *state, xkb_keycode_t kc)
{
    ثابت xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    uint32_t cp = xkb_keysym_to_utf32(sym);

    اگر (cp <= 127u && should_do_ctrl_transformation(state, kc))
        cp = (uint32_t) XkbToControl((نویسه) cp);

    بازگشت cp;
}

/**
 * Serializes the requested modifier state into an xkb_mod_mask_t, with all
 * the same disclaimers as in xkb_state_update_mask.
 */
ایستا درونخطی xkb_mod_mask_t
serialize_mods(ثابت ساختار state_components *components,
               شمارش xkb_state_component type)
{
    xkb_mod_mask_t ret = 0;

    اگر (type & XKB_STATE_MODS_EFFECTIVE)
        بازگشت components->mods;

    اگر (type & XKB_STATE_MODS_DEPRESSED)
        ret |= components->base_mods;
    اگر (type & XKB_STATE_MODS_LATCHED)
        ret |= components->latched_mods;
    اگر (type & XKB_STATE_MODS_LOCKED)
        ret |= components->locked_mods;

    بازگشت ret;
}

xkb_mod_mask_t
xkb_state_serialize_mods(ساختار xkb_state *state,
                         شمارش xkb_state_component type)
{
    بازگشت serialize_mods(&state->components, type);
}

/**
 * Serialises the requested group state, with all the same disclaimers as
 * in xkb_state_update_mask.
 */
ایستا درونخطی xkb_layout_index_t
serialize_layout(ثابت ساختار state_components *components,
                 شمارش xkb_state_component type)
{
    اگر (type & XKB_STATE_LAYOUT_EFFECTIVE)
        بازگشت components->group;

    int32_t ret = 0;
    اگر (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret += components->base_group;
    اگر (type & XKB_STATE_LAYOUT_LATCHED)
        ret += components->latched_group;
    اگر (type & XKB_STATE_LAYOUT_LOCKED)
        ret += components->locked_group;

    بازگشت (xkb_layout_index_t)ret;
}

xkb_layout_index_t
xkb_state_serialize_layout(ساختار xkb_state *state,
                           شمارش xkb_state_component type)
{
    بازگشت serialize_layout(&state->components, type);
}

xkb_led_mask_t
xkb_state_serialize_leds(ثابت ساختار xkb_state *state,
                         شمارش xkb_state_component type)
{
    بازگشت (type & XKB_STATE_LEDS)
        ? state->components.leds
        : 0;
}

ایستا درونخطی شمارش xkb_keyboard_control_flags
serialize_controls(ثابت ساختار state_components *components,
                   شمارش xkb_state_component type)
{
    بازگشت (type & XKB_STATE_CONTROLS_EFFECTIVE)
        /*
         * Enable using the public API with the all the Controls values, except
         * the internal ones, if any.
         */
        ? (شمارش xkb_keyboard_control_flags)
          (components->controls & CONTROL_ALL_BOOLEAN)
        : 0;
}

شمارش xkb_keyboard_control_flags
xkb_state_serialize_controls(ثابت ساختار xkb_state *state,
                             شمارش xkb_state_component type)
{
    بازگشت serialize_controls(&state->components, type);
}

/**
 * Gets a modifier mask and returns the resolved effective mask; this
 * is needed because some modifiers can also map to other modifiers, e.g.
 * the "NumLock" modifier usually also sets the "Mod2" modifier.
 */
xkb_mod_mask_t
mod_mask_get_effective(ساختار xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    /* Initialize the effective mask with its corresponding real mods. */
    xkb_mod_mask_t mask = mods & MOD_REAL_MASK_ALL;

    /* Resolve the virtual modifiers */
    ثابت ساختار xkb_mod *mod;
    xkb_mod_index_t i;
    xkb_vmods_enumerate(i, mod, &keymap->mods)
        اگر (mods & (UINT32_C(1) << i))
            mask |= mod->mapping;

    بازگشت mask;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
صحیح
xkb_state_mod_index_is_active(ساختار xkb_state *state,
                              xkb_mod_index_t idx,
                              شمارش xkb_state_component type)
{
    اگر (unlikely(idx >= xkb_keymap_رقم_mods(state->keymap)))
        بازگشت -1;

    ثابت xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    اگر (!mapping) {
        /* Modifier not mapped */
        بازگشت 0;
    }
    /* WARNING: this may overmatch for virtual modifiers */
    بازگشت (xkb_state_serialize_mods(state, type) & mapping) == mapping;
}

شمارش {
    XKB_STATE_MATCH_FLAGS = XKB_STATE_MATCH_ANY
                          | XKB_STATE_MATCH_ALL
                          | XKB_STATE_MATCH_NON_EXCLUSIVE
};

/**
 * Helper function for xkb_state_mod_indices_are_active and
 * xkb_state_mod_names_are_active.
 */
ایستا bool
match_mod_masks(ساختار xkb_state *state,
                شمارش xkb_state_component type,
                شمارش xkb_state_match match,
                xkb_mod_mask_t wanted)
{
    ثابت xkb_mod_mask_t active = xkb_state_serialize_mods(state, type);

    اگر (!(match & XKB_STATE_MATCH_NON_EXCLUSIVE) && (active & ~wanted))
        بازگشت false;

    اگر (match & XKB_STATE_MATCH_ANY)
        بازگشت active & wanted;

    /* Ensure all flags but XKB_STATE_MATCH_ALL are processed before */
    static_assert(XKB_STATE_MATCH_ALL ==
                  ((شمارش xkb_state_match) XKB_STATE_MATCH_FLAGS &
                   ~(XKB_STATE_MATCH_ANY | XKB_STATE_MATCH_NON_EXCLUSIVE)),
                  "");

    بازگشت (active & wanted) == wanted;
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, or -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
صحیح
xkb_state_mod_indices_are_active(ساختار xkb_state *state,
                                 شمارش xkb_state_component type,
                                 شمارش xkb_state_match match,
                                 ...)
{
    اگر (match & ~((شمارش xkb_state_match) XKB_STATE_MATCH_FLAGS))
        بازگشت -2;

    va_list ap;
    xkb_mod_mask_t wanted = 0;
    صحیح ret = 0;
    ثابت xkb_mod_index_t رقم_mods = xkb_keymap_رقم_mods(state->keymap);

    va_start(ap, match);
    وقتی (1) {
        xkb_mod_index_t idx = va_arg(ap, xkb_mod_index_t);
        اگر (idx == XKB_MOD_INVALID)
            بشکن;
        اگر (unlikely(idx >= رقم_mods)) {
            ret = -1;
            بشکن;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_end(ap);

    اگر (ret == -1)
        بازگشت ret;

    اگر (!wanted) {
        /* Modifiers not mapped */
        بازگشت 0;
    }

    بازگشت match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
صحیح
xkb_state_mod_name_is_active(ساختار xkb_state *state, ثابت نویسه *name,
                             شمارش xkb_state_component type)
{
    ثابت xkb_mod_index_t idx = xkb_keymap_mod_get_index(state->keymap, name);

    اگر (idx == XKB_MOD_INVALID)
        بازگشت -1;

    بازگشت xkb_state_mod_index_is_active(state, idx, type);
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ATTR_NULL_SENTINEL صحیح
xkb_state_mod_names_are_active(ساختار xkb_state *state,
                               شمارش xkb_state_component type,
                               شمارش xkb_state_match match,
                               ...)
{
    اگر (match & ~((شمارش xkb_state_match) XKB_STATE_MATCH_FLAGS))
        بازگشت -2;

    va_list ap;
    xkb_mod_mask_t wanted = 0;
    صحیح ret = 0;

    va_start(ap, match);
    وقتی (1) {
        ثابت نویسه *str = va_arg(ap, ثابت نویسه *);
        اگر (str == پوچ)
            بشکن;
        ثابت xkb_mod_index_t idx = xkb_keymap_mod_get_index(state->keymap, str);
        اگر (idx == XKB_MOD_INVALID) {
            ret = -1;
            بشکن;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_end(ap);

    اگر (ret == -1)
        بازگشت ret;

    اگر (!wanted) {
        /* Modifiers not mapped */
        بازگشت 0;
    }

    بازگشت match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given group is active with the specified type(s), 0 if
 * not, or -1 if the group is invalid.
 */
صحیح
xkb_state_layout_index_is_active(ساختار xkb_state *state,
                                 xkb_layout_index_t idx,
                                 شمارش xkb_state_component type)
{
    اگر (idx >= state->keymap->رقم_groups)
        بازگشت -1;

    صحیح ret = 0;
    اگر (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ret |= (state->components.group == idx);
    اگر (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret |= (state->components.base_group == (int32_t) idx);
    اگر (type & XKB_STATE_LAYOUT_LATCHED)
        ret |= (state->components.latched_group == (int32_t) idx);
    اگر (type & XKB_STATE_LAYOUT_LOCKED)
        ret |= (state->components.locked_group == (int32_t) idx);

    بازگشت ret;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
صحیح
xkb_state_layout_name_is_active(ساختار xkb_state *state, ثابت نویسه *name,
                                شمارش xkb_state_component type)
{
    ثابت xkb_layout_index_t idx =
        xkb_keymap_layout_get_index(state->keymap, name);

    اگر (idx == XKB_LAYOUT_INVALID)
        بازگشت -1;

    بازگشت xkb_state_layout_index_is_active(state, idx, type);
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
صحیح
xkb_state_led_index_is_active(ساختار xkb_state *state, xkb_led_index_t idx)
{
    اگر (idx >= state->keymap->رقم_leds ||
        state->keymap->leds[idx].name == XKB_ATOM_NONE)
        بازگشت -1;

    بازگشت !!(state->components.leds & (UINT32_C(1) << idx));
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
صحیح
xkb_state_led_name_is_active(ساختار xkb_state *state, ثابت نویسه *name)
{
    ثابت xkb_led_index_t idx = xkb_keymap_led_get_index(state->keymap, name);

    اگر (idx == XKB_LED_INVALID)
        بازگشت -1;

    بازگشت xkb_state_led_index_is_active(state, idx);
}

/**
 * See:
 * - XkbTranslateKeyCode(3), mod_rtrn return value, from libX11.
 * - MyEnhancedXkbTranslateKeyCode(), a modification of the above, from GTK+.
 */
ایستا xkb_mod_mask_t
کلید_get_consumed(ساختار xkb_state *state, ثابت ساختار xkb_کلید *کلید,
                 شمارش xkb_consumed_mode mode)
{
    ثابت xkb_layout_index_t group =
        xkb_state_کلید_get_layout(state, کلید->keycode);
    اگر (group == XKB_LAYOUT_INVALID)
        بازگشت 0;

    xkb_mod_mask_t preserve = 0;
    xkb_mod_mask_t consumed = 0;

    ثابت ساختار xkb_کلید_type_entry* ثابت matching_entry =
        get_entry_for_کلید_state(state, کلید, group);
    اگر (matching_entry)
        preserve = matching_entry->preserve.mask;

    ثابت ساختار xkb_کلید_type* ثابت type = کلید->groups[group].type;
    انتخاب (mode) {
    حالت XKB_CONSUMED_MODE_XKB:
        consumed = type->mods.mask;
        بشکن;

    حالت XKB_CONSUMED_MODE_GTK: {
        ثابت ساختار xkb_کلید_type_entry* ثابت no_mods_entry =
            get_entry_for_mods(type, 0);
        ثابت xkb_level_index_t no_mods_leveli = no_mods_entry
                                               ? no_mods_entry->level
                                               : 0;
        ثابت ساختار xkb_level* ثابت no_mods_level =
            &کلید->groups[group].levels[no_mods_leveli];

        برای (darray_size_t i = 0; i < type->رقم_entries; i++) {
            ثابت ساختار xkb_کلید_type_entry* ثابت entry = &type->entries[i];
            اگر (!entry_is_active(entry))
                ادامه;

            ثابت ساختار xkb_level* ثابت level =
                &کلید->groups[group].levels[entry->level];
            اگر (XkbLevelsSameSyms(level, no_mods_level))
                ادامه;

            اگر (entry == matching_entry || one_bit_set(entry->mods.mask))
                consumed |= entry->mods.mask & ~entry->preserve.mask;
        }
        بشکن;
    }
    }

    بازگشت consumed & ~preserve;
}

صحیح
xkb_state_mod_index_is_consumed2(ساختار xkb_state *state, xkb_keycode_t kc,
                                 xkb_mod_index_t idx,
                                 شمارش xkb_consumed_mode mode)
{
    ثابت ساختار xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);

    اگر (unlikely(!کلید || idx >= xkb_keymap_رقم_mods(state->keymap)))
        بازگشت -1;

    ثابت xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    اگر (!mapping) {
        /* Modifier not mapped */
        بازگشت 0;
    }
    بازگشت (mapping & کلید_get_consumed(state, کلید, mode)) == mapping;
}

صحیح
xkb_state_mod_index_is_consumed(ساختار xkb_state *state, xkb_keycode_t kc,
                                xkb_mod_index_t idx)
{
    بازگشت xkb_state_mod_index_is_consumed2(state, kc, idx,
                                            XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_mod_mask_remove_consumed(ساختار xkb_state *state, xkb_keycode_t kc,
                                   xkb_mod_mask_t mask)
{
    ثابت ساختار xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);

    اگر (!کلید)
        بازگشت 0;

    بازگشت resolve_to_canonical_mods(state->keymap, mask) &
           ~کلید_get_consumed(state, کلید, XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_کلید_get_consumed_mods2(ساختار xkb_state *state, xkb_keycode_t kc,
                                 شمارش xkb_consumed_mode mode)
{
    انتخاب (mode) {
    حالت XKB_CONSUMED_MODE_XKB:
    حالت XKB_CONSUMED_MODE_GTK:
        بشکن;
    پیشفرض:
        log_err_func(state->keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "unrecognized consumed modifiers mode: %d\n", mode);
        بازگشت 0;
    }

    ثابت ساختار xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);
    اگر (!کلید)
        بازگشت 0;

    بازگشت کلید_get_consumed(state, کلید, mode);
}

xkb_mod_mask_t
xkb_state_کلید_get_consumed_mods(ساختار xkb_state *state, xkb_keycode_t kc)
{
    بازگشت xkb_state_کلید_get_consumed_mods2(state, kc, XKB_CONSUMED_MODE_XKB);
}

/*******************************************************************************
 * Server state machine API
 ******************************************************************************/

/** Entry to keep track of the state of an overlaid key */
ساختار xkb_overlaid_کلید {
    /** The source key being overlaid */
    ثابت ساختار xkb_کلید *old;
    /** The target key */
    ثابت ساختار xkb_کلید *new;
    /** Counter of key currently down */
    صحیح refcnt;
};

ساختار xkb_shortcuts_config_entry {
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
ساختار xkb_machine {
    /** Inherits from `xkb_server_state` */
    ساختار xkb_server_state base;

    /* Extra server-specific state data */

    /** Keyboard overlays handling */
    ساختار {
        /**
         * Current overlaid keys
         *
         * When a key is overlaid, it remained overlaid in this configuration
         * until totally released, i.e. every key press was matched with the
         * corresponding key release.
         */
        darray(ساختار xkb_overlaid_کلید) کلیدها;
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
    ساختار machine_config {
        /** Modifiers remapping */
        ساختار machine_modifiers_config {
            /** Real modifier mask */
            xkb_mod_mask_t mask;
            /** Modifiers re-mappings */
            darray_size_t mappings_رقم;
            ساختار machine_mods_mapping {
                xkb_mod_mask_t source;
                xkb_mod_mask_t target;
            } * mappings ATTR_COUNTED_BY(mappings_رقم);
        } modifiers;

        /** Shortcuts tweak */
        ساختار machine_shortcuts_config {
            /** Substitution entries */
            ساختار xkb_shortcuts_config_entry *entries;
        } shortcuts;
    } config;

    /** Mouse keys handling */
    ساختار {
        /** Default pointer button */
        xkb_pointer_button_index_t default_button;
        /** Mask of the locked buttons */
        xkb_pointer_button_mask_t locked_buttons;
    } mouse;

    شمارش xkb_machine_flags flags;
};

تعریفنوع darray(ساختار machine_mods_mapping) machine_mods_mappings;

ساختار xkb_machine_builder {
    ساختار xkb_keymap *keymap;

    /** Controls */
    ساختار {
        /** Accessibility flags */
        ساختار {
            شمارش xkb_a11y_flags affect_flags;
            شمارش xkb_a11y_flags flags;
        } a11y;
    } controls;

    /** Modifiers re-mapping */
    machine_mods_mappings mods;

    /** Shortcuts tweak */
    ساختار xkb_shortcuts_config_options {
        /** Substitution entries */
        darray(ساختار xkb_shortcuts_config_entry) entries;
    } shortcuts;

    شمارش xkb_machine_flags machine_flags;
    شمارش xkb_machine_builder_flags builder_flags;
    صحیح refcnt;
};

ساختار xkb_machine_builder *
xkb_machine_builder_new(
    ساختار xkb_keymap * restrict keymap,
    ثابت ساختار xkb_machine_builder_config * restrict config,
    شمارش xkb_status * restrict status
)
{
    /* Handle default configuration */
    ایستا ثابت ساختار xkb_machine_builder_config default_config = {
        .size = اندازه(default_config)
    };
    اگر (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ثابت شمارش xkb_status status_ = xkb_check_state_abi(config);
    اگر (status_) {
        xkb_log_abi_error(keymap->ctx, __func__, status_);
        اگر (status)
            *status = status_;
        بازگشت پوچ;
    }

    /* Sanitize input */
    ثابت شمارش xkb_machine_builder_flags invalid_builder_flags
        = config->builder_flags
        & ~(شمارش xkb_machine_builder_flags)XKB_MACHINE_BUILDER_FLAGS_مقادیر;
    اگر (invalid_builder_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS_,
                     "unrecognized machine builder flags: 0x%x\n",
                     invalid_builder_flags);
        اگر (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS;
        بازگشت پوچ;
    }

    ثابت شمارش xkb_machine_flags invalid_machine_flags
        = (شمارش xkb_machine_flags)config->machine_flags
        & ~(شمارش xkb_machine_flags)XKB_MACHINE_FLAGS_مقادیر;
    اگر (invalid_machine_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS_,
                     "unrecognized state machine flags: 0x%x\n",
                     invalid_machine_flags);
        اگر (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS;
        بازگشت پوچ;
    }

    ساختار xkb_machine_builder * ثابت builder = calloc(1, اندازه(*builder));
    اگر (!builder) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine builder\n");
        اگر (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        بازگشت پوچ;
    }

    *builder = (ساختار xkb_machine_builder) {
        .keymap = xkb_keymap_ref(keymap),
        .refcnt = 1,
        .builder_flags = (شمارش xkb_machine_builder_flags)config->builder_flags,
        .machine_flags = (شمارش xkb_machine_flags)config->machine_flags,
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

    اگر (status)
        *status = XKB_SUCCESS;

    بازگشت builder;
}

ساختار xkb_machine_builder *
xkb_machine_builder_ref(ساختار xkb_machine_builder *builder)
{
    أكد(builder->refcnt > 0);
    builder->refcnt++;
    بازگشت builder;
}

تهی
xkb_machine_builder_unref(ساختار xkb_machine_builder *builder)
{
    أكد(!builder || builder->refcnt > 0);
    اگر (!builder || --builder->refcnt > 0)
        بازگشت;

    darray_free(builder->shortcuts.entries);
    darray_free(builder->mods);
    xkb_keymap_unref(builder->keymap);
    آزاد(builder);
}

ساختار xkb_keymap *
xkb_machine_builder_get_keymap(ثابت ساختار xkb_machine_builder *builder)
{
    /* Reference count is not updated. See API doc. */
    بازگشت builder->keymap;
}

شمارش xkb_status
xkb_machine_builder_update_a11y(
    ساختار xkb_machine_builder * restrict builder,
    ثابت ساختار xkb_machine_builder_a11y_update * restrict update)
{
    /* Check ABI compatibility */
    شمارش xkb_status status = xkb_check_state_abi(update);
    اگر (status != XKB_SUCCESS) {
        xkb_log_abi_error(builder->keymap->ctx, __func__, status);
        بازگشت status;
    }

    ثابت شمارش xkb_a11y_flags invalid_flags =
        ~(شمارش xkb_a11y_flags)XKB_A11_Y_FLAGS_مقادیر;

    اگر (update->affect_flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y affected flags: %#x\n",
                __func__, update->affect_flags & invalid_flags);
        بازگشت XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }
    اگر (update->flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y flags: %#x\n",
                __func__, update->flags & invalid_flags);
        بازگشت XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }

    builder->controls.a11y.affect_flags |= update->affect_flags;
    builder->controls.a11y.flags &= ~update->affect_flags;
    builder->controls.a11y.flags |= (update->flags & update->affect_flags);

    بازگشت XKB_SUCCESS;
}

شمارش xkb_status
xkb_machine_builder_update_mods_remap(
    ساختار xkb_machine_builder * restrict builder,
    ثابت ساختار xkb_machine_builder_mods_remap_update * restrict update
)
{
    اگر (!update->source) {
        اگر (!update->target) {
            /* Reset mappings */
            darray_resize(builder->mods, 0);
            بازگشت XKB_SUCCESS;
        } وگرنه {
            بازگشت XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
        }
    }

    /* Check the modifiers against the keymap */
    ثابت xkb_mod_mask_t invalid = ~builder->keymap->canonical_state_mask;
    اگر ((update->source & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid source modifiers: 0x%"PRIx32"\n",
                __func__, update->source);
        بازگشت XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    اگر ((update->target & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid target modifiers: 0x%"PRIx32"\n",
                __func__, update->target);
        بازگشت XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    ساختار machine_mods_mapping *mapping = پوچ;
    darray_size_t m = 0;
    darray_enumerate(m, mapping, builder->mods) {
        اگر (mapping->source == update->source) {
            اگر (!update->target) {
                /* Remove mapping */
                darray_remove(builder->mods, m);
            } وگرنه {
                /* Update mapping */
                mapping->target = update->target;
            }
            بازگشت XKB_SUCCESS;
        }
    }

    اگر (update->target) {
        /* Append new mapping */
        darray_append(
            builder->mods,
            (ساختار machine_mods_mapping) {
                .source = update->source,
                .target = update->target
            }
        );
    } وگرنه {
        /* Ignore missing mapping */
    }

    بازگشت XKB_SUCCESS;
}

شمارش xkb_status
xkb_machine_builder_update_shortcut_override(
    ساختار xkb_machine_builder * restrict builder,
    ثابت ساختار xkb_machine_builder_shortcut_override_update * restrict update
)
{
    ساختار xkb_keymap *keymap = builder->keymap;

    /* Check the modifiers against the keymap */
    ثابت xkb_mod_mask_t invalid = ~keymap->canonical_state_mask;
    اگر ((update->affect_mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid affected modifiers: 0x%"PRIx32"\n",
                __func__, update->affect_mods);
        بازگشت XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    اگر ((update->mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid modifiers: 0x%"PRIx32"\n",
                __func__, update->mods);
        بازگشت XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    /* Check the layout indices against the keymap */
    اگر (update->source >= keymap->رقم_groups &&
        update->source != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "%s: Invalid source layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->رقم_groups, update->source + 1);
        بازگشت XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }
    اگر (update->target >= keymap->رقم_groups &&
        update->target != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "%s: Invalid target layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->رقم_groups, update->target + 1);
        بازگشت XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    ساختار xkb_shortcuts_config_options * ثابت config = &builder->shortcuts;

    /* Resize array & initialize new entries, if relevant */
    اگر (update->source >= darray_size(config->entries) &&
        update->source != XKB_LAYOUT_INVALID) {
        اگر (update->target == update->source) {
            /* Skip default setting */
            بازگشت XKB_SUCCESS;
        }
        xkb_layout_index_t new = darray_size(config->entries);
        darray_resize(config->entries, update->source + 1);
        برای (; new <= update->source; new++) {
            darray_item(config->entries, new) =
                (ساختار xkb_shortcuts_config_entry) {
                    .target = XKB_LAYOUT_INVALID,
                    .mods = 0
                };
        }
    }

    ساختار xkb_shortcuts_config_entry *entry;
    xkb_layout_index_t source = (update->source == XKB_LAYOUT_INVALID)
        ? 0               /* Loop over all entries */
        : update->source; /* Single entry */
    darray_foreach_from(entry, config->entries, source) {
        اگر (update->source == XKB_LAYOUT_INVALID &&
            entry->target == XKB_LAYOUT_INVALID) {
            /* Skip previous deactivated entry */
            source++;
            ادامه;
        }

        اگر (update->affect_mods) {
            entry->mods &= ~update->affect_mods;
            entry->mods |= (update->affect_mods & update->mods);
        }

        اگر (update->source != XKB_LAYOUT_INVALID) {
            entry->target = (update->target == source)
                ? XKB_LAYOUT_INVALID
                : update->target;
            /* Single entry: exit loop */
            بشکن;
        }
        source++;
    }

    بازگشت XKB_SUCCESS;
}

/** Compare 2 modifiers combinations */
ایستا صحیح
cmp_mod_masks(ثابت تهی *a, ثابت تهی *b)
{
    ثابت xkb_mod_mask_t m1 = *((xkb_mod_mask_t *) a);
    ثابت xkb_mod_mask_t m2 = *((xkb_mod_mask_t *) b);
    اگر (m1 == m2)
        بازگشت 0;

    اگر (!m1)
        بازگشت +1;
    اگر (!m2)
        بازگشت -1;

    ثابت xkb_mod_mask_t overlap = (m1 & m2);
    اگر (overlap == m1)
        بازگشت +1; /* m1 is a subset of m2 */
    اگر (overlap == m2)
        بازگشت -1; /* m2 is a subset of m1 */

    /* Compare active modifiers numbers */
    ثابت unsigned صحیح count1 = popcount32(m1);
    ثابت unsigned صحیح count2 = popcount32(m2);
    اگر (count1 > count2)
        بازگشت -1;
    اگر (count1 < count2)
        بازگشت +1;

    /* Give priority to real modifiers */
    بازگشت (m1 < m2) ? -1 : +1;
}

ایستا شمارش xkb_status
machine_set_mods(ساختار xkb_machine *sm,
                 ثابت machine_mods_mappings *raw_mappings)
{
    اگر (!darray_empty(*raw_mappings)) {
        machine_mods_mappings mappings = darray_new();
        xkb_mod_mask_t mask = 0;
        ثابت ساختار machine_mods_mapping *mapping;
        ثابت xkb_mod_mask_t invalid =
            ~sm->base.base.keymap->canonical_state_mask;
        darray_foreach(mapping, *raw_mappings) {
            اگر (!mapping->source || !mapping->target ||
                (mapping->source & invalid) || (mapping->target & invalid))
                ادامه;
            darray_append(mappings, *mapping);
            mask |= mapping->source;
        }

        /*
         * Re-order the mappings, so that combinations have higher priority
         * if they are superset of other combinations.
         * For other type of overlaps, see the sorting function.
         */
        qsort(darray_اقلام(mappings), darray_size(mappings),
              اندازه(*darray_اقلام(mappings)), &cmp_mod_masks);

        darray_steal(
            mappings,
            &sm->config.modifiers.mappings,
            &sm->config.modifiers.mappings_رقم
        );
        sm->config.modifiers.mask = mask;
    } وگرنه {
        sm->config.modifiers = (ساختار machine_modifiers_config) {
            .mappings = پوچ,
            .mappings_رقم = 0,
            .mask = 0
        };
    }

    بازگشت XKB_SUCCESS;
}

ایستا شمارش xkb_status
machine_set_shortcuts(
    ساختار xkb_machine * restrict sm,
    ثابت ساختار xkb_shortcuts_config_options * restrict options
)
{
    اگر (darray_empty(options->entries)) {
        sm->config.shortcuts = (ساختار machine_shortcuts_config) {
            .entries = پوچ
        };
        بازگشت XKB_SUCCESS;
    }

    ساختار xkb_keymap * ثابت keymap = sm->base.base.keymap;

    ساختار xkb_shortcuts_config_entry * entries =
        calloc(keymap->رقم_groups, اندازه(*entries));
    اگر (!entries)
        بازگشت XKB_ERROR_ALLOCATION_FAILURE;

    ثابت xkb_layout_index_t شمار = MIN(
        keymap->رقم_groups,
        (xkb_layout_index_t) darray_size(options->entries)
    );

    bool some_entries = false;
    برای (xkb_layout_index_t l = 0; l < شمار; l++) {
        /* Sanitize layout target */
        ثابت xkb_layout_index_t target =
            darray_item(options->entries, l).target;
        /* Sanitize modifier mask */
        ثابت xkb_mod_mask_t mods = (
            darray_item(options->entries, l).mods &
            sm->base.base.keymap->canonical_state_mask
        );
        اگر (target < keymap->رقم_groups && target != l && mods) {
            entries[l] = (ساختار xkb_shortcuts_config_entry) {
                .target = target,
                .mods = mods
            };
            some_entries = true;
        } وگرنه {
            entries[l] = (ساختار xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
            ادامه;
        }
    }

    اگر (!some_entries) {
        آزاد(entries);
        entries = پوچ;
    } وگرنه {
        /* Initialize groups with no entries */
        برای (xkb_layout_index_t l = شمار; l < keymap->رقم_groups; l++) {
            entries[l] = (ساختار xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
        }
    }

    sm->config.shortcuts = (ساختار machine_shortcuts_config) {
        .entries = entries
    };
    بازگشت XKB_SUCCESS;
}

ساختار xkb_machine *
xkb_machine_new(ثابت ساختار xkb_machine_builder * restrict builder,
                شمارش xkb_status * restrict status)
{
    ساختار xkb_machine * ثابت machine = calloc(1, اندازه(*machine));
    اگر (!machine) {
        log_err_func1(builder->keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine\n");
        اگر (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        بازگشت پوچ;
    }

    xkb_server_state_init(&machine->base, builder->keymap, SERVER_STATE,
                          builder->controls.a11y.affect_flags,
                          builder->controls.a11y.flags);
    machine->flags = builder->machine_flags;
    اگر (!(machine->flags &
          XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_کلید_FIX))
    {
        machine->base.flags |= STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_کلید;
    }
    اگر (machine->flags & XKB_MACHINE_UNLOCK_MODS_ON_PRESS) {
        machine->base.flags |= STATE_UNLOCK_MODS_ON_PRESS;
    }

    شمارش xkb_status status_;
    اگر ((status_ = machine_set_mods(machine, &builder->mods)) != XKB_SUCCESS ||
        (status_ = machine_set_shortcuts(machine, &builder->shortcuts)) !=
        XKB_SUCCESS) {
        *status = status_;
        برو error;
    }

    darray_init(machine->overlays.کلیدها);
    machine->mouse.default_button = XKB_POINTER_BUTTON_MIN;

    اگر (status)
        *status = XKB_SUCCESS;

    بازگشت machine;

error:
    xkb_machine_unref(machine);
    بازگشت پوچ;
}

ساختار xkb_machine *
xkb_machine_ref(ساختار xkb_machine *sm)
{
    أكد(sm->base.base.refcnt > 0);
    sm->base.base.refcnt++;
    بازگشت sm;
}

تهی
xkb_machine_unref(ساختار xkb_machine *sm)
{
    أكد(!sm || sm->base.base.refcnt > 0);
    اگر (!sm || --sm->base.base.refcnt > 0)
        بازگشت;

    xkb_state_destroy(&sm->base.base);
    darray_free(sm->overlays.کلیدها);
    آزاد(sm->config.shortcuts.entries);
    آزاد(sm->config.modifiers.mappings);
    آزاد(sm);
}

ساختار xkb_keymap *
xkb_machine_get_keymap(ثابت ساختار xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    بازگشت sm->base.base.keymap;
}

ساختار xkb_state *
xkb_machine_get_state(ساختار xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    بازگشت (ساختار xkb_state *)sm;
}

ساختار xkb_state *
xkb_state_new_from_machine(ثابت ساختار xkb_machine * restrict machine,
                           شمارش xkb_status * restrict status)
{
    /* Creation */
    شمارش xkb_status status_;
    ساختار xkb_state * ثابت state = xkb_state_new_with_mode(
        machine->base.base.keymap,
        XKB_STATE_MODE_SERVER_QUERY,
        &status_
    );
    اگر (status_ != XKB_SUCCESS) {
        اگر (status)
            *status = status_;
        بازگشت پوچ;
    }

    أكد(state->mode == SERVER_COMPANION);
    أكد(state->refcnt == 1);

    /* Copy machine’s state */
    *state = machine->base.base;

    /* Restore previous values */
    state->mode = SERVER_COMPANION;
    state->refcnt = 1;

    اگر (status)
        *status = XKB_SUCCESS;
    بازگشت state;
}

ایستا تهی
machine_update_overlays(ساختار xkb_machine *sm)
{
    /*
     * Overlays indices are stored 1-indexed in nibbles: the lowest
     * nibble corresponds to the latest activated index.
     */

    ثابت xkb_overlay_mask_t mask =
        overlays_from_controls(sm->base.base.components.controls);
    xkb_overlay_mask_t added = mask & ~sm->overlays.enabled;

    /* Remove overlays no longer enabled and keep relative order */
    uint32_t order = sm->overlays.order;
    ثابت xkb_overlay_index_t overlay_max =
        format_max_overlays(sm->base.base.keymap->format);
    برای (uint8_t n = 0; n < overlay_max;) {
        xkb_overlay_index_t overlay_idx = (order >> (n * 4)) & 0xf;
        اگر (!overlay_idx)
            بشکن;
        ثابت xkb_overlay_mask_t overlay_mask =
            (xkb_overlay_mask_t)(1u << (overlay_idx - 1u) /* k is 1-indexed */);
        اگر (overlay_mask & mask) {
            /* no duplicates */
            added &= ~overlay_mask;
            n++;
        } وگرنه {
            /* remove overlay */
            ثابت uint32_t head = (UINT32_C(1) << (n * 4)) - 1;
            order = (order & head) | ((order >> 4) & ~head);
        }
    }

    /* Add new overlays in ascending order */
    برای (xkb_overlay_index_t k = 0; added; k++, added >>= 1) {
        اگر (added & 0x1) {
            order <<= 4;
            order |= k + 1u; /* k is 0-indexed */
        }
    }

    sm->overlays.order = order;
    sm->overlays.enabled = mask;
}

شمارش xkb_status
xkb_machine_process_synthetic(ساختار xkb_machine *sm,
                              ثابت ساختار xkb_synthetic_update *update,
                              ساختار xkb_events *events)
{
    /* Check ABI compatibility */
    شمارش xkb_status status =
        check_state_update_abi(sm->base.base.keymap->ctx, update);
    اگر (status != XKB_SUCCESS)
        بازگشت status;

    ساختار xkb_server_state * ثابت state = &sm->base;
    ثابت ساختار state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    darray_size(events->queue) = 0;
    events->next = 0;

    /* Update parametrized controls first */
    اگر (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        اگر (status != XKB_SUCCESS)
            بازگشت status;
    }

    اگر (update->components) {
        ثابت ساختار xkb_state_components_update * ثابت components =
            update->components;
        /* Update boolean controls first */
        اگر (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, events);
        }

        state_update_latched_locked(state, components, events);
    }

    xkb_state_update_derived(&state->base);

    ثابت شمارش xkb_state_component changed = get_state_component_changes(
        &previous_components, &state->base.components
    );
    اگر (changed) {
        // TODO: latch controls
        اگر (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        /* Create event only if some component actually changed */
        darray_append(events->queue, (ساختار xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .changed = changed,
                .components = state->base.components
            }
        });

        /* Terminate the frame */
        darray_append(events->queue, (ساختار xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    بازگشت XKB_SUCCESS;
}

ایستا ssize_t
do_remap_modifiers(ثابت ساختار machine_modifiers_config * restrict mappings,
                   ساختار xkb_state * restrict state,
                   ساختار xkb_events * restrict events,
                   ثابت ساختار xkb_کلید * restrict کلید)
{
    اگر (!(mappings->mask & state->components.mods))
        بازگشت -1;

    ثابت xkb_layout_index_t layout = state_کلید_get_layout(state, کلید);
    اگر (layout >= کلید->رقم_groups)
        بازگشت -1;

    ثابت ساختار xkb_کلید_type * ثابت type = کلید->groups[layout].type;
    xkb_mod_mask_t affect_mods = 0;
    xkb_mod_mask_t mods = 0;
    برای (darray_size_t m = 0; m < mappings->mappings_رقم; m++) {
        ثابت ساختار machine_mods_mapping * ثابت mapping =
            &mappings->mappings[m];
        اگر (/* Skip not matching active mods */
            (mapping->source & state->components.mods) == mapping->source &&
            /* Skip overlap with previous active mappings */
            !(mapping->source & affect_mods) &&
            /* Skip if the key type uses some of the source modifiers */
            !(mapping->source & type->mods.mask)) {
            affect_mods |= mappings->mappings[m].source;
            mods |= mappings->mappings[m].target;
        }
    }

    اگر (!affect_mods)
        بازگشت -1;

    ساختار xkb_state new = *state;
    new.components.base_mods = (new.components.base_mods & ~affect_mods) | mods;
    new.components.latched_mods = (new.components.latched_mods & ~affect_mods);
    new.components.locked_mods = (new.components.locked_mods & ~affect_mods);
    xkb_state_update_derived(&new);

    ssize_t event_idx = -1;
    ثابت شمارش xkb_state_component changed =
        get_state_component_changes(&state->components, &new.components);
    اگر (changed) {
        event_idx = (ssize_t) darray_size(events->queue);
        darray_append(events->queue, (ساختار xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = new.components,
                .changed = changed
            }
        });
    }

    state->components.mods = new.components.mods;
    بازگشت event_idx;
}

/**
 * Activate an alternative layout if some modifiers are active, typically
 * `Control`, `Alt` or `Super`.
 *
 * The tweak consists of computing a *base* layout such that the *effective*
 * layout is the corresponding target layout.
 */
ایستا ssize_t
do_shortcuts_tweak(ثابت ساختار machine_shortcuts_config *config,
                   ساختار xkb_state *state,
                   ثابت ساختار state_components *previous_components,
                   ساختار xkb_events *events, ssize_t remap_event)
{
    اگر (!config->entries) {
        /* No shortcuts tweak */
        بازگشت remap_event;
    }

    ثابت ساختار xkb_shortcuts_config_entry * ثابت entry =
        &config->entries[state->components.group];

    اگر (entry->target != XKB_LAYOUT_INVALID &&
        (entry->mods & state->components.mods)) {
        /*
         * Activate shortcuts tweak:
         * 1. The real base group is saved by the caller, to be restored by
         *    later by a call to undo_tweaks().
         * 2. Compute the new base group so that the resulting effective group
         *    is the target.
         * 3. Recompute derived state components.
         */
        ساختار xkb_state new = *state;
        اگر (remap_event < 0) {
            /* Create new event */
            remap_event = (ssize_t) darray_size(events->queue);
            darray_append(events->queue, (ساختار xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = {0},
                    .changed = 0
                }
            });
        } وگرنه {
            /* Merge with last remap event */
            new.components =
                darray_item(events->queue, remap_event).components.components;
        }
        new.components.base_group
            = (int32_t) entry->target
            - state->components.latched_group
            - state->components.locked_group;
        xkb_state_update_derived(&new);

        ثابت شمارش xkb_state_component changed =
            get_state_component_changes(previous_components, &new.components);
        darray_item(events->queue, remap_event).components.changed = changed;
        darray_item(events->queue, remap_event).components.components =
            new.components;

        state->components.group = new.components.group;
    } وگرنه {
        /* Deactivated entry or modifiers do not match */
    }
    بازگشت remap_event;
}

ایستا تهی
undo_tweaks(ثابت ساختار xkb_state *state,
            ثابت ساختار state_components *initial_components,
            ساختار xkb_events *events)
{
    /* Get last component event */
    ثابت ساختار xkb_event *event = پوچ;
    darray_foreach_reverse(event, events->queue) {
        اگر (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS)
            بشکن;
    }
    اگر (!event)
        بازگشت;

    /* Restore state */
    ثابت شمارش xkb_state_component changed =
        get_state_component_changes(initial_components,
                                    &event->components.components);
    اگر (changed) {
        darray_append(events->queue, (ساختار xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = *initial_components,
                .changed = changed
            }
        });
    }
}

ایستا ثابت ساختار xkb_کلید *
process_overlayable_کلید(ساختار xkb_machine *sm,
                        ثابت ساختار xkb_کلید * کلید,
                        شمارش xkb_کلید_direction direction)
{
    /* Check if key is currently overlaid */
    ساختار xkb_overlaid_کلید *entry = پوچ;
    ساختار xkb_overlaid_کلید *available_entry = پوچ;
    darray_foreach(entry, sm->overlays.کلیدها) {
        اگر (entry->old == کلید) {
            /* Maintain the key overlaid */
            انتخاب (direction) {
            حالت XKB_کلید_DOWN:
                entry->refcnt++;
                بشکن;
            حالت XKB_کلید_REPEATED:
                بشکن;
            حالت XKB_کلید_UP:
            پیشفرض:
                entry->refcnt--;
                بشکن;
            }
            اگر (entry->refcnt <= 0)
                entry->old = پوچ;
            بازگشت entry->new;
        } وگرنه اگر (!entry->old && !available_entry) {
            /* First free spot */
            available_entry = entry;
        }
    }
    اگر (direction == XKB_کلید_DOWN) {
        /* Enable key overlay */

        /*
         * Get overlay index: select the latest activated overlay
         * containing the key.
         */
        ثابت ساختار xkb_کلید *new = کلید;

        اگر (کلید->overlays & sm->overlays.enabled) {
            /* Some relevant overlay is active */
            برای (uint32_t پشته = sm->overlays.order; پشته; پشته >>= 4) {
                static_assert(XKB_OVERLAY_شمار == 8, "");
                ثابت xkb_overlay_index_t overlay = (پشته & 0xf) - 1;
                ثابت xkb_overlay_mask_t mask =
                    (xkb_overlay_mask_t)(1u << overlay);
                اگر (کلید->overlays & mask) {
                    /* Get new key */
                    اگر (کلید->overlays_inline) {
                        /* Stored inline */
                        new = کلید->overlay_کلید;
                    } وگرنه {
                        /* Stored in sparse array */
                        ثابت xkb_overlay_mask_t کم = (xkb_overlay_mask_t)(
                            کلید->overlays & (xkb_overlay_mask_t)(mask - 1u)
                        );
                        /* index = number of set bits strictly below bit */
                        ثابت xkb_overlay_index_t index =
                            (xkb_overlay_index_t)popcount32(کم);
                        new = کلید->overlays_کلیدها[index];
                    }
                    بشکن;
                }
            }
        } وگرنه {
            /*
             * No relevant overlay is active: block future overlays while
             * the key is pressed.
             */
        }

        /* Get new entry */
        اگر (available_entry) {
            /* Re-use a free entry */
            entry = available_entry;
        } وگرنه {
            /* Append a new entry */
            ثابت darray_size_t idx = darray_size(sm->overlays.کلیدها);
            darray_resize(sm->overlays.کلیدها, idx + 1);
            entry = &darray_item(sm->overlays.کلیدها, idx);
        }

        /* Overlay the key */
        entry->old = کلید;
        entry->new = new;
        entry->refcnt = 1;
        بازگشت new;
    }

    /* No overlay */
    بازگشت کلید;
}

شمارش xkb_status
xkb_machine_process_کلید(ساختار xkb_machine *sm,
                        xkb_keycode_t kc, شمارش xkb_کلید_direction direction,
                        ساختار xkb_events *events)
{
    darray_size(events->queue) = 0;
    events->next = 0;

    ساختار xkb_server_state * ثابت state = &sm->base;
    ثابت ساختار xkb_کلید * کلید = Xkb_کلید(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    اگر (!کلید || (direction == XKB_کلید_REPEATED && !کلید->repeats))
        بازگشت XKB_SUCCESS;

    ثابت ساختار state_components initial_components = state->base.components;

    اگر (direction == XKB_کلید_DOWN &&
        (sm->base.base.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیدها) &&
        (sm->base.a11y_flags & XKB_A11_Y_STICKY_کلیدها_NO_SIMULTANEOUS_کلیدها) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیدها,
                                      0, events);
    }

    اگر (کلید->overlays)
        کلید = process_overlayable_کلید(sm, کلید, direction);

    ssize_t remap_event = do_remap_modifiers(&sm->config.modifiers,
                                             &state->base, events, کلید);

    remap_event = do_shortcuts_tweak(&sm->config.shortcuts, &state->base,
                                     &initial_components, events, remap_event);

    اگر (remap_event >= 0) {
        // TODO: what about merging state event for RedirectKey?
        darray_append(events->queue, (ساختار xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    state->flags |= STATE_REQUIRE_کلید_EVENT;
    state->set_mods = 0;
    state->clear_mods = 0;

    /* Handle key actions */
    xkb_filter_apply_all(state, events, کلید, direction);

    xkb_mod_index_t i;
    xkb_mod_mask_t bit;
    برای (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        اگر (state->set_mods & bit) {
            state->mod_کلید_شمار[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    برای (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        اگر (state->clear_mods & bit) {
            state->mod_کلید_شمار[i]--;
            اگر (state->mod_کلید_شمار[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_کلید_شمار[i] = 0;
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
    اگر (state->flags & STATE_REQUIRE_کلید_EVENT) {
        darray_append(events->queue, (ساختار xkb_event) {
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
        undo_tweaks(&state->base, &initial_components, events);
    }

    ثابت شمارش xkb_state_component changed = get_state_component_changes(
        &initial_components, &state->base.components
    );
    اگر (changed) {
        اگر (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        // TODO: merge with previous state event, if any

        darray_append(events->queue, (ساختار xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = state->base.components,
                .changed = changed
            }
        });
    }

    اگر (!darray_empty(events->queue) &&
        darray_item(events->queue, darray_size(events->queue) - 1).type !=
        XKB_EVENT_TYPE_FRAME)
    {
        darray_append(events->queue, (ساختار xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    بازگشت XKB_SUCCESS;
}

ساختار xkb_events *
xkb_events_new(ساختار xkb_context * restrict context,
               ثابت ساختار xkb_events_config * restrict config,
               شمارش xkb_status * restrict status)
{
    /* Handle default configuration */
    ایستا ثابت ساختار xkb_events_config default_config = {
        .size = اندازه(default_config)
    };
    اگر (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ثابت شمارش xkb_status abi_error = xkb_check_state_abi(config);
    اگر (abi_error) {
        xkb_log_abi_error(context, __func__, abi_error);
        اگر (status)
            *status = abi_error;
        بازگشت پوچ;
    }

    /* Sanitize input */
    ثابت uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_EVENTS_FLAGS_مقادیر);
    اگر (invalid_flags) {
        log_err_func(context, XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS_,
                     "unrecognized events collection flags: 0x%"PRIx32"\n",
                     invalid_flags);
        اگر (status)
            *status = XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS;
        بازگشت پوچ;
    }

    ساختار xkb_events *events = calloc(1, اندازه(*events));
    اگر (!events) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events collection\n");
        اگر (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        بازگشت پوچ;
    }

    /* Pre-allocate event queue */
    darray_init(events->queue);
    ایستا ثابت darray_size_t queue_initial_size = 16;
    darray_resize(events->queue, queue_initial_size);
    // TODO: proper safe darray_resize() with return value
    اگر (!darray_اقلام(events->queue)) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events queue\n");
        xkb_events_destroy(events);
        اگر (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        بازگشت پوچ;
    }
    darray_size(events->queue) = 0;

    events->ctx = xkb_context_ref(context);
    events->next = 0;

    اگر (status)
        *status = XKB_SUCCESS;
    بازگشت events;
}

تهی
xkb_events_destroy(ساختار xkb_events *events)
{
    اگر (!events)
        بازگشت;
    darray_free(events->queue);
    xkb_context_unref(events->ctx);
    آزاد(events);
}

ثابت ساختار xkb_event *
xkb_events_next(ساختار xkb_events *events)
{
    اگر (events->next < darray_size(events->queue)) {
        ثابت darray_size_t index = events->next++;
        بازگشت &darray_item(events->queue, index);
    } وگرنه {
        بازگشت پوچ;
    }
}

شمارش xkb_event_type
xkb_event_get_type(ثابت ساختار xkb_event *event)
{
    بازگشت event->type;
}

شمارش xkb_status
xkb_event_get_keycode(ثابت ساختار xkb_event *event,
                      xkb_keycode_t *keycode,
                      شمارش xkb_کلید_direction *direction)
{
    اگر (event->type == XKB_EVENT_TYPE_کلید) {
        *keycode = event->کلید.keycode;
        *direction = event->کلید.direction;
        بازگشت XKB_SUCCESS;
    } وگرنه {
        بازگشت XKB_ERROR_EVENT_TYPE_MISMATCH;
    }
}

شمارش xkb_status
xkb_event_get_components(ثابت ساختار xkb_event * restrict event,
                         ساختار xkb_event_components * restrict components)
{
    اگر (event->type != XKB_EVENT_TYPE_STATE_COMPONENTS)
        بازگشت XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ثابت شمارش xkb_status status = xkb_check_state_abi(components);
    اگر (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        بازگشت status;
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

    بازگشت XKB_SUCCESS;
}

شمارش xkb_status
xkb_event_get_pointer_motion(ثابت ساختار xkb_event * restrict event,
                             ساختار xkb_event_pointer_motion * restrict motion)
{
    اگر (event->type != XKB_EVENT_TYPE_POINTER_MOTION)
        بازگشت XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ثابت شمارش xkb_status status = xkb_check_state_abi(motion);
    اگر (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        بازگشت status;
    }

    motion->flags = event->pointer_motion.flags;
    motion->x = event->pointer_motion.x;
    motion->y = event->pointer_motion.y;

    بازگشت XKB_SUCCESS;
}

شمارش xkb_status
xkb_event_get_pointer_button(ثابت ساختار xkb_event * restrict event,
                             ساختار xkb_event_pointer_button * restrict button)
{
    اگر (event->type != XKB_EVENT_TYPE_POINTER_BUTTON)
        بازگشت XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ثابت شمارش xkb_status status = xkb_check_state_abi(button);
    اگر (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        بازگشت status;
    }

    button->button = event->pointer_button.button;
    button->state = event->pointer_button.state;
    button->شمار = event->pointer_button.شمار;

    بازگشت XKB_SUCCESS;
}

شمارش xkb_status
xkb_event_get_virtual_console(ثابت ساختار xkb_event * restrict event,
                              int8_t * restrict index_or_offset,
                              bool * restrict is_offset)
{
    اگر (event->type != XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
        بازگشت XKB_ERROR_EVENT_TYPE_MISMATCH;

    *index_or_offset = event->virtual_console.index_or_offset;
    *is_offset = event->virtual_console.is_offset;

    بازگشت XKB_SUCCESS;
}

شمارش xkb_status
xkb_state_update_event(ساختار xkb_state * restrict base_state,
                       ثابت ساختار xkb_event * restrict event,
                       شمارش xkb_state_component * restrict changed)
{
    /* Guard against server-only state */
    أكد(base_state->mode == SERVER_COMPANION ||
           base_state->mode == LEGACY_MIXED_STATE);
    اگر (base_state->mode != SERVER_COMPANION &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE_,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        بازگشت XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    ساختار xkb_client_state * ثابت state =
        (ساختار xkb_client_state *)base_state;

    اگر (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
        ثابت ساختار state_components prev_components = state->base.components;
        state->base.components = event->components.components;
        /*
         * Recompute the changes instead of using the event value, because we do
         * not know if the event’s queue and the state are synced.
         */
        اگر (changed)
            *changed = get_state_component_changes(&prev_components,
                                                   &state->base.components);
    } وگرنه {
        اگر (changed)
            *changed = 0;
    }
    بازگشت XKB_SUCCESS;
}

ایستا تهی
append_pointer_button(ساختار xkb_events * restrict events,
                      ساختار xkb_filter * restrict filter,
                      شمارش xkb_pointer_button_state button_state,
                      uint8_t شمار)
{
    darray_append(events->queue, (ساختار xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_BUTTON,
        .pointer_button = {
            .size = اندازه(((ساختار xkb_event*)0)->pointer_button),
            .button = filter->priv,
            .state = button_state,
            .شمار = شمار,
        }
    });
}

ایستا تهی
xkb_filter_pointer_button_new(ساختار xkb_server_state *state,
                              ساختار xkb_events *events,
                              ساختار xkb_filter *filter)
{
    اگر (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_کلیدها)) {
        filter->func = پوچ;
        بازگشت;
    }

    ساختار xkb_machine * ثابت sm = (ساختار xkb_machine *)state;

    /* Resolve button */
    ثابت xkb_pointer_button_index_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;
    filter->priv = button;

    ثابت شمارش xkb_pointer_button_state button_state =
        XKB_POINTER_BUTTON_PRESSED;
    append_pointer_button(events, filter, button_state,
                          filter->action.btn.شمار);
    state->flags &= ~STATE_REQUIRE_کلید_EVENT;
}

ایستا bool
xkb_filter_pointer_button_func(ساختار xkb_server_state *state,
                               ساختار xkb_events *events,
                               ساختار xkb_filter *filter,
                               ثابت ساختار xkb_کلید *کلید,
                               شمارش xkb_کلید_direction direction)
{
    اگر (کلید != filter->کلید || !events)
        بازگشت XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    انتخاب (direction) {
    حالت XKB_کلید_DOWN:
        filter->refcnt++;
        /* fallthrough */
    حالت XKB_کلید_REPEATED:
        بازگشت XKB_FILTER_CONSUME;
    پیشفرض:
        اگر (--filter->refcnt > 0)
            بازگشت XKB_FILTER_CONSUME;
    }

    اگر (!filter->action.btn.شمار)
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);

    filter->func = پوچ;
    بازگشت XKB_FILTER_CONSUME;
}

ساختار xkb_filter_pointer_lock_button_priv {
    uint32_t button:31;
#ifdef _WIN32
    uint32_t locked:1; // MSVC requires matching types to pack bitfields
#وگرنه
    bool locked:1;
#endif
};

شمارش {
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH =
        اندازه(ساختار xkb_filter_pointer_lock_button_priv) * CHAR_BIT,
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX =
        (UINT64_C(1) << XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH) - 1
};

static_assert(اندازه(ساختار xkb_filter_pointer_lock_button_priv) ==
              اندازه(((ساختار xkb_filter*)0)->priv),
              "");
static_assert((uint32_t)XKB_POINTER_BUTTON_MAX < (uint32_t)XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX,
              "xkb_filter_pointer_lock_button_priv cannot store button");

ایستا تهی
xkb_filter_pointer_lock_button_new(ساختار xkb_server_state *state,
                                   ساختار xkb_events *events,
                                   ساختار xkb_filter *filter)
{
    اگر (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_کلیدها)) {
        filter->func = پوچ;
        بازگشت;
    }

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    ساختار xkb_machine * ثابت sm = (ساختار xkb_machine *)state;

    /* Resolve button */
    ثابت xkb_pointer_button_index_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;

    static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                  "invalid UINTx_C() macro");
    static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                  "invalid left shift");
    ثابت xkb_pointer_button_mask_t mask = (UINT8_C(1) << button);

    ثابت bool locked = (sm->mouse.locked_buttons & mask);

    اگر (!locked && !(filter->action.btn.flags & ACTION_LOCK_NO_LOCK)) {
        sm->mouse.locked_buttons |= mask;
        filter->priv = button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_PRESSED, 0);
    }

    ساختار xkb_filter_pointer_lock_button_priv *priv =
        (ساختار xkb_filter_pointer_lock_button_priv*)&filter->priv;
    priv->button = button,
    priv->locked = locked;
}

ایستا bool
xkb_filter_pointer_lock_button_func(ساختار xkb_server_state *state,
                                    ساختار xkb_events *events,
                                    ساختار xkb_filter *filter,
                                    ثابت ساختار xkb_کلید *کلید,
                                    شمارش xkb_کلید_direction direction)
{
    اگر (کلید != filter->کلید || !events || state->base.mode != SERVER_STATE)
        بازگشت XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    انتخاب (direction) {
    حالت XKB_کلید_DOWN:
        filter->refcnt++;
        /* fallthrough */
    حالت XKB_کلید_REPEATED:
        بازگشت XKB_FILTER_CONSUME;
    پیشفرض:
        اگر (--filter->refcnt > 0)
            بازگشت XKB_FILTER_CONSUME;
    }

    ثابت ساختار xkb_filter_pointer_lock_button_priv *priv =
        (ساختار xkb_filter_pointer_lock_button_priv*)&filter->priv;

    اگر (priv->locked && !(filter->action.btn.flags & ACTION_LOCK_NO_UNLOCK)) {
        ساختار xkb_machine * ثابت sm = (ساختار xkb_machine *)state;

        static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                    "invalid UINTx_C() macro");
        static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                    "invalid left shift");
        ثابت xkb_pointer_button_mask_t mask = (UINT8_C(1) << priv->button);

        sm->mouse.locked_buttons &= ~mask;
        filter->priv = priv->button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);
    }

    filter->func = پوچ;
    بازگشت XKB_FILTER_CONSUME;
}

ایستا تهی
xkb_filter_pointer_set_default_button_new(ساختار xkb_server_state *state,
                                          ساختار xkb_events *events,
                                          ساختار xkb_filter *filter)
{
    اگر (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_کلیدها)) {
        filter->func = پوچ;
        بازگشت;
    }

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    ساختار xkb_machine * ثابت sm = (ساختار xkb_machine *)state;

    static_assert(2 * XKB_POINTER_BUTTON_MAX <= INT_MAX, "lossy conversion");
    صحیح button = (filter->action.dflt.flags & ACTION_ABSOLUTE_SWITCH)
        ? filter->action.dflt.مقدار
        : sm->mouse.default_button + filter->action.dflt.مقدار;

    /* Wrap into range */
    ثابت صحیح rem = button % XKB_POINTER_BUTTON_MAX;
    sm->mouse.default_button = (rem >= 0)
        ? (xkb_pointer_button_index_t)rem
        : (xkb_pointer_button_index_t)(rem + XKB_POINTER_BUTTON_MAX);
}

ایستا bool
xkb_filter_pointer_set_default_button_func(ساختار xkb_server_state *state,
                                           ساختار xkb_events *events,
                                           ساختار xkb_filter *filter,
                                           ثابت ساختار xkb_کلید *کلید,
                                           شمارش xkb_کلید_direction direction)
{
    اگر (کلید != filter->کلید || !events)
        بازگشت XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    انتخاب (direction) {
    حالت XKB_کلید_DOWN:
        filter->refcnt++;
        /* fallthrough */
    حالت XKB_کلید_REPEATED:
        بازگشت XKB_FILTER_CONSUME;
    پیشفرض:
        اگر (--filter->refcnt > 0)
            بازگشت XKB_FILTER_CONSUME;
    }

    filter->func = پوچ;
    بازگشت XKB_FILTER_CONTINUE;
}

ایستا تهی
xkb_filter_terminate_server_new(ساختار xkb_server_state *state,
                                 ساختار xkb_events *events,
                                 ساختار xkb_filter *filter)
{
    اگر (!events || state->base.mode != SERVER_STATE ||
        !(((ساختار xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = پوچ;
        بازگشت;
    }

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    darray_append(events->queue, (ساختار xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    });
}

ایستا bool
xkb_filter_terminate_server_func(ساختار xkb_server_state *state,
                                 ساختار xkb_events *events,
                                 ساختار xkb_filter *filter,
                                 ثابت ساختار xkb_کلید *کلید,
                                 شمارش xkb_کلید_direction direction)
{
    اگر (کلید != filter->کلید || !events)
        بازگشت XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    انتخاب (direction) {
    حالت XKB_کلید_DOWN:
        filter->refcnt++;
        /* fallthrough */
    حالت XKB_کلید_REPEATED:
        بازگشت XKB_FILTER_CONSUME;
    پیشفرض:
        اگر (--filter->refcnt > 0)
            بازگشت XKB_FILTER_CONSUME;
    }

    filter->func = پوچ;
    بازگشت XKB_FILTER_CONSUME;
}

ایستا تهی
xkb_filter_switch_virtual_console_new(ساختار xkb_server_state *state,
                                      ساختار xkb_events *events,
                                      ساختار xkb_filter *filter)
{
    اگر (!events || state->base.mode != SERVER_STATE ||
        !(((ساختار xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = پوچ;
        بازگشت;
    }

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    darray_append(events->queue, (ساختار xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
        .virtual_console = {
            .index_or_offset = filter->action.screen.screen,
            .is_offset = !(filter->action.screen.flags & ACTION_ABSOLUTE_SWITCH)
        }
    });
}

ایستا bool
xkb_filter_switch_virtual_console_func(ساختار xkb_server_state *state,
                                       ساختار xkb_events *events,
                                       ساختار xkb_filter *filter,
                                       ثابت ساختار xkb_کلید *کلید,
                                       شمارش xkb_کلید_direction direction)
{
    اگر (کلید != filter->کلید || !events)
        بازگشت XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    انتخاب (direction) {
    حالت XKB_کلید_DOWN:
        filter->refcnt++;
        /* fallthrough */
    حالت XKB_کلید_REPEATED:
        بازگشت XKB_FILTER_CONSUME;
    پیشفرض:
        اگر (--filter->refcnt > 0)
            بازگشت XKB_FILTER_CONSUME;
    }

    filter->func = پوچ;
    بازگشت XKB_FILTER_CONSUME;
}
