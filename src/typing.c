/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdint.h>
#जइ WCHAR_MAX >= 0x7FFFFFFF
#अन्तर्भाव <wctype.h>
#define USE_ISWPRINT
#जइ HAVE_NEWLOCALE && HAVE_ISWPRINT_L
#define USE_ISWPRINT_L
#अन्तर्भाव <locale.h>
#endif
#अण्णहा
#अन्तर्भाव <ctype.h>
#endif

#अन्तर्भाव "xkbcommon/xkbcommon-status.h"
#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "state-priv.h"
#अन्तर्भाव "typing.h"
#अन्तर्भाव "util-bits.h"
#अन्तर्भाव "util-mem.h"
#अन्तर्भाव "utils.h"

गणना xkb_status
xkb_कुञ्चिका_set_init(संरचना xkb_कुञ्चिका_set *set,
                 संरचना xkb_keymap *keymap,
                 ध्रुव संरचना xkb_कुञ्चिका_set_config *config)
{
    darray_init(set->printable);
    darray_init(set->modifiers);
    darray_init(set->misc);

    स्थिर ध्रुव संरचना xkb_कुञ्चिका_set_config default_config = {0};
    जइ (!config)
        config = &default_config;
    /* keycode = 0 denotes “default” */
    ध्रुव xkb_keycode_t min = MAX(config->min, xkb_keymap_min_keycode(keymap));
    ध्रुव xkb_keycode_t max = config->max
        ? MIN(config->max, xkb_keymap_max_keycode(keymap))
        : xkb_keymap_max_keycode(keymap);

    गणना xkb_status status;
    संरचना xkb_state *state =
        xkb_state_new_with_mode(keymap, XKB_STATE_MODE_SERVER, &status);
    जइ (!state)
        प्रत्यागम status;

    #ifdef USE_ISWPRINT_L
    locale_t loc = newlocale(LC_ALL_MASK, "C.UTF-8", (locale_t)0);
    #endif

    संरचना xkb_keymap_कुञ्चिका_iterator iter;
    ध्रुव संरचना xkb_keymap_कुञ्चिका_iterator_config iter_config = {
        .size = परिमाण(iter_config),
        .flags = XKB_KEYMAP_कुञ्चिका_ITERATOR_NO_FLAGS,
        .start = min
    };
    status = xkb_keymap_कुञ्चिका_iterator_init(&iter, keymap, &iter_config);
    जइ (status != XKB_SUCCESS)
        गच्छ error;

    ध्रुव xkb_mod_mask_t numlock =
        xkb_keymap_mod_get_mask(keymap, XKB_VMOD_NAME_अङ्कः);

    ध्रुव संरचना xkb_state_components_update components_update = {
        .size = परिमाण(components_update),
        .components = XKB_STATE_MODS_LOCKED,
        .affect_locked_mods = numlock,
        .locked_mods = numlock,
    };

    ध्रुव संरचना xkb_synthetic_update update = {
        .size = परिमाण(update),
        .components = &components_update,
    };

    xkb_keycode_t k;
    जाव ((k = xkb_keymap_कुञ्चिका_iterator_next(&iter)) != XKB_KEYCODE_INVALID) {
        जइ (k > max)
            भङ्ग;

        xkb_state_reset(state);
        status = xkb_state_update_synthetic(state, &update, रिक्त);
        जइ (status != XKB_SUCCESS)
            गच्छ error;

        ध्रुव xkb_keysym_t *syms = रिक्त;
        ध्रुव पूर्णाङ्क गणनम् = xkb_state_कुञ्चिका_get_syms(state, k, &syms);

        गणना xkb_state_component changed =
            xkb_state_update_कुञ्चिका(state, k, XKB_कुञ्चिका_DOWN);
        xkb_mod_mask_t mods =
            xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);
        changed |= xkb_state_update_कुञ्चिका(state, k, XKB_कुञ्चिका_UP);
        mods |= xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);

        जइ (changed & XKB_STATE_MODS_EFFECTIVE) {
            /* Modifier key */
            गणना xkb_modifier_kind kind;
            जइ (changed & XKB_STATE_MODS_LOCKED) {
                kind = XKB_MOD_LOCK;
            } अण्णहा जइ (changed & XKB_STATE_MODS_LATCHED) {
                kind = XKB_MOD_LATCH;
            } अण्णहा {
                kind = XKB_MOD_SET;
            }
            mods &= ~numlock;
            जइ (!mods)
                mods = numlock;
            darray_append(set->modifiers, (संरचना xkb_modifier_कुञ्चिका) {
                .keycode = k,
                .mods = mods,
                .kind = kind,
            });
            अनुवर्त;
        }

        जइ (गणनम् > 0) {
            uint32_t utf32 = 0;
            प्रति (पूर्णाङ्क s = 0; s < गणनम्; s++) {
                utf32 = xkb_keysym_to_utf32(syms[s]);
                ध्रुव bool is_printable =
                    #ifdef USE_ISWPRINT_L
                    ((loc && iswprint_l((wint_t)utf32, loc)) || iswprint((wint_t)utf32))
                    #नोचेद्यदि defined(USE_ISWPRINT)
                    iswprint((wint_t)utf32)
                    #अण्णहा
                    utf32 <= 0x7f && isprint((पूर्णाङ्क)utf32)
                    #endif
                    ;
                जइ (is_printable)
                    भङ्ग;
                utf32 = 0;
            }
            जइ (utf32) {
                /* Printable keysyms */
                darray_append(set->printable, k);
                अनुवर्त;
            }
        }

        /* Miscellaneous key */
        darray_append(set->misc, k);
    }

