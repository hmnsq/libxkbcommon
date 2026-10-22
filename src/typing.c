/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <stdbool.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <stdint.h>
#എങ്കിൽ WCHAR_MAX >= 0x7FFFFFFF
#അന്തര്ഭാവ <wctype.h>
#define USE_ISWPRINT
#എങ്കിൽ HAVE_NEWLOCALE && HAVE_ISWPRINT_L
#define USE_ISWPRINT_L
#അന്തര്ഭാവ <locale.h>
#endif
#അല്ലെങ്കിൽ
#അന്തര്ഭാവ <ctype.h>
#endif

#അന്തര്ഭാവ "xkbcommon/xkbcommon-status.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "state-priv.h"
#അന്തര്ഭാവ "typing.h"
#അന്തര്ഭാവ "util-bits.h"
#അന്തര്ഭാവ "util-mem.h"
#അന്തര്ഭാവ "utils.h"

എണ്ണൽ xkb_status
xkb_താക്കോൽ_set_init(ഘടന xkb_താക്കോൽ_set *set,
                 ഘടന xkb_keymap *keymap,
                 സ്ഥിരം ഘടന xkb_താക്കോൽ_set_config *config)
{
    darray_init(set->printable);
    darray_init(set->modifiers);
    darray_init(set->misc);

    സ്ഥിതം സ്ഥിരം ഘടന xkb_താക്കോൽ_set_config default_config = {0};
    എങ്കിൽ (!config)
        config = &default_config;
    /* keycode = 0 denotes “default” */
    സ്ഥിരം xkb_keycode_t min = MAX(config->min, xkb_keymap_min_keycode(keymap));
    സ്ഥിരം xkb_keycode_t max = config->max
        ? MIN(config->max, xkb_keymap_max_keycode(keymap))
        : xkb_keymap_max_keycode(keymap);

    എണ്ണൽ xkb_status status;
    ഘടന xkb_state *state =
        xkb_state_new_with_mode(keymap, XKB_STATE_MODE_SERVER, &status);
    എങ്കിൽ (!state)
        തിരികെ status;

    #ifdef USE_ISWPRINT_L
    locale_t loc = newlocale(LC_ALL_MASK, "C.UTF-8", (locale_t)0);
    #endif

    ഘടന xkb_keymap_താക്കോൽ_iterator iter;
    സ്ഥിരം ഘടന xkb_keymap_താക്കോൽ_iterator_config iter_config = {
        .size = വലുപ്പം(iter_config),
        .flags = XKB_KEYMAP_താക്കോൽ_ITERATOR_NO_FLAGS,
        .start = min
    };
    status = xkb_keymap_താക്കോൽ_iterator_init(&iter, keymap, &iter_config);
    എങ്കിൽ (status != XKB_SUCCESS)
        പോകുക error;

    സ്ഥിരം xkb_mod_mask_t numlock =
        xkb_keymap_mod_get_mask(keymap, XKB_VMOD_NAME_അക്കം);

    സ്ഥിരം ഘടന xkb_state_components_update components_update = {
        .size = വലുപ്പം(components_update),
        .components = XKB_STATE_MODS_LOCKED,
        .affect_locked_mods = numlock,
        .locked_mods = numlock,
    };

    സ്ഥിരം ഘടന xkb_synthetic_update update = {
        .size = വലുപ്പം(update),
        .components = &components_update,
    };

    xkb_keycode_t k;
    ആകുമ്പോൾ ((k = xkb_keymap_താക്കോൽ_iterator_next(&iter)) != XKB_KEYCODE_INVALID) {
        എങ്കിൽ (k > max)
            നിർത്തുക;

        xkb_state_reset(state);
        status = xkb_state_update_synthetic(state, &update, ശൂന്യ);
        എങ്കിൽ (status != XKB_SUCCESS)
            പോകുക error;

        സ്ഥിരം xkb_keysym_t *syms = ശൂന്യ;
        സ്ഥിരം പൂർണ്ണസംഖ്യ എണ്ണം = xkb_state_താക്കോൽ_get_syms(state, k, &syms);

        എണ്ണൽ xkb_state_component changed =
            xkb_state_update_താക്കോൽ(state, k, XKB_താക്കോൽ_DOWN);
        xkb_mod_mask_t mods =
            xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);
        changed |= xkb_state_update_താക്കോൽ(state, k, XKB_താക്കോൽ_UP);
        mods |= xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);

        എങ്കിൽ (changed & XKB_STATE_MODS_EFFECTIVE) {
            /* Modifier key */
            എണ്ണൽ xkb_modifier_kind kind;
            എങ്കിൽ (changed & XKB_STATE_MODS_LOCKED) {
                kind = XKB_MOD_LOCK;
            } അല്ലെങ്കിൽ എങ്കിൽ (changed & XKB_STATE_MODS_LATCHED) {
                kind = XKB_MOD_LATCH;
            } അല്ലെങ്കിൽ {
                kind = XKB_MOD_SET;
            }
            mods &= ~numlock;
            എങ്കിൽ (!mods)
                mods = numlock;
            darray_append(set->modifiers, (ഘടന xkb_modifier_താക്കോൽ) {
                .keycode = k,
                .mods = mods,
                .kind = kind,
            });
            തുടരുക;
        }

        എങ്കിൽ (എണ്ണം > 0) {
            uint32_t utf32 = 0;
            വേണ്ടി (പൂർണ്ണസംഖ്യ s = 0; s < എണ്ണം; s++) {
                utf32 = xkb_keysym_to_utf32(syms[s]);
                സ്ഥിരം bool is_printable =
                    #ifdef USE_ISWPRINT_L
                    ((loc && iswprint_l((wint_t)utf32, loc)) || iswprint((wint_t)utf32))
                    #അല്ലെങ്കിലെങ്കിൽ defined(USE_ISWPRINT)
                    iswprint((wint_t)utf32)
                    #അല്ലെങ്കിൽ
                    utf32 <= 0x7f && isprint((പൂർണ്ണസംഖ്യ)utf32)
                    #endif
                    ;
                എങ്കിൽ (is_printable)
                    നിർത്തുക;
                utf32 = 0;
            }
            എങ്കിൽ (utf32) {
                /* Printable keysyms */
                darray_append(set->printable, k);
                തുടരുക;
            }
        }

        /* Miscellaneous key */
        darray_append(set->misc, k);
    }

