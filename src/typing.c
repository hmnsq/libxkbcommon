/*
 * Copyright © 2026 Pierre Le Marre <dev@wismill.eu>
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdint.h>
#यदि WCHAR_MAX >= 0x7FFFFFFF
#अन्तर्भाव <wctype.h>
#परिभाषय USE_ISWPRINT
#यदि HAVE_NEWLOCALE && HAVE_ISWPRINT_L
#परिभाषय USE_ISWPRINT_L
#अन्तर्भाव <locale.h>
#यदि_अन्तः
#अन्यथा
#अन्तर्भाव <ctype.h>
#यदि_अन्तः

#अन्तर्भाव "xkbcommon/xkbcommon-status.h"
#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "state-priv.h"
#अन्तर्भाव "typing.h"
#अन्तर्भाव "util-bits.h"
#अन्तर्भाव "util-mem.h"
#अन्तर्भाव "utils.h"

गणना xkb_स्थितिः
xkb_कुञ्चिका_समुच्चयः_init(संरचना xkb_कुञ्चिका_समुच्चयः *समुच्चयः,
                 संरचना xkb_keymap *keymap,
                 ध्रुव संरचना xkb_कुञ्चिका_समुच्चयः_config *config)
{
    darray_init(समुच्चयः->printable);
    darray_init(समुच्चयः->modifiers);
    darray_init(समुच्चयः->misc);

    स्थिर ध्रुव संरचना xkb_कुञ्चिका_समुच्चयः_config default_config = {0};
    यदि (!config)
        config = &default_config;
    /* keycode = 0 denotes “default” */
    ध्रुव xkb_keycode_t min = MAX(config->min, xkb_keymap_min_keycode(keymap));
    ध्रुव xkb_keycode_t max = config->max
        ? MIN(config->max, xkb_keymap_max_keycode(keymap))
        : xkb_keymap_max_keycode(keymap);

    गणना xkb_स्थितिः स्थितिः;
    संरचना xkb_state *state =
        xkb_state_new_with_बहुलकम्(keymap, XKB_STATE_बहुलकम्_SERVER, &स्थितिः);
    यदि (!state)
        प्रत्यागम स्थितिः;

    #यदि_परिभाषितम् USE_ISWPRINT_L
    locale_t loc = newlocale(LC_ALL_MASK, "C.UTF-8", (locale_t)0);
    #यदि_अन्तः

    संरचना xkb_keymap_कुञ्चिका_iterator iter;
    ध्रुव संरचना xkb_keymap_कुञ्चिका_iterator_config iter_config = {
        .आकार = परिमाण(iter_config),
        .flags = XKB_KEYMAP_कुञ्चिका_ITERATOR_NO_FLAGS,
        .start = min
    };
    स्थितिः = xkb_keymap_कुञ्चिका_iterator_init(&iter, keymap, &iter_config);
    यदि (स्थितिः != XKB_SUCCESS)
        गच्छ error;

    ध्रुव xkb_mod_आवरणम्_t numlock =
        xkb_keymap_mod_get_आवरणम्(keymap, XKB_VMOD_NAME_अङ्कः);

    ध्रुव संरचना xkb_state_components_update components_update = {
        .आकार = परिमाण(components_update),
        .components = XKB_STATE_MODS_LOCKED,
        .affect_locked_mods = numlock,
        .locked_mods = numlock,
    };

    ध्रुव संरचना xkb_synthetic_update update = {
        .आकार = परिमाण(update),
        .components = &components_update,
    };

    xkb_keycode_t k;
    यावत् ((k = xkb_keymap_कुञ्चिका_iterator_अग्र(&iter)) != XKB_KEYCODE_INVALID) {
        यदि (k > max)
            भङ्ग;

        xkb_state_reset(state);
        स्थितिः = xkb_state_update_synthetic(state, &update, रिक्त);
        यदि (स्थितिः != XKB_SUCCESS)
            गच्छ error;

        ध्रुव xkb_keysym_t *syms = रिक्त;
        ध्रुव पूर्णाङ्क गणनम् = xkb_state_कुञ्चिका_get_syms(state, k, &syms);

        गणना xkb_state_component परिवर्तितम् =
            xkb_state_update_कुञ्चिका(state, k, XKB_कुञ्चिका_अधस्तात्);
        xkb_mod_आवरणम्_t mods =
            xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);
        परिवर्तितम् |= xkb_state_update_कुञ्चिका(state, k, XKB_कुञ्चिका_ऊर्ध्वम्);
        mods |= xkb_state_serialize_mods(state, XKB_STATE_MODS_EFFECTIVE);

        यदि (परिवर्तितम् & XKB_STATE_MODS_EFFECTIVE) {
            /* Modifier key */
            गणना xkb_modifier_विधा विधा;
            यदि (परिवर्तितम् & XKB_STATE_MODS_LOCKED) {
                विधा = XKB_MOD_तालकम्;
            } अन्यथा यदि (परिवर्तितम् & XKB_STATE_MODS_LATCHED) {
                विधा = XKB_MOD_LATCH;
            } अन्यथा {
                विधा = XKB_MOD_समुच्चयः;
            }
            mods &= ~numlock;
            यदि (!mods)
                mods = numlock;
            darray_append(समुच्चयः->modifiers, (संरचना xkb_modifier_कुञ्चिका) {
                .keycode = k,
                .mods = mods,
                .विधा = विधा,
            });
            अनुवर्त;
        }

        यदि (गणनम् > 0) {
            uint32_t utf32 = 0;
            प्रति (पूर्णाङ्क s = 0; s < गणनम्; s++) {
                utf32 = xkb_keysym_गन्तव्यम्_utf32(syms[s]);
                ध्रुव bool is_printable =
                    #यदि_परिभाषितम् USE_ISWPRINT_L
                    ((loc && iswprint_l((wint_t)utf32, loc)) || iswprint((wint_t)utf32))
                    #नोचेद्यदि defined(USE_ISWPRINT)
                    iswprint((wint_t)utf32)
                    #अन्यथा
                    utf32 <= 0x7f && isprint((पूर्णाङ्क)utf32)
                    #यदि_अन्तः
                    ;
                यदि (is_printable)
                    भङ्ग;
                utf32 = 0;
            }
            यदि (utf32) {
                /* Printable keysyms */
                darray_append(समुच्चयः->printable, k);
                अनुवर्त;
            }
        }

        /* Miscellaneous key */
        darray_append(समुच्चयः->misc, k);
    }