error:
    #ifdef USE_ISWPRINT_L
    जइ (loc)
        freelocale(loc);
    #endif

    xkb_state_unref(state);
    जइ (status != XKB_SUCCESS)
        xkb_कुञ्चिका_set_destroy(set);

    प्रत्यागम status;
}

शून्य
xkb_कुञ्चिका_set_destroy(संरचना xkb_कुञ्चिका_set *set)
{
    जइ (!set)
        प्रत्यागम;

    darray_free(set->modifiers);
    darray_free(set->printable);
    darray_free(set->misc);
}

स्थिर गणना xkb_status
add_pending_events(संरचना xkb_typing_state * restrict state,
                   ध्रुव संरचना xkb_pending_typing_event * restrict events,
                   size_t गणनम्)
{
    प्रति (size_t e = 0; e < गणनम्; e++) {
        /* Look for a free slot */
        संरचना xkb_pending_typing_event *pending;
        darray_size_t idx = 0;
        darray_enumerate_from(idx, pending, state->pending, idx) {
            जइ (!pending->counter) {
                darray_item(state->pending, idx) = events[e];
                गच्छ next;
            }
        }
        /* No free slot found: append new one */
        idx = darray_size(state->pending);
        darray_append(state->pending, events[e]);
    next:
        /* Avoid immediate consumption */
        darray_item(state->pending, idx).counter++;
    }
    state->अङ्कः_pending += गणनम्;

    प्रत्यागम XKB_SUCCESS;
}

स्थिर गणना xkb_status
next_pending_events(संरचना xkb_typing_state * restrict state,
                    xkb_typing_events * restrict out)
{
    /* Reset the output */
    darray_size(*out) = 0;

    darray_size_t idx;
    संरचना xkb_pending_typing_event *pending;
    /* Stack: start from the end */
    darray_enumerate_reverse(idx, pending, state->pending) {
        जइ (!pending->counter)
            अनुवर्त;
        pending->counter--;
        जइ (pending->counter)
            अनुवर्त;
        darray_append(*out, pending->event);
        state->अङ्कः_pending--;
        /* Resize array if it was the last element */
        जइ (idx + 1 == darray_size(state->pending))
            darray_size(state->pending)--;
        state->mods &= ~pending->mods;
    }

    प्रत्यागम XKB_SUCCESS;
}

गणना {
    /** Tries to avoid pressing a key already pressed */
    कुञ्चिका_CHOICE_MAX_TRIES = 12,
    XKB_ERROR_NO_RELEASED_कुञ्चिका = -1000,
};