error:
    #ifdef USE_ISWPRINT_L
    എങ്കിൽ (loc)
        freelocale(loc);
    #endif

    xkb_state_unref(state);
    എങ്കിൽ (status != XKB_SUCCESS)
        xkb_താക്കോൽ_set_destroy(set);

    തിരികെ status;
}

ശൂന്യം
xkb_താക്കോൽ_set_destroy(ഘടന xkb_താക്കോൽ_set *set)
{
    എങ്കിൽ (!set)
        തിരികെ;

    darray_free(set->modifiers);
    darray_free(set->printable);
    darray_free(set->misc);
}

സ്ഥിതം എണ്ണൽ xkb_status
add_pending_events(ഘടന xkb_typing_state * restrict state,
                   സ്ഥിരം ഘടന xkb_pending_typing_event * restrict events,
                   size_t എണ്ണം)
{
    വേണ്ടി (size_t e = 0; e < എണ്ണം; e++) {
        /* Look for a free slot */
        ഘടന xkb_pending_typing_event *pending;
        darray_size_t idx = 0;
        darray_enumerate_from(idx, pending, state->pending, idx) {
            എങ്കിൽ (!pending->counter) {
                darray_item(state->pending, idx) = events[e];
                പോകുക next;
            }
        }
        /* No free slot found: append new one */
        idx = darray_size(state->pending);
        darray_append(state->pending, events[e]);
    next:
        /* Avoid immediate consumption */
        darray_item(state->pending, idx).counter++;
    }
    state->അക്കം_pending += എണ്ണം;

    തിരികെ XKB_SUCCESS;
}

