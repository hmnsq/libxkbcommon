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
    #endif
#endif

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
context_include_path_append(संरचना xkb_context *ctx, अचर वर्ण *path)
{
    पूर्णाङ्क err = ENOMEM;

    वर्ण *tmp = strdup(path);
    यदि (!tmp)
        गच्छ err;

    संरचना stat stat_buf;
    err = stat(path, &stat_buf);
    यदि (err != 0) {
        err = त्रुटि_सं;
        गच्छ err;
    }
    यदि (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        गच्छ err;
    }

    यदि (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        गच्छ err;
    }

    darray_append(ctx->includes, tmp);
    /* Use “info” log level to facilitate bug reporting. */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID, "Include path added: %s\n", tmp);

    फर्काऊ 1;

err:
    यदि (tmp)
        darray_append(ctx->failed_includes, tmp);
    /*
     * This error is not fatal because some valid paths may still be defined.
     * Use “info” log level to facilitate bug reporting.
     */
    log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
             "Include path failed: \"%s\" (%s)\n", path, म_त्रुटि(err));
    फर्काऊ 0;
}

/**
 * Append one directory to the context’s include path.
 */
पूर्णाङ्क
xkb_context_include_path_append(संरचना xkb_context *ctx, अचर वर्ण *path)
{
    फर्काऊ (xkb_context_init_includes(ctx))
        ? context_include_path_append(ctx, path)
        : 0;
}

अचर वर्ण *
xkb_context_include_path_get_extra_path(संरचना xkb_context *ctx)
{
    अचर वर्ण * अचर extra = xkb_context_getenv(ctx, "XKB_CONFIG_EXTRA_PATH");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    फर्काऊ extra ? extra : DFLT_XKB_CONFIG_EXTRA_PATH;
}

#यदि HAVE_XKB_EXTENSIONS_DIRECTORIES
अचर वर्ण *
xkb_context_include_path_get_unversioned_extensions_path(संरचना xkb_context *ctx)
{
    अचर वर्ण *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH");
    फर्काऊ ext
        ? ext
#ifdef DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_UNVERSIONED_EXTENSIONS_PATH;
#अन्यथा
        : रिक्त;
#endif
}

अचर वर्ण *
xkb_context_include_path_get_versioned_extensions_path(संरचना xkb_context *ctx)
{
    अचर वर्ण *ext =
        xkb_context_getenv(ctx, "XKB_CONFIG_VERSIONED_EXTENSIONS_PATH");
    फर्काऊ ext
        ? ext
#ifdef DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH
        : DFLT_XKB_CONFIG_VERSIONED_EXTENSIONS_PATH;
#अन्यथा
        : रिक्त;
#endif
}

स्थिर पूर्णाङ्क
compare_str(अचर शून्य *a, अचर शून्य *b)
{
    फर्काऊ म_भेद(*(वर्ण **)a, *(वर्ण **) b);
}

