/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಯದಿ WCHAR_MAX >= 0x7FFFFFFF
#ಅನ್ತರ್ಭಾವ <wctype.h>
#define USE_ISWPRINT
#ಯದಿ HAVE_NEWLOCALE && HAVE_ISWPRINT_L
#define USE_ISWPRINT_L
#ಅನ್ತರ್ಭಾವ <locale.h>
#endif
#ಅನ್ಯಥಾ
#ಅನ್ತರ್ಭಾವ <ctype.h>
#endif

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-status.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "state-priv.h"
#ಅನ್ತರ್ಭಾವ "typing.h"
#ಅನ್ತರ್ಭಾವ "util-bits.h"
#ಅನ್ತರ್ಭಾವ "util-mem.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

ಗಣನಾ xkb_status
xkb_ಕೀಲಿ_set_init(ಸಂರಚನಾ xkb_ಕೀಲಿ_set *set,
                 ಸಂರಚನಾ xkb_keymap *keymap,
                 ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_set_config *config)
{
    darray_init(set->printable);
    darray_init(set->modifiers);
    darray_init(set->misc);

    ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_set_config default_config = {0};
    ಯದಿ (!config)
        config = &default_config;
    /* keycode = 0 denotes “default” */
    ಧ್ರುವ xkb_keycode_t min = MAX(config->min, xkb_keymap_min_keycode(keymap));
    ಧ್ರುವ xkb_keycode_t max = config->max
        ? MIN(config->max, xkb_keymap_max_keycode(keymap))
        : xkb_keymap_max_keycode(keymap);

    ಗಣನಾ xkb_status status;
    ಸಂರಚನಾ xkb_state *state =
        xkb_state_new_with_mode(keymap, XKB_STATE_MODE_SERVER, &status);
    ಯದಿ (!state)
        ಪ್ರತ್ಯಾಗಮ status;

    #ifdef USE_ISWPRINT_L
    locale_t loc = newlocale(LC_ALL_MASK, "C.UTF-8", (locale_t)0);
    #endif

    ಸಂರಚನಾ xkb_keymap_ಕೀಲಿ_iterator iter;
    ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_ಕೀಲಿ_iterator_config iter_config = {
        .size = ಪರಿಮಾಣ(iter_config),
        .flags = XKB_KEYMAP_ಕೀಲಿ_ITERATOR_NO_FLAGS,
        .start = min
    };
    status = xkb_keymap_ಕೀಲಿ_iterator_init(&iter, keymap, &iter_config);
    ಯದಿ (status != XKB_SUCCESS)
        ಗಚ್ಛ error;

    ಧ್ರುವ xkb_mod_mask_t numlock =
        xkb_keymap_mod_get_mask(keymap, XKB_VMOD_NAME_ಅಂಕೆ);

    ಧ್ರುವ ಸಂರಚನಾ xkb_state_components_update components_update = {
        .size = ಪರಿಮಾಣ(components_update),
        .components = XKB_STATE_MODS_LOCKED,
        .affect_locked_mods = numlock,
        .locked_mods = numlock,
    };

    ಧ್ರುವ ಸಂರಚನಾ xkb_synthetic_update update = {
        .size = ಪರಿಮಾಣ(update),
        .components = &components_update,
    };

    xkb_keycode_t k;
    ಯಾವತ್ ((k = xkb_keymap_ಕೀಲಿ_iterator_next(&iter)) != XKB_KEYCODE_INVALID) {
        ಯದಿ (k > max)
            ಭಙ್ಗ;

        xkb_state_reset(state);
        status = xkb_state_update_synthetic(state, &update, ರಿಕ್ತ);
        ಯದಿ (status != XKB_SUCCESS)
            ಗಚ್ಛ error;

        ಧ್ರುವ xkb_keysym_t *syms = ರಿಕ್ತ;
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = xkb_state_ಕೀಲಿ_get_syms(state, k, &syms);

        ಗಣನಾ xkb_state_component changed =
            xkb_state_update_ಕೀಲಿ(state, k, XKB_ಕೀಲಿ_DOWN);
        xkb_mod_mask_t mods =
            xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);
        changed |= xkb_state_update_ಕೀಲಿ(state, k, XKB_ಕೀಲಿ_UP);
        mods |= xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);

        ಯದಿ (changed & XKB_STATE_MODS_EFFECTIVE) {
            /* Modifier key */
            ಗಣನಾ xkb_modifier_kind kind;
            ಯದಿ (changed & XKB_STATE_MODS_LOCKED) {
                kind = XKB_MOD_LOCK;
            } ಅನ್ಯಥಾ ಯದಿ (changed & XKB_STATE_MODS_LATCHED) {
                kind = XKB_MOD_LATCH;
            } ಅನ್ಯಥಾ {
                kind = XKB_MOD_SET;
            }
            mods &= ~numlock;
            ಯದಿ (!mods)
                mods = numlock;
            darray_append(set->modifiers, (ಸಂರಚನಾ xkb_modifier_ಕೀಲಿ) {
                .keycode = k,
                .mods = mods,
                .kind = kind,
            });
            ಅನುವರ್ತ;
        }

        ಯದಿ (ಎಣಿಕೆ > 0) {
            uint32_t utf32 = 0;
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ s = 0; s < ಎಣಿಕೆ; s++) {
                utf32 = xkb_keysym_to_utf32(syms[s]);
                ಧ್ರುವ bool is_printable =
                    #ifdef USE_ISWPRINT_L
                    ((loc && iswprint_l((wint_t)utf32, loc)) || iswprint((wint_t)utf32))
                    #ನೋಚೇದ್ಯದಿ defined(USE_ISWPRINT)
                    iswprint((wint_t)utf32)
                    #ಅನ್ಯಥಾ
                    utf32 <= 0x7f && isprint((ಪೂರ್ಣಾಙ್ಕ)utf32)
                    #endif
                    ;
                ಯದಿ (is_printable)
                    ಭಙ್ಗ;
                utf32 = 0;
            }
            ಯದಿ (utf32) {
                /* Printable keysyms */
                darray_append(set->printable, k);
                ಅನುವರ್ತ;
            }
        }

        /* Miscellaneous key */
        darray_append(set->misc, k);
    }

