/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಆದರೆ WCHAR_MAX >= 0x7FFFFFFF
#ಅನ್ತರ್ಭಾವ <wctype.h>
#ವ್ಯಾಖ್ಯಾನಿಸು USE_ISWPRINT
#ಆದರೆ HAVE_NEWLOCALE && HAVE_ISWPRINT_L
#ವ್ಯಾಖ್ಯಾನಿಸು USE_ISWPRINT_L
#ಅನ್ತರ್ಭಾವ <locale.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಇಲ್ಲದಿದ್ದರೆ
#ಅನ್ತರ್ಭಾವ <ctype.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-status.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "state-priv.h"
#ಅನ್ತರ್ಭಾವ "typing.h"
#ಅನ್ತರ್ಭಾವ "util-bits.h"
#ಅನ್ತರ್ಭಾವ "util-mem.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
xkb_ಕೀಲಿ_ಗಣ_init(ರಚನೆ xkb_ಕೀಲಿ_ಗಣ *ಗಣ,
                 ರಚನೆ xkb_keymap *keymap,
                 ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_ಗಣ_config *config)
{
    darray_init(ಗಣ->printable);
    darray_init(ಗಣ->modifiers);
    darray_init(ಗಣ->misc);

    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_ಗಣ_config default_config = {0};
    ಆದರೆ (!config)
        config = &default_config;
    /* keycode = 0 denotes “default” */
    ಸ್ಥಿರಾಂಕ xkb_keycode_t min = MAX(config->min, xkb_keymap_min_keycode(keymap));
    ಸ್ಥಿರಾಂಕ xkb_keycode_t max = config->max
        ? MIN(config->max, xkb_keymap_max_keycode(keymap))
        : xkb_keymap_max_keycode(keymap);

    ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2;
    ರಚನೆ xkb_state *state =
        xkb_state_new_with_ಬಹುಲಕ(keymap, XKB_STATE_ಬಹುಲಕ_SERVER, &ಸ್ಥಿತಿ_2);
    ಆದರೆ (!state)
        ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;

    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ USE_ISWPRINT_L
    locale_t loc = newlocale(LC_ALL_MASK, "C.UTF-8", (locale_t)0);
    #ಆದರೆ_ಅಂತ್ಯ

    ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator iter;
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_ಕೀಲಿ_iterator_config iter_config = {
        .ಗಾತ್ರ_2 = ಗಾತ್ರ(iter_config),
        .flags = XKB_KEYMAP_ಕೀಲಿ_ITERATOR_NO_FLAGS,
        .ಆರಂಭ = min
    };
    ಸ್ಥಿತಿ_2 = xkb_keymap_ಕೀಲಿ_iterator_init(&iter, keymap, &iter_config);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != XKB_SUCCESS)
        ಹೋಗು error;

    ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t numlock =
        xkb_keymap_mod_get_ಮುಖವಾಡ(keymap, XKB_VMOD_NAME_ಅಂಕೆ);

    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_state_components_update components_update = {
        .ಗಾತ್ರ_2 = ಗಾತ್ರ(components_update),
        .components = XKB_STATE_MODS_LOCKED,
        .affect_locked_mods = numlock,
        .locked_mods = numlock,
    };

    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_synthetic_update update = {
        .ಗಾತ್ರ_2 = ಗಾತ್ರ(update),
        .components = &components_update,
    };

    xkb_keycode_t k;
    ಆಗ ((k = xkb_keymap_ಕೀಲಿ_iterator_ಮುಂದಿನ(&iter)) != XKB_KEYCODE_INVALID) {
        ಆದರೆ (k > max)
            ನಿಲ್ಲಿಸು;

        xkb_state_reset(state);
        ಸ್ಥಿತಿ_2 = xkb_state_update_synthetic(state, &update, ಖಾಲಿ);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != XKB_SUCCESS)
            ಹೋಗು error;

        ಸ್ಥಿರಾಂಕ xkb_keysym_t *syms = ಖಾಲಿ;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = xkb_state_ಕೀಲಿ_get_syms(state, k, &syms);

        ಎಣಿಕೆ xkb_state_component ಬದಲಾಯಿತು =
            xkb_state_update_ಕೀಲಿ(state, k, XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ);
        xkb_mod_ಮುಖವಾಡ_t mods =
            xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);
        ಬದಲಾಯಿತು |= xkb_state_update_ಕೀಲಿ(state, k, XKB_ಕೀಲಿ_ಮೇಲೆ);
        mods |= xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);

        ಆದರೆ (ಬದಲಾಯಿತು & XKB_STATE_MODS_EFFECTIVE) {
            /* Modifier key */
            ಎಣಿಕೆ xkb_modifier_ಬಗೆ ಬಗೆ;
            ಆದರೆ (ಬದಲಾಯಿತು & XKB_STATE_MODS_LOCKED) {
                ಬಗೆ = XKB_MOD_ಬೀಗ;
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಬದಲಾಯಿತು & XKB_STATE_MODS_LATCHED) {
                ಬಗೆ = XKB_MOD_LATCH;
            } ಇಲ್ಲದಿದ್ದರೆ {
                ಬಗೆ = XKB_MOD_ಗಣ;
            }
            mods &= ~numlock;
            ಆದರೆ (!mods)
                mods = numlock;
            darray_append(ಗಣ->modifiers, (ರಚನೆ xkb_modifier_ಕೀಲಿ) {
                .keycode = k,
                .mods = mods,
                .ಬಗೆ = ಬಗೆ,
            });
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (ಎಣಿಕೆ_2 > 0) {
            uint32_t utf32 = 0;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ s = 0; s < ಎಣಿಕೆ_2; s++) {
                utf32 = xkb_keysym_ಗಮ್ಯ_utf32(syms[s]);
                ಸ್ಥಿರಾಂಕ bool is_printable =
                    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ USE_ISWPRINT_L
                    ((loc && iswprint_l((wint_t)utf32, loc)) || iswprint((wint_t)utf32))
                    #ಅಥವಾಆದರೆ defined(USE_ISWPRINT)
                    iswprint((wint_t)utf32)
                    #ಇಲ್ಲದಿದ್ದರೆ
                    utf32 <= 0x7f && isprint((ಪೂರ್ಣಾಂಕ)utf32)
                    #ಆದರೆ_ಅಂತ್ಯ
                    ;
                ಆದರೆ (is_printable)
                    ನಿಲ್ಲಿಸು;
                utf32 = 0;
            }
            ಆದರೆ (utf32) {
                /* Printable keysyms */
                darray_append(ಗಣ->printable, k);
                ಮುಂದುವರಿ;
            }
        }

        /* Miscellaneous key */
        darray_append(ಗಣ->misc, k);
    }

