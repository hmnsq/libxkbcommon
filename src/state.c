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
    darray(بنية xkb_event) قطار;
    /**
     * Read cursor for `xkb_events_next()`.
     *
     * Reset to 0 on each `process_*` call.
     */
    darray_سائيز_t ايندڙ;
};

بنية xkb_server_state;

بنية xkb_filter {
    اتحاد xkb_action action;
    ثابت بنية xkb_ڪنجي *ڪنجي;
    bool (*func)(بنية xkb_server_state *state,
                 بنية xkb_events *events,
                 بنية xkb_filter *filter,
                 ثابت بنية xkb_ڪنجي *ڪنجي,
                 تعداد xkb_ڪنجي_direction direction);
    uint32_t priv;
    صحيح refcnt;
};

تعداد xkb_state_گهڻائي_اندروني {
    /** `xkb_state`: Client-only state (server API raises errors) */
    CLIENT_STATE = XKB_STATE_گهڻائي_CLIENT,
    /** `xkb_state`: Server query companion */
    SERVER_COMPANION = XKB_STATE_گهڻائي_SERVER_QUERY,
    /** `xkb_state`: Legacy unsafe client + server state */
    LEGACY_MIXED_STATE = 2,
    /** `xkb_state`: Legacy server-only state (client-only API raises errors) */
    LEGACY_SERVER_STATE = 3,
    /** `xkb_machine`: Server-only state */
    SERVER_STATE = 4,
    XKB_STATE_TYPE_انگ_ENTRIES = 5
};

static_assert(CLIENT_STATE == 0 &&
              CLIENT_STATE == SERVER_COMPANION - 1 &&
              SERVER_COMPANION == LEGACY_MIXED_STATE - 1 &&
              LEGACY_MIXED_STATE == LEGACY_SERVER_STATE - 1 &&
              LEGACY_SERVER_STATE == SERVER_STATE - 1 &&
              SERVER_STATE == XKB_STATE_TYPE_انگ_ENTRIES - 1 &&
              XKB_STATE_TYPE_انگ_ENTRIES == 5,
              "Order is used to guard client/server -only APIs: do not change");

تعداد { XKB_LAYOUT_اشاريو_T_MIN_ويڪر = 6 };
static_assert(XKB_MAX_GROUPS < (1 << XKB_LAYOUT_اشاريو_T_MIN_ويڪر),
              "Cannot encode out_of_range_group::redirect_group");

بنية ٻاهريون_جو_range_گروهه {
    تعداد xkb_layout_ٻاهريون_جو_range_policy policy :
        (حجم(تعداد xkb_layout_ٻاهريون_جو_range_policy) * CHAR_BIT -
         XKB_LAYOUT_اشاريو_T_MIN_ويڪر);
    xkb_layout_اشاريو_t redirect_گروهه : XKB_LAYOUT_اشاريو_T_MIN_ويڪر;
};

تعداد { XKB_STATE_گهڻائي_اندروني_MIN_ويڪر = 4 /* 3 bits + sign */ };
static_assert(XKB_STATE_TYPE_انگ_ENTRIES <
              (1 << XKB_STATE_گهڻائي_اندروني_MIN_ويڪر),
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
    بنية ٻاهريون_جو_range_گروهه ٻاهريون_جو_range_گروهه;

    تعداد xkb_state_گهڻائي_اندروني گهڻائي : XKB_STATE_گهڻائي_اندروني_MIN_ويڪر;
    صحيح refcnt : (حجم(صحيح) * CHAR_BIT - XKB_STATE_گهڻائي_اندروني_MIN_ويڪر);
};

/**
 * Minimal client state
 *
 * Wrapper for strong typing
 */
بنية xkb_client_state {
    /** Inherits from `xkb_state` */
    بنية xkb_state بنياد;
};

تعداد state_flags {
    STATE_NO_FLAGS = 0,
    STATE_REQUIRE_ڪنجي_EVENT = (1 << 0),
    STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_ڪنجي = (1 << 1),
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
    بنية xkb_state بنياد;

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
    xkb_mod_نقاب_t سيٽ_mods;
    xkb_mod_نقاب_t clear_mods;

    /*
     * We mustn't clear a base modifier if there's another depressed key
     * which affects it, e.g. given this sequence
     * < Left Shift down, Right Shift down, Left Shift Up >
     * the modifier should still be set. This keeps the count.
     */
    int16_t mod_ڪنجي_ڳڻپ[XKB_MAX_MODS];
};

ساكن ثابت بنية xkb_ڪنجي_type_داخلا *
get_داخلا_for_mods(ثابت بنية xkb_ڪنجي_type *type, xkb_mod_نقاب_t mods)
{
    لكل (darray_سائيز_t i = 0; i < type->انگ_entries; i++)
        إذا (داخلا_is_سرگرم(&type->entries[i]) &&
            type->entries[i].mods.نقاب == mods)
            أرجع &type->entries[i];
    أرجع عدم;
}

ساكن ثابت بنية xkb_ڪنجي_type_داخلا *
get_داخلا_for_ڪنجي_state(بنية xkb_state *state, ثابت بنية xkb_ڪنجي *ڪنجي,
                        xkb_layout_اشاريو_t گروهه)
{
    ثابت بنية xkb_ڪنجي_type* ثابت type = ڪنجي->groups[گروهه].type;
    xkb_mod_نقاب_t سرگرم_mods = state->components.mods & type->mods.نقاب;
    أرجع get_داخلا_for_mods(type, سرگرم_mods);
}

ساكن مضمن xkb_level_اشاريو_t
state_ڪنجي_get_level(بنية xkb_state *state, ثابت بنية xkb_ڪنجي *ڪنجي,
                         xkb_layout_اشاريو_t layout)
{
    إذا (layout >= ڪنجي->انگ_groups)
        أرجع XKB_LEVEL_INVALID;

    /* If we don't find an explicit match the default is 0. */
    ثابت بنية xkb_ڪنجي_type_داخلا* ثابت داخلا =
        get_داخلا_for_ڪنجي_state(state, ڪنجي, layout);

    أرجع (داخلا) ? داخلا->level : 0;
}

/**
 * Returns the level to use for the given key and state, or
 * XKB_LEVEL_INVALID.
 */
xkb_level_اشاريو_t
xkb_state_ڪنجي_get_level(بنية xkb_state *state, xkb_keycode_t kc,
                        xkb_layout_اشاريو_t layout)
{
    ثابت بنية xkb_ڪنجي* ثابت ڪنجي = Xkb_ڪنجي(state->keymap, kc);

    أرجع (ڪنجي)
        ? state_ڪنجي_get_level(state, ڪنجي, layout)
        : XKB_LEVEL_INVALID;
}

ساكن مضمن xkb_layout_اشاريو_t
state_ڪنجي_get_layout(بنية xkb_state *state, ثابت بنية xkb_ڪنجي *ڪنجي)
{
    أرجع xkb_keymap_ڪنجي_effective_layout(ڪنجي, state->components.گروهه);
}

/**
 * Returns the layout to use for the given key and state, taking
 * wrapping/clamping/etc into account, or XKB_LAYOUT_INVALID.
 */
xkb_layout_اشاريو_t
xkb_state_ڪنجي_get_layout(بنية xkb_state *state, xkb_keycode_t kc)
{
    ثابت بنية xkb_ڪنجي* ثابت ڪنجي = Xkb_ڪنجي(state->keymap, kc);

    إذا (!ڪنجي)
        أرجع XKB_LAYOUT_INVALID;

    أرجع state_ڪنجي_get_layout(state, ڪنجي);
}

/* Empty action used for empty levels */
ساكن ثابت اتحاد xkb_action dummy_action = { .type = ACTION_TYPE_NONE };

ساكن xkb_action_ڳڻپ_t
xkb_ڪنجي_get_actions(بنية xkb_state *state, ثابت بنية xkb_ڪنجي *ڪنجي,
                    ثابت اتحاد xkb_action **actions)
{
    ثابت xkb_layout_اشاريو_t layout = state_ڪنجي_get_layout(state, ڪنجي);
    ثابت xkb_level_اشاريو_t level = state_ڪنجي_get_level(state, ڪنجي, layout);
    إذا (level == XKB_LEVEL_INVALID)
        اذهب err;

    ثابت xkb_action_ڳڻپ_t ڳڻپ =
        xkb_keymap_ڪنجي_get_actions_by_level(state->keymap, ڪنجي,
                                            layout, level, actions);
    إذا (!ڳڻپ)
        اذهب err;

    أرجع ڳڻپ;

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
        darray_resize0(state->filters, darray_سائيز(state->filters) + 1);
        filter = &darray_شي(state->filters, darray_سائيز(state->filters) -1);
    }

    filter->refcnt = 1;
    أرجع filter;
}

/***====================================================================***/

