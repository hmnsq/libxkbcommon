/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل <assert.h>
#شامل <stdbool.h>
#شامل <stdlib.h>
#شامل <stdint.h>
#اگر WCHAR_MAX >= 0x7FFFFFFF
#شامل <wctype.h>
#define USE_ISWPRINT
#اگر HAVE_NEWLOCALE && HAVE_ISWPRINT_L
#define USE_ISWPRINT_L
#شامل <locale.h>
#endif
#وگرنه
#شامل <ctype.h>
#endif

#شامل "xkbcommon/xkbcommon-status.h"
#شامل "xkbcommon/xkbcommon.h"
#شامل "darray.h"
#شامل "state-priv.h"
#شامل "typing.h"
#شامل "util-bits.h"
#شامل "util-mem.h"
#شامل "utils.h"

شمارش xkb_status
xkb_کلید_set_init(ساختار xkb_کلید_set *set,
                 ساختار xkb_keymap *keymap,
                 ثابت ساختار xkb_کلید_set_config *config)
{
    darray_init(set->printable);
    darray_init(set->modifiers);
    darray_init(set->misc);

    ایستا ثابت ساختار xkb_کلید_set_config default_config = {0};
    اگر (!config)
        config = &default_config;
    /* keycode = 0 denotes “default” */
    ثابت xkb_keycode_t min = MAX(config->min, xkb_keymap_min_keycode(keymap));
    ثابت xkb_keycode_t max = config->max
        ? MIN(config->max, xkb_keymap_max_keycode(keymap))
        : xkb_keymap_max_keycode(keymap);

    شمارش xkb_status status;
    ساختار xkb_state *state =
        xkb_state_new_with_mode(keymap, XKB_STATE_MODE_SERVER, &status);
    اگر (!state)
        بازگشت status;

    #ifdef USE_ISWPRINT_L
    locale_t loc = newlocale(LC_ALL_MASK, "C.UTF-8", (locale_t)0);
    #endif

    ساختار xkb_keymap_کلید_iterator iter;
    ثابت ساختار xkb_keymap_کلید_iterator_config iter_config = {
        .size = اندازه(iter_config),
        .flags = XKB_KEYMAP_کلید_ITERATOR_NO_FLAGS,
        .start = min
    };
    status = xkb_keymap_کلید_iterator_init(&iter, keymap, &iter_config);
    اگر (status != XKB_SUCCESS)
        برو error;

    ثابت xkb_mod_mask_t numlock =
        xkb_keymap_mod_get_mask(keymap, XKB_VMOD_NAME_رقم);

    ثابت ساختار xkb_state_components_update components_update = {
        .size = اندازه(components_update),
        .components = XKB_STATE_MODS_LOCKED,
        .affect_locked_mods = numlock,
        .locked_mods = numlock,
    };

    ثابت ساختار xkb_synthetic_update update = {
        .size = اندازه(update),
        .components = &components_update,
    };

    xkb_keycode_t k;
    وقتی ((k = xkb_keymap_کلید_iterator_next(&iter)) != XKB_KEYCODE_INVALID) {
        اگر (k > max)
            بشکن;

        xkb_state_reset(state);
        status = xkb_state_update_synthetic(state, &update, پوچ);
        اگر (status != XKB_SUCCESS)
            برو error;

        ثابت xkb_keysym_t *syms = پوچ;
        ثابت صحیح شمار = xkb_state_کلید_get_syms(state, k, &syms);

        شمارش xkb_state_component changed =
            xkb_state_update_کلید(state, k, XKB_کلید_DOWN);
        xkb_mod_mask_t mods =
            xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);
        changed |= xkb_state_update_کلید(state, k, XKB_کلید_UP);
        mods |= xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);

        اگر (changed & XKB_STATE_MODS_EFFECTIVE) {
            /* Modifier key */
            شمارش xkb_modifier_kind kind;
            اگر (changed & XKB_STATE_MODS_LOCKED) {
                kind = XKB_MOD_LOCK;
            } وگرنه اگر (changed & XKB_STATE_MODS_LATCHED) {
                kind = XKB_MOD_LATCH;
            } وگرنه {
                kind = XKB_MOD_SET;
            }
            mods &= ~numlock;
            اگر (!mods)
                mods = numlock;
            darray_append(set->modifiers, (ساختار xkb_modifier_کلید) {
                .keycode = k,
                .mods = mods,
                .kind = kind,
            });
            ادامه;
        }

        اگر (شمار > 0) {
            uint32_t utf32 = 0;
            برای (صحیح s = 0; s < شمار; s++) {
                utf32 = xkb_keysym_to_utf32(syms[s]);
                ثابت bool is_printable =
                    #ifdef USE_ISWPRINT_L
                    ((loc && iswprint_l((wint_t)utf32, loc)) || iswprint((wint_t)utf32))
                    #وگرنهاگر defined(USE_ISWPRINT)
                    iswprint((wint_t)utf32)
                    #وگرنه
                    utf32 <= 0x7f && isprint((صحیح)utf32)
                    #endif
                    ;
                اگر (is_printable)
                    بشکن;
                utf32 = 0;
            }
            اگر (utf32) {
                /* Printable keysyms */
                darray_append(set->printable, k);
                ادامه;
            }
        }

        /* Miscellaneous key */
        darray_append(set->misc, k);
    }

