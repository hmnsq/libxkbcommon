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
xkb_ئاچقۇچ_set_init(بنية xkb_ئاچقۇچ_set *set,
                 بنية xkb_keymap *keymap,
                 ثابت بنية xkb_ئاچقۇچ_set_config *config)
{
    darray_init(set->printable);
    darray_init(set->modifiers);
    darray_init(set->misc);

    ساكن ثابت بنية xkb_ئاچقۇچ_set_config default_config = {0};
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

    بنية xkb_keymap_ئاچقۇچ_iterator iter;
    ثابت بنية xkb_keymap_ئاچقۇچ_iterator_config iter_config = {
        .size = حجم(iter_config),
        .flags = XKB_KEYMAP_ئاچقۇچ_ITERATOR_NO_FLAGS,
        .start = min
    };
    status = xkb_keymap_ئاچقۇچ_iterator_init(&iter, keymap, &iter_config);
    إذا (status != XKB_SUCCESS)
        اذهب error;

    ثابت xkb_mod_mask_t numlock =
        xkb_keymap_mod_get_mask(keymap, XKB_VMOD_NAME_رەقەم);

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
    طالما ((k = xkb_keymap_ئاچقۇچ_iterator_next(&iter)) != XKB_KEYCODE_INVALID) {
        إذا (k > max)
            اكسر;

        xkb_state_reset(state);
        status = xkb_state_update_synthetic(state, &update, عدم);
        إذا (status != XKB_SUCCESS)
            اذهب error;

        ثابت xkb_keysym_t *syms = عدم;
        ثابت صحيح سانى = xkb_state_ئاچقۇچ_get_syms(state, k, &syms);

        تعداد xkb_state_component changed =
            xkb_state_update_ئاچقۇچ(state, k, XKB_ئاچقۇچ_DOWN);
        xkb_mod_mask_t mods =
            xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);
        changed |= xkb_state_update_ئاچقۇچ(state, k, XKB_ئاچقۇچ_UP);
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
            darray_append(set->modifiers, (بنية xkb_modifier_ئاچقۇچ) {
                .keycode = k,
                .mods = mods,
                .kind = kind,
            });
            استمر;
        }

        إذا (سانى > 0) {
            uint32_t utf32 = 0;
            لكل (صحيح s = 0; s < سانى; s++) {
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
        xkb_ئاچقۇچ_set_destroy(set);

    أرجع status;
}

فراغ
xkb_ئاچقۇچ_set_destroy(بنية xkb_ئاچقۇچ_set *set)
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
                   size_t سانى)
{
    لكل (size_t e = 0; e < سانى; e++) {
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
    state->رەقەم_pending += سانى;

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
        state->رەقەم_pending--;
        /* Resize array if it was the last element */
        إذا (idx + 1 == darray_size(state->pending))
            darray_size(state->pending)--;
        state->mods &= ~pending->mods;
    }

    أرجع XKB_SUCCESS;
}

تعداد {
    /** Tries to avoid pressing a key already pressed */
    ئاچقۇچ_CHOICE_MAX_TRIES = 12,
    XKB_ERROR_NO_RELEASED_ئاچقۇچ = -1000,
};

