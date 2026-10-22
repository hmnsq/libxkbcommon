/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <stdbool.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <stdint.h>
#જો WCHAR_MAX >= 0x7FFFFFFF
#અન્તર્ભાવ <wctype.h>
#define USE_ISWPRINT
#જો HAVE_NEWLOCALE && HAVE_ISWPRINT_L
#define USE_ISWPRINT_L
#અન્તર્ભાવ <locale.h>
#endif
#નહીંતર
#અન્તર્ભાવ <ctype.h>
#endif

#અન્તર્ભાવ "xkbcommon/xkbcommon-status.h"
#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "darray.h"
#અન્તર્ભાવ "state-priv.h"
#અન્તર્ભાવ "typing.h"
#અન્તર્ભાવ "util-bits.h"
#અન્તર્ભાવ "util-mem.h"
#અન્તર્ભાવ "utils.h"

ગણના xkb_status
xkb_ચાવી_set_init(સંરચના xkb_ચાવી_set *set,
                 સંરચના xkb_keymap *keymap,
                 અચલ સંરચના xkb_ચાવી_set_config *config)
{
    darray_init(set->printable);
    darray_init(set->modifiers);
    darray_init(set->misc);

    સ્થિર અચલ સંરચના xkb_ચાવી_set_config default_config = {0};
    જો (!config)
        config = &default_config;
    /* keycode = 0 denotes “default” */
    અચલ xkb_keycode_t min = MAX(config->min, xkb_keymap_min_keycode(keymap));
    અચલ xkb_keycode_t max = config->max
        ? MIN(config->max, xkb_keymap_max_keycode(keymap))
        : xkb_keymap_max_keycode(keymap);

    ગણના xkb_status status;
    સંરચના xkb_state *state =
        xkb_state_new_with_mode(keymap, XKB_STATE_MODE_SERVER, &status);
    જો (!state)
        પાછા status;

    #ifdef USE_ISWPRINT_L
    locale_t loc = newlocale(LC_ALL_MASK, "C.UTF-8", (locale_t)0);
    #endif

    સંરચના xkb_keymap_ચાવી_iterator iter;
    અચલ સંરચના xkb_keymap_ચાવી_iterator_config iter_config = {
        .size = કદ(iter_config),
        .flags = XKB_KEYMAP_ચાવી_ITERATOR_NO_FLAGS,
        .start = min
    };
    status = xkb_keymap_ચાવી_iterator_init(&iter, keymap, &iter_config);
    જો (status != XKB_SUCCESS)
        જાઓ error;

    અચલ xkb_mod_mask_t numlock =
        xkb_keymap_mod_get_mask(keymap, XKB_VMOD_NAME_અંક);

    અચલ સંરચના xkb_state_components_update components_update = {
        .size = કદ(components_update),
        .components = XKB_STATE_MODS_LOCKED,
        .affect_locked_mods = numlock,
        .locked_mods = numlock,
    };

    અચલ સંરચના xkb_synthetic_update update = {
        .size = કદ(update),
        .components = &components_update,
    };

    xkb_keycode_t k;
    જ્યારે ((k = xkb_keymap_ચાવી_iterator_next(&iter)) != XKB_KEYCODE_INVALID) {
        જો (k > max)
            તોડો;

        xkb_state_reset(state);
        status = xkb_state_update_synthetic(state, &update, ખાલી);
        જો (status != XKB_SUCCESS)
            જાઓ error;

        અચલ xkb_keysym_t *syms = ખાલી;
        અચલ પૂર્ણાંક ગણતરી = xkb_state_ચાવી_get_syms(state, k, &syms);

        ગણના xkb_state_component changed =
            xkb_state_update_ચાવી(state, k, XKB_ચાવી_DOWN);
        xkb_mod_mask_t mods =
            xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);
        changed |= xkb_state_update_ચાવી(state, k, XKB_ચાવી_UP);
        mods |= xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);

        જો (changed & XKB_STATE_MODS_EFFECTIVE) {
            /* Modifier key */
            ગણના xkb_modifier_kind kind;
            જો (changed & XKB_STATE_MODS_LOCKED) {
                kind = XKB_MOD_LOCK;
            } નહીંતર જો (changed & XKB_STATE_MODS_LATCHED) {
                kind = XKB_MOD_LATCH;
            } નહીંતર {
                kind = XKB_MOD_SET;
            }
            mods &= ~numlock;
            જો (!mods)
                mods = numlock;
            darray_append(set->modifiers, (સંરચના xkb_modifier_ચાવી) {
                .keycode = k,
                .mods = mods,
                .kind = kind,
            });
            ચાલુ;
        }

        જો (ગણતરી > 0) {
            uint32_t utf32 = 0;
            માટે (પૂર્ણાંક s = 0; s < ગણતરી; s++) {
                utf32 = xkb_keysym_to_utf32(syms[s]);
                અચલ bool is_printable =
                    #ifdef USE_ISWPRINT_L
                    ((loc && iswprint_l((wint_t)utf32, loc)) || iswprint((wint_t)utf32))
                    #અથવાજો defined(USE_ISWPRINT)
                    iswprint((wint_t)utf32)
                    #નહીંતર
                    utf32 <= 0x7f && isprint((પૂર્ણાંક)utf32)
                    #endif
                    ;
                જો (is_printable)
                    તોડો;
                utf32 = 0;
            }
            જો (utf32) {
                /* Printable keysyms */
                darray_append(set->printable, k);
                ચાલુ;
            }
        }

        /* Miscellaneous key */
        darray_append(set->misc, k);
    }

