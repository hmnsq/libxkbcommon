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

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <limits.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "xkbcommon/xkbcommon-status.h"
#अन्तर्भाव "xkbcommon/xkbcommon-features.h"
#अन्तर्भाव "abi-check.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "features/enums.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "keysym.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "state-priv.h"
#अन्तर्भाव "utf8.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "util-mem.h"
#अन्तर्भाव "util-numbers.h"

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
संरचना xkb_events {
    संरचना xkb_context *ctx;
    darray(संरचना xkb_event) queue;
    /**
     * Read cursor for `xkb_events_next()`.
     *
     * Reset to 0 on each `process_*` call.
     */
    darray_size_t next;
};

संरचना xkb_server_state;

संरचना xkb_filter {
    सङ्घ xkb_action action;
    ध्रुव संरचना xkb_किल्ली *किल्ली;
    bool (*func)(संरचना xkb_server_state *state,
                 संरचना xkb_events *events,
                 संरचना xkb_filter *filter,
                 ध्रुव संरचना xkb_किल्ली *किल्ली,
                 गणना xkb_किल्ली_direction direction);
    uint32_t priv;
    पूर्णाङ्क refcnt;
};

गणना xkb_state_mode_internal {
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

गणना { XKB_LAYOUT_INDEX_T_MIN_WIDTH = 6 };
static_assert(XKB_MAX_GROUPS < (1 << XKB_LAYOUT_INDEX_T_MIN_WIDTH),
              "Cannot encode out_of_range_group::redirect_group");

संरचना out_of_range_group {
    गणना xkb_layout_out_of_range_policy policy :
        (परिमाण(गणना xkb_layout_out_of_range_policy) * CHAR_BIT -
         XKB_LAYOUT_INDEX_T_MIN_WIDTH);
    xkb_layout_index_t redirect_group : XKB_LAYOUT_INDEX_T_MIN_WIDTH;
};

गणना { XKB_STATE_MODE_INTERNAL_MIN_WIDTH = 4 /* 3 bits + sign */ };
static_assert(XKB_STATE_TYPE_अंक_ENTRIES <
              (1 << XKB_STATE_MODE_INTERNAL_MIN_WIDTH),
              "Cannot encode xkb_state::mode");

/**
 * Common state
 *
 * `mode` indicates whether the object is casted from a `xkb_server_state` or a
 * `xkb_client_state` struct.
 */
संरचना xkb_state {
    संरचना xkb_keymap *keymap;
    /*
     * Before updating the state, we keep a copy of just this struct. This
     * allows us to report which components of the state have changed.
     */
    संरचना state_components components;
    संरचना out_of_range_group out_of_range_group;

    गणना xkb_state_mode_internal mode : XKB_STATE_MODE_INTERNAL_MIN_WIDTH;
    पूर्णाङ्क refcnt : (परिमाण(पूर्णाङ्क) * CHAR_BIT - XKB_STATE_MODE_INTERNAL_MIN_WIDTH);
};

/**
 * Minimal client state
 *
 * Wrapper for strong typing
 */
संरचना xkb_client_state {
    /** Inherits from `xkb_state` */
    संरचना xkb_state base;
};

गणना state_flags {
    STATE_NO_FLAGS = 0,
    STATE_REQUIRE_किल्ली_EVENT = (1 << 0),
    STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_किल्ली = (1 << 1),
    STATE_UNLOCK_MODS_ON_PRESS = (1 << 2),
};

/**
 * Legacy server state, used for `LEGACY_MIXED_STATE` and `LEGACY_SERVER_STATE`
 * modes.
 *
 * Casted as `xkb_state` in the public API. See `xkb_state::mode`.
 */
संरचना xkb_server_state {
    /** Inherits from `xkb_state` */
    संरचना xkb_state base;

    /* Server-specific state data */

    darray(संरचना xkb_filter) filters;

    /**
     * Mix of constant configuration flags and
     * variable state-processing flags.
     */
    गणना state_flags flags;

    गणना xkb_a11y_flags a11y_flags;

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
    int16_t mod_किल्ली_मोजणी[XKB_MAX_MODS];
};

स्थिर ध्रुव संरचना xkb_किल्ली_type_entry *
get_entry_for_mods(ध्रुव संरचना xkb_किल्ली_type *type, xkb_mod_mask_t mods)
{
    प्रति (darray_size_t i = 0; i < type->अंक_entries; i++)
        यदि (entry_is_active(&type->entries[i]) &&
            type->entries[i].mods.mask == mods)
            प्रत्यागम &type->entries[i];
    प्रत्यागम रिक्त;
}

स्थिर ध्रुव संरचना xkb_किल्ली_type_entry *
get_entry_for_किल्ली_state(संरचना xkb_state *state, ध्रुव संरचना xkb_किल्ली *किल्ली,
                        xkb_layout_index_t group)
{
    ध्रुव संरचना xkb_किल्ली_type* ध्रुव type = किल्ली->groups[group].type;
    xkb_mod_mask_t active_mods = state->components.mods & type->mods.mask;
    प्रत्यागम get_entry_for_mods(type, active_mods);
}

स्थिर अन्तःस्थ xkb_level_index_t
state_किल्ली_get_level(संरचना xkb_state *state, ध्रुव संरचना xkb_किल्ली *किल्ली,
                         xkb_layout_index_t layout)
{
    यदि (layout >= किल्ली->अंक_groups)
        प्रत्यागम XKB_LEVEL_INVALID;

    /* If we don't find an explicit match the default is 0. */
    ध्रुव संरचना xkb_किल्ली_type_entry* ध्रुव entry =
        get_entry_for_किल्ली_state(state, किल्ली, layout);

    प्रत्यागम (entry) ? entry->level : 0;
}

/**
 * Returns the level to use for the given key and state, or
 * XKB_LEVEL_INVALID.
 */
xkb_level_index_t
xkb_state_किल्ली_get_level(संरचना xkb_state *state, xkb_keycode_t kc,
                        xkb_layout_index_t layout)
{
    ध्रुव संरचना xkb_किल्ली* ध्रुव किल्ली = Xkb_किल्ली(state->keymap, kc);

    प्रत्यागम (किल्ली)
        ? state_किल्ली_get_level(state, किल्ली, layout)
        : XKB_LEVEL_INVALID;
}

स्थिर अन्तःस्थ xkb_layout_index_t
state_किल्ली_get_layout(संरचना xkb_state *state, ध्रुव संरचना xkb_किल्ली *किल्ली)
{
    प्रत्यागम xkb_keymap_किल्ली_effective_layout(किल्ली, state->components.group);
}

/**
 * Returns the layout to use for the given key and state, taking
 * wrapping/clamping/etc into account, or XKB_LAYOUT_INVALID.
 */
xkb_layout_index_t
xkb_state_किल्ली_get_layout(संरचना xkb_state *state, xkb_keycode_t kc)
{
    ध्रुव संरचना xkb_किल्ली* ध्रुव किल्ली = Xkb_किल्ली(state->keymap, kc);

    यदि (!किल्ली)
        प्रत्यागम XKB_LAYOUT_INVALID;

    प्रत्यागम state_किल्ली_get_layout(state, किल्ली);
}

/* Empty action used for empty levels */
स्थिर ध्रुव सङ्घ xkb_action dummy_action = { .type = ACTION_TYPE_NONE };

स्थिर xkb_action_मोजणी_t
xkb_किल्ली_get_actions(संरचना xkb_state *state, ध्रुव संरचना xkb_किल्ली *किल्ली,
                    ध्रुव सङ्घ xkb_action **actions)
{
    ध्रुव xkb_layout_index_t layout = state_किल्ली_get_layout(state, किल्ली);
    ध्रुव xkb_level_index_t level = state_किल्ली_get_level(state, किल्ली, layout);
    यदि (level == XKB_LEVEL_INVALID)
        गच्छ err;

    ध्रुव xkb_action_मोजणी_t मोजणी =
        xkb_keymap_किल्ली_get_actions_by_level(state->keymap, किल्ली,
                                            layout, level, actions);
    यदि (!मोजणी)
        गच्छ err;

    प्रत्यागम मोजणी;

err:
    /* Use a dummy action if no corresponding level was found or if it is empty.
     * This is required e.g. to handle latches properly. */
    *actions = &dummy_action;
    प्रत्यागम 1;
}

स्थिर संरचना xkb_filter *
xkb_filter_new(संरचना xkb_server_state *state)
{
    संरचना xkb_filter *filter = रिक्त, *iter;

    darray_foreach(iter, state->filters) {
        यदि (iter->func)
            अनुवर्त;
        /* Use available slot */
        filter = iter;
        भङ्ग;
    }

    यदि (!filter) {
        /* No available slot: resize the filters array */
        darray_resize0(state->filters, darray_size(state->filters) + 1);
        filter = &darray_item(state->filters, darray_size(state->filters) -1);
    }

    filter->refcnt = 1;
    प्रत्यागम filter;
}

/***====================================================================***/

गणना xkb_filter_result {
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
    यदि ((filter_)->action.group.flags & ACTION_ABSOLUTE_SWITCH)          \
        (state_)->components.component_ = (filter_)->action.group.group; \
    अन्यथा                                                                 \
        (state_)->components.component_ += (filter_)->action.group.group

स्थिर शून्य
xkb_filter_group_set_new(संरचना xkb_server_state *state,
                         संरचना xkb_events *events,
                         संरचना xkb_filter *filter)
{
    static_assert(परिमाण(state->base.components.base_group) ==
                  परिमाण(filter->priv),
                  "Max groups don't fit");
    filter->priv = (uint32_t)state->base.components.base_group;
    apply_group_delta(filter, &state->base, base_group);
}

स्थिर bool
xkb_filter_group_set_func(संरचना xkb_server_state *state,
                          संरचना xkb_events *events,
                          संरचना xkb_filter *filter,
                          ध्रुव संरचना xkb_किल्ली *किल्ली,
                          गणना xkb_किल्ली_direction direction)
{
    यदि (किल्ली != filter->किल्ली) {
        filter->action.group.flags &= ~ACTION_LOCK_CLEAR;
        प्रत्यागम XKB_FILTER_CONTINUE;
    }

    विकल्प (direction) {
    पक्ष XKB_किल्ली_DOWN:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_किल्ली_REPEATED:
        प्रत्यागम XKB_FILTER_CONSUME;
    स्वभाव:
        यदि (--filter->refcnt > 0)
            प्रत्यागम XKB_FILTER_CONSUME;
    }

    static_assert(परिमाण(state->base.components.base_group) == परिमाण(filter->priv),
                  "Max groups don't fit");
    state->base.components.base_group = (int32_t) filter->priv;

    यदि (filter->action.group.flags & ACTION_LOCK_CLEAR)
        state->base.components.locked_group = 0;

    filter->func = रिक्त;
    प्रत्यागम XKB_FILTER_CONTINUE;
}

स्थिर गणना xkb_state_component
get_state_component_changes(ध्रुव संरचना state_components *a,
                            ध्रुव संरचना state_components *b)
{
    गणना xkb_state_component mask = 0;

    यदि (a->group != b->group)
        mask |= XKB_STATE_LAYOUT_EFFECTIVE;
    यदि (a->base_group != b->base_group)
        mask |= XKB_STATE_LAYOUT_DEPRESSED;
    यदि (a->latched_group != b->latched_group)
        mask |= XKB_STATE_LAYOUT_LATCHED;
    यदि (a->locked_group != b->locked_group)
        mask |= XKB_STATE_LAYOUT_LOCKED;
    यदि (a->mods != b->mods)
        mask |= XKB_STATE_MODS_EFFECTIVE;
    यदि (a->base_mods != b->base_mods)
        mask |= XKB_STATE_MODS_DEPRESSED;
    यदि (a->latched_mods != b->latched_mods)
        mask |= XKB_STATE_MODS_LATCHED;
    यदि (a->locked_mods != b->locked_mods)
        mask |= XKB_STATE_MODS_LOCKED;
    यदि (a->leds != b->leds)
        mask |= XKB_STATE_LEDS;
    यदि (a->controls != b->controls)
        mask |= XKB_STATE_CONTROLS_EFFECTIVE;

    प्रत्यागम mask;
}

स्थिर शून्य
xkb_filter_group_lock_new(संरचना xkb_server_state *state,
                          संरचना xkb_events *events,
                          संरचना xkb_filter *filter)
{
    यदि (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release: do nothing on key press.
         *
         * This is a keymap format v2 extension.
         */
        प्रत्यागम;
    } अन्यथा {
        /* Lock on key press */
        apply_group_delta(filter, &state->base, locked_group);
    }
}

स्थिर bool
xkb_filter_group_lock_func(संरचना xkb_server_state *state,
                           संरचना xkb_events *events,
                           संरचना xkb_filter *filter,
                           ध्रुव संरचना xkb_किल्ली *किल्ली,
                           गणना xkb_किल्ली_direction direction)
{
    यदि (किल्ली != filter->किल्ली) {
        यदि ((filter->action.group.flags & ACTION_LOCK_ON_RELEASE) &&
            direction == XKB_किल्ली_DOWN) {
            /*
             * Another key has been pressed after the locking key:
             * cancel group lock on release.
             *
             * This is a keymap v2 extension.
             */
            filter->action.group.flags &= ~ACTION_LOCK_ON_RELEASE;
        }
        प्रत्यागम XKB_FILTER_CONTINUE;
    }

    विकल्प (direction) {
    पक्ष XKB_किल्ली_DOWN:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_किल्ली_REPEATED:
        प्रत्यागम XKB_FILTER_CONSUME;
    स्वभाव:
        यदि (--filter->refcnt > 0)
            प्रत्यागम XKB_FILTER_CONSUME;
    }

    यदि (filter->action.group.flags & ACTION_LOCK_ON_RELEASE) {
        /*
         * Lock on key release
         *
         * This is a keymap v2 extension.
         */
        apply_group_delta(filter, &state->base, locked_group);
    } अन्यथा {
        /* Lock on key press: do nothing on key release. */
    }
    filter->func = रिक्त;
    प्रत्यागम XKB_FILTER_CONTINUE;
}

स्थिर bool
xkb_action_breaks_latch(ध्रुव सङ्घ xkb_action *action,
                        गणना xkb_action_controls controls,
                        गणना xkb_internal_action_flags flag,
                        xkb_mod_mask_t mask)
{
    विकल्प (action->type) {
    पक्ष ACTION_TYPE_NONE:
    पक्ष ACTION_TYPE_VOID:
    पक्ष ACTION_TYPE_PTR_BUTTON:
    पक्ष ACTION_TYPE_PTR_LOCK:
    पक्ष ACTION_TYPE_CTRL_SET:
    पक्ष ACTION_TYPE_CTRL_LOCK:
    पक्ष ACTION_TYPE_SWITCH_VT:
    पक्ष ACTION_TYPE_TERMINATE:
    पक्ष ACTION_TYPE_REDIRECT_किल्ली:
        प्रत्यागम true;
    पक्ष ACTION_TYPE_PTR_MOVE:
    पक्ष ACTION_TYPE_PTR_DEFAULT:
        प्रत्यागम !(controls & CONTROL_MOUSE_किल्ल्या);
    पक्ष ACTION_TYPE_INTERNAL:
        प्रत्यागम (action->internal.flags & flag) &&
               ((action->internal.clear_latched_mods & mask) == mask);
    स्वभाव:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_अंक_ENTRIES,
                      "Missing action type");
        प्रत्यागम false;
    }
}

गणना xkb_किल्ली_latch_state {
    NO_LATCH = 0,
    LATCH_किल्ली_DOWN,
    LATCH_PENDING,
    किल्ली_LATCH_STATE_अंक_ENTRIES
};

#define MAX_XKB_किल्ली_LATCH_STATE_LOG2 2
#यदि (किल्ली_LATCH_STATE_अंक_ENTRIES > (1 << MAX_XKB_किल्ली_LATCH_STATE_LOG2)) || \
    (-XKB_MAX_GROUPS) < (INT32_MIN >> MAX_XKB_किल्ली_LATCH_STATE_LOG2) || \
      XKB_MAX_GROUPS > (INT32_MAX >> MAX_XKB_किल्ली_LATCH_STATE_LOG2)
#error "Cannot represent priv field of the group latch filter"
#endif

/* Hold the latch state *and* the group delta */
सङ्घ group_latch_priv {
    uint32_t priv;
    संरचना {
        /* The type is really: enum xkb_key_latch_state, but it is problematic
         * on Windows, because it is interpreted as signed and leads to wrong
         * negative values. */
        unsigned पूर्णाङ्क latch:MAX_XKB_किल्ली_LATCH_STATE_LOG2;
        int32_t group_delta:(32 - MAX_XKB_किल्ली_LATCH_STATE_LOG2);
    };
};

स्थिर शून्य
xkb_filter_group_latch_new(संरचना xkb_server_state *state,
                           संरचना xkb_events *events,
                           संरचना xkb_filter *filter)
{
    ध्रुव सङ्घ group_latch_priv priv = {
        .latch = LATCH_किल्ली_DOWN,
        .group_delta = (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)
            ? filter->action.group.group - state->base.components.base_group
            : filter->action.group.group
    };
    filter->priv = priv.priv;
    /* Like group set */
    apply_group_delta(filter, &state->base, base_group);
}

स्थिर bool
xkb_filter_group_latch_func(संरचना xkb_server_state *state,
                            संरचना xkb_events *events,
                            संरचना xkb_filter *filter,
                            ध्रुव संरचना xkb_किल्ली *किल्ली,
                            गणना xkb_किल्ली_direction direction)
{
    सङ्घ group_latch_priv priv = {.priv = filter->priv};
    गणना xkb_किल्ली_latch_state latch = priv.latch;

    यदि (direction == XKB_किल्ली_DOWN) {
        ध्रुव सङ्घ xkb_action *actions = रिक्त;
        ध्रुव xkb_action_मोजणी_t मोजणी = xkb_किल्ली_get_actions(&state->base, किल्ली,
                                                             &actions);

        यदि (latch == LATCH_किल्ली_DOWN) {
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
            यदि (state->a11y_flags & XKB_A11_Y_LATCH_SIMULTANEOUS_किल्ल्या) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                प्रति (xkb_action_मोजणी_t k = 0; k < मोजणी; k++) {
                    यदि (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_GROUP_LATCH,
                                                0))
                    {
                        latch = NO_LATCH;
                        भङ्ग;
                    }
                }
            } अन्यथा {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        अन्यथा यदि (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is
             * pressed, or promote it to a lock if it's the same group delta &
             * flags and latchToLock option is enabled. */
            ध्रुव bool sticky_किल्ल्या = state->base.components.controls
                                   & CONTROL_STICKY_किल्ल्या;
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
            प्रति (xkb_action_मोजणी_t k = 0; k < मोजणी; k++) {
                यदि (((actions[k].type == ACTION_TYPE_GROUP_LATCH) ||
                     (actions[k].type == ACTION_TYPE_GROUP_SET && sticky_किल्ल्या)) &&
                     actions[k].group.group == filter->action.group.group &&
                     (actions[k].group.flags & ACTION_ABSOLUTE_SWITCH) ==
                     (filter->action.group.flags & ACTION_ABSOLUTE_SWITCH)) {
                    यदि (filter->action.group.flags & ACTION_LATCH_TO_LOCK &&
                        filter->action.group.group != 0)
                    {
                        /* Promote to lock */
                        filter->action.type = ACTION_TYPE_GROUP_LOCK;
                        filter->func = xkb_filter_group_lock_func;
                        xkb_filter_group_lock_new(state, events, filter);
                        state->base.components.latched_group -= priv.group_delta;
                        filter->किल्ली = किल्ली;
                        /* XXX beep beep! */
                        प्रत्यागम XKB_FILTER_CONSUME;
                    }
                    /* Do nothing if `latchToLock` option is not activated; if
                     * the latch is not broken by the following actions and the
                     * key is not consumed, then another latch filter will be
                     * created.
                     */
                    अनुवर्त;
                }
                अन्यथा यदि (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_GROUP_LATCH,
                                                 0))
                {
                    /* Breaks the latch */
                    state->base.components.latched_group -= priv.group_delta;
                    filter->func = रिक्त;
                    प्रत्यागम XKB_FILTER_CONTINUE;
                }
            }
        }
        अन्यथा {
            /* Ignore press in NO_LATCH state */
            प्रतिज्ञा(latch == NO_LATCH);
        }
    }
    अन्यथा यदि (किल्ली != filter->किल्ली) {
        /* Ignore repeat/release of other keys */
    }
    अन्यथा यदि (direction == XKB_किल्ली_REPEATED) {
        प्रत्यागम XKB_FILTER_CONSUME; // TODO: check
    }
    अन्यथा {
        /* Our key got released.  If we've set it to clear locks, and we
         * currently have a group locked, then release it and
         * don't actually latch.  Else we've actually hit the latching
         * stage, so set PENDING and move our group from base to
         * latched. */
        यदि ((filter->action.group.flags & ACTION_LOCK_CLEAR) &&
             state->base.components.locked_group) {
            यदि (latch == LATCH_PENDING)
                state->base.components.latched_group -= priv.group_delta;
            अन्यथा
                state->base.components.base_group -= priv.group_delta;
            state->base.components.locked_group = 0;
            filter->func = रिक्त;
        }
        /* Broken latch */
        अन्यथा यदि (latch == NO_LATCH) {
            state->base.components.base_group -= priv.group_delta;
            filter->func = रिक्त;
        }
        /* We may already have reached the latch state if pressing the
         * key multiple times without latch-to-lock enabled. */
        अन्यथा यदि (latch == LATCH_किल्ली_DOWN) {
            latch = LATCH_PENDING;
            /* Switch from set to latch */
            state->base.components.base_group -= priv.group_delta;
            state->base.components.latched_group += priv.group_delta;
            /* XXX beep beep! */
        }
    }

    priv.latch = latch;
    filter->priv = priv.priv;

    प्रत्यागम XKB_FILTER_CONTINUE;
}