error:
    #ifdef USE_ISWPRINT_L
    ಯದಿ (loc)
        freelocale(loc);
    #endif

    xkb_state_unref(state);
    ಯದಿ (status != XKB_SUCCESS)
        xkb_ಕೀಲಿ_set_destroy(set);

    ಪ್ರತ್ಯಾಗಮ status;
}

ಶೂನ್ಯ
xkb_ಕೀಲಿ_set_destroy(ಸಂರಚನಾ xkb_ಕೀಲಿ_set *set)
{
    ಯದಿ (!set)
        ಪ್ರತ್ಯಾಗಮ;

    darray_free(set->modifiers);
    darray_free(set->printable);
    darray_free(set->misc);
}

ಸ್ಥಿರ ಗಣನಾ xkb_status
add_pending_events(ಸಂರಚನಾ xkb_typing_state * restrict state,
                   ಧ್ರುವ ಸಂರಚನಾ xkb_pending_typing_event * restrict events,
                   size_t ಎಣಿಕೆ)
{
    ಪ್ರತಿ (size_t e = 0; e < ಎಣಿಕೆ; e++) {
        /* Look for a free slot */
        ಸಂರಚನಾ xkb_pending_typing_event *pending;
        darray_size_t idx = 0;
        darray_enumerate_from(idx, pending, state->pending, idx) {
            ಯದಿ (!pending->counter) {
                darray_item(state->pending, idx) = events[e];
                ಗಚ್ಛ next;
            }
        }
        /* No free slot found: append new one */
        idx = darray_size(state->pending);
        darray_append(state->pending, events[e]);
    next:
        /* Avoid immediate consumption */
        darray_item(state->pending, idx).counter++;
    }
    state->ಅಂಕೆ_pending += ಎಣಿಕೆ;

    ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
}

ಸ್ಥಿರ ಗಣನಾ xkb_status
next_pending_events(ಸಂರಚನಾ xkb_typing_state * restrict state,
                    xkb_typing_events * restrict out)
{
    /* Reset the output */
    darray_size(*out) = 0;

    darray_size_t idx;
    ಸಂರಚನಾ xkb_pending_typing_event *pending;
    /* Stack: start from the end */
    darray_enumerate_reverse(idx, pending, state->pending) {
        ಯದಿ (!pending->counter)
            ಅನುವರ್ತ;
        pending->counter--;
        ಯದಿ (pending->counter)
            ಅನುವರ್ತ;
        darray_append(*out, pending->event);
        state->ಅಂಕೆ_pending--;
        /* Resize array if it was the last element */
        ಯದಿ (idx + 1 == darray_size(state->pending))
            darray_size(state->pending)--;
        state->mods &= ~pending->mods;
    }

    ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
}

