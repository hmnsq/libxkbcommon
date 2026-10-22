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
#تعریف_کرو USE_ISWPRINT
#إذا HAVE_NEWLOCALE && HAVE_ISWPRINT_L
#تعریف_کرو USE_ISWPRINT_L
#ضمن <locale.h>
#جے_انت
#وإلا
#ضمن <ctype.h>
#جے_انت

#ضمن "xkbcommon/xkbcommon-status.h"
#ضمن "xkbcommon/xkbcommon.h"
#ضمن "darray.h"
#ضمن "state-priv.h"
#ضمن "typing.h"
#ضمن "util-bits.h"
#ضمن "util-mem.h"
#ضمن "utils.h"

تعداد xkb_حالت
xkb_کلید_سیٹ_init(بنية xkb_کلید_سیٹ *سیٹ,
                 بنية xkb_keymap *keymap,
                 ثابت بنية xkb_کلید_سیٹ_config *config)
{
    darray_init(سیٹ->printable);
    darray_init(سیٹ->modifiers);
    darray_init(سیٹ->misc);

    ساكن ثابت بنية xkb_کلید_سیٹ_config default_config = {0};
    إذا (!config)
        config = &default_config;
    /* keycode = 0 denotes “default” */
    ثابت xkb_keycode_t min = MAX(config->min, xkb_keymap_min_keycode(keymap));
    ثابت xkb_keycode_t max = config->max
        ? MIN(config->max, xkb_keymap_max_keycode(keymap))
        : xkb_keymap_max_keycode(keymap);

    تعداد xkb_حالت حالت;
    بنية xkb_state *state =
        xkb_state_new_with_منوال(keymap, XKB_STATE_منوال_SERVER, &حالت);
    إذا (!state)
        أرجع حالت;

    #جے_تعریف_شدہ USE_ISWPRINT_L
    locale_t loc = newlocale(LC_ALL_MASK, "C.UTF-8", (locale_t)0);
    #جے_انت

    بنية xkb_keymap_کلید_iterator iter;
    ثابت بنية xkb_keymap_کلید_iterator_config iter_config = {
        .ناپ = حجم(iter_config),
        .flags = XKB_KEYMAP_کلید_ITERATOR_NO_FLAGS,
        .شروع = min
    };
    حالت = xkb_keymap_کلید_iterator_init(&iter, keymap, &iter_config);
    إذا (حالت != XKB_SUCCESS)
        اذهب error;

    ثابت xkb_mod_نقاب_t numlock =
        xkb_keymap_mod_get_نقاب(keymap, XKB_VMOD_NAME_عدد);

    ثابت بنية xkb_state_components_update components_update = {
        .ناپ = حجم(components_update),
        .components = XKB_STATE_MODS_LOCKED,
        .affect_locked_mods = numlock,
        .locked_mods = numlock,
    };

    ثابت بنية xkb_synthetic_update update = {
        .ناپ = حجم(update),
        .components = &components_update,
    };

    xkb_keycode_t k;
    طالما ((k = xkb_keymap_کلید_iterator_اگلا(&iter)) != XKB_KEYCODE_INVALID) {
        إذا (k > max)
            اكسر;

        xkb_state_reset(state);
        حالت = xkb_state_update_synthetic(state, &update, عدم);
        إذا (حالت != XKB_SUCCESS)
            اذهب error;

        ثابت xkb_keysym_t *syms = عدم;
        ثابت صحيح گنتی = xkb_state_کلید_get_syms(state, k, &syms);

        تعداد xkb_state_component بدلیا =
            xkb_state_update_کلید(state, k, XKB_کلید_تھلے);
        xkb_mod_نقاب_t mods =
            xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);
        بدلیا |= xkb_state_update_کلید(state, k, XKB_کلید_اتے);
        mods |= xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);

        إذا (بدلیا & XKB_STATE_MODS_EFFECTIVE) {
            /* Modifier key */
            تعداد xkb_modifier_قسم قسم;
            إذا (بدلیا & XKB_STATE_MODS_LOCKED) {
                قسم = XKB_MOD_جندرا;
            } وإلا إذا (بدلیا & XKB_STATE_MODS_LATCHED) {
                قسم = XKB_MOD_LATCH;
            } وإلا {
                قسم = XKB_MOD_سیٹ;
            }
            mods &= ~numlock;
            إذا (!mods)
                mods = numlock;
            darray_append(سیٹ->modifiers, (بنية xkb_modifier_کلید) {
                .keycode = k,
                .mods = mods,
                .قسم = قسم,
            });
            استمر;
        }

        إذا (گنتی > 0) {
            uint32_t utf32 = 0;
            لكل (صحيح s = 0; s < گنتی; s++) {
                utf32 = xkb_keysym_منزل_utf32(syms[s]);
                ثابت bool is_printable =
                    #جے_تعریف_شدہ USE_ISWPRINT_L
                    ((loc && iswprint_l((wint_t)utf32, loc)) || iswprint((wint_t)utf32))
                    #وإلاإذا defined(USE_ISWPRINT)
                    iswprint((wint_t)utf32)
                    #وإلا
                    utf32 <= 0x7f && isprint((صحيح)utf32)
                    #جے_انت
                    ;
                إذا (is_printable)
                    اكسر;
                utf32 = 0;
            }
            إذا (utf32) {
                /* Printable keysyms */
                darray_append(سیٹ->printable, k);
                استمر;
            }
        }

        /* Miscellaneous key */
        darray_append(سیٹ->misc, k);
    }

