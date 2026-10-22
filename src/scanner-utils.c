/*
 * Copyright © 2025 Ran Benita <ran@unusedvar.com>
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن "scanner-utils.h"

بنية scanner_loc
scanner_token_location(بنية scanner *s)
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
    اندازه_t کرښه, ستنه;
    اندازه_t کرښه_pos = 0;
    إذا (s->cached_pos > s->token_pos) {
        s->cached_pos = 0;
        s->cached_loc.کرښه = s->cached_loc.ستنه = 1;
    }
    کرښه = s->cached_loc.کرښه;
    ثابت حرف *ptr = s->s + s->cached_pos;
    ثابت حرف *وروستی = s->s + s->token_pos;
    طالما ((ptr = memchr(ptr, '\n', (اندازه_t)(وروستی - ptr)))) {
        کرښه++;
        ptr++;
        کرښه_pos = (اندازه_t)(ptr - s->s);
    }
    إذا (کرښه == s->cached_loc.کرښه) {
        ستنه = s->cached_loc.ستنه + (s->token_pos - s->cached_pos);
    } وإلا {
        ستنه = s->token_pos - کرښه_pos + 1;
    }
    بنية scanner_loc loc = {.کرښه = کرښه, .ستنه = ستنه};
    s->cached_pos = s->token_pos;
    s->cached_loc = loc;
    أرجع loc;
}
