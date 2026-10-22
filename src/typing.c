/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਜੇ WCHAR_MAX >= 0x7FFFFFFF
#ਅਨ੍ਤਰ੍ਭਾਵ <wctype.h>
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_ISWPRINT
#ਜੇ HAVE_NEWLOCALE && HAVE_ISWPRINT_L
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_ISWPRINT_L
#ਅਨ੍ਤਰ੍ਭਾਵ <locale.h>
#ਜੇ_ਅੰਤ
#ਨਹੀਂਤਾਂ
#ਅਨ੍ਤਰ੍ਭਾਵ <ctype.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon-status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "state-priv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "typing.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-bits.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-mem.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

ਗਿਣਤੀ xkb_ਸਥਿਤੀ
xkb_ਕੁੰਜੀ_ਸੈੱਟ_init(ਬਣਤਰ xkb_ਕੁੰਜੀ_ਸੈੱਟ *ਸੈੱਟ,
                 ਬਣਤਰ xkb_keymap *keymap,
                 ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_ਸੈੱਟ_config *config)
{
    darray_init(ਸੈੱਟ->printable);
    darray_init(ਸੈੱਟ->modifiers);
    darray_init(ਸੈੱਟ->misc);

    ਸਥਿਰ ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_ਸੈੱਟ_config default_config = {0};
    ਜੇ (!config)
        config = &default_config;
    /* keycode = 0 denotes “default” */
    ਅਚਲ xkb_keycode_t min = MAX(config->min, xkb_keymap_min_keycode(keymap));
    ਅਚਲ xkb_keycode_t max = config->max
        ? MIN(config->max, xkb_keymap_max_keycode(keymap))
        : xkb_keymap_max_keycode(keymap);

    ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ;
    ਬਣਤਰ xkb_state *state =
        xkb_state_new_with_ਬਹੁਲਕ(keymap, XKB_STATE_ਬਹੁਲਕ_SERVER, &ਸਥਿਤੀ);
    ਜੇ (!state)
        ਵਾਪਸ ਸਥਿਤੀ;

    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ USE_ISWPRINT_L
    locale_t loc = newlocale(LC_ALL_MASK, "C.UTF-8", (locale_t)0);
    #ਜੇ_ਅੰਤ

    ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator iter;
    ਅਚਲ ਬਣਤਰ xkb_keymap_ਕੁੰਜੀ_iterator_config iter_config = {
        .ਮਾਪ = ਆਕਾਰ(iter_config),
        .flags = XKB_KEYMAP_ਕੁੰਜੀ_ITERATOR_NO_FLAGS,
        .ਸ਼ੁਰੂ = min
    };
    ਸਥਿਤੀ = xkb_keymap_ਕੁੰਜੀ_iterator_init(&iter, keymap, &iter_config);
    ਜੇ (ਸਥਿਤੀ != XKB_SUCCESS)
        ਜਾਓ error;

    ਅਚਲ xkb_mod_ਨਕਾਬ_t numlock =
        xkb_keymap_mod_get_ਨਕਾਬ(keymap, XKB_VMOD_NAME_ਅੰਕ);

    ਅਚਲ ਬਣਤਰ xkb_state_components_update components_update = {
        .ਮਾਪ = ਆਕਾਰ(components_update),
        .components = XKB_STATE_MODS_LOCKED,
        .affect_locked_mods = numlock,
        .locked_mods = numlock,
    };

    ਅਚਲ ਬਣਤਰ xkb_synthetic_update update = {
        .ਮਾਪ = ਆਕਾਰ(update),
        .components = &components_update,
    };

    xkb_keycode_t k;
    ਜਦੋਂ ((k = xkb_keymap_ਕੁੰਜੀ_iterator_ਅਗਲਾ(&iter)) != XKB_KEYCODE_INVALID) {
        ਜੇ (k > max)
            ਰੋਕੋ;

        xkb_state_reset(state);
        ਸਥਿਤੀ = xkb_state_update_synthetic(state, &update, ਨਿਰਾ);
        ਜੇ (ਸਥਿਤੀ != XKB_SUCCESS)
            ਜਾਓ error;

        ਅਚਲ xkb_keysym_t *syms = ਨਿਰਾ;
        ਅਚਲ ਪੂਰਨ ਗਿਣਤੀ_2 = xkb_state_ਕੁੰਜੀ_get_syms(state, k, &syms);

        ਗਿਣਤੀ xkb_state_component ਬਦਲਿਆ =
            xkb_state_update_ਕੁੰਜੀ(state, k, XKB_ਕੁੰਜੀ_ਥੱਲੇ);
        xkb_mod_ਨਕਾਬ_t mods =
            xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);
        ਬਦਲਿਆ |= xkb_state_update_ਕੁੰਜੀ(state, k, XKB_ਕੁੰਜੀ_ਉੱਪਰ);
        mods |= xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);

        ਜੇ (ਬਦਲਿਆ & XKB_STATE_MODS_EFFECTIVE) {
            /* Modifier key */
            ਗਿਣਤੀ xkb_modifier_ਕਿਸਮ ਕਿਸਮ;
            ਜੇ (ਬਦਲਿਆ & XKB_STATE_MODS_LOCKED) {
                ਕਿਸਮ = XKB_MOD_ਤਾਲਾ;
            } ਨਹੀਂਤਾਂ ਜੇ (ਬਦਲਿਆ & XKB_STATE_MODS_LATCHED) {
                ਕਿਸਮ = XKB_MOD_LATCH;
            } ਨਹੀਂਤਾਂ {
                ਕਿਸਮ = XKB_MOD_ਸੈੱਟ;
            }
            mods &= ~numlock;
            ਜੇ (!mods)
                mods = numlock;
            darray_append(ਸੈੱਟ->modifiers, (ਬਣਤਰ xkb_modifier_ਕੁੰਜੀ) {
                .keycode = k,
                .mods = mods,
                .ਕਿਸਮ = ਕਿਸਮ,
            });
            ਜਾਰੀ;
        }

        ਜੇ (ਗਿਣਤੀ_2 > 0) {
            uint32_t utf32 = 0;
            ਲਈ (ਪੂਰਨ s = 0; s < ਗਿਣਤੀ_2; s++) {
                utf32 = xkb_keysym_ਮੰਜ਼ਿਲ_utf32(syms[s]);
                ਅਚਲ bool is_printable =
                    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ USE_ISWPRINT_L
                    ((loc && iswprint_l((wint_t)utf32, loc)) || iswprint((wint_t)utf32))
                    #ਜਾਂਜੇ defined(USE_ISWPRINT)
                    iswprint((wint_t)utf32)
                    #ਨਹੀਂਤਾਂ
                    utf32 <= 0x7f && isprint((ਪੂਰਨ)utf32)
                    #ਜੇ_ਅੰਤ
                    ;
                ਜੇ (is_printable)
                    ਰੋਕੋ;
                utf32 = 0;
            }
            ਜੇ (utf32) {
                /* Printable keysyms */
                darray_append(ਸੈੱਟ->printable, k);
                ਜਾਰੀ;
            }
        }

        /* Miscellaneous key */
        darray_append(ਸੈੱਟ->misc, k);
    }