error:
    #جے_تعریف_شدہ USE_ISWPRINT_L
    إذا (loc)
        freelocale(loc);
    #جے_انت

    xkb_state_unref(state);
    إذا (حالت != XKB_SUCCESS)
        xkb_کلید_سیٹ_destroy(سیٹ);

    أرجع حالت;
}

فراغ
xkb_کلید_سیٹ_destroy(بنية xkb_کلید_سیٹ *سیٹ)
{
    إذا (!سیٹ)
        أرجع;

    darray_free(سیٹ->modifiers);
    darray_free(سیٹ->printable);
    darray_free(سیٹ->misc);
}

ساكن تعداد xkb_حالت
جوڑو_pending_events(بنية xkb_typing_state * محدود state,
                   ثابت بنية xkb_pending_typing_event * محدود events,
                   ناپ_t گنتی)
{
    لكل (ناپ_t e = 0; e < گنتی; e++) {
        /* Look for a free slot */
        بنية xkb_pending_typing_event *pending;
        darray_ناپ_t idx = 0;
        darray_enumerate_from(idx, pending, state->pending, idx) {
            إذا (!pending->counter) {
                darray_شے(state->pending, idx) = events[e];
                اذهب اگلا;
            }
        }
        /* No free slot found: append new one */
        idx = darray_ناپ(state->pending);
        darray_append(state->pending, events[e]);
    اگلا:
        /* Avoid immediate consumption */
        darray_شے(state->pending, idx).counter++;
    }
    state->عدد_pending += گنتی;

    أرجع XKB_SUCCESS;
}

ساكن تعداد xkb_حالت
اگلا_pending_events(بنية xkb_typing_state * محدود state,
                    xkb_typing_events * محدود باہرلا)
{
    /* Reset the output */
    darray_ناپ(*باہرلا) = 0;

    darray_ناپ_t idx;
    بنية xkb_pending_typing_event *pending;
    /* Stack: start from the end */
    darray_enumerate_الٹ(idx, pending, state->pending) {
        إذا (!pending->counter)
            استمر;
        pending->counter--;
        إذا (pending->counter)
            استمر;
        darray_append(*باہرلا, pending->event);
        state->عدد_pending--;
        /* Resize array if it was the last element */
        إذا (idx + 1 == darray_ناپ(state->pending))
            darray_ناپ(state->pending)--;
        state->mods &= ~pending->mods;
    }

    أرجع XKB_SUCCESS;
}

تعداد {
    /** Tries to avoid pressing a key already pressed */
    کلید_CHOICE_MAX_TRIES = 12,
    XKB_ERROR_NO_RELEASED_کلید = -1000,
};

