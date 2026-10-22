/*
 * Copyright © 2013,2021 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن <assert.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "features/enums.h"
#ضمن "constants.h"
#ضمن "messages-codes.h"
#ضمن "parser.h"
#ضمن "paths.h"
#ضمن "table.h"
#ضمن "utils.h"

ساكن بنية xkb_compose_table *
xkb_compose_table_new(بنية xkb_context *ctx, ثابت حرف *func,
                      ثابت حرف *locale,
                      تعداد xkb_compose_format format,
                      تعداد xkb_compose_compile_flags flags)
{
    ثابت تعداد xkb_compose_compile_flags invalid_flags = (
        flags & ~(تعداد xkb_compose_compile_flags)XKB_COMPOSE_COMPILE_FLAGS_VALUES
    );
    אם (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized flags: %#x\n", func, invalid_flags);
        החזר عدم;
    }

    אם (format != XKB_COMPOSE_FORMAT_TEXT_V1) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unsupported compose format: %d\n", func, format);
        החזר عدم;
    }

    حرف *resolved_locale;
    بنية xkb_compose_table *table;
    بنية compose_node dummy = {0};

    resolved_locale = resolve_locale(ctx, locale);
    אם (!resolved_locale)
        החזר عدم;

    table = calloc(1, حجم(*table));
    אם (!table) {
        حرر(resolved_locale);
        החזר عدم;
    }

    table->refcnt = 1;
    table->ctx = xkb_context_ref(ctx);

    table->locale = resolved_locale;
    table->format = format;
    table->flags = flags;

    darray_init(table->nodes);
    darray_init(table->utf8);

    dummy.keysym = XKB_KEY_NoSymbol;
    dummy.leaf.is_leaf = true;
    dummy.leaf.utf8 = 0;
    dummy.leaf.keysym = XKB_KEY_NoSymbol;
    darray_append(table->nodes, dummy);

    darray_append(table->utf8, '\0');

    החזר table;
}

بنية xkb_compose_table *
xkb_compose_table_ref(بنية xkb_compose_table *table)
{
    أكد(table->refcnt > 0);
    table->refcnt++;
    החזר table;
}

فراغ
xkb_compose_table_unref(بنية xkb_compose_table *table)
{
    أكد(!table || table->refcnt > 0);
    אם (!table || --table->refcnt > 0)
        החזר;
    حرر(table->locale);
    darray_free(table->nodes);
    darray_free(table->utf8);
    xkb_context_unref(table->ctx);
    حرر(table);
}

بنية xkb_compose_table *
xkb_compose_table_new_from_file(بنية xkb_context *ctx,
                                FILE *file,
                                ثابت حرف *locale,
                                تعداد xkb_compose_format format,
                                تعداد xkb_compose_compile_flags flags)
{
    بنية xkb_compose_table * ثابت table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    אם (!table)
        החזר عدم;

    אם (!parse_file(table, file, "(unknown file)")) {
        xkb_compose_table_unref(table);
        החזר عدم;
    }

    החזר table;
}

بنية xkb_compose_table *
xkb_compose_table_new_from_buffer(بنية xkb_context *ctx,
                                  ثابت حرف *buffer, size_t length,
                                  ثابت حرف *locale,
                                  تعداد xkb_compose_format format,
                                  تعداد xkb_compose_compile_flags flags)
{
    بنية xkb_compose_table * ثابت table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    אם (!table)
        החזר عدم;

    אם (!parse_string(table, buffer, length, "(input string)")) {
        xkb_compose_table_unref(table);
        החזר عدم;
    }

    החזר table;
}

بنية xkb_compose_table *
xkb_compose_table_new_from_locale(بنية xkb_context *ctx,
                                  ثابت حرف *locale,
                                  تعداد xkb_compose_compile_flags flags)
{
    ساكن ثابت تعداد xkb_compose_format format = XKB_COMPOSE_FORMAT_TEXT_V1;
    بنية xkb_compose_table * ثابت table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    אם (!table)
        החזר عدم;

    حرف *path = get_xcomposefile_path(ctx);
    FILE *file = open_file(path);
    אם (file)
        اذهب found_path;
    حرر(path);

    path = get_xdg_xcompose_file_path(ctx);
    file = open_file(path);
    אם (file)
        اذهب found_path;
    حرر(path);

    path = get_home_xcompose_file_path(ctx);
    file = open_file(path);
    אם (file)
        اذهب found_path;
    حرر(path);

    path = get_locale_compose_file_path(ctx, table->locale);
    file = open_file(path);
    אם (file)
        اذهب found_path;
    حرر(path);

    log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
            "couldn't find a Compose file for locale \"%s\" (mapped to \"%s\")\n",
            locale, table->locale);
    xkb_compose_table_unref(table);
    החזר عدم;

found_path:
    {} /* Label followed by a declaration is a C23 extension */
    ثابت bool ok = parse_file(table, file, path);
    fclose(file);
    אם (!ok) {
        حرر(path);
        xkb_compose_table_unref(table);
        החזר عدم;
    }

    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "created compose table from locale %s with path %s\n",
            table->locale, path);

    حرر(path);
    החזר table;
}

ثابت xkb_keysym_t *
xkb_compose_table_entry_sequence(بنية xkb_compose_table_entry *entry,
                                 size_t *sequence_length)
{
    *sequence_length = entry->sequence_length;
    החזר entry->sequence;
}

xkb_keysym_t
xkb_compose_table_entry_keysym(بنية xkb_compose_table_entry *entry)
{
    החזר entry->keysym;
}

ثابت حرف *
xkb_compose_table_entry_utf8(بنية xkb_compose_table_entry *entry)
{
    החזר entry->utf8;
}