error:
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ USE_ISWPRINT_L
    ਜੇ (loc)
        freelocale(loc);
    #ਜੇ_ਅੰਤ

    xkb_state_unref(state);
    ਜੇ (ਸਥਿਤੀ != XKB_SUCCESS)
        xkb_ਕੁੰਜੀ_ਸੈੱਟ_destroy(ਸੈੱਟ);

    ਵਾਪਸ ਸਥਿਤੀ;
}

ਖਾਲੀ
xkb_ਕੁੰਜੀ_ਸੈੱਟ_destroy(ਬਣਤਰ xkb_ਕੁੰਜੀ_ਸੈੱਟ *ਸੈੱਟ)
{
    ਜੇ (!ਸੈੱਟ)
        ਵਾਪਸ;

    darray_free(ਸੈੱਟ->modifiers);
    darray_free(ਸੈੱਟ->printable);
    darray_free(ਸੈੱਟ->misc);
}

ਸਥਿਰ ਗਿਣਤੀ xkb_ਸਥਿਤੀ
ਜੋੜੋ_pending_events(ਬਣਤਰ xkb_typing_state * ਸੀਮਿਤ state,
                   ਅਚਲ ਬਣਤਰ xkb_pending_typing_event * ਸੀਮਿਤ events,
                   ਮਾਪ_t ਗਿਣਤੀ_2)
{
    ਲਈ (ਮਾਪ_t e = 0; e < ਗਿਣਤੀ_2; e++) {
        /* Look for a free slot */
        ਬਣਤਰ xkb_pending_typing_event *pending;
        darray_ਮਾਪ_t idx = 0;
        darray_enumerate_from(idx, pending, state->pending, idx) {
            ਜੇ (!pending->counter) {
                darray_ਵਸਤੂ(state->pending, idx) = events[e];
                ਜਾਓ ਅਗਲਾ;
            }
        }
        /* No free slot found: append new one */
        idx = darray_ਮਾਪ(state->pending);
        darray_append(state->pending, events[e]);
    ਅਗਲਾ:
        /* Avoid immediate consumption */
        darray_ਵਸਤੂ(state->pending, idx).counter++;
    }
    state->ਅੰਕ_pending += ਗਿਣਤੀ_2;

    ਵਾਪਸ XKB_SUCCESS;
}

