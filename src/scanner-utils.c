/*
 * Copyright © 2025 Ran Benita <ran@unusedvar.com>
 * SPDX-License-Identifier: MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ "scanner-utils.h"

கட்டமைப்பு scanner_loc
scanner_token_location(கட்டமைப்பு scanner *s)
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
    size_t வரி, column;
    size_t வரி_pos = 0;
    எனில் (s->cached_pos > s->token_pos) {
        s->cached_pos = 0;
        s->cached_loc.வரி = s->cached_loc.column = 1;
    }
    வரி = s->cached_loc.வரி;
    மாறிலி எழுத்து *ptr = s->s + s->cached_pos;
    மாறிலி எழுத்து *last = s->s + s->token_pos;
    போது ((ptr = memchr(ptr, '\n', (size_t)(last - ptr)))) {
        வரி++;
        ptr++;
        வரி_pos = (size_t)(ptr - s->s);
    }
    எனில் (வரி == s->cached_loc.வரி) {
        column = s->cached_loc.column + (s->token_pos - s->cached_pos);
    } இல்லையெனில் {
        column = s->token_pos - வரி_pos + 1;
    }
    கட்டமைப்பு scanner_loc loc = {.வரி = வரி, .column = column};
    s->cached_pos = s->token_pos;
    s->cached_loc = loc;
    திரும்பு loc;
}