error:
    #यदि_परिभाषितम् USE_ISWPRINT_L
    यदि (loc)
        freelocale(loc);
    #यदि_अन्तः

    xkb_state_unref(state);
    यदि (स्थितिः != XKB_SUCCESS)
        xkb_कुञ्चिका_समुच्चयः_destroy(समुच्चयः);

    प्रत्यागम स्थितिः;
}

शून्य
xkb_कुञ्चिका_समुच्चयः_destroy(संरचना xkb_कुञ्चिका_समुच्चयः *समुच्चयः)
{
    यदि (!समुच्चयः)
        प्रत्यागम;

    darray_free(समुच्चयः->modifiers);
    darray_free(समुच्चयः->printable);
    darray_free(समुच्चयः->misc);
}

स्थिर गणना xkb_स्थितिः
सङ्कलय_pending_events(संरचना xkb_typing_state * सीमित state,
                   ध्रुव संरचना xkb_pending_typing_event * सीमित events,
                   आकार_t गणनम्)
{
    प्रति (आकार_t e = 0; e < गणनम्; e++) {
        /* Look for a free slot */
        संरचना xkb_pending_typing_event *pending;
        darray_आकार_t idx = 0;
        darray_enumerate_from(idx, pending, state->pending, idx) {
            यदि (!pending->गणक) {
                darray_item(state->pending, idx) = events[e];
                गच्छ अग्र;
            }
        }
        /* No free slot found: append new one */
        idx = darray_आकार(state->pending);
        darray_append(state->pending, events[e]);
    अग्र:
        /* Avoid immediate consumption */
        darray_item(state->pending, idx).गणक++;
    }
    state->अङ्कः_pending += गणनम्;

    प्रत्यागम XKB_SUCCESS;
}

