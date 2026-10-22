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

ಸಂರಚನಾ xkb_compose_state {
    ಪೂರ್ಣಾಙ್ಕ refcnt;
    ಗಣನಾ xkb_compose_state_flags flags;
    ಸಂರಚನಾ xkb_compose_table *table;

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

ಸಂರಚನಾ xkb_compose_state *
xkb_compose_state_new(ಸಂರಚನಾ xkb_compose_table *table,
                      ಗಣನಾ xkb_compose_state_flags flags)
{
    ಧ್ರುವ ಗಣನಾ xkb_compose_state_flags invalid_flags =
        (flags & ~(ಗಣನಾ xkb_compose_state_flags)XKB_COMPOSE_STATE_FLAGS_ಮೌಲ್ಯಗಳು);
    ಯದಿ (invalid_flags) {
        log_err_func(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported compose state flags: %#x\n",
                     invalid_flags);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಸಂರಚನಾ xkb_compose_state * ಧ್ರುವ state = calloc(1, ಪರಿಮಾಣ(*state));
    ಯದಿ (!state)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    state->refcnt = 1;
    state->table = xkb_compose_table_ref(table);

    state->flags = flags;
    state->prev_context = 0;
    state->context = 0;

    ಪ್ರತ್ಯಾಗಮ state;
}

ಸಂರಚನಾ xkb_compose_state *
xkb_compose_state_ref(ಸಂರಚನಾ xkb_compose_state *state)
{
    ಪ್ರತಿಜ್ಞಾ(state->refcnt > 0);
    state->refcnt++;
    ಪ್ರತ್ಯಾಗಮ state;
}

ಶೂನ್ಯ
xkb_compose_state_unref(ಸಂರಚನಾ xkb_compose_state *state)
{
    ಪ್ರತಿಜ್ಞಾ(!state || state->refcnt > 0);
    ಯದಿ (!state || --state->refcnt > 0)
        ಪ್ರತ್ಯಾಗಮ;

    xkb_compose_table_unref(state->table);
    ಮುಕ್ತ(state);
}

ಸಂರಚನಾ xkb_compose_table *
xkb_compose_state_get_compose_table(ಸಂರಚನಾ xkb_compose_state *state)
{
    ಪ್ರತ್ಯಾಗಮ state->table;
}

ಗಣನಾ xkb_compose_feed_result
xkb_compose_state_feed(ಸಂರಚನಾ xkb_compose_state *state, xkb_keysym_t keysym)
{
    uint32_t context;
    ಧ್ರುವ ಸಂರಚನಾ compose_ಗಂಟು *ಗಂಟು;

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
    ಯದಿ (xkb_keysym_is_modifier(keysym))
        ಪ್ರತ್ಯಾಗಮ XKB_COMPOSE_FEED_IGNORED;

    ಗಂಟು = &darray_item(state->table->nodes, state->context);

    context = (ಗಂಟು->is_leaf ? 1 : ಗಂಟು->internal.eqkid);
    ಯದಿ (context == 1 && darray_size(state->table->nodes) == 1)
        context = 0;

    ಯಾವತ್ (context != 0) {
        ಗಂಟು = &darray_item(state->table->nodes, context);
        ಯದಿ (keysym < ಗಂಟು->keysym)
            context = ಗಂಟು->lokid;
        ಅನ್ಯಥಾ ಯದಿ (keysym > ಗಂಟು->keysym)
            context = ಗಂಟು->hikid;
        ಅನ್ಯಥಾ
            ಭಙ್ಗ;
    }

    state->prev_context = state->context;
    state->context = context;
    ಪ್ರತ್ಯಾಗಮ XKB_COMPOSE_FEED_ACCEPTED;
}

ಶೂನ್ಯ
xkb_compose_state_reset(ಸಂರಚನಾ xkb_compose_state *state)
{
    state->prev_context = 0;
    state->context = 0;
}

ಗಣನಾ xkb_compose_status
xkb_compose_state_get_status(ಸಂರಚನಾ xkb_compose_state *state)
{
    ಧ್ರುವ ಸಂರಚನಾ compose_ಗಂಟು *prev_ಗಂಟು, *ಗಂಟು;

    prev_ಗಂಟು = &darray_item(state->table->nodes, state->prev_context);
    ಗಂಟು = &darray_item(state->table->nodes, state->context);

    ಯದಿ (state->context == 0 && !prev_ಗಂಟು->is_leaf)
        ಪ್ರತ್ಯಾಗಮ XKB_COMPOSE_CANCELLED;

    ಯದಿ (state->context == 0)
        ಪ್ರತ್ಯಾಗಮ XKB_COMPOSE_NOTHING;

    ಯದಿ (!ಗಂಟು->is_leaf)
        ಪ್ರತ್ಯಾಗಮ XKB_COMPOSE_COMPOSING;

    ಪ್ರತ್ಯಾಗಮ XKB_COMPOSE_COMPOSED;
}

ಪೂರ್ಣಾಙ್ಕ
xkb_compose_state_get_utf8(ಸಂರಚನಾ xkb_compose_state *state,
                           ವರ್ಣ *buffer, size_t size)
{
    ಧ್ರುವ ಸಂರಚನಾ compose_ಗಂಟು *ಗಂಟು =
        &darray_item(state->table->nodes, state->context);

    ಯದಿ (!ಗಂಟು->is_leaf)
        ಗಚ್ಛ fail;

    /* If there's no string specified, but only a keysym, try to do the
     * most helpful thing. */
    ಯದಿ (ಗಂಟು->leaf.utf8 == 0 && ಗಂಟು->leaf.keysym != XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
        ವರ್ಣ utf8[XKB_KEYSYM_UTF8_MAX_SIZE];
        ಪೂರ್ಣಾಙ್ಕ ret;

        ret = xkb_keysym_to_utf8(ಗಂಟು->leaf.keysym, utf8, ಪರಿಮಾಣ(utf8));
        ಯದಿ (ret < 0 || ret == 0) {
            /* ret < 0 is impossible.
             * ret == 0 means the keysym has no string representation. */
            ಗಚ್ಛ fail;
        }

        ಪ್ರತ್ಯಾಗಮ snprintf(buffer, size, "%s", utf8);
    }

    ಪ್ರತ್ಯಾಗಮ snprintf(buffer, size, "%s",
                    &darray_item(state->table->utf8, ಗಂಟು->leaf.utf8));

fail:
    ಯದಿ (size > 0)
        buffer[0] = '\0';
    ಪ್ರತ್ಯಾಗಮ 0;
}

xkb_keysym_t
xkb_compose_state_get_one_sym(ಸಂರಚನಾ xkb_compose_state *state)
{
    ಧ್ರುವ ಸಂರಚನಾ compose_ಗಂಟು *ಗಂಟು =
        &darray_item(state->table->nodes, state->context);
    ಯದಿ (!ಗಂಟು->is_leaf)
        ಪ್ರತ್ಯಾಗಮ XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
    ಪ್ರತ್ಯಾಗಮ ಗಂಟು->leaf.keysym;
}
