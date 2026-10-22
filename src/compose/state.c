/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>

#અન્તર્ભાવ "context.h"
#અન્તર્ભાવ "features/enums.h"
#અન્તર્ભાવ "keysym.h"
#અન્તર્ભાવ "table.h"
#અન્તર્ભાવ "utils.h"

સંરચના xkb_compose_state {
    પૂર્ણાંક refcnt;
    ગણના xkb_compose_state_flags flags;
    સંરચના xkb_compose_table *table;

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

સંરચના xkb_compose_state *
xkb_compose_state_new(સંરચના xkb_compose_table *table,
                      ગણના xkb_compose_state_flags flags)
{
    અચલ ગણના xkb_compose_state_flags invalid_flags =
        (flags & ~(ગણના xkb_compose_state_flags)XKB_COMPOSE_STATE_FLAGS_કિંમતો);
    જો (invalid_flags) {
        log_err_func(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported compose state flags: %#x\n",
                     invalid_flags);
        પાછા ખાલી;
    }

    સંરચના xkb_compose_state * અચલ state = calloc(1, કદ(*state));
    જો (!state)
        પાછા ખાલી;

    state->refcnt = 1;
    state->table = xkb_compose_table_ref(table);

    state->flags = flags;
    state->prev_context = 0;
    state->context = 0;

    પાછા state;
}

સંરચના xkb_compose_state *
xkb_compose_state_ref(સંરચના xkb_compose_state *state)
{
    પ્રતિજ્ઞા(state->refcnt > 0);
    state->refcnt++;
    પાછા state;
}

શૂન્ય
xkb_compose_state_unref(સંરચના xkb_compose_state *state)
{
    પ્રતિજ્ઞા(!state || state->refcnt > 0);
    જો (!state || --state->refcnt > 0)
        પાછા;

    xkb_compose_table_unref(state->table);
    મુક્ત(state);
}

સંરચના xkb_compose_table *
xkb_compose_state_get_compose_table(સંરચના xkb_compose_state *state)
{
    પાછા state->table;
}

ગણના xkb_compose_feed_result
xkb_compose_state_feed(સંરચના xkb_compose_state *state, xkb_keysym_t keysym)
{
    uint32_t context;
    અચલ સંરચના compose_ગાંઠ *ગાંઠ;

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
    જો (xkb_keysym_is_modifier(keysym))
        પાછા XKB_COMPOSE_FEED_IGNORED;

    ગાંઠ = &darray_item(state->table->nodes, state->context);

    context = (ગાંઠ->is_leaf ? 1 : ગાંઠ->internal.eqkid);
    જો (context == 1 && darray_size(state->table->nodes) == 1)
        context = 0;

    જ્યારે (context != 0) {
        ગાંઠ = &darray_item(state->table->nodes, context);
        જો (keysym < ગાંઠ->keysym)
            context = ગાંઠ->lokid;
        નહીંતર જો (keysym > ગાંઠ->keysym)
            context = ગાંઠ->hikid;
        નહીંતર
            તોડો;
    }

    state->prev_context = state->context;
    state->context = context;
    પાછા XKB_COMPOSE_FEED_ACCEPTED;
}

શૂન્ય
xkb_compose_state_reset(સંરચના xkb_compose_state *state)
{
    state->prev_context = 0;
    state->context = 0;
}

ગણના xkb_compose_status
xkb_compose_state_get_status(સંરચના xkb_compose_state *state)
{
    અચલ સંરચના compose_ગાંઠ *prev_ગાંઠ, *ગાંઠ;

    prev_ગાંઠ = &darray_item(state->table->nodes, state->prev_context);
    ગાંઠ = &darray_item(state->table->nodes, state->context);

    જો (state->context == 0 && !prev_ગાંઠ->is_leaf)
        પાછા XKB_COMPOSE_CANCELLED;

    જો (state->context == 0)
        પાછા XKB_COMPOSE_NOTHING;

    જો (!ગાંઠ->is_leaf)
        પાછા XKB_COMPOSE_COMPOSING;

    પાછા XKB_COMPOSE_COMPOSED;
}

પૂર્ણાંક
xkb_compose_state_get_utf8(સંરચના xkb_compose_state *state,
                           અક્ષર *buffer, size_t size)
{
    અચલ સંરચના compose_ગાંઠ *ગાંઠ =
        &darray_item(state->table->nodes, state->context);

    જો (!ગાંઠ->is_leaf)
        જાઓ fail;

    /* If there's no string specified, but only a keysym, try to do the
     * most helpful thing. */
    જો (ગાંઠ->leaf.utf8 == 0 && ગાંઠ->leaf.keysym != XKB_ચાવી_No_પ્રતીક) {
        અક્ષર utf8[XKB_KEYSYM_UTF8_MAX_SIZE];
        પૂર્ણાંક ret;

        ret = xkb_keysym_to_utf8(ગાંઠ->leaf.keysym, utf8, કદ(utf8));
        જો (ret < 0 || ret == 0) {
            /* ret < 0 is impossible.
             * ret == 0 means the keysym has no string representation. */
            જાઓ fail;
        }

        પાછા snprintf(buffer, size, "%s", utf8);
    }

    પાછા snprintf(buffer, size, "%s",
                    &darray_item(state->table->utf8, ગાંઠ->leaf.utf8));

fail:
    જો (size > 0)
        buffer[0] = '\0';
    પાછા 0;
}

xkb_keysym_t
xkb_compose_state_get_one_sym(સંરચના xkb_compose_state *state)
{
    અચલ સંરચના compose_ગાંઠ *ગાંઠ =
        &darray_item(state->table->nodes, state->context);
    જો (!ગાંઠ->is_leaf)
        પાછા XKB_ચાવી_No_પ્રતીક;
    પાછા ગાંઠ->leaf.keysym;
}
