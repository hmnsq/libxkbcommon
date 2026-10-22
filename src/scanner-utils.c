/*
 * Copyright © 2025 Ran Benita <ran@unusedvar.com>
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव "scanner-utils.h"

संरचना scanner_loc
scanner_token_location(संरचना scanner *s)
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
    माप_प्रकार ओळ, column;
    माप_प्रकार ओळ_pos = 0;
    जर (s->cached_pos > s->token_pos) {
        s->cached_pos = 0;
        s->cached_loc.ओळ = s->cached_loc.column = 1;
    }
    ओळ = s->cached_loc.ओळ;
    स्थिरांक अक्षर *ptr = s->s + s->cached_pos;
    स्थिरांक अक्षर *last = s->s + s->token_pos;
    पर्यंत ((ptr = स_प्रथम(ptr, '\n', (माप_प्रकार)(last - ptr)))) {
        ओळ++;
        ptr++;
        ओळ_pos = (माप_प्रकार)(ptr - s->s);
    }
    जर (ओळ == s->cached_loc.ओळ) {
        column = s->cached_loc.column + (s->token_pos - s->cached_pos);
    } अन्यथा {
        column = s->token_pos - ओळ_pos + 1;
    }
    संरचना scanner_loc loc = {.ओळ = ओळ, .column = column};
    s->cached_pos = s->token_pos;
    s->cached_loc = loc;
    परत loc;
}
