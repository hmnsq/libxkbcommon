/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "context.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "features/enums.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keysym.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "table.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

ਬਣਤਰ xkb_compose_state {
    ਪੂਰਨ refcnt;
    ਗਿਣਤੀ xkb_compose_state_flags flags;
    ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *ਸਾਰਣੀ;

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

ਬਣਤਰ xkb_compose_state *
xkb_compose_state_new(ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *ਸਾਰਣੀ,
                      ਗਿਣਤੀ xkb_compose_state_flags flags)
{
    ਅਚਲ ਗਿਣਤੀ xkb_compose_state_flags invalid_flags =
        (flags & ~(ਗਿਣਤੀ xkb_compose_state_flags)XKB_COMPOSE_STATE_FLAGS_ਮੁੱਲਾਂ);
    ਜੇ (invalid_flags) {
        log_err_func(ਸਾਰਣੀ->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                     "Unsupported compose state flags: %#x\n",
                     invalid_flags);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਬਣਤਰ xkb_compose_state * ਅਚਲ state = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(*state));
    ਜੇ (!state)
        ਵਾਪਸ ਨਿਰਾ;

    state->refcnt = 1;
    state->ਸਾਰਣੀ = xkb_compose_ਸਾਰਣੀ_ref(ਸਾਰਣੀ);

    state->flags = flags;
    state->prev_context = 0;
    state->context = 0;

    ਵਾਪਸ state;
}

ਬਣਤਰ xkb_compose_state *
xkb_compose_state_ref(ਬਣਤਰ xkb_compose_state *state)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(state->refcnt > 0);
    state->refcnt++;
    ਵਾਪਸ state;
}

ਖਾਲੀ
xkb_compose_state_unref(ਬਣਤਰ xkb_compose_state *state)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(!state || state->refcnt > 0);
    ਜੇ (!state || --state->refcnt > 0)
        ਵਾਪਸ;

    xkb_compose_ਸਾਰਣੀ_unref(state->ਸਾਰਣੀ);
    ਮੁਕਤ(state);
}

ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *
xkb_compose_state_get_compose_ਸਾਰਣੀ(ਬਣਤਰ xkb_compose_state *state)
{
    ਵਾਪਸ state->ਸਾਰਣੀ;
}

ਗਿਣਤੀ xkb_compose_feed_ਨਤੀਜਾ
xkb_compose_state_feed(ਬਣਤਰ xkb_compose_state *state, xkb_keysym_t keysym)
{
    uint32_t context;
    ਅਚਲ ਬਣਤਰ compose_ਗੰਢ *ਗੰਢ;

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
    ਜੇ (xkb_keysym_is_modifier(keysym))
        ਵਾਪਸ XKB_COMPOSE_FEED_IGNORED;

    ਗੰਢ = &darray_ਵਸਤੂ(state->ਸਾਰਣੀ->nodes, state->context);

    context = (ਗੰਢ->is_ਪੱਤੀ ? 1 : ਗੰਢ->ਅੰਦਰੂਨੀ.eqkid);
    ਜੇ (context == 1 && darray_ਮਾਪ(state->ਸਾਰਣੀ->nodes) == 1)
        context = 0;

    ਜਦੋਂ (context != 0) {
        ਗੰਢ = &darray_ਵਸਤੂ(state->ਸਾਰਣੀ->nodes, context);
        ਜੇ (keysym < ਗੰਢ->keysym)
            context = ਗੰਢ->lokid;
        ਨਹੀਂਤਾਂ ਜੇ (keysym > ਗੰਢ->keysym)
            context = ਗੰਢ->hikid;
        ਨਹੀਂਤਾਂ
            ਰੋਕੋ;
    }

    state->prev_context = state->context;
    state->context = context;
    ਵਾਪਸ XKB_COMPOSE_FEED_ACCEPTED;
}

ਖਾਲੀ
xkb_compose_state_reset(ਬਣਤਰ xkb_compose_state *state)
{
    state->prev_context = 0;
    state->context = 0;
}

ਗਿਣਤੀ xkb_compose_ਸਥਿਤੀ
xkb_compose_state_get_ਸਥਿਤੀ(ਬਣਤਰ xkb_compose_state *state)
{
    ਅਚਲ ਬਣਤਰ compose_ਗੰਢ *prev_ਗੰਢ, *ਗੰਢ;

    prev_ਗੰਢ = &darray_ਵਸਤੂ(state->ਸਾਰਣੀ->nodes, state->prev_context);
    ਗੰਢ = &darray_ਵਸਤੂ(state->ਸਾਰਣੀ->nodes, state->context);

    ਜੇ (state->context == 0 && !prev_ਗੰਢ->is_ਪੱਤੀ)
        ਵਾਪਸ XKB_COMPOSE_CANCELLED;

    ਜੇ (state->context == 0)
        ਵਾਪਸ XKB_COMPOSE_NOTHING;

    ਜੇ (!ਗੰਢ->is_ਪੱਤੀ)
        ਵਾਪਸ XKB_COMPOSE_COMPOSING;

    ਵਾਪਸ XKB_COMPOSE_COMPOSED;
}

ਪੂਰਨ
xkb_compose_state_get_utf8(ਬਣਤਰ xkb_compose_state *state,
                           ਅੱਖਰ *ਬਫਰ, ਮਾਪ_t ਮਾਪ)
{
    ਅਚਲ ਬਣਤਰ compose_ਗੰਢ *ਗੰਢ =
        &darray_ਵਸਤੂ(state->ਸਾਰਣੀ->nodes, state->context);

    ਜੇ (!ਗੰਢ->is_ਪੱਤੀ)
        ਜਾਓ fail;

    /* If there's no string specified, but only a keysym, try to do the
     * most helpful thing. */
    ਜੇ (ਗੰਢ->ਪੱਤੀ.utf8 == 0 && ਗੰਢ->ਪੱਤੀ.keysym != XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ) {
        ਅੱਖਰ utf8[XKB_KEYSYM_UTF8_MAX_ਮਾਪ];
        ਪੂਰਨ ret;

        ret = xkb_keysym_ਮੰਜ਼ਿਲ_utf8(ਗੰਢ->ਪੱਤੀ.keysym, utf8, ਆਕਾਰ(utf8));
        ਜੇ (ret < 0 || ret == 0) {
            /* ret < 0 is impossible.
             * ret == 0 means the keysym has no string representation. */
            ਜਾਓ fail;
        }

        ਵਾਪਸ snprintf(ਬਫਰ, ਮਾਪ, "%s", utf8);
    }

    ਵਾਪਸ snprintf(ਬਫਰ, ਮਾਪ, "%s",
                    &darray_ਵਸਤੂ(state->ਸਾਰਣੀ->utf8, ਗੰਢ->ਪੱਤੀ.utf8));

fail:
    ਜੇ (ਮਾਪ > 0)
        ਬਫਰ[0] = '\0';
    ਵਾਪਸ 0;
}

xkb_keysym_t
xkb_compose_state_get_one_sym(ਬਣਤਰ xkb_compose_state *state)
{
    ਅਚਲ ਬਣਤਰ compose_ਗੰਢ *ਗੰਢ =
        &darray_ਵਸਤੂ(state->ਸਾਰਣੀ->nodes, state->context);
    ਜੇ (!ਗੰਢ->is_ਪੱਤੀ)
        ਵਾਪਸ XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
    ਵਾਪਸ ਗੰਢ->ਪੱਤੀ.keysym;
}
