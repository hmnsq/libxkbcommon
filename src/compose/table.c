/*
 * Copyright © 2013,2021 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل <assert.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "features/enums.h"
#شامل "constants.h"
#شامل "messages-codes.h"
#شامل "parser.h"
#شامل "paths.h"
#شامل "table.h"
#شامل "utils.h"

ایستا ساختار xkb_compose_table *
xkb_compose_table_new(ساختار xkb_context *ctx, ثابت نویسه *func,
                      ثابت نویسه *locale,
                      شمارش xkb_compose_format format,
                      شمارش xkb_compose_compile_flags flags)
{
    ثابت شمارش xkb_compose_compile_flags invalid_flags = (
        flags & ~(شمارش xkb_compose_compile_flags)XKB_COMPOSE_COMPILE_FLAGS_مقادیر
    );
    اگر (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized flags: %#x\n", func, invalid_flags);
        بازگشت پوچ;
    }

    اگر (format != XKB_COMPOSE_FORMAT_TEXT_V1) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unsupported compose format: %d\n", func, format);
        بازگشت پوچ;
    }

    نویسه *resolved_locale;
    ساختار xkb_compose_table *table;
    ساختار compose_گره dummy = {0};

    resolved_locale = resolve_locale(ctx, locale);
    اگر (!resolved_locale)
        بازگشت پوچ;

    table = calloc(1, اندازه(*table));
    اگر (!table) {
        آزاد(resolved_locale);
        بازگشت پوچ;
    }

    table->refcnt = 1;
    table->ctx = xkb_context_ref(ctx);

    table->locale = resolved_locale;
    table->format = format;
    table->flags = flags;

    darray_init(table->nodes);
    darray_init(table->utf8);

    dummy.keysym = XKB_کلید_No_نماد;
    dummy.leaf.is_leaf = true;
    dummy.leaf.utf8 = 0;
    dummy.leaf.keysym = XKB_کلید_No_نماد;
    darray_append(table->nodes, dummy);

    darray_append(table->utf8, '\0');

    بازگشت table;
}

ساختار xkb_compose_table *
xkb_compose_table_ref(ساختار xkb_compose_table *table)
{
    أكد(table->refcnt > 0);
    table->refcnt++;
    بازگشت table;
}

تهی
xkb_compose_table_unref(ساختار xkb_compose_table *table)
{
    أكد(!table || table->refcnt > 0);
    اگر (!table || --table->refcnt > 0)
        بازگشت;
    آزاد(table->locale);
    darray_free(table->nodes);
    darray_free(table->utf8);
    xkb_context_unref(table->ctx);
    آزاد(table);
}

ساختار xkb_compose_table *
xkb_compose_table_new_from_file(ساختار xkb_context *ctx,
                                FILE *file,
                                ثابت نویسه *locale,
                                شمارش xkb_compose_format format,
                                شمارش xkb_compose_compile_flags flags)
{
    ساختار xkb_compose_table * ثابت table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    اگر (!table)
        بازگشت پوچ;

    اگر (!parse_file(table, file, "(unknown file)")) {
        xkb_compose_table_unref(table);
        بازگشت پوچ;
    }

    بازگشت table;
}

ساختار xkb_compose_table *
xkb_compose_table_new_from_buffer(ساختار xkb_context *ctx,
                                  ثابت نویسه *buffer, size_t length,
                                  ثابت نویسه *locale,
                                  شمارش xkb_compose_format format,
                                  شمارش xkb_compose_compile_flags flags)
{
    ساختار xkb_compose_table * ثابت table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    اگر (!table)
        بازگشت پوچ;

    اگر (!parse_string(table, buffer, length, "(input string)")) {
        xkb_compose_table_unref(table);
        بازگشت پوچ;
    }

    بازگشت table;
}

ساختار xkb_compose_table *
xkb_compose_table_new_from_locale(ساختار xkb_context *ctx,
                                  ثابت نویسه *locale,
                                  شمارش xkb_compose_compile_flags flags)
{
    ایستا ثابت شمارش xkb_compose_format format = XKB_COMPOSE_FORMAT_TEXT_V1;
    ساختار xkb_compose_table * ثابت table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    اگر (!table)
        بازگشت پوچ;

    نویسه *path = get_xcomposefile_path(ctx);
    FILE *file = open_file(path);
    اگر (file)
        برو found_path;
    آزاد(path);

    path = get_xdg_xcompose_file_path(ctx);
    file = open_file(path);
    اگر (file)
        برو found_path;
    آزاد(path);

    path = get_home_xcompose_file_path(ctx);
    file = open_file(path);
    اگر (file)
        برو found_path;
    آزاد(path);

    path = get_locale_compose_file_path(ctx, table->locale);
    file = open_file(path);
    اگر (file)
        برو found_path;
    آزاد(path);

    log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
            "couldn't find a Compose file for locale \"%s\" (mapped to \"%s\")\n",
            locale, table->locale);
    xkb_compose_table_unref(table);
    بازگشت پوچ;

found_path:
    {} /* Label followed by a declaration is a C23 extension */
    ثابت bool ok = parse_file(table, file, path);
    fclose(file);
    اگر (!ok) {
        آزاد(path);
        xkb_compose_table_unref(table);
        بازگشت پوچ;
    }

    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "created compose table from locale %s with path %s\n",
            table->locale, path);

    آزاد(path);
    بازگشت table;
}

ثابت xkb_keysym_t *
xkb_compose_table_entry_sequence(ساختار xkb_compose_table_entry *entry,
                                 size_t *sequence_length)
{
    *sequence_length = entry->sequence_length;
    بازگشت entry->sequence;
}

