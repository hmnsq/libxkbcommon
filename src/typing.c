/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdint.h>
#जर WCHAR_MAX >= 0x7FFFFFFF
#अन्तर्भाव <wctype.h>
#define USE_ISWPRINT
#जर HAVE_NEWLOCALE && HAVE_ISWPRINT_L
#define USE_ISWPRINT_L
#अन्तर्भाव <locale.h>
#endif
#अन्यथा
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
xkb_किल्ली_set_init(संरचना xkb_किल्ली_set *set,
                 संरचना xkb_keymap *keymap,
                 स्थिरांक संरचना xkb_किल्ली_set_config *config)
{
    darray_init(set->printable);
    darray_init(set->modifiers);
    darray_init(set->misc);

    स्थिर स्थिरांक संरचना xkb_किल्ली_set_config default_config = {0};
    जर (!config)
        config = &default_config;
    /* keycode = 0 denotes “default” */
    स्थिरांक xkb_keycode_t min = MAX(config->min, xkb_keymap_min_keycode(keymap));
    स्थिरांक xkb_keycode_t max = config->max
        ? MIN(config->max, xkb_keymap_max_keycode(keymap))
        : xkb_keymap_max_keycode(keymap);

    गणना xkb_status status;
    संरचना xkb_state *state =
        xkb_state_new_with_mode(keymap, XKB_STATE_MODE_SERVER, &status);
    जर (!state)
        परत status;

    #ifdef USE_ISWPRINT_L
    locale_t loc = newlocale(LC_ALL_MASK, "C.UTF-8", (locale_t)0);
    #endif

    संरचना xkb_keymap_किल्ली_iterator iter;
    स्थिरांक संरचना xkb_keymap_किल्ली_iterator_config iter_config = {
        .size = आकार(iter_config),
        .flags = XKB_KEYMAP_किल्ली_ITERATOR_NO_FLAGS,
        .start = min
    };
    status = xkb_keymap_किल्ली_iterator_init(&iter, keymap, &iter_config);
    जर (status != XKB_SUCCESS)
        जा error;

    स्थिरांक xkb_mod_mask_t numlock =
        xkb_keymap_mod_get_mask(keymap, XKB_VMOD_NAME_अंक);

    स्थिरांक संरचना xkb_state_components_update components_update = {
        .size = आकार(components_update),
        .components = XKB_STATE_MODS_LOCKED,
        .affect_locked_mods = numlock,
        .locked_mods = numlock,
    };

    स्थिरांक संरचना xkb_synthetic_update update = {
        .size = आकार(update),
        .components = &components_update,
    };

    xkb_keycode_t k;
    पर्यंत ((k = xkb_keymap_किल्ली_iterator_next(&iter)) != XKB_KEYCODE_INVALID) {
        जर (k > max)
            थांबा;

        xkb_state_reset(state);
        status = xkb_state_update_synthetic(state, &update, रिक्त);
        जर (status != XKB_SUCCESS)
            जा error;

        स्थिरांक xkb_keysym_t *syms = रिक्त;
        स्थिरांक पूर्णांक मोजणी = xkb_state_किल्ली_get_syms(state, k, &syms);

        गणना xkb_state_component changed =
            xkb_state_update_किल्ली(state, k, XKB_किल्ली_DOWN);
        xkb_mod_mask_t mods =
            xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);
        changed |= xkb_state_update_किल्ली(state, k, XKB_किल्ली_UP);
        mods |= xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);

        जर (changed & XKB_STATE_MODS_EFFECTIVE) {
            /* Modifier key */
            गणना xkb_modifier_kind kind;
            जर (changed & XKB_STATE_MODS_LOCKED) {
                kind = XKB_MOD_LOCK;
            } अन्यथा जर (changed & XKB_STATE_MODS_LATCHED) {
                kind = XKB_MOD_LATCH;
            } अन्यथा {
                kind = XKB_MOD_SET;
            }
            mods &= ~numlock;
            जर (!mods)
                mods = numlock;
            darray_append(set->modifiers, (संरचना xkb_modifier_किल्ली) {
                .keycode = k,
                .mods = mods,
                .kind = kind,
            });
            सुरूठेवा;
        }

        जर (मोजणी > 0) {
            uint32_t utf32 = 0;
            साठी (पूर्णांक s = 0; s < मोजणी; s++) {
                utf32 = xkb_keysym_to_utf32(syms[s]);
                स्थिरांक bool is_printable =
                    #ifdef USE_ISWPRINT_L
                    ((loc && iswprint_l((wint_t)utf32, loc)) || iswprint((wint_t)utf32))
                    #किंवाजर defined(USE_ISWPRINT)
                    iswprint((wint_t)utf32)
                    #अन्यथा
                    utf32 <= 0x7f && है_छाप((पूर्णांक)utf32)
                    #endif
                    ;
                जर (is_printable)
                    थांबा;
                utf32 = 0;
            }
            जर (utf32) {
                /* Printable keysyms */
                darray_append(set->printable, k);
                सुरूठेवा;
            }
        }

        /* Miscellaneous key */
        darray_append(set->misc, k);
    }

