/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <stdbool.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <stdint.h>
#যদি WCHAR_MAX >= 0x7FFFFFFF
#অন্তর্ভাব <wctype.h>
#define USE_ISWPRINT
#যদি HAVE_NEWLOCALE && HAVE_ISWPRINT_L
#define USE_ISWPRINT_L
#অন্তর্ভাব <locale.h>
#endif
#অন্যথা
#অন্তর্ভাব <ctype.h>
#endif

#অন্তর্ভাব "xkbcommon/xkbcommon-status.h"
#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "darray.h"
#অন্তর্ভাব "state-priv.h"
#অন্তর্ভাব "typing.h"
#অন্তর্ভাব "util-bits.h"
#অন্তর্ভাব "util-mem.h"
#অন্তর্ভাব "utils.h"

গণনা xkb_status
xkb_চাবি_set_init(সংরচনা xkb_চাবি_set *set,
                 সংরচনা xkb_keymap *keymap,
                 ধ্রুব সংরচনা xkb_চাবি_set_config *config)
{
    darray_init(set->printable);
    darray_init(set->modifiers);
    darray_init(set->misc);

    স্থির ধ্রুব সংরচনা xkb_চাবি_set_config default_config = {0};
    যদি (!config)
        config = &default_config;
    /* keycode = 0 denotes “default” */
    ধ্রুব xkb_keycode_t min = MAX(config->min, xkb_keymap_min_keycode(keymap));
    ধ্রুব xkb_keycode_t max = config->max
        ? MIN(config->max, xkb_keymap_max_keycode(keymap))
        : xkb_keymap_max_keycode(keymap);

    গণনা xkb_status status;
    সংরচনা xkb_state *state =
        xkb_state_new_with_mode(keymap, XKB_STATE_MODE_SERVER, &status);
    যদি (!state)
        প্রত্যাগম status;

    #ifdef USE_ISWPRINT_L
    locale_t loc = newlocale(LC_ALL_MASK, "C.UTF-8", (locale_t)0);
    #endif

    সংরচনা xkb_keymap_চাবি_iterator iter;
    ধ্রুব সংরচনা xkb_keymap_চাবি_iterator_config iter_config = {
        .size = পরিমাণ(iter_config),
        .flags = XKB_KEYMAP_চাবি_ITERATOR_NO_FLAGS,
        .start = min
    };
    status = xkb_keymap_চাবি_iterator_init(&iter, keymap, &iter_config);
    যদি (status != XKB_SUCCESS)
        গচ্ছ error;

    ধ্রুব xkb_mod_mask_t numlock =
        xkb_keymap_mod_get_mask(keymap, XKB_VMOD_NAME_অঙ্ক);

    ধ্রুব সংরচনা xkb_state_components_update components_update = {
        .size = পরিমাণ(components_update),
        .components = XKB_STATE_MODS_LOCKED,
        .affect_locked_mods = numlock,
        .locked_mods = numlock,
    };

    ধ্রুব সংরচনা xkb_synthetic_update update = {
        .size = পরিমাণ(update),
        .components = &components_update,
    };

    xkb_keycode_t k;
    যাবত্ ((k = xkb_keymap_চাবি_iterator_next(&iter)) != XKB_KEYCODE_INVALID) {
        যদি (k > max)
            ভঙ্গ;

        xkb_state_reset(state);
        status = xkb_state_update_synthetic(state, &update, রিক্ত);
        যদি (status != XKB_SUCCESS)
            গচ্ছ error;

        ধ্রুব xkb_keysym_t *syms = রিক্ত;
        ধ্রুব পূর্ণাঙ্ক গুনতি = xkb_state_চাবি_get_syms(state, k, &syms);

        গণনা xkb_state_component changed =
            xkb_state_update_চাবি(state, k, XKB_চাবি_DOWN);
        xkb_mod_mask_t mods =
            xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);
        changed |= xkb_state_update_চাবি(state, k, XKB_চাবি_UP);
        mods |= xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);

        যদি (changed & XKB_STATE_MODS_EFFECTIVE) {
            /* Modifier key */
            গণনা xkb_modifier_kind kind;
            যদি (changed & XKB_STATE_MODS_LOCKED) {
                kind = XKB_MOD_LOCK;
            } অন্যথা যদি (changed & XKB_STATE_MODS_LATCHED) {
                kind = XKB_MOD_LATCH;
            } অন্যথা {
                kind = XKB_MOD_SET;
            }
            mods &= ~numlock;
            যদি (!mods)
                mods = numlock;
            darray_append(set->modifiers, (সংরচনা xkb_modifier_চাবি) {
                .keycode = k,
                .mods = mods,
                .kind = kind,
            });
            অনুবর্ত;
        }

        যদি (গুনতি > 0) {
            uint32_t utf32 = 0;
            প্রতি (পূর্ণাঙ্ক s = 0; s < গুনতি; s++) {
                utf32 = xkb_keysym_to_utf32(syms[s]);
                ধ্রুব bool is_printable =
                    #ifdef USE_ISWPRINT_L
                    ((loc && iswprint_l((wint_t)utf32, loc)) || iswprint((wint_t)utf32))
                    #নোচেদ্যদি defined(USE_ISWPRINT)
                    iswprint((wint_t)utf32)
                    #অন্যথা
                    utf32 <= 0x7f && isprint((পূর্ণাঙ্ক)utf32)
                    #endif
                    ;
                যদি (is_printable)
                    ভঙ্গ;
                utf32 = 0;
            }
            যদি (utf32) {
                /* Printable keysyms */
                darray_append(set->printable, k);
                অনুবর্ত;
            }
        }

        /* Miscellaneous key */
        darray_append(set->misc, k);
    }

