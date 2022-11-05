#include <gtk/gtk.h>
#include <libadwaita-1/adwaita.h>
#include "application.h"
#include "window.h"

int main(int argc, char **argv)
{
    g_autoptr(UploadApplication) app = NULL;
    app = upload_application_new("com.github.xuthus5.coco", G_APPLICATION_FLAGS_NONE);
    int status;

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}