स्थिर गणना xkb_स्थितिः
अग्र_pending_events(संरचना xkb_typing_state * सीमित state,
                    xkb_typing_events * सीमित बहिर्गमनम्)
{
    /* Reset the output */
    darray_आकार(*बहिर्गमनम्) = 0;

    darray_आकार_t idx;
    संरचना xkb_pending_typing_event *pending;
    /* Stack: start from the end */
    darray_enumerate_reverse(idx, pending, state->pending) {
        यदि (!pending->गणक)
            अनुवर्त;
        pending->गणक--;
        यदि (pending->गणक)
            अनुवर्त;
        darray_append(*बहिर्गमनम्, pending->event);
        state->अङ्कः_pending--;
        /* Resize array if it was the last element */
        यदि (idx + 1 == darray_आकार(state->pending))
            darray_आकार(state->pending)--;
        state->mods &= ~pending->mods;
    }

    प्रत्यागम XKB_SUCCESS;
}

गणना {
    /** Tries to avoid pressing a key already pressed */
    कुञ्चिका_CHOICE_MAX_TRIES = 12,
    XKB_ERROR_NO_RELEASED_कुञ्चिका = -1000,
};

स्थिर गणना xkb_स्थितिः
सङ्कलय_modifier_कुञ्चिका(संरचना xkb_typing_state * सीमित state, आकार_t * सीमित e)
{
    /* Choose a random key */
    दीर्घ idx = -1;
    प्रति (पूर्णाङ्क tries = 0; tries < कुञ्चिका_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state)
            % darray_आकार(state->कुञ्चिकाः->modifiers);
        /*
         * Ensure key is not already down and its modifier mask is not active,
         * i.e. to avoid simultaneous keys with the same modifier.
         */
        यदि (!bit_array_get(state->कुञ्चिकाः_state,
                           darray_item(state->कुञ्चिकाः->modifiers, idx).keycode) &&
            !(state->mods & darray_item(state->कुञ्चिकाः->modifiers, idx).mods))
        {
            भङ्ग;
        }
        idx = -1;
    }

    संरचना xkb_modifier_कुञ्चिका कुञ्चिका = {0};
    यदि (idx < 0) {
        /* No released key found: fall back to linear search */
        ध्रुव संरचना xkb_modifier_कुञ्चिका *कुञ्चिका_2;
        darray_foreach(कुञ्चिका_2, state->कुञ्चिकाः->modifiers) {
            /* See test doc above */
            यदि (!bit_array_get(state->कुञ्चिकाः_state, कुञ्चिका_2->keycode) &&
                !(state->mods & कुञ्चिका_2->mods))
            {
                कुञ्चिका = *कुञ्चिका_2;
                idx = 0;
                भङ्ग;
            }
        }
        यदि (idx < 0) {
            /* No fallback: skip */
            प्रत्यागम (गणना xkb_स्थितिः)XKB_ERROR_NO_RELEASED_कुञ्चिका;
        }
    } अन्यथा {
        कुञ्चिका = darray_item(state->कुञ्चिकाः->modifiers, idx);
    }

    ध्रुव आकार_t remaining_events =
        state->अङ्कः_events - state->अङ्कः_pending - *e;
    आकार_t pending_turns = 0;

    यदि (remaining_events >= 2) {
        यदि (कुञ्चिका.विधा == XKB_MOD_समुच्चयः ||
            (कुञ्चिका.विधा == XKB_MOD_तालकम् && remaining_events >= 4))
        {
            /* Delay SetMods release or LockMods unlock */
            ध्रुव आकार_t max_turns = MIN(
                (आकार_t)(
                    XKB_TYPING_EVENT_MODIFIER_MAX_PENDING_TURNS -
                    XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                 ),
                remaining_events + state->अङ्कः_pending
            );
            pending_turns = XKB_TYPING_EVENT_MODIFIER_MIN_PENDING_TURNS
                          + (आकार_t)state->prng(state->prng_state)
                          % (max_turns + 1);
        }
    } अन्यथा {
        प्रत्यागम XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (संरचना xkb_typing_event) {
        .keycode = कुञ्चिका.keycode,
        .direction = XKB_कुञ्चिका_अधस्तात्,
    };

    संरचना xkb_pending_typing_event pending[2] = {
        {
            .event = {
                .keycode = कुञ्चिका.keycode,
                .direction = XKB_कुञ्चिका_अधस्तात्,
            },
            .गणक = (uint8_t)pending_turns,
        },
        {
            .event = {
                .keycode = कुञ्चिका.keycode,
                .direction = XKB_कुञ्चिका_ऊर्ध्वम्,
            },
            .mods = कुञ्चिका.mods,
            .गणक = (uint8_t)pending_turns,
        },
    };

    यदि (pending_turns) {
        यदि (कुञ्चिका.विधा == XKB_MOD_तालकम्) {
            /* Immediate release */
            state->events[(*e)++] = pending[1].event;
            /* Delayed unlock (press + release) */
            pending[1].गणक++;
            ध्रुव गणना xkb_स्थितिः स्थितिः =
                सङ्कलय_pending_events(state, pending, ARRAY_आकार(pending));
            यदि (स्थितिः != XKB_SUCCESS)
                प्रत्यागम स्थितिः;
            /*
             * Register “locked” state, to avoid tapping the key before
             * delayed key released
             */
            bit_array_समुच्चयः(state->कुञ्चिकाः_state, कुञ्चिका.keycode);
        } अन्यथा {
            /* Delayed release */
            ध्रुव गणना xkb_स्थितिः स्थितिः =
                सङ्कलय_pending_events(state, &pending[1], 1);
            यदि (स्थितिः != XKB_SUCCESS)
                प्रत्यागम स्थितिः;
            /* Register pressed state */
            bit_array_समुच्चयः(state->कुञ्चिकाः_state, कुञ्चिका.keycode);
        }
        state->mods |= कुञ्चिका.mods;
    } अन्यथा {
        /* Immediate release */
        state->events[(*e)++] = pending[1].event;
        /* TODO: add latched modifier to mask state. */
    }

    प्रत्यागम XKB_SUCCESS;
}

