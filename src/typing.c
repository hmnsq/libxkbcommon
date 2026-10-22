/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#समावेश "config.h"

#समावेश <assert.h>
#समावेश <stdbool.h>
#समावेश <stdlib.h>
#समावेश <stdint.h>
#अगर WCHAR_MAX >= 0x7FFFFFFF
#समावेश <wctype.h>
#घोषणा USE_ISWPRINT
#अगर HAVE_NEWLOCALE && HAVE_ISWPRINT_L
#घोषणा USE_ISWPRINT_L
#समावेश <locale.h>
#पूर्ण_अगर
#अन्यथा
#समावेश <ctype.h>
#पूर्ण_अगर

#समावेश "xkbcommon/xkbcommon-status.h"
#समावेश "xkbcommon/xkbcommon.h"
#समावेश "darray.h"
#समावेश "state-priv.h"
#समावेश "typing.h"
#समावेश "util-bits.h"
#समावेश "util-mem.h"
#समावेश "utils.h"

क्रमागत xkb_status
xkb_कुंजी_set_init(काष्ठा xkb_कुंजी_set *set,
                 काष्ठा xkb_keymap *keymap,
                 स्थिर काष्ठा xkb_कुंजी_set_config *config)
{
    darray_init(set->printable);
    darray_init(set->modifiers);
    darray_init(set->misc);

    अटल स्थिर काष्ठा xkb_कुंजी_set_config default_config = {0};
    अगर (!config)
        config = &default_config;
    /* keycode = 0 denotes “default” */
    स्थिर xkb_keycode_t min = MAX(config->min, xkb_keymap_min_keycode(keymap));
    स्थिर xkb_keycode_t max = config->max
        ? MIN(config->max, xkb_keymap_max_keycode(keymap))
        : xkb_keymap_max_keycode(keymap);

    क्रमागत xkb_status status;
    काष्ठा xkb_state *state =
        xkb_state_new_with_mode(keymap, XKB_STATE_MODE_SERVER, &status);
    अगर (!state)
        वापस status;

    #अगर_घोषित USE_ISWPRINT_L
    locale_t loc = newlocale(LC_ALL_MASK, "C.UTF-8", (locale_t)0);
    #पूर्ण_अगर

    काष्ठा xkb_keymap_कुंजी_iterator iter;
    स्थिर काष्ठा xkb_keymap_कुंजी_iterator_config iter_config = {
        .आकार = माप(iter_config),
        .flags = XKB_KEYMAP_कुंजी_ITERATOR_NO_FLAGS,
        .start = min
    };
    status = xkb_keymap_कुंजी_iterator_init(&iter, keymap, &iter_config);
    अगर (status != XKB_SUCCESS)
        जाओ error;

    स्थिर xkb_mod_mask_t numlock =
        xkb_keymap_mod_get_mask(keymap, XKB_VMOD_NAME_अंक);

    स्थिर काष्ठा xkb_state_components_update components_update = {
        .आकार = माप(components_update),
        .components = XKB_STATE_MODS_LOCKED,
        .affect_locked_mods = numlock,
        .locked_mods = numlock,
    };

    स्थिर काष्ठा xkb_synthetic_update update = {
        .आकार = माप(update),
        .components = &components_update,
    };

    xkb_keycode_t k;
    जबतक ((k = xkb_keymap_कुंजी_iterator_अग्र(&iter)) != XKB_KEYCODE_INVALID) {
        अगर (k > max)
            अवरोध;

        xkb_state_reset(state);
        status = xkb_state_update_synthetic(state, &update, शून्य);
        अगर (status != XKB_SUCCESS)
            जाओ error;

        स्थिर xkb_keysym_t *syms = शून्य;
        स्थिर पूर्णांक गिनती = xkb_state_कुंजी_get_syms(state, k, &syms);

        क्रमागत xkb_state_component changed =
            xkb_state_update_कुंजी(state, k, XKB_कुंजी_DOWN);
        xkb_mod_mask_t mods =
            xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);
        changed |= xkb_state_update_कुंजी(state, k, XKB_कुंजी_UP);
        mods |= xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);

        अगर (changed & XKB_STATE_MODS_EFFECTIVE) {
            /* Modifier key */
            क्रमागत xkb_modifier_kind kind;
            अगर (changed & XKB_STATE_MODS_LOCKED) {
                kind = XKB_MOD_LOCK;
            } अन्यथा अगर (changed & XKB_STATE_MODS_LATCHED) {
                kind = XKB_MOD_LATCH;
            } अन्यथा {
                kind = XKB_MOD_SET;
            }
            mods &= ~numlock;
            अगर (!mods)
                mods = numlock;
            darray_append(set->modifiers, (काष्ठा xkb_modifier_कुंजी) {
                .keycode = k,
                .mods = mods,
                .kind = kind,
            });
            जारी;
        }

        अगर (गिनती > 0) {
            uint32_t utf32 = 0;
            क्रम (पूर्णांक s = 0; s < गिनती; s++) {
                utf32 = xkb_keysym_to_utf32(syms[s]);
                स्थिर bool is_printable =
                    #अगर_घोषित USE_ISWPRINT_L
                    ((loc && iswprint_l((wint_t)utf32, loc)) || iswprint((wint_t)utf32))
                    #या_अगर defined(USE_ISWPRINT)
                    iswprint((wint_t)utf32)
                    #अन्यथा
                    utf32 <= 0x7f && है_छाप((पूर्णांक)utf32)
                    #पूर्ण_अगर
                    ;
                अगर (is_printable)
                    अवरोध;
                utf32 = 0;
            }
            अगर (utf32) {
                /* Printable keysyms */
                darray_append(set->printable, k);
                जारी;
            }
        }

        /* Miscellaneous key */
        darray_append(set->misc, k);
    }

