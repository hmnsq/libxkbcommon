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
    size_t باريس, column;
    size_t باريس_pos = 0;
    إذا (s->cached_pos > s->token_pos) {
        s->cached_pos = 0;
        s->cached_loc.باريس = s->cached_loc.column = 1;
    }
    باريس = s->cached_loc.باريس;
    ثابت حرف *ptr = s->s + s->cached_pos;
    ثابت حرف *last = s->s + s->token_pos;
    طالما ((ptr = memchr(ptr, '\n', (size_t)(last - ptr)))) {
        باريس++;
        ptr++;
        باريس_pos = (size_t)(ptr - s->s);
    }
    إذا (باريس == s->cached_loc.باريس) {
        column = s->cached_loc.column + (s->token_pos - s->cached_pos);
    } وإلا {
        column = s->token_pos - باريس_pos + 1;
    }
    بنية scanner_loc loc = {.باريس = باريس, .column = column};
    s->cached_pos = s->token_pos;
    s->cached_loc = loc;
    أرجع loc;
}