स्थिर गणना xkb_स्थितिः
सङ्कलय_other_कुञ्चिका(संरचना xkb_typing_state * सीमित state,
              ध्रुव xkb_keycodes_t * सीमित keycodes, आकार_t * सीमित e,
              uint8_t तात्कालिकम्_release_prob, uint8_t max_pending_turns)
{
    /* Choose a random key */
    दीर्घ idx = -1;
    प्रति (पूर्णाङ्क tries = 0; tries < कुञ्चिका_CHOICE_MAX_TRIES; tries++) {
        idx = state->prng(state->prng_state) % darray_आकार(*keycodes);
        /* Ensure key is not already down */
        यदि (!bit_array_get(state->कुञ्चिकाः_state,
                           darray_item(*keycodes, idx)))
        {
            भङ्ग;
        }
        idx = -1;
    }

    xkb_keycode_t keycode = XKB_KEYCODE_INVALID;
    यदि (idx < 0) {
        /* No released key found: fall back to linear search */
        ध्रुव xkb_keycode_t *keycode_;
        darray_foreach(keycode_, *keycodes) {
            यदि (!bit_array_get(state->कुञ्चिकाः_state, *keycode_)) {
                keycode = *keycode_;
                idx = 0;
                भङ्ग;
            }
        }
        यदि (idx < 0) {
            /* No fallback: skip */
            प्रत्यागम (गणना xkb_स्थितिः)XKB_ERROR_NO_RELEASED_कुञ्चिका;
        }
    } अन्यथा {
        keycode = darray_item(*keycodes, idx);
    }

    ध्रुव आकार_t remaining_events =
        state->अङ्कः_events - state->अङ्कः_pending - *e;
    आकार_t pending_turns;

    यदि (remaining_events >= 2) {
        ध्रुव आकार_t max_turns = MIN(
            (आकार_t)max_pending_turns,
            remaining_events + state->अङ्कः_pending
        );
        ध्रुव bool तात्कालिकम्_release = (
            (तात्कालिकम्_release_prob >= 100) ||
            !max_turns ||
            (state->prng(state->prng_state) % 100) < तात्कालिकम्_release_prob
        );
        यदि (तात्कालिकम्_release) {
            pending_turns = 0;
        } अन्यथा {
            pending_turns = 1 + (आकार_t)state->prng(state->prng_state)
                          % max_turns;
        }
    } अन्यथा {
        प्रत्यागम XKB_ERROR_INVALID;
    }

    state->events[(*e)++] = (संरचना xkb_typing_event) {
        .keycode = keycode,
        .direction = XKB_कुञ्चिका_अधस्तात्,
    };

    ध्रुव संरचना xkb_pending_typing_event pending = {
        .event = {
            .keycode = keycode,
            .direction = XKB_कुञ्चिका_ऊर्ध्वम्,
        },
        .गणक = (uint8_t)pending_turns,
    };
    यदि (pending_turns) {
        ध्रुव गणना xkb_स्थितिः स्थितिः = सङ्कलय_pending_events(state, &pending, 1);
        यदि (स्थितिः != XKB_SUCCESS)
            प्रत्यागम स्थितिः;
        /* Register pressed state */
        bit_array_समुच्चयः(state->कुञ्चिकाः_state, keycode);
    } अन्यथा {
        state->events[(*e)++] = pending.event;
    }

    प्रत्यागम XKB_SUCCESS;
}

