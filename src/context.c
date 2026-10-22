/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>

#ಆದರೆ HAVE_XKB_EXTENSIONS_DIRECTORIES
    #ಅನ್ತರ್ಭಾವ <limits.h>
    #ಆದರೆ HAVE_UNISTD_H && HAVE_DIRENT_H
        #ಅನ್ತರ್ಭಾವ <dirent.h>
        #ಅನ್ತರ್ಭಾವ <unistd.h>
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "context.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "features/enums.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "utils.h"


/**
 * Append one directory to the context’s include path.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
context_include_ಮಾರ್ಗ_append(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
    ಪೂರ್ಣಾಂಕ err = ENOMEM;

    ಅಕ್ಷರ *tmp = strdup(ಮಾರ್ಗ);
    ಆದರೆ (!tmp)
        ಹೋಗು err;

    ರಚನೆ stat stat_buf;
    err = stat(ಮಾರ್ಗ, &stat_buf);
    ಆದರೆ (err != 0) {
        err = errno;
        ಹೋಗು err;
    }
    ಆದರೆ (!S_ISDIR(stat_buf.st_ಬಹುಲಕ)) {
        err = ENOTDIR;
        ಹೋಗು err;
    }

    ಆದರೆ (!check_eaccess(ಮಾರ್ಗ, R_OK | X_OK)) {
        err = EACCES;
        ಹೋಗು err;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID, "Include path added: %s\n", tmp);

    ಹಿಂತಿರುಗಿ 1;

err:
    ಆದರೆ (tmp)
        darray_append(ctx->failed_includes, tmp);
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
             "Include path failed: \"%s\" (%s)\n", ಮಾರ್ಗ, strerror(err));
    ಹಿಂತಿರುಗಿ 0;
}

/**
 * Append one directory to the context’s include path.
 */
ಪೂರ್ಣಾಂಕ
xkb_context_include_ಮಾರ್ಗ_append(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
    ಹಿಂತಿರುಗಿ (xkb_context_init_includes(ctx))
        ? context_include_ಮಾರ್ಗ_append(ctx, ಮಾರ್ಗ)
        : 0;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_context_include_ಮಾರ್ಗ_get_extra_ಮಾರ್ಗ(ರಚನೆ xkb_context *ctx)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ extra = xkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ಹಿಂತಿರುಗಿ extra ? extra : DFLT_XKB_CONFIG_EXTRA_ಮಾರ್ಗ;
}

#ಆದರೆ HAVE_XKB_EXTENSIONS_DIRECTORIES
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_context_include_ಮಾರ್ಗ_get_unversioned_extensions_ಮಾರ್ಗ(ರಚನೆ xkb_context *ctx)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
    ಹಿಂತಿರುಗಿ ext
        ? ext
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_ಮಾರ್ಗ
        : DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_ಮಾರ್ಗ;
#ಇಲ್ಲದಿದ್ದರೆ
        : ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_context_include_ಮಾರ್ಗ_get_versioned_extensions_ಮಾರ್ಗ(ರಚನೆ xkb_context *ctx)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
    ಹಿಂತಿರುಗಿ ext
        ? ext
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_ಮಾರ್ಗ
        : DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_ಮಾರ್ಗ;
#ಇಲ್ಲದಿದ್ದರೆ
        : ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
