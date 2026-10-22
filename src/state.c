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

#समावेश "config.h"

#समावेश <assert.h>
#समावेश <limits.h>
#समावेश <stdint.h>
#समावेश <stdlib.h>
#समावेश <string.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "xkbcommon/xkbcommon-status.h"
#समावेश "xkbcommon/xkbcommon-features.h"
#समावेश "abi-check.h"
#समावेश "darray.h"
#समावेश "features/enums.h"
#समावेश "keymap.h"
#समावेश "keysym.h"
#समावेश "messages-codes.h"
#समावेश "state-priv.h"
#समावेश "utf8.h"
#समावेश "utils.h"
#समावेश "util-mem.h"
#समावेश "util-numbers.h"

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
काष्ठा xkb_events {
    काष्ठा xkb_context *ctx;
    darray(काष्ठा xkb_event) queue;
    /**
     * Read cursor for `xkb_events_next()`.
     *
     * Reset to 0 on each `process_*` call.
     */
    darray_आकार_t अग्र;
};

काष्ठा xkb_server_state;

काष्ठा xkb_filter {
    जोड़ xkb_action action;
    स्थिर काष्ठा xkb_कुंजी *कुंजी;
    bool (*func)(काष्ठा xkb_server_state *state,
                 काष्ठा xkb_events *events,
                 काष्ठा xkb_filter *filter,
                 स्थिर काष्ठा xkb_कुंजी *कुंजी,
                 क्रमागत xkb_कुंजी_direction direction);
    uint32_t priv;
    पूर्णांक refcnt;
};

क्रमागत xkb_state_mode_internal {
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
    XKB_STATE_TYPE_अंक_ENTRIES = 5
};

static_assert(CLIENT_STATE == 0 &&
              CLIENT_STATE == SERVER_COMPANION - 1 &&
              SERVER_COMPANION == LEGACY_MIXED_STATE - 1 &&
              LEGACY_MIXED_STATE == LEGACY_SERVER_STATE - 1 &&
              LEGACY_SERVER_STATE == SERVER_STATE - 1 &&
              SERVER_STATE == XKB_STATE_TYPE_अंक_ENTRIES - 1 &&
              XKB_STATE_TYPE_अंक_ENTRIES == 5,
              "Order is used to guard client/server -only APIs: do not change");

क्रमागत { XKB_LAYOUT_सूचकांक_T_MIN_WIDTH = 6 };
static_assert(XKB_MAX_GROUPS < (1 << XKB_LAYOUT_सूचकांक_T_MIN_WIDTH),
              "Cannot encode out_of_range_group::redirect_group");

काष्ठा out_of_range_group {
    क्रमागत xkb_layout_out_of_range_policy policy :
        (माप(क्रमागत xkb_layout_out_of_range_policy) * अक्षर_बिट -
         XKB_LAYOUT_सूचकांक_T_MIN_WIDTH);
    xkb_layout_सूचकांक_t redirect_group : XKB_LAYOUT_सूचकांक_T_MIN_WIDTH;
};

क्रमागत { XKB_STATE_MODE_INTERNAL_MIN_WIDTH = 4 /* 3 bits + sign */ };
static_assert(XKB_STATE_TYPE_अंक_ENTRIES <
              (1 << XKB_STATE_MODE_INTERNAL_MIN_WIDTH),
              "Cannot encode xkb_state::mode");

/**
 * Common state
 *
 * `mode` indicates whether the object is casted from a `xkb_server_state` or a
 * `xkb_client_state` struct.
 */
काष्ठा xkb_state {
    काष्ठा xkb_keymap *keymap;
    /*
     * Before updating the state, we keep a copy of just this struct. This
     * allows us to report which components of the state have changed.
     */
    काष्ठा state_components components;
    काष्ठा out_of_range_group out_of_range_group;

    क्रमागत xkb_state_mode_internal mode : XKB_STATE_MODE_INTERNAL_MIN_WIDTH;
    पूर्णांक refcnt : (माप(पूर्णांक) * अक्षर_बिट - XKB_STATE_MODE_INTERNAL_MIN_WIDTH);
};

/**
 * Minimal client state
 *
 * Wrapper for strong typing
 */
काष्ठा xkb_client_state {
    /** Inherits from `xkb_state` */
    काष्ठा xkb_state base;
};

क्रमागत state_flags {
    STATE_NO_FLAGS = 0,
    STATE_REQUIRE_कुंजी_EVENT = (1 << 0),
    STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_कुंजी = (1 << 1),
    STATE_UNLOCK_MODS_ON_PRESS = (1 << 2),
};

/**
 * Legacy server state, used for `LEGACY_MIXED_STATE` and `LEGACY_SERVER_STATE`
 * modes.
 *
 * Casted as `xkb_state` in the public API. See `xkb_state::mode`.
 */
काष्ठा xkb_server_state {
    /** Inherits from `xkb_state` */
    काष्ठा xkb_state base;

    /* Server-specific state data */

    darray(काष्ठा xkb_filter) filters;

    /**
     * Mix of constant configuration flags and
     * variable state-processing flags.
     */
    क्रमागत state_flags flags;

    क्रमागत xkb_a11y_flags a11y_flags;

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
    int16_t mod_कुंजी_गिनती[XKB_MAX_MODS];
};

अटल स्थिर काष्ठा xkb_कुंजी_type_entry *
get_entry_for_mods(स्थिर काष्ठा xkb_कुंजी_type *type, xkb_mod_mask_t mods)
{
    क्रम (darray_आकार_t i = 0; i < type->अंक_entries; i++)
        अगर (entry_is_active(&type->entries[i]) &&
            type->entries[i].mods.mask == mods)
            वापस &type->entries[i];
    वापस शून्य;
}

अटल स्थिर काष्ठा xkb_कुंजी_type_entry *
get_entry_for_कुंजी_state(काष्ठा xkb_state *state, स्थिर काष्ठा xkb_कुंजी *कुंजी,
                        xkb_layout_सूचकांक_t group)
{
    स्थिर काष्ठा xkb_कुंजी_type* स्थिर type = कुंजी->groups[group].type;
    xkb_mod_mask_t active_mods = state->components.mods & type->mods.mask;
    वापस get_entry_for_mods(type, active_mods);
}

अटल अंतरभूत xkb_level_सूचकांक_t
state_कुंजी_get_level(काष्ठा xkb_state *state, स्थिर काष्ठा xkb_कुंजी *कुंजी,
                         xkb_layout_सूचकांक_t layout)
{
    अगर (layout >= कुंजी->अंक_groups)
        वापस XKB_LEVEL_INVALID;

    /* If we don't find an explicit match the default is 0. */
    स्थिर काष्ठा xkb_कुंजी_type_entry* स्थिर entry =
        get_entry_for_कुंजी_state(state, कुंजी, layout);

    वापस (entry) ? entry->level : 0;
}

/**
 * Returns the level to use for the given key and state, or
 * XKB_LEVEL_INVALID.
 */
xkb_level_सूचकांक_t
xkb_state_कुंजी_get_level(काष्ठा xkb_state *state, xkb_keycode_t kc,
                        xkb_layout_सूचकांक_t layout)
{
    स्थिर काष्ठा xkb_कुंजी* स्थिर कुंजी = Xkb_कुंजी(state->keymap, kc);

    वापस (कुंजी)
        ? state_कुंजी_get_level(state, कुंजी, layout)
        : XKB_LEVEL_INVALID;
}

अटल अंतरभूत xkb_layout_सूचकांक_t
state_कुंजी_get_layout(काष्ठा xkb_state *state, स्थिर काष्ठा xkb_कुंजी *कुंजी)
{
    वापस xkb_keymap_कुंजी_effective_layout(कुंजी, state->components.group);
}

/**
 * Returns the layout to use for the given key and state, taking
 * wrapping/clamping/etc into account, or XKB_LAYOUT_INVALID.
 */
xkb_layout_सूचकांक_t
xkb_state_कुंजी_get_layout(काष्ठा xkb_state *state, xkb_keycode_t kc)
{
    स्थिर काष्ठा xkb_कुंजी* स्थिर कुंजी = Xkb_कुंजी(state->keymap, kc);

    अगर (!कुंजी)
        वापस XKB_LAYOUT_INVALID;

    वापस state_कुंजी_get_layout(state, कुंजी);
}

/* Empty action used for empty levels */
अटल स्थिर जोड़ xkb_action dummy_action = { .type = ACTION_TYPE_NONE };

अटल xkb_action_गिनती_t
xkb_कुंजी_get_actions(काष्ठा xkb_state *state, स्थिर काष्ठा xkb_कुंजी *कुंजी,
                    स्थिर जोड़ xkb_action **actions)
{
    स्थिर xkb_layout_सूचकांक_t layout = state_कुंजी_get_layout(state, कुंजी);
    स्थिर xkb_level_सूचकांक_t level = state_कुंजी_get_level(state, कुंजी, layout);
    अगर (level == XKB_LEVEL_INVALID)
        जाओ err;

    स्थिर xkb_action_गिनती_t गिनती =
        xkb_keymap_कुंजी_get_actions_by_level(state->keymap, कुंजी,
                                            layout, level, actions);
    अगर (!गिनती)
        जाओ err;

    वापस गिनती;

err:
    /* Use a dummy action if no corresponding level was found or if it is empty.
     * This is required e.g. to handle latches properly. */
    *actions = &dummy_action;
    वापस 1;
}

अटल काष्ठा xkb_filter *
xkb_filter_new(काष्ठा xkb_server_state *state)
{
    काष्ठा xkb_filter *filter = शून्य, *iter;

    darray_foreach(iter, state->filters) {
        अगर (iter->func)
            जारी;
        /* Use available slot */
        filter = iter;
        अवरोध;
    }

    अगर (!filter) {
        /* No available slot: resize the filters array */
        darray_resize0(state->filters, darray_आकार(state->filters) + 1);
        filter = &darray_item(state->filters, darray_आकार(state->filters) -1);
    }

    filter->refcnt = 1;
    वापस filter;
}

/***====================================================================***/

क्रमागत xkb_filter_परिणाम {
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
#घोषणा apply_group_delta(filter_, state_, component_)                   \
    अगर ((filter_)->action.group.flags & ACTION_ABSOLUTE_SWITCH)          \
        (state_)->components.component_ = (filter_)->action.group.group; \
    अन्यथा                                                                 \
        (state_)->components.component_ += (filter_)->action.group.group

अटल व्योम
xkb_filter_group_set_new(काष्ठा xkb_server_state *state,
                         काष्ठा xkb_events *events,
                         काष्ठा xkb_filter *filter)
{
    static_assert(माप(state->base.components.base_group) ==
                  माप(filter->priv),
                  "Max groups don't fit");
    filter->priv = (uint32_t)state->base.components.base_group;
    apply_group_delta(filter, &state->base, base_group);
}

अटल bool
xkb_filter_group_set_func(काष्ठा xkb_server_state *state,
                          काष्ठा xkb_events *events,
                          काष्ठा xkb_filter *filter,
                          स्थिर काष्ठा xkb_कुंजी *कुंजी,
                          क्रमागत xkb_कुंजी_direction direction)
{
    अगर (कुंजी != filter->कुंजी) {
        filter->action.group.flags &= ~ACTION_LOCK_CLEAR;
        वापस XKB_FILTER_CONTINUE;
    }

    चयन (direction) {
    हाल XKB_कुंजी_DOWN:
        filter->refcnt++;
        /* fallthrough */
    हाल XKB_कुंजी_REPEATED:
        वापस XKB_FILTER_CONSUME;
    शेष:
        अगर (--filter->refcnt > 0)
            वापस XKB_FILTER_CONSUME;
    }

    static_assert(माप(state->base.components.base_group) == माप(filter->priv),
                  "Max groups don't fit");
    state->base.components.base_group = (int32_t) filter->priv;

    अगर (filter->action.group.flags & ACTION_LOCK_CLEAR)
        state->base.components.locked_group = 0;

    filter->func = शून्य;
    वापस XKB_FILTER_CONTINUE;
}

अटल क्रमागत xkb_state_component
get_state_component_changes(स्थिर काष्ठा state_components *a,
                            स्थिर काष्ठा state_components *b)
{
    क्रमागत xkb_state_component mask = 0;

    अगर (a->group != b->group)
        mask |= XKB_STATE_LAYOUT_EFFECTIVE;
    अगर (a->base_group != b->base_group)
        mask |= XKB_STATE_LAYOUT_DEPRESSED;
    अगर (a->latched_group != b->latched_group)
        mask |= XKB_STATE_LAYOUT_LATCHED;
    अगर (a->locked_group != b->locked_group)
        mask |= XKB_STATE_LAYOUT_LOCKED;
    अगर (a->mods != b->mods)
        mask |= XKB_STATE_MODS_EFFECTIVE;
    अगर (a->base_mods != b->base_mods)
        mask |= XKB_STATE_MODS_DEPRESSED;
    अगर (a->latched_mods != b->latched_mods)
        mask |= XKB_STATE_MODS_LATCHED;
    अगर (a->locked_mods != b->locked_mods)
        mask |= XKB_STATE_MODS_LOCKED;
    अगर (a->leds != b->leds)
        mask |= XKB_STATE_LEDS;
    अगर (a->controls != b->controls)
        mask |= XKB_STATE_CONTROLS_EFFECTIVE;

    वापस mask;
}

अटल व्योम
xkb_filter_group_lock_new(काष्ठा xkb_server_state *state,
                          काष्ठा xkb_events *events,
                          काष्ठा xkb_filter *filter)
{
    अगर (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release: do nothing on key press.
         *
         * This is a keymap format v2 extension.
         */
        वापस;
    } अन्यथा {
        /* Lock on key press */
        apply_group_delta(filter, &state->base, locked_group);
    }
}

अटल bool
xkb_filter_group_lock_func(काष्ठा xkb_server_state *state,
                           काष्ठा xkb_events *events,
                           काष्ठा xkb_filter *filter,
                           स्थिर काष्ठा xkb_कुंजी *कुंजी,
                           क्रमागत xkb_कुंजी_direction direction)
{
    अगर (कुंजी != filter->कुंजी) {
        अगर ((filter->action.group.flags & ACTION_LOCK_ON_RELEASE) &&
            direction == XKB_कुंजी_DOWN) {
            /*
             * Another key has been pressed after the locking key:
             * cancel group lock on release.
             *
             * This is a keymap v2 extension.
             */
            filter->action.group.flags &= ~ACTION_LOCK_ON_RELEASE;
        }
        वापस XKB_FILTER_CONTINUE;
    }

    चयन (direction) {
    हाल XKB_कुंजी_DOWN:
        filter->refcnt++;
        /* fallthrough */
    हाल XKB_कुंजी_REPEATED:
        वापस XKB_FILTER_CONSUME;
    शेष:
        अगर (--filter->refcnt > 0)
            वापस XKB_FILTER_CONSUME;
    }

    अगर (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release
         *
         * This is a keymap v2 extension.
         */
        apply_group_delta(filter, &state->base, locked_group);
    } अन्यथा {
        /* Lock on key press: do nothing on key release. */
    }
    filter->func = शून्य;
    वापस XKB_FILTER_CONTINUE;
}

अटल bool
xkb_action_breaks_latch(स्थिर जोड़ xkb_action *action,
                        क्रमागत xkb_action_controls controls,
                        क्रमागत xkb_internal_action_flags flag,
                        xkb_mod_mask_t mask)
{
    चयन (action->type) {
    हाल ACTION_TYPE_NONE:
    हाल ACTION_TYPE_VOID:
    हाल ACTION_TYPE_PTR_BUTTON:
    हाल ACTION_TYPE_PTR_LOCK:
    हाल ACTION_TYPE_CTRL_SET:
    हाल ACTION_TYPE_CTRL_LOCK:
    हाल ACTION_TYPE_SWITCH_VT:
    हाल ACTION_TYPE_TERMINATE:
    हाल ACTION_TYPE_REDIRECT_कुंजी:
        वापस true;
    हाल ACTION_TYPE_PTR_MOVE:
    हाल ACTION_TYPE_PTR_DEFAULT:
        वापस !(controls & CONTROL_MOUSE_कुंजियाँ);
    हाल ACTION_TYPE_INTERNAL:
        वापस (action->internal.flags & flag) &&
               ((action->internal.clear_latched_mods & mask) == mask);
    शेष:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_अंक_ENTRIES,
                      "Missing action type");
        वापस false;
    }
}

क्रमागत xkb_कुंजी_latch_state {
    NO_LATCH = 0,
    LATCH_कुंजी_DOWN,
    LATCH_PENDING,
    कुंजी_LATCH_STATE_अंक_ENTRIES
};

#घोषणा MAX_XKB_कुंजी_LATCH_STATE_LOG2 2
#अगर (कुंजी_LATCH_STATE_अंक_ENTRIES > (1 << MAX_XKB_कुंजी_LATCH_STATE_LOG2)) || \
    (-XKB_MAX_GROUPS) < (INT32_MIN >> MAX_XKB_कुंजी_LATCH_STATE_LOG2) || \
      XKB_MAX_GROUPS > (INT32_MAX >> MAX_XKB_कुंजी_LATCH_STATE_LOG2)
#त्रुटि "Cannot represent priv field of the group latch filter"
#पूर्ण_अगर

/* Hold the latch state *and* the group delta */
जोड़ group_latch_priv {
    uint32_t priv;
    काष्ठा {
        /* The type is really: enum xkb_key_latch_state, but it is problematic
         * on Windows, because it is interpreted as signed and leads to wrong
         * negative values. */
        अचिन्हित पूर्णांक latch:MAX_XKB_कुंजी_LATCH_STATE_LOG2;
        int32_t group_delta:(32 - MAX_XKB_कुंजी_LATCH_STATE_LOG2);
    };
};

अटल व्योम
xkb_filter_group_latch_new(काष्ठा xkb_server_state *state,
                           काष्ठा xkb_events *events,
                           काष्ठा xkb_filter *filter)
{
    स्थिर जोड़ group_latch_priv priv = {
        .latch = LATCH_कुंजी_DOWN,
        .group_delta = (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)
            ? filter->action.group.group - state->base.components.base_group
            : filter->action.group.group
    };
    filter->priv = priv.priv;
    /* Like group set */
    apply_group_delta(filter, &state->base, base_group);
}