#אם MAX_COMPOSE_NODES_LOG2 > 31
#error "Cannot implement bit field xkb_compose_table_iterator_pending_node.offset"
#endif

بنية xkb_compose_table_iterator_pending_node {
    uint32_t offset:31;
    bool processed:1;
};

بنية xkb_compose_table_iterator {
    بنية xkb_compose_table *table;
    /* Current entry */
    بنية xkb_compose_table_entry entry;
    /* Stack of pending nodes to process */
    darray(بنية xkb_compose_table_iterator_pending_node) pending_nodes;
};

بنية xkb_compose_table_iterator *
xkb_compose_table_iterator_new(بنية xkb_compose_table *table)
{
    بنية xkb_compose_table_iterator *iter;
    xkb_keysym_t *sequence;

    iter = calloc(1, حجم(*iter));
    אם (!iter) {
        החזר عدم;
    }
    iter->table = xkb_compose_table_ref(table);
    sequence = calloc(COMPOSE_MAX_LHS_LEN, حجم(xkb_keysym_t));
    אם (!sequence) {
        حرر(iter);
        החזר عدم;
    }
    iter->entry.sequence = sequence;
    iter->entry.sequence_length = 0;

    darray_init(iter->pending_nodes);
    /* Short-circuit if table contains only the dummy entry */
    אם (darray_size(table->nodes) == 1) {
        החזר iter;
    }
    /* Add root node */
    بنية xkb_compose_table_iterator_pending_node pending = {
        .offset = 1,
        .processed = false
    };
    darray_append(iter->pending_nodes, pending);
    ثابت بنية compose_node *node = &darray_item(iter->table->nodes,
                                                   pending.offset);

    /* Find the first left-most node and store intermediate nodes as pending */
    כלעוד (node->lokid) {
        pending.offset = node->lokid;
        darray_append(iter->pending_nodes, pending);
        node = &darray_item(iter->table->nodes, pending.offset);
    };

    החזר iter;
}

فراغ
xkb_compose_table_iterator_free(بنية xkb_compose_table_iterator *iter)
{
    xkb_compose_table_unref(iter->table);
    darray_free(iter->pending_nodes);
    حرر(iter->entry.sequence);
    حرر(iter);
}

بنية xkb_compose_table_entry *
xkb_compose_table_iterator_next(بنية xkb_compose_table_iterator *iter)
{
    /* Traversal algorithm (simplified):
     * 1. Resume last pending node from the stack as the current pending node.
     * 2. If the node is not yet processed, go to 5.
     * 3. Remove node from the stack and remove last keysym from the entry.
     * 4. If there is a right arrow, set it as the current pending node
     *    (unprocessed) and go to 6; else go to 1.
     * 5. Follow down arrow: set the pending node as processed, then:
     *      a) if it is a leaf, complete the entry and return it.
     *      b) else append the child node to the stack and set it as the current
     *         pending node.
     * 6. Find the next left-most arrow and store intermediate pending nodes.
     * 7. Go to 5.
     */

    بنية xkb_compose_table_iterator_pending_node *pending;
    ثابت بنية compose_node *node;

    /* Iterator is empty if there is no pending nodes */
    אם (unlikely(darray_empty(iter->pending_nodes))) {
        החזר عدم;
    }

    /* Resume to the last element in the stack */
    pending = &darray_item(iter->pending_nodes,
                           darray_size(iter->pending_nodes) - 1);
    node = &darray_item(iter->table->nodes, pending->offset);

    بنية xkb_compose_table_iterator_pending_node new = {
        .processed = false,
        .offset = 0
    };

    /* Remove processed leaves until an unprocessed right arrow or parent
     * is found, then update entry accordingly */
    כלעוד (pending->processed) {
        /* Remove last keysym */
        iter->entry.sequence_length--;
        אם (node->hikid) {
            /* Follow right arrow: replace current pending node */
            pending->processed = false;
            pending->offset = node->hikid;
            /* Process child’s left arrow */
            اذهب node_left;
        } אחרת {
            /* Remove processed node */
            darray_remove_last(iter->pending_nodes);
            אם (unlikely(darray_empty(iter->pending_nodes))) {
                /* No more nodes to process */
                החזר عدم;
            }
            /* Get parent node */
            pending = &darray_item(iter->pending_nodes,
                                   darray_size(iter->pending_nodes) - 1);
            node = &darray_item(iter->table->nodes, pending->offset);
        }
    }

    כלעוד (1) {
        /* Follow down arrow */
        pending->processed = true;
        iter->entry.sequence_length++;
        iter->entry.sequence[iter->entry.sequence_length - 1] = node->keysym;
        אם (node->is_leaf) {
            /* Leaf: return entry */
            iter->entry.keysym = node->leaf.keysym;
            iter->entry.utf8 = &darray_item(iter->table->utf8, node->leaf.utf8);
            החזר &iter->entry;
        }
        /* Not a leaf: process child node */
        new.offset = node->internal.eqkid;
        darray_append(iter->pending_nodes, new);
        pending = &darray_item(iter->pending_nodes,
                               darray_size(iter->pending_nodes) - 1);
node_left:
        node = &darray_item(iter->table->nodes, pending->offset);
        /* Find the next left-most arrow and store intermediate pending nodes */
        כלעוד (node->lokid) {
            /* Follow left arrow */
            new.offset = node->lokid;
            darray_append(iter->pending_nodes, new);
            pending = &darray_item(iter->pending_nodes,
                                   darray_size(iter->pending_nodes) - 1);
            node = &darray_item(iter->table->nodes, new.offset);
        }
    }
}