error:
    #ifdef USE_ISWPRINT_L
    যদি (loc)
        freelocale(loc);
    #endif

    xkb_state_unref(state);
    যদি (status != XKB_SUCCESS)
        xkb_চাবি_set_destroy(set);

    প্রত্যাগম status;
}

শূন্য
xkb_চাবি_set_destroy(সংরচনা xkb_চাবি_set *set)
{
    যদি (!set)
        প্রত্যাগম;

    darray_free(set->modifiers);
    darray_free(set->printable);
    darray_free(set->misc);
}

স্থির গণনা xkb_status
add_pending_events(সংরচনা xkb_typing_state * restrict state,
                   ধ্রুব সংরচনা xkb_pending_typing_event * restrict events,
                   size_t গুনতি)
{
    প্রতি (size_t e = 0; e < গুনতি; e++) {
        /* Look for a free slot */
        সংরচনা xkb_pending_typing_event *pending;
        darray_size_t idx = 0;
        darray_enumerate_from(idx, pending, state->pending, idx) {
            যদি (!pending->counter) {
                darray_item(state->pending, idx) = events[e];
                গচ্ছ next;
            }
        }
        /* No free slot found: append new one */
        idx = darray_size(state->pending);
        darray_append(state->pending, events[e]);
    next:
        /* Avoid immediate consumption */
        darray_item(state->pending, idx).counter++;
    }
    state->অঙ্ক_pending += গুনতি;

    প্রত্যাগম XKB_SUCCESS;
}

স্থির গণনা xkb_status
next_pending_events(সংরচনা xkb_typing_state * restrict state,
                    xkb_typing_events * restrict out)
{
    /* Reset the output */
    darray_size(*out) = 0;

    darray_size_t idx;
    সংরচনা xkb_pending_typing_event *pending;
    /* Stack: start from the end */
    darray_enumerate_reverse(idx, pending, state->pending) {
        যদি (!pending->counter)
            অনুবর্ত;
        pending->counter--;
        যদি (pending->counter)
            অনুবর্ত;
        darray_append(*out, pending->event);
        state->অঙ্ক_pending--;
        /* Resize array if it was the last element */
        যদি (idx + 1 == darray_size(state->pending))
            darray_size(state->pending)--;
        state->mods &= ~pending->mods;
    }

    প্রত্যাগম XKB_SUCCESS;
}

গণনা {
    /** Tries to avoid pressing a key already pressed */
    চাবি_CHOICE_MAX_TRIES = 12,
    XKB_ERROR_NO_RELEASED_চাবি = -1000,
};

