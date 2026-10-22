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

স্থির কাঠামো xkb_compose_table *
xkb_compose_table_new(কাঠামো xkb_context *ctx, ধ্রুবক অক্ষর *func,
                      ধ্রুবক অক্ষর *locale,
                      গণনা xkb_compose_format format,
                      গণনা xkb_compose_compile_flags flags)
{
    ধ্রুবক গণনা xkb_compose_compile_flags invalid_flags = (
        flags & ~(গণনা xkb_compose_compile_flags)XKB_COMPOSE_COMPILE_FLAGS_মানসমূহ
    );
    যদি (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized flags: %#x\n", func, invalid_flags);
        ফেরত ফাঁকা;
    }

    যদি (format != XKB_COMPOSE_FORMAT_TEXT_V1) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unsupported compose format: %d\n", func, format);
        ফেরত ফাঁকা;
    }

    অক্ষর *resolved_locale;
    কাঠামো xkb_compose_table *table;
    কাঠামো compose_গ্রন্থি dummy = {0};

    resolved_locale = resolve_locale(ctx, locale);
    যদি (!resolved_locale)
        ফেরত ফাঁকা;

    table = calloc(1, আকার(*table));
    যদি (!table) {
        মুক্ত(resolved_locale);
        ফেরত ফাঁকা;
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

    ফেরত table;
}

কাঠামো xkb_compose_table *
xkb_compose_table_ref(কাঠামো xkb_compose_table *table)
{
    প্রতিজ্ঞা(table->refcnt > 0);
    table->refcnt++;
    ফেরত table;
}

শূন্য
xkb_compose_table_unref(কাঠামো xkb_compose_table *table)
{
    প্রতিজ্ঞা(!table || table->refcnt > 0);
    যদি (!table || --table->refcnt > 0)
        ফেরত;
    মুক্ত(table->locale);
    darray_free(table->nodes);
    darray_free(table->utf8);
    xkb_context_unref(table->ctx);
    মুক্ত(table);
}

কাঠামো xkb_compose_table *
xkb_compose_table_new_from_file(কাঠামো xkb_context *ctx,
                                FILE *file,
                                ধ্রুবক অক্ষর *locale,
                                গণনা xkb_compose_format format,
                                গণনা xkb_compose_compile_flags flags)
{
    কাঠামো xkb_compose_table * ধ্রুবক table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    যদি (!table)
        ফেরত ফাঁকা;

    যদি (!parse_file(table, file, "(unknown file)")) {
        xkb_compose_table_unref(table);
        ফেরত ফাঁকা;
    }

    ফেরত table;
}

কাঠামো xkb_compose_table *
xkb_compose_table_new_from_বাফার(কাঠামো xkb_context *ctx,
                                  ধ্রুবক অক্ষর *বাফার, আকার_t দৈর্ঘ্য,
                                  ধ্রুবক অক্ষর *locale,
                                  গণনা xkb_compose_format format,
                                  গণনা xkb_compose_compile_flags flags)
{
    কাঠামো xkb_compose_table * ধ্রুবক table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    যদি (!table)
        ফেরত ফাঁকা;

    যদি (!parse_string(table, বাফার, দৈর্ঘ্য, "(input string)")) {
        xkb_compose_table_unref(table);
        ফেরত ফাঁকা;
    }

    ফেরত table;
}

