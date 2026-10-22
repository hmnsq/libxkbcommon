/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل <assert.h>

#شامل "context.h"
#شامل "features/enums.h"
#شامل "keysym.h"
#شامل "table.h"
#شامل "utils.h"

ساخت xkb_compose_state {
    صحیح refcnt;
    شمار xkb_compose_state_flags flags;
    ساخت xkb_compose_جدول *جدول;

    /*
     * Offsets into xkb_compose_table::nodes.
     *
     * They maintain the current and previous position in the trie; see
     * xkb_compose_state_feed().
     *
     * This is also sufficient for inferring the current status; see
     * xkb_compose_state_get_status().
     */
    uint32_t پچھلا_context;
    uint32_t context;
};

ساخت xkb_compose_state *
xkb_compose_state_new(ساخت xkb_compose_جدول *جدول,
                      شمار xkb_compose_state_flags flags)
{
    ثابت شمار xkb_compose_state_flags invalid_flags =
        (flags & ~(شمار xkb_compose_state_flags)XKB_COMPOSE_STATE_FLAGS_اقدار);
    اگر (invalid_flags) {
        log_err_func(جدول->ctx, XKB_LOG_پیغام_NO_ID,
                     "Unsupported compose state flags: %#x\n",
                     invalid_flags);
        واپس تہی;
    }

    ساخت xkb_compose_state * ثابت state = صفر_حافظہ_دو(1, حجم(*state));
    اگر (!state)
        واپس تہی;

    state->refcnt = 1;
    state->جدول = xkb_compose_جدول_ref(جدول);

    state->flags = flags;
    state->پچھلا_context = 0;
    state->context = 0;

    واپس state;
}

ساخت xkb_compose_state *
xkb_compose_state_ref(ساخت xkb_compose_state *state)
{
    أكد(state->refcnt > 0);
    state->refcnt++;
    واپس state;
}

خالی
xkb_compose_state_unref(ساخت xkb_compose_state *state)
{
    أكد(!state || state->refcnt > 0);
    اگر (!state || --state->refcnt > 0)
        واپس;

    xkb_compose_جدول_unref(state->جدول);
    آزاد(state);
}

ساخت xkb_compose_جدول *
xkb_compose_state_get_compose_جدول(ساخت xkb_compose_state *state)
{
    واپس state->جدول;
}

شمار xkb_compose_feed_نتیجہ
xkb_compose_state_feed(ساخت xkb_compose_state *state, xkb_keysym_t keysym)
{
    uint32_t context;
    ثابت ساخت compose_گرہ *گرہ;

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
    اگر (xkb_keysym_is_modifier(keysym))
        واپس XKB_COMPOSE_FEED_IGNORED;

    گرہ = &darray_شے(state->جدول->nodes, state->context);

    context = (گرہ->is_برگ ? 1 : گرہ->اندرونی_2.eqkid);
    اگر (context == 1 && darray_سائز(state->جدول->nodes) == 1)
        context = 0;

    جب (context != 0) {
        گرہ = &darray_شے(state->جدول->nodes, context);
        اگر (keysym < گرہ->keysym)
            context = گرہ->lokid;
        ورنہ اگر (keysym > گرہ->keysym)
            context = گرہ->hikid;
        ورنہ
            توڑو;
    }

    state->پچھلا_context = state->context;
    state->context = context;
    واپس XKB_COMPOSE_FEED_ACCEPTED;
}

خالی
xkb_compose_state_reset(ساخت xkb_compose_state *state)
{
    state->پچھلا_context = 0;
    state->context = 0;
}

شمار xkb_compose_حالت
xkb_compose_state_get_حالت(ساخت xkb_compose_state *state)
{
    ثابت ساخت compose_گرہ *پچھلا_گرہ, *گرہ;

    پچھلا_گرہ = &darray_شے(state->جدول->nodes, state->پچھلا_context);
    گرہ = &darray_شے(state->جدول->nodes, state->context);

    اگر (state->context == 0 && !پچھلا_گرہ->is_برگ)
        واپس XKB_COMPOSE_CANCELLED;

    اگر (state->context == 0)
        واپس XKB_COMPOSE_NOTHING;

    اگر (!گرہ->is_برگ)
        واپس XKB_COMPOSE_COMPOSING;

    واپس XKB_COMPOSE_COMPOSED;
}

صحیح
xkb_compose_state_get_utf8(ساخت xkb_compose_state *state,
                           حرف *بفر, سائز_t سائز)
{
    ثابت ساخت compose_گرہ *گرہ =
        &darray_شے(state->جدول->nodes, state->context);

    اگر (!گرہ->is_برگ)
        جاؤ fail;

    /* If there's no string specified, but only a keysym, try to do the
     * most helpful thing. */
    اگر (گرہ->برگ.utf8 == 0 && گرہ->برگ.keysym != XKB_کلید_No_علامت) {
        حرف utf8[XKB_KEYSYM_UTF8_MAX_سائز];
        صحیح ret;

        ret = xkb_keysym_مقصد_utf8(گرہ->برگ.keysym, utf8, حجم(utf8));
        اگر (ret < 0 || ret == 0) {
            /* ret < 0 is impossible.
             * ret == 0 means the keysym has no string representation. */
            جاؤ fail;
        }

        واپس snprintf(بفر, سائز, "%s", utf8);
    }

    واپس snprintf(بفر, سائز, "%s",
                    &darray_شے(state->جدول->utf8, گرہ->برگ.utf8));

fail:
    اگر (سائز > 0)
        بفر[0] = '\0';
    واپس 0;
}

xkb_keysym_t
xkb_compose_state_get_one_sym(ساخت xkb_compose_state *state)
{
    ثابت ساخت compose_گرہ *گرہ =
        &darray_شے(state->جدول->nodes, state->context);
    اگر (!گرہ->is_برگ)
        واپس XKB_کلید_No_علامت;
    واپس گرہ->برگ.keysym;
}
