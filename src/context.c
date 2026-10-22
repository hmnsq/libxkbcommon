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
    #اگر_ختم
#اگر_ختم

#شامل "xkbcommon/xkbcommon.h"
#شامل "context.h"
#شامل "darray.h"
#شامل "features/enums.h"
#شامل "messages-codes.h"
#شامل "utils.h"


/**
 * Append one directory to the context’s include path.
 */
ساکن صحیح
context_include_راستہ_append(ساخت xkb_context *ctx, ثابت حرف *راستہ)
{
    صحیح err = ENOMEM;

    حرف *tmp = strdup(راستہ);
    اگر (!tmp)
        جاؤ err;

    ساخت stat stat_buf;
    err = stat(راستہ, &stat_buf);
    اگر (err != 0) {
        err = errno;
        جاؤ err;
    }
    اگر (!S_ISDIR(stat_buf.st_منوال)) {
        err = ENOTDIR;
        جاؤ err;
    }

    اگر (!check_eaccess(راستہ, R_OK | X_OK)) {
        err = EACCES;
        جاؤ err;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_پیغام_NO_ID, "Include path added: %s\n", tmp);

    واپس 1;

err:
    اگر (tmp)
        darray_append(ctx->failed_includes, tmp);
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_پیغام_NO_ID,
             "Include path failed: \"%s\" (%s)\n", راستہ, strerror(err));
    واپس 0;
}

/**
 * Append one directory to the context’s include path.
 */
صحیح
xkb_context_include_راستہ_append(ساخت xkb_context *ctx, ثابت حرف *راستہ)
{
    واپس (xkb_context_init_includes(ctx))
        ? context_include_راستہ_append(ctx, راستہ)
        : 0;
}

ثابت حرف *
xkb_context_include_راستہ_get_extra_راستہ(ساخت xkb_context *ctx)
{
    ثابت حرف * ثابت extra = xkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    واپس extra ? extra : DFLT_XKB_CONFIG_EXTRA_راستہ;
}

#اگر HAVE_XKB_EXTENSIONS_DIRECTORIES
ثابت حرف *
xkb_context_include_راستہ_get_unversioned_extensions_راستہ(ساخت xkb_context *ctx)
{
    ثابت حرف *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
    واپس ext
        ? ext
#اگر_تعریف_شدہ DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_راستہ
        : DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_راستہ;
#ورنہ
        : تہی;
#اگر_ختم
}

ثابت حرف *
xkb_context_include_راستہ_get_versioned_extensions_راستہ(ساخت xkb_context *ctx)
{
    ثابت حرف *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
    واپس ext
        ? ext
#اگر_تعریف_شدہ DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_راستہ
        : DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_راستہ;
#ورنہ
        : تہی;
#اگر_ختم
}

ساکن صحیح
compare_str(ثابت خالی *a, ثابت خالی *b)
{
    واپس strcmp(*(حرف **)a, *(حرف **) b);
}

