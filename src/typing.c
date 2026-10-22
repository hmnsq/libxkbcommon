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
#تعریف_کن USE_ISWPRINT
#إذا HAVE_NEWLOCALE && HAVE_ISWPRINT_L
#تعریف_کن USE_ISWPRINT_L
#ضمن <locale.h>
#پایان_اگر
#وإلا
#ضمن <ctype.h>
#پایان_اگر

#ضمن "xkbcommon/xkbcommon-status.h"
#ضمن "xkbcommon/xkbcommon.h"
#ضمن "darray.h"
#ضمن "state-priv.h"
#ضمن "typing.h"
#ضمن "util-bits.h"
#ضمن "util-mem.h"
#ضمن "utils.h"

تعداد xkb_وضعیت
xkb_کلید_مجموعه_init(بنية xkb_کلید_مجموعه *مجموعه,
                 بنية xkb_keymap *keymap,
                 ثابت بنية xkb_کلید_مجموعه_config *config)
{
    darray_init(مجموعه->printable);
    darray_init(مجموعه->modifiers);
    darray_init(مجموعه->misc);

    ساكن ثابت بنية xkb_کلید_مجموعه_config default_config = {0};
    إذا (!config)
        config = &default_config;
    /* keycode = 0 denotes “default” */
    ثابت xkb_keycode_t min = MAX(config->min, xkb_keymap_min_keycode(keymap));
    ثابت xkb_keycode_t max = config->max
        ? MIN(config->max, xkb_keymap_max_keycode(keymap))
        : xkb_keymap_max_keycode(keymap);

    تعداد xkb_وضعیت وضعیت;
    بنية xkb_state *state =
        xkb_state_new_with_نما(keymap, XKB_STATE_نما_SERVER, &وضعیت);
    إذا (!state)
        أرجع وضعیت;

    #اگر_تعریف_شده USE_ISWPRINT_L
    locale_t loc = newlocale(LC_ALL_MASK, "C.UTF-8", (locale_t)0);
    #پایان_اگر

    بنية xkb_keymap_کلید_iterator iter;
    ثابت بنية xkb_keymap_کلید_iterator_config iter_config = {
        .اندازه = حجم(iter_config),
        .flags = XKB_KEYMAP_کلید_ITERATOR_NO_FLAGS,
        .شروع = min
    };
    وضعیت = xkb_keymap_کلید_iterator_init(&iter, keymap, &iter_config);
    إذا (وضعیت != XKB_SUCCESS)
        اذهب error;

    ثابت xkb_mod_ماسک_t numlock =
        xkb_keymap_mod_get_ماسک(keymap, XKB_VMOD_NAME_رقم);

    ثابت بنية xkb_state_components_update components_update = {
        .اندازه = حجم(components_update),
        .components = XKB_STATE_MODS_LOCKED,
        .affect_locked_mods = numlock,
        .locked_mods = numlock,
    };

    ثابت بنية xkb_synthetic_update update = {
        .اندازه = حجم(update),
        .components = &components_update,
    };

    xkb_keycode_t k;
    طالما ((k = xkb_keymap_کلید_iterator_بعدی(&iter)) != XKB_KEYCODE_INVALID) {
        إذا (k > max)
            اكسر;

        xkb_state_reset(state);
        وضعیت = xkb_state_update_synthetic(state, &update, عدم);
        إذا (وضعیت != XKB_SUCCESS)
            اذهب error;

        ثابت xkb_keysym_t *syms = عدم;
        ثابت صحيح شمار = xkb_state_کلید_get_syms(state, k, &syms);

        تعداد xkb_state_component تغییریافته =
            xkb_state_update_کلید(state, k, XKB_کلید_پایین);
        xkb_mod_ماسک_t mods =
            xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);
        تغییریافته |= xkb_state_update_کلید(state, k, XKB_کلید_به_بالا);
        mods |= xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);

        إذا (تغییریافته & XKB_STATE_MODS_EFFECTIVE) {
            /* Modifier key */
            تعداد xkb_modifier_نوع نوع;
            إذا (تغییریافته & XKB_STATE_MODS_LOCKED) {
                نوع = XKB_MOD_قفل;
            } وإلا إذا (تغییریافته & XKB_STATE_MODS_LATCHED) {
                نوع = XKB_MOD_LATCH;
            } وإلا {
                نوع = XKB_MOD_مجموعه;
            }
            mods &= ~numlock;
            إذا (!mods)
                mods = numlock;
            darray_append(مجموعه->modifiers, (بنية xkb_modifier_کلید) {
                .keycode = k,
                .mods = mods,
                .نوع = نوع,
            });
            استمر;
        }

        إذا (شمار > 0) {
            uint32_t utf32 = 0;
            لكل (صحيح s = 0; s < شمار; s++) {
                utf32 = xkb_keysym_مقصد_utf32(syms[s]);
                ثابت bool is_printable =
                    #اگر_تعریف_شده USE_ISWPRINT_L
                    ((loc && iswprint_l((wint_t)utf32, loc)) || iswprint((wint_t)utf32))
                    #وإلاإذا defined(USE_ISWPRINT)
                    iswprint((wint_t)utf32)
                    #وإلا
                    utf32 <= 0x7f && isprint((صحيح)utf32)
                    #پایان_اگر
                    ;
                إذا (is_printable)
                    اكسر;
                utf32 = 0;
            }
            إذا (utf32) {
                /* Printable keysyms */
                darray_append(مجموعه->printable, k);
                استمر;
            }
        }

        /* Miscellaneous key */
        darray_append(مجموعه->misc, k);
    }