स्थिर गणना xkb_status
add_modifier_कुञ्चिका(संरचना xkb_typing_state * restrict state, size_t * restrict e)
{
    /* Choose a random key */
    long idx = -1;
    प्रति (पूर्णाङ्क tries = 0; tries < कुञ्चिका_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state)
            % darray_size(state->कुञ्चिकाः->modifiers);
        /*
         * Ensure key is not already down and its modifier mask is not active,
         * i.e. to avoid simultaneous keys with the same modifier.
         */
        जइ (!bit_array_get(state->कुञ्चिकाः_state,
                           darray_item(state->कुञ्चिकाः->modifiers, idx).keycode) &&
            !(state->mods & darray_item(state->कुञ्चिकाः->modifiers, idx).mods))
        {
            भङ्ग;
        }
        idx = -1;
    }

    संरचना xkb_modifier_कुञ्चिका कुञ्चिका = {0};
    जइ (idx < 0) {
        /* No released key found: fall back to linear search */
        ध्रुव संरचना xkb_modifier_कुञ्चिका *कुञ्चिका_2;
        darray_foreach(कुञ्चिका_2, state->कुञ्चिकाः->modifiers) {
            /* See test doc above */
            जइ (!bit_array_get(state->कुञ्चिकाः_state, कुञ्चिका_2->keycode) &&
                !(state->mods & कुञ्चिका_2->mods))
            {
                कुञ्चिका = *कुञ्चिका_2;
                idx = 0;
                भङ्ग;
            }
        }
        जइ (idx < 0) {
            /* No fallback: skip */
            प्रत्यागम (गणना xkb_status)XKB_ERROR_NO_RELEASED_कुञ्चिका;
        }
    } अण्णहा {
        कुञ्चिका = darray_item(state->कुञ्चिकाः->modifiers, idx);
    }

    ध्रुव size_t remaining_events =
        state->अङ्कः_events - state->अङ्कः_pending - *e;
    size_t pending_turns = 0;

    जइ (remaining_events >= 2) {
        जइ (कुञ्चिका.kind == XKB_MOD_SET ||
            (कुञ्चिका.kind == XKB_MOD_LOCK && remaining_events >= 4))
        {
            /* Delay SetMods release or LockMods unlock */
            ध्रुव size_t max_turns = MIN(
                (size_t)(
                    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS -
                    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                 ),
                remaining_events + state->अङ्कः_pending
            );
            pending_turns = XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                          + (size_t)state->prng(state->prng_state)
                          % (max_turns + 1);
        }
    } अण्णहा {
        प्रत्यागम XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (संरचना xkb_typing_event) {
        .keycode = कुञ्चिका.keycode,
        .direction = XKB_कुञ्चिका_DOWN,
    };

    संरचना xkb_pending_typing_event pending[2] = {
        {
            .event = {
                .keycode = कुञ्चिका.keycode,
                .direction = XKB_कुञ्चिका_DOWN,
            },
            .counter = (uint8_t)pending_turns,
        },
        {
            .event = {
                .keycode = कुञ्चिका.keycode,
                .direction = XKB_कुञ्चिका_UP,
            },
            .mods = कुञ्चिका.mods,
            .counter = (uint8_t)pending_turns,
        },
    };

    जइ (pending_turns) {
        जइ (कुञ्चिका.kind == XKB_MOD_LOCK) {
            /* Immediate release */
            state->events[(*e)++] = pending[1].event;
            /* Delayed unlock (press + release) */
            pending[1].counter++;
            ध्रुव गणना xkb_status status =
                add_pending_events(state, pending, ARRAY_SIZE(pending));
            जइ (status != XKB_SUCCESS)
                प्रत्यागम status;
            /*
             * Register “locked” state, to avoid tapping the key before
             * delayed key released
             */
            bit_array_set(state->कुञ्चिकाः_state, कुञ्चिका.keycode);
        } अण्णहा {
            /* Delayed release */
            ध्रुव गणना xkb_status status =
                add_pending_events(state, &pending[1], 1);
            जइ (status != XKB_SUCCESS)
                प्रत्यागम status;
            /* Register pressed state */
            bit_array_set(state->कुञ्चिकाः_state, कुञ्चिका.keycode);
        }
        state->mods |= कुञ्चिका.mods;
    } अण्णहा {
        /* Immediate release */
        state->events[(*e)++] = pending[1].event;
        /* TODO: add latched modifier to mask state. */
    }

    प्रत्यागम XKB_SUCCESS;
}

