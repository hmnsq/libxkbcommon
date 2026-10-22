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
    darray_ناپ_t اگلا;
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

تعداد xkb_state_منوال_اندرونی {
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

تعداد { XKB_LAYOUT_اشاریہ_T_MIN_چوڑائی = 6 };
static_assert(XKB_MAX_GROUPS < (1 << XKB_LAYOUT_اشاریہ_T_MIN_چوڑائی),
              "Cannot encode out_of_range_group::redirect_group");

بنية باہرلا_دا_range_گروہ {
    تعداد xkb_layout_باہرلا_دا_range_policy policy :
        (حجم(تعداد xkb_layout_باہرلا_دا_range_policy) * CHAR_BIT -
         XKB_LAYOUT_اشاریہ_T_MIN_چوڑائی);
    xkb_layout_اشاریہ_t redirect_گروہ : XKB_LAYOUT_اشاریہ_T_MIN_چوڑائی;
};

تعداد { XKB_STATE_منوال_اندرونی_MIN_چوڑائی = 4 /* 3 bits + sign */ };
static_assert(XKB_STATE_TYPE_عدد_ENTRIES <
              (1 << XKB_STATE_منوال_اندرونی_MIN_چوڑائی),
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
    بنية باہرلا_دا_range_گروہ باہرلا_دا_range_گروہ;

    تعداد xkb_state_منوال_اندرونی منوال : XKB_STATE_منوال_اندرونی_MIN_چوڑائی;
    صحيح refcnt : (حجم(صحيح) * CHAR_BIT - XKB_STATE_منوال_اندرونی_MIN_چوڑائی);
};

/**
 * Minimal client state
 *
 * Wrapper for strong typing
 */
بنية xkb_client_state {
    /** Inherits from `xkb_state` */
    بنية xkb_state ادھار;
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
    بنية xkb_state ادھار;

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

ساكن ثابت بنية xkb_کلید_type_اندراج *
get_اندراج_for_mods(ثابت بنية xkb_کلید_type *type, xkb_mod_نقاب_t mods)
{
    لكل (darray_ناپ_t i = 0; i < type->عدد_entries; i++)
        إذا (اندراج_is_سرگرم(&type->entries[i]) &&
            type->entries[i].mods.نقاب == mods)
            أرجع &type->entries[i];
    أرجع عدم;
}

ساكن ثابت بنية xkb_کلید_type_اندراج *
get_اندراج_for_کلید_state(بنية xkb_state *state, ثابت بنية xkb_کلید *کلید,
                        xkb_layout_اشاریہ_t گروہ)
{
    ثابت بنية xkb_کلید_type* ثابت type = کلید->groups[گروہ].type;
    xkb_mod_نقاب_t سرگرم_mods = state->components.mods & type->mods.نقاب;
    أرجع get_اندراج_for_mods(type, سرگرم_mods);
}

ساكن مضمن xkb_level_اشاریہ_t
state_کلید_get_level(بنية xkb_state *state, ثابت بنية xkb_کلید *کلید,
                         xkb_layout_اشاریہ_t layout)
{
    إذا (layout >= کلید->عدد_groups)
        أرجع XKB_LEVEL_INVALID;

    /* If we don't find an explicit match the default is 0. */
    ثابت بنية xkb_کلید_type_اندراج* ثابت اندراج =
        get_اندراج_for_کلید_state(state, کلید, layout);

    أرجع (اندراج) ? اندراج->level : 0;
}

/**
 * Returns the level to use for the given key and state, or
 * XKB_LEVEL_INVALID.
 */
xkb_level_اشاریہ_t
xkb_state_کلید_get_level(بنية xkb_state *state, xkb_keycode_t kc,
                        xkb_layout_اشاریہ_t layout)
{
    ثابت بنية xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);

    أرجع (کلید)
        ? state_کلید_get_level(state, کلید, layout)
        : XKB_LEVEL_INVALID;
}

ساكن مضمن xkb_layout_اشاریہ_t
state_کلید_get_layout(بنية xkb_state *state, ثابت بنية xkb_کلید *کلید)
{
    أرجع xkb_keymap_کلید_effective_layout(کلید, state->components.گروہ);
}

/**
 * Returns the layout to use for the given key and state, taking
 * wrapping/clamping/etc into account, or XKB_LAYOUT_INVALID.
 */
xkb_layout_اشاریہ_t
xkb_state_کلید_get_layout(بنية xkb_state *state, xkb_keycode_t kc)
{
    ثابت بنية xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);

    إذا (!کلید)
        أرجع XKB_LAYOUT_INVALID;

    أرجع state_کلید_get_layout(state, کلید);
}

/* Empty action used for empty levels */
ساكن ثابت اتحاد xkb_action dummy_action = { .type = ACTION_TYPE_NONE };

ساكن xkb_action_گنتی_t
xkb_کلید_get_actions(بنية xkb_state *state, ثابت بنية xkb_کلید *کلید,
                    ثابت اتحاد xkb_action **actions)
{
    ثابت xkb_layout_اشاریہ_t layout = state_کلید_get_layout(state, کلید);
    ثابت xkb_level_اشاریہ_t level = state_کلید_get_level(state, کلید, layout);
    إذا (level == XKB_LEVEL_INVALID)
        اذهب err;

    ثابت xkb_action_گنتی_t گنتی =
        xkb_keymap_کلید_get_actions_by_level(state->keymap, کلید,
                                            layout, level, actions);
    إذا (!گنتی)
        اذهب err;

    أرجع گنتی;

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
        darray_resize0(state->filters, darray_ناپ(state->filters) + 1);
        filter = &darray_شے(state->filters, darray_ناپ(state->filters) -1);
    }

    filter->refcnt = 1;
    أرجع filter;
}

/***====================================================================***/