സ്ഥിതം എണ്ണൽ xkb_status
next_pending_events(ഘടന xkb_typing_state * restrict state,
                    xkb_typing_events * restrict out)
{
    /* Reset the output */
    darray_size(*out) = 0;

    darray_size_t idx;
    ഘടന xkb_pending_typing_event *pending;
    /* Stack: start from the end */
    darray_enumerate_reverse(idx, pending, state->pending) {
        എങ്കിൽ (!pending->counter)
            തുടരുക;
        pending->counter--;
        എങ്കിൽ (pending->counter)
            തുടരുക;
        darray_append(*out, pending->event);
        state->അക്കം_pending--;
        /* Resize array if it was the last element */
        എങ്കിൽ (idx + 1 == darray_size(state->pending))
            darray_size(state->pending)--;
        state->mods &= ~pending->mods;
    }

    തിരികെ XKB_SUCCESS;
}

എണ്ണൽ {
    /** Tries to avoid pressing a key already pressed */
    താക്കോൽ_CHOICE_MAX_TRIES = 12,
    XKB_ERROR_NO_RELEASED_താക്കോൽ = -1000,
};

സ്ഥിതം എണ്ണൽ xkb_status
add_modifier_താക്കോൽ(ഘടന xkb_typing_state * restrict state, size_t * restrict e)
{
    /* Choose a random key */
    long idx = -1;
    വേണ്ടി (പൂർണ്ണസംഖ്യ tries = 0; tries < താക്കോൽ_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state)
            % darray_size(state->താക്കോലുകൾ->modifiers);
        /*
         * Ensure key is not already down and its modifier mask is not active,
         * i.e. to avoid simultaneous keys with the same modifier.
         */
        എങ്കിൽ (!bit_array_get(state->താക്കോലുകൾ_state,
                           darray_item(state->താക്കോലുകൾ->modifiers, idx).keycode) &&
            !(state->mods & darray_item(state->താക്കോലുകൾ->modifiers, idx).mods))
        {
            നിർത്തുക;
        }
        idx = -1;
    }

    ഘടന xkb_modifier_താക്കോൽ താക്കോൽ = {0};
    എങ്കിൽ (idx < 0) {
        /* No released key found: fall back to linear search */
        സ്ഥിരം ഘടന xkb_modifier_താക്കോൽ *താക്കോൽ_2;
        darray_foreach(താക്കോൽ_2, state->താക്കോലുകൾ->modifiers) {
            /* See test doc above */
            എങ്കിൽ (!bit_array_get(state->താക്കോലുകൾ_state, താക്കോൽ_2->keycode) &&
                !(state->mods & താക്കോൽ_2->mods))
            {
                താക്കോൽ = *താക്കോൽ_2;
                idx = 0;
                നിർത്തുക;
            }
        }
        എങ്കിൽ (idx < 0) {
            /* No fallback: skip */
            തിരികെ (എണ്ണൽ xkb_status)XKB_ERROR_NO_RELEASED_താക്കോൽ;
        }
    } അല്ലെങ്കിൽ {
        താക്കോൽ = darray_item(state->താക്കോലുകൾ->modifiers, idx);
    }

    സ്ഥിരം size_t remaining_events =
        state->അക്കം_events - state->അക്കം_pending - *e;
    size_t pending_turns = 0;

    എങ്കിൽ (remaining_events >= 2) {
        എങ്കിൽ (താക്കോൽ.kind == XKB_MOD_SET ||
            (താക്കോൽ.kind == XKB_MOD_LOCK && remaining_events >= 4))
        {
            /* Delay SetMods release or LockMods unlock */
            സ്ഥിരം size_t max_turns = MIN(
                (size_t)(
                    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS -
                    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                 ),
                remaining_events + state->അക്കം_pending
            );
            pending_turns = XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                          + (size_t)state->prng(state->prng_state)
                          % (max_turns + 1);
        }
    } അല്ലെങ്കിൽ {
        തിരികെ XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (ഘടന xkb_typing_event) {
        .keycode = താക്കോൽ.keycode,
        .direction = XKB_താക്കോൽ_DOWN,
    };

    ഘടന xkb_pending_typing_event pending[2] = {
        {
            .event = {
                .keycode = താക്കോൽ.keycode,
                .direction = XKB_താക്കോൽ_DOWN,
            },
            .counter = (uint8_t)pending_turns,
        },
        {
            .event = {
                .keycode = താക്കോൽ.keycode,
                .direction = XKB_താക്കോൽ_UP,
            },
            .mods = താക്കോൽ.mods,
            .counter = (uint8_t)pending_turns,
        },
    };

    എങ്കിൽ (pending_turns) {
        എങ്കിൽ (താക്കോൽ.kind == XKB_MOD_LOCK) {
            /* Immediate release */
            state->events[(*e)++] = pending[1].event;
            /* Delayed unlock (press + release) */
            pending[1].counter++;
            സ്ഥിരം എണ്ണൽ xkb_status status =
                add_pending_events(state, pending, ARRAY_SIZE(pending));
            എങ്കിൽ (status != XKB_SUCCESS)
                തിരികെ status;
            /*
             * Register “locked” state, to avoid tapping the key before
             * delayed key released
             */
            bit_array_set(state->താക്കോലുകൾ_state, താക്കോൽ.keycode);
        } അല്ലെങ്കിൽ {
            /* Delayed release */
            സ്ഥിരം എണ്ണൽ xkb_status status =
                add_pending_events(state, &pending[1], 1);
            എങ്കിൽ (status != XKB_SUCCESS)
                തിരികെ status;
            /* Register pressed state */
            bit_array_set(state->താക്കോലുകൾ_state, താക്കോൽ.keycode);
        }
        state->mods |= താക്കോൽ.mods;
    } അല്ലെങ്കിൽ {
        /* Immediate release */
        state->events[(*e)++] = pending[1].event;
        /* TODO: add latched modifier to mask state. */
    }

    തിരികെ XKB_SUCCESS;
}

