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
    darray(بنية xkb_event) queue;
    /**
     * Read cursor for `xkb_events_next()`.
     *
     * Reset to 0 on each `process_*` call.
     */
    darray_size_t next;
};

بنية xkb_server_state;

بنية xkb_filter {
    اتحاد xkb_action action;
    ثابت بنية xkb_key *key;
    bool (*func)(بنية xkb_server_state *state,
                 بنية xkb_events *events,
                 بنية xkb_filter *filter,
                 ثابت بنية xkb_key *key,
                 تعداد xkb_key_direction direction);
    uint32_t priv;
    صحيح refcnt;
};

تعداد xkb_state_mode_internal {
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

تعداد { XKB_LAYOUT_INDEX_T_MIN_WIDTH = 6 };
static_assert(XKB_MAX_GROUPS < (1 << XKB_LAYOUT_INDEX_T_MIN_WIDTH),
              "Cannot encode out_of_range_group::redirect_group");

بنية out_of_range_group {
    تعداد xkb_layout_out_of_range_policy policy :
        (حجم(تعداد xkb_layout_out_of_range_policy) * CHAR_BIT -
         XKB_LAYOUT_INDEX_T_MIN_WIDTH);
    xkb_layout_index_t redirect_group : XKB_LAYOUT_INDEX_T_MIN_WIDTH;
};

تعداد { XKB_STATE_MODE_INTERNAL_MIN_WIDTH = 4 /* 3 bits + sign */ };
static_assert(_XKB_STATE_TYPE_NUM_ENTRIES <
              (1 << XKB_STATE_MODE_INTERNAL_MIN_WIDTH),
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
    بنية out_of_range_group out_of_range_group;

    تعداد xkb_state_mode_internal mode : XKB_STATE_MODE_INTERNAL_MIN_WIDTH;
    صحيح refcnt : (حجم(صحيح) * CHAR_BIT - XKB_STATE_MODE_INTERNAL_MIN_WIDTH);
};

/**
 * Minimal client state
 *
 * Wrapper for strong typing
 */
بنية xkb_client_state {
    /** Inherits from `xkb_state` */
    بنية xkb_state base;
};

تعداد state_flags {
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
بنية xkb_server_state {
    /** Inherits from `xkb_state` */
    بنية xkb_state base;

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

ساكن ثابت بنية xkb_key_type_entry *
get_entry_for_mods(ثابت بنية xkb_key_type *type, xkb_mod_mask_t mods)
{
    עבור (darray_size_t i = 0; i < type->num_entries; i++)
        אם (entry_is_active(&type->entries[i]) &&
            type->entries[i].mods.mask == mods)
            החזר &type->entries[i];
    החזר عدم;
}

ساكن ثابت بنية xkb_key_type_entry *
get_entry_for_key_state(بنية xkb_state *state, ثابت بنية xkb_key *key,
                        xkb_layout_index_t group)
{
    ثابت بنية xkb_key_type* ثابت type = key->groups[group].type;
    xkb_mod_mask_t active_mods = state->components.mods & type->mods.mask;
    החזר get_entry_for_mods(type, active_mods);
}

ساكن مضمن xkb_level_index_t
state_key_get_level(بنية xkb_state *state, ثابت بنية xkb_key *key,
                         xkb_layout_index_t layout)
{
    אם (layout >= key->num_groups)
        החזר XKB_LEVEL_INVALID;

    /* If we don't find an explicit match the default is 0. */
    ثابت بنية xkb_key_type_entry* ثابت entry =
        get_entry_for_key_state(state, key, layout);

    החזר (entry) ? entry->level : 0;
}

/**
 * Returns the level to use for the given key and state, or
 * XKB_LEVEL_INVALID.
 */
xkb_level_index_t
xkb_state_key_get_level(بنية xkb_state *state, xkb_keycode_t kc,
                        xkb_layout_index_t layout)
{
    ثابت بنية xkb_key* ثابت key = XkbKey(state->keymap, kc);

    החזר (key)
        ? state_key_get_level(state, key, layout)
        : XKB_LEVEL_INVALID;
}

ساكن مضمن xkb_layout_index_t
state_key_get_layout(بنية xkb_state *state, ثابت بنية xkb_key *key)
{
    החזר xkb_keymap_key_effective_layout(key, state->components.group);
}

/**
 * Returns the layout to use for the given key and state, taking
 * wrapping/clamping/etc into account, or XKB_LAYOUT_INVALID.
 */
xkb_layout_index_t
xkb_state_key_get_layout(بنية xkb_state *state, xkb_keycode_t kc)
{
    ثابت بنية xkb_key* ثابت key = XkbKey(state->keymap, kc);

    אם (!key)
        החזר XKB_LAYOUT_INVALID;

    החזר state_key_get_layout(state, key);
}

/* Empty action used for empty levels */
ساكن ثابت اتحاد xkb_action dummy_action = { .type = ACTION_TYPE_NONE };

ساكن xkb_action_count_t
xkb_key_get_actions(بنية xkb_state *state, ثابت بنية xkb_key *key,
                    ثابت اتحاد xkb_action **actions)
{
    ثابت xkb_layout_index_t layout = state_key_get_layout(state, key);
    ثابت xkb_level_index_t level = state_key_get_level(state, key, layout);
    אם (level == XKB_LEVEL_INVALID)
        اذهب err;

    ثابت xkb_action_count_t count =
        xkb_keymap_key_get_actions_by_level(state->keymap, key,
                                            layout, level, actions);
    אם (!count)
        اذهب err;

    החזר count;

err:
    /* Use a dummy action if no corresponding level was found or if it is empty.
     * This is required e.g. to handle latches properly. */
    *actions = &dummy_action;
    החזר 1;
}

ساكن بنية xkb_filter *
xkb_filter_new(بنية xkb_server_state *state)
{
    بنية xkb_filter *filter = عدم, *iter;

    darray_foreach(iter, state->filters) {
        אם (iter->func)
            استمر;
        /* Use available slot */
        filter = iter;
        اكسر;
    }

    אם (!filter) {
        /* No available slot: resize the filters array */
        darray_resize0(state->filters, darray_size(state->filters) + 1);
        filter = &darray_item(state->filters, darray_size(state->filters) -1);
    }

    filter->refcnt = 1;
    החזר filter;
}

/***====================================================================***/

تعداد xkb_filter_result {
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
    אם ((filter_)->action.group.flags & ACTION_ABSOLUTE_SWITCH)          \
        (state_)->components.component_ = (filter_)->action.group.group; \
    אחרת                                                                 \
        (state_)->components.component_ += (filter_)->action.group.group

ساكن فراغ
xkb_filter_group_set_new(بنية xkb_server_state *state,
                         بنية xkb_events *events,
                         بنية xkb_filter *filter)
{
    static_assert(حجم(state->base.components.base_group) ==
                  حجم(filter->priv),
                  "Max groups don't fit");
    filter->priv = (uint32_t)state->base.components.base_group;
    apply_group_delta(filter, &state->base, base_group);
}

ساكن bool
xkb_filter_group_set_func(بنية xkb_server_state *state,
                          بنية xkb_events *events,
                          بنية xkb_filter *filter,
                          ثابت بنية xkb_key *key,
                          تعداد xkb_key_direction direction)
{
    אם (key != filter->key) {
        filter->action.group.flags &= ~ACTION_LOCK_CLEAR;
        החזר XKB_FILTER_CONTINUE;
    }

    بدل (direction) {
    حالة XKB_KEY_DOWN:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_KEY_REPEATED:
        החזר XKB_FILTER_CONSUME;
    افتراضي:
        אם (--filter->refcnt > 0)
            החזר XKB_FILTER_CONSUME;
    }

    static_assert(حجم(state->base.components.base_group) == حجم(filter->priv),
                  "Max groups don't fit");
    state->base.components.base_group = (int32_t) filter->priv;

    אם (filter->action.group.flags & ACTION_LOCK_CLEAR)
        state->base.components.locked_group = 0;

    filter->func = عدم;
    החזר XKB_FILTER_CONTINUE;
}

ساكن تعداد xkb_state_component
get_state_component_changes(ثابت بنية state_components *a,
                            ثابت بنية state_components *b)
{
    تعداد xkb_state_component mask = 0;

    אם (a->group != b->group)
        mask |= XKB_STATE_LAYOUT_EFFECTIVE;
    אם (a->base_group != b->base_group)
        mask |= XKB_STATE_LAYOUT_DEPRESSED;
    אם (a->latched_group != b->latched_group)
        mask |= XKB_STATE_LAYOUT_LATCHED;
    אם (a->locked_group != b->locked_group)
        mask |= XKB_STATE_LAYOUT_LOCKED;
    אם (a->mods != b->mods)
        mask |= XKB_STATE_MODS_EFFECTIVE;
    אם (a->base_mods != b->base_mods)
        mask |= XKB_STATE_MODS_DEPRESSED;
    אם (a->latched_mods != b->latched_mods)
        mask |= XKB_STATE_MODS_LATCHED;
    אם (a->locked_mods != b->locked_mods)
        mask |= XKB_STATE_MODS_LOCKED;
    אם (a->leds != b->leds)
        mask |= XKB_STATE_LEDS;
    אם (a->controls != b->controls)
        mask |= XKB_STATE_CONTROLS_EFFECTIVE;

    החזר mask;
}

ساكن فراغ
xkb_filter_group_lock_new(بنية xkb_server_state *state,
                          بنية xkb_events *events,
                          بنية xkb_filter *filter)
{
    אם (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release: do nothing on key press.
         *
         * This is a keymap format v2 extension.
         */
        החזר;
    } אחרת {
        /* Lock on key press */
        apply_group_delta(filter, &state->base, locked_group);
    }
}

ساكن bool
xkb_filter_group_lock_func(بنية xkb_server_state *state,
                           بنية xkb_events *events,
                           بنية xkb_filter *filter,
                           ثابت بنية xkb_key *key,
                           تعداد xkb_key_direction direction)
{
    אם (key != filter->key) {
        אם ((filter->action.group.flags & ACTION_LOCK_ON_RELEASE) &&
            direction == XKB_KEY_DOWN) {
            /*
             * Another key has been pressed after the locking key:
             * cancel group lock on release.
             *
             * This is a keymap v2 extension.
             */
            filter->action.group.flags &= ~ACTION_LOCK_ON_RELEASE;
        }
        החזר XKB_FILTER_CONTINUE;
    }

    بدل (direction) {
    حالة XKB_KEY_DOWN:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_KEY_REPEATED:
        החזר XKB_FILTER_CONSUME;
    افتراضي:
        אם (--filter->refcnt > 0)
            החזר XKB_FILTER_CONSUME;
    }

    אם (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release
         *
         * This is a keymap v2 extension.
         */
        apply_group_delta(filter, &state->base, locked_group);
    } אחרת {
        /* Lock on key press: do nothing on key release. */
    }
    filter->func = عدم;
    החזר XKB_FILTER_CONTINUE;
}

ساكن bool
xkb_action_breaks_latch(ثابت اتحاد xkb_action *action,
                        تعداد xkb_action_controls controls,
                        تعداد xkb_internal_action_flags flag,
                        xkb_mod_mask_t mask)
{
    بدل (action->type) {
    حالة ACTION_TYPE_NONE:
    حالة ACTION_TYPE_VOID:
    حالة ACTION_TYPE_PTR_BUTTON:
    حالة ACTION_TYPE_PTR_LOCK:
    حالة ACTION_TYPE_CTRL_SET:
    حالة ACTION_TYPE_CTRL_LOCK:
    حالة ACTION_TYPE_SWITCH_VT:
    حالة ACTION_TYPE_TERMINATE:
    حالة ACTION_TYPE_REDIRECT_KEY:
        החזר true;
    حالة ACTION_TYPE_PTR_MOVE:
    حالة ACTION_TYPE_PTR_DEFAULT:
        החזר !(controls & CONTROL_MOUSE_KEYS);
    حالة ACTION_TYPE_INTERNAL:
        החזר (action->internal.flags & flag) &&
               ((action->internal.clear_latched_mods & mask) == mask);
    افتراضي:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == _ACTION_TYPE_NUM_ENTRIES,
                      "Missing action type");
        החזר false;
    }
}

تعداد xkb_key_latch_state {
    NO_LATCH = 0,
    LATCH_KEY_DOWN,
    LATCH_PENDING,
    _KEY_LATCH_STATE_NUM_ENTRIES
};

#define MAX_XKB_KEY_LATCH_STATE_LOG2 2
#אם (_KEY_LATCH_STATE_NUM_ENTRIES > (1 << MAX_XKB_KEY_LATCH_STATE_LOG2)) || \
    (-XKB_MAX_GROUPS) < (INT32_MIN >> MAX_XKB_KEY_LATCH_STATE_LOG2) || \
      XKB_MAX_GROUPS > (INT32_MAX >> MAX_XKB_KEY_LATCH_STATE_LOG2)
#error "Cannot represent priv field of the group latch filter"
#endif

/* Hold the latch state *and* the group delta */
اتحاد group_latch_priv {
    uint32_t priv;
    بنية {
        /* The type is really: enum xkb_key_latch_state, but it is problematic
         * on Windows, because it is interpreted as signed and leads to wrong
         * negative values. */
        unsigned صحيح latch:MAX_XKB_KEY_LATCH_STATE_LOG2;
        int32_t group_delta:(32 - MAX_XKB_KEY_LATCH_STATE_LOG2);
    };
};

ساكن فراغ
xkb_filter_group_latch_new(بنية xkb_server_state *state,
                           بنية xkb_events *events,
                           بنية xkb_filter *filter)
{
    ثابت اتحاد group_latch_priv priv = {
        .latch = LATCH_KEY_DOWN,
        .group_delta = (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)
            ? filter->action.group.group - state->base.components.base_group
            : filter->action.group.group
    };
    filter->priv = priv.priv;
    /* Like group set */
    apply_group_delta(filter, &state->base, base_group);
}

ساكن bool
xkb_filter_group_latch_func(بنية xkb_server_state *state,
                            بنية xkb_events *events,
                            بنية xkb_filter *filter,
                            ثابت بنية xkb_key *key,
                            تعداد xkb_key_direction direction)
{
    اتحاد group_latch_priv priv = {.priv = filter->priv};
    تعداد xkb_key_latch_state latch = priv.latch;

    אם (direction == XKB_KEY_DOWN) {
        ثابت اتحاد xkb_action *actions = عدم;
        ثابت xkb_action_count_t count = xkb_key_get_actions(&state->base, key,
                                                             &actions);

        אם (latch == LATCH_KEY_DOWN) {
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
            אם (state->a11y_flags & XKB_A11Y_LATCH_SIMULTANEOUS_KEYS) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                עבור (xkb_action_count_t k = 0; k < count; k++) {
                    אם (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_GROUP_LATCH,
                                                0))
                    {
                        latch = NO_LATCH;
                        اكسر;
                    }
                }
            } אחרת {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        אחרת אם (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is
             * pressed, or promote it to a lock if it's the same group delta &
             * flags and latchToLock option is enabled. */
            ثابت bool sticky_keys = state->base.components.controls
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
            עבור (xkb_action_count_t k = 0; k < count; k++) {
                אם (((actions[k].type == ACTION_TYPE_GROUP_LATCH) ||
                     (actions[k].type == ACTION_TYPE_GROUP_SET && sticky_keys)) &&
                     actions[k].group.group == filter->action.group.group &&
                     (actions[k].group.flags & ACTION_ABSOLUTE_SWITCH) ==
                     (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)) {
                    אם (filter->action.group.flags & ACTION_LATCH_TO_LOCK &&
                        filter->action.group.group != 0)
                    {
                        /* Promote to lock */
                        filter->action.type = ACTION_TYPE_GROUP_LOCK;
                        filter->func = xkb_filter_group_lock_func;
                        xkb_filter_group_lock_new(state, events, filter);
                        state->base.components.latched_group -= priv.group_delta;
                        filter->key = key;
                        /* XXX beep beep! */
                        החזר XKB_FILTER_CONSUME;
                    }
                    /* Do nothing if `latchToLock` option is not activated; if
                     * the latch is not broken by the following actions and the
                     * key is not consumed, then another latch filter will be
                     * created.
                     */
                    استمر;
                }
                אחרת אם (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_GROUP_LATCH,
                                                 0))
                {
                    /* Breaks the latch */
                    state->base.components.latched_group -= priv.group_delta;
                    filter->func = عدم;
                    החזר XKB_FILTER_CONTINUE;
                }
            }
        }
        אחרת {
            /* Ignore press in NO_LATCH state */
            أكد(latch == NO_LATCH);
        }
    }
    אחרת אם (key != filter->key) {
        /* Ignore repeat/release of other keys */
    }
    אחרת אם (direction == XKB_KEY_REPEATED) {
        החזר XKB_FILTER_CONSUME; // TODO: check
    }
    אחרת {
        /* Our key got released.  If we've set it to clear locks, and we
         * currently have a group locked, then release it and
         * don't actually latch.  Else we've actually hit the latching
         * stage, so set PENDING and move our group from base to
         * latched. */
        אם ((filter->action.group.flags & ACTION_LOCK_CLEAR) &&
             state->base.components.locked_group) {
            אם (latch == LATCH_PENDING)
                state->base.components.latched_group -= priv.group_delta;
            אחרת
                state->base.components.base_group -= priv.group_delta;
            state->base.components.locked_group = 0;
            filter->func = عدم;
        }
        /* Broken latch */
        אחרת אם (latch == NO_LATCH) {
            state->base.components.base_group -= priv.group_delta;
            filter->func = عدم;
        }
        /* We may already have reached the latch state if pressing the
         * key multiple times without latch-to-lock enabled. */
        אחרת אם (latch == LATCH_KEY_DOWN) {
            latch = LATCH_PENDING;
            /* Switch from set to latch */
            state->base.components.base_group -= priv.group_delta;
            state->base.components.latched_group += priv.group_delta;
            /* XXX beep beep! */
        }
    }

    priv.latch = latch;
    filter->priv = priv.priv;

    החזר XKB_FILTER_CONTINUE;
}

ساكن فراغ
xkb_filter_mod_set_new(بنية xkb_server_state *state,
                       بنية xkb_events *events,
                       بنية xkb_filter *filter)
{
    ثابت تعداد xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    אם ((filter->action.mods.flags & unlock) == unlock) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension.
         */
        filter->priv = filter->action.mods.mods.mask
                     & ~state->base.components.locked_mods;
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
    } אחרת {
        filter->priv = filter->action.mods.mods.mask;
    }

    state->set_mods |= (xkb_mod_mask_t) filter->priv;
}

ساكن bool
xkb_filter_mod_set_func(بنية xkb_server_state *state,
                        بنية xkb_events *events,
                        بنية xkb_filter *filter,
                        ثابت بنية xkb_key *key,
                        تعداد xkb_key_direction direction)
{

    אם (key != filter->key) {
        filter->action.mods.flags &= ~ACTION_LOCK_CLEAR;
        החזר XKB_FILTER_CONTINUE;
    }

    بدل (direction) {
    حالة XKB_KEY_DOWN:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_KEY_REPEATED:
        החזר XKB_FILTER_CONSUME;
    افتراضي:
        אם (--filter->refcnt > 0)
            החזר XKB_FILTER_CONSUME;
    }

    state->clear_mods |= (xkb_mod_mask_t) filter->priv;
    ثابت تعداد xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    אם ((filter->action.mods.flags & unlock) == ACTION_LOCK_CLEAR)
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;

    filter->func = عدم;
    החזר XKB_FILTER_CONTINUE;
}

ساكن فراغ
xkb_filter_mod_lock_new(بنية xkb_server_state *state,
                        بنية xkb_events *events,
                        بنية xkb_filter *filter)
{
    filter->priv = (state->base.components.locked_mods &
                    filter->action.mods.mods.mask);

    אם (filter->priv && (filter->action.mods.flags & ACTION_UNLOCK_ON_PRESS)) {
        /*
         * Some of the target modifiers were locked before key press: unlock.
         *
         * This is a keymap v2 extension: unlock-on-press.
         */
        אם (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
            state->base.components.locked_mods &= ~filter->priv;
        /* No further action: cancel filter */
        filter->func = عدم;
    } אחרת {
        /* Set base mods; lock mods if relevant (XKB 1.0 spec) */
        state->set_mods |= filter->action.mods.mods.mask;
        אם (!(filter->action.mods.flags & ACTION_LOCK_NO_LOCK))
            state->base.components.locked_mods |= filter->action.mods.mods.mask;
    }
}

ساكن bool
xkb_filter_mod_lock_func(بنية xkb_server_state *state,
                         بنية xkb_events *events,
                         بنية xkb_filter *filter,
                         ثابت بنية xkb_key *key,
                         تعداد xkb_key_direction direction)
{
    אם (key != filter->key)
        החזר XKB_FILTER_CONTINUE;

    بدل (direction) {
    حالة XKB_KEY_DOWN:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_KEY_REPEATED:
        החזר XKB_FILTER_CONSUME;
    افتراضي:
        אם (--filter->refcnt > 0)
            החזר XKB_FILTER_CONSUME;
    }

    state->clear_mods |= filter->action.mods.mods.mask;
    אם (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
        state->base.components.locked_mods &= ~filter->priv;

    filter->func = عدم;
    החזר XKB_FILTER_CONTINUE;
}

ساكن فراغ
xkb_filter_mod_latch_new(بنية xkb_server_state *state,
                         بنية xkb_events *events,
                         بنية xkb_filter *filter)
{
    /* Latch-on-press + clear-locks imply unlock-on-press */
    ثابت تعداد xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                              | ACTION_LATCH_ON_PRESS;

    אם ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
        (filter->action.mods.flags & unlockOnPress) &&
        (state->base.components.locked_mods & filter->action.mods.mods.mask) ==
         filter->action.mods.mods.mask) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension: clear locks and do not latch.
         */
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
        filter->func = عدم;
    } אחרת אם (filter->action.mods.flags & ACTION_LATCH_ON_PRESS) {
        /*
         * Latch on key press
         *
         * This is a keymap format v2 extension.
         */
        filter->priv = LATCH_PENDING;
        state->base.components.latched_mods |= filter->action.mods.mods.mask;
        /* XXX beep beep! */
    } אחרת {
        /* XKB standard latch action */
        filter->priv = LATCH_KEY_DOWN;
        state->set_mods |= filter->action.mods.mods.mask;
    }
}

ساكن bool
xkb_filter_mod_latch_func(بنية xkb_server_state *state,
                          بنية xkb_events *events,
                          بنية xkb_filter *filter,
                          ثابت بنية xkb_key *key,
                          تعداد xkb_key_direction direction)
{
    تعداد xkb_key_latch_state latch = filter->priv;

    אם (direction == XKB_KEY_DOWN) {
        ثابت اتحاد xkb_action *actions = عدم;
        ثابت xkb_action_count_t count = xkb_key_get_actions(&state->base, key,
                                                             &actions);

        אם (latch == LATCH_KEY_DOWN) {
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
            אם (state->a11y_flags & XKB_A11Y_LATCH_SIMULTANEOUS_KEYS) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                עבור (xkb_action_count_t k = 0; k < count; k++) {
                    אם (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_MOD_LATCH,
                                                filter->action.mods.mods.mask))
                    {
                        latch = NO_LATCH;
                        اكسر;
                    }
                }
            } אחרת {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        אחרת אם (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is pressed,
             * or promote it to a lock or plain base set if it's the same
             * modifier. */
            ثابت bool sticky_keys = state->base.components.controls
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
            עבור (xkb_action_count_t k = 0; k < count; k++) {
                אם (((actions[k].type == ACTION_TYPE_MOD_LATCH) ||
                     (actions[k].type == ACTION_TYPE_MOD_SET && sticky_keys)) &&
                    actions[k].mods.mods.mask == filter->action.mods.mods.mask)
                {
                    אם (filter->action.mods.flags & ACTION_LATCH_TO_LOCK) {
                        /* Mutate the action to LockMods() */
                        filter->action.type = ACTION_TYPE_MOD_LOCK;
                        filter->func = xkb_filter_mod_lock_func;
                        xkb_filter_mod_lock_new(state, events, filter);
                    }
                    אחרת {
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
                    החזר XKB_FILTER_CONSUME;
                }
                אחרת אם (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_MOD_LATCH,
                                                 filter->action.mods.mods.mask))
                {
                    /* XXX: This may be totally broken, we might need to break the
                     *      latch in the next run after this press? */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    filter->func = عدم;
                    החזר XKB_FILTER_CONTINUE;
                }
            }
        }
        אחרת {
            /* Ignore press in NO_LATCH state */
            أكد(latch == NO_LATCH);
        }
    }
    אחרת אם (key != filter->key) {
        /* Ignore repeat/release of other keys */
    }
    אחרת אם (direction == XKB_KEY_REPEATED) {
        החזר XKB_FILTER_CONSUME; // TODO: check
    }
    אחרת {
        /* Our key got released.  If we’ve set it to clear locks, and we
         * currently have the same modifiers locked, then release them and
         * don't actually latch.  Else we’ve actually hit the latching
         * stage, so set PENDING and move our modifier from base to
         * latched. */

        /* Latch-on-press + clear-locks imply unlock-on-press */
        ثابت تعداد xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                                  | ACTION_LATCH_ON_PRESS;

        אם ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
            !(filter->action.mods.flags & unlockOnPress) &&
            (state->base.components.locked_mods &
             filter->action.mods.mods.mask) == filter->action.mods.mods.mask)
        {
            /* XXX: We might be a bit overenthusiastic about clearing
             *      mods other filters have set here? */
            אם (latch == LATCH_PENDING)
                state->base.components.latched_mods &=
                    ~filter->action.mods.mods.mask;
            אחרת
                state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
            filter->func = عدم;
        }
        אחרת אם (latch == NO_LATCH) {
            /* Broken latch */
            state->clear_mods |= filter->action.mods.mods.mask;
            filter->func = عدم;
        }
        אחרת אם (!(filter->action.mods.flags & ACTION_LATCH_ON_PRESS)) {
            latch = LATCH_PENDING;
            state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.latched_mods |= filter->action.mods.mods.mask;
            /* XXX beep beep! */
        }
    }

    filter->priv = latch;

    החזר XKB_FILTER_CONTINUE;
}