error:
    #اگر_تعریف_شده USE_ISWPRINT_L
    إذا (loc)
        freelocale(loc);
    #پایان_اگر

    xkb_state_unref(state);
    إذا (وضعیت != XKB_SUCCESS)
        xkb_کلید_مجموعه_destroy(مجموعه);

    أرجع وضعیت;
}

فراغ
xkb_کلید_مجموعه_destroy(بنية xkb_کلید_مجموعه *مجموعه)
{
    إذا (!مجموعه)
        أرجع;

    darray_free(مجموعه->modifiers);
    darray_free(مجموعه->printable);
    darray_free(مجموعه->misc);
}

ساكن تعداد xkb_وضعیت
جمع_کن_pending_events(بنية xkb_typing_state * محدود state,
                   ثابت بنية xkb_pending_typing_event * محدود events,
                   اندازه_t شمار)
{
    لكل (اندازه_t e = 0; e < شمار; e++) {
        /* Look for a free slot */
        بنية xkb_pending_typing_event *pending;
        darray_اندازه_t idx = 0;
        darray_enumerate_from(idx, pending, state->pending, idx) {
            إذا (!pending->counter) {
                darray_قلم(state->pending, idx) = events[e];
                اذهب بعدی;
            }
        }
        /* No free slot found: append new one */
        idx = darray_اندازه(state->pending);
        darray_append(state->pending, events[e]);
    بعدی:
        /* Avoid immediate consumption */
        darray_قلم(state->pending, idx).counter++;
    }
    state->رقم_pending += شمار;

    أرجع XKB_SUCCESS;
}

ساكن تعداد xkb_وضعیت
بعدی_pending_events(بنية xkb_typing_state * محدود state,
                    xkb_typing_events * محدود بیرون)
{
    /* Reset the output */
    darray_اندازه(*بیرون) = 0;

    darray_اندازه_t idx;
    بنية xkb_pending_typing_event *pending;
    /* Stack: start from the end */
    darray_enumerate_معکوس(idx, pending, state->pending) {
        إذا (!pending->counter)
            استمر;
        pending->counter--;
        إذا (pending->counter)
            استمر;
        darray_append(*بیرون, pending->event);
        state->رقم_pending--;
        /* Resize array if it was the last element */
        إذا (idx + 1 == darray_اندازه(state->pending))
            darray_اندازه(state->pending)--;
        state->mods &= ~pending->mods;
    }

    أرجع XKB_SUCCESS;
}

تعداد {
    /** Tries to avoid pressing a key already pressed */
    کلید_CHOICE_MAX_TRIES = 12,
    XKB_ERROR_NO_RELEASED_کلید = -1000,
};

