/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>

#ਜੇ HAVE_XKB_EXTENSIONS_DIRECTORIES
    #ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>
    #ਜੇ HAVE_UNISTD_H && HAVE_DIRENT_H
        #ਅਨ੍ਤਰ੍ਭਾਵ <dirent.h>
        #ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "context.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "features/enums.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "messages-codes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"


/**
 * Append one directory to the context’s include path.
 */
ਸਥਿਰ ਪੂਰਨ
context_include_ਰਸਤਾ_append(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
    ਪੂਰਨ err = ENOMEM;

    ਅੱਖਰ *tmp = strdup(ਰਸਤਾ);
    ਜੇ (!tmp)
        ਜਾਓ err;

    ਬਣਤਰ stat stat_buf;
    err = stat(ਰਸਤਾ, &stat_buf);
    ਜੇ (err != 0) {
        err = errno;
        ਜਾਓ err;
    }
    ਜੇ (!S_ISDIR(stat_buf.st_ਬਹੁਲਕ)) {
        err = ENOTDIR;
        ਜਾਓ err;
    }

    ਜੇ (!check_eaccess(ਰਸਤਾ, R_OK | X_OK)) {
        err = EACCES;
        ਜਾਓ err;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID, "Include path added: %s\n", tmp);

    ਵਾਪਸ 1;

err:
    ਜੇ (tmp)
        darray_append(ctx->failed_includes, tmp);
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
             "Include path failed: \"%s\" (%s)\n", ਰਸਤਾ, strerror(err));
    ਵਾਪਸ 0;
}

/**
 * Append one directory to the context’s include path.
 */
ਪੂਰਨ
xkb_context_include_ਰਸਤਾ_append(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
    ਵਾਪਸ (xkb_context_init_includes(ctx))
        ? context_include_ਰਸਤਾ_append(ctx, ਰਸਤਾ)
        : 0;
}

ਅਚਲ ਅੱਖਰ *
xkb_context_include_ਰਸਤਾ_get_extra_ਰਸਤਾ(ਬਣਤਰ xkb_context *ctx)
{
    ਅਚਲ ਅੱਖਰ * ਅਚਲ extra = xkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ਵਾਪਸ extra ? extra : DFLT_XKB_CONFIG_EXTRA_ਰਸਤਾ;
}

#ਜੇ HAVE_XKB_EXTENSIONS_DIRECTORIES
ਅਚਲ ਅੱਖਰ *
xkb_context_include_ਰਸਤਾ_get_unversioned_extensions_ਰਸਤਾ(ਬਣਤਰ xkb_context *ctx)
{
    ਅਚਲ ਅੱਖਰ *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
    ਵਾਪਸ ext
        ? ext
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_ਰਸਤਾ
        : DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_ਰਸਤਾ;
#ਨਹੀਂਤਾਂ
        : ਨਿਰਾ;
#ਜੇ_ਅੰਤ
}

