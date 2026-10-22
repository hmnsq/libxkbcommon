/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>

#ಅನ್ತರ್ಭಾವ "context.h"
#ಅನ್ತರ್ಭಾವ "features/enums.h"
#ಅನ್ತರ್ಭಾವ "keysym.h"
#ಅನ್ತರ್ಭಾವ "table.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

ರಚನೆ xkb_compose_state {
    ಪೂರ್ಣಾಂಕ refcnt;
    ಎಣಿಕೆ xkb_compose_state_flags flags;
    ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ;

    /*
     * Offsets into xkb_compose_table::nodes.
     *
     * They maintain the current and previous position in the trie; see
     * xkb_compose_state_feed().
     *
     * This is also sufficient for inferring the current status; see
     * xkb_compose_state_get_status().
     */
    uint32_t prev_context;
    uint32_t context;
};

ರಚನೆ xkb_compose_state *
xkb_compose_state_new(ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ,
                      ಎಣಿಕೆ xkb_compose_state_flags flags)
{
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_compose_state_flags invalid_flags =
        (flags & ~(ಎಣಿಕೆ xkb_compose_state_flags)XKB_COMPOSE_STATE_FLAGS_ಮೌಲ್ಯಗಳು);
    ಆದರೆ (invalid_flags) {
        log_err_func(ಕೋಷ್ಟಕ->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                     "Unsupported compose state flags: %#x\n",
                     invalid_flags);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ರಚನೆ xkb_compose_state * ಸ್ಥಿರಾಂಕ state = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(*state));
    ಆದರೆ (!state)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    state->refcnt = 1;
    state->ಕೋಷ್ಟಕ = xkb_compose_ಕೋಷ್ಟಕ_ref(ಕೋಷ್ಟಕ);

    state->flags = flags;
    state->prev_context = 0;
    state->context = 0;

    ಹಿಂತಿರುಗಿ state;
}

ರಚನೆ xkb_compose_state *
xkb_compose_state_ref(ರಚನೆ xkb_compose_state *state)
{
    ಪ್ರತಿಜ್ಞಾ(state->refcnt > 0);
    state->refcnt++;
    ಹಿಂತಿರುಗಿ state;
}

ಶೂನ್ಯ
xkb_compose_state_unref(ರಚನೆ xkb_compose_state *state)
{
    ಪ್ರತಿಜ್ಞಾ(!state || state->refcnt > 0);
    ಆದರೆ (!state || --state->refcnt > 0)
        ಹಿಂತಿರುಗಿ;

    xkb_compose_ಕೋಷ್ಟಕ_unref(state->ಕೋಷ್ಟಕ);
    ಬಿಡುಗಡೆ(state);
}

ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *
xkb_compose_state_get_compose_ಕೋಷ್ಟಕ(ರಚನೆ xkb_compose_state *state)
{
    ಹಿಂತಿರುಗಿ state->ಕೋಷ್ಟಕ;
}

ಎಣಿಕೆ xkb_compose_feed_ಫಲಿತಾಂಶ
xkb_compose_state_feed(ರಚನೆ xkb_compose_state *state, xkb_keysym_t keysym)
{
    uint32_t context;
    ಸ್ಥಿರಾಂಕ ರಚನೆ compose_ಗಂಟು *ಗಂಟು;

    /*
     * Modifiers do not affect the sequence directly.  In particular,
     * they do not cancel a sequence; otherwise it'd be impossible to
     * have a sequence like <dead_acute><A> (needs Shift in the middle).
     *
     * The following test is not really accurate - in order to test if
     * a key is "modifier key", we really need the keymap, but we don't
     * have it here.  However, this is (approximately) what libX11 does
     * as well.
     */
    ಆದರೆ (xkb_keysym_is_modifier(keysym))
        ಹಿಂತಿರುಗಿ XKB_COMPOSE_FEED_IGNORED;

    ಗಂಟು = &darray_ಅಂಶ(state->ಕೋಷ್ಟಕ->nodes, state->context);

    context = (ಗಂಟು->is_ಎಲೆ ? 1 : ಗಂಟು->ಆಂತರಿಕ.eqkid);
    ಆದರೆ (context == 1 && darray_ಗಾತ್ರ(state->ಕೋಷ್ಟಕ->nodes) == 1)
        context = 0;

    ಆಗ (context != 0) {
        ಗಂಟು = &darray_ಅಂಶ(state->ಕೋಷ್ಟಕ->nodes, context);
        ಆದರೆ (keysym < ಗಂಟು->keysym)
            context = ಗಂಟು->lokid;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (keysym > ಗಂಟು->keysym)
            context = ಗಂಟು->hikid;
        ಇಲ್ಲದಿದ್ದರೆ
            ನಿಲ್ಲಿಸು;
    }

    state->prev_context = state->context;
    state->context = context;
    ಹಿಂತಿರುಗಿ XKB_COMPOSE_FEED_ACCEPTED;
}

