/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن <assert.h>

#ضمن "context.h"
#ضمن "features/enums.h"
#ضمن "keysym.h"
#ضمن "table.h"
#ضمن "utils.h"

بنية xkb_compose_state {
    صحيح refcnt;
    تعداد xkb_compose_state_flags flags;
    بنية xkb_compose_جدول *جدول;

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

بنية xkb_compose_state *
xkb_compose_state_new(بنية xkb_compose_جدول *جدول,
                      تعداد xkb_compose_state_flags flags)
{
    ثابت تعداد xkb_compose_state_flags invalid_flags =
        (flags & ~(تعداد xkb_compose_state_flags)XKB_COMPOSE_STATE_FLAGS_قدرون);
    إذا (invalid_flags) {
        log_err_func(جدول->ctx, XKB_LOG_پيغام_NO_ID,
                     "Unsupported compose state flags: %#x\n",
                     invalid_flags);
        أرجع عدم;
    }

    بنية xkb_compose_state * ثابت state = صفر_يادگيري_ڏيو(1, حجم(*state));
    إذا (!state)
        أرجع عدم;

    state->refcnt = 1;
    state->جدول = xkb_compose_جدول_ref(جدول);

    state->flags = flags;
    state->prev_context = 0;
    state->context = 0;

    أرجع state;
}

بنية xkb_compose_state *
xkb_compose_state_ref(بنية xkb_compose_state *state)
{
    أكد(state->refcnt > 0);
    state->refcnt++;
    أرجع state;
}

فراغ
xkb_compose_state_unref(بنية xkb_compose_state *state)
{
    أكد(!state || state->refcnt > 0);
    إذا (!state || --state->refcnt > 0)
        أرجع;

    xkb_compose_جدول_unref(state->جدول);
    حرر(state);
}

بنية xkb_compose_جدول *
xkb_compose_state_get_compose_جدول(بنية xkb_compose_state *state)
{
    أرجع state->جدول;
}

تعداد xkb_compose_feed_نتيجو
xkb_compose_state_feed(بنية xkb_compose_state *state, xkb_keysym_t keysym)
{
    uint32_t context;
    ثابت بنية compose_ڳنڍ *ڳنڍ;

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
    إذا (xkb_keysym_is_modifier(keysym))
        أرجع XKB_COMPOSE_FEED_IGNORED;

    ڳنڍ = &darray_شي(state->جدول->nodes, state->context);

    context = (ڳنڍ->is_پن ? 1 : ڳنڍ->اندروني.eqkid);
    إذا (context == 1 && darray_سائيز(state->جدول->nodes) == 1)
        context = 0;

    طالما (context != 0) {
        ڳنڍ = &darray_شي(state->جدول->nodes, context);
        إذا (keysym < ڳنڍ->keysym)
            context = ڳنڍ->lokid;
        وإلا إذا (keysym > ڳنڍ->keysym)
            context = ڳنڍ->hikid;
        وإلا
            اكسر;
    }

    state->prev_context = state->context;
    state->context = context;
    أرجع XKB_COMPOSE_FEED_ACCEPTED;
}

فراغ
xkb_compose_state_reset(بنية xkb_compose_state *state)
{
    state->prev_context = 0;
    state->context = 0;
}

تعداد xkb_compose_حالت
xkb_compose_state_get_حالت(بنية xkb_compose_state *state)
{
    ثابت بنية compose_ڳنڍ *prev_ڳنڍ, *ڳنڍ;

    prev_ڳنڍ = &darray_شي(state->جدول->nodes, state->prev_context);
    ڳنڍ = &darray_شي(state->جدول->nodes, state->context);

    إذا (state->context == 0 && !prev_ڳنڍ->is_پن)
        أرجع XKB_COMPOSE_CANCELLED;

    إذا (state->context == 0)
        أرجع XKB_COMPOSE_NOTHING;

    إذا (!ڳنڍ->is_پن)
        أرجع XKB_COMPOSE_COMPOSING;

    أرجع XKB_COMPOSE_COMPOSED;
}

صحيح
xkb_compose_state_get_utf8(بنية xkb_compose_state *state,
                           حرف *بفر, سائيز_t سائيز)
{
    ثابت بنية compose_ڳنڍ *ڳنڍ =
        &darray_شي(state->جدول->nodes, state->context);

    إذا (!ڳنڍ->is_پن)
        اذهب fail;

    /* If there's no string specified, but only a keysym, try to do the
     * most helpful thing. */
    إذا (ڳنڍ->پن.utf8 == 0 && ڳنڍ->پن.keysym != XKB_ڪنجي_No_نشان) {
        حرف utf8[XKB_KEYSYM_UTF8_MAX_سائيز];
        صحيح ret;

        ret = xkb_keysym_منزل_utf8(ڳنڍ->پن.keysym, utf8, حجم(utf8));
        إذا (ret < 0 || ret == 0) {
            /* ret < 0 is impossible.
             * ret == 0 means the keysym has no string representation. */
            اذهب fail;
        }

        أرجع snprintf(بفر, سائيز, "%s", utf8);
    }

    أرجع snprintf(بفر, سائيز, "%s",
                    &darray_شي(state->جدول->utf8, ڳنڍ->پن.utf8));

fail:
    إذا (سائيز > 0)
        بفر[0] = '\0';
    أرجع 0;
}

xkb_keysym_t
xkb_compose_state_get_one_sym(بنية xkb_compose_state *state)
{
    ثابت بنية compose_ڳنڍ *ڳنڍ =
        &darray_شي(state->جدول->nodes, state->context);
    إذا (!ڳنڍ->is_پن)
        أرجع XKB_ڪنجي_No_نشان;
    أرجع ڳنڍ->پن.keysym;
}
