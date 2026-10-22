/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل <assert.h>
#شامل <stdbool.h>
#شامل <stdlib.h>
#شامل <stdint.h>
#اگر WCHAR_MAX >= 0x7FFFFFFF
#شامل <wctype.h>
#تعریف_کرو USE_ISWPRINT
#اگر HAVE_NEWLOCALE && HAVE_ISWPRINT_L
#تعریف_کرو USE_ISWPRINT_L
#شامل <locale.h>
#اگر_ختم
#ورنہ
#شامل <ctype.h>
#اگر_ختم

#شامل "xkbcommon/xkbcommon-status.h"
#شامل "xkbcommon/xkbcommon.h"
#شامل "darray.h"
#شامل "state-priv.h"
#شامل "typing.h"
#شامل "util-bits.h"
#شامل "util-mem.h"
#شامل "utils.h"

شمار xkb_حالت
xkb_کلید_سیٹ_init(ساخت xkb_کلید_سیٹ *سیٹ,
                 ساخت xkb_keymap *keymap,
                 ثابت ساخت xkb_کلید_سیٹ_config *config)
{
    darray_init(سیٹ->printable);
    darray_init(سیٹ->modifiers);
    darray_init(سیٹ->misc);

    ساکن ثابت ساخت xkb_کلید_سیٹ_config default_config = {0};
    اگر (!config)
        config = &default_config;
    /* keycode = 0 denotes “default” */
    ثابت xkb_keycode_t min = MAX(config->min, xkb_keymap_min_keycode(keymap));
    ثابت xkb_keycode_t max = config->max
        ? MIN(config->max, xkb_keymap_max_keycode(keymap))
        : xkb_keymap_max_keycode(keymap);

    شمار xkb_حالت حالت_2;
    ساخت xkb_state *state =
        xkb_state_new_with_منوال(keymap, XKB_STATE_منوال_SERVER, &حالت_2);
    اگر (!state)
        واپس حالت_2;

    #اگر_تعریف_شدہ USE_ISWPRINT_L
    locale_t loc = newlocale(LC_ALL_MASK, "C.UTF-8", (locale_t)0);
    #اگر_ختم

    ساخت xkb_keymap_کلید_iterator iter;
    ثابت ساخت xkb_keymap_کلید_iterator_config iter_config = {
        .سائز = حجم(iter_config),
        .flags = XKB_KEYMAP_کلید_ITERATOR_NO_FLAGS,
        .آغاز = min
    };
    حالت_2 = xkb_keymap_کلید_iterator_init(&iter, keymap, &iter_config);
    اگر (حالت_2 != XKB_SUCCESS)
        جاؤ error;

    ثابت xkb_mod_نقاب_t numlock =
        xkb_keymap_mod_get_نقاب(keymap, XKB_VMOD_NAME_عدد);

    ثابت ساخت xkb_state_components_update components_update = {
        .سائز = حجم(components_update),
        .components = XKB_STATE_MODS_LOCKED,
        .affect_locked_mods = numlock,
        .locked_mods = numlock,
    };

    ثابت ساخت xkb_synthetic_update update = {
        .سائز = حجم(update),
        .components = &components_update,
    };

    xkb_keycode_t k;
    جب ((k = xkb_keymap_کلید_iterator_اگلا(&iter)) != XKB_KEYCODE_INVALID) {
        اگر (k > max)
            توڑو;

        xkb_state_reset(state);
        حالت_2 = xkb_state_update_synthetic(state, &update, تہی);
        اگر (حالت_2 != XKB_SUCCESS)
            جاؤ error;

        ثابت xkb_keysym_t *syms = تہی;
        ثابت صحیح گنتی = xkb_state_کلید_get_syms(state, k, &syms);

        شمار xkb_state_component بدلا =
            xkb_state_update_کلید(state, k, XKB_کلید_نیچے_کو);
        xkb_mod_نقاب_t mods =
            xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);
        بدلا |= xkb_state_update_کلید(state, k, XKB_کلید_اوپر);
        mods |= xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);

        اگر (بدلا & XKB_STATE_MODS_EFFECTIVE) {
            /* Modifier key */
            شمار xkb_modifier_قسم قسم;
            اگر (بدلا & XKB_STATE_MODS_LOCKED) {
                قسم = XKB_MOD_تالا;
            } ورنہ اگر (بدلا & XKB_STATE_MODS_LATCHED) {
                قسم = XKB_MOD_LATCH;
            } ورنہ {
                قسم = XKB_MOD_سیٹ;
            }
            mods &= ~numlock;
            اگر (!mods)
                mods = numlock;
            darray_append(سیٹ->modifiers, (ساخت xkb_modifier_کلید) {
                .keycode = k,
                .mods = mods,
                .قسم = قسم,
            });
            جاری;
        }

        اگر (گنتی > 0) {
            uint32_t utf32 = 0;
            برائے (صحیح s = 0; s < گنتی; s++) {
                utf32 = xkb_keysym_مقصد_utf32(syms[s]);
                ثابت bool is_printable =
                    #اگر_تعریف_شدہ USE_ISWPRINT_L
                    ((loc && iswprint_l((wint_t)utf32, loc)) || iswprint((wint_t)utf32))
                    #وگرنہ_اگر defined(USE_ISWPRINT)
                    iswprint((wint_t)utf32)
                    #ورنہ
                    utf32 <= 0x7f && isprint((صحیح)utf32)
                    #اگر_ختم
                    ;
                اگر (is_printable)
                    توڑو;
                utf32 = 0;
            }
            اگر (utf32) {
                /* Printable keysyms */
                darray_append(سیٹ->printable, k);
                جاری;
            }
        }

        /* Miscellaneous key */
        darray_append(سیٹ->misc, k);
    }

