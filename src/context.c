/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/stat.h>

#यदि HAVE_XKB_EXTENSIONS_DIRECTORIES
    #अन्तर्भाव <limits.h>
    #यदि HAVE_UNISTD_H && HAVE_DIRENT_H
        #अन्तर्भाव <dirent.h>
        #अन्तर्भाव <unistd.h>
    #यदि_अन्तः
#यदि_अन्तः

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "context.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "features/enums.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "utils.h"


/**
 * Append one directory to the context’s include path.
 */
स्थिर पूर्णाङ्क
context_include_मार्गः_append(संरचना xkb_context *ctx, ध्रुव वर्ण *मार्गः)
{
    पूर्णाङ्क err = ENOMEM;

    वर्ण *tmp = strdup(मार्गः);
    यदि (!tmp)
        गच्छ err;

    संरचना stat stat_buf;
    err = stat(मार्गः, &stat_buf);
    यदि (err != 0) {
        err = errno;
        गच्छ err;
    }
    यदि (!S_ISDIR(stat_buf.st_बहुलकम्)) {
        err = ENOTDIR;
        गच्छ err;
    }

    यदि (!check_eaccess(मार्गः, R_OK | X_OK)) {
        err = EACCES;
        गच्छ err;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_सन्देशः_NO_ID, "Include path added: %s\n", tmp);

    प्रत्यागम 1;

err:
    यदि (tmp)
        darray_append(ctx->failed_includes, tmp);
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_सन्देशः_NO_ID,
             "Include path failed: \"%s\" (%s)\n", मार्गः, strerror(err));
    प्रत्यागम 0;
}

/**
 * Append one directory to the context’s include path.
 */
पूर्णाङ्क
xkb_context_include_मार्गः_append(संरचना xkb_context *ctx, ध्रुव वर्ण *मार्गः)
{
    प्रत्यागम (xkb_context_init_includes(ctx))
        ? context_include_मार्गः_append(ctx, मार्गः)
        : 0;
}

ध्रुव वर्ण *
xkb_context_include_मार्गः_get_extra_मार्गः(संरचना xkb_context *ctx)
{
    ध्रुव वर्ण * ध्रुव extra = xkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    प्रत्यागम extra ? extra : DFLT_XKB_CONFIG_EXTRA_मार्गः;
}

#यदि HAVE_XKB_EXTENSIONS_DIRECTORIES
ध्रुव वर्ण *
xkb_context_include_मार्गः_get_unversioned_extensions_मार्गः(संरचना xkb_context *ctx)
{
    ध्रुव वर्ण *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
    प्रत्यागम ext
        ? ext
#यदि_परिभाषितम् DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_मार्गः
        : DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_मार्गः;
#अन्यथा
        : रिक्त;
#यदि_अन्तः
}

ध्रुव वर्ण *
xkb_context_include_मार्गः_get_versioned_extensions_मार्गः(संरचना xkb_context *ctx)
{
    ध्रुव वर्ण *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
    प्रत्यागम ext
        ? ext
#यदि_परिभाषितम् DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_मार्गः
        : DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_मार्गः;
#अन्यथा
        : रिक्त;
#यदि_अन्तः
}

स्थिर पूर्णाङ्क
compare_str(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
    प्रत्यागम वाक्यं_तुलय(*(वर्ण **)a, *(वर्ण **) b);
}