تعداد xkb_filter_نتیجہ {
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
    إذا ((filter_)->action.گروہ.flags & ACTION_ABSOLUTE_SWITCH)          \
        (state_)->components.component_ = (filter_)->action.گروہ.گروہ; \
    وإلا                                                                 \
        (state_)->components.component_ += (filter_)->action.گروہ.گروہ

ساكن فراغ
xkb_filter_گروہ_سیٹ_new(بنية xkb_server_state *state,
                         بنية xkb_events *events,
                         بنية xkb_filter *filter)
{
    static_assert(حجم(state->ادھار.components.ادھار_گروہ) ==
                  حجم(filter->priv),
                  "Max groups don't fit");
    filter->priv = (uint32_t)state->ادھار.components.ادھار_گروہ;
    apply_گروہ_delta(filter, &state->ادھار, ادھار_گروہ);
}

ساكن bool
xkb_filter_گروہ_سیٹ_func(بنية xkb_server_state *state,
                          بنية xkb_events *events,
                          بنية xkb_filter *filter,
                          ثابت بنية xkb_کلید *کلید,
                          تعداد xkb_کلید_direction direction)
{
    إذا (کلید != filter->کلید) {
        filter->action.گروہ.flags &= ~ACTION_جندرا_CLEAR;
        أرجع XKB_FILTER_CONTINUE;
    }

    بدل (direction) {
    حالة XKB_کلید_تھلے:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلید_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    static_assert(حجم(state->ادھار.components.ادھار_گروہ) == حجم(filter->priv),
                  "Max groups don't fit");
    state->ادھار.components.ادھار_گروہ = (int32_t) filter->priv;

    إذا (filter->action.گروہ.flags & ACTION_جندرا_CLEAR)
        state->ادھار.components.locked_گروہ = 0;

    filter->func = عدم;
    أرجع XKB_FILTER_CONTINUE;
}

ساكن تعداد xkb_state_component
get_state_component_changes(ثابت بنية state_components *a,
                            ثابت بنية state_components *b)
{
    تعداد xkb_state_component نقاب = 0;

    إذا (a->گروہ != b->گروہ)
        نقاب |= XKB_STATE_LAYOUT_EFFECTIVE;
    إذا (a->ادھار_گروہ != b->ادھار_گروہ)
        نقاب |= XKB_STATE_LAYOUT_DEPRESSED;
    إذا (a->latched_گروہ != b->latched_گروہ)
        نقاب |= XKB_STATE_LAYOUT_LATCHED;
    إذا (a->locked_گروہ != b->locked_گروہ)
        نقاب |= XKB_STATE_LAYOUT_LOCKED;
    إذا (a->mods != b->mods)
        نقاب |= XKB_STATE_MODS_EFFECTIVE;
    إذا (a->ادھار_mods != b->ادھار_mods)
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
xkb_filter_گروہ_جندرا_new(بنية xkb_server_state *state,
                          بنية xkb_events *events,
                          بنية xkb_filter *filter)
{
    إذا (filter->action.گروہ.flags & ACTION_جندرا_ON_RELEASE) {
        /*
         * Lock on key release: do nothing on key press.
         *
         * This is a keymap format v2 extension.
         */
        أرجع;
    } وإلا {
        /* Lock on key press */
        apply_گروہ_delta(filter, &state->ادھار, locked_گروہ);
    }
}

ساكن bool
xkb_filter_گروہ_جندرا_func(بنية xkb_server_state *state,
                           بنية xkb_events *events,
                           بنية xkb_filter *filter,
                           ثابت بنية xkb_کلید *کلید,
                           تعداد xkb_کلید_direction direction)
{
    إذا (کلید != filter->کلید) {
        إذا ((filter->action.گروہ.flags & ACTION_جندرا_ON_RELEASE) &&
            direction == XKB_کلید_تھلے) {
            /*
             * Another key has been pressed after the locking key:
             * cancel group lock on release.
             *
             * This is a keymap v2 extension.
             */
            filter->action.گروہ.flags &= ~ACTION_جندرا_ON_RELEASE;
        }
        أرجع XKB_FILTER_CONTINUE;
    }

    بدل (direction) {
    حالة XKB_کلید_تھلے:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلید_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    إذا (filter->action.گروہ.flags & ACTION_جندرا_ON_RELEASE) {
        /*
         * Lock on key release
         *
         * This is a keymap v2 extension.
         */
        apply_گروہ_delta(filter, &state->ادھار, locked_گروہ);
    } وإلا {
        /* Lock on key press: do nothing on key release. */
    }
    filter->func = عدم;
    أرجع XKB_FILTER_CONTINUE;
}

ساكن bool
xkb_action_breaks_latch(ثابت اتحاد xkb_action *action,
                        تعداد xkb_action_controls controls,
                        تعداد xkb_اندرونی_action_flags flag,
                        xkb_mod_نقاب_t نقاب)
{
    بدل (action->type) {
    حالة ACTION_TYPE_NONE:
    حالة ACTION_TYPE_VOID:
    حالة ACTION_TYPE_PTR_BUTTON:
    حالة ACTION_TYPE_PTR_جندرا:
    حالة ACTION_TYPE_CTRL_سیٹ:
    حالة ACTION_TYPE_CTRL_جندرا:
    حالة ACTION_TYPE_SWITCH_VT:
    حالة ACTION_TYPE_TERMINATE:
    حالة ACTION_TYPE_REDIRECT_کلید:
        أرجع true;
    حالة ACTION_TYPE_PTR_MOVE:
    حالة ACTION_TYPE_PTR_DEFAULT:
        أرجع !(controls & CONTROL_MOUSE_کلیدیں);
    حالة ACTION_TYPE_اندرونی:
        أرجع (action->اندرونی.flags & flag) &&
               ((action->اندرونی.clear_latched_mods & نقاب) == نقاب);
    افتراضي:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_اندرونی == 20 &&
                      ACTION_TYPE_اندرونی + 1 == ACTION_TYPE_عدد_ENTRIES,
                      "Missing action type");
        أرجع false;
    }
}

تعداد xkb_کلید_latch_state {
    NO_LATCH = 0,
    LATCH_کلید_تھلے,
    LATCH_PENDING,
    کلید_LATCH_STATE_عدد_ENTRIES
};

#تعریف_کرو MAX_XKB_کلید_LATCH_STATE_LOG2 2
#إذا (کلید_LATCH_STATE_عدد_ENTRIES > (1 << MAX_XKB_کلید_LATCH_STATE_LOG2)) || \
    (-XKB_MAX_GROUPS) < (INT32_MIN >> MAX_XKB_کلید_LATCH_STATE_LOG2) || \
      XKB_MAX_GROUPS > (INT32_MAX >> MAX_XKB_کلید_LATCH_STATE_LOG2)
#غلطی "Cannot represent priv field of the group latch filter"
#جے_انت

/* Hold the latch state *and* the group delta */
اتحاد گروہ_latch_priv {
    uint32_t priv;
    بنية {
        /* The type is really: enum xkb_key_latch_state, but it is problematic
         * on Windows, because it is interpreted as signed and leads to wrong
         * negative values. */
        بنانشانی صحيح latch:MAX_XKB_کلید_LATCH_STATE_LOG2;
        int32_t گروہ_delta:(32 - MAX_XKB_کلید_LATCH_STATE_LOG2);
    };
};

ساكن فراغ
xkb_filter_گروہ_latch_new(بنية xkb_server_state *state,
                           بنية xkb_events *events,
                           بنية xkb_filter *filter)
{
    ثابت اتحاد گروہ_latch_priv priv = {
        .latch = LATCH_کلید_تھلے,
        .گروہ_delta = (filter->action.گروہ.flags & ACTION_ABSOLUTE_SWITCH)
            ? filter->action.گروہ.گروہ - state->ادھار.components.ادھار_گروہ
            : filter->action.گروہ.گروہ
    };
    filter->priv = priv.priv;
    /* Like group set */
    apply_گروہ_delta(filter, &state->ادھار, ادھار_گروہ);
}

ساكن bool
xkb_filter_گروہ_latch_func(بنية xkb_server_state *state,
                            بنية xkb_events *events,
                            بنية xkb_filter *filter,
                            ثابت بنية xkb_کلید *کلید,
                            تعداد xkb_کلید_direction direction)
{
    اتحاد گروہ_latch_priv priv = {.priv = filter->priv};
    تعداد xkb_کلید_latch_state latch = priv.latch;

    إذا (direction == XKB_کلید_تھلے) {
        ثابت اتحاد xkb_action *actions = عدم;
        ثابت xkb_action_گنتی_t گنتی = xkb_کلید_get_actions(&state->ادھار, کلید,
                                                             &actions);

        إذا (latch == LATCH_کلید_تھلے) {
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
            إذا (state->a11y_flags & XKB_A11_Y_LATCH_اکو_ویلے_کلیدیں) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                لكل (xkb_action_گنتی_t k = 0; k < گنتی; k++) {
                    إذا (xkb_action_breaks_latch(&(actions[k]),
                                                state->ادھار.components.controls,
                                                اندرونی_BREAKS_گروہ_LATCH,
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
            ثابت bool sticky_کلیدیں = state->ادھار.components.controls
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
            لكل (xkb_action_گنتی_t k = 0; k < گنتی; k++) {
                إذا (((actions[k].type == ACTION_TYPE_گروہ_LATCH) ||
                     (actions[k].type == ACTION_TYPE_گروہ_سیٹ && sticky_کلیدیں)) &&
                     actions[k].گروہ.گروہ == filter->action.گروہ.گروہ &&
                     (actions[k].گروہ.flags & ACTION_ABSOLUTE_SWITCH) ==
                     (filter->action.گروہ.flags & ACTION_ABSOLUTE_SWITCH)) {
                    إذا (filter->action.گروہ.flags & ACTION_LATCH_منزل_جندرا &&
                        filter->action.گروہ.گروہ != 0)
                    {
                        /* Promote to lock */
                        filter->action.type = ACTION_TYPE_گروہ_جندرا;
                        filter->func = xkb_filter_گروہ_جندرا_func;
                        xkb_filter_گروہ_جندرا_new(state, events, filter);
                        state->ادھار.components.latched_گروہ -= priv.گروہ_delta;
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
                                                 state->ادھار.components.controls,
                                                 اندرونی_BREAKS_گروہ_LATCH,
                                                 0))
                {
                    /* Breaks the latch */
                    state->ادھار.components.latched_گروہ -= priv.گروہ_delta;
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
        إذا ((filter->action.گروہ.flags & ACTION_جندرا_CLEAR) &&
             state->ادھار.components.locked_گروہ) {
            إذا (latch == LATCH_PENDING)
                state->ادھار.components.latched_گروہ -= priv.گروہ_delta;
            وإلا
                state->ادھار.components.ادھار_گروہ -= priv.گروہ_delta;
            state->ادھار.components.locked_گروہ = 0;
            filter->func = عدم;
        }
        /* Broken latch */
        وإلا إذا (latch == NO_LATCH) {
            state->ادھار.components.ادھار_گروہ -= priv.گروہ_delta;
            filter->func = عدم;
        }
        /* We may already have reached the latch state if pressing the
         * key multiple times without latch-to-lock enabled. */
        وإلا إذا (latch == LATCH_کلید_تھلے) {
            latch = LATCH_PENDING;
            /* Switch from set to latch */
            state->ادھار.components.ادھار_گروہ -= priv.گروہ_delta;
            state->ادھار.components.latched_گروہ += priv.گروہ_delta;
            /* XXX beep beep! */
        }
    }

    priv.latch = latch;
    filter->priv = priv.priv;

    أرجع XKB_FILTER_CONTINUE;
}

ساكن فراغ
xkb_filter_mod_سیٹ_new(بنية xkb_server_state *state,
                       بنية xkb_events *events,
                       بنية xkb_filter *filter)
{
    ثابت تعداد xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_جندرا_CLEAR;
    إذا ((filter->action.mods.flags & unlock) == unlock) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension.
         */
        filter->priv = filter->action.mods.mods.نقاب
                     & ~state->ادھار.components.locked_mods;
        state->ادھار.components.locked_mods &= ~filter->action.mods.mods.نقاب;
    } وإلا {
        filter->priv = filter->action.mods.mods.نقاب;
    }

    state->سیٹ_mods |= (xkb_mod_نقاب_t) filter->priv;
}

ساكن bool
xkb_filter_mod_سیٹ_func(بنية xkb_server_state *state,
                        بنية xkb_events *events,
                        بنية xkb_filter *filter,
                        ثابت بنية xkb_کلید *کلید,
                        تعداد xkb_کلید_direction direction)
{

    إذا (کلید != filter->کلید) {
        filter->action.mods.flags &= ~ACTION_جندرا_CLEAR;
        أرجع XKB_FILTER_CONTINUE;
    }

    بدل (direction) {
    حالة XKB_کلید_تھلے:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلید_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    state->clear_mods |= (xkb_mod_نقاب_t) filter->priv;
    ثابت تعداد xkb_action_flags unlock = ACTION_UNLOCK_ON_PRESS
                                       | ACTION_جندرا_CLEAR;
    إذا ((filter->action.mods.flags & unlock) == ACTION_جندرا_CLEAR)
        state->ادھار.components.locked_mods &= ~filter->action.mods.mods.نقاب;

    filter->func = عدم;
    أرجع XKB_FILTER_CONTINUE;
}

ساكن فراغ
xkb_filter_mod_جندرا_new(بنية xkb_server_state *state,
                        بنية xkb_events *events,
                        بنية xkb_filter *filter)
{
    filter->priv = (state->ادھار.components.locked_mods &
                    filter->action.mods.mods.نقاب);

    إذا (filter->priv && (filter->action.mods.flags & ACTION_UNLOCK_ON_PRESS)) {
        /*
         * Some of the target modifiers were locked before key press: unlock.
         *
         * This is a keymap v2 extension: unlock-on-press.
         */
        إذا (!(filter->action.mods.flags & ACTION_جندرا_NO_UNLOCK))
            state->ادھار.components.locked_mods &= ~filter->priv;
        /* No further action: cancel filter */
        filter->func = عدم;
    } وإلا {
        /* Set base mods; lock mods if relevant (XKB 1.0 spec) */
        state->سیٹ_mods |= filter->action.mods.mods.نقاب;
        إذا (!(filter->action.mods.flags & ACTION_جندرا_NO_جندرا))
            state->ادھار.components.locked_mods |= filter->action.mods.mods.نقاب;
    }
}

ساكن bool
xkb_filter_mod_جندرا_func(بنية xkb_server_state *state,
                         بنية xkb_events *events,
                         بنية xkb_filter *filter,
                         ثابت بنية xkb_کلید *کلید,
                         تعداد xkb_کلید_direction direction)
{
    إذا (کلید != filter->کلید)
        أرجع XKB_FILTER_CONTINUE;

    بدل (direction) {
    حالة XKB_کلید_تھلے:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلید_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    state->clear_mods |= filter->action.mods.mods.نقاب;
    إذا (!(filter->action.mods.flags & ACTION_جندرا_NO_UNLOCK))
        state->ادھار.components.locked_mods &= ~filter->priv;

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

    إذا ((filter->action.mods.flags & ACTION_جندرا_CLEAR) &&
        (filter->action.mods.flags & unlockOnPress) &&
        (state->ادھار.components.locked_mods & filter->action.mods.mods.نقاب) ==
         filter->action.mods.mods.نقاب) {
        /*
         * Unlock on press
         *
         * This is a keymap v2 extension: clear locks and do not latch.
         */
        state->ادھار.components.locked_mods &= ~filter->action.mods.mods.نقاب;
        filter->func = عدم;
    } وإلا إذا (filter->action.mods.flags & ACTION_LATCH_ON_PRESS) {
        /*
         * Latch on key press
         *
         * This is a keymap format v2 extension.
         */
        filter->priv = LATCH_PENDING;
        state->ادھار.components.latched_mods |= filter->action.mods.mods.نقاب;
        /* XXX beep beep! */
    } وإلا {
        /* XKB standard latch action */
        filter->priv = LATCH_کلید_تھلے;
        state->سیٹ_mods |= filter->action.mods.mods.نقاب;
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

    إذا (direction == XKB_کلید_تھلے) {
        ثابت اتحاد xkb_action *actions = عدم;
        ثابت xkb_action_گنتی_t گنتی = xkb_کلید_get_actions(&state->ادھار, کلید,
                                                             &actions);

        إذا (latch == LATCH_کلید_تھلے) {
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
            إذا (state->a11y_flags & XKB_A11_Y_LATCH_اکو_ویلے_کلیدیں) {
                /*
                 * Prevent the latch to trigger only if some of the pressed
                 * key’s actions breaks latches, mirroring the behavior in the
                 * LATCH_PENDING state.
                 *
                 * This is an extension to the X11 XKB protocol.
                 */
                لكل (xkb_action_گنتی_t k = 0; k < گنتی; k++) {
                    إذا (xkb_action_breaks_latch(&(actions[k]),
                                                state->ادھار.components.controls,
                                                اندرونی_BREAKS_MOD_LATCH,
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
            ثابت bool sticky_کلیدیں = state->ادھار.components.controls
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
            لكل (xkb_action_گنتی_t k = 0; k < گنتی; k++) {
                إذا (((actions[k].type == ACTION_TYPE_MOD_LATCH) ||
                     (actions[k].type == ACTION_TYPE_MOD_سیٹ && sticky_کلیدیں)) &&
                    actions[k].mods.mods.نقاب == filter->action.mods.mods.نقاب)
                {
                    إذا (filter->action.mods.flags & ACTION_LATCH_منزل_جندرا) {
                        /* Mutate the action to LockMods() */
                        filter->action.type = ACTION_TYPE_MOD_جندرا;
                        filter->func = xkb_filter_mod_جندرا_func;
                        xkb_filter_mod_جندرا_new(state, events, filter);
                    }
                    وإلا {
                        /* Mutate the action to SetMods() */
                        filter->action.type = ACTION_TYPE_MOD_سیٹ;
                        filter->func = xkb_filter_mod_سیٹ_func;
                        xkb_filter_mod_سیٹ_new(state, events, filter);
                    }
                    filter->کلید = کلید;
                    /* Clear latches */
                    state->ادھار.components.latched_mods &=
                        ~filter->action.mods.mods.نقاب;
                    /* XXX beep beep! */
                    أرجع XKB_FILTER_CONSUME;
                }
                وإلا إذا (xkb_action_breaks_latch(&(actions[k]),
                                                 state->ادھار.components.controls,
                                                 اندرونی_BREAKS_MOD_LATCH,
                                                 filter->action.mods.mods.نقاب))
                {
                    /* XXX: This may be totally broken, we might need to break the
                     *      latch in the next run after this press? */
                    state->ادھار.components.latched_mods &=
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

        إذا ((filter->action.mods.flags & ACTION_جندرا_CLEAR) &&
            !(filter->action.mods.flags & unlockOnPress) &&
            (state->ادھار.components.locked_mods &
             filter->action.mods.mods.نقاب) == filter->action.mods.mods.نقاب)
        {
            /* XXX: We might be a bit overenthusiastic about clearing
             *      mods other filters have set here? */
            إذا (latch == LATCH_PENDING)
                state->ادھار.components.latched_mods &=
                    ~filter->action.mods.mods.نقاب;
            وإلا
                state->clear_mods |= filter->action.mods.mods.نقاب;
            state->ادھار.components.locked_mods &= ~filter->action.mods.mods.نقاب;
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
            state->ادھار.components.latched_mods |= filter->action.mods.mods.نقاب;
            /* XXX beep beep! */
        }
    }

    filter->priv = latch;

    أرجع XKB_FILTER_CONTINUE;
}

ساكن فراغ
append_اشارہ_گر_move(بنية xkb_events *events,
                    بنية xkb_filter *filter)
{
    تعداد xkb_اشارہ_گر_motion_flags flags = 0;
    إذا (filter->action.ptr.flags & ACTION_ABSOLUTE_X)
        flags |= XKB_اشارہ_گر_MOTION_ABSOLUTE_X;
    إذا (filter->action.ptr.flags & ACTION_ABSOLUTE_Y)
        flags |= XKB_اشارہ_گر_MOTION_ABSOLUTE_Y;
    إذا (filter->action.ptr.flags & ACTION_REPEAT)
        flags |= XKB_اشارہ_گر_MOTION_REPEATS;
    darray_append(events->قطار, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_اشارہ_گر_MOTION,
        .اشارہ_گر_motion = {
            .ناپ = حجم(((بنية xkb_event*)0)->اشارہ_گر_motion),
            .flags = flags,
            .x = filter->action.ptr.x,
            .y = filter->action.ptr.y,
        }
    });
}

ساكن فراغ
xkb_filter_اشارہ_گر_move_new(بنية xkb_server_state *state,
                            بنية xkb_events *events,
                            بنية xkb_filter *filter)
{
    إذا (!events) {
        filter->func = عدم;
        أرجع;
    }

    أكد(state->ادھار.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_کلیدیں);
    append_اشارہ_گر_move(events, filter);
    state->flags &= ~STATE_REQUIRE_کلید_EVENT;
}

ساكن bool
xkb_filter_اشارہ_گر_move_func(بنية xkb_server_state *state,
                             بنية xkb_events *events,
                             بنية xkb_filter *filter,
                             ثابت بنية xkb_کلید *کلید,
                             تعداد xkb_کلید_direction direction)
{
    إذا (کلید != filter->کلید || !events)
        أرجع XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    بدل (direction) {
    حالة XKB_کلید_تھلے:
        filter->refcnt++;
        append_اشارہ_گر_move(events, filter);
        أرجع XKB_FILTER_CONSUME;
    حالة XKB_کلید_REPEATED:
        إذا (filter->action.ptr.flags & ACTION_REPEAT)
            append_اشارہ_گر_move(events, filter);
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
xkb_filter_اشارہ_گر_button_new(بنية xkb_server_state *state,
                              بنية xkb_events *events,
                              بنية xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن bool
xkb_filter_اشارہ_گر_button_func(بنية xkb_server_state *state,
                               بنية xkb_events *events,
                               بنية xkb_filter *filter,
                               ثابت بنية xkb_کلید *کلید,
                               تعداد xkb_کلید_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن فراغ
xkb_filter_اشارہ_گر_جندرا_button_new(بنية xkb_server_state *state,
                                   بنية xkb_events *events,
                                   بنية xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن bool
xkb_filter_اشارہ_گر_جندرا_button_func(بنية xkb_server_state *state,
                                    بنية xkb_events *events,
                                    بنية xkb_filter *filter,
                                    ثابت بنية xkb_کلید *کلید,
                                    تعداد xkb_کلید_direction direction);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن فراغ
xkb_filter_اشارہ_گر_سیٹ_default_button_new(بنية xkb_server_state *state,
                                          بنية xkb_events *events,
                                          بنية xkb_filter *filter);

/* Implemented with xkb_machine API because it requires struct xkb_machine */
ساكن bool
xkb_filter_اشارہ_گر_سیٹ_default_button_func(بنية xkb_server_state *state,
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
    إذا (filter->action.type == ACTION_TYPE_CTRL_سیٹ) {
        /* Set: save the specified controls that are *not* already enabled */
        filter->priv = (uint32_t) (
            (~state->ادھار.components.controls) & filter->action.ctrls.ctrls
        );
    } وإلا {
        /* Lock: save the specified controls that *are* already enabled */
        filter->priv = (uint32_t) (
            state->ادھار.components.controls & filter->action.ctrls.ctrls
        );
    }

    إذا (filter->action.type == ACTION_TYPE_CTRL_سیٹ ||
        !(filter->action.ctrls.flags & ACTION_جندرا_NO_جندرا)) {
        /* Enable the specified controls that are not already enabled */
        state->ادھار.components.controls |= filter->action.ctrls.ctrls;
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
    حالة XKB_کلید_تھلے:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلید_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    إذا (filter->action.type == ACTION_TYPE_CTRL_سیٹ ||
        !(filter->action.ctrls.flags & ACTION_جندرا_NO_UNLOCK)) {
        ثابت تعداد xkb_action_controls old = state->ادھار.components.controls;

        /*
         * Set: Disable specified controls that were *not* enabled at key press.
         * Lock: Disable specified controls that *were* enabled at key press.
         */
        state->ادھار.components.controls &= ~(تعداد xkb_action_controls) filter->priv;

        إذا ((old & CONTROL_STICKY_کلیدیں) &&
            !(state->ادھار.components.controls & CONTROL_STICKY_کلیدیں)) {
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
    تعداد xkb_state_component بدلیا = 0;
    ثابت xkb_mod_نقاب_t نقاب = redirect->affect_mods;

    /*
     * Reference state: find the last state update in the queue, otherwise
     * use the current state.
     */
    بنية xkb_event *event;
    بنية state_components آخری_components = state->ادھار.components;
    darray_foreach_الٹ(event, events->قطار) {
        إذا (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
            آخری_components = event->components.components;
            اكسر;
        }
    }

    إذا (نقاب) {
        بنية state_components new = آخری_components;
        new.ادھار_mods = (new.ادھار_mods & ~نقاب) | redirect->mods;
        new.latched_mods = (new.latched_mods & ~نقاب) | redirect->mods;
        new.locked_mods = (new.locked_mods & ~نقاب) | redirect->mods;
        new.mods = (new.mods & ~نقاب) | redirect->mods;

        بدلیا = get_state_component_changes(&آخری_components, &new);
        إذا (بدلیا) {
            darray_append(events->قطار, (بنية xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = new,
                    .بدلیا = بدلیا
                }
            });
            /*
             * Commit the temporary modifiers change before the key event
             * it was computed for: consumers must observe the key event
             * under the *redirected* modifiers.
             */
            darray_append(events->قطار, (بنية xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_چوکھٹا,
            });
        }
    }

    darray_append(events->قطار, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_کلید,
        .کلید = {
            .keycode = redirect->keycode,
            .direction = direction
        }
    });

    إذا (نقاب && بدلیا) {
        /* Restore state */
        darray_append(events->قطار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = آخری_components,
                .بدلیا = بدلیا
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
                               XKB_کلید_تھلے);
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

    إذا (direction == XKB_کلید_اتے) {
        append_redirect_کلید_events(state, events,
                                   &filter->action.redirect, XKB_کلید_اتے);
        filter->func = عدم;
        أرجع XKB_FILTER_CONSUME;
     } وإلا إذا (direction == XKB_کلید_تھلے) {
        ثابت اتحاد xkb_action *actions = عدم;
        ثابت xkb_action_گنتی_t گنتی =
            xkb_کلید_get_actions(&state->ادھار, کلید, &actions);

        لكل (xkb_action_گنتی_t a = 0; a < گنتی; a++) {
            إذا (actions[a].type == ACTION_TYPE_REDIRECT_کلید &&
                actions[a].redirect.keycode != filter->action.redirect.keycode) {
                    /* One action redirects to a different key: release first */
                    append_redirect_کلید_events(state, events,
                                               &filter->action.redirect,
                                               XKB_کلید_اتے);
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
} filter_action_funcs[ACTION_TYPE_عدد_ENTRIES] = {
    [ACTION_TYPE_MOD_سیٹ]     = { xkb_filter_mod_سیٹ_new,
                                  xkb_filter_mod_سیٹ_func },
    [ACTION_TYPE_MOD_LATCH]   = { xkb_filter_mod_latch_new,
                                  xkb_filter_mod_latch_func },
    [ACTION_TYPE_MOD_جندرا]    = { xkb_filter_mod_جندرا_new,
                                  xkb_filter_mod_جندرا_func },
    [ACTION_TYPE_گروہ_سیٹ]   = { xkb_filter_گروہ_سیٹ_new,
                                  xkb_filter_گروہ_سیٹ_func },
    [ACTION_TYPE_گروہ_LATCH] = { xkb_filter_گروہ_latch_new,
                                  xkb_filter_گروہ_latch_func },
    [ACTION_TYPE_گروہ_جندرا]  = { xkb_filter_گروہ_جندرا_new,
                                  xkb_filter_گروہ_جندرا_func },
    [ACTION_TYPE_PTR_MOVE]    = { xkb_filter_اشارہ_گر_move_new,
                                  xkb_filter_اشارہ_گر_move_func },
    [ACTION_TYPE_PTR_BUTTON]  = { xkb_filter_اشارہ_گر_button_new,
                                  xkb_filter_اشارہ_گر_button_func },
    [ACTION_TYPE_PTR_جندرا]    = { xkb_filter_اشارہ_گر_جندرا_button_new,
                                  xkb_filter_اشارہ_گر_جندرا_button_func },
    [ACTION_TYPE_PTR_DEFAULT] = { xkb_filter_اشارہ_گر_سیٹ_default_button_new,
                                  xkb_filter_اشارہ_گر_سیٹ_default_button_func },
    [ACTION_TYPE_TERMINATE]   = { xkb_filter_terminate_server_new,
                                  xkb_filter_terminate_server_func },
    [ACTION_TYPE_SWITCH_VT]   = { xkb_filter_switch_virtual_console_new,
                                  xkb_filter_switch_virtual_console_func },
    [ACTION_TYPE_CTRL_سیٹ]    = { xkb_filter_ctrls_new,
                                  xkb_filter_ctrls_func },
    [ACTION_TYPE_CTRL_جندرا]   = { xkb_filter_ctrls_new,
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
    إذا (consumed || direction == XKB_کلید_اتے)
        أرجع;

    /* No filter consumed this event, so proceed with the key actions */
    ثابت اتحاد xkb_action *actions = عدم;
    xkb_action_گنتی_t گنتی = xkb_کلید_get_actions(&state->ادھار, کلید,
                                                         &actions);

    لكل (بنية {
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
        لكل (xkb_action_گنتی_t k = 0; k < گنتی; k++) {
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
            filter->کلید = کلید;
            filter->action = actions[k];

            بدل (filter->action.type) {
            حالة ACTION_TYPE_MOD_سیٹ:
                إذا (state->ادھار.components.controls & CONTROL_STICKY_کلیدیں) {
                    /* Convert modifier set action to a latch */
                    filter->action.type = ACTION_TYPE_MOD_LATCH;
                    إذا (state->a11y_flags & XKB_A11_Y_STICKY_کلیدیں_LATCH_منزل_جندرا) {
                        filter->action.mods.flags |= ACTION_LATCH_منزل_جندرا;
                    }
                }
                /* fallthrough */
            حالة ACTION_TYPE_MOD_LATCH:
            حالة ACTION_TYPE_MOD_جندرا: {
                ساكن ثابت تعداد xkb_action_flags unlock_mods_on_press
                    = ACTION_جندرا_CLEAR     /* SetMods & LatchMods: can unlock */
                    | ACTION_جندرا_NO_UNLOCK /* LockMods: cannot unlock */
                    | ACTION_EXPLICIT_UNLOCK_ON_PRESS;
                إذا ((state->flags & STATE_UNLOCK_MODS_ON_PRESS) &&
                    /*
                     * Ensure that the action can actually unlock and has
                     * no *explicit* `unlockOnPress` parameter.
                     */
                    (filter->action.mods.flags & unlock_mods_on_press) ==
                    (filter->action.type == ACTION_TYPE_MOD_جندرا
                        ? 0 : ACTION_جندرا_CLEAR))
                {
                    filter->action.mods.flags |= ACTION_UNLOCK_ON_PRESS;
                }
                اكسر;
            }
            حالة ACTION_TYPE_گروہ_سیٹ:
                إذا (state->ادھار.components.controls & CONTROL_STICKY_کلیدیں) {
                    /* Convert group set action to a latch */
                    filter->action.type = ACTION_TYPE_گروہ_LATCH;
                    إذا (state->a11y_flags & XKB_A11_Y_STICKY_کلیدیں_LATCH_منزل_جندرا) {
                        filter->action.گروہ.flags |= ACTION_LATCH_منزل_جندرا;
                    }
                }
                اكسر;
            حالة ACTION_TYPE_گروہ_جندرا:
                إذا ((filter->action.گروہ.flags &
                     ACTION_AUTO_گروہ_جندرا_ON_MODIFIER_RELEASE) &&
                    (state->flags &
                     STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_کلید) &&
                    /*
                     * FIXME: sticky keys may leave an unexpected modifier
                     * from the level 0 latched, so disable for now.
                     */
                    !(state->ادھار.components.controls & CONTROL_STICKY_کلیدیں))
                {
                    filter->action.گروہ.flags |= ACTION_جندرا_ON_RELEASE;
                    /* Add base level action */
                    اگلا.گنتی = xkb_keymap_کلید_get_actions_by_level(
                        state->ادھار.keymap, کلید, state->ادھار.components.گروہ,
                        0, &اگلا.actions
                    );
                    أكد(اگلا.گنتی == 1);
                }
                اكسر;
            حالة ACTION_TYPE_PTR_MOVE:
            حالة ACTION_TYPE_PTR_BUTTON:
            حالة ACTION_TYPE_PTR_جندرا:
            حالة ACTION_TYPE_PTR_DEFAULT:
                إذا (!(state->ادھار.components.controls & CONTROL_MOUSE_کلیدیں)) {
                    /* Convert pointer actions to NoAction */
                    أكد(!filter_action_funcs[ACTION_TYPE_NONE].new);
                    filter->func = عدم;
                    استمر;
                }
                اكسر;
            حالة ACTION_TYPE_REDIRECT_کلید:
                // FIXME: this is not efficient to resolve mods here each time
                filter->action.redirect.affect_mods = mod_نقاب_get_effective(
                    state->ادھار.keymap, filter->action.redirect.affect_mods
                );
                filter->action.redirect.mods = mod_نقاب_get_effective(
                    state->ادھار.keymap, filter->action.redirect.mods
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
               تعداد xkb_state_منوال_اندرونی منوال)
{
    state->منوال = منوال;
    state->refcnt = 1;
    state->keymap = xkb_keymap_ref(keymap);
    state->باہرلا_دا_range_گروہ.policy = XKB_LAYOUT_باہرلا_دا_RANGE_WRAP;

    /* Ensure that derived state is correctly initialized */
    xkb_state_update_derived(state);
}

ساكن مضمن فراغ
xkb_client_state_init(بنية xkb_client_state * محدود state,
                      بنية xkb_keymap * محدود keymap,
                      تعداد xkb_state_منوال_اندرونی منوال)
{
    xkb_state_init(&state->ادھار, keymap, منوال);
}

ساكن مضمن فراغ
xkb_server_state_init(بنية xkb_server_state * محدود state,
                      بنية xkb_keymap * محدود keymap,
                      تعداد xkb_state_منوال_اندرونی منوال,
                      تعداد xkb_a11y_flags a11y_affect,
                      تعداد xkb_a11y_flags a11y_flags)
{
    xkb_state_init(&state->ادھار, keymap, منوال);

    state->a11y_flags = a11y_flags;
    إذا (keymap->format != XKB_KEYMAP_FORMAT_متن_V1 &&
        !(a11y_affect & XKB_A11_Y_LATCH_اکو_ویلے_کلیدیں)) {
             /* Keymap v2+: enable extension to XKB if not manually disabled */
             state->a11y_flags |= XKB_A11_Y_LATCH_اکو_ویلے_کلیدیں;
    }
}

بنية xkb_state *
xkb_state_new(بنية xkb_keymap *keymap)
{
    بنية xkb_server_state * ثابت state = صفر_حافظہ_دیو(1, حجم(*state));
    إذا (!state) {
        log_err_func1(keymap->ctx, XKB_ERROR_ونڈ_FAILURE_2,
                      "Could not allocate a state object.\n");
        أرجع عدم;
    }

    xkb_server_state_init(state, keymap, LEGACY_MIXED_STATE, 0, 0);

    أرجع (بنية xkb_state *)state;
}

بنية xkb_state *
xkb_state_new_with_منوال(بنية xkb_keymap * محدود keymap,
                        تعداد xkb_state_منوال منوال,
                        تعداد xkb_حالت * محدود حالت)
{
    بدل (منوال) {
    حالة XKB_STATE_منوال_CLIENT:
    حالة XKB_STATE_منوال_SERVER_QUERY: {
        بنية xkb_client_state * ثابت state = صفر_حافظہ_دیو(1, حجم(*state));
        إذا (!state)
            اكسر;

        static_assert((بنانشانی)XKB_STATE_منوال_CLIENT ==
                      (بنانشانی)CLIENT_STATE, "");
        static_assert((بنانشانی)XKB_STATE_منوال_SERVER_QUERY ==
                      (بنانشانی)SERVER_COMPANION, "");
        xkb_client_state_init(state, keymap, (تعداد xkb_state_منوال_اندرونی)منوال);

        إذا (حالت)
            *حالت = XKB_SUCCESS;

        أرجع (بنية xkb_state *)state;
    }
    حالة XKB_STATE_منوال_SERVER: {
        بنية xkb_server_state * ثابت state = صفر_حافظہ_دیو(1, حجم(*state));
        إذا (!state)
            اكسر;

        xkb_server_state_init(state, keymap, LEGACY_SERVER_STATE, 0, 0);

        إذا (حالت)
            *حالت = XKB_SUCCESS;

        أرجع (بنية xkb_state *)state;
    }
    افتراضي:
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_STATE_منوال_2,
                     "Unsupported state mode: %u\n", منوال);
        إذا (حالت)
            *حالت = XKB_ERROR_UNSUPPORTED_STATE_منوال;
        أرجع عدم;
    }

    log_err_func1(keymap->ctx, XKB_ERROR_ونڈ_FAILURE_2,
                  "Could not allocate a state object.\n");
    إذا (حالت)
        *حالت = XKB_ERROR_ونڈ_FAILURE;
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

    إذا (state->منوال < LEGACY_MIXED_STATE) {
        static_assert(حجم(*state) == حجم(بنية xkb_client_state),
                      "Not a mere wrapper");
        /* Nothing else to reset */
    } وإلا {
        بنية xkb_server_state *state_ = (بنية xkb_server_state *)state;
        ثابت بنية xkb_server_state prev = *state_;
        memset(state_, 0, حجم(*state_));

        state_->ادھار = prev.ادھار;
        state_->a11y_flags = prev.a11y_flags;
        state_->filters = prev.filters;
        darray_ناپ(state_->filters) = 0;
    }
}

ساكن مضمن فراغ
xkb_state_destroy(بنية xkb_state *state)
{
    xkb_keymap_unref(state->keymap);
    إذا (state->منوال > SERVER_COMPANION)
        darray_free(((بنية xkb_server_state *)state)->filters);
}

فراغ
xkb_state_unref(بنية xkb_state *state)
{
    أكد(!state || state->refcnt > 0);
    إذا (!state || --state->refcnt > 0)
        أرجع;

    xkb_state_destroy(state);

    بدل (state->منوال) {
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
    xkb_led_اشاریہ_t idx;
    ثابت بنية xkb_led *led;

    state->components.leds = 0;

    xkb_leds_enumerate(idx, led, state->keymap) {

        إذا (led->which_mods != 0 && led->mods.نقاب != 0) {
            xkb_mod_نقاب_t mod_نقاب = 0;
            إذا (led->which_mods & XKB_STATE_MODS_EFFECTIVE)
                mod_نقاب |= state->components.mods;
            إذا (led->which_mods & XKB_STATE_MODS_DEPRESSED)
                mod_نقاب |= state->components.ادھار_mods;
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
                xkb_layout_نقاب_t گروہ_نقاب = 0;
                /* Effective and locked groups have been brought into range */
                أكد(state->components.گروہ < XKB_MAX_GROUPS);
                أكد(state->components.locked_گروہ >= 0 &&
                       state->components.locked_گروہ < XKB_MAX_GROUPS);
                /* Effective and locked groups are used as mask */
                إذا (led->which_groups & XKB_STATE_LAYOUT_EFFECTIVE)
                    گروہ_نقاب |= (UINT32_C(1) << state->components.گروہ);
                إذا (led->which_groups & XKB_STATE_LAYOUT_LOCKED)
                    گروہ_نقاب |= (UINT32_C(1) << state->components.locked_گروہ);
                /* Base and latched groups only have to be non-zero */
                إذا ((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                    state->components.ادھار_گروہ != 0)
                    گروہ_نقاب |= led->groups;
                إذا ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                    state->components.latched_گروہ != 0)
                    گروہ_نقاب |= led->groups;

                إذا (led->groups & گروہ_نقاب) {
                    state->components.leds |= (UINT32_C(1) << idx);
                    استمر;
                }
            } وإلا {
                /* Special case for Base and latched groups */
                إذا (((led->which_groups & XKB_STATE_LAYOUT_DEPRESSED) &&
                     state->components.ادھار_گروہ == 0) ||
                    ((led->which_groups & XKB_STATE_LAYOUT_LATCHED) &&
                     state->components.latched_گروہ == 0)) {
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
    xkb_layout_اشاریہ_t wrapped;

    state->components.mods = (state->components.ادھار_mods |
                              state->components.latched_mods |
                              state->components.locked_mods);

    /* Lock group must be adjusted, but not base nor latched groups */
    wrapped = Xkb_Wrap_گروہ_Into_Range(state->components.locked_گروہ,
                                    state->keymap->عدد_groups,
                                    state->باہرلا_دا_range_گروہ.policy,
                                    state->باہرلا_دا_range_گروہ.redirect_گروہ);
    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->components.locked_گروہ =
        (int32_t) (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    /* Effective group must be adjusted */
    wrapped = Xkb_Wrap_گروہ_Into_Range(state->components.ادھار_گروہ +
                                    state->components.latched_گروہ +
                                    state->components.locked_گروہ,
                                    state->keymap->عدد_groups,
                                    state->باہرلا_دا_range_گروہ.policy,
                                    state->باہرلا_دا_range_گروہ.redirect_گروہ);
    state->components.گروہ =
        (wrapped == XKB_LAYOUT_INVALID ? 0 : wrapped);

    xkb_state_led_update_all(state);
}

ساكن فراغ
state_update_enabled_controls(بنية xkb_server_state *state,
                              تعداد xkb_keyboard_control_flags affect_controls,
                              تعداد xkb_keyboard_control_flags controls,
                              بنية xkb_events *events)
{
    ثابت bool had_sticky_کلیدیں = state->ادھار.components.controls
                               & CONTROL_STICKY_کلیدیں;

    /*
     * Enable using the public API with the all the Control values, except
     * the internal ones, if any.
     */
    affect_controls =
        ( affect_controls
        & (تعداد xkb_keyboard_control_flags)CONTROL_ALL_BOOLEAN );
    state->ادھار.components.controls &=
        (تعداد xkb_action_controls)~affect_controls;
    state->ادھار.components.controls |=
        (تعداد xkb_action_controls)(controls & affect_controls);

    إذا (had_sticky_کلیدیں &&
        !(state->ادھار.components.controls & CONTROL_STICKY_کلیدیں)) {
        /* Sticky keys were disabled: clear all locks and latches */
        clear_all_latches_and_locks(state, events);
    }

    xkb_state_update_derived(&state->ادھار);
}

/**
 * Given a particular key event, updates the state structure to reflect the
 * new modifiers.
 */
تعداد xkb_state_component
xkb_state_update_کلید(بنية xkb_state *ادھار_state, xkb_keycode_t kc,
                     تعداد xkb_کلید_direction direction)
{
    /* Guard against client-only state */
    أكد(ادھار_state->منوال > SERVER_COMPANION);
    إذا (ادھار_state->منوال <= SERVER_COMPANION) {
        log_err(ادھار_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_منوال,
                "%s: Unexpected state type %d\n", __func__, ادھار_state->منوال);
        أرجع 0;
    }

    بنية xkb_server_state * ثابت state =
        (بنية xkb_server_state *)ادھار_state;

    ثابت بنية xkb_کلید* ثابت کلید = Xkb_کلید(state->ادھار.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    إذا (!کلید || (direction == XKB_کلید_REPEATED && !کلید->repeats))
        أرجع 0;

    إذا (direction == XKB_کلید_تھلے &&
        (state->ادھار.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیدیں) &&
        (state->a11y_flags & XKB_A11_Y_STICKY_کلیدیں_NO_اکو_ویلے_کلیدیں) &&
        state->ادھار.components.ادھار_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیدیں,
                                      0, عدم);
    }

    ثابت بنية state_components prev_components = state->ادھار.components;

    state->سیٹ_mods = 0;
    state->clear_mods = 0;

    xkb_filter_apply_all(state, عدم, کلید, direction);

    xkb_mod_اشاریہ_t i;
    xkb_mod_نقاب_t ثنائی_ہندسہ;
    لكل (i = 0, ثنائی_ہندسہ = 1; state->سیٹ_mods; i++, ثنائی_ہندسہ <<= 1) {
        إذا (state->سیٹ_mods & ثنائی_ہندسہ) {
            state->mod_کلید_گنتی[i]++;
            state->ادھار.components.ادھار_mods |= ثنائی_ہندسہ;
            state->سیٹ_mods &= ~ثنائی_ہندسہ;
        }
    }

    لكل (i = 0, ثنائی_ہندسہ = 1; state->clear_mods; i++, ثنائی_ہندسہ <<= 1) {
        إذا (state->clear_mods & ثنائی_ہندسہ) {
            state->mod_کلید_گنتی[i]--;
            إذا (state->mod_کلید_گنتی[i] <= 0) {
                state->ادھار.components.ادھار_mods &= ~ثنائی_ہندسہ;
                state->mod_کلید_گنتی[i] = 0;
            }
            state->clear_mods &= ~ثنائی_ہندسہ;
        }
    }

    xkb_state_update_derived(&state->ادھار);

    أرجع get_state_component_changes(&prev_components, &state->ادھار.components);
}

/* We need fake keys for `update_latch_modifiers` and `update_latch_group`.
 * These keys must have at least one level in order to break latches. We need 2
 * keys with specific actions in order to update group/mod latches without
 * affecting each other. */
ساكن بنية xkb_کلید_type_اندراج synthetic_کلید_level_اندراج = { 0 };
ساكن بنية xkb_کلید_type synthetic_کلید_type = {
     .عدد_entries = 1,
     .عدد_پدھراں = 1,
     .entries = &synthetic_کلید_level_اندراج
};
ساكن ثابت بنية xkb_کلید synthetic_کلید = { 0 };

/* Transcription from xserver: XkbLatchModifiers */
ساكن فراغ
update_latch_modifiers(بنية xkb_server_state *state,
                       بنية xkb_events *events,
                       xkb_mod_نقاب_t نقاب, xkb_mod_نقاب_t latches)
{
    /* Clear affected latched modifiers */
    ثابت xkb_mod_نقاب_t clear = نقاب & ~latches;
    state->ادھار.components.latched_mods &= ~clear;

    /* Clear any pending latch to locks using ad hoc action:
     * only affect corresponding modifier latches and no group latch. */
    بنية xkb_level synthetic_کلید_level_break_mod_latch = {
        .عدد_syms = 0,
        .عدد_actions = 1,
        .اتلا = XKB_کلید_No_علامت,
        .s.sym = XKB_کلید_No_علامت,
        .a.action.اندرونی = {
            .type = ACTION_TYPE_اندرونی,
            .flags = اندرونی_BREAKS_MOD_LATCH,
            .clear_latched_mods = clear
        }
    };
    بنية xkb_گروہ synthetic_کلید_گروہ_break_mod_latch = {
        .type = &synthetic_کلید_type,
        .پدھراں = &synthetic_کلید_level_break_mod_latch
    };
    ثابت بنية xkb_کلید synthetic_کلید_break_mod_latch = {
        .عدد_groups = 1,
        .groups = &synthetic_کلید_گروہ_break_mod_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_کلید_break_mod_latch, XKB_کلید_تھلے);

    /* Finally set the latched mods by simulate tapping a key with the
     * corresponding action */
    ثابت بنية xkb_کلید* ثابت کلید = &synthetic_کلید;
    ثابت اتحاد xkb_action latch_mods = {
        .mods = {
            .type = ACTION_TYPE_MOD_LATCH,
            .mods = { .نقاب = نقاب & latches },
            .flags = 0,
        },
    };
    بنية xkb_filter* ثابت filter = xkb_filter_new(state);
    filter->کلید = کلید;
    filter->func = xkb_filter_mod_latch_func;
    filter->action = latch_mods;
    xkb_filter_mod_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_mod_latch_func(state, events, filter, کلید, XKB_کلید_اتے);
}

/* Transcription from xserver: XkbLatchGroup */
ساكن فراغ
update_latch_گروہ(بنية xkb_server_state *state,
                   بنية xkb_events *events, int32_t گروہ)
{
    /* Clear any pending latch to locks. */
    ساكن بنية xkb_level synthetic_کلید_level_break_گروہ_latch = {
        .عدد_syms = 0,
        .عدد_actions = 1,
        .اتلا = XKB_کلید_No_علامت,
        .s.sym = XKB_کلید_No_علامت,
        .a.action.اندرونی = {
            .type = ACTION_TYPE_اندرونی,
            .flags = اندرونی_BREAKS_گروہ_LATCH,
            .clear_latched_mods = 0
        }
    };
    ساكن بنية xkb_گروہ synthetic_کلید_گروہ_break_گروہ_latch = {
        .type = &synthetic_کلید_type,
        .پدھراں = &synthetic_کلید_level_break_گروہ_latch
    };
    ساكن ثابت بنية xkb_کلید synthetic_کلید_break_گروہ_latch = {
        .عدد_groups = 1,
        .groups = &synthetic_کلید_گروہ_break_گروہ_latch
    };
    xkb_filter_apply_all(state, events,
                         &synthetic_کلید_break_گروہ_latch, XKB_کلید_تھلے);

    /* Simulate tapping a key with a group latch action, but in isolation: i.e.
     * without affecting the other filters. */
    ثابت بنية xkb_کلید* ثابت کلید = &synthetic_کلید;
    ثابت اتحاد xkb_action latch_گروہ = {
        .گروہ = {
            .type = ACTION_TYPE_گروہ_LATCH,
            .flags = ACTION_ABSOLUTE_SWITCH,
            .گروہ = گروہ,
        },
    };
    بنية xkb_filter* ثابت filter = xkb_filter_new(state);
    filter->کلید = کلید;
    filter->func = xkb_filter_گروہ_latch_func;
    filter->action = latch_گروہ;
    xkb_filter_گروہ_latch_new(state, events, filter);
    /* We added the filter manually, so only fire “up” event */
    xkb_filter_گروہ_latch_func(state, events, filter, کلید, XKB_کلید_اتے);
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
        resolve_منزل_canonical_mods(state->ادھار.keymap, update->affect_locked_mods);
    إذا ((update->components & XKB_STATE_MODS_LOCKED) &&
        affect_locked_mods) {
        ثابت xkb_mod_نقاب_t locked_mods =
            resolve_منزل_canonical_mods(state->ادھار.keymap, update->locked_mods);
        state->ادھار.components.locked_mods &= ~affect_locked_mods;
        state->ادھار.components.locked_mods |= locked_mods & affect_locked_mods;
    }
    إذا (update->components & XKB_STATE_LAYOUT_LOCKED) {
        state->ادھار.components.locked_گروہ = update->locked_layout;
    }

    /* Update latches */
    ثابت xkb_mod_نقاب_t affect_latched_mods =
        resolve_منزل_canonical_mods(state->ادھار.keymap, update->affect_latched_mods);
    إذا ((update->components & XKB_STATE_MODS_LATCHED) &&
        affect_latched_mods) {
        ثابت xkb_mod_نقاب_t latched_mods =
            resolve_منزل_canonical_mods(state->ادھار.keymap, update->latched_mods);
        update_latch_modifiers(state, events, affect_latched_mods, latched_mods);
    }
    إذا (update->components & XKB_STATE_LAYOUT_LATCHED) {
        update_latch_گروہ(state, events, update->latched_layout);
    }
}

تعداد xkb_state_component
xkb_state_update_latched_locked(بنية xkb_state *ادھار_state,
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
    أكد(ادھار_state->منوال > SERVER_COMPANION);
    إذا (ادھار_state->منوال <= SERVER_COMPANION) {
        log_err(ادھار_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_منوال,
                "%s: Unexpected state type %d\n", __func__, ادھار_state->منوال);
        أرجع 0;
    }

    بنية xkb_server_state * ثابت state =
        (بنية xkb_server_state *)ادھار_state;

    ثابت بنية state_components پچھلا_components = state->ادھار.components;
    ثابت تعداد xkb_state_component components
        = ((affect_latched_mods || latched_mods) ? XKB_STATE_MODS_LATCHED : 0)
        | ((affect_locked_mods || locked_mods) ? XKB_STATE_MODS_LOCKED : 0)
        | (affect_latched_layout ? XKB_STATE_LAYOUT_LATCHED : 0)
        | (affect_locked_layout ? XKB_STATE_LAYOUT_LOCKED : 0);
    ثابت بنية xkb_state_components_update update = {
        .ناپ = حجم(update),
        .components = components,
        .affect_latched_mods = affect_latched_mods,
        .latched_mods = latched_mods,
        .latched_layout = latched_layout,
        .affect_locked_mods = affect_locked_mods,
        .locked_mods = locked_mods,
        .locked_layout = locked_layout
    };
    state_update_latched_locked(state, &update, عدم);
    xkb_state_update_derived(&state->ادھار);
    أرجع get_state_component_changes(&پچھلا_components,
                                       &state->ادھار.components);
}

ساكن مضمن فراغ
clear_all_latches_and_locks(بنية xkb_server_state *state,
                            بنية xkb_events *events)
{
    ساكن ثابت تعداد xkb_state_component components
        = XKB_STATE_MODS_LATCHED | XKB_STATE_MODS_LOCKED
        | XKB_STATE_LAYOUT_LATCHED | XKB_STATE_LAYOUT_LOCKED;
    ثابت بنية xkb_state_components_update update = {
        .ناپ = حجم(update),
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
    إذا (xkb_feature_supported(XKB_FEATURE_ENUM_LAYOUT_باہرلا_دا_RANGE_POLICY,
                              update->policy)) {
        إذا (update->policy == XKB_LAYOUT_باہرلا_دا_RANGE_REDIRECT) {
            إذا (update->redirect < state->ادھار.keymap->عدد_groups) {
                state->ادھار.باہرلا_دا_range_گروہ.redirect_گروہ =
                    update->redirect;
            } وإلا {
                log_err(state->ادھار.keymap->ctx,
                        XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                        "Layout policy: "
                        "unsupported layout index %"PRIu32" > %"PRIu32"\n",
                        update->redirect + 1, state->ادھار.keymap->عدد_groups);
                أرجع XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ;
            }
        }
        state->ادھار.باہرلا_دا_range_گروہ.policy = update->policy;
        أرجع XKB_SUCCESS;
    } وإلا {
        log_err(state->ادھار.keymap->ctx,
                XKB_ERROR_UNSUPPORTED_LAYOUT_باہرلا_دا_RANGE_POLICY_2,
                "Unsupported layout policy: %d\n", update->policy);
        أرجع XKB_ERROR_UNSUPPORTED_LAYOUT_باہرلا_دا_RANGE_POLICY;
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

#تعریف_کرو check_state_update_abi(ctx, update) \
    check_state_update_abi_(ctx, __func__, update)

تعداد xkb_حالت
xkb_state_update_synthetic(بنية xkb_state * ادھار_state,
                           ثابت بنية xkb_synthetic_update * update,
                           تعداد xkb_state_component *بدلیا)
{
    /* Guard against client-only state */
    إذا (ادھار_state->منوال <= SERVER_COMPANION) {
        log_err(ادھار_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_منوال,
                "%s: Unexpected state type %d\n", __func__, ادھار_state->منوال);
        أرجع XKB_ERROR_UNEXPECTED_STATE_منوال;
    }

    بنية xkb_server_state * ثابت state =
        (بنية xkb_server_state *)ادھار_state;

    /* Check ABI compatibility */
    تعداد xkb_حالت حالت = check_state_update_abi(state->ادھار.keymap->ctx,
                                                    update);
    إذا (حالت != XKB_SUCCESS)
        أرجع حالت;

    ثابت بنية state_components پچھلا_components = state->ادھار.components;

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

    xkb_state_update_derived(&state->ادھار);

    إذا (بدلیا) {
        *بدلیا = get_state_component_changes(&پچھلا_components,
                                               &state->ادھار.components);
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
xkb_state_update_نقاب(بنية xkb_state *ادھار_state,
                      xkb_mod_نقاب_t ادھار_mods,
                      xkb_mod_نقاب_t latched_mods,
                      xkb_mod_نقاب_t locked_mods,
                      xkb_layout_اشاریہ_t ادھار_گروہ,
                      xkb_layout_اشاریہ_t latched_گروہ,
                      xkb_layout_اشاریہ_t locked_گروہ)
{
    /* Guard against server-only and server companion state */
    أكد(ادھار_state->منوال == CLIENT_STATE ||
           ادھار_state->منوال == LEGACY_MIXED_STATE);
    إذا (ادھار_state->منوال != CLIENT_STATE &&
        ادھار_state->منوال != LEGACY_MIXED_STATE) {
        log_err(ادھار_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_منوال,
                "%s: Unexpected state type %d\n", __func__, ادھار_state->منوال);
        أرجع 0;
    }

    بنية xkb_client_state * ثابت state =
        (بنية xkb_client_state *)ادھار_state;

    ثابت بنية state_components prev_components = state->ادھار.components;

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
    state->ادھار.components.ادھار_mods =
        resolve_منزل_canonical_mods(state->ادھار.keymap, ادھار_mods);
    state->ادھار.components.latched_mods =
        resolve_منزل_canonical_mods(state->ادھار.keymap, latched_mods);
    state->ادھار.components.locked_mods =
        resolve_منزل_canonical_mods(state->ادھار.keymap, locked_mods);

    static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups don't fit");
    state->ادھار.components.ادھار_گروہ = (int32_t) ادھار_گروہ;
    state->ادھار.components.latched_گروہ = (int32_t) latched_گروہ;
    state->ادھار.components.locked_گروہ = (int32_t) locked_گروہ;

    xkb_state_update_derived(&state->ادھار);

    أرجع get_state_component_changes(&prev_components, &state->ادھار.components);
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Lock_Modifier
 */
ساكن bool
should_do_caps_transformation(بنية xkb_state *state, xkb_keycode_t kc)
{
    أرجع
        xkb_state_mod_اشاریہ_is_سرگرم(state, XKB_MOD_اشاریہ_CAPS,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_اشاریہ_is_consumed(state, kc, XKB_MOD_اشاریہ_CAPS) == 0;
}

/*
 * https://www.x.org/releases/current/doc/kbproto/xkbproto.html#Interpreting_the_Control_Modifier
 */
ساكن bool
should_do_ctrl_transformation(بنية xkb_state *state, xkb_keycode_t kc)
{
    أرجع
        xkb_state_mod_اشاریہ_is_سرگرم(state, XKB_MOD_اشاریہ_CTRL,
                                      XKB_STATE_MODS_EFFECTIVE) > 0 &&
        xkb_state_mod_اشاریہ_is_consumed(state, kc, XKB_MOD_اشاریہ_CTRL) == 0;
}

/**
 * Provides the symbols to use for the given key and state.  Returns the
 * number of symbols pointed to in syms_out.
 */
صحيح
xkb_state_کلید_get_syms(بنية xkb_state *state, xkb_keycode_t kc,
                       ثابت xkb_keysym_t **syms_باہرلا)
{
    ثابت xkb_layout_اشاریہ_t layout = xkb_state_کلید_get_layout(state, kc);
    إذا (layout == XKB_LAYOUT_INVALID)
        اذهب err;

    ثابت xkb_level_اشاریہ_t level = xkb_state_کلید_get_level(state, kc, layout);
    إذا (level == XKB_LEVEL_INVALID)
        اذهب err;

    ثابت بنية xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);

    إذا (!کلید)
        اذهب err;

    ثابت بنية xkb_level* ثابت leveli =
        xkb_keymap_کلید_get_level(state->keymap, کلید, layout, level);

    إذا (!leveli)
        اذهب err;

    ثابت xkb_keysym_گنتی_t عدد_syms = leveli->عدد_syms;
    إذا (عدد_syms == 0)
        اذهب err;

    إذا (should_do_caps_transformation(state, kc)) {
        /* Only simple capitalization rules: keysyms count is unchanged. */
        إذا (عدد_syms > 1) {
            *syms_باہرلا = (leveli->has_اتلا)
                      ? leveli->s.syms + عدد_syms
                      : leveli->s.syms;
        } وإلا {
            *syms_باہرلا = &leveli->اتلا;
        }
    } وإلا {
        *syms_باہرلا = (عدد_syms > 1)
                  ? leveli->s.syms
                  : &leveli->s.sym;
    }
    أرجع (صحيح) عدد_syms;

err:
    *syms_باہرلا = عدم;
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
xkb_state_کلید_get_one_sym(بنية xkb_state *state, xkb_keycode_t kc)
{
    ثابت xkb_keysym_t *syms = عدم;
    ثابت صحيح عدد_syms = xkb_state_کلید_get_syms(state, kc, &syms);

    إذا (عدد_syms != 1)
        أرجع XKB_کلید_No_علامت;
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
    ثابت xkb_layout_اشاریہ_t layout = xkb_state_کلید_get_layout(state, kc);
    ثابت xkb_layout_اشاریہ_t عدد_layouts =
        xkb_keymap_عدد_layouts_for_کلید(state->keymap, kc);
    xkb_level_اشاریہ_t level = xkb_state_کلید_get_level(state, kc, layout);
    إذا (layout == XKB_LAYOUT_INVALID || عدد_layouts == 0 ||
        level == XKB_LEVEL_INVALID)
        أرجع XKB_کلید_No_علامت;

    ثابت xkb_keysym_t *syms = عدم;
    صحيح nsyms = xkb_keymap_کلید_get_syms_by_level(state->keymap, kc,
                                                 layout, level, &syms);
    إذا (nsyms != 1)
        أرجع XKB_کلید_No_علامت;
    xkb_keysym_t sym = syms[0];

    إذا (should_do_ctrl_transformation(state, kc) && sym > 127u) {
        لكل (xkb_layout_اشاریہ_t i = 0; i < عدد_layouts; i++) {
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
        sym = xkb_keysym_منزل_اتلا(sym);
    }

    أرجع sym;
}

صحيح
xkb_state_کلید_get_utf8(بنية xkb_state *state, xkb_keycode_t kc,
                       حرف *بفر, ناپ_t ناپ)
{
    صحيح nsyms;
    ثابت xkb_keysym_t *syms = عدم;
    ثابت xkb_keysym_t sym = get_one_sym_for_string(state, kc);
    إذا (sym != XKB_کلید_No_علامت) {
        nsyms = 1; syms = &sym;
    }
    وإلا {
        nsyms = xkb_state_کلید_get_syms(state, kc, &syms);
    }

    /* Make sure not to truncate in the middle of a UTF-8 sequence. */
    صحيح وتھ = 0;
    حرف tmp[XKB_KEYSYM_UTF8_MAX_ناپ];
    لكل (صحيح i = 0; i < nsyms; i++) {
        صحيح ret = xkb_keysym_منزل_utf8(syms[i], tmp, حجم(tmp));
        إذا (ret <= 0)
            اذهب err_bad;

        ret--;
        إذا ((ناپ_t)وتھ + (ناپ_t)ret <= ناپ)
            حافظہ_نقل(بفر + وتھ, tmp, (ناپ_t)ret);
        وتھ += ret;
    }

    إذا ((ناپ_t)وتھ >= ناپ)
        اذهب err_trunc;
    بفر[وتھ] = '\0';

    إذا (!is_valid_utf8(بفر, (ناپ_t)وتھ))
        اذهب err_bad;

    إذا (وتھ == 1 && (بنانشانی صحيح) بفر[0] <= 127u &&
        should_do_ctrl_transformation(state, kc))
        بفر[0] = Xkb_منزل_Control(بفر[0]);

    أرجع وتھ;

err_trunc:
    إذا (ناپ > 0)
        بفر[ناپ - 1] = '\0';
    أرجع وتھ;

err_bad:
    إذا (ناپ > 0)
        بفر[0] = '\0';
    أرجع 0;
}

uint32_t
xkb_state_کلید_get_utf32(بنية xkb_state *state, xkb_keycode_t kc)
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
        ret |= components->ادھار_mods;
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
ساكن مضمن xkb_layout_اشاریہ_t
serialize_layout(ثابت بنية state_components *components,
                 تعداد xkb_state_component type)
{
    إذا (type & XKB_STATE_LAYOUT_EFFECTIVE)
        أرجع components->گروہ;

    int32_t ret = 0;
    إذا (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret += components->ادھار_گروہ;
    إذا (type & XKB_STATE_LAYOUT_LATCHED)
        ret += components->latched_گروہ;
    إذا (type & XKB_STATE_LAYOUT_LOCKED)
        ret += components->locked_گروہ;

    أرجع (xkb_layout_اشاریہ_t)ret;
}

xkb_layout_اشاریہ_t
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
    xkb_mod_اشاریہ_t i;
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
xkb_state_mod_اشاریہ_is_سرگرم(بنية xkb_state *state,
                              xkb_mod_اشاریہ_t idx,
                              تعداد xkb_state_component type)
{
    إذا (unlikely(idx >= xkb_keymap_عدد_mods(state->keymap)))
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
    XKB_STATE_میل_FLAGS = XKB_STATE_میل_ANY
                          | XKB_STATE_میل_ALL
                          | XKB_STATE_میل_NON_EXCLUSIVE
};

/**
 * Helper function for xkb_state_mod_indices_are_active and
 * xkb_state_mod_names_are_active.
 */
ساكن bool
میل_mod_masks(بنية xkb_state *state,
                تعداد xkb_state_component type,
                تعداد xkb_state_میل میل,
                xkb_mod_نقاب_t wanted)
{
    ثابت xkb_mod_نقاب_t سرگرم = xkb_state_serialize_mods(state, type);

    إذا (!(میل & XKB_STATE_میل_NON_EXCLUSIVE) && (سرگرم & ~wanted))
        أرجع false;

    إذا (میل & XKB_STATE_میل_ANY)
        أرجع سرگرم & wanted;

    /* Ensure all flags but XKB_STATE_MATCH_ALL are processed before */
    static_assert(XKB_STATE_میل_ALL ==
                  ((تعداد xkb_state_میل) XKB_STATE_میل_FLAGS &
                   ~(XKB_STATE_میل_ANY | XKB_STATE_میل_NON_EXCLUSIVE)),
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
                                 تعداد xkb_state_میل میل,
                                 ...)
{
    إذا (میل & ~((تعداد xkb_state_میل) XKB_STATE_میل_FLAGS))
        أرجع -2;

    va_list ap;
    xkb_mod_نقاب_t wanted = 0;
    صحيح ret = 0;
    ثابت xkb_mod_اشاریہ_t عدد_mods = xkb_keymap_عدد_mods(state->keymap);

    va_شروع(ap, میل);
    طالما (1) {
        xkb_mod_اشاریہ_t idx = va_arg(ap, xkb_mod_اشاریہ_t);
        إذا (idx == XKB_MOD_INVALID)
            اكسر;
        إذا (unlikely(idx >= عدد_mods)) {
            ret = -1;
            اكسر;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_سرا(ap);

    إذا (ret == -1)
        أرجع ret;

    إذا (!wanted) {
        /* Modifiers not mapped */
        أرجع 0;
    }

    أرجع میل_mod_masks(state, type, میل, wanted);
}

/**
 * Returns 1 if the given modifier is active with the specified type(s), 0 if
 * not, or -1 if the modifier is invalid.
 */
صحيح
xkb_state_mod_name_is_سرگرم(بنية xkb_state *state, ثابت حرف *name,
                             تعداد xkb_state_component type)
{
    ثابت xkb_mod_اشاریہ_t idx = xkb_keymap_mod_get_اشاریہ(state->keymap, name);

    إذا (idx == XKB_MOD_INVALID)
        أرجع -1;

    أرجع xkb_state_mod_اشاریہ_is_سرگرم(state, idx, type);
}

/**
 * Returns 1 if the modifiers are active with the specified type(s), 0 if
 * not, -1 if any of the modifiers are invalid or -2 if the match flag is
 * invalid.
 */
ATTR_NULL_SENTINEL صحيح
xkb_state_mod_ناں_are_سرگرم(بنية xkb_state *state,
                               تعداد xkb_state_component type,
                               تعداد xkb_state_میل میل,
                               ...)
{
    إذا (میل & ~((تعداد xkb_state_میل) XKB_STATE_میل_FLAGS))
        أرجع -2;

    va_list ap;
    xkb_mod_نقاب_t wanted = 0;
    صحيح ret = 0;

    va_شروع(ap, میل);
    طالما (1) {
        ثابت حرف *str = va_arg(ap, ثابت حرف *);
        إذا (str == عدم)
            اكسر;
        ثابت xkb_mod_اشاریہ_t idx = xkb_keymap_mod_get_اشاریہ(state->keymap, str);
        إذا (idx == XKB_MOD_INVALID) {
            ret = -1;
            اكسر;
        }
        wanted |= state->keymap->mods.mods[idx].mapping;
    }
    va_سرا(ap);

    إذا (ret == -1)
        أرجع ret;

    إذا (!wanted) {
        /* Modifiers not mapped */
        أرجع 0;
    }

    أرجع میل_mod_masks(state, type, میل, wanted);
}

/**
 * Returns 1 if the given group is active with the specified type(s), 0 if
 * not, or -1 if the group is invalid.
 */
صحيح
xkb_state_layout_اشاریہ_is_سرگرم(بنية xkb_state *state,
                                 xkb_layout_اشاریہ_t idx,
                                 تعداد xkb_state_component type)
{
    إذا (idx >= state->keymap->عدد_groups)
        أرجع -1;

    صحيح ret = 0;
    إذا (type & XKB_STATE_LAYOUT_EFFECTIVE)
        ret |= (state->components.گروہ == idx);
    إذا (type & XKB_STATE_LAYOUT_DEPRESSED)
        ret |= (state->components.ادھار_گروہ == (int32_t) idx);
    إذا (type & XKB_STATE_LAYOUT_LATCHED)
        ret |= (state->components.latched_گروہ == (int32_t) idx);
    إذا (type & XKB_STATE_LAYOUT_LOCKED)
        ret |= (state->components.locked_گروہ == (int32_t) idx);

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
    ثابت xkb_layout_اشاریہ_t idx =
        xkb_keymap_layout_get_اشاریہ(state->keymap, name);

    إذا (idx == XKB_LAYOUT_INVALID)
        أرجع -1;

    أرجع xkb_state_layout_اشاریہ_is_سرگرم(state, idx, type);
}

/**
 * Returns 1 if the given LED is active, 0 if not, or -1 if the LED is invalid.
 */
صحيح
xkb_state_led_اشاریہ_is_سرگرم(بنية xkb_state *state, xkb_led_اشاریہ_t idx)
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
xkb_state_led_name_is_سرگرم(بنية xkb_state *state, ثابت حرف *name)
{
    ثابت xkb_led_اشاریہ_t idx = xkb_keymap_led_get_اشاریہ(state->keymap, name);

    إذا (idx == XKB_LED_INVALID)
        أرجع -1;

    أرجع xkb_state_led_اشاریہ_is_سرگرم(state, idx);
}

/**
 * See:
 * - XkbTranslateKeyCode(3), mod_rtrn return value, from libX11.
 * - MyEnhancedXkbTranslateKeyCode(), a modification of the above, from GTK+.
 */
ساكن xkb_mod_نقاب_t
کلید_get_consumed(بنية xkb_state *state, ثابت بنية xkb_کلید *کلید,
                 تعداد xkb_consumed_منوال منوال)
{
    ثابت xkb_layout_اشاریہ_t گروہ =
        xkb_state_کلید_get_layout(state, کلید->keycode);
    إذا (گروہ == XKB_LAYOUT_INVALID)
        أرجع 0;

    xkb_mod_نقاب_t preserve = 0;
    xkb_mod_نقاب_t consumed = 0;

    ثابت بنية xkb_کلید_type_اندراج* ثابت matching_اندراج =
        get_اندراج_for_کلید_state(state, کلید, گروہ);
    إذا (matching_اندراج)
        preserve = matching_اندراج->preserve.نقاب;

    ثابت بنية xkb_کلید_type* ثابت type = کلید->groups[گروہ].type;
    بدل (منوال) {
    حالة XKB_CONSUMED_منوال_XKB:
        consumed = type->mods.نقاب;
        اكسر;

    حالة XKB_CONSUMED_منوال_GTK: {
        ثابت بنية xkb_کلید_type_اندراج* ثابت no_mods_اندراج =
            get_اندراج_for_mods(type, 0);
        ثابت xkb_level_اشاریہ_t no_mods_leveli = no_mods_اندراج
                                               ? no_mods_اندراج->level
                                               : 0;
        ثابت بنية xkb_level* ثابت no_mods_level =
            &کلید->groups[گروہ].پدھراں[no_mods_leveli];

        لكل (darray_ناپ_t i = 0; i < type->عدد_entries; i++) {
            ثابت بنية xkb_کلید_type_اندراج* ثابت اندراج = &type->entries[i];
            إذا (!اندراج_is_سرگرم(اندراج))
                استمر;

            ثابت بنية xkb_level* ثابت level =
                &کلید->groups[گروہ].پدھراں[اندراج->level];
            إذا (Xkb_پدھراں_Same_Syms(level, no_mods_level))
                استمر;

            إذا (اندراج == matching_اندراج || one_ثنائی_ہندسہ_سیٹ(اندراج->mods.نقاب))
                consumed |= اندراج->mods.نقاب & ~اندراج->preserve.نقاب;
        }
        اكسر;
    }
    }

    أرجع consumed & ~preserve;
}

صحيح
xkb_state_mod_اشاریہ_is_consumed2(بنية xkb_state *state, xkb_keycode_t kc,
                                 xkb_mod_اشاریہ_t idx,
                                 تعداد xkb_consumed_منوال منوال)
{
    ثابت بنية xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);

    إذا (unlikely(!کلید || idx >= xkb_keymap_عدد_mods(state->keymap)))
        أرجع -1;

    ثابت xkb_mod_نقاب_t mapping = state->keymap->mods.mods[idx].mapping;
    إذا (!mapping) {
        /* Modifier not mapped */
        أرجع 0;
    }
    أرجع (mapping & کلید_get_consumed(state, کلید, منوال)) == mapping;
}

صحيح
xkb_state_mod_اشاریہ_is_consumed(بنية xkb_state *state, xkb_keycode_t kc,
                                xkb_mod_اشاریہ_t idx)
{
    أرجع xkb_state_mod_اشاریہ_is_consumed2(state, kc, idx,
                                            XKB_CONSUMED_منوال_XKB);
}

xkb_mod_نقاب_t
xkb_state_mod_نقاب_remove_consumed(بنية xkb_state *state, xkb_keycode_t kc,
                                   xkb_mod_نقاب_t نقاب)
{
    ثابت بنية xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);

    إذا (!کلید)
        أرجع 0;

    أرجع resolve_منزل_canonical_mods(state->keymap, نقاب) &
           ~کلید_get_consumed(state, کلید, XKB_CONSUMED_منوال_XKB);
}

xkb_mod_نقاب_t
xkb_state_کلید_get_consumed_mods2(بنية xkb_state *state, xkb_keycode_t kc,
                                 تعداد xkb_consumed_منوال منوال)
{
    بدل (منوال) {
    حالة XKB_CONSUMED_منوال_XKB:
    حالة XKB_CONSUMED_منوال_GTK:
        اكسر;
    افتراضي:
        log_err_func(state->keymap->ctx, XKB_LOG_سنیہا_NO_ID,
                     "unrecognized consumed modifiers mode: %d\n", منوال);
        أرجع 0;
    }

    ثابت بنية xkb_کلید* ثابت کلید = Xkb_کلید(state->keymap, kc);
    إذا (!کلید)
        أرجع 0;

    أرجع کلید_get_consumed(state, کلید, منوال);
}

xkb_mod_نقاب_t
xkb_state_کلید_get_consumed_mods(بنية xkb_state *state, xkb_keycode_t kc)
{
    أرجع xkb_state_کلید_get_consumed_mods2(state, kc, XKB_CONSUMED_منوال_XKB);
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

بنية xkb_shortcuts_config_اندراج {
    /** Real modifier mask to trigger shortcuts tweaks */
    xkb_mod_نقاب_t mods;
    /** Target layout */
    xkb_layout_اشاریہ_t نشانہ;
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
    بنية xkb_server_state ادھار;

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
        darray(بنية xkb_overlaid_کلید) کلیدیں;
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
    بنية machine_config {
        /** Modifiers remapping */
        بنية machine_modifiers_config {
            /** Real modifier mask */
            xkb_mod_نقاب_t نقاب;
            /** Modifiers re-mappings */
            darray_ناپ_t mappings_عدد;
            بنية machine_mods_mapping {
                xkb_mod_نقاب_t سروت;
                xkb_mod_نقاب_t نشانہ;
            } * mappings ATTR_COUNTED_BY(mappings_عدد);
        } modifiers;

        /** Shortcuts tweak */
        بنية machine_shortcuts_config {
            /** Substitution entries */
            بنية xkb_shortcuts_config_اندراج *entries;
        } shortcuts;
    } config;

    /** Mouse keys handling */
    بنية {
        /** Default pointer button */
        xkb_اشارہ_گر_button_اشاریہ_t default_button;
        /** Mask of the locked buttons */
        xkb_اشارہ_گر_button_نقاب_t locked_buttons;
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
        darray(بنية xkb_shortcuts_config_اندراج) entries;
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
        .ناپ = حجم(default_config)
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
        & ~(تعداد xkb_machine_builder_flags)XKB_MACHINE_BUILDER_FLAGS_اقدار;
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
        & ~(تعداد xkb_machine_flags)XKB_MACHINE_FLAGS_اقدار;
    إذا (invalid_machine_flags) {
        log_err_func(keymap->ctx, XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS_,
                     "unrecognized state machine flags: 0x%x\n",
                     invalid_machine_flags);
        إذا (حالت)
            *حالت = XKB_ERROR_UNSUPPORTED_MACHINE_FLAGS;
        أرجع عدم;
    }

    بنية xkb_machine_builder * ثابت builder = صفر_حافظہ_دیو(1, حجم(*builder));
    إذا (!builder) {
        log_err_func1(keymap->ctx, XKB_ERROR_ونڈ_FAILURE_2,
                      "cannot allocate machine builder\n");
        إذا (حالت)
            *حالت = XKB_ERROR_ونڈ_FAILURE;
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
        ~(تعداد xkb_a11y_flags)XKB_A11_Y_FLAGS_اقدار;

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
    إذا (!update->سروت) {
        إذا (!update->نشانہ) {
            /* Reset mappings */
            darray_resize(builder->mods, 0);
            أرجع XKB_SUCCESS;
        } وإلا {
            أرجع XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب;
        }
    }

    /* Check the modifiers against the keymap */
    ثابت xkb_mod_نقاب_t invalid = ~builder->keymap->canonical_state_نقاب;
    إذا ((update->سروت & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب,
                "%s: Invalid source modifiers: 0x%"PRIx32"\n",
                __func__, update->سروت);
        أرجع XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب;
    }
    إذا ((update->نشانہ & invalid)) {
        log_err(builder->keymap->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب,
                "%s: Invalid target modifiers: 0x%"PRIx32"\n",
                __func__, update->نشانہ);
        أرجع XKB_ERROR_UNSUPPORTED_MODIFIER_نقاب;
    }

    بنية machine_mods_mapping *mapping = عدم;
    darray_ناپ_t m = 0;
    darray_enumerate(m, mapping, builder->mods) {
        إذا (mapping->سروت == update->سروت) {
            إذا (!update->نشانہ) {
                /* Remove mapping */
                darray_remove(builder->mods, m);
            } وإلا {
                /* Update mapping */
                mapping->نشانہ = update->نشانہ;
            }
            أرجع XKB_SUCCESS;
        }
    }

    إذا (update->نشانہ) {
        /* Append new mapping */
        darray_append(
            builder->mods,
            (بنية machine_mods_mapping) {
                .سروت = update->سروت,
                .نشانہ = update->نشانہ
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
    إذا (update->سروت >= keymap->عدد_groups &&
        update->سروت != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                "%s: Invalid source layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->عدد_groups, update->سروت + 1);
        أرجع XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ;
    }
    إذا (update->نشانہ >= keymap->عدد_groups &&
        update->نشانہ != XKB_LAYOUT_INVALID) {
        static_assert(XKB_LAYOUT_INVALID == UINT32_MAX, "integer overflow");
        log_err(keymap->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                "%s: Invalid target layout: "
                "expected index in range 1..%"PRIu32", but got %"PRIu32"\n",
                __func__, keymap->عدد_groups, update->نشانہ + 1);
        أرجع XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ;
    }

    بنية xkb_shortcuts_config_options * ثابت config = &builder->shortcuts;

    /* Resize array & initialize new entries, if relevant */
    إذا (update->سروت >= darray_ناپ(config->entries) &&
        update->سروت != XKB_LAYOUT_INVALID) {
        إذا (update->نشانہ == update->سروت) {
            /* Skip default setting */
            أرجع XKB_SUCCESS;
        }
        xkb_layout_اشاریہ_t new = darray_ناپ(config->entries);
        darray_resize(config->entries, update->سروت + 1);
        لكل (; new <= update->سروت; new++) {
            darray_شے(config->entries, new) =
                (بنية xkb_shortcuts_config_اندراج) {
                    .نشانہ = XKB_LAYOUT_INVALID,
                    .mods = 0
                };
        }
    }

    بنية xkb_shortcuts_config_اندراج *اندراج;
    xkb_layout_اشاریہ_t سروت = (update->سروت == XKB_LAYOUT_INVALID)
        ? 0               /* Loop over all entries */
        : update->سروت; /* Single entry */
    darray_foreach_from(اندراج, config->entries, سروت) {
        إذا (update->سروت == XKB_LAYOUT_INVALID &&
            اندراج->نشانہ == XKB_LAYOUT_INVALID) {
            /* Skip previous deactivated entry */
            سروت++;
            استمر;
        }

        إذا (update->affect_mods) {
            اندراج->mods &= ~update->affect_mods;
            اندراج->mods |= (update->affect_mods & update->mods);
        }

        إذا (update->سروت != XKB_LAYOUT_INVALID) {
            اندراج->نشانہ = (update->نشانہ == سروت)
                ? XKB_LAYOUT_INVALID
                : update->نشانہ;
            /* Single entry: exit loop */
            اكسر;
        }
        سروت++;
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
    ثابت بنانشانی صحيح count1 = popcount32(m1);
    ثابت بنانشانی صحيح count2 = popcount32(m2);
    إذا (count1 > count2)
        أرجع -1;
    إذا (count1 < count2)
        أرجع +1;

    /* Give priority to real modifiers */
    أرجع (m1 < m2) ? -1 : +1;
}

ساكن تعداد xkb_حالت
machine_سیٹ_mods(بنية xkb_machine *sm,
                 ثابت machine_mods_mappings *raw_mappings)
{
    إذا (!darray_خالی_تھاواں(*raw_mappings)) {
        machine_mods_mappings mappings = darray_new();
        xkb_mod_نقاب_t نقاب = 0;
        ثابت بنية machine_mods_mapping *mapping;
        ثابت xkb_mod_نقاب_t invalid =
            ~sm->ادھار.ادھار.keymap->canonical_state_نقاب;
        darray_foreach(mapping, *raw_mappings) {
            إذا (!mapping->سروت || !mapping->نشانہ ||
                (mapping->سروت & invalid) || (mapping->نشانہ & invalid))
                استمر;
            darray_append(mappings, *mapping);
            نقاب |= mapping->سروت;
        }

        /*
         * Re-order the mappings, so that combinations have higher priority
         * if they are superset of other combinations.
         * For other type of overlaps, see the sorting function.
         */
        qsort(darray_اشیاء(mappings), darray_ناپ(mappings),
              حجم(*darray_اشیاء(mappings)), &cmp_mod_masks);

        darray_steal(
            mappings,
            &sm->config.modifiers.mappings,
            &sm->config.modifiers.mappings_عدد
        );
        sm->config.modifiers.نقاب = نقاب;
    } وإلا {
        sm->config.modifiers = (بنية machine_modifiers_config) {
            .mappings = عدم,
            .mappings_عدد = 0,
            .نقاب = 0
        };
    }

    أرجع XKB_SUCCESS;
}

ساكن تعداد xkb_حالت
machine_سیٹ_shortcuts(
    بنية xkb_machine * محدود sm,
    ثابت بنية xkb_shortcuts_config_options * محدود options
)
{
    إذا (darray_خالی_تھاواں(options->entries)) {
        sm->config.shortcuts = (بنية machine_shortcuts_config) {
            .entries = عدم
        };
        أرجع XKB_SUCCESS;
    }

    بنية xkb_keymap * ثابت keymap = sm->ادھار.ادھار.keymap;

    بنية xkb_shortcuts_config_اندراج * entries =
        صفر_حافظہ_دیو(keymap->عدد_groups, حجم(*entries));
    إذا (!entries)
        أرجع XKB_ERROR_ونڈ_FAILURE;

    ثابت xkb_layout_اشاریہ_t گنتی = MIN(
        keymap->عدد_groups,
        (xkb_layout_اشاریہ_t) darray_ناپ(options->entries)
    );

    bool some_entries = false;
    لكل (xkb_layout_اشاریہ_t l = 0; l < گنتی; l++) {
        /* Sanitize layout target */
        ثابت xkb_layout_اشاریہ_t نشانہ =
            darray_شے(options->entries, l).نشانہ;
        /* Sanitize modifier mask */
        ثابت xkb_mod_نقاب_t mods = (
            darray_شے(options->entries, l).mods &
            sm->ادھار.ادھار.keymap->canonical_state_نقاب
        );
        إذا (نشانہ < keymap->عدد_groups && نشانہ != l && mods) {
            entries[l] = (بنية xkb_shortcuts_config_اندراج) {
                .نشانہ = نشانہ,
                .mods = mods
            };
            some_entries = true;
        } وإلا {
            entries[l] = (بنية xkb_shortcuts_config_اندراج) {
                .نشانہ = XKB_LAYOUT_INVALID,
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
        لكل (xkb_layout_اشاریہ_t l = گنتی; l < keymap->عدد_groups; l++) {
            entries[l] = (بنية xkb_shortcuts_config_اندراج) {
                .نشانہ = XKB_LAYOUT_INVALID,
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
    بنية xkb_machine * ثابت machine = صفر_حافظہ_دیو(1, حجم(*machine));
    إذا (!machine) {
        log_err_func1(builder->keymap->ctx, XKB_ERROR_ونڈ_FAILURE_2,
                      "cannot allocate machine\n");
        إذا (حالت)
            *حالت = XKB_ERROR_ونڈ_FAILURE;
        أرجع عدم;
    }

    xkb_server_state_init(&machine->ادھار, builder->keymap, SERVER_STATE,
                          builder->controls.a11y.affect_flags,
                          builder->controls.a11y.flags);
    machine->flags = builder->machine_flags;
    إذا (!(machine->flags &
          XKB_MACHINE_DISABLE_LAYOUT_SWITCH_ON_MODIFIER_کلید_FIX))
    {
        machine->ادھار.flags |= STATE_FIX_LAYOUT_SWITCH_ON_MODIFIER_کلید;
    }
    إذا (machine->flags & XKB_MACHINE_UNLOCK_MODS_ON_PRESS) {
        machine->ادھار.flags |= STATE_UNLOCK_MODS_ON_PRESS;
    }

    تعداد xkb_حالت حالت_2;
    إذا ((حالت_2 = machine_سیٹ_mods(machine, &builder->mods)) != XKB_SUCCESS ||
        (حالت_2 = machine_سیٹ_shortcuts(machine, &builder->shortcuts)) !=
        XKB_SUCCESS) {
        *حالت = حالت_2;
        اذهب error;
    }

    darray_init(machine->overlays.کلیدیں);
    machine->mouse.default_button = XKB_اشارہ_گر_BUTTON_MIN;

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
    أكد(sm->ادھار.ادھار.refcnt > 0);
    sm->ادھار.ادھار.refcnt++;
    أرجع sm;
}

فراغ
xkb_machine_unref(بنية xkb_machine *sm)
{
    أكد(!sm || sm->ادھار.ادھار.refcnt > 0);
    إذا (!sm || --sm->ادھار.ادھار.refcnt > 0)
        أرجع;

    xkb_state_destroy(&sm->ادھار.ادھار);
    darray_free(sm->overlays.کلیدیں);
    حرر(sm->config.shortcuts.entries);
    حرر(sm->config.modifiers.mappings);
    حرر(sm);
}

بنية xkb_keymap *
xkb_machine_get_keymap(ثابت بنية xkb_machine *sm)
{
    /* Reference count is not updated. See API doc. */
    أرجع sm->ادھار.ادھار.keymap;
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
    بنية xkb_state * ثابت state = xkb_state_new_with_منوال(
        machine->ادھار.ادھار.keymap,
        XKB_STATE_منوال_SERVER_QUERY,
        &حالت_2
    );
    إذا (حالت_2 != XKB_SUCCESS) {
        إذا (حالت)
            *حالت = حالت_2;
        أرجع عدم;
    }

    أكد(state->منوال == SERVER_COMPANION);
    أكد(state->refcnt == 1);

    /* Copy machine’s state */
    *state = machine->ادھار.ادھار;

    /* Restore previous values */
    state->منوال = SERVER_COMPANION;
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
        overlays_from_controls(sm->ادھار.ادھار.components.controls);
    xkb_overlay_نقاب_t added = نقاب & ~sm->overlays.enabled;

    /* Remove overlays no longer enabled and keep relative order */
    uint32_t ترتیب = sm->overlays.ترتیب;
    ثابت xkb_overlay_اشاریہ_t overlay_max =
        format_max_overlays(sm->ادھار.ادھار.keymap->format);
    لكل (uint8_t n = 0; n < overlay_max;) {
        xkb_overlay_اشاریہ_t overlay_idx = (ترتیب >> (n * 4)) & 0xf;
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
            ترتیب = (ترتیب & سر) | ((ترتیب >> 4) & ~سر);
        }
    }

    /* Add new overlays in ascending order */
    لكل (xkb_overlay_اشاریہ_t k = 0; added; k++, added >>= 1) {
        إذا (added & 0x1) {
            ترتیب <<= 4;
            ترتیب |= k + 1u; /* k is 0-indexed */
        }
    }

    sm->overlays.ترتیب = ترتیب;
    sm->overlays.enabled = نقاب;
}

تعداد xkb_حالت
xkb_machine_process_synthetic(بنية xkb_machine *sm,
                              ثابت بنية xkb_synthetic_update *update,
                              بنية xkb_events *events)
{
    /* Check ABI compatibility */
    تعداد xkb_حالت حالت =
        check_state_update_abi(sm->ادھار.ادھار.keymap->ctx, update);
    إذا (حالت != XKB_SUCCESS)
        أرجع حالت;

    بنية xkb_server_state * ثابت state = &sm->ادھار;
    ثابت بنية state_components پچھلا_components = state->ادھار.components;

    // TODO: use a *transaction* mechanism: either the whole update succeeds
    //       or rollback

    darray_ناپ(events->قطار) = 0;
    events->اگلا = 0;

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

    xkb_state_update_derived(&state->ادھار);

    ثابت تعداد xkb_state_component بدلیا = get_state_component_changes(
        &پچھلا_components, &state->ادھار.components
    );
    إذا (بدلیا) {
        // TODO: latch controls
        إذا (بدلیا & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        /* Create event only if some component actually changed */
        darray_append(events->قطار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .بدلیا = بدلیا,
                .components = state->ادھار.components
            }
        });

        /* Terminate the frame */
        darray_append(events->قطار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_چوکھٹا,
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
    إذا (!(mappings->نقاب & state->components.mods))
        أرجع -1;

    ثابت xkb_layout_اشاریہ_t layout = state_کلید_get_layout(state, کلید);
    إذا (layout >= کلید->عدد_groups)
        أرجع -1;

    ثابت بنية xkb_کلید_type * ثابت type = کلید->groups[layout].type;
    xkb_mod_نقاب_t affect_mods = 0;
    xkb_mod_نقاب_t mods = 0;
    لكل (darray_ناپ_t m = 0; m < mappings->mappings_عدد; m++) {
        ثابت بنية machine_mods_mapping * ثابت mapping =
            &mappings->mappings[m];
        إذا (/* Skip not matching active mods */
            (mapping->سروت & state->components.mods) == mapping->سروت &&
            /* Skip overlap with previous active mappings */
            !(mapping->سروت & affect_mods) &&
            /* Skip if the key type uses some of the source modifiers */
            !(mapping->سروت & type->mods.نقاب)) {
            affect_mods |= mappings->mappings[m].سروت;
            mods |= mappings->mappings[m].نشانہ;
        }
    }

    إذا (!affect_mods)
        أرجع -1;

    بنية xkb_state new = *state;
    new.components.ادھار_mods = (new.components.ادھار_mods & ~affect_mods) | mods;
    new.components.latched_mods = (new.components.latched_mods & ~affect_mods);
    new.components.locked_mods = (new.components.locked_mods & ~affect_mods);
    xkb_state_update_derived(&new);

    ssize_t event_idx = -1;
    ثابت تعداد xkb_state_component بدلیا =
        get_state_component_changes(&state->components, &new.components);
    إذا (بدلیا) {
        event_idx = (ssize_t) darray_ناپ(events->قطار);
        darray_append(events->قطار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = new.components,
                .بدلیا = بدلیا
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
                   ثابت بنية state_components *پچھلا_components,
                   بنية xkb_events *events, ssize_t remap_event)
{
    إذا (!config->entries) {
        /* No shortcuts tweak */
        أرجع remap_event;
    }

    ثابت بنية xkb_shortcuts_config_اندراج * ثابت اندراج =
        &config->entries[state->components.گروہ];

    إذا (اندراج->نشانہ != XKB_LAYOUT_INVALID &&
        (اندراج->mods & state->components.mods)) {
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
            remap_event = (ssize_t) darray_ناپ(events->قطار);
            darray_append(events->قطار, (بنية xkb_event) {
                .ctx = events->ctx, /* borrowed from events */
                .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
                .components = {
                    .components = {0},
                    .بدلیا = 0
                }
            });
        } وإلا {
            /* Merge with last remap event */
            new.components =
                darray_شے(events->قطار, remap_event).components.components;
        }
        new.components.ادھار_گروہ
            = (int32_t) اندراج->نشانہ
            - state->components.latched_گروہ
            - state->components.locked_گروہ;
        xkb_state_update_derived(&new);

        ثابت تعداد xkb_state_component بدلیا =
            get_state_component_changes(پچھلا_components, &new.components);
        darray_شے(events->قطار, remap_event).components.بدلیا = بدلیا;
        darray_شے(events->قطار, remap_event).components.components =
            new.components;

        state->components.گروہ = new.components.گروہ;
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
    darray_foreach_الٹ(event, events->قطار) {
        إذا (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS)
            اكسر;
    }
    إذا (!event)
        أرجع;

    /* Restore state */
    ثابت تعداد xkb_state_component بدلیا =
        get_state_component_changes(initial_components,
                                    &event->components.components);
    إذا (بدلیا) {
        darray_append(events->قطار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = *initial_components,
                .بدلیا = بدلیا
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
    بنية xkb_overlaid_کلید *اندراج = عدم;
    بنية xkb_overlaid_کلید *دستیاب_اندراج = عدم;
    darray_foreach(اندراج, sm->overlays.کلیدیں) {
        إذا (اندراج->old == کلید) {
            /* Maintain the key overlaid */
            بدل (direction) {
            حالة XKB_کلید_تھلے:
                اندراج->refcnt++;
                اكسر;
            حالة XKB_کلید_REPEATED:
                اكسر;
            حالة XKB_کلید_اتے:
            افتراضي:
                اندراج->refcnt--;
                اكسر;
            }
            إذا (اندراج->refcnt <= 0)
                اندراج->old = عدم;
            أرجع اندراج->new;
        } وإلا إذا (!اندراج->old && !دستیاب_اندراج) {
            /* First free spot */
            دستیاب_اندراج = اندراج;
        }
    }
    إذا (direction == XKB_کلید_تھلے) {
        /* Enable key overlay */

        /*
         * Get overlay index: select the latest activated overlay
         * containing the key.
         */
        ثابت بنية xkb_کلید *new = کلید;

        إذا (کلید->overlays & sm->overlays.enabled) {
            /* Some relevant overlay is active */
            لكل (uint32_t ڈھیر = sm->overlays.ترتیب; ڈھیر; ڈھیر >>= 4) {
                static_assert(XKB_OVERLAY_گنتی == 8, "");
                ثابت xkb_overlay_اشاریہ_t overlay = (ڈھیر & 0xf) - 1;
                ثابت xkb_overlay_نقاب_t نقاب =
                    (xkb_overlay_نقاب_t)(1u << overlay);
                إذا (کلید->overlays & نقاب) {
                    /* Get new key */
                    إذا (کلید->overlays_inline) {
                        /* Stored inline */
                        new = کلید->overlay_کلید;
                    } وإلا {
                        /* Stored in sparse array */
                        ثابت xkb_overlay_نقاب_t کم = (xkb_overlay_نقاب_t)(
                            کلید->overlays & (xkb_overlay_نقاب_t)(نقاب - 1u)
                        );
                        /* index = number of set bits strictly below bit */
                        ثابت xkb_overlay_اشاریہ_t اشاریہ =
                            (xkb_overlay_اشاریہ_t)popcount32(کم);
                        new = کلید->overlays_کلیدیں[اشاریہ];
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
        إذا (دستیاب_اندراج) {
            /* Re-use a free entry */
            اندراج = دستیاب_اندراج;
        } وإلا {
            /* Append a new entry */
            ثابت darray_ناپ_t idx = darray_ناپ(sm->overlays.کلیدیں);
            darray_resize(sm->overlays.کلیدیں, idx + 1);
            اندراج = &darray_شے(sm->overlays.کلیدیں, idx);
        }

        /* Overlay the key */
        اندراج->old = کلید;
        اندراج->new = new;
        اندراج->refcnt = 1;
        أرجع new;
    }

    /* No overlay */
    أرجع کلید;
}

تعداد xkb_حالت
xkb_machine_process_کلید(بنية xkb_machine *sm,
                        xkb_keycode_t kc, تعداد xkb_کلید_direction direction,
                        بنية xkb_events *events)
{
    darray_ناپ(events->قطار) = 0;
    events->اگلا = 0;

    بنية xkb_server_state * ثابت state = &sm->ادھار;
    ثابت بنية xkb_کلید * کلید = Xkb_کلید(state->ادھار.keymap, kc);
    /* Ignore unknown key and repeat state for non-repeating key */
    إذا (!کلید || (direction == XKB_کلید_REPEATED && !کلید->repeats))
        أرجع XKB_SUCCESS;

    ثابت بنية state_components initial_components = state->ادھار.components;

    إذا (direction == XKB_کلید_تھلے &&
        (sm->ادھار.ادھار.components.controls & XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیدیں) &&
        (sm->ادھار.a11y_flags & XKB_A11_Y_STICKY_کلیدیں_NO_اکو_ویلے_کلیدیں) &&
        state->ادھار.components.ادھار_mods) {
        /* Deactivate sticky keys if a modifier was already held down*/
        state_update_enabled_controls(state,
                                      XKB_KEYBOARD_CONTROL_A11_Y_STICKY_کلیدیں,
                                      0, events);
    }

    إذا (کلید->overlays)
        کلید = process_overlayable_کلید(sm, کلید, direction);

    ssize_t remap_event = do_remap_modifiers(&sm->config.modifiers,
                                             &state->ادھار, events, کلید);

    remap_event = do_shortcuts_tweak(&sm->config.shortcuts, &state->ادھار,
                                     &initial_components, events, remap_event);

    إذا (remap_event >= 0) {
        // TODO: what about merging state event for RedirectKey?
        darray_append(events->قطار, (بنية xkb_event) {
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
    لكل (i = 0, ثنائی_ہندسہ = 1; state->سیٹ_mods; i++, ثنائی_ہندسہ <<= 1) {
        إذا (state->سیٹ_mods & ثنائی_ہندسہ) {
            state->mod_کلید_گنتی[i]++;
            state->ادھار.components.ادھار_mods |= ثنائی_ہندسہ;
            state->سیٹ_mods &= ~ثنائی_ہندسہ;
        }
    }

    لكل (i = 0, ثنائی_ہندسہ = 1; state->clear_mods; i++, ثنائی_ہندسہ <<= 1) {
        إذا (state->clear_mods & ثنائی_ہندسہ) {
            state->mod_کلید_گنتی[i]--;
            إذا (state->mod_کلید_گنتی[i] <= 0) {
                state->ادھار.components.ادھار_mods &= ~ثنائی_ہندسہ;
                state->mod_کلید_گنتی[i] = 0;
            }
            state->clear_mods &= ~ثنائی_ہندسہ;
        }
    }

    xkb_state_update_derived(&state->ادھار);

    /*
     * Insert key event only if actions did not override it, e.g.:
     * - RedirectKey() append its own key event;
     * - MovePointer() and PointerButton() generate pointer events instead
     *   of key events;
     * - TerminateServer() generate a Server event instead of key events.
     */
    إذا (state->flags & STATE_REQUIRE_کلید_EVENT) {
        darray_append(events->قطار, (بنية xkb_event) {
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
        undo_tweaks(&state->ادھار, &initial_components, events);
    }

    ثابت تعداد xkb_state_component بدلیا = get_state_component_changes(
        &initial_components, &state->ادھار.components
    );
    إذا (بدلیا) {
        إذا (بدلیا & XKB_STATE_CONTROLS_EFFECTIVE)
            machine_update_overlays(sm);

        // TODO: merge with previous state event, if any

        darray_append(events->قطار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_STATE_COMPONENTS,
            .components = {
                .components = state->ادھار.components,
                .بدلیا = بدلیا
            }
        });
    }

    إذا (!darray_خالی_تھاواں(events->قطار) &&
        darray_شے(events->قطار, darray_ناپ(events->قطار) - 1).type !=
        XKB_EVENT_TYPE_چوکھٹا)
    {
        darray_append(events->قطار, (بنية xkb_event) {
            .ctx = events->ctx, /* borrowed from events */
            .type = XKB_EVENT_TYPE_چوکھٹا,
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
        .ناپ = حجم(default_config)
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
        (config->flags & ~(uint32_t)XKB_EVENTS_FLAGS_اقدار);
    إذا (invalid_flags) {
        log_err_func(context, XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS_,
                     "unrecognized events collection flags: 0x%"PRIx32"\n",
                     invalid_flags);
        إذا (حالت)
            *حالت = XKB_ERROR_UNSUPPORTED_EVENTS_FLAGS;
        أرجع عدم;
    }

    بنية xkb_events *events = صفر_حافظہ_دیو(1, حجم(*events));
    إذا (!events) {
        log_err_func1(context, XKB_ERROR_ونڈ_FAILURE_2,
                      "cannot allocate state events collection\n");
        إذا (حالت)
            *حالت = XKB_ERROR_ونڈ_FAILURE;
        أرجع عدم;
    }

    /* Pre-allocate event queue */
    darray_init(events->قطار);
    ساكن ثابت darray_ناپ_t قطار_initial_ناپ = 16;
    darray_resize(events->قطار, قطار_initial_ناپ);
    // TODO: proper safe darray_resize() with return value
    إذا (!darray_اشیاء(events->قطار)) {
        log_err_func1(context, XKB_ERROR_ونڈ_FAILURE_2,
                      "cannot allocate state events queue\n");
        xkb_events_destroy(events);
        إذا (حالت)
            *حالت = XKB_ERROR_ونڈ_FAILURE;
        أرجع عدم;
    }
    darray_ناپ(events->قطار) = 0;

    events->ctx = xkb_context_ref(context);
    events->اگلا = 0;

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
xkb_events_اگلا(بنية xkb_events *events)
{
    إذا (events->اگلا < darray_ناپ(events->قطار)) {
        ثابت darray_ناپ_t اشاریہ = events->اگلا++;
        أرجع &darray_شے(events->قطار, اشاریہ);
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

    components->بدلیا = (uint32_t)event->components.بدلیا;
    components->depressed_mods = event->components.components.ادھار_mods;
    components->latched_mods = event->components.components.latched_mods;
    components->locked_mods = event->components.components.locked_mods;
    components->mods = event->components.components.mods;
    components->depressed_layout = event->components.components.ادھار_گروہ;
    components->latched_layout = event->components.components.latched_گروہ;
    components->locked_layout = event->components.components.locked_گروہ;
    components->layout = event->components.components.گروہ;
    components->leds = event->components.components.leds;
    components->controls = (uint32_t)event->components.components.controls;

    أرجع XKB_SUCCESS;
}

تعداد xkb_حالت
xkb_event_get_اشارہ_گر_motion(ثابت بنية xkb_event * محدود event,
                             بنية xkb_event_اشارہ_گر_motion * محدود motion)
{
    إذا (event->type != XKB_EVENT_TYPE_اشارہ_گر_MOTION)
        أرجع XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ثابت تعداد xkb_حالت حالت = xkb_check_state_abi(motion);
    إذا (حالت != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, حالت);
        أرجع حالت;
    }

    motion->flags = event->اشارہ_گر_motion.flags;
    motion->x = event->اشارہ_گر_motion.x;
    motion->y = event->اشارہ_گر_motion.y;

    أرجع XKB_SUCCESS;
}

تعداد xkb_حالت
xkb_event_get_اشارہ_گر_button(ثابت بنية xkb_event * محدود event,
                             بنية xkb_event_اشارہ_گر_button * محدود button)
{
    إذا (event->type != XKB_EVENT_TYPE_اشارہ_گر_BUTTON)
        أرجع XKB_ERROR_EVENT_TYPE_MISMATCH;

    /* Check ABI compatibility */
    ثابت تعداد xkb_حالت حالت = xkb_check_state_abi(button);
    إذا (حالت != XKB_SUCCESS) {
        xkb_log_abi_error(event->ctx, __func__, حالت);
        أرجع حالت;
    }

    button->button = event->اشارہ_گر_button.button;
    button->state = event->اشارہ_گر_button.state;
    button->گنتی = event->اشارہ_گر_button.گنتی;

    أرجع XKB_SUCCESS;
}

تعداد xkb_حالت
xkb_event_get_virtual_console(ثابت بنية xkb_event * محدود event,
                              int8_t * محدود اشاریہ_or_وتھ,
                              bool * محدود is_وتھ)
{
    إذا (event->type != XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE)
        أرجع XKB_ERROR_EVENT_TYPE_MISMATCH;

    *اشاریہ_or_وتھ = event->virtual_console.اشاریہ_or_وتھ;
    *is_وتھ = event->virtual_console.is_وتھ;

    أرجع XKB_SUCCESS;
}

تعداد xkb_حالت
xkb_state_update_event(بنية xkb_state * محدود ادھار_state,
                       ثابت بنية xkb_event * محدود event,
                       تعداد xkb_state_component * محدود بدلیا)
{
    /* Guard against server-only state */
    أكد(ادھار_state->منوال == SERVER_COMPANION ||
           ادھار_state->منوال == LEGACY_MIXED_STATE);
    إذا (ادھار_state->منوال != SERVER_COMPANION &&
        ادھار_state->منوال != LEGACY_MIXED_STATE) {
        log_err(ادھار_state->keymap->ctx, XKB_ERROR_UNEXPECTED_STATE_منوال_2,
                "%s: Unexpected state type %d\n", __func__, ادھار_state->منوال);
        أرجع XKB_ERROR_UNEXPECTED_STATE_منوال;
    }

    بنية xkb_client_state * ثابت state =
        (بنية xkb_client_state *)ادھار_state;

    إذا (event->type == XKB_EVENT_TYPE_STATE_COMPONENTS) {
        ثابت بنية state_components prev_components = state->ادھار.components;
        state->ادھار.components = event->components.components;
        /*
         * Recompute the changes instead of using the event value, because we do
         * not know if the event’s queue and the state are synced.
         */
        إذا (بدلیا)
            *بدلیا = get_state_component_changes(&prev_components,
                                                   &state->ادھار.components);
    } وإلا {
        إذا (بدلیا)
            *بدلیا = 0;
    }
    أرجع XKB_SUCCESS;
}

ساكن فراغ
append_اشارہ_گر_button(بنية xkb_events * محدود events,
                      بنية xkb_filter * محدود filter,
                      تعداد xkb_اشارہ_گر_button_state button_state,
                      uint8_t گنتی)
{
    darray_append(events->قطار, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_اشارہ_گر_BUTTON,
        .اشارہ_گر_button = {
            .ناپ = حجم(((بنية xkb_event*)0)->اشارہ_گر_button),
            .button = filter->priv,
            .state = button_state,
            .گنتی = گنتی,
        }
    });
}

ساكن فراغ
xkb_filter_اشارہ_گر_button_new(بنية xkb_server_state *state,
                              بنية xkb_events *events,
                              بنية xkb_filter *filter)
{
    إذا (!events || state->ادھار.منوال != SERVER_STATE ||
        !(state->ادھار.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_کلیدیں)) {
        filter->func = عدم;
        أرجع;
    }

    بنية xkb_machine * ثابت sm = (بنية xkb_machine *)state;

    /* Resolve button */
    ثابت xkb_اشارہ_گر_button_اشاریہ_t button =
        (filter->action.btn.button == XKB_اشارہ_گر_BUTTON_DEFAULT)
            ? sm->mouse.default_button
            : filter->action.btn.button;
    filter->priv = button;

    ثابت تعداد xkb_اشارہ_گر_button_state button_state =
        XKB_اشارہ_گر_BUTTON_PRESSED;
    append_اشارہ_گر_button(events, filter, button_state,
                          filter->action.btn.گنتی);
    state->flags &= ~STATE_REQUIRE_کلید_EVENT;
}

ساكن bool
xkb_filter_اشارہ_گر_button_func(بنية xkb_server_state *state,
                               بنية xkb_events *events,
                               بنية xkb_filter *filter,
                               ثابت بنية xkb_کلید *کلید,
                               تعداد xkb_کلید_direction direction)
{
    إذا (کلید != filter->کلید || !events)
        أرجع XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    بدل (direction) {
    حالة XKB_کلید_تھلے:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلید_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    إذا (!filter->action.btn.گنتی)
        append_اشارہ_گر_button(events, filter, XKB_اشارہ_گر_BUTTON_RELEASED, 0);

    filter->func = عدم;
    أرجع XKB_FILTER_CONSUME;
}

بنية xkb_filter_اشارہ_گر_جندرا_button_priv {
    uint32_t button:31;
#جے_تعریف_شدہ _WIN32
    uint32_t locked:1; // MSVC requires matching types to pack bitfields
#وإلا
    bool locked:1;
#جے_انت
};

تعداد {
    XKB_FILTER_اشارہ_گر_جندرا_BUTTON_PRIV_چوڑائی =
        حجم(بنية xkb_filter_اشارہ_گر_جندرا_button_priv) * CHAR_BIT,
    XKB_FILTER_اشارہ_گر_جندرا_BUTTON_PRIV_MAX =
        (UINT64_C(1) << XKB_FILTER_اشارہ_گر_جندرا_BUTTON_PRIV_چوڑائی) - 1
};

static_assert(حجم(بنية xkb_filter_اشارہ_گر_جندرا_button_priv) ==
              حجم(((بنية xkb_filter*)0)->priv),
              "");
static_assert((uint32_t)XKB_اشارہ_گر_BUTTON_MAX < (uint32_t)XKB_FILTER_اشارہ_گر_جندرا_BUTTON_PRIV_MAX,
              "xkb_filter_pointer_lock_button_priv cannot store button");

ساكن فراغ
xkb_filter_اشارہ_گر_جندرا_button_new(بنية xkb_server_state *state,
                                   بنية xkb_events *events,
                                   بنية xkb_filter *filter)
{
    إذا (!events || state->ادھار.منوال != SERVER_STATE ||
        !(state->ادھار.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_کلیدیں)) {
        filter->func = عدم;
        أرجع;
    }

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    بنية xkb_machine * ثابت sm = (بنية xkb_machine *)state;

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

    إذا (!locked && !(filter->action.btn.flags & ACTION_جندرا_NO_جندرا)) {
        sm->mouse.locked_buttons |= نقاب;
        filter->priv = button;
        append_اشارہ_گر_button(events, filter, XKB_اشارہ_گر_BUTTON_PRESSED, 0);
    }

    بنية xkb_filter_اشارہ_گر_جندرا_button_priv *priv =
        (بنية xkb_filter_اشارہ_گر_جندرا_button_priv*)&filter->priv;
    priv->button = button,
    priv->locked = locked;
}

ساكن bool
xkb_filter_اشارہ_گر_جندرا_button_func(بنية xkb_server_state *state,
                                    بنية xkb_events *events,
                                    بنية xkb_filter *filter,
                                    ثابت بنية xkb_کلید *کلید,
                                    تعداد xkb_کلید_direction direction)
{
    إذا (کلید != filter->کلید || !events || state->ادھار.منوال != SERVER_STATE)
        أرجع XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    بدل (direction) {
    حالة XKB_کلید_تھلے:
        filter->refcnt++;
        /* fallthrough */
    حالة XKB_کلید_REPEATED:
        أرجع XKB_FILTER_CONSUME;
    افتراضي:
        إذا (--filter->refcnt > 0)
            أرجع XKB_FILTER_CONSUME;
    }

    ثابت بنية xkb_filter_اشارہ_گر_جندرا_button_priv *priv =
        (بنية xkb_filter_اشارہ_گر_جندرا_button_priv*)&filter->priv;

    إذا (priv->locked && !(filter->action.btn.flags & ACTION_جندرا_NO_UNLOCK)) {
        بنية xkb_machine * ثابت sm = (بنية xkb_machine *)state;

        static_assert(XKB_اشارہ_گر_BUTTON_نقاب_چوڑائی <= 8,
                    "invalid UINTx_C() macro");
        static_assert(XKB_اشارہ_گر_BUTTON_MAX <= XKB_اشارہ_گر_BUTTON_نقاب_چوڑائی,
                    "invalid left shift");
        ثابت xkb_اشارہ_گر_button_نقاب_t نقاب = (UINT8_C(1) << priv->button);

        sm->mouse.locked_buttons &= ~نقاب;
        filter->priv = priv->button;
        append_اشارہ_گر_button(events, filter, XKB_اشارہ_گر_BUTTON_RELEASED, 0);
    }

    filter->func = عدم;
    أرجع XKB_FILTER_CONSUME;
}

ساكن فراغ
xkb_filter_اشارہ_گر_سیٹ_default_button_new(بنية xkb_server_state *state,
                                          بنية xkb_events *events,
                                          بنية xkb_filter *filter)
{
    إذا (!events || state->ادھار.منوال != SERVER_STATE ||
        !(state->ادھار.components.controls & XKB_KEYBOARD_CONTROL_MOUSE_کلیدیں)) {
        filter->func = عدم;
        أرجع;
    }

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    بنية xkb_machine * ثابت sm = (بنية xkb_machine *)state;

    static_assert(2 * XKB_اشارہ_گر_BUTTON_MAX <= ودھ_توں_ودھ_عدد, "lossy conversion");
    صحيح button = (filter->action.dflt.flags & ACTION_ABSOLUTE_SWITCH)
        ? filter->action.dflt.قدر
        : sm->mouse.default_button + filter->action.dflt.قدر;

    /* Wrap into range */
    ثابت صحيح rem = button % XKB_اشارہ_گر_BUTTON_MAX;
    sm->mouse.default_button = (rem >= 0)
        ? (xkb_اشارہ_گر_button_اشاریہ_t)rem
        : (xkb_اشارہ_گر_button_اشاریہ_t)(rem + XKB_اشارہ_گر_BUTTON_MAX);
}

ساكن bool
xkb_filter_اشارہ_گر_سیٹ_default_button_func(بنية xkb_server_state *state,
                                           بنية xkb_events *events,
                                           بنية xkb_filter *filter,
                                           ثابت بنية xkb_کلید *کلید,
                                           تعداد xkb_کلید_direction direction)
{
    إذا (کلید != filter->کلید || !events)
        أرجع XKB_FILTER_CONTINUE;

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    بدل (direction) {
    حالة XKB_کلید_تھلے:
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
    إذا (!events || state->ادھار.منوال != SERVER_STATE ||
        !(((بنية xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = عدم;
        أرجع;
    }

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    darray_append(events->قطار, (بنية xkb_event) {
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
    حالة XKB_کلید_تھلے:
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
    إذا (!events || state->ادھار.منوال != SERVER_STATE ||
        !(((بنية xkb_machine *)state)->flags & XKB_MACHINE_SERVER_ACTIONS)) {
        filter->func = عدم;
        أرجع;
    }

    state->flags &= ~STATE_REQUIRE_کلید_EVENT;

    darray_append(events->قطار, (بنية xkb_event) {
        .ctx = events->ctx, /* borrowed from events */
        .type = XKB_EVENT_TYPE_SWITCH_VIRTUAL_CONSOLE,
        .virtual_console = {
            .اشاریہ_or_وتھ = filter->action.screen.screen,
            .is_وتھ = !(filter->action.screen.flags & ACTION_ABSOLUTE_SWITCH)
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
    حالة XKB_کلید_تھلے:
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