error:
    #اگر_تعریف_شدہ USE_ISWPRINT_L
    اگر (loc)
        freelocale(loc);
    #اگر_ختم

    xkb_state_unref(state);
    اگر (حالت_2 != XKB_SUCCESS)
        xkb_کلید_سیٹ_destroy(سیٹ);

    واپس حالت_2;
}

خالی
xkb_کلید_سیٹ_destroy(ساخت xkb_کلید_سیٹ *سیٹ)
{
    اگر (!سیٹ)
        واپس;

    darray_free(سیٹ->modifiers);
    darray_free(سیٹ->printable);
    darray_free(سیٹ->misc);
}

ساکن شمار xkb_حالت
جوڑو_pending_events(ساخت xkb_typing_state * محدود state,
                   ثابت ساخت xkb_pending_typing_event * محدود events,
                   سائز_t گنتی)
{
    برائے (سائز_t e = 0; e < گنتی; e++) {
        /* Look for a free slot */
        ساخت xkb_pending_typing_event *pending;
        darray_سائز_t idx = 0;
        darray_enumerate_from(idx, pending, state->pending, idx) {
            اگر (!pending->شمار_2) {
                darray_شے(state->pending, idx) = events[e];
                جاؤ اگلا;
            }
        }
        /* No free slot found: append new one */
        idx = darray_سائز(state->pending);
        darray_append(state->pending, events[e]);
    اگلا:
        /* Avoid immediate consumption */
        darray_شے(state->pending, idx).شمار_2++;
    }
    state->عدد_pending += گنتی;

    واپس XKB_SUCCESS;
}

ساکن شمار xkb_حالت
اگلا_pending_events(ساخت xkb_typing_state * محدود state,
                    xkb_typing_events * محدود برآمد)
{
    /* Reset the output */
    darray_سائز(*برآمد) = 0;

    darray_سائز_t idx;
    ساخت xkb_pending_typing_event *pending;
    /* Stack: start from the end */
    darray_enumerate_الٹ(idx, pending, state->pending) {
        اگر (!pending->شمار_2)
            جاری;
        pending->شمار_2--;
        اگر (pending->شمار_2)
            جاری;
        darray_append(*برآمد, pending->event);
        state->عدد_pending--;
        /* Resize array if it was the last element */
        اگر (idx + 1 == darray_سائز(state->pending))
            darray_سائز(state->pending)--;
        state->mods &= ~pending->mods;
    }

    واپس XKB_SUCCESS;
}

شمار {
    /** Tries to avoid pressing a key already pressed */
    کلید_CHOICE_MAX_TRIES = 12,
    XKB_ERROR_NO_RELEASED_کلید = -1000,
};

