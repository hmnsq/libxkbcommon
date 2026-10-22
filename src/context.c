/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>
#అన్తర్భావ <errno.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <sys/stat.h>

#ఒకవేళ HAVE_XKB_EXTENSIONS_DIRECTORIES
    #అన్తర్భావ <limits.h>
    #ఒకవేళ HAVE_UNISTD_H && HAVE_DIRENT_H
        #అన్తర్భావ <dirent.h>
        #అన్తర్భావ <unistd.h>
    #అయితే_ముగింపు
#అయితే_ముగింపు

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "context.h"
#అన్తర్భావ "darray.h"
#అన్తర్భావ "features/enums.h"
#అన్తర్భావ "messages-codes.h"
#అన్తర్భావ "utils.h"


/**
 * Append one directory to the context’s include path.
 */
స్థిర పూర్ణాంకం
context_include_మార్గం_append(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం *మార్గం)
{
    పూర్ణాంకం err = ENOMEM;

    అక్షరం *tmp = strdup(మార్గం);
    ఒకవేళ (!tmp)
        వెళ్ళు err;

    నిర్మాణం stat stat_buf;
    err = stat(మార్గం, &stat_buf);
    ఒకవేళ (err != 0) {
        err = errno;
        వెళ్ళు err;
    }
    ఒకవేళ (!S_ISDIR(stat_buf.st_బహుళకం)) {
        err = ENOTDIR;
        వెళ్ళు err;
    }

    ఒకవేళ (!check_eaccess(మార్గం, R_OK | X_OK)) {
        err = EACCES;
        వెళ్ళు err;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_సందేశం_NO_ID, "Include path added: %s\n", tmp);

    తిరిగి 1;

err:
    ఒకవేళ (tmp)
        darray_append(ctx->failed_includes, tmp);
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_సందేశం_NO_ID,
             "Include path failed: \"%s\" (%s)\n", మార్గం, strerror(err));
    తిరిగి 0;
}

/**
 * Append one directory to the context’s include path.
 */
పూర్ణాంకం
xkb_context_include_మార్గం_append(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం *మార్గం)
{
    తిరిగి (xkb_context_init_includes(ctx))
        ? context_include_మార్గం_append(ctx, మార్గం)
        : 0;
}

స్థిరరాశి అక్షరం *
xkb_context_include_మార్గం_get_extra_మార్గం(నిర్మాణం xkb_context *ctx)
{
    స్థిరరాశి అక్షరం * స్థిరరాశి extra = xkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    తిరిగి extra ? extra : DFLT_XKB_CONFIG_EXTRA_మార్గం;
}

#ఒకవేళ HAVE_XKB_EXTENSIONS_DIRECTORIES
స్థిరరాశి అక్షరం *
xkb_context_include_మార్గం_get_unversioned_extensions_మార్గం(నిర్మాణం xkb_context *ctx)
{
    స్థిరరాశి అక్షరం *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
    తిరిగి ext
        ? ext
#నిర్వచితమైతే DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_మార్గం
        : DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_మార్గం;
#లేకపోతే
        : ఖాళీ;
#అయితే_ముగింపు
}

స్థిరరాశి అక్షరం *
xkb_context_include_మార్గం_get_versioned_extensions_మార్గం(నిర్మాణం xkb_context *ctx)
{
    స్థిరరాశి అక్షరం *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
    తిరిగి ext
        ? ext
#నిర్వచితమైతే DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_మార్గం
        : DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_మార్గం;
#లేకపోతే
        : ఖాళీ;
#అయితే_ముగింపు
}

స్థిర పూర్ణాంకం
compare_str(స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b)
{
    తిరిగి strcmp(*(అక్షరం **)a, *(అక్షరం **) b);
}

