/*
 * Copyright © 2013,2021 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "features/enums.h"
#అన్తర్భావ "constants.h"
#అన్తర్భావ "messages-codes.h"
#అన్తర్భావ "parser.h"
#అన్తర్భావ "paths.h"
#అన్తర్భావ "table.h"
#అన్తర్భావ "utils.h"

స్థిర నిర్మాణం xkb_compose_పట్టిక *
xkb_compose_పట్టిక_new(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం *func,
                      స్థిరరాశి అక్షరం *locale,
                      గణన xkb_compose_format format,
                      గణన xkb_compose_compile_flags flags)
{
    స్థిరరాశి గణన xkb_compose_compile_flags invalid_flags = (
        flags & ~(గణన xkb_compose_compile_flags)XKB_COMPOSE_COMPILE_FLAGS_విలువలు
    );
    ఒకవేళ (invalid_flags) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "%s: unrecognized flags: %#x\n", func, invalid_flags);
        తిరిగి ఖాళీ;
    }

    ఒకవేళ (format != XKB_COMPOSE_FORMAT_పాఠ్యం_V1) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "%s: unsupported compose format: %d\n", func, format);
        తిరిగి ఖాళీ;
    }

    అక్షరం *resolved_locale;
    నిర్మాణం xkb_compose_పట్టిక *పట్టిక;
    నిర్మాణం compose_కణుపు dummy = {0};

    resolved_locale = resolve_locale(ctx, locale);
    ఒకవేళ (!resolved_locale)
        తిరిగి ఖాళీ;

    పట్టిక = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(*పట్టిక));
    ఒకవేళ (!పట్టిక) {
        విడుదల(resolved_locale);
        తిరిగి ఖాళీ;
    }

    పట్టిక->refcnt = 1;
    పట్టిక->ctx = xkb_context_ref(ctx);

    పట్టిక->locale = resolved_locale;
    పట్టిక->format = format;
    పట్టిక->flags = flags;

    darray_init(పట్టిక->nodes);
    darray_init(పట్టిక->utf8);

    dummy.keysym = XKB_కీలకం_No_చిహ్నం;
    dummy.ఆకు.is_ఆకు = true;
    dummy.ఆకు.utf8 = 0;
    dummy.ఆకు.keysym = XKB_కీలకం_No_చిహ్నం;
    darray_append(పట్టిక->nodes, dummy);

    darray_append(పట్టిక->utf8, '\0');

    తిరిగి పట్టిక;
}

నిర్మాణం xkb_compose_పట్టిక *
xkb_compose_పట్టిక_ref(నిర్మాణం xkb_compose_పట్టిక *పట్టిక)
{
    ప్రతిజ్ఞా(పట్టిక->refcnt > 0);
    పట్టిక->refcnt++;
    తిరిగి పట్టిక;
}

శూన్య
xkb_compose_పట్టిక_unref(నిర్మాణం xkb_compose_పట్టిక *పట్టిక)
{
    ప్రతిజ్ఞా(!పట్టిక || పట్టిక->refcnt > 0);
    ఒకవేళ (!పట్టిక || --పట్టిక->refcnt > 0)
        తిరిగి;
    విడుదల(పట్టిక->locale);
    darray_free(పట్టిక->nodes);
    darray_free(పట్టిక->utf8);
    xkb_context_unref(పట్టిక->ctx);
    విడుదల(పట్టిక);
}

నిర్మాణం xkb_compose_పట్టిక *
xkb_compose_పట్టిక_new_from_file(నిర్మాణం xkb_context *ctx,
                                FILE *file,
                                స్థిరరాశి అక్షరం *locale,
                                గణన xkb_compose_format format,
                                గణన xkb_compose_compile_flags flags)
{
    నిర్మాణం xkb_compose_పట్టిక * స్థిరరాశి పట్టిక =
        xkb_compose_పట్టిక_new(ctx, __func__, locale, format, flags);
    ఒకవేళ (!పట్టిక)
        తిరిగి ఖాళీ;

    ఒకవేళ (!parse_file(పట్టిక, file, "(unknown file)")) {
        xkb_compose_పట్టిక_unref(పట్టిక);
        తిరిగి ఖాళీ;
    }

    తిరిగి పట్టిక;
}

నిర్మాణం xkb_compose_పట్టిక *
xkb_compose_పట్టిక_new_from_బఫరు(నిర్మాణం xkb_context *ctx,
                                  స్థిరరాశి అక్షరం *బఫరు, కొలత_t నిడివి,
                                  స్థిరరాశి అక్షరం *locale,
                                  గణన xkb_compose_format format,
                                  గణన xkb_compose_compile_flags flags)
{
    నిర్మాణం xkb_compose_పట్టిక * స్థిరరాశి పట్టిక =
        xkb_compose_పట్టిక_new(ctx, __func__, locale, format, flags);
    ఒకవేళ (!పట్టిక)
        తిరిగి ఖాళీ;

    ఒకవేళ (!parse_string(పట్టిక, బఫరు, నిడివి, "(input string)")) {
        xkb_compose_పట్టిక_unref(పట్టిక);
        తిరిగి ఖాళీ;
    }

    తిరిగి పట్టిక;
}

నిర్మాణం xkb_compose_పట్టిక *
xkb_compose_పట్టిక_new_from_locale(నిర్మాణం xkb_context *ctx,
                                  స్థిరరాశి అక్షరం *locale,
                                  గణన xkb_compose_compile_flags flags)
{
    స్థిర స్థిరరాశి గణన xkb_compose_format format = XKB_COMPOSE_FORMAT_పాఠ్యం_V1;
    నిర్మాణం xkb_compose_పట్టిక * స్థిరరాశి పట్టిక =
        xkb_compose_పట్టిక_new(ctx, __func__, locale, format, flags);
    ఒకవేళ (!పట్టిక)
        తిరిగి ఖాళీ;

    అక్షరం *మార్గం = get_xcomposefile_మార్గం(ctx);
    FILE *file = open_file(మార్గం);
    ఒకవేళ (file)
        వెళ్ళు దొరికింది_మార్గం;
    విడుదల(మార్గం);

    మార్గం = get_xdg_xcompose_file_మార్గం(ctx);
    file = open_file(మార్గం);
    ఒకవేళ (file)
        వెళ్ళు దొరికింది_మార్గం;
    విడుదల(మార్గం);

    మార్గం = get_home_xcompose_file_మార్గం(ctx);
    file = open_file(మార్గం);
    ఒకవేళ (file)
        వెళ్ళు దొరికింది_మార్గం;
    విడుదల(మార్గం);

    మార్గం = get_locale_compose_file_మార్గం(ctx, పట్టిక->locale);
    file = open_file(మార్గం);
    ఒకవేళ (file)
        వెళ్ళు దొరికింది_మార్గం;
    విడుదల(మార్గం);

    log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
            "couldn't find a Compose file for locale \"%s\" (mapped to \"%s\")\n",
            locale, పట్టిక->locale);
    xkb_compose_పట్టిక_unref(పట్టిక);
    తిరిగి ఖాళీ;

దొరికింది_మార్గం:
    {} /* Label followed by a declaration is a C23 extension */
    స్థిరరాశి bool ok = parse_file(పట్టిక, file, మార్గం);
    fclose(file);
    ఒకవేళ (!ok) {
        విడుదల(మార్గం);
        xkb_compose_పట్టిక_unref(పట్టిక);
        తిరిగి ఖాళీ;
    }

    log_dbg(ctx, XKB_LOG_సందేశం_NO_ID,
            "created compose table from locale %s with path %s\n",
            పట్టిక->locale, మార్గం);

    విడుదల(మార్గం);
    తిరిగి పట్టిక;
}