സ്ഥിതം എണ്ണൽ xkb_status
add_other_താക്കോൽ(ഘടന xkb_typing_state * restrict state,
              സ്ഥിരം xkb_keycodes_t * restrict keycodes, size_t * restrict e,
              uint8_t immediate_release_prob, uint8_t max_pending_turns)
{
    /* Choose a random key */
    long idx = -1;
    വേണ്ടി (പൂർണ്ണസംഖ്യ tries = 0; tries < താക്കോൽ_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state) % darray_size(*keycodes);
        /* Ensure key is not already down */
        എങ്കിൽ (!bit_array_get(state->താക്കോലുകൾ_state,
                           darray_item(*keycodes, idx)))
        {
            നിർത്തുക;
        }
        idx = -1;
    }

    xkb_keycode_t keycode = XKB_KEYCODE_INVALID;
    എങ്കിൽ (idx < 0) {
        /* No released key found: fall back to linear search */
        സ്ഥിരം xkb_keycode_t *keycode_;
        darray_foreach(keycode_, *keycodes) {
            എങ്കിൽ (!bit_array_get(state->താക്കോലുകൾ_state, *keycode_)) {
                keycode = *keycode_;
                idx = 0;
                നിർത്തുക;
            }
        }
        എങ്കിൽ (idx < 0) {
            /* No fallback: skip */
            തിരികെ (എണ്ണൽ xkb_status)XKB_ERROR_NO_RELEASED_താക്കോൽ;
        }
    } അല്ലെങ്കിൽ {
        keycode = darray_item(*keycodes, idx);
    }

    സ്ഥിരം size_t remaining_events =
        state->അക്കം_events - state->അക്കം_pending - *e;
    size_t pending_turns;

    എങ്കിൽ (remaining_events >= 2) {
        സ്ഥിരം size_t max_turns = MIN(
            (size_t)max_pending_turns,
            remaining_events + state->അക്കം_pending
        );
        സ്ഥിരം bool immediate_release = (
            (immediate_release_prob >= 100) ||
            !max_turns ||
            (state->prng(state->prng_state) % 100) < immediate_release_prob
        );
        എങ്കിൽ (immediate_release) {
            pending_turns = 0;
        } അല്ലെങ്കിൽ {
            pending_turns = 1 + (size_t)state->prng(state->prng_state)
                          % max_turns;
        }
    } അല്ലെങ്കിൽ {
        തിരികെ XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (ഘടന xkb_typing_event) {
        .keycode = keycode,
        .direction = XKB_താക്കോൽ_DOWN,
    };

    സ്ഥിരം ഘടന xkb_pending_typing_event pending = {
        .event = {
            .keycode = keycode,
            .direction = XKB_താക്കോൽ_UP,
        },
        .counter = (uint8_t)pending_turns,
    };
    എങ്കിൽ (pending_turns) {
        സ്ഥിരം എണ്ണൽ xkb_status status = add_pending_events(state, &pending, 1);
        എങ്കിൽ (status != XKB_SUCCESS)
            തിരികെ status;
        /* Register pressed state */
        bit_array_set(state->താക്കോലുകൾ_state, keycode);
    } അല്ലെങ്കിൽ {
        state->events[(*e)++] = pending.event;
    }

    തിരികെ XKB_SUCCESS;
}

