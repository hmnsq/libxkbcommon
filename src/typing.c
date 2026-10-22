/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <stdbool.h>
#ضمن <stdlib.h>
#ضمن <stdint.h>
#إذا WCHAR_MAX >= 0x7FFFFFFF
#ضمن <wctype.h>
#define USE_ISWPRINT
#إذا HAVE_NEWLOCALE && HAVE_ISWPRINT_L
#define USE_ISWPRINT_L
#ضمن <locale.h>
#endif
#وإلا
#ضمن <ctype.h>
#endif

#ضمن "xkbcommon/xkbcommon-status.h"
#ضمن "xkbcommon/xkbcommon.h"
#ضمن "darray.h"
#ضمن "state-priv.h"
#ضمن "typing.h"
#ضمن "util-bits.h"
#ضمن "util-mem.h"
#ضمن "utils.h"

تعداد xkb_status
xkb_مفتاح_set_init(بنية xkb_مفتاح_set *set,
                 بنية xkb_keymap *keymap,
                 ثابت بنية xkb_مفتاح_set_config *config)
{
    darray_init(set->printable);
    darray_init(set->modifiers);
    darray_init(set->misc);

    ساكن ثابت بنية xkb_مفتاح_set_config default_config = {0};
    إذا (!config)
        config = &default_config;
    /* keycode = 0 denotes “default” */
    ثابت xkb_keycode_t min = MAX(config->min, xkb_keymap_min_keycode(keymap));
    ثابت xkb_keycode_t max = config->max
        ? MIN(config->max, xkb_keymap_max_keycode(keymap))
        : xkb_keymap_max_keycode(keymap);

    تعداد xkb_status status;
    بنية xkb_state *state =
        xkb_state_new_with_mode(keymap, XKB_STATE_MODE_SERVER, &status);
    إذا (!state)
        أرجع status;

    #ifdef USE_ISWPRINT_L
    locale_t loc = newlocale(LC_ALL_MASK, "C.UTF-8", (locale_t)0);
    #endif

    بنية xkb_keymap_مفتاح_iterator iter;
    ثابت بنية xkb_keymap_مفتاح_iterator_config iter_config = {
        .size = حجم(iter_config),
        .flags = XKB_KEYMAP_مفتاح_ITERATOR_NO_FLAGS,
        .start = min
    };
    status = xkb_keymap_مفتاح_iterator_init(&iter, keymap, &iter_config);
    إذا (status != XKB_SUCCESS)
        اذهب error;

    ثابت xkb_mod_mask_t numlock =
        xkb_keymap_mod_get_mask(keymap, XKB_VMOD_NAME_رقم);

    ثابت بنية xkb_state_components_update components_update = {
        .size = حجم(components_update),
        .components = XKB_STATE_MODS_LOCKED,
        .affect_locked_mods = numlock,
        .locked_mods = numlock,
    };

    ثابت بنية xkb_synthetic_update update = {
        .size = حجم(update),
        .components = &components_update,
    };

    xkb_keycode_t k;
    طالما ((k = xkb_keymap_مفتاح_iterator_next(&iter)) != XKB_KEYCODE_INVALID) {
        إذا (k > max)
            اكسر;

        xkb_state_reset(state);
        status = xkb_state_update_synthetic(state, &update, عدم);
        إذا (status != XKB_SUCCESS)
            اذهب error;

        ثابت xkb_keysym_t *syms = عدم;
        ثابت صحيح عدد = xkb_state_مفتاح_get_syms(state, k, &syms);

        تعداد xkb_state_component changed =
            xkb_state_update_مفتاح(state, k, XKB_مفتاح_DOWN);
        xkb_mod_mask_t mods =
            xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);
        changed |= xkb_state_update_مفتاح(state, k, XKB_مفتاح_UP);
        mods |= xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);

        إذا (changed & XKB_STATE_MODS_EFFECTIVE) {
            /* Modifier key */
            تعداد xkb_modifier_kind kind;
            إذا (changed & XKB_STATE_MODS_LOCKED) {
                kind = XKB_MOD_LOCK;
            } وإلا إذا (changed & XKB_STATE_MODS_LATCHED) {
                kind = XKB_MOD_LATCH;
            } وإلا {
                kind = XKB_MOD_SET;
            }
            mods &= ~numlock;
            إذا (!mods)
                mods = numlock;
            darray_append(set->modifiers, (بنية xkb_modifier_مفتاح) {
                .keycode = k,
                .mods = mods,
                .kind = kind,
            });
            استمر;
        }

        إذا (عدد > 0) {
            uint32_t utf32 = 0;
            لكل (صحيح s = 0; s < عدد; s++) {
                utf32 = xkb_keysym_to_utf32(syms[s]);
                ثابت bool is_printable =
                    #ifdef USE_ISWPRINT_L
                    ((loc && iswprint_l((wint_t)utf32, loc)) || iswprint((wint_t)utf32))
                    #وإلاإذا defined(USE_ISWPRINT)
                    iswprint((wint_t)utf32)
                    #وإلا
                    utf32 <= 0x7f && isprint((صحيح)utf32)
                    #endif
                    ;
                إذا (is_printable)
                    اكسر;
                utf32 = 0;
            }
            إذا (utf32) {
                /* Printable keysyms */
                darray_append(set->printable, k);
                استمر;
            }
        }

        /* Miscellaneous key */
        darray_append(set->misc, k);
    }