تعداد xkb_filter_نتيجو {
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
#وصف_ڪريو apply_گروهه_delta(filter_, state_, component_)                   \
    إذا ((filter_)->action.گروهه.flags & ACTION_ABSOLUTE_SWITCH)          \
        (state_)->components.component_ = (filter_)->action.گروهه.گروهه; \
    وإلا                                                                 \
        (state_)->components.component_ += (filter_)->action.گروهه.گروهه

ساكن فراغ
xkb_filter_گروهه_سيٽ_new(بنية xkb_server_state *state,
                         بنية xkb_events *events,
                         بنية xkb_filter *filter)
{
    static_assert(حجم(state->بنياد.components.بنياد_گروهه) ==
                  حجم(filter->priv),
                  "Max groups don't fit");
    filter->priv = (uint32_t)state->بنياد.components.بنياد_گروهه;
    apply_گروهه_delta(filter, &state->بنياد, بنياد_گروهه);
}

ساكن bool
xkb_filter_گروهه_سيٽ_func(بنية xkb_server_state *state,
                          بنية xkb_events *events,
                          بنية xkb_filter *filter,
                          ثابت بنية xkb_ڪنجي *ڪنجي,
                          تعداد xkb_ڪنجي_direction direction)
{
    إذا (ڪنجي != filter->ڪنجي) {
        filter->action.گروهه.flags &= ~ACTION_تالو_CLEAR;
        أرجع XKB_FILTER_CONTINUE;
    }

    بدل (direction) {
    حالة XKB_ڪنجي_هيٺ_ڏانهن:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_ڪنجي_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    static_assert(حجم(state->بنياد.components.بنياد_گروهه) == حجم(filter->priv),
                  "Max groups don't fit");
    state->بنياد.components.بنياد_گروهه = (int32_t) filter->priv;

    إذا (filter->action.گروهه.flags & ACTION_تالو_CLEAR)
        state->بنياد.components.locked_گروهه = 0;

    filter->func = عدم;
    أرجع XKB_FILTER_CONTINUE;
}

ساكن تعداد xkb_state_component
get_state_component_changes(ثابت بنية state_components *a,
                            ثابت بنية state_components *b)
{
    تعداد xkb_state_component نقاب = 0;

    إذا (a->گروهه != b->گروهه)
        نقاب |= XKB_STATE_LAYOUT_EFFECTIVE;
    إذا (a->بنياد_گروهه != b->بنياد_گروهه)
        نقاب |= XKB_STATE_LAYOUT_DEPRESSED;
    إذا (a->latched_گروهه != b->latched_گروهه)
        نقاب |= XKB_STATE_LAYOUT_LATCHED;
    إذا (a->locked_گروهه != b->locked_گروهه)
        نقاب |= XKB_STATE_LAYOUT_LOCKED;
    إذا (a->mods != b->mods)
        نقاب |= XKB_STATE_MODS_EFFECTIVE;
    إذا (a->بنياد_mods != b->بنياد_mods)
        نقاب |= XKB_STATE_MODS_DEPRESSED;
    إذا (a->latched_mods != b->latched_mods)
        نقاب |= XKB_STATE_MODS_LATCHED;
    إذا (a->locked_mods != b->locked_mods)
        نقاب |= XKB_STATE_MODS_LOCKED;
    إذا (a->leds != b->leds)
        نقاب |= XKB_STATE_LEDS;
    إذا (a->controls != b->controls)
        نقاب |= XKB_STATE_CONTROLS_EFFECTIVE;

    أرجع نقاب;
}

ساكن فراغ
xkb_filter_گروهه_تالو_new(بنية xkb_server_state *state,
                          بنية xkb_events *events,
                          بنية xkb_filter *filter)
{
    إذا (filter->action.گروهه.flags & ACTION_تالو_ON_RELEASE) {
        /*
         * Lock on key release: do nothing on key press.
         *
         * This is a keymap format v2 extension.
         */
        أرجع;
    } وإلا {
        /* Lock on key press */
        apply_گروهه_delta(filter, &state->بنياد, locked_گروهه);
    }
}

ساكن bool
xkb_filter_گروهه_تالو_func(بنية xkb_server_state *state,
                           بنية xkb_events *events,
                           بنية xkb_filter *filter,
                           ثابت بنية xkb_ڪنجي *ڪنجي,
                           تعداد xkb_ڪنجي_direction direction)
{
    إذا (ڪنجي != filter->ڪنجي) {
        إذا ((filter->action.گروهه.flags & ACTION_تالو_ON_RELEASE) &&
            direction == XKB_ڪنجي_هيٺ_ڏانهن) {
            /*
             * Another key has been pressed after the locking key:
             * cancel group lock on release.
             *
             * This is a keymap v2 extension.
             */
            filter->action.گروهه.flags &= ~ACTION_تالو_ON_RELEASE;
        }
        أرجع XKB_FILTER_CONTINUE;
    }

    بدل (direction) {
    حالة XKB_ڪنجي_هيٺ_ڏانهن:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_ڪنجي_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    إذا (filter->action.گروهه.flags & ACTION_تالو_ON_RELEASE) {
        /*
         * Lock on key release
         *
         * This is a keymap v2 extension.
         */
        apply_گروهه_delta(filter, &state->بنياد, locked_گروهه);
    } وإلا {
        /* Lock on key press: do nothing on key release. */
    }
    filter->func = عدم;
    أرجع XKB_FILTER_CONTINUE;
}

ساكن bool
xkb_action_breaks_latch(ثابت اتحاد xkb_action *action,
                        تعداد xkb_action_controls controls,
                        تعداد xkb_اندروني_action_flags flag,
                        xkb_mod_نقاب_t نقاب)
{
    بدل (action->type) {
    حالة ACTION_TYPE_NONE:
    حالة ACTION_TYPE_VOID:
    حالة ACTION_TYPE_PTR_BUTTON:
    حالة ACTION_TYPE_PTR_تالو:
    حالة ACTION_TYPE_CTRL_سيٽ:
    حالة ACTION_TYPE_CTRL_تالو:
    حالة ACTION_TYPE_SWITCH_VT:
    حالة ACTION_TYPE_TERMINATE:
    حالة ACTION_TYPE_REDIRECT_ڪنجي:
        أرجع true;
    حالة ACTION_TYPE_PTR_MOVE:
    حالة ACTION_TYPE_PTR_DEFAULT:
        أرجع !(controls & CONTROL_MOUSE_ڪنجيون);
    حالة ACTION_TYPE_اندروني:
        أرجع (action->اندروني.flags & flag) &&
               ((action->اندروني.clear_latched_mods & نقاب) == نقاب);
    افتراضي:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_اندروني == 20 &&
                      ACTION_TYPE_اندروني + 1 == ACTION_TYPE_انگ_ENTRIES,
                      "Missing action type");
        أرجع false;
    }
}

تعداد xkb_ڪنجي_latch_state {
    NO_LATCH = 0,
    LATCH_ڪنجي_هيٺ_ڏانهن,
    LATCH_PENDING,
    ڪنجي_LATCH_STATE_انگ_ENTRIES
};

#وصف_ڪريو MAX_XKB_ڪنجي_LATCH_STATE_LOG2 2
#إذا (ڪنجي_LATCH_STATE_انگ_ENTRIES > (1 << MAX_XKB_ڪنجي_LATCH_STATE_LOG2)) || \
    (-XKB_MAX_GROUPS) < (INT32_MIN >> MAX_XKB_ڪنجي_LATCH_STATE_LOG2) || \
      XKB_MAX_GROUPS > (INT32_MAX >> MAX_XKB_ڪنجي_LATCH_STATE_LOG2)
#غلطي "Cannot represent priv field of the group latch filter"
#جيڪڏهن_پورو

/* Hold the latch state *and* the group delta */
اتحاد گروهه_latch_priv {
    uint32_t priv;
    بنية {
        /* The type is really: enum xkb_key_latch_state, but it is problematic
         * on Windows, because it is interpreted as signed and leads to wrong
         * negative values. */
        بنانشان صحيح latch:MAX_XKB_ڪنجي_LATCH_STATE_LOG2;
        int32_t گروهه_delta:(32 - MAX_XKB_ڪنجي_LATCH_STATE_LOG2);
    };
};

ساكن فراغ
xkb_filter_گروهه_latch_new(بنية xkb_server_state *state,
                           بنية xkb_events *events,
                           بنية xkb_filter *filter)
{
    ثابت اتحاد گروهه_latch_priv priv = {
        .latch = LATCH_ڪنجي_هيٺ_ڏانهن,
        .گروهه_delta = (filter->action.گروهه.flags & ACTION_ABSOLUTE_SWITCH)
            ? filter->action.گروهه.گروهه - state->بنياد.components.بنياد_گروهه
            : filter->action.گروهه.گروهه
    };
    filter->priv = priv.priv;
    /* Like group set */
    apply_گروهه_delta(filter, &state->بنياد, بنياد_گروهه);
}

ساكن bool
xkb_filter_گروهه_latch_func(بنية xkb_server_state *state,
                            بنية xkb_events *events,
                            بنية xkb_filter *filter,
                            ثابت بنية xkb_ڪنجي *ڪنجي,
                            تعداد xkb_ڪنجي_direction direction)
{
    اتحاد گروهه_latch_priv priv = {.priv = filter->priv};
    تعداد xkb_ڪنجي_latch_state latch = priv.latch;

    إذا (direction == XKB_ڪنجي_هيٺ_ڏانهن) {
        ثابت اتحاد xkb_action *actions = عدم;
        ثابت xkb_action_ڳڻپ_t ڳڻپ = xkb_ڪنجي_get_actions(&state->بنياد, ڪنجي,
                                                             &actions);

        إذا (latch == LATCH_ڪنجي_هيٺ_ڏانهن) {
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
            إذا (state->a11y_flags & XKB_A11_Y_LATCH_گڏو_گڏ_ڪنجيون) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                لكل (xkb_action_ڳڻپ_t k = 0; k < ڳڻپ; k++) {
                    إذا (xkb_action_breaks_latch(&(actions[k]),
                                                state->بنياد.components.controls,
                                                اندروني_BREAKS_گروهه_LATCH,
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
            ثابت bool sticky_ڪنجيون = state->بنياد.components.controls
                                   & CONTROL_STICKY_ڪنجيون;
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
            لكل (xkb_action_ڳڻپ_t k = 0; k < ڳڻپ; k++) {
                إذا (((actions[k].type == ACTION_TYPE_گروهه_LATCH) ||
                     (actions[k].type == ACTION_TYPE_گروهه_سيٽ && sticky_ڪنجيون)) &&
                     actions[k].گروهه.گروهه == filter->action.گروهه.گروهه &&
                     (actions[k].گروهه.flags & ACTION_ABSOLUTE_SWITCH) ==
                     (filter->action.گروهه.flags & ACTION_ABSOLUTE_SWITCH)) {
                    إذا (filter->action.گروهه.flags & ACTION_LATCH_منزل_تالو &&
                        filter->action.گروهه.گروهه != 0)
                    {
                        /* Promote to lock */
                        filter->action.type = ACTION_TYPE_گروهه_تالو;
                        filter->func = xkb_filter_گروهه_تالو_func;
                        xkb_filter_گروهه_تالو_new(state, events, filter);
                        state->بنياد.components.latched_گروهه -= priv.گروهه_delta;
                        filter->ڪنجي = ڪنجي;
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
                                                 state->بنياد.components.controls,
                                                 اندروني_BREAKS_گروهه_LATCH,
                                                 0))
                {
                    /* Breaks the latch */
                    state->بنياد.components.latched_گروهه -= priv.گروهه_delta;
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
    وإلا إذا (ڪنجي != filter->ڪنجي) {
        /* Ignore repeat/release of other keys */
    }
    وإلا إذا (direction == XKB_ڪنجي_REPEATED) {
        أرجع XKB_FILTER_CONSUME; // TODO: check
    }
    وإلا {
        /* Our key got released.  If we've set it to clear locks, and we
         * currently have a group locked, then release it and
         * don't actually latch.  Else we've actually hit the latching
         * stage, so set PENDING and move our group from base to
         * latched. */
        إذا ((filter->action.گروهه.flags & ACTION_تالو_CLEAR) &&
             state->بنياد.components.locked_گروهه) {
            إذا (latch == LATCH_PENDING)
                state->بنياد.components.latched_گروهه -= priv.گروهه_delta;
            وإلا
                state->بنياد.components.بنياد_گروهه -= priv.گروهه_delta;
            state->بنياد.components.locked_گروهه = 0;
            filter->func = عدم;
        }
        /* Broken latch */
        وإلا إذا (latch == NO_LATCH) {
            state->بنياد.components.بنياد_گروهه -= priv.گروهه_delta;
            filter->func = عدم;
        }
        /* We may already have reached the latch state if pressing the
         * key multiple times without latch-to-lock enabled. */
        وإلا إذا (latch == LATCH_ڪنجي_هيٺ_ڏانهن) {
            latch = LATCH_PENDING;
            /* Switch from set to latch */
            state->بنياد.components.بنياد_گروهه -= priv.گروهه_delta;
            state->بنياد.components.latched_گروهه += priv.گروهه_delta;
            /* XXX beep beep! */
        }
    }

    priv.latch = latch;
    filter->priv = priv.priv;

    أرجع XKB_FILTER_CONTINUE;
}

ساكن فراغ
xkb_filter_mod_سيٽ_new(بنية xkb_server_state *state,
                       بنية xkb_events *events,
                       بنية xkb_filter *filter)
{
    ثابت تعداد xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_تالو_CLEAR;
    إذا ((filter->action.mods.flags & unlock) == unlock) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension.
         */
        filter->priv = filter->action.mods.mods.نقاب
                     & ~state->بنياد.components.locked_mods;
        state->بنياد.components.locked_mods &= ~filter->action.mods.mods.نقاب;
    } وإلا {
        filter->priv = filter->action.mods.mods.نقاب;
    }

    state->سيٽ_mods |= (xkb_mod_نقاب_t) filter->priv;
}

ساكن bool
xkb_filter_mod_سيٽ_func(بنية xkb_server_state *state,
                        بنية xkb_events *events,
                        بنية xkb_filter *filter,
                        ثابت بنية xkb_ڪنجي *ڪنجي,
                        تعداد xkb_ڪنجي_direction direction)
{

    إذا (ڪنجي != filter->ڪنجي) {
        filter->action.mods.flags &= ~ACTION_تالو_CLEAR;
        أرجع XKB_FILTER_CONTINUE;
    }

    بدل (direction) {
    حالة XKB_ڪنجي_هيٺ_ڏانهن:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_ڪنجي_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    state->clear_mods |= (xkb_mod_نقاب_t) filter->priv;
    ثابت تعداد xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_تالو_CLEAR;
    إذا ((filter->action.mods.flags & unlock) == ACTION_تالو_CLEAR)
        state->بنياد.components.locked_mods &= ~filter->action.mods.mods.نقاب;

    filter->func = عدم;
    أرجع XKB_FILTER_CONTINUE;
}

ساكن فراغ
xkb_filter_mod_تالو_new(بنية xkb_server_state *state,
                        بنية xkb_events *events,
                        بنية xkb_filter *filter)
{
    filter->priv = (state->بنياد.components.locked_mods &
                    filter->action.mods.mods.نقاب);

    إذا (filter->priv && (filter->action.mods.flags & ACTION_UNLOCK_ON_PRESS)) {
        /*
         * Some of the target modifiers were locked before key press: unlock.
         *
         * This is a keymap v2 extension: unlock-on-press.
         */
        إذا (!(filter->action.mods.flags & ACTION_تالو_NO_UNLOCK))
            state->بنياد.components.locked_mods &= ~filter->priv;
        /* No further action: cancel filter */
        filter->func = عدم;
    } وإلا {
        /* Set base mods; lock mods if relevant (XKB 1.0 spec) */
        state->سيٽ_mods |= filter->action.mods.mods.نقاب;
        إذا (!(filter->action.mods.flags & ACTION_تالو_NO_تالو))
            state->بنياد.components.locked_mods |= filter->action.mods.mods.نقاب;
    }
}

ساكن bool
xkb_filter_mod_تالو_func(بنية xkb_server_state *state,
                         بنية xkb_events *events,
                         بنية xkb_filter *filter,
                         ثابت بنية xkb_ڪنجي *ڪنجي,
                         تعداد xkb_ڪنجي_direction direction)
{
    إذا (ڪنجي != filter->ڪنجي)
        أرجع XKB_FILTER_CONTINUE;

    بدل (direction) {
    حالة XKB_ڪنجي_هيٺ_ڏانهن:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_ڪنجي_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    state->clear_mods |= filter->action.mods.mods.نقاب;
    إذا (!(filter->action.mods.flags & ACTION_تالو_NO_UNLOCK))
        state->بنياد.components.locked_mods &= ~filter->priv;

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

    إذا ((filter->action.mods.flags & ACTION_تالو_CLEAR) &&
        (filter->action.mods.flags & unlockOnPress) &&
        (state->بنياد.components.locked_mods & filter->action.mods.mods.نقاب) ==
         filter->action.mods.mods.نقاب) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension: clear locks and do not latch.
         */
        state->بنياد.components.locked_mods &= ~filter->action.mods.mods.نقاب;
        filter->func = عدم;
    } وإلا إذا (filter->action.mods.flags & ACTION_LATCH_ON_PRESS) {
        /*
         * Latch on key press
         *
         * This is a keymap format v2 extension.
         */
        filter->priv = LATCH_PENDING;
        state->بنياد.components.latched_mods |= filter->action.mods.mods.نقاب;
        /* XXX beep beep! */
    } وإلا {
        /* XKB standard latch action */
        filter->priv = LATCH_ڪنجي_هيٺ_ڏانهن;
        state->سيٽ_mods |= filter->action.mods.mods.نقاب;
    }
}

ساكن bool
xkb_filter_mod_latch_func(بنية xkb_server_state *state,
                          بنية xkb_events *events,
                          بنية xkb_filter *filter,
                          ثابت بنية xkb_ڪنجي *ڪنجي,
                          تعداد xkb_ڪنجي_direction direction)
{
    تعداد xkb_ڪنجي_latch_state latch = filter->priv;

    إذا (direction == XKB_ڪنجي_هيٺ_ڏانهن) {
        ثابت اتحاد xkb_action *actions = عدم;
        ثابت xkb_action_ڳڻپ_t ڳڻپ = xkb_ڪنجي_get_actions(&state->بنياد, ڪنجي,
                                                             &actions);

        إذا (latch == LATCH_ڪنجي_هيٺ_ڏانهن) {
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
            إذا (state->a11y_flags & XKB_A11_Y_LATCH_گڏو_گڏ_ڪنجيون) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                لكل (xkb_action_ڳڻپ_t k = 0; k < ڳڻپ; k++) {
                    إذا (xkb_action_breaks_latch(&(actions[k]),
                                                state->بنياد.components.controls,
                                                اندروني_BREAKS_MOD_LATCH,
                                                filter->action.mods.mods.نقاب))
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
            ثابت bool sticky_ڪنجيون = state->بنياد.components.controls
                                   & CONTROL_STICKY_ڪنجيون;
            /*
             * NOTE: Action’s flags are not compared:
             * - flags may have been modified by keyboard controls at the
             *   creation of the filter;
             * - it is unlikely to have two LatchMods() actions on the same
             *   key that differ only by their flags;
             * - even if it were the case, one would most likely want both
             *   actions to be considered compatible in this context.
             */
            لكل (xkb_action_ڳڻپ_t k = 0; k < ڳڻپ; k++) {
                إذا (((actions[k].type == ACTION_TYPE_MOD_LATCH) ||
                     (actions[k].type == ACTION_TYPE_MOD_سيٽ && sticky_ڪنجيون)) &&
                    actions[k].mods.mods.نقاب == filter->action.mods.mods.نقاب)
                {
                    إذا (filter->action.mods.flags & ACTION_LATCH_منزل_تالو) {
                        /* Mutate the action to LockMods() */
                        filter->action.type = ACTION_TYPE_MOD_تالو;
                        filter->func = xkb_filter_mod_تالو_func;
                        xkb_filter_mod_تالو_new(state, events, filter);
                    }
                    وإلا {
                        /* Mutate the action to SetMods() */
                        filter->action.type = ACTION_TYPE_MOD_سيٽ;
                        filter->func = xkb_filter_mod_سيٽ_func;
                        xkb_filter_mod_سيٽ_new(state, events, filter);
                    }
                    filter->ڪنجي = ڪنجي;
                    /* Clear latches */
                    state->بنياد.components.latched_mods &=
                        ~filter->action.mods.mods.نقاب;
                    /* XXX beep beep! */
                    أرجع XKB_FILTER_CONSUME;
                }
                وإلا إذا (xkb_action_breaks_latch(&(actions[k]),
                                                 state->بنياد.components.controls,
                                                 اندروني_BREAKS_MOD_LATCH,
                                                 filter->action.mods.mods.نقاب))
                {
                    /* XXX: This may be totally broken, we might need to break the
                     *      latch in the next run after this press? */
                    state->بنياد.components.latched_mods &=
                        ~filter->action.mods.mods.نقاب;
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
    وإلا إذا (ڪنجي != filter->ڪنجي) {
        /* Ignore repeat/release of other keys */
    }
    وإلا إذا (direction == XKB_ڪنجي_REPEATED) {
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

        إذا ((filter->action.mods.flags & ACTION_تالو_CLEAR) &&
            !(filter->action.mods.flags & unlockOnPress) &&
            (state->بنياد.components.locked_mods &
             filter->action.mods.mods.نقاب) == filter->action.mods.mods.نقاب)
        {
            /* XXX: We might be a bit overenthusiastic about clearing
             *      mods other filters have set here? */
            إذا (latch == LATCH_PENDING)
                state->بنياد.components.latched_mods &=
                    ~filter->action.mods.mods.نقاب;
            وإلا
                state->clear_mods |= filter->action.mods.mods.نقاب;
            state->بنياد.components.locked_mods &= ~filter->action.mods.mods.نقاب;
            filter->func = عدم;
        }
        وإلا إذا (latch == NO_LATCH) {
            /* Broken latch */
            state->clear_mods |= filter->action.mods.mods.نقاب;
            filter->func = عدم;
        }
        وإلا إذا (!(filter->action.mods.flags & ACTION_LATCH_ON_PRESS)) {
            latch = LATCH_PENDING;
            state->clear_mods |= filter->action.mods.mods.نقاب;
            state->بنياد.components.latched_mods |= filter->action.mods.mods.نقاب;
            /* XXX beep beep! */
        }
    }

    filter->priv = latch;

    أرجع XKB_FILTER_CONTINUE;
}

ساكن فراغ
append_اشارو_move(بنية xkb_events *events,
                    بنية xkb_filter *filter)
{
    تعداد xkb_اشارو_motion_flags flags = 0;
    إذا (filter->action.ptr.flags & ACTION_ABSOLUTE_X)
        flags |= XKB_اشارو_MOTION_ABSOLUTE_X;
    إذا (filter->action.ptr.flags & ACTION_ABSOLUTE_Y)
        flags |= XKB_اشارو_MOTION_ABSOLUTE_Y;
    إذا (filter->action.ptr.flags & ACTION_REPEAT)
        flags |= XKB_اشارو_MOTION_REPEATS;
    darray_append(events->قطار, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_اشارو_MOTION,
        .اشارو_motion = {
            .سائيز = حجم(((بنية xkb_event*)0)->اشارو_motion),
            .flags = flags,
            .x = filter->action.ptr.x,
            .y = filter->action.ptr.y,
        }
    });
}

ساكن فراغ
xkb_filter_اشارو_move_new(بنية xkb_server_state *state,
                            بنية xkb_events *events,
                            بنية xkb_filter *filter)
{
    إذا (!events) {
        filter->func = عدم;
        أرجع;
    }

    أكد(state->بنياد.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ڪنجيون);
    append_اشارو_move(events, filter);
    state->flags &= ~STATE_REQUIRE_ڪنجي_EVENT;
}

ساكن bool
xkb_filter_اشارو_move_func(بنية xkb_server_state *state,
                             بنية xkb_events *events,
                             بنية xkb_filter *filter,
                             ثابت بنية xkb_ڪنجي *ڪنجي,
                             تعداد xkb_ڪنجي_direction direction)
{
    إذا (ڪنجي != filter->ڪنجي || !events)
        أرجع XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ڪنجي_EVENT;

    بدل (direction) {
    حالة XKB_ڪنجي_هيٺ_ڏانهن:
        filter->refcnt++;
        append_اشارو_move(events, filter);
        أرجع XKB_FILTER_CONSUME;
    حالة XKB_ڪنجي_REPEATED:
        إذا (filter->action.ptr.flags & ACTION_REPEAT)
            append_اشارو_move(events, filter);
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
xkb_filter_اشارو_button_new(بنية xkb_server_state *state,
                              بنية xkb_events *events,
                              بنية xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن bool
xkb_filter_اشارو_button_func(بنية xkb_server_state *state,
                               بنية xkb_events *events,
                               بنية xkb_filter *filter,
                               ثابت بنية xkb_ڪنجي *ڪنجي,
                               تعداد xkb_ڪنجي_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن فراغ
xkb_filter_اشارو_تالو_button_new(بنية xkb_server_state *state,
                                   بنية xkb_events *events,
                                   بنية xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن bool
xkb_filter_اشارو_تالو_button_func(بنية xkb_server_state *state,
                                    بنية xkb_events *events,
                                    بنية xkb_filter *filter,
                                    ثابت بنية xkb_ڪنجي *ڪنجي,
                                    تعداد xkb_ڪنجي_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن فراغ
xkb_filter_اشارو_سيٽ_default_button_new(بنية xkb_server_state *state,
                                          بنية xkb_events *events,
                                          بنية xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن bool
xkb_filter_اشارو_سيٽ_default_button_func(بنية xkb_server_state *state,
                                           بنية xkb_events *events,
                                           بنية xkb_filter *filter,
                                           ثابت بنية xkb_ڪنجي *ڪنجي,
                                           تعداد xkb_ڪنجي_direction direction);

ساكن فراغ
xkb_filter_terminate_server_new(بنية xkb_server_state *state,
                                 بنية xkb_events *events,
                                 بنية xkb_filter *filter);

ساكن bool
xkb_filter_terminate_server_func(بنية xkb_server_state *state,
                                  بنية xkb_events *events,
                                  بنية xkb_filter *filter,
                                  ثابت بنية xkb_ڪنجي *ڪنجي,
                                  تعداد xkb_ڪنجي_direction direction);

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
                                       ثابت بنية xkb_ڪنجي *ڪنجي,
                                       تعداد xkb_ڪنجي_direction direction);

ساكن مضمن فراغ
clear_all_latches_and_locks(بنية xkb_server_state *state,
                            بنية xkb_events *events);

ساكن فراغ
xkb_filter_ctrls_new(بنية xkb_server_state *state,
                     بنية xkb_events *events,
                     بنية xkb_filter *filter)
{
    إذا (filter->action.type == ACTION_TYPE_CTRL_سيٽ) {
        /* Set: save the specified controls that are *not* already enabled */
        filter->priv = (uint32_t) (
            (~state->بنياد.components.controls) & filter->action.ctrls.ctrls
        );
    } وإلا {
        /* Lock: save the specified controls that *are* already enabled */
        filter->priv = (uint32_t) (
            state->بنياد.components.controls & filter->action.ctrls.ctrls
        );
    }

    إذا (filter->action.type == ACTION_TYPE_CTRL_سيٽ ||
        !(filter->action.ctrls.flags & ACTION_تالو_NO_تالو)) {
        /* Enable the specified controls that are not already enabled */
        state->بنياد.components.controls |= filter->action.ctrls.ctrls;
    }
}

ساكن bool
xkb_filter_ctrls_func(بنية xkb_server_state *state,
                      بنية xkb_events *events,
                      بنية xkb_filter *filter,
                      ثابت بنية xkb_ڪنجي *ڪنجي,
                      تعداد xkb_ڪنجي_direction direction)
{
    إذا (ڪنجي != filter->ڪنجي)
        أرجع XKB_FILTER_CONTINUE;

    بدل (direction) {
    حالة XKB_ڪنجي_هيٺ_ڏانهن:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_ڪنجي_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    إذا (filter->action.type == ACTION_TYPE_CTRL_سيٽ ||
        !(filter->action.ctrls.flags & ACTION_تالو_NO_UNLOCK)) {
        ثابت تعداد xkb_action_controls old = state->بنياد.components.controls;

        /*
         * Set: Disable specified controls that were *not* enabled at key press.
         * Lock: Disable specified controls that *were* enabled at key press.
         */
        state->بنياد.components.controls &= ~(تعداد xkb_action_controls) filter->priv;

        إذا ((old & CONTROL_STICKY_ڪنجيون) &&
            !(state->بنياد.components.controls & CONTROL_STICKY_ڪنجيون)) {
            /* Sticky keys were disabled: clear all locks and latches */
            clear_all_latches_and_locks(state, events);
        }
    }

    filter->func = عدم;
    أرجع XKB_FILTER_CONTINUE;
}

ساكن bool
append_redirect_ڪنجي_events(بنية xkb_server_state *state,
                           بنية xkb_events *events,
                           ثابت بنية xkb_redirect_ڪنجي_action *redirect,
                           تعداد xkb_ڪنجي_direction direction)
{
    تعداد xkb_state_component بدليو = 0;
    ثابت xkb_mod_نقاب_t نقاب = redirect->affect_mods;

    /*
     * Reference state: find the last state update in the queue, otherwise
     * use the current state.
     */
    بنية xkb_event *event;
    بنية state_components آخري_components = state->بنياد.components;
    darray_foreach_ابتو(event, events->قطار) {
        إذا (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
            آخري_components = event->components.components;
            اكسر;
        }
    }

    إذا (نقاب) {
        بنية state_components new = آخري_components;
        new.بنياد_mods = (new.بنياد_mods & ~نقاب) | redirect->mods;
        new.latched_mods = (new.latched_mods & ~نقاب) | redirect->mods;
        new.locked_mods = (new.locked_mods & ~نقاب) | redirect->mods;
        new.mods = (new.mods & ~نقاب) | redirect->mods;

        بدليو = get_state_component_changes(&آخري_components, &new);
        إذا (بدليو) {
            darray_append(events->قطار, (بنية xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = new,
                    .بدليو = بدليو
                }
            });
            /*
             * Commit the temporary modifiers change before the key event
             * it was computed for: consumers must observe the key event
             * under the *redirected* modifiers.
             */
            darray_append(events->قطار, (بنية xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_چوکٺو,
            });
        }
    }

    darray_append(events->قطار, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_ڪنجي,
        .ڪنجي = {
            .keycode = redirect->keycode,
            .direction = direction
        }
    });

    إذا (نقاب && بدليو) {
        /* Restore state */
        darray_append(events->قطار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = آخري_components,
                .بدليو = بدليو
            }
        });
    }

    state->flags &= ~STATE_REQUIRE_ڪنجي_EVENT;
    أرجع true;
}

ساكن فراغ
xkb_filter_redirect_ڪنجي_new(بنية xkb_server_state *state,
                            بنية xkb_events *events,
                            بنية xkb_filter *filter)
{
    /* Action effectual only with the xkb_machine API and a valid keycode */
    إذا (!events || filter->action.redirect.keycode == XKB_KEYCODE_INVALID) {
        filter->func = عدم;
        أرجع;
    }
    append_redirect_ڪنجي_events(state, events, &filter->action.redirect,
                               XKB_ڪنجي_هيٺ_ڏانهن);
}

ساكن bool
xkb_filter_redirect_ڪنجي_func(بنية xkb_server_state *state,
                             بنية xkb_events *events,
                             بنية xkb_filter *filter,
                             ثابت بنية xkb_ڪنجي *ڪنجي,
                             تعداد xkb_ڪنجي_direction direction)
{
    إذا (ڪنجي != filter->ڪنجي)
        أرجع XKB_FILTER_CONTINUE;

    إذا (direction == XKB_ڪنجي_مٿي) {
        append_redirect_ڪنجي_events(state, events,
                                   &filter->action.redirect, XKB_ڪنجي_مٿي);
        filter->func = عدم;
        أرجع XKB_FILTER_CONSUME;
     } وإلا إذا (direction == XKB_ڪنجي_هيٺ_ڏانهن) {
        ثابت اتحاد xkb_action *actions = عدم;
        ثابت xkb_action_ڳڻپ_t ڳڻپ =
            xkb_ڪنجي_get_actions(&state->بنياد, ڪنجي, &actions);

        لكل (xkb_action_ڳڻپ_t a = 0; a < ڳڻپ; a++) {
            إذا (actions[a].type == ACTION_TYPE_REDIRECT_ڪنجي &&
                actions[a].redirect.keycode != filter->action.redirect.keycode) {
                    /* One action redirects to a different key: release first */
                    append_redirect_ڪنجي_events(state, events,
                                               &filter->action.redirect,
                                               XKB_ڪنجي_مٿي);
                filter->func = عدم;
                أرجع XKB_FILTER_CONTINUE;
            }
        }
    }

    append_redirect_ڪنجي_events(state, events, &filter->action.redirect,
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
                 ثابت بنية xkb_ڪنجي *ڪنجي, تعداد xkb_ڪنجي_direction direction);
} filter_action_funcs[ACTION_TYPE_انگ_ENTRIES] = {
    [ACTION_TYPE_MOD_سيٽ]     = { xkb_filter_mod_سيٽ_new,
                                  xkb_filter_mod_سيٽ_func },
    [ACTION_TYPE_MOD_LATCH]   = { xkb_filter_mod_latch_new,
                                  xkb_filter_mod_latch_func },
    [ACTION_TYPE_MOD_تالو]    = { xkb_filter_mod_تالو_new,
                                  xkb_filter_mod_تالو_func },
    [ACTION_TYPE_گروهه_سيٽ]   = { xkb_filter_گروهه_سيٽ_new,
                                  xkb_filter_گروهه_سيٽ_func },
    [ACTION_TYPE_گروهه_LATCH] = { xkb_filter_گروهه_latch_new,
                                  xkb_filter_گروهه_latch_func },
    [ACTION_TYPE_گروهه_تالو]  = { xkb_filter_گروهه_تالو_new,
                                  xkb_filter_گروهه_تالو_func },
    [ACTION_TYPE_PTR_MOVE]    = { xkb_filter_اشارو_move_new,
                                  xkb_filter_اشارو_move_func },
    [ACTION_TYPE_PTR_BUTTON]  = { xkb_filter_اشارو_button_new,
                                  xkb_filter_اشارو_button_func },
    [ACTION_TYPE_PTR_تالو]    = { xkb_filter_اشارو_تالو_button_new,
                                  xkb_filter_اشارو_تالو_button_func },
    [ACTION_TYPE_PTR_DEFAULT] = { xkb_filter_اشارو_سيٽ_default_button_new,
                                  xkb_filter_اشارو_سيٽ_default_button_func },
    [ACTION_TYPE_TERMINATE]   = { xkb_filter_terminate_server_new,
                                  xkb_filter_terminate_server_func },
    [ACTION_TYPE_SWITCH_VT]   = { xkb_filter_switch_virtual_console_new,
                                  xkb_filter_switch_virtual_console_func },
    [ACTION_TYPE_CTRL_سيٽ]    = { xkb_filter_ctrls_new,
                                  xkb_filter_ctrls_func },
    [ACTION_TYPE_CTRL_تالو]   = { xkb_filter_ctrls_new,
                                  xkb_filter_ctrls_func },
    [ACTION_TYPE_REDIRECT_ڪنجي] = { xkb_filter_redirect_ڪنجي_new,
                                   xkb_filter_redirect_ڪنجي_func },
};

/**
 * Applies any relevant filters to the key, first from the list of filters
 * that are currently active, then if no filter has claimed the key, possibly
 * apply a new filter from the key action.
 */
ساكن فراغ
xkb_filter_apply_all(بنية xkb_server_state *state,
                     بنية xkb_events *events,
                     ثابت بنية xkb_ڪنجي *ڪنجي,
                     تعداد xkb_ڪنجي_direction direction)
{
    /* First run through all the currently active filters and see if any of
     * them have consumed this event. */
    bool consumed = false;
    بنية xkb_filter *filter;
    darray_foreach(filter, state->filters) {
        إذا (!filter->func)
            استمر;

        إذا (filter->func(state, events, filter, ڪنجي, direction) == XKB_FILTER_CONSUME)
            consumed = true;
    }
    إذا (consumed || direction == XKB_ڪنجي_مٿي)
        أرجع;

    /* No filter consumed this event, so proceed with the key actions */
    ثابت اتحاد xkb_action *actions = عدم;
    xkb_action_ڳڻپ_t ڳڻپ = xkb_ڪنجي_get_actions(&state->بنياد, ڪنجي,
                                                         &actions);

    لكل (بنية {
            xkb_action_ڳڻپ_t ڳڻپ;
            ثابت اتحاد xkb_action *actions;
         } ايندڙ = {0};
         ڳڻپ;
         ڳڻپ = ايندڙ.ڳڻپ, ايندڙ.ڳڻپ = 0, actions = ايندڙ.actions
    ) {
        /*
         * Process actions sequentially.
         *
         * NOTE: We rely on the parser to disallow multiple modifier or group
         * actions (see `CheckMultipleActionsCategories`). Allowing multiple
         * such actions requires a refactor of the state handling.
         */
        لكل (xkb_action_ڳڻپ_t k = 0; k < ڳڻپ; k++) {
            /*
             * It's possible for the keymap to set action->type explicitly, like
             * so:
             *     interpret XF86_Next_VMode {
             *         action = Private(type=0x86, data="+VMode");
             *     };
             * We don't handle those.
             */
            إذا (actions[k].type >= ACTION_TYPE_انگ_ENTRIES)
                استمر;

            /* Go to next action if no corresponding action handler */
            إذا (!filter_action_funcs[actions[k].type].new)
                استمر;

            /* Add a new filter and run the corresponding initial action */
            filter = xkb_filter_new(state);
            filter->ڪنجي = ڪنجي;
            filter->action = actions[k];

            بدل (filter->action.type) {
            حالة ACTION_TYPE_MOD_سيٽ:
                إذا (state->بنياد.components.controls & CONTROL_STICKY_ڪنجيون) {
                    /* Convert modifier set action to a latch */
                    filter->action.type = ACTION_TYPE_MOD_LATCH;
                    إذا (state->a11y_flags & XKB_A11_Y_STICKY_ڪنجيون_LATCH_منزل_تالو) {
                        filter->action.mods.flags |= ACTION_LATCH_منزل_تالو;
                    }
                }
                /* fallthrough */
            حالة ACTION_TYPE_MOD_LATCH:
            حالة ACTION_TYPE_MOD_تالو: {
                ساكن ثابت تعداد xkb_action_flags unlock_mods_on_press
                    = ACTION_تالو_CLEAR     /* SetMods & LatchMods: can unlock */
                    | ACTION_تالو_NO_UNLOCK /* LockMods: cannot unlock */
                    | ACTION_EXPLICIT_UNLOCK_ON_PRESS;
                إذا ((state->flags & STATE_UNLOCK_MODS_ON_PRESS) &&
                    /*
                     * Ensure that the action can actually unlock and has
                     * no *explicit* `unlockOnPress` parameter.
                     */
                    (filter->action.mods.flags & unlock_mods_on_press) ==
                    (filter->action.type == ACTION_TYPE_MOD_تالو
                        ? 0 : ACTION_تالو_CLEAR))
                {
                    filter->action.mods.flags |= ACTION_UNLOCK_ON_PRESS;
                }
                اكسر;
            }
            حالة ACTION_TYPE_گروهه_سيٽ:
                إذا (state->بنياد.components.controls & CONTROL_STICKY_ڪنجيون) {
                    /* Convert group set action to a latch */
                    filter->action.type = ACTION_TYPE_گروهه_LATCH;
                    إذا (state->a11y_flags & XKB_A11_Y_STICKY_ڪنجيون_LATCH_منزل_تالو) {
                        filter->action.گروهه.flags |= ACTION_LATCH_منزل_تالو;
                    }
                }
                اكسر;
            حالة ACTION_TYPE_گروهه_تالو:
                إذا ((filter->action.گروهه.flags &
                     ACTION_AUTO_گروهه_تالو_ON_MODIFIER_RELEASE) &&
                    (state->flags &
                     STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_ڪنجي) &&
                    /*
                     * FIXME: sticky keys may leave an unexpected modifier
                     * from the level 0 latched, so disable for now.
                     */
                    !(state->بنياد.components.controls & CONTROL_STICKY_ڪنجيون))
                {
                    filter->action.گروهه.flags |= ACTION_تالو_ON_RELEASE;
                    /* Add base level action */
                    ايندڙ.ڳڻپ = xkb_keymap_ڪنجي_get_actions_by_level(
                        state->بنياد.keymap, ڪنجي, state->بنياد.components.گروهه,
                        0, &ايندڙ.actions
                    );
                    أكد(ايندڙ.ڳڻپ == 1);
                }
                اكسر;
            حالة ACTION_TYPE_PTR_MOVE:
            حالة ACTION_TYPE_PTR_BUTTON:
            حالة ACTION_TYPE_PTR_تالو:
            حالة ACTION_TYPE_PTR_DEFAULT:
                إذا (!(state->بنياد.components.controls & CONTROL_MOUSE_ڪنجيون)) {
                    /* Convert pointer actions to NoAction */
                    أكد(!filter_action_funcs[ACTION_TYPE_NONE].new);
                    filter->func = عدم;
                    استمر;
                }
                اكسر;
            حالة ACTION_TYPE_REDIRECT_ڪنجي:
                // FIXME: this is not efficient to resolve mods here each time
                filter->action.redirect.affect_mods = mod_نقاب_get_effective(
                    state->بنياد.keymap, filter->action.redirect.affect_mods
                );
                filter->action.redirect.mods = mod_نقاب_get_effective(
                    state->بنياد.keymap, filter->action.redirect.mods
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
               تعداد xkb_state_گهڻائي_اندروني گهڻائي)
{
    state->گهڻائي = گهڻائي;
    state->refcnt = 1;
    state->keymap = xkb_keymap_ref(keymap);
    state->ٻاهريون_جو_range_گروهه.policy = XKB_LAYOUT_ٻاهريون_جو_RANGE_WRAP;

    /* Ensure that derived state is correctly initialized */
    xkb_state_update_derived(state);
}

ساكن مضمن فراغ
xkb_client_state_init(بنية xkb_client_state * محدود state,
                      بنية xkb_keymap * محدود keymap,
                      تعداد xkb_state_گهڻائي_اندروني گهڻائي)
{
    xkb_state_init(&state->بنياد, keymap, گهڻائي);
}

ساكن مضمن فراغ
xkb_server_state_init(بنية xkb_server_state * محدود state,
                      بنية xkb_keymap * محدود keymap,
                      تعداد xkb_state_گهڻائي_اندروني گهڻائي,
                      تعداد xkb_a11y_flags a11y_affect,
                      تعداد xkb_a11y_flags a11y_flags)
{
    xkb_state_init(&state->بنياد, keymap, گهڻائي);

    state->a11y_flags = a11y_flags;
    إذا (keymap->format != XKB_KEYMAP_FORMAT_متن_V1 &&
        !(a11y_affect & XKB_A11_Y_LATCH_گڏو_گڏ_ڪنجيون)) {
             /* Keymap v2+: enable extension to XKB if not manually disabled */
             state->a11y_flags |= XKB_A11_Y_LATCH_گڏو_گڏ_ڪنجيون;
    }
}

بنية xkb_state *
xkb_state_new(بنية xkb_keymap *keymap)
{
    بنية xkb_server_state * ثابت state = صفر_يادگيري_ڏيو(1, حجم(*state));
    إذا (!state) {
        log_err_func1(keymap->ctx, XKB_ERROR_ورهاست_FAILURE_2,
                      "Could not allocate a state object.\n");
        أرجع عدم;
    }

    xkb_server_state_init(state, keymap, LEGACY_MIXED_STATE, 0, 0);

    أرجع (بنية xkb_state *)state;
}

بنية xkb_state *
xkb_state_new_with_گهڻائي(بنية xkb_keymap * محدود keymap,
                        تعداد xkb_state_گهڻائي گهڻائي,
                        تعداد xkb_حالت * محدود حالت)
{
    بدل (گهڻائي) {
    حالة XKB_STATE_گهڻائي_CLIENT:
    حالة XKB_STATE_گهڻائي_SERVER_QUERY: {
        بنية xkb_client_state * ثابت state = صفر_يادگيري_ڏيو(1, حجم(*state));
        إذا (!state)
            اكسر;

        static_assert((بنانشان)XKB_STATE_گهڻائي_CLIENT ==
                      (بنانشان)CLIENT_STATE, "");
        static_assert((بنانشان)XKB_STATE_گهڻائي_SERVER_QUERY ==
                      (بنانشان)SERVER_COMPANION, "");
        xkb_client_state_init(state, keymap, (تعداد xkb_state_گهڻائي_اندروني)گهڻائي);

        إذا (حالت)
            *حالت = XKB_SUCCESS;

        أرجع (بنية xkb_state *)state;
    }
    حالة XKB_STATE_گهڻائي_SERVER: {
        بنية xkb_server_state * ثابت state = صفر_يادگيري_ڏيو(1, حجم(*state));
        إذا (!state)
            اكسر;

        xkb_server_state_init(state, keymap, LEGACY_SERVER_STATE, 0, 0);

        إذا (حالت)
            *حالت = XKB_SUCCESS;

        أرجع (بنية xkb_state *)state;
    }
    افتراضي:
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_STATE_گهڻائي_2,
                     "Unsupported state mode: %u\n", گهڻائي);
        إذا (حالت)
            *حالت = XKB_ERROR_UNSUPPORTED_STATE_گهڻائي;
        أرجع عدم;
    }

    log_err_func1(keymap->ctx, XKB_ERROR_ورهاست_FAILURE_2,
                  "Could not allocate a state object.\n");
    إذا (حالت)
        *حالت = XKB_ERROR_ورهاست_FAILURE;
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

    إذا (state->گهڻائي < LEGACY_MIXED_STATE) {
        static_assert(حجم(*state) == حجم(بنية xkb_client_state),
                      "Not a mere wrapper");
        /* Nothing else to reset */
    } وإلا {
        بنية xkb_server_state *state_ = (بنية xkb_server_state *)state;
        ثابت بنية xkb_server_state prev = *state_;
        memset(state_, 0, حجم(*state_));

        state_->بنياد = prev.بنياد;
        state_->a11y_flags = prev.a11y_flags;
        state_->filters = prev.filters;
        darray_سائيز(state_->filters) = 0;
    }
}

ساكن مضمن فراغ
xkb_state_destroy(بنية xkb_state *state)
{
    xkb_keymap_unref(state->keymap);
    إذا (state->گهڻائي > SERVER_COMPANION)
        darray_free(((بنية xkb_server_state *)state)->filters);
}

فراغ
xkb_state_unref(بنية xkb_state *state)
{
    أكد(!state || state->refcnt > 0);
    إذا (!state || --state->refcnt > 0)
        أرجع;

    xkb_state_destroy(state);

    بدل (state->گهڻائي) {
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
    xkb_led_اشاريو_t idx;
    ثابت بنية xkb_led *led;

    state->components.leds = 0;

    xkb_leds_enumerate(idx, led, state->keymap) {

        إذا (led->which_mods != 0 && led->mods.نقاب != 0) {
            xkb_mod_نقاب_t mod_نقاب = 0;
            إذا (led->which_mods & XKB_STATE_MODS_EFFECTIVE)
                mod_نقاب |= state->components.mods;
            إذا (led->which_mods & XKB_STATE_MODS_DEPRESSED)
                mod_نقاب |= state->components.بنياد_mods;
            إذا (led->which_mods & XKB_STATE_MODS_LATCHED)
                mod_نقاب |= state->components.latched_mods;
            إذا (led->which_mods & XKB_STATE_MODS_LOCKED)
                mod_نقاب |= state->components.locked_mods;

            إذا (led->mods.نقاب & mod_نقاب) {
                state->components.leds |= (UINT32_C(1) << idx);
                استمر;
            }
        }

        إذا (led->which_groups != 0) {
            إذا (likely(led->groups) != 0) {
                xkb_layout_نقاب_t گروهه_نقاب = 0;
                /* Effective and locked groups have been brought into range */
                أكد(state->components.گروهه < XKB_MAX_GROUPS);
                أكد(state->components.locked_گروهه >= 0 &&
                       state->components.locked_گروهه < XKB_MAX_GROUPS);
                /* Effective and locked groups are used as mask */
                إذا (led->which_groups & XKB_STATE_LAYOUT_EFFECTIVE)
                    گروهه_نقاب |= (UINT32_C(1) << state->components.گروهه);
                إذا (led->which_groups & XKB_STATE_LAYOUT_LOCKED)
                    گروهه_نقاب |= (UINT32_C(1) << state->components.locked_گروهه);
                /* Base and latched groups only have to be non-zero */
                إذا ((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                    state->components.بنياد_گروهه != 0)
                    گروهه_نقاب |= led->groups;
                إذا ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                    state->components.latched_گروهه != 0)
                    گروهه_نقاب |= led->groups;

                إذا (led->groups & گروهه_نقاب) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    استمر;
                }
            } وإلا {
                /* Special case for Base and latched groups */
                إذا (((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                     state->components.بنياد_گروهه == 0) ||
                    ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                     state->components.latched_گروهه == 0)) {
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
    xkb_layout_اشاريو_t wrapped;

    state->components.mods = (state->components.بنياد_mods |
                              state->components.latched_mods |
                              state->components.locked_mods);

    /* Lock group must be adjusted, but not base nor latched groups */
    wrapped = Xkb_Wrap_گروهه_Into_Range(state->components.locked_گروهه,
                                    state->keymap->انگ_groups,
                                    state->ٻاهريون_جو_range_گروهه.policy,
                                    state->ٻاهريون_جو_range_گروهه.redirect_گروهه);
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->components.locked_گروهه =
        (int32_t) (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    /* Effective group must be adjusted */
    wrapped = Xkb_Wrap_گروهه_Into_Range(state->components.بنياد_گروهه +
                                    state->components.latched_گروهه +
                                    state->components.locked_گروهه,
                                    state->keymap->انگ_groups,
                                    state->ٻاهريون_جو_range_گروهه.policy,
                                    state->ٻاهريون_جو_range_گروهه.redirect_گروهه);
    state->components.گروهه =
        (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    xkb_state_led_update_all(state);
}

ساكن فراغ
state_update_enabled_controls(بنية xkb_server_state *state,
                              تعداد xkb_keyboard_control_flags affect_controls,
                              تعداد xkb_keyboard_control_flags controls,
                              بنية xkb_events *events)
{
    ثابت bool had_sticky_ڪنجيون = state->بنياد.components.controls
                               & CONTROL_STICKY_ڪنجيون;

    /*
     * Enable using the public API with the all the Control values, except
     * the internal ones, if any.
     */
    affect_controls =
        ( affect_controls
        & (تعداد xkb_keyboard_control_flags)CONTROL_ALL_BOOLEAN );
    state->بنياد.components.controls &=
        (تعداد xkb_action_controls)~affect_controls;
    state->بنياد.components.controls |=
        (تعداد xkb_action_controls)(controls & affect_controls);

    إذا (had_sticky_ڪنجيون &&
        !(state->بنياد.components.controls & CONTROL_STICKY_ڪنجيون)) {
        /* Sticky keys were disabled: clear all locks and latches */
        clear_all_latches_and_locks(state, events);
    }

    xkb_state_update_derived(&state->بنياد);
}

/**
 * Given a particular key event, updates the state structure to reflect the
 * new modifiers.
 */
تعداد xkb_state_component
xkb_state_update_ڪنجي(بنية xkb_state *بنياد_state, xkb_keycode_t kc,
                     تعداد xkb_ڪنجي_direction direction)
{
    /* Guard against client-only state */
    أكد(بنياد_state->گهڻائي > SERVER_COMPANION);
    إذا (بنياد_state->گهڻائي <= SERVER_COMPANION) {
        log_err(بنياد_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_گهڻائي,
                "%s: Unexpected state type %d\n", __func__, بنياد_state->گهڻائي);
        أرجع 0;
    }

    بنية xkb_server_state * ثابت state =
        (بنية xkb_server_state *)بنياد_state;

    ثابت بنية xkb_ڪنجي* ثابت ڪنجي = Xkb_ڪنجي(state->بنياد.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    إذا (!ڪنجي || (direction == XKB_ڪنجي_REPEATED && !ڪنجي->repeats))
        أرجع 0;

    إذا (direction == XKB_ڪنجي_هيٺ_ڏانهن &&
        (state->بنياد.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ڪنجيون) &&
        (state->a11y_flags & XKB_A11_Y_STICKY_ڪنجيون_NO_گڏو_گڏ_ڪنجيون) &&
        state->بنياد.components.بنياد_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ڪنجيون,
                                      0, عدم);
    }

    ثابت بنية state_components prev_components = state->بنياد.components;

    state->سيٽ_mods = 0;
    state->clear_mods = 0;

    xkb_filter_apply_all(state, عدم, ڪنجي, direction);

    xkb_mod_اشاريو_t i;
    xkb_mod_نقاب_t ٻٽو_انگ;
    لكل (i = 0, ٻٽو_انگ = 1; state->سيٽ_mods; i++, ٻٽو_انگ <<= 1) {
        إذا (state->سيٽ_mods & ٻٽو_انگ) {
            state->mod_ڪنجي_ڳڻپ[i]++;
            state->بنياد.components.بنياد_mods |= ٻٽو_انگ;
            state->سيٽ_mods &= ~ٻٽو_انگ;
        }
    }

    لكل (i = 0, ٻٽو_انگ = 1; state->clear_mods; i++, ٻٽو_انگ <<= 1) {
        إذا (state->clear_mods & ٻٽو_انگ) {
            state->mod_ڪنجي_ڳڻپ[i]--;
            إذا (state->mod_ڪنجي_ڳڻپ[i] <= 0) {
                state->بنياد.components.بنياد_mods &= ~ٻٽو_انگ;
                state->mod_ڪنجي_ڳڻپ[i] = 0;
            }
            state->clear_mods &= ~ٻٽو_انگ;
        }
    }

    xkb_state_update_derived(&state->بنياد);

    أرجع get_state_component_changes(&prev_components, &state->بنياد.components);
}

/* We need fake keys for `update_latch_modifiers` and `update_latch_group`.
 * These keys must have at least one level in order to break latches. We need 2
 * keys with specific actions in order to update group/mod latches without
 * affecting each other. */
ساكن بنية xkb_ڪنجي_type_داخلا synthetic_ڪنجي_level_داخلا = { 0 };
ساكن بنية xkb_ڪنجي_type synthetic_ڪنجي_type = {
     .انگ_entries = 1,
     .انگ_سطحون = 1,
     .entries = &synthetic_ڪنجي_level_داخلا
};
ساكن ثابت بنية xkb_ڪنجي synthetic_ڪنجي = { 0 };

/* Transcription from xserver: XkbLatchModifiers */
ساكن فراغ
update_latch_modifiers(بنية xkb_server_state *state,
                       بنية xkb_events *events,
                       xkb_mod_نقاب_t نقاب, xkb_mod_نقاب_t latches)
{
    /* Clear affected latched modifiers */
    ثابت xkb_mod_نقاب_t clear = نقاب & ~latches;
    state->بنياد.components.latched_mods &= ~clear;

    /* Clear any pending latch to locks using ad hoc action:
     * only affect corresponding modifier latches and no group latch. */
    بنية xkb_level synthetic_ڪنجي_level_break_mod_latch = {
        .انگ_syms = 0,
        .انگ_actions = 1,
        .مٿانهون = XKB_ڪنجي_No_نشان,
        .s.sym = XKB_ڪنجي_No_نشان,
        .a.action.اندروني = {
            .type = ACTION_TYPE_اندروني,
            .flags = اندروني_BREAKS_MOD_LATCH,
            .clear_latched_mods = clear
        }
    };
    بنية xkb_گروهه synthetic_ڪنجي_گروهه_break_mod_latch = {
        .type = &synthetic_ڪنجي_type,
        .سطحون = &synthetic_ڪنجي_level_break_mod_latch
    };
    ثابت بنية xkb_ڪنجي synthetic_ڪنجي_break_mod_latch = {
        .انگ_groups = 1,
        .groups = &synthetic_ڪنجي_گروهه_break_mod_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_ڪنجي_break_mod_latch, XKB_ڪنجي_هيٺ_ڏانهن);

    /* Finally set the latched mods by simulate tapping a key with the
     * corresponding action */
    ثابت بنية xkb_ڪنجي* ثابت ڪنجي = &synthetic_ڪنجي;
    ثابت اتحاد xkb_action latch_mods = {
        .mods = {
            .type = ACTION_TYPE_MOD_LATCH,
            .mods = { .نقاب = نقاب & latches },
            .flags = 0,
        },
    };
    بنية xkb_filter* ثابت filter = xkb_filter_new(state);
    filter->ڪنجي = ڪنجي;
    filter->func = xkb_filter_mod_latch_func;
    filter->action = latch_mods;
    xkb_filter_mod_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_mod_latch_func(state, events, filter, ڪنجي, XKB_ڪنجي_مٿي);
}

/* Transcription from xserver: XkbLatchGroup */
ساكن فراغ
update_latch_گروهه(بنية xkb_server_state *state,
                   بنية xkb_events *events, int32_t گروهه)
{
    /* Clear any pending latch to locks. */
    ساكن بنية xkb_level synthetic_ڪنجي_level_break_گروهه_latch = {
        .انگ_syms = 0,
        .انگ_actions = 1,
        .مٿانهون = XKB_ڪنجي_No_نشان,
        .s.sym = XKB_ڪنجي_No_نشان,
        .a.action.اندروني = {
            .type = ACTION_TYPE_اندروني,
            .flags = اندروني_BREAKS_گروهه_LATCH,
            .clear_latched_mods = 0
        }
    };
    ساكن بنية xkb_گروهه synthetic_ڪنجي_گروهه_break_گروهه_latch = {
        .type = &synthetic_ڪنجي_type,
        .سطحون = &synthetic_ڪنجي_level_break_گروهه_latch
    };
    ساكن ثابت بنية xkb_ڪنجي synthetic_ڪنجي_break_گروهه_latch = {
        .انگ_groups = 1,
        .groups = &synthetic_ڪنجي_گروهه_break_گروهه_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_ڪنجي_break_گروهه_latch, XKB_ڪنجي_هيٺ_ڏانهن);

    /* Simulate tapping a key with a group latch action, but in isolation: i.e.
     * without affecting the other filters. */
    ثابت بنية xkb_ڪنجي* ثابت ڪنجي = &synthetic_ڪنجي;
    ثابت اتحاد xkb_action latch_گروهه = {
        .گروهه = {
            .type = ACTION_TYPE_گروهه_LATCH,
            .flags = ACTION_ABSOLUTE_SWITCH,
            .گروهه = گروهه,
        },
    };
    بنية xkb_filter* ثابت filter = xkb_filter_new(state);
    filter->ڪنجي = ڪنجي;
    filter->func = xkb_filter_گروهه_latch_func;
    filter->action = latch_گروهه;
    xkb_filter_گروهه_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_گروهه_latch_func(state, events, filter, ڪنجي, XKB_ڪنجي_مٿي);
}

/**
 * Compute the resolved effective mask of an arbitrary input.
 *
 * Contrary to `mod_mask_get_effective`, it resolves only modifiers not present
 * in the canonical mask, so that it enables `xkb_state_serialize_mods` to
 * round trip via `xkb_state_update_mask`.
 */
ساكن مضمن xkb_mod_نقاب_t
resolve_منزل_canonical_mods(بنية xkb_keymap *keymap, xkb_mod_نقاب_t mods)
{
    أرجع
        /*
         * Keep canonical modifier mask.
         * It contains either real modifiers or canonical virtual modifiers.
         */
        (mods & keymap->canonical_state_نقاب) |
        /* Resolve other modifiers */
        mod_نقاب_get_effective(keymap,
                               mods & ~keymap->canonical_state_نقاب);
}

ساكن فراغ
state_update_latched_locked(
    بنية xkb_server_state * محدود state,
    ثابت بنية xkb_state_components_update * محدود update,
    بنية xkb_events * محدود events
)
{
    /* Update locks */
    ثابت xkb_mod_نقاب_t affect_locked_mods =
        resolve_منزل_canonical_mods(state->بنياد.keymap, update->affect_locked_mods);
    إذا ((update->components & XKB_STATE_MODS_LOCKED) &&
        affect_locked_mods) {
        ثابت xkb_mod_نقاب_t locked_mods =
            resolve_منزل_canonical_mods(state->بنياد.keymap, update->locked_mods);
        state->بنياد.components.locked_mods &= ~affect_locked_mods;
        state->بنياد.components.locked_mods |= locked_mods & affect_locked_mods;
    }
    إذا (update->components & XKB_STATE_LAYOUT_LOCKED) {
        state->بنياد.components.locked_گروهه = update->locked_layout;
    }

    /* Update latches */
    ثابت xkb_mod_نقاب_t affect_latched_mods =
        resolve_منزل_canonical_mods(state->بنياد.keymap, update->affect_latched_mods);
    إذا ((update->components & XKB_STATE_MODS_LATCHED) &&
        affect_latched_mods) {
        ثابت xkb_mod_نقاب_t latched_mods =
            resolve_منزل_canonical_mods(state->بنياد.keymap, update->latched_mods);
        update_latch_modifiers(state, events, affect_latched_mods, latched_mods);
    }
    إذا (update->components & XKB_STATE_LAYOUT_LATCHED) {
        update_latch_گروهه(state, events, update->latched_layout);
    }
}

تعداد xkb_state_component
xkb_state_update_latched_locked(بنية xkb_state *بنياد_state,
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
    أكد(بنياد_state->گهڻائي > SERVER_COMPANION);
    إذا (بنياد_state->گهڻائي <= SERVER_COMPANION) {
        log_err(بنياد_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_گهڻائي,
                "%s: Unexpected state type %d\n", __func__, بنياد_state->گهڻائي);
        أرجع 0;
    }

    بنية xkb_server_state * ثابت state =
        (بنية xkb_server_state *)بنياد_state;

    ثابت بنية state_components اڳيون_components = state->بنياد.components;
    ثابت تعداد xkb_state_component components
        = ((affect_latched_mods || latched_mods) ? XKB_STATE_MODS_LATCHED : 0)
        | ((affect_locked_mods || locked_mods) ? XKB_STATE_MODS_LOCKED : 0)
        | (affect_latched_layout ? XKB_STATE_LAYOUT_LATCHED : 0)
        | (affect_locked_layout ? XKB_STATE_LAYOUT_LOCKED : 0);
    ثابت بنية xkb_state_components_update update = {
        .سائيز = حجم(update),
        .components = components,
        .affect_latched_mods = affect_latched_mods,
        .latched_mods = latched_mods,
        .latched_layout = latched_layout,
        .affect_locked_mods = affect_locked_mods,
        .locked_mods = locked_mods,
        .locked_layout = locked_layout
    };
    state_update_latched_locked(state, &update, عدم);
    xkb_state_update_derived(&state->بنياد);
    أرجع get_state_component_changes(&اڳيون_components,
                                       &state->بنياد.components);
}

ساكن مضمن فراغ
clear_all_latches_and_locks(بنية xkb_server_state *state,
                            بنية xkb_events *events)
{
    ساكن ثابت تعداد xkb_state_component components
        = XKB_STATE_MODS_LATCHED | XKB_STATE_MODS_LOCKED
        | XKB_STATE_LAYOUT_LATCHED | XKB_STATE_LAYOUT_LOCKED;
    ثابت بنية xkb_state_components_update update = {
        .سائيز = حجم(update),
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

ساكن تعداد xkb_حالت
state_update_layout_policy(بنية xkb_server_state *state,
                           ثابت بنية xkb_layout_policy_update *update)
{
    إذا (xkb_feature_supported(XKB_FEATURE_ENUM_LAYOUT_ٻاهريون_جو_RANGE_POLICY,
                              update->policy)) {
        إذا (update->policy == XKB_LAYOUT_ٻاهريون_جو_RANGE_REDIRECT) {
            إذا (update->redirect < state->بنياد.keymap->انگ_groups) {
                state->بنياد.ٻاهريون_جو_range_گروهه.redirect_گروهه =
                    update->redirect;
            } وإلا {
                log_err(state->بنياد.keymap->ctx,
                        XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو_2,
                        "Layout policy: "
                        "unsupported layout index %"PRIu32" > %"PRIu32"\n",
                        update->redirect + 1, state->بنياد.keymap->انگ_groups);
                أرجع XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو;
            }
        }
        state->بنياد.ٻاهريون_جو_range_گروهه.policy = update->policy;
        أرجع XKB_SUCCESS;
    } وإلا {
        log_err(state->بنياد.keymap->ctx,
                XKB_ERROR_UNSUPPORTED_LAYOUT_ٻاهريون_جو_RANGE_POLICY_2,
                "Unsupported layout policy: %d\n", update->policy);
        أرجع XKB_ERROR_UNSUPPORTED_LAYOUT_ٻاهريون_جو_RANGE_POLICY;
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

#وصف_ڪريو check_state_update_abi(ctx, update) \
    check_state_update_abi_(ctx, __func__, update)

تعداد xkb_حالت
xkb_state_update_synthetic(بنية xkb_state * بنياد_state,
                           ثابت بنية xkb_synthetic_update * update,
                           تعداد xkb_state_component *بدليو)
{
    /* Guard against client-only state */
    إذا (بنياد_state->گهڻائي <= SERVER_COMPANION) {
        log_err(بنياد_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_گهڻائي,
                "%s: Unexpected state type %d\n", __func__, بنياد_state->گهڻائي);
        أرجع XKB_ERROR_UNEXPECTED_STATE_گهڻائي;
    }

    بنية xkb_server_state * ثابت state =
        (بنية xkb_server_state *)بنياد_state;

    /* Check ABI compatibility */
    تعداد xkb_حالت حالت = check_state_update_abi(state->بنياد.keymap->ctx,
                                                    update);
    إذا (حالت != XKB_SUCCESS)
        أرجع حالت;

    ثابت بنية state_components اڳيون_components = state->بنياد.components;

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

    xkb_state_update_derived(&state->بنياد);

    إذا (بدليو) {
        *بدليو = get_state_component_changes(&اڳيون_components,
                                               &state->بنياد.components);
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
xkb_state_update_نقاب(بنية xkb_state *بنياد_state,
                      xkb_mod_نقاب_t بنياد_mods,
                      xkb_mod_نقاب_t latched_mods,
                      xkb_mod_نقاب_t locked_mods,
                      xkb_layout_اشاريو_t بنياد_گروهه,
                      xkb_layout_اشاريو_t latched_گروهه,
                      xkb_layout_اشاريو_t locked_گروهه)
{
    /* Guard against server-only and server companion state */
    أكد(بنياد_state->گهڻائي == CLIENT_STATE ||
           بنياد_state->گهڻائي == LEGACY_MIXED_STATE);
    إذا (بنياد_state->گهڻائي != CLIENT_STATE &&
        بنياد_state->گهڻائي != LEGACY_MIXED_STATE) {
        log_err(بنياد_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_گهڻائي,
                "%s: Unexpected state type %d\n", __func__, بنياد_state->گهڻائي);
        أرجع 0;
    }

    بنية xkb_client_state * ثابت state =
        (بنية xkb_client_state *)بنياد_state;

    ثابت بنية state_components prev_components = state->بنياد.components;

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
    state->بنياد.components.بنياد_mods =
        resolve_منزل_canonical_mods(state->بنياد.keymap, بنياد_mods);
    state->بنياد.components.latched_mods =
        resolve_منزل_canonical_mods(state->بنياد.keymap, latched_mods);
    state->بنياد.components.locked_mods =
        resolve_منزل_canonical_mods(state->بنياد.keymap, locked_mods);

    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->بنياد.components.بنياد_گروهه = (int32_t) بنياد_گروهه;
    state->بنياد.components.latched_گروهه = (int32_t) latched_گروهه;
    state->بنياد.components.locked_گروهه = (int32_t) locked_گروهه;

    xkb_state_update_derived(&state->بنياد);

    أرجع get_state_component_changes(&prev_components, &state->بنياد.components);
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Lock_Modifier
 */
ساكن bool
should_do_caps_transformation(بنية xkb_state *state, xkb_keycode_t kc)
{
    أرجع
        xkb_state_mod_اشاريو_is_سرگرم(state, XKB_MOD_اشاريو_CAPS,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_اشاريو_is_consumed(state, kc, XKB_MOD_اشاريو_CAPS) == 0;
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Control_Modifier
 */
ساكن bool
should_do_ctrl_transformation(بنية xkb_state *state, xkb_keycode_t kc)
{
    أرجع
        xkb_state_mod_اشاريو_is_سرگرم(state, XKB_MOD_اشاريو_CTRL,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_اشاريو_is_consumed(state, kc, XKB_MOD_اشاريو_CTRL) == 0;
}

/**
 * Provides the symbols to use for the given key and state.  Returns the
 * number of symbols pointed to in syms_out.
 */
صحيح
xkb_state_ڪنجي_get_syms(بنية xkb_state *state, xkb_keycode_t kc,
                       ثابت xkb_keysym_t **syms_ٻاهريون)
{
    ثابت xkb_layout_اشاريو_t layout = xkb_state_ڪنجي_get_layout(state, kc);
    إذا (layout == XKB_LAYOUT_INVALID)
        اذهب err;

    ثابت xkb_level_اشاريو_t level = xkb_state_ڪنجي_get_level(state, kc, layout);
    إذا (level == XKB_LEVEL_INVALID)
        اذهب err;

    ثابت بنية xkb_ڪنجي* ثابت ڪنجي = Xkb_ڪنجي(state->keymap, kc);

    إذا (!ڪنجي)
        اذهب err;

    ثابت بنية xkb_level* ثابت leveli =
        xkb_keymap_ڪنجي_get_level(state->keymap, ڪنجي, layout, level);

    إذا (!leveli)
        اذهب err;

    ثابت xkb_keysym_ڳڻپ_t انگ_syms = leveli->انگ_syms;
    إذا (انگ_syms == 0)
        اذهب err;

    إذا (should_do_caps_transformation(state, kc)) {
        /* Only simple capitalization rules: keysyms count is unchanged. */
        إذا (انگ_syms > 1) {
            *syms_ٻاهريون = (leveli->has_مٿانهون)
                      ? leveli->s.syms + انگ_syms
                      : leveli->s.syms;
        } وإلا {
            *syms_ٻاهريون = &leveli->مٿانهون;
        }
    } وإلا {
        *syms_ٻاهريون = (انگ_syms > 1)
                  ? leveli->s.syms
                  : &leveli->s.sym;
    }
    أرجع (صحيح) انگ_syms;

err:
    *syms_ٻاهريون = عدم;
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
xkb_state_ڪنجي_get_one_sym(بنية xkb_state *state, xkb_keycode_t kc)
{
    ثابت xkb_keysym_t *syms = عدم;
    ثابت صحيح انگ_syms = xkb_state_ڪنجي_get_syms(state, kc, &syms);

    إذا (انگ_syms != 1)
        أرجع XKB_ڪنجي_No_نشان;
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
    ثابت xkb_layout_اشاريو_t layout = xkb_state_ڪنجي_get_layout(state, kc);
    ثابت xkb_layout_اشاريو_t انگ_layouts =
        xkb_keymap_انگ_layouts_for_ڪنجي(state->keymap, kc);
    xkb_level_اشاريو_t level = xkb_state_ڪنجي_get_level(state, kc, layout);
    إذا (layout == XKB_LAYOUT_INVALID || انگ_layouts == 0 ||
        level == XKB_LEVEL_INVALID)
        أرجع XKB_ڪنجي_No_نشان;

    ثابت xkb_keysym_t *syms = عدم;
    صحيح nsyms = xkb_keymap_ڪنجي_get_syms_by_level(state->keymap, kc,
                                                 layout, level, &syms);
    إذا (nsyms != 1)
        أرجع XKB_ڪنجي_No_نشان;
    xkb_keysym_t sym = syms[0];

    إذا (should_do_ctrl_transformation(state, kc) && sym > 127u) {
        لكل (xkb_layout_اشاريو_t i = 0; i < انگ_layouts; i++) {
            level = xkb_state_ڪنجي_get_level(state, kc, i);
            إذا (level == XKB_LEVEL_INVALID)
                استمر;

            nsyms = xkb_keymap_ڪنجي_get_syms_by_level(state->keymap, kc,
                                                     i, level, &syms);
            إذا (nsyms == 1 && syms[0] <= 127u) {
                sym = syms[0];
                اكسر;
            }
        }
    }

    إذا (should_do_caps_transformation(state, kc)) {
        sym = xkb_keysym_منزل_مٿانهون(sym);
    }

    أرجع sym;
}

صحيح
xkb_state_ڪنجي_get_utf8(بنية xkb_state *state, xkb_keycode_t kc,
                       حرف *بفر, سائيز_t سائيز)
{
    صحيح nsyms;
    ثابت xkb_keysym_t *syms = عدم;
    ثابت xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    إذا (sym != XKB_ڪنجي_No_نشان) {
        nsyms = 1; syms = &sym;
    }
    وإلا {
        nsyms = xkb_state_ڪنجي_get_syms(state, kc, &syms);
    }

    /* Make sure not to truncate in the middle of a UTF-8 sequence. */
    صحيح وٿي = 0;
    حرف tmp[XKB_KEYSYM_UTF8_MAX_سائيز];
    لكل (صحيح i = 0; i < nsyms; i++) {
        صحيح ret = xkb_keysym_منزل_utf8(syms[i], tmp, حجم(tmp));
        إذا (ret <= 0)
            اذهب err_bad;

        ret--;
        إذا ((سائيز_t)وٿي + (سائيز_t)ret <= سائيز)
            يادگيري_نقل(بفر + وٿي, tmp, (سائيز_t)ret);
        وٿي += ret;
    }

    إذا ((سائيز_t)وٿي >= سائيز)
        اذهب err_trunc;
    بفر[وٿي] = '\0';

    إذا (!is_valid_utf8(بفر, (سائيز_t)وٿي))
        اذهب err_bad;

    إذا (وٿي == 1 && (بنانشان صحيح) بفر[0] <= 127u &&
        should_do_ctrl_transformation(state, kc))
        بفر[0] = Xkb_منزل_Control(بفر[0]);

    أرجع وٿي;

err_trunc:
    إذا (سائيز > 0)
        بفر[سائيز - 1] = '\0';
    أرجع وٿي;

err_bad:
    إذا (سائيز > 0)
        بفر[0] = '\0';
    أرجع 0;
}

uint32_t
xkb_state_ڪنجي_get_utf32(بنية xkb_state *state, xkb_keycode_t kc)
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
ساكن مضمن xkb_mod_نقاب_t
serialize_mods(ثابت بنية state_components *components,
               تعداد xkb_state_component type)
{
    xkb_mod_نقاب_t ret = 0;

    إذا (type & XKB_STATE_MODS_EFFECTIVE)
        أرجع components->mods;

    إذا (type & XKB_STATE_MODS_DEPRESSED)
        ret |= components->بنياد_mods;
    إذا (type & XKB_STATE_MODS_LATCHED)
        ret |= components->latched_mods;
    إذا (type & XKB_STATE_MODS_LOCKED)
        ret |= components->locked_mods;

    أرجع ret;
}

xkb_mod_نقاب_t
xkb_state_serialize_mods(بنية xkb_state *state,
                         تعداد xkb_state_component type)
{
    أرجع serialize_mods(&state->components, type);
}

/**
 * Serialises the requested group state, with all the same disclaimers as
 * in xkb_state_update_mask.
 */
ساكن مضمن xkb_layout_اشاريو_t
serialize_layout(ثابت بنية state_components *components,
                 تعداد xkb_state_component type)
{
    إذا (type & XKB_STATE_LAYOUT_EFFECTIVE)
        أرجع components->گروهه;

    int32_t ret = 0;
    إذا (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret += components->بنياد_گروهه;
    إذا (type & XKB_STATE_LAYOUT_LATCHED)
        ret += components->latched_گروهه;
    إذا (type & XKB_STATE_LAYOUT_LOCKED)
        ret += components->locked_گروهه;

    أرجع (xkb_layout_اشاريو_t)ret;
}

xkb_layout_اشاريو_t
xkb_state_serialize_layout(بنية xkb_state *state,
                           تعداد xkb_state_component type)
{
    أرجع serialize_layout(&state->components, type);
}

xkb_led_نقاب_t
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
xkb_mod_نقاب_t
mod_نقاب_get_effective(بنية xkb_keymap *keymap, xkb_mod_نقاب_t mods)
{
    /* Initialize the effective mask with its corresponding real mods. */
    xkb_mod_نقاب_t نقاب = mods & MOD_REAL_نقاب_ALL;

    /* Resolve the virtual modifiers */
    ثابت بنية xkb_mod *mod;
    xkb_mod_اشاريو_t i;
    xkb_vmods_enumerate(i, mod, &keymap->mods)
        إذا (mods & (UINT32_C(1) << i))
            نقاب |= mod->mapping;

    أرجع نقاب;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
صحيح
xkb_state_mod_اشاريو_is_سرگرم(بنية xkb_state *state,
                              xkb_mod_اشاريو_t idx,
                              تعداد xkb_state_component type)
{
    إذا (unlikely(idx >= xkb_keymap_انگ_mods(state->keymap)))
        أرجع -1;

    ثابت xkb_mod_نقاب_t mapping = state->keymap->mods.mods[idx].mapping;
    إذا (!mapping) {
        /* Modifier not mapped */
        أرجع 0;
    }
    /* WARNING: this may overmatch for virtual modifiers */
    أرجع (xkb_state_serialize_mods(state, type) & mapping) == mapping;
}

تعداد {
    XKB_STATE_ميلاپ_FLAGS = XKB_STATE_ميلاپ_ANY
                          | XKB_STATE_ميلاپ_ALL
                          | XKB_STATE_ميلاپ_NON_EXCLUSIVE
};

/**
 * Helper function for xkb_state_mod_indices_are_active and
 * xkb_state_mod_names_are_active.
 */
ساكن bool
ميلاپ_mod_masks(بنية xkb_state *state,
                تعداد xkb_state_component type,
                تعداد xkb_state_ميلاپ ميلاپ,
                xkb_mod_نقاب_t wanted)
{
    ثابت xkb_mod_نقاب_t سرگرم = xkb_state_serialize_mods(state, type);

    إذا (!(ميلاپ & XKB_STATE_ميلاپ_NON_EXCLUSIVE) && (سرگرم & ~wanted))
        أرجع false;

    إذا (ميلاپ & XKB_STATE_ميلاپ_ANY)
        أرجع سرگرم & wanted;

    /* Ensure all flags but XKB_STATE_MATCH_ALL are processed before */
    static_assert(XKB_STATE_ميلاپ_ALL ==
                  ((تعداد xkb_state_ميلاپ) XKB_STATE_ميلاپ_FLAGS &
                   ~(XKB_STATE_ميلاپ_ANY | XKB_STATE_ميلاپ_NON_EXCLUSIVE)),
                  "");

    أرجع (سرگرم & wanted) == wanted;
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, or -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
صحيح
xkb_state_mod_indices_are_سرگرم(بنية xkb_state *state,
                                 تعداد xkb_state_component type,
                                 تعداد xkb_state_ميلاپ ميلاپ,
                                 ...)
{
    إذا (ميلاپ & ~((تعداد xkb_state_ميلاپ) XKB_STATE_ميلاپ_FLAGS))
        أرجع -2;

    va_list ap;
    xkb_mod_نقاب_t wanted = 0;
    صحيح ret = 0;
    ثابت xkb_mod_اشاريو_t انگ_mods = xkb_keymap_انگ_mods(state->keymap);

    va_شروعات(ap, ميلاپ);
    طالما (1) {
        xkb_mod_اشاريو_t idx = va_arg(ap, xkb_mod_اشاريو_t);
        إذا (idx == XKB_MOD_INVALID)
            اكسر;
        إذا (unlikely(idx >= انگ_mods)) {
            ret = -1;
            اكسر;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_پڇاڙي(ap);

    إذا (ret == -1)
        أرجع ret;

    إذا (!wanted) {
        /* Modifiers not mapped */
        أرجع 0;
    }

    أرجع ميلاپ_mod_masks(state, type, ميلاپ, wanted);
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
صحيح
xkb_state_mod_name_is_سرگرم(بنية xkb_state *state, ثابت حرف *name,
                             تعداد xkb_state_component type)
{
    ثابت xkb_mod_اشاريو_t idx = xkb_keymap_mod_get_اشاريو(state->keymap, name);

    إذا (idx == XKB_MOD_INVALID)
        أرجع -1;

    أرجع xkb_state_mod_اشاريو_is_سرگرم(state, idx, type);
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ATTR_NULL_SENTINEL صحيح
xkb_state_mod_نالا_are_سرگرم(بنية xkb_state *state,
                               تعداد xkb_state_component type,
                               تعداد xkb_state_ميلاپ ميلاپ,
                               ...)
{
    إذا (ميلاپ & ~((تعداد xkb_state_ميلاپ) XKB_STATE_ميلاپ_FLAGS))
        أرجع -2;

    va_list ap;
    xkb_mod_نقاب_t wanted = 0;
    صحيح ret = 0;

    va_شروعات(ap, ميلاپ);
    طالما (1) {
        ثابت حرف *str = va_arg(ap, ثابت حرف *);
        إذا (str == عدم)
            اكسر;
        ثابت xkb_mod_اشاريو_t idx = xkb_keymap_mod_get_اشاريو(state->keymap, str);
        إذا (idx == XKB_MOD_INVALID) {
            ret = -1;
            اكسر;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_پڇاڙي(ap);

    إذا (ret == -1)
        أرجع ret;

    إذا (!wanted) {
        /* Modifiers not mapped */
        أرجع 0;
    }

    أرجع ميلاپ_mod_masks(state, type, ميلاپ, wanted);
}

/**
 * Returns 1 if the given group is active with the specified type(s), 0 if
 * not, or -1 if the group is invalid.
 */
صحيح
xkb_state_layout_اشاريو_is_سرگرم(بنية xkb_state *state,
                                 xkb_layout_اشاريو_t idx,
                                 تعداد xkb_state_component type)
{
    إذا (idx >= state->keymap->انگ_groups)
        أرجع -1;

    صحيح ret = 0;
    إذا (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ret |= (state->components.گروهه == idx);
    إذا (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret |= (state->components.بنياد_گروهه == (int32_t) idx);
    إذا (type & XKB_STATE_LAYOUT_LATCHED)
        ret |= (state->components.latched_گروهه == (int32_t) idx);
    إذا (type & XKB_STATE_LAYOUT_LOCKED)
        ret |= (state->components.locked_گروهه == (int32_t) idx);

    أرجع ret;
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
صحيح
xkb_state_layout_name_is_سرگرم(بنية xkb_state *state, ثابت حرف *name,
                                تعداد xkb_state_component type)
{
    ثابت xkb_layout_اشاريو_t idx =
        xkb_keymap_layout_get_اشاريو(state->keymap, name);

    إذا (idx == XKB_LAYOUT_INVALID)
        أرجع -1;

    أرجع xkb_state_layout_اشاريو_is_سرگرم(state, idx, type);
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
صحيح
xkb_state_led_اشاريو_is_سرگرم(بنية xkb_state *state, xkb_led_اشاريو_t idx)
{
    إذا (idx >= state->keymap->انگ_leds ||
        state->keymap->leds[idx].name == XKB_ATOM_NONE)
        أرجع -1;

    أرجع !!(state->components.leds & (UINT32_C(1) << idx));
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
صحيح
xkb_state_led_name_is_سرگرم(بنية xkb_state *state, ثابت حرف *name)
{
    ثابت xkb_led_اشاريو_t idx = xkb_keymap_led_get_اشاريو(state->keymap, name);

    إذا (idx == XKB_LED_INVALID)
        أرجع -1;

    أرجع xkb_state_led_اشاريو_is_سرگرم(state, idx);
}

/**
 * See:
 * - XkbTranslateKeyCode(3), mod_rtrn return value, from libX11.
 * - MyEnhancedXkbTranslateKeyCode(), a modification of the above, from GTK+.
 */
ساكن xkb_mod_نقاب_t
ڪنجي_get_consumed(بنية xkb_state *state, ثابت بنية xkb_ڪنجي *ڪنجي,
                 تعداد xkb_consumed_گهڻائي گهڻائي)
{
    ثابت xkb_layout_اشاريو_t گروهه =
        xkb_state_ڪنجي_get_layout(state, ڪنجي->keycode);
    إذا (گروهه == XKB_LAYOUT_INVALID)
        أرجع 0;

    xkb_mod_نقاب_t preserve = 0;
    xkb_mod_نقاب_t consumed = 0;

    ثابت بنية xkb_ڪنجي_type_داخلا* ثابت matching_داخلا =
        get_داخلا_for_ڪنجي_state(state, ڪنجي, گروهه);
    إذا (matching_داخلا)
        preserve = matching_داخلا->preserve.نقاب;

    ثابت بنية xkb_ڪنجي_type* ثابت type = ڪنجي->groups[گروهه].type;
    بدل (گهڻائي) {
    حالة XKB_CONSUMED_گهڻائي_XKB:
        consumed = type->mods.نقاب;
        اكسر;

    حالة XKB_CONSUMED_گهڻائي_GTK: {
        ثابت بنية xkb_ڪنجي_type_داخلا* ثابت no_mods_داخلا =
            get_داخلا_for_mods(type, 0);
        ثابت xkb_level_اشاريو_t no_mods_leveli = no_mods_داخلا
                                               ? no_mods_داخلا->level
                                               : 0;
        ثابت بنية xkb_level* ثابت no_mods_level =
            &ڪنجي->groups[گروهه].سطحون[no_mods_leveli];

        لكل (darray_سائيز_t i = 0; i < type->انگ_entries; i++) {
            ثابت بنية xkb_ڪنجي_type_داخلا* ثابت داخلا = &type->entries[i];
            إذا (!داخلا_is_سرگرم(داخلا))
                استمر;

            ثابت بنية xkb_level* ثابت level =
                &ڪنجي->groups[گروهه].سطحون[داخلا->level];
            إذا (Xkb_سطحون_Same_Syms(level, no_mods_level))
                استمر;

            إذا (داخلا == matching_داخلا || one_ٻٽو_انگ_سيٽ(داخلا->mods.نقاب))
                consumed |= داخلا->mods.نقاب & ~داخلا->preserve.نقاب;
        }
        اكسر;
    }
    }

    أرجع consumed & ~preserve;
}

صحيح
xkb_state_mod_اشاريو_is_consumed2(بنية xkb_state *state, xkb_keycode_t kc,
                                 xkb_mod_اشاريو_t idx,
                                 تعداد xkb_consumed_گهڻائي گهڻائي)
{
    ثابت بنية xkb_ڪنجي* ثابت ڪنجي = Xkb_ڪنجي(state->keymap, kc);

    إذا (unlikely(!ڪنجي || idx >= xkb_keymap_انگ_mods(state->keymap)))
        أرجع -1;

    ثابت xkb_mod_نقاب_t mapping = state->keymap->mods.mods[idx].mapping;
    إذا (!mapping) {
        /* Modifier not mapped */
        أرجع 0;
    }
    أرجع (mapping & ڪنجي_get_consumed(state, ڪنجي, گهڻائي)) == mapping;
}

صحيح
xkb_state_mod_اشاريو_is_consumed(بنية xkb_state *state, xkb_keycode_t kc,
                                xkb_mod_اشاريو_t idx)
{
    أرجع xkb_state_mod_اشاريو_is_consumed2(state, kc, idx,
                                            XKB_CONSUMED_گهڻائي_XKB);
}

xkb_mod_نقاب_t
xkb_state_mod_نقاب_remove_consumed(بنية xkb_state *state, xkb_keycode_t kc,
                                   xkb_mod_نقاب_t نقاب)
{
    ثابت بنية xkb_ڪنجي* ثابت ڪنجي = Xkb_ڪنجي(state->keymap, kc);

    إذا (!ڪنجي)
        أرجع 0;

    أرجع resolve_منزل_canonical_mods(state->keymap, نقاب) &
           ~ڪنجي_get_consumed(state, ڪنجي, XKB_CONSUMED_گهڻائي_XKB);
}

xkb_mod_نقاب_t
xkb_state_ڪنجي_get_consumed_mods2(بنية xkb_state *state, xkb_keycode_t kc,
                                 تعداد xkb_consumed_گهڻائي گهڻائي)
{
    بدل (گهڻائي) {
    حالة XKB_CONSUMED_گهڻائي_XKB:
    حالة XKB_CONSUMED_گهڻائي_GTK:
        اكسر;
    افتراضي:
        log_err_func(state->keymap->ctx, XKB_LOG_پيغام_NO_ID,
                     "unrecognized consumed modifiers mode: %d\n", گهڻائي);
        أرجع 0;
    }

    ثابت بنية xkb_ڪنجي* ثابت ڪنجي = Xkb_ڪنجي(state->keymap, kc);
    إذا (!ڪنجي)
        أرجع 0;

    أرجع ڪنجي_get_consumed(state, ڪنجي, گهڻائي);
}

xkb_mod_نقاب_t
xkb_state_ڪنجي_get_consumed_mods(بنية xkb_state *state, xkb_keycode_t kc)
{
    أرجع xkb_state_ڪنجي_get_consumed_mods2(state, kc, XKB_CONSUMED_گهڻائي_XKB);
}

/*******************************************************************************
 * Server state machine API
 ******************************************************************************/

/** Entry to keep track of the state of an overlaid key */
بنية xkb_overlaid_ڪنجي {
    /** The source key being overlaid */
    ثابت بنية xkb_ڪنجي *old;
    /** The target key */
    ثابت بنية xkb_ڪنجي *new;
    /** Counter of key currently down */
    صحيح refcnt;
};

بنية xkb_shortcuts_config_داخلا {
    /** Real modifier mask to trigger shortcuts tweaks */
    xkb_mod_نقاب_t mods;
    /** Target layout */
    xkb_layout_اشاريو_t هدف;
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
    بنية xkb_server_state بنياد;

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
        darray(بنية xkb_overlaid_ڪنجي) ڪنجيون;
        /**
         * Activation order of the overlay
         *
         * Overlays indices are stored 1-indexed in nibbles: the lowest
         * nibble corresponds to the latest activated index.
         */
        uint32_t ترتيب;
        /** Current enabled overlays mask */
        xkb_overlay_نقاب_t enabled;
    } overlays;

    /** Configuration */
    بنية machine_config {
        /** Modifiers remapping */
        بنية machine_modifiers_config {
            /** Real modifier mask */
            xkb_mod_نقاب_t نقاب;
            /** Modifiers re-mappings */
            darray_سائيز_t mappings_انگ;
            بنية machine_mods_mapping {
                xkb_mod_نقاب_t ذريعو;
                xkb_mod_نقاب_t هدف;
            } * mappings ATTR_COUNTED_BY(mappings_انگ);
        } modifiers;

        /** Shortcuts tweak */
        بنية machine_shortcuts_config {
            /** Substitution entries */
            بنية xkb_shortcuts_config_داخلا *entries;
        } shortcuts;
    } config;

    /** Mouse keys handling */
    بنية {
        /** Default pointer button */
        xkb_اشارو_button_اشاريو_t default_button;
        /** Mask of the locked buttons */
        xkb_اشارو_button_نقاب_t locked_buttons;
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
        darray(بنية xkb_shortcuts_config_داخلا) entries;
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
        .سائيز = حجم(default_config)
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
        & ~(تعداد xkb_machine_builder_flags)XKB_MACHINE_BUILDER_FLAGS_قدرون;
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
        & ~(تعداد xkb_machine_flags)XKB_MACHINE_FLAGS_قدرون;
    إذا (invalid_machine_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS_,
                     "unrecognized state machine flags: 0x%x\n",
                     invalid_machine_flags);
        إذا (حالت)
            *حالت = XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS;
        أرجع عدم;
    }

    بنية xkb_machine_builder * ثابت builder = صفر_يادگيري_ڏيو(1, حجم(*builder));
    إذا (!builder) {
        log_err_func1(keymap->ctx, XKB_ERROR_ورهاست_FAILURE_2,
                      "cannot allocate machine builder\n");
        إذا (حالت)
            *حالت = XKB_ERROR_ورهاست_FAILURE;
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
        ~(تعداد xkb_a11y_flags)XKB_A11_Y_FLAGS_قدرون;

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
    إذا (!update->ذريعو) {
        إذا (!update->هدف) {
            /* Reset mappings */
            darray_resize(builder->mods, 0);
            أرجع XKB_SUCCESS;
        } وإلا {
            أرجع XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب;
        }
    }

    /* Check the modifiers against the keymap */
    ثابت xkb_mod_نقاب_t invalid = ~builder->keymap->canonical_state_نقاب;
    إذا ((update->ذريعو & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب,
                "%s: Invalid source modifiers: 0x%"PRIx32"\n",
                __func__, update->ذريعو);
        أرجع XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب;
    }
    إذا ((update->هدف & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب,
                "%s: Invalid target modifiers: 0x%"PRIx32"\n",
                __func__, update->هدف);
        أرجع XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب;
    }

    بنية machine_mods_mapping *mapping = عدم;
    darray_سائيز_t m = 0;
    darray_enumerate(m, mapping, builder->mods) {
        إذا (mapping->ذريعو == update->ذريعو) {
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
                .ذريعو = update->ذريعو,
                .هدف = update->هدف
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
    ثابت xkb_mod_نقاب_t invalid = ~keymap->canonical_state_نقاب;
    إذا ((update->affect_mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب,
                "%s: Invalid affected modifiers: 0x%"PRIx32"\n",
                __func__, update->affect_mods);
        أرجع XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب;
    }
    إذا ((update->mods & invalid)) {
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب,
                "%s: Invalid modifiers: 0x%"PRIx32"\n",
                __func__, update->mods);
        أرجع XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب;
    }

    /* Check the layout indices against the keymap */
    إذا (update->ذريعو >= keymap->انگ_groups &&
        update->ذريعو != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو_2,
                "%s: Invalid source layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->انگ_groups, update->ذريعو + 1);
        أرجع XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو;
    }
    إذا (update->هدف >= keymap->انگ_groups &&
        update->هدف != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو_2,
                "%s: Invalid target layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->انگ_groups, update->هدف + 1);
        أرجع XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو;
    }

    بنية xkb_shortcuts_config_options * ثابت config = &builder->shortcuts;

    /* Resize array & initialize new entries, if relevant */
    إذا (update->ذريعو >= darray_سائيز(config->entries) &&
        update->ذريعو != XKB_LAYOUT_INVALID) {
        إذا (update->هدف == update->ذريعو) {
            /* Skip default setting */
            أرجع XKB_SUCCESS;
        }
        xkb_layout_اشاريو_t new = darray_سائيز(config->entries);
        darray_resize(config->entries, update->ذريعو + 1);
        لكل (; new <= update->ذريعو; new++) {
            darray_شي(config->entries, new) =
                (بنية xkb_shortcuts_config_داخلا) {
                    .هدف = XKB_LAYOUT_INVALID,
                    .mods = 0
                };
        }
    }

    بنية xkb_shortcuts_config_داخلا *داخلا;
    xkb_layout_اشاريو_t ذريعو = (update->ذريعو == XKB_LAYOUT_INVALID)
        ? 0               /* Loop over all entries */
        : update->ذريعو; /* Single entry */
    darray_foreach_from(داخلا, config->entries, ذريعو) {
        إذا (update->ذريعو == XKB_LAYOUT_INVALID &&
            داخلا->هدف == XKB_LAYOUT_INVALID) {
            /* Skip previous deactivated entry */
            ذريعو++;
            استمر;
        }

        إذا (update->affect_mods) {
            داخلا->mods &= ~update->affect_mods;
            داخلا->mods |= (update->affect_mods & update->mods);
        }

        إذا (update->ذريعو != XKB_LAYOUT_INVALID) {
            داخلا->هدف = (update->هدف == ذريعو)
                ? XKB_LAYOUT_INVALID
                : update->هدف;
            /* Single entry: exit loop */
            اكسر;
        }
        ذريعو++;
    }

    أرجع XKB_SUCCESS;
}

/** Compare 2 modifiers combinations */
ساكن صحيح
cmp_mod_masks(ثابت فراغ *a, ثابت فراغ *b)
{
    ثابت xkb_mod_نقاب_t m1 = *((xkb_mod_نقاب_t *) a);
    ثابت xkb_mod_نقاب_t m2 = *((xkb_mod_نقاب_t *) b);
    إذا (m1 == m2)
        أرجع 0;

    إذا (!m1)
        أرجع +1;
    إذا (!m2)
        أرجع -1;

    ثابت xkb_mod_نقاب_t overlap = (m1 & m2);
    إذا (overlap == m1)
        أرجع +1; /* m1 is a subset of m2 */
    إذا (overlap == m2)
        أرجع -1; /* m2 is a subset of m1 */

    /* Compare active modifiers numbers */
    ثابت بنانشان صحيح count1 = popcount32(m1);
    ثابت بنانشان صحيح count2 = popcount32(m2);
    إذا (count1 > count2)
        أرجع -1;
    إذا (count1 < count2)
        أرجع +1;

    /* Give priority to real modifiers */
    أرجع (m1 < m2) ? -1 : +1;
}

ساكن تعداد xkb_حالت
machine_سيٽ_mods(بنية xkb_machine *sm,
                 ثابت machine_mods_mappings *raw_mappings)
{
    إذا (!darray_خالي_جايون(*raw_mappings)) {
        machine_mods_mappings mappings = darray_new();
        xkb_mod_نقاب_t نقاب = 0;
        ثابت بنية machine_mods_mapping *mapping;
        ثابت xkb_mod_نقاب_t invalid =
            ~sm->بنياد.بنياد.keymap->canonical_state_نقاب;
        darray_foreach(mapping, *raw_mappings) {
            إذا (!mapping->ذريعو || !mapping->هدف ||
                (mapping->ذريعو & invalid) || (mapping->هدف & invalid))
                استمر;
            darray_append(mappings, *mapping);
            نقاب |= mapping->ذريعو;
        }

        /*
         * Re-order the mappings, so that combinations have higher priority
         * if they are superset of other combinations.
         * For other type of overlaps, see the sorting function.
         */
        qsort(darray_شيون(mappings), darray_سائيز(mappings),
              حجم(*darray_شيون(mappings)), &cmp_mod_masks);

        darray_steal(
            mappings,
            &sm->config.modifiers.mappings,
            &sm->config.modifiers.mappings_انگ
        );
        sm->config.modifiers.نقاب = نقاب;
    } وإلا {
        sm->config.modifiers = (بنية machine_modifiers_config) {
            .mappings = عدم,
            .mappings_انگ = 0,
            .نقاب = 0
        };
    }

    أرجع XKB_SUCCESS;
}

ساكن تعداد xkb_حالت
machine_سيٽ_shortcuts(
    بنية xkb_machine * محدود sm,
    ثابت بنية xkb_shortcuts_config_options * محدود options
)
{
    إذا (darray_خالي_جايون(options->entries)) {
        sm->config.shortcuts = (بنية machine_shortcuts_config) {
            .entries = عدم
        };
        أرجع XKB_SUCCESS;
    }

    بنية xkb_keymap * ثابت keymap = sm->بنياد.بنياد.keymap;

    بنية xkb_shortcuts_config_داخلا * entries =
        صفر_يادگيري_ڏيو(keymap->انگ_groups, حجم(*entries));
    إذا (!entries)
        أرجع XKB_ERROR_ورهاست_FAILURE;

    ثابت xkb_layout_اشاريو_t ڳڻپ = MIN(
        keymap->انگ_groups,
        (xkb_layout_اشاريو_t) darray_سائيز(options->entries)
    );

    bool some_entries = false;
    لكل (xkb_layout_اشاريو_t l = 0; l < ڳڻپ; l++) {
        /* Sanitize layout target */
        ثابت xkb_layout_اشاريو_t هدف =
            darray_شي(options->entries, l).هدف;
        /* Sanitize modifier mask */
        ثابت xkb_mod_نقاب_t mods = (
            darray_شي(options->entries, l).mods &
            sm->بنياد.بنياد.keymap->canonical_state_نقاب
        );
        إذا (هدف < keymap->انگ_groups && هدف != l && mods) {
            entries[l] = (بنية xkb_shortcuts_config_داخلا) {
                .هدف = هدف,
                .mods = mods
            };
            some_entries = true;
        } وإلا {
            entries[l] = (بنية xkb_shortcuts_config_داخلا) {
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
        لكل (xkb_layout_اشاريو_t l = ڳڻپ; l < keymap->انگ_groups; l++) {
            entries[l] = (بنية xkb_shortcuts_config_داخلا) {
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
                تعداد xkb_حالت * محدود حالت)
{
    بنية xkb_machine * ثابت machine = صفر_يادگيري_ڏيو(1, حجم(*machine));
    إذا (!machine) {
        log_err_func1(builder->keymap->ctx, XKB_ERROR_ورهاست_FAILURE_2,
                      "cannot allocate machine\n");
        إذا (حالت)
            *حالت = XKB_ERROR_ورهاست_FAILURE;
        أرجع عدم;
    }

    xkb_server_state_init(&machine->بنياد, builder->keymap, SERVER_STATE,
                          builder->controls.a11y.affect_flags,
                          builder->controls.a11y.flags);
    machine->flags = builder->machine_flags;
    إذا (!(machine->flags &
          XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_ڪنجي_FIX))
    {
        machine->بنياد.flags |= STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_ڪنجي;
    }
    إذا (machine->flags & XKB_MACHINE_UNLOCK_MODS_ON_PRESS) {
        machine->بنياد.flags |= STATE_UNLOCK_MODS_ON_PRESS;
    }

    تعداد xkb_حالت حالت_2;
    إذا ((حالت_2 = machine_سيٽ_mods(machine, &builder->mods)) != XKB_SUCCESS ||
        (حالت_2 = machine_سيٽ_shortcuts(machine, &builder->shortcuts)) !=
        XKB_SUCCESS) {
        *حالت = حالت_2;
        اذهب error;
    }

    darray_init(machine->overlays.ڪنجيون);
    machine->mouse.default_button = XKB_اشارو_BUTTON_MIN;

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
    أكد(sm->بنياد.بنياد.refcnt > 0);
    sm->بنياد.بنياد.refcnt++;
    أرجع sm;
}

فراغ
xkb_machine_unref(بنية xkb_machine *sm)
{
    أكد(!sm || sm->بنياد.بنياد.refcnt > 0);
    إذا (!sm || --sm->بنياد.بنياد.refcnt > 0)
        أرجع;

    xkb_state_destroy(&sm->بنياد.بنياد);
    darray_free(sm->overlays.ڪنجيون);
    حرر(sm->config.shortcuts.entries);
    حرر(sm->config.modifiers.mappings);
    حرر(sm);
}

بنية xkb_keymap *
xkb_machine_get_keymap(ثابت بنية xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    أرجع sm->بنياد.بنياد.keymap;
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
    بنية xkb_state * ثابت state = xkb_state_new_with_گهڻائي(
        machine->بنياد.بنياد.keymap,
        XKB_STATE_گهڻائي_SERVER_QUERY,
        &حالت_2
    );
    إذا (حالت_2 != XKB_SUCCESS) {
        إذا (حالت)
            *حالت = حالت_2;
        أرجع عدم;
    }

    أكد(state->گهڻائي == SERVER_COMPANION);
    أكد(state->refcnt == 1);

    /* Copy machine’s state */
    *state = machine->بنياد.بنياد;

    /* Restore previous values */
    state->گهڻائي = SERVER_COMPANION;
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

    ثابت xkb_overlay_نقاب_t نقاب =
        overlays_from_controls(sm->بنياد.بنياد.components.controls);
    xkb_overlay_نقاب_t added = نقاب & ~sm->overlays.enabled;

    /* Remove overlays no longer enabled and keep relative order */
    uint32_t ترتيب = sm->overlays.ترتيب;
    ثابت xkb_overlay_اشاريو_t overlay_max =
        format_max_overlays(sm->بنياد.بنياد.keymap->format);
    لكل (uint8_t n = 0; n < overlay_max;) {
        xkb_overlay_اشاريو_t overlay_idx = (ترتيب >> (n * 4)) & 0xf;
        إذا (!overlay_idx)
            اكسر;
        ثابت xkb_overlay_نقاب_t overlay_نقاب =
            (xkb_overlay_نقاب_t)(1u << (overlay_idx - 1u) /* k is 1-indexed */);
        إذا (overlay_نقاب & نقاب) {
            /* no duplicates */
            added &= ~overlay_نقاب;
            n++;
        } وإلا {
            /* remove overlay */
            ثابت uint32_t سر = (UINT32_C(1) << (n * 4)) - 1;
            ترتيب = (ترتيب & سر) | ((ترتيب >> 4) & ~سر);
        }
    }

    /* Add new overlays in ascending order */
    لكل (xkb_overlay_اشاريو_t k = 0; added; k++, added >>= 1) {
        إذا (added & 0x1) {
            ترتيب <<= 4;
            ترتيب |= k + 1u; /* k is 0-indexed */
        }
    }

    sm->overlays.ترتيب = ترتيب;
    sm->overlays.enabled = نقاب;
}

تعداد xkb_حالت
xkb_machine_process_synthetic(بنية xkb_machine *sm,
                              ثابت بنية xkb_synthetic_update *update,
                              بنية xkb_events *events)
{
    /* Check ABI compatibility */
    تعداد xkb_حالت حالت =
        check_state_update_abi(sm->بنياد.بنياد.keymap->ctx, update);
    إذا (حالت != XKB_SUCCESS)
        أرجع حالت;

    بنية xkb_server_state * ثابت state = &sm->بنياد;
    ثابت بنية state_components اڳيون_components = state->بنياد.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    darray_سائيز(events->قطار) = 0;
    events->ايندڙ = 0;

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

    xkb_state_update_derived(&state->بنياد);

    ثابت تعداد xkb_state_component بدليو = get_state_component_changes(
        &اڳيون_components, &state->بنياد.components
    );
    إذا (بدليو) {
        // TODO: latch controls
        إذا (بدليو & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        /* Create event only if some component actually changed */
        darray_append(events->قطار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .بدليو = بدليو,
                .components = state->بنياد.components
            }
        });

        /* Terminate the frame */
        darray_append(events->قطار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_چوکٺو,
        });
    }

    أرجع XKB_SUCCESS;
}

ساكن ssize_t
do_remap_modifiers(ثابت بنية machine_modifiers_config * محدود mappings,
                   بنية xkb_state * محدود state,
                   بنية xkb_events * محدود events,
                   ثابت بنية xkb_ڪنجي * محدود ڪنجي)
{
    إذا (!(mappings->نقاب & state->components.mods))
        أرجع -1;

    ثابت xkb_layout_اشاريو_t layout = state_ڪنجي_get_layout(state, ڪنجي);
    إذا (layout >= ڪنجي->انگ_groups)
        أرجع -1;

    ثابت بنية xkb_ڪنجي_type * ثابت type = ڪنجي->groups[layout].type;
    xkb_mod_نقاب_t affect_mods = 0;
    xkb_mod_نقاب_t mods = 0;
    لكل (darray_سائيز_t m = 0; m < mappings->mappings_انگ; m++) {
        ثابت بنية machine_mods_mapping * ثابت mapping =
            &mappings->mappings[m];
        إذا (/* Skip not matching active mods */
            (mapping->ذريعو & state->components.mods) == mapping->ذريعو &&
            /* Skip overlap with previous active mappings */
            !(mapping->ذريعو & affect_mods) &&
            /* Skip if the key type uses some of the source modifiers */
            !(mapping->ذريعو & type->mods.نقاب)) {
            affect_mods |= mappings->mappings[m].ذريعو;
            mods |= mappings->mappings[m].هدف;
        }
    }

    إذا (!affect_mods)
        أرجع -1;

    بنية xkb_state new = *state;
    new.components.بنياد_mods = (new.components.بنياد_mods & ~affect_mods) | mods;
    new.components.latched_mods = (new.components.latched_mods & ~affect_mods);
    new.components.locked_mods = (new.components.locked_mods & ~affect_mods);
    xkb_state_update_derived(&new);

    ssize_t event_idx = -1;
    ثابت تعداد xkb_state_component بدليو =
        get_state_component_changes(&state->components, &new.components);
    إذا (بدليو) {
        event_idx = (ssize_t) darray_سائيز(events->قطار);
        darray_append(events->قطار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = new.components,
                .بدليو = بدليو
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
                   ثابت بنية state_components *اڳيون_components,
                   بنية xkb_events *events, ssize_t remap_event)
{
    إذا (!config->entries) {
        /* No shortcuts tweak */
        أرجع remap_event;
    }

    ثابت بنية xkb_shortcuts_config_داخلا * ثابت داخلا =
        &config->entries[state->components.گروهه];

    إذا (داخلا->هدف != XKB_LAYOUT_INVALID &&
        (داخلا->mods & state->components.mods)) {
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
            remap_event = (ssize_t) darray_سائيز(events->قطار);
            darray_append(events->قطار, (بنية xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = {0},
                    .بدليو = 0
                }
            });
        } وإلا {
            /* Merge with last remap event */
            new.components =
                darray_شي(events->قطار, remap_event).components.components;
        }
        new.components.بنياد_گروهه
            = (int32_t) داخلا->هدف
            - state->components.latched_گروهه
            - state->components.locked_گروهه;
        xkb_state_update_derived(&new);

        ثابت تعداد xkb_state_component بدليو =
            get_state_component_changes(اڳيون_components, &new.components);
        darray_شي(events->قطار, remap_event).components.بدليو = بدليو;
        darray_شي(events->قطار, remap_event).components.components =
            new.components;

        state->components.گروهه = new.components.گروهه;
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
    darray_foreach_ابتو(event, events->قطار) {
        إذا (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS)
            اكسر;
    }
    إذا (!event)
        أرجع;

    /* Restore state */
    ثابت تعداد xkb_state_component بدليو =
        get_state_component_changes(initial_components,
                                    &event->components.components);
    إذا (بدليو) {
        darray_append(events->قطار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = *initial_components,
                .بدليو = بدليو
            }
        });
    }
}

ساكن ثابت بنية xkb_ڪنجي *
process_overlayable_ڪنجي(بنية xkb_machine *sm,
                        ثابت بنية xkb_ڪنجي * ڪنجي,
                        تعداد xkb_ڪنجي_direction direction)
{
    /* Check if key is currently overlaid */
    بنية xkb_overlaid_ڪنجي *داخلا = عدم;
    بنية xkb_overlaid_ڪنجي *موجود_داخلا = عدم;
    darray_foreach(داخلا, sm->overlays.ڪنجيون) {
        إذا (داخلا->old == ڪنجي) {
            /* Maintain the key overlaid */
            بدل (direction) {
            حالة XKB_ڪنجي_هيٺ_ڏانهن:
                داخلا->refcnt++;
                اكسر;
            حالة XKB_ڪنجي_REPEATED:
                اكسر;
            حالة XKB_ڪنجي_مٿي:
            افتراضي:
                داخلا->refcnt--;
                اكسر;
            }
            إذا (داخلا->refcnt <= 0)
                داخلا->old = عدم;
            أرجع داخلا->new;
        } وإلا إذا (!داخلا->old && !موجود_داخلا) {
            /* First free spot */
            موجود_داخلا = داخلا;
        }
    }
    إذا (direction == XKB_ڪنجي_هيٺ_ڏانهن) {
        /* Enable key overlay */

        /*
         * Get overlay index: select the latest activated overlay
         * containing the key.
         */
        ثابت بنية xkb_ڪنجي *new = ڪنجي;

        إذا (ڪنجي->overlays & sm->overlays.enabled) {
            /* Some relevant overlay is active */
            لكل (uint32_t ڍير = sm->overlays.ترتيب; ڍير; ڍير >>= 4) {
                static_assert(XKB_OVERLAY_ڳڻپ == 8, "");
                ثابت xkb_overlay_اشاريو_t overlay = (ڍير & 0xf) - 1;
                ثابت xkb_overlay_نقاب_t نقاب =
                    (xkb_overlay_نقاب_t)(1u << overlay);
                إذا (ڪنجي->overlays & نقاب) {
                    /* Get new key */
                    إذا (ڪنجي->overlays_inline) {
                        /* Stored inline */
                        new = ڪنجي->overlay_ڪنجي;
                    } وإلا {
                        /* Stored in sparse array */
                        ثابت xkb_overlay_نقاب_t هيٺيون = (xkb_overlay_نقاب_t)(
                            ڪنجي->overlays & (xkb_overlay_نقاب_t)(نقاب - 1u)
                        );
                        /* index = number of set bits strictly below bit */
                        ثابت xkb_overlay_اشاريو_t اشاريو =
                            (xkb_overlay_اشاريو_t)popcount32(هيٺيون);
                        new = ڪنجي->overlays_ڪنجيون[اشاريو];
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
        إذا (موجود_داخلا) {
            /* Re-use a free entry */
            داخلا = موجود_داخلا;
        } وإلا {
            /* Append a new entry */
            ثابت darray_سائيز_t idx = darray_سائيز(sm->overlays.ڪنجيون);
            darray_resize(sm->overlays.ڪنجيون, idx + 1);
            داخلا = &darray_شي(sm->overlays.ڪنجيون, idx);
        }

        /* Overlay the key */
        داخلا->old = ڪنجي;
        داخلا->new = new;
        داخلا->refcnt = 1;
        أرجع new;
    }

    /* No overlay */
    أرجع ڪنجي;
}

تعداد xkb_حالت
xkb_machine_process_ڪنجي(بنية xkb_machine *sm,
                        xkb_keycode_t kc, تعداد xkb_ڪنجي_direction direction,
                        بنية xkb_events *events)
{
    darray_سائيز(events->قطار) = 0;
    events->ايندڙ = 0;

    بنية xkb_server_state * ثابت state = &sm->بنياد;
    ثابت بنية xkb_ڪنجي * ڪنجي = Xkb_ڪنجي(state->بنياد.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    إذا (!ڪنجي || (direction == XKB_ڪنجي_REPEATED && !ڪنجي->repeats))
        أرجع XKB_SUCCESS;

    ثابت بنية state_components initial_components = state->بنياد.components;

    إذا (direction == XKB_ڪنجي_هيٺ_ڏانهن &&
        (sm->بنياد.بنياد.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ڪنجيون) &&
        (sm->بنياد.a11y_flags & XKB_A11_Y_STICKY_ڪنجيون_NO_گڏو_گڏ_ڪنجيون) &&
        state->بنياد.components.بنياد_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_ڪنجيون,
                                      0, events);
    }

    إذا (ڪنجي->overlays)
        ڪنجي = process_overlayable_ڪنجي(sm, ڪنجي, direction);

    ssize_t remap_event = do_remap_modifiers(&sm->config.modifiers,
                                             &state->بنياد, events, ڪنجي);

    remap_event = do_shortcuts_tweak(&sm->config.shortcuts, &state->بنياد,
                                     &initial_components, events, remap_event);

    إذا (remap_event >= 0) {
        // TODO: what about merging state event for RedirectKey?
        darray_append(events->قطار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_چوکٺو,
        });
    }

    state->flags |= STATE_REQUIRE_ڪنجي_EVENT;
    state->سيٽ_mods = 0;
    state->clear_mods = 0;

    /* Handle key actions */
    xkb_filter_apply_all(state, events, ڪنجي, direction);

    xkb_mod_اشاريو_t i;
    xkb_mod_نقاب_t ٻٽو_انگ;
    لكل (i = 0, ٻٽو_انگ = 1; state->سيٽ_mods; i++, ٻٽو_انگ <<= 1) {
        إذا (state->سيٽ_mods & ٻٽو_انگ) {
            state->mod_ڪنجي_ڳڻپ[i]++;
            state->بنياد.components.بنياد_mods |= ٻٽو_انگ;
            state->سيٽ_mods &= ~ٻٽو_انگ;
        }
    }

    لكل (i = 0, ٻٽو_انگ = 1; state->clear_mods; i++, ٻٽو_انگ <<= 1) {
        إذا (state->clear_mods & ٻٽو_انگ) {
            state->mod_ڪنجي_ڳڻپ[i]--;
            إذا (state->mod_ڪنجي_ڳڻپ[i] <= 0) {
                state->بنياد.components.بنياد_mods &= ~ٻٽو_انگ;
                state->mod_ڪنجي_ڳڻپ[i] = 0;
            }
            state->clear_mods &= ~ٻٽو_انگ;
        }
    }

    xkb_state_update_derived(&state->بنياد);

    /*
     * Insert key event only if actions did not override it, e.g.:
     * - RedirectKey() append its own key event;
     * - MovePointer() and PointerButton() generate pointer events instead
     *   of key events;
     * - TerminateServer() generate a Server event instead of key events.
     */
    إذا (state->flags & STATE_REQUIRE_ڪنجي_EVENT) {
        darray_append(events->قطار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_ڪنجي,
            .ڪنجي = {
                .keycode = ڪنجي->keycode,
                .direction = direction
            }
        });
    }

    إذا (remap_event >= 0) {
        // FIXME: fragile if last state change does not restore the state to the remap event
        undo_tweaks(&state->بنياد, &initial_components, events);
    }

    ثابت تعداد xkb_state_component بدليو = get_state_component_changes(
        &initial_components, &state->بنياد.components
    );
    إذا (بدليو) {
        إذا (بدليو & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        // TODO: merge with previous state event, if any

        darray_append(events->قطار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = state->بنياد.components,
                .بدليو = بدليو
            }
        });
    }

    إذا (!darray_خالي_جايون(events->قطار) &&
        darray_شي(events->قطار, darray_سائيز(events->قطار) - 1).type !=
        XKB_EVENT_TYPE_چوکٺو)
    {
        darray_append(events->قطار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_چوکٺو,
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
        .سائيز = حجم(default_config)
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
        (config->flags & ~(uint32_t)XKB_EVENTS_FLAGS_قدرون);
    إذا (invalid_flags) {
        log_err_func(context, XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS_,
                     "unrecognized events collection flags: 0x%"PRIx32"\n",
                     invalid_flags);
        إذا (حالت)
            *حالت = XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS;
        أرجع عدم;
    }

    بنية xkb_events *events = صفر_يادگيري_ڏيو(1, حجم(*events));
    إذا (!events) {
        log_err_func1(context, XKB_ERROR_ورهاست_FAILURE_2,
                      "cannot allocate state events collection\n");
        إذا (حالت)
            *حالت = XKB_ERROR_ورهاست_FAILURE;
        أرجع عدم;
    }

    /* Pre-allocate event queue */
    darray_init(events->قطار);
    ساكن ثابت darray_سائيز_t قطار_initial_سائيز = 16;
    darray_resize(events->قطار, قطار_initial_سائيز);
    // TODO: proper safe darray_resize() with return value
    إذا (!darray_شيون(events->قطار)) {
        log_err_func1(context, XKB_ERROR_ورهاست_FAILURE_2,
                      "cannot allocate state events queue\n");
        xkb_events_destroy(events);
        إذا (حالت)
            *حالت = XKB_ERROR_ورهاست_FAILURE;
        أرجع عدم;
    }
    darray_سائيز(events->قطار) = 0;

    events->ctx = xkb_context_ref(context);
    events->ايندڙ = 0;

    إذا (حالت)
        *حالت = XKB_SUCCESS;
    أرجع events;
}

فراغ
xkb_events_destroy(بنية xkb_events *events)
{
    إذا (!events)
        أرجع;
    darray_free(events->قطار);
    xkb_context_unref(events->ctx);
    حرر(events);
}

ثابت بنية xkb_event *
xkb_events_ايندڙ(بنية xkb_events *events)
{
    إذا (events->ايندڙ < darray_سائيز(events->قطار)) {
        ثابت darray_سائيز_t اشاريو = events->ايندڙ++;
        أرجع &darray_شي(events->قطار, اشاريو);
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
                      تعداد xkb_ڪنجي_direction *direction)
{
    إذا (event->type == XKB_EVENT_TYPE_ڪنجي) {
        *keycode = event->ڪنجي.keycode;
        *direction = event->ڪنجي.direction;
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

    components->بدليو = (uint32_t)event->components.بدليو;
    components->depressed_mods = event->components.components.بنياد_mods;
    components->latched_mods = event->components.components.latched_mods;
    components->locked_mods = event->components.components.locked_mods;
    components->mods = event->components.components.mods;
    components->depressed_layout = event->components.components.بنياد_گروهه;
    components->latched_layout = event->components.components.latched_گروهه;
    components->locked_layout = event->components.components.locked_گروهه;
    components->layout = event->components.components.گروهه;
    components->leds = event->components.components.leds;
    components->controls = (uint32_t)event->components.components.controls;

    أرجع XKB_SUCCESS;
}

تعداد xkb_حالت
xkb_event_get_اشارو_motion(ثابت بنية xkb_event * محدود event,
                             بنية xkb_event_اشارو_motion * محدود motion)
{
    إذا (event->type != XKB_EVENT_TYPE_اشارو_MOTION)
        أرجع XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ثابت تعداد xkb_حالت حالت = xkb_check_state_abi(motion);
    إذا (حالت != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, حالت);
        أرجع حالت;
    }

    motion->flags = event->اشارو_motion.flags;
    motion->x = event->اشارو_motion.x;
    motion->y = event->اشارو_motion.y;

    أرجع XKB_SUCCESS;
}

تعداد xkb_حالت
xkb_event_get_اشارو_button(ثابت بنية xkb_event * محدود event,
                             بنية xkb_event_اشارو_button * محدود button)
{
    إذا (event->type != XKB_EVENT_TYPE_اشارو_BUTTON)
        أرجع XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ثابت تعداد xkb_حالت حالت = xkb_check_state_abi(button);
    إذا (حالت != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, حالت);
        أرجع حالت;
    }

    button->button = event->اشارو_button.button;
    button->state = event->اشارو_button.state;
    button->ڳڻپ = event->اشارو_button.ڳڻپ;

    أرجع XKB_SUCCESS;
}

تعداد xkb_حالت
xkb_event_get_virtual_console(ثابت بنية xkb_event * محدود event,
                              int8_t * محدود اشاريو_or_وٿي,
                              bool * محدود is_وٿي)
{
    إذا (event->type != XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
        أرجع XKB_ERROR_EVENT_TYPE_MISMATCH;

    *اشاريو_or_وٿي = event->virtual_console.اشاريو_or_وٿي;
    *is_وٿي = event->virtual_console.is_وٿي;

    أرجع XKB_SUCCESS;
}

تعداد xkb_حالت
xkb_state_update_event(بنية xkb_state * محدود بنياد_state,
                       ثابت بنية xkb_event * محدود event,
                       تعداد xkb_state_component * محدود بدليو)
{
    /* Guard against server-only state */
    أكد(بنياد_state->گهڻائي == SERVER_COMPANION ||
           بنياد_state->گهڻائي == LEGACY_MIXED_STATE);
    إذا (بنياد_state->گهڻائي != SERVER_COMPANION &&
        بنياد_state->گهڻائي != LEGACY_MIXED_STATE) {
        log_err(بنياد_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_گهڻائي_2,
                "%s: Unexpected state type %d\n", __func__, بنياد_state->گهڻائي);
        أرجع XKB_ERROR_UNEXPECTED_STATE_گهڻائي;
    }

    بنية xkb_client_state * ثابت state =
        (بنية xkb_client_state *)بنياد_state;

    إذا (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
        ثابت بنية state_components prev_components = state->بنياد.components;
        state->بنياد.components = event->components.components;
        /*
         * Recompute the changes instead of using the event value, because we do
         * not know if the event’s queue and the state are synced.
         */
        إذا (بدليو)
            *بدليو = get_state_component_changes(&prev_components,
                                                   &state->بنياد.components);
    } وإلا {
        إذا (بدليو)
            *بدليو = 0;
    }
    أرجع XKB_SUCCESS;
}

ساكن فراغ
append_اشارو_button(بنية xkb_events * محدود events,
                      بنية xkb_filter * محدود filter,
                      تعداد xkb_اشارو_button_state button_state,
                      uint8_t ڳڻپ)
{
    darray_append(events->قطار, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_اشارو_BUTTON,
        .اشارو_button = {
            .سائيز = حجم(((بنية xkb_event*)0)->اشارو_button),
            .button = filter->priv,
            .state = button_state,
            .ڳڻپ = ڳڻپ,
        }
    });
}

ساكن فراغ
xkb_filter_اشارو_button_new(بنية xkb_server_state *state,
                              بنية xkb_events *events,
                              بنية xkb_filter *filter)
{
    إذا (!events || state->بنياد.گهڻائي != SERVER_STATE ||
        !(state->بنياد.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ڪنجيون)) {
        filter->func = عدم;
        أرجع;
    }

    بنية xkb_machine * ثابت sm = (بنية xkb_machine *)state;

    /* Resolve button */
    ثابت xkb_اشارو_button_اشاريو_t button =
        (filter->action.btn.button == XKB_اشارو_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;
    filter->priv = button;

    ثابت تعداد xkb_اشارو_button_state button_state =
        XKB_اشارو_BUTTON_PRESSED;
    append_اشارو_button(events, filter, button_state,
                          filter->action.btn.ڳڻپ);
    state->flags &= ~STATE_REQUIRE_ڪنجي_EVENT;
}

ساكن bool
xkb_filter_اشارو_button_func(بنية xkb_server_state *state,
                               بنية xkb_events *events,
                               بنية xkb_filter *filter,
                               ثابت بنية xkb_ڪنجي *ڪنجي,
                               تعداد xkb_ڪنجي_direction direction)
{
    إذا (ڪنجي != filter->ڪنجي || !events)
        أرجع XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ڪنجي_EVENT;

    بدل (direction) {
    حالة XKB_ڪنجي_هيٺ_ڏانهن:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_ڪنجي_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    إذا (!filter->action.btn.ڳڻپ)
        append_اشارو_button(events, filter, XKB_اشارو_BUTTON_RELEASED, 0);

    filter->func = عدم;
    أرجع XKB_FILTER_CONSUME;
}

بنية xkb_filter_اشارو_تالو_button_priv {
    uint32_t button:31;
#جيڪڏهن_وصف_ٿيل _WIN32
    uint32_t locked:1; // MSVC requires matching types to pack bitfields
#وإلا
    bool locked:1;
#جيڪڏهن_پورو
};

تعداد {
    XKB_FILTER_اشارو_تالو_BUTTON_PRIV_ويڪر =
        حجم(بنية xkb_filter_اشارو_تالو_button_priv) * CHAR_BIT,
    XKB_FILTER_اشارو_تالو_BUTTON_PRIV_MAX =
        (UINT64_C(1) << XKB_FILTER_اشارو_تالو_BUTTON_PRIV_ويڪر) - 1
};

static_assert(حجم(بنية xkb_filter_اشارو_تالو_button_priv) ==
              حجم(((بنية xkb_filter*)0)->priv),
              "");
static_assert((uint32_t)XKB_اشارو_BUTTON_MAX < (uint32_t)XKB_FILTER_اشارو_تالو_BUTTON_PRIV_MAX,
              "xkb_filter_pointer_lock_button_priv cannot store button");

ساكن فراغ
xkb_filter_اشارو_تالو_button_new(بنية xkb_server_state *state,
                                   بنية xkb_events *events,
                                   بنية xkb_filter *filter)
{
    إذا (!events || state->بنياد.گهڻائي != SERVER_STATE ||
        !(state->بنياد.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ڪنجيون)) {
        filter->func = عدم;
        أرجع;
    }

    state->flags &= ~STATE_REQUIRE_ڪنجي_EVENT;

    بنية xkb_machine * ثابت sm = (بنية xkb_machine *)state;

    /* Resolve button */
    ثابت xkb_اشارو_button_اشاريو_t button =
        (filter->action.btn.button == XKB_اشارو_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;

    static_assert(XKB_اشارو_BUTTON_نقاب_ويڪر <= 8,
                  "invalid UINTx_C() macro");
    static_assert(XKB_اشارو_BUTTON_MAX <= XKB_اشارو_BUTTON_نقاب_ويڪر,
                  "invalid left shift");
    ثابت xkb_اشارو_button_نقاب_t نقاب = (UINT8_C(1) << button);

    ثابت bool locked = (sm->mouse.locked_buttons & نقاب);

    إذا (!locked && !(filter->action.btn.flags & ACTION_تالو_NO_تالو)) {
        sm->mouse.locked_buttons |= نقاب;
        filter->priv = button;
        append_اشارو_button(events, filter, XKB_اشارو_BUTTON_PRESSED, 0);
    }

    بنية xkb_filter_اشارو_تالو_button_priv *priv =
        (بنية xkb_filter_اشارو_تالو_button_priv*)&filter->priv;
    priv->button = button,
    priv->locked = locked;
}

ساكن bool
xkb_filter_اشارو_تالو_button_func(بنية xkb_server_state *state,
                                    بنية xkb_events *events,
                                    بنية xkb_filter *filter,
                                    ثابت بنية xkb_ڪنجي *ڪنجي,
                                    تعداد xkb_ڪنجي_direction direction)
{
    إذا (ڪنجي != filter->ڪنجي || !events || state->بنياد.گهڻائي != SERVER_STATE)
        أرجع XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ڪنجي_EVENT;

    بدل (direction) {
    حالة XKB_ڪنجي_هيٺ_ڏانهن:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_ڪنجي_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    ثابت بنية xkb_filter_اشارو_تالو_button_priv *priv =
        (بنية xkb_filter_اشارو_تالو_button_priv*)&filter->priv;

    إذا (priv->locked && !(filter->action.btn.flags & ACTION_تالو_NO_UNLOCK)) {
        بنية xkb_machine * ثابت sm = (بنية xkb_machine *)state;

        static_assert(XKB_اشارو_BUTTON_نقاب_ويڪر <= 8,
                    "invalid UINTx_C() macro");
        static_assert(XKB_اشارو_BUTTON_MAX <= XKB_اشارو_BUTTON_نقاب_ويڪر,
                    "invalid left shift");
        ثابت xkb_اشارو_button_نقاب_t نقاب = (UINT8_C(1) << priv->button);

        sm->mouse.locked_buttons &= ~نقاب;
        filter->priv = priv->button;
        append_اشارو_button(events, filter, XKB_اشارو_BUTTON_RELEASED, 0);
    }

    filter->func = عدم;
    أرجع XKB_FILTER_CONSUME;
}

ساكن فراغ
xkb_filter_اشارو_سيٽ_default_button_new(بنية xkb_server_state *state,
                                          بنية xkb_events *events,
                                          بنية xkb_filter *filter)
{
    إذا (!events || state->بنياد.گهڻائي != SERVER_STATE ||
        !(state->بنياد.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_ڪنجيون)) {
        filter->func = عدم;
        أرجع;
    }

    state->flags &= ~STATE_REQUIRE_ڪنجي_EVENT;

    بنية xkb_machine * ثابت sm = (بنية xkb_machine *)state;

    static_assert(2 * XKB_اشارو_BUTTON_MAX <= وڌ_کان_وڌ_عدد, "lossy conversion");
    صحيح button = (filter->action.dflt.flags & ACTION_ABSOLUTE_SWITCH)
        ? filter->action.dflt.قدر
        : sm->mouse.default_button + filter->action.dflt.قدر;

    /* Wrap into range */
    ثابت صحيح rem = button % XKB_اشارو_BUTTON_MAX;
    sm->mouse.default_button = (rem >= 0)
        ? (xkb_اشارو_button_اشاريو_t)rem
        : (xkb_اشارو_button_اشاريو_t)(rem + XKB_اشارو_BUTTON_MAX);
}

ساكن bool
xkb_filter_اشارو_سيٽ_default_button_func(بنية xkb_server_state *state,
                                           بنية xkb_events *events,
                                           بنية xkb_filter *filter,
                                           ثابت بنية xkb_ڪنجي *ڪنجي,
                                           تعداد xkb_ڪنجي_direction direction)
{
    إذا (ڪنجي != filter->ڪنجي || !events)
        أرجع XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ڪنجي_EVENT;

    بدل (direction) {
    حالة XKB_ڪنجي_هيٺ_ڏانهن:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_ڪنجي_REPEATED:
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
    إذا (!events || state->بنياد.گهڻائي != SERVER_STATE ||
        !(((بنية xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = عدم;
        أرجع;
    }

    state->flags &= ~STATE_REQUIRE_ڪنجي_EVENT;

    darray_append(events->قطار, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_TERMINATE_DISPLAY_SERVER,
    });
}

ساكن bool
xkb_filter_terminate_server_func(بنية xkb_server_state *state,
                                 بنية xkb_events *events,
                                 بنية xkb_filter *filter,
                                 ثابت بنية xkb_ڪنجي *ڪنجي,
                                 تعداد xkb_ڪنجي_direction direction)
{
    إذا (ڪنجي != filter->ڪنجي || !events)
        أرجع XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ڪنجي_EVENT;

    بدل (direction) {
    حالة XKB_ڪنجي_هيٺ_ڏانهن:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_ڪنجي_REPEATED:
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
    إذا (!events || state->بنياد.گهڻائي != SERVER_STATE ||
        !(((بنية xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = عدم;
        أرجع;
    }

    state->flags &= ~STATE_REQUIRE_ڪنجي_EVENT;

    darray_append(events->قطار, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
        .virtual_console = {
            .اشاريو_or_وٿي = filter->action.screen.screen,
            .is_وٿي = !(filter->action.screen.flags & ACTION_ABSOLUTE_SWITCH)
        }
    });
}

ساكن bool
xkb_filter_switch_virtual_console_func(بنية xkb_server_state *state,
                                       بنية xkb_events *events,
                                       بنية xkb_filter *filter,
                                       ثابت بنية xkb_ڪنجي *ڪنجي,
                                       تعداد xkb_ڪنجي_direction direction)
{
    إذا (ڪنجي != filter->ڪنجي || !events)
        أرجع XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_ڪنجي_EVENT;

    بدل (direction) {
    حالة XKB_ڪنجي_هيٺ_ڏانهن:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_ڪنجي_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    filter->func = عدم;
    أرجع XKB_FILTER_CONSUME;
}