ਸਥਿਰ ਗਿਣਤੀ xkb_ਸਥਿਤੀ
ਅਗਲਾ_pending_events(ਬਣਤਰ xkb_typing_state * ਸੀਮਿਤ state,
                    xkb_typing_events * ਸੀਮਿਤ ਬਾਹਰਲਾ)
{
    /* Reset the output */
    darray_ਮਾਪ(*ਬਾਹਰਲਾ) = 0;

    darray_ਮਾਪ_t idx;
    ਬਣਤਰ xkb_pending_typing_event *pending;
    /* Stack: start from the end */
    darray_enumerate_ਉਲਟ(idx, pending, state->pending) {
        ਜੇ (!pending->counter)
            ਜਾਰੀ;
        pending->counter--;
        ਜੇ (pending->counter)
            ਜਾਰੀ;
        darray_append(*ਬਾਹਰਲਾ, pending->event);
        state->ਅੰਕ_pending--;
        /* Resize array if it was the last element */
        ਜੇ (idx + 1 == darray_ਮਾਪ(state->pending))
            darray_ਮਾਪ(state->pending)--;
        state->mods &= ~pending->mods;
    }

    ਵਾਪਸ XKB_SUCCESS;
}

ਗਿਣਤੀ {
    /** Tries to avoid pressing a key already pressed */
    ਕੁੰਜੀ_CHOICE_MAX_TRIES = 12,
    XKB_ERROR_NO_RELEASED_ਕੁੰਜੀ = -1000,
};