স্থির গণনা xkb_status
add_modifier_চাবি(সংরচনা xkb_typing_state * restrict state, size_t * restrict e)
{
    /* Choose a random key */
    long idx = -1;
    প্রতি (পূর্ণাঙ্ক tries = 0; tries < চাবি_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state)
            % darray_size(state->চাবিগুলি->modifiers);
        /*
         * Ensure key is not already down and its modifier mask is not active,
         * i.e. to avoid simultaneous keys with the same modifier.
         */
        যদি (!bit_array_get(state->চাবিগুলি_state,
                           darray_item(state->চাবিগুলি->modifiers, idx).keycode) &&
            !(state->mods & darray_item(state->চাবিগুলি->modifiers, idx).mods))
        {
            ভঙ্গ;
        }
        idx = -1;
    }

    সংরচনা xkb_modifier_চাবি চাবি = {0};
    যদি (idx < 0) {
        /* No released key found: fall back to linear search */
        ধ্রুব সংরচনা xkb_modifier_চাবি *চাবি_2;
        darray_foreach(চাবি_2, state->চাবিগুলি->modifiers) {
            /* See test doc above */
            যদি (!bit_array_get(state->চাবিগুলি_state, চাবি_2->keycode) &&
                !(state->mods & চাবি_2->mods))
            {
                চাবি = *চাবি_2;
                idx = 0;
                ভঙ্গ;
            }
        }
        যদি (idx < 0) {
            /* No fallback: skip */
            প্রত্যাগম (গণনা xkb_status)XKB_ERROR_NO_RELEASED_চাবি;
        }
    } অন্যথা {
        চাবি = darray_item(state->চাবিগুলি->modifiers, idx);
    }

    ধ্রুব size_t remaining_events =
        state->অঙ্ক_events - state->অঙ্ক_pending - *e;
    size_t pending_turns = 0;

    যদি (remaining_events >= 2) {
        যদি (চাবি.kind == XKB_MOD_SET ||
            (চাবি.kind == XKB_MOD_LOCK && remaining_events >= 4))
        {
            /* Delay SetMods release or LockMods unlock */
            ধ্রুব size_t max_turns = MIN(
                (size_t)(
                    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS -
                    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                 ),
                remaining_events + state->অঙ্ক_pending
            );
            pending_turns = XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                          + (size_t)state->prng(state->prng_state)
                          % (max_turns + 1);
        }
    } অন্যথা {
        প্রত্যাগম XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (সংরচনা xkb_typing_event) {
        .keycode = চাবি.keycode,
        .direction = XKB_চাবি_DOWN,
    };

    সংরচনা xkb_pending_typing_event pending[2] = {
        {
            .event = {
                .keycode = চাবি.keycode,
                .direction = XKB_চাবি_DOWN,
            },
            .counter = (uint8_t)pending_turns,
        },
        {
            .event = {
                .keycode = চাবি.keycode,
                .direction = XKB_চাবি_UP,
            },
            .mods = চাবি.mods,
            .counter = (uint8_t)pending_turns,
        },
    };

    যদি (pending_turns) {
        যদি (চাবি.kind == XKB_MOD_LOCK) {
            /* Immediate release */
            state->events[(*e)++] = pending[1].event;
            /* Delayed unlock (press + release) */
            pending[1].counter++;
            ধ্রুব গণনা xkb_status status =
                add_pending_events(state, pending, ARRAY_SIZE(pending));
            যদি (status != XKB_SUCCESS)
                প্রত্যাগম status;
            /*
             * Register “locked” state, to avoid tapping the key before
             * delayed key released
             */
            bit_array_set(state->চাবিগুলি_state, চাবি.keycode);
        } অন্যথা {
            /* Delayed release */
            ধ্রুব গণনা xkb_status status =
                add_pending_events(state, &pending[1], 1);
            যদি (status != XKB_SUCCESS)
                প্রত্যাগম status;
            /* Register pressed state */
            bit_array_set(state->চাবিগুলি_state, চাবি.keycode);
        }
        state->mods |= চাবি.mods;
    } অন্যথা {
        /* Immediate release */
        state->events[(*e)++] = pending[1].event;
        /* TODO: add latched modifier to mask state. */
    }

    প্রত্যাগম XKB_SUCCESS;
}