error:
    #ifdef USE_ISWPRINT_L
    જો (loc)
        freelocale(loc);
    #endif

    xkb_state_unref(state);
    જો (status != XKB_SUCCESS)
        xkb_ચાવી_set_destroy(set);

    પાછા status;
}

શૂન્ય
xkb_ચાવી_set_destroy(સંરચના xkb_ચાવી_set *set)
{
    જો (!set)
        પાછા;

    darray_free(set->modifiers);
    darray_free(set->printable);
    darray_free(set->misc);
}

સ્થિર ગણના xkb_status
add_pending_events(સંરચના xkb_typing_state * restrict state,
                   અચલ સંરચના xkb_pending_typing_event * restrict events,
                   size_t ગણતરી)
{
    માટે (size_t e = 0; e < ગણતરી; e++) {
        /* Look for a free slot */
        સંરચના xkb_pending_typing_event *pending;
        darray_size_t idx = 0;
        darray_enumerate_from(idx, pending, state->pending, idx) {
            જો (!pending->counter) {
                darray_item(state->pending, idx) = events[e];
                જાઓ next;
            }
        }
        /* No free slot found: append new one */
        idx = darray_size(state->pending);
        darray_append(state->pending, events[e]);
    next:
        /* Avoid immediate consumption */
        darray_item(state->pending, idx).counter++;
    }
    state->અંક_pending += ગણતરી;

    પાછા XKB_SUCCESS;
}

સ્થિર ગણના xkb_status
next_pending_events(સંરચના xkb_typing_state * restrict state,
                    xkb_typing_events * restrict out)
{
    /* Reset the output */
    darray_size(*out) = 0;

    darray_size_t idx;
    સંરચના xkb_pending_typing_event *pending;
    /* Stack: start from the end */
    darray_enumerate_reverse(idx, pending, state->pending) {
        જો (!pending->counter)
            ચાલુ;
        pending->counter--;
        જો (pending->counter)
            ચાલુ;
        darray_append(*out, pending->event);
        state->અંક_pending--;
        /* Resize array if it was the last element */
        જો (idx + 1 == darray_size(state->pending))
            darray_size(state->pending)--;
        state->mods &= ~pending->mods;
    }

    પાછા XKB_SUCCESS;
}

ગણના {
    /** Tries to avoid pressing a key already pressed */
    ચાવી_CHOICE_MAX_TRIES = 12,
    XKB_ERROR_NO_RELEASED_ચાવી = -1000,
};