ساکن شمار xkb_حالت
جوڑو_modifier_کلید(ساخت xkb_typing_state * محدود state, سائز_t * محدود e)
{
    /* Choose a random key */
    طویل idx = -1;
    برائے (صحیح tries = 0; tries < کلید_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state)
            % darray_سائز(state->کلیدیں->modifiers);
        /*
         * Ensure key is not already down and its modifier mask is not active,
         * i.e. to avoid simultaneous keys with the same modifier.
         */
        اگر (!ثنائی_ہندسہ_array_get(state->کلیدیں_state,
                           darray_شے(state->کلیدیں->modifiers, idx).keycode) &&
            !(state->mods & darray_شے(state->کلیدیں->modifiers, idx).mods))
        {
            توڑو;
        }
        idx = -1;
    }

    ساخت xkb_modifier_کلید کلید = {0};
    اگر (idx < 0) {
        /* No released key found: fall back to linear search */
        ثابت ساخت xkb_modifier_کلید *کلید_2;
        darray_foreach(کلید_2, state->کلیدیں->modifiers) {
            /* See test doc above */
            اگر (!ثنائی_ہندسہ_array_get(state->کلیدیں_state, کلید_2->keycode) &&
                !(state->mods & کلید_2->mods))
            {
                کلید = *کلید_2;
                idx = 0;
                توڑو;
            }
        }
        اگر (idx < 0) {
            /* No fallback: skip */
            واپس (شمار xkb_حالت)XKB_ERROR_NO_RELEASED_کلید;
        }
    } ورنہ {
        کلید = darray_شے(state->کلیدیں->modifiers, idx);
    }

    ثابت سائز_t بقایا_events =
        state->عدد_events - state->عدد_pending - *e;
    سائز_t pending_turns = 0;

    اگر (بقایا_events >= 2) {
        اگر (کلید.قسم == XKB_MOD_سیٹ ||
            (کلید.قسم == XKB_MOD_تالا && بقایا_events >= 4))
        {
            /* Delay SetMods release or LockMods unlock */
            ثابت سائز_t max_turns = MIN(
                (سائز_t)(
                    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS -
                    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                 ),
                بقایا_events + state->عدد_pending
            );
            pending_turns = XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                          + (سائز_t)state->prng(state->prng_state)
                          % (max_turns + 1);
        }
    } ورنہ {
        واپس XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (ساخت xkb_typing_event) {
        .keycode = کلید.keycode,
        .direction = XKB_کلید_نیچے_کو,
    };

    ساخت xkb_pending_typing_event pending[2] = {
        {
            .event = {
                .keycode = کلید.keycode,
                .direction = XKB_کلید_نیچے_کو,
            },
            .شمار_2 = (uint8_t)pending_turns,
        },
        {
            .event = {
                .keycode = کلید.keycode,
                .direction = XKB_کلید_اوپر,
            },
            .mods = کلید.mods,
            .شمار_2 = (uint8_t)pending_turns,
        },
    };

    اگر (pending_turns) {
        اگر (کلید.قسم == XKB_MOD_تالا) {
            /* Immediate release */
            state->events[(*e)++] = pending[1].event;
            /* Delayed unlock (press + release) */
            pending[1].شمار_2++;
            ثابت شمار xkb_حالت حالت_2 =
                جوڑو_pending_events(state, pending, ARRAY_سائز(pending));
            اگر (حالت_2 != XKB_SUCCESS)
                واپس حالت_2;
            /*
             * Register “locked” state, to avoid tapping the key before
             * delayed key released
             */
            ثنائی_ہندسہ_array_سیٹ(state->کلیدیں_state, کلید.keycode);
        } ورنہ {
            /* Delayed release */
            ثابت شمار xkb_حالت حالت_2 =
                جوڑو_pending_events(state, &pending[1], 1);
            اگر (حالت_2 != XKB_SUCCESS)
                واپس حالت_2;
            /* Register pressed state */
            ثنائی_ہندسہ_array_سیٹ(state->کلیدیں_state, کلید.keycode);
        }
        state->mods |= کلید.mods;
    } ورنہ {
        /* Immediate release */
        state->events[(*e)++] = pending[1].event;
        /* TODO: add latched modifier to mask state. */
    }

    واپس XKB_SUCCESS;
}

