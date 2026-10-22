/*
 * Copyright © 2025 Ran Benita <ran@unusedvar.com>
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل "scanner-utils.h"

ساخت scanner_loc
scanner_token_location(ساخت scanner *s)
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
    سائز_t سطر, عمود;
    سائز_t سطر_pos = 0;
    اگر (s->cached_pos > s->token_pos) {
        s->cached_pos = 0;
        s->cached_loc.سطر = s->cached_loc.عمود = 1;
    }
    سطر = s->cached_loc.سطر;
    ثابت حرف *ptr = s->s + s->cached_pos;
    ثابت حرف *آخری = s->s + s->token_pos;
    جب ((ptr = memchr(ptr, '\n', (سائز_t)(آخری - ptr)))) {
        سطر++;
        ptr++;
        سطر_pos = (سائز_t)(ptr - s->s);
    }
    اگر (سطر == s->cached_loc.سطر) {
        عمود = s->cached_loc.عمود + (s->token_pos - s->cached_pos);
    } ورنہ {
        عمود = s->token_pos - سطر_pos + 1;
    }
    ساخت scanner_loc loc = {.سطر = سطر, .عمود = عمود};
    s->cached_pos = s->token_pos;
    s->cached_loc = loc;
    واپس loc;
}