स्थिर गणना xkb_कुञ्चिका_विधा
pick_कुञ्चिका_विधा(xkb_prng_t prng, शून्य *prng_state)
{
    स्थिर ध्रुव uint8_t weights[अङ्कः_XKB_कुञ्चिका_विधा] = {
        [XKB_कुञ्चिका_PRINTABLE] = 60,
        [XKB_कुञ्चिका_MODIFIER] = 30,
        [XKB_कुञ्चिका_MISC] = 10,
    };
    static_assert(XKB_कुञ्चिका_MISC == 2 &&
                  XKB_कुञ्चिका_MISC == अङ्कः_XKB_कुञ्चिका_विधा - 1, "");
    प्रतिज्ञा(weights[0] + weights[1] + weights[2] == 100);

    दीर्घ r = prng(prng_state) % 100;
    प्रति (गणना xkb_कुञ्चिका_विधा k = 0; k < अङ्कः_XKB_कुञ्चिका_विधा; k++)
        यदि ((r -= weights[k]) < 0)
            प्रत्यागम k;
    प्रत्यागम XKB_कुञ्चिका_PRINTABLE;
}

स्थिर शून्य
xkb_typing_state_destroy(संरचना xkb_typing_state *state)
{
    मुक्त(state->कुञ्चिकाः_state);
    darray_free(state->pending);
    मुक्त(state->events);
}

