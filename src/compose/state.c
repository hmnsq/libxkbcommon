/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#include "config.h"

#include <assert.h>

#include "context.h"
#include "features/enums.h"
#include "keysym.h"
#include "table.h"
#include "utils.h"

struct xkb_compose_state {
    số_nguyên refcnt;
    enum xkb_compose_state_flags flags;
    struct xkb_compose_table *table;

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

struct xkb_compose_state *
xkb_compose_state_new(struct xkb_compose_table *table,
                      enum xkb_compose_state_flags flags)
{
    const enum xkb_compose_state_flags invalid_flags =
        (flags & ~(enum xkb_compose_state_flags)XKB_COMPOSE_STATE_FLAGS_VALUES);
    nếu (invalid_flags) {
        log_err_func(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported compose state flags: %#x\n",
                     invalid_flags);
        trả_về NULL;
    }

    struct xkb_compose_state * const state = calloc(1, sizeof(*state));
    nếu (!state)
        trả_về NULL;

    state->refcnt = 1;
    state->table = xkb_compose_table_ref(table);

    state->flags = flags;
    state->prev_context = 0;
    state->context = 0;

    trả_về state;
}

struct xkb_compose_state *
xkb_compose_state_ref(struct xkb_compose_state *state)
{
    assert(state->refcnt > 0);
    state->refcnt++;
    trả_về state;
}

void
xkb_compose_state_unref(struct xkb_compose_state *state)
{
    assert(!state || state->refcnt > 0);
    nếu (!state || --state->refcnt > 0)
        trả_về;

    xkb_compose_table_unref(state->table);
    free(state);
}

struct xkb_compose_table *
xkb_compose_state_get_compose_table(struct xkb_compose_state *state)
{
    trả_về state->table;
}

enum xkb_compose_feed_result
xkb_compose_state_feed(struct xkb_compose_state *state, xkb_keysym_t keysym)
{
    uint32_t context;
    const struct compose_node *node;

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
    nếu (xkb_keysym_is_modifier(keysym))
        trả_về XKB_COMPOSE_FEED_IGNORED;

    node = &darray_item(state->table->nodes, state->context);

    context = (node->is_leaf ? 1 : node->internal.eqkid);
    nếu (context == 1 && darray_size(state->table->nodes) == 1)
        context = 0;

    trong_khi (context != 0) {
        node = &darray_item(state->table->nodes, context);
        nếu (keysym < node->keysym)
            context = node->lokid;
        không_thì nếu (keysym > node->keysym)
            context = node->hikid;
        không_thì
            break;
    }

    state->prev_context = state->context;
    state->context = context;
    trả_về XKB_COMPOSE_FEED_ACCEPTED;
}

void
xkb_compose_state_reset(struct xkb_compose_state *state)
{
    state->prev_context = 0;
    state->context = 0;
}

enum xkb_compose_status
xkb_compose_state_get_status(struct xkb_compose_state *state)
{
    const struct compose_node *prev_node, *node;

    prev_node = &darray_item(state->table->nodes, state->prev_context);
    node = &darray_item(state->table->nodes, state->context);

    nếu (state->context == 0 && !prev_node->is_leaf)
        trả_về XKB_COMPOSE_CANCELLED;

    nếu (state->context == 0)
        trả_về XKB_COMPOSE_NOTHING;

    nếu (!node->is_leaf)
        trả_về XKB_COMPOSE_COMPOSING;

    trả_về XKB_COMPOSE_COMPOSED;
}

số_nguyên
xkb_compose_state_get_utf8(struct xkb_compose_state *state,
                           char *buffer, size_t size)
{
    const struct compose_node *node =
        &darray_item(state->table->nodes, state->context);

    nếu (!node->is_leaf)
        goto fail;

    /* If there's no string specified, but only a keysym, try to do the
     * most helpful thing. */
    nếu (node->leaf.utf8 == 0 && node->leaf.keysym != XKB_KEY_NoSymbol) {
        char utf8[XKB_KEYSYM_UTF8_MAX_SIZE];
        số_nguyên ret;

        ret = xkb_keysym_to_utf8(node->leaf.keysym, utf8, sizeof(utf8));
        nếu (ret < 0 || ret == 0) {
            /* ret < 0 is impossible.
             * ret == 0 means the keysym has no string representation. */
            goto fail;
        }

        trả_về snprintf(buffer, size, "%s", utf8);
    }

    trả_về snprintf(buffer, size, "%s",
                    &darray_item(state->table->utf8, node->leaf.utf8));

fail:
    nếu (size > 0)
        buffer[0] = '\0';
    trả_về 0;
}

xkb_keysym_t
xkb_compose_state_get_one_sym(struct xkb_compose_state *state)
{
    const struct compose_node *node =
        &darray_item(state->table->nodes, state->context);
    nếu (!node->is_leaf)
        trả_về XKB_KEY_NoSymbol;
    trả_về node->leaf.keysym;
}
