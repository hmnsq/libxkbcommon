/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل <locale.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "messages-codes.h"
#شامل "utils.h"
#شامل "util-paths.h"
#شامل "context.h"
#شامل "paths.h"

شمارش resolve_name_direction {
    چپ_TO_راست,
    راست_TO_چپ,
};

ثابت نویسه *
get_xlocaledir_path(ساختار xkb_context *ctx)
{
    ثابت نویسه *dir = xkb_context_getenv(ctx, "XLOCALEDIR");
    اگر (!dir)
        dir = XLOCALEDIR;
    بازگشت dir;
}

/*
 * Files like compose.dir have the format LEFT: RIGHT.  Lookup @name in
 * such a file and return its matching value, according to @direction.
 * @filename is relative to the xlocaledir.
 */
ایستا نویسه *
resolve_name(ساختار xkb_context *ctx, ثابت نویسه *filename,
             شمارش resolve_name_direction direction, ثابت نویسه *name)
{
    صحیح ret;
    bool ok;
    ثابت نویسه *xlocaledir;
    نویسه path[512];
    FILE *file;
    نویسه *string;
    size_t string_size;
    ثابت نویسه *end;
    ثابت نویسه *s, *چپ, *راست;
    نویسه *match;
    size_t چپ_len, راست_len, name_len;

    xlocaledir = get_xlocaledir_path(ctx);

    ret = snprintf(path, اندازه(path), "%s/%s", xlocaledir, filename);
    اگر (ret < 0 || (size_t) ret >= اندازه(path))
        بازگشت پوچ;

    file = fopen(path, "rb");
    اگر (!file)
        بازگشت پوچ;

    ok = map_file(file, &string, &string_size);
    fclose(file);
    اگر (!ok)
        بازگشت پوچ;

    s = string;
    end = string + string_size;
    name_len = strlen(name);
    match = پوچ;

    وقتی (s < end) {
        /* Skip spaces. */
        وقتی (s < end && is_space(*s))
            s++;

        /* Skip comments. */
        اگر (s < end && *s == '#') {
            وقتی (s < end && *s != '\n')
                s++;
            ادامه;
        }

        /* Get the left value. */
        چپ = s;
        وقتی (s < end && !is_space(*s) && *s != ':')
            s++;
        چپ_len = (size_t)(s - چپ);

        /* There's an optional colon between left and right. */
        اگر (s < end && *s == ':')
            s++;

        /* Skip spaces. */
        وقتی (s < end && is_space(*s))
            s++;

        /* Get the right value. */
        راست = s;
        وقتی (s < end && !is_space(*s))
            s++;
        راست_len = (size_t)(s - راست);

        /* Discard rest of line. */
        وقتی (s < end && *s != '\n')
            s++;

        اگر (direction == چپ_TO_راست) {
            اگر (چپ_len == name_len && memcmp(چپ, name, چپ_len) == 0) {
                match = strndup(راست, راست_len);
                بشکن;
            }
        } وگرنه اگر (direction == راست_TO_چپ &&
                   راست_len == name_len &&
                   memcmp(راست, name, راست_len) == 0) {
            match = strndup(چپ, چپ_len);
            بشکن;
        }
    }

    unmap_file(string, string_size);
    بازگشت match;
}

نویسه *
resolve_locale(ساختار xkb_context *ctx, ثابت نویسه *locale)
{
    نویسه *alias = resolve_name(ctx, "locale.alias", چپ_TO_راست, locale);
    بازگشت alias ? alias : strdup(locale);
}

نویسه *
get_xcomposefile_path(ساختار xkb_context *ctx)
{
    بازگشت strdup_safe(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

نویسه *
get_xdg_xcompose_file_path(ساختار xkb_context *ctx)
{
    ثابت نویسه *xdg_config_home;
    ثابت نویسه *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    اگر (!xdg_config_home || !is_absolute_path(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        اگر (!home)
            بازگشت پوچ;
        بازگشت asprintf_safe("%s/.config/XCompose", home);
    }

    بازگشت asprintf_safe("%s/XCompose", xdg_config_home);
}

نویسه *
get_home_xcompose_file_path(ساختار xkb_context *ctx)
{
    ثابت نویسه *home;

    home = xkb_context_getenv(ctx, "HOME");
    اگر (!home)
        بازگشت پوچ;

    بازگشت asprintf_safe("%s/.XCompose", home);
}

نویسه *
get_locale_compose_file_path(ساختار xkb_context *ctx, ثابت نویسه *locale)
{
    نویسه *resolved;
    نویسه *path;

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
    ایستا ثابت نویسه * ثابت fallback = "en_US.UTF-8";
    ایستا ثابت نویسه * ثابت registry = "compose.dir";
    اگر (streq(locale, "C"))
        locale = fallback;

    resolved = resolve_name(ctx, registry, راست_TO_چپ, locale);
    اگر (!resolved) {
#اگر HAVE_NEWLOCALE
        /*
         * There is no extension mechanism for X11 Compose locales.
         * Instead of failing, we just use `en_US.UTF-8` as a fallback because
         * it is what most locales use anyway.
         * But we still want to fail if the locale is invalid in the system,
         * so that we can catch missing system locales and typos.
         */
        locale_t loc = newlocale(LC_ALL_MASK, locale, (locale_t) 0);
        اگر (loc == (locale_t) 0) {
            log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                    "No Compose file for locale \"%s\": "
                    "locale is either invalid or not installed\n", locale);
            بازگشت پوچ;
        } وگرنه {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, راست_TO_چپ, fallback);
            اگر (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                بازگشت پوچ;
            } وگرنه {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#وگرنه
        بازگشت پوچ;
#endif
    }

    اگر (is_absolute_path(resolved)) {
        path = resolved;
    }
    وگرنه {
        ثابت نویسه *xlocaledir = get_xlocaledir_path(ctx);
        path = asprintf_safe("%s/%s", xlocaledir, resolved);
        آزاد(resolved);
    }

    بازگشت path;
}
