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
    #پایان_اگر
#پایان_اگر

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
context_include_مسیر_append(بنية xkb_context *ctx, ثابت حرف *مسیر)
{
    صحيح err = ENOMEM;

    حرف *tmp = strdup(مسیر);
    إذا (!tmp)
        اذهب err;

    بنية stat stat_buf;
    err = stat(مسیر, &stat_buf);
    إذا (err != 0) {
        err = errno;
        اذهب err;
    }
    إذا (!S_ISDIR(stat_buf.st_نما)) {
        err = ENOTDIR;
        اذهب err;
    }

    إذا (!check_eaccess(مسیر, R_OK | X_OK)) {
        err = EACCES;
        اذهب err;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_پیام_NO_ID, "Include path added: %s\n", tmp);

    أرجع 1;

err:
    إذا (tmp)
        darray_append(ctx->failed_includes, tmp);
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_پیام_NO_ID,
             "Include path failed: \"%s\" (%s)\n", مسیر, strerror(err));
    أرجع 0;
}

/**
 * Append one directory to the context’s include path.
 */
صحيح
xkb_context_include_مسیر_append(بنية xkb_context *ctx, ثابت حرف *مسیر)
{
    أرجع (xkb_context_init_includes(ctx))
        ? context_include_مسیر_append(ctx, مسیر)
        : 0;
}

ثابت حرف *
xkb_context_include_مسیر_get_extra_مسیر(بنية xkb_context *ctx)
{
    ثابت حرف * ثابت extra = xkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    أرجع extra ? extra : DFLT_XKB_CONFIG_EXTRA_مسیر;
}

#إذا HAVE_XKB_EXTENSIONS_DIRECTORIES
ثابت حرف *
xkb_context_include_مسیر_get_unversioned_extensions_مسیر(بنية xkb_context *ctx)
{
    ثابت حرف *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
    أرجع ext
        ? ext
#اگر_تعریف_شده DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_مسیر
        : DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_مسیر;
#وإلا
        : عدم;
#پایان_اگر
}

ثابت حرف *
xkb_context_include_مسیر_get_versioned_extensions_مسیر(بنية xkb_context *ctx)
{
    ثابت حرف *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
    أرجع ext
        ? ext
#اگر_تعریف_شده DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_مسیر
        : DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_مسیر;
#وإلا
        : عدم;
#پایان_اگر
}

ساكن صحيح
compare_str(ثابت فراغ *a, ثابت فراغ *b)
{
    أرجع strcmp(*(حرف **)a, *(حرف **) b);
}

ساكن صحيح
جمع_کن_مستقیم_subdirectories(بنية xkb_context *ctx, ثابت حرف *مسیر,
                          darray_string *extensions,
                          darray_اندازه_t versioned_شمار,
                          اندازه_t versioned_مسیر_درازی)
{
    صحيح ret = 0;
    صحيح err = ENOMEM;
    DIR *dir = عدم;

    /* Check extensions parent directory */
    بنية stat stat_buf;
    err = stat(مسیر, &stat_buf);
    إذا (err != 0) {
        err = errno;
        اذهب err;
    }
    إذا (!S_ISDIR(stat_buf.st_نما)) {
        err = ENOTDIR;
        اذهب err;
    }
    إذا (!check_eaccess(مسیر, R_OK | X_OK)) {
        err = EACCES;
        اذهب err;
    }

    dir = opendir(مسیر);
    إذا (dir == عدم) {
        err = EACCES;
        اذهب err;
    }

    بنية dirent *مدخل;
    حرف مسیر_buf[PATH_MAX] = "";
    versioned_مسیر_درازی++; /* Additional final ‘/’ */
    طالما ((مدخل = readdir(dir)) != عدم) {
        ثابت حرف *name = مدخل->d_name;

        /* Skip special entries */
        إذا (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            استمر;

        إذا (!snprintf_امن(مسیر_buf, حجم(مسیر_buf), "%s/%s", مسیر, name)) {
            err = ENOMEM;
            اذهب err;
        }
        إذا (stat(مسیر_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_نما))
            استمر;

        /* Skip if the corresponding versioned directory is already included */
        لكل (darray_اندازه_t i = 0; i < versioned_شمار; i++) {
            ثابت حرف * ثابت prev_name = darray_قلم(*extensions, i)
                                         + versioned_مسیر_درازی;
            إذا (strcmp(name, prev_name) == 0)
                اذهب بعدی;
        }

        حرف *ext_مسیر = strdup_امن(مسیر_buf);
        إذا (!ext_مسیر) {
            err = ENOMEM;
            اذهب err;
        }

        darray_append(*extensions, ext_مسیر);
بعدی:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    إذا (darray_اندازه(*extensions) > versioned_شمار) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_اقلام(*extensions) + versioned_شمار,
              darray_اندازه(*extensions) - versioned_شمار,
              حجم(*darray_اقلام(*extensions)), &compare_str);
        حرف **ext_مسیر;
        darray_foreach_from(ext_مسیر, *extensions, versioned_شمار) {
            ret |= context_include_مسیر_append(ctx, *ext_مسیر);
        }
    }

    أرجع ret;

err:
    log_dbg(ctx, XKB_LOG_پیام_NO_ID,
            "Include extensions path failed: %s (%s)\n", مسیر, strerror(err));
    إذا (dir)
        closedir(dir);

    أرجع ret;
}
#پایان_اگر