ਅਚਲ ਅੱਖਰ *
xkb_context_include_ਰਸਤਾ_get_versioned_extensions_ਰਸਤਾ(ਬਣਤਰ xkb_context *ctx)
{
    ਅਚਲ ਅੱਖਰ *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
    ਵਾਪਸ ext
        ? ext
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_ਰਸਤਾ
        : DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_ਰਸਤਾ;
#ਨਹੀਂਤਾਂ
        : ਨਿਰਾ;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਪੂਰਨ
compare_str(ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b)
{
    ਵਾਪਸ strcmp(*(ਅੱਖਰ **)a, *(ਅੱਖਰ **) b);
}

ਸਥਿਰ ਪੂਰਨ
ਜੋੜੋ_ਸਿੱਧਾ_subdirectories(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
                          darray_string *extensions,
                          darray_ਮਾਪ_t versioned_ਗਿਣਤੀ,
                          ਮਾਪ_t versioned_ਰਸਤਾ_ਲੰਮਾਈ)
{
    ਪੂਰਨ ret = 0;
    ਪੂਰਨ err = ENOMEM;
    DIR *dir = ਨਿਰਾ;

    /* Check extensions parent directory */
    ਬਣਤਰ stat stat_buf;
    err = stat(ਰਸਤਾ, &stat_buf);
    ਜੇ (err != 0) {
        err = errno;
        ਜਾਓ err;
    }
    ਜੇ (!S_ISDIR(stat_buf.st_ਬਹੁਲਕ)) {
        err = ENOTDIR;
        ਜਾਓ err;
    }
    ਜੇ (!check_eaccess(ਰਸਤਾ, R_OK | X_OK)) {
        err = EACCES;
        ਜਾਓ err;
    }

    dir = opendir(ਰਸਤਾ);
    ਜੇ (dir == ਨਿਰਾ) {
        err = EACCES;
        ਜਾਓ err;
    }

    ਬਣਤਰ dirent *ਇੰਦਰਾਜ;
    ਅੱਖਰ ਰਸਤਾ_buf[PATH_MAX] = "";
    versioned_ਰਸਤਾ_ਲੰਮਾਈ++; /* Additional final ‘/’ */
    ਜਦੋਂ ((ਇੰਦਰਾਜ = readdir(dir)) != ਨਿਰਾ) {
        ਅਚਲ ਅੱਖਰ *name = ਇੰਦਰਾਜ->d_name;

        /* Skip special entries */
        ਜੇ (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            ਜਾਰੀ;

        ਜੇ (!snprintf_ਸੁਰੱਖਿਅਤ(ਰਸਤਾ_buf, ਆਕਾਰ(ਰਸਤਾ_buf), "%s/%s", ਰਸਤਾ, name)) {
            err = ENOMEM;
            ਜਾਓ err;
        }
        ਜੇ (stat(ਰਸਤਾ_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_ਬਹੁਲਕ))
            ਜਾਰੀ;

        /* Skip if the corresponding versioned directory is already included */
        ਲਈ (darray_ਮਾਪ_t i = 0; i < versioned_ਗਿਣਤੀ; i++) {
            ਅਚਲ ਅੱਖਰ * ਅਚਲ prev_name = darray_ਵਸਤੂ(*extensions, i)
                                         + versioned_ਰਸਤਾ_ਲੰਮਾਈ;
            ਜੇ (strcmp(name, prev_name) == 0)
                ਜਾਓ ਅਗਲਾ;
        }

        ਅੱਖਰ *ext_ਰਸਤਾ = strdup_ਸੁਰੱਖਿਅਤ(ਰਸਤਾ_buf);
        ਜੇ (!ext_ਰਸਤਾ) {
            err = ENOMEM;
            ਜਾਓ err;
        }

        darray_append(*extensions, ext_ਰਸਤਾ);
ਅਗਲਾ:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    ਜੇ (darray_ਮਾਪ(*extensions) > versioned_ਗਿਣਤੀ) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_ਚੀਜ਼ਾਂ(*extensions) + versioned_ਗਿਣਤੀ,
              darray_ਮਾਪ(*extensions) - versioned_ਗਿਣਤੀ,
              ਆਕਾਰ(*darray_ਚੀਜ਼ਾਂ(*extensions)), &compare_str);
        ਅੱਖਰ **ext_ਰਸਤਾ;
        darray_foreach_from(ext_ਰਸਤਾ, *extensions, versioned_ਗਿਣਤੀ) {
            ret |= context_include_ਰਸਤਾ_append(ctx, *ext_ਰਸਤਾ);
        }
    }

    ਵਾਪਸ ret;

err:
    log_dbg(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
            "Include extensions path failed: %s (%s)\n", ਰਸਤਾ, strerror(err));
    ਜੇ (dir)
        closedir(dir);

    ਵਾਪਸ ret;
}
#ਜੇ_ਅੰਤ

ਅਚਲ ਅੱਖਰ *
xkb_context_include_ਰਸਤਾ_get_system_ਰਸਤਾ(ਬਣਤਰ xkb_context *ctx)
{
    ਅਚਲ ਅੱਖਰ * ਅਚਲ ਜੜ੍ਹ = xkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ਵਾਪਸ ਜੜ੍ਹ ? ਜੜ੍ਹ : DFLT_XKB_CONFIG_ਜੜ੍ਹ;
}

/**
 * Append the default include directories to the context.
 */
ਪੂਰਨ
xkb_context_include_ਰਸਤਾ_append_default(ਬਣਤਰ xkb_context *ctx)
{
    /*
     * We do not call `xkb_context_init_includes()` here, because either
     * we already initialized the includes paths or we are doing it now.
     */

    ਅੱਖਰ *user_ਰਸਤਾ;
    ਪੂਰਨ ret = 0;

    ਅਚਲ ਅੱਖਰ * ਅਚਲ home = xkb_context_getenv(ctx, "HOME");
    ਅਚਲ ਅੱਖਰ * ਅਚਲ xdg = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    ਜੇ (xdg != ਨਿਰਾ) {
        user_ਰਸਤਾ = asprintf_ਸੁਰੱਖਿਅਤ("%s/xkb", xdg);
        ਜੇ (user_ਰਸਤਾ) {
            ret |= context_include_ਰਸਤਾ_append(ctx, user_ਰਸਤਾ);
            ਮੁਕਤ(user_ਰਸਤਾ);
        }
    } ਨਹੀਂਤਾਂ ਜੇ (home != ਨਿਰਾ) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        user_ਰਸਤਾ = asprintf_ਸੁਰੱਖਿਅਤ("%s/.config/xkb", home);
        ਜੇ (user_ਰਸਤਾ) {
            ret |= context_include_ਰਸਤਾ_append(ctx, user_ਰਸਤਾ);
            ਮੁਕਤ(user_ਰਸਤਾ);
        }
    }

    ਜੇ (home != ਨਿਰਾ) {
        user_ਰਸਤਾ = asprintf_ਸੁਰੱਖਿਅਤ("%s/.xkb", home);
        ਜੇ (user_ਰਸਤਾ) {
            ret |= context_include_ਰਸਤਾ_append(ctx, user_ਰਸਤਾ);
            ਮੁਕਤ(user_ਰਸਤਾ);
        }
    }

    ਅਚਲ ਅੱਖਰ * ਅਚਲ extra = xkb_context_include_ਰਸਤਾ_get_extra_ਰਸਤਾ(ctx);
    ret |= context_include_ਰਸਤਾ_append(ctx, extra);

#ਜੇ HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ਅਚਲ ਅੱਖਰ *extensions_ਰਸਤਾ =
        xkb_context_include_ਰਸਤਾ_get_versioned_extensions_ਰਸਤਾ(ctx);
    ਮਾਪ_t versioned_ਰਸਤਾ_ਲੰਮਾਈ = 0;
    ਜੇ (extensions_ਰਸਤਾ) {
        /* Add direct subdirectories */
        ret |= ਜੋੜੋ_ਸਿੱਧਾ_subdirectories(ctx, extensions_ਰਸਤਾ,
                                         &extensions, 0, 0);
        versioned_ਰਸਤਾ_ਲੰਮਾਈ = ਲੜੀ_ਲੰਬਾਈ(extensions_ਰਸਤਾ);
    }

    /* Unversioned extensions directory */
    extensions_ਰਸਤਾ =
        xkb_context_include_ਰਸਤਾ_get_unversioned_extensions_ਰਸਤਾ(ctx);
    ਜੇ (extensions_ਰਸਤਾ) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= ਜੋੜੋ_ਸਿੱਧਾ_subdirectories(ctx, extensions_ਰਸਤਾ, &extensions,
                                         darray_ਮਾਪ(extensions),
                                         versioned_ਰਸਤਾ_ਲੰਮਾਈ);
    }

    ਅੱਖਰ **ext_ਰਸਤਾ;
    darray_foreach(ext_ਰਸਤਾ, extensions) {
        ਮੁਕਤ(*ext_ਰਸਤਾ);
    }
    darray_free(extensions);
