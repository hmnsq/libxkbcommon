/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <stdbool.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <stdint.h>
#எனில் WCHAR_MAX >= 0x7FFFFFFF
#அந்தர்பாவ <wctype.h>
#define USE_ISWPRINT
#எனில் HAVE_NEWLOCALE && HAVE_ISWPRINT_L
#define USE_ISWPRINT_L
#அந்தர்பாவ <locale.h>
#endif
#இல்லையெனில்
#அந்தர்பாவ <ctype.h>
#endif

#அந்தர்பாவ "xkbcommon/xkbcommon-status.h"
#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "state-priv.h"
#அந்தர்பாவ "typing.h"
#அந்தர்பாவ "util-bits.h"
#அந்தர்பாவ "util-mem.h"
#அந்தர்பாவ "utils.h"

எண்ணிக்கை xkb_status
xkb_சாவி_set_init(கட்டமைப்பு xkb_சாவி_set *set,
                 கட்டமைப்பு xkb_keymap *keymap,
                 மாறிலி கட்டமைப்பு xkb_சாவி_set_config *config)
{
    darray_init(set->printable);
    darray_init(set->modifiers);
    darray_init(set->misc);

    நிலையான மாறிலி கட்டமைப்பு xkb_சாவி_set_config default_config = {0};
    எனில் (!config)
        config = &default_config;
    /* keycode = 0 denotes “default” */
    மாறிலி xkb_keycode_t min = MAX(config->min, xkb_keymap_min_keycode(keymap));
    மாறிலி xkb_keycode_t max = config->max
        ? MIN(config->max, xkb_keymap_max_keycode(keymap))
        : xkb_keymap_max_keycode(keymap);

    எண்ணிக்கை xkb_status status;
    கட்டமைப்பு xkb_state *state =
        xkb_state_new_with_mode(keymap, XKB_STATE_MODE_SERVER, &status);
    எனில் (!state)
        திரும்பு status;

    #ifdef USE_ISWPRINT_L
    locale_t loc = newlocale(LC_ALL_MASK, "C.UTF-8", (locale_t)0);
    #endif

    கட்டமைப்பு xkb_keymap_சாவி_iterator iter;
    மாறிலி கட்டமைப்பு xkb_keymap_சாவி_iterator_config iter_config = {
        .size = அளவு(iter_config),
        .flags = XKB_KEYMAP_சாவி_ITERATOR_NO_FLAGS,
        .start = min
    };
    status = xkb_keymap_சாவி_iterator_init(&iter, keymap, &iter_config);
    எனில் (status != XKB_SUCCESS)
        செல் error;

    மாறிலி xkb_mod_mask_t numlock =
        xkb_keymap_mod_get_mask(keymap, XKB_VMOD_NAME_இலக்கம்);

    மாறிலி கட்டமைப்பு xkb_state_components_update components_update = {
        .size = அளவு(components_update),
        .components = XKB_STATE_MODS_LOCKED,
        .affect_locked_mods = numlock,
        .locked_mods = numlock,
    };

    மாறிலி கட்டமைப்பு xkb_synthetic_update update = {
        .size = அளவு(update),
        .components = &components_update,
    };

    xkb_keycode_t k;
    போது ((k = xkb_keymap_சாவி_iterator_next(&iter)) != XKB_KEYCODE_INVALID) {
        எனில் (k > max)
            நிறுத்து;

        xkb_state_reset(state);
        status = xkb_state_update_synthetic(state, &update, வெற்று);
        எனில் (status != XKB_SUCCESS)
            செல் error;

        மாறிலி xkb_keysym_t *syms = வெற்று;
        மாறிலி முழு மொத்தம் = xkb_state_சாவி_get_syms(state, k, &syms);

        எண்ணிக்கை xkb_state_component changed =
            xkb_state_update_சாவி(state, k, XKB_சாவி_DOWN);
        xkb_mod_mask_t mods =
            xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);
        changed |= xkb_state_update_சாவி(state, k, XKB_சாவி_UP);
        mods |= xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);

        எனில் (changed & XKB_STATE_MODS_EFFECTIVE) {
            /* Modifier key */
            எண்ணிக்கை xkb_modifier_kind kind;
            எனில் (changed & XKB_STATE_MODS_LOCKED) {
                kind = XKB_MOD_LOCK;
            } இல்லையெனில் எனில் (changed & XKB_STATE_MODS_LATCHED) {
                kind = XKB_MOD_LATCH;
            } இல்லையெனில் {
                kind = XKB_MOD_SET;
            }
            mods &= ~numlock;
            எனில் (!mods)
                mods = numlock;
            darray_append(set->modifiers, (கட்டமைப்பு xkb_modifier_சாவி) {
                .keycode = k,
                .mods = mods,
                .kind = kind,
            });
            தொடர்;
        }

        எனில் (மொத்தம் > 0) {
            uint32_t utf32 = 0;
            க்கு (முழு s = 0; s < மொத்தம்; s++) {
                utf32 = xkb_keysym_to_utf32(syms[s]);
                மாறிலி bool is_printable =
                    #ifdef USE_ISWPRINT_L
                    ((loc && iswprint_l((wint_t)utf32, loc)) || iswprint((wint_t)utf32))
                    #அல்லதெனில் defined(USE_ISWPRINT)
                    iswprint((wint_t)utf32)
                    #இல்லையெனில்
                    utf32 <= 0x7f && isprint((முழு)utf32)
                    #endif
                    ;
                எனில் (is_printable)
                    நிறுத்து;
                utf32 = 0;
            }
            எனில் (utf32) {
                /* Printable keysyms */
                darray_append(set->printable, k);
                தொடர்;
            }
        }

        /* Miscellaneous key */
        darray_append(set->misc, k);
    }

