/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن <assert.h>

#ضمن "context.h"
#ضمن "features/enums.h"
#ضمن "keysym.h"
#ضمن "table.h"
#ضمن "utils.h"

بنية xkb_compose_state {
    صحيح refcnt;
    تعداد xkb_compose_state_flags flags;
    بنية xkb_compose_table *table;

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

بنية xkb_compose_state *
xkb_compose_state_new(بنية xkb_compose_table *table,
                      تعداد xkb_compose_state_flags flags)
{
    ثابت تعداد xkb_compose_state_flags invalid_flags =
        (flags & ~(تعداد xkb_compose_state_flags)XKB_COMPOSE_STATE_FLAGS_VALUES);
    𐤀𐤌 (invalid_flags) {
        log_err_func(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported compose state flags: %#x\n",
                     invalid_flags);
        أرجع عدم;
    }

    بنية xkb_compose_state * ثابت state = calloc(1, حجم(*state));
    𐤀𐤌 (!state)
        أرجع عدم;

    state->refcnt = 1;
    state->table = xkb_compose_table_ref(table);

    state->flags = flags;
    state->prev_context = 0;
    state->context = 0;

    أرجع state;
}

بنية xkb_compose_state *
xkb_compose_state_ref(بنية xkb_compose_state *state)
{
    أكد(state->refcnt > 0);
    state->refcnt++;
    أرجع state;
}

فراغ
xkb_compose_state_unref(بنية xkb_compose_state *state)
{
    أكد(!state || state->refcnt > 0);
    𐤀𐤌 (!state || --state->refcnt > 0)
        أرجع;

    xkb_compose_table_unref(state->table);
    حرر(state);
}

بنية xkb_compose_table *
xkb_compose_state_get_compose_table(بنية xkb_compose_state *state)
{
    أرجع state->table;
}

تعداد xkb_compose_feed_result
xkb_compose_state_feed(بنية xkb_compose_state *state, xkb_keysym_t keysym)
{
    uint32_t context;
    ثابت بنية compose_node *node;

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
    𐤀𐤌 (xkb_keysym_is_modifier(keysym))
        أرجع XKB_COMPOSE_FEED_IGNORED;

    node = &darray_item(state->table->nodes, state->context);

    context = (node->is_leaf ? 1 : node->internal.eqkid);
    𐤀𐤌 (context == 1 && darray_size(state->table->nodes) == 1)
        context = 0;

    طالما (context != 0) {
        node = &darray_item(state->table->nodes, context);
        𐤀𐤌 (keysym < node->keysym)
            context = node->lokid;
        وإلا 𐤀𐤌 (keysym > node->keysym)
            context = node->hikid;
        وإلا
            اكسر;
    }

    state->prev_context = state->context;
    state->context = context;
    أرجع XKB_COMPOSE_FEED_ACCEPTED;
}

فراغ
xkb_compose_state_reset(بنية xkb_compose_state *state)
{
    state->prev_context = 0;
    state->context = 0;
}

تعداد xkb_compose_status
xkb_compose_state_get_status(بنية xkb_compose_state *state)
{
    ثابت بنية compose_node *prev_node, *node;

    prev_node = &darray_item(state->table->nodes, state->prev_context);
    node = &darray_item(state->table->nodes, state->context);

    𐤀𐤌 (state->context == 0 && !prev_node->is_leaf)
        أرجع XKB_COMPOSE_CANCELLED;

    𐤀𐤌 (state->context == 0)
        أرجع XKB_COMPOSE_NOTHING;

    𐤀𐤌 (!node->is_leaf)
        أرجع XKB_COMPOSE_COMPOSING;

    أرجع XKB_COMPOSE_COMPOSED;
}

صحيح
xkb_compose_state_get_utf8(بنية xkb_compose_state *state,
                           حرف *buffer, size_t size)
{
    ثابت بنية compose_node *node =
        &darray_item(state->table->nodes, state->context);

    𐤀𐤌 (!node->is_leaf)
        اذهب fail;

    /* If there's no string specified, but only a keysym, try to do the
     * most helpful thing. */
    𐤀𐤌 (node->leaf.utf8 == 0 && node->leaf.keysym != XKB_KEY_NoSymbol) {
        حرف utf8[XKB_KEYSYM_UTF8_MAX_SIZE];
        صحيح ret;

        ret = xkb_keysym_to_utf8(node->leaf.keysym, utf8, حجم(utf8));
        𐤀𐤌 (ret < 0 || ret == 0) {
            /* ret < 0 is impossible.
             * ret == 0 means the keysym has no string representation. */
            اذهب fail;
        }

        أرجع snprintf(buffer, size, "%s", utf8);
    }

    أرجع snprintf(buffer, size, "%s",
                    &darray_item(state->table->utf8, node->leaf.utf8));

fail:
    𐤀𐤌 (size > 0)
        buffer[0] = '\0';
    أرجع 0;
}

xkb_keysym_t
xkb_compose_state_get_one_sym(بنية xkb_compose_state *state)
{
    ثابت بنية compose_node *node =
        &darray_item(state->table->nodes, state->context);
    𐤀𐤌 (!node->is_leaf)
        أرجع XKB_KEY_NoSymbol;
    أرجع node->leaf.keysym;
}
