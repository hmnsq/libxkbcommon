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
    सचे (!dir)
        dir = XLOCALEDIR;
    पटिनिवत्त dir;
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
    सचे (ret < 0 || (size_t) ret >= परिमाण(path))
        पटिनिवत्त रिक्त;

    file = fopen(path, "rb");
    सचे (!file)
        पटिनिवत्त रिक्त;

    ok = map_file(file, &string, &string_size);
    fclose(file);
    सचे (!ok)
        पटिनिवत्त रिक्त;

    s = string;
    end = string + string_size;
    name_len = strlen(name);
    match = रिक्त;

    याव (s < end) {
        /* Skip spaces. */
        याव (s < end && is_space(*s))
            s++;

        /* Skip comments. */
        सचे (s < end && *s == '#') {
            याव (s < end && *s != '\n')
                s++;
            अनुवर्त;
        }

        /* Get the left value. */
        वामः = s;
        याव (s < end && !is_space(*s) && *s != ':')
            s++;
        वामः_len = (size_t)(s - वामः);

        /* There's an optional colon between left and right. */
        सचे (s < end && *s == ':')
            s++;

        /* Skip spaces. */
        याव (s < end && is_space(*s))
            s++;

        /* Get the right value. */
        दक्षिणः = s;
        याव (s < end && !is_space(*s))
            s++;
        दक्षिणः_len = (size_t)(s - दक्षिणः);

        /* Discard rest of line. */
        याव (s < end && *s != '\n')
            s++;

        सचे (direction == वामः_TO_दक्षिणः) {
            सचे (वामः_len == name_len && memcmp(वामः, name, वामः_len) == 0) {
                match = strndup(दक्षिणः, दक्षिणः_len);
                भङ्ग;
            }
        } अञ्ञथा सचे (direction == दक्षिणः_TO_वामः &&
                   दक्षिणः_len == name_len &&
                   memcmp(दक्षिणः, name, दक्षिणः_len) == 0) {
            match = strndup(वामः, वामः_len);
            भङ्ग;
        }
    }

    unmap_file(string, string_size);
    पटिनिवत्त match;
}

वर्ण *
resolve_locale(संरचना xkb_context *ctx, ध्रुव वर्ण *locale)
{
    वर्ण *alias = resolve_name(ctx, "locale.alias", वामः_TO_दक्षिणः, locale);
    पटिनिवत्त alias ? alias : strdup(locale);
}

वर्ण *
get_xcomposefile_path(संरचना xkb_context *ctx)
{
    पटिनिवत्त strdup_safe(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

वर्ण *
get_xdg_xcompose_file_path(संरचना xkb_context *ctx)
{
    ध्रुव वर्ण *xdg_config_home;
    ध्रुव वर्ण *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    सचे (!xdg_config_home || !is_absolute_path(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        सचे (!home)
            पटिनिवत्त रिक्त;
        पटिनिवत्त asprintf_safe("%s/.config/XCompose", home);
    }

    पटिनिवत्त asprintf_safe("%s/XCompose", xdg_config_home);
}

वर्ण *
get_home_xcompose_file_path(संरचना xkb_context *ctx)
{
    ध्रुव वर्ण *home;

    home = xkb_context_getenv(ctx, "HOME");
    सचे (!home)
        पटिनिवत्त रिक्त;

    पटिनिवत्त asprintf_safe("%s/.XCompose", home);
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
    सचे (streq(locale, "C"))
        locale = fallback;

    resolved = resolve_name(ctx, registry, दक्षिणः_TO_वामः, locale);
    सचे (!resolved) {
#सचे HAVE_NEWLOCALE
        /*
         * There is no extension mechanism for X11 Compose locales.
         * Instead of failing, we just use `en_US.UTF-8` as a fallback because
         * it is what most locales use anyway.
         * But we still want to fail if the locale is invalid in the system,
         * so that we can catch missing system locales and typos.
         */
        locale_t loc = newlocale(LC_ALL_MASK, locale, (locale_t) 0);
        सचे (loc == (locale_t) 0) {
            log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                    "No Compose file for locale \"%s\": "
                    "locale is either invalid or not installed\n", locale);
            पटिनिवत्त रिक्त;
        } अञ्ञथा {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, दक्षिणः_TO_वामः, fallback);
            सचे (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                पटिनिवत्त रिक्त;
            } अञ्ञथा {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#अञ्ञथा
        पटिनिवत्त रिक्त;
#endif
    }

    सचे (is_absolute_path(resolved)) {
        path = resolved;
    }
    अञ्ञथा {
        ध्रुव वर्ण *xlocaledir = get_xlocaledir_path(ctx);
        path = asprintf_safe("%s/%s", xlocaledir, resolved);
        मुक्त(resolved);
    }

    पटिनिवत्त path;
}
