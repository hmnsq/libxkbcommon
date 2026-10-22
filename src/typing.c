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
xkb_চাবি_set_init(কাঠামো xkb_চাবি_set *set,
                 কাঠামো xkb_keymap *keymap,
                 ধ্রুবক কাঠামো xkb_চাবি_set_config *config)
{
    darray_init(set->printable);
    darray_init(set->modifiers);
    darray_init(set->misc);

    স্থির ধ্রুবক কাঠামো xkb_চাবি_set_config default_config = {0};
    যদি (!config)
        config = &default_config;
    /* keycode = 0 denotes “default” */
    ধ্রুবক xkb_keycode_t min = MAX(config->min, xkb_keymap_min_keycode(keymap));
    ধ্রুবক xkb_keycode_t max = config->max
        ? MIN(config->max, xkb_keymap_max_keycode(keymap))
        : xkb_keymap_max_keycode(keymap);

    গণনা xkb_status status;
    কাঠামো xkb_state *state =
        xkb_state_new_with_mode(keymap, XKB_STATE_MODE_SERVER, &status);
    যদি (!state)
        ফেরত status;

    #ifdef USE_ISWPRINT_L
    locale_t loc = newlocale(LC_ALL_MASK, "C.UTF-8", (locale_t)0);
    #endif

    কাঠামো xkb_keymap_চাবি_iterator iter;
    ধ্রুবক কাঠামো xkb_keymap_চাবি_iterator_config iter_config = {
        .আকার_2 = আকার(iter_config),
        .flags = XKB_KEYMAP_চাবি_ITERATOR_NO_FLAGS,
        .start = min
    };
    status = xkb_keymap_চাবি_iterator_init(&iter, keymap, &iter_config);
    যদি (status != XKB_SUCCESS)
        যাও error;

    ধ্রুবক xkb_mod_mask_t numlock =
        xkb_keymap_mod_get_mask(keymap, XKB_VMOD_NAME_অঙ্ক);

    ধ্রুবক কাঠামো xkb_state_components_update components_update = {
        .আকার_2 = আকার(components_update),
        .components = XKB_STATE_MODS_LOCKED,
        .affect_locked_mods = numlock,
        .locked_mods = numlock,
    };

    ধ্রুবক কাঠামো xkb_synthetic_update update = {
        .আকার_2 = আকার(update),
        .components = &components_update,
    };

    xkb_keycode_t k;
    যখন ((k = xkb_keymap_চাবি_iterator_পরবর্তী(&iter)) != XKB_KEYCODE_INVALID) {
        যদি (k > max)
            বিরতি;

        xkb_state_reset(state);
        status = xkb_state_update_synthetic(state, &update, ফাঁকা);
        যদি (status != XKB_SUCCESS)
            যাও error;

        ধ্রুবক xkb_keysym_t *syms = ফাঁকা;
        ধ্রুবক পূর্ণসংখ্যা গুনতি = xkb_state_চাবি_get_syms(state, k, &syms);

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
            darray_append(set->modifiers, (কাঠামো xkb_modifier_চাবি) {
                .keycode = k,
                .mods = mods,
                .kind = kind,
            });
            চালু;
        }

        যদি (গুনতি > 0) {
            uint32_t utf32 = 0;
            জন্য (পূর্ণসংখ্যা s = 0; s < গুনতি; s++) {
                utf32 = xkb_keysym_to_utf32(syms[s]);
                ধ্রুবক bool is_printable =
                    #ifdef USE_ISWPRINT_L
                    ((loc && iswprint_l((wint_t)utf32, loc)) || iswprint((wint_t)utf32))
                    #নতুবা_যদি defined(USE_ISWPRINT)
                    iswprint((wint_t)utf32)
                    #অন্যথা
                    utf32 <= 0x7f && isprint((পূর্ণসংখ্যা)utf32)
                    #endif
                    ;
                যদি (is_printable)
                    বিরতি;
                utf32 = 0;
            }
            যদি (utf32) {
                /* Printable keysyms */
                darray_append(set->printable, k);
                চালু;
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

    ফেরত status;
}

শূন্য
xkb_চাবি_set_destroy(কাঠামো xkb_চাবি_set *set)
{
    যদি (!set)
        ফেরত;

    darray_free(set->modifiers);
    darray_free(set->printable);
    darray_free(set->misc);
}

স্থির গণনা xkb_status
add_pending_events(কাঠামো xkb_typing_state * restrict state,
                   ধ্রুবক কাঠামো xkb_pending_typing_event * restrict events,
                   আকার_t গুনতি)
{
    জন্য (আকার_t e = 0; e < গুনতি; e++) {
        /* Look for a free slot */
        কাঠামো xkb_pending_typing_event *pending;
        darray_আকার_t idx = 0;
        darray_enumerate_from(idx, pending, state->pending, idx) {
            যদি (!pending->গণক) {
                darray_item(state->pending, idx) = events[e];
                যাও পরবর্তী;
            }
        }
        /* No free slot found: append new one */
        idx = darray_আকার(state->pending);
        darray_append(state->pending, events[e]);
    পরবর্তী:
        /* Avoid immediate consumption */
        darray_item(state->pending, idx).গণক++;
    }
    state->অঙ্ক_pending += গুনতি;

    ফেরত XKB_SUCCESS;
}

স্থির গণনা xkb_status
পরবর্তী_pending_events(কাঠামো xkb_typing_state * restrict state,
                    xkb_typing_events * restrict out)
{
    /* Reset the output */
    darray_আকার(*out) = 0;

    darray_আকার_t idx;
    কাঠামো xkb_pending_typing_event *pending;
    /* Stack: start from the end */
    darray_enumerate_reverse(idx, pending, state->pending) {
        যদি (!pending->গণক)
            চালু;
        pending->গণক--;
        যদি (pending->গণক)
            চালু;
        darray_append(*out, pending->event);
        state->অঙ্ক_pending--;
        /* Resize array if it was the last element */
        যদি (idx + 1 == darray_আকার(state->pending))
            darray_আকার(state->pending)--;
        state->mods &= ~pending->mods;
    }

    ফেরত XKB_SUCCESS;
}

গণনা {
    /** Tries to avoid pressing a key already pressed */
    চাবি_CHOICE_MAX_TRIES = 12,
    XKB_ERROR_NO_RELEASED_চাবি = -1000,
};

স্থির গণনা xkb_status
add_modifier_চাবি(কাঠামো xkb_typing_state * restrict state, আকার_t * restrict e)
{
    /* Choose a random key */
    long idx = -1;
    জন্য (পূর্ণসংখ্যা tries = 0; tries < চাবি_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state)
            % darray_আকার(state->চাবিগুলি->modifiers);
        /*
         * Ensure key is not already down and its modifier mask is not active,
         * i.e. to avoid simultaneous keys with the same modifier.
         */
        যদি (!bit_array_get(state->চাবিগুলি_state,
                           darray_item(state->চাবিগুলি->modifiers, idx).keycode) &&
            !(state->mods & darray_item(state->চাবিগুলি->modifiers, idx).mods))
        {
            বিরতি;
        }
        idx = -1;
    }

    কাঠামো xkb_modifier_চাবি চাবি = {0};
    যদি (idx < 0) {
        /* No released key found: fall back to linear search */
        ধ্রুবক কাঠামো xkb_modifier_চাবি *চাবি_2;
        darray_foreach(চাবি_2, state->চাবিগুলি->modifiers) {
            /* See test doc above */
            যদি (!bit_array_get(state->চাবিগুলি_state, চাবি_2->keycode) &&
                !(state->mods & চাবি_2->mods))
            {
                চাবি = *চাবি_2;
                idx = 0;
                বিরতি;
            }
        }
        যদি (idx < 0) {
            /* No fallback: skip */
            ফেরত (গণনা xkb_status)XKB_ERROR_NO_RELEASED_চাবি;
        }
    } অন্যথা {
        চাবি = darray_item(state->চাবিগুলি->modifiers, idx);
    }

    ধ্রুবক আকার_t remaining_events =
        state->অঙ্ক_events - state->অঙ্ক_pending - *e;
    আকার_t pending_turns = 0;

    যদি (remaining_events >= 2) {
        যদি (চাবি.kind == XKB_MOD_SET ||
            (চাবি.kind == XKB_MOD_LOCK && remaining_events >= 4))
        {
            /* Delay SetMods release or LockMods unlock */
            ধ্রুবক আকার_t max_turns = MIN(
                (আকার_t)(
                    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS -
                    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                 ),
                remaining_events + state->অঙ্ক_pending
            );
            pending_turns = XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                          + (আকার_t)state->prng(state->prng_state)
                          % (max_turns + 1);
        }
    } অন্যথা {
        ফেরত XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (কাঠামো xkb_typing_event) {
        .keycode = চাবি.keycode,
        .direction = XKB_চাবি_DOWN,
    };

    কাঠামো xkb_pending_typing_event pending[2] = {
        {
            .event = {
                .keycode = চাবি.keycode,
                .direction = XKB_চাবি_DOWN,
            },
            .গণক = (uint8_t)pending_turns,
        },
        {
            .event = {
                .keycode = চাবি.keycode,
                .direction = XKB_চাবি_UP,
            },
            .mods = চাবি.mods,
            .গণক = (uint8_t)pending_turns,
        },
    };

    যদি (pending_turns) {
        যদি (চাবি.kind == XKB_MOD_LOCK) {
            /* Immediate release */
            state->events[(*e)++] = pending[1].event;
            /* Delayed unlock (press + release) */
            pending[1].গণক++;
            ধ্রুবক গণনা xkb_status status =
                add_pending_events(state, pending, ARRAY_আকার(pending));
            যদি (status != XKB_SUCCESS)
                ফেরত status;
            /*
             * Register “locked” state, to avoid tapping the key before
             * delayed key released
             */
            bit_array_set(state->চাবিগুলি_state, চাবি.keycode);
        } অন্যথা {
            /* Delayed release */
            ধ্রুবক গণনা xkb_status status =
                add_pending_events(state, &pending[1], 1);
            যদি (status != XKB_SUCCESS)
                ফেরত status;
            /* Register pressed state */
            bit_array_set(state->চাবিগুলি_state, চাবি.keycode);
        }
        state->mods |= চাবি.mods;
    } অন্যথা {
        /* Immediate release */
        state->events[(*e)++] = pending[1].event;
        /* TODO: add latched modifier to mask state. */
    }

    ফেরত XKB_SUCCESS;
}

স্থির গণনা xkb_status
add_other_চাবি(কাঠামো xkb_typing_state * restrict state,
              ধ্রুবক xkb_keycodes_t * restrict keycodes, আকার_t * restrict e,
              uint8_t immediate_release_prob, uint8_t max_pending_turns)
{
    /* Choose a random key */
    long idx = -1;
    জন্য (পূর্ণসংখ্যা tries = 0; tries < চাবি_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state) % darray_আকার(*keycodes);
        /* Ensure key is not already down */
        যদি (!bit_array_get(state->চাবিগুলি_state,
                           darray_item(*keycodes, idx)))
        {
            বিরতি;
        }
        idx = -1;
    }

    xkb_keycode_t keycode = XKB_KEYCODE_INVALID;
    যদি (idx < 0) {
        /* No released key found: fall back to linear search */
        ধ্রুবক xkb_keycode_t *keycode_;
        darray_foreach(keycode_, *keycodes) {
            যদি (!bit_array_get(state->চাবিগুলি_state, *keycode_)) {
                keycode = *keycode_;
                idx = 0;
                বিরতি;
            }
        }
        যদি (idx < 0) {
            /* No fallback: skip */
            ফেরত (গণনা xkb_status)XKB_ERROR_NO_RELEASED_চাবি;
        }
    } অন্যথা {
        keycode = darray_item(*keycodes, idx);
    }

    ধ্রুবক আকার_t remaining_events =
        state->অঙ্ক_events - state->অঙ্ক_pending - *e;
    আকার_t pending_turns;

    যদি (remaining_events >= 2) {
        ধ্রুবক আকার_t max_turns = MIN(
            (আকার_t)max_pending_turns,
            remaining_events + state->অঙ্ক_pending
        );
        ধ্রুবক bool immediate_release = (
            (immediate_release_prob >= 100) ||
            !max_turns ||
            (state->prng(state->prng_state) % 100) < immediate_release_prob
        );
        যদি (immediate_release) {
            pending_turns = 0;
        } অন্যথা {
            pending_turns = 1 + (আকার_t)state->prng(state->prng_state)
                          % max_turns;
        }
    } অন্যথা {
        ফেরত XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (কাঠামো xkb_typing_event) {
        .keycode = keycode,
        .direction = XKB_চাবি_DOWN,
    };

    ধ্রুবক কাঠামো xkb_pending_typing_event pending = {
        .event = {
            .keycode = keycode,
            .direction = XKB_চাবি_UP,
        },
        .গণক = (uint8_t)pending_turns,
    };
    যদি (pending_turns) {
        ধ্রুবক গণনা xkb_status status = add_pending_events(state, &pending, 1);
        যদি (status != XKB_SUCCESS)
            ফেরত status;
        /* Register pressed state */
        bit_array_set(state->চাবিগুলি_state, keycode);
    } অন্যথা {
        state->events[(*e)++] = pending.event;
    }

    ফেরত XKB_SUCCESS;
}

স্থির গণনা xkb_চাবি_kind
pick_চাবি_kind(xkb_prng_t prng, শূন্য *prng_state)
{
    স্থির ধ্রুবক uint8_t weights[অঙ্ক_XKB_চাবি_KIND] = {
        [XKB_চাবি_PRINTABLE] = 60,
        [XKB_চাবি_MODIFIER] = 30,
        [XKB_চাবি_MISC] = 10,
    };
    static_assert(XKB_চাবি_MISC == 2 &&
                  XKB_চাবি_MISC == অঙ্ক_XKB_চাবি_KIND - 1, "");
    প্রতিজ্ঞা(weights[0] + weights[1] + weights[2] == 100);

    long r = prng(prng_state) % 100;
    জন্য (গণনা xkb_চাবি_kind k = 0; k < অঙ্ক_XKB_চাবি_KIND; k++)
        যদি ((r -= weights[k]) < 0)
            ফেরত k;
    ফেরত XKB_চাবি_PRINTABLE;
}

স্থির শূন্য
xkb_typing_state_destroy(কাঠামো xkb_typing_state *state)
{
    মুক্ত(state->চাবিগুলি_state);
    darray_free(state->pending);
    মুক্ত(state->events);
}

গণনা xkb_status
xkb_typing_প্রবেশ_init(কাঠামো xkb_typing_প্রবেশ * restrict typing,
                      ধ্রুবক কাঠামো xkb_চাবি_set * restrict চাবিগুলি,
                      ধ্রুবক কাঠামো xkb_typing_প্রবেশ_config * restrict config)
{
    typing->events = ফাঁকা;
    typing->অঙ্ক_events = 0;

    যদি (!config->প্রবেশ_দৈর্ঘ্য)
        ফেরত XKB_ERROR_INVALID;

    যদি (darray_empty(চাবিগুলি->printable) ||
        darray_empty(চাবিগুলি->modifiers) ||
        darray_empty(চাবিগুলি->misc))
    {
        ফেরত XKB_ERROR_INVALID;
    }

    গণনা xkb_status status = XKB_SUCCESS;

    /* NOTE: arrays are sorted because they were built using the key iterator */
    xkb_keycode_t keycode_max = darray_last(চাবিগুলি->printable, 0);
    keycode_max = MAX(
        darray_last(চাবিগুলি->modifiers, (কাঠামো xkb_modifier_চাবি){0}).keycode,
        keycode_max
    );
    keycode_max = MAX(darray_last(চাবিগুলি->misc, 0), keycode_max);
    ধ্রুবক আকার_t চাবিগুলি_state_অঙ্ক = bit_array_শব্দগুলি((আকার_t)keycode_max + 1);
    uintptr_t *চাবিগুলি_state = calloc(চাবিগুলি_state_অঙ্ক, আকার(*চাবিগুলি_state));

    ধ্রুবক আকার_t অঙ্ক_events = config->প্রবেশ_দৈর্ঘ্য;
    কাঠামো xkb_typing_event *events = calloc(অঙ্ক_events, আকার(*events));

    কাঠামো xkb_typing_state state = {
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
        যাও error;
    }

    /*
     * Invariant: e + state.num_pending <= state.num_events
     */

    জন্য (আকার_t e = 0; e < অঙ্ক_events;) {
        /*
         * Process pending events
         */
        status = পরবর্তী_pending_events(&state, &pending);
        যদি (status != XKB_SUCCESS) {
            যাও error;
        }
        কাঠামো xkb_typing_event *event;
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
            events[e] = (কাঠামো xkb_typing_event) {
                .keycode = XKB_KEYCODE_INVALID,
                .direction = XKB_চাবি_DOWN,
            };
            বিরতি;
        }

        যদি (e + 1 + state.অঙ্ক_pending >= অঙ্ক_events) {
            /* Cannot add new pair of events: require flushing pending events */
            চালু;
        }

        /*
         * Create new events
         */

        /* Choose kind of key */
        static_assert(XKB_চাবি_PRINTABLE == 0, "");
        ধ্রুবক গণনা xkb_চাবি_kind চাবি_kind =
            pick_চাবি_kind(config->prng, config->prng_state);
        নির্বাচন (চাবি_kind) {
        ক্ষেত্র XKB_চাবি_PRINTABLE:
            status = add_other_চাবি(
                &state, &state.চাবিগুলি->printable, &e,
                XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS
            );
            বিরতি;
        ক্ষেত্র XKB_চাবি_MODIFIER:
            status = add_modifier_চাবি(&state, &e);
            বিরতি;
        ক্ষেত্র XKB_চাবি_MISC:
            status = add_other_চাবি(
                &state, &state.চাবিগুলি->misc, &e,
                XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS
            );
            বিরতি;
        পূর্বনির্ধারিত: {
            static_assert(XKB_চাবি_MISC == 2 &&
                          XKB_চাবি_MISC == অঙ্ক_XKB_চাবি_KIND - 1, "");
            status = XKB_ERROR_INVALID;
        }}

        যদি (status != XKB_SUCCESS &&
            status != (গণনা xkb_status)XKB_ERROR_NO_RELEASED_চাবি)
        {
            যাও error;
        }
    }

    প্রতিজ্ঞা(!state.অঙ্ক_pending);

    typing->events = steal(&state.events);
    typing->অঙ্ক_events = অঙ্ক_events;

    status = XKB_SUCCESS;

error:
    darray_free(pending);
    xkb_typing_state_destroy(&state);

    ফেরত status;
}

শূন্য
xkb_typing_প্রবেশ_destroy(কাঠামো xkb_typing_প্রবেশ *typing)
{
    যদি (!typing)
        ফেরত;

    মুক্ত(typing->events);
    typing->events = ফাঁকা;
}