ساكن تعداد xkb_حالت
جوڑو_modifier_کلید(بنية xkb_typing_state * محدود state, ناپ_t * محدود e)
{
    /* Choose a random key */
    لما idx = -1;
    لكل (صحيح tries = 0; tries < کلید_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state)
            % darray_ناپ(state->کلیدیں->modifiers);
        /*
         * Ensure key is not already down and its modifier mask is not active,
         * i.e. to avoid simultaneous keys with the same modifier.
         */
        إذا (!ثنائی_ہندسہ_array_get(state->کلیدیں_state,
                           darray_شے(state->کلیدیں->modifiers, idx).keycode) &&
            !(state->mods & darray_شے(state->کلیدیں->modifiers, idx).mods))
        {
            اكسر;
        }
        idx = -1;
    }

    بنية xkb_modifier_کلید کلید = {0};
    إذا (idx < 0) {
        /* No released key found: fall back to linear search */
        ثابت بنية xkb_modifier_کلید *کلید_2;
        darray_foreach(کلید_2, state->کلیدیں->modifiers) {
            /* See test doc above */
            إذا (!ثنائی_ہندسہ_array_get(state->کلیدیں_state, کلید_2->keycode) &&
                !(state->mods & کلید_2->mods))
            {
                کلید = *کلید_2;
                idx = 0;
                اكسر;
            }
        }
        إذا (idx < 0) {
            /* No fallback: skip */
            أرجع (تعداد xkb_حالت)XKB_ERROR_NO_RELEASED_کلید;
        }
    } وإلا {
        کلید = darray_شے(state->کلیدیں->modifiers, idx);
    }

    ثابت ناپ_t بچیا_events =
        state->عدد_events - state->عدد_pending - *e;
    ناپ_t pending_turns = 0;

    إذا (بچیا_events >= 2) {
        إذا (کلید.قسم == XKB_MOD_سیٹ ||
            (کلید.قسم == XKB_MOD_جندرا && بچیا_events >= 4))
        {
            /* Delay SetMods release or LockMods unlock */
            ثابت ناپ_t max_turns = MIN(
                (ناپ_t)(
                    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS -
                    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                 ),
                بچیا_events + state->عدد_pending
            );
            pending_turns = XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                          + (ناپ_t)state->prng(state->prng_state)
                          % (max_turns + 1);
        }
    } وإلا {
        أرجع XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (بنية xkb_typing_event) {
        .keycode = کلید.keycode,
        .direction = XKB_کلید_تھلے,
    };

    بنية xkb_pending_typing_event pending[2] = {
        {
            .event = {
                .keycode = کلید.keycode,
                .direction = XKB_کلید_تھلے,
            },
            .counter = (uint8_t)pending_turns,
        },
        {
            .event = {
                .keycode = کلید.keycode,
                .direction = XKB_کلید_اتے,
            },
            .mods = کلید.mods,
            .counter = (uint8_t)pending_turns,
        },
    };

    إذا (pending_turns) {
        إذا (کلید.قسم == XKB_MOD_جندرا) {
            /* Immediate release */
            state->events[(*e)++] = pending[1].event;
            /* Delayed unlock (press + release) */
            pending[1].counter++;
            ثابت تعداد xkb_حالت حالت =
                جوڑو_pending_events(state, pending, ARRAY_ناپ(pending));
            إذا (حالت != XKB_SUCCESS)
                أرجع حالت;
            /*
             * Register “locked” state, to avoid tapping the key before
             * delayed key released
             */
            ثنائی_ہندسہ_array_سیٹ(state->کلیدیں_state, کلید.keycode);
        } وإلا {
            /* Delayed release */
            ثابت تعداد xkb_حالت حالت =
                جوڑو_pending_events(state, &pending[1], 1);
            إذا (حالت != XKB_SUCCESS)
                أرجع حالت;
            /* Register pressed state */
            ثنائی_ہندسہ_array_سیٹ(state->کلیدیں_state, کلید.keycode);
        }
        state->mods |= کلید.mods;
    } وإلا {
        /* Immediate release */
        state->events[(*e)++] = pending[1].event;
        /* TODO: add latched modifier to mask state. */
    }

    أرجع XKB_SUCCESS;
}

