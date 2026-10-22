/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <stdbool.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଯଦି WCHAR_MAX >= 0x7FFFFFFF
#ଅନ୍ତର୍ଭାଵ <wctype.h>
#define USE_ISWPRINT
#ଯଦି HAVE_NEWLOCALE && HAVE_ISWPRINT_L
#define USE_ISWPRINT_L
#ଅନ୍ତର୍ଭାଵ <locale.h>
#endif
#ଅନ୍ୟଥା
#ଅନ୍ତର୍ଭାଵ <ctype.h>
#endif

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon-status.h"
#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "darray.h"
#ଅନ୍ତର୍ଭାଵ "state-priv.h"
#ଅନ୍ତର୍ଭାଵ "typing.h"
#ଅନ୍ତର୍ଭାଵ "util-bits.h"
#ଅନ୍ତର୍ଭାଵ "util-mem.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"

ଗଣନା xkb_status
xkb_ଚାବି_set_init(ସଂରଚନା xkb_ଚାବି_set *set,
                 ସଂରଚନା xkb_keymap *keymap,
                 ଅଚଳ ସଂରଚନା xkb_ଚାବି_set_config *config)
{
    darray_init(set->printable);
    darray_init(set->modifiers);
    darray_init(set->misc);

    ସ୍ଥିର ଅଚଳ ସଂରଚନା xkb_ଚାବି_set_config default_config = {0};
    ଯଦି (!config)
        config = &default_config;
    /* keycode = 0 denotes “default” */
    ଅଚଳ xkb_keycode_t min = MAX(config->min, xkb_keymap_min_keycode(keymap));
    ଅଚଳ xkb_keycode_t max = config->max
        ? MIN(config->max, xkb_keymap_max_keycode(keymap))
        : xkb_keymap_max_keycode(keymap);

    ଗଣନା xkb_status status;
    ସଂରଚନା xkb_state *state =
        xkb_state_new_with_mode(keymap, XKB_STATE_MODE_SERVER, &status);
    ଯଦି (!state)
        ଫେରନ୍ତୁ status;

    #ifdef USE_ISWPRINT_L
    locale_t loc = newlocale(LC_ALL_MASK, "C.UTF-8", (locale_t)0);
    #endif

    ସଂରଚନା xkb_keymap_ଚାବି_iterator iter;
    ଅଚଳ ସଂରଚନା xkb_keymap_ଚାବି_iterator_config iter_config = {
        .size = ଆକାର(iter_config),
        .flags = XKB_KEYMAP_ଚାବି_ITERATOR_NO_FLAGS,
        .start = min
    };
    status = xkb_keymap_ଚାବି_iterator_init(&iter, keymap, &iter_config);
    ଯଦି (status != XKB_SUCCESS)
        ଯାଅ error;

    ଅଚଳ xkb_mod_mask_t numlock =
        xkb_keymap_mod_get_mask(keymap, XKB_VMOD_NAME_ଅଙ୍କ);

    ଅଚଳ ସଂରଚନା xkb_state_components_update components_update = {
        .size = ଆକାର(components_update),
        .components = XKB_STATE_MODS_LOCKED,
        .affect_locked_mods = numlock,
        .locked_mods = numlock,
    };

    ଅଚଳ ସଂରଚନା xkb_synthetic_update update = {
        .size = ଆକାର(update),
        .components = &components_update,
    };

    xkb_keycode_t k;
    ଯେତେବେଳେ ((k = xkb_keymap_ଚାବି_iterator_next(&iter)) != XKB_KEYCODE_INVALID) {
        ଯଦି (k > max)
            ଭାଙ୍ଗ;

        xkb_state_reset(state);
        status = xkb_state_update_synthetic(state, &update, ଖାଲି);
        ଯଦି (status != XKB_SUCCESS)
            ଯାଅ error;

        ଅଚଳ xkb_keysym_t *syms = ଖାଲି;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = xkb_state_ଚାବି_get_syms(state, k, &syms);

        ଗଣନା xkb_state_component changed =
            xkb_state_update_ଚାବି(state, k, XKB_ଚାବି_DOWN);
        xkb_mod_mask_t mods =
            xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);
        changed |= xkb_state_update_ଚାବି(state, k, XKB_ଚାବି_UP);
        mods |= xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);

        ଯଦି (changed & XKB_STATE_MODS_EFFECTIVE) {
            /* Modifier key */
            ଗଣନା xkb_modifier_kind kind;
            ଯଦି (changed & XKB_STATE_MODS_LOCKED) {
                kind = XKB_MOD_LOCK;
            } ଅନ୍ୟଥା ଯଦି (changed & XKB_STATE_MODS_LATCHED) {
                kind = XKB_MOD_LATCH;
            } ଅନ୍ୟଥା {
                kind = XKB_MOD_SET;
            }
            mods &= ~numlock;
            ଯଦି (!mods)
                mods = numlock;
            darray_append(set->modifiers, (ସଂରଚନା xkb_modifier_ଚାବି) {
                .keycode = k,
                .mods = mods,
                .kind = kind,
            });
            ଜାରି;
        }

        ଯଦି (ଗଣନା_2 > 0) {
            uint32_t utf32 = 0;
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 0; s < ଗଣନା_2; s++) {
                utf32 = xkb_keysym_to_utf32(syms[s]);
                ଅଚଳ bool is_printable =
                    #ifdef USE_ISWPRINT_L
                    ((loc && iswprint_l((wint_t)utf32, loc)) || iswprint((wint_t)utf32))
                    #ନଚେତଯଦି defined(USE_ISWPRINT)
                    iswprint((wint_t)utf32)
                    #ଅନ୍ୟଥା
                    utf32 <= 0x7f && isprint((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)utf32)
                    #endif
                    ;
                ଯଦି (is_printable)
                    ଭାଙ୍ଗ;
                utf32 = 0;
            }
            ଯଦି (utf32) {
                /* Printable keysyms */
                darray_append(set->printable, k);
                ଜାରି;
            }
        }

        /* Miscellaneous key */
        darray_append(set->misc, k);
    }

