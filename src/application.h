#pragma once

#include <gtk/gtk.h>
#include <adwaita.h>

G_BEGIN_DECLS

#define UPLOAD_TYPE_APPLICATION (upload_application_get_type())

G_DECLARE_FINAL_TYPE(UploadApplication, upload_application, UPLOAD, APPLICATION, AdwApplication)

UploadApplication *upload_application_new(gchar *application_id,
                                          GApplicationFlags flags);

G_END_DECLS