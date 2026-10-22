/*
 * Copyright © 2025 Ran Benita <ran@unusedvar.com>
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل "scanner-utils.h"

ساختار scanner_loc
scanner_token_location(ساختار scanner *s)
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
    size_t سطر, column;
    size_t سطر_pos = 0;
    اگر (s->cached_pos > s->token_pos) {
        s->cached_pos = 0;
        s->cached_loc.سطر = s->cached_loc.column = 1;
    }
    سطر = s->cached_loc.سطر;
    ثابت نویسه *ptr = s->s + s->cached_pos;
    ثابت نویسه *last = s->s + s->token_pos;
    وقتی ((ptr = memchr(ptr, '\n', (size_t)(last - ptr)))) {
        سطر++;
        ptr++;
        سطر_pos = (size_t)(ptr - s->s);
    }
    اگر (سطر == s->cached_loc.سطر) {
        column = s->cached_loc.column + (s->token_pos - s->cached_pos);
    } وگرنه {
        column = s->token_pos - سطر_pos + 1;
    }
    ساختار scanner_loc loc = {.سطر = سطر, .column = column};
    s->cached_pos = s->token_pos;
    s->cached_loc = loc;
    بازگشت loc;
}