സ്ഥിതം എണ്ണൽ xkb_താക്കോൽ_kind
pick_താക്കോൽ_kind(xkb_prng_t prng, ശൂന്യം *prng_state)
{
    സ്ഥിതം സ്ഥിരം uint8_t weights[അക്കം_XKB_താക്കോൽ_KIND] = {
        [XKB_താക്കോൽ_PRINTABLE] = 60,
        [XKB_താക്കോൽ_MODIFIER] = 30,
        [XKB_താക്കോൽ_MISC] = 10,
    };
    static_assert(XKB_താക്കോൽ_MISC == 2 &&
                  XKB_താക്കോൽ_MISC == അക്കം_XKB_താക്കോൽ_KIND - 1, "");
    പ്രതിജ്ഞാ(weights[0] + weights[1] + weights[2] == 100);

    long r = prng(prng_state) % 100;
    വേണ്ടി (എണ്ണൽ xkb_താക്കോൽ_kind k = 0; k < അക്കം_XKB_താക്കോൽ_KIND; k++)
        എങ്കിൽ ((r -= weights[k]) < 0)
            തിരികെ k;
    തിരികെ XKB_താക്കോൽ_PRINTABLE;
}

സ്ഥിതം ശൂന്യം
xkb_typing_state_destroy(ഘടന xkb_typing_state *state)
{
    സ്വതന്ത്രം(state->താക്കോലുകൾ_state);
    darray_free(state->pending);
    സ്വതന്ത്രം(state->events);
}

