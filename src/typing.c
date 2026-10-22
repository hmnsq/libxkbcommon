/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <stdint.h>
#ఒకవేళ WCHAR_MAX >= 0x7FFFFFFF
#అన్తర్భావ <wctype.h>
#నిర్వచించు USE_ISWPRINT
#ఒకవేళ HAVE_NEWLOCALE && HAVE_ISWPRINT_L
#నిర్వచించు USE_ISWPRINT_L
#అన్తర్భావ <locale.h>
#అయితే_ముగింపు
#లేకపోతే
#అన్తర్భావ <ctype.h>
#అయితే_ముగింపు

#అన్తర్భావ "xkbcommon/xkbcommon-status.h"
#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "darray.h"
#అన్తర్భావ "state-priv.h"
#అన్తర్భావ "typing.h"
#అన్తర్భావ "util-bits.h"
#అన్తర్భావ "util-mem.h"
#అన్తర్భావ "utils.h"

గణన xkb_స్థితి
xkb_కీలకం_సమితి_init(నిర్మాణం xkb_కీలకం_సమితి *సమితి,
                 నిర్మాణం xkb_keymap *keymap,
                 స్థిరరాశి నిర్మాణం xkb_కీలకం_సమితి_config *config)
{
    darray_init(సమితి->printable);
    darray_init(సమితి->modifiers);
    darray_init(సమితి->misc);

    స్థిర స్థిరరాశి నిర్మాణం xkb_కీలకం_సమితి_config default_config = {0};
    ఒకవేళ (!config)
        config = &default_config;
    /* keycode = 0 denotes “default” */
    స్థిరరాశి xkb_keycode_t min = MAX(config->min, xkb_keymap_min_keycode(keymap));
    స్థిరరాశి xkb_keycode_t max = config->max
        ? MIN(config->max, xkb_keymap_max_keycode(keymap))
        : xkb_keymap_max_keycode(keymap);

    గణన xkb_స్థితి స్థితి_2;
    నిర్మాణం xkb_state *state =
        xkb_state_new_with_బహుళకం(keymap, XKB_STATE_బహుళకం_SERVER, &స్థితి_2);
    ఒకవేళ (!state)
        తిరిగి స్థితి_2;

    #నిర్వచితమైతే USE_ISWPRINT_L
    locale_t loc = newlocale(LC_ALL_MASK, "C.UTF-8", (locale_t)0);
    #అయితే_ముగింపు

    నిర్మాణం xkb_keymap_కీలకం_iterator iter;
    స్థిరరాశి నిర్మాణం xkb_keymap_కీలకం_iterator_config iter_config = {
        .కొలత = పరిమాణం(iter_config),
        .flags = XKB_KEYMAP_కీలకం_ITERATOR_NO_FLAGS,
        .ప్రారంభం = min
    };
    స్థితి_2 = xkb_keymap_కీలకం_iterator_init(&iter, keymap, &iter_config);
    ఒకవేళ (స్థితి_2 != XKB_SUCCESS)
        వెళ్ళు error;

    స్థిరరాశి xkb_mod_ముసుగు_t numlock =
        xkb_keymap_mod_get_ముసుగు(keymap, XKB_VMOD_NAME_అంకె);

    స్థిరరాశి నిర్మాణం xkb_state_components_update components_update = {
        .కొలత = పరిమాణం(components_update),
        .components = XKB_STATE_MODS_LOCKED,
        .affect_locked_mods = numlock,
        .locked_mods = numlock,
    };

    స్థిరరాశి నిర్మాణం xkb_synthetic_update update = {
        .కొలత = పరిమాణం(update),
        .components = &components_update,
    };

    xkb_keycode_t k;
    అయితే ((k = xkb_keymap_కీలకం_iterator_తదుపరి(&iter)) != XKB_KEYCODE_INVALID) {
        ఒకవేళ (k > max)
            ఆపు;

        xkb_state_reset(state);
        స్థితి_2 = xkb_state_update_synthetic(state, &update, ఖాళీ);
        ఒకవేళ (స్థితి_2 != XKB_SUCCESS)
            వెళ్ళు error;

        స్థిరరాశి xkb_keysym_t *syms = ఖాళీ;
        స్థిరరాశి పూర్ణాంకం లెక్క = xkb_state_కీలకం_get_syms(state, k, &syms);

        గణన xkb_state_component మారింది =
            xkb_state_update_కీలకం(state, k, XKB_కీలకం_కిందికి);
        xkb_mod_ముసుగు_t mods =
            xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);
        మారింది |= xkb_state_update_కీలకం(state, k, XKB_కీలకం_పైకి);
        mods |= xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);

        ఒకవేళ (మారింది & XKB_STATE_MODS_EFFECTIVE) {
            /* Modifier key */
            గణన xkb_modifier_రకం రకం;
            ఒకవేళ (మారింది & XKB_STATE_MODS_LOCKED) {
                రకం = XKB_MOD_తాళం;
            } లేకపోతే ఒకవేళ (మారింది & XKB_STATE_MODS_LATCHED) {
                రకం = XKB_MOD_LATCH;
            } లేకపోతే {
                రకం = XKB_MOD_సమితి;
            }
            mods &= ~numlock;
            ఒకవేళ (!mods)
                mods = numlock;
            darray_append(సమితి->modifiers, (నిర్మాణం xkb_modifier_కీలకం) {
                .keycode = k,
                .mods = mods,
                .రకం = రకం,
            });
            కొనసాగు;
        }

        ఒకవేళ (లెక్క > 0) {
            uint32_t utf32 = 0;
            కొరకు (పూర్ణాంకం s = 0; s < లెక్క; s++) {
                utf32 = xkb_keysym_గమ్యం_utf32(syms[s]);
                స్థిరరాశి bool is_printable =
                    #నిర్వచితమైతే USE_ISWPRINT_L
                    ((loc && iswprint_l((wint_t)utf32, loc)) || iswprint((wint_t)utf32))
                    #లేదావేళ defined(USE_ISWPRINT)
                    iswprint((wint_t)utf32)
                    #లేకపోతే
                    utf32 <= 0x7f && isprint((పూర్ణాంకం)utf32)
                    #అయితే_ముగింపు
                    ;
                ఒకవేళ (is_printable)
                    ఆపు;
                utf32 = 0;
            }
            ఒకవేళ (utf32) {
                /* Printable keysyms */
                darray_append(సమితి->printable, k);
                కొనసాగు;
            }
        }

        /* Miscellaneous key */
        darray_append(సమితి->misc, k);
    }

