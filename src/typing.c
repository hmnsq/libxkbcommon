/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <stdbool.h>
#ضمن <stdlib.h>
#ضمن <stdint.h>
#إذا WCHAR_MAX >= 0x7FFFFFFF
#ضمن <wctype.h>
#وصف_ڪريو USE_ISWPRINT
#إذا HAVE_NEWLOCALE && HAVE_ISWPRINT_L
#وصف_ڪريو USE_ISWPRINT_L
#ضمن <locale.h>
#جيڪڏهن_پورو
#وإلا
#ضمن <ctype.h>
#جيڪڏهن_پورو

#ضمن "xkbcommon/xkbcommon-status.h"
#ضمن "xkbcommon/xkbcommon.h"
#ضمن "darray.h"
#ضمن "state-priv.h"
#ضمن "typing.h"
#ضمن "util-bits.h"
#ضمن "util-mem.h"
#ضمن "utils.h"

تعداد xkb_حالت
xkb_ڪنجي_سيٽ_init(بنية xkb_ڪنجي_سيٽ *سيٽ,
                 بنية xkb_keymap *keymap,
                 ثابت بنية xkb_ڪنجي_سيٽ_config *config)
{
    darray_init(سيٽ->printable);
    darray_init(سيٽ->modifiers);
    darray_init(سيٽ->misc);

    ساكن ثابت بنية xkb_ڪنجي_سيٽ_config default_config = {0};
    إذا (!config)
        config = &default_config;
    /* keycode = 0 denotes “default” */
    ثابت xkb_keycode_t min = MAX(config->min, xkb_keymap_min_keycode(keymap));
    ثابت xkb_keycode_t max = config->max
        ? MIN(config->max, xkb_keymap_max_keycode(keymap))
        : xkb_keymap_max_keycode(keymap);

    تعداد xkb_حالت حالت;
    بنية xkb_state *state =
        xkb_state_new_with_گهڻائي(keymap, XKB_STATE_گهڻائي_SERVER, &حالت);
    إذا (!state)
        أرجع حالت;

    #جيڪڏهن_وصف_ٿيل USE_ISWPRINT_L
    locale_t loc = newlocale(LC_ALL_MASK, "C.UTF-8", (locale_t)0);
    #جيڪڏهن_پورو

    بنية xkb_keymap_ڪنجي_iterator iter;
    ثابت بنية xkb_keymap_ڪنجي_iterator_config iter_config = {
        .سائيز = حجم(iter_config),
        .flags = XKB_KEYMAP_ڪنجي_ITERATOR_NO_FLAGS,
        .شروعات = min
    };
    حالت = xkb_keymap_ڪنجي_iterator_init(&iter, keymap, &iter_config);
    إذا (حالت != XKB_SUCCESS)
        اذهب error;

    ثابت xkb_mod_نقاب_t numlock =
        xkb_keymap_mod_get_نقاب(keymap, XKB_VMOD_NAME_انگ);

    ثابت بنية xkb_state_components_update components_update = {
        .سائيز = حجم(components_update),
        .components = XKB_STATE_MODS_LOCKED,
        .affect_locked_mods = numlock,
        .locked_mods = numlock,
    };

    ثابت بنية xkb_synthetic_update update = {
        .سائيز = حجم(update),
        .components = &components_update,
    };

    xkb_keycode_t k;
    طالما ((k = xkb_keymap_ڪنجي_iterator_ايندڙ(&iter)) != XKB_KEYCODE_INVALID) {
        إذا (k > max)
            اكسر;

        xkb_state_reset(state);
        حالت = xkb_state_update_synthetic(state, &update, عدم);
        إذا (حالت != XKB_SUCCESS)
            اذهب error;

        ثابت xkb_keysym_t *syms = عدم;
        ثابت صحيح ڳڻپ = xkb_state_ڪنجي_get_syms(state, k, &syms);

        تعداد xkb_state_component بدليو =
            xkb_state_update_ڪنجي(state, k, XKB_ڪنجي_هيٺ_ڏانهن);
        xkb_mod_نقاب_t mods =
            xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);
        بدليو |= xkb_state_update_ڪنجي(state, k, XKB_ڪنجي_مٿي);
        mods |= xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);

        إذا (بدليو & XKB_STATE_MODS_EFFECTIVE) {
            /* Modifier key */
            تعداد xkb_modifier_قسم قسم;
            إذا (بدليو & XKB_STATE_MODS_LOCKED) {
                قسم = XKB_MOD_تالو;
            } وإلا إذا (بدليو & XKB_STATE_MODS_LATCHED) {
                قسم = XKB_MOD_LATCH;
            } وإلا {
                قسم = XKB_MOD_سيٽ;
            }
            mods &= ~numlock;
            إذا (!mods)
                mods = numlock;
            darray_append(سيٽ->modifiers, (بنية xkb_modifier_ڪنجي) {
                .keycode = k,
                .mods = mods,
                .قسم = قسم,
            });
            استمر;
        }

        إذا (ڳڻپ > 0) {
            uint32_t utf32 = 0;
            لكل (صحيح s = 0; s < ڳڻپ; s++) {
                utf32 = xkb_keysym_منزل_utf32(syms[s]);
                ثابت bool is_printable =
                    #جيڪڏهن_وصف_ٿيل USE_ISWPRINT_L
                    ((loc && iswprint_l((wint_t)utf32, loc)) || iswprint((wint_t)utf32))
                    #وإلاإذا defined(USE_ISWPRINT)
                    iswprint((wint_t)utf32)
                    #وإلا
                    utf32 <= 0x7f && isprint((صحيح)utf32)
                    #جيڪڏهن_پورو
                    ;
                إذا (is_printable)
                    اكسر;
                utf32 = 0;
            }
            إذا (utf32) {
                /* Printable keysyms */
                darray_append(سيٽ->printable, k);
                استمر;
            }
        }

        /* Miscellaneous key */
        darray_append(سيٽ->misc, k);
    }