ساكن تعداد xkb_وضعیت
جمع_کن_modifier_کلید(بنية xkb_typing_state * محدود state, اندازه_t * محدود e)
{
    /* Choose a random key */
    دراز idx = -1;
    لكل (صحيح tries = 0; tries < کلید_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state)
            % darray_اندازه(state->کلیدها->modifiers);
        /*
         * Ensure key is not already down and its modifier mask is not active,
         * i.e. to avoid simultaneous keys with the same modifier.
         */
        إذا (!بیت_array_get(state->کلیدها_state,
                           darray_قلم(state->کلیدها->modifiers, idx).keycode) &&
            !(state->mods & darray_قلم(state->کلیدها->modifiers, idx).mods))
        {
            اكسر;
        }
        idx = -1;
    }

    بنية xkb_modifier_کلید کلید = {0};
    إذا (idx < 0) {
        /* No released key found: fall back to linear search */
        ثابت بنية xkb_modifier_کلید *کلید_2;
        darray_foreach(کلید_2, state->کلیدها->modifiers) {
            /* See test doc above */
            إذا (!بیت_array_get(state->کلیدها_state, کلید_2->keycode) &&
                !(state->mods & کلید_2->mods))
            {
                کلید = *کلید_2;
                idx = 0;
                اكسر;
            }
        }
        إذا (idx < 0) {
            /* No fallback: skip */
            أرجع (تعداد xkb_وضعیت)XKB_ERROR_NO_RELEASED_کلید;
        }
    } وإلا {
        کلید = darray_قلم(state->کلیدها->modifiers, idx);
    }

    ثابت اندازه_t مانده_events =
        state->رقم_events - state->رقم_pending - *e;
    اندازه_t pending_turns = 0;

    إذا (مانده_events >= 2) {
        إذا (کلید.نوع == XKB_MOD_مجموعه ||
            (کلید.نوع == XKB_MOD_قفل && مانده_events >= 4))
        {
            /* Delay SetMods release or LockMods unlock */
            ثابت اندازه_t max_turns = MIN(
                (اندازه_t)(
                    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS -
                    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                 ),
                مانده_events + state->رقم_pending
            );
            pending_turns = XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                          + (اندازه_t)state->prng(state->prng_state)
                          % (max_turns + 1);
        }
    } وإلا {
        أرجع XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (بنية xkb_typing_event) {
        .keycode = کلید.keycode,
        .direction = XKB_کلید_پایین,
    };

    بنية xkb_pending_typing_event pending[2] = {
        {
            .event = {
                .keycode = کلید.keycode,
                .direction = XKB_کلید_پایین,
            },
            .counter = (uint8_t)pending_turns,
        },
        {
            .event = {
                .keycode = کلید.keycode,
                .direction = XKB_کلید_به_بالا,
            },
            .mods = کلید.mods,
            .counter = (uint8_t)pending_turns,
        },
    };

    إذا (pending_turns) {
        إذا (کلید.نوع == XKB_MOD_قفل) {
            /* Immediate release */
            state->events[(*e)++] = pending[1].event;
            /* Delayed unlock (press + release) */
            pending[1].counter++;
            ثابت تعداد xkb_وضعیت وضعیت =
                جمع_کن_pending_events(state, pending, ARRAY_اندازه(pending));
            إذا (وضعیت != XKB_SUCCESS)
                أرجع وضعیت;
            /*
             * Register “locked” state, to avoid tapping the key before
             * delayed key released
             */
            بیت_array_مجموعه(state->کلیدها_state, کلید.keycode);
        } وإلا {
            /* Delayed release */
            ثابت تعداد xkb_وضعیت وضعیت =
                جمع_کن_pending_events(state, &pending[1], 1);
            إذا (وضعیت != XKB_SUCCESS)
                أرجع وضعیت;
            /* Register pressed state */
            بیت_array_مجموعه(state->کلیدها_state, کلید.keycode);
        }
        state->mods |= کلید.mods;
    } وإلا {
        /* Immediate release */
        state->events[(*e)++] = pending[1].event;
        /* TODO: add latched modifier to mask state. */
    }

    أرجع XKB_SUCCESS;
}