error:
    #ifdef USE_ISWPRINT_L
    ଯଦି (loc)
        freelocale(loc);
    #endif

    xkb_state_unref(state);
    ଯଦି (status != XKB_SUCCESS)
        xkb_ଚାବି_set_destroy(set);

    ଫେରନ୍ତୁ status;
}

ଶୂନ୍ୟ
xkb_ଚାବି_set_destroy(ସଂରଚନା xkb_ଚାବି_set *set)
{
    ଯଦି (!set)
        ଫେରନ୍ତୁ;

    darray_free(set->modifiers);
    darray_free(set->printable);
    darray_free(set->misc);
}

ସ୍ଥିର ଗଣନା xkb_status
add_pending_events(ସଂରଚନା xkb_typing_state * restrict state,
                   ଅଚଳ ସଂରଚନା xkb_pending_typing_event * restrict events,
                   size_t ଗଣନା_2)
{
    ପାଇଁ (size_t e = 0; e < ଗଣନା_2; e++) {
        /* Look for a free slot */
        ସଂରଚନା xkb_pending_typing_event *pending;
        darray_size_t idx = 0;
        darray_enumerate_from(idx, pending, state->pending, idx) {
            ଯଦି (!pending->counter) {
                darray_item(state->pending, idx) = events[e];
                ଯାଅ next;
            }
        }
        /* No free slot found: append new one */
        idx = darray_size(state->pending);
        darray_append(state->pending, events[e]);
    next:
        /* Avoid immediate consumption */
        darray_item(state->pending, idx).counter++;
    }
    state->ଅଙ୍କ_pending += ଗଣନା_2;

    ଫେରନ୍ତୁ XKB_SUCCESS;
}

ସ୍ଥିର ଗଣନା xkb_status
next_pending_events(ସଂରଚନା xkb_typing_state * restrict state,
                    xkb_typing_events * restrict out)
{
    /* Reset the output */
    darray_size(*out) = 0;

    darray_size_t idx;
    ସଂରଚନା xkb_pending_typing_event *pending;
    /* Stack: start from the end */
    darray_enumerate_reverse(idx, pending, state->pending) {
        ଯଦି (!pending->counter)
            ଜାରି;
        pending->counter--;
        ଯଦି (pending->counter)
            ଜାରି;
        darray_append(*out, pending->event);
        state->ଅଙ୍କ_pending--;
        /* Resize array if it was the last element */
        ଯଦି (idx + 1 == darray_size(state->pending))
            darray_size(state->pending)--;
        state->mods &= ~pending->mods;
    }

    ଫେରନ୍ତୁ XKB_SUCCESS;
}