ಗಣನಾ {
    /** Tries to avoid pressing a key already pressed */
    ಕೀಲಿ_CHOICE_MAX_TRIES = 12,
    XKB_ERROR_NO_RELEASED_ಕೀಲಿ = -1000,
};

ಸ್ಥಿರ ಗಣನಾ xkb_status
add_modifier_ಕೀಲಿ(ಸಂರಚನಾ xkb_typing_state * restrict state, size_t * restrict e)
{
    /* Choose a random key */
    long idx = -1;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ tries = 0; tries < ಕೀಲಿ_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state)
            % darray_size(state->ಕೀಲಿಗಳು->modifiers);
        /*
         * Ensure key is not already down and its modifier mask is not active,
         * i.e. to avoid simultaneous keys with the same modifier.
         */
        ಯದಿ (!bit_array_get(state->ಕೀಲಿಗಳು_state,
                           darray_item(state->ಕೀಲಿಗಳು->modifiers, idx).keycode) &&
            !(state->mods & darray_item(state->ಕೀಲಿಗಳು->modifiers, idx).mods))
        {
            ಭಙ್ಗ;
        }
        idx = -1;
    }

    ಸಂರಚನಾ xkb_modifier_ಕೀಲಿ ಕೀಲಿ = {0};
    ಯದಿ (idx < 0) {
        /* No released key found: fall back to linear search */
        ಧ್ರುವ ಸಂರಚನಾ xkb_modifier_ಕೀಲಿ *ಕೀಲಿ_2;
        darray_foreach(ಕೀಲಿ_2, state->ಕೀಲಿಗಳು->modifiers) {
            /* See test doc above */
            ಯದಿ (!bit_array_get(state->ಕೀಲಿಗಳು_state, ಕೀಲಿ_2->keycode) &&
                !(state->mods & ಕೀಲಿ_2->mods))
            {
                ಕೀಲಿ = *ಕೀಲಿ_2;
                idx = 0;
                ಭಙ್ಗ;
            }
        }
        ಯದಿ (idx < 0) {
            /* No fallback: skip */
            ಪ್ರತ್ಯಾಗಮ (ಗಣನಾ xkb_status)XKB_ERROR_NO_RELEASED_ಕೀಲಿ;
        }
    } ಅನ್ಯಥಾ {
        ಕೀಲಿ = darray_item(state->ಕೀಲಿಗಳು->modifiers, idx);
    }

    ಧ್ರುವ size_t remaining_events =
        state->ಅಂಕೆ_events - state->ಅಂಕೆ_pending - *e;
    size_t pending_turns = 0;

    ಯದಿ (remaining_events >= 2) {
        ಯದಿ (ಕೀಲಿ.kind == XKB_MOD_SET ||
            (ಕೀಲಿ.kind == XKB_MOD_LOCK && remaining_events >= 4))
        {
            /* Delay SetMods release or LockMods unlock */
            ಧ್ರುವ size_t max_turns = MIN(
                (size_t)(
                    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS -
                    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                 ),
                remaining_events + state->ಅಂಕೆ_pending
            );
            pending_turns = XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                          + (size_t)state->prng(state->prng_state)
                          % (max_turns + 1);
        }
    } ಅನ್ಯಥಾ {
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (ಸಂರಚನಾ xkb_typing_event) {
        .keycode = ಕೀಲಿ.keycode,
        .direction = XKB_ಕೀಲಿ_DOWN,
    };

    ಸಂರಚನಾ xkb_pending_typing_event pending[2] = {
        {
            .event = {
                .keycode = ಕೀಲಿ.keycode,
                .direction = XKB_ಕೀಲಿ_DOWN,
            },
            .counter = (uint8_t)pending_turns,
        },
        {
            .event = {
                .keycode = ಕೀಲಿ.keycode,
                .direction = XKB_ಕೀಲಿ_UP,
            },
            .mods = ಕೀಲಿ.mods,
            .counter = (uint8_t)pending_turns,
        },
    };

    ಯದಿ (pending_turns) {
        ಯದಿ (ಕೀಲಿ.kind == XKB_MOD_LOCK) {
            /* Immediate release */
            state->events[(*e)++] = pending[1].event;
            /* Delayed unlock (press + release) */
            pending[1].counter++;
            ಧ್ರುವ ಗಣನಾ xkb_status status =
                add_pending_events(state, pending, ARRAY_SIZE(pending));
            ಯದಿ (status != XKB_SUCCESS)
                ಪ್ರತ್ಯಾಗಮ status;
            /*
             * Register “locked” state, to avoid tapping the key before
             * delayed key released
             */
            bit_array_set(state->ಕೀಲಿಗಳು_state, ಕೀಲಿ.keycode);
        } ಅನ್ಯಥಾ {
            /* Delayed release */
            ಧ್ರುವ ಗಣನಾ xkb_status status =
                add_pending_events(state, &pending[1], 1);
            ಯದಿ (status != XKB_SUCCESS)
                ಪ್ರತ್ಯಾಗಮ status;
            /* Register pressed state */
            bit_array_set(state->ಕೀಲಿಗಳು_state, ಕೀಲಿ.keycode);
        }
        state->mods |= ಕೀಲಿ.mods;
    } ಅನ್ಯಥಾ {
        /* Immediate release */
        state->events[(*e)++] = pending[1].event;
        /* TODO: add latched modifier to mask state. */
    }

    ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
}

