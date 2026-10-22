/*
 * Copyright © 2025 Ran Benita <ran@unusedvar.com>
 * SPDX-License-Identifier: MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ "scanner-utils.h"

સંરચના scanner_loc
scanner_token_location(સંરચના scanner *s)
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
    size_t લીટી, column;
    size_t લીટી_pos = 0;
    જો (s->cached_pos > s->token_pos) {
        s->cached_pos = 0;
        s->cached_loc.લીટી = s->cached_loc.column = 1;
    }
    લીટી = s->cached_loc.લીટી;
    અચલ અક્ષર *ptr = s->s + s->cached_pos;
    અચલ અક્ષર *last = s->s + s->token_pos;
    જ્યારે ((ptr = memchr(ptr, '\n', (size_t)(last - ptr)))) {
        લીટી++;
        ptr++;
        લીટી_pos = (size_t)(ptr - s->s);
    }
    જો (લીટી == s->cached_loc.લીટી) {
        column = s->cached_loc.column + (s->token_pos - s->cached_pos);
    } નહીંતર {
        column = s->token_pos - લીટી_pos + 1;
    }
    સંરચના scanner_loc loc = {.લીટી = લીટી, .column = column};
    s->cached_pos = s->token_pos;
    s->cached_loc = loc;
    પાછા loc;
}