ಶೂನ್ಯ
xkb_compose_state_reset(ರಚನೆ xkb_compose_state *state)
{
    state->prev_context = 0;
    state->context = 0;
}

ಎಣಿಕೆ xkb_compose_ಸ್ಥಿತಿ
xkb_compose_state_get_ಸ್ಥಿತಿ(ರಚನೆ xkb_compose_state *state)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ compose_ಗಂಟು *prev_ಗಂಟು, *ಗಂಟು;

    prev_ಗಂಟು = &darray_ಅಂಶ(state->ಕೋಷ್ಟಕ->nodes, state->prev_context);
    ಗಂಟು = &darray_ಅಂಶ(state->ಕೋಷ್ಟಕ->nodes, state->context);

    ಆದರೆ (state->context == 0 && !prev_ಗಂಟು->is_ಎಲೆ)
        ಹಿಂತಿರುಗಿ XKB_COMPOSE_CANCELLED;

    ಆದರೆ (state->context == 0)
        ಹಿಂತಿರುಗಿ XKB_COMPOSE_NOTHING;

    ಆದರೆ (!ಗಂಟು->is_ಎಲೆ)
        ಹಿಂತಿರುಗಿ XKB_COMPOSE_COMPOSING;

    ಹಿಂತಿರುಗಿ XKB_COMPOSE_COMPOSED;
}

ಪೂರ್ಣಾಂಕ
xkb_compose_state_get_utf8(ರಚನೆ xkb_compose_state *state,
                           ಅಕ್ಷರ *ಬಫರ್, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ compose_ಗಂಟು *ಗಂಟು =
        &darray_ಅಂಶ(state->ಕೋಷ್ಟಕ->nodes, state->context);

    ಆದರೆ (!ಗಂಟು->is_ಎಲೆ)
        ಹೋಗು fail;

    /* If there's no string specified, but only a keysym, try to do the
     * most helpful thing. */
    ಆದರೆ (ಗಂಟು->ಎಲೆ.utf8 == 0 && ಗಂಟು->ಎಲೆ.keysym != XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
        ಅಕ್ಷರ utf8[XKB_KEYSYM_UTF8_MAX_ಗಾತ್ರ];
        ಪೂರ್ಣಾಂಕ ret;

        ret = xkb_keysym_ಗಮ್ಯ_utf8(ಗಂಟು->ಎಲೆ.keysym, utf8, ಗಾತ್ರ(utf8));
        ಆದರೆ (ret < 0 || ret == 0) {
            /* ret < 0 is impossible.
             * ret == 0 means the keysym has no string representation. */
            ಹೋಗು fail;
        }

        ಹಿಂತಿರುಗಿ snprintf(ಬಫರ್, ಗಾತ್ರ_2, "%s", utf8);
    }

    ಹಿಂತಿರುಗಿ snprintf(ಬಫರ್, ಗಾತ್ರ_2, "%s",
                    &darray_ಅಂಶ(state->ಕೋಷ್ಟಕ->utf8, ಗಂಟು->ಎಲೆ.utf8));

fail:
    ಆದರೆ (ಗಾತ್ರ_2 > 0)
        ಬಫರ್[0] = '\0';
    ಹಿಂತಿರುಗಿ 0;
}

xkb_keysym_t
xkb_compose_state_get_one_sym(ರಚನೆ xkb_compose_state *state)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ compose_ಗಂಟು *ಗಂಟು =
        &darray_ಅಂಶ(state->ಕೋಷ್ಟಕ->nodes, state->context);
    ಆದರೆ (!ಗಂಟು->is_ಎಲೆ)
        ಹಿಂತಿರುಗಿ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
    ಹಿಂತಿರುಗಿ ಗಂಟು->ಎಲೆ.keysym;
}