स्थिर पूर्णाङ्क
सङ्कलय_प्रत्यक्षम्_subdirectories(संरचना xkb_context *ctx, ध्रुव वर्ण *मार्गः,
                          darray_string *extensions,
                          darray_आकार_t versioned_गणनम्,
                          आकार_t versioned_मार्गः_दीर्घता)
{
    पूर्णाङ्क ret = 0;
    पूर्णाङ्क err = ENOMEM;
    DIR *dir = रिक्त;

    /* Check extensions parent directory */
    संरचना stat stat_buf;
    err = stat(मार्गः, &stat_buf);
    यदि (err != 0) {
        err = errno;
        गच्छ err;
    }
    यदि (!S_ISDIR(stat_buf.st_बहुलकम्)) {
        err = ENOTDIR;
        गच्छ err;
    }
    यदि (!check_eaccess(मार्गः, R_OK | X_OK)) {
        err = EACCES;
        गच्छ err;
    }

    dir = opendir(मार्गः);
    यदि (dir == रिक्त) {
        err = EACCES;
        गच्छ err;
    }

    संरचना dirent *प्रविष्टिः;
    वर्ण मार्गः_buf[PATH_MAX] = "";
    versioned_मार्गः_दीर्घता++; /* Additional final ‘/’ */
    यावत् ((प्रविष्टिः = readdir(dir)) != रिक्त) {
        ध्रुव वर्ण *name = प्रविष्टिः->d_name;

        /* Skip special entries */
        यदि (वाक्यं_तुलय(name, ".") == 0 || वाक्यं_तुलय(name, "..") == 0)
            अनुवर्त;

        यदि (!snprintf_safe(मार्गः_buf, परिमाण(मार्गः_buf), "%s/%s", मार्गः, name)) {
            err = ENOMEM;
            गच्छ err;
        }
        यदि (stat(मार्गः_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_बहुलकम्))
            अनुवर्त;

        /* Skip if the corresponding versioned directory is already included */
        प्रति (darray_आकार_t i = 0; i < versioned_गणनम्; i++) {
            ध्रुव वर्ण * ध्रुव पूर्व_name = darray_item(*extensions, i)
                                         + versioned_मार्गः_दीर्घता;
            यदि (वाक्यं_तुलय(name, पूर्व_name) == 0)
                गच्छ अग्र;
        }

        वर्ण *ext_मार्गः = strdup_safe(मार्गः_buf);
        यदि (!ext_मार्गः) {
            err = ENOMEM;
            गच्छ err;
        }

        darray_append(*extensions, ext_मार्गः);
अग्र:
    {} /* Label at end of compound statement is a C23 extension */
    }

    closedir(dir);

    यदि (darray_आकार(*extensions) > versioned_गणनम्) {
        /* Sort the entries, so that they are appended in lexicographic order */
        शीघ्रं_क्रमय(darray_वस्तूनि(*extensions) + versioned_गणनम्,
              darray_आकार(*extensions) - versioned_गणनम्,
              परिमाण(*darray_वस्तूनि(*extensions)), &compare_str);
        वर्ण **ext_मार्गः;
        darray_foreach_from(ext_मार्गः, *extensions, versioned_गणनम्) {
            ret |= context_include_मार्गः_append(ctx, *ext_मार्गः);
        }
    }

    प्रत्यागम ret;

err:
    log_dbg(ctx, XKB_LOG_सन्देशः_NO_ID,
            "Include extensions path failed: %s (%s)\n", मार्गः, strerror(err));
    यदि (dir)
        closedir(dir);

    प्रत्यागम ret;
}
#यदि_अन्तः

ध्रुव वर्ण *
xkb_context_include_मार्गः_get_system_मार्गः(संरचना xkb_context *ctx)
{
    ध्रुव वर्ण * ध्रुव मूलम् = xkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    प्रत्यागम मूलम् ? मूलम् : DFLT_XKB_CONFIG_मूलम्;
}

/**
 * Append the default include directories to the context.
 */
पूर्णाङ्क
xkb_context_include_मार्गः_append_default(संरचना xkb_context *ctx)
{
    /*
     * We do not call `xkb_context_init_includes()` here, because either
     * we already initialized the includes paths or we are doing it now.
     */

    वर्ण *user_मार्गः;
    पूर्णाङ्क ret = 0;

    ध्रुव वर्ण * ध्रुव home = xkb_context_getenv(ctx, "HOME");
    ध्रुव वर्ण * ध्रुव xdg = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    यदि (xdg != रिक्त) {
        user_मार्गः = asprintf_safe("%s/xkb", xdg);
        यदि (user_मार्गः) {
            ret |= context_include_मार्गः_append(ctx, user_मार्गः);
            मुक्त(user_मार्गः);
        }
    } अन्यथा यदि (home != रिक्त) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        user_मार्गः = asprintf_safe("%s/.config/xkb", home);
        यदि (user_मार्गः) {
            ret |= context_include_मार्गः_append(ctx, user_मार्गः);
            मुक्त(user_मार्गः);
        }
    }

    यदि (home != रिक्त) {
        user_मार्गः = asprintf_safe("%s/.xkb", home);
        यदि (user_मार्गः) {
            ret |= context_include_मार्गः_append(ctx, user_मार्गः);
            मुक्त(user_मार्गः);
        }
    }

    ध्रुव वर्ण * ध्रुव extra = xkb_context_include_मार्गः_get_extra_मार्गः(ctx);
    ret |= context_include_मार्गः_append(ctx, extra);

#यदि HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    ध्रुव वर्ण *extensions_मार्गः =
        xkb_context_include_मार्गः_get_versioned_extensions_मार्गः(ctx);
    आकार_t versioned_मार्गः_दीर्घता = 0;
    यदि (extensions_मार्गः) {
        /* Add direct subdirectories */
        ret |= सङ्कलय_प्रत्यक्षम्_subdirectories(ctx, extensions_मार्गः,
                                         &extensions, 0, 0);
        versioned_मार्गः_दीर्घता = वाक्यदैर्घ्यम्(extensions_मार्गः);
    }

    /* Unversioned extensions directory */
    extensions_मार्गः =
        xkb_context_include_मार्गः_get_unversioned_extensions_मार्गः(ctx);
    यदि (extensions_मार्गः) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= सङ्कलय_प्रत्यक्षम्_subdirectories(ctx, extensions_मार्गः, &extensions,
                                         darray_आकार(extensions),
                                         versioned_मार्गः_दीर्घता);
    }

    वर्ण **ext_मार्गः;
    darray_foreach(ext_मार्गः, extensions) {
        मुक्त(*ext_मार्गः);
    }
    darray_free(extensions);