error:
    #अगर_घोषित USE_ISWPRINT_L
    अगर (loc)
        freelocale(loc);
    #पूर्ण_अगर

    xkb_state_unref(state);
    अगर (status != XKB_SUCCESS)
        xkb_कुंजी_set_destroy(set);

    वापस status;
}

व्योम
xkb_कुंजी_set_destroy(काष्ठा xkb_कुंजी_set *set)
{
    अगर (!set)
        वापस;

    darray_free(set->modifiers);
    darray_free(set->printable);
    darray_free(set->misc);
}

अटल क्रमागत xkb_status
add_pending_events(काष्ठा xkb_typing_state * restrict state,
                   स्थिर काष्ठा xkb_pending_typing_event * restrict events,
                   माप_प्रकार गिनती)
{
    क्रम (माप_प्रकार e = 0; e < गिनती; e++) {
        /* Look for a free slot */
        काष्ठा xkb_pending_typing_event *pending;
        darray_आकार_t idx = 0;
        darray_enumerate_from(idx, pending, state->pending, idx) {
            अगर (!pending->गणक) {
                darray_item(state->pending, idx) = events[e];
                जाओ अग्र;
            }
        }
        /* No free slot found: append new one */
        idx = darray_आकार(state->pending);
        darray_append(state->pending, events[e]);
    अग्र:
        /* Avoid immediate consumption */
        darray_item(state->pending, idx).गणक++;
    }
    state->अंक_pending += गिनती;

    वापस XKB_SUCCESS;
}

अटल क्रमागत xkb_status
अग्र_pending_events(काष्ठा xkb_typing_state * restrict state,
                    xkb_typing_events * restrict out)
{
    /* Reset the output */
    darray_आकार(*out) = 0;

    darray_आकार_t idx;
    काष्ठा xkb_pending_typing_event *pending;
    /* Stack: start from the end */
    darray_enumerate_reverse(idx, pending, state->pending) {
        अगर (!pending->गणक)
            जारी;
        pending->गणक--;
        अगर (pending->गणक)
            जारी;
        darray_append(*out, pending->event);
        state->अंक_pending--;
        /* Resize array if it was the last element */
        अगर (idx + 1 == darray_आकार(state->pending))
            darray_आकार(state->pending)--;
        state->mods &= ~pending->mods;
    }

    वापस XKB_SUCCESS;
}

क्रमागत {
    /** Tries to avoid pressing a key already pressed */
    कुंजी_CHOICE_MAX_TRIES = 12,
    XKB_ERROR_NO_RELEASED_कुंजी = -1000,
};