स्थिर पूर्णाङ्क
add_direct_subdirectories(संरचना xkb_context *ctx, अचर वर्ण *path,
                          darray_string *extensions,
                          darray_size_t versioned_गन्ती,
                          माप_प्रकार versioned_path_length)
{
    पूर्णाङ्क ret = 0;
    पूर्णाङ्क err = ENOMEM;
    सूची *dir = रिक्त;

    /* Check extensions parent directory */
    संरचना stat stat_buf;
    err = stat(path, &stat_buf);
    यदि (err != 0) {
        err = त्रुटि_सं;
        गच्छ err;
    }
    यदि (!S_ISDIR(stat_buf.st_mode)) {
        err = ENOTDIR;
        गच्छ err;
    }
    यदि (!check_eaccess(path, R_OK | X_OK)) {
        err = EACCES;
        गच्छ err;
    }

    dir = सूची_खोलो(path);
    यदि (dir == रिक्त) {
        err = EACCES;
        गच्छ err;
    }

    संरचना dirent *entry;
    वर्ण path_buf[PATH_MAX] = "";
    versioned_path_length++; /* Additional final ‘/’ */
    जबसम्म ((entry = सूची_पढ़ो(dir)) != रिक्त) {
        अचर वर्ण *name = entry->d_name;

        /* Skip special entries */
        यदि (म_भेद(name, ".") == 0 || म_भेद(name, "..") == 0)
            जारी;

        यदि (!snprintf_safe(path_buf, परिमाण(path_buf), "%s/%s", path, name)) {
            err = ENOMEM;
            गच्छ err;
        }
        यदि (stat(path_buf, &stat_buf) != 0 || !S_ISDIR(stat_buf.st_mode))
            जारी;

        /* Skip if the corresponding versioned directory is already included */
        लागि (darray_size_t i = 0; i < versioned_गन्ती; i++) {
            अचर वर्ण * अचर prev_name = darray_item(*extensions, i)
                                         + versioned_path_length;
            यदि (म_भेद(name, prev_name) == 0)
                गच्छ next;
        }

        वर्ण *ext_path = strdup_safe(path_buf);
        यदि (!ext_path) {
            err = ENOMEM;
            गच्छ err;
        }

        darray_append(*extensions, ext_path);
next:
    {} /* Label at end of compound statement is a C23 extension */
    }

    बंद_सूची(dir);

    यदि (darray_size(*extensions) > versioned_गन्ती) {
        /* Sort the entries, so that they are appended in lexicographic order */
        क्विक(darray_वस्तुहरू(*extensions) + versioned_गन्ती,
              darray_size(*extensions) - versioned_गन्ती,
              परिमाण(*darray_वस्तुहरू(*extensions)), &compare_str);
        वर्ण **ext_path;
        darray_foreach_from(ext_path, *extensions, versioned_गन्ती) {
            ret |= context_include_path_append(ctx, *ext_path);
        }
    }

    फर्काऊ ret;

err:
    log_dbg(ctx, XKB_LOG_MESSAGE_NO_ID,
            "Include extensions path failed: %s (%s)\n", path, म_त्रुटि(err));
    यदि (dir)
        बंद_सूची(dir);

    फर्काऊ ret;
}
#endif

अचर वर्ण *
xkb_context_include_path_get_system_path(संरचना xkb_context *ctx)
{
    अचर वर्ण * अचर जरा = xkb_context_getenv(ctx, "XKB_CONFIG_ROOT");
    /*
     * Only use default if path is undefined, but accept empty string, which may
     * be unintentional and should be reported.
     */
    फर्काऊ जरा ? जरा : DFLT_XKB_CONFIG_जरा;
}

/**
 * Append the default include directories to the context.
 */
पूर्णाङ्क
xkb_context_include_path_append_default(संरचना xkb_context *ctx)
{
    /*
     * We do not call `xkb_context_init_includes()` here, because either
     * we already initialized the includes paths or we are doing it now.
     */

    वर्ण *user_path;
    पूर्णाङ्क ret = 0;

    अचर वर्ण * अचर home = xkb_context_getenv(ctx, "HOME");
    अचर वर्ण * अचर xdg = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    /* Accept empty string, which may be unintentional and should be reported */
    यदि (xdg != रिक्त) {
        user_path = asprintf_safe("%s/xkb", xdg);
        यदि (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            मुक्त(user_path);
        }
    } अन्यथा यदि (home != रिक्त) {
        /* XDG_CONFIG_HOME fallback is $HOME/.config/ */
        user_path = asprintf_safe("%s/.config/xkb", home);
        यदि (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            मुक्त(user_path);
        }
    }

    यदि (home != रिक्त) {
        user_path = asprintf_safe("%s/.xkb", home);
        यदि (user_path) {
            ret |= context_include_path_append(ctx, user_path);
            मुक्त(user_path);
        }
    }

    अचर वर्ण * अचर extra = xkb_context_include_path_get_extra_path(ctx);
    ret |= context_include_path_append(ctx, extra);

#यदि HAVE_XKB_EXTENSIONS_DIRECTORIES
    darray_string extensions = darray_new();

    /* Versioned extensions directory */
    अचर वर्ण *extensions_path =
        xkb_context_include_path_get_versioned_extensions_path(ctx);
    माप_प्रकार versioned_path_length = 0;
    यदि (extensions_path) {
        /* Add direct subdirectories */
        ret |= add_direct_subdirectories(ctx, extensions_path,
                                         &extensions, 0, 0);
        versioned_path_length = म_माप(extensions_path);
    }

    /* Unversioned extensions directory */
    extensions_path =
        xkb_context_include_path_get_unversioned_extensions_path(ctx);
    यदि (extensions_path) {
        /*
         * Add direct subdirectories, except those already added from the
         * *versioned* extensions path hereinabove.
         */
        ret |= add_direct_subdirectories(ctx, extensions_path, &extensions,
                                         darray_size(extensions),
                                         versioned_path_length);
    }

    वर्ण **ext_path;
    darray_foreach(ext_path, extensions) {
        मुक्त(*ext_path);
    }
    darray_free(extensions);
#endif

    /* Canonical XKB root */
    अचर वर्ण * अचर जरा = xkb_context_include_path_get_system_path(ctx);
    अचर bool has_जरा = context_include_path_append(ctx, जरा);
    ret |= has_जरा;

    /*
     * Fallback for misconfigured setups.
     * Some setups use the assumption that the canonical XKB root is always the
     * legacy X11 one, but this is no longer true since xkeyboard-config 2.45,
     * where the X11 path is now a mere symlink to a dedicated xkeyboard-config
     * data directory.
     * This fallback can still be skipped if deliberately using an empty string
     * for the canonical XKB root hereinabove.
     */
    यदि (!has_जरा && जरा[0] != '\0') {
        log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                 "Root include path failed; fallback to \"%s\". "
                 "The setup is probably misconfigured. "
                 "Please ensure that \"%s\" is available in the environment.\n",
                 DFLT_XKB_LEGACY_जरा, जरा);
        ret |= context_include_path_append(ctx, DFLT_XKB_LEGACY_जरा);
    }

    फर्काऊ ret;
}