എണ്ണൽ xkb_status
xkb_typing_നിവേശം_init(ഘടന xkb_typing_നിവേശം * restrict typing,
                      സ്ഥിരം ഘടന xkb_താക്കോൽ_set * restrict താക്കോലുകൾ,
                      സ്ഥിരം ഘടന xkb_typing_നിവേശം_config * restrict config)
{
    typing->events = ശൂന്യ;
    typing->അക്കം_events = 0;

    എങ്കിൽ (!config->നിവേശം_length)
        തിരികെ XKB_ERROR_INVALID;

    എങ്കിൽ (darray_empty(താക്കോലുകൾ->printable) ||
        darray_empty(താക്കോലുകൾ->modifiers) ||
        darray_empty(താക്കോലുകൾ->misc))
    {
        തിരികെ XKB_ERROR_INVALID;
    }

    എണ്ണൽ xkb_status status = XKB_SUCCESS;

    /* NOTE: arrays are sorted because they were built using the key iterator */
    xkb_keycode_t keycode_max = darray_last(താക്കോലുകൾ->printable, 0);
    keycode_max = MAX(
        darray_last(താക്കോലുകൾ->modifiers, (ഘടന xkb_modifier_താക്കോൽ){0}).keycode,
        keycode_max
    );
    keycode_max = MAX(darray_last(താക്കോലുകൾ->misc, 0), keycode_max);
    സ്ഥിരം size_t താക്കോലുകൾ_state_അക്കം = bit_array_വാക്കുകൾ((size_t)keycode_max + 1);
    uintptr_t *താക്കോലുകൾ_state = calloc(താക്കോലുകൾ_state_അക്കം, വലുപ്പം(*താക്കോലുകൾ_state));

    സ്ഥിരം size_t അക്കം_events = config->നിവേശം_length;
    ഘടന xkb_typing_event *events = calloc(അക്കം_events, വലുപ്പം(*events));

    ഘടന xkb_typing_state state = {
        .താക്കോലുകൾ = താക്കോലുകൾ,
        .താക്കോലുകൾ_state = താക്കോലുകൾ_state,
        .prng = config->prng,
        .prng_state = config->prng_state,
        .events = events,
        .അക്കം_events = അക്കം_events,
        .pending = darray_new(),
        .അക്കം_pending = 0,
    };

    xkb_typing_events pending = darray_new();

    എങ്കിൽ (!താക്കോലുകൾ_state || !events) {
        status = XKB_ERROR_ALLOCATION_FAILURE;
        പോകുക error;
    }

    /*
     * Invariant: e + state.num_pending <= state.num_events
     */

    വേണ്ടി (size_t e = 0; e < അക്കം_events;) {
        /*
         * Process pending events
         */
        status = next_pending_events(&state, &pending);
        എങ്കിൽ (status != XKB_SUCCESS) {
            പോകുക error;
        }
        ഘടന xkb_typing_event *event;
        darray_foreach(event, pending) {
            എങ്കിൽ (e >= അക്കം_events) {
                /* invariant violated: should be unreachable */
                PANIC_UNREACHABLE();
            }
            events[e] = *event;
            bit_array_assign(താക്കോലുകൾ_state, event->keycode, (bool)event->direction);
            e++;
        }

        എങ്കിൽ (!state.അക്കം_pending && e == അക്കം_events - 1) {
            /*
             * Cannot add press/release *pair*, so just use an invalid keycode
             * for the last event.
             */
            events[e] = (ഘടന xkb_typing_event) {
                .keycode = XKB_KEYCODE_INVALID,
                .direction = XKB_താക്കോൽ_DOWN,
            };
            നിർത്തുക;
        }

        എങ്കിൽ (e + 1 + state.അക്കം_pending >= അക്കം_events) {
            /* Cannot add new pair of events: require flushing pending events */
            തുടരുക;
        }

        /*
         * Create new events
         */

        /* Choose kind of key */
        static_assert(XKB_താക്കോൽ_PRINTABLE == 0, "");
        സ്ഥിരം എണ്ണൽ xkb_താക്കോൽ_kind താക്കോൽ_kind =
            pick_താക്കോൽ_kind(config->prng, config->prng_state);
        തിരഞ്ഞെടുപ്പ് (താക്കോൽ_kind) {
        അവസ്ഥ XKB_താക്കോൽ_PRINTABLE:
            status = add_other_താക്കോൽ(
                &state, &state.താക്കോലുകൾ->printable, &e,
                XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS
            );
            നിർത്തുക;
        അവസ്ഥ XKB_താക്കോൽ_MODIFIER:
            status = add_modifier_താക്കോൽ(&state, &e);
            നിർത്തുക;
        അവസ്ഥ XKB_താക്കോൽ_MISC:
            status = add_other_താക്കോൽ(
                &state, &state.താക്കോലുകൾ->misc, &e,
                XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS
            );
            നിർത്തുക;
        സ്ഥിരസ്ഥിതി: {
            static_assert(XKB_താക്കോൽ_MISC == 2 &&
                          XKB_താക്കോൽ_MISC == അക്കം_XKB_താക്കോൽ_KIND - 1, "");
            status = XKB_ERROR_INVALID;
        }}

        എങ്കിൽ (status != XKB_SUCCESS &&
            status != (എണ്ണൽ xkb_status)XKB_ERROR_NO_RELEASED_താക്കോൽ)
        {
            പോകുക error;
        }
    }

    പ്രതിജ്ഞാ(!state.അക്കം_pending);

    typing->events = steal(&state.events);
    typing->അക്കം_events = അക്കം_events;

    status = XKB_SUCCESS;

error:
    darray_free(pending);
    xkb_typing_state_destroy(&state);

    തിരികെ status;
}

ശൂന്യം
xkb_typing_നിവേശം_destroy(ഘടന xkb_typing_നിവേശം *typing)
{
    എങ്കിൽ (!typing)
        തിരികെ;

    സ്വതന്ത്രം(typing->events);
    typing->events = ശൂന്യ;
}