error:
    #ifdef USE_ISWPRINT_L
    जर (loc)
        freelocale(loc);
    #endif

    xkb_state_unref(state);
    जर (status != XKB_SUCCESS)
        xkb_किल्ली_set_destroy(set);

    परत status;
}

शून्य
xkb_किल्ली_set_destroy(संरचना xkb_किल्ली_set *set)
{
    जर (!set)
        परत;

    darray_free(set->modifiers);
    darray_free(set->printable);
    darray_free(set->misc);
}

स्थिर गणना xkb_status
add_pending_events(संरचना xkb_typing_state * restrict state,
                   स्थिरांक संरचना xkb_pending_typing_event * restrict events,
                   माप_प्रकार मोजणी)
{
    साठी (माप_प्रकार e = 0; e < मोजणी; e++) {
        /* Look for a free slot */
        संरचना xkb_pending_typing_event *pending;
        darray_size_t idx = 0;
        darray_enumerate_from(idx, pending, state->pending, idx) {
            जर (!pending->counter) {
                darray_item(state->pending, idx) = events[e];
                जा next;
            }
        }
        /* No free slot found: append new one */
        idx = darray_size(state->pending);
        darray_append(state->pending, events[e]);
    next:
        /* Avoid immediate consumption */
        darray_item(state->pending, idx).counter++;
    }
    state->अंक_pending += मोजणी;

    परत XKB_SUCCESS;
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
        जर (!pending->counter)
            सुरूठेवा;
        pending->counter--;
        जर (pending->counter)
            सुरूठेवा;
        darray_append(*out, pending->event);
        state->अंक_pending--;
        /* Resize array if it was the last element */
        जर (idx + 1 == darray_size(state->pending))
            darray_size(state->pending)--;
        state->mods &= ~pending->mods;
    }

    परत XKB_SUCCESS;
}

गणना {
    /** Tries to avoid pressing a key already pressed */
    किल्ली_CHOICE_MAX_TRIES = 12,
    XKB_ERROR_NO_RELEASED_किल्ली = -1000,
};