ثابت حرف *
xkb_context_include_مسیر_get_system_مسیر(بنية xkb_context *ctx)
{
    ثابت حرف * ثابت ریشه = xkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    أرجع ریشه ? ریشه : DFLT_XKB_CONFIG_ریشه;
}

/**
 * Append the default include directories to the context.
 */
صحيح
xkb_context_include_مسیر_append_default(بنية xkb_context *ctx)
{
    /*
     * We do not call `xkb_context_init_includes()` here, because either
     * we already initialized the includes paths or we are doing it now.
     */

    حرف *user_مسیر;
    صحيح ret = 0;

    ثابت حرف * ثابت home = xkb_context_getenv(ctx, "HOME");
    ثابت حرف * ثابت xdg = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    إذا (xdg != عدم) {
        user_مسیر = asprintf_امن("%s/xkb", xdg);
        إذا (user_مسیر) {
            ret |= context_include_مسیر_append(ctx, user_مسیر);
            حرر(user_مسیر);
        }
    } وإلا إذا (home != عدم) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        user_مسیر = asprintf_امن("%s/.config/xkb", home);
        إذا (user_مسیر) {
            ret |= context_include_مسیر_append(ctx, user_مسیر);
            حرر(user_مسیر);
        }
    }

    إذا (home != عدم) {
        user_مسیر = asprintf_امن("%s/.xkb", home);
        إذا (user_مسیر) {
            ret |= context_include_مسیر_append(ctx, user_مسیر);
            حرر(user_مسیر);
        }
    }

    ثابت حرف * ثابت extra = xkb_context_include_مسیر_get_extra_مسیر(ctx);
    ret |= context_include_مسیر_append(ctx, extra);

#إذا HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ثابت حرف *extensions_مسیر =
        xkb_context_include_مسیر_get_versioned_extensions_مسیر(ctx);
    اندازه_t versioned_مسیر_درازی = 0;
    إذا (extensions_مسیر) {
        /* Add direct subdirectories */
        ret |= جمع_کن_مستقیم_subdirectories(ctx, extensions_مسیر,
                                         &extensions, 0, 0);
        versioned_مسیر_درازی = طول_رشته(extensions_مسیر);
    }

    /* Unversioned extensions directory */
    extensions_مسیر =
        xkb_context_include_مسیر_get_unversioned_extensions_مسیر(ctx);
    إذا (extensions_مسیر) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= جمع_کن_مستقیم_subdirectories(ctx, extensions_مسیر, &extensions,
                                         darray_اندازه(extensions),
                                         versioned_مسیر_درازی);
    }

    حرف **ext_مسیر;
    darray_foreach(ext_مسیر, extensions) {
        حرر(*ext_مسیر);
    }
    darray_free(extensions);
#پایان_اگر

    /* Canonical XKB root */
    ثابت حرف * ثابت ریشه = xkb_context_include_مسیر_get_system_مسیر(ctx);
    ثابت bool has_ریشه = context_include_مسیر_append(ctx, ریشه);
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
    إذا (!has_ریشه && ریشه[0] != '\0') {
        log_warn(ctx, XKB_LOG_پیام_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_ریشه, ریشه);
        ret |= context_include_مسیر_append(ctx, DFLT_XKB_LEGACY_ریشه);
    }

    أرجع ret;
}

/**
 * Remove all entries in the context's include path.
 */
فراغ
xkb_context_include_مسیر_clear(بنية xkb_context *ctx)
{
    حرف **مسیر;

    darray_foreach(مسیر, ctx->includes)
        حرر(*مسیر);
    darray_free(ctx->includes);

    darray_foreach(مسیر, ctx->failed_includes)
        حرر(*مسیر);
    darray_free(ctx->failed_includes);

    /* It does not make sense to keep the pending defaults */
    ctx->pending_default_includes = false;
}

/**
 * xkb_context_include_path_clear() + xkb_context_include_path_append_default()
 */
صحيح
xkb_context_include_مسیر_reset_defaults(بنية xkb_context *ctx)
{
    xkb_context_include_مسیر_clear(ctx);
    أرجع xkb_context_include_مسیر_append_default(ctx);
}

/**
 * Returns the number of entries in the context's include path.
 */
بیعلامت صحيح
xkb_context_رقم_include_paths(بنية xkb_context *ctx)
{
    أرجع (xkb_context_init_includes(ctx))
        ? darray_اندازه(ctx->includes)
        : 0;
}

