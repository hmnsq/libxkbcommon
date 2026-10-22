/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#समावेश "config.h"

#समावेश <assert.h>

#समावेश "context.h"
#समावेश "features/enums.h"
#समावेश "keysym.h"
#समावेश "table.h"
#समावेश "utils.h"

काष्ठा xkb_compose_state {
    पूर्णांक refcnt;
    क्रमागत xkb_compose_state_flags flags;
    काष्ठा xkb_compose_table *table;

    /*
     * Offsets into xkb_compose_table::nodes.
     *
     * They maintain the current and previous position in the trie; see
     * xkb_compose_state_feed().
     *
     * This is also sufficient for inferring the current status; see
     * xkb_compose_state_get_status().
     */
    uint32_t पूर्व_context;
    uint32_t context;
};

काष्ठा xkb_compose_state *
xkb_compose_state_new(काष्ठा xkb_compose_table *table,
                      क्रमागत xkb_compose_state_flags flags)
{
    स्थिर क्रमागत xkb_compose_state_flags invalid_flags =
        (flags & ~(क्रमागत xkb_compose_state_flags)XKB_COMPOSE_STATE_FLAGS_मूल्यावली);
    अगर (invalid_flags) {
        log_err_func(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported compose state flags: %#x\n",
                     invalid_flags);
        वापस शून्य;
    }

    काष्ठा xkb_compose_state * स्थिर state = सुस्मृति(1, माप(*state));
    अगर (!state)
        वापस शून्य;

    state->refcnt = 1;
    state->table = xkb_compose_table_ref(table);

    state->flags = flags;
    state->पूर्व_context = 0;
    state->context = 0;

    वापस state;
}

काष्ठा xkb_compose_state *
xkb_compose_state_ref(काष्ठा xkb_compose_state *state)
{
    निश्चित(state->refcnt > 0);
    state->refcnt++;
    वापस state;
}

व्योम
xkb_compose_state_unref(काष्ठा xkb_compose_state *state)
{
    निश्चित(!state || state->refcnt > 0);
    अगर (!state || --state->refcnt > 0)
        वापस;

    xkb_compose_table_unref(state->table);
    मुक्त(state);
}

काष्ठा xkb_compose_table *
xkb_compose_state_get_compose_table(काष्ठा xkb_compose_state *state)
{
    वापस state->table;
}

क्रमागत xkb_compose_feed_परिणाम
xkb_compose_state_feed(काष्ठा xkb_compose_state *state, xkb_keysym_t keysym)
{
    uint32_t context;
    स्थिर काष्ठा compose_गाँठ *गाँठ;

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
    अगर (xkb_keysym_is_modifier(keysym))
        वापस XKB_COMPOSE_FEED_IGNORED;

    गाँठ = &darray_item(state->table->nodes, state->context);

    context = (गाँठ->is_leaf ? 1 : गाँठ->internal.eqkid);
    अगर (context == 1 && darray_आकार(state->table->nodes) == 1)
        context = 0;

    जबतक (context != 0) {
        गाँठ = &darray_item(state->table->nodes, context);
        अगर (keysym < गाँठ->keysym)
            context = गाँठ->lokid;
        अन्यथा अगर (keysym > गाँठ->keysym)
            context = गाँठ->hikid;
        अन्यथा
            अवरोध;
    }

    state->पूर्व_context = state->context;
    state->context = context;
    वापस XKB_COMPOSE_FEED_ACCEPTED;
}

व्योम
xkb_compose_state_reset(काष्ठा xkb_compose_state *state)
{
    state->पूर्व_context = 0;
    state->context = 0;
}

क्रमागत xkb_compose_status
xkb_compose_state_get_status(काष्ठा xkb_compose_state *state)
{
    स्थिर काष्ठा compose_गाँठ *पूर्व_गाँठ, *गाँठ;

    पूर्व_गाँठ = &darray_item(state->table->nodes, state->पूर्व_context);
    गाँठ = &darray_item(state->table->nodes, state->context);

    अगर (state->context == 0 && !पूर्व_गाँठ->is_leaf)
        वापस XKB_COMPOSE_CANCELLED;

    अगर (state->context == 0)
        वापस XKB_COMPOSE_NOTHING;

    अगर (!गाँठ->is_leaf)
        वापस XKB_COMPOSE_COMPOSING;

    वापस XKB_COMPOSE_COMPOSED;
}

पूर्णांक
xkb_compose_state_get_utf8(काष्ठा xkb_compose_state *state,
                           अक्षर *बफर, माप_प्रकार आकार)
{
    स्थिर काष्ठा compose_गाँठ *गाँठ =
        &darray_item(state->table->nodes, state->context);

    अगर (!गाँठ->is_leaf)
        जाओ fail;

    /* If there's no string specified, but only a keysym, try to do the
     * most helpful thing. */
    अगर (गाँठ->leaf.utf8 == 0 && गाँठ->leaf.keysym != XKB_कुंजी_No_प्रतीक) {
        अक्षर utf8[XKB_KEYSYM_UTF8_MAX_आकार];
        पूर्णांक ret;

        ret = xkb_keysym_to_utf8(गाँठ->leaf.keysym, utf8, माप(utf8));
        अगर (ret < 0 || ret == 0) {
            /* ret < 0 is impossible.
             * ret == 0 means the keysym has no string representation. */
            जाओ fail;
        }

        वापस snprintf(बफर, आकार, "%s", utf8);
    }

    वापस snprintf(बफर, आकार, "%s",
                    &darray_item(state->table->utf8, गाँठ->leaf.utf8));

fail:
    अगर (आकार > 0)
        बफर[0] = '\0';
    वापस 0;
}

xkb_keysym_t
xkb_compose_state_get_one_sym(काष्ठा xkb_compose_state *state)
{
    स्थिर काष्ठा compose_गाँठ *गाँठ =
        &darray_item(state->table->nodes, state->context);
    अगर (!गाँठ->is_leaf)
        वापस XKB_कुंजी_No_प्रतीक;
    वापस गाँठ->leaf.keysym;
}
