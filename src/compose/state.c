/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>

#അന്തര്ഭാവ "context.h"
#അന്തര്ഭാവ "features/enums.h"
#അന്തര്ഭാവ "keysym.h"
#അന്തര്ഭാവ "table.h"
#അന്തര്ഭാവ "utils.h"

ഘടന xkb_compose_state {
    പൂർണ്ണസംഖ്യ refcnt;
    എണ്ണൽ xkb_compose_state_flags flags;
    ഘടന xkb_compose_table *table;

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

ഘടന xkb_compose_state *
xkb_compose_state_new(ഘടന xkb_compose_table *table,
                      എണ്ണൽ xkb_compose_state_flags flags)
{
    സ്ഥിരം എണ്ണൽ xkb_compose_state_flags invalid_flags =
        (flags & ~(എണ്ണൽ xkb_compose_state_flags)XKB_COMPOSE_STATE_FLAGS_മൂല്യങ്ങൾ);
    എങ്കിൽ (invalid_flags) {
        log_err_func(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported compose state flags: %#x\n",
                     invalid_flags);
        തിരികെ ശൂന്യ;
    }

    ഘടന xkb_compose_state * സ്ഥിരം state = calloc(1, വലുപ്പം(*state));
    എങ്കിൽ (!state)
        തിരികെ ശൂന്യ;

    state->refcnt = 1;
    state->table = xkb_compose_table_ref(table);

    state->flags = flags;
    state->prev_context = 0;
    state->context = 0;

    തിരികെ state;
}

ഘടന xkb_compose_state *
xkb_compose_state_ref(ഘടന xkb_compose_state *state)
{
    പ്രതിജ്ഞാ(state->refcnt > 0);
    state->refcnt++;
    തിരികെ state;
}

ശൂന്യം
xkb_compose_state_unref(ഘടന xkb_compose_state *state)
{
    പ്രതിജ്ഞാ(!state || state->refcnt > 0);
    എങ്കിൽ (!state || --state->refcnt > 0)
        തിരികെ;

    xkb_compose_table_unref(state->table);
    സ്വതന്ത്രം(state);
}

ഘടന xkb_compose_table *
xkb_compose_state_get_compose_table(ഘടന xkb_compose_state *state)
{
    തിരികെ state->table;
}

എണ്ണൽ xkb_compose_feed_result
xkb_compose_state_feed(ഘടന xkb_compose_state *state, xkb_keysym_t keysym)
{
    uint32_t context;
    സ്ഥിരം ഘടന compose_കണ്ണി *കണ്ണി;

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
    എങ്കിൽ (xkb_keysym_is_modifier(keysym))
        തിരികെ XKB_COMPOSE_FEED_IGNORED;

    കണ്ണി = &darray_item(state->table->nodes, state->context);

    context = (കണ്ണി->is_leaf ? 1 : കണ്ണി->internal.eqkid);
    എങ്കിൽ (context == 1 && darray_size(state->table->nodes) == 1)
        context = 0;

    ആകുമ്പോൾ (context != 0) {
        കണ്ണി = &darray_item(state->table->nodes, context);
        എങ്കിൽ (keysym < കണ്ണി->keysym)
            context = കണ്ണി->lokid;
        അല്ലെങ്കിൽ എങ്കിൽ (keysym > കണ്ണി->keysym)
            context = കണ്ണി->hikid;
        അല്ലെങ്കിൽ
            നിർത്തുക;
    }

    state->prev_context = state->context;
    state->context = context;
    തിരികെ XKB_COMPOSE_FEED_ACCEPTED;
}

ശൂന്യം
xkb_compose_state_reset(ഘടന xkb_compose_state *state)
{
    state->prev_context = 0;
    state->context = 0;
}

എണ്ണൽ xkb_compose_status
xkb_compose_state_get_status(ഘടന xkb_compose_state *state)
{
    സ്ഥിരം ഘടന compose_കണ്ണി *prev_കണ്ണി, *കണ്ണി;

    prev_കണ്ണി = &darray_item(state->table->nodes, state->prev_context);
    കണ്ണി = &darray_item(state->table->nodes, state->context);

    എങ്കിൽ (state->context == 0 && !prev_കണ്ണി->is_leaf)
        തിരികെ XKB_COMPOSE_CANCELLED;

    എങ്കിൽ (state->context == 0)
        തിരികെ XKB_COMPOSE_NOTHING;

    എങ്കിൽ (!കണ്ണി->is_leaf)
        തിരികെ XKB_COMPOSE_COMPOSING;

    തിരികെ XKB_COMPOSE_COMPOSED;
}

പൂർണ്ണസംഖ്യ
xkb_compose_state_get_utf8(ഘടന xkb_compose_state *state,
                           അക്ഷരം *buffer, size_t size)
{
    സ്ഥിരം ഘടന compose_കണ്ണി *കണ്ണി =
        &darray_item(state->table->nodes, state->context);

    എങ്കിൽ (!കണ്ണി->is_leaf)
        പോകുക fail;

    /* If there's no string specified, but only a keysym, try to do the
     * most helpful thing. */
    എങ്കിൽ (കണ്ണി->leaf.utf8 == 0 && കണ്ണി->leaf.keysym != XKB_താക്കോൽ_No_ചിഹ്നം) {
        അക്ഷരം utf8[XKB_KEYSYM_UTF8_MAX_SIZE];
        പൂർണ്ണസംഖ്യ ret;

        ret = xkb_keysym_to_utf8(കണ്ണി->leaf.keysym, utf8, വലുപ്പം(utf8));
        എങ്കിൽ (ret < 0 || ret == 0) {
            /* ret < 0 is impossible.
             * ret == 0 means the keysym has no string representation. */
            പോകുക fail;
        }

        തിരികെ snprintf(buffer, size, "%s", utf8);
    }

    തിരികെ snprintf(buffer, size, "%s",
                    &darray_item(state->table->utf8, കണ്ണി->leaf.utf8));

fail:
    എങ്കിൽ (size > 0)
        buffer[0] = '\0';
    തിരികെ 0;
}

xkb_keysym_t
xkb_compose_state_get_one_sym(ഘടന xkb_compose_state *state)
{
    സ്ഥിരം ഘടന compose_കണ്ണി *കണ്ണി =
        &darray_item(state->table->nodes, state->context);
    എങ്കിൽ (!കണ്ണി->is_leaf)
        തിരികെ XKB_താക്കോൽ_No_ചിഹ്നം;
    തിരികെ കണ്ണി->leaf.keysym;
}