স্থির গণনা xkb_status
add_other_চাবি(সংরচনা xkb_typing_state * restrict state,
              ধ্রুব xkb_keycodes_t * restrict keycodes, size_t * restrict e,
              uint8_t immediate_release_prob, uint8_t max_pending_turns)
{
    /* Choose a random key */
    long idx = -1;
    প্রতি (পূর্ণাঙ্ক tries = 0; tries < চাবি_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state) % darray_size(*keycodes);
        /* Ensure key is not already down */
        যদি (!bit_array_get(state->চাবিগুলি_state,
                           darray_item(*keycodes, idx)))
        {
            ভঙ্গ;
        }
        idx = -1;
    }

    xkb_keycode_t keycode = XKB_KEYCODE_INVALID;
    যদি (idx < 0) {
        /* No released key found: fall back to linear search */
        ধ্রুব xkb_keycode_t *keycode_;
        darray_foreach(keycode_, *keycodes) {
            যদি (!bit_array_get(state->চাবিগুলি_state, *keycode_)) {
                keycode = *keycode_;
                idx = 0;
                ভঙ্গ;
            }
        }
        যদি (idx < 0) {
            /* No fallback: skip */
            প্রত্যাগম (গণনা xkb_status)XKB_ERROR_NO_RELEASED_চাবি;
        }
    } অন্যথা {
        keycode = darray_item(*keycodes, idx);
    }

    ধ্রুব size_t remaining_events =
        state->অঙ্ক_events - state->অঙ্ক_pending - *e;
    size_t pending_turns;

    যদি (remaining_events >= 2) {
        ধ্রুব size_t max_turns = MIN(
            (size_t)max_pending_turns,
            remaining_events + state->অঙ্ক_pending
        );
        ধ্রুব bool immediate_release = (
            (immediate_release_prob >= 100) ||
            !max_turns ||
            (state->prng(state->prng_state) % 100) < immediate_release_prob
        );
        যদি (immediate_release) {
            pending_turns = 0;
        } অন্যথা {
            pending_turns = 1 + (size_t)state->prng(state->prng_state)
                          % max_turns;
        }
    } অন্যথা {
        প্রত্যাগম XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (সংরচনা xkb_typing_event) {
        .keycode = keycode,
        .direction = XKB_চাবি_DOWN,
    };

    ধ্রুব সংরচনা xkb_pending_typing_event pending = {
        .event = {
            .keycode = keycode,
            .direction = XKB_চাবি_UP,
        },
        .counter = (uint8_t)pending_turns,
    };
    যদি (pending_turns) {
        ধ্রুব গণনা xkb_status status = add_pending_events(state, &pending, 1);
        যদি (status != XKB_SUCCESS)
            প্রত্যাগম status;
        /* Register pressed state */
        bit_array_set(state->চাবিগুলি_state, keycode);
    } অন্যথা {
        state->events[(*e)++] = pending.event;
    }

    প্রত্যাগম XKB_SUCCESS;
}

স্থির গণনা xkb_চাবি_kind
pick_চাবি_kind(xkb_prng_t prng, শূন্য *prng_state)
{
    স্থির ধ্রুব uint8_t weights[অঙ্ক_XKB_চাবি_KIND] = {
        [XKB_চাবি_PRINTABLE] = 60,
        [XKB_চাবি_MODIFIER] = 30,
        [XKB_চাবি_MISC] = 10,
    };
    static_assert(XKB_চাবি_MISC == 2 &&
                  XKB_চাবি_MISC == অঙ্ক_XKB_চাবি_KIND - 1, "");
    প্রতিজ্ঞা(weights[0] + weights[1] + weights[2] == 100);

    long r = prng(prng_state) % 100;
    প্রতি (গণনা xkb_চাবি_kind k = 0; k < অঙ্ক_XKB_চাবি_KIND; k++)
        যদি ((r -= weights[k]) < 0)
            প্রত্যাগম k;
    প্রত্যাগম XKB_চাবি_PRINTABLE;
}

স্থির শূন্য
xkb_typing_state_destroy(সংরচনা xkb_typing_state *state)
{
    মুক্ত(state->চাবিগুলি_state);
    darray_free(state->pending);
    মুক্ত(state->events);
}