अटल bool
xkb_filter_group_latch_func(काष्ठा xkb_server_state *state,
                            काष्ठा xkb_events *events,
                            काष्ठा xkb_filter *filter,
                            स्थिर काष्ठा xkb_कुंजी *कुंजी,
                            क्रमागत xkb_कुंजी_direction direction)
{
    जोड़ group_latch_priv priv = {.priv = filter->priv};
    क्रमागत xkb_कुंजी_latch_state latch = priv.latch;

    अगर (direction == XKB_कुंजी_DOWN) {
        स्थिर जोड़ xkb_action *actions = शून्य;
        स्थिर xkb_action_गिनती_t गिनती = xkb_कुंजी_get_actions(&state->base, कुंजी,
                                                             &actions);

        अगर (latch == LATCH_कुंजी_DOWN) {
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
            अगर (state->a11y_flags & XKB_A11_Y_LATCH_SIMULTANEOUS_कुंजियाँ) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                क्रम (xkb_action_गिनती_t k = 0; k < गिनती; k++) {
                    अगर (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_GROUP_LATCH,
                                                0))
                    {
                        latch = NO_LATCH;
                        अवरोध;
                    }
                }
            } अन्यथा {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        अन्यथा अगर (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is
             * pressed, or promote it to a lock if it's the same group delta &
             * flags and latchToLock option is enabled. */
            स्थिर bool sticky_कुंजियाँ = state->base.components.controls
                                   & CONTROL_STICKY_कुंजियाँ;
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
            क्रम (xkb_action_गिनती_t k = 0; k < गिनती; k++) {
                अगर (((actions[k].type == ACTION_TYPE_GROUP_LATCH) ||
                     (actions[k].type == ACTION_TYPE_GROUP_SET && sticky_कुंजियाँ)) &&
                     actions[k].group.group == filter->action.group.group &&
                     (actions[k].group.flags & ACTION_ABSOLUTE_SWITCH) ==
                     (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)) {
                    अगर (filter->action.group.flags & ACTION_LATCH_TO_LOCK &&
                        filter->action.group.group != 0)
                    {
                        /* Promote to lock */
                        filter->action.type = ACTION_TYPE_GROUP_LOCK;
                        filter->func = xkb_filter_group_lock_func;
                        xkb_filter_group_lock_new(state, events, filter);
                        state->base.components.latched_group -= priv.group_delta;
                        filter->कुंजी = कुंजी;
                        /* XXX beep beep! */
                        वापस XKB_FILTER_CONSUME;
                    }
                    /* Do nothing if `latchToLock` option is not activated; if
                     * the latch is not broken by the following actions and the
                     * key is not consumed, then another latch filter will be
                     * created.
                     */
                    जारी;
                }
                अन्यथा अगर (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_GROUP_LATCH,
                                                 0))
                {
                    /* Breaks the latch */
                    state->base.components.latched_group -= priv.group_delta;
                    filter->func = शून्य;
                    वापस XKB_FILTER_CONTINUE;
                }
            }
        }
        अन्यथा {
            /* Ignore press in NO_LATCH state */
            निश्चित(latch == NO_LATCH);
        }
    }
    अन्यथा अगर (कुंजी != filter->कुंजी) {
        /* Ignore repeat/release of other keys */
    }
    अन्यथा अगर (direction == XKB_कुंजी_REPEATED) {
        वापस XKB_FILTER_CONSUME; // TODO: check
    }
    अन्यथा {
        /* Our key got released.  If we've set it to clear locks, and we
         * currently have a group locked, then release it and
         * don't actually latch.  Else we've actually hit the latching
         * stage, so set PENDING and move our group from base to
         * latched. */
        अगर ((filter->action.group.flags & ACTION_LOCK_CLEAR) &&
             state->base.components.locked_group) {
            अगर (latch == LATCH_PENDING)
                state->base.components.latched_group -= priv.group_delta;
            अन्यथा
                state->base.components.base_group -= priv.group_delta;
            state->base.components.locked_group = 0;
            filter->func = शून्य;
        }
        /* Broken latch */
        अन्यथा अगर (latch == NO_LATCH) {
            state->base.components.base_group -= priv.group_delta;
            filter->func = शून्य;
        }
        /* We may already have reached the latch state if pressing the
         * key multiple times without latch-to-lock enabled. */
        अन्यथा अगर (latch == LATCH_कुंजी_DOWN) {
            latch = LATCH_PENDING;
            /* Switch from set to latch */
            state->base.components.base_group -= priv.group_delta;
            state->base.components.latched_group += priv.group_delta;
            /* XXX beep beep! */
        }
    }

    priv.latch = latch;
    filter->priv = priv.priv;

    वापस XKB_FILTER_CONTINUE;
}

अटल व्योम
xkb_filter_mod_set_new(काष्ठा xkb_server_state *state,
                       काष्ठा xkb_events *events,
                       काष्ठा xkb_filter *filter)
{
    स्थिर क्रमागत xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    अगर ((filter->action.mods.flags & unlock) == unlock) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension.
         */
        filter->priv = filter->action.mods.mods.mask
                     & ~state->base.components.locked_mods;
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
    } अन्यथा {
        filter->priv = filter->action.mods.mods.mask;
    }

    state->set_mods |= (xkb_mod_mask_t) filter->priv;
}

अटल bool
xkb_filter_mod_set_func(काष्ठा xkb_server_state *state,
                        काष्ठा xkb_events *events,
                        काष्ठा xkb_filter *filter,
                        स्थिर काष्ठा xkb_कुंजी *कुंजी,
                        क्रमागत xkb_कुंजी_direction direction)
{

    अगर (कुंजी != filter->कुंजी) {
        filter->action.mods.flags &= ~ACTION_LOCK_CLEAR;
        वापस XKB_FILTER_CONTINUE;
    }

    चयन (direction) {
    हाल XKB_कुंजी_DOWN:
        filter->refcnt++;
        /* fallthrough */
    हाल XKB_कुंजी_REPEATED:
        वापस XKB_FILTER_CONSUME;
    शेष:
        अगर (--filter->refcnt > 0)
            वापस XKB_FILTER_CONSUME;
    }

    state->clear_mods |= (xkb_mod_mask_t) filter->priv;
    स्थिर क्रमागत xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    अगर ((filter->action.mods.flags & unlock) == ACTION_LOCK_CLEAR)
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;

    filter->func = शून्य;
    वापस XKB_FILTER_CONTINUE;
}

अटल व्योम
xkb_filter_mod_lock_new(काष्ठा xkb_server_state *state,
                        काष्ठा xkb_events *events,
                        काष्ठा xkb_filter *filter)
{
    filter->priv = (state->base.components.locked_mods &
                    filter->action.mods.mods.mask);

    अगर (filter->priv && (filter->action.mods.flags & ACTION_UNLOCK_ON_PRESS)) {
        /*
         * Some of the target modifiers were locked before key press: unlock.
         *
         * This is a keymap v2 extension: unlock-on-press.
         */
        अगर (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
            state->base.components.locked_mods &= ~filter->priv;
        /* No further action: cancel filter */
        filter->func = शून्य;
    } अन्यथा {
        /* Set base mods; lock mods if relevant (XKB 1.0 spec) */
        state->set_mods |= filter->action.mods.mods.mask;
        अगर (!(filter->action.mods.flags & ACTION_LOCK_NO_LOCK))
            state->base.components.locked_mods |= filter->action.mods.mods.mask;
    }
}

अटल bool
xkb_filter_mod_lock_func(काष्ठा xkb_server_state *state,
                         काष्ठा xkb_events *events,
                         काष्ठा xkb_filter *filter,
                         स्थिर काष्ठा xkb_कुंजी *कुंजी,
                         क्रमागत xkb_कुंजी_direction direction)
{
    अगर (कुंजी != filter->कुंजी)
        वापस XKB_FILTER_CONTINUE;

    चयन (direction) {
    हाल XKB_कुंजी_DOWN:
        filter->refcnt++;
        /* fallthrough */
    हाल XKB_कुंजी_REPEATED:
        वापस XKB_FILTER_CONSUME;
    शेष:
        अगर (--filter->refcnt > 0)
            वापस XKB_FILTER_CONSUME;
    }

    state->clear_mods |= filter->action.mods.mods.mask;
    अगर (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
        state->base.components.locked_mods &= ~filter->priv;

    filter->func = शून्य;
    वापस XKB_FILTER_CONTINUE;
}

अटल व्योम
xkb_filter_mod_latch_new(काष्ठा xkb_server_state *state,
                         काष्ठा xkb_events *events,
                         काष्ठा xkb_filter *filter)
{
    /* Latch-on-press + clear-locks imply unlock-on-press */
    स्थिर क्रमागत xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                              | ACTION_LATCH_ON_PRESS;

    अगर ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
        (filter->action.mods.flags & unlockOnPress) &&
        (state->base.components.locked_mods & filter->action.mods.mods.mask) ==
         filter->action.mods.mods.mask) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension: clear locks and do not latch.
         */
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
        filter->func = शून्य;
    } अन्यथा अगर (filter->action.mods.flags & ACTION_LATCH_ON_PRESS) {
        /*
         * Latch on key press
         *
         * This is a keymap format v2 extension.
         */
        filter->priv = LATCH_PENDING;
        state->base.components.latched_mods |= filter->action.mods.mods.mask;
        /* XXX beep beep! */
    } अन्यथा {
        /* XKB standard latch action */
        filter->priv = LATCH_कुंजी_DOWN;
        state->set_mods |= filter->action.mods.mods.mask;
    }
}

अटल bool
xkb_filter_mod_latch_func(काष्ठा xkb_server_state *state,
                          काष्ठा xkb_events *events,
                          काष्ठा xkb_filter *filter,
                          स्थिर काष्ठा xkb_कुंजी *कुंजी,
                          क्रमागत xkb_कुंजी_direction direction)
{
    क्रमागत xkb_कुंजी_latch_state latch = filter->priv;

    अगर (direction == XKB_कुंजी_DOWN) {
        स्थिर जोड़ xkb_action *actions = शून्य;
        स्थिर xkb_action_गिनती_t गिनती = xkb_कुंजी_get_actions(&state->base, कुंजी,
                                                             &actions);

        अगर (latch == LATCH_कुंजी_DOWN) {
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
            अगर (state->a11y_flags & XKB_A11_Y_LATCH_SIMULTANEOUS_कुंजियाँ) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                क्रम (xkb_action_गिनती_t k = 0; k < गिनती; k++) {
                    अगर (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_MOD_LATCH,
                                                filter->action.mods.mods.mask))
                    {
                        latch = NO_LATCH;
                        अवरोध;
                    }
                }
            } अन्यथा {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        अन्यथा अगर (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is pressed,
             * or promote it to a lock or plain base set if it's the same
             * modifier. */
            स्थिर bool sticky_कुंजियाँ = state->base.components.controls
                                   & CONTROL_STICKY_कुंजियाँ;
            /*
             * NOTE: Action’s flags are not compared:
             * - flags may have been modified by keyboard controls at the
             *   creation of the filter;
             * - it is unlikely to have two LatchMods() actions on the same
             *   key that differ only by their flags;
             * - even if it were the case, one would most likely want both
             *   actions to be considered compatible in this context.
             */
            क्रम (xkb_action_गिनती_t k = 0; k < गिनती; k++) {
                अगर (((actions[k].type == ACTION_TYPE_MOD_LATCH) ||
                     (actions[k].type == ACTION_TYPE_MOD_SET && sticky_कुंजियाँ)) &&
                    actions[k].mods.mods.mask == filter->action.mods.mods.mask)
                {
                    अगर (filter->action.mods.flags & ACTION_LATCH_TO_LOCK) {
                        /* Mutate the action to LockMods() */
                        filter->action.type = ACTION_TYPE_MOD_LOCK;
                        filter->func = xkb_filter_mod_lock_func;
                        xkb_filter_mod_lock_new(state, events, filter);
                    }
                    अन्यथा {
                        /* Mutate the action to SetMods() */
                        filter->action.type = ACTION_TYPE_MOD_SET;
                        filter->func = xkb_filter_mod_set_func;
                        xkb_filter_mod_set_new(state, events, filter);
                    }
                    filter->कुंजी = कुंजी;
                    /* Clear latches */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    /* XXX beep beep! */
                    वापस XKB_FILTER_CONSUME;
                }
                अन्यथा अगर (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_MOD_LATCH,
                                                 filter->action.mods.mods.mask))
                {
                    /* XXX: This may be totally broken, we might need to break the
                     *      latch in the next run after this press? */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    filter->func = शून्य;
                    वापस XKB_FILTER_CONTINUE;
                }
            }
        }
        अन्यथा {
            /* Ignore press in NO_LATCH state */
            निश्चित(latch == NO_LATCH);
        }
    }
    अन्यथा अगर (कुंजी != filter->कुंजी) {
        /* Ignore repeat/release of other keys */
    }
    अन्यथा अगर (direction == XKB_कुंजी_REPEATED) {
        वापस XKB_FILTER_CONSUME; // TODO: check
    }
    अन्यथा {
        /* Our key got released.  If we’ve set it to clear locks, and we
         * currently have the same modifiers locked, then release them and
         * don't actually latch.  Else we’ve actually hit the latching
         * stage, so set PENDING and move our modifier from base to
         * latched. */

        /* Latch-on-press + clear-locks imply unlock-on-press */
        स्थिर क्रमागत xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                                  | ACTION_LATCH_ON_PRESS;

        अगर ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
            !(filter->action.mods.flags & unlockOnPress) &&
            (state->base.components.locked_mods &
             filter->action.mods.mods.mask) == filter->action.mods.mods.mask)
        {
            /* XXX: We might be a bit overenthusiastic about clearing
             *      mods other filters have set here? */
            अगर (latch == LATCH_PENDING)
                state->base.components.latched_mods &=
                    ~filter->action.mods.mods.mask;
            अन्यथा
                state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
            filter->func = शून्य;
        }
        अन्यथा अगर (latch == NO_LATCH) {
            /* Broken latch */
            state->clear_mods |= filter->action.mods.mods.mask;
            filter->func = शून्य;
        }
        अन्यथा अगर (!(filter->action.mods.flags & ACTION_LATCH_ON_PRESS)) {
            latch = LATCH_PENDING;
            state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.latched_mods |= filter->action.mods.mods.mask;
            /* XXX beep beep! */
        }
    }

    filter->priv = latch;

    वापस XKB_FILTER_CONTINUE;
}

अटल व्योम
append_pointer_move(काष्ठा xkb_events *events,
                    काष्ठा xkb_filter *filter)
{
    क्रमागत xkb_pointer_motion_flags flags = 0;
    अगर (filter->action.ptr.flags & ACTION_ABSOLUTE_X)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_X;
    अगर (filter->action.ptr.flags & ACTION_ABSOLUTE_Y)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_Y;
    अगर (filter->action.ptr.flags & ACTION_REPEAT)
        flags |= XKB_POINTER_MOTION_REPEATS;
    darray_append(events->queue, (काष्ठा xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_MOTION,
        .pointer_motion = {
            .आकार = माप(((काष्ठा xkb_event*)0)->pointer_motion),
            .flags = flags,
            .x = filter->action.ptr.x,
            .y = filter->action.ptr.y,
        }
    });
}

अटल व्योम
xkb_filter_pointer_move_new(काष्ठा xkb_server_state *state,
                            काष्ठा xkb_events *events,
                            काष्ठा xkb_filter *filter)
{
    अगर (!events) {
        filter->func = शून्य;
        वापस;
    }

    निश्चित(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_कुंजियाँ);
    append_pointer_move(events, filter);
    state->flags &= ~STATE_REQUIRE_कुंजी_EVENT;
}

अटल bool
xkb_filter_pointer_move_func(काष्ठा xkb_server_state *state,
                             काष्ठा xkb_events *events,
                             काष्ठा xkb_filter *filter,
                             स्थिर काष्ठा xkb_कुंजी *कुंजी,
                             क्रमागत xkb_कुंजी_direction direction)
{
    अगर (कुंजी != filter->कुंजी || !events)
        वापस XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_कुंजी_EVENT;

    चयन (direction) {
    हाल XKB_कुंजी_DOWN:
        filter->refcnt++;
        append_pointer_move(events, filter);
        वापस XKB_FILTER_CONSUME;
    हाल XKB_कुंजी_REPEATED:
        अगर (filter->action.ptr.flags & ACTION_REPEAT)
            append_pointer_move(events, filter);
        वापस XKB_FILTER_CONSUME;
    शेष:
        अगर (--filter->refcnt > 0)
            वापस XKB_FILTER_CONSUME;
    }

    filter->func = शून्य;
    वापस XKB_FILTER_CONSUME;
}