ಸ್ಥಿರ ಗಣನಾ xkb_status
add_other_ಕೀಲಿ(ಸಂರಚನಾ xkb_typing_state * restrict state,
              ಧ್ರುವ xkb_keycodes_t * restrict keycodes, size_t * restrict e,
              uint8_t immediate_release_prob, uint8_t max_pending_turns)
{
    /* Choose a random key */
    long idx = -1;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ tries = 0; tries < ಕೀಲಿ_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state) % darray_size(*keycodes);
        /* Ensure key is not already down */
        ಯದಿ (!bit_array_get(state->ಕೀಲಿಗಳು_state,
                           darray_item(*keycodes, idx)))
        {
            ಭಙ್ಗ;
        }
        idx = -1;
    }

    xkb_keycode_t keycode = XKB_KEYCODE_INVALID;
    ಯದಿ (idx < 0) {
        /* No released key found: fall back to linear search */
        ಧ್ರುವ xkb_keycode_t *keycode_;
        darray_foreach(keycode_, *keycodes) {
            ಯದಿ (!bit_array_get(state->ಕೀಲಿಗಳು_state, *keycode_)) {
                keycode = *keycode_;
                idx = 0;
                ಭಙ್ಗ;
            }
        }
        ಯದಿ (idx < 0) {
            /* No fallback: skip */
            ಪ್ರತ್ಯಾಗಮ (ಗಣನಾ xkb_status)XKB_ERROR_NO_RELEASED_ಕೀಲಿ;
        }
    } ಅನ್ಯಥಾ {
        keycode = darray_item(*keycodes, idx);
    }

    ಧ್ರುವ size_t remaining_events =
        state->ಅಂಕೆ_events - state->ಅಂಕೆ_pending - *e;
    size_t pending_turns;

    ಯದಿ (remaining_events >= 2) {
        ಧ್ರುವ size_t max_turns = MIN(
            (size_t)max_pending_turns,
            remaining_events + state->ಅಂಕೆ_pending
        );
        ಧ್ರುವ bool immediate_release = (
            (immediate_release_prob >= 100) ||
            !max_turns ||
            (state->prng(state->prng_state) % 100) < immediate_release_prob
        );
        ಯದಿ (immediate_release) {
            pending_turns = 0;
        } ಅನ್ಯಥಾ {
            pending_turns = 1 + (size_t)state->prng(state->prng_state)
                          % max_turns;
        }
    } ಅನ್ಯಥಾ {
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (ಸಂರಚನಾ xkb_typing_event) {
        .keycode = keycode,
        .direction = XKB_ಕೀಲಿ_DOWN,
    };

    ಧ್ರುವ ಸಂರಚನಾ xkb_pending_typing_event pending = {
        .event = {
            .keycode = keycode,
            .direction = XKB_ಕೀಲಿ_UP,
        },
        .counter = (uint8_t)pending_turns,
    };
    ಯದಿ (pending_turns) {
        ಧ್ರುವ ಗಣನಾ xkb_status status = add_pending_events(state, &pending, 1);
        ಯದಿ (status != XKB_SUCCESS)
            ಪ್ರತ್ಯಾಗಮ status;
        /* Register pressed state */
        bit_array_set(state->ಕೀಲಿಗಳು_state, keycode);
    } ಅನ್ಯಥಾ {
        state->events[(*e)++] = pending.event;
    }

    ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
}

