/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <assert.h>

#অন্তর্ভাব "context.h"
#অন্তর্ভাব "features/enums.h"
#অন্তর্ভাব "keysym.h"
#অন্তর্ভাব "table.h"
#অন্তর্ভাব "utils.h"

সংরচনা xkb_compose_state {
    পূর্ণাঙ্ক refcnt;
    গণনা xkb_compose_state_flags flags;
    সংরচনা xkb_compose_table *table;

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

সংরচনা xkb_compose_state *
xkb_compose_state_new(সংরচনা xkb_compose_table *table,
                      গণনা xkb_compose_state_flags flags)
{
    ধ্রুব গণনা xkb_compose_state_flags invalid_flags =
        (flags & ~(গণনা xkb_compose_state_flags)XKB_COMPOSE_STATE_FLAGS_মানসমূহ);
    যদি (invalid_flags) {
        log_err_func(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported compose state flags: %#x\n",
                     invalid_flags);
        প্রত্যাগম রিক্ত;
    }

    সংরচনা xkb_compose_state * ধ্রুব state = calloc(1, পরিমাণ(*state));
    যদি (!state)
        প্রত্যাগম রিক্ত;

    state->refcnt = 1;
    state->table = xkb_compose_table_ref(table);

    state->flags = flags;
    state->prev_context = 0;
    state->context = 0;

    প্রত্যাগম state;
}

সংরচনা xkb_compose_state *
xkb_compose_state_ref(সংরচনা xkb_compose_state *state)
{
    প্রতিজ্ঞা(state->refcnt > 0);
    state->refcnt++;
    প্রত্যাগম state;
}

শূন্য
xkb_compose_state_unref(সংরচনা xkb_compose_state *state)
{
    প্রতিজ্ঞা(!state || state->refcnt > 0);
    যদি (!state || --state->refcnt > 0)
        প্রত্যাগম;

    xkb_compose_table_unref(state->table);
    মুক্ত(state);
}

সংরচনা xkb_compose_table *
xkb_compose_state_get_compose_table(সংরচনা xkb_compose_state *state)
{
    প্রত্যাগম state->table;
}

গণনা xkb_compose_feed_result
xkb_compose_state_feed(সংরচনা xkb_compose_state *state, xkb_keysym_t keysym)
{
    uint32_t context;
    ধ্রুব সংরচনা compose_গ্রন্থি *গ্রন্থি;

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
    যদি (xkb_keysym_is_modifier(keysym))
        প্রত্যাগম XKB_COMPOSE_FEED_IGNORED;

    গ্রন্থি = &darray_item(state->table->nodes, state->context);

    context = (গ্রন্থি->is_leaf ? 1 : গ্রন্থি->internal.eqkid);
    যদি (context == 1 && darray_size(state->table->nodes) == 1)
        context = 0;

    যাবত্ (context != 0) {
        গ্রন্থি = &darray_item(state->table->nodes, context);
        যদি (keysym < গ্রন্থি->keysym)
            context = গ্রন্থি->lokid;
        অন্যথা যদি (keysym > গ্রন্থি->keysym)
            context = গ্রন্থি->hikid;
        অন্যথা
            ভঙ্গ;
    }

    state->prev_context = state->context;
    state->context = context;
    প্রত্যাগম XKB_COMPOSE_FEED_ACCEPTED;
}

শূন্য
xkb_compose_state_reset(সংরচনা xkb_compose_state *state)
{
    state->prev_context = 0;
    state->context = 0;
}

গণনা xkb_compose_status
xkb_compose_state_get_status(সংরচনা xkb_compose_state *state)
{
    ধ্রুব সংরচনা compose_গ্রন্থি *prev_গ্রন্থি, *গ্রন্থি;

    prev_গ্রন্থি = &darray_item(state->table->nodes, state->prev_context);
    গ্রন্থি = &darray_item(state->table->nodes, state->context);

    যদি (state->context == 0 && !prev_গ্রন্থি->is_leaf)
        প্রত্যাগম XKB_COMPOSE_CANCELLED;

    যদি (state->context == 0)
        প্রত্যাগম XKB_COMPOSE_NOTHING;

    যদি (!গ্রন্থি->is_leaf)
        প্রত্যাগম XKB_COMPOSE_COMPOSING;

    প্রত্যাগম XKB_COMPOSE_COMPOSED;
}

পূর্ণাঙ্ক
xkb_compose_state_get_utf8(সংরচনা xkb_compose_state *state,
                           বর্ণ *buffer, size_t size)
{
    ধ্রুব সংরচনা compose_গ্রন্থি *গ্রন্থি =
        &darray_item(state->table->nodes, state->context);

    যদি (!গ্রন্থি->is_leaf)
        গচ্ছ fail;

    /* If there's no string specified, but only a keysym, try to do the
     * most helpful thing. */
    যদি (গ্রন্থি->leaf.utf8 == 0 && গ্রন্থি->leaf.keysym != XKB_চাবি_No_প্রতীক) {
        বর্ণ utf8[XKB_KEYSYM_UTF8_MAX_SIZE];
        পূর্ণাঙ্ক ret;

        ret = xkb_keysym_to_utf8(গ্রন্থি->leaf.keysym, utf8, পরিমাণ(utf8));
        যদি (ret < 0 || ret == 0) {
            /* ret < 0 is impossible.
             * ret == 0 means the keysym has no string representation. */
            গচ্ছ fail;
        }

        প্রত্যাগম snprintf(buffer, size, "%s", utf8);
    }

    প্রত্যাগম snprintf(buffer, size, "%s",
                    &darray_item(state->table->utf8, গ্রন্থি->leaf.utf8));

fail:
    যদি (size > 0)
        buffer[0] = '\0';
    প্রত্যাগম 0;
}

xkb_keysym_t
xkb_compose_state_get_one_sym(সংরচনা xkb_compose_state *state)
{
    ধ্রুব সংরচনা compose_গ্রন্থি *গ্রন্থি =
        &darray_item(state->table->nodes, state->context);
    যদি (!গ্রন্থি->is_leaf)
        প্রত্যাগম XKB_চাবি_No_প্রতীক;
    প্রত্যাগম গ্রন্থি->leaf.keysym;
}