ساكن تعداد xkb_حالت
جوڑو_other_کلید(بنية xkb_typing_state * محدود state,
              ثابت xkb_keycodes_t * محدود keycodes, ناپ_t * محدود e,
              uint8_t فوری_release_prob, uint8_t max_pending_turns)
{
    /* Choose a random key */
    لما idx = -1;
    لكل (صحيح tries = 0; tries < کلید_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state) % darray_ناپ(*keycodes);
        /* Ensure key is not already down */
        إذا (!ثنائی_ہندسہ_array_get(state->کلیدیں_state,
                           darray_شے(*keycodes, idx)))
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
            إذا (!ثنائی_ہندسہ_array_get(state->کلیدیں_state, *keycode_)) {
                keycode = *keycode_;
                idx = 0;
                اكسر;
            }
        }
        إذا (idx < 0) {
            /* No fallback: skip */
            أرجع (تعداد xkb_حالت)XKB_ERROR_NO_RELEASED_کلید;
        }
    } وإلا {
        keycode = darray_شے(*keycodes, idx);
    }

    ثابت ناپ_t بچیا_events =
        state->عدد_events - state->عدد_pending - *e;
    ناپ_t pending_turns;

    إذا (بچیا_events >= 2) {
        ثابت ناپ_t max_turns = MIN(
            (ناپ_t)max_pending_turns,
            بچیا_events + state->عدد_pending
        );
        ثابت bool فوری_release = (
            (فوری_release_prob >= 100) ||
            !max_turns ||
            (state->prng(state->prng_state) % 100) < فوری_release_prob
        );
        إذا (فوری_release) {
            pending_turns = 0;
        } وإلا {
            pending_turns = 1 + (ناپ_t)state->prng(state->prng_state)
                          % max_turns;
        }
    } وإلا {
        أرجع XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (بنية xkb_typing_event) {
        .keycode = keycode,
        .direction = XKB_کلید_تھلے,
    };

    ثابت بنية xkb_pending_typing_event pending = {
        .event = {
            .keycode = keycode,
            .direction = XKB_کلید_اتے,
        },
        .counter = (uint8_t)pending_turns,
    };
    إذا (pending_turns) {
        ثابت تعداد xkb_حالت حالت = جوڑو_pending_events(state, &pending, 1);
        إذا (حالت != XKB_SUCCESS)
            أرجع حالت;
        /* Register pressed state */
        ثنائی_ہندسہ_array_سیٹ(state->کلیدیں_state, keycode);
    } وإلا {
        state->events[(*e)++] = pending.event;
    }

    أرجع XKB_SUCCESS;
}

ساكن تعداد xkb_کلید_قسم
pick_کلید_قسم(xkb_prng_t prng, فراغ *prng_state)
{
    ساكن ثابت uint8_t weights[عدد_XKB_کلید_قسم] = {
        [XKB_کلید_PRINTABLE] = 60,
        [XKB_کلید_MODIFIER] = 30,
        [XKB_کلید_MISC] = 10,
    };
    static_assert(XKB_کلید_MISC == 2 &&
                  XKB_کلید_MISC == عدد_XKB_کلید_قسم - 1, "");
    أكد(weights[0] + weights[1] + weights[2] == 100);

    لما r = prng(prng_state) % 100;
    لكل (تعداد xkb_کلید_قسم k = 0; k < عدد_XKB_کلید_قسم; k++)
        إذا ((r -= weights[k]) < 0)
            أرجع k;
    أرجع XKB_کلید_PRINTABLE;
}

ساكن فراغ
xkb_typing_state_destroy(بنية xkb_typing_state *state)
{
    حرر(state->کلیدیں_state);
    darray_free(state->pending);
    حرر(state->events);
}