ساکن شمار xkb_حالت
جوڑو_other_کلید(ساخت xkb_typing_state * محدود state,
              ثابت xkb_keycodes_t * محدود keycodes, سائز_t * محدود e,
              uint8_t فوری_release_prob, uint8_t max_pending_turns)
{
    /* Choose a random key */
    طویل idx = -1;
    برائے (صحیح tries = 0; tries < کلید_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state) % darray_سائز(*keycodes);
        /* Ensure key is not already down */
        اگر (!ثنائی_ہندسہ_array_get(state->کلیدیں_state,
                           darray_شے(*keycodes, idx)))
        {
            توڑو;
        }
        idx = -1;
    }

    xkb_keycode_t keycode = XKB_KEYCODE_INVALID;
    اگر (idx < 0) {
        /* No released key found: fall back to linear search */
        ثابت xkb_keycode_t *keycode_;
        darray_foreach(keycode_, *keycodes) {
            اگر (!ثنائی_ہندسہ_array_get(state->کلیدیں_state, *keycode_)) {
                keycode = *keycode_;
                idx = 0;
                توڑو;
            }
        }
        اگر (idx < 0) {
            /* No fallback: skip */
            واپس (شمار xkb_حالت)XKB_ERROR_NO_RELEASED_کلید;
        }
    } ورنہ {
        keycode = darray_شے(*keycodes, idx);
    }

    ثابت سائز_t بقایا_events =
        state->عدد_events - state->عدد_pending - *e;
    سائز_t pending_turns;

    اگر (بقایا_events >= 2) {
        ثابت سائز_t max_turns = MIN(
            (سائز_t)max_pending_turns,
            بقایا_events + state->عدد_pending
        );
        ثابت bool فوری_release = (
            (فوری_release_prob >= 100) ||
            !max_turns ||
            (state->prng(state->prng_state) % 100) < فوری_release_prob
        );
        اگر (فوری_release) {
            pending_turns = 0;
        } ورنہ {
            pending_turns = 1 + (سائز_t)state->prng(state->prng_state)
                          % max_turns;
        }
    } ورنہ {
        واپس XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (ساخت xkb_typing_event) {
        .keycode = keycode,
        .direction = XKB_کلید_نیچے_کو,
    };

    ثابت ساخت xkb_pending_typing_event pending = {
        .event = {
            .keycode = keycode,
            .direction = XKB_کلید_اوپر,
        },
        .شمار_2 = (uint8_t)pending_turns,
    };
    اگر (pending_turns) {
        ثابت شمار xkb_حالت حالت_2 = جوڑو_pending_events(state, &pending, 1);
        اگر (حالت_2 != XKB_SUCCESS)
            واپس حالت_2;
        /* Register pressed state */
        ثنائی_ہندسہ_array_سیٹ(state->کلیدیں_state, keycode);
    } ورنہ {
        state->events[(*e)++] = pending.event;
    }

    واپس XKB_SUCCESS;
}

ساکن شمار xkb_کلید_قسم
pick_کلید_قسم(xkb_prng_t prng, خالی *prng_state)
{
    ساکن ثابت uint8_t weights[عدد_XKB_کلید_قسم] = {
        [XKB_کلید_PRINTABLE] = 60,
        [XKB_کلید_MODIFIER] = 30,
        [XKB_کلید_MISC] = 10,
    };
    static_assert(XKB_کلید_MISC == 2 &&
                  XKB_کلید_MISC == عدد_XKB_کلید_قسم - 1, "");
    أكد(weights[0] + weights[1] + weights[2] == 100);

    طویل r = prng(prng_state) % 100;
    برائے (شمار xkb_کلید_قسم k = 0; k < عدد_XKB_کلید_قسم; k++)
        اگر ((r -= weights[k]) < 0)
            واپس k;
    واپس XKB_کلید_PRINTABLE;
}

ساکن خالی
xkb_typing_state_destroy(ساخت xkb_typing_state *state)
{
    آزاد(state->کلیدیں_state);
    darray_free(state->pending);
    آزاد(state->events);
}