स्थिर गणना xkb_status
add_modifier_किल्ली(संरचना xkb_typing_state * restrict state, माप_प्रकार * restrict e)
{
    /* Choose a random key */
    दीर्घ idx = -1;
    साठी (पूर्णांक tries = 0; tries < किल्ली_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state)
            % darray_size(state->किल्ल्या->modifiers);
        /*
         * Ensure key is not already down and its modifier mask is not active,
         * i.e. to avoid simultaneous keys with the same modifier.
         */
        जर (!bit_array_get(state->किल्ल्या_state,
                           darray_item(state->किल्ल्या->modifiers, idx).keycode) &&
            !(state->mods & darray_item(state->किल्ल्या->modifiers, idx).mods))
        {
            थांबा;
        }
        idx = -1;
    }

    संरचना xkb_modifier_किल्ली किल्ली = {0};
    जर (idx < 0) {
        /* No released key found: fall back to linear search */
        स्थिरांक संरचना xkb_modifier_किल्ली *किल्ली_2;
        darray_foreach(किल्ली_2, state->किल्ल्या->modifiers) {
            /* See test doc above */
            जर (!bit_array_get(state->किल्ल्या_state, किल्ली_2->keycode) &&
                !(state->mods & किल्ली_2->mods))
            {
                किल्ली = *किल्ली_2;
                idx = 0;
                थांबा;
            }
        }
        जर (idx < 0) {
            /* No fallback: skip */
            परत (गणना xkb_status)XKB_ERROR_NO_RELEASED_किल्ली;
        }
    } अन्यथा {
        किल्ली = darray_item(state->किल्ल्या->modifiers, idx);
    }

    स्थिरांक माप_प्रकार remaining_events =
        state->अंक_events - state->अंक_pending - *e;
    माप_प्रकार pending_turns = 0;

    जर (remaining_events >= 2) {
        जर (किल्ली.kind == XKB_MOD_SET ||
            (किल्ली.kind == XKB_MOD_LOCK && remaining_events >= 4))
        {
            /* Delay SetMods release or LockMods unlock */
            स्थिरांक माप_प्रकार max_turns = MIN(
                (माप_प्रकार)(
                    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS -
                    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                 ),
                remaining_events + state->अंक_pending
            );
            pending_turns = XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                          + (माप_प्रकार)state->prng(state->prng_state)
                          % (max_turns + 1);
        }
    } अन्यथा {
        परत XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (संरचना xkb_typing_event) {
        .keycode = किल्ली.keycode,
        .direction = XKB_किल्ली_DOWN,
    };

    संरचना xkb_pending_typing_event pending[2] = {
        {
            .event = {
                .keycode = किल्ली.keycode,
                .direction = XKB_किल्ली_DOWN,
            },
            .counter = (uint8_t)pending_turns,
        },
        {
            .event = {
                .keycode = किल्ली.keycode,
                .direction = XKB_किल्ली_UP,
            },
            .mods = किल्ली.mods,
            .counter = (uint8_t)pending_turns,
        },
    };

    जर (pending_turns) {
        जर (किल्ली.kind == XKB_MOD_LOCK) {
            /* Immediate release */
            state->events[(*e)++] = pending[1].event;
            /* Delayed unlock (press + release) */
            pending[1].counter++;
            स्थिरांक गणना xkb_status status =
                add_pending_events(state, pending, ARRAY_SIZE(pending));
            जर (status != XKB_SUCCESS)
                परत status;
            /*
             * Register “locked” state, to avoid tapping the key before
             * delayed key released
             */
            bit_array_set(state->किल्ल्या_state, किल्ली.keycode);
        } अन्यथा {
            /* Delayed release */
            स्थिरांक गणना xkb_status status =
                add_pending_events(state, &pending[1], 1);
            जर (status != XKB_SUCCESS)
                परत status;
            /* Register pressed state */
            bit_array_set(state->किल्ल्या_state, किल्ली.keycode);
        }
        state->mods |= किल्ली.mods;
    } अन्यथा {
        /* Immediate release */
        state->events[(*e)++] = pending[1].event;
        /* TODO: add latched modifier to mask state. */
    }

    परत XKB_SUCCESS;
}