error:
    #ifdef USE_ISWPRINT_L
    إذا (loc)
        freelocale(loc);
    #endif

    xkb_state_unref(state);
    إذا (status != XKB_SUCCESS)
        xkb_مفتاح_set_destroy(set);

    أرجع status;
}

فراغ
xkb_مفتاح_set_destroy(بنية xkb_مفتاح_set *set)
{
    إذا (!set)
        أرجع;

    darray_free(set->modifiers);
    darray_free(set->printable);
    darray_free(set->misc);
}

ساكن تعداد xkb_status
add_pending_events(بنية xkb_typing_state * restrict state,
                   ثابت بنية xkb_pending_typing_event * restrict events,
                   size_t عدد)
{
    لكل (size_t e = 0; e < عدد; e++) {
        /* Look for a free slot */
        بنية xkb_pending_typing_event *pending;
        darray_size_t idx = 0;
        darray_enumerate_from(idx, pending, state->pending, idx) {
            إذا (!pending->counter) {
                darray_item(state->pending, idx) = events[e];
                اذهب next;
            }
        }
        /* No free slot found: append new one */
        idx = darray_size(state->pending);
        darray_append(state->pending, events[e]);
    next:
        /* Avoid immediate consumption */
        darray_item(state->pending, idx).counter++;
    }
    state->رقم_pending += عدد;

    أرجع XKB_SUCCESS;
}

ساكن تعداد xkb_status
next_pending_events(بنية xkb_typing_state * restrict state,
                    xkb_typing_events * restrict out)
{
    /* Reset the output */
    darray_size(*out) = 0;

    darray_size_t idx;
    بنية xkb_pending_typing_event *pending;
    /* Stack: start from the end */
    darray_enumerate_reverse(idx, pending, state->pending) {
        إذا (!pending->counter)
            استمر;
        pending->counter--;
        إذا (pending->counter)
            استمر;
        darray_append(*out, pending->event);
        state->رقم_pending--;
        /* Resize array if it was the last element */
        إذا (idx + 1 == darray_size(state->pending))
            darray_size(state->pending)--;
        state->mods &= ~pending->mods;
    }

    أرجع XKB_SUCCESS;
}

تعداد {
    /** Tries to avoid pressing a key already pressed */
    مفتاح_CHOICE_MAX_TRIES = 12,
    XKB_ERROR_NO_RELEASED_مفتاح = -1000,
};