સ્થિર ગણના xkb_status
add_modifier_ચાવી(સંરચના xkb_typing_state * restrict state, size_t * restrict e)
{
    /* Choose a random key */
    long idx = -1;
    માટે (પૂર્ણાંક tries = 0; tries < ચાવી_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state)
            % darray_size(state->ચાવીઓ->modifiers);
        /*
         * Ensure key is not already down and its modifier mask is not active,
         * i.e. to avoid simultaneous keys with the same modifier.
         */
        જો (!bit_array_get(state->ચાવીઓ_state,
                           darray_item(state->ચાવીઓ->modifiers, idx).keycode) &&
            !(state->mods & darray_item(state->ચાવીઓ->modifiers, idx).mods))
        {
            તોડો;
        }
        idx = -1;
    }

    સંરચના xkb_modifier_ચાવી ચાવી = {0};
    જો (idx < 0) {
        /* No released key found: fall back to linear search */
        અચલ સંરચના xkb_modifier_ચાવી *ચાવી_2;
        darray_foreach(ચાવી_2, state->ચાવીઓ->modifiers) {
            /* See test doc above */
            જો (!bit_array_get(state->ચાવીઓ_state, ચાવી_2->keycode) &&
                !(state->mods & ચાવી_2->mods))
            {
                ચાવી = *ચાવી_2;
                idx = 0;
                તોડો;
            }
        }
        જો (idx < 0) {
            /* No fallback: skip */
            પાછા (ગણના xkb_status)XKB_ERROR_NO_RELEASED_ચાવી;
        }
    } નહીંતર {
        ચાવી = darray_item(state->ચાવીઓ->modifiers, idx);
    }

    અચલ size_t remaining_events =
        state->અંક_events - state->અંક_pending - *e;
    size_t pending_turns = 0;

    જો (remaining_events >= 2) {
        જો (ચાવી.kind == XKB_MOD_SET ||
            (ચાવી.kind == XKB_MOD_LOCK && remaining_events >= 4))
        {
            /* Delay SetMods release or LockMods unlock */
            અચલ size_t max_turns = MIN(
                (size_t)(
                    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS -
                    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                 ),
                remaining_events + state->અંક_pending
            );
            pending_turns = XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                          + (size_t)state->prng(state->prng_state)
                          % (max_turns + 1);
        }
    } નહીંતર {
        પાછા XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (સંરચના xkb_typing_event) {
        .keycode = ચાવી.keycode,
        .direction = XKB_ચાવી_DOWN,
    };

    સંરચના xkb_pending_typing_event pending[2] = {
        {
            .event = {
                .keycode = ચાવી.keycode,
                .direction = XKB_ચાવી_DOWN,
            },
            .counter = (uint8_t)pending_turns,
        },
        {
            .event = {
                .keycode = ચાવી.keycode,
                .direction = XKB_ચાવી_UP,
            },
            .mods = ચાવી.mods,
            .counter = (uint8_t)pending_turns,
        },
    };

    જો (pending_turns) {
        જો (ચાવી.kind == XKB_MOD_LOCK) {
            /* Immediate release */
            state->events[(*e)++] = pending[1].event;
            /* Delayed unlock (press + release) */
            pending[1].counter++;
            અચલ ગણના xkb_status status =
                add_pending_events(state, pending, ARRAY_SIZE(pending));
            જો (status != XKB_SUCCESS)
                પાછા status;
            /*
             * Register “locked” state, to avoid tapping the key before
             * delayed key released
             */
            bit_array_set(state->ચાવીઓ_state, ચાવી.keycode);
        } નહીંતર {
            /* Delayed release */
            અચલ ગણના xkb_status status =
                add_pending_events(state, &pending[1], 1);
            જો (status != XKB_SUCCESS)
                પાછા status;
            /* Register pressed state */
            bit_array_set(state->ચાવીઓ_state, ચાવી.keycode);
        }
        state->mods |= ચાવી.mods;
    } નહીંતર {
        /* Immediate release */
        state->events[(*e)++] = pending[1].event;
        /* TODO: add latched modifier to mask state. */
    }

    પાછા XKB_SUCCESS;
}