ଗଣନା {
    /** Tries to avoid pressing a key already pressed */
    ଚାବି_CHOICE_MAX_TRIES = 12,
    XKB_ERROR_NO_RELEASED_ଚାବି = -1000,
};

ସ୍ଥିର ଗଣନା xkb_status
add_modifier_ଚାବି(ସଂରଚନା xkb_typing_state * restrict state, size_t * restrict e)
{
    /* Choose a random key */
    long idx = -1;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା tries = 0; tries < ଚାବି_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state)
            % darray_size(state->ଚାବିଗୁଡ଼ିକ->modifiers);
        /*
         * Ensure key is not already down and its modifier mask is not active,
         * i.e. to avoid simultaneous keys with the same modifier.
         */
        ଯଦି (!bit_array_get(state->ଚାବିଗୁଡ଼ିକ_state,
                           darray_item(state->ଚାବିଗୁଡ଼ିକ->modifiers, idx).keycode) &&
            !(state->mods & darray_item(state->ଚାବିଗୁଡ଼ିକ->modifiers, idx).mods))
        {
            ଭାଙ୍ଗ;
        }
        idx = -1;
    }

    ସଂରଚନା xkb_modifier_ଚାବି ଚାବି = {0};
    ଯଦି (idx < 0) {
        /* No released key found: fall back to linear search */
        ଅଚଳ ସଂରଚନା xkb_modifier_ଚାବି *ଚାବି_2;
        darray_foreach(ଚାବି_2, state->ଚାବିଗୁଡ଼ିକ->modifiers) {
            /* See test doc above */
            ଯଦି (!bit_array_get(state->ଚାବିଗୁଡ଼ିକ_state, ଚାବି_2->keycode) &&
                !(state->mods & ଚାବି_2->mods))
            {
                ଚାବି = *ଚାବି_2;
                idx = 0;
                ଭାଙ୍ଗ;
            }
        }
        ଯଦି (idx < 0) {
            /* No fallback: skip */
            ଫେରନ୍ତୁ (ଗଣନା xkb_status)XKB_ERROR_NO_RELEASED_ଚାବି;
        }
    } ଅନ୍ୟଥା {
        ଚାବି = darray_item(state->ଚାବିଗୁଡ଼ିକ->modifiers, idx);
    }

    ଅଚଳ size_t remaining_events =
        state->ଅଙ୍କ_events - state->ଅଙ୍କ_pending - *e;
    size_t pending_turns = 0;

    ଯଦି (remaining_events >= 2) {
        ଯଦି (ଚାବି.kind == XKB_MOD_SET ||
            (ଚାବି.kind == XKB_MOD_LOCK && remaining_events >= 4))
        {
            /* Delay SetMods release or LockMods unlock */
            ଅଚଳ size_t max_turns = MIN(
                (size_t)(
                    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS -
                    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                 ),
                remaining_events + state->ଅଙ୍କ_pending
            );
            pending_turns = XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                          + (size_t)state->prng(state->prng_state)
                          % (max_turns + 1);
        }
    } ଅନ୍ୟଥା {
        ଫେରନ୍ତୁ XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (ସଂରଚନା xkb_typing_event) {
        .keycode = ଚାବି.keycode,
        .direction = XKB_ଚାବି_DOWN,
    };

    ସଂରଚନା xkb_pending_typing_event pending[2] = {
        {
            .event = {
                .keycode = ଚାବି.keycode,
                .direction = XKB_ଚାବି_DOWN,
            },
            .counter = (uint8_t)pending_turns,
        },
        {
            .event = {
                .keycode = ଚାବି.keycode,
                .direction = XKB_ଚାବି_UP,
            },
            .mods = ଚାବି.mods,
            .counter = (uint8_t)pending_turns,
        },
    };

    ଯଦି (pending_turns) {
        ଯଦି (ଚାବି.kind == XKB_MOD_LOCK) {
            /* Immediate release */
            state->events[(*e)++] = pending[1].event;
            /* Delayed unlock (press + release) */
            pending[1].counter++;
            ଅଚଳ ଗଣନା xkb_status status =
                add_pending_events(state, pending, ARRAY_SIZE(pending));
            ଯଦି (status != XKB_SUCCESS)
                ଫେରନ୍ତୁ status;
            /*
             * Register “locked” state, to avoid tapping the key before
             * delayed key released
             */
            bit_array_set(state->ଚାବିଗୁଡ଼ିକ_state, ଚାବି.keycode);
        } ଅନ୍ୟଥା {
            /* Delayed release */
            ଅଚଳ ଗଣନା xkb_status status =
                add_pending_events(state, &pending[1], 1);
            ଯଦି (status != XKB_SUCCESS)
                ଫେରନ୍ତୁ status;
            /* Register pressed state */
            bit_array_set(state->ଚାବିଗୁଡ଼ିକ_state, ଚାବି.keycode);
        }
        state->mods |= ଚାବି.mods;
    } ଅନ୍ୟଥା {
        /* Immediate release */
        state->events[(*e)++] = pending[1].event;
        /* TODO: add latched modifier to mask state. */
    }

    ଫେରନ୍ତୁ XKB_SUCCESS;
}