गणना xkb_स्थितिः
xkb_typing_निवेशः_init(संरचना xkb_typing_निवेशः * सीमित typing,
                      ध्रुव संरचना xkb_कुञ्चिका_समुच्चयः * सीमित कुञ्चिकाः,
                      ध्रुव संरचना xkb_typing_निवेशः_config * सीमित config)
{
    typing->events = रिक्त;
    typing->अङ्कः_events = 0;

    यदि (!config->निवेशः_दीर्घता)
        प्रत्यागम XKB_ERROR_INVALID;

    यदि (darray_रिक्तानि(कुञ्चिकाः->printable) ||
        darray_रिक्तानि(कुञ्चिकाः->modifiers) ||
        darray_रिक्तानि(कुञ्चिकाः->misc))
    {
        प्रत्यागम XKB_ERROR_INVALID;
    }

    गणना xkb_स्थितिः स्थितिः = XKB_SUCCESS;

    /* NOTE: arrays are sorted because they were built using the key iterator */
    xkb_keycode_t keycode_max = darray_चरमः(कुञ्चिकाः->printable, 0);
    keycode_max = MAX(
        darray_चरमः(कुञ्चिकाः->modifiers, (संरचना xkb_modifier_कुञ्चिका){0}).keycode,
        keycode_max
    );
    keycode_max = MAX(darray_चरमः(कुञ्चिकाः->misc, 0), keycode_max);
    ध्रुव आकार_t कुञ्चिकाः_state_अङ्कः = bit_array_शब्दाः((आकार_t)keycode_max + 1);
    uintptr_t *कुञ्चिकाः_state = शून्यस्मृतिं_देहि(कुञ्चिकाः_state_अङ्कः, परिमाण(*कुञ्चिकाः_state));

    ध्रुव आकार_t अङ्कः_events = config->निवेशः_दीर्घता;
    संरचना xkb_typing_event *events = शून्यस्मृतिं_देहि(अङ्कः_events, परिमाण(*events));

    संरचना xkb_typing_state state = {
        .कुञ्चिकाः = कुञ्चिकाः,
        .कुञ्चिकाः_state = कुञ्चिकाः_state,
        .prng = config->prng,
        .prng_state = config->prng_state,
        .events = events,
        .अङ्कः_events = अङ्कः_events,
        .pending = darray_new(),
        .अङ्कः_pending = 0,
    };

    xkb_typing_events pending = darray_new();

    यदि (!कुञ्चिकाः_state || !events) {
        स्थितिः = XKB_ERROR_आवण्टनम्_FAILURE;
        गच्छ error;
    }

    /*
     * Invariant: e + state.num_pending <= state.num_events
     */

    प्रति (आकार_t e = 0; e < अङ्कः_events;) {
        /*
         * Process pending events
         */
        स्थितिः = अग्र_pending_events(&state, &pending);
        यदि (स्थितिः != XKB_SUCCESS) {
            गच्छ error;
        }
        संरचना xkb_typing_event *event;
        darray_foreach(event, pending) {
            यदि (e >= अङ्कः_events) {
                /* invariant violated: should be unreachable */
                PANIC_UNREACHABLE();
            }
            events[e] = *event;
            bit_array_नियोजय(कुञ्चिकाः_state, event->keycode, (bool)event->direction);
            e++;
        }

        यदि (!state.अङ्कः_pending && e == अङ्कः_events - 1) {
            /*
             * Cannot add press/release *pair*, so just use an invalid keycode
             * for the last event.
             */
            events[e] = (संरचना xkb_typing_event) {
                .keycode = XKB_KEYCODE_INVALID,
                .direction = XKB_कुञ्चिका_अधस्तात्,
            };
            भङ्ग;
        }

        यदि (e + 1 + state.अङ्कः_pending >= अङ्कः_events) {
            /* Cannot add new pair of events: require flushing pending events */
            अनुवर्त;
        }

        /*
         * Create new events
         */

        /* Choose kind of key */
        static_assert(XKB_कुञ्चिका_PRINTABLE == 0, "");
        ध्रुव गणना xkb_कुञ्चिका_विधा कुञ्चिका_विधा =
            pick_कुञ्चिका_विधा(config->prng, config->prng_state);
        विकल्प (कुञ्चिका_विधा) {
        पक्ष XKB_कुञ्चिका_PRINTABLE:
            स्थितिः = सङ्कलय_other_कुञ्चिका(
                &state, &state.कुञ्चिकाः->printable, &e,
                XKB_TYPING_EVENT_PRINTABLE_तात्कालिकम्_RELEASE_PERCENT,
                XKB_TYPING_EVENT_PRINTABLE_MAX_PENDING_TURNS
            );
            भङ्ग;
        पक्ष XKB_कुञ्चिका_MODIFIER:
            स्थितिः = सङ्कलय_modifier_कुञ्चिका(&state, &e);
            भङ्ग;
        पक्ष XKB_कुञ्चिका_MISC:
            स्थितिः = सङ्कलय_other_कुञ्चिका(
                &state, &state.कुञ्चिकाः->misc, &e,
                XKB_TYPING_EVENT_MISC_तात्कालिकम्_RELEASE_PERCENT,
                XKB_TYPING_EVENT_MISC_MAX_PENDING_TURNS
            );
            भङ्ग;
        स्वभाव: {
            static_assert(XKB_कुञ्चिका_MISC == 2 &&
                          XKB_कुञ्चिका_MISC == अङ्कः_XKB_कुञ्चिका_विधा - 1, "");
            स्थितिः = XKB_ERROR_INVALID;
        }}

        यदि (स्थितिः != XKB_SUCCESS &&
            स्थितिः != (गणना xkb_स्थितिः)XKB_ERROR_NO_RELEASED_कुञ्चिका)
        {
            गच्छ error;
        }
    }

    प्रतिज्ञा(!state.अङ्कः_pending);

    typing->events = steal(&state.events);
    typing->अङ्कः_events = अङ्कः_events;

    स्थितिः = XKB_SUCCESS;

error:
    darray_free(pending);
    xkb_typing_state_destroy(&state);

    प्रत्यागम स्थितिः;
}

शून्य
xkb_typing_निवेशः_destroy(संरचना xkb_typing_निवेशः *typing)
{
    यदि (!typing)
        प्रत्यागम;

    मुक्त(typing->events);
    typing->events = रिक्त;
}
