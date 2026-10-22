/*
 * Copyright © 2025 Ran Benita <ran@unusedvar.com>
 * SPDX-License-Identifier: MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ "scanner-utils.h"

నిర్మాణం scanner_loc
scanner_token_location(నిర్మాణం scanner *s)
{
    /*
     * The following article and accompying code compares some algorithms:
     * https://lemire.me/blog/2017/02/14/how-fast-can-you-count-lines/
     * https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/blob/master/2017/02/14/newlines.c
     * This is the fastest portable one for me, and it's simple.
     *
     * To avoid rescanning on each call, cache the previously found position
     * and start from that on the next call. This is effective as long as the
     * tokens go forward.
     */
    కొలత_t పంక్తి, స్తంభం;
    కొలత_t పంక్తి_pos = 0;
    ఒకవేళ (s->cached_pos > s->token_pos) {
        s->cached_pos = 0;
        s->cached_loc.పంక్తి = s->cached_loc.స్తంభం = 1;
    }
    పంక్తి = s->cached_loc.పంక్తి;
    స్థిరరాశి అక్షరం *ptr = s->s + s->cached_pos;
    స్థిరరాశి అక్షరం *చివరి = s->s + s->token_pos;
    అయితే ((ptr = memchr(ptr, '\n', (కొలత_t)(చివరి - ptr)))) {
        పంక్తి++;
        ptr++;
        పంక్తి_pos = (కొలత_t)(ptr - s->s);
    }
    ఒకవేళ (పంక్తి == s->cached_loc.పంక్తి) {
        స్తంభం = s->cached_loc.స్తంభం + (s->token_pos - s->cached_pos);
    } లేకపోతే {
        స్తంభం = s->token_pos - పంక్తి_pos + 1;
    }
    నిర్మాణం scanner_loc loc = {.పంక్తి = పంక్తి, .స్తంభం = స్తంభం};
    s->cached_pos = s->token_pos;
    s->cached_loc = loc;
    తిరిగి loc;
}