error:
    #ifdef USE_ISWPRINT_L
    எனில் (loc)
        freelocale(loc);
    #endif

    xkb_state_unref(state);
    எனில் (status != XKB_SUCCESS)
        xkb_சாவி_set_destroy(set);

    திரும்பு status;
}

வெறுமை
xkb_சாவி_set_destroy(கட்டமைப்பு xkb_சாவி_set *set)
{
    எனில் (!set)
        திரும்பு;

    darray_free(set->modifiers);
    darray_free(set->printable);
    darray_free(set->misc);
}

நிலையான எண்ணிக்கை xkb_status
add_pending_events(கட்டமைப்பு xkb_typing_state * restrict state,
                   மாறிலி கட்டமைப்பு xkb_pending_typing_event * restrict events,
                   size_t மொத்தம்)
{
    க்கு (size_t e = 0; e < மொத்தம்; e++) {
        /* Look for a free slot */
        கட்டமைப்பு xkb_pending_typing_event *pending;
        darray_size_t idx = 0;
        darray_enumerate_from(idx, pending, state->pending, idx) {
            எனில் (!pending->counter) {
                darray_item(state->pending, idx) = events[e];
                செல் next;
            }
        }
        /* No free slot found: append new one */
        idx = darray_size(state->pending);
        darray_append(state->pending, events[e]);
    next:
        /* Avoid immediate consumption */
        darray_item(state->pending, idx).counter++;
    }
    state->இலக்கம்_pending += மொத்தம்;

    திரும்பு XKB_SUCCESS;
}

நிலையான எண்ணிக்கை xkb_status
next_pending_events(கட்டமைப்பு xkb_typing_state * restrict state,
                    xkb_typing_events * restrict out)
{
    /* Reset the output */
    darray_size(*out) = 0;

    darray_size_t idx;
    கட்டமைப்பு xkb_pending_typing_event *pending;
    /* Stack: start from the end */
    darray_enumerate_reverse(idx, pending, state->pending) {
        எனில் (!pending->counter)
            தொடர்;
        pending->counter--;
        எனில் (pending->counter)
            தொடர்;
        darray_append(*out, pending->event);
        state->இலக்கம்_pending--;
        /* Resize array if it was the last element */
        எனில் (idx + 1 == darray_size(state->pending))
            darray_size(state->pending)--;
        state->mods &= ~pending->mods;
    }

    திரும்பு XKB_SUCCESS;
}

எண்ணிக்கை {
    /** Tries to avoid pressing a key already pressed */
    சாவி_CHOICE_MAX_TRIES = 12,
    XKB_ERROR_NO_RELEASED_சாவி = -1000,
};

