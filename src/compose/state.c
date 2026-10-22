/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>

#அந்தர்பாவ "context.h"
#அந்தர்பாவ "features/enums.h"
#அந்தர்பாவ "keysym.h"
#அந்தர்பாவ "table.h"
#அந்தர்பாவ "utils.h"

கட்டமைப்பு xkb_compose_state {
    முழு refcnt;
    எண்ணிக்கை xkb_compose_state_flags flags;
    கட்டமைப்பு xkb_compose_table *table;

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

கட்டமைப்பு xkb_compose_state *
xkb_compose_state_new(கட்டமைப்பு xkb_compose_table *table,
                      எண்ணிக்கை xkb_compose_state_flags flags)
{
    மாறிலி எண்ணிக்கை xkb_compose_state_flags invalid_flags =
        (flags & ~(எண்ணிக்கை xkb_compose_state_flags)XKB_COMPOSE_STATE_FLAGS_மதிப்புகள்);
    எனில் (invalid_flags) {
        log_err_func(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported compose state flags: %#x\n",
                     invalid_flags);
        திரும்பு வெற்று;
    }

    கட்டமைப்பு xkb_compose_state * மாறிலி state = calloc(1, அளவு(*state));
    எனில் (!state)
        திரும்பு வெற்று;

    state->refcnt = 1;
    state->table = xkb_compose_table_ref(table);

    state->flags = flags;
    state->prev_context = 0;
    state->context = 0;

    திரும்பு state;
}

கட்டமைப்பு xkb_compose_state *
xkb_compose_state_ref(கட்டமைப்பு xkb_compose_state *state)
{
    ப்ரதிஜ்ஞா(state->refcnt > 0);
    state->refcnt++;
    திரும்பு state;
}

வெறுமை
xkb_compose_state_unref(கட்டமைப்பு xkb_compose_state *state)
{
    ப்ரதிஜ்ஞா(!state || state->refcnt > 0);
    எனில் (!state || --state->refcnt > 0)
        திரும்பு;

    xkb_compose_table_unref(state->table);
    விடுவி(state);
}

கட்டமைப்பு xkb_compose_table *
xkb_compose_state_get_compose_table(கட்டமைப்பு xkb_compose_state *state)
{
    திரும்பு state->table;
}

எண்ணிக்கை xkb_compose_feed_result
xkb_compose_state_feed(கட்டமைப்பு xkb_compose_state *state, xkb_keysym_t keysym)
{
    uint32_t context;
    மாறிலி கட்டமைப்பு compose_முனை *முனை;

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
    எனில் (xkb_keysym_is_modifier(keysym))
        திரும்பு XKB_COMPOSE_FEED_IGNORED;

    முனை = &darray_item(state->table->nodes, state->context);

    context = (முனை->is_leaf ? 1 : முனை->internal.eqkid);
    எனில் (context == 1 && darray_size(state->table->nodes) == 1)
        context = 0;

    போது (context != 0) {
        முனை = &darray_item(state->table->nodes, context);
        எனில் (keysym < முனை->keysym)
            context = முனை->lokid;
        இல்லையெனில் எனில் (keysym > முனை->keysym)
            context = முனை->hikid;
        இல்லையெனில்
            நிறுத்து;
    }

    state->prev_context = state->context;
    state->context = context;
    திரும்பு XKB_COMPOSE_FEED_ACCEPTED;
}

வெறுமை
xkb_compose_state_reset(கட்டமைப்பு xkb_compose_state *state)
{
    state->prev_context = 0;
    state->context = 0;
}

எண்ணிக்கை xkb_compose_status
xkb_compose_state_get_status(கட்டமைப்பு xkb_compose_state *state)
{
    மாறிலி கட்டமைப்பு compose_முனை *prev_முனை, *முனை;

    prev_முனை = &darray_item(state->table->nodes, state->prev_context);
    முனை = &darray_item(state->table->nodes, state->context);

    எனில் (state->context == 0 && !prev_முனை->is_leaf)
        திரும்பு XKB_COMPOSE_CANCELLED;

    எனில் (state->context == 0)
        திரும்பு XKB_COMPOSE_NOTHING;

    எனில் (!முனை->is_leaf)
        திரும்பு XKB_COMPOSE_COMPOSING;

    திரும்பு XKB_COMPOSE_COMPOSED;
}

முழு
xkb_compose_state_get_utf8(கட்டமைப்பு xkb_compose_state *state,
                           எழுத்து *buffer, size_t size)
{
    மாறிலி கட்டமைப்பு compose_முனை *முனை =
        &darray_item(state->table->nodes, state->context);

    எனில் (!முனை->is_leaf)
        செல் fail;

    /* If there's no string specified, but only a keysym, try to do the
     * most helpful thing. */
    எனில் (முனை->leaf.utf8 == 0 && முனை->leaf.keysym != XKB_சாவி_No_குறியீடு) {
        எழுத்து utf8[XKB_KEYSYM_UTF8_MAX_SIZE];
        முழு ret;

        ret = xkb_keysym_to_utf8(முனை->leaf.keysym, utf8, அளவு(utf8));
        எனில் (ret < 0 || ret == 0) {
            /* ret < 0 is impossible.
             * ret == 0 means the keysym has no string representation. */
            செல் fail;
        }

        திரும்பு snprintf(buffer, size, "%s", utf8);
    }

    திரும்பு snprintf(buffer, size, "%s",
                    &darray_item(state->table->utf8, முனை->leaf.utf8));

fail:
    எனில் (size > 0)
        buffer[0] = '\0';
    திரும்பு 0;
}

xkb_keysym_t
xkb_compose_state_get_one_sym(கட்டமைப்பு xkb_compose_state *state)
{
    மாறிலி கட்டமைப்பு compose_முனை *முனை =
        &darray_item(state->table->nodes, state->context);
    எனில் (!முனை->is_leaf)
        திரும்பு XKB_சாவி_No_குறியீடு;
    திரும்பு முனை->leaf.keysym;
}