error:
    #جيڪڏهن_وصف_ٿيل USE_ISWPRINT_L
    إذا (loc)
        freelocale(loc);
    #جيڪڏهن_پورو

    xkb_state_unref(state);
    إذا (حالت != XKB_SUCCESS)
        xkb_ڪنجي_سيٽ_destroy(سيٽ);

    أرجع حالت;
}

فراغ
xkb_ڪنجي_سيٽ_destroy(بنية xkb_ڪنجي_سيٽ *سيٽ)
{
    إذا (!سيٽ)
        أرجع;

    darray_free(سيٽ->modifiers);
    darray_free(سيٽ->printable);
    darray_free(سيٽ->misc);
}

ساكن تعداد xkb_حالت
جوڙيو_pending_events(بنية xkb_typing_state * محدود state,
                   ثابت بنية xkb_pending_typing_event * محدود events,
                   سائيز_t ڳڻپ)
{
    لكل (سائيز_t e = 0; e < ڳڻپ; e++) {
        /* Look for a free slot */
        بنية xkb_pending_typing_event *pending;
        darray_سائيز_t idx = 0;
        darray_enumerate_from(idx, pending, state->pending, idx) {
            إذا (!pending->counter) {
                darray_شي(state->pending, idx) = events[e];
                اذهب ايندڙ;
            }
        }
        /* No free slot found: append new one */
        idx = darray_سائيز(state->pending);
        darray_append(state->pending, events[e]);
    ايندڙ:
        /* Avoid immediate consumption */
        darray_شي(state->pending, idx).counter++;
    }
    state->انگ_pending += ڳڻپ;

    أرجع XKB_SUCCESS;
}

ساكن تعداد xkb_حالت
ايندڙ_pending_events(بنية xkb_typing_state * محدود state,
                    xkb_typing_events * محدود ٻاهريون)
{
    /* Reset the output */
    darray_سائيز(*ٻاهريون) = 0;

    darray_سائيز_t idx;
    بنية xkb_pending_typing_event *pending;
    /* Stack: start from the end */
    darray_enumerate_ابتو(idx, pending, state->pending) {
        إذا (!pending->counter)
            استمر;
        pending->counter--;
        إذا (pending->counter)
            استمر;
        darray_append(*ٻاهريون, pending->event);
        state->انگ_pending--;
        /* Resize array if it was the last element */
        إذا (idx + 1 == darray_سائيز(state->pending))
            darray_سائيز(state->pending)--;
        state->mods &= ~pending->mods;
    }

    أرجع XKB_SUCCESS;
}

تعداد {
    /** Tries to avoid pressing a key already pressed */
    ڪنجي_CHOICE_MAX_TRIES = 12,
    XKB_ERROR_NO_RELEASED_ڪنجي = -1000,
};