error:
    #నిర్వచితమైతే USE_ISWPRINT_L
    ఒకవేళ (loc)
        freelocale(loc);
    #అయితే_ముగింపు

    xkb_state_unref(state);
    ఒకవేళ (స్థితి_2 != XKB_SUCCESS)
        xkb_కీలకం_సమితి_destroy(సమితి);

    తిరిగి స్థితి_2;
}

శూన్య
xkb_కీలకం_సమితి_destroy(నిర్మాణం xkb_కీలకం_సమితి *సమితి)
{
    ఒకవేళ (!సమితి)
        తిరిగి;

    darray_free(సమితి->modifiers);
    darray_free(సమితి->printable);
    darray_free(సమితి->misc);
}

స్థిర గణన xkb_స్థితి
కూడు_pending_events(నిర్మాణం xkb_typing_state * పరిమిత state,
                   స్థిరరాశి నిర్మాణం xkb_pending_typing_event * పరిమిత events,
                   కొలత_t లెక్క)
{
    కొరకు (కొలత_t e = 0; e < లెక్క; e++) {
        /* Look for a free slot */
        నిర్మాణం xkb_pending_typing_event *pending;
        darray_కొలత_t idx = 0;
        darray_enumerate_from(idx, pending, state->pending, idx) {
            ఒకవేళ (!pending->counter) {
                darray_అంశం(state->pending, idx) = events[e];
                వెళ్ళు తదుపరి;
            }
        }
        /* No free slot found: append new one */
        idx = darray_కొలత(state->pending);
        darray_append(state->pending, events[e]);
    తదుపరి:
        /* Avoid immediate consumption */
        darray_అంశం(state->pending, idx).counter++;
    }
    state->అంకె_pending += లెక్క;

    తిరిగి XKB_SUCCESS;
}

