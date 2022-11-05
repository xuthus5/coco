#include "application.h"
#include "window.h"
#include <adwaita.h>

struct _UploadApplication
{
    AdwApplication parent_instance;
};

G_DEFINE_TYPE(UploadApplication, upload_application, ADW_TYPE_APPLICATION)

UploadApplication *upload_application_new(gchar *application_id, GApplicationFlags flags)
{
    return g_object_new(UPLOAD_TYPE_APPLICATION,
                        "application-id", application_id,
                        "flags", flags,
                        NULL);
}

static void upload_application_activate(GApplication *app)
{
    GtkWindow *window;

    /* It's good practice to check your parameters at the beginning of the
     * function. It helps catch errors early and in development instead of
     * by your users.
     */
    g_assert(GTK_IS_APPLICATION(app));

    /* Get the current window or create one if necessary. */
    window = gtk_application_get_active_window(GTK_APPLICATION(app));
    if (window == NULL)
    {
        window = g_object_new(UPLOAD_TYPE_WINDOW,
                              "application", app,
                              "default-width", 600,
                              "default-height", 400,
                              NULL);
    }
    gtk_window_present(window);
}

static void
upload_application_class_init(UploadApplicationClass *klass)
{
    GApplicationClass *app_class = G_APPLICATION_CLASS(klass);
    app_class->activate = upload_application_activate;
}

static void
upload_application_init(UploadApplication *self)
{
    g_type_ensure(UPLOAD_TYPE_WINDOW);
}