تعداد xkb_حالت
xkb_typing_داخلہ_init(بنية xkb_typing_داخلہ * محدود typing,
                      ثابت بنية xkb_کلید_سیٹ * محدود کلیدیں,
                      ثابت بنية xkb_typing_داخلہ_config * محدود config)
{
    typing->events = عدم;
    typing->عدد_events = 0;

    إذا (!config->داخلہ_لمائی)
        أرجع XKB_ERROR_INVALID;

    إذا (darray_خالی_تھاواں(کلیدیں->printable) ||
        darray_خالی_تھاواں(کلیدیں->modifiers) ||
        darray_خالی_تھاواں(کلیدیں->misc))
    {
        أرجع XKB_ERROR_INVALID;
    }

    تعداد xkb_حالت حالت = XKB_SUCCESS;

    /* NOTE: arrays are sorted because they were built using the key iterator */
    xkb_keycode_t keycode_max = darray_آخری(کلیدیں->printable, 0);
    keycode_max = MAX(
        darray_آخری(کلیدیں->modifiers, (بنية xkb_modifier_کلید){0}).keycode,
        keycode_max
    );
    keycode_max = MAX(darray_آخری(کلیدیں->misc, 0), keycode_max);
    ثابت ناپ_t کلیدیں_state_عدد = ثنائی_ہندسہ_array_الفاظ((ناپ_t)keycode_max + 1);
    uintptr_t *کلیدیں_state = صفر_حافظہ_دیو(کلیدیں_state_عدد, حجم(*کلیدیں_state));

    ثابت ناپ_t عدد_events = config->داخلہ_لمائی;
    بنية xkb_typing_event *events = صفر_حافظہ_دیو(عدد_events, حجم(*events));

    بنية xkb_typing_state state = {
        .کلیدیں = کلیدیں,
        .کلیدیں_state = کلیدیں_state,
        .prng = config->prng,
        .prng_state = config->prng_state,
        .events = events,
        .عدد_events = عدد_events,
        .pending = darray_new(),
        .عدد_pending = 0,
    };

    xkb_typing_events pending = darray_new();

    إذا (!کلیدیں_state || !events) {
        حالت = XKB_ERROR_ونڈ_FAILURE;
        اذهب error;
    }

    /*
     * Invariant: e + state.num_pending <= state.num_events
     */

    لكل (ناپ_t e = 0; e < عدد_events;) {
        /*
         * Process pending events
         */
        حالت = اگلا_pending_events(&state, &pending);
        إذا (حالت != XKB_SUCCESS) {
            اذهب error;
        }
        بنية xkb_typing_event *event;
        darray_foreach(event, pending) {
            إذا (e >= عدد_events) {
                /* invariant violated: should be unreachable */
                PANIC_UNREACHABLE();
            }
            events[e] = *event;
            ثنائی_ہندسہ_array_سونپو(کلیدیں_state, event->keycode, (bool)event->direction);
            e++;
        }

        إذا (!state.عدد_pending && e == عدد_events - 1) {
            /*
             * Cannot add press/release *pair*, so just use an invalid keycode
             * for the last event.
             */
            events[e] = (بنية xkb_typing_event) {
                .keycode = XKB_KEYCODE_INVALID,
                .direction = XKB_کلید_تھلے,
            };
            اكسر;
        }

        إذا (e + 1 + state.عدد_pending >= عدد_events) {
            /* Cannot add new pair of events: require flushing pending events */
            استمر;
        }

        /*
         * Create new events
         */

        /* Choose kind of key */
        static_assert(XKB_کلید_PRINTABLE == 0, "");
        ثابت تعداد xkb_کلید_قسم کلید_قسم =
            pick_کلید_قسم(config->prng, config->prng_state);
        بدل (کلید_قسم) {
        حالة XKB_کلید_PRINTABLE:
            حالت = جوڑو_other_کلید(
                &state, &state.کلیدیں->printable, &e,
                XKB_TYPING_EVENT_PRINTABLE_فوری_RELEASE_PERCENT,
                XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS
            );
            اكسر;
        حالة XKB_کلید_MODIFIER:
            حالت = جوڑو_modifier_کلید(&state, &e);
            اكسر;
        حالة XKB_کلید_MISC:
            حالت = جوڑو_other_کلید(
                &state, &state.کلیدیں->misc, &e,
                XKB_TYPING_EVENT_MISC_فوری_RELEASE_PERCENT,
                XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS
            );
            اكسر;
        افتراضي: {
            static_assert(XKB_کلید_MISC == 2 &&
                          XKB_کلید_MISC == عدد_XKB_کلید_قسم - 1, "");
            حالت = XKB_ERROR_INVALID;
        }}

        إذا (حالت != XKB_SUCCESS &&
            حالت != (تعداد xkb_حالت)XKB_ERROR_NO_RELEASED_کلید)
        {
            اذهب error;
        }
    }

    أكد(!state.عدد_pending);

    typing->events = steal(&state.events);
    typing->عدد_events = عدد_events;

    حالت = XKB_SUCCESS;

error:
    darray_free(pending);
    xkb_typing_state_destroy(&state);

    أرجع حالت;
}

فراغ
xkb_typing_داخلہ_destroy(بنية xkb_typing_داخلہ *typing)
{
    إذا (!typing)
        أرجع;

    حرر(typing->events);
    typing->events = عدم;
}