స్థిర పూర్ణాంకం
కూడు_ప్రత్యక్ష_subdirectories(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం *మార్గం,
                          darray_string *extensions,
                          darray_కొలత_t versioned_లెక్క,
                          కొలత_t versioned_మార్గం_నిడివి)
{
    పూర్ణాంకం ret = 0;
    పూర్ణాంకం err = ENOMEM;
    DIR *dir = ఖాళీ;

    /* Check extensions parent directory */
    నిర్మాణం stat stat_buf;
    err = stat(మార్గం, &stat_buf);
    ఒకవేళ (err != 0) {
        err = errno;
        వెళ్ళు err;
    }
    ఒకవేళ (!S_ISDIR(stat_buf.st_బహుళకం)) {
        err = ENOTDIR;
        వెళ్ళు err;
    }
    ఒకవేళ (!check_eaccess(మార్గం, R_OK | X_OK)) {
        err = EACCES;
        వెళ్ళు err;
    }

    dir = opendir(మార్గం);
    ఒకవేళ (dir == ఖాళీ) {
        err = EACCES;
        వెళ్ళు err;
    }

    నిర్మాణం dirent *నమోదు_2;
    అక్షరం మార్గం_buf[PATH_MAX] = "";
    versioned_మార్గం_నిడివి++; /* Additional final ‘/’ */
    అయితే ((నమోదు_2 = readdir(dir)) != ఖాళీ) {
        స్థిరరాశి అక్షరం *name = నమోదు_2->d_name;

        /* Skip special entries */
        ఒకవేళ (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            కొనసాగు;

        ఒకవేళ (!snprintf_సురక్షిత(మార్గం_buf, పరిమాణం(మార్గం_buf), "%s/%s", మార్గం, name)) {
            err = ENOMEM;
            వెళ్ళు err;
        }
        ఒకవేళ (stat(మార్గం_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_బహుళకం))
            కొనసాగు;

        /* Skip if the corresponding versioned directory is already included */
        కొరకు (darray_కొలత_t i = 0; i < versioned_లెక్క; i++) {
            స్థిరరాశి అక్షరం * స్థిరరాశి prev_name = darray_అంశం(*extensions, i)
                                         + versioned_మార్గం_నిడివి;
            ఒకవేళ (strcmp(name, prev_name) == 0)
                వెళ్ళు తదుపరి;
        }

        అక్షరం *ext_మార్గం = strdup_సురక్షిత(మార్గం_buf);
        ఒకవేళ (!ext_మార్గం) {
            err = ENOMEM;
            వెళ్ళు err;
        }

        darray_append(*extensions, ext_మార్గం);
తదుపరి:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    ఒకవేళ (darray_కొలత(*extensions) > versioned_లెక్క) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_వస్తువులు(*extensions) + versioned_లెక్క,
              darray_కొలత(*extensions) - versioned_లెక్క,
              పరిమాణం(*darray_వస్తువులు(*extensions)), &compare_str);
        అక్షరం **ext_మార్గం;
        darray_foreach_from(ext_మార్గం, *extensions, versioned_లెక్క) {
            ret |= context_include_మార్గం_append(ctx, *ext_మార్గం);
        }
    }

    తిరిగి ret;

err:
    log_dbg(ctx, XKB_LOG_సందేశం_NO_ID,
            "Include extensions path failed: %s (%s)\n", మార్గం, strerror(err));
    ఒకవేళ (dir)
        closedir(dir);

    తిరిగి ret;
}
#అయితే_ముగింపు

స్థిరరాశి అక్షరం *
xkb_context_include_మార్గం_get_system_మార్గం(నిర్మాణం xkb_context *ctx)
{
    స్థిరరాశి అక్షరం * స్థిరరాశి మూలం = xkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    తిరిగి మూలం ? మూలం : DFLT_XKB_CONFIG_మూలం;
}

/**
 * Append the default include directories to the context.
 */
పూర్ణాంకం
xkb_context_include_మార్గం_append_default(నిర్మాణం xkb_context *ctx)
{
    /*
     * We do not call `xkb_context_init_includes()` here, because either
     * we already initialized the includes paths or we are doing it now.
     */

    అక్షరం *user_మార్గం;
    పూర్ణాంకం ret = 0;

    స్థిరరాశి అక్షరం * స్థిరరాశి home = xkb_context_getenv(ctx, "HOME");
    స్థిరరాశి అక్షరం * స్థిరరాశి xdg = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    ఒకవేళ (xdg != ఖాళీ) {
        user_మార్గం = asprintf_సురక్షిత("%s/xkb", xdg);
        ఒకవేళ (user_మార్గం) {
            ret |= context_include_మార్గం_append(ctx, user_మార్గం);
            విడుదల(user_మార్గం);
        }
    } లేకపోతే ఒకవేళ (home != ఖాళీ) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        user_మార్గం = asprintf_సురక్షిత("%s/.config/xkb", home);
        ఒకవేళ (user_మార్గం) {
            ret |= context_include_మార్గం_append(ctx, user_మార్గం);
            విడుదల(user_మార్గం);
        }
    }

    ఒకవేళ (home != ఖాళీ) {
        user_మార్గం = asprintf_సురక్షిత("%s/.xkb", home);
        ఒకవేళ (user_మార్గం) {
            ret |= context_include_మార్గం_append(ctx, user_మార్గం);
            విడుదల(user_మార్గం);
        }
    }

    స్థిరరాశి అక్షరం * స్థిరరాశి extra = xkb_context_include_మార్గం_get_extra_మార్గం(ctx);
    ret |= context_include_మార్గం_append(ctx, extra);