સ્થિર ગણના xkb_status
add_other_ચાવી(સંરચના xkb_typing_state * restrict state,
              અચલ xkb_keycodes_t * restrict keycodes, size_t * restrict e,
              uint8_t immediate_release_prob, uint8_t max_pending_turns)
{
    /* Choose a random key */
    long idx = -1;
    માટે (પૂર્ણાંક tries = 0; tries < ચાવી_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state) % darray_size(*keycodes);
        /* Ensure key is not already down */
        જો (!bit_array_get(state->ચાવીઓ_state,
                           darray_item(*keycodes, idx)))
        {
            તોડો;
        }
        idx = -1;
    }

    xkb_keycode_t keycode = XKB_KEYCODE_INVALID;
    જો (idx < 0) {
        /* No released key found: fall back to linear search */
        અચલ xkb_keycode_t *keycode_;
        darray_foreach(keycode_, *keycodes) {
            જો (!bit_array_get(state->ચાવીઓ_state, *keycode_)) {
                keycode = *keycode_;
                idx = 0;
                તોડો;
            }
        }
        જો (idx < 0) {
            /* No fallback: skip */
            પાછા (ગણના xkb_status)XKB_ERROR_NO_RELEASED_ચાવી;
        }
    } નહીંતર {
        keycode = darray_item(*keycodes, idx);
    }

    અચલ size_t remaining_events =
        state->અંક_events - state->અંક_pending - *e;
    size_t pending_turns;

    જો (remaining_events >= 2) {
        અચલ size_t max_turns = MIN(
            (size_t)max_pending_turns,
            remaining_events + state->અંક_pending
        );
        અચલ bool immediate_release = (
            (immediate_release_prob >= 100) ||
            !max_turns ||
            (state->prng(state->prng_state) % 100) < immediate_release_prob
        );
        જો (immediate_release) {
            pending_turns = 0;
        } નહીંતર {
            pending_turns = 1 + (size_t)state->prng(state->prng_state)
                          % max_turns;
        }
    } નહીંતર {
        પાછા XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (સંરચના xkb_typing_event) {
        .keycode = keycode,
        .direction = XKB_ચાવી_DOWN,
    };

    અચલ સંરચના xkb_pending_typing_event pending = {
        .event = {
            .keycode = keycode,
            .direction = XKB_ચાવી_UP,
        },
        .counter = (uint8_t)pending_turns,
    };
    જો (pending_turns) {
        અચલ ગણના xkb_status status = add_pending_events(state, &pending, 1);
        જો (status != XKB_SUCCESS)
            પાછા status;
        /* Register pressed state */
        bit_array_set(state->ચાવીઓ_state, keycode);
    } નહીંતર {
        state->events[(*e)++] = pending.event;
    }

    પાછા XKB_SUCCESS;
}

સ્થિર ગણના xkb_ચાવી_kind
pick_ચાવી_kind(xkb_prng_t prng, શૂન્ય *prng_state)
{
    સ્થિર અચલ uint8_t weights[અંક_XKB_ચાવી_KIND] = {
        [XKB_ચાવી_PRINTABLE] = 60,
        [XKB_ચાવી_MODIFIER] = 30,
        [XKB_ચાવી_MISC] = 10,
    };
    static_assert(XKB_ચાવી_MISC == 2 &&
                  XKB_ચાવી_MISC == અંક_XKB_ચાવી_KIND - 1, "");
    પ્રતિજ્ઞા(weights[0] + weights[1] + weights[2] == 100);

    long r = prng(prng_state) % 100;
    માટે (ગણના xkb_ચાવી_kind k = 0; k < અંક_XKB_ચાવી_KIND; k++)
        જો ((r -= weights[k]) < 0)
            પાછા k;
    પાછા XKB_ચાવી_PRINTABLE;
}

સ્થિર શૂન્ય
xkb_typing_state_destroy(સંરચના xkb_typing_state *state)
{
    મુક્ત(state->ચાવીઓ_state);
    darray_free(state->pending);
    મુક્ત(state->events);
}

