/*
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>

#అన్తర్భావ "context.h"
#అన్తర్భావ "features/enums.h"
#అన్తర్భావ "keysym.h"
#అన్తర్భావ "table.h"
#అన్తర్భావ "utils.h"

నిర్మాణం xkb_compose_state {
    పూర్ణాంకం refcnt;
    గణన xkb_compose_state_flags flags;
    నిర్మాణం xkb_compose_పట్టిక *పట్టిక;

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

నిర్మాణం xkb_compose_state *
xkb_compose_state_new(నిర్మాణం xkb_compose_పట్టిక *పట్టిక,
                      గణన xkb_compose_state_flags flags)
{
    స్థిరరాశి గణన xkb_compose_state_flags invalid_flags =
        (flags & ~(గణన xkb_compose_state_flags)XKB_COMPOSE_STATE_FLAGS_విలువలు);
    ఒకవేళ (invalid_flags) {
        log_err_func(పట్టిక->ctx, XKB_LOG_సందేశం_NO_ID,
                     "Unsupported compose state flags: %#x\n",
                     invalid_flags);
        తిరిగి ఖాళీ;
    }

    నిర్మాణం xkb_compose_state * స్థిరరాశి state = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(*state));
    ఒకవేళ (!state)
        తిరిగి ఖాళీ;

    state->refcnt = 1;
    state->పట్టిక = xkb_compose_పట్టిక_ref(పట్టిక);

    state->flags = flags;
    state->prev_context = 0;
    state->context = 0;

    తిరిగి state;
}

నిర్మాణం xkb_compose_state *
xkb_compose_state_ref(నిర్మాణం xkb_compose_state *state)
{
    ప్రతిజ్ఞా(state->refcnt > 0);
    state->refcnt++;
    తిరిగి state;
}

శూన్య
xkb_compose_state_unref(నిర్మాణం xkb_compose_state *state)
{
    ప్రతిజ్ఞా(!state || state->refcnt > 0);
    ఒకవేళ (!state || --state->refcnt > 0)
        తిరిగి;

    xkb_compose_పట్టిక_unref(state->పట్టిక);
    విడుదల(state);
}

నిర్మాణం xkb_compose_పట్టిక *
xkb_compose_state_get_compose_పట్టిక(నిర్మాణం xkb_compose_state *state)
{
    తిరిగి state->పట్టిక;
}

గణన xkb_compose_feed_ఫలితం
xkb_compose_state_feed(నిర్మాణం xkb_compose_state *state, xkb_keysym_t keysym)
{
    uint32_t context;
    స్థిరరాశి నిర్మాణం compose_కణుపు *కణుపు;

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
    ఒకవేళ (xkb_keysym_is_modifier(keysym))
        తిరిగి XKB_COMPOSE_FEED_IGNORED;

    కణుపు = &darray_అంశం(state->పట్టిక->nodes, state->context);

    context = (కణుపు->is_ఆకు ? 1 : కణుపు->అంతర్గత.eqkid);
    ఒకవేళ (context == 1 && darray_కొలత(state->పట్టిక->nodes) == 1)
        context = 0;

    అయితే (context != 0) {
        కణుపు = &darray_అంశం(state->పట్టిక->nodes, context);
        ఒకవేళ (keysym < కణుపు->keysym)
            context = కణుపు->lokid;
        లేకపోతే ఒకవేళ (keysym > కణుపు->keysym)
            context = కణుపు->hikid;
        లేకపోతే
            ఆపు;
    }

    state->prev_context = state->context;
    state->context = context;
    తిరిగి XKB_COMPOSE_FEED_ACCEPTED;
}

శూన్య
xkb_compose_state_reset(నిర్మాణం xkb_compose_state *state)
{
    state->prev_context = 0;
    state->context = 0;
}

గణన xkb_compose_స్థితి
xkb_compose_state_get_స్థితి(నిర్మాణం xkb_compose_state *state)
{
    స్థిరరాశి నిర్మాణం compose_కణుపు *prev_కణుపు, *కణుపు;

    prev_కణుపు = &darray_అంశం(state->పట్టిక->nodes, state->prev_context);
    కణుపు = &darray_అంశం(state->పట్టిక->nodes, state->context);

    ఒకవేళ (state->context == 0 && !prev_కణుపు->is_ఆకు)
        తిరిగి XKB_COMPOSE_CANCELLED;

    ఒకవేళ (state->context == 0)
        తిరిగి XKB_COMPOSE_NOTHING;

    ఒకవేళ (!కణుపు->is_ఆకు)
        తిరిగి XKB_COMPOSE_COMPOSING;

    తిరిగి XKB_COMPOSE_COMPOSED;
}

పూర్ణాంకం
xkb_compose_state_get_utf8(నిర్మాణం xkb_compose_state *state,
                           అక్షరం *బఫరు, కొలత_t కొలత)
{
    స్థిరరాశి నిర్మాణం compose_కణుపు *కణుపు =
        &darray_అంశం(state->పట్టిక->nodes, state->context);

    ఒకవేళ (!కణుపు->is_ఆకు)
        వెళ్ళు fail;

    /* If there's no string specified, but only a keysym, try to do the
     * most helpful thing. */
    ఒకవేళ (కణుపు->ఆకు.utf8 == 0 && కణుపు->ఆకు.keysym != XKB_కీలకం_No_చిహ్నం) {
        అక్షరం utf8[XKB_KEYSYM_UTF8_MAX_కొలత];
        పూర్ణాంకం ret;

        ret = xkb_keysym_గమ్యం_utf8(కణుపు->ఆకు.keysym, utf8, పరిమాణం(utf8));
        ఒకవేళ (ret < 0 || ret == 0) {
            /* ret < 0 is impossible.
             * ret == 0 means the keysym has no string representation. */
            వెళ్ళు fail;
        }

        తిరిగి snprintf(బఫరు, కొలత, "%s", utf8);
    }

    తిరిగి snprintf(బఫరు, కొలత, "%s",
                    &darray_అంశం(state->పట్టిక->utf8, కణుపు->ఆకు.utf8));

fail:
    ఒకవేళ (కొలత > 0)
        బఫరు[0] = '\0';
    తిరిగి 0;
}

xkb_keysym_t
xkb_compose_state_get_one_sym(నిర్మాణం xkb_compose_state *state)
{
    స్థిరరాశి నిర్మాణం compose_కణుపు *కణుపు =
        &darray_అంశం(state->పట్టిక->nodes, state->context);
    ఒకవేళ (!కణుపు->is_ఆకు)
        తిరిగి XKB_కీలకం_No_చిహ్నం;
    తిరిగి కణుపు->ఆకు.keysym;
}