ساكن تعداد xkb_حالت
جوڙيو_modifier_ڪنجي(بنية xkb_typing_state * محدود state, سائيز_t * محدود e)
{
    /* Choose a random key */
    ڊگهو idx = -1;
    لكل (صحيح tries = 0; tries < ڪنجي_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state)
            % darray_سائيز(state->ڪنجيون->modifiers);
        /*
         * Ensure key is not already down and its modifier mask is not active,
         * i.e. to avoid simultaneous keys with the same modifier.
         */
        إذا (!ٻٽو_انگ_array_get(state->ڪنجيون_state,
                           darray_شي(state->ڪنجيون->modifiers, idx).keycode) &&
            !(state->mods & darray_شي(state->ڪنجيون->modifiers, idx).mods))
        {
            اكسر;
        }
        idx = -1;
    }

    بنية xkb_modifier_ڪنجي ڪنجي = {0};
    إذا (idx < 0) {
        /* No released key found: fall back to linear search */
        ثابت بنية xkb_modifier_ڪنجي *ڪنجي_2;
        darray_foreach(ڪنجي_2, state->ڪنجيون->modifiers) {
            /* See test doc above */
            إذا (!ٻٽو_انگ_array_get(state->ڪنجيون_state, ڪنجي_2->keycode) &&
                !(state->mods & ڪنجي_2->mods))
            {
                ڪنجي = *ڪنجي_2;
                idx = 0;
                اكسر;
            }
        }
        إذا (idx < 0) {
            /* No fallback: skip */
            أرجع (تعداد xkb_حالت)XKB_ERROR_NO_RELEASED_ڪنجي;
        }
    } وإلا {
        ڪنجي = darray_شي(state->ڪنجيون->modifiers, idx);
    }

    ثابت سائيز_t بچيل_events =
        state->انگ_events - state->انگ_pending - *e;
    سائيز_t pending_turns = 0;

    إذا (بچيل_events >= 2) {
        إذا (ڪنجي.قسم == XKB_MOD_سيٽ ||
            (ڪنجي.قسم == XKB_MOD_تالو && بچيل_events >= 4))
        {
            /* Delay SetMods release or LockMods unlock */
            ثابت سائيز_t max_turns = MIN(
                (سائيز_t)(
                    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS -
                    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                 ),
                بچيل_events + state->انگ_pending
            );
            pending_turns = XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                          + (سائيز_t)state->prng(state->prng_state)
                          % (max_turns + 1);
        }
    } وإلا {
        أرجع XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (بنية xkb_typing_event) {
        .keycode = ڪنجي.keycode,
        .direction = XKB_ڪنجي_هيٺ_ڏانهن,
    };

    بنية xkb_pending_typing_event pending[2] = {
        {
            .event = {
                .keycode = ڪنجي.keycode,
                .direction = XKB_ڪنجي_هيٺ_ڏانهن,
            },
            .counter = (uint8_t)pending_turns,
        },
        {
            .event = {
                .keycode = ڪنجي.keycode,
                .direction = XKB_ڪنجي_مٿي,
            },
            .mods = ڪنجي.mods,
            .counter = (uint8_t)pending_turns,
        },
    };

    إذا (pending_turns) {
        إذا (ڪنجي.قسم == XKB_MOD_تالو) {
            /* Immediate release */
            state->events[(*e)++] = pending[1].event;
            /* Delayed unlock (press + release) */
            pending[1].counter++;
            ثابت تعداد xkb_حالت حالت =
                جوڙيو_pending_events(state, pending, ARRAY_سائيز(pending));
            إذا (حالت != XKB_SUCCESS)
                أرجع حالت;
            /*
             * Register “locked” state, to avoid tapping the key before
             * delayed key released
             */
            ٻٽو_انگ_array_سيٽ(state->ڪنجيون_state, ڪنجي.keycode);
        } وإلا {
            /* Delayed release */
            ثابت تعداد xkb_حالت حالت =
                جوڙيو_pending_events(state, &pending[1], 1);
            إذا (حالت != XKB_SUCCESS)
                أرجع حالت;
            /* Register pressed state */
            ٻٽو_انگ_array_سيٽ(state->ڪنجيون_state, ڪنجي.keycode);
        }
        state->mods |= ڪنجي.mods;
    } وإلا {
        /* Immediate release */
        state->events[(*e)++] = pending[1].event;
        /* TODO: add latched modifier to mask state. */
    }

    أرجع XKB_SUCCESS;
}