స్థిరరాశి xkb_keysym_t *
xkb_compose_పట్టిక_నమోదు_sequence(నిర్మాణం xkb_compose_పట్టిక_నమోదు *నమోదు_2,
                                 కొలత_t *sequence_నిడివి)
{
    *sequence_నిడివి = నమోదు_2->sequence_నిడివి;
    తిరిగి నమోదు_2->sequence;
}

xkb_keysym_t
xkb_compose_పట్టిక_నమోదు_keysym(నిర్మాణం xkb_compose_పట్టిక_నమోదు *నమోదు_2)
{
    తిరిగి నమోదు_2->keysym;
}

స్థిరరాశి అక్షరం *
xkb_compose_పట్టిక_నమోదు_utf8(నిర్మాణం xkb_compose_పట్టిక_నమోదు *నమోదు_2)
{
    తిరిగి నమోదు_2->utf8;
}

#ఒకవేళ MAX_COMPOSE_NODES_LOG2 > 31
#దోషం "Cannot implement bit field xkb_compose_table_iterator_pending_node.offset"
#అయితే_ముగింపు

నిర్మాణం xkb_compose_పట్టిక_iterator_pending_కణుపు {
    uint32_t స్థానభ్రంశం:31;
    bool processed:1;
};

నిర్మాణం xkb_compose_పట్టిక_iterator {
    నిర్మాణం xkb_compose_పట్టిక *పట్టిక;
    /* Current entry */
    నిర్మాణం xkb_compose_పట్టిక_నమోదు నమోదు_2;
    /* Stack of pending nodes to process */
    darray(నిర్మాణం xkb_compose_పట్టిక_iterator_pending_కణుపు) pending_nodes;
};

