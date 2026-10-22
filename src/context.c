/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <sys/stat.h>

#જો HAVE_XKB_EXTENSIONS_DIRECTORIES
    #અન્તર્ભાવ <limits.h>
    #જો HAVE_UNISTD_H && HAVE_DIRENT_H
        #અન્તર્ભાવ <dirent.h>
        #અન્તર્ભાવ <unistd.h>
    #endif
#endif

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "context.h"
#અન્તર્ભાવ "darray.h"
#અન્તર્ભાવ "features/enums.h"
#અન્તર્ભાવ "messages-codes.h"
#અન્તર્ભાવ "utils.h"


/**
 * Append one directory to the context’s include path.
 */
સ્થિર પૂર્ણાંક
context_include_path_append(સંરચના xkb_context *ctx, અચલ અક્ષર *path)
{
    પૂર્ણાંક err = ENOMEM;

    અક્ષર *tmp = strdup(path);
    જો (!tmp)
        જાઓ err;

    સંરચના stat stat_buf;
    err = stat(path, &stat_buf);
    જો (err != 0) {
        err = errno;
        જાઓ err;
    }
    જો (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        જાઓ err;
    }

    જો (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        જાઓ err;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    પાછા 1;

err:
    જો (tmp)
        darray_append(ctx->failed_includes, tmp);
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    પાછા 0;
}

/**
 * Append one directory to the context’s include path.
 */
પૂર્ણાંક
xkb_context_include_path_append(સંરચના xkb_context *ctx, અચલ અક્ષર *path)
{
    પાછા (xkb_context_init_includes(ctx))
        ? context_include_path_append(ctx, path)
        : 0;
}

અચલ અક્ષર *
xkb_context_include_path_get_extra_path(સંરચના xkb_context *ctx)
{
    અચલ અક્ષર * અચલ extra = xkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    પાછા extra ? extra : DFLT_XKB_CONFIG_EXTRA_PATH;
}

#જો HAVE_XKB_EXTENSIONS_DIRECTORIES
અચલ અક્ષર *
xkb_context_include_path_get_unversioned_extensions_path(સંરચના xkb_context *ctx)
{
    અચલ અક્ષર *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
    પાછા ext
        ? ext
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#નહીંતર
        : ખાલી;
#endif
}

અચલ અક્ષર *
xkb_context_include_path_get_versioned_extensions_path(સંરચના xkb_context *ctx)
{
    અચલ અક્ષર *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
    પાછા ext
        ? ext
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#નહીંતર
        : ખાલી;
#endif
}

સ્થિર પૂર્ણાંક
compare_str(અચલ શૂન્ય *a, અચલ શૂન્ય *b)
{
    પાછા strcmp(*(અક્ષર **)a, *(અક્ષર **) b);
}

સ્થિર પૂર્ણાંક
add_direct_subdirectories(સંરચના xkb_context *ctx, અચલ અક્ષર *path,
                          darray_string *extensions,
                          darray_size_t versioned_ગણતરી,
                          size_t versioned_path_length)
{
    પૂર્ણાંક ret = 0;
    પૂર્ણાંક err = ENOMEM;
    DIR *dir = ખાલી;

    /* Check extensions parent directory */
    સંરચના stat stat_buf;
    err = stat(path, &stat_buf);
    જો (err != 0) {
        err = errno;
        જાઓ err;
    }
    જો (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        જાઓ err;
    }
    જો (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        જાઓ err;
    }

    dir = opendir(path);
    જો (dir == ખાલી) {
        err = EACCES;
        જાઓ err;
    }

    સંરચના dirent *entry;
    અક્ષર path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    જ્યારે ((entry = readdir(dir)) != ખાલી) {
        અચલ અક્ષર *name = entry->d_name;

        /* Skip special entries */
        જો (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            ચાલુ;

        જો (!snprintf_safe(path_buf, કદ(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            જાઓ err;
        }
        જો (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            ચાલુ;

        /* Skip if the corresponding versioned directory is already included */
        માટે (darray_size_t i = 0; i < versioned_ગણતરી; i++) {
            અચલ અક્ષર * અચલ prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            જો (strcmp(name, prev_name) == 0)
                જાઓ next;
        }

        અક્ષર *ext_path = strdup_safe(path_buf);
        જો (!ext_path) {
            err = ENOMEM;
            જાઓ err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    જો (darray_size(*extensions) > versioned_ગણતરી) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_વસ્તુઓ(*extensions) + versioned_ગણતરી,
              darray_size(*extensions) - versioned_ગણતરી,
              કદ(*darray_વસ્તુઓ(*extensions)), &compare_str);
        અક્ષર **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_ગણતરી) {
            ret |= context_include_path_append(ctx, *ext_path);
        }
    }

    પાછા ret;

err:
    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    જો (dir)
        closedir(dir);

    પાછા ret;
}
#endif

અચલ અક્ષર *
xkb_context_include_path_get_system_path(સંરચના xkb_context *ctx)
{
    અચલ અક્ષર * અચલ મૂળ = xkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    પાછા મૂળ ? મૂળ : DFLT_XKB_CONFIG_મૂળ;
}

/**
 * Append the default include directories to the context.
 */
પૂર્ણાંક
xkb_context_include_path_append_default(સંરચના xkb_context *ctx)
{
    /*
     * We do not call `xkb_context_init_includes()` here, because either
     * we already initialized the includes paths or we are doing it now.
     */

    અક્ષર *user_path;
    પૂર્ણાંક ret = 0;

    અચલ અક્ષર * અચલ home = xkb_context_getenv(ctx, "HOME");
    અચલ અક્ષર * અચલ xdg = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    જો (xdg != ખાલી) {
        user_path = asprintf_safe("%s/xkb", xdg);
        જો (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            મુક્ત(user_path);
        }
    } નહીંતર જો (home != ખાલી) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        user_path = asprintf_safe("%s/.config/xkb", home);
        જો (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            મુક્ત(user_path);
        }
    }

    જો (home != ખાલી) {
        user_path = asprintf_safe("%s/.xkb", home);
        જો (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            મુક્ત(user_path);
        }
    }

    અચલ અક્ષર * અચલ extra = xkb_context_include_path_get_extra_path(ctx);
    ret |= context_include_path_append(ctx, extra);

#જો HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    અચલ અક્ષર *extensions_path =
        xkb_context_include_path_get_versioned_extensions_path(ctx);
    size_t versioned_path_length = 0;
    જો (extensions_path) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = strlen(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        xkb_context_include_path_get_unversioned_extensions_path(ctx);
    જો (extensions_path) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    અક્ષર **ext_path;
    darray_foreach(ext_path, extensions) {
        મુક્ત(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    અચલ અક્ષર * અચલ મૂળ = xkb_context_include_path_get_system_path(ctx);
    અચલ bool has_મૂળ = context_include_path_append(ctx, મૂળ);
    ret |= has_મૂળ;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    જો (!has_મૂળ && મૂળ[0] != '\0') {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_મૂળ, મૂળ);
        ret |= context_include_path_append(ctx, DFLT_XKB_LEGACY_મૂળ);
    }

    પાછા ret;
}

/**
 * Remove all entries in the context's include path.
 */
શૂન્ય
xkb_context_include_path_clear(સંરચના xkb_context *ctx)
{
    અક્ષર **path;

    darray_foreach(path, ctx->includes)
        મુક્ત(*path);
    darray_free(ctx->includes);

    darray_foreach(path, ctx->failed_includes)
        મુક્ત(*path);
    darray_free(ctx->failed_includes);

    /* It does not make sense to keep the pending defaults */
    ctx->pending_default_includes = false;
}

/**
 * xkb_context_include_path_clear() + xkb_context_include_path_append_default()
 */
પૂર્ણાંક
xkb_context_include_path_reset_defaults(સંરચના xkb_context *ctx)
{
    xkb_context_include_path_clear(ctx);
    પાછા xkb_context_include_path_append_default(ctx);
}

/**
 * Returns the number of entries in the context's include path.
 */
unsigned પૂર્ણાંક
xkb_context_અંક_include_paths(સંરચના xkb_context *ctx)
{
    પાછા (xkb_context_init_includes(ctx))
        ? darray_size(ctx->includes)
        : 0;
}

/**
 * Returns the given entry in the context's include path, or NULL if an
 * invalid index is passed.
 */
અચલ અક્ષર *
xkb_context_include_path_get(સંરચના xkb_context *ctx, unsigned પૂર્ણાંક idx)
{
    જો (idx >= xkb_context_અંક_include_paths(ctx))
        પાછા ખાલી;

    પાછા darray_item(ctx->includes, idx);
}

/**
 * Take a new reference on the context.
 */
સંરચના xkb_context *
xkb_context_ref(સંરચના xkb_context *ctx)
{
    પ્રતિજ્ઞા(ctx->refcnt > 0);
    ctx->refcnt++;
    પાછા ctx;
}

/**
 * Drop an existing reference on the context, and free it if the refcnt is
 * now 0.
 */
શૂન્ય
xkb_context_unref(સંરચના xkb_context *ctx)
{
    પ્રતિજ્ઞા(!ctx || ctx->refcnt > 0);
    જો (!ctx || --ctx->refcnt > 0)
        પાછા;

    મુક્ત(ctx->x11_atom_cache);
    xkb_context_include_path_clear(ctx);
    atom_table_free(ctx->atom_table);
    મુક્ત(ctx);
}

સ્થિર અચલ અક્ષર *
log_level_to_prefix(ગણના xkb_log_level level)
{
    પસંદગી (level) {
    સ્થિતિ XKB_LOG_LEVEL_DEBUG:
        પાછા "xkbcommon: DEBUG: ";
    સ્થિતિ XKB_LOG_LEVEL_INFO:
        પાછા "xkbcommon: INFO: ";
    સ્થિતિ XKB_LOG_LEVEL_WARNING:
        પાછા "xkbcommon: WARNING: ";
    સ્થિતિ XKB_LOG_LEVEL_ERROR:
        પાછા "xkbcommon: ERROR: ";
    સ્થિતિ XKB_LOG_LEVEL_CRITICAL:
        પાછા "xkbcommon: CRITICAL: ";
    મૂળભૂત:
        પાછા ખાલી;
    }
}

ATTR_PRINTF(3, 0) સ્થિર શૂન્ય
default_log_fn(સંરચના xkb_context *ctx, ગણના xkb_log_level level,
               અચલ અક્ષર *fmt, va_list args)
{
    અચલ અક્ષર *prefix = log_level_to_prefix(level);

    જો (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, args);
}

સ્થિર ગણના xkb_log_level
log_level(અચલ અક્ષર *level) {
    errno = 0;
    અક્ષર *endptr = ખાલી;
    અચલ long lvl = strtol(level, &endptr, 10);
    જો (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        પાછા (ગણના xkb_log_level)lvl;
    જો (istreq_prefix("crit", level))
        પાછા XKB_LOG_LEVEL_CRITICAL;
    જો (istreq_prefix("err", level))
        પાછા XKB_LOG_LEVEL_ERROR;
    જો (istreq_prefix("warn", level))
        પાછા XKB_LOG_LEVEL_WARNING;
    જો (istreq_prefix("info", level))
        પાછા XKB_LOG_LEVEL_INFO;
    જો (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        પાછા XKB_LOG_LEVEL_DEBUG;

    પાછા XKB_LOG_LEVEL_ERROR;
}

સ્થિર પૂર્ણાંક
log_verbosity(અચલ અક્ષર *verbosity) {
    errno = 0;
    અચલ long પૂર્ણાંક v = strtol(verbosity, ખાલી, 10);
    જો (errno == 0) {
        /* Truncate */
        પાછા (પૂર્ણાંક) v;
    }

    પાછા XKB_LOG_VERBOSITY_DEFAULT;
}

/**
 * Create a new context.
 */
સંરચના xkb_context *
xkb_context_new(ગણના xkb_context_flags flags)
{
    અચલ અક્ષર *env;
    સંરચના xkb_context *ctx = calloc(1, કદ(*ctx));

    જો (!ctx)
        પાછા ખાલી;

    ctx->refcnt = 1;
    ctx->log_fn = default_log_fn;
    ctx->log_level = XKB_LOG_LEVEL_ERROR;
    ctx->log_verbosity = XKB_LOG_VERBOSITY_DEFAULT;

    અચલ ગણના xkb_context_flags invalid_flags =
        (flags & ~(ગણના xkb_context_flags)XKB_CONTEXT_FLAGS_કિંમતો);
    જો (invalid_flags) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Invalid context flags: 0x%x\n", invalid_flags);
        મુક્ત(ctx);
        પાછા ખાલી;
    }

    ctx->use_environment_names = !(flags & XKB_CONTEXT_NO_ENVIRONMENT_NAMES);
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
    જો (env)
        xkb_context_set_log_level(ctx, log_level(env));

    env = xkb_context_getenv(ctx, "XKB_LOG_VERBOSITY");
    જો (env)
        xkb_context_set_log_verbosity(ctx, log_verbosity(env));

    ctx->atom_table = atom_table_new();
    જો (!ctx->atom_table) {
        xkb_context_unref(ctx);
        પાછા ખાલી;
    }

    ctx->x11_atom_cache = ખાલી;

    પાછા ctx;
}

શૂન્ય
xkb_context_set_log_fn(સંરચના xkb_context *ctx,
                       શૂન્ય (*log_fn)(સંરચના xkb_context *ctx,
                                      ગણના xkb_log_level level,
                                      અચલ અક્ષર *fmt, va_list args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

ગણના xkb_log_level
xkb_context_get_log_level(સંરચના xkb_context *ctx)
{
    પાછા ctx->log_level;
}

શૂન્ય
xkb_context_set_log_level(સંરચના xkb_context *ctx, ગણના xkb_log_level level)
{
    ctx->log_level = level;
}

પૂર્ણાંક
xkb_context_get_log_verbosity(સંરચના xkb_context *ctx)
{
    પાછા ctx->log_verbosity;
}

શૂન્ય
xkb_context_set_log_verbosity(સંરચના xkb_context *ctx, પૂર્ણાંક verbosity)
{
    ctx->log_verbosity = verbosity;
}

શૂન્ય *
xkb_context_get_user_data(સંરચના xkb_context *ctx)
{
    જો (ctx)
        પાછા ctx->user_data;
    પાછા ખાલી;
}

શૂન્ય
xkb_context_set_user_data(સંરચના xkb_context *ctx, શૂન્ય *user_data)
{
    ctx->user_data = user_data;
}