நிலையான எண்ணிக்கை xkb_status
add_modifier_சாவி(கட்டமைப்பு xkb_typing_state * restrict state, size_t * restrict e)
{
    /* Choose a random key */
    long idx = -1;
    க்கு (முழு tries = 0; tries < சாவி_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state)
            % darray_size(state->சாவிகள்->modifiers);
        /*
         * Ensure key is not already down and its modifier mask is not active,
         * i.e. to avoid simultaneous keys with the same modifier.
         */
        எனில் (!bit_array_get(state->சாவிகள்_state,
                           darray_item(state->சாவிகள்->modifiers, idx).keycode) &&
            !(state->mods & darray_item(state->சாவிகள்->modifiers, idx).mods))
        {
            நிறுத்து;
        }
        idx = -1;
    }

    கட்டமைப்பு xkb_modifier_சாவி சாவி = {0};
    எனில் (idx < 0) {
        /* No released key found: fall back to linear search */
        மாறிலி கட்டமைப்பு xkb_modifier_சாவி *சாவி_2;
        darray_foreach(சாவி_2, state->சாவிகள்->modifiers) {
            /* See test doc above */
            எனில் (!bit_array_get(state->சாவிகள்_state, சாவி_2->keycode) &&
                !(state->mods & சாவி_2->mods))
            {
                சாவி = *சாவி_2;
                idx = 0;
                நிறுத்து;
            }
        }
        எனில் (idx < 0) {
            /* No fallback: skip */
            திரும்பு (எண்ணிக்கை xkb_status)XKB_ERROR_NO_RELEASED_சாவி;
        }
    } இல்லையெனில் {
        சாவி = darray_item(state->சாவிகள்->modifiers, idx);
    }

    மாறிலி size_t remaining_events =
        state->இலக்கம்_events - state->இலக்கம்_pending - *e;
    size_t pending_turns = 0;

    எனில் (remaining_events >= 2) {
        எனில் (சாவி.kind == XKB_MOD_SET ||
            (சாவி.kind == XKB_MOD_LOCK && remaining_events >= 4))
        {
            /* Delay SetMods release or LockMods unlock */
            மாறிலி size_t max_turns = MIN(
                (size_t)(
                    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS -
                    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                 ),
                remaining_events + state->இலக்கம்_pending
            );
            pending_turns = XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                          + (size_t)state->prng(state->prng_state)
                          % (max_turns + 1);
        }
    } இல்லையெனில் {
        திரும்பு XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (கட்டமைப்பு xkb_typing_event) {
        .keycode = சாவி.keycode,
        .direction = XKB_சாவி_DOWN,
    };

    கட்டமைப்பு xkb_pending_typing_event pending[2] = {
        {
            .event = {
                .keycode = சாவி.keycode,
                .direction = XKB_சாவி_DOWN,
            },
            .counter = (uint8_t)pending_turns,
        },
        {
            .event = {
                .keycode = சாவி.keycode,
                .direction = XKB_சாவி_UP,
            },
            .mods = சாவி.mods,
            .counter = (uint8_t)pending_turns,
        },
    };

    எனில் (pending_turns) {
        எனில் (சாவி.kind == XKB_MOD_LOCK) {
            /* Immediate release */
            state->events[(*e)++] = pending[1].event;
            /* Delayed unlock (press + release) */
            pending[1].counter++;
            மாறிலி எண்ணிக்கை xkb_status status =
                add_pending_events(state, pending, ARRAY_SIZE(pending));
            எனில் (status != XKB_SUCCESS)
                திரும்பு status;
            /*
             * Register “locked” state, to avoid tapping the key before
             * delayed key released
             */
            bit_array_set(state->சாவிகள்_state, சாவி.keycode);
        } இல்லையெனில் {
            /* Delayed release */
            மாறிலி எண்ணிக்கை xkb_status status =
                add_pending_events(state, &pending[1], 1);
            எனில் (status != XKB_SUCCESS)
                திரும்பு status;
            /* Register pressed state */
            bit_array_set(state->சாவிகள்_state, சாவி.keycode);
        }
        state->mods |= சாவி.mods;
    } இல்லையெனில் {
        /* Immediate release */
        state->events[(*e)++] = pending[1].event;
        /* TODO: add latched modifier to mask state. */
    }

    திரும்பு XKB_SUCCESS;
}