నిర్మాణం xkb_compose_పట్టిక_iterator *
xkb_compose_పట్టిక_iterator_new(నిర్మాణం xkb_compose_పట్టిక *పట్టిక)
{
    నిర్మాణం xkb_compose_పట్టిక_iterator *iter;
    xkb_keysym_t *sequence;

    iter = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(*iter));
    ఒకవేళ (!iter) {
        తిరిగి ఖాళీ;
    }
    iter->పట్టిక = xkb_compose_పట్టిక_ref(పట్టిక);
    sequence = సున్నా_జ్ఞాపకం_ఇవ్వు(COMPOSE_MAX_LHS_LEN, పరిమాణం(xkb_keysym_t));
    ఒకవేళ (!sequence) {
        విడుదల(iter);
        తిరిగి ఖాళీ;
    }
    iter->నమోదు_2.sequence = sequence;
    iter->నమోదు_2.sequence_నిడివి = 0;

    darray_init(iter->pending_nodes);
    /* Short-circuit if table contains only the dummy entry */
    ఒకవేళ (darray_కొలత(పట్టిక->nodes) == 1) {
        తిరిగి iter;
    }
    /* Add root node */
    నిర్మాణం xkb_compose_పట్టిక_iterator_pending_కణుపు pending = {
        .స్థానభ్రంశం = 1,
        .processed = false
    };
    darray_append(iter->pending_nodes, pending);
    స్థిరరాశి నిర్మాణం compose_కణుపు *కణుపు = &darray_అంశం(iter->పట్టిక->nodes,
                                                   pending.స్థానభ్రంశం);

    /* Find the first left-most node and store intermediate nodes as pending */
    అయితే (కణుపు->lokid) {
        pending.స్థానభ్రంశం = కణుపు->lokid;
        darray_append(iter->pending_nodes, pending);
        కణుపు = &darray_అంశం(iter->పట్టిక->nodes, pending.స్థానభ్రంశం);
    };

    తిరిగి iter;
}

శూన్య
xkb_compose_పట్టిక_iterator_free(నిర్మాణం xkb_compose_పట్టిక_iterator *iter)
{
    xkb_compose_పట్టిక_unref(iter->పట్టిక);
    darray_free(iter->pending_nodes);
    విడుదల(iter->నమోదు_2.sequence);
    విడుదల(iter);
}