ସ୍ଥିର ଗଣନା xkb_status
add_other_ଚାବି(ସଂରଚନା xkb_typing_state * restrict state,
              ଅଚଳ xkb_keycodes_t * restrict keycodes, size_t * restrict e,
              uint8_t immediate_release_prob, uint8_t max_pending_turns)
{
    /* Choose a random key */
    long idx = -1;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା tries = 0; tries < ଚାବି_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state) % darray_size(*keycodes);
        /* Ensure key is not already down */
        ଯଦି (!bit_array_get(state->ଚାବିଗୁଡ଼ିକ_state,
                           darray_item(*keycodes, idx)))
        {
            ଭାଙ୍ଗ;
        }
        idx = -1;
    }

    xkb_keycode_t keycode = XKB_KEYCODE_INVALID;
    ଯଦି (idx < 0) {
        /* No released key found: fall back to linear search */
        ଅଚଳ xkb_keycode_t *keycode_;
        darray_foreach(keycode_, *keycodes) {
            ଯଦି (!bit_array_get(state->ଚାବିଗୁଡ଼ିକ_state, *keycode_)) {
                keycode = *keycode_;
                idx = 0;
                ଭାଙ୍ଗ;
            }
        }
        ଯଦି (idx < 0) {
            /* No fallback: skip */
            ଫେରନ୍ତୁ (ଗଣନା xkb_status)XKB_ERROR_NO_RELEASED_ଚାବି;
        }
    } ଅନ୍ୟଥା {
        keycode = darray_item(*keycodes, idx);
    }

    ଅଚଳ size_t remaining_events =
        state->ଅଙ୍କ_events - state->ଅଙ୍କ_pending - *e;
    size_t pending_turns;

    ଯଦି (remaining_events >= 2) {
        ଅଚଳ size_t max_turns = MIN(
            (size_t)max_pending_turns,
            remaining_events + state->ଅଙ୍କ_pending
        );
        ଅଚଳ bool immediate_release = (
            (immediate_release_prob >= 100) ||
            !max_turns ||
            (state->prng(state->prng_state) % 100) < immediate_release_prob
        );
        ଯଦି (immediate_release) {
            pending_turns = 0;
        } ଅନ୍ୟଥା {
            pending_turns = 1 + (size_t)state->prng(state->prng_state)
                          % max_turns;
        }
    } ଅନ୍ୟଥା {
        ଫେରନ୍ତୁ XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (ସଂରଚନା xkb_typing_event) {
        .keycode = keycode,
        .direction = XKB_ଚାବି_DOWN,
    };

    ଅଚଳ ସଂରଚନା xkb_pending_typing_event pending = {
        .event = {
            .keycode = keycode,
            .direction = XKB_ଚାବି_UP,
        },
        .counter = (uint8_t)pending_turns,
    };
    ଯଦି (pending_turns) {
        ଅଚଳ ଗଣନା xkb_status status = add_pending_events(state, &pending, 1);
        ଯଦି (status != XKB_SUCCESS)
            ଫେରନ୍ତୁ status;
        /* Register pressed state */
        bit_array_set(state->ଚାବିଗୁଡ଼ିକ_state, keycode);
    } ଅନ୍ୟଥା {
        state->events[(*e)++] = pending.event;
    }

    ଫେରନ୍ତୁ XKB_SUCCESS;
}