స్థిర గణన xkb_స్థితి
తదుపరి_pending_events(నిర్మాణం xkb_typing_state * పరిమిత state,
                    xkb_typing_events * పరిమిత బహిర్గతం)
{
    /* Reset the output */
    darray_కొలత(*బహిర్గతం) = 0;

    darray_కొలత_t idx;
    నిర్మాణం xkb_pending_typing_event *pending;
    /* Stack: start from the end */
    darray_enumerate_తిరగేయి(idx, pending, state->pending) {
        ఒకవేళ (!pending->counter)
            కొనసాగు;
        pending->counter--;
        ఒకవేళ (pending->counter)
            కొనసాగు;
        darray_append(*బహిర్గతం, pending->event);
        state->అంకె_pending--;
        /* Resize array if it was the last element */
        ఒకవేళ (idx + 1 == darray_కొలత(state->pending))
            darray_కొలత(state->pending)--;
        state->mods &= ~pending->mods;
    }

    తిరిగి XKB_SUCCESS;
}

గణన {
    /** Tries to avoid pressing a key already pressed */
    కీలకం_CHOICE_MAX_TRIES = 12,
    XKB_ERROR_NO_RELEASED_కీలకం = -1000,
};

స్థిర గణన xkb_స్థితి
కూడు_modifier_కీలకం(నిర్మాణం xkb_typing_state * పరిమిత state, కొలత_t * పరిమిత e)
{
    /* Choose a random key */
    దీర్ఘ idx = -1;
    కొరకు (పూర్ణాంకం tries = 0; tries < కీలకం_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state)
            % darray_కొలత(state->కీలకాలు->modifiers);
        /*
         * Ensure key is not already down and its modifier mask is not active,
         * i.e. to avoid simultaneous keys with the same modifier.
         */
        ఒకవేళ (!బిట్టు_array_get(state->కీలకాలు_state,
                           darray_అంశం(state->కీలకాలు->modifiers, idx).keycode) &&
            !(state->mods & darray_అంశం(state->కీలకాలు->modifiers, idx).mods))
        {
            ఆపు;
        }
        idx = -1;
    }

    నిర్మాణం xkb_modifier_కీలకం కీలకం = {0};
    ఒకవేళ (idx < 0) {
        /* No released key found: fall back to linear search */
        స్థిరరాశి నిర్మాణం xkb_modifier_కీలకం *కీలకం_2;
        darray_foreach(కీలకం_2, state->కీలకాలు->modifiers) {
            /* See test doc above */
            ఒకవేళ (!బిట్టు_array_get(state->కీలకాలు_state, కీలకం_2->keycode) &&
                !(state->mods & కీలకం_2->mods))
            {
                కీలకం = *కీలకం_2;
                idx = 0;
                ఆపు;
            }
        }
        ఒకవేళ (idx < 0) {
            /* No fallback: skip */
            తిరిగి (గణన xkb_స్థితి)XKB_ERROR_NO_RELEASED_కీలకం;
        }
    } లేకపోతే {
        కీలకం = darray_అంశం(state->కీలకాలు->modifiers, idx);
    }

    స్థిరరాశి కొలత_t మిగిలిన_events =
        state->అంకె_events - state->అంకె_pending - *e;
    కొలత_t pending_turns = 0;

    ఒకవేళ (మిగిలిన_events >= 2) {
        ఒకవేళ (కీలకం.రకం == XKB_MOD_సమితి ||
            (కీలకం.రకం == XKB_MOD_తాళం && మిగిలిన_events >= 4))
        {
            /* Delay SetMods release or LockMods unlock */
            స్థిరరాశి కొలత_t max_turns = MIN(
                (కొలత_t)(
                    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS -
                    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                 ),
                మిగిలిన_events + state->అంకె_pending
            );
            pending_turns = XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                          + (కొలత_t)state->prng(state->prng_state)
                          % (max_turns + 1);
        }
    } లేకపోతే {
        తిరిగి XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (నిర్మాణం xkb_typing_event) {
        .keycode = కీలకం.keycode,
        .direction = XKB_కీలకం_కిందికి,
    };

    నిర్మాణం xkb_pending_typing_event pending[2] = {
        {
            .event = {
                .keycode = కీలకం.keycode,
                .direction = XKB_కీలకం_కిందికి,
            },
            .counter = (uint8_t)pending_turns,
        },
        {
            .event = {
                .keycode = కీలకం.keycode,
                .direction = XKB_కీలకం_పైకి,
            },
            .mods = కీలకం.mods,
            .counter = (uint8_t)pending_turns,
        },
    };

    ఒకవేళ (pending_turns) {
        ఒకవేళ (కీలకం.రకం == XKB_MOD_తాళం) {
            /* Immediate release */
            state->events[(*e)++] = pending[1].event;
            /* Delayed unlock (press + release) */
            pending[1].counter++;
            స్థిరరాశి గణన xkb_స్థితి స్థితి_2 =
                కూడు_pending_events(state, pending, ARRAY_కొలత(pending));
            ఒకవేళ (స్థితి_2 != XKB_SUCCESS)
                తిరిగి స్థితి_2;
            /*
             * Register “locked” state, to avoid tapping the key before
             * delayed key released
             */
            బిట్టు_array_సమితి(state->కీలకాలు_state, కీలకం.keycode);
        } లేకపోతే {
            /* Delayed release */
            స్థిరరాశి గణన xkb_స్థితి స్థితి_2 =
                కూడు_pending_events(state, &pending[1], 1);
            ఒకవేళ (స్థితి_2 != XKB_SUCCESS)
                తిరిగి స్థితి_2;
            /* Register pressed state */
            బిట్టు_array_సమితి(state->కీలకాలు_state, కీలకం.keycode);
        }
        state->mods |= కీలకం.mods;
    } లేకపోతే {
        /* Immediate release */
        state->events[(*e)++] = pending[1].event;
        /* TODO: add latched modifier to mask state. */
    }

    తిరిగి XKB_SUCCESS;
}