/**
 * Returns the given entry in the context's include path, or NULL if an
 * invalid index is passed.
 */
ثابت حرف *
xkb_context_include_مسیر_get(بنية xkb_context *ctx, بیعلامت صحيح idx)
{
    إذا (idx >= xkb_context_رقم_include_paths(ctx))
        أرجع عدم;

    أرجع darray_قلم(ctx->includes, idx);
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
    xkb_context_include_مسیر_clear(ctx);
    atom_جدول_free(ctx->atom_جدول);
    حرر(ctx);
}

ساكن ثابت حرف *
log_level_مقصد_پیشوند(تعداد xkb_log_level level)
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
               ثابت حرف *fmt, va_list آرگومانها)
{
    ثابت حرف *پیشوند = log_level_مقصد_پیشوند(level);

    إذا (پیشوند)
        fprintf(stderr, "%s", پیشوند);
    vfprintf(stderr, fmt, آرگومانها);
}

ساكن تعداد xkb_log_level
log_level(ثابت حرف *level) {
    errno = 0;
    حرف *endptr = عدم;
    ثابت دراز lvl = strtol(level, &endptr, 10);
    إذا (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        أرجع (تعداد xkb_log_level)lvl;
    إذا (istreq_پیشوند("crit", level))
        أرجع XKB_LOG_LEVEL_CRITICAL;
    إذا (istreq_پیشوند("err", level))
        أرجع XKB_LOG_LEVEL_ERROR;
    إذا (istreq_پیشوند("warn", level))
        أرجع XKB_LOG_LEVEL_WARNING;
    إذا (istreq_پیشوند("info", level))
        أرجع XKB_LOG_LEVEL_INFO;
    إذا (istreq_پیشوند("debug", level) || istreq_پیشوند("dbg", level))
        أرجع XKB_LOG_LEVEL_DEBUG;

    أرجع XKB_LOG_LEVEL_ERROR;
}

ساكن صحيح
log_verbosity(ثابت حرف *verbosity) {
    errno = 0;
    ثابت دراز صحيح v = strtol(verbosity, عدم, 10);
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
    بنية xkb_context *ctx = حافظه_صفر_بده(1, حجم(*ctx));

    إذا (!ctx)
        أرجع عدم;

    ctx->refcnt = 1;
    ctx->log_fn = default_log_fn;
    ctx->log_level = XKB_LOG_LEVEL_ERROR;
    ctx->log_verbosity = XKB_LOG_VERBOSITY_DEFAULT;

    ثابت تعداد xkb_context_flags invalid_flags =
        (flags & ~(تعداد xkb_context_flags)XKB_CONTEXT_FLAGS_مقادیر);
    إذا (invalid_flags) {
        log_err_func(ctx, XKB_LOG_پیام_NO_ID,
                     "Invalid context flags: 0x%x\n", invalid_flags);
        حرر(ctx);
        أرجع عدم;
    }

    ctx->use_environment_نامها = !(flags & XKB_CONTEXT_NO_ENVIRONMENT_نامها);
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
        xkb_context_مجموعه_log_level(ctx, log_level(env));

    env = xkb_context_getenv(ctx, "XKB_LOG_VERBOSITY");
    إذا (env)
        xkb_context_مجموعه_log_verbosity(ctx, log_verbosity(env));

    ctx->atom_جدول = atom_جدول_new();
    إذا (!ctx->atom_جدول) {
        xkb_context_unref(ctx);
        أرجع عدم;
    }

    ctx->x11_atom_cache = عدم;

    أرجع ctx;
}

فراغ
xkb_context_مجموعه_log_fn(بنية xkb_context *ctx,
                       فراغ (*log_fn)(بنية xkb_context *ctx,
                                      تعداد xkb_log_level level,
                                      ثابت حرف *fmt, va_list آرگومانها))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

تعداد xkb_log_level
xkb_context_get_log_level(بنية xkb_context *ctx)
{
    أرجع ctx->log_level;
}

فراغ
xkb_context_مجموعه_log_level(بنية xkb_context *ctx, تعداد xkb_log_level level)
{
    ctx->log_level = level;
}

صحيح
xkb_context_get_log_verbosity(بنية xkb_context *ctx)
{
    أرجع ctx->log_verbosity;
}

فراغ
xkb_context_مجموعه_log_verbosity(بنية xkb_context *ctx, صحيح verbosity)
{
    ctx->log_verbosity = verbosity;
}

فراغ *
xkb_context_get_user_داده(بنية xkb_context *ctx)
{
    إذا (ctx)
        أرجع ctx->user_داده;
    أرجع عدم;
}

فراغ
xkb_context_مجموعه_user_داده(بنية xkb_context *ctx, فراغ *user_داده)
{
    ctx->user_داده = user_داده;
}
