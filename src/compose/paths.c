/*
 * Copyright © 2014 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <locale.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "messages-codes.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "util-paths.h"
#অন্তর্ভাব "context.h"
#অন্তর্ভাব "paths.h"

গণনা resolve_name_direction {
    বাম_TO_ডান,
    ডান_TO_বাম,
};

ধ্রুবক অক্ষর *
get_xlocaledir_path(কাঠামো xkb_context *ctx)
{
    ধ্রুবক অক্ষর *dir = xkb_context_getenv(ctx, "XLOCALEDIR");
    যদি (!dir)
        dir = XLOCALEDIR;
    ফেরত dir;
}

/*
 * Files like compose.dir have the format LEFT: RIGHT.  Lookup @name in
 * such a file and return its matching value, according to @direction.
 * @filename is relative to the xlocaledir.
 */
স্থির অক্ষর *
resolve_name(কাঠামো xkb_context *ctx, ধ্রুবক অক্ষর *filename,
             গণনা resolve_name_direction direction, ধ্রুবক অক্ষর *name)
{
    পূর্ণসংখ্যা ret;
    bool ok;
    ধ্রুবক অক্ষর *xlocaledir;
    অক্ষর path[512];
    FILE *file;
    অক্ষর *string;
    আকার_t string_আকার;
    ধ্রুবক অক্ষর *end;
    ধ্রুবক অক্ষর *s, *বাম, *ডান;
    অক্ষর *match;
    আকার_t বাম_len, ডান_len, name_len;

    xlocaledir = get_xlocaledir_path(ctx);

    ret = snprintf(path, আকার(path), "%s/%s", xlocaledir, filename);
    যদি (ret < 0 || (আকার_t) ret >= আকার(path))
        ফেরত ফাঁকা;

    file = fopen(path, "rb");
    যদি (!file)
        ফেরত ফাঁকা;

    ok = map_file(file, &string, &string_আকার);
    fclose(file);
    যদি (!ok)
        ফেরত ফাঁকা;

    s = string;
    end = string + string_আকার;
    name_len = strlen(name);
    match = ফাঁকা;

    যখন (s < end) {
        /* Skip spaces. */
        যখন (s < end && is_space(*s))
            s++;

        /* Skip comments. */
        যদি (s < end && *s == '#') {
            যখন (s < end && *s != '\n')
                s++;
            চালু;
        }

        /* Get the left value. */
        বাম = s;
        যখন (s < end && !is_space(*s) && *s != ':')
            s++;
        বাম_len = (আকার_t)(s - বাম);

        /* There's an optional colon between left and right. */
        যদি (s < end && *s == ':')
            s++;

        /* Skip spaces. */
        যখন (s < end && is_space(*s))
            s++;

        /* Get the right value. */
        ডান = s;
        যখন (s < end && !is_space(*s))
            s++;
        ডান_len = (আকার_t)(s - ডান);

        /* Discard rest of line. */
        যখন (s < end && *s != '\n')
            s++;

        যদি (direction == বাম_TO_ডান) {
            যদি (বাম_len == name_len && memcmp(বাম, name, বাম_len) == 0) {
                match = strndup(ডান, ডান_len);
                বিরতি;
            }
        } অন্যথা যদি (direction == ডান_TO_বাম &&
                   ডান_len == name_len &&
                   memcmp(ডান, name, ডান_len) == 0) {
            match = strndup(বাম, বাম_len);
            বিরতি;
        }
    }

    unmap_file(string, string_আকার);
    ফেরত match;
}

অক্ষর *
resolve_locale(কাঠামো xkb_context *ctx, ধ্রুবক অক্ষর *locale)
{
    অক্ষর *alias = resolve_name(ctx, "locale.alias", বাম_TO_ডান, locale);
    ফেরত alias ? alias : strdup(locale);
}

অক্ষর *
get_xcomposefile_path(কাঠামো xkb_context *ctx)
{
    ফেরত strdup_safe(xkb_context_getenv(ctx, "XCOMPOSEFILE"));
}

অক্ষর *
get_xdg_xcompose_file_path(কাঠামো xkb_context *ctx)
{
    ধ্রুবক অক্ষর *xdg_config_home;
    ধ্রুবক অক্ষর *home;

    xdg_config_home = xkb_context_getenv(ctx, "XDG_CONFIG_HOME");
    যদি (!xdg_config_home || !is_absolute_path(xdg_config_home)) {
        home = xkb_context_getenv(ctx, "HOME");
        যদি (!home)
            ফেরত ফাঁকা;
        ফেরত asprintf_safe("%s/.config/XCompose", home);
    }

    ফেরত asprintf_safe("%s/XCompose", xdg_config_home);
}

অক্ষর *
get_home_xcompose_file_path(কাঠামো xkb_context *ctx)
{
    ধ্রুবক অক্ষর *home;

    home = xkb_context_getenv(ctx, "HOME");
    যদি (!home)
        ফেরত ফাঁকা;

    ফেরত asprintf_safe("%s/.XCompose", home);
}

অক্ষর *
get_locale_compose_file_path(কাঠামো xkb_context *ctx, ধ্রুবক অক্ষর *locale)
{
    অক্ষর *resolved;
    অক্ষর *path;

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
    স্থির ধ্রুবক অক্ষর * ধ্রুবক fallback = "en_US.UTF-8";
    স্থির ধ্রুবক অক্ষর * ধ্রুবক registry = "compose.dir";
    যদি (streq(locale, "C"))
        locale = fallback;

    resolved = resolve_name(ctx, registry, ডান_TO_বাম, locale);
    যদি (!resolved) {
#যদি HAVE_NEWLOCALE
        /*
         * There is no extension mechanism for X11 Compose locales.
         * Instead of failing, we just use `en_US.UTF-8` as a fallback because
         * it is what most locales use anyway.
         * But we still want to fail if the locale is invalid in the system,
         * so that we can catch missing system locales and typos.
         */
        locale_t loc = newlocale(LC_ALL_MASK, locale, (locale_t) 0);
        যদি (loc == (locale_t) 0) {
            log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                    "No Compose file for locale \"%s\": "
                    "locale is either invalid or not installed\n", locale);
            ফেরত ফাঁকা;
        } অন্যথা {
            /*
             * The locale is legit but have no entry in the X11 Compose registry,
             * so use the `en_US.UTF-8` fallback.
             */
            freelocale(loc);
            resolved = resolve_name(ctx, registry, ডান_TO_বাম, fallback);
            যদি (!resolved) {
                log_err(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                        "No Compose file for locale \"%s\": "
                        "failed to use fallback \"%s\"\n", locale, fallback);
                ফেরত ফাঁকা;
            } অন্যথা {
                log_warn(ctx, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                         "No Compose file for locale \"%s\": "
                         "using locale fallback \"%s\"\n", locale, fallback);
            }
        }
#অন্যথা
        ফেরত ফাঁকা;
#endif
    }

    যদি (is_absolute_path(resolved)) {
        path = resolved;
    }
    অন্যথা {
        ধ্রুবক অক্ষর *xlocaledir = get_xlocaledir_path(ctx);
        path = asprintf_safe("%s/%s", xlocaledir, resolved);
        মুক্ত(resolved);
    }

    ফেরত path;
}
