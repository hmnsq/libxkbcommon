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
    वामः_TO_दक्षिणः,
    दक्षिणः_TO_वामः,
};

ध्रुव वर्ण *
get_xlocaledir_path(संरचना xkb_context *ctx)
{
    ध्रुव वर्ण *dir = xkb_context_getenv(ctx, "XLOCALEDIR");
    जइ (!dir)
        dir = XLOCALEDIR;
    प्रत्यागम dir;
}

/*
 * Files like compose.dir have the format LEFT: RIGHT.  Lookup @name in
 * such a file and return its matching value, according to @direction.
 * @filename is relative to the xlocaledir.
 */
स्थिर वर्ण *
resolve_name(संरचना xkb_context *ctx, ध्रुव वर्ण *filename,
             गणना resolve_name_direction direction, ध्रुव वर्ण *name)
{
    पूर्णाङ्क ret;
    bool ok;
    ध्रुव वर्ण *xlocaledir;
    वर्ण path[512];
    FILE *file;
    वर्ण *string;
    size_t string_size;
    ध्रुव वर्ण *end;
    ध्रुव वर्ण *s, *वामः, *दक्षिणः;
    वर्ण *match;
    size_t वामः_len, दक्षिणः_len, name_len;

    xlocaledir = get_xlocaledir_path(ctx);

    ret = snprintf(path, परिमाण(path), "%s/%s", xlocaledir, filename);
    जइ (ret < 0 || (size_t) ret >= परिमाण(path))
        प्रत्यागम रिक्त;

    file = fopen(path, "rb");
    जइ (!file)
        प्रत्यागम रिक्त;

    ok = map_file(file, &string, &string_size);
    fclose(file);
    जइ (!ok)
        प्रत्यागम रिक्त;

    s = string;
    end = string + string_size;
    name_len = strlen(name);
    match = रिक्त;

    जाव (s < end) {
        /* Skip spaces. */
        जाव (s < end && is_space(*s))
            s++;

        /* Skip comments. */
        जइ (s < end && *s == '#') {
            जाव (s < end && *s != '\n')
                s++;
            अनुवर्त;
        }

        /* Get the left value. */
        वामः = s;
        जाव (s < end && !is_space(*s) && *s != ':')
            s++;
        वामः_len = (size_t)(s - वामः);

        /* There's an optional colon between left and right. */
        जइ (s < end && *s == ':')
            s++;

        /* Skip spaces. */
        जाव (s < end && is_space(*s))
            s++;

        /* Get the right value. */
        दक्षिणः = s;
        जाव (s < end && !is_space(*s))
            s++;
        दक्षिणः_len = (size_t)(s - दक्षिणः);

        /* Discard rest of line. */
        जाव (s < end && *s != '\n')
            s++;

        जइ (direction == वामः_TO_दक्षिणः) {
            जइ (वामः_len == name_len && memcmp(वामः, name, वामः_len) == 0) {
                match = strndup(दक्षिणः, दक्षिणः_len);
                भङ्ग;
            }
        } अण्णहा जइ (direction == दक्षिणः_TO_वामः &&
                   दक्षिणः_len == name_len &&
                   memcmp(दक्षिणः, name, दक्षिणः_len) == 0) {
            match = strndup(वामः, वामः_len);
            भङ्ग;
        }
    }

    unmap_file(string, string_size);
    प्रत्यागम match;
}

वर्ण *
resolve_locale(संरचना xkb_context *ctx, ध्रुव वर्ण *locale)
{
    वर्ण *alias = resolve_name(ctx, "locale.alias", वामः_TO_दक्षिणः, locale);
    प्रत्यागम alias ? alias : strdup(locale);
}

वर्ण *
get_xcomposefile_path(संरचना xkb_context *ctx)
{
    प्रत्यागम strdup_safe(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

वर्ण *
get_xdg_xcompose_file_path(संरचना xkb_context *ctx)
{
    ध्रुव वर्ण *xdg_config_home;
    ध्रुव वर्ण *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    जइ (!xdg_config_home || !is_absolute_path(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        जइ (!home)
            प्रत्यागम रिक्त;
        प्रत्यागम asprintf_safe("%s/.config/XCompose", home);
    }

    प्रत्यागम asprintf_safe("%s/XCompose", xdg_config_home);
}

वर्ण *
get_home_xcompose_file_path(संरचना xkb_context *ctx)
{
    ध्रुव वर्ण *home;

    home = xkb_context_getenv(ctx, "HOME");
    जइ (!home)
        प्रत्यागम रिक्त;

    प्रत्यागम asprintf_safe("%s/.XCompose", home);
}

वर्ण *
get_locale_compose_file_path(संरचना xkb_context *ctx, ध्रुव वर्ण *locale)
{
    वर्ण *resolved;
    वर्ण *path;

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
    स्थिर ध्रुव वर्ण * ध्रुव fallback = "en_US.UTF-8";
    स्थिर ध्रुव वर्ण * ध्रुव registry = "compose.dir";
    जइ (streq(locale, "C"))
        locale = fallback;

    resolved = resolve_name(ctx, registry, दक्षिणः_TO_वामः, locale);
    जइ (!resolved) {
#जइ HAVE_NEWLOCALE
        /*
         * There is no extension mechanism for X11 Compose locales.
         * Instead of failing, we just use `en_US.UTF-8` as a fallback because
         * it is what most locales use anyway.
         * But we still want to fail if the locale is invalid in the system,
         * so that we can catch missing system locales and typos.
         */
        locale_t loc = newlocale(LC_ALL_MASK, locale, (locale_t) 0);
        जइ (loc == (locale_t) 0) {
            log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                    "No Compose file for locale \"%s\": "
                    "locale is either invalid or not installed\n", locale);
            प्रत्यागम रिक्त;
        } अण्णहा {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, दक्षिणः_TO_वामः, fallback);
            जइ (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                प्रत्यागम रिक्त;
            } अण्णहा {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#अण्णहा
        प्रत्यागम रिक्त;
#endif
    }

    जइ (is_absolute_path(resolved)) {
        path = resolved;
    }
    अण्णहा {
        ध्रुव वर्ण *xlocaledir = get_xlocaledir_path(ctx);
        path = asprintf_safe("%s/%s", xlocaledir, resolved);
        मुक्त(resolved);
    }

    प्रत्यागम path;
}