स्थिर गणना xkb_status
add_other_किल्ली(संरचना xkb_typing_state * restrict state,
              स्थिरांक xkb_keycodes_t * restrict keycodes, माप_प्रकार * restrict e,
              uint8_t immediate_release_prob, uint8_t max_pending_turns)
{
    /* Choose a random key */
    दीर्घ idx = -1;
    साठी (पूर्णांक tries = 0; tries < किल्ली_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state) % darray_size(*keycodes);
        /* Ensure key is not already down */
        जर (!bit_array_get(state->किल्ल्या_state,
                           darray_item(*keycodes, idx)))
        {
            थांबा;
        }
        idx = -1;
    }

    xkb_keycode_t keycode = XKB_KEYCODE_INVALID;
    जर (idx < 0) {
        /* No released key found: fall back to linear search */
        स्थिरांक xkb_keycode_t *keycode_;
        darray_foreach(keycode_, *keycodes) {
            जर (!bit_array_get(state->किल्ल्या_state, *keycode_)) {
                keycode = *keycode_;
                idx = 0;
                थांबा;
            }
        }
        जर (idx < 0) {
            /* No fallback: skip */
            परत (गणना xkb_status)XKB_ERROR_NO_RELEASED_किल्ली;
        }
    } अन्यथा {
        keycode = darray_item(*keycodes, idx);
    }

    स्थिरांक माप_प्रकार remaining_events =
        state->अंक_events - state->अंक_pending - *e;
    माप_प्रकार pending_turns;

    जर (remaining_events >= 2) {
        स्थिरांक माप_प्रकार max_turns = MIN(
            (माप_प्रकार)max_pending_turns,
            remaining_events + state->अंक_pending
        );
        स्थिरांक bool immediate_release = (
            (immediate_release_prob >= 100) ||
            !max_turns ||
            (state->prng(state->prng_state) % 100) < immediate_release_prob
        );
        जर (immediate_release) {
            pending_turns = 0;
        } अन्यथा {
            pending_turns = 1 + (माप_प्रकार)state->prng(state->prng_state)
                          % max_turns;
        }
    } अन्यथा {
        परत XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (संरचना xkb_typing_event) {
        .keycode = keycode,
        .direction = XKB_किल्ली_DOWN,
    };

    स्थिरांक संरचना xkb_pending_typing_event pending = {
        .event = {
            .keycode = keycode,
            .direction = XKB_किल्ली_UP,
        },
        .counter = (uint8_t)pending_turns,
    };
    जर (pending_turns) {
        स्थिरांक गणना xkb_status status = add_pending_events(state, &pending, 1);
        जर (status != XKB_SUCCESS)
            परत status;
        /* Register pressed state */
        bit_array_set(state->किल्ल्या_state, keycode);
    } अन्यथा {
        state->events[(*e)++] = pending.event;
    }

    परत XKB_SUCCESS;
}

स्थिर गणना xkb_किल्ली_kind
pick_किल्ली_kind(xkb_prng_t prng, शून्य *prng_state)
{
    स्थिर स्थिरांक uint8_t weights[अंक_XKB_किल्ली_KIND] = {
        [XKB_किल्ली_PRINTABLE] = 60,
        [XKB_किल्ली_MODIFIER] = 30,
        [XKB_किल्ली_MISC] = 10,
    };
    static_assert(XKB_किल्ली_MISC == 2 &&
                  XKB_किल्ली_MISC == अंक_XKB_किल्ली_KIND - 1, "");
    प्रतिज्ञा(weights[0] + weights[1] + weights[2] == 100);

    दीर्घ r = prng(prng_state) % 100;
    साठी (गणना xkb_किल्ली_kind k = 0; k < अंक_XKB_किल्ली_KIND; k++)
        जर ((r -= weights[k]) < 0)
            परत k;
    परत XKB_किल्ली_PRINTABLE;
}

स्थिर शून्य
xkb_typing_state_destroy(संरचना xkb_typing_state *state)
{
    मुक्त(state->किल्ल्या_state);
    darray_free(state->pending);
    मुक्त(state->events);
}