நிலையான எண்ணிக்கை xkb_status
add_other_சாவி(கட்டமைப்பு xkb_typing_state * restrict state,
              மாறிலி xkb_keycodes_t * restrict keycodes, size_t * restrict e,
              uint8_t immediate_release_prob, uint8_t max_pending_turns)
{
    /* Choose a random key */
    long idx = -1;
    க்கு (முழு tries = 0; tries < சாவி_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state) % darray_size(*keycodes);
        /* Ensure key is not already down */
        எனில் (!bit_array_get(state->சாவிகள்_state,
                           darray_item(*keycodes, idx)))
        {
            நிறுத்து;
        }
        idx = -1;
    }

    xkb_keycode_t keycode = XKB_KEYCODE_INVALID;
    எனில் (idx < 0) {
        /* No released key found: fall back to linear search */
        மாறிலி xkb_keycode_t *keycode_;
        darray_foreach(keycode_, *keycodes) {
            எனில் (!bit_array_get(state->சாவிகள்_state, *keycode_)) {
                keycode = *keycode_;
                idx = 0;
                நிறுத்து;
            }
        }
        எனில் (idx < 0) {
            /* No fallback: skip */
            திரும்பு (எண்ணிக்கை xkb_status)XKB_ERROR_NO_RELEASED_சாவி;
        }
    } இல்லையெனில் {
        keycode = darray_item(*keycodes, idx);
    }

    மாறிலி size_t remaining_events =
        state->இலக்கம்_events - state->இலக்கம்_pending - *e;
    size_t pending_turns;

    எனில் (remaining_events >= 2) {
        மாறிலி size_t max_turns = MIN(
            (size_t)max_pending_turns,
            remaining_events + state->இலக்கம்_pending
        );
        மாறிலி bool immediate_release = (
            (immediate_release_prob >= 100) ||
            !max_turns ||
            (state->prng(state->prng_state) % 100) < immediate_release_prob
        );
        எனில் (immediate_release) {
            pending_turns = 0;
        } இல்லையெனில் {
            pending_turns = 1 + (size_t)state->prng(state->prng_state)
                          % max_turns;
        }
    } இல்லையெனில் {
        திரும்பு XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (கட்டமைப்பு xkb_typing_event) {
        .keycode = keycode,
        .direction = XKB_சாவி_DOWN,
    };

    மாறிலி கட்டமைப்பு xkb_pending_typing_event pending = {
        .event = {
            .keycode = keycode,
            .direction = XKB_சாவி_UP,
        },
        .counter = (uint8_t)pending_turns,
    };
    எனில் (pending_turns) {
        மாறிலி எண்ணிக்கை xkb_status status = add_pending_events(state, &pending, 1);
        எனில் (status != XKB_SUCCESS)
            திரும்பு status;
        /* Register pressed state */
        bit_array_set(state->சாவிகள்_state, keycode);
    } இல்லையெனில் {
        state->events[(*e)++] = pending.event;
    }

    திரும்பு XKB_SUCCESS;
}

நிலையான எண்ணிக்கை xkb_சாவி_kind
pick_சாவி_kind(xkb_prng_t prng, வெறுமை *prng_state)
{
    நிலையான மாறிலி uint8_t weights[இலக்கம்_XKB_சாவி_KIND] = {
        [XKB_சாவி_PRINTABLE] = 60,
        [XKB_சாவி_MODIFIER] = 30,
        [XKB_சாவி_MISC] = 10,
    };
    static_assert(XKB_சாவி_MISC == 2 &&
                  XKB_சாவி_MISC == இலக்கம்_XKB_சாவி_KIND - 1, "");
    ப்ரதிஜ்ஞா(weights[0] + weights[1] + weights[2] == 100);

    long r = prng(prng_state) % 100;
    க்கு (எண்ணிக்கை xkb_சாவி_kind k = 0; k < இலக்கம்_XKB_சாவி_KIND; k++)
        எனில் ((r -= weights[k]) < 0)
            திரும்பு k;
    திரும்பு XKB_சாவி_PRINTABLE;
}

நிலையான வெறுமை
xkb_typing_state_destroy(கட்டமைப்பு xkb_typing_state *state)
{
    விடுவி(state->சாவிகள்_state);
    darray_free(state->pending);
    விடுவி(state->events);
}