/**
 * Remove all entries in the context's include path.
 */
शून्य
xkb_context_include_path_clear(संरचना xkb_context *ctx)
{
    वर्ण **path;

    darray_foreach(path, ctx->includes)
        मुक्त(*path);
    darray_free(ctx->includes);

    darray_foreach(path, ctx->failed_includes)
        मुक्त(*path);
    darray_free(ctx->failed_includes);

    /* It does not make sense to keep the pending defaults */
    ctx->pending_default_includes = false;
}

/**
 * xkb_context_include_path_clear() + xkb_context_include_path_append_default()
 */
पूर्णाङ्क
xkb_context_include_path_reset_defaults(संरचना xkb_context *ctx)
{
    xkb_context_include_path_clear(ctx);
    फर्काऊ xkb_context_include_path_append_default(ctx);
}

/**
 * Returns the number of entries in the context's include path.
 */
अचिन्हित पूर्णाङ्क
xkb_context_अङ्क_include_paths(संरचना xkb_context *ctx)
{
    फर्काऊ (xkb_context_init_includes(ctx))
        ? darray_size(ctx->includes)
        : 0;
}

/**
 * Returns the given entry in the context's include path, or NULL if an
 * invalid index is passed.
 */
अचर वर्ण *
xkb_context_include_path_get(संरचना xkb_context *ctx, अचिन्हित पूर्णाङ्क idx)
{
    यदि (idx >= xkb_context_अङ्क_include_paths(ctx))
        फर्काऊ रिक्त;

    फर्काऊ darray_item(ctx->includes, idx);
}

/**
 * Take a new reference on the context.
 */
संरचना xkb_context *
xkb_context_ref(संरचना xkb_context *ctx)
{
    प्रतिज्ञा(ctx->refcnt > 0);
    ctx->refcnt++;
    फर्काऊ ctx;
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
        फर्काऊ;

    मुक्त(ctx->x11_atom_cache);
    xkb_context_include_path_clear(ctx);
    atom_table_free(ctx->atom_table);
    मुक्त(ctx);
}

स्थिर अचर वर्ण *
log_level_to_prefix(गणना xkb_log_level level)
{
    विकल्प (level) {
    पक्ष XKB_LOG_LEVEL_DEBUG:
        फर्काऊ "xkbcommon: DEBUG: ";
    पक्ष XKB_LOG_LEVEL_INFO:
        फर्काऊ "xkbcommon: INFO: ";
    पक्ष XKB_LOG_LEVEL_WARNING:
        फर्काऊ "xkbcommon: WARNING: ";
    पक्ष XKB_LOG_LEVEL_ERROR:
        फर्काऊ "xkbcommon: ERROR: ";
    पक्ष XKB_LOG_LEVEL_CRITICAL:
        फर्काऊ "xkbcommon: CRITICAL: ";
    स्वभाव:
        फर्काऊ रिक्त;
    }
}

ATTR_PRINTF(3, 0) स्थिर शून्य
default_log_fn(संरचना xkb_context *ctx, गणना xkb_log_level level,
               अचर वर्ण *fmt, बहु_सूची args)
{
    अचर वर्ण *prefix = log_level_to_prefix(level);

    यदि (prefix)
        ख_लिखो(मानक_त्रुटि, "%s", prefix);
    भख_लिखो(मानक_त्रुटि, fmt, args);
}