ساكن تعداد xkb_status
add_modifier_ئاچقۇچ(بنية xkb_typing_state * restrict state, size_t * restrict e)
{
    /* Choose a random key */
    long idx = -1;
    لكل (صحيح tries = 0; tries < ئاچقۇچ_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state)
            % darray_size(state->ئاچقۇچلار->modifiers);
        /*
         * Ensure key is not already down and its modifier mask is not active,
         * i.e. to avoid simultaneous keys with the same modifier.
         */
        إذا (!bit_array_get(state->ئاچقۇچلار_state,
                           darray_item(state->ئاچقۇچلار->modifiers, idx).keycode) &&
            !(state->mods & darray_item(state->ئاچقۇچلار->modifiers, idx).mods))
        {
            اكسر;
        }
        idx = -1;
    }

    بنية xkb_modifier_ئاچقۇچ ئاچقۇچ = {0};
    إذا (idx < 0) {
        /* No released key found: fall back to linear search */
        ثابت بنية xkb_modifier_ئاچقۇچ *ئاچقۇچ_2;
        darray_foreach(ئاچقۇچ_2, state->ئاچقۇچلار->modifiers) {
            /* See test doc above */
            إذا (!bit_array_get(state->ئاچقۇچلار_state, ئاچقۇچ_2->keycode) &&
                !(state->mods & ئاچقۇچ_2->mods))
            {
                ئاچقۇچ = *ئاچقۇچ_2;
                idx = 0;
                اكسر;
            }
        }
        إذا (idx < 0) {
            /* No fallback: skip */
            أرجع (تعداد xkb_status)XKB_ERROR_NO_RELEASED_ئاچقۇچ;
        }
    } وإلا {
        ئاچقۇچ = darray_item(state->ئاچقۇچلار->modifiers, idx);
    }

    ثابت size_t remaining_events =
        state->رەقەم_events - state->رەقەم_pending - *e;
    size_t pending_turns = 0;

    إذا (remaining_events >= 2) {
        إذا (ئاچقۇچ.kind == XKB_MOD_SET ||
            (ئاچقۇچ.kind == XKB_MOD_LOCK && remaining_events >= 4))
        {
            /* Delay SetMods release or LockMods unlock */
            ثابت size_t max_turns = MIN(
                (size_t)(
                    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS -
                    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                 ),
                remaining_events + state->رەقەم_pending
            );
            pending_turns = XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                          + (size_t)state->prng(state->prng_state)
                          % (max_turns + 1);
        }
    } وإلا {
        أرجع XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (بنية xkb_typing_event) {
        .keycode = ئاچقۇچ.keycode,
        .direction = XKB_ئاچقۇچ_DOWN,
    };

    بنية xkb_pending_typing_event pending[2] = {
        {
            .event = {
                .keycode = ئاچقۇچ.keycode,
                .direction = XKB_ئاچقۇچ_DOWN,
            },
            .counter = (uint8_t)pending_turns,
        },
        {
            .event = {
                .keycode = ئاچقۇچ.keycode,
                .direction = XKB_ئاچقۇچ_UP,
            },
            .mods = ئاچقۇچ.mods,
            .counter = (uint8_t)pending_turns,
        },
    };

    إذا (pending_turns) {
        إذا (ئاچقۇچ.kind == XKB_MOD_LOCK) {
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
            bit_array_set(state->ئاچقۇچلار_state, ئاچقۇچ.keycode);
        } وإلا {
            /* Delayed release */
            ثابت تعداد xkb_status status =
                add_pending_events(state, &pending[1], 1);
            إذا (status != XKB_SUCCESS)
                أرجع status;
            /* Register pressed state */
            bit_array_set(state->ئاچقۇچلار_state, ئاچقۇچ.keycode);
        }
        state->mods |= ئاچقۇچ.mods;
    } وإلا {
        /* Immediate release */
        state->events[(*e)++] = pending[1].event;
        /* TODO: add latched modifier to mask state. */
    }

    أرجع XKB_SUCCESS;
}