ಸ್ಥಿರ ಗಣನಾ xkb_ಕೀಲಿ_kind
pick_ಕೀಲಿ_kind(xkb_prng_t prng, ಶೂನ್ಯ *prng_state)
{
    ಸ್ಥಿರ ಧ್ರುವ uint8_t weights[ಅಂಕೆ_XKB_ಕೀಲಿ_KIND] = {
        [XKB_ಕೀಲಿ_PRINTABLE] = 60,
        [XKB_ಕೀಲಿ_MODIFIER] = 30,
        [XKB_ಕೀಲಿ_MISC] = 10,
    };
    static_assert(XKB_ಕೀಲಿ_MISC == 2 &&
                  XKB_ಕೀಲಿ_MISC == ಅಂಕೆ_XKB_ಕೀಲಿ_KIND - 1, "");
    ಪ್ರತಿಜ್ಞಾ(weights[0] + weights[1] + weights[2] == 100);

    long r = prng(prng_state) % 100;
    ಪ್ರತಿ (ಗಣನಾ xkb_ಕೀಲಿ_kind k = 0; k < ಅಂಕೆ_XKB_ಕೀಲಿ_KIND; k++)
        ಯದಿ ((r -= weights[k]) < 0)
            ಪ್ರತ್ಯಾಗಮ k;
    ಪ್ರತ್ಯಾಗಮ XKB_ಕೀಲಿ_PRINTABLE;
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_typing_state_destroy(ಸಂರಚನಾ xkb_typing_state *state)
{
    ಮುಕ್ತ(state->ಕೀಲಿಗಳು_state);
    darray_free(state->pending);
    ಮುಕ್ತ(state->events);
}

ಗಣನಾ xkb_status
xkb_typing_ಒಳಹರಿವು_init(ಸಂರಚನಾ xkb_typing_ಒಳಹರಿವು * restrict typing,
                      ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_set * restrict ಕೀಲಿಗಳು,
                      ಧ್ರುವ ಸಂರಚನಾ xkb_typing_ಒಳಹರಿವು_config * restrict config)
{
    typing->events = ರಿಕ್ತ;
    typing->ಅಂಕೆ_events = 0;

    ಯದಿ (!config->ಒಳಹರಿವು_length)
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_INVALID;

    ಯದಿ (darray_empty(ಕೀಲಿಗಳು->printable) ||
        darray_empty(ಕೀಲಿಗಳು->modifiers) ||
        darray_empty(ಕೀಲಿಗಳು->misc))
    {
        ಪ್ರತ್ಯಾಗಮ XKB_ERROR_INVALID;
    }

    ಗಣನಾ xkb_status status = XKB_SUCCESS;

    /* NOTE: arrays are sorted because they were built using the key iterator */
    xkb_keycode_t keycode_max = darray_last(ಕೀಲಿಗಳು->printable, 0);
    keycode_max = MAX(
        darray_last(ಕೀಲಿಗಳು->modifiers, (ಸಂರಚನಾ xkb_modifier_ಕೀಲಿ){0}).keycode,
        keycode_max
    );
    keycode_max = MAX(darray_last(ಕೀಲಿಗಳು->misc, 0), keycode_max);
    ಧ್ರುವ size_t ಕೀಲಿಗಳು_state_ಅಂಕೆ = bit_array_ಪದಗಳು((size_t)keycode_max + 1);
    uintptr_t *ಕೀಲಿಗಳು_state = calloc(ಕೀಲಿಗಳು_state_ಅಂಕೆ, ಪರಿಮಾಣ(*ಕೀಲಿಗಳು_state));

    ಧ್ರುವ size_t ಅಂಕೆ_events = config->ಒಳಹರಿವು_length;
    ಸಂರಚನಾ xkb_typing_event *events = calloc(ಅಂಕೆ_events, ಪರಿಮಾಣ(*events));

    ಸಂರಚನಾ xkb_typing_state state = {
        .ಕೀಲಿಗಳು = ಕೀಲಿಗಳು,
        .ಕೀಲಿಗಳು_state = ಕೀಲಿಗಳು_state,
        .prng = config->prng,
        .prng_state = config->prng_state,
        .events = events,
        .ಅಂಕೆ_events = ಅಂಕೆ_events,
        .pending = darray_new(),
        .ಅಂಕೆ_pending = 0,
    };

    xkb_typing_events pending = darray_new();

    ಯದಿ (!ಕೀಲಿಗಳು_state || !events) {
        status = XKB_ERROR_ALLOCATION_FAILURE;
        ಗಚ್ಛ error;
    }

    /*
     * Invariant: e + state.num_pending <= state.num_events
     */

    ಪ್ರತಿ (size_t e = 0; e < ಅಂಕೆ_events;) {
        /*
         * Process pending events
         */
        status = next_pending_events(&state, &pending);
        ಯದಿ (status != XKB_SUCCESS) {
            ಗಚ್ಛ error;
        }
        ಸಂರಚನಾ xkb_typing_event *event;
        darray_foreach(event, pending) {
            ಯದಿ (e >= ಅಂಕೆ_events) {
                /* invariant violated: should be unreachable */
                PANIC_UNREACHABLE();
            }
            events[e] = *event;
            bit_array_assign(ಕೀಲಿಗಳು_state, event->keycode, (bool)event->direction);
            e++;
        }

        ಯದಿ (!state.ಅಂಕೆ_pending && e == ಅಂಕೆ_events - 1) {
            /*
             * Cannot add press/release *pair*, so just use an invalid keycode
             * for the last event.
             */
            events[e] = (ಸಂರಚನಾ xkb_typing_event) {
                .keycode = XKB_KEYCODE_INVALID,
                .direction = XKB_ಕೀಲಿ_DOWN,
            };
            ಭಙ್ಗ;
        }

        ಯದಿ (e + 1 + state.ಅಂಕೆ_pending >= ಅಂಕೆ_events) {
            /* Cannot add new pair of events: require flushing pending events */
            ಅನುವರ್ತ;
        }

        /*
         * Create new events
         */

        /* Choose kind of key */
        static_assert(XKB_ಕೀಲಿ_PRINTABLE == 0, "");
        ಧ್ರುವ ಗಣನಾ xkb_ಕೀಲಿ_kind ಕೀಲಿ_kind =
            pick_ಕೀಲಿ_kind(config->prng, config->prng_state);
        ವಿಕಲ್ಪ (ಕೀಲಿ_kind) {
        ಪಕ್ಷ XKB_ಕೀಲಿ_PRINTABLE:
            status = add_other_ಕೀಲಿ(
                &state, &state.ಕೀಲಿಗಳು->printable, &e,
                XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS
            );
            ಭಙ್ಗ;
        ಪಕ್ಷ XKB_ಕೀಲಿ_MODIFIER:
            status = add_modifier_ಕೀಲಿ(&state, &e);
            ಭಙ್ಗ;
        ಪಕ್ಷ XKB_ಕೀಲಿ_MISC:
            status = add_other_ಕೀಲಿ(
                &state, &state.ಕೀಲಿಗಳು->misc, &e,
                XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS
            );
            ಭಙ್ಗ;
        ಸ್ವಭಾವ: {
            static_assert(XKB_ಕೀಲಿ_MISC == 2 &&
                          XKB_ಕೀಲಿ_MISC == ಅಂಕೆ_XKB_ಕೀಲಿ_KIND - 1, "");
            status = XKB_ERROR_INVALID;
        }}

        ಯದಿ (status != XKB_SUCCESS &&
            status != (ಗಣನಾ xkb_status)XKB_ERROR_NO_RELEASED_ಕೀಲಿ)
        {
            ಗಚ್ಛ error;
        }
    }

    ಪ್ರತಿಜ್ಞಾ(!state.ಅಂಕೆ_pending);

    typing->events = steal(&state.events);
    typing->ಅಂಕೆ_events = ಅಂಕೆ_events;

    status = XKB_SUCCESS;

error:
    darray_free(pending);
    xkb_typing_state_destroy(&state);

    ಪ್ರತ್ಯಾಗಮ status;
}

ಶೂನ್ಯ
xkb_typing_ಒಳಹರಿವು_destroy(ಸಂರಚನಾ xkb_typing_ಒಳಹರಿವು *typing)
{
    ಯದಿ (!typing)
        ಪ್ರತ್ಯಾಗಮ;

    ಮುಕ್ತ(typing->events);
    typing->events = ರಿಕ್ತ;
}