స్థిర గణన xkb_స్థితి
కూడు_other_కీలకం(నిర్మాణం xkb_typing_state * పరిమిత state,
              స్థిరరాశి xkb_keycodes_t * పరిమిత keycodes, కొలత_t * పరిమిత e,
              uint8_t తక్షణ_release_prob, uint8_t max_pending_turns)
{
    /* Choose a random key */
    దీర్ఘ idx = -1;
    కొరకు (పూర్ణాంకం tries = 0; tries < కీలకం_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state) % darray_కొలత(*keycodes);
        /* Ensure key is not already down */
        ఒకవేళ (!బిట్టు_array_get(state->కీలకాలు_state,
                           darray_అంశం(*keycodes, idx)))
        {
            ఆపు;
        }
        idx = -1;
    }

    xkb_keycode_t keycode = XKB_KEYCODE_INVALID;
    ఒకవేళ (idx < 0) {
        /* No released key found: fall back to linear search */
        స్థిరరాశి xkb_keycode_t *keycode_;
        darray_foreach(keycode_, *keycodes) {
            ఒకవేళ (!బిట్టు_array_get(state->కీలకాలు_state, *keycode_)) {
                keycode = *keycode_;
                idx = 0;
                ఆపు;
            }
        }
        ఒకవేళ (idx < 0) {
            /* No fallback: skip */
            తిరిగి (గణన xkb_స్థితి)XKB_ERROR_NO_RELEASED_కీలకం;
        }
    } లేకపోతే {
        keycode = darray_అంశం(*keycodes, idx);
    }

    స్థిరరాశి కొలత_t మిగిలిన_events =
        state->అంకె_events - state->అంకె_pending - *e;
    కొలత_t pending_turns;

    ఒకవేళ (మిగిలిన_events >= 2) {
        స్థిరరాశి కొలత_t max_turns = MIN(
            (కొలత_t)max_pending_turns,
            మిగిలిన_events + state->అంకె_pending
        );
        స్థిరరాశి bool తక్షణ_release = (
            (తక్షణ_release_prob >= 100) ||
            !max_turns ||
            (state->prng(state->prng_state) % 100) < తక్షణ_release_prob
        );
        ఒకవేళ (తక్షణ_release) {
            pending_turns = 0;
        } లేకపోతే {
            pending_turns = 1 + (కొలత_t)state->prng(state->prng_state)
                          % max_turns;
        }
    } లేకపోతే {
        తిరిగి XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (నిర్మాణం xkb_typing_event) {
        .keycode = keycode,
        .direction = XKB_కీలకం_కిందికి,
    };

    స్థిరరాశి నిర్మాణం xkb_pending_typing_event pending = {
        .event = {
            .keycode = keycode,
            .direction = XKB_కీలకం_పైకి,
        },
        .counter = (uint8_t)pending_turns,
    };
    ఒకవేళ (pending_turns) {
        స్థిరరాశి గణన xkb_స్థితి స్థితి_2 = కూడు_pending_events(state, &pending, 1);
        ఒకవేళ (స్థితి_2 != XKB_SUCCESS)
            తిరిగి స్థితి_2;
        /* Register pressed state */
        బిట్టు_array_సమితి(state->కీలకాలు_state, keycode);
    } లేకపోతే {
        state->events[(*e)++] = pending.event;
    }

    తిరిగి XKB_SUCCESS;
}