अटल क्रमागत xkb_status
add_modifier_कुंजी(काष्ठा xkb_typing_state * restrict state, माप_प्रकार * restrict e)
{
    /* Choose a random key */
    दीर्घ idx = -1;
    क्रम (पूर्णांक tries = 0; tries < कुंजी_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state)
            % darray_आकार(state->कुंजियाँ->modifiers);
        /*
         * Ensure key is not already down and its modifier mask is not active,
         * i.e. to avoid simultaneous keys with the same modifier.
         */
        अगर (!bit_array_get(state->कुंजियाँ_state,
                           darray_item(state->कुंजियाँ->modifiers, idx).keycode) &&
            !(state->mods & darray_item(state->कुंजियाँ->modifiers, idx).mods))
        {
            अवरोध;
        }
        idx = -1;
    }

    काष्ठा xkb_modifier_कुंजी कुंजी = {0};
    अगर (idx < 0) {
        /* No released key found: fall back to linear search */
        स्थिर काष्ठा xkb_modifier_कुंजी *कुंजी_2;
        darray_foreach(कुंजी_2, state->कुंजियाँ->modifiers) {
            /* See test doc above */
            अगर (!bit_array_get(state->कुंजियाँ_state, कुंजी_2->keycode) &&
                !(state->mods & कुंजी_2->mods))
            {
                कुंजी = *कुंजी_2;
                idx = 0;
                अवरोध;
            }
        }
        अगर (idx < 0) {
            /* No fallback: skip */
            वापस (क्रमागत xkb_status)XKB_ERROR_NO_RELEASED_कुंजी;
        }
    } अन्यथा {
        कुंजी = darray_item(state->कुंजियाँ->modifiers, idx);
    }

    स्थिर माप_प्रकार remaining_events =
        state->अंक_events - state->अंक_pending - *e;
    माप_प्रकार pending_turns = 0;

    अगर (remaining_events >= 2) {
        अगर (कुंजी.kind == XKB_MOD_SET ||
            (कुंजी.kind == XKB_MOD_LOCK && remaining_events >= 4))
        {
            /* Delay SetMods release or LockMods unlock */
            स्थिर माप_प्रकार max_turns = MIN(
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
        वापस XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (काष्ठा xkb_typing_event) {
        .keycode = कुंजी.keycode,
        .direction = XKB_कुंजी_DOWN,
    };

    काष्ठा xkb_pending_typing_event pending[2] = {
        {
            .event = {
                .keycode = कुंजी.keycode,
                .direction = XKB_कुंजी_DOWN,
            },
            .गणक = (uint8_t)pending_turns,
        },
        {
            .event = {
                .keycode = कुंजी.keycode,
                .direction = XKB_कुंजी_UP,
            },
            .mods = कुंजी.mods,
            .गणक = (uint8_t)pending_turns,
        },
    };

    अगर (pending_turns) {
        अगर (कुंजी.kind == XKB_MOD_LOCK) {
            /* Immediate release */
            state->events[(*e)++] = pending[1].event;
            /* Delayed unlock (press + release) */
            pending[1].गणक++;
            स्थिर क्रमागत xkb_status status =
                add_pending_events(state, pending, ARRAY_आकार(pending));
            अगर (status != XKB_SUCCESS)
                वापस status;
            /*
             * Register “locked” state, to avoid tapping the key before
             * delayed key released
             */
            bit_array_set(state->कुंजियाँ_state, कुंजी.keycode);
        } अन्यथा {
            /* Delayed release */
            स्थिर क्रमागत xkb_status status =
                add_pending_events(state, &pending[1], 1);
            अगर (status != XKB_SUCCESS)
                वापस status;
            /* Register pressed state */
            bit_array_set(state->कुंजियाँ_state, कुंजी.keycode);
        }
        state->mods |= कुंजी.mods;
    } अन्यथा {
        /* Immediate release */
        state->events[(*e)++] = pending[1].event;
        /* TODO: add latched modifier to mask state. */
    }

    वापस XKB_SUCCESS;
}

अटल क्रमागत xkb_status
add_other_कुंजी(काष्ठा xkb_typing_state * restrict state,
              स्थिर xkb_keycodes_t * restrict keycodes, माप_प्रकार * restrict e,
              uint8_t immediate_release_prob, uint8_t max_pending_turns)
{
    /* Choose a random key */
    दीर्घ idx = -1;
    क्रम (पूर्णांक tries = 0; tries < कुंजी_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state) % darray_आकार(*keycodes);
        /* Ensure key is not already down */
        अगर (!bit_array_get(state->कुंजियाँ_state,
                           darray_item(*keycodes, idx)))
        {
            अवरोध;
        }
        idx = -1;
    }

    xkb_keycode_t keycode = XKB_KEYCODE_INVALID;
    अगर (idx < 0) {
        /* No released key found: fall back to linear search */
        स्थिर xkb_keycode_t *keycode_;
        darray_foreach(keycode_, *keycodes) {
            अगर (!bit_array_get(state->कुंजियाँ_state, *keycode_)) {
                keycode = *keycode_;
                idx = 0;
                अवरोध;
            }
        }
        अगर (idx < 0) {
            /* No fallback: skip */
            वापस (क्रमागत xkb_status)XKB_ERROR_NO_RELEASED_कुंजी;
        }
    } अन्यथा {
        keycode = darray_item(*keycodes, idx);
    }

    स्थिर माप_प्रकार remaining_events =
        state->अंक_events - state->अंक_pending - *e;
    माप_प्रकार pending_turns;

    अगर (remaining_events >= 2) {
        स्थिर माप_प्रकार max_turns = MIN(
            (माप_प्रकार)max_pending_turns,
            remaining_events + state->अंक_pending
        );
        स्थिर bool immediate_release = (
            (immediate_release_prob >= 100) ||
            !max_turns ||
            (state->prng(state->prng_state) % 100) < immediate_release_prob
        );
        अगर (immediate_release) {
            pending_turns = 0;
        } अन्यथा {
            pending_turns = 1 + (माप_प्रकार)state->prng(state->prng_state)
                          % max_turns;
        }
    } अन्यथा {
        वापस XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (काष्ठा xkb_typing_event) {
        .keycode = keycode,
        .direction = XKB_कुंजी_DOWN,
    };

    स्थिर काष्ठा xkb_pending_typing_event pending = {
        .event = {
            .keycode = keycode,
            .direction = XKB_कुंजी_UP,
        },
        .गणक = (uint8_t)pending_turns,
    };
    अगर (pending_turns) {
        स्थिर क्रमागत xkb_status status = add_pending_events(state, &pending, 1);
        अगर (status != XKB_SUCCESS)
            वापस status;
        /* Register pressed state */
        bit_array_set(state->कुंजियाँ_state, keycode);
    } अन्यथा {
        state->events[(*e)++] = pending.event;
    }

    वापस XKB_SUCCESS;
}

अटल क्रमागत xkb_कुंजी_kind
pick_कुंजी_kind(xkb_prng_t prng, व्योम *prng_state)
{
    अटल स्थिर uint8_t weights[अंक_XKB_कुंजी_KIND] = {
        [XKB_कुंजी_PRINTABLE] = 60,
        [XKB_कुंजी_MODIFIER] = 30,
        [XKB_कुंजी_MISC] = 10,
    };
    static_assert(XKB_कुंजी_MISC == 2 &&
                  XKB_कुंजी_MISC == अंक_XKB_कुंजी_KIND - 1, "");
    निश्चित(weights[0] + weights[1] + weights[2] == 100);

    दीर्घ r = prng(prng_state) % 100;
    क्रम (क्रमागत xkb_कुंजी_kind k = 0; k < अंक_XKB_कुंजी_KIND; k++)
        अगर ((r -= weights[k]) < 0)
            वापस k;
    वापस XKB_कुंजी_PRINTABLE;
}

अटल व्योम
xkb_typing_state_destroy(काष्ठा xkb_typing_state *state)
{
    मुक्त(state->कुंजियाँ_state);
    darray_free(state->pending);
    मुक्त(state->events);
}

क्रमागत xkb_status
xkb_typing_निवेश_init(काष्ठा xkb_typing_निवेश * restrict typing,
                      स्थिर काष्ठा xkb_कुंजी_set * restrict कुंजियाँ,
                      स्थिर काष्ठा xkb_typing_निवेश_config * restrict config)
{
    typing->events = शून्य;
    typing->अंक_events = 0;

    अगर (!config->निवेश_लंबाई)
        वापस XKB_ERROR_INVALID;

    अगर (darray_empty(कुंजियाँ->printable) ||
        darray_empty(कुंजियाँ->modifiers) ||
        darray_empty(कुंजियाँ->misc))
    {
        वापस XKB_ERROR_INVALID;
    }

    क्रमागत xkb_status status = XKB_SUCCESS;

    /* NOTE: arrays are sorted because they were built using the key iterator */
    xkb_keycode_t keycode_max = darray_last(कुंजियाँ->printable, 0);
    keycode_max = MAX(
        darray_last(कुंजियाँ->modifiers, (काष्ठा xkb_modifier_कुंजी){0}).keycode,
        keycode_max
    );
    keycode_max = MAX(darray_last(कुंजियाँ->misc, 0), keycode_max);
    स्थिर माप_प्रकार कुंजियाँ_state_अंक = bit_array_शब्दगण((माप_प्रकार)keycode_max + 1);
    uintptr_t *कुंजियाँ_state = सुस्मृति(कुंजियाँ_state_अंक, माप(*कुंजियाँ_state));

    स्थिर माप_प्रकार अंक_events = config->निवेश_लंबाई;
    काष्ठा xkb_typing_event *events = सुस्मृति(अंक_events, माप(*events));

    काष्ठा xkb_typing_state state = {
        .कुंजियाँ = कुंजियाँ,
        .कुंजियाँ_state = कुंजियाँ_state,
        .prng = config->prng,
        .prng_state = config->prng_state,
        .events = events,
        .अंक_events = अंक_events,
        .pending = darray_new(),
        .अंक_pending = 0,
    };

    xkb_typing_events pending = darray_new();

    अगर (!कुंजियाँ_state || !events) {
        status = XKB_ERROR_ALLOCATION_FAILURE;
        जाओ error;
    }

    /*
     * Invariant: e + state.num_pending <= state.num_events
     */

    क्रम (माप_प्रकार e = 0; e < अंक_events;) {
        /*
         * Process pending events
         */
        status = अग्र_pending_events(&state, &pending);
        अगर (status != XKB_SUCCESS) {
            जाओ error;
        }
        काष्ठा xkb_typing_event *event;
        darray_foreach(event, pending) {
            अगर (e >= अंक_events) {
                /* invariant violated: should be unreachable */
                PANIC_UNREACHABLE();
            }
            events[e] = *event;
            bit_array_assign(कुंजियाँ_state, event->keycode, (bool)event->direction);
            e++;
        }

        अगर (!state.अंक_pending && e == अंक_events - 1) {
            /*
             * Cannot add press/release *pair*, so just use an invalid keycode
             * for the last event.
             */
            events[e] = (काष्ठा xkb_typing_event) {
                .keycode = XKB_KEYCODE_INVALID,
                .direction = XKB_कुंजी_DOWN,
            };
            अवरोध;
        }

        अगर (e + 1 + state.अंक_pending >= अंक_events) {
            /* Cannot add new pair of events: require flushing pending events */
            जारी;
        }

        /*
         * Create new events
         */

        /* Choose kind of key */
        static_assert(XKB_कुंजी_PRINTABLE == 0, "");
        स्थिर क्रमागत xkb_कुंजी_kind कुंजी_kind =
            pick_कुंजी_kind(config->prng, config->prng_state);
        चयन (कुंजी_kind) {
        हाल XKB_कुंजी_PRINTABLE:
            status = add_other_कुंजी(
                &state, &state.कुंजियाँ->printable, &e,
                XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS
            );
            अवरोध;
        हाल XKB_कुंजी_MODIFIER:
            status = add_modifier_कुंजी(&state, &e);
            अवरोध;
        हाल XKB_कुंजी_MISC:
            status = add_other_कुंजी(
                &state, &state.कुंजियाँ->misc, &e,
                XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS
            );
            अवरोध;
        शेष: {
            static_assert(XKB_कुंजी_MISC == 2 &&
                          XKB_कुंजी_MISC == अंक_XKB_कुंजी_KIND - 1, "");
            status = XKB_ERROR_INVALID;
        }}

        अगर (status != XKB_SUCCESS &&
            status != (क्रमागत xkb_status)XKB_ERROR_NO_RELEASED_कुंजी)
        {
            जाओ error;
        }
    }

    निश्चित(!state.अंक_pending);

    typing->events = steal(&state.events);
    typing->अंक_events = अंक_events;

    status = XKB_SUCCESS;

error:
    darray_free(pending);
    xkb_typing_state_destroy(&state);

    वापस status;
}

व्योम
xkb_typing_निवेश_destroy(काष्ठा xkb_typing_निवेश *typing)
{
    अगर (!typing)
        वापस;

    मुक्त(typing->events);
    typing->events = शून्य;
}
