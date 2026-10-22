/*
 * Copyright © 2013,2021 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ضمن "config.h"

#ضمن "xkbcommon/xkbcommon-compose.h"
#ضمن "src/utils.h"
#ضمن "src/context.h"

/*
 * The compose table data structure is a ternary search tree.
 *
 * Reference: https://www.drdobbs.com/database/ternary-search-trees/184410528
 * Visualization: https://www.cs.usfca.edu/~galles/visualization/TST.html
 *
 * Short example. Given these sequences:
 *
 *      <B> <C>        : "first"  dead_a
 *      <B> <D> <E>    : "second" dead_b
 *      <A> <F>        : "third"  dead_c
 *
 * the tree would look like:
 *
 *          -------- [<B>]---------
 *          |          |          #
 *          v          V
 *     -- [<A>] --   [<C>] --------
 *     #    |    #     |          |
 *          v          #     -- [<D>] --
 *     -- [<F>] --           #    |    #
 *     #    |    #                v
 *          #                -- [<E>] --
 *                           #    |    #
 *                                #
 *
 * where:
 * - [<X>] is a node for a sequence keysym <X>.
 * - right arrows are `hikid` pointers.
 * - left arrows are `lokid` pointers.
 * - down arrows are `eqkid` pointers.
 * - # is a nil pointer.
 *
 * The nodes are all kept in a contiguous array.  Pointers are represented
 * as integer offsets into this array.  A nil pointer is represented as 0
 * (which, helpfully, is the offset of an empty dummy node).
 *
 * Nodes without an eqkid are leaf nodes.  Since a sequence cannot be a
 * prefix of another, these are exactly the nodes which terminate the
 * sequences (in a bijective manner).
 *
 * A leaf contains the result data of its sequence.  The result keysym is
 * contained in the node struct itself; the result UTF-8 string is a byte
 * offset into an array of the form "\0first\0second\0third" (the initial
 * \0 is so offset 0 points to an empty string).
 */

/* 7 nodes for every potential Unicode character and then some should be
 * enough for all purposes. */
#define MAX_COMPOSE_NODES_LOG2 23
#define MAX_COMPOSE_NODES (1u << MAX_COMPOSE_NODES_LOG2)

بنية compose_كلي {
    xkb_keysym_t keysym;

    /* Offset into xkb_compose_table::nodes or 0. */
    uint32_t lokid;
    /* Offset into xkb_compose_table::nodes or 0. */
    uint32_t hikid;

    اتحاد {
        بنية {
            uint32_t _pad:31;
            bool is_leaf:1;
        };
        بنية {
            uint32_t _pad:31;
            bool is_leaf:1;
            /* Offset into xkb_compose_table::nodes or 0. */
            uint32_t eqkid;
        } internal;
        بنية {
            /* Offset into xkb_compose_table::utf8. */
            uint32_t utf8:31;
            bool is_leaf:1;
            xkb_keysym_t keysym;
        } leaf;
    };
};

بنية xkb_compose_table {
    صحيح refcnt;
    تعداد xkb_compose_format format;
    تعداد xkb_compose_compile_flags flags;
    بنية xkb_context *ctx;

    حرف *locale;

    darray_char utf8;
    darray(بنية compose_كلي) nodes;
};

بنية xkb_compose_table_entry {
    size_t sequence_length;
    xkb_keysym_t *sequence ATTR_COUNTED_BY(sequence_length);
    xkb_keysym_t keysym;
    ثابت حرف *utf8;
};