#ఒకవేళ HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    స్థిరరాశి అక్షరం *extensions_మార్గం =
        xkb_context_include_మార్గం_get_versioned_extensions_మార్గం(ctx);
    కొలత_t versioned_మార్గం_నిడివి = 0;
    ఒకవేళ (extensions_మార్గం) {
        /* Add direct subdirectories */
        ret |= కూడు_ప్రత్యక్ష_subdirectories(ctx, extensions_మార్గం,
                                         &extensions, 0, 0);
        versioned_మార్గం_నిడివి = అక్షరశ్రేణి_పొడవు(extensions_మార్గం);
    }

    /* Unversioned extensions directory */
    extensions_మార్గం =
        xkb_context_include_మార్గం_get_unversioned_extensions_మార్గం(ctx);
    ఒకవేళ (extensions_మార్గం) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= కూడు_ప్రత్యక్ష_subdirectories(ctx, extensions_మార్గం, &extensions,
                                         darray_కొలత(extensions),
                                         versioned_మార్గం_నిడివి);
    }

    అక్షరం **ext_మార్గం;
    darray_foreach(ext_మార్గం, extensions) {
        విడుదల(*ext_మార్గం);
    }
    darray_free(extensions);
#అయితే_ముగింపు

    /* Canonical XKB root */
    స్థిరరాశి అక్షరం * స్థిరరాశి మూలం = xkb_context_include_మార్గం_get_system_మార్గం(ctx);
    స్థిరరాశి bool has_మూలం = context_include_మార్గం_append(ctx, మూలం);
    ret |= has_మూలం;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    ఒకవేళ (!has_మూలం && మూలం[0] != '\0') {
        log_warn(ctx, XKB_LOG_సందేశం_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_మూలం, మూలం);
        ret |= context_include_మార్గం_append(ctx, DFLT_XKB_LEGACY_మూలం);
    }

    తిరిగి ret;
}

/**
 * Remove all entries in the context's include path.
 */
శూన్య
xkb_context_include_మార్గం_clear(నిర్మాణం xkb_context *ctx)
{
    అక్షరం **మార్గం;

    darray_foreach(మార్గం, ctx->includes)
        విడుదల(*మార్గం);
    darray_free(ctx->includes);

    darray_foreach(మార్గం, ctx->failed_includes)
        విడుదల(*మార్గం);
    darray_free(ctx->failed_includes);

    /* It does not make sense to keep the pending defaults */
    ctx->pending_default_includes = false;
}

/**
 * xkb_context_include_path_clear() + xkb_context_include_path_append_default()
 */
పూర్ణాంకం
xkb_context_include_మార్గం_reset_defaults(నిర్మాణం xkb_context *ctx)
{
    xkb_context_include_మార్గం_clear(ctx);
    తిరిగి xkb_context_include_మార్గం_append_default(ctx);
}

/**
 * Returns the number of entries in the context's include path.
 */
గుర్తులేని పూర్ణాంకం
xkb_context_అంకె_include_paths(నిర్మాణం xkb_context *ctx)
{
    తిరిగి (xkb_context_init_includes(ctx))
        ? darray_కొలత(ctx->includes)
        : 0;
}

/**
 * Returns the given entry in the context's include path, or NULL if an
 * invalid index is passed.
 */