ساكن تعداد xkb_وضعیت
جمع_کن_other_کلید(بنية xkb_typing_state * محدود state,
              ثابت xkb_keycodes_t * محدود keycodes, اندازه_t * محدود e,
              uint8_t فوری_release_prob, uint8_t max_pending_turns)
{
    /* Choose a random key */
    دراز idx = -1;
    لكل (صحيح tries = 0; tries < کلید_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state) % darray_اندازه(*keycodes);
        /* Ensure key is not already down */
        إذا (!بیت_array_get(state->کلیدها_state,
                           darray_قلم(*keycodes, idx)))
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
            إذا (!بیت_array_get(state->کلیدها_state, *keycode_)) {
                keycode = *keycode_;
                idx = 0;
                اكسر;
            }
        }
        إذا (idx < 0) {
            /* No fallback: skip */
            أرجع (تعداد xkb_وضعیت)XKB_ERROR_NO_RELEASED_کلید;
        }
    } وإلا {
        keycode = darray_قلم(*keycodes, idx);
    }

    ثابت اندازه_t مانده_events =
        state->رقم_events - state->رقم_pending - *e;
    اندازه_t pending_turns;

    إذا (مانده_events >= 2) {
        ثابت اندازه_t max_turns = MIN(
            (اندازه_t)max_pending_turns,
            مانده_events + state->رقم_pending
        );
        ثابت bool فوری_release = (
            (فوری_release_prob >= 100) ||
            !max_turns ||
            (state->prng(state->prng_state) % 100) < فوری_release_prob
        );
        إذا (فوری_release) {
            pending_turns = 0;
        } وإلا {
            pending_turns = 1 + (اندازه_t)state->prng(state->prng_state)
                          % max_turns;
        }
    } وإلا {
        أرجع XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (بنية xkb_typing_event) {
        .keycode = keycode,
        .direction = XKB_کلید_پایین,
    };

    ثابت بنية xkb_pending_typing_event pending = {
        .event = {
            .keycode = keycode,
            .direction = XKB_کلید_به_بالا,
        },
        .counter = (uint8_t)pending_turns,
    };
    إذا (pending_turns) {
        ثابت تعداد xkb_وضعیت وضعیت = جمع_کن_pending_events(state, &pending, 1);
        إذا (وضعیت != XKB_SUCCESS)
            أرجع وضعیت;
        /* Register pressed state */
        بیت_array_مجموعه(state->کلیدها_state, keycode);
    } وإلا {
        state->events[(*e)++] = pending.event;
    }

    أرجع XKB_SUCCESS;
}

ساكن تعداد xkb_کلید_نوع
pick_کلید_نوع(xkb_prng_t prng, فراغ *prng_state)
{
    ساكن ثابت uint8_t weights[رقم_XKB_کلید_نوع] = {
        [XKB_کلید_PRINTABLE] = 60,
        [XKB_کلید_MODIFIER] = 30,
        [XKB_کلید_MISC] = 10,
    };
    static_assert(XKB_کلید_MISC == 2 &&
                  XKB_کلید_MISC == رقم_XKB_کلید_نوع - 1, "");
    أكد(weights[0] + weights[1] + weights[2] == 100);

    دراز r = prng(prng_state) % 100;
    لكل (تعداد xkb_کلید_نوع k = 0; k < رقم_XKB_کلید_نوع; k++)
        إذا ((r -= weights[k]) < 0)
            أرجع k;
    أرجع XKB_کلید_PRINTABLE;
}

ساكن فراغ
xkb_typing_state_destroy(بنية xkb_typing_state *state)
{
    حرر(state->کلیدها_state);
    darray_free(state->pending);
    حرر(state->events);
}