स्थिर शून्य
xkb_filter_mod_set_new(संरचना xkb_server_state *state,
                       संरचना xkb_events *events,
                       संरचना xkb_filter *filter)
{
    ध्रुव गणना xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    यदि ((filter->action.mods.flags & unlock) == unlock) {
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

स्थिर bool
xkb_filter_mod_set_func(संरचना xkb_server_state *state,
                        संरचना xkb_events *events,
                        संरचना xkb_filter *filter,
                        ध्रुव संरचना xkb_किल्ली *किल्ली,
                        गणना xkb_किल्ली_direction direction)
{

    यदि (किल्ली != filter->किल्ली) {
        filter->action.mods.flags &= ~ACTION_LOCK_CLEAR;
        प्रत्यागम XKB_FILTER_CONTINUE;
    }

    विकल्प (direction) {
    पक्ष XKB_किल्ली_DOWN:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_किल्ली_REPEATED:
        प्रत्यागम XKB_FILTER_CONSUME;
    स्वभाव:
        यदि (--filter->refcnt > 0)
            प्रत्यागम XKB_FILTER_CONSUME;
    }

    state->clear_mods |= (xkb_mod_mask_t) filter->priv;
    ध्रुव गणना xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_LOCK_CLEAR;
    यदि ((filter->action.mods.flags & unlock) == ACTION_LOCK_CLEAR)
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;

    filter->func = रिक्त;
    प्रत्यागम XKB_FILTER_CONTINUE;
}

स्थिर शून्य
xkb_filter_mod_lock_new(संरचना xkb_server_state *state,
                        संरचना xkb_events *events,
                        संरचना xkb_filter *filter)
{
    filter->priv = (state->base.components.locked_mods &
                    filter->action.mods.mods.mask);

    यदि (filter->priv && (filter->action.mods.flags & ACTION_UNLOCK_ON_PRESS)) {
        /*
         * Some of the target modifiers were locked before key press: unlock.
         *
         * This is a keymap v2 extension: unlock-on-press.
         */
        यदि (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
            state->base.components.locked_mods &= ~filter->priv;
        /* No further action: cancel filter */
        filter->func = रिक्त;
    } अन्यथा {
        /* Set base mods; lock mods if relevant (XKB 1.0 spec) */
        state->set_mods |= filter->action.mods.mods.mask;
        यदि (!(filter->action.mods.flags & ACTION_LOCK_NO_LOCK))
            state->base.components.locked_mods |= filter->action.mods.mods.mask;
    }
}

स्थिर bool
xkb_filter_mod_lock_func(संरचना xkb_server_state *state,
                         संरचना xkb_events *events,
                         संरचना xkb_filter *filter,
                         ध्रुव संरचना xkb_किल्ली *किल्ली,
                         गणना xkb_किल्ली_direction direction)
{
    यदि (किल्ली != filter->किल्ली)
        प्रत्यागम XKB_FILTER_CONTINUE;

    विकल्प (direction) {
    पक्ष XKB_किल्ली_DOWN:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_किल्ली_REPEATED:
        प्रत्यागम XKB_FILTER_CONSUME;
    स्वभाव:
        यदि (--filter->refcnt > 0)
            प्रत्यागम XKB_FILTER_CONSUME;
    }

    state->clear_mods |= filter->action.mods.mods.mask;
    यदि (!(filter->action.mods.flags & ACTION_LOCK_NO_UNLOCK))
        state->base.components.locked_mods &= ~filter->priv;

    filter->func = रिक्त;
    प्रत्यागम XKB_FILTER_CONTINUE;
}

स्थिर शून्य
xkb_filter_mod_latch_new(संरचना xkb_server_state *state,
                         संरचना xkb_events *events,
                         संरचना xkb_filter *filter)
{
    /* Latch-on-press + clear-locks imply unlock-on-press */
    ध्रुव गणना xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                              | ACTION_LATCH_ON_PRESS;

    यदि ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
        (filter->action.mods.flags & unlockOnPress) &&
        (state->base.components.locked_mods & filter->action.mods.mods.mask) ==
         filter->action.mods.mods.mask) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension: clear locks and do not latch.
         */
        state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
        filter->func = रिक्त;
    } अन्यथा यदि (filter->action.mods.flags & ACTION_LATCH_ON_PRESS) {
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
        filter->priv = LATCH_किल्ली_DOWN;
        state->set_mods |= filter->action.mods.mods.mask;
    }
}

स्थिर bool
xkb_filter_mod_latch_func(संरचना xkb_server_state *state,
                          संरचना xkb_events *events,
                          संरचना xkb_filter *filter,
                          ध्रुव संरचना xkb_किल्ली *किल्ली,
                          गणना xkb_किल्ली_direction direction)
{
    गणना xkb_किल्ली_latch_state latch = filter->priv;

    यदि (direction == XKB_किल्ली_DOWN) {
        ध्रुव सङ्घ xkb_action *actions = रिक्त;
        ध्रुव xkb_action_मोजणी_t मोजणी = xkb_किल्ली_get_actions(&state->base, किल्ली,
                                                             &actions);

        यदि (latch == LATCH_किल्ली_DOWN) {
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
            यदि (state->a11y_flags & XKB_A11_Y_LATCH_SIMULTANEOUS_किल्ल्या) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                प्रति (xkb_action_मोजणी_t k = 0; k < मोजणी; k++) {
                    यदि (xkb_action_breaks_latch(&(actions[k]),
                                                state->base.components.controls,
                                                INTERNAL_BREAKS_MOD_LATCH,
                                                filter->action.mods.mods.mask))
                    {
                        latch = NO_LATCH;
                        भङ्ग;
                    }
                }
            } अन्यथा {
                /* Unconditionally prevent the latch to trigger. */
                latch = NO_LATCH;
            }
        }
        अन्यथा यदि (latch == LATCH_PENDING) {
            /* If this is a new keypress and we're awaiting our single latched
             * keypress, then either break the latch if any random key is pressed,
             * or promote it to a lock or plain base set if it's the same
             * modifier. */
            ध्रुव bool sticky_किल्ल्या = state->base.components.controls
                                   & CONTROL_STICKY_किल्ल्या;
            /*
             * NOTE: Action’s flags are not compared:
             * - flags may have been modified by keyboard controls at the
             *   creation of the filter;
             * - it is unlikely to have two LatchMods() actions on the same
             *   key that differ only by their flags;
             * - even if it were the case, one would most likely want both
             *   actions to be considered compatible in this context.
             */
            प्रति (xkb_action_मोजणी_t k = 0; k < मोजणी; k++) {
                यदि (((actions[k].type == ACTION_TYPE_MOD_LATCH) ||
                     (actions[k].type == ACTION_TYPE_MOD_SET && sticky_किल्ल्या)) &&
                    actions[k].mods.mods.mask == filter->action.mods.mods.mask)
                {
                    यदि (filter->action.mods.flags & ACTION_LATCH_TO_LOCK) {
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
                    filter->किल्ली = किल्ली;
                    /* Clear latches */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    /* XXX beep beep! */
                    प्रत्यागम XKB_FILTER_CONSUME;
                }
                अन्यथा यदि (xkb_action_breaks_latch(&(actions[k]),
                                                 state->base.components.controls,
                                                 INTERNAL_BREAKS_MOD_LATCH,
                                                 filter->action.mods.mods.mask))
                {
                    /* XXX: This may be totally broken, we might need to break the
                     *      latch in the next run after this press? */
                    state->base.components.latched_mods &=
                        ~filter->action.mods.mods.mask;
                    filter->func = रिक्त;
                    प्रत्यागम XKB_FILTER_CONTINUE;
                }
            }
        }
        अन्यथा {
            /* Ignore press in NO_LATCH state */
            प्रतिज्ञा(latch == NO_LATCH);
        }
    }
    अन्यथा यदि (किल्ली != filter->किल्ली) {
        /* Ignore repeat/release of other keys */
    }
    अन्यथा यदि (direction == XKB_किल्ली_REPEATED) {
        प्रत्यागम XKB_FILTER_CONSUME; // TODO: check
    }
    अन्यथा {
        /* Our key got released.  If we’ve set it to clear locks, and we
         * currently have the same modifiers locked, then release them and
         * don't actually latch.  Else we’ve actually hit the latching
         * stage, so set PENDING and move our modifier from base to
         * latched. */

        /* Latch-on-press + clear-locks imply unlock-on-press */
        ध्रुव गणना xkb_action_flags unlockOnPress = ACTION_UNLOCK_ON_PRESS
                                                  | ACTION_LATCH_ON_PRESS;

        यदि ((filter->action.mods.flags & ACTION_LOCK_CLEAR) &&
            !(filter->action.mods.flags & unlockOnPress) &&
            (state->base.components.locked_mods &
             filter->action.mods.mods.mask) == filter->action.mods.mods.mask)
        {
            /* XXX: We might be a bit overenthusiastic about clearing
             *      mods other filters have set here? */
            यदि (latch == LATCH_PENDING)
                state->base.components.latched_mods &=
                    ~filter->action.mods.mods.mask;
            अन्यथा
                state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.locked_mods &= ~filter->action.mods.mods.mask;
            filter->func = रिक्त;
        }
        अन्यथा यदि (latch == NO_LATCH) {
            /* Broken latch */
            state->clear_mods |= filter->action.mods.mods.mask;
            filter->func = रिक्त;
        }
        अन्यथा यदि (!(filter->action.mods.flags & ACTION_LATCH_ON_PRESS)) {
            latch = LATCH_PENDING;
            state->clear_mods |= filter->action.mods.mods.mask;
            state->base.components.latched_mods |= filter->action.mods.mods.mask;
            /* XXX beep beep! */
        }
    }

    filter->priv = latch;

    प्रत्यागम XKB_FILTER_CONTINUE;
}

स्थिर शून्य
append_pointer_move(संरचना xkb_events *events,
                    संरचना xkb_filter *filter)
{
    गणना xkb_pointer_motion_flags flags = 0;
    यदि (filter->action.ptr.flags & ACTION_ABSOLUTE_X)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_X;
    यदि (filter->action.ptr.flags & ACTION_ABSOLUTE_Y)
        flags |= XKB_POINTER_MOTION_ABSOLUTE_Y;
    यदि (filter->action.ptr.flags & ACTION_REPEAT)
        flags |= XKB_POINTER_MOTION_REPEATS;
    darray_append(events->queue, (संरचना xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_MOTION,
        .pointer_motion = {
            .size = परिमाण(((संरचना xkb_event*)0)->pointer_motion),
            .flags = flags,
            .x = filter->action.ptr.x,
            .y = filter->action.ptr.y,
        }
    });
}

स्थिर शून्य
xkb_filter_pointer_move_new(संरचना xkb_server_state *state,
                            संरचना xkb_events *events,
                            संरचना xkb_filter *filter)
{
    यदि (!events) {
        filter->func = रिक्त;
        प्रत्यागम;
    }

    प्रतिज्ञा(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_किल्ल्या);
    append_pointer_move(events, filter);
    state->flags &= ~STATE_REQUIRE_किल्ली_EVENT;
}

स्थिर bool
xkb_filter_pointer_move_func(संरचना xkb_server_state *state,
                             संरचना xkb_events *events,
                             संरचना xkb_filter *filter,
                             ध्रुव संरचना xkb_किल्ली *किल्ली,
                             गणना xkb_किल्ली_direction direction)
{
    यदि (किल्ली != filter->किल्ली || !events)
        प्रत्यागम XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_किल्ली_EVENT;

    विकल्प (direction) {
    पक्ष XKB_किल्ली_DOWN:
        filter->refcnt++;
        append_pointer_move(events, filter);
        प्रत्यागम XKB_FILTER_CONSUME;
    पक्ष XKB_किल्ली_REPEATED:
        यदि (filter->action.ptr.flags & ACTION_REPEAT)
            append_pointer_move(events, filter);
        प्रत्यागम XKB_FILTER_CONSUME;
    स्वभाव:
        यदि (--filter->refcnt > 0)
            प्रत्यागम XKB_FILTER_CONSUME;
    }

    filter->func = रिक्त;
    प्रत्यागम XKB_FILTER_CONSUME;
}

/* Implemented with xkb_machine API because it requires struct xkb_machine */
स्थिर शून्य
xkb_filter_pointer_button_new(संरचना xkb_server_state *state,
                              संरचना xkb_events *events,
                              संरचना xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
स्थिर bool
xkb_filter_pointer_button_func(संरचना xkb_server_state *state,
                               संरचना xkb_events *events,
                               संरचना xkb_filter *filter,
                               ध्रुव संरचना xkb_किल्ली *किल्ली,
                               गणना xkb_किल्ली_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
स्थिर शून्य
xkb_filter_pointer_lock_button_new(संरचना xkb_server_state *state,
                                   संरचना xkb_events *events,
                                   संरचना xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
स्थिर bool
xkb_filter_pointer_lock_button_func(संरचना xkb_server_state *state,
                                    संरचना xkb_events *events,
                                    संरचना xkb_filter *filter,
                                    ध्रुव संरचना xkb_किल्ली *किल्ली,
                                    गणना xkb_किल्ली_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
स्थिर शून्य
xkb_filter_pointer_set_default_button_new(संरचना xkb_server_state *state,
                                          संरचना xkb_events *events,
                                          संरचना xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
स्थिर bool
xkb_filter_pointer_set_default_button_func(संरचना xkb_server_state *state,
                                           संरचना xkb_events *events,
                                           संरचना xkb_filter *filter,
                                           ध्रुव संरचना xkb_किल्ली *किल्ली,
                                           गणना xkb_किल्ली_direction direction);

स्थिर शून्य
xkb_filter_terminate_server_new(संरचना xkb_server_state *state,
                                 संरचना xkb_events *events,
                                 संरचना xkb_filter *filter);

स्थिर bool
xkb_filter_terminate_server_func(संरचना xkb_server_state *state,
                                  संरचना xkb_events *events,
                                  संरचना xkb_filter *filter,
                                  ध्रुव संरचना xkb_किल्ली *किल्ली,
                                  गणना xkb_किल्ली_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
स्थिर शून्य
xkb_filter_switch_virtual_console_new(संरचना xkb_server_state *state,
                                      संरचना xkb_events *events,
                                      संरचना xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
स्थिर bool
xkb_filter_switch_virtual_console_func(संरचना xkb_server_state *state,
                                       संरचना xkb_events *events,
                                       संरचना xkb_filter *filter,
                                       ध्रुव संरचना xkb_किल्ली *किल्ली,
                                       गणना xkb_किल्ली_direction direction);

स्थिर अन्तःस्थ शून्य
clear_all_latches_and_locks(संरचना xkb_server_state *state,
                            संरचना xkb_events *events);

स्थिर शून्य
xkb_filter_ctrls_new(संरचना xkb_server_state *state,
                     संरचना xkb_events *events,
                     संरचना xkb_filter *filter)
{
    यदि (filter->action.type == ACTION_TYPE_CTRL_SET) {
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

    यदि (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_LOCK)) {
        /* Enable the specified controls that are not already enabled */
        state->base.components.controls |= filter->action.ctrls.ctrls;
    }
}

स्थिर bool
xkb_filter_ctrls_func(संरचना xkb_server_state *state,
                      संरचना xkb_events *events,
                      संरचना xkb_filter *filter,
                      ध्रुव संरचना xkb_किल्ली *किल्ली,
                      गणना xkb_किल्ली_direction direction)
{
    यदि (किल्ली != filter->किल्ली)
        प्रत्यागम XKB_FILTER_CONTINUE;

    विकल्प (direction) {
    पक्ष XKB_किल्ली_DOWN:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_किल्ली_REPEATED:
        प्रत्यागम XKB_FILTER_CONSUME;
    स्वभाव:
        यदि (--filter->refcnt > 0)
            प्रत्यागम XKB_FILTER_CONSUME;
    }

    यदि (filter->action.type == ACTION_TYPE_CTRL_SET ||
        !(filter->action.ctrls.flags & ACTION_LOCK_NO_UNLOCK)) {
        ध्रुव गणना xkb_action_controls old = state->base.components.controls;

        /*
         * Set: Disable specified controls that were *not* enabled at key press.
         * Lock: Disable specified controls that *were* enabled at key press.
         */
        state->base.components.controls &= ~(गणना xkb_action_controls) filter->priv;

        यदि ((old & CONTROL_STICKY_किल्ल्या) &&
            !(state->base.components.controls & CONTROL_STICKY_किल्ल्या)) {
            /* Sticky keys were disabled: clear all locks and latches */
            clear_all_latches_and_locks(state, events);
        }
    }

    filter->func = रिक्त;
    प्रत्यागम XKB_FILTER_CONTINUE;
}

स्थिर bool
append_redirect_किल्ली_events(संरचना xkb_server_state *state,
                           संरचना xkb_events *events,
                           ध्रुव संरचना xkb_redirect_किल्ली_action *redirect,
                           गणना xkb_किल्ली_direction direction)
{
    गणना xkb_state_component changed = 0;
    ध्रुव xkb_mod_mask_t mask = redirect->affect_mods;

    /*
     * Reference state: find the last state update in the queue, otherwise
     * use the current state.
     */
    संरचना xkb_event *event;
    संरचना state_components last_components = state->base.components;
    darray_foreach_reverse(event, events->queue) {
        यदि (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
            last_components = event->components.components;
            भङ्ग;
        }
    }

    यदि (mask) {
        संरचना state_components new = last_components;
        new.base_mods = (new.base_mods & ~mask) | redirect->mods;
        new.latched_mods = (new.latched_mods & ~mask) | redirect->mods;
        new.locked_mods = (new.locked_mods & ~mask) | redirect->mods;
        new.mods = (new.mods & ~mask) | redirect->mods;

        changed = get_state_component_changes(&last_components, &new);
        यदि (changed) {
            darray_append(events->queue, (संरचना xkb_event) {
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
            darray_append(events->queue, (संरचना xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_FRAME,
            });
        }
    }

    darray_append(events->queue, (संरचना xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_किल्ली,
        .किल्ली = {
            .keycode = redirect->keycode,
            .direction = direction
        }
    });

    यदि (mask && changed) {
        /* Restore state */
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = last_components,
                .changed = changed
            }
        });
    }

    state->flags &= ~STATE_REQUIRE_किल्ली_EVENT;
    प्रत्यागम true;
}

स्थिर शून्य
xkb_filter_redirect_किल्ली_new(संरचना xkb_server_state *state,
                            संरचना xkb_events *events,
                            संरचना xkb_filter *filter)
{
    /* Action effectual only with the xkb_machine API and a valid keycode */
    यदि (!events || filter->action.redirect.keycode == XKB_KEYCODE_INVALID) {
        filter->func = रिक्त;
        प्रत्यागम;
    }
    append_redirect_किल्ली_events(state, events, &filter->action.redirect,
                               XKB_किल्ली_DOWN);
}

स्थिर bool
xkb_filter_redirect_किल्ली_func(संरचना xkb_server_state *state,
                             संरचना xkb_events *events,
                             संरचना xkb_filter *filter,
                             ध्रुव संरचना xkb_किल्ली *किल्ली,
                             गणना xkb_किल्ली_direction direction)
{
    यदि (किल्ली != filter->किल्ली)
        प्रत्यागम XKB_FILTER_CONTINUE;

    यदि (direction == XKB_किल्ली_UP) {
        append_redirect_किल्ली_events(state, events,
                                   &filter->action.redirect, XKB_किल्ली_UP);
        filter->func = रिक्त;
        प्रत्यागम XKB_FILTER_CONSUME;
     } अन्यथा यदि (direction == XKB_किल्ली_DOWN) {
        ध्रुव सङ्घ xkb_action *actions = रिक्त;
        ध्रुव xkb_action_मोजणी_t मोजणी =
            xkb_किल्ली_get_actions(&state->base, किल्ली, &actions);

        प्रति (xkb_action_मोजणी_t a = 0; a < मोजणी; a++) {
            यदि (actions[a].type == ACTION_TYPE_REDIRECT_किल्ली &&
                actions[a].redirect.keycode != filter->action.redirect.keycode) {
                    /* One action redirects to a different key: release first */
                    append_redirect_किल्ली_events(state, events,
                                               &filter->action.redirect,
                                               XKB_किल्ली_UP);
                filter->func = रिक्त;
                प्रत्यागम XKB_FILTER_CONTINUE;
            }
        }
    }

    append_redirect_किल्ली_events(state, events, &filter->action.redirect,
                               direction);
    प्रत्यागम XKB_FILTER_CONSUME;
}

स्थिर ध्रुव संरचना {
    शून्य (*new)(संरचना xkb_server_state *state,
                संरचना xkb_events *events,
                संरचना xkb_filter *filter);
    bool (*func)(संरचना xkb_server_state *state,
                 संरचना xkb_events *events,
                 संरचना xkb_filter *filter,
                 ध्रुव संरचना xkb_किल्ली *किल्ली, गणना xkb_किल्ली_direction direction);
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
    [ACTION_TYPE_REDIRECT_किल्ली] = { xkb_filter_redirect_किल्ली_new,
                                   xkb_filter_redirect_किल्ली_func },
};

/**
 * Applies any relevant filters to the key, first from the list of filters
 * that are currently active, then if no filter has claimed the key, possibly
 * apply a new filter from the key action.
 */
स्थिर शून्य
xkb_filter_apply_all(संरचना xkb_server_state *state,
                     संरचना xkb_events *events,
                     ध्रुव संरचना xkb_किल्ली *किल्ली,
                     गणना xkb_किल्ली_direction direction)
{
    /* First run through all the currently active filters and see if any of
     * them have consumed this event. */
    bool consumed = false;
    संरचना xkb_filter *filter;
    darray_foreach(filter, state->filters) {
        यदि (!filter->func)
            अनुवर्त;

        यदि (filter->func(state, events, filter, किल्ली, direction) == XKB_FILTER_CONSUME)
            consumed = true;
    }
    यदि (consumed || direction == XKB_किल्ली_UP)
        प्रत्यागम;

    /* No filter consumed this event, so proceed with the key actions */
    ध्रुव सङ्घ xkb_action *actions = रिक्त;
    xkb_action_मोजणी_t मोजणी = xkb_किल्ली_get_actions(&state->base, किल्ली,
                                                         &actions);

    प्रति (संरचना {
            xkb_action_मोजणी_t मोजणी;
            ध्रुव सङ्घ xkb_action *actions;
         } next = {0};
         मोजणी;
         मोजणी = next.मोजणी, next.मोजणी = 0, actions = next.actions
    ) {
        /*
         * Process actions sequentially.
         *
         * NOTE: We rely on the parser to disallow multiple modifier or group
         * actions (see `CheckMultipleActionsCategories`). Allowing multiple
         * such actions requires a refactor of the state handling.
         */
        प्रति (xkb_action_मोजणी_t k = 0; k < मोजणी; k++) {
            /*
             * It's possible for the keymap to set action->type explicitly, like
             * so:
             *     interpret XF86_Next_VMode {
             *         action = Private(type=0x86, data="+VMode");
             *     };
             * We don't handle those.
             */
            यदि (actions[k].type >= ACTION_TYPE_अंक_ENTRIES)
                अनुवर्त;

            /* Go to next action if no corresponding action handler */
            यदि (!filter_action_funcs[actions[k].type].new)
                अनुवर्त;

            /* Add a new filter and run the corresponding initial action */
            filter = xkb_filter_new(state);
            filter->किल्ली = किल्ली;
            filter->action = actions[k];

            विकल्प (filter->action.type) {
            पक्ष ACTION_TYPE_MOD_SET:
                यदि (state->base.components.controls & CONTROL_STICKY_किल्ल्या) {
                    /* Convert modifier set action to a latch */
                    filter->action.type = ACTION_TYPE_MOD_LATCH;
                    यदि (state->a11y_flags & XKB_A11_Y_STICKY_किल्ल्या_LATCH_TO_LOCK) {
                        filter->action.mods.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                /* fallthrough */
            पक्ष ACTION_TYPE_MOD_LATCH:
            पक्ष ACTION_TYPE_MOD_LOCK: {
                स्थिर ध्रुव गणना xkb_action_flags unlock_mods_on_press
                    = ACTION_LOCK_CLEAR     /* SetMods & LatchMods: can unlock */
                    | ACTION_LOCK_NO_UNLOCK /* LockMods: cannot unlock */
                    | ACTION_EXPLICIT_UNLOCK_ON_PRESS;
                यदि ((state->flags & STATE_UNLOCK_MODS_ON_PRESS) &&
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
                भङ्ग;
            }
            पक्ष ACTION_TYPE_GROUP_SET:
                यदि (state->base.components.controls & CONTROL_STICKY_किल्ल्या) {
                    /* Convert group set action to a latch */
                    filter->action.type = ACTION_TYPE_GROUP_LATCH;
                    यदि (state->a11y_flags & XKB_A11_Y_STICKY_किल्ल्या_LATCH_TO_LOCK) {
                        filter->action.group.flags |= ACTION_LATCH_TO_LOCK;
                    }
                }
                भङ्ग;
            पक्ष ACTION_TYPE_GROUP_LOCK:
                यदि ((filter->action.group.flags &
                     ACTION_AUTO_GROUP_LOCK_ON_MODIFIER_RELEASE) &&
                    (state->flags &
                     STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_किल्ली) &&
                    /*
                     * FIXME: sticky keys may leave an unexpected modifier
                     * from the level 0 latched, so disable for now.
                     */
                    !(state->base.components.controls & CONTROL_STICKY_किल्ल्या))
                {
                    filter->action.group.flags |= ACTION_LOCK_ON_RELEASE;
                    /* Add base level action */
                    next.मोजणी = xkb_keymap_किल्ली_get_actions_by_level(
                        state->base.keymap, किल्ली, state->base.components.group,
                        0, &next.actions
                    );
                    प्रतिज्ञा(next.मोजणी == 1);
                }
                भङ्ग;
            पक्ष ACTION_TYPE_PTR_MOVE:
            पक्ष ACTION_TYPE_PTR_BUTTON:
            पक्ष ACTION_TYPE_PTR_LOCK:
            पक्ष ACTION_TYPE_PTR_DEFAULT:
                यदि (!(state->base.components.controls & CONTROL_MOUSE_किल्ल्या)) {
                    /* Convert pointer actions to NoAction */
                    प्रतिज्ञा(!filter_action_funcs[ACTION_TYPE_NONE].new);
                    filter->func = रिक्त;
                    अनुवर्त;
                }
                भङ्ग;
            पक्ष ACTION_TYPE_REDIRECT_किल्ली:
                // FIXME: this is not efficient to resolve mods here each time
                filter->action.redirect.affect_mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.affect_mods
                );
                filter->action.redirect.mods = mod_mask_get_effective(
                    state->base.keymap, filter->action.redirect.mods
                );
                भङ्ग;
            स्वभाव: ;
            }
            filter->func = filter_action_funcs[filter->action.type].func;
            filter_action_funcs[filter->action.type].new(state, events, filter);
        }
    }
}

स्थिर शून्य
xkb_state_update_derived(संरचना xkb_state *state);

स्थिर अन्तःस्थ शून्य
xkb_state_init(संरचना xkb_state * restrict state,
               संरचना xkb_keymap * restrict keymap,
               गणना xkb_state_mode_internal mode)
{
    state->mode = mode;
    state->refcnt = 1;
    state->keymap = xkb_keymap_ref(keymap);
    state->out_of_range_group.policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

    /* Ensure that derived state is correctly initialized */
    xkb_state_update_derived(state);
}

स्थिर अन्तःस्थ शून्य
xkb_client_state_init(संरचना xkb_client_state * restrict state,
                      संरचना xkb_keymap * restrict keymap,
                      गणना xkb_state_mode_internal mode)
{
    xkb_state_init(&state->base, keymap, mode);
}

स्थिर अन्तःस्थ शून्य
xkb_server_state_init(संरचना xkb_server_state * restrict state,
                      संरचना xkb_keymap * restrict keymap,
                      गणना xkb_state_mode_internal mode,
                      गणना xkb_a11y_flags a11y_affect,
                      गणना xkb_a11y_flags a11y_flags)
{
    xkb_state_init(&state->base, keymap, mode);

    state->a11y_flags = a11y_flags;
    यदि (keymap->format != XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !(a11y_affect & XKB_A11_Y_LATCH_SIMULTANEOUS_किल्ल्या)) {
             /* Keymap v2+: enable extension to XKB if not manually disabled */
             state->a11y_flags |= XKB_A11_Y_LATCH_SIMULTANEOUS_किल्ल्या;
    }
}

संरचना xkb_state *
xkb_state_new(संरचना xkb_keymap *keymap)
{
    संरचना xkb_server_state * ध्रुव state = calloc(1, परिमाण(*state));
    यदि (!state) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "Could not allocate a state object.\n");
        प्रत्यागम रिक्त;
    }

    xkb_server_state_init(state, keymap, LEGACY_MIXED_STATE, 0, 0);

    प्रत्यागम (संरचना xkb_state *)state;
}

संरचना xkb_state *
xkb_state_new_with_mode(संरचना xkb_keymap * restrict keymap,
                        गणना xkb_state_mode mode,
                        गणना xkb_status * restrict status)
{
    विकल्प (mode) {
    पक्ष XKB_STATE_MODE_CLIENT:
    पक्ष XKB_STATE_MODE_SERVER_QUERY: {
        संरचना xkb_client_state * ध्रुव state = calloc(1, परिमाण(*state));
        यदि (!state)
            भङ्ग;

        static_assert((unsigned)XKB_STATE_MODE_CLIENT ==
                      (unsigned)CLIENT_STATE, "");
        static_assert((unsigned)XKB_STATE_MODE_SERVER_QUERY ==
                      (unsigned)SERVER_COMPANION, "");
        xkb_client_state_init(state, keymap, (गणना xkb_state_mode_internal)mode);

        यदि (status)
            *status = XKB_SUCCESS;

        प्रत्यागम (संरचना xkb_state *)state;
    }
    पक्ष XKB_STATE_MODE_SERVER: {
        संरचना xkb_server_state * ध्रुव state = calloc(1, परिमाण(*state));
        यदि (!state)
            भङ्ग;

        xkb_server_state_init(state, keymap, LEGACY_SERVER_STATE, 0, 0);

        यदि (status)
            *status = XKB_SUCCESS;

        प्रत्यागम (संरचना xkb_state *)state;
    }
    स्वभाव:
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_STATE_MODE_,
                     "Unsupported state mode: %u\n", mode);
        यदि (status)
            *status = XKB_ERROR_UNSUPPORTED_STATE_MODE;
        प्रत्यागम रिक्त;
    }

    log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                  "Could not allocate a state object.\n");
    यदि (status)
        *status = XKB_ERROR_ALLOCATION_FAILURE;
    प्रत्यागम रिक्त;
}

संरचना xkb_state *
xkb_state_ref(संरचना xkb_state *state)
{
    प्रतिज्ञा(state->refcnt > 0);
    state->refcnt++;
    प्रत्यागम state;
}

शून्य
xkb_state_reset(संरचना xkb_state *state)
{
    memset(&state->components, 0, परिमाण(state->components));

    यदि (state->mode < LEGACY_MIXED_STATE) {
        static_assert(परिमाण(*state) == परिमाण(संरचना xkb_client_state),
                      "Not a mere wrapper");
        /* Nothing else to reset */
    } अन्यथा {
        संरचना xkb_server_state *state_ = (संरचना xkb_server_state *)state;
        ध्रुव संरचना xkb_server_state prev = *state_;
        memset(state_, 0, परिमाण(*state_));

        state_->base = prev.base;
        state_->a11y_flags = prev.a11y_flags;
        state_->filters = prev.filters;
        darray_size(state_->filters) = 0;
    }
}

स्थिर अन्तःस्थ शून्य
xkb_state_destroy(संरचना xkb_state *state)
{
    xkb_keymap_unref(state->keymap);
    यदि (state->mode > SERVER_COMPANION)
        darray_free(((संरचना xkb_server_state *)state)->filters);
}

शून्य
xkb_state_unref(संरचना xkb_state *state)
{
    प्रतिज्ञा(!state || state->refcnt > 0);
    यदि (!state || --state->refcnt > 0)
        प्रत्यागम;

    xkb_state_destroy(state);

    विकल्प (state->mode) {
    पक्ष CLIENT_STATE:
    पक्ष SERVER_COMPANION:
        मुक्त((संरचना xkb_client_state *)state);
        भङ्ग;
    पक्ष LEGACY_MIXED_STATE:
    पक्ष LEGACY_SERVER_STATE:
        मुक्त((संरचना xkb_server_state *)state);
        भङ्ग;
    पक्ष SERVER_STATE: /* handled by xkb_machine_unref() */
    स्वभाव:
        PANIC_UNREACHABLE();
    }
}

संरचना xkb_keymap *
xkb_state_get_keymap(संरचना xkb_state *state)
{
    प्रत्यागम state->keymap;
}

/**
 * Update the LED state to match the rest of the xkb_state.
 */
स्थिर शून्य
xkb_state_led_update_all(संरचना xkb_state *state)
{
    xkb_led_index_t idx;
    ध्रुव संरचना xkb_led *led;

    state->components.leds = 0;

    xkb_leds_enumerate(idx, led, state->keymap) {

        यदि (led->which_mods != 0 && led->mods.mask != 0) {
            xkb_mod_mask_t mod_mask = 0;
            यदि (led->which_mods & XKB_STATE_MODS_EFFECTIVE)
                mod_mask |= state->components.mods;
            यदि (led->which_mods & XKB_STATE_MODS_DEPRESSED)
                mod_mask |= state->components.base_mods;
            यदि (led->which_mods & XKB_STATE_MODS_LATCHED)
                mod_mask |= state->components.latched_mods;
            यदि (led->which_mods & XKB_STATE_MODS_LOCKED)
                mod_mask |= state->components.locked_mods;

            यदि (led->mods.mask & mod_mask) {
                state->components.leds |= (UINT32_C(1) << idx);
                अनुवर्त;
            }
        }

        यदि (led->which_groups != 0) {
            यदि (likely(led->groups) != 0) {
                xkb_layout_mask_t group_mask = 0;
                /* Effective and locked groups have been brought into range */
                प्रतिज्ञा(state->components.group < XKB_MAX_GROUPS);
                प्रतिज्ञा(state->components.locked_group >= 0 &&
                       state->components.locked_group < XKB_MAX_GROUPS);
                /* Effective and locked groups are used as mask */
                यदि (led->which_groups & XKB_STATE_LAYOUT_EFFECTIVE)
                    group_mask |= (UINT32_C(1) << state->components.group);
                यदि (led->which_groups & XKB_STATE_LAYOUT_LOCKED)
                    group_mask |= (UINT32_C(1) << state->components.locked_group);
                /* Base and latched groups only have to be non-zero */
                यदि ((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                    state->components.base_group != 0)
                    group_mask |= led->groups;
                यदि ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                    state->components.latched_group != 0)
                    group_mask |= led->groups;

                यदि (led->groups & group_mask) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    अनुवर्त;
                }
            } अन्यथा {
                /* Special case for Base and latched groups */
                यदि (((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                     state->components.base_group == 0) ||
                    ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                     state->components.latched_group == 0)) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    अनुवर्त;
                }
            }
        }

        यदि (led->ctrls & state->components.controls) {
            state->components.leds |= (UINT32_C(1) << idx);
            अनुवर्त;
        }
    }
}

/**
 * Calculates the derived state (effective mods/group and LEDs) from an
 * up-to-date xkb_state.
 */
स्थिर शून्य
xkb_state_update_derived(संरचना xkb_state *state)
{
    xkb_layout_index_t wrapped;

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

स्थिर शून्य
state_update_enabled_controls(संरचना xkb_server_state *state,
                              गणना xkb_keyboard_control_flags affect_controls,
                              गणना xkb_keyboard_control_flags controls,
                              संरचना xkb_events *events)
{
    ध्रुव bool had_sticky_किल्ल्या = state->base.components.controls
                               & CONTROL_STICKY_किल्ल्या;

    /*
     * Enable using the public API with the all the Control values, except
     * the internal ones, if any.
     */
    affect_controls =
        ( affect_controls
        & (गणना xkb_keyboard_control_flags)CONTROL_ALL_BOOLEAN );
    state->base.components.controls &=
        (गणना xkb_action_controls)~affect_controls;
    state->base.components.controls |=
        (गणना xkb_action_controls)(controls & affect_controls);

    यदि (had_sticky_किल्ल्या &&
        !(state->base.components.controls & CONTROL_STICKY_किल्ल्या)) {
        /* Sticky keys were disabled: clear all locks and latches */
        clear_all_latches_and_locks(state, events);
    }

    xkb_state_update_derived(&state->base);
}

/**
 * Given a particular key event, updates the state structure to reflect the
 * new modifiers.
 */
गणना xkb_state_component
xkb_state_update_किल्ली(संरचना xkb_state *base_state, xkb_keycode_t kc,
                     गणना xkb_किल्ली_direction direction)
{
    /* Guard against client-only state */
    प्रतिज्ञा(base_state->mode > SERVER_COMPANION);
    यदि (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        प्रत्यागम 0;
    }

    संरचना xkb_server_state * ध्रुव state =
        (संरचना xkb_server_state *)base_state;

    ध्रुव संरचना xkb_किल्ली* ध्रुव किल्ली = Xkb_किल्ली(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    यदि (!किल्ली || (direction == XKB_किल्ली_REPEATED && !किल्ली->repeats))
        प्रत्यागम 0;

    यदि (direction == XKB_किल्ली_DOWN &&
        (state->base.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_किल्ल्या) &&
        (state->a11y_flags & XKB_A11_Y_STICKY_किल्ल्या_NO_SIMULTANEOUS_किल्ल्या) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_किल्ल्या,
                                      0, रिक्त);
    }

    ध्रुव संरचना state_components prev_components = state->base.components;

    state->set_mods = 0;
    state->clear_mods = 0;

    xkb_filter_apply_all(state, रिक्त, किल्ली, direction);

    xkb_mod_index_t i;
    xkb_mod_mask_t bit;
    प्रति (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        यदि (state->set_mods & bit) {
            state->mod_किल्ली_मोजणी[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    प्रति (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        यदि (state->clear_mods & bit) {
            state->mod_किल्ली_मोजणी[i]--;
            यदि (state->mod_किल्ली_मोजणी[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_किल्ली_मोजणी[i] = 0;
            }
            state->clear_mods &= ~bit;
        }
    }

    xkb_state_update_derived(&state->base);

    प्रत्यागम get_state_component_changes(&prev_components, &state->base.components);
}

/* We need fake keys for `update_latch_modifiers` and `update_latch_group`.
 * These keys must have at least one level in order to break latches. We need 2
 * keys with specific actions in order to update group/mod latches without
 * affecting each other. */
स्थिर संरचना xkb_किल्ली_type_entry synthetic_किल्ली_level_entry = { 0 };
स्थिर संरचना xkb_किल्ली_type synthetic_किल्ली_type = {
     .अंक_entries = 1,
     .अंक_levels = 1,
     .entries = &synthetic_किल्ली_level_entry
};
स्थिर ध्रुव संरचना xkb_किल्ली synthetic_किल्ली = { 0 };

/* Transcription from xserver: XkbLatchModifiers */
स्थिर शून्य
update_latch_modifiers(संरचना xkb_server_state *state,
                       संरचना xkb_events *events,
                       xkb_mod_mask_t mask, xkb_mod_mask_t latches)
{
    /* Clear affected latched modifiers */
    ध्रुव xkb_mod_mask_t clear = mask & ~latches;
    state->base.components.latched_mods &= ~clear;

    /* Clear any pending latch to locks using ad hoc action:
     * only affect corresponding modifier latches and no group latch. */
    संरचना xkb_level synthetic_किल्ली_level_break_mod_latch = {
        .अंक_syms = 0,
        .अंक_actions = 1,
        .upper = XKB_किल्ली_No_चिन्ह,
        .s.sym = XKB_किल्ली_No_चिन्ह,
        .a.action.internal = {
            .type = ACTION_TYPE_INTERNAL,
            .flags = INTERNAL_BREAKS_MOD_LATCH,
            .clear_latched_mods = clear
        }
    };
    संरचना xkb_group synthetic_किल्ली_group_break_mod_latch = {
        .type = &synthetic_किल्ली_type,
        .levels = &synthetic_किल्ली_level_break_mod_latch
    };
    ध्रुव संरचना xkb_किल्ली synthetic_किल्ली_break_mod_latch = {
        .अंक_groups = 1,
        .groups = &synthetic_किल्ली_group_break_mod_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_किल्ली_break_mod_latch, XKB_किल्ली_DOWN);

    /* Finally set the latched mods by simulate tapping a key with the
     * corresponding action */
    ध्रुव संरचना xkb_किल्ली* ध्रुव किल्ली = &synthetic_किल्ली;
    ध्रुव सङ्घ xkb_action latch_mods = {
        .mods = {
            .type = ACTION_TYPE_MOD_LATCH,
            .mods = { .mask = mask & latches },
            .flags = 0,
        },
    };
    संरचना xkb_filter* ध्रुव filter = xkb_filter_new(state);
    filter->किल्ली = किल्ली;
    filter->func = xkb_filter_mod_latch_func;
    filter->action = latch_mods;
    xkb_filter_mod_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_mod_latch_func(state, events, filter, किल्ली, XKB_किल्ली_UP);
}

/* Transcription from xserver: XkbLatchGroup */
स्थिर शून्य
update_latch_group(संरचना xkb_server_state *state,
                   संरचना xkb_events *events, int32_t group)
{
    /* Clear any pending latch to locks. */
    स्थिर संरचना xkb_level synthetic_किल्ली_level_break_group_latch = {
        .अंक_syms = 0,
        .अंक_actions = 1,
        .upper = XKB_किल्ली_No_चिन्ह,
        .s.sym = XKB_किल्ली_No_चिन्ह,
        .a.action.internal = {
            .type = ACTION_TYPE_INTERNAL,
            .flags = INTERNAL_BREAKS_GROUP_LATCH,
            .clear_latched_mods = 0
        }
    };
    स्थिर संरचना xkb_group synthetic_किल्ली_group_break_group_latch = {
        .type = &synthetic_किल्ली_type,
        .levels = &synthetic_किल्ली_level_break_group_latch
    };
    स्थिर ध्रुव संरचना xkb_किल्ली synthetic_किल्ली_break_group_latch = {
        .अंक_groups = 1,
        .groups = &synthetic_किल्ली_group_break_group_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_किल्ली_break_group_latch, XKB_किल्ली_DOWN);

    /* Simulate tapping a key with a group latch action, but in isolation: i.e.
     * without affecting the other filters. */
    ध्रुव संरचना xkb_किल्ली* ध्रुव किल्ली = &synthetic_किल्ली;
    ध्रुव सङ्घ xkb_action latch_group = {
        .group = {
            .type = ACTION_TYPE_GROUP_LATCH,
            .flags = ACTION_ABSOLUTE_SWITCH,
            .group = group,
        },
    };
    संरचना xkb_filter* ध्रुव filter = xkb_filter_new(state);
    filter->किल्ली = किल्ली;
    filter->func = xkb_filter_group_latch_func;
    filter->action = latch_group;
    xkb_filter_group_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_group_latch_func(state, events, filter, किल्ली, XKB_किल्ली_UP);
}

/**
 * Compute the resolved effective mask of an arbitrary input.
 *
 * Contrary to `mod_mask_get_effective`, it resolves only modifiers not present
 * in the canonical mask, so that it enables `xkb_state_serialize_mods` to
 * round trip via `xkb_state_update_mask`.
 */
स्थिर अन्तःस्थ xkb_mod_mask_t
resolve_to_canonical_mods(संरचना xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    प्रत्यागम
        /*
         * Keep canonical modifier mask.
         * It contains either real modifiers or canonical virtual modifiers.
         */
        (mods & keymap->canonical_state_mask) |
        /* Resolve other modifiers */
        mod_mask_get_effective(keymap,
                               mods & ~keymap->canonical_state_mask);
}

स्थिर शून्य
state_update_latched_locked(
    संरचना xkb_server_state * restrict state,
    ध्रुव संरचना xkb_state_components_update * restrict update,
    संरचना xkb_events * restrict events
)
{
    /* Update locks */
    ध्रुव xkb_mod_mask_t affect_locked_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_locked_mods);
    यदि ((update->components & XKB_STATE_MODS_LOCKED) &&
        affect_locked_mods) {
        ध्रुव xkb_mod_mask_t locked_mods =
            resolve_to_canonical_mods(state->base.keymap, update->locked_mods);
        state->base.components.locked_mods &= ~affect_locked_mods;
        state->base.components.locked_mods |= locked_mods & affect_locked_mods;
    }
    यदि (update->components & XKB_STATE_LAYOUT_LOCKED) {
        state->base.components.locked_group = update->locked_layout;
    }

    /* Update latches */
    ध्रुव xkb_mod_mask_t affect_latched_mods =
        resolve_to_canonical_mods(state->base.keymap, update->affect_latched_mods);
    यदि ((update->components & XKB_STATE_MODS_LATCHED) &&
        affect_latched_mods) {
        ध्रुव xkb_mod_mask_t latched_mods =
            resolve_to_canonical_mods(state->base.keymap, update->latched_mods);
        update_latch_modifiers(state, events, affect_latched_mods, latched_mods);
    }
    यदि (update->components & XKB_STATE_LAYOUT_LATCHED) {
        update_latch_group(state, events, update->latched_layout);
    }
}

गणना xkb_state_component
xkb_state_update_latched_locked(संरचना xkb_state *base_state,
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
    प्रतिज्ञा(base_state->mode > SERVER_COMPANION);
    यदि (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        प्रत्यागम 0;
    }

    संरचना xkb_server_state * ध्रुव state =
        (संरचना xkb_server_state *)base_state;

    ध्रुव संरचना state_components previous_components = state->base.components;
    ध्रुव गणना xkb_state_component components
        = ((affect_latched_mods || latched_mods) ? XKB_STATE_MODS_LATCHED : 0)
        | ((affect_locked_mods || locked_mods) ? XKB_STATE_MODS_LOCKED : 0)
        | (affect_latched_layout ? XKB_STATE_LAYOUT_LATCHED : 0)
        | (affect_locked_layout ? XKB_STATE_LAYOUT_LOCKED : 0);
    ध्रुव संरचना xkb_state_components_update update = {
        .size = परिमाण(update),
        .components = components,
        .affect_latched_mods = affect_latched_mods,
        .latched_mods = latched_mods,
        .latched_layout = latched_layout,
        .affect_locked_mods = affect_locked_mods,
        .locked_mods = locked_mods,
        .locked_layout = locked_layout
    };
    state_update_latched_locked(state, &update, रिक्त);
    xkb_state_update_derived(&state->base);
    प्रत्यागम get_state_component_changes(&previous_components,
                                       &state->base.components);
}

स्थिर अन्तःस्थ शून्य
clear_all_latches_and_locks(संरचना xkb_server_state *state,
                            संरचना xkb_events *events)
{
    स्थिर ध्रुव गणना xkb_state_component components
        = XKB_STATE_MODS_LATCHED | XKB_STATE_MODS_LOCKED
        | XKB_STATE_LAYOUT_LATCHED | XKB_STATE_LAYOUT_LOCKED;
    ध्रुव संरचना xkb_state_components_update update = {
        .size = परिमाण(update),
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

स्थिर गणना xkb_status
state_update_layout_policy(संरचना xkb_server_state *state,
                           ध्रुव संरचना xkb_layout_policy_update *update)
{
    यदि (xkb_feature_supported(XKB_FEATURE_ENUM_LAYOUT_OUT_OF_RANGE_POLICY,
                              update->policy)) {
        यदि (update->policy == XKB_LAYOUT_OUT_OF_RANGE_REDIRECT) {
            यदि (update->redirect < state->base.keymap->अंक_groups) {
                state->base.out_of_range_group.redirect_group =
                    update->redirect;
            } अन्यथा {
                log_err(state->base.keymap->ctx,
                        XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Layout policy: "
                        "unsupported layout index %"PRIu32" > %"PRIu32"\n",
                        update->redirect + 1, state->base.keymap->अंक_groups);
                प्रत्यागम XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
            }
        }
        state->base.out_of_range_group.policy = update->policy;
        प्रत्यागम XKB_SUCCESS;
    } अन्यथा {
        log_err(state->base.keymap->ctx,
                XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY_,
                "Unsupported layout policy: %d\n", update->policy);
        प्रत्यागम XKB_ERROR_UNSUPPORTED_LAYOUT_OUT_OF_RANGE_POLICY;
    }
}

/** Check ABI compatibility */
// NOLINTBEGIN(bugprone-assignment-in-selection-statement)
स्थिर गणना xkb_status
check_state_update_abi_(संरचना xkb_context * restrict ctx,
                        ध्रुव वर्ण * restrict func,
                        ध्रुव संरचना xkb_synthetic_update * restrict update)
{
    गणना xkb_status status = XKB_SUCCESS;
    यदि ((status = xkb_check_state_abi(update)) ||
        (update->reserved0 != 0 &&
         (status = XKB_ERROR_ABI_FORWARD_COMPAT)) ||
        (update->components &&
         (status = xkb_check_state_abi(update->components))) ||
        (update->layout_policy &&
         (status = xkb_check_state_abi(update->layout_policy)))) {
        xkb_log_abi_error(ctx, func, status);
    }
    प्रत्यागम status;
}
// NOLINTEND(bugprone-assignment-in-selection-statement)

#define check_state_update_abi(ctx, update) \
    check_state_update_abi_(ctx, __func__, update)

गणना xkb_status
xkb_state_update_synthetic(संरचना xkb_state * base_state,
                           ध्रुव संरचना xkb_synthetic_update * update,
                           गणना xkb_state_component *changed)
{
    /* Guard against client-only state */
    यदि (base_state->mode <= SERVER_COMPANION) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        प्रत्यागम XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    संरचना xkb_server_state * ध्रुव state =
        (संरचना xkb_server_state *)base_state;

    /* Check ABI compatibility */
    गणना xkb_status status = check_state_update_abi(state->base.keymap->ctx,
                                                    update);
    यदि (status != XKB_SUCCESS)
        प्रत्यागम status;

    ध्रुव संरचना state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    /* Update parametrized controls first */
    यदि (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        यदि (status != XKB_SUCCESS)
            प्रत्यागम status;
    }

    यदि (update->components) {
        ध्रुव संरचना xkb_state_components_update * ध्रुव components =
            update->components;
        /* Update boolean controls first */
        यदि (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, रिक्त);
        }

        state_update_latched_locked(state, components, रिक्त);
    }

    xkb_state_update_derived(&state->base);

    यदि (changed) {
        *changed = get_state_component_changes(&previous_components,
                                               &state->base.components);
    }
    प्रत्यागम XKB_SUCCESS;
}

/**
 * Updates the state from a set of explicit masks as gained from
 * xkb_state_serialize_mods and xkb_state_serialize_groups.  As noted in the
 * documentation for these functions in xkbcommon.h, this round-trip is
 * lossy, and should only be used to update a slave state mirroring the
 * master, e.g. in a client/server window system.
 */
गणना xkb_state_component
xkb_state_update_mask(संरचना xkb_state *base_state,
                      xkb_mod_mask_t base_mods,
                      xkb_mod_mask_t latched_mods,
                      xkb_mod_mask_t locked_mods,
                      xkb_layout_index_t base_group,
                      xkb_layout_index_t latched_group,
                      xkb_layout_index_t locked_group)
{
    /* Guard against server-only and server companion state */
    प्रतिज्ञा(base_state->mode == CLIENT_STATE ||
           base_state->mode == LEGACY_MIXED_STATE);
    यदि (base_state->mode != CLIENT_STATE &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        प्रत्यागम 0;
    }

    संरचना xkb_client_state * ध्रुव state =
        (संरचना xkb_client_state *)base_state;

    ध्रुव संरचना state_components prev_components = state->base.components;

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

    प्रत्यागम get_state_component_changes(&prev_components, &state->base.components);
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Lock_Modifier
 */
स्थिर bool
should_do_caps_transformation(संरचना xkb_state *state, xkb_keycode_t kc)
{
    प्रत्यागम
        xkb_state_mod_index_is_active(state, XKB_MOD_INDEX_CAPS,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_index_is_consumed(state, kc, XKB_MOD_INDEX_CAPS) == 0;
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Control_Modifier
 */
स्थिर bool
should_do_ctrl_transformation(संरचना xkb_state *state, xkb_keycode_t kc)
{
    प्रत्यागम
        xkb_state_mod_index_is_active(state, XKB_MOD_INDEX_CTRL,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_index_is_consumed(state, kc, XKB_MOD_INDEX_CTRL) == 0;
}

/**
 * Provides the symbols to use for the given key and state.  Returns the
 * number of symbols pointed to in syms_out.
 */
पूर्णाङ्क
xkb_state_किल्ली_get_syms(संरचना xkb_state *state, xkb_keycode_t kc,
                       ध्रुव xkb_keysym_t **syms_out)
{
    ध्रुव xkb_layout_index_t layout = xkb_state_किल्ली_get_layout(state, kc);
    यदि (layout == XKB_LAYOUT_INVALID)
        गच्छ err;

    ध्रुव xkb_level_index_t level = xkb_state_किल्ली_get_level(state, kc, layout);
    यदि (level == XKB_LEVEL_INVALID)
        गच्छ err;

    ध्रुव संरचना xkb_किल्ली* ध्रुव किल्ली = Xkb_किल्ली(state->keymap, kc);

    यदि (!किल्ली)
        गच्छ err;

    ध्रुव संरचना xkb_level* ध्रुव leveli =
        xkb_keymap_किल्ली_get_level(state->keymap, किल्ली, layout, level);

    यदि (!leveli)
        गच्छ err;

    ध्रुव xkb_keysym_मोजणी_t अंक_syms = leveli->अंक_syms;
    यदि (अंक_syms == 0)
        गच्छ err;

    यदि (should_do_caps_transformation(state, kc)) {
        /* Only simple capitalization rules: keysyms count is unchanged. */
        यदि (अंक_syms > 1) {
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
    प्रत्यागम (पूर्णाङ्क) अंक_syms;

err:
    *syms_out = रिक्त;
    प्रत्यागम 0;
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
स्थिर वर्ण
XkbToControl(वर्ण ch)
{
    वर्ण c = ch;

    यदि ((c >= '@' && c < '\177') || c == ' ')
        c &= 0x1F;
    अन्यथा यदि (c == '2')
        c = '\000';
    अन्यथा यदि (c >= '3' && c <= '7')
        c -= ('3' - '\033');
    अन्यथा यदि (c == '8')
        c = '\177';
    अन्यथा यदि (c == '/')
        c = '_' & 0x1F;
    प्रत्यागम c;
}

/**
 * Provides either exactly one symbol, or XKB_KEY_NoSymbol.
 */
xkb_keysym_t
xkb_state_किल्ली_get_one_sym(संरचना xkb_state *state, xkb_keycode_t kc)
{
    ध्रुव xkb_keysym_t *syms = रिक्त;
    ध्रुव पूर्णाङ्क अंक_syms = xkb_state_किल्ली_get_syms(state, kc, &syms);

    यदि (अंक_syms != 1)
        प्रत्यागम XKB_किल्ली_No_चिन्ह;
    अन्यथा
        प्रत्यागम syms[0];
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
स्थिर xkb_keysym_t
get_one_sym_for_string(संरचना xkb_state *state, xkb_keycode_t kc)
{
    ध्रुव xkb_layout_index_t layout = xkb_state_किल्ली_get_layout(state, kc);
    ध्रुव xkb_layout_index_t अंक_layouts =
        xkb_keymap_अंक_layouts_for_किल्ली(state->keymap, kc);
    xkb_level_index_t level = xkb_state_किल्ली_get_level(state, kc, layout);
    यदि (layout == XKB_LAYOUT_INVALID || अंक_layouts == 0 ||
        level == XKB_LEVEL_INVALID)
        प्रत्यागम XKB_किल्ली_No_चिन्ह;

    ध्रुव xkb_keysym_t *syms = रिक्त;
    पूर्णाङ्क nsyms = xkb_keymap_किल्ली_get_syms_by_level(state->keymap, kc,
                                                 layout, level, &syms);
    यदि (nsyms != 1)
        प्रत्यागम XKB_किल्ली_No_चिन्ह;
    xkb_keysym_t sym = syms[0];

    यदि (should_do_ctrl_transformation(state, kc) && sym > 127u) {
        प्रति (xkb_layout_index_t i = 0; i < अंक_layouts; i++) {
            level = xkb_state_किल्ली_get_level(state, kc, i);
            यदि (level == XKB_LEVEL_INVALID)
                अनुवर्त;

            nsyms = xkb_keymap_किल्ली_get_syms_by_level(state->keymap, kc,
                                                     i, level, &syms);
            यदि (nsyms == 1 && syms[0] <= 127u) {
                sym = syms[0];
                भङ्ग;
            }
        }
    }

    यदि (should_do_caps_transformation(state, kc)) {
        sym = xkb_keysym_to_upper(sym);
    }

    प्रत्यागम sym;
}

पूर्णाङ्क
xkb_state_किल्ली_get_utf8(संरचना xkb_state *state, xkb_keycode_t kc,
                       वर्ण *buffer, size_t size)
{
    पूर्णाङ्क nsyms;
    ध्रुव xkb_keysym_t *syms = रिक्त;
    ध्रुव xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    यदि (sym != XKB_किल्ली_No_चिन्ह) {
        nsyms = 1; syms = &sym;
    }
    अन्यथा {
        nsyms = xkb_state_किल्ली_get_syms(state, kc, &syms);
    }

    /* Make sure not to truncate in the middle of a UTF-8 sequence. */
    पूर्णाङ्क offset = 0;
    वर्ण tmp[XKB_KEYSYM_UTF8_MAX_SIZE];
    प्रति (पूर्णाङ्क i = 0; i < nsyms; i++) {
        पूर्णाङ्क ret = xkb_keysym_to_utf8(syms[i], tmp, परिमाण(tmp));
        यदि (ret <= 0)
            गच्छ err_bad;

        ret--;
        यदि ((size_t)offset + (size_t)ret <= size)
            memcpy(buffer + offset, tmp, (size_t)ret);
        offset += ret;
    }

    यदि ((size_t)offset >= size)
        गच्छ err_trunc;
    buffer[offset] = '\0';

    यदि (!is_valid_utf8(buffer, (size_t)offset))
        गच्छ err_bad;

    यदि (offset == 1 && (unsigned पूर्णाङ्क) buffer[0] <= 127u &&
        should_do_ctrl_transformation(state, kc))
        buffer[0] = XkbToControl(buffer[0]);

    प्रत्यागम offset;

err_trunc:
    यदि (size > 0)
        buffer[size - 1] = '\0';
    प्रत्यागम offset;

err_bad:
    यदि (size > 0)
        buffer[0] = '\0';
    प्रत्यागम 0;
}

uint32_t
xkb_state_किल्ली_get_utf32(संरचना xkb_state *state, xkb_keycode_t kc)
{
    ध्रुव xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    uint32_t cp = xkb_keysym_to_utf32(sym);

    यदि (cp <= 127u && should_do_ctrl_transformation(state, kc))
        cp = (uint32_t) XkbToControl((वर्ण) cp);

    प्रत्यागम cp;
}

/**
 * Serializes the requested modifier state into an xkb_mod_mask_t, with all
 * the same disclaimers as in xkb_state_update_mask.
 */
स्थिर अन्तःस्थ xkb_mod_mask_t
serialize_mods(ध्रुव संरचना state_components *components,
               गणना xkb_state_component type)
{
    xkb_mod_mask_t ret = 0;

    यदि (type & XKB_STATE_MODS_EFFECTIVE)
        प्रत्यागम components->mods;

    यदि (type & XKB_STATE_MODS_DEPRESSED)
        ret |= components->base_mods;
    यदि (type & XKB_STATE_MODS_LATCHED)
        ret |= components->latched_mods;
    यदि (type & XKB_STATE_MODS_LOCKED)
        ret |= components->locked_mods;

    प्रत्यागम ret;
}

xkb_mod_mask_t
xkb_state_serialize_mods(संरचना xkb_state *state,
                         गणना xkb_state_component type)
{
    प्रत्यागम serialize_mods(&state->components, type);
}

/**
 * Serialises the requested group state, with all the same disclaimers as
 * in xkb_state_update_mask.
 */
स्थिर अन्तःस्थ xkb_layout_index_t
serialize_layout(ध्रुव संरचना state_components *components,
                 गणना xkb_state_component type)
{
    यदि (type & XKB_STATE_LAYOUT_EFFECTIVE)
        प्रत्यागम components->group;

    int32_t ret = 0;
    यदि (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret += components->base_group;
    यदि (type & XKB_STATE_LAYOUT_LATCHED)
        ret += components->latched_group;
    यदि (type & XKB_STATE_LAYOUT_LOCKED)
        ret += components->locked_group;

    प्रत्यागम (xkb_layout_index_t)ret;
}

xkb_layout_index_t
xkb_state_serialize_layout(संरचना xkb_state *state,
                           गणना xkb_state_component type)
{
    प्रत्यागम serialize_layout(&state->components, type);
}

xkb_led_mask_t
xkb_state_serialize_leds(ध्रुव संरचना xkb_state *state,
                         गणना xkb_state_component type)
{
    प्रत्यागम (type & XKB_STATE_LEDS)
        ? state->components.leds
        : 0;
}

स्थिर अन्तःस्थ गणना xkb_keyboard_control_flags
serialize_controls(ध्रुव संरचना state_components *components,
                   गणना xkb_state_component type)
{
    प्रत्यागम (type & XKB_STATE_CONTROLS_EFFECTIVE)
        /*
         * Enable using the public API with the all the Controls values, except
         * the internal ones, if any.
         */
        ? (गणना xkb_keyboard_control_flags)
          (components->controls & CONTROL_ALL_BOOLEAN)
        : 0;
}

गणना xkb_keyboard_control_flags
xkb_state_serialize_controls(ध्रुव संरचना xkb_state *state,
                             गणना xkb_state_component type)
{
    प्रत्यागम serialize_controls(&state->components, type);
}

/**
 * Gets a modifier mask and returns the resolved effective mask; this
 * is needed because some modifiers can also map to other modifiers, e.g.
 * the "NumLock" modifier usually also sets the "Mod2" modifier.
 */
xkb_mod_mask_t
mod_mask_get_effective(संरचना xkb_keymap *keymap, xkb_mod_mask_t mods)
{
    /* Initialize the effective mask with its corresponding real mods. */
    xkb_mod_mask_t mask = mods & MOD_REAL_MASK_ALL;

    /* Resolve the virtual modifiers */
    ध्रुव संरचना xkb_mod *mod;
    xkb_mod_index_t i;
    xkb_vmods_enumerate(i, mod, &keymap->mods)
        यदि (mods & (UINT32_C(1) << i))
            mask |= mod->mapping;

    प्रत्यागम mask;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
पूर्णाङ्क
xkb_state_mod_index_is_active(संरचना xkb_state *state,
                              xkb_mod_index_t idx,
                              गणना xkb_state_component type)
{
    यदि (unlikely(idx >= xkb_keymap_अंक_mods(state->keymap)))
        प्रत्यागम -1;

    ध्रुव xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    यदि (!mapping) {
        /* Modifier not mapped */
        प्रत्यागम 0;
    }
    /* WARNING: this may overmatch for virtual modifiers */
    प्रत्यागम (xkb_state_serialize_mods(state, type) & mapping) == mapping;
}

गणना {
    XKB_STATE_MATCH_FLAGS = XKB_STATE_MATCH_ANY
                          | XKB_STATE_MATCH_ALL
                          | XKB_STATE_MATCH_NON_EXCLUSIVE
};

/**
 * Helper function for xkb_state_mod_indices_are_active and
 * xkb_state_mod_names_are_active.
 */
स्थिर bool
match_mod_masks(संरचना xkb_state *state,
                गणना xkb_state_component type,
                गणना xkb_state_match match,
                xkb_mod_mask_t wanted)
{
    ध्रुव xkb_mod_mask_t active = xkb_state_serialize_mods(state, type);

    यदि (!(match & XKB_STATE_MATCH_NON_EXCLUSIVE) && (active & ~wanted))
        प्रत्यागम false;

    यदि (match & XKB_STATE_MATCH_ANY)
        प्रत्यागम active & wanted;

    /* Ensure all flags but XKB_STATE_MATCH_ALL are processed before */
    static_assert(XKB_STATE_MATCH_ALL ==
                  ((गणना xkb_state_match) XKB_STATE_MATCH_FLAGS &
                   ~(XKB_STATE_MATCH_ANY | XKB_STATE_MATCH_NON_EXCLUSIVE)),
                  "");

    प्रत्यागम (active & wanted) == wanted;
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, or -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
पूर्णाङ्क
xkb_state_mod_indices_are_active(संरचना xkb_state *state,
                                 गणना xkb_state_component type,
                                 गणना xkb_state_match match,
                                 ...)
{
    यदि (match & ~((गणना xkb_state_match) XKB_STATE_MATCH_FLAGS))
        प्रत्यागम -2;

    va_list ap;
    xkb_mod_mask_t wanted = 0;
    पूर्णाङ्क ret = 0;
    ध्रुव xkb_mod_index_t अंक_mods = xkb_keymap_अंक_mods(state->keymap);

    va_start(ap, match);
    यावत् (1) {
        xkb_mod_index_t idx = va_arg(ap, xkb_mod_index_t);
        यदि (idx == XKB_MOD_INVALID)
            भङ्ग;
        यदि (unlikely(idx >= अंक_mods)) {
            ret = -1;
            भङ्ग;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_end(ap);

    यदि (ret == -1)
        प्रत्यागम ret;

    यदि (!wanted) {
        /* Modifiers not mapped */
        प्रत्यागम 0;
    }

    प्रत्यागम match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
पूर्णाङ्क
xkb_state_mod_name_is_active(संरचना xkb_state *state, ध्रुव वर्ण *name,
                             गणना xkb_state_component type)
{
    ध्रुव xkb_mod_index_t idx = xkb_keymap_mod_get_index(state->keymap, name);

    यदि (idx == XKB_MOD_INVALID)
        प्रत्यागम -1;

    प्रत्यागम xkb_state_mod_index_is_active(state, idx, type);
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ATTR_NULL_SENTINEL पूर्णाङ्क
xkb_state_mod_names_are_active(संरचना xkb_state *state,
                               गणना xkb_state_component type,
                               गणना xkb_state_match match,
                               ...)
{
    यदि (match & ~((गणना xkb_state_match) XKB_STATE_MATCH_FLAGS))
        प्रत्यागम -2;

    va_list ap;
    xkb_mod_mask_t wanted = 0;
    पूर्णाङ्क ret = 0;

    va_start(ap, match);
    यावत् (1) {
        ध्रुव वर्ण *str = va_arg(ap, ध्रुव वर्ण *);
        यदि (str == रिक्त)
            भङ्ग;
        ध्रुव xkb_mod_index_t idx = xkb_keymap_mod_get_index(state->keymap, str);
        यदि (idx == XKB_MOD_INVALID) {
            ret = -1;
            भङ्ग;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_end(ap);

    यदि (ret == -1)
        प्रत्यागम ret;

    यदि (!wanted) {
        /* Modifiers not mapped */
        प्रत्यागम 0;
    }

    प्रत्यागम match_mod_masks(state, type, match, wanted);
}

/**
 * Returns 1 if the given group is active with the specified type(s), 0 if
 * not, or -1 if the group is invalid.
 */
पूर्णाङ्क
xkb_state_layout_index_is_active(संरचना xkb_state *state,
                                 xkb_layout_index_t idx,
                                 गणना xkb_state_component type)
{
    यदि (idx >= state->keymap->अंक_groups)
        प्रत्यागम -1;

    पूर्णाङ्क ret = 0;
    यदि (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ret |= (state->components.group == idx);
    यदि (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret |= (state->components.base_group == (int32_t) idx);
    यदि (type & XKB_STATE_LAYOUT_LATCHED)
        ret |= (state->components.latched_group == (int32_t) idx);
    यदि (type & XKB_STATE_LAYOUT_LOCKED)
        ret |= (state->components.locked_group == (int32_t) idx);

    प्रत्यागम ret;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
पूर्णाङ्क
xkb_state_layout_name_is_active(संरचना xkb_state *state, ध्रुव वर्ण *name,
                                गणना xkb_state_component type)
{
    ध्रुव xkb_layout_index_t idx =
        xkb_keymap_layout_get_index(state->keymap, name);

    यदि (idx == XKB_LAYOUT_INVALID)
        प्रत्यागम -1;

    प्रत्यागम xkb_state_layout_index_is_active(state, idx, type);
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
पूर्णाङ्क
xkb_state_led_index_is_active(संरचना xkb_state *state, xkb_led_index_t idx)
{
    यदि (idx >= state->keymap->अंक_leds ||
        state->keymap->leds[idx].name == XKB_ATOM_NONE)
        प्रत्यागम -1;

    प्रत्यागम !!(state->components.leds & (UINT32_C(1) << idx));
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
पूर्णाङ्क
xkb_state_led_name_is_active(संरचना xkb_state *state, ध्रुव वर्ण *name)
{
    ध्रुव xkb_led_index_t idx = xkb_keymap_led_get_index(state->keymap, name);

    यदि (idx == XKB_LED_INVALID)
        प्रत्यागम -1;

    प्रत्यागम xkb_state_led_index_is_active(state, idx);
}

/**
 * See:
 * - XkbTranslateKeyCode(3), mod_rtrn return value, from libX11.
 * - MyEnhancedXkbTranslateKeyCode(), a modification of the above, from GTK+.
 */
स्थिर xkb_mod_mask_t
किल्ली_get_consumed(संरचना xkb_state *state, ध्रुव संरचना xkb_किल्ली *किल्ली,
                 गणना xkb_consumed_mode mode)
{
    ध्रुव xkb_layout_index_t group =
        xkb_state_किल्ली_get_layout(state, किल्ली->keycode);
    यदि (group == XKB_LAYOUT_INVALID)
        प्रत्यागम 0;

    xkb_mod_mask_t preserve = 0;
    xkb_mod_mask_t consumed = 0;

    ध्रुव संरचना xkb_किल्ली_type_entry* ध्रुव matching_entry =
        get_entry_for_किल्ली_state(state, किल्ली, group);
    यदि (matching_entry)
        preserve = matching_entry->preserve.mask;

    ध्रुव संरचना xkb_किल्ली_type* ध्रुव type = किल्ली->groups[group].type;
    विकल्प (mode) {
    पक्ष XKB_CONSUMED_MODE_XKB:
        consumed = type->mods.mask;
        भङ्ग;

    पक्ष XKB_CONSUMED_MODE_GTK: {
        ध्रुव संरचना xkb_किल्ली_type_entry* ध्रुव no_mods_entry =
            get_entry_for_mods(type, 0);
        ध्रुव xkb_level_index_t no_mods_leveli = no_mods_entry
                                               ? no_mods_entry->level
                                               : 0;
        ध्रुव संरचना xkb_level* ध्रुव no_mods_level =
            &किल्ली->groups[group].levels[no_mods_leveli];

        प्रति (darray_size_t i = 0; i < type->अंक_entries; i++) {
            ध्रुव संरचना xkb_किल्ली_type_entry* ध्रुव entry = &type->entries[i];
            यदि (!entry_is_active(entry))
                अनुवर्त;

            ध्रुव संरचना xkb_level* ध्रुव level =
                &किल्ली->groups[group].levels[entry->level];
            यदि (XkbLevelsSameSyms(level, no_mods_level))
                अनुवर्त;

            यदि (entry == matching_entry || one_bit_set(entry->mods.mask))
                consumed |= entry->mods.mask & ~entry->preserve.mask;
        }
        भङ्ग;
    }
    }

    प्रत्यागम consumed & ~preserve;
}

पूर्णाङ्क
xkb_state_mod_index_is_consumed2(संरचना xkb_state *state, xkb_keycode_t kc,
                                 xkb_mod_index_t idx,
                                 गणना xkb_consumed_mode mode)
{
    ध्रुव संरचना xkb_किल्ली* ध्रुव किल्ली = Xkb_किल्ली(state->keymap, kc);

    यदि (unlikely(!किल्ली || idx >= xkb_keymap_अंक_mods(state->keymap)))
        प्रत्यागम -1;

    ध्रुव xkb_mod_mask_t mapping = state->keymap->mods.mods[idx].mapping;
    यदि (!mapping) {
        /* Modifier not mapped */
        प्रत्यागम 0;
    }
    प्रत्यागम (mapping & किल्ली_get_consumed(state, किल्ली, mode)) == mapping;
}

पूर्णाङ्क
xkb_state_mod_index_is_consumed(संरचना xkb_state *state, xkb_keycode_t kc,
                                xkb_mod_index_t idx)
{
    प्रत्यागम xkb_state_mod_index_is_consumed2(state, kc, idx,
                                            XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_mod_mask_remove_consumed(संरचना xkb_state *state, xkb_keycode_t kc,
                                   xkb_mod_mask_t mask)
{
    ध्रुव संरचना xkb_किल्ली* ध्रुव किल्ली = Xkb_किल्ली(state->keymap, kc);

    यदि (!किल्ली)
        प्रत्यागम 0;

    प्रत्यागम resolve_to_canonical_mods(state->keymap, mask) &
           ~किल्ली_get_consumed(state, किल्ली, XKB_CONSUMED_MODE_XKB);
}

xkb_mod_mask_t
xkb_state_किल्ली_get_consumed_mods2(संरचना xkb_state *state, xkb_keycode_t kc,
                                 गणना xkb_consumed_mode mode)
{
    विकल्प (mode) {
    पक्ष XKB_CONSUMED_MODE_XKB:
    पक्ष XKB_CONSUMED_MODE_GTK:
        भङ्ग;
    स्वभाव:
        log_err_func(state->keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "unrecognized consumed modifiers mode: %d\n", mode);
        प्रत्यागम 0;
    }

    ध्रुव संरचना xkb_किल्ली* ध्रुव किल्ली = Xkb_किल्ली(state->keymap, kc);
    यदि (!किल्ली)
        प्रत्यागम 0;

    प्रत्यागम किल्ली_get_consumed(state, किल्ली, mode);
}

xkb_mod_mask_t
xkb_state_किल्ली_get_consumed_mods(संरचना xkb_state *state, xkb_keycode_t kc)
{
    प्रत्यागम xkb_state_किल्ली_get_consumed_mods2(state, kc, XKB_CONSUMED_MODE_XKB);
}

/*******************************************************************************
 * Server state machine API
 ******************************************************************************/

/** Entry to keep track of the state of an overlaid key */
संरचना xkb_overlaid_किल्ली {
    /** The source key being overlaid */
    ध्रुव संरचना xkb_किल्ली *old;
    /** The target key */
    ध्रुव संरचना xkb_किल्ली *new;
    /** Counter of key currently down */
    पूर्णाङ्क refcnt;
};

संरचना xkb_shortcuts_config_entry {
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
संरचना xkb_machine {
    /** Inherits from `xkb_server_state` */
    संरचना xkb_server_state base;

    /* Extra server-specific state data */

    /** Keyboard overlays handling */
    संरचना {
        /**
         * Current overlaid keys
         *
         * When a key is overlaid, it remained overlaid in this configuration
         * until totally released, i.e. every key press was matched with the
         * corresponding key release.
         */
        darray(संरचना xkb_overlaid_किल्ली) किल्ल्या;
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
    संरचना machine_config {
        /** Modifiers remapping */
        संरचना machine_modifiers_config {
            /** Real modifier mask */
            xkb_mod_mask_t mask;
            /** Modifiers re-mappings */
            darray_size_t mappings_अंक;
            संरचना machine_mods_mapping {
                xkb_mod_mask_t source;
                xkb_mod_mask_t target;
            } * mappings ATTR_COUNTED_BY(mappings_अंक);
        } modifiers;

        /** Shortcuts tweak */
        संरचना machine_shortcuts_config {
            /** Substitution entries */
            संरचना xkb_shortcuts_config_entry *entries;
        } shortcuts;
    } config;

    /** Mouse keys handling */
    संरचना {
        /** Default pointer button */
        xkb_pointer_button_index_t default_button;
        /** Mask of the locked buttons */
        xkb_pointer_button_mask_t locked_buttons;
    } mouse;

    गणना xkb_machine_flags flags;
};

प्रकारनिर्देश darray(संरचना machine_mods_mapping) machine_mods_mappings;

संरचना xkb_machine_builder {
    संरचना xkb_keymap *keymap;

    /** Controls */
    संरचना {
        /** Accessibility flags */
        संरचना {
            गणना xkb_a11y_flags affect_flags;
            गणना xkb_a11y_flags flags;
        } a11y;
    } controls;

    /** Modifiers re-mapping */
    machine_mods_mappings mods;

    /** Shortcuts tweak */
    संरचना xkb_shortcuts_config_options {
        /** Substitution entries */
        darray(संरचना xkb_shortcuts_config_entry) entries;
    } shortcuts;

    गणना xkb_machine_flags machine_flags;
    गणना xkb_machine_builder_flags builder_flags;
    पूर्णाङ्क refcnt;
};

संरचना xkb_machine_builder *
xkb_machine_builder_new(
    संरचना xkb_keymap * restrict keymap,
    ध्रुव संरचना xkb_machine_builder_config * restrict config,
    गणना xkb_status * restrict status
)
{
    /* Handle default configuration */
    स्थिर ध्रुव संरचना xkb_machine_builder_config default_config = {
        .size = परिमाण(default_config)
    };
    यदि (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ध्रुव गणना xkb_status status_ = xkb_check_state_abi(config);
    यदि (status_) {
        xkb_log_abi_error(keymap->ctx, __func__, status_);
        यदि (status)
            *status = status_;
        प्रत्यागम रिक्त;
    }

    /* Sanitize input */
    ध्रुव गणना xkb_machine_builder_flags invalid_builder_flags
        = config->builder_flags
        & ~(गणना xkb_machine_builder_flags)XKB_MACHINE_BUILDER_FLAGS_मूल्ये;
    यदि (invalid_builder_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS_,
                     "unrecognized machine builder flags: 0x%x\n",
                     invalid_builder_flags);
        यदि (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_BUILDER_FLAGS;
        प्रत्यागम रिक्त;
    }

    ध्रुव गणना xkb_machine_flags invalid_machine_flags
        = (गणना xkb_machine_flags)config->machine_flags
        & ~(गणना xkb_machine_flags)XKB_MACHINE_FLAGS_मूल्ये;
    यदि (invalid_machine_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS_,
                     "unrecognized state machine flags: 0x%x\n",
                     invalid_machine_flags);
        यदि (status)
            *status = XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS;
        प्रत्यागम रिक्त;
    }

    संरचना xkb_machine_builder * ध्रुव builder = calloc(1, परिमाण(*builder));
    यदि (!builder) {
        log_err_func1(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine builder\n");
        यदि (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        प्रत्यागम रिक्त;
    }

    *builder = (संरचना xkb_machine_builder) {
        .keymap = xkb_keymap_ref(keymap),
        .refcnt = 1,
        .builder_flags = (गणना xkb_machine_builder_flags)config->builder_flags,
        .machine_flags = (गणना xkb_machine_flags)config->machine_flags,
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

    यदि (status)
        *status = XKB_SUCCESS;

    प्रत्यागम builder;
}

संरचना xkb_machine_builder *
xkb_machine_builder_ref(संरचना xkb_machine_builder *builder)
{
    प्रतिज्ञा(builder->refcnt > 0);
    builder->refcnt++;
    प्रत्यागम builder;
}

शून्य
xkb_machine_builder_unref(संरचना xkb_machine_builder *builder)
{
    प्रतिज्ञा(!builder || builder->refcnt > 0);
    यदि (!builder || --builder->refcnt > 0)
        प्रत्यागम;

    darray_free(builder->shortcuts.entries);
    darray_free(builder->mods);
    xkb_keymap_unref(builder->keymap);
    मुक्त(builder);
}

संरचना xkb_keymap *
xkb_machine_builder_get_keymap(ध्रुव संरचना xkb_machine_builder *builder)
{
    /* Reference count is not updated. See API doc. */
    प्रत्यागम builder->keymap;
}

गणना xkb_status
xkb_machine_builder_update_a11y(
    संरचना xkb_machine_builder * restrict builder,
    ध्रुव संरचना xkb_machine_builder_a11y_update * restrict update)
{
    /* Check ABI compatibility */
    गणना xkb_status status = xkb_check_state_abi(update);
    यदि (status != XKB_SUCCESS) {
        xkb_log_abi_error(builder->keymap->ctx, __func__, status);
        प्रत्यागम status;
    }

    ध्रुव गणना xkb_a11y_flags invalid_flags =
        ~(गणना xkb_a11y_flags)XKB_A11_Y_FLAGS_मूल्ये;

    यदि (update->affect_flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y affected flags: %#x\n",
                __func__, update->affect_flags & invalid_flags);
        प्रत्यागम XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }
    यदि (update->flags & invalid_flags) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_A11Y_FLAGS_,
                "%s: unrecognized A11Y flags: %#x\n",
                __func__, update->flags & invalid_flags);
        प्रत्यागम XKB_ERROR_UNSUPPORTED_A11Y_FLAGS;
    }

    builder->controls.a11y.affect_flags |= update->affect_flags;
    builder->controls.a11y.flags &= ~update->affect_flags;
    builder->controls.a11y.flags |= (update->flags & update->affect_flags);

    प्रत्यागम XKB_SUCCESS;
}

गणना xkb_status
xkb_machine_builder_update_mods_remap(
    संरचना xkb_machine_builder * restrict builder,
    ध्रुव संरचना xkb_machine_builder_mods_remap_update * restrict update
)
{
    यदि (!update->source) {
        यदि (!update->target) {
            /* Reset mappings */
            darray_resize(builder->mods, 0);
            प्रत्यागम XKB_SUCCESS;
        } अन्यथा {
            प्रत्यागम XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
        }
    }

    /* Check the modifiers against the keymap */
    ध्रुव xkb_mod_mask_t invalid = ~builder->keymap->canonical_state_mask;
    यदि ((update->source & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid source modifiers: 0x%"PRIx32"\n",
                __func__, update->source);
        प्रत्यागम XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    यदि ((update->target & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid target modifiers: 0x%"PRIx32"\n",
                __func__, update->target);
        प्रत्यागम XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    संरचना machine_mods_mapping *mapping = रिक्त;
    darray_size_t m = 0;
    darray_enumerate(m, mapping, builder->mods) {
        यदि (mapping->source == update->source) {
            यदि (!update->target) {
                /* Remove mapping */
                darray_remove(builder->mods, m);
            } अन्यथा {
                /* Update mapping */
                mapping->target = update->target;
            }
            प्रत्यागम XKB_SUCCESS;
        }
    }

    यदि (update->target) {
        /* Append new mapping */
        darray_append(
            builder->mods,
            (संरचना machine_mods_mapping) {
                .source = update->source,
                .target = update->target
            }
        );
    } अन्यथा {
        /* Ignore missing mapping */
    }

    प्रत्यागम XKB_SUCCESS;
}

गणना xkb_status
xkb_machine_builder_update_shortcut_override(
    संरचना xkb_machine_builder * restrict builder,
    ध्रुव संरचना xkb_machine_builder_shortcut_override_update * restrict update
)
{
    संरचना xkb_keymap *keymap = builder->keymap;

    /* Check the modifiers against the keymap */
    ध्रुव xkb_mod_mask_t invalid = ~keymap->canonical_state_mask;
    यदि ((update->affect_mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid affected modifiers: 0x%"PRIx32"\n",
                __func__, update->affect_mods);
        प्रत्यागम XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }
    यदि ((update->mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                "%s: Invalid modifiers: 0x%"PRIx32"\n",
                __func__, update->mods);
        प्रत्यागम XKB_ERROR_UNSUPPORTED_MODIFIER_MASK;
    }

    /* Check the layout indices against the keymap */
    यदि (update->source >= keymap->अंक_groups &&
        update->source != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "%s: Invalid source layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->अंक_groups, update->source + 1);
        प्रत्यागम XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }
    यदि (update->target >= keymap->अंक_groups &&
        update->target != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "%s: Invalid target layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->अंक_groups, update->target + 1);
        प्रत्यागम XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX;
    }

    संरचना xkb_shortcuts_config_options * ध्रुव config = &builder->shortcuts;

    /* Resize array & initialize new entries, if relevant */
    यदि (update->source >= darray_size(config->entries) &&
        update->source != XKB_LAYOUT_INVALID) {
        यदि (update->target == update->source) {
            /* Skip default setting */
            प्रत्यागम XKB_SUCCESS;
        }
        xkb_layout_index_t new = darray_size(config->entries);
        darray_resize(config->entries, update->source + 1);
        प्रति (; new <= update->source; new++) {
            darray_item(config->entries, new) =
                (संरचना xkb_shortcuts_config_entry) {
                    .target = XKB_LAYOUT_INVALID,
                    .mods = 0
                };
        }
    }

    संरचना xkb_shortcuts_config_entry *entry;
    xkb_layout_index_t source = (update->source == XKB_LAYOUT_INVALID)
        ? 0               /* Loop over all entries */
        : update->source; /* Single entry */
    darray_foreach_from(entry, config->entries, source) {
        यदि (update->source == XKB_LAYOUT_INVALID &&
            entry->target == XKB_LAYOUT_INVALID) {
            /* Skip previous deactivated entry */
            source++;
            अनुवर्त;
        }

        यदि (update->affect_mods) {
            entry->mods &= ~update->affect_mods;
            entry->mods |= (update->affect_mods & update->mods);
        }

        यदि (update->source != XKB_LAYOUT_INVALID) {
            entry->target = (update->target == source)
                ? XKB_LAYOUT_INVALID
                : update->target;
            /* Single entry: exit loop */
            भङ्ग;
        }
        source++;
    }

    प्रत्यागम XKB_SUCCESS;
}

/** Compare 2 modifiers combinations */
स्थिर पूर्णाङ्क
cmp_mod_masks(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
    ध्रुव xkb_mod_mask_t m1 = *((xkb_mod_mask_t *) a);
    ध्रुव xkb_mod_mask_t m2 = *((xkb_mod_mask_t *) b);
    यदि (m1 == m2)
        प्रत्यागम 0;

    यदि (!m1)
        प्रत्यागम +1;
    यदि (!m2)
        प्रत्यागम -1;

    ध्रुव xkb_mod_mask_t overlap = (m1 & m2);
    यदि (overlap == m1)
        प्रत्यागम +1; /* m1 is a subset of m2 */
    यदि (overlap == m2)
        प्रत्यागम -1; /* m2 is a subset of m1 */

    /* Compare active modifiers numbers */
    ध्रुव unsigned पूर्णाङ्क count1 = popcount32(m1);
    ध्रुव unsigned पूर्णाङ्क count2 = popcount32(m2);
    यदि (count1 > count2)
        प्रत्यागम -1;
    यदि (count1 < count2)
        प्रत्यागम +1;

    /* Give priority to real modifiers */
    प्रत्यागम (m1 < m2) ? -1 : +1;
}

स्थिर गणना xkb_status
machine_set_mods(संरचना xkb_machine *sm,
                 ध्रुव machine_mods_mappings *raw_mappings)
{
    यदि (!darray_empty(*raw_mappings)) {
        machine_mods_mappings mappings = darray_new();
        xkb_mod_mask_t mask = 0;
        ध्रुव संरचना machine_mods_mapping *mapping;
        ध्रुव xkb_mod_mask_t invalid =
            ~sm->base.base.keymap->canonical_state_mask;
        darray_foreach(mapping, *raw_mappings) {
            यदि (!mapping->source || !mapping->target ||
                (mapping->source & invalid) || (mapping->target & invalid))
                अनुवर्त;
            darray_append(mappings, *mapping);
            mask |= mapping->source;
        }

        /*
         * Re-order the mappings, so that combinations have higher priority
         * if they are superset of other combinations.
         * For other type of overlaps, see the sorting function.
         */
        qsort(darray_वस्तुसंच(mappings), darray_size(mappings),
              परिमाण(*darray_वस्तुसंच(mappings)), &cmp_mod_masks);

        darray_steal(
            mappings,
            &sm->config.modifiers.mappings,
            &sm->config.modifiers.mappings_अंक
        );
        sm->config.modifiers.mask = mask;
    } अन्यथा {
        sm->config.modifiers = (संरचना machine_modifiers_config) {
            .mappings = रिक्त,
            .mappings_अंक = 0,
            .mask = 0
        };
    }

    प्रत्यागम XKB_SUCCESS;
}

स्थिर गणना xkb_status
machine_set_shortcuts(
    संरचना xkb_machine * restrict sm,
    ध्रुव संरचना xkb_shortcuts_config_options * restrict options
)
{
    यदि (darray_empty(options->entries)) {
        sm->config.shortcuts = (संरचना machine_shortcuts_config) {
            .entries = रिक्त
        };
        प्रत्यागम XKB_SUCCESS;
    }

    संरचना xkb_keymap * ध्रुव keymap = sm->base.base.keymap;

    संरचना xkb_shortcuts_config_entry * entries =
        calloc(keymap->अंक_groups, परिमाण(*entries));
    यदि (!entries)
        प्रत्यागम XKB_ERROR_ALLOCATION_FAILURE;

    ध्रुव xkb_layout_index_t मोजणी = MIN(
        keymap->अंक_groups,
        (xkb_layout_index_t) darray_size(options->entries)
    );

    bool some_entries = false;
    प्रति (xkb_layout_index_t l = 0; l < मोजणी; l++) {
        /* Sanitize layout target */
        ध्रुव xkb_layout_index_t target =
            darray_item(options->entries, l).target;
        /* Sanitize modifier mask */
        ध्रुव xkb_mod_mask_t mods = (
            darray_item(options->entries, l).mods &
            sm->base.base.keymap->canonical_state_mask
        );
        यदि (target < keymap->अंक_groups && target != l && mods) {
            entries[l] = (संरचना xkb_shortcuts_config_entry) {
                .target = target,
                .mods = mods
            };
            some_entries = true;
        } अन्यथा {
            entries[l] = (संरचना xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
            अनुवर्त;
        }
    }

    यदि (!some_entries) {
        मुक्त(entries);
        entries = रिक्त;
    } अन्यथा {
        /* Initialize groups with no entries */
        प्रति (xkb_layout_index_t l = मोजणी; l < keymap->अंक_groups; l++) {
            entries[l] = (संरचना xkb_shortcuts_config_entry) {
                .target = XKB_LAYOUT_INVALID,
                .mods = 0
            };
        }
    }

    sm->config.shortcuts = (संरचना machine_shortcuts_config) {
        .entries = entries
    };
    प्रत्यागम XKB_SUCCESS;
}

संरचना xkb_machine *
xkb_machine_new(ध्रुव संरचना xkb_machine_builder * restrict builder,
                गणना xkb_status * restrict status)
{
    संरचना xkb_machine * ध्रुव machine = calloc(1, परिमाण(*machine));
    यदि (!machine) {
        log_err_func1(builder->keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate machine\n");
        यदि (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        प्रत्यागम रिक्त;
    }

    xkb_server_state_init(&machine->base, builder->keymap, SERVER_STATE,
                          builder->controls.a11y.affect_flags,
                          builder->controls.a11y.flags);
    machine->flags = builder->machine_flags;
    यदि (!(machine->flags &
          XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_किल्ली_FIX))
    {
        machine->base.flags |= STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_किल्ली;
    }
    यदि (machine->flags & XKB_MACHINE_UNLOCK_MODS_ON_PRESS) {
        machine->base.flags |= STATE_UNLOCK_MODS_ON_PRESS;
    }

    गणना xkb_status status_;
    यदि ((status_ = machine_set_mods(machine, &builder->mods)) != XKB_SUCCESS ||
        (status_ = machine_set_shortcuts(machine, &builder->shortcuts)) !=
        XKB_SUCCESS) {
        *status = status_;
        गच्छ error;
    }

    darray_init(machine->overlays.किल्ल्या);
    machine->mouse.default_button = XKB_POINTER_BUTTON_MIN;

    यदि (status)
        *status = XKB_SUCCESS;

    प्रत्यागम machine;

error:
    xkb_machine_unref(machine);
    प्रत्यागम रिक्त;
}

संरचना xkb_machine *
xkb_machine_ref(संरचना xkb_machine *sm)
{
    प्रतिज्ञा(sm->base.base.refcnt > 0);
    sm->base.base.refcnt++;
    प्रत्यागम sm;
}

शून्य
xkb_machine_unref(संरचना xkb_machine *sm)
{
    प्रतिज्ञा(!sm || sm->base.base.refcnt > 0);
    यदि (!sm || --sm->base.base.refcnt > 0)
        प्रत्यागम;

    xkb_state_destroy(&sm->base.base);
    darray_free(sm->overlays.किल्ल्या);
    मुक्त(sm->config.shortcuts.entries);
    मुक्त(sm->config.modifiers.mappings);
    मुक्त(sm);
}

संरचना xkb_keymap *
xkb_machine_get_keymap(ध्रुव संरचना xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    प्रत्यागम sm->base.base.keymap;
}

संरचना xkb_state *
xkb_machine_get_state(संरचना xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    प्रत्यागम (संरचना xkb_state *)sm;
}

संरचना xkb_state *
xkb_state_new_from_machine(ध्रुव संरचना xkb_machine * restrict machine,
                           गणना xkb_status * restrict status)
{
    /* Creation */
    गणना xkb_status status_;
    संरचना xkb_state * ध्रुव state = xkb_state_new_with_mode(
        machine->base.base.keymap,
        XKB_STATE_MODE_SERVER_QUERY,
        &status_
    );
    यदि (status_ != XKB_SUCCESS) {
        यदि (status)
            *status = status_;
        प्रत्यागम रिक्त;
    }

    प्रतिज्ञा(state->mode == SERVER_COMPANION);
    प्रतिज्ञा(state->refcnt == 1);

    /* Copy machine’s state */
    *state = machine->base.base;

    /* Restore previous values */
    state->mode = SERVER_COMPANION;
    state->refcnt = 1;

    यदि (status)
        *status = XKB_SUCCESS;
    प्रत्यागम state;
}

स्थिर शून्य
machine_update_overlays(संरचना xkb_machine *sm)
{
    /*
     * Overlays indices are stored 1-indexed in nibbles: the lowest
     * nibble corresponds to the latest activated index.
     */

    ध्रुव xkb_overlay_mask_t mask =
        overlays_from_controls(sm->base.base.components.controls);
    xkb_overlay_mask_t added = mask & ~sm->overlays.enabled;

    /* Remove overlays no longer enabled and keep relative order */
    uint32_t order = sm->overlays.order;
    ध्रुव xkb_overlay_index_t overlay_max =
        format_max_overlays(sm->base.base.keymap->format);
    प्रति (uint8_t n = 0; n < overlay_max;) {
        xkb_overlay_index_t overlay_idx = (order >> (n * 4)) & 0xf;
        यदि (!overlay_idx)
            भङ्ग;
        ध्रुव xkb_overlay_mask_t overlay_mask =
            (xkb_overlay_mask_t)(1u << (overlay_idx - 1u) /* k is 1-indexed */);
        यदि (overlay_mask & mask) {
            /* no duplicates */
            added &= ~overlay_mask;
            n++;
        } अन्यथा {
            /* remove overlay */
            ध्रुव uint32_t head = (UINT32_C(1) << (n * 4)) - 1;
            order = (order & head) | ((order >> 4) & ~head);
        }
    }

    /* Add new overlays in ascending order */
    प्रति (xkb_overlay_index_t k = 0; added; k++, added >>= 1) {
        यदि (added & 0x1) {
            order <<= 4;
            order |= k + 1u; /* k is 0-indexed */
        }
    }

    sm->overlays.order = order;
    sm->overlays.enabled = mask;
}

गणना xkb_status
xkb_machine_process_synthetic(संरचना xkb_machine *sm,
                              ध्रुव संरचना xkb_synthetic_update *update,
                              संरचना xkb_events *events)
{
    /* Check ABI compatibility */
    गणना xkb_status status =
        check_state_update_abi(sm->base.base.keymap->ctx, update);
    यदि (status != XKB_SUCCESS)
        प्रत्यागम status;

    संरचना xkb_server_state * ध्रुव state = &sm->base;
    ध्रुव संरचना state_components previous_components = state->base.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    darray_size(events->queue) = 0;
    events->next = 0;

    /* Update parametrized controls first */
    यदि (update->layout_policy) {
        status = state_update_layout_policy(state, update->layout_policy);
        यदि (status != XKB_SUCCESS)
            प्रत्यागम status;
    }

    यदि (update->components) {
        ध्रुव संरचना xkb_state_components_update * ध्रुव components =
            update->components;
        /* Update boolean controls first */
        यदि (components->components & XKB_STATE_CONTROLS_EFFECTIVE) {
            state_update_enabled_controls(state,
                                          components->affect_controls,
                                          components->controls, events);
        }

        state_update_latched_locked(state, components, events);
    }

    xkb_state_update_derived(&state->base);

    ध्रुव गणना xkb_state_component changed = get_state_component_changes(
        &previous_components, &state->base.components
    );
    यदि (changed) {
        // TODO: latch controls
        यदि (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        /* Create event only if some component actually changed */
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .changed = changed,
                .components = state->base.components
            }
        });

        /* Terminate the frame */
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    प्रत्यागम XKB_SUCCESS;
}

स्थिर ssize_t
do_remap_modifiers(ध्रुव संरचना machine_modifiers_config * restrict mappings,
                   संरचना xkb_state * restrict state,
                   संरचना xkb_events * restrict events,
                   ध्रुव संरचना xkb_किल्ली * restrict किल्ली)
{
    यदि (!(mappings->mask & state->components.mods))
        प्रत्यागम -1;

    ध्रुव xkb_layout_index_t layout = state_किल्ली_get_layout(state, किल्ली);
    यदि (layout >= किल्ली->अंक_groups)
        प्रत्यागम -1;

    ध्रुव संरचना xkb_किल्ली_type * ध्रुव type = किल्ली->groups[layout].type;
    xkb_mod_mask_t affect_mods = 0;
    xkb_mod_mask_t mods = 0;
    प्रति (darray_size_t m = 0; m < mappings->mappings_अंक; m++) {
        ध्रुव संरचना machine_mods_mapping * ध्रुव mapping =
            &mappings->mappings[m];
        यदि (/* Skip not matching active mods */
            (mapping->source & state->components.mods) == mapping->source &&
            /* Skip overlap with previous active mappings */
            !(mapping->source & affect_mods) &&
            /* Skip if the key type uses some of the source modifiers */
            !(mapping->source & type->mods.mask)) {
            affect_mods |= mappings->mappings[m].source;
            mods |= mappings->mappings[m].target;
        }
    }

    यदि (!affect_mods)
        प्रत्यागम -1;

    संरचना xkb_state new = *state;
    new.components.base_mods = (new.components.base_mods & ~affect_mods) | mods;
    new.components.latched_mods = (new.components.latched_mods & ~affect_mods);
    new.components.locked_mods = (new.components.locked_mods & ~affect_mods);
    xkb_state_update_derived(&new);

    ssize_t event_idx = -1;
    ध्रुव गणना xkb_state_component changed =
        get_state_component_changes(&state->components, &new.components);
    यदि (changed) {
        event_idx = (ssize_t) darray_size(events->queue);
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = new.components,
                .changed = changed
            }
        });
    }

    state->components.mods = new.components.mods;
    प्रत्यागम event_idx;
}

/**
 * Activate an alternative layout if some modifiers are active, typically
 * `Control`, `Alt` or `Super`.
 *
 * The tweak consists of computing a *base* layout such that the *effective*
 * layout is the corresponding target layout.
 */
स्थिर ssize_t
do_shortcuts_tweak(ध्रुव संरचना machine_shortcuts_config *config,
                   संरचना xkb_state *state,
                   ध्रुव संरचना state_components *previous_components,
                   संरचना xkb_events *events, ssize_t remap_event)
{
    यदि (!config->entries) {
        /* No shortcuts tweak */
        प्रत्यागम remap_event;
    }

    ध्रुव संरचना xkb_shortcuts_config_entry * ध्रुव entry =
        &config->entries[state->components.group];

    यदि (entry->target != XKB_LAYOUT_INVALID &&
        (entry->mods & state->components.mods)) {
        /*
         * Activate shortcuts tweak:
         * 1. The real base group is saved by the caller, to be restored by
         *    later by a call to undo_tweaks().
         * 2. Compute the new base group so that the resulting effective group
         *    is the target.
         * 3. Recompute derived state components.
         */
        संरचना xkb_state new = *state;
        यदि (remap_event < 0) {
            /* Create new event */
            remap_event = (ssize_t) darray_size(events->queue);
            darray_append(events->queue, (संरचना xkb_event) {
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

        ध्रुव गणना xkb_state_component changed =
            get_state_component_changes(previous_components, &new.components);
        darray_item(events->queue, remap_event).components.changed = changed;
        darray_item(events->queue, remap_event).components.components =
            new.components;

        state->components.group = new.components.group;
    } अन्यथा {
        /* Deactivated entry or modifiers do not match */
    }
    प्रत्यागम remap_event;
}

स्थिर शून्य
undo_tweaks(ध्रुव संरचना xkb_state *state,
            ध्रुव संरचना state_components *initial_components,
            संरचना xkb_events *events)
{
    /* Get last component event */
    ध्रुव संरचना xkb_event *event = रिक्त;
    darray_foreach_reverse(event, events->queue) {
        यदि (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS)
            भङ्ग;
    }
    यदि (!event)
        प्रत्यागम;

    /* Restore state */
    ध्रुव गणना xkb_state_component changed =
        get_state_component_changes(initial_components,
                                    &event->components.components);
    यदि (changed) {
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = *initial_components,
                .changed = changed
            }
        });
    }
}

स्थिर ध्रुव संरचना xkb_किल्ली *
process_overlayable_किल्ली(संरचना xkb_machine *sm,
                        ध्रुव संरचना xkb_किल्ली * किल्ली,
                        गणना xkb_किल्ली_direction direction)
{
    /* Check if key is currently overlaid */
    संरचना xkb_overlaid_किल्ली *entry = रिक्त;
    संरचना xkb_overlaid_किल्ली *available_entry = रिक्त;
    darray_foreach(entry, sm->overlays.किल्ल्या) {
        यदि (entry->old == किल्ली) {
            /* Maintain the key overlaid */
            विकल्प (direction) {
            पक्ष XKB_किल्ली_DOWN:
                entry->refcnt++;
                भङ्ग;
            पक्ष XKB_किल्ली_REPEATED:
                भङ्ग;
            पक्ष XKB_किल्ली_UP:
            स्वभाव:
                entry->refcnt--;
                भङ्ग;
            }
            यदि (entry->refcnt <= 0)
                entry->old = रिक्त;
            प्रत्यागम entry->new;
        } अन्यथा यदि (!entry->old && !available_entry) {
            /* First free spot */
            available_entry = entry;
        }
    }
    यदि (direction == XKB_किल्ली_DOWN) {
        /* Enable key overlay */

        /*
         * Get overlay index: select the latest activated overlay
         * containing the key.
         */
        ध्रुव संरचना xkb_किल्ली *new = किल्ली;

        यदि (किल्ली->overlays & sm->overlays.enabled) {
            /* Some relevant overlay is active */
            प्रति (uint32_t चळत = sm->overlays.order; चळत; चळत >>= 4) {
                static_assert(XKB_OVERLAY_मोजणी == 8, "");
                ध्रुव xkb_overlay_index_t overlay = (चळत & 0xf) - 1;
                ध्रुव xkb_overlay_mask_t mask =
                    (xkb_overlay_mask_t)(1u << overlay);
                यदि (किल्ली->overlays & mask) {
                    /* Get new key */
                    यदि (किल्ली->overlays_inline) {
                        /* Stored inline */
                        new = किल्ली->overlay_किल्ली;
                    } अन्यथा {
                        /* Stored in sparse array */
                        ध्रुव xkb_overlay_mask_t खालचा = (xkb_overlay_mask_t)(
                            किल्ली->overlays & (xkb_overlay_mask_t)(mask - 1u)
                        );
                        /* index = number of set bits strictly below bit */
                        ध्रुव xkb_overlay_index_t index =
                            (xkb_overlay_index_t)popcount32(खालचा);
                        new = किल्ली->overlays_किल्ल्या[index];
                    }
                    भङ्ग;
                }
            }
        } अन्यथा {
            /*
             * No relevant overlay is active: block future overlays while
             * the key is pressed.
             */
        }

        /* Get new entry */
        यदि (available_entry) {
            /* Re-use a free entry */
            entry = available_entry;
        } अन्यथा {
            /* Append a new entry */
            ध्रुव darray_size_t idx = darray_size(sm->overlays.किल्ल्या);
            darray_resize(sm->overlays.किल्ल्या, idx + 1);
            entry = &darray_item(sm->overlays.किल्ल्या, idx);
        }

        /* Overlay the key */
        entry->old = किल्ली;
        entry->new = new;
        entry->refcnt = 1;
        प्रत्यागम new;
    }

    /* No overlay */
    प्रत्यागम किल्ली;
}

गणना xkb_status
xkb_machine_process_किल्ली(संरचना xkb_machine *sm,
                        xkb_keycode_t kc, गणना xkb_किल्ली_direction direction,
                        संरचना xkb_events *events)
{
    darray_size(events->queue) = 0;
    events->next = 0;

    संरचना xkb_server_state * ध्रुव state = &sm->base;
    ध्रुव संरचना xkb_किल्ली * किल्ली = Xkb_किल्ली(state->base.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    यदि (!किल्ली || (direction == XKB_किल्ली_REPEATED && !किल्ली->repeats))
        प्रत्यागम XKB_SUCCESS;

    ध्रुव संरचना state_components initial_components = state->base.components;

    यदि (direction == XKB_किल्ली_DOWN &&
        (sm->base.base.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_किल्ल्या) &&
        (sm->base.a11y_flags & XKB_A11_Y_STICKY_किल्ल्या_NO_SIMULTANEOUS_किल्ल्या) &&
        state->base.components.base_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_किल्ल्या,
                                      0, events);
    }

    यदि (किल्ली->overlays)
        किल्ली = process_overlayable_किल्ली(sm, किल्ली, direction);

    ssize_t remap_event = do_remap_modifiers(&sm->config.modifiers,
                                             &state->base, events, किल्ली);

    remap_event = do_shortcuts_tweak(&sm->config.shortcuts, &state->base,
                                     &initial_components, events, remap_event);

    यदि (remap_event >= 0) {
        // TODO: what about merging state event for RedirectKey?
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    state->flags |= STATE_REQUIRE_किल्ली_EVENT;
    state->set_mods = 0;
    state->clear_mods = 0;

    /* Handle key actions */
    xkb_filter_apply_all(state, events, किल्ली, direction);

    xkb_mod_index_t i;
    xkb_mod_mask_t bit;
    प्रति (i = 0, bit = 1; state->set_mods; i++, bit <<= 1) {
        यदि (state->set_mods & bit) {
            state->mod_किल्ली_मोजणी[i]++;
            state->base.components.base_mods |= bit;
            state->set_mods &= ~bit;
        }
    }

    प्रति (i = 0, bit = 1; state->clear_mods; i++, bit <<= 1) {
        यदि (state->clear_mods & bit) {
            state->mod_किल्ली_मोजणी[i]--;
            यदि (state->mod_किल्ली_मोजणी[i] <= 0) {
                state->base.components.base_mods &= ~bit;
                state->mod_किल्ली_मोजणी[i] = 0;
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
    यदि (state->flags & STATE_REQUIRE_किल्ली_EVENT) {
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_किल्ली,
            .किल्ली = {
                .keycode = किल्ली->keycode,
                .direction = direction
            }
        });
    }

    यदि (remap_event >= 0) {
        // FIXME: fragile if last state change does not restore the state to the remap event
        undo_tweaks(&state->base, &initial_components, events);
    }

    ध्रुव गणना xkb_state_component changed = get_state_component_changes(
        &initial_components, &state->base.components
    );
    यदि (changed) {
        यदि (changed & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        // TODO: merge with previous state event, if any

        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = state->base.components,
                .changed = changed
            }
        });
    }

    यदि (!darray_empty(events->queue) &&
        darray_item(events->queue, darray_size(events->queue) - 1).type !=
        XKB_EVENT_TYPE_FRAME)
    {
        darray_append(events->queue, (संरचना xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_FRAME,
        });
    }

    प्रत्यागम XKB_SUCCESS;
}

संरचना xkb_events *
xkb_events_new(संरचना xkb_context * restrict context,
               ध्रुव संरचना xkb_events_config * restrict config,
               गणना xkb_status * restrict status)
{
    /* Handle default configuration */
    स्थिर ध्रुव संरचना xkb_events_config default_config = {
        .size = परिमाण(default_config)
    };
    यदि (!config)
        config = &default_config;

    /* Check ABI compatibility */
    ध्रुव गणना xkb_status abi_error = xkb_check_state_abi(config);
    यदि (abi_error) {
        xkb_log_abi_error(context, __func__, abi_error);
        यदि (status)
            *status = abi_error;
        प्रत्यागम रिक्त;
    }

    /* Sanitize input */
    ध्रुव uint32_t invalid_flags =
        (config->flags & ~(uint32_t)XKB_EVENTS_FLAGS_मूल्ये);
    यदि (invalid_flags) {
        log_err_func(context, XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS_,
                     "unrecognized events collection flags: 0x%"PRIx32"\n",
                     invalid_flags);
        यदि (status)
            *status = XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS;
        प्रत्यागम रिक्त;
    }

    संरचना xkb_events *events = calloc(1, परिमाण(*events));
    यदि (!events) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events collection\n");
        यदि (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        प्रत्यागम रिक्त;
    }

    /* Pre-allocate event queue */
    darray_init(events->queue);
    स्थिर ध्रुव darray_size_t queue_initial_size = 16;
    darray_resize(events->queue, queue_initial_size);
    // TODO: proper safe darray_resize() with return value
    यदि (!darray_वस्तुसंच(events->queue)) {
        log_err_func1(context, XKB_ERROR_ALLOCATION_FAILURE_,
                      "cannot allocate state events queue\n");
        xkb_events_destroy(events);
        यदि (status)
            *status = XKB_ERROR_ALLOCATION_FAILURE;
        प्रत्यागम रिक्त;
    }
    darray_size(events->queue) = 0;

    events->ctx = xkb_context_ref(context);
    events->next = 0;

    यदि (status)
        *status = XKB_SUCCESS;
    प्रत्यागम events;
}

शून्य
xkb_events_destroy(संरचना xkb_events *events)
{
    यदि (!events)
        प्रत्यागम;
    darray_free(events->queue);
    xkb_context_unref(events->ctx);
    मुक्त(events);
}

ध्रुव संरचना xkb_event *
xkb_events_next(संरचना xkb_events *events)
{
    यदि (events->next < darray_size(events->queue)) {
        ध्रुव darray_size_t index = events->next++;
        प्रत्यागम &darray_item(events->queue, index);
    } अन्यथा {
        प्रत्यागम रिक्त;
    }
}

गणना xkb_event_type
xkb_event_get_type(ध्रुव संरचना xkb_event *event)
{
    प्रत्यागम event->type;
}

गणना xkb_status
xkb_event_get_keycode(ध्रुव संरचना xkb_event *event,
                      xkb_keycode_t *keycode,
                      गणना xkb_किल्ली_direction *direction)
{
    यदि (event->type == XKB_EVENT_TYPE_किल्ली) {
        *keycode = event->किल्ली.keycode;
        *direction = event->किल्ली.direction;
        प्रत्यागम XKB_SUCCESS;
    } अन्यथा {
        प्रत्यागम XKB_ERROR_EVENT_TYPE_MISMATCH;
    }
}

गणना xkb_status
xkb_event_get_components(ध्रुव संरचना xkb_event * restrict event,
                         संरचना xkb_event_components * restrict components)
{
    यदि (event->type != XKB_EVENT_TYPE_STATE_COMPONENTS)
        प्रत्यागम XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ध्रुव गणना xkb_status status = xkb_check_state_abi(components);
    यदि (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        प्रत्यागम status;
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

    प्रत्यागम XKB_SUCCESS;
}

गणना xkb_status
xkb_event_get_pointer_motion(ध्रुव संरचना xkb_event * restrict event,
                             संरचना xkb_event_pointer_motion * restrict motion)
{
    यदि (event->type != XKB_EVENT_TYPE_POINTER_MOTION)
        प्रत्यागम XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ध्रुव गणना xkb_status status = xkb_check_state_abi(motion);
    यदि (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        प्रत्यागम status;
    }

    motion->flags = event->pointer_motion.flags;
    motion->x = event->pointer_motion.x;
    motion->y = event->pointer_motion.y;

    प्रत्यागम XKB_SUCCESS;
}

गणना xkb_status
xkb_event_get_pointer_button(ध्रुव संरचना xkb_event * restrict event,
                             संरचना xkb_event_pointer_button * restrict button)
{
    यदि (event->type != XKB_EVENT_TYPE_POINTER_BUTTON)
        प्रत्यागम XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ध्रुव गणना xkb_status status = xkb_check_state_abi(button);
    यदि (status != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, status);
        प्रत्यागम status;
    }

    button->button = event->pointer_button.button;
    button->state = event->pointer_button.state;
    button->मोजणी = event->pointer_button.मोजणी;

    प्रत्यागम XKB_SUCCESS;
}

गणना xkb_status
xkb_event_get_virtual_console(ध्रुव संरचना xkb_event * restrict event,
                              int8_t * restrict index_or_offset,
                              bool * restrict is_offset)
{
    यदि (event->type != XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
        प्रत्यागम XKB_ERROR_EVENT_TYPE_MISMATCH;

    *index_or_offset = event->virtual_console.index_or_offset;
    *is_offset = event->virtual_console.is_offset;

    प्रत्यागम XKB_SUCCESS;
}

गणना xkb_status
xkb_state_update_event(संरचना xkb_state * restrict base_state,
                       ध्रुव संरचना xkb_event * restrict event,
                       गणना xkb_state_component * restrict changed)
{
    /* Guard against server-only state */
    प्रतिज्ञा(base_state->mode == SERVER_COMPANION ||
           base_state->mode == LEGACY_MIXED_STATE);
    यदि (base_state->mode != SERVER_COMPANION &&
        base_state->mode != LEGACY_MIXED_STATE) {
        log_err(base_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_MODE_,
                "%s: Unexpected state type %d\n", __func__, base_state->mode);
        प्रत्यागम XKB_ERROR_UNEXPECTED_STATE_MODE;
    }

    संरचना xkb_client_state * ध्रुव state =
        (संरचना xkb_client_state *)base_state;

    यदि (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
        ध्रुव संरचना state_components prev_components = state->base.components;
        state->base.components = event->components.components;
        /*
         * Recompute the changes instead of using the event value, because we do
         * not know if the event’s queue and the state are synced.
         */
        यदि (changed)
            *changed = get_state_component_changes(&prev_components,
                                                   &state->base.components);
    } अन्यथा {
        यदि (changed)
            *changed = 0;
    }
    प्रत्यागम XKB_SUCCESS;
}

स्थिर शून्य
append_pointer_button(संरचना xkb_events * restrict events,
                      संरचना xkb_filter * restrict filter,
                      गणना xkb_pointer_button_state button_state,
                      uint8_t मोजणी)
{
    darray_append(events->queue, (संरचना xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_POINTER_BUTTON,
        .pointer_button = {
            .size = परिमाण(((संरचना xkb_event*)0)->pointer_button),
            .button = filter->priv,
            .state = button_state,
            .मोजणी = मोजणी,
        }
    });
}

स्थिर शून्य
xkb_filter_pointer_button_new(संरचना xkb_server_state *state,
                              संरचना xkb_events *events,
                              संरचना xkb_filter *filter)
{
    यदि (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_किल्ल्या)) {
        filter->func = रिक्त;
        प्रत्यागम;
    }

    संरचना xkb_machine * ध्रुव sm = (संरचना xkb_machine *)state;

    /* Resolve button */
    ध्रुव xkb_pointer_button_index_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;
    filter->priv = button;

    ध्रुव गणना xkb_pointer_button_state button_state =
        XKB_POINTER_BUTTON_PRESSED;
    append_pointer_button(events, filter, button_state,
                          filter->action.btn.मोजणी);
    state->flags &= ~STATE_REQUIRE_किल्ली_EVENT;
}

स्थिर bool
xkb_filter_pointer_button_func(संरचना xkb_server_state *state,
                               संरचना xkb_events *events,
                               संरचना xkb_filter *filter,
                               ध्रुव संरचना xkb_किल्ली *किल्ली,
                               गणना xkb_किल्ली_direction direction)
{
    यदि (किल्ली != filter->किल्ली || !events)
        प्रत्यागम XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_किल्ली_EVENT;

    विकल्प (direction) {
    पक्ष XKB_किल्ली_DOWN:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_किल्ली_REPEATED:
        प्रत्यागम XKB_FILTER_CONSUME;
    स्वभाव:
        यदि (--filter->refcnt > 0)
            प्रत्यागम XKB_FILTER_CONSUME;
    }

    यदि (!filter->action.btn.मोजणी)
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);

    filter->func = रिक्त;
    प्रत्यागम XKB_FILTER_CONSUME;
}

संरचना xkb_filter_pointer_lock_button_priv {
    uint32_t button:31;
#ifdef _WIN32
    uint32_t locked:1; // MSVC requires matching types to pack bitfields
#अन्यथा
    bool locked:1;
#endif
};

गणना {
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH =
        परिमाण(संरचना xkb_filter_pointer_lock_button_priv) * CHAR_BIT,
    XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX =
        (UINT64_C(1) << XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_WIDTH) - 1
};

static_assert(परिमाण(संरचना xkb_filter_pointer_lock_button_priv) ==
              परिमाण(((संरचना xkb_filter*)0)->priv),
              "");
static_assert((uint32_t)XKB_POINTER_BUTTON_MAX < (uint32_t)XKB_FILTER_POINTER_LOCK_BUTTON_PRIV_MAX,
              "xkb_filter_pointer_lock_button_priv cannot store button");

स्थिर शून्य
xkb_filter_pointer_lock_button_new(संरचना xkb_server_state *state,
                                   संरचना xkb_events *events,
                                   संरचना xkb_filter *filter)
{
    यदि (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_किल्ल्या)) {
        filter->func = रिक्त;
        प्रत्यागम;
    }

    state->flags &= ~STATE_REQUIRE_किल्ली_EVENT;

    संरचना xkb_machine * ध्रुव sm = (संरचना xkb_machine *)state;

    /* Resolve button */
    ध्रुव xkb_pointer_button_index_t button =
        (filter->action.btn.button == XKB_POINTER_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;

    static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                  "invalid UINTx_C() macro");
    static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                  "invalid left shift");
    ध्रुव xkb_pointer_button_mask_t mask = (UINT8_C(1) << button);

    ध्रुव bool locked = (sm->mouse.locked_buttons & mask);

    यदि (!locked && !(filter->action.btn.flags & ACTION_LOCK_NO_LOCK)) {
        sm->mouse.locked_buttons |= mask;
        filter->priv = button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_PRESSED, 0);
    }

    संरचना xkb_filter_pointer_lock_button_priv *priv =
        (संरचना xkb_filter_pointer_lock_button_priv*)&filter->priv;
    priv->button = button,
    priv->locked = locked;
}

स्थिर bool
xkb_filter_pointer_lock_button_func(संरचना xkb_server_state *state,
                                    संरचना xkb_events *events,
                                    संरचना xkb_filter *filter,
                                    ध्रुव संरचना xkb_किल्ली *किल्ली,
                                    गणना xkb_किल्ली_direction direction)
{
    यदि (किल्ली != filter->किल्ली || !events || state->base.mode != SERVER_STATE)
        प्रत्यागम XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_किल्ली_EVENT;

    विकल्प (direction) {
    पक्ष XKB_किल्ली_DOWN:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_किल्ली_REPEATED:
        प्रत्यागम XKB_FILTER_CONSUME;
    स्वभाव:
        यदि (--filter->refcnt > 0)
            प्रत्यागम XKB_FILTER_CONSUME;
    }

    ध्रुव संरचना xkb_filter_pointer_lock_button_priv *priv =
        (संरचना xkb_filter_pointer_lock_button_priv*)&filter->priv;

    यदि (priv->locked && !(filter->action.btn.flags & ACTION_LOCK_NO_UNLOCK)) {
        संरचना xkb_machine * ध्रुव sm = (संरचना xkb_machine *)state;

        static_assert(XKB_POINTER_BUTTON_MASK_WIDTH <= 8,
                    "invalid UINTx_C() macro");
        static_assert(XKB_POINTER_BUTTON_MAX <= XKB_POINTER_BUTTON_MASK_WIDTH,
                    "invalid left shift");
        ध्रुव xkb_pointer_button_mask_t mask = (UINT8_C(1) << priv->button);

        sm->mouse.locked_buttons &= ~mask;
        filter->priv = priv->button;
        append_pointer_button(events, filter, XKB_POINTER_BUTTON_RELEASED, 0);
    }

    filter->func = रिक्त;
    प्रत्यागम XKB_FILTER_CONSUME;
}

स्थिर शून्य
xkb_filter_pointer_set_default_button_new(संरचना xkb_server_state *state,
                                          संरचना xkb_events *events,
                                          संरचना xkb_filter *filter)
{
    यदि (!events || state->base.mode != SERVER_STATE ||
        !(state->base.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_किल्ल्या)) {
        filter->func = रिक्त;
        प्रत्यागम;
    }

    state->flags &= ~STATE_REQUIRE_किल्ली_EVENT;

    संरचना xkb_machine * ध्रुव sm = (संरचना xkb_machine *)state;

    static_assert(2 * XKB_POINTER_BUTTON_MAX <= INT_MAX, "lossy conversion");
    पूर्णाङ्क button = (filter->action.dflt.flags & ACTION_ABSOLUTE_SWITCH)
        ? filter->action.dflt.मूल्य
        : sm->mouse.default_button + filter->action.dflt.मूल्य;

    /* Wrap into range */
    ध्रुव पूर्णाङ्क rem = button % XKB_POINTER_BUTTON_MAX;
    sm->mouse.default_button = (rem >= 0)
        ? (xkb_pointer_button_index_t)rem
        : (xkb_pointer_button_index_t)(rem + XKB_POINTER_BUTTON_MAX);
}

स्थिर bool
xkb_filter_pointer_set_default_button_func(संरचना xkb_server_state *state,
                                           संरचना xkb_events *events,
                                           संरचना xkb_filter *filter,
                                           ध्रुव संरचना xkb_किल्ली *किल्ली,
                                           गणना xkb_किल्ली_direction direction)
{
    यदि (किल्ली != filter->किल्ली || !events)
        प्रत्यागम XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_किल्ली_EVENT;

    विकल्प (direction) {
    पक्ष XKB_किल्ली_DOWN:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_किल्ली_REPEATED:
        प्रत्यागम XKB_FILTER_CONSUME;
    स्वभाव:
        यदि (--filter->refcnt > 0)
            प्रत्यागम XKB_FILTER_CONSUME;
    }

    filter->func = रिक्त;
    प्रत्यागम XKB_FILTER_CONTINUE;
}

स्थिर शून्य
xkb_filter_terminate_server_new(संरचना xkb_server_state *state,
                                 संरचना xkb_events *events,
                                 संरचना xkb_filter *filter)
{
    यदि (!events || state->base.mode != SERVER_STATE ||
        !(((संरचना xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = रिक्त;
        प्रत्यागम;
    }

    state->flags &= ~STATE_REQUIRE_किल्ली_EVENT;

    darray_append(events->queue, (संरचना xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    });
}

स्थिर bool
xkb_filter_terminate_server_func(संरचना xkb_server_state *state,
                                 संरचना xkb_events *events,
                                 संरचना xkb_filter *filter,
                                 ध्रुव संरचना xkb_किल्ली *किल्ली,
                                 गणना xkb_किल्ली_direction direction)
{
    यदि (किल्ली != filter->किल्ली || !events)
        प्रत्यागम XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_किल्ली_EVENT;

    विकल्प (direction) {
    पक्ष XKB_किल्ली_DOWN:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_किल्ली_REPEATED:
        प्रत्यागम XKB_FILTER_CONSUME;
    स्वभाव:
        यदि (--filter->refcnt > 0)
            प्रत्यागम XKB_FILTER_CONSUME;
    }

    filter->func = रिक्त;
    प्रत्यागम XKB_FILTER_CONSUME;
}

स्थिर शून्य
xkb_filter_switch_virtual_console_new(संरचना xkb_server_state *state,
                                      संरचना xkb_events *events,
                                      संरचना xkb_filter *filter)
{
    यदि (!events || state->base.mode != SERVER_STATE ||
        !(((संरचना xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = रिक्त;
        प्रत्यागम;
    }

    state->flags &= ~STATE_REQUIRE_किल्ली_EVENT;

    darray_append(events->queue, (संरचना xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
        .virtual_console = {
            .index_or_offset = filter->action.screen.screen,
            .is_offset = !(filter->action.screen.flags & ACTION_ABSOLUTE_SWITCH)
        }
    });
}

स्थिर bool
xkb_filter_switch_virtual_console_func(संरचना xkb_server_state *state,
                                       संरचना xkb_events *events,
                                       संरचना xkb_filter *filter,
                                       ध्रुव संरचना xkb_किल्ली *किल्ली,
                                       गणना xkb_किल्ली_direction direction)
{
    यदि (किल्ली != filter->किल्ली || !events)
        प्रत्यागम XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_किल्ली_EVENT;

    विकल्प (direction) {
    पक्ष XKB_किल्ली_DOWN:
        filter->refcnt++;
        /* fallthrough */
    पक्ष XKB_किल्ली_REPEATED:
        प्रत्यागम XKB_FILTER_CONSUME;
    स्वभाव:
        यदि (--filter->refcnt > 0)
            प्रत्यागम XKB_FILTER_CONSUME;
    }

    filter->func = रिक्त;
    प्रत्यागम XKB_FILTER_CONSUME;
}
