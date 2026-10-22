/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <errno.h>
#ضمن <string.h>
#ضمن <sys/types.h>
#ضمن <sys/stat.h>

#إذا HAVE_XKB_EXTENSIONS_DIRECTORIES
    #ضمن <limits.h>
    #إذا HAVE_UNISTD_H && HAVE_DIRENT_H
        #ضمن <dirent.h>
        #ضمن <unistd.h>
    #endif
#endif

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "context.h"
#ضمن "darray.h"
#ضمن "features/enums.h"
#ضمن "messages-codes.h"
#ضمن "utils.h"


/**
 * Append one directory to the context’s include path.
 */
ساكن صحيح
context_include_path_append(بنية xkb_context *ctx, ثابت حرف *path)
{
    صحيح err = ENOMEM;

    حرف *tmp = strdup(path);
    إذا (!tmp)
        اذهب err;

    بنية stat stat_buf;
    err = stat(path, &stat_buf);
    إذا (err != 0) {
        err = errno;
        اذهب err;
    }
    إذا (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        اذهب err;
    }

    إذا (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        اذهب err;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    أرجع 1;

err:
    إذا (tmp)
        darray_append(ctx->failed_includes, tmp);
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    أرجع 0;
}

/**
 * Append one directory to the context’s include path.
 */
صحيح
xkb_context_include_path_append(بنية xkb_context *ctx, ثابت حرف *path)
{
    أرجع (xkb_context_init_includes(ctx))
        ? context_include_path_append(ctx, path)
        : 0;
}

ثابت حرف *
xkb_context_include_path_get_extra_path(بنية xkb_context *ctx)
{
    ثابت حرف * ثابت extra = xkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    أرجع extra ? extra : DFLT_XKB_CONFIG_EXTRA_PATH;
}

#إذا HAVE_XKB_EXTENSIONS_DIRECTORIES
ثابت حرف *
xkb_context_include_path_get_unversioned_extensions_path(بنية xkb_context *ctx)
{
    ثابت حرف *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
    أرجع ext
        ? ext
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#وإلا
        : عدم;
#endif
}

ثابت حرف *
xkb_context_include_path_get_versioned_extensions_path(بنية xkb_context *ctx)
{
    ثابت حرف *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
    أرجع ext
        ? ext
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#وإلا
        : عدم;
#endif
}

ساكن صحيح
compare_str(ثابت فراغ *a, ثابت فراغ *b)
{
    أرجع strcmp(*(حرف **)a, *(حرف **) b);
}

ساكن صحيح
add_direct_subdirectories(بنية xkb_context *ctx, ثابت حرف *path,
                          darray_string *extensions,
                          darray_size_t versioned_كدي,
                          size_t versioned_path_length)
{
    صحيح ret = 0;
    صحيح err = ENOMEM;
    DIR *dir = عدم;

    /* Check extensions parent directory */
    بنية stat stat_buf;
    err = stat(path, &stat_buf);
    إذا (err != 0) {
        err = errno;
        اذهب err;
    }
    إذا (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        اذهب err;
    }
    إذا (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        اذهب err;
    }

    dir = opendir(path);
    إذا (dir == عدم) {
        err = EACCES;
        اذهب err;
    }

    بنية dirent *entry;
    حرف path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    طالما ((entry = readdir(dir)) != عدم) {
        ثابت حرف *name = entry->d_name;

        /* Skip special entries */
        إذا (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            استمر;

        إذا (!snprintf_safe(path_buf, حجم(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            اذهب err;
        }
        إذا (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            استمر;

        /* Skip if the corresponding versioned directory is already included */
        لكل (darray_size_t i = 0; i < versioned_كدي; i++) {
            ثابت حرف * ثابت prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            إذا (strcmp(name, prev_name) == 0)
                اذهب next;
        }

        حرف *ext_path = strdup_safe(path_buf);
        إذا (!ext_path) {
            err = ENOMEM;
            اذهب err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    إذا (darray_size(*extensions) > versioned_كدي) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_ابوبو(*extensions) + versioned_كدي,
              darray_size(*extensions) - versioned_كدي,
              حجم(*darray_ابوبو(*extensions)), &compare_str);
        حرف **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_كدي) {
            ret |= context_include_path_append(ctx, *ext_path);
        }
    }

    أرجع ret;

err:
    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    إذا (dir)
        closedir(dir);

    أرجع ret;
}
#endif

ثابت حرف *
xkb_context_include_path_get_system_path(بنية xkb_context *ctx)
{
    ثابت حرف * ثابت سيو = xkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    أرجع سيو ? سيو : DFLT_XKB_CONFIG_سيو;
}

/**
 * Append the default include directories to the context.
 */
صحيح
xkb_context_include_path_append_default(بنية xkb_context *ctx)
{
    /*
     * We do not call `xkb_context_init_includes()` here, because either
     * we already initialized the includes paths or we are doing it now.
     */

    حرف *user_path;
    صحيح ret = 0;

    ثابت حرف * ثابت home = xkb_context_getenv(ctx, "HOME");
    ثابت حرف * ثابت xdg = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    إذا (xdg != عدم) {
        user_path = asprintf_safe("%s/xkb", xdg);
        إذا (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            حرر(user_path);
        }
    } وإلا إذا (home != عدم) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        user_path = asprintf_safe("%s/.config/xkb", home);
        إذا (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            حرر(user_path);
        }
    }

    إذا (home != عدم) {
        user_path = asprintf_safe("%s/.xkb", home);
        إذا (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            حرر(user_path);
        }
    }

    ثابت حرف * ثابت extra = xkb_context_include_path_get_extra_path(ctx);
    ret |= context_include_path_append(ctx, extra);

#إذا HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ثابت حرف *extensions_path =
        xkb_context_include_path_get_versioned_extensions_path(ctx);
    size_t versioned_path_length = 0;
    إذا (extensions_path) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = strlen(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        xkb_context_include_path_get_unversioned_extensions_path(ctx);
    إذا (extensions_path) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    حرف **ext_path;
    darray_foreach(ext_path, extensions) {
        حرر(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    ثابت حرف * ثابت سيو = xkb_context_include_path_get_system_path(ctx);
    ثابت bool has_سيو = context_include_path_append(ctx, سيو);
    ret |= has_سيو;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    إذا (!has_سيو && سيو[0] != '\0') {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_سيو, سيو);
        ret |= context_include_path_append(ctx, DFLT_XKB_LEGACY_سيو);
    }

    أرجع ret;
}

/**
 * Remove all entries in the context's include path.
 */
فراغ
xkb_context_include_path_clear(بنية xkb_context *ctx)
{
    حرف **path;

    darray_foreach(path, ctx->includes)
        حرر(*path);
    darray_free(ctx->includes);

    darray_foreach(path, ctx->failed_includes)
        حرر(*path);
    darray_free(ctx->failed_includes);

    /* It does not make sense to keep the pending defaults */
    ctx->pending_default_includes = false;
}

/**
 * xkb_context_include_path_clear() + xkb_context_include_path_append_default()
 */
صحيح
xkb_context_include_path_reset_defaults(بنية xkb_context *ctx)
{
    xkb_context_include_path_clear(ctx);
    أرجع xkb_context_include_path_append_default(ctx);
}

/**
 * Returns the number of entries in the context's include path.
 */
unsigned صحيح
xkb_context_لمب_include_paths(بنية xkb_context *ctx)
{
    أرجع (xkb_context_init_includes(ctx))
        ? darray_size(ctx->includes)
        : 0;
}

/**
 * Returns the given entry in the context's include path, or NULL if an
 * invalid index is passed.
 */
ثابت حرف *
xkb_context_include_path_get(بنية xkb_context *ctx, unsigned صحيح idx)
{
    إذا (idx >= xkb_context_لمب_include_paths(ctx))
        أرجع عدم;

    أرجع darray_item(ctx->includes, idx);
}

/**
 * Take a new reference on the context.
 */
بنية xkb_context *
xkb_context_ref(بنية xkb_context *ctx)
{
    أكد(ctx->refcnt > 0);
    ctx->refcnt++;
    أرجع ctx;
}

/**
 * Drop an existing reference on the context, and free it if the refcnt is
 * now 0.
 */
فراغ
xkb_context_unref(بنية xkb_context *ctx)
{
    أكد(!ctx || ctx->refcnt > 0);
    إذا (!ctx || --ctx->refcnt > 0)
        أرجع;

    حرر(ctx->x11_atom_cache);
    xkb_context_include_path_clear(ctx);
    atom_table_free(ctx->atom_table);
    حرر(ctx);
}

ساكن ثابت حرف *
log_level_to_prefix(تعداد xkb_log_level level)
{
    بدل (level) {
    حالة XKB_LOG_LEVEL_DEBUG:
        أرجع "xkbcommon: DEBUG: ";
    حالة XKB_LOG_LEVEL_INFO:
        أرجع "xkbcommon: INFO: ";
    حالة XKB_LOG_LEVEL_WARNING:
        أرجع "xkbcommon: WARNING: ";
    حالة XKB_LOG_LEVEL_ERROR:
        أرجع "xkbcommon: ERROR: ";
    حالة XKB_LOG_LEVEL_CRITICAL:
        أرجع "xkbcommon: CRITICAL: ";
    افتراضي:
        أرجع عدم;
    }
}

ATTR_PRINTF(3, 0) ساكن فراغ
default_log_fn(بنية xkb_context *ctx, تعداد xkb_log_level level,
               ثابت حرف *fmt, va_list حجوجي)
{
    ثابت حرف *prefix = log_level_to_prefix(level);

    إذا (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, حجوجي);
}

ساكن تعداد xkb_log_level
log_level(ثابت حرف *level) {
    errno = 0;
    حرف *endptr = عدم;
    ثابت long lvl = strtol(level, &endptr, 10);
    إذا (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        أرجع (تعداد xkb_log_level)lvl;
    إذا (istreq_prefix("crit", level))
        أرجع XKB_LOG_LEVEL_CRITICAL;
    إذا (istreq_prefix("err", level))
        أرجع XKB_LOG_LEVEL_ERROR;
    إذا (istreq_prefix("warn", level))
        أرجع XKB_LOG_LEVEL_WARNING;
    إذا (istreq_prefix("info", level))
        أرجع XKB_LOG_LEVEL_INFO;
    إذا (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        أرجع XKB_LOG_LEVEL_DEBUG;

    أرجع XKB_LOG_LEVEL_ERROR;
}

ساكن صحيح
log_verbosity(ثابت حرف *verbosity) {
    errno = 0;
    ثابت long صحيح v = strtol(verbosity, عدم, 10);
    إذا (errno == 0) {
        /* Truncate */
        أرجع (صحيح) v;
    }

    أرجع XKB_LOG_VERBOSITY_DEFAULT;
}

/**
 * Create a new context.
 */
بنية xkb_context *
xkb_context_new(تعداد xkb_context_flags flags)
{
    ثابت حرف *env;
    بنية xkb_context *ctx = calloc(1, حجم(*ctx));

    إذا (!ctx)
        أرجع عدم;

    ctx->refcnt = 1;
    ctx->log_fn = default_log_fn;
    ctx->log_level = XKB_LOG_LEVEL_ERROR;
    ctx->log_verbosity = XKB_LOG_VERBOSITY_DEFAULT;

    ثابت تعداد xkb_context_flags invalid_flags =
        (flags & ~(تعداد xkb_context_flags)XKB_CONTEXT_FLAGS_درجوجي);
    إذا (invalid_flags) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Invalid context flags: 0x%x\n", invalid_flags);
        حرر(ctx);
        أرجع عدم;
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
    إذا (env)
        xkb_context_set_log_level(ctx, log_level(env));

    env = xkb_context_getenv(ctx, "XKB_LOG_VERBOSITY");
    إذا (env)
        xkb_context_set_log_verbosity(ctx, log_verbosity(env));

    ctx->atom_table = atom_table_new();
    إذا (!ctx->atom_table) {
        xkb_context_unref(ctx);
        أرجع عدم;
    }

    ctx->x11_atom_cache = عدم;

    أرجع ctx;
}

فراغ
xkb_context_set_log_fn(بنية xkb_context *ctx,
                       فراغ (*log_fn)(بنية xkb_context *ctx,
                                      تعداد xkb_log_level level,
                                      ثابت حرف *fmt, va_list حجوجي))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

تعداد xkb_log_level
xkb_context_get_log_level(بنية xkb_context *ctx)
{
    أرجع ctx->log_level;
}

فراغ
xkb_context_set_log_level(بنية xkb_context *ctx, تعداد xkb_log_level level)
{
    ctx->log_level = level;
}

صحيح
xkb_context_get_log_verbosity(بنية xkb_context *ctx)
{
    أرجع ctx->log_verbosity;
}

فراغ
xkb_context_set_log_verbosity(بنية xkb_context *ctx, صحيح verbosity)
{
    ctx->log_verbosity = verbosity;
}

فراغ *
xkb_context_get_user_data(بنية xkb_context *ctx)
{
    إذا (ctx)
        أرجع ctx->user_data;
    أرجع عدم;
}

فراغ
xkb_context_set_user_data(بنية xkb_context *ctx, فراغ *user_data)
{
    ctx->user_data = user_data;
}
