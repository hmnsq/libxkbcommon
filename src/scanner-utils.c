/*
 * Copyright © 2025 Ran Benita <ran@unusedvar.com>
 * SPDX-License-Identifier: MIT
 */

#समावेश "config.h"

#समावेश "scanner-utils.h"

काष्ठा scanner_loc
scanner_token_location(काष्ठा scanner *s)
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
    माप_प्रकार पंक्ति, column;
    माप_प्रकार पंक्ति_pos = 0;
    अगर (s->cached_pos > s->token_pos) {
        s->cached_pos = 0;
        s->cached_loc.पंक्ति = s->cached_loc.column = 1;
    }
    पंक्ति = s->cached_loc.पंक्ति;
    स्थिर अक्षर *ptr = s->s + s->cached_pos;
    स्थिर अक्षर *last = s->s + s->token_pos;
    जबतक ((ptr = स_प्रथम(ptr, '\n', (माप_प्रकार)(last - ptr)))) {
        पंक्ति++;
        ptr++;
        पंक्ति_pos = (माप_प्रकार)(ptr - s->s);
    }
    अगर (पंक्ति == s->cached_loc.पंक्ति) {
        column = s->cached_loc.column + (s->token_pos - s->cached_pos);
    } अन्यथा {
        column = s->token_pos - पंक्ति_pos + 1;
    }
    काष्ठा scanner_loc loc = {.पंक्ति = पंक्ति, .column = column};
    s->cached_pos = s->token_pos;
    s->cached_loc = loc;
    वापस loc;
}