ਸਥਿਰ ਗਿਣਤੀ xkb_ਸਥਿਤੀ
ਜੋੜੋ_modifier_ਕੁੰਜੀ(ਬਣਤਰ xkb_typing_state * ਸੀਮਿਤ state, ਮਾਪ_t * ਸੀਮਿਤ e)
{
    /* Choose a random key */
    ਲੰਮਾ idx = -1;
    ਲਈ (ਪੂਰਨ tries = 0; tries < ਕੁੰਜੀ_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state)
            % darray_ਮਾਪ(state->ਕੁੰਜੀਆਂ->modifiers);
        /*
         * Ensure key is not already down and its modifier mask is not active,
         * i.e. to avoid simultaneous keys with the same modifier.
         */
        ਜੇ (!ਦੋਆਧਾਰੀ_ਅੰਕ_array_get(state->ਕੁੰਜੀਆਂ_state,
                           darray_ਵਸਤੂ(state->ਕੁੰਜੀਆਂ->modifiers, idx).keycode) &&
            !(state->mods & darray_ਵਸਤੂ(state->ਕੁੰਜੀਆਂ->modifiers, idx).mods))
        {
            ਰੋਕੋ;
        }
        idx = -1;
    }

    ਬਣਤਰ xkb_modifier_ਕੁੰਜੀ ਕੁੰਜੀ = {0};
    ਜੇ (idx < 0) {
        /* No released key found: fall back to linear search */
        ਅਚਲ ਬਣਤਰ xkb_modifier_ਕੁੰਜੀ *ਕੁੰਜੀ_2;
        darray_foreach(ਕੁੰਜੀ_2, state->ਕੁੰਜੀਆਂ->modifiers) {
            /* See test doc above */
            ਜੇ (!ਦੋਆਧਾਰੀ_ਅੰਕ_array_get(state->ਕੁੰਜੀਆਂ_state, ਕੁੰਜੀ_2->keycode) &&
                !(state->mods & ਕੁੰਜੀ_2->mods))
            {
                ਕੁੰਜੀ = *ਕੁੰਜੀ_2;
                idx = 0;
                ਰੋਕੋ;
            }
        }
        ਜੇ (idx < 0) {
            /* No fallback: skip */
            ਵਾਪਸ (ਗਿਣਤੀ xkb_ਸਥਿਤੀ)XKB_ERROR_NO_RELEASED_ਕੁੰਜੀ;
        }
    } ਨਹੀਂਤਾਂ {
        ਕੁੰਜੀ = darray_ਵਸਤੂ(state->ਕੁੰਜੀਆਂ->modifiers, idx);
    }

    ਅਚਲ ਮਾਪ_t ਬਚਿਆ_events =
        state->ਅੰਕ_events - state->ਅੰਕ_pending - *e;
    ਮਾਪ_t pending_turns = 0;

    ਜੇ (ਬਚਿਆ_events >= 2) {
        ਜੇ (ਕੁੰਜੀ.ਕਿਸਮ == XKB_MOD_ਸੈੱਟ ||
            (ਕੁੰਜੀ.ਕਿਸਮ == XKB_MOD_ਤਾਲਾ && ਬਚਿਆ_events >= 4))
        {
            /* Delay SetMods release or LockMods unlock */
            ਅਚਲ ਮਾਪ_t max_turns = MIN(
                (ਮਾਪ_t)(
                    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS -
                    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                 ),
                ਬਚਿਆ_events + state->ਅੰਕ_pending
            );
            pending_turns = XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                          + (ਮਾਪ_t)state->prng(state->prng_state)
                          % (max_turns + 1);
        }
    } ਨਹੀਂਤਾਂ {
        ਵਾਪਸ XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (ਬਣਤਰ xkb_typing_event) {
        .keycode = ਕੁੰਜੀ.keycode,
        .direction = XKB_ਕੁੰਜੀ_ਥੱਲੇ,
    };

    ਬਣਤਰ xkb_pending_typing_event pending[2] = {
        {
            .event = {
                .keycode = ਕੁੰਜੀ.keycode,
                .direction = XKB_ਕੁੰਜੀ_ਥੱਲੇ,
            },
            .counter = (uint8_t)pending_turns,
        },
        {
            .event = {
                .keycode = ਕੁੰਜੀ.keycode,
                .direction = XKB_ਕੁੰਜੀ_ਉੱਪਰ,
            },
            .mods = ਕੁੰਜੀ.mods,
            .counter = (uint8_t)pending_turns,
        },
    };

    ਜੇ (pending_turns) {
        ਜੇ (ਕੁੰਜੀ.ਕਿਸਮ == XKB_MOD_ਤਾਲਾ) {
            /* Immediate release */
            state->events[(*e)++] = pending[1].event;
            /* Delayed unlock (press + release) */
            pending[1].counter++;
            ਅਚਲ ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ =
                ਜੋੜੋ_pending_events(state, pending, ARRAY_ਮਾਪ(pending));
            ਜੇ (ਸਥਿਤੀ != XKB_SUCCESS)
                ਵਾਪਸ ਸਥਿਤੀ;
            /*
             * Register “locked” state, to avoid tapping the key before
             * delayed key released
             */
            ਦੋਆਧਾਰੀ_ਅੰਕ_array_ਸੈੱਟ(state->ਕੁੰਜੀਆਂ_state, ਕੁੰਜੀ.keycode);
        } ਨਹੀਂਤਾਂ {
            /* Delayed release */
            ਅਚਲ ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ =
                ਜੋੜੋ_pending_events(state, &pending[1], 1);
            ਜੇ (ਸਥਿਤੀ != XKB_SUCCESS)
                ਵਾਪਸ ਸਥਿਤੀ;
            /* Register pressed state */
            ਦੋਆਧਾਰੀ_ਅੰਕ_array_ਸੈੱਟ(state->ਕੁੰਜੀਆਂ_state, ਕੁੰਜੀ.keycode);
        }
        state->mods |= ਕੁੰਜੀ.mods;
    } ਨਹੀਂਤਾਂ {
        /* Immediate release */
        state->events[(*e)++] = pending[1].event;
        /* TODO: add latched modifier to mask state. */
    }

    ਵਾਪਸ XKB_SUCCESS;
}