స్థిర గణన xkb_కీలకం_రకం
pick_కీలకం_రకం(xkb_prng_t prng, శూన్య *prng_state)
{
    స్థిర స్థిరరాశి uint8_t weights[అంకె_XKB_కీలకం_రకం] = {
        [XKB_కీలకం_PRINTABLE] = 60,
        [XKB_కీలకం_MODIFIER] = 30,
        [XKB_కీలకం_MISC] = 10,
    };
    static_assert(XKB_కీలకం_MISC == 2 &&
                  XKB_కీలకం_MISC == అంకె_XKB_కీలకం_రకం - 1, "");
    ప్రతిజ్ఞా(weights[0] + weights[1] + weights[2] == 100);

    దీర్ఘ r = prng(prng_state) % 100;
    కొరకు (గణన xkb_కీలకం_రకం k = 0; k < అంకె_XKB_కీలకం_రకం; k++)
        ఒకవేళ ((r -= weights[k]) < 0)
            తిరిగి k;
    తిరిగి XKB_కీలకం_PRINTABLE;
}

స్థిర శూన్య
xkb_typing_state_destroy(నిర్మాణం xkb_typing_state *state)
{
    విడుదల(state->కీలకాలు_state);
    darray_free(state->pending);
    విడుదల(state->events);
}

