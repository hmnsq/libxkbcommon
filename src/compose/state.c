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
    ᤀᤡ᤺ (invalid_flags) {
        log_err_func(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported compose state flags: %#x\n",
                     invalid_flags);
        ᤀᤡᤴ NULL;
    }

    sa_nracanaa xkb_compose_state * const state = calloc(1, parimaa_na(*state));
    ᤀᤡ᤺ (!state)
        ᤀᤡᤴ NULL;

    state->refcnt = 1;
    state->table = xkb_compose_table_ref(table);

    state->flags = flags;
    state->prev_context = 0;
    state->context = 0;

    ᤀᤡᤴ state;
}

sa_nracanaa xkb_compose_state *
xkb_compose_state_ref(sa_nracanaa xkb_compose_state *state)
{
    assert(state->refcnt > 0);
    state->refcnt++;
    ᤀᤡᤴ state;
}

void
xkb_compose_state_unref(sa_nracanaa xkb_compose_state *state)
{
    assert(!state || state->refcnt > 0);
    ᤀᤡ᤺ (!state || --state->refcnt > 0)
        ᤀᤡᤴ;

    xkb_compose_table_unref(state->table);
    free(state);
}

sa_nracanaa xkb_compose_table *
xkb_compose_state_get_compose_table(sa_nracanaa xkb_compose_state *state)
{
    ᤀᤡᤴ state->table;
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
    ᤀᤡ᤺ (xkb_keysym_is_modifier(keysym))
        ᤀᤡᤴ XKB_COMPOSE_FEED_IGNORED;

    node = &darray_item(state->table->nodes, state->context);

    context = (node->is_leaf ? 1 : node->internal.eqkid);
    ᤀᤡ᤺ (context == 1 && darray_size(state->table->nodes) == 1)
        context = 0;

    ᤀᤠ᤺ (context != 0) {
        node = &darray_item(state->table->nodes, context);
        ᤀᤡ᤺ (keysym < node->keysym)
            context = node->lokid;
        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (keysym > node->keysym)
            context = node->hikid;
        ᤀᤡ᤺ᤆᤡ
            bha_nga;
    }

    state->prev_context = state->context;
    state->context = context;
    ᤀᤡᤴ XKB_COMPOSE_FEED_ACCEPTED;
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

    ᤀᤡ᤺ (state->context == 0 && !prev_node->is_leaf)
        ᤀᤡᤴ XKB_COMPOSE_CANCELLED;

    ᤀᤡ᤺ (state->context == 0)
        ᤀᤡᤴ XKB_COMPOSE_NOTHING;

    ᤀᤡ᤺ (!node->is_leaf)
        ᤀᤡᤴ XKB_COMPOSE_COMPOSING;

    ᤀᤡᤴ XKB_COMPOSE_COMPOSED;
}

puur_naa_nka
xkb_compose_state_get_utf8(sa_nracanaa xkb_compose_state *state,
                           var_na *buffer, size_t size)
{
    const sa_nracanaa compose_node *node =
        &darray_item(state->table->nodes, state->context);

    ᤀᤡ᤺ (!node->is_leaf)
        goto fail;

    /* If there's no string specified, but only a keysym, try to do the
     * most helpful thing. */
    ᤀᤡ᤺ (node->leaf.utf8 == 0 && node->leaf.keysym != XKB_KEY_NoSymbol) {
        var_na utf8[XKB_KEYSYM_UTF8_MAX_SIZE];
        puur_naa_nka ret;

        ret = xkb_keysym_to_utf8(node->leaf.keysym, utf8, parimaa_na(utf8));
        ᤀᤡ᤺ (ret < 0 || ret == 0) {
            /* ret < 0 is impossible.
             * ret == 0 means the keysym has no string representation. */
            goto fail;
        }

        ᤀᤡᤴ snprintf(buffer, size, "%s", utf8);
    }

    ᤀᤡᤴ snprintf(buffer, size, "%s",
                    &darray_item(state->table->utf8, node->leaf.utf8));

fail:
    ᤀᤡ᤺ (size > 0)
        buffer[0] = '\0';
    ᤀᤡᤴ 0;
}

xkb_keysym_t
xkb_compose_state_get_one_sym(sa_nracanaa xkb_compose_state *state)
{
    const sa_nracanaa compose_node *node =
        &darray_item(state->table->nodes, state->context);
    ᤀᤡ᤺ (!node->is_leaf)
        ᤀᤡᤴ XKB_KEY_NoSymbol;
    ᤀᤡᤴ node->leaf.keysym;
}
