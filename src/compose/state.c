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
    संरचना xkb_compose_सारणी *सारणी;

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

संरचना xkb_compose_state *
xkb_compose_state_new(संरचना xkb_compose_सारणी *सारणी,
                      गणना xkb_compose_state_flags flags)
{
    ध्रुव गणना xkb_compose_state_flags invalid_flags =
        (flags & ~(गणना xkb_compose_state_flags)XKB_COMPOSE_STATE_FLAGS_मानानि);
    यदि (invalid_flags) {
        log_err_func(सारणी->ctx, XKB_LOG_सन्देशः_NO_ID,
                     "Unsupported compose state flags: %#x\n",
                     invalid_flags);
        प्रत्यागम रिक्त;
    }

    संरचना xkb_compose_state * ध्रुव state = शून्यस्मृतिं_देहि(1, परिमाण(*state));
    यदि (!state)
        प्रत्यागम रिक्त;

    state->refcnt = 1;
    state->सारणी = xkb_compose_सारणी_ref(सारणी);

    state->flags = flags;
    state->पूर्व_context = 0;
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
    यदि (!state || --state->refcnt > 0)
        प्रत्यागम;

    xkb_compose_सारणी_unref(state->सारणी);
    मुक्त(state);
}

संरचना xkb_compose_सारणी *
xkb_compose_state_get_compose_सारणी(संरचना xkb_compose_state *state)
{
    प्रत्यागम state->सारणी;
}

गणना xkb_compose_feed_परिणाम
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
    यदि (xkb_keysym_is_modifier(keysym))
        प्रत्यागम XKB_COMPOSE_FEED_IGNORED;

    ग्रन्थिः = &darray_item(state->सारणी->nodes, state->context);

    context = (ग्रन्थिः->is_पर्णम् ? 1 : ग्रन्थिः->आन्तरिकम्.eqkid);
    यदि (context == 1 && darray_आकार(state->सारणी->nodes) == 1)
        context = 0;

    यावत् (context != 0) {
        ग्रन्थिः = &darray_item(state->सारणी->nodes, context);
        यदि (keysym < ग्रन्थिः->keysym)
            context = ग्रन्थिः->lokid;
        अन्यथा यदि (keysym > ग्रन्थिः->keysym)
            context = ग्रन्थिः->hikid;
        अन्यथा
            भङ्ग;
    }

    state->पूर्व_context = state->context;
    state->context = context;
    प्रत्यागम XKB_COMPOSE_FEED_ACCEPTED;
}

शून्य
xkb_compose_state_reset(संरचना xkb_compose_state *state)
{
    state->पूर्व_context = 0;
    state->context = 0;
}

गणना xkb_compose_स्थितिः
xkb_compose_state_get_स्थितिः(संरचना xkb_compose_state *state)
{
    ध्रुव संरचना compose_ग्रन्थिः *पूर्व_ग्रन्थिः, *ग्रन्थिः;

    पूर्व_ग्रन्थिः = &darray_item(state->सारणी->nodes, state->पूर्व_context);
    ग्रन्थिः = &darray_item(state->सारणी->nodes, state->context);

    यदि (state->context == 0 && !पूर्व_ग्रन्थिः->is_पर्णम्)
        प्रत्यागम XKB_COMPOSE_CANCELLED;

    यदि (state->context == 0)
        प्रत्यागम XKB_COMPOSE_NOTHING;

    यदि (!ग्रन्थिः->is_पर्णम्)
        प्रत्यागम XKB_COMPOSE_COMPOSING;

    प्रत्यागम XKB_COMPOSE_COMPOSED;
}

पूर्णाङ्क
xkb_compose_state_get_utf8(संरचना xkb_compose_state *state,
                           वर्ण *बफर, आकार_t आकार)
{
    ध्रुव संरचना compose_ग्रन्थिः *ग्रन्थिः =
        &darray_item(state->सारणी->nodes, state->context);

    यदि (!ग्रन्थिः->is_पर्णम्)
        गच्छ fail;

    /* If there's no string specified, but only a keysym, try to do the
     * most helpful thing. */
    यदि (ग्रन्थिः->पर्णम्.utf8 == 0 && ग्रन्थिः->पर्णम्.keysym != XKB_कुञ्चिका_No_चिह्नम्) {
        वर्ण utf8[XKB_KEYSYM_UTF8_MAX_आकार];
        पूर्णाङ्क ret;

        ret = xkb_keysym_गन्तव्यम्_utf8(ग्रन्थिः->पर्णम्.keysym, utf8, परिमाण(utf8));
        यदि (ret < 0 || ret == 0) {
            /* ret < 0 is impossible.
             * ret == 0 means the keysym has no string representation. */
            गच्छ fail;
        }

        प्रत्यागम snprintf(बफर, आकार, "%s", utf8);
    }

    प्रत्यागम snprintf(बफर, आकार, "%s",
                    &darray_item(state->सारणी->utf8, ग्रन्थिः->पर्णम्.utf8));

fail:
    यदि (आकार > 0)
        बफर[0] = '\0';
    प्रत्यागम 0;
}

xkb_keysym_t
xkb_compose_state_get_one_sym(संरचना xkb_compose_state *state)
{
    ध्रुव संरचना compose_ग्रन्थिः *ग्रन्थिः =
        &darray_item(state->सारणी->nodes, state->context);
    यदि (!ग्रन्थिः->is_पर्णम्)
        प्रत्यागम XKB_कुञ्चिका_No_चिह्नम्;
    प्रत्यागम ग्रन्थिः->पर्णम्.keysym;
}
