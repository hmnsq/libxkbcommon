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
    सचे (invalid_flags) {
        log_err_func(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported compose state flags: %#x\n",
                     invalid_flags);
        पटिनिवत्त रिक्त;
    }

    संरचना xkb_compose_state * ध्रुव state = calloc(1, परिमाण(*state));
    सचे (!state)
        पटिनिवत्त रिक्त;

    state->refcnt = 1;
    state->table = xkb_compose_table_ref(table);

    state->flags = flags;
    state->prev_context = 0;
    state->context = 0;

    पटिनिवत्त state;
}

संरचना xkb_compose_state *
xkb_compose_state_ref(संरचना xkb_compose_state *state)
{
    प्रतिज्ञा(state->refcnt > 0);
    state->refcnt++;
    पटिनिवत्त state;
}

शून्य
xkb_compose_state_unref(संरचना xkb_compose_state *state)
{
    प्रतिज्ञा(!state || state->refcnt > 0);
    सचे (!state || --state->refcnt > 0)
        पटिनिवत्त;

    xkb_compose_table_unref(state->table);
    मुक्त(state);
}

संरचना xkb_compose_table *
xkb_compose_state_get_compose_table(संरचना xkb_compose_state *state)
{
    पटिनिवत्त state->table;
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
    सचे (xkb_keysym_is_modifier(keysym))
        पटिनिवत्त XKB_COMPOSE_FEED_IGNORED;

    ग्रन्थिः = &darray_item(state->table->nodes, state->context);

    context = (ग्रन्थिः->is_leaf ? 1 : ग्रन्थिः->internal.eqkid);
    सचे (context == 1 && darray_size(state->table->nodes) == 1)
        context = 0;

    याव (context != 0) {
        ग्रन्थिः = &darray_item(state->table->nodes, context);
        सचे (keysym < ग्रन्थिः->keysym)
            context = ग्रन्थिः->lokid;
        अञ्ञथा सचे (keysym > ग्रन्थिः->keysym)
            context = ग्रन्थिः->hikid;
        अञ्ञथा
            भङ्ग;
    }

    state->prev_context = state->context;
    state->context = context;
    पटिनिवत्त XKB_COMPOSE_FEED_ACCEPTED;
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

    सचे (state->context == 0 && !prev_ग्रन्थिः->is_leaf)
        पटिनिवत्त XKB_COMPOSE_CANCELLED;

    सचे (state->context == 0)
        पटिनिवत्त XKB_COMPOSE_NOTHING;

    सचे (!ग्रन्थिः->is_leaf)
        पटिनिवत्त XKB_COMPOSE_COMPOSING;

    पटिनिवत्त XKB_COMPOSE_COMPOSED;
}

पूर्णाङ्क
xkb_compose_state_get_utf8(संरचना xkb_compose_state *state,
                           वर्ण *buffer, size_t size)
{
    ध्रुव संरचना compose_ग्रन्थिः *ग्रन्थिः =
        &darray_item(state->table->nodes, state->context);

    सचे (!ग्रन्थिः->is_leaf)
        गच्छ fail;

    /* If there's no string specified, but only a keysym, try to do the
     * most helpful thing. */
    सचे (ग्रन्थिः->leaf.utf8 == 0 && ग्रन्थिः->leaf.keysym != XKB_कुञ्चिका_No_चिह्नम्) {
        वर्ण utf8[XKB_KEYSYM_UTF8_MAX_SIZE];
        पूर्णाङ्क ret;

        ret = xkb_keysym_to_utf8(ग्रन्थिः->leaf.keysym, utf8, परिमाण(utf8));
        सचे (ret < 0 || ret == 0) {
            /* ret < 0 is impossible.
             * ret == 0 means the keysym has no string representation. */
            गच्छ fail;
        }

        पटिनिवत्त snprintf(buffer, size, "%s", utf8);
    }

    पटिनिवत्त snprintf(buffer, size, "%s",
                    &darray_item(state->table->utf8, ग्रन्थिः->leaf.utf8));

fail:
    सचे (size > 0)
        buffer[0] = '\0';
    पटिनिवत्त 0;
}

xkb_keysym_t
xkb_compose_state_get_one_sym(संरचना xkb_compose_state *state)
{
    ध्रुव संरचना compose_ग्रन्थिः *ग्रन्थिः =
        &darray_item(state->table->nodes, state->context);
    सचे (!ग्रन्थिः->is_leaf)
        पटिनिवत्त XKB_कुञ्चिका_No_चिह्नम्;
    पटिनिवत्त ग्रन्थिः->leaf.keysym;
}