ساكن فراغ
append_pointer_move(بنية xkb_events *events,
                    بنية xkb_filter *filter)
{
    تعداد xkb_pointer_motion_flags flags = 0;
    אם (filter->action.ptr.flags & ACTION_ABSOLUTE_X)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_X;
    אם (filter->action.ptr.flags & ACTION_ABSOLUTE_Y)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_Y;
    אם (filter->action.ptr.flags & ACTION_REPEAT)
        flags |= XKB_POINTER_MOTION_REPEATS;
    darray_append(events->queue, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_MOTION,
        .pointer_motion = {
            .size = حجم(((بنية xkb_event*)0)->pointer_motion),
            .flags = flags,
            .x = filter->action.ptr.x,
            .y = filter->action.ptr.y,
        }
    });
}

ساكن فراغ
xkb_filter_pointer_move_new(بنية xkb_server_state *state,
                            بنية xkb_events *events,
                            بنية xkb_filter *filter)
{
    אם (!events) {
        filter->func = عدم;
        החזר;
    }

    أكد(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_KEYS);
    append_pointer_move(events, filter);
    state->flags &= ~STATE_REQUIRE_KEY_EVENT;
}

ساكن bool
xkb_filter_pointer_move_func(بنية xkb_server_state *state,
                             بنية xkb_events *events,
                             بنية xkb_filter *filter,
                             ثابت بنية xkb_key *key,
                             تعداد xkb_key_direction direction)
{
    אם (key != filter->key || !events)
        החזר XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_KEY_EVENT;

    بدل (direction) {
    حالة XKB_KEY_DOWN:
        filter->refcnt++;
        append_pointer_move(events, filter);
        החזר XKB_FILTER_CONSUME;
    حالة XKB_KEY_REPEATED:
        אם (filter->action.ptr.flags & ACTION_REPEAT)
            append_pointer_move(events, filter);
        החזר XKB_FILTER_CONSUME;
    افتراضي:
        אם (--filter->refcnt > 0)
            החזר XKB_FILTER_CONSUME;
    }

    filter->func = عدم;
    החזר XKB_FILTER_CONSUME;
}

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن فراغ
xkb_filter_pointer_button_new(بنية xkb_server_state *state,
                              بنية xkb_events *events,
                              بنية xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن bool
xkb_filter_pointer_button_func(بنية xkb_server_state *state,
                               بنية xkb_events *events,
                               بنية xkb_filter *filter,
                               ثابت بنية xkb_key *key,
                               تعداد xkb_key_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن فراغ
xkb_filter_pointer_lock_button_new(بنية xkb_server_state *state,
                                   بنية xkb_events *events,
                                   بنية xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن bool
xkb_filter_pointer_lock_button_func(بنية xkb_server_state *state,
                                    بنية xkb_events *events,
                                    بنية xkb_filter *filter,
                                    ثابت بنية xkb_key *key,
                                    تعداد xkb_key_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن فراغ
xkb_filter_pointer_set_default_button_new(بنية xkb_server_state *state,
                                          بنية xkb_events *events,
                                          بنية xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن bool
xkb_filter_pointer_set_default_button_func(بنية xkb_server_state *state,
                                           بنية xkb_events *events,
                                           بنية xkb_filter *filter,
                                           ثابت بنية xkb_key *key,
                                           تعداد xkb_key_direction direction);

ساكن فراغ
xkb_filter_terminate_server_new(بنية xkb_server_state *state,
                                 بنية xkb_events *events,
                                 بنية xkb_filter *filter);

ساكن bool
xkb_filter_terminate_server_func(بنية xkb_server_state *state,
                                  بنية xkb_events *events,
                                  بنية xkb_filter *filter,
                                  ثابت بنية xkb_key *key,
                                  تعداد xkb_key_direction direction);

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
                                       ثابت بنية xkb_key *key,
                                       تعداد xkb_key_direction direction);

ساكن مضمن فراغ
clear_all_latches_and_locks(بنية xkb_server_state *state,
                            بنية xkb_events *events);

ساكن فراغ
xkb_filter_ctrls_new(بنية xkb_server_state *state,
                     بنية xkb_events *events,
                     بنية xkb_filter *filter)
{
    אם (filter->action.type == ACTION_TYPE_CTRL_SET) {
        /* Set: save the specified controls that are *not* already enabled */
        filter->priv = (uint32_t) (
            (~state->base.components.controls) & filter->action.ctrls.ctrls
        );
    } אחרת {
        /* Lock: save the specified controls that *are* already enabled */
        filter->priv = (uint32_t) (
            state->base.components.controls & filter->action.ctrls.ctrls
        );
    }

    אם (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_LOCK)) {
        /* Enable the specified controls that are not already enabled */
        state->base.components.controls |= filter->action.ctrls.ctrls;
    }
}

ساكن bool
xkb_filter_ctrls_func(بنية xkb_server_state *state,
                      بنية xkb_events *events,
                      بنية xkb_filter *filter,
                      ثابت بنية xkb_key *key,
                      تعداد xkb_key_direction direction)
{
    אם (key != filter->key)
        החזר XKB_FILTER_CONTINUE;

    بدل (direction) {
    حالة XKB_KEY_DOWN:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_KEY_REPEATED:
        החזר XKB_FILTER_CONSUME;
    افتراضي:
        אם (--filter->refcnt > 0)
            החזר XKB_FILTER_CONSUME;
    }

    אם (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_UNLOCK)) {
        ثابت تعداد xkb_action_controls old = state->base.components.controls;

        /*
         * Set: Disable specified controls that were *not* enabled at key press.
         * Lock: Disable specified controls that *were* enabled at key press.
         */
        state->base.components.controls &= ~(تعداد xkb_action_controls) filter->priv;

        אם ((old & CONTROL_STICKY_KEYS) &&
            !(state->base.components.controls & CONTROL_STICKY_KEYS)) {
            /* Sticky keys were disabled: clear all locks and latches */
            clear_all_latches_and_locks(state, events);
        }
    }

    filter->func = عدم;
    החזר XKB_FILTER_CONTINUE;
}

ساكن bool
append_redirect_key_events(بنية xkb_server_state *state,
                           بنية xkb_events *events,
                           ثابت بنية xkb_redirect_key_action *redirect,
                           تعداد xkb_key_direction direction)
{
    تعداد xkb_state_component changed = 0;
    ثابت xkb_mod_mask_t mask = redirect->affect_mods;

    /*
     * Reference state: find the last state update in the queue, otherwise
     * use the current state.
     */
    بنية xkb_event *event;
    بنية state_components last_components = state->base.components;
    darray_foreach_reverse(event, events->queue) {
        אם (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
            last_components = event->components.components;
            اكسر;
        }
    }

    אם (mask) {
        بنية state_components new = last_components;
        new.base_mods = (new.base_mods & ~mask) | redirect->mods;
        new.latched_mods = (new.latched_mods & ~mask) | redirect->mods;
        new.locked_mods = (new.locked_mods & ~mask) | redirect->mods;
        new.mods = (new.mods & ~mask) | redirect->mods;

        changed = get_state_component_changes(&last_components, &new);
        אם (changed) {
            darray_append(events->queue, (بنية xkb_event) {
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
            darray_append(events->queue, (بنية xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_FRAME,
            });
        }
    }

    darray_append(events->queue, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_KEY,
        .key = {
            .keycode = redirect->keycode,
            .direction = direction
        }
    });

    אם (mask && changed) {
        /* Restore state */
        darray_append(events->queue, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = last_components,
                .changed = changed
            }
        });
    }

    state->flags &= ~STATE_REQUIRE_KEY_EVENT;
    החזר true;
}

ساكن فراغ
xkb_filter_redirect_key_new(بنية xkb_server_state *state,
                            بنية xkb_events *events,
                            بنية xkb_filter *filter)
{
    /* Action effectual only with the xkb_machine API and a valid keycode */
    אם (!events || filter->action.redirect.keycode == XKB_KEYCODE_INVALID) {
        filter->func = عدم;
        החזר;
    }
    append_redirect_key_events(state, events, &filter->action.redirect,
                               XKB_KEY_DOWN);
}

ساكن bool
xkb_filter_redirect_key_func(بنية xkb_server_state *state,
                             بنية xkb_events *events,
                             بنية xkb_filter *filter,
                             ثابت بنية xkb_key *key,
                             تعداد xkb_key_direction direction)
{
    אם (key != filter->key)
        החזר XKB_FILTER_CONTINUE;

    אם (direction == XKB_KEY_UP) {
        append_redirect_key_events(state, events,
                                   &filter->action.redirect, XKB_KEY_UP);
        filter->func = عدم;
        החזר XKB_FILTER_CONSUME;
     } אחרת אם (direction == XKB_KEY_DOWN) {
        ثابت اتحاد xkb_action *actions = عدم;
        ثابت xkb_action_count_t count =
            xkb_key_get_actions(&state->base, key, &actions);

        עבור (xkb_action_count_t a = 0; a < count; a++) {
            אם (actions[a].type == ACTION_TYPE_REDIRECT_KEY &&
                actions[a].redirect.keycode != filter->action.redirect.keycode) {
                    /* One action redirects to a different key: release first */
                    append_redirect_key_events(state, events,
                                               &filter->action.redirect,
                                               XKB_KEY_UP);
                filter->func = عدم;
                החזר XKB_FILTER_CONTINUE;
            }
        }
    }

    append_redirect_key_events(state, events, &filter->action.redirect,
                               direction);
    החזר XKB_FILTER_CONSUME;
}

ساكن ثابت بنية {
    فراغ (*new)(بنية xkb_server_state *state,
                بنية xkb_events *events,
                بنية xkb_filter *filter);
    bool (*func)(بنية xkb_server_state *state,
                 بنية xkb_events *events,
                 بنية xkb_filter *filter,
                 ثابت بنية xkb_key *key, تعداد xkb_key_direction direction);
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
ساكن فراغ
xkb_filter_apply_all(بنية xkb_server_state *state,
                     بنية xkb_events *events,
                     ثابت بنية xkb_key *key,
                     تعداد xkb_key_direction direction)
{
    /* First run through all the currently active filters and see if any of
     * them have consumed this event. */
    bool consumed = false;
    بنية xkb_filter *filter;
    darray_foreach(filter, state->filters) {
        אם (!filter->func)
            استمر;

        אם (filter->func(state, events, filter, key, direction) == XKB_FILTER_CONSUME)
            consumed = true;
    }
    אם (consumed || direction == XKB_KEY_UP)
        החזר;

    /* No filter consumed this event, so proceed with the key actions */
    ثابت اتحاد xkb_action *actions = عدم;
    xkb_action_count_t count = xkb_key_get_actions(&state->base, key,
                                                         &actions);

    עבור (بنية {
            xkb_action_count_t count;
            ثابت اتحاد xkb_action *actions;
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
        עבור (xkb_action_count_t k = 0; k < count; k++) {
            /*
             * It's possible for the keymap to set action->type explicitly, like
             * so:
             *     interpret XF86_Next_VMode {
             *         action = Private(type=0x86, data="+VMode");
             *     };
             * We don't handle those.
             */
            אם (actions[k].type >= _ACTION_TYPE_NUM_ENTRIES)
                استمر;

            /* Go to next action if no corresponding action handler */
            אם (!filter_action_funcs[actions[k].type].new)
                استمر;

            /* Add a new filter and run the corresponding initial action */
            filter = xkb_filter_new(state);
            filter->key = key;
            filter->action = actions[k];

            بدل (filter->action.type) {
            حالة ACTION_TYPE_MOD_SET:
                אם (state->base.components.controls & CONTROL_STICKY_KEYS) {
                    /* Convert modifier set action to a latch */
                    filter->action.type = ACTION_TYPE_MOD_LATCH;
                    אם (state->a11y_flags & XKB_A11Y_STICKY_KEYS_LATCH_TO_LOCK) {
                        filter->action.mods.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                /* fallthrough */
            حالة ACTION_TYPE_MOD_LATCH:
            حالة ACTION_TYPE_MOD_LOCK: {
                ساكن ثابت تعداد xkb_action_flags unlock_mods_on_press
                    = ACTION_LOCK_CLEAR     /* SetMods & LatchMods: can unlock */
                    | ACTION_LOCK_NO_UNLOCK /* LockMods: cannot unlock */
                    | ACTION_EXPLICIT_UNLOCK_ON_PRESS;
                אם ((state->flags & STATE_UNLOCK_MODS_ON_PRESS) &&
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
                اكسر;
            }
            حالة ACTION_TYPE_GROUP_SET:
                אם (state->base.components.controls & CONTROL_STICKY_KEYS) {
                    /* Convert group set action to a latch */
                    filter->action.type = ACTION_TYPE_GROUP_LATCH;
                    אם (state->a11y_flags & XKB_A11Y_STICKY_KEYS_LATCH_TO_LOCK) {
                        filter->action.group.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                اكسر;
            حالة ACTION_TYPE_GROUP_LOCK:
                אם ((filter->action.group.flags &
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
                    أكد(next.count == 1);
                }
                اكسر;
            حالة ACTION_TYPE_PTR_MOVE:
            حالة ACTION_TYPE_PTR_BUTTON:
            حالة ACTION_TYPE_PTR_LOCK:
            حالة ACTION_TYPE_PTR_DEFAULT:
                אם (!(state->base.components.controls & CONTROL_MOUSE_KEYS)) {
                    /* Convert pointer actions to NoAction */
                    أكد(!filter_action_funcs[ACTION_TYPE_NONE].new);
                    filter->func = عدم;
                    استمر;
                }
                اكسر;
            حالة ACTION_TYPE_REDIRECT_KEY:
                // FIXME: this is not efficient to resolve mods here each time
                filter->action.redirect.affect_mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.affect_mods
                );
                filter->action.redirect.mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.mods
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
xkb_state_init(بنية xkb_state * restrict state,
               بنية xkb_keymap * restrict keymap,
               تعداد xkb_state_mode_internal mode)
{
    state->mode = mode;
    state->refcnt = 1;
    state->keymap = xkb_keymap_ref(keymap);
    state->out_of_range_group.policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

    /* Ensure that derived state is correctly initialized */
    xkb_state_update_derived(state);
}

ساكن مضمن فراغ
xkb_client_state_init(بنية xkb_client_state * restrict state,
                      بنية xkb_keymap * restrict keymap,
                      تعداد xkb_state_mode_internal mode)
{
    xkb_state_init(&state->base, keymap, mode);
}

ساكن مضمن فراغ
xkb_server_state_init(بنية xkb_server_state * restrict state,
                      بنية xkb_keymap * restrict keymap,
                      تعداد xkb_state_mode_internal mode,
                      تعداد xkb_a11y_flags a11y_affect,
                      تعداد xkb_a11y_flags a11y_flags)
{
    xkb_state_init(&state->base, keymap, mode);

    state->a11y_flags = a11y_flags;
    אם (keymap->format != XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !(a11y_affect & XKB_A11Y_LATCH_SIMULTANEOUS_KEYS)) {
             /* Keymap v2+: enable extension to XKB if not manually disabled */
             state->a11y_flags |= XKB_A11Y_LATCH_SIMULTANEOUS_KEYS;
    }
}

بنية xkb_state *
xkb_state_new(بنية xkb_keymap *keymap)
{
    بنية xkb_server_state * ثابت state = calloc(1, حجم(*state));
    אם (!state) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "Could not allocate a state object.\n");
        החזר عدم;
    }

    xkb_server_state_init(state, keymap, LEGACY_MIXED_STATE, 0, 0);

    החזר (بنية xkb_state *)state;
}

بنية xkb_state *
xkb_state_new_with_mode(بنية xkb_keymap * restrict keymap,
                        تعداد xkb_state_mode mode,
                        تعداد xkb_status * restrict status)
{
    بدل (mode) {
    حالة XKB_STATE_MODE_CLIENT:
    حالة XKB_STATE_MODE_SERVER_QUERY: {
        بنية xkb_client_state * ثابت state = calloc(1, حجم(*state));
        אם (!state)
            اكسر;

        static_assert((unsigned)XKB_STATE_MODE_CLIENT ==
                      (unsigned)CLIENT_STATE, "");
        static_assert((unsigned)XKB_STATE_MODE_SERVER_QUERY ==
                      (unsigned)SERVER_COMPANION, "");
        xkb_client_state_init(state, keymap, (تعداد xkb_state_mode_internal)mode);

        אם (status)
            *status = XKB_SUCCESS;

        החזר (بنية xkb_state *)state;
    }
    حالة XKB_STATE_MODE_SERVER: {
        بنية xkb_server_state * ثابت state = calloc(1, حجم(*state));
        אם (!state)
            اكسر;

        xkb_server_state_init(state, keymap, LEGACY_SERVER_STATE, 0, 0);

        אם (status)
            *status = XKB_SUCCESS;

        החזר (بنية xkb_state *)state;
    }
    افتراضي:
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_STATE_MODE_,
                     "Unsupported state mode: %u\n", mode);
        אם (status)
            *status = XKB_ERROR_UNSUPPORTED_STATE_MODE;
        החזר عدم;
    }

    log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                  "Could not allocate a state object.\n");
    אם (status)
        *status = XKB_ERROR_ALLOCATION_FAILURE;
    החזר عدم;
}

بنية xkb_state *
xkb_state_ref(بنية xkb_state *state)
{
    أكد(state->refcnt > 0);
    state->refcnt++;
    החזר state;
}

فراغ
xkb_state_reset(بنية xkb_state *state)
{
    memset(&state->components, 0, حجم(state->components));

    אם (state->mode < LEGACY_MIXED_STATE) {
        static_assert(حجم(*state) == حجم(بنية xkb_client_state),
                      "Not a mere wrapper");
        /* Nothing else to reset */
    } אחרת {
        بنية xkb_server_state *state_ = (بنية xkb_server_state *)state;
        ثابت بنية xkb_server_state prev = *state_;
        memset(state_, 0, حجم(*state_));

        state_->base = prev.base;
        state_->a11y_flags = prev.a11y_flags;
        state_->filters = prev.filters;
        darray_size(state_->filters) = 0;
    }
}

ساكن مضمن فراغ
xkb_state_destroy(بنية xkb_state *state)
{
    xkb_keymap_unref(state->keymap);
    אם (state->mode > SERVER_COMPANION)
        darray_free(((بنية xkb_server_state *)state)->filters);
}

فراغ
xkb_state_unref(بنية xkb_state *state)
{
    أكد(!state || state->refcnt > 0);
    אם (!state || --state->refcnt > 0)
        החזר;

    xkb_state_destroy(state);

    بدل (state->mode) {
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
    החזר state->keymap;
}

/**
 * Update the LED state to match the rest of the xkb_state.
 */
ساكن فراغ
xkb_state_led_update_all(بنية xkb_state *state)
{
    xkb_led_index_t idx;
    ثابت بنية xkb_led *led;

    state->components.leds = 0;

    xkb_leds_enumerate(idx, led, state->keymap) {

        אם (led->which_mods != 0 && led->mods.mask != 0) {
            xkb_mod_mask_t mod_mask = 0;
            אם (led->which_mods & XKB_STATE_MODS_EFFECTIVE)
                mod_mask |= state->components.mods;
            אם (led->which_mods & XKB_STATE_MODS_DEPRESSED)
                mod_mask |= state->components.base_mods;
            אם (led->which_mods & XKB_STATE_MODS_LATCHED)
                mod_mask |= state->components.latched_mods;
            אם (led->which_mods & XKB_STATE_MODS_LOCKED)
                mod_mask |= state->components.locked_mods;

            אם (led->mods.mask & mod_mask) {
                state->components.leds |= (UINT32_C(1) << idx);
                استمر;
            }
        }

        אם (led->which_groups != 0) {
            אם (likely(led->groups) != 0) {
                xkb_layout_mask_t group_mask = 0;
                /* Effective and locked groups have been brought into range */
                أكد(state->components.group < XKB_MAX_GROUPS);
                أكد(state->components.locked_group >= 0 &&
                       state->components.locked_group < XKB_MAX_GROUPS);
                /* Effective and locked groups are used as mask */
                אם (led->which_groups & XKB_STATE_LAYOUT_EFFECTIVE)
                    group_mask |= (UINT32_C(1) << state->components.group);
                אם (led->which_groups & XKB_STATE_LAYOUT_LOCKED)
                    group_mask |= (UINT32_C(1) << state->components.locked_group);
                /* Base and latched groups only have to be non-zero */
                אם ((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                    state->components.base_group != 0)
                    group_mask |= led->groups;
                אם ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                    state->components.latched_group != 0)
                    group_mask |= led->groups;

                אם (led->groups & group_mask) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    استمر;
                }
            } אחרת {
                /* Special case for Base and latched groups */
                אם (((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                     state->components.base_group == 0) ||
                    ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                     state->components.latched_group == 0)) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    استمر;
                }
            }
        }

        אם (led->ctrls & state->components.controls) {
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

ساكن فراغ
state_update_enabled_controls(بنية xkb_server_state *state,
                              تعداد xkb_keyboard_control_flags affect_controls,
                              تعداد xkb_keyboard_control_flags controls,
                              بنية xkb_events *events)
{
    ثابت bool had_sticky_keys = state->base.components.controls
                               & CONTROL_STICKY_KEYS;

    /*
     * Enable using the public API with the all the Control values, except
     * the internal ones, if any.
     */
    affect_controls =
        ( affect_controls
        & (تعداد xkb_keyboard_control_flags)CONTROL_ALL_BOOLEAN );
    state->base.components.controls &=
        (تعداد xkb_action_controls)~affect_controls;
    state->base.components.controls |=
        (تعداد xkb_action_controls)(controls & affect_controls);

    אם (had_sticky_keys &&
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
تعداد xkb_state_component
xkb_state_update_key(بنية xkb_state *base_state, xkb_keycode_t kc,
                     تعداد xkb_key_direction direction)
{
    /* Guard against client-only state */
    أكد(base_state->mode > SERVER_COMPANION);
    אם (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        החזר 0;
    }

    بنية xkb_server_state * ثابت state =
        (بنية xkb_server_state *)base_state;

    ثابت بنية xkb_key* ثابت key = XkbKey(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    אם (!key || (direction == XKB_KEY_REPEATED && !key->repeats))
        החזר 0;

    אם (direction == XKB_KEY_DOWN &&
        (state->base.components.controls & XKB_KEYBOARD_CONTROL_A11Y_STICKY_KEYS) &&
        (state->a11y_flags & XKB_A11Y_STICKY_KEYS_NO_SIMULTANEOUS_KEYS) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11Y_STICKY_KEYS,
                                      0, عدم);
    }

    ثابت بنية state_components prev_components = state->base.components;

    state->set_mods = 0;
    state->clear_mods = 0;

    xkb_filter_apply_all(state, عدم, key, direction);

    xkb_mod_index_t i;
    xkb_mod_mask_t bit;
    עבור (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        אם (state->set_mods & bit) {
            state->mod_key_count[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    עבור (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        אם (state->clear_mods & bit) {
            state->mod_key_count[i]--;
            אם (state->mod_key_count[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_key_count[i] = 0;
            }
            state->clear_mods &= ~bit;
        }
    }

    xkb_state_update_derived(&state->base);

    החזר get_state_component_changes(&prev_components, &state->base.components);
}

/* We need fake keys for `update_latch_modifiers` and `update_latch_group`.
 * These keys must have at least one level in order to break latches. We need 2
 * keys with specific actions in order to update group/mod latches without
 * affecting each other. */
ساكن بنية xkb_key_type_entry synthetic_key_level_entry = { 0 };
ساكن بنية xkb_key_type synthetic_key_type = {
     .num_entries = 1,
     .num_levels = 1,
     .entries = &synthetic_key_level_entry
};
ساكن ثابت بنية xkb_key synthetic_key = { 0 };

/* Transcription from xserver: XkbLatchModifiers */
ساكن فراغ
update_latch_modifiers(بنية xkb_server_state *state,
                       بنية xkb_events *events,
                       xkb_mod_mask_t mask, xkb_mod_mask_t latches)
{
    /* Clear affected latched modifiers */
    ثابت xkb_mod_mask_t clear = mask & ~latches;
    state->base.components.latched_mods &= ~clear;

    /* Clear any pending latch to locks using ad hoc action:
     * only affect corresponding modifier latches and no group latch. */
    بنية xkb_level synthetic_key_level_break_mod_latch = {
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
    بنية xkb_group synthetic_key_group_break_mod_latch = {
        .type = &synthetic_key_type,
        .levels = &synthetic_key_level_break_mod_latch
    };
    ثابت بنية xkb_key synthetic_key_break_mod_latch = {
        .num_groups = 1,
        .groups = &synthetic_key_group_break_mod_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_key_break_mod_latch, XKB_KEY_DOWN);

    /* Finally set the latched mods by simulate tapping a key with the
     * corresponding action */
    ثابت بنية xkb_key* ثابت key = &synthetic_key;
    ثابت اتحاد xkb_action latch_mods = {
        .mods = {
            .type = ACTION_TYPE_MOD_LATCH,
            .mods = { .mask = mask & latches },
            .flags = 0,
        },
    };
    بنية xkb_filter* ثابت filter = xkb_filter_new(state);
    filter->key = key;
    filter->func = xkb_filter_mod_latch_func;
    filter->action = latch_mods;
    xkb_filter_mod_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_mod_latch_func(state, events, filter, key, XKB_KEY_UP);
}

/* Transcription from xserver: XkbLatchGroup */
ساكن فراغ
update_latch_group(بنية xkb_server_state *state,
                   بنية xkb_events *events, int32_t group)
{
    /* Clear any pending latch to locks. */
    ساكن بنية xkb_level synthetic_key_level_break_group_latch = {
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
    ساكن بنية xkb_group synthetic_key_group_break_group_latch = {
        .type = &synthetic_key_type,
        .levels = &synthetic_key_level_break_group_latch
    };
    ساكن ثابت بنية xkb_key synthetic_key_break_group_latch = {
        .num_groups = 1,
        .groups = &synthetic_key_group_break_group_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_key_break_group_latch, XKB_KEY_DOWN);

    /* Simulate tapping a key with a group latch action, but in isolation: i.e.
     * without affecting the other filters. */
    ثابت بنية xkb_key* ثابت key = &synthetic_key;
    ثابت اتحاد xkb_action latch_group = {
        .group = {
            .type = ACTION_TYPE_GROUP_LATCH,
            .flags = ACTION_ABSOLUTE_SWITCH,
            .group = group,
        },
    };
    بنية xkb_filter* ثابت filter = xkb_filter_new(state);
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
ساكن مضمن xkb_mod_mask_t
resolve_to_canonical_mods(بنية xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    החזר
        /*
         * Keep canonical modifier mask.
         * It contains either real modifiers or canonical virtual modifiers.
         */
        (mods & keymap->canonical_state_mask) |
        /* Resolve other modifiers */
        mod_mask_get_effective(keymap,
                               mods & ~keymap->canonical_state_mask);
}

ساكن فراغ
state_update_latched_locked(
    بنية xkb_server_state * restrict state,
    ثابت بنية xkb_state_components_update * restrict update,
    بنية xkb_events * restrict events
)
{
    /* Update locks */
    ثابت xkb_mod_mask_t affect_locked_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_locked_mods);
    אם ((update->components & XKB_STATE_MODS_LOCKED) &&
        affect_locked_mods) {
        ثابت xkb_mod_mask_t locked_mods =
            resolve_to_canonical_mods(state->base.keymap, update->locked_mods);
        state->base.components.locked_mods &= ~affect_locked_mods;
        state->base.components.locked_mods |= locked_mods & affect_locked_mods;
    }
    אם (update->components & XKB_STATE_LAYOUT_LOCKED) {
        state->base.components.locked_group = update->locked_layout;
    }

    /* Update latches */
    ثابت xkb_mod_mask_t affect_latched_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_latched_mods);
    אם ((update->components & XKB_STATE_MODS_LATCHED) &&
        affect_latched_mods) {
        ثابت xkb_mod_mask_t latched_mods =
            resolve_to_canonical_mods(state->base.keymap, update->latched_mods);
        update_latch_modifiers(state, events, affect_latched_mods, latched_mods);
    }
    אם (update->components & XKB_STATE_LAYOUT_LATCHED) {
        update_latch_group(state, events, update->latched_layout);
    }
}

تعداد xkb_state_component
xkb_state_update_latched_locked(بنية xkb_state *base_state,
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
    אם (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        החזר 0;
    }

    بنية xkb_server_state * ثابت state =
        (بنية xkb_server_state *)base_state;

    ثابت بنية state_components previous_components = state->base.components;
    ثابت تعداد xkb_state_component components
        = ((affect_latched_mods || latched_mods) ? XKB_STATE_MODS_LATCHED : 0)
        | ((affect_locked_mods || locked_mods) ? XKB_STATE_MODS_LOCKED : 0)
        | (affect_latched_layout ? XKB_STATE_LAYOUT_LATCHED : 0)
        | (affect_locked_layout ? XKB_STATE_LAYOUT_LOCKED : 0);
    ثابت بنية xkb_state_components_update update = {
        .size = حجم(update),
        .components = components,
        .affect_latched_mods = affect_latched_mods,
        .latched_mods = latched_mods,
        .latched_layout = latched_layout,
        .affect_locked_mods = affect_locked_mods,
        .locked_mods = locked_mods,
        .locked_layout = locked_layout
    };
    state_update_latched_locked(state, &update, عدم);
    xkb_state_update_derived(&state->base);
    החזר get_state_component_changes(&previous_components,
                                       &state->base.components);
}

ساكن مضمن فراغ
clear_all_latches_and_locks(بنية xkb_server_state *state,
                            بنية xkb_events *events)
{
    ساكن ثابت تعداد xkb_state_component components
        = XKB_STATE_MODS_LATCHED | XKB_STATE_MODS_LOCKED
        | XKB_STATE_LAYOUT_LATCHED | XKB_STATE_LAYOUT_LOCKED;
    ثابت بنية xkb_state_components_update update = {
        .size = حجم(update),
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

ساكن تعداد xkb_status
state_update_layout_policy(بنية xkb_server_state *state,
                           ثابت بنية xkb_layout_policy_update *update)
{
    אם (xkb_feature_supported(XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY,
                              update->policy)) {
        אם (update->policy == XKB_LAYOUT_OUT_OF_RANGE_REDIRECT) {
            אם (update->redirect < state->base.keymap->num_groups) {
                state->base.out_of_range_group.redirect_group =
                    update->redirect;
            } אחרת {
                log_err(state->base.keymap->ctx,
                        XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Layout policy: "
                        "unsupported layout index %"PRIu32" > %"PRIu32"\n",
                        update->redirect + 1, state->base.keymap->num_groups);
                החזר XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
            }
        }
        state->base.out_of_range_group.policy = update->policy;
        החזר XKB_SUCCESS;
    } אחרת {
        log_err(state->base.keymap->ctx,
                XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY_,
                "Unsupported layout policy: %d\n", update->policy);
        החזר XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY;
    }
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
ساكن تعداد xkb_status
check_state_update_abi_(بنية xkb_context * restrict ctx,
                        ثابت حرف * restrict func,
                        ثابت بنية xkb_synthetic_update * restrict update)
{
    تعداد xkb_status status = XKB_SUCCESS;
    אם ((status = xkb_check_state_abi(update)) ||
        (update->reserved0 != 0 &&
         (status = XKB_ERROR_ABI_FORWARD_COMPAT)) ||
        (update->components &&
         (status = xkb_check_state_abi(update->components))) ||
        (update->layout_policy &&
         (status = xkb_check_state_abi(update->layout_policy)))) {
        xkb_log_abi_error(ctx, func, status);
    }
    החזר status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

#define check_state_update_abi(ctx, update) \
    check_state_update_abi_(ctx, __func__, update)

تعداد xkb_status
xkb_state_update_synthetic(بنية xkb_state * base_state,
                           ثابت بنية xkb_synthetic_update * update,
                           تعداد xkb_state_component *changed)
{
    /* Guard against client-only state */
    אם (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        החזר XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    بنية xkb_server_state * ثابت state =
        (بنية xkb_server_state *)base_state;

    /* Check ABI compatibility */
    تعداد xkb_status status = check_state_update_abi(state->base.keymap->ctx,
                                                    update);
    אם (status != XKB_SUCCESS)
        החזר status;

    ثابت بنية state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    /* Update parametrized controls first */
    אם (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        אם (status != XKB_SUCCESS)
            החזר status;
    }

    אם (update->components) {
        ثابت بنية xkb_state_components_update * ثابت components =
            update->components;
        /* Update boolean controls first */
        אם (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, عدم);
        }

        state_update_latched_locked(state, components, عدم);
    }

    xkb_state_update_derived(&state->base);

    אם (changed) {
        *changed = get_state_component_changes(&previous_components,
                                               &state->base.components);
    }
    החזר XKB_SUCCESS;
}

/**
 * Updates the state from a set of explicit masks as gained from
 * xkb_state_serialize_mods and xkb_state_serialize_groups.  As noted in the
 * documentation for these functions in xkbcommon.h, this round-trip is
 * lossy, and should only be used to update a slave state mirroring the
 * master, e.g. in a client/server window system.
 */
تعداد xkb_state_component
xkb_state_update_mask(بنية xkb_state *base_state,
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
    אם (base_state->mode != CLIENT_STATE &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        החזר 0;
    }

    بنية xkb_client_state * ثابت state =
        (بنية xkb_client_state *)base_state;

    ثابت بنية state_components prev_components = state->base.components;

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

    החזר get_state_component_changes(&prev_components, &state->base.components);
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Lock_Modifier
 */
ساكن bool
should_do_caps_transformation(بنية xkb_state *state, xkb_keycode_t kc)
{
    החזר
        xkb_state_mod_index_is_active(state, XKB_MOD_INDEX_CAPS,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_index_is_consumed(state, kc, XKB_MOD_INDEX_CAPS) == 0;
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Control_Modifier
 */
ساكن bool
should_do_ctrl_transformation(بنية xkb_state *state, xkb_keycode_t kc)
{
    החזר
        xkb_state_mod_index_is_active(state, XKB_MOD_INDEX_CTRL,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_index_is_consumed(state, kc, XKB_MOD_INDEX_CTRL) == 0;
}

/**
 * Provides the symbols to use for the given key and state.  Returns the
 * number of symbols pointed to in syms_out.
 */
صحيح
xkb_state_key_get_syms(بنية xkb_state *state, xkb_keycode_t kc,
                       ثابت xkb_keysym_t **syms_out)
{
    ثابت xkb_layout_index_t layout = xkb_state_key_get_layout(state, kc);
    אם (layout == XKB_LAYOUT_INVALID)
        اذهب err;

    ثابت xkb_level_index_t level = xkb_state_key_get_level(state, kc, layout);
    אם (level == XKB_LEVEL_INVALID)
        اذهب err;

    ثابت بنية xkb_key* ثابت key = XkbKey(state->keymap, kc);

    אם (!key)
        اذهب err;

    ثابت بنية xkb_level* ثابت leveli =
        xkb_keymap_key_get_level(state->keymap, key, layout, level);

    אם (!leveli)
        اذهب err;

    ثابت xkb_keysym_count_t num_syms = leveli->num_syms;
    אם (num_syms == 0)
        اذهب err;

    אם (should_do_caps_transformation(state, kc)) {
        /* Only simple capitalization rules: keysyms count is unchanged. */
        אם (num_syms > 1) {
            *syms_out = (leveli->has_upper)
                      ? leveli->s.syms + num_syms
                      : leveli->s.syms;
        } אחרת {
            *syms_out = &leveli->upper;
        }
    } אחרת {
        *syms_out = (num_syms > 1)
                  ? leveli->s.syms
                  : &leveli->s.sym;
    }
    החזר (صحيح) num_syms;

err:
    *syms_out = عدم;
    החזר 0;
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
XkbToControl(حرف ch)
{
    حرف c = ch;

    אם ((c >= '@' && c < '\177') || c == ' ')
        c &= 0x1F;
    אחרת אם (c == '2')
        c = '\000';
    אחרת אם (c >= '3' && c <= '7')
        c -= ('3' - '\033');
    אחרת אם (c == '8')
        c = '\177';
    אחרת אם (c == '/')
        c = '_' & 0x1F;
    החזר c;
}

/**
 * Provides either exactly one symbol, or XKB_KEY_NoSymbol.
 */
xkb_keysym_t
xkb_state_key_get_one_sym(بنية xkb_state *state, xkb_keycode_t kc)
{
    ثابت xkb_keysym_t *syms = عدم;
    ثابت صحيح num_syms = xkb_state_key_get_syms(state, kc, &syms);

    אם (num_syms != 1)
        החזר XKB_KEY_NoSymbol;
    אחרת
        החזר syms[0];
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
    ثابت xkb_layout_index_t layout = xkb_state_key_get_layout(state, kc);
    ثابت xkb_layout_index_t num_layouts =
        xkb_keymap_num_layouts_for_key(state->keymap, kc);
    xkb_level_index_t level = xkb_state_key_get_level(state, kc, layout);
    אם (layout == XKB_LAYOUT_INVALID || num_layouts == 0 ||
        level == XKB_LEVEL_INVALID)
        החזר XKB_KEY_NoSymbol;

    ثابت xkb_keysym_t *syms = عدم;
    صحيح nsyms = xkb_keymap_key_get_syms_by_level(state->keymap, kc,
                                                 layout, level, &syms);
    אם (nsyms != 1)
        החזר XKB_KEY_NoSymbol;
    xkb_keysym_t sym = syms[0];

    אם (should_do_ctrl_transformation(state, kc) && sym > 127u) {
        עבור (xkb_layout_index_t i = 0; i < num_layouts; i++) {
            level = xkb_state_key_get_level(state, kc, i);
            אם (level == XKB_LEVEL_INVALID)
                استمر;

            nsyms = xkb_keymap_key_get_syms_by_level(state->keymap, kc,
                                                     i, level, &syms);
            אם (nsyms == 1 && syms[0] <= 127u) {
                sym = syms[0];
                اكسر;
            }
        }
    }

    אם (should_do_caps_transformation(state, kc)) {
        sym = xkb_keysym_to_upper(sym);
    }

    החזר sym;
}

صحيح
xkb_state_key_get_utf8(بنية xkb_state *state, xkb_keycode_t kc,
                       حرف *buffer, size_t size)
{
    صحيح nsyms;
    ثابت xkb_keysym_t *syms = عدم;
    ثابت xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    אם (sym != XKB_KEY_NoSymbol) {
        nsyms = 1; syms = &sym;
    }
    אחרת {
        nsyms = xkb_state_key_get_syms(state, kc, &syms);
    }

    /* Make sure not to truncate in the middle of a UTF-8 sequence. */
    صحيح offset = 0;
    حرف tmp[XKB_KEYSYM_UTF8_MAX_SIZE];
    עבור (صحيح i = 0; i < nsyms; i++) {
        صحيح ret = xkb_keysym_to_utf8(syms[i], tmp, حجم(tmp));
        אם (ret <= 0)
            اذهب err_bad;

        ret--;
        אם ((size_t)offset + (size_t)ret <= size)
            memcpy(buffer + offset, tmp, (size_t)ret);
        offset += ret;
    }

    אם ((size_t)offset >= size)
        اذهب err_trunc;
    buffer[offset] = '\0';

    אם (!is_valid_utf8(buffer, (size_t)offset))
        اذهب err_bad;

    אם (offset == 1 && (unsigned صحيح) buffer[0] <= 127u &&
        should_do_ctrl_transformation(state, kc))
        buffer[0] = XkbToControl(buffer[0]);

    החזר offset;

err_trunc:
    אם (size > 0)
        buffer[size - 1] = '\0';
    החזר offset;

err_bad:
    אם (size > 0)
        buffer[0] = '\0';
    החזר 0;
}

uint32_t
xkb_state_key_get_utf32(بنية xkb_state *state, xkb_keycode_t kc)
{
    ثابت xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    uint32_t cp = xkb_keysym_to_utf32(sym);

    אם (cp <= 127u && should_do_ctrl_transformation(state, kc))
        cp = (uint32_t) XkbToControl((حرف) cp);

    החזר cp;
}

/**
 * Serializes the requested modifier state into an xkb_mod_mask_t, with all
 * the same disclaimers as in xkb_state_update_mask.
 */
ساكن مضمن xkb_mod_mask_t
serialize_mods(ثابت بنية state_components *components,
               تعداد xkb_state_component type)
{
    xkb_mod_mask_t ret = 0;

    אם (type & XKB_STATE_MODS_EFFECTIVE)
        החזר components->mods;

    אם (type & XKB_STATE_MODS_DEPRESSED)
        ret |= components->base_mods;
    אם (type & XKB_STATE_MODS_LATCHED)
        ret |= components->latched_mods;
    אם (type & XKB_STATE_MODS_LOCKED)
        ret |= components->locked_mods;

    החזר ret;
}

xkb_mod_mask_t
xkb_state_serialize_mods(بنية xkb_state *state,
                         تعداد xkb_state_component type)
{
    החזר serialize_mods(&state->components, type);
}

/**
 * Serialises the requested group state, with all the same disclaimers as
 * in xkb_state_update_mask.
 */
ساكن مضمن xkb_layout_index_t
serialize_layout(ثابت بنية state_components *components,
                 تعداد xkb_state_component type)
{
    אם (type & XKB_STATE_LAYOUT_EFFECTIVE)
        החזר components->group;

    int32_t ret = 0;
    אם (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret += components->base_group;
    אם (type & XKB_STATE_LAYOUT_LATCHED)
        ret += components->latched_group;
    אם (type & XKB_STATE_LAYOUT_LOCKED)
        ret += components->locked_group;

    החזר (xkb_layout_index_t)ret;
}

xkb_layout_index_t
xkb_state_serialize_layout(بنية xkb_state *state,
                           تعداد xkb_state_component type)
{
    החזר serialize_layout(&state->components, type);
}

xkb_led_mask_t
xkb_state_serialize_leds(ثابت بنية xkb_state *state,
                         تعداد xkb_state_component type)
{
    החזר (type & XKB_STATE_LEDS)
        ? state->components.leds
        : 0;
}

ساكن مضمن تعداد xkb_keyboard_control_flags
serialize_controls(ثابت بنية state_components *components,
                   تعداد xkb_state_component type)
{
    החזר (type & XKB_STATE_CONTROLS_EFFECTIVE)
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
    החזר serialize_controls(&state->components, type);
}

/**
 * Gets a modifier mask and returns the resolved effective mask; this
 * is needed because some modifiers can also map to other modifiers, e.g.
 * the "NumLock" modifier usually also sets the "Mod2" modifier.
 */
xkb_mod_mask_t
mod_mask_get_effective(بنية xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    /* Initialize the effective mask with its corresponding real mods. */
    xkb_mod_mask_t mask = mods & MOD_REAL_MASK_ALL;

    /* Resolve the virtual modifiers */
    ثابت بنية xkb_mod *mod;
    xkb_mod_index_t i;
    xkb_vmods_enumerate(i, mod, &keymap->mods)
        אם (mods & (UINT32_C(1) << i))
            mask |= mod->mapping;

    החזר mask;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
صحيح
xkb_state_mod_index_is_active(بنية xkb_state *state,
                              xkb_mod_index_t idx,
                              تعداد xkb_state_component type)
{
    אם (unlikely(idx >= xkb_keymap_num_mods(state->keymap)))
        החזר -1;

    ثابت xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    אם (!mapping) {
        /* Modifier not mapped */
        החזר 0;
    }
    /* WARNING: this may overmatch for virtual modifiers */
    החזר (xkb_state_serialize_mods(state, type) & mapping) == mapping;
}

تعداد {
    XKB_STATE_MATCH_FLAGS = XKB_STATE_MATCH_ANY
                          | XKB_STATE_MATCH_ALL
                          | XKB_STATE_MATCH_NON_EXCLUSIVE
};

/**
 * Helper function for xkb_state_mod_indices_are_active and
 * xkb_state_mod_names_are_active.
 */
ساكن bool
match_mod_masks(بنية xkb_state *state,
                تعداد xkb_state_component type,
                تعداد xkb_state_match match,
                xkb_mod_mask_t wanted)
{
    ثابت xkb_mod_mask_t active = xkb_state_serialize_mods(state, type);

    אם (!(match & XKB_STATE_MATCH_NON_EXCLUSIVE) && (active & ~wanted))
        החזר false;

    אם (match & XKB_STATE_MATCH_ANY)
        החזר active & wanted;

    /* Ensure all flags but XKB_STATE_MATCH_ALL are processed before */
    static_assert(XKB_STATE_MATCH_ALL ==
                  ((تعداد xkb_state_match) XKB_STATE_MATCH_FLAGS &
                   ~(XKB_STATE_MATCH_ANY | XKB_STATE_MATCH_NON_EXCLUSIVE)),
                  "");

    החזר (active & wanted) == wanted;
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, or -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
صحيح
xkb_state_mod_indices_are_active(بنية xkb_state *state,
                                 تعداد xkb_state_component type,
                                 تعداد xkb_state_match match,
                                 ...)
{
    אם (match & ~((تعداد xkb_state_match) XKB_STATE_MATCH_FLAGS))
        החזר -2;

    va_list ap;
    xkb_mod_mask_t wanted = 0;
    صحيح ret = 0;
    ثابت xkb_mod_index_t num_mods = xkb_keymap_num_mods(state->keymap);

    va_start(ap, match);
    כלעוד (1) {
        xkb_mod_index_t idx = va_arg(ap, xkb_mod_index_t);
        אם (idx == XKB_MOD_INVALID)
            اكسر;
        אם (unlikely(idx >= num_mods)) {
            ret = -1;
            اكسر;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_end(ap);

    אם (ret == -1)
        החזר ret;

    אם (!wanted) {
        /* Modifiers not mapped */
        החזר 0;
    }

    החזר match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
صحيح
xkb_state_mod_name_is_active(بنية xkb_state *state, ثابت حرف *name,
                             تعداد xkb_state_component type)
{
    ثابت xkb_mod_index_t idx = xkb_keymap_mod_get_index(state->keymap, name);

    אם (idx == XKB_MOD_INVALID)
        החזר -1;

    החזר xkb_state_mod_index_is_active(state, idx, type);
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ATTR_NULL_SENTINEL صحيح
xkb_state_mod_names_are_active(بنية xkb_state *state,
                               تعداد xkb_state_component type,
                               تعداد xkb_state_match match,
                               ...)
{
    אם (match & ~((تعداد xkb_state_match) XKB_STATE_MATCH_FLAGS))
        החזר -2;

    va_list ap;
    xkb_mod_mask_t wanted = 0;
    صحيح ret = 0;

    va_start(ap, match);
    כלעוד (1) {
        ثابت حرف *str = va_arg(ap, ثابت حرف *);
        אם (str == عدم)
            اكسر;
        ثابت xkb_mod_index_t idx = xkb_keymap_mod_get_index(state->keymap, str);
        אם (idx == XKB_MOD_INVALID) {
            ret = -1;
            اكسر;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_end(ap);

    אם (ret == -1)
        החזר ret;

    אם (!wanted) {
        /* Modifiers not mapped */
        החזר 0;
    }

    החזר match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given group is active with the specified type(s), 0 if
 * not, or -1 if the group is invalid.
 */
صحيح
xkb_state_layout_index_is_active(بنية xkb_state *state,
                                 xkb_layout_index_t idx,
                                 تعداد xkb_state_component type)
{
    אם (idx >= state->keymap->num_groups)
        החזר -1;

    صحيح ret = 0;
    אם (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ret |= (state->components.group == idx);
    אם (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret |= (state->components.base_group == (int32_t) idx);
    אם (type & XKB_STATE_LAYOUT_LATCHED)
        ret |= (state->components.latched_group == (int32_t) idx);
    אם (type & XKB_STATE_LAYOUT_LOCKED)
        ret |= (state->components.locked_group == (int32_t) idx);

    החזר ret;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
صحيح
xkb_state_layout_name_is_active(بنية xkb_state *state, ثابت حرف *name,
                                تعداد xkb_state_component type)
{
    ثابت xkb_layout_index_t idx =
        xkb_keymap_layout_get_index(state->keymap, name);

    אם (idx == XKB_LAYOUT_INVALID)
        החזר -1;

    החזר xkb_state_layout_index_is_active(state, idx, type);
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
صحيح
xkb_state_led_index_is_active(بنية xkb_state *state, xkb_led_index_t idx)
{
    אם (idx >= state->keymap->num_leds ||
        state->keymap->leds[idx].name == XKB_ATOM_NONE)
        החזר -1;

    החזר !!(state->components.leds & (UINT32_C(1) << idx));
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
صحيح
xkb_state_led_name_is_active(بنية xkb_state *state, ثابت حرف *name)
{
    ثابت xkb_led_index_t idx = xkb_keymap_led_get_index(state->keymap, name);

    אם (idx == XKB_LED_INVALID)
        החזר -1;

    החזר xkb_state_led_index_is_active(state, idx);
}

/**
 * See:
 * - XkbTranslateKeyCode(3), mod_rtrn return value, from libX11.
 * - MyEnhancedXkbTranslateKeyCode(), a modification of the above, from GTK+.
 */
ساكن xkb_mod_mask_t
key_get_consumed(بنية xkb_state *state, ثابت بنية xkb_key *key,
                 تعداد xkb_consumed_mode mode)
{
    ثابت xkb_layout_index_t group =
        xkb_state_key_get_layout(state, key->keycode);
    אם (group == XKB_LAYOUT_INVALID)
        החזר 0;

    xkb_mod_mask_t preserve = 0;
    xkb_mod_mask_t consumed = 0;

    ثابت بنية xkb_key_type_entry* ثابت matching_entry =
        get_entry_for_key_state(state, key, group);
    אם (matching_entry)
        preserve = matching_entry->preserve.mask;

    ثابت بنية xkb_key_type* ثابت type = key->groups[group].type;
    بدل (mode) {
    حالة XKB_CONSUMED_MODE_XKB:
        consumed = type->mods.mask;
        اكسر;

    حالة XKB_CONSUMED_MODE_GTK: {
        ثابت بنية xkb_key_type_entry* ثابت no_mods_entry =
            get_entry_for_mods(type, 0);
        ثابت xkb_level_index_t no_mods_leveli = no_mods_entry
                                               ? no_mods_entry->level
                                               : 0;
        ثابت بنية xkb_level* ثابت no_mods_level =
            &key->groups[group].levels[no_mods_leveli];

        עבור (darray_size_t i = 0; i < type->num_entries; i++) {
            ثابت بنية xkb_key_type_entry* ثابت entry = &type->entries[i];
            אם (!entry_is_active(entry))
                استمر;

            ثابت بنية xkb_level* ثابت level =
                &key->groups[group].levels[entry->level];
            אם (XkbLevelsSameSyms(level, no_mods_level))
                استمر;

            אם (entry == matching_entry || one_bit_set(entry->mods.mask))
                consumed |= entry->mods.mask & ~entry->preserve.mask;
        }
        اكسر;
    }
    }

    החזר consumed & ~preserve;
}

صحيح
xkb_state_mod_index_is_consumed2(بنية xkb_state *state, xkb_keycode_t kc,
                                 xkb_mod_index_t idx,
                                 تعداد xkb_consumed_mode mode)
{
    ثابت بنية xkb_key* ثابت key = XkbKey(state->keymap, kc);

    אם (unlikely(!key || idx >= xkb_keymap_num_mods(state->keymap)))
        החזר -1;

    ثابت xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    אם (!mapping) {
        /* Modifier not mapped */
        החזר 0;
    }
    החזר (mapping & key_get_consumed(state, key, mode)) == mapping;
}

صحيح
xkb_state_mod_index_is_consumed(بنية xkb_state *state, xkb_keycode_t kc,
                                xkb_mod_index_t idx)
{
    החזר xkb_state_mod_index_is_consumed2(state, kc, idx,
                                            XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_mod_mask_remove_consumed(بنية xkb_state *state, xkb_keycode_t kc,
                                   xkb_mod_mask_t mask)
{
    ثابت بنية xkb_key* ثابت key = XkbKey(state->keymap, kc);

    אם (!key)
        החזר 0;

    החזר resolve_to_canonical_mods(state->keymap, mask) &
           ~key_get_consumed(state, key, XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_key_get_consumed_mods2(بنية xkb_state *state, xkb_keycode_t kc,
                                 تعداد xkb_consumed_mode mode)
{
    بدل (mode) {
    حالة XKB_CONSUMED_MODE_XKB:
    حالة XKB_CONSUMED_MODE_GTK:
        اكسر;
    افتراضي:
        log_err_func(state->keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "unrecognized consumed modifiers mode: %d\n", mode);
        החזר 0;
    }

    ثابت بنية xkb_key* ثابت key = XkbKey(state->keymap, kc);
    אם (!key)
        החזר 0;

    החזר key_get_consumed(state, key, mode);
}

xkb_mod_mask_t
xkb_state_key_get_consumed_mods(بنية xkb_state *state, xkb_keycode_t kc)
{
    החזר xkb_state_key_get_consumed_mods2(state, kc, XKB_CONSUMED_MODE_XKB);
}

/*******************************************************************************
 * Server state machine API
 ******************************************************************************/

/** Entry to keep track of the state of an overlaid key */
بنية xkb_overlaid_key {
    /** The source key being overlaid */
    ثابت بنية xkb_key *old;
    /** The target key */
    ثابت بنية xkb_key *new;
    /** Counter of key currently down */
    صحيح refcnt;
};

بنية xkb_shortcuts_config_entry {
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
بنية xkb_machine {
    /** Inherits from `xkb_server_state` */
    بنية xkb_server_state base;

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
        darray(بنية xkb_overlaid_key) keys;
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
    بنية machine_config {
        /** Modifiers remapping */
        بنية machine_modifiers_config {
            /** Real modifier mask */
            xkb_mod_mask_t mask;
            /** Modifiers re-mappings */
            darray_size_t mappings_num;
            بنية machine_mods_mapping {
                xkb_mod_mask_t source;
                xkb_mod_mask_t target;
            } * mappings ATTR_COUNTED_BY(mappings_num);
        } modifiers;

        /** Shortcuts tweak */
        بنية machine_shortcuts_config {
            /** Substitution entries */
            بنية xkb_shortcuts_config_entry *entries;
        } shortcuts;
    } config;

    /** Mouse keys handling */
    بنية {
        /** Default pointer button */
        xkb_pointer_button_index_t default_button;
        /** Mask of the locked buttons */
        xkb_pointer_button_mask_t locked_buttons;
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
        darray(بنية xkb_shortcuts_config_entry) entries;
    } shortcuts;

    تعداد xkb_machine_flags machine_flags;
    تعداد xkb_machine_builder_flags builder_flags;
    صحيح refcnt;
};

بنية xkb_machine_builder *
xkb_machine_builder_new(
    بنية xkb_keymap * restrict keymap,
    ثابت بنية xkb_machine_builder_config * restrict config,
    تعداد xkb_status * restrict status
)
{
    /* Handle default configuration */
    ساكن ثابت بنية xkb_machine_builder_config default_config = {
        .size = حجم(default_config)
    };
    אם (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ثابت تعداد xkb_status status_ = xkb_check_state_abi(config);
    אם (status_) {
        xkb_log_abi_error(keymap->ctx, __func__, status_);
        אם (status)
            *status = status_;
        החזר عدم;
    }

    /* Sanitize input */
    ثابت تعداد xkb_machine_builder_flags invalid_builder_flags
        = config->builder_flags
        & ~(تعداد xkb_machine_builder_flags)XKB_MACHINE_BUILDER_FLAGS_VALUES;
    אם (invalid_builder_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS_,
                     "unrecognized machine builder flags: 0x%x\n",
                     invalid_builder_flags);
        אם (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS;
        החזר عدم;
    }

    ثابت تعداد xkb_machine_flags invalid_machine_flags
        = (تعداد xkb_machine_flags)config->machine_flags
        & ~(تعداد xkb_machine_flags)XKB_MACHINE_FLAGS_VALUES;
    אם (invalid_machine_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS_,
                     "unrecognized state machine flags: 0x%x\n",
                     invalid_machine_flags);
        אם (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS;
        החזר عدم;
    }

    بنية xkb_machine_builder * ثابت builder = calloc(1, حجم(*builder));
    אם (!builder) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine builder\n");
        אם (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        החזר عدم;
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

    אם (status)
        *status = XKB_SUCCESS;

    החזר builder;
}

بنية xkb_machine_builder *
xkb_machine_builder_ref(بنية xkb_machine_builder *builder)
{
    أكد(builder->refcnt > 0);
    builder->refcnt++;
    החזר builder;
}

فراغ
xkb_machine_builder_unref(بنية xkb_machine_builder *builder)
{
    أكد(!builder || builder->refcnt > 0);
    אם (!builder || --builder->refcnt > 0)
        החזר;

    darray_free(builder->shortcuts.entries);
    darray_free(builder->mods);
    xkb_keymap_unref(builder->keymap);
    حرر(builder);
}

بنية xkb_keymap *
xkb_machine_builder_get_keymap(ثابت بنية xkb_machine_builder *builder)
{
    /* Reference count is not updated. See API doc. */
    החזר builder->keymap;
}

تعداد xkb_status
xkb_machine_builder_update_a11y(
    بنية xkb_machine_builder * restrict builder,
    ثابت بنية xkb_machine_builder_a11y_update * restrict update)
{
    /* Check ABI compatibility */
    تعداد xkb_status status = xkb_check_state_abi(update);
    אם (status != XKB_SUCCESS) {
        xkb_log_abi_error(builder->keymap->ctx, __func__, status);
        החזר status;
    }

    ثابت تعداد xkb_a11y_flags invalid_flags =
        ~(تعداد xkb_a11y_flags)XKB_A11Y_FLAGS_VALUES;

    אם (update->affect_flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y affected flags: %#x\n",
                __func__, update->affect_flags & invalid_flags);
        החזר XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }
    אם (update->flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y flags: %#x\n",
                __func__, update->flags & invalid_flags);
        החזר XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }

    builder->controls.a11y.affect_flags |= update->affect_flags;
    builder->controls.a11y.flags &= ~update->affect_flags;
    builder->controls.a11y.flags |= (update->flags & update->affect_flags);

    החזר XKB_SUCCESS;
}

تعداد xkb_status
xkb_machine_builder_update_mods_remap(
    بنية xkb_machine_builder * restrict builder,
    ثابت بنية xkb_machine_builder_mods_remap_update * restrict update
)
{
    אם (!update->source) {
        אם (!update->target) {
            /* Reset mappings */
            darray_resize(builder->mods, 0);
            החזר XKB_SUCCESS;
        } אחרת {
            החזר XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
        }
    }

    /* Check the modifiers against the keymap */
    ثابت xkb_mod_mask_t invalid = ~builder->keymap->canonical_state_mask;
    אם ((update->source & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid source modifiers: 0x%"PRIx32"\n",
                __func__, update->source);
        החזר XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    אם ((update->target & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid target modifiers: 0x%"PRIx32"\n",
                __func__, update->target);
        החזר XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    بنية machine_mods_mapping *mapping = عدم;
    darray_size_t m = 0;
    darray_enumerate(m, mapping, builder->mods) {
        אם (mapping->source == update->source) {
            אם (!update->target) {
                /* Remove mapping */
                darray_remove(builder->mods, m);
            } אחרת {
                /* Update mapping */
                mapping->target = update->target;
            }
            החזר XKB_SUCCESS;
        }
    }

    אם (update->target) {
        /* Append new mapping */
        darray_append(
            builder->mods,
            (بنية machine_mods_mapping) {
                .source = update->source,
                .target = update->target
            }
        );
    } אחרת {
        /* Ignore missing mapping */
    }

    החזר XKB_SUCCESS;
}

تعداد xkb_status
xkb_machine_builder_update_shortcut_override(
    بنية xkb_machine_builder * restrict builder,
    ثابت بنية xkb_machine_builder_shortcut_override_update * restrict update
)
{
    بنية xkb_keymap *keymap = builder->keymap;

    /* Check the modifiers against the keymap */
    ثابت xkb_mod_mask_t invalid = ~keymap->canonical_state_mask;
    אם ((update->affect_mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid affected modifiers: 0x%"PRIx32"\n",
                __func__, update->affect_mods);
        החזר XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    אם ((update->mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid modifiers: 0x%"PRIx32"\n",
                __func__, update->mods);
        החזר XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    /* Check the layout indices against the keymap */
    אם (update->source >= keymap->num_groups &&
        update->source != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "%s: Invalid source layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->num_groups, update->source + 1);
        החזר XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }
    אם (update->target >= keymap->num_groups &&
        update->target != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "%s: Invalid target layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->num_groups, update->target + 1);
        החזר XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    بنية xkb_shortcuts_config_options * ثابت config = &builder->shortcuts;

    /* Resize array & initialize new entries, if relevant */
    אם (update->source >= darray_size(config->entries) &&
        update->source != XKB_LAYOUT_INVALID) {
        אם (update->target == update->source) {
            /* Skip default setting */
            החזר XKB_SUCCESS;
        }
        xkb_layout_index_t new = darray_size(config->entries);
        darray_resize(config->entries, update->source + 1);
        עבור (; new <= update->source; new++) {
            darray_item(config->entries, new) =
                (بنية xkb_shortcuts_config_entry) {
                    .target = XKB_LAYOUT_INVALID,
                    .mods = 0
                };
        }
    }

    بنية xkb_shortcuts_config_entry *entry;
    xkb_layout_index_t source = (update->source == XKB_LAYOUT_INVALID)
        ? 0               /* Loop over all entries */
        : update->source; /* Single entry */
    darray_foreach_from(entry, config->entries, source) {
        אם (update->source == XKB_LAYOUT_INVALID &&
            entry->target == XKB_LAYOUT_INVALID) {
            /* Skip previous deactivated entry */
            source++;
            استمر;
        }

        אם (update->affect_mods) {
            entry->mods &= ~update->affect_mods;
            entry->mods |= (update->affect_mods & update->mods);
        }

        אם (update->source != XKB_LAYOUT_INVALID) {
            entry->target = (update->target == source)
                ? XKB_LAYOUT_INVALID
                : update->target;
            /* Single entry: exit loop */
            اكسر;
        }
        source++;
    }

    החזר XKB_SUCCESS;
}

/** Compare 2 modifiers combinations */
ساكن صحيح
cmp_mod_masks(ثابت فراغ *a, ثابت فراغ *b)
{
    ثابت xkb_mod_mask_t m1 = *((xkb_mod_mask_t *) a);
    ثابت xkb_mod_mask_t m2 = *((xkb_mod_mask_t *) b);
    אם (m1 == m2)
        החזר 0;

    אם (!m1)
        החזר +1;
    אם (!m2)
        החזר -1;

    ثابت xkb_mod_mask_t overlap = (m1 & m2);
    אם (overlap == m1)
        החזר +1; /* m1 is a subset of m2 */
    אם (overlap == m2)
        החזר -1; /* m2 is a subset of m1 */

    /* Compare active modifiers numbers */
    ثابت unsigned صحيح count1 = popcount32(m1);
    ثابت unsigned صحيح count2 = popcount32(m2);
    אם (count1 > count2)
        החזר -1;
    אם (count1 < count2)
        החזר +1;

    /* Give priority to real modifiers */
    החזר (m1 < m2) ? -1 : +1;
}

ساكن تعداد xkb_status
machine_set_mods(بنية xkb_machine *sm,
                 ثابت machine_mods_mappings *raw_mappings)
{
    אם (!darray_empty(*raw_mappings)) {
        machine_mods_mappings mappings = darray_new();
        xkb_mod_mask_t mask = 0;
        ثابت بنية machine_mods_mapping *mapping;
        ثابت xkb_mod_mask_t invalid =
            ~sm->base.base.keymap->canonical_state_mask;
        darray_foreach(mapping, *raw_mappings) {
            אם (!mapping->source || !mapping->target ||
                (mapping->source & invalid) || (mapping->target & invalid))
                استمر;
            darray_append(mappings, *mapping);
            mask |= mapping->source;
        }

        /*
         * Re-order the mappings, so that combinations have higher priority
         * if they are superset of other combinations.
         * For other type of overlaps, see the sorting function.
         */
        qsort(darray_items(mappings), darray_size(mappings),
              حجم(*darray_items(mappings)), &cmp_mod_masks);

        darray_steal(
            mappings,
            &sm->config.modifiers.mappings,
            &sm->config.modifiers.mappings_num
        );
        sm->config.modifiers.mask = mask;
    } אחרת {
        sm->config.modifiers = (بنية machine_modifiers_config) {
            .mappings = عدم,
            .mappings_num = 0,
            .mask = 0
        };
    }

    החזר XKB_SUCCESS;
}

ساكن تعداد xkb_status
machine_set_shortcuts(
    بنية xkb_machine * restrict sm,
    ثابت بنية xkb_shortcuts_config_options * restrict options
)
{
    אם (darray_empty(options->entries)) {
        sm->config.shortcuts = (بنية machine_shortcuts_config) {
            .entries = عدم
        };
        החזר XKB_SUCCESS;
    }

    بنية xkb_keymap * ثابت keymap = sm->base.base.keymap;

    بنية xkb_shortcuts_config_entry * entries =
        calloc(keymap->num_groups, حجم(*entries));
    אם (!entries)
        החזר XKB_ERROR_ALLOCATION_FAILURE;

    ثابت xkb_layout_index_t count = MIN(
        keymap->num_groups,
        (xkb_layout_index_t) darray_size(options->entries)
    );

    bool some_entries = false;
    עבור (xkb_layout_index_t l = 0; l < count; l++) {
        /* Sanitize layout target */
        ثابت xkb_layout_index_t target =
            darray_item(options->entries, l).target;
        /* Sanitize modifier mask */
        ثابت xkb_mod_mask_t mods = (
            darray_item(options->entries, l).mods &
            sm->base.base.keymap->canonical_state_mask
        );
        אם (target < keymap->num_groups && target != l && mods) {
            entries[l] = (بنية xkb_shortcuts_config_entry) {
                .target = target,
                .mods = mods
            };
            some_entries = true;
        } אחרת {
            entries[l] = (بنية xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
            استمر;
        }
    }

    אם (!some_entries) {
        حرر(entries);
        entries = عدم;
    } אחרת {
        /* Initialize groups with no entries */
        עבור (xkb_layout_index_t l = count; l < keymap->num_groups; l++) {
            entries[l] = (بنية xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
        }
    }

    sm->config.shortcuts = (بنية machine_shortcuts_config) {
        .entries = entries
    };
    החזר XKB_SUCCESS;
}

بنية xkb_machine *
xkb_machine_new(ثابت بنية xkb_machine_builder * restrict builder,
                تعداد xkb_status * restrict status)
{
    بنية xkb_machine * ثابت machine = calloc(1, حجم(*machine));
    אם (!machine) {
        log_err_func1(builder->keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine\n");
        אם (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        החזר عدم;
    }

    xkb_server_state_init(&machine->base, builder->keymap, SERVER_STATE,
                          builder->controls.a11y.affect_flags,
                          builder->controls.a11y.flags);
    machine->flags = builder->machine_flags;
    אם (!(machine->flags &
          XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_KEY_FIX))
    {
        machine->base.flags |= STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_KEY;
    }
    אם (machine->flags & XKB_MACHINE_UNLOCK_MODS_ON_PRESS) {
        machine->base.flags |= STATE_UNLOCK_MODS_ON_PRESS;
    }

    تعداد xkb_status status_;
    אם ((status_ = machine_set_mods(machine, &builder->mods)) != XKB_SUCCESS ||
        (status_ = machine_set_shortcuts(machine, &builder->shortcuts)) !=
        XKB_SUCCESS) {
        *status = status_;
        اذهب error;
    }

    darray_init(machine->overlays.keys);
    machine->mouse.default_button = XKB_POINTER_BUTTON_MIN;

    אם (status)
        *status = XKB_SUCCESS;

    החזר machine;

error:
    xkb_machine_unref(machine);
    החזר عدم;
}

بنية xkb_machine *
xkb_machine_ref(بنية xkb_machine *sm)
{
    أكد(sm->base.base.refcnt > 0);
    sm->base.base.refcnt++;
    החזר sm;
}

فراغ
xkb_machine_unref(بنية xkb_machine *sm)
{
    أكد(!sm || sm->base.base.refcnt > 0);
    אם (!sm || --sm->base.base.refcnt > 0)
        החזר;

    xkb_state_destroy(&sm->base.base);
    darray_free(sm->overlays.keys);
    حرر(sm->config.shortcuts.entries);
    حرر(sm->config.modifiers.mappings);
    حرر(sm);
}

بنية xkb_keymap *
xkb_machine_get_keymap(ثابت بنية xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    החזר sm->base.base.keymap;
}

بنية xkb_state *
xkb_machine_get_state(بنية xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    החזר (بنية xkb_state *)sm;
}

بنية xkb_state *
xkb_state_new_from_machine(ثابت بنية xkb_machine * restrict machine,
                           تعداد xkb_status * restrict status)
{
    /* Creation */
    تعداد xkb_status status_;
    بنية xkb_state * ثابت state = xkb_state_new_with_mode(
        machine->base.base.keymap,
        XKB_STATE_MODE_SERVER_QUERY,
        &status_
    );
    אם (status_ != XKB_SUCCESS) {
        אם (status)
            *status = status_;
        החזר عدم;
    }

    أكد(state->mode == SERVER_COMPANION);
    أكد(state->refcnt == 1);

    /* Copy machine’s state */
    *state = machine->base.base;

    /* Restore previous values */
    state->mode = SERVER_COMPANION;
    state->refcnt = 1;

    אם (status)
        *status = XKB_SUCCESS;
    החזר state;
}

ساكن فراغ
machine_update_overlays(بنية xkb_machine *sm)
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
    עבור (uint8_t n = 0; n < overlay_max;) {
        xkb_overlay_index_t overlay_idx = (order >> (n * 4)) & 0xf;
        אם (!overlay_idx)
            اكسر;
        ثابت xkb_overlay_mask_t overlay_mask =
            (xkb_overlay_mask_t)(1u << (overlay_idx - 1u) /* k is 1-indexed */);
        אם (overlay_mask & mask) {
            /* no duplicates */
            added &= ~overlay_mask;
            n++;
        } אחרת {
            /* remove overlay */
            ثابت uint32_t head = (UINT32_C(1) << (n * 4)) - 1;
            order = (order & head) | ((order >> 4) & ~head);
        }
    }

    /* Add new overlays in ascending order */
    עבור (xkb_overlay_index_t k = 0; added; k++, added >>= 1) {
        אם (added & 0x1) {
            order <<= 4;
            order |= k + 1u; /* k is 0-indexed */
        }
    }

    sm->overlays.order = order;
    sm->overlays.enabled = mask;
}

تعداد xkb_status
xkb_machine_process_synthetic(بنية xkb_machine *sm,
                              ثابت بنية xkb_synthetic_update *update,
                              بنية xkb_events *events)
{
    /* Check ABI compatibility */
    تعداد xkb_status status =
        check_state_update_abi(sm->base.base.keymap->ctx, update);
    אם (status != XKB_SUCCESS)
        החזר status;

    بنية xkb_server_state * ثابت state = &sm->base;
    ثابت بنية state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    darray_size(events->queue) = 0;
    events->next = 0;

    /* Update parametrized controls first */
    אם (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        אם (status != XKB_SUCCESS)
            החזר status;
    }

    אם (update->components) {
        ثابت بنية xkb_state_components_update * ثابت components =
            update->components;
        /* Update boolean controls first */
        אם (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, events);
        }

        state_update_latched_locked(state, components, events);
    }

    xkb_state_update_derived(&state->base);

    ثابت تعداد xkb_state_component changed = get_state_component_changes(
        &previous_components, &state->base.components
    );
    אם (changed) {
        // TODO: latch controls
        אם (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        /* Create event only if some component actually changed */
        darray_append(events->queue, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .changed = changed,
                .components = state->base.components
            }
        });

        /* Terminate the frame */
        darray_append(events->queue, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    החזר XKB_SUCCESS;
}

ساكن ssize_t
do_remap_modifiers(ثابت بنية machine_modifiers_config * restrict mappings,
                   بنية xkb_state * restrict state,
                   بنية xkb_events * restrict events,
                   ثابت بنية xkb_key * restrict key)
{
    אם (!(mappings->mask & state->components.mods))
        החזר -1;

    ثابت xkb_layout_index_t layout = state_key_get_layout(state, key);
    אם (layout >= key->num_groups)
        החזר -1;

    ثابت بنية xkb_key_type * ثابت type = key->groups[layout].type;
    xkb_mod_mask_t affect_mods = 0;
    xkb_mod_mask_t mods = 0;
    עבור (darray_size_t m = 0; m < mappings->mappings_num; m++) {
        ثابت بنية machine_mods_mapping * ثابت mapping =
            &mappings->mappings[m];
        אם (/* Skip not matching active mods */
            (mapping->source & state->components.mods) == mapping->source &&
            /* Skip overlap with previous active mappings */
            !(mapping->source & affect_mods) &&
            /* Skip if the key type uses some of the source modifiers */
            !(mapping->source & type->mods.mask)) {
            affect_mods |= mappings->mappings[m].source;
            mods |= mappings->mappings[m].target;
        }
    }

    אם (!affect_mods)
        החזר -1;

    بنية xkb_state new = *state;
    new.components.base_mods = (new.components.base_mods & ~affect_mods) | mods;
    new.components.latched_mods = (new.components.latched_mods & ~affect_mods);
    new.components.locked_mods = (new.components.locked_mods & ~affect_mods);
    xkb_state_update_derived(&new);

    ssize_t event_idx = -1;
    ثابت تعداد xkb_state_component changed =
        get_state_component_changes(&state->components, &new.components);
    אם (changed) {
        event_idx = (ssize_t) darray_size(events->queue);
        darray_append(events->queue, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = new.components,
                .changed = changed
            }
        });
    }

    state->components.mods = new.components.mods;
    החזר event_idx;
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
                   ثابت بنية state_components *previous_components,
                   بنية xkb_events *events, ssize_t remap_event)
{
    אם (!config->entries) {
        /* No shortcuts tweak */
        החזר remap_event;
    }

    ثابت بنية xkb_shortcuts_config_entry * ثابت entry =
        &config->entries[state->components.group];

    אם (entry->target != XKB_LAYOUT_INVALID &&
        (entry->mods & state->components.mods)) {
        /*
         * Activate shortcuts tweak:
         * 1. The real base group is saved by the caller, to be restored by
         *    later by a call to undo_tweaks().
         * 2. Compute the new base group so that the resulting effective group
         *    is the target.
         * 3. Recompute derived state components.
         */
        بنية xkb_state new = *state;
        אם (remap_event < 0) {
            /* Create new event */
            remap_event = (ssize_t) darray_size(events->queue);
            darray_append(events->queue, (بنية xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = {0},
                    .changed = 0
                }
            });
        } אחרת {
            /* Merge with last remap event */
            new.components =
                darray_item(events->queue, remap_event).components.components;
        }
        new.components.base_group
            = (int32_t) entry->target
            - state->components.latched_group
            - state->components.locked_group;
        xkb_state_update_derived(&new);

        ثابت تعداد xkb_state_component changed =
            get_state_component_changes(previous_components, &new.components);
        darray_item(events->queue, remap_event).components.changed = changed;
        darray_item(events->queue, remap_event).components.components =
            new.components;

        state->components.group = new.components.group;
    } אחרת {
        /* Deactivated entry or modifiers do not match */
    }
    החזר remap_event;
}

ساكن فراغ
undo_tweaks(ثابت بنية xkb_state *state,
            ثابت بنية state_components *initial_components,
            بنية xkb_events *events)
{
    /* Get last component event */
    ثابت بنية xkb_event *event = عدم;
    darray_foreach_reverse(event, events->queue) {
        אם (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS)
            اكسر;
    }
    אם (!event)
        החזר;

    /* Restore state */
    ثابت تعداد xkb_state_component changed =
        get_state_component_changes(initial_components,
                                    &event->components.components);
    אם (changed) {
        darray_append(events->queue, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = *initial_components,
                .changed = changed
            }
        });
    }
}

ساكن ثابت بنية xkb_key *
process_overlayable_key(بنية xkb_machine *sm,
                        ثابت بنية xkb_key * key,
                        تعداد xkb_key_direction direction)
{
    /* Check if key is currently overlaid */
    بنية xkb_overlaid_key *entry = عدم;
    بنية xkb_overlaid_key *available_entry = عدم;
    darray_foreach(entry, sm->overlays.keys) {
        אם (entry->old == key) {
            /* Maintain the key overlaid */
            بدل (direction) {
            حالة XKB_KEY_DOWN:
                entry->refcnt++;
                اكسر;
            حالة XKB_KEY_REPEATED:
                اكسر;
            حالة XKB_KEY_UP:
            افتراضي:
                entry->refcnt--;
                اكسر;
            }
            אם (entry->refcnt <= 0)
                entry->old = عدم;
            החזר entry->new;
        } אחרת אם (!entry->old && !available_entry) {
            /* First free spot */
            available_entry = entry;
        }
    }
    אם (direction == XKB_KEY_DOWN) {
        /* Enable key overlay */

        /*
         * Get overlay index: select the latest activated overlay
         * containing the key.
         */
        ثابت بنية xkb_key *new = key;

        אם (key->overlays & sm->overlays.enabled) {
            /* Some relevant overlay is active */
            עבור (uint32_t stack = sm->overlays.order; stack; stack >>= 4) {
                static_assert(XKB_OVERLAY_COUNT == 8, "");
                ثابت xkb_overlay_index_t overlay = (stack & 0xf) - 1;
                ثابت xkb_overlay_mask_t mask =
                    (xkb_overlay_mask_t)(1u << overlay);
                אם (key->overlays & mask) {
                    /* Get new key */
                    אם (key->overlays_inline) {
                        /* Stored inline */
                        new = key->overlay_key;
                    } אחרת {
                        /* Stored in sparse array */
                        ثابت xkb_overlay_mask_t low = (xkb_overlay_mask_t)(
                            key->overlays & (xkb_overlay_mask_t)(mask - 1u)
                        );
                        /* index = number of set bits strictly below bit */
                        ثابت xkb_overlay_index_t index =
                            (xkb_overlay_index_t)popcount32(low);
                        new = key->overlays_keys[index];
                    }
                    اكسر;
                }
            }
        } אחרת {
            /*
             * No relevant overlay is active: block future overlays while
             * the key is pressed.
             */
        }

        /* Get new entry */
        אם (available_entry) {
            /* Re-use a free entry */
            entry = available_entry;
        } אחרת {
            /* Append a new entry */
            ثابت darray_size_t idx = darray_size(sm->overlays.keys);
            darray_resize(sm->overlays.keys, idx + 1);
            entry = &darray_item(sm->overlays.keys, idx);
        }

        /* Overlay the key */
        entry->old = key;
        entry->new = new;
        entry->refcnt = 1;
        החזר new;
    }

    /* No overlay */
    החזר key;
}

تعداد xkb_status
xkb_machine_process_key(بنية xkb_machine *sm,
                        xkb_keycode_t kc, تعداد xkb_key_direction direction,
                        بنية xkb_events *events)
{
    darray_size(events->queue) = 0;
    events->next = 0;

    بنية xkb_server_state * ثابت state = &sm->base;
    ثابت بنية xkb_key * key = XkbKey(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    אם (!key || (direction == XKB_KEY_REPEATED && !key->repeats))
        החזר XKB_SUCCESS;

    ثابت بنية state_components initial_components = state->base.components;

    אם (direction == XKB_KEY_DOWN &&
        (sm->base.base.components.controls & XKB_KEYBOARD_CONTROL_A11Y_STICKY_KEYS) &&
        (sm->base.a11y_flags & XKB_A11Y_STICKY_KEYS_NO_SIMULTANEOUS_KEYS) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11Y_STICKY_KEYS,
                                      0, events);
    }

    אם (key->overlays)
        key = process_overlayable_key(sm, key, direction);

    ssize_t remap_event = do_remap_modifiers(&sm->config.modifiers,
                                             &state->base, events, key);

    remap_event = do_shortcuts_tweak(&sm->config.shortcuts, &state->base,
                                     &initial_components, events, remap_event);

    אם (remap_event >= 0) {
        // TODO: what about merging state event for RedirectKey?
        darray_append(events->queue, (بنية xkb_event) {
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
    עבור (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        אם (state->set_mods & bit) {
            state->mod_key_count[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    עבור (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        אם (state->clear_mods & bit) {
            state->mod_key_count[i]--;
            אם (state->mod_key_count[i] <= 0) {
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
    אם (state->flags & STATE_REQUIRE_KEY_EVENT) {
        darray_append(events->queue, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_KEY,
            .key = {
                .keycode = key->keycode,
                .direction = direction
            }
        });
    }

    אם (remap_event >= 0) {
        // FIXME: fragile if last state change does not restore the state to the remap event
        undo_tweaks(&state->base, &initial_components, events);
    }

    ثابت تعداد xkb_state_component changed = get_state_component_changes(
        &initial_components, &state->base.components
    );
    אם (changed) {
        אם (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        // TODO: merge with previous state event, if any

        darray_append(events->queue, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = state->base.components,
                .changed = changed
            }
        });
    }

    אם (!darray_empty(events->queue) &&
        darray_item(events->queue, darray_size(events->queue) - 1).type !=
        XKB_EVENT_TYPE_FRAME)
    {
        darray_append(events->queue, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    החזר XKB_SUCCESS;
}

بنية xkb_events *
xkb_events_new(بنية xkb_context * restrict context,
               ثابت بنية xkb_events_config * restrict config,
               تعداد xkb_status * restrict status)
{
    /* Handle default configuration */
    ساكن ثابت بنية xkb_events_config default_config = {
        .size = حجم(default_config)
    };
    אם (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ثابت تعداد xkb_status abi_error = xkb_check_state_abi(config);
    אם (abi_error) {
        xkb_log_abi_error(context, __func__, abi_error);
        אם (status)
            *status = abi_error;
        החזר عدم;
    }

    /* Sanitize input */
    ثابت uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_EVENTS_FLAGS_VALUES);
    אם (invalid_flags) {
        log_err_func(context, XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS_,
                     "unrecognized events collection flags: 0x%"PRIx32"\n",
                     invalid_flags);
        אם (status)
            *status = XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS;
        החזר عدم;
    }

    بنية xkb_events *events = calloc(1, حجم(*events));
    אם (!events) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events collection\n");
        אם (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        החזר عدم;
    }

    /* Pre-allocate event queue */
    darray_init(events->queue);
    ساكن ثابت darray_size_t queue_initial_size = 16;
    darray_resize(events->queue, queue_initial_size);
    // TODO: proper safe darray_resize() with return value
    אם (!darray_items(events->queue)) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events queue\n");
        xkb_events_destroy(events);
        אם (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        החזר عدم;
    }
    darray_size(events->queue) = 0;

    events->ctx = xkb_context_ref(context);
    events->next = 0;

    אם (status)
        *status = XKB_SUCCESS;
    החזר events;
}

فراغ
xkb_events_destroy(بنية xkb_events *events)
{
    אם (!events)
        החזר;
    darray_free(events->queue);
    xkb_context_unref(events->ctx);
    حرر(events);
}

ثابت بنية xkb_event *
xkb_events_next(بنية xkb_events *events)
{
    אם (events->next < darray_size(events->queue)) {
        ثابت darray_size_t index = events->next++;
        החזר &darray_item(events->queue, index);
    } אחרת {
        החזר عدم;
    }
}

تعداد xkb_event_type
xkb_event_get_type(ثابت بنية xkb_event *event)
{
    החזר event->type;
}

تعداد xkb_status
xkb_event_get_keycode(ثابت بنية xkb_event *event,
                      xkb_keycode_t *keycode,
                      تعداد xkb_key_direction *direction)
{
    אם (event->type == XKB_EVENT_TYPE_KEY) {
        *keycode = event->key.keycode;
        *direction = event->key.direction;
        החזר XKB_SUCCESS;
    } אחרת {
        החזר XKB_ERROR_EVENT_TYPE_MISMATCH;
    }
}

تعداد xkb_status
xkb_event_get_components(ثابت بنية xkb_event * restrict event,
                         بنية xkb_event_components * restrict components)
{
    אם (event->type != XKB_EVENT_TYPE_STATE_COMPONENTS)
        החזר XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ثابت تعداد xkb_status status = xkb_check_state_abi(components);
    אם (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        החזר status;
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

    החזר XKB_SUCCESS;
}

تعداد xkb_status
xkb_event_get_pointer_motion(ثابت بنية xkb_event * restrict event,
                             بنية xkb_event_pointer_motion * restrict motion)
{
    אם (event->type != XKB_EVENT_TYPE_POINTER_MOTION)
        החזר XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ثابت تعداد xkb_status status = xkb_check_state_abi(motion);
    אם (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        החזר status;
    }

    motion->flags = event->pointer_motion.flags;
    motion->x = event->pointer_motion.x;
    motion->y = event->pointer_motion.y;

    החזר XKB_SUCCESS;
}

تعداد xkb_status
xkb_event_get_pointer_button(ثابت بنية xkb_event * restrict event,
                             بنية xkb_event_pointer_button * restrict button)
{
    אם (event->type != XKB_EVENT_TYPE_POINTER_BUTTON)
        החזר XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ثابت تعداد xkb_status status = xkb_check_state_abi(button);
    אם (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        החזר status;
    }

    button->button = event->pointer_button.button;
    button->state = event->pointer_button.state;
    button->count = event->pointer_button.count;

    החזר XKB_SUCCESS;
}

تعداد xkb_status
xkb_event_get_virtual_console(ثابت بنية xkb_event * restrict event,
                              int8_t * restrict index_or_offset,
                              bool * restrict is_offset)
{
    אם (event->type != XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
        החזר XKB_ERROR_EVENT_TYPE_MISMATCH;

    *index_or_offset = event->virtual_console.index_or_offset;
    *is_offset = event->virtual_console.is_offset;

    החזר XKB_SUCCESS;
}

تعداد xkb_status
xkb_state_update_event(بنية xkb_state * restrict base_state,
                       ثابت بنية xkb_event * restrict event,
                       تعداد xkb_state_component * restrict changed)
{
    /* Guard against server-only state */
    أكد(base_state->mode == SERVER_COMPANION ||
           base_state->mode == LEGACY_MIXED_STATE);
    אם (base_state->mode != SERVER_COMPANION &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE_,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        החזר XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    بنية xkb_client_state * ثابت state =
        (بنية xkb_client_state *)base_state;

    אם (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
        ثابت بنية state_components prev_components = state->base.components;
        state->base.components = event->components.components;
        /*
         * Recompute the changes instead of using the event value, because we do
         * not know if the event’s queue and the state are synced.
         */
        אם (changed)
            *changed = get_state_component_changes(&prev_components,
                                                   &state->base.components);
    } אחרת {
        אם (changed)
            *changed = 0;
    }
    החזר XKB_SUCCESS;
}

ساكن فراغ
append_pointer_button(بنية xkb_events * restrict events,
                      بنية xkb_filter * restrict filter,
                      تعداد xkb_pointer_button_state button_state,
                      uint8_t count)
{
    darray_append(events->queue, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_BUTTON,
        .pointer_button = {
            .size = حجم(((بنية xkb_event*)0)->pointer_button),
            .button = filter->priv,
            .state = button_state,
            .count = count,
        }
    });
}

ساكن فراغ
xkb_filter_pointer_button_new(بنية xkb_server_state *state,
                              بنية xkb_events *events,
                              بنية xkb_filter *filter)
{
    אם (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_KEYS)) {
        filter->func = عدم;
        החזר;
    }

    بنية xkb_machine * ثابت sm = (بنية xkb_machine *)state;

    /* Resolve button */
    ثابت xkb_pointer_button_index_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;
    filter->priv = button;

    ثابت تعداد xkb_pointer_button_state button_state =
        XKB_POINTER_BUTTON_PRESSED;
    append_pointer_button(events, filter, button_state,
                          filter->action.btn.count);
    state->flags &= ~STATE_REQUIRE_KEY_EVENT;
}

ساكن bool
xkb_filter_pointer_button_func(بنية xkb_server_state *state,
                               بنية xkb_events *events,
                               بنية xkb_filter *filter,
                               ثابت بنية xkb_key *key,
                               تعداد xkb_key_direction direction)
{
    אם (key != filter->key || !events)
        החזר XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_KEY_EVENT;

    بدل (direction) {
    حالة XKB_KEY_DOWN:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_KEY_REPEATED:
        החזר XKB_FILTER_CONSUME;
    افتراضي:
        אם (--filter->refcnt > 0)
            החזר XKB_FILTER_CONSUME;
    }

    אם (!filter->action.btn.count)
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);

    filter->func = عدم;
    החזר XKB_FILTER_CONSUME;
}

بنية xkb_filter_pointer_lock_button_priv {
    uint32_t button:31;
#ifdef _WIN32
    uint32_t locked:1; // MSVC requires matching types to pack bitfields
#אחרת
    bool locked:1;
#endif
};

تعداد {
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH =
        حجم(بنية xkb_filter_pointer_lock_button_priv) * CHAR_BIT,
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX =
        (UINT64_C(1) << XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH) - 1
};

static_assert(حجم(بنية xkb_filter_pointer_lock_button_priv) ==
              حجم(((بنية xkb_filter*)0)->priv),
              "");
static_assert((uint32_t)XKB_POINTER_BUTTON_MAX < (uint32_t)XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX,
              "xkb_filter_pointer_lock_button_priv cannot store button");

ساكن فراغ
xkb_filter_pointer_lock_button_new(بنية xkb_server_state *state,
                                   بنية xkb_events *events,
                                   بنية xkb_filter *filter)
{
    אם (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_KEYS)) {
        filter->func = عدم;
        החזר;
    }

    state->flags &= ~STATE_REQUIRE_KEY_EVENT;

    بنية xkb_machine * ثابت sm = (بنية xkb_machine *)state;

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

    אם (!locked && !(filter->action.btn.flags & ACTION_LOCK_NO_LOCK)) {
        sm->mouse.locked_buttons |= mask;
        filter->priv = button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_PRESSED, 0);
    }

    بنية xkb_filter_pointer_lock_button_priv *priv =
        (بنية xkb_filter_pointer_lock_button_priv*)&filter->priv;
    priv->button = button,
    priv->locked = locked;
}

ساكن bool
xkb_filter_pointer_lock_button_func(بنية xkb_server_state *state,
                                    بنية xkb_events *events,
                                    بنية xkb_filter *filter,
                                    ثابت بنية xkb_key *key,
                                    تعداد xkb_key_direction direction)
{
    אם (key != filter->key || !events || state->base.mode != SERVER_STATE)
        החזר XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_KEY_EVENT;

    بدل (direction) {
    حالة XKB_KEY_DOWN:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_KEY_REPEATED:
        החזר XKB_FILTER_CONSUME;
    افتراضي:
        אם (--filter->refcnt > 0)
            החזר XKB_FILTER_CONSUME;
    }

    ثابت بنية xkb_filter_pointer_lock_button_priv *priv =
        (بنية xkb_filter_pointer_lock_button_priv*)&filter->priv;

    אם (priv->locked && !(filter->action.btn.flags & ACTION_LOCK_NO_UNLOCK)) {
        بنية xkb_machine * ثابت sm = (بنية xkb_machine *)state;

        static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                    "invalid UINTx_C() macro");
        static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                    "invalid left shift");
        ثابت xkb_pointer_button_mask_t mask = (UINT8_C(1) << priv->button);

        sm->mouse.locked_buttons &= ~mask;
        filter->priv = priv->button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);
    }

    filter->func = عدم;
    החזר XKB_FILTER_CONSUME;
}

ساكن فراغ
xkb_filter_pointer_set_default_button_new(بنية xkb_server_state *state,
                                          بنية xkb_events *events,
                                          بنية xkb_filter *filter)
{
    אם (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_KEYS)) {
        filter->func = عدم;
        החזר;
    }

    state->flags &= ~STATE_REQUIRE_KEY_EVENT;

    بنية xkb_machine * ثابت sm = (بنية xkb_machine *)state;

    static_assert(2 * XKB_POINTER_BUTTON_MAX <= INT_MAX, "lossy conversion");
    صحيح button = (filter->action.dflt.flags & ACTION_ABSOLUTE_SWITCH)
        ? filter->action.dflt.value
        : sm->mouse.default_button + filter->action.dflt.value;

    /* Wrap into range */
    ثابت صحيح rem = button % XKB_POINTER_BUTTON_MAX;
    sm->mouse.default_button = (rem >= 0)
        ? (xkb_pointer_button_index_t)rem
        : (xkb_pointer_button_index_t)(rem + XKB_POINTER_BUTTON_MAX);
}

ساكن bool
xkb_filter_pointer_set_default_button_func(بنية xkb_server_state *state,
                                           بنية xkb_events *events,
                                           بنية xkb_filter *filter,
                                           ثابت بنية xkb_key *key,
                                           تعداد xkb_key_direction direction)
{
    אם (key != filter->key || !events)
        החזר XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_KEY_EVENT;

    بدل (direction) {
    حالة XKB_KEY_DOWN:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_KEY_REPEATED:
        החזר XKB_FILTER_CONSUME;
    افتراضي:
        אם (--filter->refcnt > 0)
            החזר XKB_FILTER_CONSUME;
    }

    filter->func = عدم;
    החזר XKB_FILTER_CONTINUE;
}

ساكن فراغ
xkb_filter_terminate_server_new(بنية xkb_server_state *state,
                                 بنية xkb_events *events,
                                 بنية xkb_filter *filter)
{
    אם (!events || state->base.mode != SERVER_STATE ||
        !(((بنية xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = عدم;
        החזר;
    }

    state->flags &= ~STATE_REQUIRE_KEY_EVENT;

    darray_append(events->queue, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    });
}

ساكن bool
xkb_filter_terminate_server_func(بنية xkb_server_state *state,
                                 بنية xkb_events *events,
                                 بنية xkb_filter *filter,
                                 ثابت بنية xkb_key *key,
                                 تعداد xkb_key_direction direction)
{
    אם (key != filter->key || !events)
        החזר XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_KEY_EVENT;

    بدل (direction) {
    حالة XKB_KEY_DOWN:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_KEY_REPEATED:
        החזר XKB_FILTER_CONSUME;
    افتراضي:
        אם (--filter->refcnt > 0)
            החזר XKB_FILTER_CONSUME;
    }

    filter->func = عدم;
    החזר XKB_FILTER_CONSUME;
}

ساكن فراغ
xkb_filter_switch_virtual_console_new(بنية xkb_server_state *state,
                                      بنية xkb_events *events,
                                      بنية xkb_filter *filter)
{
    אם (!events || state->base.mode != SERVER_STATE ||
        !(((بنية xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = عدم;
        החזר;
    }

    state->flags &= ~STATE_REQUIRE_KEY_EVENT;

    darray_append(events->queue, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
        .virtual_console = {
            .index_or_offset = filter->action.screen.screen,
            .is_offset = !(filter->action.screen.flags & ACTION_ABSOLUTE_SWITCH)
        }
    });
}

ساكن bool
xkb_filter_switch_virtual_console_func(بنية xkb_server_state *state,
                                       بنية xkb_events *events,
                                       بنية xkb_filter *filter,
                                       ثابت بنية xkb_key *key,
                                       تعداد xkb_key_direction direction)
{
    אם (key != filter->key || !events)
        החזר XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_KEY_EVENT;

    بدل (direction) {
    حالة XKB_KEY_DOWN:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_KEY_REPEATED:
        החזר XKB_FILTER_CONSUME;
    افتراضي:
        אם (--filter->refcnt > 0)
            החזר XKB_FILTER_CONSUME;
    }

    filter->func = عدم;
    החזר XKB_FILTER_CONSUME;
}