#यदि_अन्तः

    /* Canonical XKB root */
    ध्रुव वर्ण * ध्रुव मूलम् = xkb_context_include_मार्गः_get_system_मार्गः(ctx);
    ध्रुव bool has_मूलम् = context_include_मार्गः_append(ctx, मूलम्);
    ret |= has_मूलम्;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    यदि (!has_मूलम् && मूलम्[0] != '\0') {
        log_warn(ctx, XKB_LOG_सन्देशः_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_मूलम्, मूलम्);
        ret |= context_include_मार्गः_append(ctx, DFLT_XKB_LEGACY_मूलम्);
    }

    प्रत्यागम ret;
}

/**
 * Remove all entries in the context's include path.
 */
शून्य
xkb_context_include_मार्गः_clear(संरचना xkb_context *ctx)
{
    वर्ण **मार्गः;

    darray_foreach(मार्गः, ctx->includes)
        मुक्त(*मार्गः);
    darray_free(ctx->includes);

    darray_foreach(मार्गः, ctx->failed_includes)
        मुक्त(*मार्गः);
    darray_free(ctx->failed_includes);

    /* It does not make sense to keep the pending defaults */
    ctx->pending_default_includes = false;
}

/**
 * xkb_context_include_path_clear() + xkb_context_include_path_append_default()
 */
पूर्णाङ्क
xkb_context_include_मार्गः_reset_defaults(संरचना xkb_context *ctx)
{
    xkb_context_include_मार्गः_clear(ctx);
    प्रत्यागम xkb_context_include_मार्गः_append_default(ctx);
}

/**
 * Returns the number of entries in the context's include path.
 */
अचिह्नित पूर्णाङ्क
xkb_context_अङ्कः_include_paths(संरचना xkb_context *ctx)
{
    प्रत्यागम (xkb_context_init_includes(ctx))
        ? darray_आकार(ctx->includes)
        : 0;
}

/**
 * Returns the given entry in the context's include path, or NULL if an
 * invalid index is passed.
 */
ध्रुव वर्ण *
xkb_context_include_मार्गः_get(संरचना xkb_context *ctx, अचिह्नित पूर्णाङ्क idx)
{
    यदि (idx >= xkb_context_अङ्कः_include_paths(ctx))
        प्रत्यागम रिक्त;

    प्रत्यागम darray_item(ctx->includes, idx);
}

/**
 * Take a new reference on the context.
 */
संरचना xkb_context *
xkb_context_ref(संरचना xkb_context *ctx)
{
    प्रतिज्ञा(ctx->refcnt > 0);
    ctx->refcnt++;
    प्रत्यागम ctx;
}

/**
 * Drop an existing reference on the context, and free it if the refcnt is
 * now 0.
 */
शून्य
xkb_context_unref(संरचना xkb_context *ctx)
{
    प्रतिज्ञा(!ctx || ctx->refcnt > 0);
    यदि (!ctx || --ctx->refcnt > 0)
        प्रत्यागम;

    मुक्त(ctx->x11_atom_cache);
    xkb_context_include_मार्गः_clear(ctx);
    atom_सारणी_free(ctx->atom_सारणी);
    मुक्त(ctx);
}

स्थिर ध्रुव वर्ण *
log_level_गन्तव्यम्_उपसर्गः(गणना xkb_log_level level)
{
    विकल्प (level) {
    पक्ष XKB_LOG_LEVEL_DEBUG:
        प्रत्यागम "xkbcommon: DEBUG: ";
    पक्ष XKB_LOG_LEVEL_INFO:
        प्रत्यागम "xkbcommon: INFO: ";
    पक्ष XKB_LOG_LEVEL_WARNING:
        प्रत्यागम "xkbcommon: WARNING: ";
    पक्ष XKB_LOG_LEVEL_ERROR:
        प्रत्यागम "xkbcommon: ERROR: ";
    पक्ष XKB_LOG_LEVEL_CRITICAL:
        प्रत्यागम "xkbcommon: CRITICAL: ";
    स्वभाव:
        प्रत्यागम रिक्त;
    }
}

ATTR_PRINTF(3, 0) स्थिर शून्य
default_log_fn(संरचना xkb_context *ctx, गणना xkb_log_level level,
               ध्रुव वर्ण *fmt, va_list तर्काः)
{
    ध्रुव वर्ण *उपसर्गः = log_level_गन्तव्यम्_उपसर्गः(level);

    यदि (उपसर्गः)
        fprintf(stderr, "%s", उपसर्गः);
    vfprintf(stderr, fmt, तर्काः);
}