स्थिर गणना xkb_status
add_other_कुञ्चिका(संरचना xkb_typing_state * restrict state,
              ध्रुव xkb_keycodes_t * restrict keycodes, size_t * restrict e,
              uint8_t immediate_release_prob, uint8_t max_pending_turns)
{
    /* Choose a random key */
    long idx = -1;
    प्रति (पूर्णाङ्क tries = 0; tries < कुञ्चिका_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state) % darray_size(*keycodes);
        /* Ensure key is not already down */
        जइ (!bit_array_get(state->कुञ्चिकाः_state,
                           darray_item(*keycodes, idx)))
        {
            भङ्ग;
        }
        idx = -1;
    }

    xkb_keycode_t keycode = XKB_KEYCODE_INVALID;
    जइ (idx < 0) {
        /* No released key found: fall back to linear search */
        ध्रुव xkb_keycode_t *keycode_;
        darray_foreach(keycode_, *keycodes) {
            जइ (!bit_array_get(state->कुञ्चिकाः_state, *keycode_)) {
                keycode = *keycode_;
                idx = 0;
                भङ्ग;
            }
        }
        जइ (idx < 0) {
            /* No fallback: skip */
            प्रत्यागम (गणना xkb_status)XKB_ERROR_NO_RELEASED_कुञ्चिका;
        }
    } अण्णहा {
        keycode = darray_item(*keycodes, idx);
    }

    ध्रुव size_t remaining_events =
        state->अङ्कः_events - state->अङ्कः_pending - *e;
    size_t pending_turns;

    जइ (remaining_events >= 2) {
        ध्रुव size_t max_turns = MIN(
            (size_t)max_pending_turns,
            remaining_events + state->अङ्कः_pending
        );
        ध्रुव bool immediate_release = (
            (immediate_release_prob >= 100) ||
            !max_turns ||
            (state->prng(state->prng_state) % 100) < immediate_release_prob
        );
        जइ (immediate_release) {
            pending_turns = 0;
        } अण्णहा {
            pending_turns = 1 + (size_t)state->prng(state->prng_state)
                          % max_turns;
        }
    } अण्णहा {
        प्रत्यागम XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (संरचना xkb_typing_event) {
        .keycode = keycode,
        .direction = XKB_कुञ्चिका_DOWN,
    };

    ध्रुव संरचना xkb_pending_typing_event pending = {
        .event = {
            .keycode = keycode,
            .direction = XKB_कुञ्चिका_UP,
        },
        .counter = (uint8_t)pending_turns,
    };
    जइ (pending_turns) {
        ध्रुव गणना xkb_status status = add_pending_events(state, &pending, 1);
        जइ (status != XKB_SUCCESS)
            प्रत्यागम status;
        /* Register pressed state */
        bit_array_set(state->कुञ्चिकाः_state, keycode);
    } अण्णहा {
        state->events[(*e)++] = pending.event;
    }

    प्रत्यागम XKB_SUCCESS;
}

स्थिर गणना xkb_कुञ्चिका_kind
pick_कुञ्चिका_kind(xkb_prng_t prng, शून्य *prng_state)
{
    स्थिर ध्रुव uint8_t weights[अङ्कः_XKB_कुञ्चिका_KIND] = {
        [XKB_कुञ्चिका_PRINTABLE] = 60,
        [XKB_कुञ्चिका_MODIFIER] = 30,
        [XKB_कुञ्चिका_MISC] = 10,
    };
    static_assert(XKB_कुञ्चिका_MISC == 2 &&
                  XKB_कुञ्चिका_MISC == अङ्कः_XKB_कुञ्चिका_KIND - 1, "");
    प्रतिज्ञा(weights[0] + weights[1] + weights[2] == 100);

    long r = prng(prng_state) % 100;
    प्रति (गणना xkb_कुञ्चिका_kind k = 0; k < अङ्कः_XKB_कुञ्चिका_KIND; k++)
        जइ ((r -= weights[k]) < 0)
            प्रत्यागम k;
    प्रत्यागम XKB_कुञ्चिका_PRINTABLE;
}

स्थिर शून्य
xkb_typing_state_destroy(संरचना xkb_typing_state *state)
{
    मुक्त(state->कुञ्चिकाः_state);
    darray_free(state->pending);
    मुक्त(state->events);
}