#ਜੇ_ਅੰਤ

    /* Canonical XKB root */
    ਅਚਲ ਅੱਖਰ * ਅਚਲ ਜੜ੍ਹ = xkb_context_include_ਰਸਤਾ_get_system_ਰਸਤਾ(ctx);
    ਅਚਲ bool has_ਜੜ੍ਹ = context_include_ਰਸਤਾ_append(ctx, ਜੜ੍ਹ);
    ret |= has_ਜੜ੍ਹ;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    ਜੇ (!has_ਜੜ੍ਹ && ਜੜ੍ਹ[0] != '\0') {
        log_warn(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_ਜੜ੍ਹ, ਜੜ੍ਹ);
        ret |= context_include_ਰਸਤਾ_append(ctx, DFLT_XKB_LEGACY_ਜੜ੍ਹ);
    }

    ਵਾਪਸ ret;
}

/**
 * Remove all entries in the context's include path.
 */
ਖਾਲੀ
xkb_context_include_ਰਸਤਾ_clear(ਬਣਤਰ xkb_context *ctx)
{
    ਅੱਖਰ **ਰਸਤਾ;

    darray_foreach(ਰਸਤਾ, ctx->includes)
        ਮੁਕਤ(*ਰਸਤਾ);
    darray_free(ctx->includes);

    darray_foreach(ਰਸਤਾ, ctx->failed_includes)
        ਮੁਕਤ(*ਰਸਤਾ);
    darray_free(ctx->failed_includes);

    /* It does not make sense to keep the pending defaults */
    ctx->pending_default_includes = false;
}

/**
 * xkb_context_include_path_clear() + xkb_context_include_path_append_default()
 */