स्थिर गणना xkb_log_level
log_level(ध्रुव वर्ण *level) {
    errno = 0;
    वर्ण *endptr = रिक्त;
    ध्रुव दीर्घ lvl = strtol(level, &endptr, 10);
    यदि (errno == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        प्रत्यागम (गणना xkb_log_level)lvl;
    यदि (istreq_उपसर्गः("crit", level))
        प्रत्यागम XKB_LOG_LEVEL_CRITICAL;
    यदि (istreq_उपसर्गः("err", level))
        प्रत्यागम XKB_LOG_LEVEL_ERROR;
    यदि (istreq_उपसर्गः("warn", level))
        प्रत्यागम XKB_LOG_LEVEL_WARNING;
    यदि (istreq_उपसर्गः("info", level))
        प्रत्यागम XKB_LOG_LEVEL_INFO;
    यदि (istreq_उपसर्गः("debug", level) || istreq_उपसर्गः("dbg", level))
        प्रत्यागम XKB_LOG_LEVEL_DEBUG;

    प्रत्यागम XKB_LOG_LEVEL_ERROR;
}

स्थिर पूर्णाङ्क
log_verbosity(ध्रुव वर्ण *verbosity) {
    errno = 0;
    ध्रुव दीर्घ पूर्णाङ्क v = strtol(verbosity, रिक्त, 10);
    यदि (errno == 0) {
        /* Truncate */
        प्रत्यागम (पूर्णाङ्क) v;
    }

    प्रत्यागम XKB_LOG_VERBOSITY_DEFAULT;
}

/**
 * Create a new context.
 */
संरचना xkb_context *
xkb_context_new(गणना xkb_context_flags flags)
{
    ध्रुव वर्ण *env;
    संरचना xkb_context *ctx = शून्यस्मृतिं_देहि(1, परिमाण(*ctx));

    यदि (!ctx)
        प्रत्यागम रिक्त;

    ctx->refcnt = 1;
    ctx->log_fn = default_log_fn;
    ctx->log_level = XKB_LOG_LEVEL_ERROR;
    ctx->log_verbosity = XKB_LOG_VERBOSITY_DEFAULT;

    ध्रुव गणना xkb_context_flags invalid_flags =
        (flags & ~(गणना xkb_context_flags)XKB_CONTEXT_FLAGS_मानानि);
    यदि (invalid_flags) {
        log_err_func(ctx, XKB_LOG_सन्देशः_NO_ID,
                     "Invalid context flags: 0x%x\n", invalid_flags);
        मुक्त(ctx);
        प्रत्यागम रिक्त;
    }

    ctx->use_environment_नामानि = !(flags & XKB_CONTEXT_NO_ENVIRONMENT_नामानि);
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
    यदि (env)
        xkb_context_समुच्चयः_log_level(ctx, log_level(env));

    env = xkb_context_getenv(ctx, "XKB_LOG_VERBOSITY");
    यदि (env)
        xkb_context_समुच्चयः_log_verbosity(ctx, log_verbosity(env));

    ctx->atom_सारणी = atom_सारणी_new();
    यदि (!ctx->atom_सारणी) {
        xkb_context_unref(ctx);
        प्रत्यागम रिक्त;
    }

    ctx->x11_atom_cache = रिक्त;

    प्रत्यागम ctx;
}

शून्य
xkb_context_समुच्चयः_log_fn(संरचना xkb_context *ctx,
                       शून्य (*log_fn)(संरचना xkb_context *ctx,
                                      गणना xkb_log_level level,
                                      ध्रुव वर्ण *fmt, va_list तर्काः))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

गणना xkb_log_level
xkb_context_get_log_level(संरचना xkb_context *ctx)
{
    प्रत्यागम ctx->log_level;
}

शून्य
xkb_context_समुच्चयः_log_level(संरचना xkb_context *ctx, गणना xkb_log_level level)
{
    ctx->log_level = level;
}

पूर्णाङ्क
xkb_context_get_log_verbosity(संरचना xkb_context *ctx)
{
    प्रत्यागम ctx->log_verbosity;
}

शून्य
xkb_context_समुच्चयः_log_verbosity(संरचना xkb_context *ctx, पूर्णाङ्क verbosity)
{
    ctx->log_verbosity = verbosity;
}

शून्य *
xkb_context_get_user_दत्त(संरचना xkb_context *ctx)
{
    यदि (ctx)
        प्रत्यागम ctx->user_दत्त;
    प्रत्यागम रिक्त;
}

शून्य
xkb_context_समुच्चयः_user_दत्त(संरचना xkb_context *ctx, शून्य *user_दत्त)
{
    ctx->user_दत्त = user_दत्त;
}