ساكن تعداد xkb_status
add_modifier_مفتاح(بنية xkb_typing_state * restrict state, size_t * restrict e)
{
    /* Choose a random key */
    long idx = -1;
    لكل (صحيح tries = 0; tries < مفتاح_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state)
            % darray_size(state->مفاتيح->modifiers);
        /*
         * Ensure key is not already down and its modifier mask is not active,
         * i.e. to avoid simultaneous keys with the same modifier.
         */
        إذا (!bit_array_get(state->مفاتيح_state,
                           darray_item(state->مفاتيح->modifiers, idx).keycode) &&
            !(state->mods & darray_item(state->مفاتيح->modifiers, idx).mods))
        {
            اكسر;
        }
        idx = -1;
    }

    بنية xkb_modifier_مفتاح مفتاح = {0};
    إذا (idx < 0) {
        /* No released key found: fall back to linear search */
        ثابت بنية xkb_modifier_مفتاح *مفتاح_2;
        darray_foreach(مفتاح_2, state->مفاتيح->modifiers) {
            /* See test doc above */
            إذا (!bit_array_get(state->مفاتيح_state, مفتاح_2->keycode) &&
                !(state->mods & مفتاح_2->mods))
            {
                مفتاح = *مفتاح_2;
                idx = 0;
                اكسر;
            }
        }
        إذا (idx < 0) {
            /* No fallback: skip */
            أرجع (تعداد xkb_status)XKB_ERROR_NO_RELEASED_مفتاح;
        }
    } وإلا {
        مفتاح = darray_item(state->مفاتيح->modifiers, idx);
    }

    ثابت size_t remaining_events =
        state->رقم_events - state->رقم_pending - *e;
    size_t pending_turns = 0;

    إذا (remaining_events >= 2) {
        إذا (مفتاح.kind == XKB_MOD_SET ||
            (مفتاح.kind == XKB_MOD_LOCK && remaining_events >= 4))
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
    } وإلا {
        أرجع XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (بنية xkb_typing_event) {
        .keycode = مفتاح.keycode,
        .direction = XKB_مفتاح_DOWN,
    };

    بنية xkb_pending_typing_event pending[2] = {
        {
            .event = {
                .keycode = مفتاح.keycode,
                .direction = XKB_مفتاح_DOWN,
            },
            .counter = (uint8_t)pending_turns,
        },
        {
            .event = {
                .keycode = مفتاح.keycode,
                .direction = XKB_مفتاح_UP,
            },
            .mods = مفتاح.mods,
            .counter = (uint8_t)pending_turns,
        },
    };

    إذا (pending_turns) {
        إذا (مفتاح.kind == XKB_MOD_LOCK) {
            /* Immediate release */
            state->events[(*e)++] = pending[1].event;
            /* Delayed unlock (press + release) */
            pending[1].counter++;
            ثابت تعداد xkb_status status =
                add_pending_events(state, pending, ARRAY_SIZE(pending));
            إذا (status != XKB_SUCCESS)
                أرجع status;
            /*
             * Register “locked” state, to avoid tapping the key before
             * delayed key released
             */
            bit_array_set(state->مفاتيح_state, مفتاح.keycode);
        } وإلا {
            /* Delayed release */
            ثابت تعداد xkb_status status =
                add_pending_events(state, &pending[1], 1);
            إذا (status != XKB_SUCCESS)
                أرجع status;
            /* Register pressed state */
            bit_array_set(state->مفاتيح_state, مفتاح.keycode);
        }
        state->mods |= مفتاح.mods;
    } وإلا {
        /* Immediate release */
        state->events[(*e)++] = pending[1].event;
        /* TODO: add latched modifier to mask state. */
    }

    أرجع XKB_SUCCESS;
}