నిర్మాణం xkb_compose_పట్టిక_నమోదు *
xkb_compose_పట్టిక_iterator_తదుపరి(నిర్మాణం xkb_compose_పట్టిక_iterator *iter)
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

    నిర్మాణం xkb_compose_పట్టిక_iterator_pending_కణుపు *pending;
    స్థిరరాశి నిర్మాణం compose_కణుపు *కణుపు;

    /* Iterator is empty if there is no pending nodes */
    ఒకవేళ (unlikely(darray_ఖాళీలు(iter->pending_nodes))) {
        తిరిగి ఖాళీ;
    }

    /* Resume to the last element in the stack */
    pending = &darray_అంశం(iter->pending_nodes,
                           darray_కొలత(iter->pending_nodes) - 1);
    కణుపు = &darray_అంశం(iter->పట్టిక->nodes, pending->స్థానభ్రంశం);

    నిర్మాణం xkb_compose_పట్టిక_iterator_pending_కణుపు new = {
        .processed = false,
        .స్థానభ్రంశం = 0
    };

    /* Remove processed leaves until an unprocessed right arrow or parent
     * is found, then update entry accordingly */
    అయితే (pending->processed) {
        /* Remove last keysym */
        iter->నమోదు_2.sequence_నిడివి--;
        ఒకవేళ (కణుపు->hikid) {
            /* Follow right arrow: replace current pending node */
            pending->processed = false;
            pending->స్థానభ్రంశం = కణుపు->hikid;
            /* Process child’s left arrow */
            వెళ్ళు కణుపు_ఎడమ;
        } లేకపోతే {
            /* Remove processed node */
            darray_remove_చివరి(iter->pending_nodes);
            ఒకవేళ (unlikely(darray_ఖాళీలు(iter->pending_nodes))) {
                /* No more nodes to process */
                తిరిగి ఖాళీ;
            }
            /* Get parent node */
            pending = &darray_అంశం(iter->pending_nodes,
                                   darray_కొలత(iter->pending_nodes) - 1);
            కణుపు = &darray_అంశం(iter->పట్టిక->nodes, pending->స్థానభ్రంశం);
        }
    }

    అయితే (1) {
        /* Follow down arrow */
        pending->processed = true;
        iter->నమోదు_2.sequence_నిడివి++;
        iter->నమోదు_2.sequence[iter->నమోదు_2.sequence_నిడివి - 1] = కణుపు->keysym;
        ఒకవేళ (కణుపు->is_ఆకు) {
            /* Leaf: return entry */
            iter->నమోదు_2.keysym = కణుపు->ఆకు.keysym;
            iter->నమోదు_2.utf8 = &darray_అంశం(iter->పట్టిక->utf8, కణుపు->ఆకు.utf8);
            తిరిగి &iter->నమోదు_2;
        }
        /* Not a leaf: process child node */
        new.స్థానభ్రంశం = కణుపు->అంతర్గత.eqkid;
        darray_append(iter->pending_nodes, new);
        pending = &darray_అంశం(iter->pending_nodes,
                               darray_కొలత(iter->pending_nodes) - 1);
కణుపు_ఎడమ:
        కణుపు = &darray_అంశం(iter->పట్టిక->nodes, pending->స్థానభ్రంశం);
        /* Find the next left-most arrow and store intermediate pending nodes */
        అయితే (కణుపు->lokid) {
            /* Follow left arrow */
            new.స్థానభ్రంశం = కణుపు->lokid;
            darray_append(iter->pending_nodes, new);
            pending = &darray_అంశం(iter->pending_nodes,
                                   darray_కొలత(iter->pending_nodes) - 1);
            కణుపు = &darray_అంశం(iter->పట్టిక->nodes, new.స్థానభ్రంశం);
        }
    }
}