ਸਥਿਰ ਗਿਣਤੀ xkb_ਸਥਿਤੀ
ਜੋੜੋ_other_ਕੁੰਜੀ(ਬਣਤਰ xkb_typing_state * ਸੀਮਿਤ state,
              ਅਚਲ xkb_keycodes_t * ਸੀਮਿਤ keycodes, ਮਾਪ_t * ਸੀਮਿਤ e,
              uint8_t ਤੁਰੰਤ_release_prob, uint8_t max_pending_turns)
{
    /* Choose a random key */
    ਲੰਮਾ idx = -1;
    ਲਈ (ਪੂਰਨ tries = 0; tries < ਕੁੰਜੀ_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state) % darray_ਮਾਪ(*keycodes);
        /* Ensure key is not already down */
        ਜੇ (!ਦੋਆਧਾਰੀ_ਅੰਕ_array_get(state->ਕੁੰਜੀਆਂ_state,
                           darray_ਵਸਤੂ(*keycodes, idx)))
        {
            ਰੋਕੋ;
        }
        idx = -1;
    }

    xkb_keycode_t keycode = XKB_KEYCODE_INVALID;
    ਜੇ (idx < 0) {
        /* No released key found: fall back to linear search */
        ਅਚਲ xkb_keycode_t *keycode_;
        darray_foreach(keycode_, *keycodes) {
            ਜੇ (!ਦੋਆਧਾਰੀ_ਅੰਕ_array_get(state->ਕੁੰਜੀਆਂ_state, *keycode_)) {
                keycode = *keycode_;
                idx = 0;
                ਰੋਕੋ;
            }
        }
        ਜੇ (idx < 0) {
            /* No fallback: skip */
            ਵਾਪਸ (ਗਿਣਤੀ xkb_ਸਥਿਤੀ)XKB_ERROR_NO_RELEASED_ਕੁੰਜੀ;
        }
    } ਨਹੀਂਤਾਂ {
        keycode = darray_ਵਸਤੂ(*keycodes, idx);
    }

    ਅਚਲ ਮਾਪ_t ਬਚਿਆ_events =
        state->ਅੰਕ_events - state->ਅੰਕ_pending - *e;
    ਮਾਪ_t pending_turns;

    ਜੇ (ਬਚਿਆ_events >= 2) {
        ਅਚਲ ਮਾਪ_t max_turns = MIN(
            (ਮਾਪ_t)max_pending_turns,
            ਬਚਿਆ_events + state->ਅੰਕ_pending
        );
        ਅਚਲ bool ਤੁਰੰਤ_release = (
            (ਤੁਰੰਤ_release_prob >= 100) ||
            !max_turns ||
            (state->prng(state->prng_state) % 100) < ਤੁਰੰਤ_release_prob
        );
        ਜੇ (ਤੁਰੰਤ_release) {
            pending_turns = 0;
        } ਨਹੀਂਤਾਂ {
            pending_turns = 1 + (ਮਾਪ_t)state->prng(state->prng_state)
                          % max_turns;
        }
    } ਨਹੀਂਤਾਂ {
        ਵਾਪਸ XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (ਬਣਤਰ xkb_typing_event) {
        .keycode = keycode,
        .direction = XKB_ਕੁੰਜੀ_ਥੱਲੇ,
    };

    ਅਚਲ ਬਣਤਰ xkb_pending_typing_event pending = {
        .event = {
            .keycode = keycode,
            .direction = XKB_ਕੁੰਜੀ_ਉੱਪਰ,
        },
        .counter = (uint8_t)pending_turns,
    };
    ਜੇ (pending_turns) {
        ਅਚਲ ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ = ਜੋੜੋ_pending_events(state, &pending, 1);
        ਜੇ (ਸਥਿਤੀ != XKB_SUCCESS)
            ਵਾਪਸ ਸਥਿਤੀ;
        /* Register pressed state */
        ਦੋਆਧਾਰੀ_ਅੰਕ_array_ਸੈੱਟ(state->ਕੁੰਜੀਆਂ_state, keycode);
    } ਨਹੀਂਤਾਂ {
        state->events[(*e)++] = pending.event;
    }

    ਵਾਪਸ XKB_SUCCESS;
}