ساكن تعداد xkb_status
add_other_مفتاح(بنية xkb_typing_state * restrict state,
              ثابت xkb_keycodes_t * restrict keycodes, size_t * restrict e,
              uint8_t immediate_release_prob, uint8_t max_pending_turns)
{
    /* Choose a random key */
    long idx = -1;
    لكل (صحيح tries = 0; tries < مفتاح_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state) % darray_size(*keycodes);
        /* Ensure key is not already down */
        إذا (!bit_array_get(state->مفاتيح_state,
                           darray_item(*keycodes, idx)))
        {
            اكسر;
        }
        idx = -1;
    }

    xkb_keycode_t keycode = XKB_KEYCODE_INVALID;
    إذا (idx < 0) {
        /* No released key found: fall back to linear search */
        ثابت xkb_keycode_t *keycode_;
        darray_foreach(keycode_, *keycodes) {
            إذا (!bit_array_get(state->مفاتيح_state, *keycode_)) {
                keycode = *keycode_;
                idx = 0;
                اكسر;
            }
        }
        إذا (idx < 0) {
            /* No fallback: skip */
            أرجع (تعداد xkb_status)XKB_ERROR_NO_RELEASED_مفتاح;
        }
    } وإلا {
        keycode = darray_item(*keycodes, idx);
    }

    ثابت size_t remaining_events =
        state->رقم_events - state->رقم_pending - *e;
    size_t pending_turns;

    إذا (remaining_events >= 2) {
        ثابت size_t max_turns = MIN(
            (size_t)max_pending_turns,
            remaining_events + state->رقم_pending
        );
        ثابت bool immediate_release = (
            (immediate_release_prob >= 100) ||
            !max_turns ||
            (state->prng(state->prng_state) % 100) < immediate_release_prob
        );
        إذا (immediate_release) {
            pending_turns = 0;
        } وإلا {
            pending_turns = 1 + (size_t)state->prng(state->prng_state)
                          % max_turns;
        }
    } وإلا {
        أرجع XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (بنية xkb_typing_event) {
        .keycode = keycode,
        .direction = XKB_مفتاح_DOWN,
    };

    ثابت بنية xkb_pending_typing_event pending = {
        .event = {
            .keycode = keycode,
            .direction = XKB_مفتاح_UP,
        },
        .counter = (uint8_t)pending_turns,
    };
    إذا (pending_turns) {
        ثابت تعداد xkb_status status = add_pending_events(state, &pending, 1);
        إذا (status != XKB_SUCCESS)
            أرجع status;
        /* Register pressed state */
        bit_array_set(state->مفاتيح_state, keycode);
    } وإلا {
        state->events[(*e)++] = pending.event;
    }

    أرجع XKB_SUCCESS;
}

ساكن تعداد xkb_مفتاح_kind
pick_مفتاح_kind(xkb_prng_t prng, فراغ *prng_state)
{
    ساكن ثابت uint8_t weights[رقم_XKB_مفتاح_KIND] = {
        [XKB_مفتاح_PRINTABLE] = 60,
        [XKB_مفتاح_MODIFIER] = 30,
        [XKB_مفتاح_MISC] = 10,
    };
    static_assert(XKB_مفتاح_MISC == 2 &&
                  XKB_مفتاح_MISC == رقم_XKB_مفتاح_KIND - 1, "");
    أكد(weights[0] + weights[1] + weights[2] == 100);

    long r = prng(prng_state) % 100;
    لكل (تعداد xkb_مفتاح_kind k = 0; k < رقم_XKB_مفتاح_KIND; k++)
        إذا ((r -= weights[k]) < 0)
            أرجع k;
    أرجع XKB_مفتاح_PRINTABLE;
}

ساكن فراغ
xkb_typing_state_destroy(بنية xkb_typing_state *state)
{
    حرر(state->مفاتيح_state);
    darray_free(state->pending);
    حرر(state->events);
}