గణన xkb_స్థితి
xkb_typing_ఆదానం_init(నిర్మాణం xkb_typing_ఆదానం * పరిమిత typing,
                      స్థిరరాశి నిర్మాణం xkb_కీలకం_సమితి * పరిమిత కీలకాలు,
                      స్థిరరాశి నిర్మాణం xkb_typing_ఆదానం_config * పరిమిత config)
{
    typing->events = ఖాళీ;
    typing->అంకె_events = 0;

    ఒకవేళ (!config->ఆదానం_నిడివి)
        తిరిగి XKB_ERROR_INVALID;

    ఒకవేళ (darray_ఖాళీలు(కీలకాలు->printable) ||
        darray_ఖాళీలు(కీలకాలు->modifiers) ||
        darray_ఖాళీలు(కీలకాలు->misc))
    {
        తిరిగి XKB_ERROR_INVALID;
    }

    గణన xkb_స్థితి స్థితి_2 = XKB_SUCCESS;

    /* NOTE: arrays are sorted because they were built using the key iterator */
    xkb_keycode_t keycode_max = darray_చివరి(కీలకాలు->printable, 0);
    keycode_max = MAX(
        darray_చివరి(కీలకాలు->modifiers, (నిర్మాణం xkb_modifier_కీలకం){0}).keycode,
        keycode_max
    );
    keycode_max = MAX(darray_చివరి(కీలకాలు->misc, 0), keycode_max);
    స్థిరరాశి కొలత_t కీలకాలు_state_అంకె = బిట్టు_array_పదాలు((కొలత_t)keycode_max + 1);
    uintptr_t *కీలకాలు_state = సున్నా_జ్ఞాపకం_ఇవ్వు(కీలకాలు_state_అంకె, పరిమాణం(*కీలకాలు_state));

    స్థిరరాశి కొలత_t అంకె_events = config->ఆదానం_నిడివి;
    నిర్మాణం xkb_typing_event *events = సున్నా_జ్ఞాపకం_ఇవ్వు(అంకె_events, పరిమాణం(*events));

    నిర్మాణం xkb_typing_state state = {
        .కీలకాలు = కీలకాలు,
        .కీలకాలు_state = కీలకాలు_state,
        .prng = config->prng,
        .prng_state = config->prng_state,
        .events = events,
        .అంకె_events = అంకె_events,
        .pending = darray_new(),
        .అంకె_pending = 0,
    };

    xkb_typing_events pending = darray_new();

    ఒకవేళ (!కీలకాలు_state || !events) {
        స్థితి_2 = XKB_ERROR_కేటాయింపు_FAILURE;
        వెళ్ళు error;
    }

    /*
     * Invariant: e + state.num_pending <= state.num_events
     */

    కొరకు (కొలత_t e = 0; e < అంకె_events;) {
        /*
         * Process pending events
         */
        స్థితి_2 = తదుపరి_pending_events(&state, &pending);
        ఒకవేళ (స్థితి_2 != XKB_SUCCESS) {
            వెళ్ళు error;
        }
        నిర్మాణం xkb_typing_event *event;
        darray_foreach(event, pending) {
            ఒకవేళ (e >= అంకె_events) {
                /* invariant violated: should be unreachable */
                PANIC_UNREACHABLE();
            }
            events[e] = *event;
            బిట్టు_array_కేటాయించు(కీలకాలు_state, event->keycode, (bool)event->direction);
            e++;
        }

        ఒకవేళ (!state.అంకె_pending && e == అంకె_events - 1) {
            /*
             * Cannot add press/release *pair*, so just use an invalid keycode
             * for the last event.
             */
            events[e] = (నిర్మాణం xkb_typing_event) {
                .keycode = XKB_KEYCODE_INVALID,
                .direction = XKB_కీలకం_కిందికి,
            };
            ఆపు;
        }

        ఒకవేళ (e + 1 + state.అంకె_pending >= అంకె_events) {
            /* Cannot add new pair of events: require flushing pending events */
            కొనసాగు;
        }

        /*
         * Create new events
         */

        /* Choose kind of key */
        static_assert(XKB_కీలకం_PRINTABLE == 0, "");
        స్థిరరాశి గణన xkb_కీలకం_రకం కీలకం_రకం =
            pick_కీలకం_రకం(config->prng, config->prng_state);
        ఎంపిక (కీలకం_రకం) {
        స్థితి XKB_కీలకం_PRINTABLE:
            స్థితి_2 = కూడు_other_కీలకం(
                &state, &state.కీలకాలు->printable, &e,
                XKB_TYPING_EVENT_PRINTABLE_తక్షణ_RELEASE_PERCENT,
                XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS
            );
            ఆపు;
        స్థితి XKB_కీలకం_MODIFIER:
            స్థితి_2 = కూడు_modifier_కీలకం(&state, &e);
            ఆపు;
        స్థితి XKB_కీలకం_MISC:
            స్థితి_2 = కూడు_other_కీలకం(
                &state, &state.కీలకాలు->misc, &e,
                XKB_TYPING_EVENT_MISC_తక్షణ_RELEASE_PERCENT,
                XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS
            );
            ఆపు;
        అప్రమేయ: {
            static_assert(XKB_కీలకం_MISC == 2 &&
                          XKB_కీలకం_MISC == అంకె_XKB_కీలకం_రకం - 1, "");
            స్థితి_2 = XKB_ERROR_INVALID;
        }}

        ఒకవేళ (స్థితి_2 != XKB_SUCCESS &&
            స్థితి_2 != (గణన xkb_స్థితి)XKB_ERROR_NO_RELEASED_కీలకం)
        {
            వెళ్ళు error;
        }
    }

    ప్రతిజ్ఞా(!state.అంకె_pending);

    typing->events = steal(&state.events);
    typing->అంకె_events = అంకె_events;

    స్థితి_2 = XKB_SUCCESS;

error:
    darray_free(pending);
    xkb_typing_state_destroy(&state);

    తిరిగి స్థితి_2;
}

శూన్య
xkb_typing_ఆదానం_destroy(నిర్మాణం xkb_typing_ఆదానం *typing)
{
    ఒకవేళ (!typing)
        తిరిగి;

    విడుదల(typing->events);
    typing->events = ఖాళీ;
}