تعداد xkb_وضعیت
xkb_typing_درون_داد_init(بنية xkb_typing_درون_داد * محدود typing,
                      ثابت بنية xkb_کلید_مجموعه * محدود کلیدها,
                      ثابت بنية xkb_typing_درون_داد_config * محدود config)
{
    typing->events = عدم;
    typing->رقم_events = 0;

    إذا (!config->درون_داد_درازی)
        أرجع XKB_ERROR_INVALID;

    إذا (darray_خالی(کلیدها->printable) ||
        darray_خالی(کلیدها->modifiers) ||
        darray_خالی(کلیدها->misc))
    {
        أرجع XKB_ERROR_INVALID;
    }

    تعداد xkb_وضعیت وضعیت = XKB_SUCCESS;

    /* NOTE: arrays are sorted because they were built using the key iterator */
    xkb_keycode_t keycode_max = darray_آخرین(کلیدها->printable, 0);
    keycode_max = MAX(
        darray_آخرین(کلیدها->modifiers, (بنية xkb_modifier_کلید){0}).keycode,
        keycode_max
    );
    keycode_max = MAX(darray_آخرین(کلیدها->misc, 0), keycode_max);
    ثابت اندازه_t کلیدها_state_رقم = بیت_array_واژگان((اندازه_t)keycode_max + 1);
    uintptr_t *کلیدها_state = حافظه_صفر_بده(کلیدها_state_رقم, حجم(*کلیدها_state));

    ثابت اندازه_t رقم_events = config->درون_داد_درازی;
    بنية xkb_typing_event *events = حافظه_صفر_بده(رقم_events, حجم(*events));

    بنية xkb_typing_state state = {
        .کلیدها = کلیدها,
        .کلیدها_state = کلیدها_state,
        .prng = config->prng,
        .prng_state = config->prng_state,
        .events = events,
        .رقم_events = رقم_events,
        .pending = darray_new(),
        .رقم_pending = 0,
    };

    xkb_typing_events pending = darray_new();

    إذا (!کلیدها_state || !events) {
        وضعیت = XKB_ERROR_تخصیص_FAILURE;
        اذهب error;
    }

    /*
     * Invariant: e + state.num_pending <= state.num_events
     */

    لكل (اندازه_t e = 0; e < رقم_events;) {
        /*
         * Process pending events
         */
        وضعیت = بعدی_pending_events(&state, &pending);
        إذا (وضعیت != XKB_SUCCESS) {
            اذهب error;
        }
        بنية xkb_typing_event *event;
        darray_foreach(event, pending) {
            إذا (e >= رقم_events) {
                /* invariant violated: should be unreachable */
                PANIC_UNREACHABLE();
            }
            events[e] = *event;
            بیت_array_تخصیص_بده(کلیدها_state, event->keycode, (bool)event->direction);
            e++;
        }

        إذا (!state.رقم_pending && e == رقم_events - 1) {
            /*
             * Cannot add press/release *pair*, so just use an invalid keycode
             * for the last event.
             */
            events[e] = (بنية xkb_typing_event) {
                .keycode = XKB_KEYCODE_INVALID,
                .direction = XKB_کلید_پایین,
            };
            اكسر;
        }

        إذا (e + 1 + state.رقم_pending >= رقم_events) {
            /* Cannot add new pair of events: require flushing pending events */
            استمر;
        }

        /*
         * Create new events
         */

        /* Choose kind of key */
        static_assert(XKB_کلید_PRINTABLE == 0, "");
        ثابت تعداد xkb_کلید_نوع کلید_نوع =
            pick_کلید_نوع(config->prng, config->prng_state);
        بدل (کلید_نوع) {
        حالة XKB_کلید_PRINTABLE:
            وضعیت = جمع_کن_other_کلید(
                &state, &state.کلیدها->printable, &e,
                XKB_TYPING_EVENT_PRINTABLE_فوری_RELEASE_PERCENT,
                XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS
            );
            اكسر;
        حالة XKB_کلید_MODIFIER:
            وضعیت = جمع_کن_modifier_کلید(&state, &e);
            اكسر;
        حالة XKB_کلید_MISC:
            وضعیت = جمع_کن_other_کلید(
                &state, &state.کلیدها->misc, &e,
                XKB_TYPING_EVENT_MISC_فوری_RELEASE_PERCENT,
                XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS
            );
            اكسر;
        افتراضي: {
            static_assert(XKB_کلید_MISC == 2 &&
                          XKB_کلید_MISC == رقم_XKB_کلید_نوع - 1, "");
            وضعیت = XKB_ERROR_INVALID;
        }}

        إذا (وضعیت != XKB_SUCCESS &&
            وضعیت != (تعداد xkb_وضعیت)XKB_ERROR_NO_RELEASED_کلید)
        {
            اذهب error;
        }
    }

    أكد(!state.رقم_pending);

    typing->events = steal(&state.events);
    typing->رقم_events = رقم_events;

    وضعیت = XKB_SUCCESS;

error:
    darray_free(pending);
    xkb_typing_state_destroy(&state);

    أرجع وضعیت;
}

فراغ
xkb_typing_درون_داد_destroy(بنية xkb_typing_درون_داد *typing)
{
    إذا (!typing)
        أرجع;

    حرر(typing->events);
    typing->events = عدم;
}