गणना xkb_status
xkb_typing_आदान_init(संरचना xkb_typing_आदान * restrict typing,
                      स्थिरांक संरचना xkb_किल्ली_set * restrict किल्ल्या,
                      स्थिरांक संरचना xkb_typing_आदान_config * restrict config)
{
    typing->events = रिक्त;
    typing->अंक_events = 0;

    जर (!config->आदान_length)
        परत XKB_ERROR_INVALID;

    जर (darray_empty(किल्ल्या->printable) ||
        darray_empty(किल्ल्या->modifiers) ||
        darray_empty(किल्ल्या->misc))
    {
        परत XKB_ERROR_INVALID;
    }

    गणना xkb_status status = XKB_SUCCESS;

    /* NOTE: arrays are sorted because they were built using the key iterator */
    xkb_keycode_t keycode_max = darray_last(किल्ल्या->printable, 0);
    keycode_max = MAX(
        darray_last(किल्ल्या->modifiers, (संरचना xkb_modifier_किल्ली){0}).keycode,
        keycode_max
    );
    keycode_max = MAX(darray_last(किल्ल्या->misc, 0), keycode_max);
    स्थिरांक माप_प्रकार किल्ल्या_state_अंक = bit_array_शब्द((माप_प्रकार)keycode_max + 1);
    uintptr_t *किल्ल्या_state = सुस्मृति(किल्ल्या_state_अंक, आकार(*किल्ल्या_state));

    स्थिरांक माप_प्रकार अंक_events = config->आदान_length;
    संरचना xkb_typing_event *events = सुस्मृति(अंक_events, आकार(*events));

    संरचना xkb_typing_state state = {
        .किल्ल्या = किल्ल्या,
        .किल्ल्या_state = किल्ल्या_state,
        .prng = config->prng,
        .prng_state = config->prng_state,
        .events = events,
        .अंक_events = अंक_events,
        .pending = darray_new(),
        .अंक_pending = 0,
    };

    xkb_typing_events pending = darray_new();

    जर (!किल्ल्या_state || !events) {
        status = XKB_ERROR_ALLOCATION_FAILURE;
        जा error;
    }

    /*
     * Invariant: e + state.num_pending <= state.num_events
     */

    साठी (माप_प्रकार e = 0; e < अंक_events;) {
        /*
         * Process pending events
         */
        status = next_pending_events(&state, &pending);
        जर (status != XKB_SUCCESS) {
            जा error;
        }
        संरचना xkb_typing_event *event;
        darray_foreach(event, pending) {
            जर (e >= अंक_events) {
                /* invariant violated: should be unreachable */
                PANIC_UNREACHABLE();
            }
            events[e] = *event;
            bit_array_assign(किल्ल्या_state, event->keycode, (bool)event->direction);
            e++;
        }

        जर (!state.अंक_pending && e == अंक_events - 1) {
            /*
             * Cannot add press/release *pair*, so just use an invalid keycode
             * for the last event.
             */
            events[e] = (संरचना xkb_typing_event) {
                .keycode = XKB_KEYCODE_INVALID,
                .direction = XKB_किल्ली_DOWN,
            };
            थांबा;
        }

        जर (e + 1 + state.अंक_pending >= अंक_events) {
            /* Cannot add new pair of events: require flushing pending events */
            सुरूठेवा;
        }

        /*
         * Create new events
         */

        /* Choose kind of key */
        static_assert(XKB_किल्ली_PRINTABLE == 0, "");
        स्थिरांक गणना xkb_किल्ली_kind किल्ली_kind =
            pick_किल्ली_kind(config->prng, config->prng_state);
        निवड (किल्ली_kind) {
        स्थिती XKB_किल्ली_PRINTABLE:
            status = add_other_किल्ली(
                &state, &state.किल्ल्या->printable, &e,
                XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS
            );
            थांबा;
        स्थिती XKB_किल्ली_MODIFIER:
            status = add_modifier_किल्ली(&state, &e);
            थांबा;
        स्थिती XKB_किल्ली_MISC:
            status = add_other_किल्ली(
                &state, &state.किल्ल्या->misc, &e,
                XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS
            );
            थांबा;
        पूर्वनिर्धारित: {
            static_assert(XKB_किल्ली_MISC == 2 &&
                          XKB_किल्ली_MISC == अंक_XKB_किल्ली_KIND - 1, "");
            status = XKB_ERROR_INVALID;
        }}

        जर (status != XKB_SUCCESS &&
            status != (गणना xkb_status)XKB_ERROR_NO_RELEASED_किल्ली)
        {
            जा error;
        }
    }

    प्रतिज्ञा(!state.अंक_pending);

    typing->events = steal(&state.events);
    typing->अंक_events = अंक_events;

    status = XKB_SUCCESS;

error:
    darray_free(pending);
    xkb_typing_state_destroy(&state);

    परत status;
}

शून्य
xkb_typing_आदान_destroy(संरचना xkb_typing_आदान *typing)
{
    जर (!typing)
        परत;

    मुक्त(typing->events);
    typing->events = रिक्त;
}