ସ୍ଥିର ଗଣନା xkb_ଚାବି_kind
pick_ଚାବି_kind(xkb_prng_t prng, ଶୂନ୍ୟ *prng_state)
{
    ସ୍ଥିର ଅଚଳ uint8_t weights[ଅଙ୍କ_XKB_ଚାବି_KIND] = {
        [XKB_ଚାବି_PRINTABLE] = 60,
        [XKB_ଚାବି_MODIFIER] = 30,
        [XKB_ଚାବି_MISC] = 10,
    };
    static_assert(XKB_ଚାବି_MISC == 2 &&
                  XKB_ଚାବି_MISC == ଅଙ୍କ_XKB_ଚାବି_KIND - 1, "");
    ପ୍ରତିଜ୍ଞା(weights[0] + weights[1] + weights[2] == 100);

    long r = prng(prng_state) % 100;
    ପାଇଁ (ଗଣନା xkb_ଚାବି_kind k = 0; k < ଅଙ୍କ_XKB_ଚାବି_KIND; k++)
        ଯଦି ((r -= weights[k]) < 0)
            ଫେରନ୍ତୁ k;
    ଫେରନ୍ତୁ XKB_ଚାବି_PRINTABLE;
}

ସ୍ଥିର ଶୂନ୍ୟ
xkb_typing_state_destroy(ସଂରଚନା xkb_typing_state *state)
{
    ମୁକ୍ତ(state->ଚାବିଗୁଡ଼ିକ_state);
    darray_free(state->pending);
    ମୁକ୍ତ(state->events);
}

