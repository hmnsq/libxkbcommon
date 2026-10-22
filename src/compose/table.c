/*
 * Copyright © 2013,2021 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <assert.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "features/enums.h"
#অন্তর্ভাব "constants.h"
#অন্তর্ভাব "messages-codes.h"
#অন্তর্ভাব "parser.h"
#অন্তর্ভাব "paths.h"
#অন্তর্ভাব "table.h"
#অন্তর্ভাব "utils.h"

স্থির সংরচনা xkb_compose_table *
xkb_compose_table_new(সংরচনা xkb_context *ctx, ধ্রুব বর্ণ *func,
                      ধ্রুব বর্ণ *locale,
                      গণনা xkb_compose_format format,
                      গণনা xkb_compose_compile_flags flags)
{
    ধ্রুব গণনা xkb_compose_compile_flags invalid_flags = (
        flags & ~(গণনা xkb_compose_compile_flags)XKB_COMPOSE_COMPILE_FLAGS_মানসমূহ
    );
    যদি (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized flags: %#x\n", func, invalid_flags);
        প্রত্যাগম রিক্ত;
    }

    যদি (format != XKB_COMPOSE_FORMAT_TEXT_V1) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unsupported compose format: %d\n", func, format);
        প্রত্যাগম রিক্ত;
    }

    বর্ণ *resolved_locale;
    সংরচনা xkb_compose_table *table;
    সংরচনা compose_গ্রন্থি dummy = {0};

    resolved_locale = resolve_locale(ctx, locale);
    যদি (!resolved_locale)
        প্রত্যাগম রিক্ত;

    table = calloc(1, পরিমাণ(*table));
    যদি (!table) {
        মুক্ত(resolved_locale);
        প্রত্যাগম রিক্ত;
    }

    table->refcnt = 1;
    table->ctx = xkb_context_ref(ctx);

    table->locale = resolved_locale;
    table->format = format;
    table->flags = flags;

    darray_init(table->nodes);
    darray_init(table->utf8);

    dummy.keysym = XKB_চাবি_No_প্রতীক;
    dummy.leaf.is_leaf = true;
    dummy.leaf.utf8 = 0;
    dummy.leaf.keysym = XKB_চাবি_No_প্রতীক;
    darray_append(table->nodes, dummy);

    darray_append(table->utf8, '\0');

    প্রত্যাগম table;
}

সংরচনা xkb_compose_table *
xkb_compose_table_ref(সংরচনা xkb_compose_table *table)
{
    প্রতিজ্ঞা(table->refcnt > 0);
    table->refcnt++;
    প্রত্যাগম table;
}

শূন্য
xkb_compose_table_unref(সংরচনা xkb_compose_table *table)
{
    প্রতিজ্ঞা(!table || table->refcnt > 0);
    যদি (!table || --table->refcnt > 0)
        প্রত্যাগম;
    মুক্ত(table->locale);
    darray_free(table->nodes);
    darray_free(table->utf8);
    xkb_context_unref(table->ctx);
    মুক্ত(table);
}

সংরচনা xkb_compose_table *
xkb_compose_table_new_from_file(সংরচনা xkb_context *ctx,
                                FILE *file,
                                ধ্রুব বর্ণ *locale,
                                গণনা xkb_compose_format format,
                                গণনা xkb_compose_compile_flags flags)
{
    সংরচনা xkb_compose_table * ধ্রুব table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    যদি (!table)
        প্রত্যাগম রিক্ত;

    যদি (!parse_file(table, file, "(unknown file)")) {
        xkb_compose_table_unref(table);
        প্রত্যাগম রিক্ত;
    }

    প্রত্যাগম table;
}

সংরচনা xkb_compose_table *
xkb_compose_table_new_from_buffer(সংরচনা xkb_context *ctx,
                                  ধ্রুব বর্ণ *buffer, size_t length,
                                  ধ্রুব বর্ণ *locale,
                                  গণনা xkb_compose_format format,
                                  গণনা xkb_compose_compile_flags flags)
{
    সংরচনা xkb_compose_table * ধ্রুব table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    যদি (!table)
        প্রত্যাগম রিক্ত;

    যদি (!parse_string(table, buffer, length, "(input string)")) {
        xkb_compose_table_unref(table);
        প্রত্যাগম রিক্ত;
    }

    প্রত্যাগম table;
}

সংরচনা xkb_compose_table *
xkb_compose_table_new_from_locale(সংরচনা xkb_context *ctx,
                                  ধ্রুব বর্ণ *locale,
                                  গণনা xkb_compose_compile_flags flags)
{
    স্থির ধ্রুব গণনা xkb_compose_format format = XKB_COMPOSE_FORMAT_TEXT_V1;
    সংরচনা xkb_compose_table * ধ্রুব table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    যদি (!table)
        প্রত্যাগম রিক্ত;

    বর্ণ *path = get_xcomposefile_path(ctx);
    FILE *file = open_file(path);
    যদি (file)
        গচ্ছ found_path;
    মুক্ত(path);

    path = get_xdg_xcompose_file_path(ctx);
    file = open_file(path);
    যদি (file)
        গচ্ছ found_path;
    মুক্ত(path);

    path = get_home_xcompose_file_path(ctx);
    file = open_file(path);
    যদি (file)
        গচ্ছ found_path;
    মুক্ত(path);

    path = get_locale_compose_file_path(ctx, table->locale);
    file = open_file(path);
    যদি (file)
        গচ্ছ found_path;
    মুক্ত(path);

    log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
            "couldn't find a Compose file for locale \"%s\" (mapped to \"%s\")\n",
            locale, table->locale);
    xkb_compose_table_unref(table);
    প্রত্যাগম রিক্ত;

found_path:
    {} /* Label followed by a declaration is a C23 extension */
    ধ্রুব bool ok = parse_file(table, file, path);
    fclose(file);
    যদি (!ok) {
        মুক্ত(path);
        xkb_compose_table_unref(table);
        প্রত্যাগম রিক্ত;
    }

    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "created compose table from locale %s with path %s\n",
            table->locale, path);

    মুক্ত(path);
    প্রত্যাগম table;
}