تعداد xkb_status
xkb_typing_مدخل_init(بنية xkb_typing_مدخل * restrict typing,
                      ثابت بنية xkb_مفتاح_set * restrict مفاتيح,
                      ثابت بنية xkb_typing_مدخل_config * restrict config)
{
    typing->events = عدم;
    typing->رقم_events = 0;

    إذا (!config->مدخل_length)
        أرجع XKB_ERROR_INVALID;

    إذا (darray_empty(مفاتيح->printable) ||
        darray_empty(مفاتيح->modifiers) ||
        darray_empty(مفاتيح->misc))
    {
        أرجع XKB_ERROR_INVALID;
    }

    تعداد xkb_status status = XKB_SUCCESS;

    /* NOTE: arrays are sorted because they were built using the key iterator */
    xkb_keycode_t keycode_max = darray_last(مفاتيح->printable, 0);
    keycode_max = MAX(
        darray_last(مفاتيح->modifiers, (بنية xkb_modifier_مفتاح){0}).keycode,
        keycode_max
    );
    keycode_max = MAX(darray_last(مفاتيح->misc, 0), keycode_max);
    ثابت size_t مفاتيح_state_رقم = bit_array_كلمات((size_t)keycode_max + 1);
    uintptr_t *مفاتيح_state = calloc(مفاتيح_state_رقم, حجم(*مفاتيح_state));

    ثابت size_t رقم_events = config->مدخل_length;
    بنية xkb_typing_event *events = calloc(رقم_events, حجم(*events));

    بنية xkb_typing_state state = {
        .مفاتيح = مفاتيح,
        .مفاتيح_state = مفاتيح_state,
        .prng = config->prng,
        .prng_state = config->prng_state,
        .events = events,
        .رقم_events = رقم_events,
        .pending = darray_new(),
        .رقم_pending = 0,
    };

    xkb_typing_events pending = darray_new();

    إذا (!مفاتيح_state || !events) {
        status = XKB_ERROR_ALLOCATION_FAILURE;
        اذهب error;
    }

    /*
     * Invariant: e + state.num_pending <= state.num_events
     */

    لكل (size_t e = 0; e < رقم_events;) {
        /*
         * Process pending events
         */
        status = next_pending_events(&state, &pending);
        إذا (status != XKB_SUCCESS) {
            اذهب error;
        }
        بنية xkb_typing_event *event;
        darray_foreach(event, pending) {
            إذا (e >= رقم_events) {
                /* invariant violated: should be unreachable */
                PANIC_UNREACHABLE();
            }
            events[e] = *event;
            bit_array_assign(مفاتيح_state, event->keycode, (bool)event->direction);
            e++;
        }

        إذا (!state.رقم_pending && e == رقم_events - 1) {
            /*
             * Cannot add press/release *pair*, so just use an invalid keycode
             * for the last event.
             */
            events[e] = (بنية xkb_typing_event) {
                .keycode = XKB_KEYCODE_INVALID,
                .direction = XKB_مفتاح_DOWN,
            };
            اكسر;
        }

        إذا (e + 1 + state.رقم_pending >= رقم_events) {
            /* Cannot add new pair of events: require flushing pending events */
            استمر;
        }

        /*
         * Create new events
         */

        /* Choose kind of key */
        static_assert(XKB_مفتاح_PRINTABLE == 0, "");
        ثابت تعداد xkb_مفتاح_kind مفتاح_kind =
            pick_مفتاح_kind(config->prng, config->prng_state);
        بدل (مفتاح_kind) {
        حالة XKB_مفتاح_PRINTABLE:
            status = add_other_مفتاح(
                &state, &state.مفاتيح->printable, &e,
                XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS
            );
            اكسر;
        حالة XKB_مفتاح_MODIFIER:
            status = add_modifier_مفتاح(&state, &e);
            اكسر;
        حالة XKB_مفتاح_MISC:
            status = add_other_مفتاح(
                &state, &state.مفاتيح->misc, &e,
                XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS
            );
            اكسر;
        افتراضي: {
            static_assert(XKB_مفتاح_MISC == 2 &&
                          XKB_مفتاح_MISC == رقم_XKB_مفتاح_KIND - 1, "");
            status = XKB_ERROR_INVALID;
        }}

        إذا (status != XKB_SUCCESS &&
            status != (تعداد xkb_status)XKB_ERROR_NO_RELEASED_مفتاح)
        {
            اذهب error;
        }
    }

    أكد(!state.رقم_pending);

    typing->events = steal(&state.events);
    typing->رقم_events = رقم_events;

    status = XKB_SUCCESS;

error:
    darray_free(pending);
    xkb_typing_state_destroy(&state);

    أرجع status;
}

فراغ
xkb_typing_مدخل_destroy(بنية xkb_typing_مدخل *typing)
{
    إذا (!typing)
        أرجع;

    حرر(typing->events);
    typing->events = عدم;
}