error:
    #ifdef USE_ISWPRINT_L
    اگر (loc)
        freelocale(loc);
    #endif

    xkb_state_unref(state);
    اگر (status != XKB_SUCCESS)
        xkb_کلید_set_destroy(set);

    بازگشت status;
}

تهی
xkb_کلید_set_destroy(ساختار xkb_کلید_set *set)
{
    اگر (!set)
        بازگشت;

    darray_free(set->modifiers);
    darray_free(set->printable);
    darray_free(set->misc);
}

ایستا شمارش xkb_status
add_pending_events(ساختار xkb_typing_state * restrict state,
                   ثابت ساختار xkb_pending_typing_event * restrict events,
                   size_t شمار)
{
    برای (size_t e = 0; e < شمار; e++) {
        /* Look for a free slot */
        ساختار xkb_pending_typing_event *pending;
        darray_size_t idx = 0;
        darray_enumerate_from(idx, pending, state->pending, idx) {
            اگر (!pending->counter) {
                darray_item(state->pending, idx) = events[e];
                برو next;
            }
        }
        /* No free slot found: append new one */
        idx = darray_size(state->pending);
        darray_append(state->pending, events[e]);
    next:
        /* Avoid immediate consumption */
        darray_item(state->pending, idx).counter++;
    }
    state->رقم_pending += شمار;

    بازگشت XKB_SUCCESS;
}

ایستا شمارش xkb_status
next_pending_events(ساختار xkb_typing_state * restrict state,
                    xkb_typing_events * restrict out)
{
    /* Reset the output */
    darray_size(*out) = 0;

    darray_size_t idx;
    ساختار xkb_pending_typing_event *pending;
    /* Stack: start from the end */
    darray_enumerate_reverse(idx, pending, state->pending) {
        اگر (!pending->counter)
            ادامه;
        pending->counter--;
        اگر (pending->counter)
            ادامه;
        darray_append(*out, pending->event);
        state->رقم_pending--;
        /* Resize array if it was the last element */
        اگر (idx + 1 == darray_size(state->pending))
            darray_size(state->pending)--;
        state->mods &= ~pending->mods;
    }

    بازگشت XKB_SUCCESS;
}

شمارش {
    /** Tries to avoid pressing a key already pressed */
    کلید_CHOICE_MAX_TRIES = 12,
    XKB_ERROR_NO_RELEASED_کلید = -1000,
};

