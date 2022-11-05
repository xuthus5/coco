#include "application.h"
#include "window.h"
#include "upload.h"
#include <gtk/gtk.h>
#include <libadwaita-1/adwaita.h>

struct _UploadWindow
{
    AdwApplicationWindow parent_instance;
};

G_DEFINE_TYPE(UploadWindow, upload_window, ADW_TYPE_APPLICATION_WINDOW)

UploadWindow *upload_window_new(gchar *application_id, GApplicationFlags flags)
{
    return g_object_new(UPLOAD_TYPE_WINDOW,
                        "application-id", application_id,
                        "flags", flags,
                        NULL);
}

static void upload_window_class_init(UploadWindowClass *klass)
{
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
    gtk_widget_class_set_template_from_resource(widget_class, "/com/github/xuthus5/uploader/main.ui");
}

static void upload_window_init(UploadWindow *self)
{
    g_type_ensure(UPLOAD_TYPE_UPYUN);
    gtk_widget_init_template(GTK_WIDGET(self));

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(cssProvider, "/com/github/xuthus5/uploader/style.css");
    GdkDisplay *display = gdk_display_get_default();
    gtk_style_context_add_provider_for_display(display, (GtkStyleProvider *)cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}