কাঠামো xkb_compose_table *
xkb_compose_table_new_from_locale(কাঠামো xkb_context *ctx,
                                  ধ্রুবক অক্ষর *locale,
                                  গণনা xkb_compose_compile_flags flags)
{
    স্থির ধ্রুবক গণনা xkb_compose_format format = XKB_COMPOSE_FORMAT_TEXT_V1;
    কাঠামো xkb_compose_table * ধ্রুবক table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    যদি (!table)
        ফেরত ফাঁকা;

    অক্ষর *path = get_xcomposefile_path(ctx);
    FILE *file = open_file(path);
    যদি (file)
        যাও found_path;
    মুক্ত(path);

    path = get_xdg_xcompose_file_path(ctx);
    file = open_file(path);
    যদি (file)
        যাও found_path;
    মুক্ত(path);

    path = get_home_xcompose_file_path(ctx);
    file = open_file(path);
    যদি (file)
        যাও found_path;
    মুক্ত(path);

    path = get_locale_compose_file_path(ctx, table->locale);
    file = open_file(path);
    যদি (file)
        যাও found_path;
    মুক্ত(path);

    log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
            "couldn't find a Compose file for locale \"%s\" (mapped to \"%s\")\n",
            locale, table->locale);
    xkb_compose_table_unref(table);
    ফেরত ফাঁকা;

found_path:
    {} /* Label followed by a declaration is a C23 extension */
    ধ্রুবক bool ok = parse_file(table, file, path);
    fclose(file);
    যদি (!ok) {
        মুক্ত(path);
        xkb_compose_table_unref(table);
        ফেরত ফাঁকা;
    }

    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "created compose table from locale %s with path %s\n",
            table->locale, path);

    মুক্ত(path);
    ফেরত table;
}

ধ্রুবক xkb_keysym_t *
xkb_compose_table_entry_sequence(কাঠামো xkb_compose_table_entry *entry,
                                 আকার_t *sequence_দৈর্ঘ্য)
{
    *sequence_দৈর্ঘ্য = entry->sequence_দৈর্ঘ্য;
    ফেরত entry->sequence;
}

xkb_keysym_t
xkb_compose_table_entry_keysym(কাঠামো xkb_compose_table_entry *entry)
{
    ফেরত entry->keysym;
}

ধ্রুবক অক্ষর *
xkb_compose_table_entry_utf8(কাঠামো xkb_compose_table_entry *entry)
{
    ফেরত entry->utf8;
}

#যদি MAX_COMPOSE_NODES_LOG2 > 31
#error "Cannot implement bit field xkb_compose_table_iterator_pending_node.offset"
#endif

কাঠামো xkb_compose_table_iterator_pending_গ্রন্থি {
    uint32_t offset:31;
    bool processed:1;
};

কাঠামো xkb_compose_table_iterator {
    কাঠামো xkb_compose_table *table;
    /* Current entry */
    কাঠামো xkb_compose_table_entry entry;
    /* Stack of pending nodes to process */
    darray(কাঠামো xkb_compose_table_iterator_pending_গ্রন্থি) pending_nodes;
};

কাঠামো xkb_compose_table_iterator *
xkb_compose_table_iterator_new(কাঠামো xkb_compose_table *table)
{
    কাঠামো xkb_compose_table_iterator *iter;
    xkb_keysym_t *sequence;

    iter = calloc(1, আকার(*iter));
    যদি (!iter) {
        ফেরত ফাঁকা;
    }
    iter->table = xkb_compose_table_ref(table);
    sequence = calloc(COMPOSE_MAX_LHS_LEN, আকার(xkb_keysym_t));
    যদি (!sequence) {
        মুক্ত(iter);
        ফেরত ফাঁকা;
    }
    iter->entry.sequence = sequence;
    iter->entry.sequence_দৈর্ঘ্য = 0;

    darray_init(iter->pending_nodes);
    /* Short-circuit if table contains only the dummy entry */
    যদি (darray_আকার(table->nodes) == 1) {
        ফেরত iter;
    }
    /* Add root node */
    কাঠামো xkb_compose_table_iterator_pending_গ্রন্থি pending = {
        .offset = 1,
        .processed = false
    };
    darray_append(iter->pending_nodes, pending);
    ধ্রুবক কাঠামো compose_গ্রন্থি *গ্রন্থি = &darray_item(iter->table->nodes,
                                                   pending.offset);

    /* Find the first left-most node and store intermediate nodes as pending */
    যখন (গ্রন্থি->lokid) {
        pending.offset = গ্রন্থি->lokid;
        darray_append(iter->pending_nodes, pending);
        গ্রন্থি = &darray_item(iter->table->nodes, pending.offset);
    };

    ফেরত iter;
}