స్థిరరాశి అక్షరం *
xkb_context_include_మార్గం_get(నిర్మాణం xkb_context *ctx, గుర్తులేని పూర్ణాంకం idx)
{
    ఒకవేళ (idx >= xkb_context_అంకె_include_paths(ctx))
        తిరిగి ఖాళీ;

    తిరిగి darray_అంశం(ctx->includes, idx);
}

/**
 * Take a new reference on the context.
 */
నిర్మాణం xkb_context *
xkb_context_ref(నిర్మాణం xkb_context *ctx)
{
    ప్రతిజ్ఞా(ctx->refcnt > 0);
    ctx->refcnt++;
    తిరిగి ctx;
}

/**
 * Drop an existing reference on the context, and free it if the refcnt is
 * now 0.
 */
శూన్య
xkb_context_unref(నిర్మాణం xkb_context *ctx)
{
    ప్రతిజ్ఞా(!ctx || ctx->refcnt > 0);
    ఒకవేళ (!ctx || --ctx->refcnt > 0)
        తిరిగి;

    విడుదల(ctx->x11_atom_cache);
    xkb_context_include_మార్గం_clear(ctx);
    atom_పట్టిక_free(ctx->atom_పట్టిక);
    విడుదల(ctx);
}

స్థిర స్థిరరాశి అక్షరం *
log_level_గమ్యం_ఉపసర్గ(గణన xkb_log_level level)
{
    ఎంపిక (level) {
    స్థితి XKB_LOG_LEVEL_DEBUG:
        తిరిగి "xkbcommon: DEBUG: ";
    స్థితి XKB_LOG_LEVEL_INFO:
        తిరిగి "xkbcommon: INFO: ";
    స్థితి XKB_LOG_LEVEL_WARNING:
        తిరిగి "xkbcommon: WARNING: ";
    స్థితి XKB_LOG_LEVEL_ERROR:
        తిరిగి "xkbcommon: ERROR: ";
    స్థితి XKB_LOG_LEVEL_CRITICAL:
        తిరిగి "xkbcommon: CRITICAL: ";
    అప్రమేయ:
        తిరిగి ఖాళీ;
    }
}

ATTR_PRINTF(3, 0) స్థిర శూన్య
default_log_fn(నిర్మాణం xkb_context *ctx, గణన xkb_log_level level,
               స్థిరరాశి అక్షరం *fmt, va_list వాదనలు)
{
    స్థిరరాశి అక్షరం *ఉపసర్గ = log_level_గమ్యం_ఉపసర్గ(level);

    ఒకవేళ (ఉపసర్గ)
        fprintf(stderr, "%s", ఉపసర్గ);
    vfprintf(stderr, fmt, వాదనలు);
}