ধ্রুব xkb_keysym_t *
xkb_compose_table_entry_sequence(সংরচনা xkb_compose_table_entry *entry,
                                 size_t *sequence_length)
{
    *sequence_length = entry->sequence_length;
    প্রত্যাগম entry->sequence;
}

xkb_keysym_t
xkb_compose_table_entry_keysym(সংরচনা xkb_compose_table_entry *entry)
{
    প্রত্যাগম entry->keysym;
}

ধ্রুব বর্ণ *
xkb_compose_table_entry_utf8(সংরচনা xkb_compose_table_entry *entry)
{
    প্রত্যাগম entry->utf8;
}

#যদি MAX_COMPOSE_NODES_LOG2 > 31
#error "Cannot implement bit field xkb_compose_table_iterator_pending_node.offset"
#endif

সংরচনা xkb_compose_table_iterator_pending_গ্রন্থি {
    uint32_t offset:31;
    bool processed:1;
};

সংরচনা xkb_compose_table_iterator {
    সংরচনা xkb_compose_table *table;
    /* Current entry */
    সংরচনা xkb_compose_table_entry entry;
    /* Stack of pending nodes to process */
    darray(সংরচনা xkb_compose_table_iterator_pending_গ্রন্থি) pending_nodes;
};

সংরচনা xkb_compose_table_iterator *
xkb_compose_table_iterator_new(সংরচনা xkb_compose_table *table)
{
    সংরচনা xkb_compose_table_iterator *iter;
    xkb_keysym_t *sequence;

    iter = calloc(1, পরিমাণ(*iter));
    যদি (!iter) {
        প্রত্যাগম রিক্ত;
    }
    iter->table = xkb_compose_table_ref(table);
    sequence = calloc(COMPOSE_MAX_LHS_LEN, পরিমাণ(xkb_keysym_t));
    যদি (!sequence) {
        মুক্ত(iter);
        প্রত্যাগম রিক্ত;
    }
    iter->entry.sequence = sequence;
    iter->entry.sequence_length = 0;

    darray_init(iter->pending_nodes);
    /* Short-circuit if table contains only the dummy entry */
    যদি (darray_size(table->nodes) == 1) {
        প্রত্যাগম iter;
    }
    /* Add root node */
    সংরচনা xkb_compose_table_iterator_pending_গ্রন্থি pending = {
        .offset = 1,
        .processed = false
    };
    darray_append(iter->pending_nodes, pending);
    ধ্রুব সংরচনা compose_গ্রন্থি *গ্রন্থি = &darray_item(iter->table->nodes,
                                                   pending.offset);

    /* Find the first left-most node and store intermediate nodes as pending */
    যাবত্ (গ্রন্থি->lokid) {
        pending.offset = গ্রন্থি->lokid;
        darray_append(iter->pending_nodes, pending);
        গ্রন্থি = &darray_item(iter->table->nodes, pending.offset);
    };

    প্রত্যাগম iter;
}