ساکن صحیح
جوڑو_براہ_راست_subdirectories(ساخت xkb_context *ctx, ثابت حرف *راستہ,
                          darray_string *extensions,
                          darray_سائز_t versioned_گنتی,
                          سائز_t versioned_راستہ_لمبائی)
{
    صحیح ret = 0;
    صحیح err = ENOMEM;
    DIR *dir = تہی;

    /* Check extensions parent directory */
    ساخت stat stat_buf;
    err = stat(راستہ, &stat_buf);
    اگر (err != 0) {
        err = errno;
        جاؤ err;
    }
    اگر (!S_ISDIR(stat_buf.st_منوال)) {
        err = ENOTDIR;
        جاؤ err;
    }
    اگر (!check_eaccess(راستہ, R_OK | X_OK)) {
        err = EACCES;
        جاؤ err;
    }

    dir = opendir(راستہ);
    اگر (dir == تہی) {
        err = EACCES;
        جاؤ err;
    }

    ساخت dirent *اندراج_2;
    حرف راستہ_buf[PATH_MAX] = "";
    versioned_راستہ_لمبائی++; /* Additional final ‘/’ */
    جب ((اندراج_2 = readdir(dir)) != تہی) {
        ثابت حرف *name = اندراج_2->d_name;

        /* Skip special entries */
        اگر (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            جاری;

        اگر (!snprintf_محفوظ(راستہ_buf, حجم(راستہ_buf), "%s/%s", راستہ, name)) {
            err = ENOMEM;
            جاؤ err;
        }
        اگر (stat(راستہ_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_منوال))
            جاری;

        /* Skip if the corresponding versioned directory is already included */
        برائے (darray_سائز_t i = 0; i < versioned_گنتی; i++) {
            ثابت حرف * ثابت پچھلا_name = darray_شے(*extensions, i)
                                         + versioned_راستہ_لمبائی;
            اگر (strcmp(name, پچھلا_name) == 0)
                جاؤ اگلا;
        }

        حرف *ext_راستہ = strdup_محفوظ(راستہ_buf);
        اگر (!ext_راستہ) {
            err = ENOMEM;
            جاؤ err;
        }

        darray_append(*extensions, ext_راستہ);
اگلا:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    اگر (darray_سائز(*extensions) > versioned_گنتی) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_اشیاء(*extensions) + versioned_گنتی,
              darray_سائز(*extensions) - versioned_گنتی,
              حجم(*darray_اشیاء(*extensions)), &compare_str);
        حرف **ext_راستہ;
        darray_foreach_from(ext_راستہ, *extensions, versioned_گنتی) {
            ret |= context_include_راستہ_append(ctx, *ext_راستہ);
        }
    }

    واپس ret;

err:
    log_dbg(ctx, XKB_LOG_پیغام_NO_ID,
            "Include extensions path failed: %s (%s)\n", راستہ, strerror(err));
    اگر (dir)
        closedir(dir);

    واپس ret;
}
#اگر_ختم

ثابت حرف *
xkb_context_include_راستہ_get_system_راستہ(ساخت xkb_context *ctx)
{
    ثابت حرف * ثابت جڑ = xkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    واپس جڑ ? جڑ : DFLT_XKB_CONFIG_جڑ;
}

/**
 * Append the default include directories to the context.
 */
صحیح
xkb_context_include_راستہ_append_default(ساخت xkb_context *ctx)
{
    /*
     * We do not call `xkb_context_init_includes()` here, because either
     * we already initialized the includes paths or we are doing it now.
     */

    حرف *user_راستہ;
    صحیح ret = 0;

    ثابت حرف * ثابت home = xkb_context_getenv(ctx, "HOME");
    ثابت حرف * ثابت xdg = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    اگر (xdg != تہی) {
        user_راستہ = asprintf_محفوظ("%s/xkb", xdg);
        اگر (user_راستہ) {
            ret |= context_include_راستہ_append(ctx, user_راستہ);
            آزاد(user_راستہ);
        }
    } ورنہ اگر (home != تہی) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        user_راستہ = asprintf_محفوظ("%s/.config/xkb", home);
        اگر (user_راستہ) {
            ret |= context_include_راستہ_append(ctx, user_راستہ);
            آزاد(user_راستہ);
        }
    }

    اگر (home != تہی) {
        user_راستہ = asprintf_محفوظ("%s/.xkb", home);
        اگر (user_راستہ) {
            ret |= context_include_راستہ_append(ctx, user_راستہ);
            آزاد(user_راستہ);
        }
    }

    ثابت حرف * ثابت extra = xkb_context_include_راستہ_get_extra_راستہ(ctx);
    ret |= context_include_راستہ_append(ctx, extra);

#اگر HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ثابت حرف *extensions_راستہ =
        xkb_context_include_راستہ_get_versioned_extensions_راستہ(ctx);
    سائز_t versioned_راستہ_لمبائی = 0;
    اگر (extensions_راستہ) {
        /* Add direct subdirectories */
        ret |= جوڑو_براہ_راست_subdirectories(ctx, extensions_راستہ,
                                         &extensions, 0, 0);
        versioned_راستہ_لمبائی = لڑی_لمبائی(extensions_راستہ);
    }

    /* Unversioned extensions directory */
    extensions_راستہ =
        xkb_context_include_راستہ_get_unversioned_extensions_راستہ(ctx);
    اگر (extensions_راستہ) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= جوڑو_براہ_راست_subdirectories(ctx, extensions_راستہ, &extensions,
                                         darray_سائز(extensions),
                                         versioned_راستہ_لمبائی);
    }

    حرف **ext_راستہ;
    darray_foreach(ext_راستہ, extensions) {
        آزاد(*ext_راستہ);
    }
    darray_free(extensions);
