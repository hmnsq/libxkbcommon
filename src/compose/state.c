/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>

#अन्तर्भाव "context.h"
#अन्तर्भाव "features/enums.h"
#अन्तर्भाव "keysym.h"
#अन्तर्भाव "table.h"
#अन्तर्भाव "utils.h"

संरचना xkb_compose_state {
    पूर्णांक refcnt;
    गणना xkb_compose_state_flags flags;
    संरचना xkb_compose_table *table;

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

संरचना xkb_compose_state *
xkb_compose_state_new(संरचना xkb_compose_table *table,
                      गणना xkb_compose_state_flags flags)
{
    स्थिरांक गणना xkb_compose_state_flags invalid_flags =
        (flags & ~(गणना xkb_compose_state_flags)XKB_COMPOSE_STATE_FLAGS_मूल्ये);
    जर (invalid_flags) {
        log_err_func(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported compose state flags: %#x\n",
                     invalid_flags);
        परत रिक्त;
    }

    संरचना xkb_compose_state * स्थिरांक state = सुस्मृति(1, आकार(*state));
    जर (!state)
        परत रिक्त;

    state->refcnt = 1;
    state->table = xkb_compose_table_ref(table);

    state->flags = flags;
    state->prev_context = 0;
    state->context = 0;

    परत state;
}

संरचना xkb_compose_state *
xkb_compose_state_ref(संरचना xkb_compose_state *state)
{
    प्रतिज्ञा(state->refcnt > 0);
    state->refcnt++;
    परत state;
}

शून्य
xkb_compose_state_unref(संरचना xkb_compose_state *state)
{
    प्रतिज्ञा(!state || state->refcnt > 0);
    जर (!state || --state->refcnt > 0)
        परत;

    xkb_compose_table_unref(state->table);
    मुक्त(state);
}

संरचना xkb_compose_table *
xkb_compose_state_get_compose_table(संरचना xkb_compose_state *state)
{
    परत state->table;
}

गणना xkb_compose_feed_result
xkb_compose_state_feed(संरचना xkb_compose_state *state, xkb_keysym_t keysym)
{
    uint32_t context;
    स्थिरांक संरचना compose_गाठ *गाठ;

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
    जर (xkb_keysym_is_modifier(keysym))
        परत XKB_COMPOSE_FEED_IGNORED;

    गाठ = &darray_item(state->table->nodes, state->context);

    context = (गाठ->is_leaf ? 1 : गाठ->internal.eqkid);
    जर (context == 1 && darray_size(state->table->nodes) == 1)
        context = 0;

    पर्यंत (context != 0) {
        गाठ = &darray_item(state->table->nodes, context);
        जर (keysym < गाठ->keysym)
            context = गाठ->lokid;
        अन्यथा जर (keysym > गाठ->keysym)
            context = गाठ->hikid;
        अन्यथा
            थांबा;
    }

    state->prev_context = state->context;
    state->context = context;
    परत XKB_COMPOSE_FEED_ACCEPTED;
}

शून्य
xkb_compose_state_reset(संरचना xkb_compose_state *state)
{
    state->prev_context = 0;
    state->context = 0;
}

गणना xkb_compose_status
xkb_compose_state_get_status(संरचना xkb_compose_state *state)
{
    स्थिरांक संरचना compose_गाठ *prev_गाठ, *गाठ;

    prev_गाठ = &darray_item(state->table->nodes, state->prev_context);
    गाठ = &darray_item(state->table->nodes, state->context);

    जर (state->context == 0 && !prev_गाठ->is_leaf)
        परत XKB_COMPOSE_CANCELLED;

    जर (state->context == 0)
        परत XKB_COMPOSE_NOTHING;

    जर (!गाठ->is_leaf)
        परत XKB_COMPOSE_COMPOSING;

    परत XKB_COMPOSE_COMPOSED;
}

पूर्णांक
xkb_compose_state_get_utf8(संरचना xkb_compose_state *state,
                           अक्षर *buffer, माप_प्रकार size)
{
    स्थिरांक संरचना compose_गाठ *गाठ =
        &darray_item(state->table->nodes, state->context);

    जर (!गाठ->is_leaf)
        जा fail;

    /* If there's no string specified, but only a keysym, try to do the
     * most helpful thing. */
    जर (गाठ->leaf.utf8 == 0 && गाठ->leaf.keysym != XKB_किल्ली_No_चिन्ह) {
        अक्षर utf8[XKB_KEYSYM_UTF8_MAX_SIZE];
        पूर्णांक ret;

        ret = xkb_keysym_to_utf8(गाठ->leaf.keysym, utf8, आकार(utf8));
        जर (ret < 0 || ret == 0) {
            /* ret < 0 is impossible.
             * ret == 0 means the keysym has no string representation. */
            जा fail;
        }

        परत snprintf(buffer, size, "%s", utf8);
    }

    परत snprintf(buffer, size, "%s",
                    &darray_item(state->table->utf8, गाठ->leaf.utf8));

fail:
    जर (size > 0)
        buffer[0] = '\0';
    परत 0;
}

xkb_keysym_t
xkb_compose_state_get_one_sym(संरचना xkb_compose_state *state)
{
    स्थिरांक संरचना compose_गाठ *गाठ =
        &darray_item(state->table->nodes, state->context);
    जर (!गाठ->is_leaf)
        परत XKB_किल्ली_No_चिन्ह;
    परत गाठ->leaf.keysym;
}