শূন্য
xkb_compose_table_iterator_free(সংরচনা xkb_compose_table_iterator *iter)
{
    xkb_compose_table_unref(iter->table);
    darray_free(iter->pending_nodes);
    মুক্ত(iter->entry.sequence);
    মুক্ত(iter);
}

সংরচনা xkb_compose_table_entry *
xkb_compose_table_iterator_next(সংরচনা xkb_compose_table_iterator *iter)
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

    সংরচনা xkb_compose_table_iterator_pending_গ্রন্থি *pending;
    ধ্রুব সংরচনা compose_গ্রন্থি *গ্রন্থি;

    /* Iterator is empty if there is no pending nodes */
    যদি (unlikely(darray_empty(iter->pending_nodes))) {
        প্রত্যাগম রিক্ত;
    }

    /* Resume to the last element in the stack */
    pending = &darray_item(iter->pending_nodes,
                           darray_size(iter->pending_nodes) - 1);
    গ্রন্থি = &darray_item(iter->table->nodes, pending->offset);

    সংরচনা xkb_compose_table_iterator_pending_গ্রন্থি new = {
        .processed = false,
        .offset = 0
    };

    /* Remove processed leaves until an unprocessed right arrow or parent
     * is found, then update entry accordingly */
    যাবত্ (pending->processed) {
        /* Remove last keysym */
        iter->entry.sequence_length--;
        যদি (গ্রন্থি->hikid) {
            /* Follow right arrow: replace current pending node */
            pending->processed = false;
            pending->offset = গ্রন্থি->hikid;
            /* Process child’s left arrow */
            গচ্ছ গ্রন্থি_বাম;
        } অন্যথা {
            /* Remove processed node */
            darray_remove_last(iter->pending_nodes);
            যদি (unlikely(darray_empty(iter->pending_nodes))) {
                /* No more nodes to process */
                প্রত্যাগম রিক্ত;
            }
            /* Get parent node */
            pending = &darray_item(iter->pending_nodes,
                                   darray_size(iter->pending_nodes) - 1);
            গ্রন্থি = &darray_item(iter->table->nodes, pending->offset);
        }
    }

    যাবত্ (1) {
        /* Follow down arrow */
        pending->processed = true;
        iter->entry.sequence_length++;
        iter->entry.sequence[iter->entry.sequence_length - 1] = গ্রন্থি->keysym;
        যদি (গ্রন্থি->is_leaf) {
            /* Leaf: return entry */
            iter->entry.keysym = গ্রন্থি->leaf.keysym;
            iter->entry.utf8 = &darray_item(iter->table->utf8, গ্রন্থি->leaf.utf8);
            প্রত্যাগম &iter->entry;
        }
        /* Not a leaf: process child node */
        new.offset = গ্রন্থি->internal.eqkid;
        darray_append(iter->pending_nodes, new);
        pending = &darray_item(iter->pending_nodes,
                               darray_size(iter->pending_nodes) - 1);
গ্রন্থি_বাম:
        গ্রন্থি = &darray_item(iter->table->nodes, pending->offset);
        /* Find the next left-most arrow and store intermediate pending nodes */
        যাবত্ (গ্রন্থি->lokid) {
            /* Follow left arrow */
            new.offset = গ্রন্থি->lokid;
            darray_append(iter->pending_nodes, new);
            pending = &darray_item(iter->pending_nodes,
                                   darray_size(iter->pending_nodes) - 1);
            গ্রন্থি = &darray_item(iter->table->nodes, new.offset);
        }
    }
}
