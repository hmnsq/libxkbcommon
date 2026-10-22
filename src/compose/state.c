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

কাঠামো xkb_compose_state {
    পূর্ণসংখ্যা refcnt;
    গণনা xkb_compose_state_flags flags;
    কাঠামো xkb_compose_table *table;

    /*
     * Offsets into xkb_compose_table::nodes.
     *
     * They maintain the current and previous position in the trie; see
     * xkb_compose_state_feed().
     *
     * This is also sufficient for inferring the current status; see
     * xkb_compose_state_get_status().
     */
    uint32_t পূর্ব_context;
    uint32_t context;
};

কাঠামো xkb_compose_state *
xkb_compose_state_new(কাঠামো xkb_compose_table *table,
                      গণনা xkb_compose_state_flags flags)
{
    ধ্রুবক গণনা xkb_compose_state_flags invalid_flags =
        (flags & ~(গণনা xkb_compose_state_flags)XKB_COMPOSE_STATE_FLAGS_মানসমূহ);
    যদি (invalid_flags) {
        log_err_func(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported compose state flags: %#x\n",
                     invalid_flags);
        ফেরত ফাঁকা;
    }

    কাঠামো xkb_compose_state * ধ্রুবক state = calloc(1, আকার(*state));
    যদি (!state)
        ফেরত ফাঁকা;

    state->refcnt = 1;
    state->table = xkb_compose_table_ref(table);

    state->flags = flags;
    state->পূর্ব_context = 0;
    state->context = 0;

    ফেরত state;
}

কাঠামো xkb_compose_state *
xkb_compose_state_ref(কাঠামো xkb_compose_state *state)
{
    প্রতিজ্ঞা(state->refcnt > 0);
    state->refcnt++;
    ফেরত state;
}

শূন্য
xkb_compose_state_unref(কাঠামো xkb_compose_state *state)
{
    প্রতিজ্ঞা(!state || state->refcnt > 0);
    যদি (!state || --state->refcnt > 0)
        ফেরত;

    xkb_compose_table_unref(state->table);
    মুক্ত(state);
}

কাঠামো xkb_compose_table *
xkb_compose_state_get_compose_table(কাঠামো xkb_compose_state *state)
{
    ফেরত state->table;
}

গণনা xkb_compose_feed_ফলাফল
xkb_compose_state_feed(কাঠামো xkb_compose_state *state, xkb_keysym_t keysym)
{
    uint32_t context;
    ধ্রুবক কাঠামো compose_গ্রন্থি *গ্রন্থি;

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
        ফেরত XKB_COMPOSE_FEED_IGNORED;

    গ্রন্থি = &darray_item(state->table->nodes, state->context);

    context = (গ্রন্থি->is_leaf ? 1 : গ্রন্থি->internal.eqkid);
    যদি (context == 1 && darray_আকার(state->table->nodes) == 1)
        context = 0;

    যখন (context != 0) {
        গ্রন্থি = &darray_item(state->table->nodes, context);
        যদি (keysym < গ্রন্থি->keysym)
            context = গ্রন্থি->lokid;
        অন্যথা যদি (keysym > গ্রন্থি->keysym)
            context = গ্রন্থি->hikid;
        অন্যথা
            বিরতি;
    }

    state->পূর্ব_context = state->context;
    state->context = context;
    ফেরত XKB_COMPOSE_FEED_ACCEPTED;
}

শূন্য
xkb_compose_state_reset(কাঠামো xkb_compose_state *state)
{
    state->পূর্ব_context = 0;
    state->context = 0;
}

গণনা xkb_compose_status
xkb_compose_state_get_status(কাঠামো xkb_compose_state *state)
{
    ধ্রুবক কাঠামো compose_গ্রন্থি *পূর্ব_গ্রন্থি, *গ্রন্থি;

    পূর্ব_গ্রন্থি = &darray_item(state->table->nodes, state->পূর্ব_context);
    গ্রন্থি = &darray_item(state->table->nodes, state->context);

    যদি (state->context == 0 && !পূর্ব_গ্রন্থি->is_leaf)
        ফেরত XKB_COMPOSE_CANCELLED;

    যদি (state->context == 0)
        ফেরত XKB_COMPOSE_NOTHING;

    যদি (!গ্রন্থি->is_leaf)
        ফেরত XKB_COMPOSE_COMPOSING;

    ফেরত XKB_COMPOSE_COMPOSED;
}

পূর্ণসংখ্যা
xkb_compose_state_get_utf8(কাঠামো xkb_compose_state *state,
                           অক্ষর *বাফার, আকার_t আকার_2)
{
    ধ্রুবক কাঠামো compose_গ্রন্থি *গ্রন্থি =
        &darray_item(state->table->nodes, state->context);

    যদি (!গ্রন্থি->is_leaf)
        যাও fail;

    /* If there's no string specified, but only a keysym, try to do the
     * most helpful thing. */
    যদি (গ্রন্থি->leaf.utf8 == 0 && গ্রন্থি->leaf.keysym != XKB_চাবি_No_প্রতীক) {
        অক্ষর utf8[XKB_KEYSYM_UTF8_MAX_আকার];
        পূর্ণসংখ্যা ret;

        ret = xkb_keysym_to_utf8(গ্রন্থি->leaf.keysym, utf8, আকার(utf8));
        যদি (ret < 0 || ret == 0) {
            /* ret < 0 is impossible.
             * ret == 0 means the keysym has no string representation. */
            যাও fail;
        }

        ফেরত snprintf(বাফার, আকার_2, "%s", utf8);
    }

    ফেরত snprintf(বাফার, আকার_2, "%s",
                    &darray_item(state->table->utf8, গ্রন্থি->leaf.utf8));

fail:
    যদি (আকার_2 > 0)
        বাফার[0] = '\0';
    ফেরত 0;
}

xkb_keysym_t
xkb_compose_state_get_one_sym(কাঠামো xkb_compose_state *state)
{
    ধ্রুবক কাঠামো compose_গ্রন্থি *গ্রন্থি =
        &darray_item(state->table->nodes, state->context);
    যদি (!গ্রন্থি->is_leaf)
        ফেরত XKB_চাবি_No_প্রতীক;
    ফেরত গ্রন্থি->leaf.keysym;
}