స్థిర గణన xkb_log_level
log_level(స్థిరరాశి అక్షరం *level) {
    errno = 0;
    అక్షరం *endptr = ఖాళీ;
    స్థిరరాశి దీర్ఘ lvl = strtol(level, &endptr, 10);
    ఒకవేళ (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        తిరిగి (గణన xkb_log_level)lvl;
    ఒకవేళ (istreq_ఉపసర్గ("crit", level))
        తిరిగి XKB_LOG_LEVEL_CRITICAL;
    ఒకవేళ (istreq_ఉపసర్గ("err", level))
        తిరిగి XKB_LOG_LEVEL_ERROR;
    ఒకవేళ (istreq_ఉపసర్గ("warn", level))
        తిరిగి XKB_LOG_LEVEL_WARNING;
    ఒకవేళ (istreq_ఉపసర్గ("info", level))
        తిరిగి XKB_LOG_LEVEL_INFO;
    ఒకవేళ (istreq_ఉపసర్గ("debug", level) || istreq_ఉపసర్గ("dbg", level))
        తిరిగి XKB_LOG_LEVEL_DEBUG;

    తిరిగి XKB_LOG_LEVEL_ERROR;
}

స్థిర పూర్ణాంకం
log_verbosity(స్థిరరాశి అక్షరం *verbosity) {
    errno = 0;
    స్థిరరాశి దీర్ఘ పూర్ణాంకం v = strtol(verbosity, ఖాళీ, 10);
    ఒకవేళ (errno == 0) {
        /* Truncate */
        తిరిగి (పూర్ణాంకం) v;
    }

    తిరిగి XKB_LOG_VERBOSITY_DEFAULT;
}

/**
 * Create a new context.
 */
నిర్మాణం xkb_context *
xkb_context_new(గణన xkb_context_flags flags)
{
    స్థిరరాశి అక్షరం *env;
    నిర్మాణం xkb_context *ctx = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(*ctx));

    ఒకవేళ (!ctx)
        తిరిగి ఖాళీ;

    ctx->refcnt = 1;
    ctx->log_fn = default_log_fn;
    ctx->log_level = XKB_LOG_LEVEL_ERROR;
    ctx->log_verbosity = XKB_LOG_VERBOSITY_DEFAULT;

    స్థిరరాశి గణన xkb_context_flags invalid_flags =
        (flags & ~(గణన xkb_context_flags)XKB_CONTEXT_FLAGS_విలువలు);
    ఒకవేళ (invalid_flags) {
        log_err_func(ctx, XKB_LOG_సందేశం_NO_ID,
                     "Invalid context flags: 0x%x\n", invalid_flags);
        విడుదల(ctx);
        తిరిగి ఖాళీ;
    }

    ctx->use_environment_పేర్లు = !(flags & XKB_CONTEXT_NO_ENVIRONMENT_పేర్లు);
    ctx->use_secure_getenv = !(flags & XKB_CONTEXT_NO_SECURE_GETENV);

    /*
     * Default includes paths are delayed and added only if necessary.
     *
     * It is more efficient for most clients, which only get the keymap from the
     * server: it avoids unnecessary allocations and file system queries.
     *
     * It also avoid the corner case where a containerized app lacks access to
     * the XKB directories.
     *
     * There might be an issue in case the environment variables relevant to the
     * include paths change between the context initialization and the call to
     * `xkb_context_include_path_append_default()`. However this seems very
     * unlikely and would have already triggered issues when the `%`-expansion
     * is used.
     */
    ctx->pending_default_includes = !(flags & XKB_CONTEXT_NO_DEFAULT_INCLUDES);
    darray_init(ctx->includes);
    darray_init(ctx->failed_includes);

    /* Environment overwrites defaults. */
    env = xkb_context_getenv(ctx, "XKB_LOG_LEVEL");
    ఒకవేళ (env)
        xkb_context_సమితి_log_level(ctx, log_level(env));

    env = xkb_context_getenv(ctx, "XKB_LOG_VERBOSITY");
    ఒకవేళ (env)
        xkb_context_సమితి_log_verbosity(ctx, log_verbosity(env));

    ctx->atom_పట్టిక = atom_పట్టిక_new();
    ఒకవేళ (!ctx->atom_పట్టిక) {
        xkb_context_unref(ctx);
        తిరిగి ఖాళీ;
    }

    ctx->x11_atom_cache = ఖాళీ;

    తిరిగి ctx;
}

శూన్య
xkb_context_సమితి_log_fn(నిర్మాణం xkb_context *ctx,
                       శూన్య (*log_fn)(నిర్మాణం xkb_context *ctx,
                                      గణన xkb_log_level level,
                                      స్థిరరాశి అక్షరం *fmt, va_list వాదనలు))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

గణన xkb_log_level
xkb_context_get_log_level(నిర్మాణం xkb_context *ctx)
{
    తిరిగి ctx->log_level;
}

శూన్య
xkb_context_సమితి_log_level(నిర్మాణం xkb_context *ctx, గణన xkb_log_level level)
{
    ctx->log_level = level;
}

పూర్ణాంకం
xkb_context_get_log_verbosity(నిర్మాణం xkb_context *ctx)
{
    తిరిగి ctx->log_verbosity;
}

శూన్య
xkb_context_సమితి_log_verbosity(నిర్మాణం xkb_context *ctx, పూర్ణాంకం verbosity)
{
    ctx->log_verbosity = verbosity;
}

శూన్య *
xkb_context_get_user_దత్తాంశం(నిర్మాణం xkb_context *ctx)
{
    ఒకవేళ (ctx)
        తిరిగి ctx->user_దత్తాంశం;
    తిరిగి ఖాళీ;
}

శూన్య
xkb_context_సమితి_user_దత్తాంశం(నిర్మాణం xkb_context *ctx, శూన్య *user_దత్తాంశం)
{
    ctx->user_దత్తాంశం = user_దత్తాంశం;
}
