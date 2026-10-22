/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل <assert.h>

#شامل "context.h"
#شامل "features/enums.h"
#شامل "keysym.h"
#شامل "table.h"
#شامل "utils.h"

ساختار xkb_compose_state {
    صحیح refcnt;
    شمارش xkb_compose_state_flags flags;
    ساختار xkb_compose_table *table;

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

ساختار xkb_compose_state *
xkb_compose_state_new(ساختار xkb_compose_table *table,
                      شمارش xkb_compose_state_flags flags)
{
    ثابت شمارش xkb_compose_state_flags invalid_flags =
        (flags & ~(شمارش xkb_compose_state_flags)XKB_COMPOSE_STATE_FLAGS_مقادیر);
    اگر (invalid_flags) {
        log_err_func(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported compose state flags: %#x\n",
                     invalid_flags);
        بازگشت پوچ;
    }

    ساختار xkb_compose_state * ثابت state = calloc(1, اندازه(*state));
    اگر (!state)
        بازگشت پوچ;

    state->refcnt = 1;
    state->table = xkb_compose_table_ref(table);

    state->flags = flags;
    state->prev_context = 0;
    state->context = 0;

    بازگشت state;
}

ساختار xkb_compose_state *
xkb_compose_state_ref(ساختار xkb_compose_state *state)
{
    أكد(state->refcnt > 0);
    state->refcnt++;
    بازگشت state;
}

تهی
xkb_compose_state_unref(ساختار xkb_compose_state *state)
{
    أكد(!state || state->refcnt > 0);
    اگر (!state || --state->refcnt > 0)
        بازگشت;

    xkb_compose_table_unref(state->table);
    آزاد(state);
}

ساختار xkb_compose_table *
xkb_compose_state_get_compose_table(ساختار xkb_compose_state *state)
{
    بازگشت state->table;
}

شمارش xkb_compose_feed_result
xkb_compose_state_feed(ساختار xkb_compose_state *state, xkb_keysym_t keysym)
{
    uint32_t context;
    ثابت ساختار compose_گره *گره;

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
    اگر (xkb_keysym_is_modifier(keysym))
        بازگشت XKB_COMPOSE_FEED_IGNORED;

    گره = &darray_item(state->table->nodes, state->context);

    context = (گره->is_leaf ? 1 : گره->internal.eqkid);
    اگر (context == 1 && darray_size(state->table->nodes) == 1)
        context = 0;

    وقتی (context != 0) {
        گره = &darray_item(state->table->nodes, context);
        اگر (keysym < گره->keysym)
            context = گره->lokid;
        وگرنه اگر (keysym > گره->keysym)
            context = گره->hikid;
        وگرنه
            بشکن;
    }

    state->prev_context = state->context;
    state->context = context;
    بازگشت XKB_COMPOSE_FEED_ACCEPTED;
}

تهی
xkb_compose_state_reset(ساختار xkb_compose_state *state)
{
    state->prev_context = 0;
    state->context = 0;
}

شمارش xkb_compose_status
xkb_compose_state_get_status(ساختار xkb_compose_state *state)
{
    ثابت ساختار compose_گره *prev_گره, *گره;

    prev_گره = &darray_item(state->table->nodes, state->prev_context);
    گره = &darray_item(state->table->nodes, state->context);

    اگر (state->context == 0 && !prev_گره->is_leaf)
        بازگشت XKB_COMPOSE_CANCELLED;

    اگر (state->context == 0)
        بازگشت XKB_COMPOSE_NOTHING;

    اگر (!گره->is_leaf)
        بازگشت XKB_COMPOSE_COMPOSING;

    بازگشت XKB_COMPOSE_COMPOSED;
}

صحیح
xkb_compose_state_get_utf8(ساختار xkb_compose_state *state,
                           نویسه *buffer, size_t size)
{
    ثابت ساختار compose_گره *گره =
        &darray_item(state->table->nodes, state->context);

    اگر (!گره->is_leaf)
        برو fail;

    /* If there's no string specified, but only a keysym, try to do the
     * most helpful thing. */
    اگر (گره->leaf.utf8 == 0 && گره->leaf.keysym != XKB_کلید_No_نماد) {
        نویسه utf8[XKB_KEYSYM_UTF8_MAX_SIZE];
        صحیح ret;

        ret = xkb_keysym_to_utf8(گره->leaf.keysym, utf8, اندازه(utf8));
        اگر (ret < 0 || ret == 0) {
            /* ret < 0 is impossible.
             * ret == 0 means the keysym has no string representation. */
            برو fail;
        }

        بازگشت snprintf(buffer, size, "%s", utf8);
    }

    بازگشت snprintf(buffer, size, "%s",
                    &darray_item(state->table->utf8, گره->leaf.utf8));

fail:
    اگر (size > 0)
        buffer[0] = '\0';
    بازگشت 0;
}

xkb_keysym_t
xkb_compose_state_get_one_sym(ساختار xkb_compose_state *state)
{
    ثابت ساختار compose_گره *گره =
        &darray_item(state->table->nodes, state->context);
    اگر (!گره->is_leaf)
        بازگشت XKB_کلید_No_نماد;
    بازگشت گره->leaf.keysym;
}