स्थिर गणना xkb_log_level
log_level(अचर वर्ण *level) {
    त्रुटि_सं = 0;
    वर्ण *endptr = रिक्त;
    अचर दीर्घ lvl = म_से_दीर्घ(level, &endptr, 10);
    यदि (त्रुटि_सं == 0 && (endptr[0] == '\0' || is_space(endptr[0])))
        फर्काऊ (गणना xkb_log_level)lvl;
    यदि (istreq_prefix("crit", level))
        फर्काऊ XKB_LOG_LEVEL_CRITICAL;
    यदि (istreq_prefix("err", level))
        फर्काऊ XKB_LOG_LEVEL_ERROR;
    यदि (istreq_prefix("warn", level))
        फर्काऊ XKB_LOG_LEVEL_WARNING;
    यदि (istreq_prefix("info", level))
        फर्काऊ XKB_LOG_LEVEL_INFO;
    यदि (istreq_prefix("debug", level) || istreq_prefix("dbg", level))
        फर्काऊ XKB_LOG_LEVEL_DEBUG;

    फर्काऊ XKB_LOG_LEVEL_ERROR;
}

स्थिर पूर्णाङ्क
log_verbosity(अचर वर्ण *verbosity) {
    त्रुटि_सं = 0;
    अचर दीर्घ पूर्णाङ्क v = म_से_दीर्घ(verbosity, रिक्त, 10);
    यदि (त्रुटि_सं == 0) {
        /* Truncate */
        फर्काऊ (पूर्णाङ्क) v;
    }

    फर्काऊ XKB_LOG_VERBOSITY_DEFAULT;
}

/**
 * Create a new context.
 */
संरचना xkb_context *
xkb_context_new(गणना xkb_context_flags flags)
{
    अचर वर्ण *env;
    संरचना xkb_context *ctx = सुस्मृति(1, परिमाण(*ctx));

    यदि (!ctx)
        फर्काऊ रिक्त;

    ctx->refcnt = 1;
    ctx->log_fn = default_log_fn;
    ctx->log_level = XKB_LOG_LEVEL_ERROR;
    ctx->log_verbosity = XKB_LOG_VERBOSITY_DEFAULT;

    अचर गणना xkb_context_flags invalid_flags =
        (flags & ~(गणना xkb_context_flags)XKB_CONTEXT_FLAGS_मानहरू);
    यदि (invalid_flags) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Invalid context flags: 0x%x\n", invalid_flags);
        मुक्त(ctx);
        फर्काऊ रिक्त;
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
    यदि (env)
        xkb_context_set_log_level(ctx, log_level(env));

    env = xkb_context_getenv(ctx, "XKB_LOG_VERBOSITY");
    यदि (env)
        xkb_context_set_log_verbosity(ctx, log_verbosity(env));

    ctx->atom_table = atom_table_new();
    यदि (!ctx->atom_table) {
        xkb_context_unref(ctx);
        फर्काऊ रिक्त;
    }

    ctx->x11_atom_cache = रिक्त;

    फर्काऊ ctx;
}

शून्य
xkb_context_set_log_fn(संरचना xkb_context *ctx,
                       शून्य (*log_fn)(संरचना xkb_context *ctx,
                                      गणना xkb_log_level level,
                                      अचर वर्ण *fmt, बहु_सूची args))
{
    ctx->log_fn = (log_fn ? log_fn : default_log_fn);
}

गणना xkb_log_level
xkb_context_get_log_level(संरचना xkb_context *ctx)
{
    फर्काऊ ctx->log_level;
}

शून्य
xkb_context_set_log_level(संरचना xkb_context *ctx, गणना xkb_log_level level)
{
    ctx->log_level = level;
}

पूर्णाङ्क
xkb_context_get_log_verbosity(संरचना xkb_context *ctx)
{
    फर्काऊ ctx->log_verbosity;
}

शून्य
xkb_context_set_log_verbosity(संरचना xkb_context *ctx, पूर्णाङ्क verbosity)
{
    ctx->log_verbosity = verbosity;
}

शून्य *
xkb_context_get_user_data(संरचना xkb_context *ctx)
{
    यदि (ctx)
        फर्काऊ ctx->user_data;
    फर्काऊ रिक्त;
}

शून्य
xkb_context_set_user_data(संरचना xkb_context *ctx, शून्य *user_data)
{
    ctx->user_data = user_data;
}