ساكن تعداد xkb_status
add_other_ئاچقۇچ(بنية xkb_typing_state * restrict state,
              ثابت xkb_keycodes_t * restrict keycodes, size_t * restrict e,
              uint8_t immediate_release_prob, uint8_t max_pending_turns)
{
    /* Choose a random key */
    long idx = -1;
    لكل (صحيح tries = 0; tries < ئاچقۇچ_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state) % darray_size(*keycodes);
        /* Ensure key is not already down */
        إذا (!bit_array_get(state->ئاچقۇچلار_state,
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
            إذا (!bit_array_get(state->ئاچقۇچلار_state, *keycode_)) {
                keycode = *keycode_;
                idx = 0;
                اكسر;
            }
        }
        إذا (idx < 0) {
            /* No fallback: skip */
            أرجع (تعداد xkb_status)XKB_ERROR_NO_RELEASED_ئاچقۇچ;
        }
    } وإلا {
        keycode = darray_item(*keycodes, idx);
    }

    ثابت size_t remaining_events =
        state->رەقەم_events - state->رەقەم_pending - *e;
    size_t pending_turns;

    إذا (remaining_events >= 2) {
        ثابت size_t max_turns = MIN(
            (size_t)max_pending_turns,
            remaining_events + state->رەقەم_pending
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
        .direction = XKB_ئاچقۇچ_DOWN,
    };

    ثابت بنية xkb_pending_typing_event pending = {
        .event = {
            .keycode = keycode,
            .direction = XKB_ئاچقۇچ_UP,
        },
        .counter = (uint8_t)pending_turns,
    };
    إذا (pending_turns) {
        ثابت تعداد xkb_status status = add_pending_events(state, &pending, 1);
        إذا (status != XKB_SUCCESS)
            أرجع status;
        /* Register pressed state */
        bit_array_set(state->ئاچقۇچلار_state, keycode);
    } وإلا {
        state->events[(*e)++] = pending.event;
    }

    أرجع XKB_SUCCESS;
}

ساكن تعداد xkb_ئاچقۇچ_kind
pick_ئاچقۇچ_kind(xkb_prng_t prng, فراغ *prng_state)
{
    ساكن ثابت uint8_t weights[رەقەم_XKB_ئاچقۇچ_KIND] = {
        [XKB_ئاچقۇچ_PRINTABLE] = 60,
        [XKB_ئاچقۇچ_MODIFIER] = 30,
        [XKB_ئاچقۇچ_MISC] = 10,
    };
    static_assert(XKB_ئاچقۇچ_MISC == 2 &&
                  XKB_ئاچقۇچ_MISC == رەقەم_XKB_ئاچقۇچ_KIND - 1, "");
    أكد(weights[0] + weights[1] + weights[2] == 100);

    long r = prng(prng_state) % 100;
    لكل (تعداد xkb_ئاچقۇچ_kind k = 0; k < رەقەم_XKB_ئاچقۇچ_KIND; k++)
        إذا ((r -= weights[k]) < 0)
            أرجع k;
    أرجع XKB_ئاچقۇچ_PRINTABLE;
}

ساكن فراغ
xkb_typing_state_destroy(بنية xkb_typing_state *state)
{
    حرر(state->ئاچقۇچلار_state);
    darray_free(state->pending);
    حرر(state->events);
}

تعداد xkb_status
xkb_typing_كىرگۈزۈش_init(بنية xkb_typing_كىرگۈزۈش * restrict typing,
                      ثابت بنية xkb_ئاچقۇچ_set * restrict ئاچقۇچلار,
                      ثابت بنية xkb_typing_كىرگۈزۈش_config * restrict config)
{
    typing->events = عدم;
    typing->رەقەم_events = 0;

    إذا (!config->كىرگۈزۈش_length)
        أرجع XKB_ERROR_INVALID;

    إذا (darray_empty(ئاچقۇچلار->printable) ||
        darray_empty(ئاچقۇچلار->modifiers) ||
        darray_empty(ئاچقۇچلار->misc))
    {
        أرجع XKB_ERROR_INVALID;
    }

    تعداد xkb_status status = XKB_SUCCESS;

    /* NOTE: arrays are sorted because they were built using the key iterator */
    xkb_keycode_t keycode_max = darray_last(ئاچقۇچلار->printable, 0);
    keycode_max = MAX(
        darray_last(ئاچقۇچلار->modifiers, (بنية xkb_modifier_ئاچقۇچ){0}).keycode,
        keycode_max
    );
    keycode_max = MAX(darray_last(ئاچقۇچلار->misc, 0), keycode_max);
    ثابت size_t ئاچقۇچلار_state_رەقەم = bit_array_سۆزلەر((size_t)keycode_max + 1);
    uintptr_t *ئاچقۇچلار_state = calloc(ئاچقۇچلار_state_رەقەم, حجم(*ئاچقۇچلار_state));

    ثابت size_t رەقەم_events = config->كىرگۈزۈش_length;
    بنية xkb_typing_event *events = calloc(رەقەم_events, حجم(*events));

    بنية xkb_typing_state state = {
        .ئاچقۇچلار = ئاچقۇچلار,
        .ئاچقۇچلار_state = ئاچقۇچلار_state,
        .prng = config->prng,
        .prng_state = config->prng_state,
        .events = events,
        .رەقەم_events = رەقەم_events,
        .pending = darray_new(),
        .رەقەم_pending = 0,
    };

    xkb_typing_events pending = darray_new();

    إذا (!ئاچقۇچلار_state || !events) {
        status = XKB_ERROR_ALLOCATION_FAILURE;
        اذهب error;
    }

    /*
     * Invariant: e + state.num_pending <= state.num_events
     */

    لكل (size_t e = 0; e < رەقەم_events;) {
        /*
         * Process pending events
         */
        status = next_pending_events(&state, &pending);
        إذا (status != XKB_SUCCESS) {
            اذهب error;
        }
        بنية xkb_typing_event *event;
        darray_foreach(event, pending) {
            إذا (e >= رەقەم_events) {
                /* invariant violated: should be unreachable */
                PANIC_UNREACHABLE();
            }
            events[e] = *event;
            bit_array_assign(ئاچقۇچلار_state, event->keycode, (bool)event->direction);
            e++;
        }

        إذا (!state.رەقەم_pending && e == رەقەم_events - 1) {
            /*
             * Cannot add press/release *pair*, so just use an invalid keycode
             * for the last event.
             */
            events[e] = (بنية xkb_typing_event) {
                .keycode = XKB_KEYCODE_INVALID,
                .direction = XKB_ئاچقۇچ_DOWN,
            };
            اكسر;
        }

        إذا (e + 1 + state.رەقەم_pending >= رەقەم_events) {
            /* Cannot add new pair of events: require flushing pending events */
            استمر;
        }

        /*
         * Create new events
         */

        /* Choose kind of key */
        static_assert(XKB_ئاچقۇچ_PRINTABLE == 0, "");
        ثابت تعداد xkb_ئاچقۇچ_kind ئاچقۇچ_kind =
            pick_ئاچقۇچ_kind(config->prng, config->prng_state);
        بدل (ئاچقۇچ_kind) {
        حالة XKB_ئاچقۇچ_PRINTABLE:
            status = add_other_ئاچقۇچ(
                &state, &state.ئاچقۇچلار->printable, &e,
                XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS
            );
            اكسر;
        حالة XKB_ئاچقۇچ_MODIFIER:
            status = add_modifier_ئاچقۇچ(&state, &e);
            اكسر;
        حالة XKB_ئاچقۇچ_MISC:
            status = add_other_ئاچقۇچ(
                &state, &state.ئاچقۇچلار->misc, &e,
                XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS
            );
            اكسر;
        افتراضي: {
            static_assert(XKB_ئاچقۇچ_MISC == 2 &&
                          XKB_ئاچقۇچ_MISC == رەقەم_XKB_ئاچقۇچ_KIND - 1, "");
            status = XKB_ERROR_INVALID;
        }}

        إذا (status != XKB_SUCCESS &&
            status != (تعداد xkb_status)XKB_ERROR_NO_RELEASED_ئاچقۇچ)
        {
            اذهب error;
        }
    }

    أكد(!state.رەقەم_pending);

    typing->events = steal(&state.events);
    typing->رەقەم_events = رەقەم_events;

    status = XKB_SUCCESS;

error:
    darray_free(pending);
    xkb_typing_state_destroy(&state);

    أرجع status;
}

فراغ
xkb_typing_كىرگۈزۈش_destroy(بنية xkb_typing_كىرگۈزۈش *typing)
{
    إذا (!typing)
        أرجع;

    حرر(typing->events);
    typing->events = عدم;
}