ساكن تعداد xkb_حالت
جوڙيو_other_ڪنجي(بنية xkb_typing_state * محدود state,
              ثابت xkb_keycodes_t * محدود keycodes, سائيز_t * محدود e,
              uint8_t فوري_release_prob, uint8_t max_pending_turns)
{
    /* Choose a random key */
    ڊگهو idx = -1;
    لكل (صحيح tries = 0; tries < ڪنجي_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state) % darray_سائيز(*keycodes);
        /* Ensure key is not already down */
        إذا (!ٻٽو_انگ_array_get(state->ڪنجيون_state,
                           darray_شي(*keycodes, idx)))
        {
            اكسر;
        }
        idx = -1;
    }

    xkb_keycode_t keycode = XKB_KEYCODE_INVALID;
    إذا (idx < 0) {
        /* No released key found: fall back to linear search */
        ثابت xkb_keycode_t *keycode_;
        darray_foreach(keycode_, *keycodes) {
            إذا (!ٻٽو_انگ_array_get(state->ڪنجيون_state, *keycode_)) {
                keycode = *keycode_;
                idx = 0;
                اكسر;
            }
        }
        إذا (idx < 0) {
            /* No fallback: skip */
            أرجع (تعداد xkb_حالت)XKB_ERROR_NO_RELEASED_ڪنجي;
        }
    } وإلا {
        keycode = darray_شي(*keycodes, idx);
    }

    ثابت سائيز_t بچيل_events =
        state->انگ_events - state->انگ_pending - *e;
    سائيز_t pending_turns;

    إذا (بچيل_events >= 2) {
        ثابت سائيز_t max_turns = MIN(
            (سائيز_t)max_pending_turns,
            بچيل_events + state->انگ_pending
        );
        ثابت bool فوري_release = (
            (فوري_release_prob >= 100) ||
            !max_turns ||
            (state->prng(state->prng_state) % 100) < فوري_release_prob
        );
        إذا (فوري_release) {
            pending_turns = 0;
        } وإلا {
            pending_turns = 1 + (سائيز_t)state->prng(state->prng_state)
                          % max_turns;
        }
    } وإلا {
        أرجع XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (بنية xkb_typing_event) {
        .keycode = keycode,
        .direction = XKB_ڪنجي_هيٺ_ڏانهن,
    };

    ثابت بنية xkb_pending_typing_event pending = {
        .event = {
            .keycode = keycode,
            .direction = XKB_ڪنجي_مٿي,
        },
        .counter = (uint8_t)pending_turns,
    };
    إذا (pending_turns) {
        ثابت تعداد xkb_حالت حالت = جوڙيو_pending_events(state, &pending, 1);
        إذا (حالت != XKB_SUCCESS)
            أرجع حالت;
        /* Register pressed state */
        ٻٽو_انگ_array_سيٽ(state->ڪنجيون_state, keycode);
    } وإلا {
        state->events[(*e)++] = pending.event;
    }

    أرجع XKB_SUCCESS;
}

ساكن تعداد xkb_ڪنجي_قسم
pick_ڪنجي_قسم(xkb_prng_t prng, فراغ *prng_state)
{
    ساكن ثابت uint8_t weights[انگ_XKB_ڪنجي_قسم] = {
        [XKB_ڪنجي_PRINTABLE] = 60,
        [XKB_ڪنجي_MODIFIER] = 30,
        [XKB_ڪنجي_MISC] = 10,
    };
    static_assert(XKB_ڪنجي_MISC == 2 &&
                  XKB_ڪنجي_MISC == انگ_XKB_ڪنجي_قسم - 1, "");
    أكد(weights[0] + weights[1] + weights[2] == 100);

    ڊگهو r = prng(prng_state) % 100;
    لكل (تعداد xkb_ڪنجي_قسم k = 0; k < انگ_XKB_ڪنجي_قسم; k++)
        إذا ((r -= weights[k]) < 0)
            أرجع k;
    أرجع XKB_ڪنجي_PRINTABLE;
}

ساكن فراغ
xkb_typing_state_destroy(بنية xkb_typing_state *state)
{
    حرر(state->ڪنجيون_state);
    darray_free(state->pending);
    حرر(state->events);
}