شمار xkb_حالت
xkb_typing_داخلہ_init(ساخت xkb_typing_داخلہ * محدود typing,
                      ثابت ساخت xkb_کلید_سیٹ * محدود کلیدیں,
                      ثابت ساخت xkb_typing_داخلہ_config * محدود config)
{
    typing->events = تہی;
    typing->عدد_events = 0;

    اگر (!config->داخلہ_لمبائی)
        واپس XKB_ERROR_INVALID;

    اگر (darray_خالی_خانے(کلیدیں->printable) ||
        darray_خالی_خانے(کلیدیں->modifiers) ||
        darray_خالی_خانے(کلیدیں->misc))
    {
        واپس XKB_ERROR_INVALID;
    }

    شمار xkb_حالت حالت_2 = XKB_SUCCESS;

    /* NOTE: arrays are sorted because they were built using the key iterator */
    xkb_keycode_t keycode_max = darray_آخری(کلیدیں->printable, 0);
    keycode_max = MAX(
        darray_آخری(کلیدیں->modifiers, (ساخت xkb_modifier_کلید){0}).keycode,
        keycode_max
    );
    keycode_max = MAX(darray_آخری(کلیدیں->misc, 0), keycode_max);
    ثابت سائز_t کلیدیں_state_عدد = ثنائی_ہندسہ_array_الفاظ((سائز_t)keycode_max + 1);
    uintptr_t *کلیدیں_state = صفر_حافظہ_دو(کلیدیں_state_عدد, حجم(*کلیدیں_state));

    ثابت سائز_t عدد_events = config->داخلہ_لمبائی;
    ساخت xkb_typing_event *events = صفر_حافظہ_دو(عدد_events, حجم(*events));

    ساخت xkb_typing_state state = {
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

    اگر (!کلیدیں_state || !events) {
        حالت_2 = XKB_ERROR_تخصیص_FAILURE;
        جاؤ error;
    }

    /*
     * Invariant: e + state.num_pending <= state.num_events
     */

    برائے (سائز_t e = 0; e < عدد_events;) {
        /*
         * Process pending events
         */
        حالت_2 = اگلا_pending_events(&state, &pending);
        اگر (حالت_2 != XKB_SUCCESS) {
            جاؤ error;
        }
        ساخت xkb_typing_event *event;
        darray_foreach(event, pending) {
            اگر (e >= عدد_events) {
                /* invariant violated: should be unreachable */
                PANIC_UNREACHABLE();
            }
            events[e] = *event;
            ثنائی_ہندسہ_array_تفویض(کلیدیں_state, event->keycode, (bool)event->direction);
            e++;
        }

        اگر (!state.عدد_pending && e == عدد_events - 1) {
            /*
             * Cannot add press/release *pair*, so just use an invalid keycode
             * for the last event.
             */
            events[e] = (ساخت xkb_typing_event) {
                .keycode = XKB_KEYCODE_INVALID,
                .direction = XKB_کلید_نیچے_کو,
            };
            توڑو;
        }

        اگر (e + 1 + state.عدد_pending >= عدد_events) {
            /* Cannot add new pair of events: require flushing pending events */
            جاری;
        }

        /*
         * Create new events
         */

        /* Choose kind of key */
        static_assert(XKB_کلید_PRINTABLE == 0, "");
        ثابت شمار xkb_کلید_قسم کلید_قسم =
            pick_کلید_قسم(config->prng, config->prng_state);
        انتخاب (کلید_قسم) {
        حالت XKB_کلید_PRINTABLE:
            حالت_2 = جوڑو_other_کلید(
                &state, &state.کلیدیں->printable, &e,
                XKB_TYPING_EVENT_PRINTABLE_فوری_RELEASE_PERCENT,
                XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS
            );
            توڑو;
        حالت XKB_کلید_MODIFIER:
            حالت_2 = جوڑو_modifier_کلید(&state, &e);
            توڑو;
        حالت XKB_کلید_MISC:
            حالت_2 = جوڑو_other_کلید(
                &state, &state.کلیدیں->misc, &e,
                XKB_TYPING_EVENT_MISC_فوری_RELEASE_PERCENT,
                XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS
            );
            توڑو;
        طےشدہ: {
            static_assert(XKB_کلید_MISC == 2 &&
                          XKB_کلید_MISC == عدد_XKB_کلید_قسم - 1, "");
            حالت_2 = XKB_ERROR_INVALID;
        }}

        اگر (حالت_2 != XKB_SUCCESS &&
            حالت_2 != (شمار xkb_حالت)XKB_ERROR_NO_RELEASED_کلید)
        {
            جاؤ error;
        }
    }

    أكد(!state.عدد_pending);

    typing->events = steal(&state.events);
    typing->عدد_events = عدد_events;

    حالت_2 = XKB_SUCCESS;

error:
    darray_free(pending);
    xkb_typing_state_destroy(&state);

    واپس حالت_2;
}

خالی
xkb_typing_داخلہ_destroy(ساخت xkb_typing_داخلہ *typing)
{
    اگر (!typing)
        واپس;

    آزاد(typing->events);
    typing->events = تہی;
}
