/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#شامل "config.h"

#شامل <assert.h>
#شامل <errno.h>
#شامل <string.h>
#شامل <sys/types.h>
#شامل <sys/stat.h>

#اگر HAVE_XKB_EXTENSIONS_DIRECTORIES
    #شامل <limits.h>
    #اگر HAVE_UNISTD_H && HAVE_DIRENT_H
        #شامل <dirent.h>
        #شامل <unistd.h>
    #endif
#endif

#شامل "xkbcommon/xkbcommon.h"
#شامل "context.h"
#شامل "darray.h"
#شامل "features/enums.h"
#شامل "messages-codes.h"
#شامل "utils.h"


/**
 * Append one directory to the context’s include path.
 */
ایستا صحیح
context_include_path_append(ساختار xkb_context *ctx, ثابت نویسه *path)
{
    صحیح err = ENOMEM;

    نویسه *tmp = strdup(path);
    اگر (!tmp)
        برو err;

    ساختار stat stat_buf;
    err = stat(path, &stat_buf);
    اگر (err != 0) {
        err = errno;
        برو err;
    }
    اگر (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        برو err;
    }

    اگر (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        برو err;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    بازگشت 1;

err:
    اگر (tmp)
        darray_append(ctx->failed_includes, tmp);
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, strerror(err));
    بازگشت 0;
}

/**
 * Append one directory to the context’s include path.
 */
صحیح
xkb_context_include_path_append(ساختار xkb_context *ctx, ثابت نویسه *path)
{
    بازگشت (xkb_context_init_includes(ctx))
        ? context_include_path_append(ctx, path)
        : 0;
}

ثابت نویسه *
xkb_context_include_path_get_extra_path(ساختار xkb_context *ctx)
{
    ثابت نویسه * ثابت extra = xkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    بازگشت extra ? extra : DFLT_XKB_CONFIG_EXTRA_PATH;
}

#اگر HAVE_XKB_EXTENSIONS_DIRECTORIES
ثابت نویسه *
xkb_context_include_path_get_unversioned_extensions_path(ساختار xkb_context *ctx)
{
    ثابت نویسه *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
    بازگشت ext
        ? ext
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#وگرنه
        : پوچ;
#endif
}

ثابت نویسه *
xkb_context_include_path_get_versioned_extensions_path(ساختار xkb_context *ctx)
{
    ثابت نویسه *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
    بازگشت ext
        ? ext
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#وگرنه
        : پوچ;
#endif
}

ایستا صحیح
compare_str(ثابت تهی *a, ثابت تهی *b)
{
    بازگشت strcmp(*(نویسه **)a, *(نویسه **) b);
}