error:
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ USE_ISWPRINT_L
    ಆದರೆ (loc)
        freelocale(loc);
    #ಆದರೆ_ಅಂತ್ಯ

    xkb_state_unref(state);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != XKB_SUCCESS)
        xkb_ಕೀಲಿ_ಗಣ_destroy(ಗಣ);

    ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
}

ಶೂನ್ಯ
xkb_ಕೀಲಿ_ಗಣ_destroy(ರಚನೆ xkb_ಕೀಲಿ_ಗಣ *ಗಣ)
{
    ಆದರೆ (!ಗಣ)
        ಹಿಂತಿರುಗಿ;

    darray_free(ಗಣ->modifiers);
    darray_free(ಗಣ->printable);
    darray_free(ಗಣ->misc);
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
ಕೂಡಿಸು_pending_events(ರಚನೆ xkb_typing_state * ಸೀಮಿತ state,
                   ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_pending_typing_event * ಸೀಮಿತ events,
                   ಗಾತ್ರ_t ಎಣಿಕೆ_2)
{
    ಗಾಗಿ (ಗಾತ್ರ_t e = 0; e < ಎಣಿಕೆ_2; e++) {
        /* Look for a free slot */
        ರಚನೆ xkb_pending_typing_event *pending;
        darray_ಗಾತ್ರ_t idx = 0;
        darray_enumerate_from(idx, pending, state->pending, idx) {
            ಆದರೆ (!pending->counter) {
                darray_ಅಂಶ(state->pending, idx) = events[e];
                ಹೋಗು ಮುಂದಿನ;
            }
        }
        /* No free slot found: append new one */
        idx = darray_ಗಾತ್ರ(state->pending);
        darray_append(state->pending, events[e]);
    ಮುಂದಿನ:
        /* Avoid immediate consumption */
        darray_ಅಂಶ(state->pending, idx).counter++;
    }
    state->ಅಂಕೆ_pending += ಎಣಿಕೆ_2;

    ಹಿಂತಿರುಗಿ XKB_SUCCESS;
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
ಮುಂದಿನ_pending_events(ರಚನೆ xkb_typing_state * ಸೀಮಿತ state,
                    xkb_typing_events * ಸೀಮಿತ ಹೊರಹೋಗು)
{
    /* Reset the output */
    darray_ಗಾತ್ರ(*ಹೊರಹೋಗು) = 0;

    darray_ಗಾತ್ರ_t idx;
    ರಚನೆ xkb_pending_typing_event *pending;
    /* Stack: start from the end */
    darray_enumerate_ಹಿಮ್ಮುಖ(idx, pending, state->pending) {
        ಆದರೆ (!pending->counter)
            ಮುಂದುವರಿ;
        pending->counter--;
        ಆದರೆ (pending->counter)
            ಮುಂದುವರಿ;
        darray_append(*ಹೊರಹೋಗು, pending->event);
        state->ಅಂಕೆ_pending--;
        /* Resize array if it was the last element */
        ಆದರೆ (idx + 1 == darray_ಗಾತ್ರ(state->pending))
            darray_ಗಾತ್ರ(state->pending)--;
        state->mods &= ~pending->mods;
    }

    ಹಿಂತಿರುಗಿ XKB_SUCCESS;
}

ಎಣಿಕೆ {
    /** Tries to avoid pressing a key already pressed */
    ಕೀಲಿ_CHOICE_MAX_TRIES = 12,
    XKB_ERROR_NO_RELEASED_ಕೀಲಿ = -1000,
};

ಸ್ಥಿರ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
ಕೂಡಿಸು_modifier_ಕೀಲಿ(ರಚನೆ xkb_typing_state * ಸೀಮಿತ state, ಗಾತ್ರ_t * ಸೀಮಿತ e)
{
    /* Choose a random key */
    ದೀರ್ಘ idx = -1;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ tries = 0; tries < ಕೀಲಿ_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state)
            % darray_ಗಾತ್ರ(state->ಕೀಲಿಗಳು->modifiers);
        /*
         * Ensure key is not already down and its modifier mask is not active,
         * i.e. to avoid simultaneous keys with the same modifier.
         */
        ಆದರೆ (!ಬಿಟ್_array_get(state->ಕೀಲಿಗಳು_state,
                           darray_ಅಂಶ(state->ಕೀಲಿಗಳು->modifiers, idx).keycode) &&
            !(state->mods & darray_ಅಂಶ(state->ಕೀಲಿಗಳು->modifiers, idx).mods))
        {
            ನಿಲ್ಲಿಸು;
        }
        idx = -1;
    }

    ರಚನೆ xkb_modifier_ಕೀಲಿ ಕೀಲಿ = {0};
    ಆದರೆ (idx < 0) {
        /* No released key found: fall back to linear search */
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_modifier_ಕೀಲಿ *ಕೀಲಿ_2;
        darray_foreach(ಕೀಲಿ_2, state->ಕೀಲಿಗಳು->modifiers) {
            /* See test doc above */
            ಆದರೆ (!ಬಿಟ್_array_get(state->ಕೀಲಿಗಳು_state, ಕೀಲಿ_2->keycode) &&
                !(state->mods & ಕೀಲಿ_2->mods))
            {
                ಕೀಲಿ = *ಕೀಲಿ_2;
                idx = 0;
                ನಿಲ್ಲಿಸು;
            }
        }
        ಆದರೆ (idx < 0) {
            /* No fallback: skip */
            ಹಿಂತಿರುಗಿ (ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ)XKB_ERROR_NO_RELEASED_ಕೀಲಿ;
        }
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಕೀಲಿ = darray_ಅಂಶ(state->ಕೀಲಿಗಳು->modifiers, idx);
    }

    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಉಳಿದ_events =
        state->ಅಂಕೆ_events - state->ಅಂಕೆ_pending - *e;
    ಗಾತ್ರ_t pending_turns = 0;

    ಆದರೆ (ಉಳಿದ_events >= 2) {
        ಆದರೆ (ಕೀಲಿ.ಬಗೆ == XKB_MOD_ಗಣ ||
            (ಕೀಲಿ.ಬಗೆ == XKB_MOD_ಬೀಗ && ಉಳಿದ_events >= 4))
        {
            /* Delay SetMods release or LockMods unlock */
            ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t max_turns = MIN(
                (ಗಾತ್ರ_t)(
                    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS -
                    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                 ),
                ಉಳಿದ_events + state->ಅಂಕೆ_pending
            );
            pending_turns = XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                          + (ಗಾತ್ರ_t)state->prng(state->prng_state)
                          % (max_turns + 1);
        }
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಹಿಂತಿರುಗಿ XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (ರಚನೆ xkb_typing_event) {
        .keycode = ಕೀಲಿ.keycode,
        .direction = XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ,
    };

    ರಚನೆ xkb_pending_typing_event pending[2] = {
        {
            .event = {
                .keycode = ಕೀಲಿ.keycode,
                .direction = XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ,
            },
            .counter = (uint8_t)pending_turns,
        },
        {
            .event = {
                .keycode = ಕೀಲಿ.keycode,
                .direction = XKB_ಕೀಲಿ_ಮೇಲೆ,
            },
            .mods = ಕೀಲಿ.mods,
            .counter = (uint8_t)pending_turns,
        },
    };

    ಆದರೆ (pending_turns) {
        ಆದರೆ (ಕೀಲಿ.ಬಗೆ == XKB_MOD_ಬೀಗ) {
            /* Immediate release */
            state->events[(*e)++] = pending[1].event;
            /* Delayed unlock (press + release) */
            pending[1].counter++;
            ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2 =
                ಕೂಡಿಸು_pending_events(state, pending, ARRAY_ಗಾತ್ರ(pending));
            ಆದರೆ (ಸ್ಥಿತಿ_2 != XKB_SUCCESS)
                ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
            /*
             * Register “locked” state, to avoid tapping the key before
             * delayed key released
             */
            ಬಿಟ್_array_ಗಣ(state->ಕೀಲಿಗಳು_state, ಕೀಲಿ.keycode);
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* Delayed release */
            ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2 =
                ಕೂಡಿಸು_pending_events(state, &pending[1], 1);
            ಆದರೆ (ಸ್ಥಿತಿ_2 != XKB_SUCCESS)
                ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
            /* Register pressed state */
            ಬಿಟ್_array_ಗಣ(state->ಕೀಲಿಗಳು_state, ಕೀಲಿ.keycode);
        }
        state->mods |= ಕೀಲಿ.mods;
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* Immediate release */
        state->events[(*e)++] = pending[1].event;
        /* TODO: add latched modifier to mask state. */
    }

    ಹಿಂತಿರುಗಿ XKB_SUCCESS;
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
ಕೂಡಿಸು_other_ಕೀಲಿ(ರಚನೆ xkb_typing_state * ಸೀಮಿತ state,
              ಸ್ಥಿರಾಂಕ xkb_keycodes_t * ಸೀಮಿತ keycodes, ಗಾತ್ರ_t * ಸೀಮಿತ e,
              uint8_t ತಕ್ಷಣ_release_prob, uint8_t max_pending_turns)
{
    /* Choose a random key */
    ದೀರ್ಘ idx = -1;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ tries = 0; tries < ಕೀಲಿ_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state) % darray_ಗಾತ್ರ(*keycodes);
        /* Ensure key is not already down */
        ಆದರೆ (!ಬಿಟ್_array_get(state->ಕೀಲಿಗಳು_state,
                           darray_ಅಂಶ(*keycodes, idx)))
        {
            ನಿಲ್ಲಿಸು;
        }
        idx = -1;
    }

    xkb_keycode_t keycode = XKB_KEYCODE_INVALID;
    ಆದರೆ (idx < 0) {
        /* No released key found: fall back to linear search */
        ಸ್ಥಿರಾಂಕ xkb_keycode_t *keycode_;
        darray_foreach(keycode_, *keycodes) {
            ಆದರೆ (!ಬಿಟ್_array_get(state->ಕೀಲಿಗಳು_state, *keycode_)) {
                keycode = *keycode_;
                idx = 0;
                ನಿಲ್ಲಿಸು;
            }
        }
        ಆದರೆ (idx < 0) {
            /* No fallback: skip */
            ಹಿಂತಿರುಗಿ (ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ)XKB_ERROR_NO_RELEASED_ಕೀಲಿ;
        }
    } ಇಲ್ಲದಿದ್ದರೆ {
        keycode = darray_ಅಂಶ(*keycodes, idx);
    }

    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಉಳಿದ_events =
        state->ಅಂಕೆ_events - state->ಅಂಕೆ_pending - *e;
    ಗಾತ್ರ_t pending_turns;

    ಆದರೆ (ಉಳಿದ_events >= 2) {
        ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t max_turns = MIN(
            (ಗಾತ್ರ_t)max_pending_turns,
            ಉಳಿದ_events + state->ಅಂಕೆ_pending
        );
        ಸ್ಥಿರಾಂಕ bool ತಕ್ಷಣ_release = (
            (ತಕ್ಷಣ_release_prob >= 100) ||
            !max_turns ||
            (state->prng(state->prng_state) % 100) < ತಕ್ಷಣ_release_prob
        );
        ಆದರೆ (ತಕ್ಷಣ_release) {
            pending_turns = 0;
        } ಇಲ್ಲದಿದ್ದರೆ {
            pending_turns = 1 + (ಗಾತ್ರ_t)state->prng(state->prng_state)
                          % max_turns;
        }
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಹಿಂತಿರುಗಿ XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (ರಚನೆ xkb_typing_event) {
        .keycode = keycode,
        .direction = XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ,
    };

    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_pending_typing_event pending = {
        .event = {
            .keycode = keycode,
            .direction = XKB_ಕೀಲಿ_ಮೇಲೆ,
        },
        .counter = (uint8_t)pending_turns,
    };
    ಆದರೆ (pending_turns) {
        ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2 = ಕೂಡಿಸು_pending_events(state, &pending, 1);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != XKB_SUCCESS)
            ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
        /* Register pressed state */
        ಬಿಟ್_array_ಗಣ(state->ಕೀಲಿಗಳು_state, keycode);
    } ಇಲ್ಲದಿದ್ದರೆ {
        state->events[(*e)++] = pending.event;
    }

    ಹಿಂತಿರುಗಿ XKB_SUCCESS;
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_ಕೀಲಿ_ಬಗೆ
pick_ಕೀಲಿ_ಬಗೆ(xkb_prng_t prng, ಶೂನ್ಯ *prng_state)
{
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint8_t weights[ಅಂಕೆ_XKB_ಕೀಲಿ_ಬಗೆ] = {
        [XKB_ಕೀಲಿ_PRINTABLE] = 60,
        [XKB_ಕೀಲಿ_MODIFIER] = 30,
        [XKB_ಕೀಲಿ_MISC] = 10,
    };
    static_assert(XKB_ಕೀಲಿ_MISC == 2 &&
                  XKB_ಕೀಲಿ_MISC == ಅಂಕೆ_XKB_ಕೀಲಿ_ಬಗೆ - 1, "");
    ಪ್ರತಿಜ್ಞಾ(weights[0] + weights[1] + weights[2] == 100);

    ದೀರ್ಘ r = prng(prng_state) % 100;
    ಗಾಗಿ (ಎಣಿಕೆ xkb_ಕೀಲಿ_ಬಗೆ k = 0; k < ಅಂಕೆ_XKB_ಕೀಲಿ_ಬಗೆ; k++)
        ಆದರೆ ((r -= weights[k]) < 0)
            ಹಿಂತಿರುಗಿ k;
    ಹಿಂತಿರುಗಿ XKB_ಕೀಲಿ_PRINTABLE;
}

ಸ್ಥಿರ ಶೂನ್ಯ
xkb_typing_state_destroy(ರಚನೆ xkb_typing_state *state)
{
    ಬಿಡುಗಡೆ(state->ಕೀಲಿಗಳು_state);
    darray_free(state->pending);
    ಬಿಡುಗಡೆ(state->events);
}

ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ
xkb_typing_ಒಳಹರಿವು_init(ರಚನೆ xkb_typing_ಒಳಹರಿವು * ಸೀಮಿತ typing,
                      ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_ಗಣ * ಸೀಮಿತ ಕೀಲಿಗಳು,
                      ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_typing_ಒಳಹರಿವು_config * ಸೀಮಿತ config)
{
    typing->events = ಖಾಲಿ;
    typing->ಅಂಕೆ_events = 0;

    ಆದರೆ (!config->ಒಳಹರಿವು_ಉದ್ದಳತೆ)
        ಹಿಂತಿರುಗಿ XKB_ERROR_INVALID;

    ಆದರೆ (darray_ಖಾಲಿಗಳು(ಕೀಲಿಗಳು->printable) ||
        darray_ಖಾಲಿಗಳು(ಕೀಲಿಗಳು->modifiers) ||
        darray_ಖಾಲಿಗಳು(ಕೀಲಿಗಳು->misc))
    {
        ಹಿಂತಿರುಗಿ XKB_ERROR_INVALID;
    }

    ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2 = XKB_SUCCESS;

    /* NOTE: arrays are sorted because they were built using the key iterator */
    xkb_keycode_t keycode_max = darray_ಕೊನೆಯ(ಕೀಲಿಗಳು->printable, 0);
    keycode_max = MAX(
        darray_ಕೊನೆಯ(ಕೀಲಿಗಳು->modifiers, (ರಚನೆ xkb_modifier_ಕೀಲಿ){0}).keycode,
        keycode_max
    );
    keycode_max = MAX(darray_ಕೊನೆಯ(ಕೀಲಿಗಳು->misc, 0), keycode_max);
    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಕೀಲಿಗಳು_state_ಅಂಕೆ = ಬಿಟ್_array_ಪದಗಳು((ಗಾತ್ರ_t)keycode_max + 1);
    uintptr_t *ಕೀಲಿಗಳು_state = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ಕೀಲಿಗಳು_state_ಅಂಕೆ, ಗಾತ್ರ(*ಕೀಲಿಗಳು_state));

    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಅಂಕೆ_events = config->ಒಳಹರಿವು_ಉದ್ದಳತೆ;
    ರಚನೆ xkb_typing_event *events = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ಅಂಕೆ_events, ಗಾತ್ರ(*events));

    ರಚನೆ xkb_typing_state state = {
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

    ಆದರೆ (!ಕೀಲಿಗಳು_state || !events) {
        ಸ್ಥಿತಿ_2 = XKB_ERROR_ಹಂಚಿಕೆ_FAILURE;
        ಹೋಗು error;
    }

    /*
     * Invariant: e + state.num_pending <= state.num_events
     */

    ಗಾಗಿ (ಗಾತ್ರ_t e = 0; e < ಅಂಕೆ_events;) {
        /*
         * Process pending events
         */
        ಸ್ಥಿತಿ_2 = ಮುಂದಿನ_pending_events(&state, &pending);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != XKB_SUCCESS) {
            ಹೋಗು error;
        }
        ರಚನೆ xkb_typing_event *event;
        darray_foreach(event, pending) {
            ಆದರೆ (e >= ಅಂಕೆ_events) {
                /* invariant violated: should be unreachable */
                PANIC_UNREACHABLE();
            }
            events[e] = *event;
            ಬಿಟ್_array_ನಿಯೋಜಿಸು(ಕೀಲಿಗಳು_state, event->keycode, (bool)event->direction);
            e++;
        }

        ಆದರೆ (!state.ಅಂಕೆ_pending && e == ಅಂಕೆ_events - 1) {
            /*
             * Cannot add press/release *pair*, so just use an invalid keycode
             * for the last event.
             */
            events[e] = (ರಚನೆ xkb_typing_event) {
                .keycode = XKB_KEYCODE_INVALID,
                .direction = XKB_ಕೀಲಿ_ಕೆಳಕ್ಕೆ,
            };
            ನಿಲ್ಲಿಸು;
        }

        ಆದರೆ (e + 1 + state.ಅಂಕೆ_pending >= ಅಂಕೆ_events) {
            /* Cannot add new pair of events: require flushing pending events */
            ಮುಂದುವರಿ;
        }

        /*
         * Create new events
         */

        /* Choose kind of key */
        static_assert(XKB_ಕೀಲಿ_PRINTABLE == 0, "");
        ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_ಕೀಲಿ_ಬಗೆ ಕೀಲಿ_ಬಗೆ =
            pick_ಕೀಲಿ_ಬಗೆ(config->prng, config->prng_state);
        ಆಯ್ಕೆ (ಕೀಲಿ_ಬಗೆ) {
        ಸ್ಥಿತಿ XKB_ಕೀಲಿ_PRINTABLE:
            ಸ್ಥಿತಿ_2 = ಕೂಡಿಸು_other_ಕೀಲಿ(
                &state, &state.ಕೀಲಿಗಳು->printable, &e,
                XKB_TYPING_EVENT_PRINTABLE_ತಕ್ಷಣ_RELEASE_PERCENT,
                XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS
            );
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ XKB_ಕೀಲಿ_MODIFIER:
            ಸ್ಥಿತಿ_2 = ಕೂಡಿಸು_modifier_ಕೀಲಿ(&state, &e);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ XKB_ಕೀಲಿ_MISC:
            ಸ್ಥಿತಿ_2 = ಕೂಡಿಸು_other_ಕೀಲಿ(
                &state, &state.ಕೀಲಿಗಳು->misc, &e,
                XKB_TYPING_EVENT_MISC_ತಕ್ಷಣ_RELEASE_PERCENT,
                XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS
            );
            ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ: {
            static_assert(XKB_ಕೀಲಿ_MISC == 2 &&
                          XKB_ಕೀಲಿ_MISC == ಅಂಕೆ_XKB_ಕೀಲಿ_ಬಗೆ - 1, "");
            ಸ್ಥಿತಿ_2 = XKB_ERROR_INVALID;
        }}

        ಆದರೆ (ಸ್ಥಿತಿ_2 != XKB_SUCCESS &&
            ಸ್ಥಿತಿ_2 != (ಎಣಿಕೆ xkb_ಸ್ಥಿತಿ)XKB_ERROR_NO_RELEASED_ಕೀಲಿ)
        {
            ಹೋಗು error;
        }
    }

    ಪ್ರತಿಜ್ಞಾ(!state.ಅಂಕೆ_pending);

    typing->events = steal(&state.events);
    typing->ಅಂಕೆ_events = ಅಂಕೆ_events;

    ಸ್ಥಿತಿ_2 = XKB_SUCCESS;

error:
    darray_free(pending);
    xkb_typing_state_destroy(&state);

    ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
}

ಶೂನ್ಯ
xkb_typing_ಒಳಹರಿವು_destroy(ರಚನೆ xkb_typing_ಒಳಹರಿವು *typing)
{
    ಆದರೆ (!typing)
        ಹಿಂತಿರುಗಿ;

    ಬಿಡುಗಡೆ(typing->events);
    typing->events = ಖಾಲಿ;
}