ایستا شمارش xkb_status
add_modifier_کلید(ساختار xkb_typing_state * restrict state, size_t * restrict e)
{
    /* Choose a random key */
    long idx = -1;
    برای (صحیح tries = 0; tries < کلید_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state)
            % darray_size(state->کلیدها->modifiers);
        /*
         * Ensure key is not already down and its modifier mask is not active,
         * i.e. to avoid simultaneous keys with the same modifier.
         */
        اگر (!bit_array_get(state->کلیدها_state,
                           darray_item(state->کلیدها->modifiers, idx).keycode) &&
            !(state->mods & darray_item(state->کلیدها->modifiers, idx).mods))
        {
            بشکن;
        }
        idx = -1;
    }

    ساختار xkb_modifier_کلید کلید = {0};
    اگر (idx < 0) {
        /* No released key found: fall back to linear search */
        ثابت ساختار xkb_modifier_کلید *کلید_2;
        darray_foreach(کلید_2, state->کلیدها->modifiers) {
            /* See test doc above */
            اگر (!bit_array_get(state->کلیدها_state, کلید_2->keycode) &&
                !(state->mods & کلید_2->mods))
            {
                کلید = *کلید_2;
                idx = 0;
                بشکن;
            }
        }
        اگر (idx < 0) {
            /* No fallback: skip */
            بازگشت (شمارش xkb_status)XKB_ERROR_NO_RELEASED_کلید;
        }
    } وگرنه {
        کلید = darray_item(state->کلیدها->modifiers, idx);
    }

    ثابت size_t remaining_events =
        state->رقم_events - state->رقم_pending - *e;
    size_t pending_turns = 0;

    اگر (remaining_events >= 2) {
        اگر (کلید.kind == XKB_MOD_SET ||
            (کلید.kind == XKB_MOD_LOCK && remaining_events >= 4))
        {
            /* Delay SetMods release or LockMods unlock */
            ثابت size_t max_turns = MIN(
                (size_t)(
                    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS -
                    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                 ),
                remaining_events + state->رقم_pending
            );
            pending_turns = XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                          + (size_t)state->prng(state->prng_state)
                          % (max_turns + 1);
        }
    } وگرنه {
        بازگشت XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (ساختار xkb_typing_event) {
        .keycode = کلید.keycode,
        .direction = XKB_کلید_DOWN,
    };

    ساختار xkb_pending_typing_event pending[2] = {
        {
            .event = {
                .keycode = کلید.keycode,
                .direction = XKB_کلید_DOWN,
            },
            .counter = (uint8_t)pending_turns,
        },
        {
            .event = {
                .keycode = کلید.keycode,
                .direction = XKB_کلید_UP,
            },
            .mods = کلید.mods,
            .counter = (uint8_t)pending_turns,
        },
    };

    اگر (pending_turns) {
        اگر (کلید.kind == XKB_MOD_LOCK) {
            /* Immediate release */
            state->events[(*e)++] = pending[1].event;
            /* Delayed unlock (press + release) */
            pending[1].counter++;
            ثابت شمارش xkb_status status =
                add_pending_events(state, pending, ARRAY_SIZE(pending));
            اگر (status != XKB_SUCCESS)
                بازگشت status;
            /*
             * Register “locked” state, to avoid tapping the key before
             * delayed key released
             */
            bit_array_set(state->کلیدها_state, کلید.keycode);
        } وگرنه {
            /* Delayed release */
            ثابت شمارش xkb_status status =
                add_pending_events(state, &pending[1], 1);
            اگر (status != XKB_SUCCESS)
                بازگشت status;
            /* Register pressed state */
            bit_array_set(state->کلیدها_state, کلید.keycode);
        }
        state->mods |= کلید.mods;
    } وگرنه {
        /* Immediate release */
        state->events[(*e)++] = pending[1].event;
        /* TODO: add latched modifier to mask state. */
    }

    بازگشت XKB_SUCCESS;
}