ایستا صحیح
add_direct_subdirectories(ساختار xkb_context *ctx, ثابت نویسه *path,
                          darray_string *extensions,
                          darray_size_t versioned_شمار,
                          size_t versioned_path_length)
{
    صحیح ret = 0;
    صحیح err = ENOMEM;
    DIR *dir = پوچ;

    /* Check extensions parent directory */
    ساختار stat stat_buf;
    err = stat(path, &stat_buf);
    اگر (err != 0) {
        err = errno;
        برو err;
    }
    اگر (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        برو err;
    }
    اگر (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        برو err;
    }

    dir = opendir(path);
    اگر (dir == پوچ) {
        err = EACCES;
        برو err;
    }

    ساختار dirent *entry;
    نویسه path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    وقتی ((entry = readdir(dir)) != پوچ) {
        ثابت نویسه *name = entry->d_name;

        /* Skip special entries */
        اگر (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            ادامه;

        اگر (!snprintf_safe(path_buf, اندازه(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            برو err;
        }
        اگر (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            ادامه;

        /* Skip if the corresponding versioned directory is already included */
        برای (darray_size_t i = 0; i < versioned_شمار; i++) {
            ثابت نویسه * ثابت prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            اگر (strcmp(name, prev_name) == 0)
                برو next;
        }

        نویسه *ext_path = strdup_safe(path_buf);
        اگر (!ext_path) {
            err = ENOMEM;
            برو err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    اگر (darray_size(*extensions) > versioned_شمار) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_اقلام(*extensions) + versioned_شمار,
              darray_size(*extensions) - versioned_شمار,
              اندازه(*darray_اقلام(*extensions)), &compare_str);
        نویسه **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_شمار) {
            ret |= context_include_path_append(ctx, *ext_path);
        }
    }

    بازگشت ret;

err:
    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Include extensions path failed: %s (%s)\n", path, strerror(err));
    اگر (dir)
        closedir(dir);

    بازگشت ret;
}
#endif

ثابت نویسه *
xkb_context_include_path_get_system_path(ساختار xkb_context *ctx)
{
    ثابت نویسه * ثابت ریشه = xkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    بازگشت ریشه ? ریشه : DFLT_XKB_CONFIG_ریشه;
}

/**
 * Append the default include directories to the context.
 */
صحیح
xkb_context_include_path_append_default(ساختار xkb_context *ctx)
{
    /*
     * We do not call `xkb_context_init_includes()` here, because either
     * we already initialized the includes paths or we are doing it now.
     */

    نویسه *user_path;
    صحیح ret = 0;

    ثابت نویسه * ثابت home = xkb_context_getenv(ctx, "HOME");
    ثابت نویسه * ثابت xdg = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    اگر (xdg != پوچ) {
        user_path = asprintf_safe("%s/xkb", xdg);
        اگر (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            آزاد(user_path);
        }
    } وگرنه اگر (home != پوچ) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        user_path = asprintf_safe("%s/.config/xkb", home);
        اگر (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            آزاد(user_path);
        }
    }

    اگر (home != پوچ) {
        user_path = asprintf_safe("%s/.xkb", home);
        اگر (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            آزاد(user_path);
        }
    }

    ثابت نویسه * ثابت extra = xkb_context_include_path_get_extra_path(ctx);
    ret |= context_include_path_append(ctx, extra);

#اگر HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ثابت نویسه *extensions_path =
        xkb_context_include_path_get_versioned_extensions_path(ctx);
    size_t versioned_path_length = 0;
    اگر (extensions_path) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = strlen(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        xkb_context_include_path_get_unversioned_extensions_path(ctx);
    اگر (extensions_path) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    نویسه **ext_path;
    darray_foreach(ext_path, extensions) {
        آزاد(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    ثابت نویسه * ثابت ریشه = xkb_context_include_path_get_system_path(ctx);
    ثابت bool has_ریشه = context_include_path_append(ctx, ریشه);
    ret |= has_ریشه;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    اگر (!has_ریشه && ریشه[0] != '\0') {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_ریشه, ریشه);
        ret |= context_include_path_append(ctx, DFLT_XKB_LEGACY_ریشه);
    }

    بازگشت ret;
}

/**
 * Remove all entries in the context's include path.
 */
تهی
xkb_context_include_path_clear(ساختار xkb_context *ctx)
{
    نویسه **path;

    darray_foreach(path, ctx->includes)
        آزاد(*path);
    darray_free(ctx->includes);

    darray_foreach(path, ctx->failed_includes)
        آزاد(*path);
    darray_free(ctx->failed_includes);

    /* It does not make sense to keep the pending defaults */
    ctx->pending_default_includes = false;
}

/**
 * xkb_context_include_path_clear() + xkb_context_include_path_append_default()
 */
صحیح
xkb_context_include_path_reset_defaults(ساختار xkb_context *ctx)
{
    xkb_context_include_path_clear(ctx);
    بازگشت xkb_context_include_path_append_default(ctx);
}

/**
 * Returns the number of entries in the context's include path.
 */
unsigned صحیح
xkb_context_رقم_include_paths(ساختار xkb_context *ctx)
{
    بازگشت (xkb_context_init_includes(ctx))
        ? darray_size(ctx->includes)
        : 0;
}

/**
 * Returns the given entry in the context's include path, or NULL if an
 * invalid index is passed.
 */
ثابت نویسه *
xkb_context_include_path_get(ساختار xkb_context *ctx, unsigned صحیح idx)
{
    اگر (idx >= xkb_context_رقم_include_paths(ctx))
        بازگشت پوچ;

    بازگشت darray_item(ctx->includes, idx);
}

/**
 * Take a new reference on the context.
 */
ساختار xkb_context *
xkb_context_ref(ساختار xkb_context *ctx)
{
    أكد(ctx->refcnt > 0);
    ctx->refcnt++;
    بازگشت ctx;
}

/**
 * Drop an existing reference on the context, and free it if the refcnt is
 * now 0.
 */
تهی
xkb_context_unref(ساختار xkb_context *ctx)
{
    أكد(!ctx || ctx->refcnt > 0);
    اگر (!ctx || --ctx->refcnt > 0)
        بازگشت;

    آزاد(ctx->x11_atom_cache);
    xkb_context_include_path_clear(ctx);
    atom_table_free(ctx->atom_table);
    آزاد(ctx);
}

ایستا ثابت نویسه *
log_level_to_prefix(شمارش xkb_log_level level)
{
    انتخاب (level) {
    حالت XKB_LOG_LEVEL_DEBUG:
        بازگشت "xkbcommon: DEBUG: ";
    حالت XKB_LOG_LEVEL_INFO:
        بازگشت "xkbcommon: INFO: ";
    حالت XKB_LOG_LEVEL_WARNING:
        بازگشت "xkbcommon: WARNING: ";
    حالت XKB_LOG_LEVEL_ERROR:
        بازگشت "xkbcommon: ERROR: ";
    حالت XKB_LOG_LEVEL_CRITICAL:
        بازگشت "xkbcommon: CRITICAL: ";
    پیشفرض:
        بازگشت پوچ;
    }
}

ATTR_PRINTF(3, 0) ایستا تهی
default_log_fn(ساختار xkb_context *ctx, شمارش xkb_log_level level,
               ثابت نویسه *fmt, va_list args)
{
    ثابت نویسه *prefix = log_level_to_prefix(level);

    اگر (prefix)
        fprintf(stderr, "%s", prefix);
    vfprintf(stderr, fmt, args);
}

ایستا شمارش xkb_log_level
log_level(ثابت نویسه *level) {
    errno = 0;
    نویسه *endptr = پوچ;
    ثابت long lvl = strtol(level, &endptr, 10);
    اگر (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        بازگشت (شمارش xkb_log_level)lvl;
    اگر (istreq_prefix("crit", level))
        بازگشت XKB_LOG_LEVEL_CRITICAL;
    اگر (istreq_prefix("err", level))
        بازگشت XKB_LOG_LEVEL_ERROR;
    اگر (istreq_prefix("warn", level))
        بازگشت XKB_LOG_LEVEL_WARNING;
    اگر (istreq_prefix("info", level))
        بازگشت XKB_LOG_LEVEL_INFO;
    اگر (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        بازگشت XKB_LOG_LEVEL_DEBUG;

    بازگشت XKB_LOG_LEVEL_ERROR;
}

ایستا صحیح
log_verbosity(ثابت نویسه *verbosity) {
    errno = 0;
    ثابت long صحیح v = strtol(verbosity, پوچ, 10);
    اگر (errno == 0) {
        /* Truncate */
        بازگشت (صحیح) v;
    }

    بازگشت XKB_LOG_VERBOSITY_DEFAULT;
}

/**
 * Create a new context.
 */
ساختار xkb_context *
xkb_context_new(شمارش xkb_context_flags flags)
{
    ثابت نویسه *env;
    ساختار xkb_context *ctx = calloc(1, اندازه(*ctx));

    اگر (!ctx)
        بازگشت پوچ;

    ctx->refcnt = 1;
    ctx->log_fn = default_log_fn;
    ctx->log_level = XKB_LOG_LEVEL_ERROR;
    ctx->log_verbosity = XKB_LOG_VERBOSITY_DEFAULT;

    ثابت شمارش xkb_context_flags invalid_flags =
        (flags & ~(شمارش xkb_context_flags)XKB_CONTEXT_FLAGS_مقادیر);
    اگر (invalid_flags) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Invalid context flags: 0x%x\n", invalid_flags);
        آزاد(ctx);
        بازگشت پوچ;
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
    اگر (env)
        xkb_context_set_log_level(ctx, log_level(env));

    env = xkb_context_getenv(ctx, "XKB_LOG_VERBOSITY");
    اگر (env)
        xkb_context_set_log_verbosity(ctx, log_verbosity(env));

    ctx->atom_table = atom_table_new();
    اگر (!ctx->atom_table) {
        xkb_context_unref(ctx);
        بازگشت پوچ;
    }

    ctx->x11_atom_cache = پوچ;

    بازگشت ctx;
}

تهی
xkb_context_set_log_fn(ساختار xkb_context *ctx,
                       تهی (*log_fn)(ساختار xkb_context *ctx,
                                      شمارش xkb_log_level level,
                                      ثابت نویسه *fmt, va_list args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

شمارش xkb_log_level
xkb_context_get_log_level(ساختار xkb_context *ctx)
{
    بازگشت ctx->log_level;
}

تهی
xkb_context_set_log_level(ساختار xkb_context *ctx, شمارش xkb_log_level level)
{
    ctx->log_level = level;
}

صحیح
xkb_context_get_log_verbosity(ساختار xkb_context *ctx)
{
    بازگشت ctx->log_verbosity;
}

تهی
xkb_context_set_log_verbosity(ساختار xkb_context *ctx, صحیح verbosity)
{
    ctx->log_verbosity = verbosity;
}

تهی *
xkb_context_get_user_data(ساختار xkb_context *ctx)
{
    اگر (ctx)
        بازگشت ctx->user_data;
    بازگشت پوچ;
}

تهی
xkb_context_set_user_data(ساختار xkb_context *ctx, تهی *user_data)
{
    ctx->user_data = user_data;
}