تعداد xkb_حالت
xkb_typing_داخلا_init(بنية xkb_typing_داخلا * محدود typing,
                      ثابت بنية xkb_ڪنجي_سيٽ * محدود ڪنجيون,
                      ثابت بنية xkb_typing_داخلا_config * محدود config)
{
    typing->events = عدم;
    typing->انگ_events = 0;

    إذا (!config->داخلا_ڊيگهائي)
        أرجع XKB_ERROR_INVALID;

    إذا (darray_خالي_جايون(ڪنجيون->printable) ||
        darray_خالي_جايون(ڪنجيون->modifiers) ||
        darray_خالي_جايون(ڪنجيون->misc))
    {
        أرجع XKB_ERROR_INVALID;
    }

    تعداد xkb_حالت حالت = XKB_SUCCESS;

    /* NOTE: arrays are sorted because they were built using the key iterator */
    xkb_keycode_t keycode_max = darray_آخري(ڪنجيون->printable, 0);
    keycode_max = MAX(
        darray_آخري(ڪنجيون->modifiers, (بنية xkb_modifier_ڪنجي){0}).keycode,
        keycode_max
    );
    keycode_max = MAX(darray_آخري(ڪنجيون->misc, 0), keycode_max);
    ثابت سائيز_t ڪنجيون_state_انگ = ٻٽو_انگ_array_لفظ((سائيز_t)keycode_max + 1);
    uintptr_t *ڪنجيون_state = صفر_يادگيري_ڏيو(ڪنجيون_state_انگ, حجم(*ڪنجيون_state));

    ثابت سائيز_t انگ_events = config->داخلا_ڊيگهائي;
    بنية xkb_typing_event *events = صفر_يادگيري_ڏيو(انگ_events, حجم(*events));

    بنية xkb_typing_state state = {
        .ڪنجيون = ڪنجيون,
        .ڪنجيون_state = ڪنجيون_state,
        .prng = config->prng,
        .prng_state = config->prng_state,
        .events = events,
        .انگ_events = انگ_events,
        .pending = darray_new(),
        .انگ_pending = 0,
    };

    xkb_typing_events pending = darray_new();

    إذا (!ڪنجيون_state || !events) {
        حالت = XKB_ERROR_ورهاست_FAILURE;
        اذهب error;
    }

    /*
     * Invariant: e + state.num_pending <= state.num_events
     */

    لكل (سائيز_t e = 0; e < انگ_events;) {
        /*
         * Process pending events
         */
        حالت = ايندڙ_pending_events(&state, &pending);
        إذا (حالت != XKB_SUCCESS) {
            اذهب error;
        }
        بنية xkb_typing_event *event;
        darray_foreach(event, pending) {
            إذا (e >= انگ_events) {
                /* invariant violated: should be unreachable */
                PANIC_UNREACHABLE();
            }
            events[e] = *event;
            ٻٽو_انگ_array_سونپيو(ڪنجيون_state, event->keycode, (bool)event->direction);
            e++;
        }

        إذا (!state.انگ_pending && e == انگ_events - 1) {
            /*
             * Cannot add press/release *pair*, so just use an invalid keycode
             * for the last event.
             */
            events[e] = (بنية xkb_typing_event) {
                .keycode = XKB_KEYCODE_INVALID,
                .direction = XKB_ڪنجي_هيٺ_ڏانهن,
            };
            اكسر;
        }

        إذا (e + 1 + state.انگ_pending >= انگ_events) {
            /* Cannot add new pair of events: require flushing pending events */
            استمر;
        }

        /*
         * Create new events
         */

        /* Choose kind of key */
        static_assert(XKB_ڪنجي_PRINTABLE == 0, "");
        ثابت تعداد xkb_ڪنجي_قسم ڪنجي_قسم =
            pick_ڪنجي_قسم(config->prng, config->prng_state);
        بدل (ڪنجي_قسم) {
        حالة XKB_ڪنجي_PRINTABLE:
            حالت = جوڙيو_other_ڪنجي(
                &state, &state.ڪنجيون->printable, &e,
                XKB_TYPING_EVENT_PRINTABLE_فوري_RELEASE_PERCENT,
                XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS
            );
            اكسر;
        حالة XKB_ڪنجي_MODIFIER:
            حالت = جوڙيو_modifier_ڪنجي(&state, &e);
            اكسر;
        حالة XKB_ڪنجي_MISC:
            حالت = جوڙيو_other_ڪنجي(
                &state, &state.ڪنجيون->misc, &e,
                XKB_TYPING_EVENT_MISC_فوري_RELEASE_PERCENT,
                XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS
            );
            اكسر;
        افتراضي: {
            static_assert(XKB_ڪنجي_MISC == 2 &&
                          XKB_ڪنجي_MISC == انگ_XKB_ڪنجي_قسم - 1, "");
            حالت = XKB_ERROR_INVALID;
        }}

        إذا (حالت != XKB_SUCCESS &&
            حالت != (تعداد xkb_حالت)XKB_ERROR_NO_RELEASED_ڪنجي)
        {
            اذهب error;
        }
    }

    أكد(!state.انگ_pending);

    typing->events = steal(&state.events);
    typing->انگ_events = انگ_events;

    حالت = XKB_SUCCESS;

error:
    darray_free(pending);
    xkb_typing_state_destroy(&state);

    أرجع حالت;
}

فراغ
xkb_typing_داخلا_destroy(بنية xkb_typing_داخلا *typing)
{
    إذا (!typing)
        أرجع;

    حرر(typing->events);
    typing->events = عدم;
}