গণনা xkb_status
xkb_typing_প্রবেশ_init(সংরচনা xkb_typing_প্রবেশ * restrict typing,
                      ধ্রুব সংরচনা xkb_চাবি_set * restrict চাবিগুলি,
                      ধ্রুব সংরচনা xkb_typing_প্রবেশ_config * restrict config)
{
    typing->events = রিক্ত;
    typing->অঙ্ক_events = 0;

    যদি (!config->প্রবেশ_length)
        প্রত্যাগম XKB_ERROR_INVALID;

    যদি (darray_empty(চাবিগুলি->printable) ||
        darray_empty(চাবিগুলি->modifiers) ||
        darray_empty(চাবিগুলি->misc))
    {
        প্রত্যাগম XKB_ERROR_INVALID;
    }

    গণনা xkb_status status = XKB_SUCCESS;

    /* NOTE: arrays are sorted because they were built using the key iterator */
    xkb_keycode_t keycode_max = darray_last(চাবিগুলি->printable, 0);
    keycode_max = MAX(
        darray_last(চাবিগুলি->modifiers, (সংরচনা xkb_modifier_চাবি){0}).keycode,
        keycode_max
    );
    keycode_max = MAX(darray_last(চাবিগুলি->misc, 0), keycode_max);
    ধ্রুব size_t চাবিগুলি_state_অঙ্ক = bit_array_শব্দগুলি((size_t)keycode_max + 1);
    uintptr_t *চাবিগুলি_state = calloc(চাবিগুলি_state_অঙ্ক, পরিমাণ(*চাবিগুলি_state));

    ধ্রুব size_t অঙ্ক_events = config->প্রবেশ_length;
    সংরচনা xkb_typing_event *events = calloc(অঙ্ক_events, পরিমাণ(*events));

    সংরচনা xkb_typing_state state = {
        .চাবিগুলি = চাবিগুলি,
        .চাবিগুলি_state = চাবিগুলি_state,
        .prng = config->prng,
        .prng_state = config->prng_state,
        .events = events,
        .অঙ্ক_events = অঙ্ক_events,
        .pending = darray_new(),
        .অঙ্ক_pending = 0,
    };

    xkb_typing_events pending = darray_new();

    যদি (!চাবিগুলি_state || !events) {
        status = XKB_ERROR_ALLOCATION_FAILURE;
        গচ্ছ error;
    }

    /*
     * Invariant: e + state.num_pending <= state.num_events
     */

    প্রতি (size_t e = 0; e < অঙ্ক_events;) {
        /*
         * Process pending events
         */
        status = next_pending_events(&state, &pending);
        যদি (status != XKB_SUCCESS) {
            গচ্ছ error;
        }
        সংরচনা xkb_typing_event *event;
        darray_foreach(event, pending) {
            যদি (e >= অঙ্ক_events) {
                /* invariant violated: should be unreachable */
                PANIC_UNREACHABLE();
            }
            events[e] = *event;
            bit_array_assign(চাবিগুলি_state, event->keycode, (bool)event->direction);
            e++;
        }

        যদি (!state.অঙ্ক_pending && e == অঙ্ক_events - 1) {
            /*
             * Cannot add press/release *pair*, so just use an invalid keycode
             * for the last event.
             */
            events[e] = (সংরচনা xkb_typing_event) {
                .keycode = XKB_KEYCODE_INVALID,
                .direction = XKB_চাবি_DOWN,
            };
            ভঙ্গ;
        }

        যদি (e + 1 + state.অঙ্ক_pending >= অঙ্ক_events) {
            /* Cannot add new pair of events: require flushing pending events */
            অনুবর্ত;
        }

        /*
         * Create new events
         */

        /* Choose kind of key */
        static_assert(XKB_চাবি_PRINTABLE == 0, "");
        ধ্রুব গণনা xkb_চাবি_kind চাবি_kind =
            pick_চাবি_kind(config->prng, config->prng_state);
        বিকল্প (চাবি_kind) {
        পক্ষ XKB_চাবি_PRINTABLE:
            status = add_other_চাবি(
                &state, &state.চাবিগুলি->printable, &e,
                XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS
            );
            ভঙ্গ;
        পক্ষ XKB_চাবি_MODIFIER:
            status = add_modifier_চাবি(&state, &e);
            ভঙ্গ;
        পক্ষ XKB_চাবি_MISC:
            status = add_other_চাবি(
                &state, &state.চাবিগুলি->misc, &e,
                XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS
            );
            ভঙ্গ;
        স্বভাব: {
            static_assert(XKB_চাবি_MISC == 2 &&
                          XKB_চাবি_MISC == অঙ্ক_XKB_চাবি_KIND - 1, "");
            status = XKB_ERROR_INVALID;
        }}

        যদি (status != XKB_SUCCESS &&
            status != (গণনা xkb_status)XKB_ERROR_NO_RELEASED_চাবি)
        {
            গচ্ছ error;
        }
    }

    প্রতিজ্ঞা(!state.অঙ্ক_pending);

    typing->events = steal(&state.events);
    typing->অঙ্ক_events = অঙ্ক_events;

    status = XKB_SUCCESS;

error:
    darray_free(pending);
    xkb_typing_state_destroy(&state);

    প্রত্যাগম status;
}

শূন্য
xkb_typing_প্রবেশ_destroy(সংরচনা xkb_typing_প্রবেশ *typing)
{
    যদি (!typing)
        প্রত্যাগম;

    মুক্ত(typing->events);
    typing->events = রিক্ত;
}