गणना xkb_status
xkb_typing_निवेशः_init(संरचना xkb_typing_निवेशः * restrict typing,
                      ध्रुव संरचना xkb_कुञ्चिका_set * restrict कुञ्चिकाः,
                      ध्रुव संरचना xkb_typing_निवेशः_config * restrict config)
{
    typing->events = रिक्त;
    typing->अङ्कः_events = 0;

    जइ (!config->निवेशः_length)
        प्रत्यागम XKB_ERROR_INVALID;

    जइ (darray_empty(कुञ्चिकाः->printable) ||
        darray_empty(कुञ्चिकाः->modifiers) ||
        darray_empty(कुञ्चिकाः->misc))
    {
        प्रत्यागम XKB_ERROR_INVALID;
    }

    गणना xkb_status status = XKB_SUCCESS;

    /* NOTE: arrays are sorted because they were built using the key iterator */
    xkb_keycode_t keycode_max = darray_last(कुञ्चिकाः->printable, 0);
    keycode_max = MAX(
        darray_last(कुञ्चिकाः->modifiers, (संरचना xkb_modifier_कुञ्चिका){0}).keycode,
        keycode_max
    );
    keycode_max = MAX(darray_last(कुञ्चिकाः->misc, 0), keycode_max);
    ध्रुव size_t कुञ्चिकाः_state_अङ्कः = bit_array_शब्दाः((size_t)keycode_max + 1);
    uintptr_t *कुञ्चिकाः_state = calloc(कुञ्चिकाः_state_अङ्कः, परिमाण(*कुञ्चिकाः_state));

    ध्रुव size_t अङ्कः_events = config->निवेशः_length;
    संरचना xkb_typing_event *events = calloc(अङ्कः_events, परिमाण(*events));

    संरचना xkb_typing_state state = {
        .कुञ्चिकाः = कुञ्चिकाः,
        .कुञ्चिकाः_state = कुञ्चिकाः_state,
        .prng = config->prng,
        .prng_state = config->prng_state,
        .events = events,
        .अङ्कः_events = अङ्कः_events,
        .pending = darray_new(),
        .अङ्कः_pending = 0,
    };

    xkb_typing_events pending = darray_new();

    जइ (!कुञ्चिकाः_state || !events) {
        status = XKB_ERROR_ALLOCATION_FAILURE;
        गच्छ error;
    }

    /*
     * Invariant: e + state.num_pending <= state.num_events
     */

    प्रति (size_t e = 0; e < अङ्कः_events;) {
        /*
         * Process pending events
         */
        status = next_pending_events(&state, &pending);
        जइ (status != XKB_SUCCESS) {
            गच्छ error;
        }
        संरचना xkb_typing_event *event;
        darray_foreach(event, pending) {
            जइ (e >= अङ्कः_events) {
                /* invariant violated: should be unreachable */
                PANIC_UNREACHABLE();
            }
            events[e] = *event;
            bit_array_assign(कुञ्चिकाः_state, event->keycode, (bool)event->direction);
            e++;
        }

        जइ (!state.अङ्कः_pending && e == अङ्कः_events - 1) {
            /*
             * Cannot add press/release *pair*, so just use an invalid keycode
             * for the last event.
             */
            events[e] = (संरचना xkb_typing_event) {
                .keycode = XKB_KEYCODE_INVALID,
                .direction = XKB_कुञ्चिका_DOWN,
            };
            भङ्ग;
        }

        जइ (e + 1 + state.अङ्कः_pending >= अङ्कः_events) {
            /* Cannot add new pair of events: require flushing pending events */
            अनुवर्त;
        }

        /*
         * Create new events
         */

        /* Choose kind of key */
        static_assert(XKB_कुञ्चिका_PRINTABLE == 0, "");
        ध्रुव गणना xkb_कुञ्चिका_kind कुञ्चिका_kind =
            pick_कुञ्चिका_kind(config->prng, config->prng_state);
        विकल्प (कुञ्चिका_kind) {
        पक्ष XKB_कुञ्चिका_PRINTABLE:
            status = add_other_कुञ्चिका(
                &state, &state.कुञ्चिकाः->printable, &e,
                XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS
            );
            भङ्ग;
        पक्ष XKB_कुञ्चिका_MODIFIER:
            status = add_modifier_कुञ्चिका(&state, &e);
            भङ्ग;
        पक्ष XKB_कुञ्चिका_MISC:
            status = add_other_कुञ्चिका(
                &state, &state.कुञ्चिकाः->misc, &e,
                XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS
            );
            भङ्ग;
        स्वभाव: {
            static_assert(XKB_कुञ्चिका_MISC == 2 &&
                          XKB_कुञ्चिका_MISC == अङ्कः_XKB_कुञ्चिका_KIND - 1, "");
            status = XKB_ERROR_INVALID;
        }}

        जइ (status != XKB_SUCCESS &&
            status != (गणना xkb_status)XKB_ERROR_NO_RELEASED_कुञ्चिका)
        {
            गच्छ error;
        }
    }

    प्रतिज्ञा(!state.अङ्कः_pending);

    typing->events = steal(&state.events);
    typing->अङ्कः_events = अङ्कः_events;

    status = XKB_SUCCESS;

error:
    darray_free(pending);
    xkb_typing_state_destroy(&state);

    प्रत्यागम status;
}

शून्य
xkb_typing_निवेशः_destroy(संरचना xkb_typing_निवेशः *typing)
{
    जइ (!typing)
        प्रत्यागम;

    मुक्त(typing->events);
    typing->events = रिक्त;
}
