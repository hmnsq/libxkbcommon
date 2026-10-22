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

sa_nracanaa xkb_compose_state {
    puur_naa_nka refcnt;
    ga_nanaa xkb_compose_state_flags flags;
    sa_nracanaa xkb_compose_table *table;

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

sa_nracanaa xkb_compose_state *
xkb_compose_state_new(sa_nracanaa xkb_compose_table *table,
                      ga_nanaa xkb_compose_state_flags flags)
{
    const ga_nanaa xkb_compose_state_flags invalid_flags =
        (flags & ~(ga_nanaa xkb_compose_state_flags)XKB_COMPOSE_STATE_FLAGS_VALUES);
    ꦪꦼꦤ꧀ (invalid_flags) {
        log_err_func(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported compose state flags: %#x\n",
                     invalid_flags);
        ꦮꦭꦶ NULL;
    }

    sa_nracanaa xkb_compose_state * const state = calloc(1, parimaa_na(*state));
    ꦪꦼꦤ꧀ (!state)
        ꦮꦭꦶ NULL;

    state->refcnt = 1;
    state->table = xkb_compose_table_ref(table);

    state->flags = flags;
    state->prev_context = 0;
    state->context = 0;

    ꦮꦭꦶ state;
}

sa_nracanaa xkb_compose_state *
xkb_compose_state_ref(sa_nracanaa xkb_compose_state *state)
{
    assert(state->refcnt > 0);
    state->refcnt++;
    ꦮꦭꦶ state;
}

void
xkb_compose_state_unref(sa_nracanaa xkb_compose_state *state)
{
    assert(!state || state->refcnt > 0);
    ꦪꦼꦤ꧀ (!state || --state->refcnt > 0)
        ꦮꦭꦶ;

    xkb_compose_table_unref(state->table);
    free(state);
}

sa_nracanaa xkb_compose_table *
xkb_compose_state_get_compose_table(sa_nracanaa xkb_compose_state *state)
{
    ꦮꦭꦶ state->table;
}

ga_nanaa xkb_compose_feed_result
xkb_compose_state_feed(sa_nracanaa xkb_compose_state *state, xkb_keysym_t keysym)
{
    uint32_t context;
    const sa_nracanaa compose_node *node;

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
    ꦪꦼꦤ꧀ (xkb_keysym_is_modifier(keysym))
        ꦮꦭꦶ XKB_COMPOSE_FEED_IGNORED;

    node = &darray_item(state->table->nodes, state->context);

    context = (node->is_leaf ? 1 : node->internal.eqkid);
    ꦪꦼꦤ꧀ (context == 1 && darray_size(state->table->nodes) == 1)
        context = 0;

    ꦱꦼꦢꦼꦭ (context != 0) {
        node = &darray_item(state->table->nodes, context);
        ꦪꦼꦤ꧀ (keysym < node->keysym)
            context = node->lokid;
        ꦭꦶꦪ ꦪꦼꦤ꧀ (keysym > node->keysym)
            context = node->hikid;
        ꦭꦶꦪ
            bha_nga;
    }

    state->prev_context = state->context;
    state->context = context;
    ꦮꦭꦶ XKB_COMPOSE_FEED_ACCEPTED;
}

void
xkb_compose_state_reset(sa_nracanaa xkb_compose_state *state)
{
    state->prev_context = 0;
    state->context = 0;
}

ga_nanaa xkb_compose_status
xkb_compose_state_get_status(sa_nracanaa xkb_compose_state *state)
{
    const sa_nracanaa compose_node *prev_node, *node;

    prev_node = &darray_item(state->table->nodes, state->prev_context);
    node = &darray_item(state->table->nodes, state->context);

    ꦪꦼꦤ꧀ (state->context == 0 && !prev_node->is_leaf)
        ꦮꦭꦶ XKB_COMPOSE_CANCELLED;

    ꦪꦼꦤ꧀ (state->context == 0)
        ꦮꦭꦶ XKB_COMPOSE_NOTHING;

    ꦪꦼꦤ꧀ (!node->is_leaf)
        ꦮꦭꦶ XKB_COMPOSE_COMPOSING;

    ꦮꦭꦶ XKB_COMPOSE_COMPOSED;
}

puur_naa_nka
xkb_compose_state_get_utf8(sa_nracanaa xkb_compose_state *state,
                           var_na *buffer, size_t size)
{
    const sa_nracanaa compose_node *node =
        &darray_item(state->table->nodes, state->context);

    ꦪꦼꦤ꧀ (!node->is_leaf)
        goto fail;

    /* If there's no string specified, but only a keysym, try to do the
     * most helpful thing. */
    ꦪꦼꦤ꧀ (node->leaf.utf8 == 0 && node->leaf.keysym != XKB_KEY_NoSymbol) {
        var_na utf8[XKB_KEYSYM_UTF8_MAX_SIZE];
        puur_naa_nka ret;

        ret = xkb_keysym_to_utf8(node->leaf.keysym, utf8, parimaa_na(utf8));
        ꦪꦼꦤ꧀ (ret < 0 || ret == 0) {
            /* ret < 0 is impossible.
             * ret == 0 means the keysym has no string representation. */
            goto fail;
        }

        ꦮꦭꦶ snprintf(buffer, size, "%s", utf8);
    }

    ꦮꦭꦶ snprintf(buffer, size, "%s",
                    &darray_item(state->table->utf8, node->leaf.utf8));

fail:
    ꦪꦼꦤ꧀ (size > 0)
        buffer[0] = '\0';
    ꦮꦭꦶ 0;
}

xkb_keysym_t
xkb_compose_state_get_one_sym(sa_nracanaa xkb_compose_state *state)
{
    const sa_nracanaa compose_node *node =
        &darray_item(state->table->nodes, state->context);
    ꦪꦼꦤ꧀ (!node->is_leaf)
        ꦮꦭꦶ XKB_KEY_NoSymbol;
    ꦮꦭꦶ node->leaf.keysym;
}
