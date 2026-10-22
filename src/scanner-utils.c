/*
 * Copyright © 2025 Ran Benita <ran@unusedvar.com>
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ "scanner-utils.h"

ಸಂರಚನಾ scanner_loc
scanner_token_location(ಸಂರಚನಾ scanner *s)
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
    size_t ಸಾಲು, column;
    size_t ಸಾಲು_pos = 0;
    ಯದಿ (s->cached_pos > s->token_pos) {
        s->cached_pos = 0;
        s->cached_loc.ಸಾಲು = s->cached_loc.column = 1;
    }
    ಸಾಲು = s->cached_loc.ಸಾಲು;
    ಧ್ರುವ ವರ್ಣ *ptr = s->s + s->cached_pos;
    ಧ್ರುವ ವರ್ಣ *last = s->s + s->token_pos;
    ಯಾವತ್ ((ptr = memchr(ptr, '\n', (size_t)(last - ptr)))) {
        ಸಾಲು++;
        ptr++;
        ಸಾಲು_pos = (size_t)(ptr - s->s);
    }
    ಯದಿ (ಸಾಲು == s->cached_loc.ಸಾಲು) {
        column = s->cached_loc.column + (s->token_pos - s->cached_pos);
    } ಅನ್ಯಥಾ {
        column = s->token_pos - ಸಾಲು_pos + 1;
    }
    ಸಂರಚನಾ scanner_loc loc = {.ಸಾಲು = ಸಾಲು, .column = column};
    s->cached_pos = s->token_pos;
    s->cached_loc = loc;
    ಪ್ರತ್ಯಾಗಮ loc;
}