எண்ணிக்கை xkb_status
xkb_typing_உள்ளீடு_init(கட்டமைப்பு xkb_typing_உள்ளீடு * restrict typing,
                      மாறிலி கட்டமைப்பு xkb_சாவி_set * restrict சாவிகள்,
                      மாறிலி கட்டமைப்பு xkb_typing_உள்ளீடு_config * restrict config)
{
    typing->events = வெற்று;
    typing->இலக்கம்_events = 0;

    எனில் (!config->உள்ளீடு_length)
        திரும்பு XKB_ERROR_INVALID;

    எனில் (darray_empty(சாவிகள்->printable) ||
        darray_empty(சாவிகள்->modifiers) ||
        darray_empty(சாவிகள்->misc))
    {
        திரும்பு XKB_ERROR_INVALID;
    }

    எண்ணிக்கை xkb_status status = XKB_SUCCESS;

    /* NOTE: arrays are sorted because they were built using the key iterator */
    xkb_keycode_t keycode_max = darray_last(சாவிகள்->printable, 0);
    keycode_max = MAX(
        darray_last(சாவிகள்->modifiers, (கட்டமைப்பு xkb_modifier_சாவி){0}).keycode,
        keycode_max
    );
    keycode_max = MAX(darray_last(சாவிகள்->misc, 0), keycode_max);
    மாறிலி size_t சாவிகள்_state_இலக்கம் = bit_array_சொற்கள்((size_t)keycode_max + 1);
    uintptr_t *சாவிகள்_state = calloc(சாவிகள்_state_இலக்கம், அளவு(*சாவிகள்_state));

    மாறிலி size_t இலக்கம்_events = config->உள்ளீடு_length;
    கட்டமைப்பு xkb_typing_event *events = calloc(இலக்கம்_events, அளவு(*events));

    கட்டமைப்பு xkb_typing_state state = {
        .சாவிகள் = சாவிகள்,
        .சாவிகள்_state = சாவிகள்_state,
        .prng = config->prng,
        .prng_state = config->prng_state,
        .events = events,
        .இலக்கம்_events = இலக்கம்_events,
        .pending = darray_new(),
        .இலக்கம்_pending = 0,
    };

    xkb_typing_events pending = darray_new();

    எனில் (!சாவிகள்_state || !events) {
        status = XKB_ERROR_ALLOCATION_FAILURE;
        செல் error;
    }

    /*
     * Invariant: e + state.num_pending <= state.num_events
     */

    க்கு (size_t e = 0; e < இலக்கம்_events;) {
        /*
         * Process pending events
         */
        status = next_pending_events(&state, &pending);
        எனில் (status != XKB_SUCCESS) {
            செல் error;
        }
        கட்டமைப்பு xkb_typing_event *event;
        darray_foreach(event, pending) {
            எனில் (e >= இலக்கம்_events) {
                /* invariant violated: should be unreachable */
                PANIC_UNREACHABLE();
            }
            events[e] = *event;
            bit_array_assign(சாவிகள்_state, event->keycode, (bool)event->direction);
            e++;
        }

        எனில் (!state.இலக்கம்_pending && e == இலக்கம்_events - 1) {
            /*
             * Cannot add press/release *pair*, so just use an invalid keycode
             * for the last event.
             */
            events[e] = (கட்டமைப்பு xkb_typing_event) {
                .keycode = XKB_KEYCODE_INVALID,
                .direction = XKB_சாவி_DOWN,
            };
            நிறுத்து;
        }

        எனில் (e + 1 + state.இலக்கம்_pending >= இலக்கம்_events) {
            /* Cannot add new pair of events: require flushing pending events */
            தொடர்;
        }

        /*
         * Create new events
         */

        /* Choose kind of key */
        static_assert(XKB_சாவி_PRINTABLE == 0, "");
        மாறிலி எண்ணிக்கை xkb_சாவி_kind சாவி_kind =
            pick_சாவி_kind(config->prng, config->prng_state);
        தேர்வு (சாவி_kind) {
        நிலை XKB_சாவி_PRINTABLE:
            status = add_other_சாவி(
                &state, &state.சாவிகள்->printable, &e,
                XKB_TYPING_EVENT_PRINTABLE_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS
            );
            நிறுத்து;
        நிலை XKB_சாவி_MODIFIER:
            status = add_modifier_சாவி(&state, &e);
            நிறுத்து;
        நிலை XKB_சாவி_MISC:
            status = add_other_சாவி(
                &state, &state.சாவிகள்->misc, &e,
                XKB_TYPING_EVENT_MISC_IMMEDIATE_RELEASE_PERCENT,
                XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS
            );
            நிறுத்து;
        இயல்பு: {
            static_assert(XKB_சாவி_MISC == 2 &&
                          XKB_சாவி_MISC == இலக்கம்_XKB_சாவி_KIND - 1, "");
            status = XKB_ERROR_INVALID;
        }}

        எனில் (status != XKB_SUCCESS &&
            status != (எண்ணிக்கை xkb_status)XKB_ERROR_NO_RELEASED_சாவி)
        {
            செல் error;
        }
    }

    ப்ரதிஜ்ஞா(!state.இலக்கம்_pending);

    typing->events = steal(&state.events);
    typing->இலக்கம்_events = இலக்கம்_events;

    status = XKB_SUCCESS;

error:
    darray_free(pending);
    xkb_typing_state_destroy(&state);

    திரும்பு status;
}

வெறுமை
xkb_typing_உள்ளீடு_destroy(கட்டமைப்பு xkb_typing_உள்ளீடு *typing)
{
    எனில் (!typing)
        திரும்பு;

    விடுவி(typing->events);
    typing->events = வெற்று;
}
