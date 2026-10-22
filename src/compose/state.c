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
    पूर्णाङ्क refcnt;
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
    ध्रुव गणना xkb_compose_state_flags invalid_flags =
        (flags & ~(गणना xkb_compose_state_flags)XKB_COMPOSE_STATE_FLAGS_मानानि);
    जइ (invalid_flags) {
        log_err_func(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported compose state flags: %#x\n",
                     invalid_flags);
        प्रत्यागम रिक्त;
    }

    संरचना xkb_compose_state * ध्रुव state = calloc(1, परिमाण(*state));
    जइ (!state)
        प्रत्यागम रिक्त;

    state->refcnt = 1;
    state->table = xkb_compose_table_ref(table);

    state->flags = flags;
    state->prev_context = 0;
    state->context = 0;

    प्रत्यागम state;
}

संरचना xkb_compose_state *
xkb_compose_state_ref(संरचना xkb_compose_state *state)
{
    प्रतिज्ञा(state->refcnt > 0);
    state->refcnt++;
    प्रत्यागम state;
}

शून्य
xkb_compose_state_unref(संरचना xkb_compose_state *state)
{
    प्रतिज्ञा(!state || state->refcnt > 0);
    जइ (!state || --state->refcnt > 0)
        प्रत्यागम;

    xkb_compose_table_unref(state->table);
    मुक्त(state);
}

संरचना xkb_compose_table *
xkb_compose_state_get_compose_table(संरचना xkb_compose_state *state)
{
    प्रत्यागम state->table;
}

गणना xkb_compose_feed_result
xkb_compose_state_feed(संरचना xkb_compose_state *state, xkb_keysym_t keysym)
{
    uint32_t context;
    ध्रुव संरचना compose_ग्रन्थिः *ग्रन्थिः;

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
    जइ (xkb_keysym_is_modifier(keysym))
        प्रत्यागम XKB_COMPOSE_FEED_IGNORED;

    ग्रन्थिः = &darray_item(state->table->nodes, state->context);

    context = (ग्रन्थिः->is_leaf ? 1 : ग्रन्थिः->internal.eqkid);
    जइ (context == 1 && darray_size(state->table->nodes) == 1)
        context = 0;

    जाव (context != 0) {
        ग्रन्थिः = &darray_item(state->table->nodes, context);
        जइ (keysym < ग्रन्थिः->keysym)
            context = ग्रन्थिः->lokid;
        अण्णहा जइ (keysym > ग्रन्थिः->keysym)
            context = ग्रन्थिः->hikid;
        अण्णहा
            भङ्ग;
    }

    state->prev_context = state->context;
    state->context = context;
    प्रत्यागम XKB_COMPOSE_FEED_ACCEPTED;
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
    ध्रुव संरचना compose_ग्रन्थिः *prev_ग्रन्थिः, *ग्रन्थिः;

    prev_ग्रन्थिः = &darray_item(state->table->nodes, state->prev_context);
    ग्रन्थिः = &darray_item(state->table->nodes, state->context);

    जइ (state->context == 0 && !prev_ग्रन्थिः->is_leaf)
        प्रत्यागम XKB_COMPOSE_CANCELLED;

    जइ (state->context == 0)
        प्रत्यागम XKB_COMPOSE_NOTHING;

    जइ (!ग्रन्थिः->is_leaf)
        प्रत्यागम XKB_COMPOSE_COMPOSING;

    प्रत्यागम XKB_COMPOSE_COMPOSED;
}

पूर्णाङ्क
xkb_compose_state_get_utf8(संरचना xkb_compose_state *state,
                           वर्ण *buffer, size_t size)
{
    ध्रुव संरचना compose_ग्रन्थिः *ग्रन्थिः =
        &darray_item(state->table->nodes, state->context);

    जइ (!ग्रन्थिः->is_leaf)
        गच्छ fail;

    /* If there's no string specified, but only a keysym, try to do the
     * most helpful thing. */
    जइ (ग्रन्थिः->leaf.utf8 == 0 && ग्रन्थिः->leaf.keysym != XKB_कुञ्चिका_No_चिह्नम्) {
        वर्ण utf8[XKB_KEYSYM_UTF8_MAX_SIZE];
        पूर्णाङ्क ret;

        ret = xkb_keysym_to_utf8(ग्रन्थिः->leaf.keysym, utf8, परिमाण(utf8));
        जइ (ret < 0 || ret == 0) {
            /* ret < 0 is impossible.
             * ret == 0 means the keysym has no string representation. */
            गच्छ fail;
        }

        प्रत्यागम snprintf(buffer, size, "%s", utf8);
    }

    प्रत्यागम snprintf(buffer, size, "%s",
                    &darray_item(state->table->utf8, ग्रन्थिः->leaf.utf8));

fail:
    जइ (size > 0)
        buffer[0] = '\0';
    प्रत्यागम 0;
}

xkb_keysym_t
xkb_compose_state_get_one_sym(संरचना xkb_compose_state *state)
{
    ध्रुव संरचना compose_ग्रन्थिः *ग्रन्थिः =
        &darray_item(state->table->nodes, state->context);
    जइ (!ग्रन्थिः->is_leaf)
        प्रत्यागम XKB_कुञ्चिका_No_चिह्नम्;
    प्रत्यागम ग्रन्थिः->leaf.keysym;
}