ਸਥਿਰ ਗਿਣਤੀ xkb_ਕੁੰਜੀ_ਕਿਸਮ
pick_ਕੁੰਜੀ_ਕਿਸਮ(xkb_prng_t prng, ਖਾਲੀ *prng_state)
{
    ਸਥਿਰ ਅਚਲ uint8_t weights[ਅੰਕ_XKB_ਕੁੰਜੀ_ਕਿਸਮ] = {
        [XKB_ਕੁੰਜੀ_PRINTABLE] = 60,
        [XKB_ਕੁੰਜੀ_MODIFIER] = 30,
        [XKB_ਕੁੰਜੀ_MISC] = 10,
    };
    static_assert(XKB_ਕੁੰਜੀ_MISC == 2 &&
                  XKB_ਕੁੰਜੀ_MISC == ਅੰਕ_XKB_ਕੁੰਜੀ_ਕਿਸਮ - 1, "");
    ਪ੍ਰਤਿਜ੍ਞਾ(weights[0] + weights[1] + weights[2] == 100);

    ਲੰਮਾ r = prng(prng_state) % 100;
    ਲਈ (ਗਿਣਤੀ xkb_ਕੁੰਜੀ_ਕਿਸਮ k = 0; k < ਅੰਕ_XKB_ਕੁੰਜੀ_ਕਿਸਮ; k++)
        ਜੇ ((r -= weights[k]) < 0)
            ਵਾਪਸ k;
    ਵਾਪਸ XKB_ਕੁੰਜੀ_PRINTABLE;
}

ਸਥਿਰ ਖਾਲੀ
xkb_typing_state_destroy(ਬਣਤਰ xkb_typing_state *state)
{
    ਮੁਕਤ(state->ਕੁੰਜੀਆਂ_state);
    darray_free(state->pending);
    ਮੁਕਤ(state->events);
}

