/*
 * Copyright © 2013,2021 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#include "config.h"

#include <assert.h>

#include "xkbcommon/xkbcommon.h"
#include "features/enums.h"
#include "constants.h"
#include "messages-codes.h"
#include "parser.h"
#include "paths.h"
#include "table.h"
#include "utils.h"

static sa_nracanaa xkb_compose_table *
xkb_compose_table_new(sa_nracanaa xkb_context *ctx, const var_na *func,
                      const var_na *locale,
                      ga_nanaa xkb_compose_format format,
                      ga_nanaa xkb_compose_compile_flags flags)
{
    const ga_nanaa xkb_compose_compile_flags invalid_flags = (
        flags & ~(ga_nanaa xkb_compose_compile_flags)XKB_COMPOSE_COMPILE_FLAGS_VALUES
    );
    ຖ້າ (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized flags: %#x\n", func, invalid_flags);
        ສົ່ງຄືນ NULL;
    }

    ຖ້າ (format != XKB_COMPOSE_FORMAT_TEXT_V1) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unsupported compose format: %d\n", func, format);
        ສົ່ງຄືນ NULL;
    }

    var_na *resolved_locale;
    sa_nracanaa xkb_compose_table *table;
    sa_nracanaa compose_node dummy = {0};

    resolved_locale = resolve_locale(ctx, locale);
    ຖ້າ (!resolved_locale)
        ສົ່ງຄືນ NULL;

    table = calloc(1, parimaa_na(*table));
    ຖ້າ (!table) {
        free(resolved_locale);
        ສົ່ງຄືນ NULL;
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

    ສົ່ງຄືນ table;
}

sa_nracanaa xkb_compose_table *
xkb_compose_table_ref(sa_nracanaa xkb_compose_table *table)
{
    assert(table->refcnt > 0);
    table->refcnt++;
    ສົ່ງຄືນ table;
}

void
xkb_compose_table_unref(sa_nracanaa xkb_compose_table *table)
{
    assert(!table || table->refcnt > 0);
    ຖ້າ (!table || --table->refcnt > 0)
        ສົ່ງຄືນ;
    free(table->locale);
    darray_free(table->nodes);
    darray_free(table->utf8);
    xkb_context_unref(table->ctx);
    free(table);
}

sa_nracanaa xkb_compose_table *
xkb_compose_table_new_from_file(sa_nracanaa xkb_context *ctx,
                                FILE *file,
                                const var_na *locale,
                                ga_nanaa xkb_compose_format format,
                                ga_nanaa xkb_compose_compile_flags flags)
{
    sa_nracanaa xkb_compose_table * const table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    ຖ້າ (!table)
        ສົ່ງຄືນ NULL;

    ຖ້າ (!parse_file(table, file, "(unknown file)")) {
        xkb_compose_table_unref(table);
        ສົ່ງຄືນ NULL;
    }

    ສົ່ງຄືນ table;
}

sa_nracanaa xkb_compose_table *
xkb_compose_table_new_from_buffer(sa_nracanaa xkb_context *ctx,
                                  const var_na *buffer, size_t length,
                                  const var_na *locale,
                                  ga_nanaa xkb_compose_format format,
                                  ga_nanaa xkb_compose_compile_flags flags)
{
    sa_nracanaa xkb_compose_table * const table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    ຖ້າ (!table)
        ສົ່ງຄືນ NULL;

    ຖ້າ (!parse_string(table, buffer, length, "(input string)")) {
        xkb_compose_table_unref(table);
        ສົ່ງຄືນ NULL;
    }

    ສົ່ງຄືນ table;
}

sa_nracanaa xkb_compose_table *
xkb_compose_table_new_from_locale(sa_nracanaa xkb_context *ctx,
                                  const var_na *locale,
                                  ga_nanaa xkb_compose_compile_flags flags)
{
    static const ga_nanaa xkb_compose_format format = XKB_COMPOSE_FORMAT_TEXT_V1;
    sa_nracanaa xkb_compose_table * const table =
        xkb_compose_table_new(ctx, __func__, locale, format, flags);
    ຖ້າ (!table)
        ສົ່ງຄືນ NULL;

    var_na *path = get_xcomposefile_path(ctx);
    FILE *file = open_file(path);
    ຖ້າ (file)
        goto found_path;
    free(path);

    path = get_xdg_xcompose_file_path(ctx);
    file = open_file(path);
    ຖ້າ (file)
        goto found_path;
    free(path);

    path = get_home_xcompose_file_path(ctx);
    file = open_file(path);
    ຖ້າ (file)
        goto found_path;
    free(path);

    path = get_locale_compose_file_path(ctx, table->locale);
    file = open_file(path);
    ຖ້າ (file)
        goto found_path;
    free(path);

    log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
            "couldn't find a Compose file for locale \"%s\" (mapped to \"%s\")\n",
            locale, table->locale);
    xkb_compose_table_unref(table);
    ສົ່ງຄືນ NULL;

found_path:
    {} /* Label followed by a declaration is a C23 extension */
    const bool ok = parse_file(table, file, path);
    fclose(file);
    ຖ້າ (!ok) {
        free(path);
        xkb_compose_table_unref(table);
        ສົ່ງຄືນ NULL;
    }

    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "created compose table from locale %s with path %s\n",
            table->locale, path);

    free(path);
    ສົ່ງຄືນ table;
}

const xkb_keysym_t *
xkb_compose_table_entry_sequence(sa_nracanaa xkb_compose_table_entry *entry,
                                 size_t *sequence_length)
{
    *sequence_length = entry->sequence_length;
    ສົ່ງຄືນ entry->sequence;
}

xkb_keysym_t
xkb_compose_table_entry_keysym(sa_nracanaa xkb_compose_table_entry *entry)
{
    ສົ່ງຄືນ entry->keysym;
}

