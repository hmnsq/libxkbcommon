/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdint.h>
#यदि WCHAR_MAX >= 0x7FFFFFFF
#अन्तर्भाव <wctype.h>
#define USE_ISWPRINT
#यदि HAVE_NEWLOCALE && HAVE_ISWPRINT_L
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
                 ध्रुव संरचना xkb_किल्ली_set_config *config)
{
    darray_init(set->printable);
    darray_init(set->modifiers);
    darray_init(set->misc);

    स्थिर ध्रुव संरचना xkb_किल्ली_set_config default_config = {0};
    यदि (!config)
        config = &default_config;
    /* keycode = 0 denotes “default” */
    ध्रुव xkb_keycode_t min = MAX(config->min, xkb_keymap_min_keycode(keymap));
    ध्रुव xkb_keycode_t max = config->max
        ? MIN(config->max, xkb_keymap_max_keycode(keymap))
        : xkb_keymap_max_keycode(keymap);

    गणना xkb_status status;
    संरचना xkb_state *state =
        xkb_state_new_with_mode(keymap, XKB_STATE_MODE_SERVER, &status);
    यदि (!state)
        प्रत्यागम status;

    #ifdef USE_ISWPRINT_L
    locale_t loc = newlocale(LC_ALL_MASK, "C.UTF-8", (locale_t)0);
    #endif

    संरचना xkb_keymap_किल्ली_iterator iter;
    ध्रुव संरचना xkb_keymap_किल्ली_iterator_config iter_config = {
        .size = परिमाण(iter_config),
        .flags = XKB_KEYMAP_किल्ली_ITERATOR_NO_FLAGS,
        .start = min
    };
    status = xkb_keymap_किल्ली_iterator_init(&iter, keymap, &iter_config);
    यदि (status != XKB_SUCCESS)
        गच्छ error;

    ध्रुव xkb_mod_mask_t numlock =
        xkb_keymap_mod_get_mask(keymap, XKB_VMOD_NAME_अंक);

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
    यावत् ((k = xkb_keymap_किल्ली_iterator_next(&iter)) != XKB_KEYCODE_INVALID) {
        यदि (k > max)
            भङ्ग;

        xkb_state_reset(state);
        status = xkb_state_update_synthetic(state, &update, रिक्त);
        यदि (status != XKB_SUCCESS)
            गच्छ error;

        ध्रुव xkb_keysym_t *syms = रिक्त;
        ध्रुव पूर्णाङ्क मोजणी = xkb_state_किल्ली_get_syms(state, k, &syms);

        गणना xkb_state_component changed =
            xkb_state_update_किल्ली(state, k, XKB_किल्ली_DOWN);
        xkb_mod_mask_t mods =
            xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);
        changed |= xkb_state_update_किल्ली(state, k, XKB_किल्ली_UP);
        mods |= xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);

        यदि (changed & XKB_STATE_MODS_EFFECTIVE) {
            /* Modifier key */
            गणना xkb_modifier_kind kind;
            यदि (changed & XKB_STATE_MODS_LOCKED) {
                kind = XKB_MOD_LOCK;
            } अन्यथा यदि (changed & XKB_STATE_MODS_LATCHED) {
                kind = XKB_MOD_LATCH;
            } अन्यथा {
                kind = XKB_MOD_SET;
            }
            mods &= ~numlock;
            यदि (!mods)
                mods = numlock;
            darray_append(set->modifiers, (संरचना xkb_modifier_किल्ली) {
                .keycode = k,
                .mods = mods,
                .kind = kind,
            });
            अनुवर्त;
        }

        यदि (मोजणी > 0) {
            uint32_t utf32 = 0;
            प्रति (पूर्णाङ्क s = 0; s < मोजणी; s++) {
                utf32 = xkb_keysym_to_utf32(syms[s]);
                ध्रुव bool is_printable =
                    #ifdef USE_ISWPRINT_L
                    ((loc && iswprint_l((wint_t)utf32, loc)) || iswprint((wint_t)utf32))
                    #नोचेद्यदि defined(USE_ISWPRINT)
                    iswprint((wint_t)utf32)
                    #अन्यथा
                    utf32 <= 0x7f && isprint((पूर्णाङ्क)utf32)
                    #endif
                    ;
                यदि (is_printable)
                    भङ्ग;
                utf32 = 0;
            }
            यदि (utf32) {
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
    यदि (loc)
        freelocale(loc);
    #endif

    xkb_state_unref(state);
    यदि (status != XKB_SUCCESS)
        xkb_किल्ली_set_destroy(set);

    प्रत्यागम status;
}

शून्य
xkb_किल्ली_set_destroy(संरचना xkb_किल्ली_set *set)
{
    यदि (!set)
        प्रत्यागम;

    darray_free(set->modifiers);
    darray_free(set->printable);
    darray_free(set->misc);
}

स्थिर गणना xkb_status
add_pending_events(संरचना xkb_typing_state * restrict state,
                   ध्रुव संरचना xkb_pending_typing_event * restrict events,
                   size_t मोजणी)
{
    प्रति (size_t e = 0; e < मोजणी; e++) {
        /* Look for a free slot */
        संरचना xkb_pending_typing_event *pending;
        darray_size_t idx = 0;
        darray_enumerate_from(idx, pending, state->pending, idx) {
            यदि (!pending->counter) {
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
    state->अंक_pending += मोजणी;

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
        यदि (!pending->counter)
            अनुवर्त;
        pending->counter--;
        यदि (pending->counter)
            अनुवर्त;
        darray_append(*out, pending->event);
        state->अंक_pending--;
        /* Resize array if it was the last element */
        यदि (idx + 1 == darray_size(state->pending))
            darray_size(state->pending)--;
        state->mods &= ~pending->mods;
    }

    प्रत्यागम XKB_SUCCESS;
}

गणना {
    /** Tries to avoid pressing a key already pressed */
    किल्ली_CHOICE_MAX_TRIES = 12,
    XKB_ERROR_NO_RELEASED_किल्ली = -1000,
};

स्थिर गणना xkb_status
add_modifier_किल्ली(संरचना xkb_typing_state * restrict state, size_t * restrict e)
{
    /* Choose a random key */
    long idx = -1;
    प्रति (पूर्णाङ्क tries = 0; tries < किल्ली_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state)
            % darray_size(state->किल्ल्या->modifiers);
        /*
         * Ensure key is not already down and its modifier mask is not active,
         * i.e. to avoid simultaneous keys with the same modifier.
         */
        यदि (!bit_array_get(state->किल्ल्या_state,
                           darray_item(state->किल्ल्या->modifiers, idx).keycode) &&
            !(state->mods & darray_item(state->किल्ल्या->modifiers, idx).mods))
        {
            भङ्ग;
        }
        idx = -1;
    }

    संरचना xkb_modifier_किल्ली किल्ली = {0};
    यदि (idx < 0) {
        /* No released key found: fall back to linear search */
        ध्रुव संरचना xkb_modifier_किल्ली *किल्ली_2;
        darray_foreach(किल्ली_2, state->किल्ल्या->modifiers) {
            /* See test doc above */
            यदि (!bit_array_get(state->किल्ल्या_state, किल्ली_2->keycode) &&
                !(state->mods & किल्ली_2->mods))
            {
                किल्ली = *किल्ली_2;
                idx = 0;
                भङ्ग;
            }
        }
        यदि (idx < 0) {
            /* No fallback: skip */
            प्रत्यागम (गणना xkb_status)XKB_ERROR_NO_RELEASED_किल्ली;
        }
    } अन्यथा {
        किल्ली = darray_item(state->किल्ल्या->modifiers, idx);
    }

    ध्रुव size_t remaining_events =
        state->अंक_events - state->अंक_pending - *e;
    size_t pending_turns = 0;

    यदि (remaining_events >= 2) {
        यदि (किल्ली.kind == XKB_MOD_SET ||
            (किल्ली.kind == XKB_MOD_LOCK && remaining_events >= 4))
        {
            /* Delay SetMods release or LockMods unlock */
            ध्रुव size_t max_turns = MIN(
                (size_t)(
                    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS -
                    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                 ),
                remaining_events + state->अंक_pending
            );
            pending_turns = XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                          + (size_t)state->prng(state->prng_state)
                          % (max_turns + 1);
        }
    } अन्यथा {
        प्रत्यागम XKB_ERROR_INVALID;
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

    यदि (pending_turns) {
        यदि (किल्ली.kind == XKB_MOD_LOCK) {
            /* Immediate release */
            state->events[(*e)++] = pending[1].event;
            /* Delayed unlock (press + release) */
            pending[1].counter++;
            ध्रुव गणना xkb_status status =
                add_pending_events(state, pending, ARRAY_SIZE(pending));
            यदि (status != XKB_SUCCESS)
                प्रत्यागम status;
            /*
             * Register “locked” state, to avoid tapping the key before
             * delayed key released
             */
            bit_array_set(state->किल्ल्या_state, किल्ली.keycode);
        } अन्यथा {
            /* Delayed release */
            ध्रुव गणना xkb_status status =
                add_pending_events(state, &pending[1], 1);
            यदि (status != XKB_SUCCESS)
                प्रत्यागम status;
            /* Register pressed state */
            bit_array_set(state->किल्ल्या_state, किल्ली.keycode);
        }
        state->mods |= किल्ली.mods;
    } अन्यथा {
        /* Immediate release */
        state->events[(*e)++] = pending[1].event;
        /* TODO: add latched modifier to mask state. */
    }

    प्रत्यागम XKB_SUCCESS;
}

स्थिर गणना xkb_status
add_other_किल्ली(संरचना xkb_typing_state * restrict state,
              ध्रुव xkb_keycodes_t * restrict keycodes, size_t * restrict e,
              uint8_t immediate_release_prob, uint8_t max_pending_turns)
{
    /* Choose a random key */
    long idx = -1;
    प्रति (पूर्णाङ्क tries = 0; tries < किल्ली_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state) % darray_size(*keycodes);
        /* Ensure key is not already down */
        यदि (!bit_array_get(state->किल्ल्या_state,
                           darray_item(*keycodes, idx)))
        {
            भङ्ग;
        }
        idx = -1;
    }

    xkb_keycode_t keycode = XKB_KEYCODE_INVALID;
    यदि (idx < 0) {
        /* No released key found: fall back to linear search */
        ध्रुव xkb_keycode_t *keycode_;
        darray_foreach(keycode_, *keycodes) {
            यदि (!bit_array_get(state->किल्ल्या_state, *keycode_)) {
                keycode = *keycode_;
                idx = 0;
                भङ्ग;
            }
        }
        यदि (idx < 0) {
            /* No fallback: skip */
            प्रत्यागम (गणना xkb_status)XKB_ERROR_NO_RELEASED_किल्ली;
        }
    } अन्यथा {
        keycode = darray_item(*keycodes, idx);
    }

    ध्रुव size_t remaining_events =
        state->अंक_events - state->अंक_pending - *e;
    size_t pending_turns;

    यदि (remaining_events >= 2) {
        ध्रुव size_t max_turns = MIN(
            (size_t)max_pending_turns,
            remaining_events + state->अंक_pending
        );
        ध्रुव bool immediate_release = (
            (immediate_release_prob >= 100) ||
            !max_turns ||
            (state->prng(state->prng_state) % 100) < immediate_release_prob
        );
        यदि (immediate_release) {
            pending_turns = 0;
        } अन्यथा {
            pending_turns = 1 + (size_t)state->prng(state->prng_state)
                          % max_turns;
        }
    } अन्यथा {
        प्रत्यागम XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (संरचना xkb_typing_event) {
        .keycode = keycode,
        .direction = XKB_किल्ली_DOWN,
    };

    ध्रुव संरचना xkb_pending_typing_event pending = {
        .event = {
            .keycode = keycode,
            .direction = XKB_किल्ली_UP,
        },
        .counter = (uint8_t)pending_turns,
    };
    यदि (pending_turns) {
        ध्रुव गणना xkb_status status = add_pending_events(state, &pending, 1);
        यदि (status != XKB_SUCCESS)
            प्रत्यागम status;
        /* Register pressed state */
        bit_array_set(state->किल्ल्या_state, keycode);
    } अन्यथा {
        state->events[(*e)++] = pending.event;
    }

    प्रत्यागम XKB_SUCCESS;
}

स्थिर गणना xkb_किल्ली_kind
pick_किल्ली_kind(xkb_prng_t prng, शून्य *prng_state)
{
    स्थिर ध्रुव uint8_t weights[अंक_XKB_किल्ली_KIND] = {
        [XKB_किल्ली_PRINTABLE] = 60,
        [XKB_किल्ली_MODIFIER] = 30,
        [XKB_किल्ली_MISC] = 10,
    };
    static_assert(XKB_किल्ली_MISC == 2 &&
                  XKB_किल्ली_MISC == अंक_XKB_किल्ली_KIND - 1, "");
    प्रतिज्ञा(weights[0] + weights[1] + weights[2] == 100);

    long r = prng(prng_state) % 100;
    प्रति (गणना xkb_किल्ली_kind k = 0; k < अंक_XKB_किल्ली_KIND; k++)
        यदि ((r -= weights[k]) < 0)
            प्रत्यागम k;
    प्रत्यागम XKB_किल्ली_PRINTABLE;
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
                      ध्रुव संरचना xkb_किल्ली_set * restrict किल्ल्या,
                      ध्रुव संरचना xkb_typing_आदान_config * restrict config)
{
    typing->events = रिक्त;
    typing->अंक_events = 0;

    यदि (!config->आदान_length)
        प्रत्यागम XKB_ERROR_INVALID;

    यदि (darray_empty(किल्ल्या->printable) ||
        darray_empty(किल्ल्या->modifiers) ||
        darray_empty(किल्ल्या->misc))
    {
        प्रत्यागम XKB_ERROR_INVALID;
    }

    गणना xkb_status status = XKB_SUCCESS;

    /* NOTE: arrays are sorted because they were built using the key iterator */
    xkb_keycode_t keycode_max = darray_last(किल्ल्या->printable, 0);
    keycode_max = MAX(
        darray_last(किल्ल्या->modifiers, (संरचना xkb_modifier_किल्ली){0}).keycode,
        keycode_max
    );
    keycode_max = MAX(darray_last(किल्ल्या->misc, 0), keycode_max);
    ध्रुव size_t किल्ल्या_state_अंक = bit_array_शब्द((size_t)keycode_max + 1);
    uintptr_t *किल्ल्या_state = calloc(किल्ल्या_state_अंक, परिमाण(*किल्ल्या_state));

    ध्रुव size_t अंक_events = config->आदान_length;
    संरचना xkb_typing_event *events = calloc(अंक_events, परिमाण(*events));

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

    यदि (!किल्ल्या_state || !events) {
        status = XKB_ERROR_ALLOCATION_FAILURE;
        गच्छ error;
    }

    /*
     * Invariant: e + state.num_pending <= state.num_events
     */

    प्रति (size_t e = 0; e < अंक_events;) {
        /*
         * Process pending events
         */
        status = next_pending_events(&state, &pending);
        यदि (status != XKB_SUCCESS) {
            गच्छ error;
        }
        संरचना xkb_typing_event *event;
        darray_foreach(event, pending) {
            यदि (e >= अंक_events) {
                /* invariant violated: should be unreachable */
                PANIC_UNREACHABLE();
            }
            events[e] = *event;
            bit_array_assign(किल्ल्या_state, event->keycode, (bool)event->direction);
            e++;
        }

        यदि (!state.अंक_pending && e == अंक_events - 1) {
            /*
             * Cannot add press/release *pair*, so just use an invalid keycode
             * for the last event.
             */
            events[e] = (संरचना xkb_typing_event) {
                .keycode = XKB_KEYCODE_INVALID,
                .direction = XKB_किल्ली_DOWN,
            };
            भङ्ग;
        }

        यदि (e + 1 + state.अंक_pending >= अंक_events) {
            /* Cannot add new pair of events: require flushing pending events */
            अनुवर्त;
        }

        /*
         * Create new events
         */

        /* Choose kind of key */
        static_assert(XKB_किल्ली_PRINTABLE == 0, "");
        ध्रुव गणना xkb_किल्ली_kind किल्ली_kind =
            pick_किल्ली_kind(config->prng, config->prng_state);
        विकल्प (किल्ली_kind) {
        पक्ष XKB_किल्ली_PRINTABLE:
            status = add_other_किल्ली(
                &state, &state.किल्ल्या->printable, &e,
                XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS
            );
            भङ्ग;
        पक्ष XKB_किल्ली_MODIFIER:
            status = add_modifier_किल्ली(&state, &e);
            भङ्ग;
        पक्ष XKB_किल्ली_MISC:
            status = add_other_किल्ली(
                &state, &state.किल्ल्या->misc, &e,
                XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS
            );
            भङ्ग;
        स्वभाव: {
            static_assert(XKB_किल्ली_MISC == 2 &&
                          XKB_किल्ली_MISC == अंक_XKB_किल्ली_KIND - 1, "");
            status = XKB_ERROR_INVALID;
        }}

        यदि (status != XKB_SUCCESS &&
            status != (गणना xkb_status)XKB_ERROR_NO_RELEASED_किल्ली)
        {
            गच्छ error;
        }
    }

    प्रतिज्ञा(!state.अंक_pending);

    typing->events = steal(&state.events);
    typing->अंक_events = अंक_events;

    status = XKB_SUCCESS;

error:
    darray_free(pending);
    xkb_typing_state_destroy(&state);

    प्रत्यागम status;
}

शून्य
xkb_typing_आदान_destroy(संरचना xkb_typing_आदान *typing)
{
    यदि (!typing)
        प्रत्यागम;

    मुक्त(typing->events);
    typing->events = रिक्त;
}
