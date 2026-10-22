/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>

#ଅନ୍ତର୍ଭାଵ "context.h"
#ଅନ୍ତର୍ଭାଵ "features/enums.h"
#ଅନ୍ତର୍ଭାଵ "keysym.h"
#ଅନ୍ତର୍ଭାଵ "table.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"

ସଂରଚନା xkb_compose_state {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା refcnt;
    ଗଣନା xkb_compose_state_flags flags;
    ସଂରଚନା xkb_compose_table *table;

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

ସଂରଚନା xkb_compose_state *
xkb_compose_state_new(ସଂରଚନା xkb_compose_table *table,
                      ଗଣନା xkb_compose_state_flags flags)
{
    ଅଚଳ ଗଣନା xkb_compose_state_flags invalid_flags =
        (flags & ~(ଗଣନା xkb_compose_state_flags)XKB_COMPOSE_STATE_FLAGS_ମୂଲ୍ୟଗୁଡ଼ିକ);
    ଯଦି (invalid_flags) {
        log_err_func(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported compose state flags: %#x\n",
                     invalid_flags);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ସଂରଚନା xkb_compose_state * ଅଚଳ state = calloc(1, ଆକାର(*state));
    ଯଦି (!state)
        ଫେରନ୍ତୁ ଖାଲି;

    state->refcnt = 1;
    state->table = xkb_compose_table_ref(table);

    state->flags = flags;
    state->prev_context = 0;
    state->context = 0;

    ଫେରନ୍ତୁ state;
}

ସଂରଚନା xkb_compose_state *
xkb_compose_state_ref(ସଂରଚନା xkb_compose_state *state)
{
    ପ୍ରତିଜ୍ଞା(state->refcnt > 0);
    state->refcnt++;
    ଫେରନ୍ତୁ state;
}

ଶୂନ୍ୟ
xkb_compose_state_unref(ସଂରଚନା xkb_compose_state *state)
{
    ପ୍ରତିଜ୍ଞା(!state || state->refcnt > 0);
    ଯଦି (!state || --state->refcnt > 0)
        ଫେରନ୍ତୁ;

    xkb_compose_table_unref(state->table);
    ମୁକ୍ତ(state);
}

ସଂରଚନା xkb_compose_table *
xkb_compose_state_get_compose_table(ସଂରଚନା xkb_compose_state *state)
{
    ଫେରନ୍ତୁ state->table;
}

ଗଣନା xkb_compose_feed_result
xkb_compose_state_feed(ସଂରଚନା xkb_compose_state *state, xkb_keysym_t keysym)
{
    uint32_t context;
    ଅଚଳ ସଂରଚନା compose_ଗଣ୍ଠି *ଗଣ୍ଠି;

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
    ଯଦି (xkb_keysym_is_modifier(keysym))
        ଫେରନ୍ତୁ XKB_COMPOSE_FEED_IGNORED;

    ଗଣ୍ଠି = &darray_item(state->table->nodes, state->context);

    context = (ଗଣ୍ଠି->is_leaf ? 1 : ଗଣ୍ଠି->internal.eqkid);
    ଯଦି (context == 1 && darray_size(state->table->nodes) == 1)
        context = 0;

    ଯେତେବେଳେ (context != 0) {
        ଗଣ୍ଠି = &darray_item(state->table->nodes, context);
        ଯଦି (keysym < ଗଣ୍ଠି->keysym)
            context = ଗଣ୍ଠି->lokid;
        ଅନ୍ୟଥା ଯଦି (keysym > ଗଣ୍ଠି->keysym)
            context = ଗଣ୍ଠି->hikid;
        ଅନ୍ୟଥା
            ଭାଙ୍ଗ;
    }

    state->prev_context = state->context;
    state->context = context;
    ଫେରନ୍ତୁ XKB_COMPOSE_FEED_ACCEPTED;
}

ଶୂନ୍ୟ
xkb_compose_state_reset(ସଂରଚନା xkb_compose_state *state)
{
    state->prev_context = 0;
    state->context = 0;
}

ଗଣନା xkb_compose_status
xkb_compose_state_get_status(ସଂରଚନା xkb_compose_state *state)
{
    ଅଚଳ ସଂରଚନା compose_ଗଣ୍ଠି *prev_ଗଣ୍ଠି, *ଗଣ୍ଠି;

    prev_ଗଣ୍ଠି = &darray_item(state->table->nodes, state->prev_context);
    ଗଣ୍ଠି = &darray_item(state->table->nodes, state->context);

    ଯଦି (state->context == 0 && !prev_ଗଣ୍ଠି->is_leaf)
        ଫେରନ୍ତୁ XKB_COMPOSE_CANCELLED;

    ଯଦି (state->context == 0)
        ଫେରନ୍ତୁ XKB_COMPOSE_NOTHING;

    ଯଦି (!ଗଣ୍ଠି->is_leaf)
        ଫେରନ୍ତୁ XKB_COMPOSE_COMPOSING;

    ଫେରନ୍ତୁ XKB_COMPOSE_COMPOSED;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
xkb_compose_state_get_utf8(ସଂରଚନା xkb_compose_state *state,
                           ଅକ୍ଷର *buffer, size_t size)
{
    ଅଚଳ ସଂରଚନା compose_ଗଣ୍ଠି *ଗଣ୍ଠି =
        &darray_item(state->table->nodes, state->context);

    ଯଦି (!ଗଣ୍ଠି->is_leaf)
        ଯାଅ fail;

    /* If there's no string specified, but only a keysym, try to do the
     * most helpful thing. */
    ଯଦି (ଗଣ୍ଠି->leaf.utf8 == 0 && ଗଣ୍ଠି->leaf.keysym != XKB_ଚାବି_No_ପ୍ରତୀକ) {
        ଅକ୍ଷର utf8[XKB_KEYSYM_UTF8_MAX_SIZE];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

        ret = xkb_keysym_to_utf8(ଗଣ୍ଠି->leaf.keysym, utf8, ଆକାର(utf8));
        ଯଦି (ret < 0 || ret == 0) {
            /* ret < 0 is impossible.
             * ret == 0 means the keysym has no string representation. */
            ଯାଅ fail;
        }

        ଫେରନ୍ତୁ snprintf(buffer, size, "%s", utf8);
    }

    ଫେରନ୍ତୁ snprintf(buffer, size, "%s",
                    &darray_item(state->table->utf8, ଗଣ୍ଠି->leaf.utf8));

fail:
    ଯଦି (size > 0)
        buffer[0] = '\0';
    ଫେରନ୍ତୁ 0;
}

xkb_keysym_t
xkb_compose_state_get_one_sym(ସଂରଚନା xkb_compose_state *state)
{
    ଅଚଳ ସଂରଚନା compose_ଗଣ୍ଠି *ଗଣ୍ଠି =
        &darray_item(state->table->nodes, state->context);
    ଯଦି (!ଗଣ୍ଠି->is_leaf)
        ଫେରନ୍ତୁ XKB_ଚାବି_No_ପ୍ରତୀକ;
    ଫେରନ୍ତୁ ଗଣ୍ଠି->leaf.keysym;
}
