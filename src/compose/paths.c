/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <locale.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "util-paths.h"
#अन्तर्भाव "context.h"
#अन्तर्भाव "paths.h"

गणना resolve_name_direction {
    डावा_TO_उजवा,
    उजवा_TO_डावा,
};

स्थिरांक अक्षर *
get_xlocaledir_path(संरचना xkb_context *ctx)
{
    स्थिरांक अक्षर *dir = xkb_context_getenv(ctx, "XLOCALEDIR");
    जर (!dir)
        dir = XLOCALEDIR;
    परत dir;
}

/*
 * Files like compose.dir have the format LEFT: RIGHT.  Lookup @name in
 * such a file and return its matching value, according to @direction.
 * @filename is relative to the xlocaledir.
 */
स्थिर अक्षर *
resolve_name(संरचना xkb_context *ctx, स्थिरांक अक्षर *filename,
             गणना resolve_name_direction direction, स्थिरांक अक्षर *name)
{
    पूर्णांक ret;
    bool ok;
    स्थिरांक अक्षर *xlocaledir;
    अक्षर path[512];
    खाता *file;
    अक्षर *string;
    माप_प्रकार string_size;
    स्थिरांक अक्षर *end;
    स्थिरांक अक्षर *s, *डावा, *उजवा;
    अक्षर *match;
    माप_प्रकार डावा_len, उजवा_len, name_len;

    xlocaledir = get_xlocaledir_path(ctx);

    ret = snprintf(path, आकार(path), "%s/%s", xlocaledir, filename);
    जर (ret < 0 || (माप_प्रकार) ret >= आकार(path))
        परत रिक्त;

    file = ख_खोलो(path, "rb");
    जर (!file)
        परत रिक्त;

    ok = map_file(file, &string, &string_size);
    ख_बंद(file);
    जर (!ok)
        परत रिक्त;

    s = string;
    end = string + string_size;
    name_len = म_माप(name);
    match = रिक्त;

    पर्यंत (s < end) {
        /* Skip spaces. */
        पर्यंत (s < end && is_space(*s))
            s++;

        /* Skip comments. */
        जर (s < end && *s == '#') {
            पर्यंत (s < end && *s != '\n')
                s++;
            सुरूठेवा;
        }

        /* Get the left value. */
        डावा = s;
        पर्यंत (s < end && !is_space(*s) && *s != ':')
            s++;
        डावा_len = (माप_प्रकार)(s - डावा);

        /* There's an optional colon between left and right. */
        जर (s < end && *s == ':')
            s++;

        /* Skip spaces. */
        पर्यंत (s < end && is_space(*s))
            s++;

        /* Get the right value. */
        उजवा = s;
        पर्यंत (s < end && !is_space(*s))
            s++;
        उजवा_len = (माप_प्रकार)(s - उजवा);

        /* Discard rest of line. */
        पर्यंत (s < end && *s != '\n')
            s++;

        जर (direction == डावा_TO_उजवा) {
            जर (डावा_len == name_len && स_भेद(डावा, name, डावा_len) == 0) {
                match = strndup(उजवा, उजवा_len);
                थांबा;
            }
        } अन्यथा जर (direction == उजवा_TO_डावा &&
                   उजवा_len == name_len &&
                   स_भेद(उजवा, name, उजवा_len) == 0) {
            match = strndup(डावा, डावा_len);
            थांबा;
        }
    }

    unmap_file(string, string_size);
    परत match;
}

अक्षर *
resolve_locale(संरचना xkb_context *ctx, स्थिरांक अक्षर *locale)
{
    अक्षर *alias = resolve_name(ctx, "locale.alias", डावा_TO_उजवा, locale);
    परत alias ? alias : strdup(locale);
}

अक्षर *
get_xcomposefile_path(संरचना xkb_context *ctx)
{
    परत strdup_safe(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

अक्षर *
get_xdg_xcompose_file_path(संरचना xkb_context *ctx)
{
    स्थिरांक अक्षर *xdg_config_home;
    स्थिरांक अक्षर *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    जर (!xdg_config_home || !is_absolute_path(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        जर (!home)
            परत रिक्त;
        परत asprintf_safe("%s/.config/XCompose", home);
    }

    परत asprintf_safe("%s/XCompose", xdg_config_home);
}

अक्षर *
get_home_xcompose_file_path(संरचना xkb_context *ctx)
{
    स्थिरांक अक्षर *home;

    home = xkb_context_getenv(ctx, "HOME");
    जर (!home)
        परत रिक्त;

    परत asprintf_safe("%s/.XCompose", home);
}

अक्षर *
get_locale_compose_file_path(संरचना xkb_context *ctx, स्थिरांक अक्षर *locale)
{
    अक्षर *resolved;
    अक्षर *path;

    /*
     * WARNING: Random workaround ahead.
     *
     * We currently do not support non-UTF-8 Compose files.  The C/POSIX
     * locale is specified to be the default fallback locale with an
     * ASCII charset.  But for some reason the compose.dir points the C
     * locale to the iso8859-1/Compose file, which is not ASCII but
     * ISO8859-1.  Since this is bound to happen a lot, and since our API
     * is UTF-8 based, and since 99% of the time a C locale is really just
     * a misconfiguration for UTF-8, let's do the most helpful thing.
     */
    स्थिर स्थिरांक अक्षर * स्थिरांक fallback = "en_US.UTF-8";
    स्थिर स्थिरांक अक्षर * स्थिरांक registry = "compose.dir";
    जर (streq(locale, "C"))
        locale = fallback;

    resolved = resolve_name(ctx, registry, उजवा_TO_डावा, locale);
    जर (!resolved) {
#जर HAVE_NEWLOCALE
        /*
         * There is no extension mechanism for X11 Compose locales.
         * Instead of failing, we just use `en_US.UTF-8` as a fallback because
         * it is what most locales use anyway.
         * But we still want to fail if the locale is invalid in the system,
         * so that we can catch missing system locales and typos.
         */
        locale_t loc = newlocale(LC_ALL_MASK, locale, (locale_t) 0);
        जर (loc == (locale_t) 0) {
            log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                    "No Compose file for locale \"%s\": "
                    "locale is either invalid or not installed\n", locale);
            परत रिक्त;
        } अन्यथा {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, उजवा_TO_डावा, fallback);
            जर (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                परत रिक्त;
            } अन्यथा {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#अन्यथा
        परत रिक्त;
#endif
    }

    जर (is_absolute_path(resolved)) {
        path = resolved;
    }
    अन्यथा {
        स्थिरांक अक्षर *xlocaledir = get_xlocaledir_path(ctx);
        path = asprintf_safe("%s/%s", xlocaledir, resolved);
        मुक्त(resolved);
    }

    परत path;
}