ਪੂਰਨ
xkb_context_include_ਰਸਤਾ_reset_defaults(ਬਣਤਰ xkb_context *ctx)
{
    xkb_context_include_ਰਸਤਾ_clear(ctx);
    ਵਾਪਸ xkb_context_include_ਰਸਤਾ_append_default(ctx);
}

/**
 * Returns the number of entries in the context's include path.
 */
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
xkb_context_ਅੰਕ_include_paths(ਬਣਤਰ xkb_context *ctx)
{
    ਵਾਪਸ (xkb_context_init_includes(ctx))
        ? darray_ਮਾਪ(ctx->includes)
        : 0;
}

/**
 * Returns the given entry in the context's include path, or NULL if an
 * invalid index is passed.
 */
ਅਚਲ ਅੱਖਰ *
xkb_context_include_ਰਸਤਾ_get(ਬਣਤਰ xkb_context *ctx, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ idx)
{
    ਜੇ (idx >= xkb_context_ਅੰਕ_include_paths(ctx))
        ਵਾਪਸ ਨਿਰਾ;

    ਵਾਪਸ darray_ਵਸਤੂ(ctx->includes, idx);
}

/**
 * Take a new reference on the context.
 */
ਬਣਤਰ xkb_context *
xkb_context_ref(ਬਣਤਰ xkb_context *ctx)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(ctx->refcnt > 0);
    ctx->refcnt++;
    ਵਾਪਸ ctx;
}

/**
 * Drop an existing reference on the context, and free it if the refcnt is
 * now 0.
 */
ਖਾਲੀ
xkb_context_unref(ਬਣਤਰ xkb_context *ctx)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(!ctx || ctx->refcnt > 0);
    ਜੇ (!ctx || --ctx->refcnt > 0)
        ਵਾਪਸ;

    ਮੁਕਤ(ctx->x11_atom_cache);
    xkb_context_include_ਰਸਤਾ_clear(ctx);
    atom_ਸਾਰਣੀ_free(ctx->atom_ਸਾਰਣੀ);
    ਮੁਕਤ(ctx);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
log_level_ਮੰਜ਼ਿਲ_ਅਗੇਤਰ(ਗਿਣਤੀ xkb_log_level level)
{
    ਚੋਣ (level) {
    ਹਾਲਤ XKB_LOG_LEVEL_DEBUG:
        ਵਾਪਸ "xkbcommon: DEBUG: ";
    ਹਾਲਤ XKB_LOG_LEVEL_INFO:
        ਵਾਪਸ "xkbcommon: INFO: ";
    ਹਾਲਤ XKB_LOG_LEVEL_WARNING:
        ਵਾਪਸ "xkbcommon: WARNING: ";
    ਹਾਲਤ XKB_LOG_LEVEL_ERROR:
        ਵਾਪਸ "xkbcommon: ERROR: ";
    ਹਾਲਤ XKB_LOG_LEVEL_CRITICAL:
        ਵਾਪਸ "xkbcommon: CRITICAL: ";
    ਮੂਲ:
        ਵਾਪਸ ਨਿਰਾ;
    }
}

ATTR_PRINTF(3, 0) ਸਥਿਰ ਖਾਲੀ
default_log_fn(ਬਣਤਰ xkb_context *ctx, ਗਿਣਤੀ xkb_log_level level,
               ਅਚਲ ਅੱਖਰ *fmt, va_list ਦਲੀਲਾਂ)
{
    ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ = log_level_ਮੰਜ਼ਿਲ_ਅਗੇਤਰ(level);

    ਜੇ (ਅਗੇਤਰ)
        fprintf(stderr, "%s", ਅਗੇਤਰ);
    vfprintf(stderr, fmt, ਦਲੀਲਾਂ);
}