ایستا شمارش xkb_status
add_other_کلید(ساختار xkb_typing_state * restrict state,
              ثابت xkb_keycodes_t * restrict keycodes, size_t * restrict e,
              uint8_t immediate_release_prob, uint8_t max_pending_turns)
{
    /* Choose a random key */
    long idx = -1;
    برای (صحیح tries = 0; tries < کلید_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state) % darray_size(*keycodes);
        /* Ensure key is not already down */
        اگر (!bit_array_get(state->کلیدها_state,
                           darray_item(*keycodes, idx)))
        {
            بشکن;
        }
        idx = -1;
    }

    xkb_keycode_t keycode = XKB_KEYCODE_INVALID;
    اگر (idx < 0) {
        /* No released key found: fall back to linear search */
        ثابت xkb_keycode_t *keycode_;
        darray_foreach(keycode_, *keycodes) {
            اگر (!bit_array_get(state->کلیدها_state, *keycode_)) {
                keycode = *keycode_;
                idx = 0;
                بشکن;
            }
        }
        اگر (idx < 0) {
            /* No fallback: skip */
            بازگشت (شمارش xkb_status)XKB_ERROR_NO_RELEASED_کلید;
        }
    } وگرنه {
        keycode = darray_item(*keycodes, idx);
    }

    ثابت size_t remaining_events =
        state->رقم_events - state->رقم_pending - *e;
    size_t pending_turns;

    اگر (remaining_events >= 2) {
        ثابت size_t max_turns = MIN(
            (size_t)max_pending_turns,
            remaining_events + state->رقم_pending
        );
        ثابت bool immediate_release = (
            (immediate_release_prob >= 100) ||
            !max_turns ||
            (state->prng(state->prng_state) % 100) < immediate_release_prob
        );
        اگر (immediate_release) {
            pending_turns = 0;
        } وگرنه {
            pending_turns = 1 + (size_t)state->prng(state->prng_state)
                          % max_turns;
        }
    } وگرنه {
        بازگشت XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (ساختار xkb_typing_event) {
        .keycode = keycode,
        .direction = XKB_کلید_DOWN,
    };

    ثابت ساختار xkb_pending_typing_event pending = {
        .event = {
            .keycode = keycode,
            .direction = XKB_کلید_UP,
        },
        .counter = (uint8_t)pending_turns,
    };
    اگر (pending_turns) {
        ثابت شمارش xkb_status status = add_pending_events(state, &pending, 1);
        اگر (status != XKB_SUCCESS)
            بازگشت status;
        /* Register pressed state */
        bit_array_set(state->کلیدها_state, keycode);
    } وگرنه {
        state->events[(*e)++] = pending.event;
    }

    بازگشت XKB_SUCCESS;
}

ایستا شمارش xkb_کلید_kind
pick_کلید_kind(xkb_prng_t prng, تهی *prng_state)
{
    ایستا ثابت uint8_t weights[رقم_XKB_کلید_KIND] = {
        [XKB_کلید_PRINTABLE] = 60,
        [XKB_کلید_MODIFIER] = 30,
        [XKB_کلید_MISC] = 10,
    };
    static_assert(XKB_کلید_MISC == 2 &&
                  XKB_کلید_MISC == رقم_XKB_کلید_KIND - 1, "");
    أكد(weights[0] + weights[1] + weights[2] == 100);

    long r = prng(prng_state) % 100;
    برای (شمارش xkb_کلید_kind k = 0; k < رقم_XKB_کلید_KIND; k++)
        اگر ((r -= weights[k]) < 0)
            بازگشت k;
    بازگشت XKB_کلید_PRINTABLE;
}

ایستا تهی
xkb_typing_state_destroy(ساختار xkb_typing_state *state)
{
    آزاد(state->کلیدها_state);
    darray_free(state->pending);
    آزاد(state->events);
}