ଗଣନା xkb_status
xkb_typing_ନିବେଶ_init(ସଂରଚନା xkb_typing_ନିବେଶ * restrict typing,
                      ଅଚଳ ସଂରଚନା xkb_ଚାବି_set * restrict ଚାବିଗୁଡ଼ିକ,
                      ଅଚଳ ସଂରଚନା xkb_typing_ନିବେଶ_config * restrict config)
{
    typing->events = ଖାଲି;
    typing->ଅଙ୍କ_events = 0;

    ଯଦି (!config->ନିବେଶ_length)
        ଫେରନ୍ତୁ XKB_ERROR_INVALID;

    ଯଦି (darray_empty(ଚାବିଗୁଡ଼ିକ->printable) ||
        darray_empty(ଚାବିଗୁଡ଼ିକ->modifiers) ||
        darray_empty(ଚାବିଗୁଡ଼ିକ->misc))
    {
        ଫେରନ୍ତୁ XKB_ERROR_INVALID;
    }

    ଗଣନା xkb_status status = XKB_SUCCESS;

    /* NOTE: arrays are sorted because they were built using the key iterator */
    xkb_keycode_t keycode_max = darray_last(ଚାବିଗୁଡ଼ିକ->printable, 0);
    keycode_max = MAX(
        darray_last(ଚାବିଗୁଡ଼ିକ->modifiers, (ସଂରଚନା xkb_modifier_ଚାବି){0}).keycode,
        keycode_max
    );
    keycode_max = MAX(darray_last(ଚାବିଗୁଡ଼ିକ->misc, 0), keycode_max);
    ଅଚଳ size_t ଚାବିଗୁଡ଼ିକ_state_ଅଙ୍କ = bit_array_ଶବ୍ଦଗୁଡ଼ିକ((size_t)keycode_max + 1);
    uintptr_t *ଚାବିଗୁଡ଼ିକ_state = calloc(ଚାବିଗୁଡ଼ିକ_state_ଅଙ୍କ, ଆକାର(*ଚାବିଗୁଡ଼ିକ_state));

    ଅଚଳ size_t ଅଙ୍କ_events = config->ନିବେଶ_length;
    ସଂରଚନା xkb_typing_event *events = calloc(ଅଙ୍କ_events, ଆକାର(*events));

    ସଂରଚନା xkb_typing_state state = {
        .ଚାବିଗୁଡ଼ିକ = ଚାବିଗୁଡ଼ିକ,
        .ଚାବିଗୁଡ଼ିକ_state = ଚାବିଗୁଡ଼ିକ_state,
        .prng = config->prng,
        .prng_state = config->prng_state,
        .events = events,
        .ଅଙ୍କ_events = ଅଙ୍କ_events,
        .pending = darray_new(),
        .ଅଙ୍କ_pending = 0,
    };

    xkb_typing_events pending = darray_new();

    ଯଦି (!ଚାବିଗୁଡ଼ିକ_state || !events) {
        status = XKB_ERROR_ALLOCATION_FAILURE;
        ଯାଅ error;
    }

    /*
     * Invariant: e + state.num_pending <= state.num_events
     */

    ପାଇଁ (size_t e = 0; e < ଅଙ୍କ_events;) {
        /*
         * Process pending events
         */
        status = next_pending_events(&state, &pending);
        ଯଦି (status != XKB_SUCCESS) {
            ଯାଅ error;
        }
        ସଂରଚନା xkb_typing_event *event;
        darray_foreach(event, pending) {
            ଯଦି (e >= ଅଙ୍କ_events) {
                /* invariant violated: should be unreachable */
                PANIC_UNREACHABLE();
            }
            events[e] = *event;
            bit_array_assign(ଚାବିଗୁଡ଼ିକ_state, event->keycode, (bool)event->direction);
            e++;
        }

        ଯଦି (!state.ଅଙ୍କ_pending && e == ଅଙ୍କ_events - 1) {
            /*
             * Cannot add press/release *pair*, so just use an invalid keycode
             * for the last event.
             */
            events[e] = (ସଂରଚନା xkb_typing_event) {
                .keycode = XKB_KEYCODE_INVALID,
                .direction = XKB_ଚାବି_DOWN,
            };
            ଭାଙ୍ଗ;
        }

        ଯଦି (e + 1 + state.ଅଙ୍କ_pending >= ଅଙ୍କ_events) {
            /* Cannot add new pair of events: require flushing pending events */
            ଜାରି;
        }

        /*
         * Create new events
         */

        /* Choose kind of key */
        static_assert(XKB_ଚାବି_PRINTABLE == 0, "");
        ଅଚଳ ଗଣନା xkb_ଚାବି_kind ଚାବି_kind =
            pick_ଚାବି_kind(config->prng, config->prng_state);
        ବାଛ (ଚାବି_kind) {
        ସ୍ଥିତି XKB_ଚାବି_PRINTABLE:
            status = add_other_ଚାବି(
                &state, &state.ଚାବିଗୁଡ଼ିକ->printable, &e,
                XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS
            );
            ଭାଙ୍ଗ;
        ସ୍ଥିତି XKB_ଚାବି_MODIFIER:
            status = add_modifier_ଚାବି(&state, &e);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି XKB_ଚାବି_MISC:
            status = add_other_ଚାବି(
                &state, &state.ଚାବିଗୁଡ଼ିକ->misc, &e,
                XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS
            );
            ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ: {
            static_assert(XKB_ଚାବି_MISC == 2 &&
                          XKB_ଚାବି_MISC == ଅଙ୍କ_XKB_ଚାବି_KIND - 1, "");
            status = XKB_ERROR_INVALID;
        }}

        ଯଦି (status != XKB_SUCCESS &&
            status != (ଗଣନା xkb_status)XKB_ERROR_NO_RELEASED_ଚାବି)
        {
            ଯାଅ error;
        }
    }

    ପ୍ରତିଜ୍ଞା(!state.ଅଙ୍କ_pending);

    typing->events = steal(&state.events);
    typing->ଅଙ୍କ_events = ଅଙ୍କ_events;

    status = XKB_SUCCESS;

error:
    darray_free(pending);
    xkb_typing_state_destroy(&state);

    ଫେରନ୍ତୁ status;
}

ଶୂନ୍ୟ
xkb_typing_ନିବେଶ_destroy(ସଂରଚନା xkb_typing_ନିବେଶ *typing)
{
    ଯଦି (!typing)
        ଫେରନ୍ତୁ;

    ମୁକ୍ତ(typing->events);
    typing->events = ଖାଲି;
}