ગણના xkb_status
xkb_typing_આગત_init(સંરચના xkb_typing_આગત * restrict typing,
                      અચલ સંરચના xkb_ચાવી_set * restrict ચાવીઓ,
                      અચલ સંરચના xkb_typing_આગત_config * restrict config)
{
    typing->events = ખાલી;
    typing->અંક_events = 0;

    જો (!config->આગત_length)
        પાછા XKB_ERROR_INVALID;

    જો (darray_empty(ચાવીઓ->printable) ||
        darray_empty(ચાવીઓ->modifiers) ||
        darray_empty(ચાવીઓ->misc))
    {
        પાછા XKB_ERROR_INVALID;
    }

    ગણના xkb_status status = XKB_SUCCESS;

    /* NOTE: arrays are sorted because they were built using the key iterator */
    xkb_keycode_t keycode_max = darray_last(ચાવીઓ->printable, 0);
    keycode_max = MAX(
        darray_last(ચાવીઓ->modifiers, (સંરચના xkb_modifier_ચાવી){0}).keycode,
        keycode_max
    );
    keycode_max = MAX(darray_last(ચાવીઓ->misc, 0), keycode_max);
    અચલ size_t ચાવીઓ_state_અંક = bit_array_શબ્દો((size_t)keycode_max + 1);
    uintptr_t *ચાવીઓ_state = calloc(ચાવીઓ_state_અંક, કદ(*ચાવીઓ_state));

    અચલ size_t અંક_events = config->આગત_length;
    સંરચના xkb_typing_event *events = calloc(અંક_events, કદ(*events));

    સંરચના xkb_typing_state state = {
        .ચાવીઓ = ચાવીઓ,
        .ચાવીઓ_state = ચાવીઓ_state,
        .prng = config->prng,
        .prng_state = config->prng_state,
        .events = events,
        .અંક_events = અંક_events,
        .pending = darray_new(),
        .અંક_pending = 0,
    };

    xkb_typing_events pending = darray_new();

    જો (!ચાવીઓ_state || !events) {
        status = XKB_ERROR_ALLOCATION_FAILURE;
        જાઓ error;
    }

    /*
     * Invariant: e + state.num_pending <= state.num_events
     */

    માટે (size_t e = 0; e < અંક_events;) {
        /*
         * Process pending events
         */
        status = next_pending_events(&state, &pending);
        જો (status != XKB_SUCCESS) {
            જાઓ error;
        }
        સંરચના xkb_typing_event *event;
        darray_foreach(event, pending) {
            જો (e >= અંક_events) {
                /* invariant violated: should be unreachable */
                PANIC_UNREACHABLE();
            }
            events[e] = *event;
            bit_array_assign(ચાવીઓ_state, event->keycode, (bool)event->direction);
            e++;
        }

        જો (!state.અંક_pending && e == અંક_events - 1) {
            /*
             * Cannot add press/release *pair*, so just use an invalid keycode
             * for the last event.
             */
            events[e] = (સંરચના xkb_typing_event) {
                .keycode = XKB_KEYCODE_INVALID,
                .direction = XKB_ચાવી_DOWN,
            };
            તોડો;
        }

        જો (e + 1 + state.અંક_pending >= અંક_events) {
            /* Cannot add new pair of events: require flushing pending events */
            ચાલુ;
        }

        /*
         * Create new events
         */

        /* Choose kind of key */
        static_assert(XKB_ચાવી_PRINTABLE == 0, "");
        અચલ ગણના xkb_ચાવી_kind ચાવી_kind =
            pick_ચાવી_kind(config->prng, config->prng_state);
        પસંદગી (ચાવી_kind) {
        સ્થિતિ XKB_ચાવી_PRINTABLE:
            status = add_other_ચાવી(
                &state, &state.ચાવીઓ->printable, &e,
                XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS
            );
            તોડો;
        સ્થિતિ XKB_ચાવી_MODIFIER:
            status = add_modifier_ચાવી(&state, &e);
            તોડો;
        સ્થિતિ XKB_ચાવી_MISC:
            status = add_other_ચાવી(
                &state, &state.ચાવીઓ->misc, &e,
                XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS
            );
            તોડો;
        મૂળભૂત: {
            static_assert(XKB_ચાવી_MISC == 2 &&
                          XKB_ચાવી_MISC == અંક_XKB_ચાવી_KIND - 1, "");
            status = XKB_ERROR_INVALID;
        }}

        જો (status != XKB_SUCCESS &&
            status != (ગણના xkb_status)XKB_ERROR_NO_RELEASED_ચાવી)
        {
            જાઓ error;
        }
    }

    પ્રતિજ્ઞા(!state.અંક_pending);

    typing->events = steal(&state.events);
    typing->અંક_events = અંક_events;

    status = XKB_SUCCESS;

error:
    darray_free(pending);
    xkb_typing_state_destroy(&state);

    પાછા status;
}

શૂન્ય
xkb_typing_આગત_destroy(સંરચના xkb_typing_આગત *typing)
{
    જો (!typing)
        પાછા;

    મુક્ત(typing->events);
    typing->events = ખાલી;
}