شمارش xkb_status
xkb_typing_درون_داد_init(ساختار xkb_typing_درون_داد * restrict typing,
                      ثابت ساختار xkb_کلید_set * restrict کلیدها,
                      ثابت ساختار xkb_typing_درون_داد_config * restrict config)
{
    typing->events = پوچ;
    typing->رقم_events = 0;

    اگر (!config->درون_داد_length)
        بازگشت XKB_ERROR_INVALID;

    اگر (darray_empty(کلیدها->printable) ||
        darray_empty(کلیدها->modifiers) ||
        darray_empty(کلیدها->misc))
    {
        بازگشت XKB_ERROR_INVALID;
    }

    شمارش xkb_status status = XKB_SUCCESS;

    /* NOTE: arrays are sorted because they were built using the key iterator */
    xkb_keycode_t keycode_max = darray_last(کلیدها->printable, 0);
    keycode_max = MAX(
        darray_last(کلیدها->modifiers, (ساختار xkb_modifier_کلید){0}).keycode,
        keycode_max
    );
    keycode_max = MAX(darray_last(کلیدها->misc, 0), keycode_max);
    ثابت size_t کلیدها_state_رقم = bit_array_واژگان((size_t)keycode_max + 1);
    uintptr_t *کلیدها_state = calloc(کلیدها_state_رقم, اندازه(*کلیدها_state));

    ثابت size_t رقم_events = config->درون_داد_length;
    ساختار xkb_typing_event *events = calloc(رقم_events, اندازه(*events));

    ساختار xkb_typing_state state = {
        .کلیدها = کلیدها,
        .کلیدها_state = کلیدها_state,
        .prng = config->prng,
        .prng_state = config->prng_state,
        .events = events,
        .رقم_events = رقم_events,
        .pending = darray_new(),
        .رقم_pending = 0,
    };

    xkb_typing_events pending = darray_new();

    اگر (!کلیدها_state || !events) {
        status = XKB_ERROR_ALLOCATION_FAILURE;
        برو error;
    }

    /*
     * Invariant: e + state.num_pending <= state.num_events
     */

    برای (size_t e = 0; e < رقم_events;) {
        /*
         * Process pending events
         */
        status = next_pending_events(&state, &pending);
        اگر (status != XKB_SUCCESS) {
            برو error;
        }
        ساختار xkb_typing_event *event;
        darray_foreach(event, pending) {
            اگر (e >= رقم_events) {
                /* invariant violated: should be unreachable */
                PANIC_UNREACHABLE();
            }
            events[e] = *event;
            bit_array_assign(کلیدها_state, event->keycode, (bool)event->direction);
            e++;
        }

        اگر (!state.رقم_pending && e == رقم_events - 1) {
            /*
             * Cannot add press/release *pair*, so just use an invalid keycode
             * for the last event.
             */
            events[e] = (ساختار xkb_typing_event) {
                .keycode = XKB_KEYCODE_INVALID,
                .direction = XKB_کلید_DOWN,
            };
            بشکن;
        }

        اگر (e + 1 + state.رقم_pending >= رقم_events) {
            /* Cannot add new pair of events: require flushing pending events */
            ادامه;
        }

        /*
         * Create new events
         */

        /* Choose kind of key */
        static_assert(XKB_کلید_PRINTABLE == 0, "");
        ثابت شمارش xkb_کلید_kind کلید_kind =
            pick_کلید_kind(config->prng, config->prng_state);
        انتخاب (کلید_kind) {
        حالت XKB_کلید_PRINTABLE:
            status = add_other_کلید(
                &state, &state.کلیدها->printable, &e,
                XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS
            );
            بشکن;
        حالت XKB_کلید_MODIFIER:
            status = add_modifier_کلید(&state, &e);
            بشکن;
        حالت XKB_کلید_MISC:
            status = add_other_کلید(
                &state, &state.کلیدها->misc, &e,
                XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS
            );
            بشکن;
        پیشفرض: {
            static_assert(XKB_کلید_MISC == 2 &&
                          XKB_کلید_MISC == رقم_XKB_کلید_KIND - 1, "");
            status = XKB_ERROR_INVALID;
        }}

        اگر (status != XKB_SUCCESS &&
            status != (شمارش xkb_status)XKB_ERROR_NO_RELEASED_کلید)
        {
            برو error;
        }
    }

    أكد(!state.رقم_pending);

    typing->events = steal(&state.events);
    typing->رقم_events = رقم_events;

    status = XKB_SUCCESS;

error:
    darray_free(pending);
    xkb_typing_state_destroy(&state);

    بازگشت status;
}

تهی
xkb_typing_درون_داد_destroy(ساختار xkb_typing_درون_داد *typing)
{
    اگر (!typing)
        بازگشت;

    آزاد(typing->events);
    typing->events = پوچ;
}