#اگر_ختم

    /* Canonical XKB root */
    ثابت حرف * ثابت جڑ = xkb_context_include_راستہ_get_system_راستہ(ctx);
    ثابت bool has_جڑ = context_include_راستہ_append(ctx, جڑ);
    ret |= has_جڑ;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    اگر (!has_جڑ && جڑ[0] != '\0') {
        log_warn(ctx, XKB_LOG_پیغام_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_جڑ, جڑ);
        ret |= context_include_راستہ_append(ctx, DFLT_XKB_LEGACY_جڑ);
    }

    واپس ret;
}

/**
 * Remove all entries in the context's include path.
 */
خالی
xkb_context_include_راستہ_clear(ساخت xkb_context *ctx)
{
    حرف **راستہ;

    darray_foreach(راستہ, ctx->includes)
        آزاد(*راستہ);
    darray_free(ctx->includes);

    darray_foreach(راستہ, ctx->failed_includes)
        آزاد(*راستہ);
    darray_free(ctx->failed_includes);

    /* It does not make sense to keep the pending defaults */
    ctx->pending_default_includes = false;
}

/**
 * xkb_context_include_path_clear() + xkb_context_include_path_append_default()
 */
صحیح
xkb_context_include_راستہ_reset_defaults(ساخت xkb_context *ctx)
{
    xkb_context_include_راستہ_clear(ctx);
    واپس xkb_context_include_راستہ_append_default(ctx);
}

/**
 * Returns the number of entries in the context's include path.
 */
غیرعلامتی صحیح
xkb_context_عدد_include_paths(ساخت xkb_context *ctx)
{
    واپس (xkb_context_init_includes(ctx))
        ? darray_سائز(ctx->includes)
        : 0;
}

/**
 * Returns the given entry in the context's include path, or NULL if an
 * invalid index is passed.
 */
ثابت حرف *
xkb_context_include_راستہ_get(ساخت xkb_context *ctx, غیرعلامتی صحیح idx)
{
    اگر (idx >= xkb_context_عدد_include_paths(ctx))
        واپس تہی;

    واپس darray_شے(ctx->includes, idx);
}

/**
 * Take a new reference on the context.
 */
ساخت xkb_context *
xkb_context_ref(ساخت xkb_context *ctx)
{
    أكد(ctx->refcnt > 0);
    ctx->refcnt++;
    واپس ctx;
}

/**
 * Drop an existing reference on the context, and free it if the refcnt is
 * now 0.
 */
خالی
xkb_context_unref(ساخت xkb_context *ctx)
{
    أكد(!ctx || ctx->refcnt > 0);
    اگر (!ctx || --ctx->refcnt > 0)
        واپس;

    آزاد(ctx->x11_atom_cache);
    xkb_context_include_راستہ_clear(ctx);
    atom_جدول_free(ctx->atom_جدول);
    آزاد(ctx);
}

ساکن ثابت حرف *
log_level_مقصد_سابقہ(شمار xkb_log_level level)
{
    انتخاب (level) {
    حالت XKB_LOG_LEVEL_DEBUG:
        واپس "xkbcommon: DEBUG: ";
    حالت XKB_LOG_LEVEL_INFO:
        واپس "xkbcommon: INFO: ";
    حالت XKB_LOG_LEVEL_WARNING:
        واپس "xkbcommon: WARNING: ";
    حالت XKB_LOG_LEVEL_ERROR:
        واپس "xkbcommon: ERROR: ";
    حالت XKB_LOG_LEVEL_CRITICAL:
        واپس "xkbcommon: CRITICAL: ";
    طےشدہ:
        واپس تہی;
    }
}

ATTR_PRINTF(3, 0) ساکن خالی
default_log_fn(ساخت xkb_context *ctx, شمار xkb_log_level level,
               ثابت حرف *fmt, va_list دلائل)
{
    ثابت حرف *سابقہ = log_level_مقصد_سابقہ(level);

    اگر (سابقہ)
        fprintf(stderr, "%s", سابقہ);
    vfprintf(stderr, fmt, دلائل);
}