ਸਥਿਰ ਗਿਣਤੀ xkb_log_level
log_level(ਅਚਲ ਅੱਖਰ *level) {
    errno = 0;
    ਅੱਖਰ *endptr = ਨਿਰਾ;
    ਅਚਲ ਲੰਮਾ lvl = strtol(level, &endptr, 10);
    ਜੇ (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        ਵਾਪਸ (ਗਿਣਤੀ xkb_log_level)lvl;
    ਜੇ (istreq_ਅਗੇਤਰ("crit", level))
        ਵਾਪਸ XKB_LOG_LEVEL_CRITICAL;
    ਜੇ (istreq_ਅਗੇਤਰ("err", level))
        ਵਾਪਸ XKB_LOG_LEVEL_ERROR;
    ਜੇ (istreq_ਅਗੇਤਰ("warn", level))
        ਵਾਪਸ XKB_LOG_LEVEL_WARNING;
    ਜੇ (istreq_ਅਗੇਤਰ("info", level))
        ਵਾਪਸ XKB_LOG_LEVEL_INFO;
    ਜੇ (istreq_ਅਗੇਤਰ("debug", level) || istreq_ਅਗੇਤਰ("dbg", level))
        ਵਾਪਸ XKB_LOG_LEVEL_DEBUG;

    ਵਾਪਸ XKB_LOG_LEVEL_ERROR;
}

ਸਥਿਰ ਪੂਰਨ
log_verbosity(ਅਚਲ ਅੱਖਰ *verbosity) {
    errno = 0;
    ਅਚਲ ਲੰਮਾ ਪੂਰਨ v = strtol(verbosity, ਨਿਰਾ, 10);
    ਜੇ (errno == 0) {
        /* Truncate */
        ਵਾਪਸ (ਪੂਰਨ) v;
    }

    ਵਾਪਸ XKB_LOG_VERBOSITY_DEFAULT;
}

/**
 * Create a new context.
 */
ਬਣਤਰ xkb_context *
xkb_context_new(ਗਿਣਤੀ xkb_context_flags flags)
{
    ਅਚਲ ਅੱਖਰ *env;
    ਬਣਤਰ xkb_context *ctx = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(*ctx));

    ਜੇ (!ctx)
        ਵਾਪਸ ਨਿਰਾ;

    ctx->refcnt = 1;
    ctx->log_fn = default_log_fn;
    ctx->log_level = XKB_LOG_LEVEL_ERROR;
    ctx->log_verbosity = XKB_LOG_VERBOSITY_DEFAULT;

    ਅਚਲ ਗਿਣਤੀ xkb_context_flags invalid_flags =
        (flags & ~(ਗਿਣਤੀ xkb_context_flags)XKB_CONTEXT_FLAGS_ਮੁੱਲਾਂ);
    ਜੇ (invalid_flags) {
        log_err_func(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                     "Invalid context flags: 0x%x\n", invalid_flags);
        ਮੁਕਤ(ctx);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ctx->use_environment_ਨਾਂ = !(flags & XKB_CONTEXT_NO_ENVIRONMENT_ਨਾਂ);
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
    ਜੇ (env)
        xkb_context_ਸੈੱਟ_log_level(ctx, log_level(env));

    env = xkb_context_getenv(ctx, "XKB_LOG_VERBOSITY");
    ਜੇ (env)
        xkb_context_ਸੈੱਟ_log_verbosity(ctx, log_verbosity(env));

    ctx->atom_ਸਾਰਣੀ = atom_ਸਾਰਣੀ_new();
    ਜੇ (!ctx->atom_ਸਾਰਣੀ) {
        xkb_context_unref(ctx);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ctx->x11_atom_cache = ਨਿਰਾ;

    ਵਾਪਸ ctx;
}

ਖਾਲੀ
xkb_context_ਸੈੱਟ_log_fn(ਬਣਤਰ xkb_context *ctx,
                       ਖਾਲੀ (*log_fn)(ਬਣਤਰ xkb_context *ctx,
                                      ਗਿਣਤੀ xkb_log_level level,
                                      ਅਚਲ ਅੱਖਰ *fmt, va_list ਦਲੀਲਾਂ))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

ਗਿਣਤੀ xkb_log_level
xkb_context_get_log_level(ਬਣਤਰ xkb_context *ctx)
{
    ਵਾਪਸ ctx->log_level;
}

ਖਾਲੀ
xkb_context_ਸੈੱਟ_log_level(ਬਣਤਰ xkb_context *ctx, ਗਿਣਤੀ xkb_log_level level)
{
    ctx->log_level = level;
}

ਪੂਰਨ
xkb_context_get_log_verbosity(ਬਣਤਰ xkb_context *ctx)
{
    ਵਾਪਸ ctx->log_verbosity;
}

ਖਾਲੀ
xkb_context_ਸੈੱਟ_log_verbosity(ਬਣਤਰ xkb_context *ctx, ਪੂਰਨ verbosity)
{
    ctx->log_verbosity = verbosity;
}

ਖਾਲੀ *
xkb_context_get_user_ਅੰਕੜੇ(ਬਣਤਰ xkb_context *ctx)
{
    ਜੇ (ctx)
        ਵਾਪਸ ctx->user_ਅੰਕੜੇ;
    ਵਾਪਸ ਨਿਰਾ;
}

ਖਾਲੀ
xkb_context_ਸੈੱਟ_user_ਅੰਕੜੇ(ਬਣਤਰ xkb_context *ctx, ਖਾਲੀ *user_ਅੰਕੜੇ)
{
    ctx->user_ਅੰਕੜੇ = user_ਅੰਕੜੇ;
}