/* Implemented with xkb_machine API because it requires struct xkb_machine */
अटल व्योम
xkb_filter_pointer_button_new(काष्ठा xkb_server_state *state,
                              काष्ठा xkb_events *events,
                              काष्ठा xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
अटल bool
xkb_filter_pointer_button_func(काष्ठा xkb_server_state *state,
                               काष्ठा xkb_events *events,
                               काष्ठा xkb_filter *filter,
                               स्थिर काष्ठा xkb_कुंजी *कुंजी,
                               क्रमागत xkb_कुंजी_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
अटल व्योम
xkb_filter_pointer_lock_button_new(काष्ठा xkb_server_state *state,
                                   काष्ठा xkb_events *events,
                                   काष्ठा xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
अटल bool
xkb_filter_pointer_lock_button_func(काष्ठा xkb_server_state *state,
                                    काष्ठा xkb_events *events,
                                    काष्ठा xkb_filter *filter,
                                    स्थिर काष्ठा xkb_कुंजी *कुंजी,
                                    क्रमागत xkb_कुंजी_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
अटल व्योम
xkb_filter_pointer_set_default_button_new(काष्ठा xkb_server_state *state,
                                          काष्ठा xkb_events *events,
                                          काष्ठा xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
अटल bool
xkb_filter_pointer_set_default_button_func(काष्ठा xkb_server_state *state,
                                           काष्ठा xkb_events *events,
                                           काष्ठा xkb_filter *filter,
                                           स्थिर काष्ठा xkb_कुंजी *कुंजी,
                                           क्रमागत xkb_कुंजी_direction direction);

अटल व्योम
xkb_filter_terminate_server_new(काष्ठा xkb_server_state *state,
                                 काष्ठा xkb_events *events,
                                 काष्ठा xkb_filter *filter);

अटल bool
xkb_filter_terminate_server_func(काष्ठा xkb_server_state *state,
                                  काष्ठा xkb_events *events,
                                  काष्ठा xkb_filter *filter,
                                  स्थिर काष्ठा xkb_कुंजी *कुंजी,
                                  क्रमागत xkb_कुंजी_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
अटल व्योम
xkb_filter_switch_virtual_console_new(काष्ठा xkb_server_state *state,
                                      काष्ठा xkb_events *events,
                                      काष्ठा xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
अटल bool
xkb_filter_switch_virtual_console_func(काष्ठा xkb_server_state *state,
                                       काष्ठा xkb_events *events,
                                       काष्ठा xkb_filter *filter,
                                       स्थिर काष्ठा xkb_कुंजी *कुंजी,
                                       क्रमागत xkb_कुंजी_direction direction);

अटल अंतरभूत व्योम
clear_all_latches_and_locks(काष्ठा xkb_server_state *state,
                            काष्ठा xkb_events *events);

अटल व्योम
xkb_filter_ctrls_new(काष्ठा xkb_server_state *state,
                     काष्ठा xkb_events *events,
                     काष्ठा xkb_filter *filter)
{
    अगर (filter->action.type == ACTION_TYPE_CTRL_SET) {
        /* Set: save the specified controls that are *not* already enabled */
        filter->priv = (uint32_t) (
            (~state->base.components.controls) & filter->action.ctrls.ctrls
        );
    } अन्यथा {
        /* Lock: save the specified controls that *are* already enabled */
        filter->priv = (uint32_t) (
            state->base.components.controls & filter->action.ctrls.ctrls
        );
    }

    अगर (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_LOCK)) {
        /* Enable the specified controls that are not already enabled */
        state->base.components.controls |= filter->action.ctrls.ctrls;
    }
}

अटल bool
xkb_filter_ctrls_func(काष्ठा xkb_server_state *state,
                      काष्ठा xkb_events *events,
                      काष्ठा xkb_filter *filter,
                      स्थिर काष्ठा xkb_कुंजी *कुंजी,
                      क्रमागत xkb_कुंजी_direction direction)
{
    अगर (कुंजी != filter->कुंजी)
        वापस XKB_FILTER_CONTINUE;

    चयन (direction) {
    हाल XKB_कुंजी_DOWN:
        filter->refcnt++;
        /* fallthrough */
    हाल XKB_कुंजी_REPEATED:
        वापस XKB_FILTER_CONSUME;
    शेष:
        अगर (--filter->refcnt > 0)
            वापस XKB_FILTER_CONSUME;
    }

    अगर (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_UNLOCK)) {
        स्थिर क्रमागत xkb_action_controls old = state->base.components.controls;

        /*
         * Set: Disable specified controls that were *not* enabled at key press.
         * Lock: Disable specified controls that *were* enabled at key press.
         */
        state->base.components.controls &= ~(क्रमागत xkb_action_controls) filter->priv;

        अगर ((old & CONTROL_STICKY_कुंजियाँ) &&
            !(state->base.components.controls & CONTROL_STICKY_कुंजियाँ)) {
            /* Sticky keys were disabled: clear all locks and latches */
            clear_all_latches_and_locks(state, events);
        }
    }

    filter->func = शून्य;
    वापस XKB_FILTER_CONTINUE;
}

अटल bool
append_redirect_कुंजी_events(काष्ठा xkb_server_state *state,
                           काष्ठा xkb_events *events,
                           स्थिर काष्ठा xkb_redirect_कुंजी_action *redirect,
                           क्रमागत xkb_कुंजी_direction direction)
{
    क्रमागत xkb_state_component changed = 0;
    स्थिर xkb_mod_mask_t mask = redirect->affect_mods;

    /*
     * Reference state: find the last state update in the queue, otherwise
     * use the current state.
     */
    काष्ठा xkb_event *event;
    काष्ठा state_components last_components = state->base.components;
    darray_foreach_reverse(event, events->queue) {
        अगर (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
            last_components = event->components.components;
            अवरोध;
        }
    }

    अगर (mask) {
        काष्ठा state_components new = last_components;
        new.base_mods = (new.base_mods & ~mask) | redirect->mods;
        new.latched_mods = (new.latched_mods & ~mask) | redirect->mods;
        new.locked_mods = (new.locked_mods & ~mask) | redirect->mods;
        new.mods = (new.mods & ~mask) | redirect->mods;

        changed = get_state_component_changes(&last_components, &new);
        अगर (changed) {
            darray_append(events->queue, (काष्ठा xkb_event) {
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
            darray_append(events->queue, (काष्ठा xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_FRAME,
            });
        }
    }

    darray_append(events->queue, (काष्ठा xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_कुंजी,
        .कुंजी = {
            .keycode = redirect->keycode,
            .direction = direction
        }
    });

    अगर (mask && changed) {
        /* Restore state */
        darray_append(events->queue, (काष्ठा xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = last_components,
                .changed = changed
            }
        });
    }

    state->flags &= ~STATE_REQUIRE_कुंजी_EVENT;
    वापस true;
}

अटल व्योम
xkb_filter_redirect_कुंजी_new(काष्ठा xkb_server_state *state,
                            काष्ठा xkb_events *events,
                            काष्ठा xkb_filter *filter)
{
    /* Action effectual only with the xkb_machine API and a valid keycode */
    अगर (!events || filter->action.redirect.keycode == XKB_KEYCODE_INVALID) {
        filter->func = शून्य;
        वापस;
    }
    append_redirect_कुंजी_events(state, events, &filter->action.redirect,
                               XKB_कुंजी_DOWN);
}

अटल bool
xkb_filter_redirect_कुंजी_func(काष्ठा xkb_server_state *state,
                             काष्ठा xkb_events *events,
                             काष्ठा xkb_filter *filter,
                             स्थिर काष्ठा xkb_कुंजी *कुंजी,
                             क्रमागत xkb_कुंजी_direction direction)
{
    अगर (कुंजी != filter->कुंजी)
        वापस XKB_FILTER_CONTINUE;

    अगर (direction == XKB_कुंजी_UP) {
        append_redirect_कुंजी_events(state, events,
                                   &filter->action.redirect, XKB_कुंजी_UP);
        filter->func = शून्य;
        वापस XKB_FILTER_CONSUME;
     } अन्यथा अगर (direction == XKB_कुंजी_DOWN) {
        स्थिर जोड़ xkb_action *actions = शून्य;
        स्थिर xkb_action_गिनती_t गिनती =
            xkb_कुंजी_get_actions(&state->base, कुंजी, &actions);

        क्रम (xkb_action_गिनती_t a = 0; a < गिनती; a++) {
            अगर (actions[a].type == ACTION_TYPE_REDIRECT_कुंजी &&
                actions[a].redirect.keycode != filter->action.redirect.keycode) {
                    /* One action redirects to a different key: release first */
                    append_redirect_कुंजी_events(state, events,
                                               &filter->action.redirect,
                                               XKB_कुंजी_UP);
                filter->func = शून्य;
                वापस XKB_FILTER_CONTINUE;
            }
        }
    }

    append_redirect_कुंजी_events(state, events, &filter->action.redirect,
                               direction);
    वापस XKB_FILTER_CONSUME;
}

अटल स्थिर काष्ठा {
    व्योम (*new)(काष्ठा xkb_server_state *state,
                काष्ठा xkb_events *events,
                काष्ठा xkb_filter *filter);
    bool (*func)(काष्ठा xkb_server_state *state,
                 काष्ठा xkb_events *events,
                 काष्ठा xkb_filter *filter,
                 स्थिर काष्ठा xkb_कुंजी *कुंजी, क्रमागत xkb_कुंजी_direction direction);
} filter_action_funcs[ACTION_TYPE_अंक_ENTRIES] = {
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
    [ACTION_TYPE_REDIRECT_कुंजी] = { xkb_filter_redirect_कुंजी_new,
                                   xkb_filter_redirect_कुंजी_func },
};

/**
 * Applies any relevant filters to the key, first from the list of filters
 * that are currently active, then if no filter has claimed the key, possibly
 * apply a new filter from the key action.
 */
अटल व्योम
xkb_filter_apply_all(काष्ठा xkb_server_state *state,
                     काष्ठा xkb_events *events,
                     स्थिर काष्ठा xkb_कुंजी *कुंजी,
                     क्रमागत xkb_कुंजी_direction direction)
{
    /* First run through all the currently active filters and see if any of
     * them have consumed this event. */
    bool consumed = false;
    काष्ठा xkb_filter *filter;
    darray_foreach(filter, state->filters) {
        अगर (!filter->func)
            जारी;

        अगर (filter->func(state, events, filter, कुंजी, direction) == XKB_FILTER_CONSUME)
            consumed = true;
    }
    अगर (consumed || direction == XKB_कुंजी_UP)
        वापस;

    /* No filter consumed this event, so proceed with the key actions */
    स्थिर जोड़ xkb_action *actions = शून्य;
    xkb_action_गिनती_t गिनती = xkb_कुंजी_get_actions(&state->base, कुंजी,
                                                         &actions);

    क्रम (काष्ठा {
            xkb_action_गिनती_t गिनती;
            स्थिर जोड़ xkb_action *actions;
         } अग्र = {0};
         गिनती;
         गिनती = अग्र.गिनती, अग्र.गिनती = 0, actions = अग्र.actions
    ) {
        /*
         * Process actions sequentially.
         *
         * NOTE: We rely on the parser to disallow multiple modifier or group
         * actions (see `CheckMultipleActionsCategories`). Allowing multiple
         * such actions requires a refactor of the state handling.
         */
        क्रम (xkb_action_गिनती_t k = 0; k < गिनती; k++) {
            /*
             * It's possible for the keymap to set action->type explicitly, like
             * so:
             *     interpret XF86_Next_VMode {
             *         action = Private(type=0x86, data="+VMode");
             *     };
             * We don't handle those.
             */
            अगर (actions[k].type >= ACTION_TYPE_अंक_ENTRIES)
                जारी;

            /* Go to next action if no corresponding action handler */
            अगर (!filter_action_funcs[actions[k].type].new)
                जारी;

            /* Add a new filter and run the corresponding initial action */
            filter = xkb_filter_new(state);
            filter->कुंजी = कुंजी;
            filter->action = actions[k];

            चयन (filter->action.type) {
            हाल ACTION_TYPE_MOD_SET:
                अगर (state->base.components.controls & CONTROL_STICKY_कुंजियाँ) {
                    /* Convert modifier set action to a latch */
                    filter->action.type = ACTION_TYPE_MOD_LATCH;
                    अगर (state->a11y_flags & XKB_A11_Y_STICKY_कुंजियाँ_LATCH_TO_LOCK) {
                        filter->action.mods.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                /* fallthrough */
            हाल ACTION_TYPE_MOD_LATCH:
            हाल ACTION_TYPE_MOD_LOCK: {
                अटल स्थिर क्रमागत xkb_action_flags unlock_mods_on_press
                    = ACTION_LOCK_CLEAR     /* SetMods & LatchMods: can unlock */
                    | ACTION_LOCK_NO_UNLOCK /* LockMods: cannot unlock */
                    | ACTION_EXPLICIT_UNLOCK_ON_PRESS;
                अगर ((state->flags & STATE_UNLOCK_MODS_ON_PRESS) &&
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
                अवरोध;
            }
            हाल ACTION_TYPE_GROUP_SET:
                अगर (state->base.components.controls & CONTROL_STICKY_कुंजियाँ) {
                    /* Convert group set action to a latch */
                    filter->action.type = ACTION_TYPE_GROUP_LATCH;
                    अगर (state->a11y_flags & XKB_A11_Y_STICKY_कुंजियाँ_LATCH_TO_LOCK) {
                        filter->action.group.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                अवरोध;
            हाल ACTION_TYPE_GROUP_LOCK:
                अगर ((filter->action.group.flags &
                     ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE) &&
                    (state->flags &
                     STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_कुंजी) &&
                    /*
                     * FIXME: sticky keys may leave an unexpected modifier
                     * from the level 0 latched, so disable for now.
                     */
                    !(state->base.components.controls & CONTROL_STICKY_कुंजियाँ))
                {
                    filter->action.group.flags |= ACTION_LOCK_ON_RELEASE;
                    /* Add base level action */
                    अग्र.गिनती = xkb_keymap_कुंजी_get_actions_by_level(
                        state->base.keymap, कुंजी, state->base.components.group,
                        0, &अग्र.actions
                    );
                    निश्चित(अग्र.गिनती == 1);
                }
                अवरोध;
            हाल ACTION_TYPE_PTR_MOVE:
            हाल ACTION_TYPE_PTR_BUTTON:
            हाल ACTION_TYPE_PTR_LOCK:
            हाल ACTION_TYPE_PTR_DEFAULT:
                अगर (!(state->base.components.controls & CONTROL_MOUSE_कुंजियाँ)) {
                    /* Convert pointer actions to NoAction */
                    निश्चित(!filter_action_funcs[ACTION_TYPE_NONE].new);
                    filter->func = शून्य;
                    जारी;
                }
                अवरोध;
            हाल ACTION_TYPE_REDIRECT_कुंजी:
                // FIXME: this is not efficient to resolve mods here each time
                filter->action.redirect.affect_mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.affect_mods
                );
                filter->action.redirect.mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.mods
                );
                अवरोध;
            शेष: ;
            }
            filter->func = filter_action_funcs[filter->action.type].func;
            filter_action_funcs[filter->action.type].new(state, events, filter);
        }
    }
}

अटल व्योम
xkb_state_update_derived(काष्ठा xkb_state *state);

अटल अंतरभूत व्योम
xkb_state_init(काष्ठा xkb_state * restrict state,
               काष्ठा xkb_keymap * restrict keymap,
               क्रमागत xkb_state_mode_internal mode)
{
    state->mode = mode;
    state->refcnt = 1;
    state->keymap = xkb_keymap_ref(keymap);
    state->out_of_range_group.policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

    /* Ensure that derived state is correctly initialized */
    xkb_state_update_derived(state);
}

अटल अंतरभूत व्योम
xkb_client_state_init(काष्ठा xkb_client_state * restrict state,
                      काष्ठा xkb_keymap * restrict keymap,
                      क्रमागत xkb_state_mode_internal mode)
{
    xkb_state_init(&state->base, keymap, mode);
}

अटल अंतरभूत व्योम
xkb_server_state_init(काष्ठा xkb_server_state * restrict state,
                      काष्ठा xkb_keymap * restrict keymap,
                      क्रमागत xkb_state_mode_internal mode,
                      क्रमागत xkb_a11y_flags a11y_affect,
                      क्रमागत xkb_a11y_flags a11y_flags)
{
    xkb_state_init(&state->base, keymap, mode);

    state->a11y_flags = a11y_flags;
    अगर (keymap->format != XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !(a11y_affect & XKB_A11_Y_LATCH_SIMULTANEOUS_कुंजियाँ)) {
             /* Keymap v2+: enable extension to XKB if not manually disabled */
             state->a11y_flags |= XKB_A11_Y_LATCH_SIMULTANEOUS_कुंजियाँ;
    }
}

काष्ठा xkb_state *
xkb_state_new(काष्ठा xkb_keymap *keymap)
{
    काष्ठा xkb_server_state * स्थिर state = सुस्मृति(1, माप(*state));
    अगर (!state) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "Could not allocate a state object.\n");
        वापस शून्य;
    }

    xkb_server_state_init(state, keymap, LEGACY_MIXED_STATE, 0, 0);

    वापस (काष्ठा xkb_state *)state;
}

काष्ठा xkb_state *
xkb_state_new_with_mode(काष्ठा xkb_keymap * restrict keymap,
                        क्रमागत xkb_state_mode mode,
                        क्रमागत xkb_status * restrict status)
{
    चयन (mode) {
    हाल XKB_STATE_MODE_CLIENT:
    हाल XKB_STATE_MODE_SERVER_QUERY: {
        काष्ठा xkb_client_state * स्थिर state = सुस्मृति(1, माप(*state));
        अगर (!state)
            अवरोध;

        static_assert((अचिन्हित)XKB_STATE_MODE_CLIENT ==
                      (अचिन्हित)CLIENT_STATE, "");
        static_assert((अचिन्हित)XKB_STATE_MODE_SERVER_QUERY ==
                      (अचिन्हित)SERVER_COMPANION, "");
        xkb_client_state_init(state, keymap, (क्रमागत xkb_state_mode_internal)mode);

        अगर (status)
            *status = XKB_SUCCESS;

        वापस (काष्ठा xkb_state *)state;
    }
    हाल XKB_STATE_MODE_SERVER: {
        काष्ठा xkb_server_state * स्थिर state = सुस्मृति(1, माप(*state));
        अगर (!state)
            अवरोध;

        xkb_server_state_init(state, keymap, LEGACY_SERVER_STATE, 0, 0);

        अगर (status)
            *status = XKB_SUCCESS;

        वापस (काष्ठा xkb_state *)state;
    }
    शेष:
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_STATE_MODE_,
                     "Unsupported state mode: %u\n", mode);
        अगर (status)
            *status = XKB_ERROR_UNSUPPORTED_STATE_MODE;
        वापस शून्य;
    }

    log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                  "Could not allocate a state object.\n");
    अगर (status)
        *status = XKB_ERROR_ALLOCATION_FAILURE;
    वापस शून्य;
}

काष्ठा xkb_state *
xkb_state_ref(काष्ठा xkb_state *state)
{
    निश्चित(state->refcnt > 0);
    state->refcnt++;
    वापस state;
}

व्योम
xkb_state_reset(काष्ठा xkb_state *state)
{
    स_रखो(&state->components, 0, माप(state->components));

    अगर (state->mode < LEGACY_MIXED_STATE) {
        static_assert(माप(*state) == माप(काष्ठा xkb_client_state),
                      "Not a mere wrapper");
        /* Nothing else to reset */
    } अन्यथा {
        काष्ठा xkb_server_state *state_ = (काष्ठा xkb_server_state *)state;
        स्थिर काष्ठा xkb_server_state पूर्व = *state_;
        स_रखो(state_, 0, माप(*state_));

        state_->base = पूर्व.base;
        state_->a11y_flags = पूर्व.a11y_flags;
        state_->filters = पूर्व.filters;
        darray_आकार(state_->filters) = 0;
    }
}

अटल अंतरभूत व्योम
xkb_state_destroy(काष्ठा xkb_state *state)
{
    xkb_keymap_unref(state->keymap);
    अगर (state->mode > SERVER_COMPANION)
        darray_free(((काष्ठा xkb_server_state *)state)->filters);
}

व्योम
xkb_state_unref(काष्ठा xkb_state *state)
{
    निश्चित(!state || state->refcnt > 0);
    अगर (!state || --state->refcnt > 0)
        वापस;

    xkb_state_destroy(state);

    चयन (state->mode) {
    हाल CLIENT_STATE:
    हाल SERVER_COMPANION:
        मुक्त((काष्ठा xkb_client_state *)state);
        अवरोध;
    हाल LEGACY_MIXED_STATE:
    हाल LEGACY_SERVER_STATE:
        मुक्त((काष्ठा xkb_server_state *)state);
        अवरोध;
    हाल SERVER_STATE: /* handled by xkb_machine_unref() */
    शेष:
        PANIC_UNREACHABLE();
    }
}

काष्ठा xkb_keymap *
xkb_state_get_keymap(काष्ठा xkb_state *state)
{
    वापस state->keymap;
}

/**
 * Update the LED state to match the rest of the xkb_state.
 */
अटल व्योम
xkb_state_led_update_all(काष्ठा xkb_state *state)
{
    xkb_led_सूचकांक_t idx;
    स्थिर काष्ठा xkb_led *led;

    state->components.leds = 0;

    xkb_leds_enumerate(idx, led, state->keymap) {

        अगर (led->which_mods != 0 && led->mods.mask != 0) {
            xkb_mod_mask_t mod_mask = 0;
            अगर (led->which_mods & XKB_STATE_MODS_EFFECTIVE)
                mod_mask |= state->components.mods;
            अगर (led->which_mods & XKB_STATE_MODS_DEPRESSED)
                mod_mask |= state->components.base_mods;
            अगर (led->which_mods & XKB_STATE_MODS_LATCHED)
                mod_mask |= state->components.latched_mods;
            अगर (led->which_mods & XKB_STATE_MODS_LOCKED)
                mod_mask |= state->components.locked_mods;

            अगर (led->mods.mask & mod_mask) {
                state->components.leds |= (UINT32_C(1) << idx);
                जारी;
            }
        }

        अगर (led->which_groups != 0) {
            अगर (likely(led->groups) != 0) {
                xkb_layout_mask_t group_mask = 0;
                /* Effective and locked groups have been brought into range */
                निश्चित(state->components.group < XKB_MAX_GROUPS);
                निश्चित(state->components.locked_group >= 0 &&
                       state->components.locked_group < XKB_MAX_GROUPS);
                /* Effective and locked groups are used as mask */
                अगर (led->which_groups & XKB_STATE_LAYOUT_EFFECTIVE)
                    group_mask |= (UINT32_C(1) << state->components.group);
                अगर (led->which_groups & XKB_STATE_LAYOUT_LOCKED)
                    group_mask |= (UINT32_C(1) << state->components.locked_group);
                /* Base and latched groups only have to be non-zero */
                अगर ((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                    state->components.base_group != 0)
                    group_mask |= led->groups;
                अगर ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                    state->components.latched_group != 0)
                    group_mask |= led->groups;

                अगर (led->groups & group_mask) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    जारी;
                }
            } अन्यथा {
                /* Special case for Base and latched groups */
                अगर (((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                     state->components.base_group == 0) ||
                    ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                     state->components.latched_group == 0)) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    जारी;
                }
            }
        }

        अगर (led->ctrls & state->components.controls) {
            state->components.leds |= (UINT32_C(1) << idx);
            जारी;
        }
    }
}

/**
 * Calculates the derived state (effective mods/group and LEDs) from an
 * up-to-date xkb_state.
 */
अटल व्योम
xkb_state_update_derived(काष्ठा xkb_state *state)
{
    xkb_layout_सूचकांक_t wrapped;

    state->components.mods = (state->components.base_mods |
                              state->components.latched_mods |
                              state->components.locked_mods);

    /* Lock group must be adjusted, but not base nor latched groups */
    wrapped = XkbWrapGroupIntoRange(state->components.locked_group,
                                    state->keymap->अंक_groups,
                                    state->out_of_range_group.policy,
                                    state->out_of_range_group.redirect_group);
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->components.locked_group =
        (int32_t) (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    /* Effective group must be adjusted */
    wrapped = XkbWrapGroupIntoRange(state->components.base_group +
                                    state->components.latched_group +
                                    state->components.locked_group,
                                    state->keymap->अंक_groups,
                                    state->out_of_range_group.policy,
                                    state->out_of_range_group.redirect_group);
    state->components.group =
        (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    xkb_state_led_update_all(state);
}

अटल व्योम
state_update_enabled_controls(काष्ठा xkb_server_state *state,
                              क्रमागत xkb_keyboard_control_flags affect_controls,
                              क्रमागत xkb_keyboard_control_flags controls,
                              काष्ठा xkb_events *events)
{
    स्थिर bool had_sticky_कुंजियाँ = state->base.components.controls
                               & CONTROL_STICKY_कुंजियाँ;

    /*
     * Enable using the public API with the all the Control values, except
     * the internal ones, if any.
     */
    affect_controls =
        ( affect_controls
        & (क्रमागत xkb_keyboard_control_flags)CONTROL_ALL_BOOLEAN );
    state->base.components.controls &=
        (क्रमागत xkb_action_controls)~affect_controls;
    state->base.components.controls |=
        (क्रमागत xkb_action_controls)(controls & affect_controls);

    अगर (had_sticky_कुंजियाँ &&
        !(state->base.components.controls & CONTROL_STICKY_कुंजियाँ)) {
        /* Sticky keys were disabled: clear all locks and latches */
        clear_all_latches_and_locks(state, events);
    }

    xkb_state_update_derived(&state->base);
}

/**
 * Given a particular key event, updates the state structure to reflect the
 * new modifiers.
 */
क्रमागत xkb_state_component
xkb_state_update_कुंजी(काष्ठा xkb_state *base_state, xkb_keycode_t kc,
                     क्रमागत xkb_कुंजी_direction direction)
{
    /* Guard against client-only state */
    निश्चित(base_state->mode > SERVER_COMPANION);
    अगर (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        वापस 0;
    }

    काष्ठा xkb_server_state * स्थिर state =
        (काष्ठा xkb_server_state *)base_state;

    स्थिर काष्ठा xkb_कुंजी* स्थिर कुंजी = Xkb_कुंजी(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    अगर (!कुंजी || (direction == XKB_कुंजी_REPEATED && !कुंजी->repeats))
        वापस 0;

    अगर (direction == XKB_कुंजी_DOWN &&
        (state->base.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_कुंजियाँ) &&
        (state->a11y_flags & XKB_A11_Y_STICKY_कुंजियाँ_NO_SIMULTANEOUS_कुंजियाँ) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_कुंजियाँ,
                                      0, शून्य);
    }

    स्थिर काष्ठा state_components पूर्व_components = state->base.components;

    state->set_mods = 0;
    state->clear_mods = 0;

    xkb_filter_apply_all(state, शून्य, कुंजी, direction);

    xkb_mod_सूचकांक_t i;
    xkb_mod_mask_t bit;
    क्रम (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        अगर (state->set_mods & bit) {
            state->mod_कुंजी_गिनती[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    क्रम (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        अगर (state->clear_mods & bit) {
            state->mod_कुंजी_गिनती[i]--;
            अगर (state->mod_कुंजी_गिनती[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_कुंजी_गिनती[i] = 0;
            }
            state->clear_mods &= ~bit;
        }
    }

    xkb_state_update_derived(&state->base);

    वापस get_state_component_changes(&पूर्व_components, &state->base.components);
}

/* We need fake keys for `update_latch_modifiers` and `update_latch_group`.
 * These keys must have at least one level in order to break latches. We need 2
 * keys with specific actions in order to update group/mod latches without
 * affecting each other. */
अटल काष्ठा xkb_कुंजी_type_entry synthetic_कुंजी_level_entry = { 0 };
अटल काष्ठा xkb_कुंजी_type synthetic_कुंजी_type = {
     .अंक_entries = 1,
     .अंक_levels = 1,
     .entries = &synthetic_कुंजी_level_entry
};
अटल स्थिर काष्ठा xkb_कुंजी synthetic_कुंजी = { 0 };

/* Transcription from xserver: XkbLatchModifiers */
अटल व्योम
update_latch_modifiers(काष्ठा xkb_server_state *state,
                       काष्ठा xkb_events *events,
                       xkb_mod_mask_t mask, xkb_mod_mask_t latches)
{
    /* Clear affected latched modifiers */
    स्थिर xkb_mod_mask_t clear = mask & ~latches;
    state->base.components.latched_mods &= ~clear;

    /* Clear any pending latch to locks using ad hoc action:
     * only affect corresponding modifier latches and no group latch. */
    काष्ठा xkb_level synthetic_कुंजी_level_break_mod_latch = {
        .अंक_syms = 0,
        .अंक_actions = 1,
        .upper = XKB_कुंजी_No_प्रतीक,
        .s.sym = XKB_कुंजी_No_प्रतीक,
        .a.action.internal = {
            .type = ACTION_TYPE_INTERNAL,
            .flags = INTERNAL_BREAKS_MOD_LATCH,
            .clear_latched_mods = clear
        }
    };
    काष्ठा xkb_group synthetic_कुंजी_group_break_mod_latch = {
        .type = &synthetic_कुंजी_type,
        .levels = &synthetic_कुंजी_level_break_mod_latch
    };
    स्थिर काष्ठा xkb_कुंजी synthetic_कुंजी_break_mod_latch = {
        .अंक_groups = 1,
        .groups = &synthetic_कुंजी_group_break_mod_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_कुंजी_break_mod_latch, XKB_कुंजी_DOWN);

    /* Finally set the latched mods by simulate tapping a key with the
     * corresponding action */
    स्थिर काष्ठा xkb_कुंजी* स्थिर कुंजी = &synthetic_कुंजी;
    स्थिर जोड़ xkb_action latch_mods = {
        .mods = {
            .type = ACTION_TYPE_MOD_LATCH,
            .mods = { .mask = mask & latches },
            .flags = 0,
        },
    };
    काष्ठा xkb_filter* स्थिर filter = xkb_filter_new(state);
    filter->कुंजी = कुंजी;
    filter->func = xkb_filter_mod_latch_func;
    filter->action = latch_mods;
    xkb_filter_mod_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_mod_latch_func(state, events, filter, कुंजी, XKB_कुंजी_UP);
}

/* Transcription from xserver: XkbLatchGroup */
अटल व्योम
update_latch_group(काष्ठा xkb_server_state *state,
                   काष्ठा xkb_events *events, int32_t group)
{
    /* Clear any pending latch to locks. */
    अटल काष्ठा xkb_level synthetic_कुंजी_level_break_group_latch = {
        .अंक_syms = 0,
        .अंक_actions = 1,
        .upper = XKB_कुंजी_No_प्रतीक,
        .s.sym = XKB_कुंजी_No_प्रतीक,
        .a.action.internal = {
            .type = ACTION_TYPE_INTERNAL,
            .flags = INTERNAL_BREAKS_GROUP_LATCH,
            .clear_latched_mods = 0
        }
    };
    अटल काष्ठा xkb_group synthetic_कुंजी_group_break_group_latch = {
        .type = &synthetic_कुंजी_type,
        .levels = &synthetic_कुंजी_level_break_group_latch
    };
    अटल स्थिर काष्ठा xkb_कुंजी synthetic_कुंजी_break_group_latch = {
        .अंक_groups = 1,
        .groups = &synthetic_कुंजी_group_break_group_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_कुंजी_break_group_latch, XKB_कुंजी_DOWN);

    /* Simulate tapping a key with a group latch action, but in isolation: i.e.
     * without affecting the other filters. */
    स्थिर काष्ठा xkb_कुंजी* स्थिर कुंजी = &synthetic_कुंजी;
    स्थिर जोड़ xkb_action latch_group = {
        .group = {
            .type = ACTION_TYPE_GROUP_LATCH,
            .flags = ACTION_ABSOLUTE_SWITCH,
            .group = group,
        },
    };
    काष्ठा xkb_filter* स्थिर filter = xkb_filter_new(state);
    filter->कुंजी = कुंजी;
    filter->func = xkb_filter_group_latch_func;
    filter->action = latch_group;
    xkb_filter_group_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_group_latch_func(state, events, filter, कुंजी, XKB_कुंजी_UP);
}

/**
 * Compute the resolved effective mask of an arbitrary input.
 *
 * Contrary to `mod_mask_get_effective`, it resolves only modifiers not present
 * in the canonical mask, so that it enables `xkb_state_serialize_mods` to
 * round trip via `xkb_state_update_mask`.
 */
अटल अंतरभूत xkb_mod_mask_t
resolve_to_canonical_mods(काष्ठा xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    वापस
        /*
         * Keep canonical modifier mask.
         * It contains either real modifiers or canonical virtual modifiers.
         */
        (mods & keymap->canonical_state_mask) |
        /* Resolve other modifiers */
        mod_mask_get_effective(keymap,
                               mods & ~keymap->canonical_state_mask);
}

अटल व्योम
state_update_latched_locked(
    काष्ठा xkb_server_state * restrict state,
    स्थिर काष्ठा xkb_state_components_update * restrict update,
    काष्ठा xkb_events * restrict events
)
{
    /* Update locks */
    स्थिर xkb_mod_mask_t affect_locked_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_locked_mods);
    अगर ((update->components & XKB_STATE_MODS_LOCKED) &&
        affect_locked_mods) {
        स्थिर xkb_mod_mask_t locked_mods =
            resolve_to_canonical_mods(state->base.keymap, update->locked_mods);
        state->base.components.locked_mods &= ~affect_locked_mods;
        state->base.components.locked_mods |= locked_mods & affect_locked_mods;
    }
    अगर (update->components & XKB_STATE_LAYOUT_LOCKED) {
        state->base.components.locked_group = update->locked_layout;
    }

    /* Update latches */
    स्थिर xkb_mod_mask_t affect_latched_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_latched_mods);
    अगर ((update->components & XKB_STATE_MODS_LATCHED) &&
        affect_latched_mods) {
        स्थिर xkb_mod_mask_t latched_mods =
            resolve_to_canonical_mods(state->base.keymap, update->latched_mods);
        update_latch_modifiers(state, events, affect_latched_mods, latched_mods);
    }
    अगर (update->components & XKB_STATE_LAYOUT_LATCHED) {
        update_latch_group(state, events, update->latched_layout);
    }
}

क्रमागत xkb_state_component
xkb_state_update_latched_locked(काष्ठा xkb_state *base_state,
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
    निश्चित(base_state->mode > SERVER_COMPANION);
    अगर (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        वापस 0;
    }

    काष्ठा xkb_server_state * स्थिर state =
        (काष्ठा xkb_server_state *)base_state;

    स्थिर काष्ठा state_components previous_components = state->base.components;
    स्थिर क्रमागत xkb_state_component components
        = ((affect_latched_mods || latched_mods) ? XKB_STATE_MODS_LATCHED : 0)
        | ((affect_locked_mods || locked_mods) ? XKB_STATE_MODS_LOCKED : 0)
        | (affect_latched_layout ? XKB_STATE_LAYOUT_LATCHED : 0)
        | (affect_locked_layout ? XKB_STATE_LAYOUT_LOCKED : 0);
    स्थिर काष्ठा xkb_state_components_update update = {
        .आकार = माप(update),
        .components = components,
        .affect_latched_mods = affect_latched_mods,
        .latched_mods = latched_mods,
        .latched_layout = latched_layout,
        .affect_locked_mods = affect_locked_mods,
        .locked_mods = locked_mods,
        .locked_layout = locked_layout
    };
    state_update_latched_locked(state, &update, शून्य);
    xkb_state_update_derived(&state->base);
    वापस get_state_component_changes(&previous_components,
                                       &state->base.components);
}

अटल अंतरभूत व्योम
clear_all_latches_and_locks(काष्ठा xkb_server_state *state,
                            काष्ठा xkb_events *events)
{
    अटल स्थिर क्रमागत xkb_state_component components
        = XKB_STATE_MODS_LATCHED | XKB_STATE_MODS_LOCKED
        | XKB_STATE_LAYOUT_LATCHED | XKB_STATE_LAYOUT_LOCKED;
    स्थिर काष्ठा xkb_state_components_update update = {
        .आकार = माप(update),
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

अटल क्रमागत xkb_status
state_update_layout_policy(काष्ठा xkb_server_state *state,
                           स्थिर काष्ठा xkb_layout_policy_update *update)
{
    अगर (xkb_feature_supported(XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY,
                              update->policy)) {
        अगर (update->policy == XKB_LAYOUT_OUT_OF_RANGE_REDIRECT) {
            अगर (update->redirect < state->base.keymap->अंक_groups) {
                state->base.out_of_range_group.redirect_group =
                    update->redirect;
            } अन्यथा {
                log_err(state->base.keymap->ctx,
                        XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                        "Layout policy: "
                        "unsupported layout index %"PRIu32" > %"PRIu32"\n",
                        update->redirect + 1, state->base.keymap->अंक_groups);
                वापस XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक;
            }
        }
        state->base.out_of_range_group.policy = update->policy;
        वापस XKB_SUCCESS;
    } अन्यथा {
        log_err(state->base.keymap->ctx,
                XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY_,
                "Unsupported layout policy: %d\n", update->policy);
        वापस XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY;
    }
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
अटल क्रमागत xkb_status
check_state_update_abi_(काष्ठा xkb_context * restrict ctx,
                        स्थिर अक्षर * restrict func,
                        स्थिर काष्ठा xkb_synthetic_update * restrict update)
{
    क्रमागत xkb_status status = XKB_SUCCESS;
    अगर ((status = xkb_check_state_abi(update)) ||
        (update->reserved0 != 0 &&
         (status = XKB_ERROR_ABI_FORWARD_COMPAT)) ||
        (update->components &&
         (status = xkb_check_state_abi(update->components))) ||
        (update->layout_policy &&
         (status = xkb_check_state_abi(update->layout_policy)))) {
        xkb_log_abi_error(ctx, func, status);
    }
    वापस status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

#घोषणा check_state_update_abi(ctx, update) \
    check_state_update_abi_(ctx, __func__, update)

क्रमागत xkb_status
xkb_state_update_synthetic(काष्ठा xkb_state * base_state,
                           स्थिर काष्ठा xkb_synthetic_update * update,
                           क्रमागत xkb_state_component *changed)
{
    /* Guard against client-only state */
    अगर (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        वापस XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    काष्ठा xkb_server_state * स्थिर state =
        (काष्ठा xkb_server_state *)base_state;

    /* Check ABI compatibility */
    क्रमागत xkb_status status = check_state_update_abi(state->base.keymap->ctx,
                                                    update);
    अगर (status != XKB_SUCCESS)
        वापस status;

    स्थिर काष्ठा state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    /* Update parametrized controls first */
    अगर (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        अगर (status != XKB_SUCCESS)
            वापस status;
    }

    अगर (update->components) {
        स्थिर काष्ठा xkb_state_components_update * स्थिर components =
            update->components;
        /* Update boolean controls first */
        अगर (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, शून्य);
        }

        state_update_latched_locked(state, components, शून्य);
    }

    xkb_state_update_derived(&state->base);

    अगर (changed) {
        *changed = get_state_component_changes(&previous_components,
                                               &state->base.components);
    }
    वापस XKB_SUCCESS;
}

/**
 * Updates the state from a set of explicit masks as gained from
 * xkb_state_serialize_mods and xkb_state_serialize_groups.  As noted in the
 * documentation for these functions in xkbcommon.h, this round-trip is
 * lossy, and should only be used to update a slave state mirroring the
 * master, e.g. in a client/server window system.
 */
क्रमागत xkb_state_component
xkb_state_update_mask(काष्ठा xkb_state *base_state,
                      xkb_mod_mask_t base_mods,
                      xkb_mod_mask_t latched_mods,
                      xkb_mod_mask_t locked_mods,
                      xkb_layout_सूचकांक_t base_group,
                      xkb_layout_सूचकांक_t latched_group,
                      xkb_layout_सूचकांक_t locked_group)
{
    /* Guard against server-only and server companion state */
    निश्चित(base_state->mode == CLIENT_STATE ||
           base_state->mode == LEGACY_MIXED_STATE);
    अगर (base_state->mode != CLIENT_STATE &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        वापस 0;
    }

    काष्ठा xkb_client_state * स्थिर state =
        (काष्ठा xkb_client_state *)base_state;

    स्थिर काष्ठा state_components पूर्व_components = state->base.components;

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

    वापस get_state_component_changes(&पूर्व_components, &state->base.components);
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Lock_Modifier
 */
अटल bool
should_do_caps_transformation(काष्ठा xkb_state *state, xkb_keycode_t kc)
{
    वापस
        xkb_state_mod_सूचकांक_is_active(state, XKB_MOD_सूचकांक_CAPS,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_सूचकांक_is_consumed(state, kc, XKB_MOD_सूचकांक_CAPS) == 0;
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Control_Modifier
 */
अटल bool
should_do_ctrl_transformation(काष्ठा xkb_state *state, xkb_keycode_t kc)
{
    वापस
        xkb_state_mod_सूचकांक_is_active(state, XKB_MOD_सूचकांक_CTRL,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_सूचकांक_is_consumed(state, kc, XKB_MOD_सूचकांक_CTRL) == 0;
}

/**
 * Provides the symbols to use for the given key and state.  Returns the
 * number of symbols pointed to in syms_out.
 */
पूर्णांक
xkb_state_कुंजी_get_syms(काष्ठा xkb_state *state, xkb_keycode_t kc,
                       स्थिर xkb_keysym_t **syms_out)
{
    स्थिर xkb_layout_सूचकांक_t layout = xkb_state_कुंजी_get_layout(state, kc);
    अगर (layout == XKB_LAYOUT_INVALID)
        जाओ err;

    स्थिर xkb_level_सूचकांक_t level = xkb_state_कुंजी_get_level(state, kc, layout);
    अगर (level == XKB_LEVEL_INVALID)
        जाओ err;

    स्थिर काष्ठा xkb_कुंजी* स्थिर कुंजी = Xkb_कुंजी(state->keymap, kc);

    अगर (!कुंजी)
        जाओ err;

    स्थिर काष्ठा xkb_level* स्थिर leveli =
        xkb_keymap_कुंजी_get_level(state->keymap, कुंजी, layout, level);

    अगर (!leveli)
        जाओ err;

    स्थिर xkb_keysym_गिनती_t अंक_syms = leveli->अंक_syms;
    अगर (अंक_syms == 0)
        जाओ err;

    अगर (should_do_caps_transformation(state, kc)) {
        /* Only simple capitalization rules: keysyms count is unchanged. */
        अगर (अंक_syms > 1) {
            *syms_out = (leveli->has_upper)
                      ? leveli->s.syms + अंक_syms
                      : leveli->s.syms;
        } अन्यथा {
            *syms_out = &leveli->upper;
        }
    } अन्यथा {
        *syms_out = (अंक_syms > 1)
                  ? leveli->s.syms
                  : &leveli->s.sym;
    }
    वापस (पूर्णांक) अंक_syms;

err:
    *syms_out = शून्य;
    वापस 0;
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
अटल अक्षर
XkbToControl(अक्षर ch)
{
    अक्षर c = ch;

    अगर ((c >= '@' && c < '\177') || c == ' ')
        c &= 0x1F;
    अन्यथा अगर (c == '2')
        c = '\000';
    अन्यथा अगर (c >= '3' && c <= '7')
        c -= ('3' - '\033');
    अन्यथा अगर (c == '8')
        c = '\177';
    अन्यथा अगर (c == '/')
        c = '_' & 0x1F;
    वापस c;
}

/**
 * Provides either exactly one symbol, or XKB_KEY_NoSymbol.
 */
xkb_keysym_t
xkb_state_कुंजी_get_one_sym(काष्ठा xkb_state *state, xkb_keycode_t kc)
{
    स्थिर xkb_keysym_t *syms = शून्य;
    स्थिर पूर्णांक अंक_syms = xkb_state_कुंजी_get_syms(state, kc, &syms);

    अगर (अंक_syms != 1)
        वापस XKB_कुंजी_No_प्रतीक;
    अन्यथा
        वापस syms[0];
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
अटल xkb_keysym_t
get_one_sym_for_string(काष्ठा xkb_state *state, xkb_keycode_t kc)
{
    स्थिर xkb_layout_सूचकांक_t layout = xkb_state_कुंजी_get_layout(state, kc);
    स्थिर xkb_layout_सूचकांक_t अंक_layouts =
        xkb_keymap_अंक_layouts_for_कुंजी(state->keymap, kc);
    xkb_level_सूचकांक_t level = xkb_state_कुंजी_get_level(state, kc, layout);
    अगर (layout == XKB_LAYOUT_INVALID || अंक_layouts == 0 ||
        level == XKB_LEVEL_INVALID)
        वापस XKB_कुंजी_No_प्रतीक;

    स्थिर xkb_keysym_t *syms = शून्य;
    पूर्णांक nsyms = xkb_keymap_कुंजी_get_syms_by_level(state->keymap, kc,
                                                 layout, level, &syms);
    अगर (nsyms != 1)
        वापस XKB_कुंजी_No_प्रतीक;
    xkb_keysym_t sym = syms[0];

    अगर (should_do_ctrl_transformation(state, kc) && sym > 127u) {
        क्रम (xkb_layout_सूचकांक_t i = 0; i < अंक_layouts; i++) {
            level = xkb_state_कुंजी_get_level(state, kc, i);
            अगर (level == XKB_LEVEL_INVALID)
                जारी;

            nsyms = xkb_keymap_कुंजी_get_syms_by_level(state->keymap, kc,
                                                     i, level, &syms);
            अगर (nsyms == 1 && syms[0] <= 127u) {
                sym = syms[0];
                अवरोध;
            }
        }
    }

    अगर (should_do_caps_transformation(state, kc)) {
        sym = xkb_keysym_to_upper(sym);
    }

    वापस sym;
}

पूर्णांक
xkb_state_कुंजी_get_utf8(काष्ठा xkb_state *state, xkb_keycode_t kc,
                       अक्षर *बफर, माप_प्रकार आकार)
{
    पूर्णांक nsyms;
    स्थिर xkb_keysym_t *syms = शून्य;
    स्थिर xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    अगर (sym != XKB_कुंजी_No_प्रतीक) {
        nsyms = 1; syms = &sym;
    }
    अन्यथा {
        nsyms = xkb_state_कुंजी_get_syms(state, kc, &syms);
    }

    /* Make sure not to truncate in the middle of a UTF-8 sequence. */
    पूर्णांक offset = 0;
    अक्षर tmp[XKB_KEYSYM_UTF8_MAX_आकार];
    क्रम (पूर्णांक i = 0; i < nsyms; i++) {
        पूर्णांक ret = xkb_keysym_to_utf8(syms[i], tmp, माप(tmp));
        अगर (ret <= 0)
            जाओ err_bad;

        ret--;
        अगर ((माप_प्रकार)offset + (माप_प्रकार)ret <= आकार)
            स_नकल(बफर + offset, tmp, (माप_प्रकार)ret);
        offset += ret;
    }

    अगर ((माप_प्रकार)offset >= आकार)
        जाओ err_trunc;
    बफर[offset] = '\0';

    अगर (!is_valid_utf8(बफर, (माप_प्रकार)offset))
        जाओ err_bad;

    अगर (offset == 1 && (अचिन्हित पूर्णांक) बफर[0] <= 127u &&
        should_do_ctrl_transformation(state, kc))
        बफर[0] = XkbToControl(बफर[0]);

    वापस offset;

err_trunc:
    अगर (आकार > 0)
        बफर[आकार - 1] = '\0';
    वापस offset;

err_bad:
    अगर (आकार > 0)
        बफर[0] = '\0';
    वापस 0;
}

uint32_t
xkb_state_कुंजी_get_utf32(काष्ठा xkb_state *state, xkb_keycode_t kc)
{
    स्थिर xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    uint32_t cp = xkb_keysym_to_utf32(sym);

    अगर (cp <= 127u && should_do_ctrl_transformation(state, kc))
        cp = (uint32_t) XkbToControl((अक्षर) cp);

    वापस cp;
}

/**
 * Serializes the requested modifier state into an xkb_mod_mask_t, with all
 * the same disclaimers as in xkb_state_update_mask.
 */
अटल अंतरभूत xkb_mod_mask_t
serialize_mods(स्थिर काष्ठा state_components *components,
               क्रमागत xkb_state_component type)
{
    xkb_mod_mask_t ret = 0;

    अगर (type & XKB_STATE_MODS_EFFECTIVE)
        वापस components->mods;

    अगर (type & XKB_STATE_MODS_DEPRESSED)
        ret |= components->base_mods;
    अगर (type & XKB_STATE_MODS_LATCHED)
        ret |= components->latched_mods;
    अगर (type & XKB_STATE_MODS_LOCKED)
        ret |= components->locked_mods;

    वापस ret;
}

xkb_mod_mask_t
xkb_state_serialize_mods(काष्ठा xkb_state *state,
                         क्रमागत xkb_state_component type)
{
    वापस serialize_mods(&state->components, type);
}

/**
 * Serialises the requested group state, with all the same disclaimers as
 * in xkb_state_update_mask.
 */
अटल अंतरभूत xkb_layout_सूचकांक_t
serialize_layout(स्थिर काष्ठा state_components *components,
                 क्रमागत xkb_state_component type)
{
    अगर (type & XKB_STATE_LAYOUT_EFFECTIVE)
        वापस components->group;

    int32_t ret = 0;
    अगर (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret += components->base_group;
    अगर (type & XKB_STATE_LAYOUT_LATCHED)
        ret += components->latched_group;
    अगर (type & XKB_STATE_LAYOUT_LOCKED)
        ret += components->locked_group;

    वापस (xkb_layout_सूचकांक_t)ret;
}

xkb_layout_सूचकांक_t
xkb_state_serialize_layout(काष्ठा xkb_state *state,
                           क्रमागत xkb_state_component type)
{
    वापस serialize_layout(&state->components, type);
}

xkb_led_mask_t
xkb_state_serialize_leds(स्थिर काष्ठा xkb_state *state,
                         क्रमागत xkb_state_component type)
{
    वापस (type & XKB_STATE_LEDS)
        ? state->components.leds
        : 0;
}

अटल अंतरभूत क्रमागत xkb_keyboard_control_flags
serialize_controls(स्थिर काष्ठा state_components *components,
                   क्रमागत xkb_state_component type)
{
    वापस (type & XKB_STATE_CONTROLS_EFFECTIVE)
        /*
         * Enable using the public API with the all the Controls values, except
         * the internal ones, if any.
         */
        ? (क्रमागत xkb_keyboard_control_flags)
          (components->controls & CONTROL_ALL_BOOLEAN)
        : 0;
}

क्रमागत xkb_keyboard_control_flags
xkb_state_serialize_controls(स्थिर काष्ठा xkb_state *state,
                             क्रमागत xkb_state_component type)
{
    वापस serialize_controls(&state->components, type);
}

/**
 * Gets a modifier mask and returns the resolved effective mask; this
 * is needed because some modifiers can also map to other modifiers, e.g.
 * the "NumLock" modifier usually also sets the "Mod2" modifier.
 */
xkb_mod_mask_t
mod_mask_get_effective(काष्ठा xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    /* Initialize the effective mask with its corresponding real mods. */
    xkb_mod_mask_t mask = mods & MOD_REAL_MASK_ALL;

    /* Resolve the virtual modifiers */
    स्थिर काष्ठा xkb_mod *mod;
    xkb_mod_सूचकांक_t i;
    xkb_vmods_enumerate(i, mod, &keymap->mods)
        अगर (mods & (UINT32_C(1) << i))
            mask |= mod->mapping;

    वापस mask;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
पूर्णांक
xkb_state_mod_सूचकांक_is_active(काष्ठा xkb_state *state,
                              xkb_mod_सूचकांक_t idx,
                              क्रमागत xkb_state_component type)
{
    अगर (unlikely(idx >= xkb_keymap_अंक_mods(state->keymap)))
        वापस -1;

    स्थिर xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    अगर (!mapping) {
        /* Modifier not mapped */
        वापस 0;
    }
    /* WARNING: this may overmatch for virtual modifiers */
    वापस (xkb_state_serialize_mods(state, type) & mapping) == mapping;
}

क्रमागत {
    XKB_STATE_MATCH_FLAGS = XKB_STATE_MATCH_ANY
                          | XKB_STATE_MATCH_ALL
                          | XKB_STATE_MATCH_NON_EXCLUSIVE
};

/**
 * Helper function for xkb_state_mod_indices_are_active and
 * xkb_state_mod_names_are_active.
 */
अटल bool
match_mod_masks(काष्ठा xkb_state *state,
                क्रमागत xkb_state_component type,
                क्रमागत xkb_state_match match,
                xkb_mod_mask_t wanted)
{
    स्थिर xkb_mod_mask_t active = xkb_state_serialize_mods(state, type);

    अगर (!(match & XKB_STATE_MATCH_NON_EXCLUSIVE) && (active & ~wanted))
        वापस false;

    अगर (match & XKB_STATE_MATCH_ANY)
        वापस active & wanted;

    /* Ensure all flags but XKB_STATE_MATCH_ALL are processed before */
    static_assert(XKB_STATE_MATCH_ALL ==
                  ((क्रमागत xkb_state_match) XKB_STATE_MATCH_FLAGS &
                   ~(XKB_STATE_MATCH_ANY | XKB_STATE_MATCH_NON_EXCLUSIVE)),
                  "");

    वापस (active & wanted) == wanted;
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, or -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
पूर्णांक
xkb_state_mod_indices_are_active(काष्ठा xkb_state *state,
                                 क्रमागत xkb_state_component type,
                                 क्रमागत xkb_state_match match,
                                 ...)
{
    अगर (match & ~((क्रमागत xkb_state_match) XKB_STATE_MATCH_FLAGS))
        वापस -2;

    बहु_सूची ap;
    xkb_mod_mask_t wanted = 0;
    पूर्णांक ret = 0;
    स्थिर xkb_mod_सूचकांक_t अंक_mods = xkb_keymap_अंक_mods(state->keymap);

    बहु_शुरू(ap, match);
    जबतक (1) {
        xkb_mod_सूचकांक_t idx = बहु_तर्क(ap, xkb_mod_सूचकांक_t);
        अगर (idx == XKB_MOD_INVALID)
            अवरोध;
        अगर (unlikely(idx >= अंक_mods)) {
            ret = -1;
            अवरोध;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    बहु_पूर्ण(ap);

    अगर (ret == -1)
        वापस ret;

    अगर (!wanted) {
        /* Modifiers not mapped */
        वापस 0;
    }

    वापस match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
पूर्णांक
xkb_state_mod_name_is_active(काष्ठा xkb_state *state, स्थिर अक्षर *name,
                             क्रमागत xkb_state_component type)
{
    स्थिर xkb_mod_सूचकांक_t idx = xkb_keymap_mod_get_सूचकांक(state->keymap, name);

    अगर (idx == XKB_MOD_INVALID)
        वापस -1;

    वापस xkb_state_mod_सूचकांक_is_active(state, idx, type);
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ATTR_NULL_SENTINEL पूर्णांक
xkb_state_mod_names_are_active(काष्ठा xkb_state *state,
                               क्रमागत xkb_state_component type,
                               क्रमागत xkb_state_match match,
                               ...)
{
    अगर (match & ~((क्रमागत xkb_state_match) XKB_STATE_MATCH_FLAGS))
        वापस -2;

    बहु_सूची ap;
    xkb_mod_mask_t wanted = 0;
    पूर्णांक ret = 0;

    बहु_शुरू(ap, match);
    जबतक (1) {
        स्थिर अक्षर *str = बहु_तर्क(ap, स्थिर अक्षर *);
        अगर (str == शून्य)
            अवरोध;
        स्थिर xkb_mod_सूचकांक_t idx = xkb_keymap_mod_get_सूचकांक(state->keymap, str);
        अगर (idx == XKB_MOD_INVALID) {
            ret = -1;
            अवरोध;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    बहु_पूर्ण(ap);

    अगर (ret == -1)
        वापस ret;

    अगर (!wanted) {
        /* Modifiers not mapped */
        वापस 0;
    }

    वापस match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given group is active with the specified type(s), 0 if
 * not, or -1 if the group is invalid.
 */
पूर्णांक
xkb_state_layout_सूचकांक_is_active(काष्ठा xkb_state *state,
                                 xkb_layout_सूचकांक_t idx,
                                 क्रमागत xkb_state_component type)
{
    अगर (idx >= state->keymap->अंक_groups)
        वापस -1;

    पूर्णांक ret = 0;
    अगर (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ret |= (state->components.group == idx);
    अगर (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret |= (state->components.base_group == (int32_t) idx);
    अगर (type & XKB_STATE_LAYOUT_LATCHED)
        ret |= (state->components.latched_group == (int32_t) idx);
    अगर (type & XKB_STATE_LAYOUT_LOCKED)
        ret |= (state->components.locked_group == (int32_t) idx);

    वापस ret;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
पूर्णांक
xkb_state_layout_name_is_active(काष्ठा xkb_state *state, स्थिर अक्षर *name,
                                क्रमागत xkb_state_component type)
{
    स्थिर xkb_layout_सूचकांक_t idx =
        xkb_keymap_layout_get_सूचकांक(state->keymap, name);

    अगर (idx == XKB_LAYOUT_INVALID)
        वापस -1;

    वापस xkb_state_layout_सूचकांक_is_active(state, idx, type);
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
पूर्णांक
xkb_state_led_सूचकांक_is_active(काष्ठा xkb_state *state, xkb_led_सूचकांक_t idx)
{
    अगर (idx >= state->keymap->अंक_leds ||
        state->keymap->leds[idx].name == XKB_ATOM_NONE)
        वापस -1;

    वापस !!(state->components.leds & (UINT32_C(1) << idx));
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
पूर्णांक
xkb_state_led_name_is_active(काष्ठा xkb_state *state, स्थिर अक्षर *name)
{
    स्थिर xkb_led_सूचकांक_t idx = xkb_keymap_led_get_सूचकांक(state->keymap, name);

    अगर (idx == XKB_LED_INVALID)
        वापस -1;

    वापस xkb_state_led_सूचकांक_is_active(state, idx);
}

/**
 * See:
 * - XkbTranslateKeyCode(3), mod_rtrn return value, from libX11.
 * - MyEnhancedXkbTranslateKeyCode(), a modification of the above, from GTK+.
 */
अटल xkb_mod_mask_t
कुंजी_get_consumed(काष्ठा xkb_state *state, स्थिर काष्ठा xkb_कुंजी *कुंजी,
                 क्रमागत xkb_consumed_mode mode)
{
    स्थिर xkb_layout_सूचकांक_t group =
        xkb_state_कुंजी_get_layout(state, कुंजी->keycode);
    अगर (group == XKB_LAYOUT_INVALID)
        वापस 0;

    xkb_mod_mask_t preserve = 0;
    xkb_mod_mask_t consumed = 0;

    स्थिर काष्ठा xkb_कुंजी_type_entry* स्थिर matching_entry =
        get_entry_for_कुंजी_state(state, कुंजी, group);
    अगर (matching_entry)
        preserve = matching_entry->preserve.mask;

    स्थिर काष्ठा xkb_कुंजी_type* स्थिर type = कुंजी->groups[group].type;
    चयन (mode) {
    हाल XKB_CONSUMED_MODE_XKB:
        consumed = type->mods.mask;
        अवरोध;

    हाल XKB_CONSUMED_MODE_GTK: {
        स्थिर काष्ठा xkb_कुंजी_type_entry* स्थिर no_mods_entry =
            get_entry_for_mods(type, 0);
        स्थिर xkb_level_सूचकांक_t no_mods_leveli = no_mods_entry
                                               ? no_mods_entry->level
                                               : 0;
        स्थिर काष्ठा xkb_level* स्थिर no_mods_level =
            &कुंजी->groups[group].levels[no_mods_leveli];

        क्रम (darray_आकार_t i = 0; i < type->अंक_entries; i++) {
            स्थिर काष्ठा xkb_कुंजी_type_entry* स्थिर entry = &type->entries[i];
            अगर (!entry_is_active(entry))
                जारी;

            स्थिर काष्ठा xkb_level* स्थिर level =
                &कुंजी->groups[group].levels[entry->level];
            अगर (XkbLevelsSameSyms(level, no_mods_level))
                जारी;

            अगर (entry == matching_entry || one_bit_set(entry->mods.mask))
                consumed |= entry->mods.mask & ~entry->preserve.mask;
        }
        अवरोध;
    }
    }

    वापस consumed & ~preserve;
}

पूर्णांक
xkb_state_mod_सूचकांक_is_consumed2(काष्ठा xkb_state *state, xkb_keycode_t kc,
                                 xkb_mod_सूचकांक_t idx,
                                 क्रमागत xkb_consumed_mode mode)
{
    स्थिर काष्ठा xkb_कुंजी* स्थिर कुंजी = Xkb_कुंजी(state->keymap, kc);

    अगर (unlikely(!कुंजी || idx >= xkb_keymap_अंक_mods(state->keymap)))
        वापस -1;

    स्थिर xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    अगर (!mapping) {
        /* Modifier not mapped */
        वापस 0;
    }
    वापस (mapping & कुंजी_get_consumed(state, कुंजी, mode)) == mapping;
}

पूर्णांक
xkb_state_mod_सूचकांक_is_consumed(काष्ठा xkb_state *state, xkb_keycode_t kc,
                                xkb_mod_सूचकांक_t idx)
{
    वापस xkb_state_mod_सूचकांक_is_consumed2(state, kc, idx,
                                            XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_mod_mask_remove_consumed(काष्ठा xkb_state *state, xkb_keycode_t kc,
                                   xkb_mod_mask_t mask)
{
    स्थिर काष्ठा xkb_कुंजी* स्थिर कुंजी = Xkb_कुंजी(state->keymap, kc);

    अगर (!कुंजी)
        वापस 0;

    वापस resolve_to_canonical_mods(state->keymap, mask) &
           ~कुंजी_get_consumed(state, कुंजी, XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_कुंजी_get_consumed_mods2(काष्ठा xkb_state *state, xkb_keycode_t kc,
                                 क्रमागत xkb_consumed_mode mode)
{
    चयन (mode) {
    हाल XKB_CONSUMED_MODE_XKB:
    हाल XKB_CONSUMED_MODE_GTK:
        अवरोध;
    शेष:
        log_err_func(state->keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "unrecognized consumed modifiers mode: %d\n", mode);
        वापस 0;
    }

    स्थिर काष्ठा xkb_कुंजी* स्थिर कुंजी = Xkb_कुंजी(state->keymap, kc);
    अगर (!कुंजी)
        वापस 0;

    वापस कुंजी_get_consumed(state, कुंजी, mode);
}

xkb_mod_mask_t
xkb_state_कुंजी_get_consumed_mods(काष्ठा xkb_state *state, xkb_keycode_t kc)
{
    वापस xkb_state_कुंजी_get_consumed_mods2(state, kc, XKB_CONSUMED_MODE_XKB);
}

/*******************************************************************************
 * Server state machine API
 ******************************************************************************/

/** Entry to keep track of the state of an overlaid key */
काष्ठा xkb_overlaid_कुंजी {
    /** The source key being overlaid */
    स्थिर काष्ठा xkb_कुंजी *old;
    /** The target key */
    स्थिर काष्ठा xkb_कुंजी *new;
    /** Counter of key currently down */
    पूर्णांक refcnt;
};

काष्ठा xkb_shortcuts_config_entry {
    /** Real modifier mask to trigger shortcuts tweaks */
    xkb_mod_mask_t mods;
    /** Target layout */
    xkb_layout_सूचकांक_t target;
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
काष्ठा xkb_machine {
    /** Inherits from `xkb_server_state` */
    काष्ठा xkb_server_state base;

    /* Extra server-specific state data */

    /** Keyboard overlays handling */
    काष्ठा {
        /**
         * Current overlaid keys
         *
         * When a key is overlaid, it remained overlaid in this configuration
         * until totally released, i.e. every key press was matched with the
         * corresponding key release.
         */
        darray(काष्ठा xkb_overlaid_कुंजी) कुंजियाँ;
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
    काष्ठा machine_config {
        /** Modifiers remapping */
        काष्ठा machine_modifiers_config {
            /** Real modifier mask */
            xkb_mod_mask_t mask;
            /** Modifiers re-mappings */
            darray_आकार_t mappings_अंक;
            काष्ठा machine_mods_mapping {
                xkb_mod_mask_t source;
                xkb_mod_mask_t target;
            } * mappings ATTR_COUNTED_BY(mappings_अंक);
        } modifiers;

        /** Shortcuts tweak */
        काष्ठा machine_shortcuts_config {
            /** Substitution entries */
            काष्ठा xkb_shortcuts_config_entry *entries;
        } shortcuts;
    } config;

    /** Mouse keys handling */
    काष्ठा {
        /** Default pointer button */
        xkb_pointer_button_सूचकांक_t default_button;
        /** Mask of the locked buttons */
        xkb_pointer_button_mask_t locked_buttons;
    } mouse;

    क्रमागत xkb_machine_flags flags;
};

प्रकार darray(काष्ठा machine_mods_mapping) machine_mods_mappings;

काष्ठा xkb_machine_builder {
    काष्ठा xkb_keymap *keymap;

    /** Controls */
    काष्ठा {
        /** Accessibility flags */
        काष्ठा {
            क्रमागत xkb_a11y_flags affect_flags;
            क्रमागत xkb_a11y_flags flags;
        } a11y;
    } controls;

    /** Modifiers re-mapping */
    machine_mods_mappings mods;

    /** Shortcuts tweak */
    काष्ठा xkb_shortcuts_config_options {
        /** Substitution entries */
        darray(काष्ठा xkb_shortcuts_config_entry) entries;
    } shortcuts;

    क्रमागत xkb_machine_flags machine_flags;
    क्रमागत xkb_machine_builder_flags builder_flags;
    पूर्णांक refcnt;
};

काष्ठा xkb_machine_builder *
xkb_machine_builder_new(
    काष्ठा xkb_keymap * restrict keymap,
    स्थिर काष्ठा xkb_machine_builder_config * restrict config,
    क्रमागत xkb_status * restrict status
)
{
    /* Handle default configuration */
    अटल स्थिर काष्ठा xkb_machine_builder_config default_config = {
        .आकार = माप(default_config)
    };
    अगर (!config)
        config = &default_config;

    /* Check ABI compatibility */
    स्थिर क्रमागत xkb_status status_ = xkb_check_state_abi(config);
    अगर (status_) {
        xkb_log_abi_error(keymap->ctx, __func__, status_);
        अगर (status)
            *status = status_;
        वापस शून्य;
    }

    /* Sanitize input */
    स्थिर क्रमागत xkb_machine_builder_flags invalid_builder_flags
        = config->builder_flags
        & ~(क्रमागत xkb_machine_builder_flags)XKB_MACHINE_BUILDER_FLAGS_मूल्यावली;
    अगर (invalid_builder_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS_,
                     "unrecognized machine builder flags: 0x%x\n",
                     invalid_builder_flags);
        अगर (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS;
        वापस शून्य;
    }

    स्थिर क्रमागत xkb_machine_flags invalid_machine_flags
        = (क्रमागत xkb_machine_flags)config->machine_flags
        & ~(क्रमागत xkb_machine_flags)XKB_MACHINE_FLAGS_मूल्यावली;
    अगर (invalid_machine_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS_,
                     "unrecognized state machine flags: 0x%x\n",
                     invalid_machine_flags);
        अगर (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS;
        वापस शून्य;
    }

    काष्ठा xkb_machine_builder * स्थिर builder = सुस्मृति(1, माप(*builder));
    अगर (!builder) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine builder\n");
        अगर (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        वापस शून्य;
    }

    *builder = (काष्ठा xkb_machine_builder) {
        .keymap = xkb_keymap_ref(keymap),
        .refcnt = 1,
        .builder_flags = (क्रमागत xkb_machine_builder_flags)config->builder_flags,
        .machine_flags = (क्रमागत xkb_machine_flags)config->machine_flags,
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

    अगर (status)
        *status = XKB_SUCCESS;

    वापस builder;
}

काष्ठा xkb_machine_builder *
xkb_machine_builder_ref(काष्ठा xkb_machine_builder *builder)
{
    निश्चित(builder->refcnt > 0);
    builder->refcnt++;
    वापस builder;
}

व्योम
xkb_machine_builder_unref(काष्ठा xkb_machine_builder *builder)
{
    निश्चित(!builder || builder->refcnt > 0);
    अगर (!builder || --builder->refcnt > 0)
        वापस;

    darray_free(builder->shortcuts.entries);
    darray_free(builder->mods);
    xkb_keymap_unref(builder->keymap);
    मुक्त(builder);
}

काष्ठा xkb_keymap *
xkb_machine_builder_get_keymap(स्थिर काष्ठा xkb_machine_builder *builder)
{
    /* Reference count is not updated. See API doc. */
    वापस builder->keymap;
}

क्रमागत xkb_status
xkb_machine_builder_update_a11y(
    काष्ठा xkb_machine_builder * restrict builder,
    स्थिर काष्ठा xkb_machine_builder_a11y_update * restrict update)
{
    /* Check ABI compatibility */
    क्रमागत xkb_status status = xkb_check_state_abi(update);
    अगर (status != XKB_SUCCESS) {
        xkb_log_abi_error(builder->keymap->ctx, __func__, status);
        वापस status;
    }

    स्थिर क्रमागत xkb_a11y_flags invalid_flags =
        ~(क्रमागत xkb_a11y_flags)XKB_A11_Y_FLAGS_मूल्यावली;

    अगर (update->affect_flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y affected flags: %#x\n",
                __func__, update->affect_flags & invalid_flags);
        वापस XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }
    अगर (update->flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y flags: %#x\n",
                __func__, update->flags & invalid_flags);
        वापस XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }

    builder->controls.a11y.affect_flags |= update->affect_flags;
    builder->controls.a11y.flags &= ~update->affect_flags;
    builder->controls.a11y.flags |= (update->flags & update->affect_flags);

    वापस XKB_SUCCESS;
}

क्रमागत xkb_status
xkb_machine_builder_update_mods_remap(
    काष्ठा xkb_machine_builder * restrict builder,
    स्थिर काष्ठा xkb_machine_builder_mods_remap_update * restrict update
)
{
    अगर (!update->source) {
        अगर (!update->target) {
            /* Reset mappings */
            darray_resize(builder->mods, 0);
            वापस XKB_SUCCESS;
        } अन्यथा {
            वापस XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
        }
    }

    /* Check the modifiers against the keymap */
    स्थिर xkb_mod_mask_t invalid = ~builder->keymap->canonical_state_mask;
    अगर ((update->source & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid source modifiers: 0x%"PRIx32"\n",
                __func__, update->source);
        वापस XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    अगर ((update->target & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid target modifiers: 0x%"PRIx32"\n",
                __func__, update->target);
        वापस XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    काष्ठा machine_mods_mapping *mapping = शून्य;
    darray_आकार_t m = 0;
    darray_enumerate(m, mapping, builder->mods) {
        अगर (mapping->source == update->source) {
            अगर (!update->target) {
                /* Remove mapping */
                darray_remove(builder->mods, m);
            } अन्यथा {
                /* Update mapping */
                mapping->target = update->target;
            }
            वापस XKB_SUCCESS;
        }
    }

    अगर (update->target) {
        /* Append new mapping */
        darray_append(
            builder->mods,
            (काष्ठा machine_mods_mapping) {
                .source = update->source,
                .target = update->target
            }
        );
    } अन्यथा {
        /* Ignore missing mapping */
    }

    वापस XKB_SUCCESS;
}

क्रमागत xkb_status
xkb_machine_builder_update_shortcut_override(
    काष्ठा xkb_machine_builder * restrict builder,
    स्थिर काष्ठा xkb_machine_builder_shortcut_override_update * restrict update
)
{
    काष्ठा xkb_keymap *keymap = builder->keymap;

    /* Check the modifiers against the keymap */
    स्थिर xkb_mod_mask_t invalid = ~keymap->canonical_state_mask;
    अगर ((update->affect_mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid affected modifiers: 0x%"PRIx32"\n",
                __func__, update->affect_mods);
        वापस XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    अगर ((update->mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid modifiers: 0x%"PRIx32"\n",
                __func__, update->mods);
        वापस XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    /* Check the layout indices against the keymap */
    अगर (update->source >= keymap->अंक_groups &&
        update->source != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                "%s: Invalid source layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->अंक_groups, update->source + 1);
        वापस XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक;
    }
    अगर (update->target >= keymap->अंक_groups &&
        update->target != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                "%s: Invalid target layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->अंक_groups, update->target + 1);
        वापस XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक;
    }

    काष्ठा xkb_shortcuts_config_options * स्थिर config = &builder->shortcuts;

    /* Resize array & initialize new entries, if relevant */
    अगर (update->source >= darray_आकार(config->entries) &&
        update->source != XKB_LAYOUT_INVALID) {
        अगर (update->target == update->source) {
            /* Skip default setting */
            वापस XKB_SUCCESS;
        }
        xkb_layout_सूचकांक_t new = darray_आकार(config->entries);
        darray_resize(config->entries, update->source + 1);
        क्रम (; new <= update->source; new++) {
            darray_item(config->entries, new) =
                (काष्ठा xkb_shortcuts_config_entry) {
                    .target = XKB_LAYOUT_INVALID,
                    .mods = 0
                };
        }
    }

    काष्ठा xkb_shortcuts_config_entry *entry;
    xkb_layout_सूचकांक_t source = (update->source == XKB_LAYOUT_INVALID)
        ? 0               /* Loop over all entries */
        : update->source; /* Single entry */
    darray_foreach_from(entry, config->entries, source) {
        अगर (update->source == XKB_LAYOUT_INVALID &&
            entry->target == XKB_LAYOUT_INVALID) {
            /* Skip previous deactivated entry */
            source++;
            जारी;
        }

        अगर (update->affect_mods) {
            entry->mods &= ~update->affect_mods;
            entry->mods |= (update->affect_mods & update->mods);
        }

        अगर (update->source != XKB_LAYOUT_INVALID) {
            entry->target = (update->target == source)
                ? XKB_LAYOUT_INVALID
                : update->target;
            /* Single entry: exit loop */
            अवरोध;
        }
        source++;
    }

    वापस XKB_SUCCESS;
}

/** Compare 2 modifiers combinations */
अटल पूर्णांक
cmp_mod_masks(स्थिर व्योम *a, स्थिर व्योम *b)
{
    स्थिर xkb_mod_mask_t m1 = *((xkb_mod_mask_t *) a);
    स्थिर xkb_mod_mask_t m2 = *((xkb_mod_mask_t *) b);
    अगर (m1 == m2)
        वापस 0;

    अगर (!m1)
        वापस +1;
    अगर (!m2)
        वापस -1;

    स्थिर xkb_mod_mask_t overlap = (m1 & m2);
    अगर (overlap == m1)
        वापस +1; /* m1 is a subset of m2 */
    अगर (overlap == m2)
        वापस -1; /* m2 is a subset of m1 */

    /* Compare active modifiers numbers */
    स्थिर अचिन्हित पूर्णांक count1 = popcount32(m1);
    स्थिर अचिन्हित पूर्णांक count2 = popcount32(m2);
    अगर (count1 > count2)
        वापस -1;
    अगर (count1 < count2)
        वापस +1;

    /* Give priority to real modifiers */
    वापस (m1 < m2) ? -1 : +1;
}

अटल क्रमागत xkb_status
machine_set_mods(काष्ठा xkb_machine *sm,
                 स्थिर machine_mods_mappings *raw_mappings)
{
    अगर (!darray_empty(*raw_mappings)) {
        machine_mods_mappings mappings = darray_new();
        xkb_mod_mask_t mask = 0;
        स्थिर काष्ठा machine_mods_mapping *mapping;
        स्थिर xkb_mod_mask_t invalid =
            ~sm->base.base.keymap->canonical_state_mask;
        darray_foreach(mapping, *raw_mappings) {
            अगर (!mapping->source || !mapping->target ||
                (mapping->source & invalid) || (mapping->target & invalid))
                जारी;
            darray_append(mappings, *mapping);
            mask |= mapping->source;
        }

        /*
         * Re-order the mappings, so that combinations have higher priority
         * if they are superset of other combinations.
         * For other type of overlaps, see the sorting function.
         */
        क्विक(darray_वस्तुएँ(mappings), darray_आकार(mappings),
              माप(*darray_वस्तुएँ(mappings)), &cmp_mod_masks);

        darray_steal(
            mappings,
            &sm->config.modifiers.mappings,
            &sm->config.modifiers.mappings_अंक
        );
        sm->config.modifiers.mask = mask;
    } अन्यथा {
        sm->config.modifiers = (काष्ठा machine_modifiers_config) {
            .mappings = शून्य,
            .mappings_अंक = 0,
            .mask = 0
        };
    }

    वापस XKB_SUCCESS;
}

अटल क्रमागत xkb_status
machine_set_shortcuts(
    काष्ठा xkb_machine * restrict sm,
    स्थिर काष्ठा xkb_shortcuts_config_options * restrict options
)
{
    अगर (darray_empty(options->entries)) {
        sm->config.shortcuts = (काष्ठा machine_shortcuts_config) {
            .entries = शून्य
        };
        वापस XKB_SUCCESS;
    }

    काष्ठा xkb_keymap * स्थिर keymap = sm->base.base.keymap;

    काष्ठा xkb_shortcuts_config_entry * entries =
        सुस्मृति(keymap->अंक_groups, माप(*entries));
    अगर (!entries)
        वापस XKB_ERROR_ALLOCATION_FAILURE;

    स्थिर xkb_layout_सूचकांक_t गिनती = MIN(
        keymap->अंक_groups,
        (xkb_layout_सूचकांक_t) darray_आकार(options->entries)
    );

    bool some_entries = false;
    क्रम (xkb_layout_सूचकांक_t l = 0; l < गिनती; l++) {
        /* Sanitize layout target */
        स्थिर xkb_layout_सूचकांक_t target =
            darray_item(options->entries, l).target;
        /* Sanitize modifier mask */
        स्थिर xkb_mod_mask_t mods = (
            darray_item(options->entries, l).mods &
            sm->base.base.keymap->canonical_state_mask
        );
        अगर (target < keymap->अंक_groups && target != l && mods) {
            entries[l] = (काष्ठा xkb_shortcuts_config_entry) {
                .target = target,
                .mods = mods
            };
            some_entries = true;
        } अन्यथा {
            entries[l] = (काष्ठा xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
            जारी;
        }
    }

    अगर (!some_entries) {
        मुक्त(entries);
        entries = शून्य;
    } अन्यथा {
        /* Initialize groups with no entries */
        क्रम (xkb_layout_सूचकांक_t l = गिनती; l < keymap->अंक_groups; l++) {
            entries[l] = (काष्ठा xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
        }
    }

    sm->config.shortcuts = (काष्ठा machine_shortcuts_config) {
        .entries = entries
    };
    वापस XKB_SUCCESS;
}

काष्ठा xkb_machine *
xkb_machine_new(स्थिर काष्ठा xkb_machine_builder * restrict builder,
                क्रमागत xkb_status * restrict status)
{
    काष्ठा xkb_machine * स्थिर machine = सुस्मृति(1, माप(*machine));
    अगर (!machine) {
        log_err_func1(builder->keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine\n");
        अगर (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        वापस शून्य;
    }

    xkb_server_state_init(&machine->base, builder->keymap, SERVER_STATE,
                          builder->controls.a11y.affect_flags,
                          builder->controls.a11y.flags);
    machine->flags = builder->machine_flags;
    अगर (!(machine->flags &
          XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_कुंजी_FIX))
    {
        machine->base.flags |= STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_कुंजी;
    }
    अगर (machine->flags & XKB_MACHINE_UNLOCK_MODS_ON_PRESS) {
        machine->base.flags |= STATE_UNLOCK_MODS_ON_PRESS;
    }

    क्रमागत xkb_status status_;
    अगर ((status_ = machine_set_mods(machine, &builder->mods)) != XKB_SUCCESS ||
        (status_ = machine_set_shortcuts(machine, &builder->shortcuts)) !=
        XKB_SUCCESS) {
        *status = status_;
        जाओ error;
    }

    darray_init(machine->overlays.कुंजियाँ);
    machine->mouse.default_button = XKB_POINTER_BUTTON_MIN;

    अगर (status)
        *status = XKB_SUCCESS;

    वापस machine;

error:
    xkb_machine_unref(machine);
    वापस शून्य;
}

काष्ठा xkb_machine *
xkb_machine_ref(काष्ठा xkb_machine *sm)
{
    निश्चित(sm->base.base.refcnt > 0);
    sm->base.base.refcnt++;
    वापस sm;
}

व्योम
xkb_machine_unref(काष्ठा xkb_machine *sm)
{
    निश्चित(!sm || sm->base.base.refcnt > 0);
    अगर (!sm || --sm->base.base.refcnt > 0)
        वापस;

    xkb_state_destroy(&sm->base.base);
    darray_free(sm->overlays.कुंजियाँ);
    मुक्त(sm->config.shortcuts.entries);
    मुक्त(sm->config.modifiers.mappings);
    मुक्त(sm);
}

काष्ठा xkb_keymap *
xkb_machine_get_keymap(स्थिर काष्ठा xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    वापस sm->base.base.keymap;
}

काष्ठा xkb_state *
xkb_machine_get_state(काष्ठा xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    वापस (काष्ठा xkb_state *)sm;
}

काष्ठा xkb_state *
xkb_state_new_from_machine(स्थिर काष्ठा xkb_machine * restrict machine,
                           क्रमागत xkb_status * restrict status)
{
    /* Creation */
    क्रमागत xkb_status status_;
    काष्ठा xkb_state * स्थिर state = xkb_state_new_with_mode(
        machine->base.base.keymap,
        XKB_STATE_MODE_SERVER_QUERY,
        &status_
    );
    अगर (status_ != XKB_SUCCESS) {
        अगर (status)
            *status = status_;
        वापस शून्य;
    }

    निश्चित(state->mode == SERVER_COMPANION);
    निश्चित(state->refcnt == 1);

    /* Copy machine’s state */
    *state = machine->base.base;

    /* Restore previous values */
    state->mode = SERVER_COMPANION;
    state->refcnt = 1;

    अगर (status)
        *status = XKB_SUCCESS;
    वापस state;
}

अटल व्योम
machine_update_overlays(काष्ठा xkb_machine *sm)
{
    /*
     * Overlays indices are stored 1-indexed in nibbles: the lowest
     * nibble corresponds to the latest activated index.
     */

    स्थिर xkb_overlay_mask_t mask =
        overlays_from_controls(sm->base.base.components.controls);
    xkb_overlay_mask_t added = mask & ~sm->overlays.enabled;

    /* Remove overlays no longer enabled and keep relative order */
    uint32_t order = sm->overlays.order;
    स्थिर xkb_overlay_सूचकांक_t overlay_max =
        format_max_overlays(sm->base.base.keymap->format);
    क्रम (uint8_t n = 0; n < overlay_max;) {
        xkb_overlay_सूचकांक_t overlay_idx = (order >> (n * 4)) & 0xf;
        अगर (!overlay_idx)
            अवरोध;
        स्थिर xkb_overlay_mask_t overlay_mask =
            (xkb_overlay_mask_t)(1u << (overlay_idx - 1u) /* k is 1-indexed */);
        अगर (overlay_mask & mask) {
            /* no duplicates */
            added &= ~overlay_mask;
            n++;
        } अन्यथा {
            /* remove overlay */
            स्थिर uint32_t शीर्ष = (UINT32_C(1) << (n * 4)) - 1;
            order = (order & शीर्ष) | ((order >> 4) & ~शीर्ष);
        }
    }

    /* Add new overlays in ascending order */
    क्रम (xkb_overlay_सूचकांक_t k = 0; added; k++, added >>= 1) {
        अगर (added & 0x1) {
            order <<= 4;
            order |= k + 1u; /* k is 0-indexed */
        }
    }

    sm->overlays.order = order;
    sm->overlays.enabled = mask;
}

क्रमागत xkb_status
xkb_machine_process_synthetic(काष्ठा xkb_machine *sm,
                              स्थिर काष्ठा xkb_synthetic_update *update,
                              काष्ठा xkb_events *events)
{
    /* Check ABI compatibility */
    क्रमागत xkb_status status =
        check_state_update_abi(sm->base.base.keymap->ctx, update);
    अगर (status != XKB_SUCCESS)
        वापस status;

    काष्ठा xkb_server_state * स्थिर state = &sm->base;
    स्थिर काष्ठा state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    darray_आकार(events->queue) = 0;
    events->अग्र = 0;

    /* Update parametrized controls first */
    अगर (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        अगर (status != XKB_SUCCESS)
            वापस status;
    }

    अगर (update->components) {
        स्थिर काष्ठा xkb_state_components_update * स्थिर components =
            update->components;
        /* Update boolean controls first */
        अगर (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, events);
        }

        state_update_latched_locked(state, components, events);
    }

    xkb_state_update_derived(&state->base);

    स्थिर क्रमागत xkb_state_component changed = get_state_component_changes(
        &previous_components, &state->base.components
    );
    अगर (changed) {
        // TODO: latch controls
        अगर (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        /* Create event only if some component actually changed */
        darray_append(events->queue, (काष्ठा xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .changed = changed,
                .components = state->base.components
            }
        });

        /* Terminate the frame */
        darray_append(events->queue, (काष्ठा xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    वापस XKB_SUCCESS;
}

अटल ssize_t
do_remap_modifiers(स्थिर काष्ठा machine_modifiers_config * restrict mappings,
                   काष्ठा xkb_state * restrict state,
                   काष्ठा xkb_events * restrict events,
                   स्थिर काष्ठा xkb_कुंजी * restrict कुंजी)
{
    अगर (!(mappings->mask & state->components.mods))
        वापस -1;

    स्थिर xkb_layout_सूचकांक_t layout = state_कुंजी_get_layout(state, कुंजी);
    अगर (layout >= कुंजी->अंक_groups)
        वापस -1;

    स्थिर काष्ठा xkb_कुंजी_type * स्थिर type = कुंजी->groups[layout].type;
    xkb_mod_mask_t affect_mods = 0;
    xkb_mod_mask_t mods = 0;
    क्रम (darray_आकार_t m = 0; m < mappings->mappings_अंक; m++) {
        स्थिर काष्ठा machine_mods_mapping * स्थिर mapping =
            &mappings->mappings[m];
        अगर (/* Skip not matching active mods */
            (mapping->source & state->components.mods) == mapping->source &&
            /* Skip overlap with previous active mappings */
            !(mapping->source & affect_mods) &&
            /* Skip if the key type uses some of the source modifiers */
            !(mapping->source & type->mods.mask)) {
            affect_mods |= mappings->mappings[m].source;
            mods |= mappings->mappings[m].target;
        }
    }

    अगर (!affect_mods)
        वापस -1;

    काष्ठा xkb_state new = *state;
    new.components.base_mods = (new.components.base_mods & ~affect_mods) | mods;
    new.components.latched_mods = (new.components.latched_mods & ~affect_mods);
    new.components.locked_mods = (new.components.locked_mods & ~affect_mods);
    xkb_state_update_derived(&new);

    ssize_t event_idx = -1;
    स्थिर क्रमागत xkb_state_component changed =
        get_state_component_changes(&state->components, &new.components);
    अगर (changed) {
        event_idx = (ssize_t) darray_आकार(events->queue);
        darray_append(events->queue, (काष्ठा xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = new.components,
                .changed = changed
            }
        });
    }

    state->components.mods = new.components.mods;
    वापस event_idx;
}

/**
 * Activate an alternative layout if some modifiers are active, typically
 * `Control`, `Alt` or `Super`.
 *
 * The tweak consists of computing a *base* layout such that the *effective*
 * layout is the corresponding target layout.
 */
अटल ssize_t
do_shortcuts_tweak(स्थिर काष्ठा machine_shortcuts_config *config,
                   काष्ठा xkb_state *state,
                   स्थिर काष्ठा state_components *previous_components,
                   काष्ठा xkb_events *events, ssize_t remap_event)
{
    अगर (!config->entries) {
        /* No shortcuts tweak */
        वापस remap_event;
    }

    स्थिर काष्ठा xkb_shortcuts_config_entry * स्थिर entry =
        &config->entries[state->components.group];

    अगर (entry->target != XKB_LAYOUT_INVALID &&
        (entry->mods & state->components.mods)) {
        /*
         * Activate shortcuts tweak:
         * 1. The real base group is saved by the caller, to be restored by
         *    later by a call to undo_tweaks().
         * 2. Compute the new base group so that the resulting effective group
         *    is the target.
         * 3. Recompute derived state components.
         */
        काष्ठा xkb_state new = *state;
        अगर (remap_event < 0) {
            /* Create new event */
            remap_event = (ssize_t) darray_आकार(events->queue);
            darray_append(events->queue, (काष्ठा xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = {0},
                    .changed = 0
                }
            });
        } अन्यथा {
            /* Merge with last remap event */
            new.components =
                darray_item(events->queue, remap_event).components.components;
        }
        new.components.base_group
            = (int32_t) entry->target
            - state->components.latched_group
            - state->components.locked_group;
        xkb_state_update_derived(&new);

        स्थिर क्रमागत xkb_state_component changed =
            get_state_component_changes(previous_components, &new.components);
        darray_item(events->queue, remap_event).components.changed = changed;
        darray_item(events->queue, remap_event).components.components =
            new.components;

        state->components.group = new.components.group;
    } अन्यथा {
        /* Deactivated entry or modifiers do not match */
    }
    वापस remap_event;
}

अटल व्योम
undo_tweaks(स्थिर काष्ठा xkb_state *state,
            स्थिर काष्ठा state_components *initial_components,
            काष्ठा xkb_events *events)
{
    /* Get last component event */
    स्थिर काष्ठा xkb_event *event = शून्य;
    darray_foreach_reverse(event, events->queue) {
        अगर (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS)
            अवरोध;
    }
    अगर (!event)
        वापस;

    /* Restore state */
    स्थिर क्रमागत xkb_state_component changed =
        get_state_component_changes(initial_components,
                                    &event->components.components);
    अगर (changed) {
        darray_append(events->queue, (काष्ठा xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = *initial_components,
                .changed = changed
            }
        });
    }
}

अटल स्थिर काष्ठा xkb_कुंजी *
process_overlayable_कुंजी(काष्ठा xkb_machine *sm,
                        स्थिर काष्ठा xkb_कुंजी * कुंजी,
                        क्रमागत xkb_कुंजी_direction direction)
{
    /* Check if key is currently overlaid */
    काष्ठा xkb_overlaid_कुंजी *entry = शून्य;
    काष्ठा xkb_overlaid_कुंजी *available_entry = शून्य;
    darray_foreach(entry, sm->overlays.कुंजियाँ) {
        अगर (entry->old == कुंजी) {
            /* Maintain the key overlaid */
            चयन (direction) {
            हाल XKB_कुंजी_DOWN:
                entry->refcnt++;
                अवरोध;
            हाल XKB_कुंजी_REPEATED:
                अवरोध;
            हाल XKB_कुंजी_UP:
            शेष:
                entry->refcnt--;
                अवरोध;
            }
            अगर (entry->refcnt <= 0)
                entry->old = शून्य;
            वापस entry->new;
        } अन्यथा अगर (!entry->old && !available_entry) {
            /* First free spot */
            available_entry = entry;
        }
    }
    अगर (direction == XKB_कुंजी_DOWN) {
        /* Enable key overlay */

        /*
         * Get overlay index: select the latest activated overlay
         * containing the key.
         */
        स्थिर काष्ठा xkb_कुंजी *new = कुंजी;

        अगर (कुंजी->overlays & sm->overlays.enabled) {
            /* Some relevant overlay is active */
            क्रम (uint32_t ढेर = sm->overlays.order; ढेर; ढेर >>= 4) {
                static_assert(XKB_OVERLAY_गिनती == 8, "");
                स्थिर xkb_overlay_सूचकांक_t overlay = (ढेर & 0xf) - 1;
                स्थिर xkb_overlay_mask_t mask =
                    (xkb_overlay_mask_t)(1u << overlay);
                अगर (कुंजी->overlays & mask) {
                    /* Get new key */
                    अगर (कुंजी->overlays_inline) {
                        /* Stored inline */
                        new = कुंजी->overlay_कुंजी;
                    } अन्यथा {
                        /* Stored in sparse array */
                        स्थिर xkb_overlay_mask_t निचला = (xkb_overlay_mask_t)(
                            कुंजी->overlays & (xkb_overlay_mask_t)(mask - 1u)
                        );
                        /* index = number of set bits strictly below bit */
                        स्थिर xkb_overlay_सूचकांक_t सूचकांक =
                            (xkb_overlay_सूचकांक_t)popcount32(निचला);
                        new = कुंजी->overlays_कुंजियाँ[सूचकांक];
                    }
                    अवरोध;
                }
            }
        } अन्यथा {
            /*
             * No relevant overlay is active: block future overlays while
             * the key is pressed.
             */
        }

        /* Get new entry */
        अगर (available_entry) {
            /* Re-use a free entry */
            entry = available_entry;
        } अन्यथा {
            /* Append a new entry */
            स्थिर darray_आकार_t idx = darray_आकार(sm->overlays.कुंजियाँ);
            darray_resize(sm->overlays.कुंजियाँ, idx + 1);
            entry = &darray_item(sm->overlays.कुंजियाँ, idx);
        }

        /* Overlay the key */
        entry->old = कुंजी;
        entry->new = new;
        entry->refcnt = 1;
        वापस new;
    }

    /* No overlay */
    वापस कुंजी;
}

क्रमागत xkb_status
xkb_machine_process_कुंजी(काष्ठा xkb_machine *sm,
                        xkb_keycode_t kc, क्रमागत xkb_कुंजी_direction direction,
                        काष्ठा xkb_events *events)
{
    darray_आकार(events->queue) = 0;
    events->अग्र = 0;

    काष्ठा xkb_server_state * स्थिर state = &sm->base;
    स्थिर काष्ठा xkb_कुंजी * कुंजी = Xkb_कुंजी(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    अगर (!कुंजी || (direction == XKB_कुंजी_REPEATED && !कुंजी->repeats))
        वापस XKB_SUCCESS;

    स्थिर काष्ठा state_components initial_components = state->base.components;

    अगर (direction == XKB_कुंजी_DOWN &&
        (sm->base.base.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_कुंजियाँ) &&
        (sm->base.a11y_flags & XKB_A11_Y_STICKY_कुंजियाँ_NO_SIMULTANEOUS_कुंजियाँ) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_कुंजियाँ,
                                      0, events);
    }

    अगर (कुंजी->overlays)
        कुंजी = process_overlayable_कुंजी(sm, कुंजी, direction);

    ssize_t remap_event = do_remap_modifiers(&sm->config.modifiers,
                                             &state->base, events, कुंजी);

    remap_event = do_shortcuts_tweak(&sm->config.shortcuts, &state->base,
                                     &initial_components, events, remap_event);

    अगर (remap_event >= 0) {
        // TODO: what about merging state event for RedirectKey?
        darray_append(events->queue, (काष्ठा xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    state->flags |= STATE_REQUIRE_कुंजी_EVENT;
    state->set_mods = 0;
    state->clear_mods = 0;

    /* Handle key actions */
    xkb_filter_apply_all(state, events, कुंजी, direction);

    xkb_mod_सूचकांक_t i;
    xkb_mod_mask_t bit;
    क्रम (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        अगर (state->set_mods & bit) {
            state->mod_कुंजी_गिनती[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    क्रम (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        अगर (state->clear_mods & bit) {
            state->mod_कुंजी_गिनती[i]--;
            अगर (state->mod_कुंजी_गिनती[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_कुंजी_गिनती[i] = 0;
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
    अगर (state->flags & STATE_REQUIRE_कुंजी_EVENT) {
        darray_append(events->queue, (काष्ठा xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_कुंजी,
            .कुंजी = {
                .keycode = कुंजी->keycode,
                .direction = direction
            }
        });
    }

    अगर (remap_event >= 0) {
        // FIXME: fragile if last state change does not restore the state to the remap event
        undo_tweaks(&state->base, &initial_components, events);
    }

    स्थिर क्रमागत xkb_state_component changed = get_state_component_changes(
        &initial_components, &state->base.components
    );
    अगर (changed) {
        अगर (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        // TODO: merge with previous state event, if any

        darray_append(events->queue, (काष्ठा xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = state->base.components,
                .changed = changed
            }
        });
    }

    अगर (!darray_empty(events->queue) &&
        darray_item(events->queue, darray_आकार(events->queue) - 1).type !=
        XKB_EVENT_TYPE_FRAME)
    {
        darray_append(events->queue, (काष्ठा xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    वापस XKB_SUCCESS;
}

काष्ठा xkb_events *
xkb_events_new(काष्ठा xkb_context * restrict context,
               स्थिर काष्ठा xkb_events_config * restrict config,
               क्रमागत xkb_status * restrict status)
{
    /* Handle default configuration */
    अटल स्थिर काष्ठा xkb_events_config default_config = {
        .आकार = माप(default_config)
    };
    अगर (!config)
        config = &default_config;

    /* Check ABI compatibility */
    स्थिर क्रमागत xkb_status abi_error = xkb_check_state_abi(config);
    अगर (abi_error) {
        xkb_log_abi_error(context, __func__, abi_error);
        अगर (status)
            *status = abi_error;
        वापस शून्य;
    }

    /* Sanitize input */
    स्थिर uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_EVENTS_FLAGS_मूल्यावली);
    अगर (invalid_flags) {
        log_err_func(context, XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS_,
                     "unrecognized events collection flags: 0x%"PRIx32"\n",
                     invalid_flags);
        अगर (status)
            *status = XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS;
        वापस शून्य;
    }

    काष्ठा xkb_events *events = सुस्मृति(1, माप(*events));
    अगर (!events) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events collection\n");
        अगर (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        वापस शून्य;
    }

    /* Pre-allocate event queue */
    darray_init(events->queue);
    अटल स्थिर darray_आकार_t queue_initial_आकार = 16;
    darray_resize(events->queue, queue_initial_आकार);
    // TODO: proper safe darray_resize() with return value
    अगर (!darray_वस्तुएँ(events->queue)) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events queue\n");
        xkb_events_destroy(events);
        अगर (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        वापस शून्य;
    }
    darray_आकार(events->queue) = 0;

    events->ctx = xkb_context_ref(context);
    events->अग्र = 0;

    अगर (status)
        *status = XKB_SUCCESS;
    वापस events;
}

व्योम
xkb_events_destroy(काष्ठा xkb_events *events)
{
    अगर (!events)
        वापस;
    darray_free(events->queue);
    xkb_context_unref(events->ctx);
    मुक्त(events);
}

स्थिर काष्ठा xkb_event *
xkb_events_अग्र(काष्ठा xkb_events *events)
{
    अगर (events->अग्र < darray_आकार(events->queue)) {
        स्थिर darray_आकार_t सूचकांक = events->अग्र++;
        वापस &darray_item(events->queue, सूचकांक);
    } अन्यथा {
        वापस शून्य;
    }
}

क्रमागत xkb_event_type
xkb_event_get_type(स्थिर काष्ठा xkb_event *event)
{
    वापस event->type;
}

क्रमागत xkb_status
xkb_event_get_keycode(स्थिर काष्ठा xkb_event *event,
                      xkb_keycode_t *keycode,
                      क्रमागत xkb_कुंजी_direction *direction)
{
    अगर (event->type == XKB_EVENT_TYPE_कुंजी) {
        *keycode = event->कुंजी.keycode;
        *direction = event->कुंजी.direction;
        वापस XKB_SUCCESS;
    } अन्यथा {
        वापस XKB_ERROR_EVENT_TYPE_MISMATCH;
    }
}

क्रमागत xkb_status
xkb_event_get_components(स्थिर काष्ठा xkb_event * restrict event,
                         काष्ठा xkb_event_components * restrict components)
{
    अगर (event->type != XKB_EVENT_TYPE_STATE_COMPONENTS)
        वापस XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    स्थिर क्रमागत xkb_status status = xkb_check_state_abi(components);
    अगर (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        वापस status;
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

    वापस XKB_SUCCESS;
}

क्रमागत xkb_status
xkb_event_get_pointer_motion(स्थिर काष्ठा xkb_event * restrict event,
                             काष्ठा xkb_event_pointer_motion * restrict motion)
{
    अगर (event->type != XKB_EVENT_TYPE_POINTER_MOTION)
        वापस XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    स्थिर क्रमागत xkb_status status = xkb_check_state_abi(motion);
    अगर (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        वापस status;
    }

    motion->flags = event->pointer_motion.flags;
    motion->x = event->pointer_motion.x;
    motion->y = event->pointer_motion.y;

    वापस XKB_SUCCESS;
}

क्रमागत xkb_status
xkb_event_get_pointer_button(स्थिर काष्ठा xkb_event * restrict event,
                             काष्ठा xkb_event_pointer_button * restrict button)
{
    अगर (event->type != XKB_EVENT_TYPE_POINTER_BUTTON)
        वापस XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    स्थिर क्रमागत xkb_status status = xkb_check_state_abi(button);
    अगर (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        वापस status;
    }

    button->button = event->pointer_button.button;
    button->state = event->pointer_button.state;
    button->गिनती = event->pointer_button.गिनती;

    वापस XKB_SUCCESS;
}

क्रमागत xkb_status
xkb_event_get_virtual_console(स्थिर काष्ठा xkb_event * restrict event,
                              int8_t * restrict सूचकांक_or_offset,
                              bool * restrict is_offset)
{
    अगर (event->type != XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
        वापस XKB_ERROR_EVENT_TYPE_MISMATCH;

    *सूचकांक_or_offset = event->virtual_console.सूचकांक_or_offset;
    *is_offset = event->virtual_console.is_offset;

    वापस XKB_SUCCESS;
}

क्रमागत xkb_status
xkb_state_update_event(काष्ठा xkb_state * restrict base_state,
                       स्थिर काष्ठा xkb_event * restrict event,
                       क्रमागत xkb_state_component * restrict changed)
{
    /* Guard against server-only state */
    निश्चित(base_state->mode == SERVER_COMPANION ||
           base_state->mode == LEGACY_MIXED_STATE);
    अगर (base_state->mode != SERVER_COMPANION &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE_,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        वापस XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    काष्ठा xkb_client_state * स्थिर state =
        (काष्ठा xkb_client_state *)base_state;

    अगर (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
        स्थिर काष्ठा state_components पूर्व_components = state->base.components;
        state->base.components = event->components.components;
        /*
         * Recompute the changes instead of using the event value, because we do
         * not know if the event’s queue and the state are synced.
         */
        अगर (changed)
            *changed = get_state_component_changes(&पूर्व_components,
                                                   &state->base.components);
    } अन्यथा {
        अगर (changed)
            *changed = 0;
    }
    वापस XKB_SUCCESS;
}

अटल व्योम
append_pointer_button(काष्ठा xkb_events * restrict events,
                      काष्ठा xkb_filter * restrict filter,
                      क्रमागत xkb_pointer_button_state button_state,
                      uint8_t गिनती)
{
    darray_append(events->queue, (काष्ठा xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_BUTTON,
        .pointer_button = {
            .आकार = माप(((काष्ठा xkb_event*)0)->pointer_button),
            .button = filter->priv,
            .state = button_state,
            .गिनती = गिनती,
        }
    });
}

अटल व्योम
xkb_filter_pointer_button_new(काष्ठा xkb_server_state *state,
                              काष्ठा xkb_events *events,
                              काष्ठा xkb_filter *filter)
{
    अगर (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_कुंजियाँ)) {
        filter->func = शून्य;
        वापस;
    }

    काष्ठा xkb_machine * स्थिर sm = (काष्ठा xkb_machine *)state;

    /* Resolve button */
    स्थिर xkb_pointer_button_सूचकांक_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;
    filter->priv = button;

    स्थिर क्रमागत xkb_pointer_button_state button_state =
        XKB_POINTER_BUTTON_PRESSED;
    append_pointer_button(events, filter, button_state,
                          filter->action.btn.गिनती);
    state->flags &= ~STATE_REQUIRE_कुंजी_EVENT;
}

अटल bool
xkb_filter_pointer_button_func(काष्ठा xkb_server_state *state,
                               काष्ठा xkb_events *events,
                               काष्ठा xkb_filter *filter,
                               स्थिर काष्ठा xkb_कुंजी *कुंजी,
                               क्रमागत xkb_कुंजी_direction direction)
{
    अगर (कुंजी != filter->कुंजी || !events)
        वापस XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_कुंजी_EVENT;

    चयन (direction) {
    हाल XKB_कुंजी_DOWN:
        filter->refcnt++;
        /* fallthrough */
    हाल XKB_कुंजी_REPEATED:
        वापस XKB_FILTER_CONSUME;
    शेष:
        अगर (--filter->refcnt > 0)
            वापस XKB_FILTER_CONSUME;
    }

    अगर (!filter->action.btn.गिनती)
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);

    filter->func = शून्य;
    वापस XKB_FILTER_CONSUME;
}

काष्ठा xkb_filter_pointer_lock_button_priv {
    uint32_t button:31;
#अगर_घोषित _WIN32
    uint32_t locked:1; // MSVC requires matching types to pack bitfields
#अन्यथा
    bool locked:1;
#पूर्ण_अगर
};

क्रमागत {
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH =
        माप(काष्ठा xkb_filter_pointer_lock_button_priv) * अक्षर_बिट,
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX =
        (UINT64_C(1) << XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH) - 1
};

static_assert(माप(काष्ठा xkb_filter_pointer_lock_button_priv) ==
              माप(((काष्ठा xkb_filter*)0)->priv),
              "");
static_assert((uint32_t)XKB_POINTER_BUTTON_MAX < (uint32_t)XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX,
              "xkb_filter_pointer_lock_button_priv cannot store button");

अटल व्योम
xkb_filter_pointer_lock_button_new(काष्ठा xkb_server_state *state,
                                   काष्ठा xkb_events *events,
                                   काष्ठा xkb_filter *filter)
{
    अगर (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_कुंजियाँ)) {
        filter->func = शून्य;
        वापस;
    }

    state->flags &= ~STATE_REQUIRE_कुंजी_EVENT;

    काष्ठा xkb_machine * स्थिर sm = (काष्ठा xkb_machine *)state;

    /* Resolve button */
    स्थिर xkb_pointer_button_सूचकांक_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;

    static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                  "invalid UINTx_C() macro");
    static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                  "invalid left shift");
    स्थिर xkb_pointer_button_mask_t mask = (UINT8_C(1) << button);

    स्थिर bool locked = (sm->mouse.locked_buttons & mask);

    अगर (!locked && !(filter->action.btn.flags & ACTION_LOCK_NO_LOCK)) {
        sm->mouse.locked_buttons |= mask;
        filter->priv = button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_PRESSED, 0);
    }

    काष्ठा xkb_filter_pointer_lock_button_priv *priv =
        (काष्ठा xkb_filter_pointer_lock_button_priv*)&filter->priv;
    priv->button = button,
    priv->locked = locked;
}

अटल bool
xkb_filter_pointer_lock_button_func(काष्ठा xkb_server_state *state,
                                    काष्ठा xkb_events *events,
                                    काष्ठा xkb_filter *filter,
                                    स्थिर काष्ठा xkb_कुंजी *कुंजी,
                                    क्रमागत xkb_कुंजी_direction direction)
{
    अगर (कुंजी != filter->कुंजी || !events || state->base.mode != SERVER_STATE)
        वापस XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_कुंजी_EVENT;

    चयन (direction) {
    हाल XKB_कुंजी_DOWN:
        filter->refcnt++;
        /* fallthrough */
    हाल XKB_कुंजी_REPEATED:
        वापस XKB_FILTER_CONSUME;
    शेष:
        अगर (--filter->refcnt > 0)
            वापस XKB_FILTER_CONSUME;
    }

    स्थिर काष्ठा xkb_filter_pointer_lock_button_priv *priv =
        (काष्ठा xkb_filter_pointer_lock_button_priv*)&filter->priv;

    अगर (priv->locked && !(filter->action.btn.flags & ACTION_LOCK_NO_UNLOCK)) {
        काष्ठा xkb_machine * स्थिर sm = (काष्ठा xkb_machine *)state;

        static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                    "invalid UINTx_C() macro");
        static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                    "invalid left shift");
        स्थिर xkb_pointer_button_mask_t mask = (UINT8_C(1) << priv->button);

        sm->mouse.locked_buttons &= ~mask;
        filter->priv = priv->button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);
    }

    filter->func = शून्य;
    वापस XKB_FILTER_CONSUME;
}

अटल व्योम
xkb_filter_pointer_set_default_button_new(काष्ठा xkb_server_state *state,
                                          काष्ठा xkb_events *events,
                                          काष्ठा xkb_filter *filter)
{
    अगर (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_कुंजियाँ)) {
        filter->func = शून्य;
        वापस;
    }

    state->flags &= ~STATE_REQUIRE_कुंजी_EVENT;

    काष्ठा xkb_machine * स्थिर sm = (काष्ठा xkb_machine *)state;

    static_assert(2 * XKB_POINTER_BUTTON_MAX <= पूर्णांक_उच्च, "lossy conversion");
    पूर्णांक button = (filter->action.dflt.flags & ACTION_ABSOLUTE_SWITCH)
        ? filter->action.dflt.मूल्य
        : sm->mouse.default_button + filter->action.dflt.मूल्य;

    /* Wrap into range */
    स्थिर पूर्णांक rem = button % XKB_POINTER_BUTTON_MAX;
    sm->mouse.default_button = (rem >= 0)
        ? (xkb_pointer_button_सूचकांक_t)rem
        : (xkb_pointer_button_सूचकांक_t)(rem + XKB_POINTER_BUTTON_MAX);
}

अटल bool
xkb_filter_pointer_set_default_button_func(काष्ठा xkb_server_state *state,
                                           काष्ठा xkb_events *events,
                                           काष्ठा xkb_filter *filter,
                                           स्थिर काष्ठा xkb_कुंजी *कुंजी,
                                           क्रमागत xkb_कुंजी_direction direction)
{
    अगर (कुंजी != filter->कुंजी || !events)
        वापस XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_कुंजी_EVENT;

    चयन (direction) {
    हाल XKB_कुंजी_DOWN:
        filter->refcnt++;
        /* fallthrough */
    हाल XKB_कुंजी_REPEATED:
        वापस XKB_FILTER_CONSUME;
    शेष:
        अगर (--filter->refcnt > 0)
            वापस XKB_FILTER_CONSUME;
    }

    filter->func = शून्य;
    वापस XKB_FILTER_CONTINUE;
}

अटल व्योम
xkb_filter_terminate_server_new(काष्ठा xkb_server_state *state,
                                 काष्ठा xkb_events *events,
                                 काष्ठा xkb_filter *filter)
{
    अगर (!events || state->base.mode != SERVER_STATE ||
        !(((काष्ठा xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = शून्य;
        वापस;
    }

    state->flags &= ~STATE_REQUIRE_कुंजी_EVENT;

    darray_append(events->queue, (काष्ठा xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    });
}

अटल bool
xkb_filter_terminate_server_func(काष्ठा xkb_server_state *state,
                                 काष्ठा xkb_events *events,
                                 काष्ठा xkb_filter *filter,
                                 स्थिर काष्ठा xkb_कुंजी *कुंजी,
                                 क्रमागत xkb_कुंजी_direction direction)
{
    अगर (कुंजी != filter->कुंजी || !events)
        वापस XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_कुंजी_EVENT;

    चयन (direction) {
    हाल XKB_कुंजी_DOWN:
        filter->refcnt++;
        /* fallthrough */
    हाल XKB_कुंजी_REPEATED:
        वापस XKB_FILTER_CONSUME;
    शेष:
        अगर (--filter->refcnt > 0)
            वापस XKB_FILTER_CONSUME;
    }

    filter->func = शून्य;
    वापस XKB_FILTER_CONSUME;
}

अटल व्योम
xkb_filter_switch_virtual_console_new(काष्ठा xkb_server_state *state,
                                      काष्ठा xkb_events *events,
                                      काष्ठा xkb_filter *filter)
{
    अगर (!events || state->base.mode != SERVER_STATE ||
        !(((काष्ठा xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = शून्य;
        वापस;
    }

    state->flags &= ~STATE_REQUIRE_कुंजी_EVENT;

    darray_append(events->queue, (काष्ठा xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
        .virtual_console = {
            .सूचकांक_or_offset = filter->action.screen.screen,
            .is_offset = !(filter->action.screen.flags & ACTION_ABSOLUTE_SWITCH)
        }
    });
}

अटल bool
xkb_filter_switch_virtual_console_func(काष्ठा xkb_server_state *state,
                                       काष्ठा xkb_events *events,
                                       काष्ठा xkb_filter *filter,
                                       स्थिर काष्ठा xkb_कुंजी *कुंजी,
                                       क्रमागत xkb_कुंजी_direction direction)
{
    अगर (कुंजी != filter->कुंजी || !events)
        वापस XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_कुंजी_EVENT;

    चयन (direction) {
    हाल XKB_कुंजी_DOWN:
        filter->refcnt++;
        /* fallthrough */
    हाल XKB_कुंजी_REPEATED:
        वापस XKB_FILTER_CONSUME;
    शेष:
        अगर (--filter->refcnt > 0)
            वापस XKB_FILTER_CONSUME;
    }

    filter->func = शून्य;
    वापस XKB_FILTER_CONSUME;
}