শূন্য
xkb_compose_table_iterator_free(কাঠামো xkb_compose_table_iterator *iter)
{
    xkb_compose_table_unref(iter->table);
    darray_free(iter->pending_nodes);
    মুক্ত(iter->entry.sequence);
    মুক্ত(iter);
}

কাঠামো xkb_compose_table_entry *
xkb_compose_table_iterator_পরবর্তী(কাঠামো xkb_compose_table_iterator *iter)
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

    কাঠামো xkb_compose_table_iterator_pending_গ্রন্থি *pending;
    ধ্রুবক কাঠামো compose_গ্রন্থি *গ্রন্থি;

    /* Iterator is empty if there is no pending nodes */
    যদি (unlikely(darray_empty(iter->pending_nodes))) {
        ফেরত ফাঁকা;
    }

    /* Resume to the last element in the stack */
    pending = &darray_item(iter->pending_nodes,
                           darray_আকার(iter->pending_nodes) - 1);
    গ্রন্থি = &darray_item(iter->table->nodes, pending->offset);

    কাঠামো xkb_compose_table_iterator_pending_গ্রন্থি new = {
        .processed = false,
        .offset = 0
    };

    /* Remove processed leaves until an unprocessed right arrow or parent
     * is found, then update entry accordingly */
    যখন (pending->processed) {
        /* Remove last keysym */
        iter->entry.sequence_দৈর্ঘ্য--;
        যদি (গ্রন্থি->hikid) {
            /* Follow right arrow: replace current pending node */
            pending->processed = false;
            pending->offset = গ্রন্থি->hikid;
            /* Process child’s left arrow */
            যাও গ্রন্থি_বাম;
        } অন্যথা {
            /* Remove processed node */
            darray_remove_last(iter->pending_nodes);
            যদি (unlikely(darray_empty(iter->pending_nodes))) {
                /* No more nodes to process */
                ফেরত ফাঁকা;
            }
            /* Get parent node */
            pending = &darray_item(iter->pending_nodes,
                                   darray_আকার(iter->pending_nodes) - 1);
            গ্রন্থি = &darray_item(iter->table->nodes, pending->offset);
        }
    }

    যখন (1) {
        /* Follow down arrow */
        pending->processed = true;
        iter->entry.sequence_দৈর্ঘ্য++;
        iter->entry.sequence[iter->entry.sequence_দৈর্ঘ্য - 1] = গ্রন্থি->keysym;
        যদি (গ্রন্থি->is_leaf) {
            /* Leaf: return entry */
            iter->entry.keysym = গ্রন্থি->leaf.keysym;
            iter->entry.utf8 = &darray_item(iter->table->utf8, গ্রন্থি->leaf.utf8);
            ফেরত &iter->entry;
        }
        /* Not a leaf: process child node */
        new.offset = গ্রন্থি->internal.eqkid;
        darray_append(iter->pending_nodes, new);
        pending = &darray_item(iter->pending_nodes,
                               darray_আকার(iter->pending_nodes) - 1);
গ্রন্থি_বাম:
        গ্রন্থি = &darray_item(iter->table->nodes, pending->offset);
        /* Find the next left-most arrow and store intermediate pending nodes */
        যখন (গ্রন্থি->lokid) {
            /* Follow left arrow */
            new.offset = গ্রন্থি->lokid;
            darray_append(iter->pending_nodes, new);
            pending = &darray_item(iter->pending_nodes,
                                   darray_আকার(iter->pending_nodes) - 1);
            গ্রন্থি = &darray_item(iter->table->nodes, new.offset);
        }
    }
}