xkb_keysym_t
xkb_compose_table_entry_keysym(ساختار xkb_compose_table_entry *entry)
{
    بازگشت entry->keysym;
}

ثابت نویسه *
xkb_compose_table_entry_utf8(ساختار xkb_compose_table_entry *entry)
{
    بازگشت entry->utf8;
}

#اگر MAX_COMPOSE_NODES_LOG2 > 31
#error "Cannot implement bit field xkb_compose_table_iterator_pending_node.offset"
#endif

ساختار xkb_compose_table_iterator_pending_گره {
    uint32_t offset:31;
    bool processed:1;
};

ساختار xkb_compose_table_iterator {
    ساختار xkb_compose_table *table;
    /* Current entry */
    ساختار xkb_compose_table_entry entry;
    /* Stack of pending nodes to process */
    darray(ساختار xkb_compose_table_iterator_pending_گره) pending_nodes;
};

ساختار xkb_compose_table_iterator *
xkb_compose_table_iterator_new(ساختار xkb_compose_table *table)
{
    ساختار xkb_compose_table_iterator *iter;
    xkb_keysym_t *sequence;

    iter = calloc(1, اندازه(*iter));
    اگر (!iter) {
        بازگشت پوچ;
    }
    iter->table = xkb_compose_table_ref(table);
    sequence = calloc(COMPOSE_MAX_LHS_LEN, اندازه(xkb_keysym_t));
    اگر (!sequence) {
        آزاد(iter);
        بازگشت پوچ;
    }
    iter->entry.sequence = sequence;
    iter->entry.sequence_length = 0;

    darray_init(iter->pending_nodes);
    /* Short-circuit if table contains only the dummy entry */
    اگر (darray_size(table->nodes) == 1) {
        بازگشت iter;
    }
    /* Add root node */
    ساختار xkb_compose_table_iterator_pending_گره pending = {
        .offset = 1,
        .processed = false
    };
    darray_append(iter->pending_nodes, pending);
    ثابت ساختار compose_گره *گره = &darray_item(iter->table->nodes,
                                                   pending.offset);

    /* Find the first left-most node and store intermediate nodes as pending */
    وقتی (گره->lokid) {
        pending.offset = گره->lokid;
        darray_append(iter->pending_nodes, pending);
        گره = &darray_item(iter->table->nodes, pending.offset);
    };

    بازگشت iter;
}

تهی
xkb_compose_table_iterator_free(ساختار xkb_compose_table_iterator *iter)
{
    xkb_compose_table_unref(iter->table);
    darray_free(iter->pending_nodes);
    آزاد(iter->entry.sequence);
    آزاد(iter);
}

ساختار xkb_compose_table_entry *
xkb_compose_table_iterator_next(ساختار xkb_compose_table_iterator *iter)
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

    ساختار xkb_compose_table_iterator_pending_گره *pending;
    ثابت ساختار compose_گره *گره;

    /* Iterator is empty if there is no pending nodes */
    اگر (unlikely(darray_empty(iter->pending_nodes))) {
        بازگشت پوچ;
    }

    /* Resume to the last element in the stack */
    pending = &darray_item(iter->pending_nodes,
                           darray_size(iter->pending_nodes) - 1);
    گره = &darray_item(iter->table->nodes, pending->offset);

    ساختار xkb_compose_table_iterator_pending_گره new = {
        .processed = false,
        .offset = 0
    };

    /* Remove processed leaves until an unprocessed right arrow or parent
     * is found, then update entry accordingly */
    وقتی (pending->processed) {
        /* Remove last keysym */
        iter->entry.sequence_length--;
        اگر (گره->hikid) {
            /* Follow right arrow: replace current pending node */
            pending->processed = false;
            pending->offset = گره->hikid;
            /* Process child’s left arrow */
            برو گره_چپ;
        } وگرنه {
            /* Remove processed node */
            darray_remove_last(iter->pending_nodes);
            اگر (unlikely(darray_empty(iter->pending_nodes))) {
                /* No more nodes to process */
                بازگشت پوچ;
            }
            /* Get parent node */
            pending = &darray_item(iter->pending_nodes,
                                   darray_size(iter->pending_nodes) - 1);
            گره = &darray_item(iter->table->nodes, pending->offset);
        }
    }

    وقتی (1) {
        /* Follow down arrow */
        pending->processed = true;
        iter->entry.sequence_length++;
        iter->entry.sequence[iter->entry.sequence_length - 1] = گره->keysym;
        اگر (گره->is_leaf) {
            /* Leaf: return entry */
            iter->entry.keysym = گره->leaf.keysym;
            iter->entry.utf8 = &darray_item(iter->table->utf8, گره->leaf.utf8);
            بازگشت &iter->entry;
        }
        /* Not a leaf: process child node */
        new.offset = گره->internal.eqkid;
        darray_append(iter->pending_nodes, new);
        pending = &darray_item(iter->pending_nodes,
                               darray_size(iter->pending_nodes) - 1);
گره_چپ:
        گره = &darray_item(iter->table->nodes, pending->offset);
        /* Find the next left-most arrow and store intermediate pending nodes */
        وقتی (گره->lokid) {
            /* Follow left arrow */
            new.offset = گره->lokid;
            darray_append(iter->pending_nodes, new);
            pending = &darray_item(iter->pending_nodes,
                                   darray_size(iter->pending_nodes) - 1);
            گره = &darray_item(iter->table->nodes, new.offset);
        }
    }
}