ساکن شمار xkb_log_level
log_level(ثابت حرف *level) {
    errno = 0;
    حرف *endptr = تہی;
    ثابت طویل lvl = strtol(level, &endptr, 10);
    اگر (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        واپس (شمار xkb_log_level)lvl;
    اگر (istreq_سابقہ("crit", level))
        واپس XKB_LOG_LEVEL_CRITICAL;
    اگر (istreq_سابقہ("err", level))
        واپس XKB_LOG_LEVEL_ERROR;
    اگر (istreq_سابقہ("warn", level))
        واپس XKB_LOG_LEVEL_WARNING;
    اگر (istreq_سابقہ("info", level))
        واپس XKB_LOG_LEVEL_INFO;
    اگر (istreq_سابقہ("debug", level) || istreq_سابقہ("dbg", level))
        واپس XKB_LOG_LEVEL_DEBUG;

    واپس XKB_LOG_LEVEL_ERROR;
}

ساکن صحیح
log_verbosity(ثابت حرف *verbosity) {
    errno = 0;
    ثابت طویل صحیح v = strtol(verbosity, تہی, 10);
    اگر (errno == 0) {
        /* Truncate */
        واپس (صحیح) v;
    }

    واپس XKB_LOG_VERBOSITY_DEFAULT;
}

/**
 * Create a new context.
 */
ساخت xkb_context *
xkb_context_new(شمار xkb_context_flags flags)
{
    ثابت حرف *env;
    ساخت xkb_context *ctx = صفر_حافظہ_دو(1, حجم(*ctx));

    اگر (!ctx)
        واپس تہی;

    ctx->refcnt = 1;
    ctx->log_fn = default_log_fn;
    ctx->log_level = XKB_LOG_LEVEL_ERROR;
    ctx->log_verbosity = XKB_LOG_VERBOSITY_DEFAULT;

    ثابت شمار xkb_context_flags invalid_flags =
        (flags & ~(شمار xkb_context_flags)XKB_CONTEXT_FLAGS_اقدار);
    اگر (invalid_flags) {
        log_err_func(ctx, XKB_LOG_پیغام_NO_ID,
                     "Invalid context flags: 0x%x\n", invalid_flags);
        آزاد(ctx);
        واپس تہی;
    }

    ctx->use_environment_نام = !(flags & XKB_CONTEXT_NO_ENVIRONMENT_نام);
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
        xkb_context_سیٹ_log_level(ctx, log_level(env));

    env = xkb_context_getenv(ctx, "XKB_LOG_VERBOSITY");
    اگر (env)
        xkb_context_سیٹ_log_verbosity(ctx, log_verbosity(env));

    ctx->atom_جدول = atom_جدول_new();
    اگر (!ctx->atom_جدول) {
        xkb_context_unref(ctx);
        واپس تہی;
    }

    ctx->x11_atom_cache = تہی;

    واپس ctx;
}

خالی
xkb_context_سیٹ_log_fn(ساخت xkb_context *ctx,
                       خالی (*log_fn)(ساخت xkb_context *ctx,
                                      شمار xkb_log_level level,
                                      ثابت حرف *fmt, va_list دلائل))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

شمار xkb_log_level
xkb_context_get_log_level(ساخت xkb_context *ctx)
{
    واپس ctx->log_level;
}

خالی
xkb_context_سیٹ_log_level(ساخت xkb_context *ctx, شمار xkb_log_level level)
{
    ctx->log_level = level;
}

صحیح
xkb_context_get_log_verbosity(ساخت xkb_context *ctx)
{
    واپس ctx->log_verbosity;
}

خالی
xkb_context_سیٹ_log_verbosity(ساخت xkb_context *ctx, صحیح verbosity)
{
    ctx->log_verbosity = verbosity;
}

خالی *
xkb_context_get_user_ڈیٹا(ساخت xkb_context *ctx)
{
    اگر (ctx)
        واپس ctx->user_ڈیٹا;
    واپس تہی;
}

خالی
xkb_context_سیٹ_user_ڈیٹا(ساخت xkb_context *ctx, خالی *user_ڈیٹا)
{
    ctx->user_ڈیٹا = user_ڈیٹا;
}