ਗਿਣਤੀ xkb_ਸਥਿਤੀ
xkb_typing_ਨਿਵੇਸ਼_init(ਬਣਤਰ xkb_typing_ਨਿਵੇਸ਼ * ਸੀਮਿਤ typing,
                      ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_ਸੈੱਟ * ਸੀਮਿਤ ਕੁੰਜੀਆਂ,
                      ਅਚਲ ਬਣਤਰ xkb_typing_ਨਿਵੇਸ਼_config * ਸੀਮਿਤ config)
{
    typing->events = ਨਿਰਾ;
    typing->ਅੰਕ_events = 0;

    ਜੇ (!config->ਨਿਵੇਸ਼_ਲੰਮਾਈ)
        ਵਾਪਸ XKB_ERROR_INVALID;

    ਜੇ (darray_ਖਾਲੀ_ਥਾਵਾਂ(ਕੁੰਜੀਆਂ->printable) ||
        darray_ਖਾਲੀ_ਥਾਵਾਂ(ਕੁੰਜੀਆਂ->modifiers) ||
        darray_ਖਾਲੀ_ਥਾਵਾਂ(ਕੁੰਜੀਆਂ->misc))
    {
        ਵਾਪਸ XKB_ERROR_INVALID;
    }

    ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ = XKB_SUCCESS;

    /* NOTE: arrays are sorted because they were built using the key iterator */
    xkb_keycode_t keycode_max = darray_ਆਖਰੀ(ਕੁੰਜੀਆਂ->printable, 0);
    keycode_max = MAX(
        darray_ਆਖਰੀ(ਕੁੰਜੀਆਂ->modifiers, (ਬਣਤਰ xkb_modifier_ਕੁੰਜੀ){0}).keycode,
        keycode_max
    );
    keycode_max = MAX(darray_ਆਖਰੀ(ਕੁੰਜੀਆਂ->misc, 0), keycode_max);
    ਅਚਲ ਮਾਪ_t ਕੁੰਜੀਆਂ_state_ਅੰਕ = ਦੋਆਧਾਰੀ_ਅੰਕ_array_ਸ਼ਬਦ((ਮਾਪ_t)keycode_max + 1);
    uintptr_t *ਕੁੰਜੀਆਂ_state = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਕੁੰਜੀਆਂ_state_ਅੰਕ, ਆਕਾਰ(*ਕੁੰਜੀਆਂ_state));

    ਅਚਲ ਮਾਪ_t ਅੰਕ_events = config->ਨਿਵੇਸ਼_ਲੰਮਾਈ;
    ਬਣਤਰ xkb_typing_event *events = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਅੰਕ_events, ਆਕਾਰ(*events));

    ਬਣਤਰ xkb_typing_state state = {
        .ਕੁੰਜੀਆਂ = ਕੁੰਜੀਆਂ,
        .ਕੁੰਜੀਆਂ_state = ਕੁੰਜੀਆਂ_state,
        .prng = config->prng,
        .prng_state = config->prng_state,
        .events = events,
        .ਅੰਕ_events = ਅੰਕ_events,
        .pending = darray_new(),
        .ਅੰਕ_pending = 0,
    };

    xkb_typing_events pending = darray_new();

    ਜੇ (!ਕੁੰਜੀਆਂ_state || !events) {
        ਸਥਿਤੀ = XKB_ERROR_ਵੰਡ_FAILURE;
        ਜਾਓ error;
    }

    /*
     * Invariant: e + state.num_pending <= state.num_events
     */

    ਲਈ (ਮਾਪ_t e = 0; e < ਅੰਕ_events;) {
        /*
         * Process pending events
         */
        ਸਥਿਤੀ = ਅਗਲਾ_pending_events(&state, &pending);
        ਜੇ (ਸਥਿਤੀ != XKB_SUCCESS) {
            ਜਾਓ error;
        }
        ਬਣਤਰ xkb_typing_event *event;
        darray_foreach(event, pending) {
            ਜੇ (e >= ਅੰਕ_events) {
                /* invariant violated: should be unreachable */
                PANIC_UNREACHABLE();
            }
            events[e] = *event;
            ਦੋਆਧਾਰੀ_ਅੰਕ_array_ਸੌਂਪੋ(ਕੁੰਜੀਆਂ_state, event->keycode, (bool)event->direction);
            e++;
        }

        ਜੇ (!state.ਅੰਕ_pending && e == ਅੰਕ_events - 1) {
            /*
             * Cannot add press/release *pair*, so just use an invalid keycode
             * for the last event.
             */
            events[e] = (ਬਣਤਰ xkb_typing_event) {
                .keycode = XKB_KEYCODE_INVALID,
                .direction = XKB_ਕੁੰਜੀ_ਥੱਲੇ,
            };
            ਰੋਕੋ;
        }

        ਜੇ (e + 1 + state.ਅੰਕ_pending >= ਅੰਕ_events) {
            /* Cannot add new pair of events: require flushing pending events */
            ਜਾਰੀ;
        }

        /*
         * Create new events
         */

        /* Choose kind of key */
        static_assert(XKB_ਕੁੰਜੀ_PRINTABLE == 0, "");
        ਅਚਲ ਗਿਣਤੀ xkb_ਕੁੰਜੀ_ਕਿਸਮ ਕੁੰਜੀ_ਕਿਸਮ =
            pick_ਕੁੰਜੀ_ਕਿਸਮ(config->prng, config->prng_state);
        ਚੋਣ (ਕੁੰਜੀ_ਕਿਸਮ) {
        ਹਾਲਤ XKB_ਕੁੰਜੀ_PRINTABLE:
            ਸਥਿਤੀ = ਜੋੜੋ_other_ਕੁੰਜੀ(
                &state, &state.ਕੁੰਜੀਆਂ->printable, &e,
                XKB_TYPING_EVENT_PRINTABLE_ਤੁਰੰਤ_RELEASE_PERCENT,
                XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS
            );
            ਰੋਕੋ;
        ਹਾਲਤ XKB_ਕੁੰਜੀ_MODIFIER:
            ਸਥਿਤੀ = ਜੋੜੋ_modifier_ਕੁੰਜੀ(&state, &e);
            ਰੋਕੋ;
        ਹਾਲਤ XKB_ਕੁੰਜੀ_MISC:
            ਸਥਿਤੀ = ਜੋੜੋ_other_ਕੁੰਜੀ(
                &state, &state.ਕੁੰਜੀਆਂ->misc, &e,
                XKB_TYPING_EVENT_MISC_ਤੁਰੰਤ_RELEASE_PERCENT,
                XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS
            );
            ਰੋਕੋ;
        ਮੂਲ: {
            static_assert(XKB_ਕੁੰਜੀ_MISC == 2 &&
                          XKB_ਕੁੰਜੀ_MISC == ਅੰਕ_XKB_ਕੁੰਜੀ_ਕਿਸਮ - 1, "");
            ਸਥਿਤੀ = XKB_ERROR_INVALID;
        }}

        ਜੇ (ਸਥਿਤੀ != XKB_SUCCESS &&
            ਸਥਿਤੀ != (ਗਿਣਤੀ xkb_ਸਥਿਤੀ)XKB_ERROR_NO_RELEASED_ਕੁੰਜੀ)
        {
            ਜਾਓ error;
        }
    }

    ਪ੍ਰਤਿਜ੍ਞਾ(!state.ਅੰਕ_pending);

    typing->events = steal(&state.events);
    typing->ਅੰਕ_events = ਅੰਕ_events;

    ਸਥਿਤੀ = XKB_SUCCESS;

error:
    darray_free(pending);
    xkb_typing_state_destroy(&state);

    ਵਾਪਸ ਸਥਿਤੀ;
}

ਖਾਲੀ
xkb_typing_ਨਿਵੇਸ਼_destroy(ਬਣਤਰ xkb_typing_ਨਿਵੇਸ਼ *typing)
{
    ਜੇ (!typing)
        ਵਾਪਸ;

    ਮੁਕਤ(typing->events);
    typing->events = ਨਿਰਾ;
}