compare_str(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b)
{
    ಹಿಂತಿರುಗಿ strcmp(*(ಅಕ್ಷರ **)a, *(ಅಕ್ಷರ **) b);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಕೂಡಿಸು_ನೇರ_subdirectories(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ,
                          darray_string *extensions,
                          darray_ಗಾತ್ರ_t versioned_ಎಣಿಕೆ,
                          ಗಾತ್ರ_t versioned_ಮಾರ್ಗ_ಉದ್ದಳತೆ)
{
    ಪೂರ್ಣಾಂಕ ret = 0;
    ಪೂರ್ಣಾಂಕ err = ENOMEM;
    DIR *dir = ಖಾಲಿ;

    /* Check extensions parent directory */
    ರಚನೆ stat stat_buf;
    err = stat(ಮಾರ್ಗ, &stat_buf);
    ಆದರೆ (err != 0) {
        err = errno;
        ಹೋಗು err;
    }
    ಆದರೆ (!S_ISDIR(stat_buf.st_ಬಹುಲಕ)) {
        err = ENOTDIR;
        ಹೋಗು err;
    }
    ಆದರೆ (!check_eaccess(ಮಾರ್ಗ, R_OK | X_OK)) {
        err = EACCES;
        ಹೋಗು err;
    }

    dir = opendir(ಮಾರ್ಗ);
    ಆದರೆ (dir == ಖಾಲಿ) {
        err = EACCES;
        ಹೋಗು err;
    }

    ರಚನೆ dirent *ನಮೂದು;
    ಅಕ್ಷರ ಮಾರ್ಗ_buf[PATH_MAX] = "";
    versioned_ಮಾರ್ಗ_ಉದ್ದಳತೆ++; /* Additional final ‘/’ */
    ಆಗ ((ನಮೂದು = readdir(dir)) != ಖಾಲಿ) {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = ನಮೂದು->d_name;

        /* Skip special entries */
        ಆದರೆ (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            ಮುಂದುವರಿ;

        ಆದರೆ (!snprintf_ಸುರಕ್ಷಿತ(ಮಾರ್ಗ_buf, ಗಾತ್ರ(ಮಾರ್ಗ_buf), "%s/%s", ಮಾರ್ಗ, name)) {
            err = ENOMEM;
            ಹೋಗು err;
        }
        ಆದರೆ (stat(ಮಾರ್ಗ_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_ಬಹುಲಕ))
            ಮುಂದುವರಿ;

        /* Skip if the corresponding versioned directory is already included */
        ಗಾಗಿ (darray_ಗಾತ್ರ_t i = 0; i < versioned_ಎಣಿಕೆ; i++) {
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ prev_name = darray_ಅಂಶ(*extensions, i)
                                         + versioned_ಮಾರ್ಗ_ಉದ್ದಳತೆ;
            ಆದರೆ (strcmp(name, prev_name) == 0)
                ಹೋಗು ಮುಂದಿನ;
        }

        ಅಕ್ಷರ *ext_ಮಾರ್ಗ = strdup_ಸುರಕ್ಷಿತ(ಮಾರ್ಗ_buf);
        ಆದರೆ (!ext_ಮಾರ್ಗ) {
            err = ENOMEM;
            ಹೋಗು err;
        }

        darray_append(*extensions, ext_ಮಾರ್ಗ);
ಮುಂದಿನ:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    ಆದರೆ (darray_ಗಾತ್ರ(*extensions) > versioned_ಎಣಿಕೆ) {
        /* Sort the entries, so that they are appended in lexicographic order */
        qsort(darray_ವಸ್ತುಗಳು(*extensions) + versioned_ಎಣಿಕೆ,
              darray_ಗಾತ್ರ(*extensions) - versioned_ಎಣಿಕೆ,
              ಗಾತ್ರ(*darray_ವಸ್ತುಗಳು(*extensions)), &compare_str);
        ಅಕ್ಷರ **ext_ಮಾರ್ಗ;
        darray_foreach_from(ext_ಮಾರ್ಗ, *extensions, versioned_ಎಣಿಕೆ) {
            ret |= context_include_ಮಾರ್ಗ_append(ctx, *ext_ಮಾರ್ಗ);
        }
    }

    ಹಿಂತಿರುಗಿ ret;

err:
    log_dbg(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
            "Include extensions path failed: %s (%s)\n", ಮಾರ್ಗ, strerror(err));
    ಆದರೆ (dir)
        closedir(dir);

    ಹಿಂತಿರುಗಿ ret;
}
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_context_include_ಮಾರ್ಗ_get_system_ಮಾರ್ಗ(ರಚನೆ xkb_context *ctx)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ ಬೇರು = xkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    ಹಿಂತಿರುಗಿ ಬೇರು ? ಬೇರು : DFLT_XKB_CONFIG_ಬೇರು;
}

/**
 * Append the default include directories to the context.
 */
ಪೂರ್ಣಾಂಕ
xkb_context_include_ಮಾರ್ಗ_append_default(ರಚನೆ xkb_context *ctx)
{
    /*
     * We do not call `xkb_context_init_includes()` here, because either
     * we already initialized the includes paths or we are doing it now.
     */

    ಅಕ್ಷರ *user_ಮಾರ್ಗ;
    ಪೂರ್ಣಾಂಕ ret = 0;

    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ home = xkb_context_getenv(ctx, "HOME");
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ xdg = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    ಆದರೆ (xdg != ಖಾಲಿ) {
        user_ಮಾರ್ಗ = asprintf_ಸುರಕ್ಷಿತ("%s/xkb", xdg);
        ಆದರೆ (user_ಮಾರ್ಗ) {
            ret |= context_include_ಮಾರ್ಗ_append(ctx, user_ಮಾರ್ಗ);
            ಬಿಡುಗಡೆ(user_ಮಾರ್ಗ);
        }
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (home != ಖಾಲಿ) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        user_ಮಾರ್ಗ = asprintf_ಸುರಕ್ಷಿತ("%s/.config/xkb", home);
        ಆದರೆ (user_ಮಾರ್ಗ) {
            ret |= context_include_ಮಾರ್ಗ_append(ctx, user_ಮಾರ್ಗ);
            ಬಿಡುಗಡೆ(user_ಮಾರ್ಗ);
        }
    }

    ಆದರೆ (home != ಖಾಲಿ) {
        user_ಮಾರ್ಗ = asprintf_ಸುರಕ್ಷಿತ("%s/.xkb", home);
        ಆದರೆ (user_ಮಾರ್ಗ) {
            ret |= context_include_ಮಾರ್ಗ_append(ctx, user_ಮಾರ್ಗ);
            ಬಿಡುಗಡೆ(user_ಮಾರ್ಗ);
        }
    }

    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ extra = xkb_context_include_ಮಾರ್ಗ_get_extra_ಮಾರ್ಗ(ctx);
    ret |= context_include_ಮಾರ್ಗ_append(ctx, extra);

#ಆದರೆ HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *extensions_ಮಾರ್ಗ =
        xkb_context_include_ಮಾರ್ಗ_get_versioned_extensions_ಮಾರ್ಗ(ctx);
    ಗಾತ್ರ_t versioned_ಮಾರ್ಗ_ಉದ್ದಳತೆ = 0;
    ಆದರೆ (extensions_ಮಾರ್ಗ) {
        /* Add direct subdirectories */
        ret |= ಕೂಡಿಸು_ನೇರ_subdirectories(ctx, extensions_ಮಾರ್ಗ,
                                         &extensions, 0, 0);
        versioned_ಮಾರ್ಗ_ಉದ್ದಳತೆ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(extensions_ಮಾರ್ಗ);
    }

    /* Unversioned extensions directory */
    extensions_ಮಾರ್ಗ =
        xkb_context_include_ಮಾರ್ಗ_get_unversioned_extensions_ಮಾರ್ಗ(ctx);
    ಆದರೆ (extensions_ಮಾರ್ಗ) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= ಕೂಡಿಸು_ನೇರ_subdirectories(ctx, extensions_ಮಾರ್ಗ, &extensions,
                                         darray_ಗಾತ್ರ(extensions),
                                         versioned_ಮಾರ್ಗ_ಉದ್ದಳತೆ);
    }

    ಅಕ್ಷರ **ext_ಮಾರ್ಗ;
    darray_foreach(ext_ಮಾರ್ಗ, extensions) {
        ಬಿಡುಗಡೆ(*ext_ಮಾರ್ಗ);
    }
    darray_free(extensions);
#ಆದರೆ_ಅಂತ್ಯ

    /* Canonical XKB root */
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ ಬೇರು = xkb_context_include_ಮಾರ್ಗ_get_system_ಮಾರ್ಗ(ctx);
    ಸ್ಥಿರಾಂಕ bool has_ಬೇರು = context_include_ಮಾರ್ಗ_append(ctx, ಬೇರು);
    ret |= has_ಬೇರು;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    ಆದರೆ (!has_ಬೇರು && ಬೇರು[0] != '\0') {
        log_warn(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_ಬೇರು, ಬೇರು);
        ret |= context_include_ಮಾರ್ಗ_append(ctx, DFLT_XKB_LEGACY_ಬೇರು);
    }

    ಹಿಂತಿರುಗಿ ret;
}

/**
 * Remove all entries in the context's include path.
 */
ಶೂನ್ಯ
xkb_context_include_ಮಾರ್ಗ_clear(ರಚನೆ xkb_context *ctx)
{
    ಅಕ್ಷರ **ಮಾರ್ಗ;

    darray_foreach(ಮಾರ್ಗ, ctx->includes)
        ಬಿಡುಗಡೆ(*ಮಾರ್ಗ);
    darray_free(ctx->includes);

    darray_foreach(ಮಾರ್ಗ, ctx->failed_includes)
        ಬಿಡುಗಡೆ(*ಮಾರ್ಗ);
    darray_free(ctx->failed_includes);

    /* It does not make sense to keep the pending defaults */
    ctx->pending_default_includes = false;
}

/**
 * xkb_context_include_path_clear() + xkb_context_include_path_append_default()
 */
ಪೂರ್ಣಾಂಕ
xkb_context_include_ಮಾರ್ಗ_reset_defaults(ರಚನೆ xkb_context *ctx)
{
    xkb_context_include_ಮಾರ್ಗ_clear(ctx);
    ಹಿಂತಿರುಗಿ xkb_context_include_ಮಾರ್ಗ_append_default(ctx);
}

/**
 * Returns the number of entries in the context's include path.
 */
ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ
xkb_context_ಅಂಕೆ_include_paths(ರಚನೆ xkb_context *ctx)
{
    ಹಿಂತಿರುಗಿ (xkb_context_init_includes(ctx))
        ? darray_ಗಾತ್ರ(ctx->includes)
        : 0;
}

/**
 * Returns the given entry in the context's include path, or NULL if an
 * invalid index is passed.
 */
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
xkb_context_include_ಮಾರ್ಗ_get(ರಚನೆ xkb_context *ctx, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ idx)
{
    ಆದರೆ (idx >= xkb_context_ಅಂಕೆ_include_paths(ctx))
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ darray_ಅಂಶ(ctx->includes, idx);
}

/**
 * Take a new reference on the context.
 */
ರಚನೆ xkb_context *
xkb_context_ref(ರಚನೆ xkb_context *ctx)
{
    ಪ್ರತಿಜ್ಞಾ(ctx->refcnt > 0);
    ctx->refcnt++;
    ಹಿಂತಿರುಗಿ ctx;
}

/**
 * Drop an existing reference on the context, and free it if the refcnt is
 * now 0.
 */
ಶೂನ್ಯ
xkb_context_unref(ರಚನೆ xkb_context *ctx)
{
    ಪ್ರತಿಜ್ಞಾ(!ctx || ctx->refcnt > 0);
    ಆದರೆ (!ctx || --ctx->refcnt > 0)
        ಹಿಂತಿರುಗಿ;

    ಬಿಡುಗಡೆ(ctx->x11_atom_cache);
    xkb_context_include_ಮಾರ್ಗ_clear(ctx);
    atom_ಕೋಷ್ಟಕ_free(ctx->atom_ಕೋಷ್ಟಕ);
    ಬಿಡುಗಡೆ(ctx);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
log_level_ಗಮ್ಯ_ಪೂರ್ವಪ್ರತ್ಯಯ(ಎಣಿಕೆ xkb_log_level level)
{
    ಆಯ್ಕೆ (level) {
    ಸ್ಥಿತಿ XKB_LOG_LEVEL_DEBUG:
        ಹಿಂತಿರುಗಿ "xkbcommon: DEBUG: ";
    ಸ್ಥಿತಿ XKB_LOG_LEVEL_INFO:
        ಹಿಂತಿರುಗಿ "xkbcommon: INFO: ";
    ಸ್ಥಿತಿ XKB_LOG_LEVEL_WARNING:
        ಹಿಂತಿರುಗಿ "xkbcommon: WARNING: ";
    ಸ್ಥಿತಿ XKB_LOG_LEVEL_ERROR:
        ಹಿಂತಿರುಗಿ "xkbcommon: ERROR: ";
    ಸ್ಥಿತಿ XKB_LOG_LEVEL_CRITICAL:
        ಹಿಂತಿರುಗಿ "xkbcommon: CRITICAL: ";
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }
}

ATTR_PRINTF(3, 0) ಸ್ಥಿರ ಶೂನ್ಯ
default_log_fn(ರಚನೆ xkb_context *ctx, ಎಣಿಕೆ xkb_log_level level,
               ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, va_list ವಾದಗಳು)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ = log_level_ಗಮ್ಯ_ಪೂರ್ವಪ್ರತ್ಯಯ(level);

    ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ)
        fprintf(stderr, "%s", ಪೂರ್ವಪ್ರತ್ಯಯ);
    vfprintf(stderr, fmt, ವಾದಗಳು);
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_log_level
log_level(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *level) {
    errno = 0;
    ಅಕ್ಷರ *endptr = ಖಾಲಿ;
    ಸ್ಥಿರಾಂಕ ದೀರ್ಘ lvl = strtol(level, &endptr, 10);
    ಆದರೆ (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        ಹಿಂತಿರುಗಿ (ಎಣಿಕೆ xkb_log_level)lvl;
    ಆದರೆ (istreq_ಪೂರ್ವಪ್ರತ್ಯಯ("crit", level))
        ಹಿಂತಿರುಗಿ XKB_LOG_LEVEL_CRITICAL;
    ಆದರೆ (istreq_ಪೂರ್ವಪ್ರತ್ಯಯ("err", level))
        ಹಿಂತಿರುಗಿ XKB_LOG_LEVEL_ERROR;
    ಆದರೆ (istreq_ಪೂರ್ವಪ್ರತ್ಯಯ("warn", level))
        ಹಿಂತಿರುಗಿ XKB_LOG_LEVEL_WARNING;
    ಆದರೆ (istreq_ಪೂರ್ವಪ್ರತ್ಯಯ("info", level))
        ಹಿಂತಿರುಗಿ XKB_LOG_LEVEL_INFO;
    ಆದರೆ (istreq_ಪೂರ್ವಪ್ರತ್ಯಯ("debug", level) || istreq_ಪೂರ್ವಪ್ರತ್ಯಯ("dbg", level))
        ಹಿಂತಿರುಗಿ XKB_LOG_LEVEL_DEBUG;

    ಹಿಂತಿರುಗಿ XKB_LOG_LEVEL_ERROR;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
log_verbosity(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *verbosity) {
    errno = 0;
    ಸ್ಥಿರಾಂಕ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ v = strtol(verbosity, ಖಾಲಿ, 10);
    ಆದರೆ (errno == 0) {
        /* Truncate */
        ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) v;
    }

    ಹಿಂತಿರುಗಿ XKB_LOG_VERBOSITY_DEFAULT;
}

/**
 * Create a new context.
 */
ರಚನೆ xkb_context *
xkb_context_new(ಎಣಿಕೆ xkb_context_flags flags)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *env;
    ರಚನೆ xkb_context *ctx = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(*ctx));

    ಆದರೆ (!ctx)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ctx->refcnt = 1;
    ctx->log_fn = default_log_fn;
    ctx->log_level = XKB_LOG_LEVEL_ERROR;
    ctx->log_verbosity = XKB_LOG_VERBOSITY_DEFAULT;

    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_context_flags invalid_flags =
        (flags & ~(ಎಣಿಕೆ xkb_context_flags)XKB_CONTEXT_FLAGS_ಮೌಲ್ಯಗಳು);
    ಆದರೆ (invalid_flags) {
        log_err_func(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                     "Invalid context flags: 0x%x\n", invalid_flags);
        ಬಿಡುಗಡೆ(ctx);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ctx->use_environment_ಹೆಸರುಗಳು = !(flags & XKB_CONTEXT_NO_ENVIRONMENT_ಹೆಸರುಗಳು);
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
    ಆದರೆ (env)
        xkb_context_ಗಣ_log_level(ctx, log_level(env));

    env = xkb_context_getenv(ctx, "XKB_LOG_VERBOSITY");
    ಆದರೆ (env)
        xkb_context_ಗಣ_log_verbosity(ctx, log_verbosity(env));

    ctx->atom_ಕೋಷ್ಟಕ = atom_ಕೋಷ್ಟಕ_new();
    ಆದರೆ (!ctx->atom_ಕೋಷ್ಟಕ) {
        xkb_context_unref(ctx);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ctx->x11_atom_cache = ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ ctx;
}

ಶೂನ್ಯ
xkb_context_ಗಣ_log_fn(ರಚನೆ xkb_context *ctx,
                       ಶೂನ್ಯ (*log_fn)(ರಚನೆ xkb_context *ctx,
                                      ಎಣಿಕೆ xkb_log_level level,
                                      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, va_list ವಾದಗಳು))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

ಎಣಿಕೆ xkb_log_level
xkb_context_get_log_level(ರಚನೆ xkb_context *ctx)
{
    ಹಿಂತಿರುಗಿ ctx->log_level;
}

ಶೂನ್ಯ
xkb_context_ಗಣ_log_level(ರಚನೆ xkb_context *ctx, ಎಣಿಕೆ xkb_log_level level)
{
    ctx->log_level = level;
}

ಪೂರ್ಣಾಂಕ
xkb_context_get_log_verbosity(ರಚನೆ xkb_context *ctx)
{
    ಹಿಂತಿರುಗಿ ctx->log_verbosity;
}

ಶೂನ್ಯ
xkb_context_ಗಣ_log_verbosity(ರಚನೆ xkb_context *ctx, ಪೂರ್ಣಾಂಕ verbosity)
{
    ctx->log_verbosity = verbosity;
}

ಶೂನ್ಯ *
xkb_context_get_user_ದತ್ತಾಂಶ(ರಚನೆ xkb_context *ctx)
{
    ಆದರೆ (ctx)
        ಹಿಂತಿರುಗಿ ctx->user_ದತ್ತಾಂಶ;
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಶೂನ್ಯ
xkb_context_ಗಣ_user_ದತ್ತಾಂಶ(ರಚನೆ xkb_context *ctx, ಶೂನ್ಯ *user_ದತ್ತಾಂಶ)
{
    ctx->user_ದತ್ತಾಂಶ = user_ದತ್ತಾಂಶ;
}