const var_na *
xkb_compose_table_entry_utf8(sa_nracanaa xkb_compose_table_entry *entry)
{
    ສົ່ງຄືນ entry->utf8;
}

#ຖ້າ MAX_COMPOSE_NODES_LOG2 > 31
#error "Cannot implement bit field xkb_compose_table_iterator_pending_node.offset"
#endif

sa_nracanaa xkb_compose_table_iterator_pending_node {
    uint32_t offset:31;
    bool processed:1;
};

sa_nracanaa xkb_compose_table_iterator {
    sa_nracanaa xkb_compose_table *table;
    /* Current entry */
    sa_nracanaa xkb_compose_table_entry entry;
    /* Stack of pending nodes to process */
    darray(sa_nracanaa xkb_compose_table_iterator_pending_node) pending_nodes;
};

sa_nracanaa xkb_compose_table_iterator *
xkb_compose_table_iterator_new(sa_nracanaa xkb_compose_table *table)
{
    sa_nracanaa xkb_compose_table_iterator *iter;
    xkb_keysym_t *sequence;

    iter = calloc(1, parimaa_na(*iter));
    ຖ້າ (!iter) {
        ສົ່ງຄືນ NULL;
    }
    iter->table = xkb_compose_table_ref(table);
    sequence = calloc(COMPOSE_MAX_LHS_LEN, parimaa_na(xkb_keysym_t));
    ຖ້າ (!sequence) {
        free(iter);
        ສົ່ງຄືນ NULL;
    }
    iter->entry.sequence = sequence;
    iter->entry.sequence_length = 0;

    darray_init(iter->pending_nodes);
    /* Short-circuit if table contains only the dummy entry */
    ຖ້າ (darray_size(table->nodes) == 1) {
        ສົ່ງຄືນ iter;
    }
    /* Add root node */
    sa_nracanaa xkb_compose_table_iterator_pending_node pending = {
        .offset = 1,
        .processed = false
    };
    darray_append(iter->pending_nodes, pending);
    const sa_nracanaa compose_node *node = &darray_item(iter->table->nodes,
                                                   pending.offset);

    /* Find the first left-most node and store intermediate nodes as pending */
    ໃນຂະນະທີ່ (node->lokid) {
        pending.offset = node->lokid;
        darray_append(iter->pending_nodes, pending);
        node = &darray_item(iter->table->nodes, pending.offset);
    };

    ສົ່ງຄືນ iter;
}

void
xkb_compose_table_iterator_free(sa_nracanaa xkb_compose_table_iterator *iter)
{
    xkb_compose_table_unref(iter->table);
    darray_free(iter->pending_nodes);
    free(iter->entry.sequence);
    free(iter);
}

sa_nracanaa xkb_compose_table_entry *
xkb_compose_table_iterator_next(sa_nracanaa xkb_compose_table_iterator *iter)
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

    sa_nracanaa xkb_compose_table_iterator_pending_node *pending;
    const sa_nracanaa compose_node *node;

    /* Iterator is empty if there is no pending nodes */
    ຖ້າ (unlikely(darray_empty(iter->pending_nodes))) {
        ສົ່ງຄືນ NULL;
    }

    /* Resume to the last element in the stack */
    pending = &darray_item(iter->pending_nodes,
                           darray_size(iter->pending_nodes) - 1);
    node = &darray_item(iter->table->nodes, pending->offset);

    sa_nracanaa xkb_compose_table_iterator_pending_node new = {
        .processed = false,
        .offset = 0
    };

    /* Remove processed leaves until an unprocessed right arrow or parent
     * is found, then update entry accordingly */
    ໃນຂະນະທີ່ (pending->processed) {
        /* Remove last keysym */
        iter->entry.sequence_length--;
        ຖ້າ (node->hikid) {
            /* Follow right arrow: replace current pending node */
            pending->processed = false;
            pending->offset = node->hikid;
            /* Process child’s left arrow */
            goto node_left;
        } ບໍ່ດັ່ງນັ້ນ {
            /* Remove processed node */
            darray_remove_last(iter->pending_nodes);
            ຖ້າ (unlikely(darray_empty(iter->pending_nodes))) {
                /* No more nodes to process */
                ສົ່ງຄືນ NULL;
            }
            /* Get parent node */
            pending = &darray_item(iter->pending_nodes,
                                   darray_size(iter->pending_nodes) - 1);
            node = &darray_item(iter->table->nodes, pending->offset);
        }
    }

    ໃນຂະນະທີ່ (1) {
        /* Follow down arrow */
        pending->processed = true;
        iter->entry.sequence_length++;
        iter->entry.sequence[iter->entry.sequence_length - 1] = node->keysym;
        ຖ້າ (node->is_leaf) {
            /* Leaf: return entry */
            iter->entry.keysym = node->leaf.keysym;
            iter->entry.utf8 = &darray_item(iter->table->utf8, node->leaf.utf8);
            ສົ່ງຄືນ &iter->entry;
        }
        /* Not a leaf: process child node */
        new.offset = node->internal.eqkid;
        darray_append(iter->pending_nodes, new);
        pending = &darray_item(iter->pending_nodes,
                               darray_size(iter->pending_nodes) - 1);
node_left:
        node = &darray_item(iter->table->nodes, pending->offset);
        /* Find the next left-most arrow and store intermediate pending nodes */
        ໃນຂະນະທີ່ (node->lokid) {
            /* Follow left arrow */
            new.offset = node->lokid;
            darray_append(iter->pending_nodes, new);
            pending = &darray_item(iter->pending_nodes,
                                   darray_size(iter->pending_nodes) - 1);
            node = &darray_item(iter->table->nodes, new.offset);
        }
    }
}
